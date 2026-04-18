#include "Soulworker/GameServer/XRelayServer/RelayServer.h"

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#else
#include <unistd.h>
#endif

namespace {
#ifdef _WIN32
BOOL WINAPI ConsoleCtrlThunk(DWORD opCode) {
    return XRelayServer::ConsolCtrlHandler(opCode) ? TRUE : FALSE;
}
#else
void ConsoleCtrlThunk(int signalCode) {
    XRelayServer::ConsolCtrlHandler(static_cast<unsigned int>(signalCode));
}
#endif

constexpr std::uint32_t E_SERVER_CACHING_LOAD_NONE = 0u;
constexpr std::uint32_t E_SERVER_CACHING_LOAD_PARTY = 1u;
constexpr std::uint32_t E_SERVER_CACHING_LOAD_LEAGUE = 2u;
constexpr std::uint32_t E_SERVER_CACHING_LOAD_FRINED = 4u;
constexpr std::uint32_t E_SERVER_CACHING_LOAD_USER = 8u;

std::int64_t GreenDamTan_GetCurDateSec() {
    return static_cast<std::int64_t>(std::time(nullptr));
}

bool GreenDamTan_DBGetText(XDBStmt& dbStmt,
                           XDBBinder& xDBBinder,
                           char* buffer,
                           std::size_t bufferSize,
                           std::int64_t* outResult = nullptr) {
    if (!buffer || bufferSize == 0) {
        return false;
    }

    buffer[0] = '\0';
    std::int64_t strLenOrInd = 0;
    const std::uint16_t outParam = xDBBinder.m_sOutParam++;
    const std::int64_t result =
        dbStmt.SQLGetData(outParam, 1, buffer, static_cast<std::int64_t>(bufferSize), &strLenOrInd);
    if (outResult) {
        *outResult = result;
    }
    if ((result & ~1LL) != 0) {
        return false;
    }

    if (strLenOrInd < 0) {
        buffer[0] = '\0';
        return true;
    }

    buffer[bufferSize - 1] = '\0';
    return true;
}

template <typename T>
bool GreenDamTan_DBGetUnsigned(XDBStmt& dbStmt,
                               XDBBinder& xDBBinder,
                               T* value,
                               std::int64_t* outResult = nullptr) {
    if (!value) {
        return false;
    }

    char buffer[128] = {};
    if (!GreenDamTan_DBGetText(dbStmt, xDBBinder, buffer, sizeof(buffer), outResult)) {
        return false;
    }

    T parsedValue{};
    if (buffer[0]) {
        parsedValue = static_cast<T>(std::strtoull(buffer, nullptr, 10));
    }

    std::memcpy(value, &parsedValue, sizeof(parsedValue));
    return true;
}
}

void CFriendRecruitManager::UpdateRecruit(std::uint32_t dwActorID, int state) {
    CFAutoSlimWriteLock autolock(&m_rwLock);
    RecruitInfo& recruit = m_mapRecruit[dwActorID];
    recruit.bLogin = (state != 0);
    if (state != 0) {
        recruit.bRecruit = true;
        recruit.tExpire = GreenDamTan_GetCurDateSec() + 1800;
    } else if (!recruit.bRecruit) {
        m_mapRecruit.erase(dwActorID);
    }
}

bool CFriendRecruitManager::IsRecruitList(std::uint32_t dwActorID) const {
    CFAutoSlimReadLock autolock(&m_rwLock);
    const auto it = m_mapRecruit.find(dwActorID);
    return it != m_mapRecruit.end() && it->second.bRecruit;
}

void CFriendRecruitManager::DeleteRecruit(std::uint32_t dwActorID) {
    CFAutoSlimWriteLock autolock(&m_rwLock);
    const auto it = m_mapRecruit.find(dwActorID);
    if (it == m_mapRecruit.end()) {
        return;
    }

    it->second.bRecruit = false;
    it->second.tExpire = 0;
    if (!it->second.bLogin) {
        m_mapRecruit.erase(it);
    }
}

void CFriendRecruitManager::OnUpdate() {
    const std::int64_t currentTime = GreenDamTan_GetCurDateSec();
    if (m_tUpdate != 0 && currentTime < m_tUpdate + 1) {
        return;
    }
    m_tUpdate = currentTime;

    std::vector<std::uint32_t> expiredRecruits;
    {
        CFAutoSlimReadLock autolock(&m_rwLock);
        for (const auto& [ucid, recruit] : m_mapRecruit) {
            if (recruit.bRecruit && recruit.tExpire != 0 && recruit.tExpire <= currentTime) {
                expiredRecruits.push_back(ucid);
            }
        }
    }

    for (std::uint32_t ucid : expiredRecruits) {
        TXSingleton<XRelayServer>::Instance()->SendRecruitDelete(ucid);
    }
}

