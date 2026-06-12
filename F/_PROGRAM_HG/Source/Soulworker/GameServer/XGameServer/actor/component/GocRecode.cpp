// GocRecode.cpp - CGocRecode implementation
// Maze recording and reward calculation component
//
// IDA Analysis from GameServer.exe:
// - Constructor: 0x140144B90
// - Destructor: 0x140144DD0
// - Init: 0x140144EE0
// - Clear: 0x1401451B0
// - InitMaze: 0x140144F00
// - MazeReward: 0x140145930
// - OnUpdate: 0x1401455A0

#include "GocRecode.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMaze.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstdio>

// Temporary logging macros until GreenDamTan_LogHelper.h is available
// Note: First argument (category) is ignored in this placeholder
#define GreenDamTan_log_debug(cat, fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define GreenDamTan_log_info(cat, fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)
#define GreenDamTan_log_error(cat, fmt, ...) printf("[ERROR] " fmt "\n", ##__VA_ARGS__)

// Forward declarations for external types
class CMover;
class CUser;
class XGameServer;

// External global variables
extern int g_nRewardRateTotal[6];  // Reward rate totals for each rank (0-5)

// TODO: 需要外部依赖 - 以下头文件需要后续添加
// #include "Mover.h"
// #include "User.h"
// #include "GameServer.h"
// #include "GocAttribute.h"
// #include "GocInventory.h"
// #include "GocPost.h"
// #include "GocQuest.h"
// #include "GocAchieve.h"

// Placeholder for missing structs
// IDA: ST_MAZE_CLEAR_INFO (40 bytes)
struct ST_MAZE_CLEAR_INFO {
    int nMazeGroup = 0;                    // offset 0
    int _pad0 = 0;                         // offset 4 (padding)
    std::vector<int> vecClearMazeList;     // offset 8, size 32 (vector)
};
struct ST_MAZE_CASH_REWARD_INFO {};
struct ST_RANDOM_BUFF {};
struct PS_REWARD_PARTY_SCORE {
    unsigned char byOrder;
    unsigned char byRank;
    unsigned int dwMemberID;
    wchar_t strMemberName[64];
    int nPlayPoint;
};
// PS_MAZE_ENTER_LIMIT_COUNT_GROUP, PS_UPDATE_MAZE_ENTER_LIMIT_COUNT, ST_USER_RANKING_INFO, ST_USER_LAST_RANKING_INFO, PS_KILLED_USER_INFOS
// are defined in PSServerMaze.h and PSServerDB.h

// External function declarations - TODO: 需要从其他模块导入
// These are placeholder implementations for now

namespace {
    // Helper to get XGameServer instance
    // IDA: TXSingleton<XGameServer>::Instance @ 0x140001450
    XGameServer* GetGameServerInstance() {
        return TXSingleton<XGameServer>::Instance();
    }
}

// Helper to get owner user
CUser* CGocRecode::GetOwnerUser() const {
    // TODO: Implement when CUser is available
    // IDA shows: _RTDynamicCast_0 with CMover and CUser type descriptors
    return nullptr;
}

// ============================================================================
// Constructor / Destructor
// ============================================================================

// IDA: ??0CGocRecode@@QEAA@XZ (0x140144B90)
CGocRecode::CGocRecode()
    : GOComponent()
    , m_stCashRewardItem(nullptr)
    , m_stCashRewardItemEx(nullptr)
{
    // Initialize arrays
    memset(m_nMazeRecode, 0, sizeof(m_nMazeRecode));
    memset(m_stCashRewardItemListEX, 0, sizeof(m_stCashRewardItemListEX));
    memset(m_stToolRewardInfo, 0, sizeof(m_stToolRewardInfo));
    memset(m_stRewardInfiniteTower, 0, sizeof(m_stRewardInfiniteTower));
    memset(m_dw64RankingListTick, 0, sizeof(m_dw64RankingListTick));
}

// IDA: ??1CGocRecode@@UEAA@XZ (0x140144DD0)
// Verified: Direct IDA decompilation - destructor clears all maps and calls base destructor
CGocRecode::~CGocRecode() {
    // IDA: Call each map's destructor in reverse order of construction
    m_mapEnterGroupLimitCount_Character.clear();
    m_mapEnterGroupLimitCount_Account.clear();
    m_mapEnterMazeLimitCount.clear();
    m_mapMyLastRanking.clear();
    m_mapMyRanking.clear();
    m_mapKilledUser.clear();
    m_mapMazeGroupState.clear();
    m_mapMazeEpisodeState.clear();
    m_mapDistrictState.clear();
    m_mapMazeClearInfo.clear();

    // IDA: Clear tool reward info (ST_LEAGUE_RECORD_LIST destructor)
    memset(m_stToolRewardInfo, 0, sizeof(m_stToolRewardInfo));

    // IDA: Call base class destructor
    // GOComponent::~GOComponent(this);
}

// ============================================================================
// Init / Clear
// ============================================================================

// IDA: ?Init@CGocRecode@@QEAAXXZ (0x140144EE0)
void CGocRecode::Init() {
    Clear();
}

// IDA: ?Clear@CGocRecode@@QEAAXXZ (0x1401451B0)
void CGocRecode::Clear() {
    // Clear all maps
    m_mapMazeClearInfo.clear();
    m_mapMazeEpisodeState.clear();
    m_mapMazeGroupState.clear();
    m_mapDistrictState.clear();
    m_mapKilledUser.clear();
    m_mapEnterMazeLimitCount.clear();
    m_mapEnterGroupLimitCount_Account.clear();
    m_mapEnterGroupLimitCount_Character.clear();
    m_mapMyRanking.clear();
    m_mapMyLastRanking.clear();

    // Reset time tracking
    m_nInitKilledUserTime = 0;
    m_nInitEnterMazeLimitCountTime = 0;
    m_nInitEnterGroupLimitCountTime_Account = 0;
    m_nInitEnterGroupLimitCountTime_Character = 0;

    // Reset flags
    m_bLoadWorldSharePoint = false;
    m_bLoadMazeEnterLimitCount = false;

    // Reset play point and ranking
    m_nPlayPointOrder = 1;
    m_fPlayPoint = 0.0f;
    m_nRank = 0;
    m_fExpValue = 1.0f;
    m_fMoneyValue = 1.0f;
    m_fPartyExpValue = 0.0f;
    m_fPartyMoneyValue = 0.0f;

    // Reset clear point
    m_nClearPoint = 0;

    // Reset final rewards
    m_fFinalRewardExp = 0.0f;
    m_nFinalRewardMoney = 0;

    // Reset play time
    m_dwPlayTime = 0;

    // Reset unity bonuses
    m_nComboBPByUnity = 0;
    m_nDSPointByUnity = 0;

    // Reset maze info
    m_nMazeID = 0;
    m_nMazeClearTime = 0;
    m_nMazeEpisodeNo = 0;
    m_nMazeDifficulty = 0;
    m_nMazeLevel = 0;
    m_nMazeExp = 0;
    m_nMazeMoney = 0;
    m_nMazeGroup = 0;

    // Reset maze recode
    memset(m_nMazeRecode, 0, sizeof(m_nMazeRecode));

    // Reset reward items
    memset(&m_stBaseRewardItem, 0, sizeof(m_stBaseRewardItem));
    if (m_stCashRewardItem) {
        memset(m_stCashRewardItem, 0, sizeof(ST_MAZE_CASH_REWARD_INFO));
    }
    if (m_stCashRewardItemEx) {
        memset(m_stCashRewardItemEx, 0, sizeof(ST_MAZE_CASH_REWARD_INFO));
    }
    memset(m_stCashRewardItemListEX, 0, sizeof(m_stCashRewardItemListEX));
    memset(&m_stHiddenEventRewardItem, 0, sizeof(m_stHiddenEventRewardItem));
    m_byHiddenEventSendPost = 0;

    // Reset infinite tower
    m_sInfiniteTowerClearChapter = 0;
    m_nInfiniteTowerLimitTime = 0;
    m_sInfiniteTowerClearStage = 0;
    m_wWaitDistrictStateID = 0;
    m_nInfiniteTowerLimitCount = 0;
    m_nInfiniteTowerLimitPCBangCount = 0;
    m_bEnterInfinite = false;
    m_nMazeDifficulty = 0;
    memset(m_stRewardInfiniteTower, 0, sizeof(m_stRewardInfiniteTower));

    // Reset over-indulgence
    m_nIndulgenceConnectTermTick = 0;
    m_nIndulgenceDisconnectTermTick = 0;
    m_nIndulgenceAlertTick = 0;
    m_nIndulgencePrevTick = 0;
    m_nIndulgenceState = 0;

    // Reset tutorial
    m_bClearTurtorial = false;

    // Reset party
    m_nPartyMemberCount = 0;
    m_bUseRevive = false;

    // Reset ranking ticks
    memset(m_dw64RankingListTick, 0, sizeof(m_dw64RankingListTick));
}

// ============================================================================
// Maze Functions
// ============================================================================

// IDA: ?InitMaze@CGocRecode@@QEAAXH@Z (0x140144F00)
void CGocRecode::InitMaze(int nMazeID) {
    // Reset maze state
    m_nMazeClearTime = 0;
    m_nMazeEpisodeNo = 0;
    m_nMazeDifficulty = 0;
    m_nMazeLevel = 0;
    m_nMazeExp = 0;
    m_nMazeMoney = 0;
    m_nMazeGroup = 0;

    // Reset play point
    m_nPlayPointOrder = 1;
    m_fPlayPoint = 0.0f;
    m_nRank = 0;
    m_fExpValue = 1.0f;
    m_fMoneyValue = 1.0f;
    m_fPartyExpValue = 0.0f;
    m_fPartyMoneyValue = 0.0f;

    // Reset clear point
    m_nClearPoint = 0;

    // Reset final rewards
    m_fFinalRewardExp = 0.0f;
    m_nFinalRewardMoney = 0;

    // Reset play time
    m_dwPlayTime = 0;

    // Reset unity bonuses
    m_nComboBPByUnity = 0;
    m_nDSPointByUnity = 0;

    // Set maze ID
    m_nMazeID = nMazeID;

    // Get maze info from resource manager
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_MAZE_INFO* pMazeInfo = pServer
        ? pServer->GetResourceMgr().GetTB_MAZE_INFO(static_cast<std::uint16_t>(m_nMazeID))
        : nullptr;
    if (pMazeInfo) {
        m_nMazeClearTime = pMazeInfo->Maze_ClearTime * 1000;
        m_nMazeEpisodeNo = pMazeInfo->Maze_Episode_No;
        m_nMazeDifficulty = pMazeInfo->Maze_Difficulty_Type;
        m_nMazeLevel = pMazeInfo->Req_Min_Lv;
        m_nMazeExp = static_cast<int>(pMazeInfo->Maze_Reward_EXP);
        m_nMazeMoney = static_cast<int>(pMazeInfo->Maze_Reward_Money);
        m_nMazeGroup = pMazeInfo->Maze_Group;
    }

    // Reset recode
    memset(m_nMazeRecode, 0, sizeof(m_nMazeRecode));

    // Reset reward items
    memset(&m_stBaseRewardItem, 0, sizeof(m_stBaseRewardItem));
    if (m_stCashRewardItem) {
        memset(m_stCashRewardItem, 0, sizeof(ST_MAZE_CASH_REWARD_INFO));
    }
    if (m_stCashRewardItemEx) {
        memset(m_stCashRewardItemEx, 0, sizeof(ST_MAZE_CASH_REWARD_INFO));
    }
    memset(m_stCashRewardItemListEX, 0, sizeof(m_stCashRewardItemListEX));
    memset(&m_stHiddenEventRewardItem, 0, sizeof(m_stHiddenEventRewardItem));
    m_byHiddenEventSendPost = 0;

    // Reset party
    m_nPartyMemberCount = 0;
    m_bShowCutscene = false;
}

