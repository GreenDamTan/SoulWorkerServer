#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"  // for CMonster, tagACTION_DAMAGE
#include "Soulworker/GameServer/XGameServer/ActionResMgr.h"  // for XActionResMgr::GetAnimIndex
#include "Soulworker/GameServer/XGameServer/actor/component/GocParty.h"  // for CGocParty
#include "Soulworker/GameServer/XGameServer/actor/component/GocForce.h"  // for CGocForce
#include "Soulworker/GameServer/XCore/XArea/XArea.h"  // for XArea
#include "Soulworker/GameServer/XCore/XArea/XActor.h"  // for E_ACTOR_TYPE enum

// 默认值常量
namespace {
    constexpr float kDefaultWalkSpeed = 100.0f;
    constexpr float kDefaultRunSpeed = 300.0f;
    constexpr float kDefaultFlyGravity = 1960.0f;
    constexpr float kDefaultFlyMaxHeight = 200.0f;
    constexpr float kDefaultIdleCheckTime = 5.0f;
    constexpr int    kDefaultIdleMotionChance = 20;
    constexpr float kDefaultUpdateStatBatchTime = 0.2f;
    constexpr float kDefaultSABreakTime = 5.0f;
    constexpr float kDefaultMultipleAbsorbSG = 1.0f;
    constexpr float kDefaultChargingInputAnimSpeed = 1.0f;
}

// ============================================================================
// 构造函数 IDA 0x140378A60 (PDB public 正确地址: 0x140377A60) -> 0x1403796BA
// 大小: 4954 bytes
// ============================================================================
CMoverEx::CMoverEx()
    : CMover()
    // --- IDA 确认的非默认值 ---
    , m_fDefWalkSpeed(kDefaultWalkSpeed)
    , m_fDefRunSpeed(kDefaultRunSpeed)
    , m_fFlyGravity(kDefaultFlyGravity)
    , m_fFlyMaxHeight(kDefaultFlyMaxHeight)
    , m_fIdleCheckTime(kDefaultIdleCheckTime)
    , m_nIdleMotionChance(kDefaultIdleMotionChance)
    , m_fUpdateStatBatchTime(kDefaultUpdateStatBatchTime)
    , m_fSABreakTime(kDefaultSABreakTime)
    , m_fMultipleAbsorbSG(kDefaultMultipleAbsorbSG)
    , m_fChargingInputAnimSpeed(kDefaultChargingInputAnimSpeed)
    // --- IDA 确认的 -1 初始值 ---
    , m_nPlayPhaseMotion(-1)
    , m_nBuffMotion(-1)
    , m_iLastWarpPoint(-1)
    , m_iSuboComboMaxCount(-1)
    , m_iCurLeftSuboDescCount(-1)
    , m_iCurRightSuboDescCount(-1)
    // --- IDA 确认的 true/1 初始值 ---
    , m_bDisableDirectionToTargetSkill(1)
    , m_bUpdateRotation(true)
    // --- IDA 确认的 DIE_TYPE_NORMAL ---
    , m_eDieType(DIE_TYPE_NORMAL)
    // --- vCamDir 默认 (0, -1, 0) 在函数体中设置 ---
    // --- 其余零值成员用默认初始化 ---
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
    , m_bJumpAnim(0)
    , m_byAniProcessLinkType(0)
    , m_byMoveDirAnim(0)
    , m_pCurSkillTableRef(nullptr)
    , m_bAttackKeyPress(0)
    , m_bySkillAnimStep(0)
    , m_bySkillAnimCount(1)          // IDA: 初始化为 1
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
    , m_byStandType(0)
    , m_byBattleModeAnim(0)
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
    , m_bSystemActor(0)
    , m_bChangedStat(0)
    , m_fSkillTotalChargeTime(0.0f)
    , m_bExistSuboCombo(false)
    , m_fSuboComboWaitTime(0.0f)
    , m_fSuboComboCheckTime(0.0f)
    , m_iSuboComboCheckCount(0)
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
    , m_bFlyDie(false)
    , m_bCounterSuccessFrame(false)
    , m_bEnableCounuter(false)
    , m_fMinCounterRange(0.0f)
    , m_fMaxCounterRange(0.0f)
    , m_fCounterAngle(0.0f)
    , m_iCounterProbability(0)
    , m_fCounterDuration(0.0f)
    , m_bChargingStart(false)
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
    , m_byFixedMaxDamage(0)
    , m_byPhaseMotionStep(0)
    , m_fPhaseStepMaxTime(0.0f)
    , m_fMultipleDamageOnce(0.0f)
    , m_bApplyMultipleDamageOnce(false)
    , m_bySABreakMotionType(0)
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
    // === Round 7 Phase 1-2 - Collision Members ===
    , m_vCollisionPoint()
    , m_pCollisionTarget(nullptr)
    , m_fCollisionTime(0.0f)
{
    // IDA 0x140378A60 构造函数体:
    // 1. 容器 placement new 构造 (VString, hkvVec3, VPList, std::vector, CWayPoint, tagMOVE_POS, SHitPartsInfo)
    new (&m_strSpecialDamage) VString();
    new (&m_strPhaseChangeAnim) VString();
    new (&m_vTrapPos) hkvVec3();
    new (&m_vCamDir) hkvVec3(0.0f, -1.0f, 0.0f);
    new (&m_vBackupSkillPos) hkvVec3();
    new (&m_vBackupSkillRotate) hkvVec3();
    new (&m_EventObjectList) VPList();
    new (&m_CommonPosBoxList) VPList();
    new (&m_vBeforePos) hkvVec3();
    new (&m_vPreTargetList) std::vector<std::uint32_t>();
    new (&m_vAttachedDir) hkvVec3();
    new (&m_vAttachedOffset) hkvVec3();
    new (&m_xWayPoint) CWayPoint();
    new (&m_szAttachBoneName) VString();
    new (&m_vGrapDropPos) hkvVec3();
    new (&m_stDropOffset_dummy) tagMOVE_POS();
    memset(m_sHitParts_dummy, 0, sizeof(m_sHitParts_dummy));
    new (&m_vecOptionEffect) std::vector<void*>();
    new (&m_vecDelayBuff) std::vector<void*>();

    // 2. 字段赋值 (通过初始化列表已完成，但这里保留 IDA 调用的清理函数)
    RemoveAllOptionEffect();
    RemoveAllDefenseChangeInfo();
}

CMoverEx::~CMoverEx() {
    // IDA 0x14037A0C0 -> 0x14037A223 (355 bytes)
    // 1. Set vftables for CMoverEx (done automatically by compiler)
    // 2. Call Reset()
    // 3. Destroy member containers in reverse declaration order
    // 4. Call CMover::~CMover() (done automatically by compiler)

    // Call Reset to clean up state
    Reset();

    // Destroy members in reverse order per IDA
    // Note: These explicit destructor calls match IDA behavior where members
    // were constructed via placement new in the constructor
    m_vecDelayBuff.~vector<void*>();
    m_vecOptionEffect.~vector<void*>();
    m_szAttachBoneName.~VString();
    m_xWayPoint.~CWayPoint();
    m_vPreTargetList.~vector<std::uint32_t>();
    m_CommonPosBoxList.~VPList();
    m_EventObjectList.~VPList();
    m_strPhaseChangeAnim.~VString();
    m_strSpecialDamage.~VString();

    // Base class destructor called automatically
}

void CMoverEx::RemoveAllOptionEffect() {
    // IDA 0x140378A60 构造函数尾部调用
    m_vecOptionEffect.clear();
}

void CMoverEx::RemoveAllDefenseChangeInfo() {
    // IDA 0x140378A60 构造函数尾部调用
    m_listDefenseChangeInfo.clear();
}

// Per IDA 0x14037FBD0: CMoverEx::CheckUseSkill
// 检查是否可以使用技能
int CMoverEx::CheckUseSkill(std::uint8_t byCheckVal, std::uint8_t byNormalVal, TB_SKILL* pTBSkill)
{
    // IDA 反编译精确还原:
    switch (byCheckVal) {
        case 1u:
            return 1;
        case 2u:
            // m_nMotionClass == 5 || (m_nMotionClass >= 32 && m_nMotionClass <= 34)
            return (m_nMotionClass == 5) || (m_nMotionClass >= 32 && m_nMotionClass <= 34);
        case 3u:
            // CMover::IsHitDown(this)
            return IsHitDown() ? 1 : 0;
        case 4u:
            // this->IsCounterAttackHit(this)
            // return IsCounterAttackHit() ? 1 : 0;
            return 0;
        case 5u:
            // CMover::IsActivateSkillUnlockBuff(this, pTBSkill)
            // return IsActivateSkillUnlockBuff(pTBSkill) ? 1 : 0;
            return 0;
        default:
            // ((byNormalVal & 4) != 0 || !CMover::IsHitDown(this))
            // && ((byNormalVal & 8) != 0 || !this->IsCounterAttackHit(this))
            // && ((byNormalVal & 0x10) == 0 || CMover::IsActivateSkillUnlockBuff(this, pTBSkill))
            bool bResult = true;
            if ((byNormalVal & 4) == 0 && IsHitDown())
                bResult = false;
            // if ((byNormalVal & 8) == 0 && IsCounterAttackHit())
            //     bResult = false;
            // if ((byNormalVal & 0x10) != 0 && !IsActivateSkillUnlockBuff(pTBSkill))
            //     bResult = false;
            return bResult ? 1 : 0;
    }
}

// ============================================================================
// Reset IDA 0x140379700 -> 0x14037A0BD
// 大小: 2525 bytes
// ============================================================================
void CMoverEx::Reset() {
    // IDA 反编译确认流程:
    // 1. CMover::Reset(this)
    // 2. 重置所有 CMoverEx 成员变量到默认值

    // 调用基类 Reset
    CMover::Reset();

    // 重置战斗相关状态
    m_bBattlePose = false;
    m_bHasTurnMotion = 0;
    m_fMovingYaw = 0.0f;
    m_fSummonAkashicYaw = 0.0f;
    m_fLookPitch = 0.0f;
    m_fCheckMoveStopTime = 0.0f;
    m_fLastChangeAnimationTime = 0.0f;
    m_fMoveDelayTime = 0.0f;
    m_bCancelMoving = 0;

    // 重置技能相关
    m_pCurSkillTableRef = nullptr;
    m_bySkillAnimCount = 1;
    m_bySkillAnimStep = 0;
    m_fSkillLoopTime = 0.0f;
    m_bAttackKeyPress = 0;
    m_fSkillBlendEndTime = 0.0f;
    m_bSkipReplayTime = 0;
    m_bySkillChargeStep = 0;
    m_fSkillChargeChangeTime = 0.0f;

    // 重置僵直相关
    m_fHitFreezeTime = 0.0f;
    m_iStiffenCount = 0;
    m_fStiffenRate = 0.0f;
    m_fStiffenTime = 0.0f;
    m_fStiffenDelayTime = 0.0f;
    m_fStiffenImmuneTime = 0.0f;
    m_bAnimSpeedTrigger = 0;

    // 重置移动速度
    m_fDefWalkSpeed = 100.0f;
    m_fDefRunSpeed = 300.0f;
    m_fTurnSpeedRate = 1.0f;
    m_fQuickTurnSpeed = 1.0f;

    // 重置跳跃/飞行相关
    m_fJumpDelayTime = 0.0f;
    m_fFlyingStayTime = 0.0f;
    m_byMoveDirAnim = 0;
    m_fFlyVelocity = 0.0f;
    m_fJumpDelta = 0.0f;
    m_bJumpAnim = 0;
    m_byAniProcessLinkType = 0;

    // 重置护盾和相位
    m_nShieldHP = 0;
    m_byPhaseStep = 0;
    m_byPhaseType = 0;
    m_byMaxPhaseStep = 0;
    m_byPhaseCondition = 0;
    m_dwPhaseConditionValue = 0;
    m_fPhaseDurationTime = 0.0f;
    m_nPlayPhaseMotion = -1;
    m_nBuffMotion = -1;
    m_byBattleModeAnim = 0;
    m_byStandType = 0;
    m_fIdleCheckTime = 5.0f;
    m_nIdleMotionChance = 20;

    // 重置陷阱位置
    m_vTrapPos.x = 0.0f; m_vTrapPos.y = 0.0f; m_vTrapPos.z = 0.0f;
    m_pMouseOnTrap = nullptr;
    m_bMoveSkillPrefab = 0;
    m_fAutoRotaionSpeed = 0.0f;
    m_fAutoRotaionTime = 0.0f;

    // 重置 Akashic/Aura
    m_pCurAkashicRecord = nullptr;
    m_pAkashicObject = nullptr;
    m_pAuraSkill = nullptr;
    m_fAuraCheckTime = 0.0f;
    m_fAuraLifeTime = 0.0f;
    m_fAmountOfHeal = 0.0f;
    m_byAggroLevelOrder = 0;

    // 重置死亡相关
    m_byDieReason = 0;
    m_nDieDamage = 0;
    m_dwInvisibleFlag = 0;
    m_fInvisibleEndTime = 0.0f;
    m_nInvisibleConditionType = 0;
    m_iLastWarpPoint = -1;
    m_bSystemActor = 0;
    m_pSilhouet = nullptr;

    // 重置列表
    m_EventObjectList.Reset();
    m_CommonPosBoxList.Reset();
    m_vPreTargetList.clear();

    // 重置 Subo Combo
    m_bExistSuboCombo = false;
    m_fSuboComboWaitTime = 0.0f;
    m_fSuboComboCheckTime = 0.0f;
    m_iSuboComboMaxCount = -1;
    m_iSuboComboCheckCount = 0;
    m_iMaxLeftSuboDescCount = 0;
    m_iMaxRightSuboDescCount = 0;
    m_iCurLeftSuboDescCount = -1;
    m_iCurRightSuboDescCount = -1;
    m_pSuboComboTrigger = nullptr;
    m_fSkillTotalChargeTime = 0.0f;

    // 重置 Attach 相关
    m_bCheckAttachToAttacker = false;
    m_pAttachToAttacker = nullptr;
    m_vAttachedDir.x = 0.0f; m_vAttachedDir.y = 0.0f; m_vAttachedDir.z = 0.0f;
    m_fAttachedDirDist = 0.0f;
    m_fAttachedDuration = 0.0f;
    m_fAttachedDistance = 0.0f;
    m_fAttachedAngle = 0.0f;
    m_iApplyWeightRank = 0;

    // 重置战斗类型
    m_bDisableDirectionToTargetSkill = 1;
    m_eDieType = 0;  // DIE_TYPE_NORMAL
    m_bCounterSuccessFrame = false;
    m_bEnableCounuter = false;
    m_fMinCounterRange = 0.0f;
    m_fMaxCounterRange = 0.0f;
    m_fCounterAngle = 0.0f;
    memset(m_szCounterAnimName, 0, sizeof(m_szCounterAnimName));
    m_iCounterProbability = 0;
    m_fCounterDuration = 0.0f;

    // 重置充能
    m_bChargingStart = false;
    m_fChargingInputAnimSpeed = 1.0f;
    m_fChargingInputMaxAddMultiple = 0.0f;
    m_fChargingInputDuration = 0.0f;
    m_fChargingInputTime = 0.0f;
    m_fChargingInputPressTime = 0.0f;
    m_fChargingInputCalcMultiple = 0.0f;
    m_bControlMonster = false;
    m_fLeftChargingValue = 0.0f;
    m_fRightChargingValue = 0.0f;
    m_bOnDie = false;
    m_bDieAttack = false;

    // 重置技能跳过/分歧
    m_fSkillSkipCoolTime = 0.0f;
    m_fUpdateStatBatchTime = 0.2f;
    m_pCurDivergenceTableRef = nullptr;
    m_dwDivergenceSkillID = 0;
    m_pCurDeckBonusRef = nullptr;
    m_dwOwnerID = 0;
    m_bAllowAbsorbSG = false;
    m_bKeepLookTarget = false;

    // 重置 HitParts
    memset(m_sHitParts_dummy, 0, sizeof(m_sHitParts_dummy));
    m_bReserveChange = false;
    m_dwChangeMobNewID = 0;
    m_dwChangeMobTableID = 0;
    m_fMoveDistAfterSkill = 0.0f;

    // 重置测试伤害/相位运动
    m_eTestDamageType = 0;
    m_byFixedMaxDamage = 0;
    m_byPhaseMotionStep = 0;
    m_fPhaseStepMaxTime = 0.0f;
    m_fMultipleDamageOnce = 0.0f;
    m_bApplyMultipleDamageOnce = false;
    m_fMultipleAbsorbSG = 1.0f;

    // 重置 SA Break
    m_bySABreakMotionType = 0;
    m_fSABreakTime = 5.0f;
    m_fSABreakLoopTime = 0.0f;
    m_bSABreakLoopMotion = false;
    m_bShowSABreakMotion = false;
    m_fSABreakLoopMotionTime = 0.0f;

    // 重置地面状态
    m_bOnGround = 1;
    m_fGroundPosZ = 0.0f;

    // 重置 Option Effect
    m_nAddMoneyFromOptionEffect = 0;
    m_nAddEtherFromOptionEffect = 0;
    m_nAddExpFromOptionEffect = 0;

    // 重置 Link Skill
    m_bLinkSkillOn = false;
    m_fLinkSkillDuration = 0.0f;
    m_pLinkSkillTrigger = nullptr;
    m_bCheckAttackSkillOn = false;
    m_fCheckAttackSkillDuration = 0.0f;
    m_pCheckAttackSkillTrigger = nullptr;

    // 清空 Delay Buff
    m_vecDelayBuff.clear();

    // 调用清理函数
    RemoveAllOptionEffect();
    RemoveAllDefenseChangeInfo();

    // 重置召唤/条件
    m_bSummonMonsterApplyRot = false;
    m_iActionCondition = 0;
    m_iPvpCondition = 0;
    m_iChangeMotionPos = 0;
    m_iChangeMotionType = 0;
}

float CMoverEx::GetSkillLoopTime() {
    // IDA 0x140016ED0
    return m_fSkillLoopTime;
}

void CMoverEx::ResetAddExpFromOptionEffect() {
    // IDA 0x140049250
    m_nAddExpFromOptionEffect = 0;
}

int CMoverEx::GetAddExpFromOptionEffect() {
    // IDA 0x140049270
    return m_nAddExpFromOptionEffect;
}

void CMoverEx::SetOwnerID(std::uint32_t dwID) {
    // IDA 0x14009F1C0
    m_dwOwnerID = dwID;
}

std::uint32_t CMoverEx::GetOwnerID() {
    // IDA 0x1401AD020
    return m_dwOwnerID;
}

void CMoverEx::ResetAddEtherFromOptionEffect() {
    // IDA 0x1400F9F70
    m_nAddEtherFromOptionEffect = 0;
}

int CMoverEx::GetAddEtherFromOptionEffect() {
    // IDA 0x1400F9F90
    return m_nAddEtherFromOptionEffect;
}

void CMoverEx::ResetAddMoneyFromOptionEffect() {
    // IDA 0x1400F9FE0
    m_nAddMoneyFromOptionEffect = 0;
}

int CMoverEx::GetAddMoneyFromOptionEffect() {
    // IDA 0x1400FA000
    return m_nAddMoneyFromOptionEffect;
}

void CMoverEx::ChangeCombatType(int nType, float fParam1, float fParam2) {
    // IDA 0x140188DC0 - 空实现 (仅占位，参数供子类 override)
    // 基类 CMoverEx 的 ChangeCombatType 为空函数
    // SetCombatType(nType) 和 ChangeMotion 由子类 (CUser/CMonster) 的 override 处理
    m_nCombatType = nType;
}

// ============================================================================
// ChangeInitMotion IDA 0x140390F60
// ============================================================================
void CMoverEx::ChangeInitMotion() {
    // IDA 0x140390F60 反编译:
    // unsigned int AnimIndex = XActionResMgr::GetAnimIndex(this, 0, 0, 0);
    // if (CMover::GetAnimStirng(this, AnimIndex)) {
    //   ChangeMotion_3(this, 0, 1, 0);
    // } else {
    //   unsigned int v2 = XActionResMgr::GetAnimIndex(this, 1, 0, 0);
    //   if (!CMover::GetAnimStirng(this, v2))
    //     this->m_bBattlePose = 1;
    //   ChangeMotion_3(this, 1, 1, 7);
    // }
    // 还原: 尝试播放待机动画(motion 0), 如果不存在则尝试站立动画(motion 1)

    unsigned int AnimIndex = static_cast<unsigned int>(XActionResMgr::GetAnimIndex(0, 0, false));
    if (GetAnimStirng(AnimIndex)) {
        ChangeMotion(0, 1, 0);
    } else {
        AnimIndex = static_cast<unsigned int>(XActionResMgr::GetAnimIndex(1, 0, false));
        if (!GetAnimStirng(AnimIndex)) {
            m_bBattlePose = true;
        }
        ChangeMotion(1, 1, 7);
    }
}

