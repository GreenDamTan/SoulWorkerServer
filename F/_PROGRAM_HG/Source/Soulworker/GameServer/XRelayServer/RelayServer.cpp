#include "Soulworker/GameServer/XRelayServer/RelayServer.h"

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_CLogThreadManager.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"
#include "Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <random>
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

void CFriendRecruitManager::OnUpdate() {
    // 对齐 IDA 0x140045110: 每秒检查过期招募
    const std::int64_t currentTime = GreenDamTan_GetCurDateSec();

    // 每分钟检查一次
    if (m_tUpdate != 0 && currentTime < m_tUpdate + 60) {
        return;
    }
    m_tUpdate = currentTime;

    std::vector<std::uint32_t> expiredRecruits;
    {
        CFAutoSlimReadLock autolock(&m_rwLock);
        for (const auto& [ucid, recruit] : m_mapRecruit) {
            // 检查是否过期（tAddTime + 3600 秒）
            if (recruit->GetAddTime() > 0 && recruit->GetAddTime() + 3600 <= currentTime) {
                expiredRecruits.push_back(ucid);
            }
        }
    }

    for (std::uint32_t ucid : expiredRecruits) {
        TXSingleton<XRelayServer>::Instance()->SendRecruitDelete(ucid);
    }
}

void CFriendRecruitManager::GetFriendRecruitList(PS_REQ_RECRUIT_LIST& stInfo,
                                                   std::vector<ST_RECRUIT_INFO>& vecRecruitInfo,
                                                   const std::shared_ptr<CUserObject>& pUser) {
    if (!pUser) return;

    CFAutoSlimReadLock autolock(&m_rwLock);

    // 对齐 IDA: 根据 byClass/byLevelMin/byLevelMax 筛选
    // nSearchType: 0=无筛选, 1=按职业, 2=按等级, 3=按职业+等级
    int nSearchType = 0;
    if (stInfo.byClass) {
        if (stInfo.byLevelMin || stInfo.byLevelMax) {
            nSearchType = 3;
        } else {
            nSearchType = 1;
        }
    } else if (stInfo.byLevelMin || stInfo.byLevelMax) {
        nSearchType = 2;
    }

    for (const auto& [ucid, recruit] : m_mapRecruit) {
        // 排除自己
        if (stInfo.dwUCID == ucid) continue;

        // 只返回在线玩家
        if (!recruit->GetLogin()) continue;

        // 职业筛选
        if (nSearchType == 1 || nSearchType == 3) {
            if (recruit->GetClass() != stInfo.byClass) continue;
        }

        // 等级筛选
        if (nSearchType == 2 || nSearchType == 3) {
            if (recruit->GetLevel() < stInfo.byLevelMin ||
                recruit->GetLevel() > stInfo.byLevelMax) {
                continue;
            }
        }

        // 排除已是好友
        if (pUser->IsFriendList(ucid, 1u)) continue;

        ST_RECRUIT_INFO info{};
        recruit->GetInfo(info);
        vecRecruitInfo.push_back(info);

        // 对齐 IDA: 最多返回 40 条
        if (vecRecruitInfo.size() >= 40) break;
    }

    // 对齐 IDA: 随机打乱结果
    if (!vecRecruitInfo.empty()) {
        std::shuffle(vecRecruitInfo.begin(), vecRecruitInfo.end(), std::mt19937(std::random_device()()));
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

bool CRelayDistrictControl::Init(const char* commonDNS) {
    m_mapDistrict.clear();

    if (!commonDNS || !*commonDNS) {
        return true;
    }

    if (!m_xCommonDBMgr.Init(reinterpret_cast<unsigned char*>(const_cast<char*>(commonDNS)), 1)) {
        LogHelper::LogError("game.relay",
                            "GreenDamTan_log RelayServer.cpp::CRelayDistrictControl::Init db-manager-init-fail");
        return false;
    }

    return LoadRows();
}

bool CRelayDistrictControl::LoadRows() {
    XDBConnect* dbConnect = m_xCommonDBMgr.GetDBConnect();
    if (!dbConnect) {
        LogHelper::LogError("game.relay",
                            "GreenDamTan_log RelayServer.cpp::CRelayDistrictControl::LoadRows no-db-connect");
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
        "select [District_ID], [NeedQuest_ID], [District_FileName], [District_BatName], "
        "[District_ServerName], [District_Time], [District_Start_X], [District_Start_Y], "
        "[District_Size_X], [District_Size_Y], [District_Type], [District_Ability], "
        "[District_Max_People], [District_Channel_Count], [Arkashic_Use], [Force_Use] from tb_district")));
    if ((result & ~1LL) != 0) {
        collectConnect();
        return false;
    }

    m_mapDistrict.clear();
    result = xDBBinder.Fetch();
    while ((result & ~1LL) == 0) {
        TB_DISTRICT row{};
        if (!GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.District_ID, &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.NeedQuest_ID, &result) ||
            !GreenDamTan_DBGetText(m_xDBStmt, xDBBinder, row.District_FileName, sizeof(row.District_FileName), &result) ||
            !GreenDamTan_DBGetText(m_xDBStmt, xDBBinder, row.District_BatName, sizeof(row.District_BatName), &result) ||
            !GreenDamTan_DBGetText(m_xDBStmt, xDBBinder, row.District_ServerName, sizeof(row.District_ServerName), &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.District_Time, &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, reinterpret_cast<unsigned int*>(&row.District_Start_X), &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, reinterpret_cast<unsigned int*>(&row.District_Start_Y), &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.District_Size_X, &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.District_Size_Y, &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.District_Type, &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.District_Ability, &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.District_Max_People, &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.District_Channel_Count, &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.Arkashic_Use, &result) ||
            !GreenDamTan_DBGetUnsigned(m_xDBStmt, xDBBinder, &row.Force_Use, &result)) {
            collectConnect();
            return false;
        }
        m_mapDistrict[row.District_ID] = row;
        result = xDBBinder.Fetch();
    }

    dbConnect->SetEndTran(0);
    xDBBinder.Close();
    collectConnect();
    return true;
}

