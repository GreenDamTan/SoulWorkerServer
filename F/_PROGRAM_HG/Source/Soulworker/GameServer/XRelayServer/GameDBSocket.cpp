#include "Soulworker/GameServer/XRelayServer/GameDBSocket.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

// 对齐 IDA 0x1400496E0: SetInfomation - 设置 DB Agent 连接信息
void CGameDBSocket::SetInfomation() {
    sprintf_s(m_szName, "DBAGENT");
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    XOption& option = relayServer.GetOption();
    std::uint16_t port = 0;
    option.GetServerPrivateIPAndPort(m_szName, m_szIP, port);
    m_sPort = static_cast<std::int16_t>(port);
}

// 对齐 IDA 0x14004D970: OnDisConnect
void CGameDBSocket::OnDisConnect() {
}

// 对齐 IDA 0x14004DA40: OnNotConnect
void CGameDBSocket::OnNotConnect() {
}

// 对齐 IDA: 参数类型 H = int
CServer* CGameDBSocket::FindUser(int xSessionID) {
    static_cast<void>(xSessionID);
    return nullptr;
}

bool CGameDBSocket::OnParse(XPacket& xPacket) {
    int xSessionID = 0;
    xPacket.XParse >> xSessionID;
    return DBParse(nullptr, xPacket);
}

bool CGameDBSocket::DBParse(CServer* pServer, XPacket& xPacket) {
    const std::uint8_t byMainCmd = xPacket.GetMainCmd();
    const std::uint8_t bySubCmd = xPacket.GetSubCmd();
    LogHelper::LogDebug("game.db", "CGameDBSocket::DBParse Main:%02X Sub:%02X", byMainCmd, bySubCmd);

    switch (byMainCmd) {
    case 4:
        return DBPartyParse(xPacket);
    case 5:
        return DBFriendParse(xPacket);
    case 7:
        return DBLeagueParse(xPacket);
    case 8:
        return DBForceParse(xPacket);
    case 0x26:
        return DBHelperParse(xPacket);
    case 0x27:
        return DBExchangeParse(xPacket);
    case 0x43:
        return DBWorldParse(pServer, xPacket);
    default:
        return true;
    }
}

bool CGameDBSocket::DBPartyParse(XPacket& xPacket) {
    switch (static_cast<unsigned char>(xPacket.GetSubCmd())) {
    case 1:
        return ResPartyCreate(xPacket);
    case 2:
        return ResPartyJoin(xPacket);
    case 3:
        return ResPartyLeave(xPacket);
    case 4:
        return ResPartyUpdateMemberInfo(xPacket);
    case 5:
        return ResPartyChangeMaster(xPacket);
    case 6:
        return ResPartyDelete(xPacket);
    case 0x11:
        return ResPartyLoadAll(xPacket);
    case 0x13:
        return ResPartyMatchingCreate(xPacket);
    default:
        return true;
    }
}

bool CGameDBSocket::DBFriendParse(XPacket& xPacket) {
    // 对齐 IDA 0x140049B80: 完整 sub switch
    switch (static_cast<unsigned char>(xPacket.GetSubCmd())) {
    case 0x01:
        return ResFriendLoad(xPacket);
    case 0x02:
        return ResFriendInvite(xPacket);
    case 0x03:
        return ResFriendInviteCheck(xPacket);
    case 0x04:
        return ResFriendDelete(xPacket);
    case 0x05:
        return ResFriendAccept(xPacket);
    case 0x06:
        return ResBlockListAdd(xPacket);
    case 0x07:
        return ResBlockListDelete(xPacket);
    case 0x08:
        return ResRecruitLoad(xPacket);
    case 0x09:
        return ResRecruitAdd(xPacket);
    case 0x10:
        return ResRecruitDelete(xPacket);
    case 0x11:
        return ResFriendFind(xPacket);
    default:
        return true;
    }
}

bool CGameDBSocket::DBForceParse(XPacket& xPacket) {
    switch (static_cast<unsigned char>(xPacket.GetSubCmd())) {
    case 1:
        return ResForceCreate(xPacket);
    case 2:
        return ResForceJoin(xPacket);
    case 3:
        return ResForceLeave(xPacket);
    case 4:
        return ResForceUpdateMemberInfo(xPacket);
    case 5:
        return ResForceChangeMaster(xPacket);
    case 6:
        return ResForceDelete(xPacket);
    case 0x0B:
        return ResForceLoadAll(xPacket);
    case 0x0D:
        return ResForceMatchingCreate(xPacket);
    default:
        return true;
    }
}