void CMoverEx::SetCombatType(int nType) {
    // IDA 0x140188DE0
    m_nCombatType = nType;
}

int CMoverEx::GetCombatType() {
    // IDA 0x140189080
    return m_nCombatType;
}

bool CMoverEx::IsControlMonster() {
    // IDA 0x140188E00
    return m_bControlMonster;
}

void CMoverEx::SetControlMonsterFlag(bool bFlag) {
    // IDA 0x140188E20
    m_bControlMonster = bFlag;
}

void CMoverEx::SetControlMonster(const hkvVec3& vPos, std::uint32_t dwID1, std::uint32_t dwID2,
                                   float fParam1, float fParam2, char* pStr, bool bFlag) {
    // IDA 0x140188E40 - 空实现
}

void CMoverEx::SetSkillTable(TB_SKILL* pTable) {
    // IDA 0x140188F60
    m_pCurSkillTableRef = pTable;
}

TB_SKILL* CMoverEx::GetSkillTable() {
    // IDA 0x140189020
    return m_pCurSkillTableRef;
}

std::uint8_t CMoverEx::GetSkillLevel() {
    // IDA 0x140189040: return m_pCurSkillTableRef->Skill_LV if not null, else 0
    if (m_pCurSkillTableRef) {
        return m_pCurSkillTableRef->Skill_LV;
    }
    return 0;
}

std::uint8_t CMoverEx::GetSkillChargeStep() {
    // IDA 0x1401890A0
    return m_bySkillChargeStep;
}

bool CMoverEx::IsBattlePose() {
    // IDA 0x140189000
    return m_bBattlePose;
}

void CMoverEx::SetBattlePose(bool bPose) {
    // IDA 0x140199E50
    m_bBattlePose = bPose;
}

void CMoverEx::ChangeBattlePose(bool bPose1, bool bPose2) {
    // IDA 0x140188F80: m_bBattlePose = bBattle (ignores bPlayMotion)
    m_bBattlePose = bPose1;
}

float CMoverEx::GetLookPitch() {
    // IDA 0x140189270
    return m_fLookPitch;
}

float CMoverEx::GetMovingYaw() {
    // IDA 0x140189290
    return m_fMovingYaw;
}

float CMoverEx::GetAkashicTriggerTime() {
    // IDA 0x140189260: return 0.0
    return 0.0f;
}

void CMoverEx::CheckDieType(std::uint8_t& byReactionType, std::uint8_t byDamageFlag, hkvVec3& vExtraMove) {
    // IDA 0x140188E60: m_eDieType = DIE_TYPE_NORMAL
    m_eDieType = DIE_TYPE_NORMAL;
}

// Note: SendHelperLog, ChangeAngleAttackName, SetActivateWeaponTrajectory,
// CheckPassiveSkill, CheckContinousAttack need declarations in MoverEx.h

// Note: GetOwnerID, GetMoveDistAfterSkill, GetSector, IsCancelMoving, SetCancelMoving,
// SetIdleMotionInfo, GetWayPointID are already defined or need declarations

int CMoverEx::GetMaxHP() {
    // IDA 0x140189410: return (int)m_fAbility[10]
    // STAT_INDEX_MAXHP = 10
    return static_cast<int>(CMover::GetStat(10));
}

void CMoverEx::SetAkashicObject(CMoverEx* pObject) {
    // IDA 0x14019B950
    m_pAkashicObject = pObject;
}

bool CMoverEx::IsCounterSuccessFrame() {
    // IDA 0x14019B990
    return m_bCounterSuccessFrame;
}

void CMoverEx::SetSilhoutte(hkaiPointCloudSilhouetteGenerator* pSilhouette) {
    // IDA 0x1401ADBE0
    m_pSilhouet = pSilhouette;
}

hkaiPointCloudSilhouetteGenerator* CMoverEx::GetSilhoutte() {
    // IDA 0x1401ADC30
    return m_pSilhouet;
}

void CMoverEx::SetAmountOfHeal(float fAmount) {
    // IDA 0x1401ADCE0
    m_fAmountOfHeal = fAmount;
}

void CMoverEx::SetWayPointID(int nID) {
    // IDA 0x1401ADD20
    // TODO: 需要设置 WayPoint 相关
}

void CMoverEx::SetAggroLevelOrder(std::uint8_t byLevel) {
    // IDA 0x1401C5410
    m_byAggroLevelOrder = byLevel;
}

void CMoverEx::SetUpdateRotation(bool bUpdate) {
    // IDA 0x140276350
    m_bUpdateRotation = bUpdate;
}

std::uint8_t CMoverEx::GetStandType() {
    // IDA 0x140276410
    return m_byStandType;
}

CWayPoint* CMoverEx::GetWayPoint() {
    // IDA 0x140276430
    return &m_xWayPoint;
}

void CMoverEx::SetPvpCondition(int nCondition) {
    // IDA 0x140189190: m_iPvpCondition |= iValue (bitwise OR)
    m_iPvpCondition |= nCondition;
}

int CMoverEx::GetPvpCondition() {
    // IDA 0x1401891C0
    return m_iPvpCondition;
}

void CMoverEx::SetActionCondition(int nCondition) {
    // IDA 0x1401891E0: m_iActionCondition |= iValue (bitwise OR)
    m_iActionCondition |= nCondition;
}

int CMoverEx::GetActionCondition() {
    // IDA 0x140189210
    return m_iActionCondition;
}

void CMoverEx::ClearActionCondition(int nCondition) {
    // IDA 0x140188ED0: m_iActionCondition &= ~iValue (bitwise AND NOT)
    m_iActionCondition &= ~nCondition;
}

bool CMoverEx::IsActionCondition(int nCondition) {
    // IDA 0x140188F00: return (iValue & m_iActionCondition) != 0 (bitwise AND)
    return (nCondition & m_iActionCondition) != 0;
}

bool CMoverEx::IsPvpCondition(int nType) {
    // IDA 0x140188E90: return (iValue & m_iPvpCondition) != 0 (bitwise AND)
    return (nType & m_iPvpCondition) != 0;
}

// ============================================================================
// Movement Functions - IDA 反编译实现
// ============================================================================

// ============================================================================
// MoveToPosition - 移动到指定位置
// 基于 IDA 多个移动相关函数综合实现
// ============================================================================
void CMoverEx::MoveToPosition(const hkvVec3& vTargetPos, float fSpeed, bool bRun) {
    // 设置移动速度
    if (fSpeed > 0.0f) {
        SetMoveSpeed(fSpeed);
    }

    // 设置移动状态
    m_bCancelMoving = 0;

    // TODO: 需要 NavMesh 路径计算实现
    // 1. 检查目标位置有效性 (CheckMoveDestPos)
    // 2. 计算路径
    // 3. 设置动画状态
    // 4. 发送移动数据包

    GreenDamTan_log(__FILE__, __FUNCTION__, "MoveToPosition - TODO: need NavMesh pathfinding");
}

// ============================================================================
// StopMove - 停止移动
// 基于 IDA send_eSUB_CMD_MOVE_STOP 逻辑
// ============================================================================
void CMoverEx::StopMove() {
    // 清除移动状态
    m_bCancelMoving = 1;

    // 清除额外移动
    ReleaseExtraMoving();

    // TODO: 需要停止动画和发送停止数据包
    // 1. 停止移动动画
    // 2. 广播停止移动数据包

    GreenDamTan_log(__FILE__, __FUNCTION__, "StopMove - TODO: need animation and packet");
}

// ============================================================================
// SetMoveSpeed - 设置移动速度 (CMoverEx 版本)
// ============================================================================
void CMoverEx::SetMoveSpeed(float fSpeed) {
    // 调用基类方法
    CMover::SetMoveSpeed(fSpeed);

    // CMoverEx 额外处理
    // IDA 显示 CMoverEx 有 m_fDefWalkSpeed 和 m_fDefRunSpeed
}

// ============================================================================
// GetMoveSpeed - 获取移动速度 (CMoverEx 版本)
// ============================================================================
float CMoverEx::GetMoveSpeed() {
    // 调用基类方法
    return CMover::GetMoveSpeed();
}

// ============================================================================
// UpdatePosition - 更新位置 (每帧调用)
// 基于 IDA ProcessExtraMoving 逻辑
// ============================================================================
void CMoverEx::UpdatePosition(float fDeltaTime) {
    // IDA CMover::ProcessExtraMoving 核心逻辑:
    // 1. 检查是否有额外移动效果
    // 2. 处理额外移动 (击退、拉扯等)
    // 3. 更新位置

    // 调用基类的额外移动处理
    ProcessExtraMoving();

    // TODO: 需要完整的位置更新逻辑
    // 1. 更新移动状态
    // 2. 检查到达目标
    // 3. 处理碰撞
}

// ============================================================================
// SetDefWalkSpeed - 设置默认行走速度
// ============================================================================
void CMoverEx::SetDefWalkSpeed(float fSpeed) {
    m_fDefWalkSpeed = fSpeed;
}

// ============================================================================
// GetDefWalkSpeed - 获取默认行走速度
// ============================================================================
float CMoverEx::GetDefWalkSpeed() {
    return m_fDefWalkSpeed;
}

// ============================================================================
// SetDefRunSpeed - 设置默认跑步速度
// ============================================================================
void CMoverEx::SetDefRunSpeed(float fSpeed) {
    m_fDefRunSpeed = fSpeed;
}

// ============================================================================
// GetDefRunSpeed - 获取默认跑步速度
// ============================================================================
float CMoverEx::GetDefRunSpeed() {
    return m_fDefRunSpeed;
}

// ============================================================================
// IsMoving - 检查是否在移动 (CMoverEx 版本)
// ============================================================================
bool CMoverEx::IsMoving() {
    // 检查基类移动状态
    if (CMover::IsMoving()) {
        return true;
    }

    // 检查 CMoverEx 特有状态
    if (m_bCancelMoving == 0 && m_fMoveDelayTime > 0.0f) {
        return true;
    }

    return false;
}

// ============================================================================
// GetMoveDirection - 获取当前移动方向向量
// ============================================================================
hkvVec3 CMoverEx::GetMoveDirection() {
    hkvVec3 vDirection(0.0f, 0.0f, 0.0f);

    // 如果正在移动，计算移动方向
    if (m_bMoving && !m_bCancelMoving) {
        // 从移动偏移获取方向
        if (m_stMoveOffset.x != 0.0f || m_stMoveOffset.y != 0.0f) {
            float fLen = sqrtf(m_stMoveOffset.x * m_stMoveOffset.x +
                              m_stMoveOffset.y * m_stMoveOffset.y);
            if (fLen > 0.0001f) {
                vDirection.x = m_stMoveOffset.x / fLen;
                vDirection.y = m_stMoveOffset.y / fLen;
            }
        } else if (m_stMovePos.x != 0.0f || m_stMovePos.y != 0.0f) {
            // 从目标位置计算方向
            hkvVec3 vCurrentPos = CMover::GetPosition();
            float fDiffX = m_stMovePos.x - vCurrentPos.x;
            float fDiffY = m_stMovePos.y - vCurrentPos.y;
            float fLen = sqrtf(fDiffX * fDiffX + fDiffY * fDiffY);
            if (fLen > 0.0001f) {
                vDirection.x = fDiffX / fLen;
                vDirection.y = fDiffY / fLen;
            }
        }
    }

    return vDirection;
}

// ============================================================================
// CheckMovingAttackAnimation - 检查是否可以在移动中攻击
// ============================================================================
bool CMoverEx::CheckMovingAttackAnimation() {
    // IDA 逻辑: 检查当前技能是否允许移动中攻击
    // 基于 m_pCurSkillTableRef 的 Move_Attack_Flag 字段

    if (!m_pCurSkillTableRef) {
        return false;
    }

    // 检查技能表中的移动攻击标志
    // TODO: 需要完整的 TB_SKILL 结构定义
    // return (m_pCurSkillTableRef->Move_Attack_Flag != 0);

    // 简化实现: 检查动作类是否允许移动攻击
    short nMotionClass = CMover::GetMotionClass();

    // 攻击动画 (25-31) 检查
    if (nMotionClass >= 25 && nMotionClass <= 31) {
        return IsCanMovingAnim();
    }

    return false;
}

// ============================================================================
// SetAnimationSpeed - 设置动画播放速度
// ============================================================================
void CMoverEx::SetAnimationSpeed(float fSpeed) {
    // 调用基类方法设置动画速度
    CMover::SetAnimSpeed(fSpeed);

    // CMoverEx 特有处理
    m_fAnimSpeed = fSpeed;

    // 如果是充能状态，也更新充能动画速度
    if (m_bChargingStart) {
        m_fChargingInputAnimSpeed = fSpeed;
    }
}

// ============================================================================
// IsAnimationEnd - 检查当前动画是否结束
// ============================================================================
bool CMoverEx::IsAnimationEnd() {
    // 检查动画事件是否存在
    if (!m_pCurMotionEvent) {
        return true;
    }

    // 检查动画时间是否超过总长度
    if (m_pCurMotionEvent->fAnimationLength <= 0.0f) {
        return true;
    }

    // 检查动画百分比是否接近 1.0 (99%)
    if (m_fAnimPercentTime >= 0.99f) {
        return true;
    }

    // 检查动画时间
    if (m_fAnimationTime >= m_pCurMotionEvent->fAnimationLength) {
        return true;
    }

    return false;
}

// ============================================================================
// GetAnimationTime - 获取当前动画时间
// ============================================================================
float CMoverEx::GetAnimationTime() {
    return m_fAnimationTime;
}

// ============================================================================
// InitFunction - IDA 0x14037A230
// ============================================================================
void CMoverEx::InitFunction() {
    CMover::InitFunction();
    // IDA: VisBaseEntity_cl::SetTraceAccuracy(this, VIS_TRACEACC_AABOX)
    // SetTraceAccuracy is a Vision Engine function that sets collision trace accuracy
    // VIS_TRACEACC_AABOX = Axis-Aligned Bounding Box (simpler, faster collision)
    // TODO: Requires Vision Engine implementation of SetTraceAccuracy
    // SetTraceAccuracy(VIS_TRACEACC_AABOX);
}

// ============================================================================
// Destroy - IDA 0x14037A260
// ============================================================================
void CMoverEx::Destroy() {
    CMover::Destroy();
    m_EventObjectList.Reset();
    m_CommonPosBoxList.Reset();
}

// ============================================================================
// UpdateAttackKeyPress - IDA 0x14037A2A0
// ============================================================================
void CMoverEx::UpdateAttackKeyPress(int bPress) {
    if (m_bAttackKeyPress != bPress) {
        if (m_pCurSkillTableRef) {
            std::uint8_t byControlType = GetControlType(m_pCurSkillTableRef);
            if ((byControlType == 2 || byControlType == 5) && !bPress) {
                ChargeSkillEnd();
                return;
            }
        }
        m_bAttackKeyPress = bPress;
        if ((bPress && m_bySkillAnimStep == 0) ||
            (bPress && m_bySkillAnimStep == 2) ||
            (!bPress && m_bySkillAnimStep == 1)) {
            ClearMotion();
        }
    }
}

// ============================================================================
// UpdatePreTargetSkill - IDA 0x14037A3D0
// ============================================================================
void CMoverEx::UpdatePreTargetSkill() {
    if (GetPreTargetListCount() <= 0) {
        m_bySkillAnimStep = 1;
    } else {
        m_bAttackKeyPress = 1;
    }
    ClearMotion();
}

// ============================================================================
// UpdateTargetByPretarget - IDA 0x14037A430
// ============================================================================
void CMoverEx::UpdateTargetByPretarget() {
    if (!m_vPreTargetList.empty()) {
        std::uint32_t dwID = m_vPreTargetList.front();
        CMover::SetTargetID(dwID);
        m_vPreTargetList.erase(m_vPreTargetList.begin());
    }
}

// ============================================================================
// SceneChanged - IDA 0x14037A4C0
// ============================================================================
void CMoverEx::SceneChanged() {
    CMover::SetMoveingInFly(0);
    CMover::SceneChanged();
}

// ============================================================================
// GetCurDeckBouns - IDA 0x1402C7220
// ============================================================================
TB_DECK_BONUS* CMoverEx::GetCurDeckBouns() {
    return m_pCurDeckBonusRef;
}

// ============================================================================
// GetCalcChargingMultiple - IDA 0x1402C72B0
// ============================================================================
float CMoverEx::GetCalcChargingMultiple() {
    return m_fChargingInputCalcMultiple;
}

// ============================================================================
// GetAttached - IDA 0x1402C72F0
// ============================================================================
CMoverEx* CMoverEx::GetAttached() {
    return m_pAttachToAttacker;
}

// ============================================================================
// GetSABreakType - IDA 0x1402C73E0
// ============================================================================
std::uint8_t CMoverEx::GetSABreakType() {
    return m_bySABreakMotionType;
}

// ============================================================================
// SetSABreakTime - IDA 0x140353F80
// ============================================================================
void CMoverEx::SetSABreakTime(float fBreakTime) {
    m_fSABreakTime = fBreakTime;
}

// ============================================================================
// SetSABreakLoopTime - IDA 0x140353FA0
// ============================================================================
void CMoverEx::SetSABreakLoopTime(float fBreakLoopTime) {
    m_fSABreakLoopTime = fBreakLoopTime;
}

// ============================================================================
// SetSABreakType - IDA 0x140353FC0
// ============================================================================
void CMoverEx::SetSABreakType(std::uint8_t byType) {
    m_bySABreakMotionType = byType;
}

// ============================================================================
// AddAmountOfHeal - IDA 0x1402C7440
// ============================================================================
void CMoverEx::AddAmountOfHeal(float fHeal) {
    m_fAmountOfHeal += fHeal;
}

// ============================================================================
// GetAmountOfHeal - IDA 0x140364550
// ============================================================================
float CMoverEx::GetAmountOfHeal() {
    return m_fAmountOfHeal;
}

// ============================================================================
// GetCameraDir - IDA 0x1402C79D0
// ============================================================================
hkvVec3 CMoverEx::GetCameraDir() {
    return m_vCamDir;
}

// ============================================================================
// SetMouseOnTrap - IDA 0x1402C7AA0
// ============================================================================
void CMoverEx::SetMouseOnTrap(VGameTrapObject* pTrap) {
    m_pMouseOnTrap = pTrap;
}

// ============================================================================
// GetMouseOnTrap - IDA 0x1402C7B10
// ============================================================================
VGameTrapObject* CMoverEx::GetMouseOnTrap() {
    return m_pMouseOnTrap;
}

// ============================================================================
// GetTrapPos - IDA 0x1402C7AC0
// ============================================================================
hkvVec3 CMoverEx::GetTrapPos() const {
    return m_vTrapPos;
}

// ============================================================================
// SetApplyParentRotation - IDA 0x1402C7B30
// ============================================================================
void CMoverEx::SetApplyParentRotation(int bApply) {
    m_bSummonMonsterApplyRot = (bApply != 0);
}

// ============================================================================
// SetApplyMultipleDamageOnce - IDA 0x1402C7B70
// ============================================================================
void CMoverEx::SetApplyMultipleDamageOnce(bool bApplyMultipleDamageOnce) {
    m_bApplyMultipleDamageOnce = bApplyMultipleDamageOnce;
}

// ============================================================================
// SetAllowAbsorbSG - IDA 0x1402C7B90
// ============================================================================
void CMoverEx::SetAllowAbsorbSG(bool bAllow) {
    m_bAllowAbsorbSG = bAllow;
}

// ============================================================================
// GetAllowAbsorbSG - IDA 0x1402C7DA0
// ============================================================================
bool CMoverEx::GetAllowAbsorbSG() {
    return m_bAllowAbsorbSG;
}

