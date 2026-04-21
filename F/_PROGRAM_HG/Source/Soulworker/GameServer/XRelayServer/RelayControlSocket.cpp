#include "Soulworker/GameServer/XRelayServer/RelayControlSocket.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/IXObject.h"
#include "Soulworker/GameServer/XRelayServer/ForceManager.h"
#include "Soulworker/GameServer/XRelayServer/ForceMatching.h"
#include "Soulworker/GameServer/XRelayServer/FriendProcess.h"
#include "Soulworker/GameServer/XRelayServer/ForceProcess.h"
#include "Soulworker/GameServer/XRelayServer/PartyManager.h"
#include "Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/PartyProcess.h"
#include "Soulworker/GameServer/XRelayServer/LeagueProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerModeMazeProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XRelayServer/UserProcess.h"
#include "Soulworker/GameServer/XRelayServer/WorldModeProcess.h"

#include <chrono>
#include <cstdio>
#include <memory>
#include <thread>
#include <utility>

namespace {
thread_local std::unique_ptr<CServer> g_relayThreadServer;

CServer* GetRelayProcessServer() {
    if (!g_relayThreadServer) {
        g_relayThreadServer = std::make_unique<CServer>();
    }
    return g_relayThreadServer.get();
}
}

void XRelaySocket::SetMyInfo(const XOption* option) {
    if (!option) {
        m_myInfo = {};
        m_relayInfo = {};
        return;
    }

    m_myInfo.dwID = option->GetServerID();
    m_myInfo.nGroup = option->GetGroupID();
    m_myInfo.nType = option->GetType();
    m_myInfo.nChannel = option->GetChannel();
    m_myInfo.nState = 1;
    m_myInfo.nCurUser = 0;
    m_myInfo.nMaxUser = option->GetMaxConnect();
    m_myInfo.sThreadCount = option->GetLogicThread();
    m_myInfo.sPort = static_cast<std::uint16_t>(option->GetPort());
    m_myInfo.nMazeCount = 0;
    m_myInfo.nNPCCount = 0;
#ifdef _WIN32
    strcpy_s(m_myInfo.szPrivateIP, sizeof(m_myInfo.szPrivateIP), option->GetIP());
    strcpy_s(m_myInfo.szPublicIP, sizeof(m_myInfo.szPublicIP), option->GetPublicIP());
    strcpy_s(m_myInfo.szName, sizeof(m_myInfo.szName), option->GetName());
    sprintf_s(m_relayInfo.szName, "CONTROL");
#else
    std::strncpy(m_myInfo.szPrivateIP, option->GetIP(), sizeof(m_myInfo.szPrivateIP) - 1);
    std::strncpy(m_myInfo.szPublicIP, option->GetPublicIP(), sizeof(m_myInfo.szPublicIP) - 1);
    std::strncpy(m_myInfo.szName, option->GetName(), sizeof(m_myInfo.szName) - 1);
    std::snprintf(m_relayInfo.szName, sizeof(m_relayInfo.szName), "%s", "CONTROL");
#endif

    m_relayInfo.nState = 0;
    std::uint16_t relayPort = 0;
    if (option->GetServerPrivateIPAndPort(m_relayInfo.szName, m_relayInfo.szPrivateIP, relayPort)) {
        m_relayInfo.sPort = static_cast<std::int16_t>(relayPort);
    } else {
        m_relayInfo.sPort = 0;
    }
}

bool XRelaySocket::Init(E_POOL_ID poolId, const char* ip, std::uint16_t port) {
    m_myInfo = {};
    m_relayInfo = {};
    m_nSyncServerData = 0;
    return XIOCPClient::Init(poolId, ip, port);
}

bool XRelaySocket::Connect() {
    return XIOCPClient::Connect(m_relayInfo.szPrivateIP,
                                static_cast<std::uint16_t>(m_relayInfo.sPort));
}

