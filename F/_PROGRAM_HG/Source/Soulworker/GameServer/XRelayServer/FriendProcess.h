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

    // sub 0x01
    bool ReqFriendListLoad(XPacket& xPacket);
    // sub 0x02
    bool ReqBlockListLoad(XPacket& xPacket);
    // sub 0x03
    bool ReqFriendInvite(XPacket& xPacket);
    // sub 0x04
    bool ReqFriendAccept(XPacket& xPacket);
    // sub 0x05
    bool ReqFriendDelete(XPacket& xPacket);
    // sub 0x07
    bool ReqBlockListAdd(XPacket& xPacket);
    // sub 0x08
    bool ReqBlockListDelete(XPacket& xPacket);
    // sub 0x11
    bool ReqFriendRecommand(XPacket& xPacket);
    // sub 0x15
    bool ReqFriendRecruitList(XPacket& xPacket);
    // sub 0x16
    bool ReqFriendRecruitAdd(XPacket& xPacket);
    // sub 0x17
    bool ReqFriendRecruitDelete(XPacket& xPacket);
    // sub 0x18
    bool ReqFriendRecruitInfo(XPacket& xPacket);
    // sub 0x21
    bool ReqUpdateFriendCommunity(XPacket& xPacket);
    // sub 0x22
    bool ReqFriendFind(XPacket& xPacket);
    // sub 0x25
    bool ReqCheckDailyMissionFirend(XPacket& xPacket);
    // sub 0x26
    bool ResCheckDailyMissionFirend(XPacket& xPacket);
    // sub 0x27
    bool ReqHelperSupportInfo(XPacket& xPacket);
    // sub 0x28
    bool ReqHelperSupportRegister(XPacket& xPacket);
    // sub 0x29
    bool ReqHelperSupportReward(XPacket& xPacket);
    // sub 0x30
    bool ReqHelperSupportList(XPacket& xPacket);
    // sub 0x31
    bool ReqHelperSupportEquip(XPacket& xPacket);
};
