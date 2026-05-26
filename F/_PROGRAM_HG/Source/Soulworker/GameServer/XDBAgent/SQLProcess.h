#pragma once

#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XDBAgent/ServerCon.h"

// Forward declaration
class XDBStmt;

class XSQLProcess : public TXProcess<XServerCon> {
public:
    XSQLProcess();
    virtual ~XSQLProcess();

    bool Init(XClient* pClient) override;
    bool Clear() override;
    bool Parse(XPacket& xPacket) override;

    // DBParse - 子类实现此方法处理具体 SubCmd 路由
    // 默认返回 -1 表示未处理
    virtual std::int32_t DBParse(XDBStmt* pDBStmt, XPacket& xPacket, int xReturnSessionID) {
        return -1;
    }
};