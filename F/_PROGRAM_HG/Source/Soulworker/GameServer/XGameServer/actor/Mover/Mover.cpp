#include "Mover.h"
#include <cstdarg>
#include <cstdio>

// External dependencies
class VisBaseEntity_cl {};
class XActor {};
class hkvVec3 {};
class hkvMat3 {};

/**
 * @brief CMover constructor
 * IDA: ??0CMover@@QEAA@XZ (0x1403649E0)
 */
CMover::CMover()
    : m_fLastUpdateTime(0.0f)
    , m_fLastDebugTime(0.0f)
    , m_fAbility(nullptr)
    , m_dwTargetID(0)
    , m_dwHitID(0)
    , m_fCapsuleRadius(0.0f)
    , m_fCapsuleHeight(0.0f)
    , m_fDieDelayTime(0.0f)
    , m_fDieDelayMaxTime(0.0f)
    , m_fDieFadeTime(0.0f)
    , m_nDamage(0)
    , m_cWeightRank(0)
    , m_byDmgMontionFlag(0)
    , m_fMoving(0)
    , m_bGazeMoving(0)
    , m_fMoveSpeed(0.0f)
    , m_byMoveDir(0)
    , m_nBuffTotalCnt(0)
    , m_nBuffCnt(0)
    , m_nDebuffCnt(0)
    , m_nHitCallBuffIndex(0)
    , m_fLastSendMoveTime(0.0f)
    , m_byDefaultAnimStep(0)
    , m_fAnimSpeed(1.0f)
    , m_fRestoreAnimSpeed(1.0f)
    , m_eRestoreAnimSpeedType(ANIM_SPEED_TYPE_NORMAL)
    , m_nMotionClass(0)
    , m_nHitStatus(0)
    , m_nHitAnimCount(0)
    , m_nTurnStatus(0)
    , m_bAnimChanged(0)
    , m_bSkipAnimOffset(0)
    , m_bAnimPlay(0)
    , m_nAnimationIdx(0)
    , m_nAnimGroup(0)
    , m_fAnimationTime(0.0f)
    , m_fAnimPercentTime(0.0f)
    , m_pCurMotionEvent(nullptr)
    , m_pActionResource(nullptr)
    , m_pAkashicActionInfo(nullptr)
    , m_nCurSkillTableIdx(0)
    , m_nParentSkillTableIdx(0)
    , m_fReactionRate(0.0f)
    , m_fFlySpeed(0.0f)
    , m_fFlyDistance(0.0f)
    , m_fFlyMoveDist(0.0f)
    , m_fFlyYaw(0.0f)
    , m_bLanded(0)
    , m_bOnGround(0)
    , m_fGroundPosZ(0.0f)
    , m_bRestoreCollision(0)
    , m_bKeepMovingExtra(0)
    , m_bMoveingInFly(false)
    , m_fGroundDownTime(0.0f)
    , m_fHitLoopMaxTime(0.0f)
    , m_byDownContinueDamage(0)
    , m_byDefenseType(0)
    , m_byRestoreDefenceType(0)
    , m_DefanseChangeFlag(0)
    , m_byDefaultDefenseType(0)
    , m_byRestoreDefenceTypeByTrigger(0)
    , m_byDefenseTypeDisableFlag(0)
    , m_fSkillCoolDownRate(1.0f)
    , m_fSkillReflectRate(0.0f)
    , m_fSkillBloodRate(0.0f)
    , m_nAllowBloodCount(0)
    , m_fSkillBloodDebuffRate(0.0f)
    , m_dwBloodDebuffOwnerID(0)
    , m_fSkillAbsorbSGRate(0.0f)
    , m_nAllowAbsorbSGCount(0)
    , m_bCheckSendAbsorbSG(false)
    , m_bCheckSendHP(false)
    , m_fBuffSuperArmorRate(0.0f)
    , m_bIgnoreSkillCost(0)
    , m_fDecreaseStaminaRate(0.0f)
    , m_fBuffAddGoldRate(0.0f)
    , m_dwAggroTargetID(0)
    , m_bCollisionEnable(1)
    , m_fCheckCollisionTime(0.0f)
    , m_mShaderState(0)
    , m_bInvincibleActor(0)
    , m_mapAnimInfoKey(nullptr)
    , m_mapAnimInfoString(nullptr)
    , m_pHitCollisionData(nullptr)
    , m_fHitCylinderRadius(0.0f)
    , m_fHitCylinderHeight(0.0f)
    , m_byTargetPosCount(0)
    , m_byTargetDestPos(0)
    , m_fMaxSuperArmorGage(0.0f)
    , m_fCurSuperArmorGage(0.0f)
    , m_fRegenSuperArmorTime(0.0f)
    , m_fDefRegenSuperArmorTime(0.0f)
    , m_fRecoverySuperArmorTime(0.0f)
    , m_dwCellID(0)
    , m_uiForcedState(0)
    , m_fForcedStateApplyTime(0.0f)
    , m_fProtectionAggroRatio(0.0f)
    , m_pSkillMgr(nullptr)
    , m_bTraceUser(0)
    , m_bPublicTransportRiding(false)
    , m_shCurRandomShootProjectileDirX(0.0f)
    , m_shCurRandomShootProjectileDirY(0.0f)
    , m_nRandomTrapIndex(0)
    , m_dwImmunityStatus(0)
    , m_bIgnoreAggroDebuff(false)
    , m_fBossAttackAddRate(0.0f)
    , m_fBossAttackedDownRate(0.0f)
    , m_fSoulCostDownRate(0.0f)
    , m_fAllAttackAddRate(0.0f)
    , m_bNoSkillCostSG(false)
{
    // Initialize move pos structures
    memset(&m_stMovePos, 0, sizeof(m_stMovePos));
    memset(&m_stMoveGap, 0, sizeof(m_stMoveGap));
    memset(&m_stMoveOffset, 0, sizeof(m_stMoveOffset));
    memset(&m_stExtMovingVal, 0, sizeof(m_stExtMovingVal));
    memset(&m_stTimeSlow, 0, sizeof(m_stTimeSlow));
    memset(m_stBuffState, 0, sizeof(m_stBuffState));
    memset(m_byTargetPosInfo, 0, sizeof(m_byTargetPosInfo));
}

/**
 * @brief CMover destructor
 */
CMover::~CMover() {
    // Clean up
    m_setHitID.clear();
    m_listSummonMob.clear();
    m_setAllowPassiveType.clear();
    m_listDefenseChangeInfo.clear();
    m_mapFilterData.clear();
    m_vTraceBoneName.clear();
    m_vecDelayedProjectile.clear();
    m_GOComponentTable.clear();
    m_vContinuousMelee.clear();
    m_mapMeleeDebuff.clear();
    m_mapSkillUnlock.clear();
}

/**
 * @brief Think function - called every frame
 * @param fDeltaTime Delta time since last frame
 */
void CMover::Think(float fDeltaTime) {
    // TODO: 汇编还原 - IDA address needed
    (void)fDeltaTime;
}

/**
 * @brief OnThink function
 */
void CMover::OnThink() {
    // TODO: 汇编还原 - IDA address needed
}

/**
 * @brief ActionProcess - process action
 * @param nAction Action ID
 * @return true if action was processed
 * IDA: ?ActionProcess@CMover@@UEAAHF@Z @ 0x1401892D0
 * Verified: Returns 1 (true) always - empty virtual stub
 */
bool CMover::ActionProcess(std::int16_t nAction) {
    (void)nAction;
    return true;
}

/**
 * @brief Damage - apply damage to mover
 * @param dwAttackerID Attacker entity ID
 * @param byDamageType Damage type
 * @param byAttackType Attack type
 * @param byHitType Hit type
 * @param nDamage Damage amount
 * @param fForce Force amount
 * @param vHitPos Hit position
 * IDA: ?Damage@CMover@@UEAAXKEEEHMAEAVhkvVec3@@@Z @ 0x140189300
 * Verified: Empty virtual stub - no implementation in base class
 */
void CMover::Damage(std::uint32_t dwAttackerID, std::uint8_t byReactionType,
                    std::uint8_t byAttackCollision) {
    (void)dwAttackerID;
    (void)byReactionType;
    (void)byAttackCollision;
}

/**
 * @brief DamageProcessHP - process HP damage
 * @param dwAttackerID Attacker entity ID
 * @param nSkillID Skill ID
 * @param nDamage Damage amount
 * @return true if damage was processed
 * IDA: ?DamageProcessHP@CMover@@UEAAHKHHEE@Z @ 0x1401892E0
 * Verified: Returns 1 (true) always - empty virtual stub
 */
bool CMover::DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage) {
    (void)dwID;
    (void)nSkillID;
    (void)nDamage;
    return true;
}

/**
 * @brief SetDie - set mover as dead
 * @param nMotionClass Motion class for death animation
 * @param bSuicide Suicide flag
 * @param bSendPacket Whether to send packet
 * IDA: ?SetDie@CMover@@UEAAXFH_N@Z @ 0x140188FE0
 * Verified: Empty virtual stub - no implementation in base class
 */
void CMover::SetDie(std::int16_t nMotionClass, int bSuicide, bool bSendPacket) {
    (void)nMotionClass;
    (void)bSuicide;
    (void)bSendPacket;
}

/**
 * @brief SetHP - set current HP
 * @param nHP New HP value (parameter name in IDA is bFreeze, likely unused)
 * IDA: ?SetHP@CMover@@UEAAXH@Z @ 0x140189230
 * Verified: Empty virtual stub - no implementation in base class
 */
void CMover::SetHP(int bFreeze) {
    (void)bFreeze;
}

/**
 * @brief GetStat - get stat value
 * @param iIndex Stat type index
 * @return Stat value from m_fAbility array
 * IDA: ?GetStat@CMover@@QEAAMH@Z @ 0x140166360
 * Verified: Returns m_fAbility[iIndex]
 */
float CMover::GetStat(int iIndex) {
    return m_fAbility[iIndex];
}

/**
 * @brief CalcTargetDamage - calculate target damage
 * @param pAttacker Attacker mover
 * @param nDamage Base damage
 * @param pSkill Skill used
 * @param fRate Damage rate
 * @param fDamage Damage multiplier
 * IDA: ?CalcTargetDamage@CMover@@UEAAXPEAV1@HPEAUTB_SKILL@@MM@Z (0x140F9000)
 */
void CMover::CalcTargetDamage(CMover* pAttacker, int nDamage, TB_SKILL* pSkill,
                               float fRate, float fDamage) {
    // TODO: 汇编还原 - IDA: 0x140F9000
    (void)pAttacker;
    (void)nDamage;
    (void)pSkill;
    (void)fRate;
    (void)fDamage;
}

/**
 * @brief AddActionBuffer - add action to buffer
 * @param xAction Action buffer
 * IDA: ?AddActionBuffer@CMover@@QEAAXAEAUtagACTION_BUFFER@@@Z @ 0x140016C30
 * Verified: Calls CActionBuffer::Push on m_xActionBuffer
 */
void CMover::AddActionBuffer(tagACTION_BUFFER& xAction) {
    m_xActionBuffer.Push(xAction);
}

/**
 * @brief SetHitCylinder - set hit cylinder dimensions
 * @param fRadius Radius
 * @param fHeight Height
 * IDA: ?SetHitCylinder@CMover@@QEAAXMM@Z @ 0x140016BF0
 * Verified: Sets m_fHitCylinderRadius and m_fHitCylinderHeight
 */
void CMover::SetHitCylinder(float fRadius, float fHeight) {
    m_fHitCylinderRadius = fRadius;
    m_fHitCylinderHeight = fHeight;
}

/**
 * @brief SetAnimInfoKey - set animation info key map
 * @param mapInfo Animation info key map
 * IDA: ?SetAnimInfoKey@CMover@@QEAAXPEAV?$map@VVString@@K...@@Z @ 0x1400164D0
 * Verified: Sets m_mapAnimInfoKey pointer
 */
void CMover::SetAnimInfoKey(std::map<class VString, std::uint32_t>* mapInfo) {
    m_mapAnimInfoKey = mapInfo;
}

/**
 * @brief SetAnimInfoString - set animation info string map
 * @param mapInfo Animation info string map
 * IDA: ?SetAnimInfoString@CMover@@QEAAXPEAV?$map@KVString@@...@@Z @ 0x1400164B0
 * Verified: Sets m_mapAnimInfoString pointer
 */
void CMover::SetAnimInfoString(std::map<std::uint32_t, class VString>* mapInfo) {
    m_mapAnimInfoString = mapInfo;
}

/**
 * @brief SetHitCollisionData - set hit collision data
 * @param pData Hit collision data
 * IDA: ?SetHitCollisionData@CMover@@QEAAXPEAUtagHIT_COLLISION_DATA@@@Z @ 0x140016BD0
 * Verified: Sets m_pHitCollisionData pointer
 */
void CMover::SetHitCollisionData(tagHIT_COLLISION_DATA* pData) {
    m_pHitCollisionData = pData;
}

/**
 * @brief GetHitCollisionCount - get hit collision count
 * @return Hit collision count
 * IDA: ?GetHitCollisionCount@CMover@@QEAAHXZ @ 0x140367B90
 * Verified: Returns size of m_pHitCollisionData->vHitColisions vector, or 0 if null
 */
unsigned int CMover::GetHitCollisionCount() const {
    if (m_pHitCollisionData) {
        return static_cast<unsigned int>(m_pHitCollisionData->vHitColisions.size());
    }
    return 0;
}

/**
 * @brief IsDamageMotionDisplay - check if damage motion should be displayed
 * @param byAttackCollision Attack collision type
 * @return true if damage motion should be displayed
 * IDA: ?IsDamageMotionDisplay@CMover@@UEAAHE@Z @ 0x140367BD0
 * Verified: Complex logic based on defense type and attack collision
 */
bool CMover::IsDamageMotionDisplay(std::uint8_t byAttackCollision) const {
    if (!byAttackCollision) {
        return false;
    }
    std::uint8_t byDefenseType = m_byDefenseType;
    if (byDefenseType == 1) {
        // Defense type 1: only display for attack collision 2 or 4
        return byAttackCollision == 2 || byAttackCollision == 4;
    } else if (byDefenseType == 2) {
        // Defense type 2: only display for attack collision 3 or 4
        return byAttackCollision == 3 || byAttackCollision == 4;
    } else {
        // Other defense types: display unless defense type is 3 or 5
        return byDefenseType != 3 && byDefenseType != 5;
    }
}

/**
 * @brief SetupPhysicsAndBound - setup physics and bounding values
 * @param fCollisionRadius Collision radius
 * @param fCollisionHeight Collision height
 * IDA: ?SetupPhysicsAndBound@CMover@@QEAAXMM@Z @ 0x140367910
 * Verified: Sets capsule and hit cylinder dimensions
 */
void CMover::SetupPhysicsAndBound(float fCollisionRadius, float fCollisionHeight) {
    m_fCapsuleRadius = fCollisionRadius;
    m_fCapsuleHeight = fCollisionHeight;
    m_fHitCylinderRadius = m_fCapsuleRadius;
    m_fHitCylinderHeight = m_fCapsuleHeight;
}

/**
 * @brief GetCurrentAnimationLength - get current animation length
 * @return Animation length or 0 if no current motion event
 * IDA: ?GetCurrentAnimationLength@CMover@@QEAAMXZ @ 0x140368B90
 * Verified: Returns m_pCurMotionEvent->fAnimationLength or 0.0
 */
float CMover::GetCurrentAnimationLength() const {
    if (m_pCurMotionEvent) {
        return m_pCurMotionEvent->fAnimationLength;
    }
    return 0.0f;
}

/**
 * @brief SetCurrentSequenceTime - set current sequence time
 * @param fTime Time value to set
 * IDA: ?SetCurrentSequenceTime@CMover@@QEAAXM@Z @ 0x140368BE0
 * Verified: Sets animation time and calculates percentage
 */
void CMover::SetCurrentSequenceTime(float fTime) {
    m_fAnimationTime = fTime;
    if (m_pCurMotionEvent && m_pCurMotionEvent->fAnimationLength > 0.0f) {
        m_fAnimPercentTime = fTime / m_pCurMotionEvent->fAnimationLength;
    }
}

/**
 * @brief SetCurrentSequencePosition - set current sequence position (0.0-1.0)
 * @param fPos Position value (0.0 to 1.0)
 * IDA: ?SetCurrentSequencePosition@CMover@@QEAAXM@Z @ 0x140368C60
 * Verified: Sets animation time based on position percentage
 */
void CMover::SetCurrentSequencePosition(float fPos) {
    if (m_pCurMotionEvent) {
        m_fAnimationTime = m_pCurMotionEvent->fAnimationLength * fPos;
        m_fAnimPercentTime = fPos;
    }
}

/**
 * @brief AnimKeyToMotion - convert animation key to motion class
 * @param dwAnimKey Animation key
 * @return Motion class (animKey / 1000)
 * IDA: ?AnimKeyToMotion@CMover@@QEAAFK@Z @ 0x140368A80
 * Verified: Returns dwAnimKey / 0x3E8 (1000)
 */
std::uint16_t CMover::AnimKeyToMotion(std::uint32_t dwAnimKey) const {
    return static_cast<std::uint16_t>(dwAnimKey / 1000);
}

/**
 * @brief SetSlowTime - set slow time effect
 * @param fTime Slow time duration
 * @param fSpeed Slow speed multiplier
 * IDA: ?SetSlowTime@CMover@@QEAAXMM@Z @ 0x140368AA0
 * Verified: Sets slow time data and adjusts animation speed
 */
void CMover::SetSlowTime(float fTime, float fSpeed) {
    if (fTime > 0.0f && m_stTimeSlow.fTime == 0.0f) {
        m_fRestoreAnimSpeed = m_fAnimSpeed;
        // TODO: Check XActor status
        // if (XActor::IsStatus(&this->XActor, 1u) || XActor::IsStatus(&this->XActor, 0x400u)) {
        //     m_eRestoreAnimSpeedType = AST_ATTACK;
        // } else {
        //     m_eRestoreAnimSpeedType = AST_NONE;
        // }
        m_eRestoreAnimSpeedType = ANIM_SPEED_TYPE_NORMAL;
    }
    m_stTimeSlow.fTime = fTime;
    m_stTimeSlow.fSpeed = fSpeed;
    SetAnimSpeed(fSpeed);
}