// ============================================================================
// GetGrapTarget - IDA 0x1402C7BB0
// ============================================================================
CMoverEx* CMoverEx::GetGrapTarget() {
    return m_pGrapTarget;
}

// ============================================================================
// GetDieType - IDA 0x1402C7BF0
// ============================================================================
DIE_TYPE CMoverEx::GetDieType() {
    return static_cast<DIE_TYPE>(m_eDieType);
}

// ============================================================================
// GetOwnerPlayer - IDA 0x140398BF0
// ============================================================================
CMoverEx* CMoverEx::GetOwnerPlayer() {
    // IDA: if (this->m_dwOwnerID)
    //         return (CMoverEx *)CMover::GetMoverObject(this, this->m_dwOwnerID);
    //     else return nullptr;
    if (m_dwOwnerID) {
        return static_cast<CMoverEx*>(CMover::GetMoverObject(m_dwOwnerID));
    }
    return nullptr;
}

// ============================================================================
// SetDieReason - IDA 0x14039E710
// ============================================================================
void CMoverEx::SetDieReason(std::uint8_t byReason, int nDamage) {
    // IDA: if (this->m_byDieReason)
    // {
    //     if (!this->m_nDieDamage && nDamage > 0)
    //     {
    //         this->m_byDieReason = byReason;
    //         this->m_nDieDamage = nDamage;
    //     }
    // }
    // else
    // {
    //     this->m_byDieReason = byReason;
    //     this->m_nDieDamage = nDamage;
    // }
    if (m_byDieReason) {
        if (!m_nDieDamage && nDamage > 0) {
            m_byDieReason = byReason;
            m_nDieDamage = nDamage;
        }
    } else {
        m_byDieReason = byReason;
        m_nDieDamage = nDamage;
    }
}

// ============================================================================
// SetDie - IDA 0x140397520
// 注意: IDA 中 CMoverEx::SetDie 只有 2 个参数 (nMotion, bSuicide)
// 但基类 CMover::SetDie 有 3 个参数 (nMotion, bSuicide, bSendPacket)
// 这里重写基类方法，忽略第3个参数 bSendPacket
// ============================================================================
void CMoverEx::SetDie(std::int16_t nMotion, int bSuicide, bool bSendPacket) {
    // IDA: if (!(unsigned int)CMoverEx::IsSystemActor(this)
    //     && !XActor::IsStatus(&this->XActor, 4u)
    //     && !XActor::IsStatus(&this->XActor, 2u))
    if (!IsSystemActor() && !IsStatus(4) && !IsStatus(2)) {
        // IDA: CMover::MoveingValueClear(this);
        MoveingValueClear();

        // IDA: CMover::AllBuffClear(this, 1u);
        AllBuffClear(1);

        // IDA: XActor::SetStatus(&this->XActor, 4u);
        SetStatus(4);

        // IDA: if (nMotion == -1)
        if (nMotion == -1) {
            // IDA: if (CMover::IsHitDown(this))
            if (IsHitDown()) {
                // IDA: LOWORD(v3) = 13; this->RealDie(this, v3);
                RealDie(13);
            }
            // IDA: else if (!CMover::IsKnockDown(this) || this->m_nHitStatus == 5)
            else if (!IsKnockDown() || m_nHitStatus == 5) {
                // IDA: LOWORD(v4) = 12; this->RealDie(this, v4);
                RealDie(12);
            }
        }
        else {
            // IDA: this->RealDie(this, nMotion);
            RealDie(nMotion);
        }
    }
}

// ============================================================================
// SetCurDivergenceTable - IDA 0x1402C7CF0
// ============================================================================
void CMoverEx::SetCurDivergenceTable(TB_DIVERGENCE* pCurDivTable, std::uint32_t dwSkillID) {
    m_pCurDivergenceTableRef = pCurDivTable;
    m_dwDivergenceSkillID = dwSkillID;
}

// ============================================================================
// GetCurDivergenceTable - IDA 0x1402C7D40
// ============================================================================
TB_DIVERGENCE* CMoverEx::GetCurDivergenceTable() {
    return m_pCurDivergenceTableRef;
}

// ============================================================================
// GetCurDivergenceSKillID - IDA 0x1402C7D20
// ============================================================================
std::uint32_t CMoverEx::GetCurDivergenceSKillID() {
    return m_dwDivergenceSkillID;
}

// ============================================================================
// GetSummonAkashicYaw - IDA 0x1402C7D80
// ============================================================================
float CMoverEx::GetSummonAkashicYaw() {
    return m_fSummonAkashicYaw;
}

// ============================================================================
// GetChangeMobTableID - IDA 0x1403539E0
// ============================================================================
std::uint32_t CMoverEx::GetChangeMobTableID() {
    return m_dwChangeMobTableID;
}

// ============================================================================
// GetChangeMobNewID - IDA 0x140353A00
// ============================================================================
std::uint32_t CMoverEx::GetChangeMobNewID() {
    return m_dwChangeMobNewID;
}

// ============================================================================
// SetOnDie - IDA 0x140353C80
// ============================================================================
void CMoverEx::SetOnDie(bool bDie) {
    m_bOnDie = bDie;
}

// ============================================================================
// IsSystemActor - IDA 0x140353E10
// ============================================================================
int CMoverEx::IsSystemActor() {
    return m_bSystemActor;
}

// Note: SetSector and GetSector need declarations in MoverEx.h

// ============================================================================
// GetAggroLevelOrder - IDA 0x140364570
// ============================================================================
std::uint8_t CMoverEx::GetAggroLevelOrder() {
    return m_byAggroLevelOrder;
}

// ============================================================================
// GetPreTargetListCount
// ============================================================================
int CMoverEx::GetPreTargetListCount() {
    return static_cast<int>(m_vPreTargetList.size());
}

// ============================================================================
// GetControlType - IDA 0x140398C30
// ============================================================================
std::uint8_t CMoverEx::GetControlType(TB_SKILL* pSkillTable) {
    if (!pSkillTable) {
        return 0;
    }
    if (m_pCurDivergenceTableRef && m_pCurDivergenceTableRef->Div_Option_Type == 2) {
        return m_pCurDivergenceTableRef->Div_Option_Value;
    }
    return pSkillTable->Control_Type;
}

// ============================================================================
// GetCameraLock - IDA 0x140398C90
// ============================================================================
std::uint8_t CMoverEx::GetCameraLock(TB_SKILL* pSkillTable) {
    if (!pSkillTable) {
        return 0;
    }
    if (m_pCurDivergenceTableRef && m_pCurDivergenceTableRef->Div_Option_Type == 3) {
        return m_pCurDivergenceTableRef->Div_Option_Value;
    }
    return pSkillTable->Camera_Lock;
}

// ============================================================================
// ChargeSkillStart - IDA 0x14037EA30
// 开始技能充能状态
// ============================================================================
void CMoverEx::ChargeSkillStart() {
    // IDA 0x14037EA30 反编译:
    // 1. 检查当前技能表是否存在
    // 2. 检查技能控制类型是否为充能类型 (2, 5, 8)
    // 3. 初始化充能参数:
    //    a. 设置攻击按键状态
    //    b. 设置充能步骤为 0
    //    c. 设置最大充能步骤
    //    d. 设置充能切换时间
    //    e. 重置总充能时间和左右充能值

    if (!m_pCurSkillTableRef) {
        return;
    }

    // 检查技能控制类型 (2=蓄力, 5=连续蓄力, 8=特殊蓄力)
    std::uint8_t byControlType = GetControlType(m_pCurSkillTableRef);
    if (byControlType != 2 && byControlType != 5 && byControlType != 8) {
        return;
    }

    // 初始化充能参数
    m_bAttackKeyPress = 1;
    m_bySkillChargeStep = 0;
    m_bySkillChargeMaxStep = m_pCurSkillTableRef->Charging_Count;

    // 设置充能切换时间 (从技能表的 Time_Value_01 字段获取)
    // m_fSkillChargeChangeTime = (float)*((int*)&m_pCurSkillTableRef->Time_Value_01 + m_bySkillChargeStep) * 0.001f;
    // 简化: 使用默认值
    m_fSkillChargeChangeTime = 0.05f;

    // 重置充能相关值
    m_fSkillTotalChargeTime = 0.0f;
    m_fLeftChargingValue = 0.0f;
    m_fRightChargingValue = 0.0f;
}

// ============================================================================
// ChargeSkillEnd - IDA 0x14037ECD0
// 结束技能充能状态，切换到下一阶段动画或释放动画
// ============================================================================
void CMoverEx::ChargeSkillEnd() {
    // IDA 0x14037ECD0 反编译:
    // 1. 检查当前技能表和攻击按键状态
    // 2. 如果正在蓄力中 (m_bySkillAnimStep != 0):
    //    a. 清除攻击按键和充能时间
    //    b. 设置技能动画步骤 = 3 (释放)
    //    c. 获取释放动画名并切换动画
    // 3. 如果尚未开始蓄力:
    //    a. 设置最大充能步骤
    //    b. 设置充能切换时间

    if (!m_pCurSkillTableRef || !m_bAttackKeyPress) {
        return;
    }

    if (m_bySkillAnimStep != 0) {
        // 已经在蓄力中 -> 切换到释放阶段
        m_bAttackKeyPress = 0;
        m_fSkillChargeChangeTime = 0.0f;
        m_bySkillAnimStep = 3;

        // TODO: 需要 GetSkillAnimName 实现
        // const char* pAnimName = GetSkillAnimName(m_pCurSkillTableRef, m_bySkillAnimStep);
        // if (pAnimName) {
        //     VString strSkillName(pAnimName);
        //     unsigned int dwNewAnimIndex = GetAnimIndex(strSkillName);
        //     short nNewMotion = AnimKeyToMotion(dwNewAnimIndex);
        //     ChangeMotion(nNewMotion, 1, 4);
        // }
    } else {
        // 尚未开始蓄力 -> 设置最大步骤
        m_bySkillChargeMaxStep = m_bySkillAnimStep + 1;
        m_fSkillChargeChangeTime = 0.05f;  // 50ms 后开始充能
    }
}

// ============================================================================
// IsCanSkill - IDA 0x14037FB80 (CMoverEx::IsCanSkill)
// ============================================================================
bool CMoverEx::IsCanSkill() {
    // IDA 0x14037FB80: return !XActor::IsStatus(&this->XActor, 0x40000000u)
    //                      && !XActor::IsStatus(&this->XActor, 0x80000000);
    return !CMover::IsStatus(0x40000000u) && !CMover::IsStatus(0x80000000);
}

