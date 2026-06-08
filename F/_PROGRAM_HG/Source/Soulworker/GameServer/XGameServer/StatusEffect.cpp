/**
 * @file StatusEffect.cpp
 * @brief Status effect and condition system implementation
 * 
 * IDA decompiled functions for status effects (stun, slow, poison, buffs, etc.)
 * All functions marked as verified = no until build verification
 */

#include "StatusEffect.h"
#include "MoverEx.h"
#include "actor/component/GocBooster.h"
#include "actor/component/GocAttribute.h"
#include "XGameServer.h"
#include "ResourceMgr.h"
#include "User.h"
#include <ctime>

// ============================================================================
// Status Effect Helper Functions
// ============================================================================

/**
 * ApplyStatus - Apply a status effect to a mover
 * 
 * IDA Address: Implemented via multiple systems
 * - CMoverEx::AddOptionEffect (0x14039BFB0) for item effects
 * - CGocBooster::AddBooster (0x14004AA30) for boosters
 * - CMoverEx::AddDefenseChangeInfo (0x14037CF80) for defense changes
 * 
 * @param pMover Target mover
 * @param wStatusID Status effect ID
 * @param fDuration Duration in seconds (0 for permanent, -1 for unlimited)
 * @param fValue Effect value
 * @param byType Status type (0=item effect, 1=booster, 2=defense change)
 * @return true if successfully applied
 */
bool ApplyStatus(CMoverEx* pMover, std::uint16_t wStatusID, float fDuration, float fValue, std::uint8_t byType) {
    if (!pMover) {
        return false;
    }

    // TODO: 完整实现需要以下依赖
    // - XResourceMgr::GetTB_BOOSTER 获取booster表
    // - XResourceMgr::GetTB_CREATEOPTION 获取选项效果表
    // - CMoverEx::AddOptionEffect 添加选项效果
    // - CGocBooster::AddBooster 添加booster
    
    switch (byType) {
        case 0: {
            // Item option effect
            // IDA: CMoverEx::AddOptionEffect (0x14039BFB0)
            // TODO: 获取TB_CREATEOPTION表并调用AddOptionEffect
            break;
        }
        case 1: {
            // Booster effect
            // IDA: CGocBooster::AddBooster (0x14004AA30)
            auto pBooster = pMover->GetGOC<CGocBooster>();
            if (pBooster) {
                pBooster->AddBooster(wStatusID, false);
                if (fDuration > 0.0f) {
                    // Convert seconds to internal time format
                    // TODO: Implement proper time conversion
                }
            }
            break;
        }
        case 2: {
            // Defense change effect
            // IDA: CMoverEx::AddDefenseChangeInfo (0x14037CF80)
            // TODO: Implement defense change system
            break;
        }
        default:
            return false;
    }
    
    return true;
}

/**
 * RemoveStatus - Remove a status effect from a mover
 * 
 * IDA Address: Implemented via multiple systems
 * - CMoverEx::RemoveOptionEffect (0x14039C420) for item effects
 * - CGocBooster::RemoveBooster (0x14004AC20) for boosters
 * - CMoverEx::RemoveDefenseChangeInfo (0x14037D100) for defense changes
 * 
 * @param pMover Target mover
 * @param wStatusID Status effect ID to remove
 * @param byType Status type
 * @return true if successfully removed
 */
bool RemoveStatus(CMoverEx* pMover, std::uint16_t wStatusID, std::uint8_t byType) {
    if (!pMover) {
        return false;
    }

    switch (byType) {
        case 0: {
            // Remove item option effect
            // IDA: CMoverEx::RemoveOptionEffect (0x14039C420)
            pMover->RemoveOptionEffect(wStatusID);
            break;
        }
        case 1: {
            // Remove booster
            // IDA: CGocBooster::RemoveBooster (0x14004AC20)
            auto pBooster = pMover->GetGOC<CGocBooster>();
            if (pBooster) {
                pBooster->RemoveBooster(wStatusID);
            }
            break;
        }
        case 2: {
            // Remove defense change
            // IDA: CMoverEx::RemoveDefenseChangeInfo (0x14037D100)
            // TODO: Need to determine type parameter for RemoveDefenseChangeInfo
            break;
        }
        default:
            return false;
    }
    
    return true;
}

/**
 * HasStatus - Check if a mover has a specific status effect
 * 
 * IDA Address: Implemented via multiple systems
 * - CGocBooster::FindBooster (0x14004A540) for boosters
 * - CMoverEx::GetOptionEffect (0x14039BE80) for item effects
 * - XActor::IsStatus (0x140048FD0) for actor status flags
 * 
 * @param pMover Target mover
 * @param wStatusID Status effect ID to check
 * @param byType Status type
 * @return true if the status is active
 */