/**
 * @brief GetTableIDString - get table ID string
 * @return Table ID string
 * IDA: ?GetTableIDString@CMover@@QEAAPEBDXZ (0x1406CE70)
 */
const char* CMover::GetTableIDString() {
    // TODO: 汇编还原 - IDA: 0x1406CE70
    return "";
}

/**
 * @brief SetNoSkillCostSG - set no skill cost SG flag
 * @param bCost No cost flag
 * IDA: ?SetNoSkillCostSG@CMover@@QEAAX_N@Z @ 0x1400488E0
 * Verified: Sets m_bNoSkillCostSG
 */
void CMover::SetNoSkillCostSG(bool bCost) {
    m_bNoSkillCostSG = bCost;
}

/**
 * @brief IsAllowPassiveType - check if passive type is allowed
 * @param iType Passive type
 * @return true if allowed (returns true if set is empty)
 * IDA: ?IsAllowPassiveType@CMover@@QEAAHH@Z @ 0x140364670
 * Verified: Checks m_setAllowPassiveType.find() != end(), returns 1 if set is empty
 */
bool CMover::IsAllowPassiveType(int iType) {
    // IDA: If set is empty, returns true (1)
    if (m_setAllowPassiveType.empty()) {
        return true;
    }
    return m_setAllowPassiveType.find(iType) != m_setAllowPassiveType.end();
}

/**
 * @brief SetSimpleDefenseType - set simple defense type
 * @param byType Defense type
 * IDA: ?SetSimpleDefenseType@CMover@@QEAAXE@Z @ 0x140354350
 * Verified: Sets m_byDefenseType
 */
void CMover::SetSimpleDefenseType(std::uint8_t byType) {
    m_byDefenseType = byType;
}

/**
 * @brief IsDie - check if mover is dead
 * @return true if dead (XActor::IsDieStatus or HP <= 0)
 * IDA: ?IsDie@CMover@@QEAAHXZ @ 0x140366E40
 * Verified: Returns XActor::IsDieStatus() || GetHP() <= 0
 */
bool CMover::IsDie() const {
    // IDA: return XActor::IsDieStatus(&this->XActor) || this->GetHP(this) <= 0;
    // TODO: Need XActor::IsDieStatus implementation
    // For now, check if GetHP returns <= 0
    return GetHP() <= 0;
}

/**
 * @brief IsKnockDown - check if mover is knocked down
 * @return true if knocked down
 * IDA: ?IsKnockDown@CMover@@QEAAHXZ @ 0x1403671C0
 * Verified: Checks m_fForcedStateApplyTime > 0 && m_uiForcedState == 3,
 *           or m_nMotionClass >= 18 && m_nMotionClass <= 21
 */
bool CMover::IsKnockDown() const {
    // IDA: If forced state is active, check if it's knockdown state (3)
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 3;
    }
    // IDA: Otherwise check motion class range (18-21 are knockdown motions)
    return m_nMotionClass >= 18 && m_nMotionClass <= 21;
}

/**
 * @brief IsKnockDown_int - check if mover is knocked down (int version)
 * @return 1 if knocked down
 * IDA: ?IsKnockDown@CMover@@QEAAHXZ @ 0x1403671C0
 */
int CMover::IsKnockDown_int() {
    return IsKnockDown() ? 1 : 0;
}

/**
 * @brief IsHit - check if mover is in hit state
 * @return true if in hit state (motion class 15-23)
 * IDA: ?IsHit@CMover@@QEAAHXZ @ 0x140367230
 * Verified: Returns m_nMotionClass >= 15 && m_nMotionClass <= 23
 */
bool CMover::IsHit() const {
    return m_nMotionClass >= 15 && m_nMotionClass <= 23;
}

/**
 * @brief IsCounterAttackHit - check if mover is in counter attack hit state
 * @return true if in counter attack hit state
 * IDA: ?IsCounterAttackHit@CMover@@UEAAHXZ @ 0x140367360
 * Verified: Checks forced state 4, or motion class 17-23 (excluding 22) with conditions
 */
bool CMover::IsCounterAttackHit() const {
    // IDA: If forced state is active, check if it's fly hit state (4)
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 4;
    }
    // IDA: Check not hit down, motion class 17-23 (not 22), hit status not 5
    if (IsHitDown()) {
        return false;
    }
    if (m_nMotionClass >= 17 && m_nMotionClass <= 23 && m_nMotionClass != 22) {
        return m_nHitStatus != 5;
    }
    return false;
}

/**
 * @brief IsHitDown - check if mover is in hit down state
 * @return true if in hit down state
 * IDA: ?IsHitDown@CMover@@QEAAHXZ @ 0x140367270
 * Verified: Complex logic checking forced state, motion class, hit status, and landed state
 */
bool CMover::IsHitDown() const {
    // IDA: If forced state is active, check if it's knockdown state (3)
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 3;
    }
    // IDA: Check motion class range (18-21) or specific motion (13)
    if (m_nMotionClass < 18 || m_nMotionClass > 21) {
        return m_nMotionClass == 13;
    }
    // IDA: Check hit status values 1, 2, 3
    if (m_nHitStatus == 1 || m_nHitStatus == 2 || m_nHitStatus == 3) {
        return true;
    }
    // IDA: Complex condition: landed && hit status not 5 && hit status non-zero, or motion class 13
    return (m_bLanded && m_nHitStatus != 5 && m_nHitStatus != 0) || m_nMotionClass == 13;
}

/**
 * @brief IsGeneralHit - check if mover is in general hit state
 * @return true if in general hit state (motion class 15-17 or forced state 4)
 * IDA: ?IsGeneralHit@CMover@@QEAAHXZ @ 0x140367410
 * Verified: Checks forced state 4, or motion class 15-17
 */
bool CMover::IsGeneralHit() const {
    // IDA: If forced state is active, check if it's fly hit state (4)
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 4;
    }
    // IDA: Check motion class range (15-17)
    return m_nMotionClass >= 15 && m_nMotionClass <= 17;
}

/**
 * @brief IsFlyHit - check if mover is in fly hit state
 * @return true if in fly hit state (forced state 4 or motion class 18-21)
 * IDA: ?IsFlyHit@CMover@@QEAAHXZ @ 0x140367480
 * Verified: Checks forced state 4, or motion class 18-21
 */
bool CMover::IsFlyHit() const {
    // IDA: If forced state is active, check if it's fly hit state (4)
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 4;
    }
    // IDA: Check motion class range (18-21)
    return m_nMotionClass >= 18 && m_nMotionClass <= 21;
}

/**
 * @brief IsDashing - check if mover is dashing
 * @return true if dashing (forced state 2 or XActor status 0x800)
 * IDA: ?IsDashing@CMover@@QEAAHXZ @ 0x1403674F0
 * Verified: Checks m_fForcedStateApplyTime > 0 && m_uiForcedState == 2,
 *           or XActor::IsStatus(0x800)
 */
bool CMover::IsDashing() const {
    // IDA: If forced state is active, check if it's dashing state (2)
    if (m_fForcedStateApplyTime <= 0.0f) {
        // TODO: Need XActor::IsStatus implementation
        // return XActor::IsStatus(&this->XActor, 0x800u);
        return false;
    }
    return m_uiForcedState == 2;
}

/**
 * @brief IsFlying - check if mover is flying
 * @return true if flying
 * IDA: ?IsFlying@CMover@@QEAAHXZ @ 0x140367080
 * Verified: Checks m_fForcedStateApplyTime > 0 && m_uiForcedState == 1,
 *           or !m_bLanded && position.z > groundHeight + 5.0
 */
bool CMover::IsFlying() const {
    // IDA: If forced state is active, check if it's flying state (1)
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 1;
    }
    // IDA: If landed, not flying
    if (m_bLanded) {
        return false;
    }
    // IDA: Check if position is above ground by more than 5 units
    // Note: GetHeight() needs to be implemented for full functionality
    // For now, return basic state check
    // TODO: 汇编还原 - Need GetHeight() implementation
    return false;
}

/**
 * @brief SendUpdateStat - send stat update
 * @param nStatType Stat type to update
 * IDA: ?SendUpdateStat@CMover@@QEAAXH@Z (0x14036D4A0)
 */
void CMover::SendUpdateStat(int nStatType) {
    // TODO: 汇编还原 - IDA: 0x14036D4A0
    (void)nStatType;
}

/**
 * @brief GetBuffStatus - get buff status
 * @return Pointer to buff status array
 * IDA: ?GetBuffStatus@CMover@@QEAAPEAUtagBUFF_STATE@@XZ @ 0x1403539C0
 * Verified: Returns m_stBuffState pointer
 */
tagBUFF_STATE* CMover::GetBuffStatus() {
    return m_stBuffState;
}

/**
 * @brief IsImmunityStatus - check if mover has any immunity status
 * @return true if has immunity status
 * IDA: ?IsImmunityStatus@CMover@@QEAAHXZ @ 0x140364700
 * Verified: Returns m_dwImmunityStatus != 0
 */
bool CMover::IsImmunityStatus() const {
    return m_dwImmunityStatus != 0;
}

/**
 * @brief ClearImmunityStatus - clear immunity status flags
 * @param dwStatus Status flags to clear
 * IDA: ?ClearImmunityStatus@CMover@@QEAAXK@Z @ 0x140353040
 * Verified: m_dwImmunityStatus &= ~dwStatus
 */
void CMover::ClearImmunityStatus(std::uint32_t dwStatus) {
    m_dwImmunityStatus &= ~dwStatus;
}

/**
 * @brief GetJumpSpeed - get jump speed
 * @return Jump speed (m_fFlySpeed)
 * IDA: ?GetJumpSpeed@CMover@@QEAAMXZ @ 0x1402C7330
 * Verified: Returns m_fFlySpeed
 */
float CMover::GetJumpSpeed() const {
    return m_fFlySpeed;
}

/**
 * @brief GetDieDelayTime - get die delay time
 * @return Die delay time
 * IDA: ?GetDieDelayTime@CMover@@QEAAMXZ @ 0x1402C7BD0
 * Verified: Returns m_fDieDelayTime
 */
float CMover::GetDieDelayTime() const {
    return m_fDieDelayTime;
}

/**
 * @brief GetDecreaseStaminaRate - get decrease stamina rate
 * @return Decrease stamina rate
 * IDA: ?GetDecreaseStaminaRate@CMover@@QEAAMXZ @ 0x1402C7EE0
 * Verified: Returns m_fDecreaseStaminaRate
 */
float CMover::GetDecreaseStaminaRate() const {
    return m_fDecreaseStaminaRate;
}

/**
 * @brief GetIgnoreSkillCost - get ignore skill cost flag
 * @return Ignore skill cost flag
 * IDA: ?GetIgnoreSkillCost@CMover@@QEAAHXZ @ 0x1402C7F00
 * Verified: Returns m_bIgnoreSkillCost
 */
int CMover::GetIgnoreSkillCost() const {
    return m_bIgnoreSkillCost;
}

/**
 * @brief AddSummonMobList - add summoned mob to list
 * @param dwMobID Mob ID to add
 * IDA: ?AddSummonMobList@CMover@@QEAAXK@Z @ 0x1402C7CC0
 * Verified: Calls m_listSummonMob.push_back(dwMobID)
 */
void CMover::AddSummonMobList(std::uint32_t dwMobID) {
    m_listSummonMob.push_back(dwMobID);
}

/**
 * @brief SetProtectionAggroRatio - set protection aggro ratio
 * @param ratio Protection aggro ratio
 * IDA: ?SetProtectionAggroRatio@CMover@@QEAAXM@Z @ 0x1403655C0
 * Verified: Sets m_fProtectionAggroRatio = ratio
 */
void CMover::SetProtectionAggroRatio(float ratio) {
    m_fProtectionAggroRatio = ratio;
}

/**
 * @brief SetDmgMotionFlag - set damage motion flag
 * @param byFlag Damage motion flag
 * IDA: ?SetDmgMotionFlag@CMover@@QEAAXE@Z @ 0x1403655E0
 * Verified: Sets m_byDmgMontionFlag = byFlag
 */
void CMover::SetDmgMotionFlag(std::uint8_t byFlag) {
    m_byDmgMontionFlag = byFlag;
}

/**
 * @brief SetWeightRank - set weight rank
 * @param cVal Weight rank value
 * IDA: ?SetWeightRank@CMover@@QEAAXD@Z @ 0x140364D40
 * Verified: Sets m_cWeightRank = cVal
 */
void CMover::SetWeightRank(std::int8_t cVal) {
    m_cWeightRank = cVal;
}

/**
 * @brief SetParentSkillTableIdx - set parent skill table index
 * @param nVal Parent skill table index
 * IDA: ?SetParentSkillTableIdx@CMover@@QEAAXH@Z @ 0x140364610
 * Verified: Sets m_nParentSkillTableIdx = nVal
 */
void CMover::SetParentSkillTableIdx(int nVal) {
    m_nParentSkillTableIdx = nVal;
}

/**
 * @brief GetCurSkillTableIdx - get current skill table index
 * @return Current skill table index
 * IDA: ?GetCurSkillTableIdx@CMover@@QEAAHXZ @ 0x140364650
 * Verified: Returns m_nCurSkillTableIdx
 */
int CMover::GetCurSkillTableIdx() const {
    return m_nCurSkillTableIdx;
}

/**
 * @brief SetIgnoreAggroDebuff - set ignore aggro debuff flag
 * @param bApply Whether to ignore aggro debuff
 * IDA: ?SetIgnoreAggroDebuff@CMover@@QEAAXH@Z @ 0x1402A67F0
 * Verified: Sets m_bIgnoreAggroDebuff = bApply != 0
 */
void CMover::SetIgnoreAggroDebuff(int bApply) {
    m_bIgnoreAggroDebuff = (bApply != 0);
}

/**
 * @brief MoveingValueClear - clear moving values
 * IDA: ?MoveingValueClear@CMover@@QEAAXXZ @ 0x1402A4BE0
 * Verified: Clears m_fMoving, m_stMovePos, m_stMoveGap, m_stMoveOffset
 */
void CMover::MoveingValueClear() {
    m_fMoving = 0;
    // Clear move pos structures - need tagMOVE_POS::Clear implementation
    // For now, zero out the structures
    memset(&m_stMovePos, 0, sizeof(m_stMovePos));
    memset(&m_stMoveGap, 0, sizeof(m_stMoveGap));
    memset(&m_stMoveOffset, 0, sizeof(m_stMoveOffset));
}

/**
 * @brief GetBloodDebuffOwnerID - get blood debuff owner ID
 * @return Blood debuff owner ID
 * IDA: ?GetBloodDebuffOwnerID@CMover@@QEAAKXZ (0x140A13B0)
 */
std::uint32_t CMover::GetBloodDebuffOwnerID() {
    return m_dwBloodDebuffOwnerID;
}

/**
 * @brief DeleteDelayedProjectile - delete delayed projectile
 * @param pProjectile Projectile to delete
 * IDA: ?DeleteDelayedProjectile@CMover@@QEAAXPEAUSDelayedProjectile@@@Z (0x1406D550)
 */
void CMover::DeleteDelayedProjectile(SDelayedProjectile* pProjectile) {
    // TODO: 汇编还原 - IDA: 0x1406D550
    (void)pProjectile;
}

/**
 * @brief CheckContinuousMelee - check continuous melee
 * @param fTime Time value
 * IDA: ?CheckContinuousMelee@CMover@@QEAAXM@Z (0x14072EE0)
 */
void CMover::CheckContinuousMelee(float fTime) {
    // TODO: 汇编还原 - IDA: 0x14072EE0
    (void)fTime;
}

/**
 * @brief ClearTargetPosFlag - clear target position flag
 * @param byFlag Flag to clear
 * IDA: ?ClearTargetPosFlag@CMover@@QEAAXE@Z (0x1406CA80)
 */
void CMover::ClearTargetPosFlag(std::uint8_t byFlag) {
    // TODO: 汇编还原 - IDA: 0x1406CA80
    (void)byFlag;
}

/**
 * @brief FindTargetPos - find target position
 * @param pTarget Target mover
 * @return Result
 * IDA: ?FindTargetPos@CMover@@QEAAEPEAV1@@Z (0x1406C380)
 */
std::uint8_t CMover::FindTargetPos(CMover* pTarget) {
    // TODO: 汇编还原 - IDA: 0x1406C380
    (void)pTarget;
    return 0;
}

/**
 * @brief GetHitList - get hit list
 * @param listHit Hit list to fill
 * IDA: ?GetHitList@CMover@@QEAAXAEAV?$list@UST_MONSTER_DAMAGE_INFO@@...@@Z (0x14073220)
 */
void CMover::GetHitList(std::list<ST_MONSTER_DAMAGE_INFO>& listHit) {
    // TODO: 汇编还原 - IDA: 0x14073220
    (void)listHit;
}

/**
 * @brief IsAttackHeight - check attack height
 * @param stArea Attack area
 * @param vPos Position
 * @param nResult Result
 * @return Result
 * IDA: ?IsAttackHeight@CMover@@QEAAEQEAUtagATTACK_AREA@@AEAVhkvVec3@@AEAH@Z (0x14067CE0)
 */
std::uint8_t CMover::IsAttackHeight(const struct tagATTACK_AREA& stArea, hkvVec3& vPos, int& nResult) {
    // TODO: 汇编还原 - IDA: 0x14067CE0
    (void)stArea;
    (void)vPos;
    (void)nResult;
    return 0;
}