bool CRelayPartyMatchingConfig::Init(const char* commonDNS) {
    m_nMatchingWaitMs = 0;

    if (!commonDNS || !*commonDNS) {
        return true;
    }

    if (!m_xCommonDBMgr.Init(reinterpret_cast<unsigned char*>(const_cast<char*>(commonDNS)), 1)) {
        LogHelper::LogError("game.relay",
                            "GreenDamTan_log RelayServer.cpp::CRelayPartyMatchingConfig::Init db-manager-init-fail");
        return false;
    }

    XDBConnect* dbConnect = m_xCommonDBMgr.GetDBConnect();
    if (!dbConnect) {
        LogHelper::LogError("game.relay",
                            "GreenDamTan_log RelayServer.cpp::CRelayPartyMatchingConfig::Init no-db-connect");
        return false;
    }

    auto collectConnect = [&]() {
        m_xDBStmt.Clear();
        m_xCommonDBMgr.CollectDBConnect(dbConnect);
    };

    void** hdbc = dbConnect->GetHDBC();
    if (!hdbc || !*hdbc) {
        collectConnect();
        return false;
    }

    if (m_xDBStmt.Init(dbConnect, nullptr, nullptr) != 0) {
        collectConnect();
        return false;
    }

    XDBBinder xDBBinder(&m_xDBStmt);
    std::int64_t result = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
        "select [Value] from tb_Common where [Define_ID] = 30002")));
    if ((result & ~1LL) != 0) {
        collectConnect();
        return false;
    }

    result = xDBBinder.Fetch();
    if ((result & ~1LL) == 0) {
        char valueText[128] = {};
        if (!GreenDamTan_DBGetText(m_xDBStmt, xDBBinder, valueText, sizeof(valueText), &result)) {
            collectConnect();
            return false;
        }
        m_nMatchingWaitMs = static_cast<int>(std::strtof(valueText, nullptr) * 1000.0f);
    }

    dbConnect->SetEndTran(0);
    xDBBinder.Close();
    collectConnect();
    return true;
}

bool CRelayMazeOpenControl::Init(const char* commonDNS) {
    m_mapMazeOpenControl.clear();

    if (!commonDNS || !*commonDNS) {
        return true;
    }

    if (!m_xCommonDBMgr.Init(reinterpret_cast<unsigned char*>(const_cast<char*>(commonDNS)), 1)) {
        LogHelper::LogError("game.relay",
                            "GreenDamTan_log RelayServer.cpp::CRelayMazeOpenControl::Init db-manager-init-fail");
        return false;
    }

    return LoadRows();
}

bool CRelayMazeOpenControl::LoadRows() {
    XDBConnect* dbConnect = m_xCommonDBMgr.GetDBConnect();
    if (!dbConnect) {
        LogHelper::LogError("game.relay",
                            "GreenDamTan_log RelayServer.cpp::CRelayMazeOpenControl::LoadRows no-db-connect");
        return false;
    }

    auto collectConnect = [&]() {
        m_xDBStmt.Clear();
        m_xCommonDBMgr.CollectDBConnect(dbConnect);
    };

    void** hdbc = dbConnect->GetHDBC();
    if (!hdbc || !*hdbc) {
        collectConnect();
        return false;
    }

    if (m_xDBStmt.Init(dbConnect, nullptr, nullptr) != 0) {
        collectConnect();
        return false;
    }

    XDBBinder xDBBinder(&m_xDBStmt);
    std::int64_t result = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
        "select [ID], [Open_Time], [Remain_Time] from tb_Maze_OpenControl")));
    if ((result & ~1LL) != 0) {
        collectConnect();
        return false;
    }

    m_mapMazeOpenControl.clear();
    result = xDBBinder.Fetch();
    while ((result & ~1LL) == 0) {
        TB_MAZE_OPENCONTROL row{};
        if (!GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.ID, &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.Open_Time, &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.Remain_Time, &result)) {
            collectConnect();
            return false;
        }
        m_mapMazeOpenControl[row.ID] = row;
        result = xDBBinder.Fetch();
    }

    dbConnect->SetEndTran(0);
    xDBBinder.Close();
    collectConnect();
    return true;
}

bool CRelayMazeOpenControl::CheckMazeOpenTime(std::uint32_t dwMazeID) {
    if (!m_bCheckMazeOpenTime) {
        return true;
    }

    std::time_t now = std::time(nullptr);
    std::tm localTime{};
#ifdef _WIN32
    if (localtime_s(&localTime, &now) != 0) {
        return true;
    }
#else
    if (localtime_r(&now, &localTime) == nullptr) {
        return true;
    }
#endif

    const std::uint32_t weekDay = static_cast<std::uint32_t>(localTime.tm_wday + 1);
    const std::uint32_t startID = 100u * (100u * dwMazeID + weekDay) + 1u;
    const std::uint32_t endID = 100u * (100u * dwMazeID + weekDay) + 20u;

    bool sawRow = false;
    for (std::uint32_t rowID = startID; rowID <= endID; ++rowID) {
        const auto it = m_mapMazeOpenControl.find(rowID);
        if (it == m_mapMazeOpenControl.end()) {
            continue;
        }

        sawRow = true;
        const TB_MAZE_OPENCONTROL& row = it->second;
        const int openHour = static_cast<int>(row.Open_Time / 100u);
        const int openMinute = static_cast<int>(row.Open_Time % 100u);

        std::tm openTime = localTime;
        openTime.tm_sec = 0;
        openTime.tm_min = openMinute;
        openTime.tm_hour = openHour;
        openTime.tm_isdst = -1;

        const std::time_t openTick = std::mktime(&openTime);
        if (openTick == static_cast<std::time_t>(-1)) {
            continue;
        }

        const std::time_t closeTick = openTick + static_cast<std::time_t>(60u * row.Remain_Time);
        if (openTick <= now && now <= closeTick) {
            return true;
        }
    }

    return !sawRow;
}

XRelayServer::XRelayServer() = default;

