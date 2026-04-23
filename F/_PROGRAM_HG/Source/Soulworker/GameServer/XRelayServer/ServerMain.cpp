#include "Soulworker/GameServer/XRelayServer/ServerMain.h"

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_RuntimeFaultLogger.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"

#include <cctype>
#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#undef min
#undef max
#endif

namespace {
constexpr char Locale[] = "";
char g_serviceName[256] = "RelayServer";
char g_description[256] = "SoulWorker RelayServer";
#ifdef _WIN32
SERVICE_STATUS_HANDLE g_hService = nullptr;
GreenDamTan_RuntimeFaultLogger::UnhandledExceptionFilter g_previousFilter = nullptr;
DWORD g_dwServiceState = 0;
#else
void* g_hService = nullptr;
GreenDamTan_RuntimeFaultLogger::UnhandledExceptionFilter g_previousFilter = nullptr;
unsigned long g_dwServiceState = 0;
#endif

bool EqualsIgnoreCase(const char* lhs, const char* rhs) {
    if (!lhs || !rhs) {
        return false;
    }

    while (*lhs && *rhs) {
        const unsigned char a = static_cast<unsigned char>(*lhs);
        const unsigned char b = static_cast<unsigned char>(*rhs);
        if (std::tolower(a) != std::tolower(b)) {
            return false;
        }
        ++lhs;
        ++rhs;
    }

    return *lhs == '\0' && *rhs == '\0';
}

std::string GetModulePath(const char* argv0) {
    if (argv0 && *argv0) {
        return std::filesystem::absolute(argv0).string();
    }
    return std::filesystem::current_path().string();
}

std::string GetFileStem(const std::string& path) {
    return std::filesystem::path(path).stem().string();
}

void PrintServiceUsage(const char* fileName) {
    std::printf("USAGE:\n");
    std::printf("%s [/INSTALL | /UNINSTALL | /TEST]\n\n", fileName ? fileName : "RelayServer");
    std::printf("    /INSTALL   Creates a service object and adds it to the specified service\n");
    std::printf("               control manager database.\n");
    std::printf("    OPTION     -DEF NAME Uses the name specified during installation.\n");
    std::printf("    /UNINSTALL Marks the specified service for deletion from the service\n");
    std::printf("               control manager database.\n");
    std::printf("    OPTION     -DEF NAME While deleting the specified name is used.\n");
    std::printf("    /INSERT    Upload script files to MSSQL Database.\n");
    std::printf("    /TEST      Running the Server Program in Console mode.\n");
    std::printf("    OPTIONS    -VERBOSE  Display Debug Information.\n");
    std::printf("               -PACKET   Display Network Information.\n");
    std::printf("               -DEF NAME Uses the name specified during execution.\n");
}

void CopyServiceName(const char* serviceName) {
    std::memset(g_serviceName, 0, sizeof(g_serviceName));
    if (serviceName) {
#ifdef _WIN32
        strncpy_s(g_serviceName, sizeof(g_serviceName), serviceName, _TRUNCATE);
#else
        std::strncpy(g_serviceName, serviceName, sizeof(g_serviceName) - 1);
#endif
    }
}

std::uint64_t ReadAutoShutdownMs() {
    // GreenDamTan 扩展：自动关闭计时器（环境变量 GREENDAMTAN_AUTOSTOP_MS）
    // 用于还原工程自动化测试验证，非原版服务器行为
#ifdef _WIN32
    char* value = nullptr;
    std::size_t length = 0;
    if (_dupenv_s(&value, &length, "GREENDAMTAN_AUTOSTOP_MS") != 0 || !value || !*value) {
        if (value) {
            std::free(value);
        }
        return 0;
    }
#else
    const char* value = std::getenv("GREENDAMTAN_AUTOSTOP_MS");
    if (!value || !*value) {
        return 0;
    }
#endif

    char* end = nullptr;
    const unsigned long long parsed = std::strtoull(value, &end, 10);
    if (!end || *end != '\0') {
#ifdef _WIN32
        std::free(value);
#endif
        return 0;
    }
#ifdef _WIN32
    std::free(value);
#endif
    return static_cast<std::uint64_t>(parsed);
}

void FreeMessageBuffer(void* msgBuf) {
    if (!msgBuf) {
        return;
    }
#ifdef _WIN32
    LocalFree(msgBuf);
#else
    std::free(msgBuf);
#endif
}
}