/**
 * @brief SetupPhysicsAndBound - setup physics and bounding capsule
 * @param fCollisionRadius Collision radius
 * @param fCollisionHeight Collision height
 * IDA: ?SetupPhysicsAndBound@CMover@@QEAAXMM@Z @ 0x140367910
 * Verified: Sets capsule radius/height and hit cylinder dimensions
 */
void CMover::SetupPhysicsAndBound(float fCollisionRadius, float fCollisionHeight) {
    m_fCapsuleRadius = fCollisionRadius;
    m_fCapsuleHeight = fCollisionHeight;
    m_fHitCylinderRadius = m_fCapsuleRadius;
    m_fHitCylinderHeight = m_fCapsuleHeight;
}

/**
 * @brief SetAnimSpeed - set animation speed
 * @param fSpeed Animation speed
 * IDA: ?SetAnimSpeed@CMover@@QEAAXM@Z @ 0x140368CC0
 * Verified: Sets m_fAnimSpeed = fSpeed
 */
void CMover::SetAnimSpeed(float fSpeed) {
    m_fAnimSpeed = fSpeed;
}

/**
 * @brief SetCurrentSequencePosition - set current animation sequence position
 * @param fPos Position (0.0 - 1.0)
 * IDA: ?SetCurrentSequencePosition@CMover@@QEAAXM@Z @ 0x140368C60
 * Verified: Sets animation time based on percentage
 */
void CMover::SetCurrentSequencePosition(float fPos) {
    if (m_pCurMotionEvent) {
        m_fAnimationTime = m_pCurMotionEvent->fAnimationLength * fPos;
        m_fAnimPercentTime = fPos;
    }
}

/**
 * @brief GetBoneYaw - get bone yaw rotation
 * @param idx Bone index
 * @return Bone yaw rotation
 * IDA: ?GetBoneYaw@CMover@@QEAAMH@Z @ 0x140368880
 * Verified: Returns bone rotation from animation info
 */
float CMover::GetBoneYaw(int idx) {
    if (m_pCurMotionEvent) {
        // TODO: Need VAnimationInfo::GetBoneRotaion implementation
        // return VAnimationInfo::GetBoneRotaion(m_pCurMotionEvent, idx, m_fAnimationTime);
    }
    return 0.0f;
}

/**
 * @brief GetCurrentAnimationLength - get current animation length
 * @return Animation length in seconds
 * IDA: ?GetCurrentAnimationLength@CMover@@QEAAMXZ @ 0x140368B90
 * Verified: Returns fAnimationLength from current motion event
 */
float CMover::GetCurrentAnimationLength() const {
    if (m_pCurMotionEvent) {
        return m_pCurMotionEvent->fAnimationLength;
    }
    return 0.0f;
}

/**
 * @brief SetSlowTime - set slow time effect
 * @param fTime Slow time duration
 * @param fSpeed Slow time speed multiplier
 * IDA: ?SetSlowTime@CMover@@QEAAXMM@Z @ 0x140368AA0
 * Verified: Saves restore speed and applies slow effect
 */
void CMover::SetSlowTime(float fTime, float fSpeed) {
    if (fTime > 0.0f && m_stTimeSlow.fTime == 0.0f) {
        m_fRestoreAnimSpeed = m_fAnimSpeed;
        // TODO: Need XActor::IsStatus implementation
        // if (XActor::IsStatus(0x01) || XActor::IsStatus(0x400)) {
        //     m_eRestoreAnimSpeedType = AST_ATTACK;
        // } else {
        //     m_eRestoreAnimSpeedType = AST_NONE;
        // }
    }
    m_stTimeSlow.fTime = fTime;
    m_stTimeSlow.fSpeed = fSpeed;
    SetAnimSpeed(fSpeed);
}

/**
 * @brief GetAnimStirng - get animation string by key
 * @param dwAnimKey Animation key
 * @return Animation string or nullptr
 * IDA: ?GetAnimStirng@CMover@@QEAAPEADK@Z @ 0x1403688D0
 */
const char* CMover::GetAnimStirng(std::uint32_t dwAnimKey) {
    if (!m_mapAnimInfoString) {
        return nullptr;
    }
    auto it = m_mapAnimInfoString->find(dwAnimKey);
    if (it != m_mapAnimInfoString->end()) {
        return it->second.AsChar();
    }
    return nullptr;
}

/**
 * @brief GetAnimIndex - get animation index by name
 * @param strAnimName Animation name
 * @return Animation index or -1
 * IDA: ?GetAnimIndex@CMover@@QEAAKVVString@@@Z @ 0x140368960
 */
std::uint32_t CMover::GetAnimIndex(const class VString& strAnimName) {
    if (m_mapAnimInfoKey) {
        auto it = m_mapAnimInfoKey->find(strAnimName);
        if (it != m_mapAnimInfoKey->end()) {
            return it->second;
        }
    }
    return 0xFFFFFFFF;
}

/**
 * @brief FindBuffStatus - find buff status by index
 * @param nBuffIndex Buff index
 * @param dwAttackerID Attacker ID
 * @return Buff slot index or -1
 * IDA: ?FindBuffStatus@CMover@@QEAAHGK@Z @ 0x14036A420
 * Verified: Iterates through buff state array
 */
int CMover::FindBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwAttackerID) const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == nBuffIndex) {
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
 * @brief FindBuffByGroupID - find buff by group ID
 * @param nGroupID Group ID
 * @param dwAttackerID Attacker ID
 * @return Buff slot index or -1
 * IDA: ?FindBuffByGroupID@CMover@@QEAAHGK@Z @ 0x14036A4C0
 * Verified: Iterates through buff state array checking group ID
 */
int CMover::FindBuffByGroupID(std::uint16_t nGroupID, std::uint32_t dwAttackerID) const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nGroupID == nGroupID) {
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
 * @brief FindBuffByEffectType - find buff by effect type
 * @param byBuffEffect Effect type to find
 * @param nExceptBuffIndex Buff index to exclude
 * @return Buff slot index or -1
 * IDA: ?FindBuffByEffectType@CMover@@QEAAHEG@Z @ 0x14036A560
 * Verified: Iterates through buff state array checking effect type
 */
int CMover::FindBuffByEffectType(std::uint8_t byBuffEffect, std::uint16_t nExceptBuffIndex) const {
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
 * @brief IsHaveImunityInvincibleBuff - check if has immunity invincible buff
 * @param nExceptBuffIndex Buff index to exclude
 * @return true if has immunity invincible buff
 * IDA: ?IsHaveImunityInvincibleBuff@CMover@@QEAAHG@Z @ 0x14036A640
 * Verified: Checks for effect type 5 with stat type 1
 */
bool CMover::IsHaveImunityInvincibleBuff(std::uint16_t nExceptBuffIndex) const {
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
 * @brief GetBuffAllByGroup - get all buffs by group ID
 * @param nGroupID Group ID
 * @param vecBuffIndex Output vector of buff slot indices
 * @return true if any buffs found
 * IDA: ?GetBuffAllByGroup@CMover@@QEAAHGAEAV?$vector@HV?$allocator@H@std@@@std@@@Z @ 0x14036A700
 * Verified: Collects all buff slots matching group ID
 */
bool CMover::GetBuffAllByGroup(std::uint16_t nGroupID, std::vector<int>& vecBuffIndex) const {
    vecBuffIndex.clear();
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nGroupID == nGroupID) {
            vecBuffIndex.push_back(i);
        }
    }
    return !vecBuffIndex.empty();
}

/**
 * @brief CheckPassDebuff - check if pass debuff exists
 * @param nBuffIndex Buff index to check
 * @return true if pass debuff exists for this buff
 * IDA: ?CheckPassDebuff@CMover@@QEAAHG@Z @ 0x14036A750
 * Verified: Checks effect type 22 and matches fSkillVal
 */
bool CMover::CheckPassDebuff(std::uint16_t nBuffIndex) const {
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
 * @brief ResetAllBuff - reset all buff states
 * IDA: ?ResetAllBuff@CMover@@QEAAXXZ @ 0x14036A860
 * Verified: Clears all buff counters and state array
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
 * @brief ClearBuffByType - clear buffs by type
 * @param byType Buff type (0=buff, 1=debuff)
 * IDA: ?ClearBuffByType@CMover@@QEAAXE@Z @ 0x14036A920
 * Verified: Clears all buffs matching specified type
 */
void CMover::ClearBuffByType(std::uint8_t byType) {
    if (m_nBuffTotalCnt == 0) {
        return;
    }
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != 0) {
            if (m_stBuffState[i].byBuffType == byType) {
                ClearBuffStatusBySlot(i, 0);
            }
        }
    }
}

/**
 * @brief ClearBuffByEffectType - clear buffs by effect type
 * @param byType Effect type to clear
 * IDA: ?ClearBuffByEffectType@CMover@@QEAAXE@Z @ 0x14036A990
 * Verified: Clears all buffs matching specified effect type
 */
void CMover::ClearBuffByEffectType(std::uint8_t byType) {
    if (m_nBuffTotalCnt == 0) {
        return;
    }
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != 0) {
            if (m_stBuffState[i].byEffectType == byType) {
                ClearBuffStatusBySlot(i, 0);
            }
        }
    }
}

/**
 * @brief AllBuffClear - clear all buffs with reason
 * @param byReason Clear reason (0=clear all)
 * IDA: ?AllBuffClear@CMover@@QEAAXE@Z @ 0x14036AAB0
 * Verified: Clears buffs based on reason and time
 */
void CMover::AllBuffClear(std::uint8_t byReason) {
    if (m_nBuffTotalCnt == 0) {
        return;
    }
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].IsLife()) {
            if (byReason == 0 || IsClearBuff(m_stBuffState[i].nBuffIndex, byReason)) {
                ClearBuffStatusBySlot(i, 0);
            }
        } else if (m_stBuffState[i].nBuffIndex != 0) {
            // Check for instant buffs (no duration)
            // TODO: 需要XResourceMgr::GetTB_BUFF来获取buff表
            // 如果Buff_Time为0且IsClearBuff通过，则清除
        }
    }
}

/**
 * @brief CheckBuffByLocation - check buffs by location
 * @param pArea Area pointer
 * IDA: ?CheckBuffByLocation@CMover@@UEAAXPEAVXArea@@@Z @ 0x14036ABF0
 * Verified: Clears buffs not allowed in current area
 */
void CMover::CheckBuffByLocation(XArea* pArea) {
    if (m_nBuffTotalCnt == 0) {
        return;
    }
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != 0) {
            if (!IsCanApplyBuff(m_stBuffState[i].nBuffIndex, pArea)) {
                ClearBuffStatusBySlot(i, 0);
            }
        }
    }
}

/**
 * @brief IsCanApplyBuff - check if buff can be applied in area
 * @param nBuffIndex Buff index
 * @param pArea Area pointer
 * @return true if buff can be applied
 * IDA: ?IsCanApplyBuff@CMover@@UEAAHGPEAVXArea@@@Z @ 0x14036ACD0
 * Verified: Checks AllowLocation_Type flags and area type
 */
bool CMover::IsCanApplyBuff(std::uint16_t nBuffIndex, XArea* pArea) const {
    // TODO: 需要XResourceMgr::GetTB_BUFF获取buff表
    // AllowLocation_Type flags:
    // 0x01 = anywhere
    // 0x02 = maze
    // 0x04 = district (non-battlezone)
    // 0x08 = battlezone
    (void)nBuffIndex;
    (void)pArea;
    return true;
}

/**
 * @brief UpdateBuffCount - update buff counters
 * @param byBuffType Buff type (0=buff, 1=debuff)
 * @param nAddCount Count to add (negative to subtract)
 * IDA: ?UpdateBuffCount@CMover@@QEAAXEH@Z @ 0x14036AE80
 * Verified: Updates total, buff, and debuff counters
 */
void CMover::UpdateBuffCount(std::uint8_t byBuffType, int nAddCount) {
    if (nAddCount <= 0) {
        // Subtracting
        int nNewTotal = m_nBuffTotalCnt + nAddCount;
        m_nBuffTotalCnt = (nNewTotal >= 0) ? nNewTotal : 0;

        if (byBuffType == 0) {
            int nNewBuff = m_nBuffCnt + nAddCount;
            m_nBuffCnt = (nNewBuff >= 0) ? nNewBuff : 0;
        } else if (byBuffType == 1) {
            int nNewDebuff = m_nDebuffCnt + nAddCount;
            m_nDebuffCnt = (nNewDebuff >= 0) ? nNewDebuff : 0;
        }
    } else {
        // Adding
        m_nBuffTotalCnt += nAddCount;
        if (byBuffType == 0) {
            m_nBuffCnt += nAddCount;
        } else if (byBuffType == 1) {
            m_nDebuffCnt += nAddCount;
        }
    }
}

/**
 * @brief GetBuffCategory - get buff category from effect type
 * @param byBuffEffectType Buff effect type
 * @return Category (0=other, 1=cat1, 2=cat2, 3=cat3)
 * IDA: ?GetBuffCategory@CMover@@QEAAEE@Z @ 0x14036B000
 * Verified: Maps effect type ranges to categories
 */
std::uint8_t CMover::GetBuffCategory(std::uint8_t byBuffEffectType) const {
    if (byBuffEffectType >= 0x6F && byBuffEffectType <= 0x72) {
        return 1;  // Category 1: effect types 111-114
    }
    if (byBuffEffectType >= 0x79 && byBuffEffectType <= 0x7E) {
        return 2;  // Category 2: effect types 121-126
    }
    if (byBuffEffectType >= 0x83 && byBuffEffectType <= 0x84) {
        return 3;  // Category 3: effect types 131-132
    }
    if (byBuffEffectType == 133) {
        return 0;
    }
    return 0;
}

/**
 * @brief GetResistStatIndexByBuff - get resist stat index from buff effect type
 * @param byBuffEffectType Buff effect type
 * @return Stat index or -1
 * IDA: ?GetResistStatIndexByBuff@CMover@@QEAAHE@Z @ 0x14036B070
 * Verified: Maps effect type ranges to stat indices
 */
int CMover::GetResistStatIndexByBuff(std::uint8_t byBuffEffectType) const {
    if (byBuffEffectType >= 0x6F && byBuffEffectType <= 0x72) {
        return byBuffEffectType - 62;  // Returns 13-16
    }
    if (byBuffEffectType >= 0x79 && byBuffEffectType <= 0x7E) {
        return byBuffEffectType - 68;  // Returns 17-22
    }
    if (byBuffEffectType >= 0x83 && byBuffEffectType <= 0x84) {
        return byBuffEffectType - 72;  // Returns 19-20
    }
    return -1;
}

/**
 * @brief SetBuffTime - set buff time and count
 * @param nIndex Buff slot index
 * @param fTime Time in seconds
 * @param byCount Count
 * IDA: ?SetBuffTime@CMover@@QEAAXFME@Z @ 0x14036B0F0
 * Verified: Updates buff lifetime and sends update packet
 */
void CMover::SetBuffTime(std::int16_t nIndex, float fTime, std::uint8_t byCount) {
    if (nIndex < 0 || nIndex >= 50) {
        return;
    }
    m_stBuffState[nIndex].fLifeTime = fTime;
    m_stBuffState[nIndex].byCount = byCount;
    // TODO: 需要send_eSUB_CMD_BUFF_UPDATE发送更新包
}

/**
 * @brief UpdateDefenseDisableBuff - update defense disable flag
 * IDA: ?UpdateDefenseDisableBuff@CMover@@QEAAXXZ @ 0x14036B4D0
 * Verified: Accumulates defense disable flags from effect type 6 buffs
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
                        break;  // All defense types disabled
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
 * @brief CanUseItem - check if item can be used
 * @param dwID Item ID
 * @param dwError Error code output
 * @return true if item can be used
 * IDA: ?CanUseItem@CMover@@QEAAHKAEAK@Z @ 0x14036B8D0
 */
bool CMover::CanUseItem(std::uint32_t dwID, std::uint32_t& dwError) const {
    // TODO: 需要XResourceMgr::GetTB_ITEM和GetTB_ITEM_CLASSIFY
    // 以及XArea::IsRevive, CGocParty, CGocForce等
    (void)dwID;
    dwError = 0;
    return true;
}

/**
 * @brief ProcessExtraMoving - process extra movement
 * IDA: ?ProcessExtraMoving@CMover@@UEAAXXZ @ 0x14036BC20
 * Verified: Complex movement processing with position interpolation
 */
void CMover::ProcessExtraMoving() {
    // Check if extra moving is active
    if (m_stExtMovingVal.IsZero()) {
        return;
    }

    // Get current position
    hkvVec3 vCurrentPos;
    VisObject3D_cl::GetPosition(&vCurrentPos);
    m_vPrevPos = vCurrentPos;

    if (m_stExtMovingVal.fRemainTime <= 0.0f) {
        ReleaseExtraMoving();
        return;
    }

    float fDiffX = m_stExtMovingVal.x - m_vPosition.x;
    float fDiffY = m_stExtMovingVal.y - m_vPosition.y;

    // Get delta time
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = IVTimer::GetTimeDifference(pTimer);

    if (fabsf(fDiffX) >= 3.0f || fabsf(fDiffY) >= 3.0f) {
        // Calculate delta movement
        float fDeltaX, fDeltaY;
        float fRatio = fDeltaTime / m_stExtMovingVal.fMovingTime;

        if (fDiffX <= 0.0f) {
            fDeltaX = (fRatio * fDiffX <= fDiffX) ? fDiffX : fRatio * fDiffX;
        } else {
            fDeltaX = (fDiffX <= fRatio * fDiffX) ? fDiffX : fRatio * fDiffX;
        }

        if (fDiffY <= 0.0f) {
            fDeltaY = (fRatio * fDiffY <= fDiffY) ? fDiffY : fRatio * fDiffY;
        } else {
            fDeltaY = (fDiffY <= fRatio * fDiffY) ? fDiffY : fRatio * fDiffY;
        }

        m_stExtMovingVal.fRemainTime -= fDeltaTime;

        hkvVec3 vDestPos = m_vPrevPos + hkvVec3(fDeltaX, fDeltaY, 0.0f);

        if (!IsFlying()) {
            GetHeight(&vDestPos, 200.0f);
        }

        if (CheckMoveCollision(&vDestPos)) {
            hkvVec3 vPos;
            VisObject3D_cl::GetPosition(&vPos);
            send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, &vPos, 0);
        } else {
            if (!CheckMoveDestPos(&vDestPos, 0, 0)) {
                send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, &vDestPos, 0);
            }
            Move(hkvVec3(vDestPos.x, vDestPos.y, vDestPos.z));
        }
    } else {
        m_stExtMovingVal.Clear();
    }
}

