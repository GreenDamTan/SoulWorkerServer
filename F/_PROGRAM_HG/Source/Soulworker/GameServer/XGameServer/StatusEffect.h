/**
 * @file StatusEffect.h
 * @brief Status effect and condition system header
 * 
 * Status effects include: buffs, debuffs, stun, slow, poison, defense changes, etc.
 * All functions marked as verified = no until build verification.
 */

#pragma once

#include <cstdint>

// Forward declarations
class CMoverEx;
class CMover;
class CUser;
class XActor;
class CGocAttribute;
class CCalculateStatus;
struct TB_CREATEOPTION;

/**
 * @brief EFFECT_STATUS_TYPE - Status effect types
 * 
 * IDA verified from CMoverEx::ApplyOptionEffectValue and related functions
 */
enum EFFECT_STATUS_TYPE : std::int32_t {
    EFFECT_STATUS_NONE = 0,
    // TODO: Fill in from IDA analysis
    // Values discovered from CMoverEx::ApplyOptionEffectValue (0x14039CAB0)
};

/**
 * @brief EFFECT_CONDITION_TYPE - Effect trigger condition types
 * 
 * IDA verified from CMoverEx::CheckOptionEffectInvoke
 */
enum EFFECT_CONDITION_TYPE : std::int32_t {
    EFFECT_CONDITION_NONE = 0,
    // TODO: Fill in from IDA analysis
};

/**
 * @brief EFFECT_INVOKE_TYPE - Effect invocation types
 * 
 * IDA verified from CMoverEx::CheckOptionEffectInvoke
 */
enum EFFECT_INVOKE_TYPE : std::int32_t {
    EFFECT_INVOKE_NONE = 0,
    // TODO: Fill in from IDA analysis
};

/**
 * @brief Status effect type categorization for helper functions
 */
enum STATUS_EFFECT_CATEGORY : std::uint8_t {
    STATUS_EFFECT_ITEM_OPTION = 0,      // Item-based option effects
    STATUS_EFFECT_BOOSTER = 1,           // Booster/buff effects
    STATUS_EFFECT_DEFENSE_CHANGE = 2,    // Defense modification effects
    STATUS_EFFECT_ACTOR_FLAG = 3,        // Actor status flags
};

// ============================================================================
// Status Effect Helper Functions (Global)
// ============================================================================

/**
 * ApplyStatus - Apply a status effect to a mover
 * 
 * Dispatches to the appropriate system based on status type:
 * - Item option effects -> CMoverEx::AddOptionEffect
 * - Boosters -> CGocBooster::AddBooster
 * - Defense changes -> CMoverEx::AddDefenseChangeInfo
 * 
 * @param pMover Target mover
 * @param wStatusID Status effect ID
 * @param fDuration Duration in seconds (0 for permanent, -1 for unlimited)
 * @param fValue Effect value
 * @param byType Status category (STATUS_EFFECT_CATEGORY)
 * @return true if successfully applied
 */
bool ApplyStatus(CMoverEx* pMover, std::uint16_t wStatusID, float fDuration, 
                 float fValue, std::uint8_t byType);

/**
 * RemoveStatus - Remove a status effect from a mover
 * 
 * Dispatches to the appropriate removal function based on status type
 * 
 * @param pMover Target mover
 * @param wStatusID Status effect ID to remove
 * @param byType Status category (STATUS_EFFECT_CATEGORY)
 * @return true if successfully removed
 */
bool RemoveStatus(CMoverEx* pMover, std::uint16_t wStatusID, std::uint8_t byType);

/**
 * HasStatus - Check if a mover has a specific status effect
 * 
 * @param pMover Target mover
 * @param wStatusID Status effect ID to check
 * @param byType Status category (STATUS_EFFECT_CATEGORY)
 * @return true if the status is active
 */
bool HasStatus(CMoverEx* pMover, std::uint16_t wStatusID, std::uint8_t byType);

/**
 * GetStatusDuration - Get remaining duration of a status effect
 * 
 * @param pMover Target mover
 * @param wStatusID Status effect ID
 * @param byType Status category (STATUS_EFFECT_CATEGORY)
 * @return Remaining duration in seconds (-1 for permanent, 0 if not found)
 */
float GetStatusDuration(CMoverEx* pMover, std::uint16_t wStatusID, std::uint8_t byType);

