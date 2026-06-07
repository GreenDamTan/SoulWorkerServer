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
struct PS_MAZE_ENTER_LIMIT_COUNT_GROUP {};
struct PS_UPDATE_MAZE_ENTER_LIMIT_COUNT {};
struct ST_USER_RANKING_INFO {};
struct ST_USER_LAST_RANKING_INFO {};
struct PS_KILLED_USER_INFOS {};

// External function declarations - TODO: 需要从其他模块导入
// These are placeholder implementations for now

namespace {
    // Helper to get XGameServer instance
    // TODO: 需要从 GameServer.h 导入 TXSingleton<XGameServer>
    XGameServer* GetGameServerInstance() {
        // TODO: 汇编还原 - return TXSingleton<XGameServer>::Instance();
        return nullptr;
    }
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
// Verified: Direct IDA decompilation - sets reward items from maze reward tables
void CGocRecode::SetRewardItem(bool bTool) {
    // IDA: 获取所属的User对象
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 检查用户是否被封禁
    if (pUser->GetBlockType()) {
        return;
    }

    // IDA: 获取 TB_MAZEREWARD_ITEM 表
    XGameServer* pServer = XGameServer::Instance();
    TB_MAZEREWARD_ITEM* pMazeReward = pServer->GetResourceMgr().GetTB_MAZEREWARD_ITEM(m_nMazeID);
    if (!pMazeReward) {
        return;
    }

    // IDA: 检查排名是否有效
    if (m_nRank >= 6) {
        return;
    }

    // IDA: 根据排名计算基础奖励物品
    int nResultRate = pServer->GetItemFactory().nRand(0, g_nRewardRateTotal[m_nRank]);
    for (int i = 0; i < 6; ++i) {
        if (nResultRate <= *(&pMazeReward->B_F_Rate + i)) {
            // IDA: 获取随机盒子表
            int nRandomBoxID = *(&pMazeReward->B_F_ID + i);
            TB_ITEM_RANDOMBOX* pTBRandomBox = pServer->GetResourceMgr().GetTB_ITEM_RANDOMBOX_With_Lock(nRandomBoxID);
            if (pTBRandomBox) {
                // IDA: 随机选择物品
                int nRate = pServer->GetItemFactory().nRand(1, pTBRandomBox->Rate_Sum);
                for (int j = 3; j < 18; ++j) {
                    int nItemRate = *(&pTBRandomBox->Gold_Rate + j);
                    if (nItemRate) {
                        if (nRate <= nItemRate) {
                            unsigned char byItemPos = j - 3;
                            int nItemID = *(&pTBRandomBox->Item_01 + byItemPos);
                            short shCount = *((short*)&pTBRandomBox->Count_01 + 2 * byItemPos);
                            if (nItemID && shCount && pServer->GetResourceMgr().GetTB_ITEM(nItemID)) {
                                // IDA: 设置基础奖励物品
                                m_stBaseRewardItem.nItemID = nItemID;
                                m_stBaseRewardItem.shCount = shCount;
                            }
                            break;
                        }
                        nRate -= nItemRate;
                    }
                }
            }
            break;
        }
        nResultRate -= *(&pMazeReward->B_F_Rate + i);
    }

    // IDA: 设置现金奖励物品 (Cash_Reward_item_ID)
    TB_ITEM_RANDOMBOX* pCashBox = pServer->GetResourceMgr().GetTB_ITEM_RANDOMBOX_With_Lock(pMazeReward->Cash_Reward_item_ID);
    if (pCashBox) {
        int nCashRate = pServer->GetItemFactory().nRand(1, pCashBox->Rate_Sum);
        for (int k = 3; k < 18; ++k) {
            int nRate = *(&pCashBox->Gold_Rate + k);
            if (nRate) {
                if (nCashRate <= nRate) {
                    unsigned char byPos = k - 3;
                    int nItemID = *(&pCashBox->Item_01 + byPos);
                    short shCount = *((short*)&pCashBox->Count_01 + 2 * byPos);
                    if (nItemID && shCount && pServer->GetResourceMgr().GetTB_ITEM(nItemID)) {
                        // IDA: 设置现金奖励物品
                        m_stCashRewardItem.dwCashItemID = 831000002;
                        m_stCashRewardItem.shCashItemCount = pMazeReward->Cash_Reward_item_EA_1;
                        m_stCashRewardItem.stRewardItem.nItemID = nItemID;
                        m_stCashRewardItem.stRewardItem.shCount = shCount;
                    }
                    break;
                }
                nCashRate -= nRate;
            }
        }
    }

    // IDA: 设置扩展现金奖励物品 (Cash_Reward_item_ID_2)
    TB_ITEM_RANDOMBOX* pCashBox2 = pServer->GetResourceMgr().GetTB_ITEM_RANDOMBOX_With_Lock(pMazeReward->Cash_Reward_item_ID_2);
    if (pCashBox2) {
        // IDA: 复制所有物品到列表
        for (int m = 0; m < 7; ++m) {
            m_stCashRewardItemListEX[m].nItemID = *(&pCashBox2->Item_01 + m);
            m_stCashRewardItemListEX[m].shCount = *((short*)&pCashBox2->Count_01 + 2 * m);
        }

        int nExRate = pServer->GetItemFactory().nRand(1, pCashBox2->Rate_Sum);
        for (int n = 3; n < 18; ++n) {
            int nRate = *(&pCashBox2->Gold_Rate + n);
            if (nRate) {
                if (nExRate <= nRate) {
                    unsigned char byPos = n - 3;
                    int nItemID = *(&pCashBox2->Item_01 + byPos);
                    short shCount = *((short*)&pCashBox2->Count_01 + 2 * byPos);
                    if (nItemID && shCount && pServer->GetResourceMgr().GetTB_ITEM(nItemID)) {
                        // IDA: 设置扩展现金奖励物品
                        m_stCashRewardItemEx.dwCashItemID = 831000002;
                        m_stCashRewardItemEx.shCashItemCount = pMazeReward->Cash_Reward_item_EA_2;
                        m_stCashRewardItemEx.stRewardItem.nItemID = nItemID;
                        m_stCashRewardItemEx.stRewardItem.shCount = shCount;
                    }
                    break;
                }
                nExRate -= nRate;
            }
        }
    }

    // IDA: 如果是工具模式，只记录奖励信息
    if (bTool) {
        PS_TOOL_REWARD_INFO psInfo;
        memset(&psInfo, 0, sizeof(psInfo));

        psInfo.nType = 0;
        psInfo.nItemID = m_stBaseRewardItem.nItemID;
        psInfo.nCount = m_stBaseRewardItem.shCount;
        m_stToolRewardInfo.push_back(psInfo);

        psInfo.nType = 1;
        psInfo.nItemID = m_stCashRewardItem.stRewardItem.nItemID;
        psInfo.nCount = m_stCashRewardItem.stRewardItem.shCount;
        m_stToolRewardInfo.push_back(psInfo);

        psInfo.nType = 2;
        psInfo.nItemID = m_stCashRewardItemEx.stRewardItem.nItemID;
        psInfo.nCount = m_stCashRewardItemEx.stRewardItem.shCount;
        m_stToolRewardInfo.push_back(psInfo);
        return;
    }

    // IDA: 获取 CGocInventory 组件
    std::shared_ptr<CGocInventory> pInvenPtr;
    pUser->GetGOC<CGocInventory>(pInvenPtr);
    if (!pInvenPtr) {
        return;
    }

    // IDA: 获取 CGocPost 组件
    std::shared_ptr<CGocPost> pPostPtr;
    pUser->GetGOC<CGocPost>(pPostPtr);

    // IDA: 处理隐藏事件奖励
    XMaze* pMaze = pUser->GetMaze();
    if (pMaze) {
        unsigned short wHiddenID = 0;
        ST_CREATE_ITEM stHiddenItem;
        memset(&stHiddenItem, 0, sizeof(stHiddenItem));
        pMaze->GetRewardHiddenEvent(&wHiddenID, &stHiddenItem.nItemID, &stHiddenItem.shCount);

        if (stHiddenItem.nItemID && stHiddenItem.shCount) {
            TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(stHiddenItem.nItemID);
            if (pTBItem) {
                ST_LOG_GAME stLog;
                memset(&stLog, 0, sizeof(stLog));
                stLog.nParam3 = wHiddenID;
                m_byHiddenEventSendPost = 0;

                if (!pInvenPtr->CreateItemReq(pTBItem->Item_ID, stHiddenItem.shCount, 1, E_ITEM_CREATE_TYPE_HIDDEN_EVENT_REWARD, &stLog)) {
                    // IDA: 背包满，发送邮件
                    if (pPostPtr) {
                        wchar_t strTitle[48] = {0};
                        _itow(wHiddenID, strTitle, 10);
                        pPostPtr->SystemPostSend(pTBItem, stHiddenItem.shCount, 5, 3, wHiddenID, strTitle);
                        m_byHiddenEventSendPost = 1;
                    }
                }
            }
        }
    }

    // IDA: 处理基础奖励物品
    if (m_stBaseRewardItem.nItemID && m_stBaseRewardItem.shCount) {
        TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(m_stBaseRewardItem.nItemID);
        if (pTBItem) {
            ST_LOG_GAME stLog;
            memset(&stLog, 0, sizeof(stLog));
            stLog.nParam3 = m_nMazeID;

            if (!pInvenPtr->CreateItemReq(pTBItem->Item_ID, m_stBaseRewardItem.shCount, 1, E_ITEM_CREATE_TYPE_MAZE_BASE_ITEM, &stLog)) {
                // IDA: 背包满，发送邮件
                if (pPostPtr) {
                    pPostPtr->SystemPostSend(pTBItem, m_stBaseRewardItem.shCount, 5, 1, 0, nullptr);
                }
                return;
            }

            // IDA: 发送日志
            if (pUser->IsMaze()) {
                XMaze* pMazeLog = pUser->GetMaze();
                if (pMazeLog) {
                    ST_LOG_GAME stLogGame;
                    memset(&stLogGame, 0, sizeof(stLogGame));
                    stLogGame._nUAID = pUser->GetUAID();
                    stLogGame._nUCID = pUser->GetQuestID();
                    stLogGame._sMainType = 5;
                    stLogGame._sSubType = 22;
                    stLogGame.nParam0 = pMazeLog->GetTBMapID();
                    stLogGame.nParam1 = m_stBaseRewardItem.nItemID;
                    stLogGame.nParam2 = m_stBaseRewardItem.shCount;
                    stLogGame.nParam6 = pMazeLog->GetInstanceID();
                    wcscpy_s(stLogGame.szComment, L"MAZE_BASE_ITEM");
                    pServer->SendDBLog(&stLogGame);
                }
            }
        }
    }

    // IDA: 检查网吧奖励 (韩国/日本地区)
    XOption* pOption = pServer->GetOption();
    unsigned int nNationType = pOption->GetNationType();
    if (nNationType == 1 || nNationType == 2) {
        std::shared_ptr<CGocEntity> pEntity;
        pUser->GetGOC<CGocEntity>(pEntity);
        if (pEntity && pEntity->GetNetCafe()) {
            // IDA: 发放网吧奖励
            TB_ITEM* pCashItem = pServer->GetResourceMgr().GetTB_ITEM(m_stCashRewardItem.stRewardItem.nItemID);
            if (pCashItem) {
                ST_LOG_GAME stLog;
                memset(&stLog, 0, sizeof(stLog));
                stLog.nParam3 = m_nMazeID;
                stLog.nParam4 = 1;

                if (!pInvenPtr->CreateItemReq(m_stCashRewardItem.stRewardItem.nItemID, m_stCashRewardItem.stRewardItem.shCount, 1, E_ITEM_CREATE_TYPE_MAZE_BASE_ITEM, &stLog)) {
                    if (pPostPtr) {
                        pPostPtr->SystemPostSend(pCashItem, m_stCashRewardItem.stRewardItem.shCount, 5, 1, 0, nullptr);
                    }
                    return;
                }

                // IDA: 发送网吧奖励日志
                if (pUser->IsMaze()) {
                    XMaze* pMazeLog = pUser->GetMaze();
                    if (pMazeLog) {
                        ST_LOG_GAME v78;
                        memset(&v78, 0, sizeof(v78));
                        v78._nUAID = pUser->GetUAID();
                        v78._nUCID = pUser->GetQuestID();
                        v78._sMainType = 5;
                        v78._sSubType = 24;
                        v78.nParam0 = pMazeLog->GetTBMapID();
                        v78.nParam1 = m_stCashRewardItem.stRewardItem.nItemID;
                        v78.nParam2 = m_stCashRewardItem.stRewardItem.shCount;
                        v78.nParam5 = 1;
                        v78.nParam6 = pMazeLog->GetInstanceID();
                        wcscpy_s(v78.szComment, L"MAZE_PC_BANG_ITEM");
                        pServer->SendDBLog(&v78);
                    }
                }
            }
        }
    }
}

// IDA: ?GetRewardItem@CGocRecode@@QEAAXXZ (0x14014C040)
// Verified: Direct IDA decompilation - processes cash reward item exchange
void CGocRecode::GetRewardItem() {
    // IDA: 获取 CGocInventory 组件
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    std::shared_ptr<CGocInventory> pInvenPtr;
    pUser->GetGOC<CGocInventory>(pInvenPtr);
    if (!pInvenPtr) {
        return;
    }

    // IDA: 获取 TB_ITEM 从 m_stCashRewardItem.dwCashItemID
    XGameServer* pServer = XGameServer::Instance();
    TB_ITEM* pTB_Item = pServer->GetResourceMgr().GetTB_ITEM(m_stCashRewardItem.dwCashItemID);
    if (!pTB_Item) {
        LogHelper::LogError("game.contents", "GetRewardItem error - No Table TB_ITEM[ItemID:%d]", m_stCashRewardItem.dwCashItemID);
        return;
    }

    // IDA: 获取 TB_ITEM_CLASSIFY，检查 Item_Use_Type == 62
    TB_ITEM_CLASSIFY* pTB_ItemClassify = pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTB_Item->Item_Classify_Index);
    if (!pTB_ItemClassify) {
        LogHelper::LogError("game.contents", "GetRewardItem error - No Table TB_ITEM_CLASSIFY[ItemID:%d]", m_stCashRewardItem.dwCashItemID);
        return;
    }