// IDA: ?SetRecode@CGocRecode@@QEAAXHH@Z (0x140145810)
void CGocRecode::SetRecode(unsigned int nRecodeType, int nCount) {
    if (nRecodeType >= 10) {
        return;
    }

    if (nRecodeType == 0) {
        // Type 0: Take max
        if (m_nMazeRecode[0] > nCount) {
            m_nMazeRecode[0] = m_nMazeRecode[0];
        } else {
            m_nMazeRecode[0] = nCount;
        }
    } else {
        // Other types: Add
        m_nMazeRecode[nRecodeType] += nCount;
    }
}

// IDA: ?SetFullRecode@CGocRecode@@QEAAXPEAH@Z (0x140148110)
void CGocRecode::SetFullRecode(int* nRecode) {
    memcpy(m_nMazeRecode, nRecode, sizeof(m_nMazeRecode));
}

// IDA: ?TestRandomScore@CGocRecode@@QEAAXXZ (0x1401480B0)
void CGocRecode::TestRandomScore() {
    for (int i = 0; i < 10; ++i) {
        int nRand = rand() % 1000;
        SetRecode(i, nRand);
    }
}

// IDA: ?SetRewardValue@CGocRecode@@QEAAXMHMM@Z (0x1401458A0)
void CGocRecode::SetRewardValue(float fPlayPoint, unsigned int nRank, float fExpValue, float fMoneyValue) {
    m_nRank = static_cast<int>(nRank);
    m_fPlayPoint = fPlayPoint;
    m_fExpValue = fExpValue;
    m_fMoneyValue = fMoneyValue;

    GreenDamTan_log_debug("game.contents", "<REWARD> reward ( %.2f / %d )", fPlayPoint, nRank);
}

// IDA: ?SetPlayPointOrder@CGocRecode@@QEAAXHH@Z (0x140146F00)
void CGocRecode::SetPlayPointOrder(int nCount, int nOrder) {
    m_nPlayPointOrder = nOrder;

    int nCountClamped = nCount;
    if (nCount > 4) {
        nCountClamped = 4;
    }

    // TODO: 需要外部依赖 - XResourceMgr::GetTB_MAZEREWARD_PARTYVALUE
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_MAZEREWARD_PARTYVALUE* pPartyValue = XResourceMgr::GetTB_MAZEREWARD_PARTYVALUE(&pServer->m_xResourceMgr, nCountClamped);
    // if (pPartyValue) {
    //     int nOrderClamped = nOrder;
    //     if (nOrder > 4) {
    //         nOrderClamped = 4;
    //     }
    //     m_fPartyExpValue = pPartyValue->EXP_Values[nOrderClamped - 1];
    //     m_fPartyMoneyValue = pPartyValue->Money_Values[nOrderClamped];
    // }

    UpdateOverIndulgence();
}

// ============================================================================
// Calculation Functions
// ============================================================================

// IDA: ?CalculateFirst@CGocRecode@@QEAAXAEAMH@Z (0x140145D90)
void CGocRecode::CalculateFirst(float& fPlayPoint, int nPartyMemberCount) {
    // TODO: 需要复杂外部依赖 - XResourceMgr, CMover, CGocAttribute, XMaze
    // 此函数计算第一个阶段的分数，包括：
    // 1. Clear Point (从 TB_MAZEREWARD_STANDARD)
    // 2. Party Point
    // 3. Combo Point
    // 4. Monster Kill Point
    // 5. User Skill
    // 6. User Damage Percent
    // 7. Victim Hit Rate

    // 占位实现
    (void)fPlayPoint;
    (void)nPartyMemberCount;
    GreenDamTan_log_debug("game.contents", "<REWARD> CalculateFirst - MazeID:%d", m_nMazeID);
}

// IDA: ?CalculateSecond@CGocRecode@@QEAAXAEAM@Z (0x1401468D0)
void CGocRecode::CalculateSecond(float& fPlayPoint) {
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer || !pServer->GetResourceMgr().GetTB_MAZE_INFO(static_cast<std::uint16_t>(m_nMazeID))) {
        return;
    }

    int nClearTime = GetClearTime();
    if (nClearTime <= 0) {
        nClearTime = 1;
    }

    // TODO: 需人工审查
    // IDA 0x1401468D0 reads the first TB_MAZEREWARD_TIME row from XResourceMgr's
    // internal map iterator. The current table fragment exposes indexed getters only,
    // so this keeps the same scoring formula through the stable row ID used by the
    // restored resource accessor until the original begin-iterator accessor is exposed.
    TB_MAZEREWARD_TIME* pRewardTime = pServer->GetResourceMgr().GetTB_MAZEREWARD_TIME(1);
    if (!pRewardTime || pRewardTime->Time_Value_Min == 0) {
        return;
    }

    int nTimeValue = nClearTime / pRewardTime->Time_Value_Min;
    if (nTimeValue <= 0) {
        return;
    }

    if (nTimeValue > pRewardTime->Time_Value_Max) {
        nTimeValue = pRewardTime->Time_Value_Max;
    }

    const float fScore = static_cast<float>(nTimeValue) * pRewardTime->ClearTime_Value + 1.0f;
    fPlayPoint *= fScore;

    CMover* pMover = GetOwnerGO();
    const std::uint32_t dwActorID = pMover ? 0 : 0;
    GreenDamTan_log_debug("game.contents", "<%u RECODE> Second total ( %d ) ClearTime ( %d ) Value( %0.2f )",
                          dwActorID, static_cast<int>(fPlayPoint), nClearTime, fScore);
}

// IDA: ?CalculateThird@CGocRecode@@QEAAXAEAM@Z (0x140146B10)
void CGocRecode::CalculateThird(float& fPlayPoint) {
    const int nDifficulty = m_nMazeDifficulty;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_MAZEREWARD_DIFFICULTY* pRewardDifficulty = pServer
        ? pServer->GetResourceMgr().GetTB_MAZEREWARD_DIFFICULTY(static_cast<std::uint8_t>(nDifficulty + 1))
        : nullptr;
    if (!pRewardDifficulty) {
        return;
    }

    fPlayPoint *= pRewardDifficulty->Difficulty_Value;

    CMover* pMover = GetOwnerGO();
    const std::uint32_t dwActorID = pMover ? 0 : 0;
    GreenDamTan_log_debug("game.contents", "<%u RECODE> Third Total( %d ) Difficulty ( %d ) value ( %0.2f ) ",
                          dwActorID, static_cast<int>(fPlayPoint), nDifficulty,
                          pRewardDifficulty->Difficulty_Value);
}

// IDA: ?CalculateFourth@CGocRecode@@QEAAXAEAM@Z (0x140146C20)
void CGocRecode::CalculateFourth(float& fPlayPoint) {
    const int nMazeLevel = GetMazeLevel();
    CMover* pMover = GetOwnerGO();
    const int nPlayerLevel = pMover ? static_cast<int>(pMover->GetLevel()) : nMazeLevel;
    const int nFinalLevel = std::max(0, nPlayerLevel - nMazeLevel);

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_MAZEREWARD_LEVEL* pRewardLevel = pServer
        ? pServer->GetResourceMgr().GetTB_MAZEREWARD_LEVEL(static_cast<std::uint8_t>(nFinalLevel))
        : nullptr;
    if (!pRewardLevel) {
        return;
    }

    fPlayPoint *= pRewardLevel->Level_Value;

    const std::uint32_t dwActorID = pMover ? 0 : 0;
    GreenDamTan_log_debug("game.contents", "<%u RECODE> Fourth Total ( %d ) GapLevel( %d ) LevelValue( %0.2f )",
                          dwActorID, static_cast<int>(fPlayPoint), nFinalLevel, pRewardLevel->Level_Value);
}

// IDA: ?GetRank@CGocRecode@@QEAAXM@Z (0x140146D80)
void CGocRecode::GetRank(float fPlayPoint) {
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_MAZEREWARD_STANDARD* pStandard = pServer
        ? pServer->GetResourceMgr().GetTB_MAZEREWARD_STANDARD(static_cast<std::int16_t>(m_nMazeID))
        : nullptr;
    if (!pStandard) {
        CMover* pMover = GetOwnerGO();
        const std::uint32_t dwActorID = pMover ? 0 : 0;
        GreenDamTan_log_error("game.contents", "GetRank error - No Table TB_MAZEREWARD_STANDARD[ ActorID:%u, MazeID:%d ] ( %d )",
                              dwActorID, m_nMazeID, 593);
        return;
    }

    const int rankPoints[] = {
        pStandard->Rank_C_Point,
        pStandard->Rank_B_Point,
        pStandard->Rank_A_Point,
        pStandard->Rank_S_Point,
        pStandard->Rank_SS_Point,
    };

    for (int i = 0; i < 5; ++i) {
        if (static_cast<float>(rankPoints[i]) >= fPlayPoint) {
            TB_MAZEREWARD_RANK* pRank = pServer->GetResourceMgr().GetTB_MAZEREWARD_RANK(static_cast<std::uint8_t>(i));
            if (pRank) {
                SetRewardValue(fPlayPoint, static_cast<unsigned int>(i), pRank->EXP_Value, pRank->Money_Value);
            }
            return;
        }
    }

    TB_MAZEREWARD_RANK* pRank = pServer->GetResourceMgr().GetTB_MAZEREWARD_RANK(5);
    if (pRank) {
        SetRewardValue(fPlayPoint, 5, pRank->EXP_Value, pRank->Money_Value);
    }
}

// ============================================================================
// Reward Functions
// ============================================================================

