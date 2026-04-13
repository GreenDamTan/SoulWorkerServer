#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_RuntimeFaultLogger.h"

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

#include <atomic>
#include <csignal>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <exception>
#include <mutex>
#include <new>
#include <string>
#include <vector>

#ifdef _WIN32
#include <crtdbg.h>
#include <windows.h>
#include <dbghelp.h>
#undef min
#undef max
#else
#include <unistd.h>
#endif

namespace {
// 同一进程里只安装一次 runtime fault hook，避免重复叠挂 CRT report hook。
std::atomic_bool g_GreenDamTan_logInstalled{false};
// fault 回调里仍可能再次打日志，这里用最小重入保护避免递归刷爆日志。
std::atomic_flag g_GreenDamTan_logFaultActive = ATOMIC_FLAG_INIT;

const char* GreenDamTan_GetSignalName(int signalNumber) {
#ifdef SIGABRT
    if (signalNumber == SIGABRT) {
        return "SIGABRT";
    }
#endif
#ifdef SIGFPE
    if (signalNumber == SIGFPE) {
        return "SIGFPE";
    }
#endif
#ifdef SIGILL
    if (signalNumber == SIGILL) {
        return "SIGILL";
    }
#endif
#ifdef SIGSEGV
    if (signalNumber == SIGSEGV) {
        return "SIGSEGV";
    }
#endif
    return "SIGUNKNOWN";
}

#ifdef _WIN32
// CRT 回调给的是宽字符串，这里统一转成 UTF-8 再写进现有日志系统。
std::string GreenDamTan_WideToUtf8(const wchar_t* text) {
    if (!text || !*text) {
        return {};
    }

    const int required = WideCharToMultiByte(CP_UTF8, 0, text, -1, nullptr, 0, nullptr, nullptr);
    if (required <= 1) {
        return {};
    }

    std::string result(static_cast<std::size_t>(required) - 1, '\0');
    WideCharToMultiByte(CP_UTF8,
                        0,
                        text,
                        -1,
                        result.data(),
                        required,
                        nullptr,
                        nullptr);
    return result;
}

// 统一把 CRT report type 转成稳定字符串，便于日志搜索和后续对照。
const char* GreenDamTan_GetCrtReportTypeName(int reportType) {
    switch (reportType) {
    case _CRT_WARN:
        return "_CRT_WARN";
    case _CRT_ERROR:
        return "_CRT_ERROR";
    case _CRT_ASSERT:
        return "_CRT_ASSERT";
    default:
        return "_CRT_UNKNOWN";
    }
}

constexpr unsigned long kGreenDamTanStatusCppException = 0xE06D7363UL;
constexpr unsigned long kGreenDamTanStatusMsVcThreadName = 0x406D1388UL;
constexpr unsigned long kGreenDamTanStatusHeapCorruption = 0xC0000374UL;
constexpr unsigned long kGreenDamTanStatusStackBufferOverrun = 0xC0000409UL;
constexpr unsigned long kGreenDamTanStatusFailFastException = 0xC0000602UL;

std::atomic_bool g_GreenDamTan_logSymbolsReady{false};
std::mutex g_GreenDamTan_logSymbolLock;

struct GreenDamTan_SymbolizedFrame {
    void* address = nullptr;
    std::string function;
    std::string file;
    unsigned long line = 0;
    unsigned long long displacement = 0;
};

bool GreenDamTan_EnsureSymbolsInitialized() {
    if (g_GreenDamTan_logSymbolsReady.load()) {
        return true;
    }

    std::lock_guard<std::mutex> lock(g_GreenDamTan_logSymbolLock);
    if (g_GreenDamTan_logSymbolsReady.load()) {
        return true;
    }

    HANDLE processHandle = GetCurrentProcess();
    SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME |
                  SYMOPT_FAIL_CRITICAL_ERRORS);
    if (!SymInitialize(processHandle, nullptr, TRUE)) {
        return false;
    }

    g_GreenDamTan_logSymbolsReady = true;
    return true;
}