// 好友数据库响应处理 (对齐 IDA 0x140049B80 DBFriendParse switch)
bool CGameDBSocket::ResFriendLoad(XPacket& xPacket) {
    // 对齐 IDA 0x14004BAF0: 加载好友列表、黑名单、社区状态
    int nErrorCode = 0;
    xPacket.XParse >> nErrorCode;

    PS_DB_FRIEND_LIST stFriendList{};
    PS_BLOCKLIST_INFO stBlockList{};
    ST_CHAR_COMMUNITY stCharCommunity{};

    xPacket >> stFriendList;
    xPacket >> stBlockList;
    xPacket >> stCharCommunity;

    if (nErrorCode != 0) {
        LogHelper::LogError("game.system", "<FRIEND> FAILED LOAD FRIEND ( errCode %d )", nErrorCode);
        return false;
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.SetCharCommunity(stFriendList.dwActorID, stCharCommunity);  // 对齐 IDA: 传引用
    relayServer.SetBlockLoad(stFriendList.dwActorID, stBlockList);  // 对齐 IDA: 传引用
    relayServer.SetFriendLoad(stFriendList);  // 对齐 IDA: 传引用
    relayServer.SendFriendServerLoad(stFriendList.dwActorID);

    return true;
}

bool CGameDBSocket::ResFriendInvite(XPacket& xPacket) {
    // 对齐 IDA 0x14004BCC0: 好友邀请响应
    PS_RES_DB_FRIEND_INVITE stInvite{};
    xPacket >> stInvite;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.InviteFriend(stInvite);

    return true;
}

bool CGameDBSocket::ResFriendInviteCheck(XPacket& xPacket) {
    // 对齐 IDA 0x14004BDB0: 好友邀请检查响应
    PS_RES_FRIEND_INVITE stInvite{};
    xPacket >> stInvite;

    std::uint32_t dwDelUCID = 0;
    xPacket.XParse >> dwDelUCID;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.InviteCheckFriend(stInvite, dwDelUCID);

    return true;
}

bool CGameDBSocket::ResFriendDelete(XPacket& xPacket) {
    // 对齐 IDA 0x14004BEC0: 删除好友响应
    PS_DB_FRIEND_DELETE stDelete{};
    xPacket >> stDelete;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.DeleteFriend(stDelete);

    return true;
}

bool CGameDBSocket::ResFriendAccept(XPacket& xPacket) {
    // 对齐 IDA 0x14004BE50: 好友接受响应
    PS_DB_FRIEND_ACCEPT_RES stAccept{};
    xPacket >> stAccept;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.AcceptFriend(stAccept);

    return true;
}

bool CGameDBSocket::ResBlockListAdd(XPacket& xPacket) {
    // 对齐 IDA 0x14004BF00: 添加黑名单响应
    PS_RES_DB_FRIEND_BLOCK stBlock{};
    xPacket >> stBlock;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.AddBlockList(stBlock);

    return true;
}

bool CGameDBSocket::ResBlockListDelete(XPacket& xPacket) {
    // 对齐 IDA 0x14004BF60: 删除黑名单响应
    PS_RES_BLOCKLIST_DELETE stBlock{};
    xPacket >> stBlock;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.DeleteBlockList(stBlock);

    return true;
}

bool CGameDBSocket::ResRecruitLoad(XPacket& xPacket) {
    // 对齐 IDA 0x14004BFC0: 加载招募列表
    std::uint8_t byLast = 0;
    xPacket.XParse >> byLast;

    ST_RECRUIT_LIST stList{};
    xPacket >> stList;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.SetRecruitList(stList, byLast);

    return true;
}

bool CGameDBSocket::ResRecruitAdd(XPacket& xPacket) {
    // 对齐 IDA 0x14004C040: 添加招募响应
    PS_RES_RECRUIT_ADD stList{};
    xPacket >> stList;

    return CLogicThreadManager::Instance().DoJob(2, [stList]() mutable {
        TXSingleton<XRelayServer>::Instance()->SendRecruitAdd(stList.stAdd);
    });
}

bool CGameDBSocket::ResFriendFind(XPacket& xPacket) {
    // 对齐 IDA 0x14004C200: 查找好友响应
    PS_DB_FRIEND_FIND stInfo{};
    xPacket >> stInfo;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.ResFriendFind(stInfo);

    return true;
}

bool CGameDBSocket::ResRecruitDelete(XPacket& xPacket) {
    PS_RES_RECRUIT_DELETE deleteInfo{};
    xPacket >> deleteInfo;
    return CLogicThreadManager::Instance().DoJob(2, [deleteInfo]() mutable {
        TXSingleton<XRelayServer>::Instance()->DeleteRecruit(deleteInfo);
    });
}

bool CGameDBSocket::ResForceMatchingCreate(XPacket& xPacket) {
    int errorCode = 0;
    std::uint32_t matchingID = 0;
    std::uint32_t forceID = 0;
    xPacket.XParse >> errorCode;
    xPacket.XParse >> matchingID;
    xPacket.XParse >> forceID;

    return CLogicThreadManager::Instance().DoJob(0, [errorCode, matchingID, forceID]() {
        if (errorCode != 0) {
            LogHelper::LogError("game.contents",
                                "[FORCE] Failed ResForceMatchingCreate - [nErrorCode:%d]",
                                errorCode);
            return;
        }

        TXSingleton<XRelayServer>::Instance()->GetForceMatchingMgr().ResForceMatchingCreate(matchingID, forceID);
    });
}

XGameDBSocketMgr::~XGameDBSocketMgr() {
    Clear();
}

void XGameDBSocketMgr::Init() {
    Clear();
    InitAgentGroup(0, m_pGameDBAgent, m_nGameAgentCnt);
    InitAgentGroup(1, m_pAccountDBAgent, m_nAccountAgentCnt);
}

void XGameDBSocketMgr::AutoConnect() {
    AutoConnectGroup(m_pAccountDBAgent, m_nAccountAgentCnt);
    AutoConnectGroup(m_pGameDBAgent, m_nGameAgentCnt);
}

void XGameDBSocketMgr::DisConnect() {
    DisconnectGroup(m_pAccountDBAgent, m_nAccountAgentCnt);
    DisconnectGroup(m_pGameDBAgent, m_nGameAgentCnt);
}

// 对齐 IDA: 参数类型 XSendPacket& (非const)
bool XGameDBSocketMgr::SendAccountDBAgent(int iIndex, XSendPacket& xSendPacket) {
    return m_pAccountDBAgent && iIndex < GetAccountDBAgentCount() &&
           m_pAccountDBAgent[iIndex].m_bState && m_pAccountDBAgent[iIndex].Send(xSendPacket);
}

bool XGameDBSocketMgr::SendGameDBAgent(int iIndex, XSendPacket& xSendPacket) {
    return m_pGameDBAgent && iIndex < GetGameDBAgentCount() &&
           m_pGameDBAgent[iIndex].m_bState && m_pGameDBAgent[iIndex].Send(xSendPacket);
}

int XGameDBSocketMgr::GetGameDBAgentCount() {
    return m_nGameAgentCnt;
}

int XGameDBSocketMgr::GetAccountDBAgentCount() {
    return m_nAccountAgentCnt;
}

void XGameDBSocketMgr::InitAgentGroup(std::uint8_t byType, CGameDBSocket*& ppAgents, int& nAgentCount) {
    ppAgents = nullptr;
    nAgentCount = 0;

    XOption& option = TXSingleton<XRelayServer>::Instance()->GetOption();
    char szIP[513] = {};
    char szName[21] = {};
    std::int16_t shPort = 0;
    int nMaxIOPoolCount = 0;
    if (!option.GetDBAgentInfo(byType, szIP, &shPort, szName, &nMaxIOPoolCount, &nAgentCount) ||
        nAgentCount <= 0) {
        nAgentCount = 0;
        return;
    }

    ppAgents = new CGameDBSocket[static_cast<std::size_t>(nAgentCount)];
    for (int index = 0; index < nAgentCount; ++index) {
        if (ppAgents[index].Init(ePoolIDNone, szIP, shPort, szName, nMaxIOPoolCount)) {
            ppAgents[index].m_bState = true;
            ppAgents[index].m_byType = byType;
        }
    }
}

void XGameDBSocketMgr::AutoConnectGroup(CGameDBSocket* pAgents, int nAgentCount) {
    if (!pAgents || nAgentCount <= 0) {
        return;
    }

    for (int index = 0; index < nAgentCount; ++index) {
        if (pAgents[index].m_bState) {
            pAgents[index].AutoConnect();
        }
    }
}

void XGameDBSocketMgr::DisconnectGroup(CGameDBSocket* pAgents, int nAgentCount) {
    if (!pAgents || nAgentCount <= 0) {
        return;
    }

    for (int index = 0; index < nAgentCount; ++index) {
        if (pAgents[index].m_bState) {
            pAgents[index].SetSafetyShutdown(true);
            pAgents[index].DisConnect();
        }
    }
}

void XGameDBSocketMgr::Clear() {
    delete[] m_pGameDBAgent;
    delete[] m_pAccountDBAgent;
    m_pGameDBAgent = nullptr;
    m_nGameAgentCnt = 0;
    m_pAccountDBAgent = nullptr;
    m_nAccountAgentCnt = 0;
}

bool CGameDBSocket::ResPartyJoin(XPacket& xPacket) {
    PS_PARTY_ADDMEMBER stAddMember{};
    std::uint32_t dwRecruitID = 0;
    xPacket >> stAddMember;
    xPacket.XParse >> dwRecruitID;

    return CLogicThreadManager::Instance().DoJob(0, [stAddMember, dwRecruitID]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ResJoinMember(
            stAddMember, dwRecruitID);
    });
}

