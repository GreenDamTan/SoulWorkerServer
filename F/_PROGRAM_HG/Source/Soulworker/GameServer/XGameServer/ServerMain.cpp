#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cstdlib>

// GameServer 入口点
int main(int argc, char* argv[]) {
    GreenDamTan_log(__FILE__, __FUNCTION__, "GameServer starting...");

    // 创建服务器实例
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "Failed to create XGameServer instance");
        return 1;
    }

    // 初始化服务器
    if (!pServer->InitServer()) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "Failed to initialize XGameServer");
        return 1;
    }

    // 运行服务器主循环
    pServer->Run();

    // 清理
    pServer->Clear();

    GreenDamTan_log(__FILE__, __FUNCTION__, "GameServer shutdown complete");
    return 0;
}
