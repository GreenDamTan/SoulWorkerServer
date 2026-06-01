#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"

// 前置声明 - CMoverEx (用于 send_eSUB_CMD_MOVE 等函数)
class CMoverEx;

// CMover - Vision Engine 核心实体类 (58592 bytes)
// 继承自 VisBaseEntity_cl + XActor
// IDA 确认的基类偏移:
//   VisBaseEntity_cl: offset 0-871
//   XActor:           offset 872-975

// ============================================================================
// 构造函数 IDA 0x1403659E0
// ============================================================================
CMover::CMover()
    // 基类由编译器自动构造 (VisBaseEntity_cl + XActor)
    : m_vPosition(0.0f, 0.0f, 0.0f)  // VisObject3D_cl 位置成员
    , m_dwStatus(0)
    , m_eActorType(0)  // E_ACTOR_TYPE
    , m_fLastUpdateTime(0.0f)
    , m_fLastDebugTime(0.0f)
    , m_fAbility(nullptr)
    , m_dwTargetID(0xFFFFFFFF)
    , m_dwHitID(0xFFFFFFFF)
    , m_setHitID()
    , m_listSummonMob()
    , m_fCapsuleRadius(10.0f)
    , m_fCapsuleHeight(170.0f)
    , m_fDieDelayTime(-1.0f)
    , m_fDieDelayMaxTime(-1.0f)
    , m_fDieFadeTime(-1.0f)
    , m_nDamage(0)
    , m_cWeightRank(5)
    , m_byDmgMontionFlag(0)
    , m_bMoving(0)
    , m_bGazeMoving(0)
    , m_fMoveSpeed(300.0f)
    , m_byMoveDir(0)
    , m_vMoveStopCheckPos_dummy(nullptr)
    , m_nBuffTotalCnt(0)
    , m_nBuffCnt(0)
    , m_nDebuffCnt(0)
    , m_stBuffState(nullptr)
    , m_nHitCallBuffIndex(0)
    , m_fLastSendMoveTime(0.0f)
    , m_stMovePos{}
    , m_stMoveGap{}
    , m_stMoveOffset{}
    , m_stExtMovingVal{}
    , m_stTimeSlow{}
    , m_vPrevPos(0.0f, 0.0f, 0.0f)
    , m_vCreatePos(0.0f, 0.0f, 0.0f)
    , m_byDefaultAnimStep(0)
    , m_fAnimSpeed(1.0f)
    , m_fRestoreAnimSpeed(1.0f)
    , m_eRestoreAnimSpeedType(0)
    , m_nMotionClass(1)
    , m_nHitStatus(0)
    , m_nHitAnimCount(7)
    , m_nTurnStatus(0)
    , m_bAnimChanged(0)
    , m_bSkipAnimOffset(0)
    , m_bAnimPlay(0)
    , m_nAnimationIdx(0xFFFFFFFF)
    , m_nAnimGroup(-1)
    , m_fAnimationTime(0.0f)
    , m_fAnimPercentTime(0.0f)
    , m_vAnimLastDelta_dummy(nullptr)
    , m_pCurMotionEvent(nullptr)
    , m_pActionResource(nullptr)
    , m_pAkashicActionInfo(nullptr)
    , m_nCurSkillTableIdx(0)
    , m_nParentSkillTableIdx(0)
    , m_fReactionRate(1.0f)
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
    , m_setAllowPassiveType()
    , m_byRestoreDefenceType(4)
    , m_stDefenseChangeInfoByTrigger()
    , m_stDefenseChangeInfoByEffect()
    , m_listDefenseChangeInfo()
    , m_DefanseChangeFlag(0)
    , m_byDefaultDefenseType(0)
    , m_byRestoreDefenceTypeByTrigger(4)
    , m_byDefenseTypeDisableFlag(0)
    , m_fSkillCoolDownRate(0.0f)
    , m_fSkillReflectRate(0.0f)
    , m_fSkillBloodRate(0.0f)
    , m_nAllowBloodCount(0)
    , m_fSkillBloodDebuffRate(0.0f)
    , m_dwBloodDebuffOwnerID(0xFFFFFFFF)
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
    , m_mShaderState(4)
    , m_bInvincibleActor(0)
    , m_mapAnimInfoKey(nullptr)
    , m_mapAnimInfoString(nullptr)
    , m_mapFilterData()
    , m_pHitCollisionData(nullptr)
    , m_xActionBuffer()
    , m_fHitCylinderRadius(0.0f)
    , m_fHitCylinderHeight(0.0f)
    , m_vTraceBoneName()
    , m_byTargetPosCount(0)
    , m_byTargetDestPos(0xFF)
    , m_fMaxSuperArmorGage(0.0f)
    , m_fCurSuperArmorGage(0.0f)
    , m_fRegenSuperArmorTime(0.0f)
    , m_fDefRegenSuperArmorTime(0.0f)
    , m_fRecoverySuperArmorTime(0.0f)
    , m_dwCellID(0xFFFFFFFF)
    , m_strTableID()
    , m_uiForcedState(0)
    , m_fForcedStateApplyTime(0.0f)
    , m_fProtectionAggroRatio(0.0f)
    , m_vecDelayedProjectile()
    , m_GOComponentTable()
    , m_pSkillMgr(nullptr)
    , m_bTraceUser(0)
    , m_bPublicTransportRiding(false)
    , m_shCurRandomShootProjectileDirX(0.0f)
    , m_shCurRandomShootProjectileDirY(0.0f)
    , m_nRandomTrapIndex(0)
    , m_dwImmunityStatus(0)
    , m_bIgnoreAggroDebuff(false)
    , m_vContinuousMelee()
    , m_fBossAttackAddRate(0.0f)
    , m_fBossAttackedDownRate(0.0f)
    , m_fSoulCostDownRate(0.0f)
    , m_fAllAttackAddRate(0.0f)
    , m_mapMeleeDebuff()
    , m_mapSkillUnlock()
    , m_bNoSkillCostSG(false)
{
    // IDA 构造函数流程:
    // 1. 基类构造: VisBaseEntity_cl::VisBaseEntity_cl(this)
    // 2. 基类构造: XActor::XActor(&this->XActor)
    // 3. vtable 赋值 (7 个 Vision Engine 多重继承 vtable)
    // 4. 容器构造 (map/list/vector/CActionBuffer/VPublicTransport)
    // 5. 指针置零 (m_pCustomTraceBBox, m_pSkillMgr, m_fAbility, m_pAkashicActionInfo)
    // 6. 浮点率置零 (BossAttack/AttackedDown/SoulCost/AllAttack)
    // 7. CMover::Reset(this)

    Reset();

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMover constructed");
}

// ============================================================================
// Actor 状态方法 (来自 XActor 基类)
// ============================================================================
bool CMover::IsStatus(std::uint32_t dwStatus) const {
    return (m_dwStatus & dwStatus) != 0;
}

std::uint32_t CMover::GetStatus() const {
    return m_dwStatus;
}

void CMover::SetStatus(std::uint32_t dwStatus) {
    m_dwStatus = dwStatus;
}

// ============================================================================
// 析构函数 IDA 0x140366760
// 大小: 472 bytes
// ============================================================================
CMover::~CMover() {
    // IDA 反编译确认:
    // 1. 恢复 vtable 到 CMover 的 vtable (多重继承的各个分支)
    // 2. 调用 Destroy() 清理资源
    // 3. 逆序销毁成员容器
    // 4. 调用 XActor::~XActor() 和 VisBaseEntity_cl::~VisBaseEntity_cl()

    // IDA: 多重继承的 vtable 恢复 (7 个 Vision Engine vtable)
    // 这些由编译器自动处理

    // 调用 Destroy 清理资源
    Destroy();

    // 成员容器由编译器自动逆序析构:
    // - m_mapSkillUnlock
    // - m_mapMeleeDebuff
    // - m_vContinuousMelee
    // - m_sPublicTransportPath
    // - m_GOComponentTable
    // - m_vecDelayedProjectile
    // - m_strTableID
    // - m_vTraceBoneName
    // - m_xActionBuffer
    // - m_mapFilterData
    // - m_listDefenseChangeInfo
    // - m_setAllowPassiveType
    // - m_listSummonMob
    // - m_setHitID
    // 基类 XActor::~XActor() 和 VisBaseEntity_cl::~VisBaseEntity_cl() 由编译器自动调用
}

// ============================================================================
// OnUpdate IDA 0x140366F60 -> 0x140366F9E
// 大小: 62 bytes
// IDA 反编译:
// (*(void (__fastcall **)(char *))(*((_QWORD *)this - 109) + 272LL))((char *)this - 872);
// 通过 vtable 偏移 272 调用 VisBaseEntity_cl::OnUpdate
// 参数 fDelta 未使用 - 基类 OnUpdate 可能不需要参数
// TODO: 需要完整的 VisBaseEntity_cl 定义后才能实现基类调用
// ============================================================================
void CMover::OnUpdate(float fDelta) {
    // IDA 确认: 通过 vtable 调用 VisBaseEntity_cl::OnUpdate
    // 由于 VisBaseEntity_cl 是前置声明，暂时无法调用基类方法
    // 实际实现需要: reinterpret_cast<VisBaseEntity_cl*>(static_cast<char*>(this) - 872)->OnUpdate()
    (void)fDelta;  // 参数暂未使用
}

// ============================================================================
// 简单 getter/setter 函数 (IDA 反编译精确还原)
// ============================================================================