bool CGameDBSocket::ResForceJoin(XPacket& xPacket) {
    PS_FORCE_ADDMEMBER stAddMember{};
    std::uint32_t dwRecruitID = 0;
    xPacket >> stAddMember;
    xPacket.XParse >> dwRecruitID;

    return CLogicThreadManager::Instance().DoJob(0, [stAddMember, dwRecruitID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetForceManager().ResJoinMember(
            const_cast<PS_FORCE_ADDMEMBER&>(stAddMember), dwRecruitID);
    });
}

bool CGameDBSocket::ResPartyCreate(XPacket& xPacket) {
    PS_REQ_PARTY_CREATE stPartyRes{};
    xPacket >> stPartyRes;

    return CLogicThreadManager::Instance().DoJob(0, [stPartyRes]() mutable {
        if (stPartyRes.nErrorCode != 0) {
            LogHelper::LogError("game.contents",
                                "[PARTY] FAILED ResPartyCreate - ( errCode %d ) ",
                                static_cast<int>(stPartyRes.nErrorCode));
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().CreateParty(
            stPartyRes);
    });
}

bool CGameDBSocket::ResForceCreate(XPacket& xPacket) {
    PS_REQ_FORCE_CREATE stForceRes{};
    xPacket >> stForceRes;

    return CLogicThreadManager::Instance().DoJob(0, [stForceRes]() {
        if (stForceRes.nErrorCode != 0) {
            LogHelper::LogError("game.contents",
                                "[FORCE] FAILED ResForceCreate - ( errCode %d ) ",
                                static_cast<int>(stForceRes.nErrorCode));
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetForceManager().CreateForce(
            const_cast<PS_REQ_FORCE_CREATE&>(stForceRes));
    });
}

bool CGameDBSocket::ResPartyLeave(XPacket& xPacket) {
    int nErrorCode = 0;
    PS_PARTY_LEAVE stPartyLeave{};
    std::uint32_t dwNewMaster = 0;

    xPacket.XParse >> nErrorCode;
    xPacket >> stPartyLeave;
    xPacket.XParse >> dwNewMaster;

    return CLogicThreadManager::Instance().DoJob(0, [nErrorCode, stPartyLeave, dwNewMaster]() mutable {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.contents",
                                "[PARTY] FAILED ResPartyLeave - ( errCode %d ) ",
                                nErrorCode);
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ResPartyLeave(
            stPartyLeave, dwNewMaster);
    });
}

bool CGameDBSocket::ResPartyDelete(XPacket& xPacket) {
    PS_PARTY_LEAVE stPartyLeave{};
    xPacket >> stPartyLeave;

    return CLogicThreadManager::Instance().DoJob(0, [stPartyLeave]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ResDeleteParty(
            stPartyLeave);
    });
}

bool CGameDBSocket::ResPartyUpdateMemberInfo(XPacket& xPacket) {
    ST_UPDATE_PARTY_MEMBER stUpdateMember{};
    xPacket >> stUpdateMember;

    return CLogicThreadManager::Instance().DoJob(0, [stUpdateMember]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ResUpdateMemberInfo(
            stUpdateMember);
    });
}

bool CGameDBSocket::ResPartyChangeMaster(XPacket& xPacket) {
    PS_PARTY_CHANGE_MASTER stChangeMaster{};
    xPacket >> stChangeMaster;

    return CLogicThreadManager::Instance().DoJob(0, [stChangeMaster]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ResChangeMaster(
            stChangeMaster);
        if (stChangeMaster.nErrorCode == 0) {
            relayServer.GetPartyMatchingMgr().ReqPartyRecruitDel(stChangeMaster.dwReqActorID);
        }
    });
}

bool CGameDBSocket::ResForceUpdateMemberInfo(XPacket& xPacket) {
    ST_UPDATE_FORCE_MEMBER stUpdateMember{};
    xPacket >> stUpdateMember;

    return CLogicThreadManager::Instance().DoJob(0, [stUpdateMember]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetForceManager().ResUpdateMemberInfo(
            const_cast<ST_UPDATE_FORCE_MEMBER&>(stUpdateMember));
    });
}

bool CGameDBSocket::ResForceChangeMaster(XPacket& xPacket) {
    PS_FORCE_CHANGE_MASTER stChangeMaster{};
    xPacket >> stChangeMaster;

    return CLogicThreadManager::Instance().DoJob(0, [stChangeMaster]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetForceManager().ResChangeMaster(
            const_cast<PS_FORCE_CHANGE_MASTER&>(stChangeMaster));
    });
}

bool CGameDBSocket::ResForceLeave(XPacket& xPacket) {
    int nErrorCode = 0;
    PS_FORCE_LEAVE stForceLeave{};
    std::uint32_t dwNewMaster = 0;

    xPacket.XParse >> nErrorCode;
    xPacket >> stForceLeave;
    xPacket.XParse >> dwNewMaster;

    return CLogicThreadManager::Instance().DoJob(0, [nErrorCode, stForceLeave, dwNewMaster]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.contents",
                                "[FORCE] FAILED ResForceLeave - ( errCode %d ) ",
                                nErrorCode);
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetForceManager().ResForceLeave(
            const_cast<PS_FORCE_LEAVE&>(stForceLeave), dwNewMaster);
    });
}

bool CGameDBSocket::ResForceDelete(XPacket& xPacket) {
    PS_FORCE_LEAVE stForceLeave{};
    xPacket >> stForceLeave;

    return CLogicThreadManager::Instance().DoJob(0, [stForceLeave]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetForceManager().ResDeleteForce(
            const_cast<PS_FORCE_LEAVE&>(stForceLeave));
    });
}

bool CGameDBSocket::ResPartyLoadAll(XPacket& xPacket) {
    int nErrorCode = 0;
    std::uint8_t byEnd = 0;
    PS_PARTY_INFO_ALL stPartyInfoAll{};

    xPacket.XParse >> nErrorCode;
    xPacket.XParse >> byEnd;
    xPacket >> stPartyInfoAll;

    if (nErrorCode != 0) {
        LogHelper::LogError("game.system",
                            "<PARTY> FAILED ResPartyLoadAll - ( errCode %d )",
                            nErrorCode);
        return false;
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.GetPartyManager().ResLoadPartyAll(stPartyInfoAll, byEnd);
    return true;
}

bool CGameDBSocket::ResPartyMatchingCreate(XPacket& xPacket) {
    int nErrorCode = 0;
    std::uint32_t dwMatchingID = 0;
    std::uint32_t dwPartyID = 0;

    xPacket.XParse >> nErrorCode;
    xPacket.XParse >> dwMatchingID;
    xPacket.XParse >> dwPartyID;

    return CLogicThreadManager::Instance().DoJob(0, [nErrorCode, dwMatchingID, dwPartyID]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.contents",
                                "[LEAGUE] Failed ResPartyMatchingCreate - [nErrorCode:%d]",
                                nErrorCode);
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyMatchingMgr().ResPartyMatchingCreate(dwMatchingID, dwPartyID);
    });
}