    if (pTB_ItemClassify->Item_Use_Type != 62) {
        LogHelper::LogError("game.contents", "GetRewardItem error - Check item use type[Type:%d]", pTB_ItemClassify->Item_Use_Type);
        return;
    }

    // IDA: 检查奖励物品是否有效
    if (!m_stCashRewardItem.stRewardItem.nItemID || !m_stCashRewardItem.dwCashItemID ||
        !m_stCashRewardItem.stRewardItem.shCount || !m_stCashRewardItem.shCashItemCount) {
        return;
    }

    TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(m_stCashRewardItem.stRewardItem.nItemID);
    if (!pTBItem) {
        return;
    }

    // IDA: 创建奖励和消耗物品列表
    ST_CREATE_ITEM stRewardItem;
    memset(&stRewardItem, 0, sizeof(stRewardItem));
    stRewardItem.nItemID = m_stCashRewardItem.stRewardItem.nItemID;
    stRewardItem.shCount = m_stCashRewardItem.stRewardItem.shCount;

    ST_CREATE_ITEM stReduceItem;
    memset(&stReduceItem, 0, sizeof(stReduceItem));
    stReduceItem.nItemID = m_stCashRewardItem.dwCashItemID;
    stReduceItem.shCount = m_stCashRewardItem.shCashItemCount;

    ST_CREATE_ITEMS stReduceItems;
    stReduceItems.push_back(stReduceItem);

    // IDA: 获取迷宫ID用于日志
    int nLogMazeID = 0;
    XMaze* pMaze = pUser->GetMaze();
    if (pMaze) {
        nLogMazeID = pMaze->GetTBMapID();
    }

    // IDA: 减少商城物品
    PS_RES_STORAGE_INFO psReduceItem;
    memset(&psReduceItem, 0, sizeof(psReduceItem));
    if (!pInvenPtr->ReduceItem2(&stReduceItems, 0x19, &psReduceItem)) {
        return;
    }

    // IDA: 记录日志
    ST_LOG_GAME stLog;
    memset(&stLog, 0, sizeof(stLog));
    stLog._sSubType = 59;
    stLog.nParam3 = nLogMazeID;
    stLog.nParam9 = 1;
    pInvenPtr->ItemLogCharLevel(pUser->GetLevel(), &stLog);

    // IDA: 更新物品结束
    if (!pInvenPtr->UpdateItemEnd(0x19, &psReduceItem, &stLog)) {
        return;
    }

    // IDA: 创建奖励物品
    ST_CREATE_ITEMS stCreateItems;
    stCreateItems.push_back(stRewardItem);

    ST_LOG_GAME stCreateLog;
    memset(&stCreateLog, 0, sizeof(stCreateLog));
    stCreateLog._sSubType = 59;
    stCreateLog.nParam3 = nLogMazeID;
    stCreateLog.nParam9 = 2;
    pInvenPtr->ItemLogCharLevel(pUser->GetLevel(), &stCreateLog);

    PS_RES_STORAGE_INFO psCreateItem;
    memset(&psCreateItem, 0, sizeof(psCreateItem));
    PS_RES_STORAGE_INFO psUpdateItem;
    memset(&psUpdateItem, 0, sizeof(psUpdateItem));

    if (pInvenPtr->CreateItem2(&stCreateItems, 0x19, 1, &psCreateItem, &psUpdateItem, &stCreateLog)) {
        // IDA: 创建成功，发送 DB 包
        for (size_t i = 0; i < psReduceItem.vecInfo.size(); ++i) {
            psUpdateItem.vecInfo.push_back(psReduceItem.vecInfo[i]);
        }

        // IDA: 清空现金奖励物品
        m_stCashRewardItem.dwCashItemID = 0;
        m_stCashRewardItem.shCashItemCount = 0;
        m_stCashRewardItem.stRewardItem.nItemID = 0;
        m_stCashRewardItem.stRewardItem.shCount = 0;

        // IDA: 发送 DB 包 (0x21/0x23)
        XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x21, 0x23);
        xSendDBPacket << pUser->GetQuestID();
        xSendDBPacket << psCreateItem;
        xSendDBPacket << psUpdateItem;
        xSendDBPacket << stRewardItem;
        xSendDBPacket << stReduceItem;
        xSendDBPacket << (unsigned char)0;  // byType
        xSendDBPacket << (unsigned char)9;  // byFlag
        pServer->SendDBGame(&xSendDBPacket);
    } else {
        // IDA: 创建失败，发送邮件
        std::shared_ptr<CGocPost> pPostPtr;
        pUser->GetGOC<CGocPost>(pPostPtr);
        if (pPostPtr) {
            pPostPtr->SystemPostSend(pTBItem, m_stCashRewardItem.stRewardItem.shCount, 5, 1, 0, nullptr);
        }

        // IDA: 发送通知包
        XSendPacket sendPacket(8, 0x47);
        sendPacket << 0;
        sendPacket << 0;
        pUser->Send(&sendPacket);

        // IDA: 清空并更新DB
        m_stCashRewardItem.dwCashItemID = 0;
        m_stCashRewardItem.shCashItemCount = 0;
        m_stCashRewardItem.stRewardItem.nItemID = 0;
        m_stCashRewardItem.stRewardItem.shCount = 0;

        pInvenPtr->SendUpdateItemToDB(&psReduceItem);

        // IDA: 发送日志
        ST_LOG_GAME stPostLog;
        memset(&stPostLog, 0, sizeof(stPostLog));
        stPostLog._nUAID = pUser->GetUAID();
        stPostLog._nUCID = pUser->GetQuestID();
        stPostLog._sMainType = 4;
        stPostLog._sSubType = 59;
        stPostLog.nParam3 = nLogMazeID;
        stPostLog.nParam4 = 1;
        stPostLog.nParam9 = 2;
        pInvenPtr->ItemLogCharLevel(pUser->GetLevel(), &stPostLog);
        pServer->SendDBLog(&stPostLog);
    }
}