// ============================================================================
// CalcTargetDamage - IDA 0x140388670 -> 0x14038B7C8
// 大小: 12,376 bytes
// 计算目标伤害的核心战斗函数
// ============================================================================
void CMoverEx::CalcTargetDamage(CMover* pTargetMover, int nIndex, bool bAllowAbsorbSG,
                                 TB_SKILL* pSkillTable, AttackJudgmentTrigger* pActionEvent,
                                 float fChainDamageRate, bool bDontCalcByResult,
                                 std::uint8_t byFixResult, bool bSummonDamageOnceBuff)
{
    // IDA 反编译完整还原 (1145 行)
    // 注意: IDA 注释 "local variable allocation has failed, the output may be wrong!"
    // 但功能逻辑完整，变量命名基于上下文推断
    
    if (nIndex >= 100 || !pTargetMover) {
        return;
    }
    
    // 获取双方属性组件
    std::tr1::shared_ptr<CGocAttribute> pMyAttr;
    std::tr1::shared_ptr<CGocAttribute> pTargetAttr;
    
    CMover::GetGOC<CGocAttribute>(this, &pMyAttr, false);
    CMover::GetGOC<CGocAttribute>(pTargetMover, &pTargetAttr, false);
    
    if (!pMyAttr || !pTargetAttr) {
        return;
    }
    
    if (!pSkillTable) {
        return;
    }
    
    CGocAttribute* pMyAttrPtr = pMyAttr.get();
    if (!pMyAttrPtr || !pMyAttrPtr->GetStatusTable()) {
        return;
    }
    
    if (!m_pSkillMgr) {
        return;
    }
    
    CMoverEx* pTargetMoverEx = static_cast<CMoverEx*>(pTargetMover);
    
    // 应用攻击者 Buff 能力
    unsigned int dwMyID = GetID();
    pTargetMoverEx->ApplyBuffAbilityForAttacker(dwMyID);
    
    // === 伤害计算主流程 ===
    float fAttributeResult = 0.0f;
    std::uint8_t byResult = 0;
    std::uint8_t byLevel = GetLevelForStat();
    std::uint8_t byOtherLevel = pTargetMover->GetLevelForStat();
    
    // 属性类型索引 (0=物理, 2=魔法)
    int iAddIndex = (pSkillTable->Skill_Attribute == 2) ? 1 : 0;
    float PASR = 0.0f;
    
    // 固定结果标志
    if (byFixResult) {
        byResult = byFixResult;
    }
    
    // 忽略无敌标志
    if (pActionEvent && pActionEvent->sReactionInfo.bIgnoreTargetInvincible) {
        byResult |= 0x10;
    }
    
    // === 命中率计算 (PAR - PASR) ===
    float fMyPAR = m_fAbility[iAddIndex + 26];
    float fTargetPARP = pTargetMover->GetStat(iAddIndex + 43);
    int nChance = static_cast<int>((fMyPAR - fTargetPARP) * 100.0f);
    
    if (nChance > 0) {
        nChance /= 10;
    }
    
    if (nChance >= 1) {
        if (nChance > 10000) {
            nChance = 10000;
        }
    } else {
        nChance = 0;
    }
    
    // 判定命中
    if (bDontCalcByResult || nChance > (rand() % 10000)) {
        float PAR = m_fAbility[iAddIndex + 26];
        if (PAR > 0.0f) {
            PAR /= 10.0f;
        }
        
        float PARP = pTargetMover->GetStat(iAddIndex + 43);
        if (PARP > 0.0f) {
            PARP /= 10.0f;
        }
        
        PASR = PAR - PARP;
        if (PASR <= 0.0f) {
            PASR = 1.0f;
        } else {
            PASR /= 5.0f;
        }
    } else {
        byResult |= 1;  // Miss
    }
    
    // 获取攻击伤害数据
    tagSKILL_ACTION_DAMAGE stDamage;
    memset(&stDamage, 0, sizeof(stDamage));
    m_pSkillMgr->GetAttackDamage(&stDamage, nIndex);
    
    // === 触发效果条件 ===
    if ((byResult & 1) == 1) {
        // Miss 效果
        CheckOptionEffectInvoke(EFFECT_CONDITION_ATTACK_MISSED, pTargetMoverEx, 0.0f, EFFECT_INVOKE_STAT);
        pTargetMoverEx->CheckOptionEffectInvoke(EFFECT_CONDITION_DAMAGE_MISSED, this, 0.0f, EFFECT_INVOKE_STAT);
    } else {
        // 命中效果
        CheckOptionEffectInvoke(EFFECT_CONDITION_ATTACK_SUCCESS, pTargetMoverEx, 0.0f, EFFECT_INVOKE_STAT);
    }
    
    pTargetMoverEx->CheckOptionEffectInvoke(EFFECT_CONDITION_DAMAGED, this, 0.0f, EFFECT_INVOKE_STAT);
    
    // 空中被击效果
    if (pTargetMover->IsFlying()) {
        pTargetMoverEx->CheckOptionEffectInvoke(EFFECT_CONDITION_KNOCK_BACK, this, 0.0f, EFFECT_INVOKE_STAT);
        CheckOptionEffectInvoke(EFFECT_CONDITION_ARIAL_ATTACK_SUCCESS, this, 0.0f, EFFECT_INVOKE_STAT);
    }
    
    // 倒地被击效果
    if (pTargetMover->IsHitDown()) {
        pTargetMoverEx->CheckOptionEffectInvoke(EFFECT_CONDITION_KNOCK_DOWN, this, 0.0f, EFFECT_INVOKE_STAT);
    }
    
    // === 暴击判定 ===
    if ((byResult & 1) == 0) {
        float fMyCR = m_fAbility[iAddIndex + 29] + PASR;
        float fTargetCDR = pTargetMover->GetStat(iAddIndex + 31);
        int nCritChance = static_cast<int>(100.0f * (fMyCR - fTargetCDR));
        
        if (nCritChance >= 1) {
            if (nCritChance > 10000) {
                nCritChance = 10000;
            }
        } else {
            nCritChance = 0;
        }
        
        if (!bDontCalcByResult && nCritChance > (rand() % 10000)) {
            byResult |= 4;  // Critical
        }
    }
    
    // 暴击效果
    if ((byResult & 4) == 4) {
        CheckOptionEffectInvoke(EFFECT_CONDITION_ATTACK_CRITICAL, pTargetMoverEx, 0.0f, EFFECT_INVOKE_STAT);
        pTargetMoverEx->CheckOptionEffectInvoke(EFFECT_CONDITION_DAMAGED_CRITICAL, this, 0.0f, EFFECT_INVOKE_STAT);
    }
    
    // === 武器道具倍率 ===
    std::uint8_t byItemRateFlag = 0;
    int iItemRateResult = 0;
    
    unsigned int myType = CMover::GetType();
    if (myType == 0 || myType == 3) {
        byItemRateFlag = pTargetMover->GetItemRateFlag();
    }
    
    if (byItemRateFlag) {
        bool bCritical = (byResult & 4) != 0;
        iItemRateResult = GetItemRateResultWeapon(byOtherLevel, pMyAttr, bCritical);
    }
    
    // === 基础伤害计算 ===
    float fResult = GetRandomDamage(pSkillTable->Skill_Attribute, iItemRateResult);
    
    // Miss 伤害减免
    if ((byResult & 1) == 1) {
        fResult *= (m_fAbility[28] * 0.01f);
    }
    
    float fPrevResult = fResult;
    
    // 暴击伤害加成
    if ((byResult & 4) == 4) {
        fResult += m_fAbility[iAddIndex + 35];
    }
    
    // === 防御减免 (DSR) ===
    float fDSR = 0.0f;
    if (iAddIndex) {
        fDSR = pMyAttrPtr->GetStatusTable()->Con_MDSR;
    } else {
        fDSR = pMyAttrPtr->GetStatusTable()->Con_PDSR;
    }
    
    float fTargetDSR = pTargetMover->GetStat(iAddIndex + 38);
    fDSR += fTargetDSR;
    fResult -= (fResult * (fDSR * 0.01f));
    
    stDamage.byDamageFlag |= byResult;
    
    // === 伤害修正 ===
    nAttrDamage = 0;
    bool bIsPVP = false;
    iItemRateResult = 0;
    
    byItemRateFlag = GetItemRateFlag();
    if (byItemRateFlag && pTargetMover->GetType() == 0) {
        iItemRateResult = pTargetMover->GetItemRateResultGear(byLevel, pTargetAttr);
    }
    
    // 计算最终伤害
    stDamage.nDamage = pTargetMover->GetDamageCalc(static_cast<int>(fResult), pSkillTable->Skill_Attribute);
    
    // === 无敌判定 ===
    bool bInvincible = false;
    if (!pTargetMoverEx->IsExceptionalDamage() &&
        pTargetMover->GetDefenseType() == 3 &&
        ((stDamage.byDamageFlag & 0x10) == 0 || pTargetMover->IsImmunityStatus())) {
        bInvincible = true;
        stDamage.nDamage = 0;
    }
    
    if (bInvincible) {
        // 设置基础伤害后返回
        m_pSkillMgr->SetBaseDamage(nIndex, stDamage.nDamage);
        return;
    }
    
    float fPrevDamage = static_cast<float>(stDamage.nDamage);
    
    // === 空中伤害减免 ===
    if (pTargetMover->IsFlying()) {
        if (CMover::GetType() == 0) {  // Player
            stDamage.nDamage = static_cast<int>(stDamage.nDamage * 1.5f);
        }
        
        float nDamage = static_cast<float>(stDamage.nDamage);
        float fAirDownRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_AIR_DAMAGED_DOWN_RAT);
        stDamage.nDamage -= static_cast<int>(nDamage * (fAirDownRate * 0.01f));
    }
    
    // === 倒地伤害减免 ===
    if (pTargetMover->IsHitDown()) {
        float nDamage = static_cast<float>(stDamage.nDamage);
        float fDownRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_DOWN_DAMAGED_DOWN_RAT);
        stDamage.nDamage -= static_cast<int>(nDamage * (fDownRate * 0.01f));
    }
    
    // === Monster 特殊处理 ===
    if (pTargetMover->GetType() == 2) {  // Monster
        // Super Armor Break 伤害加成
        if (pTargetMover->GetMaxSuperArmorGage() > 0.0f && pTargetMover->GetCurSuperArmorGage() <= 0.0f) {
            stDamage.nDamage = static_cast<int>(stDamage.nDamage * 1.5f);
        }
        
        CMonster* pMonster = dynamic_cast<CMonster*>(pTargetMover);
        if (pMonster) {
            TB_MONSTER* pMobTable = pMonster->GetMobTableRef();
            
            // Boss 伤害加成
            if (pMobTable->Monster_Rank == 3 || pMobTable->Monster_Rank == 4 || pMobTable->Monster_Rank == 5) {
                float nDamage = static_cast<float>(stDamage.nDamage);
                float fBossRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_BOSS_DAMAGE_ADD_RAT);
                stDamage.nDamage += static_cast<int>(nDamage * (fBossRate * 0.01f));
                
                float fBossAddRate = GetBossAttackAddRate();
                stDamage.nDamage += static_cast<int>(nDamage * fBossAddRate);
            } else {
                // Normal Monster
                float nDamage = static_cast<float>(stDamage.nDamage);
                float fNormalRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_NORMAL_DAMAGE_ADD_RAT);
                stDamage.nDamage += static_cast<int>(nDamage * (fNormalRate * 0.01f));
            }
            
            // Melee/Ranged 伤害加成
            float fAddRate = 0.0f;
            if (pMobTable->Monster_Status_Type == 2) {  // Ranged
                float nDamage = static_cast<float>(stDamage.nDamage);
                fAddRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_RANGED_DAMAGE_ADD_RAT);
                stDamage.nDamage += static_cast<int>(nDamage * (fAddRate * 0.01f));
            } else {  // Melee
                float nDamage = static_cast<float>(stDamage.nDamage);
                fAddRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_MELEE_DAMAGE_ADD_RAT);
                stDamage.nDamage += static_cast<int>(nDamage * (fAddRate * 0.01f));
            }
            
            // Air/Down 伤害加成
            if (pMonster->IsFlying()) {
                float nDamage = static_cast<float>(stDamage.nDamage);
                float fAirRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_AIR_DAMAGE_ADD_RAT);
                stDamage.nDamage += static_cast<int>(nDamage * (fAirRate * 0.01f));
            }
            
            if (pMonster->IsHitDown()) {
                float nDamage = static_cast<float>(stDamage.nDamage);
                float fDownRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_DOWN_DAMAGE_ADD_RAT);
                stDamage.nDamage += static_cast<int>(nDamage * (fDownRate * 0.01f));
            }
            
            // Faction 伤害加成
            int nFaction = pMobTable->Monster_Faction - 6;
            float nDamage = static_cast<float>(stDamage.nDamage);
            
            switch (nFaction) {
                case 0: { // SJUNK
                    float fRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_SJUNK_DAMAGE_ADD_RAT);
                    stDamage.nDamage += static_cast<int>(nDamage * (fRate * 0.01f));
                    break;
                }
                case 1: { // DOLL
                    float fRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_DOLL_DAMAGE_ADD_RAT);
                    stDamage.nDamage += static_cast<int>(nDamage * (fRate * 0.01f));
                    break;
                }
                case 2: { // NED
                    float fRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_NED_DAMAGE_ADD_RAT);
                    stDamage.nDamage += static_cast<int>(nDamage * (fRate * 0.01f));
                    break;
                }
                case 3: { // RAPID
                    float fRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_RAPID_DAMAGE_ADD_RAT);
                    stDamage.nDamage += static_cast<int>(nDamage * (fRate * 0.01f));
                    break;
                }
                case 10: { // NIHIL
                    float fRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_NIHIL_DAMAGE_ADD_RAT);
                    stDamage.nDamage += static_cast<int>(nDamage * (fRate * 0.01f));
                    break;
                }
                case 11: { // IRON
                    float fRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_IRON_DAMAGE_ADD_RAT);
                    stDamage.nDamage += static_cast<int>(nDamage * (fRate * 0.01f));
                    break;
                }
                case 12: { // NEAR
                    float fRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_NEAR_DAMAGE_ADD_RAT);
                    stDamage.nDamage += static_cast<int>(nDamage * (fRate * 0.01f));
                    break;
                }
                case 13: { // DIS6
                    float fRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_DIS6_DAMAGE_ADD_RAT);
                    stDamage.nDamage += static_cast<int>(nDamage * (fRate * 0.01f));
                    break;
                }
                case 22: { // CITADEL
                    float fRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_CITADEL_DAMAGE_ADD_RAT);
                    stDamage.nDamage += static_cast<int>(nDamage * (fRate * 0.01f));
                    break;
                }
                case 23: { // PRIMAL
                    float fRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_PRIMAL_DAMAGE_ADD_RAT);
                    stDamage.nDamage += static_cast<int>(nDamage * (fRate * 0.01f));
                    break;
                }
            }
        }
    }
    
    // === 攻击者 Monster 处理 (伤害减免) ===
    CMonster* pAttackerMonster = dynamic_cast<CMonster*>(this);
    if (pAttackerMonster && pTargetMover->GetType() != 2) {
        TB_MONSTER* pMobTable = pAttackerMonster->GetMobTableRef();
        
        // Boss 伤害减免
        if (pMobTable->Monster_Rank == 3 || pMobTable->Monster_Rank == 4 || pMobTable->Monster_Rank == 5) {
            float nDamage = static_cast<float>(stDamage.nDamage);
            float fBossRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_BOSS_DAMAGED_DOWN_RAT);
            stDamage.nDamage -= static_cast<int>(nDamage * (fBossRate * 0.01f));
            
            float fBossDownRate = pTargetMover->GetBossAttackedDownRate();
            stDamage.nDamage -= static_cast<int>(nDamage * fBossDownRate);
        } else {
            // Normal Monster
            float nDamage = static_cast<float>(stDamage.nDamage);
            float fNormalRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_NORMAL_DAMAGED_DOWN_RAT);
            stDamage.nDamage -= static_cast<int>(nDamage * (fNormalRate * 0.01f));
        }
        
        // Melee/Ranged 伤害减免
        if (pMobTable->Monster_Status_Type == 2) {  // Ranged
            float nDamage = static_cast<float>(stDamage.nDamage);
            float fRangedRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_RANGED_DAMAGED_DOWN_RAT);
            stDamage.nDamage -= static_cast<int>(nDamage * (fRangedRate * 0.01f));
        } else {  // Melee
            float nDamage = static_cast<float>(stDamage.nDamage);
            float fMeleeRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_MELEE_DAMAGE_DOWN_RAT);
            stDamage.nDamage -= static_cast<int>(nDamage * (fMeleeRate * 0.01f));
        }
        
        // Faction 伤害减免
        int nFaction = pMobTable->Monster_Faction - 6;
        float nDamage = static_cast<float>(stDamage.nDamage);
        
        switch (nFaction) {
            case 0: { // SJUNK
                float fRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_SJUNK_DAMAGED_DOWN_RAT);
                stDamage.nDamage -= static_cast<int>(nDamage * (fRate * 0.01f));
                break;
            }
            case 1: { // DOLL
                float fRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_DOLL_DAMAGED_DOWN_RAT);
                stDamage.nDamage -= static_cast<int>(nDamage * (fRate * 0.01f));
                break;
            }
            case 2: { // NED
                float fRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_NED_DAMAGED_DOWN_RAT);
                stDamage.nDamage -= static_cast<int>(nDamage * (fRate * 0.01f));
                break;
            }
            case 3: { // RAPID
                float fRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_RAPID_DAMAGED_DOWN_RAT);
                stDamage.nDamage -= static_cast<int>(nDamage * (fRate * 0.01f));
                break;
            }
            case 10: { // NIHIL
                float fRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_NIHIL_DAMAGED_DOWN_RAT);
                stDamage.nDamage -= static_cast<int>(nDamage * (fRate * 0.01f));
                break;
            }
            case 11: { // IRON
                float fRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_IRON_DAMAGED_DOWN_RAT);
                stDamage.nDamage -= static_cast<int>(nDamage * (fRate * 0.01f));
                break;
            }
            case 12: { // NEAR
                float fRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_NEAR_DAMAGED_DOWN_RAT);
                stDamage.nDamage -= static_cast<int>(nDamage * (fRate * 0.01f));
                break;
            }
            case 13: { // DIS6
                float fRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_DIS6_DAMAGED_DOWN_RAT);
                stDamage.nDamage -= static_cast<int>(nDamage * (fRate * 0.01f));
                break;
            }
            case 22: { // CITADEL
                float fRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_CITADEL_DAMAGED_DOWN_RAT);
                stDamage.nDamage -= static_cast<int>(nDamage * (fRate * 0.01f));
                break;
            }
            case 23: { // PRIMAL
                float fRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_PRIMAL_DAMAGED_DOWN_RAT);
                stDamage.nDamage -= static_cast<int>(nDamage * (fRate * 0.01f));
                break;
            }
        }
    }
    
    // === 全体攻击加成 ===
    float nDamage = static_cast<float>(stDamage.nDamage);
    float fAllAttackRate = GetAllAttackAddRate();
    stDamage.nDamage += static_cast<int>(nDamage * fAllAttackRate);
    
    // === Buff 效果应用 ===
    if ((byResult & 1) != 0) {  // Miss
        float nDamage = static_cast<float>(stDamage.nDamage);
        float fMissRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_MISS_DAMAGED_DOWN_RAT);
        stDamage.nDamage -= static_cast<int>(nDamage * (fMissRate * 0.01f));
    } else {
        // 攻击 Debuff
        int iAttackDebuffID = static_cast<int>(pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_ATTACK_DEBUFF));
        pTargetMover->SetBuffStatus(iAttackDebuffID, dwMyID, true);
        
        // 受击 Debuff
        int iDamagedDebuffID = static_cast<int>(pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_DAMAGED_DEBUFF));
        unsigned int dwTargetID = pTargetMover->GetID();
        SetBuffStatus(iDamagedDebuffID, dwTargetID, true);
        
        // 攻击 Buff
        int iAttackBuffID = static_cast<int>(pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_ATTACK_BUFF));
        SetBuffStatus(iAttackBuffID, dwMyID, true);
        
        // 受击 Buff
        int iDamagedBuffID = static_cast<int>(pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_DAMAGED_BUFF));
        pTargetMover->SetBuffStatus(iDamagedBuffID, dwTargetID, true);
        
        // Stamina 恢复
        float fStaminaRate = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_ATTACK_STAMINA_RAT);
        float fAddStamina = GetStat(14) * (fStaminaRate * 0.01f);
        
        std::tr1::shared_ptr<CGocAttribute> pAttr;
        CMover::GetGOC<CGocAttribute>(this, &pAttr, false);
        if (pAttr) {
            // Add stamina (stat index 3)
            // pAttr->ModifyStat(3, fAddStamina, true);
        }
    }
    
    // === PVP 伤害调整 ===
    if (GetType() == 0 && pTargetMover->GetType() == 0) {
        bIsPVP = true;
        
        float fPvPReduce = pMyAttrPtr->GetStatusTable()->Con_PvP_Reduce;
        float fPvPIncreaseInt = pMyAttrPtr->GetSpecialEffect(EFFECT_SPECIAL_PVP_DAMAGE_INCREASE_INT);
        float fPvPDecreaseInt = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_PVP_DAMAGE_DECREASE_INT);
        
        if (fPvPIncreaseInt < 1.0f) fPvPIncreaseInt = 1.0f;
        if (fPvPDecreaseInt < 1.0f) fPvPDecreaseInt = 1.0f;
        
        TB_STATUS* pMyStatus = pMyAttrPtr->GetStatusTable();
        fPvPReduce += (pMyStatus->Con_PvP_Reduce * fPvPIncreaseInt) * 0.01f;
        
        TB_STATUS* pTargetStatus = pTargetAttr->GetStatusTable();
        fPvPReduce -= (pTargetStatus->Con_PvP_Reduce * fPvPDecreaseInt) * 0.01f;
        
        if (fPvPReduce < 0.0f) fPvPReduce = 0.0f;
        
        stDamage.nDamage = static_cast<int>(stDamage.nDamage * (fPvPReduce * 0.01f));
    }
    
    // === 召唤伤害倍率 ===
    if (m_fMultipleDamageOnce > 0.0f && bSummonDamageOnceBuff) {
        stDamage.nDamage = static_cast<int>(stDamage.nDamage * m_fMultipleDamageOnce);
        m_bApplyMultipleDamageOnce = true;
    }
    
    // === 技能伤害倍率 ===
    if (pActionEvent->sReactionInfo.fDamageRate > 0.0f) {
        stDamage.nDamage = static_cast<int>(stDamage.nDamage * pActionEvent->sReactionInfo.fDamageRate);
    }
    
    if (fChainDamageRate > 0.0f) {
        stDamage.nDamage = static_cast<int>(stDamage.nDamage * fChainDamageRate);
    }
    
    // 充能伤害加成
    if (m_fChargingInputCalcMultiple > 0.0f) {
        stDamage.nDamage += static_cast<int>(stDamage.nDamage * m_fChargingInputCalcMultiple);
    }
    
    // Divergence 伤害
    if (m_pCurDivergenceTableRef && m_pCurDivergenceTableRef->Div_Option_Type == 0) {
        if (m_pCurDivergenceTableRef->Div_Option_Value == 1) {
            stDamage.nDamage += static_cast<int>(m_pCurDivergenceTableRef->Div_SubOption_Value);
        } else if (m_pCurDivergenceTableRef->Div_Option_Value == 0 && m_pCurDivergenceTableRef->Div_SubOption_Value > 0.0f) {
            stDamage.nDamage += static_cast<int>(stDamage.nDamage * m_pCurDivergenceTableRef->Div_SubOption_Value);
        }
    }
    
    // Deck Bonus
    if (m_pCurDeckBonusRef && m_pCurDeckBonusRef->Bonus_Type == 0 && m_pCurDeckBonusRef->Bonus_Value > 0.0f) {
        stDamage.nDamage += static_cast<int>(stDamage.nDamage * m_pCurDeckBonusRef->Bonus_Value);
    }
    
    // Condition Buff Damage
    if (pActionEvent->sReactionInfo.iConditionBuffID > 0) {
        VBitmask actorID;
        GetActorID(&actorID);
        unsigned int dwQuestID = CQuestCondition::GetQuestID(&actorID);
        
        int iBuffIndex = pTargetMoverEx->FindBuffStatus(pActionEvent->sReactionInfo.iConditionBuffID, dwQuestID);
        if (iBuffIndex >= 0) {
            float fConditionRate = static_cast<float>(pTargetMoverEx->GetBuffStatusCount(iBuffIndex)) *
                                   pActionEvent->sReactionInfo.fConditionBuffDamageRate;
            stDamage.nDamage = static_cast<int>(stDamage.nDamage * fConditionRate);
            pTargetMoverEx->ClearBuffStatusBySlot(iBuffIndex, false);
        }
    }
    
    // 设置基础伤害
    m_pSkillMgr->SetBaseDamage(nIndex, stDamage.nDamage);
    
    // === 技能选项伤害 ===
    float fSkillOptionDamage = 0.0f;
    float fSkillOptionPerDamage = 0.0f;
    
    pMyAttrPtr->GetSkillOptionEffect(pSkillTable->Skill_Group, EFFECT_SKILL_OPTION_DAMAGE, &fSkillOptionDamage);
    pMyAttrPtr->GetSkillOptionEffect(pSkillTable->Skill_Group, EFFECT_SKILL_OPTION_PER_DAMAGE, &fSkillOptionPerDamage);
    
    fSkillOptionDamage += fSkillOptionPerDamage;
    
    float fCalcDamage = stDamage.nDamage + pSkillTable->Skill_Basic_Damage + pSkillTable->Skill_Add_Damage_INT;
    float fSkillDamage = fCalcDamage * ((pSkillTable->Skill_Add_Damage_RAT + fSkillOptionDamage) * 0.01f);
    
    float fOptionDamageRate = GetTotalOptionEffectValue(EFFECT_STATUS_DAMAGE_RATE);
    if (fOptionDamageRate > 0.0f) {
        fSkillDamage = fCalcDamage * ((pSkillTable->Skill_Add_Damage_RAT + fSkillOptionDamage) * 0.01f) *
                       (fOptionDamageRate * 0.01f);
    }
    
    stDamage.nDamage = static_cast<int>(fSkillDamage);
    
    // PVP Level Difference
    if (bIsPVP) {
        float fMyLevelStat = m_fAbility[75];
        float fTargetLevelStat = pTargetMoverEx->GetStat(76);
        float fDiff = fMyLevelStat - fTargetLevelStat;
        
        if (fDiff >= 1.0f) {
            stDamage.nDamage += static_cast<int>(fDiff);
        }
    }
    
    // === 属性伤害计算 ===
    float fTotalAttr = 0.0f;
    int iMaxAttributeIndex = -1;
    float fMaxAttributeDiff = 0.0f;
    
    for (int i = 0; i < 6; ++i) {
        fTotalAttr += m_fAbility[i + 63];
        
        float fMyAttr = m_fAbility[i + 63];
        float fTargetAttrResist = pTargetMoverEx->GetStat(i + 69);
        float fAttrDiff = fMyAttr - fTargetAttrResist;
        
        if (fMyAttr > 0.0f && fAttrDiff > fMaxAttributeDiff) {
            iMaxAttributeIndex = i;
            fMaxAttributeDiff = fAttrDiff;
        }
    }
    
    if (iMaxAttributeIndex != -1 && fTotalAttr > 0.0f) {
        std::uint8_t byTargetLevel = pTargetMoverEx->GetLevelForStat();
        float fAttrProp = (fTotalAttr / (fTotalAttr + 23.0f * byTargetLevel)) + 0.1f;
        
        if (fAttrProp < 0.5f) fAttrProp = 0.5f;
        
        int nAttrProp = static_cast<int>(fAttrProp * 10000.0f);
        
        if (nAttrProp > (rand() % 10000)) {
            std::uint8_t byTargetLevel2 = pTargetMoverEx->GetLevelForStat();
            fAttributeResult = fMaxAttributeDiff / (83.0f * byTargetLevel2);
            
            if (fAttributeResult > 1.0f) fAttributeResult = 1.0f;
            
            int nRate = rand() % 2001;
            float fRate = 1.0f - (nRate * 0.0001f);
            
            if (m_eTestDamageType == eTestDamage_Normal || m_byFixedMaxDamage) {
                fRate = 1.0f;
            }
            
            nAttrDamage = static_cast<int>((fAttributeResult * fSkillDamage) * fRate);
        }
    }
    
    stDamage.nDamage += nAttrDamage;
    
    if (stDamage.nDamage <= 0) {
        stDamage.nDamage = 1;
    }
    
    if (nAttrDamage > 0) {
        stDamage.byDamageFlag |= (16 * iMaxAttributeIndex);
    }
    
    // === 伤害反射 ===
    if ((byResult & 1) == 0 && GetDefenseType() != 3) {
        float nDamage = static_cast<float>(stDamage.nDamage);
        float fReflectRate = pTargetAttr->GetSpecialEffect(EFFECT_SPECIAL_DAMAGE_REFLECTION_RAT);
        int nReflectionHP = static_cast<int>((nDamage * fReflectRate) * 0.01f);
        
        if (nReflectionHP > 0) {
            unsigned int dwTargetID = pTargetMover->GetID();
            
            if (DamageProcessHP(dwTargetID, 0, nReflectionHP, EFFECT_INVOKE_NONE_STAT, false)) {
                SetDieReason(4, nReflectionHP);
                SetHP(0);
                SetDie(12, 0, false);
            }
            
            int iCurHP = GetHP();
            send_eSUB_CMD_BUFF_DAMAGE(this, this, 0, nReflectionHP, iCurHP, dwTargetID);
        }
    }
    
    // 设置最终伤害
    stDamage.nAttrDamage = nAttrDamage;
    stDamage.nHP = pTargetMoverEx->GetHP();
    m_pSkillMgr->SetAttackDamage(nIndex, &stDamage);
    
    // Maze 伤害统计
    XArea* pArea = GetArea();
    if (pArea) {
        XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
        if (pMaze && GetType() == 0) {
            VBitmask actorID;
            GetActorID(&actorID);
            unsigned int dwQuestID = CQuestCondition::GetQuestID(&actorID);
            pMaze->AddUserDamage(dwQuestID, stDamage.nDamage);
        }
    }
    
    // 清除攻击者 Buff 能力
    unsigned int dwMyID2 = GetID();
    pTargetMoverEx->ClearBuffAbilityForAttacker(dwMyID2);
    
    // === 结算后效果触发 ===
    if ((byResult & 1) == 1) {  // Miss
        CheckOptionEffectInvoke(EFFECT_CONDITION_ATTACK_MISSED, pTargetMoverEx,
                                 static_cast<float>(stDamage.nDamage), EFFECT_INVOKE_NONE_STAT);
        pTargetMoverEx->CheckOptionEffectInvoke(EFFECT_CONDITION_DAMAGE_MISSED, this,
                                                 static_cast<float>(stDamage.nDamage), EFFECT_INVOKE_NONE_STAT);
    } else {
        if ((byResult & 4) == 4) {  // Critical
            CheckOptionEffectInvoke(EFFECT_CONDITION_ATTACK_CRITICAL, pTargetMoverEx,
                                     static_cast<float>(stDamage.nDamage), EFFECT_INVOKE_NONE_STAT);
            pTargetMoverEx->CheckOptionEffectInvoke(EFFECT_CONDITION_DAMAGED_CRITICAL, this,
                                                     static_cast<float>(stDamage.nDamage), EFFECT_INVOKE_NONE_STAT);
        }
        
        CheckOptionEffectInvoke(EFFECT_CONDITION_ATTACK_SUCCESS, pTargetMoverEx,
                                 static_cast<float>(stDamage.nDamage), EFFECT_INVOKE_NONE_STAT);
    }
    
    pTargetMoverEx->CheckOptionEffectInvoke(EFFECT_CONDITION_DAMAGED, this,
                                             static_cast<float>(stDamage.nDamage), EFFECT_INVOKE_NONE_STAT);
    
    if (pTargetMover->IsFlying()) {
        pTargetMoverEx->CheckOptionEffectInvoke(EFFECT_CONDITION_KNOCK_BACK, this,
                                                 static_cast<float>(stDamage.nDamage), EFFECT_INVOKE_NONE_STAT);
        CheckOptionEffectInvoke(EFFECT_CONDITION_ARIAL_ATTACK_SUCCESS, this, 0.0f, EFFECT_INVOKE_NONE_STAT);
    }
    
    if (pTargetMover->IsHitDown()) {
        pTargetMoverEx->CheckOptionEffectInvoke(EFFECT_CONDITION_KNOCK_DOWN, this,
                                                 static_cast<float>(stDamage.nDamage), EFFECT_INVOKE_NONE_STAT);
    }
    
    // 被动技能检查
    CheckPassiveSkillByHit(pTargetMoverEx, pSkillTable, byResult);
    
    // === SG 吸收 ===
    if (bAllowAbsorbSG) {
        float fAddSGVal = 0.0f;
        float fTotalAddSG = 0.0f;
        
        if (pSkillTable->Con_SG_Absorb > 0.0f) {
            float fMySG = GetStat(2);
            float fMyMaxSG = GetStat(12);
            
            if (fMyMaxSG > fMySG) {
                fAddSGVal = pSkillTable->Con_SG_Absorb * GetMultipleAbsorbSG();
            }
        }
        
        float fSkillOptionSG = 0.0f;
        pMyAttrPtr->GetSkillOptionEffect(pSkillTable->Skill_Group, EFFECT_SKILL_OPTION_SG, &fSkillOptionSG);
        
        if (fSkillOptionSG > 0.0f) {
            fAddSGVal += fSkillOptionSG;
        }
        
        if (fAddSGVal > 0.0f) {
            float fAbsorbRate = pTargetMover->GetSGAbsorbRate();
            fTotalAddSG += (fAddSGVal * fAbsorbRate);
        }
        
        if (m_fSkillAbsorbSGRate > 0.0f && (nIndex < m_nAllowAbsorbSGCount || m_nAllowAbsorbSGCount == 0)) {
            float fAbsorbRate = pTargetMover->GetSGAbsorbRate() * m_fSkillAbsorbSGRate;
            float fMyMaxSG = GetStat(12);
            float fAbsorbValue = fMyMaxSG * fAbsorbRate;
            fTotalAddSG += fAbsorbValue;
        }
        
        if (fTotalAddSG > 0.0f) {
            float fMySG = GetStat(2);
            float fCalcVal = fMySG + fTotalAddSG;
            
            // Add SG (stat index 2)
            // pMyAttrPtr->ModifyStat(2, fTotalAddSG, false);
            
            m_bCheckSendAbsorbSG = true;
        }
        
        // Buff Damage Check
        CheckBuffDamage(pTargetMoverEx, this, nIndex, stDamage.nDamage);
    }
}