bool CGameDBSocket::ResForceLoadAll(XPacket& xPacket) {
    int nErrorCode = 0;
    std::uint8_t byEnd = 0;
    PS_FORCE_INFO_ALL stForceInfoAll{};

    xPacket.XParse >> nErrorCode;
    xPacket.XParse >> byEnd;
    xPacket >> stForceInfoAll;

    if (nErrorCode != 0) {
        LogHelper::LogError("game.system",
                            "[FORCE] FAILED ResForceLoadAll - ( errCode %d )",
                            nErrorCode);
        return false;
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.GetForceManager().ResLoadForceAll(stForceInfoAll, byEnd);
    return true;
}

// ============================================================================
// 联赛数据库响应分发
// ============================================================================

bool CGameDBSocket::DBLeagueParse(XPacket& xPacket) {
    switch (static_cast<unsigned char>(xPacket.GetSubCmd())) {
    case 0x00: return ResLeagueCreate(xPacket);
    case 0x01: return ResLeagueDelete(xPacket);
    case 0x05: return ResLeagueBoard(xPacket);
    case 0x06: return ResLeagueWithDraw(xPacket);
    case 0x09: return ResLeagueApplicant(xPacket);
    case 0x0B: return ResLeagueKick(xPacket);
    case 0x0F: return ResLeagueInviteAccept(xPacket);
    case 0x10: return ResLeagueApplicantAccept(xPacket);
    case 0x11: return ResLeagueApplicantReject(xPacket);
    case 0x14: return ResLeagueNoticeChange(xPacket);
    case 0x15: return ResLeagueNameChange(xPacket);
    case 0x16: return ResLeagueCardChange(xPacket);
    case 0x17: return ResLeaguePositionNameChange(xPacket);
    case 0x18: return ResLeagueAuthChange(xPacket);
    case 0x19: return ResLeagueMemberPositionChange(xPacket);
    case 0x20: return ResLeagueApplicantDelete(xPacket);
    case 0x23: return ResLeagueInfo(xPacket);
    case 0x24: return ResLoadLeagueMember(xPacket);
    case 0x25: return ResLoadLeagueApplicant(xPacket);
    case 0x26: return ResLoadLeagueBoard(xPacket);
    case 0x27: return ResLeagueOpenOrNot(xPacket);
    case 0x28: return ResLeagueRecruitNotice(xPacket);
    case 0x29: return ResLeagueSearch(xPacket);
    case 0x31: return ResLeagueRecord(xPacket);
    case 0x32: return ResLeagueDelegate(xPacket);
    case 0x33: return ResLeagueWealth(xPacket);
    case 0x34: return ResLeagueLevelup(xPacket);
    case 0x35: return ResLeagueSkillLearn(xPacket);
    case 0x37: return ReqLeagueInventoryMove(xPacket);
    case 0x39: return ReqLeagueInventoryInfo(xPacket);
    case 0x41: return ResLeagueList(xPacket);
    case 0x42: return ResGMTLeagueInfo(xPacket);
    case 0x80: return ResLeagueWithdrawPenalty(xPacket);
    case 0x81: return ResLeagueDeletePenalty(xPacket);
    default:
        return false;  // 对齐 IDA: default 返回 false
    }
}

// ============================================================================
// 联赛数据库响应处理
// ============================================================================

bool CGameDBSocket::ResLeagueCreate(XPacket& xPacket) {
    PS_LEAGUE_CREATE_FOR_SERVER stCreate{};
    xPacket >> stCreate;

    // 对齐 IDA lambda2 (0x14004a710): 内含 GetServer/GetUser/nErrorCode检查/SetLeagueID
    return CLogicThreadManager::Instance().DoJob(1, [stCreate]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        // 对齐 IDA: 获取服务器
        CServer* pServer = relayServer.GetServer(static_cast<std::uint32_t>(stCreate.nServerID));
        if (pServer) {
            // 对齐 IDA: 获取用户
            auto pUser = relayServer.GetUser(stCreate.dwActorID);
            if (!pUser) {
                LogHelper::LogError("game.contents",
                    "ResLeagueCreate error - User is NULL[ UCID:%d ] ( %d )",
                    stCreate.dwActorID, 346);
            } else {
                // 对齐 IDA: nErrorCode <= 0 表示成功
                if (stCreate.stCreateInfo.nErrorCode <= 0) {
                    // 对齐 IDA: 设置用户联赛 ID
                    pUser->SetLeagueID(stCreate.stCreateInfo.nLeagueID);
                    PS_LEAGUE_CREATE_FOR_SERVER stCreateCopy = stCreate;
                    relayServer.GetLeagueManager().ResCreateLeague(pServer, stCreateCopy);
                } else {
                    LogHelper::LogError("game.contents",
                        "ResLeagueCreate error - DBFail[ ErrorCode:%d ] ( %d )",
                        stCreate.stCreateInfo.nErrorCode, 352);
                }
            }
        } else {
            LogHelper::LogError("game.contents",
                "ResLeagueCreate error - pServer is NULL[ ServerID:%d ] ( %d )",
                stCreate.nServerID, 339);
        }
    });
}

bool CGameDBSocket::ResLeagueDelete(XPacket& xPacket) {
    int nErrorCode = 0;
    std::uint32_t dwServerID = 0;
    std::uint32_t dwUCID = 0;
    int nLeagueID = 0;
    std::int64_t biPenalty = 0;

    xPacket.XParse >> nErrorCode;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> biPenalty;

    // 对齐 IDA lambda21 (0x14004cf20): 内含完整删除逻辑
    return CLogicThreadManager::Instance().DoJob(1, [nErrorCode, dwServerID, dwUCID, nLeagueID, biPenalty]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        if (nErrorCode != 0) {
            LogHelper::LogError("game.contents",
                "[LEAGUE] Failed ResLeagueDelete - nErrorCode != SQL_SUCCESS %d",
                nErrorCode);
            return;
        }
        // 对齐 IDA: 调用 DeleteLeagueMember 删除成员并获取名字
        wchar_t szDeleteName[21] = {};
        relayServer.GetLeagueManager().DeleteLeagueMember(nLeagueID, dwUCID, szDeleteName);
        // 对齐 IDA: 调用 DeleteLeague 删除联赛（IDA 原名 DelLeague）
        relayServer.GetLeagueManager().DelLeague(nLeagueID);
        // 对齐 IDA: 获取服务器
        CServer* pServer = relayServer.GetServer(dwServerID);
        if (pServer) {
            // 对齐 IDA: 获取用户并设置 LeagueID = 0
            auto pUser = relayServer.GetUser(dwUCID);
            if (pUser) {
                pUser->SetLeagueID(0);
            }
            // 对齐 IDA: 发送删除成功包 (0xF6, 2)
            XSendPacket xSendPacket(0xF6, 2);
            xSendPacket.XParse << dwUCID;
            xSendPacket.XParse << nLeagueID;
            xSendPacket.XParse << biPenalty;
            xSendPacket.XParse << nErrorCode;
            pServer->SendEx(xSendPacket);
        } else {
            LogHelper::LogError("game.contents",
                "[LEAGUE] Failed ResLeagueDelete - pServer == NULL %d",
                dwServerID);
        }
    });
}