std::size_t ErrorCode2String(void** msgBuf,
                             unsigned long lastError,
                             std::uint16_t priLangId,
                             std::uint16_t subLangId) {
    if (!msgBuf) {
        return 0;
    }
    *msgBuf = nullptr;

#ifdef _WIN32
    HMODULE module = nullptr;
    DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS;
    if (lastError >= 0x834 && lastError <= 0xBB7) {
        module = LoadLibraryExA("netmsg.dll", nullptr, LOAD_LIBRARY_AS_DATAFILE);
        if (module) {
            flags = FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_IGNORE_INSERTS;
        }
    }
    if (lastError > 0xC0000000UL) {
        module = LoadLibraryExA("ntdll.dll", nullptr, LOAD_LIBRARY_AS_DATAFILE);
        if (module) {
            flags |= FORMAT_MESSAGE_FROM_HMODULE;
        }
    }

    const DWORD languageId =
        static_cast<DWORD>(priLangId | static_cast<std::uint16_t>(subLangId << 10));
    const DWORD length = FormatMessageA(flags,
                                        module,
                                        lastError,
                                        languageId,
                                        reinterpret_cast<LPSTR>(msgBuf),
                                        0,
                                        nullptr);
    if (module) {
        FreeLibrary(module);
    }
    return static_cast<std::size_t>(length);
#else
    (void)lastError;
    (void)priLangId;
    (void)subLangId;
    return 0;
#endif
}

bool InstallService(const char* binaryPathName, const char* serviceName, std::uint32_t startType) {
#ifdef _WIN32
    if (!binaryPathName || !serviceName) {
        return false;
    }

    char batchPathName[272] = {};
    char displayName[272] = {};
    std::snprintf(batchPathName, sizeof(batchPathName), "%s -def %s", binaryPathName, serviceName);
    std::snprintf(displayName, sizeof(displayName), "SOUL_WORKER - %s", serviceName);

    SC_HANDLE hManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (!hManager) {
        return false;
    }

    SC_HANDLE hService = CreateServiceA(hManager,
                                        serviceName,
                                        displayName,
                                        SERVICE_ALL_ACCESS,
                                        SERVICE_WIN32_OWN_PROCESS,
                                        startType,
                                        SERVICE_ERROR_NORMAL,
                                        batchPathName,
                                        nullptr,
                                        nullptr,
                                        nullptr,
                                        nullptr,
                                        nullptr);
    if (!hService) {
        void* msgBuf = nullptr;
        const DWORD errorCode = GetLastError();
        if (ErrorCode2String(&msgBuf, errorCode, 0, 1)) {
            std::printf("\nCreateService (%lu): %s\n", errorCode, static_cast<const char*>(msgBuf));
            FreeMessageBuffer(msgBuf);
        }
        CloseServiceHandle(hManager);
        return false;
    }

    SERVICE_DESCRIPTIONA serviceDescription{};
    serviceDescription.lpDescription = g_description;
    ChangeServiceConfig2A(hService, SERVICE_CONFIG_DESCRIPTION, &serviceDescription);
    CloseServiceHandle(hService);
    CloseServiceHandle(hManager);
    return true;
#else
    (void)binaryPathName;
    (void)serviceName;
    (void)startType;
    return false;
#endif
}