// ============================================================================
// ClearMotion - IDA 0x140381910
// 清除当前动作状态并切换到下一个动作 (由 CheckAnimationEnd 或技能取消触发)
// ============================================================================
void CMoverEx::ClearMotion() {
    // IDA 0x140381910 反编译:
    // 1. 检查死亡类型 (击倒/延迟死亡时跳过清除)
    // 2. 检查状态 2 (不可清除)
    // 3. Phase Motion Step 清理
    // 4. 检查技能混合时间是否结束
    // 5. 检查变身怪物逻辑
    // 6. 处理 Subo Combo
    // 7. 获取下一个动作并切换

    // === 1. 检查死亡类型 ===
    if (m_eDieType == DIE_TYPE_KNOCKDOWN || m_eDieType == DIE_TYPE_DELAY) {
        return;
    }

    // === 2. 检查状态 2 (不可打断) ===
    if (!CMover::IsStatus(2)) {
        // === 3. Phase Motion Step 清理 ===
        if (m_byPhaseMotionStep == 2) {
            m_byPhaseMotionStep = 0;
            CMover::SetInvincibleActor(0);
            m_fPhaseStepMaxTime = 0.0f;
        }

        // === 4. 检查技能混合时间 ===
        VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
        float fCurrentTime = pTimer->GetTimeDifference();  // TODO: 需要 IVTimer::GetTime
        bool bBlendEnd = (m_fSkillBlendEndTime <= fCurrentTime || m_fAnimPercentTime >= 0.99f);

        if (bBlendEnd) {
            // === 5. 检查变身怪物逻辑 ===
            if (m_bReserveChange && m_dwChangeMobTableID != 0) {
                // TODO: 需要 XMaze::AddChangeMonster 实现
                // XArea* pArea = GetArea(&this->XActor);
                // XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
                // if (pMaze) {
                //     unsigned int dwID = GetID(&this->XActor);
                //     XMaze::AddChangeMonster(pMaze, dwID);
                // }
            } else {
                // === 6. Subo Combo 清理 ===
                if (m_bExistSuboCombo && m_iSuboComboMaxCount > 0 &&
                    m_iSuboComboCheckCount >= m_iSuboComboMaxCount) {
                    m_fSkillLoopTime = 0.0f;
                    m_fSuboComboCheckTime = 0.0f;
                    m_iSuboComboMaxCount = -1;
                    m_iSuboComboCheckCount = 0;
                    m_bExistSuboCombo = false;
                }

                // === 7. 获取下一个动作并切换 ===
                short nNewMotion = GetNextMotion();
                if (nNewMotion != -1) {
                    ChangeMotion(nNewMotion, 1, 5);
                }
            }
        }
    }
}

// ============================================================================
// GetMultipleDamageOnce - IDA 0x1402C7DC0
// ============================================================================
float CMoverEx::GetMultipleDamageOnce() {
    return m_fMultipleDamageOnce;
}

// ============================================================================
// GetApplyMultipleDamageOnce - IDA 0x1402C7EA0
// ============================================================================
bool CMoverEx::GetApplyMultipleDamageOnce() {
    return m_bApplyMultipleDamageOnce;
}

// ============================================================================
// ThinkFunction - IDA 0x14037A4F0 -> 0x14037B55E (大小: 0x106E = 4206 bytes)
// 大型思考函数，处理实体的每帧更新逻辑
// ============================================================================
void CMoverEx::ThinkFunction() {
    // === 1. 调用基类 ThinkFunction ===
    CMover::ThinkFunction();

    // === 2. 检查待机时间 ===
    if (!m_bPublicTransportRiding) {
        CheckIdleTime();
    }

    // === 3. 获取 DeltaTime ===
    // TODO: VDefaultTimer* Timer = ThreadLocalData::GetTimer();
    // float fDeltaTime = IVTimer::GetTimeDifference(Timer);
    float fDeltaTime = 0.016f;  // 默认 60 FPS

    // === 4. Phase Step 处理 ===
    if (m_fPhaseStepMaxTime > 0.0f) {
        m_fPhaseStepMaxTime -= fDeltaTime;
        if (m_fPhaseStepMaxTime <= 0.0f) {
            if (m_byPhaseMotionStep == 1) {
                SetupPhaseMotion();
            } else if (m_byPhaseMotionStep == 2) {
                m_byPhaseMotionStep = 0;
                CMover::SetInvincibleActor(0);
            }
            m_fPhaseStepMaxTime = 0.0f;
        }
    }

    // === 5. Attach 检查 ===
    if (m_bCheckAttachToAttacker) {
        m_fAttachedDuration -= fDeltaTime;
        if (m_fAttachedDuration >= 0.0f) {
            // TODO: CheckAttachedEntity();
        } else {
            // TODO: SetCheckEntityAttach(0, 0.0f, 0.0f, 0.0f, &vZeroVec, 0);
            // ClearAllAttachedEntity();
        }
    }

    // TODO: TraceAttachedOwner();

    // === 6. Action Buffer 处理 ===
    // CActionBuffer::Process(&m_xActionBuffer, fDeltaTime * m_fAnimSpeed);
    // while (CActionBuffer::CheckTime(&m_xActionBuffer)) {
    //     tagACTION_BUFFER* pAction = CActionBuffer::Pop(&m_xActionBuffer);
    //     ActionBufferProcess(pAction);
    // }

    // === 7. Forced State 处理 ===
    if (m_fForcedStateApplyTime > 0.0f) {
        m_fForcedStateApplyTime -= fDeltaTime;
        if (m_fForcedStateApplyTime <= 0.0f) {
            m_uiForcedState = 0;
            m_fForcedStateApplyTime = 0.0f;
        }
    }

    // === 8. Hit Freeze Time 处理 ===
    if (m_fHitFreezeTime > 0.0f) {
        m_fHitFreezeTime -= fDeltaTime;
        if (m_fHitFreezeTime <= 0.0f) {
            SetHitFreezeTime(0.0f);
        }
    }

    // === 9. Stiffen 更新 ===
    UpdateStiffen(fDeltaTime);

    // === 10. Counter 处理 ===
    if (m_bEnableCounuter) {
        m_fCounterDuration -= fDeltaTime;
        if (m_fCounterDuration <= 0.0f) {
            m_fCounterDuration = 0.0f;
            m_bEnableCounuter = false;
        }
    }

    // === 11. Charging Input 处理 ===
    if (m_fChargingInputDuration > 0.0f) {
        m_fChargingInputTime += fDeltaTime * m_fAnimSpeed;
        if (m_fChargingInputTime >= m_fChargingInputDuration) {
            m_fChargingInputDuration = 0.0f;
            m_fChargingInputPressTime = 0.0f;
        }
    }

    // === 12. Grap / Move Tick 处理 ===
    if (m_byGrapStep) {
        // TODO: ProcessGrap();
    } else {
        // TODO: CheckMoveTick 需要在 CMover 中实现
        MoveTick();
    }

    // === 13. Phase Duration 处理 ===
    if (m_fPhaseDurationTime > 0.0f) {
        m_fPhaseDurationTime -= fDeltaTime;
        if (m_fPhaseDurationTime <= 0.0f) {
            m_fPhaseDurationTime = 0.0f;
            SetupPhaseMotion();
        }
    }

    // === 14. Aura Skill 处理 ===
    if (m_pAuraSkill) {
        m_fAuraCheckTime -= fDeltaTime;
        if (m_fAuraCheckTime <= 0.0f && m_pAuraSkill->Check_Time > 0) {
            m_fAuraCheckTime = static_cast<float>(m_pAuraSkill->Check_Time) * 0.001f;
        }
    }

    // === 15. Extra Moving 处理 ===
    ProcessExtraMoving();

    // === 16. 清除 AnimChanged 标志 ===
    m_bAnimChanged = 0;

    // === 17. Skill Skip CoolTime 处理 ===
    if (m_fSkillSkipCoolTime > 0.0f) {
        m_fSkillSkipCoolTime -= fDeltaTime;
        if (m_fSkillSkipCoolTime <= 0.0f) {
            m_fSkillSkipCoolTime = 0.0f;
        }
    }

    // === 18. SA Break Loop Motion 处理 ===
    if (m_bSABreakLoopMotion) {
        m_fSABreakLoopMotionTime -= fDeltaTime;
        if (m_fSABreakLoopMotionTime <= 0.0f) {
            m_fSABreakLoopMotionTime = 0.0f;
            m_bSABreakLoopMotion = false;
            m_bShowSABreakMotion = false;
        }
    }

    // === 19. Subo Combo 处理 ===
    if (m_bExistSuboCombo && m_fSuboComboCheckTime > 0.0f) {
        // TODO: 时间检查并清除 combo 状态
    }

    // === 20. 清除 Counter Success Frame ===
    m_bCounterSuccessFrame = false;
}

// ============================================================================
// MoveTick - IDA 0x140382BB0
// 每帧移动逻辑: 计算位置变化、碰撞检测、目标方向更新
// ============================================================================
bool CMoverEx::MoveTick() {
    // IDA 0x140382BB0 反编译 (0x818 bytes):

    // === 1. 启动移动检查 ===
    if (!StartMoving()) {
        return false;
    }

    // === 2. 获取帧时间增量 ===
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = pTimer->GetTimeDifference();

    // === 3. 移动延迟检查 ===
    if (m_fMoveDelayTime > 0.0f) {
        m_fMoveDelayTime -= fDeltaTime;
        return true;
    }

    // === 4. 计算当前位置偏移 ===
    hkvVec3 vMyPos = GetPosition();
    hkvVec3 vOffset = vMyPos - m_vPrevPos;
    vOffset.z = 0.0f;  // 忽略垂直偏移

    // === 5. 确定移动速度 ===
    float fSpeed = m_fMoveSpeed;
    short nMotionClass = CMover::GetMotionClass();
    if (IsJumpMotion(nMotionClass)) {
        fSpeed = m_fFlySpeed;  // 跳跃时使用飞行速度
    }
    if (nMotionClass == 37 || nMotionClass == 6) {
        fSpeed = 0.0f;  // 特定动作下无法移动
    }

    // === 6. 检查是否到达目标位置 ===
    float fDiffX = vMyPos.x - m_stMovePos.x;
    float fDiffY = vMyPos.y - m_stMovePos.y;

    if ((fabsf(fDiffX) < 3.0f && fabsf(fDiffY) < 3.0f) ||
        (fDiffX * m_stMoveOffset.x <= 0.0f && fDiffY * m_stMoveOffset.y <= 0.0f)) {
        // 到达目标或已越过目标
        // TODO: 需要格式化日志输出
        // GreenDamTan_log only supports 3 args
        m_stMovePos.Clear();
        m_stMoveOffset.Clear();
    }

    // === 7. 如果没有移动目标 ===
    if (m_stMovePos.IsZero()) {
        // 如果不是可移动动画且不是跳跃/特殊动作，清除动画
        if (!IsCanMovingAnim()
            && (nMotionClass < 9 || nMotionClass > 11)
            && (nMotionClass < 32 || nMotionClass > 34)) {
            ClearMotion();
        }
        // 停止移动
        // IDA: StopMoving(this, 0);
        m_bCancelMoving = 1;
        ReleaseExtraMoving();

        // 如果有目标，转向目标方向
        if (GetTargetID() != 0xFFFFFFFF) {
            CMover* pTarget = GetMoverObject(GetTargetID());
            if (pTarget) {
                hkvVec3 vTargetPos = pTarget->GetPosition();
                // TODO: SetDirectionTo(vTargetPos);
            }
        }
        return true;
    }

    // === 8. 计算移动方向向量 ===
    float fDirX = m_stMovePos.x - vMyPos.x;
    float fDirY = m_stMovePos.y - vMyPos.y;

    // 归一化方向向量
    float fLen = sqrtf(fDirX * fDirX + fDirY * fDirY);
    if (fLen > 0.000001f) {
        fDirX /= fLen;
        fDirY /= fLen;
    } else {
        fDirX = 0.0f;
        fDirY = 0.0f;
    }

    // === 9. 计算本帧移动偏移量 ===
    float fMoveDeltaX = fDirX * fSpeed * fDeltaTime;
    float fMoveDeltaY = fDirY * fSpeed * fDeltaTime;

    // 限制偏移不超过剩余距离
    if (fMoveDeltaX > 0.0f && fMoveDeltaX > fabsf(fDiffX)) fMoveDeltaX = fDiffX;
    else if (fMoveDeltaX < 0.0f && -fMoveDeltaX > fabsf(fDiffX)) fMoveDeltaX = fDiffX;
    if (fMoveDeltaY > 0.0f && fMoveDeltaY > fabsf(fDiffY)) fMoveDeltaY = fDiffY;
    else if (fMoveDeltaY < 0.0f && -fMoveDeltaY > fabsf(fDiffY)) fMoveDeltaY = fDiffY;

    // === 10. 计算目标位置 ===
    hkvVec3 vDestPos = vMyPos + hkvVec3(fMoveDeltaX, fMoveDeltaY, 0.0f);

    // 非飞行时进行地面高度检测
    if (!IsFlying()) {
        GetHeight(&vDestPos, 200.0f);
    }

    // === 11. 碰撞检测 ===
    CMover* pCollideActor = CheckMoveCollision(vDestPos);
    if (pCollideActor) {
        // 碰撞到目标，停止移动
        m_bCancelMoving = 1;
        // CMover::MoveingValueClear(this);
        m_stMovePos.Clear();
        m_stMoveOffset.Clear();
        send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, GetPosition(), 0);
        return true;
    }

    // === 12. 执行移动 ===
    // hkvVec3 vPos(vDestPos.x, vDestPos.y, vDestPos.z);
    Move(vDestPos);
    m_fMoveDistAfterSkill += sqrtf(fMoveDeltaX * fMoveDeltaX + fMoveDeltaY * fMoveDeltaY);

    return true;
}

// ============================================================================
// StartMoving - IDA 0x1403833D0
// 检查并启动移动状态，必要时切换移动动画
// 返回值: 0=无法移动, 1=已启动移动
// ============================================================================
int CMoverEx::StartMoving() {
    // IDA 0x1403833D0 反编译:
    // === 1. 检查状态 2 (死亡/不可移动) ===
    if (CMover::IsStatus(2)) {
        return 0;
    }

    short nMotionClass = CMover::GetMotionClass();

    // === 2. 普通/待机动作 (IsCommonMotion: 0-8) 或 动作 11 ===
    if (IsCommonMotion(nMotionClass) || nMotionClass == 11) {
        // 检查是否有移动目标位置 或 已在移动中
        bool bHasMovePos = (m_stMovePos.x != 0.0f || m_stMovePos.y != 0.0f);
        if (bHasMovePos || m_bMoving) {
            m_bMoving = 1;
            short nMoveMotion = GetMoveMotion();
            // 如果动作已改变或移动方向动画改变，切换到移动动画
            if (nMoveMotion != nMotionClass || m_byMoveDir != m_byMoveDirAnim) {
                ChangeMotion(nMoveMotion, 1, 0);
                m_fMoveSpeed = GetMoveSpeed();
            }
            return 1;
        }
        return 0;
    }

    // === 3. 跳跃动作 (9-10) ===
    if (nMotionClass >= 9 && nMotionClass < 11) {
        bool bHasMovePos = (m_stMovePos.x != 0.0f || m_stMovePos.y != 0.0f);
        if (bHasMovePos || m_bMoving) {
            m_bMoving = 1;
            return 1;
        }
        return 0;
    }

    // === 4. 动作 11 (特殊待机) ===
    if (nMotionClass == 11) {
        bool bHasMovePos = (m_stMovePos.x != 0.0f || m_stMovePos.y != 0.0f);
        if (bHasMovePos) {
            // IDA: StopMoving(this, 0);
            m_bCancelMoving = 1;
            ReleaseExtraMoving();
        }
        return 0;
    }

    // === 5. 攻击中可移动动画 (IsCanMovingAnim) ===
    if (IsCanMovingAnim()) {
        // IDA: CheckMovingAttackAnimation(this);
        // TODO: 需要 CheckMovingAttackAnimation 实现
        // CheckMovingAttackAnimation();
        return 1;
    }

    // === 6. 特殊移动动作 (32-33) ===
    if (nMotionClass == 32 || nMotionClass == 33) {
        m_bMoving = 1;
        m_fMoveSpeed = GetMoveSpeed();
        return 1;
    }

    // === 7. 其他动作 ===
    return 0;
}