bool CGameDBSocket::ResLeagueBoard(XPacket& xPacket) {
    int nErrorCode = 0;
    ST_LEAGUE_BOARD stBoard{};
    std::uint32_t dwServerID = 0;
    int nLeagueID = 0;
    std::uint32_t dwActorID = 0;

    xPacket.XParse >> nErrorCode;
    xPacket >> stBoard;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwActorID;

    return CLogicThreadManager::Instance().DoJob(1, [nErrorCode, stBoard, dwServerID, nLeagueID, dwActorID]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.league",
                                "[LEAGUE] FAILED ResLeagueBoard - ( errCode %d leagueID %d )",
                                nErrorCode, nLeagueID);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(dwServerID);
        relayServer.GetLeagueManager().ResLeagueBoard(pServer, dwActorID, nLeagueID, stBoard);
    });
}

bool CGameDBSocket::ResLeagueWithDraw(XPacket& xPacket) {
    int nErrorCode = 0;
    std::uint32_t dwActorID = 0;
    int nLeagueID = 0;
    std::int64_t biPenalty = 0;
    std::uint32_t dwServerID = 0;

    xPacket.XParse >> nErrorCode;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> biPenalty;
    xPacket.XParse >> dwServerID;

    return CLogicThreadManager::Instance().DoJob(1, [nErrorCode, dwActorID, nLeagueID, biPenalty, dwServerID]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.league",
                                "[LEAGUE] FAILED ResLeagueWithDraw - ( errCode %d leagueID %d )",
                                nErrorCode, nLeagueID);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        // 对齐 IDA: DB 响应后调用 ResLeagueWithdraw 而非 ReqLeagueWithDraw
        relayServer.GetLeagueManager().ResLeagueWithdraw(nLeagueID, dwActorID, biPenalty);
    });
}

bool CGameDBSocket::ResLeagueApplicant(XPacket& xPacket) {
    ST_LEAGUE_APPLICANT stApplicant{};
    int nServerID = 0;

    xPacket >> stApplicant;
    xPacket.XParse >> nServerID;

    return CLogicThreadManager::Instance().DoJob(1, [stApplicant, nServerID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ResLeagueApplicant(
            nullptr, stApplicant);
    });
}

bool CGameDBSocket::ResLeagueKick(XPacket& xPacket) {
    int nErrorCode = 0;
    std::uint32_t dwActorID = 0;
    std::uint32_t dwTargetID = 0;
    int nLeagueID = 0;
    std::uint32_t dwServerID = 0;

    xPacket.XParse >> nErrorCode;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> dwTargetID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwServerID;

    return CLogicThreadManager::Instance().DoJob(1, [nErrorCode, dwActorID, dwTargetID, nLeagueID, dwServerID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        // 对齐 IDA: DB 响应后调用 ResLeagueKickout 而非 ReqLeagueKick
        // ResLeagueKickout 内部处理成功和失败两种情况
        CServer* pServer = relayServer.GetServer(dwServerID);
        relayServer.GetLeagueManager().ResLeagueKickout(
            dwActorID, dwTargetID, nLeagueID, nErrorCode, pServer);
    });
}

bool CGameDBSocket::ResLeagueInviteAccept(XPacket& xPacket) {
    ST_REQ_LEAGUE_INVITE_ACCEPT stAccept{};
    ST_LEAGUE_MEMBER_EX stMemberEx{};
    std::uint32_t dwServerID = 0;

    xPacket >> stAccept;
    xPacket >> stMemberEx;
    xPacket.XParse >> dwServerID;

    // 对齐 IDA 0x14004cb20: lambda 调用 ResInviteUser 而非 ReqInviteAccept
    return CLogicThreadManager::Instance().DoJob(1, [stAccept, stMemberEx, dwServerID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

        // 对齐 IDA: 获取服务器并设置被邀请用户联赛ID
        CServer* pServer = relayServer.GetServer(dwServerID);
        if (pServer) {
            auto pUser = relayServer.GetUser(stMemberEx.dwUCID);
            if (pUser) {
                pUser->SetLeagueID(stAccept.nLeagueID);
            }
            relayServer.GetLeagueManager().DeleteInviteUser(stMemberEx.dwUCID);
            relayServer.GetLeagueManager().ResInviteUser(
                pServer, stAccept.nLeagueID, stMemberEx, stAccept.dwReqUCID);
        } else {
            LogHelper::LogError("game.contents",
                               "[LEAGUE] Failed ResLeagueInviteAccept - pServer == NULL %d",
                               dwServerID);
        }
    });
}

bool CGameDBSocket::ResLeagueApplicantAccept(XPacket& xPacket) {
    ST_REQ_LEAGUE_APPLICANT_ACCEPT stAccept{};
    int nServerID = 0;
    ST_LEAGUE_MEMBER_EX stMemberEx{};
    std::uint32_t dwActorID = 0;

    xPacket >> stAccept;
    xPacket.XParse >> nServerID;
    xPacket >> stMemberEx;
    xPacket.XParse >> dwActorID;

    // 对齐 IDA: lambda 调用 AppliCantJoinSucc
    return CLogicThreadManager::Instance().DoJob(1, [stAccept, nServerID, stMemberEx, dwActorID]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(nServerID);
        if (pServer) {
            relayServer.GetLeagueManager().AppliCantJoinSucc(
                pServer, stAccept, stMemberEx, dwActorID);
        } else {
            LogHelper::LogError("game.contents",
                               "[LEAGUE] Failed ResLeagueApplicantAccept - pServer == NULL %d",
                               nServerID);
        }
    });
}

bool CGameDBSocket::ResLeagueApplicantReject(XPacket& xPacket) {
    ST_REQ_LEAGUE_APPLICANT_REJECT stReject{};
    std::uint32_t dwServerID = 0;
    int nErrorCode = 0;

    xPacket >> stReject;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> nErrorCode;

    return CLogicThreadManager::Instance().DoJob(1, [stReject, dwServerID, nErrorCode]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.league",
                                "[LEAGUE] FAILED ResLeagueApplicantReject - ( errCode %d )",
                                nErrorCode);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        // 对齐 IDA: DB 响应后调用 ApplicantRejectSucc 而非 ReqLeagueApplicantReject
        CServer* pServer = relayServer.GetServer(dwServerID);
        relayServer.GetLeagueManager().ApplicantRejectSucc(pServer, stReject);
    });
}

