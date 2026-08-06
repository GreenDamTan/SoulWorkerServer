// GocRecode.cpp
// Game Object Component - Record/Ranking System Implementation
// Recovered from IDA GameServer.exe port 10004
//
// Chinese: 游戏对象组件 - 记录/排名系统实现

#include "GocRecode.h"
#include "User.h"
#include "actor/component/GocEntity.h"
#include "actor/component/GocInventory.h"
#include "../XSCommon/XSendPacket.h"
#include "../XSCommon/XSendDBPacket.h"
#include "../XSCommon/LogHelper.h"
#include <cfloat>
#include <vector>

// Forward declarations
class XGameServer;
class XResourceMgr;
struct TB_RANK_INFO;
struct TB_RANK_REWARD;
struct TB_ITEM;
struct TB_MAZEREWARD_STANDARD;
struct TB_MAZEREWARD_RANK;

// ============================================================================
// CGocRecode Implementation
// ============================================================================

CGocRecode::CGocRecode() 
    : m_nMazeID(0)
    , m_bShowCutscene(0) {
    memset(m_dw64RankingListTick, 0, sizeof(m_dw64RankingListTick));
}

CGocRecode::~CGocRecode() {
    m_mapMyRanking.clear();
    m_mapMyLastRanking.clear();
}

// ============================================================================
// IDA: 0x140146D80
// Chinese: 根据游玩积分获取排名
// ============================================================================
void CGocRecode::GetRank(float fPlay_Point) {
    // IDA decompiled logic:
    // 1. Get TB_MAZEREWARD_STANDARD table for this maze
    // 2. Check play points against rank thresholds (5 ranks)
    // 3. Get TB_MAZEREWARD_RANK for matching rank
    // 4. Call SetRewardValue with rank, EXP, and Money
    
    // TODO: Requires:
    // - XGameServer singleton access
    // - XResourceMgr::GetTB_MAZEREWARD_STANDARD
    // - XResourceMgr::GetTB_MAZEREWARD_RANK
    // - SetRewardValue implementation
    
    /* Original IDA pseudocode:
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_MAZEREWARD_STANDARD* pStandard = pServer->m_xResourceMgr.GetTB_MAZEREWARD_STANDARD(m_nMazeID);
    
    if (!pStandard) {
        LogHelper::LogError("GetRank error - No Table TB_MAZEREWARD_STANDARD");
        return;
    }
    
    for (int i = 0; i < 5; ++i) {
        if (pStandard->Rank_C_Point[i] >= fPlay_Point) {
            TB_MAZEREWARD_RANK* pRank = pServer->m_xResourceMgr.GetTB_MAZEREWARD_RANK(i);
            if (pRank) {
                SetRewardValue(fPlay_Point, i, pRank->EXP_Value, pRank->Money_Value);
                return;
            }
        }
    }
    
    // Rank 5 (lowest)
    TB_MAZEREWARD_RANK* pRank = pServer->m_xResourceMgr.GetTB_MAZEREWARD_RANK(5);
    if (pRank) {
        SetRewardValue(fPlay_Point, 5, pRank->EXP_Value, pRank->Money_Value);
    }
    */
}