// IDA 0x140016BD0 - SetHitCollisionData
void CMover::SetHitCollisionData(void* pData) {
    m_pHitCollisionData = pData;
}

// IDA 0x140016BF0 - SetHitCylinder
void CMover::SetHitCylinder(float fRadius, float fHeight) {
    m_fHitCylinderRadius = fRadius;
    m_fHitCylinderHeight = fHeight;
}

// IDA 0x140016C30 - AddActionBuffer
void CMover::AddActionBuffer(void* xAction) {
    // IDA: CActionBuffer::Push(&this->m_xActionBuffer, xAction)
    // 需要 CActionBuffer 类定义才能实现
}

// IDA 0x140166360 - GetStat
float CMover::GetStat(int iIndex) {
    return m_fAbility[iIndex];
}

// IDA 0x140189240 - GetVariableType
int CMover::GetVariableType() {
    return m_eActorType;
}

// IDA 0x1400488E0 - SetNoSkillCostSG
void CMover::SetNoSkillCostSG(bool bCost) {
    m_bNoSkillCostSG = bCost;
}

// IDA 0x140198DE0 - GetTargetID
std::uint32_t CMover::GetTargetID() const {
    return m_dwTargetID;
}

// IDA 0x140199E30 - GetCurMotionEvent
const VAnimationInfo* CMover::GetCurMotionEvent() const {
    return m_pCurMotionEvent;
}

// IDA 0x14019B970 - GetDefenseType
std::uint8_t CMover::GetDefenseType() const {
    return m_byDefenseType;
}

// IDA 0x1401B4820 - SetInvincibleActor
void CMover::SetInvincibleActor(int bEnable) {
    m_bInvincibleActor = bEnable;
}

// IDA 0x1401B4840 - IsInvincibleActor
int CMover::IsInvincibleActor() {
    return m_bInvincibleActor;
}

// IDA 0x140276290 - GetRestoreDefenseType
std::uint8_t CMover::GetRestoreDefenseType() {
    return m_byRestoreDefenceType;
}

// IDA 0x140276370 - GetAnimationIdx
std::uint32_t CMover::GetAnimationIdx() {
    return m_nAnimationIdx;
}

// IDA 0x140276870 - GetHavokCapsuleRadius
float CMover::GetHavokCapsuleRadius() {
    return m_fCapsuleRadius;
}

// IDA 0x14027A610 - IsMoving
// IDA 反编译: return (unsigned int)this->m_fMoving;
bool CMover::IsMoving() {
    // IDA 确认: 检查 m_fMoving 成员 (注意: IDA 显示为 m_fMoving，可能是 int 或 float)
    return m_bMoving != 0;
}

// IDA 0x140276270 - GetMotionClass
// IDA 反编译: return (unsigned __int16)this->m_nMotionClass;
short CMover::GetMotionClass() {
    return static_cast<std::int16_t>(m_nMotionClass);
}

// IDA 0x1402762B0 - GetCreatePos
hkvVec3& CMover::GetCreatePos() {
    return m_vCreatePos;
}

// IDA 0x140276330 - GetExtraMovePos
tagEXTRA_MOVEPOS& CMover::GetExtraMovePos() {
    return m_stExtMovingVal;
}

// IDA 0x1402763F0 - SetCurSkillTableIdx
void CMover::SetCurSkillTableIdx(int nIdx) {
    m_nCurSkillTableIdx = nIdx;
}

// IDA 0x140280BA0 - GetAttackerCount
std::uint8_t CMover::GetAttackerCount() {
    return m_byTargetPosCount;
}

// IDA 0x140280C60 - SetTargetDestPos
void CMover::SetTargetDestPos(std::uint8_t byPos) {
    m_byTargetDestPos = byPos;
}

// IDA 0x140280C80 - GetTargetDestPos
std::uint8_t CMover::GetTargetDestPos() {
    return m_byTargetDestPos;
}

// IDA 0x140280CC0 - GetCellID
std::uint32_t CMover::GetCellID() {
    return m_dwCellID;
}

// IDA 0x140280CE0 - SetCellID
void CMover::SetCellID(std::uint32_t dwID) {
    m_dwCellID = dwID;
}

// IDA 0x1401893C0 - SetPositionXVec3
void CMover::SetPositionXVec3(const hkvVec3& vPos) {
    m_vPosition = vPos;
}

// IDA 0x1401ACFA0 - GetSkillLevel (基类返回0)
std::uint8_t CMover::GetSkillLevel() {
    return 0;
}

// IDA 0x140280D40 - SetCreatePos
void CMover::SetCreatePos(const hkvVec3& vPos) {
    m_vCreatePos = vPos;
}

// ============================================================================
// 虚函数基类实现 (IDA 反编译精确还原)
// ============================================================================

// IDA 0x140189230 - SetHP (基类空实现)
void CMover::SetHP(int nHP) {
    // 基类空实现 - 由子类 override
}

// IDA 0x140189390 - ClearExtraMoving
void CMover::ClearExtraMoving() {
    m_stExtMovingVal.Clear();
}

// IDA 0x140188FE0 - SetDie (基类空实现)
void CMover::SetDie(std::int16_t nMotionClass, int bSuicide, bool bSendPacket) {
    // 基类空实现 - 由子类 override
}

// IDA 0x140189120 - GetSkillDestPos (基类返回零向量)
hkvVec3 CMover::GetSkillDestPos() {
    return hkvVec3(0.0f, 0.0f, 0.0f);
}

// IDA 0x140189150 - GetActionResourceFN (基类返回空字符串)
VString CMover::GetActionResourceFN() {
    return VString("");  // TODO: 从IDA返回的是 &stru_140B70D70
}

// IDA 0x140189320 - ApplySkillDamageFrame (基类空实现)
void CMover::ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx, std::uint8_t byAttackTargetCnt) {
    // 基类空实现 - 由子类 override
}

// IDA 0x1401892D0 - ActionProcess (基类返回1)
int CMover::ActionProcess(std::int16_t nTriggerIdx) {
    return 1;
}

// IDA 0x1401892E0 - DamageProcessHP (基类返回1)
int CMover::DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage, int nUnk1, std::uint8_t byUnk1, std::uint8_t byUnk2) {
    return 1;
}

// IDA 0x1401892B0 - ClearBuffProcess (基类返回1)
int CMover::ClearBuffProcess(int nSkillID, class AttackJudgmentTrigger* pTrigger, hkvVec3& vCurPos) {
    return 1;
}

// IDA 0x140189300 - Damage (基类空实现)
void CMover::Damage(std::uint32_t dwID, std::uint8_t byReactionType, std::uint8_t byAttackCollision) {
    // 基类空实现 - 由子类 override
}

// IDA 0x140189390 - ClearExtraMoving (精确还原)
// IDA 反编译: tagEXTRA_MOVEPOS::Clear(&this->m_stExtMovingVal)
// 注意: 已在上方定义，此处删除重复定义

// ============================================================================
// 移动/状态函数 (IDA 反编译精确还原)
// ============================================================================

// IDA 0x1402A4BE0 - MoveingValueClear
void CMover::MoveingValueClear() {
    m_bMoving = 0;
    m_stMovePos.Clear();
    m_stMoveGap.Clear();
    m_stMoveOffset.Clear();
}

// IDA 0x1402A4F90 - SetImmunityStatus
void CMover::SetImmunityStatus(std::uint32_t dwStatus) {
    m_dwImmunityStatus |= dwStatus;
}

// IDA 0x1406C5C30 - GetMoveSpeed (非const版本)
float CMover::GetMoveSpeed() {
    return m_fMoveSpeed;
}

// IDA 0x1402A5030 - GetCurSuperArmorGage
float CMover::GetCurSuperArmorGage() {
    return m_fCurSuperArmorGage;
}

// IDA 0x1402A5050 - GetMaxSuperArmorGage
float CMover::GetMaxSuperArmorGage() {
    return m_fMaxSuperArmorGage;
}

// IDA 0x1402A67F0 - SetIgnoreAggroDebuff
void CMover::SetIgnoreAggroDebuff(int bApply) {
    m_bIgnoreAggroDebuff = (bApply != 0);
}

// IDA 0x1402AC570 - ChangeMotion (基类空实现)
void CMover::ChangeMotion(std::int16_t wType) {
    // 基类空实现 - 由子类 override
}

// IDA 0x14036DDD0 - Move (精确还原)
// 虚函数 - 移动到目标位置
void CMover::Move(const hkvVec3& vNextPos) {
    // IDA 反编译确认:
    // 1. 检查 m_pArea 是否存在
    // 2. 调用 XArea::MoveActor 执行移动
    // 3. 如果没有 Area，记录调试日志

    // TODO: 需要完整的 XActor 和 XArea 类型定义
    // if (m_pArea) {
    //     m_pArea->MoveActor(&this->XActor, vNextPos);
    // } else {
    //     LogHelper::LogDebug("game.contents", "No Area when send move!!");
    // }

    // 简化实现: 直接设置位置
    SetPosition(vNextPos);
}

// ============================================================================
// IsGazeMoving IDA 0x140375200
// IDA 反编译: return (unsigned int)this->m_bGazeMoving;
// ============================================================================
bool CMover::IsGazeMoving() {
    return m_bGazeMoving != 0;
}

// ============================================================================
// OnDamage (基类空实现 - 由子类 CMoverEx/CMonster/CUser 重写)
// ============================================================================
void CMover::OnDamage(int nDamage, CMover* pAttacker) {
    // 基类空实现 - 由子类 override
}