// IDA: ?MazeReward@CGocRecode@@QEAAHK_NH@Z (0x140145930)
int CGocRecode::MazeReward(unsigned int dwPlayTime, bool bApplyReward, int nPartyMemberCount) {
    // Store play time
    m_dwPlayTime = dwPlayTime;

    // Calculate remaining clear time
    m_nMazeClearTime -= dwPlayTime;
    m_nMazeClearTime /= 1000;

    // Store party member count
    m_nPartyMemberCount = nPartyMemberCount;

    // Calculate play point through all stages
    float fPlayPoint = 0.0f;
    CalculateFirst(fPlayPoint, nPartyMemberCount);
    CalculateSecond(fPlayPoint);
    CalculateThird(fPlayPoint);
    CalculateFourth(fPlayPoint);

    // Determine rank
    GetRank(fPlayPoint);

    // Apply reward if requested
    if (bApplyReward) {
        ApplyReward();
    }

    // Update clear info for ranking
    // TODO: UpdateClearInfo(dwPlayTime / 100);

    // TODO: 更新任务、成就等
    // - CGocQuest::UpdateCondition
    // - CGocDailyMission::UpdateMazeClearType
    // - CGocWeeklyMission::CheckWeeklyMissionUpdate
    // - CGocAchieve::UpdatemMazeClearAchieve
    // - CGocEntity::UpdateOpenTitle

    GreenDamTan_log_debug("game.contents", "<REWARD> MazeReward - MazeID:%d, Rank:%d, PlayTime:%u",
                          m_nMazeID, m_nRank, dwPlayTime);

    return m_nRank;
}

// IDA: ?ApplyReward@CGocRecode@@QEAAXAEAUPS_REWARD_PARTY_SCORE@@@Z (0x140146FB0)
void CGocRecode::ApplyReward(PS_REWARD_PARTY_SCORE& psPartyScore) {
    // TODO: 需要复杂外部依赖 - CUser, CGocInventory, CGocAttribute, CGocPost

    // Calculate final rewards
    m_fFinalRewardExp = static_cast<float>(m_nMazeExp) * (m_fExpValue + m_fPartyExpValue);
    m_fFinalRewardExp += static_cast<float>(GetSpecialExp(static_cast<int>(m_fFinalRewardExp)));

    m_nFinalRewardMoney = static_cast<int>(static_cast<float>(m_nMazeMoney) * (m_fMoneyValue + m_fPartyMoneyValue));
    m_nFinalRewardMoney += GetSpecialGold(m_nFinalRewardMoney);

    // TODO: Add money to inventory
    // TODO: Set exp to attribute

    SetRecode(5, m_nFinalRewardMoney);

    // Fill party score
    psPartyScore.byOrder = static_cast<unsigned char>(m_nPlayPointOrder);
    psPartyScore.byRank = static_cast<unsigned char>(m_nRank);
    psPartyScore.nPlayPoint = static_cast<int>(m_fPlayPoint);

    // TODO: Copy member name
    // TODO: Set reward items
    // TODO: Send reward

    GreenDamTan_log_info("game.contents", "<%d RECODE> Final ( %d )", 0, psPartyScore.nPlayPoint);
}

// IDA: ?ApplyReward@CGocRecode@@QEAAXXZ (0x140147730)
void CGocRecode::ApplyReward() {
    // TODO: 需要复杂外部依赖

    // Calculate final rewards
    m_fFinalRewardExp = static_cast<float>(m_nMazeExp) * (m_fExpValue + m_fPartyExpValue);
    m_fFinalRewardExp += static_cast<float>(GetSpecialExp(static_cast<int>(m_fFinalRewardExp)));

    m_nFinalRewardMoney = static_cast<int>(static_cast<float>(m_nMazeMoney) * (m_fMoneyValue + m_fPartyMoneyValue));
    m_nFinalRewardMoney += GetSpecialGold(m_nFinalRewardMoney);

    // TODO: Apply exp and money
    // TODO: Set reward items
    // TODO: Send reward

    GreenDamTan_log_info("game.contents", "[SetExp] CGocRecode::ApplyReward - Exp:%.2f, Money:%d",
                          m_fFinalRewardExp, m_nFinalRewardMoney);
}

// IDA: ?SendReward@CGocRecode@@QEAAXXZ (0x140147CC0)
void CGocRecode::SendReward() {
    // TODO: 需要外部依赖 - 发送奖励包给客户端
    GreenDamTan_log_debug("game.contents", "CGocRecode::SendReward - MazeID:%d", m_nMazeID);
}

// IDA: ?SetRewardItem@CGocRecode@@QEAAX_N@Z (0x14014ADD0)
// TODO: Requires TB_MAZEREWARD_ITEM structure and GetTB_ITEM_RANDOMBOX_With_Lock function
void CGocRecode::SetRewardItem(bool bTool) {
    // TODO: Implement when TB_MAZEREWARD_ITEM and related structures are available
    // IDA shows this function:
    // 1. Gets TB_MAZEREWARD_ITEM for current maze
    // 2. Uses g_nRewardRateTotal[m_nRank] for random selection
    // 3. Gets TB_ITEM_RANDOMBOX and selects items based on rates
    // 4. Sets m_stBaseRewardItem and m_stCashRewardItem
    GreenDamTan_log_debug("game.contents", "CGocRecode::SetRewardItem - MazeID:%d, Tool:%d (TODO)", m_nMazeID, bTool ? 1 : 0);
}

// IDA: ?GetRewardItem@CGocRecode@@QEAAXXZ (0x14014C040)
// TODO: Requires CGocInventory, ST_MAZE_CASH_REWARD_INFO with proper fields
void CGocRecode::GetRewardItem() {
    // TODO: Implement when CGocInventory and related structures are available
    // IDA shows this function:
    // 1. Gets CGocInventory component
    // 2. Validates m_stCashRewardItem fields
    // 3. Creates reward items and reduces cash items
    // 4. Sends DB packets
    GreenDamTan_log_debug("game.contents", "CGocRecode::GetRewardItem (TODO)");
}

// IDA: ?GetRewardItemEx@CGocRecode@@QEAAXXZ (0x14014CCD0)
// TODO: Requires CGocInventory, ST_MAZE_CASH_REWARD_INFO with proper fields
void CGocRecode::GetRewardItemEx() {
    // TODO: Implement when CGocInventory and related structures are available
    // Similar to GetRewardItem but for extended cash reward items
    GreenDamTan_log_debug("game.contents", "CGocRecode::GetRewardItemEx (TODO)");
}

// ============================================================================
// OnUpdate
// ============================================================================

// IDA: ?OnUpdate@CGocRecode@@QEAAXXZ (0x1401455A0)
void CGocRecode::OnUpdate() {
    // TODO: 需要复杂外部依赖 - CMover, CUser, CGocEntity
    // 此函数处理：
    // 1. 清理击杀用户列表（每日重置）
    // 2. 初始化无限塔信息
    // 3. 清理迷宫进入次数限制
    // 4. 更新防沉迷系统
    // 5. 更新排名列表 tick

    // TODO: 检查用户是否在线
    // CMover* pMover = GetOwnerGO();
    // CUser* pUser = dynamic_cast<CUser*>(pMover);
    // if (!pUser || !(pUser->GetUserFlags() & 0x10)) return;

    // TODO: 获取服务器时间
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // __int64 nTime = pServer->GetBeforeInitDate();

    // TODO: 检查并清理各种计时器
    // - ClearKilledUser if needed
    // - InitInfiniteTowerInfo if needed
    // - ClearEnterMazeLimitCount if needed
    // - ClearEnterGroupLimitCount if needed

    UpdateOverIndulgence();

    // Update ranking list tick
    // if (m_dw64RankingListTick[0] < GetTickCount64()) {
    //     for (short sh = 1; sh < 4; ++sh) {
    //         if (m_dw64RankingListTick[sh] < GetTickCount64()) {
    //             m_dw64RankingListTick[sh] = 0;
    //         }
    //     }
    //     m_dw64RankingListTick[0] = GetTickCount64() + 1000;
    // }
}

// ============================================================================
// Infinite Tower Functions
// ============================================================================

// IDA: ?SetInfiniteTowerInfo@CGocRecode@@QEAAXFF_JFH_N@Z (0x14014D9F0)
void CGocRecode::SetInfiniteTowerInfo(short sClearChapter, short sClearStage, __int64 nLimitTime, int nLimitCount, short sLimitPCBangCount, bool bEnter) {
    m_sInfiniteTowerClearChapter = sClearChapter;
    m_sInfiniteTowerClearStage = sClearStage;
    m_nInfiniteTowerLimitTime = static_cast<int>(nLimitTime);
    m_nInfiniteTowerLimitCount = nLimitCount;
    m_nInfiniteTowerLimitPCBangCount = sLimitPCBangCount;
    m_bEnterInfinite = bEnter;
}

// IDA: ?InitInfiniteTowerInfo@CGocRecode@@QEAAX_J@Z (0x14014DBC0)
// TODO: Requires PS_UPDATE_INFINITETOWER_LIMIT_TIME structure and CUser::GetQuestID
void CGocRecode::InitInfiniteTowerInfo(__int64 nInitTime) {
    // TODO: Implement when PS_UPDATE_INFINITETOWER_LIMIT_TIME and CUser methods are available
    m_nInfiniteTowerLimitTime = nInitTime;
    m_nInfiniteTowerLimitCount = 0;
    m_nInfiniteTowerLimitPCBangCount = 0;
    GreenDamTan_log_debug("game.contents", "CGocRecode::InitInfiniteTowerInfo - LimitTime:%lld (TODO)", nInitTime);
}

// IDA: ?SetInfiniteTowerLimitTime@CGocRecode@@QEAAX_N0@Z (0x14014DF10)
// Verified: Direct IDA decompilation - sets infinite tower limit time and syncs to DB
void CGocRecode::SetInfiniteTowerLimitTime(bool bNetCafe, bool bSyncDB) {
    // IDA: 首先调用 SetInfiniteTowerLimitCount(bNetCafe, false)
    SetInfiniteTowerLimitCount(bNetCafe ? 1 : 0, false);

    if (bSyncDB) {
        // IDA: 发送 DB 包 (3/0x53)
        // PS_UPDATE_INFINITETOWER_LIMIT_TIME stInfinite;
        // stInfinite.dwActorID = GetQuestID();
        // stInfinite.nLimitTime = m_nInfiniteTowerLimitTime;
        // XSendDBPacket xSendDBPacket(pObject, 3, 0x53);
        // Send to DB and call SendInfiniteTowerInfo

        // TODO: 需要完整外部依赖
        // - CQuestCondition::GetQuestID
        // - XSendDBPacket, XGameServer::SendDBGame
        // - SendInfiniteTowerInfo

        GreenDamTan_log_debug("game.contents", "CGocRecode::SetInfiniteTowerLimitTime - SyncDB:true");
    }
}