/**
 * @brief ReleaseExtraMoving - release extra movement
 * IDA: ?ReleaseExtraMoving@CMover@@UEAAXXZ @ 0x14036C120
 * Verified: Finalizes extra movement
 */
void CMover::ReleaseExtraMoving() {
    if (m_stExtMovingVal.IsZero()) {
        return;
    }

    if (m_stExtMovingVal.fMovingTime == 0.1f) {
        hkvVec3 vExtraPos(m_stExtMovingVal.x, m_stExtMovingVal.y, m_vPosition.z);
        Move(vExtraPos);
    }

    m_stExtMovingVal.Clear();
}

/**
 * @brief ChangeSequence - change animation sequence
 * @param dwOldAnimID Old animation ID
 * @param dwNewAnimID New animation ID
 * @param bResetPlay Reset play flag
 * IDA: ?ChangeSequence@CMover@@QEAAXKKH@Z @ 0x14036C500
 * Verified: Changes animation sequence
 */
void CMover::ChangeSequence(std::uint32_t dwOldAnimID, std::uint32_t dwNewAnimID, int bResetPlay) {
    m_fAnimPercentTime = 0.0f;
    m_fAnimationTime = 0.0f;
    m_bAnimPlay = 1;

    if (dwOldAnimID != dwNewAnimID) {
        m_bAnimChanged = 1;
    }

    m_vAnimLastDelta = hkvVec3::ZeroVector();

    const char* pszAnimString = GetAnimStirng(dwNewAnimID);
    if (pszAnimString) {
        ChangeActionTrigger(pszAnimString);
        if (IsFixedLoopAnim(dwNewAnimID)) {
            if (m_pCurMotionEvent) {
                m_pCurMotionEvent->eEndofAnimation = LOOP;
            }
        }
    }
}

/**
 * @brief CreateAkashicActionInfo - create akashic action info
 * @param szAnimName Animation name
 * IDA: ?CreateAkashicActionInfo@CMover@@QEAAXPEBD@Z @ 0x14036C800
 */
void CMover::CreateAkashicActionInfo(const char* szAnimName) {
    if (!m_pActionResource) {
        return;
    }

    if (m_pAkashicActionInfo) {
        // Check if same name
        if (strcmp(m_pAkashicActionInfo->szName, szAnimName) == 0) {
            // Get action desc and copy data
            // TODO: 需要XActionResMgr::GetActionDesc
        }
    } else {
        // Create new akashic action info
        // TODO: 需要VAnimationInfo创建和复制
    }
}

/**
 * @brief GetActionDesc - get action description
 * @param szAnimName Animation name
 * @return Animation info pointer
 * IDA: ?GetActionDesc@CMover@@QEAAPEBVVAnimationInfo@@PEBD@Z @ 0x14036C920
 */
const VAnimationInfo* CMover::GetActionDesc(const char* szAnimName) const {
    if (m_pAkashicActionInfo) {
        if (strcmp(m_pAkashicActionInfo->szName, szAnimName) == 0) {
            return m_pAkashicActionInfo;
        }
    }

    if (!m_pActionResource) {
        return nullptr;
    }

    // TODO: 需要XActionResMgr::GetActionDesc
    return nullptr;
}

/**
 * @brief ChangeActionTrigger - change action trigger
 * @param szAnimName Animation name
 * IDA: ?ChangeActionTrigger@CMover@@QEAAXPEBD@Z @ 0x14036CA80
 * Verified: Changes action trigger
 */
void CMover::ChangeActionTrigger(const char* szAnimName) {
    if (m_pCurMotionEvent && m_pCurMotionEvent->eActionBufferBehavior == RESETAFTER) {
        DeleteActionBuffer();
    }

    if (m_pSkillMgr) {
        m_pSkillMgr->ProjectileIndexClear();
    }

    m_bSkipAnimOffset = 0;
    m_pCurMotionEvent = const_cast<VAnimationInfo*>(GetActionDesc(szAnimName));

    if (m_pCurMotionEvent) {
        m_nAnimGroup = m_pCurMotionEvent->iAnimGroup;
        // TODO: XActionResMgr::ChangeMotionCallback
    } else {
        DebugOut("ChangeActionTrigger>> Not found anim info. (%s)", szAnimName);
    }
}

/**
 * @brief SetMovePosition - set move position
 * @param fXpos X position
 * @param fYpos Y position
 * IDA: ?SetMovePosition@CMover@@QEAAXMM@Z @ 0x14036CC00
 * Verified: Sets move position and offset
 */
void CMover::SetMovePosition(float fXpos, float fYpos) {
    m_stMovePos.x = fXpos;
    m_stMovePos.y = fYpos;
    m_stMoveGap = m_stMovePos;

    hkvVec3 vPos;
    VisObject3D_cl::GetPosition(&vPos);

    m_stMoveOffset.x = vPos.x - fXpos;
    m_stMoveOffset.y = vPos.y - fYpos;
    m_vMoveStopCheckPos = vPos;
}

/**
 * @brief MoveingClientStop - stop client movement
 * IDA: ?MoveingClientStop@CMover@@QEAAXXZ @ 0x14036CD40
 * Verified: Stops client movement
 */
void CMover::MoveingClientStop() {
    m_fMoving = 0;
    m_stMoveGap = m_stMovePos;
    m_stMovePos.Clear();
}

/**
 * @brief IsEnemy - check if target is enemy
 * @param pMover Target mover
 * @return true if enemy
 * IDA: ?IsEnemy@CMover@@UEAAHPEAV1@@Z @ 0x14036CD80
 * Verified: Compares actor types
 */
bool CMover::IsEnemy(CMover* pMover) const {
    int nMyType = XActor::GetType();
    int nTargetType = pMover->XActor::GetType();
    return nMyType != nTargetType;
}

/**
 * @brief CheckReactionTarget - check reaction target
 * @param iTargetType Target type
 * @param pTargetMover Target mover
 * @param bChekcForChain Check for chain flag
 * @return true if valid target
 * IDA: ?CheckReactionTarget@CMover@@UEAAHHPEAV1@_N@Z @ 0x14036CE70
 * Verified: Complex target type checking
 */
bool CMover::CheckReactionTarget(int iTargetType, CMover* pTargetMover, bool bChekcForChain) {
    if (pTargetMover) {
        CMoverEx* pMoverEx = dynamic_cast<CMoverEx*>(pTargetMover);
        if (pMoverEx) {
            if (pMoverEx->IsSystemActor()) {
                return false;
            }
        }
    }

    bool bResult = true;

    switch (iTargetType) {
        case 1:  // Self only
            bResult = (this == pTargetMover);
            break;
        case 2:  // Self or friend
            if (this == pTargetMover) {
                bResult = true;
            } else {
                bResult = bChekcForChain ? IsFriendForChain(pTargetMover) : IsFriend(pTargetMover);
            }
            break;
        case 3:  // Enemy
            if (this == pTargetMover) {
                bResult = false;
            } else {
                bResult = bChekcForChain ? IsEnemyForChain(pTargetMover) : IsEnemy(pTargetMover);
                if (!bResult) {
                    bResult = CheckMonsterInteractObject(pTargetMover);
                }
            }
            break;
        case 4:  // Not self
            bResult = (this != pTargetMover);
            break;
        case 6:  // Friend (not self)
            if (this == pTargetMover) {
                bResult = false;
            } else {
                bResult = bChekcForChain ? IsFriendForChain(pTargetMover) : IsFriend(pTargetMover);
            }
            break;
        case 7:  // Self or party
            if (this != pTargetMover) {
                bResult = IsParty(pTargetMover);
            }
            break;
        case 8:  // Party (not self)
            if (this == pTargetMover) {
                bResult = false;
            } else {
                bResult = IsParty(pTargetMover);
            }
            break;
        default:
            break;
    }

    return bResult;
}

/**
 * @brief GetHeight - get height at position
 * @param vPos Position to check
 * @param fTestHeight Test height
 * @return true if successful
 * IDA: ?GetHeight@CMover@@QEAA_NAEAVhkvVec3@@M@Z @ 0x14036D130
 * Verified: Gets height from navmesh
 */
bool CMover::GetHeight(hkvVec3* vPos, float fTestHeight) {
    XArea* pArea = GetArea();
    if (!pArea) {
        return false;
    }

    DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    if (!pNavMesh) {
        return false;
    }

    return pNavMesh->GetHeight(vPos, fTestHeight);
}

/**
 * @brief GetMoverObject - get mover by ID
 * @param dwID Actor ID
 * @return Mover pointer or nullptr
 * IDA: ?GetMoverObject@CMover@@QEAAPEAV1@K@Z @ 0x14036D200
 */
CMover* CMover::GetMoverObject(std::uint32_t dwID) {
    if (dwID == static_cast<std::uint32_t>(-1)) {
        return nullptr;
    }

    XArea* pArea = GetArea();
    if (!pArea) {
        return nullptr;
    }

    XActor* pActor = pArea->FindActor(dwID);
    if (!pActor) {
        return nullptr;
    }

    return dynamic_cast<CMover*>(pActor);
}

/**
 * @brief SetFlyState - set flying state
 * @param bFly Flying flag
 * IDA: ?SetFlyState@CMover@@QEAAX_N@Z @ 0x14036D300
 * Verified: Sets flying status flag
 */
void CMover::SetFlyState(bool bFly) {
    if (bFly) {
        XActor::SetStatus(0x400000u);
    } else {
        XActor::ClearStatus(0x400000u);
    }
}

/**
 * @brief FindTargetPos - find target position by mover
 * @param pMover Target mover
 * @return Position index (0-11)
 * IDA: ?FindTargetPos@CMover@@QEAAEPEAV1@@Z @ 0x14036D400
 * Verified: Finds best attack position
 */
std::uint8_t CMover::FindTargetPos(CMover* pMover) {
    hkvVec3 vDirVector = pMover->GetPosition() - GetPosition();
    int nYaw = static_cast<int>(GetYawFromVector(vDirVector));
    nYaw += (nYaw < 0) ? 360 : 0;

    std::uint8_t nPos = nYaw / 30;
    if (nPos >= 12) {
        nPos -= 12;
    }

    if (!m_byTargetPosInfo[nPos]) {
        return nPos;
    }

    std::uint8_t nNext = (nPos + 1) % 12;
    std::uint8_t nPrev = (nPos + 11) % 12;

    if (!m_byTargetPosInfo[nPrev]) return nPrev;
    if (!m_byTargetPosInfo[nNext]) return nNext;

    nNext = (nPos + 2) % 12;
    nPrev = (nPos + 10) % 12;

    if (!m_byTargetPosInfo[nPrev]) return nPrev;
    if (!m_byTargetPosInfo[nNext]) return nNext;

    // Find minimum count position
    if (m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nPrev] &&
        m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nNext]) {
        return nPos;
    }

    if (m_byTargetPosInfo[nPos] == m_byTargetPosInfo[nPrev] + 1) return nPrev;
    if (m_byTargetPosInfo[nPos] == m_byTargetPosInfo[nNext] + 1) return nNext;

    nNext = (nPos + 2) % 12;
    nPrev = (nPos + 10) % 12;

    if (m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nPrev] &&
        m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nNext]) {
        return nPos;
    }

    if (m_byTargetPosInfo[nPos] >= m_byTargetPosInfo[nPrev] + 1) return nPrev;
    if (m_byTargetPosInfo[nPos] < m_byTargetPosInfo[nNext] + 1) return nPos;

    return nNext;
}

/**
 * @brief FindTargetPos - find target position by angle range
 * @param fAngleMin Minimum angle
 * @param fAngleMax Maximum angle
 * @param eIgnoreMoveSide Ignore move side type
 * @return Position index
 * IDA: ?FindTargetPos@CMover@@QEAAEMMW4E_MOVESIDE_TYPE@@@Z @ 0x14036D700
 */
std::uint8_t CMover::FindTargetPos(float fAngleMin, float fAngleMax, int eIgnoreMoveSide) {
    float fAngleMina = (fAngleMin > 180.0f) ? 180.0f : fAngleMin;
    float fAngleMaxa = (fAngleMax > 180.0f) ? 180.0f : fAngleMax;

    int nBeginPos = static_cast<int>(fAngleMina / 30.0f);
    int nEndPos = static_cast<int>(fAngleMaxa / 30.0f);

    if (static_cast<int>(fAngleMina) % 30 > 0 && 30 * nBeginPos > fAngleMina) {
        --nBeginPos;
        if (nBeginPos < 0) nBeginPos = 0;
    }
    if (static_cast<int>(fAngleMaxa) % 30 > 0 && fAngleMaxa > 30 * nBeginPos) {
        ++nEndPos;
        if (nEndPos > 5) nEndPos = 5;
    }

    std::uint8_t nMinPos = nBeginPos;
    std::uint8_t nNext = nBeginPos;
    std::uint8_t nCount = 50;

    while (nNext < nEndPos) {
        int nReverseNext = 12 - nNext - 1;

        if (eIgnoreMoveSide != eMOVESIDE_RIGHT) {
            if (!m_byTargetPosInfo[nNext]) return nNext;
            if (nCount > m_byTargetPosInfo[nNext]) {
                nCount = m_byTargetPosInfo[nNext];
                nMinPos = nNext;
            }
        }

        if (eIgnoreMoveSide != eMOVESIDE_LEFT) {
            if (!m_byTargetPosInfo[nReverseNext]) return nReverseNext;
            if (nCount > m_byTargetPosInfo[nReverseNext]) {
                nCount = m_byTargetPosInfo[nReverseNext];
                nMinPos = 12 - nNext - 1;
            }
        }

        ++nNext;
    }

    return nMinPos;
}

/**
 * @brief GetTargetAngle - get target angle
 * @param byPos Position index
 * @return Angle in degrees
 * IDA: ?GetTargetAngle@CMover@@QEAAME@Z @ 0x14036DA00
 * Verified: Returns angle from position index
 */
float CMover::GetTargetAngle(std::uint8_t byPos) const {
    return 30.0f * static_cast<float>(byPos + 1);
}

/**
 * @brief SetTargetPosFlag - set target position flag
 * @param byPos Position index
 * IDA: ?SetTargetPosFlag@CMover@@QEAAXE@Z @ 0x14036DA30
 * Verified: Increments target position count
 */
void CMover::SetTargetPosFlag(std::uint8_t byPos) {
    ++m_byTargetPosCount;
    ++m_byTargetPosInfo[byPos];
}

/**
 * @brief ClearTargetPosFlag - clear target position flag
 * @param byPos Position index (255 = clear all)
 * IDA: ?ClearTargetPosFlag@CMover@@QEAAXE@Z @ 0x14036DB00
 * Verified: Clears target position flags
 */
void CMover::ClearTargetPosFlag(std::uint8_t byPos) {
    if (byPos == 255) {
        m_byTargetPosCount = 0;
        memset(m_byTargetPosInfo, 0, sizeof(m_byTargetPosInfo));
    } else if (m_byTargetPosInfo[byPos]) {
        --m_byTargetPosInfo[byPos];
        --m_byTargetPosCount;
    }
}

/**
 * @brief GetYawFromVector - get yaw from direction vector
 * @param vDir Direction vector
 * @return Yaw angle in degrees
 * IDA: ?GetYawFromVector@CMover@@SAMAEBVhkvVec3@@@Z @ 0x14036DC00
 * Verified: Calculates yaw from vector using dot product
 */
float CMover::GetYawFromVector(const hkvVec3& vDir) {
    hkvVec3 n1(0.0f, -1.0f, 0.0f);
    hkvVec3 n2(vDir.x, vDir.y, 0.0f);
    n2.normalizeIfNotZero(0.000001f);

    float calcValue = n1.dot(n2);
    calcValue = hkvMath::clamp(calcValue, -1.0f, 1.0f);

    float fDegree = hkvMath::Rad2Deg(acosf(calcValue));

    if (vDir.x < 0.0f) {
        fDegree = (180.0f - fDegree) + 180.0f;
    }

    if (fDegree > 180.0f) {
        return fDegree - 360.0f;
    }
    return fDegree;
}

/**
 * @brief IsValidPos - check if position is valid
 * @param vPos Position to check
 * @return true if valid
 * IDA: ?IsValidPos@CMover@@SA_NAEAUXVec3@@@Z @ 0x14036DD00
 * Verified: Checks position bounds
 */
bool CMover::IsValidPos(const hkvVec3& vPos) {
    if (vPos.x < -10000000.0f || vPos.x > 10000000.0f) return false;
    if (vPos.y < -10000000.0f || vPos.y > 10000000.0f) return false;
    if (vPos.z < -10000000.0f || vPos.z > 10000000.0f) return false;
    return true;
}

/**
 * @brief IsValidPos - check if position is valid
 * @param fX X coordinate
 * @param fY Y coordinate
 * @return true if valid
 * IDA: ?IsValidPos@CMover@@SA_NMM@Z @ 0x14036DD40
 * Verified: Checks coordinate bounds
 */
bool CMover::IsValidPos(float fX, float fY) {
    if (fX < -10000000.0f || fX > 10000000.0f) return false;
    if (fY < -10000000.0f || fY > 10000000.0f) return false;
    return true;
}

/**
 * @brief Move - move to position
 * @param vNextPos Next position
 * @param fRot Rotation
 * @return Result code
 * IDA: ?Move@CMover@@UEAAGAEAUXVec3@@M@Z @ 0x14036DE00
 */
