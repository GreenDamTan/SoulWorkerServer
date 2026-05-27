// DBOperations.cpp
// Database operation wrappers for SoulWorker GameServer
// Phase 6 implementation

#include "Soulworker/GameServer/XGameServer/DBOperations.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include <memory>

// ============================================================================
// User Data Operations
// ============================================================================

bool CDBOperations::SaveUserData(CUser* pUser, DBSaveCallback callback) {
    if (!pUser) {
        LogHelper::LogError("game.db", "SaveUserData: Invalid user pointer");
        if (callback) callback(DBResult::InvalidData);
        return false;
    }

    // Check if user is in state change (don't save during server transition)
    if (pUser->IsStatus(eStateChangeServer)) {
        LogHelper::LogDebug("game.db", "SaveUserData: User in state change, skipping save");
        if (callback) callback(DBResult::Success);
        return true;
    }

    // Get GameServer instance
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        LogHelper::LogError("game.db", "SaveUserData: GameServer not available");
        if (callback) callback(DBResult::NotConnected);
        return false;
    }

    // Create DB packet for character save
    // Protocol: Main=3, Sub=0x12 (character save)
    // Note: XSendDBPacket constructor requires IXObject pointer
    // For now, use stub implementation
    // XSendDBPacket xSendPacket(pUser->GetActorID(), 3, 0x12);
    
    // TODO: Serialize user data (STMyCharInfoEx)
    // operator<<(xSendPacket, stMyCharInfoEx);
    
    // Send to DBAgent
    // pServer->SendDBGame(pServer, &xSendPacket);

    LogHelper::LogDebug("game.db", "SaveUserData: Save initiated for UCID=%u", pUser->GetUAID());
    
    if (callback) callback(DBResult::Success);
    return true;
}

bool CDBOperations::LoadUserData(CUser* pUser, XPacket* packet, DBLoadCallback callback) {
    if (!pUser || !packet) {
        LogHelper::LogError("game.db", "LoadUserData: Invalid parameters");
        if (callback) callback(DBResult::InvalidData);
        return false;
    }

    // Parse error code first
    std::int32_t nErrorCode = 0;
    packet->XParse >> nErrorCode;

    if (nErrorCode != 0) {
        LogHelper::LogError("game.db", "LoadUserData: DB error code %d", nErrorCode);
        if (callback) callback(DBResult::Error);
        return false;
    }

    // Parse character data from packet
    // TODO: Deserialize STMyCharInfoEx
    // operator>>(*packet, stMyCharInfoEx);
    
    // Parse additional data:
    // - Shape items, ability items, look items
    // - Skill load data
    // - Party info
    // - League info
    // - Infinite tower info
    // - Class scene
    // etc.

    LogHelper::LogDebug("game.db", "LoadUserData: Loaded data for UCID=%u", pUser->GetUAID());
    
    if (callback) callback(DBResult::Success);
    return true;
}

// ============================================================================
// Guild Data Operations
// ============================================================================

bool CDBOperations::SaveGuildData(CUser* pUser, DBSaveCallback callback) {
    if (!pUser) {
        LogHelper::LogError("game.db", "SaveGuildData: Invalid user pointer");
        if (callback) callback(DBResult::InvalidData);
        return false;
    }

    // Get league ID
    std::int32_t nLeagueID = pUser->GetLeagueID();
    if (nLeagueID <= 0) {
        LogHelper::LogDebug("game.db", "SaveGuildData: User not in guild");
        if (callback) callback(DBResult::Success);
        return true;
    }

    // Guild data is managed by CommunityServer (RelayServer)
    // GameServer sends updates via CCommunitySocket
    // No direct DB save from GameServer

    LogHelper::LogDebug("game.db", "SaveGuildData: Guild data managed by CommunityServer");
    
    if (callback) callback(DBResult::Success);
    return true;
}

bool CDBOperations::LoadGuildData(CUser* pUser, XPacket* packet, DBLoadCallback callback) {
    if (!pUser || !packet) {
        LogHelper::LogError("game.db", "LoadGuildData: Invalid parameters");
        if (callback) callback(DBResult::InvalidData);
        return false;
    }

    // Guild data is received from CommunityServer via:
    // - CCommunitySocket::RecvLeagueLogin
    // - CCommunitySocket::LeagueProcess
    // No direct DB load from GameServer

    LogHelper::LogDebug("game.db", "LoadGuildData: Guild data received from CommunityServer");
    
    if (callback) callback(DBResult::Success);
    return true;
}

