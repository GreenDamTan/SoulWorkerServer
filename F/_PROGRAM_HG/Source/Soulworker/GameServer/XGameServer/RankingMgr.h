#pragma once

// RankingMgr.h
// Ranking Manager System for SoulWorker GameServer
// Recovered from IDA GameServer.exe port 10004
//
// Chinese: 排名管理系统

#include <cstdint>
#include <map>
#include <vector>
#include "GocRecode.h"

// Forward declarations
class XGameServer;
class XResourceMgr;
struct TB_RANK_INFO;

// ============================================================================
// CRankingMgr - Ranking Manager
// IDA: CRankingMgr class
// ============================================================================
class CRankingMgr {
public:
    // Constructor & Destructor
    CRankingMgr();
    virtual ~CRankingMgr();
    
    // IDA: 0x140639260
    // Clear all ranking data
    void Clear();
    
    // IDA: 0x1406392E0
    // Set ranking list from database response
    void SetRankingList(struct PS_DB_RANKING_LIST_RES* psList);
    
    // IDA: 0x140639CB0
    // Get ranking list for specified index/class
    bool GetRankingList(unsigned short wRankInfoIndex,
                        unsigned char byClass,
                        bool bLastRanking,
                        std::vector<ST_USER_RANKING_INFO>& vecList);
    
    // IDA: 0x140639E50
    // Load ranking list request from database
    void LoadRankingListReq(unsigned long long dw64Tick);
    
    // IDA: 0x14063A2E0
    // Get last reward ID date
    unsigned int GetLastRewardID_Date();
    
    // IDA: 0x14063A5D0
    // Get starting reward ID for ranking index
    unsigned int GetLastRewardID_Start(unsigned short wRankInfoIndex);
    
    // IDA: 0x14063A6E0
    // Delete all ranking lists
    void DeleteList_All();
    
    // Member variables - IDA recovered
    unsigned long long m_dw64LoadRankingList;
    std::vector<struct ST_RANKING_INFO> m_vecRankInfoList;
    std::map<UST_RANKING_KEY, RANKING_LIST_DATA> m_mapRankingList;
    std::map<UST_RANKING_KEY, RANKING_LIST_DATA> m_mapLastRankingList;
    std::map<UST_RANKING_KEY, std::vector<ST_USER_RANKING_INFO>> m_mapTempRankingList;
    std::map<UST_RANKING_KEY, std::vector<ST_USER_RANKING_INFO>> m_mapTempLastRankingList;
};
