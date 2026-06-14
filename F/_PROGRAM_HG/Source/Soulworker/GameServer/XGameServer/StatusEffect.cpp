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
#include "GameServer.h"
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
            auto pServer = TXSingleton<XGameServer>::Instance();
            if (!pServer) {
                return false;
            }
            
            TB_CREATEOPTION* pOptionTable = pServer->GetResourceMgr().GetTB_CREATEOPTION(wStatusID);
            if (!pOptionTable) {
                return false;
            }
            
            // Apply option effect to self
            pMover->ApplyOptionEffectValue(static_cast<EFFECT_STATUS_TYPE>(pOptionTable->StatusType), fValue);
            
            // Add to option effect list
            pMover->AddOptionEffect(0, pOptionTable, fValue, 0.0f, fValue, pMover);
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
            pMover->AddDefenseChangeInfo(1, static_cast<std::uint8_t>(wStatusID), wStatusID, fDuration);
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
            pMover->RemoveDefenseChangeInfo(1, wStatusID);
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
            SOptionEffect* pEffect = pMover->GetOptionEffect(0, wStatusID, nullptr);
            return pEffect != nullptr;
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
            // IDA: Check m_DefanseChangeFlag and defense change structures
            // Check if any defense change info matches
            for (auto* pInfo : pMover->m_listDefenseChangeInfo) {
                if (pInfo && pInfo->dwID == wStatusID) {
                    return true;
                }
            }
            // Check trigger and effect structures
            if (pMover->m_stDefenseChangeInfoByTrigger.dwID == wStatusID &&
                pMover->m_stDefenseChangeInfoByTrigger.fChangeTime > 0.0f) {
                return true;
            }
            if (pMover->m_stDefenseChangeInfoByEffect.dwID == wStatusID &&
                pMover->m_stDefenseChangeInfoByEffect.fChangeTime > 0.0f) {
                return true;
            }
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
            // IDA: CMoverEx::GetOptionEffect (0x14039BE80)
            SOptionEffect* pEffect = pMover->GetOptionEffect(0, wStatusID, nullptr);
            if (pEffect) {
                // Check if lifetime is permanent (< 1000ms means permanent)
                if (pEffect->dwLifeTime < 1000) {
                    return -1.0f;  // Permanent
                }
                // Calculate remaining time
                float fLifeSeconds = static_cast<float>(pEffect->dwLifeTime) / 1000.0f;
                float fRemaining = fLifeSeconds - pEffect->fCurTime;
                return (fRemaining > 0.0f) ? fRemaining : 0.0f;
            }
            return 0.0f;
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
            // IDA: Check defense change structures
            for (auto* pInfo : pMover->m_listDefenseChangeInfo) {
                if (pInfo && pInfo->dwID == wStatusID) {
                    return pInfo->fChangeTime;
                }
            }
            if (pMover->m_stDefenseChangeInfoByTrigger.dwID == wStatusID) {
                return pMover->m_stDefenseChangeInfoByTrigger.fChangeTime;
            }
            if (pMover->m_stDefenseChangeInfoByEffect.dwID == wStatusID) {
                return pMover->m_stDefenseChangeInfoByEffect.fChangeTime;
            }
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

// Note: CMoverEx::AddDefenseChangeInfo implemented at line 1465 (int return version)

/**
 * CMoverEx::RemoveDefenseChangeInfo - Remove defense change status effect
 * 
 * IDA: ?RemoveDefenseChangeInfo@CMoverEx@@QEAAHEK@Z (0x14037D100)
 * Removes a defense modification effect
 */
int CMoverEx::RemoveDefenseChangeInfo(std::uint8_t byType, std::uint32_t dwID) {
    // IDA 精确还原:
    if (byType == 1) {
        // 从m_listDefenseChangeInfo列表中删除
        for (auto it = m_listDefenseChangeInfo.begin(); it != m_listDefenseChangeInfo.end(); ++it) {
            SDefenseChangeInfo* pInfo = *it;
            if (pInfo->dwID == dwID) {
                delete pInfo;
                m_listDefenseChangeInfo.erase(it);
                break;
            }
        }
    } else if (byType == 2) {
        // 从Effect结构中删除
        if (m_stDefenseChangeInfoByEffect.byDefenseType == 3) {
            CMover::SetInvincibleActor(false);
        }
        m_stDefenseChangeInfoByEffect.Clear();
    } else {
        // 从Trigger结构中删除 (byType == 0)
        m_stDefenseChangeInfoByTrigger.Clear();
        CMover::ClearImmunityStatus(2);
    }

    // 重新计算m_DefanseChangeFlag
    m_DefanseChangeFlag = 0;
    
    if (m_stDefenseChangeInfoByTrigger.fChangeTime > 0.0f) {
        m_DefanseChangeFlag |= (1 << m_stDefenseChangeInfoByTrigger.byDefenseType);
    }
    
    for (auto* pInfo : m_listDefenseChangeInfo) {
        m_DefanseChangeFlag |= (1 << pInfo->byDefenseType);
    }
    
    if (m_stDefenseChangeInfoByEffect.fChangeTime > 0.0f) {
        m_DefanseChangeFlag |= (1 << m_stDefenseChangeInfoByEffect.byDefenseType);
    }

    return ApplyDefenseChangeInfo();
}

/**
 * CMoverEx::RemoveAllDefenseChangeInfo - Remove all defense change effects
 * 
 * IDA: ?RemoveAllDefenseChangeInfo@CMoverEx@@QEAAXXZ (0x14037D420)
 */
void CMoverEx::RemoveAllDefenseChangeInfo() {
    // IDA 精确还原:
    // 1. 清空Trigger结构并清除immunity状态
    m_stDefenseChangeInfoByTrigger.Clear();
    CMover::ClearImmunityStatus(2);
    
    // 2. 删除列表中的所有DefenseChangeInfo
    for (auto it = m_listDefenseChangeInfo.begin(); it != m_listDefenseChangeInfo.end(); ) {
        SDefenseChangeInfo* pInfo = *it;
        if (pInfo) {
            delete pInfo;
        }
        it = m_listDefenseChangeInfo.erase(it);
    }
    
    // 3. 清空Effect结构
    if (m_stDefenseChangeInfoByEffect.byDefenseType == 3) {
        CMover::SetInvincibleActor(false);
    }
    m_stDefenseChangeInfoByEffect.Clear();
    
    // 4. 重置flag
    m_DefanseChangeFlag = 0;
}

// Note: CMoverEx::UppdateDefenseChangeInfo implemented at line 1491

// Note: CMoverEx::ApplyDefenseChangeInfo implemented at line 1505 (bool return version)