bool CGameDBSocket::ResLeagueNoticeChange(XPacket& xPacket) {
    ST_LEAGUE_NOTICE stNotice{};
    std::uint32_t dwServerID = 0;
    std::uint32_t dwActorID = 0;
    int nErrorCode = 0;

    xPacket >> stNotice;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nErrorCode;

    return CLogicThreadManager::Instance().DoJob(1, [stNotice, dwServerID, dwActorID, nErrorCode]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.league",
                                "[LEAGUE] FAILED ResLeagueNoticeChange - ( errCode %d )",
                                nErrorCode);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(dwServerID);
        relayServer.GetLeagueManager().ResLeagueNoticeChange(pServer, stNotice, dwActorID);
    });
}

// 对齐 IDA 0x140050940: 获取 pServer, lambda 包含 pServer 和 nResult 分支逻辑
bool CGameDBSocket::ResLeagueNameChange(XPacket& xPacket) {
    PS_LEAGUE_NAME_CHANGE_SERVER stChange{};

    xPacket >> stChange;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    CServer* pServer = relayServer.GetServer(stChange.dwServerID);

    return CLogicThreadManager::Instance().DoJob(1, [pServer, stChange]() mutable {
        if (!pServer) {
            return;
        }

        if (stChange.nResult <= 0) {
            // 成功或需要处理的响应
            XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
            bool bSuccess = relayServer.GetLeagueManager().ResLeaugeNameChange(stChange);
            if (bSuccess) {
                // 发送成功响应 0xF6/0x25
                XSendPacket xSendPacket(0xF6, 0x25);
                xSendPacket << stChange;
                pServer->SendEx(xSendPacket);
            }
        } else {
            // 错误响应 nResult > 0
            XSendPacket xSendPacket(0xF6, 0x25);
            xSendPacket << stChange;
            pServer->SendEx(xSendPacket);
        }
    });
}

// 对齐 IDA 0x14004f3b0: psCardInfo >> dwUCID >> dwServerID >> vecUpdateItem >> nErrorCode
bool CGameDBSocket::ResLeagueCardChange(XPacket& xPacket) {
    PS_REQ_LEAGUE_CARD stCard{};
    std::uint32_t dwUCID = 0;
    std::uint32_t dwServerID = 0;
    PS_RES_STORAGE_INFO stStorage{};
    int nErrorCode = 0;

    xPacket >> stCard;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> dwServerID;
    xPacket >> stStorage;
    xPacket.XParse >> nErrorCode;

    return CLogicThreadManager::Instance().DoJob(1, [dwServerID, dwUCID, stCard, stStorage, nErrorCode]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.league",
                                "[LEAGUE] FAILED ResLeagueCardChange - ( errCode %d )",
                                nErrorCode);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(dwServerID);
        // 对齐 IDA: 调用 ResLeagueCardChange
        PS_REQ_LEAGUE_CARD stCardCopy = stCard;
        PS_RES_STORAGE_INFO stStorageCopy = stStorage;
        relayServer.GetLeagueManager().ResLeagueCardChange(
            pServer, stCardCopy, dwUCID, stStorageCopy, nErrorCode);
    });
}

bool CGameDBSocket::ResLeaguePositionNameChange(XPacket& xPacket) {
    ST_LEAGUE_POSITION_NAME_CHANGE stChange{};
    int nLeagueID = 0;
    std::uint32_t dwServerID = 0;
    std::uint32_t dwActorID = 0;

    // 对齐 IDA 0x14004ddd0: stChange >> nLeagueID >> nServerID >> dwActorID
    // 注意：第四个参数是 dwActorID，不是 nErrorCode
    xPacket >> stChange;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> dwActorID;

    return CLogicThreadManager::Instance().DoJob(1, [stChange, nLeagueID, dwServerID, dwActorID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(dwServerID);
        // 对齐 IDA: DB 响应后调用 ResLeaguePositionNameChange
        // IDA 签名: void ResLeaguePositionNameChange(CServer*, int nLeagueID, unsigned int dwActorID, ST_LEAGUE_POSITION_NAME_CHANGE*)
        relayServer.GetLeagueManager().ResLeaguePositionNameChange(pServer, nLeagueID, dwActorID, stChange);
    });
}

bool CGameDBSocket::ResLeagueAuthChange(XPacket& xPacket) {
    ST_LEAGUE_AUTH_CHANGE stChange{};
    int nLeagueID = 0;
    int nServerID = 0;
    std::uint32_t dwActorID = 0;

    xPacket >> stChange;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> nServerID;
    xPacket.XParse >> dwActorID;

    return CLogicThreadManager::Instance().DoJob(1, [stChange, nLeagueID, nServerID, dwActorID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        // 对齐 IDA: DB 响应后调用 ResLeagueAuthChange 而非 ReqLeagueChangeAuth
        CServer* pServer = relayServer.GetServer(nServerID);
        relayServer.GetLeagueManager().ResLeagueAuthChange(pServer, nLeagueID, stChange, dwActorID);
    });
}

bool CGameDBSocket::ResLeagueMemberPositionChange(XPacket& xPacket) {
    ST_LEAGUE_MEMBER_POSITION stPos{};
    std::uint32_t dwActorID = 0;
    int nLeagueID = 0;
    std::uint32_t dwServerID = 0;
    int nErrorCode = 0;

    // 对齐 IDA 0x14004e060: nErrorCode >> stPosition >> nLeagueID >> dwActorID >> nServerID
    xPacket.XParse >> nErrorCode;
    xPacket >> stPos;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> dwServerID;

    return CLogicThreadManager::Instance().DoJob(1, [stPos, dwActorID, nLeagueID, dwServerID, nErrorCode]() mutable {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.league",
                                "[LEAGUE] FAILED ResLeagueMemberPositionChange - ( errCode %d )",
                                nErrorCode);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(dwServerID);
        relayServer.GetLeagueManager().ResLeagueMemberPositionChange(pServer, stPos, nLeagueID, dwActorID);
    });
}

bool CGameDBSocket::ResLeagueApplicantDelete(XPacket& xPacket) {
    int nLeagueID = 0;
    std::uint32_t dwActorID = 0;

    // 对齐 IDA 0x14004e300: nLeagueID >> dwActorID
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwActorID;

    return CLogicThreadManager::Instance().DoJob(1, [nLeagueID, dwActorID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        // 对齐 IDA 0x140076AE0: DeleteApplicantList(CServer*, dwActorID)
        // nLeagueID 被 lambda 捕获但 DeleteApplicantList 函数签名不使用它
        static_cast<void>(nLeagueID);
        relayServer.GetLeagueManager().DeleteApplicantList(nullptr, dwActorID);
    });
}

