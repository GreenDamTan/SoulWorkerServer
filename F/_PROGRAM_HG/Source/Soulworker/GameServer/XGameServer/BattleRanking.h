#pragma once

// BattleRanking.h
// Battle Arena Ranking and Score System for SoulWorker GameServer
// Recovered from IDA GameServer.exe port 10004
// 
// Chinese: 战斗竞技场排名与积分系统

#include <cstdint>

// Forward declarations
class CUser;
class CGocRecode;
class CGocEntity;
class XGameServer;
class XResourceMgr;
class XPacket;

// ============================================================================
// Ranking Types
// IDA: TB_RANK_INFO::Ranking_Category
// ============================================================================
enum ERankingCategory : int {
    RANKING_CATEGORY_TOTAL = 1,      // Total ranking (total points)
    RANKING_CATEGORY_PERSONAL = 2,   // Personal ranking (individual points)
    RANKING_CATEGORY_SEASON = 3,     // Season ranking
};

// ============================================================================
// Ranking Total Types
// IDA: TB_RANK_INFO::Ranking_Total_Type
// ============================================================================
enum ERankingTotalType : int {
    RANKING_TOTAL_TYPE_1 = 1,        // For category 1 and 3
    RANKING_TOTAL_TYPE_4 = 4,        // For category 2
};

// ============================================================================
// Ranking Error Codes
// IDA: From CGocRecode::CanRecvRankingReward and related functions
// ============================================================================
enum ERankingError : int {
    RANKING_ERROR_SUCCESS = 0,
    RANKING_ERROR_NOT_FOUND = 58502,        // User not found
    RANKING_ERROR_ALREADY_RECEIVED = 58503, // Already received reward
    RANKING_ERROR_INVALID_RANK = 58504,     // Invalid rank
    RANKING_ERROR_ITEM_FAILED = 52010,      // Item creation failed
    RANKING_ERROR_DB_FAILED = 58502,        // Database operation failed
    RANKING_ERROR_INVALID_TABLE = 58502,    // Invalid table data
    RANKING_ERROR_NOT_ELIGIBLE = 58503,     // Not eligible for reward
};

// ============================================================================
// Ranking Key Structure
// IDA: UST_RANKING_KEY
// ============================================================================
struct UST_RANKING_KEY {
    unsigned short wRankInfoIndex;    // Ranking info index
    unsigned short wYear;             // Year
    unsigned short wMonth;            // Month
    unsigned short wDay;              // Day
    
    bool operator<(const UST_RANKING_KEY& other) const {
        if (wRankInfoIndex != other.wRankInfoIndex) return wRankInfoIndex < other.wRankInfoIndex;
        if (wYear != other.wYear) return wYear < other.wYear;
        if (wMonth != other.wMonth) return wMonth < other.wMonth;
        return wDay < other.wDay;
    }
};

// ============================================================================
// User Ranking Info Structure
// IDA: ST_USER_RANKING_INFO
// ============================================================================
struct ST_USER_RANKING_INFO {
    unsigned int dwUAID;              // User account ID
    unsigned int dwUCID;              // User character ID
    int nRank;                        // Current rank
    int nMaxRank;                     // Maximum rank achieved
    unsigned long long dw64Score;     // Score/points
    unsigned long long dw64PlayTime;  // Play time
    unsigned long long dw64ClearCount; // Clear count
    unsigned long long dw64KillCount;  // Kill count
    char strName[51];                 // Character name (Unicode)
    unsigned char byClass;            // Character class
    unsigned char byLevel;            // Character level
    unsigned char byAwaken;           // Awaken level
    unsigned char byPadding;          // Padding
    
    // Get percentage rank for reward calculation
    // IDA: ST_USER_RANKING_INFO::GetPercentRank
    unsigned short GetPercentRank() const {
        if (nMaxRank <= 0) return 100;
        return (unsigned short)((nRank * 100) / nMaxRank);
    }
};

// ============================================================================
// User Last Ranking Info Structure
// IDA: ST_USER_LAST_RANKING_INFO
// ============================================================================
struct ST_USER_LAST_RANKING_INFO {
    ST_USER_RANKING_INFO stInfo;            // Ranking info
    unsigned long long dw64SeasonSetCount;  // Season set count
    unsigned long long dw64LastDate;        // Last update date
};

// ============================================================================
// Ranking List Data Structure
// IDA: RANKING_LIST_DATA
// ============================================================================
struct RANKING_LIST_DATA {
    unsigned short wRankInfoIndex;          // Ranking info index
    unsigned long long dw64UpdateDate;      // Update date
    unsigned long long dw64SeasonSetCount;  // Season set count
    // std::vector<ST_USER_RANKING_INFO> vecRankingList; // Ranking list
    void* pRankingList;                     // Placeholder
};

// ============================================================================
// Battle Ranking System Class
// Static helper functions for ranking functionality
// ============================================================================
class BattleRanking {
public:
    // ========================================================================
    // Ranking Data Update Functions
    // ========================================================================
    
    // IDA: ?RankingDataUpdate@CGocRecode@@QEAAXKKH@Z (0x140154150)
    // Chinese: 更新排名数据 - 更新玩家的排名积分和统计数据
    // Parameters:
    //   dwMazeID - 迷宫ID
    //   dwMazePlayTime - 迷宫游玩时间
    //   nMonsterKillScore - 怪物击杀分数
    static void RankingDataUpdate(CGocRecode* pRecode, 
                                   unsigned long dwMazeID,
                                   unsigned long dwMazePlayTime,
                                   int nMonsterKillScore);
    
    // ========================================================================
    // Ranking Reward Functions
    // ========================================================================
    