GreenDamTan_SymbolizedFrame GreenDamTan_SymbolizeAddress(void* address) {
    GreenDamTan_SymbolizedFrame frame{};
    frame.address = address;
    if (!address || !GreenDamTan_EnsureSymbolsInitialized()) {
        return frame;
    }

    std::lock_guard<std::mutex> lock(g_GreenDamTan_logSymbolLock);
    HANDLE processHandle = GetCurrentProcess();
    const DWORD64 rawAddress = reinterpret_cast<DWORD64>(address);

    unsigned char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME] = {};
    SYMBOL_INFO* symbolInfo = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);
    symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbolInfo->MaxNameLen = MAX_SYM_NAME;
    DWORD64 displacement = 0;
    if (SymFromAddr(processHandle, rawAddress, &displacement, symbolInfo)) {
        frame.function.assign(symbolInfo->Name, symbolInfo->NameLen);
        frame.displacement = static_cast<unsigned long long>(displacement);
    }

    IMAGEHLP_LINE64 lineInfo{};
    lineInfo.SizeOfStruct = sizeof(lineInfo);
    DWORD lineDisplacement = 0;
    if (SymGetLineFromAddr64(processHandle, rawAddress, &lineDisplacement, &lineInfo)) {
        if (lineInfo.FileName) {
            frame.file = lineInfo.FileName;
        }
        frame.line = lineInfo.LineNumber;
    }

    return frame;
}

bool GreenDamTan_IsLikelyProjectFrame(const GreenDamTan_SymbolizedFrame& frame) {
    if (frame.file.empty()) {
        return false;
    }

    std::string lowered = frame.file;
    for (char& ch : lowered) {
        if (ch >= 'A' && ch <= 'Z') {
            ch = static_cast<char>(ch - 'A' + 'a');
        }
    }

    return lowered.find("\\server\\src\\") != std::string::npos ||
           lowered.find("/server/src/") != std::string::npos;
}

bool GreenDamTan_IsInternalLoggerFrame(const GreenDamTan_SymbolizedFrame& frame) {
    if (frame.function.find("GreenDamTan_") != std::string::npos) {
        return true;
    }

    if (frame.file.empty()) {
        return false;
    }

    std::string lowered = frame.file;
    for (char& ch : lowered) {
        if (ch >= 'A' && ch <= 'Z') {
            ch = static_cast<char>(ch - 'A' + 'a');
        }
    }

    return lowered.find("greendamtan_runtimefaultlogger.cpp") != std::string::npos;
}

void GreenDamTan_LogFrameSummary(const char* prefix,
                                 const GreenDamTan_SymbolizedFrame& frame);
void GreenDamTan_LogCurrentStack(const char* reason, unsigned short framesToSkip);
#endif

// 所有 fault 回调最终都收口到这里，保证 stderr 和文件日志能看到同一条诊断信息。
void GreenDamTan_LogProcessFault(const char* format, ...) noexcept {
    char message[2048] = {};
    va_list args;
    va_start(args, format);
    std::vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    if (message[0] == '\0') {
        return;
    }

    std::fprintf(stderr, "%s\n", message);
    std::fflush(stderr);

    const bool firstEntry = !g_GreenDamTan_logFaultActive.test_and_set();
    if (!firstEntry) {
        return;
    }

    try {
        LogHelper::LogError("game.system", "%s", message);
    } catch (...) {
    }

    g_GreenDamTan_logFaultActive.clear();
}

#ifdef _WIN32
void GreenDamTan_LogFrameSummary(const char* prefix,
                                 const GreenDamTan_SymbolizedFrame& frame) {
    const char* functionName = frame.function.empty() ? "<unknown>" : frame.function.c_str();
    const char* fileName = frame.file.empty() ? "<unknown>" : frame.file.c_str();
    GreenDamTan_LogProcessFault(
        "GreenDamTan_log %s address=%p function=%s file=%s line=%lu displacement=0x%llX",
        prefix ? prefix : "frame",
        frame.address,
        functionName,
        fileName,
        frame.line,
        frame.displacement);
}

