// ServerMain.cpp
// GameServer 进程入口
// 对齐 IDA: main @ 0x1406CE570

#define _CRT_SECURE_NO_WARNINGS
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_RuntimeFaultLogger.h"
#include <clocale>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>  // _getch
#endif

namespace {
constexpr char Locale[] = "";
}

// 对齐 IDA: 全局服务名称
static char g_serviceName[256] = "GameServer";

// 对齐 IDA: ServiceMain 前置声明
#ifdef _WIN32
static void WINAPI ServiceMain(unsigned long argc, char** argv);
#endif

// 对齐 IDA: ErrorCode2String 前置声明
extern bool ErrorCode2String(void** ppMsgBuf, unsigned long dwErrorCode, int nLangID, unsigned int dwFlags);

// 对齐 IDA: InstallService 前置声明
extern bool InstallService(const char* szModulePath, const char* szServiceName, unsigned long dwServiceType);

// 对齐 IDA: UninstallService 前置声明
extern bool UninstallService(const char* szServiceName);

// 对齐 IDA: ?ServiceInit@@YA_NKPEAPEAD@Z @ 0x1406CE880
// 服务初始化函数，处理命令行参数
bool ServiceInit(unsigned long argc, char** argv) {
#ifdef _WIN32
    char moduleName[272];
    char drive[16];
    char directory[256];
    char fileName[256];
    char ext[256];

    GetModuleFileNameA(nullptr, moduleName, 0x104u);
    _splitpath(moduleName, drive, directory, fileName, ext);

    if (argc < 2) {
        printf("USAGE:\n");
        printf("%s [/INSTALL | /UNINSTALL | /TEST]\n\n", fileName);
        printf("    /INSTALL   Creates a service object and adds it to the specified service\n");
        printf("               control manager database.\n");
        printf("    OPTION     -DEF NAME Uses the name specified during installation.\n");
        printf("    /UNINSTALL Marks the specified service for deletion from the service\n");
        printf("               control manager database.\n");
        printf("    OPTION     -DEF NAME While deleting the specified name is used.\n");
        printf("    /INSERT    Upload script files to MSSQL Database.\n");
        printf("    /TEST      Running the Server Program in Console mode.\n");
        printf("    OPTIONS    -VERBOSE  Display Debug Information.\n");
        printf("               -PACKET   Display Network Information.\n");
        printf("               -DEF NAME Uses the name specified during execution.\n");
        return true;
    }

    // 对齐 IDA: argc == 3 && !stricmp(argv[1], "-def")
    if (argc == 3 && _stricmp(argv[1], "-def") == 0) {
        // 复制服务名称
        strcpy_s(g_serviceName, sizeof(g_serviceName), argv[2]);

        // 对齐 IDA: SERVICE_TABLE_ENTRYA DispatchTable[2]
        SERVICE_TABLE_ENTRYA dispatchTable[2];
        dispatchTable[0].lpServiceName = g_serviceName;
        dispatchTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONA)ServiceMain;
        dispatchTable[1].lpServiceName = nullptr;
        dispatchTable[1].lpServiceProc = nullptr;

        printf("\n");
        printf("StartServiceCtrlDispatcher being called.\n");
        printf("This may take several seconds.  Please wait.\n");

        if (!StartServiceCtrlDispatcherA(dispatchTable)) {
            void* msgBuf = nullptr;
            DWORD lastError = GetLastError();
            if (ErrorCode2String(&msgBuf, lastError, 0, 1u)) {
                DWORD err = GetLastError();
                printf("\nStartServiceCtrlDispatcher (%d): %s\n", err, (const char*)msgBuf);
                LocalFree(msgBuf);
            }
        }
        return true;
    }

    // 对齐 IDA: stricmp(argv[1], "/VER")
    if (_stricmp(argv[1], "/VER") != 0) {
        // 对齐 IDA: /INSTALL
        if (_stricmp(argv[1], "/INSTALL") == 0) {
            if (argc == 4 && _stricmp(argv[2], "-DEF") == 0) {
                printf("\nInstallation to your service is initiated.\n");
                if (InstallService(moduleName, argv[3], 2u)) {
                    printf("\nInstallation to your service is completed.\n");
                } else {
                    printf("\nInstallation to your service is not completed.\n");
                }
            } else {
                printf("USAGE:\n");
                printf("    /INSTALL   Creates a service object and adds it to the specified service\n");
                printf("               control manager database.\n");
                printf("    OPTION     -DEF NAME  Uses the name specified during installation.");
            }
            _getch();
            return true;
        }

        // 对齐 IDA: /UNINSTALL
        if (_stricmp(argv[1], "/UNINSTALL") == 0) {
            if (argc == 4 && _stricmp(argv[2], "-DEF") == 0) {
                printf("\nDeleting your service is initiated.\n");
                if (UninstallService(argv[3])) {
                    printf("\nDeleting your service is completed.\n");
                } else {
                    printf("\nDeleting our service is not completed.\n");
                }
            } else {
                printf("USAGE:\n");
                printf("    /UNINSTALL Marks the specified service for deletion from the service\n");
                printf("               control manager database.\n");
                printf("    OPTION     -DEF NAME  While deleting the specified name is used.");
            }
            _getch();
            return true;
        }
    }