bool CGameDBSocket::ResLeagueInfo(XPacket& xPacket) {
    ST_LEAGUE_INFO stLeagueInfo{};
    ST_LEAGUE_MEMBER_LIST stMemberList{};
    ST_LEAGUE_BOARD_LIST stBoardList{};
    ST_LEAGUE_APPLICANT_LIST stApplicantList{};
    ST_LEAGUE_RECORD_LIST stRecordList{};
    PS_DB_LEAGUE_LOAD psDBLoadInfo{};
    int nDBErrorCode = 0;

    xPacket >> stLeagueInfo;
    xPacket >> stMemberList;
    xPacket >> stBoardList;
    xPacket >> stApplicantList;
    xPacket >> stRecordList;
    xPacket >> psDBLoadInfo;
    xPacket.XParse >> nDBErrorCode;

    // 对齐 IDA 0x140051200: lambda 检查 nDBErrorCode, 非零则 LogError 不调用 LoadLeagueInfo
    return CLogicThreadManager::Instance().DoJob(1,
        [stLeagueInfo, stMemberList, stBoardList, stApplicantList, stRecordList, psDBLoadInfo, nDBErrorCode]() {
            if (nDBErrorCode != 0) {
                LogHelper::LogError("game.league", "ResLeagueInfo error - Failed Load League");
                return;
            }
            XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
            relayServer.GetLeagueManager().LoadLeagueInfo(
                psDBLoadInfo, stLeagueInfo, stMemberList, stBoardList, stApplicantList, stRecordList);
        });
}

bool CGameDBSocket::ResLoadLeagueMember(XPacket& xPacket) {
    bool bSuccess = false;
    ST_LEAGUE_MEMBER_LIST stMemberList{};

    xPacket.XParse >> bSuccess;
    xPacket >> stMemberList;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.GetLeagueManager().ResLoadLeagueMember(bSuccess, stMemberList);
    return true;
}

bool CGameDBSocket::ResLoadLeagueApplicant(XPacket& xPacket) {
    bool bSuccess = false;
    ST_LEAGUE_APPLICANT_LIST stApplicantList{};

    xPacket.XParse >> bSuccess;
    xPacket >> stApplicantList;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.GetLeagueManager().ResLoadLeagueApplicant(bSuccess, stApplicantList);
    return true;
}

bool CGameDBSocket::ResLoadLeagueBoard(XPacket& xPacket) {
    bool bSuccess = false;
    ST_LEAGUE_BOARD_LIST stBoardList{};

    xPacket.XParse >> bSuccess;
    xPacket >> stBoardList;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.GetLeagueManager().ResLoadLeagueBoard(bSuccess, stBoardList);
    return true;
}

bool CGameDBSocket::ResLeagueOpenOrNot(XPacket& xPacket) {
    ST_LEAGUE_OPEN stOpen{};
    std::uint32_t dwServerID = 0;
    std::uint32_t dwUCID = 0;

    // 对齐 IDA 0x14004e970: stOpen >> dwServerID >> dwUCID（无 nErrorCode）
    xPacket >> stOpen;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> dwUCID;

    return CLogicThreadManager::Instance().DoJob(1, [stOpen, dwServerID, dwUCID]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(dwServerID);
        // 对齐 IDA: 无 nErrorCode 检查，直接调用 ResLeagueOpenOrNot
        relayServer.GetLeagueManager().ResLeagueOpenOrNot(pServer, stOpen, dwUCID);
    });
}

bool CGameDBSocket::ResLeagueRecruitNotice(XPacket& xPacket) {
    ST_LEAGUE_RECRUIT_NOTICE stNotice{};
    std::uint32_t dwServerID = 0;
    std::uint32_t dwUCID = 0;

    // 对齐 IDA 0x14004eb50: stRecruitNotice >> dwServerID >> dwUCID（无 nErrorCode）
    xPacket >> stNotice;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> dwUCID;

    return CLogicThreadManager::Instance().DoJob(1, [stNotice, dwServerID, dwUCID]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(dwServerID);
        // 对齐 IDA: 无 nErrorCode 检查，直接调用 ResLeagueRecruitNotice
        relayServer.GetLeagueManager().ResLeagueRecruitNotice(
            pServer, dwUCID, stNotice);
    });
}

bool CGameDBSocket::ResLeagueSearch(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwServerID = 0;
    PS_LEAGUE_SUMMARY_LIST psLeagueSummaryList{};

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> dwServerID;
    xPacket >> psLeagueSummaryList;

    return CLogicThreadManager::Instance().DoJob(1, [dwUCID, dwServerID, psLeagueSummaryList]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(dwServerID);
        if (pServer) {
            relayServer.GetLeagueManager().ResLeagueSearch(pServer, psLeagueSummaryList, dwUCID);
        } else {
            LogHelper::LogError("game.contents",
                                "[LEAGUE] Failed ResLeagueSearch - pServer == NULL [serverID:%u]",
                                dwServerID);
        }
    });
}

bool CGameDBSocket::ResLeagueRecord(XPacket& xPacket) {
    // 对齐 IDA 0x14004f0b0: 直接调用 ResLoadLeagueRecord（不经过 DoJob）
    bool bLoadRecord = false;
    ST_LEAGUE_RECORD_LIST stRecordList{};

    xPacket.XParse >> bLoadRecord;
    xPacket >> stRecordList;

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    relayServer.GetLeagueManager().ResLoadLeagueRecord(bLoadRecord, stRecordList);
    return true;
}

bool CGameDBSocket::ResLeagueDelegate(XPacket& xPacket) {
    PS_REQ_LEAGUE_DELEGATE psDelegateReq{};
    std::uint32_t dwServerID = 0;
    std::uint32_t dwReqUCID = 0;
    int nErrorCode = 0;

    xPacket >> psDelegateReq;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> dwReqUCID;
    xPacket.XParse >> nErrorCode;

    return CLogicThreadManager::Instance().DoJob(1, [psDelegateReq, dwServerID, dwReqUCID, nErrorCode]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(dwServerID);
        relayServer.GetLeagueManager().ResLeagueDelegate(
            pServer, dwReqUCID, psDelegateReq, nErrorCode);
    });
}

bool CGameDBSocket::ResLeagueWealth(XPacket& xPacket) {
    PS_LEAGUE_WEALTH_FOR_SERVER stWealth{};

    xPacket >> stWealth;

    return CLogicThreadManager::Instance().DoJob(1, [stWealth]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ResApplyLeagueWealth(stWealth);
    });
}

bool CGameDBSocket::ResLeagueLevelup(XPacket& xPacket) {
    // 对齐 IDA - DB 返回 levelup 结果（非作弊路径）
    std::int32_t nLeagueID = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t bySkillPoint = 0;
    PS_AUTO_SKILL stSkill{};
    std::uint32_t dwUCID = 0;

    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> byLevel;
    xPacket.XParse >> bySkillPoint;
    xPacket >> stSkill;
    xPacket.XParse >> dwUCID;

    return CLogicThreadManager::Instance().DoJob(1, [nLeagueID, byLevel, bySkillPoint, stSkill, dwUCID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ResLeagueLevelup(nLeagueID, byLevel, bySkillPoint, stSkill, dwUCID);
    });
}