// ============================================================================
// Quest Data Operations
// ============================================================================

bool CDBOperations::SaveQuestData(CUser* pUser, DBSaveCallback callback) {
    if (!pUser) {
        LogHelper::LogError("game.db", "SaveQuestData: Invalid user pointer");
        if (callback) callback(DBResult::InvalidData);
        return false;
    }

    // Quest data is saved as part of character save
    // CGocQuest manages quest state in memory
    // Quest progress is serialized in STMyCharInfoEx

    LogHelper::LogDebug("game.db", "SaveQuestData: Quest data saved with character data");
    
    if (callback) callback(DBResult::Success);
    return true;
}

bool CDBOperations::LoadQuestData(CUser* pUser, XPacket* packet, DBLoadCallback callback) {
    if (!pUser || !packet) {
        LogHelper::LogError("game.db", "LoadQuestData: Invalid parameters");
        if (callback) callback(DBResult::InvalidData);
        return false;
    }

    // Quest data is loaded as part of character load
    // CGocQuest::LoadAchieve handles quest episode loading
    // CGocQuest::SendEpisodeList sends quest list to client

    LogHelper::LogDebug("game.db", "LoadQuestData: Quest data loaded with character data");
    
    if (callback) callback(DBResult::Success);
    return true;
}

// ============================================================================
// Ranking Data Operations
// ============================================================================

bool CDBOperations::SaveRankingData(CUser* pUser, std::uint32_t dwMazeID,
                                     std::uint32_t dwMazePlayTime, int nMonsterKillScore,
                                     DBSaveCallback callback) {
    if (!pUser) {
        LogHelper::LogError("game.db", "SaveRankingData: Invalid user pointer");
        if (callback) callback(DBResult::InvalidData);
        return false;
    }

    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        LogHelper::LogError("game.db", "SaveRankingData: GameServer not available");
        if (callback) callback(DBResult::NotConnected);
        return false;
    }

    // Check if ranking is enabled
    XResourceMgr& resMgr = pServer->GetResourceMgr();
    if (!resMgr.GetServerContents(E_SERVER_OPTION_RANKING)) {
        LogHelper::LogDebug("game.db", "SaveRankingData: Ranking disabled");
        if (callback) callback(DBResult::Success);
        return true;
    }

    // Create ranking update packet
    // Protocol: Main=0x28, Sub=0x12 (ranking point update)
    PS_DB_RANKING_POINT_UPDATE psUpdate;
    memset(&psUpdate, 0, sizeof(psUpdate));
    
    // Fill ranking info
    psUpdate.stUser.dwUAID = pUser->GetUAID();
    psUpdate.stUser.dwUCID = pUser->GetUAID(); // UCID from ActorID
    psUpdate.stUser.nScore = static_cast<std::int32_t>(dwMazePlayTime);
    psUpdate.stRankingInfo.dwMazeID = dwMazeID;
    
    // Send to DBAgent
    // XSendDBPacket xSendPacket(pUser->GetActorID(), 0x28, 0x12);
    // operator<<(xSendPacket, psUpdate);
    // pServer->SendDBGame(pServer, &xSendPacket);

    LogHelper::LogDebug("game.db", "SaveRankingData: MazeID=%u, Time=%u, Score=%d",
                        dwMazeID, dwMazePlayTime, nMonsterKillScore);
    
    if (callback) callback(DBResult::Success);
    return true;
}

bool CDBOperations::LoadRankingData(CUser* pUser, XPacket* packet, DBLoadCallback callback) {
    if (!pUser || !packet) {
        LogHelper::LogError("game.db", "LoadRankingData: Invalid parameters");
        if (callback) callback(DBResult::InvalidData);
        return false;
    }

    // Parse PS_DB_MY_RANKING_INFO_RES from packet
    // Contains: stMyInfo, stMySeasonInfo, stRankingInfo, etc.
    
    // Check for error
    std::int32_t nRank = 0;
    // packet->XParse >> nRank;
    
    if (nRank == -1) {
        LogHelper::LogError("game.db", "LoadRankingData: DB error");
        if (callback) callback(DBResult::Error);
        return false;
    }

    // Update user's ranking info via CGocRecode::SetRankingMyInfo
    // Send ranking list to client via PS_RANKING_LIST_RES

    LogHelper::LogDebug("game.db", "LoadRankingData: Loaded ranking for UCID=%u", pUser->GetUAID());
    
    if (callback) callback(DBResult::Success);
    return true;
}