// Note: CMoverEx::AddOptionEffect implemented at line 1543

// Note: CMoverEx::RemoveOptionEffect implemented at line 1570

/**
 * CMoverEx::RemoveAllOptionEffect - Remove all option effects
 * 
 * IDA: ?RemoveAllOptionEffect@CMoverEx@@QEAAXXZ (0x14039C620)
 */
void CMoverEx::RemoveAllOptionEffect() {
    // IDA 精确还原:
    for (auto it = m_vecOptionEffect.begin(); it != m_vecOptionEffect.end(); ) {
        SOptionEffect* pOptionEffect = *it;
        CMoverEx* pTargetMoverEx = static_cast<CMoverEx*>(CMover::GetMoverObject(pOptionEffect->dwTargetMoverID));
        
        if (pTargetMoverEx && pOptionEffect->pOptionTable) {
            ReleaseOptionEffectValue(pTargetMoverEx, 
                                    static_cast<EFFECT_STATUS_TYPE>(pOptionEffect->pOptionTable->StatusType),
                                    pOptionEffect->fAppliedValue);
        }
        
        if (pOptionEffect) {
            delete pOptionEffect;
        }
        
        it = m_vecOptionEffect.erase(it);
    }
    
    m_vecOptionEffect.clear();
}

/**
 * CMoverEx::UpdateOptionEffect - Update option effects over time
 * 
 * IDA: ?UpdateOptionEffect@CMoverEx@@QEAAXM@Z (0x14039C1C0)
 * IDA decompiled: Updates timers and removes expired effects
 */
void CMoverEx::UpdateOptionEffect(float fDeltaTime) {
    for (auto it = m_vecOptionEffect.begin(); it != m_vecOptionEffect.end(); ) {
        SOptionEffect* pOptionEffect = *it;
        CMoverEx* pTargetMoverEx = static_cast<CMoverEx*>(CMover::GetMoverObject(pOptionEffect->dwTargetMoverID));
        
        if (pTargetMoverEx && pOptionEffect->pOptionTable) {
            // Check if lifetime is permanent (>= 1000ms means timed)
            if (pOptionEffect->dwLifeTime < 1000) {
                ++it;
                continue;
            }
            
            pOptionEffect->fCurTime += fDeltaTime;
            float fLifeSeconds = static_cast<float>(pOptionEffect->dwLifeTime) / 1000.0f;
            
            if (pOptionEffect->fCurTime < fLifeSeconds) {
                ++it;
                continue;
            }
            
            // Effect expired - release and remove
            ReleaseOptionEffectValue(
                static_cast<EFFECT_STATUS_TYPE>(pOptionEffect->pOptionTable->StatusType),
                pOptionEffect->fAppliedValue);
            
            delete pOptionEffect;
            it = m_vecOptionEffect.erase(it);
        } else {
            // Invalid target or table - just remove
            delete pOptionEffect;
            it = m_vecOptionEffect.erase(it);
        }
    }
}

/**
 * CMoverEx::ApplyOptionEffectValue - Apply option effect value to stats
 * 
 * IDA: ?ApplyOptionEffectValue@CMoverEx@@QEAAXW4EFFECT_STATUS_TYPE@@M@Z (0x14039CAB0)
 * IDA decompiled: Applies effect value to appropriate stat based on type
 */