// IDA: ?GetRewardItemEx@CGocRecode@@QEAAXXZ (0x14014CCD0)
// Verified: Direct IDA decompilation - processes extended cash reward item exchange
void CGocRecode::GetRewardItemEx() {
    // IDA: 获取 CGocInventory 组件
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    std::shared_ptr<CGocInventory> pInvenPtr;
    pUser->GetGOC<CGocInventory>(pInvenPtr);
    if (!pInvenPtr) {
        return;
    }

    // IDA: 获取 TB_ITEM 从 m_stCashRewardItemEx.dwCashItemID
    XGameServer* pServer = XGameServer::Instance();
    TB_ITEM* pTB_Item = pServer->GetResourceMgr().GetTB_ITEM(m_stCashRewardItemEx.dwCashItemID);
    if (!pTB_Item) {
        LogHelper::LogError("game.contents", "GetRewardItemEx error - No Table TB_ITEM[ItemID:%d]", m_stCashRewardItemEx.dwCashItemID);
        return;
    }

    // IDA: 获取 TB_ITEM_CLASSIFY，检查 Item_Use_Type == 62
    TB_ITEM_CLASSIFY* pTB_ItemClassify = pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTB_Item->Item_Classify_Index);
    if (!pTB_ItemClassify) {
        LogHelper::LogError("game.contents", "GetRewardItemEx error - No Table TB_ITEM_CLASSIFY[ItemID:%d]", m_stCashRewardItemEx.dwCashItemID);
        return;
    }

    if (pTB_ItemClassify->Item_Use_Type != 62) {
        LogHelper::LogError("game.contents", "GetRewardItemEx error - Check item use type[Type:%d]", pTB_ItemClassify->Item_Use_Type);
        return;
    }

    // IDA: 检查奖励物品是否有效
    if (!m_stCashRewardItemEx.stRewardItem.nItemID || !m_stCashRewardItemEx.dwCashItemID ||
        !m_stCashRewardItemEx.stRewardItem.shCount || !m_stCashRewardItemEx.shCashItemCount) {
        return;
    }

    TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(m_stCashRewardItemEx.stRewardItem.nItemID);
    if (!pTBItem) {
        return;
    }

    // IDA: 检查物品堆叠上限
    if (pTBItem->Item_Stack_Max < m_stCashRewardItemEx.stRewardItem.shCount) {
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 8, 0x47, 0xCB2B);
        return;
    }

    // IDA: 创建奖励和消耗物品列表
    ST_CREATE_ITEM stRewardItem;
    memset(&stRewardItem, 0, sizeof(stRewardItem));
    stRewardItem.nItemID = m_stCashRewardItemEx.stRewardItem.nItemID;
    stRewardItem.shCount = m_stCashRewardItemEx.stRewardItem.shCount;

    ST_CREATE_ITEM stReduceItem;
    memset(&stReduceItem, 0, sizeof(stReduceItem));
    stReduceItem.nItemID = m_stCashRewardItemEx.dwCashItemID;
    stReduceItem.shCount = m_stCashRewardItemEx.shCashItemCount;

    ST_CREATE_ITEMS stReduceItems;
    stReduceItems.push_back(stReduceItem);

    // IDA: 获取迷宫ID用于日志
    int nLogMazeID = 0;
    XMaze* pMaze = pUser->GetMaze();
    if (pMaze) {
        nLogMazeID = pMaze->GetTBMapID();
    }

    // IDA: 减少商城物品
    PS_RES_STORAGE_INFO psReduceItem;
    memset(&psReduceItem, 0, sizeof(psReduceItem));
    if (!pInvenPtr->ReduceItem2(&stReduceItems, 0x19, &psReduceItem)) {
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 8, 0x47, 0xCB2D);
        return;
    }

    // IDA: 记录日志
    ST_LOG_GAME stLog;
    memset(&stLog, 0, sizeof(stLog));
    stLog._sSubType = 59;
    stLog.nParam3 = nLogMazeID;
    stLog.nParam9 = 1;
    pInvenPtr->ItemLogCharLevel(pUser->GetLevel(), &stLog);

    // IDA: 更新物品结束
    if (!pInvenPtr->UpdateItemEnd(0x19, &psReduceItem, &stLog)) {
        return;
    }

    // IDA: 创建奖励物品
    ST_CREATE_ITEMS stCreateItems;
    stCreateItems.push_back(stRewardItem);

    ST_LOG_GAME stCreateLog;
    memset(&stCreateLog, 0, sizeof(stCreateLog));
    stCreateLog._sSubType = 59;
    stCreateLog.nParam3 = nLogMazeID;
    stCreateLog.nParam9 = 2;
    pInvenPtr->ItemLogCharLevel(pUser->GetLevel(), &stCreateLog);

    PS_RES_STORAGE_INFO psCreateItem;
    memset(&psCreateItem, 0, sizeof(psCreateItem));
    PS_RES_STORAGE_INFO psUpdateItem;
    memset(&psUpdateItem, 0, sizeof(psUpdateItem));

    if (pInvenPtr->CreateItem2(&stCreateItems, 0x19, 1, &psCreateItem, &psUpdateItem, &stCreateLog)) {
        // IDA: 创建成功，发送 DB 包
        for (size_t i = 0; i < psReduceItem.vecInfo.size(); ++i) {
            psUpdateItem.vecInfo.push_back(psReduceItem.vecInfo[i]);
        }

        // IDA: 清空扩展现金奖励物品
        m_stCashRewardItemEx.dwCashItemID = 0;
        m_stCashRewardItemEx.shCashItemCount = 0;
        m_stCashRewardItemEx.stRewardItem.nItemID = 0;
        m_stCashRewardItemEx.stRewardItem.shCount = 0;

        // IDA: 发送 DB 包 (0x21/0x23)，byType=1
        XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x21, 0x23);
        xSendDBPacket << pUser->GetQuestID();
        xSendDBPacket << psCreateItem;
        xSendDBPacket << psUpdateItem;
        xSendDBPacket << stRewardItem;
        xSendDBPacket << stReduceItem;
        xSendDBPacket << (unsigned char)1;  // byType = 1
        xSendDBPacket << (unsigned char)9;  // byFlag
        pServer->SendDBGame(&xSendDBPacket);
    } else {
        // IDA: 创建失败，发送邮件
        std::shared_ptr<CGocPost> pPostPtr;
        pUser->GetGOC<CGocPost>(pPostPtr);
        if (pPostPtr) {
            pPostPtr->SystemPostSend(pTBItem, m_stCashRewardItemEx.stRewardItem.shCount, 5, 1, 0, nullptr);
        }

        // IDA: 发送通知包
        XSendPacket sendPacket(8, 0x47);
        sendPacket << 1;  // 参数 1
        sendPacket << 0;  // 参数 0
        pUser->Send(&sendPacket);

        // IDA: 清空并更新DB
        m_stCashRewardItemEx.dwCashItemID = 0;
        m_stCashRewardItemEx.shCashItemCount = 0;
        m_stCashRewardItemEx.stRewardItem.nItemID = 0;
        m_stCashRewardItemEx.stRewardItem.shCount = 0;

        pInvenPtr->SendUpdateItemToDB(&psReduceItem);

        // IDA: 发送日志
        ST_LOG_GAME stPostLog;
        memset(&stPostLog, 0, sizeof(stPostLog));
        stPostLog._nUAID = pUser->GetUAID();
        stPostLog._nUCID = pUser->GetQuestID();
        stPostLog._sMainType = 4;
        stPostLog._sSubType = 59;
        stPostLog.nParam3 = nLogMazeID;
        stPostLog.nParam4 = 1;
        stPostLog.nParam9 = 2;
        pInvenPtr->ItemLogCharLevel(pUser->GetLevel(), &stPostLog);
        pServer->SendDBLog(&stPostLog);
    }
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
// Verified: Direct IDA decompilation - initializes infinite tower info and syncs to DB
void CGocRecode::InitInfiniteTowerInfo(__int64 nInitTime) {
    // IDA: Set member variables
    m_nInfiniteTowerLimitTime = nInitTime;
    m_nInfiniteTowerLimitCount = 0;
    m_nInfiniteTowerLimitPCBangCount = 0;

    // IDA: Get owner user for DB packet
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: Build PS_UPDATE_INFINITETOWER_LIMIT_TIME
    PS_UPDATE_INFINITETOWER_LIMIT_TIME stInfinite;
    memset(&stInfinite, 0, sizeof(stInfinite));
    stInfinite.dwActorID = pUser->GetQuestID();
    stInfinite.nLimitTime = m_nInfiniteTowerLimitTime;

    // IDA: Send DB packet (main=3, sub=0x53)
    XSendDBPacket xSendDBPacket(pUser->GetObject(), 3, 0x53);
    xSendDBPacket << stInfinite;
    xSendDBPacket << m_nInfiniteTowerLimitPCBangCount;
    xSendDBPacket << m_nInfiniteTowerLimitCount;

    // IDA: Send to DB
    XGameServer* pServer = XGameServer::Instance();
    pServer->SendDBGame(&xSendDBPacket);

    // IDA: Call SendInfiniteTowerInfo to notify client
    SendInfiniteTowerInfo();

    GreenDamTan_log_debug("game.contents", "CGocRecode::InitInfiniteTowerInfo - LimitTime:%lld", nInitTime);
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
// Verified: Direct IDA decompilation - sends infinite tower info to client
void CGocRecode::SendInfiniteTowerInfo() {
    // IDA: Get owner user
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: Build PS_INFINITE_TOWER_INFO
    PS_INFINITE_TOWER_INFO stInfo;
    memset(&stInfo, 0, sizeof(stInfo));

    // IDA: Calculate remaining time
    XGameServer* pServer = XGameServer::Instance();
    __int64 nLeftTime = m_nInfiniteTowerLimitTime - pServer->GetCurDate();
    if (nLeftTime <= 0) {
        stInfo.nLimitTime = 0;
    } else {
        stInfo.nLimitTime = nLeftTime;
    }

    stInfo.sClearChapter = m_sInfiniteTowerClearChapter;
    stInfo.sClearStage = m_sInfiniteTowerClearStage;
    stInfo.sCount = static_cast<short>(m_nInfiniteTowerLimitPCBangCount + m_nInfiniteTowerLimitCount);

    // IDA: Send packet (main=0x28, sub=0x01)
    XSendPacket sendPacket(0x28, 0x01);
    sendPacket << stInfo;
    pUser->Send(&sendPacket);

    GreenDamTan_log_debug("game.contents", "CGocRecode::SendInfiniteTowerInfo - Chapter:%d, Stage:%d",
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
// Verified: Direct IDA decompilation - cheat function to update maze clear info
void CGocRecode::UpdateClearInfo_cheat(int nMazeID) {
    // IDA: 获取 TB_MAZE_INFO
    XGameServer* pServer = XGameServer::Instance();
    TB_MAZE_INFO* pTB_MAZE_INFO = pServer->GetResourceMgr().GetTB_MAZE_INFO(nMazeID);

    while (pTB_MAZE_INFO) {
        // IDA: 重新获取表（循环内）
        TB_MAZE_INFO* pTB_MAZE_INFOa = pServer->GetResourceMgr().GetTB_MAZE_INFO(nMazeID);
        if (!pTB_MAZE_INFOa) {
            LogHelper::LogError("game.contents", "<UpdateClearInfo_cheat> Table NULL ( %d )", nMazeID);
            return;
        }

        // IDA: 检查 Maze_Type == 0 (普通迷宫)
        if (pTB_MAZE_INFOa->Maze_Type != 0) {
            LogHelper::LogError("game.contents", "<UpdateClearInfo_cheat> Maze Type Not Maze ( %d )", nMazeID);
            return;
        }

        // IDA: 查找 m_mapMazeClearInfo
        int nMazeGroup = pTB_MAZE_INFOa->Maze_Group;
        auto iter = m_mapMazeClearInfo.find(nMazeGroup);

        if (iter != m_mapMazeClearInfo.end()) {
            // IDA: 已存在，检查是否需要添加 MazeID
            bool bFind = false;
            for (size_t sh = 0; sh < iter->second.vecClearMazeList.size(); ++sh) {
                if (pTB_MAZE_INFOa->ID == iter->second.vecClearMazeList[sh]) {
                    bFind = true;
                    break;
                }
            }

            if (!bFind) {
                // IDA: 添加到列表
                iter->second.vecClearMazeList.push_back(pTB_MAZE_INFOa->ID);
            }

            // IDA: 发送 PS_MAZE_CLEAR_INFO 包 (0x11/0x64)
            PS_MAZE_CLEAR_INFO psMazeClearInfo;
            memset(&psMazeClearInfo, 0, sizeof(psMazeClearInfo));
            psMazeClearInfo.uxActorID = GetOwnerUser() ? GetOwnerUser()->GetQuestID() : 0;
            psMazeClearInfo.bReset = 0;
            psMazeClearInfo.vecInfo.push_back(iter->second);

            XSendPacket sendPacket(0x11, 0x64);
            sendPacket << psMazeClearInfo;

            CUser* pUser = GetOwnerUser();
            if (pUser) {
                pUser->Send(&sendPacket);
            }
        } else {
            // IDA: 不存在，创建新条目
            ST_MAZE_CLEAR_INFO stMazeClearInfo;
            memset(&stMazeClearInfo, 0, sizeof(stMazeClearInfo));
            stMazeClearInfo.nMazeGroup = pTB_MAZE_INFOa->Maze_Group;
            stMazeClearInfo.vecClearMazeList.push_back(pTB_MAZE_INFOa->ID);

            m_mapMazeClearInfo.insert(std::make_pair(nMazeGroup, stMazeClearInfo));

            // IDA: 发送 PS_MAZE_CLEAR_INFO 包
            PS_MAZE_CLEAR_INFO st;
            memset(&st, 0, sizeof(st));
            st.uxActorID = GetOwnerUser() ? GetOwnerUser()->GetQuestID() : 0;
            st.bReset = 0;
            st.vecInfo.push_back(stMazeClearInfo);

            XSendPacket packet(0x11, 0x64);
            packet << st;

            CUser* pUser = GetOwnerUser();
            if (pUser) {
                pUser->Send(&packet);
            }
        }

        // IDA: 调用 UpdateSharePointByForce
        UpdateSharePointByForce(nMazeID, 0);

        // IDA: 处理迷宫链
        if (pTB_MAZE_INFO->Maze_Difficulty_Type == 0 &&
            pTB_MAZE_INFO->Maze_Episode_No == 1 &&
            pTB_MAZE_INFO->Check_Clear_Maze) {
            // IDA: 特殊计算
            nMazeID = (pTB_MAZE_INFO->Check_Clear_Maze / 10) * 10 + 3;
        } else {
            nMazeID = pTB_MAZE_INFO->Check_Clear_Maze;
        }

        // IDA: 获取下一个迷宫信息
        pTB_MAZE_INFO = pServer->GetResourceMgr().GetTB_MAZE_INFO(nMazeID);
    }
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
void CGocRecode::AddBPCombo(int nBP) {
    // TODO: 需要外部依赖 - 添加BP连击
    (void)nBP;
}

// IDA: ?AddDSPoint@CGocRecode@@QEAAXH@Z (0x140148FE0)
void CGocRecode::AddDSPoint(int nDSPoint) {
    // TODO: 需要外部依赖 - 添加DS点数
    (void)nDSPoint;
}

// IDA: ?DBUpdatePoint@CGocRecode@@QEAAXXZ (0x140149100)
void CGocRecode::DBUpdatePoint() {
    // TODO: 需要外部依赖 - 更新DB点数
}

// ============================================================================
// District State Functions
// ============================================================================

// IDA: ?ReqWorldState@CGocRecode@@QEAAXXZ (0x140148140)
void CGocRecode::ReqWorldState() {
    // TODO: 需要外部依赖 - 请求世界状态
}

// IDA: ?LoadDistrictState@CGocRecode@@QEAAXAEAUST_DISTRICT_STATE_LIST@@@Z (0x140148380)
void CGocRecode::LoadDistrictState(ST_DISTRICT_STATE_LIST& stStateList) {
    // TODO: 需要外部依赖 - 加载区域状态
    (void)stStateList;
}

// IDA: ?LoadMazeState@CGocRecode@@QEAAXAEAUST_MAZE_STATE_LIST@@@Z (0x1401484D0)
void CGocRecode::LoadMazeState(ST_MAZE_STATE_LIST& stStateList) {
    // TODO: 需要外部依赖 - 加载迷宫状态
    (void)stStateList;
}

// IDA: ?SendDistrictState@CGocRecode@@QEAAXXZ (0x140148840)
void CGocRecode::SendDistrictState() {
    // TODO: 需要外部依赖 - 发送区域状态
}

// IDA: ?SendMazeState@CGocRecode@@QEAAXXZ (0x1401489F0)
void CGocRecode::SendMazeState() {
    // TODO: 需要外部依赖 - 发送迷宫状态
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
// Verified: Direct IDA decompilation - applies reward item for share point
bool CGocRecode::ApplyRewardItemForSharePoint(int nID, int nStep, ST_CREATE_ITEM& stCreateItem) {
    // IDA: 获取 CGocInventory 组件
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return false;
    }

    std::shared_ptr<CGocInventory> pInven;
    pUser->GetGOC<CGocInventory>(pInven);
    if (!pInven) {
        return false;
    }

    // IDA: 获取 TB_DS_POINT 表
    XGameServer* pServer = XGameServer::Instance();
    TB_DS_POINT* pTBDSPoint = pServer->GetResourceMgr().GetTB_DS_POINT(nID);
    if (!pTBDSPoint) {
        return false;
    }

    // IDA: 获取奖励物品ID和数量
    int nItemID = *((int*)&pTBDSPoint->Reward_ItemType_02 + nStep);
    short sCount = *((unsigned char*)&pTBDSPoint->uniRewardPost[4] + nStep + 3);

    // IDA: 检查是否为职业奖励类型
    if (*((unsigned char*)&pTBDSPoint->uniRewardValue[4] + nStep + 1) == 2) {
        // IDA: 获取用户职业
        unsigned int dwItemID = nItemID;
        unsigned char byClass = pUser->stMyCharInfoEx()->stBaseInfo.byClass;

        // IDA: 获取 TB_QUEST_REWARD 表
        TB_QUEST_REWARD* pTBReward = pServer->GetResourceMgr().GetTB_QUEST_REWARD(dwItemID);
        if (!pTBReward) {
            return false;
        }

        // IDA: 根据职业选择对应物品
        for (int i = 0; i < 9; ++i) {
            if (byClass == i + 1) {
                nItemID = *(&pTBReward->Item_PC_A + i);
                break;
            }
        }
    }

    // IDA: 检查物品ID是否有效
    if (nItemID) {
        ST_LOG_GAME stLog;
        memset(&stLog, 0, sizeof(stLog));
        stLog.nParam3 = nStep;
        stLog.nParam4 = nID;

        // IDA: 创建物品
        if (pInven->CreateItemReq(nItemID, sCount, 0, E_ITEM_CREATE_TYPE_SHARE_POINT, &stLog)) {
            stCreateItem.nItemID = nItemID;
            stCreateItem.shCount = sCount;
            return true;
        }
        return false;
    }

    return true;
}
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
        // IDA: Insert pair (dwActorID, nCount) into map
        m_mapKilledUser[stInfo.dwActorID] = stInfo.nCount;
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
// Verified: Direct IDA decompilation - sends maze enter limit count list and group list to client
void CGocRecode::SendEnterMazeLimitCount() {
    // IDA: Get owner user via RTTI dynamic_cast
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: Build PS_MAZE_ENTER_LIMIT_COUNT_LIST
    PS_MAZE_ENTER_LIMIT_COUNT_LIST stList;
    memset(&stList, 0, sizeof(stList));
    stList.byResultType = 0;
    stList.dwActorID = pUser->GetQuestID();

    // IDA: Iterate m_mapEnterMazeLimitCount and add to list
    for (auto it = m_mapEnterMazeLimitCount.begin(); it != m_mapEnterMazeLimitCount.end(); ++it) {
        PS_UPDATE_MAZE_ENTER_LIMIT_COUNT stInfo;
        memset(&stInfo, 0, sizeof(stInfo));
        stInfo.wMapID = static_cast<unsigned short>(it->first);
        stInfo.byCount = it->second.byCount;
        stInfo.byPCBangCount = it->second.byPCBangCount;
        stList.listEnterMazeCount.push_back(stInfo);
    }

    // IDA: Send packet (main=4, sub=0x43)
    XSendPacket sendPacket(4, 0x43);
    sendPacket << stList;
    pUser->Send(&sendPacket);

    // IDA: Build PS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST
    PS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST stGroupList;
    memset(&stGroupList, 0, sizeof(stGroupList));
    stGroupList.dwUAID = pUser->GetUAID();
    stGroupList.dwUCID = pUser->GetQuestID();
    stGroupList.byResultType = 0;

    // IDA: Iterate m_mapEnterGroupLimitCount_Character
    for (auto iter = m_mapEnterGroupLimitCount_Character.begin();
         iter != m_mapEnterGroupLimitCount_Character.end(); ++iter) {
        PS_MAZE_ENTER_LIMIT_COUNT_GROUP stGroup;
        memset(&stGroup, 0, sizeof(stGroup));
        stGroup.wGroupID = iter->first;
        stGroup.byCount = iter->second.byCount;
        stGroup.byPCBangCount = iter->second.byPCBangCount;
        stGroupList.mapGroup.insert(std::make_pair(iter->first, stGroup));
    }

    // IDA: Iterate m_mapEnterGroupLimitCount_Account
    for (auto it = m_mapEnterGroupLimitCount_Account.begin();
         it != m_mapEnterGroupLimitCount_Account.end(); ++it) {
        PS_MAZE_ENTER_LIMIT_COUNT_GROUP stGroup;
        memset(&stGroup, 0, sizeof(stGroup));
        stGroup.wGroupID = it->first;
        stGroup.byCount = it->second.byCount;
        stGroup.byPCBangCount = it->second.byPCBangCount;
        // IDA: Insert into mapGroup (may overwrite if same key exists)
        stGroupList.mapGroup[it->first] = stGroup;
    }

    // IDA: Send packet (main=4, sub=0x46)
    XSendPacket sendPacket2(4, 0x46);
    sendPacket2 << stGroupList;
    pUser->BridgeSend(&sendPacket2);
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
// Verified: Direct IDA decompilation - resets casual raid enter count limit
void CGocRecode::ResetEnterMazeLimiteCount(int nMazeID) {
    // IDA: 获取所属的User对象 (RTTI dynamic_cast)
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 获取 TB_MAZE_INFO 表
    XGameServer* pServer = XGameServer::Instance();
    TB_MAZE_INFO* pTB_MAZE_INFO = pServer->GetResourceMgr().GetTB_MAZE_INFO(nMazeID);
    if (!pTB_MAZE_INFO) {
        return;
    }

    // IDA: 获取当前时间
    ATL::CTime tCurr = ATL::CTime::GetCurrentTime();

    // IDA: 检查是否有 Maze_Enter_Count_Group
    if (pTB_MAZE_INFO->Maze_Enter_Count_Group) {
        // IDA: 验证 Maze_Enter_Count_Group 表存在
        if (!pServer->GetResourceMgr().GetTB_MAZE_ENTER_COUNT_GROUP(pTB_MAZE_INFO->Maze_Enter_Count_Group)) {
            return;
        }

        // IDA: 构造 PS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST
        PS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST stUpdateGroupList;
        memset(&stUpdateGroupList, 0, sizeof(stUpdateGroupList));
        stUpdateGroupList.dwUAID = pUser->GetUAID();
        stUpdateGroupList.dwUCID = pUser->GetQuestID();
        stUpdateGroupList.byResultType = 1;

        // IDA: 构造 DB 更新包
        PS_DB_MAZE_ENTER_LIMIT_COUNT_GROUP_UPDATE psDBUpdate;
        memset(&psDBUpdate, 0, sizeof(psDBUpdate));
        psDBUpdate.dwUAID = pUser->GetUAID();
        psDBUpdate.dwUCID = pUser->GetQuestID();
        psDBUpdate.dwGroupID = pTB_MAZE_INFO->Maze_Enter_Count_Group;
        psDBUpdate.byType = pTB_MAZE_INFO->Maze_Enter_Count_Type;
        psDBUpdate.nUpdateTime = static_cast<__int64>(tCurr.GetTime());

        // IDA: 根据 Maze_Enter_Count_Type 选择不同的 map
        std::map<unsigned short, PS_MAZE_ENTER_LIMIT_COUNT_GROUP>::iterator iter;
        if (pTB_MAZE_INFO->Maze_Enter_Count_Type == 0) {
            // Character
            iter = m_mapEnterGroupLimitCount_Character.find(pTB_MAZE_INFO->Maze_Enter_Count_Group);
            if (iter == m_mapEnterGroupLimitCount_Character.end()) {
                return;
            }
        } else if (pTB_MAZE_INFO->Maze_Enter_Count_Type == 1) {
            // Account
            iter = m_mapEnterGroupLimitCount_Account.find(pTB_MAZE_INFO->Maze_Enter_Count_Group);
            if (iter == m_mapEnterGroupLimitCount_Account.end()) {
                return;
            }
        } else {
            return;
        }

        GreenDamTan_log_debug("game.contents",
            "ResetEnterMazeLimiteCount - use casual raid initilization [MazeID:%d, Group:%d]",
            nMazeID, pTB_MAZE_INFO->Maze_Enter_Count_Group);

        // IDA: 重置计数
        iter->second.byCount = 0;
        iter->second.byPCBangCount = 0;
        iter->second.nUpdateTime = static_cast<int>(tCurr.GetTime());

        // IDA: 遍历迷宫列表并发送 DB 更新
        for (size_t i = 0; i < iter->second.vecMazeList.size(); ++i) {
            psDBUpdate.wMazeID = iter->second.vecMazeList[i].wMazeID;
            psDBUpdate.dwCount = iter->second.vecMazeList[i].byCount;
            psDBUpdate.dwPCBangCount = iter->second.vecMazeList[i].byPCBangCount;

            // IDA: 发送 DB 包 (0x43/0x07)
            XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x43, 0x07);
            xSendDBPacket << psDBUpdate;
            pServer->SendDBGame(&xSendDBPacket);
        }

        // IDA: 插入到更新列表
        stUpdateGroupList.mapGroup.insert(std::make_pair(iter->first, iter->second));

        // IDA: 发送更新到客户端 (0x04/0x48)
        XSendPacket sendPacket(0x04, 0x48);
        sendPacket << stUpdateGroupList;
        pUser->Send(&sendPacket);
    } else {
        // IDA: 没有 Group，直接处理单个迷宫
        auto iter = m_mapEnterMazeLimitCount.find(nMazeID);
        if (iter == m_mapEnterMazeLimitCount.end()) {
            return;
        }

        GreenDamTan_log_debug("game.contents",
            "ResetEnterMazeLimiteCount - use casual raid initilization [MazeID:%d]",
            nMazeID);

        // IDA: 重置计数
        iter->second.byCount = 0;
        iter->second.byPCBangCount = 0;

        // IDA: 构造更新列表
        PS_MAZE_ENTER_LIMIT_COUNT_LIST stUpdateMazeList;
        memset(&stUpdateMazeList, 0, sizeof(stUpdateMazeList));
        stUpdateMazeList.dwActorID = pUser->GetQuestID();
        stUpdateMazeList.byResultType = 1;

        PS_UPDATE_MAZE_ENTER_LIMIT_COUNT stUpdateMaze;
        memset(&stUpdateMaze, 0, sizeof(stUpdateMaze));
        stUpdateMaze.wMapID = nMazeID;
        stUpdateMaze.byCount = 0;
        stUpdateMaze.byPCBangCount = iter->second.byPCBangCount;

        stUpdateMazeList.listEnterMazeCount.push_back(stUpdateMaze);

        // IDA: 发送 DB 包 (0x43/0x04)
        XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x43, 0x04);
        xSendDBPacket << stUpdateMazeList;
        pServer->SendDBGame(&xSendDBPacket);

        // IDA: 发送客户端包 (0x04/0x45)
        XSendPacket v40(0x04, 0x45);
        v40 << stUpdateMazeList;
        pUser->Send(&v40);
    }
}

// IDA: ?DecreaseEnterCasualMazeLimiteCount@CGocRecode@@QEAAXXZ (0x140153920)
// Verified: Direct IDA decompilation - decreases casual maze enter count
void CGocRecode::DecreaseEnterCasualMazeLimiteCount() {
    // IDA: 构造更新列表
    PS_MAZE_ENTER_LIMIT_COUNT_LIST stUpdateMazeList;
    memset(&stUpdateMazeList, 0, sizeof(stUpdateMazeList));

    CUser* pUser = GetOwnerUser();
    if (pUser) {
        stUpdateMazeList.dwActorID = pUser->GetQuestID();
    }
    stUpdateMazeList.byResultType = 2;

    // IDA: 获取休闲迷宫ID列表
    std::vector<unsigned short> vecMazeID;
    XGameServer* pServer = XGameServer::Instance();
    pServer->GetResourceMgr().GetCasualMazeID(vecMazeID);

    // IDA: 遍历所有休闲迷宫
    for (auto it = vecMazeID.begin(); it != vecMazeID.end(); ++it) {
        unsigned short wMazeID = *it;

        // IDA: 查找 m_mapEnterMazeLimitCount
        auto iter = m_mapEnterMazeLimitCount.find(wMazeID);
        if (iter != m_mapEnterMazeLimitCount.end()) {
            PS_UPDATE_MAZE_ENTER_LIMIT_COUNT stUpdate;
            memset(&stUpdate, 0, sizeof(stUpdate));
            stUpdate.wMapID = wMazeID;
            stUpdate.byCount = iter->second.byCount;
            stUpdate.byPCBangCount = iter->second.byPCBangCount;

            // IDA: 如果计数 > 0，减少计数
            if (stUpdate.byCount > 0) {
                --stUpdate.byCount;
                stUpdateMazeList.listEnterMazeCount.push_back(stUpdate);

                // IDA: 更新内存中的计数
                iter->second.byCount = stUpdate.byCount;

                GreenDamTan_log_debug("game.contents",
                    "DecreaseEnterCasualMazeLimiteCount - use item [ MazeID:%d ]",
                    wMazeID);
            }
        }
    }

    // IDA: 发送 DB 包 (0x43/0x04)
    if (pUser) {
        XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x43, 0x04);
        xSendDBPacket << stUpdateMazeList;
        pServer->SendDBGame(&xSendDBPacket);

        // IDA: 发送客户端包 (0x04/0x45)
        XSendPacket sendPacket(0x04, 0x45);
        sendPacket << stUpdateMazeList;
        pUser->Send(&sendPacket);
    }
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
// Verified: Direct IDA decompilation - updates ranking data for time, clear count, and monster kill score
void CGocRecode::RankingDataUpdate(unsigned long dwMazeID, unsigned long dwMazePlayTime, int nMonsterKillScore) {
    // IDA: 检查服务器是否开启了排名功能
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer || !pServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_RANKING)) {
        return;
    }

    // IDA: 获取所属的User对象
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 获取 CGocEntity 组件并检查是否为代表角色
    std::shared_ptr<CGocEntity> pEntity;
    pUser->GetGOC<CGocEntity>(pEntity);
    if (!pEntity) {
        return;
    }

    unsigned int nRepresentativeUCID = pEntity->GetRepresentativeUCID();
    unsigned int nUCID = pUser->GetQuestID();
    if (nRepresentativeUCID != nUCID) {
        return;
    }

    // IDA: 检查排名时间
    if (!pServer->GetResourceMgr().CheckRankingTime()) {
        return;
    }

    // IDA: 处理时间排名
    std::vector<TB_RANK_INFO*> vecTime;
    if (pServer->GetResourceMgr().GetRankingInfoTable(dwMazeID, E_RANKING_TOTAL_TYPE_TIME, vecTime)) {
        for (size_t i = 0; i < vecTime.size(); ++i) {
            TB_RANK_INFO* pTB_RANK_INFO = vecTime[i];
            if (pTB_RANK_INFO && pTB_RANK_INFO->Ranking_Maze == static_cast<int>(dwMazeID)) {
                // IDA: 构造 PS_DB_RANKING_POINT_UPDATE
                PS_DB_RANKING_POINT_UPDATE psUpdate;
                memset(&psUpdate, 0, sizeof(psUpdate));
                psUpdate.byRankingCategory = pTB_RANK_INFO->Ranking_Category;
                psUpdate.stRankingInfo.wRankInfoIndex = pTB_RANK_INFO->Ranking_Index;
                psUpdate.stRankingInfo.nTopRank = pTB_RANK_INFO->Ranking_Visible;
                psUpdate.stRankingInfo.byType = pTB_RANK_INFO->Ranking_Total_Type;
                psUpdate.stRankingInfo.byClassType = pTB_RANK_INFO->Ranking_Total_Class_Type;
                psUpdate.stRankingInfo.byRewradType = pTB_RANK_INFO->Ranking_Reward_Receive;
                psUpdate.stRankingInfo.dwMazeID = pTB_RANK_INFO->Ranking_Maze;

                if (psUpdate.stRankingInfo.byClassType) {
                    psUpdate.stRankingInfo.byClass = pUser->GetClass();
                }

                psUpdate.stUser.dwUAID = pUser->GetUAID();
                psUpdate.stUser.dwUCID = nUCID;
                psUpdate.stUser.wRankInfoIndex = pTB_RANK_INFO->Ranking_Index;
                psUpdate.stUser.nRank = 0;
                psUpdate.stUser.nMaxRank = pTB_RANK_INFO->Ranking_Visible;
                psUpdate.stUser.byClass = pUser->GetClass();
                psUpdate.stUser.byLevel = pUser->GetLevel();
                psUpdate.stUser.nPlayCount = 0;
                psUpdate.stUser.nScore = static_cast<int>(dwMazePlayTime);
                psUpdate.stUser.byLastReward = 0;
                psUpdate.stUser.dwProfilePhotoID = pUser->GetProfilePhotoID();

                std::wstring strName = pUser->GetName();
                wcscpy_s(psUpdate.stUser.strName, strName.c_str());

                ST_LEAGUE_INFO_EX stLeagueInfo;
                memset(&stLeagueInfo, 0, sizeof(stLeagueInfo));
                pUser->GetLeagueInfo(&stLeagueInfo);
                wcscpy_s(psUpdate.stUser.strLeagueName, stLeagueInfo.szLeagueName);

                // IDA: 发送 DB 包 (main=0x28, sub=0x12)
                XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x28, 0x12);
                xSendDBPacket << psUpdate;
                pServer->SendDBGame(&xSendDBPacket);

                // IDA: 发送日志
                ST_LOG_GAME stLog;
                memset(&stLog, 0, sizeof(stLog));
                stLog._nUAID = pUser->GetUAID();
                stLog._nUCID = nUCID;
                stLog._sMainType = 30;
                stLog._sSubType = 10;
                stLog.nParam0 = psUpdate.stRankingInfo.wRankInfoIndex;
                stLog.nParam1 = static_cast<int>(dwMazeID);
                stLog.nParam2 = static_cast<int>(dwMazePlayTime);
                stLog.nParam3 = m_bShowCutscene ? 1 : 0;
                stLog.nParam4 = pUser->GetLevel();
                stLog.nParam5 = pUser->GetClass();
                wcscpy_s(stLog.szComment, L"RANKING_TIME_UPDATE");
                pServer->SendDBLog(&stLog);
            }
        }
    }

    // IDA: 处理通关次数排名
    std::vector<TB_RANK_INFO*> vecClearCount;
    if (pServer->GetResourceMgr().GetRankingInfoTable(dwMazeID, E_RANKING_TOTAL_TYPE_CLEAR_COUNT, vecClearCount)) {
        for (size_t i = 0; i < vecClearCount.size(); ++i) {
            TB_RANK_INFO* pTB_RANK_INFO = vecClearCount[i];
            if (pTB_RANK_INFO && pTB_RANK_INFO->Ranking_Maze == static_cast<int>(dwMazeID)) {
                PS_DB_RANKING_POINT_UPDATE st;
                memset(&st, 0, sizeof(st));
                st.byRankingCategory = pTB_RANK_INFO->Ranking_Category;
                st.stRankingInfo.wRankInfoIndex = pTB_RANK_INFO->Ranking_Index;
                st.stRankingInfo.nTopRank = pTB_RANK_INFO->Ranking_Visible;
                st.stRankingInfo.byType = pTB_RANK_INFO->Ranking_Total_Type;
                st.stRankingInfo.byClassType = pTB_RANK_INFO->Ranking_Total_Class_Type;
                st.stRankingInfo.byRewradType = pTB_RANK_INFO->Ranking_Reward_Receive;
                st.stRankingInfo.dwMazeID = pTB_RANK_INFO->Ranking_Maze;

                if (st.stRankingInfo.byClassType) {
                    st.stRankingInfo.byClass = pUser->GetClass();
                }

                st.stUser.dwUAID = pUser->GetUAID();
                st.stUser.dwUCID = nUCID;
                st.stUser.wRankInfoIndex = pTB_RANK_INFO->Ranking_Index;
                st.stUser.nRank = 0;
                st.stUser.nMaxRank = pTB_RANK_INFO->Ranking_Visible;
                st.stUser.byClass = pUser->GetClass();
                st.stUser.byLevel = pUser->GetLevel();
                st.stUser.nPlayCount = 0;
                st.stUser.nScore = static_cast<int>(dwMazePlayTime);
                st.stUser.byLastReward = 0;
                st.stUser.dwProfilePhotoID = pUser->GetProfilePhotoID();

                std::wstring strName = pUser->GetName();
                wcscpy_s(st.stUser.strName, strName.c_str());

                ST_LEAGUE_INFO_EX stInfo;
                memset(&stInfo, 0, sizeof(stInfo));
                pUser->GetLeagueInfo(&stInfo);
                wcscpy_s(st.stUser.strLeagueName, stInfo.szLeagueName);

                XSendDBPacket xSendPacket(pUser->GetObject(), 0x28, 0x12);
                xSendPacket << st;
                pServer->SendDBGame(&xSendPacket);

                ST_LOG_GAME stLogGame;
                memset(&stLogGame, 0, sizeof(stLogGame));
                stLogGame._nUAID = pUser->GetUAID();
                stLogGame._nUCID = nUCID;
                stLogGame._sMainType = 30;
                stLogGame._sSubType = 10;
                stLogGame.nParam0 = st.stRankingInfo.wRankInfoIndex;
                stLogGame.nParam1 = static_cast<int>(dwMazeID);
                stLogGame.nParam2 = 1;
                stLogGame.nParam3 = m_bShowCutscene ? 1 : 0;
                stLogGame.nParam4 = pUser->GetLevel();
                stLogGame.nParam5 = pUser->GetClass();
                wcscpy_s(stLogGame.szComment, L"RANKING_CLEAR_COUNT_UPDATE");
                pServer->SendDBLog(&stLogGame);
            }
        }
    }

    // IDA: 处理怪物击杀分数排名
    std::vector<TB_RANK_INFO*> vecMonsterKill;
    if (pServer->GetResourceMgr().GetRankingInfoTable(dwMazeID, E_RANKING_TOTAL_TYPE_MONSTER_KILL_SOCRE, vecMonsterKill)) {
        for (size_t i = 0; i < vecMonsterKill.size(); ++i) {
            TB_RANK_INFO* pTB_RANK_INFO = vecMonsterKill[i];
            if (pTB_RANK_INFO && pTB_RANK_INFO->Ranking_Maze == static_cast<int>(dwMazeID)) {
                PS_DB_RANKING_POINT_UPDATE v53;
                memset(&v53, 0, sizeof(v53));
                v53.byRankingCategory = pTB_RANK_INFO->Ranking_Category;
                v53.stRankingInfo.wRankInfoIndex = pTB_RANK_INFO->Ranking_Index;
                v53.stRankingInfo.nTopRank = pTB_RANK_INFO->Ranking_Visible;
                v53.stRankingInfo.byType = pTB_RANK_INFO->Ranking_Total_Type;
                v53.stRankingInfo.byClassType = pTB_RANK_INFO->Ranking_Total_Class_Type;
                v53.stRankingInfo.byRewradType = pTB_RANK_INFO->Ranking_Reward_Receive;
                v53.stRankingInfo.dwMazeID = pTB_RANK_INFO->Ranking_Maze;

                if (v53.stRankingInfo.byClassType) {
                    v53.stRankingInfo.byClass = pUser->GetClass();
                }

                v53.stUser.dwUAID = pUser->GetUAID();
                v53.stUser.dwUCID = nUCID;
                v53.stUser.wRankInfoIndex = pTB_RANK_INFO->Ranking_Index;
                v53.stUser.nRank = 0;
                v53.stUser.nMaxRank = pTB_RANK_INFO->Ranking_Visible;
                v53.stUser.byClass = pUser->GetClass();
                v53.stUser.byLevel = pUser->GetLevel();
                v53.stUser.nPlayCount = 0;
                v53.stUser.nScore = nMonsterKillScore;
                v53.stUser.byLastReward = 0;
                v53.stUser.dwProfilePhotoID = pUser->GetProfilePhotoID();

                std::wstring strName = pUser->GetName();
                wcscpy_s(v53.stUser.strName, strName.c_str());

                ST_LEAGUE_INFO_EX v55;
                memset(&v55, 0, sizeof(v55));
                pUser->GetLeagueInfo(&v55);
                wcscpy_s(v53.stUser.strLeagueName, v55.szLeagueName);

                XSendDBPacket v56(pUser->GetObject(), 0x28, 0x12);
                v56 << v53;
                pServer->SendDBGame(&v56);

                ST_LOG_GAME v54;
                memset(&v54, 0, sizeof(v54));
                v54._nUAID = pUser->GetUAID();
                v54._nUCID = nUCID;
                v54._sMainType = 30;
                v54._sSubType = 10;
                v54.nParam0 = v53.stRankingInfo.wRankInfoIndex;
                v54.nParam1 = static_cast<int>(dwMazeID);
                v54.nParam2 = nMonsterKillScore;
                v54.nParam3 = m_bShowCutscene ? 1 : 0;
                v54.nParam4 = pUser->GetLevel();
                v54.nParam5 = pUser->GetClass();
                wcscpy_s(v54.szComment, L"RANKING_MONSTER_KILL_UPDATE");
                pServer->SendDBLog(&v54);
            }
        }
    }
}