// ============================================================================
// IDA: 0x140154150
// Chinese: 更新排名数据
// ============================================================================
void CGocRecode::RankingDataUpdate(unsigned int dwMazeID, 
                                     unsigned long dwMazePlayTime,
                                     int nMonsterKillScore) {
    // IDA: Complex function with ~4500 bytes
    // Updates three ranking categories: TIME, CLEAR_COUNT, MONSTER_KILL_SCORE
    // Sends DB packets and logs events
    
    // TODO: Full implementation requires:
    // - CUser retrieval
    // - CGocEntity for representative UCID
    // - TB_RANK_INFO table lookups
    // - PS_DB_RANKING_POINT_UPDATE packet construction
    // - XSendDBPacket to DB server
    // - ST_LOG_GAME event logging
    
    /* Key IDA pseudocode structure:
    
    CUser* pUser = GetOwnerUser();
    if (!pUser) return;
    
    CGocEntity* pEntity = GetGOC<CGocEntity>(pUser);
    if (!pEntity) return;
    
    unsigned int dwUCID = pEntity->GetRepresentativeUCID();
    
    // For each ranking type (TIME, CLEAR_COUNT, MONSTER_KILL):
    // - Get ranking info table
    // - For each ranking entry:
    //   - Create PS_DB_RANKING_POINT_UPDATE
    //   - Fill user and ranking info
    //   - Send to DB
    //   - Log event
    
    std::vector<TB_RANK_INFO*> vecTime;
    std::vector<TB_RANK_INFO*> vecClearCount;
    std::vector<TB_RANK_INFO*> vecMonsterKill;
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    pServer->m_xResourceMgr.GetRankingInfoTable(dwMazeID, E_RANKING_TOTAL_TYPE_TIME, &vecTime);
    pServer->m_xResourceMgr.GetRankingInfoTable(dwMazeID, E_RANKING_TOTAL_TYPE_CLEAR_COUNT, &vecClearCount);
    pServer->m_xResourceMgr.GetRankingInfoTable(dwMazeID, E_RANKING_TOTAL_TYPE_MONSTER_KILL_SOCRE, &vecMonsterKill);
    
    // Process each vector...
    */
}

// ============================================================================
// IDA: 0x1401553D0
// Chinese: 检查是否可以接收排名奖励
// ============================================================================
unsigned int CGocRecode::CanRecvRankingReward(unsigned short wRankInfoIndex,
                                                ST_USER_LAST_RANKING_INFO& stLastRank) {
    // IDA: Check if user can receive ranking reward
    // Returns error code or 0 for success
    
    auto it = m_mapMyLastRanking.find(wRankInfoIndex);
    if (it == m_mapMyLastRanking.end()) {
        return 58503; // Not found
    }
    
    stLastRank = it->second;
    
    if (stLastRank.stInfo.nRank <= 0) {
        return 58503; // Invalid rank
    }
    
    if (stLastRank.stInfo.byLastReward) {
        return 0; // Already received
    }
    
    return 58504; // Can receive
}

// ============================================================================
// IDA: 0x1401554B0
// Chinese: 设置我的排名信息
// ============================================================================
void CGocRecode::SetRankingMyInfo(unsigned short wRankInfoIndex,
                                    bool bLastRanking,
                                    ST_USER_RANKING_INFO& stMyInfo,
                                    unsigned long long dw64SeasonSetCount) {
    // IDA: Set user's ranking info in local cache
    // Handles both current and last season rankings
    
    CUser* pUser = GetOwnerUser();
    if (!pUser) return;
    
    // TODO: Get TB_RANK_INFO and fill user data
    // TODO: Handle league rankings (category 2)
    // TODO: Update m_mapMyRanking or m_mapMyLastRanking
    
    /* Key IDA pseudocode:
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_RANK_INFO* pRankInfo = pServer->m_xResourceMgr.GetTB_RANK_INFO(wRankInfoIndex);
    if (!pRankInfo) return;
    
    CGocEntity* pEntity = GetGOC<CGocEntity>(pUser);
    if (!pEntity) return;
    
    // Fill stMyInfo with user data...
    
    if (bLastRanking) {
        auto it = m_mapMyLastRanking.find(wRankInfoIndex);
        if (it != m_mapMyLastRanking.end()) {
            // Update existing
            it->second.stInfo = stMyInfo;
            it->second.dw64SeasonSetCount = dw64SeasonSetCount;
        } else {
            // Insert new
            ST_USER_LAST_RANKING_INFO lastInfo;
            lastInfo.stInfo = stMyInfo;
            lastInfo.dw64SeasonSetCount = dw64SeasonSetCount;
            m_mapMyLastRanking[wRankInfoIndex] = lastInfo;
        }
    } else {
        m_mapMyRanking[wRankInfoIndex] = stMyInfo;
    }
    */
}

