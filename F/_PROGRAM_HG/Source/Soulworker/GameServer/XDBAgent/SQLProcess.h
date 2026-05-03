#pragma once

#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XDBAgent/ServerCon.h"

class XSQLProcess : public TXProcess<XServerCon> {
public:
    XSQLProcess();
    virtual ~XSQLProcess();

    bool Init(XClient* pClient) override;
    bool Clear() override;
    bool Parse(XPacket& xPacket) override;
};