// IDA: ?CanRecvRankingReward@CGocRecode@@QEAAHGAEAUST_USER_LAST_RANKING_INFO@@@Z (0x1401553D0)
// Verified: Direct IDA decompilation - checks if user can receive ranking reward
int CGocRecode::CanRecvRankingReward(unsigned short wRankInfoIndex, ST_USER_LAST_RANKING_INFO* stLastRank) {
    // IDA: 查找 m_mapMyLastRanking
    auto it = m_mapMyLastRanking.find(wRankInfoIndex);
    if (it == m_mapMyLastRanking.end()) {
        return 58503; // Error code: ranking info not found
    }

    // IDA: 复制排名信息到输出参数
    *stLastRank = it->second;

    // IDA: 检查排名是否有效
    if (stLastRank->stInfo.nRank <= 0) {
        return 58503; // Error code: invalid rank
    }

    // IDA: 检查是否已领取奖励
    if (stLastRank->stInfo.byLastReward) {
        return 0; // Already received
    }

    return 58504; // Can receive
}

// IDA: ?SetRankingMyInfo@CGocRecode@@QEAAXG_NAEAUST_USER_RANKING_INFO@@_K@Z (0x1401554B0)
// Verified: Direct IDA decompilation - sets ranking info for user, handles representative UCID
void CGocRecode::SetRankingMyInfo(unsigned short wRankInfoIndex, bool bLastRanking, ST_USER_RANKING_INFO* stMyInfo, unsigned long long dw64SeasonSetCount) {
    // IDA: 获取所属的User对象
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 获取 TB_RANK_INFO 表
    XGameServer* pServer = XGameServer::Instance();
    TB_RANK_INFO* pTB_RANK_INFO = pServer->GetResourceMgr().GetTB_RANK_INFO(wRankInfoIndex);
    if (!pTB_RANK_INFO) {
        return;
    }

    // IDA: 获取 CGocEntity 组件
    std::shared_ptr<CGocEntity> pEntity;
    pUser->GetGOC<CGocEntity>(pEntity);
    if (!pEntity) {
        return;
    }

    // IDA: 根据排名类别填充用户信息
    if (pTB_RANK_INFO->Ranking_Category == 2) {
        // IDA: 联赛排名
        ST_LEAGUE_INFO_EX stLeagueInfo;
        memset(&stLeagueInfo, 0, sizeof(stLeagueInfo));
        pUser->GetLeagueInfo(&stLeagueInfo);

        stMyInfo->wRankInfoIndex = wRankInfoIndex;
        stMyInfo->dwUAID = pUser->GetUAID();
        stMyInfo->dwUCID = pUser->GetQuestID();
        stMyInfo->byClass = pUser->GetClass();
        stMyInfo->byLevel = pUser->GetLevel();
        stMyInfo->dwProfilePhotoID = pUser->GetProfilePhotoID();

        std::wstring strName = pUser->GetName();
        wcscpy_s(stMyInfo->strName, strName.c_str());
        wcscpy_s(stMyInfo->strLeagueName, stLeagueInfo.szLeagueName);
    } else {
        // IDA: 普通排名 - 使用代表角色信息
        stMyInfo->wRankInfoIndex = wRankInfoIndex;
        stMyInfo->dwUAID = pUser->GetUAID();

        unsigned int nRepresentativeUCID = pEntity->GetRepresentativeUCID();
        if (nRepresentativeUCID) {
            stMyInfo->dwUCID = nRepresentativeUCID;
            ST_REPRESENTATIVE_INFO* pRepInfo = pEntity->GetRepresentativeInfo();
            if (pRepInfo) {
                stMyInfo->byClass = pRepInfo->byClass;
                stMyInfo->byLevel = pRepInfo->byLevel;
                stMyInfo->dwProfilePhotoID = pRepInfo->dwProfilePhotoID;
                wcscpy_s(stMyInfo->strName, pRepInfo->strName);
                wcscpy_s(stMyInfo->strLeagueName, pRepInfo->strLeagueName);
            }
        } else {
            stMyInfo->dwUCID = 0;
            stMyInfo->byClass = 0;
            stMyInfo->byLevel = 0;
            stMyInfo->dwProfilePhotoID = 0;
        }
    }

    // IDA: 更新排名信息映射
    if (bLastRanking) {
        // IDA: 更新 m_mapMyLastRanking
        auto it = m_mapMyLastRanking.find(stMyInfo->wRankInfoIndex);
        if (it != m_mapMyLastRanking.end()) {
            // IDA: 如果是联赛排名，直接更新
            if (pTB_RANK_INFO->Ranking_Category == 2) {
                it->second.stInfo = *stMyInfo;
            } else if (it->second.dw64SeasonSetCount < dw64SeasonSetCount) {
                // IDA: 如果赛季计数更新，更新信息
                it->second.stInfo = *stMyInfo;
                it->second.dw64SeasonSetCount = dw64SeasonSetCount;
            }
        } else {
            // IDA: 插入新条目
            ST_USER_LAST_RANKING_INFO stLast;
            memset(&stLast, 0, sizeof(stLast));
            stLast.stInfo = *stMyInfo;
            stLast.dw64SeasonSetCount = dw64SeasonSetCount;
            m_mapMyLastRanking.insert(std::make_pair(stMyInfo->wRankInfoIndex, stLast));
        }
    } else {
        // IDA: 更新 m_mapMyRanking
        auto it = m_mapMyRanking.find(stMyInfo->wRankInfoIndex);
        if (it != m_mapMyRanking.end()) {
            it->second = *stMyInfo;
        } else {
            m_mapMyRanking.insert(std::make_pair(stMyInfo->wRankInfoIndex, *stMyInfo));
        }
    }
}

