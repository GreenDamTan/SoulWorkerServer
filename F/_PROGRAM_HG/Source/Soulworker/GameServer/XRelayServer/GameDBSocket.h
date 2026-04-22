#pragma once

#include <cstdint>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CGameDBSocket : public TXDBSocketT<CServer> {
public:
    bool OnParse(XPacket& xPacket) override;
    CServer* FindUser(unsigned int xSessionID) override;
    bool DBParse(CServer* pServer, XPacket& xPacket) override;
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
    bool ResLeagueInventoryMove(XPacket& xPacket);
    bool ResLeagueInventoryInfo(XPacket& xPacket);
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

class XGameDBSocketMgr {
public:
    XGameDBSocketMgr() = default;
    ~XGameDBSocketMgr();

    void Init();
    void AutoConnect();
    void DisConnect();
    bool SendAccountDBAgent(int iIndex, const XSendDBPacket& xSendPacket);
    bool SendGameDBAgent(int iIndex, const XSendDBPacket& xSendPacket);
    int GetGameDBAgentCount() const;
    int GetAccountDBAgentCount() const;

private:
    void InitAgentGroup(std::uint8_t byType, CGameDBSocket*& ppAgents, int& nAgentCount);
    void AutoConnectGroup(CGameDBSocket* pAgents, int nAgentCount);
    void DisconnectGroup(CGameDBSocket* pAgents, int nAgentCount);
    void Clear();

    CGameDBSocket* m_pGameDBAgent = nullptr;
    int m_nGameAgentCnt = 0;
    CGameDBSocket* m_pAccountDBAgent = nullptr;
    int m_nAccountAgentCnt = 0;
};