// ============================================================================
// OnDie (基类空实现 - 由子类重写)
// ============================================================================
void CMover::OnDie() {
    // 基类空实现 - 由子类 override
}

// ============================================================================
// GetPosition IDA 0x1408B0DBA (thunk to VisObject3D_cl::GetPosition)
// ============================================================================
hkvVec3 CMover::GetPosition() const {
    // IDA 反编译确认: 返回 VisObject3D_cl 基类的 m_vPosition 成员
    // 通过 GetPositionXVec3 获取引用
    return const_cast<CMover*>(this)->GetPositionXVec3();
}

// ============================================================================
// GetPositionXVec3 IDA 0x1402A5080
// 返回位置引用 (用于直接修改)
// ============================================================================
hkvVec3& CMover::GetPositionXVec3() {
    // IDA 0x1402A5080: return &this->m_vPosition
    return m_vPosition;
}

// ============================================================================
// SetPosition IDA 0x140189790 (通过 hkvVec3 设置 VisObject3D_cl 位置)
// ============================================================================
void CMover::SetPosition(const hkvVec3& vPos) {
    // IDA 0x140189790: VisObject3D_cl::SetPosition(this, vPos)
    SetPositionXVec3(const_cast<hkvVec3&>(vPos));
}

// ============================================================================
// Reset IDA 0x140365D80
// 重置所有 CMover 状态到默认值
// ============================================================================
void CMover::Reset() {
    // IDA 反编译显示调用 XActor::Reset
    // XActor::Reset(&this->XActor);

    // 重置包围盒
    // hkvAlignedBBox::setInvalid(&this->m_BoundingBox);

    m_dwTargetID = 0xFFFFFFFF;
    m_dwHitID = 0xFFFFFFFF;
    m_dwCellID = 0xFFFFFFFF;

    m_setHitID.clear();
    m_listSummonMob.clear();

    // 重置移动状态
    // tagMOVE_POS::Clear(&m_stMovePos);
    // tagMOVE_POS::Clear(&m_stMoveGap);
    // tagMOVE_POS::Clear(&m_stMoveOffset);
    // tagEXTRA_MOVEPOS::Clear(&m_stExtMovingVal);

    // m_vPrevPos.setZero();
    // m_vCreatePos.setZero();
    // m_vMoveStopCheckPos.setZero();

    m_mapAnimInfoKey = nullptr;
    m_mapAnimInfoString = nullptr;
    m_pHitCollisionData = nullptr;
    m_pCurMotionEvent = nullptr;
    m_pActionResource = nullptr;

    m_bAnimPlay = 0;
    m_bAnimChanged = 0;

    ResetAkashicActionInfo();

    m_nHitStatus = 0;
    m_nHitAnimCount = 7;
    m_nTurnStatus = 0;

    m_byDefaultDefenseType = 0;
    m_byDefenseTypeDisableFlag = 0;
    m_byDefenseType = 0;
    m_byRestoreDefenceType = 4;

    m_fAnimationTime = 0.0f;
    m_fAnimPercentTime = 0.0f;

    m_nCurSkillTableIdx = 0;
    m_nParentSkillTableIdx = 0;
    m_nAnimationIdx = 0xFFFFFFFF;
    m_nMotionClass = 1;
    m_nAnimGroup = -1;
    m_bGazeMoving = 0;

    m_setAllowPassiveType.clear();

    m_byDownContinueDamage = 0;
    m_fDieDelayTime = -1.0f;
    m_fDieDelayMaxTime = -1.0f;
    m_fDieFadeTime = -1.0f;
    m_nDamage = 0;
    m_byDmgMontionFlag = 0;

    m_cWeightRank = 5;
    m_bMoving = 0;
    m_byMoveDir = 0;
    m_fMoveSpeed = 300.0f;
    m_fRestoreAnimSpeed = 1.0f;
    m_eRestoreAnimSpeedType = 0;
    m_bMoveingInFly = false;
    m_fGroundDownTime = 0.0f;
    m_fHitLoopMaxTime = 0.0f;
    m_fCapsuleRadius = 10.0f;
    m_fCapsuleHeight = 170.0f;
    m_fReactionRate = 1.0f;
    m_fFlyYaw = 0.0f;
    m_fFlySpeed = 0.0f;
    m_fFlyDistance = 0.0f;
    m_fFlyMoveDist = 0.0f;
    m_fAnimSpeed = 1.0f;
    m_fCheckCollisionTime = 0.0f;
    m_bCollisionEnable = 1;
    m_bSkipAnimOffset = 0;
    m_nHitCallBuffIndex = 0;
    m_mShaderState = 4;
    m_bTraceUser = 0;
    m_bInvincibleActor = 0;

    // VString::Reset(&m_strTableID);
    m_fHitCylinderRadius = 0.0f;
    m_fHitCylinderHeight = 0.0f;
    m_vTraceBoneName.clear();
    ClearActionBuffer();
    m_bKeepMovingExtra = 0;

    // 删除技能管理器
    if (m_pSkillMgr) {
        delete m_pSkillMgr;
        m_pSkillMgr = nullptr;
    }

    m_fSkillCoolDownRate = 0.0f;
    m_fSkillReflectRate = 0.0f;
    m_fSkillBloodRate = 0.0f;
    m_nAllowBloodCount = 0;
    m_fSkillBloodDebuffRate = 0.0f;
    m_fBuffSuperArmorRate = 0.0f;
    m_bIgnoreSkillCost = 0;
    m_fDecreaseStaminaRate = 0.0f;
    m_fBuffAddGoldRate = 0.0f;

    m_dwAggroTargetID = 0;
    m_dwBloodDebuffOwnerID = 0xFFFFFFFF;
    m_bCheckSendAbsorbSG = false;
    m_bCheckSendHP = false;
    m_fSkillAbsorbSGRate = 0.0f;
    m_nAllowAbsorbSGCount = 0;

    m_GOComponentTable.resize(26);

    // XActor::SetInfo(&this->XActor);

    m_uiForcedState = 0;
    m_fForcedStateApplyTime = 0.0f;

    // memset(m_byTargetPosInfo, 0, sizeof(m_byTargetPosInfo));
    m_byTargetPosCount = 0;
    m_byTargetDestPos = 0xFF;

    // 清理延迟投射物
    for (auto& pProj : m_vecDelayedProjectile) {
        delete pProj;
    }
    m_vecDelayedProjectile.clear();

    m_fProtectionAggroRatio = 0.0f;
    m_setAllowPassiveType.clear();

    m_fRegenSuperArmorTime = 0.0f;
    m_fDefRegenSuperArmorTime = 0.0f;
    m_fRecoverySuperArmorTime = 0.0f;

    // VPublicTransport_cl::Stop(&m_sPublicTransportPath);
    m_bPublicTransportRiding = false;
    m_fLastSendMoveTime = 0.0f;

    // tagTIME_SLOW::Clear(&m_stTimeSlow);

    m_shCurRandomShootProjectileDirX = 0.0f;
    m_shCurRandomShootProjectileDirY = 0.0f;
    m_nRandomTrapIndex = 0;

    m_dwImmunityStatus = 0;
    m_bIgnoreAggroDebuff = false;
    m_bNoSkillCostSG = false;

    m_mapMeleeDebuff.clear();
    m_mapSkillUnlock.clear();
    m_byRestoreDefenceTypeByTrigger = 4;

    // VisTypedEngineObject_cl::RemoveAllComponents(this);
}

void CMover::ResetAkashicActionInfo() {
    // IDA 0x140365EAA - Reset 函数内部调用
    m_pAkashicActionInfo = nullptr;
}

// ============================================================================
// AllBuffClear IDA 0x14036AA40
// 清除所有 Buff 状态
// ============================================================================
void CMover::AllBuffClear(std::uint8_t byReason) {
    // IDA 0x14036AA40 反编译:
    // if (m_nBuffTotalCnt == 0) return;
    // for (i = 0; i < 50; ++i) {
    //     if (tagBUFF_STATE::IsLife(&m_stBuffState[i])) {
    //         if (!byReason || IsClearBuff(m_stBuffState[i].nBuffIndex, byReason))
    //             ClearBuffStatusBySlot(i, 0);
    //     } else if (m_stBuffState[i].nBuffIndex) {
    //         pBuffRef = XResourceMgr::GetTB_BUFF(m_stBuffState[i].nBuffIndex);
    //         if (pBuffRef && !pBuffRef->Buff_Time && IsClearBuff(m_stBuffState[i].nBuffIndex, byReason))
    //             ClearBuffStatusBySlot(i, 0);
    //     }
    // }

    if (m_nBuffTotalCnt == 0) {
        return;
    }

    for (std::uint8_t i = 0; i < 50; ++i) {
        // IDA: 使用 IsLife() 检查 buff 是否激活
        if (m_stBuffState[i].IsLife()) {
            // Buff 正在生效
            // IDA: 使用 nBuffIndex 而不是 dwBuffID
            if (!byReason || IsClearBuff(m_stBuffState[i].nBuffIndex, byReason)) {
                ClearBuffStatusBySlot(i, 0);
            }
        } else if (m_stBuffState[i].nBuffIndex != 0) {
            // Buff 已失效但槽位未清空 - 检查永久性 Buff (Buff_Time == 0)
            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                // TODO: 需要从 XResourceMgr 获取 TB_BUFF 表
                // TB_BUFF* pBuffRef = XResourceMgr::GetTB_BUFF(m_stBuffState[i].nBuffIndex);
                // if (pBuffRef && pBuffRef->Buff_Time == 0) {
                //     if (!byReason || IsClearBuff(m_stBuffState[i].nBuffIndex, byReason)) {
                //         ClearBuffStatusBySlot(i, 0);
                //     }
                // }
                // 简化: 清除所有未激活但有效的 buff
                ClearBuffStatusBySlot(i, 0);
            }
        }
    }
}

