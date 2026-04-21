#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CLeagueProcess : public TXProcess<CServer> {
public:
    explicit CLeagueProcess(CServer* server = nullptr) {
        SetCmd(0xF6);
        SetName("CServerLeagueProcess");
        Init(server);
    }

    bool Parse(XPacket& xPacket) override;

    // League sub-command handlers
    bool ReqLeagueCreate(XPacket& xPacket);
    bool ReqLeagueDelete(XPacket& xPacket);
    bool ReqLeagueApplicant(XPacket& xPacket);
    bool ReqLeagueInfo(XPacket& xPacket);
    bool ReqLeagueDelegate(XPacket& xPacket);
    bool ReqLeagueWithDraw(XPacket& xPacket);
    bool ReqLeagueKick(XPacket& xPacket);
    bool ReqLeagueInvite(XPacket& xPacket);
    bool ReqLeagueInviteAccept(XPacket& xPacket);
    bool ReqLeagueInviteReject(XPacket& xPacket);
    bool ReqLeagueBoard(XPacket& xPacket);
    bool ReqLeagueApplicantAccept(XPacket& xPacket);
    bool ReqLeagueApplicantReject(XPacket& xPacket);
    bool ReqLeagueSearch(XPacket& xPacket);
    bool ReqLeagueNoticeChange(XPacket& xPacket);
    bool ReqLeagueList(XPacket& xPacket);
    bool ReqLeagueNameChange(XPacket& xPacket);
    bool ReqLeagueCardChange(XPacket& xPacket);
    bool ReqLeaguePositionNameChange(XPacket& xPacket);
    bool ReqLeagueAuthChange(XPacket& xPacket);
    bool ReqLeagueMessage(XPacket& xPacket);
    bool ReqLeagueMemberPositionChange(XPacket& xPacket);
    bool ReqLeagueMemberLogOut(XPacket& xPacket);
    bool ReqLeagueDeletePenalty(XPacket& xPacket);
    bool ReqLeagueWithdrawPenalty(XPacket& xPacket);
    bool ReqLeagueOpenOrNot(XPacket& xPacket);
    bool ReqLeagueRecruitNotice(XPacket& xPacket);
    bool ReqLeagueRecordUpdate(XPacket& xPacket);
    bool ReqLeagueLevelup(XPacket& xPacket);
    bool ReqLeagueSkillLearn(XPacket& xPacket);
    bool ReqLeagueWealth(XPacket& xPacket);
    bool ReqLeagueMemberInitExp(XPacket& xPacket);
    bool ReqLeagueSkillPointUpdate(XPacket& xPacket);
    bool ReqSyncLeagueInfo(XPacket& xPacket);
    bool ReqLeagueInventoryInfo(XPacket& xPacket);
    bool ReqLeagueInventoryMove(XPacket& xPacket);
};