void XRelayServer::AddServerInfo(CServer* pServer) {
    if (!pServer) {
        return;
    }

    if (pServer->GetServerType() == 2) {
        AddGameServerInfo(pServer);
    }
}

void XRelayServer::AddGameServerInfo(CServer* pServer) {
    if (!pServer) {
        return;
    }

    CFAutoSlimWriteLock autolock(&m_rwServerLock);
    m_mapGameServer[pServer->GetServerID()] = pServer;
    LogHelper::LogInfo("game.relay",
                       "GreenDamTan_log RelayServer.cpp::XRelayServer::AddGameServerInfo serverID=%u",
                       static_cast<unsigned int>(pServer->GetServerID()));
    UnSetCachingLoad(E_SERVER_CACHING_LOAD_USER);
}

bool XRelayServer::AddUser(CServer* pServer,
                           STCharInfo& stInfo,
                           UXMapID uxMapID,
                           ST_GAME_OPTION& stGameOption) {
    if (!pServer) {
        return false;
    }

    const std::uint32_t actorID = stInfo.uxActorID.dwActorID;
    std::shared_ptr<CUserObject> userInfo;

    {
        CFAutoSlimWriteLock autolock(&m_rwLock);

        auto& userSlot = m_mapUserInfos[actorID];
        if (!userSlot) {
            userSlot = std::make_shared<CUserObject>(pServer, stInfo, uxMapID);
        }
        userSlot->UpdateFromSync(pServer, pServer->GetServerID(), stInfo, uxMapID);
        userSlot->SetGameOption(stGameOption);
        userInfo = userSlot;

        auto& partySlot = m_mapUserPartyInfos[actorID];
        if (!partySlot) {
            partySlot = std::make_shared<CUserPartyInfo>(actorID);
        }
        partySlot->SetActorID(actorID);
        partySlot->SetServerID(pServer->GetServerID());
    }

    AddPartyUser(pServer, actorID);
    if (userInfo) {
        userInfo->SendFriendServerLoad();
        m_RecruitManager.UpdateRecruit(actorID, 1);
        if (!AddLeagueUser(pServer, actorID, stInfo.stLeagueInfo.nLeagueID)) {
            userInfo->SetLeagueID(0);
        }
        m_RecommandManager.DeleteUser(userInfo);
        m_RecommandManager.AddUser(userInfo);

        XSendDBPacket sendPacket(static_cast<IXObject*>(pServer), 5u, 1u);
        sendPacket.XParse << static_cast<unsigned int>(stInfo.dwUAID);
        sendPacket.XParse << static_cast<unsigned int>(stInfo.uxActorID.dwActorID);
        SendDBGame(sendPacket);
    }

    LogHelper::LogDebug("game.relay",
                        "GreenDamTan_log RelayServer.cpp::XRelayServer::AddUser actorID=%u uaid=%u serverID=%u",
                        static_cast<unsigned int>(actorID),
                        static_cast<unsigned int>(stInfo.dwUAID),
                        static_cast<unsigned int>(pServer->GetServerID()));
    return true;
}

bool XRelayServer::AddPartyUser(CServer* pServer, std::uint32_t dwActorID) {
    if (!pServer) {
        return false;
    }

    return CLogicThreadManager::Instance().DoJob(0, [this, pServer, dwActorID]() {
        CFAutoSlimWriteLock autolock(&m_rwLock);
        auto& partySlot = m_mapUserPartyInfos[dwActorID];
        if (!partySlot) {
            partySlot = std::make_shared<CUserPartyInfo>(dwActorID);
        }
        partySlot->SetActorID(dwActorID);
        partySlot->SetServerID(pServer->GetServerID());
    });
}

bool XRelayServer::AddLeagueUser(CServer* pServer, std::uint32_t dwUCID, int nLeagueID) {
    static_cast<void>(pServer);
    if (nLeagueID == 0) {
        return false;
    }

    const std::shared_ptr<CUserObject> userInfo = GetUser(dwUCID);
    if (!userInfo) {
        return false;
    }

    userInfo->SetLeagueID(nLeagueID);
    LogHelper::LogDebug("game.relay",
                        "GreenDamTan_log RelayServer.cpp::XRelayServer::AddLeagueUser actorID=%u leagueID=%d",
                        static_cast<unsigned int>(dwUCID),
                        nLeagueID);
    return true;
}

std::shared_ptr<CUserObject> XRelayServer::GetUser(std::uint32_t dwActorID) {
    CFAutoSlimReadLock autolock(&m_rwLock);
    const auto it = m_mapUserInfos.find(dwActorID);
    return it == m_mapUserInfos.end() ? std::shared_ptr<CUserObject>{} : it->second;
}

std::shared_ptr<CUserObject> XRelayServer::GetUser(const wchar_t* pName) {
    if (!pName || !*pName) {
        return {};
    }

    CFAutoSlimReadLock autolock(&m_rwLock);
    for (const auto& [actorID, userInfo] : m_mapUserInfos) {
        static_cast<void>(actorID);
        if (userInfo && userInfo->GetName() == pName) {
            return userInfo;
        }
    }
    return {};
}

std::shared_ptr<CUserPartyInfo> XRelayServer::GetPartyUser(std::uint32_t dwActorID) {
    CFAutoSlimReadLock autolock(&m_rwLock);
    const auto it = m_mapUserPartyInfos.find(dwActorID);
    return it == m_mapUserPartyInfos.end() ? std::shared_ptr<CUserPartyInfo>{} : it->second;
}