void GreenDamTan_LogCurrentStack(const char* reason, unsigned short framesToSkip) {
    void* frames[16] = {};
    const USHORT frameCount =
        CaptureStackBackTrace(static_cast<DWORD>(framesToSkip + 1), 16, frames, nullptr);
    if (frameCount == 0) {
        GreenDamTan_LogProcessFault("GreenDamTan_log stack capture failed reason=%s",
                                    reason ? reason : "unknown");
        return;
    }

    std::vector<GreenDamTan_SymbolizedFrame> resolvedFrames;
    resolvedFrames.reserve(frameCount);
    for (USHORT index = 0; index < frameCount; ++index) {
        resolvedFrames.push_back(GreenDamTan_SymbolizeAddress(frames[index]));
    }

    const GreenDamTan_SymbolizedFrame* probableFrame = nullptr;
    for (const GreenDamTan_SymbolizedFrame& frame : resolvedFrames) {
        if (!GreenDamTan_IsLikelyProjectFrame(frame) || GreenDamTan_IsInternalLoggerFrame(frame)) {
            continue;
        }
        probableFrame = &frame;
        break;
    }
    if (!probableFrame) {
        for (const GreenDamTan_SymbolizedFrame& frame : resolvedFrames) {
            if (frame.file.empty() || GreenDamTan_IsInternalLoggerFrame(frame)) {
                continue;
            }
            probableFrame = &frame;
            break;
        }
    }
    if (!probableFrame) {
        for (const GreenDamTan_SymbolizedFrame& frame : resolvedFrames) {
            if (frame.function.empty() || GreenDamTan_IsInternalLoggerFrame(frame)) {
                continue;
            }
            probableFrame = &frame;
            break;
        }
    }
    if (probableFrame) {
        GreenDamTan_LogFrameSummary("probable crash site", *probableFrame);
    }

    for (USHORT index = 0; index < frameCount; ++index) {
        const GreenDamTan_SymbolizedFrame& frame = resolvedFrames[index];
        const char* functionName = frame.function.empty() ? "<unknown>" : frame.function.c_str();
        const char* fileName = frame.file.empty() ? "<unknown>" : frame.file.c_str();
        GreenDamTan_LogProcessFault(
            "GreenDamTan_log stack[%u] reason=%s address=%p function=%s file=%s line=%lu displacement=0x%llX",
            static_cast<unsigned int>(index),
            reason ? reason : "unknown",
            frame.address,
            functionName,
            fileName,
            frame.line,
            frame.displacement);
    }
}

const char* GreenDamTan_GetExceptionCodeName(unsigned long exceptionCode) {
    switch (exceptionCode) {
    case EXCEPTION_ACCESS_VIOLATION:
        return "EXCEPTION_ACCESS_VIOLATION";
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
    case EXCEPTION_BREAKPOINT:
        return "EXCEPTION_BREAKPOINT";
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        return "EXCEPTION_DATATYPE_MISALIGNMENT";
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        return "EXCEPTION_FLT_DENORMAL_OPERAND";
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
    case EXCEPTION_FLT_INEXACT_RESULT:
        return "EXCEPTION_FLT_INEXACT_RESULT";
    case EXCEPTION_FLT_INVALID_OPERATION:
        return "EXCEPTION_FLT_INVALID_OPERATION";
    case EXCEPTION_FLT_OVERFLOW:
        return "EXCEPTION_FLT_OVERFLOW";
    case EXCEPTION_FLT_STACK_CHECK:
        return "EXCEPTION_FLT_STACK_CHECK";
    case EXCEPTION_FLT_UNDERFLOW:
        return "EXCEPTION_FLT_UNDERFLOW";
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        return "EXCEPTION_ILLEGAL_INSTRUCTION";
    case EXCEPTION_IN_PAGE_ERROR:
        return "EXCEPTION_IN_PAGE_ERROR";
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        return "EXCEPTION_INT_DIVIDE_BY_ZERO";
    case EXCEPTION_INT_OVERFLOW:
        return "EXCEPTION_INT_OVERFLOW";
    case EXCEPTION_INVALID_DISPOSITION:
        return "EXCEPTION_INVALID_DISPOSITION";
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
    case EXCEPTION_PRIV_INSTRUCTION:
        return "EXCEPTION_PRIV_INSTRUCTION";
    case EXCEPTION_SINGLE_STEP:
        return "EXCEPTION_SINGLE_STEP";
    case EXCEPTION_STACK_OVERFLOW:
        return "EXCEPTION_STACK_OVERFLOW";
    case kGreenDamTanStatusCppException:
        return "MSVC_CPP_EXCEPTION";
    case kGreenDamTanStatusMsVcThreadName:
        return "MSVC_THREAD_NAME";
    case kGreenDamTanStatusHeapCorruption:
        return "STATUS_HEAP_CORRUPTION";
    case kGreenDamTanStatusStackBufferOverrun:
        return "STATUS_STACK_BUFFER_OVERRUN";
    case kGreenDamTanStatusFailFastException:
        return "STATUS_FAIL_FAST_EXCEPTION";
    default:
        return "UNKNOWN_EXCEPTION";
    }
}

const char* GreenDamTan_GetAccessOperationName(ULONG_PTR accessType) {
    switch (accessType) {
    case 0:
        return "read";
    case 1:
        return "write";
    case 8:
        return "execute";
    default:
        return "unknown";
    }
}