// IDA: ?SetInfiniteTowerLimitCount@CGocRecode@@QEAAXH_N@Z (0x14014E0C0)
// Verified: Direct IDA decompilation - sets infinite tower limit count with optional clear
void CGocRecode::SetInfiniteTowerLimitCount(int nState, bool bClear) {
    // IDA: 如果 bClear 为 true，清零计数
    if (bClear) {
        m_nInfiniteTowerLimitCount = 0;
        m_nInfiniteTowerLimitPCBangCount = 0;
    }
    // IDA: 否则如果 nState != 0，需要检查 PC Bang 计数
    else if (nState != 0) {
        // IDA: 获取 TB_MAZE_INFO 表检查 PC Room 进入限制
        // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // TB_MAZE_INFO* pMaze = pServer->GetResourceMgr().GetTB_MAZE_INFO(m_nMazeID);
        // if (pMaze && GetInfiniteTowerLimitPCBangCount() < pMaze->Maze_Enter_Count_PC_Room) {
        //     ++m_nInfiniteTowerLimitPCBangCount;
        // } else {
        //     ++m_nInfiniteTowerLimitCount;
        // }
        ++m_nInfiniteTowerLimitCount;
    }
    // IDA: 否则直接增加计数
    else {
        ++m_nInfiniteTowerLimitCount;
    }
}

// IDA: ?RewardInfinteTower@CGocRecode@@QEAAXHHK@Z (0x14014E1E0)
void CGocRecode::RewardInfinteTower(int nMazeID, int nChapter, unsigned long dwPlayTime) {
    // TODO: 需要复杂外部依赖
    (void)nMazeID;
    (void)nChapter;
    (void)dwPlayTime;
}

// IDA: ?SendInfiniteTowerInfo@CGocRecode@@QEAAXXZ (0x14014EB30)
// TODO: Requires PS_INFINITE_TOWER_INFO structure and CUser::Send
void CGocRecode::SendInfiniteTowerInfo() {
    // TODO: Implement when PS_INFINITE_TOWER_INFO and CUser::Send are available
    GreenDamTan_log_debug("game.contents", "CGocRecode::SendInfiniteTowerInfo - Chapter:%d, Stage:%d (TODO)",
                          m_sInfiniteTowerClearChapter, m_sInfiniteTowerClearStage);
}

// IDA: ?UseInfiniteTowerInitItem@CGocRecode@@QEAAXXZ (0x14014DD80)
// Verified: Direct IDA decompilation - resets infinite tower count and syncs to DB
void CGocRecode::UseInfiniteTowerInitItem() {
    // IDA: 重置计数为 0
    m_nInfiniteTowerLimitCount = 0;

    // IDA: 发送 DB 包 (3/0x53)
    // PS_UPDATE_INFINITETOWER_LIMIT_TIME stInfinite;
    // stInfinite.dwActorID = GetQuestID();
    // stInfinite.nLimitTime = m_nInfiniteTowerLimitTime;
    // XSendDBPacket xSendDBPacket(pObject, 3, 0x53);
    // xSendDBPacket << stInfinite;
    // xSendDBPacket << m_nInfiniteTowerLimitPCBangCount;
    // xSendDBPacket << m_nInfiniteTowerLimitCount;
    // XGameServer::SendDBGame(&xSendDBPacket);
    // SendInfiniteTowerInfo();

    // TODO: 需要完整外部依赖
    // - CQuestCondition::GetQuestID
    // - XSendDBPacket, XGameServer::SendDBGame
    // - SendInfiniteTowerInfo

    GreenDamTan_log_debug("game.contents", "CGocRecode::UseInfiniteTowerInitItem - Count reset to 0");
}

// ============================================================================
// Maze State Functions
// ============================================================================

// IDA: ?IsClearMazeOnce@CGocRecode@@QEAA_NH@Z (0x140149190)
// Verified: Direct IDA decompilation - checks if maze group exists in m_mapMazeClearInfo
bool CGocRecode::IsClearMazeOnce(int nGroupID) {
    // IDA: 查找 m_mapMazeClearInfo 中是否存在该 GroupID
    auto it = m_mapMazeClearInfo.find(nGroupID);
    return it != m_mapMazeClearInfo.end();
}

// IDA: ?IsClearMaze@CGocRecode@@QEAA_NH@Z (0x140149200)
// Verified: Direct IDA decompilation - checks if specific maze ID is cleared
bool CGocRecode::IsClearMaze(int nMazeID) {
    // IDA: 如果 nMazeID <= 0，直接返回 true
    if (nMazeID <= 0) {
        return true;
    }

    // TODO: 需要外部依赖 XResourceMgr::GetTB_MAZE_INFO
    // IDA 逻辑：
    // 1. 获取 TB_MAZE_INFO
    // 2. 查找 m_mapMazeClearInfo 中的 Maze_Group
    // 3. 遍历 vecClearMazeList 检查是否存在 nMazeID
    // 简化实现：调用 IsClearMazeOnce
    return IsClearMazeOnce(nMazeID);
}

// IDA: ?UpdateClearInfo@CGocRecode@@QEAAXK@Z (0x140149320)
// Verified: Direct IDA decompilation - complex function updating maze clear info
// IDA 逻辑精确还原:
//   1. 获取 TB_MAZE_INFO
//   2. 检查 EpisodeNo (1-5)
//   3. 查找或创建 m_mapMazeClearInfo 条目
//   4. 发送 PS_MAZE_CLEAR_INFO 包 (0x11/0x64)
//   5. 调用 CGocSoulMetry::FindNewSoulMetry
//   6. 调用 RankingDataUpdate
void CGocRecode::UpdateClearInfo(unsigned int dwPlayTime) {
    // IDA: 获取 XGameServer 和 TB_MAZE_INFO
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    TB_MAZE_INFO* pTB_MAZE_INFO = pServer->GetResourceMgr().GetTB_MAZE_INFO(static_cast<std::uint16_t>(m_nMazeID));
    if (!pTB_MAZE_INFO) {
        return;
    }

    // IDA: 检查 EpisodeNo (1-5), 即 Maze_Episode_No - 1 <= 4
    int nEpisodeNo = pTB_MAZE_INFO->Maze_Episode_No - 1;
    if (nEpisodeNo < 0 || nEpisodeNo > 4) {
        return;
    }

    // IDA: 查找 m_mapMazeClearInfo
    int nMazeGroup = pTB_MAZE_INFO->Maze_Group;
    auto iter = m_mapMazeClearInfo.find(nMazeGroup);

    if (iter != m_mapMazeClearInfo.end()) {
        // IDA: 已存在，检查是否需要添加 MazeID
        bool bFind = false;
        for (size_t sh = 0; sh < iter->second.vecClearMazeList.size(); ++sh) {
            if (pTB_MAZE_INFO->ID == iter->second.vecClearMazeList[sh]) {
                bFind = true;
                break;
            }
        }

        if (!bFind) {
            // IDA: 添加到列表
            iter->second.vecClearMazeList.push_back(pTB_MAZE_INFO->ID);
        }

        // IDA: 发送 PS_MAZE_CLEAR_INFO 包 (0x11/0x64)
        // TODO: 需要完整的 PS_MAZE_CLEAR_INFO 结构体和发送逻辑
        // PS_MAZE_CLEAR_INFO psMazeClearInfo;
        // psMazeClearInfo.uxActorID = GetOwnerUser() ? GetOwnerUser()->GetActorID() : 0;
        // psMazeClearInfo.bReset = 0;
        // psMazeClearInfo.vecInfo.push_back(iter->second);
        // XSendPacket sendPacket(0x11, 0x64);
        // sendPacket << psMazeClearInfo;
        // CUser* pUser = GetOwnerUser();
        // if (pUser) { pUser->Send(&sendPacket); }

        // TODO: 调用 CGocSoulMetry::FindNewSoulMetry
    } else {
        // IDA: 不存在，创建新条目
        ST_MAZE_CLEAR_INFO stMazeClearInfo;
        stMazeClearInfo.nMazeGroup = pTB_MAZE_INFO->Maze_Group;
        stMazeClearInfo.vecClearMazeList.push_back(pTB_MAZE_INFO->ID);

        m_mapMazeClearInfo.insert(std::make_pair(nMazeGroup, stMazeClearInfo));

        // IDA: 发送 PS_MAZE_CLEAR_INFO 包
        // TODO: 需要完整的 PS_MAZE_CLEAR_INFO 结构体和发送逻辑
        // PS_MAZE_CLEAR_INFO st;
        // st.uxActorID = GetOwnerUser() ? GetOwnerUser()->GetActorID() : 0;
        // st.bReset = 0;
        // st.vecInfo.push_back(stMazeClearInfo);
        // XSendPacket packet(0x11, 0x64);
        // packet << st;
        // CUser* pUser = GetOwnerUser();
        // if (pUser) { pUser->Send(&packet); }

        // TODO: 调用 CGocSoulMetry::FindNewSoulMetry
    }

    // IDA: 调用 RankingDataUpdate
    RankingDataUpdate(m_nMazeID, dwPlayTime, 0);
}

// IDA: ?UpdateClearInfo_cheat@CGocRecode@@QEAAXH@Z (0x140149850)
// TODO: Requires PS_MAZE_CLEAR_INFO structure and CUser methods
void CGocRecode::UpdateClearInfo_cheat(int nMazeID) {
    // TODO: Implement when PS_MAZE_CLEAR_INFO and related structures are available
    GreenDamTan_log_debug("game.contents", "CGocRecode::UpdateClearInfo_cheat - MazeID:%d (TODO)", nMazeID);
}

// IDA: ?SendDBLogClearMaze@CGocRecode@@QEAAXHH@Z (0x14015AD70)
void CGocRecode::SendDBLogClearMaze(int nMazeID, int nClearCount) {
    // TODO: 需要外部依赖 - 发送DB日志
    (void)nMazeID;
    (void)nClearCount;
}

// ============================================================================
// BP/DS Point Functions
// ============================================================================

// IDA: ?AddBPCombo@CGocRecode@@QEAAXH@Z (0x140148D70)
// TODO: Requires XMaze, CMover::Send
void CGocRecode::AddBPCombo(int nBP) {
    // TODO: Implement when XMaze and CMover::Send are available
    if (nBP <= 0) {
        return;
    }
    m_nComboBPByUnity += nBP;
    GreenDamTan_log_debug("game.contents", "<UNITY> Add BP ( %d / %d ) (TODO)", nBP, m_nComboBPByUnity);
}

// IDA: ?AddDSPoint@CGocRecode@@QEAAXH@Z (0x140148FE0)
// TODO: Requires CMover::Send
void CGocRecode::AddDSPoint(int nDSPoint) {
    // TODO: Implement when CMover::Send is available
    if (nDSPoint <= 0) {
        return;
    }
    m_nDSPointByUnity += nDSPoint;
    GreenDamTan_log_debug("game.contents", "<UNITY> Add DSPoint ( %d ) (TODO)", nDSPoint);
}

