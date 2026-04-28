// ServerMain.cpp
// ControlServer 进程入口
// 对齐 IDA: main @ 0x14003BED0

#include "ControlServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_RuntimeFaultLogger.h"
#include <clocale>
#include <cstdio>

namespace {
constexpr char Locale[] = "";
}

// 对齐 IDA: ?ServiceInit@@YA_NKPEAPEAD@Z
// 服务初始化函数，处理命令行参数
extern bool ServiceInit(unsigned long argc, char** argv);

int main(int argc, char* argv[]) {
    std::setlocale(LC_ALL, Locale);

#ifdef _WIN32
    // 对齐 IDA: SetUnhandledExceptionFilter with XDump::XGenerateDump_Popup
    GreenDamTan_RuntimeFaultLogger::UnhandledExceptionFilter previousFilter =
        GreenDamTan_RuntimeFaultLogger::InstallUnhandledExceptionFilter();
#endif

    // 对齐 IDA: if (argc > 1) ServiceInit(argc, argv)
    bool bServiceMode = false;
    if (argc > 1) {
        bServiceMode = ServiceInit(static_cast<unsigned long>(argc), argv);
    }

    if (!bServiceMode) {
        // 对齐 IDA: XControlServer::Instance()->Run()
        XControlServer* controlServer = XControlServer::Instance();
        controlServer->StartLog();
        GreenDamTan_RuntimeFaultLogger::LogReady("console");
        controlServer->Run();
    }

#ifdef _WIN32
    GreenDamTan_RuntimeFaultLogger::RestoreUnhandledExceptionFilter(previousFilter);
#endif

    return 0;
}

// 简化实现: ServiceInit 处理服务模式参数
bool ServiceInit(unsigned long argc, char** argv) {
    // 对齐 IDA: ?ServiceInit@@YA_NKPEAPEAD@Z
    // 原始实现解析命令行参数并启动服务模式
    // 当前简化: 返回 false 表示不使用服务模式
    for (unsigned long i = 1; i < argc; ++i) {
        if (argv[i] && argv[i][0] == '-') {
            // 解析参数 (简化实现)
            GreenDamTan_log(__FILE__, __FUNCTION__, "ServiceInit arg detected");
        }
    }
    return false;
}
