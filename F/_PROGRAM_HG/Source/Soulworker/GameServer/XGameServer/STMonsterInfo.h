#pragma once

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <vector>

// ============================================================================
// StatInfo - 属性信息
// IDA 确认大小: 8 bytes
// ============================================================================
struct StatInfo {
    std::uint8_t byIndex;       // offset 0
    char _pad0[3];              // offset 1-3 (padding)
    float statValue;            // offset 4

    StatInfo() : byIndex(0), statValue(0.0f) {}
};

// ============================================================================
// STPosInfo - 位置信息
// 注: 已在 PSCommon.h 中定义，此处不再重复
// ============================================================================

// ============================================================================
// STNpcInfo - NPC 信息
// IDA 确认大小: 64 bytes
// ============================================================================
struct STNpcInfo {
    UXActorID uxActorID;        // offset 0 (4 bytes)
    char _pad0[4];              // offset 4-7 (padding)
    STPosInfo stPosInfo;        // offset 8 (32 bytes)
    int nTableID;               // offset 40
    int nHP;                    // offset 44
    int nWayPointID;            // offset 48
    int nSectorID;              // offset 52
    std::uint8_t byLevel;       // offset 56
    char _pad1[7];              // offset 57-63 (padding)

    STNpcInfo()
        : uxActorID{}
        , stPosInfo{}
        , nTableID(0)
        , nHP(0)
        , nWayPointID(0)
        , nSectorID(0)
        , byLevel(0)
    {}
};

// ============================================================================
// STMonsterInfo - 怪物信息
// IDA 确认大小: 128 bytes
// ============================================================================
struct STMonsterInfo {
    STNpcInfo stNpcInfo;                    // offset 0 (64 bytes)
    UXActorID uxParentActorID;              // offset 64 (4 bytes)
    int nSpawnBoxID;                        // offset 68
    int nMotionClass;                       // offset 72
    bool bBattlePos;                        // offset 76
    char _pad0[3];                          // offset 77-79 (padding)
    float fCurSuperArmor;                   // offset 80
    float fMaxSuperArmor;                   // offset 84
    std::vector<StatInfo> vecStat;          // offset 88 (32 bytes)
    // Note: std::vector is typically 24 bytes, but IDA shows 32 bytes
    // This may include additional padding or extended allocator
    float fSuicideTime;                     // offset 120

    STMonsterInfo()
        : stNpcInfo{}
        , uxParentActorID{}
        , nSpawnBoxID(0)
        , nMotionClass(0)
        , bBattlePos(false)
        , fCurSuperArmor(0.0f)
        , fMaxSuperArmor(0.0f)
        , fSuicideTime(0.0f)
    {}

    // 便捷访问方法
    UXActorID& GetActorID() { return stNpcInfo.uxActorID; }
    const UXActorID& GetActorID() const { return stNpcInfo.uxActorID; }

    int GetHP() const { return stNpcInfo.nHP; }
    void SetHP(int nHP) { stNpcInfo.nHP = nHP; }

    int GetTableID() const { return stNpcInfo.nTableID; }
    void SetTableID(int nTableID) { stNpcInfo.nTableID = nTableID; }

    std::uint8_t GetLevel() const { return stNpcInfo.byLevel; }
    void SetLevel(std::uint8_t byLevel) { stNpcInfo.byLevel = byLevel; }
};

// ============================================================================
// STMonsterInfo 辅助函数声明
// ============================================================================

// 超级护甲相关
float STMonsterInfo_GetSuperArmorRatio(const STMonsterInfo* pInfo);
void STMonsterInfo_SetSuperArmor(STMonsterInfo* pInfo, float fCur, float fMax);
void STMonsterInfo_ResetSuperArmor(STMonsterInfo* pInfo);

// 属性相关
void STMonsterInfo_AddStat(STMonsterInfo* pInfo, std::uint8_t byIndex, float fValue);
float STMonsterInfo_GetStat(const STMonsterInfo* pInfo, std::uint8_t byIndex);
void STMonsterInfo_SetStat(STMonsterInfo* pInfo, std::uint8_t byIndex, float fValue);
void STMonsterInfo_ClearStats(STMonsterInfo* pInfo);
std::size_t STMonsterInfo_GetStatCount(const STMonsterInfo* pInfo);

// 父 ActorID 相关
void STMonsterInfo_SetParentActorID(STMonsterInfo* pInfo, UXActorID uxParentID);

// 生成盒 ID 相关
int STMonsterInfo_GetSpawnBoxID(const STMonsterInfo* pInfo);
void STMonsterInfo_SetSpawnBoxID(STMonsterInfo* pInfo, int nSpawnBoxID);

// 动作类型相关
int STMonsterInfo_GetMotionClass(const STMonsterInfo* pInfo);
void STMonsterInfo_SetMotionClass(STMonsterInfo* pInfo, int nMotionClass);

// 战斗位置相关
bool STMonsterInfo_IsBattlePos(const STMonsterInfo* pInfo);
void STMonsterInfo_SetBattlePos(STMonsterInfo* pInfo, bool bBattlePos);

// 自杀时间相关
float STMonsterInfo_GetSuicideTime(const STMonsterInfo* pInfo);
void STMonsterInfo_SetSuicideTime(STMonsterInfo* pInfo, float fSuicideTime);

// 重置和复制
void STMonsterInfo_Reset(STMonsterInfo* pInfo);
void STMonsterInfo_CopyFrom(STMonsterInfo* pDest, const STMonsterInfo* pSrc);