bool UninstallService(const char* serviceName) {
#ifdef _WIN32
    if (!serviceName) {
        return false;
    }

    SC_HANDLE hManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (!hManager) {
        return false;
    }
    SC_HANDLE hService = OpenServiceA(hManager, serviceName, SERVICE_ALL_ACCESS);
    if (!hService) {
        void* msgBuf = nullptr;
        const DWORD errorCode = GetLastError();
        if (ErrorCode2String(&msgBuf, errorCode, 0, 1)) {
            std::printf("\nOpenService (%lu): %s\n", errorCode, static_cast<const char*>(msgBuf));
            FreeMessageBuffer(msgBuf);
        }
        CloseServiceHandle(hManager);
        return false;
    }
    const bool deleted = DeleteService(hService) == TRUE;
    CloseServiceHandle(hService);
    CloseServiceHandle(hManager);
    return deleted;
#else
    (void)serviceName;
    return false;
#endif
}

void SET_SERVICE_STATE(unsigned long dwState, unsigned long dwAccept) {
#ifdef _WIN32
    if (g_dwServiceState == dwState || !g_hService) {
        return;
    }

    SERVICE_STATUS serviceStatus{};
    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwCurrentState = dwState;
    serviceStatus.dwControlsAccepted = dwAccept;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    serviceStatus.dwWaitHint = 0;
    g_dwServiceState = dwState;
    SetServiceStatus(g_hService, &serviceStatus);
#else
    (void)dwAccept;
    g_dwServiceState = dwState;
#endif
}

std::uint64_t SERVICE_HANDLER(unsigned int fdwControl,
                              unsigned int /*dwEventType*/,
                              void* /*lpEventData*/,
                              void* /*lpContext*/) {
    switch (fdwControl) {
    case 1:
        SET_SERVICE_STATE(3u, 0);
        SET_SERVICE_STATE(1u, 3u);
        GreenDamTan_RuntimeFaultLogger::RestoreUnhandledExceptionFilter(g_previousFilter);
        TXSingleton<XRelayServer>::Instance()->Shutdown(0xFFFFFFFFu);
        break;
    case 2:
        SET_SERVICE_STATE(6u, 0);
        SET_SERVICE_STATE(7u, 3u);
        break;
    case 3:
        SET_SERVICE_STATE(5u, 0);
        SET_SERVICE_STATE(4u, 3u);
        break;
    default:
        break;
    }
    return 0;
}

void ServiceMain(unsigned int /*argc*/, const char** argv) {
#ifdef _WIN32
    g_hService = RegisterServiceCtrlHandlerExA(argv && *argv ? *argv : g_serviceName,
                                               reinterpret_cast<LPHANDLER_FUNCTION_EX>(SERVICE_HANDLER),
                                               nullptr);
    if (!g_hService) {
        return;
    }

    SET_SERVICE_STATE(4u, 3u);
    g_previousFilter = GreenDamTan_RuntimeFaultLogger::InstallUnhandledExceptionFilter();
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();
    relayServer->StartLog();
    GreenDamTan_RuntimeFaultLogger::LogReady("service");
    relayServer->Run();
    GreenDamTan_RuntimeFaultLogger::RestoreUnhandledExceptionFilter(g_previousFilter);
    relayServer->Shutdown(0xFFFFFFFFu);
    SET_SERVICE_STATE(1u, 3u);
#else
    (void)argv;
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();
    relayServer->StartLog();
    GreenDamTan_RuntimeFaultLogger::LogReady("service");
    relayServer->Run();
    relayServer->Shutdown(0xFFFFFFFFu);
#endif
}