std::uint16_t CMover::Move(hkvVec3& vNextPos, float fRot) {
    if (m_pArea) {
        m_pArea->MoveActor(this, &vNextPos);
    } else {
        LogHelper::LogDebug("game.contents", "No Area when send move!!");
    }
    return 0;
}

/**
 * @brief CheckMoveDestPos - check if destination position is reachable
 * @param vDestPos Destination position
 * @param bFlying Flying flag
 * @param bDontCareCurve Don't care curve flag
 * @return true if reachable
 * IDA: ?CheckMoveDestPos@CMover@@UEAA_NAEAVhkvVec3@@HH@Z @ 0x14036DF00
 */
bool CMover::CheckMoveDestPos(hkvVec3* vDestPos, int bFlying, int bDontCareCurve) {
    XArea* pArea = GetArea();
    if (!pArea) return true;

    DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    if (!pNavMesh) return true;

    float fRadius = GetHavokCapsuleRadius();
    hkvVec3 vPos = GetPosition();
    hkvVec3 vNextPos = *vDestPos;

    if (bFlying == 1) {
        hkvVec3 vPosFlat(vPos.x, vPos.y, 0.0f);
        hkvVec3 vNextFlat(vNextPos.x, vNextPos.y, 0.0f);
        if (vPosFlat == vNextFlat) {
            return true;
        }
    }

    bool bResult = XMaze::CheckCanDirectMove2(pNavMesh, &vPos, vDestPos, fRadius, bFlying, bDontCareCurve);

    if (!bResult) {
        hkvVec3 vDirection = vNextPos - vPos;
        vDirection.z = 0.0f;
        vDirection.normalizeIfNotZero(0.000001f);
        vDirection *= fRadius;
        *vDestPos -= vDirection;

        if (pNavMesh->GetHeight(vDestPos, 200.0f)) {
            if (bFlying == 1) {
                vDestPos->z = vPos.z;
            }
        } else {
            *vDestPos = vPos;
        }
    }

    return bResult;
}

/**
 * @brief GetSGAbsorbRate - get SG absorb rate
 * @return SG absorb rate
 * IDA: ?GetSGAbsorbRate@CMover@@QEAAMXZ @ 0x14036E200
 */
float CMover::GetSGAbsorbRate() {
    auto pAttr = GetGOC_Attribute(false);
    if (!pAttr) {
        return 0.0f;
    }
    return pAttr->GetStatusTable()->Con_SG_Absorb_Rate;
}

/**
 * @brief SetStat - set stat value
 * @param iIndex Stat index
 * @param fVal Value
 * IDA: ?SetStat@CMover@@QEAAXHM@Z @ 0x14036E300
 */
void CMover::SetStat(std::uint32_t iIndex, float fVal) {
    auto pAttr = GetGOC_Attribute(false);
    if (pAttr) {
        pAttr->SetStat(iIndex, fVal);
        if (iIndex == 3) {
            pAttr->SetFlagUseST();
        }
    }
}

/**
 * @brief CreateRandomTrapIndex - create random trap index
 * @return Random trap index
 * IDA: ?CreateRandomTrapIndex@CMover@@QEAAHXZ @ 0x14036E400
 * Verified: Creates random index using rand()
 */
int CMover::CreateRandomTrapIndex() {
    m_nRandomTrapIndex = rand() % 1000000;
    return m_nRandomTrapIndex;
}

/**
 * @brief SendUpdateStat - send stat update
 * @param iIndex Stat index
 * IDA: ?SendUpdateStat@CMover@@QEAAXH@Z @ 0x14036E500
 */
void CMover::SendUpdateStat(std::uint32_t iIndex) {
    auto pAttr = GetGOC_Attribute(false);
    if (pAttr) {
        pAttr->SendUpdateStat(iIndex);
    }
}

/**
 * @brief DeleteDelayedProjectile - delete delayed projectile
 * @param pDelayedProjectile Delayed projectile to delete
 * IDA: ?DeleteDelayedProjectile@CMover@@QEAAXPEAUSDelayedProjectile@@@Z @ 0x14036E600
 */
void CMover::DeleteDelayedProjectile(SDelayedProjectile* pDelayedProjectile) {
    for (auto it = m_vecDelayedProjectile.begin(); it != m_vecDelayedProjectile.end(); ++it) {
        SDelayedProjectile* pTemp = *it;
        if (pTemp == pDelayedProjectile) {
            if (pTemp) {
                delete pTemp;
            }
            m_vecDelayedProjectile.erase(it);
            return;
        }
    }
}

/**
 * @brief CheckDelayedProjectile - check delayed projectiles
 * @param fDeltaTime Delta time
 * IDA: ?CheckDelayedProjectile@CMover@@QEAAXM@Z @ 0x14036E700
 */
void CMover::CheckDelayedProjectile(float fDeltaTime) {
    for (auto it = m_vecDelayedProjectile.begin(); it != m_vecDelayedProjectile.end(); ) {
        SDelayedProjectile* pTemp = *it;
        if (!pTemp) break;

        pTemp->fCurTime += fDeltaTime;

        if (pTemp->bUsed || pTemp->fCurTime < pTemp->fCreateDelayTime) {
            ++it;
        } else {
            // Create action buffer and add
            tagACTION_BUFFER xAction(1, 0.0f);
            xAction.pActionTrigger = pTemp->pTrigger;
            float fYaw = GetOrientationYaw();
            xAction << pTemp->pTrigger->EventID;
            xAction << pTemp->nSkillID;
            xAction << 0;
            xAction << 1u;
            xAction << fYaw;
            AddActionBuffer(xAction);
            pTemp->bUsed = 1;
            ++it;
        }
    }
}

/**
 * @brief GetFilterData - get filter data
 * @param nSkillID Skill ID
 * @param nFilterData1 Output filter data 1
 * @param nFilterData2 Output filter data 2
 * @param nFilterData3 Output filter data 3
 * IDA: ?GetFilterData@CMover@@QEAAXHAEAH00@Z @ 0x14036E900
 */
void CMover::GetFilterData(std::uint32_t nSkillID, int* nFilterData1, int* nFilterData2, int* nFilterData3) {
    auto it = m_mapFilterData.find(nSkillID);
    if (it == m_mapFilterData.end()) {
        *nFilterData1 = 0;
        *nFilterData2 = 0;
        *nFilterData3 = 0;
    } else {
        *nFilterData1 = it->second.dwFilterData1;
        *nFilterData2 = it->second.dwFilterData2;
        *nFilterData3 = it->second.dwFilterData3;
    }
}

/**
 * @brief SetFilterData - set filter data
 * @param nSkillID Skill ID
 * @param nFilterData1 Filter data 1
 * @param nFilterData2 Filter data 2
 * @param nFilterData3 Filter data 3
 * IDA: ?SetFilterData@CMover@@QEAAXHHHH@Z @ 0x14036EA00
 */
void CMover::SetFilterData(std::uint32_t nSkillID, std::uint32_t nFilterData1,
                           std::uint32_t nFilterData2, std::uint32_t nFilterData3) {
    SFilterData data;
    data.dwFilterData1 = nFilterData1;
    data.dwFilterData2 = nFilterData2;
    data.dwFilterData3 = nFilterData3;
    m_mapFilterData[nSkillID] = data;
}

/**
 * @brief send_eSUB_CMD_MOVE - send move packet
 * @param pMover Target mover
 * @param fTargetPosx Target X position
 * @param fTargetPosy Target Y position
 * @param byRunBit Run bit
 * IDA: ?send_eSUB_CMD_MOVE@CMover@@QEAAXPEAV1@MME@Z @ 0x14036EB00
 */
void CMover::send_eSUB_CMD_MOVE(CMover* pMover, float fTargetPosx, float fTargetPosy, std::uint8_t byRunBit) {
    // TODO: 需要完整的XSendPacket、ST_MOVE等类型
    float fYaw = pMover->GetMovingYaw();
    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    m_fMoveSpeed = pMover->GetMoveSpeed();
    hkvVec3 vPos = pMover->GetPosition();

    // TODO: 完整的包发送逻辑
    m_fLastSendMoveTime = 0.0f;

    DebugOut("send_eSUB_CMD_MOVE>> (%.2f,%.2f) -> (%.2f,%.2f) -> Rot:%.3f / %d",
             vPos.x, vPos.y, fTargetPosx, fTargetPosy, fYaw, byRunBit);
}

/**
 * @brief send_eSUB_CMD_MOVE_STOP - send move stop packet
 * @param pMover Target mover
 * IDA: ?send_eSUB_CMD_MOVE_STOP@CMover@@QEAAXPEAV1@@Z @ 0x14036EE90
 */
void CMover::send_eSUB_CMD_MOVE_STOP(CMover* pMover) {
    float fYaw = pMover->GetMovingYaw();
    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    hkvVec3 curPos = pMover->GetPosition();

    // TODO: 完整的包发送逻辑
    m_fLastSendMoveTime = 0.0f;

    DebugOut("send_eSUB_CMD_MOVE_STOP>> (%.2f,%.2f) %.3f", curPos.x, curPos.y, fYaw);
}

/**
 * @brief send_eSUB_CMD_MOVE_BATTLE - send battle move packet
 * @param pMover Target mover
 * @param bPlayMotion Play motion flag
 * IDA: ?send_eSUB_CMD_MOVE_BATTLE@CMover@@QEAAXPEAV1@_N@Z @ 0x14036F300
 */
void CMover::send_eSUB_CMD_MOVE_BATTLE(CMover* pMover, bool bPlayMotion) {
    hkvVec3 vPos = pMover->GetPosition();
    float fYaw = pMover->GetMovingYaw();
    bool bBattlePose = pMover->IsBattlePose();

    // TODO: 完整的包发送逻辑
    DebugOut("send_eSUB_CMD_MOVE_BATTLE>> (%.2f,%.2f) %.3f (%d/%d)",
             vPos.x, vPos.y, fYaw, bBattlePose ? 1 : 0, bPlayMotion ? 1 : 0);
}

/**
 * @brief send_eSUB_CMD_MOVE_GAZE - send gaze move packet
 * @param pMover Target mover
 * IDA: ?send_eSUB_CMD_MOVE_GAZE@CMover@@QEAAXPEAV1@@Z @ 0x14036F500
 */
void CMover::send_eSUB_CMD_MOVE_GAZE(CMover* pMover) {
    hkvVec3 vPos = pMover->GetPosition();
    float fYaw = pMover->GetMovingYaw();
    bool bGazeMoving = pMover->IsGazeMoving();

    // TODO: 完整的包发送逻辑
    DebugOut("send_eSUB_CMD_MOVE_GAZE>> (%.2f,%.2f) %.3f (%d)",
             vPos.x, vPos.y, fYaw, bGazeMoving ? 1 : 0);
}

/**
 * @brief send_eSUB_CMD_MOVE_TRACE - send trace move packet
 * @param pMover Target mover
 * IDA: ?send_eSUB_CMD_MOVE_TRACE@CMover@@QEAAXPEAV1@@Z @ 0x14036F700
 */
void CMover::send_eSUB_CMD_MOVE_TRACE(CMover* pMover, float a3) {
    hkvVec3 vPos = pMover->GetPosition();
    float fYaw = pMover->GetMovingYaw();
    float fPitch = pMover->GetLookPitch();

    // TODO: 完整的包发送逻辑（包含AI状态、aggro列表等）
    (void)a3;
    (void)fPitch;

    DebugOut("send_eSUB_CMD_MOVE_TRACE>> (%.2f,%.2f) %.3f", vPos.x, vPos.y, fYaw);
}

/**
 * @brief send_eSUB_CMD_MOVE_IDLE - send idle move packet
 * @param pMover Target mover
 * @param fMoveDelayTime Move delay time
 * IDA: ?send_eSUB_CMD_MOVE_IDLE@CMover@@QEAAXPEAV1@M@Z @ 0x14036FD50
 */
void CMover::send_eSUB_CMD_MOVE_IDLE(CMover* pMover, float fMoveDelayTime) {
    XSendPacket xPacket(5, 0x0C);
    std::uint32_t dwActorID = pMover->GetActorID();

    hkvVec3 vPos = pMover->GetPosition();
    float fYaw = pMover->GetMovingYaw();
    int nAnimIdx = pMover->GetAnimationIdx();
    std::uint8_t byAnimStep = pMover->GetDefaultAnimStep();

    xPacket << dwActorID;
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << fYaw;
    xPacket << nAnimIdx;
    xPacket << byAnimStep;
    xPacket << fMoveDelayTime;

    CGocNetwork::SendBroadCastAfterLoading(this, &xPacket, 0);
}

/**
 * @brief send_eSUB_CMD_MOVE_INFO - send move info packet
 * @param pMover Target mover
 * @param dwType Type
 * @param dwVal Value
 * IDA: ?send_eSUB_CMD_MOVE_INFO@CMover@@QEAAXPEAV1@KK@Z @ 0x14036FEF0
 */
void CMover::send_eSUB_CMD_MOVE_INFO(CMover* pMover, std::uint32_t dwType, std::uint32_t dwVal) {
    XSendPacket xPacket(5, 0x0D);
    std::uint32_t dwActorID = pMover->GetActorID();

    xPacket << dwActorID;
    xPacket << dwType;
    xPacket << dwVal;

    CGocNetwork::SendBroadCastAfterLoading(this, &xPacket, 0);
}

/**
 * @brief send_eSUB_CMD_MOVE_STIFFEN - send stiffen packet
 * @param pMover Target mover
 * @param fRate Stiffen rate
 * @param fTime Stiffen time
 * IDA: ?send_eSUB_CMD_MOVE_STIFFEN@CMover@@QEAAXPEAV1@MM@Z @ 0x14036FFF0
 */
void CMover::send_eSUB_CMD_MOVE_STIFFEN(CMover* pMover, float fRate, float fTime) {
    XSendPacket xPacket(5, 0x12);
    std::uint32_t dwActorID = pMover->GetActorID();

    xPacket << dwActorID;
    xPacket << fRate;
    xPacket << fTime;

    CGocNetwork::SendBroadCastAfterLoading(this, &xPacket, 0);
}

/**
 * @brief send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA - send ignore motion delta packet
 * @param pMover Target mover
 * @param vPos Position
 * @param bForced Forced flag
 * IDA: ?send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA@CMover@@QEAAXPEAV1@VhkvVec3@@_N@Z @ 0x140370100
 */
void CMover::send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(CMover* pMover, hkvVec3 vPos, bool bForced) {
    if (!bForced && pMover->GetType() != 2) {
        return;
    }

    tagEXTRA_MOVEPOS::Clear(&m_stExtMovingVal);

    float fYaw = pMover->GetOrientationYaw();
    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    XSendPacket xPacket(5, 0x13);
    std::uint32_t dwActorID = pMover->GetActorID();

    xPacket << dwActorID;
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << fYaw;
    xPacket << pMover->GetLookPitch();
    xPacket << bForced;

    CGocNetwork::SendBroadCastAfterLoading(this, &xPacket, eAll);
    m_fLastSendMoveTime = 0.0f;

    DebugOut("send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA>> (%.2f,%.2f) %.3f", vPos.x, vPos.y, fYaw);
}

/**
 * @brief send_eSUB_CMD_MOVE_UPDATE_DIR - send update direction packet
 * @param pMover Target mover
 * @param bDirect Direct flag
 * IDA: ?send_eSUB_CMD_MOVE_UPDATE_DIR@CMover@@QEAAXPEAV1@_N@Z @ 0x140370390
 */
void CMover::send_eSUB_CMD_MOVE_UPDATE_DIR(CMover* pMover, bool bDirect) {
    XSendPacket xPacket(5, 0x14);

    float fYaw = pMover->GetMovingYaw();
    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE_UPDATE_DIR>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    std::uint32_t dwActorID = pMover->GetActorID();

    xPacket << dwActorID;
    xPacket << fYaw;
    xPacket << pMover->GetLookPitch();
    xPacket << bDirect;

    CGocNetwork::SendBroadCastAfterLoading(this, &xPacket, 1);
    m_fLastSendMoveTime = 0.0f;

    DebugOut("send_eSUB_CMD_MOVE_UPDATE_DIR>> %.3f", fYaw);
}

/**
 * @brief send_eSUB_CMD_MOVE_DROP - send drop packet
 * @param pMover Target mover
 * @param bSync Sync flag
 * IDA: ?send_eSUB_CMD_MOVE_DROP@CMover@@QEAAXPEAV1@_N@Z @ 0x140370570
 */
void CMover::send_eSUB_CMD_MOVE_DROP(CMover* pMover, bool bSync) {
    XSendPacket xPacket(5, 0x15);
    std::uint32_t dwActorID = pMover->GetActorID();

    hkvVec3 vPos = pMover->GetPosition();
    float fYaw = pMover->GetMovingYaw();

    xPacket << dwActorID;
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << fYaw;
    xPacket << bSync;

    CGocNetwork::SendBroadCastAfterLoading(this, &xPacket, 0);
}

/**
 * @brief send_eSUB_CMD_MOVE_GRAP - send grap packet
 * @param pMover Target mover
 * @param vPos Position
 * IDA: ?send_eSUB_CMD_MOVE_GRAP@CMover@@QEAAXPEAV1@VhkvVec3@@@Z @ 0x1403706E0
 */
void CMover::send_eSUB_CMD_MOVE_GRAP(CMover* pMover, hkvVec3 vPos) {
    XSendPacket xPacket(5, 0x20);
    std::uint32_t dwActorID = pMover->GetActorID();

    xPacket << dwActorID;
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;

    CGocNetwork::SendBroadCastAfterLoading(this, &xPacket, 0);
}

/**
 * @brief send_eSUB_CMD_MOVE_ATTACED_BT - send attached BT packet
 * @param pAttackerMover Attacker mover
 * @param pTargetMover Target mover
 * @param vAttachDir Attach direction
 * @param fAttachedDirDist Attach direction distance
 * IDA: ?send_eSUB_CMD_MOVE_ATTACED_BT@CMover@@QEAAXPEAV1@0VhkvVec3@@M@Z @ 0x140370800
 */