void XRelaySocket::OnConnect() {
    m_nSyncServerData = 0;
    SendAddServer();
}

void XRelaySocket::OnDisConnect() {
    m_relayInfo.nState = 0;
    LogHelper::LogInfo("game.relay", "Relay Client DisConnect");
    m_eState = eConnectStateDisconnected;
}

void XRelaySocket::OnNotConnect() {
    LogHelper::LogInfo("game.relay", "NotConnect to Relay! ");
}

bool XRelaySocket::OnParse(XPacket& xPacket) {
    switch (xPacket.GetMainCmd()) {
    case eCMD_SERVER:
        return ServerProcess(xPacket);
    case 0xF3: {
        CUserProcess userProcess(GetRelayProcessServer());
        return userProcess.Parse(xPacket);
    }
    case 0xF4:
        return CPartyProcess(GetRelayProcessServer()).Parse(xPacket);
    case 0xF5:
        return CFriendProcess(GetRelayProcessServer()).Parse(xPacket);
    case 0xF6:
        return CLeagueProcess(GetRelayProcessServer()).Parse(xPacket);
    case 0xFA:
        return CForceProcess(GetRelayProcessServer(), &TXSingleton<XRelayServer>::Instance()->GetForceManager()).Parse(xPacket);
    case 0xFB:
        return CServerWorldModeProcess(GetRelayProcessServer()).Parse(xPacket);
    case 0xFD:
        return CServerModeMazeProcess(GetRelayProcessServer()).Parse(xPacket);
    default:
        return false;
    }
}

bool XRelaySocket::ServerProcess(XPacket& xPacket) {
    switch (xPacket.GetSubCmd()) {
    case eSUB_CMD_SERVER_UPDATE:
        return RecvServerUpdate(xPacket);
    case eSUB_CMD_SERVER_FROM_RELAY:
        RecvPacketFromRelay(xPacket);
        return true;
    case eSUB_CMD_SERVER_CHANGE_CHANNEL:
        RecvChangeChannelRes(xPacket);
        return true;
    case eSUB_CMD_SERVER_CHANNEL_UPDATE_ALL:
        return RecvUpdateChannelAll(xPacket);
    case eSUB_CMD_SERVER_CHANNEL_UPDATE:
        return RecvUpdateChannel(xPacket);
    default: {
        CServerProcess serverProcess(GetRelayProcessServer());
        if (serverProcess.Parse(xPacket)) {
            return true;
        }
        return ServerProcessEx(xPacket);
    }
    }
}

