#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CFriendProcess : public TXProcess<CServer> {
public:
    explicit CFriendProcess(CServer* server = nullptr) {
        SetCmd(0xF5);
        SetName("CServerFriendProcess");
        Init(server);
    }

    bool Parse(XPacket& xPacket) override;
    bool ReqFriendListLoad(XPacket& xPacket);
    bool ReqBlockListLoad(XPacket& xPacket);
    bool ReqFriendRecruitDelete(XPacket& xPacket);
};