// ============================================================================
// IDA: 0x140155BC0
// Chinese: 请求排名列表
// ============================================================================
bool CGocRecode::ReqRankingList(struct PS_RANKING_LIST_REQ* psReq) {
    // IDA: Request ranking list from database
    // Validates ranking type, class, maze, etc.
    // Sends PS_DB_MY_RANKING_INFO_REQ to database
    
    CUser* pUser = GetOwnerUser();
    if (!pUser) return false;
    
    // TODO: Full validation per IDA:
    // - Check TB_RANK_INFO exists
    // - Validate Ranking_Total_Type (1-5)
    // - Validate Ranking_Total_Class_Type (0 or 1)
    // - Validate Ranking_Visible (max 100)
    // - Validate Ranking_Category (1-3)
    // - Check maze exists
    // - Rate limit check (m_dw64RankingListTick)
    
    /* Key IDA pseudocode:
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_RANK_INFO* pRankInfo = pServer->m_xResourceMgr.GetTB_RANK_INFO(psReq->wRankInfoIndex);
    if (!pRankInfo) {
        LogHelper::LogError("ReqRankingList - Error Table");
        CGocNetwork::SendErrorMessage(pUser, 0x2C, 1, 0xE484);
        return false;
    }
    
    // Validate type
    if (pRankInfo->Ranking_Total_Type < 1 || pRankInfo->Ranking_Total_Type > 5) {
        LogHelper::LogError("ReqRankingList - Error Ranking_Total_Type");
        CGocNetwork::SendErrorMessage(pUser, 0x2C, 1, 0xE484);
        return false;
    }
    
    // Rate limit
    if (m_dw64RankingListTick[pRankInfo->Ranking_Category] > GetTickCount64()) {
        LogHelper::LogError("ReqRankingList - Already Request");
        CGocNetwork::SendErrorMessage(pUser, 0x2C, 1, 0xE489);
        return false;
    }
    
    m_dw64RankingListTick[pRankInfo->Ranking_Category] = GetTickCount64() + 1000;
    
    // Send DB request
    PS_DB_MY_RANKING_INFO_REQ psDBReq;
    // Fill request...
    
    XSendDBPacket xSendPacket(pUser, 0x28, 0x11);
    xSendPacket << psDBReq;
    pServer->SendDBGame(&xSendPacket);
    
    return true;
    */
    
    return false;
}