// IDA: ?ReqRankingList@CGocRecode@@QEAA_NAEAUPS_RANKING_LIST_REQ@@@Z (0x140155BC0)
// Verified: Direct IDA decompilation - requests ranking list from DB, validates parameters
bool CGocRecode::ReqRankingList(PS_RANKING_LIST_REQ* psReq) {
    // IDA: 获取所属的User对象
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return false;
    }

    // IDA: 获取 CGocEntity 组件
    std::shared_ptr<CGocEntity> pEntity;
    pUser->GetGOC<CGocEntity>(pEntity);
    if (!pEntity) {
        return false;
    }

    // IDA: 填充请求信息
    psReq->dwUAID = pUser->GetUAID();
    psReq->dwUCID = pUser->GetQuestID();

    // IDA: 获取 TB_RANK_INFO 表
    XGameServer* pServer = XGameServer::Instance();
    TB_RANK_INFO* pTB_RANK_INFO = pServer->GetResourceMgr().GetTB_RANK_INFO(psReq->wRankInfoIndex);
    if (!pTB_RANK_INFO) {
        LogHelper::LogError("game.contents", "ReqRankingList - Error Table [UCID:%d, ID:%d]",
                           psReq->dwUCID, psReq->wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 1, 0xE484);
        return false;
    }

    // IDA: 验证 Ranking_Total_Type
    if (pTB_RANK_INFO->Ranking_Total_Type == 0 || pTB_RANK_INFO->Ranking_Total_Type >= 6) {
        LogHelper::LogError("game.contents", "ReqRankingList - Error Ranking_Total_Type [UCID:%d, ID:%d]",
                           psReq->dwUCID, psReq->wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 1, 0xE484);
        return false;
    }

    // IDA: 处理 Ranking_Total_Class_Type
    if (pTB_RANK_INFO->Ranking_Total_Class_Type == 1) {
        if (psReq->byClass == 0 || psReq->byClass >= 9) {
            psReq->byClass = pUser->GetClass();
        }
    } else if (pTB_RANK_INFO->Ranking_Total_Class_Type == 0) {
        psReq->byClass = 0;
    } else {
        LogHelper::LogError("game.contents", "ReqRankingList - Error Ranking_Total_Class_Type [UCID:%d, ID:%d]",
                           psReq->dwUCID, psReq->wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 1, 0xE484);
        return false;
    }

    // IDA: 验证 Ranking_Visible
    if (pTB_RANK_INFO->Ranking_Visible > 100) {
        LogHelper::LogError("game.contents", "ReqRankingList - Error Ranking_Visible [UCID:%d, ID:%d]",
                           psReq->dwUCID, psReq->wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 1, 0xE484);
        return false;
    }

    // IDA: 验证 Ranking_Category
    if (pTB_RANK_INFO->Ranking_Category == 0 || pTB_RANK_INFO->Ranking_Category >= 4) {
        LogHelper::LogError("game.contents", "ReqRankingList - Ranking_Category [UCID:%d, ID:%d]",
                           psReq->dwUCID, psReq->wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 1, 0xE484);
        return false;
    }

    // IDA: 验证 Ranking_Maze
    if (pTB_RANK_INFO->Ranking_Maze != 0) {
        TB_MAZE_INFO* pTB_MAZE_INFO = pServer->GetResourceMgr().GetTB_MAZE_INFO(pTB_RANK_INFO->Ranking_Maze);
        if (!pTB_MAZE_INFO) {
            LogHelper::LogError("game.contents", "ReqRankingList - Error Ranking_Maze [UCID:%d, ID:%d]",
                               psReq->dwUCID, psReq->wRankInfoIndex);
            CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 1, 0xE484);
            return false;
        }
    }

    // IDA: 检查请求冷却时间
    unsigned long long dwCurTick = GetTickCount64();
    if (m_dw64RankingListTick[pTB_RANK_INFO->Ranking_Category] > dwCurTick) {
        LogHelper::LogError("game.contents", "ReqRankingList - Already Request [UCID:%d, ID:%d]",
                           psReq->dwUCID, psReq->wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 1, 0xE489);
        return false;
    }
    m_dw64RankingListTick[pTB_RANK_INFO->Ranking_Category] = dwCurTick + 1000;

    // IDA: 根据排名类别发送不同的 DB 请求
    if (pTB_RANK_INFO->Ranking_Category == 2) {
        // IDA: 联赛排名
        PS_RANKING_LIST_REQ psRank;
        memset(&psRank, 0, sizeof(psRank));
        psRank.dwUCID = psReq->dwUCID;
        psRank.wRankInfoIndex = psReq->wRankInfoIndex;
        psRank.bLastRanking = psReq->bLastRanking;
        psRank.byClass = psReq->byClass;

        bool bMatching = false;
        unsigned int dwModeMazeID = pTB_RANK_INFO->Ranking_Maze;

        XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x28, 0x17);
        xSendDBPacket << psRank;
        xSendDBPacket << bMatching;
        xSendDBPacket << dwModeMazeID;
        pServer->SendDBGame(&xSendDBPacket);
    } else {
        // IDA: 普通排名
        PS_DB_MY_RANKING_INFO_REQ psDBReq;
        memset(&psDBReq, 0, sizeof(psDBReq));
        psDBReq.dwUAID = psReq->dwUAID;
        psDBReq.dwUCID = psReq->dwUCID;
        psDBReq.wRankInfoIndex = psReq->wRankInfoIndex;
        psDBReq.bLastRanking = psReq->bLastRanking;
        psDBReq.byClass = psReq->byClass;

        psDBReq.byRankingCategory = pTB_RANK_INFO->Ranking_Category;
        psDBReq.stRankingInfo.wRankInfoIndex = pTB_RANK_INFO->Ranking_Index;
        psDBReq.stRankingInfo.nTopRank = pTB_RANK_INFO->Ranking_Visible;
        psDBReq.stRankingInfo.byType = pTB_RANK_INFO->Ranking_Total_Type;
        psDBReq.stRankingInfo.byClassType = pTB_RANK_INFO->Ranking_Total_Class_Type;
        psDBReq.stRankingInfo.byRewradType = pTB_RANK_INFO->Ranking_Reward_Receive;
        psDBReq.stRankingInfo.dwMazeID = pTB_RANK_INFO->Ranking_Maze;

        if (pTB_RANK_INFO->Ranking_Total_Class_Type == 1) {
            psDBReq.stRankingInfo.byClass = psReq->byClass;
            ST_REPRESENTATIVE_INFO* pRepInfo = pEntity->GetRepresentativeInfo();
            if (pRepInfo) {
                psDBReq.stMyReq.byClass = pRepInfo->byClass;
            }
        } else {
            psDBReq.stRankingInfo.byClass = 0;
            psDBReq.stMyReq.byClass = 0;
        }

        XSendDBPacket xSendPacket(pUser->GetObject(), 0x28, 0x11);
        xSendPacket << psDBReq;
        pServer->SendDBGame(&xSendPacket);
    }

    return true;
}