// IDA: ?DBUpdatePoint@CGocRecode@@QEAAXXZ (0x140149100)
// TODO: Requires CGocInventory
void CGocRecode::DBUpdatePoint() {
    // TODO: Implement when CGocInventory is available
    if (m_nComboBPByUnity <= 0) {
        return;
    }
    GreenDamTan_log_debug("game.contents", "CGocRecode::DBUpdatePoint - BP:%d (TODO)", m_nComboBPByUnity);
}

// ============================================================================
// District State Functions
// ============================================================================

// IDA: ?ReqWorldState@CGocRecode@@QEAAXXZ (0x140148140)
// TODO: Requires CUser::GetObject, CUser::GetQuestID, CUser::GetUAID
void CGocRecode::ReqWorldState() {
    // TODO: Implement when CUser methods are available
    GreenDamTan_log_debug("game.contents", "CGocRecode::ReqWorldState (TODO)");
}

// IDA: ?LoadDistrictState@CGocRecode@@QEAAXAEAUST_DISTRICT_STATE_LIST@@@Z (0x140148380)
// Verified: Direct IDA decompilation - loads district state from DB
void CGocRecode::LoadDistrictState(ST_DISTRICT_STATE_LIST& stStateList) {
    // IDA: Clear existing district state map
    m_mapDistrictState.clear();

    // IDA: Iterate through the state list and insert into map
    for (const auto& stInfo : stStateList.vecList) {
        if (stInfo.nDistrictID > 0) {
            // IDA: Insert into m_mapDistrictState
            // Note: The map stores ST_DISTRICT_STATE values
            m_mapDistrictState[stInfo.nDistrictID] = stInfo;
            GreenDamTan_log_debug("game.contents", "<LOAD_DSPOINT> District ( %d ) / Value ( %d )",
                                  stInfo.nDistrictID, stInfo.nDSPoint);
        }
    }

    // IDA: Check if there's a waiting district state ID
    if (m_wWaitDistrictStateID) {
        AddDistrictState(m_wWaitDistrictStateID, false);
    }
}

// IDA: ?LoadMazeState@CGocRecode@@QEAAXAEAUST_MAZE_STATE_LIST@@@Z (0x1401484D0)
// Verified: Direct IDA decompilation - loads maze state from DB
void CGocRecode::LoadMazeState(ST_MAZE_STATE_LIST& stStateList) {
    // IDA: Clear existing maps
    m_mapMazeEpisodeState.clear();
    m_mapMazeGroupState.clear();

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    // IDA: Iterate through the state list
    for (const auto& stMazeState : stStateList.vecList) {
        if (stMazeState.nMazeID <= 0) {
            // IDA: MazeID <= 0 means group state
            TB_DS_POINT* pTBDSPoint = pServer->GetResourceMgr().GetTB_DS_POINT(stMazeState.nGroupID);
            if (pTBDSPoint) {
                // IDA: Insert into m_mapMazeGroupState
                ST_RANDOM_BUFF stBuff;
                memset(&stBuff, 0, sizeof(stBuff));
                // Copy relevant data from stMazeState to stBuff
                m_mapMazeGroupState[stMazeState.nGroupID] = stBuff;
            }
        } else {
            // IDA: MazeID > 0 means episode state
            TB_MAZE_INFO* pTB_MAZE_INFO = pServer->GetResourceMgr().GetTB_MAZE_INFO(stMazeState.nMazeID);
            if (pTB_MAZE_INFO) {
                int nGroup = pTB_MAZE_INFO->Maze_Group;

                // IDA: Insert into m_mapMazeEpisodeState
                ST_RANDOM_BUFF stBuff;
                memset(&stBuff, 0, sizeof(stBuff));
                m_mapMazeEpisodeState[stMazeState.nMazeID] = stBuff;

                // IDA: Update m_mapMazeClearInfo
                auto iter = m_mapMazeClearInfo.find(nGroup);
                if (iter != m_mapMazeClearInfo.end()) {
                    // IDA: Check if MazeID already exists in the list
                    bool bFind = false;
                    for (size_t sh = 0; sh < iter->second.vecClearMazeList.size(); ++sh) {
                        if (stMazeState.nMazeID == iter->second.vecClearMazeList[sh]) {
                            bFind = true;
                            break;
                        }
                    }
                    if (!bFind) {
                        iter->second.vecClearMazeList.push_back(stMazeState.nMazeID);
                    }
                } else {
                    // IDA: Create new entry
                    ST_MAZE_CLEAR_INFO stClearInfo;
                    memset(&stClearInfo, 0, sizeof(stClearInfo));
                    stClearInfo.nMazeGroup = nGroup;
                    stClearInfo.vecClearMazeList.push_back(stMazeState.nMazeID);
                    m_mapMazeClearInfo[nGroup] = stClearInfo;
                }
            }
        }
    }
}

// IDA: ?SendDistrictState@CGocRecode@@QEAAXXZ (0x140148840)
// TODO: Requires CGocNetwork, CMover::CMoverEx
void CGocRecode::SendDistrictState() {
    // TODO: Implement when CGocNetwork and CMover::CMoverEx are available
    GreenDamTan_log_debug("game.contents", "CGocRecode::SendDistrictState (TODO)");
}

// IDA: ?SendMazeState@CGocRecode@@QEAAXXZ (0x1401489F0)
// TODO: Requires CGocNetwork, PS_MAZE_CLEAR_INFO, CMover::CMoverEx
void CGocRecode::SendMazeState() {
    // TODO: Implement when CGocNetwork and related structures are available
    GreenDamTan_log_debug("game.contents", "CGocRecode::SendMazeState (TODO)");
}

// IDA: ?AddDistrictState@CGocRecode@@QEAAXG_N@Z (0x14014F6C0)
// Verified: Direct IDA decompilation - adds district state entry
void CGocRecode::AddDistrictState(unsigned short wDistrictID, bool bSend) {
    // IDA 逻辑摘要:
    // 1. 获取 CUser 并检查用户是否在线 (UserDB & 0x10)
    // 2. 如果用户不在线，设置 m_wWaitDistrictStateID = wDistrictID 并返回
    // 3. 查找 m_mapDistrictState，如果已存在则不处理
    // 4. 获取 TB_DS_POINT 表验证 wDistrictID 有效
    // 5. 创建新的 ST_DISTRICT_STATE:
    //    - nDistrictID = wDistrictID
    //    - nDSPoint = 0
    //    - bReward = 0
    // 6. 插入到 m_mapDistrictState
    // 7. 发送 DB 包 (3/0x50)
    // 8. 如果 bSend == true，发送客户端包 (3/0x58) PS_UPDATE_SHARE_POINT

    // TODO: 需要完整外部依赖
    // - CUser::stMyCharInfoEx
    // - XResourceMgr::GetTB_DS_POINT
    // - XSendDBPacket, XSendPacket
    // - CQuestCondition::GetQuestID

    (void)wDistrictID;
    (void)bSend;
    GreenDamTan_log_debug("game.contents", "CGocRecode::AddDistrictState - DistrictID:%d, Send:%d", wDistrictID, bSend);
}

// ============================================================================
// Share Point Functions
// ============================================================================

// IDA: ?RewardSharePoint@CGocRecode@@QEAAXHH@Z (0x140149E30)
// Verified: Direct IDA decompilation - rewards share point for district/maze
void CGocRecode::RewardSharePoint(int nType, int nID) {
    // IDA 逻辑:
    // nType == 0: District State (区域状态)
    // nType != 0: Maze State (迷宫状态)

    // TODO: 需要完整外部依赖 - XResourceMgr::GetTB_DS_POINT, CGocInventory, CUser
    // IDA 关键逻辑摘要:
    // 1. 如果 nType != 0 (迷宫):
    //    - 获取 TB_DS_POINT 表
    //    - 查找 m_mapMazeGroupState 中的状态
    //    - 检查当前步骤 (byStep)，如果已满(5)则返回
    //    - 循环奖励步骤，创建奖励物品
    //    - 调用 ApplyRewardItemForSharePoint
    //    - 发送 DB 包 (3/0x51)
    // 2. 如果 nType == 0 (区域):
    //    - 查找 m_mapDistrictState
    //    - 检查是否已奖励 (bReward)
    //    - 获取 TB_DS_POINT
    //    - 检查点数是否达到奖励阈值
    //    - 调用 ApplyRewardItemForSharePoint
    //    - 发送 DB 包 (3/0x50)
    // 3. 发送响应包 (3/0x63) PS_RES_GET_REWARD_SHARE_POINT

    (void)nType;
    (void)nID;
    GreenDamTan_log_debug("game.contents", "<REWARD SPOINT> Type:%d, ID:%d", nType, nID);
}

// IDA: ?ApplyRewardItemForSharePoint@CGocRecode@@QEAA_NHHAEAUST_CREATE_ITEM@@@Z (0x14014A910)
// TODO: Requires CGocInventory, TB_DS_POINT fields, TB_QUEST_REWARD fields
bool CGocRecode::ApplyRewardItemForSharePoint(int nID, int nStep, ST_CREATE_ITEM& stCreateItem) {
    // TODO: Implement when CGocInventory and related structures are available
    (void)nID;
    (void)nStep;
    (void)stCreateItem;
    GreenDamTan_log_debug("game.contents", "CGocRecode::ApplyRewardItemForSharePoint - ID:%d, Step:%d (TODO)", nID, nStep);
    return false;
}

// IDA: ?FullSharePoint@CGocRecode@@QEAAXH@Z (0x14014FBB0)
// Verified: Direct IDA decompilation - fills share point to max for district
void CGocRecode::FullSharePoint(int nMapID) {
    // IDA 逻辑摘要:
    // 1. 查找 m_mapDistrictState 中的 nMapID
    // 2. 如果不存在则直接返回
    // 3. 获取 TB_DS_POINT 表
    // 4. 设置 stState.nDSPoint = pTBDSPoint->Reward_Value_01 (填满到上限)
    // 5. 设置 stState.bReward = 0
    // 6. 发送 DB 包 (3/0x4A)
    // 7. 调用 SetClearTurtorial(1, 1)

    // TODO: 需要完整外部依赖
    // - XResourceMgr::GetTB_DS_POINT
    // - XSendDBPacket
    // - CQuestCondition::GetQuestID
    // - XGameServer::SendDBGame
    // - SetClearTurtorial

    (void)nMapID;
    GreenDamTan_log_debug("game.contents", "CGocRecode::FullSharePoint - MapID:%d", nMapID);
}

