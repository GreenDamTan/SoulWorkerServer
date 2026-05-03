// DBAgent ServerMain - 入口点恢复
// Per IDA 0x140010080: main 函数

#include "Soulworker/GameServer/XDBAgent/DBAgent.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_RuntimeFaultLogger.h"

#include <chrono>
#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
constexpr char Locale[] = "";

std::uint64_t ReadAutoShutdownMs() {
    // TODO: 仅做测试用：便于还原工程在自动验证时退出，不代表原版 DBAgent 行为。
    const char* value = std::getenv("GREENDAMTAN_AUTOSTOP_MS");
    if (!value || !*value) {
        return 0;
    }

    char* end = nullptr;
    const unsigned long long parsed = std::strtoull(value, &end, 10);
    if (!end || *end != '\0') {
        return 0;
    }

    return static_cast<std::uint64_t>(parsed);
}
}

bool ServiceInit(int argc, char** argv) {
    // 简化实现：当前仅保留参数探测，不进入真实服务模式。
    for (int i = 1; i < argc; ++i) {
        if (argv[i] && argv[i][0] == '-') {
            GreenDamTan_log(__FILE__, __FUNCTION__, "ServiceInit arg detected");
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    std::setlocale(LC_ALL, Locale);
    GreenDamTan_RuntimeFaultLogger::Install();

#ifdef _WIN32
    GreenDamTan_RuntimeFaultLogger::UnhandledExceptionFilter previousFilter =
        GreenDamTan_RuntimeFaultLogger::InstallUnhandledExceptionFilter();
#endif

    const std::uint64_t autoShutdownMs = ReadAutoShutdownMs();

    if (argc > 1) {
#ifdef _WIN32
        GreenDamTan_RuntimeFaultLogger::RestoreUnhandledExceptionFilter(previousFilter);
#endif
        return ServiceInit(argc, argv) ? 1 : 0;
    }

    XDBAgent* pDBAgent = TXSingleton<XDBAgent>::Instance();
    pDBAgent->StartLog();
    GreenDamTan_RuntimeFaultLogger::LogReady("console");

    int exitCode = 0;
    if (autoShutdownMs > 0) {
        if (!pDBAgent->Init()) {
            exitCode = 1;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(autoShutdownMs));
            pDBAgent->Shutdown(0xFFFFFFFFu);
        }
    } else {
        pDBAgent->Run();
    }

#ifdef _WIN32
    GreenDamTan_RuntimeFaultLogger::RestoreUnhandledExceptionFilter(previousFilter);
#endif

    return exitCode;
}