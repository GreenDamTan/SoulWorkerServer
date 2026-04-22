#pragma once

#include <cstdint>
#include <functional>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CPartyProcess : public TXProcess<CServer> {
public:
    explicit CPartyProcess(CServer* server = nullptr) {
        SetCmd(0xF4);
        SetName("CPartyProcess");
        Init(server);
    }

    bool Parse(XPacket& xPacket) override;

    // sub 0x01
    bool ReqPartyCreate(XPacket& xPacket);
    // sub 0x03
    bool ReqPartyLeaveMember(XPacket& xPacket);
    // sub 0x04
    bool ReqPartyChangeMaster(XPacket& xPacket);
    // sub 0x05
    bool ReqPartyUpdateMember(XPacket& xPacket);
    // sub 0x10
    bool ReqPartyEnterServer(XPacket& xPacket);
    // sub 0x11
    bool ReqPartyInvite(XPacket& xPacket);
    // sub 0x12
    bool ReqPartyAccept(XPacket& xPacket);
    // sub 0x13
    bool ReqPartyCancel(XPacket& xPacket);
    // sub 0x14
    bool SyncPartyMessage(XPacket& xPacket);
    // sub 0x20
    bool ReqPartyMatchingEnter(XPacket& xPacket);
    // sub 0x21
    bool ReqPartyMatchingExit(XPacket& xPacket);
    // sub 0x22
    bool ReqPartyMatchingCheck(XPacket& xPacket);
    // sub 0x25
    bool ReqPartyRecruitAdd(XPacket& xPacket);
    // sub 0x26
    bool ReqPartyRecruitDel(XPacket& xPacket);
    // sub 0x27
    bool ReqPartyRecruitApply(XPacket& xPacket);
    // sub 0x28
    bool ReqPartyRecruitApplyAccept(XPacket& xPacket);
    // sub 0x29
    bool ReqPartyRecruitApplyReject(XPacket& xPacket);
    // sub 0x2B
    bool ReqPartyRecruitList(XPacket& xPacket);
    // sub 0x2C
    bool ReqPartyRecruitMyApplyList(XPacket& xPacket);
    // sub 0x2D
    bool ReqPartyRecruitApplyList(XPacket& xPacket);
    // sub 0x2F
    bool ReqPartyRecruitApplyDel(XPacket& xPacket);
    // sub 0x30
    bool ReqPartyRecruitApplyInfo(XPacket& xPacket);
    // sub 0x32
    bool ResPartyRecruitApplyAcceptCheck(XPacket& xPacket);
    // sub 0x40
    bool ReqPartyInfo(XPacket& xPacket);
    // sub 0x43
    bool ReqPartyMazeClear(XPacket& xPacket);

private:
    bool DispatchPartyJob(const std::function<void()>& job);
};
