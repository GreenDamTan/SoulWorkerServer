#include "Soulworker/GameServer/XRelayServer/GameDBSocket.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

CServer* CGameDBSocket::FindUser(unsigned int xSessionID) {
    static_cast<void>(xSessionID);
    return nullptr;
}

bool CGameDBSocket::OnParse(XPacket& xPacket) {
    int xSessionID = 0;
    xPacket.XParse >> xSessionID;

    CServer* pUser = FindUser(static_cast<unsigned int>(xSessionID));
    if (pUser && !pUser->GetBlockType()) {
        return DBParse(pUser, xPacket);
    }

    if (!pUser) {
        const std::uint8_t byMainCmd = xPacket.GetMainCmd();
        if (byMainCmd == 8u || byMainCmd == 37u || byMainCmd == 1u || byMainCmd == 40u || byMainCmd == 5u) {
            return DBParse(nullptr, xPacket);
        }
    }

    return false;
}

bool CGameDBSocket::DBParse(CServer* pServer, XPacket& xPacket) {
    static_cast<void>(pServer);
    switch (static_cast<unsigned char>(xPacket.GetMainCmd())) {
    case 4:
        return DBPartyParse(xPacket);
    case 5:
        return DBFriendParse(xPacket);
    case 7:
        return DBLeagueParse(xPacket);
    case 8:
        return DBForceParse(xPacket);
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
    switch (static_cast<unsigned char>(xPacket.GetSubCmd())) {
    case 0x10:
        return ResRecruitDelete(xPacket);
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

bool CGameDBSocket::ResRecruitDelete(XPacket& xPacket) {
    PS_RES_RECRUIT_DELETE deleteInfo{};
    xPacket >> deleteInfo;
    return CLogicThreadManager::Instance().DoJob(2, [deleteInfo]() {
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

bool XGameDBSocketMgr::SendAccountDBAgent(int iIndex, const XSendDBPacket& xSendPacket) {
    return m_pAccountDBAgent && iIndex < GetAccountDBAgentCount() &&
           m_pAccountDBAgent[iIndex].m_bState && m_pAccountDBAgent[iIndex].Send(xSendPacket);
}

bool XGameDBSocketMgr::SendGameDBAgent(int iIndex, const XSendDBPacket& xSendPacket) {
    return m_pGameDBAgent && iIndex < GetGameDBAgentCount() &&
           m_pGameDBAgent[iIndex].m_bState && m_pGameDBAgent[iIndex].Send(xSendPacket);
}

int XGameDBSocketMgr::GetGameDBAgentCount() const {
    return m_nGameAgentCnt;
}

int XGameDBSocketMgr::GetAccountDBAgentCount() const {
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

    return CLogicThreadManager::Instance().DoJob(0, [stAddMember, dwRecruitID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ResJoinMember(
            const_cast<PS_PARTY_ADDMEMBER&>(stAddMember), dwRecruitID);
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

    return CLogicThreadManager::Instance().DoJob(0, [stPartyRes]() {
        if (stPartyRes.nErrorCode != 0) {
            LogHelper::LogError("game.contents",
                                "[PARTY] FAILED ResPartyCreate - ( errCode %d ) ",
                                static_cast<int>(stPartyRes.nErrorCode));
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().CreateParty(
            const_cast<PS_REQ_PARTY_CREATE&>(stPartyRes));
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

    return CLogicThreadManager::Instance().DoJob(0, [nErrorCode, stPartyLeave, dwNewMaster]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.contents",
                                "[PARTY] FAILED ResPartyLeave - ( errCode %d ) ",
                                nErrorCode);
            return;
        }

        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ResPartyLeave(
            const_cast<PS_PARTY_LEAVE&>(stPartyLeave), dwNewMaster);
    });
}

bool CGameDBSocket::ResPartyDelete(XPacket& xPacket) {
    PS_PARTY_LEAVE stPartyLeave{};
    xPacket >> stPartyLeave;

    return CLogicThreadManager::Instance().DoJob(0, [stPartyLeave]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ResDeleteParty(
            const_cast<PS_PARTY_LEAVE&>(stPartyLeave));
    });
}

bool CGameDBSocket::ResPartyUpdateMemberInfo(XPacket& xPacket) {
    ST_UPDATE_PARTY_MEMBER stUpdateMember{};
    xPacket >> stUpdateMember;

    return CLogicThreadManager::Instance().DoJob(0, [stUpdateMember]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ResUpdateMemberInfo(
            const_cast<ST_UPDATE_PARTY_MEMBER&>(stUpdateMember));
    });
}

bool CGameDBSocket::ResPartyChangeMaster(XPacket& xPacket) {
    PS_PARTY_CHANGE_MASTER stChangeMaster{};
    xPacket >> stChangeMaster;

    return CLogicThreadManager::Instance().DoJob(0, [stChangeMaster]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetPartyManager().ResChangeMaster(
            const_cast<PS_PARTY_CHANGE_MASTER&>(stChangeMaster));
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
        return true;
    }
}

// ============================================================================
// 联赛数据库响应处理
// ============================================================================

bool CGameDBSocket::ResLeagueCreate(XPacket& xPacket) {
    PS_LEAGUE_CREATE_FOR_SERVER stCreate{};
    xPacket >> stCreate;

    return CLogicThreadManager::Instance().DoJob(1, [stCreate]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeagueCreate(
            nullptr, stCreate);
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

    return CLogicThreadManager::Instance().DoJob(1, [nErrorCode, dwServerID, dwUCID, nLeagueID, biPenalty]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeagueDel(
            nullptr, nLeagueID, nErrorCode, biPenalty);
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
        relayServer.GetLeagueManager().ReqLeagueBoard(
            nullptr, dwActorID, stBoard, nLeagueID);
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
        UXActorID uxActorID;
        uxActorID.dwActorID = dwActorID;
        relayServer.GetLeagueManager().ReqLeagueWithDraw(
            nullptr, uxActorID, nLeagueID, biPenalty);
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
        if (nErrorCode != 0) {
            LogHelper::LogError("game.league",
                                "[LEAGUE] FAILED ResLeagueKick - ( errCode %d leagueID %d )",
                                nErrorCode, nLeagueID);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeagueKick(
            nullptr, dwActorID, dwTargetID, nLeagueID);
    });
}

bool CGameDBSocket::ResLeagueInviteAccept(XPacket& xPacket) {
    ST_REQ_LEAGUE_INVITE_ACCEPT stAccept{};
    ST_LEAGUE_MEMBER_EX stMemberEx{};
    std::uint32_t dwServerID = 0;

    xPacket >> stAccept;
    xPacket >> stMemberEx;
    xPacket.XParse >> dwServerID;

    return CLogicThreadManager::Instance().DoJob(1, [stAccept, stMemberEx, dwServerID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqInviteAccept(
            nullptr, stAccept, 0);
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

    return CLogicThreadManager::Instance().DoJob(1, [stAccept, nServerID, stMemberEx, dwActorID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeagueApplicantAccept(
            nullptr, stAccept, dwActorID);
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
        // 使用 stReject.dwUCID 作为操作者ID
        relayServer.GetLeagueManager().ReqLeagueApplicantReject(
            nullptr, stReject, stReject.dwUCID);
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
        relayServer.GetLeagueManager().ReqLeagueNoticeChange(
            nullptr, dwActorID, stNotice);
    });
}

bool CGameDBSocket::ResLeagueNameChange(XPacket& xPacket) {
    PS_LEAGUE_NAME_CHANGE_SERVER stChange{};

    xPacket >> stChange;

    return CLogicThreadManager::Instance().DoJob(1, [stChange]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeagueNameChange(stChange);
    });
}

bool CGameDBSocket::ResLeagueCardChange(XPacket& xPacket) {
    PS_REQ_LEAGUE_CARD stCard{};
    PS_RES_STORAGE_INFO stStorage{};
    std::uint32_t dwActorID = 0;
    int nErrorCode = 0;

    xPacket >> stCard;
    xPacket >> stStorage;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nErrorCode;

    return CLogicThreadManager::Instance().DoJob(1, [stCard, stStorage, dwActorID, nErrorCode]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.league",
                                "[LEAGUE] FAILED ResLeagueCardChange - ( errCode %d )",
                                nErrorCode);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeagueCardChange(
            nullptr, dwActorID, stCard, stStorage);
    });
}

bool CGameDBSocket::ResLeaguePositionNameChange(XPacket& xPacket) {
    ST_LEAGUE_POSITION_NAME_CHANGE stChange{};
    int nLeagueID = 0;
    std::uint32_t dwServerID = 0;
    int nErrorCode = 0;

    xPacket >> stChange;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> nErrorCode;

    return CLogicThreadManager::Instance().DoJob(1, [stChange, nLeagueID, dwServerID, nErrorCode]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.league",
                                "[LEAGUE] FAILED ResLeaguePositionNameChange - ( errCode %d leagueID %d )",
                                nErrorCode, nLeagueID);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeaguePositionNameChange(
            nullptr, nLeagueID, stChange, dwServerID);
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
        relayServer.GetLeagueManager().ReqLeagueChangeAuth(
            nullptr, nLeagueID, dwActorID, stChange);
    });
}

bool CGameDBSocket::ResLeagueMemberPositionChange(XPacket& xPacket) {
    ST_LEAGUE_MEMBER_POSITION stPos{};
    std::uint32_t dwActorID = 0;
    int nLeagueID = 0;
    std::uint32_t dwServerID = 0;
    int nErrorCode = 0;

    xPacket >> stPos;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nLeagueID;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> nErrorCode;

    return CLogicThreadManager::Instance().DoJob(1, [stPos, dwActorID, nLeagueID, dwServerID, nErrorCode]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.league",
                                "[LEAGUE] FAILED ResLeagueMemberPositionChange - ( errCode %d )",
                                nErrorCode);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeagueMemberPositionChange(
            nullptr, stPos, dwActorID, nLeagueID);
    });
}

bool CGameDBSocket::ResLeagueApplicantDelete(XPacket& xPacket) {
    std::uint32_t dwActorID = 0;

    xPacket.XParse >> dwActorID;

    return CLogicThreadManager::Instance().DoJob(1, [dwActorID]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
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

    return CLogicThreadManager::Instance().DoJob(1,
        [stLeagueInfo, stMemberList, stBoardList, stApplicantList, stRecordList, psDBLoadInfo, nDBErrorCode]() {
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
    std::uint32_t dwActorID = 0;
    int nErrorCode = 0;

    xPacket >> stOpen;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nErrorCode;

    return CLogicThreadManager::Instance().DoJob(1, [stOpen, dwServerID, dwActorID, nErrorCode]() {
        if (nErrorCode != 0) {
            LogHelper::LogError("game.league",
                                "[LEAGUE] FAILED ResLeagueOpenOrNot - ( errCode %d )",
                                nErrorCode);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeagueOpenOrNot(
            nullptr, stOpen, dwActorID);
    });
}

bool CGameDBSocket::ResLeagueRecruitNotice(XPacket& xPacket) {
    ST_LEAGUE_RECRUIT_NOTICE stNotice{};
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
                                "[LEAGUE] FAILED ResLeagueRecruitNotice - ( errCode %d )",
                                nErrorCode);
            return;
        }
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        CServer* pServer = relayServer.GetServer(dwServerID);
        relayServer.GetLeagueManager().ResLeagueRecruitNotice(
            pServer, dwActorID, stNotice);
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
    ST_LEAGUE_RECORD stRecord{};

    xPacket >> stRecord;

    return CLogicThreadManager::Instance().DoJob(1, [stRecord]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeagueRecordUpdate(stRecord);
    });
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

    return CLogicThreadManager::Instance().DoJob(1, [psDelegateReq, dwServerID, dwReqUCID, nErrorCode]() {
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
        relayServer.GetLeagueManager().ReqApplyLeagueExp(stWealth);
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

bool CGameDBSocket::ReqLeagueInventoryMove(XPacket& xPacket) {
    std::uint32_t dwActorID = 0;
    PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME stMove{};

    xPacket.XParse >> dwActorID;
    xPacket >> stMove;

    return CLogicThreadManager::Instance().DoJob(1, [dwActorID, stMove]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeagueInventoryMove(dwActorID, stMove);
    });
}

bool CGameDBSocket::ReqLeagueInventoryInfo(XPacket& xPacket) {
    std::uint32_t dwActorID = 0;
    PS_REQ_LEAGUE_INVEN_INFO stReq{};

    xPacket.XParse >> dwActorID;
    xPacket >> stReq;

    return CLogicThreadManager::Instance().DoJob(1, [dwActorID, stReq]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().ReqLeagueInevntoryInfo(dwActorID, stReq);
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

bool CGameDBSocket::ResGMTLeagueInfo(XPacket& xPacket) {
    PS_GMT_LEAGUE_UPDATE_LIST stList{};

    xPacket >> stList;

    return CLogicThreadManager::Instance().DoJob(1, [stList]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().SendGMTLeagueInfo(stList);
    });
}

bool CGameDBSocket::ResLeagueWithdrawPenalty(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    std::int64_t biPenalty = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biPenalty;

    return CLogicThreadManager::Instance().DoJob(1, [dwUCID, biPenalty]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().LogOutLeagueMember(dwUCID, 0, biPenalty);
    });
}

bool CGameDBSocket::ResLeagueDeletePenalty(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    std::int64_t biPenalty = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> biPenalty;

    return CLogicThreadManager::Instance().DoJob(1, [dwUCID, biPenalty]() {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetLeagueManager().LogOutLeagueMember(dwUCID, 0, biPenalty);
    });
}