// ============================================================================
// IDA: 0x140156540
// Chinese: 响应我的排名信息
// ============================================================================
void CGocRecode::ResRankingMyInfo(struct PS_DB_MY_RANKING_INFO_RES* psRes) {
    // IDA: Handle database response for ranking info
    // Updates local cache and sends ranking list to client
    
    CUser* pUser = GetOwnerUser();
    if (!pUser) return;
    
    if (psRes->stMyInfo.nRank == -1) {
        LogHelper::LogError("ResRankingMyInfo - DB Error");
        // CGocNetwork::SendErrorMessage(pUser, 0x2C, 1, 0xE484);
        return;
    }
    
    // Update local cache
    SetRankingMyInfo(psRes->stRankingInfo.wRankInfoIndex, false, psRes->stMyInfo, 0);
    SetRankingMyInfo(psRes->stRankingInfo.wRankInfoIndex, true, psRes->stMySeasonInfo, psRes->dw64SeasonSetCount);
    
    // TODO: Get ranking list from CRankingMgr
    // TODO: Send PS_RANKING_LIST_RES to client in chunks (20 per packet)
    
    /* Key IDA pseudocode:
    
    PS_RANKING_LIST_RES psRankingList;
    psRankingList.wRankInfoIndex = psRes->stRankingInfo.wRankInfoIndex;
    psRankingList.byClass = psRes->stRankingInfo.byClass;
    psRankingList.bLastRanking = psRes->stMyReq.bLastRanking;
    
    if (psRankingList.bLastRanking) {
        psRankingList.stMyInfo = psRes->stMySeasonInfo;
    } else {
        psRankingList.stMyInfo = psRes->stMyInfo;
        psRankingList.stMyInfo.byLastReward = psRes->stMySeasonInfo.byLastReward;
    }
    
    std::vector<ST_USER_RANKING_INFO> vecList;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    pServer->m_RankingMgr.GetRankingList(
        psRankingList.wRankInfoIndex,
        psRankingList.byClass,
        psRankingList.bLastRanking,
        &vecList
    );
    
    // Send in chunks of 20
    for (size_t i = 0; i < vecList.size(); ++i) {
        psRankingList.vecList.push_back(vecList[i]);
        
        if (psRankingList.vecList.size() >= 20) {
            psRankingList.bLast = (i == vecList.size() - 1);
            XSendPacket packet(0x2C, 1);
            packet << psRankingList;
            CGocNetwork::Send(pUser, &packet);
            psRankingList.vecList.clear();
        }
    }
    
    // Send remaining
    if (!psRankingList.vecList.empty()) {
        psRankingList.bLast = 1;
        XSendPacket packet(0x2C, 1);
        packet << psRankingList;
        CGocNetwork::Send(pUser, &packet);
    }
    */
}

// ============================================================================
// IDA: 0x1401569D0
// Chinese: 请求排名奖励
// ============================================================================
void CGocRecode::ReqRankingReward(struct PS_RANKING_REWARD_REQ* psReward) {
    // IDA: Very large function (~6600 bytes)
    // Validates eligibility, calculates rewards, creates items, sends to DB
    
    CUser* pUser = GetOwnerUser();
    if (!pUser) return;
    
    // TODO: Full implementation:
    // 1. Validate representative UCID
    // 2. Check TB_RANK_INFO
    // 3. Check CanRecvRankingReward
    // 4. Get TB_RANK_REWARD
    // 5. Create reward items
    // 6. Send DB update
    
    /* Key IDA pseudocode structure:
    
    CGocEntity* pEntity = GetGOC<CGocEntity>(pUser);
    if (pEntity->GetRepresentativeUCID() != pUser->GetUCID()) {
        LogHelper::LogError("ReqRankingReward - No RepresentativeUCID Error");
        CGocNetwork::SendErrorMessage(pUser, 0x2C, 2, 0xE486);
        return;
    }
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_RANK_INFO* pRankInfo = pServer->m_xResourceMgr.GetTB_RANK_INFO(psReward->wRankInfoIndex);
    if (!pRankInfo) {
        LogHelper::LogError("ReqRankingReward - TB_RANK_INFO Table Error");
        CGocNetwork::SendErrorMessage(pUser, 0x2C, 2, 0xE486);
        return;
    }
    
    ST_USER_LAST_RANKING_INFO stMyLastRanking;
    int nError = CanRecvRankingReward(psReward->wRankInfoIndex, stMyLastRanking);
    if (nError) {
        CGocNetwork::SendErrorMessage(pUser, 0x2C, 2, nError);
        return;
    }
    
    // Find matching TB_RANK_REWARD
    TB_RANK_REWARD* pReward = nullptr;
    unsigned short wPercentRank = stMyLastRanking.stInfo.GetPercentRank();
    unsigned int dwRewardID_Start = pServer->m_RankingMgr.GetLastRewardID_Start(psReward->wRankInfoIndex);
    
    for (int i = 0; i < 50; ++i) {
        TB_RANK_REWARD* pCheck = pServer->m_xResourceMgr.GetTB_RANK_REWARD(dwRewardID_Start + i);
        if (!pCheck) break;
        
        if (pCheck->Max_Ranking_Type) {
            if (wPercentRank <= pCheck->Max_Ranking) {
                pReward = pCheck;
                break;
            }
        } else {
            if (stMyLastRanking.stInfo.nRank <= pCheck->Max_Ranking) {
                pReward = pCheck;
                break;
            }
        }
    }
    
    if (!pReward) {
        CGocNetwork::SendErrorMessage(pUser, 0x2C, 2, 0xE487);
        return;
    }
    
    // Create reward items
    PS_DB_RANKING_REWARD psDB;
    psDB.dwRewardID = pReward->Reward_Index;
    psDB.dw64SeasonSetCount = stMyLastRanking.dw64SeasonSetCount;
    psDB.nRank = stMyLastRanking.stInfo.nRank;
    psDB.nMaxRank = stMyLastRanking.stInfo.nMaxRank;
    
    // Add items from reward table
    for (int i = 0; i < 2; ++i) {
        if (pReward->Ranking_Reward_Item_01[i] && pReward->Item_Amount_01[i]) {
            ST_CREATE_ITEM stItem;
            stItem.nItemID = pReward->Ranking_Reward_Item_01[i];
            stItem.shCount = pReward->Item_Amount_01[i];
            psDB.vecRewardItem.push_back(stItem);
        }
    }
    
    // Add class-specific item
    unsigned char byClass = pUser->GetClass() - 1;
    if (byClass < 8 && pReward->Ranking_Class_Reward_Item_01[byClass]) {
        ST_CREATE_ITEM stItem;
        stItem.nItemID = pReward->Ranking_Class_Reward_Item_01[byClass];
        stItem.shCount = 1;
        psDB.vecRewardItem.push_back(stItem);
    }
    
    // Create items in inventory
    CGocInventory* pInventory = GetGOC<CGocInventory>(pUser);
    if (!pInventory->CreateItem2(&psDB.vecRewardItem, ...)) {
        LogHelper::LogError("ReqRankingReward error - CreateItem2");
        CGocNetwork::SendErrorMessage(pUser, 0x2C, 2, 0xCB2A);
        return;
    }
    
    // Send to DB
    XSendDBPacket xSendDBPacket(pUser, 0x28, 0x13);
    xSendDBPacket << psDB;
    pServer->SendDBGame(&xSendDBPacket);
    */
}

