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
    case 5:
        return DBFriendParse(xPacket);
    case 8:
        return DBForceParse(xPacket);
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
