// ============================================================================
// STMonsterInfo.cpp - STMonsterInfo 结构体相关函数实现
//
// IDA 确认结构体大小: 128 bytes
// 结构体定义见: STMonsterInfo.h
// ============================================================================

#include "Soulworker/GameServer/XGameServer/STMonsterInfo.h"
#include <algorithm>

// ============================================================================
// STMonsterInfo 辅助函数实现
// ============================================================================

// ============================================================================
// GetSuperArmorRatio - 获取超级护甲比率
// 返回当前超级护甲与最大超级护甲的比率 (0.0 ~ 1.0)
// ============================================================================
float STMonsterInfo_GetSuperArmorRatio(const STMonsterInfo* pInfo) {
    if (!pInfo || pInfo->fMaxSuperArmor <= 0.0f) {
        return 0.0f;
    }
    return pInfo->fCurSuperArmor / pInfo->fMaxSuperArmor;
}

// ============================================================================
// SetSuperArmor - 设置超级护甲值
// ============================================================================
void STMonsterInfo_SetSuperArmor(STMonsterInfo* pInfo, float fCur, float fMax) {
    if (pInfo) {
        pInfo->fCurSuperArmor = fCur;
        pInfo->fMaxSuperArmor = fMax;
    }
}

// ============================================================================
// ResetSuperArmor - 重置超级护甲
// ============================================================================
void STMonsterInfo_ResetSuperArmor(STMonsterInfo* pInfo) {
    if (pInfo) {
        pInfo->fCurSuperArmor = 0.0f;
        pInfo->fMaxSuperArmor = 0.0f;
    }
}

// ============================================================================
// AddStat - 添加属性信息
// ============================================================================
void STMonsterInfo_AddStat(STMonsterInfo* pInfo, std::uint8_t byIndex, float fValue) {
    if (pInfo) {
        StatInfo stat;
        stat.byIndex = byIndex;
        stat.statValue = fValue;
        pInfo->vecStat.push_back(stat);
    }
}

// ============================================================================
// GetStat - 获取指定索引的属性值
// 返回: 属性值，如果不存在返回 0.0f
// ============================================================================
float STMonsterInfo_GetStat(const STMonsterInfo* pInfo, std::uint8_t byIndex) {
    if (!pInfo) {
        return 0.0f;
    }

    for (const auto& stat : pInfo->vecStat) {
        if (stat.byIndex == byIndex) {
            return stat.statValue;
        }
    }

    return 0.0f;
}

// ============================================================================
// SetStat - 设置指定索引的属性值
// 如果属性不存在则添加，否则更新
// ============================================================================
void STMonsterInfo_SetStat(STMonsterInfo* pInfo, std::uint8_t byIndex, float fValue) {
    if (!pInfo) {
        return;
    }

    // 查找现有属性
    for (auto& stat : pInfo->vecStat) {
        if (stat.byIndex == byIndex) {
            stat.statValue = fValue;
            return;
        }
    }

    // 添加新属性
    StatInfo newStat;
    newStat.byIndex = byIndex;
    newStat.statValue = fValue;
    pInfo->vecStat.push_back(newStat);
}

// ============================================================================
// ClearStats - 清空所有属性
// ============================================================================
void STMonsterInfo_ClearStats(STMonsterInfo* pInfo) {
    if (pInfo) {
        pInfo->vecStat.clear();
    }
}

// ============================================================================
// GetStatCount - 获取属性数量
// ============================================================================
std::size_t STMonsterInfo_GetStatCount(const STMonsterInfo* pInfo) {
    if (!pInfo) {
        return 0;
    }
    return pInfo->vecStat.size();
}

// ============================================================================
// SetParentActorID - 设置父 ActorID
// ============================================================================
void STMonsterInfo_SetParentActorID(STMonsterInfo* pInfo, UXActorID uxParentID) {
    if (pInfo) {
        pInfo->uxParentActorID = uxParentID;
    }
}

// ============================================================================
// GetSpawnBoxID - 获取生成盒 ID
// ============================================================================
int STMonsterInfo_GetSpawnBoxID(const STMonsterInfo* pInfo) {
    if (!pInfo) {
        return 0;
    }
    return pInfo->nSpawnBoxID;
}

