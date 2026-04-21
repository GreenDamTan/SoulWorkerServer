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
    bool ReqPartyUpdateMember(XPacket& xPacket);
    bool ReqPartyEnterServer(XPacket& xPacket);
    bool ReqPartyInfo(XPacket& xPacket);
    bool ReqPartyRecruitAdd(XPacket& xPacket);
    bool ReqPartyRecruitDel(XPacket& xPacket);
    bool ReqPartyRecruitApply(XPacket& xPacket);
    bool ReqPartyRecruitApplyAccept(XPacket& xPacket);
    bool ReqPartyRecruitApplyReject(XPacket& xPacket);
    bool ReqPartyRecruitList(XPacket& xPacket);
    bool ReqPartyRecruitMyApplyList(XPacket& xPacket);
    bool ReqPartyRecruitApplyList(XPacket& xPacket);
    bool ReqPartyRecruitApplyDel(XPacket& xPacket);
    bool ReqPartyRecruitApplyInfo(XPacket& xPacket);
    bool ResPartyRecruitApplyAcceptCheck(XPacket& xPacket);
    bool ReqPartyInvite(XPacket& xPacket);

private:
    bool DispatchPartyJob(const std::function<void()>& job);
};