/**
 * UpdateStatus - Update status effects over time
 * 
 * Calls update functions for all status effect systems
 * 
 * @param pMover Target mover
 * @param fDeltaTime Delta time in seconds
 */
void UpdateStatus(CMoverEx* pMover, float fDeltaTime);

// ============================================================================
// CMoverEx Status Effect Method Declarations
// ============================================================================

// Note: These methods are declared in MoverEx.h but implemented in StatusEffect.cpp

// Defense Change Functions
// void CMoverEx::AddDefenseChangeInfo(std::uint8_t byType, std::uint8_t byDefenseType, 
//                                       std::uint32_t dwID, float fChangeTime);
// void CMoverEx::RemoveDefenseChangeInfo(std::uint8_t byType, std::uint32_t dwID);
// void CMoverEx::RemoveAllDefenseChangeInfo();
// void CMoverEx::UppdateDefenseChangeInfo(float fDeltaTime);
// int CMoverEx::ApplyDefenseChangeInfo();

// Option Effect Functions
// void CMoverEx::AddOptionEffect(std::uint32_t dwEquipedIndex, TB_CREATEOPTION* pOptionTable,
//                                 float fParam1, float fParam2, float fParam3, CMoverEx* pTargetMover);
// void CMoverEx::RemoveOptionEffect(std::uint32_t dwEquipedIndex);
// void CMoverEx::RemoveAllOptionEffect();
// void CMoverEx::UpdateOptionEffect(float fDeltaTime);
// void CMoverEx::ApplyOptionEffectValue(EFFECT_STATUS_TYPE eType, float fValue);
// void CMoverEx::ReleaseOptionEffectValue(EFFECT_STATUS_TYPE eType, float fValue);
// float CMoverEx::GetTotalOptionEffectValue(EFFECT_STATUS_TYPE eType);
// void CMoverEx::CheckOptionEffectInvoke(EFFECT_CONDITION_TYPE eConditionType, CMoverEx* pMover,
//                                         float fParam, EFFECT_INVOKE_TYPE eInvokeType);
// void CMoverEx::ReleaseInvokedOptionEffect(EFFECT_CONDITION_TYPE eConditionType);
// void CMoverEx::ClearOptionEffect();
// void CMoverEx::ReleaseInvokedOptionEffectAfterDamage(std::uint32_t dwSkillID, bool bFlag);
// bool CMoverEx::IsCanOptionEffectExcute(TB_CREATEOPTION* pOptionTable, CMoverEx* pMover, float fParam);
// float CMoverEx::GetOptionEffectValue(TB_CREATEOPTION* pOptionTable, CMoverEx* pMover, 
//                                       float fParam1, float fParam2);
// float CMoverEx::GetSpecificOptionEffectValue(std::uint32_t dwIndex, EFFECT_STATUS_TYPE eType);
// bool CMoverEx::IsStatOptionEffect(EFFECT_STATUS_TYPE eType);
// float CMoverEx::GetLinkSkillDuration();

// ============================================================================
// CGocAttribute Status Effect Method Declarations
// ============================================================================

// Note: Declared in GocAttribute.h but implemented in StatusEffect.cpp

// int CGocAttribute::UpdateBuffEffectStat(int nStatType, float fValue, bool bCalc, bool bUseInClear);

// ============================================================================
// CCalculateStatus Status Resistance Methods
// ============================================================================

// Note: These methods should be declared in a CCalculateStatus header

// float CCalculateStatus::CALCULATE_STAT_RES_STUN(CGocAttribute* pAttr);
// float CCalculateStatus::CALCULATE_STAT_RES_POISON(CGocAttribute* pAttr);

// ============================================================================
// Status Flag Methods (XActor/CUser/CMover)
// ============================================================================

// Note: These methods should be declared in their respective headers

// XActor methods
// void XActor::SetStatus(std::uint32_t dwStatus);
// void XActor::ClearStatus(std::uint32_t dwStatus);
// void XActor::ResetStatus();
// bool XActor::IsStatus(std::uint32_t dwStatus) const;

// CUser methods
// bool CUser::IsStatus(std::uint32_t dwStatus) const;

// CMover methods
// void CMover::SetImmunityStatus(std::uint32_t dwStatus);
// void CMover::ClearImmunityStatus(std::uint32_t dwStatus);
// void CMover::SetSlowTime(float fSlowRate, float fDuration);