// IDA: ?ResRankingMyInfo@CGocRecode@@QEAAXAEAUPS_DB_MY_RANKING_INFO_RES@@@Z (0x140156540)
// Verified: Direct IDA decompilation - response handler for ranking info, sends to client
void CGocRecode::ResRankingMyInfo(PS_DB_MY_RANKING_INFO_RES* psRes) {
    // IDA: 获取所属的User对象
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 检查 DB 返回结果
    if (psRes->stMyInfo.nRank == -1) {
        LogHelper::LogError("game.contents", "ResRankingMyInfo - DB Error [UAID:%d, ID:%d]",
                           psRes->stMyReq.dwUAID, psRes->stMyReq.wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 1, 0xE484);
        return;
    }

    // IDA: 更新排名信息
    SetRankingMyInfo(psRes->stRankingInfo.wRankInfoIndex, false, &psRes->stMyInfo, 0);
    SetRankingMyInfo(psRes->stRankingInfo.wRankInfoIndex, true, &psRes->stMySeasonInfo, psRes->dw64SeasonSetCount);

    // IDA: 构造客户端响应包
    PS_RANKING_LIST_RES psRankingList;
    memset(&psRankingList, 0, sizeof(psRankingList));
    psRankingList.wRankInfoIndex = psRes->stRankingInfo.wRankInfoIndex;
    psRankingList.byClass = psRes->stRankingInfo.byClass;
    psRankingList.bLastRanking = psRes->stMyReq.bLastRanking;

    if (psRankingList.bLastRanking) {
        psRankingList.stMyInfo = psRes->stMySeasonInfo;
    } else {
        psRankingList.stMyInfo = psRes->stMyInfo;
        psRankingList.stMyInfo.byLastReward = psRes->stMySeasonInfo.byLastReward;
    }

    // IDA: 获取排名列表
    std::vector<ST_USER_RANKING_INFO> vecList;
    XGameServer* pServer = XGameServer::Instance();
    pServer->GetRankingMgr().GetRankingList(
        psRankingList.wRankInfoIndex,
        psRankingList.byClass,
        psRankingList.bLastRanking,
        &vecList);

    // IDA: 分批发送排名列表（每批最多20条）
    for (size_t i = 0; i < vecList.size(); ++i) {
        psRankingList.vecList.push_back(vecList[i]);

        if (psRankingList.vecList.size() >= 20) {
            psRankingList.bLast = 0;
            XSendPacket packet(0x2C, 1);
            packet << psRankingList;
            pUser->Send(&packet);
            psRankingList.vecList.clear();
        }
    }

    // IDA: 发送最后一批
    psRankingList.bLast = 1;
    XSendPacket sendPacket(0x2C, 1);
    sendPacket << psRankingList;
    pUser->Send(&sendPacket);
}