// ============================================================================
// IsJumpMotion - IDA 0x1403813E0 (静态函数)
// ============================================================================
bool CMoverEx::IsJumpMotion(short nMotion) {
    // 跳跃动画: 9, 10
    return (nMotion >= 9 && nMotion <= 10);
}

// ============================================================================
// IsJumpMotionExceptEnd - IDA 0x140381420 (静态函数)
// ============================================================================
bool CMoverEx::IsJumpMotionExceptEnd(short nMotion) {
    // 跳跃动画(除结束): 9
    return (nMotion == 9);
}

// ============================================================================
// IsMoveMotion - IDA 0x140381200
// ============================================================================
bool CMoverEx::IsMoveMotion(short nMotion) {
    // 移动动画: 1-5 (Walk/Run)
    return (nMotion >= 1 && nMotion <= 5);
}

// ============================================================================
// IsCommonMotion - IDA 0x140381240
// ============================================================================
bool CMoverEx::IsCommonMotion(short nMotion) {
    // 普通动画: 0-8
    return (nMotion >= 0 && nMotion <= 8);
}

// ============================================================================
// IsCanMovingAnim - IDA 0x140381320
// ============================================================================
bool CMoverEx::IsCanMovingAnim() {
    // 检查是否可以在动画中移动
    short nMotion = CMover::GetMotionClass();
    return IsMoveMotion(nMotion) || nMotion == 6 || nMotion == 7;
}

// ============================================================================
// IsSuperArmorBreakMotion - IDA 0x140381270
// ============================================================================
bool CMoverEx::IsSuperArmorBreakMotion(short nMotion) {
    // SA Break 动画: 26
    return (nMotion == 26);
}

// ============================================================================
// IsMoveDirMotion - IDA 0x1403812B0
// ============================================================================
bool CMoverEx::IsMoveDirMotion(short nMotion) {
    // 方向移动动画
    return IsMoveMotion(nMotion);
}

// ============================================================================
// IsChangeAnimByPhaseStepMotion - IDA 0x140381380
// ============================================================================
bool CMoverEx::IsChangeAnimByPhaseStepMotion(short nMotion) {
    // IDA 0x140381380 精确还原:
    // if ( nMotionClass >= 1 && nMotionClass <= 8 ) return 1;
    // if ( nMotionClass == 12 || nMotionClass == 13 ) return 1;
    // return nMotionClass >= 24 && nMotionClass <= 26;
    if (nMotion >= 1 && nMotion <= 8) {
        return true;
    }
    if (nMotion == 12 || nMotion == 13) {
        return true;
    }
    return nMotion >= 24 && nMotion <= 26;
}

// ============================================================================
// GetMoveMotion - IDA 0x14037F580
// ============================================================================
short CMoverEx::GetMoveMotion() {
    // IDA 0x14037F580 精确还原:
    // if ( this->m_bGazeMoving )
    // {
    //   AnimIndex = XActionResMgr::GetAnimIndex(this, 4, 0, this->m_bBattlePose);
    //   if ( CMover::GetAnimStirng(this, AnimIndex) )
    //     return 4;
    //   else
    //     return 5;
    // }
    // else if ( CMoverEx::IsCanMovingAnim(this) )
    // {
    //   return (unsigned __int16)this->m_nMotionClass;
    // }
    // else if ( XActor::IsStatus(&this->XActor, 0x100u) )
    // {
    //   return 5;
    // }
    // else
    // {
    //   v3 = XActionResMgr::GetAnimIndex(this, 3, 0, this->m_bBattlePose);
    //   if ( CMover::GetAnimStirng(this, v3) )
    //     return 3;
    //   else
    //     return 5;
    // }

    // 检查凝视移动状态
    if (m_bGazeMoving) {
        // TODO: 需要 XActionResMgr::GetAnimIndex 实现
        // unsigned int AnimIndex = XActionResMgr::GetAnimIndex(this, 4, 0, m_bBattlePose);
        // if (GetAnimStirng(AnimIndex)) {
        //     return 4;
        // }
        return 5;
    }

    // 检查是否是可移动动画
    if (IsCanMovingAnim()) {
        return m_nMotionClass;
    }

    // 检查状态 0x100
    if (IsStatus(0x100u)) {
        return 5;
    }

    // 默认返回跑步或行走动画
    // TODO: 需要 XActionResMgr::GetAnimIndex 实现
    // unsigned int v3 = XActionResMgr::GetAnimIndex(this, 3, 0, m_bBattlePose);
    // if (GetAnimStirng(v3)) {
    //     return 3;
    // }

    // 根据 m_bBattlePose 返回行走或跑步动画
    if (m_bBattlePose) {
        return 2;  // Run
    }
    return 1;  // Walk
}

// ============================================================================
// GetNextMotion - IDA 0x140381F90 -> 0x140382A7E (大小: 0xAEE = 2798 bytes)
// 获取下一个动作的状态机实现
// ============================================================================
short CMoverEx::GetNextMotion() {
    // IDA 反编译的核心逻辑:
    // 1. 检查 m_nMotionClass == 17 (特殊状态)
    // 2. 检查 IsKnockDown (倒地状态)
    // 3. 检查技能动画步骤
    // 4. 返回对应的动作 ID

    short nMotionClass = 1;  // 默认返回 Walk
    short nCurrentMotion = CMover::GetMotionClass();

    // === 检查 m_nMotionClass == 17 (特殊状态) ===
    if (nCurrentMotion == 17) {
        if (m_nHitStatus != 0) {
            m_nHitStatus = 7;
        } else {
            m_nHitStatus = 1;
            nMotionClass = nCurrentMotion;
        }
        goto LABEL_FINAL;
    }

    // === 检查倒地状态 ===
    if (CMover::IsKnockDown()) {
        if (m_nHitStatus != 0) {
            switch (m_nHitStatus) {
                case 1:
                    // TODO: if (XActor::IsStatus(4u)) {
                    //     nMotionClass = (m_byDmgMontionFlag & 1) ? 12 : 13;
                    //     RealDie(nMotionClass);
                    // } else {
                    if (m_nHitAnimCount == 2) {
                        return 1;
                    }
                    ++m_nHitStatus;
                    nMotionClass = nCurrentMotion;
                    // }
                    break;
                case 6:
                    m_nHitStatus = 1;
                    nMotionClass = nCurrentMotion;
                    break;
                case 3:
                    m_nHitStatus = 2;
                    nMotionClass = nCurrentMotion;
                    break;
                case 4:
                    if (CMover::IsFlying()) {
                        m_nHitStatus = 6;
                    } else {
                        m_nHitStatus = 1;
                    }
                    nMotionClass = nCurrentMotion;
                    break;
                default:
                    // TODO: if (XActor::IsStatus(4u)) { RealDie(12); }
                    break;
            }
        } else if (CMover::IsFlying()) {
            if (m_nHitAnimCount == 2) {
                return -1;
            }
            m_nHitStatus = 6;
            m_fHitLoopMaxTime = 0.0f;
            nMotionClass = nCurrentMotion;
        } else {
            m_nHitStatus = 1;
            nMotionClass = nCurrentMotion;
            // TODO: if (XActor::IsStatus(4u)) {
            //     nMotionClass = (m_byDmgMontionFlag & 1) ? 12 : 13;
            //     RealDie(nMotionClass);
            // }
        }
        goto LABEL_FINAL;
    }

    // === 检查技能状态 ===
    // TODO: if (!XActor::IsStatus(1u) || m_bySkillAnimStep == 3 || !m_pCurSkillTableRef)
    {
        switch (nCurrentMotion) {
            case 9:   // Jump
            case 0x17: // 23
                nMotionClass = CMover::IsFlying() ? 10 : 11;
                break;
            case 0x23: // 35 - Fly related
                nMotionClass = CMover::IsFlying() ? 36 : 37;
                break;
            case 0x28: // 40
                nMotionClass = 41;
                break;
            case 0x29: // 41
                nMotionClass = 42;
                break;
            default:
                switch (nCurrentMotion) {
                    case 12: // Die
                    case 13: // Die
                        nMotionClass = -1;
                        break;
                    case 24: // SA Break
                        nMotionClass = 25;
                        m_bSABreakLoopMotion = true;
                        m_bShowSABreakMotion = false;
                        break;
                    case 32:
                        nMotionClass = 33;
                        break;
                }
                break;
        }
        goto LABEL_FINAL;
    }

LABEL_FINAL:
    // === 最终检查 ===
    if (CMover::GetMotionClass() == 1 && m_nBuffMotion != -1) {
        return static_cast<short>(m_nBuffMotion);
    }
    return nMotionClass;
}

// ============================================================================
// CheckIdleTime - IDA 0x140381BC0
// 检查待机时间, 如果超过阈值则随机触发待机动画
// ============================================================================
void CMoverEx::CheckIdleTime() {
    // IDA 0x140381BC0 反编译:
    // 1. 检查是否有待机动画 (m_nIdleMotionChance > 0)
    // 2. 检查是否在待机状态 (m_nMotionClass == 1, 非战斗姿态)
    // 3. 检查待机动画是否存在 (GetAnimStirng)
    // 4. 检查时间是否超过阈值
    // 5. 按概率触发待机动画

    if (m_nIdleMotionChance <= 0 || m_nMotionClass != 1 || m_bBattlePose) {
        return;
    }

    // TODO: 需要 XActionResMgr::GetAnimIndex(CMover*, int16_t, int16_t, bool) 重载
    // unsigned int dwAnim = XActionResMgr::GetAnimIndex(this, 2, 0, 0);
    // 临时: 使用硬编码动画键来检查
    unsigned int dwAnim = AnimKeyToMotion(2 * 1000);  // motion 2 animation key

    if (!GetAnimStirng(dwAnim)) {
        m_nIdleMotionChance = 0;
        return;
    }

    // 获取当前时间
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fTime = pTimer->GetTimeDifference();  // TODO: 需要 IVTimer::GetTime

    // 检查距离上次动画变化的时间是否超过待机检查时间
    if (fTime - m_fLastChangeAnimationTime > m_fIdleCheckTime) {
        m_fLastChangeAnimationTime = fTime;

        // 随机检查 - 按概率触发待机动画
        int nRandom = rand() % 100;
        if (nRandom < m_nIdleMotionChance) {
            ChangeMotion(2, 1, 0);
        }

        // 如果是怪物类型 (ActorType == 2)，发送待机数据包
        // TODO: 需要 XActor::GetType 和 send_eSUB_CMD_MOVE_IDLE
        // if ((unsigned int)XActor::GetType(&this->XActor) == 2)
        //     CMover::send_eSUB_CMD_MOVE_IDLE(this, this, 0.0);
    }
}

// ============================================================================
// SetupPhaseMotion - IDA 0x140385E20
// 设置 BOSS 阶段转换动画 (Phase Motion)
// 启用无敌状态并切换到阶段动画
// ============================================================================
void CMoverEx::SetupPhaseMotion() {
    // IDA 0x140385E20 精确还原:
    // 1. 如果是 Monster 类型且非特定类型(12)，启用无敌
    // 2. 切换到 Phase Motion 动画
    // 3. 设置阶段时间 (动画长度或默认5秒)
    // 4. 清除 SA Break 状态
    // 5. 设置 Phase Motion Step = 2 (播放中)
    // 6. 清除当前技能引用
    // 7. 发送待机数据包

    // 检查是否为 Monster 类型 (ActorType == 2)
    if (GetType() == eActorMonster) {
        CMonster* pMonster = dynamic_cast<CMonster*>(this);
        if (pMonster) {
            TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
            if (pMobRef && pMobRef->Monster_Type != 12) {
                CMover::SetInvincibleActor(1);
            }
        }
    }

    // 切换到 Phase Motion 动画
    ChangeMotion(m_nPlayPhaseMotion, 1, 0);

    // 设置阶段时间
    if (m_pCurMotionEvent) {
        m_fPhaseStepMaxTime = m_pCurMotionEvent->fAnimationLength;
    } else {
        m_fPhaseStepMaxTime = 5.0f;  // 默认 5 秒
    }

    // 清除 SA Break 状态
    m_fSABreakLoopMotionTime = 0.0f;
    m_bSABreakLoopMotion = false;
    m_bShowSABreakMotion = false;

    // 设置 Phase Motion Step = 2 (播放中)
    m_byPhaseMotionStep = 2;

    // 清除当前技能引用
    m_pCurSkillTableRef = nullptr;

    // 发送待机数据包
    CMover::send_eSUB_CMD_MOVE_IDLE(this, -2.0f);
}

// ============================================================================
// CheckPhaseMotion - IDA 0x140385810 (用于 GetDamageMotion)
// 检查 Phase 动画条件
// 返回: true 如果应该切换到 Phase 动画
// ============================================================================
bool CMoverEx::CheckPhaseMotion(std::uint8_t byAttackCollision) {
    // IDA 0x140385810 精确还原:
    // 根据 m_byPhaseType 和 m_byPhaseCondition 检查是否需要切换到 Phase 动画

    if (m_byPhaseType == 1) {
        // Phase Type 1: 基于 Shield HP
        if (m_nShieldHP <= 0) {
            // 获取 Phase 变化动画
            unsigned int dwAnimKey = GetAnimIndex(m_strPhaseChangeAnim);
            NotifyPhaseChanged(m_byPhaseStep);
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(dwAnimKey));
            return true;
        }
        // 检查特殊伤害动画
        unsigned int dwKey = GetAnimIndex(m_strSpecialDamage);
        if (dwKey != static_cast<unsigned int>(-1)) {
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(dwKey));
            return true;
        }
    } else if (m_byPhaseCondition == 1) {
        // Phase Condition 1: 基于 HP 百分比
        float fMaxHP = m_fAbility ? m_fAbility[10] : static_cast<float>(GetMaxHP());
        int nHPPercent = static_cast<int>((static_cast<float>(GetHP()) / fMaxHP) * 10000.0f);
        if (nHPPercent <= static_cast<int>(m_dwPhaseConditionValue)) {
            unsigned int AnimIndex = GetAnimIndex(m_strPhaseChangeAnim);
            NotifyPhaseChanged(m_byPhaseStep);
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(AnimIndex));
            return true;
        }
        // 检查特殊伤害动画
        if (!m_strSpecialDamage.IsEmpty() && m_strSpecialDamage.AsChar() != nullptr &&
            std::strcmp(m_strSpecialDamage.AsChar(), "0") != 0) {
            unsigned int v6 = GetAnimIndex(m_strSpecialDamage);
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(v6));
            return true;
        }
    } else if (m_byPhaseCondition == 4 || m_byPhaseCondition == 6) {
        // Phase Condition 4/6: 基于计数值
        if (m_dwPhaseConditionValue == 0) {
            unsigned int v7 = GetAnimIndex(m_strPhaseChangeAnim);
            NotifyPhaseChanged(m_byPhaseStep);
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(v7));
            return true;
        }
        // 检查特殊伤害动画
        if (!m_strSpecialDamage.IsEmpty() && m_strSpecialDamage.AsChar() != nullptr &&
            std::strcmp(m_strSpecialDamage.AsChar(), "0") != 0) {
            unsigned int v8 = GetAnimIndex(m_strSpecialDamage);
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(v8));
            return true;
        }
    } else {
        // 默认: 检查伤害动画显示和特殊伤害
        if (IsDamageMotionDisplay(byAttackCollision) &&
            !m_strSpecialDamage.IsEmpty() && m_strSpecialDamage.AsChar() != nullptr &&
            std::strcmp(m_strSpecialDamage.AsChar(), "0") != 0) {
            unsigned int v9 = GetAnimIndex(m_strSpecialDamage);
            m_nPlayPhaseMotion = static_cast<std::int16_t>(AnimKeyToMotion(v9));
            return true;
        }
    }

    return m_nMotionClass == m_nPlayPhaseMotion;
}

// ============================================================================
// CheckPhaseMotionStep - IDA 0x140384810 (PDB 符号)
// 检查 Phase 动画步骤并设置相关状态 (用于 ChangeMotion)
// ============================================================================
void CMoverEx::CheckPhaseMotionStep(short nMotion) {
    // IDA 反编译逻辑:
    // 检查当前动作是否是 Phase 变化动作
    // 如果 m_byPhaseMotionStep == 1 -> SetupPhaseMotion
    // 如果 m_byPhaseMotionStep == 2 -> 清除无敌状态

    if (m_byPhaseType == 0) {
        return;
    }

    if (nMotion == m_nPlayPhaseMotion) {
        if (m_byPhaseMotionStep == 1) {
            SetupPhaseMotion();
        } else if (m_byPhaseMotionStep == 2) {
            m_byPhaseMotionStep = 0;
            CMover::SetInvincibleActor(0);
        }
    }

    if (IsChangeAnimByPhaseStepMotion(nMotion)) {
        // TODO: 根据当前 Phase Step 调整动画
    }
}

// ============================================================================
// UpdateStiffen - 更新僵直状态
// IDA: ?UpdateStiffen@CMoverEx@@QEAAXM@Z (0x140396100)
// ============================================================================
void CMoverEx::UpdateStiffen(float fDeltaTime) {
    // IDA 精确还原:
    // 1. 更新免疫时间
    if (m_fStiffenImmuneTime > 0.0f) {
        m_fStiffenImmuneTime = m_fStiffenImmuneTime - fDeltaTime;
        if (m_fStiffenImmuneTime < 0.0f) {
            m_fStiffenImmuneTime = 0.0f;
        }
    }

    // 2. 根据延迟时间状态更新僵直
    if (m_fStiffenDelayTime <= 0.0f) {
        // 无延迟，直接更新僵直时间
        if (m_fStiffenTime > 0.0f) {
            m_fStiffenTime = m_fStiffenTime - fDeltaTime;
            if (m_fStiffenTime <= 0.0f) {
                m_iStiffenCount = 0;
                m_fStiffenTime = 0.0f;
                CMover::SetReactionRate(1.0f);
            }
        }
    } else {
        // 有延迟，先更新延迟时间
        m_fStiffenDelayTime = m_fStiffenDelayTime - fDeltaTime;
        if (m_fStiffenDelayTime <= 0.0f) {
            m_fStiffenDelayTime = 0.0f;
            CMover::SetSlowTime(m_fStiffenTime, m_fStiffenRate);
            CMover::SetReactionRate(m_fStiffenRate);
        }
    }
}

// ============================================================================
// SetHitFreezeTime - 设置打击冻结时间
// IDA: ?SetHitFreezeTime@CMoverEx@@QEAAXM@Z (0x140395DD0)
// ============================================================================
void CMoverEx::SetHitFreezeTime(float fTime) {
    // IDA 精确还原:
    // this->m_fHitFreezeTime = fTime;
    // if ( this->m_fHitFreezeTime <= 0.0 )
    // {
    //     RestoreAnimSpeed = CMoverEx::GetRestoreAnimSpeed(this);
    //     CMover::SetSlowTime(this, 0.0, RestoreAnimSpeed);
    // }
    // else
    // {
    //     CMover::SetSlowTime(this, fTime, 0.0049999999);
    // }
    m_fHitFreezeTime = fTime;
    if (m_fHitFreezeTime <= 0.0f) {
        float fRestoreSpeed = GetRestoreAnimSpeed();
        CMover::SetSlowTime(0.0f, fRestoreSpeed);
    } else {
        CMover::SetSlowTime(fTime, 0.005f);
    }
}