void CMoverEx::ApplyOptionEffectValue(EFFECT_STATUS_TYPE eType, float fValue) {
    auto pAttr = GetGOC<CGocAttribute>();
    if (!pAttr) {
        return;
    }
    
    // Status type to stat type mapping for user-only stats
    static const int iStatusList[] = {
        EFFECT_STATUS_PATK, EFFECT_STATUS_PDEF, EFFECT_STATUS_REG_ST,
        EFFECT_STATUS_MSR, EFFECT_STATUS_ASR, EFFECT_STATUS_PAR,
        EFFECT_STATUS_ADR, EFFECT_STATUS_PCP, EFFECT_STATUS_PCRP,
        EFFECT_STATUS_PDSR, EFFECT_STATUS_PARP, EFFECT_STATUS_PDPR,
        EFFECT_STATUS_PCA
    };
    static const int iStatList[] = {21, 24, 15, 18, 19, 26, 29, 31, 38, 43, 47, 35, 28};
    
    // Max stat type mapping
    static const int v39[] = {EFFECT_STATUS_MAX_HP, EFFECT_STATUS_MAX_SG, EFFECT_STATUS_MAX_ST, EFFECT_STATUS_MAX_SV};
    static const int nStat[] = {10, 12, 14, 17};
    static const int iStatList2[] = {1, 2, 3, 16};
    
    switch (eType) {
        case EFFECT_STATUS_PATK:
        case EFFECT_STATUS_PDEF:
        case EFFECT_STATUS_REG_ST:
        case EFFECT_STATUS_MSR:
        case EFFECT_STATUS_ASR:
        case EFFECT_STATUS_PAR:
        case EFFECT_STATUS_ADR:
        case EFFECT_STATUS_PCP:
        case EFFECT_STATUS_PCRP:
        case EFFECT_STATUS_PDSR:
        case EFFECT_STATUS_PARP:
        case EFFECT_STATUS_PDPR:
        case EFFECT_STATUS_PCA: {
            // User-only stats
            CUser* pUser = dynamic_cast<CUser*>(this);
            if (pUser) {
                int iIndex = -1;
                for (int i = 0; i < 13; ++i) {
                    if (iStatusList[i] == eType) {
                        iIndex = i;
                        break;
                    }
                }
                if (iIndex >= 0) {
                    SYNC_STAT_TYPE eSyncStatType = SYNC_STAT_TYPE_ME_ONLY;
                    if (pAttr->IsShouldSyncStatBroadcast(iStatList[iIndex])) {
                        eSyncStatType = SYNC_STAT_TYPE_BROADCAST;
                    }
                    
                    if (iStatList[iIndex] == 18 || iStatList[iIndex] == 19) {
                        pAttr->UpdateScaleStat(iStatList[iIndex], fValue, true);
                    } else {
                        pAttr->UpdateAddStat(iStatList[iIndex], fValue, true);
                    }
                    pAttr->SetSyncStatFlag(iStatList[iIndex], eSyncStatType);
                    SendUpdateStat(iStatList[iIndex]);
                }
            }
            break;
        }
        case EFFECT_STATUS_MAX_HP:
        case EFFECT_STATUS_MAX_SG:
        case EFFECT_STATUS_MAX_ST:
        case EFFECT_STATUS_MAX_SV: {
            int v41 = -1;
            for (int j = 0; j < 4; ++j) {
                if (v39[j] == eType) {
                    v41 = j;
                    break;
                }
            }
            if (v41 >= 0) {
                pAttr->UpdateAddStat(nStat[v41], fValue, true);
                pAttr->SetSyncStatFlag(nStat[v41], SYNC_STAT_TYPE_BROADCAST);
                SendUpdateStat(nStat[v41]);
                
                float fMax = GetStat(nStat[v41]);
                float fCur = GetStat(iStatList2[v41]);
                if (fCur > fMax) {
                    SetStat(iStatList2[v41], fMax);
                    pAttr->SetSyncStatFlag(iStatList2[v41], SYNC_STAT_TYPE_ME_ONLY);
                    SendUpdateStat(iStatList2[v41]);
                }
            }
            break;
        }
        case EFFECT_STATUS_SA_DEFENCE:
            AddDefenseChangeInfo(2, 2, 0, 0.0f);
            send_eSUB_CMD_SKILL_DEFENCE_TYPE(this, 2, true);
            break;
        case EFFECT_STATUS_REFLECTION: {
            if (GetDefenseType() != 3) {
                float fCurHP = GetStat(1);
                float fDamage = fValue;
                if (fCurHP - fDamage < 0.0f) {
                    fDamage = fCurHP - 1.0f;
                }
                SetHP(static_cast<int>(fCurHP - fDamage));
                pAttr->SetSyncStatFlag(1, SYNC_STAT_TYPE_BROADCAST);
                SendUpdateStat(1);
            }
            break;
        }
        case EFFECT_STATUS_COOLTIME:
            if (m_pSkillMgr) {
                m_pSkillMgr->ReduceSkillCooltime(fValue);
                send_eSUB_CMD_SKILL_COOLTIME_REDUCE(this, fValue);
            }
            break;
        case EFFECT_STATUS_CUR_HP:
            if (!IsDie()) {
                float fCurHP = GetStat(1);
                SetStat(1, fCurHP + fValue);
                pAttr->SetSyncStatFlag(1, SYNC_STAT_TYPE_BROADCAST);
                SendUpdateStat(1);
            }
            break;
        case EFFECT_STATUS_CUR_SG: {
            float fCurSG = GetStat(2);
            SetStat(2, fCurSG + fValue);
            pAttr->SetSyncStatFlag(2, SYNC_STAT_TYPE_BROADCAST);
            SendUpdateStat(1);
            break;
        }
        case EFFECT_STATUS_INVINCIBLE:
            AddDefenseChangeInfo(2, 3, 0, 0.0f);
            send_eSUB_CMD_SKILL_DEFENCE_TYPE(this, 3, true);
            break;
        case EFFECT_STATUS_EXP:
            m_nAddExpFromOptionEffect += static_cast<int>(fValue);
            break;
        case EFFECT_STATUS_GOLD:
            m_nAddMoneyFromOptionEffect += static_cast<int>(fValue);
            break;
        case EFFECT_STATUS_ETHER:
            m_nAddEtherFromOptionEffect += static_cast<int>(fValue);
            break;
        case EFFECT_STATUS_FATIGUE: {
            CUser* pUser = dynamic_cast<CUser*>(this);
            if (pUser) {
                pUser->AddBonusFP(static_cast<int>(fValue));
                pAttr->SendDBUpdateFP();
            }
            break;
        }
        case EFFECT_STATUS_CUR_SV: {
            float fCurSV = GetStat(16);
            SetStat(16, fCurSV + fValue);
            pAttr->SetSyncStatFlag(16, SYNC_STAT_TYPE_ME_ONLY);
            SendUpdateStat(16);
            break;
        }
        case EFFECT_STATUS_REVIVAL: {
            CUser* pUser = dynamic_cast<CUser*>(this);
            if (pUser) {
                pUser->SetReserveReviveImmediate(true);
            }
            break;
        }
        case EFFECT_STATUS_CUR_ST: {
            float fCurST = GetStat(3);
            SetStat(3, fCurST + fValue);
            pAttr->SetSyncStatFlag(3, SYNC_STAT_TYPE_ME_ONLY);
            SendUpdateStat(3);
            break;
        }
        case EFFECT_STATUS_REMOVE_DEBUFF:
            for (int iType = 111; iType <= 132; ++iType) {
                int iTempIndex = FindBuffByEffectType(iType, 0);
                if (iTempIndex != -1) {
                    ClearBuffStatusBySlot(iTempIndex, false);
                }
            }
            break;
        case EFFECT_STATUS_SA_ATTACK_INCREASE:
            pAttr->UpdateEffectStat(0, 0x71, fValue, true);
            pAttr->CalculateChangedEffect(true, fValue);
            break;
        default:
            break;
    }
}

/**
 * CMoverEx::ReleaseOptionEffectValue - Release option effect value from stats
 * 
 * IDA: ?ReleaseOptionEffectValue@CMoverEx@@QEAAXW4EFFECT_STATUS_TYPE@@M@Z (0x14039D890)
 * IDA decompiled: Removes effect value from stats (negates the applied value)
 */