// ============================================================================
// SetSpawnBoxID - 设置生成盒 ID
// ============================================================================
void STMonsterInfo_SetSpawnBoxID(STMonsterInfo* pInfo, int nSpawnBoxID) {
    if (pInfo) {
        pInfo->nSpawnBoxID = nSpawnBoxID;
    }
}

// ============================================================================
// GetMotionClass - 获取动作类型
// ============================================================================
int STMonsterInfo_GetMotionClass(const STMonsterInfo* pInfo) {
    if (!pInfo) {
        return 0;
    }
    return pInfo->nMotionClass;
}

// ============================================================================
// SetMotionClass - 设置动作类型
// ============================================================================
void STMonsterInfo_SetMotionClass(STMonsterInfo* pInfo, int nMotionClass) {
    if (pInfo) {
        pInfo->nMotionClass = nMotionClass;
    }
}

// ============================================================================
// IsBattlePos - 是否在战斗位置
// ============================================================================
bool STMonsterInfo_IsBattlePos(const STMonsterInfo* pInfo) {
    if (!pInfo) {
        return false;
    }
    return pInfo->bBattlePos;
}

// ============================================================================
// SetBattlePos - 设置战斗位置标志
// ============================================================================
void STMonsterInfo_SetBattlePos(STMonsterInfo* pInfo, bool bBattlePos) {
    if (pInfo) {
        pInfo->bBattlePos = bBattlePos;
    }
}

// ============================================================================
// GetSuicideTime - 获取自杀时间
// ============================================================================
float STMonsterInfo_GetSuicideTime(const STMonsterInfo* pInfo) {
    if (!pInfo) {
        return 0.0f;
    }
    return pInfo->fSuicideTime;
}

// ============================================================================
// SetSuicideTime - 设置自杀时间
// ============================================================================
void STMonsterInfo_SetSuicideTime(STMonsterInfo* pInfo, float fSuicideTime) {
    if (pInfo) {
        pInfo->fSuicideTime = fSuicideTime;
    }
}

// ============================================================================
// Reset - 重置所有字段到默认值
// ============================================================================
void STMonsterInfo_Reset(STMonsterInfo* pInfo) {
    if (!pInfo) {
        return;
    }

    // 重置 STNpcInfo 部分
    pInfo->stNpcInfo.uxActorID = UXActorID{};
    pInfo->stNpcInfo.stPosInfo = STPosInfo{};
    pInfo->stNpcInfo.nTableID = 0;
    pInfo->stNpcInfo.nHP = 0;
    pInfo->stNpcInfo.nWayPointID = 0;
    pInfo->stNpcInfo.nSectorID = 0;
    pInfo->stNpcInfo.byLevel = 0;

    // 重置 STMonsterInfo 特有字段
    pInfo->uxParentActorID = UXActorID{};
    pInfo->nSpawnBoxID = 0;
    pInfo->nMotionClass = 0;
    pInfo->bBattlePos = false;
    pInfo->fCurSuperArmor = 0.0f;
    pInfo->fMaxSuperArmor = 0.0f;
    pInfo->vecStat.clear();
    pInfo->fSuicideTime = 0.0f;
}

// ============================================================================
// CopyFrom - 从另一个 STMonsterInfo 复制数据
// ============================================================================
void STMonsterInfo_CopyFrom(STMonsterInfo* pDest, const STMonsterInfo* pSrc) {
    if (!pDest || !pSrc) {
        return;
    }

    // 复制 STNpcInfo 部分
    pDest->stNpcInfo = pSrc->stNpcInfo;

    // 复制 STMonsterInfo 特有字段
    pDest->uxParentActorID = pSrc->uxParentActorID;
    pDest->nSpawnBoxID = pSrc->nSpawnBoxID;
    pDest->nMotionClass = pSrc->nMotionClass;
    pDest->bBattlePos = pSrc->bBattlePos;
    pDest->fCurSuperArmor = pSrc->fCurSuperArmor;
    pDest->fMaxSuperArmor = pSrc->fMaxSuperArmor;
    pDest->vecStat = pSrc->vecStat;
    pDest->fSuicideTime = pSrc->fSuicideTime;
}
