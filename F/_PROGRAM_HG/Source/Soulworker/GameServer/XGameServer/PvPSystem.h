#pragma once

// PvPSystem.h
// PvP and Battle Arena System for SoulWorker GameServer
// Recovered from IDA GameServer.exe port 10004
// 
// Chinese: PvP 和战斗竞技场系统

#include <cstdint>

// Forward declarations
class CUser;
class CMoverEx;
class XActor;
class CBattleZone;
class CGocAttribute;
struct XVec3;

// ============================================================================
// PvP Condition Flags
// IDA: Used with CMoverEx::m_iPvpCondition
// ============================================================================
enum EPvPCondition : int {
    PVP_CONDITION_NONE         = 0,
    PVP_CONDITION_ACTIVE       = 1,      // Active in PvP
    PVP_CONDITION_ATTACKABLE   = 2,      // Can be attacked
    PVP_CONDITION_PROTECTED    = 4,      // Newbie protection
    PVP_CONDITION_WANTED       = 8,      // Wanted status
    PVP_CONDITION_KILLER       = 16,     // Player killer flag
};

// ============================================================================
// PvP Map Types
// IDA: XArea::IsPvPZone checks map IDs
// ============================================================================
enum EPvPMapType : unsigned short {
    PVP_MAP_NONE       = 0,
    PVP_MAP_ZONE_20005 = 20005,   // PvP District zone
    PVP_MAP_TUTORIAL   = 30031,   // Tutorial map (special PvP rules)
};

// ============================================================================
// PvP Server Options
// IDA: E_SERVER_OPTION_PVP_DISTRICT
// ============================================================================
enum EServerOptionPvP : int {
    SERVER_OPTION_PVP_DISTRICT = 72,   // Server option for PvP district
};

// ============================================================================
// PvP System Class
// Static helper functions for PvP functionality
// ============================================================================
class PvPSystem {
public:
    // ========================================================================
    // CMoverEx PvP Condition Functions
    // IDA: Direct member functions on CMoverEx
    // ========================================================================
    
    // IDA: ?IsPvpCondition@CMoverEx@@UEAAHH@Z (0x140188E90)
    // Chinese: 检查是否满足指定 PvP 条件
    static bool IsPvpCondition(CMoverEx* pMover, int iValue);
    
    // IDA: ?SetPvpCondition@CMoverEx@@UEAAXH@Z (0x140189190)
    // Chinese: 设置 PvP 条件标志
    static void SetPvpCondition(CMoverEx* pMover, int iValue);
    
    // IDA: ?GetPvpCondition@CMoverEx@@UEAAHXZ (0x1401891C0)
    // Chinese: 获取当前 PvP 条件值
    static int GetPvpCondition(CMoverEx* pMover);
    
    // IDA: ?ResetPvpCondition@CMoverEx@@QEAAXXZ (0x14070A6A0)
    // Chinese: 重置 PvP 条件为默认值
    static void ResetPvpCondition(CMoverEx* pMover);
    
    // ========================================================================
    // PvP Zone Detection
    // ========================================================================
    
    // IDA: ?IsPvPZone@XArea@@QEAA_NXZ (0x1408EF100)
    // Chinese: 检查当前区域是否为 PvP 区域
    // Returns true if world type is 2 with PVP_DISTRICT option, or map ID starts with 20005
    static bool IsPvPZone(class XArea* pArea);
    
    // IDA: ?IsEnemyPVP@CBattleZone@@UEAA_NPEAVXActor@@0@Z (0x1401A73D0)
    // Chinese: 检查两个 Actor 是否为 PvP 敌对关系
    // Checks: server option, safety zone, party membership, force membership, league status
    static bool IsEnemyPVP(class CBattleZone* pZone, XActor* pAttacker, XActor* pDefender);
    
    // IDA: ?IsEnemyPVP@XMaze@@UEAA_NPEAVXActor@@0@Z (0x14033AC90)
    // Chinese: 迷宫内 PvP 敌对检查
    // Returns true if not in cutscene and map ID is 20005
    static bool IsEnemyPVP_Maze(class XMaze* pMaze, XActor* pAttacker, XActor* pDefender);
    
    // IDA: ?IsEnemyPVP@XArea@@UEAA_NPEAVXActor@@0@Z (0x1402B5900)
    // Chinese: 区域 PvP 敌对检查 (默认实现)
    static bool IsEnemyPVP_Area(class XArea* pArea, XActor* pAttacker, XActor* pDefender);
    