// IDA: ?UpdateSharePoint@CGocRecode@@QEAAXXZ (0x14014FDE0)
// Verified: Direct IDA decompilation - updates share point for current maze
void CGocRecode::UpdateSharePoint() {
    // IDA 逻辑摘要:
    // 1. 获取 TB_MAZE_INFO 从 m_nMazeID
    // 2. 获取 TB_DS_POINT 从 Maze_Group
    // 3. 计算 EpisodeNo (限制最大为4)
    // 4. 计算增加点数 nAddPoint = pTBDSPoint->Gauge_Type[EpisodeNo]
    // 5. 查找/更新 m_mapMazeGroupState:
    //    - 如果存在: 增加点数，检查是否超过上限
    //    - 如果不存在: 创建新条目
    // 6. 查找/更新 m_mapMazeEpisodeState
    // 7. 查找/更新 m_mapDistrictState
    // 8. 发送 DB 包 (3/0x4A)
    // 9. 发送客户端包 (3/0x58) PS_UPDATE_SHARE_POINT

    // TODO: 需要完整外部依赖
    // - XResourceMgr::GetTB_MAZE_INFO
    // - XResourceMgr::GetTB_DS_POINT
    // - XSendDBPacket, XSendPacket
    // - CQuestCondition::GetQuestID
    // - CGocNetwork::Send

    GreenDamTan_log_debug("game.contents", "<MAZE_DSPOINT> UpdateSharePoint - MazeID:%d", m_nMazeID);
}

// IDA: ?UpdateSharePointByForce@CGocRecode@@QEAAXHH@Z (0x140150570)
// Verified: Direct IDA decompilation - updates share point by force for maze/district
void CGocRecode::UpdateSharePointByForce(int nMazeID, int nAddPoint) {
    // IDA 逻辑摘要:
    // 1. 获取 TB_MAZE_INFO 从 nMazeID
    // 2. 获取 Maze_Group 和 TB_DS_POINT
    // 3. 查找/更新 m_mapMazeGroupState:
    //    - 如果存在: 增加 nSharePoint，检查上限
    //    - 如果不存在: 创建新条目
    // 4. 更新 m_mapMazeEpisodeState
    // 5. 更新 m_mapDistrictState
    // 6. 发送 DB 包 (3/0x4A)
    // 7. 发送客户端包 (3/0x58) PS_UPDATE_SHARE_POINT

    // TODO: 需要完整外部依赖
    // - XResourceMgr::GetTB_MAZE_INFO, GetTB_DS_POINT
    // - XSendDBPacket, XSendPacket
    // - CQuestCondition::GetQuestID
    // - CGocNetwork::Send

    (void)nMazeID;
    (void)nAddPoint;
    GreenDamTan_log_debug("game.contents", "<MAZE_DSPOINT> UpdateSharePointByForce - MazeID:%d, AddPoint:%d", nMazeID, nAddPoint);
}

// IDA: ?ResetSharePoint@CGocRecode@@QEAAXXZ (0x140150CA0)
// Verified: Direct IDA decompilation - resets all share point data
void CGocRecode::ResetSharePoint() {
    // IDA 逻辑:
    // 1. 清空 m_mapDistrictState
    // 2. 清空 m_mapMazeEpisodeState
    // 3. 清空 m_mapMazeGroupState
    // 4. 清空 m_mapMazeClearInfo
    // 5. 发送 DB 包 (3/0x48)
    // 6. 发送客户端包 (3/0x59)

    m_mapDistrictState.clear();
    m_mapMazeEpisodeState.clear();
    m_mapMazeGroupState.clear();
    m_mapMazeClearInfo.clear();

    // TODO: 需要完整外部依赖发送网络包
    // - XSendDBPacket, XSendPacket
    // - CQuestCondition::GetQuestID
    // - XGameServer::SendDBGame

    GreenDamTan_log_debug("game.contents", "CGocRecode::ResetSharePoint - All share point data cleared");
}

// ============================================================================
// Killed User Functions
// ============================================================================

// IDA: ?SetKilledUserInfo@CGocRecode@@QEAAXAEAUPS_KILLED_USER_INFOS@@@Z (0x14014ECC0)
// Verified: Direct IDA decompilation - initializes killed user tracking from DB load
void CGocRecode::SetKilledUserInfo(PS_KILLED_USER_INFOS& stInfos) {
    // IDA: Store init time
    m_nInitKilledUserTime = stInfos.nInitTime;

    // IDA: Iterate through killed user list and insert into m_mapKilledUser
    for (const auto& stInfo : stInfos.vecKilledUser) {
        // IDA: Insert pair (dwUCID, nCount) into map
        m_mapKilledUser[stInfo.dwUCID] = stInfo.nCount;
    }
}

// IDA: ?AddKilledUser@CGocRecode@@QEAAXPEAVCUser@@@Z (0x14014ED80)
// Verified: Direct IDA decompilation - PvP kill recording with BP rewards
void CGocRecode::AddKilledUser(CUser* pKilledUser) {
    // IDA: Get owner CMover and dynamic_cast to CUser
    // CMover* pMover = GetOwnerGO();
    // CUser* pUser = dynamic_cast<CUser*>(pMover);
    CUser* pUser = GetOwnerUser();
    if (!pUser || !pKilledUser) {
        return;
    }

    // IDA: Increment PvP kill count
    int nCount[6] = {1, 0, 0, 0, 0, 0};
    // TODO: pUser->stMyCharInfoEx()->dwPvPKillCount++;

    // IDA: Send DB packet (main=3, sub=0x35) for kill count update
    // XSendDBPacket xSendDBPacket(pUser->GetObject(), 3, 0x35);
    // xSendDBPacket << pUser->GetUAID();
    // xSendDBPacket << nCount[0];
    // XGameServer::SendDBGame(&xSendDBPacket);

    // IDA: Update achievement (type=0x3B, count=1)
    // auto pAchieve = pMover->GetGOC<CGocAchieve>();
    // if (pAchieve) {
    //     pAchieve->UpdateCollect(0x3B, 1, 0);
    // }

    // IDA: Get killer level
    std::uint8_t byKillerLevel = pUser->GetLevel();
    // IDA: Get killed user level
    std::uint8_t byKilledLevel = pKilledUser->GetLevel();

    // IDA: Check level difference condition (killed level + 10 > killer level)
    if (byKilledLevel + 10 > byKillerLevel) {
        // IDA: Get killed user's actor ID
        // unsigned int dwKilledActorID = pKilledUser->GetActorID();

        // IDA: Check if already killed this user
        auto it = m_mapKilledUser.find(/*dwKilledActorID*/ 0);
        if (it == m_mapKilledUser.end()) {
            // IDA: Not in map - first kill of this user

            // IDA: Get inventory components
            // auto pKillerInven = pMover->GetGOC<CGocInventory>();
            // auto pKilledInven = pKilledUser->GetGOC<CGocInventory>();

            // IDA: Get nation type
            // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            // XOption* pOption = pServer->GetOption();
            // int nNationType = pOption->GetNationType();

            // IDA: Add BP to killer (type=2, reason=0x28)
            // if (nNationType == 2) {
            //     pKillerInven->AddBP(2, 0x28);
            // } else {
            //     std::wstring strKilledName = pKilledUser->GetName();
            //     pKillerInven->AddLimitBP(2, strKilledName, 0x28);
            // }

            // IDA: Deduct BP from killed user (type=-1, reason=0x29)
            // if (nNationType == 2) {
            //     pKilledInven->AddBP(-1, 0x29);
            // } else {
            //     std::wstring strKillerName = pUser->GetName();
            //     pKilledInven->AddLimitBP(-1, strKillerName, 0x29);
            // }

            // IDA: Update attribute stat (stat=16)
            // auto pAttr = pMover->GetGOC<CGocAttribute>();
            // if (pAttr) {
            //     pAttr->SetStat(16, pMover->GetStat(16), true);
            // }

            // IDA: Insert into m_mapKilledUser
            m_mapKilledUser[/*dwKilledActorID*/ 0] = 1;

            // IDA: Send DB packet (main=3, sub=0x55) for killed user record
            // XSendDBPacket xSendPacket(pUser->GetObject(), 3, 0x55);
            // xSendPacket << pUser->GetUAID();
            // xSendPacket << dwKilledActorID;
            // xSendPacket << nCount[0];
            // XGameServer::SendDBGame(&xSendPacket);
        } else {
            // IDA: Already killed this user - send notification (52701)
            // pUser->SendChatNotify(0, 52701);
            // pKilledUser->SendChatNotify(0, 52701);
            // it->second++;  // Increment kill count
        }
    } else {
        // IDA: Level difference too large - send notification (52700)
        // pUser->SendChatNotify(0, 52700);
        // pKilledUser->SendChatNotify(0, 52700);
    }

    GreenDamTan_log_debug("game.pvp", "CGocRecode::AddKilledUser - PvP kill recorded, KillerLevel:%d, KilledLevel:%d",
                          byKillerLevel, byKilledLevel);
}

// IDA: ?ClearKilledUser@CGocRecode@@QEAAXXZ (0x14014F520)
// Verified: Direct IDA decompilation - clears killed user map and sends DB update
void CGocRecode::ClearKilledUser() {
    // IDA: 清空 m_mapKilledUser
    m_mapKilledUser.clear();

    // IDA: 获取 CGocInventory 并调用 InitLimitBP
    // TODO: 需要外部依赖 CMover::GetGOC<CGocInventory>
    // CGocInventory::InitLimitBP()

    // IDA: 发送 DB 包 (3/0x56) 通知击杀用户清空
    // TODO: 需要外部依赖 XSendDBPacket, XGameServer::SendDBGame
}

// ============================================================================
// Enter Maze Limit Count Functions
// ============================================================================

// IDA: ?LoadEnterMazeLimitCount@CGocRecode@@QEAAXAEAUPS_MAZE_ENTER_LIMIT_COUNT_LIST@@@Z (0x140150EC0)
void CGocRecode::LoadEnterMazeLimitCount(/*PS_MAZE_ENTER_LIMIT_COUNT_LIST& stList*/) {
    m_bLoadMazeEnterLimitCount = true;
}

// IDA: ?UpdateEnterMazeLimitCount@CGocRecode@@QEAAXGEH_N@Z (0x140151160)
void CGocRecode::UpdateEnterMazeLimitCount(unsigned short wMazeID, unsigned char byType, int nCount, bool bSend) {
    // TODO: 需要外部依赖
    (void)wMazeID;
    (void)byType;
    (void)nCount;
    (void)bSend;
}

