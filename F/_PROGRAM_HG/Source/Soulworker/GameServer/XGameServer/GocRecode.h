#pragma once

// GocRecode.h
// Game Object Component - Record/Ranking System for SoulWorker GameServer
// Recovered from IDA GameServer.exe port 10004
//
// Chinese: 游戏对象组件 - 记录/排名系统

#include <cstdint>
#include <map>
#include <vector>

// Forward declarations
class CUser;
class CGocEntity;
class XGameServer;
class XResourceMgr;
struct TB_RANK_INFO;
struct TB_RANK_REWARD;

// ============================================================================
// Ranking Key Structure
// IDA: UST_RANKING_KEY
// ============================================================================
struct UST_RANKING_KEY {
    unsigned short wRankInfoIndex;
    unsigned short wYear;
    unsigned short wMonth;
    unsigned short wDay;
    
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
    unsigned short wRankInfoIndex;
    unsigned int dwUAID;
    unsigned int dwUCID;
    int nRank;
    int nMaxRank;
    int nPlayCount;
    int nScore;
    unsigned char byClass;
    unsigned char byLevel;
    unsigned char byLastReward;
    unsigned char byPadding;
    unsigned int dwProfilePhotoID;
    wchar_t strName[21];
    wchar_t strLeagueName[10];
    
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
    ST_USER_RANKING_INFO stInfo;
    unsigned long long dw64SeasonSetCount;
};

// ============================================================================
// Ranking Info Structure
// IDA: ST_RANKING_INFO
// ============================================================================
struct ST_RANKING_INFO {
    unsigned short wRankInfoIndex;
    int nTopRank;
    unsigned char byType;
    unsigned char byClassType;
    unsigned char byRewradType;
    unsigned char byClass;
    unsigned int dwMazeID;
};

// ============================================================================
// Ranking List Data Structure
// IDA: RANKING_LIST_DATA
// ============================================================================
struct RANKING_LIST_DATA {
    unsigned long long dw64SetCount;
    std::vector<ST_USER_RANKING_INFO> vecList;
};

// ============================================================================
// CGocRecode - Game Object Component for Records/Rankings
// IDA: CGocRecode class
// ============================================================================
class CGocRecode {
public:
    // Constructor & Destructor
    CGocRecode();
    virtual ~CGocRecode();
    
    // Ranking Functions - IDA Decompiled
    
    // IDA: 0x140146D80
    // Get rank based on play points
    void GetRank(float fPlay_Point);
    
    // IDA: 0x140154150
    // Update ranking data for maze completion
    void RankingDataUpdate(unsigned int dwMazeID, unsigned long dwMazePlayTime, int nMonsterKillScore);
    
    // IDA: 0x1401553D0
    // Check if user can receive ranking reward
    unsigned int CanRecvRankingReward(unsigned short wRankInfoIndex, ST_USER_LAST_RANKING_INFO& stLastRank);
    
    // IDA: 0x1401554B0
    // Set user's ranking information
    void SetRankingMyInfo(unsigned short wRankInfoIndex, bool bLastRanking, ST_USER_RANKING_INFO& stMyInfo, unsigned long long dw64SeasonSetCount);
    
    // IDA: 0x140155BC0
    // Request ranking list from database
    bool ReqRankingList(struct PS_RANKING_LIST_REQ* psReq);
    
    // IDA: 0x140156540
    // Response for user's ranking information from database
    void ResRankingMyInfo(struct PS_DB_MY_RANKING_INFO_RES* psRes);
    
    // IDA: 0x1401569D0
    // Request ranking reward
    void ReqRankingReward(struct PS_RANKING_REWARD_REQ* psReward);
    
    // IDA: 0x140158410
    // Response for ranking reward from database
    void ResRankingReward(struct PS_DB_RANKING_REWARD* psRes);
    
    // IDA: 0x14015BB20
    // Ranking cheat function (GM command)
    bool Ranking_Cheat(int nType, int nValue1, unsigned int nValue2);
    
    // Helper function for reward calculation
    void SetRewardValue(float fPlayPoint, int nRank, int nEXP, int nMoney);
    
    // Member variables - IDA recovered
    std::map<unsigned short, ST_USER_RANKING_INFO> m_mapMyRanking;
    std::map<unsigned short, ST_USER_LAST_RANKING_INFO> m_mapMyLastRanking;
    unsigned long long m_dw64RankingListTick[4];
    int m_nMazeID;
    int m_bShowCutscene;
    
private:
    // Internal helper functions
    CUser* GetOwnerUser();
};