void CMoverEx::ReleaseOptionEffectValue(EFFECT_STATUS_TYPE eType, float fValue) {
    auto pAttr = GetGOC<CGocAttribute>();
    if (!pAttr) {
        return;
    }
    
    // Status type to stat type mapping for user-only stats
    static const int iStatusList[] = {
        EFFECT_STATUS_PATK, EFFECT_STATUS_PDEF, EFFECT_STATUS_REG_ST,
        EFFECT_STATUS_MSR, EFFECT_STATUS_ASR, EFFECT_STATUS_PAR,
        EFFECT_STATUS_ADR, EFFECT_STATUS_PCP, EFFECT_STATUS_PCRP,
        EFFECT_STATUS_PDSR, EFFECT_STATUS_PARP, EFFECT_STATUS_PDPR,
        EFFECT_STATUS_PCA
    };
    static const int iStatList[] = {21, 24, 15, 18, 19, 26, 29, 31, 38, 43, 47, 35, 28};
    
    // Max stat type mapping
    static const int v22[] = {EFFECT_STATUS_MAX_HP, EFFECT_STATUS_MAX_SG, EFFECT_STATUS_MAX_ST, EFFECT_STATUS_MAX_SV};
    static const int iStatIndex[] = {10, 12, 14, 17};
    static const int iStatList2[] = {1, 2, 3, 16};
    
    switch (eType) {
        case EFFECT_STATUS_PATK:
        case EFFECT_STATUS_PDEF:
        case EFFECT_STATUS_REG_ST:
        case EFFECT_STATUS_MSR:
        case EFFECT_STATUS_ASR:
        case EFFECT_STATUS_PAR:
        case EFFECT_STATUS_ADR:
        case EFFECT_STATUS_PCP:
        case EFFECT_STATUS_PCRP:
        case EFFECT_STATUS_PDSR:
        case EFFECT_STATUS_PARP:
        case EFFECT_STATUS_PDPR:
        case EFFECT_STATUS_PCA: {
            int iIndex = -1;
            for (int i = 0; i < 13; ++i) {
                if (iStatusList[i] == eType) {
                    iIndex = i;
                    break;
                }
            }
            if (iIndex >= 0) {
                SYNC_STAT_TYPE eSyncStatType = SYNC_STAT_TYPE_ME_ONLY;
                if (pAttr->IsShouldSyncStatBroadcast(iStatList[iIndex])) {
                    eSyncStatType = SYNC_STAT_TYPE_BROADCAST;
                }
                
                float fNegValue = -fValue;
                if (iStatList[iIndex] == 18 || iStatList[iIndex] == 19) {
                    pAttr->UpdateScaleStat(iStatList[iIndex], fNegValue, true);
                } else {
                    pAttr->UpdateAddStat(iStatList[iIndex], fNegValue, true);
                }
                pAttr->SetSyncStatFlag(iStatList[iIndex], eSyncStatType);
                SendUpdateStat(iStatList[iIndex]);
            }
            break;
        }
        case EFFECT_STATUS_MAX_HP:
        case EFFECT_STATUS_MAX_SG:
        case EFFECT_STATUS_MAX_ST:
        case EFFECT_STATUS_MAX_SV: {
            int v24 = -1;
            for (int j = 0; j < 4; ++j) {
                if (v22[j] == eType) {
                    v24 = j;
                    break;
                }
            }
            if (v24 >= 0) {
                SYNC_STAT_TYPE eSyncStatType = SYNC_STAT_TYPE_ME_ONLY;
                if (pAttr->IsShouldSyncStatBroadcast(iStatIndex[v24])) {
                    eSyncStatType = SYNC_STAT_TYPE_BROADCAST;
                }
                
                float fCur = GetStat(iStatList2[v24]);
                float fNegValue = -fValue;
                pAttr->UpdateAddStat(iStatIndex[v24], fNegValue, true);
                pAttr->SetSyncStatFlag(iStatIndex[v24], eSyncStatType);
                SendUpdateStat(iStatIndex[v24]);
                
                float fMax = GetStat(iStatIndex[v24]);
                if (fCur != fMax) {
                    eSyncStatType = SYNC_STAT_TYPE_ME_ONLY;
                    if (pAttr->IsShouldSyncStatBroadcast(iStatList2[v24])) {
                        eSyncStatType = SYNC_STAT_TYPE_BROADCAST;
                    }
                    pAttr->SetSyncStatFlag(iStatList2[v24], eSyncStatType);
                    SendUpdateStat(iStatList2[v24]);
                }
            }
            break;
        }
        case EFFECT_STATUS_SA_DEFENCE:
        case EFFECT_STATUS_INVINCIBLE:
            RemoveDefenseChangeInfo(2, 0);
            send_eSUB_CMD_SKILL_DEFENCE_TYPE(this, m_byDefenseType, false);
            break;
        case EFFECT_STATUS_SA_ATTACK_INCREASE: {
            float fNegValue = -fValue;
            pAttr->UpdateEffectStat(0, 0x71, fNegValue, true);
            pAttr->CalculateChangedEffect(true, fValue);
            break;
        }
        default:
            break;
    }
}

/**
 * CMoverEx::GetTotalOptionEffectValue - Get total value of an effect type
 * 
 * IDA: ?GetTotalOptionEffectValue@CMoverEx@@QEAAMW4EFFECT_STATUS_TYPE@@@Z (0x14039E060)
 * IDA decompiled: Sums all applied values for matching effect type
 */
float CMoverEx::GetTotalOptionEffectValue(EFFECT_STATUS_TYPE eType) {
    float fTotalAppliedValue = 0.0f;
    
    for (auto it = m_vecOptionEffect.begin(); it != m_vecOptionEffect.end(); ++it) {
        SOptionEffect* pOptionEffect = *it;
        if (pOptionEffect && pOptionEffect->pOptionTable) {
            if (pOptionEffect->pOptionTable->StatusType == eType) {
                fTotalAppliedValue += pOptionEffect->fAppliedValue;
            }
        }
    }
    
    return fTotalAppliedValue;
}

/**
 * CMoverEx::CheckOptionEffectInvoke - Check if option effect should be invoked
 * 
 * IDA: ?CheckOptionEffectInvoke@CMoverEx@@QEAAXW4EFFECT_CONDITION_TYPE@@PEAV1@MW4EFFECT_INVOKE_TYPE@@@Z (0x14039B670)
 * IDA decompiled: Checks and invokes option effects based on conditions
 */