// IDA: ?ClearEnterMazeLimitCount@CGocRecode@@QEAAX_J@Z (0x140151800)
// Verified: Direct IDA decompilation - clears maze enter limit count map and sends DB sync
void CGocRecode::ClearEnterMazeLimitCount(__int64 nClearTime) {
    // IDA: Store clear time
    m_nInitEnterMazeLimitCountTime = nClearTime;

    // IDA: Clear the map
    m_mapEnterMazeLimitCount.clear();

    // IDA: Send DB packet (main=0x43, sub=5)
    // PS_MAZE_ENTER_LIMIT_COUNT_CLEAR stClear;
    // stClear.dwActorID = GetQuestID();
    // stClear.nClearTime = m_nInitEnterMazeLimitCountTime;
    // XSendDBPacket xSendDBPacket(pObject, 0x43, 5);
    // xSendDBPacket << stClear;
    // XGameServer::SendDBGame(&xSendDBPacket);

    // IDA: Send client packet (main=4, sub=0x44)
    // XSendPacket sendPacket(4, 0x44);
    // sendPacket << GetQuestID();
    // CGocNetwork::Send(&sendPacket);

    GreenDamTan_log_debug("game.contents", "CGocRecode::ClearEnterMazeLimitCount - ClearTime:%lld", nClearTime);
}

// IDA: ?GetEnterMazeLimitCount@CGocRecode@@QEAAGG@Z (0x140151A10)
unsigned short CGocRecode::GetEnterMazeLimitCount() {
    // TODO: 需要外部依赖 - 计算总进入次数
    return 0;
}

// IDA: ?GetEnterMazeLimitPCBangCount@CGocRecode@@QEAAGG@Z (0x140151BB0)
unsigned short CGocRecode::GetEnterMazeLimitPCBangCount() {
    // TODO: 需要外部依赖 - 计算PC Bang进入次数
    return 0;
}

// IDA: ?SendEnterMazeLimitCount@CGocRecode@@QEAAXXZ (0x140153CB0)
// TODO: Requires CUser::GetQuestID, CUser::Send, CUser::BridgeSend, PS_MAZE_ENTER_LIMIT_COUNT_GROUP fields
void CGocRecode::SendEnterMazeLimitCount() {
    // TODO: Implement when CUser methods and structure fields are available
    GreenDamTan_log_debug("game.contents", "CGocRecode::SendEnterMazeLimitCount (TODO)");
}

// IDA: ?LoadEnterGroupLimitCount@CGocRecode@@QEAAXAEAUPS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST@@@Z (0x140151D50)
void CGocRecode::LoadEnterGroupLimitCount(/*PS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST& stList*/) {
    // TODO: 需要外部依赖
}

// IDA: ?UpdateEnterGroupLimitCount@CGocRecode@@QEAAXGE_N0@Z (0x1401520A0)
void CGocRecode::UpdateEnterGroupLimitCount(unsigned short wGroupID, unsigned char byType, bool bAccount, bool bSend) {
    // TODO: 需要外部依赖
    (void)wGroupID;
    (void)byType;
    (void)bAccount;
    (void)bSend;
}

// IDA: ?ClearEnterGroupLimitCount@CGocRecode@@QEAAX_JE@Z (0x140152C60)
// Verified: Direct IDA decompilation - clears group limit count by type and sends DB sync
void CGocRecode::ClearEnterGroupLimitCount(__int64 nClearTime, unsigned char byType) {
    // IDA: Get owner user
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: Build clear packet
    // PS_MAZE_ENTER_LIMIT_COUNT_GROUP_CLEAR stClear;
    // stClear.dwUAID = pUser->GetUAID();
    // stClear.dwUCID = GetQuestID();
    // stClear.byType = byType;
    // stClear.nClearTime = nClearTime;

    if (byType == 0) {
        // IDA: Character type - clear m_mapEnterGroupLimitCount_Character
        // Iterate and collect group IDs
        for (const auto& pair : m_mapEnterGroupLimitCount_Character) {
            // stClear.vecDelGroupID.push_back(pair.first);
        }
        SetEnterGroupLimitCountTime_Character(nClearTime);
        m_mapEnterGroupLimitCount_Character.clear();
    } else if (byType == 1) {
        // IDA: Account type - clear m_mapEnterGroupLimitCount_Account
        for (const auto& pair : m_mapEnterGroupLimitCount_Account) {
            // stClear.vecDelGroupID.push_back(pair.first);
        }
        SetEnterGroupLimitCountTime_Account(nClearTime);
        m_mapEnterGroupLimitCount_Account.clear();
    }

    // IDA: Send DB packet (main=0x43, sub=8)
    // XSendDBPacket xSendDBPacket(pObject, 0x43, 8);
    // xSendDBPacket << stClear;
    // XGameServer::SendDBGame(&xSendDBPacket);

    // IDA: Send client packet (main=4, sub=0x47)
    // XSendPacket sendPacket(4, 0x47);
    // sendPacket << stClear;
    // pUser->BridgeSend(&sendPacket);

    GreenDamTan_log_debug("game.contents", "CGocRecode::ClearEnterGroupLimitCount - ClearTime:%lld, Type:%d", nClearTime, byType);
}

// IDA: ?ResetEnterMazeLimiteCount@CGocRecode@@QEAAXH@Z (0x140153020)
// TODO: Requires CUser::GetQuestID, CUser::Send, PS_MAZE_ENTER_LIMIT_COUNT_GROUP fields
void CGocRecode::ResetEnterMazeLimiteCount(int nMazeID) {
    // TODO: Implement when CUser methods and structure fields are available
    GreenDamTan_log_debug("game.contents", "CGocRecode::ResetEnterMazeLimiteCount - MazeID:%d (TODO)", nMazeID);
}

// IDA: ?DecreaseEnterCasualMazeLimiteCount@CGocRecode@@QEAAXXZ (0x140153920)
// TODO: Requires CUser::GetQuestID, CUser::Send
void CGocRecode::DecreaseEnterCasualMazeLimiteCount() {
    // TODO: Implement when CUser methods are available
    GreenDamTan_log_debug("game.contents", "CGocRecode::DecreaseEnterCasualMazeLimiteCount (TODO)");
}

// IDA: ?SetEnterGroupLimitCountTime_Account@CGocRecode@@QEAAX_J@Z (0x140166050)
void CGocRecode::SetEnterGroupLimitCountTime_Account(__int64 nTime) {
    m_nInitEnterGroupLimitCountTime_Account = nTime;
}

// IDA: ?SetEnterGroupLimitCountTime_Character@CGocRecode@@QEAAX_J@Z (0x140166070)
void CGocRecode::SetEnterGroupLimitCountTime_Character(__int64 nTime) {
    m_nInitEnterGroupLimitCountTime_Character = nTime;
}

// ============================================================================
// Ranking Functions
// ============================================================================

// IDA: ?RankingDataUpdate@CGocRecode@@QEAAXKKH@Z (0x140154150)
// TODO: Requires CGocEntity, CUser::GetQuestID, XResourceMgr::GetRankingInfoTable, many other dependencies
void CGocRecode::RankingDataUpdate(unsigned long dwMazeID, unsigned long dwMazePlayTime, int nMonsterKillScore) {
    // TODO: Implement when CGocEntity and other dependencies are available
    (void)dwMazeID;
    (void)dwMazePlayTime;
    (void)nMonsterKillScore;
    GreenDamTan_log_debug("game.contents", "CGocRecode::RankingDataUpdate - MazeID:%lu (TODO)", dwMazeID);
}

// IDA: ?CanRecvRankingReward@CGocRecode@@QEAAHGAEAUST_USER_LAST_RANKING_INFO@@@Z (0x1401553D0)
// TODO: Requires ST_USER_LAST_RANKING_INFO definition
bool CGocRecode::CanRecvRankingReward(unsigned short wRankInfoIndex, void* stLastRank) {
    // TODO: Implement when ST_USER_LAST_RANKING_INFO is available
    (void)wRankInfoIndex;
    (void)stLastRank;
    GreenDamTan_log_debug("game.contents", "CGocRecode::CanRecvRankingReward - Index:%d (TODO)", wRankInfoIndex);
    return false;
}

// IDA: ?SetRankingMyInfo@CGocRecode@@QEAAXG_NAEAUST_USER_RANKING_INFO@@_K@Z (0x1401554B0)
// TODO: Requires CGocEntity, CUser::GetQuestID, CUser::GetLeagueInfo, CUser::GetProfilePhotoID
void CGocRecode::SetRankingMyInfo(unsigned short wRankInfoIndex, bool bLastRanking, void* stMyInfo, unsigned long long dw64SeasonSetCount) {
    // TODO: Implement when CGocEntity and other dependencies are available
    (void)wRankInfoIndex;
    (void)bLastRanking;
    (void)stMyInfo;
    (void)dw64SeasonSetCount;
    GreenDamTan_log_debug("game.contents", "CGocRecode::SetRankingMyInfo - Index:%d (TODO)", wRankInfoIndex);
}

// IDA: ?ReqRankingList@CGocRecode@@QEAA_NAEAUPS_RANKING_LIST_REQ@@@Z (0x140155BC0)
// TODO: Requires CGocEntity, CUser::GetQuestID, CGocNetwork::SendErrorMessage
bool CGocRecode::ReqRankingList(void* psReq) {
    // TODO: Implement when CGocEntity and other dependencies are available
    (void)psReq;
    GreenDamTan_log_debug("game.contents", "CGocRecode::ReqRankingList (TODO)");
    return false;
}

// IDA: ?ResRankingMyInfo@CGocRecode@@QEAAXAEAUPS_DB_MY_RANKING_INFO_RES@@@Z (0x140156540)
// TODO: Requires CGocNetwork, CUser::Send, XRankingMgr::GetRankingList
void CGocRecode::ResRankingMyInfo(void* psRes) {
    // TODO: Implement when dependencies are available
    (void)psRes;
    GreenDamTan_log_debug("game.contents", "CGocRecode::ResRankingMyInfo (TODO)");
}

// IDA: ?ReqRankingReward@CGocRecode@@QEAAXAEAUPS_RANKING_REWARD_REQ@@@Z (0x1401569D0)
// TODO: Requires CGocEntity, CGocInventory, CGocNetwork, CUser::GetQuestID, many other dependencies
void CGocRecode::ReqRankingReward(void* psReward) {
    // TODO: Implement when dependencies are available
    (void)psReward;
    GreenDamTan_log_debug("game.contents", "CGocRecode::ReqRankingReward (TODO)");
}

// IDA: ?ResRankingReward@CGocRecode@@QEAAXAEAUPS_DB_RANKING_REWARD@@@Z (0x140158410)
// TODO: Requires CGocNetwork, CUser::GetQuestID
void CGocRecode::ResRankingReward(void* psRes) {
    // TODO: Implement when dependencies are available
    (void)psRes;
    GreenDamTan_log_debug("game.contents", "CGocRecode::ResRankingReward (TODO)");
}