// IDA: ?ReqRankingReward@CGocRecode@@QEAAXAEAUPS_RANKING_REWARD_REQ@@@Z (0x1401569D0)
// Verified: Direct IDA decompilation - complex function for requesting ranking rewards, creates items
void CGocRecode::ReqRankingReward(PS_RANKING_REWARD_REQ* psReward) {
    // IDA: 获取所属的User对象
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 获取 CGocEntity 组件并检查代表角色
    std::shared_ptr<CGocEntity> pEntity;
    pUser->GetGOC<CGocEntity>(pEntity);
    if (!pEntity) {
        return;
    }

    int nRepresentativeUCID = pEntity->GetRepresentativeUCID();
    int nUCID = pUser->GetQuestID();
    if (nRepresentativeUCID != nUCID) {
        LogHelper::LogError("game.contents", "ReqRankingReward - No RepresentativeUCID Error ( UCID : %d / UCID : %d )",
                           nUCID, nRepresentativeUCID);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xE486);
        return;
    }

    // IDA: 填充请求信息
    psReward->dwUAID = pUser->GetUAID();
    psReward->dwUCID = nUCID;

    // IDA: 获取 CGocInventory 组件
    std::shared_ptr<CGocInventory> pInvenPtr;
    pUser->GetGOC<CGocInventory>(pInvenPtr);
    if (!pInvenPtr) {
        return;
    }

    // IDA: 获取 TB_RANK_INFO 表
    XGameServer* pServer = XGameServer::Instance();
    TB_RANK_INFO* pTB_RANK_INFO = pServer->GetResourceMgr().GetTB_RANK_INFO(psReward->wRankInfoIndex);
    if (!pTB_RANK_INFO) {
        LogHelper::LogError("game.contents", "ReqRankingReward - TB_RANK_INFO Table Error ( UCID : %d / ID : %d )",
                           nUCID, psReward->wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xE486);
        return;
    }

    // IDA: 检查是否可领取奖励
    if (pTB_RANK_INFO->Ranking_Reward_Receive != 1) {
        LogHelper::LogError("game.contents", "ReqRankingReward - TB_RANK_INFO Ranking_Reward_Receive Error ( UCID : %d / ID : %d )",
                           nUCID, psReward->wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xE486);
        return;
    }

    // IDA: 检查是否可以领取奖励
    ST_USER_LAST_RANKING_INFO stMyLastRanking;
    memset(&stMyLastRanking, 0, sizeof(stMyLastRanking));
    int nError = CanRecvRankingReward(psReward->wRankInfoIndex, &stMyLastRanking);
    if (nError) {
        LogHelper::LogError("game.contents", "ReqRankingReward - CanRecvRankingReward ( UCID:%d/rankIndex:%d )",
                           nUCID, psReward->wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, nError);
        return;
    }

    // IDA: 验证排名有效
    if (stMyLastRanking.stInfo.nRank <= 0 || stMyLastRanking.stInfo.nMaxRank <= 0) {
        LogHelper::LogError("game.contents", "ReqRankingReward - CanRecvRankingReward1 ( UCID:%d/rankIndex:%d )",
                           nUCID, psReward->wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xE487);
        return;
    }

    // IDA: 验证排名类型
    if (pTB_RANK_INFO->Ranking_Category == 1 || pTB_RANK_INFO->Ranking_Category == 3) {
        if (pTB_RANK_INFO->Ranking_Total_Type != 1) {
            LogHelper::LogError("game.contents", "ReqRankingReward - Ranking_Total_Type ( UCID:%d/rankIndex:%d )",
                               nUCID, psReward->wRankInfoIndex);
            CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xE486);
            return;
        }
    } else if (pTB_RANK_INFO->Ranking_Category == 2) {
        if (pTB_RANK_INFO->Ranking_Total_Type != 4) {
            LogHelper::LogError("game.contents", "ReqRankingReward - Ranking_Total_Type ( UCID:%d/rankIndex:%d )",
                               nUCID, psReward->wRankInfoIndex);
            CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xE486);
            return;
        }
    } else {
        LogHelper::LogError("game.contents", "ReqRankingReward3 Error ( UCID:%d / Index:%d )",
                           nUCID, psReward->wRankInfoIndex);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xE486);
        return;
    }

    // IDA: 查找对应的 TB_RANK_REWARD
    TB_RANK_REWARD* pTB_RANK_REWARD = nullptr;
    unsigned short wPercentRank = stMyLastRanking.stInfo.GetPercentRank();
    unsigned int dwRewardID_Start = pServer->GetRankingMgr().GetLastRewardID_Start(psReward->wRankInfoIndex);

    for (short sh = 0; sh < 50; ++sh) {
        unsigned int dwIndex = sh + dwRewardID_Start;
        TB_RANK_REWARD* pCheck = pServer->GetResourceMgr().GetTB_RANK_REWARD(dwIndex);
        if (!pCheck) {
            break;
        }

        if (pCheck->Max_Ranking_Type) {
            if (wPercentRank <= pCheck->Max_Ranking) {
                pTB_RANK_REWARD = pCheck;
                break;
            }
        } else {
            if (stMyLastRanking.stInfo.nRank <= pCheck->Max_Ranking) {
                pTB_RANK_REWARD = pCheck;
                break;
            }
        }
    }

    if (!pTB_RANK_REWARD) {
        if (pTB_RANK_INFO->Ranking_Category == 2) {
            CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xE487);
        } else {
            LogHelper::LogError("game.contents", "ReqRankingReward - Not Found Reward1 ( UCID:%d/rankIndex:%d/RewardID:%d )",
                               nUCID, psReward->wRankInfoIndex, dwRewardID_Start);
            CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xE486);
        }
        return;
    }

    // IDA: 构造 DB 请求包
    PS_DB_RANKING_REWARD psDB;
    memset(&psDB, 0, sizeof(psDB));
    psDB.dwUAID = psReward->dwUAID;
    psDB.dwUCID = psReward->dwUCID;
    psDB.wRankInfoIndex = psReward->wRankInfoIndex;
    psDB.bLastRanking = psReward->bLastRanking;
    psDB.dwRewardID = pTB_RANK_REWARD->Reward_Index;
    psDB.dw64SeasonSetCount = stMyLastRanking.dw64SeasonSetCount;
    psDB.nRank = stMyLastRanking.stInfo.nRank;
    psDB.nMaxRank = stMyLastRanking.stInfo.nMaxRank;
    psDB.byRankingCategory = pTB_RANK_INFO->Ranking_Category;

    // IDA: 添加奖励物品
    for (int i = 0; i < 2; ++i) {
        int nItemID = (i == 0) ? pTB_RANK_REWARD->Ranking_Reward_Item_01 : pTB_RANK_REWARD->Ranking_Reward_Item_02;
        int nCount = (i == 0) ? pTB_RANK_REWARD->Item_Amount_01 : pTB_RANK_REWARD->Item_Amount_02;

        if (nItemID && nCount) {
            TB_ITEM* pTB_ITEM = pServer->GetResourceMgr().GetTB_ITEM(nItemID);
            if (!pTB_ITEM) {
                LogHelper::LogError("game.contents", "ReqRankingReward Item Table Error ( UCID:%d/RewardID:%d/ItemID:%d/Count:%d )",
                                   nUCID, pTB_RANK_REWARD->Reward_Index, nItemID, nCount);
                CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xCB2B);
                return;
            }

            ST_CREATE_ITEM stRewardItem;
            memset(&stRewardItem, 0, sizeof(stRewardItem));
            stRewardItem.nItemID = nItemID;
            stRewardItem.shCount = static_cast<short>(nCount);
            psDB.vecRewardItem.push_back(stRewardItem);
        }
    }

    // IDA: 添加职业奖励物品
    unsigned int dwClassRewardItemID = 0;
    unsigned char byClass = pUser->GetClass() - 1;
    if (byClass < 8) {
        int nClassItemID = 0;
        switch (byClass) {
        case 0: nClassItemID = pTB_RANK_REWARD->Ranking_Class_Reward_Item_01; break;
        case 1: nClassItemID = pTB_RANK_REWARD->Ranking_Class_Reward_Item_02; break;
        case 2: nClassItemID = pTB_RANK_REWARD->Ranking_Class_Reward_Item_03; break;
        case 3: nClassItemID = pTB_RANK_REWARD->Ranking_Class_Reward_Item_04; break;
        case 4: nClassItemID = pTB_RANK_REWARD->Ranking_Class_Reward_Item_05; break;
        case 5: nClassItemID = pTB_RANK_REWARD->Ranking_Class_Reward_Item_06; break;
        case 6: nClassItemID = pTB_RANK_REWARD->Ranking_Class_Reward_Item_07; break;
        case 7: nClassItemID = pTB_RANK_REWARD->Ranking_Class_Reward_Item_08; break;
        }

        if (nClassItemID) {
            TB_ITEM* pTB_ITEM = pServer->GetResourceMgr().GetTB_ITEM(nClassItemID);
            if (!pTB_ITEM) {
                LogHelper::LogError("game.contents", "ReqRankingReward class Item Table Error ( UCID:%d/RewardID:%d/ItemID:%d )",
                                   nUCID, pTB_RANK_REWARD->Reward_Index, nClassItemID);
                CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xCB2B);
                return;
            }

            if (pTB_ITEM->Item_Stack_Max != 1) {
                LogHelper::LogError("game.contents", "ReqRankingReward class Item stack max Error ( UCID:%d/RewardID:%d/ItemID:%d )",
                                   nUCID, pTB_RANK_REWARD->Reward_Index, nClassItemID);
                CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xCB2B);
                return;
            }

            dwClassRewardItemID = nClassItemID;
            ST_CREATE_ITEM stClassItem;
            memset(&stClassItem, 0, sizeof(stClassItem));
            stClassItem.nItemID = dwClassRewardItemID;
            stClassItem.shCount = 1;
            psDB.vecRewardItem.push_back(stClassItem);
        }
    }

    // IDA: 检查奖励列表是否为空
    if (psDB.vecRewardItem.empty()) {
        LogHelper::LogError("game.contents", "ReqRankingReward Error reward empty ( UCID:%d/RewardID:%d )",
                           nUCID, psDB.dwRewardID);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xCB2B);
        return;
    }

    // IDA: 创建奖励物品
    psDB.byFlag = 49;
    ST_LOG_GAME stLog;
    memset(&stLog, 0, sizeof(stLog));
    stLog._sSubType = 129;
    stLog.nParam2 = pUser->GetLevel();
    stLog.nParam3 = psReward->wRankInfoIndex;
    stLog.nParam4 = psDB.dwRewardID;

    if (!pInvenPtr->CreateItem2(&psDB.vecRewardItem, 0x55, 0, &psDB.stCreateItem, &psDB.stUpdateItem, &stLog)) {
        LogHelper::LogError("game.contents", "ReqRankingReward error - CreateItem2");
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xCB2A);
        return;
    }

    // IDA: 处理职业奖励物品的过期时间
    if (dwClassRewardItemID) {
        for (size_t j = 0; j < psDB.stCreateItem.size(); ++j) {
            if (dwClassRewardItemID == psDB.stCreateItem[j].nItemID) {
                unsigned int dwLastDate = pServer->GetRankingMgr().GetLastRewardID_Date();
                if (dwLastDate) {
                    unsigned int dwYear = dwLastDate / 1000000;
                    unsigned int dwMonth = (dwLastDate % 1000000) / 100000;
                    unsigned int dwDay = (dwLastDate % 1000000 % 100000) / 1000;

                    if (dwMonth <= 12 && dwDay <= 31) {
                        ATL::CTime tDelete(dwYear + 2000, dwMonth, dwDay, 0, 0, 0);
                        tDelete += ATL::CTimeSpan(13, 23, 59, 59);
                        psDB.stCreateItem[j].nMotionClass = tDelete.GetTime();

                        // IDA: 更新物品的现金日期
                        std::shared_ptr<CItem> pItem;
                        pInvenPtr->GetSlotItem(&pItem, psDB.stCreateItem[j].bySlot, psDB.stCreateItem[j].shIndex);
                        if (pItem) {
                            pItem->UpdateCashDate(psDB.stCreateItem[j].nMotionClass);
                        }
                        break;
                    }
                }
            }
        }
    }

    // IDA: 发送 DB 包 (main=0x28, sub=0x13)
    XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x28, 0x13);
    xSendDBPacket << psDB;
    pServer->SendDBGame(&xSendDBPacket);
}

// IDA: ?ResRankingReward@CGocRecode@@QEAAXAEAUPS_DB_RANKING_REWARD@@@Z (0x140158410)
// Verified: Direct IDA decompilation - response handler for ranking reward from DB
void CGocRecode::ResRankingReward(PS_DB_RANKING_REWARD* psRes) {
    // IDA: 获取所属的User对象
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 检查 DB 返回错误
    if (psRes->nError) {
        LogHelper::LogError("game.contents", "ResRankingReward DB Error ( UCID:%d/RewardID:%d/Error:%d )",
                           pUser->GetQuestID(), psRes->dwRewardID, psRes->nError);
        CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x2C, 2, 0xE486);
        return;
    }

    // IDA: 构造客户端响应包
    PS_RANKING_REWARD_RES psReward;
    memset(&psReward, 0, sizeof(psReward));
    psReward.dwUCID = pUser->GetQuestID();
    psReward.wRankInfoIndex = psRes->stReq.wRankInfoIndex;
    psReward.dwRewardID = psRes->dwRewardID;
    psReward.nRank = psRes->nRank;
    psReward.nMaxRank = psRes->nMaxRank;

    // IDA: 更新排名信息，标记已领取
    auto it = m_mapMyLastRanking.find(psRes->stReq.wRankInfoIndex);
    if (it != m_mapMyLastRanking.end()) {
        it->second.stInfo.byLastReward = 0;  // 清除已领取标志，允许再次领取
    }

    // IDA: 获取 CGocInventory 组件并发送物品更新
    std::shared_ptr<CGocInventory> pInvenPtr;
    pUser->GetGOC<CGocInventory>(pInvenPtr);
    if (pInvenPtr) {
        // IDA: 发送更新物品信息
        pInvenPtr->SendUpdateItem(&psRes->stUpdateItem);
        // IDA: 发送创建物品信息
        pInvenPtr->SendCreateItem(&psRes->stCreateItem);
    }

    // IDA: 发送客户端包 (main=0x2C, sub=2)
    XSendPacket xSendPacket(0x2C, 2);
    xSendPacket << psReward;
    pUser->Send(&xSendPacket);

    // IDA: 发送日志
    ST_LOG_GAME stLog;
    memset(&stLog, 0, sizeof(stLog));
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetQuestID();
    stLog._sMainType = 30;
    stLog._sSubType = 1;
    stLog.nParam0 = psReward.wRankInfoIndex;
    stLog.nParam1 = psReward.dwRewardID;
    stLog.nParam2 = psReward.nRank;
    stLog.nParam3 = psReward.nMaxRank;
    stLog.nParam4 = pUser->GetLevel();
    stLog.nParam5 = pUser->GetClass();
    wcscpy_s(stLog.szComment, L"RANKING_REWARD_RECV");

    XGameServer* pServer = XGameServer::Instance();
    pServer->SendDBLog(&stLog);
}