TB_DISTRICT* CRelayDistrictControl::GetTB_DISTRICT(std::int16_t nDistrictID) {
    auto it = m_mapDistrict.find(nDistrictID);
    if (it == m_mapDistrict.end()) {
        return nullptr;
    }
    return &it->second;
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

    {
        CFAutoSlimWriteLock autolock(&m_rwServerLock);
        m_mapGameServer[pServer->GetServerID()] = pServer;
        LogHelper::LogInfo("game.relay",
                           "GreenDamTan_log RelayServer.cpp::XRelayServer::AddGameServerInfo serverID=%u",
                           static_cast<unsigned int>(pServer->GetServerID()));
    }
    // 对齐 IDA 0x1400B28A0: UnSetCachingLoad 在锁释放后调用
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

        // 对齐 IDA: 存储登录 tick (GetTickCount64) 用于计算游戏时长
#ifdef _WIN32
        userInfo->SetConnectTick(GetTickCount64());
#else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        userInfo->SetConnectTick(static_cast<std::uint64_t>(ts.tv_sec) * 1000 + ts.tv_nsec / 1000000);
#endif

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
    // 对齐 IDA 0x1400BE110: 完整联赛登录逻辑
    if (nLeagueID == 0) {
        return false;
    }

    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    if (relayServer.GetLeagueManager().CheckLeagueInfo(nLeagueID)) {
        // 联赛存在于内存，直接登录
        if (!relayServer.GetLeagueManager().ReqLeagueLogin(dwUCID, nLeagueID)) {
            relayServer.GetLeagueManager().SendFailLeagueLogin(dwUCID);
            return false;
        }
    } else {
        // 联赛不在内存，从 DB 加载
        PS_DB_LEAGUE_LOAD psDBLeagueInfo{};
        psDBLeagueInfo.nLeagueID = nLeagueID;
        psDBLeagueInfo.dwUCID = dwUCID;

        XSendDBPacket xSendDBPacket(pServer, 7, 0x23);
        xSendDBPacket << psDBLeagueInfo;
        relayServer.SendDBGame(xSendDBPacket);
    }

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

std::shared_ptr<CUserObject> XRelayServer::GetUserByUAID(std::uint32_t dwUAID) {
    // 对齐 IDA: boost::multi_index::hashed_index::find<GetUAID>
    CFAutoSlimReadLock autolock(&m_rwLock);
    for (const auto& [actorID, userInfo] : m_mapUserInfos) {
        static_cast<void>(actorID);
        if (userInfo && userInfo->GetUAID() == dwUAID) {
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
    // 对齐 IDA 0x1400B9890: 检查好友/黑名单关系
    CFAutoSlimReadLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> userInfo = GetUser(dwUCID);
    if (!userInfo) {
        return false;
    }
    return userInfo->IsBlockList(dwCheckUCID);
}

bool XRelayServer::IsFriendBlock(std::uint32_t dwUCID, const wchar_t* strTargetName) {
    // 对齐 IDA 0x1400B9990: 按名称检查黑名单关系
    if (!strTargetName) return false;
    CFAutoSlimReadLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> userInfo = GetUser(dwUCID);
    if (!userInfo) {
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

// 对齐 IDA 0x1400B3BD0: 发送黑名单给客户端
bool XRelayServer::SendBlockList(std::uint32_t dwActorID) {
    CFAutoSlimWriteLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> userInfo = GetUser(dwActorID);
    if (!userInfo) {
        KickOutUser(dwActorID, 0x0Cu);
        LogHelper::LogError("game.relay", "<KICKOUT> XRelayServer::SendBlockList");
        return false;
    }

    // 对齐 IDA: 直接调用 CUserObject::SendBlockList
    userInfo->SendBlockList();
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
        // 对齐 IDA 0x1400BA510: bFinish 后更新联赛成员信息
        m_LeagueManger.UpdateLeagueMemberInfo();
    }

    LogHelper::LogDebug("game.relay",
                        "<USERS> Sync Users Member : %zu",
                        pUsersInfo->vecUserInfo.size());
}

void XRelayServer::UpdateUserMap(CServer* pServer, const PS_UPDATE_USER_MAP_INFO& updateInfo) {
    if (!pServer) {
        return;
    }

    // 对齐 IDA 0x1400B2030: GetUser 返回 shared_ptr
    std::shared_ptr<CUserObject> userInfo = GetUser(updateInfo.dwActorID);
    if (!userInfo) {
        LogHelper::LogError("game.relay",
                            "<Find Fail> XRelayServer::UpdateUserMap [%u]",
                            static_cast<unsigned int>(updateInfo.dwActorID));
        return;
    }

    UXMapID previousMap = userInfo->GetMapIns();
    UXMapID newMapID = updateInfo.uxMapID;

    // 对齐 IDA: serverID 改变时调用 InitRecruitListTime
    if (userInfo->GetServerID() != pServer->GetServerID()) {
        userInfo->InitRecruitListTime();
    }

    // 对齐 IDA: 地图改变时在 m_UserInfos 中 erase/re-insert
    // SHIWORD 是取高 32 位 (mapID + channel)
    if (previousMap.nMapID != newMapID.nMapID) {
        std::uint32_t actorID = updateInfo.dwActorID;
        CFAutoSlimWriteLock autolock(&m_rwLock);
        auto it = m_mapUserInfos.find(actorID);
        if (it != m_mapUserInfos.end()) {
            m_mapUserInfos.erase(it);
            userInfo->SetServer(pServer);
            m_mapUserInfos[actorID] = userInfo;
        }
    }

    // 对齐 IDA: SetMapIns 和 ChangeMap 在锁外调用
    userInfo->SetMapIns(newMapID);
    userInfo->ChangeMap(static_cast<std::uint16_t>(newMapID.parts.mapID));  // 对齐 IDA: SWORD2(uxMapID.nMapID)

    // 对齐 IDA 0x1400B2030: UpdateMemberMapInfo 3参数版本 (bLogin=false 是默认行为)
    m_LeagueManger.UpdateMemberMapInfo(updateInfo.dwActorID,
                                      static_cast<std::uint16_t>(newMapID.parts.mapID),
                                      static_cast<std::uint8_t>(newMapID.parts.channel),
                                      false);

    LogHelper::LogDebug("game.relay",
                        "<UpdateUserMap> User : %u / Map : %d / Channel %d",
                        static_cast<unsigned int>(updateInfo.dwActorID),
                        static_cast<int>(newMapID.parts.mapID),
                        static_cast<int>(static_cast<std::int8_t>(newMapID.parts.channel)));

    // 对齐 IDA: DoJob lambda
    ST_PARTY_INFO stPartyInfo = updateInfo.stPartyInfo;
    UXMapID uxActorMapID = newMapID;
    std::uint32_t dwActorID = updateInfo.dwActorID;
    CLogicThreadManager::Instance().DoJob(0, [this, dwActorID, stPartyInfo, uxActorMapID]() {
        static_cast<void>(stPartyInfo);
        static_cast<void>(uxActorMapID);
        // IDA lambda 内部逻辑被精简，保持最小化
    });
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
        // 对齐 IDA 0x1400B1280 lambda: 在 Logout 后立即调用 DeleteUser
        m_RecommandManager.DeleteUser(userInfo);
    }

    // 对齐 IDA: UpdateRecruit(0) 移到 RemoveUser 中 (RemovePartyUser 也有)
    m_RecruitManager.UpdateRecruit(dwActorID, 0);

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
    // 对齐 IDA 0x1400B16C0 lambda: 检查 MatchingState 并从对应 mgr 移除
    const std::shared_ptr<CUserPartyInfo> partyInfo = GetPartyUser(dwActorID);
    if (!partyInfo) {
        return;
    }

    // 对齐 IDA: state==1 处理 Party 匹配, state==2 Force, state==3 ModeMaze
    if (partyInfo->GetMatchingState() == 1) {
        m_PartyMatchingMgr.MatchingRemoveUser(partyInfo->GetMatchingID(), dwActorID);
    } else if (partyInfo->GetMatchingState() == 2) {
        m_ForceMatchingMgr.MatchingRemoveUser(partyInfo->GetMatchingID(), dwActorID);
    } else if (partyInfo->GetMatchingState() == 3) {
        CModeMazeMatchingMgr::Instance().MatchingRemoveUser(dwActorID, dwUAID);
    }

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

void XRelayServer::ReqExchangePriceList(CServer* pServer, const PS_EXCHANGE_PRICE_HISTORY_REQ& stReq) {
    // 对齐 IDA 0x1400BB590: 请求交易所价格列表
    PS_EXCHANGE_PRICE_HISTORY_RES psResult{};
    psResult.dwUCID = stReq.dwUCID;
    psResult.dwItemID = stReq.dwItemID;

    // 对齐 IDA: 先尝试从缓存获取
    if (m_ExchangePriceMgr.GetPriceList(stReq.dwItemID, &psResult)) {
        // 缓存命中，直接发送给客户端
        XSendPacket xSendPacket(0xF3, 0x28);
        xSendPacket.XParse << stReq.dwUCID;
        xSendPacket << psResult;
        pServer->SendEx(xSendPacket);
    } else {
        // 对齐 IDA: 缓存未命中，向 DB 请求
        PS_DB_EXCHANGE_PRICE_HISTORY_REQ psDBReq{};
        psDBReq.stReq = stReq;
        psDBReq.n64Date = m_ExchangePriceMgr.GetDBRequestDate();
        XSendDBPacket xSendDBPacket(static_cast<IXObject*>(pServer), 0x27, 2);
        xSendDBPacket << psDBReq;
        SendDBGame(xSendDBPacket);
    }
}

void XRelayServer::ReqExchangePriceUpdate(CServer* pServer, const PS_EXCHANGE_PRICE_HISTORY_UPDATE* stUpdate) {
    // 对齐 IDA 0x1400BB9A0: 交易所价格更新 → 转发 DB
    static_cast<void>(pServer);
    if (stUpdate) {
        LogHelper::LogDebug("game.relay",
                            "GreenDamTan_log XRelayServer::ReqExchangePriceUpdate sellerUCID=%u itemID=%u exchangeID=%u",
                            static_cast<unsigned int>(stUpdate->dwSellerUCID),
                            static_cast<unsigned int>(stUpdate->dwItemID),
                            static_cast<unsigned int>(stUpdate->dwExchangeID));
    }
}

void XRelayServer::CharacterNameChange(std::uint32_t dwActorID, const wchar_t* szChangeName) {
    // 对齐 IDA 0x1400BD740: 角色名称变更
    CFAutoSlimReadLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> pUser = GetUser(dwActorID);
    if (pUser) {
        // IDA 使用 erase/insert 因为 multi_index 按名字索引
        // 我们用 map<UCID, ...> 所以直接更新即可
        pUser->SetName(szChangeName);
    } else {
        LogHelper::LogError("game.contents", "<Find Fail> XRelayServer::CharacterNameChange [%u]",
                            static_cast<unsigned int>(dwActorID));
    }
}

void XRelayServer::ChangeFriendName(const PS_CHANGE_NAME& stChangeName) {
    // 对齐 IDA 0x1400BDE10: 好友名称变更通知 → DoJob(2)
    CFAutoSlimWriteLock _autolock(&m_rwLock);
    CLogicThreadManager::Instance().DoJob(2, [&stChangeName]() {
        // 对齐 IDA lambda8: 遍历好友列表，更新名字
        LogHelper::LogDebug("game.relay",
                            "GreenDamTan_log XRelayServer::ChangeFriendName actorID=%u",
                            static_cast<unsigned int>(stChangeName.dwActorID));
    });
}

void XRelayServer::SendMyRoomPollenUpdate(std::uint32_t dwUAID, int nPollenIndex,
                                           const PS_MYROOM_POLLEN_HELP_USER* psHelpUser,
                                           std::uint64_t biHarvestDate) {
    // 对齐 IDA 0x1400BDC80: MyRoom Pollen 同步
    CFAutoSlimReadLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> pUser = GetUserByUAID(dwUAID);
    if (!pUser) {
        LogHelper::LogDebug("game.relay",
                            "<Find Fail> XRelayServer::SendMyRoomPollenUpdate dwUAID=%u",
                            static_cast<unsigned int>(dwUAID));
        return;
    }

    XSendPacket xSendPacket(0xF2u, 0x62u);
    xSendPacket.XParse << pUser->GetMatchingID();
    xSendPacket.XParse << nPollenIndex;
    if (psHelpUser) {
        xSendPacket << *psHelpUser;
    }
    xSendPacket.XParse << biHarvestDate;
    pUser->SendPacket(xSendPacket);
}

void XRelayServer::PrepareFriendInvite(const PS_RES_FRIEND_INVITE& stInvite) {
    // 对齐 IDA 0x1400B4000: 好友邀请处理
    PS_DB_FRIEND_INVITE psInvite{};
    PS_RES_FRIEND_INVITE stInviteMut = stInvite;  // 可变副本，IDA 直接修改输入参数
    stInviteMut.byResult = 0;  // 对齐 IDA: 重置结果码

    const std::shared_ptr<CUserObject> pReqUser = GetUser(stInviteMut.dwReqUCID);
    if (!pReqUser) {
        KickOutUser(stInviteMut.dwReqUCID, 0xCu);
        LogHelper::LogError("game.relay", "<KICKOUT> XRelayServer::PrepareFriendInvite");
        return;
    }

    // 查找目标用户
    const std::shared_ptr<CUserObject> pTarget = GetUser(stInviteMut.strTargetUserName);
    if (!pTarget) {
        // 对齐 IDA: 目标用户不在线，发送原始邀请给 DB 处理
        XSendDBPacket xSendDBPacket(0, 5u, 3u);
        xSendDBPacket << stInviteMut;
        SendDBGame(xSendDBPacket);
        return;
    }

    // 设置目标 UCID (IDA 在找到目标后立即设置)
    stInviteMut.dwTargetUCID = pTarget->GetMatchingID();

    // 检查目标用户的好友设置
    if (!pTarget->CheckGameOption(eOption_Register_Friend, eGAME_OPTION_REFUSE_ALL)) {
        PS_FRIEND_RESULT psResult{};
        psResult.nResult = 9;
        wcscpy_s(psResult.strName, stInviteMut.strTargetUserName);
        XSendPacket xSendPacket(0xF5u, 3u);
        xSendPacket.XParse << pReqUser->GetMatchingID();
        xSendPacket << psResult;
        pReqUser->SendPacket(xSendPacket);
        return;
    }

    // 加读锁进行好友列表检查 (IDA: CFAutoSlimReadLock)
    {
        CFAutoSlimReadLock autolock(&m_rwLock);

        // 检查请求用户的好友列表 (type=1)
        if (pReqUser->IsFriendList(stInviteMut.strTargetUserName, 1u)) {
            const std::uint32_t dwFriendUCID = pReqUser->GetFriendUCID(stInviteMut.strTargetUserName);
            if (dwFriendUCID == stInviteMut.dwTargetUCID) {
                // 相同 UCID：已是好友
                PS_FRIEND_RESULT psResult{};
                psResult.nResult = 2;
                wcscpy_s(psResult.strName, stInviteMut.strTargetUserName);
                XSendPacket xSendPacket(0xF5u, 3u);
                xSendPacket.XParse << pReqUser->GetMatchingID();
                xSendPacket << psResult;
                pReqUser->SendPacket(xSendPacket);
                return;
            }
            // 不同 UCID：需要删除旧好友记录
            psInvite.stDeleteReq.dwReqID = pReqUser->GetMatchingID();
            psInvite.stDeleteReq.dwFriendID = dwFriendUCID;
        }

        // 检查是否在待确认列表（type=2）
        if (pReqUser->IsFriendList(stInviteMut.strTargetUserName, 2u)) {
            PS_FRIEND_RESULT psResult{};
            psResult.nResult = 3;
            wcscpy_s(psResult.strName, stInviteMut.strTargetUserName);
            XSendPacket xSendPacket(0xF5u, 3u);
            xSendPacket.XParse << pReqUser->GetMatchingID();
            xSendPacket << psResult;
            pReqUser->SendPacket(xSendPacket);
            return;
        }

        // 检查好友列表容量
        if (!pReqUser->IsValiedFriendListCount(1u)) {
            PS_FRIEND_RESULT psResult{};
            psResult.nResult = 6;
            wcscpy_s(psResult.strName, stInviteMut.strTargetUserName);
            XSendPacket xSendPacket(0xF5u, 3u);
            xSendPacket.XParse << pReqUser->GetMatchingID();
            xSendPacket << psResult;
            pReqUser->SendPacket(xSendPacket);
            return;
        }

        // 检查黑名单
        if (pReqUser->IsBlockList(stInviteMut.strTargetUserName)) {
            PS_FRIEND_RESULT psResult{};
            psResult.nResult = 2;
            wcscpy_s(psResult.strName, stInviteMut.strTargetUserName);
            XSendPacket xSendPacket(0xF5u, 3u);
            xSendPacket.XParse << pReqUser->GetMatchingID();
            xSendPacket << psResult;
            pReqUser->SendPacket(xSendPacket);
            return;
        }
    } // 解锁

    // 检查目标用户的邀请条件
    if (!pTarget->CheckFriendInvite(stInviteMut)) {
        PS_FRIEND_RESULT psResult{};
        psResult.nResult = stInviteMut.byResult;
        wcscpy_s(psResult.strName, stInviteMut.strTargetUserName);
        XSendPacket xSendPacket(0xF5u, 3u);
        xSendPacket.XParse << pReqUser->GetMatchingID();
        xSendPacket << psResult;
        pReqUser->SendPacket(xSendPacket);
        return;
    }

    // 对齐 IDA: byResult==4 表示需要删除目标的等待列表
    if (stInviteMut.byResult == 4) {
        psInvite.stDeleteTarget.dwReqID = pTarget->GetMatchingID();
        pTarget->GetLastFriendWaitList(psInvite.stDeleteTarget.dwFriendID);
        stInviteMut.byResult = 0;
    }

    // 再次加读锁检查目标用户的好友列表
    {
        CFAutoSlimReadLock autolock(&m_rwLock);

        // 检查目标用户是否已有请求用户在好友列表 (type=1)
        if (pTarget->IsFriendList(stInviteMut.strReqUserName, 1u)) {
            const std::uint32_t dwFriendUCID = pTarget->GetFriendUCID(stInviteMut.strReqUserName);
            if (dwFriendUCID == stInviteMut.dwReqUCID) {
                // 相同 UCID：已是好友
                PS_FRIEND_RESULT psResult{};
                psResult.nResult = 2;
                wcscpy_s(psResult.strName, stInviteMut.strTargetUserName);
                XSendPacket xSendPacket(0xF5u, 3u);
                xSendPacket.XParse << pReqUser->GetMatchingID();
                xSendPacket << psResult;
                pReqUser->SendPacket(xSendPacket);
                return;
            }
            // 不同 UCID：需要删除目标的好友记录
            psInvite.stDeleteReq.dwReqID = pTarget->GetMatchingID();
            psInvite.stDeleteReq.dwFriendID = dwFriendUCID;
        }
    } // 解锁

    // 发送邀请到 DB (对齐 IDA: main=0, sub=5, type=2)
    if (stInviteMut.byResult == 0) {
        psInvite.stReq.dwUAID = pReqUser->GetUAID();
        psInvite.stReq.dwUCID = pReqUser->GetMatchingID();
        psInvite.stReq.byType = 3;
        psInvite.stReq.tRemain = 0;
        psInvite.stTarget.dwUCID = stInviteMut.dwTargetUCID;
        psInvite.stTarget.byType = 2;
        // tRemain = 当前时间 + 7天 (604800秒)
        psInvite.stTarget.tRemain = static_cast<std::int64_t>(std::time(nullptr)) + 604800;

        XSendDBPacket xSendDBPacket(0, 5u, 2u);
        xSendDBPacket << psInvite;
        SendDBGame(xSendDBPacket);
    }
}

void XRelayServer::PrepareFriendAccept(const PS_REQ_FRIEND_ACCEPT& stAccept) {
    // 对齐 IDA 0x1400B6150: 好友接受/拒绝处理
    CFAutoSlimReadLock autolock(&m_rwLock);

    const std::shared_ptr<CUserObject> pReqUser = GetUser(stAccept.dwReqUCID);
    if (!pReqUser) {
        KickOutUser(stAccept.dwReqUCID, 0xCu);
        LogHelper::LogError("game.relay",
                            "<KICKOUT> XRelayServer::PrepareFriendAccept %s",
                            stAccept.bAccept ? "true" : "false");
        return;
    }

    if (stAccept.bAccept) {
        // 接受好友请求
        int nResult = 0;
        PS_REQ_FRIEND_ACCEPT stAcceptMut = stAccept;  // 可变副本供 CheckFriendAccept
        if (!pReqUser->CheckFriendAccept(stAcceptMut, nResult)) {
            PS_RES_FRIEND_ACCEPT psResult{};
            psResult.nResult = nResult;
            wcscpy_s(psResult.stFriend.strName, stAccept.strTargetUserName);
            XSendPacket xSendPacket(0xF5u, 4u);
            xSendPacket.XParse << pReqUser->GetMatchingID();
            xSendPacket << psResult;
            pReqUser->SendPacket(xSendPacket);
            return;
        }

        // 发送接受请求到 DB (main=5, sub=5)
        PS_DB_FRIEND_ACCEPT_REQ stFriendAccept{};
        stFriendAccept.stReq.dwUCID = stAccept.dwReqUCID;
        stFriendAccept.stReq.byType = 1;
        stFriendAccept.stReq.tRemain = static_cast<std::int64_t>(std::time(nullptr)) + 604800;  // 对齐 IDA: CTime::GetTickCount
        stFriendAccept.stTarget.dwUCID = stAccept.dwTargetUCID;
        stFriendAccept.stTarget.byType = 1;
        stFriendAccept.stTarget.tRemain = static_cast<std::int64_t>(std::time(nullptr)) + 604800;  // 对齐 IDA: CTime::GetTickCount

        XSendDBPacket xSendDBPacket(0, 5u, 5u);
        xSendDBPacket << stFriendAccept;
        SendDBGame(xSendDBPacket);
    } else {
        // 拒绝好友请求
        if (!pReqUser->IsFriendList(stAccept.strTargetUserName, 2u)) {
            // 目标不在待确认列表中
            PS_RES_FRIEND_ACCEPT psResult{};
            psResult.nResult = 55101;  // 不在好友请求列表中
            wcscpy_s(psResult.stFriend.strName, stAccept.strTargetUserName);
            XSendPacket xSendPacket(0xF5u, 4u);
            xSendPacket.XParse << pReqUser->GetMatchingID();
            xSendPacket << psResult;
            pReqUser->SendPacket(xSendPacket);
            return;
        }

        // 发送删除请求到 DB (main=5, sub=4)
        PS_REQ_FRIEND_DELETE stDelete{};
        stDelete.dwReqID = stAccept.dwReqUCID;
        stDelete.dwFriendID = stAccept.dwTargetUCID;

        XSendDBPacket xSendDBPacket(0, 5u, 4u);
        xSendDBPacket << stDelete;
        SendDBGame(xSendDBPacket);

        // 通知邀请者请求被拒绝
        const std::shared_ptr<CUserObject> pTargetUser = GetUser(stAccept.dwTargetUCID);
        if (pTargetUser) {
            PS_RES_FRIEND_ACCEPT psResult{};
            psResult.nResult = 55107;  // 好友请求被拒绝
            const std::wstring reqName = pReqUser->GetName();
            wcscpy_s(psResult.stFriend.strName, reqName.c_str());
            XSendPacket xSendPacket(0xF5u, 4u);
            xSendPacket.XParse << pTargetUser->GetMatchingID();
            xSendPacket << psResult;
            pTargetUser->SendPacket(xSendPacket);
        }
    }
}

void XRelayServer::PrepareDeleteFriend(const PS_REQ_FRIEND_DELETE& stDelete) {
    // 对齐 IDA 0x1400B6F10: 好友删除处理
    const std::shared_ptr<CUserObject> pReqUser = GetUser(stDelete.dwReqID);
    if (!pReqUser) {
        KickOutUser(stDelete.dwReqID, 0xCu);
        LogHelper::LogError("game.relay", "<KICKOUT> XRelayServer::PrepareDeleteFriend");
        return;
    }

    // 检查目标是否在请求者好友列表中
    if (!pReqUser->IsFriendList(stDelete.dwFriendID, 1u)) {
        PS_RES_FRIEND_DELETE stRes{};
        stRes.dwReqID = stDelete.dwReqID;
        stRes.dwFriendID = stDelete.dwFriendID;
        stRes.byUsePopup = 1;
        stRes.nResult = 55109;  // 不在好友列表中
        XSendPacket xSendPacket(0xF5u, 5u);
        xSendPacket.XParse << pReqUser->GetMatchingID();
        xSendPacket << stRes;
        pReqUser->SendPacket(xSendPacket);
        return;
    }

    // 检查目标用户是否在线，以及请求者是否在目标好友列表中
    const std::shared_ptr<CUserObject> pTarget = GetUser(stDelete.dwFriendID);
    if (pTarget) {
        if (!pTarget->IsFriendList(stDelete.dwReqID, 1u)) {
            PS_RES_FRIEND_DELETE stRes{};
            stRes.dwReqID = stDelete.dwReqID;
            stRes.dwFriendID = stDelete.dwFriendID;
            stRes.byUsePopup = 0;
            stRes.nResult = 55109;
            XSendPacket xSendPacket(0xF5u, 5u);
            xSendPacket.XParse << pReqUser->GetMatchingID();
            xSendPacket << stRes;
            pReqUser->SendPacket(xSendPacket);
            return;
        }
    }

    // 发送删除请求到 DB (main=5, sub=4)
    XSendDBPacket xSendDBPacket(0, 5u, 4u);
    xSendDBPacket << stDelete;
    SendDBGame(xSendDBPacket);
}

void XRelayServer::PrepareBlockListAdd(const PS_REQ_FRIEND_BLOCK_ADD& stBlock) {
    // 对齐 IDA 0x1400B77B0: 黑名单添加处理
    CFAutoSlimReadLock autolock(&m_rwLock);

    const std::shared_ptr<CUserObject> pReqUser = GetUser(stBlock.dwReqUCID);
    if (!pReqUser) {
        KickOutUser(stBlock.dwReqUCID, 0xCu);
        LogHelper::LogError("game.relay", "<KICKOUT> XRelayServer::PrepareBlockListAdd");
        return;
    }

    // 检查黑名单添加条件
    int nResult = 0;
    if (!pReqUser->CheckBlockAdd(stBlock.strTargetName, &nResult)) {
        PS_RES_BLOCKLIST_ADD psBlockRes{};
        psBlockRes.dwReqUAID = pReqUser->GetUAID();
        psBlockRes.stResult.nResult = nResult;
        wcscpy_s(psBlockRes.stResult.strName, stBlock.strTargetName);
        XSendPacket xSendPacket(0xF5u, 7u);
        xSendPacket.XParse << pReqUser->GetMatchingID();
        xSendPacket << psBlockRes;
        pReqUser->SendPacket(xSendPacket);
        return;
    }

    // 发送黑名单添加到 DB (main=5, sub=6)
    PS_REQ_FRIEND_DELETE stDelete{};
    if (nResult == 55101) {
        // 目标在好友列表中，需要同时删除好友
        stDelete.dwReqID = pReqUser->GetMatchingID();
        stDelete.dwFriendID = pReqUser->GetFriendUCID(stBlock.strTargetName);
    }

    XSendDBPacket xSendDBPacket(0, 5u, 6u);
    xSendDBPacket << stBlock;
    xSendDBPacket << stDelete;
    SendDBGame(xSendDBPacket);
}

void XRelayServer::PrepareBlockListDel(const PS_REQ_FRIEND_BLOCK_DELETE& stBlock) {
    // 对齐 IDA 0x1400B7E90: 黑名单删除处理
    const std::shared_ptr<CUserObject> pReqUser = GetUser(stBlock.dwReqUCID);
    if (!pReqUser) {
        KickOutUser(stBlock.dwReqUCID, 0xCu);
        LogHelper::LogError("game.relay", "<KICKOUT> XRelayServer::PrepareBlockListDel");
        return;
    }

    // 检查目标是否在黑名单中
    if (!pReqUser->IsBlockList(stBlock.strTargetName)) {
        // 目标不在黑名单中
        PS_RES_BLOCKLIST_DELETE psBlockRes{};
        psBlockRes.nResult = 55109;
        psBlockRes.dwReqUAID = stBlock.dwReqUCID;
        wcscpy_s(psBlockRes.strTargetName, stBlock.strTargetName);
        XSendPacket xSendPacket(0xF5u, 8u);
        xSendPacket.XParse << pReqUser->GetMatchingID();
        xSendPacket << psBlockRes;
        pReqUser->SendPacket(xSendPacket);
        return;
    }

    // 发送黑名单删除到 DB (main=5, sub=7)
    XSendDBPacket xSendDBPacket(0, 5u, 7u);
    xSendDBPacket << stBlock;
    SendDBGame(xSendDBPacket);
}

void XRelayServer::RecommandFriend(const PS_RES_FRIEND_RECOMMAND& stRecommand) {
    // 对齐 IDA 0x1400B9AA0: 好友推荐处理
    CFAutoSlimReadLock autolock(&m_rwLock);

    const std::shared_ptr<CUserObject> pReqUser = GetUser(stRecommand.dwReqUCID);
    if (!pReqUser) {
        KickOutUser(stRecommand.dwReqUCID, 0xCu);
        LogHelper::LogError("game.relay", "<KICKOUT> XRelayServer::RecommandFriend");
        return;
    }

    // 对齐 IDA: 获取推荐好友列表
    PS_RES_FRIEND_RECOMMAND psRecommand{};
    psRecommand.dwReqUCID = stRecommand.dwReqUCID;
    m_RecommandManager.GetFriendRecommandList(pReqUser, 3, psRecommand.vecFriends);

    // 对齐 IDA: 记录每个推荐好友的日志
    for (const auto& friendInfo : psRecommand.vecFriends) {
        LogHelper::LogDebug("game.relay",
                            "<RECOMMAND_FRIEND> ( %u / %u )",
                            static_cast<unsigned int>(friendInfo.dwID),
                            static_cast<unsigned int>(friendInfo.byLevel));
    }

    // 发送推荐列表给客户端
    XSendPacket xSendPacket(0xF5u, 0x11u);
    xSendPacket.XParse << pReqUser->GetMatchingID();
    xSendPacket << psRecommand;
    pReqUser->SendPacket(xSendPacket);
}

void XRelayServer::UpdateFriendCommunity(std::uint32_t dwActorID, const ST_CHAR_COMMUNITY& stCommunity) {
    // 对齐 IDA 0x1400B3CF0: 更新好友社区状态
    const std::shared_ptr<CUserObject> pUser = GetUser(dwActorID);
    if (!pUser) {
        KickOutUser(dwActorID, 0xCu);
        LogHelper::LogError("game.relay", "<KICKOUT> XRelayServer::UpdateFriendCommunity");
        return;
    }

    ST_FRIEND_COMMUNITY stFriendCommunity{};
    stFriendCommunity.byState = stCommunity.byState;
#ifdef _WIN32
    wcscpy_s(stFriendCommunity.strMemo, stCommunity.szMemo);
#else
    std::wcsncpy(stFriendCommunity.strMemo, stCommunity.szMemo, 30);
    stFriendCommunity.strMemo[30] = L'\0';
#endif

    pUser->UpdateCharCommunity(&stFriendCommunity);

    // 对齐 IDA: DoJob(2, lambda) 通知好友社区状态变更
    CLogicThreadManager::Instance().DoJob(2, [dwActorID, stCommunity, this]() {
        // 对齐 IDA: 获取用户并发送社区状态更新给在线好友
        const std::shared_ptr<CUserObject> pUser = GetUser(dwActorID);
        if (!pUser) return;

        // 对齐 IDA: 获取用户信息用于发送
        ST_FRIEND_INFO stMyInfo{};
        pUser->GetUserInfo(&stMyInfo);
        stMyInfo.byState = stCommunity.byState;
#ifdef _WIN32
        wcscpy_s(stMyInfo.strMemo, stCommunity.szMemo);
#else
        std::wcsncpy(stMyInfo.strMemo, stCommunity.szMemo, 30);
        stMyInfo.strMemo[30] = L'\0';
#endif

        // 对齐 IDA: 遍历好友列表，通知在线好友
        PS_FRIEND_LIST stFriendList;
        pUser->GetFriendList(stFriendList, 0);  // 0 = 所有类型
        for (const auto& friendInfo : stFriendList.vecFriends) {
            const std::shared_ptr<CUserObject> pFriend = GetUser(friendInfo.dwID);
            if (pFriend && pFriend->GetLoadFriendList()) {
                // 发送好友状态更新包 (main=0xF5, sub=0x20)
                XSendPacket xSendPacket(0xF5, 0x20);
                xSendPacket.XParse << pFriend->GetMatchingID();
                xSendPacket << stMyInfo;
                pFriend->SendPacket(xSendPacket);
            }
        }

        LogHelper::LogDebug("game.relay",
                            "<FRIEND_COMMUNITY_UPDATE> actorID=%u state=%u friendCount=%u",
                            static_cast<unsigned int>(dwActorID),
                            static_cast<unsigned int>(stCommunity.byState),
                            static_cast<unsigned int>(stFriendList.vecFriends.size()));
    });
}

void XRelayServer::ReqFriendFind(const PS_REQ_FRIEND_FIND& stFind) {
    // 对齐 IDA 0x1400B9440: 向 DB 发送好友查找请求
    IXObject* pObject = nullptr;
    XSendDBPacket xSendDBPacket(pObject, 5u, 0x11u);
    xSendDBPacket << stFind;
    SendDBGame(xSendDBPacket);
}

void XRelayServer::DailyMissionFriendReq(const PS_DAILY_MISSION_FRIEND_REQ& psMission) {
    // 对齐 IDA 0x1400BB1E0: 向目标好友发送每日任务请求
    CFAutoSlimReadLock autolock(&m_rwLock);

    const std::shared_ptr<CUserObject> pReqUser = GetUser(psMission.dwReqID);
    if (!pReqUser) {
        KickOutUser(psMission.dwReqID, 0xCu);
        LogHelper::LogError("game.contents", "<KICKOUT> XRelayServer::DailyMissionFriendReq");
        return;
    }

    const std::shared_ptr<CUserObject> pTargetUser = GetUser(psMission.dwTargetID);
    if (pTargetUser) {
        XSendPacket sendPacket(0xF5u, 0x25u);
        sendPacket.XParse << pTargetUser->GetMatchingID();
        sendPacket << psMission;
        pTargetUser->SendPacket(sendPacket);
    }
}

void XRelayServer::DailyMissionFriendRes(const PS_DAILY_MISSION_FRIEND_RES& psMission) {
    // 对齐 IDA 0x1400BB3E0: 向请求者回复每日任务响应
    CFAutoSlimReadLock autolock(&m_rwLock);

    const std::shared_ptr<CUserObject> pReqUser = GetUser(psMission.dwReqID);
    if (pReqUser) {
        XSendPacket sendPacket(0xF5u, 0x26u);
        sendPacket.XParse << pReqUser->GetMatchingID();
        sendPacket << psMission;
        pReqUser->SendPacket(sendPacket);
    } else {
        KickOutUser(psMission.dwReqID, 0xCu);
        LogHelper::LogError("game.contents", "<KICKOUT> XRelayServer::DailyMissionFriendRes");
    }
}

// 对齐 IDA 0x1400BBC10: 助战信息查询
void XRelayServer::HelperSupportInfo(std::uint32_t dwUCID) {
    CFAutoSlimReadLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> pUser = GetUser(dwUCID);
    if (!pUser) {
        KickOutUser(dwUCID, 0xCu);
        LogHelper::LogError("game.contents", "<KICKOUT> XRelayServer::HelperSupportInfo");
        return;
    }

    // 对齐 IDA: CHelperSupportMgr::FindSupport + CheckVaildTime + GetRewardState + GetSupportInfo
    PS_HELPER_SUPPORT_INFO_RES psInfo{};
    const std::shared_ptr<CHelperSupport> pSupport = m_HelperSupportMgr.FindSupport(dwUCID);
    if (pSupport && pSupport->CheckVaildTime()) {
        psInfo.bRegister = 1;
        psInfo.byRewardType = pSupport->GetRewardState();
        pSupport->GetSupportInfo(psInfo.stInfo);
    } else {
        psInfo.bRegister = 0;
        psInfo.byRewardType = 0;
        if (pSupport) {
            m_HelperSupportMgr.DeleteSupport(dwUCID);
        }
    }

    XSendPacket xSendPacket(0xF5u, 0x27u);
    xSendPacket.XParse << pUser->GetMatchingID();
    xSendPacket << psInfo;
    pUser->SendPacket(xSendPacket);
}

// 对齐 IDA 0x1400BBE90: 助战注册
void XRelayServer::HelperSupportRegister(const PS_SERVER_HELPER_SUPPORT_REGISTER& psSupport) {
    CFAutoSlimReadLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> pUser = GetUser(psSupport.stInfo.dwFriendUCID);
    if (!pUser) {
        KickOutUser(psSupport.stInfo.dwFriendUCID, 0xCu);
        LogHelper::LogError("game.contents", "<KICKOUT> XRelayServer::HelperSupportRegister");
        return;
    }

    // 对齐 IDA: CHelperSupportMgr::AddSupport + GetSupportReward
    PS_SERVER_HELPER_SUPPORT_REGISTER psRes = psSupport;
    psRes.nResult = 0;

    if (!m_HelperSupportMgr.AddSupport(psRes)) {
        if (m_HelperSupportMgr.GetSupportReward(psSupport.stInfo.dwFriendUCID) != 1) {
            return;
        }
        psRes.nResult = 58003;  // 已注册助战
    }

    XSendPacket xSendPacket(0xF5u, 0x28u);
    xSendPacket.XParse << pUser->GetMatchingID();
    xSendPacket << psRes;
    pUser->SendPacket(xSendPacket);
}

// 对齐 IDA 0x1400BC0D0: 助战奖励领取
void XRelayServer::HelperSupportReward(const PS_SERVER_HELPER_SUPPORT_REWARD& psReward) {
    CFAutoSlimReadLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> pUser = GetUser(psReward.dwUCID);
    if (!pUser) {
        KickOutUser(psReward.dwUCID, 0xCu);
        LogHelper::LogError("game.contents", "<KICKOUT> XRelayServer::HelperSupportReward");
        return;
    }

    // 对齐 IDA: CHelperSupportMgr::FindSupport + GetRewardState + SetMatchingState
    PS_SERVER_HELPER_SUPPORT_REWARD psRes = psReward;
    const std::shared_ptr<CHelperSupport> pHelper = m_HelperSupportMgr.FindSupport(psReward.dwUCID);
    if (!pHelper) {
        psRes.nResult = 2;  // 未注册助战
    } else if (pHelper->GetRewardState() == 1) {
        pHelper->SetMatchingState(2);  // 已领取
        psRes.nResult = 0;  // 成功
    } else {
        psRes.nResult = 1;  // 不可领取
    }

    XSendPacket xSendPacket(0xF5u, 0x29u);
    xSendPacket.XParse << pUser->GetMatchingID();
    xSendPacket << psRes;
    pUser->SendPacket(xSendPacket);
}

// 对齐 IDA 0x1400BC320: 助战列表查询
void XRelayServer::HelperSupportList(std::uint32_t dwUCID) {
    CFAutoSlimReadLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> pUser = GetUser(dwUCID);
    if (!pUser) {
        KickOutUser(dwUCID, 0xCu);
        LogHelper::LogError("game.contents", "<KICKOUT> XRelayServer::HelperSupportList");
        return;
    }

    // 对齐 IDA: GetFriendList + FindSupport + CheckReceived + GetSupportInfo
    PS_FRIEND_LIST stFriendList;
    pUser->GetFriendList(stFriendList, 1u);  // type=1 好友列表

    PS_HELPER_SUPPORT_LIST_RES psListRes{};
    int nCount = 0;

    // 对齐 IDA: 遍历好友列表，查找助战信息
    for (size_t i = 0; i < stFriendList.vecFriends.size() && nCount < 3; ++i) {
        const ST_FRIEND_INFO& stFriend = stFriendList.vecFriends[i];

        const std::shared_ptr<CHelperSupport> pSupport = m_HelperSupportMgr.FindSupport(stFriend.dwID);
        if (!pSupport) continue;
        if (pSupport->CheckReceived(dwUCID)) continue;  // 已领取过

        // 对齐 IDA: 构造助战条目
        ST_HELPER_SUPPORT_ENTRY& entry = psListRes.stEntries[nCount];
        pSupport->GetSupportInfo(entry.stInfo);
        entry.byRewardState = pSupport->GetRewardState();

#ifdef _WIN32
        wcscpy_s(entry.strName, stFriend.strName);
#else
        std::wcsncpy(entry.strName, stFriend.strName, 20);
        entry.strName[20] = L'\0';
#endif
        entry.byLevel = stFriend.byLevel;
        entry.byClass = stFriend.byClass;
        entry.byAwaken = stFriend.byAwaken;

        // 对齐 IDA: 等级缩放，好友等级比用户高超过 5 级时，按比例降低属性值
        float fOriVal = entry.stInfo.fVal;
        if (entry.byLevel > pUser->GetLevel() + 5) {
            entry.stInfo.fVal = fOriVal / static_cast<float>(entry.byLevel) * static_cast<float>(pUser->GetLevel());
        }

        ++nCount;
    }
    psListRes.byCount = static_cast<std::uint8_t>(nCount);

    XSendPacket xSendPacket(0xF5u, 0x30u);
    xSendPacket.XParse << pUser->GetMatchingID();
    xSendPacket << psListRes;
    pUser->SendPacket(xSendPacket);
}

// 对齐 IDA 0x1400BC7F0: 助战装备请求
void XRelayServer::HelperSupportEquip(const PS_HELPER_SUPPORT_EQUIP_REQ& psEquip) {
    CFAutoSlimReadLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> pUser = GetUser(psEquip.dwUCID);
    if (!pUser) {
        KickOutUser(psEquip.dwUCID, 0xCu);
        LogHelper::LogError("game.contents", "<KICKOUT> XRelayServer::HelperSupportEquip");
        return;
    }

    // 对齐 IDA: CHelperSupportMgr::FindSupport
    const std::shared_ptr<CHelperSupport> pSupport = m_HelperSupportMgr.FindSupport(psEquip.dwFriendUCID);
    if (!pSupport) {
        PS_HELPER_SUPPORT_EQUIP_RES psRes{};
        psRes.nResult = 2;  // 助战不存在

        XSendPacket xSendPacket(0xF5u, 0x31u);
        xSendPacket.XParse << pUser->GetMatchingID();
        xSendPacket << psRes;
        pUser->SendPacket(xSendPacket);
        return;
    }

    // 对齐 IDA: 已领取检查
    if (pSupport->CheckReceived(psEquip.dwUCID)) {
        return;  // 已领取，不发送响应
    }

    // 对齐 IDA: 获取当前时间
    const std::int64_t tCurrent = GreenDamTan_GetCurDateSec();

    // 对齐 IDA: 构造 DB 请求
    PS_DB_HELPER_SUPPORT_EQUIP psDB{};
    psDB.dwUCID = psEquip.dwUCID;
    psDB.dwFriendUCID = psEquip.dwFriendUCID;
    psDB.dwReserved = psEquip.dwReserved;
    pSupport->GetSupportInfo(psDB.stSupport);

    // 对齐 IDA: 有效时间检查 - nDate < currentTime 表示过期
    if (psDB.stSupport.nDate < tCurrent) {
        m_HelperSupportMgr.DeleteSupport(psEquip.dwFriendUCID);

        PS_HELPER_SUPPORT_EQUIP_RES psRes{};
        psRes.nResult = 1;  // 已过期

        XSendPacket xSendPacket(0xF5u, 0x31u);
        xSendPacket.XParse << pUser->GetMatchingID();
        xSendPacket << psRes;
        pUser->SendPacket(xSendPacket);
        return;
    }

    // 对齐 IDA: 等级缩放
    std::uint8_t byFriendLevel = pUser->GetFriendLevel(psEquip.dwFriendUCID);
    if (byFriendLevel == 0) {
        byFriendLevel = pUser->GetLevel();
    }
    if (byFriendLevel > pUser->GetLevel() + 5) {
        psDB.stSupport.fVal = psDB.stSupport.fVal / static_cast<float>(byFriendLevel) * static_cast<float>(pUser->GetLevel());
    }

    // 对齐 IDA: 查询奖励表
    TB_HELPER_REWARD* pTBHelperReward = resourceMgr_.GetTB_HELPER_REWARD(2u);
    if (!pTBHelperReward) {
        return;  // 奖励表不存在
    }
    psDB.wFriendPointReward = pTBHelperReward->Reward_FriendPoint_Value;

    // 对齐 IDA: 发送 DB 请求 (main=0x26, sub=3)
    XSendDBPacket xSendDBPacket(0, 0x26u, 3u);
    xSendDBPacket << psDB;
    SendDBGame(xSendDBPacket);
}

void XRelayServer::ResExchangePriceList(const PS_DB_EXCHANGE_PRICE_HISTORY_RES& psHistory) {
    // 对齐 IDA 0x1400BB770: GameDBSocket 解析 PS_DB_EXCHANGE_PRICE_HISTORY_RES 后回调
    // 加载价格列表到缓存
    m_ExchangePriceMgr.LoadPriceList(&psHistory);

    // 查找请求用户
    const std::shared_ptr<CUserObject> pUser = GetUser(psHistory.stRes.dwUCID);
    if (!pUser) {
        LogHelper::LogError("game.contents", "<Find Fail> XRelayServer::ResExchangePriceList [%u]",
                           psHistory.stRes.dwUCID);
        return;
    }

    // 构造响应
    PS_EXCHANGE_PRICE_HISTORY_RES psResult{};
    psResult.dwUCID = psHistory.stRes.dwUCID;
    psResult.dwItemID = psHistory.stRes.dwItemID;
    m_ExchangePriceMgr.GetPriceList(psResult.dwItemID, &psResult);

    // 发送给客户端 (main=0xF3, sub=0x28)
    XSendPacket xSendPacket(0xF3u, 0x28u);
    xSendPacket.XParse << pUser->GetMatchingID();
    xSendPacket << psResult;
    pUser->SendPacket(xSendPacket);
}

// 对齐 IDA 0x1400BCDC0: GameDBSocket 解析 PS_DB_HELPER_SUPPORT_EQUIP 后回调
void XRelayServer::ResHelperSupportEquipDB(const PS_DB_HELPER_SUPPORT_EQUIP& psEquip) {
    // 对齐 IDA: nResult != 0 时直接返回
    if (psEquip.nResult != 0) {
        return;
    }

    // 对齐 IDA: 第一步 - 通知请求用户
    {
        CFAutoSlimReadLock autolock(&m_rwLock);
        const std::shared_ptr<CUserObject> pUser = GetUser(psEquip.dwUCID);
        if (pUser) {
            // 对齐 IDA: CHelperSupportMgr::AddSupportReceived
            m_HelperSupportMgr.AddSupportReceived(psEquip.dwFriendUCID, psEquip.dwUCID);

            // 对齐 IDA: 构造并发送装备响应 (main=0xF5, sub=0x31)
            PS_HELPER_SUPPORT_EQUIP_RES psRes{};
            psRes.nResult = psEquip.nResult;
            psRes.stInfo = psEquip.stSupport;
            psRes.wFriendPointReward = psEquip.wFriendPointReward;

            XSendPacket xSendPacket(0xF5u, 0x31u);
            xSendPacket.XParse << pUser->GetMatchingID();
            xSendPacket << psRes;
            pUser->SendPacket(xSendPacket);
        }
    }

    // 对齐 IDA: 第二步 - 通知助战好友（获得友情点）
    {
        CFAutoSlimReadLock autolock(&m_rwLock);
        const std::shared_ptr<CUserObject> pFriendUser = GetUser(psEquip.dwFriendUCID);
        if (pFriendUser) {
            // 对齐 IDA: CUserObject::AddFriendPoint
            const std::int64_t nFriendPoint = pFriendUser->AddFriendPoint(psEquip.dwUCID, psEquip.wFriendPointReward);
            if (nFriendPoint == -1) {
                return;  // 添加失败
            }

            // 对齐 IDA: 构造并发送友情点奖励通知 (main=0xF5, sub=0x32)
            // 使用简单结构: dwFriendUCID + wFriendPointReward
            XSendPacket xPacket(0xF5u, 0x32u);
            xPacket.XParse << pFriendUser->GetMatchingID();
            xPacket.XParse << psEquip.dwUCID;  // dwFriendUCID - 谁使用了助战
            xPacket.XParse << psEquip.wFriendPointReward;
            pFriendUser->SendPacket(xPacket);
        }
    }
}

void XRelayServer::SendRecruitList(CServer* pServer, const PS_REQ_RECRUIT_LIST& stList) {
    // 对齐 IDA 0x140040970: PS_REQ_RECRUIT_LIST>> + DoJob(2, lambda)
    // 对齐 IDA: 发送招募列表请求到 DB
    XSendDBPacket xSendDBPacket(pServer, 0x26u, 0x0Bu);
    xSendDBPacket << stList;
    SendDBGame(xSendDBPacket);
}

void XRelayServer::SendRecruitAdd(const ST_RECRUIT_INFO& stAdd) {
    // 对齐 IDA 0x140040AD0: PS_RECRUIT_ADD>> + DoJob(2, lambda)
    // 对齐 IDA: 发送招募添加请求到 DB
    XSendDBPacket xSendDBPacket(nullptr, 0x26u, 0x0Cu);
    xSendDBPacket << stAdd;
    SendDBGame(xSendDBPacket);
}

void XRelayServer::PrepareRecruitInfo(std::uint32_t dwUCID) {
    // 对齐 IDA 0x140040CC0: dwUCID>> + DoJob(2, lambda) → PrepareRecruitInfo
    // 对齐 IDA: 发送招募信息请求到 DB
    XSendDBPacket xSendDBPacket(nullptr, 0x26u, 0x0Du);
    xSendDBPacket.XParse << dwUCID;
    SendDBGame(xSendDBPacket);
}

void XRelayServer::SendChatNotice(const PS_CHAT_NOTICE& stChatNotice) {
    // 对齐 IDA 0x1400BA3C0: XSendPacket(0xF3,0x11) + SendPacketAll
    XSendPacket sendPacket(0xF3u, 0x11u);
    sendPacket << stChatNotice;
    SendPacketAll(sendPacket);
}

void XRelayServer::SendChatWhisper(std::uint32_t dwActorID,
                                   const PS_CHAT_WHISPER& stChatWhisper,
                                   const PS_CHAT_ITEM_LINK_FOR_SERVER& psItemLinkInfo) {
    // 对齐 IDA 0x1400B9DF0: 私聊消息处理
    CFAutoSlimReadLock autolock(&m_rwLock);

    // 按名称查找目标用户
    const std::shared_ptr<CUserObject> pTargetUser = GetUser(stChatWhisper.strReciver);

    if (pTargetUser) {
        // 目标在线
        PS_CHAT_WHISPER stSenderReply = stChatWhisper;
        std::uint8_t byResult = 0;

        // 检查目标的私聊权限设置
        if (pTargetUser->CheckGameOption(eOption_WhisperMsg, eGAME_OPTION_ALLOW_PARTIAL)) {
            // 只允许好友私聊
            if (pTargetUser->GetFriendUCID(stChatWhisper.strSender) == 0) {
                stSenderReply.nResult = 59201;  // 拒绝非好友
            }
        } else if (pTargetUser->CheckGameOption(eOption_WhisperMsg, eGAME_OPTION_REFUSE_ALL)) {
            stSenderReply.nResult = 59201;  // 拒绝所有私聊
        }

        // 查找发送者
        const std::shared_ptr<CUserObject> pSenderUser = GetUser(dwActorID);
        std::uint32_t dwSenderUAID = 0;
        if (pSenderUser) {
            dwSenderUAID = pSenderUser->GetUAID();
            // 发送回执给发送者
            XSendPacket xPacket(0xF3u, 0x10u);
            xPacket.XParse << pSenderUser->GetMatchingID();
            xPacket << stSenderReply;
            xPacket.XParse << byResult;
            xPacket << psItemLinkInfo;
            pSenderUser->SendPacket(xPacket);
        }

        // 如果发送者的私聊被拒绝，不继续处理
        if (stSenderReply.nResult != 0) {
            return;
        }

        // 发送给目标用户
        PS_CHAT_WHISPER stTargetReply = stChatWhisper;
        stTargetReply.nResult = 0;
        XSendPacket xSendPacket(0xF3u, 0x10u);
        xSendPacket.XParse << pTargetUser->GetMatchingID();
        xSendPacket << stTargetReply;
        xSendPacket.XParse << static_cast<std::uint8_t>(0);
        xSendPacket << psItemLinkInfo;
        pTargetUser->SendPacket(xSendPacket);

        // 对齐 IDA: 发送聊天日志到 DB
        SendDBChatLog(dwSenderUAID, dwActorID, 5,
                      pTargetUser->GetUAID(), pTargetUser->GetMatchingID(),
                      0, 0, 0, 0, 0, stChatWhisper.strMsg);
    } else {
        // 目标不在线，通知发送者
        const std::shared_ptr<CUserObject> pSenderUser = GetUser(dwActorID);
        if (pSenderUser) {
            std::uint8_t byResult = 1;  // 目标不在线
            XSendPacket xPacket(0xF3u, 0x10u);
            xPacket.XParse << pSenderUser->GetMatchingID();
            xPacket << stChatWhisper;
            xPacket.XParse << byResult;
            xPacket << psItemLinkInfo;
            pSenderUser->SendPacket(xPacket);
        } else {
            LogHelper::LogError("game.relay",
                                "<Find Fail> XRelayServer::SendChatWhisper [%u]",
                                dwActorID);
        }
    }
}

void XRelayServer::SendChatMegaPhone(const PS_CHAT_MEGAPHONE& stMegaPhone,
                                     const PS_CHAT_ITEM_LINK_FOR_SERVER& psItemLinkInfo) {
    // 对齐 IDA 0x1400BA450: XSendPacket(0xF3,0x17) + SendPacketAll
    XSendPacket sendPacket(0xF3u, 0x17u);
    sendPacket << stMegaPhone;
    sendPacket << psItemLinkInfo;
    SendPacketAll(sendPacket);
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
        // 对齐 IDA 0x1400bdb94: SendPacketAll 广播缓存加载状态
        SendPacketAll(sendPacket);
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

void XRelayServer::SendDBChatLog(int uaid,
                                  int ucid,
                                  std::int16_t sType,
                                  int param0,
                                  int param1,
                                  int param2,
                                  int param3,
                                  int param4,
                                  std::int64_t param5,
                                  std::int64_t param6,
                                  const wchar_t* comment) {
    // 对齐 IDA 0x1400BAD10: 发送聊天日志到 DB
    ST_CHAT_LOG_GAME stLog{};
    stLog.nUAID = uaid;
    stLog.nUCID = ucid;
    stLog.sType = sType;
    stLog.nParam0 = param0;
    stLog.nParam1 = param1;
    stLog.nParam2 = param2;
    stLog.nParam3 = param3;
    stLog.nParam4 = param4;
    stLog.nParam5 = param5;
    stLog.nParam6 = param6;
    if (comment) {
        wcscpy_s(stLog.szComment, comment);
    }

    XSendDBPacket sendPacket(nullptr, 0x42u, 9u);
    sendPacket << stLog;
    SendDBGame(sendPacket);
}

bool XRelayServer::LoadForceDataReq() {
    XSendDBPacket sendPacket(nullptr, 8u, 0x0Bu);
    return SendDBGame(sendPacket);
}

CServer* XRelayServer::GetServer(std::uint32_t dwServerID) {
    CFAutoSlimReadLock autolock(&m_rwServerLock);
    const auto it = m_mapGameServer.find(dwServerID);
    return it == m_mapGameServer.end() ? nullptr : it->second;
}

void XRelayServer::UpdateServerState() {
    // 对齐 IDA 0x1400BD5C0 XRelayServer::UpdateServerState
    // 遍历所有游戏服务器，发送状态更新包
    CFAutoSlimReadLock autolock(&m_rwServerLock);
    for (const auto& [serverID, pServer] : m_mapGameServer) {
        static_cast<void>(serverID);
        if (!pServer) {
            continue;
        }

        SS_UPDATE_SERVER_INFO stUpdateInfo{};
        stUpdateInfo.dwID = 0;
        // 对齐 IDA: IsRecvServerInfo && Isload → nState=2 (就绪), 否则 nState=1
        if (pServer->IsRecvServerInfo() && m_partyManager.Isload()) {
            stUpdateInfo.nState = 2;
        } else {
            stUpdateInfo.nState = 1;
        }

        XSendPacket xSendPacket(0xF2u, 3u);
        xSendPacket << stUpdateInfo;
        pServer->SendEx(xSendPacket);
    }
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

std::int64_t XRelayServer::GetCurDateSec() const {
    return GreenDamTan_GetCurDateSec();
}

void XRelayServer::SetName() {
    std::snprintf(m_szName, sizeof(m_szName), "%s", "RELAY");
}

bool XRelayServer::InitServer() {
    // 对齐 IDA 0x1400B05A0 XRelayServer::InitServer

    // 对齐 IDA: CLogThreadManager::Start(GetName())
    TXSingleton<CLogThreadManager>::Instance()->Start(GetName());

    m_xOption.ShowServerInfo();

    // 对齐 IDA: m_dwCachingLoad = 0
    m_dwCachingLoad = 0;

    // 对齐 IDA: XSeed::Init(&m_xSeed, 1) - 随机种子初始化
    GreenDamTan_XSeedInit(1);

    // 对齐 IDA: m_bRegisterAuth = 0
    m_bRegisterAuth = false;

    // 对齐 IDA: memset(&m_stServerGroupInfo, 0, ...)
    std::memset(&m_stServerGroupInfo, 0, sizeof(m_stServerGroupInfo));

    // 初始化资源管理器 - 对齐 IDA 0x1400b05a0
    const char* commonDNS = m_xOption.GetDNS(2);
    const char* gameDNS = m_xOption.GetDNS(1);
    std::uint32_t dwServerID = m_xOption.GetServerID();

    if (!resourceMgr_.Init(commonDNS, gameDNS, dwServerID)) {
        LogHelper::LogError("game.relay", "Error ResourceMgr Init fail");
        return false;
    }

    const char* szResFilePath = m_xOption.GetResFilePath();
    RES_LOAD_TYPE eResLoadType = m_xOption.GetResLoadType();

    if (!resourceMgr_.Load(eResLoadType, szResFilePath, 0, 0)) {
        LogHelper::LogError("game.relay", "Error Table Load fail");
        return false;
    }
    LogHelper::LogInfo("game.system", "[INIT] ResourceMgr - Load Complete!");

    // 设置服务器内容选项 - 对齐 IDA 0x1400b0760
    if (m_xOption.GetContentsOption()->nOptionFlag == 2) {
        for (int i = E_SERVER_OPTION_ATTENDANCE; i < E_SERVER_OPTION_MAX; ++i) {
            resourceMgr_.SetServerContents(i, m_xOption.GetContentsOption()->bContents[i]);
        }
    }

    // 以下三个 Init 是 RelayServer 特有的本地表加载（IDA 中无，是重构新增的辅助模块）
    if (!m_PartyMatchingConfig.Init(commonDNS)) {
        LogHelper::LogError("game.relay", "Error PartyMatchingConfig Init fail");
        return false;
    }

    if (!m_MazeOpenControl.Init(commonDNS)) {
        LogHelper::LogError("game.relay", "Error MazeOpenControl Init fail");
        return false;
    }

    if (!m_DistrictControl.Init(commonDNS)) {
        LogHelper::LogError("game.relay", "Error DistrictControl Init fail");
        return false;
    }

    // IDA: XGameDBSocketMgr::Init + AutoConnect
    m_xDBAgentMgr.Init();
    m_xDBAgentMgr.AutoConnect();
    LogHelper::LogInfo("game.system", "[INIT] DBAgentMgr - Init");

    // IDA: SetMyInfo 在 Init 之前
    m_scControlSocket.SetMyInfo(&m_xOption);
    if (!m_scControlSocket.Init(ePoolIDRelayServer, "127.0.0.1", 5001)) {
        LogHelper::LogError("game.relay", "Error Community Socket Init fail");
        return false;
    }

    if (!m_scControlSocket.Connect()) {
        LogHelper::LogError("game.relay", "Failed conect Community Server!!");
        return false;
    }

    LogHelper::LogInfo("game.system", "[INIT] ControlSocket - Init");

    // 对齐 IDA: CObserveSocket::StartUp
    if (!m_scObserveSocket.StartUp(&m_xOption)) {
        LogHelper::LogError("game.relay", "Error ObserveSocket StartUp fail");
        return false;
    }

    // IDA: srand(time(nullptr))
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // IDA: CLogicThreadManager::Start(3) 在末尾
    if (!CLogicThreadManager::Instance().Start(3)) {
        LogHelper::LogError("game.relay", "Error LogicThreadManager Start fail");
        return false;
    }

    LogHelper::LogInfo("game.system", "[INIT] Complete Server Init");
    return true;
}

bool XRelayServer::Clear(std::uint32_t maxWait) {
    // 对齐 IDA 0x1400B0950 XRelayServer::Clear
    // IDA: 使用 m_rwLock 写锁保护整个清理过程
    CFAutoSlimWriteLock autolock(&m_rwLock);
    if (m_bClose) {
        return false;
    }

    // IDA: CLogicThreadManager::End
    CLogicThreadManager::Instance().End();

    // 对齐 IDA: CLogThreadManager::End
    TXSingleton<CLogThreadManager>::Instance()->End();

    // IDA: XIOCPClient::DisConnect (非 Shutdown)
    m_scControlSocket.XIOCPClient::DisConnect();

    // IDA: m_mapGameServer.clear() 需要写锁
    {
        CFAutoSlimWriteLock serverLock(&m_rwServerLock);
        m_mapGameServer.clear();
    }

    // IDA: XResourceMgr::Clear
    resourceMgr_.Clear();

    // IDA: XGameDBSocketMgr::DisConnect
    m_xDBAgentMgr.DisConnect();

    m_bClose = true;
    return true;
}

void XRelayServer::OnUpdate(std::uint64_t currentTick) {
    // 对齐 IDA 0x1400B2D90 XRelayServer::OnUpdate
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

    // IDA: 使用静态变量管理首次初始化和周期计时
    static bool s_bInitControlTick = false;
    static bool s_bInitUpdateInfoTick = false;
    static bool s_bInitServerGroupSync = false;
    static std::uint64_t s_dwControlConnectTick = 0;
    static std::uint64_t s_dwUpdateServerInfoTick = 0;
    static std::uint64_t s_dwServerGroupSync = 0;

    if (!s_bInitControlTick) {
        s_bInitControlTick = true;
        s_dwControlConnectTick = currentTick;
    }
    if (!s_bInitUpdateInfoTick) {
        s_bInitUpdateInfoTick = true;
        s_dwUpdateServerInfoTick = currentTick;
    }
    if (!s_bInitServerGroupSync) {
        s_bInitServerGroupSync = true;
        s_dwServerGroupSync = currentTick + 60000;
    }

    if (!m_bClose) {
        const bool controlConnected = m_scControlSocket.XIOCPClient::IsConnection();
        if (controlConnected) {
            // 对齐 IDA: SendUpdateServerInfo(2, nUserCount)，原版参数为 (1,0) 是错误的
            if (s_dwUpdateServerInfoTick < currentTick) {
                int nUserCount = static_cast<int>(m_mapUserInfos.size());
                m_scControlSocket.SendUpdateServerInfo(2, nUserCount);
                s_dwUpdateServerInfoTick = currentTick + 10000;  // 对齐 IDA: 10秒间隔
            }
        } else if (s_dwControlConnectTick < currentTick) {
            LogHelper::LogInfo("game.relay", "OnUpdate [RelayControlSocket.Connect()]");
            m_scControlSocket.Connect();
            s_dwControlConnectTick = currentTick + 10000;  // 对齐 IDA: 10秒重连间隔（非30000）
        }

        // 对齐 IDA: CObserveSocket::OnUpdate 调用
        {
            XOption& option = m_xOption;
            int nMaxThreadCount = static_cast<int>(option.GetLogicThread());
            int nUserCount = static_cast<int>(m_mapUserInfos.size());
            int nPort = static_cast<int>(option.GetPort());
            const char* szIP = option.GetIP();
            m_scObserveSocket.OnUpdate(currentTick, szIP, nPort, nUserCount,
                                       controlConnected, false, nMaxThreadCount, false);
        }
    }

    // 对齐 IDA: 周期性 UpdateServerState (每5秒)
    if (s_dwServerGroupSync < currentTick) {
        UpdateServerState();
        s_dwServerGroupSync = currentTick + 5000;
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

// ============================================================================
// 对齐 IDA: 好友 DB 响应处理方法
// ============================================================================

void XRelayServer::SetCharCommunity(std::uint32_t dwActorID, const ST_CHAR_COMMUNITY* pCommunity) {
    // 对齐 IDA 0x1400B3990: 设置角色社区状态
    if (!pCommunity) return;

    const std::shared_ptr<CUserObject> pUser = GetUser(dwActorID);
    if (pUser) {
        pUser->SetCommunityState(pCommunity->byState);
        pUser->SetMemo(pCommunity->szMemo);
    }
}

// 对齐 IDA 0x1400B3770: 加载黑名单到内存
// 签名 Note: IDA 显示 PS_BLOCKLIST_INFO，实际 DB_BLOCKLIST_INFO 二进制兼容
bool XRelayServer::SetBlockLoad(std::uint32_t dwActorID, const DB_BLOCKLIST_INFO* pBlockList) {
    if (!pBlockList) return false;

    CFAutoSlimWriteLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> pUser = GetUser(dwActorID);
    if (!pUser) return false;

    // 对齐 IDA: 遍历黑名单条目，调用 LoadBlock
    for (const auto& blockInfo : pBlockList->vecBlockList) {
        pUser->LoadBlock(&blockInfo);
    }

    // 对齐 IDA: 设置黑名单加载完成标志
    pUser->SetLoadBlockList(true);

    // 对齐 IDA: 如果需要同步，发送黑名单给客户端
    if (pUser->GetSyncBlockList()) {
        pUser->SendBlockList();
    }

    return true;
}

void XRelayServer::SetFriendLoad(const PS_DB_FRIEND_LIST* pFriendList) {
    // 对齐 IDA 0x1400B3400: 加载好友列表到内存
    if (!pFriendList) return;

    CFAutoSlimWriteLock autolock(&m_rwLock);
    const std::shared_ptr<CUserObject> pUser = GetUser(pFriendList->dwActorID);
    if (!pUser) return;

    pUser->SetLoadFriend();

    // 对齐 IDA: 获取用户的 ST_FRIEND_INFO（用于通知在线好友）
    ST_FRIEND_INFO stMyInfo{};
    pUser->GetUserInfo(&stMyInfo);

    // 对齐 IDA: 遍历 DB 好友列表，加载每个好友信息
    for (const auto& stFriend : pFriendList->vecFriend) {
        // 对齐 IDA: 查找好友是否在线
        const std::shared_ptr<CUserObject> pFriendUser = GetUser(stFriend.dwUCID);
        std::shared_ptr<CUserObject> pFriendUserCopy;
        if (pFriendUser) {
            pFriendUserCopy = pFriendUser;
        }

        // 对齐 IDA: 加载好友到用户社区
        ST_FRIEND_INFO stFriendRes{};
        pUser->LoadFriend(&stFriend, pFriendUserCopy, &stFriendRes);

        // 对齐 IDA: 如果好友在线且类型不是推荐好友(2)，通知好友我已上线
        if (stFriend.byType != 2 && pFriendUserCopy) {
            pFriendUserCopy->LoginFriend(stMyInfo, pUser);
        }
    }

    pUser->SetLoadFriendList(true);

    if (pUser->GetSyncFriendList()) {
        pUser->SendFriendList();
    }
}

void XRelayServer::SendFriendServerLoad(std::uint32_t dwActorID) {
    // 对齐 IDA 0x1400B3310: 通知客户端好友列表加载完成
    const std::shared_ptr<CUserObject> pUser = GetUser(dwActorID);
    if (pUser) {
        pUser->SendFriendServerLoad();
    }
}

void XRelayServer::InviteFriend(const PS_RES_DB_FRIEND_INVITE* pInvite) {
    // 对齐 IDA 0x1400B4BA0: 好友邀请结果处理
    if (!pInvite) return;

    CFAutoSlimWriteLock autolock(&m_rwLock);

    // 处理删除请求方旧好友
    if (pInvite->stInvite.stDeleteReq.dwFriendID) {
        std::shared_ptr<CUserObject> pUser = GetUser(pInvite->stInvite.stDeleteReq.dwReqID);
        if (pUser) {
            pUser->DeleteFriend(pInvite->stInvite.stDeleteReq.dwFriendID);
            PS_RES_FRIEND_DELETE stRes{};
            stRes.dwReqID = pInvite->stInvite.stDeleteReq.dwReqID;
            stRes.dwFriendID = pInvite->stInvite.stDeleteReq.dwFriendID;
            stRes.byUsePopup = 0;
            stRes.nResult = 0;
            XSendPacket sendPacket(0xF5u, 5u);
            sendPacket.XParse << pUser->GetMatchingID();
            sendPacket << stRes;
            pUser->SendPacket(sendPacket);
        }
        // 从好友方删除
        std::shared_ptr<CUserObject> pFriendUser = GetUser(pInvite->stInvite.stDeleteReq.dwFriendID);
        if (pFriendUser) {
            pFriendUser->DeleteFriend(pInvite->stInvite.stDeleteReq.dwReqID);
            PS_RES_FRIEND_DELETE stRes{};
            stRes.dwReqID = pInvite->stInvite.stDeleteReq.dwReqID;
            stRes.dwFriendID = pInvite->stInvite.stDeleteReq.dwFriendID;
            stRes.byUsePopup = 0;
            stRes.nResult = 0;
            XSendPacket sendPacket(0xF5u, 5u);
            sendPacket.XParse << pFriendUser->GetMatchingID();
            sendPacket << stRes;
            pFriendUser->SendPacket(sendPacket);
        }
    }

    // 处理删除目标方旧好友
    if (pInvite->stInvite.stDeleteTarget.dwFriendID) {
        std::shared_ptr<CUserObject> pUser = GetUser(pInvite->stInvite.stDeleteTarget.dwReqID);
        if (pUser) {
            pUser->DeleteFriend(pInvite->stInvite.stDeleteTarget.dwFriendID);
            PS_RES_FRIEND_DELETE stRes{};
            stRes.dwReqID = pInvite->stInvite.stDeleteTarget.dwReqID;
            stRes.dwFriendID = pInvite->stInvite.stDeleteTarget.dwFriendID;
            stRes.byUsePopup = 0;
            stRes.nResult = 0;
            XSendPacket sendPacket(0xF5u, 5u);
            sendPacket.XParse << pUser->GetMatchingID();
            sendPacket << stRes;
            pUser->SendPacket(sendPacket);
        }
        std::shared_ptr<CUserObject> pFriendUser = GetUser(pInvite->stInvite.stDeleteTarget.dwFriendID);
        if (pFriendUser) {
            pFriendUser->DeleteFriend(pInvite->stInvite.stDeleteTarget.dwReqID);
            PS_RES_FRIEND_DELETE stRes{};
            stRes.dwReqID = pInvite->stInvite.stDeleteTarget.dwReqID;
            stRes.dwFriendID = pInvite->stInvite.stDeleteTarget.dwFriendID;
            stRes.byUsePopup = 0;
            stRes.nResult = 0;
            XSendPacket sendPacket(0xF5u, 5u);
            sendPacket.XParse << pFriendUser->GetMatchingID();
            sendPacket << stRes;
            pFriendUser->SendPacket(sendPacket);
        }
    }

    // 添加好友关系
    std::shared_ptr<CUserObject> pReqUser = GetUser(pInvite->stInvite.stReq.dwUCID);
    if (pReqUser) {
        std::shared_ptr<CUserObject> pTargetUser = GetUser(pInvite->stInvite.stTarget.dwUCID);
        if (pTargetUser) {
            // 目标在线 - 构建在线好友信息
            ST_FRIEND_INFO stTargetInfo{};
            pTargetUser->GetUserInfo(&stTargetInfo);
            stTargetInfo.byType = pInvite->stInvite.stReq.byType;
            stTargetInfo.tRemain = pInvite->stInvite.stReq.tRemain;
            pReqUser->AddFriend(&stTargetInfo, pTargetUser);

            XSendPacket sendPacket(0xF5u, 6u);
            sendPacket.XParse << pReqUser->GetMatchingID();
            sendPacket << stTargetInfo;
            pReqUser->SendPacket(sendPacket);

            // 发送结果通知
            PS_FRIEND_RESULT psResult{};
            psResult.nResult = 0;
            wcscpy_s(psResult.strName, stTargetInfo.strName);
            XSendPacket resultPacket(0xF5u, 3u);
            resultPacket.XParse << pReqUser->GetMatchingID();
            resultPacket << psResult;
            pReqUser->SendPacket(resultPacket);

            // 反向添加
            ST_FRIEND_INFO stReqInfo{};
            pReqUser->GetUserInfo(&stReqInfo);
            stReqInfo.byType = pInvite->stInvite.stTarget.byType;
            stReqInfo.tRemain = pInvite->stInvite.stTarget.tRemain;
            pTargetUser->AddFriend(&stReqInfo, pReqUser);

            XSendPacket targetPacket(0xF5u, 6u);
            targetPacket.XParse << pTargetUser->GetMatchingID();
            targetPacket << stReqInfo;
            pTargetUser->SendPacket(targetPacket);
        } else {
            // 目标离线 - 使用 DB 返回的信息（需转换为 ST_FRIEND_INFO）
            ST_FRIEND_INFO stOfflineInfo{};
            wcscpy_s(stOfflineInfo.strName, pInvite->stTargetInfo.strName);
            stOfflineInfo.dwID = pInvite->stTargetInfo.dwID;
            stOfflineInfo.byLevel = pInvite->stTargetInfo.byLevel;
            stOfflineInfo.byClass = pInvite->stTargetInfo.byClass;
            stOfflineInfo.byAwaken = pInvite->stTargetInfo.byAwaken;
            stOfflineInfo.dwProfilePhotoID = pInvite->stTargetInfo.dwProfilePhotoID;
            stOfflineInfo.byType = pInvite->stTargetInfo.byType;
            stOfflineInfo.byState = pInvite->stTargetInfo.byState;
            wcscpy_s(stOfflineInfo.strMemo, pInvite->stTargetInfo.strMemo);
            stOfflineInfo.byChannel = pInvite->stTargetInfo.byChannel;
            stOfflineInfo.wMapID = pInvite->stTargetInfo.wMapID;
            stOfflineInfo.nFriendPoint = pInvite->stTargetInfo.nFriendPoint;
            stOfflineInfo.bLogin = pInvite->stTargetInfo.bLogin;
            stOfflineInfo.tLogOut = pInvite->stTargetInfo.tLogOut;
            stOfflineInfo.tRemain = pInvite->stTargetInfo.tRemain;
            pReqUser->AddFriend(&stOfflineInfo, std::shared_ptr<CUserObject>());

            XSendPacket sendPacket(0xF5u, 6u);
            sendPacket.XParse << pReqUser->GetMatchingID();
            sendPacket << stOfflineInfo;
            pReqUser->SendPacket(sendPacket);

            PS_FRIEND_RESULT psResult{};
            psResult.nResult = 0;
            wcscpy_s(psResult.strName, pInvite->stTargetInfo.strName);
            XSendPacket resultPacket(0xF5u, 3u);
            resultPacket.XParse << pReqUser->GetMatchingID();
            resultPacket << psResult;
            pReqUser->SendPacket(resultPacket);
        }
    }

    // 记录日志
    SendDBLog(pInvite->stInvite.stReq.dwUAID, pInvite->stInvite.stReq.dwUCID, 3, 3, 0, 0,
              pInvite->stInvite.stTarget.dwUCID, 0, 0, 0, 0, L"");
    SendDBLog(pInvite->stInvite.stTarget.dwUAID, pInvite->stInvite.stTarget.dwUCID, 3, 6, 0, 0,
              pInvite->stInvite.stReq.dwUCID, 0, 0, 0, 0, L"");
}

void XRelayServer::InviteCheckFriend(const PS_RES_FRIEND_INVITE* pInvite, std::uint32_t dwDelUCID) {
    // 对齐 IDA 0x1400B5860: 好友邀请检查
    if (!pInvite) return;

    // 邀请失败的情况
    if (pInvite->byResult != 0) {
        const std::shared_ptr<CUserObject> pUser = GetUser(pInvite->dwReqUCID);
        if (!pUser) return;

        // 发送失败结果
        PS_FRIEND_RESULT psResult{};
        psResult.nResult = 55104;
        wcscpy_s(psResult.strName, pInvite->strTargetUserName);

        XSendPacket xSendPacket(0xF5u, 3u);
        xSendPacket.XParse << pUser->GetMatchingID();
        xSendPacket << psResult;
        pUser->SendPacket(xSendPacket);
        return;
    }

    // 构造 DB 请求（提前声明以便后续填充）
    PS_DB_FRIEND_INVITE psDBInvite{};

    {
        CFAutoSlimReadLock autolock(&m_rwLock);

        // 邀请成功，检查请求用户
        const std::shared_ptr<CUserObject> pReqUser = GetUser(pInvite->dwReqUCID);
        if (!pReqUser) return;

        // 检查是否已是好友
        if (pReqUser->IsFriendList(pInvite->strTargetUserName, 1u)) {
            const std::uint32_t dwFriendUCID = pReqUser->GetFriendUCID(pInvite->strTargetUserName);
            if (dwFriendUCID == pInvite->dwTargetUCID) {
                // 已是好友
                PS_FRIEND_RESULT psResult{};
                psResult.nResult = 55101;
                wcscpy_s(psResult.strName, pInvite->strTargetUserName);

                XSendPacket xSendPacket(0xF5u, 3u);
                xSendPacket.XParse << pReqUser->GetMatchingID();
                xSendPacket << psResult;
                pReqUser->SendPacket(xSendPacket);
                return;
            }
            // 需要先删除旧好友
            psDBInvite.stDeleteReq.dwReqID = pReqUser->GetMatchingID();
            psDBInvite.stDeleteReq.dwFriendID = dwFriendUCID;
        }

        // 检查是否已在邀请列表中
        if (pReqUser->IsFriendList(pInvite->strTargetUserName, 2u)) {
            PS_FRIEND_RESULT psResult{};
            psResult.nResult = 55111;
            wcscpy_s(psResult.strName, pInvite->strTargetUserName);

            XSendPacket xSendPacket(0xF5u, 3u);
            xSendPacket.XParse << pReqUser->GetMatchingID();
            xSendPacket << psResult;
            pReqUser->SendPacket(xSendPacket);
            return;
        }

        // 检查好友列表容量
        if (!pReqUser->IsValiedFriendListCount(1u)) {
            PS_FRIEND_RESULT psResult{};
            psResult.nResult = 55103;
            wcscpy_s(psResult.strName, pInvite->strTargetUserName);

            XSendPacket xSendPacket(0xF5u, 3u);
            xSendPacket.XParse << pReqUser->GetMatchingID();
            xSendPacket << psResult;
            pReqUser->SendPacket(xSendPacket);
            return;
        }

        // 检查是否在黑名单中
        if (pReqUser->IsBlockList(pInvite->strTargetUserName)) {
            PS_FRIEND_RESULT psResult{};
            psResult.nResult = 55105;
            wcscpy_s(psResult.strName, pInvite->strTargetUserName);

            XSendPacket xSendPacket(0xF5u, 3u);
            xSendPacket.XParse << pReqUser->GetMatchingID();
            xSendPacket << psResult;
            pReqUser->SendPacket(xSendPacket);
            return;
        }
    }

    // 填充 DB 请求
    if (dwDelUCID != 0) {
        psDBInvite.stDeleteTarget.dwReqID = pInvite->dwTargetUCID;
        psDBInvite.stDeleteTarget.dwFriendID = dwDelUCID;
    }
    psDBInvite.stReq.dwUCID = pInvite->dwReqUCID;
    psDBInvite.stReq.byType = 3;
    psDBInvite.stReq.tRemain = 0;
    psDBInvite.stTarget.dwUCID = pInvite->dwTargetUCID;
    psDBInvite.stTarget.byType = 2;
    psDBInvite.stTarget.tRemain = static_cast<std::int64_t>(std::time(nullptr)) + 604800;  // 对齐 IDA: CTime::GetTickCount + 7天

    XSendDBPacket xSendDBPacket(0, 5u, 2u);
    xSendDBPacket << psDBInvite;
    SendDBGame(xSendDBPacket);
}

void XRelayServer::AcceptFriend(const PS_DB_FRIEND_ACCEPT_RES* pAccept) {
    // 对齐 IDA 0x1400B6850: 好友接受结果处理
    if (!pAccept) return;

    CFAutoSlimWriteLock autolock(&m_rwLock);

    std::shared_ptr<CUserObject> pReqUser = GetUser(pAccept->stReq.dwUCID);
    if (!pReqUser) return;

    std::shared_ptr<CUserObject> pTargetUser = GetUser(pAccept->stTarget.dwUCID);

    // 复制 stTargetInfo 以便修改
    DB_FRIEND_INFO stTargetInfo = pAccept->stTargetInfo;

    // 更新目标用户在线信息（对齐 IDA: 包括名称更新）
    if (pTargetUser) {
        stTargetInfo.bLogin = true;
        stTargetInfo.byChannel = pTargetUser->GetChannel();
        stTargetInfo.wMapID = pTargetUser->GetMapID();
        std::wstring strName = pTargetUser->GetName();
        wcscpy_s(stTargetInfo.strName, strName.c_str());
    }

    if (pAccept->nResult) {
        // 接受失败
        PS_RES_FRIEND_ACCEPT stRes{};
        stRes.nResult = 55114;  // 错误码
        stRes.stFriend.dwID = pAccept->stTarget.dwUCID;
        XSendPacket sendPacket(0xF5u, 4u);
        sendPacket.XParse << pReqUser->GetMatchingID();
        sendPacket << stRes;
        pReqUser->SendPacket(sendPacket);
        return;
    }

    // 成功 - 转换 DB_FRIEND_INFO 到 ST_FRIEND_INFO 并更新好友列表
    stTargetInfo.byType = 1;
    ST_FRIEND_INFO stTargetFriend{};
    wcscpy_s(stTargetFriend.strName, stTargetInfo.strName);
    stTargetFriend.dwID = stTargetInfo.dwID;
    stTargetFriend.byLevel = stTargetInfo.byLevel;
    stTargetFriend.byClass = stTargetInfo.byClass;
    stTargetFriend.byAwaken = stTargetInfo.byAwaken;
    stTargetFriend.dwProfilePhotoID = stTargetInfo.dwProfilePhotoID;
    stTargetFriend.byType = stTargetInfo.byType;
    stTargetFriend.byState = stTargetInfo.byState;
    wcscpy_s(stTargetFriend.strMemo, stTargetInfo.strMemo);
    stTargetFriend.byChannel = stTargetInfo.byChannel;
    stTargetFriend.wMapID = stTargetInfo.wMapID;
    stTargetFriend.nFriendPoint = stTargetInfo.nFriendPoint;
    stTargetFriend.bLogin = stTargetInfo.bLogin;
    stTargetFriend.tLogOut = stTargetInfo.tLogOut;
    stTargetFriend.tRemain = stTargetInfo.tRemain;
    pReqUser->UpdateFriend(&stTargetFriend, 1);

    PS_RES_FRIEND_ACCEPT stRes{};
    stRes.nResult = 0;
    stRes.stFriend = stTargetInfo;
    XSendPacket sendPacket(0xF5u, 4u);
    sendPacket.XParse << pReqUser->GetMatchingID();
    sendPacket << stRes;
    pReqUser->SendPacket(sendPacket);

    // 日志
    SendDBLog(pReqUser->GetUAID(), pReqUser->GetCID(), 3, 4, 0, 0, pAccept->stTarget.dwUCID, 0, 0, 0, 0, L"");
    SendDBLog(pAccept->stTarget.dwUAID, pAccept->stTarget.dwUCID, 3, 7, 0, 0, pReqUser->GetCID(), 0, 0, 0, 0, L"");

    // 如果目标在线，也更新并通知
    if (pTargetUser) {
        ST_FRIEND_INFO stReqInfo{};
        pReqUser->GetUserInfo(&stReqInfo);
        stReqInfo.byType = 1;
        pTargetUser->UpdateFriend(&stReqInfo, 0);

        XSendPacket targetPacket(0xF5u, 6u);
        targetPacket.XParse << pTargetUser->GetMatchingID();
        targetPacket << stReqInfo;
        pTargetUser->SendPacket(targetPacket);
    }
}

void XRelayServer::DeleteFriend(const PS_DB_FRIEND_DELETE* pDelete) {
    // 对齐 IDA 0x1400B7330: 删除好友结果处理
    if (!pDelete) return;

    CFAutoSlimWriteLock autolock(&m_rwLock);

    // 通知请求方
    std::shared_ptr<CUserObject> pReqUser = GetUser(pDelete->dwReqUCID);
    if (pReqUser) {
        if (!pDelete->nResult) {
            pReqUser->DeleteFriend(pDelete->dwFriendUCID);
        }
        PS_RES_FRIEND_DELETE stRes{};
        stRes.dwReqID = pReqUser->GetMatchingID();
        stRes.dwFriendID = pDelete->dwFriendUCID;
        stRes.byUsePopup = 1;
        stRes.nResult = pDelete->nResult;
        XSendPacket sendPacket(0xF5u, 5u);
        sendPacket.XParse << pReqUser->GetMatchingID();
        sendPacket << stRes;
        pReqUser->SendPacket(sendPacket);
    }

    // 通知好友方
    std::shared_ptr<CUserObject> pFriendUser = GetUser(pDelete->dwFriendUCID);
    if (pFriendUser) {
        if (!pDelete->nResult) {
            pFriendUser->DeleteFriend(pDelete->dwReqUCID);
        }
        PS_RES_FRIEND_DELETE stRes{};
        stRes.dwReqID = pFriendUser->GetMatchingID();
        stRes.dwFriendID = pDelete->dwReqUCID;
        stRes.byUsePopup = 0;
        stRes.nResult = pDelete->nResult;
        XSendPacket sendPacket(0xF5u, 5u);
        sendPacket.XParse << pFriendUser->GetMatchingID();
        sendPacket << stRes;
        pFriendUser->SendPacket(sendPacket);
    }

    // 日志
    SendDBLog(pDelete->dwReqUAID, pDelete->dwReqUCID, 3, 5, 0, 0, pDelete->dwFriendUCID, 0, 0, 0, 0, L"");
    SendDBLog(pDelete->dwFriendUAID, pDelete->dwFriendUCID, 3, 8, 0, 0, pDelete->dwReqUCID, 0, 0, 0, 0, L"");
}

void XRelayServer::AddBlockList(const PS_RES_DB_FRIEND_BLOCK* pBlock) {
    // 对齐 IDA 0x1400B7B30: 添加黑名单结果处理
    if (!pBlock) return;

    CFAutoSlimWriteLock autolock(&m_rwLock);

    std::shared_ptr<CUserObject> pReqUser = GetUserByUAID(pBlock->dwReqUAID);
    if (!pReqUser) return;

    if (!pBlock->nResult) {
        // 转换 DB_BLOCK_INFO 到 ST_BLOCK_INFO
        ST_BLOCK_INFO stBlockInfo{};
        stBlockInfo.dwUCID = pBlock->stBlock.dwUCID;
        stBlockInfo.byLevel = pBlock->stBlock.byLevel;
        wcscpy_s(stBlockInfo.strName, pBlock->stBlock.strName);
        pReqUser->AddBlockList(&stBlockInfo);
        SendDBLog(pReqUser->GetUAID(), pReqUser->GetCID(), 3, 10, pBlock->stBlock.dwUCID, 0, 0, 0, 0, 0, 0, L"");
    }

    PS_RES_BLOCKLIST_ADD stRes{};
    stRes.dwReqUAID = pBlock->dwReqUAID;
    stRes.stBlock = pBlock->stBlock;
    stRes.stResult.nResult = pBlock->nResult;
    wcscpy_s(stRes.stResult.strName, pBlock->stBlock.strName);

    XSendPacket sendPacket(0xF5u, 7u);
    sendPacket.XParse << pReqUser->GetMatchingID();
    sendPacket << stRes;
    pReqUser->SendPacket(sendPacket);
}

void XRelayServer::DeleteBlockList(const PS_RES_BLOCKLIST_DELETE* pDelete) {
    // 对齐 IDA 0x1400B8190: 删除黑名单结果处理
    if (!pDelete) return;

    CFAutoSlimWriteLock autolock(&m_rwLock);

    std::shared_ptr<CUserObject> pUser = GetUserByUAID(pDelete->dwReqUAID);
    if (!pUser) return;

    if (!pDelete->nResult) {
        pUser->DeleteBlockList(pDelete->dwTargetUCID);
        SendDBLog(pUser->GetUAID(), pUser->GetCID(), 3, 11, pDelete->dwTargetUCID, 0, 0, 0, 0, 0, 0, L"");
    }

    XSendPacket sendPacket(0xF5u, 8u);
    sendPacket.XParse << pUser->GetMatchingID();
    sendPacket << *pDelete;
    pUser->SendPacket(sendPacket);
}

void XRelayServer::SetRecruitList(const ST_RECRUIT_LIST* pList, std::uint8_t byLast) {
    // 对齐 IDA 0x1400B8440: 加载招募列表
    m_RecruitManager.LoadRecruitList(&pList->vecRecruit, byLast);
}

// 对齐 IDA 0x1400BD1E0: SendServerInfoAll
// 收集所有 GameServer 信息并发送给监控请求方
void XRelayServer::SendServerInfoAll(CServer* pReqServer) {
    if (!pReqServer) return;

    std::vector<SS_SERVER_INFO> vecServerInfo;
    XSendPacket xSendPacket(0xF7, 1);

    int nServerCount = 0;
    int nUserCount = 0;

    // 对齐 IDA: 读锁保护 m_mapGameServer 遍历
    {
        CFAutoSlimReadLock autolock(&m_rwServerLock);
        for (auto it = m_mapGameServer.begin(); it != m_mapGameServer.end(); ++it) {
            CServer* pServer = it->second;
            if (pServer) {
                vecServerInfo.push_back(pServer->GetServerInfo());
                nUserCount += pServer->GetUserCount();
                ++nServerCount;
            }
        }
    }

    // 对齐 IDA: 序列化服务器列表 + 统计信息
    const std::uint32_t dwSize = static_cast<std::uint32_t>(vecServerInfo.size());
    xSendPacket.XParse << dwSize;
    for (const auto& info : vecServerInfo) {
        xSendPacket << info;
    }

    PS_SERVER_COMMON_INFO stCommonInfo{};
    stCommonInfo.nServerCount = nServerCount;
    stCommonInfo.nTotalUserCount = nUserCount;
    xSendPacket << stCommonInfo;

    pReqServer->SendEx(xSendPacket);
}

// 对齐 IDA 0x1400BD410: SendOperationTimeInfo
// 向所有在线用户发送 ModeMaze 运营时间信息
void XRelayServer::SendOperationTimeInfo(const PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO& psInfo) {
    XSendPacket xSendPacket(0xFD, 0x11);
    xSendPacket << psInfo;
    SendPacketAll(xSendPacket);
}

void XRelayServer::ResFriendFind(const PS_DB_FRIEND_FIND* pFind) {
    // 对齐 IDA 0x1400B94E0: 查找好友结果处理
    if (!pFind) return;

    CFAutoSlimReadLock autolock(&m_rwLock);

    const std::shared_ptr<CUserObject> pReqUser = GetUser(pFind->dwReqUCID);
    if (!pReqUser) return;

    // 对齐 IDA: 复制查找结果以更新在线用户状态
    PS_FIND_FRIEND_LIST psList = pFind->psList;
    for (auto& stFriend : psList.vecList) {
        const std::shared_ptr<CUserObject> pFriend = GetUser(stFriend.dwUCID);
        if (pFriend) {
            stFriend.bLogin = 1;
            stFriend.byChannel = pFriend->GetChannel();
            stFriend.wMapID = pFriend->GetMapID();
            stFriend.byLevel = pFriend->GetLevel();
        } else {
            stFriend.bLogin = 0;
        }
    }

    // 对齐 IDA: 发送 0xF5/0x22 而非 0xF5/0x0B
    XSendPacket xSendPacket(0xF5u, 0x22u);
    xSendPacket.XParse << pReqUser->GetMatchingID();
    xSendPacket << psList;
    pReqUser->SendPacket(xSendPacket);
}