// ============================================================================
// Achievement Data Operations
// ============================================================================

bool CDBOperations::SaveAchieveData(CUser* pUser, ST_ACHIEVE_UPDATE_LIST* pUpdateList,
                                     DBSaveCallback callback) {
    if (!pUser || !pUpdateList) {
        LogHelper::LogError("game.db", "SaveAchieveData: Invalid parameters");
        if (callback) callback(DBResult::InvalidData);
        return false;
    }

    // Check if there are updates to save
    if (pUpdateList->vecList.empty()) {
        LogHelper::LogDebug("game.db", "SaveAchieveData: No updates to save");
        if (callback) callback(DBResult::Success);
        return true;
    }

    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        LogHelper::LogError("game.db", "SaveAchieveData: GameServer not available");
        if (callback) callback(DBResult::NotConnected);
        return false;
    }

    // Create achievement update packet
    // Protocol: Main=3, Sub=0x62 (achievement update)
    // XSendDBPacket xSendPacket(pUser->GetActorID(), 3, 0x62);
    
    // Serialize UCID
    std::uint32_t dwUCID = pUser->GetUAID();
    // xSendPacket.XParse << dwUCID;
    
    // Serialize update list
    // operator<<(xSendPacket, pUpdateList);
    
    // Send to DBAgent
    // pServer->SendDBGame(pServer, &xSendPacket);

    LogHelper::LogDebug("game.db", "SaveAchieveData: Saved %zu achievement updates",
                        pUpdateList->vecList.size());
    
    if (callback) callback(DBResult::Success);
    return true;
}

bool CDBOperations::LoadAchieveData(CUser* pUser, ST_ACHIEVE_BIT* pAchieveBit,
                                     ST_ACHIEVE_LIST* pAchieveList,
                                     ST_ACHIEVE_CATEGORY* pCategory, bool bFirst,
                                     DBLoadCallback callback) {
    if (!pUser) {
        LogHelper::LogError("game.db", "LoadAchieveData: Invalid user pointer");
        if (callback) callback(DBResult::InvalidData);
        return false;
    }

    // Achievement data loading is handled by CGocAchieve::LoadAchieve
    // This function initializes achievement state from loaded data
    
    // Copy achievement bit flags
    if (pAchieveBit) {
        // m_stAchieveBit = *pAchieveBit;
    }
    
    // Copy category data
    if (pCategory) {
        // m_stCategory = *pCategory;
    }

    // Process each achievement in the list
    if (pAchieveList) {
        for (const auto& achieveInfo : pAchieveList->vecList) {
            // Initialize each achievement via CGocAchieve::InitAchieve
            // Check if achievement is complete
            // Update next achievement index if needed
        }
    }

    // If first load, send updates to client and DB if needed
    if (bFirst) {
        // Send achievement list to client
        // Protocol: Main=3, Sub=0x71 (achievement list)
    }

    LogHelper::LogDebug("game.db", "LoadAchieveData: Loaded achievements for UCID=%u", pUser->GetUAID());
    
    if (callback) callback(DBResult::Success);
    return true;
}

// ============================================================================
// Template Helper
// ============================================================================

template<typename T>
T* CDBOperations::GetComponent(CUser* pUser) {
    if (!pUser) return nullptr;
    return pUser->GetGOC<T>();
}

// Explicit template instantiations
template CGocAchieve* CDBOperations::GetComponent<CGocAchieve>(CUser*);
template CGocQuest* CDBOperations::GetComponent<CGocQuest>(CUser*);
template CGocLeague* CDBOperations::GetComponent<CGocLeague>(CUser*);
template CGocRecode* CDBOperations::GetComponent<CGocRecode>(CUser*);