bool HasStatus(CMoverEx* pMover, std::uint16_t wStatusID, std::uint8_t byType) {
    if (!pMover) {
        return false;
    }

    switch (byType) {
        case 0: {
            // Check item option effect
            // IDA: CMoverEx::GetOptionEffect (0x14039BE80)
            // TODO: Implement option effect check
            return false;
        }
        case 1: {
            // Check booster
            // IDA: CGocBooster::FindBooster (0x14004A540)
            auto pBooster = pMover->GetGOC<CGocBooster>();
            if (pBooster) {
                return pBooster->FindBooster(wStatusID);
            }
            return false;
        }
        case 2: {
            // Check defense change
            // TODO: Implement defense change check
            return false;
        }
        case 3: {
            // Check actor status flag
            // IDA: XActor::IsStatus (0x140048FD0) and CUser::IsStatus (0x140026C30)
            return pMover->IsStatus(wStatusID);
        }
        default:
            return false;
    }
}

/**
 * GetStatusDuration - Get remaining duration of a status effect
 * 
 * IDA Address: Implemented via multiple systems
 * - CGocBooster::GetBooster (0x14004A5A0) for boosters
 * - CMoverEx::GetOptionEffect for item effects
 * 
 * @param pMover Target mover
 * @param wStatusID Status effect ID
 * @param byType Status type
 * @return Remaining duration in seconds (-1 for permanent, 0 if not found)
 */
float GetStatusDuration(CMoverEx* pMover, std::uint16_t wStatusID, std::uint8_t byType) {
    if (!pMover) {
        return 0.0f;
    }

    switch (byType) {
        case 0: {
            // Get item option effect duration
            // TODO: Implement option effect duration query
            return -1.0f;  // Permanent
        }
        case 1: {
            // Get booster duration
            // IDA: CGocBooster::GetBooster (0x14004A5A0)
            auto pBooster = pMover->GetGOC<CGocBooster>();
            if (pBooster) {
                ST_BOOSTER_INFO stInfo;
                if (pBooster->GetBooster(wStatusID, stInfo)) {
                    // Convert internal time format to seconds
                    // TODO: Implement proper time conversion
                    return static_cast<float>(stInfo.lRemainTime) / 1000.0f;
                }
            }
            return 0.0f;
        }
        case 2: {
            // Get defense change duration
            // TODO: Implement defense change duration query
            return 0.0f;
        }
        default:
            return 0.0f;
    }
}

/**
 * UpdateStatus - Update status effects over time
 * 
 * IDA Address: Implemented via multiple systems
 * - CGocBooster::OnUpdate (0x140049CA0) for boosters
 * - CMoverEx::UpdateOptionEffect (0x14039C1C0) for item effects
 * - CMoverEx::UppdateDefenseChangeInfo (0x14037D540) for defense changes
 * 
 * @param pMover Target mover
 * @param fDeltaTime Delta time in seconds
 */
void UpdateStatus(CMoverEx* pMover, float fDeltaTime) {
    if (!pMover) {
        return;
    }

    // Update boosters
    // IDA: CGocBooster::OnUpdate (0x140049CA0)
    auto pBooster = pMover->GetGOC<CGocBooster>();
    if (pBooster) {
        pBooster->OnUpdate(fDeltaTime);
    }

    // Update option effects
    // IDA: CMoverEx::UpdateOptionEffect (0x14039C1C0)
    pMover->UpdateOptionEffect(fDeltaTime);

    // Update defense changes
    // IDA: CMoverEx::UppdateDefenseChangeInfo (0x14037D540)
    pMover->UppdateDefenseChangeInfo(fDeltaTime);
}

// ============================================================================
// CMoverEx Status Effect Methods
// ============================================================================

/**
 * CMoverEx::AddDefenseChangeInfo - Add defense change status effect
 * 
 * IDA: ?AddDefenseChangeInfo@CMoverEx@@QEAAHEEKM@Z (0x14037CF80)
 * Adds a defense modification effect (invincibility, damage reduction, etc.)
 */