bool XRelayServer::IsFriendBlock(std::uint32_t dwUCID, std::uint32_t dwCheckUCID) {
    const std::shared_ptr<CUserObject> userInfo = GetUser(dwUCID);
    if (!userInfo) {
        return false;
    }
    return userInfo->IsBlockList(dwCheckUCID);
}

bool XRelayServer::IsFriendBlock(std::uint32_t dwUCID, const wchar_t* strTargetName) {
    const std::shared_ptr<CUserObject> userInfo = GetUser(dwUCID);
    if (!userInfo || !strTargetName) {
        return false;
    }
    return userInfo->IsBlockList(strTargetName);
}

void XRelayServer::SendPacketAll(XSendPacket& xSendPacket) {
    for (XClient* client = m_xClientPool.GetHead(); client; client = m_xClientPool.GetNext()) {
        if (!client || client->GetSessionID() == 0) {
            continue;
        }
        client->SendEx(xSendPacket);
    }
}

void XRelayServer::SendPacket(std::uint32_t dwServerID, XSendPacket& xSendPacket) {
    if (CServer* server = GetServer(dwServerID)) {
        server->SendEx(xSendPacket);
    }
}

bool XRelayServer::PrepareDeleteRecruit(const PS_RECRUIT_DELETE& stDelete) {
    const std::shared_ptr<CUserObject> userInfo = GetUser(stDelete.dwUCID);
    if (!userInfo) {
        KickOutUser(stDelete.dwUCID, 0x0Cu);
        LogHelper::LogError("game.relay", "<KICKOUT> XRelayServer::PrepareDeleteRecruit");
        return true;
    }

    if (!m_RecruitManager.IsRecruitList(stDelete.dwUCID)) {
        PS_RECRUIT_STATE recruitState{};
        XSendPacket packet(0xF5u, 0x18u);
        packet.XParse << stDelete.dwUCID;
        packet << recruitState;
        userInfo->SendPacket(packet);
        return false;
    }

    XSendDBPacket sendPacket(nullptr, 5u, 0x10u);
    sendPacket << stDelete;
    SendDBGame(sendPacket);
    return true;
}

void XRelayServer::SendRecruitDelete(std::uint32_t dwUCID) {
    PS_RECRUIT_DELETE deleteInfo{};
    deleteInfo.dwUCID = dwUCID;

    XSendDBPacket sendPacket(nullptr, 5u, 0x10u);
    sendPacket << deleteInfo;
    SendDBGame(sendPacket);
}

bool XRelayServer::DeleteRecruit(const PS_RES_RECRUIT_DELETE& stDelete) {
    const std::shared_ptr<CUserObject> userInfo = GetUser(stDelete.dwUCID);
    if (stDelete.nResult == 0) {
        m_RecruitManager.DeleteRecruit(stDelete.dwUCID);
        if (userInfo) {
            PS_RECRUIT_STATE recruitState{};
            XSendPacket packet(0xF5u, 0x18u);
            packet.XParse << stDelete.dwUCID;
            packet << recruitState;
            userInfo->SendPacket(packet);
        }
    }
    return true;
}

void XRelayServer::KickOutUser(std::uint32_t dwUCID, std::uint8_t byType) {
    PS_KICK_USER_INFO_UCID psKick{};
    psKick.dwUCID = dwUCID;
    psKick.byKickType = byType;

    XSendPacket xSendPacket(0xF3u, 0x07u);
    xSendPacket << psKick;
    SendPacketAll(xSendPacket);
    LogHelper::LogInfo("game.system", "<KICKOUT> User[%u], Type[%u]", static_cast<unsigned int>(dwUCID), static_cast<unsigned int>(byType));
}

bool XRelayServer::SendFriendList(std::uint32_t dwActorID) {
    const std::shared_ptr<CUserObject> userInfo = GetUser(dwActorID);
    if (!userInfo) {
        KickOutUser(dwActorID, 0x0Cu);
        LogHelper::LogError("game.relay",
                            "<KICKOUT> XRelayServer::SendFriendList (UCID : %d)",
                            static_cast<int>(dwActorID));
        return false;
    }

    if (userInfo->GetLoadFriendList()) {
        PS_FRIEND_LIST stFriendList{};
        userInfo->GetFriendList(stFriendList, 0);

        XSendPacket xSendPacket(0xF5u, 0x01u);
        const std::shared_ptr<CUserPartyInfo> partyInfo = GetPartyUser(dwActorID);
        xSendPacket.XParse << (partyInfo ? partyInfo->GetMatchingID() : 0u);
        xSendPacket << stFriendList;
        userInfo->SendPacket(xSendPacket);
        userInfo->SetSyncFriendList(false);

        LogHelper::LogDebug("game.relay",
                            "<%d FRIEND_LIST> Send List ( Count : %d ) ",
                            static_cast<int>(dwActorID),
                            static_cast<int>(stFriendList.vecFriends.size()));
    } else {
        userInfo->SetSyncFriendList(true);
    }

    return true;
}