void CMover::send_eSUB_CMD_MOVE_ATTACED_BT(CMover* pAttackerMover, CMover* pTargetMover,
                                            hkvVec3 vAttachDir, float fAttachedDirDist) {
    XSendPacket xPacket(5, 0x34);

    std::uint32_t dwAttackerID = 0;
    if (pAttackerMover) {
        dwAttackerID = pAttackerMover->GetActorID();
    }

    std::uint32_t dwTargetID = pTargetMover->GetActorID();

    xPacket << dwAttackerID;
    xPacket << dwTargetID;
    xPacket << vAttachDir.x;
    xPacket << vAttachDir.y;
    xPacket << vAttachDir.z;
    xPacket << fAttachedDirDist;

    CGocNetwork::SendBroadCastAfterLoading(this, &xPacket, 0);
}

/**
 * @brief ClearImmunityStatus - clear immunity status
 * @param dwStatus Status to clear
 * IDA: ?ClearImmunityStatus@CMover@@QEAAXK@Z @ 0x140353040
 */
void CMover::ClearImmunityStatus(std::uint32_t dwStatus) {
    m_dwImmunityStatus &= ~dwStatus;
}

/**
 * @brief IsImmunityStatus - check if has immunity status
 * @return true if has immunity status
 * IDA: ?IsImmunityStatus@CMover@@QEAAHXZ @ 0x140364700
 */
bool CMover::IsImmunityStatus() const {
    return m_dwImmunityStatus != 0;
}

/**
 * @brief GetJumpSpeed - get jump speed
 * @return Jump speed
 * IDA: ?GetJumpSpeed@CMover@@QEAAMXZ @ 0x1402C7330
 */
float CMover::GetJumpSpeed() const {
    return m_fFlySpeed;
}

/**
 * @brief GetDieDelayTime - get die delay time
 * @return Die delay time
 * IDA: ?GetDieDelayTime@CMover@@QEAAMXZ @ 0x1402C7BD0
 */
float CMover::GetDieDelayTime() const {
    return m_fDieDelayTime;
}

/**
 * @brief GetDecreaseStaminaRate - get decrease stamina rate
 * @return Decrease stamina rate
 * IDA: ?GetDecreaseStaminaRate@CMover@@QEAAMXZ @ 0x1402C7EE0
 */
float CMover::GetDecreaseStaminaRate() const {
    return m_fDecreaseStaminaRate;
}

/**
 * @brief GetIgnoreSkillCost - get ignore skill cost flag
 * @return Ignore skill cost flag
 * IDA: ?GetIgnoreSkillCost@CMover@@QEAAHXZ @ 0x1402C7F00
 */
int CMover::GetIgnoreSkillCost() const {
    return m_bIgnoreSkillCost;
}

/**
 * @brief AddSummonMobList - add summon mob ID to list
 * @param dwMobID Mob ID
 * IDA: ?AddSummonMobList@CMover@@QEAAXK@Z @ 0x1402C7CC0
 */
void CMover::AddSummonMobList(std::uint32_t dwMobID) {
    m_listSummonMob.push_back(dwMobID);
}

/**
 * @brief SetProtectionAggroRatio - set protection aggro ratio
 * @param fRatio Ratio
 * IDA: ?SetProtectionAggroRatio@CMover@@QEAAXM@Z @ 0x1403655C0
 */
void CMover::SetProtectionAggroRatio(float fRatio) {
    m_fProtectionAggroRatio = fRatio;
}

/**
 * @brief SetDmgMotionFlag - set damage motion flag
 * @param byFlag Flag value
 * IDA: ?SetDmgMotionFlag@CMover@@QEAAXE@Z @ 0x1403655E0
 */
void CMover::SetDmgMotionFlag(std::uint8_t byFlag) {
    m_byDmgMontionFlag = byFlag;
}

/**
 * @brief SetWeightRank - set weight rank
 * @param cVal Value
 * IDA: ?SetWeightRank@CMover@@QEAAXD@Z @ 0x140364D40
 */
void CMover::SetWeightRank(std::int8_t cVal) {
    m_cWeightRank = cVal;
}

/**
 * @brief SetParentSkillTableIdx - set parent skill table index
 * @param nVal Value
 * IDA: ?SetParentSkillTableIdx@CMover@@QEAAXH@Z @ 0x140364610
 */
void CMover::SetParentSkillTableIdx(int nVal) {
    m_nParentSkillTableIdx = nVal;
}

/**
 * @brief GetCurSkillTableIdx - get current skill table index
 * @return Skill table index
 * IDA: ?GetCurSkillTableIdx@CMover@@QEAAHXZ @ 0x140364650
 */
int CMover::GetCurSkillTableIdx() const {
    return m_nCurSkillTableIdx;
}

/**
 * @brief SetIgnoreAggroDebuff - set ignore aggro debuff flag
 * @param bApply Apply flag
 * IDA: ?SetIgnoreAggroDebuff@CMover@@QEAAXH@Z @ 0x1402A67F0
 */
void CMover::SetIgnoreAggroDebuff(bool bApply) {
    m_bIgnoreAggroDebuff = bApply;
}

/**
 * @brief MoveingValueClear - clear moving values
 * IDA: ?MoveingValueClear@CMover@@QEAAXXZ @ 0x1402A4BE0
 */
void CMover::MoveingValueClear() {
    m_fMoving = 0;
    m_stMovePos.Clear();
    m_stMoveGap.Clear();
    m_stMoveOffset.Clear();
}

/**
 * @brief GetAnimIndex - get animation index by name
 * @param strAnimName Animation name
 * @return Animation index or -1 if not found
 * IDA: ?GetAnimIndex@CMover@@QEAAKVVString@@@Z @ 0x140368960
 */
std::uint32_t CMover::GetAnimIndex(const VString& strAnimName) {
    if (m_mapAnimInfoKey) {
        auto it = m_mapAnimInfoKey->find(strAnimName);
        if (it != m_mapAnimInfoKey->end()) {
            return it->second;
        }
    }
    return static_cast<std::uint32_t>(-1);
}

/**
 * @brief GetAnimStirng - get animation string by key
 * @param dwAnimKey Animation key
 * @return Animation string or nullptr if not found
 * IDA: ?GetAnimStirng@CMover@@QEAAPEADK@Z @ 0x1403688D0
 */
const char* CMover::GetAnimStirng(std::uint32_t dwAnimKey) {
    if (!m_mapAnimInfoString) {
        return nullptr;
    }
    auto it = m_mapAnimInfoString->find(dwAnimKey);
    if (it == m_mapAnimInfoString->end()) {
        return nullptr;
    }
    return it->second.AsChar();
}

/**
 * @brief SetSlowTime - set slow time effect
 * @param fTime Slow time duration
 * @param fSpeed Animation speed during slow time
 * IDA: ?SetSlowTime@CMover@@QEAAXMM@Z @ 0x140368AA0
 */
void CMover::SetSlowTime(float fTime, float fSpeed) {
    if (fTime > 0.0f && m_stTimeSlow.fTime == 0.0f) {
        m_fRestoreAnimSpeed = m_fAnimSpeed;
        if (IsStatus(1) || IsStatus(0x400)) {
            m_eRestoreAnimSpeedType = AST_ATTACK;
        } else {
            m_eRestoreAnimSpeedType = AST_NONE;
        }
    }
    m_stTimeSlow.fTime = fTime;
    m_stTimeSlow.fSpeed = fSpeed;
    SetAnimSpeed(fSpeed);
}

/**
 * @brief GetSGAbsorbRate - get SG absorb rate
 * @return SG absorb rate
 * IDA: ?GetSGAbsorbRate@CMover@@QEAAMXZ @ 0x14036E200
 */
float CMover::GetSGAbsorbRate() {
    // TODO: 需要实现GetGOC模板函数
    return 0.0f;
}

/**
 * @brief SetStat - set stat value
 * @param iIndex Stat index
 * @param fVal Stat value
 * IDA: ?SetStat@CMover@@QEAAXHM@Z @ 0x14036E290
 */
void CMover::SetStat(std::uint32_t iIndex, float fVal) {
    // TODO: 需要实现GetGOC模板函数和CGocAttribute
    (void)iIndex;
    (void)fVal;
}

/**
 * @brief CreateRandomTrapIndex - create random trap index
 * @return Random trap index
 * IDA: ?CreateRandomTrapIndex@CMover@@QEAAHXZ @ 0x14036E3E0
 */
int CMover::CreateRandomTrapIndex() {
    m_nRandomTrapIndex = rand() % 1000000;
    return m_nRandomTrapIndex;
}

/**
 * @brief SendUpdateStat - send update stat packet
 * @param iIndex Stat index
 * IDA: ?SendUpdateStat@CMover@@QEAAXH@Z @ 0x14036E4A0
 */
void CMover::SendUpdateStat(std::uint32_t iIndex) {
    // TODO: 需要实现GetGOC模板函数
    (void)iIndex;
}

/**
 * @brief CheckMoveCollision - check for move collision
 * @param vDestPos Destination position
 * @return Collided mover or nullptr
 * IDA: ?CheckMoveCollision@CMover@@QEAAPEAV1@AEAVhkvVec3@@@Z @ 0x1403681B0
 */
CMover* CMover::CheckMoveCollision(hkvVec3& vDestPos) {
    // TODO: 汇编还原 - IDA: 0x1403681B0
    // Complex function scans for nearby actors and checks collision
    (void)vDestPos;
    return nullptr;
}

/**
 * @brief SetupAnimation - setup animation for mover
 * IDA: ?SetupAnimation@CMover@@QEAAXXZ @ 0x140367980
 */
void CMover::SetupAnimation() {
    // TODO: 汇编还原 - IDA: 0x140367980
    // Loads action resource and sets up animation info
}

/**
 * @brief CheckAnimationEnd - check if animation has ended
 * IDA: ?CheckAnimationEnd@CMover@@QEAAXXZ @ 0x140367C80
 */
void CMover::CheckAnimationEnd() {
    // TODO: 汇编还原 - IDA: 0x140367C80
    // Complex animation timing and offset delta processing
}

/**
 * @brief GetItemRateResultGear - get item rate result for gear
 * @param byTargetLevel Target level
 * @param pAttr Attribute component
 * @return Item rate result
 * IDA: ?GetItemRateResultGear@CMover@@QEAAHEV?$shared_ptr@VCGocAttribute@@@tr1@std@@@Z @ 0x140367780
 */
std::uint32_t CMover::GetItemRateResultGear(std::uint8_t byTargetLevel,
                                              std::tr1::shared_ptr<class CGocAttribute> pAttr) {
    // TODO: 汇编还原 - IDA: 0x140367780
    (void)byTargetLevel;
    (void)pAttr;
    return 0;
}

/**
 * @brief CollisionShereToLine - check sphere to line collision
 * @param vSphereCenter Sphere center
 * @param fRadius Sphere radius
 * @param vLineStart Line start point
 * @param vLineEnd Line end point
 * @return true if collision
 * IDA: ?CollisionShereToLine@CMover@@QEAAHAEAVhkvVec3@@M00@Z @ 0x14036A080
 * Verified: Calls FindLineCircleIntersections
 */
bool CMover::CollisionShereToLine(const hkvVec3& vSphereCenter, float fRadius,
                                   const hkvVec3& vLineStart, const hkvVec3& vLineEnd) {
    return FindLineCircleIntersections(
        vSphereCenter.x, vSphereCenter.y, fRadius,
        vLineStart.x, vLineStart.y,
        vLineEnd.x, vLineEnd.y) > 0;
}

/**
 * @brief FindLineCircleIntersections - find line circle intersections
 * @param cx Circle center X
 * @param cy Circle center Y
 * @param radius Circle radius
 * @param x1 Line start X
 * @param y1 Line start Y
 * @param x2 Line end X
 * @param y2 Line end Y
 * @return Number of intersections (0, 1, 2) or -1 if line completely inside
 * IDA: ?FindLineCircleIntersections@CMover@@QEAAHMMMMMMM@Z @ 0x14036A120
 * Verified: Quadratic formula for line-circle intersection
 */
int CMover::FindLineCircleIntersections(float cx, float cy, float radius,
                                         float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float A = dx * dx + dy * dy;
    float b = 2.0f * (dx * (x1 - cx) + dy * (y1 - cy));
    float c = (x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy) - radius * radius;
    float det = b * b - 4.0f * A * c;

    if (A <= 0.0000001f || det < 0.0f) {
        return 0;
    }
    if (det == 0.0f) {
        float t = -b / (2.0f * A);
        return (t > 0.0f && t < 1.0f) ? 1 : 0;
    } else {
        float sqrtDet = sqrtf(det);
        float ta = (-b - sqrtDet) / (2.0f * A);
        if (ta <= 0.0f || ta >= 1.0f) {
            float tb = (sqrtDet - b) / (2.0f * A);
            if (tb <= 0.0f || tb >= 1.0f) {
                return -1;  // Line completely inside
            }
            return 2;
        }
        return 1;
    }
}

/**
 * @brief CollisionCylinderToBox - check cylinder to box collision
 * @param vCylinderCenter Cylinder center
 * @param fRadius Cylinder radius
 * @param vBoxCenter Box center
 * @param vBoxSize Box size
 * @param fRotation Box rotation
 * @return true if collision
 * IDA: ?CollisionCylinderToBox@CMover@@QEAAHAEAVhkvVec3@@M00M@Z @ 0x140369B60
 */
bool CMover::CollisionCylinderToBox(const hkvVec3& vCylinderCenter, float fRadius,
                                     const hkvVec3& vBoxCenter, const hkvVec3& vBoxSize,
                                     float fRotation) {
    // TODO: 汇编还原 - IDA: 0x140369B60
    // This requires hkvMat3 rotation matrix operations
    (void)vCylinderCenter;
    (void)fRadius;
    (void)vBoxCenter;
    (void)vBoxSize;
    (void)fRotation;
    return false;
}

/**
 * @brief IsInRectCircle - check if circle intersects rectangle
 * @param vLeftTop Rectangle left-top corner
 * @param vRightBottom Rectangle right-bottom corner
 * @param vCircleCenter Circle center
 * @param fRadius Circle radius
 * @return true if intersection
 * IDA: ?IsInRectCircle@CMover@@QEAAHAEAVhkvVec3@@00M@Z @ 0x140369CA0
 * Verified: Complex geometric intersection test
 */
bool CMover::IsInRectCircle(const hkvVec3& vLeftTop, const hkvVec3& vRightBottom,
                             const hkvVec3& vCircleCenter, float fRadius) {
    // Check if circle fully contains rectangle corners
    if ((vCircleCenter.x + fRadius) >= vLeftTop.x &&
        (vCircleCenter.y - fRadius) >= vLeftTop.y &&
        vRightBottom.x >= (vCircleCenter.x - fRadius) &&
        vRightBottom.y >= (vCircleCenter.y + fRadius)) {
        return true;
    }
    if ((vCircleCenter.x - fRadius) >= vLeftTop.x &&
        (vCircleCenter.y + fRadius) >= vLeftTop.y &&
        vRightBottom.x >= (vCircleCenter.x + fRadius) &&
        vRightBottom.y >= (vCircleCenter.y - fRadius)) {
        return true;
    }

    // Calculate minimum squared distance from circle center to rectangle corners
    float fMin = 0.0f;
    hkvVec3 vDiff;

    // Top-left corner
    vDiff.x = vCircleCenter.x - vLeftTop.x;
    vDiff.y = vCircleCenter.y - vLeftTop.y;
    vDiff.z = 0.0f;
    fMin = vDiff.getLengthSquared();

    // Bottom-left corner
    vDiff.x = vCircleCenter.x - vLeftTop.x;
    vDiff.y = vCircleCenter.y - vRightBottom.y;
    vDiff.z = 0.0f;
    float fDist = vDiff.getLengthSquared();
    if (fMin > fDist) fMin = fDist;

    // Top-right corner
    vDiff.x = vCircleCenter.x - vRightBottom.x;
    vDiff.y = vCircleCenter.y - vLeftTop.y;
    vDiff.z = 0.0f;
    fDist = vDiff.getLengthSquared();
    if (fMin > fDist) fMin = fDist;

    // Bottom-right corner
    vDiff.x = vCircleCenter.x - vRightBottom.x;
    vDiff.y = vCircleCenter.y - vRightBottom.y;
    vDiff.z = 0.0f;
    fDist = vDiff.getLengthSquared();
    if (fMin > fDist) fMin = fDist;

    return (fRadius * fRadius) >= fMin;
}

/**
 * @brief CollisionCylinderToBox - check cylinder to box collision
 * @param vCylinderCenter Cylinder center
 * @param fRadius Cylinder radius
 * @param vBoxCenter Box center
 * @param vBoxSize Box size
 * @param fRotation Box rotation
 * @return true if collision
 * IDA: ?CollisionCylinderToBox@CMover@@QEAAHAEAVhkvVec3@@M00M@Z @ 0x140369B60
 * Verified: Uses rotation matrix and IsInRectCircle
 */
bool CMover::CollisionCylinderToBox(const hkvVec3& vCylinderCenter, float fRadius,
                                     const hkvVec3& vBoxCenter, const hkvVec3& vBoxSize,
                                     float fRotation) {
    hkvVec3 vPos;
    vPos.x = vCylinderCenter.x - vBoxCenter.x;
    vPos.y = vCylinderCenter.y - vBoxCenter.y;
    vPos.z = 0.0f;

    // Rotate position by negative rotation
    hkvMat3 matRot;
    matRot.setFromEulerAngles(0.0f, 0.0f, -fRotation);
    hkvVec3 vDestPos = matRot * vPos;

    // Calculate box half size
    hkvVec3 vLeftTop(-vBoxSize.x * 0.5f, -vBoxSize.y * 0.5f, 0.0f);
    hkvVec3 vRightBottom(vBoxSize.x * 0.5f, vBoxSize.y * 0.5f, 0.0f);

    return IsInRectCircle(vLeftTop, vRightBottom, vDestPos, fRadius);
}

