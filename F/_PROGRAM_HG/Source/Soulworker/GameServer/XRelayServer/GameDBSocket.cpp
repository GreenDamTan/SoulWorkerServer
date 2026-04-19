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
