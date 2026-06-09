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
struct ST_BOOSTER_OUTPUT;

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

    // ========================================================================
    // Extended Database Persistence Functions (Phase 6 - 25 functions)
    // ========================================================================

    // 1. SaveInventory - Save player inventory to database
    // IDA: CGocInventory::SendDBSocketLoad (0x140BB880)
    static bool SaveInventory(CUser* pUser, DBSaveCallback callback = nullptr);

    // 2. LoadInventory - Load player inventory from database
    static bool LoadInventory(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // 3. SavePlayerData - Save complete player data
    // IDA: CCharacterProcess::ReqCharacterSave (0x1403B31A0)
    static bool SavePlayerData(CUser* pUser, DBSaveCallback callback = nullptr);

    // 4. LoadPlayerData - Load complete player data
    // IDA: CCharacterProcess::ResCharacterLoad (0x1403B7060)
    static bool LoadPlayerData(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // 5. SaveQuestProgress - Save quest state to database
    // IDA: CGocQuest::DBUpdateEpisodeInfo (0x140129C80)
    static bool SaveQuestProgress(CUser* pUser, std::uint32_t dwEpisodeID,
                                   ST_QUEST_EPISODE* pEpisode, DBSaveCallback callback = nullptr);

    // 6. LoadQuestProgress - Load quest state from database
    // IDA: CGocQuest::SendReqQuestList (0x140129EF0)
    static bool LoadQuestProgress(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // 7. ExecuteQuery - Execute SQL query (via DBAgent)
    static bool ExecuteQuery(CUser* pUser, const char* szQuery, DBSaveCallback callback = nullptr);

    // 8. BeginTransaction - Start database transaction
    static bool BeginTransaction(CUser* pUser, DBSaveCallback callback = nullptr);

    // 9. CommitTransaction - Commit database transaction
    static bool CommitTransaction(CUser* pUser, DBSaveCallback callback = nullptr);

    // 10. RollbackTransaction - Rollback database transaction
    static bool RollbackTransaction(CUser* pUser, DBSaveCallback callback = nullptr);

    // 11. SaveFriendList - Save friend list to database
    // IDA: CGocFriend functions
    static bool SaveFriendList(CUser* pUser, DBSaveCallback callback = nullptr);

    // 12. LoadFriendList - Load friend list from database
    // IDA: CGocFriend::SetFriendList (0x140086CB0)
    static bool LoadFriendList(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // 13. SaveMailData - Save mail/post data to database
    // IDA: CGocPost::SendDBPostList (0x140114AB0)
    static bool SaveMailData(CUser* pUser, DBSaveCallback callback = nullptr);

    // 14. LoadMailData - Load mail/post data from database
    static bool LoadMailData(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // 15. SaveExchangeData - Save exchange/trade data to database
    // IDA: CGocExchange::DBReqExchangeMyList (0x1407C5B0)
    static bool SaveExchangeData(CUser* pUser, DBSaveCallback callback = nullptr);

    // 16. LoadExchangeData - Load exchange/trade data from database
    static bool LoadExchangeData(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // 17. SaveAttendanceData - Save attendance data to database
    // IDA: CGocAttendance::SendDBAttendance (0x14033D60)
    static bool SaveAttendanceData(CUser* pUser, DBSaveCallback callback = nullptr);

    // 18. LoadAttendanceData - Load attendance data from database
    static bool LoadAttendanceData(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // 19. SaveDailyMissionData - Save daily mission data to database
    // IDA: CGocDailyMission::DBUpdateMissionInfo (0x14053AD0)
    static bool SaveDailyMissionData(CUser* pUser, PS_DAILY_MISSION_UPDATE* pUpdate,
                                      DBSaveCallback callback = nullptr);

    // 20. LoadDailyMissionData - Load daily mission data from database
    // IDA: CGocDailyMission::DBAddDailyMissionList (0x14053EE0)
    static bool LoadDailyMissionData(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // 21. SaveAttributeData - Save character attribute/FP data to database
    // IDA: CGocAttribute::SendDBUpdateFP (0x1403FB30)
    static bool SaveAttributeData(CUser* pUser, DBSaveCallback callback = nullptr);

    // 22. LoadAttributeData - Load character attribute/FP data from database
    static bool LoadAttributeData(CUser* pUser, XPacket* packet, DBLoadCallback callback = nullptr);

    // 23. SaveEventData - Save event/roulette data to database
    // IDA: CGocEvent::SendDBRouletteInfo (0x1406C310)
    static bool SaveEventData(CUser* pUser, std::uint8_t byType,
                              std::uint32_t dwEventID, DBSaveCallback callback = nullptr);

    // 24. SaveBoosterData - Save booster/buff data to database
    // IDA: CGocBooster::SendAddBooster (0x14049340)
    static bool SaveBoosterData(CUser* pUser, ST_BOOSTER_OUTPUT* pBooster,
                                DBSaveCallback callback = nullptr);

    // 25. SaveAkashicRecordData - Save Akashic Record data to database
    // IDA: CGocAkashicRecord::SendDBAkashicRecordLoad (0x14017DB0)
    static bool SaveAkashicRecordData(CUser* pUser, DBSaveCallback callback = nullptr);

private:
    // Helper to get GOC component
    template<typename T>
    static T* GetComponent(CUser* pUser);
};