bool GreenDamTan_ShouldLogVectoredException(unsigned long exceptionCode) {
    switch (exceptionCode) {
    case EXCEPTION_STACK_OVERFLOW:
    case kGreenDamTanStatusHeapCorruption:
    case kGreenDamTanStatusStackBufferOverrun:
    case kGreenDamTanStatusFailFastException:
        return true;
    default:
        return false;
    }
}

void GreenDamTan_LogExceptionContext(const CONTEXT* context) {
    if (!context) {
        return;
    }

#if defined(_M_X64)
    GreenDamTan_LogProcessFault(
        "GreenDamTan_log exception context rip=0x%llX rsp=0x%llX rbp=0x%llX rax=0x%llX rbx=0x%llX rcx=0x%llX rdx=0x%llX",
        static_cast<unsigned long long>(context->Rip),
        static_cast<unsigned long long>(context->Rsp),
        static_cast<unsigned long long>(context->Rbp),
        static_cast<unsigned long long>(context->Rax),
        static_cast<unsigned long long>(context->Rbx),
        static_cast<unsigned long long>(context->Rcx),
        static_cast<unsigned long long>(context->Rdx));
#elif defined(_M_IX86)
    GreenDamTan_LogProcessFault(
        "GreenDamTan_log exception context eip=0x%08lX esp=0x%08lX ebp=0x%08lX eax=0x%08lX ebx=0x%08lX ecx=0x%08lX edx=0x%08lX",
        static_cast<unsigned long>(context->Eip),
        static_cast<unsigned long>(context->Esp),
        static_cast<unsigned long>(context->Ebp),
        static_cast<unsigned long>(context->Eax),
        static_cast<unsigned long>(context->Ebx),
        static_cast<unsigned long>(context->Ecx),
        static_cast<unsigned long>(context->Edx));
#else
    (void)context;
#endif
}

void GreenDamTan_LogExceptionDetails(const char* source,
                                     EXCEPTION_POINTERS* exceptionPointers,
                                     bool includeStack) {
    const EXCEPTION_RECORD* exceptionRecord =
        exceptionPointers ? exceptionPointers->ExceptionRecord : nullptr;
    const CONTEXT* contextRecord = exceptionPointers ? exceptionPointers->ContextRecord : nullptr;

    unsigned long exceptionCode = 0;
    unsigned long exceptionFlags = 0;
    void* exceptionAddress = nullptr;
    if (exceptionRecord) {
        exceptionCode = exceptionRecord->ExceptionCode;
        exceptionFlags = exceptionRecord->ExceptionFlags;
        exceptionAddress = exceptionRecord->ExceptionAddress;
    }

    GreenDamTan_LogProcessFault(
        "GreenDamTan_log %s code=%s(0x%08lX) flags=0x%08lX address=%p",
        source ? source : "exception",
        GreenDamTan_GetExceptionCodeName(exceptionCode),
        exceptionCode,
        exceptionFlags,
        exceptionAddress);

    if (exceptionRecord &&
        (exceptionCode == EXCEPTION_ACCESS_VIOLATION || exceptionCode == EXCEPTION_IN_PAGE_ERROR) &&
        exceptionRecord->NumberParameters >= 2) {
        GreenDamTan_LogProcessFault(
            "GreenDamTan_log %s memory access operation=%s target=%p",
            source ? source : "exception",
            GreenDamTan_GetAccessOperationName(exceptionRecord->ExceptionInformation[0]),
            reinterpret_cast<void*>(exceptionRecord->ExceptionInformation[1]));
    }

    if (exceptionRecord &&
        (exceptionCode == kGreenDamTanStatusStackBufferOverrun ||
         exceptionCode == kGreenDamTanStatusFailFastException) &&
        exceptionRecord->NumberParameters >= 1) {
        GreenDamTan_LogProcessFault(
            "GreenDamTan_log %s failfast subcode=0x%llX",
            source ? source : "exception",
            static_cast<unsigned long long>(exceptionRecord->ExceptionInformation[0]));
    }

    GreenDamTan_LogFrameSummary("exception site", GreenDamTan_SymbolizeAddress(exceptionAddress));
    GreenDamTan_LogExceptionContext(contextRecord);
    if (includeStack) {
        GreenDamTan_LogCurrentStack(source ? source : "exception", 1);
    }
}