void CMoverEx::CheckOptionEffectInvoke(EFFECT_CONDITION_TYPE eConditionType, CMoverEx* pMover,
                                        float fParam, EFFECT_INVOKE_TYPE eInvokeType) {
    auto pAttr = GetGOC<CGocAttribute>();
    if (!pAttr) {
        return;
    }
    
    // Scan area for game objects
    std::vector<CMover*> vecGameObjList;
    XArea::ScanGridOrigin(this, 2, 3, &vecGameObjList);
    
    // Reset effect counters
    m_nAddMoneyFromOptionEffect = 0;
    m_nAddEtherFromOptionEffect = 0;
    m_nAddExpFromOptionEffect = 0;
    
    // Get equipped options
    const auto& vecEquipedOption = pAttr->GetEquipedOption();
    
    for (auto it = vecEquipedOption.begin(); it != vecEquipedOption.end(); ++it) {
        SEquipedOption* pEquipedOption = *it;
        
        // Get option table
        auto pServer = TXSingleton<XGameServer>::Instance();
        TB_CREATEOPTION* pOptionTable = pServer->GetResourceMgr().GetTB_CREATEOPTION(pEquipedOption->dwOptionID);
        
        if (!pOptionTable) {
            continue;
        }
        
        // Check invoke area
        if (!IsOptionInvokeArea(static_cast<EFFECT_INVOKE_AREA_TYPE>(pOptionTable->Invoke_Area))) {
            continue;
        }
        
        // Determine target
        CMoverEx* pRealTargetMover = pMover;
        if (pOptionTable->Apply_Target == 1) {
            pRealTargetMover = this;
        }
        
        // Check target
        if (!IsOptionTarget(static_cast<EFFECT_TARGET_TYPE>(pOptionTable->Invoke_Target), pMover)) {
            continue;
        }
        
        // Check if effect already exists
        SOptionEffect* pAppliedOptionEffect = GetOptionEffect(
            pEquipedOption->dwIndex,
            pEquipedOption->dwOptionID,
            pRealTargetMover);
        
        // Skip if already equipped and condition is EQUIP
        if (eConditionType == EFFECT_CONDITION_EQUIP && pAppliedOptionEffect) {
            continue;
        }
        
        // Check condition
        if (pOptionTable->Invoke_Condition != eConditionType) {
            continue;
        }
        
        // Check invoke type
        if (eInvokeType == EFFECT_INVOKE_STAT) {
            if (!IsStatOptionEffect(static_cast<EFFECT_STATUS_TYPE>(pOptionTable->StatusType))) {
                continue;
            }
        } else if (eInvokeType == EFFECT_INVOKE_NONE_STAT) {
            if (IsStatOptionEffect(static_cast<EFFECT_STATUS_TYPE>(pOptionTable->StatusType))) {
                continue;
            }
        }
        
        // Check chance
        if (pOptionTable->Chance_Clm != 0) {
            if (rand() % 100 >= pOptionTable->Chance_Clm) {
                continue;
            }
        }
        
        // Check situation
        if (!IsOptionSituation(
            static_cast<EFFECT_SITUATION_TYPE>(pOptionTable->Situation_Check),
            pRealTargetMover,
            pOptionTable->Aid_Value,
            pOptionTable->Aid_Clm)) {
            continue;
        }
        
        // Check if can execute (for Value_Clm == 3)
        if (pOptionTable->Value_Clm == 3) {
            if (!IsCanOptionEffectExcute(pOptionTable, pRealTargetMover, pEquipedOption->fOptionValue)) {
                continue;
            }
        }
        
        // Apply or refresh effect
        if (pAppliedOptionEffect) {
            // Refresh existing timed effect
            if (pAppliedOptionEffect->dwLifeTime >= 1000) {
                pAppliedOptionEffect->dwLifeTime = pOptionTable->Value_Clmcrt;
                pAppliedOptionEffect->fCurTime = 0.0f;
            }
        } else {
            // Calculate and apply new effect
            float fCalcValue = GetOptionEffectValue(
                pOptionTable,
                pRealTargetMover,
                pEquipedOption->fOptionValue,
                fParam);
            
            pRealTargetMover->ApplyOptionEffectValue(
                static_cast<EFFECT_STATUS_TYPE>(pOptionTable->StatusType),
                fCalcValue);
            
            AddOptionEffect(
                pEquipedOption->dwIndex,
                pOptionTable,
                pEquipedOption->fOptionValue,
                fParam,
                fCalcValue,
                pRealTargetMover);
        }
    }
}

/**
 * CMoverEx::ReleaseInvokedOptionEffect - Release invoked option effects
 * 
 * IDA: ?ReleaseInvokedOptionEffect@CMoverEx@@QEAAXW4EFFECT_CONDITION_TYPE@@@Z (0x14039BB70)
 * IDA decompiled: Releases option effects matching condition type
 */
void CMoverEx::ReleaseInvokedOptionEffect(EFFECT_CONDITION_TYPE eConditionType) {
    // Condition mapping array
    static const std::uint8_t iConditionList[] = {
        0,   // index 0
        0,   // index 1 - will be filled with Invoke_Condition
        2,   // index 2
        6,   // index 3
        9,   // index 4
        10,  // index 5
        21,  // index 6
        20,  // index 7
        3,   // index 8
        7,   // index 9
        5,   // index 10
        4    // index 11
    };
    
    for (auto it = m_vecOptionEffect.begin(); it != m_vecOptionEffect.end(); ) {
        SOptionEffect* pOptionEffect = *it;
        
        // Only process permanent effects (lifetime < 1000ms)
        if (pOptionEffect->dwLifeTime < 1000 && pOptionEffect->pOptionTable) {
            CMoverEx* pTargetMoverEx = static_cast<CMoverEx*>(CMover::GetMoverObject(pOptionEffect->dwTargetMoverID));
            
            if (pTargetMoverEx) {
                int iValueCondition = pOptionEffect->pOptionTable->Value_Clmcrt;
                
                // Check if condition matches
                bool bMatch = false;
                if (iValueCondition >= 0 && iValueCondition <= 11) {
                    std::uint8_t iCondList[12];
                    iCondList[0] = 0;
                    iCondList[1] = pOptionEffect->pOptionTable->Invoke_Condition;
                    for (int i = 2; i < 12; ++i) {
                        iCondList[i] = iConditionList[i];
                    }
                    
                    if (iCondList[iValueCondition] == eConditionType) {
                        bMatch = true;
                    }
                }
                
                if (bMatch) {
                    // Release the effect
                    ReleaseOptionEffectValue(
                        static_cast<EFFECT_STATUS_TYPE>(pOptionEffect->pOptionTable->StatusType),
                        pOptionEffect->fAppliedValue);
                    
                    delete pOptionEffect;
                    it = m_vecOptionEffect.erase(it);
                } else {
                    ++it;
                }
            } else {
                // Invalid target - just remove
                delete pOptionEffect;
                it = m_vecOptionEffect.erase(it);
            }
        } else {
            ++it;
        }
    }
}

/**
 * CMoverEx::ClearOptionEffect - Clear all option effects
 * 
 * IDA: ?ClearOptionEffect@CMoverEx@@QEAAXXZ (0x14039E990)
 * IDA decompiled: Deletes all option effects without releasing values
 */