bool XRelayServer::SendBlockList(std::uint32_t dwActorID) {
    const std::shared_ptr<CUserObject> userInfo = GetUser(dwActorID);
    if (!userInfo) {
        KickOutUser(dwActorID, 0x0Cu);
        LogHelper::LogError("game.relay", "<KICKOUT> XRelayServer::SendBlockList");
        return false;
    }

    if (userInfo->GetLoadBlockList()) {
        PS_BLOCKLIST_INFO stBlockList{};
        userInfo->GetBlcokList(stBlockList);

        XSendPacket xSendPacket(0xF5u, 0x02u);
        const std::shared_ptr<CUserPartyInfo> partyInfo = GetPartyUser(dwActorID);
        xSendPacket.XParse << (partyInfo ? partyInfo->GetMatchingID() : 0u);
        xSendPacket << stBlockList;
        userInfo->SendPacket(xSendPacket);
        userInfo->SetSyncBlockList(false);

        LogHelper::LogDebug("game.relay",
                            "<%d BLOCK_LIST> Send List ( Count : %d ) ",
                            static_cast<int>(dwActorID),
                            static_cast<int>(stBlockList.vecBlockList.size()));
    } else {
        userInfo->SetSyncBlockList(true);
    }

    return true;
}

void XRelayServer::SetUsersInfo(CServer* pServer, const PS_USERS_INFO* pUsersInfo) {
    if (!pServer || !pUsersInfo) {
        return;
    }

    for (const PS_USER_INFO_FOR_RELAY& userInfo : pUsersInfo->vecUserInfo) {
        STCharInfo stInfo = userInfo.stCharInfo;
        UXMapID uxMapID = userInfo.uxMapID;
        ST_GAME_OPTION stGameOption = userInfo.stGameOption;
        AddUser(pServer, stInfo, uxMapID, stGameOption);
    }

    if (pUsersInfo->bFinish) {
        LogHelper::LogInfo("game.system",
                           "<SYNC> Users Info Finish : %u",
                           static_cast<unsigned int>(m_mapUserInfos.size()));
        pServer->RecvUserInfo();
        pServer->SetSyncLoad(1u);
        pServer->SetSyncLoad(2u);
    }

    LogHelper::LogDebug("game.relay",
                        "<USERS> Sync Users Member : %zu",
                        pUsersInfo->vecUserInfo.size());
}

void XRelayServer::UpdateUserMap(CServer* pServer, const PS_UPDATE_USER_MAP_INFO& updateInfo) {
    if (!pServer) {
        return;
    }

    std::shared_ptr<CUserObject> userInfo;
    UXMapID previousMap{};
    const std::uint32_t actorID = updateInfo.dwActorID;
    bool needsPartyRefresh = false;
    {
        CFAutoSlimWriteLock autolock(&m_rwLock);
        const auto it = m_mapUserInfos.find(actorID);
        if (it == m_mapUserInfos.end() || !it->second) {
            LogHelper::LogError("game.relay",
                                "<Find Fail> XRelayServer::UpdateUserMap [%u]",
                                static_cast<unsigned int>(actorID));
            return;
        }
        userInfo = it->second;
        previousMap = userInfo->GetMapIns();
        if (userInfo->GetServerID() != pServer->GetServerID()) {
            userInfo->InitRecruitListTime();
        }
        userInfo->SetServer(pServer, pServer->GetServerID());
        userInfo->SetMapIns(updateInfo.uxMapID);
        userInfo->ChangeMap(updateInfo.uxMapID);
        needsPartyRefresh = previousMap.parts.mapID != updateInfo.uxMapID.parts.mapID;
    }

    m_LeagueManger.UpdateMemberMapInfo(actorID,
                                      static_cast<std::uint16_t>(updateInfo.uxMapID.parts.mapID),
                                      static_cast<std::uint8_t>(updateInfo.uxMapID.parts.channel));

    ST_PARTY_INFO stPartyInfo = updateInfo.stPartyInfo;
    UXMapID uxActorMapID = updateInfo.uxMapID;
    CLogicThreadManager::Instance().DoJob(0, [this, pServer, actorID, stPartyInfo, uxActorMapID, needsPartyRefresh]() {
        static_cast<void>(stPartyInfo);
        static_cast<void>(uxActorMapID);
        if (!needsPartyRefresh || !pServer) {
            return;
        }

        CFAutoSlimWriteLock autolock(&m_rwLock);
        auto& partySlot = m_mapUserPartyInfos[actorID];
        if (!partySlot) {
            partySlot = std::make_shared<CUserPartyInfo>(actorID);
        }
        partySlot->SetActorID(actorID);
        partySlot->SetServerID(pServer->GetServerID());
    });

    LogHelper::LogDebug("game.relay",
                        "<UpdateUserMap> User : %u / Map : %u / Channel %u",
                        static_cast<unsigned int>(actorID),
                        static_cast<unsigned int>(updateInfo.uxMapID.parts.mapID),
                        static_cast<unsigned int>(static_cast<std::uint8_t>(updateInfo.uxMapID.parts.channel)));
}