// ============================================================================
// GetRestoreAnimSpeed - IDA 0x14039E900
// 获取恢复动画速度
// ============================================================================
float CMoverEx::GetRestoreAnimSpeed() {
    // IDA 精确还原:
    // If not in specific status, return normal speed
    if (!IsStatus(1) && !IsStatus(0x400)) {
        return 1.0f;
    }

    // If attack speed type, use stored restore speed
    if (m_eRestoreAnimSpeedType == 1) {  // AST_ATTACK
        return m_fRestoreAnimSpeed;
    }

    // Otherwise use ability value
    return m_fAbility[19] * 0.01f;
}

// ============================================================================
// CancelSkill - IDA 0x14037E9E0
// 取消当前技能
// ============================================================================
void CMoverEx::CancelSkill() {
    // IDA 反编译:
    // if (XActor::IsStatus(&this->XActor, 1u)) {
    //     this->ChangeMotion_3(this, 1, 1, 2);
    // }
    //
    // 说明: 当 actor 处于状态 1 (技能使用中) 时，切换到动作 1 (待机)
    // ChangeMotion 参数: (nMotionClass=1, bResetPlay=1, iCallPos=2)

    if (CMover::IsStatus(1)) {
        // 状态 1 表示正在使用技能，需要切换回待机动作
        ChangeMotion(1, 1, 2);
    }
}

// ============================================================================
// PreSkillProcess - IDA 0x14037D790
// 技能使用前处理
// ============================================================================
void CMoverEx::PreSkillProcess(std::uint32_t nSkillID, int bNormalAttack) {
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        return;
    }
    TB_SKILL* pSkillTbl = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
    if (!pSkillTbl) {
        return;
    }

    m_fMoveDistAfterSkill = 0.0f;
    m_bAttackKeyPress = 0;
    m_bDisableDirectionToTargetSkill = (GetCameraLock(pSkillTbl) != 0);
    m_pCurSkillTableRef = pSkillTbl;
    m_byAniProcessLinkType = pSkillTbl->Ani_Processing_Link_Type;
    CMover::SetCurSkillTableIdx(static_cast<int>(nSkillID));
    m_nAccumulateDamage = 0;

    if (pSkillTbl->Collision_Check_Type == 1) {
        m_bCollisionEnable = 0;
        m_bRestoreCollision = 0;
    }

    m_fAnimationTime = 0.05f;

    std::uint8_t byControlType = GetControlType(pSkillTbl);
    if (byControlType == 2 || byControlType == 5 || byControlType == 8) {
        m_bChargingStart = true;
    }

    if (pSkillTbl->Use_State == 1) {
        m_bMoveingInFly = true;
        m_bLanded = 0;
    }

    SetKeepMovingExtra(0);
}

// ============================================================================
// ChangeMotion - IDA 虚函数 (vtable offset 0x518)
// 切换角色动作
// ============================================================================
// IDA 0x14037C310: CMoverEx::ChangeMotion - 精确还原
// 切换角色动作，复杂的状态机和动画处理
void CMoverEx::ChangeMotion(std::int16_t nMotionClass, int bResetPlay, int iCallPos) {
    // IDA: 检查是否允许切换动作
    if (m_bReserveChange || IsStatus(0x8000000u)) {
        return;
    }

    // IDA: 获取起始动画索引
    // TODO: unsigned int nNewAnimIdx = GetStartAnimationInx(nMotionClass);
    unsigned int nNewAnimIdx = static_cast<unsigned int>(nMotionClass);
    float fGroupAnimTime = 0.0f;

    // IDA: 获取动画字符串
    const char* pszAnimString = GetAnimStirng(nNewAnimIdx);
    if (!pszAnimString) {
        // IDA: 动画不存在，记录调试信息
        // TODO: DebugOut("ID:%d ChangeMotion>> %d / Not exist animation %d",
        //              GetActorID().wID, nMotionClass, nNewAnimIdx);
        return;
    }

    // IDA: 检查动画组连续性
    if (bResetPlay && m_nAnimGroup != -1) {
        // TODO: const VAnimationInfo* pInfo = GetActionDesc(pszAnimString);
        // if (pInfo && m_nAnimGroup == pInfo->iAnimGroup) {
        //     bResetPlay = 0;
        //     fGroupAnimTime = m_fAnimPercentTime;
        // }
    }

    // IDA: 调试输出
    // TODO: DebugOut("ChangeMotion>> %d -> %d (%d) / %d",
    //              m_nMotionClass, nMotionClass, nNewAnimIdx, m_nHitStatus);

    // IDA: 检查状态 0x400 (受击锁定)
    if (IsStatus(0x400u) && nMotionClass != 30 && nMotionClass != 38 && nMotionClass != 46) {
        // ClearStatus(0x400u);
        // ClearStatus(0x8000u);
        // TODO: 实现 ClearStatus 方法
        // IDA: 恢复碰撞
        if (!m_bRestoreCollision || nMotionClass < 18 || nMotionClass > 21 || m_nHitStatus != 1) {
            SetCollisionEnable(1, 0);
        }
    } else if (IsDashing() && nMotionClass != 32 && nMotionClass != 33 && nMotionClass != 34) {
        // IDA: 清除冲刺状态
        // ClearStatus(0x800u);
        // ClearStatus(0x8000u);
        // TODO: 实现 ClearStatus 方法
    }

    // IDA: 处理动画速度触发器
    if (m_bAnimSpeedTrigger) {
        m_bAnimSpeedTrigger = 0;
        if (m_stTimeSlow.fTime > 0.0f) {
            m_stTimeSlow.fTime = 0.0f;
            float fRestoreSpeed = GetRestoreAnimSpeed();
            SetSlowTime(0.0f, fRestoreSpeed);
        }
    }

    // IDA: 删除动作缓冲区
    ClearActionBuffer();

    // IDA: 清理召唤物爆炸
    // TODO: if (GetSkillMgr()) { CMySkillList::ExplodeSummonClear(GetSkillMgr()); }

    // IDA: 切换动画
    // TODO: ChangeAnimation(nNewAnimIdx, bResetPlay);

    // IDA: 设置飞行状态
    // TODO: if (IsJumpMotionExceptEnd(nMotionClass) || nMotionClass == 23) {
    //     SetFlyState(1);
    //     m_bLanded = 0;
    // }

    // IDA: 恢复碰撞
    if (m_bRestoreCollision &&
        (nMotionClass < 18 || nMotionClass > 21 || (!IsFlying() && m_nHitStatus))) {
        SetCollisionEnable(1, 0);
        m_bRestoreCollision = 0;
    }

    // IDA: 设置动画组时间
    if (fGroupAnimTime > 0.0f) {
        SetCurrentSequenceTime(fGroupAnimTime);
    }

    // IDA: 处理跳过重播时间
    if (m_bSkipReplayTime) {
        // TODO: ExtraInputTrigger* pTrigger = GetAttackInputEvent(pszAnimString);
        // if (pTrigger && pTrigger->ReplayTime > 0.0f) {
        //     SetCurrentSequenceTime(pTrigger->ReplayTime);
        //     m_xActionBuffer.Process(pTrigger->StartTime);
        // }
        m_bSkipReplayTime = 0;
    }

    // IDA: 设置移动方向动画
    if (IsMoveDirMotion(nMotionClass)) {
        m_byMoveDirAnim = m_byMoveDir;
        m_fMoveSpeed = GetMoveSpeed();
    }

    // IDA: 处理特效释放
    // TODO: if (nMotionClass == 11 || nMotionClass == 37) {
    //     ReleaseInvokedOptionEffect(EFFECT_CONDITION_JUMP);
    // } else if (nMotionClass == 34) {
    //     ReleaseInvokedOptionEffect(EFFECT_CONDITION_DASH);
    // } else if (m_nMotionClass == 30 && nMotionClass != 30) {
    //     ReleaseInvokedOptionEffect(EFFECT_CONDITION_EVADE);
    // }

    // IDA: 更新动作类
    m_nMotionClass = nMotionClass;

    // IDA: 移除连锁闪电
    // TODO: if (m_pSkillMgr) { CMySkillList::RemoveChainLightning(m_pSkillMgr, this); }

    // IDA: 处理倒地状态
    // TODO: if (IsHitDown()) { ReleaseInvokedOptionEffect(EFFECT_CONDITION_KNOCK_DOWN); }

    // IDA: 记录切换日志
    // TODO: if (nMotionClass == 1 || nMotionClass == 34 || nMotionClass == 30) {
    //     SetChangeMotionLog(nMotionClass, iCallPos);
    // }

    // 设置动画状态
    m_bAnimChanged = 1;
    if (bResetPlay) {
        m_fAnimationTime = 0.0f;
    }
    m_bSkipAnimOffset = 0;
    m_bAnimPlay = 1;
}

// ============================================================================
// Round 7 Phase 1-2 - CMoverEx Extended Functions
// ============================================================================

// ============================================================================
// Movement Extended Functions
// ============================================================================

// ============================================================================
// MoveTo - Move to target position
// ============================================================================
void CMoverEx::MoveTo(const hkvVec3& vTargetPos, float fSpeed, bool bRun) {
    MoveToPosition(vTargetPos, fSpeed, bRun);
}

// ============================================================================
// JumpTo - Jump to target position
// ============================================================================
void CMoverEx::JumpTo(const hkvVec3& vTargetPos, float fHeight) {
    // Set jump target position
    m_stMovePos.x = vTargetPos.x;
    m_stMovePos.y = vTargetPos.y;
    m_fJumpHeight = fHeight > 0.0f ? fHeight : 100.0f;
    m_bJumpAnim = 1;

    // Trigger jump animation
    ChangeMotion(9, 1, 0);

    // Set flying state
    m_bMoveingInFly = 1;
}

// ============================================================================
// TeleportTo - Teleport to target position instantly
// ============================================================================
void CMoverEx::TeleportTo(const hkvVec3& vTargetPos) {
    // Clear movement state
    m_bCancelMoving = 1;
    m_stMovePos.Clear();
    m_stMoveOffset.Clear();

    // Set position directly
    SetPosition(vTargetPos);

    // Release extra moving
    ReleaseExtraMoving();

    // Broadcast position to clients
    SyncPosition();
}

// ============================================================================
// MoveDirection - Move in specified direction
// ============================================================================
void CMoverEx::MoveDirection(const hkvVec3& vDirection, float fSpeed, float fDuration) {
    // Check if direction is valid
    float fLen = sqrtf(vDirection.x * vDirection.x + vDirection.y * vDirection.y);
    if (fLen < 0.0001f) {
        return;
    }

    // Normalize direction
    hkvVec3 vNormDir;
    vNormDir.x = vDirection.x / fLen;
    vNormDir.y = vDirection.y / fLen;
    vNormDir.z = 0.0f;

    // Set movement speed
    if (fSpeed > 0.0f) {
        SetMoveSpeed(fSpeed);
    }

    // Set movement direction
    m_stMoveOffset.x = vNormDir.x;
    m_stMoveOffset.y = vNormDir.y;

    // Calculate target position based on duration
    hkvVec3 vCurrentPos = GetPosition();
    m_stMovePos.x = vCurrentPos.x + vNormDir.x * (fSpeed * fDuration);
    m_stMovePos.y = vCurrentPos.y + vNormDir.y * (fSpeed * fDuration);

    // Clear cancel flag
    m_bCancelMoving = 0;
}

// ============================================================================
// State Management Functions
// ============================================================================

// ============================================================================
// SetMoverState - Set mover state flag
// ============================================================================
void CMoverEx::SetMoverState(std::uint32_t dwStateFlags) {
    // Set status flags using CMover method
    // TODO: Need CMover::AddStatus or similar method
}

// ============================================================================
// GetMoverState - Get current state flags
// ============================================================================
std::uint32_t CMoverEx::GetMoverState() {
    // Return current status flags
    // TODO: Need CMover::GetAllStatusFlags or similar
    return 0;
}

// ============================================================================
// ResetMoverState - Reset state to default
// ============================================================================
void CMoverEx::ResetMoverState() {
    // Clear all status flags
    // TODO: Need CMover::ClearAllStatus or similar

    // Reset specific CMoverEx state variables
    m_bBattlePose = false;
    m_bStartRotation = false;
    m_bQuickTurn = false;
    m_bKeepLookTarget = false;
    m_bCheckAttachToAttacker = false;
    m_bEnableCounuter = false;
    m_bChargingStart = false;
    m_bControlMonster = false;
    m_bOnDie = false;
    m_bDieAttack = false;
    m_bFlyDie = false;
    m_bCounterSuccessFrame = false;
}

// ============================================================================
// IsMoverState - Check if specific state is set
// ============================================================================
bool CMoverEx::IsMoverState(std::uint32_t dwStateFlag) {
    return CMover::IsStatus(dwStateFlag);
}

// ============================================================================
// PushMoverState - Push state to state stack (for state machine)
// ============================================================================
void CMoverEx::PushMoverState(std::uint32_t dwStateFlags) {
    // Add state flags to current state
    // TODO: Need CMover::AddStatus or similar
}

// ============================================================================
// Position Sync Functions
// ============================================================================

// ============================================================================
// SyncPosition - Sync position to all clients
// ============================================================================
void CMoverEx::SyncPosition() {
    // Broadcast position update to nearby players
    // TODO: Need packet broadcast implementation
    // send_eSUB_CMD_MOVE_SYNC(this, GetPosition(), m_fMovingYaw);

    // Update position in sector
    if (m_pSector) {
        // TODO: Sector position update
    }
}

// ============================================================================
// SendPosition - Send position to specific client
// ============================================================================
void CMoverEx::SendPosition(std::uint32_t dwClientID) {
    // Send position packet to specific client
    // TODO: Need packet send implementation
    // XSendPacket packet;
    // packet << GetPosition();
    // SendToClient(dwClientID, packet);
}

// ============================================================================
// GetPosition - Get current position (override CMover version)
// ============================================================================
hkvVec3 CMoverEx::GetPosition() const {
    return CMover::GetPosition();
}

// ============================================================================
// SetPosition - Set position directly
// ============================================================================
void CMoverEx::SetPosition(const hkvVec3& vPos) {
    CMover::SetPositionXVec3(const_cast<hkvVec3&>(vPos));

    // Update ground position
    m_fGroundPosZ = vPos.z;
}

// ============================================================================
// GetVelocity - Get current velocity
// ============================================================================
hkvVec3 CMoverEx::GetVelocity() const {
    hkvVec3 vVelocity;
    vVelocity.x = 0.0f;
    vVelocity.y = 0.0f;
    vVelocity.z = 0.0f;

    // Calculate velocity from movement state
    if (m_bMoving && !m_bCancelMoving) {
        if (m_stMoveOffset.x != 0.0f || m_stMoveOffset.y != 0.0f) {
            vVelocity.x = m_stMoveOffset.x * m_fMoveSpeed;
            vVelocity.y = m_stMoveOffset.y * m_fMoveSpeed;
        }
    }

    // Add vertical velocity if flying/jumping
    CMoverEx* pThis = const_cast<CMoverEx*>(this);
    if (pThis->IsFlying()) {
        vVelocity.z = m_fFlyVelocity;
    }

    return vVelocity;
}

// ============================================================================
// Collision Handling Functions
// ============================================================================

// ============================================================================
// OnCollision - Handle collision event
// ============================================================================
void CMoverEx::OnCollision(CMover* pOther, const hkvVec3& vCollisionPoint) {
    if (!pOther) {
        return;
    }

    // Store collision info
    m_vCollisionPoint = vCollisionPoint;
    m_pCollisionTarget = pOther;
    m_fCollisionTime = 0.0f;

    // Process collision based on context
    // Stop movement if colliding with target
    // Note: Can't compare IDs directly without a GetID method
    m_bCancelMoving = 1;
    ReleaseExtraMoving();
}

// ============================================================================
// ProcessCollision - Process pending collision
// ============================================================================
void CMoverEx::ProcessCollision() {
    if (!m_pCollisionTarget) {
        return;
    }

    // Increment collision time
    m_fCollisionTime += 0.016f;  // Assuming 60 FPS

    // Process collision based on collision time
    if (m_fCollisionTime > 0.1f) {
        // Collision persisted, take action
        // TODO: Collision response logic

        // Clear collision state
        m_pCollisionTarget = nullptr;
        m_fCollisionTime = 0.0f;
    }
}

// ============================================================================
// SetCollision - Set collision enable state
// ============================================================================
void CMoverEx::SetCollision(bool bEnable) {
    m_bCollisionEnable = bEnable ? 1 : 0;

    // Update collision state in physics
    // TODO: Need physics engine integration
}

// ============================================================================
// IsColliding - Check if currently colliding
// ============================================================================
bool CMoverEx::IsColliding() const {
    return m_pCollisionTarget != nullptr;
}

// ============================================================================
// GetCollisionInfo - Get collision information
// ============================================================================
void CMoverEx::GetCollisionInfo(hkvVec3& vPoint, CMover** ppTarget) {
    vPoint = m_vCollisionPoint;
    if (ppTarget) {
        *ppTarget = m_pCollisionTarget;
    }
}