    // IDA: ?IsEnemyPVP@XModeMaze@@UEAA_NPEAVXActor@@0@Z (0x140294200)
    // Chinese: ModeMaze PvP 敌对检查
    static bool IsEnemyPVP_ModeMaze(class XModeMaze* pModeMaze, XActor* pAttacker, XActor* pDefender);
    
    // ========================================================================
    // PvP Penalty System
    // ========================================================================
    
    // IDA: ?IsPVPPenalty@CUser@@QEAA_NXZ (0x1401ADC50)
    // Chinese: 检查用户是否有 PvP 惩罚
    static bool IsPVPPenalty(CUser* pUser);
    
    // IDA: ?SetPVPPenalty@CUser@@QEAAX_N@Z (0x1403E1A50)
    // Chinese: 设置 PvP 惩罚状态
    static void SetPVPPenalty(CUser* pUser, bool bPenalty);
    
    // ========================================================================
    // PvP Statistics (PvP Attack/Defense Stats)
    // ========================================================================
    
    // IDA: ?CALCULATE_STAT_PVP_ATK@CCalculateStatus@@QEAAMPEAVCGocAttribute@@@Z (0x1402D8670)
    // Chinese: 计算 PvP 攻击力属性
    // Returns attribute index 75 (PvP Attack)
    static float CalculatePvPAttack(CGocAttribute* pAttr);
    
    // IDA: ?CALCULATE_STAT_PVP_DEF@CCalculateStatus@@QEAAMPEAVCGocAttribute@@@Z (0x1402D86A0)
    // Chinese: 计算 PvP 防御力属性
    // Returns attribute index 76 (PvP Defense)
    static float CalculatePvPDefense(CGocAttribute* pAttr);
    
    // ========================================================================
    // PvP Drops (Dimension Shutter)
    // ========================================================================
    
    // IDA: ?MakeDropItemDemensionShutter_PVP@CDropProcess@@QEAA_NPEAVCUser@@KHAEAUXVec3@@@Z (0x14040E080)
    // Chinese: 生成 PvP 维度裂隙掉落物品
    // Object ID: 800009998 (special PvP drop marker)
    static bool MakeDropItemDimensionShutter_PVP(class CDropProcess* pProcess, CUser* pUser, 
                                                   unsigned int dwDieUCID, int nDropPoint, 
                                                   XVec3* pPos);
    
    // ========================================================================
    // PvP Resource Management
    // ========================================================================
    
    // IDA: ?GetRealPVPOption@XResourceMgr@@QEAAHXZ (0x1401E7BB0)
    // Chinese: 获取真实 PvP 选项状态
    static int GetRealPVPOption(class XResourceMgr* pMgr);
    
    // IDA: ?SetRealPVPOption@XResourceMgr@@QEAAXH@Z (0x1408B3950)
    // Chinese: 设置真实 PvP 选项状态
    static void SetRealPVPOption(class XResourceMgr* pMgr, int nState);
    
    // IDA: ?GetTB_BATTLE_ARENA_INFO@XResourceMgr@@QEAAPEAUTB_BATTLE_ARENA_INFO@@G@Z (0x140518DE0)
    // Chinese: 获取战斗竞技场信息表数据
    static struct TB_BATTLE_ARENA_INFO* GetBattleArenaInfo(class XResourceMgr* pMgr, unsigned short wIndex);
    
    // ========================================================================
    // PvP Matching System
    // (Integrated with Party/Force matching - not standalone PvP matching)
    // ========================================================================
    
    // IDA: ?CheckForceMatchingEnter@CGocForce@@QEAA_NXZ (0x140085210)
    // Chinese: 检查队伍是否可以进入匹配
    // Returns false if any member is in PVP penalty
    static bool CheckForceMatchingEnter(class CGocForce* pForce);
    
    // ========================================================================
    // PvP Ranking System
    // (Integrated with general ranking system in CGocRecode)
    // ========================================================================
    
    // IDA: ?RankingDataUpdate@CGocRecode@@QEAAXKKH@Z (0x140154150)
    // Chinese: 更新排名数据
    static void RankingDataUpdate(class CGocRecode* pRecode, unsigned long dwData1, 
                                   unsigned long dwData2, int nData3);
    
    // IDA: ?GetRank@CGocRecode@@QEAAXM@Z (0x140146D80)
    // Chinese: 根据分数获取排名
    static void GetRank(class CGocRecode* pRecode, float fPlayPoint);
    
    // IDA: ?SetRankingMyInfo@CGocRecode@@QEAAXG_NAEAUST_USER_RANKING_INFO@@_K@Z (0x1401554B0)
    // Chinese: 设置我的排名信息
    static void SetRankingMyInfo(class CGocRecode* pRecode, unsigned short wType, 
                                  bool bFlag, void* pInfo, unsigned long long dwData);
    