void XRelayServer::RemoveUser(std::uint32_t dwActorID, int nAccountState, bool bKickAlreadyLogin) {
    std::shared_ptr<CUserObject> userInfo;
    {
        CFAutoSlimWriteLock autolock(&m_rwLock);
        const auto it = m_mapUserInfos.find(dwActorID);
        if (it == m_mapUserInfos.end() || !it->second) {
            LogHelper::LogError("game.contents",
                                "<Find Fail> XRelayServer::RemoveUser [%u]",
                                static_cast<unsigned int>(dwActorID));
            LogHelper::LogDebug("game.relay",
                                "<REMOVE_USER> Failed < UCID : %u > ",
                                static_cast<unsigned int>(dwActorID));
            return;
        }

        userInfo = it->second;
        userInfo->Logout();
        m_mapUserInfos.erase(it);
    }

    std::int16_t lastServerID = 0;
    if (nAccountState == 2 || bKickAlreadyLogin) {
        lastServerID = static_cast<std::int16_t>(m_xOption.GetServerID());
    }

    XSendDBPacket sendPacket(nullptr, 2u, 2u);
    sendPacket.XParse << static_cast<unsigned int>(userInfo->GetUAID());
    sendPacket.XParse << lastServerID;
    sendPacket.XParse << nAccountState;
    sendPacket.XParse << static_cast<unsigned int>(userInfo->GetIP());
    SendDBAccount(sendPacket);

    LogHelper::LogDebug("game.relay",
                        "<REMOVE_USER> < UID : %u / State : %d > ",
                        static_cast<unsigned int>(dwActorID),
                        nAccountState);
    RemovePartyUser(dwActorID, userInfo->GetUAID());
}

void XRelayServer::RemovePartyUser(std::uint32_t dwActorID, std::uint32_t dwUAID) {
    const std::shared_ptr<CUserPartyInfo> partyInfo = GetPartyUser(dwActorID);
    if (!partyInfo) {
        return;
    }

    if (partyInfo->GetMatchingState() == 2) {
        m_ForceMatchingMgr.MatchingRemoveUser(partyInfo->GetMatchingID(), dwActorID);
    } else if (partyInfo->GetMatchingState() == 3) {
        CModeMazeMatchingMgr::Instance().MatchingRemoveUser(dwActorID, dwUAID);
    }

    m_RecruitManager.UpdateRecruit(dwActorID, 0);
    partyInfo->Logout();
    CFAutoSlimWriteLock autolock(&m_rwLock);
    m_mapUserPartyInfos.erase(dwActorID);
}

void XRelayServer::UpdateUserLevelUp(std::uint32_t dwActorID, std::uint8_t byLevel) {
    const std::shared_ptr<CUserObject> userInfo = GetUser(dwActorID);
    if (!userInfo) {
        LogHelper::LogError("game.contents",
                            "<Find Fail> XRelayServer::UpdateUserLevelUp [%u]",
                            static_cast<unsigned int>(dwActorID));
        return;
    }

    userInfo->SetLevel(byLevel);
    LogHelper::LogDebug("game.relay",
                        "<LEVELUP> UCID : %u - Level %u ",
                        static_cast<unsigned int>(dwActorID),
                        static_cast<unsigned int>(byLevel));
}

void XRelayServer::UpdateUserAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken) {
    const std::shared_ptr<CUserObject> userInfo = GetUser(dwActorID);
    if (!userInfo) {
        LogHelper::LogError("game.relay",
                            "<Find Fail> XRelayServer::UpdateUserAwaken [%u]",
                            static_cast<unsigned int>(dwActorID));
        return;
    }

    userInfo->SetAwaken(byAwaken);
    LogHelper::LogDebug("game.relay",
                        "<AWAKEN> UCID : %u - Awaken %u ",
                        static_cast<unsigned int>(dwActorID),
                        static_cast<unsigned int>(byAwaken));
}

void XRelayServer::UpdateUserProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwProfilePhotoID) {
    const std::shared_ptr<CUserObject> userInfo = GetUser(dwActorID);
    if (!userInfo) {
        LogHelper::LogError("game.relay",
                            "<Find Fail> XRelayServer::UpdateUserProfilePhoto [%u]",
                            static_cast<unsigned int>(dwActorID));
        return;
    }

    userInfo->SetProfilePhoto(dwProfilePhotoID);
    LogHelper::LogDebug("game.relay",
                        "<ProfilePhoto> UCID : %u - ProfilePhoto %u ",
                        static_cast<unsigned int>(dwActorID),
                        static_cast<unsigned int>(dwProfilePhotoID));
}

void XRelayServer::SendChatNotice(const PS_CHAT_NOTICE& stChatNotice) {
    XSendPacket sendPacket(0xF3u, 0x11u);
    sendPacket << stChatNotice;

    for (XClient* client = m_xClientPool.GetHead(); client; client = m_xClientPool.GetNext()) {
        if (!client || client->GetSessionID() == 0) {
            continue;
        }
        client->SendEx(sendPacket);
    }
}