// IDA: ?Ranking_Cheat@CGocRecode@@QEAA_NHHH@Z (0x14015BB20)
// TODO: Requires CUser::GetGMPower, XOption::GetSystemType, XRankingMgr
bool CGocRecode::Ranking_Cheat(int nType, int nValue1, int nValue2) {
    // TODO: Implement when dependencies are available
    (void)nType;
    (void)nValue1;
    (void)nValue2;
    GreenDamTan_log_debug("game.contents", "CGocRecode::Ranking_Cheat - Type:%d (TODO)", nType);
    return false;
}

// ============================================================================
// Over-Indulgence Functions
// ============================================================================

// IDA: ?SetOverIndulgence@CGocRecode@@QEAAX_J000@Z (0x158A90)
void CGocRecode::SetOverIndulgence(__int64 nConnectTerm, __int64 nDisconnectTerm, __int64 nAlertTick, __int64 nPrevTick) {
    m_nIndulgenceConnectTermTick = static_cast<int>(nConnectTerm);
    m_nIndulgenceDisconnectTermTick = static_cast<int>(nDisconnectTerm);
    m_nIndulgenceAlertTick = static_cast<int>(nAlertTick);
    m_nIndulgencePrevTick = static_cast<int>(nPrevTick);
}

// IDA: ?SaveOverIndulgence@CGocRecode@@QEAAXXZ (0x1401590B0)
// Verified: Direct IDA decompilation - saves over-indulgence state to DB
void CGocRecode::SaveOverIndulgence() {
    // IDA: Check nation type == 3 (Korea)
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // XOption* pOption = pServer->GetOption();
    // if (pOption->GetNationType() != 3) return;

    // IDA: Check server content option
    // if (!XResourceMgr::GetServerContents(E_SERVER_OPTION_OVER_INDULGENCE)) return;

    // IDA: Get current date
    // __int64 nCurDate = pServer->GetCurDate();

    // IDA: Get owner user
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: Build PS_INDULGENCE_INFO packet
    // PS_INDULGENCE_INFO stUpdate;
    // stUpdate.dwUAID = pUser->GetUAID();
    // stUpdate.nIndulgenceConnectTerm = m_nIndulgenceConnectTermTick;
    // stUpdate.nIndulgenceDisconnectTerm = m_nIndulgenceDisconnectTermTick;
    // stUpdate.nLastDisConnectTime = nCurDate;
    // stUpdate.nLeftAlertTime = m_nIndulgenceAlertTick - GetTickCount64();
    // if (stUpdate.nLeftAlertTime < 0) stUpdate.nLeftAlertTime = 0;

    // IDA: Send DB packet (main=2, sub=0x47)
    // XSendDBPacket xSendDBPacket(pObject, 2, 0x47);
    // xSendDBPacket << stUpdate;
    // XGameServer::SendDBAccount(&xSendDBPacket);

    // IDA: Send DB log
    // ST_LOG_GAME stLog;
    // stLog._nUCID = GetQuestID();
    // stLog._nUAID = pUser->GetUAID();
    // stLog._sMainType = 27;
    // stLog._sSubType = 3;
    // stLog.nParam5 = m_nIndulgenceConnectTermTick;
    // stLog.nParam6 = m_nIndulgenceDisconnectTermTick;
    // wcscpy_s(stLog.szComment, L"INDULGENCE_SAVE");
    // XGameServer::SendDBLog(&stLog);

    GreenDamTan_log_debug("game.indulgence", "CGocRecode::SaveOverIndulgence - ConnectTerm:%d, DisconnectTerm:%d",
                          m_nIndulgenceConnectTermTick, m_nIndulgenceDisconnectTermTick);
}

// IDA: ?UpdateOverIndulgence@CGocRecode@@QEAAXXZ (0x140159400)
// TODO: Requires XOption::GetNationType, CUser::GetQuestID, CGocNetwork, PS_INDULGENCE_ALERT
void CGocRecode::UpdateOverIndulgence() {
    // TODO: Implement when dependencies are available
    GreenDamTan_log_debug("game.contents", "CGocRecode::UpdateOverIndulgence (TODO)");
}

// IDA: ?CheckOverIndulgenceState@CGocRecode@@QEAAHXZ (0x140159840)
int CGocRecode::CheckOverIndulgenceState() {
    return m_nIndulgenceState;
}

// IDA: ?GetIndulgenceDropRate@CGocRecode@@QEAAMXZ (0x140159890)
float CGocRecode::GetIndulgenceDropRate() {
    // TODO: 需要外部依赖 - 获取防沉迷掉落率
    // 根据防沉迷状态返回不同的掉落率
    switch (m_nIndulgenceState) {
        case 1: return 0.5f;   // 50%
        case 2: return 0.0f;   // 0%
        default: return 1.0f;  // 100%
    }
}

// IDA: ?SetIndulgenceByForce@CGocRecode@@QEAAXHH@Z (0x1401598F0)
void CGocRecode::SetIndulgenceByForce(int nState, int nType) {
    // TODO: 需要复杂外部依赖
    m_nIndulgenceState = nState;
    (void)nType;
}

// IDA: ?SendDBLoadIndulgence@CGocRecode@@QEAAX_N@Z (0x14015A210)
// TODO: Requires CUser::GetObject, XGameServer::SendDBAccount
void CGocRecode::SendDBLoadIndulgence(bool bLogin) {
    // TODO: Implement when dependencies are available
    (void)bLogin;
    GreenDamTan_log_debug("game.contents", "CGocRecode::SendDBLoadIndulgence (TODO)");
}

// IDA: ?ShowIndulgenceInfo@CGocRecode@@QEAAXXZ (0x14015A370)
// TODO: Requires CUser::GetGocNetwork, CGocNetwork::Send
void CGocRecode::ShowIndulgenceInfo() {
    // TODO: Implement when dependencies are available
    GreenDamTan_log_debug("game.contents", "CGocRecode::ShowIndulgenceInfo (TODO)");
}

// ============================================================================
// Special Reward Functions
// ============================================================================

// IDA: ?GetSpecialExp@CGocRecode@@QEAAHM@Z (0x14014FA40)
// Verified: Direct IDA decompilation - gets special exp from booster effects
int CGocRecode::GetSpecialExp(int nExp) {
    // IDA: 如果 nExp <= 0，直接返回 0
    if (nExp <= 0) {
        return 0;
    }

    // IDA: 获取 CGocBooster 组件
    // CMover* pMover = GetOwnerGO();
    // auto pBooster = pMover->GetGOC<CGocBooster>();
    // if (!pBooster) return 0;

    // IDA: 获取迷宫奖励经验增加率
    // float fRate = pBooster->GetTotalRate(eBooster_Effect_Maze_Reward_IncExp);
    // if (fRate <= 0.0f) return 0;

    // IDA: 返回额外经验 = nExp * fRate
    // return static_cast<int>(nExp * fRate);

    // TODO: 需要外部依赖 - CMover::GetGOC<CGocBooster>, CGocBooster::GetTotalRate
    return 0;
}

// IDA: ?GetSpecialGold@CGocRecode@@QEAAHH@Z (0x14014FB00)
// Verified: Direct IDA decompilation - gets special gold from booster effects
int CGocRecode::GetSpecialGold(int nGold) {
    // IDA: 如果 nGold <= 0，直接返回 0
    if (nGold <= 0) {
        return 0;
    }

    // IDA: 获取 CGocBooster 组件
    // CMover* pMover = GetOwnerGO();
    // auto pBooster = pMover->GetGOC<CGocBooster>();
    // if (!pBooster) return 0;

    // IDA: 获取迷宫奖励金币增加率
    // float fRate = pBooster->GetTotalRate(eBooster_Effect_Maze_Reward_IncMoney);
    // if (fRate <= 0.0f) return 0;

    // IDA: 返回额外金币 = nGold * fRate
    // return static_cast<int>(nGold * fRate);

    // TODO: 需要外部依赖 - CMover::GetGOC<CGocBooster>, CGocBooster::GetTotalRate
    return 0;
}

// IDA: ?SendLeagueWealth@CGocRecode@@QEAAXFH@Z (0x1401588F0)
// TODO: Requires CUser::GetQuestID, CUser::GetLeagueID, XGameServer::GetCommunitySocket
void CGocRecode::SendLeagueWealth(float fExp, int nGold) {
    // TODO: Implement when dependencies are available
    (void)fExp;
    (void)nGold;
    GreenDamTan_log_debug("game.contents", "CGocRecode::SendLeagueWealth (TODO)");
}

// ============================================================================
// Tutorial Functions
// ============================================================================

// IDA: ?SetClearTurtorial@CGocRecode@@QEAAX_N0@Z (0x14015A4C0)
void CGocRecode::SetClearTurtorial(bool bClear, bool bSend) {
    m_bClearTurtorial = bClear;
    // TODO: 发送更新到客户端/DB
    (void)bSend;
}

// IDA: ?GetDailyBaseMazeID@CGocRecode@@QEAA_NGG_NAEAGAEAEAEAH@Z (0x14015A700)
bool CGocRecode::GetDailyBaseMazeID(unsigned short wMazeID, unsigned short wGroupID, bool bFlag,
                                     unsigned short& wOut1, unsigned char& byOut2, unsigned char& byOut3, int& nOut) {
    // TODO: 需要复杂外部依赖
    (void)wMazeID;
    (void)wGroupID;
    (void)bFlag;
    wOut1 = 0;
    byOut2 = 0;
    byOut3 = 0;
    nOut = 0;
    return false;
}

// ============================================================================
// Tool Reward Functions
// ============================================================================

// IDA: ?GetToolClearRewardInfo@CGocRecode@@QEAAXAEAUPS_RES_TOOL_REWARD_INFO@@@Z (0x14015ACE0)
void CGocRecode::GetToolClearRewardInfo(void* pInfo) {
    // TODO: 需要外部依赖
    (void)pInfo;
}

// IDA: ?ClearToolInfo@CGocRecode@@QEAAXXZ (0x14015AD40)
void CGocRecode::ClearToolInfo() {
    memset(m_stToolRewardInfo, 0, sizeof(m_stToolRewardInfo));
}

// IDA: ?MonsterKillScoreReward@CGocRecode@@QEAAXHE_K@Z (0x14015B6D0)
// TODO: Requires PS_REWARD_SCORE, CUser::Send, CUser::GetQuestID, CUser::GetMapInsID
void CGocRecode::MonsterKillScoreReward(int nPoint, unsigned char byMazeType, unsigned long long dwClearTime) {
    // TODO: Implement when dependencies are available
    (void)nPoint;
    (void)byMazeType;
    (void)dwClearTime;
    GreenDamTan_log_debug("game.contents", "CGocRecode::MonsterKillScoreReward - Point:%d (TODO)", nPoint);
}

// ============================================================================
// Share Point Reward Functions
// ============================================================================

// (ApplyRewardItemForSharePoint is defined earlier in the file)