void CMoverEx::ClearOptionEffect() {
    for (auto it = m_vecOptionEffect.begin(); it != m_vecOptionEffect.end(); ) {
        SOptionEffect* pOptionEffect = *it;
        if (pOptionEffect) {
            delete pOptionEffect;
            it = m_vecOptionEffect.erase(it);
        } else {
            ++it;
        }
    }
    m_vecOptionEffect.clear();
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
 * IDA decompiled: Updates stat based on effect type
 */
int CGocAttribute::UpdateBuffEffectStat(int nStatType, float fValue, bool bCalc, bool bUseInClear) {
    int nResultStatType = 0;
    bool bAdd = false;
    
    // Get stat type from effect
    auto pCalcStatus = TXSingleton<CCalculateStatus>::Instance();
    pCalcStatus->GetStatFromEffect(nStatType, &nResultStatType, &bAdd);
    
    if (nResultStatType) {
        // Basic stats (HP, SG, ST, SV)
        if (nResultStatType <= 3 || nResultStatType == 16) {
            if (bUseInClear && fValue <= 0.0f) {
                return nResultStatType;
            }
            
            if (!bAdd) {
                int iTargetStat = GetRateTargetStat(nResultStatType);
                // Note: IDA shows SetStat call here but parameters unclear
                // For now, just return the stat type
            }
            // Note: IDA shows SetStat call but implementation unclear
            return nResultStatType;
        }
        
        // Add or scale stat
        if (bAdd) {
            UpdateAddStat(nResultStatType, fValue, bCalc);
        } else {
            UpdateScaleStat(nResultStatType, fValue, bCalc);
        }
        
        return nResultStatType;
    } else {
        // Special effect
        int iIndex = GetSpecialEffectIndex(nStatType);
        if (iIndex >= 0 && iIndex <= 0x36) {
            m_fItemSpecaillEffect[iIndex] += fValue;
            m_bItemSpecialEffectChanged[iIndex] = true;
        }
        return 0;
    }
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
 * IDA decompiled: Returns base Res_Stun_Rate from status table + GetMaxInt(53)
 */
float CCalculateStatus::CALCULATE_STAT_RES_STUN(CGocAttribute* pAttr) {
    if (!pAttr) {
        return 0.0f;
    }
    float fResStunRate = static_cast<float>(pAttr->GetStatusTable()->Res_Stun_Rate);
    return fResStunRate + pAttr->GetMaxInt(53);
}

/**
 * CCalculateStatus::CALCULATE_STAT_RES_POISON - Calculate poison resistance
 * 
 * IDA: ?CALCULATE_STAT_RES_POISON@CCalculateStatus@@QEAAMPEAVCGocAttribute@@@Z (0x1402D8020)
 * IDA decompiled: Returns base Res_Poision_Rate from status table + GetMaxInt(50)
 */
float CCalculateStatus::CALCULATE_STAT_RES_POISON(CGocAttribute* pAttr) {
    if (!pAttr) {
        return 0.0f;
    }
    float fResPoisonRate = static_cast<float>(pAttr->GetStatusTable()->Res_Poision_Rate);
    return fResPoisonRate + pAttr->GetMaxInt(50);
}

/**
 * CMover::SetSlowTime - Set slow effect timer
 * 
 * IDA: ?SetSlowTime@CMover@@QEAAXMM@Z (0x140368AA0)
 * IDA decompiled: Sets slow time and speed, saves restore animation speed
 */
void CMover::SetSlowTime(float fTime, float fSpeed) {
    if (fTime > 0.0f && m_stTimeSlow.fTime == 0.0f) {
        m_fRestoreAnimSpeed = m_fAnimSpeed;
        if (XActor::IsStatus(1) || XActor::IsStatus(0x400)) {
            m_eRestoreAnimSpeedType = AST_ATTACK;
        } else {
            m_eRestoreAnimSpeedType = AST_NONE;
        }
    }
    m_stTimeSlow.fTime = fTime;
    m_stTimeSlow.fSpeed = fSpeed;
    SetAnimSpeed(fSpeed);
}

// ============================================================================
// Status Effect Trigger/Invoke Functions
// ============================================================================

/**
 * CMoverEx::ReleaseInvokedOptionEffectAfterDamage - Release effects after damage
 * 
 * IDA: ?ReleaseInvokedOptionEffectAfterDamage@CMoverEx@@QEAAXKE_N@Z (0x14039C780)
 * IDA decompiled: Releases invoked effects based on damage conditions
 */
void CMoverEx::ReleaseInvokedOptionEffectAfterDamage(std::uint32_t dwSkillID, bool bFlag) {
    // Get attacker from hit ID
    XArea* pArea = GetArea();
    if (!pArea) {
        return;
    }
    
    CMoverEx* pAttacker = static_cast<CMoverEx*>(pArea->FindActor(m_dwHitID));
    if (!pAttacker) {
        return;
    }
    
    // Check if attacker is a user
    CUser* pAttackerUser = nullptr;
    if (GetType() == 0) {
        pAttackerUser = dynamic_cast<CUser*>(pAttacker);
    }
    
    // Release effects on self (victim)
    ReleaseInvokedOptionEffect(EFFECT_CONDITION_DAMAGED);
    
    if (pAttackerUser && pAttackerUser->IsFlying()) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_DAMAGED_BY_JUMP_ATTACK);
    }
    
    // Check damage flags (from byDamageFlag parameter - using dwSkillID as flags for now)
    std::uint8_t byDamageFlag = static_cast<std::uint8_t>(dwSkillID);
    
    if ((byDamageFlag & 4) != 0) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_DAMAGED_CRITICAL);
    }
    
    if ((byDamageFlag & 1) != 0) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_DAMAGE_MISSED);
    }
    
    if (IsHitDown()) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_KNOCK_DOWN);
    }
    
    if (IsFlying()) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_KNOCK_BACK);
    }
    
    // Release effects on attacker
    if (pAttackerUser) {
        if (IsFlying()) {
            pAttackerUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ARIAL_ATTACK_SUCCESS);
        }
        
        if ((byDamageFlag & 4) != 0) {
            pAttackerUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_CRITICAL);
        }
        
        if ((byDamageFlag & 1) != 0) {
            pAttackerUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_MISSED);
        }
        
        if (bFlag) {
            pAttackerUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_MAKE_SA_BREAK);
        }
    } else if (pAttacker) {
        if ((byDamageFlag & 4) != 0) {
            pAttacker->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_CRITICAL);
        }
        
        if ((byDamageFlag & 1) != 0) {
            pAttacker->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_MISSED);
        }
        
        if (bFlag) {
            pAttacker->ReleaseInvokedOptionEffect(EFFECT_CONDITION_MAKE_SA_BREAK);
        }
    }
}

/**
 * CMoverEx::IsCanOptionEffectExcute - Check if option effect can execute
 * 
 * IDA: ?IsCanOptionEffectExcute@CMoverEx@@QEAA_NPEAUTB_CREATEOPTION@@PEAV1@M@Z (0x14039B010)
 * IDA decompiled: Checks if option effect with Value_Clm=3 can execute (probability check)
 */
bool CMoverEx::IsCanOptionEffectExcute(TB_CREATEOPTION* pOptionTable, CMoverEx* pMover, float fParam) {
    if (!pOptionTable) {
        return false;
    }
    
    // Only process Value_Clm == 3 (probability-based)
    if (pOptionTable->Value_Clm != 3) {
        return false;
    }
    
    // Check if target matches
    if (!IsOptionTarget(static_cast<EFFECT_TARGET_TYPE>(pOptionTable->Apply_Target), pMover)) {
        return false;
    }
    
    // Probability check: fParam > random(0-99)
    return fParam > static_cast<float>(rand() % 100);
}