void XRelayServer::SendChatWhisper(std::uint32_t dwActorID,
                                   const PS_CHAT_WHISPER& stChatWhisper,
                                   const PS_CHAT_ITEM_LINK_FOR_SERVER& psItemLinkInfo) {
    const std::shared_ptr<CUserObject> targetUser = GetUser(stChatWhisper.strReciver);
    const std::shared_ptr<CUserObject> senderUser = GetUser(dwActorID);

    if (!senderUser) {
        LogHelper::LogError("game.relay",
                            "<Find Fail> XRelayServer::SendChatWhisper [%u]",
                            static_cast<unsigned int>(dwActorID));
        return;
    }

    auto sendToSender = [&](std::uint32_t matchingID, const PS_CHAT_WHISPER& whisper, std::uint8_t byResult) {
        XSendPacket xPacket(0xF3u, 0x10u);
        xPacket.XParse << matchingID;
        xPacket << whisper;
        xPacket.XParse << byResult;
        xPacket << psItemLinkInfo;
        senderUser->SendPacket(xPacket);
    };

    if (!targetUser) {
        PS_CHAT_WHISPER senderReply = stChatWhisper;
        sendToSender(dwActorID, senderReply, 1);
        LogHelper::LogError("game.relay",
                            "<Find Fail> XRelayServer::SendChatWhisper [%u]",
                            static_cast<unsigned int>(dwActorID));
        return;
    }

    PS_CHAT_WHISPER senderReply = stChatWhisper;
    if (targetUser->CheckGameOption(eOption_WhisperMsg, eGAME_OPTION_ALLOW_PARTIAL)) {
        if (targetUser->GetFriendUCID(stChatWhisper.strSender) == 0) {
            senderReply.nResult = 59201;
        }
    } else if (targetUser->CheckGameOption(eOption_WhisperMsg, eGAME_OPTION_REFUSE_ALL)) {
        senderReply.nResult = 59201;
    }

    sendToSender(dwActorID, senderReply, 0);
    if (senderReply.nResult != 0) {
        return;
    }

    PS_CHAT_WHISPER targetReply = stChatWhisper;
    targetReply.nResult = 0;

    const std::shared_ptr<CUserPartyInfo> targetPartyInfo = GetPartyUser(targetUser->GetCID());
    const std::uint32_t matchingID = targetPartyInfo ? targetPartyInfo->GetMatchingID() : 0u;

    XSendPacket xSendPacket(0xF3u, 0x10u);
    xSendPacket.XParse << matchingID;
    xSendPacket << targetReply;
    xSendPacket.XParse << static_cast<std::uint8_t>(0);
    xSendPacket << psItemLinkInfo;
    targetUser->SendPacket(xSendPacket);
}

void XRelayServer::SendChatMegaPhone(const PS_CHAT_MEGAPHONE& stMegaPhone,
                                     const PS_CHAT_ITEM_LINK_FOR_SERVER& psItemLinkInfo) {
    XSendPacket sendPacket(0xF3u, 0x17u);
    sendPacket << stMegaPhone;
    sendPacket << psItemLinkInfo;

    for (XClient* client = m_xClientPool.GetHead(); client; client = m_xClientPool.GetNext()) {
        if (!client || client->GetSessionID() == 0) {
            continue;
        }
        client->SendEx(sendPacket);
    }
}


void XRelayServer::SetCachingLoad(std::uint32_t loadMask) {
    if ((m_dwCachingLoad & loadMask) == 0) {
        m_dwCachingLoad |= loadMask;
    }

    if (loadMask > E_SERVER_CACHING_LOAD_NONE &&
        (loadMask <= E_SERVER_CACHING_LOAD_LEAGUE || loadMask == E_SERVER_CACHING_LOAD_FRINED) &&
        (m_dwCachingLoad & E_SERVER_CACHING_LOAD_FRINED) != 0 &&
        (m_dwCachingLoad & E_SERVER_CACHING_LOAD_PARTY) != 0 &&
        (m_dwCachingLoad & E_SERVER_CACHING_LOAD_LEAGUE) != 0) {
        SendCachingLoad();
    }

    LogHelper::LogInfo("game.system",
                       "<SetCachingLoad> Server:%u, Req:%u",
                       static_cast<unsigned int>(m_dwCachingLoad),
                       static_cast<unsigned int>(loadMask));
}

void XRelayServer::UnSetCachingLoad(std::uint32_t loadMask) {
    if ((m_dwCachingLoad & loadMask) != 0) {
        m_dwCachingLoad ^= loadMask;
    }

    LogHelper::LogInfo("game.system",
                       "<UnSetCachingLoad> Server:%u, Req:%u",
                       static_cast<unsigned int>(m_dwCachingLoad),
                       static_cast<unsigned int>(loadMask));
    SendCachingLoad();
}

void XRelayServer::SendCachingLoad() {
    CFAutoSlimReadLock autolock(&m_rwServerLock);
    bool bComplete = true;
    for (const auto& [serverID, pServer] : m_mapGameServer) {
        static_cast<void>(serverID);
        if (!pServer || !pServer->IsSyncLoad()) {
            bComplete = false;
        }
    }

    if (!m_mapGameServer.empty()) {
        if (bComplete) {
            SetCachingLoad(E_SERVER_CACHING_LOAD_USER);
        }
        XSendPacket sendPacket(eCMD_SERVER, 0x70u);
        sendPacket.XParse << m_dwCachingLoad;
        LogHelper::LogInfo("game.system",
                           "<SendCachingLoad> CachingState : [%u]",
                           static_cast<unsigned int>(m_dwCachingLoad));
    }
}

bool XRelayServer::SendDBGame(const XSendDBPacket& packet) {
    const int agentCount = m_xDBAgentMgr.GetGameDBAgentCount();
    const int index = agentCount > 0 ? packet.GetOrderID() % agentCount : 0;
    if (m_xDBAgentMgr.SendGameDBAgent(index, packet)) {
        return true;
    }

    LogHelper::LogError("game.system", "<Send GameDB> Error Stat!");
    return false;
}

bool XRelayServer::SendDBAccount(const XSendDBPacket& packet) {
    const int agentCount = m_xDBAgentMgr.GetAccountDBAgentCount();
    const int index = agentCount > 0 ? packet.GetOrderID() % agentCount : 0;
    if (m_xDBAgentMgr.SendAccountDBAgent(index, packet)) {
        return true;
    }

    LogHelper::LogError("game.system", "<Send AccountDB> Error Stat!");
    return false;
}