bool CGameDBSocket::ResLeagueSkillLearn(XPacket& xPacket) {
    // 对齐 IDA - DB 返回 PS_RES_LEAGUE_SKILL + byType
    PS_RES_LEAGUE_SKILL stSkill{};
    std::uint8_t byType = 0;

    xPacket >> stSkill;
    xPacket.XParse >> byType;

    return CLogicThreadManager::Instance().DoJob(1, [stSkill, byType]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ResLeagueSkillLearn(stSkill, byType);
    });
}

bool CGameDBSocket::ReqLeagueInventoryMove(XPacket& xPacket) {  // 对齐 IDA: Req 不是 Res
    // 对齐 IDA lambda41: DB 返回仓库移动结果
    std::uint32_t dwReqUCID = 0;
    PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME stMove{};

    xPacket.XParse >> dwReqUCID;
    xPacket >> stMove;

    return CLogicThreadManager::Instance().DoJob(1, [dwReqUCID, stMove]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ResLeagueInventoryMove(dwReqUCID, stMove);
    });
}

bool CGameDBSocket::ReqLeagueInventoryInfo(XPacket& xPacket) {  // 对齐 IDA: Req 不是 Res
    // 对齐 IDA lambda40: DB 返回仓库信息
    std::int32_t nLeagueID = 0;
    std::uint32_t dwReqUCID = 0;
    PS_RES_STORAGE_INFO psResInvenInfo{};
    PS_ITEM_BROACH_LIST psBroachList{};
    PS_ITEM_SOCKET_LIST psSocketList{};
    PS_ITEM_PACKAGE_LIST psPackageList{};

    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwReqUCID;
    xPacket >> psResInvenInfo;
    xPacket >> psBroachList;
    xPacket >> psSocketList;
    xPacket >> psPackageList;

    return CLogicThreadManager::Instance().DoJob(1, [nLeagueID, dwReqUCID, psResInvenInfo, psBroachList, psSocketList, psPackageList]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ResLeagueInventoryInfo(nLeagueID, dwReqUCID, psResInvenInfo, psBroachList, psSocketList, psPackageList);
    });
}

bool CGameDBSocket::ResLeagueList(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwServerID = 0;
    PS_LEAGUE_SUMMARY_LIST psLeagueList{};

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> dwServerID;
    xPacket >> psLeagueList;

    return CLogicThreadManager::Instance().DoJob(1, [dwUCID, dwServerID, psLeagueList]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(dwServerID);
        if (pServer) {
            relayServer.GetLeagueManager().ResLeagueList(pServer, psLeagueList, dwUCID);
        } else {
            LogHelper::LogError("game.contents",
                                "[LEAGUE] Failed ResLeagueList - pServer == NULL [serverID:%u]",
                                dwServerID);
        }
    });
}

// 对齐 IDA 0x140051820: 解析 ST_LEAGUE_LIST + ST_LEAGUE_MEMBER_LIST，调用 UpdateGMTLeagueInfo
bool CGameDBSocket::ResGMTLeagueInfo(XPacket& xPacket) {
    ST_LEAGUE_LIST stList{};
    ST_LEAGUE_MEMBER_LIST stMembers{};

    xPacket >> stList;
    xPacket >> stMembers;

    return CLogicThreadManager::Instance().DoJob(1, [stList, stMembers]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().UpdateGMTLeagueInfo(stList, stMembers);
    });
}

// 对齐 IDA 0x14004e7e0: 从 DB 返回包读取 dwUCID + biPenalty，投递 worker-1 获取用户并设置惩罚值
bool CGameDBSocket::ResLeagueWithdrawPenalty(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    std::int64_t biPenalty = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biPenalty;

    return CLogicThreadManager::Instance().DoJob(1, [dwUCID, biPenalty]() {
        // 对齐 IDA lambda 0x140088650: GetUser -> SetLeagueWithdrawPenalty
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        auto pUser = relayServer.GetUser(dwUCID);
        if (!pUser) {
            LogHelper::LogError("game.relay", "[LEAUGE] ReqLeagueWithdrawPenalty - if( pUser ) %u", dwUCID);
            return;
        }
        pUser->SetLeagueWithdrawPenalty(biPenalty);
    });
}

// 对齐 IDA 0x14004e690: 从 DB 返回包读取 dwUCID + biPenalty，投递 worker-1 获取用户并设置惩罚值
bool CGameDBSocket::ResLeagueDeletePenalty(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    std::int64_t biPenalty = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biPenalty;

    return CLogicThreadManager::Instance().DoJob(1, [dwUCID, biPenalty]() {
        // 对齐 IDA lambda 0x14004e780: GetUser -> SetLeagueDeletePenalty
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        auto pUser = relayServer.GetUser(dwUCID);
        if (!pUser) {
            return;
        }
        pUser->SetLeagueDeletePenalty(biPenalty);
    });
}

bool CGameDBSocket::DBWorldParse(CServer* pServer, XPacket& xPacket) {
    static_cast<void>(pServer);
    static_cast<void>(xPacket);
    // IDA 0x14004a850: just returns 1
    return true;
}

bool CGameDBSocket::DBExchangeParse(XPacket& xPacket) {
    // IDA 0x14004a890: sub==2 -> ResExchangePriceHistory
    if (xPacket.GetSubCmd() == 2) {
        return ResExchangePriceHistory(xPacket);
    }
    return true;
}

bool CGameDBSocket::DBHelperParse(XPacket& xPacket) {
    // IDA 0x14004a8e0: sub==3 -> ResHelperSupportEquip
    if (xPacket.GetSubCmd() == 3) {
        return ResHelperSupportEquip(xPacket);
    }
    return true;
}

bool CGameDBSocket::ResExchangePriceHistory(XPacket& xPacket) {
    // 对齐 IDA 0x14004E430: PS_DB_EXCHANGE_PRICE_HISTORY_RES>> + ResExchangePriceList
    PS_DB_EXCHANGE_PRICE_HISTORY_RES psHistory{};
    xPacket >> psHistory;
    TXSingleton<XRelayServer>::Instance()->ResExchangePriceList(psHistory);
    return true;
}

bool CGameDBSocket::ResHelperSupportEquip(XPacket& xPacket) {
    // 对齐 IDA 0x14004E930: PS_DB_HELPER_SUPPORT_EQUIP>> + ResHelperSupportEquip
    PS_DB_HELPER_SUPPORT_EQUIP psEquip{};
    xPacket >> psEquip;
    TXSingleton<XRelayServer>::Instance()->ResHelperSupportEquip(psEquip);
    return true;
}
