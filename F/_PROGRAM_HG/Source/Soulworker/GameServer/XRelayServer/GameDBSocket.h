#pragma once

#include <cstdint>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CGameDBSocket : public TXDBSocketT<CServer> {
protected:  // 对齐 IDA: MEAA = protected virtual
    // 对齐 IDA: 这些 virtual 方法是 MEAA = protected virtual
    bool OnParse(XPacket& xPacket) override;
    // 对齐 IDA: MEAAPEAVCServer@@H = protected virtual CServer* FindUser(int)
    CServer* FindUser(int xSessionID) override;
    bool DBParse(CServer* pServer, XPacket& xPacket) override;
    void SetInfomation();  // 对齐 IDA: MEAAXXZ - 新的虚方法，非 override
    void OnDisConnect() override;   // 对齐 IDA: MEAAXXZ - 基类 TXDBSocketT override
    void OnNotConnect() override;   // 对齐 IDA: MEAAXXZ - 基类 TXDBSocketT override

protected:  // 对齐 IDA: IEAA = protected
    bool DBPartyParse(XPacket& xPacket);
    bool DBFriendParse(XPacket& xPacket);
    bool DBForceParse(XPacket& xPacket);
    bool DBLeagueParse(XPacket& xPacket);
    bool DBWorldParse(CServer* pServer, XPacket& xPacket);
    bool DBExchangeParse(XPacket& xPacket);
    bool DBHelperParse(XPacket& xPacket);

    // 联赛数据库响应处理
    bool ResLeagueCreate(XPacket& xPacket);
    bool ResLeagueDelete(XPacket& xPacket);
    bool ResLeagueBoard(XPacket& xPacket);
    bool ResLeagueWithDraw(XPacket& xPacket);
    bool ResLeagueApplicant(XPacket& xPacket);
    bool ResLeagueKick(XPacket& xPacket);
    bool ResLeagueInviteAccept(XPacket& xPacket);
    bool ResLeagueApplicantAccept(XPacket& xPacket);
    bool ResLeagueApplicantReject(XPacket& xPacket);
    bool ResLeagueNoticeChange(XPacket& xPacket);
    bool ResLeagueNameChange(XPacket& xPacket);
    bool ResLeagueCardChange(XPacket& xPacket);
    bool ResLeaguePositionNameChange(XPacket& xPacket);
    bool ResLeagueAuthChange(XPacket& xPacket);
    bool ResLeagueMemberPositionChange(XPacket& xPacket);
    bool ResLeagueApplicantDelete(XPacket& xPacket);
    bool ResLeagueInfo(XPacket& xPacket);
    bool ResLoadLeagueMember(XPacket& xPacket);
    bool ResLoadLeagueApplicant(XPacket& xPacket);
    bool ResLoadLeagueBoard(XPacket& xPacket);
    bool ResLeagueOpenOrNot(XPacket& xPacket);
    bool ResLeagueRecruitNotice(XPacket& xPacket);
    bool ResLeagueSearch(XPacket& xPacket);
    bool ResLeagueRecord(XPacket& xPacket);
    bool ResLeagueDelegate(XPacket& xPacket);
    bool ResLeagueWealth(XPacket& xPacket);
    bool ResLeagueLevelup(XPacket& xPacket);
    bool ResLeagueSkillLearn(XPacket& xPacket);
    bool ReqLeagueInventoryMove(XPacket& xPacket);  // 对齐 IDA: Req 不是 Res
    bool ReqLeagueInventoryInfo(XPacket& xPacket);  // 对齐 IDA: Req 不是 Res
    bool ResLeagueList(XPacket& xPacket);
    bool ResGMTLeagueInfo(XPacket& xPacket);
    bool ResLeagueWithdrawPenalty(XPacket& xPacket);
    bool ResLeagueDeletePenalty(XPacket& xPacket);
    // 好友数据库响应处理 (对齐 IDA 0x140049B80 DBFriendParse switch)
    // sub 0x01
    bool ResFriendLoad(XPacket& xPacket);
    // sub 0x02
    bool ResFriendInvite(XPacket& xPacket);
    // sub 0x03
    bool ResFriendInviteCheck(XPacket& xPacket);
    // sub 0x04
    bool ResFriendDelete(XPacket& xPacket);
    // sub 0x05
    bool ResFriendAccept(XPacket& xPacket);
    // sub 0x06
    bool ResBlockListAdd(XPacket& xPacket);
    // sub 0x07
    bool ResBlockListDelete(XPacket& xPacket);
    // sub 0x08
    bool ResRecruitLoad(XPacket& xPacket);
    // sub 0x09
    bool ResRecruitAdd(XPacket& xPacket);
    // sub 0x10
    bool ResRecruitDelete(XPacket& xPacket);
    // sub 0x11
    bool ResFriendFind(XPacket& xPacket);

    bool ResForceMatchingCreate(XPacket& xPacket);
    bool ResPartyJoin(XPacket& xPacket);
    bool ResForceJoin(XPacket& xPacket);
    bool ResPartyCreate(XPacket& xPacket);
    bool ResForceCreate(XPacket& xPacket);
    bool ResPartyLeave(XPacket& xPacket);
    bool ResPartyDelete(XPacket& xPacket);
    bool ResPartyUpdateMemberInfo(XPacket& xPacket);
    bool ResPartyChangeMaster(XPacket& xPacket);
    bool ResForceUpdateMemberInfo(XPacket& xPacket);
    bool ResForceChangeMaster(XPacket& xPacket);
    bool ResForceLeave(XPacket& xPacket);
    bool ResForceDelete(XPacket& xPacket);
    bool ResPartyLoadAll(XPacket& xPacket);
    bool ResPartyMatchingCreate(XPacket& xPacket);
    bool ResForceLoadAll(XPacket& xPacket);
    bool ResExchangePriceHistory(XPacket& xPacket);
    bool ResHelperSupportEquip(XPacket& xPacket);
};

// Note: XGameDBSocketMgr is defined in XCore/XServer/XGameDBSocketMgr.h