bool ServiceInit(unsigned int argc, char** argv) {
    const std::string modulePath = GetModulePath(argv ? argv[0] : nullptr);
    const std::string fileName = GetFileStem(modulePath);

    if (argc < 2) {
        PrintServiceUsage(fileName.c_str());
        return true;
    }

    if (argc == 3 && EqualsIgnoreCase(argv[1], "-def")) {
        CopyServiceName(argv[2]);
        std::printf("\n");
        std::printf("StartServiceCtrlDispatcher being called.\n");
        std::printf("This may take several seconds.  Please wait.\n");
#ifdef _WIN32
        SERVICE_TABLE_ENTRYA dispatchTable[2] = {};
        dispatchTable[0].lpServiceName = g_serviceName;
        dispatchTable[0].lpServiceProc = reinterpret_cast<LPSERVICE_MAIN_FUNCTIONA>(ServiceMain);
        if (!StartServiceCtrlDispatcherA(dispatchTable)) {
            void* msgBuf = nullptr;
            const DWORD errorCode = GetLastError();
            if (ErrorCode2String(&msgBuf, errorCode, 0, 1)) {
                std::printf("\nStartServiceCtrlDispatcher (%lu): %s\n",
                            errorCode,
                            static_cast<const char*>(msgBuf));
                FreeMessageBuffer(msgBuf);
            }
        }
#else
        const char* serviceArgv[1] = {g_serviceName};
        ServiceMain(1, serviceArgv);
#endif
        return false;
    }

    if (EqualsIgnoreCase(argv[1], "/TEST")) {
        return false;
    }

    if (!EqualsIgnoreCase(argv[1], "/VER")) {
        if (EqualsIgnoreCase(argv[1], "/INSTALL")) {
            if (argc == 4 && EqualsIgnoreCase(argv[2], "-DEF")) {
                std::printf("\nInstallation to your service is initiated.\n");
                if (InstallService(modulePath.c_str(), argv[3], 2u)) {
                    std::printf("\nInstallation to your service is completed.\n");
                } else {
                    std::printf("\nInstallation to your service is not completed.\n");
                }
            } else {
                std::printf("USAGE:\n");
                std::printf("    /INSTALL   Creates a service object and adds it to the specified service\n");
                std::printf("               control manager database.\n");
                std::printf("    OPTION     -DEF NAME  Uses the name specified during installation.");
            }
#ifdef _WIN32
            _getch();
#endif
            return false;
        }

        if (EqualsIgnoreCase(argv[1], "/UNINSTALL")) {
            if (argc == 4 && EqualsIgnoreCase(argv[2], "-DEF")) {
                std::printf("\nDeleting your service is initiated.\n");
                if (UninstallService(argv[3])) {
                    std::printf("\nDeleting your service is completed.\n");
                } else {
                    std::printf("\nDeleting our service is not completed.\n");
                }
            } else {
                std::printf("USAGE:\n");
                std::printf("    /UNINSTALL Marks the specified service for deletion from the service\n");
                std::printf("               control manager database.\n");
                std::printf("    OPTION     -DEF NAME  While deleting the specified name is used.");
            }
#ifdef _WIN32
            _getch();
#endif
            return false;
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

    const bool consoleTestMode = argc > 1 && EqualsIgnoreCase(argv[1], "/TEST");
    const std::uint64_t autoShutdownMs = ReadAutoShutdownMs();

    if (argc > 1 && !consoleTestMode) {
        return ServiceInit(static_cast<unsigned int>(argc), argv) ? 1 : 0;
    }

    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();
    relayServer->StartLog();
    GreenDamTan_RuntimeFaultLogger::LogReady("console");

    int exitCode = 0;
    if (consoleTestMode) {
        if (!relayServer->Init()) {
            exitCode = 1;
        } else {
            relayServer->Shutdown(0xFFFFFFFFu);
        }
    } else if (autoShutdownMs > 0) {
        // GREENDAMTAN_AUTOSTOP_MS set: run for specified time then auto-shutdown
        if (!relayServer->Init()) {
            exitCode = 1;
        } else {
#ifdef _WIN32
            Sleep(static_cast<DWORD>(autoShutdownMs));
#else
            usleep(static_cast<useconds_t>(autoShutdownMs) * 1000);
#endif
            relayServer->Shutdown(0xFFFFFFFFu);
        }
    } else {
        relayServer->Run();
    }

#ifdef _WIN32
    GreenDamTan_RuntimeFaultLogger::RestoreUnhandledExceptionFilter(previousFilter);
#endif
    return exitCode;
}

