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
    EFFECT_STATUS_DAMAGE_RATE = 14,
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
    EFFECT_CONDITION_USE_AKASHIC = 1,
    EFFECT_CONDITION_ATTACK_SUCCESS = 2,
    EFFECT_CONDITION_ATTACK_CRITICAL = 3,
    EFFECT_CONDITION_ATTACK_MISSED = 4,
    EFFECT_CONDITION_DAMAGE_MISSED = EFFECT_CONDITION_ATTACK_MISSED,
    EFFECT_CONDITION_DAMAGED = 6,
    EFFECT_CONDITION_DAMAGED_CRITICAL = 7,
    EFFECT_CONDITION_DEAD = 8,
    EFFECT_CONDITION_KILL = 9,
    EFFECT_CONDITION_DAMAGED_BY_JUMP_ATTACK = 10,
    EFFECT_CONDITION_MAKE_SA_BREAK = 11,
    EFFECT_CONDITION_KNOCK_BACK = 20,
    EFFECT_CONDITION_KNOCK_DOWN = 21,
    EFFECT_CONDITION_EQUIP = 30,
    EFFECT_CONDITION_HEAL = 31,
    EFFECT_CONDITION_SV_GAGE_UP = 32,
    EFFECT_CONDITION_SV_GAGE_DOWN = 33,
    EFFECT_CONDITION_REVIVE = 34,
    EFFECT_CONDITION_GAIN_EXP = 35,
    EFFECT_CONDITION_ARIAL_ATTACK_SUCCESS = 29,
};

enum EFFECT_SKILL_OPTION_TYPE : std::int32_t {
    EFFECT_SKILL_OPTION_DAMAGE = 1,
    EFFECT_SKILL_OPTION_PER_DAMAGE = 2,
    EFFECT_SKILL_OPTION_SG = 4,
};

/**
 * @brief EFFECT_INVOKE_TYPE - Effect invocation types
 *
 * IDA verified from CMoverEx::CheckOptionEffectInvoke
 */
enum EFFECT_INVOKE_TYPE : std::int32_t {
    EFFECT_INVOKE_DONT_CARE = 0,
    EFFECT_INVOKE_STAT = 0,
    EFFECT_INVOKE_NONE_STAT = 1,
    EFFECT_INVOKE_NONE = EFFECT_INVOKE_STAT,
};

enum EFFECT_SPECIAL_TYPE : std::int32_t {
    EFFECT_SPECIAL_COOLTIME_RAT = 100,
    EFFECT_SPECIAL_PVP_DAMAGE_INCREASE_INT = 101,
    EFFECT_SPECIAL_PVP_DAMAGE_DECREASE_INT = 102,
    EFFECT_SPECIAL_NORMAL_DAMAGE_ADD_RAT = 103,
    EFFECT_SPECIAL_BOSS_DAMAGE_ADD_RAT = 104,
    EFFECT_SPECIAL_MELEE_DAMAGE_ADD_RAT = 105,
    EFFECT_SPECIAL_RANGED_DAMAGE_ADD_RAT = 106,
    EFFECT_SPECIAL_AIR_DAMAGE_ADD_RAT = 107,
    EFFECT_SPECIAL_DOWN_DAMAGE_ADD_RAT = 108,
    EFFECT_SPECIAL_NORMAL_DAMAGED_DOWN_RAT = 109,
    EFFECT_SPECIAL_BOSS_DAMAGED_DOWN_RAT = 110,
    EFFECT_SPECIAL_MELEE_DAMAGE_DOWN_RAT = 111,
    EFFECT_SPECIAL_RANGED_DAMAGED_DOWN_RAT = 112,
    EFFECT_SPECIAL_SA_ATTACK_INCREASE_RAT = 113,
    EFFECT_SPECIAL_COST_DOWN_RAT = 114,
    EFFECT_SPECIAL_EXP_ADD_RAT = 115,
    EFFECT_SPECIAL_GOLD_ADD_RAT = 116,
    EFFECT_SPECIAL_SV_ADD_RAT = 117,
    EFFECT_SPECIAL_FIXED_ENDURANCE = 118,
    EFFECT_SPECIAL_GESTURE = 119,
    EFFECT_SPECIAL_DAMAGED_EFFECT = 120,
    EFFECT_SPECIAL_ABSORB_HP_RAT = 121,
    EFFECT_SPECIAL_ABSORB_SG_RAT = 122,
    EFFECT_SPECIAL_ATTACK_DEBUFF = 123,
    EFFECT_SPECIAL_DAMAGED_DEBUFF = 124,
    EFFECT_SPECIAL_DAMAGE_REFLECTION_RAT = 125,
    EFFECT_SPECIAL_SJUNK_DAMAGE_ADD_RAT = 126,
    EFFECT_SPECIAL_DOLL_DAMAGE_ADD_RAT = 127,
    EFFECT_SPECIAL_NED_DAMAGE_ADD_RAT = 128,
    EFFECT_SPECIAL_RAPID_DAMAGE_ADD_RAT = 129,
    EFFECT_SPECIAL_NIHIL_DAMAGE_ADD_RAT = 130,
    EFFECT_SPECIAL_IRON_DAMAGE_ADD_RAT = 131,
    EFFECT_SPECIAL_NEAR_DAMAGE_ADD_RAT = 132,
    EFFECT_SPECIAL_DIS6_DAMAGE_ADD_RAT = 133,
    EFFECT_SPECIAL_SJUNK_DAMAGED_DOWN_RAT = 134,
    EFFECT_SPECIAL_DOLL_DAMAGED_DOWN_RAT = 135,
    EFFECT_SPECIAL_NED_DAMAGED_DOWN_RAT = 136,
    EFFECT_SPECIAL_RAPID_DAMAGED_DOWN_RAT = 137,
    EFFECT_SPECIAL_NIHIL_DAMAGED_DOWN_RAT = 138,
    EFFECT_SPECIAL_IRON_DAMAGED_DOWN_RAT = 139,
    EFFECT_SPECIAL_NEAR_DAMAGED_DOWN_RAT = 140,
    EFFECT_SPECIAL_DIS6_DAMAGED_DOWN_RAT = 141,
    EFFECT_SPECIAL_MISS_DAMAGED_DOWN_RAT = 142,
    EFFECT_SPECIAL_AIR_DAMAGED_DOWN_RAT = 143,
    EFFECT_SPECIAL_DOWN_DAMAGED_DOWN_RAT = 144,
    EFFECT_SPECIAL_ATTACK_STAMINA_RAT = 145,
    EFFECT_SPECIAL_ATTACK_BUFF = 146,
    EFFECT_SPECIAL_DAMAGED_BUFF = 147,
    EFFECT_SPECIAL_EVADE_BUFF = 148,
    EFFECT_SPECIAL_INCREASE_SPEED_IN_DISTRICT = 149,
    EFFECT_SPECIAL_INCREASE_SPEED_IN_MAZE = 150,
    EFFECT_SPECIAL_CITADEL_DAMAGE_ADD_RAT = 151,
    EFFECT_SPECIAL_CITADEL_DAMAGED_DOWN_RAT = 152,
    EFFECT_SPECIAL_PRIMAL_DAMAGE_ADD_RAT = 153,
    EFFECT_SPECIAL_PRIMAL_DAMAGED_DOWN_RAT = 154,
    EFFECT_SPECIAL_END = 155,
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
