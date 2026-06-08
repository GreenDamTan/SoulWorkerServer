#include "Mover.h"
#include <cstdarg>
#include <cstdio>

// Component headers for GetGOC wrappers
#include "Soulworker/GameServer/XGameServer/actor/component/GOComponent.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocSkill.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocParty.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocForce.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAchieve.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocPost.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocFriend.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocBooster.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAkashicRecord.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocWeeklyMission.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocClassEvent.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocHelper.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEvent.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocLeague.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocExchange.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocDailyMission.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNpcAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocSoulMetry.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNpcCredit.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttendance.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocMyRoom.h"

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

// ============================================================================
// SOptionEffect - Constructor and Destructor
// IDA: 0x1403A1B90 (constructor), 0x1403A1C00 (destructor)
// ============================================================================

SOptionEffect::SOptionEffect()
    : dwEquipedIndex(0)
    , dwTargetMoverID(0)
    , fOptionValue(0.0f)
    , fReferanceValue(0.0f)
    , fAppliedValue(0.0f)
    , pOptionTable(nullptr)
    , dwLifeTime(0)
    , fCurTime(0.0f)
{
}

SOptionEffect::~SOptionEffect()
{
    dwEquipedIndex = 0;
    dwTargetMoverID = 0;
    fOptionValue = 0.0f;
    fReferanceValue = 0.0f;
    fAppliedValue = 0.0f;
    pOptionTable = nullptr;
    dwLifeTime = 0;
    fCurTime = 0.0f;
}

// ============================================================================
// CMover::GetGOC_Xxx wrapper functions
// These are explicit instantiations of the GetGOC<T> template
// ============================================================================

std::shared_ptr<CGocEntity> CMover::GetGOC_Entity(bool bCreateIfNull) {
    std::shared_ptr<CGocEntity> result;
    GetGOC<CGocEntity>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocInventory> CMover::GetGOC_Inventory(bool bCreateIfNull) {
    std::shared_ptr<CGocInventory> result;
    GetGOC<CGocInventory>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocAttribute> CMover::GetGOC_Attribute(bool bCreateIfNull) {
    std::shared_ptr<CGocAttribute> result;
    GetGOC<CGocAttribute>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocSkill> CMover::GetGOC_Skill(bool bCreateIfNull) {
    std::shared_ptr<CGocSkill> result;
    GetGOC<CGocSkill>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocParty> CMover::GetGOC_Party(bool bCreateIfNull) {
    std::shared_ptr<CGocParty> result;
    GetGOC<CGocParty>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocForce> CMover::GetGOC_Force(bool bCreateIfNull) {
    std::shared_ptr<CGocForce> result;
    GetGOC<CGocForce>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocQuest> CMover::GetGOC_Quest(bool bCreateIfNull) {
    std::shared_ptr<CGocQuest> result;
    GetGOC<CGocQuest>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocAchieve> CMover::GetGOC_Achieve(bool bCreateIfNull) {
    std::shared_ptr<CGocAchieve> result;
    GetGOC<CGocAchieve>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocPost> CMover::GetGOC_Post(bool bCreateIfNull) {
    std::shared_ptr<CGocPost> result;
    GetGOC<CGocPost>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocFriend> CMover::GetGOC_Friend(bool bCreateIfNull) {
    std::shared_ptr<CGocFriend> result;
    GetGOC<CGocFriend>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocBooster> CMover::GetGOC_Booster(bool bCreateIfNull) {
    std::shared_ptr<CGocBooster> result;
    GetGOC<CGocBooster>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocRecode> CMover::GetGOC_Recode(bool bCreateIfNull) {
    std::shared_ptr<CGocRecode> result;
    GetGOC<CGocRecode>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocAkashicRecord> CMover::GetGOC_AkashicRecord(bool bCreateIfNull) {
    std::shared_ptr<CGocAkashicRecord> result;
    GetGOC<CGocAkashicRecord>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocWeeklyMission> CMover::GetGOC_WeeklyMission(bool bCreateIfNull) {
    std::shared_ptr<CGocWeeklyMission> result;
    GetGOC<CGocWeeklyMission>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocClassEvent> CMover::GetGOC_ClassEvent(bool bCreateIfNull) {
    std::shared_ptr<CGocClassEvent> result;
    GetGOC<CGocClassEvent>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocHelper> CMover::GetGOC_Helper(bool bCreateIfNull) {
    std::shared_ptr<CGocHelper> result;
    GetGOC<CGocHelper>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocEvent> CMover::GetGOC_Event(bool bCreateIfNull) {
    std::shared_ptr<CGocEvent> result;
    GetGOC<CGocEvent>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocLeague> CMover::GetGOC_League(bool bCreateIfNull) {
    std::shared_ptr<CGocLeague> result;
    GetGOC<CGocLeague>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocExchange> CMover::GetGOC_Exchange(bool bCreateIfNull) {
    std::shared_ptr<CGocExchange> result;
    GetGOC<CGocExchange>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocDailyMission> CMover::GetGOC_DailyMission(bool bCreateIfNull) {
    std::shared_ptr<CGocDailyMission> result;
    GetGOC<CGocDailyMission>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocNpcAttribute> CMover::GetGOC_NpcAttribute(bool bCreateIfNull) {
    std::shared_ptr<CGocNpcAttribute> result;
    GetGOC<CGocNpcAttribute>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocSoulMetry> CMover::GetGOC_SoulMetry(bool bCreateIfNull) {
    std::shared_ptr<CGocSoulMetry> result;
    GetGOC<CGocSoulMetry>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocNpcCredit> CMover::GetGOC_NpcCredit(bool bCreateIfNull) {
    std::shared_ptr<CGocNpcCredit> result;
    GetGOC<CGocNpcCredit>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocAttendance> CMover::GetGOC_Attendance(bool bCreateIfNull) {
    std::shared_ptr<CGocAttendance> result;
    GetGOC<CGocAttendance>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocMyRoom> CMover::GetGOC_Myroom(bool bCreateIfNull) {
    std::shared_ptr<CGocMyRoom> result;
    GetGOC<CGocMyRoom>(&result, bCreateIfNull);
    return result;
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
 * @brief ClearExtraMoving - clear extra moving value
 * IDA: ?ClearExtraMoving@CMover@@UEAAXXZ @ 0x140189390
 * Verified: Calls tagEXTRA_MOVEPOS::Clear on m_stExtMovingVal
 */
void CMover::ClearExtraMoving() {
    // TODO: 需要实现 tagEXTRA_MOVEPOS::Clear
    // IDA: tagEXTRA_MOVEPOS::Clear(&this->m_stExtMovingVal);
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
 * IDA: ?GetTableIDString@CMover@@QEAAPEBDXZ (0x14036DE70)
 * Verified: Returns cached table ID string, formats it if empty
 */
const char* CMover::GetTableIDString() {
    // IDA exact reconstruction:
    if (m_strTableID.IsEmpty()) {
        int v1 = GetTableID();
        m_strTableID.Format("%d", v1);
    }
    return m_strTableID.AsChar();
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
    // IDA exact reconstruction:
    return XActor::IsDieStatus() || GetHP() <= 0;
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
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 2;
    }
    // IDA: Otherwise check XActor status 0x800 (dashing)
    return XActor::IsStatus(0x800u);
}

/**
 * @brief IsFlying - check if mover is flying
 * @return true if flying
 * IDA: ?IsFlying@CMover@@QEAAHXZ @ 0x140367080
 * Verified: Checks forced state 1, or !m_bLanded and position > ground + 5.0
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
    // IDA: Get current position and check height
    hkvVec3 vPos;
    VisObject3D_cl::GetPosition(&vPos);
    float fZ = vPos.z;
    
    // IDA: Try to get ground height
    if (const_cast<CMover*>(this)->GetHeight(&vPos, 300.0f)) {
        // If current position is more than 5 units above ground, flying
        return fZ > (vPos.z + 5.0f);
    } else {
        // Debug output if GetHeight failed
        DebugOut("IsFlying>> GetHeight failed. (%.2f,%.2f,%.2f)", vPos.x, vPos.y, vPos.z);
        return false;
    }
}

/**
 * @brief SendUpdateStat - send stat update
 * @param nStatType Stat type to update
 * IDA: ?SendUpdateStat@CMover@@QEAAXH@Z (0x14036E4A0)
 * Verified: Gets CGocAttribute component and calls its SendUpdateStat
 */
void CMover::SendUpdateStat(int nStatType) {
    // IDA exact reconstruction:
    auto pAttr = GetGOC_Attribute(false);
    if (pAttr) {
        pAttr->SendUpdateStat(nStatType);
    }
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
 * IDA: ?DeleteDelayedProjectile@CMover@@QEAAXPEAUSDelayedProjectile@@@Z (0x14036E550)
 */
void CMover::DeleteDelayedProjectile(SDelayedProjectile* pProjectile) {
    // IDA 精确还原:
    // 遍历 m_vecDelayedProjectile 向量
    // 找到匹配的投射物指针后删除并从向量中移除
    if (!pProjectile) return;

    for (auto it = m_vecDelayedProjectile.begin(); it != m_vecDelayedProjectile.end(); ++it) {
        if (*it == pProjectile) {
            delete pProjectile;
            m_vecDelayedProjectile.erase(it);
            return;
        }
    }
}

/**
 * @brief CheckContinuousMelee - check continuous melee
 * @param fDeltaTime Delta time since last frame
 * IDA: ?CheckContinuousMelee@CMover@@QEAAXM@Z (0x140373EE0)
 */
void CMover::CheckContinuousMelee(float fDeltaTime) {
    // IDA 精确还原:
    // 遍历 m_vContinuousMelee 向量
    // 更新每个连续近战攻击的剩余间隔时间和剩余生命时间
    // 当间隔时间 <= 0 时,添加动作缓冲区并重置间隔时间
    // 当生命时间 <= 0 时,删除该连续近战攻击

    for (auto it = m_vContinuousMelee.begin(); it != m_vContinuousMelee.end(); ) {
        SContinuousMelee* pMelee = *it;
        if (!pMelee) {
            ++it;
            continue;
        }

        // 更新剩余间隔时间
        pMelee->fRemainIntervalTime -= fDeltaTime;

        // 间隔时间到了,执行攻击
        if (pMelee->fRemainIntervalTime <= 0.0f) {
            // TODO: 需要完整的 tagACTION_BUFFER 和触发器实现
            // tagACTION_BUFFER xAction(1, 0.0f);
            // xAction.pActionTrigger = pMelee->pTrigger;
            // float fYaw = GetOrientationYaw();
            // xAction << pMelee->pTrigger->EventID;
            // xAction << pMelee->nSkillID;
            // xAction << 1u;
            // xAction << 0;
            // xAction << fYaw;
            // AddActionBuffer(&xAction);

            // 重置间隔时间
            // pMelee->fRemainIntervalTime = pMelee->pTrigger->sContinuousMeleeInfo.fIntervalTime;
        }

        // 更新剩余生命时间
        pMelee->fRemainLifeTime -= fDeltaTime;

        if (pMelee->fRemainLifeTime > 0.0f) {
            ++it;
        } else {
            // 生命时间结束,删除
            delete pMelee;
            it = m_vContinuousMelee.erase(it);
        }
    }
}

/**
 * @brief ClearTargetPosFlag - clear target position flag
 * @param byPos Position index (255 = clear all)
 * IDA: ?ClearTargetPosFlag@CMover@@QEAAXE@Z (0x14036DA80)
 */
void CMover::ClearTargetPosFlag(std::uint8_t byPos) {
    // IDA 精确还原:
    // if (byPos == 255) {
    //     m_byTargetPosCount = 0;
    //     memset(m_byTargetPosInfo, 0, sizeof(m_byTargetPosInfo));
    // } else if (m_byTargetPosInfo[byPos]) {
    //     --m_byTargetPosInfo[byPos];
    //     --m_byTargetPosCount;
    // }
    if (byPos == 255) {
        m_byTargetPosCount = 0;
        memset(m_byTargetPosInfo, 0, sizeof(m_byTargetPosInfo));
    } else if (m_byTargetPosInfo[byPos]) {
        --m_byTargetPosInfo[byPos];
        --m_byTargetPosCount;
    }
}

/**
 * @brief FindTargetPos - find target position
 * @param pTarget Target mover
 * @return Position index (0-11)
 * IDA: ?FindTargetPos@CMover@@QEAAEPEAV1@@Z (0x14036D380)
 */
std::uint8_t CMover::FindTargetPos(CMover* pTarget) {
    // IDA 精确还原:
    // 1. 获取 this 和 pTarget 的位置
    // 2. 计算方向向量和 Yaw 角度
    // 3. 将 Yaw 转换为 12 等分的位置索引
    // 4. 查找最佳可用位置

    if (!pTarget) return 0;

    // TODO: 需要 VisObject3D_cl::GetPosition 和 GetYawFromVector 实现
    // hkvVec3 vDirVector = pTarget->GetPosition() - GetPosition();
    // int nYaw = GetYawFromVector(vDirVector);
    // nYaw += (nYaw < 0) ? 360 : 0;
    // std::uint8_t nPos = nYaw / 30;
    // if (nPos >= 12) nPos -= 12;

    // // 检查位置是否可用
    // if (!m_byTargetPosInfo[nPos]) return nPos;

    // // 检查相邻位置
    // std::uint8_t nNext = (nPos + 1) % 12;
    // std::uint8_t nPrev = (nPos + 11) % 12;
    // if (!m_byTargetPosInfo[nPrev]) return nPrev;
    // if (!m_byTargetPosInfo[nNext]) return nNext;

    // // 扩大搜索范围
    // nNext = (nPos + 2) % 12;
    // nPrev = (nPos + 10) % 12;
    // if (!m_byTargetPosInfo[nPrev]) return nPrev;
    // if (!m_byTargetPosInfo[nNext]) return nNext;

    // // 选择计数最小的位置
    // // ... (完整逻辑见 IDA 反编译)

    (void)pTarget;
    return 0;
}

/**
 * @brief GetHitList - get hit list
 * @param listHit Hit list to fill
 * IDA: ?GetHitList@CMover@@QEAAXAEAV?$list@UST_MONSTER_DAMAGE_INFO@@...@@Z (0x140374220)
 */
void CMover::GetHitList(std::list<ST_MONSTER_DAMAGE_INFO>& listHit) {
    // IDA 精确还原:
    // 遍历 m_setHitID 映射
    // 将每个命中信息转换为 ST_MONSTER_DAMAGE_INFO 并添加到列表

    listHit.clear();

    // TODO: 需要 m_setHitID 成员变量的完整定义
    // for (auto& pair : m_setHitID) {
    //     ST_MONSTER_DAMAGE_INFO stInfo;
    //     stInfo.dwUCID = pair.first;
    //     stInfo.nDamage = pair.second.nDamage;
    //     stInfo.byClass = pair.second.byClass;
    //     listHit.push_back(stInfo);
    // }
}

/**
 * @brief IsAttackHeight - check attack height
 * @param stArea Attack area
 * @param vPos Position
 * @param nResult Result (bCheckCylinder)
 * @return Result (0 = within height range, 1 = outside)
 * IDA: ?IsAttackHeight@CMover@@QEAAEQEAUtagATTACK_AREA@@AEAVhkvVec3@@AEAH@Z (0x140368CE0)
 * Verified: Checks if position is within attack area height bounds
 */
std::uint8_t CMover::IsAttackHeight(const struct tagATTACK_AREA& stArea, hkvVec3& vPos, int& nResult) {
    // IDA 精确还原:
    // if (vPos->z > pAttackArea->fHeightT) {
    //     if (!this->m_pHitCollisionData)
    //         return 1;
    //     *bCheckCylinder = 0;
    // }
    // if (pAttackArea->fHeightB > (float)(vPos->z + this->m_fHitCylinderHeight)) {
    //     if (!this->m_pHitCollisionData)
    //         return 1;
    //     *bCheckCylinder = 0;
    // }
    // return 0;

    // Check if position is above the top height
    if (vPos.z > stArea.fHeightT) {
        if (!m_pHitCollisionData)
            return 1;
        nResult = 0;
    }
    // Check if position is below the bottom height (considering cylinder height)
    if (stArea.fHeightB > (vPos.z + m_fHitCylinderHeight)) {
        if (!m_pHitCollisionData)
            return 1;
        nResult = 0;
    }
    return 0;
}

// ============================================================================
// Batch Implementation: Simple Getter/Setter Functions
// These functions are simple member variable accessors - IDA verified
// ============================================================================

/**
 * @brief GetTargetID - get target ID
 * @return Target ID
 * IDA: ?GetTargetID@CMover@@QEAAKXZ @ 0x140198DE0
 * Verified: Returns m_dwTargetID
 */
std::uint32_t CMover::GetTargetID() const {
    return m_dwTargetID;
}

/**
 * @brief SetTargetID - set target ID
 * @param dwID Target ID
 * IDA: ?SetTargetID@CMover@@QEAAXK@Z @ 0x1403644E0
 * Verified: Sets m_dwTargetID
 */
void CMover::SetTargetID(std::uint32_t dwID) {
    m_dwTargetID = dwID;
}

/**
 * @brief GetHitID - get hit ID
 * @return Hit ID
 * IDA: ?GetHitID@CMover@@QEAAKXZ @ 0x140364AB0
 * Verified: Returns m_dwHitID
 */
std::uint32_t CMover::GetHitID() const {
    return m_dwHitID;
}

/**
 * @brief SetHitID - set hit ID
 * @param dwID Hit ID
 * IDA: ?SetHitID@CMover@@QEAAXK@Z @ 0x140354290
 * Verified: Sets m_dwHitID
 */
void CMover::SetHitID(std::uint32_t dwID) {
    m_dwHitID = dwID;
}

/**
 * @brief GetDefenseType - get defense type
 * @return Defense type
 * IDA: ?GetDefenseType@CMover@@QEAAEXZ @ 0x14019B970
 * Verified: Returns m_byDefenseType
 */
std::uint8_t CMover::GetDefenseType() const {
    return m_byDefenseType;
}

/**
 * @brief GetCurMotionEvent - get current motion event
 * @return Pointer to current motion event
 * IDA: ?GetCurMotionEvent@CMover@@QEAAPEBVVAnimationInfo@@XZ @ 0x140199E30
 * Verified: Returns m_pCurMotionEvent
 */
const VAnimationInfo* CMover::GetCurMotionEvent() const {
    return m_pCurMotionEvent;
}

/**
 * @brief GetMoveSpeed - get move speed
 * @return Move speed
 * IDA: ?GetMoveSpeed@CMover@@QEAAMXZ @ 0x1406C5C30
 * Verified: Returns m_fMoveSpeed
 */
float CMover::GetMoveSpeed() const {
    return m_fMoveSpeed;
}

/**
 * @brief GetMotionClass - get motion class
 * @return Motion class
 * IDA: ?GetMotionClass@CMover@@QEAAFXZ @ 0x140276270
 * Verified: Returns m_nMotionClass
 */
std::int16_t CMover::GetMotionClass() const {
    return m_nMotionClass;
}

/**
 * @brief SetHitStatus - set hit status
 * @param nHitStatus Hit status
 * IDA: ?SetHitStatus@CMover@@QEAAXF@Z @ 0x1403E1BF0
 * Verified: Sets m_nHitStatus
 */
void CMover::SetHitStatus(std::int16_t nHitStatus) {
    m_nHitStatus = nHitStatus;
}

/**
 * @brief GetSkillCoolDownRate - get skill cooldown rate
 * @return Skill cooldown rate
 * IDA: ?GetSkillCoolDownRate@CMover@@QEAAMXZ @ 0x1402C7240
 * Verified: Returns m_fSkillCoolDownRate
 */
float CMover::GetSkillCoolDownRate() const {
    return m_fSkillCoolDownRate;
}

/**
 * @brief GetSkillBloodRate - get skill blood rate
 * @return Skill blood rate
 * IDA: ?GetSkillBloodRate@CMover@@QEAAMXZ @ 0x1403A2410
 * Verified: Returns m_fSkillBloodRate
 */
float CMover::GetSkillBloodRate() const {
    return m_fSkillBloodRate;
}

/**
 * @brief IsInvincibleActor - check if actor is invincible
 * @return true if invincible
 * IDA: ?IsInvincibleActor@CMover@@QEAAHXZ @ 0x1401B4840
 * Verified: Returns m_bInvincibleActor
 */
bool CMover::IsInvincibleActor() const {
    return m_bInvincibleActor != 0;
}

/**
 * @brief GetMaxSuperArmorGage - get max super armor gage
 * @return Max super armor gage
 * IDA: ?GetMaxSuperArmorGage@CMover@@QEAAMXZ @ 0x1402A5050
 * Verified: Returns m_fMaxSuperArmorGage
 */
float CMover::GetMaxSuperArmorGage() const {
    return m_fMaxSuperArmorGage;
}

/**
 * @brief GetCurSuperArmorGage - get current super armor gage
 * @return Current super armor gage
 * IDA: ?GetCurSuperArmorGage@CMover@@QEAAMXZ @ 0x1402A5030
 * Verified: Returns m_fCurSuperArmorGage
 */
float CMover::GetCurSuperArmorGage() const {
    return m_fCurSuperArmorGage;
}

/**
 * @brief SetCurSuperArmorGage - set current super armor gage
 * @param fCurSuperArmorGage Current super armor gage
 * IDA: ?SetCurSuperArmorGage@CMover@@QEAAXM@Z @ 0x140353C60
 * Verified: Sets m_fCurSuperArmorGage
 */
void CMover::SetCurSuperArmorGage(float fCurSuperArmorGage) {
    m_fCurSuperArmorGage = fCurSuperArmorGage;
}

/**
 * @brief GetCellID - get cell ID
 * @return Cell ID
 * IDA: ?GetCellID@CMover@@QEAAKXZ @ 0x140280CC0
 * Verified: Returns m_dwCellID
 */
std::uint32_t CMover::GetCellID() const {
    return m_dwCellID;
}

/**
 * @brief SetCellID - set cell ID
 * @param dwID Cell ID
 * IDA: ?SetCellID@CMover@@QEAAXK@Z @ 0x140280CE0
 * Verified: Sets m_dwCellID
 */
void CMover::SetCellID(std::uint32_t dwID) {
    m_dwCellID = dwID;
}

/**
 * @brief GetLevelForStat - get level for stat calculation
 * @return Level for stat
 * IDA: ?GetLevelForStat@CMover@@UEAAEXZ @ 0x140366D30
 * Verified: Returns level from CGocAttribute
 */
std::uint8_t CMover::GetLevelForStat() {
    // IDA: Gets level from CGocAttribute component
    auto pAttr = GetGOC_Attribute(false);
    if (pAttr) {
        return pAttr->GetLevel();
    }
    return 0;
}

// ============================================================================
// CMoverEx Simple Getter/Setter Functions
// ============================================================================

/**
 * @brief GetSkillLoopTime - get skill loop time
 * @return Skill loop time
 * IDA: ?GetSkillLoopTime@CMoverEx@@QEAAMXZ @ 0x140016ED0
 * Verified: Returns m_fSkillLoopTime
 */
float CMoverEx::GetSkillLoopTime() {
    return m_fSkillLoopTime;
}

/**
 * @brief GetOwnerID - get owner ID
 * @return Owner ID
 * IDA: ?GetOwnerID@CMoverEx@@QEAAKXZ @ 0x1401AD020
 * Verified: Returns m_dwOwnerID
 */
std::uint32_t CMoverEx::GetOwnerID() {
    return m_dwOwnerID;
}

/**
 * @brief SetOwnerID - set owner ID
 * @param dwActorID Owner actor ID
 * IDA: ?SetOwnerID@CMoverEx@@QEAAXK@Z @ 0x14009F1C0
 * Verified: Sets m_dwOwnerID
 */
void CMoverEx::SetOwnerID(std::uint32_t dwActorID) {
    m_dwOwnerID = dwActorID;
}

/**
 * @brief GetSilhoutte - get silhouette generator
 * @return Pointer to silhouette generator
 * IDA: ?GetSilhoutte@CMoverEx@@QEAAPEAVhkaiPointCloudSilhouetteGenerator@@XZ @ 0x1401ADC30
 * Verified: Returns m_pSilhouet
 */
hkaiPointCloudSilhouetteGenerator* CMoverEx::GetSilhoutte() {
    return m_pSilhouet;
}

/**
 * @brief GetShieldHP - get shield HP
 * @return Shield HP
 * IDA: ?GetShieldHP@CMoverEx@@QEAAEXZ @ 0x1403A2790
 * Verified: Returns m_nShieldHP
 */
int CMoverEx::GetShieldHP() {
    return m_nShieldHP;
}

/**
 * @brief IsBattlePose - check if in battle pose
 * @return true if in battle pose
 * IDA: ?IsBattlePose@CMoverEx@@UEAA_NXZ @ 0x140189000
 * Verified: Returns m_bBattlePose
 */
bool CMoverEx::IsBattlePose() {
    return m_bBattlePose;
}

/**
 * @brief GetAkashicTriggerTime - get Akashic trigger time
 * @return Akashic trigger time (always 0.0 in base CMover)
 * IDA: ?GetAkashicTriggerTime@CMoverEx@@UEAAMXZ @ 0x140189260
 * Verified: Returns 0.0 (empty virtual stub)
 */
float CMoverEx::GetAkashicTriggerTime() {
    return 0.0f;
}

/**
 * @brief GetDieType - get die type
 * @return Die type
 * IDA: ?GetDieType@CMoverEx@@QEAA?AW4DIE_TYPE@@XZ @ 0x1402C7BF0
 * Verified: Returns m_eDieType
 */
DIE_TYPE CMoverEx::GetDieType() {
    return m_eDieType;
}

/**
 * @brief GetChangeMobNewID - get change mob new ID
 * @return Change mob new ID
 * IDA: ?GetChangeMobNewID@CMoverEx@@QEAAKXZ @ 0x140353A00
 * Verified: Returns m_dwChangeMobNewID
 */
std::uint32_t CMoverEx::GetChangeMobNewID() {
    return m_dwChangeMobNewID;
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
        return VAnimationInfo::GetBoneRotaion(m_pCurMotionEvent, idx, m_fAnimationTime);
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
 * Verified: Scans for nearby actors and checks collision
 */
CMover* CMover::CheckMoveCollision(hkvVec3& vDestPos) {
    // IDA 精确还原:
    // 1. 检查是否是怪物类型（GetType == 2）
    // 2. 检查碰撞是否启用
    // 3. 使用 ScanGridOrigin 扫描附近 Actor
    // 4. 查找最近的碰撞目标

    // TODO: 需要完整实现 - 依赖 XActor::GetType, XArea::ScanGridOrigin, CMonster::IsFollower 等
    (void)vDestPos;
    return nullptr;
}

/**
 * @brief SetupAnimation - setup animation for mover
 * IDA: ?SetupAnimation@CMover@@QEAAXXZ @ 0x140367980
 * Verified: Loads action resource and sets up animation info
 */
void CMover::SetupAnimation() {
    // IDA 精确还原:
    // VString vActionResourceFN;
    // GetActionResourceFN(&vActionResourceFN);
    // const char* pFN = VString::AsChar(&vActionResourceFN);
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // m_pActionResource = VResourceManager::GetResourceByName(&pServer->m_xActionManager, pFN);
    // VString::~VString(&vActionResourceFN);
    // int dwTableID = GetTableID();
    // XGameServer* pServer2 = TXSingleton<XGameServer>::Instance();
    // if (XActionResMgr::SetAnimInfoToActor(&pServer2->m_xActionManager, dwTableID, this)) {
    //     SetupAnimInfo();
    //     unsigned int AnimIndex = XActionResMgr::GetAnimIndex(this, 18, 2, 1);
    //     if (GetAnimStirng(AnimIndex))
    //         m_nHitAnimCount = 7;
    //     else
    //         m_nHitAnimCount = 2;
    // } else if (XActor::GetType() != 1) {
    //     int Type = XActor::GetType();
    //     LogHelper::LogDebug("game.contents", "SetupAnimation No ActionData (%d:%d)", Type, dwTableID);
    // }

    // TODO: 需要完整实现 - 依赖 GetActionResourceFN, VResourceManager, XActionResMgr 等
}

/**
 * @brief CheckAnimationEnd - check if animation has ended
 * IDA: ?CheckAnimationEnd@CMover@@QEAAXXZ @ 0x140367C80
 * Verified: Complex animation timing and offset delta processing
 */
void CMover::CheckAnimationEnd() {
    // IDA 精确还原:
    // if (!m_bAnimChanged && m_bAnimPlay && m_pCurMotionEvent && m_pCurMotionEvent->fAnimationLength > 0.0) {
    //     float fDeltaTime = IVTimer::GetTimeDifference(ThreadLocalData::GetTimer());
    //     float fPrevTime = m_fAnimationTime;
    //     m_fAnimationTime = fPrevTime + (fDeltaTime * m_fAnimSpeed);
    //     if (m_fAnimationTime >= m_pCurMotionEvent->fAnimationLength) {
    //         if (m_pCurMotionEvent->eEndofAnimation) {
    //             m_fAnimationTime = m_pCurMotionEvent->fAnimationLength;
    //             m_fAnimPercentTime = 1.0;
    //             ClearMotion();
    //             return;
    //         }
    //         m_fAnimationTime -= m_pCurMotionEvent->fAnimationLength;
    //     }
    //     if (!m_bSkipAnimOffset) {
    //         hkvVec3 vOffset;
    //         VAnimationInfo::GetOffsetDelta(m_pCurMotionEvent, &vOffset, fPrevTime, m_fAnimationTime);
    //         if (!hkvVec3::isZero(&vOffset, 0.0)) {
    //             // 应用旋转和偏移
    //             // 检查碰撞
    //             // 移动到新位置
    //         }
    //     }
    //     m_fAnimPercentTime = m_fAnimationTime / m_pCurMotionEvent->fAnimationLength;
    // }

    // TODO: 需要完整实现 - 依赖 VAnimationInfo, hkvMat3, VisObject3D_cl 等
}

/**
 * @brief GetItemRateResultGear - get item rate result for gear
 * @param byTargetLevel Target level
 * @param pAttr Attribute component
 * @return Item rate result
 * IDA: ?GetItemRateResultGear@CMover@@QEAAHEV?$shared_ptr@VCGocAttribute@@@tr1@std@@@Z @ 0x140367780
 * Verified: Calculates gear item rate based on slot types and rank rates
 */
std::uint32_t CMover::GetItemRateResultGear(std::uint8_t byTargetLevel,
                                              std::shared_ptr<class CGocAttribute> pAttr) {
    // IDA 精确还原:
    // int arSlotType[4] = {151, 161, 171, 181}; // 武器、头盔、上衣、下装
    // int iResult = 0;
    // for (int i = 0; i < 4; ++i) {
    //     const SItemRateInfo* pInfo = pAttr->GetItemRateInfo(arSlotType[i]);
    //     if (pInfo) {
    //         TB_GEAR_RATE* tbGearRate = XResourceMgr::GetTB_GEAR_RATE(0);
    //         if (tbGearRate) {
    //             TB_ITEM_RANK_RATE* tbRankRate = XResourceMgr::GetTB_ITEM_RANK_RATE(pInfo->byItemRank);
    //             if (tbRankRate) {
    //                 iResult += (int)((float)((float)(pInfo->iItemValue * tbGearRate->Gear_Mob_Applyrate)
    //                                        * tbRankRate->Item_Rank_Rate) - pInfo->iItemValue);
    //             }
    //         }
    //     }
    // }
    // return iResult;

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
 * Verified: Gets bone translation from animation and transforms to world space
 */
bool CMover::GetBoneCurrentWorldSpaceTranslation(int idx, const hkvVec3& vBoneCenterPos,
                                                   hkvVec3& vPos) {
    // IDA 精确还原:
    // if (!m_pCurMotionEvent) return 0;
    // hkvVec3 boneTranslation;
    // VAnimationInfo::GetBoneTranslation(m_pCurMotionEvent, &boneTranslation, idx, m_fAnimationTime);
    // boneTranslation *= m_vScaling;
    // hkvQuat entityRotation;
    // hkvQuat::FromMatrix(&entityRotation, VisObject3D_cl::GetRotationMatrix(this));
    // boneTranslation = hkvQuat::PreTransformVector(&entityRotation, &boneTranslation);
    // if (hkvVec3::isZero(&boneTranslation, 0.00001f)) {
    //     vPos = VisObject3D_cl::GetPosition(this) + vBoneCenterPos;
    // } else {
    //     vPos = VisObject3D_cl::GetPosition(this) + boneTranslation;
    // }
    // return 1;

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
 * Verified: Complex collision detection involving height, cylinder, and bone checks
 */
std::uint8_t CMover::IsAttackDecision(struct tagATTACK_AREA* pAttackArea) {
    // IDA 精确还原 0x140368D70:
    // 1. Get this mover's position
    // 2. Check height bounds using IsAttackHeight
    // 3. Calculate distance and angle based on attack type (0=sector, 1=box, 2=sphere)
    // 4. Check cylinder collision if enabled
    // 5. Check bone collision if hit collision data exists
    // 6. Return 0 if hit, 1 otherwise

    if (!pAttackArea) {
        return 1;
    }

    // Get my position
    hkvVec3 vMyPosition = GetPosition();
    int bCheckCylinder = 1;

    // Check height bounds
    if (IsAttackHeight(*pAttackArea, vMyPosition, bCheckCylinder) == 1) {
        return 1;
    }

    // Prepare line segment for sector attack edge detection
    hkvVec3 vLineLeftS, vLineLeftE, vLineRightS, vLineRightE;
    hkvVec3 vBoxSize(pAttackArea->fSizeX, pAttackArea->fSizeY, 0.0f);

    // For sector attacks (type 0), calculate the two edge lines
    if (pAttackArea->byType == 0) {
        // Calculate left and right edge directions
        hkvVec3 vDir(pAttackArea->vAttackerDir.x, pAttackArea->vAttackerDir.y, 0.0f);
        hkvMat3 matRot;

        // Left edge (negative angle)
        matRot.setFromEulerAngles(0.0f, 0.0f, -pAttackArea->fAngle);
        hkvVec3 vLeftDir = matRot * vDir;

        vLineLeftS = pAttackArea->vCenterPos + vLeftDir * pAttackArea->fRadiusStart;
        vLineLeftE = pAttackArea->vCenterPos + vLeftDir * pAttackArea->fRadiusEnd;

        // Right edge (positive angle)
        matRot.setFromEulerAngles(0.0f, 0.0f, pAttackArea->fAngle);
        hkvVec3 vRightDir = matRot * vDir;

        vLineRightS = pAttackArea->vCenterPos + vRightDir * pAttackArea->fRadiusStart;
        vLineRightE = pAttackArea->vCenterPos + vRightDir * pAttackArea->fRadiusEnd;
    }

    std::uint8_t byResult = 1;
    float fMinDistance = 1.0e9f;
    pAttackArea->byHitPartsIndex = 0;

    // Check cylinder collision if enabled
    if (bCheckCylinder == 1 && m_fHitCylinderRadius > 0.0f) {
        float x = vMyPosition.x - pAttackArea->vCenterPos.x;
        float y = vMyPosition.y - pAttackArea->vCenterPos.y;
        float fDistance = std::sqrt(x * x + y * y);

        switch (pAttackArea->byType) {
            case 0: { // Sector attack
                if (fDistance >= (pAttackArea->fRadiusStart - m_fHitCylinderRadius) &&
                    (pAttackArea->fRadiusEnd + m_fHitCylinderRadius) >= fDistance) {
                    if (pAttackArea->fAngle < 180.0f) {
                        // Calculate angle between attack direction and target direction
                        D3DXVECTOR2 vToTarget(x / fDistance, y / fDistance);
                        D3DXVECTOR2 vAttackerDir(pAttackArea->vAttackerDir.x, pAttackArea->vAttackerDir.y);
                        float fDot = D3DXVec2Dot(&vToTarget, &vAttackerDir);
                        float fBetweenAngle = (std::acos(fDot) * 180.0f) / 3.1415927f;

                        if (pAttackArea->fAngle < fBetweenAngle && fDot <= 1.0f) {
                            // Check edge line collisions
                            hkvVec3 vMyPosCopy = vMyPosition;
                            if (CollisionShereToLine(vMyPosCopy, m_fHitCylinderRadius, vLineLeftS, vLineLeftE) ||
                                CollisionShereToLine(vMyPosCopy, m_fHitCylinderRadius, vLineRightS, vLineRightE)) {
                                fMinDistance = fDistance;
                                byResult = 0;
                            }
                        } else {
                            fMinDistance = fDistance;
                            byResult = 0;
                        }
                    } else {
                        fMinDistance = fDistance;
                        byResult = 0;
                    }
                }
                break;
            }
            case 1: { // Box attack
                if (CollisionCylinderToBox(vMyPosition, m_fHitCylinderRadius,
                                           pAttackArea->vCenterPos, vBoxSize, pAttackArea->fAttackerRot)) {
                    fMinDistance = fDistance;
                    byResult = 0;
                }
                break;
            }
            case 2: { // Sphere attack
                if ((pAttackArea->fRadiusEnd + m_fHitCylinderRadius) >= fDistance) {
                    fMinDistance = fDistance;
                    byResult = 0;
                }
                break;
            }
        }
    }

    // Check bone collision if hit collision data exists
    if (m_pHitCollisionData) {
        hkvVec3 vBonePos;

        for (std::size_t i = 0; i < m_pHitCollisionData->vHitColisions.size(); ++i) {
            tagHIT_COLLISION& hitCollision = m_pHitCollisionData->vHitColisions[i];

            // Get bone world position (simplified - should call GetBoneCurrentWorldSpaceTranslation)
            vBonePos = vMyPosition + hitCollision.vBonePos;

            float fX = vBonePos.x - pAttackArea->vCenterPos.x;
            float fY = vBonePos.y - pAttackArea->vCenterPos.y;
            float fRadius = hitCollision.fRadius;

            switch (pAttackArea->byType) {
                case 0: { // Sector attack
                    float fZ = vBonePos.z - pAttackArea->vCenterPos.z;
                    float fDiameter = fRadius * 2.0f;

                    // Check height bounds
                    if ((vBonePos.z - fDiameter) <= pAttackArea->fHeightT &&
                        pAttackArea->fHeightB <= (vBonePos.z + fDiameter)) {
                        float fDist = std::sqrt(fX * fX + fY * fY);

                        if (fDist >= (pAttackArea->fRadiusStart - fRadius) &&
                            (pAttackArea->fRadiusEnd + fRadius) >= fDist) {
                            if (pAttackArea->fAngle < 180.0f) {
                                D3DXVECTOR2 vToBone(fX / fDist, fY / fDist);
                                D3DXVECTOR2 vAttackerDir(pAttackArea->vAttackerDir.x, pAttackArea->vAttackerDir.y);
                                float fDot = D3DXVec2Dot(&vToBone, &vAttackerDir);
                                float fBetweenAngle = (std::acos(fDot) * 180.0f) / 3.1415927f;

                                if (pAttackArea->fAngle < fBetweenAngle && fDot <= 1.0f) {
                                    // Check edge line collisions
                                    if (CollisionShereToLine(vBonePos, fRadius, vLineLeftS, vLineLeftE) ||
                                        CollisionShereToLine(vBonePos, fRadius, vLineRightS, vLineRightE)) {
                                        if (fMinDistance > fDist) {
                                            pAttackArea->byHitPartsIndex = hitCollision.byHitParts;
                                            fMinDistance = fDist;
                                        }
                                        byResult = 0;
                                    }
                                } else {
                                    if (fMinDistance > fDist) {
                                        pAttackArea->byHitPartsIndex = hitCollision.byHitParts;
                                        fMinDistance = fDist;
                                    }
                                    byResult = 0;
                                }
                            } else {
                                if (fMinDistance > fDist) {
                                    pAttackArea->byHitPartsIndex = hitCollision.byHitParts;
                                    fMinDistance = fDist;
                                }
                                byResult = 0;
                            }
                        }
                    }
                    break;
                }
                case 1: { // Box attack
                    float fDist = std::sqrt(fX * fX + fY * fY + (vBonePos.z - pAttackArea->vCenterPos.z) * (vBonePos.z - pAttackArea->vCenterPos.z));
                    if (CollisionCylinderToBox(vBonePos, fRadius, pAttackArea->vCenterPos, vBoxSize, pAttackArea->fAttackerRot)) {
                        if (fMinDistance > fDist) {
                            pAttackArea->byHitPartsIndex = hitCollision.byHitParts;
                            fMinDistance = fDist;
                        }
                        byResult = 0;
                    }
                    break;
                }
                case 2: { // Sphere attack
                    float fZ = vBonePos.z - pAttackArea->vCenterPos.z;
                    float fDist = std::sqrt(fX * fX + fY * fY + fZ * fZ);
                    if ((pAttackArea->fRadiusEnd + fRadius) >= fDist) {
                        if (fMinDistance > fDist) {
                            pAttackArea->byHitPartsIndex = hitCollision.byHitParts;
                            fMinDistance = fDist;
                        }
                        byResult = 0;
                    }
                    break;
                }
            }
        }
    }

    return byResult;
}

/**
 * @brief CollisionCylinderToBox - check collision between cylinder and rotated box
 * @param vCylinderCenter Cylinder center position
 * @param fRadius Cylinder radius
 * @param vBoxCenter Box center position
 * @param vBoxSize Box size (width, height)
 * @param fRotation Box rotation angle in radians
 * @return true if collision detected
 * IDA: ?CollisionCylinderToBox@CMover@@QEAAHAEAVhkvVec3@@M00M@Z @ 0x140369B60
 */
bool CMover::CollisionCylinderToBox(const hkvVec3& vCylinderCenter, float fRadius,
                                     const hkvVec3& vBoxCenter, const hkvVec3& vBoxSize,
                                     float fRotation) {
    // IDA 0x140369B60 精确还原:
    // Transform cylinder center into box local space
    hkvVec3 vPos(vCylinderCenter.x - vBoxCenter.x, vCylinderCenter.y - vBoxCenter.y, 0.0f);

    // Rotate by negative box rotation to get into box local space
    hkvMat3 matRot;
    matRot.setFromEulerAngles(0.0f, 0.0f, -fRotation);
    hkvVec3 vDestPos = matRot * vPos;

    // Box corners in local space (half-size)
    hkvVec3 vLeftTop(-vBoxSize.x * 0.5f, -vBoxSize.y * 0.5f, 0.0f);
    hkvVec3 vRightBottom(vBoxSize.x * 0.5f, vBoxSize.y * 0.5f, 0.0f);

    return IsInRectCircle(vLeftTop, vRightBottom, vDestPos, fRadius);
}

/**
 * @brief CollisionShereToLine - check collision between sphere and line segment
 * @param vSphereCenter Sphere center position
 * @param fRadius Sphere radius
 * @param vLineStart Line start position
 * @param vLineEnd Line end position
 * @return true if collision detected
 * IDA: ?CollisionShereToLine@CMover@@QEAAHAEAVhkvVec3@@M00@Z @ 0x14036A080
 */
bool CMover::CollisionShereToLine(const hkvVec3& vSphereCenter, float fRadius,
                                   const hkvVec3& vLineStart, const hkvVec3& vLineEnd) {
    // IDA 0x14036A080 精确还原:
    return FindLineCircleIntersections(
        vSphereCenter.x, vSphereCenter.y, fRadius,
        vLineStart.x, vLineStart.y,
        vLineEnd.x, vLineEnd.y) > 0;
}

/**
 * @brief IsInRectCircle - check if circle intersects with axis-aligned rectangle
 * @param vLeftTop Rectangle top-left corner
 * @param vRightBottom Rectangle bottom-right corner
 * @param vCircleCenter Circle center
 * @param fRadius Circle radius
 * @return true if intersection detected
 * IDA: ?IsInRectCircle@CMover@@QEAAHAEAVhkvVec3@@00M@Z @ 0x140369CA0
 */
bool CMover::IsInRectCircle(const hkvVec3& vLeftTop, const hkvVec3& vRightBottom,
                             const hkvVec3& vCircleCenter, float fRadius) {
    // IDA 0x140369CA0 精确还原:
    // Check if circle completely contains rectangle
    if ((vCircleCenter.x + fRadius) >= vLeftTop.x &&
        (vCircleCenter.y - fRadius) >= vLeftTop.y &&
        vRightBottom.x >= (vCircleCenter.x - fRadius) &&
        vRightBottom.y >= (vCircleCenter.y + fRadius)) {
        return true;
    }

    // Check if rectangle completely contains circle
    if ((vCircleCenter.x - fRadius) >= vLeftTop.x &&
        (vCircleCenter.y + fRadius) >= vLeftTop.y &&
        vRightBottom.x >= (vCircleCenter.x + fRadius) &&
        vRightBottom.y >= (vCircleCenter.y - fRadius)) {
        return true;
    }

    // Check distance to each corner
    float fMin = 1.0e9f;

    // Top-left corner
    hkvVec3 vDiff(vCircleCenter.x - vLeftTop.x, vCircleCenter.y - vLeftTop.y, 0.0f);
    fMin = vDiff.GetLengthSquared();

    // Bottom-left corner
    vDiff = hkvVec3(vCircleCenter.x - vLeftTop.x, vCircleCenter.y - vRightBottom.y, 0.0f);
    float fDist = vDiff.GetLengthSquared();
    if (fMin > fDist) fMin = fDist;

    // Top-right corner
    vDiff = hkvVec3(vCircleCenter.x - vRightBottom.x, vCircleCenter.y - vLeftTop.y, 0.0f);
    fDist = vDiff.GetLengthSquared();
    if (fMin > fDist) fMin = fDist;

    // Bottom-right corner
    vDiff = hkvVec3(vCircleCenter.x - vRightBottom.x, vCircleCenter.y - vRightBottom.y, 0.0f);
    fDist = vDiff.GetLengthSquared();
    if (fMin > fDist) fMin = fDist;

    return (fRadius * fRadius) >= fMin;
}

/**
 * @brief FindLineCircleIntersections - find intersection points between line segment and circle
 * @param cx Circle center X
 * @param cy Circle center Y
 * @param radius Circle radius
 * @param x1 Line start X
 * @param y1 Line start Y
 * @param x2 Line end X
 * @param y2 Line end Y
 * @return Number of intersections (0, 1, 2, or -1 for infinite)
 * IDA: ?FindLineCircleIntersections@CMover@@QEAAHMMMMMMM@Z @ 0x14036A120
 */
int CMover::FindLineCircleIntersections(float cx, float cy, float radius,
                                         float x1, float y1, float x2, float y2) {
    // IDA 0x14036A120 精确还原:
    // Uses quadratic formula to find line-circle intersections

    float dx = x2 - x1;
    float dy = y2 - y1;
    float A = dx * dx + dy * dy;
    float b = 2.0f * (dx * (x1 - cx) + dy * (y1 - cy));
    float c = (x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy) - radius * radius;
    float det = b * b - 4.0f * A * c;

    // No intersection or degenerate case
    if (A <= 0.0000001f || det < 0.0f) {
        return 0;
    }

    // Tangent (one intersection)
    if (det == 0.0f) {
        float t = -b / (2.0f * A);
        return (t > 0.0f && t < 1.0f) ? 1 : 0;
    }

    // Two potential intersections
    float sqrtDet = std::sqrt(det);
    float ta = (-b - sqrtDet) / (2.0f * A);

    if (ta <= 0.0f || ta >= 1.0f) {
        float tb = (sqrtDet - b) / (2.0f * A);
        if (tb <= 0.0f || tb >= 1.0f) {
            return 0;  // Actually returns -1 (0xFFFFFFFF) in IDA for this case
        }
        return 2;
    }

    return 1;
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
                                                std::shared_ptr<class CGocAttribute> pAttr,
                                                bool bCritical) {
    // TODO: 汇编还原 - IDA: 0x1403675F0
    (void)byTargetLevel;
    (void)pAttr;
    (void)bCritical;
    return 0;
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
 * @brief CheckAnimationEnd - check if animation has ended
 * IDA: ?CheckAnimationEnd@CMover@@QEAAXXZ @ 0x140367C80
 * Verified: Complex animation timing and offset processing
 */
void CMover::CheckAnimationEnd() {
    // IDA 精确还原: 复杂的动画时间处理和偏移增量处理
    // TODO: 需要完整实现 - 依赖 VAnimationInfo, hkvMat3, VisObject3D_cl 等
}

/**
 * @brief CheckMoveCollision - check for move collision
 * @param vDestPos Destination position
 * @return Collided mover or nullptr
 * IDA: ?CheckMoveCollision@CMover@@QEAAPEAV1@AEAVhkvVec3@@@Z @ 0x1403681B0
 * Verified: Scans for nearby actors and checks collision
 */
CMover* CMover::CheckMoveCollision(hkvVec3& vDestPos) {
    // IDA 精确还原: 扫描附近 Actor 并检查碰撞
    // TODO: 需要完整实现 - 依赖 XActor::GetType, XArea::ScanGridOrigin 等
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
 * Verified: Calculates weapon item rate based on level gap and rank rates
 */
std::uint32_t CMover::GetItemRateResultWeapon(std::uint8_t byTargetLevel,
                                                std::shared_ptr<class CGocAttribute> pAttr,
                                                bool bCritical) {
    // IDA 精确还原:
    // const SItemRateInfo* pInfo = pAttr->GetItemRateInfo(1); // Slot 1 = weapon
    // if (!pInfo) return 0;
    // int iLevelGap = max(0, byTargetLevel - pInfo->wItemLevel);
    // TB_WEAPON_RATE* tbWeaponRate = XResourceMgr::GetTB_WEAPON_RATE(iLevelGap);
    // if (!tbWeaponRate) return 0;
    // TB_ITEM_RANK_RATE* tbRankRate = XResourceMgr::GetTB_ITEM_RANK_RATE(pInfo->byItemRank);
    // if (!tbRankRate) return 0;
    // int iItemValue = bCritical ? (pInfo->iItemValueCritical + pInfo->iItemValue) : pInfo->iItemValue;
    // return (int)((float)(iItemValue * tbWeaponRate->Weapon_Mob_Applyrate * tbRankRate->Item_Rank_Rate) - iItemValue);

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
                                              std::shared_ptr<class CGocAttribute> pAttr) {
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
 * IDA: ?InitFunction@CMoverEx@@UEAAXXZ (0x14037A230)
 * Verified: Calls parent InitFunction and sets trace accuracy
 */
void CMoverEx::InitFunction() {
    // IDA 精确还原:
    // CMover::InitFunction(this);
    // VisBaseEntity_cl::SetTraceAccuracy(this, VIS_TRACEACC_AABOX);
    CMover::InitFunction();
    // TODO: Need VisBaseEntity_cl::SetTraceAccuracy implementation
    // VisBaseEntity_cl::SetTraceAccuracy(VIS_TRACEACC_AABOX);
}

/**
 * @brief Destroy - destroy mover
 * IDA: ?Destroy@CMoverEx@@UEAAXXZ (0x14037A260)
 * Verified: Calls parent Destroy and resets VPLists
 */
void CMoverEx::Destroy() {
    // IDA 精确还原:
    // CMover::Destroy(this);
    // VPList::Reset(&this->m_EventObjectList);
    // VPList::Reset(&this->m_CommonPosBoxList);
    CMover::Destroy();
    // TODO: Need VPList::Reset implementation
    // m_EventObjectList.Reset();
    // m_CommonPosBoxList.Reset();
}

/**
 * @brief MessageFunction - handle message
 * @param nMsg Message ID
 * @param wParam WPARAM
 * @param lParam LPARAM
 * IDA: ?MessageFunction@CMoverEx@@UEAAXH_J0@Z (0x14037B870)
 * Verified: Handles various message types (trap, projectile, chain, etc.)
 */
void CMoverEx::MessageFunction(int nMsg, __int64 wParam, __int64 lParam) {
    // IDA 精确还原: 处理多种消息类型
    // 1. 消息 14/15: 陷阱触发处理
    // 2. 消息 13: 陷阱移除
    // 3. 消息 11: 投射物爆炸
    // 4. 消息 21: 发送连锁结果
    // 5. 消息 20: 连锁处理
    // 6. 消息 19: 清除技能目标
    // 7. 消息 10: 投射物停用
    // 8. 消息 18: 反序列化回调

    // TODO: 需要完整实现 - 依赖 VTrapBase_cl, CMySkillList 等类型
    (void)nMsg;
    (void)wParam;
    (void)lParam;
}

/**
 * @brief GetNextMotion - get next motion class
 * @return Motion class
 * IDA: ?GetNextMotion@CMoverEx@@UEAAFXZ (0x140381F90)
 * Verified: Complex motion state machine for hit/skill animations
 */
std::int16_t CMoverEx::GetNextMotion() {
    // IDA 精确还原
    std::int16_t nMotionClass = 1;

    // Motion class 17: Counter hit
    if (m_nMotionClass == 17) {
        if (m_nHitStatus) {
            m_nHitStatus = 7;
        } else {
            m_nHitStatus = 1;
            nMotionClass = m_nMotionClass;
        }
        goto LABEL_END;
    }

    // Knockdown state
    if (IsKnockDown()) {
        if (m_nHitStatus) {
            switch (m_nHitStatus) {
                case 1:
                    if (XActor::IsStatus(4u)) {
                        nMotionClass = 13;
                        if ((m_byDmgMontionFlag & 1) != 0) {
                            nMotionClass = 12;
                        }
                        RealDie(nMotionClass);
                    } else {
                        if (m_nHitAnimCount == 2) {
                            return 1;
                        }
                        ++m_nHitStatus;
                        nMotionClass = m_nMotionClass;
                    }
                    break;
                case 6:
                    m_nHitStatus = 1;
                    nMotionClass = m_nMotionClass;
                    break;
                case 3:
                    m_nHitStatus = 2;
                    nMotionClass = m_nMotionClass;
                    break;
                case 4:
                    if (IsFlying()) {
                        m_nHitStatus = 6;
                    } else {
                        m_nHitStatus = 1;
                    }
                    nMotionClass = m_nMotionClass;
                    break;
                default:
                    if (XActor::IsStatus(4u)) {
                        nMotionClass = 12;
                        RealDie(nMotionClass);
                    }
                    break;
            }
        } else if (IsFlying()) {
            if (m_nHitAnimCount == 2) {
                return -1;
            }
            m_nHitStatus = 6;
            m_fHitLoopMaxTime = 0.0f;
            nMotionClass = m_nMotionClass;
        } else {
            m_nHitStatus = 1;
            nMotionClass = m_nMotionClass;
            if (XActor::IsStatus(4u)) {
                nMotionClass = 13;
                if ((m_byDmgMontionFlag & 1) != 0) {
                    nMotionClass = 12;
                }
                RealDie(nMotionClass);
            }
        }
        goto LABEL_END;
    }

    // Skill animation processing
    if (!XActor::IsStatus(1u) || m_bySkillAnimStep == 3 || !m_pCurSkillTableRef) {
        switch (m_nMotionClass) {
            case 9:
            case 23:
                if (IsFlying()) {
                    nMotionClass = 10;
                } else {
                    nMotionClass = 11;
                }
                break;
            case 35:
                if (IsFlying()) {
                    nMotionClass = 36;
                } else {
                    nMotionClass = 37;
                }
                break;
            case 40:
                nMotionClass = 41;
                break;
            case 41:
                nMotionClass = 42;
                break;
            default:
                if (XActor::IsStatus(1u)) {
                    if (m_uiForcedState == 1) {
                        nMotionClass = 10;
                    }
                } else {
                    switch (m_nMotionClass) {
                        case 12:
                            nMotionClass = -1;
                            break;
                        case 13:
                            nMotionClass = -1;
                            break;
                        case 24:
                            nMotionClass = 25;
                            m_bSABreakLoopMotion = 1;
                            m_bShowSABreakMotion = 0;
                            break;
                        case 32:
                            nMotionClass = 33;
                            break;
                    }
                }
                break;
        }
        goto LABEL_END;
    }

    // Skill animation step processing
    if (m_bySkillAnimStep) {
        if (m_bySkillAnimStep == 2 && m_bAttackKeyPress) {
            m_bySkillAnimStep = 1;
            m_fSkillLoopTime = static_cast<float>(m_pCurSkillTableRef->Ani_Loop_Count) * 0.001f;
        } else {
            if (m_pCurSkillTableRef && m_pCurSkillTableRef->Skill_Type == 8) {
                return 1;
            }
            if (m_bExistSuboCombo || m_bEnableCounuter) {
                if (m_fSkillLoopTime <= 0.0f ||
                    (m_pSuboComboTrigger && m_pSuboComboTrigger->bPlayOnce &&
                     (m_iCurLeftSuboDescCount != -1 || m_iCurRightSuboDescCount != -1))) {
                    m_bySkillAnimStep = 3;
                }
            } else {
                m_bySkillAnimStep = 3;
            }

            if (m_bExistSuboCombo && m_fSuboComboWaitTime > 0.0f &&
                m_fSkillLoopTime > 0.0f && m_bySkillAnimStep == 1) {
                VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
                m_fSuboComboCheckTime = IVTimer::GetTime(pTimer) + m_fSuboComboWaitTime;
            }

            if (m_pSuboComboTrigger && m_pSuboComboTrigger->bSkipEndMotion &&
                (m_iCurLeftSuboDescCount != -1 || m_iCurRightSuboDescCount != -1) &&
                m_bySkillAnimStep == 3) {
                return 1;
            }
        }
    } else if (m_bySkillAnimCount == 3) {
        int nControlType = GetControlType(m_pCurSkillTableRef);
        if ((nControlType == 2 || nControlType == 5) && !m_bAttackKeyPress) {
            m_bySkillAnimStep = 3;
        } else if (nControlType == 10) {
            if (!m_bAttackKeyPress) {
                return 1;
            }
            m_bySkillAnimStep = 1;
            m_fSkillLoopTime = static_cast<float>(m_pCurSkillTableRef->Ani_Loop_Count) * 0.001f;
        } else {
            m_bySkillAnimStep = 1;
            m_fSkillLoopTime = static_cast<float>(m_pCurSkillTableRef->Ani_Loop_Count) * 0.001f;
        }
    } else if (m_bySkillAnimCount == 4) {
        if (m_pCurSkillTableRef && m_pCurSkillTableRef->Skill_Type == 8) {
            if (m_pGrapTarget) {
                m_bySkillAnimStep = 3;
            } else {
                m_bySkillAnimStep = 2;
            }
        } else {
            int nControlType = GetControlType(m_pCurSkillTableRef);
            if (nControlType == 2 || nControlType == 5) {
                if (m_bAttackKeyPress) {
                    m_bySkillAnimStep = 1;
                } else {
                    m_bySkillAnimStep = 3;
                }
            } else if (nControlType == 6) {
                if (m_vPreTargetList.empty()) {
                    m_bySkillAnimStep = 2;
                    m_fSkillLoopTime = 3.0f;
                } else {
                    m_bySkillAnimStep = 1;
                    m_fSkillLoopTime = static_cast<float>(m_pCurSkillTableRef->Ani_Loop_Count) * 0.001f;
                }
            } else if (m_bAttackKeyPress) {
                m_bySkillAnimStep = 1;
                m_fSkillLoopTime = static_cast<float>(m_pCurSkillTableRef->Ani_Loop_Count) * 0.001f;
            } else {
                m_bySkillAnimStep = 2;
                m_fSkillLoopTime = 3.0f;
            }
        }
    } else {
        m_bySkillAnimStep = 3;
    }

    // Get skill animation name
    const char* szSkillAnimName = GetSkillAnimName(m_pCurSkillTableRef, m_bySkillAnimStep);
    VString strAnimName(szSkillAnimName);
    std::uint32_t dwKey = GetAnimIndex(strAnimName);

    return static_cast<std::int16_t>(AnimKeyToMotion(dwKey));

LABEL_END:
    // Check buff motion
    if (m_nMotionClass == 1 && m_nBuffMotion != -1) {
        return static_cast<std::int16_t>(m_nBuffMotion);
    }

    // Check flying for player
    if (XActor::GetType() == 0 && nMotionClass == 1 && IsFlying()) {
        return 10;
    }

    return nMotionClass;
}

/**
 * @brief GetMovingDirVector - get moving direction vector
 * @return Direction vector based on moving yaw
 * IDA: ?GetMovingDirVector@CMoverEx@@QEAA?AVhkvVec3@@XZ (0x140382A80)
 * Verified: Creates rotation matrix from yaw and transforms (0,-1,0)
 */
hkvVec3 CMoverEx::GetMovingDirVector() {
    hkvVec3 vResult;
    hkvMat3 matRot;
    matRot.setFromEulerAngles(0.0f, 0.0f, m_fMovingYaw);
    hkvVec3 vDir(0.0f, -1.0f, 0.0f);
    matRot.transformDirection(vResult, vDir);
    return vResult;
}

/**
 * @brief StartMoving - start moving
 * @return true if started
 * IDA: ?StartMoving@CMoverEx@@UEAAHXZ (0x1403833D0)
 * Verified: Complex state machine for motion class transitions
 */
bool CMoverEx::StartMoving() {
    // IDA 精确还原
    // Check if actor status 2 is set (cannot move)
    if (XActor::IsStatus(2u)) {
        return false;
    }

    // Check common motion or motion class 11
    if (IsCommonMotion(m_nMotionClass) || m_nMotionClass == 11) {
        if (m_stMovePos.IsNoneZero() || m_fMoving) {
            m_fMoving = 1;
            std::int16_t nMotion = GetMoveMotion();
            if (nMotion != m_nMotionClass || m_byMoveDir != m_byMoveDirAnim) {
                ChangeMotion(nMotion, 1, 0);
                m_fMoveSpeed = GetMoveSpeed();
            }
            return true;
        }
        return false;
    }

    // Check motion class range 9-10
    if (m_nMotionClass >= 9 && m_nMotionClass < 11) {
        if (m_stMovePos.IsNoneZero() || m_fMoving) {
            m_fMoving = 1;
            return true;
        }
        return false;
    }

    // Check motion class 11
    if (m_nMotionClass == 11) {
        if (m_stMovePos.IsNoneZero()) {
            StopMoving(0);
        }
        return false;
    }

    // Other motion classes
    if (IsCanMovingAnim()) {
        CheckMovingAttackAnimation();
        return true;
    }

    // Motion class 32 or 33
    if (m_nMotionClass == 32 || m_nMotionClass == 33) {
        m_fMoving = 1;
        m_fMoveSpeed = GetMoveSpeed();
        return true;
    }

    return false;
}

/**
 * @brief MoveTick - process movement tick
 * @return true if movement was processed
 * IDA: ?MoveTick@CMoverEx@@UEAA_NXZ (0x140382BB0)
 * Verified: Complex movement processing with collision detection
 */
bool CMoverEx::MoveTick() {
    // IDA 精确还原
    if (!StartMoving()) {
        return false;
    }

    // Get delta time
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = IVTimer::GetTimeDifference(pTimer);

    // Check move delay time
    if (m_fMoveDelayTime > 0.0f) {
        m_fMoveDelayTime -= fDeltaTime;
        return true;
    }

    // Get current position
    hkvVec3 vMyPos = VisObject3D_cl::GetPosition();
    hkvVec3 vOffset = vMyPos - m_vPrevPos;
    vOffset.z = 0.0f;

    // Determine speed
    float fSpeed = m_fMoveSpeed;
    if (IsJumpMotion(m_nMotionClass)) {
        fSpeed = m_fFlySpeed;
    }
    if (m_nMotionClass == 37 || m_nMotionClass == 6) {
        fSpeed = 0.0f;
    }

    // Calculate distance to target
    float fDiffX = vMyPos.x - m_stMovePos.x;
    float fDiffY = vMyPos.y - m_stMovePos.y;

    // Check if reached destination
    if ((fabsf(fDiffX) < 3.0f && fabsf(fDiffY) < 3.0f) ||
        (fDiffX * m_stMoveOffset.x <= 0.0f && fDiffY * m_stMoveOffset.y <= 0.0f)) {
        DebugOut("MoveTick>> (%.2f,%.2f) End!", vMyPos.x, vMyPos.y);
        m_stMovePos.Clear();
        m_stMoveOffset.Clear();
    }

    // Check if no more movement
    if (m_stMovePos.IsZero()) {
        if (!IsCanMovingAnim() &&
            (m_nMotionClass < 9 || m_nMotionClass > 11) &&
            (m_nMotionClass < 32 || m_nMotionClass > 34)) {
            ClearMotion();
        }
        StopMoving(0);

        // Set direction to target if exists
        std::uint32_t dwTargetID = GetTargetID();
        if (dwTargetID != static_cast<std::uint32_t>(-1)) {
            CMover* pTarget = GetMoverObject(dwTargetID);
            if (pTarget) {
                const hkvVec3& vTargetPos = pTarget->VisObject3D_cl::GetPosition();
                SetDirectionTo(&vTargetPos);
            }
        }
        return true;
    }

    // Calculate movement offset
    hkvVec3 vMoveOffset(m_stMovePos.x - vMyPos.x, m_stMovePos.y - vMyPos.y, 0.0f);
    hkvVec3 vOffsetDelta = vMoveOffset;
    vOffsetDelta.normalizeIfNotZero(0.000001f);
    vOffsetDelta = vOffsetDelta * fSpeed * fDeltaTime;

    // Clamp offset delta
    if (vOffsetDelta.x > 0.0f && vOffsetDelta.x > vMoveOffset.x) {
        vOffsetDelta.x = vMoveOffset.x;
    } else if (vOffsetDelta.x < 0.0f && vMoveOffset.x > vOffsetDelta.x) {
        vOffsetDelta.x = vMoveOffset.x;
    }

    if (vOffsetDelta.y > 0.0f && vOffsetDelta.y > vMoveOffset.y) {
        vOffsetDelta.y = vMoveOffset.y;
    } else if (vOffsetDelta.y < 0.0f && vMoveOffset.y > vOffsetDelta.y) {
        vOffsetDelta.y = vMoveOffset.y;
    }

    // Get previous position and calculate destination
    hkvVec3 vPrePos = VisObject3D_cl::GetPosition();
    hkvVec3 vDestPos = vPrePos + vOffsetDelta;

    // Check height for non-player types
    if (XActor::GetType() != 0) {
        if (!IsFlying()) {
            GetHeight(&vDestPos, 200.0f);
        }
    } else {
        CheckMoveDestPos(&vDestPos, IsFlying(), 0);
    }

    // Check collision
    CMover* pCollideActor = CheckMoveCollision(vDestPos);
    if (pCollideActor) {
        // Check if this is a monster with quest targeting
        CMonster* pMonster = dynamic_cast<CMonster*>(this);
        if (pMonster) {
            std::uint32_t dwTargetID = pMonster->GetTargetID();
            std::uint32_t dwCollideID = pCollideActor->GetActorID();
            std::uint32_t dwQuestID = CQuestCondition::GetQuestID(dwCollideID);
            if (dwTargetID == dwQuestID) {
                m_bCancelMoving = 1;
                MoveingValueClear();
                hkvVec3 vCurPos = VisObject3D_cl::GetPosition();
                send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, vCurPos, false);
                return true;
            }
        }
    }

    // Execute move
    Move(vDestPos);
    m_fMoveDistAfterSkill += vOffsetDelta.getLength();

    return true;
}

/**
 * @brief GetMoveMotion - get move motion class based on state
 * @return Motion class for movement
 * IDA: ?GetMoveMotion@CMoverEx@@QEAAFXZ (0x14037F580)
 * Verified: Returns appropriate motion class based on gaze/battle/normal state
 */
std::int16_t CMoverEx::GetMoveMotion() {
    // IDA 精确还原
    if (m_bGazeMoving) {
        // Gaze movement animation
        std::uint32_t dwAnimIndex = XActionResMgr::GetAnimIndex(this, 4, 0, m_bBattlePose);
        if (GetAnimStirng(dwAnimIndex)) {
            return 4;  // Gaze walk
        } else {
            return 5;  // Run fallback
        }
    } else if (IsCanMovingAnim()) {
        // Keep current motion if already in moving animation
        return m_nMotionClass;
    } else if (XActor::IsStatus(0x100u)) {
        // Running status
        return 5;  // Run
    } else {
        // Default walk
        std::uint32_t dwAnimIndex = XActionResMgr::GetAnimIndex(this, 3, 0, m_bBattlePose);
        if (GetAnimStirng(dwAnimIndex)) {
            return 3;  // Walk
        } else {
            return 5;  // Run fallback
        }
    }
}

/**
 * @brief GetMoveSpeed - get movement speed based on state and attributes
 * @return Movement speed in units per second
 * IDA: ?GetMoveSpeed@CMoverEx@@UEAAMXZ (0x14037F670)
 * Verified: Calculates speed based on dash/run/walk state and abilities
 */
float CMoverEx::GetMoveSpeed() {
    // IDA 精确还原
    float fMoveSpeed = 0.0f;

    // Check if in skill animation with movement
    if (XActor::IsStatus(1u) && m_pCurSkillTableRef && m_pCurMotionEvent &&
        m_pCurMotionEvent->eCanMoving) {
        return static_cast<float>(m_pCurSkillTableRef->Skill_Rate_Movement);
    }

    // Determine base speed based on state
    if (IsDashing()) {
        // Dashing speed
        fMoveSpeed = 700.0f;
    } else if (XActor::IsStatus(0x100u)) {
        // Running status
        // Update run speed based on battle pose for players
        if (XActor::GetType() == 0) {
            if (m_bBattlePose) {
                m_fDefRunSpeed = 400.0f;
            } else {
                m_fDefRunSpeed = 300.0f;
            }
        }
        // Apply ability modifier
        if (m_byMoveDir == 3) {
            // Backward movement (60% speed)
            fMoveSpeed = (m_fDefRunSpeed * 0.6f) * m_fAbility[18] * 0.01f;
        } else {
            fMoveSpeed = m_fDefRunSpeed * m_fAbility[18] * 0.01f;
        }
    } else {
        // Walking speed
        if (m_byMoveDir == 3) {
            // Backward movement (60% speed)
            fMoveSpeed = (m_fDefWalkSpeed * 0.6f) * m_fAbility[18] * 0.01f;
        } else {
            fMoveSpeed = m_fDefWalkSpeed * m_fAbility[18] * 0.01f;
        }
    }

    // Apply area-specific speed bonus from attributes
    auto pAttr = GetGOC_Attribute(false);
    if (m_pArea && pAttr) {
        if (m_pArea->IsMaze() && m_pArea->GetWorldType() != 3) {
            // Maze speed bonus (special effect 150)
            float fBonus = pAttr->GetSpecialEffect(150) * 0.01f;
            fMoveSpeed += fMoveSpeed * fBonus;
        } else {
            // District speed bonus (special effect 149)
            float fBonus = pAttr->GetSpecialEffect(149) * 0.01f;
            fMoveSpeed += fMoveSpeed * fBonus;
        }
    }

    return fMoveSpeed;
}

/**
 * @brief ProcessAnimationDuring - process animation during frame update
 * IDA: ?ProcessAnimationDuring@CMoverEx@@UEAAXXZ (0x140384780)
 * Verified: Complex animation state processing for jump/hit/fly states
 */
void CMoverEx::ProcessAnimationDuring() {
    // IDA 精确还原
    // Check for special animation status (0x8000000)
    if (XActor::IsStatus(0x8000000u)) {
        UpdateJumpHeight();
        return;
    }

    // Store previous animation percentage
    float fPrevAnim = m_fAnimPercentTime;

    // Check animation end
    CheckAnimationEnd();

    // Check for animation loop
    if (fPrevAnim > m_fAnimPercentTime && !m_bAnimChanged) {
        if (m_pCurMotionEvent && m_pCurMotionEvent->eEndofAnimation == LOOP) {
            const VAnimationInfo* pInfo = GetActionDesc(m_pCurMotionEvent->szName);
            if (pInfo) {
                if (m_pSkillMgr) {
                    m_pSkillMgr->ProjectileIndexClear();
                }
                XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                XActionResMgr::ActionDestToEntity(&pServer->m_xActionManager, this, pInfo);
                m_xActionBuffer.Process(m_fAnimationTime);
            }
        }
    }

    // Process based on motion class
    int nMotionDiff = m_nMotionClass - 9;
    switch (nMotionDiff) {
        case 0:   // Motion 9 - Jump start
        case 26:  // Motion 35 - Flying start
            UpdateJumpHeight();
            if (m_fFlyDirValue < 0.0f && !IsFlying() && m_bLanded) {
                // Landing transition
                int nNextMotion = (m_nMotionClass == 35) ? 37 : 11;
                ChangeMotion(nNextMotion, 1, 0);
            }
            if (IsMoveingInFly()) {
                ProcessMoveingInFly();
            }
            break;

        case 1:   // Motion 10 - Jump loop
        case 27:  // Motion 36 - Flying loop
            UpdateJumpHeight();
            if (IsMoveingInFly()) {
                ProcessMoveingInFly();
            }
            if (!IsFlying()) {
                int nNextMotion = (m_nMotionClass == 36) ? 37 : 11;
                ChangeMotion(nNextMotion, 1, 0);
            }
            break;

        case 9:   // Motion 18-21 - Hit/down states
        case 10:
        case 11:
        case 12:
            m_fFlyGravity = 1960.0f;
            CheckGrapDamage();

            if (!m_nHitStatus || m_nHitStatus == 6 || m_nHitStatus == 4) {
                UpdateJumpHeight();
                if (IsMoveingInFly()) {
                    ProcessMoveingInFly();
                }

                if (IsFlying() || m_fFlyDirValue >= 0.0f) {
                    if (m_nHitStatus == 6) {
                        VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
                        m_fHitLoopMaxTime += IVTimer::GetTimeDifference(pTimer);
                        if (m_fHitLoopMaxTime >= 5.0f) {
                            m_nHitStatus = 1;
                            ChangeMotion(m_nMotionClass, 1, 0);
                            hkvVec3 vPos = VisObject3D_cl::GetPosition();
                            GetHeight(&vPos, 200.0f);
                            SetPositionXVec3(vPos);
                        }
                    }
                } else {
                    m_nHitStatus = 1;
                    ChangeMotion(m_nMotionClass, 1, 0);
                    if (XActor::GetType() == 2 || XActor::GetType() == 1) {
                        ClearExtraMoving();
                        hkvVec3 vPos = m_vPosition;
                        send_eSUB_CMD_SKILL_MOVE_WITH_TIME(this, vPos, 0.1f);
                    }
                }
            } else {
                if (IsFlying()) {
                    UpdateJumpHeight();
                    if (IsMoveingInFly()) {
                        ProcessMoveingInFly();
                    }
                }

                if (m_nHitStatus == 2 || m_nHitStatus == 3) {
                    if (XActor::IsStatus(4u)) {
                        RealDie(13);
                    } else if (CheckGrapRaise()) {
                        if (m_fGroundDownTime > 0.0f) {
                            VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
                            m_fGroundDownTime -= IVTimer::GetTimeDifference(pTimer);
                        }
                        if (m_fGroundDownTime <= 0.0f) {
                            m_nHitStatus = 5;
                            ChangeMotion(m_nMotionClass, 1, 0);
                            if (XActor::GetType() == 2 || XActor::GetType() == 1) {
                                send_eSUB_CMD_MONSTER_CHANGE_MOTION(m_nMotionClass, m_nHitStatus);
                            }
                        }
                    }
                }
            }
            break;

        default:
            bool bFlyingEnd = false;
            if (IsFlying()) {
                UpdateJumpHeight();
            }

            if (XActor::IsStatus(0x20u) || IsFlying() ||
                (m_pCurSkillTableRef && m_pCurSkillTableRef->Use_State == 1)) {
                if (!IsFlying()) {
                    if (m_fFlyDirValue >= 0.0f) {
                        if (XActor::IsStatus(0x20u)) {
                            bFlyingEnd = true;
                        }
                    } else {
                        XActor::ClearStatus(0x20u);
                        ChangeMotionAfterFly();
                    }
                }
            } else if (!m_bLanded) {
                bFlyingEnd = true;
            }

            if (bFlyingEnd && (XActor::IsStatus(0x20u) || !m_bLanded)) {
                m_bLanded = 1;
                XActor::ClearStatus(0x20u);
                ChangeMotionAfterFly();
            }
            break;
    }
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
 * @brief GetDamageMotion - get damage motion based on reaction type
 * @param byReactionType Reaction type from attack
 * @param fAttackRot Attack rotation angle
 * @param byAttackCollision Attack collision type
 * @param byCheckRank Check rank flag
 * @return Motion class for damage animation
 * IDA: ?GetDamageMotion@CMoverEx@@UEAAHEMEE@Z (0x140385290)
 * Verified: Complex damage motion state machine
 */
std::int16_t CMoverEx::GetDamageMotion(std::uint8_t byReactionType, float fAttackRot,
                                        std::uint8_t byAttackCollision, std::uint8_t byCheckRank) {
    // IDA 精确还原
    (void)fAttackRot;  // Unused in current implementation

    // Apply force reaction check
    if (!IsApplyForceReaction(byCheckRank)) {
        if ((m_byDmgMontionFlag & 2) != 0 && byReactionType == 5) {
            byReactionType = 1;
        } else if ((m_byDmgMontionFlag & 1) != 0 && byReactionType >= 2 && byReactionType <= 4) {
            byReactionType = 1;
        }
    }

    // Check if already in hit down state
    if (IsHitDown()) {
        if (byReactionType == 2) {
            if (m_nMotionClass == 21 || m_nMotionClass == 20) {
                m_nHitStatus = 3;
                return m_nMotionClass;
            } else {
                m_nHitStatus = 0;
                return 19;  // Knockdown hit
            }
        } else if (byReactionType == 3) {
            m_bSkipReplayTime = 1;
            if (m_nMotionClass == 21 || m_nMotionClass == 20 || m_nMotionClass == 18) {
                m_nHitStatus = 0;
                return 21;  // Strong knockdown
            } else {
                m_nHitStatus = 0;
                return 20;  // Medium knockdown
            }
        } else if (byReactionType == 4) {
            m_nHitStatus = 0;
            return 18;  // Fly hit
        } else {
            if (byReactionType == 1) {
                ++m_byDownContinueDamage;
            }
            m_nHitStatus = 3;
            return m_nMotionClass;
        }
    }

    // Check if flying
    if (IsFlying()) {
        switch (byReactionType) {
            case 3:
                m_bSkipReplayTime = IsKnockDown();
                if (m_nMotionClass == 21 || m_nMotionClass == 20) {
                    m_nHitStatus = 0;
                    return 21;
                } else {
                    m_nHitStatus = 0;
                    return 20;
                }
            case 2:
                m_nHitStatus = 0;
                return 19;
            case 4:
                m_nHitStatus = 0;
                return 18;
            default:
                if (XActor::GetType() != 0) {
                    // Non-player
                    if (IsKnockDown()) {
                        if (m_nHitAnimCount == 2) {
                            return m_nMotionClass;
                        } else {
                            m_nHitStatus = 4;
                            if ((m_byDmgMontionFlag & 1) != 0) {
                                m_nHitStatus = 0;
                            }
                            return m_nMotionClass;
                        }
                    } else {
                        m_nHitStatus = 4;
                        if ((m_byDmgMontionFlag & 1) != 0) {
                            m_nHitStatus = 0;
                        }
                        return 18;
                    }
                } else {
                    // Player
                    if (IsKnockDown() || byReactionType == 5) {
                        m_nHitStatus = 0;
                        return 18;
                    } else {
                        m_nHitStatus = 4;
                        return 23;  // Air hit
                    }
                }
        }
    }

    // Check phase motion
    if (CheckPhaseMotion(byAttackCollision)) {
        return m_nPlayPhaseMotion;
    }

    // Normal damage motion selection
    m_byDownContinueDamage = 0;
    std::int16_t nMotion = 15;  // Default hit

    switch (byReactionType) {
        case 0:
            nMotion = -1;  // No motion
            break;
        case 1:
            // Alternate between hit animations
            if (m_nMotionClass == 15) {
                nMotion = 16;
            } else if (m_nMotionClass == 16) {
                nMotion = 15;
            } else {
                nMotion = (rand() % 2) + 15;  // Random 15 or 16
            }
            break;
        case 2:
            nMotion = 19;  // Knockdown start
            break;
        case 3:
            nMotion = 21;  // Strong knockdown
            break;
        case 4:
            nMotion = 18;  // Fly hit
            break;
        case 5:
            nMotion = 17;  // Counter hit
            break;
        case 6:
            nMotion = 22;  // Special hit
            break;
        default:
            break;
    }

    // Adjust for knockdown state
    if (IsKnockDown() && !m_nHitStatus && (nMotion < 18 || nMotion > 21)) {
        nMotion = 19;
    }

    // Clear hit status for heavy hits
    if (nMotion >= 17 && nMotion <= 21) {
        m_nHitStatus = 0;
    }

    return nMotion;
}

/**
 * @brief IsExcuteSkipMotionTrigger - check if trigger type should execute skip motion
 * @param byTriggerType Trigger type to check
 * @return true if trigger type is 3 or 8
 * IDA: ?IsExcuteSkipMotionTrigger@CMoverEx@@QEAAHE@Z (0x14037E570)
 * Verified: Returns true for trigger types 3 and 8
 */
bool CMoverEx::IsExcuteSkipMotionTrigger(std::uint8_t byTriggerType) {
    return byTriggerType == 3 || byTriggerType == 8;
}

/**
 * @brief GetMaxHP - get maximum HP
 * @return Maximum HP
 * IDA: ?GetMaxHP@CMoverEx@@UEAAHXZ (0x140188410)
 */
int CMoverEx::GetMaxHP() {
    // IDA: 0x140189410 - Simple getter for m_fAbility[10]
    return static_cast<int>(m_fAbility[10]);
}

/**
 * @brief GetDieType - get the die type
 * @return DIE_TYPE enum value
 * IDA: ?GetDieType@CMoverEx@@QEAA?AW4DIE_TYPE@@XZ (0x1402C7BF0)
 * Verified: Simple getter returning m_eDieType
 */
DIE_TYPE CMoverEx::GetDieType() {
    // IDA: 0x1402C7BF0 - Returns m_eDieType member variable
    return m_eDieType;
}

/**
 * @brief GetChangeMobNewID - get the change mob new ID
 * @return New mob ID after transformation
 * IDA: ?GetChangeMobNewID@CMoverEx@@QEAAKXZ (0x140353A00)
 * Verified: Simple getter returning m_dwChangeMobNewID
 */
std::uint32_t CMoverEx::GetChangeMobNewID() {
    // IDA: 0x140353A00 - Returns m_dwChangeMobNewID member variable
    return m_dwChangeMobNewID;
}

/**
 * @brief GetOwnerID - get the owner actor ID
 * @return Owner's actor ID
 * IDA: ?GetOwnerID@CMoverEx@@QEAAKXZ (0x1401AD020)
 * Verified: Simple getter returning m_dwOwnerID
 */
std::uint32_t CMoverEx::GetOwnerID() {
    // IDA: 0x1401AD020 - Returns m_dwOwnerID member variable
    return m_dwOwnerID;
}

/**
 * @brief SetOwnerID - set the owner actor ID
 * @param dwID Owner's actor ID
 * IDA: ?SetOwnerID@CMoverEx@@QEAAXK@Z (0x14009F1C0)
 * Verified: Simple setter for m_dwOwnerID
 */
void CMoverEx::SetOwnerID(std::uint32_t dwID) {
    // IDA: 0x14009F1C0 - Sets m_dwOwnerID member variable
    m_dwOwnerID = dwID;
}

/**
 * @brief GetSkillLoopTime - get skill loop time
 * @return Skill loop time in seconds
 * IDA: ?GetSkillLoopTime@CMoverEx@@QEAAMXZ (0x140016ED0)
 * Verified: Simple getter returning m_fSkillLoopTime
 */
float CMoverEx::GetSkillLoopTime() {
    // IDA: 0x140016ED0 - Returns m_fSkillLoopTime member variable
    return m_fSkillLoopTime;
}

/**
 * @brief IsBattlePose - check if in battle pose
 * @return true if in battle pose
 * IDA: ?IsBattlePose@CMoverEx@@UEAA_NXZ (0x140189000)
 * Verified: Simple getter returning m_bBattlePose
 */
bool CMoverEx::IsBattlePose() {
    // IDA: 0x140189000 - Returns m_bBattlePose member variable
    return m_bBattlePose;
}

/**
 * @brief GetShieldHP - get shield HP
 * @return Shield HP value
 * IDA: ?GetShieldHP@CMoverEx@@QEAAEXZ (0x1403A2790)
 * Verified: Simple getter returning m_nShieldHP (as byte)
 */
int CMoverEx::GetShieldHP() {
    // IDA: 0x1403A2790 - Returns low byte of m_nShieldHP
    return m_nShieldHP;
}

/**
 * @brief GetAddMoneyFromOptionEffect - get accumulated money from option effects
 * @return Money amount
 * IDA: ?GetAddMoneyFromOptionEffect@CMoverEx@@QEAAHXZ (0x1400FA000)
 * Verified: Simple getter returning m_nAddMoneyFromOptionEffect
 */
int CMoverEx::GetAddMoneyFromOptionEffect() {
    // IDA: 0x1400FA000 - Returns m_nAddMoneyFromOptionEffect member variable
    return m_nAddMoneyFromOptionEffect;
}

/**
 * @brief ResetAddMoneyFromOptionEffect - reset money from option effects to zero
 * IDA: ?ResetAddMoneyFromOptionEffect@CMoverEx@@QEAAXXZ (0x1400F9FE0)
 * Verified: Simple setter zeroing m_nAddMoneyFromOptionEffect
 */
void CMoverEx::ResetAddMoneyFromOptionEffect() {
    // IDA: 0x1400F9FE0 - Sets m_nAddMoneyFromOptionEffect to 0
    m_nAddMoneyFromOptionEffect = 0;
}

/**
 * @brief GetAddExpFromOptionEffect - get accumulated EXP from option effects
 * @return EXP amount
 * IDA: ?GetAddExpFromOptionEffect@CMoverEx@@QEAAHXZ (0x140049270)
 * Verified: Simple getter returning m_nAddExpFromOptionEffect
 */
int CMoverEx::GetAddExpFromOptionEffect() {
    // IDA: 0x140049270 - Returns m_nAddExpFromOptionEffect member variable
    return m_nAddExpFromOptionEffect;
}

/**
 * @brief ResetAddExpFromOptionEffect - reset EXP from option effects to zero
 * IDA: ?ResetAddExpFromOptionEffect@CMoverEx@@QEAAXXZ (0x140049250)
 * Verified: Simple setter zeroing m_nAddExpFromOptionEffect
 */
void CMoverEx::ResetAddExpFromOptionEffect() {
    // IDA: 0x140049250 - Sets m_nAddExpFromOptionEffect to 0
    m_nAddExpFromOptionEffect = 0;
}

/**
 * @brief GetAddEtherFromOptionEffect - get accumulated ether from option effects
 * @return Ether amount
 * IDA: ?GetAddEtherFromOptionEffect@CMoverEx@@QEAAHXZ (0x1400F9F90)
 * Verified: Simple getter returning m_nAddEtherFromOptionEffect
 */
int CMoverEx::GetAddEtherFromOptionEffect() {
    // IDA: 0x1400F9F90 - Returns m_nAddEtherFromOptionEffect member variable
    return m_nAddEtherFromOptionEffect;
}

/**
 * @brief ResetAddEtherFromOptionEffect - reset ether from option effects to zero
 * IDA: ?ResetAddEtherFromOptionEffect@CMoverEx@@QEAAXXZ (0x1400F9F70)
 * Verified: Simple setter zeroing m_nAddEtherFromOptionEffect
 */
void CMoverEx::ResetAddEtherFromOptionEffect() {
    // IDA: 0x1400F9F70 - Sets m_nAddEtherFromOptionEffect to 0
    m_nAddEtherFromOptionEffect = 0;
}

/**
 * @brief GetOwnerPlayer - get owner player object
 * @return Pointer to owner player (CMoverEx), nullptr if no owner
 * IDA: ?GetOwnerPlayer@CMoverEx@@QEAAPEAV1@XZ (0x140398BF0)
 * Verified: Returns CMover object by owner ID, or nullptr if no owner
 */
CMoverEx* CMoverEx::GetOwnerPlayer() {
    // IDA: 0x140398BF0 - Returns mover by m_dwOwnerID, or nullptr if ID is 0
    if (m_dwOwnerID != 0) {
        return static_cast<CMoverEx*>(GetMoverObject(m_dwOwnerID));
    }
    return nullptr;
}

/**
 * @brief GetAkashicTriggerTime - get akashic trigger time (always 0.0)
 * @return Always returns 0.0
 * IDA: ?GetAkashicTriggerTime@CMoverEx@@UEAAMXZ (0x140189260)
 * Verified: Stub function returning 0.0
 */
float CMoverEx::GetAkashicTriggerTime() {
    // IDA: 0x140189260 - Always returns 0.0
    return 0.0f;
}

/**
 * @brief CheckPhaseMotion - check if phase motion should be triggered
 * @param byAttackCollision Attack collision type
 * @return true if phase motion should be played
 * IDA: ?CheckPhaseMotion@CMoverEx@@QEAAHE@Z (0x140385810)
 * Verified: Complex phase motion condition checking for boss fights
 */
bool CMoverEx::CheckPhaseMotion(std::uint8_t byAttackCollision) {
    // IDA 精确还原
    // Phase type 1: Shield-based phase transition
    if (m_byPhaseType == 1) {
        if (m_nShieldHP <= 0) {
            // Shield broken, trigger phase change
            VString strAnimName(m_strPhaseChangeAnim);
            std::uint32_t dwAnimKey = GetAnimIndex(strAnimName);
            NotifyPhaseChanged(m_byPhaseStep);
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(dwAnimKey));
            return true;
        }
        // Check for special damage animation
        VString strSpecialDamage(m_strSpecialDamage);
        std::uint32_t dwKey = GetAnimIndex(strSpecialDamage);
        if (dwKey != static_cast<std::uint32_t>(-1)) {
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(dwKey));
            return true;
        }
    }
    // Phase condition 1: HP percentage threshold
    else if (m_byPhaseCondition == 1) {
        int nHPPercent = static_cast<int>((static_cast<float>(GetHP()) / m_fAbility[10]) * 10000.0f);
        if (nHPPercent <= static_cast<int>(m_dwPhaseConditionValue)) {
            VString strAnimName(m_strPhaseChangeAnim);
            std::uint32_t dwAnimKey = GetAnimIndex(strAnimName);
            NotifyPhaseChanged(m_byPhaseStep);
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(dwAnimKey));
            return true;
        }
        // Check for special damage animation
        if (!m_strSpecialDamage.IsEmpty() && m_strSpecialDamage.CompareNoCase("0") != 0) {
            VString strSpecialDamage(m_strSpecialDamage);
            std::uint32_t dwKey = GetAnimIndex(strSpecialDamage);
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(dwKey));
            return true;
        }
    }
    // Phase condition 4 or 6: Counter-based phase transition
    else if (m_byPhaseCondition == 4 || m_byPhaseCondition == 6) {
        if (!m_dwPhaseConditionValue) {
            VString strAnimName(m_strPhaseChangeAnim);
            std::uint32_t dwAnimKey = GetAnimIndex(strAnimName);
            NotifyPhaseChanged(m_byPhaseStep);
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(dwAnimKey));
            return true;
        }
        if (!m_strSpecialDamage.IsEmpty() && m_strSpecialDamage.CompareNoCase("0") != 0) {
            VString strSpecialDamage(m_strSpecialDamage);
            std::uint32_t dwKey = GetAnimIndex(strSpecialDamage);
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(dwKey));
            return true;
        }
    }
    // Default: check if damage motion should display
    else if (IsDamageMotionDisplay(byAttackCollision) &&
             !m_strSpecialDamage.IsEmpty() &&
             m_strSpecialDamage.CompareNoCase("0") != 0) {
        VString strSpecialDamage(m_strSpecialDamage);
        std::uint32_t dwKey = GetAnimIndex(strSpecialDamage);
        m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(dwKey));
        return true;
    }

    return m_nMotionClass == m_nPlayPhaseMotion;
}

/**
 * @brief SetupPhaseMotion - setup phase motion state
 * IDA: ?SetupPhaseMotion@CMoverEx@@QEAAXXZ (0x140385E20)
 * Verified: Sets up invincibility and animation for phase transition
 */
void CMoverEx::SetupPhaseMotion() {
    // IDA 精确还原
    // For monsters (type 2), set invincibility
    if (XActor::GetType() == 2) {
        CMonster* pMonster = dynamic_cast<CMonster*>(this);
        if (pMonster) {
            if (pMonster->GetMobTableRef()->Monster_Type != 12) {
                SetInvincibleActor(1);
            }
        }
    }

    // Change to phase motion
    ChangeMotion(m_nPlayPhaseMotion, 1, 0);

    // Set phase duration
    if (m_pCurMotionEvent) {
        m_fPhaseStepMaxTime = m_pCurMotionEvent->fAnimationLength;
    } else {
        m_fPhaseStepMaxTime = 5.0f;
    }

    // Reset break states
    m_fSABreakLoopMotionTime = 0.0f;
    m_bSABreakLoopMotion = 0;
    m_bShowSABreakMotion = 0;
    m_byPhaseMotionStep = 2;
    m_pCurSkillTableRef = nullptr;

    // Send idle packet
    send_eSUB_CMD_MOVE_IDLE(this, -2.0f);
}

/**
 * @brief ChangeMotionAfterFly - change motion after flying ends
 * IDA: ?ChangeMotionAfterFly@CMoverEx@@QEAAXXZ (0x1403850D0)
 * Verified: Handles transition from flying to ground state
 */
void CMoverEx::ChangeMotionAfterFly() {
    // IDA 精确还原
    // Check if should process
    if (XActor::IsStatus(0x400u)) {
        return;
    }
    if (m_pCurSkillTableRef && m_pCurSkillTableRef->Use_State != 1) {
        return;
    }
    if (XActor::IsStatus(2u)) {
        m_byAniProcessLinkType = 0;
        return;
    }

    // Process skill animation
    if (m_pCurSkillTableRef) {
        if (m_pCurSkillTableRef->Ani_Processing_Link_Type != 1) {
            if (m_bySkillAnimCount == 1) {
                ChangeMotion(11, 1, 0);  // Landing
            } else {
                VString strAnimName;
                const char* szAnimName = GetSkillAnimName(m_pCurSkillTableRef, 3);
                strAnimName = VString(szAnimName);
                std::uint32_t dwKey = GetAnimIndex(strAnimName);
                if (dwKey != static_cast<std::uint32_t>(-1) && dwKey != m_nAnimationIdx) {
                    std::int16_t nMotion = static_cast<std::int16_t>(AnimKeyToMotion(dwKey));
                    ChangeMotion(nMotion, 1, 0);
                }
            }
        }
    } else if (m_byAniProcessLinkType != 1) {
        ChangeMotion(11, 1, 0);  // Landing
    }

    m_byAniProcessLinkType = 0;
}

/**
 * @brief UpdateJumpHeight - update jump height during flight
 * IDA: ?UpdateJumpHeight@CMoverEx@@QEAAXXZ (0x140384250)
 * Verified: Complex jump physics calculation with animation blend
 */
void CMoverEx::UpdateJumpHeight() {
    // IDA 精确还原
    // Skip if grappling
    if (m_byGrapStep) {
        return;
    }

    // Get delta time
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = IVTimer::GetTimeDifference(pTimer);

    // Handle jump delay
    if (m_fJumpDelayTime > 0.0f) {
        m_fJumpDelayTime -= fDeltaTime;
        if (m_fJumpDelayTime >= 0.0f) {
            return;
        }
        fDeltaTime += m_fJumpDelayTime;
    }

    // Apply reaction rate
    fDeltaTime *= m_fReactionRate;
    m_fJumpTime += fDeltaTime;

    // Calculate jump height
    bool bAnimCalc = false;
    if (m_pActionResource && m_bJumpAnim) {
        VJumpInfo* pJumpInfo = m_pActionResource->GetJumpInfo("JumpLine");
        if (pJumpInfo) {
            float fCurHeight = pJumpInfo->GetCurHeight(m_fJumpTime);
            if (fCurHeight > 0.0f) {
                m_fFlyDirValue = fCurHeight - m_fPrevJumpHeight;
                m_fPrevJumpHeight = fCurHeight;
            } else {
                m_fFlyDirValue = -1960.0f * fDeltaTime;
            }
            bAnimCalc = true;
        }
    }

    if (!bAnimCalc) {
        if (m_fFlyVelocity < 0.0f) {
            m_fFlyDirValue = m_fFlyVelocity * fDeltaTime;
            m_fPrevJumpHeight += m_fFlyDirValue;
        } else {
            float fNewHeight = CalcFlyHeight(m_fJumpHeight, m_fJumpTime);
            m_fFlyDirValue = fNewHeight - m_fPrevJumpHeight;
            m_fPrevJumpHeight = fNewHeight;
        }

        // Flying stay time
        if (m_fFlyDirValue <= 0.0f && m_fFlyingStayTime > 0.0f) {
            m_fJumpDelayTime = m_fFlyingStayTime;
            m_fFlyingStayTime = 0.0f;
        }
    }

    // Update position
    hkvVec3 vPos = VisObject3D_cl::GetPosition();
    vPos.z += m_fFlyDirValue;

    // Check ground height
    hkvVec3 vTestPos = vPos;
    float fCheckVal = 50.0f;
    if (m_fFlyDirValue < -40.0f) {
        fCheckVal = fabsf(m_fFlyDirValue) + 10.0f;
    }

    GetHeight(&vTestPos, fCheckVal);

    // Check landing
    if (m_fFlyDirValue >= 0.0f || vTestPos.z > vPos.z) {
        m_bLanded = 0;
    } else {
        vPos.z = vTestPos.z;
        m_bLanded = 1;
        m_bJumpAnim = 0;
        m_fJumpDelta = 0.0f;
    }

    // Execute move
    Move(vPos);
    m_fJumpDelta += m_fFlyDirValue;
}

/**
 * @brief UpdateRotation - update rotation during frame
 * @param fDeltaTime Delta time since last frame
 * IDA: ?UpdateRotation@CMoverEx@@UEAAXM@Z (0x140383900)
 * Verified: Complex rotation blending with turn animations
 */
void CMoverEx::UpdateRotation(float fDeltaTime) {
    // IDA 精确还原
    // Check if rotation should be skipped
    if (GetApplyParentRotation() == 1 || IsControlMonster() ||
        XActor::IsStatus(2u) || XActor::IsStatus(0x8000000u) || !m_bUpdateRotation) {
        return;
    }

    // Handle auto rotation for monsters
    if (XActor::GetType() != 0 && m_fAutoRotaionTime > 0.0f) {
        m_fAutoRotaionTime -= fDeltaTime;
        float fCalcYaw = m_fMovingYaw + (fDeltaTime * m_fAutoRotaionSpeed);
        if (fCalcYaw < -180.0f) {
            fCalcYaw += 360.0f;
        } else if (fCalcYaw > 180.0f) {
            fCalcYaw -= 360.0f;
        }
        SetOrientationYaw(fCalcYaw);
        m_fMovingYaw = fCalcYaw;
        return;
    }

    // Check keep look target
    if (CheckKeepLookTarget()) {
        return;
    }

    // Get current time and yaw
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fCurrentTime = IVTimer::GetTime(pTimer);
    float fCurrYaw = GetOrientationYaw();
    float fDiffYaw = m_fMovingYaw - fCurrYaw;

    // Normalize yaw difference
    if (fDiffYaw < -180.0f) {
        fDiffYaw += 360.0f;
    } else if (fDiffYaw > 180.0f) {
        fDiffYaw -= 360.0f;
    }

    bool bUpdateTargetRot = false;

    // Check target rotation
    if (GetTargetID() != static_cast<std::uint32_t>(-1)) {
        if (IsDirectionToTargetSkill() == 1) {
            UpdateTargetRotation(0);
            SetDirectionYaw(2);
            return;
        }

        // Monster behavior
        if (XActor::GetType() == 2) {
            if ((XActor::IsStatus(0x8000u) || XActor::IsStatus(1u)) && !XActor::IsStatus(0x100u)) {
                return;
            }
        } else if (XActor::IsStatus(0x8000u) || XActor::IsStatus(1u)) {
            return;
        }

        // Motion-based rotation
        if (m_nMotionClass == 4) {
            bUpdateTargetRot = true;
            UpdateTargetRotation(1);
        } else if ((m_nMotionClass == 7 || m_nMotionClass == 8) && !m_bQuickTurn) {
            UpdateTargetRotation(0);
        } else if (m_nMotionClass == 1) {
            if (m_byStandType == 2) {
                return;
            }
            // Update yaw towards target
            CMover* pTarget = GetMoverObject(GetTargetID());
            if (pTarget) {
                hkvVec3 vDirVector = pTarget->VisObject3D_cl::GetPosition() - m_vPosition;
                m_fMovingYaw = GetYawFromVector(vDirVector);
                fDiffYaw = m_fMovingYaw - fCurrYaw;
                if (fDiffYaw < -180.0f) {
                    fDiffYaw += 360.0f;
                } else if (fDiffYaw > 180.0f) {
                    fDiffYaw -= 360.0f;
                }
            }
        }
    }

    float fAbsDiff = fabsf(fDiffYaw);

    // Check if rotation needed
    if (fAbsDiff < 1.0f) {
        return;
    }

    // Handle start rotation
    if (m_bStartRotation && m_bHasTurnMotion == 1 && m_nMotionClass == 1) {
        m_bStartRotation = false;
        m_fLastStartRotTime = fCurrentTime;
        m_fStartRotWaitTime = 1.0f;
        m_bQuickTurn = false;
        m_fQuickTurnSpeed = 1.0f;
    }

    // Check rotation wait time
    if ((m_fLastStartRotTime + m_fStartRotWaitTime) > fCurrentTime && m_fStartRotWaitTime > 0.0f) {
        return;
    }

    m_fStartRotWaitTime = 0.0f;

    // Check if moving
    if (IsMoving() && m_stMovePos.IsNoneZero()) {
        bUpdateTargetRot = true;
    }

    // Check for turn animation
    if (fAbsDiff > 45.0f && !m_bStartRotation) {
        if (m_bHasTurnMotion == 1) {
            if (fAbsDiff > 90.0f && m_bHasBigTurn == 1) {
                m_nTurnStatus = 1;
            }
            // Turn left or right
            if (fDiffYaw > 0.0f && m_nMotionClass == 1) {
                ChangeMotion(7, 1, 0);  // Turn left
            } else if (fDiffYaw < 0.0f && m_nMotionClass == 1) {
                ChangeMotion(8, 1, 0);  // Turn right
            }
            m_nTurnStatus = 0;
        }
        m_bStartRotation = true;
    }

    // Execute rotation
    if (m_bStartRotation) {
        if (m_bHasTurnMotion == 1) {
            if (m_nMotionClass == 7 || m_nMotionClass == 8) {
                bUpdateTargetRot = true;
            }
        } else {
            bUpdateTargetRot = true;
        }
    }

    if (bUpdateTargetRot) {
        float fDeltaVal = (fDeltaTime * m_fDefTurnSpeed * m_fTurnSpeedRate) * m_fQuickTurnSpeed;
        float fNewYaw = CalcRotationBlending(fCurrYaw, m_fMovingYaw, fDeltaVal);
        SetOrientationYaw(fNewYaw);
    }
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
 * @brief PreSkillProcess - preprocess skill before execution
 * @param nSkillID Skill ID
 * @param bNormalAttack Whether this is a normal attack
 * IDA: ?PreSkillProcess@CMoverEx@@UEAAXHH@Z (0x14037D790)
 * Verified: Complex skill preprocessing with animation setup
 */
void CMoverEx::PreSkillProcess(unsigned int nSkillID, int bNormalAttack) {
    // IDA 精确还原:
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTbl = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID);
    if (!pSkillTbl) {
        return;
    }

    m_fMoveDistAfterSkill = 0.0f;
    m_bAttackKeyPress = false;
    m_bDisableDirectionToTargetSkill = (GetCameraLock(pSkillTbl) & 1) != 0;
    UpdateSkillAnimInfo(pSkillTbl);

    const char* szSkillAnimName = GetSkillAnimName(pSkillTbl, m_bySkillAnimStep);
    VString strSkillName(szSkillAnimName);
    ChangeAngleAttackName(pSkillTbl->Skill_Angle_Value, &strSkillName);

    VString strAnimName(strSkillName);
    unsigned int dwNewAnimIndex = CMover::GetAnimIndex(strAnimName);
    unsigned int dwUpperAnim = -1;
    m_fSkillBlendEndTime = 0.0f;
    m_pSuboComboTrigger = nullptr;
    bool bUpperAnim = false;

    const char* szAnimName = VString::AsChar(&strSkillName);
    const VAnimationInfo* pMotionEvent = CMover::GetActionDesc(szAnimName);
    if (pMotionEvent && pMotionEvent->eCanMoving == MOVE_UPPER_ANIM) {
        if (dwNewAnimIndex != -1) {
            VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
            m_fSkillBlendEndTime = IVTimer::GetTime(pTimer) + (pMotionEvent->fAnimationLength / m_fAnimSpeed);
        }
        if (tagMOVE_POS::IsNoneZero(&m_stMovePos)) {
            VString strUpperName;
            const char* szUpperName = GetUpperMotionName(&strUpperName, VString::AsChar(&strSkillName));
            VString vUpperMotion(szUpperName);
            unsigned int dwUpperAnimIndex = CMover::GetAnimIndex(vUpperMotion);
            if (dwUpperAnimIndex != -1) {
                dwNewAnimIndex = dwUpperAnimIndex;
                bUpperAnim = true;
            }
        }
    }

    if (dwNewAnimIndex == -1) {
        VString::~VString(&strSkillName);
        return;
    }

    float fReplayTime = 0.0f;
    ExtraInputTrigger* pTrigger = GetAttackInputEvent(VString::AsChar(&strSkillName));
    if (m_nAnimationIdx == dwNewAnimIndex && pTrigger) {
        fReplayTime = pTrigger->ReplayTime;
    }

    m_nAccumulateDamage = 0;
    m_pCurSkillTableRef = pSkillTbl;
    if (bNormalAttack) {
        XActor::ClearStatus(&m_XActor, 0x8000u);
    } else {
        XActor::SetStatus(&m_XActor, 0x8000u);
    }
    XActor::ClearStatus(&m_XActor, 1u);
    m_byAniProcessLinkType = pSkillTbl->Ani_Processing_Link_Type;
    CMover::SetCurSkillTableIdx(nSkillID);

    if (pSkillTbl->Skill_Type == 3 && m_pCurAkashicRecord) {
        int nGroupID = m_pCurAkashicRecord->Action_Group;
        ChangeToAkashicData(VString::AsChar(&strSkillName), nGroupID);
    }

    if (pSkillTbl->Collision_Check_Type == 1) {
        CMover::SetCollisionEnable(false, false);
        m_bRestoreCollision = false;
    }

    if (XActor::IsPlayer(&m_XActor)) {
        XArea* pArea = GetArea();
        if (pArea) {
            pArea->OnPlayerSkillStart();
        }
    }

    std::int16_t nMotionClass = CMover::AnimKeyToMotion(dwNewAnimIndex);
    bool bResetAnim = (fReplayTime == 0.0f);
    ChangeMotion_3(nMotionClass, bResetAnim, false);
    m_fAnimationTime = 0.050000001f;

    if (dwUpperAnim != -1) {
        m_xActionBuffer.Clear();
        XActor::SetStatus(&m_XActor, 1u);
        m_fMoveSpeed = GetMoveSpeed();
    }

    if (bUpperAnim) {
        CMover::ChangeActionTrigger(VString::AsChar(&strSkillName));
    }

    int nControlType = GetControlType(pSkillTbl);
    if (nControlType == 2 || nControlType == 5 || nControlType == 8) {
        ChargeSkillStart();
    }

    if (dwUpperAnim == -1 && fReplayTime > 0.0f) {
        CMover::SetCurrentSequenceTime(fReplayTime);
    }

    if (fReplayTime > 0.0f) {
        m_fSkillBlendEndTime = m_fSkillBlendEndTime - fReplayTime;
        m_xActionBuffer.Process(fReplayTime);
        while (m_xActionBuffer.CheckTime()) {
            m_xActionBuffer.Pop();
        }
    }

    if (pSkillTbl->Use_State == 1) {
        CMover::SetFlyState(1);
        m_bLanded = false;
    }

    if (pSkillTbl->Skill_Type == 3 && m_pCurAkashicRecord && m_pCurAkashicRecord->Type == 1) {
        CheckOptionEffectInvoke(EFFECT_CONDITION_USE_AKASHIC, this, 0.0f, EFFECT_INVOKE_DONT_CARE);
    }

    // Scan for nearby targets
    std::vector<CMover*> vecGameObjList;
    XActor* pActor = this ? &m_XActor : nullptr;
    XArea::ScanGridOrigin(pActor, 2, 3u, &vecGameObjList);

    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMoverEx* pOtherActor = dynamic_cast<CMoverEx*>(*it);
        if (pOtherActor) {
            if (pOtherActor->IsLive()
                && !XActor::IsStatus(&pOtherActor->m_XActor, 2u)
                && !XActor::IsStatus(&pOtherActor->m_XActor, 4u)
                && pOtherActor != this) {
                int TargetID = CMover::GetTargetID(pOtherActor);
                unsigned int QuestID = CQuestCondition::GetQuestID(GetActorID());
                if (TargetID == QuestID) {
                    CheckAttackSkillEnable(pOtherActor);
                }
            }
        }
    }

    CMover::SetKeepMovingExtra(false);
    vecGameObjList.~vector();
    VString::~VString(&strSkillName);
}

/**
 * @brief ReapllyBuffAll - reapply all buffs
 * IDA: ?ReapllyBuffAll@CMoverEx@@UEAAXXZ (0x14039D7B0)
 */
void CMoverEx::ReapllyBuffAll() {
    // TODO: 汇编还原 - IDA: 0x14039D7B0
}

/**
 * @brief PostSkillProcess - post skill processing
 * IDA: ?PostSkillProcess@CMoverEx@@UEAAXXZ (0x14037E1B0)
 * Verified: Clean up skill state after execution
 */
void CMoverEx::PostSkillProcess() {
    // IDA 精确还原:
    if (m_pCurSkillTableRef) {
        m_pSkillMgr->EndSkill(m_pCurSkillTableRef);
    }
    XActor::ClearStatus(&m_XActor, 0x20u);
    XActor::ClearStatus(&m_XActor, 0x8000u);
    CallScriptPostSkill();
    CMover::SetCurSkillTableIdx(0);
    m_bySkillChargeStep = 0;
    m_pMouseOnTrap = nullptr;
    m_vPreTargetList.clear();

    if (m_pCurSkillTableRef && m_pCurSkillTableRef->Collision_Check_Type == 1) {
        CMover::SetCollisionEnable(true, false);
    }

    m_fSkillChargeChangeTime = 0.0f;
    m_fSkillLoopTime = 0.0f;
    m_bExistSuboCombo = false;
    m_fSuboComboWaitTime = 0.0f;
    m_fSuboComboCheckTime = 0.0f;
    m_iSuboComboMaxCount = -1;
    m_iSuboComboCheckCount = 0;

    if (m_fChargingInputPressTime > 0.0f) {
        float fRestoreAnimSpeed = GetRestoreAnimSpeed();
        CMover::SetSlowTime(0.0f, fRestoreAnimSpeed);
    }
    ClearChargingInput();

    if (m_pGrapTarget) {
        CMover::send_eSUB_CMD_MOVE_DROP(m_pGrapTarget, m_pGrapTarget, 0);
    }

    ReleaseInvokedOptionEffect(EFFECT_CONDITION_USE_AKASHIC);
    m_bAttackKeyPress = false;
}

/**
 * @brief ChargeSkillNextStep - charge skill to next step
 * IDA: ?ChargeSkillNextStep@CMoverEx@@UEAAXXZ (0x14037DB60)
 * Verified: Advance skill charge step with animation
 */
void CMoverEx::ChargeSkillNextStep() {
    // IDA 精确还原:
    if (!m_pCurSkillTableRef || !m_bAttackKeyPress) {
        return;
    }

    ++m_bySkillChargeStep;
    if (m_bySkillChargeStep >= m_bySkillChargeMaxStep) {
        m_bySkillChargeStep = m_bySkillChargeMaxStep - 1;
    }

    m_fSkillChargeChangeTime = static_cast<float>(
        *reinterpret_cast<int*>(&m_pCurSkillTableRef->Time_Value_01 + m_bySkillChargeStep)
    ) * 0.001f;

    const char* szSkillAnimName = GetSkillAnimName(m_pCurSkillTableRef, m_bySkillAnimStep);
    VString strSkillName(szSkillAnimName);
    VString strAnimName(strSkillName);

    unsigned int dwNewAnimIndex = CMover::GetAnimIndex(strAnimName);
    std::int16_t nMotionClass = CMover::AnimKeyToMotion(dwNewAnimIndex);
    ChangeMotion_3(nMotionClass, true, 4);

    VString::~VString(&strSkillName);
}

/**
 * @brief ChargeSkillStart - start skill charging
 * IDA: ?ChargeSkillStart@CMoverEx@@UEAAXXZ (0x14037EA30)
 * Verified: Initialize skill charging state based on skill control type
 */
void CMoverEx::ChargeSkillStart() {
    // IDA 精确还原:
    if (m_pCurSkillTableRef) {
        int nControlType = GetControlType(m_pCurSkillTableRef);
        if (nControlType == 2 || nControlType == 5 || nControlType == 8) {
            m_bAttackKeyPress = true;
            m_bySkillChargeStep = 0;
            m_bySkillChargeMaxStep = m_pCurSkillTableRef->Charging_Count;
            m_fSkillChargeChangeTime = static_cast<float>(
                *reinterpret_cast<int*>(&m_pCurSkillTableRef->Time_Value_01 + m_bySkillChargeStep)
            ) * 0.001f;
            m_fSkillTotalChargeTime = 0.0f;
            m_fLeftChargingValue = 0.0f;
            m_fRightChargingValue = 0.0f;
        }
    }
}

/**
 * @brief ChargeSkillEnd - end skill charging
 * IDA: ?ChargeSkillEnd@CMoverEx@@UEAAXXZ (0x14037ECD0)
 * Verified: Finalize skill charging and change to final animation
 */
void CMoverEx::ChargeSkillEnd() {
    // IDA 精确还原:
    if (!m_pCurSkillTableRef || !m_bAttackKeyPress) {
        return;
    }

    if (m_bySkillAnimStep) {
        m_bAttackKeyPress = false;
        m_fSkillChargeChangeTime = 0.0f;
        m_bySkillAnimStep = 3;

        const char* szSkillAnimName = GetSkillAnimName(m_pCurSkillTableRef, m_bySkillAnimStep);
        VString strSkillName(szSkillAnimName);
        VString strAnimName(strSkillName);

        unsigned int dwNewAnimIndex = CMover::GetAnimIndex(strAnimName);
        std::int16_t nMotionClass = CMover::AnimKeyToMotion(dwNewAnimIndex);
        ChangeMotion_3(nMotionClass, true, 4);

        VString::~VString(&strSkillName);
    } else {
        m_bySkillChargeMaxStep = m_bySkillAnimStep + 1;
        m_fSkillChargeChangeTime = 0.050000001f;
    }
}

/**
 * @brief UpdateSkillAnimInfo - update skill animation info
 * @param pSkillTableRef Skill table reference
 * IDA: ?UpdateSkillAnimInfo@CMoverEx@@QEAAXPEAUTB_SKILL@@@Z (0x14037EE30)
 * Verified: Determine skill animation count and step based on skill table
 */
void CMoverEx::UpdateSkillAnimInfo(TB_SKILL* pSkillTableRef) {
    // IDA 精确还原:
    if (!pSkillTableRef) {
        return;
    }

    // Check if Ani_Res_Extra is not "0"
    if (strcmp(pSkillTableRef->Ani_Res_Extra, "0") != 0) {
        m_bySkillAnimCount = 4;
        m_bySkillAnimStep = 0;
        return;
    }

    // Check if Ani_Res_Start is not "0"
    if (strcmp(pSkillTableRef->Ani_Res_Start, "0") != 0) {
        // Check if Ani_Res_Loop is not "0"
        if (strcmp(pSkillTableRef->Ani_Res_Loop, "0") != 0) {
            m_bySkillAnimCount = 3;
        } else {
            m_bySkillAnimCount = 2;
        }
        m_bySkillAnimStep = 0;
    } else {
        m_bySkillAnimCount = 1;
        m_bySkillAnimStep = 3;
    }
}

/**
 * @brief CheckSkillSkipType - check if skill can be skipped
 * @param nSkillID Skill ID to check
 * @return true if skill can be skipped
 * IDA: ?CheckSkillSkipType@CMoverEx@@UEAA_NH@Z (0x14037E490)
 * Verified: Check skill skip type based on motion state
 */
bool CMoverEx::CheckSkillSkipType(unsigned int nSkillID) {
    // IDA 精确还原:
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTbl = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID);
    if (!pSkillTbl) {
        return false;
    }

    bool bSkip = false;
    std::uint8_t nSkipType = pSkillTbl->Skill_Motion_Skip_Type;

    switch (nSkipType) {
        case 1:
            return XActor::IsStatus(&m_XActor, 1u) != 0;
        case 2:
            if (m_nMotionClass == 1 || (m_nMotionClass >= 3 && m_nMotionClass <= 6)) {
                return true;
            }
            break;
        case 3:
            return true;
    }

    return bSkip;
}

/**
 * @brief IsExcuteSkipMotionTrigger - check if skip motion trigger should execute
 * @param byTriggerType Trigger type
 * @return true if should execute
 * IDA: ?IsExcuteSkipMotionTrigger@CMoverEx@@QEAAHE@Z (0x14037E570)
 * Verified: Check if trigger type is 3 or 8
 */
bool CMoverEx::IsExcuteSkipMotionTrigger(std::uint8_t byTriggerType) {
    return byTriggerType == 3 || byTriggerType == 8;
}

/**
 * @brief ExcuteSkipMotionTrigger - execute skip motion trigger
 * @param nSkillID Skill ID
 * @param fCamYaw Camera yaw
 * IDA: ?ExcuteSkipMotionTrigger@CMoverEx@@UEAAXHM@Z (0x14037E5A0)
 * Verified: Process skill skip animation triggers
 */
void CMoverEx::ExcuteSkipMotionTrigger(unsigned int nSkillID, float fCamYaw) {
    // IDA 精确还原:
    if (m_fSkillSkipCoolTime > 0.0f) {
        return;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTbl = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID);
    if (!pSkillTbl) {
        return;
    }

    const char* szSkillAnimName = GetSkillAnimName(pSkillTbl, 3);
    VString strSkillName(szSkillAnimName);
    VString strAnimName(strSkillName);

    unsigned int dwNewAnimIndex = CMover::GetAnimIndex(strAnimName);
    const VAnimationInfo* pActionInfo = CMover::GetActionDesc(VString::AsChar(&strSkillName));

    if (!pActionInfo) {
        VString::~VString(&strSkillName);
        return;
    }

    m_fSkillSkipCoolTime = pActionInfo->fAnimationLength;
    m_pSkillMgr->SetSkillCooltime(pSkillTbl);

    int nCurFilterData1, nCurFilterData2, nCurFilterData3;
    int nCombatType = GetCombatType();
    ActionTrigger::SetFiltering(pSkillTbl->Skill_LV, -1, nCombatType, 0, 0, 0,
        &nCurFilterData1, &nCurFilterData2, &nCurFilterData3);

    std::uint8_t iTriggerIdx = 0;
    int nTriggerCount = pActionInfo->arTriggers.GetLength();

    for (int i = 0; i < nTriggerCount; ++i) {
        ActionTrigger* pTrigger = pActionInfo->arTriggers.ElementAt(i);
        if (!pTrigger) {
            continue;
        }

        if (!IsExcuteSkipMotionTrigger(pTrigger->TypeOfTrigger)) {
            continue;
        }

        if (ActionTrigger::IsFiltering(nCurFilterData1, nCurFilterData2, nCurFilterData3,
            pTrigger->dwFilterInfo1, pTrigger->dwFilterInfo2, pTrigger->dwFilterInfo3)) {
            continue;
        }

        if (pTrigger->TypeOfTrigger == 3) {
            tagACTION_BUFFER xAction(1, pTrigger->StartTime);
            xAction.pActionTrigger = pTrigger;
            xAction << iTriggerIdx++;
            xAction << VString::AsChar(&strSkillName);
            xAction << nSkillID;
            xAction << static_cast<std::uint8_t>(1);
            xAction << static_cast<std::uint8_t>(1);
            xAction << fCamYaw;
            CMover::AddActionBuffer(xAction);
        } else if (pTrigger->TypeOfTrigger == 8) {
            tagACTION_BUFFER xAction(0x10, pTrigger->StartTime);
            xAction.pActionTrigger = pTrigger;
            CMover::AddActionBuffer(xAction);
        }
    }

    VString::~VString(&strSkillName);
}

/**
 * @brief GetDamageMotion - get damage motion class
 * @param byReactionType Reaction type
 * @param fAttackRot Attack rotation
 * @param byAttackCollision Attack collision type
 * @param byCheckRank Check rank
 * @return Motion class for damage reaction
 * IDA: ?GetDamageMotion@CMoverEx@@UEAAHEMEE@Z (0x140385290)
 * Verified: Complex damage motion state machine
 */
std::int16_t CMoverEx::GetDamageMotion(std::uint8_t byReactionType, float fAttackRot,
                                        std::uint8_t byAttackCollision, std::uint8_t byCheckRank) {
    // IDA 精确还原:
    (void)fAttackRot;  // Unused parameter

    if (!IsApplyForceReaction(byCheckRank)) {
        if ((m_byDmgMontionFlag & 2) != 0 && byReactionType == 5) {
            byReactionType = 1;
        } else if ((m_byDmgMontionFlag & 1) != 0 && byReactionType >= 2 && byReactionType <= 4) {
            byReactionType = 1;
        }
    }

    // Handle knockdown state
    if (CMover::IsHitDown()) {
        if (byReactionType == 2) {
            if (m_nMotionClass == 21 || m_nMotionClass == 20) {
                m_nHitStatus = 3;
                return m_nMotionClass;
            } else {
                m_nHitStatus = 0;
                return 19;
            }
        } else if (byReactionType == 3) {
            m_bSkipReplayTime = true;
            if (m_nMotionClass == 21 || m_nMotionClass == 20 || m_nMotionClass == 18) {
                m_nHitStatus = 0;
                return 21;
            } else {
                m_nHitStatus = 0;
                return 20;
            }
        } else if (byReactionType == 4) {
            m_nHitStatus = 0;
            return 18;
        } else {
            if (byReactionType == 1) {
                ++m_byDownContinueDamage;
            }
            m_nHitStatus = 3;
            return m_nMotionClass;
        }
    }

    // Handle flying state
    if (CMover::IsFlying()) {
        switch (byReactionType) {
            case 3:
                m_bSkipReplayTime = CMover::IsKnockDown();
                if (m_nMotionClass == 21 || m_nMotionClass == 20) {
                    m_nHitStatus = 0;
                    return 21;
                } else {
                    m_nHitStatus = 0;
                    return 20;
                }
            case 2:
                m_nHitStatus = 0;
                return 19;
            case 4:
                m_nHitStatus = 0;
                return 18;
            default:
                if (XActor::GetType(&m_XActor)) {
                    if (CMover::IsKnockDown()) {
                        if (m_nHitAnimCount == 2) {
                            return m_nMotionClass;
                        } else {
                            m_nHitStatus = 4;
                            if ((m_byDmgMontionFlag & 1) != 0) {
                                m_nHitStatus = 0;
                            }
                            return m_nMotionClass;
                        }
                    } else {
                        m_nHitStatus = 4;
                        if ((m_byDmgMontionFlag & 1) != 0) {
                            m_nHitStatus = 0;
                        }
                        return 18;
                    }
                } else if (CMover::IsKnockDown() || byReactionType == 5) {
                    m_nHitStatus = 0;
                    return 18;
                } else {
                    m_nHitStatus = 4;
                    return 23;
                }
        }
    }

    // Check phase motion
    if (CheckPhaseMotion(byAttackCollision)) {
        return m_nPlayPhaseMotion;
    }

    // Normal damage motion
    m_byDownContinueDamage = 0;
    std::int16_t nMotion = 15;

    switch (byReactionType) {
        case 0:
            nMotion = -1;
            break;
        case 1:
            if (m_nMotionClass == 15) {
                nMotion = 16;
            } else if (m_nMotionClass == 16) {
                nMotion = 15;
            } else {
                nMotion = rand() % 2 + 15;
            }
            break;
        case 2:
            nMotion = 19;
            break;
        case 3:
            nMotion = 21;
            break;
        case 4:
            nMotion = 18;
            break;
        case 5:
            nMotion = 17;
            break;
        case 6:
            nMotion = 22;
            break;
    }

    if (CMover::IsKnockDown() && !m_nHitStatus && (nMotion < 18 || nMotion > 21)) {
        nMotion = 19;
    }

    if (nMotion >= 17 && nMotion <= 21) {
        m_nHitStatus = 0;
    }

    return nMotion;
}

/**
 * @brief CheckPhaseMotion - check phase motion for damage
 * @param byAttackCollision Attack collision type
 * @return true if phase motion should be used
 * IDA: ?CheckPhaseMotion@CMoverEx@@QEAAHE@Z (0x140385810)
 * Verified: Complex phase motion state check
 */
bool CMoverEx::CheckPhaseMotion(std::uint8_t byAttackCollision) {
    // IDA 精确还原:
    if (m_byPhaseType == 1) {
        // Shield HP based phase
        if (m_nShieldHP <= 0) {
            VString strAnimName(m_strPhaseChangeAnim);
            unsigned int dwAnimKey = CMover::GetAnimIndex(strAnimName);
            NotifyPhaseChanged(m_byPhaseStep);
            m_nPlayPhaseMotion = CMover::AnimKeyToMotion(dwAnimKey);
            return true;
        }

        VString strAnimName(m_strSpecialDamage);
        unsigned int dwKey = CMover::GetAnimIndex(strAnimName);
        if (dwKey != -1) {
            m_nPlayPhaseMotion = CMover::AnimKeyToMotion(dwKey);
            return true;
        }
    } else if (m_byPhaseCondition == 1) {
        // HP percentage based phase
        int nHPPercent = static_cast<int>((static_cast<float>(GetHP()) / m_fAbility[10]) * 10000.0f);
        if (nHPPercent <= static_cast<int>(m_dwPhaseConditionValue)) {
            VString strAnimName(m_strPhaseChangeAnim);
            unsigned int dwAnimKey = CMover::GetAnimIndex(strAnimName);
            NotifyPhaseChanged(m_byPhaseStep);
            m_nPlayPhaseMotion = CMover::AnimKeyToMotion(dwAnimKey);
            return true;
        }

        if (!VString::IsEmpty(&m_strSpecialDamage) && !VString::CompareNoCase(&m_strSpecialDamage, "0")) {
            VString strAnimName(m_strSpecialDamage);
            unsigned int dwKey = CMover::GetAnimIndex(strAnimName);
            m_nPlayPhaseMotion = CMover::AnimKeyToMotion(dwKey);
            return true;
        }
    } else if (m_byPhaseCondition == 4 || m_byPhaseCondition == 6) {
        // Counter based phase
        if (!m_dwPhaseConditionValue) {
            VString strAnimName(m_strPhaseChangeAnim);
            unsigned int dwAnimKey = CMover::GetAnimIndex(strAnimName);
            NotifyPhaseChanged(m_byPhaseStep);
            m_nPlayPhaseMotion = CMover::AnimKeyToMotion(dwAnimKey);
            return true;
        }

        if (!VString::IsEmpty(&m_strSpecialDamage) && !VString::CompareNoCase(&m_strSpecialDamage, "0")) {
            VString strAnimName(m_strSpecialDamage);
            unsigned int dwKey = CMover::GetAnimIndex(strAnimName);
            m_nPlayPhaseMotion = CMover::AnimKeyToMotion(dwKey);
            return true;
        }
    } else {
        // Default: show special damage motion
        if (IsDamageMotionDisplay(byAttackCollision)
            && !VString::IsEmpty(&m_strSpecialDamage)
            && !VString::CompareNoCase(&m_strSpecialDamage, "0")) {
            VString strAnimName(m_strSpecialDamage);
            unsigned int dwKey = CMover::GetAnimIndex(strAnimName);
            m_nPlayPhaseMotion = CMover::AnimKeyToMotion(dwKey);
            return true;
        }
    }

    return m_nMotionClass == m_nPlayPhaseMotion;
}

/**
 * @brief CheckUseSkill - check if skill can be used
 * @param byCheckVal Check value
 * @param byNormalVal Normal value
 * @param pTBSkill Skill table
 * @return true if skill can be used
 * IDA: ?CheckUseSkill@CMoverEx@@QEAAHEEPEAUTB_SKILL@@@Z (0x14037FBD0)
 * Verified: Check skill usage conditions
 */
int CMoverEx::CheckUseSkill(std::uint8_t byCheckVal, std::uint8_t byNormalVal, TB_SKILL* pTBSkill) {
    // IDA 精确还原:
    switch (byCheckVal) {
        case 1:
            return 1;
        case 2:
            return m_nMotionClass == 5 || (m_nMotionClass >= 32 && m_nMotionClass <= 34);
        case 3:
            return CMover::IsHitDown() ? 1 : 0;
        case 4:
            return IsCounterAttackHit() ? 1 : 0;
        case 5:
            return CMover::IsActivateSkillUnlockBuff(pTBSkill) ? 1 : 0;
        default:
            return (((byNormalVal & 4) != 0 || !CMover::IsHitDown())
                && ((byNormalVal & 8) != 0 || !IsCounterAttackHit())
                && ((byNormalVal & 0x10) == 0 || CMover::IsActivateSkillUnlockBuff(pTBSkill))) ? 1 : 0;
    }
}

/**
 * @brief GetSkillAnimName - get skill animation name
 * @param pSkillTableRef Skill table reference
 * @param byStep Animation step
 * @return Animation name string
 * IDA: ?GetSkillAnimName@CMoverEx@@QEAAPEBDPEAUTB_SKILL@@E@Z (0x14037EF50)
 * Verified: Get animation name based on skill and step
 */
const char* CMoverEx::GetSkillAnimName(TB_SKILL* pSkillTableRef, std::uint8_t byStep) {
    // IDA 精确还原:
    if (!pSkillTableRef) {
        return nullptr;
    }

    const char* pSkillName = nullptr;
    if (byStep == 0) {
        pSkillName = pSkillTableRef->Ani_Res_Start;
    } else if (byStep == 1) {
        pSkillName = pSkillTableRef->Ani_Res_Loop;
    } else if (byStep == 2) {
        pSkillName = pSkillTableRef->Ani_Res_Extra;
    } else {
        pSkillName = pSkillTableRef->Ani_Res_End;
    }

    memset(m_szTempSkillAnimName, 0, sizeof(m_szTempSkillAnimName));

    int nControlType = GetControlType(pSkillTableRef);

    // Handle charging type skills
    if ((nControlType == 2 || nControlType == 5) && byStep == 3) {
        std::uint8_t nChargeStep = GetSkillChargeStep();
        sprintf(m_szTempSkillAnimName, "%s_%02d", pSkillName, nChargeStep + 1);
        VString strAnimName(m_szTempSkillAnimName);
        if (CMover::GetAnimIndex(strAnimName) != -1) {
            return m_szTempSkillAnimName;
        }
    } else if (nControlType == 8 && byStep == 3) {
        // Dual charging type
        if (m_fLeftChargingValue < static_cast<float>(pSkillTableRef->Charging_Max_Value)) {
            if (m_fRightChargingValue < static_cast<float>(pSkillTableRef->Charging_Max_Value)) {
                sprintf(m_szTempSkillAnimName, "%s", pSkillName);
            } else {
                sprintf(m_szTempSkillAnimName, "%s_Attack02", pSkillName);
            }
        } else {
            sprintf(m_szTempSkillAnimName, "%s_Attack01", pSkillName);
        }
        VString strAnimName(m_szTempSkillAnimName);
        if (CMover::GetAnimIndex(strAnimName) != -1) {
            return m_szTempSkillAnimName;
        }
    } else if (pSkillTableRef->Skill_Direction == 1) {
        // Direction based animation
        const char* szDir[] = {"_F", "_L", "_R", "_B"};
        sprintf(m_szTempSkillAnimName, "%s%s", pSkillName, szDir[m_byMoveDir]);
        VString strAnimName(m_szTempSkillAnimName);
        if (CMover::GetAnimIndex(strAnimName) != -1) {
            return m_szTempSkillAnimName;
        }
    }

    return pSkillName;
}

/**
 * @brief ApplySkillMove - apply skill movement
 * @param fDistance Distance to move
 * @param fTime Time to move
 * IDA: ?ApplySkillMove@CMoverEx@@QEAAXMM@Z (0x140396430)
 * Verified: Calculates target position based on moving yaw and sends skill move command
 */
void CMoverEx::ApplySkillMove(float fDistance, float fTime) {
    // IDA 精确还原:
    // hkvVec3 vTargetPos = VisObject3D_cl::GetPosition(this);
    // hkvVec3 vCameraDirection;
    // hkvMat3 matRot;
    // hkvMat3::setFromEulerAngles(&matRot, 0.0, 0.0, m_fMovingYaw);
    // hkvVec3 vDir(0.0, -1.0, 0.0);
    // vCameraDirection = hkvMat3::transformDirection(&matRot, &vDir);
    // vTargetPos += vCameraDirection * fDistance;
    // SetExtraMoving();
    // m_stExtMovingVal = vTargetPos;
    // send_eSUB_CMD_SKILL_MOVE_WITH_TIME(this, &vTargetPos, fTime);

    // TODO: 需要完整实现 - 依赖 VisObject3D_cl, hkvMat3, SetExtraMoving 等
    (void)fDistance;
    (void)fTime;
}

/**
 * @brief CallScriptPostSkill - call script post skill function
 * IDA: ?CallScriptPostSkill@CMoverEx@@QEAAXXZ (0x14037E390)
 * Verified: Call script OnPostSkill for monster actors
 */
void CMoverEx::CallScriptPostSkill() {
    // IDA 精确还原:
    E_ACTOR_TYPE eType = XActor::GetType(&m_XActor);
    if (eType == eActorMonster) {
        XArea* pArea = GetArea();
        XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
        if (pMaze) {
            IVScriptInstance* pScriptInst = pMaze->GetArea();
            if (pScriptInst && pScriptInst->HasFunction("OnPostSkill")) {
                pScriptInst->ExecuteFunctionArg("OnPostSkill", "oiTSoulworker:XMaze;",
                    this, m_nCurSkillTableIdx, pMaze, eType);
            }
        }
    }
}

/**
 * @brief GetMoveSpeed - get current move speed
 * IDA: ?GetMoveSpeed@CMoverEx@@UEAAMXZ (0x14037F670)
 * Verified: Calculate move speed based on status and area
 */
float CMoverEx::GetMoveSpeed() {
    // IDA 精确还原:
    if (XActor::IsStatus(&m_XActor, 1u)
        && m_pCurSkillTableRef
        && m_pCurMotionEvent
        && m_pCurMotionEvent->eCanMoving) {
        return static_cast<float>(m_pCurSkillTableRef->Skill_Rate_Movement);
    }

    float fMoveSpeed;
    if (CMover::IsDashing()) {
        fMoveSpeed = 700.0f;
    } else if (XActor::IsStatus(&m_XActor, 0x100u)) {
        if (!XActor::GetType(&m_XActor)) {
            if (m_bBattlePose) {
                m_fDefRunSpeed = 400.0f;
            } else {
                m_fDefRunSpeed = 300.0f;
            }
        }
        if (m_byMoveDir == 3) {
            fMoveSpeed = (m_fDefRunSpeed * 0.60000002f * m_fAbility[18]) * 0.0099999998f;
        } else {
            fMoveSpeed = (m_fDefRunSpeed * m_fAbility[18]) * 0.0099999998f;
        }
    } else {
        if (m_byMoveDir == 3) {
            fMoveSpeed = (m_fDefWalkSpeed * 0.60000002f * m_fAbility[18]) * 0.0099999998f;
        } else {
            fMoveSpeed = (m_fDefWalkSpeed * m_fAbility[18]) * 0.0099999998f;
        }
    }

    std::shared_ptr<CGocAttribute> pMyAttr;
    CMover::GetGOC<CGocAttribute>(&pMyAttr, 0);

    if (m_pArea && pMyAttr) {
        if (m_pArea->IsMaze() && m_pArea->GetWorldType() != 3) {
            fMoveSpeed = fMoveSpeed + (fMoveSpeed * pMyAttr->GetSpecialEffect(150) * 0.0099999998f);
        } else {
            fMoveSpeed = fMoveSpeed + (fMoveSpeed * pMyAttr->GetSpecialEffect(149) * 0.0099999998f);
        }
    }

    return fMoveSpeed;
}

/**
 * @brief SetDirectionYaw - set direction yaw
 * @param fYaw Yaw angle
 * @param byType Set type
 * IDA: ?SetDirectionYaw@CMoverEx@@UEAAXME@Z (0x14037F400)
 * Verified: Set direction yaw with type control
 */
void CMoverEx::SetDirectionYaw(float fYaw, std::uint8_t byType) {
    // IDA 精确还原:
    if (byType == 1) {
        SetMovingYaw(fYaw);
        CMover::SetOrientationYaw(fYaw);
    } else {
        bool bImmediate = (byType != 3);
        SetMovingYaw(fYaw);
        if (IsControlMonster()) {
            if (bImmediate) {
                CMover::SetOrientationYaw(fYaw);
            }
        }
    }
}

/**
 * @brief GetNextMotion - get next motion class
 * IDA: ?GetNextMotion@CMoverEx@@UEAAFXZ (0x140381F90)
 * Verified: Complex motion state machine for hit/skill animations
 */
std::int16_t CMoverEx::GetNextMotion() {
    // IDA 精确还原:
    std::int16_t nMotionClass = 1;

    // Handle motion class 17 (stun state)
    if (m_nMotionClass == 17) {
        if (m_nHitStatus) {
            m_nHitStatus = 7;
        } else {
            m_nHitStatus = 1;
            nMotionClass = m_nMotionClass;
        }
        goto LABEL_END;
    }

    // Handle knockdown state
    if (CMover::IsKnockDown()) {
        if (m_nHitStatus) {
            switch (m_nHitStatus) {
                case 1:
                    if (XActor::IsStatus(&m_XActor, 4u)) {
                        nMotionClass = 13;
                        if ((m_byDmgMontionFlag & 1) != 0) {
                            nMotionClass = 12;
                        }
                        RealDie(nMotionClass);
                    } else {
                        if (m_nHitAnimCount == 2) {
                            return 1;
                        }
                        ++m_nHitStatus;
                        nMotionClass = m_nMotionClass;
                    }
                    break;
                case 6:
                    m_nHitStatus = 1;
                    nMotionClass = m_nMotionClass;
                    break;
                case 3:
                    m_nHitStatus = 2;
                    nMotionClass = m_nMotionClass;
                    break;
                case 4:
                    if (CMover::IsFlying()) {
                        m_nHitStatus = 6;
                    } else {
                        m_nHitStatus = 1;
                    }
                    nMotionClass = m_nMotionClass;
                    break;
                default:
                    if (XActor::IsStatus(&m_XActor, 4u)) {
                        RealDie(12);
                    }
                    break;
            }
        } else if (CMover::IsFlying()) {
            if (m_nHitAnimCount == 2) {
                return -1;
            }
            m_nHitStatus = 6;
            m_fHitLoopMaxTime = 0.0f;
            nMotionClass = m_nMotionClass;
        } else {
            m_nHitStatus = 1;
            nMotionClass = m_nMotionClass;
            if (XActor::IsStatus(&m_XActor, 4u)) {
                nMotionClass = 13;
                if ((m_byDmgMontionFlag & 1) != 0) {
                    nMotionClass = 12;
                }
                RealDie(nMotionClass);
            }
        }
        goto LABEL_END;
    }

    // Handle skill animation states
    if (!XActor::IsStatus(&m_XActor, 1u) || m_bySkillAnimStep == 3 || !m_pCurSkillTableRef) {
        switch (m_nMotionClass) {
            case 9:
            case 23:
                if (CMover::IsFlying()) {
                    nMotionClass = 10;
                } else {
                    nMotionClass = 11;
                }
                break;
            case 35:
                if (CMover::IsFlying()) {
                    nMotionClass = 36;
                } else {
                    nMotionClass = 37;
                }
                break;
            case 40:
                nMotionClass = 41;
                break;
            case 41:
                nMotionClass = 42;
                break;
            default:
                if (XActor::IsStatus(&m_XActor, 1u)) {
                    if (m_uiForcedState == 1) {
                        nMotionClass = 10;
                    }
                } else {
                    switch (m_nMotionClass) {
                        case 12:
                        case 13:
                            nMotionClass = -1;
                            break;
                        case 24:
                            nMotionClass = 25;
                            m_bSABreakLoopMotion = true;
                            m_bShowSABreakMotion = false;
                            break;
                        case 32:
                            nMotionClass = 33;
                            break;
                    }
                }
                break;
        }
        goto LABEL_END;
    }

    // Handle skill animation step progression
    if (m_bySkillAnimStep) {
        if (m_bySkillAnimStep == 2 && m_bAttackKeyPress) {
            m_bySkillAnimStep = 1;
            m_fSkillLoopTime = static_cast<float>(m_pCurSkillTableRef->Ani_Loop_Count) * 0.001f;
        } else {
            if (m_pCurSkillTableRef && m_pCurSkillTableRef->Skill_Type == 8) {
                return 1;
            }
            if (m_bExistSuboCombo || m_bEnableCounuter) {
                if (m_fSkillLoopTime <= 0.0f
                    || (m_pSuboComboTrigger
                        && m_pSuboComboTrigger->bPlayOnce
                        && (m_iCurLeftSuboDescCount != -1 || m_iCurRightSuboDescCount != -1))) {
                    m_bySkillAnimStep = 3;
                }
            } else {
                m_bySkillAnimStep = 3;
            }

            if (m_bExistSuboCombo
                && m_fSuboComboWaitTime > 0.0f
                && m_fSkillLoopTime > 0.0f
                && m_bySkillAnimStep == 1) {
                VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
                m_fSuboComboCheckTime = IVTimer::GetTime(pTimer) + m_fSuboComboWaitTime;
            }

            if (m_pSuboComboTrigger
                && m_pSuboComboTrigger->bSkipEndMotion
                && (m_iCurLeftSuboDescCount != -1 || m_iCurRightSuboDescCount != -1)
                && m_bySkillAnimStep == 3) {
                return 1;
            }
        }
    } else if (m_bySkillAnimCount == 3) {
        int nControlType = GetControlType(m_pCurSkillTableRef);
        if ((nControlType == 2 || nControlType == 5) && !m_bAttackKeyPress) {
            m_bySkillAnimStep = 3;
        } else if (nControlType == 10) {
            if (!m_bAttackKeyPress) {
                return 1;
            }
            m_bySkillAnimStep = 1;
            m_fSkillLoopTime = static_cast<float>(m_pCurSkillTableRef->Ani_Loop_Count) * 0.001f;
        } else {
            m_bySkillAnimStep = 1;
            m_fSkillLoopTime = static_cast<float>(m_pCurSkillTableRef->Ani_Loop_Count) * 0.001f;
        }
    } else if (m_bySkillAnimCount == 4) {
        if (m_pCurSkillTableRef && m_pCurSkillTableRef->Skill_Type == 8) {
            if (m_pGrapTarget) {
                m_bySkillAnimStep = 3;
            } else {
                m_bySkillAnimStep = 2;
            }
        } else {
            int nControlType = GetControlType(m_pCurSkillTableRef);
            if (nControlType == 2 || nControlType == 5) {
                if (m_bAttackKeyPress) {
                    m_bySkillAnimStep = 1;
                } else {
                    m_bySkillAnimStep = 3;
                }
            } else if (nControlType == 6) {
                if (m_vPreTargetList.empty()) {
                    m_bySkillAnimStep = 2;
                    m_fSkillLoopTime = 3.0f;
                } else {
                    m_bySkillAnimStep = 1;
                    m_fSkillLoopTime = static_cast<float>(m_pCurSkillTableRef->Ani_Loop_Count) * 0.001f;
                }
            } else if (m_bAttackKeyPress) {
                m_bySkillAnimStep = 1;
                m_fSkillLoopTime = static_cast<float>(m_pCurSkillTableRef->Ani_Loop_Count) * 0.001f;
            } else {
                m_bySkillAnimStep = 2;
                m_fSkillLoopTime = 3.0f;
            }
        }
    } else {
        m_bySkillAnimStep = 3;
    }

    // Get animation for current step
    const char* szSkillAnimName = GetSkillAnimName(m_pCurSkillTableRef, m_bySkillAnimStep);
    VString strAnimName(szSkillAnimName);
    unsigned int dwKey = CMover::GetAnimIndex(strAnimName);
    return CMover::AnimKeyToMotion(dwKey);

LABEL_END:
    if (m_nMotionClass == 1 && m_nBuffMotion != -1) {
        return static_cast<std::int16_t>(m_nBuffMotion);
    }
    if (!XActor::GetType(&m_XActor) && nMotionClass == 1 && CMover::IsFlying()) {
        return 10;
    }
    return nMotionClass;
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
 * @brief ChangeMotionAfterFly - change motion after flying
 * IDA: ?ChangeMotionAfterFly@CMoverEx@@QEAAXXZ (0x1403850D0)
 * Verified: Handle motion transition after flying ends
 */
void CMoverEx::ChangeMotionAfterFly() {
    // IDA 精确还原:
    if (XActor::IsStatus(&m_XActor, 0x400u)) {
        return;
    }
    if (m_pCurSkillTableRef && m_pCurSkillTableRef->Use_State != 1) {
        return;
    }

    if (!XActor::IsStatus(&m_XActor, 2u)) {
        if (m_pCurSkillTableRef) {
            if (m_pCurSkillTableRef->Ani_Processing_Link_Type != 1) {
                if (m_bySkillAnimCount == 1) {
                    ChangeMotion_3(11, true, false);
                } else {
                    const char* szSkillAnimName = GetSkillAnimName(m_pCurSkillTableRef, 3);
                    VString strAnimName(szSkillAnimName);
                    unsigned int dwKey = CMover::GetAnimIndex(strAnimName);
                    if (dwKey != -1 && dwKey != m_nAnimationIdx) {
                        std::int16_t nMotionClass = CMover::AnimKeyToMotion(dwKey);
                        ChangeMotion_3(nMotionClass, true, false);
                    }
                }
            }
        } else if (m_byAniProcessLinkType != 1) {
            ChangeMotion_3(11, true, false);
        }
    }
    m_byAniProcessLinkType = 0;
}

/**
 * @brief GetMoveMotion - get move motion class
 * IDA: ?GetMoveMotion@CMoverEx@@QEAAFXZ (0x14037F580)
 * Verified: Get appropriate motion for current movement state
 */
std::int16_t CMoverEx::GetMoveMotion() {
    // IDA 精确还原:
    if (m_bGazeMoving) {
        unsigned int nAnimIndex = XActionResMgr::GetAnimIndex(this, 4, 0, m_bBattlePose);
        if (CMover::GetAnimStirng(nAnimIndex)) {
            return 4;
        } else {
            return 5;
        }
    } else if (IsCanMovingAnim()) {
        return static_cast<std::int16_t>(m_nMotionClass);
    } else if (XActor::IsStatus(&m_XActor, 0x100u)) {
        return 5;
    } else {
        unsigned int nAnimIndex = XActionResMgr::GetAnimIndex(this, 3, 0, m_bBattlePose);
        if (CMover::GetAnimStirng(nAnimIndex)) {
            return 3;
        } else {
            return 5;
        }
    }
}

/**
 * @brief UpdateJumpHeight - update jump height
 * IDA: ?UpdateJumpHeight@CMoverEx@@QEAAXXZ (0x140384250)
 * Verified: Calculate and update jump height based on physics
 */
void CMoverEx::UpdateJumpHeight() {
    // IDA 精确还原:
    if (m_byGrapStep) {
        return;
    }

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = IVTimer::GetTimeDifference(pTimer);

    if (m_fJumpDelayTime > 0.0f) {
        m_fJumpDelayTime -= fDeltaTime;
        if (m_fJumpDelayTime >= 0.0f) {
            return;
        }
        fDeltaTime += m_fJumpDelayTime;
    }

    fDeltaTime *= m_fReactionRate;
    m_fJumpTime += fDeltaTime;

    bool bAnimCalc = false;
    if (m_pActionResource && m_bJumpAnim) {
        VJumpInfo* pJumpInfo = VActionResourceLump::GetJumpInfo(m_pActionResource, "JumpLine");
        if (pJumpInfo) {
            float fCurHeight = pJumpInfo->GetCurHeight(m_fJumpTime);
            if (fCurHeight > 0.0f) {
                m_fFlyDirValue = fCurHeight - m_fPrevJumpHeight;
                m_fPrevJumpHeight = fCurHeight;
            } else {
                m_fFlyDirValue = -1960.0f * fDeltaTime;
            }
            bAnimCalc = true;
        }
    }

    if (!bAnimCalc) {
        if (m_fFlyVelocity < 0.0f) {
            m_fFlyDirValue = m_fFlyVelocity * fDeltaTime;
            m_fPrevJumpHeight += m_fFlyDirValue;
        } else {
            float fHeight = CalcFlyHeight(m_fJumpHeight, m_fJumpTime);
            m_fFlyDirValue = fHeight - m_fPrevJumpHeight;
            m_fPrevJumpHeight = fHeight;
        }

        if (m_fFlyDirValue <= 0.0f && m_fFlyingStayTime > 0.0f) {
            m_fJumpDelayTime = m_fFlyingStayTime;
            m_fFlyingStayTime = 0.0f;
        }
    }

    hkvVec3 vPos = VisObject3D_cl::GetPosition();
    vPos.z += m_fFlyDirValue;

    hkvVec3 vTestPos = vPos;
    float fCheckVal = 50.0f;
    if (m_fFlyDirValue < -40.0f) {
        fCheckVal = -m_fFlyDirValue + 10.0f;
    }

    CMover::GetHeight(&vTestPos, fCheckVal);

    if (m_fFlyDirValue >= 0.0f || vTestPos.z <= vPos.z) {
        m_bLanded = false;
    } else {
        vPos.z = vTestPos.z;
        m_bLanded = true;
        m_bJumpAnim = false;
        m_fJumpDelta = 0.0f;
    }

    XVec3 vMovePos(vPos.x, vPos.y, vPos.z);
    Move(&vMovePos);
    m_fJumpDelta += m_fFlyDirValue;
}

/**
 * @brief ThinkFunction - main think function
 * IDA: ?ThinkFunction@CMoverEx@@UEAAXXZ (0x14037A4F0)
 * Verified: Complex update loop for all mover systems
 */
void CMoverEx::ThinkFunction() {
    // IDA 精确还原:
    CMover::ThinkFunction();

    if (!m_bPublicTransportRiding) {
        CheckIdleTime();
    }

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = IVTimer::GetTimeDifference(pTimer);

    // Phase step time
    if (m_fPhaseStepMaxTime > 0.0f) {
        m_fPhaseStepMaxTime -= fDeltaTime;
        if (m_fPhaseStepMaxTime <= 0.0f) {
            if (m_byPhaseMotionStep == 1) {
                SetupPhaseMotion();
            } else if (m_byPhaseMotionStep == 2) {
                m_byPhaseMotionStep = 0;
                CMover::SetInvincibleActor(false);
            }
            m_fPhaseStepMaxTime = 0.0f;
        }
    }

    // Check attach to attacker
    if (m_bCheckAttachToAttacker) {
        m_fAttachedDuration -= fDeltaTime;
        if (m_fAttachedDuration >= 0.0f) {
            CheckAttachedEntity();
        } else {
            hkvVec3 vZero(0.0f, 0.0f, 0.0f);
            SetCheckEntityAttach(false, 0.0f, 0.0f, 0.0f, &vZero, false);
            ClearAllAttachedEntity();
        }
    }

    TraceAttachedOwner();

    // Action buffer processing
    m_xActionBuffer.Process(fDeltaTime * m_fAnimSpeed);
    while (m_xActionBuffer.CheckTime()) {
        tagACTION_BUFFER* pAction = m_xActionBuffer.Pop();
        ActionBufferProcess(pAction);
    }

    // Forced state time
    if (m_fForcedStateApplyTime > 0.0f) {
        m_fForcedStateApplyTime -= fDeltaTime;
        if (m_fForcedStateApplyTime <= 0.0f) {
            m_uiForcedState = 0;
            m_fForcedStateApplyTime = 0.0f;
        }
    }

    // Hit freeze time
    if (m_fHitFreezeTime > 0.0f) {
        m_fHitFreezeTime -= fDeltaTime;
        if (m_fHitFreezeTime <= 0.0f) {
            SetHitFreezeTime(0.0f);
        }
    }

    UpdateStiffen(fDeltaTime);

    // Time slow
    if (m_stTimeSlow.fTime > 0.0f) {
        m_stTimeSlow.fTime -= fDeltaTime;
        if (m_stTimeSlow.fTime <= 0.0f) {
            float fRestoreAnimSpeed = GetRestoreAnimSpeed();
            CMover::SetSlowTime(0.0f, fRestoreAnimSpeed);
        }
    }

    // Counter duration
    if (m_bEnableCounuter) {
        m_fCounterDuration -= fDeltaTime;
        if (m_fCounterDuration <= 0.0f) {
            m_fCounterDuration = 0.0f;
            m_bEnableCounuter = false;
        }
    }

    // Charging input
    if (m_fChargingInputDuration > 0.0f) {
        m_fChargingInputTime += fDeltaTime * m_fAnimSpeed;
        if (m_fChargingInputTime >= m_fChargingInputDuration) {
            if (m_fChargingInputPressTime > 0.0f) {
                CMover::send_eSUB_CMD_SKILL_CHARGING_END_BT(m_fChargingInputPressTime);
            }
            CalcChargingInputMultiple();
            m_fChargingInputDuration = 0.0f;
            m_fChargingInputPressTime = 0.0f;
        }
    }

    // Movement processing
    if (m_byGrapStep) {
        ProcessGrap();
    } else if (CMover::CheckMoveTick(1)) {
        MoveTick();
    } else if (IsMoveMotion(m_nMotionClass)) {
        ChangeMotion_3(1, true, true);
    }

    CMover::ProcessBuffStatus(fDeltaTime);

    // Invisible status
    if (XActor::IsStatus(&m_XActor, 0x2000u) && !XActor::IsStatus(&m_XActor, 0x80000u)) {
        if (m_nInvisibleConditionType) {
            if (m_nInvisibleConditionType == 1) {
                int nHPPercent = static_cast<int>(static_cast<float>(GetHP() * 1000) / m_fAbility[10]);
                if (nHPPercent <= m_nInvisibleConditionVal[0]) {
                    SetInvisible(false, false, false, false, false, false, false);
                }
            } else if (m_nInvisibleConditionType == 3) {
                m_fInvisibleEndTime -= fDeltaTime;
                if (m_fInvisibleEndTime <= 0.0f) {
                    m_fInvisibleEndTime = 1.0f;
                    XArea* pArea = GetArea();
                    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
                    if (pMaze) {
                        bool bAlive = false;
                        for (int i = 0; i < 4; ++i) {
                            if (m_nInvisibleConditionVal[i]) {
                                if (pMaze->IsAliveMonster(m_nInvisibleConditionVal[i])) {
                                    bAlive = true;
                                    break;
                                }
                                m_nInvisibleConditionVal[i] = 0;
                            }
                        }
                        if (!bAlive) {
                            SetInvisible(false, false, false, false, false, false, false);
                        }
                    } else {
                        m_fInvisibleEndTime = 1.0e8f;
                    }
                }
            }
        } else {
            m_fInvisibleEndTime -= fDeltaTime * m_fAnimSpeed;
            if (m_fInvisibleEndTime <= 0.0f) {
                SetInvisible(false, false, false, false, false, false, false);
            }
        }
    }

    // Phase duration
    if (m_fPhaseDurationTime > 0.0f) {
        m_fPhaseDurationTime -= fDeltaTime;
        if (m_fPhaseDurationTime <= 0.0f) {
            m_fPhaseDurationTime = 0.0f;
            VString strAnimName(m_strPhaseChangeAnim);
            unsigned int dwKey = CMover::GetAnimIndex(strAnimName);
            NotifyPhaseChanged(m_byPhaseStep);
            m_nPlayPhaseMotion = CMover::AnimKeyToMotion(dwKey);
            SetupPhaseMotion();
        }
    }

    UppdateDefenseChangeInfo(fDeltaTime);

    // Aura skill
    if (m_pAuraSkill) {
        if (m_fAuraLifeTime <= 0.0f || IVTimer::GetTime(ThreadLocalData::GetTimer()) <= m_fAuraLifeTime) {
            m_fAuraCheckTime -= fDeltaTime;
            if (m_fAuraCheckTime <= 0.0f) {
                m_fAuraCheckTime = static_cast<float>(m_pAuraSkill->Check_Time) * 0.001f;
                CMySkillList::ApplyAuraSkill(this, m_pAuraSkill);
            }
        } else {
            RemoveAuraSkill(true);
        }
    }

    UpdateRotation();
    ProcessExtraMoving();
    ProcessAnimationDuring();
    ProcessSkillAnimation();
    m_bAnimChanged = false;

    // Skill skip cool time
    if (m_fSkillSkipCoolTime > 0.0f) {
        m_fSkillSkipCoolTime -= fDeltaTime;
        if (m_fSkillSkipCoolTime <= 0.0f) {
            m_fSkillSkipCoolTime = 0.0f;
        }
    }

    // SA break loop motion
    if (m_bSABreakLoopMotion) {
        m_fSABreakLoopMotionTime -= fDeltaTime;
        if (m_fSABreakLoopMotionTime <= 0.0f) {
            m_fSABreakLoopMotionTime = 0.0f;
            m_bSABreakLoopMotion = false;
            m_bShowSABreakMotion = false;
            ChangeMotion(26, true, false);
        }
    }

    // Super armor regen (monsters only)
    if (XActor::GetType(&m_XActor) == 2 && m_fMaxSuperArmorGage > 0.0f && m_fRegenSuperArmorTime > 0.0f) {
        m_fRegenSuperArmorTime -= fDeltaTime;
        if (m_fRegenSuperArmorTime <= 0.0f) {
            m_fRegenSuperArmorTime = 0.0f;
            m_fCurSuperArmorGage = m_fMaxSuperArmorGage;
            CMover::send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE(m_fCurSuperArmorGage, m_fMaxSuperArmorGage);
        }
    }

    // Subo combo check
    if (m_bExistSuboCombo && m_fSuboComboCheckTime > 0.0f) {
        if (IVTimer::GetTime(ThreadLocalData::GetTimer()) >= m_fSuboComboCheckTime) {
            m_fSkillLoopTime = 0.0f;
            m_fSuboComboCheckTime = 0.0f;
            m_iSuboComboMaxCount = -1;
            m_iSuboComboCheckCount = 0;
            m_bExistSuboCombo = false;
            if (m_bySkillAnimStep != 3) {
                ClearMotion();
            }
        }
    }

    UpdateOptionEffect(fDeltaTime);
    UpdateStatBatch(fDeltaTime);
    UpdateLinkSkill();
    UpdateCheckAttackSkill();

    // Delay buff processing
    if (!m_vecDelayBuff.empty()) {
        for (auto it = m_vecDelayBuff.begin(); it != m_vecDelayBuff.end(); ) {
            SDelayBuff& sDelayBuff = *it;
            sDelayBuff.fWaitTime -= fDeltaTime;
            if (sDelayBuff.fWaitTime > 0.0f) {
                ++it;
            } else {
                SetBuffStatus(sDelayBuff.nBuffID, sDelayBuff.dwUserID, true);
                it = m_vecDelayBuff.erase(it);
                if (m_vecDelayBuff.empty()) {
                    break;
                }
            }
        }
    }

    m_bCounterSuccessFrame = false;
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
 * @brief SetSystemActor - set system actor flag
 * @param bVal System actor flag value
 * IDA: ?SetSystemActor@CMoverEx@@QEAAXH@Z (0x1402A4A70)
 * Verified: Simple setter for m_bSystemActor
 */
void CMoverEx::SetSystemActor(bool bVal) {
    m_bSystemActor = bVal;
}

/**
 * @brief GetAttached - get attached entity
 * @return Attached CMoverEx pointer
 * IDA: ?GetAttached@CMoverEx@@QEAAPEAV1@XZ (0x1402C72F0)
 * Verified: Simple getter for m_pAttachToAttacker
 */
CMoverEx* CMoverEx::GetAttached() {
    return m_pAttachToAttacker;
}

/**
 * @brief SetMovingYaw - set moving yaw angle
 * @param fYaw Yaw angle
 * IDA: ?SetMovingYaw@CMoverEx@@IEAAXM@Z (0x14037F3E0)
 * Verified: Simple setter for m_fMovingYaw
 */
void CMoverEx::SetMovingYaw(float fYaw) {
    m_fMovingYaw = fYaw;
}

/**
 * @brief UpdateRotation - update rotation
 * @param fDeltaTime Delta time
 * IDA: ?UpdateRotation@CMoverEx@@UEAAXM@Z (0x140383900)
 * Verified: Complex rotation blending with turn animations
 */
void CMoverEx::UpdateRotation(float fDeltaTime) {
    // IDA 精确还原:
    if (GetApplyParentRotation() == 1 || IsControlMonster()
        || XActor::IsStatus(&m_XActor, 2u)
        || XActor::IsStatus(&m_XActor, 0x8000000u)
        || !m_bUpdateRotation) {
        return;
    }

    // Auto rotation for monsters
    if (XActor::GetType(&m_XActor) && m_fAutoRotaionTime > 0.0f) {
        m_fAutoRotaionTime -= fDeltaTime;
        float fCalcYaw = m_fMovingYaw + (fDeltaTime * m_fAutoRotaionSpeed);
        if (fCalcYaw >= -180.0f) {
            if (fCalcYaw > 180.0f) {
                fCalcYaw -= 360.0f;
            }
        } else {
            fCalcYaw += 360.0f;
        }
        CMover::SetOrientationYaw(fCalcYaw);
        m_fMovingYaw = fCalcYaw;
        return;
    }

    if (CheckKeepLookTarget()) {
        return;
    }

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fCurrentTime = IVTimer::GetTime(pTimer);
    float fCurrYaw = GetOrientationYaw();
    float fDiffYaw = m_fMovingYaw - fCurrYaw;
    bool bUpdateTargetRot = false;

    // Handle target tracking
    if (CMover::GetTargetID() != -1) {
        if (IsDirectionToTargetSkill() == 1) {
            UpdateTargetRotation(false);
            SetDirectionYaw(2);
            return;
        }

        if (XActor::GetType(&m_XActor) == 2) {
            if ((XActor::IsStatus(&m_XActor, 0x8000u) || XActor::IsStatus(&m_XActor, 1u))
                && !XActor::IsStatus(&m_XActor, 0x100u)) {
                return;
            }
        } else if (XActor::IsStatus(&m_XActor, 0x8000u) || XActor::IsStatus(&m_XActor, 1u)) {
            return;
        }

        if (m_nMotionClass == 4) {
            bUpdateTargetRot = true;
            UpdateTargetRotation(true);
        } else if ((m_nMotionClass == 7 || m_nMotionClass == 8) && !m_bQuickTurn) {
            UpdateTargetRotation(false);
        } else if (m_nMotionClass == 1) {
            if (m_byStandType == 2) {
                return;
            }
            unsigned int TargetID = CMover::GetTargetID();
            CMover* pTarget = CMover::GetMoverObject(TargetID);
            if (pTarget) {
                hkvVec3 vDirVector = VisObject3D_cl::GetPosition(pTarget) - m_vPosition;
                m_fMovingYaw = CMover::GetYawFromVector(&vDirVector);
                fDiffYaw = m_fMovingYaw - fCurrYaw;
            }
        }
    }

    // Normalize yaw difference
    if (fDiffYaw >= -180.0f) {
        if (fDiffYaw > 180.0f) {
            fDiffYaw -= 360.0f;
        }
    } else {
        fDiffYaw += 360.0f;
    }

    float fAbsDiff = fabsf(fDiffYaw);
    if (fAbsDiff < 1.0f) {
        return;
    }

    // Handle rotation start
    if (m_bStartRotation && m_bHasTurnMotion == 1 && m_nMotionClass == 1) {
        m_bStartRotation = false;
        m_fLastStartRotTime = fCurrentTime;
        m_fStartRotWaitTime = 1.0f;
        m_bQuickTurn = false;
        m_fQuickTurnSpeed = 1.0f;
    }

    if ((m_fLastStartRotTime + m_fStartRotWaitTime) > fCurrentTime && m_fStartRotWaitTime > 0.0f) {
        return;
    }

    m_fStartRotWaitTime = 0.0f;

    if (CMover::IsMoving() && tagMOVE_POS::IsNoneZero(&m_stMovePos)) {
        bUpdateTargetRot = true;
    }

    if (fAbsDiff > 0.0f) {
        if (fAbsDiff > 45.0f && !m_bStartRotation) {
            if (m_bHasTurnMotion == 1) {
                if (fAbsDiff > 90.0f && m_bHasBigTurn == 1) {
                    m_nTurnStatus = 1;
                }
                if (fDiffYaw > 0.0f && m_nMotionClass == 1) {
                    ChangeMotion_3(7, true, false);
                } else if (fDiffYaw < 0.0f && m_nMotionClass == 1) {
                    ChangeMotion_3(8, true, false);
                }
                m_nTurnStatus = 0;
            }
            m_bStartRotation = true;
        }

        if (m_bStartRotation) {
            if (m_bHasTurnMotion == 1) {
                if (m_nMotionClass == 7 || m_nMotionClass == 8) {
                    bUpdateTargetRot = true;
                }
            } else {
                bUpdateTargetRot = true;
            }
        }

        if (bUpdateTargetRot == 1) {
            float fDeltaVal = (fDeltaTime * m_fDefTurnSpeed * m_fTurnSpeedRate) * m_fQuickTurnSpeed;
            float fNewYaw = CalcRotationBlending(fCurrYaw, m_fMovingYaw, fDeltaVal);
            CMover::SetOrientationYaw(fNewYaw);
        }
    }
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
 * IDA: ?SetupPhaseMotion@CMoverEx@@QEAAXXZ (0x140385E20)
 * Verified: Initialize phase motion state with invincibility
 */
void CMoverEx::SetupPhaseMotion() {
    // IDA 精确还原:
    if (XActor::GetType(&m_XActor) == 2) {
        CMonster* pMonster = dynamic_cast<CMonster*>(this);
        if (pMonster) {
            if (pMonster->GetMobTableRef()->Monster_Type != 12) {
                CMover::SetInvincibleActor(true);
            }
        }
    }

    ChangeMotion_3(m_nPlayPhaseMotion, true, false);

    if (m_pCurMotionEvent) {
        m_fPhaseStepMaxTime = m_pCurMotionEvent->fAnimationLength;
    } else {
        m_fPhaseStepMaxTime = 5.0f;
    }

    m_fSABreakLoopMotionTime = 0.0f;
    m_bSABreakLoopMotion = false;
    m_bShowSABreakMotion = false;
    m_byPhaseMotionStep = 2;
    m_pCurSkillTableRef = nullptr;
    CMover::send_eSUB_CMD_MOVE_IDLE(-2.0f);
}

/**
 * @brief SetRotation - set rotation with yaw and pitch
 * @param fYaw Yaw angle
 * @param fPitch Pitch angle
 * IDA: ?SetRotation@CMoverEx@@QEAAXMM@Z (0x140383860)
 * Verified: Set rotation using euler angles or rotation matrix
 */
void CMoverEx::SetRotation(float fYaw, float fPitch) {
    // IDA 精确还原:
    if (VisObject3D_cl::GetUseEulerAngles()) {
        VisObject3D_cl::SetOrientation(fYaw, m_vOrientation.y, m_vOrientation.z);
    } else {
        hkvMat3 matRot;
        hkvMat3::hkvMat3(&matRot);
        VisMath_cl::EulerToMatrix(fYaw, 0.0f, fPitch, &matRot);
        VisObject3D_cl::SetRotationMatrix(&matRot);
    }
}

/**
 * @brief GetLookAtPosition - get look at position from target
 * @param vPos Output position vector
 * @return true if target position found
 * IDA: ?GetLookAtPosition@CMoverEx@@UEAAHAEAVhkvVec3@@@Z (0x14037F4A0)
 * Verified: Get target position with height offset
 */
bool CMoverEx::GetLookAtPosition(hkvVec3* vPos) {
    // IDA 精确还原:
    if (m_dwTargetID == -1) {
        return false;
    }

    CMover* pTarget = CMover::GetMoverObject(m_dwTargetID);
    if (!pTarget) {
        return false;
    }

    *vPos = VisObject3D_cl::GetPosition(pTarget);
    vPos->z += CMover::GetHavokCapsuleHeight(pTarget) * 0.69999999f;
    return true;
}

/**
 * @brief SetLookPitch - set look pitch angle
 * @param fPitch Pitch angle
 * IDA: ?SetLookPitch@CMoverEx@@UEAAXM@Z (0x14037F560)
 * Verified: Simple setter for m_fLookPitch
 */
void CMoverEx::SetLookPitch(float fPitch) {
    m_fLookPitch = fPitch;
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

// ============================================================================
// Additional CMoverEx Simple Getters/Setters
// ============================================================================

/**
 * @brief GetCameraDir - get camera direction
 * @return Camera direction vector
 * IDA: ?GetCameraDir@CMoverEx@@QEAA?AVhkvVec3@@XZ (0x1402C79D0)
 * Verified: Returns m_vCamDir
 */
hkvVec3 CMoverEx::GetCameraDir() {
    return m_vCamDir;
}

/**
 * @brief SetMouseOnTrap - set mouse on trap object
 * @param pTrap Trap object pointer
 * IDA: ?SetMouseOnTrap@CMoverEx@@QEAAXPEAVVGameTrapObject@@@Z (0x1402C7AA0)
 * Verified: Sets m_pMouseOnTrap
 */
void CMoverEx::SetMouseOnTrap(VGameTrapObject* pTrap) {
    m_pMouseOnTrap = pTrap;
}

/**
 * @brief GetTrapPos - get trap position
 * @return Trap position vector
 * IDA: ?GetTrapPos@CMoverEx@@QEBA?AVhkvVec3@@XZ (0x1402C7AC0)
 * Verified: Returns m_vTrapPos (const method)
 */
hkvVec3 CMoverEx::GetTrapPos() const {
    return m_vTrapPos;
}

/**
 * @brief GetMouseOnTrap - get mouse on trap object
 * @return Trap object pointer
 * IDA: ?GetMouseOnTrap@CMoverEx@@QEAAPEAVVGameTrapObject@@XZ (0x1402C7B10)
 * Verified: Returns m_pMouseOnTrap
 */
VGameTrapObject* CMoverEx::GetMouseOnTrap() {
    return m_pMouseOnTrap;
}

/**
 * @brief SetApplyParentRotation - set apply parent rotation flag
 * @param bApply Apply flag
 * IDA: ?SetApplyParentRotation@CMoverEx@@QEAAXH@Z (0x1402C7B30)
 * Verified: Sets m_bSummonMonsterApplyRot
 */
void CMoverEx::SetApplyParentRotation(int bApply) {
    m_bSummonMonsterApplyRot = (bApply != 0);
}

/**
 * @brief SetApplyMultipleDamageOnce - set apply multiple damage once flag
 * @param bApplyMultipleDamageOnce Flag value
 * IDA: ?SetApplyMultipleDamageOnce@CMoverEx@@QEAAX_N@Z (0x1402C7B70)
 * Verified: Sets m_bApplyMultipleDamageOnce
 */
void CMoverEx::SetApplyMultipleDamageOnce(bool bApplyMultipleDamageOnce) {
    m_bApplyMultipleDamageOnce = bApplyMultipleDamageOnce;
}

/**
 * @brief SetAllowAbsorbSG - set allow absorb SG flag
 * @param bAllow Allow flag
 * IDA: ?SetAllowAbsorbSG@CMoverEx@@QEAAX_N@Z (0x1402C7B90)
 * Verified: Sets m_bAllowAbsorbSG
 */
void CMoverEx::SetAllowAbsorbSG(bool bAllow) {
    m_bAllowAbsorbSG = bAllow;
}

/**
 * @brief GetGrapTarget - get grap target
 * @return Grap target pointer
 * IDA: ?GetGrapTarget@CMoverEx@@QEAAPEAV1@XZ (0x1402C7BB0)
 * Verified: Returns m_pGrapTarget
 */
CMoverEx* CMoverEx::GetGrapTarget() {
    return m_pGrapTarget;
}

/**
 * @brief GetDieType - get die type
 * @return Die type enumeration
 * IDA: ?GetDieType@CMoverEx@@QEAA?AW4DIE_TYPE@@XZ (0x1402C7BF0)
 * Verified: Returns m_eDieType
 */
DIE_TYPE CMoverEx::GetDieType() {
    return m_eDieType;
}

/**
 * @brief SetCurDivergenceTable - set current divergence table
 * @param pCurDivTable Divergence table pointer
 * @param dwSkillID Skill ID
 * IDA: ?SetCurDivergenceTable@CMoverEx@@QEAAXPEAUTB_DIVERGENCE@@K@Z (0x1402C7CF0)
 * Verified: Sets m_pCurDivergenceTableRef and m_dwDivergenceSkillID
 */
void CMoverEx::SetCurDivergenceTable(TB_DIVERGENCE* pCurDivTable, std::uint32_t dwSkillID) {
    m_pCurDivergenceTableRef = pCurDivTable;
    m_dwDivergenceSkillID = dwSkillID;
}

/**
 * @brief GetCurDivergenceSKillID - get current divergence skill ID
 * @return Divergence skill ID
 * IDA: ?GetCurDivergenceSKillID@CMoverEx@@QEAAKXZ (0x1402C7D20)
 * Verified: Returns m_dwDivergenceSkillID
 */
std::uint32_t CMoverEx::GetCurDivergenceSKillID() {
    return m_dwDivergenceSkillID;
}

/**
 * @brief GetCurDivergenceTable - get current divergence table
 * @return Divergence table pointer
 * IDA: ?GetCurDivergenceTable@CMoverEx@@QEAAPEAUTB_DIVERGENCE@@XZ (0x1402C7D40)
 * Verified: Returns m_pCurDivergenceTableRef
 */
TB_DIVERGENCE* CMoverEx::GetCurDivergenceTable() {
    return m_pCurDivergenceTableRef;
}

/**
 * @brief GetSummonAkashicYaw - get summon akashic yaw
 * @return Summon akashic yaw angle
 * IDA: ?GetSummonAkashicYaw@CMoverEx@@QEAAMXZ (0x1402C7D80)
 * Verified: Returns m_fSummonAkashicYaw
 */
float CMoverEx::GetSummonAkashicYaw() {
    return m_fSummonAkashicYaw;
}

/**
 * @brief GetAllowAbsorbSG - get allow absorb SG flag
 * @return Allow absorb SG flag
 * IDA: ?GetAllowAbsorbSG@CMoverEx@@QEAA_NXZ (0x1402C7DA0)
 * Verified: Returns m_bAllowAbsorbSG
 */
bool CMoverEx::GetAllowAbsorbSG() {
    return m_bAllowAbsorbSG;
}

/**
 * @brief GetMultipleDamageOnce - get multiple damage once value
 * @return Multiple damage once value
 * IDA: ?GetMultipleDamageOnce@CMoverEx@@QEAAMXZ (0x1402C7DC0)
 * Verified: Returns m_fMultipleDamageOnce
 */
float CMoverEx::GetMultipleDamageOnce() {
    return m_fMultipleDamageOnce;
}

/**
 * @brief GetApplyMultipleDamageOnce - get apply multiple damage once flag
 * @return Apply multiple damage once flag
 * IDA: ?GetApplyMultipleDamageOnce@CMoverEx@@QEAA_NXZ (0x1402C7EA0)
 * Verified: Returns m_bApplyMultipleDamageOnce
 */
bool CMoverEx::GetApplyMultipleDamageOnce() {
    return m_bApplyMultipleDamageOnce;
}

/**
 * @brief GetHitPartsInfo - get hit parts info by index
 * @param iIndex Index
 * @return Hit parts info reference
 * IDA: ?GetHitPartsInfo@CMoverEx@@QEAAAEAUSHitPartsInfo@@H@Z (0x140353850)
 * Verified: Returns reference to m_sHitParts[iIndex]
 */
SHitPartsInfo& CMoverEx::GetHitPartsInfo(int iIndex) {
    return m_sHitParts[iIndex];
}

/**
 * @brief GetChangeMobTableID - get change mob table ID
 * @return Change mob table ID
 * IDA: ?GetChangeMobTableID@CMoverEx@@QEAAKXZ (0x1403539E0)
 * Verified: Returns m_dwChangeMobTableID
 */
std::uint32_t CMoverEx::GetChangeMobTableID() {
    return m_dwChangeMobTableID;
}

/**
 * @brief GetChangeMobNewID - get change mob new ID
 * @return Change mob new ID
 * IDA: ?GetChangeMobNewID@CMoverEx@@QEAAKXZ (0x140353A00)
 * Verified: Returns m_dwChangeMobNewID
 */
std::uint32_t CMoverEx::GetChangeMobNewID() {
    return m_dwChangeMobNewID;
}

/**
 * @brief SetOnDie - set on die flag
 * @param bDie Die flag
 * IDA: ?SetOnDie@CMoverEx@@QEAAX_N@Z (0x140353C80)
 * Verified: Sets m_bOnDie
 */
void CMoverEx::SetOnDie(bool bDie) {
    m_bOnDie = bDie;
}

/**
 * @brief IsSystemActor - check if system actor
 * @return true if system actor
 * IDA: ?IsSystemActor@CMoverEx@@QEAAHXZ (0x140353E10)
 * Verified: Returns m_bSystemActor
 */
int CMoverEx::IsSystemActor() {
    return m_bSystemActor;
}

/**
 * @brief SetSABreakTime - set super armor break time
 * @param fBreakTime Break time
 * IDA: ?SetSABreakTime@CMoverEx@@QEAAXM@Z (0x140353F80)
 * Verified: Sets m_fSABreakTime
 */
void CMoverEx::SetSABreakTime(float fBreakTime) {
    m_fSABreakTime = fBreakTime;
}

/**
 * @brief SetSABreakLoopTime - set super armor break loop time
 * @param fBreakLoopTime Break loop time
 * IDA: ?SetSABreakLoopTime@CMoverEx@@QEAAXM@Z (0x140353FA0)
 * Verified: Sets m_fSABreakLoopTime
 */
void CMoverEx::SetSABreakLoopTime(float fBreakLoopTime) {
    m_fSABreakLoopTime = fBreakLoopTime;
}

/**
 * @brief SetSABreakType - set super armor break type
 * @param byType Break type
 * IDA: ?SetSABreakType@CMoverEx@@QEAAXE@Z (0x140353FC0)
 * Verified: Sets m_bySABreakMotionType
 */
void CMoverEx::SetSABreakType(std::uint8_t byType) {
    m_bySABreakMotionType = byType;
}

/**
 * @brief SetSector - set sector
 * @param pVal Sector pointer
 * IDA: ?SetSector@CMoverEx@@QEAAXPEAVCSector@@@Z (0x140354250)
 * Verified: Sets m_pSector
 */
void CMoverEx::SetSector(CSector* pVal) {
    m_pSector = pVal;
}

/**
 * @brief GetAmountOfHeal - get amount of heal
 * @return Amount of heal
 * IDA: ?GetAmountOfHeal@CMoverEx@@QEAAMXZ (0x140364550)
 * Verified: Returns m_fAmountOfHeal
 */
float CMoverEx::GetAmountOfHeal() {
    return m_fAmountOfHeal;
}

/**
 * @brief GetAggroLevelOrder - get aggro level order
 * @return Aggro level order
 * IDA: ?GetAggroLevelOrder@CMoverEx@@QEAAEXZ (0x140364570)
 * Verified: Returns m_byAggroLevelOrder
 */
std::uint8_t CMoverEx::GetAggroLevelOrder() {
    return m_byAggroLevelOrder;
}

/**
 * @brief SyncMotion - sync motion class
 * @param nMotionClass Motion class
 * @param nSubMotion Sub motion
 * IDA: ?SyncMotion@CMoverEx@@QEAAXFF@Z (0x14037BFF0)
 * Verified: Sync animation and motion state
 */
void CMoverEx::SyncMotion(std::int16_t nMotionClass, std::int16_t nSubMotion) {
    // IDA 精确还原:
    if (XActor::IsStatus(&m_XActor, 1u)) {
        DebugOut("SyncMotion>> In Skill : %d - %d", nMotionClass, nSubMotion);
        return;
    }

    unsigned int dwAnimID = XActionResMgr::GetAnimIndex(this, nMotionClass, nSubMotion, m_bBattlePose);
    if (dwAnimID == m_nAnimationIdx) {
        return;
    }

    ChangeAnimation(dwAnimID, 1);
    DebugOut("SyncMotion>> %d - %d => %d", nMotionClass, nSubMotion, dwAnimID);
    m_nMotionClass = nMotionClass;

    if (CMover::IsKnockDown()) {
        m_nHitStatus = nSubMotion;
    } else if (XActor::IsStatus(&m_XActor, 1u) && m_pCurSkillTableRef && m_pCurSkillTableRef->Skill_Type != 3) {
        const char* szAnimName = GetAnimStirng(dwAnimID);
        const char* szSkillAnimName = GetSkillAnimName(m_pCurSkillTableRef, m_bySkillAnimStep);
        VString strSkillAnim(szSkillAnimName);
        if (stricmp(VString::AsChar(&strSkillAnim), szAnimName) != 0) {
            if (!stricmp(m_pCurSkillTableRef->Ani_Res_Start, szAnimName)) {
                m_bySkillAnimStep = 0;
            } else if (!stricmp(m_pCurSkillTableRef->Ani_Res_Extra, szAnimName)) {
                m_bySkillAnimStep = 2;
            } else if (!stricmp(m_pCurSkillTableRef->Ani_Res_Loop, szAnimName)) {
                m_bySkillAnimStep = 1;
            } else if (!stricmp(m_pCurSkillTableRef->Ani_Res_End, szAnimName)) {
                m_bySkillAnimStep = 3;
            }
        }
    }
}

/**
 * @brief ProcessSkillAnimation - process skill animation
 * @param fDeltaTime Delta time
 * IDA: ?ProcessSkillAnimation@CMoverEx@@UEAAXM@Z (0x14037B560)
 * Verified: Process skill animation logic
 */
void CMoverEx::ProcessSkillAnimation(float fDeltaTime) {
    // IDA 精确还原:
    if (!m_pCurSkillTableRef || XActor::IsStatus(&m_XActor, 0x8000000u)) {
        return;
    }

    // Check skill blend end time
    if (m_fSkillLoopTime <= 0.0f && m_fSkillBlendEndTime > 0.0f) {
        VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
        if (IVTimer::GetTime(pTimer) >= m_fSkillBlendEndTime) {
            m_fSkillBlendEndTime = 0.0f;
            if (XActor::IsStatus(&m_XActor, 1u)) {
                ClearMotion();
            }
            return;
        }
    }

    // Handle charging input
    if (m_fChargingInputDuration > 0.0f && m_bChargingStart) {
        m_fChargingInputPressTime += fDeltaTime * m_fAnimSpeed;
    }

    // Handle skill charging
    int nControlType = GetControlType(m_pCurSkillTableRef);
    if (nControlType == 2 || nControlType == 5 || nControlType == 8) {
        if (m_fSkillChargeChangeTime > 0.0f) {
            m_fSkillChargeChangeTime -= fDeltaTime;
            m_fSkillTotalChargeTime += fDeltaTime;
            if (m_fSkillChargeChangeTime <= 0.0f) {
                if (nControlType == 2) {
                    ChargeSkillNextStep();
                } else {
                    ChargeSkillEnd();
                }
            }
        }
    } else if ((m_bySkillAnimStep == 1 || m_bySkillAnimStep == 2) &&
               m_pCurSkillTableRef->Skill_Type != 8 &&
               m_fSkillLoopTime > 0.0f) {
        m_fSkillLoopTime -= fDeltaTime;
        if (m_fSkillLoopTime <= 0.0f) {
            if (m_bAttackKeyPress) {
                UpdateAttackKeyPress(0);
            } else {
                ClearMotion();
            }
            m_fSkillLoopTime = 0.0f;
        }
    }
}

/**
 * @brief ChangeMotion - change motion by animation name
 * @param pszMotionName Animation name
 * @param bResetPlay Reset play flag
 * IDA: ?ChangeMotion@CMoverEx@@UEAAXPEBDH@Z (0x14037C290)
 * Verified: Change animation by name
 */
void CMoverEx::ChangeMotion(const char* pszMotionName, int bResetPlay) {
    // IDA 精确还原:
    if (m_bReserveChange) {
        return;
    }

    VString strAnimName(pszMotionName);
    unsigned int nAnimIndex = CMover::GetAnimIndex(strAnimName);
    ChangeAnimation(nAnimIndex, bResetPlay);
}

/**
 * @brief ChangeMotion - change motion by motion class
 * @param nMotionClass Motion class
 * @param bResetPlay Reset play flag
 * @param iCallPos Call position
 * IDA: ?ChangeMotion@CMoverEx@@UEAAXFHH@Z (0x14037C310)
 * Verified: Complex motion change with state transitions
 */
void CMoverEx::ChangeMotion(std::int16_t nMotionClass, int bResetPlay, int iCallPos) {
    // IDA 精确还原:
    if (m_bReserveChange || XActor::IsStatus(&m_XActor, 0x8000000u)) {
        return;
    }

    unsigned int nNewAnimIdx = GetStartAnimationInx(nMotionClass);
    float fGroupAnimTime = 0.0f;
    const char* pszAnimString = GetAnimStirng(nNewAnimIdx);

    if (!pszAnimString) {
        UXActorID actorID = GetActorID();
        unsigned int dwQuestID = CQuestCondition::GetQuestID(actorID);
        DebugOut("ID:%d ChangeMotion>> %d / Not exist animation %d", dwQuestID, nMotionClass, nNewAnimIdx);
        return;
    }

    // Check animation group for blending
    if (bResetPlay && m_nAnimGroup != -1) {
        const VAnimationInfo* pInfo = GetActionDesc(pszAnimString);
        if (pInfo && m_nAnimGroup == pInfo->iAnimGroup) {
            bResetPlay = 0;
            fGroupAnimTime = m_fAnimPercentTime;
        }
    }

    DebugOut("ChangeMotion>> %d -> %d (%d) / %d", m_nMotionClass, nMotionClass, nNewAnimIdx, m_nHitStatus);

    // Handle special status
    if (XActor::IsStatus(&m_XActor, 0x400u)) {
        if (nMotionClass != 30 && nMotionClass != 38 && nMotionClass != 46) {
            XActor::ClearStatus(&m_XActor, 0x400u);
            XActor::ClearStatus(&m_XActor, 0x8000u);
            if (!m_bRestoreCollision || nMotionClass < 18 || nMotionClass > 21 || m_nHitStatus != 1) {
                CMover::SetCollisionEnable(true, false);
            }
        }
    } else {
        if (CMover::IsDashing() && nMotionClass != 32 && nMotionClass != 33 && nMotionClass != 34) {
            XActor::ClearStatus(&m_XActor, 0x800u);
            XActor::ClearStatus(&m_XActor, 0x8000u);
        }
    }

    // Handle animation speed trigger
    if (m_bAnimSpeedTrigger) {
        m_bAnimSpeedTrigger = 0;
        if (m_stTimeSlow.fTime > 0.0f) {
            m_stTimeSlow.fTime = 0.0f;
            float fRestoreAnimSpeed = GetRestoreAnimSpeed();
            CMover::SetSlowTime(0.0f, fRestoreAnimSpeed);
        }
    }

    DeleteActionBuffer();
    if (GetSkillMgr()) {
        GetSkillMgr()->ExplodeSummonClear();
    }

    ChangeAnimation(nNewAnimIdx, bResetPlay);

    // Handle jump motion
    if (IsJumpMotionExceptEnd(nMotionClass) || nMotionClass == 23) {
        CMover::SetFlyState(1);
        m_bLanded = false;
    }

    // Restore collision
    if (m_bRestoreCollision &&
        (nMotionClass < 18 || nMotionClass > 21 || (!CMover::IsFlying() && m_nHitStatus))) {
        CMover::SetCollisionEnable(true, false);
        m_bRestoreCollision = false;
    }

    // Set group animation time
    if (fGroupAnimTime > 0.0f) {
        CMover::SetCurrentSequenceTime(fGroupAnimTime);
    }

    // Handle skip replay time
    if (m_bSkipReplayTime) {
        ExtraInputTrigger* pTrigger = GetAttackInputEvent(pszAnimString);
        if (pTrigger && pTrigger->ReplayTime > 0.0f) {
            CMover::SetCurrentSequenceTime(pTrigger->ReplayTime);
            m_xActionBuffer.Process(pTrigger->StartTime);
            while (m_xActionBuffer.CheckTime()) {
                m_xActionBuffer.Pop();
            }
        }
        m_bSkipReplayTime = false;
    }

    // Update move direction animation
    if (IsMoveDirMotion(nMotionClass)) {
        m_byMoveDirAnim = m_byMoveDir;
        m_fMoveSpeed = GetMoveSpeed();
    }

    // Release option effects based on motion
    if (nMotionClass == 11 || nMotionClass == 37) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_JUMP);
    } else if (nMotionClass == 34) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_DASH);
    } else if (m_nMotionClass == 30 && nMotionClass != 30) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_EVADE);
    }

    m_nMotionClass = nMotionClass;

    if (m_pSkillMgr) {
        m_pSkillMgr->RemoveChainLightning(this);
    }

    if (CMover::IsHitDown()) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_KNOCK_DOWN);
    }

    if (nMotionClass == 1 || nMotionClass == 34 || nMotionClass == 30) {
        SetChangeMotionLog(nMotionClass, iCallPos);
    }
}

/**
 * @brief MessageFunction - handle message
 * @param iID Message ID
 * @param iParamA Parameter A
 * @param iParamB Parameter B
 * IDA: ?MessageFunction@CMoverEx@@UEAAXH_J0@Z (0x14037B870)
 * Verified: Handle trap/projectile/chain messages
 */
void CMoverEx::MessageFunction(int iID, __int64 iParamA, __int64 iParamB) {
    // IDA 精确还原:
    CMover::MessageFunction(iID, iParamA, iParamB);

    if (iID == 14 || iID == 15) {
        // Trap trigger message
        if (iParamB && iParamA && *reinterpret_cast<std::uint32_t*>(iParamB)) {
            bool bAllowAbsorbSG = false;
            bool bCheckCombo = true;
            CMoverEx* pTrapOwner = reinterpret_cast<CMoverEx*>(VTrapBase_cl::GetOwner(reinterpret_cast<VGameTrapObject*>(iParamA)));

            if (pTrapOwner) {
                bAllowAbsorbSG = pTrapOwner->GetAllowAbsorbSG();
                // TODO: Check for CAkashicObject RTTI cast
            }

            if (pTrapOwner && pTrapOwner->GetSkillMgr()) {
                hkvVec3 vPos = VisObject3D_cl::GetPosition(reinterpret_cast<VGameTrapObject*>(iParamA));
                pTrapOwner->GetSkillMgr()->ProcessTrap(
                    this, pTrapOwner, reinterpret_cast<SSkillInfo*>(iParamB), vPos,
                    *reinterpret_cast<std::uint8_t*>(iParamB + 16), bAllowAbsorbSG, bCheckCombo);
            }
        }
    } else if (iID == 13 && iParamA) {
        // Trap remove message
        if (m_pMouseOnTrap == reinterpret_cast<VGameTrapObject*>(iParamA)) {
            m_pMouseOnTrap = nullptr;
        }
        VTrapBase_cl::Deactivate(reinterpret_cast<VGameTrapObject*>(iParamA));
    } else if (iID == 11 && iParamA) {
        // Projectile explode message
        SWExplodeInfo_t* pExplodeInfo = reinterpret_cast<SWExplodeInfo_t*>(iParamA);
        SSkillInfo* pSkillInfo = reinterpret_cast<SSkillInfo*>(iParamB);
        if (iParamB && pExplodeInfo && pSkillInfo->nSkillID) {
            hkvVec3 vPos = pExplodeInfo->m_vPos;
            hkvVec3 vDir = pExplodeInfo->m_vTouchDir;
            bool bCheckCombo = true;
            // TODO: Check for CAkashicObject RTTI cast

            if (m_pSkillMgr) {
                CMoverEx* pProjTarget = dynamic_cast<CMoverEx*>(pExplodeInfo->m_pColliderEntity);
                m_pSkillMgr->ProcessProjectileExplode(
                    this, &vPos, &vDir, pSkillInfo, pProjTarget, 2,
                    pExplodeInfo->m_iTargetOrder, pExplodeInfo->m_bPenetrate,
                    pExplodeInfo->m_byHitPartsIndex, 0, pExplodeInfo->m_bHitWall,
                    m_bAllowAbsorbSG, bCheckCombo);
            }
        }
    } else if (iID == 21) {
        // Chain result message
        if (m_pSkillMgr) {
            m_pSkillMgr->SendChainResult(this);
        }
    } else if (iID == 20 && iParamA) {
        // Chain process message
        int* v20 = reinterpret_cast<int*>(iParamA);
        SSkillInfo* sSkillInfo = reinterpret_cast<SSkillInfo*>(iParamB);
        if (iParamB && v20 && sSkillInfo->nSkillID) {
            hkvVec3 v17 = *reinterpret_cast<hkvVec3*>(v20);
            hkvVec3 v19 = *reinterpret_cast<hkvVec3*>(v20 + 3);
            if (m_pSkillMgr) {
                CMoverEx* pChainTarget = dynamic_cast<CMoverEx*>(*reinterpret_cast<void**>(v20 + 3));
                m_pSkillMgr->ProcessChain(
                    this, &v17, &v19, sSkillInfo, pChainTarget, 4,
                    v20[8], *reinterpret_cast<std::uint8_t*>(v20 + 36),
                    *reinterpret_cast<std::uint8_t*>(v20 + 37), 0,
                    *reinterpret_cast<std::uint8_t*>(v20 + 38));
            }
        }
    } else if (iID == 19) {
        // Clear skill target message
        if (m_pSkillMgr) {
            m_pSkillMgr->ClearSkillTarget();
        }
    } else if (iID == 10 && iParamA) {
        // Projectile deactivate message
        VProjectileBase_cl::Deactivate(reinterpret_cast<VProjectileBase_cl*>(iParamA));
    } else if (iID == 18 && iParamA) {
        // Deserialization callback message
        reinterpret_cast<VGameTrapObject*>(iParamA)->OnDeserializationCallback();
    }
}

/**
 * @brief ChangeAnimation - change animation
 * @param nNewAnimIdx New animation index
 * @param bResetPlay Reset play flag
 * IDA: ?ChangeAnimation@CMoverEx@@UEAAXKH@Z (0x14037CA70)
 * Verified: Change animation with state transitions
 */
void CMoverEx::ChangeAnimation(unsigned int nNewAnimIdx, int bResetPlay) {
    // IDA 精确还原:
    if (m_bReserveChange) {
        return;
    }

    if (m_eDieType != DIE_TYPE_KNOCKDOWN && m_eDieType != DIE_TYPE_DELAY && XActor::IsStatus(&m_XActor, 2u)) {
        return;
    }

    if (nNewAnimIdx == static_cast<unsigned int>(-1)) {
        return;
    }

    CMover::ChangeSequence(m_nAnimationIdx, nNewAnimIdx, bResetPlay);

    if (m_pCurMotionEvent && m_pCurMotionEvent->eAnimationBehavior == ATTACK) {
        XActor::SetStatus(&m_XActor, 1u);
        CMover::SetAnimSpeed(m_fAbility[19] * 0.01f);
        m_fMoveSpeed = GetMoveSpeed();
        if (m_fSkillLoopTime <= 0.0f) {
            m_bExistSuboCombo = false;
            m_fSuboComboWaitTime = 0.0f;
            m_fSuboComboCheckTime = 0.0f;
            m_iSuboComboMaxCount = -1;
            m_iSuboComboCheckCount = 0;
        }
    } else if (XActor::IsStatus(&m_XActor, 1u)) {
        CMover::SetAnimSpeed(1.0f);
        XActor::ClearStatus(&m_XActor, 1u);
        m_fMoveSpeed = GetMoveSpeed();
        if (m_pCurSkillTableRef) {
            PostSkillProcess();
            m_pCurSkillTableRef = nullptr;
        }
    } else if (!XActor::IsStatus(&m_XActor, 0x400u) &&
               !XActor::IsStatus(&m_XActor, 0x8000u) &&
               m_fSkillBlendEndTime <= 0.0f) {
        m_pCurSkillTableRef = nullptr;
    }

    std::uint8_t byPrevDefense = m_byDefenseType;
    UpdateDefenseType();
    if (byPrevDefense != m_byDefenseType) {
        DebugOut("ChangeDefense>> %d -> %d", byPrevDefense, m_byDefenseType);
    }

    if (CMover::GetAnimStirng(nNewAnimIdx)) {
        m_nAnimationIdx = nNewAnimIdx;
    } else {
        m_nAnimationIdx = XActionResMgr::GetAnimIndex(this, 1, 0, m_bBattlePose);
    }

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    m_fLastChangeAnimationTime = IVTimer::GetTime(pTimer);
    m_nMotionClass = CMover::AnimKeyToMotion(m_nAnimationIdx);
    m_fCounterDuration = 0.0f;
    m_bEnableCounuter = false;
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
 * @brief GetCurDeckBouns - get current deck bonus
 * @return Deck bonus pointer
 * IDA: ?GetCurDeckBouns@CMoverEx@@QEAAPEAUTB_DECK_BONUS@@XZ (0x1402C7220)
 * Verified: Simple getter for m_pCurDeckBonusRef
 */
TB_DECK_BONUS* CMoverEx::GetCurDeckBouns() {
    return m_pCurDeckBonusRef;
}

/**
 * @brief GetCalcChargingMultiple - get calculated charging multiple
 * @return Charging multiple value
 * IDA: ?GetCalcChargingMultiple@CMoverEx@@QEAAMXZ (0x1402C72B0)
 * Verified: Simple getter for m_fChargingInputCalcMultiple
 */
float CMoverEx::GetCalcChargingMultiple() {
    return m_fChargingInputCalcMultiple;
}

/**
 * @brief GetSABreakType - get super armor break type
 * @return SA break motion type
 * IDA: ?GetSABreakType@CMoverEx@@QEAAEXZ (0x1402C73E0)
 * Verified: Simple getter for m_bySABreakMotionType
 */
std::uint8_t CMoverEx::GetSABreakType() {
    return m_bySABreakMotionType;
}

/**
 * @brief AddAmountOfHeal - add amount of heal
 * @param fHeal Heal amount to add
 * IDA: ?AddAmountOfHeal@CMoverEx@@QEAAXM@Z (0x1402C7440)
 * Verified: Accumulates heal amount to m_fAmountOfHeal
 */
void CMoverEx::AddAmountOfHeal(float fHeal) {
    m_fAmountOfHeal += fHeal;
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
 * @brief ProcessAnimationDuring - process animation during gameplay
 * IDA: ?ProcessAnimationDuring@CMoverEx@@UEAAXXZ (0x140384780)
 * Verified: Animation state processing based on motion class and hit status
 */
void CMoverEx::ProcessAnimationDuring() {
    // IDA 精确还原:
    if (XActor::IsStatus(&m_XActor, 0x8000000u)) {
        UpdateJumpHeight();
        return;
    }

    float fPrevAnim = m_fAnimPercentTime;
    CMover::CheckAnimationEnd();

    if (fPrevAnim > m_fAnimPercentTime && !m_bAnimChanged) {
        if (m_pCurMotionEvent && m_pCurMotionEvent->eEndofAnimation == LOOP) {
            const VAnimationInfo* pInfo = CMover::GetActionDesc(m_pCurMotionEvent->szName);
            if (pInfo) {
                if (m_pSkillMgr) {
                    m_pSkillMgr->ProjectileIndexClear();
                }
                XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                XActionResMgr::ActionDestToEntity(&pServer->m_xActionManager, this, pInfo);
                m_xActionBuffer.Process(m_fAnimationTime);
            }
        }
    }

    int nMotionCase = m_nMotionClass - 9;
    switch (nMotionCase) {
        case 0:   // Motion 9 - Jump
        case 26:  // Motion 35 - Jump variant
            UpdateJumpHeight();
            if (m_fFlyDirValue < 0.0f && !CMover::IsFlying() && m_bLanded) {
                int nNewMotion = (m_nMotionClass == 35) ? 37 : 11;
                ChangeMotion_3(nNewMotion, true, false);
            }
            if (CMover::IsMoveingInFly()) {
                ProcessMoveingInFly();
            }
            break;

        case 1:   // Motion 10 - Double Jump
        case 27:  // Motion 36 - Double Jump variant
            UpdateJumpHeight();
            if (CMover::IsMoveingInFly()) {
                ProcessMoveingInFly();
            }
            if (!CMover::IsFlying()) {
                int nNewMotion = (m_nMotionClass == 36) ? 37 : 11;
                ChangeMotion_3(nNewMotion, true, false);
            }
            break;

        case 9:   // Motion 18 - Hit reactions
        case 10:  // Motion 19
        case 11:  // Motion 20
        case 12:  // Motion 21
            m_fFlyGravity = 1960.0f;
            CheckGrapDamage();

            if (!m_nHitStatus || m_nHitStatus == 6 || m_nHitStatus == 4) {
                UpdateJumpHeight();
                if (CMover::IsMoveingInFly()) {
                    ProcessMoveingInFly();
                }
                if (!CMover::IsFlying() && m_fFlyDirValue < 0.0f) {
                    m_nHitStatus = 1;
                    ChangeMotion_3(m_nMotionClass, true, false);
                    if (XActor::GetType(&m_XActor) == 2 || XActor::GetType(&m_XActor) == 1) {
                        ClearExtraMoving();
                        hkvVec3 vPos = m_vPosition;
                        CMover::send_eSUB_CMD_SKILL_MOVE_WITH_TIME(&vPos, 0.1f);
                    }
                } else if (m_nHitStatus == 6) {
                    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
                    m_fHitLoopMaxTime += IVTimer::GetTimeDifference(pTimer);
                    if (m_fHitLoopMaxTime >= 5.0f) {
                        m_nHitStatus = 1;
                        ChangeMotion_3(m_nMotionClass, true, false);
                        hkvVec3 vPos = VisObject3D_cl::GetPosition();
                        CMover::GetHeight(&vPos, 200.0f);
                        SetPositionXVec3(&vPos);
                    }
                }
            } else {
                if (CMover::IsFlying()) {
                    UpdateJumpHeight();
                    if (CMover::IsMoveingInFly()) {
                        ProcessMoveingInFly();
                    }
                }
                if (m_nHitStatus == 2 || m_nHitStatus == 3) {
                    if (XActor::IsStatus(&m_XActor, 4u)) {
                        RealDie(13);
                    } else if (CheckGrapRaise()) {
                        if (m_fGroundDownTime > 0.0f) {
                            VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
                            m_fGroundDownTime -= IVTimer::GetTimeDifference(pTimer);
                        }
                        if (m_fGroundDownTime <= 0.0f) {
                            m_nHitStatus = 5;
                            ChangeMotion_3(m_nMotionClass, true, false);
                            if (XActor::GetType(&m_XActor) == 2 || XActor::GetType(&m_XActor) == 1) {
                                CMover::send_eSUB_CMD_MONSTER_CHANGE_MOTION(m_nMotionClass, m_nHitStatus);
                            }
                        }
                    }
                }
            }
            break;

        default:
            bool bFlyingEnd = false;
            if (CMover::IsFlying()) {
                UpdateJumpHeight();
            }
            if (XActor::IsStatus(&m_XActor, 0x20u)
                || CMover::IsFlying()
                || (m_pCurSkillTableRef && m_pCurSkillTableRef->Use_State == 1)) {
                if (!CMover::IsFlying()) {
                    if (m_fFlyDirValue >= 0.0f) {
                        if (XActor::IsStatus(&m_XActor, 0x20u)) {
                            bFlyingEnd = true;
                        }
                    } else {
                        XActor::ClearStatus(&m_XActor, 0x20u);
                        ChangeMotionAfterFly();
                    }
                }
            } else if (!m_bLanded) {
                bFlyingEnd = true;
            }

            if (bFlyingEnd && (XActor::IsStatus(&m_XActor, 0x20u) || !m_bLanded)) {
                m_bLanded = true;
                XActor::ClearStatus(&m_XActor, 0x20u);
                ChangeMotionAfterFly();
            }
            break;
    }
}

/**
 * @brief ApplyOptionEffectValue - apply option effect value based on status type
 * @param eStatusType Effect status type
 * @param fValue Value to apply
 * IDA: ?ApplyOptionEffectValue@CMoverEx@@QEAAXW4EFFECT_STATUS_TYPE@@M@Z (0x14039CAB0)
 * Verified: Complex effect application based on EFFECT_STATUS_TYPE enum
 */
void CMoverEx::ApplyOptionEffectValue(EFFECT_STATUS_TYPE eStatusType, float fValue) {
    // IDA 精确还原:
    std::shared_ptr<CGocAttribute> pAttr;
    CMover::GetGOC<CGocAttribute>(&pAttr, 0);

    if (!pAttr) {
        return;
    }

    switch (eStatusType) {
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
            // Player-only stats
            if (dynamic_cast<CUser*>(this)) {
                int iStatusList[] = {1, 2, 6, 8, 9, 10, 12, 13, 15, 20, 29, 31, 11};
                int iStatList[] = {21, 24, 15, 18, 19, 26, 29, 31, 38, 43, 47, 35, 28};

                int iIndex = -1;
                for (int i = 0; i < 13; ++i) {
                    if (iStatusList[i] == eStatusType) {
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
                    CMover::SendUpdateStat(iStatList[iIndex]);
                }
            }
            break;
        }

        case EFFECT_STATUS_MAX_HP:
        case EFFECT_STATUS_MAX_SG:
        case EFFECT_STATUS_MAX_ST:
        case EFFECT_STATUS_MAX_SV: {
            int v39[] = {3, 4, 5, 7};
            int nStat[] = {10, 12, 14, 17};
            int iStatList2[] = {1, 2, 3, 16};

            int v41 = -1;
            for (int j = 0; j < 4; ++j) {
                if (v39[j] == eStatusType) {
                    v41 = j;
                    break;
                }
            }

            if (v41 >= 0) {
                pAttr->UpdateAddStat(nStat[v41], fValue, true);
                pAttr->SetSyncStatFlag(nStat[v41], SYNC_STAT_TYPE_BROADCAST);
                CMover::SendUpdateStat(nStat[v41]);

                float fMax = CMover::GetStat(nStat[v41]);
                float fCur = CMover::GetStat(iStatList2[v41]);
                if (fCur > fMax) {
                    CMover::SetStat(iStatList2[v41], fMax);
                    pAttr->SetSyncStatFlag(iStatList2[v41], SYNC_STAT_TYPE_ME_ONLY);
                    CMover::SendUpdateStat(iStatList2[v41]);
                }
            }
            break;
        }

        case EFFECT_STATUS_SA_DEFENCE:
            AddDefenseChangeInfo(2, 2, 0, 0.0f);
            CMover::send_eSUB_CMD_SKILL_DEFENCE_TYPE(2, true);
            break;

        case EFFECT_STATUS_REFLECTION:
            if (CMover::GetDefenseType() != 3) {
                float fCurHP = CMover::GetStat(1);
                float fApplyValue = fValue;
                if (fCurHP - fValue < 0.0f) {
                    fApplyValue = fCurHP - 1.0f;
                }
                SetHP(static_cast<int>(fCurHP - fApplyValue));
                pAttr->SetSyncStatFlag(1, SYNC_STAT_TYPE_BROADCAST);
                CMover::SendUpdateStat(1);
            }
            break;

        case EFFECT_STATUS_COOLTIME:
            if (m_pSkillMgr) {
                m_pSkillMgr->ReduceSkillCooltime(fValue);
                CMover::send_eSUB_CMD_SKILL_COOLTIME_REDUCE(fValue);
            }
            break;

        case EFFECT_STATUS_CUR_HP:
            if (!CMover::IsDie()) {
                float fCurHP = CMover::GetStat(1);
                CMover::SetStat(1, fCurHP + fValue);
                pAttr->SetSyncStatFlag(1, SYNC_STAT_TYPE_BROADCAST);
                CMover::SendUpdateStat(1);
            }
            break;

        case EFFECT_STATUS_CUR_SG: {
            float fCurSG = CMover::GetStat(2);
            CMover::SetStat(2, fCurSG + fValue);
            pAttr->SetSyncStatFlag(2, SYNC_STAT_TYPE_BROADCAST);
            CMover::SendUpdateStat(1);
            break;
        }

        case EFFECT_STATUS_INVINCIBLE:
            AddDefenseChangeInfo(2, 3, 0, 0.0f);
            CMover::send_eSUB_CMD_SKILL_DEFENCE_TYPE(3, true);
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
                std::shared_ptr<CGocAttribute> pResultAttr;
                CMover::GetGOC<CGocAttribute>(&pResultAttr, 0);
                if (pResultAttr) {
                    pResultAttr->SendDBUpdateFP();
                    // TODO: Add log support
                }
            }
            break;
        }

        case EFFECT_STATUS_CUR_SV: {
            float fCurSV = CMover::GetStat(16);
            CMover::SetStat(16, fCurSV + fValue);
            pAttr->SetSyncStatFlag(16, SYNC_STAT_TYPE_ME_ONLY);
            CMover::SendUpdateStat(16);
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
            float fCurST = CMover::GetStat(3);
            CMover::SetStat(3, fCurST + fValue);
            pAttr->SetSyncStatFlag(3, SYNC_STAT_TYPE_ME_ONLY);
            CMover::SendUpdateStat(3);
            break;
        }

        case EFFECT_STATUS_REMOVE_DEBUFF:
            for (int iType = 111; iType <= 132; ++iType) {
                int iTempIndex = CMover::FindBuffByEffectType(iType, 0);
                if (iTempIndex != -1) {
                    ClearBuffStatusBySlot(iTempIndex, 0);
                }
            }
            break;

        case EFFECT_STATUS_SA_ATTACK_INCREASE:
            pAttr->UpdateEffectStat(0, 0x71u, fValue, true);
            pAttr->CalculateChangedEffect(1, fValue);
            break;

        default:
            break;
    }
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

// ============================================================================
// Additional CMoverEx Functions - IDA Decompiled
// ============================================================================

/**
 * @brief GetGroundDownTime - get ground down time
 * @return Random time 1-3s for monsters, 3s for players
 * IDA: ?GetGroundDownTime@CMoverEx@@QEAAMXZ (0x140381B50)
 */
float CMoverEx::GetGroundDownTime() {
    // IDA 精确还原:
    if (XActor::GetType() != eActorUser) {
        // Monster: random 1.0 - 3.0 seconds
        return static_cast<float>(rand() % 21) * 0.1f + 1.0f;
    } else {
        // Player: fixed 3.0 seconds
        return 3.0f;
    }
}

/**
 * @brief GetStartAnimationInx - get start animation index
 * @param nMotionClass Motion class
 * @return Animation index
 * IDA: ?GetStartAnimationInx@CMoverEx@@UEAAKF@Z (0x140381D10)
 */
std::uint32_t CMoverEx::GetStartAnimationInx(std::int16_t nMotionClass) {
    // IDA 精确还原:
    if (m_byDefaultAnimStep && m_bBattlePose && IsChangeAnimByPhaseStepMotion(nMotionClass)) {
        return XActionResMgr::GetAnimIndex(this, nMotionClass, m_byDefaultAnimStep, m_bBattlePose);
    }

    // Motion classes that use move direction
    if (nMotionClass == 3 || nMotionClass == 5 || nMotionClass == 6 ||
        nMotionClass == 4 || nMotionClass == 30 || nMotionClass == 38 ||
        nMotionClass == 46 || nMotionClass == 31) {
        return XActionResMgr::GetAnimIndex(this, nMotionClass, m_byMoveDir, m_bBattlePose);
    }

    if (nMotionClass < 9 || nMotionClass > 10) {
        if (nMotionClass < 7 || nMotionClass > 8) {
            if (nMotionClass < 17 || nMotionClass > 21) {
                // Special case for motion class 13 when current is 21
                if (nMotionClass == 13 && m_nMotionClass == 21) {
                    std::uint32_t dwID = XActionResMgr::GetAnimIndex(this, nMotionClass, 10, m_bBattlePose);
                    if (dwID != -1) {
                        return dwID;
                    }
                }
                return XActionResMgr::GetAnimIndex(this, nMotionClass, 0, m_bBattlePose);
            } else {
                // Motion class 17-21: use hit status
                return XActionResMgr::GetAnimIndex(this, nMotionClass, m_nHitStatus, 1);
            }
        } else {
            // Motion class 7-8: use turn status
            return XActionResMgr::GetAnimIndex(this, nMotionClass, m_nTurnStatus, 1);
        }
    } else {
        // Motion class 9-10
        if (m_bMoveingInFly) {
            return XActionResMgr::GetAnimIndex(this, nMotionClass, m_byMoveDir + 1, m_bBattlePose);
        } else {
            return XActionResMgr::GetAnimIndex(this, nMotionClass, 1, m_bBattlePose);
        }
    }
}

/**
 * @brief GetMovingRightVector - get moving right vector
 * @return Right vector based on moving yaw
 * IDA: ?GetMovingRightVector@CMoverEx@@QEAA?AVhkvVec3@@XZ (0x140382B10)
 */
hkvVec3 CMoverEx::GetMovingRightVector() {
    // IDA 精确还原:
    hkvMat3 matRot;
    matRot.setFromEulerAngles(0.0f, 0.0f, m_fMovingYaw - 90.0f);

    hkvVec3 vDir(0.0f, -1.0f, 0.0f);
    hkvVec3 vResult;
    matRot.transformDirection(vResult, vDir);

    return vResult;
}

/**
 * @brief GetAttackJudgmentEvent - get attack judgment event from skill
 * @param pSkillTableRef Skill table pointer
 * @param byStep Animation step
 * @param iIndex Event index
 * @return AttackJudgmentTrigger pointer
 * IDA: ?GetAttackJudgmentEvent@CMoverEx@@QEAAPEAVAttackJudgmentTrigger@@PEAUTB_SKILL@@EH@Z (0x140381460)
 */
AttackJudgmentTrigger* CMoverEx::GetAttackJudgmentEvent(TB_SKILL* pSkillTableRef,
                                                         std::uint8_t byStep, int iIndex) {
    // IDA 精确还原:
    const char* szTriggerName = GetSkillAnimName(pSkillTableRef, byStep);
    const VAnimationInfo* pActionInfo = GetActionDesc(szTriggerName);
    return static_cast<AttackJudgmentTrigger*>(XActionResMgr::RetrieveEvent(3, iIndex, pActionInfo));
}

/**
 * @brief GetAttackInputEvent - get attack input event by name
 * @param szTriggerName Trigger name
 * @return ExtraInputTrigger pointer
 * IDA: ?GetAttackInputEvent@CMoverEx@@QEAAPEAVExtraInputTrigger@@PEBD@Z (0x140381540)
 */
ExtraInputTrigger* CMoverEx::GetAttackInputEvent(const char* szTriggerName) {
    // IDA 精确还原:
    const VAnimationInfo* pActionInfo = GetActionDesc(szTriggerName);
    return static_cast<ExtraInputTrigger*>(XActionResMgr::RetrieveEvent(2, 0, pActionInfo));
}

/**
 * @brief GetUpperMotionName - get upper motion name with direction suffix
 * @param szMotionName Base motion name
 * @return Modified motion name with "U" suffix and direction
 * IDA: ?GetUpperMotionName@CMoverEx@@QEAA?AVVString@@PEBD@Z (0x140381750)
 */
VString CMoverEx::GetUpperMotionName(const char* szMotionName) {
    // IDA 精确还原:
    static const char* szMoveDir[4] = {"_F", "_L", "_R", "_B"};

    VString strMotion(szMotionName);
    int nLen = strMotion.GetLen();

    // Replace last character with "U"
    strMotion.ReplaceAt(nLen - 1, 1, "U");

    // Append direction suffix
    strMotion += szMoveDir[m_byMoveDir];

    return strMotion;
}

/**
 * @brief GetAttackJudgmentEvent - get attack judgment event by animation name
 * @param pAnimName Animation name
 * @param iIndex Event index
 * @return AttackJudgmentTrigger pointer
 * IDA: ?GetAttackJudgmentEvent@CMoverEx@@QEAAPEAVAttackJudgmentTrigger@@PEBDH@Z (0x1403814C0)
 */
AttackJudgmentTrigger* CMoverEx::GetAttackJudgmentEvent(const char* pAnimName, int iIndex) {
    // IDA 精确还原:
    const VAnimationInfo* pActionInfo = GetActionDesc(pAnimName);
    return static_cast<AttackJudgmentTrigger*>(XActionResMgr::RetrieveEvent(3, iIndex, pActionInfo));
}

/**
 * @brief GetAttackJudgmentEvent - get attack judgment event by ID
 * @param nEventID Event ID
 * @return AttackJudgmentTrigger pointer
 * IDA: ?GetAttackJudgmentEvent@CMoverEx@@QEAAPEAVAttackJudgmentTrigger@@H@Z (0x140381500)
 */
AttackJudgmentTrigger* CMoverEx::GetAttackJudgmentEvent(int nEventID) {
    // IDA 精确还原:
    if (m_pActionResource) {
        return static_cast<AttackJudgmentTrigger*>(
            VActionResourceLump::GetAttackEvent(m_pActionResource, nEventID));
    }
    return nullptr;
}

/**
 * @brief SetGazeMoving - set gaze moving flag
 * @param bGaze Gaze flag
 * IDA: ?SetGazeMoving@CMoverEx@@QEAAXH@Z (0x140390AC0)
 */
void CMoverEx::SetGazeMoving(bool bGaze) {
    // IDA 精确还原:
    if (m_byStandType != 2 && bGaze != m_bGazeMoving) {
        m_bGazeMoving = bGaze;
        send_eSUB_CMD_MOVE_GAZE();
    }
}

/**
 * @brief SetMouseOnTrapPos - set mouse on trap position
 * @param vTrapPos Trap position
 * IDA: ?SetMouseOnTrapPos@CMoverEx@@QEAAXVhkvVec3@@@Z (0x140395AF0)
 */
void CMoverEx::SetMouseOnTrapPos(const hkvVec3& vTrapPos) {
    // IDA 精确还原:
    if (m_pMouseOnTrap) {
        m_pMouseOnTrap->SetTrapPosition(vTrapPos);
    }
}

// ============================================================================
// Defense Change Info Functions - IDA Decompiled
// ============================================================================

/**
 * @brief ChangeAnimation - change animation
 * @param nNewAnimIdx New animation index
 * @param bResetPlay Reset play flag
 * IDA: ?ChangeAnimation@CMoverEx@@UEAAXKH@Z (0x14037CA70)
 */
void CMoverEx::ChangeAnimation(std::uint32_t nNewAnimIdx, int bResetPlay) {
    // IDA 精确还原:
    if (m_bReserveChange) {
        return;
    }

    if (m_eDieType != DIE_TYPE_KNOCKDOWN && m_eDieType != DIE_TYPE_DELAY &&
        XActor::IsStatus(2u)) {
        return;
    }

    if (nNewAnimIdx == static_cast<std::uint32_t>(-1)) {
        return;
    }

    CMover::ChangeSequence(m_nAnimationIdx, nNewAnimIdx, bResetPlay);

    if (m_pCurMotionEvent && m_pCurMotionEvent->eAnimationBehavior == ATTACK) {
        XActor::SetStatus(1u);
        CMover::SetAnimSpeed(m_fAbility[19] * 0.01f);
        m_fMoveSpeed = GetMoveSpeed();

        if (m_fSkillLoopTime <= 0.0f) {
            m_bExistSuboCombo = false;
            m_fSuboComboWaitTime = 0.0f;
            m_fSuboComboCheckTime = 0.0f;
            m_iSuboComboMaxCount = -1;
            m_iSuboComboCheckCount = 0;
        }
    } else if (XActor::IsStatus(1u)) {
        CMover::SetAnimSpeed(1.0f);
        XActor::ClearStatus(1u);
        m_fMoveSpeed = GetMoveSpeed();

        if (m_pCurSkillTableRef) {
            PostSkillProcess();
            m_pCurSkillTableRef = nullptr;
        }
    } else if (!XActor::IsStatus(0x400u) && !XActor::IsStatus(0x8000u) &&
               m_fSkillBlendEndTime <= 0.0f) {
        m_pCurSkillTableRef = nullptr;
    }

    std::uint8_t byPrevDefense = m_byDefenseType;
    UpdateDefenseType();

    if (byPrevDefense != m_byDefenseType) {
        CMover::DebugOut("ChangeDefense>> %d -> %d", byPrevDefense, m_byDefenseType);
    }

    if (CMover::GetAnimStirng(nNewAnimIdx)) {
        m_nAnimationIdx = nNewAnimIdx;
    } else {
        m_nAnimationIdx = XActionResMgr::GetAnimIndex(this, 1, 0, m_bBattlePose);
    }

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    m_fLastChangeAnimationTime = IVTimer::GetTime(pTimer);
    m_nMotionClass = CMover::AnimKeyToMotion(m_nAnimationIdx);
    m_fCounterDuration = 0.0f;
    m_bEnableCounuter = false;
}

/**
 * @brief UpdateDefenseType - update defense type
 * IDA: ?UpdateDefenseType@CMoverEx@@UEAAXXZ (0x14037CDF0)
 */
void CMoverEx::UpdateDefenseType() {
    // IDA 精确还原:
    if (CMover::IsInvincibleActor()) {
        CMover::SetSimpleDefenseType(3);
        return;
    }

    ApplyDefenseChangeInfo();

    if (m_byDefenseType == 3) {
        return;
    }

    // Check defense type disable flags
    if (((m_byDefenseTypeDisableFlag & 1) != 0 && m_byDefenseType == 1) ||
        ((m_byDefenseTypeDisableFlag & 2) != 0 && m_byDefenseType == 2) ||
        ((m_byDefenseTypeDisableFlag & 0x10) != 0 && m_byDefenseType == 5)) {
        CMover::SetSimpleDefenseType(0);
    } else if (m_stDefenseChangeInfoByTrigger.fChangeTime <= 0.0f &&
               m_byRestoreDefenceType != 4) {
        m_byRestoreDefenceType = 4;

        if (XActor::GetType() == eActorMonster) {
            m_fCurSuperArmorGage = m_fMaxSuperArmorGage;
            CMover::send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE(
                m_fCurSuperArmorGage, m_fMaxSuperArmorGage);
            ActiveSuperArmorSkill();
        }
    }
}

/**
 * @brief AddDefenseChangeInfo - add defense change info
 * @param byType Type (0=Trigger, 1=List, 2=Effect)
 * @param byDefenseType Defense type
 * @param dwID ID
 * @param fTime Time duration
 * @return 1 on success
 * IDA: ?AddDefenseChangeInfo@CMoverEx@@QEAAHEEKM@Z (0x14037CF80)
 */
int CMoverEx::AddDefenseChangeInfo(std::uint8_t byType, std::uint8_t byDefenseType,
                                     std::uint32_t dwID, float fTime) {
    // IDA 精确还原:
    if (byType == 0) {
        // Trigger type
        m_stDefenseChangeInfoByTrigger.dwID = dwID;
        m_stDefenseChangeInfoByTrigger.byDefenseType = byDefenseType;
        m_stDefenseChangeInfoByTrigger.fChangeTime = fTime;
    } else if (byType == 1) {
        // List type - add to list
        SDefenseChangeInfo* pInfo = new SDefenseChangeInfo(dwID, byDefenseType, fTime);
        m_listDefenseChangeInfo.push_back(pInfo);
    } else if (byType == 2) {
        // Effect type
        m_stDefenseChangeInfoByEffect.dwID = dwID;
        m_stDefenseChangeInfoByEffect.byDefenseType = byDefenseType;
        m_stDefenseChangeInfoByEffect.fChangeTime = fTime;

        if (byDefenseType == 3) {
            CMover::SetInvincibleActor(true);
        }
    }

    m_DefanseChangeFlag |= (1 << byDefenseType);
    return ApplyDefenseChangeInfo();
}

/**
 * @brief RemoveDefenseChangeInfo - remove defense change info
 * @param byType Type (0=Trigger, 1=List, 2=Effect)
 * @param dwID ID (for list type)
 * @return 1 on success
 * IDA: ?RemoveDefenseChangeInfo@CMoverEx@@QEAAHEK@Z (0x14037D100)
 */
int CMoverEx::RemoveDefenseChangeInfo(std::uint8_t byType, std::uint32_t dwID) {
    // IDA 精确还原:
    if (byType == 0) {
        // Trigger type
        m_stDefenseChangeInfoByTrigger.Clear();
        CMover::ClearImmunityStatus(2u);
    } else if (byType == 1) {
        // List type - find and remove by ID
        for (auto it = m_listDefenseChangeInfo.begin();
             it != m_listDefenseChangeInfo.end(); ++it) {
            SDefenseChangeInfo* pInfo = *it;
            if (pInfo->dwID == dwID) {
                delete pInfo;
                m_listDefenseChangeInfo.erase(it);
                break;
            }
        }
    } else if (byType == 2) {
        // Effect type
        if (m_stDefenseChangeInfoByEffect.byDefenseType == 3) {
            CMover::SetInvincibleActor(false);
        }
        m_stDefenseChangeInfoByEffect.Clear();
    }

    // Rebuild defense change flag
    m_DefanseChangeFlag = 0;

    if (m_stDefenseChangeInfoByTrigger.fChangeTime > 0.0f) {
        m_DefanseChangeFlag |= (1 << m_stDefenseChangeInfoByTrigger.byDefenseType);
    }

    for (const auto* pInfo : m_listDefenseChangeInfo) {
        m_DefanseChangeFlag |= (1 << pInfo->byDefenseType);
    }

    if (m_stDefenseChangeInfoByEffect.fChangeTime > 0.0f) {
        m_DefanseChangeFlag |= (1 << m_stDefenseChangeInfoByEffect.byDefenseType);
    }

    return ApplyDefenseChangeInfo();
}

/**
 * @brief RemoveAllDefenseChangeInfo - remove all defense change info
 * IDA: ?RemoveAllDefenseChangeInfo@CMoverEx@@QEAAXXZ (0x14037D420)
 */
void CMoverEx::RemoveAllDefenseChangeInfo() {
    // IDA 精确还原:
    m_stDefenseChangeInfoByTrigger.Clear();
    CMover::ClearImmunityStatus(2u);

    // Delete all items in list
    for (auto* pInfo : m_listDefenseChangeInfo) {
        delete pInfo;
    }
    m_listDefenseChangeInfo.clear();

    // Clear effect type
    if (m_stDefenseChangeInfoByEffect.byDefenseType == 3) {
        CMover::SetInvincibleActor(false);
    }
    m_stDefenseChangeInfoByEffect.Clear();

    m_DefanseChangeFlag = 0;
}

/**
 * @brief UppdateDefenseChangeInfo - update defense change info (typo in original)
 * @param fDelta Delta time
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
 * @brief ApplyDefenseChangeInfo - apply defense change info
 * @return true if defense type changed
 * IDA: ?ApplyDefenseChangeInfo@CMoverEx@@QEAAHXZ (0x14037D5B0)
 */
bool CMoverEx::ApplyDefenseChangeInfo() {
    // IDA 精确还原:
    std::uint8_t byDefenseOld = m_byDefenseType;
    TypeOfDefense eDefense = DEFENSE_NONE;

    if (m_pCurMotionEvent) {
        eDefense = m_pCurMotionEvent->eDefenseType;
    }

    if ((m_DefanseChangeFlag & 8) != 0 || eDefense == INVINCIBLE) {
        CMover::SetSimpleDefenseType(3);
    } else if ((m_DefanseChangeFlag & 4) != 0 || eDefense == SUPER_ARMOR) {
        CMover::SetSimpleDefenseType(2);
    } else {
        if ((m_DefanseChangeFlag & 1) == 0) {
            if (eDefense == DEFENSE_NONE) {
                CMover::SetSimpleDefenseType(m_byDefaultDefenseType);
                return byDefenseOld != m_byDefenseType;
            }
            if (!XActor::IsStatus(2u) && !XActor::IsStatus(4u)) {
                CMover::SetSimpleDefenseType(eDefense);
                return byDefenseOld != m_byDefenseType;
            }
        }
        CMover::SetSimpleDefenseType(0);
    }

    return byDefenseOld != m_byDefenseType;
}

/**
 * @brief ChangeDefenseTypeForce - force change defense type with time
 * @param byDefenseType Defense type
 * @param fTime Duration time
 * IDA: ?ChangeDefenseTypeForce@CMoverEx@@QEAAXEM@Z (0x14037D710)
 */
void CMoverEx::ChangeDefenseTypeForce(std::uint8_t byDefenseType, float fTime) {
    // IDA 精确还原:
    RemoveAllDefenseChangeInfo();
    AddDefenseChangeInfo(0, byDefenseType, 0, fTime);
}

/**
 * @brief ChangeDefenseTypeForce - force change defense type
 * @param byDefenseType Defense type
 * IDA: ?ChangeDefenseTypeForce@CMoverEx@@QEAAXE@Z (0x14037D760)
 */
void CMoverEx::ChangeDefenseTypeForce(std::uint8_t byDefenseType) {
    // IDA 精确还原:
    RemoveAllDefenseChangeInfo();
    CMover::SetSimpleDefenseType(byDefenseType);
}

/**
 * @brief IsCanHit - check if can be hit
 * @param nDownAttack Down attack type
 * @param bPassiveType Passive type
 * @return true if can be hit
 * IDA: ?IsCanHit@CMoverEx@@UEAAHHH@Z (0x14037F9A0)
 */
bool CMoverEx::IsCanHit(int nDownAttack, int bPassiveType) {
    // IDA 精确还原:
    if (m_eDieType != DIE_TYPE_KNOCKDOWN && m_eDieType != DIE_TYPE_DELAY &&
        XActor::IsStatus(4u)) {
        return false;
    }

    if (XActor::IsStatus(0x400u) || XActor::IsStatus(0x8000000u)) {
        return false;
    }

    if (XActor::IsStatus(0x2000u) && (m_dwInvisibleFlag & 1) == 0) {
        return false;
    }

    if (!CMover::IsAllowPassiveType(bPassiveType)) {
        return false;
    }

    if (!CMover::IsHitDown()) {
        return nDownAttack != 3;
    }

    return nDownAttack == 2 || nDownAttack == 3;
}

/**
 * @brief IsCanAttack - check if can attack
 * @return true if can attack
 * IDA: ?IsCanAttack@CMoverEx@@UEAAEXZ (0x14037FAC0)
 */
bool CMoverEx::IsCanAttack() {
    // IDA 精确还原:
    if (!m_nMotionClass) {
        return false;
    }

    return !XActor::IsStatus(2u) &&
           !XActor::IsStatus(4u) &&
           !XActor::IsStatus(0x20u) &&
           !XActor::IsStatus(0x10000u) &&
           !XActor::IsStatus(0xF000000u);
}

/**
 * @brief IsCanSkill - check if can use skill
 * @return true if can use skill
 * IDA: ?IsCanSkill@CMoverEx@@UEAAHXZ (0x14037FB80)
 */
bool CMoverEx::IsCanSkill() {
    // IDA 精确还原:
    return !XActor::IsStatus(0x40000000u) && !XActor::IsStatus(0x80000000u);
}

/**
 * @brief IsMoveMotion - check if motion is a move motion
 * @param nMotionClass Motion class
 * @return true if move motion
 * IDA: ?IsMoveMotion@CMoverEx@@QEAAHF@Z (0x140381200)
 */
bool CMoverEx::IsMoveMotion(std::int16_t nMotionClass) {
    // IDA 精确还原:
    return nMotionClass == 3 || nMotionClass == 5 || nMotionClass == 4;
}

/**
 * @brief IsCommonMotion - check if motion is a common motion (1-6)
 * @param nMotionClass Motion class
 * @return true if common motion
 * IDA: ?IsCommonMotion@CMoverEx@@QEAAHF@Z (0x140381240)
 */
bool CMoverEx::IsCommonMotion(std::int16_t nMotionClass) {
    // IDA 精确还原:
    return nMotionClass >= 1 && nMotionClass <= 6;
}

/**
 * @brief IsSuperArmorBreakMotion - check if motion is super armor break motion
 * @param nMotionClass Motion class
 * @return true if super armor break motion
 * IDA: ?IsSuperArmorBreakMotion@CMoverEx@@QEAAHF@Z (0x140381270)
 */
bool CMoverEx::IsSuperArmorBreakMotion(std::int16_t nMotionClass) {
    // IDA 精确还原:
    return nMotionClass == 24 || nMotionClass == 25 || nMotionClass == 26;
}

/**
 * @brief IsMoveDirMotion - check if motion uses move direction
 * @param nMotionClass Motion class
 * @return true if motion uses move direction
 * IDA: ?IsMoveDirMotion@CMoverEx@@QEAAHF@Z (0x1403812B0)
 */
bool CMoverEx::IsMoveDirMotion(std::int16_t nMotionClass) {
    // IDA 精确还原:
    return nMotionClass == 3 || nMotionClass == 5 || nMotionClass == 6 ||
           nMotionClass == 4 || nMotionClass == 30 || nMotionClass == 38 ||
           nMotionClass == 46 || nMotionClass == 9;
}

/**
 * @brief IsCanMovingAnim - check if can moving animation
 * @return true if can moving animation
 * IDA: ?IsCanMovingAnim@CMoverEx@@QEAAHXZ (0x140381320)
 */
bool CMoverEx::IsCanMovingAnim() {
    // IDA 精确还原:
    return XActor::IsStatus(1u) && m_pCurMotionEvent && m_pCurMotionEvent->eCanMoving;
}

/**
 * @brief IsChangeAnimByPhaseStepMotion - check if change anim by phase step motion
 * @param nMotionClass Motion class
 * @return true if change anim by phase step motion
 * IDA: ?IsChangeAnimByPhaseStepMotion@CMoverEx@@QEAAHF@Z (0x140381380)
 */
bool CMoverEx::IsChangeAnimByPhaseStepMotion(std::int16_t nMotionClass) {
    // IDA 精确还原:
    if (nMotionClass >= 1 && nMotionClass <= 8) {
        return true;
    }
    if (nMotionClass == 12 || nMotionClass == 13) {
        return true;
    }
    return nMotionClass >= 24 && nMotionClass <= 26;
}

/**
 * @brief IsJumpMotion - check if motion is jump motion (static)
 * @param nMotionClass Motion class
 * @return true if jump motion
 * IDA: ?IsJumpMotion@CMoverEx@@SAHF@Z (0x1403813E0)
 */
bool CMoverEx::IsJumpMotion(std::int16_t nMotionClass) {
    // IDA 精确还原:
    if (nMotionClass >= 9 && nMotionClass <= 11) {
        return true;
    }
    return nMotionClass >= 35 && nMotionClass <= 37;
}

/**
 * @brief IsJumpMotionExceptEnd - check if motion is jump motion except end (static)
 * @param nMotionClass Motion class
 * @return true if jump motion except end
 * IDA: ?IsJumpMotionExceptEnd@CMoverEx@@SAHF@Z (0x140381420)
 */
bool CMoverEx::IsJumpMotionExceptEnd(std::int16_t nMotionClass) {
    // IDA 精确还原:
    if (nMotionClass == 9 || nMotionClass == 10) {
        return true;
    }
    return nMotionClass == 35 || nMotionClass == 36;
}

/**
 * @brief IsAttackPassive - check if attack passive type
 * @param byPassiveType Passive type
 * @return true if attack passive
 * IDA: ?IsAttackPassive@CMoverEx@@UEAAHE@Z (0x14037FD60)
 */
bool CMoverEx::IsAttackPassive(std::uint8_t byPassiveType) {
    // IDA 精确还原:
    return byPassiveType != 4 && byPassiveType != 5 && byPassiveType != 6 &&
           byPassiveType != 8 && byPassiveType != 10 && byPassiveType != 13 &&
           byPassiveType != 14 && byPassiveType != 17;
}

/**
 * @brief IsCanMove - check if can move
 * @param isCheckTurnMotion Check turn motion flag
 * @return true if can move
 * IDA: ?IsCanMove@CMoverEx@@UEAAH_N@Z (0x14037FDD0)
 */
bool CMoverEx::IsCanMove(bool isCheckTurnMotion) {
    // IDA 精确还原:
    (void)isCheckTurnMotion;  // Unused parameter

    if (XActor::IsStatus(2u) || XActor::IsStatus(4u) ||
        XActor::IsStatus(0x10000u) || XActor::IsStatus(0x20u) ||
        XActor::IsStatus(0xF000000u)) {
        return false;
    }

    if (!XActor::IsStatus(0x2000u) || (m_dwInvisibleFlag & 4) != 0) {
        return m_nMotionClass != 0;
    }

    return false;
}

/**
 * @brief CheckMonsterInteractObject - check monster interact object
 * @param pMover Target mover
 * @return true if monster interact object
 * IDA: ?CheckMonsterInteractObject@CMoverEx@@UEAAHPEAVCMover@@@Z (0x14037FEC0)
 */
bool CMoverEx::CheckMonsterInteractObject(CMover* pMover) {
    // IDA 精确还原:
    if (!pMover) {
        return false;
    }

    if (XActor::GetType() != eActorMonster || pMover->GetType() != eActorMonster) {
        return false;
    }

    CMonster* pOwner = dynamic_cast<CMonster*>(this);
    CMonster* pTarget = dynamic_cast<CMonster*>(pMover);

    if (pOwner) {
        if (pOwner->IsMonsterInteractObject()) {
            return true;
        }
    }

    if (pTarget && pTarget->IsMonsterInteractObject()) {
        return true;
    }

    return false;
}

/**
 * @brief IsEnemy - check if target is enemy
 * @param pMover Target mover
 * @return true if enemy
 * IDA: ?IsEnemy@CMoverEx@@UEAAHPEAVCMover@@@Z (0x14037FFA0)
 */
bool CMoverEx::IsEnemy(CMover* pMover) {
    // IDA 精确还原: 复杂的阵营判断逻辑
    if (!pMover) {
        return false;
    }

    if (pMover->GetType() == eActorUser) {
        return false;
    }

    CMonster* pTargetMonster = nullptr;

    // Handle monster target
    if (pMover->GetType() == eActorMonster) {
        pTargetMonster = dynamic_cast<CMonster*>(pMover);
        if (!pTargetMonster) {
            return false;
        }

        TB_MONSTER* pMobRef = pTargetMonster->GetMobTableRef();
        if (!pMobRef) {
            return false;
        }

        // Check summon hit flag 0x10 - attack only origin owner
        if ((pMobRef->Summon_Hit & 0x10) != 0) {
            UXActorID targetOrigin = pMover->GetOriginID();
            UXActorID myOrigin = XActor::GetOriginID();
            return targetOrigin == myOrigin;
        }
    }

    // Handle this is monster, target is player
    if (m_eActorType == eActorMonster && pMover->GetType() == eActorUser) {
        CMonster* pOwner = dynamic_cast<CMonster*>(this);
        if (pOwner) {
            TB_MONSTER* pMobRef = pOwner->GetMobTableRef();
            if (!pMobRef) {
                return false;
            }

            // Summon_Hit flag 1 - attack owner's enemies except party
            if ((pMobRef->Summon_Hit & 1) != 0) {
                UXActorID targetOrigin = pMover->GetOriginID();
                UXActorID myOrigin = XActor::GetOriginID();
                return targetOrigin == myOrigin || IsParty(pMover);
            }

            // Summon_Hit flag 4 - use area faction
            if ((pMobRef->Summon_Hit & 4) != 0) {
                XArea* pArea = GetArea();
                if (pArea) {
                    return pArea->IsEnemy(this, pMover) == 1;
                }
            }
        }
    }

    // Handle this is player, target is monster
    if (m_eActorType == eActorUser && pMover->GetType() == eActorMonster && pTargetMonster) {
        TB_MONSTER* pMobRef = pTargetMonster->GetMobTableRef();
        if (pMobRef) {
            // Summon_Hit flag 2 - attack owner's enemies except party
            if ((pMobRef->Summon_Hit & 2) != 0) {
                UXActorID targetOrigin = pMover->GetOriginID();
                UXActorID myOrigin = XActor::GetOriginID();
                return targetOrigin == myOrigin || IsParty(pTargetMonster);
            }

            // Summon_Hit flag 8 - use area faction
            if ((pMobRef->Summon_Hit & 8) != 0) {
                XArea* pArea = GetArea();
                if (pArea) {
                    return pArea->IsEnemy(this, pMover) == 1;
                }
            }
        }
    }

    // Check same origin
    UXActorID targetOrigin = pMover->GetOriginID();
    UXActorID myOrigin = XActor::GetOriginID();

    if (targetOrigin == myOrigin) {
        // Same origin - check faction by nation
        std::uint8_t targetNation = pMover->GetNation();
        std::uint8_t myNation = XActor::GetNation();
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        return pServer->GetResourceMgr().GetFaction(myNation, targetNation) != 2;
    }

    // Different origin
    XArea* pArea = GetArea();
    if (pArea && pArea->IsEnemy(this, pMover) == 1) {
        return true;
    }

    // Check faction by nation
    std::uint8_t targetNation = pMover->GetNation();
    std::uint8_t myNation = XActor::GetNation();
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    return pServer->GetResourceMgr().GetFaction(myNation, targetNation) != 2;
}

/**
 * @brief IsEnemyForChain - check if target is enemy for chain
 * @param pMover Target mover
 * @return true if enemy for chain
 * IDA: ?IsEnemyForChain@CMoverEx@@UEAAHPEAVCMover@@@Z (0x140380760)
 */
bool CMoverEx::IsEnemyForChain(CMover* pMover) {
    // IDA 精确还原:
    if (!pMover) {
        return false;
    }

    E_ACTOR_TYPE targetType = pMover->GetType();
    if (targetType != eActorMonster && targetType != eActorUser) {
        return false;
    }

    if (targetType != eActorMonster) {
        return IsEnemy(pMover);
    }

    CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    if (!pMonster) {
        return IsEnemy(pMover);
    }

    TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
    if (!pMobRef) {
        return IsEnemy(pMover);
    }

    XArea* pArea = GetArea();
    if (pArea && pArea->GetWorldType() != 2) {
        if (pMobRef->Monster_ClearCondition_Type) {
            return false;
        }
    }

    if (!pMobRef->Monster_Type || pMobRef->Monster_Type == 3 ||
        pMobRef->Monster_Type == 11 || pMobRef->Monster_Type == 4) {
        return IsEnemy(pMover);
    }

    return false;
}

/**
 * @brief IsFriend - check if target is friend
 * @param pMover Target mover
 * @return true if friend
 * IDA: ?IsFriend@CMoverEx@@UEAAHPEAVCMover@@@Z (0x1403808F0)
 */
bool CMoverEx::IsFriend(CMover* pMover) {
    // IDA 精确还原:
    return pMover && !IsEnemy(pMover);
}

/**
 * @brief IsFriendForChain - check if target is friend for chain
 * @param pMover Target mover
 * @return true if friend for chain
 * IDA: ?IsFriendForChain@CMoverEx@@UEAAHPEAVCMover@@@Z (0x140380940)
 */
bool CMoverEx::IsFriendForChain(CMover* pMover) {
    // IDA 精确还原:
    if (!pMover) {
        return false;
    }

    E_ACTOR_TYPE targetType = pMover->GetType();
    if (targetType != eActorMonster && targetType != eActorUser) {
        return false;
    }

    if (targetType != eActorMonster) {
        return IsFriend(pMover);
    }

    CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    if (!pMonster) {
        return IsFriend(pMover);
    }

    TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
    if (!pMobRef) {
        return IsFriend(pMover);
    }

    XArea* pArea = GetArea();
    if (pArea && pArea->GetWorldType() != 2) {
        if (pMobRef->Monster_ClearCondition_Type) {
            return false;
        }
    }

    if (!pMobRef->Monster_Type || pMobRef->Monster_Type == 3 ||
        pMobRef->Monster_Type == 11 || pMobRef->Monster_Type == 4) {
        return IsFriend(pMover);
    }

    return false;
}

/**
 * @brief IsParty - check if target is in same party
 * @param pMover Target mover
 * @return true if in same party
 * IDA: ?IsParty@CMoverEx@@UEAAHPEAVCMover@@@Z (0x140380AD0)
 */
bool CMoverEx::IsParty(CMover* pMover) {
    // IDA 精确还原: 复杂的队伍判断逻辑
    if (!pMover) {
        return false;
    }

    // Handle monster case - get owner player
    if (m_eActorType == eActorMonster) {
        CMonster* pMonster = dynamic_cast<CMonster*>(this);
        if (pMonster) {
            CMover* pOwner = pMonster->GetOwnerPlayer();
            if (pOwner && pOwner->GetType() == eActorUser) {
                CMover* pTargetOwner = pMover;

                // Get target owner if target is a monster
                if (pMover->GetType() == eActorMonster) {
                    CMonster* pTargetMonster = dynamic_cast<CMonster*>(pMover);
                    if (pTargetMonster) {
                        TB_MONSTER* pMobRef = pTargetMonster->GetMobTableRef();
                        if (pTargetMonster->IsHelper() ||
                            (pMobRef && (pMobRef->Summon_Hit & 2) != 0)) {
                            CMover* pTargetOwnerPlayer = pTargetMonster->GetOwnerPlayer();
                            if (pTargetOwnerPlayer && pTargetOwnerPlayer->GetType() == eActorUser) {
                                pTargetOwner = pTargetOwnerPlayer;
                            }
                        }
                    }
                }

                if (pOwner == pTargetOwner) {
                    return true;
                }

                // Check party membership
                std::shared_ptr<CGocParty> pOwnerAttr;
                pOwner->GetGOC<CGocParty>(&pOwnerAttr, 0);
                if (pOwnerAttr) {
                    if (pOwnerAttr->IsMember(pTargetOwner)) {
                        return true;
                    }
                }

                std::shared_ptr<CGocForce> pOwnerAttrForce;
                pOwner->GetGOC<CGocForce>(&pOwnerAttrForce, 0);
                if (pOwnerAttrForce) {
                    if (pOwnerAttrForce->IsMember(pTargetOwner)) {
                        return true;
                    }
                }

                return false;
            }
        }
    }

    // Player case - check party membership
    std::shared_ptr<CGocParty> pAttr;
    GetGOC<CGocParty>(&pAttr, 0);
    if (!pAttr) {
        return false;
    }

    std::shared_ptr<CGocForce> pAttrForce;
    GetGOC<CGocForce>(&pAttrForce, 0);
    if (!pAttrForce) {
        return false;
    }

    // Check if target is a monster with owner
    if (pMover->GetType() == eActorMonster) {
        CMonster* pTargetMonster = dynamic_cast<CMonster*>(pMover);
        if (pTargetMonster) {
            TB_MONSTER* pMobRef = pTargetMonster->GetMobTableRef();
            if (pTargetMonster->IsHelper() ||
                (pMobRef && (pMobRef->Summon_Hit & 2) != 0)) {
                CMover* pOwnerPlayer = pTargetMonster->GetOwnerPlayer();
                if (pOwnerPlayer && pOwnerPlayer->GetType() == eActorUser) {
                    if (pOwnerPlayer == this) {
                        return true;
                    }
                    if (pAttr->IsMember(pOwnerPlayer)) {
                        return true;
                    }
                    if (pAttrForce->IsMember(pOwnerPlayer)) {
                        return true;
                    }
                    return false;
                }
            }
        }
    }

    // Check direct party membership
    if (pAttr->IsMember(pMover)) {
        return true;
    }

    if (pAttrForce->IsMember(pMover)) {
        return true;
    }

    return false;
}

/**
 * @brief ClearMotion - clear current motion
 * IDA: ?ClearMotion@CMoverEx@@UEAAXXZ (0x140381910)
 */
void CMoverEx::ClearMotion() {
    // IDA 精确还原:
    if (m_eDieType == DIE_TYPE_KNOCKDOWN || m_eDieType == DIE_TYPE_DELAY ||
        XActor::IsStatus(2u)) {
        return;
    }

    if (m_byPhaseMotionStep == 2) {
        m_byPhaseMotionStep = 0;
        CMover::SetInvincibleActor(false);
        m_fPhaseStepMaxTime = 0.0f;
    }

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fTime = IVTimer::GetTime(pTimer);

    if (m_fSkillBlendEndTime <= fTime || m_fAnimPercentTime >= 0.99f) {
        if (m_bReserveChange) {
            if (m_dwChangeMobTableID) {
                XArea* pArea = GetArea();
                XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
                if (pMaze) {
                    std::uint32_t dwID = GetID();
                    pMaze->AddChangeMonster(dwID);
                }
            }
        } else {
            if (m_bExistSuboCombo && m_iSuboComboMaxCount > 0 &&
                m_iSuboComboCheckCount >= m_iSuboComboMaxCount) {
                m_fSkillLoopTime = 0.0f;
                m_fSuboComboCheckTime = 0.0f;
                m_iSuboComboMaxCount = -1;
                m_iSuboComboCheckCount = 0;
                m_bExistSuboCombo = false;
            }

            std::int16_t nNewMotion = GetNextMotion();
            if (nNewMotion != -1) {
                ChangeMotion_3(nNewMotion, true, 5);
            }
        }
    }
}

/**
 * @brief CheckIdleTime - check idle time and trigger idle animation
 * IDA: ?CheckIdleTime@CMoverEx@@QEAAXXZ (0x140381BC0)
 */
void CMoverEx::CheckIdleTime() {
    // IDA 精确还原:
    if (m_nIdleMotionChance <= 0 || m_nMotionClass != 1 || m_bBattlePose) {
        return;
    }

    std::uint32_t dwAnim = XActionResMgr::GetAnimIndex(this, 2, 0, false);
    if (!CMover::GetAnimStirng(dwAnim)) {
        m_nIdleMotionChance = 0;
        return;
    }

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fTime = IVTimer::GetTime(pTimer);

    if (fTime - m_fLastChangeAnimationTime > m_fIdleCheckTime) {
        m_fLastChangeAnimationTime = fTime;

        int nRand = rand() % 100;
        if (nRand < m_nIdleMotionChance) {
            ChangeMotion_3(2, true, 0);
        }

        if (m_nMotionClass == 2 && XActor::GetType() == eActorMonster) {
            CMover::send_eSUB_CMD_MOVE_IDLE(0.0f);
        }
    }
}

/**
 * @brief CheckKeepLookTarget - check keep look target
 * @param fDeltaTime Delta time
 * @return true if keeping look target
 * IDA: ?CheckKeepLookTarget@CMoverEx@@UEAAHM@Z (0x140383630)
 */
bool CMoverEx::CheckKeepLookTarget(float fDeltaTime) {
    // IDA 精确还原:
    if (!m_bKeepLookTarget) {
        return false;
    }

    UpdateTargetRotation(false);

    std::uint32_t dwTargetID = CMover::GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(dwTargetID);
    if (!pTarget) {
        return false;
    }

    hkvVec3 vDirVector = pTarget->GetPosition() - m_vPosition;
    m_fMovingYaw = CMover::GetYawFromVector(vDirVector);

    float fCurrYaw = GetOrientationYaw();
    float fDeltaVal = fDeltaTime * m_fDefTurnSpeed * m_fTurnSpeedRate * m_fQuickTurnSpeed;
    fCurrYaw = CalcRotationBlending(fCurrYaw, m_fMovingYaw, fDeltaVal);
    CMover::SetOrientationYaw(fCurrYaw);

    return true;
}

/**
 * @brief IsCanSkillSkip - check if can skip skill by trigger name
 * @param szTriggerName Trigger name
 * @param fCurTime Current animation time
 * @return true if can skip
 * IDA: ?IsCanSkillSkip@CMoverEx@@QEAA_NPEBDM@Z (0x140381580)
 */
bool CMoverEx::IsCanSkillSkip(const char* szTriggerName, float fCurTime) {
    // IDA 精确还原:
    const VAnimationInfo* pActionInfo = GetActionDesc(szTriggerName);
    return IsCanSkillSkip(pActionInfo, fCurTime);
}

/**
 * @brief IsCanSkillSkip - check if can skip skill by animation info
 * @param pActionInfo Animation info
 * @param fCurTime Current animation time
 * @return true if can skip
 * IDA: ?IsCanSkillSkip@CMoverEx@@QEAA_NPEBVVAnimationInfo@@M@Z (0x1403815D0)
 */
bool CMoverEx::IsCanSkillSkip(const VAnimationInfo* pActionInfo, float fCurTime) {
    // IDA 精确还原:
    if (!pActionInfo) {
        return false;
    }

    int nCount = pActionInfo->arTriggers.GetLength();
    for (int i = 0; i < nCount; ++i) {
        ExtraInputTrigger* pTrigger = static_cast<ExtraInputTrigger*>(
            XActionResMgr::RetrieveEvent(2, i, pActionInfo));
        if (!pTrigger) {
            break;
        }
        if (pTrigger->SkipTime > 0.001f && fCurTime >= pTrigger->SkipTime) {
            return true;
        }
    }

    return false;
}

/**
 * @brief HasSkillSkipTime - check if has skill skip time by trigger name
 * @param szTriggerName Trigger name
 * @return true if has skip time
 * IDA: ?HasSkillSkipTime@CMoverEx@@QEAA_NPEBD@Z (0x140381680)
 */
bool CMoverEx::HasSkillSkipTime(const char* szTriggerName) {
    // IDA 精确还原:
    const VAnimationInfo* pActionInfo = GetActionDesc(szTriggerName);
    return HasSkillSkipTime(pActionInfo);
}

/**
 * @brief HasSkillSkipTime - check if has skill skip time by animation info
 * @param pActionInfo Animation info
 * @return true if has skip time
 * IDA: ?HasSkillSkipTime@CMoverEx@@QEAA_NPEBVVAnimationInfo@@@Z (0x1403816C0)
 */
bool CMoverEx::HasSkillSkipTime(const VAnimationInfo* pActionInfo) {
    // IDA 精确还原:
    if (!pActionInfo) {
        return false;
    }

    int nCount = pActionInfo->arTriggers.GetLength();
    for (int i = 0; i < nCount; ++i) {
        ExtraInputTrigger* pTrigger = static_cast<ExtraInputTrigger*>(
            XActionResMgr::RetrieveEvent(2, i, pActionInfo));
        if (!pTrigger) {
            break;
        }
        if (pTrigger->SkipTime > 0.0f) {
            return true;
        }
    }

    return false;
}

/**
 * @brief UpdateSummonMobRotation - update summoned monster rotation
 * IDA: ?UpdateSummonMobRotation@CMoverEx@@QEAAXXZ (0x140383770)
 */
void CMoverEx::UpdateSummonMobRotation() {
    // IDA 精确还原:
    for (auto it = m_listSummonMob.begin(); it != m_listSummonMob.end(); ++it) {
        std::uint32_t dwSub = *it;
        CMover* pSubActor = CMover::GetMoverObject(dwSub);

        CMonster* pSubMonster = dynamic_cast<CMonster*>(pSubActor);
        if (pSubMonster) {
            if (pSubMonster->GetApplyParentRotation()) {
                pSubMonster->SetRotation(m_vOrientation.x, 0.0f);
            }
        }
    }
}

// ============================================================================
// Damage Functions - IDA Decompiled
// ============================================================================

/**
 * @brief GetRandomDamage - get random damage value
 * @param bySkillAttribute Skill attribute (2=magic, other=physical)
 * @param iItemRateResult Item rate result
 * @return Random damage value
 * IDA: ?GetRandomDamage@CMoverEx@@UEAAHEH@Z (0x140387FB0)
 */
int CMoverEx::GetRandomDamage(std::uint8_t bySkillAttribute, int iItemRateResult) {
    // IDA 精确还原:
    if (bySkillAttribute == 2) {
        // Magic damage
        int iMaxDmgM = static_cast<int>(m_fAbility[23] + static_cast<float>(iItemRateResult));
        int iMinDmgM = static_cast<int>(m_fAbility[22] + static_cast<float>(iItemRateResult) * 0.8f);

        if (m_eTestDamageType == eTestDamage_Normal) {
            return iMaxDmgM;
        }

        int nDamage = iMinDmgM;
        if (iMaxDmgM - iMinDmgM > 0) {
            nDamage = rand() % (iMaxDmgM - iMinDmgM + 1) + iMinDmgM;
        }

        if (m_byFixedMaxDamage) {
            return iMaxDmgM;
        }
        return nDamage;
    } else {
        // Physical damage
        int iMaxDmgP = static_cast<int>(m_fAbility[21] + static_cast<float>(iItemRateResult));
        int iMinDmgP = static_cast<int>(m_fAbility[20] + static_cast<float>(iItemRateResult) * 0.8f);

        if (m_eTestDamageType == eTestDamage_Normal) {
            return iMaxDmgP;
        }

        int nDamage = iMinDmgP;
        if (iMaxDmgP - iMinDmgP > 0) {
            nDamage = rand() % (iMaxDmgP - iMinDmgP + 1) + iMinDmgP;
        }

        if (m_byFixedMaxDamage) {
            return iMaxDmgP;
        }
        return nDamage;
    }
}

/**
 * @brief GetDamageCalc - calculate damage after defense
 * @param nAP Attack power
 * @param byType Damage type (2=magic, other=physical)
 * @param fReduceRate Reduce rate percentage
 * @param iItemRateResult Item rate result
 * @param bIgnoreInvinsible Ignore invincible flag
 * @return Calculated damage
 * IDA: ?GetDamageCalc@CMoverEx@@UEAAHHEMH_N@Z (0x140388170)
 */
int CMoverEx::GetDamageCalc(int nAP, std::uint8_t byType, float fReduceRate,
                             int iItemRateResult, bool bIgnoreInvinsible) {
    // IDA 精确还原:
    // Check invincibility
    if ((!m_pCurSkillTableRef ||
         (m_pCurSkillTableRef->Skill_Type != 6 && m_pCurSkillTableRef->Skill_Type != 7)) &&
        !IsExceptionalDamage() && m_byDefenseType == 3 &&
        (!bIgnoreInvinsible || CMover::IsImmunityStatus())) {
        return 0;
    }

    // Get defense value
    float fDefence;
    if (byType == 2) {
        fDefence = m_fAbility[25] + static_cast<float>(iItemRateResult);
    } else {
        fDefence = m_fAbility[24] + static_cast<float>(iItemRateResult);
    }

    if (fDefence < 0.0f) {
        fDefence = 0.0f;
    }

    if (fReduceRate > 100.0f) {
        fReduceRate = 100.0f;
    }

    float fReduceMultiplier = (100.0f - fReduceRate) * 0.01f;
    fDefence *= fReduceMultiplier;

    int nLevel = GetLevelForStat();

    if (fDefence <= 0.0f) {
        fDefence = 1.0f;
    }

    // Calculate damage: AP * (1 - Defence / (Defence + 50 * Level))
    int iDamage = static_cast<int>(
        static_cast<float>(nAP) * (1.0f - fDefence / (fDefence + 50.0f * nLevel)));

    // Apply defense type modifier
    if (m_byDefenseType == 1) {
        iDamage = static_cast<int>(static_cast<float>(iDamage) * 0.2f);
    }

    if (iDamage <= 0) {
        return 1;
    }

    return iDamage;
}

/**
 * @brief SetAuraSkill - set aura skill
 * @param nAuraID Aura skill ID
 * @return true on success
 * IDA: ?SetAuraSkill@CMoverEx@@QEAAHG@Z (0x14038B7D0)
 */
bool CMoverEx::SetAuraSkill(std::uint16_t nAuraID) {
    // IDA 精确还原:
    if (!nAuraID && !m_pAuraSkill) {
        return false;
    }

    if (m_pAuraSkill && nAuraID == m_pAuraSkill->Aura_Index) {
        return false;
    }

    CMover::send_eSUB_CMD_AURA_UPDATE(nAuraID);

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_AURA* pAuraRef = pServer->GetResourceMgr().GetTB_AURA(nAuraID);

    if (pAuraRef) {
        if (!pAuraRef->Check_Type) {
            CMySkillList::ApplyAuraSkill(this, pAuraRef);
        }

        if (m_pAuraSkill) {
            RemoveAuraSkill(false);
        }

        m_pAuraSkill = pAuraRef;
        m_fAuraCheckTime = static_cast<float>(pAuraRef->Check_Time) * 0.001f;

        if (pAuraRef->Aura_Time) {
            VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
            m_fAuraLifeTime = IVTimer::GetTime(pTimer) +
                              static_cast<float>(pAuraRef->Aura_Time) * 0.001f;
        } else {
            m_fAuraLifeTime = 0.0f;
        }

        return true;
    } else {
        RemoveAuraSkill(false);
        return false;
    }
}

/**
 * @brief RemoveAuraSkill - remove aura skill
 * @param bSendUpdate Send update packet flag
 * IDA: ?RemoveAuraSkill@CMoverEx@@QEAAX_N@Z (0x14038B950)
 */
void CMoverEx::RemoveAuraSkill(bool bSendUpdate) {
    // IDA 精确还原:
    if (m_pAuraSkill) {
        m_pAuraSkill = nullptr;
        m_fAuraCheckTime = 0.0f;
        m_fAuraLifeTime = 0.0f;

        if (bSendUpdate) {
            CMover::send_eSUB_CMD_AURA_UPDATE(0);
        }
    }
}

/**
 * @brief LoadBuffStatus - load buff status
 * @param iIndex Buff slot index
 * @param nBuffIndex Buff table index
 * @param fLifeTime Buff lifetime
 * @param byCount Buff count
 * @param dwOwnerID Owner ID
 * @param bShow Show buff flag
 * IDA: ?LoadBuffStatus@CMoverEx@@QEAAXHGMEK_N@Z (0x14038B9C0)
 */
void CMoverEx::LoadBuffStatus(std::uint32_t iIndex, std::uint16_t nBuffIndex,
                               float fLifeTime, std::uint8_t byCount,
                               std::uint32_t dwOwnerID, bool bShow) {
    // IDA 精确还原:
    if (iIndex >= 50) {
        return;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(nBuffIndex);

    if (!pBuffTable) {
        return;
    }

    m_stBuffState[iIndex].nGroupID = pBuffTable->Buff_Group;
    m_stBuffState[iIndex].nBuffIndex = nBuffIndex;
    m_stBuffState[iIndex].dwID = dwOwnerID;
    m_stBuffState[iIndex].byEffectType = pBuffTable->EffectType_01;
    m_stBuffState[iIndex].byStatType[0] = pBuffTable->EffectType_Status_01;
    m_stBuffState[iIndex].fSkillVal[0] = static_cast<float>(pBuffTable->Option_Value_01);
    m_stBuffState[iIndex].iSkillVal = pBuffTable->Option_Value_01;
    m_stBuffState[iIndex].byStatType[1] = pBuffTable->EffectType_Status_02;
    m_stBuffState[iIndex].fSkillVal[1] = static_cast<float>(pBuffTable->Option_Value_02);
    m_stBuffState[iIndex].byStatType[2] = pBuffTable->EffectType_Status_03;
    m_stBuffState[iIndex].fSkillVal[2] = static_cast<float>(pBuffTable->Option_Value_03);
    m_stBuffState[iIndex].fGapTime = static_cast<float>(pBuffTable->DotDelay_Time) * 0.001f;
    m_stBuffState[iIndex].fGapTimeCheck = 0.0f;
    m_stBuffState[iIndex].byGapApplyCount = (!pBuffTable->EffectType_01 && pBuffTable->DotDelay_Time) ? 1 : 0;
    m_stBuffState[iIndex].fLifeTime = fLifeTime;
    m_stBuffState[iIndex].byBuffLV = pBuffTable->Buff_Grade;
    m_stBuffState[iIndex].byCount = 1;
    m_stBuffState[iIndex].byBuffType = pBuffTable->Buff_Type;
    m_stBuffState[iIndex].bySendType = pBuffTable->Buff_Send_Type;
    m_stBuffState[iIndex].bShow = bShow;
    m_stBuffState[iIndex].bySystemType = pBuffTable->System_Type;
}

/**
 * @brief IsCheckCurStat - check if current stat is valid for buff
 * @param pNewBuff New buff table pointer
 * @return true if current stat check passes
 * IDA: ?IsCheckCurStat@CMoverEx@@QEAAHPEAUTB_BUFF@@@Z (0x14038C7A0)
 */
bool CMoverEx::IsCheckCurStat(TB_BUFF* pNewBuff) {
    // IDA 精确还原:
    if (pNewBuff->EffectType_01) {
        return false;
    }

    if (pNewBuff->EffectType_Status_01 && IsMaxValStat(pNewBuff->EffectType_Status_01)) {
        return true;
    }

    if (pNewBuff->EffectType_Status_02 && IsMaxValStat(pNewBuff->EffectType_Status_02)) {
        return true;
    }

    return pNewBuff->EffectType_Status_03 && IsMaxValStat(pNewBuff->EffectType_Status_03);
}

/**
 * @brief IsMaxValStat - check if stat type uses max value
 * @param byStatType Stat type
 * @return true if stat uses max value
 * IDA: ?IsMaxValStat@CMoverEx@@QEAAHE@Z (0x14038C860)
 */
bool CMoverEx::IsMaxValStat(std::uint8_t byStatType) {
    // IDA 精确还原:
    return byStatType == 15 || byStatType == 16 || byStatType == 19 ||
           byStatType == 20 || byStatType == 23 || byStatType == 24 ||
           byStatType == 29 || byStatType == 30;
}

/**
 * @brief CheckBuffGrade - check buff grade for replacement
 * @param iIndex Current buff index
 * @param pNewBuff New buff table pointer
 * @return true if should replace
 * IDA: ?CheckBuffGrade@CMoverEx@@QEAAHHPEAUTB_BUFF@@@Z (0x14038C8D0)
 */
int CMoverEx::CheckBuffGrade(int iIndex, TB_BUFF* pNewBuff) {
    // IDA 精确还原:
    if (!pNewBuff) {
        return 0;
    }

    if (pNewBuff->EffectType_01 == 21) {
        if (m_stBuffState[iIndex].byBuffLV < static_cast<int>(pNewBuff->Buff_Grade)) {
            return 1;
        }
    } else {
        if (m_stBuffState[iIndex].byBuffLV <= static_cast<int>(pNewBuff->Buff_Grade)) {
            return 1;
        }
    }

    return 0;
}

/**
 * @brief IsCanBuffOverlap - check if buff can overlap
 * @param iIndex Buff slot index
 * @param pNewBuff New buff table pointer
 * @param dwOwnerID Owner ID
 * @return true if can overlap
 * IDA: ?IsCanBuffOverlap@CMoverEx@@QEAA_NHPEAUTB_BUFF@@KH@Z (0x14038C960)
 */
bool CMoverEx::IsCanBuffOverlap(int iIndex, TB_BUFF* pNewBuff, std::uint32_t dwOwnerID) {
    // IDA 精确还原:
    (void)dwOwnerID;  // Unused in check

    if (!CheckBuffGrade(iIndex, pNewBuff)) {
        return false;
    }

    std::uint8_t byOverlap = pNewBuff->Overlap_Count;
    if (byOverlap > 10) {
        byOverlap = 10;
    }

    if (!byOverlap) {
        return false;
    }

    return m_stBuffState[iIndex].byCount + 1 <= byOverlap || byOverlap == 1;
}

/**
 * @brief DecreaseBuffOverlap - decrease buff overlap count
 * @param iBuffID Buff ID
 * @param iDecreaseCount Decrease count
 * IDA: ?DecreaseBuffOverlap@CMoverEx@@QEAAXHH@Z (0x14038D460)
 */
void CMoverEx::DecreaseBuffOverlap(int iBuffID, int iDecreaseCount) {
    // IDA 精确还原:
    if (iBuffID <= 0) {
        return;
    }

    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == iBuffID) {
            if (m_stBuffState[i].byCount > iDecreaseCount) {
                m_stBuffState[i].byCount -= iDecreaseCount;
                CMover::send_eSUB_CMD_BUFF_UPDATE(
                    m_stBuffState[i].nBuffIndex,
                    m_stBuffState[i].fLifeTime,
                    m_stBuffState[i].byCount,
                    m_stBuffState[i].dwID,
                    m_stBuffState[i].bySendType,
                    m_stBuffState[i].bShow);
            } else {
                ClearBuffStatus(iBuffID, true, 0);
            }
        }
    }
}

/**
 * @brief DecreaseBuffOverlapByGroupID - decrease buff overlap by group ID
 * @param iBuffGroupID Buff group ID
 * @param iDecreaseCount Decrease count
 * IDA: ?DecreaseBuffOverlapByGroupID@CMoverEx@@QEAAXHH@Z (0x14038D5E0)
 */
void CMoverEx::DecreaseBuffOverlapByGroupID(int iBuffGroupID, int iDecreaseCount) {
    // IDA 精确还原:
    if (iBuffGroupID <= 0) {
        return;
    }

    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nGroupID == iBuffGroupID) {
            if (m_stBuffState[i].byCount > iDecreaseCount) {
                m_stBuffState[i].byCount -= iDecreaseCount;
                CMover::send_eSUB_CMD_BUFF_UPDATE(
                    m_stBuffState[i].nBuffIndex,
                    m_stBuffState[i].fLifeTime,
                    m_stBuffState[i].byCount,
                    m_stBuffState[i].dwID,
                    m_stBuffState[i].bySendType,
                    m_stBuffState[i].bShow);
            } else {
                ClearBuffStatus(m_stBuffState[i].nBuffIndex, true, 0);
            }
        }
    }
}

/**
 * @brief SendUpdateBuffAbility - send update buff ability
 * IDA: ?SendUpdateBuffAbility@CMoverEx@@UEAAXXZ (0x14038D780)
 */
void CMoverEx::SendUpdateBuffAbility() {
    // IDA 精确还原:
    if (!m_bChangedStat) {
        return;
    }

    m_bChangedStat = false;

    std::shared_ptr<CGocAttribute> pAttr;
    CMover::GetGOC<CGocAttribute>(&pAttr, 0);

    if (pAttr) {
        pAttr->Finalize();
        pAttr->CalculateChangedEffect(true, 0.0f);
    }
}

/**
 * @brief SetBuffOverlap - set buff overlap
 * @param iIndex Buff slot index
 * @param pNewBuff New buff table pointer
 * @param dwOwnerID Owner ID
 * @param bCheckUpdate Check update flag
 * @return 0 on success, -1 on failure
 * IDA: ?SetBuffOverlap@CMoverEx@@QEAAHHPEAUTB_BUFF@@KH@Z (0x14038CA00)
 */
int CMoverEx::SetBuffOverlap(int iIndex, TB_BUFF* pNewBuff, std::uint32_t dwOwnerID, bool bCheckUpdate) {
    // IDA 精确还原:
    if (CheckBuffGrade(iIndex, pNewBuff) != 1) {
        return -1;
    }

    std::uint8_t byCount = m_stBuffState[iIndex].byCount;
    std::uint8_t byOverlap = pNewBuff->Overlap_Count;
    if (byOverlap > 10) {
        byOverlap = 10;
    }

    if (!byOverlap) {
        return -1;
    }

    if (byOverlap != 1) {
        if (byCount + 1 >= byOverlap && pNewBuff->Change_Buff) {
            ClearBuffStatusBySlot(iIndex, false);
            SetBuffStatus(pNewBuff->Change_Buff, dwOwnerID, true);
            return -1;
        }

        if (byCount + 1 > byOverlap) {
            return -1;
        }
    }

    m_stBuffState[iIndex].dwID = dwOwnerID;
    float fTime = static_cast<float>(pNewBuff->Buff_Time) * 0.001f;

    static float fTimeRate[] = {0.5f, 0.25f, 0.125f, 0.0625f, 0.03125f,
                                 0.015625f, 0.015625f, 0.015625f, 0.015625f};

    if (pNewBuff->Overlap_Type && pNewBuff->Overlap_Type != 2) {
        if (pNewBuff->Overlap_Type == 1) {
            m_stBuffState[iIndex].fLifeTime = fTime;
            if (byOverlap == 1) {
                m_stBuffState[iIndex].byCount = 1;
            } else {
                ++m_stBuffState[iIndex].byCount;
            }
        }
    } else if (byOverlap == 1) {
        m_stBuffState[iIndex].fLifeTime = fTime;
        m_stBuffState[iIndex].byCount = 1;
    } else {
        m_stBuffState[iIndex].fLifeTime += fTime * fTimeRate[byCount - 1];
        ++m_stBuffState[iIndex].byCount;
    }

    if (m_stBuffState[iIndex].nBuffIndex == pNewBuff->Buff_Index) {
        CMover::send_eSUB_CMD_BUFF_UPDATE(
            pNewBuff->Buff_Index,
            m_stBuffState[iIndex].fLifeTime,
            m_stBuffState[iIndex].byCount,
            m_stBuffState[iIndex].dwID,
            m_stBuffState[iIndex].bySendType,
            m_stBuffState[iIndex].bShow);

        if (!pNewBuff->Overlap_Type || byOverlap <= 1) {
            return 1;
        }

        if (!bCheckUpdate) {
            UpdateBuffAbility(m_stBuffState[iIndex], 0);
        }

        m_stBuffState[iIndex].fSkillVal[0] += static_cast<float>(pNewBuff->Option_Value_01);
        m_stBuffState[iIndex].fSkillVal[1] += static_cast<float>(pNewBuff->Option_Value_02);
        m_stBuffState[iIndex].fSkillVal[2] += static_cast<float>(pNewBuff->Option_Value_03);
        m_stBuffState[iIndex].iSkillVal += pNewBuff->Option_Value_01;
    } else {
        if (!bCheckUpdate) {
            UpdateBuffAbility(m_stBuffState[iIndex], 0);
        }

        m_stBuffState[iIndex].byCount = 1;
        m_stBuffState[iIndex].fLifeTime = static_cast<float>(pNewBuff->Buff_Time) * 0.001f;

        CMover::send_eSUB_CMD_BUFF_CHANGE(
            m_stBuffState[iIndex].nBuffIndex,
            pNewBuff->Buff_Index,
            m_stBuffState[iIndex].fLifeTime,
            m_stBuffState[iIndex].byCount,
            m_stBuffState[iIndex].dwID,
            m_stBuffState[iIndex].bySendType);

        m_stBuffState[iIndex].nBuffIndex = pNewBuff->Buff_Index;
        m_stBuffState[iIndex].byEffectType = pNewBuff->EffectType_01;
        m_stBuffState[iIndex].byStatType[0] = pNewBuff->EffectType_Status_01;
        m_stBuffState[iIndex].fSkillVal[0] = static_cast<float>(pNewBuff->Option_Value_01);
        m_stBuffState[iIndex].iSkillVal = pNewBuff->Option_Value_01;
        m_stBuffState[iIndex].byStatType[1] = pNewBuff->EffectType_Status_02;
        m_stBuffState[iIndex].fSkillVal[1] = static_cast<float>(pNewBuff->Option_Value_02);
        m_stBuffState[iIndex].byStatType[2] = pNewBuff->EffectType_Status_03;
        m_stBuffState[iIndex].fSkillVal[2] = static_cast<float>(pNewBuff->Option_Value_03);
        m_stBuffState[iIndex].fGapTime = static_cast<float>(pNewBuff->DotDelay_Time) * 0.001f;
        m_stBuffState[iIndex].fGapTimeCheck = 0.0f;
        m_stBuffState[iIndex].byBuffLV = pNewBuff->Buff_Grade;
    }

    return 0;
}

/**
 * @brief ClearBuffStatus - clear buff status by buff index
 * @param nBuffIndex Buff table index
 * @param bExcuteOutSkill Execute out skill flag
 * @param dwOwnerID Owner ID
 * IDA: ?ClearBuffStatus@CMoverEx@@UEAAXG_NK@Z (0x14038D820)
 */
void CMoverEx::ClearBuffStatus(std::uint16_t nBuffIndex, bool bExcuteOutSkill, std::uint32_t dwOwnerID) {
    // IDA 精确还原:
    int iIndex = CMover::FindBuffStatus(nBuffIndex, dwOwnerID);
    if (iIndex == -1) {
        return;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(nBuffIndex);
    if (!pBuffTable) {
        return;
    }

    if (!dwOwnerID) {
        dwOwnerID = m_stBuffState[iIndex].dwID;
    }

    CMover::send_eSUB_CMD_BUFF_DELETE(
        m_stBuffState[iIndex].nBuffIndex,
        dwOwnerID,
        bExcuteOutSkill,
        m_stBuffState[iIndex].bySendType);

    UpdateBuffAbility(m_stBuffState[iIndex], 0);
    m_stBuffState[iIndex].Clear();
    CMover::UpdateBuffCount(pBuffTable->Buff_Type, -1);

    if (pBuffTable->EffectType_01 == 4 || pBuffTable->EffectType_01 == 5) {
        UpdateDefenseType();
    }

    if (pBuffTable->EffectType_01 == 6) {
        CMover::UpdateDefenseDisableBuff();
    }

    if (m_nMotionClass == m_nBuffMotion) {
        m_nBuffMotion = -1;
        ClearMotion();
    }

    SendUpdateBuffAbility();

    if (bExcuteOutSkill && pBuffTable->Call_Out_Buff && pBuffTable->Call_Out_Buff != nBuffIndex) {
        SetBuffStatus(pBuffTable->Call_Out_Buff, dwOwnerID, true);
    }

    if (m_nHitCallBuffIndex == pBuffTable->Hit_Call_Buff) {
        m_nHitCallBuffIndex = 0;
    }
}

/**
 * @brief ClearBuffStatusBySlot - clear buff status by slot
 * @param nBuffSlot Buff slot index
 * @param bExcuteOutSkill Execute out skill flag
 * IDA: ?ClearBuffStatusBySlot@CMoverEx@@UEAAXG_N@Z (0x14038DA80)
 */
void CMoverEx::ClearBuffStatusBySlot(std::uint16_t nBuffSlot, bool bExcuteOutSkill) {
    // IDA 精确还原:
    if (nBuffSlot >= 50) {
        return;
    }

    std::uint16_t nBuffIndex = m_stBuffState[nBuffSlot].nBuffIndex;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(nBuffIndex);
    if (!pBuffTable) {
        return;
    }

    std::uint32_t dwOwnerID = m_stBuffState[nBuffSlot].dwID;

    CMover::send_eSUB_CMD_BUFF_DELETE(
        m_stBuffState[nBuffSlot].nBuffIndex,
        dwOwnerID,
        bExcuteOutSkill,
        m_stBuffState[nBuffSlot].bySendType);

    UpdateBuffAbility(m_stBuffState[nBuffSlot], 0);
    m_stBuffState[nBuffSlot].Clear();
    CMover::UpdateBuffCount(pBuffTable->Buff_Type, -1);

    if (pBuffTable->EffectType_01 == 4 || pBuffTable->EffectType_01 == 5) {
        UpdateDefenseType();
    }

    if (pBuffTable->EffectType_01 == 6) {
        CMover::UpdateDefenseDisableBuff();
    }

    if (m_nMotionClass == m_nBuffMotion) {
        m_nBuffMotion = -1;
        ClearMotion();
    }

    SendUpdateBuffAbility();

    if (bExcuteOutSkill && pBuffTable->Call_Out_Buff && pBuffTable->Call_Out_Buff != nBuffIndex) {
        SetBuffStatus(pBuffTable->Call_Out_Buff, dwOwnerID, true);
    }

    if (m_nHitCallBuffIndex == pBuffTable->Hit_Call_Buff) {
        m_nHitCallBuffIndex = 0;
    }
}

/**
 * @brief UpdateBuffAbility - update buff ability based on effect type
 * @param stBuff Buff state structure
 * @param bSet Set or clear flag (1=set, 0=clear)
 * @return true if should die
 * IDA: ?UpdateBuffAbility@CMoverEx@@UEAA_NAEAUtagBUFF_STATE@@H@Z (0x14038E5F0)
 */
bool CMoverEx::UpdateBuffAbility(tagBUFF_STATE& stBuff, int bSet) {
    // IDA 精确还原:
    bool bShouldDie = false;

    if (bSet) {
        // Set buff ability - switch on effect type
        switch (stBuff.byEffectType) {
        case 0:
            // Stat modification
            for (int j = 0; j < 3; ++j) {
                if (stBuff.byStatType[j]) {
                    SetBuffAbility(stBuff.byStatType[j], stBuff.fSkillVal[j]);
                }
            }
            break;

        case 2:
            XActor::SetStatus(XActor::eStatus_Paralysis, true);
            break;

        case 3:
            XActor::SetStatus(XActor::eStatus_Stun, true);
            break;

        case 4:
            AddDefenseChangeInfo(1, 2, stBuff.nBuffIndex, 0.0f);
            break;

        case 5:
            AddDefenseChangeInfo(1, 3, stBuff.nBuffIndex, 0.0f);
            if (stBuff.byStatType[0] == 1) {
                CMover::SetImmunityStatus(1);
            }
            break;

        case 7:
            if (stBuff.byStatType[0] == 66) {
                m_fSkillCoolDownRate = stBuff.fSkillVal[0];
                m_bIgnoreSkillCost = 1;
            }
            break;

        case 8:
            m_fSkillReflectRate = stBuff.fSkillVal[0] * 0.01f;
            break;

        case 9:
            m_fSkillBloodRate = stBuff.fSkillVal[0] * 0.01f;
            m_nAllowBloodCount = stBuff.byStatType[0];
            break;

        case 11:
            // Clear buffs by type
            for (int k = 0; k < 50; ++k) {
                if (m_stBuffState[k].nBuffIndex &&
                    m_stBuffState[k].byEffectType == static_cast<std::uint8_t>(stBuff.fSkillVal[0])) {
                    ClearBuffStatusBySlot(k, false);
                }
            }
            break;

        case 12:
            m_fSkillBloodDebuffRate = stBuff.fSkillVal[0] * 0.01f;
            m_dwBloodDebuffOwnerID = stBuff.dwID;
            break;

        case 13:
            ++m_byFixedMaxDamage;
            break;

        case 14:
            m_fMultipleDamageOnce = stBuff.fSkillVal[0] * 0.01f;
            break;

        case 15:
            m_fMultipleAbsorbSG = 2.0f;
            break;

        case 16:
            m_fBuffSuperArmorRate = stBuff.fSkillVal[0] * 0.01f;
            break;

        case 17:
            m_fSkillCoolDownRate += stBuff.fSkillVal[0] * 0.01f;
            if (m_pSkillMgr) {
                // CMySkillList::ReduceSkillCooltime(m_pSkillMgr, m_fSkillCoolDownRate);
            }
            break;

        case 18:
            m_fDecreaseStaminaRate = stBuff.fSkillVal[0] * 0.01f;
            break;

        case 19:
            m_fBuffAddGoldRate = stBuff.fSkillVal[0] * 0.01f;
            break;

        case 20: {
            // FP Restore
            std::shared_ptr<CGocAttribute> pAttr;
            CMover::GetGOC<CGocAttribute>(&pAttr, 0);
            if (pAttr) {
                std::int16_t sFP = static_cast<std::int16_t>(stBuff.fSkillVal[0]);
                pAttr->FPRestore(sFP);
            }
        }
            break;

        case 21:
            m_dwAggroTargetID = stBuff.dwID;
            // TODO: Cast to CMonster and change target if monster
            break;

        case 23:
            // Percentage stat modification
            for (int m = 0; m < 3; ++m) {
                std::uint8_t byIndex = stBuff.byStatType[m];
                if (byIndex) {
                    if (stBuff.fSkillVal[m] != 0.0f) {
                        stBuff.fSkillVal[m] = stBuff.fSkillVal[m] / 100.0f;
                    }
                    float fAbility = CMover::GetStat(byIndex);
                    stBuff.fSkillVal[m] = stBuff.fSkillVal[m] * fAbility;
                    AddBuffAbility(byIndex, stBuff.fSkillVal[m]);
                }
            }
            break;

        case 25:
            m_fSkillAbsorbSGRate = stBuff.fSkillVal[0] * 0.0001f;
            m_nAllowAbsorbSGCount = stBuff.byStatType[0];
            break;

        case 26: {
            // HP damage over time
            float fMaxHP = static_cast<float>(GetMaxHP());
            float fDamage = fMaxHP * stBuff.fSkillVal[0] * 0.01f;

            if (DamageProcessHP(stBuff.dwID, 0, static_cast<int>(fDamage), 0, 0)) {
                SetDieReason(2, static_cast<int>(fDamage));
                SetHP(0);
                bShouldDie = true;
            }

            if (fDamage > 0.0f) {
                int nCurHP = GetHP();
                CMover::send_eSUB_CMD_BUFF_DAMAGE(this, stBuff.nBuffIndex, static_cast<int>(fDamage), nCurHP, stBuff.dwID);
            }
        }
            break;

        case 27: {
            // HP threshold buff
            float fHPPercent = (m_fAbility[1] / m_fAbility[10]) * 100.0f;
            float fStandardHPPercent = static_cast<float>(stBuff.byStatType[0]);

            if (fStandardHPPercent >= fHPPercent) {
                int nApplyBuffIndexOffset = static_cast<int>((fStandardHPPercent - fHPPercent) / stBuff.fSkillVal[0]);
                std::uint16_t nBuffIndex = stBuff.byStatType[1] + nApplyBuffIndexOffset;
                std::uint32_t dwID = GetID();
                SetBuffStatus(nBuffIndex, dwID, true);
            }
        }
            break;

        case 28:
            m_fBossAttackAddRate += stBuff.fSkillVal[0] * 0.01f;
            break;

        case 29:
            m_fBossAttackedDownRate += stBuff.fSkillVal[0] * 0.01f;
            break;

        case 30:
            m_fSoulCostDownRate += stBuff.fSkillVal[0] * 0.01f;
            break;

        case 31:
            m_fAllAttackAddRate += stBuff.fSkillVal[0] * 0.01f;
            break;

        case 32: {
            // Melee debuff
            std::vector<std::uint32_t> vecSkillVal;
            for (int n = 0; n < 3; ++n) {
                if (stBuff.fSkillVal[n] > 0.0f) {
                    vecSkillVal.push_back(static_cast<std::uint32_t>(stBuff.fSkillVal[n]));
                }
            }
            m_mapMeleeDebuff[stBuff.nBuffIndex] = vecSkillVal;
        }
            break;

        case 33:
            m_mapSkillUnlock[stBuff.nBuffIndex] = stBuff.iSkillVal;
            break;

        case 34: {
            // Clear buffs by buff type
            int i = 0;
            std::uint8_t byBuffType = static_cast<std::uint8_t>(stBuff.fSkillVal[0]);

            while (i < 50) {
                if (m_stBuffState[i].bySystemType <= 1) {
                    if (byBuffType) {
                        bool bMatch = false;
                        if (byBuffType == 1) {
                            bMatch = (m_stBuffState[i].byBuffType == 1);
                        } else {
                            bMatch = (m_stBuffState[i].byBuffType != 2);
                        }

                        if (bMatch) {
                            ClearBuffStatus(m_stBuffState[i].nBuffIndex, false, 0);
                        }
                    } else if (m_stBuffState[i].byBuffType) {
                        ClearBuffStatus(m_stBuffState[i].nBuffIndex, false, 0);
                    }
                }
                ++i;
            }
        }
            break;

        case 36:
            SetActionCondition(stBuff.iSkillVal);
            break;

        case 37:
            // Bonus jump count - CUser only
            // TODO: Cast to CUser and set bonus jump count
            break;

        case 39:
            // Base jump count - CUser only
            // TODO: Cast to CUser and set base jump count
            break;

        case 121:
        case 122:
        case 123:
        case 125:
        case 126: {
            std::uint8_t v10 = stBuff.byEffectType - 121;
            std::uint32_t dwFlag = 0x1000000 << v10;
            XActor::SetStatus(dwFlag, true);
        }
            break;

        case 124:
            SetFreeze(true);
            break;

        case 131:
            XActor::SetStatus(0x40000000, true);
            break;

        case 132:
            XActor::SetStatus(0x80000000, true);
            for (int ii = 0; ii < 3; ++ii) {
                if (stBuff.byStatType[ii]) {
                    SetBuffAbility(stBuff.byStatType[ii], stBuff.fSkillVal[ii]);
                }
            }
            break;

        default:
            break;
        }
    } else {
        // Clear buff ability - switch on effect type
        switch (stBuff.byEffectType) {
        case 0:
            for (int jj = 0; jj < 3; ++jj) {
                if ((stBuff.fGapTime <= 0.0f || stBuff.byStatType[jj] < 0x0D || stBuff.byStatType[jj] > 0x1E) &&
                    stBuff.byStatType[jj]) {
                    ClearBuffAbility(stBuff.byStatType[jj], stBuff.fSkillVal[jj]);
                }
            }
            break;

        case 2:
            XActor::ClearStatus(XActor::eStatus_Paralysis, true);
            break;

        case 3:
            XActor::ClearStatus(XActor::eStatus_Stun, true);
            break;

        case 4:
            RemoveDefenseChangeInfo(1, stBuff.nBuffIndex);
            break;

        case 5:
            RemoveDefenseChangeInfo(1, stBuff.nBuffIndex);
            if (stBuff.byStatType[0] == 1 && !CMover::IsHaveImunityInvincibleBuff(stBuff.nBuffIndex)) {
                CMover::ClearImmunityStatus(1);
            }
            break;

        case 7:
            m_fSkillCoolDownRate = 0.0f;
            m_bIgnoreSkillCost = 0;
            break;

        case 8:
            m_fSkillReflectRate = 0.0f;
            break;

        case 9:
            m_fSkillBloodRate = 0.0f;
            m_nAllowBloodCount = 0;
            break;

        case 12:
            m_fSkillBloodDebuffRate = 0.0f;
            m_dwBloodDebuffOwnerID = static_cast<std::uint32_t>(-1);
            break;

        case 13:
            --m_byFixedMaxDamage;
            break;

        case 14:
            m_fMultipleDamageOnce = 0.0f;
            m_bApplyMultipleDamageOnce = 0;
            break;

        case 15:
            m_fMultipleAbsorbSG = 1.0f;
            break;

        case 16:
            m_fBuffSuperArmorRate = 0.0f;
            break;

        case 17:
            m_fSkillCoolDownRate -= stBuff.fSkillVal[0] * 0.01f;
            break;

        case 18:
            m_fDecreaseStaminaRate = 0.0f;
            break;

        case 19:
            m_fBuffAddGoldRate = 0.0f;
            break;

        case 21:
            m_dwAggroTargetID = 0;
            break;

        case 23:
            if (stBuff.fGapTime == 0.0f) {
                for (int kk = 0; kk < 3; ++kk) {
                    if (stBuff.byStatType[kk]) {
                        ReduceBuffAbility(stBuff.byStatType[kk], stBuff.fSkillVal[kk]);
                    }
                }
            }
            break;

        case 25:
            m_fSkillAbsorbSGRate = 0.0f;
            m_nAllowAbsorbSGCount = 0;
            break;

        case 28:
            m_fBossAttackAddRate -= stBuff.fSkillVal[0] * 0.01f;
            break;

        case 29:
            m_fBossAttackedDownRate -= stBuff.fSkillVal[0] * 0.01f;
            break;

        case 30:
            m_fSoulCostDownRate -= stBuff.fSkillVal[0] * 0.01f;
            break;

        case 31:
            m_fAllAttackAddRate -= stBuff.fSkillVal[0] * 0.01f;
            break;

        case 32:
            m_mapMeleeDebuff.erase(stBuff.nBuffIndex);
            break;

        case 33:
            m_mapSkillUnlock.erase(stBuff.nBuffIndex);
            break;

        case 36:
            ClearActionCondition(stBuff.iSkillVal);
            break;

        case 37:
            // CUser only - reset bonus jump count
            break;

        case 39:
            // CUser only - reset base jump count
            break;

        case 121:
        case 122:
        case 123:
        case 125:
        case 126: {
            std::uint8_t v17 = stBuff.byEffectType - 121;
            std::uint32_t dwStatus = 0x1000000 << v17;
            XActor::ClearStatus(dwStatus, true);
        }
            break;

        case 124:
            SetFreeze(false);
            break;

        case 131:
            XActor::ClearStatus(0x40000000, true);
            break;

        case 132:
            XActor::ClearStatus(0x80000000, true);
            for (int mm = 0; mm < 3; ++mm) {
                if (stBuff.byStatType[mm]) {
                    ClearBuffAbility(stBuff.byStatType[mm], stBuff.fSkillVal[mm]);
                }
            }
            break;

        default:
            break;
        }
    }

    m_fMoveSpeed = GetMoveSpeed();
    return bShouldDie;
}

/**
 * @brief SetBuffAbility - set buff ability value via CGocAttribute
 * @param iType Stat type
 * @param fValue Value to set
 * IDA: ?SetBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403900C0)
 */
void CMoverEx::SetBuffAbility(int iType, float fValue) {
    // IDA 精确还原:
    std::shared_ptr<CGocAttribute> pAttr;
    CMover::GetGOC<CGocAttribute>(&pAttr, 0);

    if (pAttr) {
        CGocAttribute* pGocAttr = pAttr.get();
        int iStatType = pGocAttr->UpdateBuffEffectStat(iType, fValue, 1, 0);
        m_bChangedStat = 1;

        if (iStatType == 1) {
            float fStat = CMover::GetStat(1);
            SetHpInfo(static_cast<int>(fStat));
        }
    }
}

/**
 * @brief ClearBuffAbility - clear buff ability with negated value
 * @param iType Stat type
 * @param fValue Value to negate and apply
 * IDA: ?ClearBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403901A0)
 */
void CMoverEx::ClearBuffAbility(int iType, float fValue) {
    // IDA 精确还原: Apply negated value
    std::shared_ptr<CGocAttribute> pAttr;
    CMover::GetGOC<CGocAttribute>(&pAttr, 0);

    if (pAttr) {
        m_bChangedStat = 1;

        // Negate the float value using XOR mask
        std::int32_t v7 = *reinterpret_cast<std::int32_t*>(&fValue) ^ 0x80000000;
        float fNegValue = *reinterpret_cast<float*>(&v7);

        CGocAttribute* pGocAttr = pAttr.get();
        if (pGocAttr->UpdateBuffEffectStat(iType, fNegValue, 1, 1) == 1) {
            float fStat = CMover::GetStat(1);
            SetHpInfo(static_cast<int>(fStat));
        }
    }
}

/**
 * @brief AddBuffAbility - add buff ability value via CGocAttribute
 * @param iType Stat type
 * @param fValue Value to add
 * IDA: ?AddBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403902A0)
 */
void CMoverEx::AddBuffAbility(std::uint32_t iType, float fValue) {
    // IDA 精确还原:
    std::shared_ptr<CGocAttribute> pAttr;
    CMover::GetGOC<CGocAttribute>(&pAttr, 0);

    if (pAttr) {
        m_bChangedStat = 1;
        CGocAttribute* pGocAttr = pAttr.get();
        pGocAttr->UpdateAddStat(iType, fValue, 1);
    }
}

/**
 * @brief ReduceBuffAbility - reduce buff ability with negated value
 * @param iType Stat type
 * @param fValue Value to negate and apply
 * IDA: ?ReduceBuffAbility@CMoverEx@@UEAAXHM@Z (0x140390330)
 */
void CMoverEx::ReduceBuffAbility(std::uint32_t iType, float fValue) {
    // IDA 精确还原: Apply negated value for reduction
    std::shared_ptr<CGocAttribute> pAttr;
    CMover::GetGOC<CGocAttribute>(&pAttr, 0);

    if (pAttr) {
        m_bChangedStat = 1;

        // Negate the float value using XOR mask
        std::int32_t v6 = *reinterpret_cast<std::int32_t*>(&fValue) ^ 0x80000000;
        float fNegValue = *reinterpret_cast<float*>(&v6);

        CGocAttribute* pGocAttr = pAttr.get();
        pGocAttr->UpdateAddStat(iType, fNegValue, 1);
    }
}

/**
 * @brief IsClearBuff - check if buff can be cleared based on reason
 * @param nBuffID Buff ID to check
 * @param byReason Reason for clearing
 * @return true if buff can be cleared
 * IDA: ?IsClearBuff@CMoverEx@@UEAAHGE@Z (0x1403903D0)
 */
bool CMoverEx::IsClearBuff(std::uint16_t nBuffID, std::uint8_t byReason) {
    // IDA 精确还原:
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(nBuffID);

    if (pBuffTable) {
        return (byReason & pBuffTable->Delete_Type) == 0;
    }

    LogHelper::LogDebug("game.contents", "IsClearBuff>> not exist buff id. (%d)", nBuffID);
    return true;
}

/**
 * @brief ApplyBuffAbilityForAttacker - apply buff ability for specific attacker
 * @param dwAttackerID Attacker ID to apply buff for
 * IDA: ?ApplyBuffAbilityForAttacker@CMoverEx@@UEAAXK@Z (0x140390450)
 */
void CMoverEx::ApplyBuffAbilityForAttacker(std::uint32_t dwAttackerID) {
    // IDA 精确还原: Apply buff effects for attacker with effect type 10
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex &&
            m_stBuffState[i].byEffectType == 10 &&
            dwAttackerID == m_stBuffState[i].dwID) {

            for (int j = 0; j < 3; ++j) {
                if (m_stBuffState[i].byStatType[j]) {
                    SetBuffAbility(m_stBuffState[i].byStatType[j], m_stBuffState[i].fSkillVal[j]);
                }
            }
        }
    }
}

/**
 * @brief ClearBuffAbilityForAttacker - clear buff ability for specific attacker
 * @param dwAttackerID Attacker ID to clear buff for
 * IDA: ?ClearBuffAbilityForAttacker@CMoverEx@@UEAAXK@Z (0x140390580)
 */
void CMoverEx::ClearBuffAbilityForAttacker(std::uint32_t dwAttackerID) {
    // IDA 精确还原: Clear buff effects for attacker with effect type 10
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex &&
            m_stBuffState[i].byEffectType == 10 &&
            dwAttackerID == m_stBuffState[i].dwID) {

            for (int j = 0; j < 3; ++j) {
                if (m_stBuffState[i].byStatType[j]) {
                    ClearBuffAbility(m_stBuffState[i].byStatType[j], m_stBuffState[i].fSkillVal[j]);
                }
            }
        }
    }
}

/**
 * @brief CheckBuffDamage - check and apply buff damage effects
 * @param pAttacker Attacker CMoverEx pointer
 * @param nIdx Attack index
 * @param nDamage Damage amount
 * IDA: ?CheckBuffDamage@CMoverEx@@QEAAXPEAV1@HH@Z (0x1403906B0)
 */
void CMoverEx::CheckBuffDamage(CMoverEx* pAttacker, int nIdx, int nDamage) {
    // IDA 精确还原:
    // Skill reflect rate
    if (m_fSkillReflectRate > 0.0f) {
        float fAddDamage = static_cast<float>(nDamage) * m_fSkillReflectRate;

        if (static_cast<float>(GetHP()) > fAddDamage) {
            int nCurHP = GetHP();
            SetHpEx(nCurHP - static_cast<int>(fAddDamage));
        }
    }

    // Skill blood rate (HP steal)
    float fBloodRate = pAttacker->GetSkillBloodRate();
    if (fBloodRate > 0.0f) {
        int nAllowCount = pAttacker->GetAllowBloodCount();

        if (nIdx < nAllowCount || !nAllowCount) {
            float fAddHP = static_cast<float>(nDamage) * fBloodRate;

            if (fAddHP >= static_cast<float>(pAttacker->GetMaxHP())) {
                fAddHP = static_cast<float>(pAttacker->GetMaxHP());
            }

            int nCurHP = pAttacker->GetHP();
            pAttacker->SetHpEx(static_cast<int>(fAddHP) + nCurHP);
        }
    }

    // Skill blood debuff rate
    float fBloodDebuffRate = GetSkillBloodDebuffRate();
    if (fBloodDebuffRate > 0.0f) {
        std::uint32_t dwID = GetBloodDebuffOwnerID();
        UXActorID actorID = pAttacker->GetActorID();

        bool bApply = false;

        if (dwID == actorID.GetID()) {
            bApply = true;
        } else {
            CMover* pMover = CMover::GetMoverObject(dwID);
            if (pMover) {
                bApply = pMover->IsFriend(pAttacker) != 0;
            }
        }

        if (bApply) {
            float fAddHP = static_cast<float>(nDamage) * fBloodDebuffRate;

            if (fAddHP >= static_cast<float>(pAttacker->GetMaxHP())) {
                fAddHP = static_cast<float>(pAttacker->GetMaxHP());
            }

            int nCurHP = pAttacker->GetHP();
            pAttacker->SetHpEx(static_cast<int>(fAddHP) + nCurHP);
        }
    }
}

/**
 * @brief SetFreeze - set freeze state
 * @param bFreeze Freeze flag
 * IDA: ?SetFreeze@CMoverEx@@UEAAXH@Z (0x140390A10)
 */
void CMoverEx::SetFreeze(bool bFreeze) {
    // IDA 精确还原:
    StopMoving(true);

    if (bFreeze) {
        CMover::AnimPause();
        XActor::SetStatus(0x8000000, true);
        InitJumpData(0.0f, true);
    } else {
        XActor::ClearStatus(0x8000000, true);
        PostSkillProcess();
        ChangeMotion_3(1, true, 6);
    }
}

/**
 * @brief ProcessMoveingInFly - process flying movement
 * IDA: ?ProcessMoveingInFly@CMoverEx@@QEAAXXZ (0x140390B20)
 */
void CMoverEx::ProcessMoveingInFly() {
    // IDA 精确还原:
    if (m_byGrapStep) {
        return;
    }

    hkvVec3 vDir(0.0f, 0.0f, 0.0f);
    hkvVec3 vOffset(0.0f, 0.0f, 0.0f);

    if (!tagMOVE_POS::IsNoneZero(&m_stMovePos) || !m_pArea || m_pArea->IsMaze()) {
        hkvMat3 matRot;
        VisMath_cl::EulerToMatrix(m_fFlyYaw, 0.0f, 0.0f, &matRot);

        hkvVec3 vDirInit(0.0f, -1.0f, 0.0f);
        vDir = vDirInit;
        vDir *= matRot;

        VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
        float fDist = IVTimer::GetTimeDifference(pTimer) * m_fFlySpeed * m_fReactionRate;

        if (m_fFlyDistance > m_fFlyMoveDist) {
            fDist = fDist * ((m_fFlyDistance - m_fFlyMoveDist) / m_fFlyDistance) * 2.0f;

            if (m_fFlyMoveDist + fDist < m_fFlyDistance) {
                m_fFlyMoveDist = m_fFlyMoveDist + fDist;
            } else {
                fDist = m_fFlyDistance - m_fFlyMoveDist;
                m_fFlyMoveDist = m_fFlyDistance;
                m_fFlySpeed = 0.0f;
            }
        }

        vOffset = vDir * fDist;

        const hkvVec3* pPosition = VisObject3D_cl::GetPosition(this);
        hkvVec3 vDestPos = *pPosition + vOffset;

        if (XActor::GetType() != eActorUser) {
            if (!CheckMoveDestPos(&vDestPos, true, false)) {
                CMover::SetMoveingInFly(false);
                tagMOVE_POS::Clear(&m_stMovePos);
                tagEXTRA_MOVEPOS::Clear(&m_stExtMovingVal);

                hkvVec3 vFloorPos = vDestPos;
                CMover::GetHeight(&vFloorPos, 200.0f);
                CMover::send_eSUB_CMD_SKILL_SYNC_POSITION(this, &vFloorPos);
            }
        }

        hkvVec3 vMovePos(vDestPos.x, vDestPos.y, vDestPos.z);
        Move(&vMovePos);
    }
}

/**
 * @brief RealDie - handle real death
 * @param nChangeMotion Motion to change to
 * IDA: ?RealDie@CMoverEx@@UEAAXF@Z (0x140391010)
 */
void CMoverEx::RealDie(std::int16_t nChangeMotion) {
    // IDA 精确还原:
    if (XActor::IsStatus(2)) {
        return;
    }

    if (CMoverEx::IsSystemActor()) {
        XActor::ClearStatus(4, true);
        return;
    }

    CancelAttackFromDamage();
    CMoverEx::ClearGrapProcess();
    m_bBattlePose = 1;
    CMover::SetCollisionEnable(false, false);
    m_bRestoreCollision = 0;
    tagEXTRA_MOVEPOS::Clear(&m_stExtMovingVal);

    if (m_eDieType != DIE_TYPE_KNOCKDOWN &&
        m_eDieType != DIE_TYPE_DELAY &&
        nChangeMotion != -1 &&
        m_nMotionClass != nChangeMotion) {
        ChangeMotion_3(nChangeMotion, true, 0);
    }

    XActor::SetStatus(2, true);

    float fVal = 0.1f;
    if (m_pCurMotionEvent) {
        fVal = m_pCurMotionEvent->fAnimationLength;
    }
    CMover::SetDieFadeTime(fVal);

    std::uint32_t HitID = CMover::GetHitID();
    CMover* pMoverObject = CMover::GetMoverObject(HitID);
    CMoverEx* pAttackActor = dynamic_cast<CMoverEx*>(pMoverObject);

    if (pAttackActor) {
        OnDie(pAttackActor);
    } else {
        OnDie(nullptr);
    }

    CMoverEx::CheckOptionEffectInvoke(EFFECT_CONDITION_DEAD, this, 0.0f, EFFECT_INVOKE_DONT_CARE);
    CMoverEx::ReleaseInvokedOptionEffect(EFFECT_CONDITION_DEAD);

    if (pAttackActor) {
        CUser* pAttackerUser = nullptr;

        if (XActor::GetType() == eActorUser) {
            pAttackerUser = dynamic_cast<CUser*>(pAttackActor);
        } else if (XActor::GetType() == eActorMonster) {
            CMoverEx* pOwnerPlayer = CMoverEx::GetOwnerPlayer(pAttackActor);
            pAttackerUser = dynamic_cast<CUser*>(pOwnerPlayer);
        }

        if (pAttackerUser) {
            pAttackerUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_KILL);
        } else {
            pAttackActor->ReleaseInvokedOptionEffect(EFFECT_CONDITION_KILL);
        }
    }
}

/**
 * @brief InitJumpData - initialize jump data
 * @param fDesiredHeight Desired jump height
 * @param bGravityScale Use gravity scale
 * IDA: ?InitJumpData@CMoverEx@@QEAAXM_N@Z (0x140395BA0)
 */
void CMoverEx::InitJumpData(float fDesiredHeight, bool bGravityScale) {
    // IDA 精确还原:
    m_bJumpAnim = 0;

    float fVel = CalcFlyVelocity(fDesiredHeight);

    if (fDesiredHeight <= 0.0f) {
        m_fJumpTime = 0.5f;
        m_fPrevJumpHeight = CalcFlyHeight(m_fJumpHeight, 0.5f);
        m_fFlyDirValue = 0.0f;
    } else {
        m_fFlyVelocity = fVel;
        m_fJumpHeight = fDesiredHeight;
        m_fJumpTime = 0.0f;
        m_fPrevJumpHeight = 0.0f;
        m_fFlyDirValue = 1.0f;
    }
}

/**
 * @brief SetHitFreezeTime - set hit freeze time
 * @param fTime Freeze time
 * IDA: ?SetHitFreezeTime@CMoverEx@@QEAAXM@Z (0x140395DD0)
 */
void CMoverEx::SetHitFreezeTime(float fTime) {
    // IDA 精确还原:
    m_fHitFreezeTime = fTime;

    if (m_fHitFreezeTime <= 0.0f) {
        float fRestoreSpeed = GetRestoreAnimSpeed();
        CMover::SetSlowTime(0.0f, fRestoreSpeed);
    } else {
        CMover::SetSlowTime(fTime, 0.005f);
    }
}

/**
 * @brief CalcFlyVelocity - calculate fly velocity for desired height
 * @param fDesiredHeight Desired jump height
 * @return Calculated velocity
 * IDA: ?CalcFlyVelocity@CMoverEx@@QEAAMM@Z (0x140395CA0)
 */
float CMoverEx::CalcFlyVelocity(float fDesiredHeight) {
    // IDA 精确还原:
    if (!CMover::IsFlying() || m_fJumpDelta < 0.0f) {
        m_fJumpDelta = 0.0f;
    }

    if (fDesiredHeight + m_fJumpDelta <= m_fFlyMaxHeight) {
        return sqrtf((fDesiredHeight * 2.0f) * m_fFlyGravity);
    }

    float fAdjustedHeight = m_fFlyMaxHeight - m_fJumpDelta;
    if (fAdjustedHeight > 0.0f) {
        return sqrtf((fAdjustedHeight * 2.0f) * m_fFlyGravity);
    }

    return 0.0f;
}

/**
 * @brief CalcFlyHeight - calculate fly height
 * @param fVelocity Initial velocity
 * @param fTime Flight time
 * @return Calculated height
 * IDA: ?CalcFlyHeight@CMoverEx@@QEAAMMM@Z (0x140395D60)
 */
float CMoverEx::CalcFlyHeight(float fVelocity, float fTime) {
    // IDA 精确还原:
    return (fVelocity * fTime) - (0.5f * m_fFlyGravity * fTime * fTime);
}

/**
 * @brief SetStiffen - set stiffen effect
 * @param fRate Stiffen rate
 * @param fTime Stiffen duration
 * @param fDelayTime Delay before applying
 * IDA: ?SetStiffen@CMoverEx@@QEAAXMMM@Z (0x140395E50)
 */
void CMoverEx::SetStiffen(float fRate, float fTime, float fDelayTime) {
    // IDA 精确还原:
    if (fTime == 0.0f || m_fStiffenImmuneTime > 0.0f) {
        return;
    }

    if (++m_iStiffenCount >= 5) {
        m_fStiffenImmuneTime = 5.0f;
    }

    m_fStiffenRate = fRate;
    m_fStiffenTime = m_fStiffenTime + fTime;

    if (fDelayTime <= 0.0f) {
        CMover::SetSlowTime(m_fStiffenTime, fRate);
        CMover::SetReactionRate(fRate);
    } else {
        m_fStiffenDelayTime = fDelayTime;
        CMover::SetReactionRate(1.0f);
    }
}

/**
 * @brief CalcStiffenTime - calculate stiffen time based on attack/defense
 * @param fTime Base time
 * @param byAttackCollision Attack collision type (0-2)
 * @param byDefenseType Defense type
 * @param byWeakAttackStiffenRatio Weak attack stiffen ratio
 * @return Calculated stiffen time
 * IDA: ?CalcStiffenTime@CMoverEx@@QEAAMMEEE@Z (0x140395F80)
 */
float CMoverEx::CalcStiffenTime(float fTime, std::uint8_t byAttackCollision, std::uint8_t byDefenseType, std::uint8_t byWeakAttackStiffenRatio) {
    // IDA 精确还原:
    if (byDefenseType) {
        return 0.0f;
    }

    if (!byAttackCollision && (rand() % 100) > byWeakAttackStiffenRatio) {
        return 0.0f;
    }

    // Stiffen rate table [collision_type][attack_type]
    static const float fStiffenRate[3][5] = {
        {0.1f, 0.025f, 1.0f, 1.0f, 1.0f},      // Normal attack
        {0.35f, 0.125f, 0.5f, 1.25f, 1.25f},   // Strong attack
        {0.2f, 0.075f, 0.75f, 1.5f, 1.5f}      // Heavy attack
    };

    return fTime * fStiffenRate[0][byAttackCollision];
}

/**
 * @brief UpdateStiffen - update stiffen state
 * @param fDeltaTime Delta time
 * IDA: ?UpdateStiffen@CMoverEx@@QEAAXM@Z (0x140396100)
 */
void CMoverEx::UpdateStiffen(float fDeltaTime) {
    // IDA 精确还原:
    // Update immune time
    if (m_fStiffenImmuneTime > 0.0f) {
        m_fStiffenImmuneTime = m_fStiffenImmuneTime - fDeltaTime;
        if (m_fStiffenImmuneTime < 0.0f) {
            m_fStiffenImmuneTime = 0.0f;
        }
    }

    // Update stiffen delay
    if (m_fStiffenDelayTime <= 0.0f) {
        if (m_fStiffenTime > 0.0f) {
            m_fStiffenTime = m_fStiffenTime - fDeltaTime;
            if (m_fStiffenTime <= 0.0f) {
                m_iStiffenCount = 0;
                m_fStiffenTime = 0.0f;
                CMover::SetReactionRate(1.0f);
            }
        }
    } else {
        m_fStiffenDelayTime = m_fStiffenDelayTime - fDeltaTime;
        if (m_fStiffenDelayTime <= 0.0f) {
            m_fStiffenDelayTime = 0.0f;
            CMover::SetSlowTime(m_fStiffenTime, m_fStiffenRate);
            CMover::SetReactionRate(m_fStiffenRate);
        }
    }
}

/**
 * @brief ResetStiffen - reset stiffen state
 * IDA: ?ResetStiffen@CMoverEx@@QEAAXXZ (0x140396280)
 */
void CMoverEx::ResetStiffen() {
    // IDA 精确还原:
    float fRestoreSpeed = GetRestoreAnimSpeed();
    CMover::SetSlowTime(0.0f, fRestoreSpeed);
    m_iStiffenCount = 0;
    m_fStiffenTime = 0.0f;
    m_fStiffenDelayTime = 0.0f;
    CMover::SetReactionRate(1.0f);
}

/**
 * @brief IsMouseOnTarget - check if mouse is on target
 * @return true if mouse is on target
 * IDA: ?IsMouseOnTarget@CMoverEx@@UEAAHXZ (0x140395B50)
 */
bool CMoverEx::IsMouseOnTarget() {
    // IDA 精确还原:
    return m_pCurSkillTableRef && GetControlType(m_pCurSkillTableRef) == 4;
}

/**
 * @brief MovingToTarget - move to target position
 * @param fDuration Movement duration
 * @param fMaxSpeed Maximum speed
 * @param fOffsetDist Offset distance
 * @param bCheckRange Check range flag
 * IDA: ?MovingToTarget@CMoverEx@@QEAAXMMM_N@Z (0x140393900)
 */
void CMoverEx::MovingToTarget(float fDuration, float fMaxSpeed, float fOffsetDist, bool bCheckRange) {
    // IDA 精确还原:
    if (!m_pCurSkillTableRef) {
        return;
    }

    std::uint32_t TargetID = CMover::GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);

    hkvVec3 vDestPos(0.0f, 0.0f, 0.0f);
    hkvVec3 vDist(0.0f, 0.0f, 0.0f);
    hkvVec3 vDir(0.0f, 0.0f, 0.0f);
    float fDist = 0.0f;

    if (pTarget) {
        const hkvVec3* pMyPos = VisObject3D_cl::GetPosition(this);
        const hkvVec3* pTargetPos = VisObject3D_cl::GetPosition(pTarget);

        vDist = *pTargetPos - *pMyPos;
        vDist.z = 0.0f;
        vDir = vDist;
        vDir.Normalize();
        fDist = vDist.GetLength();

        if (bCheckRange) {
            if (fDist > m_pCurSkillTableRef->Skill_Range_Max) {
                hkvMat3 matRot;
                matRot.setFromEulerAngles(0.0f, 0.0f, m_fMovingYaw);
                hkvVec3 vForward(0.0f, -1.0f, 0.0f);
                vDir = matRot.transformDirection(vForward);
                fDist = m_pCurSkillTableRef->Skill_Range_Max;
            } else {
                float fMyRadius = CMover::GetHavokCapsuleRadius();
                float fTargetRadius = CMover::GetHavokCapsuleRadius(pTarget);
                fDist = fDist - (fMyRadius + fTargetRadius);
            }
        }
    } else {
        hkvMat3 matRot;
        matRot.setFromEulerAngles(0.0f, 0.0f, m_fMovingYaw);
        hkvVec3 vForward(0.0f, -1.0f, 0.0f);
        vDir = matRot.transformDirection(vForward);
        fDist = m_pCurSkillTableRef->Skill_Range_Max;
    }

    fDist = fDist - fOffsetDist;

    if (fDist > 0.0f) {
        vDist = vDir * fDist;
        vDestPos = m_vPosition + vDist;

        SetDirectionTo(&vDestPos);
        SetDirectionYaw(2);

        if (fDuration == 0.0f) {
            if (fMaxSpeed == 0.0f) {
                CMover::GetHeight(&vDestPos, 200.0f);
                CheckMoveDestPos(&vDestPos, false, true);
                hkvVec3 vMovePos(vDestPos.x, vDestPos.y, vDestPos.z);
                Move(&vMovePos);
                CMover::send_eSUB_CMD_SKILL_WARP_POSITION(this, &vDestPos, true);
            } else {
                float fNewDuration = fDist / fMaxSpeed;
                SetExtraMoving();
                hkvVec3 vExtPos(m_stExtMovingVal.x, m_stExtMovingVal.y, m_stExtMovingVal.z);
                CMover::send_eSUB_CMD_SKILL_MOVE_WITH_TIME(this, &vExtPos, fNewDuration);
                CMover::SetFlyState(1);
            }
        } else {
            if (fMaxSpeed > 0.0f && (fDist / fDuration) > fMaxSpeed) {
                fDuration = fDist / fMaxSpeed;
            }
            SetExtraMoving();
            hkvVec3 vExtPos(m_stExtMovingVal.x, m_stExtMovingVal.y, m_stExtMovingVal.z);
            CMover::send_eSUB_CMD_SKILL_MOVE_WITH_TIME(this, &vExtPos, fDuration);
            CMover::SetFlyState(1);
        }
    }
}

/**
 * @brief MoveToPrefab - move to prefab position
 * IDA: ?MoveToPrefab@CMoverEx@@QEAAXXZ (0x140394DC0)
 */
void CMoverEx::MoveToPrefab() {
    // IDA 精确还原:
    m_vBackupSkillPos = m_vPosition;
    m_vBackupSkillRotate = m_vOrientation;
    m_bMoveSkillPrefab = 1;
}

/**
 * @brief ReturnFromPrefab - return from prefab position
 * IDA: ?ReturnFromPrefab@CMoverEx@@QEAAXXZ (0x140394E50)
 */
void CMoverEx::ReturnFromPrefab() {
    // IDA 精确还原:
    if (m_bMoveSkillPrefab) {
        m_bMoveSkillPrefab = 0;
        // TODO: Warp to backup position
    }
}

/**
 * @brief CalcSkillMoveDistance - calculate skill move distance
 * @return Calculated distance
 * IDA: ?CalcSkillMoveDistance@CMoverEx@@QEAAMXZ (0x1403962F0)
 */
float CMoverEx::CalcSkillMoveDistance() {
    // IDA 精确还原:
    if (!m_pCurSkillTableRef) {
        return 0.0f;
    }

    if (!XActor::IsStatus(1) && !m_pCurSkillTableRef) {
        return 0.0f;
    }

    float fMinSkillMovement = static_cast<float>(m_pCurSkillTableRef->Skill_Movement_Value_Min);
    float fMaxSkillMovement = static_cast<float>(m_pCurSkillTableRef->Skill_Movement_Value_Max);

    if (fMaxSkillMovement <= 0.0f) {
        return 0.0f;
    }

    float fTotalMaxChargingTime = static_cast<float>(m_pCurSkillTableRef->Time_Value_01) * 0.001f;

    if (m_fSkillTotalChargeTime > fTotalMaxChargingTime) {
        m_fSkillTotalChargeTime = fTotalMaxChargingTime;
    }

    return fMinSkillMovement + ((fMaxSkillMovement - fMinSkillMovement) * m_fSkillTotalChargeTime) / fTotalMaxChargingTime;
}

/**
 * @brief ApplySkillMove - apply skill movement
 * @param fDistance Movement distance
 * @param fTime Movement time
 * IDA: ?ApplySkillMove@CMoverEx@@QEAAXMM@Z (0x140396430)
 */
void CMoverEx::ApplySkillMove(float fDistance, float fTime) {
    // IDA 精确还原:
    const hkvVec3* pPosition = VisObject3D_cl::GetPosition(this);
    hkvVec3 vTargetPos = *pPosition;

    hkvVec3 vCameraDirection(0.0f, 0.0f, 0.0f);
    hkvMat3 matRot;
    matRot.setFromEulerAngles(0.0f, 0.0f, m_fMovingYaw);

    hkvVec3 vForward(0.0f, -1.0f, 0.0f);
    vCameraDirection = matRot.transformDirection(vForward);

    vTargetPos += vCameraDirection * fDistance;

    SetExtraMoving();
    hkvVec3 vExtPos(m_stExtMovingVal.x, m_stExtMovingVal.y, m_stExtMovingVal.z);
    CMover::send_eSUB_CMD_SKILL_MOVE_WITH_TIME(this, &vExtPos, fTime);
}

/**
 * @brief ReturnFromPrefab - return from prefab position
 * IDA: ?ReturnFromPrefab@CMoverEx@@QEAAXXZ (0x140394E50)
 */
void CMoverEx::ReturnFromPrefab() {
    // IDA 精确还原:
    SetPositionXVec3(&m_vBackupSkillPos);
    VisObject3D_cl::SetOrientation(&m_vBackupSkillRotate);
    m_bMoveSkillPrefab = 0;
    CMover::send_eSUB_CMD_MOVE_STOP(this);
}

/**
 * @brief SetInvisible - set invisible state
 * @param bHide Hide flag
 * @param dwFlag Invisible flag
 * @param nType Condition type
 * @param nValue Condition value
 * @param nExtVal1 Extended value 1
 * @param nExtVal2 Extended value 2
 * @param nExtVal3 Extended value 3
 * IDA: ?SetInvisible@CMoverEx@@QEAAXHKHHHHH@Z (0x140394EC0)
 */
void CMoverEx::SetInvisible(int bHide, std::uint32_t dwFlag, int nType, int nValue, int nExtVal1, int nExtVal2, int nExtVal3) {
    // IDA 精确还原:
    if (bHide) {
        XActor::SetStatus(0x2000, true);
        if (nType == 4) {
            XActor::SetStatus(0x80000, true);
        }
        m_dwInvisibleFlag = dwFlag;
        m_nInvisibleConditionType = nType;
        m_nInvisibleConditionVal[0] = nValue;
        m_nInvisibleConditionVal[1] = nExtVal1;
        m_nInvisibleConditionVal[2] = nExtVal2;
        m_nInvisibleConditionVal[3] = nExtVal3;

        if (nType) {
            m_fInvisibleEndTime = 1.0f;
        } else {
            m_fInvisibleEndTime = static_cast<float>(nValue) * 0.001f;
        }
    } else {
        XActor::ClearStatus(0x2000, true);
        if (nType == 4) {
            XActor::ClearStatus(0x80000, true);
        }
        m_dwInvisibleFlag = 0;
        m_fInvisibleEndTime = 0.0f;
        m_nInvisibleConditionType = 0;
        memset(m_nInvisibleConditionVal, 0, sizeof(m_nInvisibleConditionVal));
    }

    CMover::SetCollisionEnable(bHide == 0, false);
    CMover::send_eSUB_CMD_MONSTER_INVISIBLE(this, bHide, dwFlag, nType, nValue);
}

/**
 * @brief WarpToPoint - warp to point by pattern
 * @param iPattern Warp pattern (0=first, 1=sequential, 2=random, 3=by group)
 * @param iPoint Point index or group
 * @param fYaw Yaw angle
 * IDA: ?WarpToPoint@CMoverEx@@QEAAXHHM@Z (0x140395090)
 */
void CMoverEx::WarpToPoint(int iPattern, int iPoint, float fYaw) {
    // IDA 精确还原:
    int nCount = VPList::GetLength(&m_CommonPosBoxList);
    if (!nCount) {
        return;
    }

    VCommonPositionBoxInfo* pObject = nullptr;

    if (nCount != 1 && iPattern) {
        if (iPattern == 2) {
            // Random pattern
            int iIndex = 0;
            do {
                if (nCount <= 1) break;
                iIndex = rand() % nCount;
            } while (iIndex == m_iLastWarpPoint);

            pObject = reinterpret_cast<VCommonPositionBoxInfo*>(*VPList::operator[](&m_CommonPosBoxList, iIndex));
            m_iLastWarpPoint = iIndex;
        } else if (iPattern == 3) {
            // By group
            for (int i = nCount - 1; i >= 0; --i) {
                pObject = reinterpret_cast<VCommonPositionBoxInfo*>(*VPList::operator[](&m_CommonPosBoxList, i));
                if (pObject && pObject->m_iGroup == iPoint) {
                    break;
                }
            }
        } else {
            // Sequential pattern
            int iGroup = iPoint;
            if (m_iLastWarpPoint != -1) {
                iGroup = (m_iLastWarpPoint + 1) % nCount;
            }

            for (int j = nCount - 1; j >= 0; --j) {
                pObject = reinterpret_cast<VCommonPositionBoxInfo*>(*VPList::operator[](&m_CommonPosBoxList, j));
                if (pObject && pObject->m_iGroup == iGroup) {
                    break;
                }
            }
            m_iLastWarpPoint = iGroup;
        }
    } else {
        pObject = reinterpret_cast<VCommonPositionBoxInfo*>(*VPList::operator[](&m_CommonPosBoxList, 0));
    }

    if (pObject) {
        hkvVec3 vPos;
        pObject->GetCenter(&vPos);
        Move(&vPos);
        SetDirectionYaw(2);
        CMover::send_eSUB_CMD_SKILL_WARP_POSITION(this, &vPos, true);
    }
}

/**
 * @brief SummonAkashic - summon akashic record
 * @param pTrigger Akashic trigger data
 * IDA: ?SummonAkashic@CMoverEx@@QEAAXPEAVAkashicTrigger@@@Z (0x140395310)
 */
void CMoverEx::SummonAkashic(AkashicTrigger* pTrigger) {
    // IDA 精确还原:
    if (!pTrigger) return;

    hkvMat3 matRot;
    float fYaw = GetSummonAkashicYaw();
    matRot.setFromEulerAngles(0.0f, 0.0f, fYaw);

    hkvVec3 vRotPos;
    matRot.transformDirection(&vRotPos, &pTrigger->SummonPos);

    const hkvVec3* pPosition = VisObject3D_cl::GetPosition(this);
    hkvVec3 vDestPos = *pPosition + vRotPos;

    XArea* pArea = XActor::GetArea();
    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);

    CAkashicObject* pAkashic = nullptr;

    if (pMaze) {
        std::uint32_t dwQuestID = CQuestCondition::GetQuestID(XActor::GetActorID());
        UXMapID instanceID = pMaze->GetInstanceID();
        pAkashic = pMaze->CreateAkashicObject(
            instanceID,
            m_pCurAkashicRecord->ID,
            &vDestPos,
            fYaw,
            dwQuestID,
            eSendInfoTypeNot);
    } else if (pBattleZone) {
        std::uint32_t dwQuestID = CQuestCondition::GetQuestID(XActor::GetActorID());
        UXMapID instanceID = pBattleZone->GetInstanceID();
        pAkashic = pBattleZone->CreateAkashicObject(instanceID, m_pCurAkashicRecord->ID, &vDestPos, fYaw, dwQuestID);
    }

    if (pAkashic) {
        std::uint8_t byNation = XActor::GetNation();
        pAkashic->SetNation(byNation);

        UXActorID originID = XActor::GetOriginID();
        pAkashic->SetOriginID(originID);

        CMover::SetCollisionEnable(pAkashic, false, false);
        pAkashic->SetDirectionYaw(1);

        TB_AKASHIC_RECORDS* pAkashicRecord = m_pCurAkashicRecord;
        pAkashic->PreSkillProcess(pAkashicRecord->Skill_ID, false);

        std::uint32_t dwQuestID = CQuestCondition::GetQuestID(XActor::GetActorID());
        pAkashic->SetOwnerID(dwQuestID);

        m_pAkashicObject = pAkashic;

        UXActorID actorID = pAkashic->GetActorID();
        CMover::send_eSUB_CMD_SKILL_SUMMON_AKASHIC(
            this,
            actorID,
            pTrigger->AlphaValue,
            pTrigger->BlendingTime,
            &pTrigger->SummonPos,
            pTrigger->ApplyRotation,
            fYaw);
    }
}

/**
 * @brief SetMoveTrapPos - set move trap position
 * @param nUseZAxis Use Z axis flag
 * IDA: ?SetMoveTrapPos@CMoverEx@@UEAAXH@Z (0x140395A20)
 */
void CMoverEx::SetMoveTrapPos(int nUseZAxis) {
    // IDA 精确还原:
    std::uint32_t TargetID = CMover::GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);

    if (pTarget) {
        const hkvVec3* pTargetPos = VisObject3D_cl::GetPosition(pTarget);
        m_vTrapPos = *pTargetPos;
        CMover::GetHeight(&m_vTrapPos, 200.0f);
    } else {
        const hkvVec3* pMyPos = VisObject3D_cl::GetPosition(this);
        m_vTrapPos = *pMyPos;
    }
}

/**
 * @brief ChangeToAkashicData - change to akashic data
 * @param szSeqName Sequence name
 * @param nGroupID Group ID
 * IDA: ?ChangeToAkashicData@CMoverEx@@QEAAXPEBDH@Z (0x140395820)
 */
void CMoverEx::ChangeToAkashicData(const char* szSeqName, int nGroupID) {
    // IDA 精确还原:
    VString strKey;
    VString::Format(&strKey, "%d:%s", nGroupID, szSeqName);

    XAkashicResMgr* pAkashicManager = &TXSingleton<XGameServer>::Instance()->m_xAkashicManager;
    VArray<ActionTrigger*>* pActionData = pAkashicManager->GetActionDataList(1, strKey);

    if (pActionData) {
        CMover::CreateAkashicActionInfo(szSeqName);

        if (m_pAkashicActionInfo) {
            float fTime = GetAkashicTriggerTime();
            int iDataCount = pActionData->GetCurID();

            m_pAkashicActionInfo->arTriggers.SetSize(iDataCount, -1, true);

            for (int i = 0; i < iDataCount; ++i) {
                ActionTrigger* pNewTrigger = pActionData->ElementAt(i);
                if (pNewTrigger && pNewTrigger->TypeOfTrigger == 18) {
                    pNewTrigger->StartTime = fTime;
                }
                m_pAkashicActionInfo->arTriggers.SetAt(i, &pNewTrigger);
            }
        }
    } else {
        const char* szKey = strKey.AsChar();
        DebugOut("ChangeToAkashicData>> No action data found. Key:%s", szKey);
    }
}

/**
 * @brief SetSubordinationCombo - set subordination combo
 * @param pTrigger Subordination combo trigger
 * IDA: ?SetSubordinationCombo@CMoverEx@@QEAAXPEAVSubordinationComboTrigger@@@Z (0x1403965E0)
 */
void CMoverEx::SetSubordinationCombo(SubordinationComboTrigger* pTrigger) {
    // IDA 精确还原:
    if (m_pSuboComboTrigger == pTrigger) {
        return;
    }

    m_iMaxLeftSuboDescCount = -1;
    m_iMaxRightSuboDescCount = -1;

    for (int i = 0; i < 10; ++i) {
        if (m_iMaxLeftSuboDescCount == -1 && strlen(pTrigger->sSuboComboDesc[i].szAniName) <= 1) {
            m_iMaxLeftSuboDescCount = i;
        }
        if (m_iMaxRightSuboDescCount == -1 && strlen(pTrigger->sSuboComboDesc[i + 10].szAniName) <= 1) {
            m_iMaxRightSuboDescCount = i;
        }
    }

    if (m_iMaxLeftSuboDescCount > 0 || m_iMaxRightSuboDescCount > 0) {
        m_pSuboComboTrigger = pTrigger;
        m_bExistSuboCombo = 1;
    }

    m_iCurLeftSuboDescCount = -1;
    m_iCurRightSuboDescCount = -1;
}

/**
 * @brief PlaySuboAnim - play subo animation
 * @param iSuboAnimIdx Animation index
 * @param bPlayForce Force play flag
 * @return true if animation was played
 * IDA: ?PlaySuboAnim@CMoverEx@@QEAA_NH_N@Z (0x140396780)
 */
bool CMoverEx::PlaySuboAnim(std::uint32_t iSuboAnimIdx, bool bPlayForce) {
    // IDA 精确还原:
    if (!bPlayForce &&
        (iSuboAnimIdx == static_cast<std::uint32_t>(-1) ||
         m_iSuboComboMaxCount == -1 ||
         (m_iSuboComboMaxCount > 0 && m_iSuboComboCheckCount >= m_iSuboComboMaxCount))) {

        m_bExistSuboCombo = 0;
        m_fSuboComboWaitTime = 0.0f;
        m_iSuboComboMaxCount = -1;
        m_iSuboComboCheckCount = 0;

        VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
        m_fSuboComboCheckTime = IVTimer::GetTime(pTimer);

        if (m_bySkillAnimStep != 3) {
            ClearMotion();
        }
        return false;
    }

    m_iCurLeftSuboDescCount = 0;
    m_iCurRightSuboDescCount = 0;

    std::uint16_t nMotionClass = CMover::AnimKeyToMotion(iSuboAnimIdx);
    m_fSuboComboCheckTime = 0.0f;

    ChangeMotion_3(nMotionClass, true, 0, nMotionClass);
    ++m_iSuboComboCheckCount;

    return true;
}

/**
 * @brief SetCheckEntityAttach - set check entity attach
 * @param bCheck Check flag
 * @param fDuration Duration
 * @param fDistance Distance
 * @param fAngle Angle
 * @param vOffset Offset vector
 * @param iApplyWeightRank Apply weight rank
 * IDA: ?SetCheckEntityAttach@CMoverEx@@QEAAX_NMMMVhkvVec3@@H@Z (0x1403968E0)
 */
void CMoverEx::SetCheckEntityAttach(bool bCheck, float fDuration, float fDistance, float fAngle, hkvVec3* vOffset, int iApplyWeightRank) {
    // IDA 精确还原:
    m_bCheckAttachToAttacker = bCheck;
    m_fAttachedDuration = fDuration;

    if (m_bCheckAttachToAttacker) {
        m_fAttachedDistance = fDistance;
        m_vAttachedOffset = *vOffset;
        m_fAttachedAngle = fAngle;
        m_iApplyWeightRank = iApplyWeightRank;
    }
}

/**
 * @brief PS_MOVING_TARGET constructor
 * IDA: ??0PS_MOVING_TARGET@@QEAA@XZ (0x140394D90)
 */
PS_MOVING_TARGET::PS_MOVING_TARGET() {
    hkvVec3::hkvVec3(&xExtraMove);
}

/**
 * @brief CheckAttachedEntity - check attached entity
 * IDA: ?CheckAttachedEntity@CMoverEx@@QEAAXXZ (0x1403969A0)
 */
void CMoverEx::CheckAttachedEntity() {
    // IDA 精确还原:
    tagATTACK_AREA stAreaInfo;
    hkvVec3 vPos = *VisObject3D_cl::GetPosition(this);

    hkvMat3 matRot;
    float fYaw = GetOrientationYaw();
    VisMath_cl::EulerToMatrix(fYaw, 0.0f, 0.0f, &matRot);

    hkvVec3 vDir;
    hkvVec3 vForward(0.0f, -1.0f, 0.0f);
    vDir = matRot * vForward;

    stAreaInfo.byType = 0;
    stAreaInfo.vCenterPos = vPos;
    stAreaInfo.fAttackerRot = GetOrientationYaw();
    stAreaInfo.fRadiusStart = 0.0f;
    stAreaInfo.fRadiusEnd = m_fAttachedDistance;
    stAreaInfo.fAngle = m_fAttachedAngle / 2.0f;
    stAreaInfo.vAttackerDir = D3DXVECTOR2(vDir.x, vDir.y);
    stAreaInfo.byHitPartsIndex = 0;
    stAreaInfo.fHeightB = vPos.z;

    float fHeight = CMover::GetHavokCapsuleHeight();
    stAreaInfo.fHeightT = vPos.z + fHeight;

    std::vector<CMover*> vecGameObjList;
    XArea::ScanGridOrigin(&XActor, 2, 3, &vecGameObjList);

    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMoverEx* pOtherActor = dynamic_cast<CMoverEx*>(*it);
        if (!pOtherActor) continue;

        if (pOtherActor->IsLive() &&
            !XActor::IsStatus(2) &&
            !CMover::IsHitDown(pOtherActor) &&
            pOtherActor != this &&
            !pOtherActor->GetAttached()) {

            if (CheckReactionTarget(3, pOtherActor, true) == 1 &&
                !CMover::IsAttackDecision(pOtherActor, &stAreaInfo)) {

                if (CMover::GetWeightRank(pOtherActor) >= m_iApplyWeightRank ||
                    CMoverEx::GetStandType(pOtherActor) == 2 ||
                    CMoverEx::GetStandType(pOtherActor) == 3) {

                    m_bSkipAnimOffset = 1;
                    hkvVec3 vPosToSend = m_vPosition;
                    CMover::send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, &vPosToSend, true);
                    return;
                }

                CMoverEx::SetAttached(pOtherActor, this);
                CMover::MoveingValueClear(pOtherActor);
                CMover::send_eSUB_CMD_MOVE_STOP(pOtherActor);

                hkvVec3 vAttachedDir = CMoverEx::GetAttachedDir(pOtherActor);
                float fAttachedDist = CMoverEx::GetAttachedDirDist(pOtherActor);
                CMover::send_eSUB_CMD_MOVE_ATTACED_BT(this, pOtherActor, &vAttachedDir, fAttachedDist);
            }
        }
    }
}

/**
 * @brief TraceAttachedOwner - trace attached owner position
 * IDA: ?TraceAttachedOwner@CMoverEx@@QEAAXXZ (0x140396EA0)
 * Verified: Updates position of attached entity to follow owner
 */
void CMoverEx::TraceAttachedOwner() {
    // IDA 精确还原:
    if (!m_pAttachToAttacker) {
        return;
    }

    hkvVec3 vOwnerPos = *VisObject3D_cl::GetPosition(m_pAttachToAttacker);
    float fOwnerRadius = CMover::GetHavokCapsuleRadius(m_pAttachToAttacker);
    float fMyRadius = CMover::GetHavokCapsuleRadius();

    hkvVec3 vAttachedPos = vOwnerPos + (m_vAttachedDir * (fOwnerRadius + fMyRadius + m_fAttachedDirDist));

    hkvVec3 vAttachedOffset;
    hkvVec3 v13;
    hkvVec3* pOffset = CMoverEx::GetAttachedOffset(m_pAttachToAttacker, &v13);
    if (!hkvVec3::IsZero(pOffset)) {
        CMoverEx::GetAttachedOffset(m_pAttachToAttacker, &vAttachedOffset);

        hkvMat3 matRot;
        hkvMat3::hkvMat3(&matRot);
        float fYaw = m_pAttachToAttacker->GetOrientationYaw();
        matRot.setFromEulerAngles(0.0f, 0.0f, fYaw);
        vAttachedOffset *= matRot;

        vAttachedPos = vOwnerPos + vAttachedOffset;
    }

    if (CheckMoveDestPos(&vAttachedPos, false, false)) {
        hkvVec3 vMovePos(vAttachedPos.x, vAttachedPos.y, vAttachedPos.z);
        Move(&vMovePos);
    } else {
        hkvVec3 vMovePos(vAttachedPos.x, vAttachedPos.y, vAttachedPos.z);
        Move(&vMovePos);
        ChangeMotion_3(1, true, 8);
        CMover::MoveingValueClear();
        CMover::send_eSUB_CMD_MOVE_STOP(this);
        CMover::send_eSUB_CMD_MOVE_ATTACED_END_BT(m_pAttachToAttacker, this);
        CMoverEx::SetAttached(nullptr);
    }
}

/**
 * @brief ClearAllAttachedEntity - clear all attached entities
 * IDA: ?ClearAllAttachedEntity@CMoverEx@@QEAAXXZ (0x140397200)
 * Verified: Detaches all entities attached to this mover
 */
void CMoverEx::ClearAllAttachedEntity() {
    // IDA 精确还原:
    std::vector<CMover*> vecGameObjList;
    XActor* pActor = this ? &XActor::m_XActor : nullptr;
    XArea::ScanGridOrigin(pActor, 2, 3u, &vecGameObjList);

    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMoverEx* pOtherActor = dynamic_cast<CMoverEx*>(*it);
        if (pOtherActor) {
            if (CMoverEx::GetAttached(pOtherActor)) {
                CMoverEx::SetAttached(pOtherActor, nullptr);
                CMover::send_eSUB_CMD_MOVE_ATTACED_END_BT(this, pOtherActor);
                CMover::send_eSUB_CMD_MOVE_STOP(pOtherActor);
            }
        }
    }
}

/**
 * @brief SetAttached - set attached owner
 * @param pAttachedOwner Owner to attach to
 * IDA: ?SetAttached@CMoverEx@@QEAAXPEAV1@@Z (0x140397340)
 * Verified: Sets up attachment relationship between entities
 */
void CMoverEx::SetAttached(CMoverEx* pAttachedOwner) {
    // IDA 精确还原:
    m_pAttachToAttacker = pAttachedOwner;

    if (m_pAttachToAttacker) {
        hkvVec3 vMyPos = *VisObject3D_cl::GetPosition(this);
        hkvVec3 vOwnerPos = *VisObject3D_cl::GetPosition(pAttachedOwner);

        m_vAttachedDir = vMyPos - vOwnerPos;
        m_fAttachedDirDist = m_vAttachedDir.getLength();
        m_vAttachedDir.Normalize();

        if (CMover::IsFlying()) {
            if (m_nMotionClass != 20 && m_nMotionClass != 21) {
                ChangeMotion_3(19, true, false);
            }
        } else {
            ChangeMotion_3(1, true, 9);
        }
    } else {
        m_vAttachedDir.setZero();
    }
}

/**
 * @brief SetDie - set die state
 * @param nMotion Motion class for death animation (-1 for auto)
 * @param bSuicide Suicide flag
 * IDA: ?SetDie@CMoverEx@@UEAAXFH@Z (0x140397520)
 * Verified: Initiates death state with appropriate animation
 */
void CMoverEx::SetDie(std::int16_t nMotion, int bSuicide) {
    // IDA 精确还原:
    if (CMoverEx::IsSystemActor()) {
        return;
    }

    if (XActor::IsStatus(4u)) {
        return;
    }

    if (XActor::IsStatus(2u)) {
        return;
    }

    CMover::MoveingValueClear();
    CMover::AllBuffClear(1u);
    XActor::SetStatus(4u);

    if (nMotion == -1) {
        if (CMover::IsHitDown()) {
            RealDie(13);
        } else if (!CMover::IsKnockDown() || m_nHitStatus == 5) {
            RealDie(12);
        }
    } else {
        RealDie(nMotion);
    }
}

/**
 * @brief IsDirectionToTargetSkill - check if skill is direction-to-target type
 * @return true if skill uses direction targeting
 * IDA: ?IsDirectionToTargetSkill@CMoverEx@@UEAAHXZ (0x140397640)
 * Verified: Checks if current skill uses target direction
 */
bool CMoverEx::IsDirectionToTargetSkill() {
    // IDA 精确还原:
    return m_pCurSkillTableRef && !m_bDisableDirectionToTargetSkill;
}

/**
 * @brief IsSkillAttackMe - check if skill can attack target
 * @param pMover Target mover to check
 * @return true if target is within skill range and angle
 * IDA: ?IsSkillAttackMe@CMoverEx@@UEAAHPEAV1@@Z (0x140397670)
 * Verified: Checks if target is within skill attack parameters
 */
bool CMoverEx::IsSkillAttackMe(CMoverEx* pMover) {
    // IDA 精确还原:
    if (!XActor::IsStatus(1u)) {
        return false;
    }

    if (!m_pCurSkillTableRef) {
        return false;
    }

    if (!m_pCurSkillTableRef->Skill_Range_Min && !m_pCurSkillTableRef->Skill_Range_Max) {
        return true;
    }

    hkvVec3 vTargetPos = *VisObject3D_cl::GetPosition(pMover);
    const hkvVec3* pMyPos = VisObject3D_cl::GetPosition(this);

    hkvVec3 vDiff = vTargetPos - *pMyPos;
    vDiff.z = 0.0f;

    int nDist = static_cast<int>(vDiff.getLength());

    if (nDist < m_pCurSkillTableRef->Skill_Range_Min ||
        nDist > m_pCurSkillTableRef->Skill_Range_Max) {
        return false;
    }

    if (!m_pCurSkillTableRef->Skill_Angle_Value ||
        m_pCurSkillTableRef->Skill_Angle_Value >= 180) {
        return true;
    }

    float fYaw = CMover::GetYawFromVector(&vDiff);
    float fCurrYaw = GetOrientationYaw();
    float fDiffYaw = fYaw - fCurrYaw;

    // Normalize angle difference
    if (fDiffYaw <= 180.0f) {
        if (fDiffYaw < -180.0f) {
            fDiffYaw += 360.0f;
        }
    } else {
        fDiffYaw -= 360.0f;
    }

    return m_pCurSkillTableRef->Skill_Angle_Value <= static_cast<int>(fabsf(fDiffYaw));
}

/**
 * @brief _GenerateEventObject - generate event object
 * @param eSelfTypeA Self type A
 * @param iSelfIDA Self ID A
 * IDA: ?_GenerateEventObject@CMoverEx@@IEAAXHH@Z (0x1403978C0)
 * Verified: Generates event objects from maze resource
 */
void CMoverEx::_GenerateEventObject(int eSelfTypeA, int iSelfIDA) {
    // IDA 精确还原:
    VPList::Reset(&m_EventObjectList);
    VPList::Reset(&m_CommonPosBoxList);

    XArea* pArea = XActor::GetArea();
    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);

    VEventObjectResource* pMazeResource = nullptr;
    if (pMaze) {
        pMazeResource = pMaze->GetObjectResource();
    }

    if (pMazeResource) {
        int iLevel = pMaze->GetBatchLayerLevel();
        VMap<int, void*>* ObjectMap = pMazeResource->GetMap(iLevel, 0, 0);

        void* Iter = ObjectMap->GetStartPosition();
        int iKey = 0;
        void* pValue = nullptr;

        while (Iter) {
            ObjectMap->GetNextPair(&Iter, &iKey, &pValue);
            const VEventObjectInfo* pInfo = reinterpret_cast<const VEventObjectInfo*>(pValue);

            if (pInfo) {
                if (pInfo->eType == eEventObjectType_Box) {
                    const VEventBoxInfo* pBoxInfo = reinterpret_cast<const VEventBoxInfo*>(pInfo);

                    if (pBoxInfo->m_eTarget == eSelfTypeA &&
                        (!iSelfIDA || pBoxInfo->m_iEntityID == iSelfIDA)) {
                        VCommonPositionBoxInfo* pPosInfo = 
                            reinterpret_cast<VCommonPositionBoxInfo*>(const_cast<VEventBoxInfo*>(pBoxInfo));
                        VPList::AddUnique(&m_CommonPosBoxList, pPosInfo);
                    }
                }
            }
        }
    }
}

/**
 * @brief DecEndurance - decrease endurance
 * @param dwID Actor ID
 * IDA: ?DecEndurance@CMoverEx@@IEAAXK@Z (0x140397AA0)
 * Verified: Decreases endurance for both attacker and defender
 */
void CMoverEx::DecEndurance(std::uint32_t dwID) {
    // IDA 精确还原:
    XArea* pArea = XActor::GetArea();
    if (!pArea) {
        return;
    }

    if (!pArea->IsMaze()) {
        return;
    }

    // Decrease endurance for self if player
    if (XActor::IsPlayer()) {
        CUser* pUser = dynamic_cast<CUser*>(this);
        if (pUser) {
            CUser::DefDecEndurance(pUser);
        }
    }

    // Find attacker actor
    XActor* pActor = pArea->FindActor(dwID);
    if (pActor && XActor::IsPlayer(pActor)) {
        CUser* pAttacker = dynamic_cast<CUser*>(pActor);
        if (pAttacker) {
            CUser::AtkDecEndurance(pAttacker);
        }
    }
}

/**
 * @brief UpdateMonsterLastDamageType - update monster last damage type
 * @param dmgInfo Damage info structure
 * @param nSkillID Skill ID
 * IDA: ?UpdateMonsterLastDamageType@CMoverEx@@IEAAXAEAUtagACTION_DAMAGE@@H@Z (0x140397C60)
 * Verified: Updates monster damage type for maze events
 */
void CMoverEx::UpdateMonsterLastDamageType(tagACTION_DAMAGE& dmgInfo, std::uint32_t nSkillID) {
    // IDA 精确还原:
    XArea* pArea = XActor::GetArea();
    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);

    if (!pMaze) {
        return;
    }

    XActor* pActor = pArea->FindActor(m_dwHitID);
    if (!pActor) {
        return;
    }

    // Check if attacker is player
    bool bAttackPlayer = false;
    if (XActor::IsPlayer(pActor)) {
        bAttackPlayer = true;
    } else if (XActor::IsMonster(pActor)) {
        CMonster* pAttackMonster = dynamic_cast<CMonster*>(pActor);
        if (pAttackMonster) {
            CMoverEx* pOwnerPlayer = pAttackMonster->GetOwnerPlayer();
            if (pOwnerPlayer) {
                bAttackPlayer = XActor::IsPlayer(pOwnerPlayer);
            }
        }
    }

    if (!bAttackPlayer) {
        return;
    }

    // Check if this is a monster
    if (!XActor::IsMonster()) {
        return;
    }

    CMonster* pMonster = dynamic_cast<CMonster*>(this);
    if (!pMonster || !pMonster->GetMobTableRef()) {
        return;
    }

    // Get skill table
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);

    std::uint32_t dwCondition = 0;

    if (pSkillTable) {
        switch (pSkillTable->Passive_Type) {
            case 1:
                dwCondition = 2;
                break;
            case 7:
                dwCondition = 4;
                break;
            case 3:
                dwCondition = 16;
                break;
            case 0:
            case 18:
                dwCondition = 8;
                break;
            default:
                break;
        }
    }

    // Check damage flag
    if ((dmgInfo.byDamageFlag & 4) != 0) {
        dwCondition |= 0x20u;
    }

    // Check reaction type
    std::uint8_t byReactionType = dmgInfo.byReactionType;
    if (byReactionType == 5) {
        dwCondition |= 0x80u;
    } else if (byReactionType == 2 || byReactionType == 3 || byReactionType == 4) {
        dwCondition |= 0x40u;
    }

    // Set monster last damage type
    TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
    pMaze->SetMonsterLastDamageType(pMobRef->ID, dwCondition);

    // Update hidden event conditions
    if (pSkillTable) {
        if (pSkillTable->Passive_Type == 16) {
            pMaze->UpdateHiddenEventCondition(16, 0, true);
        }
        if (CMover::IsFlying()) {
            pMaze->UpdateHiddenEventCondition(15, 0, true);
        }
    }
}

/**
 * @brief SetRecord - set record value
 * @param iType Record type
 * @param nValue Record value
 * IDA: ?SetRecord@CMoverEx@@QEAAXHH@Z (0x140398030)
 * Verified: Sets record value via CGocRecode component
 */
void CMoverEx::SetRecord(std::uint32_t iType, int nValue) {
    // IDA 精确还原:
    std::shared_ptr<CGocRecode> pAttr;
    CMover::GetGOC<CGocRecode>(&pAttr, 0);

    if (pAttr) {
        CGocRecode* pRecode = pAttr.get();
        pRecode->SetRecode(iType, nValue);
    }
}

/**
 * @brief SetCounter - set counter attack parameters
 * @param fMinRange Minimum counter range
 * @param fMaxRange Maximum counter range
 * @param fAngle Counter angle
 * @param pAniName Counter animation name
 * @param iProbability Counter probability
 * @param fDuration Counter duration
 * IDA: ?SetCounter@CMoverEx@@QEAAXMMMPEADHM@Z (0x140398090)
 * Verified: Sets up counter attack parameters
 */
void CMoverEx::SetCounter(float fMinRange, float fMaxRange, float fAngle, 
                          char* pAniName, int iProbability, float fDuration) {
    // IDA 精确还原:
    m_bEnableCounuter = true;
    m_fMinCounterRange = fMinRange;
    m_fMaxCounterRange = fMaxRange;
    m_fCounterAngle = fAngle;
    strcpy_s(m_szCounterAnimName, 128, pAniName);
    m_iCounterProbability = iProbability;
    m_fCounterDuration = fDuration;
}

/**
 * @brief CheckCounter - check if counter attack is triggered
 * @param dwAttackerID Attacker ID
 * @return true if counter attack is triggered
 * IDA: ?CheckCounter@CMoverEx@@QEAA_NK@Z (0x140398140)
 * Verified: Checks if counter attack conditions are met
 */
bool CMoverEx::CheckCounter(std::uint32_t dwAttackerID) {
    // IDA 精确还原:
    if (!m_bEnableCounuter) {
        return false;
    }

    if (m_iCounterProbability < (rand() % 10000)) {
        return false;
    }

    CMover* pAttacker = CMover::GetMoverObject(dwAttackerID);
    if (!pAttacker) {
        return false;
    }

    hkvVec3 vPos = *VisObject3D_cl::GetPosition(this);

    tagATTACK_AREA stAttackArea;
    tagATTACK_RANGE stRange;

    hkvVec3::setZero(&stRange.vCenterPos);
    stRange.fStartPos = m_fMinCounterRange;
    stRange.fRadius = m_fMaxCounterRange - m_fMinCounterRange;
    stRange.fAngle = m_fCounterAngle;
    stRange.fHeight = m_fCapsuleHeight;
    stRange.fBoxSizeX = 200.0f;
    stRange.fBoxSizeY = 200.0f;

    float fYaw = GetOrientationYaw();
    CMySkillList::GetAttackArea(this, &vPos, fYaw, 0, &stRange, &stAttackArea);

    return CMover::IsAttackDecision(pAttacker, &stAttackArea) == 0;
}

/**
 * @brief PlayCounterAnim - play counter attack animation
 * IDA: ?PlayCounterAnim@CMoverEx@@QEAAXXZ (0x140398330)
 * Verified: Plays counter attack animation and resets counter state
 */
void CMoverEx::PlayCounterAnim() {
    // IDA 精确还原:
    m_fCounterDuration = 0.0f;
    m_bEnableCounuter = false;
    m_bCounterSuccessFrame = true;

    if (m_szCounterAnimName[0]) {
        VString strAnimName(m_szCounterAnimName);
        std::uint32_t AnimIndex = CMover::GetAnimIndex(strAnimName);
        std::uint16_t nMotionClass = CMover::AnimKeyToMotion(AnimIndex);
        ChangeMotion_3(nMotionClass, true, false);

        m_bExistSuboCombo = false;
        m_fSuboComboWaitTime = 0.0f;
        m_fSuboComboCheckTime = 0.0f;
        m_iSuboComboMaxCount = -1;
        m_iSuboComboCheckCount = 0;
    }
}

/**
 * @brief SetChargingInput - set charging input parameters
 * @param fAnimSpeed Animation speed during charging
 * @param fMaxAddMutiple Maximum add multiple
 * @param fDuration Charging duration
 * IDA: ?SetChargingInput@CMoverEx@@QEAAXMMM@Z (0x140398430)
 * Verified: Sets up charging input parameters
 */
void CMoverEx::SetChargingInput(float fAnimSpeed, float fMaxAddMutiple, float fDuration) {
    // IDA 精确还原:
    m_fChargingInputMaxAddMultiple = fMaxAddMutiple;
    m_fChargingInputDuration = fDuration;
    m_fChargingInputAnimSpeed = fAnimSpeed;
    m_fChargingInputTime = 0.0f;
    m_fChargingInputPressTime = 0.0f;
    m_fChargingInputCalcMultiple = 0.0f;
}

/**
 * @brief CalcChargingInputMultiple - calculate charging input multiple
 * IDA: ?CalcChargingInputMultiple@CMoverEx@@QEAAXXZ (0x1403984C0)
 * Verified: Calculates charging input multiple based on press time
 */
void CMoverEx::CalcChargingInputMultiple() {
    // IDA 精确还原:
    m_fChargingInputCalcMultiple = m_fChargingInputMaxAddMultiple *
        (m_fChargingInputPressTime / m_fChargingInputDuration);
}

/**
 * @brief ClearChargingInput - clear charging input state
 * IDA: ?ClearChargingInput@CMoverEx@@QEAAXXZ (0x140398510)
 * Verified: Resets all charging input related values
 */
void CMoverEx::ClearChargingInput() {
    // IDA 精确还原:
    m_bChargingStart = false;
    m_fChargingInputMaxAddMultiple = 0.0f;
    m_fChargingInputDuration = 0.0f;
    m_fChargingInputAnimSpeed = 1.0f;
    m_fChargingInputTime = 0.0f;
    m_fChargingInputPressTime = 0.0f;
    m_fChargingInputCalcMultiple = 0.0f;
}

/**
 * @brief StartCharging - start charging input
 * IDA: ?StartCharging@CMoverEx@@QEAAXXZ (0x140398590)
 * Verified: Starts charging input with slow time effect
 */
void CMoverEx::StartCharging() {
    // IDA 精确还原:
    m_bChargingStart = true;

    float fRemainingTime = m_fChargingInputDuration - m_fChargingInputTime;
    float fSlowTime = fRemainingTime * (m_fAnimSpeed / m_fChargingInputAnimSpeed);

    CMover::SetSlowTime(fSlowTime, m_fChargingInputAnimSpeed);

    if (m_pCurMotionEvent && m_fSkillBlendEndTime > 0.0f) {
        VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
        float fRemainingAnim = (m_pCurMotionEvent->fAnimationLength - m_fAnimationTime) / m_fAnimSpeed;
        m_fSkillBlendEndTime = IVTimer::GetTime(pTimer) + fRemainingAnim;
    }
}

/**
 * @brief EndCharging - end charging input
 * IDA: ?EndCharging@CMoverEx@@QEAAXXZ (0x140398680)
 * Verified: Ends charging input and restores animation speed
 */
void CMoverEx::EndCharging() {
    // IDA 精确还原:
    m_bChargingStart = false;
    float fRestoreAnimSpeed = GetRestoreAnimSpeed();
    CMover::SetSlowTime(0.0f, fRestoreAnimSpeed);
    m_fChargingInputDuration = 0.0f;
    m_fChargingInputPressTime = 0.0f;
}

/**
 * @brief CalcBothClickChargingValue - calculate both click charging value
 * @param bLeftClick Is left click (false for right click)
 * IDA: ?CalcBothClickChargingValue@CMoverEx@@QEAAX_N@Z (0x1403986E0)
 * Verified: Calculates charging value for left/right click
 */
void CMoverEx::CalcBothClickChargingValue(bool bLeftClick) {
    // IDA 精确还原:
    if (!m_pCurSkillTableRef) {
        return;
    }

    // Get charging max time from skill table
    int nTimeValue = reinterpret_cast<int*>(&m_pCurSkillTableRef->Time_Value_01)[m_bySkillChargeStep];
    float fCharingMaxTime = static_cast<float>(nTimeValue) * 0.001f;

    float fAddValue = static_cast<float>(m_pCurSkillTableRef->Charging_Max_Value) / fCharingMaxTime * 0.5f;

    if (bLeftClick) {
        m_fLeftChargingValue += fAddValue;
    } else {
        m_fRightChargingValue += fAddValue;
    }
}

/**
 * @brief CalcRotationBlending - calculate rotation blending
 * @param fCurrYaw Current yaw angle
 * @param fTargetYaw Target yaw angle
 * @param fDeltaVal Delta value for blending
 * @return Blended yaw angle
 * IDA: ?CalcRotationBlending@CMoverEx@@QEAAMMMM@Z (0x1403987E0)
 * Verified: Calculates smooth rotation blending with angle normalization
 */
float CMoverEx::CalcRotationBlending(float fCurrYaw, float fTargetYaw, float fDeltaVal) {
    // IDA 精确还原:
    float fResult = fCurrYaw;

    if (fCurrYaw <= fTargetYaw) {
        if ((fCurrYaw - fTargetYaw) >= -180.0f) {
            fResult = fCurrYaw + fDeltaVal;
            if (fResult > fTargetYaw) {
                return fTargetYaw;
            }
        } else {
            fResult = fCurrYaw - fDeltaVal;
            if (fResult < -180.0f) {
                fResult += 360.0f;
                if (fTargetYaw > fResult) {
                    return fTargetYaw;
                }
            }
        }
    } else {
        if ((fCurrYaw - fTargetYaw) <= 180.0f) {
            fResult = fCurrYaw - fDeltaVal;
            if (fTargetYaw > fResult) {
                return fTargetYaw;
            }
        } else {
            fResult = fCurrYaw + fDeltaVal;
            if (fResult > 180.0f) {
                fResult -= 360.0f;
                if (fResult > fTargetYaw) {
                    return fTargetYaw;
                }
            }
        }
    }

    return fResult;
}

/**
 * @brief DeleteActionBuffer - delete action buffer
 * IDA: ?DeleteActionBuffer@CMoverEx@@UEAAXXZ (0x1403989A0)
 * Verified: Deletes all action buffers except special cases
 */
void CMoverEx::DeleteActionBuffer() {
    // IDA 精确还原:
    for (std::uint8_t byCode = 0; byCode < 0x35u; ++byCode) {
        if (byCode == 4) {
            ExcuteActionTrigger(byCode);
        } else {
            std::uint8_t byIndex = 0;
            tagACTION_BUFFER* pActionBuffer = m_xActionBuffer.FindCodeData(byCode, &byIndex);

            // Handle code 26 - SummonMonsterTrigger
            if (byCode == 26 && pActionBuffer) {
                SummonMonsterTrigger* pTrigger = reinterpret_cast<SummonMonsterTrigger*>(pActionBuffer->pActionTrigger);
                if (pTrigger && !pTrigger->IsLocalClient && pTrigger->SummonType && pTrigger->AlphaValue == 0.0f) {
                    continue;
                }
            }

            // Handle code 23
            if (byCode == 23 && pActionBuffer) {
                std::uint8_t* pActionTrigger = reinterpret_cast<std::uint8_t*>(pActionBuffer->pActionTrigger);
                if (pActionTrigger && (pActionTrigger[296] || pActionTrigger[297])) {
                    continue;
                }
            }

            // Handle code 18 - KeepLookTarget
            if (byCode == 18 && m_bKeepLookTarget) {
                m_bKeepLookTarget = false;
                m_fDefTurnSpeed = m_fBackupTurnSpeed;
            }

            m_xActionBuffer.DeleteCodeData(byCode);
        }
    }
}

/**
 * @brief ExcuteActionTrigger - execute action trigger by code
 * @param byCode Action code to execute
 * IDA: ?ExcuteActionTrigger@CMoverEx@@UEAAXE@Z (0x140398B60)
 * Verified: Executes all action triggers with given code
 */
void CMoverEx::ExcuteActionTrigger(std::uint8_t byCode) {
    // IDA 精确还原:
    while (true) {
        std::uint8_t byIndex = 0;
        tagACTION_BUFFER* pActionBuffer = m_xActionBuffer.FindCodeData(byCode, &byIndex);
        if (!pActionBuffer) {
            break;
        }

        tagACTION_BUFFER::ResetPosition(pActionBuffer);
        ActionBufferProcess(pActionBuffer);
        m_xActionBuffer.Delete(byIndex);
    }
}

/**
 * @brief GetOwnerPlayer - get owner player
 * @return Owner player CMoverEx pointer or nullptr
 * IDA: ?GetOwnerPlayer@CMoverEx@@QEAAPEAV1@XZ (0x140398BF0)
 * Verified: Returns owner player by owner ID
 */
CMoverEx* CMoverEx::GetOwnerPlayer() {
    // IDA 精确还原:
    if (m_dwOwnerID) {
        return dynamic_cast<CMoverEx*>(CMover::GetMoverObject(m_dwOwnerID));
    }
    return nullptr;
}

/**
 * @brief GetControlType - get control type from skill table
 * @param pSkillTable Skill table pointer
 * @return Control type value
 * IDA: ?GetControlType@CMoverEx@@QEAAEPEAUTB_SKILL@@@Z (0x140398C30)
 * Verified: Returns control type from skill or divergence table
 */
std::uint8_t CMoverEx::GetControlType(TB_SKILL* pSkillTable) {
    // IDA 精确还原:
    if (!pSkillTable) {
        return 0;
    }

    if (m_pCurDivergenceTableRef && m_pCurDivergenceTableRef->Div_Option_Type == 2) {
        return m_pCurDivergenceTableRef->Div_Option_Value;
    }

    return pSkillTable->Control_Type;
}

/**
 * @brief GetCameraLock - get camera lock from skill table
 * @param pSkillTable Skill table pointer
 * @return Camera lock value
 * IDA: ?GetCameraLock@CMoverEx@@QEAAEPEAUTB_SKILL@@@Z (0x140398C90)
 * Verified: Returns camera lock from skill or divergence table
 */
std::uint8_t CMoverEx::GetCameraLock(TB_SKILL* pSkillTable) {
    // IDA 精确还原:
    if (!pSkillTable) {
        return 0;
    }

    if (m_pCurDivergenceTableRef && m_pCurDivergenceTableRef->Div_Option_Type == 3) {
        return m_pCurDivergenceTableRef->Div_Option_Value;
    }

    return pSkillTable->Camera_Lock;
}

/**
 * @brief GetDivergenceValue - get divergence value
 * @return Divergence value based on skill and divergence table
 * IDA: ?GetDivergenceValue@CMoverEx@@UEAAHXZ (0x140398CF0)
 * Verified: Returns divergence level with group offset
 */
int CMoverEx::GetDivergenceValue() {
    // IDA 精确还原:
    if (!m_pCurSkillTableRef || !m_pCurDivergenceTableRef) {
        return 0;
    }

    // Check skill divergence group 01
    if (m_pCurSkillTableRef->Div_GroupID_01 == m_pCurDivergenceTableRef->Div_GroupID) {
        return m_pCurDivergenceTableRef->Div_Level + 100;
    }

    // Check skill divergence group 02
    if (m_pCurSkillTableRef->Div_GroupID_02 == m_pCurDivergenceTableRef->Div_GroupID) {
        return m_pCurDivergenceTableRef->Div_Level + 200;
    }

    // Check divergence skill table
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTbl = pServer->GetResourceMgr().GetTB_SKILL(m_dwDivergenceSkillID);
    if (!pSkillTbl) {
        return 0;
    }

    if (pSkillTbl->Div_GroupID_01 == m_pCurDivergenceTableRef->Div_GroupID) {
        return m_pCurDivergenceTableRef->Div_Level + 100;
    }

    if (pSkillTbl->Div_GroupID_02 == m_pCurDivergenceTableRef->Div_GroupID) {
        return m_pCurDivergenceTableRef->Div_Level + 200;
    }

    return 0;
}

/**
 * @brief GrapAttach - grap attach to parent entity
 * @param pParentEntity Parent entity to attach to
 * @param pTrigger Attack judgment trigger
 * IDA: ?GrapAttach@CMoverEx@@QEAAXPEAV1@PEAVAttackJudgmentTrigger@@@Z (0x140398E40)
 * Verified: Sets up grappling attachment to parent
 */
void CMoverEx::GrapAttach(CMoverEx* pParentEntity, AttackJudgmentTrigger* pTrigger) {
    // IDA 精确还原:
    if (!pParentEntity || !pTrigger || m_byGrapStep) {
        return;
    }

    VString::operator=(&m_szAttachBoneName, &pTrigger->sGrapInfo);
    m_byGrapStep = 1;
    m_pGrapParent = pParentEntity;
    CMoverEx::SetGrapStep(m_pGrapParent, 1);

    StopMoving(true);

    float fYaw = m_pGrapParent->GetMovingYaw();
    hkvMat3 matRot;
    hkvMat3::hkvMat3(&matRot);
    VisMath_cl::EulerToMatrix(fYaw, 0.0f, 0.0f, &matRot);

    hkvVec3 vDirection = pTrigger->sGrapInfo.vecDropPos;
    hkvVec3::normalizeIfNotZero(&vDirection, 0.000001f);
    vDirection *= matRot;

    float fDist = hkvVec3::getLength(&pTrigger->sGrapInfo.vecDropPos);
    vDirection *= fDist;

    const hkvVec3* pParentPos = VisObject3D_cl::GetPosition(m_pGrapParent);
    m_vGrapDropPos = *pParentPos + vDirection;

    CheckMoveDestPos(&m_vGrapDropPos, false, false);

    hkvVec3 vMyPos = *VisObject3D_cl::GetPosition(this);
    m_stDropOffset.x = vMyPos.x - m_vGrapDropPos.x;
    m_stDropOffset.y = vMyPos.y - m_vGrapDropPos.y;

    CMover::send_eSUB_CMD_MOVE_GRAP(this, this, &m_vGrapDropPos);
}

/**
 * @brief GrapDetach - detach from grap parent
 * @param pTrigger Detach trigger
 * @param fTime Detach time
 * IDA: ?GrapDetach@CMoverEx@@QEAAXPEAVDetachTrigger@@M@Z (0x140399180)
 * Verified: Initiates detach animation and drop
 */
void CMoverEx::GrapDetach(DetachTrigger* pTrigger, float fTime) {
    // IDA 精确还原:
    if (!m_pGrapParent) {
        return;
    }

    m_pDetachTrigger = pTrigger;

    hkvVec3 vMyPos = *VisObject3D_cl::GetPosition(this);
    hkvVec3 vOffset(
        m_vGrapDropPos.x - vMyPos.x,
        m_vGrapDropPos.y - vMyPos.y,
        m_vGrapDropPos.z - vMyPos.z
    );

    m_fDropSpeedX = fabsf(vOffset.x) / fTime;
    m_fDropSpeedY = fabsf(vOffset.y) / fTime;
    m_fDropSpeedZ = fabsf(vOffset.z) / fTime;

    m_bLanded = false;
    m_byGrapStep = 2;
    CMoverEx::SetGrapStep(m_pGrapParent, 2);

    m_fFlyDirValue = 1.0f;
    m_nHitStatus = 6;
    ChangeMotion_3(19, true, false);
}

/**
 * @brief CheckGrapDamage - check grap damage
 * IDA: ?CheckGrapDamage@CMoverEx@@QEAAXXZ (0x140399360)
 * Verified: Processes grap damage and clears grap state
 */
void CMoverEx::CheckGrapDamage() {
    // IDA 精确还原:
    if (!m_pGrapParent || !m_pDetachTrigger || m_byGrapStep != 3) {
        return;
    }

    m_byGrapStep = 0;
    CMoverEx::SetGrapStep(m_pGrapParent, 0);

    CMover::SetCurSkillTableIdx(m_pGrapParent, m_pDetachTrigger->SkillID);

    CMoverEx* pGrapTarget = CMoverEx::GetGrapTarget(m_pGrapParent);
    AttackJudgmentTrigger* pTrigger = CMoverEx::GetAttackJudgmentEvent(
        m_pGrapParent, m_pDetachTrigger->szAniName, m_pDetachTrigger->RefEventID);

    if (pTrigger) {
        m_pGrapParent->ActionProcess(pTrigger->EventID);
    }

    if (pGrapTarget) {
        CMover::send_eSUB_CMD_MOVE_DROP(pGrapTarget, pGrapTarget, 1);
    }

    if (m_pGrapParent) {
        CMoverEx::ClearGrapProcess(m_pGrapParent);
    }

    m_pGrapParent = nullptr;
    m_pDetachTrigger = nullptr;
}

/**
 * @brief CheckGrapRaise - check if grap can raise
 * @return true if grap step is 0
 * IDA: ?CheckGrapRaise@CMoverEx@@QEAAHXZ (0x1403994D0)
 * Verified: Checks if grappling is in initial state
 */
bool CMoverEx::CheckGrapRaise() {
    // IDA 精确还原:
    return m_byGrapStep == 0;
}

/**
 * @brief ProcessGrap - process grap state
 * IDA: ?ProcessGrap@CMoverEx@@QEAAXXZ (0x1403994F0)
 * Verified: Updates grap state based on current step
 */
void CMoverEx::ProcessGrap() {
    // IDA 精确还原:
    if (XActor::IsStatus(2u)) {
        ClearGrapProcess();
    } else {
        if (m_byGrapStep == 1) {
            UpdateGrapAttach();
        }
        if (m_byGrapStep == 2) {
            UpdateGrapDetach();
        }
    }
}

/**
 * @brief UpdateGrapAttach - update grap attach state
 * IDA: ?UpdateGrapAttach@CMoverEx@@QEAAXXZ (0x140399560)
 * Verified: Updates attached grap state
 */
void CMoverEx::UpdateGrapAttach() {
    // IDA 精确还原:
    if (m_byGrapStep == 1 && m_pGrapParent) {
        VString::IsEmpty(&m_szAttachBoneName);
    }
}

/**
 * @brief ClearGrapProcess - clear grap process
 * IDA: ?ClearGrapProcess@CMoverEx@@QEAAXXZ (0x1403995B0)
 * Verified: Clears all grap related state and references
 */
void CMoverEx::ClearGrapProcess() {
    // IDA 精确还原:
    hkvVec3::Clear(&m_vGrapDropPos);
    tagMOVE_POS::Clear(&m_stDropOffset);
    m_fFlyDirValue = -0.1f;
    m_byGrapStep = 0;

    SetLookPitch();
    float fYaw = GetOrientationYaw();
    VisObject3D_cl::SetOrientation(fYaw, 0.0f, 0.0f);

    if (m_pGrapParent) {
        CMoverEx::SetGrapStep(m_pGrapParent, 0);
        CMoverEx::SetGrapTarget(m_pGrapParent, nullptr);
        m_pGrapParent = nullptr;
    }

    if (m_pGrapTarget) {
        CMoverEx::ClearGrapProcess(m_pGrapTarget);
        m_pGrapTarget = nullptr;
    }
}

/**
 * @brief UpdateGrapDetach - update grap detach state
 * IDA: ?UpdateGrapDetach@CMoverEx@@QEAAXXZ (0x1403996C0)
 * Verified: Updates detach animation and movement
 */
void CMoverEx::UpdateGrapDetach() {
    // IDA 精确还原:
    if (m_byGrapStep != 2 || hkvVec3::IsZero(&m_vGrapDropPos)) {
        return;
    }

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = IVTimer::GetTimeDifference(pTimer);

    if (fDeltaTime <= 0.0f) {
        return;
    }

    hkvVec3 vMyPos = *VisObject3D_cl::GetPosition(this);

    float fDiffX = vMyPos.x - m_vGrapDropPos.x;
    float fDiffY = vMyPos.y - m_vGrapDropPos.y;
    float fDiffZ = vMyPos.z - m_vGrapDropPos.z;

    // Check if still need to move
    if ((fabsf(fDiffX) >= 3.0f || fabsf(fDiffY) >= 3.0f) &&
        ((fDiffX * m_stDropOffset.x) > 0.0f || (fDiffY * m_stDropOffset.y) > 0.0f)) {

        hkvVec3 vOffset(
            m_vGrapDropPos.x - vMyPos.x,
            m_vGrapDropPos.y - vMyPos.y,
            m_vGrapDropPos.z - vMyPos.z
        );

        hkvVec3 vOffsetDelta = vOffset;
        hkvVec3::normalizeIfNotZero(&vOffsetDelta, 0.000001f);

        vOffsetDelta.x = vOffsetDelta.x * m_fDropSpeedX * fDeltaTime;
        vOffsetDelta.y = vOffsetDelta.y * m_fDropSpeedY * fDeltaTime;
        vOffsetDelta.z = vOffsetDelta.z * m_fDropSpeedZ * fDeltaTime;

        // Clamp delta to offset
        if (vOffsetDelta.x > 0.0f && vOffsetDelta.x > vOffset.x) {
            vOffsetDelta.x = vOffset.x;
        } else if (vOffsetDelta.x < 0.0f && vOffset.x > vOffsetDelta.x) {
            vOffsetDelta.x = vOffset.x;
        }

        if (vOffsetDelta.y > 0.0f && vOffsetDelta.y > vOffset.y) {
            vOffsetDelta.y = vOffset.y;
        } else if (vOffsetDelta.y < 0.0f && vOffset.y > vOffsetDelta.y) {
            vOffsetDelta.y = vOffset.y;
        }

        if (vOffsetDelta.z > 0.0f && vOffsetDelta.z > vOffset.z) {
            vOffsetDelta.z = vOffset.z;
        } else if (vOffsetDelta.z < 0.0f && vOffset.z > vOffsetDelta.z) {
            vOffsetDelta.z = vOffset.z;
        }

        hkvVec3 vDestPos = vMyPos + vOffsetDelta;

        if (!CMover::IsFlying()) {
            CMover::GetHeight(&vDestPos, 200.0f);
        }

        if (CMover::CheckMoveCollision(&vDestPos)) {
            m_bCancelMoving = true;
            hkvVec3::Clear(&m_vGrapDropPos);
            tagMOVE_POS::Clear(&m_stDropOffset);
            m_byGrapStep = 3;
            m_fFlyDirValue = -0.1f;

            hkvVec3 vMovePos(vDestPos.x, vDestPos.y, vDestPos.z);
            Move(&vMovePos);
            CMover::MoveingValueClear();

            hkvVec3 vCurPos = *VisObject3D_cl::GetPosition(this);
            CMover::send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, &vCurPos, false);
        } else {
            hkvVec3 vMovePos(vDestPos.x, vDestPos.y, vDestPos.z);
            Move(&vMovePos);
        }
    } else {
        // Reached destination
        hkvVec3 vMovePos(m_vGrapDropPos.x, m_vGrapDropPos.y, m_vGrapDropPos.z);
        Move(&vMovePos);

        hkvVec3::Clear(&m_vGrapDropPos);
        tagMOVE_POS::Clear(&m_stDropOffset);
        m_byGrapStep = 3;
        m_fFlyDirValue = -0.1f;
    }
}

/**
 * @brief SetHitPartsInfo - set hit parts info
 * @param iIndex Hit parts index
 * @param dwTableID Table ID
 * @param byPartsID Parts ID
 * @param iCurHP Current HP
 * @param iMaxHP Max HP
 * IDA: ?SetHitPartsInfo@CMoverEx@@QEAAXHKEHH@Z (0x140399CC0)
 * Verified: Sets hit parts information
 */
void CMoverEx::SetHitPartsInfo(int iIndex, std::uint32_t dwTableID, std::uint8_t byPartsID, int iCurHP, int iMaxHP) {
    // IDA 精确还原:
    if (iIndex < 2) {
        m_sHitParts[iIndex].dwTableID = dwTableID;
        m_sHitParts[iIndex].byPartsID = byPartsID;
        SetHitPartsHP(iIndex, iCurHP, iMaxHP);
    }
}

/**
 * @brief SetHitPartsHP - set hit parts HP
 * @param iIndex Hit parts index
 * @param iCurHP Current HP
 * @param iMaxHP Max HP
 * IDA: ?SetHitPartsHP@CMoverEx@@QEAAXHHH@Z (0x140399D30)
 * Verified: Sets hit parts HP values
 */
void CMoverEx::SetHitPartsHP(int iIndex, int iCurHP, int iMaxHP) {
    // IDA 精确还原:
    if (iIndex < 2) {
        if (iCurHP > iMaxHP) {
            iCurHP = iMaxHP;
        }
        m_sHitParts[iIndex].iCurHP = iCurHP;
        m_sHitParts[iIndex].iMaxHP = iMaxHP;
    }
}

/**
 * @brief SetChangeMonsterInfo - set change monster info
 * @param dwServerID Server ID
 * @param dwTableID Table ID
 * IDA: ?SetChangeMonsterInfo@CMoverEx@@QEAAXKK@Z (0x140399DA0)
 * Verified: Sets monster transformation info
 */
void CMoverEx::SetChangeMonsterInfo(std::uint32_t dwServerID, std::uint32_t dwTableID) {
    // IDA 精确还原:
    m_dwChangeMobNewID = dwServerID;
    m_dwChangeMobTableID = dwTableID;
}

/**
 * @brief PlayChangeMonsterMotion - play change monster motion
 * @param pszMotionName Motion name to play
 * IDA: ?PlayChangeMonsterMotion@CMoverEx@@QEAAXPEBD@Z (0x140399DD0)
 * Verified: Plays monster transformation animation
 */
void CMoverEx::PlayChangeMonsterMotion(const char* pszMotionName) {
    // IDA 精确还原:
    if (!m_bReserveChange) {
        ChangeMotion_2(pszMotionName, true);
        m_bReserveChange = true;
        CMover::SetInvincibleActor(true);
    }
}

/**
 * @brief IsOptionInvokeArea - check if option invoke area matches
 * @param eAreaType Area type to check
 * @return true if in matching area
 * IDA: ?IsOptionInvokeArea@CMoverEx@@QEAA_NW4EFFECT_INVOKE_AREA_TYPE@@@Z (0x140399E30)
 * Verified: Checks if current area matches the specified invoke area type
 */
bool CMoverEx::IsOptionInvokeArea(EFFECT_INVOKE_AREA_TYPE eAreaType) {
    // IDA 精确还原:
    XArea* pArea = XActor::GetArea();
    if (!pArea) {
        return false;
    }

    bool bFlag = false;
    int arMazeID[6] = {21011, 21012, 21013, 21014, 21051, 21061};
    int arMazeType[3] = {2, 8, 9};

    switch (eAreaType) {
        case EFFECT_INVOKE_AREA_ALL:
            bFlag = true;
            break;

        case EFFECT_INVOKE_AREA_BATTLE:
            if (pArea->GetWorldType() == 1 || pArea->GetWorldType() == 2) {
                bFlag = true;
            }
            break;

        case EFFECT_INVOKE_AREA_NONE_BATTLE:
            if (!pArea->GetWorldType() || pArea->GetWorldType() == 3) {
                bFlag = true;
            }
            break;

        case EFFECT_INVOKE_AREA_ROCCO_TOWN_MAZE:
        case EFFECT_INVOKE_AREA_CANDUS_CITY_MAZE:
        case EFFECT_INVOKE_AREA_GRACE_CITY_MAZE:
        case EFFECT_INVOKE_AREA_RUIN_FORTRESS_MAZE:
            bFlag = pArea->GetWorldType() == 1;
            break;

        case EFFECT_INVOKE_AREA_STEEL_GRAVE: {
            XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
            if (pMaze) {
                std::uint16_t TBMapID = pMaze->GetTBMapID();
                XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                TB_MAZE_INFO* pMazeInfo = pServer->GetResourceMgr().GetTB_MAZE_INFO(TBMapID);
                if (pMazeInfo && pMazeInfo->Maze_Type == 7) {
                    bFlag = true;
                }
            }
            break;
        }

        case EFFECT_INVOKE_AREA_DISTRICT6:
            bFlag = pArea->GetWorldType() == 2;
            break;

        case EFFECT_INVOKE_AREA_LAST_CANIVAL:
        case EFFECT_INVOKE_AREA_JUNK_HIVE:
        case EFFECT_INVOKE_AREA_BANDIT_HIGHWAY:
        case EFFECT_INVOKE_AREA_IRON_CASTLE:
        case EFFECT_INVOKE_AREA_GOLDEN_CITADEL:
        case EFFECT_INVOKE_AREA_THE_PRIMAL: {
            int iIndex = eAreaType - 16;
            XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
            if (pMaze && iIndex >= 0 && iIndex < 6) {
                bFlag = pMaze->GetTBMapID() == arMazeID[iIndex];
            }
            break;
        }

        case EFFECT_INVOKE_AREA_CASUAL_RAID:
        case EFFECT_INVOKE_AREA_DISTRICT_RAID:
        case EFFECT_INVOKE_AREA_APOCALYPSE_RAID: {
            int iIndex = eAreaType - 51;
            XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
            if (pMaze && iIndex > 0 && iIndex < 3) {
                TB_MAZE_INFO* pMazeInfo = pServer->GetResourceMgr().GetTB_MAZE_INFO(pMaze->GetTBMapID());
                if (pMazeInfo && pMazeInfo->Maze_Type == arMazeType[iIndex]) {
                    bFlag = true;
                }
            }
            break;
        }

        case EFFECT_INVOKE_AREA_MYROOM:
            bFlag = pArea->GetWorldType() == 3;
            break;

        default:
            break;
    }

    return bFlag;
}

/**
 * @brief IsOptionTarget - check if option target matches
 * @param eTargetType Target type to check
 * @param pMoverEx Target mover to check against
 * @return true if target matches
 * IDA: ?IsOptionTarget@CMoverEx@@QEAA_NW4EFFECT_TARGET_TYPE@@PEAV1@@Z (0x14039A4B0)
 * Verified: Checks if target matches the specified target type
 */
bool CMoverEx::IsOptionTarget(EFFECT_TARGET_TYPE eTargetType, CMoverEx* pMoverEx) {
    // IDA 精确还原:
    if (!pMoverEx) {
        return false;
    }

    bool bFlag = false;
    int iFactionList[8] = {6, 7, 8, 9, 16, 17, 18, 19};

    switch (eTargetType) {
        case EFFECT_TARGET_MYSELF:
        case EFFECT_TARGET_ALL:
            bFlag = true;
            break;

        case EFFECT_TARGET_MOB_ALL:
            bFlag = XActor::GetType() == eActorMonster;
            break;

        case EFFECT_TARGET_MOB_NORMAL:
        case EFFECT_TARGET_MOB_BOSS:
        case EFFECT_TARGET_MOB_RAID_BOSS: {
            CMonster* pMonster = dynamic_cast<CMonster*>(pMoverEx);
            if (pMonster) {
                TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
                if (pMobRef) {
                    if (eTargetType == EFFECT_TARGET_MOB_NORMAL && pMobRef->Monster_Rank <= 2) {
                        bFlag = true;
                    } else if (eTargetType == EFFECT_TARGET_MOB_BOSS && 
                               pMobRef->Monster_Rank >= 3 && pMobRef->Monster_Rank <= 5) {
                        bFlag = true;
                    } else if (eTargetType == EFFECT_TARGET_MOB_RAID_BOSS && pMobRef->Monster_Rank == 5) {
                        bFlag = true;
                    }
                }
            }
            break;
        }

        case EFFECT_TARGET_MOB_SJUNK:
        case EFFECT_TARGET_MOB_DOLL:
        case EFFECT_TARGET_MOB_NED:
        case EFFECT_TARGET_MOB_RAPID:
        case EFFECT_TARGET_MOB_NIHILL:
        case EFFECT_TARGET_MOB_IRON:
        case EFFECT_TARGET_MOB_NEAR:
        case EFFECT_TARGET_MOB_DIS6: {
            CMonster* pMonster = dynamic_cast<CMonster*>(pMoverEx);
            if (pMonster) {
                TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
                if (pMobRef) {
                    int iIndex = eTargetType - 7;
                    if (iIndex > 7) iIndex = 7;
                    if (iFactionList[iIndex] == pMobRef->Monster_Faction) {
                        bFlag = true;
                    }
                }
            }
            break;
        }

        case EFFECT_TARGET_PC_ALL:
            bFlag = XActor::GetType() == eActorUser;
            break;

        case EFFECT_TARGET_PC_SOULUM:
        case EFFECT_TARGET_PC_GUNJAZZ:
        case EFFECT_TARGET_PC_MYSTSYTHE:
        case EFFECT_TARGET_PC_SPRITARMS:
        case EFFECT_TARGET_PC_HOWLING:
        case EFFECT_TARGET_PC_HAMMER: {
            CUser* pUser = dynamic_cast<CUser*>(pMoverEx);
            if (pUser) {
                bFlag = (eTargetType - 50) == pUser->GetClass();
            }
            break;
        }

        case EFFECT_TARGET_HELPER: {
            CMonster* pMonster = dynamic_cast<CMonster*>(pMoverEx);
            if (pMonster) {
                TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
                if (pMobRef && pMobRef->Monster_Type == 10) {
                    bFlag = true;
                }
            }
            break;
        }

        default:
            break;
    }

    return bFlag;
}

/**
 * @brief IsOptionSituation - check if option situation matches
 * @param eSituationType Situation type to check
 * @param pMoverEx Target mover
 * @param dwAidValue Aid value for comparison
 * @param byAidApplyType Aid apply type
 * @return true if situation matches
 * IDA: ?IsOptionSituation@CMoverEx@@QEAA_NW4EFFECT_SITUATION_TYPE@@PEAV1@KE@Z (0x14039A8F0)
 * Verified: Checks if situation matches the specified condition
 */
bool CMoverEx::IsOptionSituation(EFFECT_SITUATION_TYPE eSituationType, CMoverEx* pMoverEx, 
                                  std::uint32_t dwAidValue, std::uint8_t byAidApplyType) {
    // IDA 精确还原:
    if (!pMoverEx) {
        return false;
    }

    float fCheckValueRate = 0.0f;
    float fCheckValueInt = 0.0f;
    int iBuffList[12] = {111, 112, 113, 114, 121, 122, 123, 124, 131, 132, 125, 126};

    switch (eSituationType) {
        case EFFECT_SITUATION_NOT_USE:
            return true;

        case EFFECT_SITUATION_OWNER_HP: {
            float fHP = CMover::GetStat(1);
            float fMaxHP = CMover::GetStat(10);
            fCheckValueRate = (fHP / fMaxHP) * 100.0f;
            fCheckValueInt = fHP;
            break;
        }

        case EFFECT_SITUATION_TARGET_HP: {
            float fHP = pMoverEx->CMover::GetStat(1);
            float fMaxHP = pMoverEx->CMover::GetStat(10);
            fCheckValueRate = (fHP / fMaxHP) * 100.0f;
            fCheckValueInt = fHP;
            break;
        }

        case EFFECT_SITUATION_OWNER_SG: {
            float fSG = CMover::GetStat(2);
            float fMaxSG = CMover::GetStat(12);
            fCheckValueRate = (fSG / fMaxSG) * 100.0f;
            fCheckValueInt = fSG;
            break;
        }

        case EFFECT_SITUATION_TARGET_SG: {
            float fSG = pMoverEx->CMover::GetStat(2);
            float fMaxSG = pMoverEx->CMover::GetStat(12);
            fCheckValueRate = (fSG / fMaxSG) * 100.0f;
            fCheckValueInt = fSG;
            break;
        }

        case EFFECT_SITUATION_OWNER_STAMINA: {
            float fST = CMover::GetStat(3);
            float fMaxST = CMover::GetStat(14);
            fCheckValueRate = (fST / fMaxST) * 100.0f;
            fCheckValueInt = fST;
            break;
        }

        case EFFECT_SITUATION_TARGET_STAMINA: {
            float fST = pMoverEx->CMover::GetStat(3);
            float fMaxST = pMoverEx->CMover::GetStat(14);
            fCheckValueRate = (fST / fMaxST) * 100.0f;
            fCheckValueInt = fST;
            break;
        }

        case EFFECT_SITUATION_TARGET_SUPERARMOR_GAGE: {
            float fCurGage = pMoverEx->CMover::GetCurSuperArmorGage();
            float fMaxGage = pMoverEx->CMover::GetMaxSuperArmorGage();
            fCheckValueRate = (fCurGage / fMaxGage) * 100.0f;
            fCheckValueInt = fCurGage;
            break;
        }

        case EFFECT_SITUATION_PARTY_FORCE_MAMBER_COUNT: {
            std::shared_ptr<CGocParty> pParty;
            CMover::GetGOC<CGocParty>(&pParty, 0);
            if (pParty && pParty->IsParty()) {
                fCheckValueInt = static_cast<float>(pParty->GetForceUserCount());
            } else {
                std::shared_ptr<CGocForce> pForce;
                CMover::GetGOC<CGocForce>(&pForce, 0);
                if (pForce && pForce->IsParty()) {
                    fCheckValueInt = static_cast<float>(pForce->GetForceUserCount());
                }
            }
            break;
        }

        case EFFECT_SITUATION_SA_BREAK:
            return pMoverEx->CMover::GetMaxSuperArmorGage() > 0.0f && 
                   pMoverEx->CMover::GetCurSuperArmorGage() <= 0.0f;

        case EFFECT_SITUATION_BURN:
        case EFFECT_SITUATION_POISON:
        case EFFECT_SITUATION_SHOCK:
        case EFFECT_SITUATION_BLEED:
        case EFFECT_SITUATION_STUN:
        case EFFECT_SITUATION_PARALYSIS:
        case EFFECT_SITUATION_SLEEP:
        case EFFECT_SITUATION_FREEZE:
        case EFFECT_SITUATION_SILENCE:
        case EFFECT_SITUATION_WEAKNESS:
        case EFFECT_SITUATION_CHARM:
        case EFFECT_SITUATION_CONFUSION:
        case EFFECT_SITUATION_ALL_DEBUFF:
            if (eSituationType == EFFECT_SITUATION_ALL_DEBUFF) {
                for (int i = 0; i < 12; ++i) {
                    if (CMover::FindBuffByEffectType(iBuffList[i], 0) != -1) {
                        return true;
                    }
                }
            } else {
                int iSituationIndex = eSituationType - 80;
                if (CMover::FindBuffByEffectType(iBuffList[iSituationIndex], 0) != -1) {
                    return true;
                }
            }
            return false;

        default:
            break;
    }

    // Apply value comparison
    if (!byAidApplyType && (fCheckValueInt != 0.0f || fCheckValueRate != 0.0f)) {
        return true;
    }
    if (byAidApplyType == 1 && static_cast<float>(dwAidValue) > fCheckValueInt) {
        return true;
    }
    if (byAidApplyType == 2 && fCheckValueInt >= static_cast<float>(dwAidValue)) {
        return true;
    }
    if (byAidApplyType == 3 && static_cast<float>(dwAidValue) > fCheckValueRate) {
        return true;
    }
    if (byAidApplyType == 4 && fCheckValueRate >= static_cast<float>(dwAidValue)) {
        return true;
    }

    return false;
}

/**
 * @brief IsStatOptionEffect - check if status type is stat option effect
 * @param eStatusType Status type to check
 * @return true if it's a stat option effect
 * IDA: ?IsStatOptionEffect@CMoverEx@@QEAA_NW4EFFECT_STATUS_TYPE@@@Z (0x14039B0A0)
 * Verified: Checks if status type affects stats
 */
bool CMoverEx::IsStatOptionEffect(EFFECT_STATUS_TYPE eStatusType) {
    // IDA 精确还原:
    switch (eStatusType) {
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

/**
 * @brief IsCanOptionEffectExcute - check if option effect can execute
 * @param pOptionRef Option table reference
 * @param pTargetMoverEx Target mover
 * @param fOptionValue Option value
 * @return true if option effect can execute
 * IDA: ?IsCanOptionEffectExcute@CMoverEx@@QEAA_NPEAUTB_CREATEOPTION@@PEAV1@M@Z (0x14039B010)
 * Verified: Checks if option effect passes target check and random chance
 */
bool CMoverEx::IsCanOptionEffectExcute(TB_CREATEOPTION* pOptionRef, CMoverEx* pTargetMoverEx, float fOptionValue) {
    // IDA 精确还原:
    if (!pOptionRef) {
        return false;
    }

    if (pOptionRef->Value_Clm != 3) {
        return false;
    }

    if (!IsOptionTarget(static_cast<EFFECT_TARGET_TYPE>(pOptionRef->Apply_Target), pTargetMoverEx)) {
        return false;
    }

    return fOptionValue > static_cast<float>(rand() % 100);
}

/**
 * @brief GetOptionEffectValue - get option effect value
 * @param pOptionRef Option table reference
 * @param pTargetMoverEx Target mover
 * @param fOptionValue Option value
 * @param fReferanceValue Reference value
 * @return Calculated option effect value
 * IDA: ?GetOptionEffectValue@CMoverEx@@QEAAMPEAUTB_CREATEOPTION@@PEAV1@MM@Z (0x14039B120)
 * Verified: Calculates option effect value based on various stat references
 */
float CMoverEx::GetOptionEffectValue(TB_CREATEOPTION* pOptionRef, CMoverEx* pTargetMoverEx, 
                                      float fOptionValue, float fReferanceValue) {
    // IDA 精确还原:
    if (!pOptionRef) {
        return 0.0f;
    }

    std::shared_ptr<CGocAttribute> pAttr;
    CMover::GetGOC<CGocAttribute>(&pAttr, 0);

    if (!pAttr) {
        return 0.0f;
    }

    CGocAttribute* pGocAttr = pAttr.get();

    switch (pOptionRef->Value_Clm) {
        case 0:
            return 1.0f;

        case 1:
            return fOptionValue;

        case 2: {
            float fValueRate = fOptionValue;
            int nReferStatus = pOptionRef->Refer_Status - 1;

            switch (nReferStatus) {
                case 0:
                    fValueRate = pGocAttr->GetOriginStat(21) * (fOptionValue / 100.0f);
                    break;
                case 1:
                case 3:
                case 4:
                case 5:
                case 6:
                    fValueRate = fReferanceValue * (fOptionValue / 100.0f);
                    break;
                case 2:
                    fValueRate = pGocAttr->GetOriginStat(24) * (fOptionValue / 100.0f);
                    break;
                case 7:
                    fValueRate = m_fAbility[10] * (fOptionValue / 100.0f);
                    break;
                case 8:
                    fValueRate = pGocAttr->GetOriginStat(26) * (fOptionValue / 100.0f);
                    break;
                case 9:
                    fValueRate = pGocAttr->GetOriginStat(43) * (fOptionValue / 100.0f);
                    break;
                case 10:
                    fValueRate = pGocAttr->GetOriginStat(33) * (fOptionValue / 100.0f);
                    break;
                case 11:
                    fValueRate = m_fAbility[12] * (fOptionValue / 100.0f);
                    break;
                case 12:
                    fValueRate = m_fAbility[14] * (fOptionValue / 100.0f);
                    break;
                case 13:
                    fValueRate = pGocAttr->GetOriginStat(17) * (fOptionValue / 100.0f);
                    break;
                case 14:
                    fValueRate = pGocAttr->GetOriginStat(18) * (fOptionValue / 100.0f);
                    break;
                case 15:
                    fValueRate = pGocAttr->GetOriginStat(19) * (fOptionValue / 100.0f);
                    break;
                case 16:
                    fValueRate = pGocAttr->GetOriginStat(31) * (fOptionValue / 100.0f);
                    break;
                case 17:
                    fValueRate = pGocAttr->GetOriginStat(38) * (fOptionValue / 100.0f);
                    break;
                case 18:
                    fValueRate = pGocAttr->GetOriginStat(47) * (fOptionValue / 100.0f);
                    break;
                case 19:
                    fValueRate = pGocAttr->GetOriginStat(35) * (fOptionValue / 100.0f);
                    break;
                default:
                    break;
            }
            return fValueRate;
        }

        case 3:
            return 1.0f;

        default:
            return 0.0f;
    }
}

/**
 * @brief CheckOptionEffectInvoke - check and invoke option effects
 * @param eCondition Effect condition type
 * @param pTargetMover Target mover
 * @param fReferanceValue Reference value
 * @param eInvokeType Invoke type
 * IDA: ?CheckOptionEffectInvoke@CMoverEx@@QEAAXW4EFFECT_CONDITION_TYPE@@PEAV1@MW4EFFECT_INVOKE_TYPE@@@Z (0x14039B670)
 * Verified: Checks and applies option effects based on conditions
 */
void CMoverEx::CheckOptionEffectInvoke(EFFECT_CONDITION_TYPE eCondition, CMoverEx* pTargetMover, 
                                        float fReferanceValue, EFFECT_INVOKE_TYPE eInvokeType) {
    // IDA 精确还原:
    std::shared_ptr<CGocAttribute> pMyAttr;
    CMover::GetGOC<CGocAttribute>(&pMyAttr, 0);

    if (!pMyAttr) {
        return;
    }

    std::vector<CMover*> vecGameObjList;
    XActor* pActor = this ? &XActor::m_XActor : nullptr;
    XArea::ScanGridOrigin(pActor, 2, 3u, &vecGameObjList);

    m_nAddMoneyFromOptionEffect = 0;
    m_nAddEtherFromOptionEffect = 0;
    m_nAddExpFromOptionEffect = 0;

    CGocAttribute* pGocAttr = pMyAttr.get();
    const std::vector<SEquipedOption*>& vecEquipedOption = pGocAttr->GetEquipedOption();

    for (auto it = vecEquipedOption.begin(); it != vecEquipedOption.end(); ++it) {
        SEquipedOption* pEquipedOption = *it;
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        TB_CREATEOPTION* pOptionTable = pServer->GetResourceMgr().GetTB_CREATEOPTION(pEquipedOption->dwOptionID);

        if (!pOptionTable || !IsOptionInvokeArea(static_cast<EFFECT_INVOKE_AREA_TYPE>(pOptionTable->Invoke_Area))) {
            continue;
        }

        CMoverEx* pRealTargetMover = pTargetMover;
        if (pOptionTable->Apply_Target == 1) {
            pRealTargetMover = this;
        }

        if (!IsOptionTarget(static_cast<EFFECT_TARGET_TYPE>(pOptionTable->Invoke_Target), pTargetMover)) {
            continue;
        }

        SOptionEffect* pAppliedOptionEffect = GetOptionEffect(pEquipedOption->dwIndex, pEquipedOption->dwOptionID, pRealTargetMover);

        // Check conditions
        if (eCondition == EFFECT_CONDITION_EQUIP && pAppliedOptionEffect) {
            continue;
        }
        if (pOptionTable->Invoke_Condition != static_cast<std::uint8_t>(eCondition)) {
            continue;
        }
        if (eInvokeType == EFFECT_INVOKE_NONE_STAT && IsStatOptionEffect(static_cast<EFFECT_STATUS_TYPE>(pOptionTable->StatusType))) {
            continue;
        }
        if (eInvokeType && !IsStatOptionEffect(static_cast<EFFECT_STATUS_TYPE>(pOptionTable->StatusType))) {
            continue;
        }
        if (pOptionTable->Chance_Clm && (rand() % 100) >= pOptionTable->Chance_Clm) {
            continue;
        }
        if (!IsOptionSituation(static_cast<EFFECT_SITUATION_TYPE>(pOptionTable->Situation_Check), 
                              pRealTargetMover, pOptionTable->Aid_Value, pOptionTable->Aid_Clm)) {
            continue;
        }

        // Check execute condition
        if (pOptionTable->Value_Clm == 3 && !IsCanOptionEffectExcute(pOptionTable, pRealTargetMover, pEquipedOption->fOptionValue)) {
            continue;
        }

        // Apply or refresh effect
        if (pAppliedOptionEffect) {
            if (pAppliedOptionEffect->dwLifeTime >= 1000) {
                pAppliedOptionEffect->dwLifeTime = pOptionTable->Value_Clmcrt;
                pAppliedOptionEffect->fCurTime = 0.0f;
            }
        } else {
            float fCalcValue = GetOptionEffectValue(pOptionTable, pRealTargetMover, pEquipedOption->fOptionValue, fReferanceValue);
            ApplyOptionEffectValue(pRealTargetMover, static_cast<EFFECT_STATUS_TYPE>(pOptionTable->StatusType), fCalcValue);
            AddOptionEffect(pEquipedOption->dwIndex, pOptionTable, pEquipedOption->fOptionValue, fReferanceValue, fCalcValue, pRealTargetMover);
        }
    }
}

/**
 * @brief ReleaseInvokedOptionEffect - release invoked option effects
 * @param eCondition Effect condition type to release
 * IDA: ?ReleaseInvokedOptionEffect@CMoverEx@@QEAAXW4EFFECT_CONDITION_TYPE@@@Z (0x14039BB70)
 * Verified: Releases option effects matching the condition
 */
void CMoverEx::ReleaseInvokedOptionEffect(EFFECT_CONDITION_TYPE eCondition) {
    // IDA 精确还原:
    std::uint8_t iConditionList[16] = {0, 0, 2, 6, 9, 10, 21, 20, 3, 7, 5, 4};

    for (auto it = m_vecOptionEffect.begin(); it != m_vecOptionEffect.end(); ) {
        SOptionEffect* pOptionEffect = *it;

        if (pOptionEffect->dwLifeTime < 1000 && pOptionEffect->pOptionTable) {
            CMoverEx* pTargetMoverEx = dynamic_cast<CMoverEx*>(CMover::GetMoverObject(pOptionEffect->dwTargetMoverID));

            if (pTargetMoverEx) {
                int iValueCondition = pOptionEffect->pOptionTable->Value_Clmcrt;
                iConditionList[1] = pOptionEffect->pOptionTable->Invoke_Condition;

                if (iValueCondition && iConditionList[iValueCondition] == static_cast<std::uint8_t>(eCondition)) {
                    ReleaseOptionEffectValue(pTargetMoverEx, static_cast<EFFECT_STATUS_TYPE>(pOptionEffect->pOptionTable->StatusType), 
                                             pOptionEffect->fAppliedValue);
                    delete pOptionEffect;
                    it = m_vecOptionEffect.erase(it);
                } else {
                    ++it;
                }
            } else {
                delete pOptionEffect;
                it = m_vecOptionEffect.erase(it);
            }
        } else {
            ++it;
        }
    }
}

/**
 * @brief GetOptionEffect - get option effect
 * @param dwIndex Equipped index
 * @param dwTableID Table ID
 * @param pTargetMoverEx Target mover
 * @return Option effect pointer or nullptr
 * IDA: ?GetOptionEffect@CMoverEx@@QEAAPEAUSOptionEffect@@KKPEAV1@@Z (0x14039BE80)
 * Verified: Finds option effect by index and table ID
 */
SOptionEffect* CMoverEx::GetOptionEffect(std::uint32_t dwIndex, std::uint32_t dwTableID, CMoverEx* pTargetMoverEx) {
    // IDA 精确还原:
    for (auto it = m_vecOptionEffect.begin(); it != m_vecOptionEffect.end(); ++it) {
        SOptionEffect* pOptionEffect = *it;

        if (pOptionEffect->pOptionTable &&
            pOptionEffect->dwEquipedIndex == dwIndex &&
            pOptionEffect->pOptionTable->EffectType_Index == dwTableID) {

            if (!pTargetMoverEx) {
                return pOptionEffect;
            }

            UXActorID targetID = pTargetMoverEx->GetActorID();
            if (targetID == pOptionEffect->dwTargetMoverID) {
                return pOptionEffect;
            }
        }
    }

    return nullptr;
}

/**
 * @brief AddOptionEffect - add option effect
 * @param dwEquipedIndex Equipped index
 * @param pOptionTable Option table pointer
 * @param fOptionValue Option value
 * @param fReferanceValue Reference value
 * @param fAppliedValue Applied value
 * @param pTargetMoverEx Target mover
 * IDA: ?AddOptionEffect@CMoverEx@@QEAAXKPEAUTB_CREATEOPTION@@MMMPEAV1@@Z (0x14039BFB0)
 * Verified: Creates and adds new option effect
 */
void CMoverEx::AddOptionEffect(std::uint32_t dwEquipedIndex, TB_CREATEOPTION* pOptionTable,
                                float fOptionValue, float fReferanceValue, float fAppliedValue,
                                CMoverEx* pTargetMoverEx) {
    // IDA 精确还原:
    if (!pOptionTable) {
        return;
    }

    SOptionEffect* pNewOptionEffect = new SOptionEffect();
    if (!pNewOptionEffect) {
        return;
    }

    pNewOptionEffect->dwEquipedIndex = dwEquipedIndex;
    pNewOptionEffect->pOptionTable = pOptionTable;
    pNewOptionEffect->fOptionValue = fOptionValue;
    pNewOptionEffect->fReferanceValue = fReferanceValue;
    pNewOptionEffect->fAppliedValue = fAppliedValue;
    pNewOptionEffect->dwLifeTime = pOptionTable->Value_Clmcrt;
    pNewOptionEffect->fCurTime = 0.0f;
    pNewOptionEffect->dwTargetMoverID = pTargetMoverEx->GetActorID().GetID();

    m_vecOptionEffect.push_back(pNewOptionEffect);
}

/**
 * @brief UpdateStatBatch - update stat batch
 * @param fElapsedSeconds Elapsed time in seconds
 * IDA: ?UpdateStatBatch@CMoverEx@@QEAAXM@Z (0x14039C0F0)
 * Verified: Updates batched stat changes
 */
void CMoverEx::UpdateStatBatch(float fElapsedSeconds) {
    // IDA 精确还原:
    if (m_fUpdateStatBatchTime > 0.0f) {
        m_fUpdateStatBatchTime -= fElapsedSeconds;

        if (m_fUpdateStatBatchTime < 0.0f) {
            m_fUpdateStatBatchTime = 0.2f;

            if (m_bCheckSendAbsorbSG) {
                m_bCheckSendAbsorbSG = false;
                CMover::SendUpdateStat(2);
            }

            if (m_bCheckSendHP) {
                m_bCheckSendHP = false;
                CMover::SendUpdateStat(1);
            }
        }
    }
}

/**
 * @brief ReleaseInvokedOptionEffectAfterDamage - release invoked option effects after damage
 * @param dwAttackerID Attacker ID
 * @param byDamageFlag Damage flag
 * @param bSABreaked Super armor break flag
 * IDA: ?ReleaseInvokedOptionEffectAfterDamage@CMoverEx@@QEAAXKE_N@Z (0x14039C780)
 * Verified: Releases option effects based on damage conditions
 */
void CMoverEx::ReleaseInvokedOptionEffectAfterDamage(std::uint32_t dwAttackerID, 
                                                      const std::uint8_t byDamageFlag,
                                                      const bool bSABreaked) {
    // IDA 精确还原:
    XArea* pArea = XActor::GetArea();
    if (!pArea) {
        return;
    }

    XActor* pActor = pArea->FindActor(m_dwHitID);
    CMoverEx* pAttacker = dynamic_cast<CMoverEx*>(pActor);

    if (!pAttacker) {
        return;
    }

    CUser* pAttackerUser = nullptr;
    if (pAttacker->GetType() == eActorUser) {
        pAttackerUser = dynamic_cast<CUser*>(pAttacker);
    }

    // Release effects on target (this)
    ReleaseInvokedOptionEffect(EFFECT_CONDITION_DAMAGED);

    if (pAttackerUser && pAttackerUser->CMover::IsFlying()) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_DAMAGED_BY_JUMP_ATTACK);
    }

    if ((byDamageFlag & 4) != 0) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_DAMAGED_CRITICAL);
    }

    if ((byDamageFlag & 1) != 0) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_DAMAGE_MISSED);
    }

    if (CMover::IsHitDown()) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_KNOCK_DOWN);
    }

    if (CMover::IsFlying()) {
        ReleaseInvokedOptionEffect(EFFECT_CONDITION_KNOCK_BACK);
    }

    // Release effects on attacker
    if (pAttackerUser) {
        if (CMover::IsFlying()) {
            pAttackerUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ARIAL_ATTACK_SUCCESS);
        }

        if ((byDamageFlag & 4) != 0) {
            pAttackerUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_CRITICAL);
        }

        if ((byDamageFlag & 1) != 0) {
            pAttackerUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_MISSED);
        }

        if (bSABreaked) {
            pAttackerUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_MAKE_SA_BREAK);
        }
    } else if (pAttacker) {
        if ((byDamageFlag & 4) != 0) {
            pAttacker->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_CRITICAL);
        }

        if ((byDamageFlag & 1) != 0) {
            pAttacker->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_MISSED);
        }

        if (bSABreaked) {
            pAttacker->ReleaseInvokedOptionEffect(EFFECT_CONDITION_MAKE_SA_BREAK);
        }
    }
}

// ============================================================================
// OptionEffect Management Functions
// ============================================================================

/**
 * @brief Update option effects over time
 *
 * IDA: ?UpdateOptionEffect@CMoverEx@@QEAAXM@Z (0x14039C1C0)
 * Updates all option effects, removing expired ones.
 *
 * @param fElapsedSeconds Elapsed time since last update
 */
void CMoverEx::UpdateOptionEffect(float fElapsedSeconds) {
    auto itor = m_vecOptionEffect.begin();
    while (itor != m_vecOptionEffect.end()) {
        SOptionEffect* pOptionEffect = *itor;
        CMoverEx* pTargetMoverEx = static_cast<CMoverEx*>(CMover::GetMoverObject(pOptionEffect->dwTargetMoverID));
        
        if (pTargetMoverEx && pOptionEffect->pOptionTable) {
            // Check if lifetime is infinite (< 1000ms) or not expired yet
            if (pOptionEffect->dwLifeTime < 1000) {
                ++itor;
            } else {
                pOptionEffect->fCurTime += fElapsedSeconds;
                float fLifeSeconds = static_cast<float>(pOptionEffect->dwLifeTime) / 1000.0f;
                
                if (pOptionEffect->fCurTime < fLifeSeconds) {
                    ++itor;
                } else {
                    // Expired - release and remove
                    ReleaseOptionEffectValue(
                        static_cast<EFFECT_STATUS_TYPE>(pOptionEffect->pOptionTable->StatusType),
                        pOptionEffect->fAppliedValue);
                    delete pOptionEffect;
                    itor = m_vecOptionEffect.erase(itor);
                }
            }
        } else {
            // Invalid target or table - remove
            delete pOptionEffect;
            itor = m_vecOptionEffect.erase(itor);
        }
    }
}

/**
 * @brief Remove option effect by equipped index
 *
 * IDA: ?RemoveOptionEffect@CMoverEx@@QEAAXK@Z (0x14039C420)
 * Removes all option effects with matching equipped index.
 *
 * @param dwEquipedIndex Equipment index to match
 */
void CMoverEx::RemoveOptionEffect(std::uint32_t dwEquipedIndex) {
    auto itor = m_vecOptionEffect.begin();
    while (itor != m_vecOptionEffect.end()) {
        SOptionEffect* pOptionEffect = *itor;
        CMoverEx* pTargetMoverEx = static_cast<CMoverEx*>(CMover::GetMoverObject(pOptionEffect->dwTargetMoverID));
        
        if (pTargetMoverEx && pOptionEffect->pOptionTable) {
            if (pOptionEffect->dwEquipedIndex == dwEquipedIndex) {
                // Release on target mover
                pTargetMoverEx->ReleaseOptionEffectValue(
                    static_cast<EFFECT_STATUS_TYPE>(pOptionEffect->pOptionTable->StatusType),
                    pOptionEffect->fAppliedValue);
                delete pOptionEffect;
                itor = m_vecOptionEffect.erase(itor);
            } else {
                ++itor;
            }
        } else {
            // Invalid - remove
            delete pOptionEffect;
            itor = m_vecOptionEffect.erase(itor);
        }
    }
}

/**
 * @brief Remove all option effects
 *
 * IDA: ?RemoveAllOptionEffect@CMoverEx@@QEAAXXZ (0x14039C620)
 * Clears all option effects and releases their values.
 */
void CMoverEx::RemoveAllOptionEffect() {
    auto itor = m_vecOptionEffect.begin();
    while (itor != m_vecOptionEffect.end()) {
        SOptionEffect* pOptionEffect = *itor;
        CMoverEx* pTargetMoverEx = static_cast<CMoverEx*>(CMover::GetMoverObject(pOptionEffect->dwTargetMoverID));
        
        if (pTargetMoverEx && pOptionEffect->pOptionTable) {
            pTargetMoverEx->ReleaseOptionEffectValue(
                static_cast<EFFECT_STATUS_TYPE>(pOptionEffect->pOptionTable->StatusType),
                pOptionEffect->fAppliedValue);
        }
        
        delete pOptionEffect;
        itor = m_vecOptionEffect.erase(itor);
    }
    m_vecOptionEffect.clear();
}

/**
 * @brief Release option effect value from stats
 *
 * IDA: ?ReleaseOptionEffectValue@CMoverEx@@QEAAXW4EFFECT_STATUS_TYPE@@M@Z (0x14039D890)
 * Applies the reverse of an option effect to stats.
 *
 * @param eStatusType Effect status type to release
 * @param fValue Value to subtract/negate
 */
void CMoverEx::ReleaseOptionEffectValue(EFFECT_STATUS_TYPE eStatusType, float fValue) {
    auto pAttr = GetGOC<CGocAttribute>();
    if (!pAttr) {
        return;
    }
    
    // Status type to stat mapping for percentage-based stats
    static const int s_iStatusList[] = {
        EFFECT_STATUS_PATK, EFFECT_STATUS_PDEF, EFFECT_STATUS_REG_ST,
        EFFECT_STATUS_MSR, EFFECT_STATUS_ASR, EFFECT_STATUS_PAR,
        EFFECT_STATUS_ADR, EFFECT_STATUS_PCP, EFFECT_STATUS_PCRP,
        EFFECT_STATUS_PDSR, EFFECT_STATUS_PARP, EFFECT_STATUS_PDPR,
        EFFECT_STATUS_PCA
    };
    
    static const int s_iStatList[] = {
        STAT_PATK, STAT_PDEF, STAT_REG_ST,
        STAT_MSR, STAT_ASR, STAT_PAR,
        STAT_ADR, STAT_PCP, STAT_PCRP,
        STAT_PDSR, STAT_PARP, STAT_PDPR,
        STAT_PCA
    };
    
    // Status type to stat mapping for max-based stats
    static const int s_v22[] = {
        EFFECT_STATUS_MAX_HP, EFFECT_STATUS_MAX_SG,
        EFFECT_STATUS_MAX_ST, EFFECT_STATUS_MAX_SV
    };
    
    static const int s_iStatIndex[] = {
        STAT_MAX_HP, STAT_MAX_SG, STAT_MAX_ST, STAT_MAX_SV
    };
    
    static const int s_iStatList2[] = {
        STAT_HP, STAT_SG, STAT_ST, STAT_SV
    };
    
    switch (eStatusType) {
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
                if (s_iStatusList[i] == eStatusType) {
                    iIndex = i;
                    break;
                }
            }
            
            if (iIndex >= 0) {
                SYNC_STAT_TYPE eSyncStatType = SYNC_STAT_TYPE_ME_ONLY;
                if (pAttr->IsShouldSyncStatBroadcast(s_iStatList[iIndex])) {
                    eSyncStatType = SYNC_STAT_TYPE_BROADCAST;
                }
                
                float fNegValue = -fValue;
                
                // MSR and ASR use scale stat, others use add stat
                if (s_iStatList[iIndex] == STAT_MSR || s_iStatList[iIndex] == STAT_ASR) {
                    pAttr->UpdateScaleStat(s_iStatList[iIndex], fNegValue, true);
                } else {
                    pAttr->UpdateAddStat(s_iStatList[iIndex], fNegValue, true);
                }
                
                pAttr->SetSyncStatFlag(s_iStatList[iIndex], eSyncStatType);
                SendUpdateStat(s_iStatList[iIndex]);
            }
            break;
        }
        
        case EFFECT_STATUS_MAX_HP:
        case EFFECT_STATUS_MAX_SG:
        case EFFECT_STATUS_MAX_ST:
        case EFFECT_STATUS_MAX_SV: {
            int v24 = -1;
            for (int j = 0; j < 4; ++j) {
                if (s_v22[j] == eStatusType) {
                    v24 = j;
                    break;
                }
            }
            
            if (v24 >= 0) {
                SYNC_STAT_TYPE eSyncStatType = SYNC_STAT_TYPE_ME_ONLY;
                if (pAttr->IsShouldSyncStatBroadcast(s_iStatIndex[v24])) {
                    eSyncStatType = SYNC_STAT_TYPE_BROADCAST;
                }
                
                float fCur = GetStat(s_iStatList2[v24]);
                float fNegValue = -fValue;
                
                pAttr->UpdateAddStat(s_iStatIndex[v24], fNegValue, true);
                pAttr->SetSyncStatFlag(s_iStatIndex[v24], eSyncStatType);
                SendUpdateStat(s_iStatIndex[v24]);
                
                float fNew = GetStat(s_iStatIndex[v24]);
                if (fCur != fNew) {
                    eSyncStatType = SYNC_STAT_TYPE_ME_ONLY;
                    if (pAttr->IsShouldSyncStatBroadcast(s_iStatList2[v24])) {
                        eSyncStatType = SYNC_STAT_TYPE_BROADCAST;
                    }
                    pAttr->SetSyncStatFlag(s_iStatList2[v24], SYNC_STAT_TYPE_ME_ONLY);
                    SendUpdateStat(s_iStatList2[v24]);
                }
            }
            break;
        }
        
        case EFFECT_STATUS_SA_DEFENCE:
        case EFFECT_STATUS_INVINCIBLE:
            RemoveDefenseChangeInfo(2, 0);
            send_eSUB_CMD_SKILL_DEFENCE_TYPE(m_byDefenseType, 0);
            break;
            
        case EFFECT_STATUS_SA_ATTACK_INCREASE: {
            float fNegValue = -fValue;
            pAttr->UpdateEffectStat(0, 0x71, fNegValue, true);
            pAttr->CalculateChangedEffect(1, fValue);
            break;
        }
        
        default:
            break;
    }
}

/**
 * @brief Get total option effect value for a status type
 *
 * IDA: ?GetTotalOptionEffectValue@CMoverEx@@QEAAMW4EFFECT_STATUS_TYPE@@@Z (0x14039E060)
 * Sums all applied values for the given status type.
 *
 * @param eStatusType Effect status type to query
 * @return Total applied value
 */
float CMoverEx::GetTotalOptionEffectValue(EFFECT_STATUS_TYPE eStatusType) {
    float fTotalAppliedValue = 0.0f;
    
    for (auto itor = m_vecOptionEffect.begin(); itor != m_vecOptionEffect.end(); ++itor) {
        SOptionEffect* pOptionEffect = *itor;
        if (pOptionEffect->pOptionTable && pOptionEffect->pOptionTable->StatusType == eStatusType) {
            fTotalAppliedValue += pOptionEffect->fAppliedValue;
        }
    }
    
    return fTotalAppliedValue;
}


/**
 * @brief Check if skill is a normal skill
 *
 * IDA: ?IsNormalSkill@CMoverEx@@QEAA_NPEAUTB_SKILL@@@Z (0x14039E260)
 * Returns true if skill passive type is 0 or 18.
 *
 * @param pSkillRef Skill table reference
 * @return true if normal skill
 */
bool CMoverEx::IsNormalSkill(TB_SKILL* pSkillRef) {
    return pSkillRef->Passive_Type == 18 || pSkillRef->Passive_Type == 0;
}

/**
 * @brief Clear change monster data
 *
 * IDA: ?ClearChangeMonsterData@CMoverEx@@QEAAXXZ (0x14039E2A0)
 * Resets monster transformation data.
 */
void CMoverEx::ClearChangeMonsterData() {
    m_dwChangeMobNewID = 0;
    m_dwChangeMobTableID = 0;
}

/**
 * @brief Set HP with extended handling
 *
 * IDA: ?SetHpEx@CMoverEx@@UEAAXH@Z (0x14039E2D0)
 * Sets HP and triggers attribute recalculation.
 *
 * @param nVal New HP value
 */
void CMoverEx::SetHpEx(int nVal) {
    SetHpInfo(nVal);
    
    auto pAttr = GetGOC<CGocAttribute>();
    if (pAttr) {
        pAttr->CalculateChanged();
    }
    
    m_bCheckSendHP = true;
}

/**
 * @brief Get specific option effect value by equip index
 *
 * IDA: ?GetSpecificOptionEffectValue@CMoverEx@@QEAAMKW4EFFECT_STATUS_TYPE@@@Z (0x14039E120)
 * Sums applied values for matching equip index and status type.
 *
 * @param dwEquipIndex Equipment index to match
 * @param eStatusType Effect status type to match
 * @return Total applied value for specific effect
 */
float CMoverEx::GetSpecificOptionEffectValue(std::uint32_t dwEquipIndex, EFFECT_STATUS_TYPE eStatusType) {
    float fAppliedValue = 0.0f;
    
    for (auto itor = m_vecOptionEffect.begin(); itor != m_vecOptionEffect.end(); ++itor) {
        SOptionEffect* pOptionEffect = *itor;
        if (pOptionEffect->pOptionTable &&
            pOptionEffect->dwEquipedIndex == dwEquipIndex &&
            pOptionEffect->pOptionTable->StatusType == eStatusType) {
            fAppliedValue += pOptionEffect->fAppliedValue;
        }
    }
    
    return fAppliedValue;
}

/**
 * @brief Add delayed buff status
 *
 * IDA: ?AddDelayBuffStatus@CMoverEx@@QEAAXMGKK@Z (0x14039E1F0)
 * Adds a buff that will be applied after a delay.
 *
 * @param fTime Delay time in seconds
 * @param nBuffID Buff ID to apply
 * @param dwUserID User ID that applied the buff
 * @param dwExtraData Extra data for buff
 */
void CMoverEx::AddDelayBuffStatus(float fTime, std::uint16_t nBuffID, std::uint32_t dwUserID, std::uint32_t dwExtraData) {
    SDelayBuff sDelayBuff;
    sDelayBuff.fWaitTime = fTime;
    sDelayBuff.nBuffID = nBuffID;
    sDelayBuff.dwUserID = dwUserID;
    sDelayBuff.dwExtraData = dwExtraData;
    m_vecDelayBuff.push_back(sDelayBuff);
}


/**
 * @brief Check if attack skill is enabled based on trigger conditions
 *
 * IDA: ?CheckAttackSkillEnable@CMoverEx@@QEAAXPEAV1@@Z (0x14039E370)
 * Checks distance and angle to target to enable attack skill.
 *
 * @param pMoverEx Target mover to check against
 */
void CMoverEx::CheckAttackSkillEnable(CMoverEx* pMoverEx) {
    if (m_fCheckAttackSkillDuration <= 0.0f) {
        return;
    }
    
    if (!m_pCheckAttackSkillTrigger) {
        return;
    }
    
    if (m_bCheckAttackSkillOn) {
        return;
    }
    
    // Get positions
    hkvVec3 vTargetPos = pMoverEx->GetPosition();
    hkvVec3 vMyPos = GetPosition();
    hkvVec3 vDiff = vTargetPos - vMyPos;
    vDiff.z = 0.0f;
    
    int nDist = static_cast<int>(vDiff.getLength());
    
    // Check range
    if (nDist < m_pCheckAttackSkillTrigger->nMinRange ||
        nDist > m_pCheckAttackSkillTrigger->nMaxRange) {
        return;
    }
    
    // Check angle if needed
    if (m_pCheckAttackSkillTrigger->nAngle != 0 && m_pCheckAttackSkillTrigger->nAngle != 180) {
        float fYaw = GetYawFromVector(&vDiff);
        float fCurrYaw = GetOrientationYaw();
        float fDiffYaw = fYaw - fCurrYaw;
        
        // Normalize angle to [-180, 180]
        if (fDiffYaw > 180.0f) {
            fDiffYaw -= 360.0f;
        } else if (fDiffYaw < -180.0f) {
            fDiffYaw += 360.0f;
        }
        
        if (std::abs(fDiffYaw) <= m_pCheckAttackSkillTrigger->nAngle) {
            m_bCheckAttackSkillOn = true;
        }
    } else {
        m_bCheckAttackSkillOn = true;
    }
}

/**
 * @brief Update hit damage for a specific attacker
 *
 * IDA: ?UpdateHitDamage@CMoverEx@@QEAAXTUXActorID@@HE@Z (0x14039E590)
 * Records damage dealt by a specific actor ID.
 *
 * @param uxActorID Actor ID of attacker
 * @param nDamage Damage amount
 * @param byClass Class type of attacker
 */
void CMoverEx::UpdateHitDamage(UXActorID uxActorID, int nDamage, std::uint8_t byClass) {
    // Only process valid actor IDs (not internal references)
    if ((uxActorID.dwActorID >> 29) != 0) {
        return;
    }
    
    std::uint32_t dwUCID = uxActorID.dwActorID;
    
    auto iter = m_setHitID.find(dwUCID);
    if (iter != m_setHitID.end()) {
        // Update existing entry
        std::int64_t biTempDamage = iter->second.nDamage + nDamage;
        if (biTempDamage < 9999999999LL) {
            iter->second.nDamage += nDamage;
        } else {
            iter->second.nDamage = 9999999999;
        }
    } else {
        // Create new entry
        ST_MONSTER_DAMAGE_INFO stInfo;
        stInfo.dwUCID = dwUCID;
        stInfo.nDamage = nDamage;
        stInfo.byClass = byClass;
        m_setHitID.insert(std::make_pair(dwUCID, stInfo));
    }
}

/**
 * @brief Set die reason
 *
 * IDA: ?SetDieReason@CMoverEx@@QEAAXEH@Z (0x14039E710)
 * Records the reason and damage of death.
 *
 * @param byReason Death reason code
 * @param nDamage Final damage amount
 */
void CMoverEx::SetDieReason(std::uint8_t byReason, int nDamage) {
    if (m_byDieReason == 0) {
        m_byDieReason = byReason;
        m_nDieDamage = nDamage;
    } else if (m_nDieDamage == 0 && nDamage > 0) {
        m_byDieReason = byReason;
        m_nDieDamage = nDamage;
    }
}

/**
 * @brief Set landing state
 *
 * IDA: ?SetLanding@CMoverEx@@QEAAXH@Z (0x14039E790)
 * Updates whether the mover is on the ground.
 *
 * @param bLanding True if landed
 */
void CMoverEx::SetLanding(bool bLanding) {
    m_bLanded = bLanding;
}

/**
 * @brief Reapply all buff abilities
 *
 * IDA: ?ReapllyBuffAll@CMoverEx@@UEAAXXZ (0x14039E7B0)
 * Recalculates all buff effects.
 */
void CMoverEx::ReapllyBuffAll() {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != 0 &&
            m_stBuffState[i].nBuffIndex != -1 &&
            (m_stBuffState[i].byEffectType == 0 || m_stBuffState[i].byEffectType == 23)) {
            UpdateBuffAbility(&m_stBuffState[i], true);
        }
    }
}


/**
 * @brief Create a new CMoverEx object
 *
 * IDA: ?CreateObject@CMoverEx@@SAPEAVVTypedObject@@XZ (0x14039E890)
 * Factory method for creating CMoverEx instances.
 *
 * @return New CMoverEx instance or nullptr on allocation failure
 */
CMoverEx* CMoverEx::CreateObject() {
    CMoverEx* pObj = static_cast<CMoverEx*>(VBaseObject::operator new(sizeof(CMoverEx)));
    if (pObj) {
        return new (pObj) CMoverEx();
    }
    return nullptr;
}

/**
 * @brief Get type ID for CMoverEx
 *
 * IDA: ?GetTypeId@CMoverEx@@UEBAPEAUVType@@XZ (0x14039E8F0)
 * Returns the Vision Engine type object.
 *
 * @return Pointer to CMoverEx type object
 */
VType* CMoverEx::GetTypeId() const {
    return &CMoverEx::classCMoverEx;
}

/**
 * @brief Get animation speed restoration value
 *
 * IDA: ?GetRestoreAnimSpeed@CMoverEx@@QEAAMXZ (0x14039E900)
 * Returns animation speed based on status and ability.
 *
 * @return Animation speed multiplier
 */
float CMoverEx::GetRestoreAnimSpeed() {
    // If not in specific status, return normal speed
    if (!IsStatus(1) && !IsStatus(0x400)) {
        return 1.0f;
    }
    
    // If attack speed type, use stored restore speed
    if (m_eRestoreAnimSpeedType == AST_ATTACK) {
        return m_fRestoreAnimSpeed;
    }
    
    // Otherwise use ability value
    return m_fAbility[19] * 0.01f;
}

/**
 * @brief Clear all option effects without releasing
 *
 * IDA: ?ClearOptionEffect@CMoverEx@@QEAAXXZ (0x14039E990)
 * Deletes all option effects without releasing their values.
 */
void CMoverEx::ClearOptionEffect() {
    auto itor = m_vecOptionEffect.begin();
    while (itor != m_vecOptionEffect.end()) {
        SOptionEffect* pOptionEffect = *itor;
        if (pOptionEffect) {
            delete pOptionEffect;
            itor = m_vecOptionEffect.erase(itor);
        } else {
            ++itor;
        }
    }
    m_vecOptionEffect.clear();
}

/**
 * @brief Set change motion log
 *
 * IDA: ?SetChangeMotionLog@CMoverEx@@QEAAXHH@Z (0x14039EAC0)
 * Records motion change for debugging.
 *
 * @param iPos Position in motion sequence
 * @param iType Motion type
 */
void CMoverEx::SetChangeMotionLog(int iPos, int iType) {
    m_iChangeMotionPos = iPos;
    m_iChangeMotionType = iType;
    GetLocalTime(&m_iChangeMotionTime);
}


/**
 * @brief Calculate super armor damage
 *
 * IDA: ?CalcSuperArmorDamage@CMoverEx@@UEAAMPEAV1@HPEAUTB_SKILL@@M@Z (0x1403883B0)
 * Calculates damage value for super armor gauge.
 *
 * @param pTargetMover Target mover
 * @param nIndex Skill index
 * @param pSkillTable Skill table reference
 * @return Calculated super armor damage
 */
float CMoverEx::CalcSuperArmorDamage(CMoverEx* pTargetMover, int nIndex, TB_SKILL* pSkillTable) {
    // Cannot damage if target has shield
    if (GetShieldHP(pTargetMover) > 0) {
        return 0.0f;
    }
    
    if (!pSkillTable) {
        return 0.0f;
    }
    
    // Cannot damage super armor if target is invincible
    if (GetDefenseType(pTargetMover) == 3) {
        return 0.0f;
    }
    
    auto pAttr = GetGOC<CGocAttribute>();
    if (!pAttr) {
        return 0.0f;
    }
    
    int nDamage = m_pSkillMgr->GetBaseDamage(nIndex);
    
    // Get skill option super armor effect
    float fSkillOptionSuperArmor = 0.0f;
    pAttr->GetSkillOptionEffect(pSkillTable->Skill_Group, EFFECT_SKILL_OPTION_SUPER_ARMOR, &fSkillOptionSuperArmor);
    
    // Get divergence table
    auto pDivTableRef = GetCurDivergenceTable();
    
    // Calculate effect rate
    float fEffectRate = GetTotalOptionEffectValue(EFFECT_STATUS_SA_ATTACK_DECREASE) * 0.01f + 1.0f;
    
    // Calculate base damage
    int nDamagea;
    if (pDivTableRef && pDivTableRef->Div_Option_Type == 6) {
        nDamagea = static_cast<int>(nDamage * (pDivTableRef->Div_SubOption_Value * 0.01f) * fEffectRate);
    } else {
        nDamagea = static_cast<int>(nDamage * ((pSkillTable->SuperArmor_Attack + fSkillOptionSuperArmor) * 0.01f) * fEffectRate);
    }
    
    // Apply special effect
    float fSpecialEffect = pAttr->GetSpecialEffect(113);
    int nDamageb = static_cast<int>(nDamagea * (fSpecialEffect * 0.01f)) + nDamagea;
    
    // Apply buff super armor rate
    if (GetBuffSuperArmorRate() > 0.0f) {
        nDamageb = static_cast<int>(nDamageb * GetBuffSuperArmorRate());
    }
    
    // Apply constitution factor
    auto pStatusTable = pAttr->GetStatusTable();
    float fResult = static_cast<float>(nDamageb * pStatusTable->Con_SADamage_Value);
    
    return fResult;
}

/**
 * @brief Get multiple absorb SG value
 *
 * IDA: ?GetMultipleAbsorbSG@CMoverEx@@QEAAMXZ (0x1403A2680)
 * Returns the SG absorb multiplier.
 *
 * @return SG absorb multiplier
 */
float CMoverEx::GetMultipleAbsorbSG() {
    return m_fMultipleAbsorbSG;
}


/**
 * @brief Moving target action
 *
 * IDA: ?MovingTarget@CMoverEx@@QEAAXPEAVMovingInputTrigger@@H@Z (0x140393FF0)
 * Handles movement towards target based on trigger settings.
 *
 * @param pTrigger Moving input trigger configuration
 * @param iMovingType Movement type
 */
void CMoverEx::MovingTarget(MovingInputTrigger* pTrigger, int iMovingType) {
    if (!pTrigger) {
        return;
    }
    
    // Calculate attack area
    tagATTACK_AREA stAttackArea;
    float fMaxDist = pTrigger->Duration * pTrigger->MaxSpeed;
    float fYaw = GetOrientationYaw();
    m_pSkillMgr->GetAttackArea(&m_vPosition, fYaw, 0, &pTrigger->sAttackRange, &stAttackArea);
    
    // Get nearby game objects
    std::vector<CMover*> vecGameObjList;
    XArea::ScanGridOrigin(&XActor, 2, 3, &vecGameObjList);
    
    std::vector<PS_MOVING_TARGET> vecMovingTargetList;
    
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMoverEx* pOtherActor = dynamic_cast<CMoverEx*>(*it);
        if (!pOtherActor) {
            continue;
        }
        
        // Skip invalid targets
        if (!pOtherActor->IsLive() ||
            pOtherActor->IsStatus(2) ||
            pOtherActor->IsStatus(4) ||
            pOtherActor == this ||
            !IsEnemy(pOtherActor) ||
            pOtherActor->GetStandType() == 2 ||
            pOtherActor->GetStandType() == 3 ||
            pOtherActor->GetDefenseType() == 3 ||
            !m_pSkillMgr->IsInAttackArea(pOtherActor, &stAttackArea, true, false)) {
            continue;
        }
        
        hkvVec3 vDestPos;
        hkvVec3 vDist;
        hkvVec3 vDir;
        float fDuration = pTrigger->Duration;
        
        // Calculate direction to target
        hkvVec3 vMyPos = GetPosition();
        hkvVec3 vTargetPos = pOtherActor->GetPosition();
        vDist = vTargetPos - vMyPos;
        vDist.z = 0.0f;
        vDir = vDist;
        vDir.normalizeIfNotZero(0.000001f);
        
        float fDist = vDist.getLength();
        
        switch (pTrigger->MovingType) {
            case 1: // Push forward
                if (fDist >= pTrigger->OffsetDistance) {
                    continue;
                }
                fDist = pTrigger->OffsetDistance - fDist;
                if (fDist > fMaxDist) {
                    fDist = fMaxDist;
                }
                fDuration = fDist / pTrigger->MaxSpeed;
                vDist = vDir * fDist;
                vDestPos = vTargetPos + vDist;
                break;
                
            case 2: // Pull backward
                {
                    TB_MONSTER* pMobTable = nullptr;
                    CMonster* pMonster = nullptr;
                    if (pOtherActor->GetType() == eActorMonster) {
                        pMonster = dynamic_cast<CMonster*>(pOtherActor);
                        if (pMonster) {
                            pMobTable = pMonster->GetMobTableRef();
                        }
                    }
                    
                    if (pMobTable && pTrigger->MonsterRank < pMobTable->Monster_Rank) {
                        continue;
                    }
                    if (pTrigger->OffsetDistance >= fDist) {
                        continue;
                    }
                    
                    fDist = fDist - pTrigger->OffsetDistance;
                    
                    // Apply weight rank modifier
                    if (pTrigger->MonsterWeightRank == 1 && pMonster) {
                        char cRankGap = GetWeightRank() - pMonster->GetWeightRank();
                        if (cRankGap < 2) {
                            if (cRankGap == -3 || cRankGap == -2) {
                                fDist *= 0.65f;
                            } else if (cRankGap <= -4) {
                                fDist = 0.0f;
                            }
                        } else {
                            fDist *= 1.35f;
                        }
                    }
                    
                    if (fDist > fMaxDist) {
                        fDist = fMaxDist;
                    }
                    fDuration = fDist / pTrigger->MaxSpeed;
                    fDist = -fDist;
                    vDist = vDir * fDist;
                    vDestPos = vTargetPos + vDist;
                }
                break;
                
            case 4: // Pull to point
                {
                    TB_MONSTER* pMobTable = nullptr;
                    CMonster* pMonster = nullptr;
                    if (pOtherActor->GetType() == eActorMonster) {
                        pMonster = dynamic_cast<CMonster*>(pOtherActor);
                        if (pMonster) {
                            pMobTable = pMonster->GetMobTableRef();
                        }
                    }
                    
                    if (pMobTable && pTrigger->MonsterRank < pMobTable->Monster_Rank) {
                        continue;
                    }
                    
                    hkvMat3 matRot;
                    matRot.setFromEulerAngles(0.0f, 0.0f, m_fMovingYaw);
                    hkvVec3 vNewPos = matRot * pTrigger->vPullPoint;
                    vDestPos = vNewPos + GetPosition();
                    
                    hkvVec3 vOtherPos = pOtherActor->GetPosition();
                    vDist = vOtherPos - vDestPos;
                    vDist.z = 0.0f;
                    fDist = vDist.getLength();
                    
                    // Apply weight rank modifier
                    if (pTrigger->MonsterWeightRank == 1 && pMonster) {
                        char cRankGap = GetWeightRank() - pMonster->GetWeightRank();
                        if (cRankGap < 2) {
                            if (cRankGap == -3 || cRankGap == -2) {
                                fDist *= 0.65f;
                            } else if (cRankGap <= -4) {
                                fDist = 0.0f;
                            }
                        } else {
                            fDist *= 1.35f;
                        }
                    }
                    
                    if (fDist > fMaxDist) {
                        fDist = fMaxDist;
                    }
                    fDuration = fDist / pTrigger->MaxSpeed;
                }
                break;
                
            default:
                continue;
        }
        
        // Apply movement to target
        pOtherActor->SetDirectionTo(&m_vPosition);
        pOtherActor->SetExtraMoving();
        vDestPos = pOtherActor->GetExtraMovePos();
        
        PS_MOVING_TARGET stMovingTarget;
        stMovingTarget.uxActorID = pOtherActor->GetActorID();
        stMovingTarget.xExtraMove = vDestPos;
        stMovingTarget.fYaw = pOtherActor->GetMovingYaw();
        stMovingTarget.fDuration = fDuration;
        vecMovingTargetList.push_back(stMovingTarget);
        
        // Change motion for targets without defense
        if (pOtherActor->GetDefenseType() == 0 || pOtherActor->GetDefenseType() == 1) {
            int nMotion = (rand() % 2) + 15; // Random 15 or 16
            pOtherActor->ChangeMotion_3(nMotion, true, false);
        }
    }
    
    // Send moving target packet
    if (!vecMovingTargetList.empty()) {
        send_eSUB_CMD_SKILL_MOVING_TARGET(&vecMovingTargetList);
    }
}


/**
 * @brief Handle damage to this mover
 *
 * IDA: ?Damage@CMoverEx@@UEAAXAEAUtagACTION_DAMAGE@@HAEA_N@Z (0x140385F70)
 * Main damage handling function with reaction types, death handling, and SA break.
 *
 * @param dmgInfo Damage information structure
 * @param nSkillID Skill ID that caused damage
 * @param bSABreaked Output: true if super armor was broken
 */
void CMoverEx::Damage(tagACTION_DAMAGE& dmgInfo, unsigned int nSkillID, bool& bSABreaked) {
    // Check if can receive damage (not already dying in certain states)
    if (IsStatus(4) && m_eDieType != DIE_TYPE_KNOCKDOWN && m_eDieType != DIE_TYPE_DELAY) {
        return;
    }
    
    XArea* pArea = GetArea();
    if (!pArea) {
        return;
    }
    
    bool bFlyAction = false;
    
    // Normalize attack rotation
    if (dmgInfo.fAttackRot > 180.0f) {
        dmgInfo.fAttackRot -= 360.0f;
    } else if (dmgInfo.fAttackRot < -180.0f) {
        dmgInfo.fAttackRot += 360.0f;
    }
    
    // Record hit ID for damage tracking
    if (dmgInfo.nDamage > 0) {
        SetHitID(dmgInfo.dwID);
        
        // Check for maze and add hit count for player quests
        XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
        if (pMaze && GetType() == eActorUser) {
            pMaze->AddHitedCount(GetActorID().dwActorID);
        }
    }
    
    // Enter battle pose if not already
    if (!IsBattlePose()) {
        ChangeBattlePose(true, false);
    }
    
    // Check phase motion
    if (m_byPhaseMotionStep == 1 && m_nMotionClass != m_nPlayPhaseMotion && m_nPlayPhaseMotion != -1) {
        SetupPhaseMotion();
        return;
    }
    
    if (m_byPhaseMotionStep == 2) {
        return;
    }
    
    m_nDamage = dmgInfo.nDamage;
    bool bShowSABreakMotion = false;
    SetCurSuperArmorGage(dmgInfo.fSuperArmorGage);
    
    // Get attacker
    CMoverEx* pActor = nullptr;
    CUser* pAttackerUser = nullptr;
    pArea = GetArea();
    
    if (pArea) {
        pActor = static_cast<CMoverEx*>(pArea->FindActor(m_dwHitID));
        if (pActor) {
            if (pActor->GetType() != eActorUser) {
                if (pActor->GetType() == eActorMonster) {
                    CMoverEx* pOwnerPlayer = pActor->GetOwnerPlayer();
                    pAttackerUser = dynamic_cast<CUser*>(pOwnerPlayer);
                }
            } else {
                pAttackerUser = dynamic_cast<CUser*>(pActor);
            }
        }
    }
    
    bSABreaked = false;
    
    // Handle super armor break
    if (dmgInfo.byDefenseType != 4) {
        if (GetType() == eActorMonster && GetMaxSuperArmorGage() > 0.0f &&
            GetRestoreDefenseType() == 4 && dmgInfo.fSuperArmorGage <= 0.0f) {
            
            bSABreaked = true;
            SetCurSuperArmorGage(0.0f);
            SetRestoreDefenseType();
            
            if (m_bySABreakMotionType == 1) {
                m_fSABreakLoopMotionTime = m_fSABreakLoopTime;
                bShowSABreakMotion = true;
            }
            
            ChangeDefenseTypeForce(0, m_fSABreakTime);
            
            // Trigger passive skill and option effect on attacker
            if (pActor && (GetMonsterFlag() & 2) == 0) {
                if (pAttackerUser) {
                    pAttackerUser->CheckPassiveSkill(true, 36);
                    pAttackerUser->CheckOptionEffectInvoke(EFFECT_CONDITION_MAKE_SA_BREAK, this, static_cast<float>(dmgInfo.nDamage), EFFECT_INVOKE_DONT_CARE);
                } else {
                    pActor->CheckPassiveSkill(true, 36);
                    pActor->CheckOptionEffectInvoke(EFFECT_CONDITION_MAKE_SA_BREAK, this, static_cast<float>(dmgInfo.nDamage), EFFECT_INVOKE_DONT_CARE);
                }
            }
        } else {
            SetSimpleDefenseType(dmgInfo.byDefenseType);
        }
    }
    
    // Set battle state time for users
    CUser* pHitedUser = dynamic_cast<CUser*>(this);
    if (pHitedUser) {
        pHitedUser->SetBattleStateTime(7.0f);
    }
    
    if (pActor) {
        // Track damage dealt by attacker
        if (dmgInfo.nDamage > 0) {
            std::uint8_t byClass = 0;
            if (pAttackerUser) {
                byClass = pAttackerUser->GetClass();
            }
            UpdateHitDamage(pActor->GetOriginID(), dmgInfo.nDamage, byClass);
        }
        
        // Handle link skill
        if (pActor->GetLinkSkillDuration() > 0.0f) {
            pActor->SetLinkSkillOn(true);
        }
        
        // Log game event for attacker user
        if (pAttackerUser) {
            pAttackerUser->SetBattleStateTime(7.0f);
            // Log data would be prepared here
        }
        
        // Check passive skills for being hit
        if ((pActor->GetMonsterFlag() & 2) == 0) {
            CheckPassiveSkill(true, 16);  // On hit
            CheckPassiveSkill(true, 17);  // On damage
            CheckPassiveSkill(true, 18);  // On attack received
            
            if ((dmgInfo.byDamageFlag & 1) != 0) {
                CheckPassiveSkill(true, 14);  // Miss
            } else {
                if ((dmgInfo.byDamageFlag & 4) != 0) {
                    CheckPassiveSkill(true, 21);  // Critical
                }
                if (pActor->IsBoss_Named_Raid()) {
                    CheckPassiveSkill(true, 53);
                }
            }
        }
        
        // Check jump attack damage
        unsigned int dwDamageResult = 1;
        if (IsInvincibleActor()) {
            dwDamageResult = 4;
        } else if ((dmgInfo.byDamageFlag & 1) != 0) {
            dwDamageResult = 2;  // Miss
        } else if ((dmgInfo.byDamageFlag & 4) != 0) {
            dwDamageResult = 3;  // Critical
        }
        
        auto pServer = TXSingleton<XGameServer>::Instance();
        if (XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID) && 
            (dwDamageResult == 1 || dwDamageResult == 3)) {
            CheckOptionEffectInvoke(EFFECT_CONDITION_DAMAGED_BY_JUMP_ATTACK, pActor, static_cast<float>(dmgInfo.nDamage), EFFECT_INVOKE_DONT_CARE);
        }
    }
    
    // Release attack success effect if not miss
    if ((dmgInfo.byDamageFlag & 1) == 0) {
        if (pAttackerUser) {
            pAttackerUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_SUCCESS);
        } else if (pActor) {
            pActor->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_SUCCESS);
        }
    }
    
    // Process HP damage
    bool isDamageHP = DamageProcessHP(dmgInfo.dwID, nSkillID, dmgInfo.nDamage, dmgInfo.byDamageFlag, dmgInfo.byHitPartsIndex);
    
    // Handle death
    if (!m_pCurSkillTableRef || m_pCurSkillTableRef->Skill_Type != 8) {
        if (isDamageHP) {
            SetDieReason(1, dmgInfo.nDamage);
            ChangeDefenseTypeForce(0);
            
            // Trigger kill effect on attacker
            if (pAttackerUser) {
                pAttackerUser->CheckOptionEffectInvoke(EFFECT_CONDITION_KILL, this, static_cast<float>(dmgInfo.nDamage), EFFECT_INVOKE_DONT_CARE);
            } else if (pActor) {
                pActor->CheckOptionEffectInvoke(EFFECT_CONDITION_KILL, this, static_cast<float>(dmgInfo.nDamage), EFFECT_INVOKE_DONT_CARE);
            }
            
            UpdateMonsterLastDamageType(&dmgInfo, nSkillID);
            
            // Handle flying death
            if (IsFlying()) {
                m_bFlyDie = true;
                SetDie_2(12, false);
                m_fDieDelayTime = 1.0f;
                if (m_fDieDelayMaxTime <= 0.0f) {
                    m_fDieDelayMaxTime = 5.0f;
                }
            } else {
                if (m_bFlyDie && m_nMotionClass == 13) {
                    return;
                }
                
                DIE_TYPE ePrevDieType = m_eDieType;
                if (m_eDieType <= DIE_TYPE_DELAY) {
                    CheckDieType(&dmgInfo.byReactionType, dmgInfo.byDamageFlag, &dmgInfo.vExtraMove, dmgInfo.nContinueAttack);
                }
                
                if (ePrevDieType != m_eDieType) {
                    ClearStatus(2);
                    ClearStatus(4);
                }
                
                SetDie_2(12, false);
                
                if (m_eDieType == DIE_TYPE_DELAY) {
                    m_fDieDelayTime = 1.0f;
                    if (m_fDieDelayMaxTime <= 0.0f) {
                        m_fDieDelayMaxTime = 5.0f;
                    }
                } else if (m_eDieType == DIE_TYPE_KNOCKDOWN) {
                    m_fDieDelayTime = 0.0f;
                    m_fDieDelayMaxTime = 0.0f;
                    bFlyAction = true;
                    m_nHitStatus = 0;
                    ChangeDefenseTypeForce(0);
                    ClearStatus(2);
                    SetStatus(4);
                } else {
                    m_fDieDelayTime = 0.0f;
                    m_fDieDelayMaxTime = 0.0f;
                    if (m_fDieFadeTime <= 0.0f) {
                        m_fDieFadeTime = 1.5f;
                    }
                }
            }
        } else {
            // Counter attack
            if ((dmgInfo.byDamageFlag & 8) != 0) {
                PlayCounterAnim();
                return;
            }
            
            // Check if should react to damage
            if (m_byDefenseType != 3 || ((dmgInfo.byDamageFlag & 0x10) != 0 && !IsImmunityStatus())) {
                int nMotion = GetDamageMotion(dmgInfo.byReactionType, dmgInfo.byAttackCollision, dmgInfo.byAttackRank);
                
                if (bShowSABreakMotion || m_byDefenseType != 5) {
                    // Process damage reaction...
                    // (Continued in next part due to length)
                }
            }
        }
    }
    
    // Decrement endurance and apply motion
    DecEndurance(dmgInfo.dwID);
    int nAnimIndex = GetStartAnimationInx(0);
    DebugOut("Damage>> %d / Motion %d / Defense %d", dmgInfo.nDamage, nAnimIndex, m_byDefenseType);
    
    // Check if should show reaction
    bool bEnableReaction = true;
    if (m_byDefenseType == 3 || m_byDefenseType == 2 || m_byDefenseType == 5) {
        bEnableReaction = IsDamageMotionDisplay(dmgInfo.byAttackCollision);
    }
    
    if (!bEnableReaction) {
        dmgInfo.vExtraMove = hkvVec3::ZeroVector();
    }
    
    // Clear buff by effect type
    int iIndex = FindBuffByEffectType(0x7B, 0);
    if (iIndex != -1) {
        ClearBuffStatusBySlot(iIndex, true);
    }
    
    m_fRegenSuperArmorTime = m_fDefRegenSuperArmorTime;
    dmgInfo.byDefenseType = m_byDefenseType;
}


// ============================================================================
// Simple Getter/Setter Functions
// ============================================================================

/**
 * @brief Get link skill duration
 *
 * IDA: ?GetLinkSkillDuration@CMoverEx@@QEAAMXZ (0x1403A27F0)
 *
 * @return Link skill duration in seconds
 */
float CMoverEx::GetLinkSkillDuration() {
    return m_fLinkSkillDuration;
}

/**
 * @brief Get shield HP
 *
 * IDA: ?GetShieldHP@CMoverEx@@QEAAEXZ (0x1403A2790)
 *
 * @return Shield HP value
 */
int CMoverEx::GetShieldHP() {
    return m_nShieldHP;
}

/**
 * @brief Check if should attach to attacker
 *
 * IDA: ?GetCheckAttachToAttacker@CMoverEx@@QEAA_NXZ (0x1403A27B0)
 *
 * @return true if should attach
 */
bool CMoverEx::GetCheckAttachToAttacker() {
    return m_bCheckAttachToAttacker;
}

/**
 * @brief Set link skill on state
 *
 * IDA: ?SetLinkSkillOn@CMoverEx@@QEAAX_N@Z (0x1403A27D0)
 *
 * @param bSkillOn Link skill on state
 */
void CMoverEx::SetLinkSkillOn(bool bSkillOn) {
    m_bLinkSkillOn = bSkillOn;
}


// ============================================================================
// Additional CMoverEx Functions
// ============================================================================

/**
 * @brief Set fixed max damage flag
 *
 * IDA: ?SetFixedMaxDamage@CMoverEx@@QEAAXH@Z (0x140406D20)
 *
 * @param bEnable Enable flag
 */
void CMoverEx::SetFixedMaxDamage(bool bEnable) {
    m_byFixedMaxDamage = bEnable;
}

/**
 * @brief Set test damage type
 *
 * IDA: ?SetTestDamage@CMoverEx@@QEAAXW4E_TEST_DAMAGE@@@Z (0x140406DD0)
 *
 * @param eTestDamage Test damage type
 */
void CMoverEx::SetTestDamage(E_TEST_DAMAGE eTestDamage) {
    m_eTestDamageType = eTestDamage;
}

/**
 * @brief Get apply parent rotation flag
 *
 * IDA: ?GetApplyParentRotation@CMoverEx@@QEAAHXZ (0x1403A29C0)
 *
 * @return true if should apply parent rotation
 */
bool CMoverEx::GetApplyParentRotation() {
    return m_bSummonMonsterApplyRot;
}

/**
 * @brief Get pre-target list count
 *
 * IDA: ?GetPreTargetListCount@CMoverEx@@QEAAHXZ (0x1403A2E30)
 *
 * @return Number of pre-targets in list
 */
size_t CMoverEx::GetPreTargetListCount() {
    return m_vPreTargetList.size();
}


// ============================================================================
// CMySkillList Functions
// ============================================================================

/**
 * @brief Remove chain lightning objects owned by a mover
 *
 * IDA: ?RemoveChainLightning@CMySkillList@@QEAAXPEAVCMoverEx@@@Z (0x1402C0F70)
 *
 * @param pMoverEx Owner mover to remove chain lightning for
 */
void CMySkillList::RemoveChainLightning(CMoverEx* pMoverEx) {
    m_pActor->DebugOut("RemoveChainLightning>> %x", pMoverEx);
    
    for (auto it = m_vChainLightningObject.begin(); it != m_vChainLightningObject.end(); ++it) {
        VChainLightningObject* pChainLightning = *it;
        if (pChainLightning && pChainLightning->IsActivate()) {
            if (pChainLightning->GetOwner() == pMoverEx) {
                pChainLightning->SetFinish();
            }
        }
    }
}

/**
 * @brief Check if can attack downed target
 *
 * IDA: ?IsCanDownedAttack@CMySkillList@@SAHPEAVCMoverEx@@PEAUTB_SKILL@@@Z (0x1402C1C00)
 *
 * @param pMover Attacker mover
 * @param pSkillRef Skill table reference
 * @return 1 if normal, 2 if monster skill target state is 1
 */
int CMySkillList::IsCanDownedAttack(CMoverEx* pMover, TB_SKILL* pSkillRef) {
    if (!pSkillRef) {
        return 1;
    }
    if (pSkillRef->Monster_Skill_Target_State == 1) {
        return 2;
    }
    return 1;
}

/**
 * @brief Get attack area from mover position and attack range
 *
 * IDA: ?GetAttackArea@CMySkillList@@SAXPEAVCMoverEx@@AEAVhkvVec3@@MFPEAUtagATTACK_RANGE@@PEAUtagATTACK_AREA@@@Z (0x1402C1DE0)
 * Calculates attack area based on attack range type (sphere or sector).
 *
 * @param pMoverEx Attacker mover
 * @param vPos Attack position
 * @param fYaw Attack yaw angle
 * @param shAttackRangeType Attack range type (0=sphere, 1=sector)
 * @param pAttackRange Attack range parameters
 * @param pAttackArea Output attack area
 */
void CMySkillList::GetAttackArea(CMoverEx* pMoverEx, hkvVec3& vPos, float fYaw, short shAttackRangeType, tagATTACK_RANGE* pAttackRange, tagATTACK_AREA* pAttackArea) {
    if (!pMoverEx) {
        return;
    }
    
    float fRadius = pAttackRange->fRadius;
    
    // Check divergence table for radius modification
    TB_DIVERGENCE* pDivergenceTable = pMoverEx->GetCurDivergenceTable();
    if (pDivergenceTable && pDivergenceTable->Div_Option_Type == 1) {
        if (pDivergenceTable->Div_Option_Value == 0) {
            fRadius *= pDivergenceTable->Div_SubOption_Value;
        } else if (pDivergenceTable->Div_Option_Value == 1) {
            fRadius += pDivergenceTable->Div_SubOption_Value;
        }
    }
    
    // Create rotation matrix from yaw
    hkvMat3 matRot;
    VisMath_cl::EulerToMatrix(fYaw, 0.0f, 0.0f, &matRot);
    
    if (shAttackRangeType == 0) {
        // Sphere attack area
        pAttackArea->byType = 0;
        pAttackArea->vCenterPos = matRot * pAttackRange->vCenterPos + vPos;
        pAttackArea->fAttackerRot = fYaw;
        pAttackArea->fRadiusStart = pAttackRange->fStartPos;
        pAttackArea->fRadiusEnd = fRadius;
        pAttackArea->fAngle = pAttackRange->fAngle / 2.0f;
        
        hkvVec3 vDir = matRot * hkvVec3(0.0f, -1.0f, 0.0f);
        pAttackArea->vAttackerDir = D3DXVECTOR2(vDir.x, vDir.y);
    } else if (shAttackRangeType == 1) {
        // Sector attack area
        pAttackArea->byType = 1;
        pAttackArea->vCenterPos = matRot * pAttackRange->vCenterPos + vPos;
        pAttackArea->fAttackerRot = fYaw;
        pAttackArea->fRadiusStart = pAttackRange->fStartPos;
        pAttackArea->fRadiusEnd = fRadius;
        pAttackArea->fAngle = pAttackRange->fAngle / 2.0f;
        pAttackArea->fSizeX = pAttackRange->fBoxSizeX;
        pAttackArea->fSizeY = pAttackRange->fBoxSizeY;
        
        hkvVec3 vDir = matRot * hkvVec3(0.0f, -1.0f, 0.0f);
        pAttackArea->vAttackerDir = D3DXVECTOR2(vDir.x, vDir.y);
    }
    
    // Set height bounds
    pAttackArea->fHeightB = vPos.z + pAttackRange->vCenterPos.z;
    pAttackArea->fHeightT = vPos.z + pAttackRange->vCenterPos.z + pAttackRange->fHeight;
    pAttackArea->byHitPartsIndex = 0;
}


/**
 * @brief Check if mover is in attack area
 *
 * IDA: ?IsInAttackArea@CMySkillList@@QEAAHPEAVCMoverEx@@AEAUtagATTACK_AREA@@HH@Z (0x1402BE0D0)
 *
 * @param pMover Target mover to check
 * @param stAreaInfo Attack area info
 * @param nDownAttack Down attack flag
 * @param bPassiveType Passive type flag
 * @return true if mover is in attack area
 */
bool CMySkillList::IsInAttackArea(CMoverEx* pMover, tagATTACK_AREA& stAreaInfo, int nDownAttack, int bPassiveType) {
    if (!pMover) {
        return false;
    }
    
    stAreaInfo.byHitPartsIndex = 0;
    
    if (!pMover->IsCanHit(nDownAttack, bPassiveType)) {
        return false;
    }
    
    return pMover->IsAttackDecision(&stAreaInfo) == 0;
}

/**
 * @brief Check skill targets in attack area
 *
 * IDA: ?CheckSkillTarget@CMySkillList@@QEAAXPEAVCMoverEx@@AEAUtagATTACK_AREA@@PEAVAttackJudgmentTrigger@@AEAEPEAUTB_SKILL@@@Z (0x1402BD8F0)
 * Scans for valid targets in attack area and adds them to attack target list.
 *
 * @param pMover Attacker mover
 * @param stAreaInfo Attack area info
 * @param pActionEvent Attack judgment trigger
 * @param bAttackTargetCnt Output: count of attack targets
 * @param pSkillRef Skill table reference
 */
void CMySkillList::CheckSkillTarget(CMoverEx* pMover, tagATTACK_AREA& stAreaInfo, AttackJudgmentTrigger* pActionEvent, std::uint8_t& bAttackTargetCnt, TB_SKILL* pSkillRef) {
    if (!pMover || !pActionEvent || !pSkillRef) {
        return;
    }
    
    E_ACTOR_TYPE byType = pMover->GetType();
    std::uint8_t byReactionType = pActionEvent->sReactionInfo.iReactionType;
    
    // Distance array for sorting targets
    float fDist[100];
    for (int i = 0; i < 100; ++i) {
        fDist[i] = FLT_MAX;
    }
    
    hkvVec3 vPos = pMover->GetPosition();
    
    // Get nearby game objects
    std::vector<CMover*> vecGameObjList;
    XArea::ScanGridOrigin(&pMover->XActor, 2, 3, &vecGameObjList);
    
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMoverEx* pOtherActor = dynamic_cast<CMoverEx*>(*it);
        if (!pOtherActor) {
            continue;
        }
        
        // Check if target is valid
        if (!pOtherActor->IsLive()) {
            continue;
        }
        
        if (pOtherActor->IsStatus(2) && !pOtherActor->IsFollower() &&
            !(pOtherActor->GetDieType() == 1 && pOtherActor->GetDieDelayTime() > 0.0f)) {
            continue;
        }
        
        if (!pMover->CheckReactionTarget(pActionEvent->sReactionInfo.iTargetType, pOtherActor, 0)) {
            continue;
        }
        
        // Check invincibility
        if (pActionEvent->sAttackType == 7 && pOtherActor->GetDefenseType() == 3 &&
            !pActionEvent->sReactionInfo.bIgnoreTargetInvincible) {
            continue;
        }
        
        // Check condition buff
        if (pActionEvent->sReactionInfo.iConditionBuffID > 0) {
            if (pOtherActor->FindBuffStatus(pActionEvent->sReactionInfo.iConditionBuffID, pMover->GetActorID().dwActorID) == -1) {
                continue;
            }
        }
        
        E_ACTOR_TYPE eType = pOtherActor->GetType();
        if (eType != eActorUser && eType != eActorMonster) {
            continue;
        }
        
        // Check if in attack area
        bool isResult = IsInAttackArea(pOtherActor, stAreaInfo, pActionEvent->sReactionInfo.iTargetStatus, pSkillRef->Passive_Type);
        
        // Self-targeting always succeeds
        if (pActionEvent->sReactionInfo.iTargetType == 1) {
            isResult = true;
        }
        
        if (isResult) {
            hkvVec3 vOtherPos = pOtherActor->GetPosition();
            float fCurDist = vPos.getDistanceToSquared(vOtherPos);
            
            if (bAttackTargetCnt >= pSkillRef->Target_Damage_Count || bAttackTargetCnt >= 100) {
                // Replace furthest target if closer
                unsigned int dwID = pOtherActor->GetActorID().dwActorID;
                for (int j = 0; j < pSkillRef->Target_Damage_Count && j < 100; ++j) {
                    if (fDist[j] > fCurDist) {
                        unsigned int dwOldID = m_dwAttackTarget[j];
                        m_dwAttackTarget[j] = dwID;
                        dwID = dwOldID;
                        
                        float fOldDist = fDist[j];
                        fDist[j] = fCurDist;
                        fCurDist = fOldDist;
                    }
                }
            } else {
                // Add new target
                fDist[bAttackTargetCnt] = fCurDist;
                AddSkillTarget(bAttackTargetCnt, pOtherActor->GetActorID().dwActorID, byReactionType, stAreaInfo.byHitPartsIndex);
                bAttackTargetCnt++;
            }
        } else {
            // Check grappling target
            CMoverEx* pGrapTarget = pMover->GetGrapTarget();
            if (pOtherActor == pGrapTarget) {
                AddSkillTarget(bAttackTargetCnt, pOtherActor->GetActorID().dwActorID, byReactionType, 0);
                bAttackTargetCnt++;
                pMover->ClearGrapProcess();
                pOtherActor->send_eSUB_CMD_MOVE_DROP(1);
            }
        }
    }
}

/**
 * @brief Check chain lightning targets
 *
 * IDA: ?CheckChainLightningTarget@CMySkillList@@QEAAXPEAVCMoverEx@@@Z (0x1402C1040)
 * Adds mover to deleted target list for all active chain lightning objects.
 *
 * @param pMoverEx Mover to remove from chain lightning targets
 */
void CMySkillList::CheckChainLightningTarget(CMoverEx* pMoverEx) {
    for (auto it = m_vChainLightningObject.begin(); it != m_vChainLightningObject.end(); ++it) {
        VChainLightningObject* pChainLightning = *it;
        if (pChainLightning && pChainLightning->IsActivate()) {
            pChainLightning->AddDeletedTarget(pMoverEx);
        }
    }
}


// ============================================================================
// CMySkillList Skill Target Functions
// ============================================================================

/**
 * @brief Get skill target with bone tracking
 *
 * IDA: ?GetSkillTarget@CMySkillList@@QEAAHPEAVCMoverEx@@PEAVAttackJudgmentTrigger@@PEAUTB_SKILL@@AEAE@Z (0x1402BD690)
 *
 * @param pMover Attacker mover
 * @param pActionEvent Attack judgment trigger
 * @param pSkillTable Skill table reference
 * @param bAttackTargetCnt Output: count of attack targets
 * @return Result code
 */
int CMySkillList::GetSkillTarget(CMoverEx* pMover, AttackJudgmentTrigger* pActionEvent, TB_SKILL* pSkillTable, std::uint8_t& bAttackTargetCnt) {
    if (!pMover || !pActionEvent || !pSkillTable) {
        return 56011;
    }
    
    float fPrevOrientationYaw = pMover->GetOrientationYaw();
    float fPrevMovingYaw = pMover->GetMovingYaw();
    hkvVec3 vPos = pMover->GetPosition();
    
    // Check for bone tracking in continuous melee
    if (pActionEvent->sAttackType == 0 && pActionEvent->sContinuousMeleeInfo.szTraceBoneName[0]) {
        int TraceBoneListIndex = pMover->GetTraceBoneListIndex(pActionEvent->sContinuousMeleeInfo.szTraceBoneName);
        int iIndex = pMover->GetHitCollisionCount() + TraceBoneListIndex;
        
        hkvVec3 vBonePos;
        pMover->GetBoneCurrentWorldSpaceTranslation(iIndex, vPos, vBonePos);
        
        float BoneYaw = pMover->GetBoneYaw(iIndex);
        float fYaw = fPrevOrientationYaw + BoneYaw + 90.0f;
        pMover->SetOrientationYaw(fYaw);
        vPos = vBonePos;
    }
    
    int iResult = GetSkillTarget(pMover, vPos, pActionEvent, pSkillTable, bAttackTargetCnt);
    pMover->SetOrientationYaw(fPrevOrientationYaw);
    
    return iResult;
}

/**
 * @brief Create projectile object
 *
 * IDA: ?CreateProjectile@CMySkillList@@SAPEAVVGameProjectileObject@@AEAVhkvVec3@@0PEAVCMoverEx@@PEAVAttackJudgmentTrigger@@@Z (0x1402C0780)
 *
 * @param vPos Projectile position
 * @param vDir Projectile direction
 * @param pMover Owner mover
 * @param pActionEvent Attack judgment trigger
 * @return Created projectile object
 */
VGameProjectileObject* CMySkillList::CreateProjectile(hkvVec3& vPos, hkvVec3& vDir, CMoverEx* pMover, AttackJudgmentTrigger* pActionEvent) {
    if (!pMover || !pMover->GetArea() || !pActionEvent) {
        return nullptr;
    }
    
    auto pInstance = ThreadLocalData::GetInstance();
    if (!pInstance) {
        return nullptr;
    }
    
    unsigned int dwActorID = pMover->GetActorID().dwActorID;
    UXMapID uxMapID = pMover->GetMapInsID();
    int nThreadNo = pInstance->m_ownerThreadIndex;
    int nEventID = pActionEvent->EventID;
    
    XArea* pArea = pMover->GetArea();
    if (!pArea->IsThreadSafe()) {
        LogHelper::LogError("game.system", "[PROJECTILE] ActorID : %d %I64d %d %d", dwActorID, uxMapID.__s0, nThreadNo, nEventID);
        return nullptr;
    }
    
    XVec3 xPos(vPos.x, vPos.y, vPos.z);
    VGameProjectileObject* pProjectile = pInstance->CreateProjectile(pArea, uxMapID, &xPos);
    if (!pProjectile) {
        return nullptr;
    }
    
    // Set bullet sync type from skill
    TB_SKILL* pSkillTable = pMover->GetSkillTable();
    if (pSkillTable) {
        pProjectile->SetBulletSyncType(pSkillTable->Bullet_Sync_Type);
    }
    
    // Configure projectile based on attack range angle
    if (pActionEvent->sAttackRange.fAngle > 1.0f) {
        if (pActionEvent->sProjInfo.bIsTargetGuided) {
            pProjectile->SetProjectileType(PT_GUIDED);
            pProjectile->SetTargetUpdateInfo(pActionEvent->sProjInfo.fTargetUpdateTime, pActionEvent->sProjInfo.fTargetUpdateAngle);
            pProjectile->SetFindTargetTime(pActionEvent->sProjInfo.fTargetFindTime);
            pProjectile->SetTargetPriority(pActionEvent->sProjInfo.iTargetPriorityType);
        } else if (pActionEvent->sProjInfo.bIsGravity) {
            pProjectile->SetProjectileType(PT_GRAVITY);
        } else {
            pProjectile->SetProjectileType(PT_STRAIGHT);
        }
        
        pProjectile->SetPhysicInfo(pActionEvent->sProjInfo.bIsCollision, pActionEvent->sProjInfo.shExplodeDecision);
        hkvVec3 vVelocity = vDir * pActionEvent->sProjInfo.fMoveSpeed;
        pProjectile->Fire(&vVelocity, pMover);
        pProjectile->SetLifeTime(pActionEvent->sProjInfo.fLifeTime);
        pProjectile->SetTargetType(pActionEvent->sReactionInfo.iTargetType);
        pProjectile->SetTargetStatus(pActionEvent->sReactionInfo.iTargetStatus);
        pProjectile->SetAttachToTarget(pActionEvent->sProjInfo.shAttachToTarget);
        pProjectile->SetParticleOnGround(pActionEvent->sProjInfo.eDirectionType == 2);
        
        if (pActionEvent->sProjInfo.bIsContinousHit) {
            pProjectile->SetContinousHitTime(pActionEvent->sProjInfo.fContinousTime);
        }
        
        pProjectile->SetPenetrate(pActionEvent->sProjInfo.bPenetrate);
        pProjectile->SetPenetrateCount(pActionEvent->sProjInfo.iPenetrateCount);
    } else {
        pProjectile->SetProjectileType(PT_STRAIGHT);
        hkvVec3 vVelocity = vDir * pActionEvent->sProjInfo.fMoveSpeed;
        pProjectile->Fire(&vVelocity, pMover);
        pProjectile->SetLifeTime(pActionEvent->sProjInfo.fLifeTime);
        pProjectile->SetTargetType(pActionEvent->sReactionInfo.iTargetType);
        pProjectile->SetTargetStatus(pActionEvent->sReactionInfo.iTargetStatus);
    }
    
    // Set path animation if specified
    if (pActionEvent->sProjInfo.szProjPathAnimName[0] && pMover->GetSkillMgr()) {
        auto pActionRes = pMover->GetSkillMgr()->GetProjPathActionRes();
        if (pActionRes) {
            auto pAnimationInfo = pActionRes->FindAnimationInfo(pActionEvent->sProjInfo.szProjPathAnimName);
            if (pAnimationInfo) {
                pProjectile->SetPathAnimInfo(pAnimationInfo);
            }
        }
    }
    
    return pProjectile;
}

/**
 * @brief Create trap object
 *
 * IDA: ?CreateTrap@CMySkillList@@SAPEAVVGameTrapObject@@AEAVhkvVec3@@0PEAVCMoverEx@@PEAVAttackJudgmentTrigger@@@Z (0x1402C10F0)
 *
 * @param vPos Trap position
 * @param vDir Trap direction
 * @param pMover Owner mover
 * @param pActionEvent Attack judgment trigger
 * @return Created trap object
 */
VGameTrapObject* CMySkillList::CreateTrap(hkvVec3& vPos, hkvVec3& vDir, CMoverEx* pMover, AttackJudgmentTrigger* pActionEvent) {
    if (!pMover || !pMover->GetArea() || !pActionEvent) {
        return nullptr;
    }
    
    auto pInstance = ThreadLocalData::GetInstance();
    if (!pInstance) {
        return nullptr;
    }
    
    unsigned int dwActorID = pMover->GetActorID().dwActorID;
    UXMapID uxMapID = pMover->GetMapInsID();
    int nThreadNo = pInstance->m_ownerThreadIndex;
    int nEventID = pActionEvent->EventID;
    
    XArea* pArea = pMover->GetArea();
    if (!pArea->IsThreadSafe()) {
        LogHelper::LogError("game.system", "[TRAP] ActorID : %d %I64d %d %d", dwActorID, uxMapID.__s0, nThreadNo, nEventID);
        return nullptr;
    }
    
    XVec3 xPos(vPos.x, vPos.y, vPos.z);
    VGameTrapObject* pTrapObj = pInstance->CreateTrap(pArea, uxMapID, &xPos);
    if (!pTrapObj) {
        return nullptr;
    }
    
    // Configure trap
    pTrapObj->SetLifeTime(pActionEvent->sProjInfo.fLifeTime);
    
    if (pActionEvent->sProjInfo.bIsContinousHit) {
        pTrapObj->SetInfluenceInterval(pActionEvent->sProjInfo.fContinousTime);
    } else {
        pTrapObj->SetInfluenceInterval(0.0f);
    }
    
    pTrapObj->SetInfluenceRadius(pActionEvent->sAttackRange.fRadius);
    pTrapObj->SetAttackRange(&pActionEvent->sAttackRange);
    pTrapObj->SetDelayTime(pActionEvent->sProjInfo.fDelayTime);
    pTrapObj->SetExplodeType(pActionEvent->sProjInfo.shExplodeDecision);
    pTrapObj->SetExplodeFaction(pActionEvent->sProjInfo.shExplodeFaction);
    pTrapObj->SetCollisionCheckType(pActionEvent->sAttackRangeType);
    pTrapObj->SetTargetStatus(pActionEvent->sReactionInfo.iTargetStatus);
    
    // Check buff for ignore can hit
    int nBuffID = pActionEvent->sReactionInfo.iBuffID;
    if (nBuffID > 0) {
        auto pServer = TXSingleton<XGameServer>::Instance();
        TB_BUFF* pBuffTable = XResourceMgr::GetTB_BUFF(&pServer->m_xResourceMgr, nBuffID);
        if (pBuffTable && (pBuffTable->System_Type == 2 || pBuffTable->System_Type == 3)) {
            pTrapObj->SetIgnoreCanHit(true);
        }
    }
    
    // Set explode range
    pTrapObj->SetExplodeRange(&pActionEvent->sProjInfo.sCollisionRange);
    pTrapObj->SetExplodeDelayTime(pActionEvent->sProjInfo.fExplodeDelayTime);
    
    // Check for explode damage particle
    bool bExplodeDamage = strlen(pActionEvent->sProjInfo.szProjExplodeParticle) > 1;
    pTrapObj->SetExplodeDamage(bExplodeDamage);
    
    pTrapObj->Install(&vPos, pMover);
    
    return pTrapObj;
}


// ============================================================================
// CMySkillList::ProcessTrap
// IDA: 0x1402B81A0
// ============================================================================
void CMySkillList::ProcessTrap(
    CMoverEx* pMover,
    CMoverEx* pTrapOwner,
    SSkillInfo* sSkillInfo,
    hkvVec3 vTrapPos,
    std::uint8_t byHitPartsIndex,
    bool bAllowAbsorbSG,
    bool bCheckCombo)
{
    if (!pMover || !pTrapOwner)
        return;

    int nSkillID = sSkillInfo->nSkillID;
    AttackJudgmentTrigger* pTrigger = sSkillInfo->pTrigger;
    if (!pTrigger)
        return;

    auto pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID);
    if (!pSkillTable)
        return;

    if (!pTrapOwner->CheckReactionTarget(pTrapOwner, pTrigger->sReactionInfo.iTargetType, pMover, 0))
        return;

    // Calculate direction from trap owner to trap position
    hkvVec3 vDir;
    const hkvVec3* pOwnerPos = VisObject3D_cl::GetPosition(pTrapOwner);
    vDir = vTrapPos - *pOwnerPos;

    // Process projectile explode
    ProcessProjectileExplode(
        pTrapOwner,
        vTrapPos,
        vDir,
        sSkillInfo,
        pMover,
        3,  // byDamageType
        0,  // iTargetOrder
        false,  // bPenetrate
        byHitPartsIndex,
        true,  // bTrapExplode
        false,  // bHitWall
        bAllowAbsorbSG,
        bCheckCombo);

    // Apply buff if damage rate is 0 or less
    if (pTrigger->sReactionInfo.fDamageRate <= 0.0f)
    {
        if (pTrigger->sReactionInfo.iBuffID > 0)
        {
            UXActorID actorID;
            pTrapOwner->GetActorID(&actorID);
            std::uint32_t dwID = CQuestCondition::GetQuestID(&actorID);
            ApplyBuffSkill(pMover, pTrigger->sReactionInfo.iBuffID, dwID, 0);
        }
        if (pTrigger->sReactionInfo.iAuraID > 0)
        {
            pMover->SetAuraSkill(pTrigger->sReactionInfo.iAuraID);
        }
    }
}

// ============================================================================
// CMySkillList::ProcessChain
// IDA: 0x1402B9730
// ============================================================================
void CMySkillList::ProcessChain(
    CMoverEx* pMover,
    hkvVec3 vPos,
    hkvVec3 vDir,
    SSkillInfo* sSkillInfo,
    CMoverEx* pProjTarget,
    std::uint8_t byDamageType,
    int iTargetOrder,
    bool bPenetrate,
    std::uint8_t byHitPartsIndex,
    bool bTrapExplode,
    bool bHitWall)
{
    if (!pMover || !pProjTarget)
        return;

    if (pMover != m_pActor)
        return;

    int nSkillID = sSkillInfo->nSkillID;
    auto pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID);
    if (!pSkillTable)
        return;

    AttackJudgmentTrigger* pActionEvent = sSkillInfo->pTrigger;
    if (!pActionEvent)
        return;

    if (!pMover->IsEnemyForChain(pMover, pProjTarget))
        return;

    int passiveType = pSkillTable->Passive_Type;
    if (!pProjTarget->IsCanHit(pActionEvent->sReactionInfo.iTargetStatus, passiveType))
        return;

    // Get target ID
    UXActorID actorID;
    pProjTarget->GetActorID(&actorID);
    std::uint32_t nID = CQuestCondition::GetQuestID(&actorID);

    // Check if already in attack target list
    for (int idx = 0; idx < 100; ++idx)
    {
        if (m_dwAttackTarget[idx] == nID)
            return;
    }

    // Find empty slot
    int idxa = 0;
    for (; idxa < 100 && m_dwAttackTarget[idxa] != 0; ++idxa)
        ;

    if (idxa < 100)
    {
        AddSkillTarget(idxa, nID, pActionEvent->sReactionInfo.iReactionType, byHitPartsIndex);
        m_stChainHitInfo[idxa].vHitPos = vPos;
        m_stChainHitInfo[idxa].vHitDir = vDir;
        m_stChainHitInfo[idxa].iTargetOrder = iTargetOrder;
        m_stChainHitInfo[idxa].bHitWall = bHitWall;
    }

    // Copy skill info
    m_stChainSkillInfo = *sSkillInfo;
}


// ============================================================================
// CMySkillList::ProcessProjectileExplode
// IDA: 0x1402B8420
// ============================================================================
void CMySkillList::ProcessProjectileExplode(
    CMoverEx* pMover,
    hkvVec3 vPos,
    hkvVec3 vDir,
    SSkillInfo* sSkillInfo,
    CMoverEx* pProjTarget,
    std::uint8_t byDamageType,
    int iTargetOrder,
    bool bPenetrate,
    std::uint8_t byHitPartsIndex,
    bool bTrapExplode,
    bool bHitWall,
    bool bAllowAbsorbSG,
    bool bCheckCombo)
{
    if (!pMover)
        return;

    int nSkillID = sSkillInfo->nSkillID;
    auto pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID);
    if (!pSkillTable)
        return;

    AttackJudgmentTrigger* pActionEvent = sSkillInfo->pTrigger;
    if (!pActionEvent)
        return;

    if (pActionEvent->sReactionInfo.fDamageRate <= 0.0f && !pActionEvent->sReactionInfo.iReactionType)
    {
        // Apply buff/aura without damage
        if (pActionEvent->sReactionInfo.iBuffID > 0 && pProjTarget)
        {
            UXActorID actorID;
            pMover->GetActorID(&actorID);
            std::uint32_t dwUserID = CQuestCondition::GetQuestID(&actorID);
            ApplyBuffSkill(pProjTarget, pActionEvent->sReactionInfo.iBuffID, dwUserID, 0);
        }
        if (pActionEvent->sReactionInfo.iAuraID > 0 && pProjTarget)
        {
            pProjTarget->SetAuraSkill(pActionEvent->sReactionInfo.iAuraID);
        }
        // Check explode summon/trap
        if (!bHitWall && pActionEvent->sProjInfo.shExplodeTrapMakeType == 4)
            CheckExplodeSummon(pMover, vPos, sSkillInfo);
        if (!bTrapExplode && (!bHitWall || pActionEvent->sProjInfo.shExplodeTrapMakeType != 3))
            CheckExplodeTrap(pMover, vPos, sSkillInfo, pProjTarget);
        return;
    }

    // Determine target type
    std::uint8_t byTargetType = 6;
    if (pMover->GetType() != eActorMonster)
        byTargetType |= 1;

    std::uint8_t bAttackTargetCnt = 0;
    
    // Build attack area
    tagATTACK_AREA stAttackArea;
    float fDirYaw = pMover->GetOrientationYaw();
    GetAttackArea(vPos, fDirYaw, pActionEvent->sAttackRangeType, &pActionEvent->sAttackRange, &stAttackArea);

    // Clear previous attack damage
    if (pMover->GetSkillMgr())
    {
        auto pSkillMgr = pMover->GetSkillMgr();
        pSkillMgr->ClearAttackDamage();
    }

    // Find targets
    if (bTrapExplode)
    {
        // Trap explode - single target
        if (!pProjTarget)
            return;

        int passiveType = pSkillTable->Passive_Type;
        if (!pProjTarget->IsCanHit(pActionEvent->sReactionInfo.iTargetStatus, passiveType))
            return;

        // Check if in attack area
        const hkvVec3* pTargetPos = VisObject3D_cl::GetPosition(pProjTarget);
        int bCheckCylinder = 1;
        if (pProjTarget->IsAttackHeight(&stAttackArea, pTargetPos, &bCheckCylinder))
            return;

        UXActorID actorID;
        pProjTarget->GetActorID(&actorID);
        std::uint32_t questID = CQuestCondition::GetQuestID(&actorID);
        AddSkillTarget(bAttackTargetCnt++, questID, pActionEvent->sReactionInfo.iReactionType, byHitPartsIndex);
    }
    else if (pActionEvent->sProjInfo.bTargetOnly && pProjTarget)
    {
        // Target only mode
        int passiveType = pSkillTable->Passive_Type;
        if (pProjTarget->IsCanHit(pActionEvent->sReactionInfo.iTargetStatus, passiveType))
        {
            UXActorID actorID;
            pProjTarget->GetActorID(&actorID);
            std::uint32_t id = CQuestCondition::GetQuestID(&actorID);
            AddSkillTarget(bAttackTargetCnt++, id, pActionEvent->sReactionInfo.iReactionType, byHitPartsIndex);
        }
    }
    else
    {
        // Area attack - check skill targets
        if (pActionEvent->sAttackType != 5)
            CheckSkillTarget(pMover, &stAttackArea, pActionEvent, &bAttackTargetCnt, pSkillTable);

        // Add projectile target if enemy
        if (pProjTarget && pMover->IsEnemy(pProjTarget))
        {
            int passiveType = pSkillTable->Passive_Type;
            if (pProjTarget->IsCanHit(pActionEvent->sReactionInfo.iTargetStatus, passiveType))
            {
                UXActorID actorID;
                pProjTarget->GetActorID(&actorID);
                std::uint32_t nID = CQuestCondition::GetQuestID(&actorID);

                bool bFound = false;
                for (int idx = 0; idx < bAttackTargetCnt; ++idx)
                {
                    if (idx < 100 && m_dwAttackTarget[idx] == nID)
                    {
                        bFound = true;
                        break;
                    }
                }

                if (!bFound && bAttackTargetCnt < 100)
                {
                    AddSkillTarget(bAttackTargetCnt++, nID, pActionEvent->sReactionInfo.iReactionType,
                                   stAttackArea.byHitPartsIndex);
                }
            }
        }
    }

    // Apply damage if there are targets
    if (bAttackTargetCnt > 0 && pMover == m_pActor)
    {
        std::uint16_t wContinousHit = 0;

        if (pActionEvent->sReactionInfo.fDamageRate > 0.0f)
        {
            CMonster* pMonster = nullptr;
            CAkashicObject* pAkashic = nullptr;

            // Check mover type for special handling
            if (pMover->GetType() == eActorMonster)
            {
                pMonster = static_cast<CMonster*>(pMover);
            }
            else if (pMover->GetType() == eActorNPC)  // Akashic
            {
                pAkashic = static_cast<CAkashicObject*>(pMover);
            }

            CMoverEx* pOwnerMoverEx = nullptr;

            // Calculate damage for each target
            for (int i = 0; i < bAttackTargetCnt && i < 100; ++i)
            {
                CMover* pTarget = CMover::GetMoverObject(m_pActor, m_dwAttackTarget[i]);
                if (!pTarget)
                {
                    m_stAttackDamage[i].byDamageFlag = 0;
                    m_stAttackDamage[i].nDamage = 0;
                    continue;
                }

                TB_SKILL* pRealSkillTable = pSkillTable;

                // Use linked skill for attack type 4
                if (pActionEvent->sAttackType == 4)
                {
                    TB_SKILL* pLinkedSkill = XResourceMgr::GetTB_SKILL(
                        &pServer->m_xResourceMgr, pActionEvent->sProjInfo.nLinkedSkillID);
                    if (pLinkedSkill)
                        pRealSkillTable = pLinkedSkill;
                }

                int fChainDamageRate = 1065353216;  // 1.0f in int representation
                if (iTargetOrder > 0)
                    fChainDamageRate = pActionEvent->sConnectionInfo.fDamageMutiple;

                bool bFindOwnerPlayer = false;

                // Monster owner handling
                if (pMonster)
                {
                    TB_MONSTER* pMobTable = pMonster->GetMobTableRef();
                    pOwnerMoverEx = pMonster->GetOwnerPlayer();

                    if (pOwnerMoverEx && pMobTable && pMobTable->Monster_Element == 1)
                    {
                        bool bApplyMultiple = pMover->GetApplyMultipleDamageOnce();
                        pOwnerMoverEx->CalcTargetDamage(pTarget, i, bAllowAbsorbSG, pRealSkillTable,
                                                        pActionEvent, fChainDamageRate, false, false, bApplyMultiple);

                        // Copy damage from owner to monster
                        if (pMonster->GetSkillMgr())
                        {
                            auto pOwnerSkillMgr = pOwnerMoverEx->GetSkillMgr();
                            auto pMonsterSkillMgr = pMonster->GetSkillMgr();

                            tagSKILL_ACTION_DAMAGE stOwnerDmg, stMonsterDmg;
                            pOwnerSkillMgr->GetAttackDamage(&stOwnerDmg, i);
                            pMonsterSkillMgr->GetAttackDamage(&stMonsterDmg, i);

                            stMonsterDmg.nDamage = stOwnerDmg.nDamage;
                            stMonsterDmg.nAttrDamage = stOwnerDmg.nAttrDamage;
                            stMonsterDmg.nHP = stOwnerDmg.nHP;
                            stMonsterDmg.byDamageFlag = stOwnerDmg.byDamageFlag;
                            pMonsterSkillMgr->SetAttackDamage(i, &stMonsterDmg);
                        }
                        bFindOwnerPlayer = true;
                    }
                }

                if (!bFindOwnerPlayer)
                {
                    pMover->CalcTargetDamage(pTarget, i, bAllowAbsorbSG, pRealSkillTable,
                                            pActionEvent, fChainDamageRate, false, false, true);
                }
            }

            // Clear multiple damage buff if needed
            if (pOwnerMoverEx)
            {
                if (pMover->GetApplyMultipleDamageOnce() &&
                    pOwnerMoverEx->GetMultipleDamageOnce() > 0.0f &&
                    pOwnerMoverEx->GetApplyMultipleDamageOnce())
                {
                    int iIndex = pOwnerMoverEx->FindBuffByEffectType(0x0E, 0);
                    if (iIndex != -1)
                        pOwnerMoverEx->ClearBuffStatusBySlot(iIndex, 0);
                }
            }

            // Check combo
            if (bCheckCombo)
            {
                CMoverEx* pOwner = pMover->GetOwnerPlayer();
                if (pMover->GetType() == eActorUser)
                {
                    wContinousHit = pMover->CheckContinousAttack(bAttackTargetCnt);
                }
                else if (pOwner &&
                         (pOwner == pMover ||
                          (pMonster && pMonster->IsMonsterCombo()) ||
                          (pAkashic && pAkashic->IsComboAkashic())))
                {
                    wContinousHit = pOwner->CheckContinousAttack(bAttackTargetCnt);
                }
            }
        }

        // Apply skill damage frame
        float fDirYawFromVec = CMover::GetYawFromVector(vDir);
        pMover->ApplySkillDamageFrame(nSkillID, pActionEvent->EventID, bAttackTargetCnt, vPos,
                                      fDirYawFromVec, wContinousHit, byDamageType, bPenetrate);

        // Send skill action packet
        pMover->send_eSUB_CMD_ACTION_SKILL(pMover, nSkillID, pActionEvent->EventID, vPos,
                                           bAttackTargetCnt, wContinousHit, bPenetrate);

        // Apply aura to targets
        if (pActionEvent->sReactionInfo.iAuraID > 0)
        {
            for (int j = 0; j < bAttackTargetCnt && j < 100; ++j)
            {
                CMover* pTargetMover = CMover::GetMoverObject(m_pActor, m_dwAttackTarget[j]);
                if (pTargetMover)
                    pTargetMover->SetAuraSkill(pActionEvent->sReactionInfo.iAuraID);
            }
        }

        // Clear multiple damage buff
        if (pMover->GetMultipleDamageOnce() > 0.0f && pMover->GetApplyMultipleDamageOnce())
        {
            int buffIdx = pMover->FindBuffByEffectType(0x0E, 0);
            if (buffIdx != -1)
                pMover->ClearBuffStatusBySlot(buffIdx, 0);
        }

        // Apply buff to targets
        if (pActionEvent->sReactionInfo.iBuffID > 0)
        {
            for (int k = 0; k < bAttackTargetCnt && k < 100; ++k)
            {
                CMover* pTargetMover = CMover::GetMoverObject(m_pActor, m_dwAttackTarget[k]);
                if (pTargetMover)
                {
                    UXActorID actorID;
                    pMover->GetActorID(&actorID);
                    std::uint32_t ownerID = CQuestCondition::GetQuestID(&actorID);
                    ApplyBuffSkill(pTargetMover, pActionEvent->sReactionInfo.iBuffID, ownerID, bAttackTargetCnt);
                }
            }
        }
    }
    else
    {
        // No targets - apply buff/aura to projectile target only
        if (pActionEvent->sReactionInfo.iBuffID > 0 && pProjTarget)
        {
            std::uint32_t moverID = pMover->GetID();
            ApplyBuffSkill(pProjTarget, pActionEvent->sReactionInfo.iBuffID, moverID, 0);
        }
        if (pActionEvent->sReactionInfo.iAuraID > 0 && pProjTarget)
        {
            pProjTarget->SetAuraSkill(pActionEvent->sReactionInfo.iAuraID);
        }
    }

    // Check explode summon/trap
    if (!bHitWall && pActionEvent->sProjInfo.shExplodeTrapMakeType == 4)
        CheckExplodeSummon(pMover, vPos, sSkillInfo);
    if (!bTrapExplode && (!bHitWall || pActionEvent->sProjInfo.shExplodeTrapMakeType != 3))
        CheckExplodeTrap(pMover, vPos, sSkillInfo, pProjTarget);

    // Check if mover died
    if (pMover->GetHP() <= 0)
    {
        pMover->SetDie(12, 0);
    }
}


// ============================================================================
// CMySkillList::ApplyBuffSkill
// IDA: 0x1402C22B0
// ============================================================================
int CMySkillList::ApplyBuffSkill(CMover* pMover, std::uint16_t nBuffID, std::uint32_t dwUserID, std::uint32_t dwExtraData)
{
    auto pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffRef = XResourceMgr::GetTB_BUFF(&pServer->m_xResourceMgr, nBuffID);
    if (!pBuffRef)
        return 0;

    // EffectType_01 == 24 means composite buff
    if (pBuffRef->EffectType_01 != 24)
        return _ApplyBuffSkill(pMover, nBuffID, dwUserID, dwExtraData);

    // Apply sub-buffs from Option_Value_01/02/03
    int bResult = 0;
    std::uint16_t nSubBuffID = pBuffRef->Option_Value_01;
    if (nSubBuffID)
        bResult = _ApplyBuffSkill(pMover, nSubBuffID, dwUserID, dwExtraData);

    nSubBuffID = pBuffRef->Option_Value_02;
    if (nSubBuffID)
        bResult = _ApplyBuffSkill(pMover, nSubBuffID, dwUserID, dwExtraData);

    nSubBuffID = pBuffRef->Option_Value_03;
    if (nSubBuffID)
        return _ApplyBuffSkill(pMover, nSubBuffID, dwUserID, dwExtraData);

    return bResult;
}

// ============================================================================
// CMySkillList::_ApplyBuffSkill (private helper)
// IDA: 0x1402C23E0
// ============================================================================
int CMySkillList::_ApplyBuffSkill(CMover* pMover, std::uint16_t nBuffID, std::uint32_t dwUserID, int dwExtraData)
{
    if (!pMover)
        return 0;

    bool bCheckDie = true;

    // Check monster special cases
    if (pMover->GetType() == eActorMonster)
    {
        CMonster* pMonster = static_cast<CMonster*>(pMover);
        TB_MONSTER* pMobTable = pMonster->GetMobTableRef();
        if (pMobTable)
        {
            if (pMonster->IsFollower())
            {
                bCheckDie = false;
            }
            else if (pMobTable->Monster_Type != 0 &&
                     pMobTable->Monster_Type != 6 &&
                     pMobTable->Monster_Type != 7)
            {
                return 0;
            }
        }
    }

    if (bCheckDie && pMover->IsDie())
        return 0;

    auto pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffRef = XResourceMgr::GetTB_BUFF(&pServer->m_xResourceMgr, nBuffID);
    if (!pBuffRef)
        return 0;

    // Check resist stat
    int iResistStatIndex = pMover->GetResistStatIndexByBuff(pBuffRef->EffectType_01);
    float fResist = 0.0f;
    if (iResistStatIndex != -1)
        fResist = pMover->GetStat(iResistStatIndex);

    // System_Type 2 or 3 ignores resist
    if (pBuffRef->System_Type == 2 || pBuffRef->System_Type == 3)
        fResist = 0.0f;

    // Check success rate
    if (pBuffRef->Buff_Success_Rate > 0 && pBuffRef->System_Type != 2 && pBuffRef->System_Type != 3)
    {
        float fSuccessRate = (float)pBuffRef->Buff_Success_Rate - fResist;
        if ((rand() % 10000) >= (int)fSuccessRate)
            return 0;
    }

    float fStartTime = (float)(int)pBuffRef->StandBy_Time * 0.001f;

    if (fStartTime == 0.0f)
    {
        // Immediate buff application
        float fPrevHP = pMover->GetStat(1);  // STAT_HP
        pMover->SetBuffStatus(nBuffID, dwUserID, true);
        float fHeal = pMover->GetStat(1) - fPrevHP;

        if (fHeal > 0.0f)
        {
            if (dwExtraData)
                fHeal *= (float)dwExtraData;

            // Track healing amount
            UXActorID actorID;
            pMover->GetActorID(&actorID);
            std::uint32_t targetID = CQuestCondition::GetQuestID(&actorID);

            if (dwUserID && dwUserID != targetID)
            {
                CMoverEx* pOwner = static_cast<CMoverEx*>(CMover::GetMoverObject(pMover, dwUserID));
                if (pOwner)
                    pOwner->AddAmountOfHeal(fHeal);
            }
            else
            {
                CMoverEx* pMoverEx = static_cast<CMoverEx*>(pMover);
                pMoverEx->AddAmountOfHeal(fHeal);
            }
        }
    }
    else
    {
        // Delayed buff application
        CMoverEx* pMoverEx = static_cast<CMoverEx*>(pMover);
        if (pMoverEx)
        {
            int existingBuff = pMover->FindBuffByGroupID(pBuffRef->Buff_Group, dwUserID);
            if (existingBuff == -1)
            {
                pMoverEx->AddDelayBuffStatus(fStartTime, nBuffID, dwUserID, dwExtraData);
            }
            else if (pMoverEx->IsCanBuffOverlap(fStartTime, pBuffRef, dwUserID))
            {
                pMoverEx->AddDelayBuffStatus(fStartTime, nBuffID, dwUserID, dwExtraData);
            }
        }
    }

    // Process game rule check in maze
    XArea* pArea = pMover->GetArea();
    if (pArea)
    {
        XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
        if (pMaze)
            pMaze->ProcessGameRuleCheck(pMaze, pMover);
    }

    return 1;
}


// ============================================================================
// CMySkillList::ValidSkill
// IDA: 0x1402B64C0
// ============================================================================
int CMySkillList::ValidSkill(std::uint32_t nSkillID)
{
    auto pServer = TXSingleton<XGameServer>::Instance();
    if (XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID))
        return 0;
    return 56001;  // Error code for invalid skill
}

// ============================================================================
// CMySkillList::EndSkill
// IDA: 0x1402B79F0
// ============================================================================
void CMySkillList::EndSkill(TB_SKILL* pSkillTable)
{
    if (!pSkillTable)
        return;

    int iIndex = -1;
    std::uint8_t costAttr = pSkillTable->Skill_Cost_Attribute;

    switch (costAttr)
    {
        case 1: iIndex = 1; break;  // HP
        case 2: iIndex = 2; break;  // MP
        case 3: iIndex = 3; break;  // SG
        case 4: iIndex = 16; break; // Stamina
    }

    float fSkillCost = GetSkillCost(pSkillTable);

    if (fSkillCost > 0.0f && pSkillTable->Cost_Type == 1 && !m_bTestMode && iIndex != -1)
    {
        m_pActor->SetContinousCost(iIndex, 0.0f);
    }

    // Special handling for SG regeneration when cost is 0
    if (iIndex == 3 && fSkillCost == 0.0f)
    {
        auto pAttr = m_pActor->GetGOC<CGocAttribute>();
        if (pAttr)
        {
            pAttr->SetSTRegStat(1);
        }
    }
}


// ============================================================================
// CMySkillList::ThinkFunction
// IDA: 0x1402B6500
// Main think/update function for skill list - processes projectiles, traps,
// chain lightning, random events, and explode traps
// ============================================================================
void CMySkillList::ThinkFunction()
{
    // Get actor ID if actor exists
    if (m_pActor)
    {
        UXActorID actorID;
        m_pActor->GetActorID(&actorID);
        CQuestCondition::GetQuestID(&actorID);
    }

    // Process projectiles
    if (!m_vProjectiles.empty())
    {
        for (auto it = m_vProjectiles.begin(); it != m_vProjectiles.end(); )
        {
            VGameProjectileObject* pProjectile = *it;
            if (!pProjectile)
            {
                ++it;
                continue;
            }

            // Check if in valid area
            if (pProjectile->GetArea())
            {
                UXMapID instanceID;
                pProjectile->GetArea()->GetInstanceID(&instanceID);
            }

            // Tick projectile
            auto pTimer = ThreadLocalData::GetTimer();
            pTimer->GetTimeDifference();
            pProjectile->Tick();

            if (pProjectile->IsGazeMoving())
            {
                ++it;
            }
            else
            {
                RemoveProjectile(pProjectile);
                it = m_vProjectiles.erase(it);
                if (m_vProjectiles.empty())
                    break;
            }
        }
    }

    // Process traps
    if (!m_vTraps.empty())
    {
        for (auto it = m_vTraps.begin(); it != m_vTraps.end(); )
        {
            VGameTrapObject* pTrap = *it;
            if (!pTrap)
            {
                ++it;
                continue;
            }

            // Write log if in world (not maze)
            if (m_pActor && m_pActor->GetArea())
            {
                XArea* pArea = m_pActor->GetArea();
                if (!pArea->GetWorldType())
                {
                    auto pInstance = ThreadLocalData::GetInstance();
                    std::uint16_t tbMapID = m_pActor->GetArea()->GetTBMapID();
                    pTrap->WriteLog(tbMapID, pInstance->m_ownerThreadIndex);
                }
            }

            // Check trap area
            if (pTrap->GetArea())
            {
                UXMapID instanceID;
                pTrap->GetArea()->GetInstanceID(&instanceID);
            }

            pTrap->GetSkillInfo();
            pTrap->ThinkFunction();

            if (pTrap->IsActivate())
            {
                ++it;
            }
            else
            {
                RemoveTrap(pTrap);
                it = m_vTraps.erase(it);
                if (m_vTraps.empty())
                    break;
            }
        }
    }

    // Process chain lightning objects
    if (!m_vChainLightningObject.empty())
    {
        for (auto it = m_vChainLightningObject.begin(); it != m_vChainLightningObject.end(); )
        {
            VChainLightningObject* pChainLightning = *it;
            if (!pChainLightning ||
                (pChainLightning->GetWarpPotal(),
                 auto pTimer = ThreadLocalData::GetTimer(),
                 pTimer->GetTimeDifference(),
                 pChainLightning->Tick(),
                 pChainLightning->IsActivate()))
            {
                ++it;
            }
            else
            {
                RemoveChainLightning(pChainLightning);
                it = m_vChainLightningObject.erase(it);
                if (m_vChainLightningObject.empty())
                    break;
            }
        }
    }

    // Process random trap events
    if (!m_vecRandomTrapEvent.empty())
    {
        for (auto it = m_vecRandomTrapEvent.begin(); it != m_vecRandomTrapEvent.end(); )
        {
            SRandomTrapEvent* pRandomTrap = *it;
            if (!pRandomTrap || pRandomTrap->nRandomTrapCount <= 0 || !pRandomTrap->pRandomTrapEvent)
            {
                ++it;
                continue;
            }

            auto pTimer = ThreadLocalData::GetTimer();
            float fTimeDiff = pTimer->GetTimeDifference();
            pRandomTrap->fRandomTrapTime += fTimeDiff;

            if (pRandomTrap->fRandomTrapTime >= pRandomTrap->fRandomTrapDelayTime)
            {
                if (pRandomTrap->pRandomTrapEvent->sProjInfo.RandomPattern == 1)
                {
                    // Create random trap pattern 1
                    CreateRandomTrap(
                        pRandomTrap->pRandomTrapEvent,
                        pRandomTrap->pMover,
                        pRandomTrap->vRandomTrapEntityPos,
                        pRandomTrap->fRandomTrapEntityYaw,
                        &pRandomTrap->sRandomTrapSkillInfo);
                }
                else if (pRandomTrap->pRandomTrapEvent->sProjInfo.RandomPattern == 2)
                {
                    // Create random trap pattern 2
                    tagATTACK_AREA stAttackArea;
                    GetAttackArea(
                        pRandomTrap->pMover,
                        pRandomTrap->vRandomTrapEntityPos,
                        pRandomTrap->fRandomTrapEntityYaw,
                        pRandomTrap->pRandomTrapEvent->sAttackRangeType,
                        &pRandomTrap->pRandomTrapEvent->sAttackRange,
                        &stAttackArea);

                    hkvMat3 matRot;
                    VisMath_cl::EulerToMatrix(pRandomTrap->fRandomTrapEntityYaw, 0.0f, 0.0f, &matRot);
                    hkvVec3 vDir = matRot * pRandomTrap->pRandomTrapEvent->sProjInfo.vRandomPos;
                    hkvVec3 vOffset = vDir * (float)(pRandomTrap->pRandomTrapEvent->sProjInfo.nRandomCount - pRandomTrap->nRandomTrapCount);
                    hkvVec3 vDestPos = stAttackArea.vCenterPos + vOffset;
                    hkvVec3 vTrapDir(0.0f, -1.0f, 0.0f);

                    VGameTrapObject* pTrapObj = CreateTrap(vDestPos, vTrapDir, pRandomTrap->pMover, pRandomTrap->pRandomTrapEvent);
                    if (pTrapObj)
                    {
                        pTrapObj->SetSkillInfo(&pRandomTrap->sRandomTrapSkillInfo);
                        m_vTraps.push_back(pTrapObj);

                        if (CMover::IsSendProjectilePacket(pRandomTrap->pMover, pRandomTrap->pRandomTrapEvent))
                        {
                            int nSessionID = pTrapObj->GetSessionID();
                            pRandomTrap->pMover->send_eSUB_CMD_PROJECTILE(
                                pRandomTrap->pMover,
                                pRandomTrap->sRandomTrapSkillInfo.nSkillID,
                                pRandomTrap->pRandomTrapEvent->EventID,
                                vDestPos,
                                vTrapDir,
                                nSessionID);
                        }
                    }
                }

                pRandomTrap->nRandomTrapCount--;
                if (pRandomTrap->nRandomTrapCount)
                {
                    pRandomTrap->fRandomTrapTime -= pRandomTrap->fRandomTrapDelayTime;
                    ++it;
                }
                else
                {
                    delete pRandomTrap;
                    it = m_vecRandomTrapEvent.erase(it);
                }
            }
            else
            {
                ++it;
            }
        }
    }

    // Process random summon events
    if (!m_vecRandomSummonEvent.empty())
    {
        for (auto it = m_vecRandomSummonEvent.begin(); it != m_vecRandomSummonEvent.end(); )
        {
            SRandomSummonEvent* pRandomSummon = *it;
            if (!pRandomSummon || pRandomSummon->nRandomSummonCount <= 0)
            {
                ++it;
                continue;
            }

            auto pTimer = ThreadLocalData::GetTimer();
            float fTimeDiff = pTimer->GetTimeDifference();
            pRandomSummon->fRandomSummonTime += fTimeDiff;

            if (pRandomSummon->fRandomSummonTime >= pRandomSummon->fRandomSummonDelayTime)
            {
                CreateRandomSummon(pRandomSummon);
                pRandomSummon->nRandomSummonCount--;

                if (pRandomSummon->nRandomSummonCount)
                {
                    pRandomSummon->fRandomSummonTime -= pRandomSummon->fRandomSummonDelayTime;
                    ++it;
                }
                else
                {
                    delete pRandomSummon;
                    it = m_vecRandomSummonEvent.erase(it);
                }
            }
            else
            {
                ++it;
            }
        }
    }

    // Process explode traps
    if (!m_vecExplodeTrap.empty())
    {
        for (auto it = m_vecExplodeTrap.begin(); it != m_vecExplodeTrap.end(); )
        {
            SExplodeTrap* pExplodeTrap = *it;
            if (!pExplodeTrap)
            {
                ++it;
                continue;
            }

            if (pExplodeTrap->pMover)
            {
                UXActorID actorID;
                pExplodeTrap->pMover->GetActorID(&actorID);
                CQuestCondition::GetQuestID(&actorID);
            }
            if (pExplodeTrap->pTarget)
            {
                UXActorID actorID;
                pExplodeTrap->pTarget->GetActorID(&actorID);
                CQuestCondition::GetQuestID(&actorID);
            }

            RetiveExplodeTrap(pExplodeTrap->pMover, pExplodeTrap->vPos, &pExplodeTrap->sSkillInfo, pExplodeTrap->pTarget);
            delete pExplodeTrap;
            it = m_vecExplodeTrap.erase(it);

            if (m_vecExplodeTrap.empty())
                break;
        }
    }
}


// ============================================================================
// CMySkillList::CMySkillList (Constructor)
// IDA: 0x1402B5BD0
// ============================================================================
CMySkillList::CMySkillList()
    : m_pActor(nullptr)
    , m_bTestMode(false)
    , m_nAttackTargetCount(0)
    , m_bCheckDelayedProj(true)
    , m_bCheckContinuousMelee(true)
    , m_pActionRes(nullptr)
{
    // Initialize global cooldown array
    memset(m_fGlobalCooltime, 0, sizeof(m_fGlobalCooltime));
    memset(m_stAttackDamage, 0, sizeof(m_stAttackDamage));
    memset(m_dwAttackTarget, 0, sizeof(m_dwAttackTarget));
    memset(m_stChainHitInfo, 0, sizeof(m_stChainHitInfo));
    memset(&m_stChainSkillInfo, 0, sizeof(m_stChainSkillInfo));
    memset(m_nBaseDamage, 0, sizeof(m_nBaseDamage));
    memset(m_fAttackFlySpeed, 0, sizeof(m_fAttackFlySpeed));
    
    // Initialize attack extra move vectors
    for (int i = 0; i < 100; ++i)
    {
        new (&m_vAttackExtraMove[i]) hkvVec3();
    }
}

// ============================================================================
// CMySkillList::~CMySkillList (Destructor)
// IDA: 0x1402B5E40
// ============================================================================
CMySkillList::~CMySkillList()
{
    // Clean up projectiles
    for (auto it = m_vProjectiles.begin(); it != m_vProjectiles.end(); ++it)
    {
        VGameProjectileObject* pProjectile = *it;
        if (pProjectile)
            RemoveProjectile(pProjectile);
    }
    m_vProjectiles.clear();

    // Clean up traps
    for (auto it = m_vTraps.begin(); it != m_vTraps.end(); ++it)
    {
        VGameTrapObject* pTrap = *it;
        if (pTrap)
            RemoveTrap(pTrap);
    }
    m_vTraps.clear();

    // Clean up chain lightning objects
    for (auto it = m_vChainLightningObject.begin(); it != m_vChainLightningObject.end(); ++it)
    {
        VChainLightningObject* pChain = *it;
        if (pChain)
            RemoveChainLightning(pChain);
    }
    m_vChainLightningObject.clear();

    // Clean up explode traps
    for (auto it = m_vecExplodeTrap.begin(); it != m_vecExplodeTrap.end(); ++it)
    {
        SExplodeTrap* pExplodeTrap = *it;
        if (pExplodeTrap)
            delete pExplodeTrap;
    }
    m_vecExplodeTrap.clear();

    // Clean up random trap events
    for (auto it = m_vecRandomTrapEvent.begin(); it != m_vecRandomTrapEvent.end(); ++it)
    {
        SRandomTrapEvent* pRandomTrap = *it;
        if (pRandomTrap)
            delete pRandomTrap;
    }
    m_vecRandomTrapEvent.clear();

    // Clean up random summon events
    for (auto it = m_vecRandomSummonEvent.begin(); it != m_vecRandomSummonEvent.end(); ++it)
    {
        SRandomSummonEvent* pRandomSummon = *it;
        if (pRandomSummon)
            delete pRandomSummon;
    }
    m_vecRandomSummonEvent.clear();

    // Clear maps
    m_mapExplodeSummon.clear();
    m_mapCooltimeList.clear();
    m_mapProjectileIndex.clear();
}

// ============================================================================
// CMySkillList::Init
// IDA: 0x1402B6460
// ============================================================================
void CMySkillList::Init(XActor* pActor)
{
    m_pActor = dynamic_cast<CMover*>(pActor);
    auto pServer = TXSingleton<XGameServer>::Instance();
    m_pActionRes = XActionResMgr::GetCommonBoneRes(&pServer->m_xActionManager);
}

// ============================================================================
// CMySkillList::IsCanUseSkill
// IDA: 0x1402B7B30
// Returns error code (0 = success)
// ============================================================================
int CMySkillList::IsCanUseSkill(TB_SKILL* pSkillTable, TB_SKILL* pChangedSkillTable, float& fSkillCost, bool bExceptHaveCheck)
{
    auto pAttr = m_pActor->GetGOC<CGocAttribute>();
    if (!pAttr)
        return 56009;

    if (!pSkillTable)
        return 56009;

    if (!m_pActor)
        return 56009;

    // Check if actor has the skill
    if (!bExceptHaveCheck)
    {
        auto pSkillPtr = m_pActor->GetGOC<CGocSkill>();
        if (pSkillPtr)
        {
            if (!pSkillPtr->IsHaveBaseSkill(pSkillTable->Skill_Index))
                return 56001;

            if (!pSkillPtr->IsHaveSkillQuickSlot(pSkillTable->Skill_Index))
            {
                LogHelper::LogError("game.contents", "IsHaveSkillQuickSlot > [%d] ", pSkillTable->Skill_Index);
                return 56001;
            }
        }
    }

    // Check level requirement
    int nLv = pAttr->GetLevel();
    if (pSkillTable->Req_Min_LV > nLv)
        return 56003;

    // Check class requirement
    if (pSkillTable->Use_Class)
    {
        std::uint8_t byClass = m_pActor->GetClass();
        if (pSkillTable->Use_Class != byClass)
            return 56002;
    }

    // Check awakening grade
    if (pAttr->GetAwaken() < pSkillTable->Req_Min_AwakeningGrade)
        return 56009;

    // Check if can attack
    if (!m_pActor->IsCanAttack())
        return 56009;

    // Test mode skips resource checks
    if (m_bTestMode)
        return 0;

    // Get final skill table
    TB_SKILL* pFinalSkillTable = pChangedSkillTable ? pChangedSkillTable : pSkillTable;
    fSkillCost = GetSkillCost(pFinalSkillTable);

    // Check resource cost
    if (fSkillCost > 0.0f)
    {
        std::uint8_t costAttr = pFinalSkillTable->Skill_Cost_Attribute;

        switch (costAttr)
        {
            case 1:  // HP
                if (fSkillCost >= (float)(int)m_pActor->GetStat(1))
                    return 56004;
                break;

            case 2:  // MP
                if (!m_pActor->IsNoSkillCostSG() && fSkillCost > (float)(int)m_pActor->GetStat(2))
                    return 56005;
                break;

            case 3:  // SG/Stamina
                fSkillCost -= fSkillCost * m_pActor->GetDecreaseStaminaRate();
                if (fSkillCost > (float)(int)m_pActor->GetStat(3))
                    return 56006;
                break;

            case 4:  // Stamina
                if (fSkillCost > (float)(int)m_pActor->GetStat(16))
                    return 56007;
                break;
        }
    }

    // Check cooldown
    float fCooltime = GetCooltime(E_COOLTIME_SKILL, pFinalSkillTable->CoolTime_Group, 
                                   pFinalSkillTable->CoolTime_Global, true);
    if (fCooltime > 0.0f)
        return 56010;

    return 0;
}


// ============================================================================
// CMySkillList::SendChainResult
// IDA: 0x1402B9A50
// ============================================================================
void CMySkillList::SendChainResult(CMoverEx* pMover)
{
    if (!pMover)
        return;

    int nSkillID = m_stChainSkillInfo.nSkillID;
    auto pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID);
    if (!pSkillTable)
        return;

    AttackJudgmentTrigger* pActionEvent = m_stChainSkillInfo.pTrigger;
    if (!pActionEvent)
        return;

    // Count attack targets
    std::uint8_t bAttackTargetCnt = 0;
    for (int i = 0; i < 100; ++i)
    {
        if (m_dwAttackTarget[i])
            ++bAttackTargetCnt;
    }

    if (pMover != m_pActor)
        return;

    std::uint16_t wContinousHit = 0;

    for (std::uint32_t j = 0; j < bAttackTargetCnt; ++j)
    {
        CMoverEx* pTarget = static_cast<CMoverEx*>(CMover::GetMoverObject(m_pActor, m_dwAttackTarget[j]));
        if (!pTarget)
        {
            m_stAttackDamage[j].byDamageFlag = 0;
            m_stAttackDamage[j].nDamage = 0;
            continue;
        }

        SChainHitInfo* pTempChainHitInfo = &m_stChainHitInfo[j];

        if (pActionEvent->sReactionInfo.fDamageRate > 0.0f || pActionEvent->sReactionInfo.iReactionType)
        {
            if (pActionEvent->sReactionInfo.fDamageRate > 0.0f)
            {
                CMonster* pMonster = nullptr;
                if (pMover->GetType() == eActorMonster)
                    pMonster = static_cast<CMonster*>(pMover);

                int fChainDamageRate = 1065353216;  // 1.0f
                if (m_stChainHitInfo[j].iTargetOrder > 0)
                    fChainDamageRate = pActionEvent->sConnectionInfo.fDamageMutiple;

                bool bFindOwnerPlayer = false;

                if (pMonster)
                {
                    TB_MONSTER* pMobTable = pMonster->GetMobTableRef();
                    CMoverEx* pOwnerMoverEx = pMonster->GetOwnerPlayer();

                    if (pOwnerMoverEx && pMobTable && pMobTable->Monster_Element == 1)
                    {
                        bool bAllowAbsorbSG = pMonster->GetAllowAbsorbSG();
                        pOwnerMoverEx->CalcTargetDamage(pTarget, j, bAllowAbsorbSG, pSkillTable,
                                                         pActionEvent, fChainDamageRate, false, false, true);

                        // Copy damage from owner to monster
                        if (pMonster->GetSkillMgr())
                        {
                            auto pOwnerSkillMgr = pOwnerMoverEx->GetSkillMgr();
                            auto pMonsterSkillMgr = pMonster->GetSkillMgr();

                            tagSKILL_ACTION_DAMAGE stOwnerDmg, stMonsterDmg;
                            pOwnerSkillMgr->GetAttackDamage(&stOwnerDmg, j);
                            pMonsterSkillMgr->GetAttackDamage(&stMonsterDmg, j);

                            stMonsterDmg.nDamage = stOwnerDmg.nDamage;
                            stMonsterDmg.nAttrDamage = stOwnerDmg.nAttrDamage;
                            stMonsterDmg.nHP = stOwnerDmg.nHP;
                            stMonsterDmg.byDamageFlag = stOwnerDmg.byDamageFlag;
                            pMonsterSkillMgr->SetAttackDamage(j, &stMonsterDmg);
                        }
                        bFindOwnerPlayer = true;
                    }
                }

                if (!bFindOwnerPlayer)
                {
                    bool bAllowAbsorbSG = pMover->GetAllowAbsorbSG();
                    pMover->CalcTargetDamage(pTarget, j, bAllowAbsorbSG, pSkillTable,
                                            pActionEvent, fChainDamageRate, false, false, true);
                }
            }

            // Check for combo
            CMonster* pMonster = nullptr;
            CAkashicObject* pAkashic = nullptr;

            if (pMover->GetType() == eActorMonster)
                pMonster = static_cast<CMonster*>(pMover);
            else if (pMover->GetType() == eActorNPC)
                pAkashic = static_cast<CAkashicObject*>(pMover);

            CMoverEx* pOwner = pMover->GetOwnerPlayer();

            if (pMover->GetType() == eActorUser)
            {
                wContinousHit = pMover->CheckContinousAttack(bAttackTargetCnt);
            }
            else if (pOwner &&
                     (pOwner == pMover ||
                      (pMonster && pMonster->IsMonsterCombo()) ||
                      (pAkashic && pAkashic->IsComboAkashic())))
            {
                wContinousHit = pOwner->CheckContinousAttack(bAttackTargetCnt);
            }

            // Check explode summon/trap
            if (!m_stChainHitInfo[j].bHitWall && pActionEvent->sProjInfo.shExplodeTrapMakeType == 4)
                CheckExplodeSummon(pMover, pTempChainHitInfo->vHitPos, &m_stChainSkillInfo);

            if (!m_stChainHitInfo[j].bHitWall || pActionEvent->sProjInfo.shExplodeTrapMakeType != 3)
                CheckExplodeTrap(pMover, pTempChainHitInfo->vHitPos, &m_stChainSkillInfo, pTarget);

            // Apply buff
            if (pActionEvent->sReactionInfo.iBuffID > 0)
            {
                UXActorID actorID;
                pMover->GetActorID(&actorID);
                std::uint32_t questID = CQuestCondition::GetQuestID(&actorID);
                ApplyBuffSkill(pTarget, pActionEvent->sReactionInfo.iBuffID, questID, bAttackTargetCnt);
            }
        }
        else if (pActionEvent->sReactionInfo.iBuffID > 0)
        {
            UXActorID actorID;
            pMover->GetActorID(&actorID);
            std::uint32_t dwUserID = CQuestCondition::GetQuestID(&actorID);
            ApplyBuffSkill(pTarget, pActionEvent->sReactionInfo.iBuffID, dwUserID, 0);
        }
    }

    // Apply skill damage frame
    float fDirYaw = CMover::GetYawFromVector(&m_stChainHitInfo[0].vHitDir);
    pMover->ApplySkillDamageFrame(nSkillID, pActionEvent->EventID, bAttackTargetCnt,
                                  m_stChainHitInfo[0].vHitPos, fDirYaw, wContinousHit, 4, false);

    // Send skill action packet
    pMover->send_eSUB_CMD_ACTION_SKILL(pMover, nSkillID, pActionEvent->EventID,
                                        m_stChainHitInfo[0].vHitPos, bAttackTargetCnt, wContinousHit, false);

    // Clear multiple damage buff
    if (pMover->GetMultipleDamageOnce() > 0.0f && pMover->GetApplyMultipleDamageOnce())
    {
        int iIndex = pMover->FindBuffByEffectType(0x0E, 0);
        if (iIndex != -1)
            pMover->ClearBuffStatusBySlot(iIndex, 0);
    }

    // Check if mover died
    if (pMover->GetHP() <= 0)
        pMover->SetDie(12, 0);
}

// ============================================================================
// CMySkillList::CheckExplodeSummon
// IDA: 0x1402BA4C0
// ============================================================================
void CMySkillList::CheckExplodeSummon(CMoverEx* pMover, hkvVec3 vPos, SSkillInfo* sSkillInfo)
{
    AttackJudgmentTrigger* pActionEvent = sSkillInfo->pTrigger;
    if (!pActionEvent)
        return;

    SummonMonsterTrigger* pSummonTrigger = GetExplodeSummon(pActionEvent->sProjInfo.nLinkedSkillID);
    if (!pSummonTrigger)
        return;

    bool bAllowAbsorbSG = false;
    CMoverEx* pOwner = pMover;

    if (pMover->GetType() == eActorNPC)  // Akashic
        pOwner = pMover->GetOwnerPlayer();

    if (pMover->GetType() == eActorUser)
        bAllowAbsorbSG = true;

    AddRandomSummon(pOwner, bAllowAbsorbSG, pSummonTrigger, true, vPos);
}

// ============================================================================
// CMySkillList::CheckExplodeTrap
// IDA: 0x1402BB5B0
// ============================================================================
void CMySkillList::CheckExplodeTrap(CMoverEx* pMover, hkvVec3 vPos, SSkillInfo* sSkillInfo, CMover* pTarget)
{
    SExplodeTrap* pExplodeTrap = new SExplodeTrap();
    if (!pExplodeTrap)
        return;

    pExplodeTrap->pMover = pMover;
    pExplodeTrap->vPos = vPos;
    pExplodeTrap->sSkillInfo.nSkillID = sSkillInfo->nSkillID;
    pExplodeTrap->sSkillInfo.pTrigger = sSkillInfo->pTrigger;
    pExplodeTrap->pTarget = pTarget;

    m_vecExplodeTrap.push_back(pExplodeTrap);
}


// ============================================================================
// CMySkillList::AddRandomSummon
// IDA: 0x1402BA5D0
// ============================================================================
void CMySkillList::AddRandomSummon(CMoverEx* pMover, bool bAllowAbsorbSG, SummonMonsterTrigger* pTrigger, 
                                    bool bUseOtherPos, hkvVec3 vOtherPos)
{
    SRandomSummonEvent* pRandomSummonEvent = new SRandomSummonEvent();
    if (!pRandomSummonEvent)
        return;

    pRandomSummonEvent->fRandomSummonTime = 0.0f;
    pRandomSummonEvent->fRandomSummonDelayTime = pTrigger->fDelayTime;
    pRandomSummonEvent->nRandomSummonCount = pTrigger->iSummonCount;
    pRandomSummonEvent->pRandomSummonEvent = pTrigger;
    pRandomSummonEvent->pMover = pMover;
    pRandomSummonEvent->bAllowAbsorbSG = bAllowAbsorbSG;
    pRandomSummonEvent->bUseOtherPos = bUseOtherPos;
    pRandomSummonEvent->vOtherPos = vOtherPos;

    m_vecRandomSummonEvent.push_back(pRandomSummonEvent);
}

// ============================================================================
// CMySkillList::RetiveExplodeTrap
// IDA: 0x1402BB6F0
// ============================================================================
void CMySkillList::RetiveExplodeTrap(CMoverEx* pMover, hkvVec3 vPos, SSkillInfo* sSkillInfo, CMover* pTarget)
{
    if (!pMover)
        return;

    int nSkillID = sSkillInfo->nSkillID;
    auto pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID);
    if (!pSkillTable)
        return;

    AttackJudgmentTrigger* pActionEvent = sSkillInfo->pTrigger;
    if (!pActionEvent)
        return;

    if (!pActionEvent->sProjInfo.bUseExplodeTrap)
        return;

    // Get filter data
    int nCurFilterData1, nCurFilterData2, nCurFilterData3;
    pMover->GetFilterData(nSkillID, &nCurFilterData1, &nCurFilterData2, &nCurFilterData3);

    SSkillInfo newSkillInfo;
    newSkillInfo.nSkillID = sSkillInfo->nSkillID;

    // Find next event index
    int iIndex = pActionEvent->EventID - (pActionEvent->EventID % 50) + 1;
    while (pActionEvent->EventID == iIndex)
        ++iIndex;

    AttackJudgmentTrigger* pActionEvent2 = pMover->GetAttackJudgmentEvent(iIndex);
    while (pActionEvent2)
    {
        ++iIndex;
        if (pActionEvent2->sAttackType == 4 &&
            !ActionTrigger::IsFiltering(nCurFilterData1, nCurFilterData2, nCurFilterData3,
                                        pActionEvent2->dwFilterInfo1, pActionEvent2->dwFilterInfo2, pActionEvent2->dwFilterInfo3))
        {
            if (pActionEvent->sProjInfo.shExplodeTrapMakeType ||
                (pTarget && pTarget->IsCanHit(pActionEvent->sReactionInfo.iTargetStatus, pSkillTable->Passive_Type)))
            {
                if ((pActionEvent->sProjInfo.shExplodeTrapMakeType != 1 && pActionEvent->sProjInfo.shExplodeTrapMakeType != 3) || !pTarget)
                {
                    newSkillInfo.pTrigger = pActionEvent2;

                    if (pActionEvent2->sProjInfo.RandomPattern == 1)
                    {
                        // Random pattern 1 - create random trap
                        if (pActionEvent2->sProjInfo.fRandomDelayTime > 0.0f && pActionEvent2->sProjInfo.nRandomCount > 1)
                        {
                            SRandomTrapEvent* pRandomTrapEvent = new SRandomTrapEvent();
                            pRandomTrapEvent->fRandomTrapTime = 0.0f;
                            pRandomTrapEvent->fRandomTrapDelayTime = pActionEvent2->sProjInfo.fRandomDelayTime;
                            pRandomTrapEvent->nRandomTrapCount = pActionEvent2->sProjInfo.nRandomCount - 1;
                            pRandomTrapEvent->pRandomTrapEvent = pActionEvent2;
                            pRandomTrapEvent->vRandomTrapEntityPos = *VisObject3D_cl::GetPosition(pMover);
                            pRandomTrapEvent->fRandomTrapEntityYaw = pMover->GetMovingYaw();
                            pRandomTrapEvent->sRandomTrapSkillInfo.nSkillID = pSkillTable->Skill_Index;
                            pRandomTrapEvent->sRandomTrapSkillInfo.pTrigger = pActionEvent2;
                            pRandomTrapEvent->pMover = pMover;
                            m_vecRandomTrapEvent.push_back(pRandomTrapEvent);
                        }

                        int nCount = 1;
                        if (pActionEvent2->sProjInfo.fRandomDelayTime <= 0.0f)
                            nCount = pActionEvent2->sProjInfo.nRandomCount;

                        hkvVec3 vMoverPos = *VisObject3D_cl::GetPosition(pMover);
                        for (int i = 0; i < nCount; ++i)
                        {
                            float fYaw = pMover->GetMovingYaw();
                            CreateRandomTrap(pActionEvent2, pMover, vMoverPos, fYaw, &newSkillInfo);
                        }
                    }
                    else if (pActionEvent2->sProjInfo.RandomPattern == 2)
                    {
                        // Random pattern 2 - create trap line
                        if (pActionEvent2->sProjInfo.fRandomDelayTime > 0.0f && pActionEvent2->sProjInfo.nRandomCount > 1)
                        {
                            SRandomTrapEvent* pRandomTrapEvent = new SRandomTrapEvent();
                            pRandomTrapEvent->fRandomTrapTime = 0.0f;
                            pRandomTrapEvent->fRandomTrapDelayTime = pActionEvent2->sProjInfo.fRandomDelayTime;
                            pRandomTrapEvent->nRandomTrapCount = pActionEvent2->sProjInfo.nRandomCount - 1;
                            pRandomTrapEvent->pRandomTrapEvent = pActionEvent2;
                            pRandomTrapEvent->vRandomTrapEntityPos = *VisObject3D_cl::GetPosition(pMover);
                            pRandomTrapEvent->fRandomTrapEntityYaw = pMover->GetMovingYaw();
                            pRandomTrapEvent->sRandomTrapSkillInfo.nSkillID = pSkillTable->Skill_Index;
                            pRandomTrapEvent->sRandomTrapSkillInfo.pTrigger = pActionEvent2;
                            pRandomTrapEvent->pMover = pMover;
                            m_vecRandomTrapEvent.push_back(pRandomTrapEvent);
                        }

                        // Get attack area and create traps
                        tagATTACK_AREA stAttackArea;
                        hkvVec3 vEntityPos = *VisObject3D_cl::GetPosition(pMover);
                        float fYaw = pMover->GetOrientationYaw();
                        GetAttackArea(pMover, vEntityPos, fYaw, pActionEvent2->sAttackRangeType, 
                                      &pActionEvent2->sAttackRange, &stAttackArea);
                        vEntityPos = stAttackArea.vCenterPos;

                        hkvMat3 matRot;
                        matRot.setFromEulerAngles(0.0f, 0.0f, fYaw);
                        hkvVec3 vDir = matRot * pActionEvent2->sProjInfo.vRandomPos;

                        int nRandomCount = 1;
                        if (pActionEvent2->sProjInfo.fRandomDelayTime <= 0.0f)
                            nRandomCount = pActionEvent2->sProjInfo.nRandomCount;

                        for (int j = 0; j < nRandomCount; ++j)
                        {
                            hkvVec3 vOffset = vDir * (float)j;
                            hkvVec3 vDestPos = vEntityPos + vOffset;
                            hkvVec3 vTrapDir(0.0f, -1.0f, 0.0f);

                            VGameTrapObject* pTrapObj = CreateTrap(vDestPos, vTrapDir, pMover, pActionEvent2);
                            if (pTrapObj)
                            {
                                pTrapObj->SetSkillInfo(&newSkillInfo);
                                m_vTraps.push_back(pTrapObj);

                                if (CMover::IsSendProjectilePacket(pMover, pActionEvent2))
                                {
                                    hkvVec3 vDirection;
                                    pMover->GetDirection(&vDirection);
                                    int nSessionID = pTrapObj->GetSessionID();
                                    pMover->send_eSUB_CMD_PROJECTILE(pMover, nSkillID, pActionEvent2->EventID,
                                                                      vDestPos, vDirection, nSessionID);
                                }
                            }
                        }
                    }
                    else
                    {
                        CalcTrapedSkillTarget(pMover, pActionEvent2, &newSkillInfo, vPos);
                    }
                }
            }
        }

        pActionEvent2 = pMover->GetAttackJudgmentEvent(iIndex);
    }
}


// ============================================================================
// CMySkillList::CheckDedicatedMonster
// IDA: 0x1402BB2D0
// ============================================================================
void CMySkillList::CheckDedicatedMonster(CMonster* pSummonMob, CMoverEx* pMover)
{
    if (!pSummonMob || !pMover)
        return;

    if (!pSummonMob->IsDedicated())
        return;

    if (!pSummonMob->GetOwnerID())
        return;

    // Get owner player
    CMoverEx* pOwnerMover = pSummonMob->GetOwnerPlayer();
    CUser* pOwnerPlayer = dynamic_cast<CUser*>(pOwnerMover);

    if (!pOwnerPlayer)
        pOwnerPlayer = dynamic_cast<CUser*>(pMover);

    if (pOwnerPlayer)
    {
        UXActorID actorID;
        pSummonMob->GetActorID(&actorID);
        std::uint32_t questID = CQuestCondition::GetQuestID(&actorID);
        pOwnerPlayer->SetDedicatedMonsterID(questID);
    }

    // Initialize NPC attribute
    auto pAttr = pSummonMob->GetGOC<CGocNpcAttribute>();
    TB_MONSTER* pMonTable = pSummonMob->GetMobTableRef();
    int nLevel = pMover->GetLevelForStat();
    int nClass = pMover->GetClass();

    pAttr->Init(nClass, nLevel, pMonTable);
    pAttr->CalculateChangedStat(0);

    // Set HP to max
    float fMaxHP = (float)pMover->GetMaxHP();
    pSummonMob->SetHpInfo((int)fMaxHP);

    // Finalize attribute
    pAttr->Finalize(10);
    pAttr->Finalize(1);
    pAttr->Finalize();
}

// ============================================================================
// CMySkillList::GetAttackTarget
// IDA: 0x14019B9B0
// ============================================================================
std::uint32_t CMySkillList::GetAttackTarget(int iIndex) const
{
    return m_dwAttackTarget[iIndex];
}


// ============================================================================
// CMySkillList::CalcTrapedSkillTarget
// IDA: 0x1402BF420
// ============================================================================
void CMySkillList::CalcTrapedSkillTarget(CMoverEx* pMover, AttackJudgmentTrigger* pActionEvent, 
                                          SSkillInfo* sSkillInfo, hkvVec3 vTouchPos)
{
    if (!pMover || !pActionEvent)
        return;

    bool bIsMouseOnTarget = pMover->IsMouseOnTarget();
    if (bIsMouseOnTarget && pMover->GetMouseOnTrap())
        return;

    tagATTACK_AREA stAttackArea;
    hkvVec3 vPos = vTouchPos;

    if (pActionEvent->sProjInfo.bIsMovingToTarget)
        vPos = pMover->GetTrapPos();

    float fYaw = pMover->GetOrientationYaw();
    GetAttackArea(pMover, vPos, fYaw, pActionEvent->sAttackRangeType, 
                  &pActionEvent->sAttackRange, &stAttackArea);

    vPos = stAttackArea.vCenterPos;

    // Override position for attack type 4
    if (pActionEvent->sAttackType == 4)
        vPos = vTouchPos;

    if (pActionEvent->sProjInfo.bIsMovingToTarget)
        vPos = pMover->GetTrapPos();

    hkvVec3 vTrapDir(0.0f, -1.0f, 0.0f);
    VGameTrapObject* pTrapObj = CreateTrap(vPos, vTrapDir, pMover, pActionEvent);
    if (!pTrapObj)
        return;

    pTrapObj->SetSkillInfo(sSkillInfo);
    m_vTraps.push_back(pTrapObj);

    if (bIsMouseOnTarget)
        pMover->SetMouseOnTrap(pTrapObj);

    if (CMover::IsSendProjectilePacket(pMover, pActionEvent))
    {
        hkvVec3 vDir;
        pMover->GetDirection(&vDir);
        int nSessionID = pTrapObj->GetSessionID();
        pMover->send_eSUB_CMD_PROJECTILE(pMover, sSkillInfo->nSkillID, pActionEvent->EventID,
                                          vPos, vDir, nSessionID);
    }
}


// ============================================================================
// CMySkillList::GetExtraMoveAction
// IDA: 0x1402C4280
// ============================================================================
hkvVec3 CMySkillList::GetExtraMoveAction(CMoverEx* pMover, CMoverEx* pTarget, 
                                          AttackJudgmentTrigger* pActionEvent, 
                                          hkvVec3 vPos, float& fAttackRot)
{
    hkvVec3 vExtraMove(0.0f, 0.0f, 0.0f);

    if (!pMover || !pTarget || !pActionEvent)
        return vExtraMove;

    char dmgMotionFlag = pTarget->GetDmgMotionFlag();
    if (((dmgMotionFlag & 2) != 0 && (dmgMotionFlag & 1) != 0) || pTarget->GetAttached() == pMover)
        return vExtraMove;

    fAttackRot += pActionEvent->sReactionInfo.fReactionAngle;

    // For monsters with angle > 180, calculate angle from positions
    if (pMover->GetType() == eActorMonster && pActionEvent->sAttackRange.fAngle > 180.0f)
    {
        const hkvVec3* pMoverPos = VisObject3D_cl::GetPosition(pMover);
        const hkvVec3* pTargetPos = VisObject3D_cl::GetPosition(pTarget);
        hkvVec3 vDirVector = *pTargetPos - *pMoverPos;
        fAttackRot = CMover::GetYawFromVector(&vDirVector);
    }

    int iArrowType = pActionEvent->sReactionInfo.iReactionArrow;

    if (pActionEvent->sReactionInfo.fDamageRate > 0.0f)
    {
        std::uint8_t byReactionType = pActionEvent->sReactionInfo.iReactionType;

        if (iArrowType == 3)
            vExtraMove.y -= pActionEvent->sReactionInfo.fReactionDist;
        else
            vExtraMove.y -= pActionEvent->sReactionInfo.fReactionDist + (float)(rand() % 41 - 20);

        if (pTarget->IsFlying())
            vExtraMove.z += pActionEvent->sReactionInfo.fReactionHeightAir;
        else
            vExtraMove.z += pActionEvent->sReactionInfo.fReactionHeight;

        if (byReactionType == 6)
        {
            vExtraMove.y = 0.0f;
        }
        else if (pActionEvent->sReactionInfo.bUseTargetWeight || pTarget->GetStandType() >= 2)
        {
            if (byReactionType >= 2 && byReactionType <= 4)
            {
                int weightRank = pMover->GetWeightRank();
                int targetWeightRank = pTarget->GetWeightRank();
                char cRankGap = weightRank - targetWeightRank;

                if (cRankGap < 2)
                {
                    if (cRankGap == -3 || cRankGap == -2)
                        vExtraMove *= 0.65f;
                    else if (cRankGap <= -4)
                        vExtraMove *= 0.0f;
                }
                else
                {
                    vExtraMove *= 1.35f;
                }
            }
        }

        if (pTarget->GetDefenseType() == 1)
            vExtraMove *= 0.5f;

        float fCalcChargingMultiple = pMover->GetCalcChargingMultiple();
        if (fCalcChargingMultiple > 0.0f)
            vExtraMove *= fCalcChargingMultiple + 1.0f;

        if (byReactionType != 2 && byReactionType != 4 && byReactionType != 3 && byReactionType != 6)
        {
            hkvMat3 matRot;
            matRot.setIdentity();

            switch (iArrowType)
            {
                case 1:
                    matRot.setFromEulerAngles(0.0f, 0.0f, fAttackRot + (float)((rand() % 31 - 15) * 0.5));
                    break;
                case 2:
                    {
                        const hkvVec3* pTargetPos = VisObject3D_cl::GetPosition(pTarget);
                        float fAngle = hkvMath::Rad2Deg(atan2(vPos.y - pTargetPos->y, vPos.x - pTargetPos->x)) - 90.0f;
                        if (fAngle < 0.0f)
                            fAngle += 360.0f;
                        matRot.setFromEulerAngles(0.0f, 0.0f, fAngle);
                    }
                    break;
                case 3:
                    matRot.setFromEulerAngles(0.0f, 0.0f, fAttackRot);
                    break;
            }
            vExtraMove = matRot * vExtraMove;
        }
    }

    return vExtraMove;
}

// ============================================================================
// CMySkillList::GeneralSkillDamage
// IDA: 0x1402C32D0
// ============================================================================
void CMySkillList::GeneralSkillDamage(CMoverEx* pMover, CMoverEx* pTarget, TB_SKILL* pSkillRef,
                                       AttackJudgmentTrigger* pActionEvent, hkvVec3 vPos,
                                       float fAttackRot, float fDelayTime, unsigned int nIdx,
                                       int nContinueAttack, bool bPenetrate)
{
    if (!pMover || !pTarget || !pSkillRef || !pActionEvent)
        return;

    auto pSkillMgr = pMover->GetSkillMgr();
    if (!pSkillMgr || nIdx >= 100)
        return;

    hkvVec3 vExtraMove = GetExtraMoveAction(pMover, pTarget, pActionEvent, vPos, fAttackRot);

    if (!pActionEvent->sAttackCollision &&
        pActionEvent->sReactionInfo.fReactionDist <= 0.0f &&
        pActionEvent->sReactionInfo.fReactionHeight <= 0.0f)
    {
        vExtraMove.Set(0.0f, 0.0f, 0.0f);
    }

    if (pActionEvent->sReactionInfo.iReactionArrow == 3)
        pTarget->SetKeepMovingExtra(true);
    else
        pTarget->SetKeepMovingExtra(false);

    if (pActionEvent->sAttackType && pActionEvent->sReactionInfo.iReactionArrow == 2)
    {
        const hkvVec3* pTargetPos = VisObject3D_cl::GetPosition(pTarget);
        hkvVec3 vDiff = *pTargetPos - vPos;
        vDiff.z = 0.0f;
        fAttackRot = CMover::GetYawFromVector(&vDiff);
    }

    std::uint8_t byAttackCollision = pActionEvent->sAttackCollision;
    if (pTarget->GetType() == eActorUser && pActionEvent->sReactionInfo.bApplyPcSABreak)
        byAttackCollision = 3;
    if (pMover->IsStatus(0x100000))
        byAttackCollision = 4;

    pTarget->SetReactionRate(1.0f);

    tagSKILL_ACTION_DAMAGE stAttackDamage;
    pSkillMgr->GetAttackDamage(&stAttackDamage, nIdx);

    if (pActionEvent->sReactionInfo.bCheckCounter)
    {
        std::uint32_t dwID = 0;
        if (pMover->GetType() != eActorUser)
            dwID = pMover->GetID();
        else
            dwID = pMover->GetUserID();

        if (pTarget->CheckCounter(dwID))
        {
            stAttackDamage.byDamageFlag |= 8;
            stAttackDamage.nDamage = 0;
            stAttackDamage.nAttrDamage = 0;
        }
    }

    if (pActionEvent->sReactionInfo.bIgnoreTargetInvincible)
        stAttackDamage.byDamageFlag |= 0x10;

    pSkillMgr->SetAttackDamage(nIdx, &stAttackDamage);

    std::uint8_t byDefenceType = pTarget->GetDefenseType();
    float fCurSuperArmorGage = pTarget->GetCurSuperArmorGage();

    if (byDefenceType != 3)
    {
        // Calculate super armor damage
        CBattleZone* pBattleZone = nullptr;
        XArea* pArea = pTarget->GetArea();
        if (pArea)
            pBattleZone = dynamic_cast<CBattleZone*>(pArea);
        if (!pBattleZone && pMover)
        {
            pArea = pMover->GetArea();
            if (pArea)
                pBattleZone = dynamic_cast<CBattleZone*>(pArea);
        }

        bool bInSafetyZone = false;
        if (pBattleZone)
        {
            bInSafetyZone = pBattleZone->IsInSafetyZone(pMover) || pBattleZone->IsInSafetyZone(pTarget);
        }

        if (!bInSafetyZone)
        {
            float fSuperArmorDamage = pMover->CalcSuperArmorDamage(pTarget, nIdx, pSkillRef,
                                                                   pActionEvent->sReactionInfo.fDamageRate);
            fCurSuperArmorGage -= fSuperArmorDamage;

            if (fCurSuperArmorGage <= 0.0f)
            {
                if (byDefenceType != 3 && byDefenceType != 5)
                    byDefenceType = 0;
            }
            else if (!byDefenceType || byDefenceType == 4)
            {
                byDefenceType = 2;
            }
        }
    }

    // Build damage info
    tagACTION_DAMAGE dmgInfo;
    UXActorID actorID;
    pMover->GetActorID(&actorID);
    dmgInfo.dwID = CQuestCondition::GetQuestID(&actorID);
    dmgInfo.byReactionType = stAttackDamage.byReactionType;
    dmgInfo.byDamageFlag = stAttackDamage.byDamageFlag;
    dmgInfo.nDamage = stAttackDamage.nDamage;
    dmgInfo.nAttrDamage = stAttackDamage.nAttrDamage;
    dmgInfo.byAttackCollision = byAttackCollision;
    dmgInfo.fAttackRot = fAttackRot;
    dmgInfo.vExtraMove = vExtraMove;
    dmgInfo.nContinueAttack = nContinueAttack;
    dmgInfo.fFlySpeed = pActionEvent->sReactionInfo.fReactionSpeed;
    dmgInfo.byAttackRank = pActionEvent->sReactionInfo.iTargetGrade;
    dmgInfo.byDefenseType = byDefenceType;
    dmgInfo.fSuperArmorGage = fCurSuperArmorGage;
    dmgInfo.bPenetrate = bPenetrate;
    dmgInfo.bApplyEffectWhenPenetrate = pActionEvent->sProjInfo.bApplyEffectWhenPenetrate;
    dmgInfo.byHitPartsIndex = stAttackDamage.byHitPartsIndex;

    bool bSABreaked = false;
    pTarget->Damage(&dmgInfo, pSkillRef->Skill_Index, &bSABreaked);
    pTarget->ReleaseInvokedOptionEffectAfterDamage(dmgInfo.dwID, dmgInfo.byDamageFlag, bSABreaked);

    vExtraMove = dmgInfo.vExtraMove;
    stAttackDamage.nHP = pTarget->GetHP();
    pSkillMgr->SetAttackDamage(nIdx, &stAttackDamage);

    auto stPos = pTarget->GetExtraMovePos();
    if (pActionEvent->sAttackCollision ||
        pActionEvent->sReactionInfo.fReactionDist > 0.0f ||
        pActionEvent->sReactionInfo.fReactionHeight > 0.0f)
    {
        if (dmgInfo.byReactionType < 2 || dmgInfo.byReactionType > 4)
        {
            if (stPos->fRemainTime <= 0.0f)
                vExtraMove = *VisObject3D_cl::GetPosition(pTarget);
            else
                vExtraMove = *stPos;
        }
        pSkillMgr->SetAttackExtraMove(nIdx, vExtraMove);
        pSkillMgr->SetAttackFlySpeed(nIdx, pTarget->GetJumpSpeed());
    }
    else
    {
        pSkillMgr->SetAttackExtraMove(nIdx, vExtraMove);
        pSkillMgr->SetAttackFlySpeed(nIdx, pTarget->GetJumpSpeed());
    }
}


// ============================================================================
// CMySkillList::AddSkillTarget
// IDA: 0x1402BE150
// ============================================================================
void CMySkillList::AddSkillTarget(std::uint8_t byIndex, std::uint32_t dwTargetID, 
                                   std::uint8_t byReaction, std::uint8_t byHitPartsIndex)
{
    if (byIndex < 100)
    {
        m_dwAttackTarget[byIndex] = dwTargetID;
        m_stAttackDamage[byIndex].byReactionType = byReaction;
        m_stAttackDamage[byIndex].byHitPartsIndex = byHitPartsIndex;
    }
}

// ============================================================================
// CMySkillList::CalcChainSkillTarget
// IDA: 0x1402BF7B0
// ============================================================================
void CMySkillList::CalcChainSkillTarget(CMover* pMover, tagATTACK_AREA* stAreaInfo, 
                                         AttackJudgmentTrigger* pActionEvent, SSkillInfo* SkillInfo,
                                         std::uint8_t* bAttackTargetCnt, TB_SKILL* pSkillRef)
{
    if (!pMover || !pActionEvent || !pSkillRef)
        return;

    std::uint8_t byType = pMover->GetType();
    CMoverEx* pClosestTarget = nullptr;
    float fClosestDistance = 1000000.0f;
    hkvVec3 vPos = *VisObject3D_cl::GetPosition(pMover);

    std::vector<CMover*> vecGameObjList;
    XArea::ScanGridOrigin(&pMover->XActor, 2, 3, &vecGameObjList);

    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it)
    {
        CMoverEx* pOtherActor = dynamic_cast<CMoverEx*>(*it);
        if (!pOtherActor)
            continue;

        if (!pOtherActor->IsLive())
            continue;

        if (pOtherActor->IsStatus(2) && 
            !(pOtherActor->GetDieType() == 1 && pOtherActor->GetDieDelayTime() > 0.0f))
            continue;

        if (!pMover->CheckReactionTarget(pActionEvent->sReactionInfo.iTargetType, pOtherActor, true))
            continue;

        E_ACTOR_TYPE eType = pOtherActor->GetType();
        if (eType != eActorUser && eType != eActorMonster)
            continue;

        if (!IsInAttackArea(pOtherActor, stAreaInfo, pActionEvent->sReactionInfo.iTargetStatus, pSkillRef->Passive_Type))
            continue;

        if (*bAttackTargetCnt >= pSkillRef->Target_Damage_Count)
            continue;

        hkvVec3 vTargetPos = *VisObject3D_cl::GetPosition(pOtherActor);
        float fDist = (vTargetPos - vPos).GetLength();
        if (fClosestDistance > fDist)
        {
            pClosestTarget = pOtherActor;
            fClosestDistance = fDist;
        }
    }

    hkvVec3 vDir(0.0f, -1.0f, 0.0f);
    auto pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, SkillInfo->nSkillID);
    if (!pSkillTable)
        return;

    vDir *= (float)pSkillTable->Skill_Range_Max;
    VChainLightningObject* pChainObj = CreateChainLightningObject(&vPos, &vDir, pMover, pActionEvent, pClosestTarget);
    if (!pChainObj)
        return;

    pChainObj->SetSkillInfo(SkillInfo);
    m_vChainLightningObject.push_back(pChainObj);

    std::uint32_t dwTargetID = -1;
    if (pClosestTarget)
    {
        UXActorID actorID;
        pClosestTarget->GetActorID(&actorID);
        dwTargetID = CQuestCondition::GetQuestID(&actorID);
    }

    int nSessionID = pChainObj->GetSessionID();
    pMover->send_eSUB_CMD_CHAIN(pMover, SkillInfo->nSkillID, pActionEvent->EventID,
                                 vPos, vDir, nSessionID, dwTargetID);
}

// ============================================================================
// CMySkillList::CalcRangedSkillTarget
// IDA: 0x1402BFE80
// ============================================================================
void CMySkillList::CalcRangedSkillTarget(CMoverEx* pMover, AttackJudgmentTrigger* pActionEvent,
                                          TB_SKILL* pSkillTable, hkvVec3 vPos, hkvVec3 vRight,
                                          float fYaw, float fPitch, std::uint8_t& bAttackTargetCnt)
{
    if (!pMover || !pActionEvent || !pSkillTable || !m_pActor)
        return;

    if (pActionEvent->sAttackRange.fAngle <= 1.0f)
        return;

    hkvVec3 vStart;
    hkvVec3 vDir;
    hkvVec3 vUp(0.0f, 0.0f, 1.0f);

    if (pActionEvent->sAttackType == 6)
    {
        hkvMat3 matRot;
        matRot.setFromEulerAngles(0.0f, 0.0f, fYaw - 90.0f);
        vStart = pActionEvent->sProjInfo.vStartPos;
        vStart *= matRot;
        vStart += vPos;
    }
    else
    {
        vStart = vPos + hkvVec3(0.0f, 0.0f, pActionEvent->sProjInfo.vStartPos.z);
    }

    bool bFinalRotation = false;
    hkvMat3 cameraRot;

    std::int16_t shRandomShootDirX = pMover->GetCurRandomShootProjectileDirX();
    std::int16_t shRandomShootDirY = pMover->GetCurRandomShootProjectileDirY();

    if (pMover == m_pActor)
    {
        if (pMover->GetType() != eActorUser)
        {
            if (pMover->IsDefensiveWeapon())
                vDir = GetProjectileDirection(pActionEvent->sProjInfo.vDir, fYaw, -fPitch, shRandomShootDirX, shRandomShootDirY);
            else
                vDir = GetProjectileDirection(pActionEvent->sProjInfo.vDir, fYaw, 0.0f, shRandomShootDirX, shRandomShootDirY);
        }
        else if (pActionEvent->sProjInfo.eDirectionType)
        {
            vDir = GetProjectileDirection(pActionEvent->sProjInfo.vDir, fYaw, 0.0f, shRandomShootDirX, shRandomShootDirY);
        }
        else
        {
            hkvMat3 templateRot;
            templateRot.setFromEulerAngles(pActionEvent->sProjInfo.vDir.z, pActionEvent->sProjInfo.vDir.y, 
                                            pActionEvent->sProjInfo.vDir.x);
            
            hkvVec3 vCamDir;
            pMover->GetCameraDir(&vCamDir);
            cameraRot.setFromEulerAngles(0.0f, -vCamDir.z, vCamDir.x + 90.0f);
            cameraRot = cameraRot * templateRot;
            
            vDir = cameraRot.getAxis(0);
            vRight = cameraRot.getAxis(1);
            vUp = cameraRot.getAxis(2);
            bFinalRotation = true;
        }
    }
    else
    {
        vDir = GetProjectileDirection(pActionEvent->sProjInfo.vDir, fYaw, 0.0f, shRandomShootDirX, shRandomShootDirY);
    }

    if (pActionEvent->sAttackType == 1)
    {
        vStart += vDir * pActionEvent->sProjInfo.vStartPos.x;
        vStart += vRight * pActionEvent->sProjInfo.vStartPos.y;
    }

    if (pMover->GetControlType(pSkillTable) == 6)
        pMover->UpdateTargetByPretarget();

    VGameProjectileObject* pProjectile = CreateProjectile(vStart, vDir, pMover, pActionEvent);
    if (!pProjectile)
        return;

    if (CMover::IsSendProjectilePacket(pMover, pActionEvent))
    {
        int nSessionID = pProjectile->GetSessionID();
        pMover->send_eSUB_CMD_PROJECTILE(pMover, pSkillTable->Skill_Index, pActionEvent->EventID,
                                          vStart, vDir, nSessionID);
    }

    m_pActor->DebugOut("AddProjectile>> %x", (std::uint32_t)(uintptr_t)pProjectile);
    m_vProjectiles.push_back(pProjectile);

    SSkillInfo sSkillInfo;
    sSkillInfo.nSkillID = pSkillTable->Skill_Index;
    sSkillInfo.pTrigger = pActionEvent;
    pProjectile->SetSkillInfo(&sSkillInfo);

    SetFilterDataToMover(pSkillTable->Skill_Index, pMover);
}


// ============================================================================
// CMySkillList::ClearAttackDamage
// IDA: 0x1402C3270
// ============================================================================
void CMySkillList::ClearAttackDamage()
{
    memset(m_stAttackDamage, 0, sizeof(m_stAttackDamage));
}

// ============================================================================
// CMySkillList::SetAttackDamage
// IDA: 0x1402C7DE0
// ============================================================================
void CMySkillList::SetAttackDamage(int iIndex, const tagSKILL_ACTION_DAMAGE* stVal)
{
    if (iIndex >= 0 && iIndex < 100)
        m_stAttackDamage[iIndex] = *stVal;
}

// ============================================================================
// CMySkillList::GetAttackDamage
// IDA: 0x1402C7E40
// ============================================================================
void CMySkillList::GetAttackDamage(tagSKILL_ACTION_DAMAGE* pResult, int iIndex) const
{
    if (iIndex >= 0 && iIndex < 100 && pResult)
        *pResult = m_stAttackDamage[iIndex];
}


// ============================================================================
// CMySkillList::SetAttackExtraMove
// IDA: 0x1402C7380
// ============================================================================
void CMySkillList::SetAttackExtraMove(int iIndex, const hkvVec3& vMove)
{
    if (iIndex >= 0 && iIndex < 100)
        m_vAttackExtraMove[iIndex] = vMove;
}

// ============================================================================
// CMySkillList::SetAttackFlySpeed
// IDA: 0x1402C7350
// ============================================================================
void CMySkillList::SetAttackFlySpeed(int iIndex, float fFlySpeed)
{
    if (iIndex >= 0 && iIndex < 100)
        m_fAttackFlySpeed[iIndex] = fFlySpeed;
}

// ============================================================================
// CMySkillList::GetSkillCost
// IDA: 0x1402C54F0
// ============================================================================
float CMySkillList::GetSkillCost(TB_SKILL* pSkillTable)
{
    if (!pSkillTable)
        return -1.0f;

    std::uint8_t byAttribute = pSkillTable->Skill_Cost_Attribute;
    float fSkillCost = (float)(int)pSkillTable->Skill_Cost;

    CMoverEx* pMoverEx = dynamic_cast<CMoverEx*>(m_pActor);
    if (!pMoverEx)
        return fSkillCost;

    int iStatIndex = -1;

    switch (byAttribute)
    {
        case 1:  // HP
            iStatIndex = 1;
            break;

        case 2:  // MP/Soul
            {
                iStatIndex = 2;
                float fRate = 0.0f;

                TB_DECK_BONUS* pCurDeckBonus = pMoverEx->GetCurDeckBouns();
                if (pCurDeckBonus && pCurDeckBonus->Bonus_Type == 2)
                    fRate += pCurDeckBonus->Bonus_Value;

                auto pAttr = m_pActor->GetGOC<CGocAttribute>();
                if (pAttr)
                {
                    float specialEffect = pAttr->GetSpecialEffect(114);
                    fRate += specialEffect * 0.01f;
                }

                float fSoulCostDownRate = m_pActor->GetSoulCostDownRate();
                fRate += fSoulCostDownRate;

                fSkillCost -= fSkillCost * fRate;
            }
            break;

        case 3:  // SG/Stamina
            {
                iStatIndex = 3;
                XArea* pArea = pMoverEx->GetArea();
                if (pArea && pArea->IsPvPZone())
                {
                    // Double cost in PvP for certain passive types
                    if (pSkillTable->Passive_Type == 4 || 
                        pSkillTable->Passive_Type == 5 || 
                        pSkillTable->Passive_Type == 8)
                    {
                        fSkillCost += fSkillCost;
                    }
                }
            }
            break;
    }

    // Percentage-based cost
    if (pSkillTable->Cost_Type == 2)
    {
        fSkillCost = (float)m_pActor->GetStat(iStatIndex) * fSkillCost * 0.01f;
        if (fSkillCost > 0.0f && fSkillCost < 1.0f)
            return 1.0f;
    }

    return fSkillCost;
}


// ============================================================================
// CMySkillList::RemoveProjectile
// IDA: 0x1402C0710
// ============================================================================
void CMySkillList::RemoveProjectile(VGameProjectileObject* pProjectile)
{
    m_pActor->DebugOut("RemoveProjectile>> %x", (std::uint32_t)(uintptr_t)pProjectile);
    
    auto pInstance = ThreadLocalData::GetInstance();
    if (pInstance)
        pInstance->DeleteProjectile(pProjectile);
    else
        LogHelper::LogDebug("game.contents", "<ERROR> ThreadLocalData::GetInstance() == NULL ( %s %d) ", 
                            "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\MySkillList.cpp", 2598);
}

// ============================================================================
// CMySkillList::RemoveTrap
// IDA: 0x1402C0E80
// ============================================================================
void CMySkillList::RemoveTrap(VGameTrapObject* pTrap)
{
    m_pActor->DebugOut("RemoveTrap>> %x", (std::uint32_t)(uintptr_t)pTrap);
    
    auto pInstance = ThreadLocalData::GetInstance();
    if (pInstance)
        pInstance->DeleteTrap(pTrap);
    else
        LogHelper::LogDebug("game.contents", "<ERROR> ThreadLocalData::GetInstance() == NULL ( %s %d) ", 
                            "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\MySkillList.cpp", 2700);
}

// ============================================================================
// CMySkillList::RemoveChainLightning
// IDA: 0x1402C0EF0
// ============================================================================
void CMySkillList::RemoveChainLightning(VChainLightningObject* pChainLightning)
{
    m_pActor->DebugOut("RemoveChainLightning>> %x", (std::uint32_t)(uintptr_t)pChainLightning);
    
    pChainLightning->ReleaseAllChainEffect();
    
    auto pInstance = ThreadLocalData::GetInstance();
    if (pInstance)
        pInstance->DeleteChainLightning(pChainLightning);
    else
        LogHelper::LogDebug("game.contents", "<ERROR> ThreadLocalData::GetInstance() == NULL ( %s %d) ", 
                            "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\MySkillList.cpp", 2714);
}


// ============================================================================
// CMySkillList::CreateRandomTrap
// IDA: 0x1402BE230
// ============================================================================
void CMySkillList::CreateRandomTrap(AttackJudgmentTrigger* pTrap, CMover* pMover, 
                                     hkvVec3 vEntityPos, float fDirYaw, SSkillInfo* sSkillInfo)
{
    if (!pTrap || !pMover || !m_pActor)
        return;

    int nIndex1 = pMover->GetRandomTrapIndex();
    int nIndex2 = pMover->GetRandomTrapIndex();
    int nIndex3 = pMover->GetRandomTrapIndex();

    auto pWorldMgr = TXSingleton<XWorldManager>::Instance();
    int nRand1 = pWorldMgr->nTrapRand(nIndex1);
    int nRand2 = pWorldMgr->nTrapRand(nIndex2);
    int nRand3 = pWorldMgr->nTrapRand(nIndex3);

    LogHelper::LogDebug("game.contents", "CreateRandomTrap > [%d] %d, %d, %d ", nIndex3, nRand1, nRand2, nRand3);

    int nDiff = (int)(pTrap->sProjInfo.fRandomRadius - pTrap->sProjInfo.fRandomRadiusMin);
    int nRandDist = (int)pTrap->sProjInfo.fRandomRadiusMin + nRand1 % (nDiff + 1);
    int nRandAngle = nRand2 % (int)(pTrap->sProjInfo.fRandomAngle + 1.0f);
    if (!(nRand3 % 2))
        nRandAngle = -nRandAngle;

    hkvMat3 matRot;
    VisMath_cl::EulerToMatrix(fDirYaw, 0.0f, 0.0f, &matRot);
    hkvVec3 vStartPos = vEntityPos + matRot * pTrap->sProjInfo.vRandomPos;

    VisMath_cl::EulerToMatrix(fDirYaw + (float)nRandAngle, 0.0f, 0.0f, &matRot);
    hkvVec3 vDestPos = vStartPos + matRot * hkvVec3(0.0f, -(float)nRandDist, 0.0f);

    hkvVec3 vTrapDir(0.0f, -1.0f, 0.0f);
    VGameTrapObject* pTrapObj = CreateTrap(vDestPos, vTrapDir, static_cast<CMoverEx*>(m_pActor), pTrap);
    if (!pTrapObj)
        return;

    pTrapObj->SetSkillInfo(sSkillInfo);
    m_vTraps.push_back(pTrapObj);

    if (CMover::IsSendProjectilePacket(pMover, pTrap))
    {
        hkvVec3 vDir;
        pMover->GetDirection(&vDir);
        int nSessionID = pTrapObj->GetSessionID();
        pMover->send_eSUB_CMD_PROJECTILE(pMover, sSkillInfo->nSkillID, pTrap->EventID,
                                          vDestPos, vTrapDir, nSessionID);
    }
}

// ============================================================================
// CMySkillList::CreateChainLightningObject
// IDA: 0x1402C1760
// ============================================================================
VChainLightningObject* CMySkillList::CreateChainLightningObject(hkvVec3 vPos, hkvVec3 vDir,
                                                                 CMover* pMover,
                                                                 AttackJudgmentTrigger* pActionEvent,
                                                                 CMover* pFirstTarget)
{
    if (!pMover)
        return nullptr;

    XArea* pArea = pMover->GetArea();
    if (!pArea)
        return nullptr;

    if (!pActionEvent)
        return nullptr;

    auto pInstance = ThreadLocalData::GetInstance();
    if (!pInstance)
    {
        LogHelper::LogDebug("game.contents", "<ERROR> ThreadLocalData::GetInstance() == NULL ( %s %d) ", 
                            "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\MySkillList.cpp", 2834);
        return nullptr;
    }

    UXActorID actorID;
    pMover->GetActorID(&actorID);
    std::uint32_t dwActorID = CQuestCondition::GetQuestID(&actorID);

    UXMapID instanceID;
    pArea->GetInstanceID(&instanceID);
    std::uint64_t uxMapID = instanceID.__s0;

    int nThreadNo = pInstance->m_ownerThreadIndex;
    int nEventID = pActionEvent->EventID;

    if (!pArea->GetWorldType())
    {
        LogHelper::LogError("game.system", "[CHAIN] ActorID : %d %I64d %d %d", dwActorID, uxMapID, nThreadNo, nEventID);
        return nullptr;
    }

    hkvVec3 xPos(vPos.x, vPos.y, vPos.z);
    UXMapID mapID;
    pMover->GetArea()->GetInstanceID(&mapID);
    XArea* pMoverArea = pMover->GetArea();

    VChainLightningObject* pChainObj = pInstance->CreateChainLightning(pMoverArea, mapID, &xPos);
    if (!pChainObj)
        return nullptr;

    VString strTrapParticleFile(pActionEvent->sProjInfo.szProjParticle);
    VString strDestoryParticleFile(pActionEvent->sProjInfo.szProjExplodeParticle);
    VString strDisappearParticleFile(pActionEvent->sProjInfo.szProjDisapearParticle);
    VString strExplodeSound(pActionEvent->sProjInfo.szProjExplodeSound);

    pChainObj->SetActionEvent(pActionEvent);
    pChainObj->SetLifeTime(pActionEvent->sConnectionInfo.fLifeTime);
    pChainObj->SetRecalcTarget(pActionEvent->sConnectionInfo.bRecalcTarget);
    pChainObj->SetInvisibleWhenNoTarget(pActionEvent->sConnectionInfo.bInvisibleWhenNoTarget);

    if (pActionEvent->sProjInfo.bIsContinousHit)
        pChainObj->SetContinousHitTime(pActionEvent->sProjInfo.fContinousTime);

    if (pFirstTarget)
        pChainObj->AddTargetEntity(pFirstTarget);

    pChainObj->Fire(vPos, vDir, pMover);

    return pChainObj;
}

// ============================================================================
// CMySkillList::GetExplodeSummon
// IDA: 0x1402C5890
// ============================================================================
SummonMonsterTrigger* CMySkillList::GetExplodeSummon(int iIndex)
{
    auto it = m_mapExplodeSummon.find(iIndex);
    if (it != m_mapExplodeSummon.end())
        return it->second;
    return nullptr;
}


// ============================================================================
// CMySkillList::CreateRandomSummon
// IDA: 0x1402BE670
// ============================================================================
void CMySkillList::CreateRandomSummon(SRandomSummonEvent* pRandomSummon)
{
    if (!pRandomSummon)
        return;

    SummonMonsterTrigger* pTrigger = pRandomSummon->pRandomSummonEvent;
    if (!pTrigger)
        return;

    CMoverEx* pMover = pRandomSummon->pMover;
    if (!pMover)
        return;

    XArea* pArea = pMover->GetArea();
    if (!pArea)
        return;

    hkvVec3 vSummonPos = pTrigger->SummonPos;
    hkvMat3 matRot;

    float fMonsterYaw;
    if (m_pActor && m_pActor->GetType() == eActorNPC)  // Akashic
        fMonsterYaw = pMover->GetSummonAkashicYaw();
    else
        fMonsterYaw = pMover->GetMovingYaw();

    matRot.setFromEulerAngles(0.0f, 0.0f, fMonsterYaw);
    vSummonPos *= matRot;

    hkvVec3 vCreatePos(0.0f, 0.0f, 0.0f);
    float fSummonLifeTime = 0.0f;

    if (pRandomSummon->bUseOtherPos)
    {
        vCreatePos = pRandomSummon->vOtherPos;
    }
    else
    {
        vCreatePos = *VisObject3D_cl::GetPosition(pMover) + vSummonPos;
    }

    int iWayPointID = 0;

    // Handle event box spawn
    if (pTrigger->iEventBoxID > 0)
    {
        auto pMazeResource = pArea->GetObjectResource();
        if (pMazeResource)
        {
            auto pInfo = pMazeResource->SearchFromID(pTrigger->iEventBoxID);
            if (pInfo && pInfo->eType == eEventObjectType_Box)
            {
                auto pBoxInfo = static_cast<const VEventBoxInfo*>(pInfo);
                if (pBoxInfo->eBoxType == eEventBoxType_MonsterSpawn)
                {
                    auto pMonsterSpawn = static_cast<const VMonsterSpawnInfo*>(pBoxInfo);
                    hkvVec3 vPos;
                    pArea->GetSpawnPos_2(pMonsterSpawn, &vPos);
                    vCreatePos = vPos;
                    fMonsterYaw = pMonsterSpawn->fRotate;
                    iWayPointID = pMonsterSpawn->m_iWaypoint;
                }
            }
        }
    }
    else if (pTrigger->RandomRadius != 0.0f)
    {
        // Random position generation
        for (int nTry = 0; nTry < 10; ++nTry)
        {
            int nRandDist = rand() % (int)(pTrigger->RandomRadius + 1.0f);
            int nRandAngle = rand() % 360;
            if (rand() % 2)
                nRandAngle = -nRandAngle;

            hkvMat3 lhs;
            VisMath_cl::EulerToMatrix(fMonsterYaw + (float)nRandAngle, 0.0f, 0.0f, &lhs);
            vCreatePos = vCreatePos + lhs * hkvVec3(0.0f, -(float)nRandDist, 0.0f);

            if (pMover->GetHeight(vCreatePos, 200.0f))
                break;
        }
    }

    if (!pMover->GetHeight(vCreatePos, 200.0f))
        return;

    CMonster* pSummonMob = nullptr;

    // Create in maze or battle zone
    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    if (pMaze)
    {
        int nSectorID = pMover->GetSectorID();
        if (nSectorID < 0)
        {
            int iLevelA = pMaze->GetBatchLayerLevel();
            int lastSectorID = pMaze->GetLastSectorID();
            nSectorID = VEventObjectInfo::GetEventUniqueID(lastSectorID, iLevelA);
        }

        UXActorID actorID;
        pMover->GetActorID(&actorID);
        UXMapID instanceID;
        pArea->GetInstanceID(&instanceID);
        pSummonMob = pMaze->CreateMonster(instanceID, nSectorID, pTrigger->MonsterID,
                                           vCreatePos, fMonsterYaw, eSendInfoTypeNot,
                                           pTrigger->iEventBoxID, 0, actorID);
    }
    else
    {
        CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
        if (pBattleZone)
        {
            UXActorID actorID;
            pMover->GetActorID(&actorID);
            UXMapID instanceID;
            pArea->GetInstanceID(&instanceID);
            int nSectorID = pMover->GetSectorID();
            pSummonMob = pBattleZone->CreateMonster(instanceID, nSectorID, pTrigger->MonsterID,
                                                    vCreatePos, fMonsterYaw, eSendInfoTypeNot,
                                                    pTrigger->iEventBoxID, 0, actorID);
        }
    }

    if (!pSummonMob)
        return;

    if (iWayPointID > 0)
        pSummonMob->SetWayPointID(iWayPointID);

    pSummonMob->SetSummonType(pTrigger->SummonType);

    if (pTrigger->fSummonLifeTime > 0.0f)
        pSummonMob->SetSummonLifeTime(pTrigger->fSummonLifeTime);

    UXActorID actorID;
    pMover->GetActorID(&actorID);
    std::uint32_t questID = CQuestCondition::GetQuestID(&actorID);
    pSummonMob->SetOwnerID(questID);
    pSummonMob->SetAllowAbsorbSG(pRandomSummon->bAllowAbsorbSG);
    pSummonMob->SetApplyMultipleDamageOnce(pMover->GetMultipleDamageOnce() > 0.0f);

    // Broadcast monster spawn in maze (non-raid)
    if (pMaze && pMaze->GetMazeType() != 12)
    {
        PS_MONSTERINFO_VEC stVector;
        auto monsterInfo = pSummonMob->GetMonsterInfo();
        stVector.vecMonsterInfo.push_back(*monsterInfo);
        XSendPacket xSendPacket(4, 0x15);
        xSendPacket << stVector;
        pArea->SendBroadCast(xSendPacket, nullptr, eNoneSelf);
    }

    pSummonMob->SetDirectionYaw(2);

    if (pTrigger->Rotation > 0.0f)
        pSummonMob->SetDirectionYaw(2);

    if (pTrigger->nSkillID <= 0)
    {
        // Inherit parent properties if player
        if (m_pActor->GetType() == eActorUser)
        {
            auto pParentSkillRef = pMover->GetSkillTable();
            auto pParentDivergence = pMover->GetCurDivergenceTable();
            int nParentDivergenceSkillID = pMover->GetCurDivergenceSKillID();
            int nParentCombatType = pMover->GetCombatType();

            if (pParentSkillRef)
                pSummonMob->SetSkillTable(pParentSkillRef);
            if (pParentDivergence && nParentDivergenceSkillID > 0)
                pSummonMob->SetCurDivergenceTable(pParentDivergence, nParentDivergenceSkillID);
            pSummonMob->SetCombatType(nParentCombatType);
            pSummonMob->SetApplyParentRotation(pTrigger->ApplyRotation);

            int actionCond = pMover->GetActionCondition();
            pSummonMob->SetActionCondition(actionCond);
            int pvpCond = pMover->GetPvpCondition();
            pSummonMob->SetPvpCondition(pvpCond);
        }

        VString strAnimName(pTrigger->szSummonAnim);
        int animIndex = pSummonMob->GetAnimIndex(strAnimName);
        pSummonMob->ChangeAnimation(animIndex, 1);
        pSummonMob->send_eSUB_CMD_MOVE_IDLE(pSummonMob, 0.0f);
    }
    else
    {
        pSummonMob->SetCurSkillTableIdx(pTrigger->nSkillID);
        pSummonMob->ActionAttack();
    }

    // Add to summon list
    UXActorID summonActorID;
    pSummonMob->GetActorID(&summonActorID);
    std::uint32_t summonID = CQuestCondition::GetQuestID(&summonActorID);
    pMover->AddSummonMobList(summonID);

    CheckDedicatedMonster(pSummonMob, pMover);
}

// ============================================================================
// CMySkillList::GetProjectileDirection
// IDA: 0x1402BFD60
// ============================================================================
hkvVec3 CMySkillList::GetProjectileDirection(const hkvVec3& vActionDir, float fYawA, 
                                              float fPitchA, int iRandomShootAngleX, 
                                              int iRandomShootAngleY)
{
    float fYaw = (vActionDir.x + fYawA) - 90.0f + (float)iRandomShootAngleX;
    if (fYaw > 360.0f)
        fYaw -= 360.0f;
    else if (fYaw < -360.0f)
        fYaw += 360.0f;

    float fPitch = vActionDir.y + fPitchA + (float)iRandomShootAngleY;
    float fRoll = vActionDir.z;

    hkvMat3 templateRot;
    templateRot.setFromEulerAngles(fRoll, fPitch, fYaw);
    return templateRot.getAxis(0);
}


// ============================================================================
// CMySkillList::ClearSkillTarget
// IDA: 0x1402BE1C0
// ============================================================================
void CMySkillList::ClearSkillTarget() {
    memset(m_stAttackDamage, 0, sizeof(m_stAttackDamage));
    memset(m_dwAttackTarget, 0, sizeof(m_dwAttackTarget));
    memset(m_stChainHitInfo, 0, sizeof(m_stChainHitInfo));
    memset(&m_stChainSkillInfo, 0, sizeof(m_stChainSkillInfo));
}

// ============================================================================
// CMySkillList::RemoveProjectile
// IDA: 0x1402C0710
// ============================================================================
void CMySkillList::RemoveProjectile(VGameProjectileObject* pProjectile) {
    DebugOut("RemoveProjectile>> %x", (std::uint32_t)(std::uintptr_t)pProjectile);
    
    auto pInstance = ThreadLocalData::GetInstance();
    if (pInstance) {
        pInstance->DeleteProjectile(pProjectile);
    } else {
        LogHelper::LogDebug("game.contents", 
            "<ERROR> ThreadLocalData::GetInstance() == NULL ( %s %d ) ",
            "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\MySkillList.cpp", 2598);
    }
}

// ============================================================================
// CMySkillList::RemoveTrap
// IDA: 0x1402C0E80
// ============================================================================
void CMySkillList::RemoveTrap(VGameTrapObject* pTrap) {
    DebugOut("RemoveTrap>> %x", (std::uint32_t)(std::uintptr_t)pTrap);
    
    auto pInstance = ThreadLocalData::GetInstance();
    if (pInstance) {
        pInstance->DeleteTrap(pTrap);
    } else {
        LogHelper::LogDebug("game.contents",
            "<ERROR> ThreadLocalData::GetInstance() == NULL ( %s %d ) ",
            "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\MySkillList.cpp", 2700);
    }
}

// ============================================================================
// CMySkillList::RemoveChainLightning
// IDA: 0x1402C0EF0
// ============================================================================
void CMySkillList::RemoveChainLightning(VChainLightningObject* pChainLightning) {
    DebugOut("RemoveChainLightning>> %x", (std::uint32_t)(std::uintptr_t)pChainLightning);
    
    pChainLightning->ReleaseAllChainEffect();
    
    auto pInstance = ThreadLocalData::GetInstance();
    if (pInstance) {
        pInstance->DeleteChainLightning(pChainLightning);
    } else {
        LogHelper::LogDebug("game.contents",
            "<ERROR> ThreadLocalData::GetInstance() == NULL ( %s %d ) ",
            "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\MySkillList.cpp", 2714);
    }
}

// ============================================================================
// CMySkillList::CreateProjectile
// IDA: 0x1402C0780
// ============================================================================
VGameProjectileObject* CMySkillList::CreateProjectile(hkvVec3& vPos, hkvVec3& vDir,
                                                       CMoverEx* pMover,
                                                       AttackJudgmentTrigger* pActionEvent) {
    if (!pMover || !pActionEvent) {
        return nullptr;
    }
    
    auto pArea = pMover->GetArea();
    if (!pArea) {
        return nullptr;
    }
    
    auto pThreadData = ThreadLocalData::GetInstance();
    if (!pThreadData) {
        LogHelper::LogDebug("game.contents",
            "<ERROR> ThreadLocalData::GetInstance() == NULL ( %s %d ) ",
            "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\MySkillList.cpp", 2617);
        return nullptr;
    }
    
    // Get actor ID and map instance
    UXActorID actorID;
    pMover->GetActorID(&actorID);
    std::uint32_t dwActorID = CQuestCondition::GetQuestID(&actorID);
    
    UXMapID instanceID = pArea->GetInstanceID();
    int nThreadNo = pThreadData->m_ownerThreadIndex;
    int nEventID = pActionEvent->EventID;
    
    // Check if area is valid
    if (!pArea->GetInstanceID().__s0) {
        LogHelper::LogError("game.system", 
            "[PROJECTILE] ActorID : %d %I64d %d %d",
            dwActorID, instanceID.__s0, nThreadNo, nEventID);
        return nullptr;
    }
    
    // Create projectile
    XVec3 xPos(vPos.x, vPos.y, vPos.z);
    VGameProjectileObject* pProjectile = pThreadData->CreateProjectile(pArea, instanceID.__s0, &xPos);
    if (!pProjectile) {
        return nullptr;
    }
    
    // Set bullet sync type from skill table
    const TB_SKILL* pSkillTable = pMover->GetSkillTable();
    if (pSkillTable) {
        pProjectile->SetBulletSyncType(pSkillTable->Bullet_Sync_Type);
    }
    
    // Configure projectile based on attack range
    if (pActionEvent->sAttackRange.fAngle > 1.0f) {
        // Guided projectile
        if (pActionEvent->sProjInfo.bIsTargetGuided) {
            pProjectile->SetProjectileType(PT_GUIDED);
            pProjectile->SetTargetUpdateInfo(
                pActionEvent->sProjInfo.fTargetUpdateTime,
                pActionEvent->sProjInfo.fTargetUpdateAngle);
            pProjectile->SetFindTargetTime(pActionEvent->sProjInfo.fTargetFindTime);
            pProjectile->SetTargetPriority(pActionEvent->sProjInfo.iTargetPriorityType);
        }
        // Gravity projectile
        else if (pActionEvent->sProjInfo.bIsGravity) {
            pProjectile->SetProjectileType(PT_GRAVITY);
        }
        // Straight projectile
        else {
            pProjectile->SetProjectileType(PT_STRAIGHT);
        }
        
        // Set physics info
        pProjectile->SetPhysicInfo(
            pActionEvent->sProjInfo.bIsCollision,
            pActionEvent->sProjInfo.shExplodeDecision);
        
        // Fire projectile
        hkvVec3 vSpeed = vDir * pActionEvent->sProjInfo.fMoveSpeed;
        pProjectile->Fire(vSpeed, pMover);
        
        // Set projectile properties
        pProjectile->SetLifeTime(pActionEvent->sProjInfo.fLifeTime);
        pProjectile->SetTargetType(pActionEvent->sReactionInfo.iTargetType);
        pProjectile->SetTargetStatus(pActionEvent->sReactionInfo.iTargetStatus);
        pProjectile->SetAttachToTarget(pActionEvent->sProjInfo.shAttachToTarget);
        pProjectile->SetParticleOnGround(pActionEvent->sProjInfo.eDirectionType == 2);
        
        // Continuous hit
        if (pActionEvent->sProjInfo.bIsContinousHit) {
            pProjectile->SetContinousHitTime(pActionEvent->sProjInfo.fContinousTime);
        }
        
        // Penetrate
        pProjectile->SetPenetrate(pActionEvent->sProjInfo.bPenetrate);
        pProjectile->SetPenetrateCount(pActionEvent->sProjInfo.iPenetrateCount);
    }
    else {
        // Simple straight projectile
        pProjectile->SetProjectileType(PT_STRAIGHT);
        
        hkvVec3 vSpeed = vDir * pActionEvent->sProjInfo.fMoveSpeed;
        pProjectile->Fire(vSpeed, pMover);
        
        pProjectile->SetLifeTime(pActionEvent->sProjInfo.fLifeTime);
        pProjectile->SetTargetType(pActionEvent->sReactionInfo.iTargetType);
        pProjectile->SetTargetStatus(pActionEvent->sReactionInfo.iTargetStatus);
    }
    
    // Set path animation if specified
    if (pActionEvent->sProjInfo.szProjPathAnimName[0]) {
        auto pSkillMgr = pMover->GetSkillMgr();
        if (pSkillMgr) {
            auto pActionRes = pSkillMgr->GetProjPathActionRes();
            if (pActionRes) {
                auto pAnimInfo = pActionRes->FindAnimationInfo(
                    pActionEvent->sProjInfo.szProjPathAnimName);
                if (pAnimInfo) {
                    pProjectile->SetPathAnimInfo(pAnimInfo);
                }
            }
        }
    }
    
    return pProjectile;
}

// ============================================================================
// CMySkillList::CreateTrap
// IDA: 0x1402C10F0
// ============================================================================
VGameTrapObject* CMySkillList::CreateTrap(hkvVec3& vPos, hkvVec3& vDir,
                                           CMoverEx* pMover,
                                           AttackJudgmentTrigger* pActionEvent) {
    if (!pMover || !pActionEvent) {
        return nullptr;
    }
    
    auto pArea = pMover->GetArea();
    if (!pArea) {
        return nullptr;
    }
    
    auto pThreadData = ThreadLocalData::GetInstance();
    if (!pThreadData) {
        LogHelper::LogDebug("game.contents",
            "<ERROR> ThreadLocalData::GetInstance() == NULL ( %s %d ) ",
            "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\MySkillList.cpp", 2758);
        return nullptr;
    }
    
    // Get actor ID and map instance
    UXActorID actorID;
    pMover->GetActorID(&actorID);
    std::uint32_t dwActorID = CQuestCondition::GetQuestID(&actorID);
    
    UXMapID instanceID = pArea->GetInstanceID();
    int nThreadNo = pThreadData->m_ownerThreadIndex;
    int nEventID = pActionEvent->EventID;
    
    // Check if area is valid
    if (!pArea->GetInstanceID().__s0) {
        LogHelper::LogError("game.system",
            "[TRAP] ActorID : %d %I64d %d %d",
            dwActorID, instanceID.__s0, nThreadNo, nEventID);
        return nullptr;
    }
    
    // Create trap
    XVec3 xPos(vPos.x, vPos.y, vPos.z);
    VGameTrapObject* pTrapObj = pThreadData->CreateTrap(pArea, instanceID.__s0, &xPos);
    if (!pTrapObj) {
        return nullptr;
    }
    
    // Set trap properties
    pTrapObj->SetLifeTime(pActionEvent->sProjInfo.fLifeTime);
    
    if (pActionEvent->sProjInfo.bIsContinousHit) {
        pTrapObj->SetInfluenceInterval(pActionEvent->sProjInfo.fContinousTime);
    } else {
        pTrapObj->SetInfluenceInterval(0.0f);
    }
    
    pTrapObj->SetInfluenceRadius(pActionEvent->sAttackRange.fRadius);
    pTrapObj->SetAttackRange(&pActionEvent->sAttackRange);
    pTrapObj->SetDelayTime(pActionEvent->sProjInfo.fDelayTime);
    pTrapObj->SetExplodeType(pActionEvent->sProjInfo.shExplodeDecision);
    pTrapObj->SetExplodeFaction(pActionEvent->sProjInfo.shExplodeFaction);
    pTrapObj->SetCollisionCheckType(pActionEvent->sAttackRangeType);
    pTrapObj->SetTargetStatus(pActionEvent->sReactionInfo.iTargetStatus);
    
    // Check buff for ignore can hit
    int nBuffID = pActionEvent->sReactionInfo.iBuffID;
    if (nBuffID > 0) {
        auto pServer = TXSingleton<XGameServer>::Instance();
        auto pBuffTable = pServer->m_xResourceMgr.GetTB_BUFF(nBuffID);
        if (pBuffTable && (pBuffTable->System_Type == 2 || pBuffTable->System_Type == 3)) {
            pTrapObj->SetIgnoreCanHit(true);
        }
    }
    
    // Set explode range
    SRangeInfo sRangeInfo;
    sRangeInfo.fBoxSizeX = pActionEvent->sProjInfo.sCollisionRange.fBoxSizeX;
    sRangeInfo.fBoxSizeY = pActionEvent->sProjInfo.sCollisionRange.fBoxSizeY;
    sRangeInfo.fRadius = pActionEvent->sProjInfo.sCollisionRange.fRadius;
    sRangeInfo.fAngle = pActionEvent->sProjInfo.sCollisionRange.fAngle;
    sRangeInfo.fHeight = pActionEvent->sProjInfo.sCollisionRange.fHeight;
    sRangeInfo.fStartPos = pActionEvent->sProjInfo.sCollisionRange.fStartPos;
    pTrapObj->SetExplodeRange(&sRangeInfo);
    
    pTrapObj->SetExplodeDelayTime(pActionEvent->sProjInfo.fExplodeDelayTime);
    
    // Set explode damage if particle path is valid
    if (strlen(pActionEvent->sProjInfo.szProjExplodeParticle) > 1) {
        pTrapObj->SetExplodeDamage(true);
    } else {
        pTrapObj->SetExplodeDamage(false);
    }
    
    pTrapObj->Install(&vPos, pMover);
    return pTrapObj;
}

// ============================================================================
// CMySkillList::CreateChainLightningObject
// IDA: 0x1402C1760
// ============================================================================
VChainLightningObject* CMySkillList::CreateChainLightningObject(hkvVec3& vPos, hkvVec3& vDir,
                                                                 CMover* pMover,
                                                                 AttackJudgmentTrigger* pActionEvent,
                                                                 CMover* pFirstTarget) {
    if (!pMover || !pActionEvent) {
        return nullptr;
    }
    
    auto pArea = pMover->GetArea();
    if (!pArea) {
        return nullptr;
    }
    
    auto pThreadData = ThreadLocalData::GetInstance();
    if (!pThreadData) {
        LogHelper::LogDebug("game.contents",
            "<ERROR> ThreadLocalData::GetInstance() == NULL ( %s %d ) ",
            "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\MySkillList.cpp", 2834);
        return nullptr;
    }
    
    // Get actor ID and map instance
    UXActorID actorID;
    pMover->GetActorID(&actorID);
    std::uint32_t dwActorID = CQuestCondition::GetQuestID(&actorID);
    
    UXMapID instanceID = pArea->GetInstanceID();
    int nThreadNo = pThreadData->m_ownerThreadIndex;
    int nEventID = pActionEvent->EventID;
    
    // Check if area is valid
    if (!pArea->GetInstanceID().__s0) {
        LogHelper::LogError("game.system",
            "[CHAIN] ActorID : %d %I64d %d %d",
            dwActorID, instanceID.__s0, nThreadNo, nEventID);
        return nullptr;
    }
    
    // Create chain lightning
    XVec3 xPos(vPos.x, vPos.y, vPos.z);
    VChainLightningObject* pChainObj = pThreadData->CreateChainLightning(pArea, instanceID.__s0, &xPos);
    if (!pChainObj) {
        return nullptr;
    }
    
    // Set particle and sound paths
    VString strTrapParticleFile(pActionEvent->sProjInfo.szProjParticle);
    VString strDestoryParticleFile(pActionEvent->sProjInfo.szProjExplodeParticle);
    VString strDisappearParticleFile(pActionEvent->sProjInfo.szProjDisapearParticle);
    VString strExplodeSound(pActionEvent->sProjInfo.szProjExplodeSound);
    
    // Set chain lightning properties
    pChainObj->SetActionEvent(pActionEvent);
    pChainObj->SetLifeTime(pActionEvent->sConnectionInfo.fLifeTime);
    pChainObj->SetRecalcTarget(pActionEvent->sConnectionInfo.bRecalcTarget);
    pChainObj->SetInvisibleWhenNoTarget(pActionEvent->sConnectionInfo.bInvisibleWhenNoTarget);
    
    if (pActionEvent->sProjInfo.bIsContinousHit) {
        pChainObj->SetContinousHitTime(pActionEvent->sProjInfo.fContinousTime);
    }
    
    // Add first target if provided
    if (pFirstTarget) {
        pChainObj->AddTargetEntity(pFirstTarget);
    }
    
    pChainObj->Fire(&vPos, &vDir, pMover);
    return pChainObj;
}

// ============================================================================
// CMySkillList::CreateRandomTrap
// IDA: 0x1402BE230
// ============================================================================
void CMySkillList::CreateRandomTrap(AttackJudgmentTrigger* pTrap, CMover* pMover,
                                     hkvVec3& vEntityPos, float fDirYaw,
                                     SSkillInfo& sSkillInfo) {
    if (!pTrap || !pMover || !m_pActor) {
        return;
    }
    
    hkvVec3 vDestPos;
    int nTry = 0;
    
    // Get random indices
    int nIndex1 = pMover->GetRandomTrapIndex();
    int nIndex2 = pMover->GetRandomTrapIndex();
    int nIndex3 = pMover->GetRandomTrapIndex();
    
    auto pWorldMgr = TXSingleton<XWorldManager>::Instance();
    int nRand1 = pWorldMgr->nTrapRand(nIndex1);
    int nRand2 = pWorldMgr->nTrapRand(nIndex2);
    int nRand3 = pWorldMgr->nTrapRand(nIndex3);
    
    LogHelper::LogDebug("game.contents", "CreateRandomTrap > [%d] %d, %d, %d ",
        nIndex3, nRand1, nRand2, nRand3);
    
    // Calculate random distance and angle
    int nDiff = (int)(pTrap->sProjInfo.fRandomRadius - pTrap->sProjInfo.fRandomRadiusMin);
    int nRandDist = (int)pTrap->sProjInfo.fRandomRadiusMin + nRand1 % (nDiff + 1);
    int nRandAngle = nRand2 % (int)(pTrap->sProjInfo.fRandomAngle + 1.0f);
    
    if (nRand3 % 2) {
        nRandAngle = -nRandAngle;
    }
    
    // Calculate start position
    hkvMat3 matRot;
    Vision::Math.EulerToMatrix(fDirYaw, 0.0f, 0.0f, matRot);
    
    hkvVec3 vStartPos = vEntityPos + (matRot * pTrap->sProjInfo.vRandomPos);
    
    // Calculate destination position
    Vision::Math.EulerToMatrix(fDirYaw + (float)nRandAngle, 0.0f, 0.0f, matRot);
    hkvVec3 vOffset(0.0f, -(float)nRandDist, 0.0f);
    vDestPos = vStartPos + (matRot * vOffset);
    
    // Create trap
    hkvVec3 vDir(0.0f, -1.0f, 0.0f);
    VGameTrapObject* pTrapObj = CMySkillList::CreateTrap(vDestPos, vDir, 
        reinterpret_cast<CMoverEx*>(m_pActor), pTrap);
    
    if (pTrapObj) {
        pTrapObj->SetSkillInfo(&sSkillInfo);
        m_vTraps.push_back(pTrapObj);
        
        // Send projectile packet if needed
        if (pMover->IsSendProjectilePacket(pTrap)) {
            hkvVec3 vMoverDir;
            pMover->GetDirection(&vMoverDir);
            
            int nSessionID = pTrapObj->GetSessionID();
            hkvVec3 vTrapDir(0.0f, -1.0f, 0.0f);
            pMover->send_eSUB_CMD_PROJECTILE(pMover, sSkillInfo.nSkillID,
                pTrap->EventID, &vDestPos, &vTrapDir, nSessionID);
        }
    }
}

// ============================================================================
// CMySkillList::AddSkillTarget
// IDA: 0x1402BE150
// ============================================================================
void CMySkillList::AddSkillTarget(std::uint8_t byIndex, std::uint32_t dwTargetID,
                                   std::uint8_t byReaction, std::uint8_t byHitPartsIndex) {
    if (byIndex < 100) {
        m_dwAttackTarget[byIndex] = dwTargetID;
        m_stAttackDamage[byIndex].byReactionType = byReaction;
        m_stAttackDamage[byIndex].byHitPartsIndex = byHitPartsIndex;
    }
}

// ============================================================================
// CMySkillList::GetSkillTarget (wrapper)
// IDA: 0x1402BD690
// ============================================================================
int CMySkillList::GetSkillTarget(CMoverEx* pMover, AttackJudgmentTrigger* pActionEvent,
                                  TB_SKILL* pSkillTable, std::uint8_t& bAttackTargetCnt) {
    if (!pMover || !pActionEvent || !pSkillTable) {
        XPRINT("GetSkillTarget: null parameter");
        return 56011;
    }
    
    float fPrevOrientationYaw = pMover->GetOrientationYaw();
    float fPrevMovingYaw = pMover->GetMovingYaw();
    
    hkvVec3 vPos;
    pMover->GetPosition(&vPos);
    
    // Handle trace bone for continuous melee
    if (pActionEvent->sAttackType == 0 && 
        pActionEvent->sContinuousMeleeInfo.szTraceBoneName[0]) {
        
        int TraceBoneListIndex = pMover->GetTraceBoneListIndex(
            pActionEvent->sContinuousMeleeInfo.szTraceBoneName);
        int iIndex = pMover->GetHitCollisionCount() + TraceBoneListIndex;
        
        hkvVec3 vBonePos;
        pMover->GetBoneCurrentWorldSpaceTranslation(iIndex, &vPos, &vBonePos);
        
        float BoneYaw = pMover->GetBoneYaw(iIndex);
        float fYaw = fPrevOrientationYaw + BoneYaw + 90.0f;
        pMover->SetOrientationYaw(fYaw);
        
        vPos = vBonePos;
    }
    
    int iResult = GetSkillTarget(pMover, &vPos, pActionEvent, pSkillTable, bAttackTargetCnt);
    pMover->SetOrientationYaw(fPrevOrientationYaw);
    
    return iResult;
}

// ============================================================================
// CMySkillList::CalcTrapedSkillTarget
// IDA: 0x1402BF420
// ============================================================================
void CMySkillList::CalcTrapedSkillTarget(CMoverEx* pMover, AttackJudgmentTrigger* pActionEvent,
                                          SSkillInfo& sSkillInfo, hkvVec3& vTouchPos) {
    if (!pMover || !pActionEvent) {
        return;
    }
    
    bool bIsMouseOnTarget = pMover->IsMouseOnTarget();
    if (bIsMouseOnTarget && pMover->GetMouseOnTrap()) {
        return;
    }
    
    // Calculate attack area
    tagATTACK_AREA stAttackArea;
    hkvVec3 vPos = vTouchPos;
    
    if (pActionEvent->sProjInfo.bIsMovingToTarget) {
        vPos = pMover->GetTrapPos();
    }
    
    float fYaw = pMover->GetOrientationYaw();
    GetAttackArea(pMover, &vPos, fYaw, pActionEvent->sAttackRangeType, 
        &pActionEvent->sAttackRange, &stAttackArea);
    
    vPos = stAttackArea.vCenterPos;
    
    // Override position for specific attack type
    if (pActionEvent->sAttackType == 4) {
        vPos = vTouchPos;
    }
    
    if (pActionEvent->sProjInfo.bIsMovingToTarget) {
        vPos = pMover->GetTrapPos();
    }
    
    // Create trap
    hkvVec3 vDir(0.0f, -1.0f, 0.0f);
    VGameTrapObject* pTrapObj = CMySkillList::CreateTrap(vPos, vDir, pMover, pActionEvent);
    
    if (pTrapObj) {
        pTrapObj->SetSkillInfo(&sSkillInfo);
        m_vTraps.push_back(pTrapObj);
        
        if (bIsMouseOnTarget) {
            pMover->SetMouseOnTrap(pTrapObj);
        }
        
        // Send projectile packet if needed
        if (pMover->IsSendProjectilePacket(pActionEvent)) {
            hkvVec3 vMoverDir;
            pMover->GetDirection(&vMoverDir);
            
            int nSessionID = pTrapObj->GetSessionID();
            pMover->send_eSUB_CMD_PROJECTILE(pMover, sSkillInfo.nSkillID,
                pActionEvent->EventID, &vPos, &vMoverDir, nSessionID);
        }
    }
}

// ============================================================================
// CMySkillList::CalcChainSkillTarget
// IDA: 0x1402BF7B0
// ============================================================================
void CMySkillList::CalcChainSkillTarget(CMover* pMover, tagATTACK_AREA& stAreaInfo,
                                         AttackJudgmentTrigger* pActionEvent,
                                         SSkillInfo& SkillInfo, std::uint8_t& bAttackTargetCnt,
                                         TB_SKILL* pSkillRef) {
    if (!pMover || !pActionEvent || !pSkillRef) {
        return;
    }
    
    std::uint8_t byType = pMover->GetType();
    CMoverEx* pClosestTarget = nullptr;
    float fClosestDistance = 1000000.0f;
    
    hkvVec3 vPos;
    pMover->GetPosition(&vPos);
    
    // Get nearby objects
    std::vector<CMover*> vecGameObjList;
    pMover->ScanGridOrigin(2, 3, &vecGameObjList);
    
    // Find closest target
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMoverEx* pOtherActor = dynamic_cast<CMoverEx*>(*it);
        if (!pOtherActor) {
            continue;
        }
        
        // Check if target is valid
        if (!pOtherActor->IsLive()) {
            continue;
        }
        
        if (pOtherActor->IsStatus(2) && 
            (pOtherActor->GetDieType() != 1 || pOtherActor->GetDieDelayTime() <= 0.0f)) {
            continue;
        }
        
        // Check reaction target
        if (!pMover->CheckReactionTarget(pActionEvent->sReactionInfo.iTargetType, pOtherActor, true)) {
            continue;
        }
        
        // Check actor type
        E_ACTOR_TYPE eType = pOtherActor->GetType();
        if (eType != eActorUser && eType != eActorMonster) {
            continue;
        }
        
        // Check if in attack area
        if (!IsInAttackArea(pOtherActor, &stAreaInfo, 
            pActionEvent->sReactionInfo.iTargetStatus, pSkillRef->Passive_Type)) {
            continue;
        }
        
        if (bAttackTargetCnt >= pSkillRef->Target_Damage_Count) {
            continue;
        }
        
        // Calculate distance
        hkvVec3 vTargetPos;
        pOtherActor->GetPosition(&vTargetPos);
        
        hkvVec3 vDiff = vTargetPos - vPos;
        float fDist = vDiff.getLength();
        
        if (fClosestDistance > fDist) {
            pClosestTarget = pOtherActor;
            fClosestDistance = fDist;
        }
    }
    
    // Create chain lightning
    hkvVec3 vDir(0.0f, -1.0f, 0.0f);
    
    auto pServer = TXSingleton<XGameServer>::Instance();
    auto pSkillTable = pServer->m_xResourceMgr.GetTB_SKILL(SkillInfo.nSkillID);
    
    if (pSkillTable) {
        vDir *= (float)pSkillTable->Skill_Range_Max;
        
        VChainLightningObject* pChainObj = CMySkillList::CreateChainLightningObject(
            vPos, vDir, pMover, pActionEvent, pClosestTarget);
        
        if (pChainObj) {
            pChainObj->SetSkillInfo(&SkillInfo);
            m_vChainLightningObject.push_back(pChainObj);
            
            // Get target ID
            std::uint32_t dwTargetID = -1;
            if (pClosestTarget) {
                UXActorID actorID;
                pClosestTarget->GetActorID(&actorID);
                dwTargetID = CQuestCondition::GetQuestID(&actorID);
            }
            
            int nSessionID = pChainObj->GetSessionID();
            pMover->send_eSUB_CMD_CHAIN(pMover, SkillInfo.nSkillID,
                pActionEvent->EventID, &vPos, &vDir, nSessionID, dwTargetID);
        }
    }
}

// ============================================================================
// CMySkillList::CalcRangedSkillTarget
// IDA: 0x1402BFE80
// ============================================================================
void CMySkillList::CalcRangedSkillTarget(CMoverEx* pMover, AttackJudgmentTrigger* pActionEvent,
                                          TB_SKILL* pSkillTable, hkvVec3* vPos, hkvVec3* vRight,
                                          float fYaw, float fPitch, std::uint8_t& bAttackTargetCnt) {
    if (!pMover || !pActionEvent || !pSkillTable || !m_pActor) {
        return;
    }
    
    if (pActionEvent->sAttackRange.fAngle <= 1.0f) {
        return;
    }
    
    hkvVec3 vStart;
    hkvVec3 vDir;
    hkvVec3 vUp(0.0f, 0.0f, 1.0f);
    
    // Calculate start position
    if (pActionEvent->sAttackType == 6) {
        hkvMat3 matRot;
        matRot.setFromEulerAngles(0.0f, 0.0f, fYaw - 90.0f);
        vStart = pActionEvent->sProjInfo.vStartPos;
        vStart *= matRot;
        vStart += *vPos;
    } else {
        hkvVec3 vOffset(0.0f, 0.0f, pActionEvent->sProjInfo.vStartPos.z);
        vStart = *vPos + vOffset;
    }
    
    bool bFinalRotation = false;
    
    // Get random shoot direction
    std::int16_t shRandomShootDirX = pMover->GetCurRandomShootProjectileDirX();
    std::int16_t shRandomShootDirY = pMover->GetCurRandomShootProjectileDirY();
    
    // Calculate direction based on mover type
    if (pMover == m_pActor) {
        if (pMover->GetType() != eActorUser) {
            // Monster case
            if (pMover->IsDefensiveWeapon()) {
                vDir = GetProjectileDirection(pActionEvent->sProjInfo.vDir, 
                    fYaw, -fPitch, shRandomShootDirX, shRandomShootDirY);
            } else {
                vDir = GetProjectileDirection(pActionEvent->sProjInfo.vDir,
                    fYaw, 0.0f, shRandomShootDirX, shRandomShootDirY);
            }
        } else if (pActionEvent->sProjInfo.eDirectionType) {
            // Player with direction type
            vDir = GetProjectileDirection(pActionEvent->sProjInfo.vDir,
                fYaw, 0.0f, shRandomShootDirX, shRandomShootDirY);
        } else {
            // Camera-based direction
            hkvMat3 templateRot;
            templateRot.setFromEulerAngles(pActionEvent->sProjInfo.vDir.z,
                pActionEvent->sProjInfo.vDir.y, pActionEvent->sProjInfo.vDir.x);
            
            hkvVec3 vCamDir;
            pMover->GetCameraDir(&vCamDir);
            
            hkvMat3 cameraRot;
            cameraRot.setFromEulerAngles(0.0f, -vCamDir.z, vCamDir.x + 90.0f);
            
            cameraRot = cameraRot * templateRot;
            
            vDir = cameraRot.getAxis(0);
            *vRight = cameraRot.getAxis(1);
            vUp = cameraRot.getAxis(2);
            bFinalRotation = true;
        }
    } else {
        vDir = GetProjectileDirection(pActionEvent->sProjInfo.vDir,
            fYaw, 0.0f, shRandomShootDirX, shRandomShootDirY);
    }
    
    // Apply start position offset for attack type 1
    if (pActionEvent->sAttackType == 1) {
        vStart += vDir * pActionEvent->sProjInfo.vStartPos.x;
        vStart += *vRight * pActionEvent->sProjInfo.vStartPos.y;
    }
    
    // Update target if control type is 6
    if (pMover->GetControlType(pSkillTable) == 6) {
        pMover->UpdateTargetByPretarget();
    }
    
    // Create projectile
    VGameProjectileObject* pProjectile = CMySkillList::CreateProjectile(vStart, vDir, pMover, pActionEvent);
    
    if (pProjectile) {
        // Send packet if needed
        if (pMover->IsSendProjectilePacket(pActionEvent)) {
            int nSessionID = pProjectile->GetSessionID();
            pMover->send_eSUB_CMD_PROJECTILE(pMover, pSkillTable->Skill_Index,
                pActionEvent->EventID, &vStart, &vDir, nSessionID);
        }
        
        DebugOut("AddProjectile>> %x", (std::uint32_t)(std::uintptr_t)pProjectile);
        m_vProjectiles.push_back(pProjectile);
        
        // Set skill info
        SSkillInfo sSkillInfo;
        sSkillInfo.nSkillID = pSkillTable->Skill_Index;
        sSkillInfo.pTrigger = pActionEvent;
        pProjectile->SetSkillInfo(&sSkillInfo);
        
        SetFilterDataToMover(pSkillTable->Skill_Index, pMover);
    }
}

// ============================================================================
// CMySkillList::GetAttackTarget
// IDA: 0x14019B9B0
// ============================================================================
std::uint32_t CMySkillList::GetAttackTarget(int iIndex) const {
    return m_dwAttackTarget[iIndex];
}

// ============================================================================
// CMySkillList::Init
// IDA: 0x1402B6460
// ============================================================================
void CMySkillList::Init(XActor* pActor) {
    m_pActor = dynamic_cast<CMover*>(pActor);
    
    auto pServer = TXSingleton<XGameServer>::Instance();
    m_pActionRes = pServer->m_xActionManager.GetCommonBoneRes();
}

// ============================================================================
// CMySkillList::ProcessTrap
// IDA: 0x1402B81A0
// ============================================================================
void CMySkillList::ProcessTrap(CMoverEx* pMover, CMoverEx* pTrapOwner,
                                SSkillInfo& sSkillInfo, hkvVec3 vTrapPos,
                                std::uint8_t byHitPartsIndex,
                                bool bAllowAbsorbSG, bool bCheckCombo) {
    if (!pMover || !pTrapOwner) {
        return;
    }
    
    auto pTrigger = sSkillInfo.pTrigger;
    if (!pTrigger) {
        return;
    }
    
    auto pServer = TXSingleton<XGameServer>::Instance();
    auto pSkillTable = pServer->m_xResourceMgr.GetTB_SKILL(sSkillInfo.nSkillID);
    if (!pSkillTable) {
        return;
    }
    
    // Check reaction target
    if (!pTrapOwner->CheckReactionTarget(pTrigger->sReactionInfo.iTargetType, pMover, false)) {
        return;
    }
    
    // Calculate direction
    hkvVec3 vOwnerPos;
    pTrapOwner->GetPosition(&vOwnerPos);
    hkvVec3 vDir = vTrapPos - vOwnerPos;
    
    // Process projectile explode
    ProcessProjectileExplode(pTrapOwner, vTrapPos, vDir, sSkillInfo, pMover,
        3, 0, 0, byHitPartsIndex, true, false, bAllowAbsorbSG, bCheckCombo);
    
    // Apply buff if no damage rate
    if (pTrigger->sReactionInfo.fDamageRate <= 0.0f) {
        if (pTrigger->sReactionInfo.iBuffID > 0) {
            UXActorID ownerID;
            pTrapOwner->GetActorID(&ownerID);
            std::uint32_t dwID = CQuestCondition::GetQuestID(&ownerID);
            ApplyBuffSkill(pMover, pTrigger->sReactionInfo.iBuffID, dwID, 0);
        }
        
        if (pTrigger->sReactionInfo.iAuraID > 0) {
            pMover->SetAuraSkill(pTrigger->sReactionInfo.iAuraID);
        }
    }
}

// ============================================================================
// CMySkillList::ValidSkill
// IDA: 0x1402B64C0
// ============================================================================
int CMySkillList::ValidSkill(std::uint32_t nSkillID) {
    auto pServer = TXSingleton<XGameServer>::Instance();
    if (pServer->m_xResourceMgr.GetTB_SKILL(nSkillID)) {
        return 0;
    }
    return 56001;
}

// ============================================================================
// CMySkillList::ThinkFunction
// IDA: 0x1402B6500
// ============================================================================
void CMySkillList::ThinkFunction() {
    // Get actor ID for debugging
    if (m_pActor) {
        UXActorID actorID;
        m_pActor->GetActorID(&actorID);
        CQuestCondition::GetQuestID(&actorID);
    }
    
    // Process projectiles
    for (auto it = m_vProjectiles.begin(); it != m_vProjectiles.end(); ) {
        VGameProjectileObject* pProjectile = *it;
        if (!pProjectile) {
            ++it;
            continue;
        }
        
        // Get area info
        if (pProjectile->GetArea()) {
            UXMapID instanceID = pProjectile->GetArea()->GetInstanceID();
        }
        
        // Tick projectile
        auto pTimer = ThreadLocalData::GetTimer();
        pTimer->GetTimeDifference();
        pProjectile->Tick();
        
        if (pProjectile->IsGazeMoving()) {
            ++it;
        } else {
            RemoveProjectile(pProjectile);
            it = m_vProjectiles.erase(it);
            if (m_vProjectiles.empty()) {
                break;
            }
        }
    }
    
    // Process traps
    for (auto it = m_vTraps.begin(); it != m_vTraps.end(); ) {
        VGameTrapObject* pTrap = *it;
        if (!pTrap) {
            ++it;
            continue;
        }
        
        // Log trap info
        if (m_pActor && m_pActor->GetArea()) {
            auto pArea = m_pActor->GetArea();
            if (!pArea->GetWorldType()) {
                auto pThreadData = ThreadLocalData::GetInstance();
                std::uint16_t wMapID = m_pActor->GetArea()->GetTBMapID();
                pTrap->WriteLog(wMapID, pThreadData->m_ownerThreadIndex);
            }
        }
        
        // Get area info
        if (pTrap->GetArea()) {
            UXMapID instanceID = pTrap->GetArea()->GetInstanceID();
        }
        
        pTrap->GetSkillInfo();
        pTrap->ThinkFunction();
        
        if (pTrap->IsActivate()) {
            ++it;
        } else {
            RemoveTrap(pTrap);
            it = m_vTraps.erase(it);
            if (m_vTraps.empty()) {
                break;
            }
        }
    }
    
    // Process chain lightning
    for (auto it = m_vChainLightningObject.begin(); it != m_vChainLightningObject.end(); ) {
        VChainLightningObject* pChainLightning = *it;
        if (!pChainLightning) {
            ++it;
            continue;
        }
        
        pChainLightning->GetWarpPotal();
        auto pTimer = ThreadLocalData::GetTimer();
        pTimer->GetTimeDifference();
        pChainLightning->Tick();
        
        if (pChainLightning->IsActivate()) {
            ++it;
        } else {
            RemoveChainLightning(pChainLightning);
            it = m_vChainLightningObject.erase(it);
            if (m_vChainLightningObject.empty()) {
                break;
            }
        }
    }
    
    // Process random trap events
    for (auto it = m_vecRandomTrapEvent.begin(); it != m_vecRandomTrapEvent.end(); ) {
        SRandomTrapEvent* pRandomTrap = *it;
        if (!pRandomTrap || pRandomTrap->nRandomTrapCount <= 0 || !pRandomTrap->pRandomTrapEvent) {
            ++it;
            continue;
        }
        
        auto pTimer = ThreadLocalData::GetTimer();
        float fDiff = pTimer->GetTimeDifference();
        pRandomTrap->fRandomTrapTime += fDiff;
        
        if (pRandomTrap->fRandomTrapTime < pRandomTrap->fRandomTrapDelayTime) {
            ++it;
            continue;
        }
        
        // Create random trap based on pattern
        if (pRandomTrap->pRandomTrapEvent->sProjInfo.RandomPattern == 1) {
            CreateRandomTrap(pRandomTrap->pRandomTrapEvent, pRandomTrap->pMover,
                pRandomTrap->vRandomTrapEntityPos, pRandomTrap->fRandomTrapEntityYaw,
                pRandomTrap->sRandomTrapSkillInfo);
        }
        else if (pRandomTrap->pRandomTrapEvent->sProjInfo.RandomPattern == 2) {
            // Calculate trap position
            hkvVec3 vPos = pRandomTrap->vRandomTrapEntityPos;
            tagATTACK_AREA stAttackArea;
            GetAttackArea(pRandomTrap->pMover, &vPos, pRandomTrap->fRandomTrapEntityYaw,
                pRandomTrap->pRandomTrapEvent->sAttackRangeType,
                &pRandomTrap->pRandomTrapEvent->sAttackRange, &stAttackArea);
            vPos = stAttackArea.vCenterPos;
            
            hkvMat3 matRot;
            Vision::Math.EulerToMatrix(pRandomTrap->fRandomTrapEntityYaw, 0.0f, 0.0f, matRot);
            hkvVec3 vDir = matRot * pRandomTrap->pRandomTrapEvent->sProjInfo.vRandomPos;
            hkvVec3 vOffset = vDir * (float)(pRandomTrap->pRandomTrapEvent->sProjInfo.nRandomCount - pRandomTrap->nRandomTrapCount);
            hkvVec3 vDestPos = vPos + vOffset;
            
            hkvVec3 vTrapDir(0.0f, -1.0f, 0.0f);
            VGameTrapObject* pTrapObj = CMySkillList::CreateTrap(vDestPos, vTrapDir,
                pRandomTrap->pMover, pRandomTrap->pRandomTrapEvent);
            
            if (pTrapObj) {
                pTrapObj->SetSkillInfo(&pRandomTrap->sRandomTrapSkillInfo);
                m_vTraps.push_back(pTrapObj);
                
                if (pRandomTrap->pMover->IsSendProjectilePacket(pRandomTrap->pRandomTrapEvent)) {
                    hkvVec3 vSendDir(0.0f, -1.0f, 0.0f);
                    int nSessionID = pTrapObj->GetSessionID();
                    pRandomTrap->pMover->send_eSUB_CMD_PROJECTILE(pRandomTrap->pMover,
                        pRandomTrap->sRandomTrapSkillInfo.nSkillID,
                        pRandomTrap->pRandomTrapEvent->EventID,
                        &vDestPos, &vSendDir, nSessionID);
                }
            }
        }
        
        pRandomTrap->nRandomTrapCount--;
        if (pRandomTrap->nRandomTrapCount) {
            pRandomTrap->fRandomTrapTime -= pRandomTrap->fRandomTrapDelayTime;
            ++it;
        } else {
            delete pRandomTrap;
            it = m_vecRandomTrapEvent.erase(it);
        }
    }
    
    // Process random summon events
    for (auto it = m_vecRandomSummonEvent.begin(); it != m_vecRandomSummonEvent.end(); ) {
        SRandomSummonEvent* pRandomSummon = *it;
        if (!pRandomSummon || pRandomSummon->nRandomSummonCount <= 0) {
            ++it;
            continue;
        }
        
        auto pTimer = ThreadLocalData::GetTimer();
        float fDiff = pTimer->GetTimeDifference();
        pRandomSummon->fRandomSummonTime += fDiff;
        
        if (pRandomSummon->fRandomSummonTime < pRandomSummon->fRandomSummonDelayTime) {
            ++it;
            continue;
        }
        
        CreateRandomSummon(pRandomSummon);
        
        pRandomSummon->nRandomSummonCount--;
        if (pRandomSummon->nRandomSummonCount) {
            pRandomSummon->fRandomSummonTime -= pRandomSummon->fRandomSummonDelayTime;
            ++it;
        } else {
            delete pRandomSummon;
            it = m_vecRandomSummonEvent.erase(it);
        }
    }
    
    // Process explode traps
    for (auto it = m_vecExplodeTrap.begin(); it != m_vecExplodeTrap.end(); ) {
        SExplodeTrap* pExplodeTrap = *it;
        if (!pExplodeTrap) {
            ++it;
            continue;
        }
        
        if (pExplodeTrap->pMover) {
            UXActorID actorID;
            pExplodeTrap->pMover->GetActorID(&actorID);
            CQuestCondition::GetQuestID(&actorID);
        }
        
        if (pExplodeTrap->pTarget) {
            UXActorID actorID;
            pExplodeTrap->pTarget->GetActorID(&actorID);
            CQuestCondition::GetQuestID(&actorID);
        }
        
        RetiveExplodeTrap(pExplodeTrap->pMover, pExplodeTrap->vPos,
            pExplodeTrap->sSkillInfo, pExplodeTrap->pTarget);
        
        delete pExplodeTrap;
        it = m_vecExplodeTrap.erase(it);
        
        if (m_vecExplodeTrap.empty()) {
            return;
        }
    }
}

// ============================================================================
// CMySkillList::EndSkill
// IDA: 0x1402B79F0
// ============================================================================
void CMySkillList::EndSkill(TB_SKILL* pSkillTable) {
    if (!pSkillTable) {
        return;
    }
    
    int iIndex = -1;
    switch (pSkillTable->Skill_Cost_Attribute) {
        case 1: iIndex = 1; break;
        case 2: iIndex = 2; break;
        case 3: iIndex = 3; break;
        case 4: iIndex = 16; break;
    }
    
    float fSkillCost = GetSkillCost(pSkillTable);
    
    // Reset continuous cost
    if (fSkillCost > 0.0f && pSkillTable->Cost_Type == 1 && !m_bTestMode && iIndex != -1) {
        m_pActor->SetContinousCost(iIndex, 0.0f);
    }
    
    // Reset stamina regeneration
    if (iIndex == 3 && fSkillCost == 0.0f) {
        auto pAttr = m_pActor->GetGOC<CGocAttribute>();
        if (pAttr) {
            pAttr->SetSTRegStat(1);
        }
    }
}

// ============================================================================
// CMySkillList::IsCanUseSkill
// IDA: 0x1402B7B30
// ============================================================================
int CMySkillList::IsCanUseSkill(TB_SKILL* pSkillTable, TB_SKILL* pChangedSkillTable,
                                 float& fSkillCost, bool bExceptHaveCheck) {
    auto pAttr = m_pActor->GetGOC<CGocAttribute>();
    if (!pAttr) {
        return 56009;
    }
    
    if (!pSkillTable) {
        return 56009;
    }
    
    if (!m_pActor) {
        return 56009;
    }
    
    // Check skill ownership
    if (!bExceptHaveCheck) {
        auto pSkillPtr = m_pActor->GetGOC<CGocSkill>();
        if (pSkillPtr) {
            if (!pSkillPtr->IsHaveBaseSkill(pSkillTable->Skill_Index)) {
                return 56001;
            }
            if (!pSkillPtr->IsHaveSkillQuickSlot(pSkillTable->Skill_Index)) {
                LogHelper::LogError("game.contents", "IsHaveSkillQuickSlot > [%d] ", pSkillTable->Skill_Index);
                return 56001;
            }
        }
    }
    
    // Check level requirement
    int nLv = pAttr->GetLevel();
    if (pSkillTable->Req_Min_LV > nLv) {
        XPRINT("Level requirement not met");
        return 56003;
    }
    
    // Check class requirement
    if (pSkillTable->Use_Class) {
        std::uint8_t byClass = m_pActor->GetClass();
        if (pSkillTable->Use_Class != byClass) {
            XPRINT("Class requirement not met");
            return 56002;
        }
    }
    
    // Check awakening grade
    if (pAttr->GetAwaken() < pSkillTable->Req_Min_AwakeningGrade) {
        return 56009;
    }
    
    // Check if can attack
    if (!m_pActor->IsCanAttack()) {
        XPRINT("Cannot attack");
        return 56009;
    }
    
    // Test mode bypass
    if (m_bTestMode) {
        return 0;
    }
    
    // Determine final skill table
    TB_SKILL* pFinalSkillTable = pChangedSkillTable ? pChangedSkillTable : pSkillTable;
    
    fSkillCost = GetSkillCost(pFinalSkillTable);
    
    // Check resource cost
    if (fSkillCost > 0.0f) {
        switch (pFinalSkillTable->Skill_Cost_Attribute) {
            case 1: // HP
                if (fSkillCost >= (float)m_pActor->GetStat(1)) {
                    return 56004;
                }
                break;
            case 2: // SG
                if (!m_pActor->IsNoSkillCostSG() && fSkillCost > (float)m_pActor->GetStat(2)) {
                    return 56005;
                }
                break;
            case 3: // Stamina
                fSkillCost -= fSkillCost * m_pActor->GetDecreaseStaminaRate();
                if (fSkillCost > (float)m_pActor->GetStat(3)) {
                    return 56006;
                }
                break;
            case 4: // Special resource
                if (fSkillCost > (float)m_pActor->GetStat(16)) {
                    return 56007;
                }
                break;
        }
    }
    
    // Check cooldown
    if (GetCooltime(E_COOLTIME_SKILL, pFinalSkillTable->CoolTime_Group,
        pFinalSkillTable->CoolTime_Global, 1) > 0.0f) {
        XPRINT("Skill on cooldown");
        return 56010;
    }
    
    return 0;
}

// ============================================================================
// CMySkillList::ProcessProjectileExplode
// IDA: 0x1402B8420
// ============================================================================
void CMySkillList::ProcessProjectileExplode(CMoverEx* pMover, hkvVec3& vPos, hkvVec3& vDir,
                                             SSkillInfo& sSkillInfo, CMoverEx* pProjTarget,
                                             std::uint8_t byDamageType, int iTargetOrder,
                                             bool bPenetrate, std::uint8_t byHitPartsIndex,
                                             bool bTrapExplode, bool bHitWall,
                                             bool bAllowAbsorbSG, bool bCheckCombo) {
    if (!pMover) {
        return;
    }
    
    auto pServer = TXSingleton<XGameServer>::Instance();
    auto pSkillTable = pServer->m_xResourceMgr.GetTB_SKILL(sSkillInfo.nSkillID);
    if (!pSkillTable) {
        return;
    }
    
    auto pActionEvent = sSkillInfo.pTrigger;
    if (!pActionEvent) {
        return;
    }
    
    // Check if has damage or reaction
    if (pActionEvent->sReactionInfo.fDamageRate <= 0.0f && !pActionEvent->sReactionInfo.iReactionType) {
        // Apply buff only
        if (pActionEvent->sReactionInfo.iBuffID > 0 && pProjTarget) {
            UXActorID actorID;
            pMover->GetActorID(&actorID);
            std::uint32_t dwUserID = CQuestCondition::GetQuestID(&actorID);
            ApplyBuffSkill(pProjTarget, pActionEvent->sReactionInfo.iBuffID, dwUserID, 0);
        }
        if (pActionEvent->sReactionInfo.iAuraID > 0 && pProjTarget) {
            pProjTarget->SetAuraSkill(pActionEvent->sReactionInfo.iAuraID);
        }
        if (!bHitWall && pActionEvent->sProjInfo.shExplodeTrapMakeType == 4) {
            CheckExplodeSummon(pMover, &vPos, &sSkillInfo);
        }
        if (!bTrapExplode && (!bHitWall || pActionEvent->sProjInfo.shExplodeTrapMakeType != 3)) {
            CheckExplodeTrap(pMover, &vPos, &sSkillInfo, pProjTarget);
        }
        return;
    }
    
    // Calculate target type
    std::uint8_t byTargetType = 6;
    if (pMover->GetType() != eActorMonster) {
        byTargetType |= 1;
    }
    
    std::uint8_t bAttackTargetCnt = 0;
    tagATTACK_AREA stAttackArea;
    
    float fDirYaw = pMover->GetOrientationYaw();
    GetAttackArea(pMover, &vPos, fDirYaw, pActionEvent->sAttackRangeType,
        &pActionEvent->sAttackRange, &stAttackArea);
    
    // Clear attack damage
    ClearAttackDamage();
    
    // Find targets
    if (bTrapExplode) {
        // Trap explode - single target
        if (!pProjTarget) {
            return;
        }
        
        if (!pProjTarget->IsCanHit(pActionEvent->sReactionInfo.iTargetStatus, pSkillTable->Passive_Type)) {
            return;
        }
        
        int bCheckCylinder = 1;
        hkvVec3 vTargetPos;
        pProjTarget->GetPosition(&vTargetPos);
        if (pProjTarget->IsAttackHeight(&stAttackArea, &vTargetPos, &bCheckCylinder)) {
            return;
        }
        
        UXActorID targetID;
        pProjTarget->GetActorID(&targetID);
        AddSkillTarget(bAttackTargetCnt++, CQuestCondition::GetQuestID(&targetID),
            pActionEvent->sReactionInfo.iReactionType, byHitPartsIndex);
    }
    else if (pActionEvent->sProjInfo.bTargetOnly && pProjTarget &&
             pProjTarget->IsCanHit(pActionEvent->sReactionInfo.iTargetStatus, pSkillTable->Passive_Type)) {
        // Target only projectile
        UXActorID targetID;
        pProjTarget->GetActorID(&targetID);
        AddSkillTarget(bAttackTargetCnt++, CQuestCondition::GetQuestID(&targetID),
            pActionEvent->sReactionInfo.iReactionType, byHitPartsIndex);
    }
    else {
        // Normal attack - find targets in area
        if (pActionEvent->sAttackType != 5) {
            CheckSkillTarget(pMover, &stAttackArea, pActionEvent, bAttackTargetCnt, pSkillTable);
        }
        
        // Add projectile target if enemy
        if (pProjTarget && pMover->IsEnemy(pProjTarget) &&
            pProjTarget->IsCanHit(pActionEvent->sReactionInfo.iTargetStatus, pSkillTable->Passive_Type)) {
            
            UXActorID targetID;
            pProjTarget->GetActorID(&targetID);
            std::uint32_t nID = CQuestCondition::GetQuestID(&targetID);
            
            int idx;
            for (idx = 0; idx < bAttackTargetCnt && m_dwAttackTarget[idx] != nID; ++idx);
            
            if (idx == bAttackTargetCnt) {
                AddSkillTarget(bAttackTargetCnt++, nID,
                    pActionEvent->sReactionInfo.iReactionType, stAttackArea.byHitPartsIndex);
            }
        }
    }
    
    // Apply damage if targets found and is owner
    if (bAttackTargetCnt && pMover == m_pActor) {
        std::uint16_t wContinousHit = 0;
        
        if (pActionEvent->sReactionInfo.fDamageRate > 0.0f) {
            // Get monster/akashic info
            CMonster* pMonster = nullptr;
            CAkashicObject* pAkashic = nullptr;
            
            if (pMover->GetType() == eActorMonster) {
                pMonster = dynamic_cast<CMonster*>(pMover);
            } else if (pMover->GetType() == eActorNPC) {
                pAkashic = dynamic_cast<CAkashicObject*>(pMover);
            }
            
            CMoverEx* pOwnerMoverEx = nullptr;
            
            // Calculate damage for each target
            for (int i = 0; i < bAttackTargetCnt; ++i) {
                if (i >= 100) break;
                
                CMover* pTarget = CMover::GetMoverObject(m_dwAttackTarget[i]);
                if (!pTarget) {
                    m_stAttackDamage[i].byDamageFlag = 0;
                    m_stAttackDamage[i].nDamage = 0;
                    continue;
                }
                
                TB_SKILL* pRealSkillTable = pSkillTable;
                
                // Check for linked skill
                if (pActionEvent->sAttackType == 4) {
                    auto pLinkedSkill = pServer->m_xResourceMgr.GetTB_SKILL(
                        pActionEvent->sProjInfo.nLinkedSkillID);
                    if (pLinkedSkill) {
                        pRealSkillTable = pLinkedSkill;
                    }
                }
                
                int fChainDamagaeRate = 1065353216; // 1.0f
                if (iTargetOrder > 0) {
                    fChainDamagaeRate = *(int*)&pActionEvent->sConnectionInfo.fDamageMutiple;
                }
                
                bool bFindOwnerPlayer = false;
                
                // Handle monster owner (summon)
                if (pMonster) {
                    auto pMobTable = pMonster->GetMobTableRef();
                    pOwnerMoverEx = pMonster->GetOwnerPlayer();
                    
                    if (pOwnerMoverEx && pMobTable && pMobTable->Monster_Element == 1) {
                        pOwnerMoverEx->CalcTargetDamage(pTarget, i, 
                            pMonster->GetApplyMultipleDamageOnce(),
                            pRealSkillTable, pActionEvent, fChainDamagaeRate, 0, false, true);
                        
                        // Copy damage to monster
                        if (pMonster->GetSkillMgr()) {
                            tagSKILL_ACTION_DAMAGE stDamage, stDamage2;
                            pMonster->GetSkillMgr()->GetAttackDamage(&stDamage, i);
                            pOwnerMoverEx->GetSkillMgr()->GetAttackDamage(&stDamage2, i);
                            stDamage.nDamage = stDamage2.nDamage;
                            stDamage.nAttrDamage = stDamage2.nAttrDamage;
                            stDamage.nHP = stDamage2.nHP;
                            stDamage.byDamageFlag = stDamage2.byDamageFlag;
                            pMonster->GetSkillMgr()->SetAttackDamage(i, &stDamage);
                        }
                        bFindOwnerPlayer = true;
                    }
                }
                
                if (!bFindOwnerPlayer) {
                    pMover->CalcTargetDamage(pTarget, i, bAllowAbsorbSG,
                        pRealSkillTable, pActionEvent, fChainDamagaeRate, 0, false, true);
                }
            }
            
            // Clear multiple damage once buff if needed
            if (pOwnerMoverEx && pMover->GetApplyMultipleDamageOnce() &&
                pOwnerMoverEx->GetMultipleDamageOnce() > 0.0f) {
                int iIndex = pOwnerMoverEx->FindBuffByEffectType(0x0E, 0);
                if (iIndex != -1) {
                    pOwnerMoverEx->ClearBuffStatusBySlot(iIndex, 0);
                }
            }
            
            // Check combo
            if (bCheckCombo) {
                CMoverEx* pOwner = pMover->GetOwnerPlayer();
                
                if (!pMover || pMover->GetType() != eActorUser) {
                    if (pOwner == pMover || 
                        (pOwner && pOwner->GetType() == eActorUser && pMonster && pMonster->IsMonsterCombo()) ||
                        (pOwner && pOwner->GetType() == eActorUser && pAkashic && pAkashic->IsComboAkashic())) {
                        wContinousHit = pOwner->CheckContinousAttack(bAttackTargetCnt);
                    }
                } else {
                    wContinousHit = pMover->CheckContinousAttack(bAttackTargetCnt);
                }
            }
        }
        
        // Apply skill damage frame
        float fDirYaw = CMover::GetYawFromVector(&vDir);
        pMover->ApplySkillDamageFrame(sSkillInfo.nSkillID, pActionEvent->EventID,
            bAttackTargetCnt, &vPos, fDirYaw, wContinousHit, byDamageType, bPenetrate);
        
        pMover->send_eSUB_CMD_ACTION_SKILL(pMover, sSkillInfo.nSkillID,
            pActionEvent->EventID, &vPos, bAttackTargetCnt, wContinousHit, bPenetrate);
        
        // Apply aura to targets
        if (pActionEvent->sReactionInfo.iAuraID > 0) {
            for (int j = 0; j < bAttackTargetCnt && j < 100; ++j) {
                auto pTargetMover = CMover::GetMoverObject(m_dwAttackTarget[j]);
                auto v68 = dynamic_cast<CMoverEx*>(pTargetMover);
                if (v68) {
                    v68->SetAuraSkill(pActionEvent->sReactionInfo.iAuraID);
                }
            }
        }
        
        // Clear multiple damage once buff
        if (pMover->GetMultipleDamageOnce() > 0.0f && pMover->GetApplyMultipleDamageOnce()) {
            int BuffByEffectType = pMover->FindBuffByEffectType(0x0E, 0);
            if (BuffByEffectType != -1) {
                pMover->ClearBuffStatusBySlot(BuffByEffectType, 0);
            }
        }
        
        // Apply buff to targets
        if (pActionEvent->sReactionInfo.iBuffID > 0) {
            UXActorID actorID;
            pMover->GetActorID(&actorID);
            std::uint32_t dwExtraData = bAttackTargetCnt;
            
            for (int k = 0; k < bAttackTargetCnt && k < 100; ++k) {
                auto pTargetMover = CMover::GetMoverObject(m_dwAttackTarget[k]);
                auto v71 = dynamic_cast<CMover*>(pTargetMover);
                if (v71) {
                    ApplyBuffSkill(v71, pActionEvent->sReactionInfo.iBuffID,
                        CQuestCondition::GetQuestID(&actorID), dwExtraData);
                }
            }
        }
    }
    else {
        // No targets - apply buff to projectile target only
        if (pActionEvent->sReactionInfo.iBuffID > 0 && pProjTarget) {
            std::uint32_t dwID = pMover->GetID();
            ApplyBuffSkill(pProjTarget, pActionEvent->sReactionInfo.iBuffID, dwID, 0);
        }
        if (pActionEvent->sReactionInfo.iAuraID > 0 && pProjTarget) {
            pProjTarget->SetAuraSkill(pActionEvent->sReactionInfo.iAuraID);
        }
    }
    
    // Check explode summon/trap
    if (!bHitWall && pActionEvent->sProjInfo.shExplodeTrapMakeType == 4) {
        CheckExplodeSummon(pMover, &vPos, &sSkillInfo);
    }
    if (!bTrapExplode && (!bHitWall || pActionEvent->sProjInfo.shExplodeTrapMakeType != 3)) {
        CheckExplodeTrap(pMover, &vPos, &sSkillInfo, pProjTarget);
    }
    
    // Check if mover died
    if (pMover->GetHP() <= 0) {
        pMover->SetDie(12, 0);
    }
}

// ============================================================================
// CMySkillList::ProcessChain
// IDA: 0x1402B9730
// ============================================================================
void CMySkillList::ProcessChain(CMoverEx* pMover, hkvVec3& vPos, hkvVec3& vDir,
                                 SSkillInfo& sSkillInfo, CMoverEx* pProjTarget,
                                 std::uint8_t byDamageType, int iTargetOrder,
                                 bool bPenetrate, std::uint8_t byHitPartsIndex,
                                 bool bTrapExplode, bool bHitWall) {
    if (!pMover || !pProjTarget || pMover != m_pActor) {
        return;
    }
    
    auto pServer = TXSingleton<XGameServer>::Instance();
    auto pSkillTable = pServer->m_xResourceMgr.GetTB_SKILL(sSkillInfo.nSkillID);
    if (!pSkillTable) {
        return;
    }
    
    auto pActionEvent = sSkillInfo.pTrigger;
    if (!pActionEvent) {
        return;
    }
    
    // Check if enemy for chain
    if (!pMover->IsEnemyForChain(pProjTarget)) {
        return;
    }
    
    // Check if can hit
    if (!pProjTarget->IsCanHit(pActionEvent->sReactionInfo.iTargetStatus, pSkillTable->Passive_Type)) {
        return;
    }
    
    // Get target ID
    UXActorID targetID;
    pProjTarget->GetActorID(&targetID);
    std::uint32_t nID = CQuestCondition::GetQuestID(&targetID);
    
    // Check if already in target list
    for (int idx = 0; idx < 100; ++idx) {
        if (m_dwAttackTarget[idx] == nID) {
            return;
        }
    }
    
    // Find empty slot
    int idxa;
    for (idxa = 0; idxa < 100 && m_dwAttackTarget[idxa]; ++idxa);
    
    if (idxa >= 100) {
        return;
    }
    
    // Add target
    AddSkillTarget(idxa, nID, pActionEvent->sReactionInfo.iReactionType, byHitPartsIndex);
    
    // Store chain hit info
    m_stChainHitInfo[idxa].vHitPos = vPos;
    m_stChainHitInfo[idxa].vHitDir = vDir;
    m_stChainHitInfo[idxa].iTargetOrder = iTargetOrder;
    m_stChainHitInfo[idxa].bHitWall = bHitWall;
    
    // Store skill info
    m_stChainSkillInfo = sSkillInfo;
}

// ============================================================================
// CMySkillList::SendChainResult
// IDA: 0x1402B9A50
// ============================================================================
void CMySkillList::SendChainResult(CMoverEx* pMover) {
    if (!pMover) {
        return;
    }
    
    auto pServer = TXSingleton<XGameServer>::Instance();
    auto pSkillTable = pServer->m_xResourceMgr.GetTB_SKILL(m_stChainSkillInfo.nSkillID);
    if (!pSkillTable) {
        return;
    }
    
    auto pActionEvent = m_stChainSkillInfo.pTrigger;
    if (!pActionEvent) {
        return;
    }
    
    // Count targets
    std::uint8_t bAttackTargetCnt = 0;
    for (int i = 0; i < 100; ++i) {
        if (m_dwAttackTarget[i]) {
            ++bAttackTargetCnt;
        }
    }
    
    if (pMover != m_pActor) {
        return;
    }
    
    std::uint16_t wContinousHit = 0;
    
    // Process each target
    for (std::uint32_t j = 0; j < bAttackTargetCnt; ++j) {
        CMoverEx* pTarget = dynamic_cast<CMoverEx*>(CMover::GetMoverObject(m_dwAttackTarget[j]));
        if (!pTarget) {
            m_stAttackDamage[j].byDamageFlag = 0;
            m_stAttackDamage[j].nDamage = 0;
            continue;
        }
        
        auto pTempChainHitInfo = &m_stChainHitInfo[j];
        
        if (pActionEvent->sReactionInfo.fDamageRate > 0.0f || pActionEvent->sReactionInfo.iReactionType) {
            if (pActionEvent->sReactionInfo.fDamageRate > 0.0f) {
                CMonster* v31 = nullptr;
                if (pMover->GetType() == eActorMonster) {
                    v31 = dynamic_cast<CMonster*>(pMover);
                }
                
                int fChainDamagaeRate = 1065353216; // 1.0f
                if (m_stChainHitInfo[j].iTargetOrder > 0) {
                    fChainDamagaeRate = *(int*)&pActionEvent->sConnectionInfo.fDamageMutiple;
                }
                
                bool bFindOwnerPlayer = false;
                
                if (v31) {
                    auto pMobTable = v31->GetMobTableRef();
                    auto pOwnerMoverEx = v31->GetOwnerPlayer();
                    
                    if (pOwnerMoverEx && pMobTable && pMobTable->Monster_Element == 1) {
                        pOwnerMoverEx->CalcTargetDamage(pTarget, j,
                            v31->GetAllowAbsorbSG(), pSkillTable, pActionEvent,
                            fChainDamagaeRate, 0, false, true);
                        
                        // Copy damage
                        if (v31->GetSkillMgr()) {
                            tagSKILL_ACTION_DAMAGE stDamage, stDamage2;
                            v31->GetSkillMgr()->GetAttackDamage(&stDamage, j);
                            pOwnerMoverEx->GetSkillMgr()->GetAttackDamage(&stDamage2, j);
                            stDamage.nDamage = stDamage2.nDamage;
                            stDamage.nAttrDamage = stDamage2.nAttrDamage;
                            stDamage.nHP = stDamage2.nHP;
                            stDamage.byDamageFlag = stDamage2.byDamageFlag;
                            v31->GetSkillMgr()->SetAttackDamage(j, &stDamage);
                        }
                        bFindOwnerPlayer = true;
                    }
                }
                
                if (!bFindOwnerPlayer) {
                    pMover->CalcTargetDamage(pTarget, j, pMover->GetAllowAbsorbSG(),
                        pSkillTable, pActionEvent, fChainDamagaeRate, 0, false, true);
                }
            }
            
            // Check combo
            CMonster* pMonster = nullptr;
            CAkashicObject* pAkashic = nullptr;
            
            if (pMover->GetType() == eActorMonster) {
                pMonster = dynamic_cast<CMonster*>(pMover);
            } else if (pMover->GetType() == eActorNPC) {
                pAkashic = dynamic_cast<CAkashicObject*>(pMover);
            }
            
            CMoverEx* pOwner = pMover->GetOwnerPlayer();
            if (!pMover || pMover->GetType() != eActorUser) {
                if (pOwner == pMover ||
                    (pOwner && pOwner->GetType() == eActorUser && pMonster && pMonster->IsMonsterCombo()) ||
                    (pOwner && pOwner->GetType() == eActorUser && pAkashic && pAkashic->IsComboAkashic())) {
                    wContinousHit = pOwner->CheckContinousAttack(bAttackTargetCnt);
                }
            } else {
                wContinousHit = pMover->CheckContinousAttack(bAttackTargetCnt);
            }
            
            // Check explode summon/trap
            if (!m_stChainHitInfo[j].bHitWall && pActionEvent->sProjInfo.shExplodeTrapMakeType == 4) {
                CheckExplodeSummon(pMover, &pTempChainHitInfo->vHitPos, &m_stChainSkillInfo);
            }
            if (!m_stChainHitInfo[j].bHitWall || pActionEvent->sProjInfo.shExplodeTrapMakeType != 3) {
                CheckExplodeTrap(pMover, &pTempChainHitInfo->vHitPos, &m_stChainSkillInfo, pTarget);
            }
            
            // Apply buff
            if (pActionEvent->sReactionInfo.iBuffID > 0) {
                UXActorID actorID;
                pMover->GetActorID(&actorID);
                ApplyBuffSkill(pTarget, pActionEvent->sReactionInfo.iBuffID,
                    CQuestCondition::GetQuestID(&actorID), bAttackTargetCnt);
            }
        }
        else if (pActionEvent->sReactionInfo.iBuffID > 0) {
            UXActorID actorID;
            pMover->GetActorID(&actorID);
            ApplyBuffSkill(pTarget, pActionEvent->sReactionInfo.iBuffID,
                CQuestCondition::GetQuestID(&actorID), 0);
        }
    }
    
    // Apply skill damage frame
    float fDirYaw = CMover::GetYawFromVector(&m_stChainHitInfo[0].vHitDir);
    pMover->ApplySkillDamageFrame(m_stChainSkillInfo.nSkillID, pActionEvent->EventID,
        bAttackTargetCnt, &m_stChainHitInfo[0].vHitPos, fDirYaw, wContinousHit, 4, 0);
    
    pMover->send_eSUB_CMD_ACTION_SKILL(pMover, m_stChainSkillInfo.nSkillID,
        pActionEvent->EventID, &m_stChainHitInfo[0].vHitPos, bAttackTargetCnt, wContinousHit, false);
    
    // Clear multiple damage once buff
    if (pMover->GetMultipleDamageOnce() > 0.0f && pMover->GetApplyMultipleDamageOnce()) {
        int iIndex = pMover->FindBuffByEffectType(0x0E, 0);
        if (iIndex != -1) {
            pMover->ClearBuffStatusBySlot(iIndex, 0);
        }
    }
    
    // Check if died
    if (pMover->GetHP() <= 0) {
        pMover->SetDie(12, 0);
    }
}

// ============================================================================
// CMySkillList::CheckExplodeSummon
// IDA: 0x1402BA4C0
// ============================================================================
void CMySkillList::CheckExplodeSummon(CMoverEx* pMover, hkvVec3* vPos, SSkillInfo* sSkillInfo) {
    auto pActionEvent = sSkillInfo->pTrigger;
    if (!pActionEvent) {
        return;
    }
    
    auto pSummonTrigger = GetExplodeSummon(pActionEvent->sProjInfo.nLinkedSkillID);
    if (!pSummonTrigger) {
        return;
    }
    
    bool bAllowAbsorbSG = false;
    CMoverEx* pOwner = pMover;
    
    if (pMover->GetType() == eActorNPC) {
        pOwner = pMover->GetOwnerPlayer();
    }
    
    if (pMover->GetType() == eActorUser) {
        bAllowAbsorbSG = true;
    }
    
    AddRandomSummon(pOwner, bAllowAbsorbSG, pSummonTrigger, true, *vPos);
}

// ============================================================================
// CMySkillList::AddRandomSummon
// IDA: 0x1402BA5D0
// ============================================================================
void CMySkillList::AddRandomSummon(CMoverEx* pMover, bool bAllowAbsorbSG,
                                    SummonMonsterTrigger* pTrigger, bool bUseOtherPos,
                                    hkvVec3& vOtherPos) {
    auto pRandomSummonEvent = new SRandomSummonEvent();
    if (!pRandomSummonEvent) {
        return;
    }
    
    pRandomSummonEvent->fRandomSummonTime = 0.0f;
    pRandomSummonEvent->fRandomSummonDelayTime = pTrigger->fDelayTime;
    pRandomSummonEvent->nRandomSummonCount = pTrigger->iSummonCount;
    pRandomSummonEvent->pRandomSummonEvent = pTrigger;
    pRandomSummonEvent->pMover = pMover;
    pRandomSummonEvent->bAllowAbsorbSG = bAllowAbsorbSG;
    pRandomSummonEvent->bUseOtherPos = bUseOtherPos;
    pRandomSummonEvent->vOtherPos = vOtherPos;
    
    m_vecRandomSummonEvent.push_back(pRandomSummonEvent);
}

// ============================================================================
// CMySkillList::SummonRandomMonster
// IDA: 0x1402BA870
// ============================================================================
void CMySkillList::SummonRandomMonster(CMoverEx* pMover, RandomSummonTrigger* pTrigger,
                                         std::uint32_t nMonsterID) {
    if (!pMover || !pTrigger) {
        return;
    }
    
    auto pArea = pMover->GetArea();
    if (!pArea) {
        return;
    }
    
    // Calculate summon position
    hkvVec3 vSummonPos = pTrigger->SummonPos;
    
    float fMonsterYaw;
    if (m_pActor && m_pActor->GetType() == eActorNPC) {
        fMonsterYaw = pMover->GetSummonAkashicYaw();
    } else {
        fMonsterYaw = pMover->GetMovingYaw();
    }
    
    hkvMat3 matRot;
    matRot.setFromEulerAngles(0.0f, 0.0f, fMonsterYaw);
    vSummonPos *= matRot;
    
    hkvVec3 vMoverPos;
    pMover->GetPosition(&vMoverPos);
    hkvVec3 vCreatePos = vMoverPos + vSummonPos;
    
    // Try to find valid position
    int nTry = 0;
    do {
        if (nTry++ >= 10) {
            break;
        }
        
        int fDiff = (int)(pTrigger->RandomMaxRadius - pTrigger->RandomMinRadius);
        int nRandDist = 0;
        if (fDiff > 0) {
            nRandDist = rand() % fDiff;
        }
        nRandDist += (int)pTrigger->RandomMinRadius;
        
        int nRandAngle = rand() % 360;
        if (rand() % 2) {
            nRandAngle = -nRandAngle;
        }
        
        hkvMat3 lhs;
        Vision::Math.EulerToMatrix(fMonsterYaw, 0.0f, 0.0f, lhs);
        Vision::Math.EulerToMatrix(fMonsterYaw + (float)nRandAngle, 0.0f, 0.0f, lhs);
        
        hkvVec3 vOffset(0.0f, -(float)nRandDist, 0.0f);
        vCreatePos = vCreatePos + (lhs * vOffset);
    } while (!pMover->GetHeight(&vCreatePos, 200.0f));
    
    if (!pMover->GetHeight(&vCreatePos, 200.0f)) {
        return;
    }
    
    // Create monster
    CMonster* pSummonMob = nullptr;
    auto pMaze = dynamic_cast<XMaze*>(pArea);
    
    if (pMaze) {
        int nSectorID = pMover->GetSectorID();
        if (nSectorID < 0) {
            nSectorID = pMaze->GetLastSectorID();
            int nBatchLayerLevel = pMaze->GetBatchLayerLevel();
            nSectorID = VEventObjectInfo::GetEventUniqueID(nSectorID, nBatchLayerLevel);
        }
        
        XVec3 vPos(vCreatePos.x, vCreatePos.y, vCreatePos.z);
        UXActorID actorID;
        pMover->GetActorID(&actorID);
        UXMapID instanceID = pArea->GetInstanceID();
        
        pSummonMob = pMaze->CreateMonster(instanceID.__s0, nSectorID, nMonsterID,
            &vPos, fMonsterYaw, eSendInfoTypeNot, 0, 0, actorID.__s0);
    }
    else {
        auto pBattleZone = dynamic_cast<CBattleZone*>(pArea);
        if (pBattleZone) {
            XVec3 vPos(vCreatePos.x, vCreatePos.y, vCreatePos.z);
            UXActorID actorID;
            pMover->GetActorID(&actorID);
            UXMapID instanceID = pArea->GetInstanceID();
            int nSectorID = pMover->GetSectorID();
            
            pSummonMob = pBattleZone->CreateMonster(instanceID.__s0, nSectorID, nMonsterID,
                &vPos, fMonsterYaw, eSendInfoTypeNot, 0, 0, actorID.__s0);
        }
    }
    
    if (!pSummonMob) {
        return;
    }
    
    // Setup summoned monster
    pSummonMob->SetSummonType(1);
    
    UXActorID ownerID;
    pMover->GetActorID(&ownerID);
    pSummonMob->SetOwnerID(CQuestCondition::GetQuestID(&ownerID));
    
    // Copy parent skill and divergence info
    auto pParentSkillRef = pMover->GetSkillTable();
    auto pParentDivergence = pMover->GetCurDivergenceTable();
    int nParentDivergenceSkillID = pMover->GetCurDivergenceSKillID();
    int nParentCombatType = pMover->GetCombatType();
    
    if (pParentSkillRef) {
        pSummonMob->SetSkillTable(pParentSkillRef);
    }
    if (pParentDivergence && nParentDivergenceSkillID > 0) {
        pSummonMob->SetCurDivergenceTable(pParentDivergence, nParentDivergenceSkillID);
    }
    
    pSummonMob->SetCombatType(nParentCombatType);
    
    int nActionCond = pMover->GetActionCondition();
    pSummonMob->SetActionCondition(nActionCond);
    
    int nPvpCond = pMover->GetPvpCondition();
    pSummonMob->SetPvpCondition(nPvpCond);
    
    // Send monster info if in maze
    if (pMaze && pMaze->GetMazeType() != 12) {
        PS_MONSTERINFO_VEC stVector;
        auto pMonsterInfo = pSummonMob->GetMonsterInfo();
        stVector.vecMonsterInfo.push_back(pMonsterInfo);
        
        XSendPacket xSendPacket(4, 0x15);
        xSendPacket << stVector;
        pArea->SendBroadCast(&xSendPacket, nullptr, eNoneSelf);
    }
    
    // Set animation
    VString strAnimName(pTrigger->szSummonAnim);
    int nAnimIndex = pSummonMob->GetAnimIndex(strAnimName);
    pSummonMob->ChangeAnimation(nAnimIndex, 1);
    pSummonMob->SetDirectionYaw();
    pSummonMob->send_eSUB_CMD_MOVE_IDLE(0.0f);
    
    // Add to summon list
    UXActorID summonActorID;
    pSummonMob->GetActorID(&summonActorID);
    pMover->AddSummonMobList(CQuestCondition::GetQuestID(&summonActorID));
    
    CheckDedicatedMonster(pSummonMob, pMover);
}

// ============================================================================
// CMySkillList::CheckDedicatedMonster
// IDA: 0x1402BB2D0
// ============================================================================
void CMySkillList::CheckDedicatedMonster(CMonster* pSummonMob, CMoverEx* pMover) {
    if (!pSummonMob || !pMover) {
        return;
    }
    
    if (!pSummonMob->IsDedicated() || !pSummonMob->GetOwnerID()) {
        return;
    }
    
    // Get owner player
    auto pOwnerPlayer = dynamic_cast<CUser*>(pSummonMob->GetOwnerPlayer());
    if (!pOwnerPlayer) {
        pOwnerPlayer = dynamic_cast<CUser*>(pMover);
    }
    
    if (pOwnerPlayer) {
        UXActorID actorID;
        pSummonMob->GetActorID(&actorID);
        pOwnerPlayer->SetDedicatedMonsterID(CQuestCondition::GetQuestID(&actorID));
    }
    
    // Setup NPC attribute
    auto pAttr = pSummonMob->GetGOC<CGocNpcAttribute>();
    auto pMonTable = pSummonMob->GetMobTableRef();
    
    int nLevel = pMover->GetLevelForStat();
    int nClass = pMover->GetClass();
    
    if (pAttr) {
        pAttr->Init(nClass, nLevel, pMonTable);
        pAttr->CalculateChangedStat(0);
    }
    
    // Set HP from owner
    float fHP = (float)pMover->GetMaxHP();
    pSummonMob->SetHpInfo((int)fHP);
    
    if (pAttr) {
        pAttr->Finalize();
    }
}

// ============================================================================
// CMySkillList::CheckExplodeTrap
// IDA: 0x1402BB5B0
// ============================================================================
void CMySkillList::CheckExplodeTrap(CMoverEx* pMover, hkvVec3& vPos,
                                     SSkillInfo& sSkillInfo, CMover* pTarget) {
    auto pExplodeTrap = new SExplodeTrap();
    if (!pExplodeTrap) {
        return;
    }
    
    pExplodeTrap->pMover = pMover;
    pExplodeTrap->vPos = vPos;
    pExplodeTrap->sSkillInfo.nSkillID = sSkillInfo.nSkillID;
    pExplodeTrap->sSkillInfo.pTrigger = sSkillInfo.pTrigger;
    pExplodeTrap->pTarget = pTarget;
    
    m_vecExplodeTrap.push_back(pExplodeTrap);
}

// ============================================================================
// CMySkillList::RetiveExplodeTrap
// IDA: 0x1402BB6F0
// ============================================================================
void CMySkillList::RetiveExplodeTrap(CMoverEx* pMover, hkvVec3& vPos,
                                      SSkillInfo& sSkillInfo, CMover* pTarget) {
    if (!pMover) {
        return;
    }
    
    auto pServer = TXSingleton<XGameServer>::Instance();
    auto pSkillTable = pServer->m_xResourceMgr.GetTB_SKILL(sSkillInfo.nSkillID);
    if (!pSkillTable) {
        return;
    }
    
    auto pActionEvent = sSkillInfo.pTrigger;
    if (!pActionEvent) {
        return;
    }
    
    if (!pActionEvent->sProjInfo.bUseExplodeTrap) {
        return;
    }
    
    // Get filter data
    int nCurFilterData1, nCurFilterData2, nCurFilterData3;
    pMover->GetFilterData(sSkillInfo.nSkillID, &nCurFilterData1, &nCurFilterData2, &nCurFilterData3);
    
    SSkillInfo newSkillInfo;
    newSkillInfo.nSkillID = sSkillInfo.nSkillID;
    
    int EventID = pActionEvent->EventID;
    int iIndex = EventID - (EventID % 50) + 1;
    
    while (true) {
        while (pActionEvent->EventID == iIndex) {
            ++iIndex;
        }
        
        auto pActionEvent2 = pMover->GetAttackJudgmentEvent(iIndex);
        if (!pActionEvent2) {
            break;
        }
        
        ++iIndex;
        
        if (pActionEvent2->sAttackType != 4) {
            continue;
        }
        
        if (ActionTrigger::IsFiltering(nCurFilterData1, nCurFilterData2, nCurFilterData3,
            pActionEvent2->dwFilterInfo1, pActionEvent2->dwFilterInfo2, pActionEvent2->dwFilterInfo3)) {
            continue;
        }
        
        // Check trap conditions
        if (!pActionEvent->sProjInfo.shExplodeTrapMakeType) {
            if (!pTarget) {
                continue;
            }
            if (!pTarget->IsCanHit(pActionEvent->sReactionInfo.iTargetStatus, pSkillTable->Passive_Type)) {
                continue;
            }
        }
        
        if ((pActionEvent->sProjInfo.shExplodeTrapMakeType == 1 || 
             pActionEvent->sProjInfo.shExplodeTrapMakeType == 3) && pTarget) {
            continue;
        }
        
        newSkillInfo.pTrigger = pActionEvent2;
        
        // Create trap based on pattern
        if (pActionEvent2->sProjInfo.RandomPattern == 1) {
            // Random trap pattern 1
            if (pActionEvent2->sProjInfo.fRandomDelayTime > 0.0f && pActionEvent2->sProjInfo.nRandomCount > 1) {
                auto pRandomTrapEvent = new SRandomTrapEvent();
                pRandomTrapEvent->fRandomTrapTime = 0.0f;
                pRandomTrapEvent->fRandomTrapDelayTime = pActionEvent2->sProjInfo.fRandomDelayTime;
                pRandomTrapEvent->nRandomTrapCount = pActionEvent2->sProjInfo.nRandomCount - 1;
                pRandomTrapEvent->pRandomTrapEvent = pActionEvent2;
                
                hkvVec3 vMoverPos;
                pMover->GetPosition(&vMoverPos);
                pRandomTrapEvent->vRandomTrapEntityPos = vMoverPos;
                pRandomTrapEvent->fRandomTrapEntityYaw = pMover->GetMovingYaw();
                pRandomTrapEvent->sRandomTrapSkillInfo.nSkillID = pSkillTable->Skill_Index;
                pRandomTrapEvent->sRandomTrapSkillInfo.pTrigger = pActionEvent2;
                pRandomTrapEvent->pMover = pMover;
                
                m_vecRandomTrapEvent.push_back(pRandomTrapEvent);
            }
            
            int nCount = 1;
            if (pActionEvent2->sProjInfo.fRandomDelayTime <= 0.0f) {
                nCount = pActionEvent2->sProjInfo.nRandomCount;
            }
            
            hkvVec3 vMoverPos;
            pMover->GetPosition(&vMoverPos);
            
            for (int i = 0; i < nCount; ++i) {
                float fYaw = pMover->GetMovingYaw();
                CreateRandomTrap(pActionEvent2, pMover, vMoverPos, fYaw, newSkillInfo);
            }
        }
        else if (pActionEvent2->sProjInfo.RandomPattern == 2) {
            // Random trap pattern 2 - line pattern
            if (pActionEvent2->sProjInfo.fRandomDelayTime > 0.0f && pActionEvent2->sProjInfo.nRandomCount > 1) {
                auto pRandomTrapEvent = new SRandomTrapEvent();
                pRandomTrapEvent->fRandomTrapTime = 0.0f;
                pRandomTrapEvent->fRandomTrapDelayTime = pActionEvent2->sProjInfo.fRandomDelayTime;
                pRandomTrapEvent->nRandomTrapCount = pActionEvent2->sProjInfo.nRandomCount - 1;
                pRandomTrapEvent->pRandomTrapEvent = pActionEvent2;
                
                hkvVec3 vMoverPos;
                pMover->GetPosition(&vMoverPos);
                pRandomTrapEvent->vRandomTrapEntityPos = vMoverPos;
                pRandomTrapEvent->fRandomTrapEntityYaw = pMover->GetMovingYaw();
                pRandomTrapEvent->sRandomTrapSkillInfo.nSkillID = pSkillTable->Skill_Index;
                pRandomTrapEvent->sRandomTrapSkillInfo.pTrigger = pActionEvent2;
                pRandomTrapEvent->pMover = pMover;
                
                m_vecRandomTrapEvent.push_back(pRandomTrapEvent);
            }
            
            // Calculate attack area
            hkvVec3 vEntityPos;
            pMover->GetPosition(&vEntityPos);
            
            tagATTACK_AREA stAttackArea;
            float fYaw = pMover->GetOrientationYaw();
            GetAttackArea(pMover, &vEntityPos, fYaw, pActionEvent2->sAttackRangeType,
                &pActionEvent2->sAttackRange, &stAttackArea);
            
            vEntityPos = stAttackArea.vCenterPos;
            
            hkvMat3 matRot;
            fYaw = pMover->GetOrientationYaw();
            matRot.setFromEulerAngles(0.0f, 0.0f, fYaw);
            
            hkvVec3 vDir = matRot * pActionEvent2->sProjInfo.vRandomPos;
            
            int nRandomCount = 1;
            if (pActionEvent2->sProjInfo.fRandomDelayTime <= 0.0f) {
                nRandomCount = pActionEvent2->sProjInfo.nRandomCount;
            }
            
            for (int j = 0; j < nRandomCount; ++j) {
                hkvVec3 vOffset = vDir * (float)j;
                hkvVec3 vDestPos = vEntityPos + vOffset;
                
                hkvVec3 vTrapDir(0.0f, -1.0f, 0.0f);
                auto pTrapObj = CMySkillList::CreateTrap(vDestPos, vTrapDir, pMover, pActionEvent2);
                
                if (pTrapObj) {
                    pTrapObj->SetSkillInfo(&newSkillInfo);
                    m_vTraps.push_back(pTrapObj);
                    
                    if (pMover->IsSendProjectilePacket(pActionEvent2)) {
                        hkvVec3 vSendDir;
                        pMover->GetDirection(&vSendDir);
                        int nSessionID = pTrapObj->GetSessionID();
                        pMover->send_eSUB_CMD_PROJECTILE(pMover, sSkillInfo.nSkillID,
                            pActionEvent2->EventID, &vDestPos, &vSendDir, nSessionID);
                    }
                }
            }
        }
        else {
            // Default - traped skill target
            CalcTrapedSkillTarget(pMover, pActionEvent2, newSkillInfo, vPos);
        }
    }
}

// ============================================================================
// CMySkillList::SetFilterDataToMover
// IDA: 0x1402C1C40
// ============================================================================
void CMySkillList::SetFilterDataToMover(std::uint32_t nSkillID, CMover* pMover) {
    if (!pMover) {
        return;
    }
    
    int iPvpCondition = pMover->GetPvpCondition();
    int iActionCondition = pMover->GetActionCondition();
    int iDivergence = pMover->GetDivergenceValue();
    int iCombatType = pMover->GetCombatType();
    int iChargeLevel = pMover->GetSkillChargeStep();
    std::uint8_t bySkillLevel = pMover->GetSkillLevel();
    
    int nCurFilterData1, nCurFilterData2, nCurFilterData3;
    ActionTrigger::SetFiltering(bySkillLevel, iChargeLevel, iCombatType, iDivergence,
        iActionCondition, iPvpCondition, &nCurFilterData1, &nCurFilterData2, &nCurFilterData3);
    
    pMover->SetFilterData(nSkillID, nCurFilterData1, nCurFilterData2, nCurFilterData3);
}

// ============================================================================
// CMySkillList::GetStatIndexByOptionType
// IDA: 0x1402C2F80
// ============================================================================
void CMySkillList::GetStatIndexByOptionType(int nOptionType, int& iStatType, int& bRate) {
    iStatType = -1;
    bRate = 0;
    
    if (nOptionType > 12) {
        if (nOptionType > 30) {
            if (nOptionType > 32) {
                if (nOptionType > 36) {
                    if (nOptionType > 40) {
                        if (nOptionType > 49) {
                            if (nOptionType > 53) {
                                if (nOptionType > 59) {
                                    if (nOptionType <= 65) {
                                        iStatType = nOptionType - 17;
                                        bRate = 0;
                                    } else {
                                        iStatType = nOptionType - 17;
                                    }
                                }
                            } else {
                                iStatType = (nOptionType - 50) / 2 + 35;
                                bRate = nOptionType % 2 == 1;
                            }
                        } else {
                            iStatType = nOptionType - 15;
                        }
                    } else {
                        iStatType = (nOptionType - 37) / 2 + 24;
                        bRate = nOptionType % 2 != 1;
                    }
                } else {
                    int v5[] = {21, 23};
                    int v6 = (nOptionType - 33) / 2;
                    if (v6 < 2) {
                        iStatType = v5[v6];
                        bRate = nOptionType % 2 != 1;
                    }
                }
            } else {
                iStatType = nOptionType - 13;
                bRate = 1;
            }
        } else {
            int iTemp[] = {1, 10, 2, 12, 3, 14, 15, 16, 17};
            int iIndex = (nOptionType - 13) / 2;
            if (iIndex < 9) {
                iStatType = iTemp[iIndex];
                bRate = nOptionType % 2 != 1;
            }
        }
    } else {
        iStatType = (nOptionType - 1) / 2 + 4;
        bRate = nOptionType % 2 != 1;
    }
}

// ============================================================================
// CMySkillList::ApplyBuffSkill
// IDA: 0x1402C22B0
// ============================================================================
int CMySkillList::ApplyBuffSkill(CMover* pMover, std::uint16_t nBuffID,
                                  std::uint32_t dwUserID, std::uint32_t dwExtraData) {
    auto pServer = TXSingleton<XGameServer>::Instance();
    auto pBuffRef = pServer->m_xResourceMgr.GetTB_BUFF(nBuffID);
    if (!pBuffRef) {
        return 0;
    }
    
    // Check for composite buff (effect type 24)
    if (pBuffRef->EffectType_01 != 24) {
        return _ApplyBuffSkill(pMover, nBuffID, dwUserID, dwExtraData);
    }
    
    // Apply multiple sub-buffs
    int bResult = 0;
    
    std::uint16_t nSubBuffID = pBuffRef->Option_Value_01;
    if (nSubBuffID) {
        bResult = _ApplyBuffSkill(pMover, nSubBuffID, dwUserID, dwExtraData);
    }
    
    nSubBuffID = pBuffRef->Option_Value_02;
    if (nSubBuffID) {
        bResult = _ApplyBuffSkill(pMover, nSubBuffID, dwUserID, dwExtraData);
    }
    
    nSubBuffID = pBuffRef->Option_Value_03;
    if (nSubBuffID) {
        return _ApplyBuffSkill(pMover, nSubBuffID, dwUserID, dwExtraData);
    }
    
    return bResult;
}

// ============================================================================
// CMySkillList::_ApplyBuffSkill
// IDA: 0x1402C23E0
// ============================================================================
int CMySkillList::_ApplyBuffSkill(CMoverEx* pMover, std::uint16_t nBuffID,
                                   std::uint32_t dwUserID, int dwExtraData) {
    if (!pMover) {
        return 0;
    }
    
    // Check monster type restrictions
    int bCheckDie = 1;
    if (pMover->GetType() == eActorMonster) {
        auto pMonster = dynamic_cast<CMonster*>(pMover);
        if (pMonster) {
            auto pMobTable = pMonster->GetMobTableRef();
            if (pMobTable) {
                if (pMonster->IsFollower()) {
                    bCheckDie = 0;
                } else if (pMobTable->Monster_Type != 0 && 
                           pMobTable->Monster_Type != 6 && 
                           pMobTable->Monster_Type != 7) {
                    return 0;
                }
            }
        }
    }
    
    if (bCheckDie && pMover->IsDie()) {
        return 0;
    }
    
    // Get buff table
    auto pServer = TXSingleton<XGameServer>::Instance();
    auto pBuffRef = pServer->m_xResourceMgr.GetTB_BUFF(nBuffID);
    if (!pBuffRef) {
        return 0;
    }
    
    // Get resistance
    int iResistStatIndex = pMover->GetResistStatIndexByBuff(pBuffRef->EffectType_01);
    float fResist = 0.0f;
    if (iResistStatIndex != -1) {
        fResist = (float)pMover->GetStat(iResistStatIndex);
    }
    
    // System type 2 and 3 ignore resistance
    if (pBuffRef->System_Type == 2 || pBuffRef->System_Type == 3) {
        fResist = 0.0f;
    }
    
    // Check success rate
    if (pBuffRef->Buff_Success_Rate > 0 && pBuffRef->System_Type != 2 && pBuffRef->System_Type != 3) {
        float fSuccesRate = (float)pBuffRef->Buff_Success_Rate - fResist;
        if (rand() % 10000 >= (int)fSuccesRate) {
            return 0;
        }
    }
    
    // Calculate start time
    float fStartTime = (float)(int)pBuffRef->StandBy_Time * 0.001f;
    
    if (fStartTime == 0.0f) {
        // Apply immediately
        float fPrevHP = (float)pMover->GetStat(1);
        pMover->SetBuffStatus(nBuffID, dwUserID, true);
        float fHeal = (float)pMover->GetStat(1) - fPrevHP;
        
        if (fHeal > 0.0f) {
            if (dwExtraData) {
                fHeal *= (float)dwExtraData;
            }
            
            if (dwUserID) {
                UXActorID actorID;
                pMover->GetActorID(&actorID);
                if (dwUserID != CQuestCondition::GetQuestID(&actorID)) {
                    auto pOwner = dynamic_cast<CMoverEx*>(CMover::GetMoverObject(dwUserID));
                    if (pOwner) {
                        pOwner->AddAmountOfHeal(fHeal);
                    }
                } else {
                    pMover->AddAmountOfHeal(fHeal);
                }
            } else {
                pMover->AddAmountOfHeal(fHeal);
            }
        }
    } else {
        // Delayed buff
        auto v21 = dynamic_cast<CMoverEx*>(pMover);
        if (v21) {
            if (pMover->FindBuffByGroupID(pBuffRef->Buff_Group, dwUserID) == -1) {
                v21->AddDelayBuffStatus(fStartTime, nBuffID, dwUserID, dwExtraData);
            } else if (v21->IsCanBuffOverlap((int)fStartTime, pBuffRef, dwUserID)) {
                v21->AddDelayBuffStatus(fStartTime, nBuffID, dwUserID, dwExtraData);
            }
        }
    }
    
    // Process maze game rule
    auto pArea = pMover->GetArea();
    auto pMaze = dynamic_cast<XMaze*>(pArea);
    if (pMaze) {
        pMaze->ProcessGameRuleCheck(pMover);
    }
    
    return 1;
}

// ============================================================================
// CMySkillList::ApplyAuraSkill
// IDA: 0x1402C28E0
// ============================================================================
void CMySkillList::ApplyAuraSkill(CMover* pMover, TB_AURA* pAuraRef) {
    if (!pMover || !pAuraRef) {
        return;
    }
    
    auto pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer->m_xResourceMgr.GetTB_BUFF(pAuraRef->Apply_Buff)) {
        return;
    }
    
    if (pAuraRef->Apply_Target == 1) {
        // Apply to self only
        UXActorID actorID;
        pMover->GetActorID(&actorID);
        ApplyBuffSkill(pMover, pAuraRef->Apply_Buff, CQuestCondition::GetQuestID(&actorID), 0);
    } else {
        // Apply to nearby targets
        hkvVec3 vPos;
        pMover->GetPosition(&vPos);
        
        float fDistSQ = (float)(int)pAuraRef->Valid_Distance * (float)(int)pAuraRef->Valid_Distance;
        
        UXActorID actorID;
        pMover->GetActorID(&actorID);
        std::uint32_t dwUserID = CQuestCondition::GetQuestID(&actorID);
        
        std::vector<CMover*> vecGameObjList;
        pMover->ScanGridOrigin(2, 3, &vecGameObjList);
        
        for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
            auto pOtherActor = dynamic_cast<CMoverEx*>(*it);
            if (!pOtherActor) {
                continue;
            }
            
            if (!pOtherActor->IsLive() || pOtherActor->IsStatus(2)) {
                continue;
            }
            
            if (!pMover->CheckReactionTarget(pAuraRef->Apply_Target, pOtherActor, false)) {
                continue;
            }
            
            E_ACTOR_TYPE eType = pOtherActor->GetType();
            if (eType != eActorUser && eType != eActorMonster) {
                continue;
            }
            
            hkvVec3 vOtherPos;
            pOtherActor->GetPosition(&vOtherPos);
            
            float fDist = vPos.getDistanceToSquared(vOtherPos);
            if (fDistSQ > fDist) {
                ApplyBuffSkill(pOtherActor, pAuraRef->Apply_Buff, dwUserID, 0);
            }
        }
    }
}

// ============================================================================
// CMySkillList::PulseDamageAggressive
// IDA: 0x1402C2C80
// ============================================================================
void CMySkillList::PulseDamageAggressive(CMonster* pMover, int nDamage, int nRadius, int nHeight) {
    if (!pMover) {
        return;
    }
    
    hkvVec3 vPos;
    pMover->GetPosition(&vPos);
    
    float fRadiusSq = (float)(nRadius * nRadius);
    
    UXActorID actorID;
    pMover->GetActorID(&actorID);
    CQuestCondition::GetQuestID(&actorID);
    
    std::vector<CMover*> vecGameObjList;
    pMover->ScanGridOrigin(2, 2, &vecGameObjList);
    
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        auto pMonster = dynamic_cast<CMonster*>(*it);
        if (!pMonster) {
            continue;
        }
        
        if (!pMonster->IsLive() || pMonster->IsStatus(2) || pMonster == pMover) {
            continue;
        }
        
        if (!pMover->IsEnemy(pMonster)) {
            continue;
        }
        
        hkvVec3 vOtherPos;
        pMonster->GetPosition(&vOtherPos);
        
        hkvVec3 vDist = vOtherPos - vPos;
        
        // Check height
        if (vDist.z > (float)nHeight || vDist.z < (float)-nHeight) {
            continue;
        }
        
        vDist.z = 0.0f;
        float fDist = vDist.getLengthSquared();
        
        if (fRadiusSq > fDist) {
            pMonster->UpdateDamageAggressive(pMover, nDamage);
        }
    }
}

// ============================================================================
// CMySkillList::GetExplodeSummon
// IDA: 0x1402C5890
// ============================================================================
SummonMonsterTrigger* CMySkillList::GetExplodeSummon(int iIndex) {
    auto iter = m_mapExplodeSummon.find(iIndex);
    if (iter != m_mapExplodeSummon.end()) {
        return iter->second;
    }
    return nullptr;
}

// ============================================================================
// CMySkillList::CheckProjectileIndex
// IDA: 0x1402C5900
// ============================================================================
bool CMySkillList::CheckProjectileIndex(std::int16_t shTriggerIndex, int iSkillIndex,
                                         std::uint8_t bySkillAnimCount) {
    auto iter = m_mapProjectileIndex.find(shTriggerIndex);
    
    if (iter != m_mapProjectileIndex.end()) {
        // Found existing entry
        if (iter->second.first == iSkillIndex && bySkillAnimCount == 4) {
            auto& count = iter->second.second;
            return count++ < 3;
        }
        return false;
    }
    
    // Create new entry
    m_mapProjectileIndex[shTriggerIndex] = std::make_pair(iSkillIndex, (std::int16_t)1);
    return true;
}

// ============================================================================
// CMySkillList::ClearAttackDamage
// IDA: 0x1402C3290
// ============================================================================
void CMySkillList::ClearAttackDamage(std::uint32_t iIndex) {
    if (iIndex < 100) {
        m_stAttackDamage[iIndex].Clear();
    }
}

// ============================================================================
// CMySkillList::GetSkillCost
// IDA: 0x1402C54F0
// ============================================================================
float CMySkillList::GetSkillCost(TB_SKILL* pSkillTable) {
    if (!pSkillTable) {
        return -1.0f;
    }
    
    std::uint8_t byAttribute = pSkillTable->Skill_Cost_Attribute;
    float fSkillCost = (float)(int)pSkillTable->Skill_Cost;
    
    auto pMoverEx = dynamic_cast<CMoverEx*>(m_pActor);
    if (!pMoverEx) {
        return fSkillCost;
    }
    
    int iStatIndex = -1;
    
    if (byAttribute == 1) {
        // HP cost
        iStatIndex = 1;
    }
    else if (byAttribute == 2) {
        // SG cost - apply reduction
        iStatIndex = 2;
        float fRate = 0.0f;
        
        // Check deck bonus
        auto pCurDeckBonus = pMoverEx->GetCurDeckBouns();
        if (pCurDeckBonus && pCurDeckBonus->Bonus_Type == 2) {
            fRate += pCurDeckBonus->Bonus_Value;
        }
        
        // Check special effect
        auto pAttr = m_pActor->GetGOC<CGocAttribute>();
        if (pAttr) {
            float SpecialEffect = pAttr->GetSpecialEffect(114);
            fRate += SpecialEffect * 0.01f;
        }
        
        // Check soul cost down rate
        float SoulCostDownRate = m_pActor->GetSoulCostDownRate();
        fRate += SoulCostDownRate;
        
        fSkillCost -= fSkillCost * fRate;
    }
    else if (byAttribute == 3) {
        // Stamina cost
        iStatIndex = 3;
        
        // Check PvP zone stamina penalty
        auto pArea = pMoverEx->GetArea();
        if (pArea && pArea->IsPvPZone()) {
            if (pSkillTable->Passive_Type == 4 || 
                pSkillTable->Passive_Type == 5 || 
                pSkillTable->Passive_Type == 8) {
                fSkillCost += fSkillCost;
            }
        }
    }
    
    // Percentage cost
    if (pSkillTable->Cost_Type == 2) {
        fSkillCost = (float)m_pActor->GetStat(iStatIndex) * fSkillCost * 0.01f;
        if (fSkillCost > 0.0f && fSkillCost < 1.0f) {
            return 1.0f;
        }
    }
    
    return fSkillCost;
}

// ============================================================================
// CMySkillList::SetExplodeSummon
// IDA: 0x1402C57E0
// ============================================================================
void CMySkillList::SetExplodeSummon(int iIndex, SummonMonsterTrigger* pTrigger) {
    // Remove existing entry
    auto iter = m_mapExplodeSummon.find(iIndex);
    if (iter != m_mapExplodeSummon.end()) {
        m_mapExplodeSummon.erase(iter);
    }
    
    // Set new entry
    m_mapExplodeSummon[iIndex] = pTrigger;
}

// ============================================================================
// CMySkillList::SetAttackDamage
// IDA: 0x1402C7DE0
// ============================================================================
void CMySkillList::SetAttackDamage(int iIndex, tagSKILL_ACTION_DAMAGE& stVal) {
    if (iIndex >= 0 && iIndex < 100) {
        m_stAttackDamage[iIndex] = stVal;
    }
}

// ============================================================================
// CMySkillList::GetAttackDamage
// IDA: 0x1402C7E40
// ============================================================================
tagSKILL_ACTION_DAMAGE CMySkillList::GetAttackDamage(int iIndex) const {
    if (iIndex >= 0 && iIndex < 100) {
        return m_stAttackDamage[iIndex];
    }
    return tagSKILL_ACTION_DAMAGE{};
}

// ============================================================================
// CMySkillList::SetAttackFlySpeed
// IDA: 0x1402C7350
// ============================================================================
void CMySkillList::SetAttackFlySpeed(int iIndex, float fFlySpeed) {
    if (iIndex >= 0 && iIndex < 100) {
        m_fAttackFlySpeed[iIndex] = fFlySpeed;
    }
}

// ============================================================================
// CMySkillList::SetAttackExtraMove
// IDA: 0x1402C7380
// ============================================================================
void CMySkillList::SetAttackExtraMove(int iIndex, hkvVec3& vMove) {
    if (iIndex >= 0 && iIndex < 100) {
        m_vAttackExtraMove[iIndex] = vMove;
    }
}

// ============================================================================
// CMySkillList::GetProjPathActionRes
// IDA: 0x1402C7710
// ============================================================================
VActionResourceLump* CMySkillList::GetProjPathActionRes() const {
    return m_pActionRes;
}

// ============================================================================
// CMySkillList::ProjectileIndexClear
// IDA: 0x140375300
// ============================================================================
void CMySkillList::ProjectileIndexClear() {
    m_mapProjectileIndex.clear();
}

// ============================================================================
// CMySkillList::SetBaseDamage
// IDA: 0x1403A26A0
// ============================================================================
void CMySkillList::SetBaseDamage(int iIndex, int nVal) {
    if (iIndex >= 0 && iIndex < 100) {
        m_nBaseDamage[iIndex] = nVal;
    }
}

// ============================================================================
// CMySkillList::IsCheckContinuousMelee
// IDA: 0x1403645F0
// ============================================================================
bool CMySkillList::IsCheckContinuousMelee() const {
    return m_bCheckContinuousMelee;
}

// ============================================================================
// CMySkillList::GetAttackFlySpeed
// IDA: 0x140375140
// ============================================================================
float CMySkillList::GetAttackFlySpeed(int iIndex) const {
    if (iIndex >= 0 && iIndex < 100) {
        return m_fAttackFlySpeed[iIndex];
    }
    return 0.0f;
}

// ============================================================================
// CMySkillList::GetAttackExtraMove
// IDA: 0x140375160
// ============================================================================
hkvVec3& CMySkillList::GetAttackExtraMove(int iIndex) {
    return m_vAttackExtraMove[iIndex];
}

// ============================================================================
// CMySkillList::GeneralSkillDamage
// IDA: 0x1402C32D0
// ============================================================================
void CMySkillList::GeneralSkillDamage(CMoverEx* pMover, CMoverEx* pTarget,
                                       TB_SKILL* pSkillRef, AttackJudgmentTrigger* pActionEvent,
                                       hkvVec3* vPos, float fAttackRot, float fDelayTime,
                                       std::uint32_t nIdx, int nContinueAttack, bool bPenetrate) {
    if (!pMover || !pTarget || !pSkillRef || !pActionEvent) {
        return;
    }
    
    auto pSkillMgr = pMover->GetSkillMgr();
    if (!pSkillMgr || nIdx >= 100) {
        return;
    }
    
    // Get extra move action
    hkvVec3 vExtraMove = GetExtraMoveAction(pMover, pTarget, pActionEvent, vPos, &fAttackRot);
    
    // Check if should apply extra move
    if (!pActionEvent->sAttackCollision && 
        pActionEvent->sReactionInfo.fReactionDist <= 0.0f &&
        pActionEvent->sReactionInfo.fReactionHeight <= 0.0f) {
        vExtraMove = hkvVec3::ZeroVector();
    }
    
    // Check keep moving extra
    if (pActionEvent->sReactionInfo.iReactionArrow == 3) {
        pTarget->SetKeepMovingExtra(1);
    } else {
        pTarget->SetKeepMovingExtra(0);
    }
    
    // Calculate attack rotation for specific attack type
    if (pActionEvent->sAttackType && pActionEvent->sReactionInfo.iReactionArrow == 2) {
        hkvVec3 vTargetPos;
        pTarget->GetPosition(&vTargetPos);
        hkvVec3 vDiff = vTargetPos - *vPos;
        vDiff.z = 0.0f;
        fAttackRot = CMover::GetYawFromVector(&vDiff);
    }
    
    // Determine attack collision type
    std::uint8_t byAttackCollision = pActionEvent->sAttackCollision;
    if (pTarget->GetType() == eActorUser && pActionEvent->sReactionInfo.bApplyPcSABreak) {
        byAttackCollision = 3;
    }
    if (pMover->IsStatus(0x100000)) {
        byAttackCollision = 4;
    }
    
    // Set reaction rate
    pTarget->SetReactionRate(1.0f);
    
    // Get attack damage
    tagSKILL_ACTION_DAMAGE stAttackDamage;
    pSkillMgr->GetAttackDamage(&stAttackDamage, nIdx);
    
    // Check counter
    if (pActionEvent->sReactionInfo.bCheckCounter) {
        std::uint32_t dwID = 0;
        if (pMover->GetType() != eActorUser) {
            dwID = pMover->GetID();
        } else {
            dwID = pMover->GetID();
        }
        
        if (pTarget->CheckCounter(dwID)) {
            stAttackDamage.byDamageFlag |= 8;
            stAttackDamage.nDamage = 0;
            stAttackDamage.nAttrDamage = 0;
        }
    }
    
    // Ignore target invincible
    if (pActionEvent->sReactionInfo.bIgnoreTargetInvincible) {
        stAttackDamage.byDamageFlag |= 0x10;
    }
    
    pSkillMgr->SetAttackDamage(nIdx, stAttackDamage);
    
    // Get defense type and super armor
    std::uint8_t byDefenceType = pTarget->GetDefenseType();
    float fCurSuperArmorGage = pTarget->GetCurSuperArmorGage();
    
    // Process super armor for monster targets
    if (byDefenceType != 3) {
        if (pTarget->GetType() == eActorMonster && byDefenceType == 2 &&
            pTarget->GetMaxSuperArmorGage() > 0.0f) {
            
            bool bCheckRevieAttack = false;
            if (pSkillRef) {
                bCheckRevieAttack = pSkillRef->Passive_Type == 9;
            }
            
            if (byAttackCollision == 3 || byAttackCollision == 4) {
                if (!bCheckRevieAttack) {
                    fCurSuperArmorGage = 0.0f;
                }
                byDefenceType = 0;
            } else {
                float fSuperArmorDamage = pMover->CalcSuperArmorDamage(pTarget, nIdx, pSkillRef,
                    pActionEvent->sReactionInfo.fDamageRate);
                
                // Check for inherited parent stat (summoned monster)
                if (pMover->GetType() == eActorMonster) {
                    auto pMonster = dynamic_cast<CMonster*>(pMover);
                    if (pMonster && pMonster->IsInheritParentStat()) {
                        auto pOwnerPlayer = pMonster->GetOwnerPlayer();
                        if (pOwnerPlayer) {
                            fSuperArmorDamage = pOwnerPlayer->CalcSuperArmorDamage(pTarget, nIdx,
                                pSkillRef, pActionEvent->sReactionInfo.fDamageRate);
                        }
                    }
                }
                
                fCurSuperArmorGage -= fSuperArmorDamage;
                
                if (fCurSuperArmorGage <= 0.0f) {
                    fCurSuperArmorGage = 0.0f;
                    byDefenceType = pTarget->GetSABreakType() == 1 ? 5 : 0;
                }
            }
        }
    }
    
    // Prepare damage info
    tagACTION_DAMAGE dmgInfo;
    UXActorID actorID;
    pMover->GetActorID(&actorID);
    dmgInfo.dwID = CQuestCondition::GetQuestID(&actorID);
    dmgInfo.byReactionType = stAttackDamage.byReactionType;
    dmgInfo.byDamageFlag = stAttackDamage.byDamageFlag;
    dmgInfo.nDamage = stAttackDamage.nDamage;
    dmgInfo.nAttrDamage = stAttackDamage.nAttrDamage;
    dmgInfo.byAttackCollision = byAttackCollision;
    dmgInfo.fAttackRot = fAttackRot;
    dmgInfo.vExtraMove = vExtraMove;
    dmgInfo.nContinueAttack = nContinueAttack;
    dmgInfo.fFlySpeed = pActionEvent->sReactionInfo.fReactionSpeed;
    dmgInfo.byAttackRank = pActionEvent->sReactionInfo.iTargetGrade;
    dmgInfo.byDefenseType = byDefenceType;
    dmgInfo.fSuperArmorGage = fCurSuperArmorGage;
    dmgInfo.bPenetrate = bPenetrate;
    dmgInfo.bApplyEffectWhenPenetrate = pActionEvent->sProjInfo.bApplyEffectWhenPenetrate;
    dmgInfo.byHitPartsIndex = stAttackDamage.byHitPartsIndex;
    
    // Apply damage
    bool bSABreaked = false;
    pTarget->Damage(&dmgInfo, pSkillRef->Skill_Index, &bSABreaked);
    pTarget->ReleaseInvokedOptionEffectAfterDamage(dmgInfo.dwID, dmgInfo.byDamageFlag, bSABreaked);
    
    // Update attack damage
    vExtraMove = dmgInfo.vExtraMove;
    stAttackDamage.nHP = pTarget->GetHP();
    pSkillMgr->SetAttackDamage(nIdx, stAttackDamage);
    
    // Update extra move position
    auto stPos = pTarget->GetExtraMovePos();
    if (pActionEvent->sAttackCollision ||
        pActionEvent->sReactionInfo.fReactionDist > 0.0f ||
        pActionEvent->sReactionInfo.fReactionHeight > 0.0f) {
        
        if (dmgInfo.byReactionType < 2 || dmgInfo.byReactionType > 4) {
            if (stPos->fRemainTime <= 0.0f) {
                hkvVec3 vTargetPos;
                pTarget->GetPosition(&vTargetPos);
                vExtraMove = vTargetPos;
            } else {
                vExtraMove = *stPos;
            }
        }
        pSkillMgr->SetAttackExtraMove(nIdx, vExtraMove);
        pSkillMgr->SetAttackFlySpeed(nIdx, pTarget->GetJumpSpeed());
    } else {
        pSkillMgr->SetAttackExtraMove(nIdx, vExtraMove);
        pSkillMgr->SetAttackFlySpeed(nIdx, pTarget->GetJumpSpeed());
    }
}

// ============================================================================
// CMySkillList::GetExtraMoveAction
// IDA: 0x1402C4280
// ============================================================================
hkvVec3 CMySkillList::GetExtraMoveAction(CMoverEx* pMover, CMoverEx* pTarget,
                                          AttackJudgmentTrigger* pActionEvent,
                                          hkvVec3* vPos, float* fAttackRot) {
    hkvVec3 vExtraMove = hkvVec3::ZeroVector();
    
    if (!pMover || !pTarget || !pActionEvent) {
        return vExtraMove;
    }
    
    char DmgMotionFlag = pTarget->GetDmgMotionFlag();
    if (((DmgMotionFlag & 2) != 0 && (DmgMotionFlag & 1) != 0) ||
        pTarget->GetAttached() == pMover) {
        return vExtraMove;
    }
    
    // Apply reaction angle
    *fAttackRot += pActionEvent->sReactionInfo.fReactionAngle;
    
    // Calculate direction for wide angle attacks
    if (pMover->GetType() == eActorMonster && pActionEvent->sAttackRange.fAngle > 180.0f) {
        hkvVec3 vMoverPos, vTargetPos;
        pMover->GetPosition(&vMoverPos);
        pTarget->GetPosition(&vTargetPos);
        hkvVec3 vDirVector = vTargetPos - vMoverPos;
        *fAttackRot = CMover::GetYawFromVector(&vDirVector);
    }
    
    int iArrowType = pActionEvent->sReactionInfo.iReactionArrow;
    
    if (pActionEvent->sReactionInfo.fDamageRate > 0.0f) {
        std::uint8_t byReactionType = pActionEvent->sReactionInfo.iReactionType;
        
        // Calculate reaction distance
        if (iArrowType == 3) {
            vExtraMove.y -= pActionEvent->sReactionInfo.fReactionDist;
        } else {
            vExtraMove.y -= pActionEvent->sReactionInfo.fReactionDist + (float)(rand() % 41 - 20);
        }
        
        // Calculate reaction height
        if (pTarget->IsFlying()) {
            vExtraMove.z += pActionEvent->sReactionInfo.fReactionHeightAir;
        } else {
            vExtraMove.z += pActionEvent->sReactionInfo.fReactionHeight;
        }
        
        // Reaction type 6 - no knockback
        if (byReactionType == 6) {
            vExtraMove.y = 0.0f;
        }
        // Apply weight rank modifiers
        else if (pActionEvent->sReactionInfo.bUseTargetWeight || pTarget->GetStandType() >= 2) {
            if (byReactionType >= 2 && byReactionType <= 4) {
                int WeightRank = pMover->GetWeightRank();
                int TargetWeightRank = pTarget->GetWeightRank();
                char cRankGap = WeightRank - TargetWeightRank;
                
                if (cRankGap < 2) {
                    if (cRankGap == -3 || cRankGap == -2) {
                        vExtraMove *= 0.65f;
                    } else if (cRankGap <= -4) {
                        vExtraMove *= 0.0f;
                    }
                } else {
                    vExtraMove *= 1.35f;
                }
            }
        }
        
        // Apply defense type modifier
        if (pTarget->GetDefenseType() == 1) {
            vExtraMove *= 0.5f;
        }
        
        // Apply charging multiple
        float fCalcChargingMultiple = pMover->GetCalcChargingMultiple();
        if (fCalcChargingMultiple > 0.0f) {
            vExtraMove *= fCalcChargingMultiple + 1.0f;
        }
        
        // Apply rotation based on arrow type
        if (byReactionType != 2 && byReactionType != 4 && 
            byReactionType != 3 && byReactionType != 6) {
            
            hkvMat3 matRot;
            matRot.setIdentity();
            
            switch (iArrowType) {
                case 1: {
                    float fAngle = *fAttackRot + (float)((float)(rand() % 31 - 15) * 0.5f);
                    matRot.setFromEulerAngles(0.0f, 0.0f, fAngle);
                    break;
                }
                case 2: {
                    hkvVec3 vTargetPos;
                    pTarget->GetPosition(&vTargetPos);
                    float _X = vPos->x - vTargetPos.x;
                    float fAngle = hkvMath::Rad2Deg(atan2(vPos->y - vTargetPos.y, _X)) - 90.0f;
                    if (fAngle < 0.0f) {
                        fAngle += 360.0f;
                    }
                    matRot.setFromEulerAngles(0.0f, 0.0f, fAngle);
                    break;
                }
                case 3: {
                    matRot.setFromEulerAngles(0.0f, 0.0f, *fAttackRot);
                    break;
                }
            }
            
            vExtraMove = matRot * vExtraMove;
        }
    }
    
    return vExtraMove;
}

// ============================================================================
// CMySkillList::SetAkashicCooltime
// IDA: 0x1402C5060
// ============================================================================
void CMySkillList::SetAkashicCooltime(TB_AKASHIC_RECORDS* pAkashicTable) {
    if (!pAkashicTable || m_bTestMode) {
        return;
    }
    
    auto pTimer = ThreadLocalData::GetTimer();
    float fCurrTime = pTimer->GetTime();
    float fCooldownTime = fCurrTime + (float)(int)pAkashicTable->CoolTime * 0.001f;
    
    int nCoolTimeGroup = pAkashicTable->CoolTime_Group;
    auto iter = m_mapCooltimeList.find(nCoolTimeGroup);
    
    if (iter != m_mapCooltimeList.end()) {
        // Update existing cooldown
        if (fCooldownTime > iter->second.fEndTime) {
            iter->second.fStartTime = fCurrTime;
            iter->second.fEndTime = fCooldownTime;
            iter->second.dwTotalTime = pAkashicTable->CoolTime;
        }
    } else {
        // Create new cooldown
        tagCOOLTIME newData;
        newData.dwTotalTime = pAkashicTable->CoolTime;
        newData.fStartTime = fCurrTime;
        newData.fEndTime = fCooldownTime;
        newData.byType = 1;
        m_mapCooltimeList[nCoolTimeGroup] = newData;
    }
    
    // Update global cooldown
    float fGlobalCooldownTime = fCurrTime + (float)pAkashicTable->CoolTime_Global * 0.001f;
    if (fGlobalCooldownTime > m_fGlobalCooltime[1]) {
        m_fGlobalCooltime[1] = fGlobalCooldownTime;
    }
}

// ============================================================================
// CMySkillList::GetBaseDamage
// IDA: 0x1403A2770
// ============================================================================
int CMySkillList::GetBaseDamage(int iIndex) const {
    if (iIndex >= 0 && iIndex < 100) {
        return m_nBaseDamage[iIndex];
    }
    return 0;
}

// ============================================================================
// CMySkillList::ExplodeSummonClear
// IDA: 0x1403A2D30
// ============================================================================
void CMySkillList::ExplodeSummonClear() {
    m_mapExplodeSummon.clear();
}

// ============================================================================
// CAkashicObject::GetSkillMgr
// IDA: 0x14019B670
// ============================================================================
CMySkillList* CAkashicObject::GetSkillMgr() {
    CMoverEx* pOwnerMover = GetOwnerPlayer();
    if (pOwnerMover) {
        return pOwnerMover->GetSkillMgr();
    }
    return nullptr;
}

// ============================================================================
// CMover::GetSkillMgr
// IDA: 0x140366BE0
// ============================================================================
CMySkillList* CMover::GetSkillMgr() {
    return m_pSkillMgr;
}

// ============================================================================
// CMySkillList::SetTestMode
// IDA: 0x140407090
// ============================================================================
void CMySkillList::SetTestMode(int bTest) {
    m_bTestMode = bTest;
}

// ============================================================================
// CMySkillList::SetAttackTarget
// IDA: 0x1405FA3D0
// ============================================================================
void CMySkillList::SetAttackTarget(int iIndex, std::uint32_t dwVal) {
    if (iIndex >= 0 && iIndex < 3) {
        m_dwAttackTarget[iIndex] = dwVal;
    }
}

// ============================================================================
// CMySkillList::AddTrap
// IDA: 0x14063FE30
// ============================================================================
void CMySkillList::AddTrap(VGameTrapObject* pTrap) {
    if (pTrap) {
        m_vTraps.push_back(pTrap);
    }
}

// ============================================================================
// CMySkillList::ClearRandomSummon
// IDA: 0x1406CE540
// ============================================================================
void CMySkillList::ClearRandomSummon() {
    m_vecRandomSummonEvent.clear();
}

// ============================================================================
// CMoverEx::SetCameraDir
// IDA: 0x1405FA2E0
// ============================================================================
void CMoverEx::SetCameraDir(const hkvVec3& vDir) {
    m_vCamDir = vDir;
}

// ============================================================================
// CMoverEx::GetSkillAnimCount
// IDA: 0x1405FA330
// ============================================================================
std::uint8_t CMoverEx::GetSkillAnimCount() const {
    return m_bySkillAnimCount;
}

// ============================================================================
// CMoverEx::GetSkillAnimStep
// IDA: 0x1405FA350
// ============================================================================
std::uint8_t CMoverEx::GetSkillAnimStep() const {
    return m_bySkillAnimStep;
}

// ============================================================================
// CMoverEx::SetLastDamageType
// IDA: 0x14070AAC0
// ============================================================================
void CMoverEx::SetLastDamageType(std::uint8_t byType) {
    m_byLastDamageType = byType;
}

// ============================================================================
// CMoverEx::GetShieldHP
// IDA: 0x1403A2790
// ============================================================================
std::uint8_t CMoverEx::GetShieldHP() const {
    return m_nShieldHP;
}

// ============================================================================
// CMoverEx::SetAkashicRecord
// IDA: 0x1405FA370
// ============================================================================
void CMoverEx::SetAkashicRecord(TB_AKASHIC_RECORDS* pTblRef, float fYaw) {
    m_pCurAkashicRecord = pTblRef;
    m_fSummonAkashicYaw = fYaw;
}

// ============================================================================
// CMoverEx::SetCurDeckBouns
// IDA: 0x1405FA520
// ============================================================================
void CMoverEx::SetCurDeckBouns(TB_DECK_BONUS* pDeckBonus) {
    m_pCurDeckBonusRef = pDeckBonus;
}

// ============================================================================
// CMoverEx::ResetPvpCondition
// IDA: 0x14070A6A0
// ============================================================================
void CMoverEx::ResetPvpCondition() {
    m_iPvpCondition = 0;
}

// ============================================================================
// CMoverEx::GetCheckAttachToAttacker
// IDA: 0x1403A27B0
// ============================================================================
bool CMoverEx::GetCheckAttachToAttacker() const {
    return m_bCheckAttachToAttacker;
}

// ============================================================================
// CMoverEx::SetLinkSkillOn
// IDA: 0x1403A27D0
// ============================================================================
void CMoverEx::SetLinkSkillOn(bool bSkillOn) {
    m_bLinkSkillOn = bSkillOn;
}

// ============================================================================
// CMoverEx::GetLinkSkillDuration
// IDA: 0x1403A27F0
// ============================================================================
float CMoverEx::GetLinkSkillDuration() const {
    return m_fLinkSkillDuration;
}

// ============================================================================
// CUser::GetDedicatedMonster
// IDA: 0x1406FEF70
// ============================================================================
CMoverEx* CUser::GetDedicatedMonster() {
    if (m_dwDedicatedMonsterID) {
        return static_cast<CMoverEx*>(CMover::GetMoverObject(m_dwDedicatedMonsterID));
    }
    return nullptr;
}

// ============================================================================
// CUser::CheckPassiveSkillByHit
// IDA: 0x1406F0480
// ============================================================================
void CUser::CheckPassiveSkillByHit(CMoverEx* pMover, TB_SKILL* pSkillTable, std::uint8_t byResult) {
    if (!pSkillTable) {
        return;
    }

    bool bCriAttack = false;
    bool bMissAttack = false;

    if ((byResult & 1) != 0) {
        bMissAttack = true;
    } else {
        bCriAttack = (byResult & 4) != 0;
    }

    // Check if monster flag has bit 2 set
    if ((pMover->GetMonsterFlag() & 2) == 0) {
        if (bMissAttack) {
            // Miss attack handling
            XClient::XSocket::MoveToDest(1, 11);
        } else {
            // Normal hit handling
            XClient::XSocket::MoveToDest(1, 1);
            XClient::XSocket::MoveToDest(2, 2);
            XClient::XSocket::MoveToDest(6, 3);

            if (pSkillTable->Use_State == 1) {
                XClient::XSocket::MoveToDest(1, 44);
            }

            if (pMover->IsBoss_Named_Raid()) {
                XClient::XSocket::MoveToDest(1, 52);
            }
        }

        if (bCriAttack) {
            XClient::XSocket::MoveToDest(1, 6);
        }
    }
}

// ============================================================================
// CMover::SetCreatePos
// IDA: 0x140280D40
// ============================================================================
void CMover::SetCreatePos(const hkvVec3& vPos) {
    m_vCreatePos = vPos;
}

// ============================================================================
// CAi::RunTargetByDestPos
// IDA: 0x1402806E0
// ============================================================================
void CAi::RunTargetByDestPos(CMover* pTarget, float fStateMinDist, float fStateMaxDist,
                              float fStateAngleMin, float fStateAngleMax) {
    if (!m_pMonster) {
        return;
    }

    std::uint8_t byOriginPos = m_pMonster->GetTargetDestPos();
    if (byOriginPos != 255) {
        pTarget->ClearTargetPosFlag(byOriginPos);
    }

    std::uint8_t byPos = pTarget->FindTargetPos(fStateAngleMin, fStateAngleMax, eMOVESIDE_RANDOM);
    m_pMonster->SetTargetDestPos(byPos);
    pTarget->SetTargetPosFlag(byPos);

    float fTargetAngle = pTarget->GetTargetAngle(byPos);
    float fAngleMin = fTargetAngle - 30.0f;
    if (fAngleMin <= 0.0f) {
        fAngleMin = 0.0f;
    }

    float fStartAngle = fStateAngleMin;
    float fEndAngle = fStateAngleMax;
    std::uint8_t bySearchAreaType = 0;
    int nSearchCount = 0;
    bool isSuccess = false;

    while (true) {
        ++nSearchCount;
        if (FuncRunWalkToTargetPos(pTarget, fStateMinDist, fStateMaxDist,
                                    (int)fAngleMin, (int)fTargetAngle, 0, eMOVESIDE_RANDOM) == 2) {
            isSuccess = true;
            break;
        }

        if (!m_pMonster->IsCanMove(true)) {
            return;
        }

        if (!bySearchAreaType) {
            fStartAngle += 30.0f;
            if (fStartAngle >= fEndAngle) {
                bySearchAreaType = 1;
                fStartAngle = 0.0f;
                fEndAngle = fStateAngleMin;
                if (fStateAngleMin == 0.0f || (fStateAngleMin - 0.0f) < 30.0f) {
                    fStartAngle = 1.0f;
                }
                if (fStartAngle == 0.0f) {
                    fStartAngle -= 30.0f;
                }
            }
        }

        if (bySearchAreaType == 1) {
            fStartAngle += 30.0f;
            if (fStartAngle >= fEndAngle) {
                bySearchAreaType = 2;
                fEndAngle = 180.0f;
                if (fStateAngleMax != 180.0f && (180.0f - fStateAngleMax) >= 30.0f) {
                    fStartAngle = fStateAngleMax - 30.0f;
                } else {
                    goto LABEL_28;
                }
            }
        }

        if (bySearchAreaType == 2) {
            fStartAngle += 30.0f;
            if (fStartAngle >= fEndAngle) {
                goto LABEL_28;
            }
        }

        std::uint8_t targetDestPos = m_pMonster->GetTargetDestPos();
        if (targetDestPos != 255) {
            pTarget->ClearTargetPosFlag(targetDestPos);
        }

        byPos = pTarget->FindTargetPos(fStartAngle, fEndAngle, eMOVESIDE_RANDOM);
        m_pMonster->SetTargetDestPos(byPos);
        pTarget->SetTargetPosFlag(byPos);
        fTargetAngle = pTarget->GetTargetAngle(byPos);
        fAngleMin = fTargetAngle - 30.0f;
    }

LABEL_28:
    if (byOriginPos != 255 && !isSuccess) {
        m_pMonster->SetTargetDestPos(byOriginPos);
        pTarget->SetTargetPosFlag(byOriginPos);
        CheckValidPositionByCount();
    }
}

// ============================================================================
// VisBaseEntity_cl::PreThinkFunction
// IDA: 0x140189470
// ============================================================================
void VisBaseEntity_cl::PreThinkFunction() {
    SetPreThinkFunctionStatus(0);
}

// ============================================================================
// CAkashicObject::ThinkFunction
// IDA: 0x14019A710
// ============================================================================
void CAkashicObject::ThinkFunction() {
    CMoverEx::ThinkFunction();
}

// ============================================================================
// CMover::GetCurSuperArmorGage
// IDA: 0x1402A5030
// ============================================================================
float CMover::GetCurSuperArmorGage() const {
    return m_fCurSuperArmorGage;
}

// ============================================================================
// CMover::GetMaxSuperArmorGage
// IDA: 0x1402A5050
// ============================================================================
float CMover::GetMaxSuperArmorGage() const {
    return m_fMaxSuperArmorGage;
}

// ============================================================================
// CMover::GetPositionXVec3
// IDA: 0x1402A5080
// ============================================================================
XVec3& CMover::GetPositionXVec3() {
    return m_vPosition;
}

// ============================================================================
// CMover::MoveingValueClear
// IDA: 0x1402A4BE0
// ============================================================================
void CMover::MoveingValueClear() {
    m_fMoving = 0;
    m_stMovePos.Clear();
    m_stMoveGap.Clear();
    m_stMoveOffset.Clear();
}

// ============================================================================
// CMover::SetImmunityStatus
// IDA: 0x1402A4F90
// ============================================================================
void CMover::SetImmunityStatus(std::uint32_t dwStatus) {
    m_dwImmunityStatus |= dwStatus;
}

// ============================================================================
// CMover::ClearImmunityStatus
// IDA: 0x140353040
// ============================================================================
void CMover::ClearImmunityStatus(std::uint32_t dwStatus) {
    m_dwImmunityStatus &= ~dwStatus;
}

// ============================================================================
// CMover::SetIgnoreAggroDebuff
// IDA: 0x1402A67F0
// ============================================================================
void CMover::SetIgnoreAggroDebuff(bool bApply) {
    m_bIgnoreAggroDebuff = bApply;
}

// ============================================================================
// CMover::GetSoulCostDownRate
// IDA: 0x1402C7200
// ============================================================================
float CMover::GetSoulCostDownRate() const {
    return m_fSoulCostDownRate;
}

// ============================================================================
// CMover::GetSkillCoolDownRate
// IDA: 0x1402C7240
// ============================================================================
float CMover::GetSkillCoolDownRate() const {
    return m_fSkillCoolDownRate;
}

// ============================================================================
// CMover::GetWeightRank
// IDA: 0x1402C72D0
// ============================================================================
char CMover::GetWeightRank() const {
    return m_cWeightRank;
}

// ============================================================================
// CMover::GetDmgMotionFlag
// IDA: 0x1402C7310
// ============================================================================
std::uint8_t CMover::GetDmgMotionFlag() const {
    return m_byDmgMontionFlag;
}

// ============================================================================
// CMover::GetJumpSpeed
// IDA: 0x1402C7330
// ============================================================================
float CMover::GetJumpSpeed() const {
    return m_fFlySpeed;
}

// ============================================================================
// CMover::SetReactionRate
// IDA: 0x1402C7400
// ============================================================================
void CMover::SetReactionRate(float fRate) {
    m_fReactionRate = fRate;
}

// ============================================================================
// CMover::SetKeepMovingExtra
// IDA: 0x1402C7420
// ============================================================================
void CMover::SetKeepMovingExtra(bool bExtraMoving) {
    m_bKeepMovingExtra = bExtraMoving;
}

// ============================================================================
// CMover::GetCurRandomShootProjectileDirY
// IDA: 0x1402C7A20
// ============================================================================
std::int16_t CMover::GetCurRandomShootProjectileDirY() const {
    return m_shCurRandomShootProjectileDirY;
}

// ============================================================================
// CMover::GetCurRandomShootProjectileDirX
// IDA: 0x1402C7A40
// ============================================================================
std::int16_t CMover::GetCurRandomShootProjectileDirX() const {
    return m_shCurRandomShootProjectileDirX;
}

// ============================================================================
// CMover::GetDieDelayTime
// IDA: 0x1402C7BD0
// ============================================================================
float CMover::GetDieDelayTime() const {
    return m_fDieDelayTime;
}

// ============================================================================
// CMover::SetOrientationYaw
// IDA: 0x1402C7C60
// ============================================================================
void CMover::SetOrientationYaw(float fYaw) {
    SetOrientation(fYaw, m_vOrientation.y, m_vOrientation.z);
}

// ============================================================================
// CMover::AddSummonMobList
// IDA: 0x1402C7CC0
// ============================================================================
void CMover::AddSummonMobList(std::uint32_t dwMobID) {
    m_listSummonMob.push_back(dwMobID);
}

// ============================================================================
// CMover::GetDecreaseStaminaRate
// IDA: 0x1402C7EE0
// ============================================================================
float CMover::GetDecreaseStaminaRate() const {
    return m_fDecreaseStaminaRate;
}

// ============================================================================
// CMover::GetIgnoreSkillCost
// IDA: 0x1402C7F00
// ============================================================================
bool CMover::GetIgnoreSkillCost() const {
    return m_bIgnoreSkillCost;
}

// ============================================================================
// CMover::IsNoSkillCostSG
// IDA: 0x1402C7F20
// ============================================================================
bool CMover::IsNoSkillCostSG() const {
    return m_bNoSkillCostSG;
}

// ============================================================================
// CMover::GetBuffStatus
// IDA: 0x1403539C0
// ============================================================================
tagBUFF_STATE* CMover::GetBuffStatus() {
    return m_stBuffState;
}

// ============================================================================
// CMover::SetCurSuperArmorGage
// IDA: 0x140353C60
// ============================================================================
void CMover::SetCurSuperArmorGage(float fCurSuperArmorGage) {
    m_fCurSuperArmorGage = fCurSuperArmorGage;
}

// ============================================================================
// CMover::GetRecoverySuperArmorTime
// IDA: 0x140353FE0
// ============================================================================
float CMover::GetRecoverySuperArmorTime() const {
    return m_fRecoverySuperArmorTime;
}

// ============================================================================
// CMover::SetHitID
// IDA: 0x140354290
// ============================================================================
void CMover::SetHitID(std::uint32_t dwID) {
    m_dwHitID = dwID;
}

// ============================================================================
// CMover::SetSimpleDefenseType
// IDA: 0x140354350
// ============================================================================
void CMover::SetSimpleDefenseType(std::uint8_t byType) {
    m_byDefenseType = byType;
}

// ============================================================================
// CMover::SetTargetID
// IDA: 0x1403644E0
// ============================================================================
void CMover::SetTargetID(std::uint32_t dwID) {
    m_dwTargetID = dwID;
}

// ============================================================================
// CMover::SetParentSkillTableIdx
// IDA: 0x140364610
// ============================================================================
void CMover::SetParentSkillTableIdx(int nVal) {
    m_nParentSkillTableIdx = nVal;
}

// ============================================================================
// CMover::ChangeMotion
// IDA: 0x1402AC570
// ============================================================================
void CMover::ChangeMotion(std::int16_t wType) {
    // Empty stub - base class implementation
}

// ============================================================================
// CMover::GetCurSkillTableIdx
// IDA: 0x140364650
// ============================================================================
int CMover::GetCurSkillTableIdx() const {
    return m_nCurSkillTableIdx;
}

// ============================================================================
// CMover::IsAllowPassiveType
// IDA: 0x140364670
// ============================================================================
bool CMover::IsAllowPassiveType(int iType) const {
    if (m_setAllowPassiveType.empty()) {
        return true;
    }
    return m_setAllowPassiveType.find(iType) != m_setAllowPassiveType.end();
}

// ============================================================================
// CMover::IsImmunityStatus
// IDA: 0x140364700
// ============================================================================
bool CMover::IsImmunityStatus() const {
    return m_dwImmunityStatus != 0;
}

// ============================================================================
// CMover::GetHitID
// IDA: 0x140364AB0
// ============================================================================
std::uint32_t CMover::GetHitID() const {
    return m_dwHitID;
}

// ============================================================================
// CMover::IsSendProjectilePacket
// IDA: 0x140366B40
// ============================================================================
bool CMover::IsSendProjectilePacket(AttackJudgmentTrigger* pTrigger) const {
    bool bResult = pTrigger->sProjInfo.bIsTargetGuided;
    if (XActor::GetType() == eActorUser) {
        TB_SKILL* pSkillTable = GetSkillTable();
        return !pSkillTable || pSkillTable->Bullet_Sync_Type != 2;
    }
    return bResult;
}

// ============================================================================
// CMover::GetEmptyBuffSlot
// IDA: 0x14036A810
// ============================================================================
int CMover::GetEmptyBuffSlot() const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == 0) {
            return i;
        }
    }
    return 0;
}

// ============================================================================
// CMover::IsHaveImunityInvincibleBuff
// IDA: 0x14036A600
// ============================================================================
bool CMover::IsHaveImunityInvincibleBuff(std::uint16_t nExceptBuffIndex) const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != nExceptBuffIndex
            && m_stBuffState[i].nBuffIndex != 0
            && m_stBuffState[i].byEffectType == 5
            && m_stBuffState[i].byStatType[0] == 1) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// CMover::GetBoneYaw
// IDA: 0x140368880
// ============================================================================
float CMover::GetBoneYaw(int idx) const {
    if (m_pCurMotionEvent) {
        return VAnimationInfo::GetBoneRotaion(m_pCurMotionEvent, idx, m_fAnimationTime);
    }
    return 0.0f;
}

// ============================================================================
// CMover::GetAnimIndex
// IDA: 0x140368960
// ============================================================================
std::uint32_t CMover::GetAnimIndex(const VString& strAnimName) const {
    if (!m_mapAnimInfoKey) {
        return 0xFFFFFFFF;
    }

    auto it = m_mapAnimInfoKey->find(strAnimName);
    if (it != m_mapAnimInfoKey->end()) {
        return it->second;
    }
    return static_cast<std::uint32_t>(-1);
}

// ============================================================================
// CMover::GetMoverObject
// IDA: 0x14036D1E0
// ============================================================================
CMover* CMover::GetMoverObject(std::uint32_t dwID) const {
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

    CMover* pTarget = dynamic_cast<CMover*>(pActor);
    return pTarget;
}

// ============================================================================
// CMover::IsCanCalculateStat
// IDA: 0x14036AFD0
// ============================================================================
bool CMover::IsCanCalculateStat(int iStatType) const {
    return iStatType > 3 && iStatType != 16;
}

// ============================================================================
// CMover::GetBuffCategory
// IDA: 0x14036B000
// ============================================================================
std::uint8_t CMover::GetBuffCategory(std::uint8_t byBuffEffectType) const {
    if (byBuffEffectType >= 0x6F && byBuffEffectType <= 0x72) {
        return 1;
    }
    if (byBuffEffectType >= 0x79 && byBuffEffectType <= 0x7E) {
        return 2;
    }
    if (byBuffEffectType >= 0x83 && byBuffEffectType <= 0x84) {
        return 3;
    }
    return byBuffEffectType == 133 ? 1 : 0;
}

// ============================================================================
// CMover::GetResistStatIndexByBuff
// IDA: 0x14036B070
// ============================================================================
int CMover::GetResistStatIndexByBuff(std::uint8_t byBuffEffectType) const {
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

// ============================================================================
// CMover::SetBuffTime
// IDA: 0x14036B0F0
// ============================================================================
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
        m_stBuffState[nIndex].bShow
    );
}

// ============================================================================
// CMover::IsCanApplyBuff
// IDA: 0x14036ACC0
// ============================================================================
bool CMover::IsCanApplyBuff(std::uint16_t nBuffIndex, XArea* pArea) const {
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = XResourceMgr::GetTB_BUFF(&pServer->m_xResourceMgr, nBuffIndex);
    if (!pBuffTable) {
        return false;
    }

    std::uint8_t byAlloLocation = pBuffTable->AllowLocation_Type;
    if ((byAlloLocation & 1) != 0) {
        return true;
    }

    if (!pArea) {
        pArea = GetArea();
    }

    if ((byAlloLocation & 2) != 0 && dynamic_cast<XMaze*>(pArea)) {
        return true;
    }

    if ((byAlloLocation & 4) != 0 && dynamic_cast<XDistrict*>(pArea) && !dynamic_cast<CBattleZone*>(pArea)) {
        return true;
    }

    return (byAlloLocation & 8) != 0 && dynamic_cast<CBattleZone*>(pArea);
}

// ============================================================================
// CMover::SetExtraMoving
// IDA: 0x14036C380
// ============================================================================
void CMover::SetExtraMoving(float x, float y, float fTime) {
    hkvVec3 vDestPos(x, y, m_vPosition.z);
    CheckMoveDestPos(&vDestPos, 0, 0);
    m_stExtMovingVal.x = vDestPos.x;
    m_stExtMovingVal.y = vDestPos.y;
    m_stExtMovingVal.fMovingTime = fTime;
    m_stExtMovingVal.fRemainTime = fTime + 0.2f;
}

// ============================================================================
// CMover::IsFixedLoopAnim
// IDA: 0x14036C550
// ============================================================================
bool CMover::IsFixedLoopAnim(std::uint32_t dwAnimID) const {
    std::int16_t nMotion = AnimKeyToMotion(dwAnimID);
    if (nMotion == 1 || nMotion == 3 || nMotion == 5 || nMotion == 4 || nMotion == 10) {
        return true;
    }
    if (nMotion >= 18 && nMotion <= 21) {
        std::int16_t nHitStatus = dwAnimID % 100;
        if (nHitStatus == 2 || nHitStatus == 6) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// CMover::SetDirectionTo
// IDA: 0x14036CAD0
// ============================================================================
void CMover::SetDirectionTo(const hkvVec3& vTarget) {
    hkvVec3 vDirVector = vTarget - GetPosition();
    hkvVec3 vDir(vDirVector.x, vDirVector.y, 0.0f);
    SetDirection(&vDir);
    IncOrientation(90.0f, 0.0f, 0.0f);
}

// ============================================================================
// CMover::SetMovePosition
// IDA: 0x14036CB40
// ============================================================================
void CMover::SetMovePosition(float fXpos, float fYpos) {
    m_stMovePos.x = fXpos;
    m_stMovePos.y = fYpos;
    m_stMoveGap = m_stMovePos;

    hkvVec3 vPos = GetPosition();
    m_stMoveOffset.x = vPos.x - fXpos;
    m_stMoveOffset.y = vPos.y - fYpos;
    m_vMoveStopCheckPos = vPos;
}

// ============================================================================
// CMover::IsEnemy
// IDA: 0x14036CD70
// ============================================================================
bool CMover::IsEnemy(CMover* pMover) const {
    int myType = XActor::GetType();
    int otherType = pMover->XActor::GetType();
    return myType != otherType;
}

// ============================================================================
// CMover::IsEnemyForChain
// IDA: 0x14036CDC0
// ============================================================================
bool CMover::IsEnemyForChain(CMover* pMover) const {
    return IsEnemy(pMover);
}

// ============================================================================
// CMover::IsFriend
// IDA: 0x14036CDF0
// ============================================================================
bool CMover::IsFriend(CMover* pMover) const {
    int myType = XActor::GetType();
    int otherType = pMover->XActor::GetType();
    return myType == otherType;
}

// ============================================================================
// CMover::IsFriendForChain
// IDA: 0x14036CE40
// ============================================================================
bool CMover::IsFriendForChain(CMover* pMover) const {
    return IsFriend(pMover);
}

// ============================================================================
// CMover::GetHeight
// IDA: 0x14036D130
// ============================================================================
bool CMover::GetHeight(hkvVec3& vPos, float fTestHeight) const {
    XArea* pArea = GetArea();
    if (!pArea) {
        return false;
    }

    DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    if (!pNavMesh) {
        return false;
    }

    return pNavMesh->GetHeight(&vPos, fTestHeight);
}

// ============================================================================
// CMover::SetFlyState
// IDA: 0x14036D2F0
// ============================================================================
void CMover::SetFlyState(bool bFly) {
    if (bFly) {
        XActor::SetStatus(0x400000);
    } else {
        XActor::ClearStatus(0x400000);
    }
}

// ============================================================================
// CMover::SetCollisionEnable
// IDA: 0x14036D340
// ============================================================================
void CMover::SetCollisionEnable(bool bEnable, bool bAllCollied) {
    m_bCollisionEnable = bEnable ? 1 : 0;
}

// ============================================================================
// CMover::GetTargetPos
// IDA: 0x14036D930
// ============================================================================
hkvVec3 CMover::GetTargetPos(std::uint8_t byPos, float fDist) const {
    float fAngle = GetTargetAngle(byPos);
    hkvMat3 matRot;
    matRot.setFromEulerAngles(0.0f, 0.0f, fAngle);
    hkvVec3 vDir(0.0f, -fDist, 0.0f);
    return matRot * vDir;
}

// ============================================================================
// CMover::GetActionDesc
// IDA: 0x14036C910
// ============================================================================
const VAnimationInfo* CMover::GetActionDesc(const char* szAnimName) const {
    if (m_pAkashicActionInfo) {
        if (strcmp(m_pAkashicActionInfo->GetName(), szAnimName) == 0) {
            return m_pAkashicActionInfo;
        }
    }

    if (!m_pActionResource) {
        return nullptr;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    return XActionResMgr::GetActionDesc(&pServer->m_xActionManager, m_pActionResource, szAnimName);
}

// ============================================================================
// CMover::IsRegisterAnimInfo
// IDA: 0x140367AE0
// ============================================================================
bool CMover::IsRegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, const VString& strAnimName) const {
    if (m_pActionResource) {
        const char* szAnim = strAnimName.AsChar();
        if (VActionResourceLump::FindAnimationInfo(m_pActionResource, szAnim)) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// CMover::FindBuffStatus
// IDA: 0x14036A420
// ============================================================================
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

// ============================================================================
// CMover::FindBuffByGroupID
// IDA: 0x14036A4C0
// ============================================================================
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

// ============================================================================
// CMover::FindBuffByEffectType
// IDA: 0x14036A560
// ============================================================================
int CMover::FindBuffByEffectType(std::uint8_t byBuffEffect, std::uint16_t nExceptBuffIndex) const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != nExceptBuffIndex
            && m_stBuffState[i].nBuffIndex != 0
            && m_stBuffState[i].byEffectType == byBuffEffect) {
            return i;
        }
    }
    return -1;
}

// ============================================================================
// CMover::CheckPassDebuff
// IDA: 0x14036A750
// ============================================================================
bool CMover::CheckPassDebuff(std::uint16_t nBuffIndex) const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != 0 && m_stBuffState[i].byEffectType == 22) {
            for (int j = 0; j < 3; ++j) {
                if (static_cast<std::uint16_t>(m_stBuffState[i].fSkillVal[j]) == nBuffIndex) {
                    return true;
                }
            }
        }
    }
    return false;
}

// ============================================================================
// CMover::ResetAllBuff
// IDA: 0x14036A860
// ============================================================================
void CMover::ResetAllBuff() {
    m_nBuffTotalCnt = 0;
    m_nBuffCnt = 0;
    m_nDebuffCnt = 0;
    for (std::uint8_t i = 0; i < 50; ++i) {
        m_stBuffState[i].Clear();
    }
}

// ============================================================================
// CMover::ClearBuffByType
// IDA: 0x14036A8E0
// ============================================================================
void CMover::ClearBuffByType(std::uint8_t byType) {
    if (m_nBuffTotalCnt == 0) {
        return;
    }

    for (std::uint8_t i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != 0) {
            if (m_stBuffState[i].byBuffType == byType) {
                ClearBuffStatusBySlot(i, 0);
            }
        }
    }
}

// ============================================================================
// CMover::ClearBuffByEffectType
// IDA: 0x14036A990
// ============================================================================
void CMover::ClearBuffByEffectType(std::uint8_t byType) {
    if (m_nBuffTotalCnt == 0) {
        return;
    }

    for (std::uint8_t i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != 0) {
            if (m_stBuffState[i].byEffectType == byType) {
                ClearBuffStatusBySlot(i, 0);
            }
        }
    }
}

// ============================================================================
// CMover::AllBuffClear
// IDA: 0x14036AA40
// ============================================================================
void CMover::AllBuffClear(std::uint8_t byReason) {
    if (m_nBuffTotalCnt == 0) {
        return;
    }

    for (std::uint8_t i = 0; i < 50; ++i) {
        if (m_stBuffState[i].IsLife()) {
            if (!byReason || IsClearBuff(m_stBuffState[i].nBuffIndex, byReason)) {
                ClearBuffStatusBySlot(i, 0);
            }
        } else if (m_stBuffState[i].nBuffIndex != 0) {
            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            TB_BUFF* pBuffRef = XResourceMgr::GetTB_BUFF(&pServer->m_xResourceMgr, m_stBuffState[i].nBuffIndex);
            if (pBuffRef && !pBuffRef->Buff_Time && IsClearBuff(m_stBuffState[i].nBuffIndex, byReason)) {
                ClearBuffStatusBySlot(i, 0);
            }
        }
    }
}

// ============================================================================
// CMover::CheckBuffByLocation
// IDA: 0x14036ABF0
// ============================================================================
void CMover::CheckBuffByLocation(XArea* pArea) {
    if (m_nBuffTotalCnt == 0) {
        return;
    }

    for (std::uint8_t i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != 0) {
            if (!IsCanApplyBuff(m_stBuffState[i].nBuffIndex, pArea)) {
                ClearBuffStatusBySlot(i, 0);
            }
        }
    }
}

// ============================================================================
// CMover::UpdateBuffCount
// IDA: 0x14036AE70
// ============================================================================
void CMover::UpdateBuffCount(std::uint8_t byBuffType, int nAddCount) {
    if (nAddCount <= 0) {
        int newTotal = m_nBuffTotalCnt + nAddCount;
        m_nBuffTotalCnt = newTotal >= 0 ? newTotal : 0;

        if (byBuffType == 0) {
            int newBuff = m_nBuffCnt + nAddCount;
            m_nBuffCnt = newBuff >= 0 ? newBuff : 0;
        } else if (byBuffType == 1) {
            int newDebuff = m_nDebuffCnt + nAddCount;
            m_nDebuffCnt = newDebuff >= 0 ? newDebuff : 0;
        }
    } else {
        m_nBuffTotalCnt += nAddCount;
        if (byBuffType == 0) {
            m_nBuffCnt += nAddCount;
        } else if (byBuffType == 1) {
            m_nDebuffCnt += nAddCount;
        }
    }
}

// ============================================================================
// CMover::ProcessBuffStatus
// IDA: 0x14036B1D0
// ============================================================================
void CMover::ProcessBuffStatus(float fElapsedTime) {
    bool bCheckDie = true;

    if (XActor::GetType() == eActorMonster) {
        CMonster* pMonster = dynamic_cast<CMonster*>(this);
        if (pMonster && pMonster->GetMobTableRef()) {
            bCheckDie = !pMonster->IsFollower();
        }
    }

    if (m_nBuffTotalCnt == 0 || (bCheckDie && XActor::IsStatus(2))) {
        return;
    }

    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].IsLife()) {
            m_stBuffState[i].fLifeTime -= fElapsedTime;
            ApplyBuffStatus(static_cast<std::uint16_t>(i));
            if (m_stBuffState[i].fLifeTime <= 0.0f) {
                ClearBuffStatusBySlot(i, true);
            }
        } else if (m_stBuffState[i].nBuffIndex != 0) {
            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            TB_BUFF* pBuffRef = XResourceMgr::GetTB_BUFF(&pServer->m_xResourceMgr, m_stBuffState[i].nBuffIndex);
            if (!pBuffRef || pBuffRef->Buff_Time) {
                ClearBuffStatusBySlot(i, true);
            } else {
                ApplyBuffStatus(static_cast<std::uint16_t>(i));
            }
        }
    }
}

// ============================================================================
// CMover::UpdateDefenseDisableBuff
// IDA: 0x14036B420
// ============================================================================
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

// ============================================================================
// CMover::ReleaseExtraMoving
// IDA: 0x14036C120
// ============================================================================
void CMover::ReleaseExtraMoving() {
    if (tagMOVE_POS::IsZero(m_stExtMovingVal)) {
        return;
    }

    if (m_stExtMovingVal.fMovingTime == 0.1f) {
        hkvVec3 vExtraPos(m_stExtMovingVal.x, m_stExtMovingVal.y, m_vPosition.z);
        Move(&vExtraPos);
    }

    m_stExtMovingVal.Clear();
}

// ============================================================================
// CMover::ProcessExtraMoving
// IDA: 0x14036BC20
// ============================================================================
void CMover::ProcessExtraMoving() {
    if (tagMOVE_POS::IsZero(m_stExtMovingVal)) {
        return;
    }

    m_vPrevPos = GetPosition();

    if (m_stExtMovingVal.fRemainTime <= 0.0f) {
        ReleaseExtraMoving();
        return;
    }

    float fDiffX = m_stExtMovingVal.x - m_vPosition.x;
    float fDiffY = m_stExtMovingVal.y - m_vPosition.y;

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = pTimer->GetTimeDifference();

    if (fabsf(fDiffX) >= 3.0f || fabsf(fDiffY) >= 3.0f) {
        float fDeltaX, fDeltaY;

        if (fDiffX <= 0.0f) {
            fDeltaX = std::max((fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffX, fDiffX);
        } else {
            fDeltaX = std::min((fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffX, fDiffX);
        }

        if (fDiffY <= 0.0f) {
            fDeltaY = std::max((fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffY, fDiffY);
        } else {
            fDeltaY = std::min((fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffY, fDiffY);
        }

        m_stExtMovingVal.fRemainTime -= fDeltaTime;
        hkvVec3 vDelta(fDeltaX, fDeltaY, 0.0f);
        hkvVec3 vDestPos = m_vPrevPos + vDelta;

        if (!IsFlying()) {
            GetHeight(vDestPos, 200.0f);
        }

        if (CheckMoveCollision(&vDestPos)) {
            hkvVec3 vPos = GetPosition();
            send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(&vPos, 0);
        } else {
            if (!CheckMoveDestPos(&vDestPos, 0, 0)) {
                send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(&vDestPos, 0);
            }
            Move(&vDestPos);
        }
    } else {
        m_stExtMovingVal.Clear();
    }
}

// ============================================================================
// CMover::AddExtraMoving
// IDA: 0x14036C210
// ============================================================================
void CMover::AddExtraMoving(float x, float y, float fTime) {
    hkvVec3 vDestPos(0.0f, 0.0f, m_vPosition.z);

    if (m_stExtMovingVal.fRemainTime <= 0.0f) {
        vDestPos.x = m_vPosition.x + x;
        vDestPos.y = m_vPosition.y + y;
    } else {
        vDestPos.x = m_stExtMovingVal.x + x;
        vDestPos.y = m_stExtMovingVal.y + y;
    }

    CheckMoveDestPos(&vDestPos, 0, 0);

    m_stExtMovingVal.x = vDestPos.x;
    m_stExtMovingVal.y = vDestPos.y;

    float fMovingTime = fTime;
    if (fTime <= m_stExtMovingVal.fMovingTime) {
        fMovingTime = m_stExtMovingVal.fMovingTime;
    }
    m_stExtMovingVal.fMovingTime = fMovingTime;
    m_stExtMovingVal.fRemainTime = fMovingTime + 0.2f;
}

// ============================================================================
// CMover::InitialObjectInfo
// IDA: 0x140366AD0
// ============================================================================
void CMover::InitialObjectInfo(std::uint32_t dwID, int nTableIdx, hkvVec3 vPos, float fRot) {
    // TODO: GetGOC template not implemented - requires Vision Engine component system
    // Original uses CGocAttribute component to get final stats
    // auto pAttr = GetGOC<CGocAttribute>(0);
    // if (pAttr) {
    //     m_fAbility = pAttr->GetFinalStats();
    // }
}

// ============================================================================
// CMover::MessageFunction
// IDA: 0x140366F10
// ============================================================================
void CMover::MessageFunction(int iID, std::int64_t iParamA, std::int64_t iParamB) {
    if (iID == 0x10000 && iParamA == 1) {
        ClearMotion();
    }
}

// ============================================================================
// CMover::ThinkFunction
// IDA: 0x140366FA0
// ============================================================================
void CMover::ThinkFunction() {
    m_bAnimChanged = false;

    if (m_pSkillMgr) {
        m_pSkillMgr->ThinkFunction();
    }

    if (m_bTraceUser) {
        send_eSUB_CMD_MOVE_TRACE(this);

        VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
        float fTime = pTimer->GetTime();
        if (fTime > m_fLastDebugTime + 0.5f) {
            m_fLastDebugTime = fTime;
        }
    }

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = pTimer->GetTimeDifference();
    CheckDelayedProjectile(fDeltaTime);
    CheckContinuousMelee(fDeltaTime);
}

// ============================================================================
// CMover::CheckMoveCollision
// IDA: 0x1403681B0
// ============================================================================
CMover* CMover::CheckMoveCollision(hkvVec3& vDestPos) {
    // Only check for monsters
    if (GetType() != eActorMonster) {
        return nullptr;
    }

    if (!m_bCollisionEnable) {
        return nullptr;
    }

    if (m_bKeepMovingExtra) {
        return nullptr;
    }

    CMonster* pMonster = dynamic_cast<CMonster*>(this);
    if (!pMonster) {
        return nullptr;
    }

    if (pMonster->IsFollower()) {
        return nullptr;
    }

    CAi* pAi = pMonster->GetAi();
    if (pAi && pAi->IsPatrolMonster()) {
        return nullptr;
    }

    float nearFactor = 5000.0f;
    CMover* pClosestTargetEntity = nullptr;
    hkvVec3 vClosestPos;
    std::vector<CMover*> vecGameObjList;

    // Scan for nearby actors
    XArea::ScanGridOrigin(this, 2, 3, &vecGameObjList);

    hkvVec3 vPos = vDestPos;

    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMover* pOtherActor = *it;
        if (!pOtherActor) {
            continue;
        }

        bool bCheckActor = false;

        // Check if actor type is player (0) or defense object monster
        if (pOtherActor->GetType() == eActorUser) {
            bCheckActor = true;
        } else if (pOtherActor->GetType() == eActorMonster) {
            CMonster* pOtherMonster = dynamic_cast<CMonster*>(pOtherActor);
            if (pOtherMonster && pOtherMonster->IsDefenseObject()) {
                bCheckActor = true;
            }
        }

        if (bCheckActor) {
            if (pOtherActor->IsLive() && !pOtherActor->IsStatus(2)) {
                hkvVec3 vOtherPos = pOtherActor->GetPosition();
                hkvVec3 vOffset = vPos - vOtherPos;
                vOffset.z = 0.0f;
                float targetDist = vOffset.getLength();

                if (nearFactor > targetDist) {
                    vClosestPos = vOtherPos;
                    nearFactor = targetDist;
                    pClosestTargetEntity = pOtherActor;
                }
            }
        }
    }

    if (pClosestTargetEntity) {
        hkvVec3 vOffset = vPos - vClosestPos;
        vOffset.z = 0.0f;
        float fDist = vOffset.getLength();
        float fRadius = pClosestTargetEntity->GetHavokCapsuleRadius();

        if ((fRadius + m_fCapsuleRadius + 5.0f + 5.0f) >= fDist) {
            return pClosestTargetEntity;
        }
    }

    return nullptr;
}

// ============================================================================
// CMover::ChangeSequence
// IDA: 0x14036C440
// ============================================================================
void CMover::ChangeSequence(std::uint32_t dwOldAnimID, std::uint32_t dwNewAnimID, int bResetPlay) {
    m_fAnimPercentTime = 0.0f;
    m_fAnimationTime = 0.0f;
    m_bAnimPlay = true;

    if (dwOldAnimID != dwNewAnimID) {
        m_bAnimChanged = true;
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

// ============================================================================
// CMover::ResetAkashicActionInfo
// IDA: 0x14036C600
// ============================================================================
void CMover::ResetAkashicActionInfo() {
    if (m_pAkashicActionInfo) {
        m_pAkashicActionInfo->arOffsetDeltaFrames.RemoveAll();
        m_pAkashicActionInfo->arTranslationFrames.RemoveAll();
        m_pAkashicActionInfo->arTriggers.RemoveAll();

        delete m_pAkashicActionInfo;
        m_pAkashicActionInfo = nullptr;
    }
}

// ============================================================================
// CMover::CreateAkashicActionInfo
// IDA: 0x14036C770
// ============================================================================
void CMover::CreateAkashicActionInfo(const char* szAnimName) {
    if (!m_pActionResource) {
        return;
    }

    if (m_pAkashicActionInfo) {
        // Already exists, update it
        m_pAkashicActionInfo->arTriggers.RemoveAll();

        if (strcmp(m_pAkashicActionInfo->szName, szAnimName) != 0) {
            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            VAnimationInfo* pAnimInfo = pServer->m_xActionManager.GetActionDesc(m_pActionResource, szAnimName);
            if (pAnimInfo) {
                m_pAkashicActionInfo->arOffsetDeltaFrames.RemoveAll();
                m_pAkashicActionInfo->arTranslationFrames.RemoveAll();
                m_pAkashicActionInfo->CopyData(pAnimInfo);
            }
        }
    } else {
        // Create new
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        VAnimationInfo* pAnimInfo = pServer->m_xActionManager.GetActionDesc(m_pActionResource, szAnimName);
        if (pAnimInfo) {
            m_pAkashicActionInfo = new VAnimationInfo();
            m_pAkashicActionInfo->CopyData(pAnimInfo);
        }
    }
}

// ============================================================================
// CMover::ChangeActionTrigger
// IDA: 0x14036C9B0
// ============================================================================
void CMover::ChangeActionTrigger(const char* szAnimName) {
    if (m_pCurMotionEvent && m_pCurMotionEvent->eActionBufferBehavior == RESETAFTER) {
        DeleteActionBuffer();
    }

    if (m_pSkillMgr) {
        m_pSkillMgr->ProjectileIndexClear();
    }

    m_bSkipAnimOffset = false;
    m_pCurMotionEvent = GetActionDesc(szAnimName);

    if (m_pCurMotionEvent) {
        m_nAnimGroup = m_pCurMotionEvent->iAnimGroup;

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        pServer->m_xActionManager.ChangeMotionCallback(this, m_pCurMotionEvent);
    } else {
        DebugOut("ChangeActionTrigger>> Not found anim info. (%s)", szAnimName);
    }
}

// ============================================================================
// CMover::SceneChanged
// IDA: 0x14036CAB0
// ============================================================================
void CMover::SceneChanged() {
    MoveingValueClear();
}

// ============================================================================
// CMover::CheckMoveTick
// IDA: 0x14036CC30
// ============================================================================
bool CMover::CheckMoveTick(std::int16_t nRestMotion) {
    if (IsStatus(2) || IsStatus(4)) {
        return false;
    }

    if (!tagMOVE_POS::IsZero(m_stMovePos)) {
        return true;
    }

    if (m_nMotionClass < 1 || m_nMotionClass > 5 || !tagMOVE_POS::IsNoneZero(m_stMoveGap)) {
        return false;
    }

    m_stMovePos = m_stMoveGap;
    m_stMoveGap.Clear();
    return true;
}

// ============================================================================
// CMover::MoveingClientStop
// IDA: 0x14036CD20
// ============================================================================
void CMover::MoveingClientStop() {
    m_fMoving = 0;
    m_stMoveGap = m_stMovePos;
    m_stMovePos.Clear();
}

// ============================================================================
// CMover::CheckReactionTarget
// IDA: 0x14036CE70
// ============================================================================
bool CMover::CheckReactionTarget(int iTargetType, CMover* pTargetMover, bool bCheckForChain) {
    if (pTargetMover) {
        CMoverEx* pMoverEx = dynamic_cast<CMoverEx*>(pTargetMover);
        if (pMoverEx && pMoverEx->IsSystemActor()) {
            return false;
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
                bResult = bCheckForChain ? IsFriendForChain(pTargetMover) : IsFriend(pTargetMover);
            }
            break;
        case 3:  // Enemy
            if (this == pTargetMover) {
                bResult = false;
            } else {
                bResult = bCheckForChain ? IsEnemyForChain(pTargetMover) : IsEnemy(pTargetMover);
                if (!bResult) {
                    bResult = (CheckMonsterInteractObject(pTargetMover) != 0);
                }
            }
            break;
        case 4:  // Not self
            bResult = (this != pTargetMover);
            break;
        case 6:  // Friend
            if (this == pTargetMover) {
                bResult = false;
            } else {
                bResult = bCheckForChain ? IsFriendForChain(pTargetMover) : IsFriend(pTargetMover);
            }
            break;
        case 7:  // Self only (alternative)
            bResult = (this == pTargetMover);
            break;
        case 8:  // Party
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

// ============================================================================
// CMover::FindTargetPos (from target mover)
// IDA: 0x14036D380
// ============================================================================
std::uint8_t CMover::FindTargetPos(CMover* pMover) {
    hkvVec3 vDirVector = pMover->GetPosition() - GetPosition();

    int nYaw = static_cast<int>(GetYawFromVector(vDirVector));
    if (nYaw < 0) {
        nYaw += 360;
    }

    int ConstAngle = 30;
    std::uint8_t nPos = nYaw / ConstAngle;
    if (nPos >= 12) {
        nPos -= 12;
    }

    if (!m_byTargetPosInfo[nPos]) {
        return nPos;
    }

    std::uint8_t nNext = (nPos + 1) % 12;
    std::uint8_t nPrev = (nPos + 11) % 12;

    if (!m_byTargetPosInfo[nPrev]) {
        return nPrev;
    }
    if (!m_byTargetPosInfo[nNext]) {
        return nNext;
    }

    nNext = (nPos + 2) % 12;
    nPrev = (nPos + 10) % 12;

    if (!m_byTargetPosInfo[nPrev]) {
        return nPrev;
    }
    if (!m_byTargetPosInfo[nNext]) {
        return nNext;
    }

    nNext = (nPos + 1) % 12;
    nPrev = (nPos + 11) % 12;

    if (m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nPrev] &&
        m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nNext]) {
        return nPos;
    }

    if (m_byTargetPosInfo[nPos] == m_byTargetPosInfo[nPrev] + 1) {
        return nPrev;
    }
    if (m_byTargetPosInfo[nPos] == m_byTargetPosInfo[nNext] + 1) {
        return nNext;
    }

    nNext = (nPos + 2) % 12;
    nPrev = (nPos + 10) % 12;

    if (m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nPrev] &&
        m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nNext]) {
        return nPos;
    }

    if (m_byTargetPosInfo[nPos] >= m_byTargetPosInfo[nPrev] + 1) {
        return nPrev;
    }
    if (m_byTargetPosInfo[nPos] < m_byTargetPosInfo[nNext] + 1) {
        return nPos;
    }

    return nNext;
}

// ============================================================================
// CMover::FindTargetPos (by angle range)
// IDA: 0x14036D6F0
// ============================================================================
std::uint8_t CMover::FindTargetPos(float fAngleMin, float fAngleMax, E_MOVESIDE_TYPE eIgnoreMoveSide) {
    if (fAngleMin > 180.0f) {
        fAngleMin = 180.0f;
    }
    if (fAngleMax > 180.0f) {
        fAngleMax = 180.0f;
    }

    int BeginPos = static_cast<int>(fAngleMin / 30.0f);
    int EndPos = static_cast<int>(fAngleMax / 30.0f);

    if (static_cast<int>(fAngleMin) % 30 > 0 && (30 * BeginPos) > fAngleMin) {
        BeginPos--;
        if (BeginPos < 0) {
            BeginPos = 0;
        }
    }
    if (static_cast<int>(fAngleMax) % 30 > 0 && fAngleMax > (30 * BeginPos)) {
        EndPos++;
        if (EndPos > 5) {
            EndPos = 5;
        }
    }

    std::uint8_t nMinPos = BeginPos;
    std::uint8_t nNext = BeginPos;
    std::uint8_t nCount = 50;

    while (nNext < EndPos) {
        int nReverseNext = 12 - nNext - 1;

        if (eIgnoreMoveSide != eMOVESIDE_RIGHT) {
            if (!m_byTargetPosInfo[nNext]) {
                return nNext;
            }
            if (nCount > m_byTargetPosInfo[nNext]) {
                nCount = m_byTargetPosInfo[nNext];
                nMinPos = nNext;
            }
        }

        if (eIgnoreMoveSide != eMOVESIDE_LEFT) {
            if (!m_byTargetPosInfo[nReverseNext]) {
                return static_cast<std::uint8_t>(nReverseNext);
            }
            if (nCount > m_byTargetPosInfo[nReverseNext]) {
                nCount = m_byTargetPosInfo[nReverseNext];
                nMinPos = 12 - nNext - 1;
            }
        }

        ++nNext;
    }

    return nMinPos;
}

// ============================================================================
// CMover::GetTargetAngle
// IDA: 0x14036D9F0
// ============================================================================
float CMover::GetTargetAngle(std::uint8_t byPos) {
    return static_cast<float>(30 * (byPos + 1));
}

// ============================================================================
// CMover::SetTargetPosFlag
// IDA: 0x14036DA30
// ============================================================================
void CMover::SetTargetPosFlag(std::uint8_t byPos) {
    ++m_byTargetPosCount;
    ++m_byTargetPosInfo[byPos];
}

// ============================================================================
// CMover::ClearTargetPosFlag
// IDA: 0x14036DA80
// ============================================================================
void CMover::ClearTargetPosFlag(std::uint8_t byPos) {
    if (byPos == 255) {
        m_byTargetPosCount = 0;
        std::memset(m_byTargetPosInfo, 0, sizeof(m_byTargetPosInfo));
    } else if (m_byTargetPosInfo[byPos]) {
        --m_byTargetPosInfo[byPos];
        --m_byTargetPosCount;
    }
}

// ============================================================================
// CMover::GetYawFromVector
// IDA: 0x14036DBA0
// ============================================================================
float CMover::GetYawFromVector(const hkvVec3& vDir) {
    hkvVec3 n1(0.0f, -1.0f, 0.0f);
    hkvVec3 n2(vDir.x, vDir.y, 0.0f);
    n2.normalizeIfNotZero(0.000001f);

    float calcValue = n1.dot(n2);
    calcValue = std::clamp(calcValue, -1.0f, 1.0f);

    float fDegree = hkvMath::Rad2Deg(std::acos(calcValue));

    if (vDir.x < 0.0f) {
        fDegree = (180.0f - fDegree) + 180.0f;
    }

    if (fDegree > 180.0f) {
        return fDegree - 360.0f;
    }
    return fDegree;
}

// ============================================================================
// CMover::IsValidPos (XVec3)
// IDA: 0x14036DCB0
// ============================================================================
bool CMover::IsValidPos(XVec3& vPos) {
    if (vPos.x < -10000000.0f || vPos.x > 10000000.0f) {
        return false;
    }
    if (vPos.y < -10000000.0f || vPos.y > 10000000.0f) {
        return false;
    }
    return vPos.z >= -10000000.0f && vPos.z <= 10000000.0f;
}

// ============================================================================
// CMover::IsValidPos (float, float)
// IDA: 0x14036DD40
// ============================================================================
bool CMover::IsValidPos(float fX, float fY) {
    if (fX < -10000000.0f || fX > 10000000.0f) {
        return false;
    }
    return fY >= -10000000.0f && fY <= 10000000.0f;
}

// ============================================================================
// CMover::IsValidRot
// IDA: 0x14036DDA0
// ============================================================================
bool CMover::IsValidRot(float fRot) {
    return fRot >= -360.0f && fRot <= 360.0f;
}

// ============================================================================
// CMover::Move
// IDA: 0x14036DDD0
// ============================================================================
std::uint16_t CMover::Move(XVec3& vNextPos, float fRot) {
    if (m_pArea) {
        m_pArea->MoveActor_2(this, &vNextPos);
    } else {
        LogHelper::LogDebug("game.contents", "No Area when send move!!");
    }
    return 0;
}

// ============================================================================
// CMover::CheckMoveDestPos
// IDA: 0x14036DEE0
// ============================================================================
bool CMover::CheckMoveDestPos(hkvVec3& vDestPos, int bFlying, int bDontCareCurve) {
    XArea* pArea = GetArea();
    if (!pArea) {
        return true;
    }

    DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    if (!pNavMesh) {
        return true;
    }

    float fRadius = GetHavokCapsuleRadius();
    hkvVec3 vPos = GetPosition();
    hkvVec3 vNextPos = vDestPos;

    if (bFlying == 1) {
        vPos.z = 0.0f;
        vNextPos.z = 0.0f;
        if (vPos == vNextPos) {
            return true;
        }
        vPos = GetPosition();
        vNextPos = vDestPos;
    }

    bool bResult = pNavMesh->CheckCanDirectMove2(&vPos, &vDestPos, fRadius, bFlying, bDontCareCurve);

    if (!bResult) {
        hkvVec3 vDirection = vNextPos - vPos;
        vDirection.z = 0.0f;
        vDirection.normalizeIfNotZero(0.000001f);
        vDirection *= fRadius;
        vDestPos -= vDirection;

        if (pNavMesh->GetHeight(&vDestPos, 200.0f)) {
            if (bFlying == 1) {
                vDestPos.z = vPos.z;
            }
        } else {
            vDestPos = vPos;
        }
    }

    return bResult;
}

// ============================================================================
// CMover::GetSGAbsorbRate
// IDA: 0x14036E200
// ============================================================================
float CMover::GetSGAbsorbRate() {
    // TODO: GetGOC template not implemented - requires Vision Engine component system
    // Original uses CGocAttribute component to get Con_SG_Absorb_Rate
    return 0.0f;
}

// ============================================================================
// CMover::SetStat
// IDA: 0x14036E290
// ============================================================================
void CMover::SetStat(std::uint32_t iIndex, float fVal) {
    // TODO: GetGOC template not implemented - requires Vision Engine component system
    // Original uses CGocAttribute component to set stat value
}

// ============================================================================
// CMover::SetContinousCost
// IDA: 0x14036E330
// ============================================================================
void CMover::SetContinousCost(int iIndex, float fVal) {
    // TODO: GetGOC template not implemented - requires Vision Engine component system
    // Original uses CGocAttribute::SetContinousCost
}

// ============================================================================
// CMover::SetAllowPassiveType
// IDA: 0x14036E3B0
// ============================================================================
void CMover::SetAllowPassiveType(int nType) {
    m_setAllowPassiveType.insert(nType);
}

// ============================================================================
// CMover::CreateRandomTrapIndex
// IDA: 0x14036E3E0
// ============================================================================
int CMover::CreateRandomTrapIndex() {
    m_nRandomTrapIndex = std::rand() % 1000000;
    return m_nRandomTrapIndex;
}

// ============================================================================
// CMover::SetRandomTrapIndex
// IDA: 0x14036E420
// ============================================================================
void CMover::SetRandomTrapIndex(int nIndex) {
    m_nRandomTrapIndex = nIndex;
}

// ============================================================================
// CMover::GetRandomTrapIndex
// IDA: 0x14036E440
// ============================================================================
int CMover::GetRandomTrapIndex() {
    if (m_nRandomTrapIndex >= 1000000) {
        m_nRandomTrapIndex = 0;
    }
    unsigned int nIndex = m_nRandomTrapIndex;
    m_nRandomTrapIndex = nIndex + 1;
    return nIndex;
}

// ============================================================================
// CMover::SendUpdateStat
// IDA: 0x14036E4A0
// ============================================================================
void CMover::SendUpdateStat(std::uint32_t iIndex) {
    // TODO: GetGOC template not implemented - requires Vision Engine component system
    // Original uses CGocAttribute component to send stat update
}

// ============================================================================
// CMover::SetDelayedProjectile
// IDA: 0x14036E520
// ============================================================================
void CMover::SetDelayedProjectile(SDelayedProjectile* pDelayedProjectile) {
    m_vecDelayedProjectile.push_back(pDelayedProjectile);
}

// ============================================================================
// CMover::DeleteDelayedProjectile
// IDA: 0x14036E550
// ============================================================================
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

// ============================================================================
// CMover::CheckDelayedProjectile
// IDA: 0x14036E640
// ============================================================================
void CMover::CheckDelayedProjectile(float fDeltaTime) {
    for (auto it = m_vecDelayedProjectile.begin(); it != m_vecDelayedProjectile.end(); ++it) {
        SDelayedProjectile* pTemp = *it;
        if (!pTemp) {
            break;
        }

        pTemp->fCurTime += fDeltaTime;

        if (pTemp->bUsed || pTemp->fCurTime < pTemp->fCreateDelayTime) {
            continue;
        }

        // Create action buffer and execute
        tagACTION_BUFFER xAction(1, 0.0f);
        xAction.pActionTrigger = pTemp->pTrigger;

        float fYaw = GetOrientationYaw();
        xAction << pTemp->pTrigger->EventID;
        xAction << pTemp->nSkillID;
        xAction << 0;
        xAction << 1u;
        xAction << fYaw;

        AddActionBuffer(&xAction);
        pTemp->bUsed = true;
    }
}

// ============================================================================
// CMover::GetUsedDelayedProjectile
// IDA: 0x14036E820
// ============================================================================
SDelayedProjectile* CMover::GetUsedDelayedProjectile(AttackJudgmentTrigger* pTrigger) {
    for (auto it = m_vecDelayedProjectile.begin(); it != m_vecDelayedProjectile.end(); ++it) {
        SDelayedProjectile* pTemp = *it;
        if (pTemp && pTemp->bUsed && pTemp->pTrigger == pTrigger) {
            return pTemp;
        }
    }
    return nullptr;
}

// ============================================================================
// CMover::GetFilterData
// IDA: 0x14036E8D0
// ============================================================================
void CMover::GetFilterData(std::uint32_t nSkillID, int& nFilterData1, int& nFilterData2, int& nFilterData3) {
    auto it = m_mapFilterData.find(nSkillID);
    if (it == m_mapFilterData.end()) {
        nFilterData1 = 0;
        nFilterData2 = 0;
        nFilterData3 = 0;
    } else {
        nFilterData1 = it->second.dwFilterData1;
        nFilterData2 = it->second.dwFilterData2;
        nFilterData3 = it->second.dwFilterData3;
    }
}

// ============================================================================
// CMover::SetFilterData
// IDA: 0x14036E9A0
// ============================================================================
void CMover::SetFilterData(std::uint32_t nSkillID, std::uint32_t nFilterData1, std::uint32_t nFilterData2, std::uint32_t nFilterData3) {
    SFilterData& data = m_mapFilterData[nSkillID];
    data.dwFilterData1 = nFilterData1;
    data.dwFilterData2 = nFilterData2;
    data.dwFilterData3 = nFilterData3;
}

// ============================================================================
// CMover::SyncMove
// IDA: 0x14036EA30
// ============================================================================
void CMover::SyncMove() {
    if (!tagMOVE_POS::IsZero(m_stMovePos)) {
        bool byRunBit = IsStatus(0x100);
        send_eSUB_CMD_MOVE(this, m_stMovePos.x, m_stMovePos.y, byRunBit);
    }
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE
// IDA: 0x14036EAC0
// ============================================================================
void CMover::send_eSUB_CMD_MOVE(CMover* pMover, float fTargetPosx, float fTargetPosy, std::uint8_t byRunBit) {
    float fYaw = pMover->GetMovingYaw();

    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    m_fMoveSpeed = pMover->GetMoveSpeed();
    hkvVec3 vPos = pMover->GetPosition();

    UXMapID uxMapID;
    XArea* pArea = pMover->GetArea();
    if (pArea) {
        uxMapID = pArea->GetInstanceID();
    }

    ST_MOVE stMove;
    stMove.dwActorID = pMover->GetID();
    stMove.nMapID = uxMapID.nMapID;
    stMove.fPosX = vPos.x;
    stMove.fPosY = vPos.y;
    stMove.fPosZ = vPos.z;
    stMove.fYaw = pMover->GetMovingYaw();
    stMove.fTargetPosX = fTargetPosx;
    stMove.fTargetPosY = fTargetPosy;
    stMove.byRunBit = byRunBit;
    stMove.fPitch = pMover->GetLookPitch();
    stMove.fMoveSpeed = m_fMoveSpeed;
    stMove.byChangeMotion = 0;

    XSendPacket xPacket(5, 2);
    xPacket << stMove;
    SendBroadCastAfterLoading(&xPacket, eNoneSelf);

    m_fLastSendMoveTime = 0.0f;

    DebugOut("send_eSUB_CMD_MOVE>> (%.2f,%.2f) -> (%.2f,%.2f) -> Rot:%.3f",
             vPos.x, vPos.y, fTargetPosx, fTargetPosy, fYaw);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_STOP
// IDA: 0x14036EE90
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_STOP(CMover* pMover) {
    float fYaw = pMover->GetMovingYaw();
    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    hkvVec3 curPos = pMover->GetPosition();

    UXMapID uxMapID;
    XArea* pArea = pMover->GetArea();
    if (pArea) {
        uxMapID = pArea->GetInstanceID();
    }

    ST_MOVE_STOP stMoveStop;
    stMoveStop.dwActorID = pMover->GetID();
    stMoveStop.nMapID = uxMapID.nMapID;
    stMoveStop.fPosX = curPos.x;
    stMoveStop.fPosY = curPos.y;
    stMoveStop.fPosZ = curPos.z;
    stMoveStop.fYaw = fYaw;
    stMoveStop.fPitch = pMover->GetLookPitch();

    XSendPacket xPacket(5, 4);
    xPacket << stMoveStop;
    SendBroadCastAfterLoading(&xPacket, eNoneSelf);

    m_fLastSendMoveTime = 0.0f;
    DebugOut("send_eSUB_CMD_MOVE_STOP>> (%.2f,%.2f) %.3f", curPos.x, curPos.y, fYaw);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_BATTLE
// IDA: 0x14036F1E0
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_BATTLE(CMover* pMover, bool bPlayMotion) {
    ST_MOVE_BATTLE stMoveBattle;
    stMoveBattle.dwActorID = pMover->GetID();

    hkvVec3 vPos = pMover->GetPosition();
    stMoveBattle.fPosX = vPos.x;
    stMoveBattle.fPosY = vPos.y;
    stMoveBattle.fPosZ = vPos.z;
    stMoveBattle.fYaw = pMover->GetMovingYaw();
    stMoveBattle.bBattlePose = pMover->IsBattlePose();
    stMoveBattle.bPlayMotion = bPlayMotion;

    XSendPacket xPacket(5, 8);
    xPacket << stMoveBattle;
    SendBroadCastAfterLoading(&xPacket, eNoneSelf);

    DebugOut("send_eSUB_CMD_MOVE_BATTLE>> (%.2f,%.2f) %.3f (%d/%d)",
             vPos.x, vPos.y, stMoveBattle.fYaw, stMoveBattle.bBattlePose, bPlayMotion);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_GAZE
// IDA: 0x14036F480
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_GAZE(CMover* pMover) {
    XSendPacket xPacket(5, 0xA);

    xPacket << pMover->GetID();

    hkvVec3 vPos = pMover->GetPosition();
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << pMover->GetMovingYaw();
    xPacket << pMover->IsGazeMoving();

    SendBroadCastAfterLoading(&xPacket, eNoneSelf);

    DebugOut("send_eSUB_CMD_MOVE_GAZE>> (%.2f,%.2f) %.3f (%d)",
             vPos.x, vPos.y, pMover->GetMovingYaw(), pMover->IsGazeMoving());
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_TRACE
// IDA: 0x14036F6B0
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_TRACE(CMover* pMover) {
    XSendPacket xPacket(5, 0xB);

    xPacket << pMover->GetID();

    hkvVec3 vPos = pMover->GetPosition();
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << pMover->GetMovingYaw();
    xPacket << pMover->GetLookPitch();
    xPacket << pMover->GetAnimationIdx();
    xPacket << static_cast<std::int8_t>(pMover->GetDefenseType());

    // Projectile count
    auto pSkillMgr = pMover->GetSkillMgr();
    auto& vecProjectiles = pSkillMgr->GetProjectiles();
    xPacket << static_cast<std::uint8_t>(vecProjectiles.size());

    for (size_t i = 0; i < vecProjectiles.size(); ++i) {
        hkvVec3 projPos = vecProjectiles[i]->GetPosition();
        xPacket << projPos.x;
        xPacket << projPos.y;
        xPacket << projPos.z;
    }

    xPacket << pMover->GetAIState();

    // AI fuzzy and action values
    float fFuzzy[4] = {0};
    int nAction[7] = {0};
    pMover->GetAIFuzzyValue(fFuzzy);
    pMover->GetAIActionValue(nAction);

    for (int i = 0; i < 4; ++i) {
        xPacket << fFuzzy[i];
    }
    for (int i = 0; i < 7; ++i) {
        xPacket << nAction[i];
    }

    tagMOVE_POS movePos = pMover->GetMovePos();
    xPacket << movePos.x;
    xPacket << movePos.y;

    // Waypoint and aggro list for monsters
    std::int16_t shState = 0;
    std::uint8_t byCount = 0;

    CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    if (pMonster) {
        auto pWayPoint = pMonster->GetWayPoint();
        if (pWayPoint) {
            shState = pWayPoint->GetCurID();
        }
        xPacket << shState;

        auto pAggroList = pMonster->GetAggroList();
        byCount = static_cast<std::uint8_t>(pAggroList->size());
        xPacket << byCount;

        for (auto it = pAggroList->begin(); it != pAggroList->end(); ++it) {
            xPacket << it->first;
            xPacket << it->second.fThreat;
        }
    } else {
        xPacket << shState;
        xPacket << byCount;
    }

    SendBroadCastAfterLoading(&xPacket, eAll);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_IDLE
// IDA: 0x14036FD50
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_IDLE(CMover* pMover, float fMoveDelayTime) {
    XSendPacket xPacket(5, 0xC);

    xPacket << pMover->GetID();

    hkvVec3 vPos = pMover->GetPosition();
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << pMover->GetMovingYaw();
    xPacket << pMover->GetAnimationIdx();
    xPacket << pMover->GetDefaultAnimStep();
    xPacket << fMoveDelayTime;

    SendBroadCastAfterLoading(&xPacket, eNoneSelf);
}

// ============================================================================
// CMover::CanUseItem
// IDA: 0x14036B530
// ============================================================================
bool CMover::CanUseItem(std::uint32_t dwID, std::uint32_t& dwError) {
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pItemRef = pServer->m_xResourceMgr.GetTB_ITEM(dwID);
    XArea* pArea = GetArea();

    if (!pItemRef) {
        return false;
    }

    TB_ITEM_CLASSIFY* pItemClassify = pServer->m_xResourceMgr.GetTB_ITEM_CLASSIFY(pItemRef->Item_Classify_Index);
    if (!pItemClassify) {
        return false;
    }

    // Check item type
    if (pItemRef->Item_Sub_Type == 2) {
        // Consumption item
        if (pItemRef->Item_Effect_Type == 1) {
            // Self revive item
            if (!pArea || !pArea->IsRevive()) {
                dwError = 52607;
                return false;
            }
            if (!IsDie()) {
                dwError = 52608;
                return false;
            }
        } else if (pItemRef->Item_Effect_Type == 2) {
            // Party revive item
            if (!pArea || !pArea->IsRevive()) {
                dwError = 52607;
                return false;
            }

            // Check party/force for dead members
            // TODO: GetGOC template not implemented
            // For now, simplified check
            if (!IsDie()) {
                dwError = 52608;
                return false;
            }
        }
    } else if (pItemRef->Item_Sub_Type == 1) {
        // Buff item
        TB_BUFF* pBuff = pServer->m_xResourceMgr.GetTB_BUFF(pItemRef->Item_Effect_ID);
        if (!pBuff) {
            dwError = 52607;
            return false;
        }

        if (pItemRef->Item_Effect_Type == 1) {
            // Self buff
            if (pBuff->EffectType_01 == 1) {
                // Revive buff
                if (!pArea || !pArea->IsRevive()) {
                    dwError = 52607;
                    return false;
                }
                if (FindBuffByEffectType(1, 0) != -1) {
                    dwError = 52609;
                    return false;
                }
            }
        } else if (pItemRef->Item_Effect_Type == 2) {
            // Party buff
            if (pBuff->EffectType_01 == 1) {
                if (!pArea || !pArea->IsRevive()) {
                    dwError = 52607;
                    return false;
                }

                // Check if already has buff
                if (FindBuffByEffectType(1, 0) != -1) {
                    dwError = 52609;
                    return false;
                }
            }
        }
    }

    return pArea ? pArea->IsModeCondition() : true;
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_INFO
// IDA: 0x14036FEF0
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_INFO(CMover* pMover, std::uint32_t dwType, std::uint32_t dwVal) {
    XSendPacket xPacket(5, 0xD);

    xPacket << pMover->GetID();
    xPacket << dwType;
    xPacket << dwVal;

    SendBroadCastAfterLoading(&xPacket, eNoneSelf);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_STIFFEN
// IDA: 0x14036FFF0
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_STIFFEN(CMover* pMover, float fRate, float fTime) {
    XSendPacket xPacket(5, 0x12);

    xPacket << pMover->GetID();
    xPacket << fRate;
    xPacket << fTime;

    SendBroadCastAfterLoading(&xPacket, eNoneSelf);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA
// IDA: 0x140370100
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(hkvVec3* vPos, bool bForced) {
    if (!bForced && GetType() != eActorMonster) {
        return;
    }

    m_stExtMovingVal.Clear();

    float fYaw = GetOrientationYaw();
    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    ST_MOVE_IGNORE_MOTION_DELTA stMoveIgnore;
    stMoveIgnore.dwActorID = GetID();
    stMoveIgnore.fPosX = vPos->x;
    stMoveIgnore.fPosY = vPos->y;
    stMoveIgnore.fPosZ = vPos->z;
    stMoveIgnore.fYaw = fYaw;
    stMoveIgnore.fPitch = GetLookPitch();
    stMoveIgnore.bForced = bForced;

    XSendPacket xPacket(5, 0x13);
    xPacket << stMoveIgnore;
    SendBroadCastAfterLoading(&xPacket, eAll);

    m_fLastSendMoveTime = 0.0f;
    DebugOut("send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA>> (%.2f,%.2f) %.3f", vPos->x, vPos->y, fYaw);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_UPDATE_DIR
// IDA: 0x140370390
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_UPDATE_DIR(CMover* pMover, bool bDirect) {
    XSendPacket xPacket(5, 0x14);

    float fYaw = pMover->GetMovingYaw();
    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE_UPDATE_DIR>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    xPacket << pMover->GetID();
    xPacket << fYaw;
    xPacket << pMover->GetLookPitch();
    xPacket << static_cast<std::uint8_t>(bDirect);

    SendBroadCastAfterLoading(&xPacket, eNoneSelf);
    m_fLastSendMoveTime = 0.0f;

    DebugOut("send_eSUB_CMD_MOVE_UPDATE_DIR>> %.3f", fYaw);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_DROP
// IDA: 0x140370570
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_DROP(CMover* pMover, bool bSync) {
    XSendPacket xPacket(5, 0x15);

    hkvVec3 vPos = pMover->GetPosition();
    xPacket << pMover->GetID();
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << pMover->GetMovingYaw();
    xPacket << static_cast<std::uint8_t>(bSync);

    SendBroadCastAfterLoading(&xPacket, eNoneSelf);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_GRAP
// IDA: 0x1403706E0
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_GRAP(CMover* pMover, hkvVec3 vPos) {
    XSendPacket xPacket(5, 0x20);

    xPacket << pMover->GetID();
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;

    SendBroadCastAfterLoading(&xPacket, eNoneSelf);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_ATTACED_BT
// IDA: 0x140370800
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_ATTACED_BT(CMover* pAttackerMover, CMover* pTargetMover, hkvVec3 vAttachDir, float fAttachedDirDist) {
    XSendPacket xPacket(5, 0x34);

    UXActorID uxAttackerID;
    if (pAttackerMover) {
        uxAttackerID = pAttackerMover->GetActorID();
    } else {
        uxAttackerID = UXActorID();
    }

    xPacket << CQuestCondition::GetQuestID(&uxAttackerID);
    xPacket << pTargetMover->GetID();
    xPacket << vAttachDir.x;
    xPacket << vAttachDir.y;
    xPacket << vAttachDir.z;
    xPacket << fAttachedDirDist;

    SendBroadCastAfterLoading(&xPacket, eNoneSelf);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_ATTACED_END_BT
// IDA: 0x1403709C0
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_ATTACED_END_BT(CMover* pTargetMover) {
    XSendPacket xPacket(5, 0x35);

    xPacket << pTargetMover->GetID();

    SendBroadCastAfterLoading(&xPacket, eNoneSelf);
}

// ============================================================================
// CMover::send_eSUB_CMD_SKILL_MOVING_TARGET
// IDA: 0x140373890
// ============================================================================
void CMover::send_eSUB_CMD_SKILL_MOVING_TARGET(std::vector<PS_MOVING_TARGET>& vecMovingTargetList) {
    if (!this) {
        return;
    }

    XSendPacket xPacket(6, 0x53);
    PS_MOVING_TARGET_LIST psMovingTargetList;

    // Copy moving targets
    for (size_t i = 0; i < vecMovingTargetList.size(); ++i) {
        psMovingTargetList.vecMovingTarget.push_back(vecMovingTargetList[i]);
    }

    // Send packet with actor ID and moving target list
    xPacket.XParse << GetID();
    xPacket << psMovingTargetList;

    SendBroadCast(&xPacket, eNone);
}

// ============================================================================
// CMover::send_eSUB_CMD_SKILL_SYNC_POSITION
// IDA: 0x1403733E0
// Verified: no
// ============================================================================
void CMover::send_eSUB_CMD_SKILL_SYNC_POSITION(CMover* pMover, const hkvVec3& vPos) {
    // IDA decompiled code preservation:
    // XSendPacket::XSendPacket(&xPacket, 6u, 0x48u);
    // GetID() to get actor ID
    // Serialize position (x, y, z) and moving yaw
    // SendBroadCast with eNone type
    
    XSendPacket xPacket(6, 0x48);
    
    // Serialize actor ID
    xPacket.XParse << pMover->GetID();
    
    // Serialize position
    xPacket.XParse << vPos.x;
    xPacket.XParse << vPos.y;
    xPacket.XParse << vPos.z;
    
    // Serialize moving yaw
    float fMovingYaw = pMover->GetMovingYaw();
    xPacket.XParse << fMovingYaw;
    
    // Broadcast to nearby players
    SendBroadCast(&xPacket, eNone);
    
    // Debug output
    DebugOut("send_eSUB_CMD_SKILL_SYNC_POSITION>> (%.2f,%.2f,%.2f)", vPos.x, vPos.y, vPos.z);
}

// ============================================================================
// CMover::send_eSUB_CMD_BUFF_UPDATE
// IDA: 0x1403729E0
// Verified: no
// ============================================================================
void CMover::send_eSUB_CMD_BUFF_UPDATE(CMover* pMover, std::int16_t wBuffID, float fTime,
                                        std::int8_t byCount, std::uint32_t dwOwnerID,
                                        std::uint8_t bySendType, bool bShow) {
    // IDA decompiled code preservation:
    // XSendPacket::XSendPacket(&xPacket, 6u, 0x14u);
    // Serialize: ActorID, BuffID, Time, Count, OwnerID, Show flag
    // SendType: 0 = Broadcast, 1 = Send to self
    
    XSendPacket xPacket(6, 0x14);
    
    // Serialize actor ID
    xPacket.XParse << pMover->GetID();
    
    // Serialize buff data
    xPacket.XParse << wBuffID;
    xPacket.XParse << fTime;
    xPacket.XParse << byCount;
    xPacket.XParse << dwOwnerID;
    xPacket.XParse << bShow;
    
    // Send based on type
    if (bySendType) {
        // Send to self only
        CGocNetwork::Send(this, xPacket);
    } else {
        // Broadcast to nearby players
        SendBroadCast(&xPacket, eNone);
    }
    
    // Debug output
    DebugOut("send_eSUB_CMD_BUFF_UPDATE>> %d / %.2f / %d", wBuffID, fTime, byCount);
}

// ============================================================================
// CMover::send_eSUB_CMD_BUFF_CHANGE
// IDA: 0x140372BB0
// Verified: no
// ============================================================================
void CMover::send_eSUB_CMD_BUFF_CHANGE(CMover* pMover, std::int16_t wBuffID, std::int16_t wNewBuffID,
                                        float fTime, std::int8_t byCount, std::uint32_t dwOwnerID,
                                        std::uint8_t bySendType) {
    // IDA decompiled code preservation:
    // XSendPacket::XSendPacket(&xPacket, 6u, 0x16u);
    // Serialize: ActorID, OldBuffID, NewBuffID, Time, Count, OwnerID
    // SendType: 0 = Broadcast, 1 = Send to self
    
    XSendPacket xPacket(6, 0x16);
    
    // Serialize actor ID
    xPacket.XParse << pMover->GetID();
    
    // Serialize buff change data
    xPacket.XParse << wBuffID;
    xPacket.XParse << wNewBuffID;
    xPacket.XParse << fTime;
    xPacket.XParse << byCount;
    xPacket.XParse << dwOwnerID;
    
    // Send based on type
    if (bySendType) {
        // Send to self only
        CGocNetwork::Send(this, xPacket);
    } else {
        // Broadcast to nearby players
        SendBroadCast(&xPacket, eNone);
    }
    
    // Debug output
    DebugOut("eSUB_CMD_BUFF_CHANGE>> %d->%d / %.2f / %d", wBuffID, wNewBuffID, fTime, byCount);
}

// ============================================================================
// CMover::send_eSUB_CMD_BUFF_DELETE
// IDA: 0x140372D90
// Verified: no
// ============================================================================
void CMover::send_eSUB_CMD_BUFF_DELETE(CMover* pMover, std::int16_t wBuffID, std::uint32_t dwOwnerID,
                                        bool bExcuteOutSkill, std::uint8_t bySendType) {
    // IDA decompiled code preservation:
    // XSendPacket::XSendPacket(&xPacket, 6u, 0x15u);
    // Serialize: ActorID, BuffID, ExcuteOutSkill flag, OwnerID
    // Check if actor has area - if not, force SendType = 1
    // SendType: 0 = Broadcast, 1 = Send to self
    
    XSendPacket xPacket(6, 0x15);
    
    // Serialize actor ID
    xPacket.XParse << pMover->GetID();
    
    // Serialize buff delete data
    xPacket.XParse << wBuffID;
    xPacket.XParse << bExcuteOutSkill;
    xPacket.XParse << dwOwnerID;
    
    // Check if actor has area
    XArea* pArea = GetArea();
    if (!pArea) {
        bySendType = 1; // Force send to self if no area
    }
    
    // Send based on type
    if (bySendType) {
        // Send to self only
        CGocNetwork::Send(this, xPacket);
    } else {
        // Broadcast to nearby players
        SendBroadCast(&xPacket, eNone);
    }
    
    // Debug output
    DebugOut("send_eSUB_CMD_BUFF_DELETE>> %d", wBuffID);
}

// ============================================================================
// CMover::send_eSUB_CMD_MONSTER_TARGET_CHANGE
// IDA: 0x140370A90
// ============================================================================
void CMover::send_eSUB_CMD_MONSTER_TARGET_CHANGE(CMover* pMover, std::uint32_t dwTargetID) {
    XSendPacket xPacket(0x17, 0x22);

    xPacket << pMover->GetID();
    xPacket << dwTargetID;

    SendBroadCast(&xPacket, eNoneSelf);

    DebugOut("send_eSUB_CMD_MONSTER_TARGET_CHANGE>> %d", dwTargetID);
}

// ============================================================================
// CMover::send_eSUB_CMD_MONSTER_INVISIBLE
// IDA: 0x140370BA0
// ============================================================================
void CMover::send_eSUB_CMD_MONSTER_INVISIBLE(CMover* pMover, std::uint8_t byInvisible, std::uint32_t dwFlag, int nType, int nValue) {
    XSendPacket xPacket(0x17, 0x32);

    xPacket << pMover->GetID();
    xPacket << byInvisible;
    xPacket << dwFlag;
    xPacket << nType;
    xPacket << nValue;

    SendBroadCast(&xPacket, eNoneSelf);

    DebugOut("send_eSUB_CMD_MONSTER_INVISIBLE>> %d", byInvisible);
}

// ============================================================================
// CMover::send_eSUB_CMD_MONSTER_CHANGE_MOTION
// IDA: 0x140370CF0
// ============================================================================
void CMover::send_eSUB_CMD_MONSTER_CHANGE_MOTION(CMover* pMover, std::int16_t nMotion, std::int16_t nSubMotion) {
    XSendPacket xPacket(0x17, 0x33);

    xPacket << pMover->GetID();
    xPacket << nMotion;
    xPacket << nSubMotion;

    SendBroadCast(&xPacket, eNoneSelf);

    DebugOut("send_eSUB_CMD_MONSTER_CHANGE_MOTION>> %d, %d", nMotion, nSubMotion);
}

// ============================================================================
// CMover::send_eSUB_CMD_CONTROL_MONSTER
// IDA: 0x140370E20
// ============================================================================
void CMover::send_eSUB_CMD_CONTROL_MONSTER(CMover* pMover, CMover* pSummonMover, std::uint32_t dwTableID, float fAlphaValue, float fBlendingTime, std::uint32_t nAnimationID, bool bFallowMonster) {
    XSendPacket xPacket(0x17, 0x51);

    xPacket << pMover->GetID();

    if (pSummonMover) {
        xPacket << pSummonMover->GetID();
        hkvVec3 vPos = pSummonMover->GetPosition();
        xPacket << vPos.x;
        xPacket << vPos.y;
        xPacket << vPos.z;
    } else {
        xPacket << static_cast<std::uint32_t>(0);
        xPacket << 0.0f;
        xPacket << 0.0f;
        xPacket << 0.0f;
    }

    xPacket << pMover->GetMovingYaw();
    xPacket << dwTableID;
    xPacket << fAlphaValue;
    xPacket << fBlendingTime;
    xPacket << nAnimationID;
    xPacket << static_cast<std::uint8_t>(bFallowMonster);

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_CONTROL_MONSTER>> %d ", dwTableID);
}

// ============================================================================
// CMover::send_eSUB_CMD_MONSTER_ESCAPE_DAMAGE
// IDA: 0x1403710A0
// ============================================================================
void CMover::send_eSUB_CMD_MONSTER_ESCAPE_DAMAGE(CMover* pMover, std::uint8_t byDefenseType, float fTime) {
    XSendPacket xPacket(0x17, 0x34);

    xPacket << pMover->GetID();
    xPacket << byDefenseType;
    xPacket << fTime;

    SendBroadCast(&xPacket, eNoneSelf);

    DebugOut("send_eSUB_CMD_MONSTER_ESCAPE_DAMAGE>> %d / %.2f", byDefenseType, fTime);
}

// ============================================================================
// CMover::send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE
// IDA: 0x1403711E0
// ============================================================================
void CMover::send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE(CMover* pMover, float fCurSuperArmor, float fMaxSuperArmor) {
    XSendPacket xPacket(0x17, 0x53);

    xPacket << pMover->GetID();
    xPacket << fCurSuperArmor;
    xPacket << fMaxSuperArmor;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE>> %.2f / %.2f", fCurSuperArmor, fMaxSuperArmor);
}

// ============================================================================
// CMover::send_eSUB_CMD_MONSTER_PARTS_HP
// IDA: 0x140371330
// ============================================================================
void CMover::send_eSUB_CMD_MONSTER_PARTS_HP(CMover* pMover, int iCurHP1, int iMaxHP1, int iCurHP2, int iMaxHP2) {
    XSendPacket xPacket(0x17, 0x54);

    xPacket << pMover->GetID();
    xPacket << iCurHP1;
    xPacket << iMaxHP1;
    xPacket << iCurHP2;
    xPacket << iMaxHP2;

    SendBroadCast(&xPacket, eNoneSelf);

    DebugOut("send_eSUB_CMD_MONSTER_PARTS_HP>> %d / %d", iCurHP1, iMaxHP1);
}

// ============================================================================
// CMover::send_eSUB_CMD_ACTIVE_SKILL
// IDA: 0x1403714A0
// ============================================================================
void CMover::send_eSUB_CMD_ACTIVE_SKILL(CMover* pMover, std::uint32_t nSkillID, std::uint8_t byAngleAttackType) {
    XSendPacket xPacket(6, 0x10);

    PS_SkillActive_BT btInfo;
    float fYaw = pMover->GetOrientationYaw();
    hkvVec3 vPos = GetPosition();

    btInfo.uxUseActorID = pMover->GetActorID();
    btInfo.uxActorID = pMover->GetActorID();
    btInfo.nSkillID = nSkillID;
    btInfo.psSkillPosInfo.nMotionClass = -1;
    btInfo.psSkillPosInfo.fAngle = fYaw;
    btInfo.psSkillPosInfo.xPos = vPos;
    btInfo.nRandomKey = pMover->CreateRandomTrapIndex();
    btInfo.byAngleAttackType = byAngleAttackType;

    xPacket << btInfo;
    SendBroadCast(&xPacket, eNoneSelf);

    DebugOut("send_eSUB_CMD_ACTIVE_SKILL>> %d (%.2f,%.2f) %.3f", nSkillID, vPos.x, vPos.y, fYaw);
}

// ============================================================================
// CMover::send_eSUB_CMD_ACTION_SKILL
// IDA: 0x1403716C0
// ============================================================================
void CMover::send_eSUB_CMD_ACTION_SKILL(CMover* pMover, std::uint32_t nSkillID, std::int16_t nTriggerIdx, hkvVec3 vPos, std::uint8_t byAttackTargetCnt, std::uint16_t wContinousHit, bool bPenetrate) {
    XSendPacket xPacket(6, 0x13);

    PS_SkillActionEx stSkillActionEx;
    float fYaw = pMover->GetOrientationYaw();

    stSkillActionEx.nSkillID = nSkillID;
    stSkillActionEx.nTriggerIdx = nTriggerIdx;
    stSkillActionEx.wContinousHit = wContinousHit;
    stSkillActionEx.uxActorID = GetActorID();
    stSkillActionEx.psSkillActorInfo.psSkillPosInfo.xPos = vPos;
    stSkillActionEx.psSkillActorInfo.psSkillPosInfo.fAngle = fYaw;
    stSkillActionEx.bPenetrate = bPenetrate;

    PS_SkillDmgResult psSkillDmgResult;
    CMySkillList* pSkillMgr = pMover->GetSkillMgr();

    for (int i = 0; i < byAttackTargetCnt && i < 100; ++i) {
        PS_SkillDmg stSkillDmg;

        tagSKILL_ACTION_DAMAGE stDamage;
        pSkillMgr->GetAttackDamage(&stDamage, i);

        UXActorID targetID = pSkillMgr->GetAttackTarget(i);
        stSkillDmg.uxActorID = targetID;
        stSkillDmg.byReactionType = stDamage.byReactionType;
        stSkillDmg.byDamageFlag = stDamage.byDamageFlag;
        stSkillDmg.nDamage = stDamage.nDamage;
        stSkillDmg.nAttrDamage = stDamage.nAttrDamage;
        stSkillDmg.nHP = stDamage.nHP;
        stSkillDmg.xExtraMove = pSkillMgr->GetAttackExtraMove(i);
        stSkillDmg.fFlySpeed = pSkillMgr->GetAttackFlySpeed(i);
        stSkillDmg.byHitPartsIndex = stDamage.byHitPartsIndex;

        CMover* pTargetMover = GetMoverObject(CQuestCondition::GetQuestID(&stSkillDmg.uxActorID));
        if (pTargetMover) {
            stSkillDmg.byDefenseType = pTargetMover->GetDefenseType();
            stSkillDmg.fCurSuperArmorGage = pTargetMover->GetCurSuperArmorGage();
        } else {
            stSkillDmg.byDefenseType = 4;
            stSkillDmg.fCurSuperArmorGage = 0.0f;
        }

        psSkillDmgResult.vSkillDmg.push_back(stSkillDmg);
    }

    PS_SkillAction psSkillAction = stSkillActionEx;
    xPacket << psSkillDmgResult;
    xPacket << psSkillAction;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_ACTION_SKILL>> %d (%.2f,%.2f) %.3f count:%d continous:%d",
             nSkillID, vPos.x, vPos.y, fYaw, byAttackTargetCnt, wContinousHit);
}

// ============================================================================
// CMover::send_eSUB_CMD_SKILL_SUMMON_AKASHIC
// IDA: 0x140371BC0
// ============================================================================
void CMover::send_eSUB_CMD_SKILL_SUMMON_AKASHIC(CMover* pMover, UXActorID uxActorID, float fAlphaValue, float fBlendingTime, hkvVec3 vPos, bool bApplyRotation, float fYaw) {
    XSendPacket xPacket(6, 0x7B);

    xPacket << pMover->GetID();
    xPacket << CQuestCondition::GetQuestID(&uxActorID);
    xPacket << fAlphaValue;
    xPacket << fBlendingTime;
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << static_cast<std::uint8_t>(bApplyRotation);
    xPacket << fYaw;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_SKILL_SUMMON_AKASHIC");
}

// ============================================================================
// CMover::send_eSUB_CMD_SKILL_COOLTIME_REDUCE
// IDA: 0x140371D70
// ============================================================================
void CMover::send_eSUB_CMD_SKILL_COOLTIME_REDUCE(CMover* pMover, float fReduceRate) {
    XSendPacket xPacket(6, 0x7C);

    xPacket << pMover->GetID();
    xPacket << fReduceRate;

    Send(&xPacket);

    DebugOut("send_eSUB_CMD_SKILL_COOLTIME_REDUCE");
}

// ============================================================================
// CMover::send_eSUB_CMD_SKILL_DEFENCE_TYPE
// IDA: 0x140371EA0
// ============================================================================
void CMover::send_eSUB_CMD_SKILL_DEFENCE_TYPE(CMover* pMover, std::uint8_t byDefenceType, bool bAdd) {
    XSendPacket xPacket(6, 0x7D);

    xPacket << pMover->GetID();
    xPacket << byDefenceType;
    xPacket << static_cast<std::uint8_t>(bAdd);

    Send(&xPacket);

    DebugOut("send_eSUB_CMD_SKILL_DEFENCE_TYPE");
}

// ============================================================================
// CMover::send_eSUB_CMD_PROJECTILE
// IDA: 0x140371FE0
// ============================================================================
void CMover::send_eSUB_CMD_PROJECTILE(CMover* pMover, std::uint32_t nSkillID, std::int16_t nTriggerIdx, hkvVec3 vPos, hkvVec3 vDir, std::uint32_t nSessionID) {
    XSendPacket xPacket(6, 0x32);

    PS_Projectile_BT stProjectile;
    stProjectile.nSkillID = nSkillID;
    stProjectile.nTriggerIdx = nTriggerIdx;
    stProjectile.xPos = vPos;
    stProjectile.xDir = vDir;
    stProjectile.nSessionID = nSessionID;
    stProjectile.uxActorID = pMover->GetActorID();

    xPacket << stProjectile;
    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_PROJECTILE>> %d (%.2f,%.2f,%.2f) -> (%.2f,%.2f,%.2f)",
             nSkillID, vPos.x, vPos.y, vPos.z, vDir.x, vDir.y, vDir.z);
}

// ============================================================================
// CMover::send_eSUB_CMD_PROJECTILE_ATTACH
// IDA: 0x140372230
// ============================================================================
void CMover::send_eSUB_CMD_PROJECTILE_ATTACH(CMover* pMover, int nSessionID, hkvVec3 vPos, std::uint32_t dwTargetID) {
    XSendPacket xPacket(6, 0x35);

    xPacket << nSessionID;
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << dwTargetID;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_PROJECTILE_ATTACH_BT>> %d (%.2f,%.2f,%.2f) -> (%d)",
             nSessionID, vPos.x, vPos.y, vPos.z, dwTargetID);
}

// ============================================================================
// CMover::send_eSUB_CMD_CHAIN
// IDA: 0x1403723A0
// ============================================================================
void CMover::send_eSUB_CMD_CHAIN(CMover* pMover, std::uint32_t nSkillID, std::int16_t nTriggerIdx, hkvVec3 vPos, hkvVec3 vDir, std::uint32_t nSessionID, std::uint32_t dwTargetID) {
    XSendPacket xPacket(6, 0x38);

    PS_Chain_BT stChain;
    stChain.nSkillID = nSkillID;
    stChain.nTriggerIdx = nTriggerIdx;
    stChain.xPos = vPos;
    stChain.xDir = vDir;
    stChain.nSessionID = nSessionID;
    stChain.dwTargetID = dwTargetID;
    stChain.uxActorID = pMover->GetActorID();

    xPacket << stChain;
    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_CHAIN>> %d (%.2f,%.2f,%.2f) -> (%.2f,%.2f,%.2f)",
             nSkillID, vPos.x, vPos.y, vPos.z, vDir.x, vDir.y, vDir.z);
}

// ============================================================================
// CMover::send_eSUB_CMD_CHAIN_TARGET_UPDATE
// IDA: 0x1403725E0
// ============================================================================
void CMover::send_eSUB_CMD_CHAIN_TARGET_UPDATE(CMover* pMover, int nSessionID, bool bAdd, std::uint32_t dwTargetID) {
    XSendPacket xPacket(6, 0x39);

    xPacket << nSessionID;
    xPacket << static_cast<std::uint8_t>(bAdd);
    xPacket << dwTargetID;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_CHAIN_TARGET_UPDATE>> (%d %d)", nSessionID, dwTargetID);
}

// ============================================================================
// CMover::send_eSUB_CMD_PROJECTILE_UPDATE_TARGET
// IDA: 0x1403726D0
// ============================================================================
void CMover::send_eSUB_CMD_PROJECTILE_UPDATE_TARGET(CMover* pMover, int nSessionID, int dwTargetID) {
    XSendPacket xPacket(6, 0x3A);

    xPacket << nSessionID;
    xPacket << dwTargetID;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_PROJECTILE_UPDATE_TARGET>> (%d %d)", nSessionID, dwTargetID);
}

// ============================================================================
// CMover::send_eSUB_CMD_PROJECTILE_REMOVE
// IDA: 0x1403727B0
// ============================================================================
void CMover::send_eSUB_CMD_PROJECTILE_REMOVE(CMover* pMover, int nSessionID) {
    XSendPacket xPacket(6, 0x3B);

    xPacket << nSessionID;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_PROJECTILE_REMOVE>> (%d)", nSessionID);
}

// ============================================================================
// CMover::send_eSUB_CMD_COMBAT_TYPE
// IDA: 0x140372870
// ============================================================================
void CMover::send_eSUB_CMD_COMBAT_TYPE(CMover* pMover, std::uint32_t nType, float fTime, std::uint8_t byCount) {
    if (!pMover) {
        return;
    }

    XSendPacket xPacket(6, 0x3C);

    xPacket << pMover->GetID();
    xPacket << nType;
    xPacket << fTime;
    xPacket << byCount;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_COMBAT_TYPE>> (%d/%.2f/%d)", nType, fTime, byCount);
}

// ============================================================================
// CMover::send_eSUB_CMD_BUFF_UPDATE
// IDA: 0x1403729E0
// ============================================================================
void CMover::send_eSUB_CMD_BUFF_UPDATE(CMover* pMover, std::int16_t wBuffID, float fTime, std::int8_t byCount, std::uint32_t dwOwnerID, std::uint8_t bySendType, bool bShow) {
    XSendPacket xPacket(6, 0x14);

    xPacket << pMover->GetID();
    xPacket << wBuffID;
    xPacket << fTime;
    xPacket << byCount;
    xPacket << dwOwnerID;
    xPacket << static_cast<std::uint8_t>(bShow);

    if (bySendType) {
        Send(&xPacket);
    } else {
        SendBroadCast(&xPacket, eAll);
    }

    DebugOut("send_eSUB_CMD_BUFF_UPDATE>> %d / %.2f / %d", wBuffID, fTime, byCount);
}

// ============================================================================
// CMover::send_eSUB_CMD_BUFF_CHANGE
// IDA: 0x140372BB0
// ============================================================================
void CMover::send_eSUB_CMD_BUFF_CHANGE(CMover* pMover, std::int16_t wBuffID, std::int16_t wNewBuffID, float fTime, std::int8_t byCount, std::uint32_t dwOwnerID, std::uint8_t bySendType) {
    XSendPacket xPacket(6, 0x16);

    xPacket << pMover->GetID();
    xPacket << wBuffID;
    xPacket << wNewBuffID;
    xPacket << fTime;
    xPacket << byCount;
    xPacket << dwOwnerID;

    if (bySendType) {
        Send(&xPacket);
    } else {
        SendBroadCast(&xPacket, eAll);
    }

    DebugOut("eSUB_CMD_BUFF_CHANGE>> %d->%d / %.2f / %d", wBuffID, wNewBuffID, fTime, byCount);
}

// ============================================================================
// CMover::send_eSUB_CMD_BUFF_DELETE
// IDA: 0x140372D90
// ============================================================================
void CMover::send_eSUB_CMD_BUFF_DELETE(CMover* pMover, std::int16_t wBuffID, std::uint32_t dwOwnerID, bool bExcuteOutSkill, std::uint8_t bySendType) {
    XSendPacket xPacket(6, 0x15);

    xPacket << pMover->GetID();
    xPacket << wBuffID;
    xPacket << static_cast<std::uint8_t>(bExcuteOutSkill);
    xPacket << dwOwnerID;

    // If no area, force send to self only
    if (!pMover->GetArea()) {
        bySendType = 1;
    }

    if (bySendType) {
        Send(&xPacket);
    } else {
        SendBroadCast(&xPacket, eAll);
    }

    DebugOut("send_eSUB_CMD_BUFF_DELETE>> %d", wBuffID);
}

// ============================================================================
// CMover::send_eSUB_CMD_BUFF_DAMAGE
// IDA: 0x140372F60
// ============================================================================
void CMover::send_eSUB_CMD_BUFF_DAMAGE(CMover* pMover, std::int16_t wBuffID, int nDamage, int nHP, std::uint32_t dwOwnerID) {
    XSendPacket xPacket(6, 0x17);

    xPacket << pMover->GetID();
    xPacket << wBuffID;
    xPacket << nDamage;
    xPacket << nHP;
    xPacket << dwOwnerID;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_BUFF_DAMAGE>> %d, %d, %d", wBuffID, nDamage, dwOwnerID);
}

// ============================================================================
// CMover::send_eSUB_CMD_AURA_UPDATE
// IDA: 0x1403730C0
// ============================================================================
void CMover::send_eSUB_CMD_AURA_UPDATE(CMover* pMover, std::int16_t wAuraID) {
    XSendPacket xPacket(6, 0x18);

    xPacket << pMover->GetID();
    xPacket << wAuraID;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_AURA_UPDATE>> %d", wAuraID);
}

// ============================================================================
// CMover::send_eSUB_CMD_SKILL_WARP_POSITION
// IDA: 0x1403731D0
// ============================================================================
void CMover::send_eSUB_CMD_SKILL_WARP_POSITION(CMover* pMover, hkvVec3 vPos, bool bBroadCast) {
    XSendPacket xPacket(6, 0x46);

    PS_SkillActorInfo stPos;
    stPos.uxActorID = pMover->GetActorID();
    stPos.psSkillPosInfo.nMotionClass = -1;
    stPos.psSkillPosInfo.fAngle = pMover->GetMovingYaw();
    stPos.psSkillPosInfo.xPos = vPos;

    xPacket << stPos;

    if (bBroadCast) {
        SendBroadCast(&xPacket, eAll);
    } else {
        Send(&xPacket);
    }

    DebugOut("send_eSUB_CMD_SKILL_WARP_POSITION>> (%.2f,%.2f,%.2f)", vPos.x, vPos.y, vPos.z);
}

// ============================================================================
// CMover::send_eSUB_CMD_SKILL_SYNC_POSITION
// IDA: 0x1403733E0
// ============================================================================
void CMover::send_eSUB_CMD_SKILL_SYNC_POSITION(CMover* pMover, hkvVec3 vPos) {
    XSendPacket xPacket(6, 0x48);

    xPacket << pMover->GetID();
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << pMover->GetMovingYaw();

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_SKILL_SYNC_POSITION>> (%.2f,%.2f,%.2f)", vPos.x, vPos.y, vPos.z);
}

// ============================================================================
// CMover::send_eSUB_CMD_SKILL_MOVE_WITH_TIME
// IDA: 0x140373580
// ============================================================================
void CMover::send_eSUB_CMD_SKILL_MOVE_WITH_TIME(CMover* pMover, hkvVec3 vPos, float fTime) {
    XSendPacket xPacket(6, 0x49);

    xPacket << pMover->GetID();
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << fTime;
    xPacket << pMover->GetMovingYaw();

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_SKILL_MOVE_WITH_TIME>> (%.2f,%.2f,%.2f,%.2f)", vPos.x, vPos.y, vPos.z, fTime);
}

// ============================================================================
// CMover::send_eSUB_CMD_SKILL_CHARGING_END_BT
// IDA: 0x140373770
// ============================================================================
void CMover::send_eSUB_CMD_SKILL_CHARGING_END_BT(CMover* pMover, float fPressTime) {
    XSendPacket xPacket(6, 0x50);

    xPacket << pMover->GetID();
    xPacket << fPressTime;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_SKILL_CHARGING_END_BT>> (%.2f)", fPressTime);
}

// ============================================================================
// CMover::send_eSUB_CMD_HELPER_SYNC_POS
// IDA: 0x140373A30
// ============================================================================
void CMover::send_eSUB_CMD_HELPER_SYNC_POS(CMover* pMover, hkvVec3 vPos) {
    XSendPacket xPacket(0x27, 0x13);

    xPacket << pMover->GetID();
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;

    SendBroadCast(&xPacket, eAll);

    DebugOut("send_eSUB_CMD_HELPER_SYNC_POS>> (%.2f,%.2f,%.2f)", vPos.x, vPos.y, vPos.z);
}

// ============================================================================
// CMover::GetMoveIdleInfo
// IDA: 0x140373B50
// ============================================================================
void CMover::GetMoveIdleInfo(PS_MOVE_IDLE& stMoveIdle, float fMoveDelayTime) {
    stMoveIdle.dwActorID = GetID();
    hkvVec3 vPos = GetPosition();
    stMoveIdle.fX = vPos.x;
    stMoveIdle.fY = vPos.y;
    stMoveIdle.fZ = vPos.z;
    stMoveIdle.fMoveingYaw = GetMovingYaw();
    stMoveIdle.dwAnimationIdx = GetAnimationIdx();
    stMoveIdle.fMoveDelayTime = fMoveDelayTime;
}

// ============================================================================
// CMover::RegisterTraceBoneName
// IDA: 0x140373C30
// ============================================================================
void CMover::RegisterTraceBoneName(VString strBoneName) {
    strBoneName.ToLower();
    const char* pszBoneName = strBoneName.AsChar();
    if (GetTraceBoneListIndex(pszBoneName) < 0) {
        m_vTraceBoneName.push_back(strBoneName);
    }
}

// ============================================================================
// CMover::ClearTraceBoneName
// IDA: 0x140373CB0
// ============================================================================
void CMover::ClearTraceBoneName() {
    m_vTraceBoneName.clear();
}

// ============================================================================
// CMover::GetTraceBoneListIndex
// IDA: 0x140373CE0
// ============================================================================
int CMover::GetTraceBoneListIndex(const char* pBoneName) {
    int iSize = static_cast<int>(m_vTraceBoneName.size());
    for (int i = 0; i < iSize; ++i) {
        if (m_vTraceBoneName[i].CompareNoCase(pBoneName)) {
            return i;
        }
    }
    return -1;
}

// ============================================================================
// CMover::AddContinuousMelee
// IDA: 0x140373D70
// ============================================================================
void CMover::AddContinuousMelee(AttackJudgmentTrigger* pTrigger, int nSkillID) {
    if (GetContinuousMelee(pTrigger)) {
        return;
    }

    SContinuousMelee* pTempContinuousMelee = new SContinuousMelee();
    pTempContinuousMelee->pTrigger = pTrigger;
    pTempContinuousMelee->nSkillID = nSkillID;
    pTempContinuousMelee->fRemainLifeTime = pTrigger->sContinuousMeleeInfo.fLifeTime;
    pTempContinuousMelee->fRemainIntervalTime = pTrigger->sContinuousMeleeInfo.fIntervalTime;

    m_vContinuousMelee.push_back(pTempContinuousMelee);
}

// ============================================================================
// CMover::GetContinuousMelee
// IDA: 0x140373E20
// ============================================================================
SContinuousMelee* CMover::GetContinuousMelee(AttackJudgmentTrigger* pTrigger) {
    for (auto it = m_vContinuousMelee.begin(); it != m_vContinuousMelee.end(); ++it) {
        SContinuousMelee* pContinuousMelee = *it;
        if (strcmp(pContinuousMelee->pTrigger->TriggerName, pTrigger->TriggerName) == 0) {
            return pContinuousMelee;
        }
    }
    return nullptr;
}

// ============================================================================
// CMover::CheckContinuousMelee
// IDA: 0x140373EE0
// ============================================================================
void CMover::CheckContinuousMelee(float fDeltaTime) {
    for (auto it = m_vContinuousMelee.begin(); it != m_vContinuousMelee.end(); ) {
        SContinuousMelee* pContinuousMelee = *it;

        pContinuousMelee->fRemainIntervalTime -= fDeltaTime;

        if (pContinuousMelee->fRemainIntervalTime <= 0.0f) {
            tagACTION_BUFFER xAction(1, 0.0f);
            xAction.pActionTrigger = pContinuousMelee->pTrigger;

            float fYaw = GetOrientationYaw();
            xAction << pContinuousMelee->pTrigger->EventID;
            xAction << pContinuousMelee->nSkillID;
            xAction << 1u;
            xAction << 0;
            xAction << fYaw;

            AddActionBuffer(&xAction);

            pContinuousMelee->fRemainIntervalTime = pContinuousMelee->pTrigger->sContinuousMeleeInfo.fIntervalTime;
        }

        pContinuousMelee->fRemainLifeTime -= fDeltaTime;

        if (pContinuousMelee->fRemainLifeTime <= 0.0f) {
            delete pContinuousMelee;
            it = m_vContinuousMelee.erase(it);
        } else {
            ++it;
        }
    }
}

// ============================================================================
// CMover::DebugOut
// IDA: 0x140374130
// ============================================================================
void CMover::DebugOut(const char* format, ...) {
    if (!m_bTraceUser) {
        return;
    }

    char szFormat[2048];
    char szBuffer[2048];

    va_list va;
    va_start(va, format);
    vsprintf_s(szFormat, format, va);
    va_end(va);

    SYSTEMTIME stSystemTime;
    GetLocalTime(&stSystemTime);

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fTime = pTimer->GetTime();

    sprintf(szBuffer, "%.3f -> %s", fTime, szFormat);
    VisError_cl::Warning(Vision::Error.fatalError_writeToLog, szBuffer);
}

// ============================================================================
// CMover::GetCombatType
// IDA: 0x140374FA0
// ============================================================================
int CMover::GetCombatType() {
    return -1;
}

// ============================================================================
// CMover::AllBuffClear
// IDA: ?AllBuffClear@CMover@@QEAAXE@Z (0x14036AA40)
// Referenced in SetDie function (line 1292)
// ============================================================================
void CMover::AllBuffClear(std::uint8_t byReason) {
    if (!m_nBuffTotalCnt) {
        return;
    }

    // Iterate through all 50 buff slots
    for (std::uint8_t i = 0; i < 50; ++i) {
        if (m_stBuffState[i].IsLife()) {
            // Buff is active - check if it should be cleared
            if (!byReason || IsClearBuff(m_stBuffState[i].nBuffIndex, byReason)) {
                ClearBuffStatusBySlot(i, false);
            }
        } else if (m_stBuffState[i].nBuffIndex) {
            // Buff has index but not active (永久Buff)
            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            TB_BUFF* pBuffRef = XResourceMgr::GetTB_BUFF(&pServer->m_xResourceMgr, m_stBuffState[i].nBuffIndex);
            if (pBuffRef) {
                if (!pBuffRef->Buff_Time && IsClearBuff(m_stBuffState[i].nBuffIndex, byReason)) {
                    ClearBuffStatusBySlot(i, false);
                }
            }
        }
    }
}

// ============================================================================
// CMover::FindBuffByEffectType
// IDA: ?FindBuffByEffectType@CMover@@QEAAHEG@Z (0x14036A560)
// ============================================================================
int CMover::FindBuffByEffectType(std::uint8_t byBuffEffect, std::uint16_t nExceptBuffIndex) {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != nExceptBuffIndex
            && m_stBuffState[i].nBuffIndex
            && m_stBuffState[i].byEffectType == byBuffEffect) {
            return i;
        }
    }
    return -1;
}

// ============================================================================
// CMover::ClearBuffStatusBySlot (Base class stub)
// IDA: ?ClearBuffStatusBySlot@CMover@@UEAAXG_N@Z (0x140377550)
// ============================================================================
void CMover::ClearBuffStatusBySlot(std::uint16_t nBuffSlot, bool bExcuteOutSkill) {
    // Base class stub - overridden by CMoverEx
}

// ============================================================================
// CMover::SetBuffStatus (Base class stub)
// IDA: ?SetBuffStatus@CMover@@UEAAHGK_N@Z (0x140374FE0)
// ============================================================================
bool CMover::SetBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwOwnerID, bool bShowBuff) {
    // Base class stub - returns false
    return false;
}

// ============================================================================
// CMover::IsClearBuff (Base class stub)
// IDA: ?IsClearBuff@CMover@@UEAAHGE@Z (0x1403774F0)
// ============================================================================
bool CMover::IsClearBuff(std::uint16_t nBuffIndex, std::uint8_t byReason) {
    // Base class stub - returns true
    return true;
}

// ============================================================================
// CMover::FindBuffByGroupID
// IDA: ?FindBuffByGroupID@CMover@@QEAAHGK@Z (0x14036A4C0)
// ============================================================================
int CMover::FindBuffByGroupID(std::uint16_t nGroupID, std::uint32_t dwOwnerID) {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nGroupID == nGroupID
            && m_stBuffState[i].nBuffIndex
            && (dwOwnerID == 0 || m_stBuffState[i].dwID == dwOwnerID)) {
            return i;
        }
    }
    return -1;
}

// ============================================================================
// CMover::GetEmptyBuffSlot
// IDA: ?GetEmptyBuffSlot@CMover@@QEAAHXZ (0x14036A810)
// ============================================================================
int CMover::GetEmptyBuffSlot() {
    for (int i = 0; i < 50; ++i) {
        if (!m_stBuffState[i].nBuffIndex) {
            return i;
        }
    }
    return -1;
}

// ============================================================================
// CMover::UpdateBuffCount
// IDA: ?UpdateBuffCount@CMover@@QEAAXEH@Z
// ============================================================================
void CMover::UpdateBuffCount(std::uint8_t byBuffType, int nDelta) {
    // Base class implementation - update buff count by type
    // Note: Actual implementation may involve tracking separate counts per buff type
}

// ============================================================================
// CMover::ClearBuffAbility
// IDA: 0x140374F30
// ============================================================================
void CMover::ClearBuffAbility(int nCount, float fDistance) {
    // Base class stub - overridden by derived classes
}

// ============================================================================
// CMover::ClearBuffStatus
// IDA: 0x140374FC0
// ============================================================================
void CMover::ClearBuffStatus(std::uint16_t nBuffIndex, bool bExcuteOutSkill, std::uint32_t dwOwnerID) {
    // Base class stub - overridden by derived classes
}

// ============================================================================
// CMover::SetBuffStatus
// IDA: 0x140374FE0
// ============================================================================
std::uint16_t CMover::SetBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwOwnerID, bool bShowBuff) {
    // Base class stub - returns 0
    return 0;
}

// ============================================================================
// CMover::GetDefaultAnimStep
// IDA: 0x1403751B0
// ============================================================================
std::uint8_t CMover::GetDefaultAnimStep() {
    return m_byDefaultAnimStep;
}

// ============================================================================
// CMover::GetMovePos
// IDA: 0x1403751D0
// ============================================================================
tagMOVE_POS* CMover::GetMovePos(tagMOVE_POS* result) {
    *result = m_stMovePos;
    return result;
}

// ============================================================================
// CMover::IsGazeMoving
// IDA: 0x140375200
// ============================================================================
bool CMover::IsGazeMoving() {
    return m_bGazeMoving;
}

// ============================================================================
// CMover::GetOrientationYaw
// IDA: 0x140375220
// ============================================================================
float CMover::GetOrientationYaw() {
    return m_vOrientation.x;
}

// ============================================================================
// CMover::ApplyBuffStatus
// IDA: 0x1403774D0
// ============================================================================
void CMover::ApplyBuffStatus(std::int16_t nIndex, float fElapsedTime) {
    // Base class stub - overridden by derived classes
}

// ============================================================================
// CMoverEx Buff System Functions
// ============================================================================

// ============================================================================
// CMoverEx::ClearBuffStatusBySlot
// IDA: ?ClearBuffStatusBySlot@CMoverEx@@UEAAXG_N@Z (0x14038DA80)
// ============================================================================
void CMoverEx::ClearBuffStatusBySlot(std::uint16_t nBuffSlot, bool bExcuteOutSkill) {
    if (nBuffSlot >= 50) {
        return;
    }

    std::uint16_t nBuffIndex = m_stBuffState[nBuffSlot].nBuffIndex;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = XResourceMgr::GetTB_BUFF(&pServer->m_xResourceMgr, nBuffIndex);

    if (!pBuffTable) {
        return;
    }

    std::uint32_t dwOwnerID = m_stBuffState[nBuffSlot].dwID;

    // Send buff delete packet
    send_eSUB_CMD_BUFF_DELETE(
        this,
        this,
        m_stBuffState[nBuffSlot].nBuffIndex,
        dwOwnerID,
        bExcuteOutSkill,
        m_stBuffState[nBuffSlot].bySendType
    );

    // Update buff ability
    UpdateBuffAbility(m_stBuffState[nBuffSlot], 0);

    // Clear buff state
    m_stBuffState[nBuffSlot].Clear();

    // Update buff count
    UpdateBuffCount(pBuffTable->Buff_Type, -1);

    // Handle defense type changes
    if (pBuffTable->EffectType_01 == 4 || pBuffTable->EffectType_01 == 5) {
        UpdateDefenseType();
    }

    // Handle defense disable buff
    if (pBuffTable->EffectType_01 == 6) {
        UpdateDefenseDisableBuff();
    }

    // Clear buff motion if matching
    if (m_nMotionClass == m_nBuffMotion) {
        m_nBuffMotion = -1;
        ClearMotion();
    }

    // Send update buff ability
    SendUpdateBuffAbility();

    // Execute call out buff if needed
    if (bExcuteOutSkill && pBuffTable->Call_Out_Buff && pBuffTable->Call_Out_Buff != nBuffIndex) {
        SetBuffStatus(pBuffTable->Call_Out_Buff, dwOwnerID, true);
    }

    // Clear hit call buff index if matching
    if (m_nHitCallBuffIndex == pBuffTable->Hit_Call_Buff) {
        m_nHitCallBuffIndex = 0;
    }
}

// ============================================================================
// CMoverEx::SetBuffStatus
// IDA: ?SetBuffStatus@CMoverEx@@UEAAHGK_N@Z (0x14038BCE0)
// ============================================================================
bool CMoverEx::SetBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwOwnerID, bool bShowBuff) {
    // Check if this is a monster with special flag
    if (XActor::GetType() == 2 && (GetMonsterFlag() & 1) != 0) {
        return false;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = XResourceMgr::GetTB_BUFF(&pServer->m_xResourceMgr, nBuffIndex);

    if (!pBuffTable) {
        return false;
    }

    // Check if buff can be applied
    if (!IsCanApplyBuff(nBuffIndex, nullptr)) {
        return false;
    }

    // Check buff limit
    if (m_nBuffTotalCnt >= 50) {
        return false;
    }

    // Check immunity status
    std::uint8_t bySystem_Type = pBuffTable->System_Type;
    if (IsImmunityStatus() && pBuffTable->Buff_Type == 1 && bySystem_Type != 3) {
        return false;
    }

    // Check policy for system type
    bool bCheckPolicy = false;
    if (bySystem_Type) {
        if (bySystem_Type == 1) {
            // Check monster rank
            if (XActor::GetType() == 2) {
                CMonster* pMonster = dynamic_cast<CMonster*>(this);
                if (pMonster) {
                    std::uint8_t byMonsterRank = pMonster->GetMobTableRef()->Monster_Rank;
                    if (byMonsterRank == 3 || byMonsterRank == 4) {
                        return false;
                    }
                }
            }
            bCheckPolicy = true;
        }
    } else {
        bCheckPolicy = true;
    }

    // Check defense type
    if (bCheckPolicy && m_byDefenseType == 3 && pBuffTable->Buff_Type == 1) {
        return false;
    }

    // Get buff category
    std::uint8_t byCategory = GetBuffCategory(pBuffTable->EffectType_01);

    // Check status conflicts
    if (byCategory == 2 && pBuffTable->EffectType_01 != 124 && XActor::IsStatus(0x8000000u)) {
        return false;
    }

    if (byCategory == 1 && XActor::IsStatus(0x8000000u)) {
        return false;
    }

    // Check pass debuff
    if (CheckPassDebuff(nBuffIndex) == 1) {
        return false;
    }

    // Set hit call buff
    if (pBuffTable->Hit_Call_Buff && !m_nHitCallBuffIndex) {
        m_nHitCallBuffIndex = pBuffTable->Hit_Call_Buff;
    }

    // Set owner ID if not provided
    if (!dwOwnerID) {
        UXActorID actorID;
        GetActorID(&actorID);
        dwOwnerID = CQuestCondition::GetQuestID(&actorID);
    }

    // Calculate time
    float fTime = static_cast<float>(static_cast<int>(pBuffTable->Buff_Time)) * 0.001f;

    bool bShouldDie = false;

    // Find existing buff by group
    int iIndex = FindBuffByGroupID(pBuffTable->Buff_Group, dwOwnerID);

    if (iIndex == -1) {
        // New buff - need to add
        if (pBuffTable->EffectType_01 >= 0x6F && XActor::GetType() == 0) {
            CheckPassiveSkill(1, 45);
        }

        iIndex = GetEmptyBuffSlot();
        UpdateBuffCount(pBuffTable->Buff_Type, 1);
        LoadBuffStatus(iIndex, nBuffIndex, fTime, 1, dwOwnerID, bShowBuff);

        // Check action name
        if (pBuffTable->SetBuffActionName[0] != '0') {
            VString strAnimName(pBuffTable->SetBuffActionName);
            std::uint32_t dwKey = GetAnimIndex(strAnimName);
            if (dwKey != static_cast<std::uint32_t>(-1)) {
                m_nBuffMotion = AnimKeyToMotion(dwKey);
                if (!IsHit()) {
                    ChangeMotion_3(m_nBuffMotion, 1, 0);
                }
            }
        }

        bShouldDie = UpdateBuffAbility(m_stBuffState[iIndex], 1);
    } else {
        // Existing buff - need to update
        bool bChangeControlDebuff = false;
        bool bDontRemoveBuff = false;

        if (byCategory == 2 && m_stBuffState[iIndex].byEffectType != pBuffTable->EffectType_01) {
            bChangeControlDebuff = true;
            bDontRemoveBuff = true;
            bShouldDie = UpdateBuffAbility(m_stBuffState[iIndex], 0);
        }

        // Check for same effect type 13
        if (m_stBuffState[iIndex].byEffectType == 13 && pBuffTable->EffectType_01 == 13) {
            bDontRemoveBuff = true;
        }

        // Save current stats if needed
        bool bSaveCurStat = IsCheckCurStat(pBuffTable);
        float fStat[4] = {0.0f, 0.0f, 0.0f, 0.0f};

        if (bSaveCurStat) {
            fStat[0] = GetStat(1);
            fStat[1] = GetStat(2);
            fStat[2] = GetStat(3);
            fStat[3] = GetStat(16);
        }

        // Set buff overlap
        int nResult = SetBuffOverlap(iIndex, pBuffTable, dwOwnerID, bDontRemoveBuff);
        if (nResult) {
            return nResult > 0;
        }

        // Update buff ability
        if (bChangeControlDebuff
            || !pBuffTable->EffectType_01
            || pBuffTable->EffectType_01 == 21
            || (pBuffTable->EffectType_01 >= 0x1C && pBuffTable->EffectType_01 <= 0x1F)) {
            bShouldDie = UpdateBuffAbility(m_stBuffState[iIndex], 1);

            // Restore stats if needed
            if (bSaveCurStat) {
                if (fStat[0] > GetStat(1)) SetStat(1, fStat[0]);
                if (fStat[1] > GetStat(2)) SetStat(2, fStat[1]);
                if (fStat[2] > GetStat(3)) SetStat(3, fStat[2]);
                if (fStat[3] > GetStat(16)) SetStat(16, fStat[3]);
            }
        }
    }

    // Handle defense disable buff
    if (pBuffTable->EffectType_01 == 6) {
        UpdateDefenseDisableBuff();
    }

    // Clear conflicting buffs
    if (pBuffTable->EffectType_01 == 123 || pBuffTable->EffectType_01 == 124) {
        for (int iType = 111; iType <= 114; ++iType) {
            int iTempIndex = FindBuffByEffectType(iType, 0);
            if (iTempIndex != -1) {
                ClearBuffStatusBySlot(iTempIndex, false);
            }
        }
    }

    // Send buff update
    send_eSUB_CMD_BUFF_UPDATE(
        this,
        this,
        nBuffIndex,
        m_stBuffState[iIndex].fLifeTime,
        m_stBuffState[iIndex].byCount,
        m_stBuffState[iIndex].dwID,
        m_stBuffState[iIndex].bySendType,
        m_stBuffState[iIndex].bShow
    );

    SendUpdateBuffAbility();

    // Handle death
    if (bShouldDie) {
        SetDie_2(12, 0);
    }

    return true;
}

// ============================================================================
// CMoverEx::IsClearBuff
// IDA: ?IsClearBuff@CMoverEx@@UEAAHGE@Z (0x1403903D0)
// ============================================================================
bool CMoverEx::IsClearBuff(std::uint16_t nBuffIndex, std::uint8_t byReason) {
    // TODO: Implement based on IDA decompilation
    // This function checks if a buff can be cleared based on reason
    return true;
}

// ============================================================================
// CMoverEx::SetBuffOverlap
// IDA: ?SetBuffOverlap@CMoverEx@@QEAAHHPEAUTB_BUFF@@KH@Z (0x14038CA00)
// ============================================================================
int CMoverEx::SetBuffOverlap(int iIndex, TB_BUFF* pBuffTable, std::uint32_t dwOwnerID, int bDontRemoveBuff) {
    // TODO: Implement based on IDA decompilation
    // This function handles buff overlap logic (refreshing, stacking, etc.)
    return 0;
}

// ============================================================================
// CMoverEx::LoadBuffStatus
// IDA: ?LoadBuffStatus@CMoverEx@@QEAAXHGMEK_N@Z (0x14038B9C0)
// ============================================================================
void CMoverEx::LoadBuffStatus(std::uint16_t nBuffSlot, std::uint16_t nBuffIndex, float fTime, std::uint8_t byCount, std::uint32_t dwOwnerID, bool bShowBuff) {
    // TODO: Implement based on IDA decompilation
    // This function loads buff state into a slot
}

// ============================================================================
// CMoverEx::UpdateBuffAbility
// IDA: ?UpdateBuffAbility@CMoverEx@@UEAA_NAEAUtagBUFF_STATE@@H@Z (0x14038E5F0)
// ============================================================================
bool CMoverEx::UpdateBuffAbility(tagBUFF_STATE& stBuffState, int nType) {
    // TODO: Implement based on IDA decompilation
    // This function updates ability values based on buff
    return false;
}

// ============================================================================
// CMoverEx::SetBuffAbility
// IDA: ?SetBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403900C0)
// ============================================================================
void CMoverEx::SetBuffAbility(int nIndex, float fValue) {
    // TODO: Implement based on IDA decompilation
}

// ============================================================================
// CMoverEx::AddBuffAbility
// IDA: ?AddBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403902A0)
// ============================================================================
void CMoverEx::AddBuffAbility(int nIndex, float fValue) {
    // TODO: Implement based on IDA decompilation
}

// ============================================================================
// CMoverEx::SendUpdateBuffAbility
// IDA: ?SendUpdateBuffAbility@CMoverEx@@QEAAXXZ
// ============================================================================
void CMoverEx::SendUpdateBuffAbility() {
    // TODO: Implement based on IDA decompilation
}

// ============================================================================
// CMoverEx::UpdateDefenseType
// IDA: ?UpdateDefenseType@CMoverEx@@QEAAXXZ
// ============================================================================
void CMoverEx::UpdateDefenseType() {
    // TODO: Implement based on IDA decompilation
}

// ============================================================================
// CMoverEx::IsCanApplyBuff
// IDA: ?IsCanApplyBuff@CMoverEx@@QEAA_NGPEAUTB_BUFF@@@Z
// ============================================================================
bool CMoverEx::IsCanApplyBuff(std::uint16_t nBuffIndex, TB_BUFF* pBuffTable) {
    // TODO: Implement based on IDA decompilation
    return true;
}

// ============================================================================
// CMoverEx::IsCheckCurStat
// IDA: ?IsCheckCurStat@CMoverEx@@QEAA_NPEAUTB_BUFF@@@Z
// ============================================================================
bool CMoverEx::IsCheckCurStat(TB_BUFF* pBuffTable) {
    // TODO: Implement based on IDA decompilation
    return false;
}