    // IDA: ?CanRecvRankingReward@CGocRecode@@QEAAHGAEAUST_USER_LAST_RANKING_INFO@@@Z (0x1401553D0)
    // Chinese: 检查是否可以领取排名奖励
    static bool CanRecvRankingReward(class CGocRecode* pRecode, unsigned short wType, void* pInfo);
    
    // IDA: ?ReqRankingList@CGocRecode@@QEAA_NAEAUPS_RANKING_LIST_REQ@@@Z (0x140155BC0)
    // Chinese: 请求排名列表
    static bool ReqRankingList(class CGocRecode* pRecode, void* pReq);
    
    // IDA: ?ResRankingMyInfo@CGocRecode@@QEAAXAEAUPS_DB_MY_RANKING_INFO_RES@@@Z (0x140156540)
    // Chinese: 响应我的排名信息
    static void ResRankingMyInfo(class CGocRecode* pRecode, void* pRes);
    
    // IDA: ?ReqRankingReward@CGocRecode@@QEAAXAEAUPS_RANKING_REWARD_REQ@@@Z (0x1401569D0)
    // Chinese: 请求排名奖励
    static void ReqRankingReward(class CGocRecode* pRecode, void* pReq);
    
    // IDA: ?ResRankingReward@CGocRecode@@QEAAXAEAUPS_DB_RANKING_REWARD@@@Z (0x140158410)
    // Chinese: 响应排名奖励
    static void ResRankingReward(class CGocRecode* pRecode, void* pRes);
    
    // IDA: ?Ranking_Cheat@CGocRecode@@QEAA_NHHH@Z (0x14015BB20)
    // Chinese: GM 作弊命令设置排名
    static bool Ranking_Cheat(class CGocRecode* pRecode, int nType, int nRank, int nScore);
};

// ============================================================================
// PvP Helper Inline Implementations
// These provide thin wrappers over existing member functions
// ============================================================================

inline bool PvPSystem::IsPvpCondition(CMoverEx* pMover, int iValue) {
    // IDA: return (iValue & this->m_iPvpCondition) != 0;
    // TODO: Requires CMoverEx::m_iPvpCondition member access
    // Direct call to member function when CMoverEx is defined
    return false;  // Stub - requires full CMoverEx implementation
}

inline void PvPSystem::SetPvpCondition(CMoverEx* pMover, int iValue) {
    // IDA: this->m_iPvpCondition |= iValue;
    // TODO: Requires CMoverEx::m_iPvpCondition member access
}

inline int PvPSystem::GetPvpCondition(CMoverEx* pMover) {
    // IDA: return (unsigned int)this->m_iPvpCondition;
    // TODO: Requires CMoverEx::m_iPvpCondition member access
    return 0;  // Stub
}

inline void PvPSystem::ResetPvpCondition(CMoverEx* pMover) {
    // IDA: this->m_iPvpCondition = 0;
    // TODO: Requires CMoverEx::m_iPvpCondition member access
}

inline bool PvPSystem::IsPVPPenalty(CUser* pUser) {
    // IDA: return this->m_bPVPPenalty;
    // TODO: Requires CUser::m_bPVPPenalty member access
    return false;  // Stub
}

inline void PvPSystem::SetPVPPenalty(CUser* pUser, bool bPenalty) {
    // IDA: this->m_bPVPPenalty = bPenalty;
    // TODO: Requires CUser::m_bPVPPenalty member access
}

inline float PvPSystem::CalculatePvPAttack(CGocAttribute* pAttr) {
    // IDA: return CGocAttribute::GetMaxInt(pAttr, 75);
    // Attribute index 75 = PvP Attack
    // TODO: Requires CGocAttribute::GetMaxInt implementation
    return 0.0f;  // Stub
}

inline float PvPSystem::CalculatePvPDefense(CGocAttribute* pAttr) {
    // IDA: return CGocAttribute::GetMaxInt(pAttr, 76);
    // Attribute index 76 = PvP Defense
    // TODO: Requires CGocAttribute::GetMaxInt implementation
    return 0.0f;  // Stub
}

inline int PvPSystem::GetRealPVPOption(class XResourceMgr* pMgr) {
    // IDA: return (unsigned int)this->m_nRealPVPOption;
    // TODO: Requires XResourceMgr::m_nRealPVPOption member access
    return 0;  // Stub
}

inline void PvPSystem::SetRealPVPOption(class XResourceMgr* pMgr, int nState) {
    // IDA: this->m_nRealPVPOption = nState;
    // TODO: Requires XResourceMgr::m_nRealPVPOption member access
}