LONG CALLBACK GreenDamTan_LogVectoredException(EXCEPTION_POINTERS* exceptionPointers) {
    const EXCEPTION_RECORD* exceptionRecord =
        exceptionPointers ? exceptionPointers->ExceptionRecord : nullptr;
    if (!exceptionRecord || !GreenDamTan_ShouldLogVectoredException(exceptionRecord->ExceptionCode)) {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    GreenDamTan_LogExceptionDetails("vectored exception", exceptionPointers, true);
    return EXCEPTION_CONTINUE_SEARCH;
}
#endif

// 所有进程级 fatal signal 都走同一条日志入口，尽量先把信号名和栈记下来再交回默认处理。
void GreenDamTan_LogSignal(int signalNumber) {
    GreenDamTan_LogProcessFault("GreenDamTan_log signal handler caught signal=%s(%d)",
                                GreenDamTan_GetSignalName(signalNumber),
                                signalNumber);
#ifdef _WIN32
    GreenDamTan_LogCurrentStack("signal", 1);
#endif
    std::signal(signalNumber, SIG_DFL);
    std::raise(signalNumber);
    std::_Exit(EXIT_FAILURE);
}

// terminate 往往意味着异常已经越过业务边界，这里尽量把 current_exception 内容捞出来。
void GreenDamTan_LogTerminate() {
    try {
        if (std::exception_ptr current = std::current_exception()) {
            try {
                std::rethrow_exception(current);
            } catch (const std::exception& exception) {
                GreenDamTan_LogProcessFault(
                    "GreenDamTan_log terminate handler caught std::exception what=%s",
                    exception.what());
#ifdef _WIN32
                GreenDamTan_LogCurrentStack("terminate std::exception", 1);
#endif
            } catch (...) {
                GreenDamTan_LogProcessFault(
                    "GreenDamTan_log terminate handler caught non-std exception");
#ifdef _WIN32
                GreenDamTan_LogCurrentStack("terminate non-std exception", 1);
#endif
            }
        } else {
            GreenDamTan_LogProcessFault(
                "GreenDamTan_log terminate handler called without current_exception");
#ifdef _WIN32
            GreenDamTan_LogCurrentStack("terminate without current_exception", 1);
#endif
        }
    } catch (...) {
        GreenDamTan_LogProcessFault(
            "GreenDamTan_log terminate handler failed while probing current_exception");
    }

    std::abort();
}

// operator new 失败时默认只会不断重试 new_handler；这里先记日志，再显式抛 bad_alloc 交回上层。
void GreenDamTan_LogNewFailure() {
    GreenDamTan_LogProcessFault("GreenDamTan_log new handler allocation failed");
#ifdef _WIN32
    GreenDamTan_LogCurrentStack("new handler", 1);
#endif
    throw std::bad_alloc();
}

#ifdef _WIN32
// pure virtual 被错误调用时，先把 fault 点落日志，再让 CRT 按原路径继续处理。
void __cdecl GreenDamTan_LogPurecall() {
    GreenDamTan_LogProcessFault("GreenDamTan_log purecall handler invoked");
    GreenDamTan_LogCurrentStack("purecall", 1);
}

// CRT 的 debug error / assert 文本会先走 report hook；这里尽量在 abort 之前把正文记下来。
int __cdecl GreenDamTan_LogCrtReport(int reportType, wchar_t* message, int* /*returnValue*/) {
    const std::string messageUtf8 = GreenDamTan_WideToUtf8(message);
    GreenDamTan_LogProcessFault("GreenDamTan_log CRT report type=%s(%d) message=%s",
                                GreenDamTan_GetCrtReportTypeName(reportType),
                                reportType,
                                messageUtf8.empty() ? "<empty>" : messageUtf8.c_str());
    if (reportType != _CRT_WARN) {
        GreenDamTan_LogCurrentStack("crt report", 1);
    }
    return FALSE;
}

// 参数校验失败通常会直接弹 CRT debug error，这里把表达式、函数、文件、行号都写出来。
void __cdecl GreenDamTan_LogInvalidParameter(const wchar_t* expression,
                                             const wchar_t* function,
                                             const wchar_t* file,
                                             unsigned int line,
                                             std::uintptr_t /*reserved*/) {
    const std::string expressionUtf8 = GreenDamTan_WideToUtf8(expression);
    const std::string functionUtf8 = GreenDamTan_WideToUtf8(function);
    const std::string fileUtf8 = GreenDamTan_WideToUtf8(file);
    GreenDamTan_LogProcessFault(
        "GreenDamTan_log invalid parameter expression=%s function=%s file=%s line=%u",
        expressionUtf8.empty() ? "<null>" : expressionUtf8.c_str(),
        functionUtf8.empty() ? "<null>" : functionUtf8.c_str(),
        fileUtf8.empty() ? "<null>" : fileUtf8.c_str(),
        line);
    GreenDamTan_LogCurrentStack("invalid parameter", 1);
}

// Win32 顶层异常兜底：即使没走到 C++/CRT 诊断链，也尽量保住异常码和地址。
LONG WINAPI GreenDamTan_LogUnhandledException(EXCEPTION_POINTERS* exceptionPointers) {
    GreenDamTan_LogExceptionDetails("unhandled exception", exceptionPointers, true);
    return EXCEPTION_EXECUTE_HANDLER;
}
#else
// Linux 侧当前没有 Win32 SEH；先保留最小占位日志，避免静默吞掉 reconstructed dump hook。
long GreenDamTan_LogUnhandledException(void* /*exceptionPointers*/) {
    GreenDamTan_LogProcessFault(
        "GreenDamTan_log unhandled exception captured by reconstructed dump hook");
    return 0;
}
#endif
}