    // IDA: ?CanRecvRankingReward@CGocRecode@@QEAAHGAEAUST_USER_LAST_RANKING_INFO@@@Z (0x1401553D0)
    // Chinese: 检查是否可以领取排名奖励
    // Returns error code (0 = success, others = error codes)
    static int CanRecvRankingReward(CGocRecode* pRecode, 
                                     unsigned short wRankInfoIndex,
                                     ST_USER_LAST_RANKING_INFO* pLastRanking);
    
    // IDA: ?ReqRankingReward@CGocRecode@@QEAAXAEAUPS_RANKING_REWARD_REQ@@@Z (0x1401569D0)
    // Chinese: 请求排名奖励
    // Validates user, checks eligibility, creates reward items, sends DB request
    static void ReqRankingReward(CGocRecode* pRecode, void* pRewardReq);
    
    // IDA: ?ResRankingReward@CGocRecode@@QEAAXAEAUPS_DB_RANKING_REWARD@@@Z (0x140158410)
    // Chinese: 响应排名奖励结果
    // Handles DB response, sends reward to user, logs the transaction
    static void ResRankingReward(CGocRecode* pRecode, void* pDBReward);
    
    // ========================================================================
    // Ranking Query Functions
    // ========================================================================
    
    // IDA: ?ReqRankingList@CGocRecode@@QEAA_NAEAUPS_RANKING_LIST_REQ@@@Z (0x140155BC0)
    // Chinese: 请求排名列表
    // Queries ranking list for specified ranking type and date range
    static bool ReqRankingList(CGocRecode* pRecode, void* pRankingListReq);
    
    // IDA: ?SetRankingMyInfo@CGocRecode@@QEAAXG_NAEAUST_USER_RANKING_INFO@@_K@Z (0x1401554B0)
    // Chinese: 设置我的排名信息
    // Updates user's personal ranking information
    static void SetRankingMyInfo(CGocRecode* pRecode,
                                  unsigned short wRankInfoIndex,
                                  bool bUpdate,
                                  ST_USER_RANKING_INFO* pInfo,
                                  unsigned long long dwData);
    
    // IDA: ?ResRankingMyInfo@CGocRecode@@QEAAXAEAUPS_DB_MY_RANKING_INFO_RES@@@Z (0x140156540)
    // Chinese: 响应我的排名信息
    // Handles DB response for personal ranking query
    static void ResRankingMyInfo(CGocRecode* pRecode, void* pMyRankingRes);
    
    // ========================================================================
    // GM/Cheat Commands
    // ========================================================================
    
    // IDA: ?Ranking_Cheat@CGocRecode@@QEAA_NHHH@Z (0x14015BB20)
    // Chinese: GM 命令设置排名 (测试用)
    // Sets ranking manually for testing purposes
    static bool Ranking_Cheat(CGocRecode* pRecode, 
                               int nType,
                               int nRank,
                               int nScore);
    
    // ========================================================================
    // Ranking for Matching System
    // ========================================================================
    
    // IDA: ?ResRankingOperationForMatching@CRankingProcess@@QEAA_NAEAVXPacket@@@Z (0x14059FEA0)
    // Chinese: 为匹配系统响应排名操作
    // Handles ranking operations for party/force matching system
    static bool ResRankingOperationForMatching(class CRankingProcess* pProcess, XPacket* pPacket);
    
    // ========================================================================
    // Helper Functions
    // ========================================================================
    
    // Calculate reward based on rank
    // Returns reward ID from TB_RANK_REWARD table
    static unsigned int CalculateReward(unsigned short wRankInfoIndex, 
                                        int nRank, 
                                        int nMaxRank,
                                        unsigned short wPercentRank);
    
    // Get last reward ID start index for ranking
    static unsigned int GetLastRewardID_Start(unsigned short wRankInfoIndex);
    
    // Get last reward date
    static unsigned long long GetLastRewardID_Date();
};

// ============================================================================
// Internal Implementation Notes (from IDA decompilation)
// ============================================================================

/*
From IDA decompilation analysis:

1. CGocRecode::RankingDataUpdate (0x140154150):
   Complex function that:
   - Retrieves user and entity information
   - Validates maze ID and play time
   - Updates multiple ranking categories:
     * Time ranking (fastest clear time)
     * Clear count ranking
     * Monster kill ranking
   - Calculates league bonus
   - Sends DB update packet
   - Logs game event
   
2. CGocRecode::ReqRankingReward (0x1401569D0):
   Very complex function that:
   - Validates user UCID matches representative UCID
   - Retrieves TB_RANK_INFO for ranking type
   - Checks Ranking_Reward_Receive flag
   - Calls CanRecvRankingReward to validate eligibility
   - Calculates percentage rank
   - Finds appropriate TB_RANK_REWARD entry
   - Validates reward items in TB_ITEM
   - Creates reward items with class-specific bonuses
   - Sets expiration date for class reward items (13 days 23:59:59)
   - Sends DB packet and client response
   - Logs transaction
   
3. CGocRecode::ResRankingReward (0x140158410):
   Response handler that:
   - Checks for DB errors
   - Updates inventory (CreateItem2)
   - Sends update/create item packets to client
   - Logs the reward transaction
   
4. ST_USER_RANKING_INFO::GetPercentRank:
   - Returns (nRank * 100) / nMaxRank
   - Used to determine reward tier based on percentile
   
5. Ranking Categories:
   - Category 1 (Total): Uses Total_Type 1
   - Category 2 (Personal): Uses Total_Type 4
   - Category 3 (Season): Uses Total_Type 1
   
6. Reward System:
   - Up to 50 reward tiers checked (sh < 50 loop)
   - Max_Ranking_Type determines if percentile or absolute rank
   - Class-specific reward items (8 classes supported)
   - Expiration date: 13 days 23:59:59 from reward date
*/
