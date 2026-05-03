// DBAgent ServerMain - 入口点恢复
// Per IDA 0x140010080: main 函数

#include "Soulworker/GameServer/XDBAgent/DBAgent.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_RuntimeFaultLogger.h"

#include <locale>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#endif

// ServiceInit 函数声明 - 处理命令行参数启动
extern bool ServiceInit(int argc, char** argv);

int main(int argc, char* argv[]) {
    // 设置本地化
    std::setlocale(LC_ALL, "");

#ifdef _WIN32
    // 设置异常过滤器
    // TODO: 原始使用 XDump::XGenerateDump_Popup，需要恢复 XDump 模块
    // LPTOP_LEVEL_EXCEPTION_FILTER previousFilter = SetUnhandledExceptionFilter(XDump::XGenerateDump_Popup);
#endif

    // 命令行参数处理
    if (argc > 1) {
        // TODO: 原始调用 ServiceInit(argc, argv)
        GreenDamTan_log("DBAgent", "ServerMain", "argc > 1, would call ServiceInit");
        return 1;
    }

    // 启动 DBAgent 服务
    XDBAgent* pDBAgent = TXSingleton<XDBAgent>::Instance();
    if (pDBAgent) {
        // TODO: 原始调用 XServer::Run(v4)
        GreenDamTan_log("DBAgent", "ServerMain", "Starting DBAgent service...");
        // XServer::Run(pDBAgent);
    }

#ifdef _WIN32
    // SetUnhandledExceptionFilter(previousFilter);
#endif

    return 0;
}