#if defined(ENABLE_UBSAN)
extern "C" const char* __ubsan_default_options() {
#if defined(UBSAN_PRINT_STACKTRACE)
#if defined(UBSAN_SYMBOLIZER_PATH)
    return "print_stacktrace=1:halt_on_error=1:external_symbolizer_path="
           UBSAN_SYMBOLIZER_PATH;
#else
    return "print_stacktrace=1:halt_on_error=1";
#endif
#else
    return "halt_on_error=1";
#endif
}
#endif

void GreenDamTan_RuntimeFaultLogger::Install() {
    if (g_GreenDamTan_logInstalled.exchange(true)) {
        return;
    }

    // 先把平台无关的 terminate / abort 路径接住，保证最差情况也能看到尾段 fault。
    std::set_terminate(GreenDamTan_LogTerminate);
    std::set_new_handler(GreenDamTan_LogNewFailure);
#ifdef SIGABRT
    std::signal(SIGABRT, GreenDamTan_LogSignal);
#endif
#ifdef SIGFPE
    std::signal(SIGFPE, GreenDamTan_LogSignal);
#endif
#ifdef SIGILL
    std::signal(SIGILL, GreenDamTan_LogSignal);
#endif
#ifdef SIGSEGV
    std::signal(SIGSEGV, GreenDamTan_LogSignal);
#endif
#ifdef _WIN32
    // Windows CRT / SEH 侧继续补 report / purecall / invalid parameter / vectored exception。
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    GreenDamTan_EnsureSymbolsInitialized();
    _set_purecall_handler(GreenDamTan_LogPurecall);
    _set_invalid_parameter_handler(GreenDamTan_LogInvalidParameter);
    _CrtSetReportHookW2(_CRT_RPTHOOK_INSTALL, GreenDamTan_LogCrtReport);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    if (!AddVectoredExceptionHandler(1, GreenDamTan_LogVectoredException)) {
        GreenDamTan_LogProcessFault(
            "GreenDamTan_log AddVectoredExceptionHandler failed error=%lu",
            static_cast<unsigned long>(GetLastError()));
    }
#endif
}

void GreenDamTan_RuntimeFaultLogger::LogReady(const char* stage) {
    // 这条日志放在 StartLog() 之后打，表示后续 fault 诊断已经能稳定落到文件。
    LogHelper::LogInfo("game.system",
                       "GreenDamTan_log runtime fault hooks armed stage=%s pid=%lu",
                       stage ? stage : "unknown",
#ifdef _WIN32
                       static_cast<unsigned long>(GetCurrentProcessId())
#else
                       static_cast<unsigned long>(::getpid())
#endif
    );
}

GreenDamTan_RuntimeFaultLogger::UnhandledExceptionFilter
GreenDamTan_RuntimeFaultLogger::InstallUnhandledExceptionFilter() {
#ifdef _WIN32
    // 返回旧 filter 给外层保存，停机时再恢复，避免影响其它全局异常处理器。
    return SetUnhandledExceptionFilter(GreenDamTan_LogUnhandledException);
#else
    return nullptr;
#endif
}

void GreenDamTan_RuntimeFaultLogger::RestoreUnhandledExceptionFilter(
    UnhandledExceptionFilter previousFilter) {
#ifdef _WIN32
    // 服务退出时把旧 filter 挂回去，保持入口层对 Win32 全局状态的修改可逆。
    SetUnhandledExceptionFilter(previousFilter);
#else
    (void)previousFilter;
#endif
}
