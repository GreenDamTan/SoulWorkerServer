#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"

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
    // 1. Reset()
    // 2. 逆序销毁成员容器
    // 3. CMover::~CMover()
    Reset();
    GreenDamTan_log(__FILE__, __FUNCTION__, "CMoverEx destructed");
}

void CMoverEx::RemoveAllOptionEffect() {
    // TODO: 汇编还原 - IDA 构造函数尾部调用
    m_vecOptionEffect.clear();
}

void CMoverEx::RemoveAllDefenseChangeInfo() {
    // TODO: 汇编还原 - IDA 构造函数尾部调用
    m_listDefenseChangeInfo.clear();
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
    // IDA 0x140188DC0 - 空实现
}

// ============================================================================
// ChangeInitMotion IDA 0x140390F60
// ============================================================================
void CMoverEx::ChangeInitMotion() {
    // IDA 0x140390F60:
    // unsigned int AnimIndex = XActionResMgr::GetAnimIndex(this, 0, 0, 0);
    // if (CMover::GetAnimStirng(this, AnimIndex)) {
    //   ChangeMotion_3(this, 0, 1, 0);
    // } else {
    //   unsigned int v2 = XActionResMgr::GetAnimIndex(this, 1, 0, 0);
    //   if (!CMover::GetAnimStirng(this, v2))
    //     this->m_bBattlePose = 1;
    //   ChangeMotion_3(this, 1, 1, 7);
    // }

    // TODO: 需要实现 XActionResMgr::GetAnimIndex 和 ChangeMotion_3
    // 简化实现：设置默认动画状态
    unsigned int AnimIndex = 0;  // XActionResMgr::GetAnimIndex(this, 0, 0, 0)
    if (GetAnimStirng(AnimIndex)) {
        // ChangeMotion_3(0, 1, 0);
    } else {
        AnimIndex = 0;  // XActionResMgr::GetAnimIndex(this, 1, 0, 0)
        if (!GetAnimStirng(AnimIndex)) {
            m_bBattlePose = true;
        }
        // ChangeMotion_3(1, 1, 7);
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