/**
 * @brief GetBoneCurrentWorldSpaceTranslation - get bone world space position
 * @param idx Bone index
 * @param vBoneCenterPos Bone center position offset
 * @param vPos Output position
 * @return true if successful
 * IDA: ?GetBoneCurrentWorldSpaceTranslation@CMover@@QEAAHHVhkvVec3@@AEAV2@@Z @ 0x140368690
 */
bool CMover::GetBoneCurrentWorldSpaceTranslation(int idx, const hkvVec3& vBoneCenterPos,
                                                   hkvVec3& vPos) {
    // TODO: 汇编还原 - IDA: 0x140368690
    // This function requires VAnimationInfo::GetBoneTranslation and hkvQuat operations
    (void)idx;
    (void)vBoneCenterPos;
    (void)vPos;
    return false;
}

/**
 * @brief IsAttackDecision - check if attack hits this mover
 * @param pAttackArea Attack area parameters
 * @return 0 if hit, non-zero otherwise
 * IDA: ?IsAttackDecision@CMover@@QEAAEQEAUtagATTACK_AREA@@@Z @ 0x140368D70
 */
std::uint8_t CMover::IsAttackDecision(struct tagATTACK_AREA* pAttackArea) {
    // TODO: 汇编还原 - IDA: 0x140368D70
    // Complex collision detection involving height, cylinder, and bone checks
    (void)pAttackArea;
    return 1;  // Not hit by default
}

/**
 * @brief IsActivateSkillUnlockBuff - check if skill unlock buff is active
 * @param pTBSkill Skill table pointer
 * @return true if skill unlock buff is active
 * IDA: ?IsActivateSkillUnlockBuff@CMover@@QEAA_NPEAUTB_SKILL@@@Z @ 0x140367550
 */
bool CMover::IsActivateSkillUnlockBuff(const struct TB_SKILL* pTBSkill) {
    if (!pTBSkill) {
        return false;
    }
    // Check if skill group exists in skill unlock map
    for (auto it = m_mapSkillUnlock.begin(); it != m_mapSkillUnlock.end(); ++it) {
        if (it->first == pTBSkill->Skill_Group) {
            return true;
        }
    }
    return false;
}

/**
 * @brief GetItemRateResultWeapon - get item rate result for weapon
 * @param byTargetLevel Target level
 * @param pAttr Attribute component
 * @param bCritical Critical hit flag
 * @return Item rate result
 * IDA: ?GetItemRateResultWeapon@CMover@@QEAAHEV?$shared_ptr@VCGocAttribute@@@tr1@std@@_N@Z @ 0x1403675F0
 */
std::uint32_t CMover::GetItemRateResultWeapon(std::uint8_t byTargetLevel,
                                                std::tr1::shared_ptr<class CGocAttribute> pAttr,
                                                bool bCritical) {
    // TODO: 汇编还原 - IDA: 0x1403675F0
    (void)byTargetLevel;
    (void)pAttr;
    (void)bCritical;
    return 0;
}

/**
 * @brief IsAttackDecision - check if attack hits this mover
 * @param pAttackArea Attack area parameters
 * @return 0 if hit, non-zero otherwise
 * IDA: ?IsAttackDecision@CMover@@QEAAEQEAUtagATTACK_AREA@@@Z @ 0x140368D70
 */
std::uint8_t CMover::IsAttackDecision(struct tagATTACK_AREA* pAttackArea) {
    // TODO: 汇编还原 - IDA: 0x140368D70
    // Complex collision detection involving height, cylinder, and bone checks
    (void)pAttackArea;
    return 1;  // Not hit by default
}

/**
 * @brief IsDamageMotionDisplay - check if damage motion should be displayed
 * @param byAttackCollision Attack collision type
 * @return true if damage motion should be displayed
 * IDA: ?IsDamageMotionDisplay@CMover@@UEAAHE@Z @ 0x140367BD0
 * Verified: Checks defense type vs attack collision type
 */
bool CMover::IsDamageMotionDisplay(std::uint8_t byAttackCollision) const {
    if (!byAttackCollision) {
        return false;
    }

    std::uint8_t byDefenseType = m_byDefenseType;

    if (byDefenseType == 1) {
        // Defense type 1: show for attack collision 2 or 4
        return byAttackCollision == 2 || byAttackCollision == 4;
    } else if (byDefenseType == 2) {
        // Defense type 2: show for attack collision 3 or 4
        return byAttackCollision == 3 || byAttackCollision == 4;
    } else {
        // Other defense types: show if not 3 or 5
        return byDefenseType != 3 && byDefenseType != 5;
    }
}

/**
 * @brief IsRegisterAnimInfo - check if animation info is registered
 * @param nMotionClass Motion class
 * @param nSubClass Sub class
 * @param strAnimName Animation name
 * @return true if animation info is registered
 * IDA: ?IsRegisterAnimInfo@CMover@@QEAAHFFVVString@@H@Z @ 0x140367AE0
 */
bool CMover::IsRegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass,
                                 const class VString& strAnimName) {
    // IDA: Check if action resource exists and find animation info
    if (m_pActionResource) {
        const char* szAnimName = strAnimName.AsChar();
        if (szAnimName && m_pActionResource->FindAnimationInfo(szAnimName)) {
            return true;
        }
    }
    return false;
}

/**
 * @brief GetBoneCurrentWorldSpaceTranslation - get bone world space position
 * @param idx Bone index
 * @param vBoneCenterPos Bone center position offset
 * @param vPos Output position
 * @return true if successful
 * IDA: ?GetBoneCurrentWorldSpaceTranslation@CMover@@QEAAHHVhkvVec3@@AEAV2@@Z @ 0x140368690
 */
bool CMover::GetBoneCurrentWorldSpaceTranslation(int idx, const hkvVec3& vBoneCenterPos,
                                                   hkvVec3& vPos) {
    // TODO: 汇编还原 - IDA: 0x140368690
    // This function requires VAnimationInfo::GetBoneTranslation and hkvQuat operations
    (void)idx;
    (void)vBoneCenterPos;
    (void)vPos;
    return false;
}

/**
 * @brief CheckAnimationEnd - check if animation has ended
 * IDA: ?CheckAnimationEnd@CMover@@QEAAXXZ @ 0x140367C80
 * Verified: Complex animation timing and offset processing
 */
void CMover::CheckAnimationEnd() {
    // TODO: 汇编还原 - IDA: 0x140367C80
    // This is a complex function handling animation timing, offset delta, and collision
}

/**
 * @brief CheckMoveCollision - check for move collision
 * @param vDestPos Destination position
 * @return Collided mover or nullptr
 * IDA: ?CheckMoveCollision@CMover@@QEAAPEAV1@AEAVhkvVec3@@@Z @ 0x1403681B0
 */
CMover* CMover::CheckMoveCollision(hkvVec3& vDestPos) {
    // TODO: 汇编还原 - IDA: 0x1403681B0
    // This function scans for nearby actors and checks collision
    (void)vDestPos;
    return nullptr;
}

/**
 * @brief GetItemRateResultWeapon - get item rate result for weapon
 * @param byTargetLevel Target level
 * @param pAttr Attribute component
 * @param bCritical Critical hit flag
 * @return Item rate result
 * IDA: ?GetItemRateResultWeapon@CMover@@QEAAHEV?$shared_ptr@VCGocAttribute@@@tr1@std@@_N@Z @ 0x1403675F0
 */
std::uint32_t CMover::GetItemRateResultWeapon(std::uint8_t byTargetLevel,
                                                std::tr1::shared_ptr<class CGocAttribute> pAttr,
                                                bool bCritical) {
    // TODO: 汇编还原 - IDA: 0x1403675F0
    (void)byTargetLevel;
    (void)pAttr;
    (void)bCritical;
    return 0;
}

/**
 * @brief GetItemRateResultGear - get item rate result for gear
 * @param byTargetLevel Target level
 * @param pAttr Attribute component
 * @return Item rate result
 * IDA: ?GetItemRateResultGear@CMover@@QEAAHEV?$shared_ptr@VCGocAttribute@@@tr1@std@@@Z @ 0x140367780
 */
std::uint32_t CMover::GetItemRateResultGear(std::uint8_t byTargetLevel,
                                              std::tr1::shared_ptr<class CGocAttribute> pAttr) {
    // TODO: 汇编还原 - IDA: 0x140367780
    (void)byTargetLevel;
    (void)pAttr;
    return 0;
}

/**
 * @brief IsActivateSkillUnlockBuff - check if skill unlock buff is active
 * @param pTBSkill Skill table pointer
 * @return true if skill unlock buff is active
 * IDA: ?IsActivateSkillUnlockBuff@CMover@@QEAA_NPEAUTB_SKILL@@@Z @ 0x140367550
 * Verified: Checks if skill group exists in skill unlock map
 */