#else
    // Linux 简化实现
    for (unsigned long i = 1; i < argc; ++i) {
        if (argv[i] && argv[i][0] == '-') {
            GreenDamTan_log(__FILE__, __FUNCTION__, "ServiceInit arg detected");
        }
    }
#endif
    return false;
}

// 对齐 IDA: main @ 0x1406CE570
int main(int argc, char* argv[]) {
    // 对齐 IDA: setlocale(0, &Locale)
    std::setlocale(LC_ALL, Locale);

#ifdef _WIN32
    // 对齐 IDA: SetUnhandledExceptionFilter(XDump::XGenerateDump_Popup)
    GreenDamTan_RuntimeFaultLogger::UnhandledExceptionFilter previousFilter =
        GreenDamTan_RuntimeFaultLogger::InstallUnhandledExceptionFilter();

    // 对齐 IDA: CAPIHook 构造函数调用
    // 注意: CAPIHook 用于拦截 SetUnhandledExceptionFilter 调用
    // 当前简化实现: 直接使用 GreenDamTan_RuntimeFaultLogger
#endif

    // 对齐 IDA: if (argc <= 1) 正常启动，否则 ServiceInit
    if (argc <= 1) {
        // 对齐 IDA: TXSingleton<XGameServer>::Instance()
        XGameServer* pServer = XGameServer::Instance();

        // 对齐 IDA: XIOCPServer::StartLog(v4)
        pServer->StartLog();

        // 对齐 IDA: XServer::Run(v5)
        pServer->Run();

#ifdef _WIN32
        // 对齐 IDA: SetUnhandledExceptionFilter(previousFilter)
        GreenDamTan_RuntimeFaultLogger::RestoreUnhandledExceptionFilter(previousFilter);
#endif
        return 0;
    } else {
        // 对齐 IDA: ServiceInit(argc, argv)
        bool bResult = ServiceInit(static_cast<unsigned long>(argc), argv);

#ifdef _WIN32
        // 对齐 IDA: CAPIHook 析构函数调用
        GreenDamTan_RuntimeFaultLogger::RestoreUnhandledExceptionFilter(previousFilter);
#endif
        return bResult ? 0 : 1;
    }
}

#ifdef _WIN32
// 对齐 IDA: ServiceMain 函数
static void WINAPI ServiceMain(unsigned long argc, char** argv) {
    // 简化实现: 调用正常启动流程
    XGameServer* pServer = XGameServer::Instance();
    pServer->StartLog();
    pServer->Run();
}
#endif

// 简化实现: InstallService
bool InstallService(const char* szModulePath, const char* szServiceName, unsigned long dwServiceType) {
#ifdef _WIN32
    SC_HANDLE hSCManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (!hSCManager) return false;

    SC_HANDLE hService = CreateServiceA(
        hSCManager,
        szServiceName,
        szServiceName,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        szModulePath,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    CloseServiceHandle(hSCManager);
    if (!hService) return false;
    CloseServiceHandle(hService);
#endif
    return true;
}

// 简化实现: UninstallService
bool UninstallService(const char* szServiceName) {
#ifdef _WIN32
    SC_HANDLE hSCManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (!hSCManager) return false;

    SC_HANDLE hService = OpenServiceA(hSCManager, szServiceName, DELETE);
    CloseServiceHandle(hSCManager);
    if (!hService) return false;

    bool bResult = DeleteService(hService) != FALSE;
    CloseServiceHandle(hService);
    return bResult;
#else
    return true;
#endif
}

// 简化实现: ErrorCode2String
bool ErrorCode2String(void** ppMsgBuf, unsigned long dwErrorCode, int nLangID, unsigned int dwFlags) {
#ifdef _WIN32
    DWORD result = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        dwErrorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)ppMsgBuf,
        0,
        nullptr
    );
    return result != 0;
#else
    return false;
#endif
}
