#include "Soulworker/GameServer/XRelayServer/ObserveSocket.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

bool CObserveSocket::StartUp(XOption* pOption) {
    // 对齐 IDA: StartUp 初始化观察套接字
    SetMyInfo(pOption);
    // TODO: 完整实现 (端口配置、Connect)
    return true;
}

bool CObserveSocket::OnUpdate(std::uint64_t currentTick,
                               const char* ip,
                               int port,
                               int userCount,
                               bool controlConnect,
                               bool communityConnect,
                               int maxThreadCount,
                               bool netCafe) {
    // 对齐 IDA 0x1400B2D90 XRelayServer::OnUpdate: 定期向 ObserveAgent 报告服务器状态
    // TODO: 完整实现 (SendReportServerStatus)
    static_cast<void>(currentTick);
    static_cast<void>(ip);
    static_cast<void>(port);
    static_cast<void>(userCount);
    static_cast<void>(controlConnect);
    static_cast<void>(communityConnect);
    static_cast<void>(maxThreadCount);
    static_cast<void>(netCafe);
    return true;
}

void CObserveSocket::OnConnect() {
    SendAddServer();
}

void CObserveSocket::OnDisConnect() {}

void CObserveSocket::OnNotConnect() {}

void CObserveSocket::SetMyInfo(const XOption* option) {
    XRelaySocket::SetMyInfo(option);
}