bool XRelaySocket::ServerProcessEx(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

void XRelaySocket::OnStartThread() {
    using namespace std::chrono;

    while (m_eState != eConnectStateShutdown) {
        const steady_clock::time_point begin = steady_clock::now();
        if (m_bInit) {
            GreenDamTan_PumpSocketRecv();
            XIOCPClient::Parsing();
        }
        const auto elapsed = duration_cast<milliseconds>(steady_clock::now() - begin);
        if (elapsed < milliseconds(15)) {
            std::this_thread::sleep_for(milliseconds(15) - elapsed);
        }
    }
}

bool XRelaySocket::RecvServerUpdate(XPacket& xPacket) {
    SS_UPDATE_SERVER_INFO updateInfo{};
    xPacket >> updateInfo;
    m_relayInfo.nState = static_cast<int>(updateInfo.nState);
    return true;
}

void XRelaySocket::RecvPacketFromRelay(XPacket& xPacket) {
    static_cast<void>(xPacket);
}

bool XRelaySocket::RecvChangeChannelRes(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvUpdateChannelAll(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvUpdateChannel(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

void XRelaySocket::SendAddServer() {
    m_myInfo.nState = 1;
    XSendPacket sendPacket(eCMD_SERVER, eSUB_CMD_SERVER_ADD);
    sendPacket << m_myInfo;
    XIOCPClient::Send(sendPacket);
}

void XRelaySocket::SendUpdateServerInfo(std::int16_t nState, int nUserCount) {
    if (static_cast<float>(nUserCount) >= static_cast<float>(m_myInfo.nMaxUser) * 0.95f) {
        nState = 3;
    }

    SS_UPDATE_SERVER_INFO updateInfo{};
    updateInfo.dwID = m_myInfo.dwID;
    updateInfo.nState = static_cast<std::uint16_t>(nState);
    m_myInfo.nCurUser = nUserCount;
    updateInfo.nCurUser = static_cast<std::uint16_t>(nUserCount);

    XSendPacket sendPacket(eCMD_SERVER, eSUB_CMD_SERVER_UPDATE);
    sendPacket << updateInfo;
    XIOCPClient::Send(sendPacket);
}

bool XRelaySocket::IsReady() const {
    return XIOCPClient::IsConnection();
}

void CRelayControlSocket::SetMyInfo(const XOption* option) {
    XRelaySocket::SetMyInfo(option);
}

bool CRelayControlSocket::ServerProcessEx(XPacket& xPacket) {
    LogHelper::LogDebug("game.relay",
                        "GreenDamTan_log RelayControlSocket.cpp::CRelayControlSocket::ServerProcessEx sub=%u",
                        static_cast<unsigned int>(xPacket.GetSubCmd()));
    switch (xPacket.GetSubCmd()) {
    case 'D':
        return ResCreateMatchingMaze(xPacket);
    case 'E':
        return SyncPartyMazeInfo(xPacket);
    case 'F':
        return SyncForceMazeInfo(xPacket);
    case 'J':
        return ResCreateMatchingModeMaze(xPacket);
    default:
        return false;
    }
}

bool CRelayControlSocket::DispatchLogicJob(std::int64_t instanceID, const std::function<void()>& job) {
    if (!job) {
        return false;
    }
    return CLogicThreadManager::Instance().DoJob(instanceID, job);
}

bool CRelayControlSocket::ResCreateMatchingMaze(XPacket& xPacket) {
    std::uint32_t matchingID = 0;
    ST_CREATE_MAZE createMaze{};
    PS_PARTY_INFO partyInfo{};
    PS_FORCE_INFO forceInfo{};

    xPacket.XParse >> matchingID;
    xPacket >> createMaze;
    xPacket >> partyInfo;
    xPacket >> forceInfo;

    return DispatchLogicJob(static_cast<std::int64_t>(matchingID),
                            [matchingID, createMaze, partyInfo, forceInfo]() mutable {
                                XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

                                if (createMaze.stPartyInfo.byGroupType == 1 &&
                                    createMaze.stPartyInfo.nID > 0) {
                                    // Party path
                                    relayServer.GetPartyManager().SetMaze(
                                        static_cast<std::uint32_t>(createMaze.stPartyInfo.nID),
                                        createMaze.uxMapID,
                                        createMaze.uxParentInstanceID);

                                    relayServer.GetPartyMatchingMgr().SendCreateMatchingMaze(
                                        matchingID,
                                        createMaze,
                                        partyInfo);

                                    // DB update 0x04/0x08
                                    IXObject* pObject = nullptr;
                                    XSendDBPacket xSendDBPacket(pObject, 4u, 8u);
                                    xSendDBPacket.XParse << createMaze.stPartyInfo.nID;
                                    xSendDBPacket.XParse << createMaze.uxMapID.nMapID;
                                    xSendDBPacket.XParse << 0;
                                    relayServer.SendDBGame(xSendDBPacket);
                                } else if (createMaze.stPartyInfo.byGroupType == 2 &&
                                           createMaze.stPartyInfo.nID > 0) {
                                    // Force path
                                    relayServer.GetForceManager().SetMaze(
                                        static_cast<std::uint32_t>(createMaze.stPartyInfo.nID),
                                        createMaze.uxMapID,
                                        createMaze.uxParentInstanceID);

                                    relayServer.GetForceMatchingMgr().SendCreateMatchingMaze(
                                        matchingID,
                                        createMaze,
                                        forceInfo);

                                    // DB update 0x04/0x08
                                    IXObject* pObject = nullptr;
                                    XSendDBPacket xSendDBPacket(pObject, 4u, 8u);
                                    xSendDBPacket.XParse << createMaze.stPartyInfo.nID;
                                    xSendDBPacket.XParse << createMaze.uxMapID.nMapID;
                                    xSendDBPacket.XParse << 0;
                                    relayServer.SendDBGame(xSendDBPacket);
                                }

                                LogHelper::LogDebug(
                                    "game.relay",
                                    "GreenDamTan_log RelayControlSocket.cpp::CRelayControlSocket::ResCreateMatchingMaze matchingID=%u createType=%d result=%d partyType=%u forceID=%u",
                                    static_cast<unsigned int>(matchingID),
                                    createMaze.nCreateType,
                                    createMaze.nResult,
                                    static_cast<unsigned int>(partyInfo.byPartyType),
                                    static_cast<unsigned int>(forceInfo.dwForceID));
                            });
}

bool CRelayControlSocket::SyncPartyMazeInfo(XPacket& xPacket) {
    std::uint32_t partyID = 0;
    UXMapID mapID{};
    UXMapID beforeMapID{};

    xPacket.XParse >> partyID;
    xPacket.XParse >> mapID.nMapID;
    xPacket.XParse >> beforeMapID.nMapID;

    return DispatchLogicJob(static_cast<std::int64_t>(partyID),
                            [partyID, mapID, beforeMapID]() {
                                LogHelper::LogDebug(
                                    "game.relay",
                                    "GreenDamTan_log RelayControlSocket.cpp::CRelayControlSocket::SyncPartyMazeInfo partyID=%u mapID=%llu beforeMapID=%llu",
                                    static_cast<unsigned int>(partyID),
                                    static_cast<unsigned long long>(mapID.nMapID),
                                    static_cast<unsigned long long>(beforeMapID.nMapID));
                            });
}

bool CRelayControlSocket::SyncForceMazeInfo(XPacket& xPacket) {
    std::uint32_t forceID = 0;
    UXMapID mapID{};
    UXMapID beforeMapID{};

    xPacket.XParse >> forceID;
    xPacket.XParse >> mapID.nMapID;
    xPacket.XParse >> beforeMapID.nMapID;

    return DispatchLogicJob(static_cast<std::int64_t>(forceID),
                            [forceID, mapID, beforeMapID]() {
                                LogHelper::LogDebug(
                                    "game.relay",
                                    "GreenDamTan_log RelayControlSocket.cpp::CRelayControlSocket::SyncForceMazeInfo forceID=%u mapID=%llu beforeMapID=%llu",
                                    static_cast<unsigned int>(forceID),
                                    static_cast<unsigned long long>(mapID.nMapID),
                                    static_cast<unsigned long long>(beforeMapID.nMapID));
                            });
}

bool CRelayControlSocket::ResCreateMatchingModeMaze(XPacket& xPacket) {
    ST_CREATE_MODE_MAZE createModeMaze{};
    xPacket >> createModeMaze;

    return DispatchLogicJob(0, [createModeMaze]() mutable {
        CModeMazeMatchingMgr::Instance().SendCreateMatchingModeMaze(createModeMaze);
        LogHelper::LogDebug(
            "game.relay",
            "GreenDamTan_log RelayControlSocket.cpp::CRelayControlSocket::ResCreateMatchingModeMaze matchingID=%u eventRoomID=%u result=%d memberCount=%zu",
            static_cast<unsigned int>(createModeMaze.dwMatchingID),
            static_cast<unsigned int>(createModeMaze.dwEventRoomID),
            createModeMaze.nResult,
            createModeMaze.vecEnterMember.size());
    });
}