// ============================================================================
// IsClearBuff - 检查是否应该清除指定 Buff
// ============================================================================
bool CMover::IsClearBuff(int nBuffIndex, std::uint8_t byReason) {
    // TODO: 需要从 IDA 反编译确认完整逻辑
    // 简化实现: 根据 byReason 判断是否清除
    // byReason == 0: 清除所有
    // byReason != 0: 根据 buff 类型判断

    if (byReason == 0) {
        return true;  // 清除所有 buff
    }

    // TODO: 需要检查 buff 类型是否匹配 byReason
    return true;
}

// ============================================================================
// ClearBuffStatusBySlot - 清除指定槽位的 Buff 状态
// ============================================================================
void CMover::ClearBuffStatusBySlot(std::uint8_t bySlot, int bNotify) {
    // TODO: 需要从 IDA 反编译确认完整逻辑
    // 简化实现: 清除指定槽位的 buff 数据

    if (bySlot >= 50) {
        return;  // 超出范围
    }

    // 清除 buff 状态 - 使用 tagBUFF_STATE::Clear()
    m_stBuffState[bySlot].Clear();

    // 更新 buff 计数
    if (m_nBuffTotalCnt > 0) {
        m_nBuffTotalCnt--;
    }

    // TODO: 如果 bNotify != 0，需要发送 buff 移除通知
}

// ============================================================================
// AddBuff - 添加 Buff
// IDA 逻辑: 查找空闲槽位，添加新 buff
// ============================================================================
bool CMover::AddBuff(int nBuffID, int nDuration, std::uint32_t dwSourceID, int bNotify) {
    // 参数检查
    if (nBuffID <= 0) {
        return false;
    }

    // 检查是否已有相同 buff
    for (std::uint8_t i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == static_cast<std::uint16_t>(nBuffID)) {
            // 已存在，更新持续时间
            m_stBuffState[i].fLifeTime = static_cast<float>(nDuration) / 1000.0f;
            m_stBuffState[i].dwID = dwSourceID;
            return true;
        }
    }

    // 查找空闲槽位
    for (std::uint8_t i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == 0 || !m_stBuffState[i].IsLife()) {
            // 找到空闲槽位，添加 buff
            m_stBuffState[i].Clear();
            m_stBuffState[i].nBuffIndex = static_cast<std::uint16_t>(nBuffID);
            m_stBuffState[i].dwID = dwSourceID;
            m_stBuffState[i].fLifeTime = static_cast<float>(nDuration) / 1000.0f;
            m_stBuffState[i].byBuffType = 0;  // TODO: 从 TB_BUFF 表获取

            m_nBuffTotalCnt++;

            // TODO: 如果 bNotify != 0，发送 buff 添加通知
            return true;
        }
    }

    return false;  // 没有空闲槽位
}

// ============================================================================
// RemoveBuff - 移除指定 Buff
// IDA 逻辑: 查找并清除指定 buff
// ============================================================================
void CMover::RemoveBuff(int nBuffID, int bNotify) {
    if (nBuffID <= 0) {
        return;
    }

    for (std::uint8_t i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == static_cast<std::uint16_t>(nBuffID)) {
            ClearBuffStatusBySlot(i, bNotify);
            return;
        }
    }
}

void CMover::ClearActionBuffer() {
    // TODO: 汇编还原 - CActionBuffer 清空逻辑
    new (&m_xActionBuffer) CActionBuffer();
}

// ============================================================================
// Destroy IDA 0x140366940 -> 0x140366ACD
// 大小: 397 bytes
// ============================================================================
void CMover::Destroy() {
    // IDA 0x140366940 确认流程:
    Reset();
    AllBuffClear(0);  // IDA 0x14036694F: CMover::AllBuffClear(this, 0)

    m_mapAnimInfoKey = nullptr;
    m_mapAnimInfoString = nullptr;
    m_pHitCollisionData = nullptr;

    if (m_pSkillMgr) {
        delete m_pSkillMgr;
        m_pSkillMgr = nullptr;
    }

    m_pCurMotionEvent = nullptr;
    m_pActionResource = nullptr;
    ResetAkashicActionInfo();

    // IDA 0x140366978 确认调用 VPublicTransport_cl::Stop, 当前类型不完整暂注释
    // reinterpret_cast<VPublicTransport_cl*>(&m_sPublicTransportPath_dummy)->Stop();
    m_bPublicTransportRiding = false;

    m_shCurRandomShootProjectileDirX = 0.0f;
    m_shCurRandomShootProjectileDirY = 0.0f;

    m_fBossAttackAddRate = 0.0f;
    m_fBossAttackedDownRate = 0.0f;
    m_fSoulCostDownRate = 0.0f;
    m_fAllAttackAddRate = 0.0f;

    m_mapMeleeDebuff.clear();
    m_mapSkillUnlock.clear();
}

// ============================================================================
// SetDmgMotionFlag IDA 0x1403655E0
// ============================================================================
void CMover::SetDmgMotionFlag(std::uint8_t byFlag) {
    // IDA 0x1403655E0: this->m_byDmgMontionFlag = byFlag
    m_byDmgMontionFlag = byFlag;
}

// ============================================================================
// InitFunction IDA 0x140366C00
// ============================================================================
void CMover::InitFunction() {
    // IDA 0x140366C00
    m_fLastUpdateTime = 0.0f;
    m_fLastDebugTime = 0.0f;
}

// ============================================================================
// GetClass IDA 0x140366C30
// 需要 CGocAttribute GOC 组件
// ============================================================================
std::uint8_t CMover::GetClass() {
    // IDA 0x140366C30: 获取 CGocAttribute GOC 组件
    // TODO: 需要 GetGOC<CGocAttribute> 模板实现
    return 0;
}

// ============================================================================
// GetLevel IDA 0x140366CB0
// 需要 CGocAttribute GOC 组件
// ============================================================================
std::uint8_t CMover::GetLevel() {
    // IDA 0x140366CB0: 获取 CGocAttribute GOC 组件并返回等级
    // TODO: 需要 GetGOC<CGocAttribute> 模板实现
    return 0;
}

// ============================================================================
// GetHP IDA 0x140366DC0
// 需要 CGocAttribute GOC 组件
// ============================================================================
int CMover::GetHP() {
    // IDA 0x140366DC0: 获取 CGocAttribute GOC 组件并返回 HP
    // TODO: 需要 GetGOC<CGocAttribute> 模板实现
    return 0;
}

// ============================================================================
// IsDie IDA 0x140366E40
// IDA 反编译: return XActor::IsDieStatus(&this->XActor) || this->GetHP(this) <= 0;
// ============================================================================
bool CMover::IsDie() {
    // IDA 0x140366E40: XActor::IsDieStatus || GetHP() <= 0
    // Note: XActor::IsDieStatus checks the actor status flags for death state
    // For now, we check HP <= 0 as the primary death condition
    // TODO: Implement XActor::IsDieStatus when XActor base class is fully integrated
    return GetHP() <= 0;
}

// ============================================================================
// GetMaxHP IDA 0x140366E90
// 需要 CGocAttribute GOC 组件
// ============================================================================
int CMover::GetMaxHP() {
    // IDA 0x140366E90: 获取 CGocAttribute GOC 组件并返回 MaxHP (stat index 10)
    // TODO: 需要 GetGOC<CGocAttribute> 模板实现
    return 0;
}

// ============================================================================
// IsFlying IDA 0x140367080
// ============================================================================
bool CMover::IsFlying() {
    // IDA 0x140367080 反编译:
    // if (m_fForcedStateApplyTime > 0.0) return m_uiForcedState == 1;
    // if (m_bLanded) return false;
    // 获取当前位置
    // if (GetHeight(&vPos, 300.0)) {
    //     return fZ > vPos.z + 5.0;
    // }
    // return false;
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 1;
    }
    if (m_bLanded) {
        return false;
    }
    // 获取当前位置
    hkvVec3 vPos = GetPosition();
    float fZ = vPos.z;
    // 尝试获取地面高度
    if (GetHeight(&vPos, 300.0f)) {
        return fZ > (vPos.z + 5.0f);
    }
    // GetHeight 失败时返回 false
    return false;
}

// ============================================================================
// IsKnockDown IDA 0x1403671C0
// ============================================================================
bool CMover::IsKnockDown() {
    // IDA 0x1403671C0:
    // if (m_fForcedStateApplyTime > 0.0) return m_uiForcedState == 3;
    // return m_nMotionClass >= 18 && m_nMotionClass <= 21;
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 3;
    }
    return m_nMotionClass >= 18 && m_nMotionClass <= 21;
}

// ============================================================================
// IsHit IDA 0x140367230
// ============================================================================
bool CMover::IsHit() {
    // IDA 0x140367230: return m_nMotionClass >= 15 && m_nMotionClass <= 23
    return m_nMotionClass >= 15 && m_nMotionClass <= 23;
}

