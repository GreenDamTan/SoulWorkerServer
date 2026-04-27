#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

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
    , m_fDefWalkSpeed(0.0f)
    , m_fDefRunSpeed(0.0f)
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
    , m_fFlyGravity(0.0f)
    , m_fFlyMaxHeight(0.0f)
    , m_bJumpAnim(0)
    , m_byAniProcessLinkType(0)
    , m_byMoveDirAnim(0)
    , m_pCurSkillTableRef(nullptr)
    , m_bAttackKeyPress(0)
    , m_bySkillAnimStep(0)
    , m_bySkillAnimCount(0)
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
    , m_nPlayPhaseMotion(0)
    , m_nBuffMotion(0)
    , m_byStandType(0)
    , m_byBattleModeAnim(0)
    , m_fIdleCheckTime(0.0f)
    , m_nIdleMotionChance(0)
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
    , m_iLastWarpPoint(0)
    , m_bSystemActor(0)
    , m_bChangedStat(0)
    , m_fSkillTotalChargeTime(0.0f)
    , m_bExistSuboCombo(false)
    , m_fSuboComboWaitTime(0.0f)
    , m_fSuboComboCheckTime(0.0f)
    , m_iSuboComboMaxCount(0)
    , m_iSuboComboCheckCount(0)
    , m_iCurLeftSuboDescCount(0)
    , m_iCurRightSuboDescCount(0)
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
    , m_bDisableDirectionToTargetSkill(0)
    , m_bFlyDie(false)
    , m_bCounterSuccessFrame(false)
    , m_bEnableCounuter(false)
    , m_fMinCounterRange(0.0f)
    , m_fMaxCounterRange(0.0f)
    , m_fCounterAngle(0.0f)
    , m_iCounterProbability(0)
    , m_fCounterDuration(0.0f)
    , m_bChargingStart(false)
    , m_fChargingInputAnimSpeed(0.0f)
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
    , m_fUpdateStatBatchTime(0.0f)
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
    , m_bUpdateRotation(false)
    , m_byPhaseMotionStep(0)
    , m_fPhaseStepMaxTime(0.0f)
    , m_fMultipleDamageOnce(0.0f)
    , m_bApplyMultipleDamageOnce(false)
    , m_fMultipleAbsorbSG(0.0f)
    , m_bySABreakMotionType(0)
    , m_fSABreakTime(0.0f)
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
}

CMoverEx::~CMoverEx() {
    // TODO: 汇编还原 - 析构函数
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
    // IDA 0x140189040
    // TODO: 需要从 STMyCharInfoEx 或其他数据源获取
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
    // IDA 0x140188F80 - 空实现
}

float CMoverEx::GetLookPitch() {
    // IDA 0x140189270
    return m_fLookPitch;
}

float CMoverEx::GetMovingYaw() {
    // IDA 0x140189290
    return m_fMovingYaw;
}

int CMoverEx::GetMaxHP() {
    // IDA 0x140189410
    // TODO: 需要计算
    return 0;
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
    // IDA 0x140189190
    m_iPvpCondition = nCondition;
}

int CMoverEx::GetPvpCondition() {
    // IDA 0x1401891C0
    return m_iPvpCondition;
}

void CMoverEx::SetActionCondition(int nCondition) {
    // IDA 0x1401891E0
    m_iActionCondition = nCondition;
}

int CMoverEx::GetActionCondition() {
    // IDA 0x140189210
    return m_iActionCondition;
}

void CMoverEx::ClearActionCondition(int nCondition) {
    // IDA 0x140188ED0 - 清除特定条件
}

bool CMoverEx::IsActionCondition(int nCondition) {
    // IDA 0x140188F00
    return m_iActionCondition == nCondition;
}

bool CMoverEx::IsPvpCondition(int nType) {
    // IDA 0x140188E90
    return m_iPvpCondition == nType;
}