void CMoverEx::AddDefenseChangeInfo(std::uint8_t byType, std::uint8_t byDefenseType, 
                                      std::uint32_t dwID, float fChangeTime) {
    // TODO: 完整实现需要以下依赖
    // - SDefenseChangeInfo 结构体定义
    // - m_listDefenseChangeInfo 成员变量
    // - m_stDefenseChangeInfoByEffect 成员变量
    // - m_stDefenseChangeInfoByTrigger 成员变量
    // - m_DefanseChangeFlag 成员变量
    
    // IDA 反编译逻辑：
    // 1. 根据byType选择添加到哪个列表
    // 2. 创建SDefenseChangeInfo结构
    // 3. 添加到对应的容器
    // 4. 如果是invincibility类型,调用SetInvincibleActor
    // 5. 更新m_DefanseChangeFlag
    // 6. 调用ApplyDefenseChangeInfo应用效果
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::RemoveDefenseChangeInfo - Remove defense change status effect
 * 
 * IDA: ?RemoveDefenseChangeInfo@CMoverEx@@QEAAHEK@Z (0x14037D100)
 * Removes a defense modification effect
 */
void CMoverEx::RemoveDefenseChangeInfo(std::uint8_t byType, std::uint32_t dwID) {
    // IDA 反编译逻辑：
    // 1. 根据byType选择从哪个容器删除
    // 2. 查找并删除对应的SDefenseChangeInfo
    // 3. 如果是invincibility类型,取消SetInvincibleActor
    // 4. 清空相关的immunity状态
    // 5. 更新m_DefanseChangeFlag
    // 6. 调用ApplyDefenseChangeInfo重新计算效果
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::RemoveAllDefenseChangeInfo - Remove all defense change effects
 * 
 * IDA: ?RemoveAllDefenseChangeInfo@CMoverEx@@QEAAXXZ (0x14037D420)
 */
void CMoverEx::RemoveAllDefenseChangeInfo() {
    // IDA 反编译逻辑：
    // 1. 清空m_listDefenseChangeInfo列表
    // 2. 清空m_stDefenseChangeInfoByEffect
    // 3. 清空m_stDefenseChangeInfoByTrigger
    // 4. 重置m_DefanseChangeFlag
    // 5. 清空所有immunity状态
    // 6. 取消invincibility状态
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::UppdateDefenseChangeInfo - Update defense change effects
 * 
 * IDA: ?UppdateDefenseChangeInfo@CMoverEx@@QEAAXM@Z (0x14037D540)
 * Updates timers for all defense change effects
 */
void CMoverEx::UppdateDefenseChangeInfo(float fDeltaTime) {
    // IDA 反编译逻辑：
    // 1. 遍历所有defense change信息
    // 2. 减少fChangeTime by fDeltaTime
    // 3. 如果时间到期,标记为删除
    // 4. 清理过期的效果
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::ApplyDefenseChangeInfo - Apply defense change calculations
 * 
 * IDA: ?ApplyDefenseChangeInfo@CMoverEx@@QEAAHXZ (0x14037D5B0)
 * Calculates and applies the total defense modification
 */
int CMoverEx::ApplyDefenseChangeInfo() {
    // IDA 反编译逻辑：
    // 1. 检查m_DefanseChangeFlag确定激活的防御类型
    // 2. 根据优先级选择防御效果
    // 3. 应用对应的防御修改
    // 4. 返回应用的防御类型
    
    // TODO: Implement based on IDA decompilation
    return 0;
}

/**
 * CMoverEx::AddOptionEffect - Add item option effect
 * 
 * IDA: ?AddOptionEffect@CMoverEx@@QEAAXKPEAUTB_CREATEOPTION@@MMMPEAV1@@Z (0x14039BFB0)
 * Adds an item option effect (stat bonuses, special effects)
 */
void CMoverEx::AddOptionEffect(std::uint32_t dwEquipedIndex, TB_CREATEOPTION* pOptionTable,
                                float fParam1, float fParam2, float fParam3, CMoverEx* pTargetMover) {
    // TODO: 完整实现需要以下依赖
    // - SOptionEffect 结构体定义
    // - m_vecOptionEffect 成员变量
    // - TB_CREATEOPTION 结构体定义
    // - CMoverEx::ApplyOptionEffectValue 应用效果值
    
    // IDA 反编译逻辑：
    // 1. 检查选项效果是否有效
    // 2. 创建SOptionEffect结构
    // 3. 计算效果值
    // 4. 如果需要目标,验证目标是否存在
    // 5. 添加到m_vecOptionEffect
    // 6. 调用ApplyOptionEffectValue应用效果
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::RemoveOptionEffect - Remove item option effect
 * 
 * IDA: ?RemoveOptionEffect@CMoverEx@@QEAAXK@Z (0x14039C420)
 */
void CMoverEx::RemoveOptionEffect(std::uint32_t dwEquipedIndex) {
    // IDA 反编译逻辑：
    // 1. 遍历m_vecOptionEffect
    // 2. 查找匹配dwEquipedIndex的效果
    // 3. 获取目标mover
    // 4. 调用ReleaseOptionEffectValue释放效果
    // 5. 从vector中删除
    // 6. 销毁SOptionEffect对象
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::RemoveAllOptionEffect - Remove all option effects
 * 
 * IDA: ?RemoveAllOptionEffect@CMoverEx@@QEAAXXZ (0x14039C620)
 */
void CMoverEx::RemoveAllOptionEffect() {
    // IDA 反编译逻辑：
    // 1. 遍历m_vecOptionEffect
    // 2. 对每个效果调用RemoveOptionEffect
    // 3. 清空vector
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::UpdateOptionEffect - Update option effects over time
 * 
 * IDA: ?UpdateOptionEffect@CMoverEx@@QEAAXM@Z (0x14039C1C0)
 */
void CMoverEx::UpdateOptionEffect(float fDeltaTime) {
    // IDA 反编译逻辑：
    // 1. 遍历所有option effects
    // 2. 更新持续时间
    // 3. 检查触发条件
    // 4. 清理过期效果
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::ApplyOptionEffectValue - Apply option effect value to stats
 * 
 * IDA: ?ApplyOptionEffectValue@CMoverEx@@QEAAXW4EFFECT_STATUS_TYPE@@M@Z (0x14039CAB0)
 */
void CMoverEx::ApplyOptionEffectValue(EFFECT_STATUS_TYPE eType, float fValue) {
    // IDA 反编译逻辑：
    // 1. 检查效果类型
    // 2. 根据类型修改对应的stat
    // 3. 调用CGocAttribute更新属性
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::ReleaseOptionEffectValue - Release option effect value from stats
 * 
 * IDA: ?ReleaseOptionEffectValue@CMoverEx@@QEAAXW4EFFECT_STATUS_TYPE@@M@Z (0x14039D890)
 */
void CMoverEx::ReleaseOptionEffectValue(EFFECT_STATUS_TYPE eType, float fValue) {
    // IDA 反编译逻辑：
    // 1. 检查效果类型
    // 2. 根据类型移除对应的stat修改
    // 3. 调用CGocAttribute更新属性
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::GetTotalOptionEffectValue - Get total value of an effect type
 * 
 * IDA: ?GetTotalOptionEffectValue@CMoverEx@@QEAAMW4EFFECT_STATUS_TYPE@@@Z (0x14039E060)
 */
float CMoverEx::GetTotalOptionEffectValue(EFFECT_STATUS_TYPE eType) {
    // IDA 反编译逻辑：
    // 1. 遍历所有option effects
    // 2. 累加匹配类型的值
    // 3. 返回总和
    
    // TODO: Implement based on IDA decompilation
    return 0.0f;
}

/**
 * CMoverEx::CheckOptionEffectInvoke - Check if option effect should be invoked
 * 
 * IDA: ?CheckOptionEffectInvoke@CMoverEx@@QEAAXW4EFFECT_CONDITION_TYPE@@PEAV1@MW4EFFECT_INVOKE_TYPE@@@Z (0x14039B670)
 */
void CMoverEx::CheckOptionEffectInvoke(EFFECT_CONDITION_TYPE eConditionType, CMoverEx* pMover,
                                        float fParam, EFFECT_INVOKE_TYPE eInvokeType) {
    // IDA 反编译逻辑：
    // 1. 检查条件类型
    // 2. 验证触发条件
    // 3. 如果条件满足,触发效果
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::ReleaseInvokedOptionEffect - Release invoked option effects
 * 
 * IDA: ?ReleaseInvokedOptionEffect@CMoverEx@@QEAAXW4EFFECT_CONDITION_TYPE@@@Z (0x14039BB70)
 */
void CMoverEx::ReleaseInvokedOptionEffect(EFFECT_CONDITION_TYPE eConditionType) {
    // IDA 反编译逻辑：
    // 1. 查找指定条件类型的效果
    // 2. 释放这些效果
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::ClearOptionEffect - Clear all option effects
 * 
 * IDA: ?ClearOptionEffect@CMoverEx@@QEAAXXZ (0x14039E990)
 */
void CMoverEx::ClearOptionEffect() {
    // IDA 反编译逻辑：
    // 1. 清空所有option effects
    // 2. 重置所有相关的stat修改
    
    // TODO: Implement based on IDA decompilation
}

// ============================================================================
// CGocBooster Status Effect Methods (Already implemented in GocBooster.cpp)
// ============================================================================

// Note: CGocBooster methods are already implemented in GocBooster.cpp
// The following are documented for reference:

/**
 * CGocBooster::AddBooster - Add a booster/buff effect
 * IDA: ?AddBooster@CGocBooster@@QEAAXG_N@Z (0x14004AA30)
 * Already implemented - see GocBooster.cpp
 */

/**
 * CGocBooster::RemoveBooster - Remove a booster/buff effect
 * IDA: ?RemoveBooster@CGocBooster@@QEAAXG@Z (0x14004AC20)
 * Already implemented - see GocBooster.cpp
 */

/**
 * CGocBooster::OnUpdate - Update booster timers
 * IDA: ?OnUpdate@CGocBooster@@QEAAXM@Z (0x140049CA0)
 * Already implemented - see GocBooster.cpp
 */

/**
 * CGocBooster::ApplyBoosterStat - Apply booster stats
 * IDA: ?ApplyBoosterStat@CGocBooster@@QEAAXPEAUTB_BOOSTER@@@Z (0x14004B040)
 * Already implemented - see GocBooster.cpp
 */

/**
 * CGocBooster::ClearBoosterStat - Clear booster stats
 * IDA: ?ClearBoosterStat@CGocBooster@@QEAAXPEAUTB_BOOSTER@@@Z (0x14004B0F0)
 * Already implemented - see GocBooster.cpp
 */

// ============================================================================
// CGocAttribute Status Effect Methods
// ============================================================================

/**
 * CGocAttribute::UpdateBuffEffectStat - Update buff effect on stats
 * 
 * IDA: ?UpdateBuffEffectStat@CGocAttribute@@QEAAHHM_N0@Z (0x14003B750)
 */
int CGocAttribute::UpdateBuffEffectStat(int nStatType, float fValue, bool bCalc, bool bUseInClear) {
    // TODO: 完整实现需要以下依赖
    // - CCalculateStatus::GetStatFromEffect 获取效果对应的状态
    // - CGocAttribute::GetRateTargetStat 获取百分比目标状态
    // - CGocAttribute::SetStat 设置状态值
    // - CGocAttribute::UpdateAddStat 更新加法状态
    // - CGocAttribute::UpdateScaleStat 更新百分比状态
    // - CGocAttribute::GetSpecialEffectIndex 获取特殊效果索引
    // - m_fItemSpecaillEffect 数组
    // - m_bItemSpecialEffectChanged 数组
    
    // IDA 反编译逻辑：
    // 1. 调用CCalculateStatus::GetStatFromEffect获取对应的状态类型和是否为加法
    // 2. 如果有对应的状态类型:
    //    a. 如果是基础属性(<=3或==16),调用SetStat
    //    b. 如果是加法类型,调用UpdateAddStat
    //    c. 否则调用UpdateScaleStat
    // 3. 如果没有对应状态,查找特殊效果索引并更新
    
    // TODO: Implement based on IDA decompilation
    return 0;
}

// ============================================================================
// Status Flag Methods (XActor/CUser)
// ============================================================================

/**
 * XActor::SetStatus - Set status flag
 * 
 * IDA: ?SetStatus@XActor@@QEAAXK@Z (0x140276490)
 */
void XActor::SetStatus(std::uint32_t dwStatus) {
    m_dwStatus |= dwStatus;
}

/**
 * XActor::ClearStatus - Clear status flag
 * 
 * IDA: ?ClearStatus@XActor@@QEAAXK@Z (0x1402764B0)
 */
void XActor::ClearStatus(std::uint32_t dwStatus) {
    m_dwStatus &= ~dwStatus;
}

/**
 * XActor::ResetStatus - Reset all status flags
 * 
 * IDA: ?ResetStatus@XActor@@QEAAXXZ (0x140280C20)
 */
void XActor::ResetStatus() {
    m_dwStatus = 0;
}

/**
 * XActor::IsStatus - Check status flag
 * 
 * IDA: ?IsStatus@XActor@@QEAAHK@Z (0x140048FD0)
 */
bool XActor::IsStatus(std::uint32_t dwStatus) const {
    return (m_dwStatus & dwStatus) != 0;
}

/**
 * CUser::IsStatus - Check user status flag
 * 
 * IDA: ?IsStatus@CUser@@QEAAHK@Z (0x140026C30)
 * Note: Same as XActor::IsStatus but for CUser class
 */
bool CUser::IsStatus(std::uint32_t dwStatus) const {
    return (m_dwStatus & dwStatus) != 0;
}

/**
 * CMover::SetImmunityStatus - Set immunity status
 * 
 * IDA: ?SetImmunityStatus@CMover@@QEAAXK@Z (0x1402A4F90)
 * Verified decompilation: this->m_dwImmunityStatus |= dwStatus
 */
void CMover::SetImmunityStatus(std::uint32_t dwStatus) {
    m_dwImmunityStatus |= dwStatus;
}

/**
 * CMover::ClearImmunityStatus - Clear immunity status
 * Related to SetImmunityStatus
 */
void CMover::ClearImmunityStatus(std::uint32_t dwStatus) {
    m_dwImmunityStatus &= ~dwStatus;
}

// ============================================================================
// CMover Buff Management Functions (IDA decompiled)
// ============================================================================

/**
 * CMover::FindBuffStatus - Find buff slot by index and attacker ID
 * 
 * IDA: ?FindBuffStatus@CMover@@QEAAHGK@Z (0x14036A420)
 * Returns slot index (0-49) or -1 if not found
 */
int CMover::FindBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwAttackerID) {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == nBuffIndex) {
            // Effect type 10 requires exact attacker ID match
            if (m_stBuffState[i].byEffectType != 10) {
                return i;
            }
            if (m_stBuffState[i].dwID == dwAttackerID) {
                return i;
            }
        }
    }
    return -1;
}

/**
 * CMover::FindBuffByGroupID - Find buff slot by group ID
 * 
 * IDA: ?FindBuffByGroupID@CMover@@QEAAHGK@Z (0x14036A4C0)
 * Returns slot index (0-49) or -1 if not found
 */
int CMover::FindBuffByGroupID(std::uint16_t nGroupID, std::uint32_t dwAttackerID) {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nGroupID == nGroupID) {
            // Effect type 10 requires exact attacker ID match
            if (m_stBuffState[i].byEffectType != 10) {
                return i;
            }
            if (m_stBuffState[i].dwID == dwAttackerID) {
                return i;
            }
        }
    }
    return -1;
}

/**
 * CMover::FindBuffByEffectType - Find buff slot by effect type
 * 
 * IDA: ?FindBuffByEffectType@CMover@@QEAAHEG@Z (0x14036A560)
 * Returns slot index (0-49) or -1 if not found
 */
int CMover::FindBuffByEffectType(std::uint8_t byBuffEffect, std::uint16_t nExceptBuffIndex) {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != nExceptBuffIndex &&
            m_stBuffState[i].nBuffIndex != 0 &&
            m_stBuffState[i].byEffectType == byBuffEffect) {
            return i;
        }
    }
    return -1;
}

/**
 * CMover::IsHaveImunityInvincibleBuff - Check for immunity/invincible buff
 * 
 * IDA: ?IsHaveImunityInvincibleBuff@CMover@@QEAAHG@Z (0x14036A600)
 * Returns true if immunity invincible buff exists (effect type 5, stat type 0 = 1)
 */
bool CMover::IsHaveImunityInvincibleBuff(std::uint16_t nExceptBuffIndex) {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != nExceptBuffIndex &&
            m_stBuffState[i].nBuffIndex != 0 &&
            m_stBuffState[i].byEffectType == 5 &&
            m_stBuffState[i].byStatType[0] == 1) {
            return true;
        }
    }
    return false;
}

/**
 * CMover::CheckPassDebuff - Check if debuff passes through
 * 
 * IDA: ?CheckPassDebuff@CMover@@QEAAHG@Z (0x14036A750)
 * Returns true if buff passes through
 */
bool CMover::CheckPassDebuff(std::uint16_t nBuffIndex) {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != 0 && m_stBuffState[i].byEffectType == 22) {
            for (int j = 0; j < 3; ++j) {
                if (static_cast<std::uint16_t>(static_cast<int>(m_stBuffState[i].fSkillVal[j])) == nBuffIndex) {
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * CMover::GetEmptyBuffSlot - Find empty buff slot
 * 
 * IDA: ?GetEmptyBuffSlot@CMover@@QEAAHXZ (0x14036A810)
 * Returns first empty slot index (0-49) or 0 if all full
 */
int CMover::GetEmptyBuffSlot() {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == 0) {
            return i;
        }
    }
    return 0;
}

/**
 * CMover::ResetAllBuff - Reset all buff states
 * 
 * IDA: ?ResetAllBuff@CMover@@QEAAXXZ (0x14036A860)
 */
void CMover::ResetAllBuff() {
    m_nBuffTotalCnt = 0;
    m_nBuffCnt = 0;
    m_nDebuffCnt = 0;
    for (int i = 0; i < 50; ++i) {
        m_stBuffState[i].Clear();
    }
}

/**
 * CMover::ClearBuffByType - Clear all buffs by type
 * 
 * IDA: ?ClearBuffByType@CMover@@QEAAXE@Z (0x14036A8E0)
 */
void CMover::ClearBuffByType(std::uint8_t byType) {
    if (m_nBuffTotalCnt > 0) {
        for (int i = 0; i < 50; ++i) {
            if (m_stBuffState[i].nBuffIndex != 0) {
                if (m_stBuffState[i].byBuffType == byType) {
                    ClearBuffStatusBySlot(i, false);
                }
            }
        }
    }
}

/**
 * CMover::UpdateBuffCount - Update buff count
 * 
 * IDA: ?UpdateBuffCount@CMover@@QEAAXEH@Z (0x14036AE70)
 */
void CMover::UpdateBuffCount(std::uint8_t byBuffType, int nAddCount) {
    if (nAddCount <= 0) {
        // Clamp to 0 minimum when decreasing
        int newTotal = m_nBuffTotalCnt + nAddCount;
        m_nBuffTotalCnt = (newTotal >= 0) ? newTotal : 0;
        
        if (byBuffType == 0) {
            // Buff
            int newBuff = m_nBuffCnt + nAddCount;
            m_nBuffCnt = (newBuff >= 0) ? newBuff : 0;
        } else if (byBuffType == 1) {
            // Debuff
            int newDebuff = m_nDebuffCnt + nAddCount;
            m_nDebuffCnt = (newDebuff >= 0) ? newDebuff : 0;
        }
    } else {
        // Simply add when positive
        m_nBuffTotalCnt += nAddCount;
        if (byBuffType == 0) {
            m_nBuffCnt += nAddCount;
        } else if (byBuffType == 1) {
            m_nDebuffCnt += nAddCount;
        }
    }
}

/**
 * CMover::GetBuffCategory - Get buff category by effect type
 * 
 * IDA: ?GetBuffCategory@CMover@@QEAAEE@Z (0x14036B000)
 * Returns: 1 for range 0x6F-0x72, 2 for range 0x79-0x7E, 3 for 0x83-0x84, 4 for 0x85
 */
std::uint8_t CMover::GetBuffCategory(std::uint8_t byBuffEffectType) {
    if (byBuffEffectType >= 0x6F && byBuffEffectType <= 0x72) {
        return 1;
    }
    if (byBuffEffectType >= 0x79 && byBuffEffectType <= 0x7E) {
        return 2;
    }
    if (byBuffEffectType >= 0x83 && byBuffEffectType <= 0x84) {
        return 3;
    }
    if (byBuffEffectType == 0x85) {
        return 4;
    }
    return 0;
}

/**
 * CMover::GetResistStatIndexByBuff - Get resistance stat index for buff
 * 
 * IDA: ?GetResistStatIndexByBuff@CMover@@QEAAHE@Z (0x14036B070)
 * Returns stat index or -1 if not applicable
 */
int CMover::GetResistStatIndexByBuff(std::uint8_t byBuffEffectType) {
    if (byBuffEffectType >= 0x6F && byBuffEffectType <= 0x72) {
        return byBuffEffectType - 62;
    }
    if (byBuffEffectType >= 0x79 && byBuffEffectType <= 0x7E) {
        return byBuffEffectType - 68;
    }
    if (byBuffEffectType >= 0x83 && byBuffEffectType <= 0x84) {
        return byBuffEffectType - 72;
    }
    return -1;
}

/**
 * CMover::SetBuffTime - Set buff duration and count
 * 
 * IDA: ?SetBuffTime@CMover@@QEAAXFME@Z (0x14036B0F0)
 */
void CMover::SetBuffTime(std::int16_t nIndex, float fTime, std::uint8_t byCount) {
    m_stBuffState[nIndex].fLifeTime = fTime;
    m_stBuffState[nIndex].byCount = byCount;
    send_eSUB_CMD_BUFF_UPDATE(
        this,
        m_stBuffState[nIndex].nBuffIndex,
        fTime,
        byCount,
        m_stBuffState[nIndex].dwID,
        m_stBuffState[nIndex].bySendType,
        m_stBuffState[nIndex].bShow);
}

/**
 * CMover::UpdateDefenseDisableBuff - Update defense disable buff flag
 * 
 * IDA: ?UpdateDefenseDisableBuff@CMover@@QEAAXXZ (0x14036B420)
 */
void CMover::UpdateDefenseDisableBuff() {
    std::uint8_t byOldVal = m_byDefenseTypeDisableFlag;
    m_byDefenseTypeDisableFlag = 0;
    
    if (m_nBuffTotalCnt > 0) {
        for (int i = 0; i < 50; ++i) {
            if (m_stBuffState[i].IsLife()) {
                if (m_stBuffState[i].byEffectType == 6) {
                    m_byDefenseTypeDisableFlag |= m_stBuffState[i].byStatType[0];
                    if (m_byDefenseTypeDisableFlag == 19) {
                        break;
                    }
                }
            }
        }
    }
    
    if (m_byDefenseTypeDisableFlag != byOldVal) {
        UpdateDefenseType();
    }
}

/**
 * CMover::GetBuffStatus - Get buff status array
 * 
 * IDA: ?GetBuffStatus@CMover@@QEAAPEAUtagBUFF_STATE@@XZ (0x1403539C0)
 */
tagBUFF_STATE* CMover::GetBuffStatus() {
    return m_stBuffState;
}

/**
 * tagBUFF_STATE::IsLife - Check if buff is alive (active)
 * 
 * IDA: ?IsLife@tagBUFF_STATE@@QEAAHXZ (0x140377510)
 * Note: Already implemented in BuffState.h, documenting here for reference
 */

// ============================================================================
// Specialized Status Effect Calculations
// ============================================================================

/**
 * CCalculateStatus::CALCULATE_STAT_RES_STUN - Calculate stun resistance
 * 
 * IDA: ?CALCULATE_STAT_RES_STUN@CCalculateStatus@@QEAAMPEAVCGocAttribute@@@Z (0x1402D8110)
 */
float CCalculateStatus::CALCULATE_STAT_RES_STUN(CGocAttribute* pAttr) {
    // TODO: Implement based on IDA decompilation
    // Calculate stun resistance from attributes
    return 0.0f;
}

/**
 * CCalculateStatus::CALCULATE_STAT_RES_POISON - Calculate poison resistance
 * 
 * IDA: ?CALCULATE_STAT_RES_POISON@CCalculateStatus@@QEAAMPEAVCGocAttribute@@@Z (0x1402D8020)
 */
float CCalculateStatus::CALCULATE_STAT_RES_POISON(CGocAttribute* pAttr) {
    // TODO: Implement based on IDA decompilation
    // Calculate poison resistance from attributes
    return 0.0f;
}

/**
 * CMover::SetSlowTime - Set slow effect timer
 * 
 * IDA: ?SetSlowTime@CMover@@QEAAXMM@Z (0x140368AA0)
 */
void CMover::SetSlowTime(float fSlowRate, float fDuration) {
    // TODO: 完整实现需要以下依赖
    // - tagTIME_SLOW 结构体定义
    // - m_stTimeSlow 成员变量
    
    // IDA 反编译逻辑：
    // 1. 设置m_stTimeSlow.fSlowRate = fSlowRate
    // 2. 设置m_stTimeSlow.fRemainTime = fDuration
    // 3. 如果fDuration > 0,激活slow状态
    // 4. 否则清除slow状态
    
    // TODO: Implement based on IDA decompilation
}

// ============================================================================
// Status Effect Trigger/Invoke Functions
// ============================================================================

/**
 * CMoverEx::ReleaseInvokedOptionEffectAfterDamage - Release effects after damage
 * 
 * IDA: ?ReleaseInvokedOptionEffectAfterDamage@CMoverEx@@QEAAXKE_N@Z (0x14039C780)
 */
void CMoverEx::ReleaseInvokedOptionEffectAfterDamage(std::uint32_t dwSkillID, bool bFlag) {
    // IDA 反编译逻辑：
    // 1. 检查受到伤害后触发的效果
    // 2. 根据技能ID和标志释放对应效果
    
    // TODO: Implement based on IDA decompilation
}

/**
 * CMoverEx::IsCanOptionEffectExcute - Check if option effect can execute
 * 
 * IDA: ?IsCanOptionEffectExcute@CMoverEx@@QEAA_NPEAUTB_CREATEOPTION@@PEAV1@M@Z (0x14039B010)
 */
bool CMoverEx::IsCanOptionEffectExcute(TB_CREATEOPTION* pOptionTable, CMoverEx* pMover, float fParam) {
    // IDA 反编译逻辑：
    // 1. 检查选项效果的执行条件
    // 2. 验证目标状态
    // 3. 返回是否可以执行
    
    // TODO: Implement based on IDA decompilation
    return false;
}

/**
 * CMoverEx::GetOptionEffectValue - Calculate option effect value
 * 
 * IDA: ?GetOptionEffectValue@CMoverEx@@QEAAMPEAUTB_CREATEOPTION@@PEAV1@MM@Z (0x14039B120)
 */
float CMoverEx::GetOptionEffectValue(TB_CREATEOPTION* pOptionTable, CMoverEx* pMover, 
                                      float fParam1, float fParam2) {
    // IDA 反编译逻辑：
    // 1. 根据选项表和参数计算效果值
    // 2. 返回计算结果
    
    // TODO: Implement based on IDA decompilation
    return 0.0f;
}

/**
 * CMoverEx::GetSpecificOptionEffectValue - Get specific effect value
 * 
 * IDA: ?GetSpecificOptionEffectValue@CMoverEx@@QEAAMKW4EFFECT_STATUS_TYPE@@@Z (0x14039E120)
 */
float CMoverEx::GetSpecificOptionEffectValue(std::uint32_t dwIndex, EFFECT_STATUS_TYPE eType) {
    // IDA 反编译逻辑：
    // 1. 查找指定索引的选项效果
    // 2. 返回该效果的值
    
    // TODO: Implement based on IDA decompilation
    return 0.0f;
}

/**
 * CMoverEx::IsStatOptionEffect - Check if effect is a stat effect
 * 
 * IDA: ?IsStatOptionEffect@CMoverEx@@QEAA_NW4EFFECT_STATUS_TYPE@@@Z (0x14039B0A0)
 */
bool CMoverEx::IsStatOptionEffect(EFFECT_STATUS_TYPE eType) {
    // IDA 反编译逻辑：
    // 检查效果类型是否影响属性值
    
    // TODO: Implement based on IDA decompilation
    return false;
}

// ============================================================================
// Link Skill Duration (Special Status Effect)
// ============================================================================

/**
 * CMoverEx::GetLinkSkillDuration - Get link skill duration
 * 
 * IDA: ?GetLinkSkillDuration@CMoverEx@@QEAAMXZ (0x1403A27F0)
 */
float CMoverEx::GetLinkSkillDuration() {
    // IDA 反编译逻辑：
    // 返回链接技能的持续时间
    
    // TODO: Implement based on IDA decompilation
    return 0.0f;
}