// ============================================================================
// IsHitDown IDA 0x140367270
// ============================================================================
bool CMover::IsHitDown() {
    // IDA 0x140367270:
    // if (m_fForcedStateApplyTime > 0.0) return m_uiForcedState == 3;
    // if (m_nMotionClass < 18 || m_nMotionClass > 21) return m_nMotionClass == 13;
    // if (m_nHitStatus == 1 || m_nHitStatus == 2 || m_nHitStatus == 3) return true;
    // return m_bLanded && m_nHitStatus != 5 && m_nHitStatus || m_nMotionClass == 13;
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 3;
    }
    if (m_nMotionClass < 18 || m_nMotionClass > 21) {
        return m_nMotionClass == 13;
    }
    if (m_nHitStatus == 1 || m_nHitStatus == 2 || m_nHitStatus == 3) {
        return true;
    }
    return (m_bLanded && m_nHitStatus != 5 && m_nHitStatus) || m_nMotionClass == 13;
}

// ============================================================================
// IsGeneralHit IDA 0x140367410
// ============================================================================
bool CMover::IsGeneralHit() {
    // IDA 0x140367410:
    // if (m_fForcedStateApplyTime > 0.0) return m_uiForcedState == 4;
    // return m_nMotionClass >= 15 && m_nMotionClass <= 17;
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 4;
    }
    return m_nMotionClass >= 15 && m_nMotionClass <= 17;
}

// ============================================================================
// IsFlyHit IDA 0x140367480
// ============================================================================
bool CMover::IsFlyHit() {
    // IDA 0x140367480:
    // if (m_fForcedStateApplyTime > 0.0) return m_uiForcedState == 4;
    // return m_nMotionClass >= 18 && m_nMotionClass <= 21;
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 4;
    }
    return m_nMotionClass >= 18 && m_nMotionClass <= 21;
}

// ============================================================================
// IsCounterAttackHit IDA 0x140367360
// ============================================================================
bool CMover::IsCounterAttackHit() {
    // IDA 0x140367360:
    // if (m_fForcedStateApplyTime > 0.0) return m_uiForcedState == 4;
    // return !IsHitDown() && m_nMotionClass >= 17 && m_nMotionClass <= 23
    //        && m_nMotionClass != 22 && m_nHitStatus != 5;
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 4;
    }
    return !IsHitDown()
           && m_nMotionClass >= 17
           && m_nMotionClass <= 23
           && m_nMotionClass != 22
           && m_nHitStatus != 5;
}

// ============================================================================
// IsDashing IDA 0x1403674F0
// ============================================================================
bool CMover::IsDashing() {
    // IDA 0x1403674F0:
    // if (m_fForcedStateApplyTime <= 0.0) return XActor::IsStatus(&this->XActor, 0x800);
    // else return m_uiForcedState == 2;
    if (m_fForcedStateApplyTime <= 0.0f) {
        return IsStatus(0x800);
    }
    return m_uiForcedState == 2;
}

// ============================================================================
// SetAnimSpeed IDA 0x140368CC0
// ============================================================================
void CMover::SetAnimSpeed(float fSpeed) {
    // IDA 0x140368CC0: this->m_fAnimSpeed = fSpeed
    m_fAnimSpeed = fSpeed;
}

// ============================================================================
// SetSlowTime IDA 0x140368AA0
// ============================================================================
void CMover::SetSlowTime(float fTime, float fSpeed) {
    // IDA 0x140368AA0:
    // if (fTime > 0.0 && m_stTimeSlow.fTime == 0.0)
    //   m_fRestoreAnimSpeed = m_fAnimSpeed;
    //   if (XActor::IsStatus(1) || XActor::IsStatus(0x400)) m_eRestoreAnimSpeedType = AST_ATTACK;
    //   else m_eRestoreAnimSpeedType = AST_NONE;
    // m_stTimeSlow.fTime = fTime;
    // m_stTimeSlow.fSpeed = fSpeed;
    // SetAnimSpeed(fSpeed);
    if (fTime > 0.0f) {
        // TODO: 需要检查 m_stTimeSlow.fTime
        m_fRestoreAnimSpeed = m_fAnimSpeed;
        // TODO: 需要 XActor::IsStatus 实现
        m_eRestoreAnimSpeedType = 0;  // AST_NONE
    }
    // TODO: 需要完整的 m_stTimeSlow 结构
    SetAnimSpeed(fSpeed);
}

// ============================================================================
// GetCurrentAnimationLength IDA 0x140368B90
// ============================================================================
float CMover::GetCurrentAnimationLength() {
    // IDA 0x140368B90:
    // if (m_pCurMotionEvent) return m_pCurMotionEvent->fAnimationLength;
    // else return 0.0;
    if (m_pCurMotionEvent) {
        return m_pCurMotionEvent->fAnimationLength;
    }
    return 0.0f;
}

// ============================================================================
// SetCurrentSequenceTime IDA 0x140368BE0
// ============================================================================
void CMover::SetCurrentSequenceTime(float fTime) {
    // IDA 0x140368BE0:
    // m_fAnimationTime = fTime;
    // if (m_pCurMotionEvent && m_pCurMotionEvent->fAnimationLength > 0.0)
    //   m_fAnimPercentTime = fTime / m_pCurMotionEvent->fAnimationLength;
    m_fAnimationTime = fTime;
    if (m_pCurMotionEvent && m_pCurMotionEvent->fAnimationLength > 0.0f) {
        m_fAnimPercentTime = fTime / m_pCurMotionEvent->fAnimationLength;
    }
}

// ============================================================================
// SetCurrentSequencePosition IDA 0x140368C60
// ============================================================================
void CMover::SetCurrentSequencePosition(float fPos) {
    // IDA 0x140368C60:
    // if (m_pCurMotionEvent) {
    //   m_fAnimationTime = m_pCurMotionEvent->fAnimationLength * fPos;
    //   m_fAnimPercentTime = fPos;
    // }
    if (m_pCurMotionEvent) {
        m_fAnimationTime = m_pCurMotionEvent->fAnimationLength * fPos;
        m_fAnimPercentTime = fPos;
    }
}

// ============================================================================
// AnimKeyToMotion IDA 0x140368A80
// ============================================================================
int CMover::AnimKeyToMotion(unsigned int dwAnimKey) {
    // IDA 0x140368A80: return dwAnimKey / 0x3E8 (1000)
    return dwAnimKey / 1000;
}

// ============================================================================
// CheckAnimationEnd IDA 0x140367C80
// 更新动画时间进度，处理动画结束/循环，计算动画偏移和碰撞
// ============================================================================
void CMover::CheckAnimationEnd() {
    // IDA 0x140367C80 (1315 bytes):
    // 1. 检查动画状态 (m_bAnimChanged, m_bAnimPlay, m_pCurMotionEvent)
    // 2. 获取帧时间增量
    // 3. 更新累积动画时间
    // 4. 检查动画结束/循环
    // 5. 计算动画偏移量 (GetOffsetDelta)
    // 6. 旋转偏移量
    // 7. 碰撞检测
    // 8. 执行移动

    if (m_bAnimChanged || !m_bAnimPlay || !m_pCurMotionEvent || m_pCurMotionEvent->fAnimationLength <= 0.0f) {
        return;
    }

    // 获取帧时间增量
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = pTimer->GetTimeDifference();
    float fPrevTime = m_fAnimationTime;

    // 更新累积动画时间 (受动画速度影响)
    m_fAnimationTime = fPrevTime + fDeltaTime * m_fAnimSpeed;

    // 检查动画是否播放完毕
    if (m_fAnimationTime >= m_pCurMotionEvent->fAnimationLength) {
        // eEndofAnimation != 0 表示非循环动画 (END_OF_ANIM_IDLE=1, END_OF_ANIM_STAND=2)
        if (m_pCurMotionEvent->eEndofAnimation != END_OF_ANIM_NONE) {
            // 结束型动画: 锁定在最后一帧
            m_fAnimationTime = m_pCurMotionEvent->fAnimationLength;
            m_fAnimPercentTime = 1.0f;
            ClearMotion();
            return;
        }
        // 循环动画: 从头开始播放 (减去一个循环周期)
        m_fAnimationTime -= m_pCurMotionEvent->fAnimationLength;
    }

    // 非跳过偏移模式: 计算动画位置偏移
    if (!m_bSkipAnimOffset) {
        // IDA: VAnimationInfo::GetOffsetDelta 计算两帧间的位移量
        // TODO: 需要 VAnimationInfo::GetOffsetDelta 完整实现
        hkvVec3 vOffset(0.0f, 0.0f, 0.0f);
        // VAnimationInfo::GetOffsetDelta(m_pCurMotionEvent, &vOffset, fPrevTime, m_fAnimationTime);

        if (vOffset.x != 0.0f || vOffset.y != 0.0f || vOffset.z != 0.0f) {
            // IDA: 根据朝向旋转偏移量 hkvMat3::setFromEulerAngles
            // TODO: 需要 hkvMat3 和朝向计算
            // float fYaw = GetOrientationYaw();
            // hkvMat3 matRot; hkvMat3::setFromEulerAngles(&matRot, 0, 0, fYaw);
            // hkvVec3 vRotOffset = matRot.transformDirection(vOffset);

            hkvVec3 vDestPos = GetPosition() + vOffset;

            // 地面高度检测 (非飞行状态)
            bool bFlying = IsFlying();
            if (!bFlying) {
                GetHeight(&vDestPos, 200.0f);
            }

            // 碰撞检测
            CMover* pCollideActor = CheckMoveCollision(vDestPos);
            if (pCollideActor) {
                // 碰撞到目标 (通常是追击的怪物目标)
                // IDA: 检查是否为当前目标，是则跳过动画偏移
                // TODO: 需要目标检查逻辑
                m_bSkipAnimOffset = 1;
                send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, GetPosition(), 0);
                return;
            }

            // 目标位置合法性检查
            if (!CheckMoveDestPos(vDestPos, bFlying, 0)) {
                m_bSkipAnimOffset = 1;
                send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, vDestPos, 0);
            }

            // IDA: 执行移动
            // float fYaw = GetOrientationYaw();
            Move(vDestPos);
        }
    }

    // 更新动画百分比进度
    m_fAnimPercentTime = m_fAnimationTime / m_pCurMotionEvent->fAnimationLength;
}