/**
 * CMoverEx::GetOptionEffectValue - Calculate option effect value
 * 
 * IDA: ?GetOptionEffectValue@CMoverEx@@QEAAMPEAUTB_CREATEOPTION@@PEAV1@MM@Z (0x14039B120)
 * IDA decompiled: Calculates effect value based on Value_Clm and Refer_Status
 */
float CMoverEx::GetOptionEffectValue(TB_CREATEOPTION* pOptionTable, CMoverEx* pMover, 
                                      float fParam1, float fParam2) {
    if (!pOptionTable) {
        return 0.0f;
    }
    
    auto pAttr = GetGOC<CGocAttribute>();
    if (!pAttr) {
        return 0.0f;
    }
    
    // Value_Clm determines calculation type
    switch (pOptionTable->Value_Clm) {
        case 0:  // Fixed value 1.0
            return 1.0f;
        case 1:  // Direct option value
            return fParam1;
        case 2: {  // Percentage-based calculation
            float fValueRate = fParam1;
            int nReferStatus = pOptionTable->Refer_Status;
            
            switch (nReferStatus) {
                case 1:  // PATK
                    fValueRate = pAttr->GetOriginStat(21) * (fParam1 / 100.0f);
                    break;
                case 2:  // PDEF
                    fValueRate = pAttr->GetOriginStat(24) * (fParam1 / 100.0f);
                    break;
                case 3:  // Damage reference
                case 4:
                case 5:
                case 6:
                    fValueRate = fParam2 * (fParam1 / 100.0f);
                    break;
                case 7:  // Ability[10]
                    fValueRate = m_fAbility[10] * (fParam1 / 100.0f);
                    break;
                case 8:  // Stat 26
                    fValueRate = pAttr->GetOriginStat(26) * (fParam1 / 100.0f);
                    break;
                case 9:  // Stat 43
                    fValueRate = pAttr->GetOriginStat(43) * (fParam1 / 100.0f);
                    break;
                case 10:  // Stat 33
                    fValueRate = pAttr->GetOriginStat(33) * (fParam1 / 100.0f);
                    break;
                case 11:  // Ability[12]
                    fValueRate = m_fAbility[12] * (fParam1 / 100.0f);
                    break;
                case 12:  // Ability[14]
                    fValueRate = m_fAbility[14] * (fParam1 / 100.0f);
                    break;
                case 13:  // Stat 17 (Max SV)
                    fValueRate = pAttr->GetOriginStat(17) * (fParam1 / 100.0f);
                    break;
                case 14:  // Stat 18
                    fValueRate = pAttr->GetOriginStat(18) * (fParam1 / 100.0f);
                    break;
                case 15:  // Stat 19
                    fValueRate = pAttr->GetOriginStat(19) * (fParam1 / 100.0f);
                    break;
                case 16:  // Stat 31
                    fValueRate = pAttr->GetOriginStat(31) * (fParam1 / 100.0f);
                    break;
                case 17:  // Stat 38
                    fValueRate = pAttr->GetOriginStat(38) * (fParam1 / 100.0f);
                    break;
                case 18:  // Stat 47
                    fValueRate = pAttr->GetOriginStat(47) * (fParam1 / 100.0f);
                    break;
                case 19:  // Stat 35
                    fValueRate = pAttr->GetOriginStat(35) * (fParam1 / 100.0f);
                    break;
                default:
                    break;
            }
            return fValueRate;
        }
        case 3:  // Probability check result (always 1.0 if passed)
            return 1.0f;
        default:
            return 0.0f;
    }
}

/**
 * CMoverEx::GetSpecificOptionEffectValue - Get specific effect value
 * 
 * IDA: ?GetSpecificOptionEffectValue@CMoverEx@@QEAAMKW4EFFECT_STATUS_TYPE@@@Z (0x14039E120)
 * IDA decompiled: Sums applied values for matching equip index and status type
 */
float CMoverEx::GetSpecificOptionEffectValue(std::uint32_t dwIndex, EFFECT_STATUS_TYPE eType) {
    float fAppliedValue = 0.0f;
    
    for (auto it = m_vecOptionEffect.begin(); it != m_vecOptionEffect.end(); ++it) {
        SOptionEffect* pOptionEffect = *it;
        if (pOptionEffect && pOptionEffect->pOptionTable) {
            if (pOptionEffect->dwEquipedIndex == dwIndex && 
                pOptionEffect->pOptionTable->StatusType == eType) {
                fAppliedValue += pOptionEffect->fAppliedValue;
            }
        }
    }
    
    return fAppliedValue;
}

/**
 * CMoverEx::IsStatOptionEffect - Check if effect is a stat effect
 * 
 * IDA: ?IsStatOptionEffect@CMoverEx@@QEAA_NW4EFFECT_STATUS_TYPE@@@Z (0x14039B0A0)
 * IDA decompiled: Returns true for stat-affecting effect types
 */
bool CMoverEx::IsStatOptionEffect(EFFECT_STATUS_TYPE eType) {
    switch (eType) {
        case EFFECT_STATUS_PATK:
        case EFFECT_STATUS_PDEF:
        case EFFECT_STATUS_MAX_HP:
        case EFFECT_STATUS_MAX_SG:
        case EFFECT_STATUS_MAX_ST:
        case EFFECT_STATUS_REG_ST:
        case EFFECT_STATUS_MAX_SV:
        case EFFECT_STATUS_MSR:
        case EFFECT_STATUS_ASR:
        case EFFECT_STATUS_PAR:
        case EFFECT_STATUS_ADR:
        case EFFECT_STATUS_PCP:
        case EFFECT_STATUS_PCRP:
        case EFFECT_STATUS_PDSR:
        case EFFECT_STATUS_PARP:
        case EFFECT_STATUS_PDPR:
        case EFFECT_STATUS_PCA:
        case EFFECT_STATUS_CUR_ST:
            return true;
        default:
            return false;
    }
}

// ============================================================================
// Link Skill Duration (Special Status Effect)
// ============================================================================

/**
 * CMoverEx::GetLinkSkillDuration - Get link skill duration
 * 
 * IDA: ?GetLinkSkillDuration@CMoverEx@@QEAAMXZ (0x1403A27F0)
 * IDA decompiled: Simple getter for m_fLinkSkillDuration
 */
float CMoverEx::GetLinkSkillDuration() {
    return m_fLinkSkillDuration;
}

// ============================================================================
// Defense Change System
// ============================================================================

/**
 * CMoverEx::AddDefenseChangeInfo - Add defense change info
 * IDA: ?AddDefenseChangeInfo@CMoverEx@@QEAAHEEKM@Z (0x14037CF80)
 */
