// GameDBSocket.cpp
// XGameDBSocket implementation
// IDA decompilation from GameServer.exe

#include "GameDBSocket.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// FindUser - 根据会话ID查找用户
// TODO: 需要从 XGameServer 单例获取用户
CUser* XGameDBSocket::FindUser(int xSessionID) {
    (void)xSessionID;
    // TODO: Implement properly - 需要访问 XGameServer::FindUser
    return nullptr;
}

// DBParse - 数据库包解析
bool XGameDBSocket::DBParse(CUser* pUser, XPacket& xPacket) {
    (void)pUser;
    std::uint8_t byMainCmd = xPacket.GetMainCmd();
    std::uint8_t bySubCmd = xPacket.GetSubCmd();
    
    LogHelper::LogDebug("game.system", "XGameDBSocket::DBParse Main:%02X Sub:%02X", byMainCmd, bySubCmd);
    
    // TODO: Implement packet routing based on MainCmd
    return true;
}

// OnDisConnect - 断开连接处理
void XGameDBSocket::OnDisConnect() {
    LogHelper::LogError("game.system", "[DB_SOCKET] OnDisconnect (type=%d)", m_byType);
    
    // 调用基类
    TXDBSocketT<CUser>::OnDisConnect();
}

// OnNotConnect - 连接失败处理
void XGameDBSocket::OnNotConnect() {
    LogHelper::LogError("game.system", "[DB_SOCKET] OnNotConnect (type=%d)", m_byType);
    
    // 调用基类
    TXDBSocketT<CUser>::OnNotConnect();
}