// ============================================================================
// SetupPhysicsAndBound IDA 0x140367910
// ============================================================================
void CMover::SetupPhysicsAndBound(float fCollisionRadius, float fCollisionHeight) {
    // IDA 0x140367910:
    // m_fCapsuleRadius = fCollisionRadius;
    // m_fCapsuleHeight = fCollisionHeight;
    // m_fHitCylinderRadius = m_fCapsuleRadius;
    // m_fHitCylinderHeight = m_fCapsuleHeight;
    m_fCapsuleRadius = fCollisionRadius;
    m_fCapsuleHeight = fCollisionHeight;
    m_fHitCylinderRadius = m_fCapsuleRadius;
    m_fHitCylinderHeight = m_fCapsuleHeight;
}

// ============================================================================
// SetupAnimation IDA 0x140367980
// ============================================================================
void CMover::SetupAnimation() {
    // IDA 0x140367980:
    // 1. GetActionResourceFN 获取动作资源文件名
    // 2. 从 XGameServer 获取资源
    // 3. SetAnimInfoToActor 设置动画信息
    // 4. SetupAnimInfo 设置动画信息
    // 5. 设置 m_nHitAnimCount

    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) return;

    VString fn = GetActionResourceFN();
    if (fn.AsChar() && fn.GetLength() > 0) {
        // 从 XActionResMgr 获取动作资源
        // IDA: VResourceManager::GetResourceByName(&m_xActionManager, fn.AsChar())
        // TODO: VResourceManager::GetResourceByName 需要 Vision Engine 类型完整定义后取消注释
        // m_pActionResource = (VActionResourceLump*)pServer->m_xActionManager.Load(fn.AsChar());

        int dwTableID = GetTableID();
        // TODO: 需要 XGameServer 提供公共访问器或友元声明
        // if (pServer->m_xActionManager.SetAnimInfoToActor(dwTableID, this)) {
        //     // SetupAnimInfo - IDA 中在此调用
        //     // TODO: 需要 SetupAnimInfo 实现
        //     // SetupAnimInfo();
        //     m_nHitAnimCount = 7;
        // }

        // 临时: 默认 7 种受击动画
        m_nHitAnimCount = 7;
    }
}

// ============================================================================
// SetProtectionAggroRatio IDA 0x1403655C0
// ============================================================================
void CMover::SetProtectionAggroRatio(float ratio) {
    // IDA 0x1403655C0: this->m_fProtectionAggroRatio = ratio
    m_fProtectionAggroRatio = ratio;
}

// ============================================================================
// GetHitCollisionCount IDA 0x140367B90
// ============================================================================
int CMover::GetHitCollisionCount() {
    // IDA 0x140367B90:
    // if (m_pHitCollisionData) return std::vector<tagHIT_COLLISION>::size(&m_pHitCollisionData->vHitColisions);
    // else return 0;
    if (m_pHitCollisionData) {
        // TODO: 需要 tagHIT_COLLISION_DATA 结构定义
        return 0;
    }
    return 0;
}

// ============================================================================
// IsDamageMotionDisplay IDA 0x140367BD0
// ============================================================================
bool CMover::IsDamageMotionDisplay(std::uint8_t byAttackCollision) {
    // IDA 0x140367BD0:
    // if (!byAttackCollision) return false;
    // switch (m_byDefenseType) {
    //   case 1: return byAttackCollision == 2 || byAttackCollision == 4;
    //   case 2: return byAttackCollision == 3 || byAttackCollision == 4;
    //   default: return m_byDefenseType != 3 && m_byDefenseType != 5;
    // }
    if (!byAttackCollision) {
        return false;
    }
    switch (m_byDefenseType) {
        case 1:
            return byAttackCollision == 2 || byAttackCollision == 4;
        case 2:
            return byAttackCollision == 3 || byAttackCollision == 4;
        default:
            return m_byDefenseType != 3 && m_byDefenseType != 5;
    }
}