// ============================================================================
// IDA: 0x140158410
// Chinese: 响应排名奖励
// ============================================================================
void CGocRecode::ResRankingReward(struct PS_DB_RANKING_REWARD* psRes) {
    // IDA: Handle database response for reward
    // Updates items and sends confirmation to client
    
    CUser* pUser = GetOwnerUser();
    if (!pUser) return;
    
    if (psRes->nError) {
        LogHelper::LogError("ResRankingReward DB Error");
        // CGocNetwork::SendErrorMessage(pUser, 0x2C, 2, 0xE486);
        return;
    }
    
    // Mark as received
    auto it = m_mapMyLastRanking.find(psRes->stReq.wRankInfoIndex);
    if (it != m_mapMyLastRanking.end()) {
        it->second.stInfo.byLastReward = 1;
    }
    
    // TODO: Send update/create item packets to client
    // TODO: Send PS_RANKING_REWARD_RES to client
    // TODO: Log event
    
    /* Key IDA pseudocode:
    
    PS_RANKING_REWARD_RES psReward;
    psReward.dwUCID = pUser->GetUCID();
    psReward.wRankInfoIndex = psRes->stReq.wRankInfoIndex;
    psReward.dwRewardID = psRes->dwRewardID;
    psReward.nRank = psRes->nRank;
    psReward.nMaxRank = psRes->nMaxRank;
    
    CGocInventory* pInventory = GetGOC<CGocInventory>(pUser);
    pInventory->SendUpdateItem(psRes->stUpdateItem);
    pInventory->SendCreateItem(psRes->stCreateItem);
    
    XSendPacket xSendPacket(0x2C, 2);
    xSendPacket << psReward;
    CGocNetwork::Send(pUser, &xSendPacket);
    
    ST_LOG_GAME stLog;
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetUCID();
    stLog._sMainType = 30;
    stLog._sSubType = 1;
    stLog.nParam0 = psReward.wRankInfoIndex;
    stLog.nParam1 = psReward.dwRewardID;
    stLog.nParam2 = psReward.nRank;
    stLog.nParam3 = psReward.nMaxRank;
    stLog.nParam4 = pUser->GetLevel();
    stLog.nParam5 = pUser->GetClass();
    wcscpy_s(stLog.szComment, L"Ranking Reward Received");
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    pServer->SendDBLog(&stLog);
    */
}