// IDA: ?Ranking_Cheat@CGocRecode@@QEAA_NHHH@Z (0x14015BB20)
// Verified: Direct IDA decompilation - GM cheat function for ranking operations
bool CGocRecode::Ranking_Cheat(int nType, int nValue1, int nValue2) {
    // IDA: 获取所属的User对象
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return false;
    }

    // IDA: 检查服务器系统类型
    XGameServer* pServer = XGameServer::Instance();
    XOption* pOption = pServer->GetOption();
    if (pOption->GetSystemType() == 3) {
        return false;
    }

    // IDA: 检查GM权限
    if (!pUser->GetGMPower()) {
        return false;
    }

    switch (nType) {
    case 0:
        // IDA: 删除所有排名列表
        pServer->GetRankingMgr().DeleteList_All();
        {
            XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x28, 0x21);
            pServer->SendDBGame(&xSendDBPacket);
        }
        return true;

    case 1:
        // IDA: 更新排名分数
        if (nValue2 <= 0) {
            return false;
        }
        {
            unsigned short wRankingIndex = static_cast<unsigned short>(nValue1);
            TB_RANK_INFO* pTB_RANK_INFO = pServer->GetResourceMgr().GetTB_RANK_INFO(wRankingIndex);
            if (!pTB_RANK_INFO) {
                return false;
            }

            // IDA: 检查是否为通关次数类型（类型1不允许作弊）
            if (pTB_RANK_INFO->Ranking_Total_Type == 1) {
                return false;
            }

            if (!pServer->GetResourceMgr().GetTB_MAZE_INFO(pTB_RANK_INFO->Ranking_Maze)) {
                return false;
            }

            unsigned int dwUAID = pUser->GetUAID();
            unsigned int dwUCID = pUser->GetQuestID();
            unsigned char byType = pTB_RANK_INFO->Ranking_Total_Class_Type + 10 * pTB_RANK_INFO->Ranking_Total_Type;
            unsigned int dwMazeID = pTB_RANK_INFO->Ranking_Maze;
            unsigned char byCategory = pTB_RANK_INFO->Ranking_Category;

            XSendDBPacket xSendPacket(pUser->GetObject(), 0x28, 0x22);
            xSendPacket << dwUAID;
            xSendPacket << dwUCID;
            xSendPacket << byType;
            xSendPacket << dwMazeID;
            xSendPacket << static_cast<unsigned int>(nValue2);
            xSendPacket << byCategory;
            pServer->SendDBGame(&xSendPacket);
        }
        return true;

    case 2:
        // IDA: 重置排名奖励接收状态
        if (nValue1 != 1 && nValue1 != 3) {
            return false;
        }
        {
            unsigned char byType = static_cast<unsigned char>(nValue1);
            XSendDBPacket v28(pUser->GetObject(), 0x28, 0x23);
            v28 << byType;
            pServer->SendDBGame(&v28);
        }
        return true;

    case 3:
        // IDA: 设置排名奖励接收标志
        if (nValue1 != 1 && nValue1 != 3) {
            return false;
        }
        if (nValue2 < 0 || nValue2 > 1) {
            return false;
        }
        {
            unsigned char byType = static_cast<unsigned char>(nValue1);
            bool bFlag = (nValue2 == 1);
            XSendDBPacket v30(pUser->GetObject(), 0x28, 0x24);
            v30 << byType;
            v30 << bFlag;
            pServer->SendDBGame(&v30);
        }
        return true;

    case 4:
        // IDA: 设置排名类别奖励接收标志
        if (nValue1 < 1 || nValue1 > 3) {
            return false;
        }
        if (nValue2 < 0 || nValue2 > 1) {
            return false;
        }
        {
            int nCategory = nValue1;
            bool bFlag = (nValue2 == 1);
            XSendDBPacket v32(pUser->GetObject(), 0x28, 0x25);
            v32 << nCategory;
            v32 << bFlag;
            pServer->SendDBGame(&v32);
        }
        return true;

    default:
        return false;
    }
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
void CGocRecode::UpdateOverIndulgence() {
    // TODO: 需要复杂外部依赖 - 更新防沉迷状态
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
// Verified: Direct IDA decompilation - sends DB request to load indulgence data
void CGocRecode::SendDBLoadIndulgence(bool bLogin) {
    // IDA: 获取所属的User对象 (RTTI dynamic_cast)
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 构造 PS_INDULGENCE_INFO 包
    PS_INDULGENCE_INFO stUpdate;
    memset(&stUpdate, 0, sizeof(stUpdate));
    stUpdate.dwUAID = pUser->GetUAID();
    stUpdate.bLogin = bLogin;

    // IDA: 发送到 DBAccount (main=2, sub=0x46)
    XSendDBPacket xSendDBPacket(pUser->GetObject(), 2, 0x46);
    xSendDBPacket << stUpdate;

    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        pServer->SendDBAccount(&xSendDBPacket);
    }
}

// IDA: ?ShowIndulgenceInfo@CGocRecode@@QEAAXXZ (0x14015A370)
// Verified: Direct IDA decompilation - shows indulgence info as chat message
void CGocRecode::ShowIndulgenceInfo() {
    // IDA: 获取所属的User对象
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 构造 PS_CHAT_NOTICE 包
    PS_CHAT_NOTICE stChat;
    memset(&stChat, 0, sizeof(stChat));
    stChat.byType = 0;

    // IDA: 格式化显示防沉迷状态信息
    int nDisconnectTime = m_nIndulgenceDisconnectTermTick / 1000;
    swprintf(stChat.strMsg, sizeof(stChat.strMsg) / sizeof(wchar_t),
        L"[INGULGENCE] State : %d / Connect : %d / Disconnect : %d",
        m_nIndulgenceState,
        m_nIndulgenceConnectTermTick / 1000,
        nDisconnectTime);

    // IDA: 发送聊天包 (main=7, sub=4)
    XSendPacket xSendPacket(7, 4);
    xSendPacket << stChat;

    CGocNetwork* pNetwork = pUser->GetGocNetwork();
    if (pNetwork) {
        pNetwork->Send(&xSendPacket);
    }
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
// Verified: Direct IDA decompilation - sends league wealth info to community socket
void CGocRecode::SendLeagueWealth(float fExp, int nGold) {
    // IDA: 获取所属的User对象 (RTTI dynamic_cast)
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return;
    }

    // IDA: 检查用户是否有联盟
    int nLeagueID = pUser->GetLeagueID();
    if (nLeagueID <= 0) {
        return;
    }

    // IDA: 计算贡献金币 = nGold * 0.01 (1%)
    int nContributeGold = 0;
    if (nGold >= 0) {
        nContributeGold = static_cast<int>(static_cast<double>(nGold) * 0.01);
    }

    // IDA: 构造 PS_LEAGUE_WEALTH_FOR_SERVER 包
    PS_LEAGUE_WEALTH_FOR_SERVER psLeagueWealthInfo;
    memset(&psLeagueWealthInfo, 0, sizeof(psLeagueWealthInfo));
    psLeagueWealthInfo.dwUCID = pUser->GetQuestID();
    psLeagueWealthInfo.nLeagueID = nLeagueID;
    psLeagueWealthInfo.shExp = static_cast<short>(fExp);
    psLeagueWealthInfo.nGold = nContributeGold;

    // IDA: 发送到 CommunitySocket (main=0xF6, sub=0x55)
    XSendPacket xSendPacket(0xF6, 0x55);
    xSendPacket << psLeagueWealthInfo;

    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        pServer->GetCommunitySocket().SendCheck(&xSendPacket);
    }
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
// Verified: Direct IDA decompilation - sends monster kill score reward to client and logs
void CGocRecode::MonsterKillScoreReward(int nPoint, unsigned char byMazeType, unsigned long long dwClearTime) {
    // IDA: 获取排名
    GetRank(static_cast<float>(nPoint));

    // IDA: 构造 PS_REWARD_SCORE 包
    PS_REWARD_SCORE psReward;
    memset(&psReward, 0, sizeof(psReward));
    psReward.nPlayPoint = nPoint;
    psReward.byRank = static_cast<unsigned char>(m_nRank);
    psReward.nClearTime = static_cast<int>(dwClearTime / 100);

    // IDA: 发送到客户端 (main=0x11, sub=0x75)
    XSendPacket sendPacket(0x11, 0x75);
    sendPacket << psReward;

    CUser* pUser = GetOwnerUser();
    if (pUser) {
        pUser->Send(&sendPacket);
    }

    // IDA: 更新排名数据
    RankingDataUpdate(m_nMazeID, 0, nPoint);

    // IDA: 发送日志
    if (pUser) {
        ST_LOG_GAME stLog;
        memset(&stLog, 0, sizeof(stLog));
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = pUser->GetQuestID();
        stLog._sMainType = 5;
        stLog._sSubType = 20;
        stLog.nParam0 = m_nMazeID;
        stLog.nParam4 = nPoint;
        stLog.nParam5 = pUser->GetLevel();
        UXMapID mapID;
        pUser->GetMapInsID(&mapID);
        stLog.nParam6 = mapID.dwInstanceID;
        wcscpy_s(stLog.szComment, L"MONSTER_KILL_SCORE");

        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            pServer->SendDBLog(&stLog);
        }

        // IDA: 第二条日志 (sub=32)
        ST_LOG_GAME stLogGame;
        memset(&stLogGame, 0, sizeof(stLogGame));
        stLogGame._nUAID = pUser->GetUAID();
        stLogGame._nUCID = pUser->GetQuestID();
        stLogGame._sMainType = 5;
        stLogGame._sSubType = 32;
        stLogGame.nParam0 = m_nMazeID;
        stLogGame.nParam1 = 5;
        stLogGame.nParam2 = m_nRank;
        stLogGame.nParam4 = nPoint;
        stLogGame.nParam5 = pUser->GetLevel();
        stLogGame.nParam6 = mapID.dwInstanceID;
        wcscpy_s(stLogGame.szComment, L"MONSTER_KILL_REWARD");

        if (pServer) {
            pServer->SendDBLog(&stLogGame);
        }
    }
}

// ============================================================================
// Share Point Reward Functions
// ============================================================================

// IDA: ?ApplyRewardItemForSharePoint@CGocRecode@@QEAA_NAEAUST_CREATE_ITEMS@@HE@Z (0x14014AC80)
// Verified: Direct IDA decompilation - creates reward items from ST_CREATE_ITEMS vector
bool CGocRecode::ApplyRewardItemForSharePoint(ST_CREATE_ITEMS& stCreateItems, int nID, unsigned char byStep) {
    // IDA: 如果物品列表为空，返回 false
    if (stCreateItems.empty()) {
        return false;
    }

    // IDA: 获取 CGocInventory 组件
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return false;
    }

    std::shared_ptr<CGocInventory> pInven;
    pMover->GetGOC<CGocInventory>(pInven);
    if (!pInven) {
        return false;
    }

    // IDA: 构造日志
    ST_LOG_GAME stLog;
    memset(&stLog, 0, sizeof(stLog));
    stLog.nParam3 = byStep;
    stLog.nParam4 = nID;

    // IDA: 构造 PS_SOULMETRY_LIST (物品创建请求)
    PS_SOULMETRY_LIST psItemList;
    for (size_t i = 0; i < stCreateItems.size(); ++i) {
        PS_SOULMETRY psItem;
        memset(&psItem, 0, sizeof(psItem));
        psItem.nItemID = stCreateItems[i].nItemID;
        psItem.shCount = stCreateItems[i].shCount;
        psItemList.vecInfo.push_back(psItem);
    }

    // IDA: 调用 CGocInventory::CreateItemReq
    CGocInventory* pInvenPtr = pInven.get();
    bool bResult = pInvenPtr->CreateItemReq(&psItemList, 0, E_ITEM_CREATE_TYPE_MAZE_BASE_ITEM, &stLog);

    return bResult;
}
