// GameDBSocket.h
// XGameDBSocket - Database socket for GameServer
// IDA decompilation from GameServer.exe

#pragma once

#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XGameServer/User.h"

// ============================================================================
// XGameDBSocket - Database socket for GameServer
// 继承自 TXDBSocketT<CUser>
// ============================================================================
class XGameDBSocket : public TXDBSocketT<CUser> {
public:
    XGameDBSocket() : TXDBSocketT<CUser>() {
        m_byType = 0;
        m_bState = false;
    }

    virtual ~XGameDBSocket() = default;

    // FindUser - 根据会话ID查找用户
    CUser* FindUser(int xSessionID) override;

    // DBParse - 数据库包解析
    bool DBParse(CUser* pUser, XPacket& xPacket) override;

    // OnDisConnect - 断开连接处理
    void OnDisConnect() override;

    // OnNotConnect - 连接失败处理
    void OnNotConnect() override;
};