void XRelayServer::SendDBLog(int uaid,
                             int ucid,
                             std::uint16_t mainType,
                             std::uint16_t subType,
                             int param0,
                             int param1,
                             int param2,
                             int param3,
                             int param4,
                             std::int64_t param5,
                             std::int64_t param6,
                             const wchar_t* comment) {
    ST_LOG_GAME stLog{};
    stLog._nUAID = uaid;
    stLog._nUCID = ucid;
    stLog._sMainType = mainType;
    stLog._sSubType = subType;
    stLog.nParam0 = param0;
    stLog.nParam1 = param1;
    stLog.nParam2 = param2;
    stLog.nParam3 = param3;
    stLog.nParam4 = param4;
    stLog.nParam5 = param5;
    stLog.nParam6 = param6;
    if (comment) {
        wcsncpy_s(stLog.szComment, comment, _TRUNCATE);
    }

    XSendDBPacket sendPacket(nullptr, 0x42u, 1u);
    sendPacket << stLog;
    SendDBGame(sendPacket);
}

CServer* XRelayServer::GetServer(std::uint32_t dwServerID) {
    CFAutoSlimReadLock autolock(&m_rwServerLock);
    const auto it = m_mapGameServer.find(dwServerID);
    return it == m_mapGameServer.end() ? nullptr : it->second;
}

int XRelayServer::ConsolCtrlHandler(unsigned int dwOPCode) {
#ifndef _WIN32
    if (dwOPCode == static_cast<unsigned int>(SIGINT) ||
        dwOPCode == static_cast<unsigned int>(SIGTERM)) {
        auto* server = TXSingleton<XRelayServer>::Instance();
        return server ? static_cast<XServer*>(server)->Shutdown(0xFFFFFFFFu) : 0;
    }
#endif

    if (dwOPCode > 2 && (dwOPCode <= 4 || dwOPCode > 6)) {
        return 0;
    }

    auto* server = TXSingleton<XRelayServer>::Instance();
    return server ? static_cast<XServer*>(server)->Shutdown(0xFFFFFFFFu) : 0;
}

void XRelayServer::SetName() {
    std::snprintf(m_szName, sizeof(m_szName), "%s", "RELAY");
}

bool XRelayServer::InitServer() {
    m_xOption.ShowServerInfo();

    int logicThreadCount = m_xOption.GetLogicThread();
    if (logicThreadCount <= 0) {
        logicThreadCount = 1;
    }
    if (!CLogicThreadManager::Instance().Start(logicThreadCount)) {
        LogHelper::LogError("game.relay", "Error LogicThreadManager Start fail");
        return false;
    }

    const char* commonDNS = m_xOption.GetDNS(2);
    if (!m_PartyMatchingConfig.Init(commonDNS)) {
        LogHelper::LogError("game.relay", "Error PartyMatchingConfig Init fail");
        return false;
    }

    if (!m_MazeOpenControl.Init(commonDNS)) {
        LogHelper::LogError("game.relay", "Error MazeOpenControl Init fail");
        return false;
    }

    m_xDBAgentMgr.Init();
    m_xDBAgentMgr.AutoConnect();
    LogHelper::LogInfo("game.relay", "[INIT] DBAgent Init ");

    if (!m_scControlSocket.Init(ePoolIDRelayServer, "127.0.0.1", 5001)) {
        LogHelper::LogError("game.relay", "Error Relay Control Socket Init fail");
        return false;
    }

    m_scControlSocket.SetMyInfo(&m_xOption);
    if (!m_scControlSocket.Connect()) {
        LogHelper::LogError("game.relay", "Failed conect Control Server!!");
        return false;
    }

    LogHelper::LogInfo("game.relay", "[INIT] RelayControlSocket - Init ");
    return true;
}

bool XRelayServer::Clear(std::uint32_t maxWait) {
    if (m_bClose) {
        return false;
    }

    m_scControlSocket.Shutdown(maxWait);
    m_xDBAgentMgr.DisConnect();
    CLogicThreadManager::Instance().End();
    m_bClose = true;
    return true;
}

void XRelayServer::OnUpdate(std::uint64_t currentTick) {
    if (m_dw64FPSTick == 0) {
        m_dw64FPSTick = currentTick;
    }

    if (currentTick - m_dw64FPSTick >= 1000) {
        XServer::OnUpdate(currentTick);
        m_dw64FPSTick = currentTick;
        m_dwFrame = 0;
    } else {
        ++m_dwFrame;
    }

    const bool controlConnected = m_scControlSocket.XIOCPClient::IsConnection();
    if (controlConnected || m_bClose) {
        if (controlConnected) {
            m_scControlSocket.SendUpdateServerInfo(1, 0);
        }
    } else if (m_dwConnectTick < currentTick) {
        m_scControlSocket.Connect();
        m_dwConnectTick = currentTick + 30000;
        LogHelper::LogInfo("game.relay", "OnUpdate [RelayControlSocket.Connect()]");
    }

    if (m_bClose) {
        m_bRunFlag = false;
    }
}

int XRelayServer::SetConsoleHandler(int add) {
#ifdef _WIN32
    return SetConsoleCtrlHandler(ConsoleCtrlThunk, add ? TRUE : FALSE) ? 1 : 0;
#else
    if (add) {
        std::signal(SIGINT, ConsoleCtrlThunk);
        std::signal(SIGTERM, ConsoleCtrlThunk);
    } else {
        std::signal(SIGINT, SIG_DFL);
        std::signal(SIGTERM, SIG_DFL);
    }
    return 1;
#endif
}