int CMoverEx::AddDefenseChangeInfo(std::uint8_t byType, std::uint8_t byDefenseType, std::uint32_t dwID, float fTime) {
    // IDA 精确还原:
    if (byType == 1) {
        SDefenseChangeInfo* pInfo = new SDefenseChangeInfo(dwID, byDefenseType, fTime);
        m_listDefenseChangeInfo.push_back(pInfo);
    } else if (byType == 2) {
        m_stDefenseChangeInfoByEffect.dwID = dwID;
        m_stDefenseChangeInfoByEffect.byDefenseType = byDefenseType;
        m_stDefenseChangeInfoByEffect.fChangeTime = fTime;
        if (byDefenseType == 3) {
            CMover::SetInvincibleActor(true);
        }
    } else {
        m_stDefenseChangeInfoByTrigger.dwID = dwID;
        m_stDefenseChangeInfoByTrigger.byDefenseType = byDefenseType;
        m_stDefenseChangeInfoByTrigger.fChangeTime = fTime;
    }

    m_DefanseChangeFlag |= (1 << byDefenseType);
    return ApplyDefenseChangeInfo();
}

/**
 * CMoverEx::UppdateDefenseChangeInfo - Update defense change timer
 * IDA: ?UppdateDefenseChangeInfo@CMoverEx@@QEAAXM@Z (0x14037D540)
 */
void CMoverEx::UppdateDefenseChangeInfo(float fDelta) {
    // IDA 精确还原:
    if (m_stDefenseChangeInfoByTrigger.fChangeTime > 0.0f) {
        m_stDefenseChangeInfoByTrigger.fChangeTime -= fDelta;
        if (m_stDefenseChangeInfoByTrigger.fChangeTime <= 0.0f) {
            RemoveDefenseChangeInfo(0, 0);
        }
    }
}

/**
 * CMoverEx::ApplyDefenseChangeInfo - Apply defense type based on flags
 * IDA: ?ApplyDefenseChangeInfo@CMoverEx@@QEAAHXZ (0x14037D5B0)
 */
bool CMoverEx::ApplyDefenseChangeInfo() {
    // IDA 精确还原:
    std::uint8_t byDefenseOld = m_byDefenseType;
    std::uint8_t eDefense = 0;

    if (m_pCurMotionEvent) {
        eDefense = m_pCurMotionEvent->eDefenseType;
    }

    if ((m_DefanseChangeFlag & 8) != 0 || eDefense == 3) { // INVINCIBLE
        CMover::SetSimpleDefenseType(3);
    } else if ((m_DefanseChangeFlag & 4) != 0 || eDefense == 2) { // SUPER_ARMOR
        CMover::SetSimpleDefenseType(2);
    } else {
        if ((m_DefanseChangeFlag & 1) == 0) {
            if (eDefense == 0) { // DEFENSE_NONE
                CMover::SetSimpleDefenseType(m_byDefaultDefenseType);
                return byDefenseOld != m_byDefenseType;
            }
            if (!XActor::IsStatus(2) && !XActor::IsStatus(4)) {
                CMover::SetSimpleDefenseType(eDefense);
                return byDefenseOld != m_byDefenseType;
            }
        }
        CMover::SetSimpleDefenseType(0);
    }

    return byDefenseOld != m_byDefenseType;
}

// ============================================================================
// Option Effect System
// ============================================================================

/**
 * CMoverEx::GetOptionEffect - Get option effect by index and table ID
 * IDA: ?GetOptionEffect@CMoverEx@@QEAAPEAUSOptionEffect@@KKPEAV1@@Z (0x14039BE80)
 */
SOptionEffect* CMoverEx::GetOptionEffect(std::uint32_t dwIndex, std::uint32_t dwTableID, CMoverEx* pTargetMoverEx) {
    // IDA 精确还原:
    for (auto it = m_vecOptionEffect.begin(); it != m_vecOptionEffect.end(); ++it) {
        SOptionEffect* pOptionEffect = *it;
        if (pOptionEffect && pOptionEffect->pOptionTable) {
            if (pOptionEffect->dwEquipedIndex == dwIndex &&
                pOptionEffect->pOptionTable->EffectType_Index == dwTableID) {
                if (!pTargetMoverEx) {
                    return pOptionEffect;
                }
                // Check if target mover ID matches
                if (pOptionEffect->dwTargetMoverID == pTargetMoverEx->GetActorID().__s0) {
                    return pOptionEffect;
                }
            }
        }
    }
    return nullptr;
}

/**
 * CMoverEx::AddOptionEffect - Add option effect
 * IDA: ?AddOptionEffect@CMoverEx@@QEAAXKPEAUTB_CREATEOPTION@@MMMPEAV1@@Z (0x14039BFB0)
 */
void CMoverEx::AddOptionEffect(std::uint32_t dwEquipedIndex, TB_CREATEOPTION* pOptionTable,
                               float fOptionValue, float fReferanceValue, float fAppliedValue,
                               CMoverEx* pTargetMoverEx) {
    // IDA 精确还原:
    if (!pOptionTable) return;

    SOptionEffect* pNewEffect = new SOptionEffect();
    pNewEffect->dwEquipedIndex = dwEquipedIndex;
    pNewEffect->pOptionTable = pOptionTable;
    pNewEffect->fOptionValue = fOptionValue;
    pNewEffect->fReferanceValue = fReferanceValue;
    pNewEffect->fAppliedValue = fAppliedValue;
    pNewEffect->dwLifeTime = pOptionTable->Value_Clmcrt;
    pNewEffect->fCurTime = 0.0f;
    
    if (pTargetMoverEx) {
        UXActorID actorID = pTargetMoverEx->GetActorID();
        pNewEffect->dwTargetMoverID = actorID.__s0;
    }

    m_vecOptionEffect.push_back(pNewEffect);
}

/**
 * CMoverEx::RemoveOptionEffect - Remove option effect by equipped index
 * IDA: ?RemoveOptionEffect@CMoverEx@@QEAAXK@Z (0x14039C420)
 */
void CMoverEx::RemoveOptionEffect(std::uint32_t dwEquipedIndex) {
    // IDA 精确还原:
    for (auto it = m_vecOptionEffect.begin(); it != m_vecOptionEffect.end(); ) {
        SOptionEffect* pOptionEffect = *it;
        CMoverEx* pTargetMoverEx = static_cast<CMoverEx*>(CMover::GetMoverObject(pOptionEffect->dwTargetMoverID));

        if (pTargetMoverEx && pOptionEffect->pOptionTable) {
            if (pOptionEffect->dwEquipedIndex == dwEquipedIndex) {
                delete pOptionEffect;
                it = m_vecOptionEffect.erase(it);
            } else {
                ++it;
            }
        } else {
            delete pOptionEffect;
            it = m_vecOptionEffect.erase(it);
        }
    }
}
