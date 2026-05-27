// DBOperations.h
// Database operation wrappers for SoulWorker GameServer
// Phase 6 implementation - wraps existing CGoc* and CCharacterProcess functions

#pragma once

#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include <cstdint>
#include <functional>

// Forward declarations
class XSendDBPacket;
class XPacket;
class CGocAchieve;
class CGocQuest;
class CGocLeague;
class CGocRecode;

// ============================================================================
// Database Operation Result Codes
// ============================================================================
enum class DBResult : std::int32_t {
    Success = 0,
    Error = -1,
    NotConnected = -2,
    InvalidData = -3,
    Timeout = -4
};

// ============================================================================
// Database Operation Callbacks (for async operations)
// ============================================================================
using DBLoadCallback = std::function<void(DBResult result)>;
using DBSaveCallback = std::function<void(DBResult result)>;

// ============================================================================
// CDBOperations - Database Operation Manager
// ============================================================================
class CDBOperations {
public:
    CDBOperations() = default;
    ~CDBOperations() = default;

    // ========================================================================
    // User Data Operations
    // ========================================================================
    
    /**
     * SaveUserData - Save user data to database
     * Corresponds to CCharacterProcess::ReqCharacterSave (0x1403B31A0)
     * @param pUser User to save
     * @param callback Optional callback for async completion
     * @return true if save initiated successfully
     */
    static bool SaveUserData(CUser* pUser, DBSaveCallback callback = nullptr);
    
    /**
     * LoadUserData - Load user data from database
     * Corresponds to CCharacterProcess::ResCharacterLoad (0x1403B7060)
     * @param pUser User to load data into
     * @param packet Packet containing loaded data
     * @param callback Optional callback for async completion
     * @return true if load successful
     */
    static bool LoadUserData(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // ========================================================================
    // Guild Data Operations
    // ========================================================================
    
    /**
     * SaveGuildData - Save guild data to database
     * Uses CGocLeague functions
     * @param pUser User whose guild data to save
     * @param callback Optional callback for async completion
     * @return true if save initiated successfully
     */
    static bool SaveGuildData(CUser* pUser, DBSaveCallback callback = nullptr);
    
    /**
     * LoadGuildData - Load guild data from database
     * Uses CGocLeague functions
     * @param pUser User to load guild data for
     * @param packet Packet containing guild data
     * @param callback Optional callback for async completion
     * @return true if load successful
     */
    static bool LoadGuildData(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // ========================================================================
    // Quest Data Operations
    // ========================================================================
    
    /**
     * SaveQuestData - Save quest progress to database
     * Uses CGocQuest functions
     * @param pUser User whose quest data to save
     * @param callback Optional callback for async completion
     * @return true if save initiated successfully
     */
    static bool SaveQuestData(CUser* pUser, DBSaveCallback callback = nullptr);
    
    /**
     * LoadQuestData - Load quest progress from database
     * Uses CGocQuest functions
     * @param pUser User to load quest data for
     * @param packet Packet containing quest data
     * @param callback Optional callback for async completion
     * @return true if load successful
     */
    static bool LoadQuestData(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // ========================================================================
    // Ranking Data Operations
    // ========================================================================
    
    /**
     * SaveRankingData - Save ranking data to database
     * Corresponds to CGocRecode::RankingDataUpdate (0x140154150)
     * @param pUser User whose ranking data to save
     * @param dwMazeID Maze ID for ranking
     * @param dwMazePlayTime Play time
     * @param nMonsterKillScore Monster kill score
     * @param callback Optional callback for async completion
     * @return true if save initiated successfully
     */
    static bool SaveRankingData(CUser* pUser, std::uint32_t dwMazeID, 
                                std::uint32_t dwMazePlayTime, int nMonsterKillScore,
                                DBSaveCallback callback = nullptr);
    
    /**
     * LoadRankingData - Load ranking data from database
     * Corresponds to CGocRecode::ResRankingMyInfo (0x140156540)
     * @param pUser User to load ranking data for
     * @param packet Packet containing ranking data
     * @param callback Optional callback for async completion
     * @return true if load successful
     */
    static bool LoadRankingData(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // ========================================================================
    // Achievement Data Operations
    // ========================================================================
    
    /**
     * SaveAchieveData - Save achievement data to database
     * Corresponds to CGocAchieve::SendDBUpdateList (0x140029580)
     * @param pUser User whose achievement data to save
     * @param pUpdateList Achievement update list to save
     * @param callback Optional callback for async completion
     * @return true if save initiated successfully
     */
    static bool SaveAchieveData(CUser* pUser, ST_ACHIEVE_UPDATE_LIST* pUpdateList,
                                 DBSaveCallback callback = nullptr);
    
    /**
     * LoadAchieveData - Load achievement data from database
     * Corresponds to CGocAchieve::LoadAchieve (0x14002BDD0)
     * @param pUser User to load achievement data for
     * @param pAchieveBit Achievement bit flags
     * @param pAchieveList Achievement list
     * @param pCategory Achievement category
     * @param bFirst First load flag
     * @param callback Optional callback for async completion
     * @return true if load successful
     */
    static bool LoadAchieveData(CUser* pUser, ST_ACHIEVE_BIT* pAchieveBit,
                                 ST_ACHIEVE_LIST* pAchieveList,
                                 ST_ACHIEVE_CATEGORY* pCategory, bool bFirst,
                                 DBLoadCallback callback = nullptr);

private:
    // Helper to get GOC component
    template<typename T>
    static T* GetComponent(CUser* pUser);
};