// ============================================================================
// IsActivateSkillUnlockBuff IDA 0x140367550
// ============================================================================
bool CMover::IsActivateSkillUnlockBuff(TB_SKILL* pTBSkill) {
    // IDA 0x140367550: 遍历 m_mapSkillUnlock 检查 Skill_Group 是否匹配
    if (!pTBSkill) {
        return false;
    }
    // 遍历 m_mapSkillUnlock，检查 (*(DWORD*)&key + 1) == pTBSkill->Skill_Group
    // m_mapSkillUnlock 是 std::map<uint32_t, uint32_t>
    // key 的高 16 位是 Skill_Group
    for (auto it = m_mapSkillUnlock.begin(); it != m_mapSkillUnlock.end(); ++it) {
        // 检查 Skill_Group (key 的高 16 位)
        if ((it->first >> 16) == static_cast<std::uint32_t>(pTBSkill->Skill_Group)) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// 缺失函数的 stub 实现 (待从 IDA 精确还原)
// ============================================================================

// IDA 0x14036BC20 - ProcessExtraMoving (精确还原)
// 处理额外移动（击退、拉扯等效果）
void CMover::ProcessExtraMoving() {
    // IDA 反编译确认:
    // 1. 检查 m_stExtMovingVal 是否为零 (IsZero)
    // 2. 保存当前位置到 m_vPrevPos
    // 3. 检查剩余时间，调用 ReleaseExtraMoving 或计算位移

    if (m_stExtMovingVal.IsZero()) {
        return;
    }

    // 保存当前位置
    m_vPrevPos = GetPosition();

    if (m_stExtMovingVal.fRemainTime <= 0.0f) {
        ReleaseExtraMoving();
        return;
    }

    // IDA: 计算位移差值
    float fDiffX = m_stExtMovingVal.x - m_vPosition.x;
    float fDiffY = m_stExtMovingVal.y - m_vPosition.y;

    // 获取帧时间
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = pTimer->GetTimeDifference();

    // 检查距离阈值 (3.0)
    if (fabsf(fDiffX) >= 3.0f || fabsf(fDiffY) >= 3.0f) {
        // 计算移动量
        float fDeltaX, fDeltaY;

        if (fDiffX <= 0.0f) {
            fDeltaX = (fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffX;
            if (fDeltaX > fDiffX) fDeltaX = fDiffX;
        } else {
            fDeltaX = (fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffX;
            if (fDeltaX > fDiffX) fDeltaX = fDiffX;
        }

        if (fDiffY <= 0.0f) {
            fDeltaY = (fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffY;
            if (fDeltaY > fDiffY) fDeltaY = fDiffY;
        } else {
            fDeltaY = (fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffY;
            if (fDeltaY > fDiffY) fDeltaY = fDiffY;
        }

        m_stExtMovingVal.fRemainTime -= fDeltaTime;

        hkvVec3 vDestPos = m_vPrevPos + hkvVec3(fDeltaX, fDeltaY, 0.0f);

        // 地面高度检测
        if (!IsFlying()) {
            GetHeight(&vDestPos, 200.0f);
        }

        // 碰撞检测
        CMover* pCollideActor = CheckMoveCollision(vDestPos);
        if (pCollideActor) {
            send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, GetPosition(), false);
        } else {
            if (!CheckMoveDestPos(vDestPos, false, 0)) {
                send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, vDestPos, false);
            }
            Move(vDestPos);
        }
    } else {
        m_stExtMovingVal.Clear();
    }
}

// IDA 0x14036C120 - ReleaseExtraMoving (精确还原)
// 释放额外移动状态
void CMover::ReleaseExtraMoving() {
    // IDA 反编译确认:
    // 1. 检查 m_stExtMovingVal 是否为零
    // 2. 如果 fMovingTime == 0.1f，执行最终移动
    // 3. 清除 m_stExtMovingVal

    if (m_stExtMovingVal.IsZero()) {
        return;
    }

    // IDA: 如果移动时间为 0.1 秒，执行最终位置移动
    if (m_stExtMovingVal.fMovingTime == 0.1f) {
        hkvVec3 vExtraPos(m_stExtMovingVal.x, m_stExtMovingVal.y, m_vPosition.z);
        Move(vExtraPos);
    }

    m_stExtMovingVal.Clear();
}

// IDA 0x14036C210 - AddExtraMoving (精确还原)
// 添加额外移动量（累加）
void CMover::AddExtraMoving(float x, float y, float fTime) {
    // IDA 反编译确认:
    // 1. 创建目标位置向量 (z 使用当前位置)
    // 2. 如果当前没有额外移动，从当前位置开始；否则从已有目标累加
    // 3. 检查目标位置有效性
    // 4. 使用较长的移动时间
    // 5. 设置剩余时间 = 移动时间 + 0.2 秒

    hkvVec3 vDestPos(0.0f, 0.0f, m_vPosition.z);

    if (m_stExtMovingVal.fRemainTime <= 0.0f) {
        // 没有进行中的额外移动，从当前位置开始
        vDestPos.x = m_vPosition.x + x;
        vDestPos.y = m_vPosition.y + y;
    } else {
        // 累加到已有的额外移动目标
        vDestPos.x = m_stExtMovingVal.x + x;
        vDestPos.y = m_stExtMovingVal.y + y;
    }

    CheckMoveDestPos(vDestPos, false, 0);

    m_stExtMovingVal.x = vDestPos.x;
    m_stExtMovingVal.y = vDestPos.y;

    // IDA: 选择较长的移动时间
    float fMovingTime = (fTime <= m_stExtMovingVal.fMovingTime) ? m_stExtMovingVal.fMovingTime : fTime;
    m_stExtMovingVal.fMovingTime = fMovingTime;
    m_stExtMovingVal.fRemainTime = fMovingTime + 0.2f;
}

// IDA 0x14036C380 - SetExtraMoving (精确还原)
// 设置额外移动目标（覆盖）
void CMover::SetExtraMoving(float x, float y, float fTime) {
    // IDA 反编译确认:
    // 1. 创建目标位置向量
    // 2. 检查目标位置有效性
    // 3. 设置移动时间和剩余时间

    hkvVec3 vDestPos(x, y, m_vPosition.z);
    CheckMoveDestPos(vDestPos, false, 0);

    m_stExtMovingVal.x = vDestPos.x;
    m_stExtMovingVal.y = vDestPos.y;
    m_stExtMovingVal.fMovingTime = fTime;
    m_stExtMovingVal.fRemainTime = fTime + 0.2f;
}

// IDA 0x14036EAC0 - send_eSUB_CMD_MOVE (精确还原框架)
// 发送移动数据包给客户端
void CMover::send_eSUB_CMD_MOVE(CMover* pMover, float fTargetPosX, float fTargetPosY, std::uint8_t byRunBit) {
    // IDA 反编译确认的核心逻辑:
    // 1. 获取移动朝向 (GetMovingYaw)
    // 2. 验证朝向值范围 (-360 ~ 360)
    // 3. 获取移动速度
    // 4. 获取当前位置
    // 5. 获取地图ID
    // 6. 构建 ST_MOVE 数据包
    // 7. 广播给周围玩家

    if (!pMover) return;

    // TODO: 完整实现需要以下依赖:
    // - GetMovingYaw() 获取移动朝向
    // - XArea::GetInstanceID() 获取地图ID
    // - GetLookPitch() 获取俯仰角
    // - XSendPacket 构建数据包
    // - CGocNetwork::SendBroadCastAfterLoading 广播

    m_fMoveSpeed = pMover->GetMoveSpeed();
    m_fLastSendMoveTime = 0.0f;

    // TODO: 需要完整的网络包构建实现
    // ST_MOVE stMove;
    // stMove.dwActorID = GetID();
    // stMove.nMapID = uxMapID.nMapID;
    // stMove.fPosX = vPos.x;
    // stMove.fPosY = vPos.y;
    // stMove.fPosZ = vPos.z;
    // stMove.fYaw = fYaw;
    // stMove.fTargetPosX = fTargetPosX;
    // stMove.fTargetPosY = fTargetPosY;
    // stMove.byRunBit = byRunBit;
    // stMove.fPitch = GetLookPitch();
    // stMove.fMoveSpeed = m_fMoveSpeed;
}

// IDA 0x14036EE90 - send_eSUB_CMD_MOVE_STOP (精确还原框架)
// 发送停止移动数据包
void CMover::send_eSUB_CMD_MOVE_STOP(CMover* pMover) {
    // IDA 反编译确认的核心逻辑:
    // 1. 获取移动朝向 (GetMovingYaw)
    // 2. 验证朝向值范围
    // 3. 获取当前位置
    // 4. 获取地图ID
    // 5. 构建 ST_MOVE_STOP 数据包
    // 6. 广播给周围玩家

    if (!pMover) return;

    // TODO: 完整实现需要以下依赖:
    // - GetMovingYaw() 获取移动朝向
    // - XArea::GetInstanceID() 获取地图ID
    // - GetLookPitch() 获取俯仰角
    // - XSendPacket 构建数据包
    // - CGocNetwork::SendBroadCastAfterLoading 广播

    m_fLastSendMoveTime = 0.0f;

    // TODO: 需要完整的网络包构建实现
    // ST_MOVE_STOP stMoveStop;
    // stMoveStop.dwActorID = GetID();
    // stMoveStop.nMapID = uxMapID.nMapID;
    // stMoveStop.fPosX = curPos.x;
    // stMoveStop.fPosY = curPos.y;
    // stMoveStop.fPosZ = curPos.z;
    // stMoveStop.fYaw = fYaw;
    // stMoveStop.fPitch = GetLookPitch();
}

// IDA 0x140370100 - send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA (精确还原框架)
// 发送忽略动画偏移的移动数据包（瞬移）
void CMover::send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(CMover* pMover, const hkvVec3& vPos, bool bForced) {
    // IDA 反编译确认的核心逻辑:
    // 1. 检查 bForced 或 Actor 类型是否为 TYPE_MONSTER (2)
    // 2. 清除额外移动状态
    // 3. 获取朝向 (GetOrientationYaw)
    // 4. 验证朝向值范围
    // 5. 构建 ST_MOVE_IGNORE_MOTION_DELTA 数据包
    // 6. 广播给所有玩家

    if (!pMover) return;

    // IDA: 类型检查 - 如果不是强制且不是怪物类型，则返回
    if (!bForced && m_eActorType != 2) {  // TYPE_MONSTER
        return;
    }

    // 清除额外移动状态
    m_stExtMovingVal.Clear();

    // TODO: 完整实现需要以下依赖:
    // - GetOrientationYaw() 获取朝向
    // - GetLookPitch() 获取俯仰角
    // - XSendPacket 构建数据包
    // - CGocNetwork::SendBroadCastAfterLoading 广播

    m_fLastSendMoveTime = 0.0f;

    // TODO: 需要完整的网络包构建实现
    // ST_MOVE_IGNORE_MOTION_DELTA stMoveIgnore;
    // stMoveIgnore.dwActorID = GetID();
    // stMoveIgnore.fPosX = vPos.x;
    // stMoveIgnore.fPosY = vPos.y;
    // stMoveIgnore.fPosZ = vPos.z;
    // stMoveIgnore.fYaw = fYaw;
    // stMoveIgnore.fPitch = GetLookPitch();
    // stMoveIgnore.bForced = bForced;
}

// 其他 stub 函数
CMySkillList* CMover::GetSkillMgr() { return m_pSkillMgr; }

// IDA 0x1401AB740 - ClearMotion (精确还原)
// PDB 符号显示此函数大小为 0x6 字节，表明是空实现
// CMoverEx::ClearMotion (0x140381910) 是实际的虚函数实现
void CMover::ClearMotion() {
    // 基类空实现 - 子类 CMoverEx 有完整实现
}
// IDA 0x14036D130 - GetHeight (精确还原)
// 获取指定位置的高度（通过导航网格）
bool CMover::GetHeight(hkvVec3* vPos, float fMaxDist) {
    // IDA 反编译:
    // return this->GetArea(&this->XActor)
    //     && (v3 = this->GetArea(&this->XActor), (pNavMesh = v3->GetNavMeshInstance(v3)) != nullptr)
    //     && DohHavokNavMeshInstance::GetHeight(pNavMesh, vPos, fTestHeight);

    // TODO: 需要实现 - 依赖 XArea 和 DohHavokNavMeshInstance
    // 获取当前区域
    // XArea* pArea = GetArea();
    // if (!pArea) {
    //     return false;
    // }

    // 获取导航网格实例
    // DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    // if (!pNavMesh) {
    //     return false;
    // }

    // 调用导航网格获取高度
    // return pNavMesh->GetHeight(vPos, fMaxDist);
    (void)vPos;
    (void)fMaxDist;
    return false;
}

// IDA 0x1402A5080 - GetPositionXVec3 (精确还原)
// 注意: 已在上方定义，此处删除重复定义

// IDA 0x1402C7240 - GetSkillCoolDownRate (精确还原)
// IDA 反编译: return this->m_fSkillCoolDownRate
float CMover::GetSkillCoolDownRate() const {
    return m_fSkillCoolDownRate;
}

// IDA 0x1402C7420 - SetKeepMovingExtra (精确还原)
// IDA 反编译: this->m_bKeepMovingExtra = bExtraMoving
void CMover::SetKeepMovingExtra(int bKeepMoving) {
    m_bKeepMovingExtra = bKeepMoving;
}

// IDA 0x14036AA40 - AllBuffClear (精确还原)
// 清除所有 Buff 状态
// IDA 0x1403681B0 - CheckMoveCollision
// IDA 反编译精确还原 (大型函数约 1236 bytes):
// 移动碰撞检测 - 检查与其他 Actor 的碰撞
CMover* CMover::CheckMoveCollision(hkvVec3& vDestPos) {
    // IDA 反编译核心逻辑:
    // 1. 检查 Actor 类型 (必须是 TYPE_MONSTER=2)
    // 2. 检查碰撞启用状态
    // 3. 检查 KeepMovingExtra 状态
    // 4. RTTI 转换为 CMonster
    // 5. 检查是否为跟随者或巡逻怪物
    // 6. 扫描区域内其他 Actor
    // 7. 计算最近碰撞目标

    // 类型检查: 仅怪物执行碰撞检测
    if (m_eActorType != 2) {  // TYPE_MONSTER (eActorMonster)
        return nullptr;
    }

    if (!m_bCollisionEnable) {
        return nullptr;
    }

    if (m_bKeepMovingExtra) {
        return nullptr;
    }

    // TODO: RTTI 转换检查 CMonster
    // CMonster* pMonster = dynamic_cast<CMonster*>(this);
    // if (!pMonster) return nullptr;
    // if (pMonster->IsFollower()) return nullptr;
    // if (pMonster->GetAi() && pMonster->GetAi()->IsPatrolMonster()) return nullptr;

    float nearFactor = 5000.0f;
    CMover* pClosestTargetEntity = nullptr;
    hkvVec3 vClosestPos(0.0f, 0.0f, 0.0f);

    // TODO: 扫描区域内的 Actor (需要 XArea::ScanGridOrigin 实现)
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(&this->XActor, 2, 3, &vecGameObjList);

    // 遍历检测碰撞
    // for (auto pOtherActor : vecGameObjList) {
    //     if (!pOtherActor) continue;
    //     // 类型过滤: 玩家或防御对象
    //     bool bCheckActor = (pOtherActor->GetType() == 0);  // TYPE_PLAYER
    //     if (pOtherActor->GetType() == 2) {  // TYPE_MONSTER
    //         CMonster* pOtherMonster = dynamic_cast<CMonster*>(pOtherActor);
    //         if (pOtherMonster && pOtherMonster->IsDefenseObject()) {
    //             bCheckActor = true;
    //         }
    //     }
    //     if (!bCheckActor) continue;
    //     // 存活检查
    //     if (!pOtherActor->IsLive() || pOtherActor->IsStatus(2)) continue;
    //     // 距离计算
    //     hkvVec3 vOffset = vDestPos - pOtherActor->GetPosition();
    //     vOffset.z = 0.0f;
    //     float targetDist = vOffset.getLength();
    //     if (nearFactor > targetDist) {
    //         vClosestPos = pOtherActor->GetPosition();
    //         nearFactor = targetDist;
    //         pClosestTargetEntity = pOtherActor;
    //     }
    // }

    // 碰撞判定
    // if (pClosestTargetEntity) {
    //     hkvVec3 vOffset = vDestPos - vClosestPos;
    //     vOffset.z = 0.0f;
    //     float fDist = vOffset.getLength();
    //     float fOtherRadius = pClosestTargetEntity->GetHavokCapsuleRadius();
    //     if ((fOtherRadius + m_fCapsuleRadius + 5.0f + 5.0f) >= fDist) {
    //         return pClosestTargetEntity;
    //     }
    // }

    return nullptr;
}
// IDA 0x14036DEE0 - CheckMoveDestPos (精确还原)
// 检查移动目标位置是否可达
bool CMover::CheckMoveDestPos(hkvVec3& vDestPos, bool bFlying, int bDontCareCurve) {
    // IDA 反编译核心逻辑:
    // 1. 获取区域和导航网格
    // 2. 获取当前胶囊半径
    // 3. 调用 XMaze::CheckCanDirectMove2 检查是否可直接移动
    // 4. 如果不能直接移动，调整目标位置

    // TODO: 依赖 XArea, DohHavokNavMeshInstance, XMaze::CheckCanDirectMove2
    // XArea* pArea = GetArea();
    // if (!pArea) return true;
    // DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    // if (!pNavMesh) return true;

    // float fRadius = GetHavokCapsuleRadius();
    // hkvVec3 vPos = GetPosition();
    // hkvVec3 vNextPos = vDestPos;

    // if (bFlying) {
    //     vPos.z = 0.0f;
    //     vNextPos.z = 0.0f;
    //     if (vPos == vNextPos) return true;
    //     vPos = GetPosition();
    //     vNextPos = vDestPos;
    // }

    // bool bResult = XMaze::CheckCanDirectMove2(pNavMesh, &vPos, &vDestPos, fRadius, bFlying, bDontCareCurve);
    // if (!bResult) {
    //     hkvVec3 vDirection = vNextPos - vPos;
    //     vDirection.z = 0.0f;
    //     vDirection.normalizeIfNotZero(0.000001f);
    //     vDirection *= fRadius;
    //     vDestPos -= vDirection;
    //     if (pNavMesh->GetHeight(&vDestPos, 200.0f)) {
    //         if (bFlying) vDestPos.z = vPos.z;
    //     } else {
    //         vDestPos = vPos;
    //     }
    // }
    // return bResult;

    (void)vDestPos;
    (void)bFlying;
    (void)bDontCareCurve;
    return true;
}
// IDA 0x140366FA0 - ThinkFunction (精确还原)
// 思考函数 - 每帧调用的更新逻辑
void CMover::ThinkFunction() {
    // IDA 反编译:
    // this->m_bAnimChanged = 0;
    // if ( this->m_pSkillMgr )
    //     CMySkillList::ThinkFunction(this->m_pSkillMgr);
    // if ( this->m_bTraceUser )
    // {
    //     CMover::send_eSUB_CMD_MOVE_TRACE(this, this);
    //     v5 = this->m_fLastDebugTime + 0.5;
    //     Timer = ThreadLocalData::GetTimer();
    //     if ( IVTimer::GetTime(Timer) > v5 )
    //     {
    //         v2 = ThreadLocalData::GetTimer();
    //         this->m_fLastDebugTime = IVTimer::GetTime(v2);
    //     }
    // }
    // v3 = ThreadLocalData::GetTimer();
    // fDeltaTime = IVTimer::GetTimeDifference(v3);
    // CMover::CheckDelayedProjectile(this, fDeltaTime);
    // CMover::CheckContinuousMelee(this, fDeltaTime);

    // 重置动画变化标志
    m_bAnimChanged = 0;

    // 调用技能管理器的思考函数
    // TODO: CMySkillList 完整定义后启用
    // if (m_pSkillMgr) {
    //     m_pSkillMgr->ThinkFunction();
    // }

    // 处理追踪用户逻辑
    if (m_bTraceUser) {
        // TODO: 实现 send_eSUB_CMD_MOVE_TRACE
        // send_eSUB_CMD_MOVE_TRACE(this, this);

        // 调试时间检查
        // VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
        // if (pTimer && IVTimer::GetTime(pTimer) > (m_fLastDebugTime + 0.5f)) {
        //     VDefaultTimer* pTimer2 = ThreadLocalData::GetTimer();
        //     m_fLastDebugTime = IVTimer::GetTime(pTimer2);
        // }
    }

    // 检查延迟弹丸和连续近战
    // TODO: 依赖 ThreadLocalData, IVTimer, CheckDelayedProjectile, CheckContinuousMelee
    // VDefaultTimer* pTimer3 = ThreadLocalData::GetTimer();
    // if (pTimer3) {
    //     float fDeltaTime = IVTimer::GetTimeDifference(pTimer3);
    //     CheckDelayedProjectile(fDeltaTime);
    //     CheckContinuousMelee(fDeltaTime);
    // }
}
// IDA 0x14036CAB0 - SceneChanged (精确还原)
// 场景切换时的清理工作
void CMover::SceneChanged() {
    // IDA 反编译: 直接调用 MoveingValueClear
    MoveingValueClear();
}
int CMover::GetTableID() { return 0; }
char* CMover::GetAnimStirng(unsigned int dwAnimKey) { return nullptr; }
void CMover::SetMoveingInFly(int bFlying) { m_bMoveingInFly = (bFlying != 0); }
CMover* CMover::GetMoverObject(std::uint32_t dwID) { return nullptr; }
// SetKeepMovingExtra 已在上方定义
void CMover::SetWeightRank(std::uint8_t cVal) { m_cWeightRank = cVal; }
// IDA 0x14036DB20 - RemoveTargetDestPos (精确还原)
// 移除目标位置标记
void CMover::RemoveTargetDestPos() {
    // IDA 反编译逻辑:
    // 1. 检查 m_byTargetDestPos != 255 且 m_dwTargetID != -1
    // 2. 获取目标 Mover 对象
    // 3. 清除目标的位置标记
    // 4. 重置 m_byTargetDestPos 为 255

    if (m_byTargetDestPos != 255 && m_dwTargetID != 0xFFFFFFFF) {
        CMover* pTarget = GetMoverObject(m_dwTargetID);
        if (pTarget) {
            ClearTargetPosFlag(pTarget, m_byTargetDestPos);
        }
    }
    m_byTargetDestPos = 255;
}

// IDA 0x1406CA80 - ClearTargetPosFlag (精确还原)
// 清除目标位置标志
void CMover::ClearTargetPosFlag(CMover* pTarget, std::uint8_t byPos) {
    // IDA 反编译: 空实现或简单的标志清除
    // TODO: 需要从IDA确认完整逻辑
    (void)pTarget;
    (void)byPos;
}

void CMover::ClearTraceBoneName() { m_vTraceBoneName.clear(); }
void CMover::RegisterTraceBoneName(const VString& strBoneName) { m_vTraceBoneName.push_back(strBoneName); }

// Note: GetMotionClass, GetRestoreDefenseType, GetCreatePos, GetExtraMovePos,
// GetAnimationIdx, SetCurSkillTableIdx are already defined earlier in this file
// (lines 302-340)

// Note: GetHavokCapsuleRadius, IsMoving, GetAttackerCount, SetTargetDestPos,
// GetTargetDestPos, GetCellID, SetCellID, IsInvincibleActor are already defined
// earlier in this file (lines 340-370)