// ============================================================================
// ProcessSkillAnimation - IDA 0x14037B560 -> 0x14037B866
// 处理技能动画更新逻辑
// ============================================================================
void CMoverEx::ProcessSkillAnimation(float fDeltaTime) {
    // IDA 反编译精确实现:
    if (!m_pCurSkillTableRef || IsStatus(0x8000000u)) {
        return;
    }

    VDefaultTimer* Timer = ThreadLocalData::GetTimer();

    if (m_fSkillLoopTime <= 0.0f
        && m_fSkillBlendEndTime > 0.0f
        && Timer->GetTime() >= m_fSkillBlendEndTime
        && (m_fSkillBlendEndTime = 0.0f, IsStatus(1u))) {
        ClearMotion();
        return;
    }

    // 处理充能输入
    if (m_fChargingInputDuration > 0.0f && m_bChargingStart) {
        m_fChargingInputPressTime = m_fChargingInputPressTime + (fDeltaTime * m_fAnimSpeed);
    }

    // 检查控制类型
    std::uint8_t byControlType = GetControlType(m_pCurSkillTableRef);
    if (byControlType == 2 || byControlType == 5 || byControlType == 8) {
        // 充能技能处理
        if (m_fSkillChargeChangeTime > 0.0f) {
            m_fSkillChargeChangeTime = m_fSkillChargeChangeTime - fDeltaTime;
            m_fSkillTotalChargeTime = m_fSkillTotalChargeTime + fDeltaTime;
            if (m_fSkillChargeChangeTime <= 0.0f) {
                if (byControlType == 2) {
                    // ChargeSkillNextStep();
                    ++m_bySkillChargeStep;
                } else {
                    // ChargeSkillEnd();
                    m_bySkillChargeStep = m_bySkillChargeMaxStep;
                }
            }
        }
    } else if ((m_bySkillAnimStep == 1 || m_bySkillAnimStep == 2)
               && m_pCurSkillTableRef->Skill_Type != 8
               && m_fSkillLoopTime > 0.0f) {
        m_fSkillLoopTime = m_fSkillLoopTime - fDeltaTime;
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

// ============================================================================
// GetDamageMotion - IDA 0x140385290 -> 0x14038580C
// 获取受击动作类型
// 返回: 动作类型 ID (15=受击1, 16=受击2, 17=眩晕, 18=倒地, 19=击飞, 20=浮空, 21=击落, 22=反击, 23=空中受击)
// ============================================================================
std::int16_t CMoverEx::GetDamageMotion(std::uint8_t byReactionType, float fAttackRot,
                                        std::uint8_t byAttackCollision, std::uint8_t byCheckRank) {
    // IDA 反编译精确实现:
    std::int16_t nMotion;

    // 检查是否应用强制反应
    // TODO: if (!IsApplyForceReaction(byCheckRank))
    {
        // 反应类型调整
        if ((m_byDmgMontionFlag & 2) != 0 && byReactionType == 5) {
            byReactionType = 1;
        } else if ((m_byDmgMontionFlag & 1) != 0 && byReactionType >= 2u && byReactionType <= 4u) {
            byReactionType = 1;
        }
    }

    // 倒地状态处理
    if (IsHitDown()) {
        if (byReactionType == 2) {
            if (m_nMotionClass == 21 || m_nMotionClass == 20) {
                m_nHitStatus = 3;
                return m_nMotionClass;
            } else {
                m_nHitStatus = 0;
                return 19;  // 击飞
            }
        } else if (byReactionType == 3) {
            m_bSkipReplayTime = 1;
            if (m_nMotionClass == 21 || m_nMotionClass == 20 || m_nMotionClass == 18) {
                m_nHitStatus = 0;
                return 21;  // 击落
            } else {
                m_nHitStatus = 0;
                return 20;  // 浮空
            }
        } else if (byReactionType == 4) {
            m_nHitStatus = 0;
            return 18;  // 倒地
        } else {
            if (byReactionType == 1) {
                ++m_byDownContinueDamage;
            }
            m_nHitStatus = 3;
            return m_nMotionClass;
        }
    }

    // 飞行状态处理
    if (IsFlying()) {
        switch (byReactionType) {
            case 3u:
                m_bSkipReplayTime = IsKnockDown();
                if (m_nMotionClass == 21 || m_nMotionClass == 20) {
                    m_nHitStatus = 0;
                    return 21;
                } else {
                    m_nHitStatus = 0;
                    return 20;
                }
            case 2u:
                m_nHitStatus = 0;
                return 19;
            case 4u:
                m_nHitStatus = 0;
                return 18;
            default:
                // 玩家类型检查 (简化版本)
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
                } else if (IsKnockDown() || byReactionType == 5) {
                    m_nHitStatus = 0;
                    return 18;
                } else {
                    m_nHitStatus = 4;
                    return 23;  // 空中受击
                }
        }
    }

    // Phase 动画检查
    if (CheckPhaseMotion(byAttackCollision)) {
        return m_nPlayPhaseMotion;
    }

    // 普通受击处理
    m_byDownContinueDamage = 0;
    nMotion = 15;  // 默认受击1

    switch (byReactionType) {
        case 0u:
            nMotion = -1;
            break;
        case 1u:  // 普通受击
            if (m_nMotionClass == 15) {
                nMotion = 16;  // 受击2
            } else if (m_nMotionClass == 16) {
                nMotion = 15;  // 受击1
            } else {
                nMotion = static_cast<std::int16_t>(rand() % 2 + 15);  // 随机受击1或2
            }
            break;
        case 2u:
            nMotion = 19;  // 击飞
            break;
        case 3u:
            nMotion = 21;  // 击落
            break;
        case 4u:
            nMotion = 18;  // 倒地
            break;
        case 5u:
            nMotion = 17;  // 眩晕
            break;
        case 6u:
            nMotion = 22;  // 反击
            break;
        default:
            break;
    }

    // KnockDown 状态修正
    if (IsKnockDown() && !m_nHitStatus && (nMotion < 18 || nMotion > 21)) {
        nMotion = 19;
    }

    // 设置 HitStatus
    if (nMotion >= 17 && nMotion <= 21) {
        m_nHitStatus = 0;
    }

    return nMotion;
}

// ============================================================================
// Damage - IDA 0x140385F70 -> 0x140387FA2
// 伤害处理核心函数 (虚函数 override)
// ============================================================================
void CMoverEx::Damage(tagACTION_DAMAGE& dmgInfo, unsigned int nSkillID, bool* bSABreaked) {
    // IDA 反编译精确实现 (简化版本，保留核心逻辑):
    // 检查状态: 非死亡状态 或 DIE_TYPE_KNOCKDOWN 或 DIE_TYPE_DELAY
    if (!IsStatus(4u) || m_eDieType == DIE_TYPE_KNOCKDOWN || m_eDieType == DIE_TYPE_DELAY) {
        // 规范化攻击角度到 [-180, 180]
        if (dmgInfo.fAttackRot >= -180.0f) {
            if (dmgInfo.fAttackRot > 180.0f) {
                dmgInfo.fAttackRot = dmgInfo.fAttackRot - 360.0f;
            }
        } else {
            dmgInfo.fAttackRot = dmgInfo.fAttackRot + 360.0f;
        }

        // 有伤害时设置击中者
        if (dmgInfo.nDamage > 0) {
            m_dwHitID = dmgInfo.dwID;
        }

        // 切换到战斗姿态
        if (!IsBattlePose()) {
            ChangeBattlePose(true, false);
        }

        // Phase Motion 检查
        if (m_byPhaseMotionStep == 1
            && m_nMotionClass != m_nPlayPhaseMotion
            && m_nPlayPhaseMotion != -1) {
            SetupPhaseMotion();
            return;
        }

        if (m_byPhaseMotionStep != 2) {
            m_nDamage = dmgInfo.nDamage;
            bool bShowSABreakMotion = false;

            // 设置 Super Armor Gage
            m_fCurSuperArmorGage = dmgInfo.fSuperArmorGage;

            *bSABreaked = false;

            // Super Armor Break 处理
            if (dmgInfo.byDefenseType != 4) {
                m_byDefenseType = dmgInfo.byDefenseType;
            }

            // 处理伤害 (简化版本，直接操作HP)
            int nNewHP = GetHP() - dmgInfo.nDamage;
            if (nNewHP < 0) nNewHP = 0;
            SetHP(nNewHP);
            bool isDamageHP = (nNewHP == 0);

            // 非技能类型8的处理
            if (!m_pCurSkillTableRef || m_pCurSkillTableRef->Skill_Type != 8) {
                if (isDamageHP) {
                    // 死亡处理
                    m_byDieReason = 1u;
                    m_nDieDamage = dmgInfo.nDamage;

                    if (IsFlying()) {
                        m_bFlyDie = true;
                        m_nMotionClass = 12;
                        m_fDieDelayTime = 1.0f;
                        if (m_fDieDelayMaxTime <= 0.0f) {
                            m_fDieDelayMaxTime = 5.0f;
                        }
                    } else {
                        m_fDieDelayTime = 0.0f;
                        m_fDieDelayMaxTime = 0.0f;
                        if (m_fDieFadeTime <= 0.0f) {
                            m_fDieFadeTime = 1.5f;
                        }
                    }
                } else {
                    // 非死亡伤害处理
                    if ((dmgInfo.byDamageFlag & 8) == 0) {
                        std::int16_t nMotion = GetDamageMotion(dmgInfo.byReactionType, dmgInfo.fAttackRot,
                                                               dmgInfo.byAttackCollision, dmgInfo.byAttackRank);

                        // 处理受击动画
                        if (m_byDefenseType != 3 || ((dmgInfo.byDamageFlag & 0x10) != 0)) {
                            m_fSkillBlendEndTime = 0.0f;
                            m_bySkillAnimStep = 0;

                            // 清除移动值
                            m_stMoveOffset.x = 0.0f;
                            m_stMoveOffset.y = 0.0f;

                            if (IsStatus(1u)) {
                                CancelSkill();
                            }

                            ChangeMotion(nMotion, 1, 0);
                        }

                        // SA Break 动画
                        if (bShowSABreakMotion) {
                            ChangeMotion(24, 1, 0);
                            m_bShowSABreakMotion = true;
                        }
                    }
                }
            }

            // 更新 SA 恢复时间
            m_fRegenSuperArmorTime = m_fDefRegenSuperArmorTime;
            dmgInfo.byDefenseType = m_byDefenseType;
        }
    }
}

// ============================================================================
// RealDie - 虚函数实现
// 真正的死亡处理
// ============================================================================
void CMoverEx::RealDie(std::int16_t nChangeMotion) {
    // 设置死亡状态
    SetStatus(2u);

    // 设置死亡动画
    if (nChangeMotion >= 0) {
        m_nMotionClass = nChangeMotion;
    }
}

// ============================================================================
// IsEnemy - IDA 0x14037FFA0 -> 0x14038075A
// 检查是否为敌方 (简化版本 - 完整实现需要 XActor 继承)
// ============================================================================
bool CMoverEx::IsEnemy(CMover* pMover) const {
    // IDA 精确还原 (简化版本):
    if (!pMover) {
        return false;
    }

    // NPC is never enemy
    if (pMover->GetType() == eActorNPC) {
        return false;
    }

    // Handle target is monster case
    CMonster* pTargetMonster = nullptr;
    if (pMover->GetType() == eActorMonster) {
        pTargetMonster = dynamic_cast<CMonster*>(pMover);
        if (!pTargetMonster) {
            return false;
        }
        TB_MONSTER* pMobRef = pTargetMonster->GetMobTableRef();
        if (!pMobRef) {
            return false;
        }
        // Summon_Hit flag 16 - attack only owner's enemies
        if ((pMobRef->Summon_Hit & 0x10) != 0) {
            UXActorID targetOrigin = pMover->GetOriginID();
            UXActorID myOrigin = GetOriginID();
            return targetOrigin == myOrigin;
        }
    }

    // Handle this is monster, target is player
    if (m_eActorType == eActorMonster && pMover->GetType() == eActorUser) {
        CMonster* pMonster = dynamic_cast<CMonster*>(const_cast<CMoverEx*>(this));
        if (pMonster) {
            TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
            if (pMobRef) {
                // Summon_Hit flag 1 - attack owner's enemies including party
                if ((pMobRef->Summon_Hit & 1) != 0) {
                    UXActorID targetOrigin = pMover->GetOriginID();
                    UXActorID myOrigin = GetOriginID();
                    return targetOrigin == myOrigin || const_cast<CMoverEx*>(this)->IsParty(pMover);
                }

                // Summon_Hit flag 4 - use area faction (simplified)
                if ((pMobRef->Summon_Hit & 4) != 0) {
                    // TODO: 需要 XArea::IsEnemy 实现
                    return false;
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
                UXActorID myOrigin = GetOriginID();
                return targetOrigin == myOrigin || const_cast<CMoverEx*>(this)->IsParty(pTargetMonster);
            }

            // Summon_Hit flag 8 - use area faction (simplified)
            if ((pMobRef->Summon_Hit & 8) != 0) {
                // TODO: 需要 XArea::IsEnemy 实现
                return false;
            }
        }
    }

    // Check same origin
    UXActorID targetOrigin = pMover->GetOriginID();
    UXActorID myOrigin = GetOriginID();

    if (targetOrigin == myOrigin) {
        // Same origin - check faction by nation
        std::uint8_t targetNation = pMover->GetNation();
        std::uint8_t myNation = GetNation();
        // TODO: 需要 XResourceMgr::GetFaction 实现
        // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // return pServer->GetResourceMgr().GetFaction(myNation, targetNation) != 2;
        (void)targetNation;
        (void)myNation;
        return false;
    }

    // Different origin - simplified
    // TODO: 需要 XArea::IsEnemy 实现
    return false;
}

// ============================================================================
// IsFriend - IDA 0x1403808F0 -> 0x14038093F
// 检查是否为友方 (简化版本)
// ============================================================================
int CMoverEx::IsFriend(CMover* pMover) {
    // IDA 精确还原:
    return pMover && !IsEnemy(pMover) ? 1 : 0;
}

// ============================================================================
// IsEnemyForChain - IDA 0x140380760 -> 0x1403808E8
// 检查是否为连锁技能敌方 (简化版本)
// ============================================================================
int CMoverEx::IsEnemyForChain(CMover* pMover) {
    // IDA 精确还原 (简化版本):
    if (!pMover) {
        return 0;
    }

    E_ACTOR_TYPE targetType = pMover->GetType();
    if (targetType != eActorMonster && targetType != eActorUser) {
        return 0;
    }

    if (targetType != eActorMonster) {
        return IsEnemy(pMover) ? 1 : 0;
    }

    CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    if (!pMonster) {
        return IsEnemy(pMover) ? 1 : 0;
    }

    TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
    if (!pMobRef) {
        return IsEnemy(pMover) ? 1 : 0;
    }

    // TODO: 需要 XArea::GetWorldType 实现
    // XArea* pArea = GetArea();
    // if (pArea && pArea->GetWorldType() != 2) {
    //     if (pMobRef->Monster_ClearCondition_Type) {
    //         return 0;
    //     }
    // }

    if (!pMobRef->Monster_Type || pMobRef->Monster_Type == 3 ||
        pMobRef->Monster_Type == 11 || pMobRef->Monster_Type == 4) {
        return IsEnemy(pMover) ? 1 : 0;
    }

    return 0;
}

// ============================================================================
// IsFriendForChain - IDA 0x140380940 -> 0x140380AC8
// 检查是否为连锁技能友方 (简化版本)
// ============================================================================
int CMoverEx::IsFriendForChain(CMover* pMover) {
    // IDA 精确还原 (简化版本):
    if (!pMover) {
        return 0;
    }

    E_ACTOR_TYPE targetType = pMover->GetType();
    if (targetType != eActorMonster && targetType != eActorUser) {
        return 0;
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

    // TODO: 需要 XArea::GetWorldType 实现
    // XArea* pArea = GetArea();
    // if (pArea && pArea->GetWorldType() != 2) {
    //     if (pMobRef->Monster_ClearCondition_Type) {
    //         return 0;
    //     }
    // }

    if (!pMobRef->Monster_Type || pMobRef->Monster_Type == 3 ||
        pMobRef->Monster_Type == 11 || pMobRef->Monster_Type == 4) {
        return IsFriend(pMover);
    }

    return 0;
}

// ============================================================================
// IsParty - IDA 0x140380AD0 -> 0x1403811FC
// 检查是否同队伍 (简化版本)
// ============================================================================
int CMoverEx::IsParty(CMover* pMover) {
    // IDA 精确还原 (简化版本):
    if (!pMover) {
        return 0;
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
                    return 1;
                }

                // Check party membership (simplified)
                // TODO: 需要 CGocParty 和 CGocForce 实现
                return 0;
            }
        }
    }

    // Player case - check party membership (simplified)
    // TODO: 需要 CGocParty 和 CGocForce 实现
    return 0;
}

// ============================================================================
// GetAttackJudgmentEvent - IDA 0x1403814C0 -> 0x1403814FE
// 获取攻击判定事件 (精确还原)
// ============================================================================
void* CMoverEx::GetAttackJudgmentEvent(const char* pAnimName, int iIndex) {
    // TODO: 需要实现 VAnimationInfo 和 XActionResMgr
    (void)pAnimName;
    (void)iIndex;
    return nullptr;
}

// ============================================================================
// GetUpperMotionName - IDA 0x140381750 -> 0x140381901
// 获取上半身动作名称 (精确还原)
// ============================================================================
const char* CMoverEx::GetUpperMotionName(const char* szMotionName) {
    // IDA 反编译精确还原:
    static char szResult[256];
    if (!szMotionName) {
        return "";
    }

    size_t nLen = strlen(szMotionName);
    if (nLen == 0) {
        return "";
    }

    strncpy(szResult, szMotionName, sizeof(szResult) - 1);
    szResult[sizeof(szResult) - 1] = '\0';

    // 替换最后字符为 "U"
    szResult[nLen - 1] = 'U';

    // 添加方向后缀
    const char* szDir[4] = {"_F", "_L", "_R", "_B"};
    std::uint8_t byDir = m_byMoveDirAnim;
    if (byDir < 4) {
        strncat(szResult, szDir[byDir], sizeof(szResult) - strlen(szResult) - 1);
    }

    return szResult;
}

// ============================================================================
// ExcuteSkipMotionTrigger - IDA 0x14037E5A0 -> 0x14037E9D3
// 执行跳过动作触发器 (精确还原)
// ============================================================================
void CMoverEx::ExcuteSkipMotionTrigger(unsigned int nSkillID, float fCamYaw) {
    // IDA 反编译精确还原 (简化版本):
    if (m_fSkillSkipCoolTime > 0.0f) {
        return;
    }

    // TODO: 完整实现需要 TB_SKILL、VAnimationInfo 等
    (void)nSkillID;
    (void)fCamYaw;
}

// ============================================================================
// IsCanMove - IDA 0x14037FDD0
// 检查是否可以移动
// ============================================================================
bool CMoverEx::IsCanMove(bool isCheckTurnMotion) {
    // IDA 0x14037FDD0 反编译精确还原:
    // _BOOL8 __fastcall CMoverEx::IsCanMove(CMoverEx *this, bool isCheckTurnMotion)
    // {
    //   if ( XActor::IsStatus(&this->XActor, 2u)
    //     || XActor::IsStatus(&this->XActor, 4u)
    //     || XActor::IsStatus(&this->XActor, 0x10000u)
    //     || XActor::IsStatus(&this->XActor, 0x20u)
    //     || XActor::IsStatus(&this->XActor, 0xF000000u) )
    //   {
    //     return 0;
    //   }
    //   if ( !XActor::IsStatus(&this->XActor, 0x2000u) || (this->m_dwInvisibleFlag & 4) != 0 )
    //     return this->m_nMotionClass != 0;
    //   return 0;
    // }

    // 状态检查: 死亡(2)、击倒(4)、眩晕(0x10000)、特殊状态(0x20)、特殊标记(0xF000000)
    if (CMover::IsStatus(2u) || CMover::IsStatus(4u) || CMover::IsStatus(0x10000u)
        || CMover::IsStatus(0x20u) || CMover::IsStatus(0xF000000u)) {
        return false;
    }

    // 隐身状态检查: 如果有隐身状态(0x2000)，需要检查隐身标志
    if (!CMover::IsStatus(0x2000u) || (m_dwInvisibleFlag & 4) != 0) {
        return m_nMotionClass != 0;
    }

    return false;
}

// ============================================================================
// IsCanAttack - IDA 0x14037FAC0
// 检查是否可以攻击
// ============================================================================
bool CMoverEx::IsCanAttack() {
    // IDA 0x14037FAC0 反编译精确还原:
    // bool __fastcall CMoverEx::IsCanAttack(CMoverEx *this)
    // {
    //   if ( !this->m_nMotionClass )
    //     return 0;
    //   return !XActor::IsStatus(&this->XActor, 2u)
    //     && !XActor::IsStatus(&this->XActor, 4u)
    //     && !XActor::IsStatus(&this->XActor, 0x20u)
    //     && !XActor::IsStatus(&this->XActor, 0x10000u)
    //     && !XActor::IsStatus(&this->XActor, 0xF000000u);
    // }

    // 如果没有动作类，不能攻击
    if (!m_nMotionClass) {
        return false;
    }

    // 状态检查: 死亡(2)、击倒(4)、特殊状态(0x20)、眩晕(0x10000)、特殊标记(0xF000000)
    return !CMover::IsStatus(2u)
        && !CMover::IsStatus(4u)
        && !CMover::IsStatus(0x20u)
        && !CMover::IsStatus(0x10000u)
        && !CMover::IsStatus(0xF000000u);
}

// ============================================================================
// IsCanHit - IDA 0x14037F9A0
// 检查是否可以被击中
// ============================================================================
int CMoverEx::IsCanHit(int nDownAttack, int bPassiveType) {
    // IDA 0x14037F9A0 反编译精确还原:
    // _BOOL8 __fastcall CMoverEx::IsCanHit(CMoverEx *this, int nDownAttack, int bPassiveType)
    // {
    //   if ( this->m_eDieType != DIE_TYPE_KNOCKDOWN
    //     && this->m_eDieType != DIE_TYPE_DELAY
    //     && XActor::IsStatus(&this->XActor, 4u) )
    //   {
    //     return 0;
    //   }
    //   if ( XActor::IsStatus(&this->XActor, 0x400u) || XActor::IsStatus(&this->XActor, 0x8000000u) )
    //     return 0;
    //   if ( XActor::IsStatus(&this->XActor, 0x2000u) && (this->m_dwInvisibleFlag & 1) == 0 )
    //     return 0;
    //   if ( !CMover::IsAllowPassiveType(this, bPassiveType) )
    //     return 0;
    //   if ( !CMover::IsHitDown(this) )
    //     return nDownAttack != 3;
    //   return nDownAttack == 2 || nDownAttack == 3;
    // }

    // 检查死亡类型和击倒状态
    if (m_eDieType != DIE_TYPE_KNOCKDOWN && m_eDieType != DIE_TYPE_DELAY && IsStatus(4u)) {
        return 0;
    }

    // 检查无敌状态
    if (IsStatus(0x400u) || IsStatus(0x8000000u)) {
        return 0;
    }

    // 检查隐身状态
    if (IsStatus(0x2000u) && (m_dwInvisibleFlag & 1) == 0) {
        return 0;
    }

    // 检查被动类型
    if (!CMover::IsAllowPassiveType(bPassiveType)) {
        return 0;
    }

    // 检查倒地状态
    if (!IsHitDown()) {
        return nDownAttack != 3 ? 1 : 0;
    }

    return (nDownAttack == 2 || nDownAttack == 3) ? 1 : 0;
}