// ============================================================================
// IDA: 0x14015BB20
// Chinese: 排名作弊函数
// ============================================================================
bool CGocRecode::Ranking_Cheat(int nType, int nValue1, unsigned int nValue2) {
    // IDA: GM command for ranking operations
    // Types: 0=Reset all, 1=Insert dummy, 2=Season reset, 3=Season operation, 4=Season matching
    
    CUser* pUser = GetOwnerUser();
    if (!pUser) return false;
    
    // TODO: Check system type and GM power
    // TODO: Send appropriate DB packet
    
    /* Key IDA pseudocode:
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer->GetOption()->GetSystemType() == 3) return false;
    if (!pUser->GetGMPower()) return false;
    
    switch (nType) {
        case 0: // Reset all rankings
            pServer->m_RankingMgr.DeleteList_All();
            XSendDBPacket packet(pUser, 0x28, 0x21);
            pServer->SendDBGame(&packet);
            return true;
            
        case 1: // Insert dummy ranking
            TB_RANK_INFO* pRankInfo = pServer->m_xResourceMgr.GetTB_RANK_INFO(nValue1);
            if (!pRankInfo || pRankInfo->Ranking_Total_Type != 1) return false;
            
            XSendDBPacket packet(pUser, 0x28, 0x22);
            packet << pUser->GetUAID();
            packet << pUser->GetUCID();
            packet << (pRankInfo->Ranking_Total_Class_Type + 10 * pRankInfo->Ranking_Total_Type);
            packet << pRankInfo->Ranking_Maze;
            packet << nValue2;
            packet << pRankInfo->Ranking_Category;
            pServer->SendDBGame(&packet);
            return true;
            
        case 2: // Season reset
            if (nValue1 != 1 && nValue1 != 3) return false;
            XSendDBPacket packet(pUser, 0x28, 0x23);
            packet << nValue1;
            pServer->SendDBGame(&packet);
            return true;
            
        case 3: // Season operation
            if (nValue1 != 1 && nValue1 != 3) return false;
            if (nValue2 >= 2) return false;
            XSendDBPacket packet(pUser, 0x28, 0x24);
            packet << nValue1;
            packet << (nValue2 == 1);
            pServer->SendDBGame(&packet);
            return true;
            
        case 4: // Season matching
            if (nValue1 < 1 || nValue1 > 3) return false;
            XSendDBPacket packet(pUser, 0x28, 0x25);
            packet << nValue1;
            packet << (nValue2 == 1);
            pServer->SendDBGame(&packet);
            return true;
            
        default:
            return false;
    }
    */
    
    return false;
}

// ============================================================================
// Helper function
// Chinese: 设置奖励值
// ============================================================================
void CGocRecode::SetRewardValue(float fPlayPoint, int nRank, int nEXP, int nMoney) {
    // TODO: Store or apply reward values
    // Called from GetRank
}

// ============================================================================
// Internal helper
// Chinese: 获取所属用户
// ============================================================================
CUser* CGocRecode::GetOwnerUser() {
    // TODO: Get CUser from owner object
    // Requires casting from component owner
    return nullptr;
}