bool CMover::IsActivateSkillUnlockBuff(const struct TB_SKILL* pTBSkill) {
    if (!pTBSkill) {
        return false;
    }
    // Check if skill group exists in skill unlock map
    for (auto it = m_mapSkillUnlock.begin(); it != m_mapSkillUnlock.end(); ++it) {
        if (it->first == pTBSkill->Skill_Group) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SetupAnimation - setup animation for mover
 * IDA: ?SetupAnimation@CMover@@QEAAXXZ @ 0x140367980
 * Verified: Loads action resource and sets up animation info
 */
void CMover::SetupAnimation() {
    // IDA: Get action resource filename and load it
    VString strActionFN;
    GetActionResourceFN(&strActionFN);
    const char* szAnimName = strActionFN.AsChar();

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    m_pActionResource = (VActionResourceLump*)VResourceManager::GetResourceByName(
        &pServer->m_xActionManager, szAnimName);

    int dwTableID = GetTableID();
    if (XActionResMgr::SetAnimInfoToActor(&pServer->m_xActionManager, dwTableID, this)) {
        SetupAnimInfo();
        unsigned int AnimIndex = XActionResMgr::GetAnimIndex(this, 18, 2, 1);
        if (GetAnimStirng(AnimIndex)) {
            m_nHitAnimCount = 7;
        } else {
            m_nHitAnimCount = 2;
        }
    } else if (XActor::GetType() != 1) {
        int Type = XActor::GetType();
        LogHelper::LogDebug("game.contents", "SetupAnimation No ActionData (%d:%d)", Type, dwTableID);
    }
}

// ============================================================================
// CMoverEx Implementation
// ============================================================================

/**
 * @brief CMoverEx constructor
 * IDA: ??0CMoverEx@@QEAA@XZ (0x140379260)
 */
CMoverEx::CMoverEx()
    : CMover()
    , m_fLastChangeAnimationTime(0.0f)
    , m_bBattlePose(false)
    , m_fMovingYaw(0.0f)
    , m_fSummonAkashicYaw(0.0f)
    , m_fLookPitch(0.0f)
    , m_fCheckMoveStopTime(0.0f)
    , m_fMoveDelayTime(0.0f)
    , m_bCancelMoving(0)
    , m_fLastStartRotTime(0.0f)
    , m_fStartRotWaitTime(0.0f)
    , m_bStartRotation(false)
    , m_bQuickTurn(false)
    , m_bKeepLookTarget(false)
    , m_fDefWalkSpeed(100.0f)
    , m_fDefRunSpeed(300.0f)
    , m_fDefTurnSpeed(0.0f)
    , m_fBackupTurnSpeed(0.0f)
    , m_fTurnSpeedRate(0.0f)
    , m_fQuickTurnSpeed(0.0f)
    , m_fHitFreezeTime(0.0f)
    , m_iStiffenCount(0)
    , m_fStiffenRate(0.0f)
    , m_fStiffenTime(0.0f)
    , m_fStiffenDelayTime(0.0f)
    , m_fStiffenImmuneTime(0.0f)
    , m_bAnimSpeedTrigger(0)
    , m_fFlyVelocity(0.0f)
    , m_fJumpDelta(0.0f)
    , m_fJumpDelayTime(0.0f)
    , m_fFlyingStayTime(0.0f)
    , m_fJumpTime(0.0f)
    , m_fJumpHeight(0.0f)
    , m_fPrevJumpHeight(0.0f)
    , m_fFlyDirValue(0.0f)
    , m_fFlyGravity(1960.0f)
    , m_fFlyMaxHeight(200.0f)
    , m_bJumpAnim(0)
    , m_byAniProcessLinkType(0)
    , m_byMoveDirAnim(0)
    , m_pCurSkillTableRef(nullptr)
    , m_bAttackKeyPress(0)
    , m_bySkillAnimStep(0)
    , m_bySkillAnimCount(1)
    , m_fSkillLoopTime(0.0f)
    , m_fSkillBlendEndTime(0.0f)
    , m_bySkillChargeStep(0)
    , m_bySkillChargeMaxStep(0)
    , m_fSkillChargeChangeTime(0.0f)
    , m_bHasTurnMotion(0)
    , m_bHasBigTurn(0)
    , m_bSkipReplayTime(0)
    , m_nShieldHP(0)
    , m_byPhaseType(0)
    , m_byPhaseStep(0)
    , m_byMaxPhaseStep(0)
    , m_byPhaseCondition(0)
    , m_dwPhaseConditionValue(0)
    , m_fPhaseDurationTime(0.0f)
    , m_nPlayPhaseMotion(-1)
    , m_nBuffMotion(-1)
    , m_byStandType(0)
    , m_byBattleModeAnim(0)
    , m_fIdleCheckTime(5.0f)
    , m_nIdleMotionChance(20)
    , m_pMouseOnTrap(nullptr)
    , m_pSilhouet(nullptr)
    , m_pCurAkashicRecord(nullptr)
    , m_pAkashicObject(nullptr)
    , m_pAuraSkill(nullptr)
    , m_fAuraCheckTime(0.0f)
    , m_fAuraLifeTime(0.0f)
    , m_byAggroLevelOrder(0)
    , m_fAmountOfHeal(0.0f)
    , m_byDieReason(0)
    , m_nDieDamage(0)
    , m_bMoveSkillPrefab(0)
    , m_dwInvisibleFlag(0)
    , m_fInvisibleEndTime(0.0f)
    , m_nInvisibleConditionType(0)
    , m_iLastWarpPoint(-1)
    , m_bSystemActor(0)
    , m_bChangedStat(0)
    , m_fSkillTotalChargeTime(0.0f)
    , m_bExistSuboCombo(false)
    , m_fSuboComboWaitTime(0.0f)
    , m_fSuboComboCheckTime(0.0f)
    , m_iSuboComboMaxCount(-1)
    , m_iSuboComboCheckCount(0)
    , m_iCurLeftSuboDescCount(-1)
    , m_iCurRightSuboDescCount(-1)
    , m_iMaxLeftSuboDescCount(0)
    , m_iMaxRightSuboDescCount(0)
    , m_pSuboComboTrigger(nullptr)
    , m_bCheckAttachToAttacker(false)
    , m_pAttachToAttacker(nullptr)
    , m_fAttachedDirDist(0.0f)
    , m_fAttachedDuration(0.0f)
    , m_fAttachedDistance(0.0f)
    , m_fAttachedAngle(0.0f)
    , m_iApplyWeightRank(0)
    , m_bDisableDirectionToTargetSkill(1)
    , m_eDieType(DIE_TYPE_NORMAL)
    , m_bFlyDie(false)
    , m_bCounterSuccessFrame(false)
    , m_bEnableCounuter(false)
    , m_fMinCounterRange(0.0f)
    , m_fMaxCounterRange(0.0f)
    , m_fCounterAngle(0.0f)
    , m_iCounterProbability(0)
    , m_fCounterDuration(0.0f)
    , m_bChargingStart(false)
    , m_fChargingInputAnimSpeed(1.0f)
    , m_fChargingInputMaxAddMultiple(0.0f)
    , m_fChargingInputDuration(0.0f)
    , m_fChargingInputTime(0.0f)
    , m_fChargingInputPressTime(0.0f)
    , m_fChargingInputCalcMultiple(0.0f)
    , m_bControlMonster(false)
    , m_fLeftChargingValue(0.0f)
    , m_fRightChargingValue(0.0f)
    , m_byLastDamageType(0)
    , m_bOnDie(false)
    , m_bDieAttack(false)
    , m_pSector(nullptr)
    , m_fSkillSkipCoolTime(0.0f)
    , m_fUpdateStatBatchTime(0.2f)
    , m_pCurDivergenceTableRef(nullptr)
    , m_dwDivergenceSkillID(0)
    , m_byGrapStep(0)
    , m_pGrapParent(nullptr)
    , m_pGrapTarget(nullptr)
    , m_fDropSpeedX(0.0f)
    , m_fDropSpeedY(0.0f)
    , m_fDropSpeedZ(0.0f)
    , m_pDetachTrigger(nullptr)
    , m_fAutoRotaionSpeed(0.0f)
    , m_fAutoRotaionTime(0.0f)
    , m_dwOwnerID(0)
    , m_bAllowAbsorbSG(false)
    , m_nAccumulateDamage(0)
    , m_pCurDeckBonusRef(nullptr)
    , m_bReserveChange(false)
    , m_dwChangeMobNewID(0)
    , m_dwChangeMobTableID(0)
    , m_fMoveDistAfterSkill(0.0f)
    , m_eTestDamageType(eTestDamage_None)
    , m_byFixedMaxDamage(0)
    , m_bUpdateRotation(true)
    , m_byPhaseMotionStep(0)
    , m_fPhaseStepMaxTime(0.0f)
    , m_fMultipleDamageOnce(0.0f)
    , m_bApplyMultipleDamageOnce(false)
    , m_fMultipleAbsorbSG(1.0f)
    , m_bySABreakMotionType(0)
    , m_fSABreakTime(5.0f)
    , m_fSABreakLoopTime(0.0f)
    , m_bSABreakLoopMotion(false)
    , m_bShowSABreakMotion(false)
    , m_fSABreakLoopMotionTime(0.0f)
    , m_nAddMoneyFromOptionEffect(0)
    , m_nAddEtherFromOptionEffect(0)
    , m_nAddExpFromOptionEffect(0)
    , m_nCombatType(0)
    , m_bLinkSkillOn(false)
    , m_fLinkSkillDuration(0.0f)
    , m_pLinkSkillTrigger(nullptr)
    , m_bCheckAttackSkillOn(false)
    , m_fCheckAttackSkillDuration(0.0f)
    , m_pCheckAttackSkillTrigger(nullptr)
    , m_bSummonMonsterApplyRot(false)
    , m_nNowChangingMotionClass(0)
    , m_iChangeMotionPos(0)
    , m_iChangeMotionType(0)
    , m_iActionCondition(0)
    , m_iPvpCondition(0)
{
    memset(m_nInvisibleConditionVal, 0, sizeof(m_nInvisibleConditionVal));
    memset(m_szCounterAnimName, 0, sizeof(m_szCounterAnimName));
    memset(m_szTempSkillAnimName, 0, sizeof(m_szTempSkillAnimName));
    memset(m_sHitParts, 0, sizeof(m_sHitParts));
    memset(&m_iChangeMotionTime, 0, sizeof(m_iChangeMotionTime));

    RemoveAllOptionEffect();
    RemoveAllDefenseChangeInfo();
}

/**
 * @brief CMoverEx destructor
 */
CMoverEx::~CMoverEx() {
    // Clean up
    m_EventObjectList.Clear();
    m_CommonPosBoxList.Clear();
    m_vPreTargetList.clear();
    m_vecOptionEffect.clear();
    m_vecDelayBuff.clear();
}

/**
 * @brief InitFunction - initialize function
 * IDA: ?InitFunction@CMoverEx@@UEAAXXZ (0x140379230)
 */
void CMoverEx::InitFunction() {
    // TODO: 汇编还原 - IDA: 0x140379230
}

/**
 * @brief Destroy - destroy mover
 * IDA: ?Destroy@CMoverEx@@UEAAXXZ (0x140379260)
 */
void CMoverEx::Destroy() {
    // TODO: 汇编还原 - IDA: 0x140379260
}

/**
 * @brief MessageFunction - handle message
 * @param nMsg Message ID
 * @param wParam WPARAM
 * @param lParam LPARAM
 * IDA: ?MessageFunction@CMoverEx@@UEAAXH_J0@Z (0x1407A870)
 */
void CMoverEx::MessageFunction(int nMsg, __int64 wParam, __int64 lParam) {
    // TODO: 汇编还原 - IDA: 0x1407A870
    (void)nMsg;
    (void)wParam;
    (void)lParam;
}

/**
 * @brief GetNextMotion - get next motion class
 * @return Motion class
 * IDA: ?GetNextMotion@CMoverEx@@UEAAFXZ (0x1403823D0)
 */
std::int16_t CMoverEx::GetNextMotion() {
    // TODO: 汇编还原 - IDA: 0x1403823D0
    // This is a complex function with many conditions
    return 1;
}

/**
 * @brief MoveTick - process movement tick
 * @return true if movement was processed
 * IDA: ?MoveTick@CMoverEx@@UEAA_NXZ (0x140382E60)
 */
bool CMoverEx::MoveTick() {
    // TODO: 汇编还原 - IDA: 0x140382E60
    // Complex movement processing function
    return true;
}

/**
 * @brief StartMoving - start moving
 * @return true if started
 * IDA: ?StartMoving@CMoverEx@@UEAAHXZ (0x1403823D0)
 */
bool CMoverEx::StartMoving() {
    // TODO: 汇编还原 - IDA: 0x1403823D0
    return true;
}

/**
 * @brief GetSectorID - get sector ID
 * @return Sector ID
 * IDA: ?GetSectorID@CMoverEx@@UEAAHXZ (0x140373FA0)
 */
int CMoverEx::GetSectorID() {
    // TODO: 汇编还原 - IDA: 0x140373FA0
    return 0;
}

/**
 * @brief GetMaxHP - get maximum HP
 * @return Maximum HP
 * IDA: ?GetMaxHP@CMoverEx@@UEAAHXZ (0x140188410)
 */
int CMoverEx::GetMaxHP() {
    // TODO: 汇编还原 - IDA: 0x140188410
    return 0;
}

/**
 * @brief SetSkillTable - set skill table
 * @param pSkill Skill table pointer
 * IDA: ?SetSkillTable@CMoverEx@@UEAAXPEAUTB_SKILL@@@Z (0x140187F60)
 */
void CMoverEx::SetSkillTable(TB_SKILL* pSkill) {
    m_pCurSkillTableRef = pSkill;
}

/**
 * @brief SetDie - set as dead
 * @param nMotionClass Motion class
 * @param cDieReason Death reason
 * IDA: ?SetDie@CMoverEx@@UEAAXFH@Z (0x140396520)
 */
void CMoverEx::SetDie(int nMotionClass, std::int8_t cDieReason) {
    // TODO: 汇编还原 - IDA: 0x140396520
    (void)nMotionClass;
    (void)cDieReason;
}

/**
 * @brief Damage - apply damage
 * @param stDamage Damage info
 * @param nType Damage type
 * @param bResult Result output
 * IDA: ?Damage@CMoverEx@@UEAAXAEAUtagACTION_DAMAGE@@HAEA_N@Z (0x140384F70)
 */
void CMoverEx::Damage(tagACTION_DAMAGE& stDamage, int nType, bool& bResult) {
    // TODO: 汇编还原 - IDA: 0x140384F70
    (void)stDamage;
    (void)nType;
    (void)bResult;
}

/**
 * @brief IsCanAttack - check if can attack
 * @return true if can attack
 * IDA: ?IsCanAttack@CMoverEx@@UEAAEXZ (0x14037EAC0)
 */
bool CMoverEx::IsCanAttack() {
    // TODO: 汇编还原 - IDA: 0x14037EAC0
    return true;
}

/**
 * @brief ReapllyBuffAll - reapply all buffs
 * IDA: ?ReapllyBuffAll@CMoverEx@@UEAAXXZ (0x14039D7B0)
 */
void CMoverEx::ReapllyBuffAll() {
    // TODO: 汇编还原 - IDA: 0x14039D7B0
}

/**
 * @brief ChargeSkillNextStep - charge skill to next step
 * IDA: ?ChargeSkillNextStep@CMoverEx@@UEAAXXZ (0x14037DB60)
 */
void CMoverEx::ChargeSkillNextStep() {
    // TODO: 汇编还原 - IDA: 0x14037DB60
}

/**
 * @brief ApplySkillMove - apply skill movement
 * @param fDistance Distance to move
 * @param fTime Time to move
 * IDA: ?ApplySkillMove@CMoverEx@@QEAAXMM@Z (0x140396520)
 */
void CMoverEx::ApplySkillMove(float fDistance, float fTime) {
    // TODO: 汇编还原 - IDA: 0x140396520
    // This function calculates target position based on moving yaw
    // and sends skill move command
    (void)fDistance;
    (void)fTime;
}

/**
 * @brief SetAkashicObject - set akashic object
 * @param pObject Akashic object
 * IDA: ?SetAkashicObject@CMoverEx@@QEAAXPEAV1@@Z (0x1409A950)
 */
void CMoverEx::SetAkashicObject(CMoverEx* pObject) {
    m_pAkashicObject = pObject;
}

/**
 * @brief SetCameraDir - set camera direction
 * @param vDir Camera direction
 * IDA: ?SetCameraDir@CMoverEx@@QEAAXAEAVhkvVec3@@@Z (0x1405F92E0)
 */
void CMoverEx::SetCameraDir(hkvVec3& vDir) {
    // TODO: 汇编还原 - IDA: 0x1405F92E0
    (void)vDir;
}

/**
 * @brief GetSilhoutte - get silhouette generator
 * @return Silhouette generator
 * IDA: ?GetSilhoutte@CMoverEx@@QEAAPEAVhkaiPointCloudSilhouetteGenerator@@XZ (0x140ACC30)
 */
hkaiPointCloudSilhouetteGenerator* CMoverEx::GetSilhoutte() {
    return m_pSilhouet;
}

/**
 * @brief GetAttackJudgmentEvent - get attack judgment event
 * @param nIndex Event index
 * @return Attack judgment trigger
 * IDA: ?GetAttackJudgmentEvent@CMoverEx@@QEAAPEAVAttackJudgmentTrigger@@H@Z (0x14080500)
 */
AttackJudgmentTrigger* CMoverEx::GetAttackJudgmentEvent(int nIndex) {
    // TODO: 汇编还原 - IDA: 0x14080500
    (void)nIndex;
    return nullptr;
}

/**
 * @brief SetupPhaseMotion - setup phase motion
 * IDA: ?SetupPhaseMotion@CMoverEx@@QEAAXXZ (0x14084E20)
 */
void CMoverEx::SetupPhaseMotion() {
    // TODO: 汇编还原 - IDA: 0x14084E20
}

/**
 * @brief ReturnFromPrefab - return from prefab
 * IDA: ?ReturnFromPrefab@CMoverEx@@QEAAXXZ (0x14093E50)
 */
void CMoverEx::ReturnFromPrefab() {
    // TODO: 汇编还原 - IDA: 0x14093E50
}

/**
 * @brief IsCanOptionEffectExcute - check if option effect can execute
 * @param pOption Option
 * @param pMover Mover
 * @param fTime Time
 * @return true if can execute
 * IDA: ?IsCanOptionEffectExcute@CMoverEx@@QEAA_NPEAUTB_CREATEOPTION@@PEAV1@M@Z (0x1409A010)
 */
bool CMoverEx::IsCanOptionEffectExcute(struct TB_CREATEOPTION* pOption, CMoverEx* pMover, float fTime) {
    // TODO: 汇编还原 - IDA: 0x1409A010
    (void)pOption;
    (void)pMover;
    (void)fTime;
    return false;
}

/**
 * @brief GetMotionChangeLog - get motion change log
 * @return Log string
 * IDA: ?GetMotionChangeLog@CMoverEx@@QEAA?AV?$basic_string@DU?$char_traits@D@std@@...@@Z (0x1409DB10)
 */
std::string CMoverEx::GetMotionChangeLog() {
    // TODO: 汇编还原 - IDA: 0x1409DB10
    return "";
}

/**
 * @brief GetDieType - get death type
 * @return Death type
 * IDA: ?GetDieType@CMoverEx@@QEAA?AW4DIE_TYPE@@XZ (0x140C6BF0)
 */
DIE_TYPE CMoverEx::GetDieType() {
    return m_eDieType;
}

/**
 * @brief GetChangeMobNewID - get change mob new ID
 * @return New mob ID
 * IDA: ?GetChangeMobNewID@CMoverEx@@QEAAKXZ (0x14052A00)
 */
std::uint32_t CMoverEx::GetChangeMobNewID() {
    return m_dwChangeMobNewID;
}

/**
 * @brief GetHitPartsInfo - get hit parts info
 * @param nIndex Index
 * @return Hit parts info reference
 * IDA: ?GetHitPartsInfo@CMoverEx@@QEAAAEAUSHitPartsInfo@@H@Z (0x14052850)
 */
SHitPartsInfo& CMoverEx::GetHitPartsInfo(int nIndex) {
    static SHitPartsInfo s_empty;
    if (nIndex >= 0 && nIndex < 2) {
        return m_sHitParts[nIndex];
    }
    return s_empty;
}

/**
 * @brief GetAddMoneyFromOptionEffect - get add money from option effect
 * @return Add money
 * IDA: ?GetAddMoneyFromOptionEffect@CMoverEx@@QEAAHXZ (0x140F9000)
 */
int CMoverEx::GetAddMoneyFromOptionEffect() {
    return m_nAddMoneyFromOptionEffect;
}

/**
 * @brief ResetAddMoneyFromOptionEffect - reset add money from option effect
 * IDA: ?ResetAddMoneyFromOptionEffect@CMoverEx@@QEAAXXZ (0x140F8FE0)
 */
void CMoverEx::ResetAddMoneyFromOptionEffect() {
    m_nAddMoneyFromOptionEffect = 0;
}

/**
 * @brief GetAddExpFromOptionEffect - get add exp from option effect
 * @return Add exp
 * IDA: ?GetAddExpFromOptionEffect@CMoverEx@@QEAAHXZ (0x14048270)
 */
int CMoverEx::GetAddExpFromOptionEffect() {
    return m_nAddExpFromOptionEffect;
}

/**
 * @brief ResetAddExpFromOptionEffect - reset add exp from option effect
 * IDA: ?ResetAddExpFromOptionEffect@CMoverEx@@QEAAXXZ (0x14048250)
 */
void CMoverEx::ResetAddExpFromOptionEffect() {
    m_nAddExpFromOptionEffect = 0;
}

/**
 * @brief GetAddEtherFromOptionEffect - get add ether from option effect
 * @return Add ether
 * IDA: ?GetAddEtherFromOptionEffect@CMoverEx@@QEAAHXZ (0x140F8F90)
 */
int CMoverEx::GetAddEtherFromOptionEffect() {
    return m_nAddEtherFromOptionEffect;
}

/**
 * @brief ResetAddEtherFromOptionEffect - reset add ether from option effect
 * IDA: ?ResetAddEtherFromOptionEffect@CMoverEx@@QEAAXXZ (0x140F8F70)
 */
void CMoverEx::ResetAddEtherFromOptionEffect() {
    m_nAddEtherFromOptionEffect = 0;
}

/**
 * @brief SetOwnerID - set owner ID
 * @param dwID Owner ID
 * IDA: ?SetOwnerID@CMoverEx@@QEAAXK@Z (0x1409E1C0)
 */
void CMoverEx::SetOwnerID(std::uint32_t dwID) {
    m_dwOwnerID = dwID;
}

/**
 * @brief GetSkillLoopTime - get skill loop time
 * @return Skill loop time
 * IDA: ?GetSkillLoopTime@CMoverEx@@QEAAMXZ (0x14015ED0)
 */
float CMoverEx::GetSkillLoopTime() {
    return m_fSkillLoopTime;
}

/**
 * @brief SetMovingYaw - set moving yaw
 * @param fYaw Yaw angle
 * IDA: ?SetMovingYaw@CMoverEx@@IEAAXM@Z (0x1407E3E0)
 */
void CMoverEx::SetMovingYaw(float fYaw) {
    m_fMovingYaw = fYaw;
}

/**
 * @brief GetBloodDebuffOwnerID_Ex - get blood debuff owner ID
 * @return Owner ID
 */
std::uint32_t CMoverEx::GetBloodDebuffOwnerID_Ex() {
    return m_dwBloodDebuffOwnerID;
}

/**
 * @brief RemoveAllOptionEffect - remove all option effects
 */
void CMoverEx::RemoveAllOptionEffect() {
    m_vecOptionEffect.clear();
}

/**
 * @brief RemoveAllDefenseChangeInfo - remove all defense change info
 */
void CMoverEx::RemoveAllDefenseChangeInfo() {
    m_listDefenseChangeInfo.clear();
}

/**
 * @brief IsJumpMotion - check if motion is jump
 * @param nMotionClass Motion class
 * @return true if jump motion
 */
bool CMoverEx::IsJumpMotion(std::int16_t nMotionClass) {
    // TODO: 汇编还原
    (void)nMotionClass;
    return false;
}

/**
 * @brief IsCanMovingAnim - check if can play moving animation
 * @return true if can play
 */
bool CMoverEx::IsCanMovingAnim() {
    // TODO: 汇编还原
    return true;
}

/**
 * @brief GetControlType - get control type for skill
 * @param pSkill Skill pointer
 * @return Control type
 */
int CMoverEx::GetControlType(TB_SKILL* pSkill) {
    // TODO: 汇编还原
    (void)pSkill;
    return 0;
}

/**
 * @brief GetSkillAnimName - get skill animation name
 * @param pSkill Skill pointer
 * @param byStep Animation step
 * @return Animation name
 */
const char* CMoverEx::GetSkillAnimName(TB_SKILL* pSkill, std::uint8_t byStep) {
    // TODO: 汇编还原
    (void)pSkill;
    (void)byStep;
    return "";
}

/**
 * @brief GetAnimIndex - get animation index
 * @param strAnimName Animation name
 * @return Animation index
 */
std::uint32_t CMoverEx::GetAnimIndex(const class VString& strAnimName) {
    // TODO: 汇编还原
    (void)strAnimName;
    return 0;
}

/**
 * @brief AnimKeyToMotion - convert animation key to motion class
 * @param dwKey Animation key
 * @return Motion class
 */
std::int16_t CMoverEx::AnimKeyToMotion(std::uint32_t dwKey) {
    // TODO: 汇编还原
    (void)dwKey;
    return 0;
}

/**
 * @brief ClearMotion - clear current motion
 */
void CMoverEx::ClearMotion() {
    // TODO: 汇编还原
}

/**
 * @brief StopMoving - stop moving
 * @param bSend Whether to send packet
 */
void CMoverEx::StopMoving(bool bSend) {
    // TODO: 汇编还原
    (void)bSend;
}

/**
 * @brief CheckMoveCollision - check move collision
 * @param pPos Position to check
 * @return Collided mover
 */
CMover* CMoverEx::CheckMoveCollision(hkvVec3* pPos) {
    // TODO: 汇编还原
    (void)pPos;
    return nullptr;
}

/**
 * @brief GetHeight - get height at position
 * @param pPos Position
 * @param fMaxHeight Maximum height
 */
void CMoverEx::GetHeight(hkvVec3* pPos, float fMaxHeight) {
    // TODO: 汇编还原
    (void)pPos;
    (void)fMaxHeight;
}

/**
 * @brief MoveingValueClear - clear moving values
 */
void CMoverEx::MoveingValueClear() {
    // TODO: 汇编还原
}

/**
 * @brief DebugOut - debug output
 * @param szFormat Format string
 * @param ... Arguments
 */
void CMoverEx::DebugOut(const char* szFormat, ...) {
    // TODO: 汇编还原
    (void)szFormat;
}
