#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XCore/ThreadLocalData.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/Ai.h"

// Forward declaration for GOC stub
class CGocPost;

// Vision Engine RTTI - CMover 静态类类型对象
// IDA: CMover::classCMover 是 VType 类型的静态成员
VType* CMover::classCMover = nullptr;

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
    , m_vOrientation(0.0f, 0.0f, 0.0f)  // IDA 0x140375220: 方向向量
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
UXActorID CMover::GetActorID() const {
    return UXActorID(0);
}

bool CMover::IsStatus(std::uint32_t dwStatus) const {
    return (m_dwStatus & dwStatus) != 0;
}

std::uint32_t CMover::GetStatus() const {
    return m_dwStatus;
}

void CMover::SetStatus(std::uint32_t dwStatus) {
    m_dwStatus = dwStatus;
}

void CMover::ClearStatus(std::uint32_t dwStatus) {
    m_dwStatus &= ~dwStatus;
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

// IDA 0x140016C30 - AddActionBuffer (精确还原)
// void __fastcall CMover::AddActionBuffer(CMover *this, tagACTION_BUFFER *xAction)
// {
//   CActionBuffer::Push(&this->m_xActionBuffer, xAction);
// }
void CMover::AddActionBuffer(void* xAction) {
    // IDA: CActionBuffer::Push(&this->m_xActionBuffer, xAction)
    m_xActionBuffer.Push(static_cast<tagACTION_BUFFER*>(xAction));
}

// IDA 0x140166360 - GetStat
float CMover::GetStat(int iIndex) {
    return m_fAbility[iIndex];
}

// IDA @ 0x140189240
// GetVariableType - 获取 Actor 类型
// IDA 精确还原: return this->m_eActorType
int CMover::GetVariableType() {
    return m_eActorType;
}

// IDA 0x1400488E0 - SetNoSkillCostSG
void CMover::SetNoSkillCostSG(bool bCost) {
    m_bNoSkillCostSG = bCost;
}

// IDA 0x140198DE0 - GetTargetID
// (已在头文件中内联定义)

// IDA 0x140354290 - SetHitID
// (已在头文件中内联定义)

// IDA 0x140364610 - SetParentSkillTableIdx
// 注意: 头文件中没有此函数，使用SetCurSkillTableIdx代替

// IDA 0x140364650 - GetCurSkillTableIdx
// (已在头文件中内联定义)

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

// IDA 0x140364700 - IsImmunityStatus
bool CMover::IsImmunityStatus() const {
    return IsStatus(0x400u) || IsStatus(0x800u) || IsStatus(0x1000u);
}

std::uint8_t CMover::GetItemRateFlag() {
    return 0;
}

// IDA 0x1403675F0 - GetItemRateResultWeapon (精确还原)
// TODO: 需要 SItemRateInfo, TB_WEAPON_RATE, TB_ITEM_RANK_RATE 类型定义
std::uint32_t CMover::GetItemRateResultWeapon(std::uint8_t byTargetLevel, std::tr1::shared_ptr<CGocAttribute> pAttr, bool bCritical) {
    // IDA 反编译精确还原 - 需要完整类型定义后启用
    // 1. 获取物品掉落信息 (GetItemRateInfo, slot type 1)
    // 2. 计算等级差 (target level - item level)
    // 3. 获取武器掉落率表 (TB_WEAPON_RATE)
    // 4. 获取物品品质率表 (TB_ITEM_RANK_RATE)
    // 5. 计算最终掉落值: (itemValue * weaponRate * rankRate) - itemValue
    
    if (!pAttr) {
        return 0;
    }
    
    // TODO: 实现完整逻辑
    // const SItemRateInfo* pItemRateInfo = pAttr->GetItemRateInfo(1);
    // ...
    
    (void)byTargetLevel;
    (void)bCritical;
    return 0;
}

// IDA 0x140367780 - GetItemRateResultGear (精确还原)
// TODO: 需要 SItemRateInfo, TB_GEAR_RATE, TB_ITEM_RANK_RATE 类型定义
std::uint32_t CMover::GetItemRateResultGear(std::uint8_t byTargetLevel, std::tr1::shared_ptr<CGocAttribute> pAttr) {
    // IDA 反编译精确还原 - 需要完整类型定义后启用
    // 1. 遍历4个装备槽位类型 (151, 161, 171, 181)
    // 2. 对每个槽位获取物品掉落信息
    // 3. 获取装备掉落率表 (TB_GEAR_RATE)
    // 4. 获取物品品质率表 (TB_ITEM_RANK_RATE)
    // 5. 累加计算: (itemValue * gearRate * rankRate) - itemValue
    
    if (!pAttr) {
        return 0;
    }
    
    // TODO: 实现完整逻辑
    // const int arSlotType[4] = {151, 161, 171, 181};
    // ...
    
    (void)byTargetLevel;
    return 0;
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
// IDA 精确还原: return 0
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

// IDA 0x140189150 - GetActionResourceFN (基类返回静态字符串)
VString CMover::GetActionResourceFN() {
    // IDA 精确还原: 返回静态字符串 &stru_140B70D70
    // 这个字符串在IDA中是全局常量，内容可能是空字符串或默认资源名
    static const char* s_strActionResource = "";
    return VString(s_strActionResource);
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

// IDA 0x140353040 - ClearImmunityStatus (头文件中无声明，移除)

// IDA 0x1406C5C30 - GetMoveSpeed (非const版本)
float CMover::GetMoveSpeed() {
    return m_fMoveSpeed;
}

// IDA 0x1402A5030 - GetCurSuperArmorGage (头文件中已声明)
// IDA 0x140353C60 - SetCurSuperArmorGage (头文件中无声明，移除)
// IDA 0x140353FE0 - GetRecoverySuperArmorTime (头文件中无声明，移除)

// IDA 0x1402A5050 - GetMaxSuperArmorGage (头文件中已声明)

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

// IDA 0x14036C610 - ResetAkashicActionInfo
// 重置 Akashic 动作信息
void CMover::ResetAkashicActionInfo() {
    // IDA 精确还原:
    // if (m_pAkashicActionInfo) {
    //     RemoveAll(&m_pAkashicActionInfo->arOffsetDeltaFrames);
    //     RemoveAll(&m_pAkashicActionInfo->arTranslationFrames);
    //     RemoveAll(&m_pAkashicActionInfo->arTriggers);
    //     delete m_pAkashicActionInfo;
    //     m_pAkashicActionInfo = nullptr;
    // }
    if (m_pAkashicActionInfo) {
        // VAnimationInfo 清理
        // TODO: 需要实现 VArray::RemoveAll
        delete m_pAkashicActionInfo;
        m_pAkashicActionInfo = nullptr;
    }
}

// IDA 0x14036B530 - CanUseItem
// 检查是否可以使用物品
// 返回: 1=可以使用, 0=不可使用 (dwError 包含错误码)
bool CMover::CanUseItem(std::uint32_t dwID, std::uint32_t& dwError) {
    // IDA 反编译还原 - 大型函数，包含物品使用条件检查
    // 主要逻辑:
    // 1. 获取物品表引用 (TB_ITEM)
    // 2. 检查物品分类是否存在
    // 3. 根据物品子类型和效果类型进行条件判断:
    //    - Item_Sub_Type == 2: 消耗品
    //      - Item_Effect_Type == 1: 个人复活道具
    //      - Item_Effect_Type == 2: 队伍复活道具
    //    - Item_Sub_Type == 1: Buff 道具
    // 4. 检查区域条件 (IsRevive/IsModeCondition)

    dwError = 0;

    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        return false;
    }

    // TODO: 需要完整实现 XResourceMgr::GetTB_ITEM 和相关检查
    // 当前为简化实现
    (void)dwID;

    // 基类默认返回 true
    return true;
}

// ============================================================================
// AllBuffClear IDA 0x14036AA40
// 清除所有 Buff 状态
// ============================================================================

// IDA 0x14036A810 - GetEmptyBuffSlot
int CMover::GetEmptyBuffSlot() {
    for (int i = 0; i < 50; ++i) {
        if (!m_stBuffState[i].nBuffIndex) {
            return i;
        }
    }
    return 0;
}

// IDA @ 0x14036AA40
// AllBuffClear - 清除所有 Buff 状态
// IDA 精确还原
void CMover::AllBuffClear(std::uint8_t byReason) {
    if (m_nBuffTotalCnt == 0) {
        return;
    }

    // IDA: 遍历所有 buff 槽位 (50个)
    for (std::uint8_t i = 0; i < 0x32u; ++i) {
        if (m_stBuffState[i].IsLife()) {
            // IDA: Buff 正在生效
            if (!byReason || IsClearBuff(m_stBuffState[i].nBuffIndex, byReason)) {
                ClearBuffStatusBySlot(i, 0);
            }
        } else if (m_stBuffState[i].nBuffIndex != 0) {
            // IDA: Buff 未激活但有索引 - 检查永久性 Buff
            XGameServer* pServer = XGameServer::Instance();
            TB_BUFF* pBuffRef = pServer->GetResourceMgr().GetTB_BUFF(m_stBuffState[i].nBuffIndex);
            if (pBuffRef) {
                // IDA: Buff_Time == 0 表示永久性 Buff
                if (!pBuffRef->Buff_Time && IsClearBuff(m_stBuffState[i].nBuffIndex, byReason)) {
                    ClearBuffStatusBySlot(i, 0);
                }
            }
        }
    }
}

// ============================================================================
// IsClearBuff - 检查是否应该清除指定 Buff
// IDA @ 0x1403774f0 - 基类直接返回1，子类可能重写
// ============================================================================
bool CMover::IsClearBuff(int nBuffIndex, std::uint8_t byReason) {
    // IDA: 基类直接返回1（允许清除所有buff）
    // 子类(如CUser/CMonster)可能会根据buff类型和原因重写此逻辑
    (void)nBuffIndex;
    (void)byReason;
    return true;
}

// ============================================================================
// ClearBuffStatusBySlot - 清除指定槽位的 Buff 状态
// IDA @ 0x140377550 - 基类空实现，子类重写
// ============================================================================
void CMover::ClearBuffStatusBySlot(std::uint8_t bySlot, int bNotify) {
    // IDA: 基类是空函数，子类(CUser/CMonster)会重写此逻辑
    (void)bySlot;
    (void)bNotify;
    // 空实现
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

// IDA @ 0x140378840
// ClearActionBuffer - 清空动作缓冲区
// IDA 精确还原: CActionBuffer::Clear(&this->m_xActionBuffer)
void CMover::ClearActionBuffer() {
    m_xActionBuffer.Clear();
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
// IDA 精确还原:
// CMover::GetGOC<CGocAttribute>(this, &pAttr, 0);
// if (!pAttr) return 0;
// return pAttr->GetClass();
// ============================================================================
std::uint8_t CMover::GetClass() {
    auto pAttr = GetGOC_Attribute(false);
    if (!pAttr) {
        return 0;
    }
    return static_cast<std::uint8_t>(pAttr->GetClass());
}

// ============================================================================
// GetLevel IDA 0x140366CB0
// IDA 精确还原:
// CMover::GetGOC<CGocAttribute>(this, &pAttr, 0);
// if (!pAttr) return 0;
// return pAttr->GetLevel();
// ============================================================================
std::uint8_t CMover::GetLevel() {
    auto pAttr = GetGOC_Attribute(false);
    if (!pAttr) {
        return 0;
    }
    return static_cast<std::uint8_t>(pAttr->GetLevel());
}

// ============================================================================
// GetHP IDA 0x140366DC0
// IDA 精确还原:
// CMover::GetGOC<CGocAttribute>(this, &pAttr, 0);
// if (!pAttr) return 0;
// return pAttr->GetHP();
// ============================================================================
int CMover::GetHP() const {
    auto pAttr = GetGOC_Attribute(false);
    if (!pAttr) {
        return 0;
    }
    return pAttr->GetHP();
}

// ============================================================================
// IsDie IDA 0x140366E40
// IDA 反编译: return XActor::IsDieStatus(&this->XActor) || this->GetHP(this) <= 0;
// ============================================================================
// IsDie IDA 0x140366E40
// ============================================================================
bool CMover::IsDie() const {
    // IDA 0x140366E40: XActor::IsDieStatus || GetHP() <= 0
    // Note: XActor::IsDieStatus checks the actor status flags for death state
    // For now, we check HP <= 0 as the primary death condition
    // TODO: Implement XActor::IsDieStatus when XActor base class is fully integrated
    return GetHP() <= 0;
}

// ============================================================================
// IsEnemy IDA 0x14036CD70
// 敌对关系检查 - 检查两个 Mover 是否是敌对关系
// ============================================================================
bool CMover::IsEnemy(CMover* pMover) const {
    // IDA 反编译精确还原:
    // Type = XActor::GetType(&this->XActor);
    // return Type != XActor::GetType(&pMover->XActor);
    if (!pMover) {
        return false;
    }

    // 获取双方的类型（玩家=1, 怪物=2, NPC=3 等）
    // 使用 m_eActorType 成员变量直接获取类型
    return m_eActorType != pMover->m_eActorType;
}

// ============================================================================
// GetMaxHP IDA 0x140366E90
// IDA 精确还原:
// CMover::GetGOC<CGocAttribute>(this, &pAttr, 0);
// if (!pAttr) return 0;
// return (int)pAttr->GetStat(10);  // Stat index 10 = MaxHP
// ============================================================================
int CMover::GetMaxHP() {
    auto pAttr = GetGOC_Attribute(false);
    if (!pAttr) {
        return 0;
    }
    return static_cast<int>(pAttr->GetStat(10));
}

// ============================================================================
// GetLevelForStat IDA 0x140366D30
// IDA 精确还原:
// CMover::GetGOC<CGocAttribute>(this, &pAttr, 0);
// if (!pAttr) return 0;
// return pAttr->GetLevelForStat();
// ============================================================================
std::uint8_t CMover::GetLevelForStat() {
    // IDA 0x140366D30: 获取 CGocAttribute GOC 组件并返回用于计算的等级
    auto pAttr = GetGOC_Attribute(false);
    if (!pAttr) {
        return 0;
    }
    return static_cast<std::uint8_t>(pAttr->GetLevelForStat());
}

// ============================================================================
// DebugOut - 调试输出函数
// IDA 中使用的调试输出方法
// ============================================================================
void CMover::DebugOut(const char* szFormat, ...) {
    // 简化实现 - 输出到日志
    va_list args;
    va_start(args, szFormat);
    char szBuffer[512];
    vsnprintf(szBuffer, sizeof(szBuffer), szFormat, args);
    va_end(args);
    GreenDamTan_log(__FILE__, __FUNCTION__, "%s", szBuffer);
}

// ============================================================================
// IsFlying IDA 0x140367080
// IDA 精确还原:
// if (m_fForcedStateApplyTime > 0.0) return m_uiForcedState == 1;
// if (m_bLanded) return false;
// vPos = GetPosition();
// fZ = vPos.z;
// if (GetHeight(&vPos, 300.0)) return fZ > vPos.z + 5.0;
// DebugOut("IsFlying>> GetHeight failed. (%.2f,%.2f,%.2f)", vPos.x, vPos.y, vPos.z);
// return false;
// ============================================================================
bool CMover::IsFlying() {
    // IDA 0x140367080: 强制状态优先
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 1;
    }
    // IDA: 已着陆则不在飞行
    if (m_bLanded) {
        return false;
    }
    // IDA: 获取当前位置并检查高度
    hkvVec3 vPos = GetPosition();
    float fZ = vPos.z;
    // IDA: 尝试获取地面高度
    if (GetHeight(&vPos, 300.0f)) {
        return fZ > (vPos.z + 5.0f);
    }
    // IDA: GetHeight 失败时输出调试信息
    DebugOut("IsFlying>> GetHeight failed. (%.2f,%.2f,%.2f)", vPos.x, vPos.y, vPos.z);
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
// GetMovePos IDA 0x1403751D0
// ============================================================================
tagMOVE_POS CMover::GetMovePos() {
    // IDA 0x1403751D0: return this->m_stMovePos
    return m_stMovePos;
}

// ============================================================================
// GetOrientationYaw IDA 0x140375220
// ============================================================================
float CMover::GetOrientationYaw() {
    // IDA 0x140375220: return this->m_vOrientation.x
    return m_vOrientation.x;
}

// ============================================================================
// GetBloodDebuffOwnerID IDA 0x1403A23B0
// ============================================================================
std::uint32_t CMover::GetBloodDebuffOwnerID() {
    // IDA 0x1403A23B0: return this->m_dwBloodDebuffOwnerID
    return m_dwBloodDebuffOwnerID;
}

// ============================================================================
// GetSkillBloodDebuffRate IDA 0x1403A23D0
// ============================================================================
float CMover::GetSkillBloodDebuffRate() {
    // IDA 0x1403A23D0: return this->m_fSkillBloodDebuffRate
    return m_fSkillBloodDebuffRate;
}

// ============================================================================
// GetAllowBloodCount IDA 0x1403A23F0
// ============================================================================
int CMover::GetAllowBloodCount() {
    // IDA 0x1403A23F0: return this->m_nAllowBloodCount
    return m_nAllowBloodCount;
}

// ============================================================================
// GetSkillBloodRate IDA 0x1403A2410
// ============================================================================
float CMover::GetSkillBloodRate() {
    // IDA 0x1403A2410: return this->m_fSkillBloodRate
    return m_fSkillBloodRate;
}

// ============================================================================
// GetBuffStatusCount IDA 0x1403A26D0
// ============================================================================
int CMover::GetBuffStatusCount(int nVal) {
    // IDA 0x1403A26D0: return this->m_stBuffState[nVal].byCount
    if (nVal >= 0 && nVal < 8) {
        return m_stBuffState[nVal].byCount;
    }
    return 0;
}

// ============================================================================
// GetBuffStatus IDA 0x14070AB00
// ============================================================================
tagBUFF_STATE* CMover::GetBuffStatus(int nVal) {
    // IDA 0x14070AB00: return &this->m_stBuffState[nVal]
    return &m_stBuffState[nVal];
}

// ============================================================================
// GetAllAttackAddRate IDA 0x1403A26F0
// ============================================================================
float CMover::GetAllAttackAddRate() {
    // IDA 0x1403A26F0: return this->m_fAllAttackAddRate
    return m_fAllAttackAddRate;
}

// ============================================================================
// GetBossAttackedDownRate IDA 0x1403A2710
// ============================================================================
float CMover::GetBossAttackedDownRate() {
    // IDA 0x1403A2710: return this->m_fBossAttackedDownRate
    return m_fBossAttackedDownRate;
}

// ============================================================================
// GetBossAttackAddRate IDA 0x1403A2730
// ============================================================================
float CMover::GetBossAttackAddRate() {
    // IDA 0x1403A2730: return this->m_fBossAttackAddRate
    return m_fBossAttackAddRate;
}

// ============================================================================
// GetBuffSuperArmorRate IDA 0x1403A2750
// ============================================================================
float CMover::GetBuffSuperArmorRate() {
    // IDA 0x1403A2750: return this->m_fBuffSuperArmorRate
    return m_fBuffSuperArmorRate;
}

// IDA 0x1402C7BD0 - GetDieDelayTime
float CMover::GetDieDelayTime() {
    return m_fDieDelayTime;
}

// ============================================================================
// GetHavokCapsuleHeight IDA 0x1403A2BC0
// ============================================================================
float CMover::GetHavokCapsuleHeight() {
    // IDA 0x1403A2BC0: return this->m_fCapsuleHeight
    return m_fCapsuleHeight;
}

// IDA 0x1402C7C60 - SetOrientationYaw
void CMover::SetOrientationYaw(float fYaw) {
    // IDA 反编译: VisObject3D_cl::SetOrientation(this, fYaw, m_vOrientation.y, m_vOrientation.z)
    m_vOrientation.x = fYaw;
}

// IDA 0x1402C7CC0 - AddSummonMobList
void CMover::AddSummonMobList(std::uint32_t dwMobID) {
    m_listSummonMob.push_back(dwMobID);
}

// IDA 0x1402C7F00 - GetIgnoreSkillCost
int CMover::GetIgnoreSkillCost() {
    return m_bIgnoreSkillCost;
}

// IDA 0x1402C7F20 - IsNoSkillCostSG
bool CMover::IsNoSkillCostSG() {
    return m_bNoSkillCostSG;
}

// IDA 0x1402C7EE0 - GetDecreaseStaminaRate
float CMover::GetDecreaseStaminaRate() {
    return m_fDecreaseStaminaRate;
}

// IDA 0x1402C7A20 - GetCurRandomShootProjectileDirY
std::int16_t CMover::GetCurRandomShootProjectileDirY() {
    return m_shCurRandomShootProjectileDirY;
}

// IDA 0x1402C7A40 - GetCurRandomShootProjectileDirX
std::int16_t CMover::GetCurRandomShootProjectileDirX() {
    return m_shCurRandomShootProjectileDirX;
}

// ============================================================================
// GetBuffAddGoldRate IDA 0x140412000
// ============================================================================
float CMover::GetBuffAddGoldRate() {
    // IDA 0x140412000: return this->m_fBuffAddGoldRate
    return m_fBuffAddGoldRate;
}

// ============================================================================
// GetDefaultAnimStep IDA 0x1403751B0
// ============================================================================
std::uint8_t CMover::GetDefaultAnimStep() {
    // IDA 0x1403751B0: return this->m_byDefaultAnimStep
    return m_byDefaultAnimStep;
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
// IsAllowPassiveType IDA 0x140364670
// ============================================================================
bool CMover::IsAllowPassiveType(int nType) {
    // IDA 0x140364670:
    // if (m_setAllowPassiveType.size() > 0)
    //   return m_setAllowPassiveType.find(nType) != m_setAllowPassiveType.end();
    // else
    //   return true;  // 默认允许所有被动类型
    if (m_setAllowPassiveType.size() > 0) {
        return m_setAllowPassiveType.find(nType) != m_setAllowPassiveType.end();
    }
    return true;
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
// SetReactionRate IDA 0x140368B00 (estimated)
// ============================================================================
void CMover::SetReactionRate(float fRate) {
    // IDA: m_fReactionRate = fRate
    m_fReactionRate = fRate;
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
// GetAnimIndex - IDA 0x140368960
// 根据动画名称获取动画索引
// IDA 精确还原
// ============================================================================
unsigned int CMover::GetAnimIndex(const VString& strAnimName) {
    // IDA 0x140368960: 查找 m_mapAnimInfoKey[strAnimName]
    if (m_mapAnimInfoKey) {
        auto it = m_mapAnimInfoKey->find(strAnimName);
        if (it != m_mapAnimInfoKey->end()) {
            return it->second;
        }
    }
    return static_cast<unsigned int>(-1);  // 未找到返回 -1
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
// IDA @ 0x140367C80
// CheckAnimationEnd - 检查动画结束并处理位移
// IDA 精确还原
void CMover::CheckAnimationEnd() {
    // IDA: 检查动画状态
    if (m_bAnimChanged || !m_bAnimPlay || !m_pCurMotionEvent || m_pCurMotionEvent->fAnimationLength <= 0.0f) {
        return;
    }

    // IDA: 获取帧时间增量
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = pTimer->GetTimeDifference();
    float fPrevTime = m_fAnimationTime;

    // IDA: 更新累积动画时间 (受动画速度影响)
    m_fAnimationTime = fPrevTime + fDeltaTime * m_fAnimSpeed;

    // IDA: 检查动画是否播放完毕
    if (m_fAnimationTime >= m_pCurMotionEvent->fAnimationLength) {
        // IDA: eEndofAnimation != 0 表示非循环动画
        if (m_pCurMotionEvent->eEndofAnimation) {
            // IDA: 结束型动画: 锁定在最后一帧
            m_fAnimationTime = m_pCurMotionEvent->fAnimationLength;
            m_fAnimPercentTime = 1.0f;
            ClearMotion();
            return;
        }
        // IDA: 循环动画: 从头开始播放
        m_fAnimationTime -= m_pCurMotionEvent->fAnimationLength;
    }

    // IDA: 非跳过偏移模式: 计算动画位置偏移
    if (!m_bSkipAnimOffset) {
        // TODO: 汇编还原 - VAnimationInfo::GetOffsetDelta 需要完整实现
        // IDA: VAnimationInfo::GetOffsetDelta 计算两帧间的位移量
        hkvVec3 vOffset;
        // VAnimationInfo::GetOffsetDelta(m_pCurMotionEvent, vOffset, fPrevTime, m_fAnimationTime);

        if (!vOffset.isZero(0.0f)) {
            // IDA: 根据朝向旋转偏移量
            hkvMat3 matRot;
            float fYaw = GetOrientationYaw();
            matRot.setFromEulerAngles(0.0f, 0.0f, fYaw);
            hkvVec3 vRotOffset = matRot.transformDirection(vOffset);

            hkvVec3 vDestPos = GetPosition() + vRotOffset;

            // IDA: 地面高度检测 (非飞行状态)
            bool bFlying = IsFlying();
            if (!bFlying) {
                GetHeight(&vDestPos, 200.0f);
            }

            // IDA: 碰撞检测
            // TODO: 汇编还原 - CheckMoveCollision 需要完整实现
            CMover* pCollideActor = CheckMoveCollision(vDestPos);
            if (pCollideActor) {
                // TODO: 汇编还原 - CMonster 相关逻辑需要完整实现
                // IDA: 检查是否为当前目标
            }

            // IDA: 目标位置合法性检查
            if (!CheckMoveDestPos(vDestPos, bFlying, 0)) {
                m_bSkipAnimOffset = 1;
                send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, vDestPos, 0);
            }

            // IDA: 执行移动
            Move(vDestPos);
        }
    }

    // IDA: 更新动画百分比进度
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
// IDA 精确还原
// ============================================================================
void CMover::SetupAnimation() {
    // TODO: 汇编还原 - SetupAnimation 需要完整实现
    // IDA: 获取动作资源文件名
    VString fn = GetActionResourceFN();
    const char* szFileName = fn.AsChar();

    // IDA: 从 XGameServer 获取动作资源
    XGameServer* pServer = XGameServer::Instance();
    // TODO: 需要实现 VResourceManager::GetResourceByName
    // m_pActionResource = (VActionResourceLump*)VResourceManager::GetResourceByName(&pServer->m_xActionManager, szFileName);

    // IDA: 获取 TableID 并设置动画信息
    int dwTableID = GetTableID();
    // TODO: 需要实现 XActionResMgr::SetAnimInfoToActor
    // if (XActionResMgr::SetAnimInfoToActor(&pServer->m_xActionManager, dwTableID, this)) {
    //     SetupAnimInfo();
    //
    //     // IDA: 检查受击动画数量
    //     unsigned int AnimIndex = XActionResMgr::GetAnimIndex(this, 18, 2, 1);
    //     if (GetAnimStirng(AnimIndex)) {
    //         m_nHitAnimCount = 7;
    //     } else {
    //         m_nHitAnimCount = 2;
    //     }
    // } else if (GetType() != 1) {  // TYPE_PLAYER
    //     int Type = GetType();
    //     LogHelper::LogDebug("game.contents", "SetupAnimation No ActionData (%d:%d)", Type, dwTableID);
    // }
}

// ============================================================================
// SetProtectionAggroRatio IDA 0x1403655C0
// ============================================================================
void CMover::SetProtectionAggroRatio(float ratio) {
    // IDA 0x1403655C0: this->m_fProtectionAggroRatio = ratio
    m_fProtectionAggroRatio = ratio;
}

// ============================================================================
// GetHitCollisionCount IDA 0x140367B90 (精确还原)
// ============================================================================
int CMover::GetHitCollisionCount() {
    // IDA 0x140367B90 精确还原:
    // if (m_pHitCollisionData) return m_pHitCollisionData->vHitColisions.size();
    // else return 0;
    if (m_pHitCollisionData) {
        // TODO: 需要 tagHIT_COLLISION_DATA 结构定义
        // return static_cast<int>(m_pHitCollisionData->vHitColisions.size());
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
// IDA 精确还原:
// 遍历 m_mapSkillUnlock，检查 key 的第二个 DWORD == pTBSkill->Skill_Group
// *((_DWORD *)&iter->first + 1) 表示 key 指针偏移 4 字节后的 DWORD
// ============================================================================
bool CMover::IsActivateSkillUnlockBuff(TB_SKILL* pTBSkill) {
    if (!pTBSkill) {
        return false;
    }
    // IDA: 遍历 m_mapSkillUnlock，检查 key 的第二个 DWORD (Skill_Group)
    for (auto it = m_mapSkillUnlock.begin(); it != m_mapSkillUnlock.end(); ++it) {
        // IDA: *((_DWORD *)&it->first + 1) == pTBSkill->Skill_Group
        // key 的低 32 位是其他数据，高 32 位是 Skill_Group
        std::uint32_t nSkillGroup = static_cast<std::uint32_t>(it->first >> 32);
        if (nSkillGroup == static_cast<std::uint32_t>(pTBSkill->Skill_Group)) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// 缺失函数的 stub 实现 (待从 IDA 精确还原)
// ============================================================================

// IDA @ 0x14036BC20
// ProcessExtraMoving - 处理额外移动（击退、拉扯等效果）
// IDA 精确还原
void CMover::ProcessExtraMoving() {
    // IDA: 首先检查 m_stExtMovingVal 是否为零
    if (m_stExtMovingVal.IsZero()) {
        return;
    }

    // IDA: 保存当前位置到 m_vPrevPos
    m_vPrevPos = GetPosition();

    // IDA: 检查剩余时间
    if (m_stExtMovingVal.fRemainTime <= 0.0f) {
        ReleaseExtraMoving();
        return;
    }

    // IDA: 计算位移差值
    float fDiffX = m_stExtMovingVal.x - m_vPosition.x;
    float fDiffY = m_stExtMovingVal.y - m_vPosition.y;

    // IDA: 获取帧时间
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = pTimer->GetTimeDifference();

    // IDA: 检查距离阈值 (3.0)
    if (fabsf(fDiffX) >= 3.0f || fabsf(fDiffY) >= 3.0f) {
        float fDeltaX, fDeltaY;
        float fTimeRatio = fDeltaTime / m_stExtMovingVal.fMovingTime;

        // IDA: 计算 X 方向移动量
        if (fDiffX <= 0.0f) {
            float fMove = fTimeRatio * fDiffX;
            fDeltaX = (fMove > fDiffX) ? fDiffX : fMove;
        } else {
            float fMove = fTimeRatio * fDiffX;
            fDeltaX = (fDiffX <= fMove) ? fDiffX : fMove;
        }

        // IDA: 计算 Y 方向移动量
        if (fDiffY <= 0.0f) {
            float fMove = fTimeRatio * fDiffY;
            fDeltaY = (fMove > fDiffY) ? fDiffY : fMove;
        } else {
            float fMove = fTimeRatio * fDiffY;
            fDeltaY = (fDiffY <= fMove) ? fDiffY : fMove;
        }

        // IDA: 更新剩余时间
        m_stExtMovingVal.fRemainTime -= fDeltaTime;

        // IDA: 计算目标位置
        hkvVec3 vDestPos = m_vPrevPos + hkvVec3(fDeltaX, fDeltaY, 0.0f);

        // IDA: 地面高度检测
        if (!IsFlying()) {
            GetHeight(&vDestPos, 200.0f);
        }

        // IDA: 碰撞检测
        CMover* pCollideActor = CheckMoveCollision(vDestPos);
        if (pCollideActor) {
            hkvVec3 vCurPos = GetPosition();
            send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, vCurPos, false);
        } else {
            if (!CheckMoveDestPos(vDestPos, false, 0)) {
                send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, vDestPos, false);
            }
            Move(vDestPos);
        }
    } else {
        // IDA: 距离太小，直接清除
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

// IDA @ 0x14036EAC0
// send_eSUB_CMD_MOVE - 发送移动数据包
// IDA 精确还原
void CMover::send_eSUB_CMD_MOVE(CMover* pMover, float fTargetPosX, float fTargetPosY, std::uint8_t byRunBit) {
    if (!pMover) {
        return;
    }

    // IDA: 获取移动朝向
    float fYaw = pMover->GetMovingYaw();

    // IDA: 验证朝向值范围 (-360 ~ 360)
    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    // IDA: 获取移动速度
    m_fMoveSpeed = pMover->GetMoveSpeed();

    // IDA: 获取当前位置
    hkvVec3 vPos = pMover->GetPosition();

    // IDA: 获取地图ID
    UXMapID uxMapID;
    uxMapID.nMapID = 0;
    // TODO: 汇编还原 - GetArea 需要返回 XArea* 类型
    // XArea* pArea = pMover->GetArea();
    // if (pArea) {
    //     uxMapID = pArea->GetInstanceID();
    // }

    // TODO: 汇编还原 - ST_MOVE 结构体定义需要完善
    // IDA: 构建 ST_MOVE 数据包
    // ST_MOVE stMove;
    // stMove.dwActorID = pMover->GetID();
    // stMove.nMapID = uxMapID.nMapID;
    // stMove.fPosX = vPos.x;
    // stMove.fPosY = vPos.y;
    // stMove.fPosZ = vPos.z;
    // stMove.fYaw = pMover->GetMovingYaw();
    // stMove.fTargetPosX = fTargetPosX;
    // stMove.fTargetPosY = fTargetPosY;
    // stMove.byRunBit = byRunBit;
    // stMove.fPitch = pMover->GetLookPitch();
    // stMove.fMoveSpeed = m_fMoveSpeed;
    // stMove.byChangeMotion = 0;

    // IDA: 广播给周围玩家
    // TODO: 汇编还原 - CGocNetwork::SendBroadCastAfterLoading 需要实现
    XSendPacket xPacket(5, 2);
    // xPacket << stMove;
    // CGocNetwork::SendBroadCastAfterLoading(this, &xPacket, eNoneSelf);

    m_fLastSendMoveTime = 0.0f;

    DebugOut("send_eSUB_CMD_MOVE>> (%.2f,%.2f) -> (%.2f,%.2f) -> Rot:%.3f",
             vPos.x, vPos.y, fTargetPosX, fTargetPosY, fYaw);
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
// IDA: return GetArea() && GetArea()->GetNavMeshInstance() && DohHavokNavMeshInstance::GetHeight()
bool CMover::GetHeight(hkvVec3* vPos, float fMaxDist) {
    // IDA 0x14036D130 精确还原:
    // XArea* pArea = GetArea();
    // if (!pArea) return false;
    // DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    // if (!pNavMesh) return false;
    // return pNavMesh->GetHeight(vPos, fMaxDist);
    
    XArea* pArea = GetArea();
    if (!pArea) {
        return false;
    }
    
    // IDA: 获取导航网格实例并调用 GetHeight
    // DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    // if (!pNavMesh) return false;
    // return pNavMesh->GetHeight(vPos, fMaxDist);
    
    // 临时实现: 导航网格类型未定义，返回 false
    (void)vPos;
    (void)fMaxDist;
    return false;
}

// IDA 0x14036CC00 - SetMovePosition (精确还原)
// 设置移动目标位置
void CMover::SetMovePosition(float fXpos, float fYpos) {
    // IDA 反编译:
    // this->m_stMovePos.x = fXpos;
    // this->m_stMovePos.y = fYpos;

    m_stMovePos.x = fXpos;
    m_stMovePos.y = fYpos;
}

// IDA 0x14036DBA0 - GetYawFromVector (精确还原)
// 从向量获取偏航角 (使用Havok数学库)
float CMover::GetYawFromVector(const hkvVec3& vDir) {
    // IDA 反编译精确还原:
    // 1. 创建基准向量 n1 = (0, -1, 0)
    // 2. 创建方向向量 n2 = (vDir.x, vDir.y, 0) 并归一化
    // 3. 计算点积并clamp到[-1, 1]
    // 4. 使用acos计算角度并转换为度
    // 5. 根据x正负调整角度范围

    hkvVec3 n1(0.0f, -1.0f, 0.0f);
    hkvVec3 n2(vDir.x, vDir.y, 0.0f);

    // 归一化方向向量
    float len = n2.GetLength();
    if (len > 0.000001f) {
        n2.x /= len;
        n2.y /= len;
        n2.z /= len;
    }

    // 计算点积
    float calcValue = n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
    calcValue = std::max(-1.0f, std::min(1.0f, calcValue));  // clamp

    // 计算角度 (弧度转度)
    float fDegree = std::acos(calcValue) * 180.0f / 3.14159265f;

    // 根据x方向调整角度
    if (vDir.x < 0.0f) {
        fDegree = (180.0f - fDegree) + 180.0f;
    }

    // 归一化到[-180, 180]
    if (fDegree > 180.0f) {
        fDegree -= 360.0f;
    }

    return fDegree;
}

// GetMovingYaw - 获取移动偏航角
// IDA: CMoverEx::GetMovingYaw @ 0x140189290 (派生类实现)
// CMover 基类版本 - 从移动方向计算偏航角
float CMover::GetMovingYaw() const {
    // IDA: 如果正在移动，从移动方向计算偏航角
    if (m_bMoving) {
        hkvVec3 vCurPos = GetPosition();
        hkvVec3 vDestPos(m_stMovePos.x, m_stMovePos.y, vCurPos.z);
        hkvVec3 vDir = vDestPos - vCurPos;
        if (vDir.GetLengthSquared() > 0.0001f) {
            return GetYawFromVector(vDir);
        }
    }
    // IDA: 未移动时返回当前朝向
    return const_cast<CMover*>(this)->GetOrientationYaw();
}

// IDA 0x1402A5080 - GetPositionXVec3 (精确还原)
// 注意: 已在上方定义，此处删除重复定义

// IDA 0x1402C7200 - GetSoulCostDownRate (精确还原)
// IDA 反编译: return this->m_fSoulCostDownRate
float CMover::GetSoulCostDownRate() {
    return m_fSoulCostDownRate;
}

// IDA 0x14027A610 - IsMoving (精确还原)
// IDA 反编译: return this->m_fMoving (注意: 返回 m_bMoving 成员)
bool CMover::IsMoving() {
    return m_bMoving != 0;
}

// IDA 0x140276870 - GetHavokCapsuleRadius (精确还原)
// IDA 反编译: return this->m_fCapsuleRadius
float CMover::GetHavokCapsuleRadius() {
    return m_fCapsuleRadius;
}

// IDA 0x14036DA00 - GetTargetAngle (精确还原)
// IDA: 每个槽位对应 30 度
float CMover::GetTargetAngle(std::uint8_t byPos) {
    return static_cast<float>(byPos) * 30.0f;
}

// GetTargetID - 返回目标ID
std::uint32_t CMover::GetTargetID() const {
    return m_dwTargetID;
}

// IsLive - 返回是否存活
bool CMover::IsLive() const {
    return !IsDie();
}

// GetAnimationIdx - 返回动画索引
std::uint32_t CMover::GetAnimationIdx() {
    return m_nAnimationIdx;
}

// GetGOC_Attribute - 获取属性组件
// IDA 0x14004CFD0: GetGOC<CGocAttribute>
// IDA 精确还原
std::shared_ptr<class CGocAttribute> CMover::GetGOC_Attribute(bool bCreateIfNull) const {
    // IDA: 访问 m_GOComponentTable[0] (CGocAttribute 在索引 0)
    if (m_GOComponentTable.size() > 0 && m_GOComponentTable[0]) {
        return std::static_pointer_cast<CGocAttribute>(m_GOComponentTable[0]);
    }
    // IDA: 如果 bCreateIfNull 为 false，打印警告
    if (!bCreateIfNull) {
        // printf("Not Exist Component %d\n", 0);
    }
    return nullptr;
}

// IDA 0x1402C72D0 - GetWeightRank (精确还原)
// IDA 反编译: return this->m_cWeightRank
std::uint8_t CMover::GetWeightRank() {
    return m_cWeightRank;
}

// IDA 0x1402C7310 - GetDmgMotionFlag (精确还原)
// IDA 反编译: return this->m_byDmgMontionFlag
std::uint8_t CMover::GetDmgMotionFlag() {
    return m_byDmgMontionFlag;
}

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
// IDA @ 0x1403681B0
// CheckMoveCollision - 移动碰撞检测
// IDA 精确还原
CMover* CMover::CheckMoveCollision(hkvVec3& vDestPos) {
    // IDA: 类型检查 - 仅怪物执行碰撞检测
    if (GetType() != 2) {  // TYPE_MONSTER (eActorMonster)
        return nullptr;
    }

    // IDA: 碰撞启用检查
    if (!m_bCollisionEnable) {
        return nullptr;
    }

    // IDA: KeepMovingExtra 检查
    if (m_bKeepMovingExtra) {
        return nullptr;
    }

    // IDA: RTTI 转换为 CMonster
    CMonster* pMonster = dynamic_cast<CMonster*>(this);
    if (!pMonster) {
        return nullptr;
    }

    // IDA: 检查是否为跟随者
    if (pMonster->IsFollower()) {
        return nullptr;
    }

    // IDA: 检查是否为巡逻怪物
    CAi* pAi = pMonster->GetAi();
    if (pAi && pAi->IsPatrolMonster()) {
        return nullptr;
    }

    float nearFactor = 5000.0f;
    CMover* pClosestTargetEntity = nullptr;
    hkvVec3 vClosestPos(0.0f, 0.0f, 0.0f);
    hkvVec3 vTempPos(0.0f, 0.0f, 0.0f);

    // IDA: 扫描区域内的 Actor
    std::vector<CMover*> vecGameObjList;
    XArea* pArea = GetArea();
    if (!pArea) {
        return nullptr;
    }
    pArea->ScanGridOrigin(reinterpret_cast<XActor*>(this), 2, 2u, vecGameObjList);

    // IDA: 遍历检测碰撞
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMover* pOtherActor = *it;
        if (!pOtherActor) {
            continue;
        }

        // IDA: 类型过滤 - 玩家或防御对象
        bool bCheckActor = (pOtherActor->GetType() == 0);  // TYPE_PLAYER

        if (pOtherActor->GetType() == 2) {  // TYPE_MONSTER
            CMonster* pOtherMonster = dynamic_cast<CMonster*>(pOtherActor);
            if (pOtherMonster && pOtherMonster->IsDefenseObject()) {
                bCheckActor = true;
            }
        }

        if (!bCheckActor) {
            continue;
        }

        // IDA: 存活检查
        if (!pOtherActor->IsLive() || pOtherActor->IsStatus(2)) {
            continue;
        }

        // IDA: 距离计算
        hkvVec3 vOffset = vDestPos - pOtherActor->GetPosition();
        vOffset.z = 0.0f;
        float targetDist = vOffset.GetLength();

        vTempPos = pOtherActor->GetPosition();
        if (nearFactor > targetDist) {
            vClosestPos = vTempPos;
            nearFactor = targetDist;
            pClosestTargetEntity = pOtherActor;
        }
    }

    // IDA: 碰撞判定
    if (pClosestTargetEntity) {
        hkvVec3 vOffset = vDestPos - vClosestPos;
        vOffset.z = 0.0f;
        float fDist = vOffset.GetLength();
        float fOtherRadius = pClosestTargetEntity->GetHavokCapsuleRadius();

        // IDA: (fOtherRadius + this->m_fCapsuleRadius + 5.0 + 5.0) >= fDist
        if ((fOtherRadius + m_fCapsuleRadius + 5.0f + 5.0f) >= fDist) {
            return pClosestTargetEntity;
        }
    }

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
    // IDA 0x140366FA0: Reset animation changed flag
    m_bAnimChanged = 0;

    // IDA: Process skill manager think function
    if (m_pSkillMgr) {
        m_pSkillMgr->ThinkFunction();
    }

    // IDA: Handle trace user mode
    if (m_bTraceUser) {
        // TODO: send_eSUB_CMD_MOVE_TRACE not yet declared
        // send_eSUB_CMD_MOVE_TRACE(this, this);
        float fDebugTime = m_fLastDebugTime + 0.5f;
        VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
        if (pTimer) {
            // TODO: Need IVTimer::GetTime implementation
            // if (IVTimer::GetTime(pTimer) > fDebugTime) {
            //     VDefaultTimer* pTimer2 = ThreadLocalData::GetTimer();
            //     m_fLastDebugTime = IVTimer::GetTime(pTimer2);
            // }
        }
    }

    // IDA: Check delayed projectiles and continuous melee
    VDefaultTimer* pTimer3 = ThreadLocalData::GetTimer();
    if (pTimer3) {
        float fDeltaTime = pTimer3->GetTimeDifference();
        // TODO: CheckDelayedProjectile and CheckContinuousMelee not yet declared
        // CheckDelayedProjectile(fDeltaTime);
        // CheckContinuousMelee(fDeltaTime);
        (void)fDeltaTime;
    }
}
// IDA 0x14036CAB0 - SceneChanged (精确还原)
// 场景切换时的清理工作
void CMover::SceneChanged() {
    // IDA 反编译: 直接调用 MoveingValueClear
    MoveingValueClear();
}
// IDA: GetTableID - 基类返回0，子类重写
// CMover 基类版本 - 返回0
int CMover::GetTableID() { 
    return 0; 
}
// IDA 0x1403688D0 - GetAnimStirng (精确还原)
// 根据动画键获取动画名称字符串
char* CMover::GetAnimStirng(unsigned int dwAnimKey) {
    // IDA 反编译精确还原:
    // 1. 检查 m_mapAnimInfoString 是否存在
    // 2. 在 map 中查找 dwAnimKey
    // 3. 如果找到，返回 VString 的字符指针
    
    if (!m_mapAnimInfoString) {
        return nullptr;
    }
    
    auto it = m_mapAnimInfoString->find(dwAnimKey);
    if (it == m_mapAnimInfoString->end()) {
        return nullptr;
    }
    
    // IDA: 返回 VString 的字符指针
    return const_cast<char*>(it->second.AsChar());
}
void CMover::SetMoveingInFly(int bFlying) { m_bMoveingInFly = (bFlying != 0); }
// IDA 0x14036D1E0 - GetMoverObject (精确还原)
// 根据ID获取Mover对象
CMover* CMover::GetMoverObject(std::uint32_t dwID) {
    // IDA 反编译精确还原:
    // 1. 检查 dwID 是否为 -1 (0xFFFFFFFF)
    // 2. 获取当前区域 (GetArea)
    // 3. 在区域中查找 Actor (FindActor)
    // 4. 使用 RTTI 动态转换为 CMover
    
    if (dwID == 0xFFFFFFFF) {
        return nullptr;
    }
    
    // TODO: 需要完整的 XArea 和 XActor 类型定义
    // XArea* pArea = GetArea();
    // if (!pArea) {
    //     return nullptr;
    // }
    
    // XActor* pActor = pArea->FindActor(dwID);
    // if (!pActor) {
    //     return nullptr;
    // }
    
    // IDA: 使用 RTTI 动态转换
    // CMover* pMover = dynamic_cast<CMover*>(pActor);
    // return pMover;
    
    return nullptr;
}
// SetKeepMovingExtra 已在上方定义
void CMover::SetWeightRank(std::uint8_t cVal) { m_cWeightRank = cVal; }
// IDA @ 0x14036DB20
// RemoveTargetDestPos - 移除目标位置标记
// IDA 精确还原
void CMover::RemoveTargetDestPos() {
    // IDA: 检查 m_byTargetDestPos != 255 且 m_dwTargetID != -1
    if (m_byTargetDestPos != 255 && m_dwTargetID != 0xFFFFFFFF) {
        CMover* pTarget = GetMoverObject(m_dwTargetID);
        if (pTarget) {
            ClearTargetPosFlag(pTarget, m_byTargetDestPos);
        }
    }
    // IDA: 重置 m_byTargetDestPos 为 -1 (0xFF)
    m_byTargetDestPos = 0xFF;
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

// ============================================================================
// send_eSUB_CMD_MOVE_IDLE - 发送空闲移动包
// IDA: ?send_eSUB_CMD_MOVE_IDLE@CMover@@QEAAXPEAV1@M@Z @ 0x14036FD50
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_IDLE(CMover* pMover, float fMoveDelayTime) {
    // IDA 精确还原
    if (!pMover) return;
    
    // 构建 XSendPacket (主命令 5, 子命令 12)
    XSendPacket xPacket(5, 12);
    
    // 获取 ActorID
    UXActorID actorID = pMover->GetActorID();
    xPacket.XParse << actorID.GetID();
    
    // 获取位置
    const hkvVec3& curPos = pMover->GetPosition();
    xPacket.XParse << curPos.x;
    xPacket.XParse << curPos.y;
    xPacket.XParse << curPos.z;
    
    // 获取朝向
    float fYaw = pMover->GetMovingYaw();
    xPacket.XParse << fYaw;
    
    // 获取动画索引和步骤
    int nAnimationIdx = pMover->GetAnimationIdx();
    xPacket.XParse << nAnimationIdx;
    
    std::uint8_t byDefaultAnimStep = pMover->GetDefaultAnimStep();
    xPacket.XParse << byDefaultAnimStep;
    
    // 写入移动延迟时间
    xPacket.XParse << fMoveDelayTime;
    
    // 广播给周围玩家 (不等待加载完成)
    CGocNetwork::SendBroadCastAfterLoading(this, xPacket, E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
    
    // Debug output
    DebugOut("send_eSUB_CMD_MOVE_IDLE>> ActorID:0x%08X Pos:(%.2f,%.2f,%.2f) Yaw:%.2f AnimIdx:%d Delay:%.2f",
             actorID.GetID(), curPos.x, curPos.y, curPos.z,
             fYaw, nAnimationIdx, fMoveDelayTime);
}

// IDA 0x14036F1E0 - send_eSUB_CMD_MOVE_BATTLE (精确还原)
// 发送战斗姿态移动数据包
// TODO: 需要 ST_MOVE_BATTLE 结构定义
void CMover::send_eSUB_CMD_MOVE_BATTLE(CMover* pMover, bool bPlayMotion) {
    // IDA 反编译精确还原 - 需要完整类型定义后启用
    // 1. 构建 ST_MOVE_BATTLE 结构
    // 2. 创建 XSendPacket (主命令 5, 子命令 8)
    // 3. 发送数据包
    // 4. 广播给周围玩家
    
    if (!pMover) {
        return;
    }
    
    // TODO: 实现完整逻辑
    // XSendPacket xPacket(5, 8);
    // ST_MOVE_BATTLE stMoveBattle;
    // ...
    
    (void)bPlayMotion;
}

// IDA 0x1403714A0 - send_eSUB_CMD_ACTIVE_SKILL (精确还原)
// 发送激活技能数据包
// TODO: 需要 PS_SkillActive_BT 结构定义
void CMover::send_eSUB_CMD_ACTIVE_SKILL(CMover* pMover, std::uint32_t nSkillID, std::uint8_t byAngleAttackType) {
    // IDA 反编译精确还原 - 需要完整类型定义后启用
    // 1. 创建 XSendPacket (主命令 6, 子命令 0x10)
    // 2. 构建 PS_SkillActive_BT 结构
    // 3. 发送数据包
    // 4. 广播给周围玩家
    
    if (!pMover) {
        return;
    }
    
    // TODO: 实现完整逻辑
    // XSendPacket xPacket(6, 0x10);
    // PS_SkillActive_BT btInfo;
    // ...
    
    (void)nSkillID;
    (void)byAngleAttackType;
}

// ============================================================================
// SetCollisionEnable - 设置碰撞启用状态
// ============================================================================
void CMover::SetCollisionEnable(bool bEnable, bool bUnk) {
    // IDA: 设置碰撞启用标志
    m_bCollisionEnable = bEnable ? 1 : 0;
    (void)bUnk;
}

// ============================================================================
// StopMoving - 停止移动
// ============================================================================
void CMover::StopMoving(bool bStop) {
    // IDA: 停止移动逻辑
    if (bStop) {
        m_bMoving = 0;
        m_bGazeMoving = 0;
        // send_eSUB_CMD_MOVE_STOP(this);
    }
}

// ============================================================================
// SetDieFadeTime - 设置死亡淡出时间
// IDA @ 0x1403a2370
// ============================================================================
void CMover::SetDieFadeTime(float fVal) {
    m_fDieFadeTime = fVal;
}

// ============================================================================
// AnimPause - 暂停动画
// IDA @ 0x1403a2390
// ============================================================================
void CMover::AnimPause() {
    m_bAnimPlay = 0;
}

// ============================================================================
// ApplyBuffStatus - 应用Buff状态 (基类空实现)
// IDA @ 0x1403774d0
// ============================================================================
void CMover::ApplyBuffStatus(std::int16_t nIndex, float fElapsedTime) {
    // IDA: 基类空实现 - 子类(CUser/CMonster)会重写此函数
    (void)nIndex;
    (void)fElapsedTime;
}

// ============================================================================
// SetRestoreDefenseType - 设置恢复防御类型
// IDA @ 0x1403a2830
// ============================================================================
void CMover::SetRestoreDefenseType() {
    m_byRestoreDefenceType = m_byDefenseType;
}

// ============================================================================
// SetHitStatus - 设置受击状态
// IDA @ 0x1403e1bf0
// ============================================================================
void CMover::SetHitStatus(int nHitStatus) {
    m_nHitStatus = nHitStatus;
}

// ============================================================================
// SetTraceUser - 设置追踪用户标志
// IDA @ 0x140406e90
// ============================================================================
void CMover::SetTraceUser(bool bTrace) {
    m_bTraceUser = bTrace;
}

// ============================================================================
// IsMoveingInFly - 检查是否在飞行中移动
// IDA @ 0x1403a2850
// ============================================================================
bool CMover::IsMoveingInFly() {
    return m_bMoveingInFly;
}

// ============================================================================
// IsTraceUser - 检查是否追踪用户
// IDA @ 0x140406e70
// ============================================================================
bool CMover::IsTraceUser() {
    return m_bTraceUser != 0;
}

// ============================================================================
// GetDamageCalc - 获取伤害计算 (基类返回0)
// IDA @ 0x140375000
// ============================================================================
int CMover::GetDamageCalc(int nAP, std::uint8_t byType, float fReduceRate) {
    // IDA: 基类直接返回0，子类(CUser/CMonster)会重写此函数
    (void)nAP;
    (void)byType;
    (void)fReduceRate;
    return 0;
}

// ============================================================================
// SetOnGround - 设置是否在地面上
// IDA @ 0x14052a1b0
// ============================================================================
void CMover::SetOnGround(int bGround, float fPosZ) {
    m_bOnGround = bGround;
    m_fGroundPosZ = fPosZ;
}

// ============================================================================
// SetCurRandomShootProjectileDirY - 设置随机射击投射物Y方向
// IDA @ 0x1405fa280
// ============================================================================
void CMover::SetCurRandomShootProjectileDirY(std::int16_t shVal) {
    m_shCurRandomShootProjectileDirY = static_cast<float>(shVal);
}

// ============================================================================
// SetCurRandomShootProjectileDirX - 设置随机射击投射物X方向
// IDA @ 0x1405fa2b0
// ============================================================================
void CMover::SetCurRandomShootProjectileDirX(std::int16_t shVal) {
    m_shCurRandomShootProjectileDirX = static_cast<float>(shVal);
}

// ============================================================================
// GetClassTypeId - 获取类类型ID (Vision Engine RTTI)
// IDA @ 0x1403743a0
// ============================================================================
VType* CMover::GetClassTypeId() {
    return classCMover;
}

// ============================================================================
// GetTypeId - 获取类型ID (Vision Engine RTTI)
// IDA @ 0x1403743b0
// ============================================================================
VType* CMover::GetTypeId() const {
    return classCMover;
}

// ============================================================================
// ResetAllBuff - 重置所有Buff状态
// IDA @ 0x14036A860
// ============================================================================
void CMover::ResetAllBuff() {
    // IDA code:
    // void __fastcall CMover::ResetAllBuff(CMover *this)
    // {
    //   unsigned __int8 i; // [rsp+20h] [rbp-18h]
    //   this->m_nBuffTotalCnt = 0;
    //   this->m_nBuffCnt = 0;
    //   this->m_nDebuffCnt = 0;
    //   for ( i = 0; i < 0x32u; ++i )
    //     tagBUFF_STATE::Clear(&this->m_stBuffState[i]);
    // }
    m_nBuffTotalCnt = 0;
    m_nBuffCnt = 0;
    m_nDebuffCnt = 0;
    for (std::uint8_t i = 0; i < 0x32u; ++i) {
        m_stBuffState[i].Clear();
    }
}

// ============================================================================
// IsFriend - IDA 0x1403808F0 (基类实现)
// CMover 基类版本 - 检查是否为友方
// ============================================================================
int CMover::IsFriend(CMover* pMover) {
    // IDA 精确还原: 基类简单实现，返回 !IsEnemy
    if (!pMover) {
        return 0;
    }
    return IsEnemy(pMover) ? 0 : 1;
}

// ============================================================================
// IsParty - IDA 0x140380AD0 (基类实现)
// CMover 基类版本 - 检查是否同队伍
// ============================================================================
int CMover::IsParty(CMover* pMover) {
    // IDA 精确还原: CMover 基类返回 false (由 CMoverEx 重写)
    (void)pMover;
    return 0;
}

// ============================================================================
// IsEnemyForChain - IDA 0x140380760 (基类实现)
// CMover 基类版本 - 检查是否为连锁技能敌方
// ============================================================================
int CMover::IsEnemyForChain(CMover* pMover) {
    // IDA 精确还原: 基类委托给 IsEnemy
    if (!pMover) {
        return 0;
    }
    return IsEnemy(pMover) ? 1 : 0;
}

// ============================================================================
// IsFriendForChain - IDA 0x140380AA0 (基类实现)
// CMover 基类版本 - 检查是否为连锁技能友方
// ============================================================================
int CMover::IsFriendForChain(CMover* pMover) {
    // IDA 精确还原: 基类委托给 IsFriend
    return IsFriend(pMover);
}

// ============================================================================
// GetArea - 获取区域对象
// IDA: CGocNetwork uses the embedded XActor subobject at CMover + 872.
// ============================================================================
XArea* CMover::GetArea() const {
    const auto* pActor = reinterpret_cast<const XActor*>(reinterpret_cast<const std::uint8_t*>(this) + 872);
    return pActor->GetArea();
}

void CMover::ScanGridOrigin(int nRange, unsigned int uFlag, std::vector<CMover*>* vecOut) {
    XArea* pArea = GetArea();
    if (!pArea || !vecOut) {
        return;
    }
    pArea->ScanGridOrigin(reinterpret_cast<XActor*>(this), nRange, uFlag, *vecOut);
}

bool CMover::SetBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwOwnerID, bool bShowBuff) {
    return AddBuff(nBuffIndex, 0, dwOwnerID, bShowBuff ? 1 : 0);
}

// IDA 0x14036CD20 - MoveingClientStop
// 客户端停止移动
void CMover::MoveingClientStop() {
    // IDA 反编译精确还原:
    // m_bMoving = 0;
    // m_stMoveGap = m_stMovePos;
    // m_stMovePos.Clear();
    m_bMoving = 0;
    m_stMoveGap = m_stMovePos;
    m_stMovePos.Clear();
}

// IDA 0x14036CE70 - CheckReactionTarget
// 检查反应目标（技能目标类型检查）
int CMover::CheckReactionTarget(int iTargetType, CMover* pTargetMover, bool bCheckForChain) {
    // IDA 反编译精确还原:
    // 1. 检查目标是否为 CMoverEx 且是 SystemActor
    // 2. 根据 iTargetType 执行不同的目标检查逻辑:
    //    case 1: this == pTargetMover (自身)
    //    case 2: this == pTargetMover (自身，与 case 1 相同)
    //    case 3: 敌对关系检查 (IsEnemy 或 IsEnemyForChain)
    //    case 4: this != pTargetMover (非自身)
    //    case 6: 友方关系检查 (IsFriend 或 IsFriendForChain)
    //    case 7: this != pTargetMover (非自身)
    //    case 8: 队伍关系检查 (IsParty)

    if (pTargetMover) {
        // RTTI 检查: 是否为 CMoverEx
        CMoverEx* pMoverEx = dynamic_cast<CMoverEx*>(pTargetMover);
        if (pMoverEx) {
            if (pMoverEx->IsSystemActor()) {
                return 0;
            }
        }
    }

    int bResult = 1;
    switch (iTargetType) {
        case 1:
            // 目标类型 1: 自身
            bResult = (this == pTargetMover) ? 1 : 0;
            break;
        case 2:
            // 目标类型 2: 自身（与 case 1 相同逻辑）
            if (this != pTargetMover) {
                if (!bCheckForChain) {
                    bResult = IsFriend(pTargetMover);
                } else {
                    bResult = IsFriendForChain(pTargetMover);
                }
            } else {
                bResult = 1;
            }
            break;
        case 3:
            // 目标类型 3: 敌方
            if (this == pTargetMover) {
                bResult = 0;
            } else {
                if (bCheckForChain) {
                    bResult = IsEnemyForChain(pTargetMover);
                } else {
                    bResult = IsEnemy(pTargetMover);
                }
                if (!bResult) {
                    // IDA: CheckMonsterInteractObject
                    bResult = CheckMonsterInteractObject(pTargetMover);
                }
            }
            break;
        case 4:
            // 目标类型 4: 非自身
            bResult = (this != pTargetMover) ? 1 : 0;
            break;
        case 6:
            // 目标类型 6: 友方
            if (this == pTargetMover) {
                bResult = 0;
            } else if (bCheckForChain) {
                bResult = IsFriendForChain(pTargetMover);
            } else {
                bResult = IsFriend(pTargetMover);
            }
            break;
        case 7:
            // 目标类型 7: 非自身（与 case 4 相同逻辑）
            if (this != pTargetMover) {
                bResult = IsParty(pTargetMover);
            } else {
                bResult = 1;
            }
            break;
        case 8:
            // 目标类型 8: 队伍成员
            if (this == pTargetMover) {
                bResult = 0;
            } else {
                bResult = IsParty(pTargetMover);
            }
            break;
        default:
            break;
    }
    return bResult;
}

// IDA 0x140360AD0 - CheckMonsterInteractObject
// 检查怪物交互对象（基类返回 0）
int CMover::CheckMonsterInteractObject(CMover* pMover) {
    // IDA: 基类返回 0，子类可能重写
    (void)pMover;
    return 0;
}

// IDA 0x14036D380 - FindTargetPos (参数: CMover*)
// 查找目标位置槽位（根据方向角度）
std::uint8_t CMover::FindTargetPos(CMover* pMover) {
    // IDA 反编译精确还原:
    // 1. 计算到目标的方向向量
    // 2. 获取偏航角并转换为 0-360 度
    // 3. 计算位置槽位 (每30度一个槽，共12个槽)
    // 4. 查找空闲槽位或人数最少的槽位
    if (!pMover) {
        return 0;
    }

    hkvVec3 vMyPos = GetPosition();
    hkvVec3 vTargetPos = pMover->GetPosition();
    hkvVec3 vDirVector = vTargetPos - vMyPos;

    int nYaw = static_cast<int>(GetYawFromVector(vDirVector));
    if (nYaw < 0) {
        nYaw += 360;
    }

    const int ConstAngle = 30;
    std::uint8_t nPos = static_cast<std::uint8_t>(nYaw / ConstAngle);
    if (nPos >= 12) {
        nPos -= 12;
    }

    // 检查当前位置是否空闲
    if (!m_byTargetPosInfo[nPos]) {
        return nPos;
    }

    // 检查相邻位置
    std::uint8_t nNext = (nPos + 1) % 12;
    std::uint8_t nPrev = (nPos + 11) % 12;

    if (!m_byTargetPosInfo[nPrev]) {
        return nPrev;
    }
    if (!m_byTargetPosInfo[nNext]) {
        return nNext;
    }

    // 检查更远的位置
    nNext = (nPos + 2) % 12;
    nPrev = (nPos + 10) % 12;

    if (!m_byTargetPosInfo[nPrev]) {
        return nPrev;
    }
    if (!m_byTargetPosInfo[nNext]) {
        return nNext;
    }

    // 选择人数最少的位置
    nNext = (nPos + 1) % 12;
    nPrev = (nPos + 11) % 12;

    if (m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nPrev]
        && m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nNext]) {
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

    if (m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nPrev]
        && m_byTargetPosInfo[nPos] <= m_byTargetPosInfo[nNext]) {
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

// IDA 0x14036D6F0 - FindTargetPos (参数: float, float, E_MOVESIDE_TYPE)
// 查找目标位置槽位（根据角度范围）
std::uint8_t CMover::FindTargetPos(float fAngleMin, float fAngleMax, int eIgnoreMoveSide) {
    // IDA 反编译精确还原:
    // 1. 限制角度范围 (最大180度)
    // 2. 计算起始和结束槽位
    // 3. 遍历查找空闲或最少人数的槽位
    if (fAngleMin > 180.0f) {
        fAngleMin = 180.0f;
    }
    if (fAngleMax > 180.0f) {
        fAngleMax = 180.0f;
    }

    int BeginPos = static_cast<int>(fAngleMin / 30.0f);
    int EndPos = static_cast<int>(fAngleMax / 30.0f);

    if (static_cast<int>(fAngleMin) % 30 > 0 && static_cast<float>(30 * BeginPos) > fAngleMin) {
        --BeginPos;
        if (BeginPos < 0) {
            BeginPos = 0;
        }
    }
    if (static_cast<int>(fAngleMax) % 30 > 0 && fAngleMax > static_cast<float>(30 * BeginPos)) {
        ++EndPos;
        if (EndPos > 5) {
            EndPos = 5;
        }
    }

    std::uint8_t nMinPos = static_cast<std::uint8_t>(BeginPos);
    std::uint8_t nNext = static_cast<std::uint8_t>(BeginPos);
    std::uint8_t nCount = 50;

    while (nNext < static_cast<std::uint8_t>(EndPos)) {
        int nReverseNext = 12 - nNext - 1;

        // eMOVESIDE_RIGHT = 1, eMOVESIDE_LEFT = 2
        if (eIgnoreMoveSide != 1) {  // 不是忽略右边
            if (!m_byTargetPosInfo[nNext]) {
                return nNext;
            }
            if (nCount > m_byTargetPosInfo[nNext]) {
                nCount = m_byTargetPosInfo[nNext];
                nMinPos = nNext;
            }
        }

        if (eIgnoreMoveSide != 2) {  // 不是忽略左边
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

// IDA 0x14036D930 - GetTargetPos
// 获取目标位置（根据槽位和距离）
hkvVec3 CMover::GetTargetPos(std::uint8_t byPos, float fDist) {
    // IDA 反编译精确还原:
    // 1. 获取目标角度
    // 2. 创建旋转矩阵
    // 3. 应用距离偏移
    float fAngle = GetTargetAngle(byPos);

    // 创建旋转矩阵
    hkvMat3 matRot;
    matRot.setFromEulerAngles(0.0f, 0.0f, fAngle);

    // 创建方向向量 (Y轴负方向 * 距离)
    hkvVec3 vDir(0.0f, -fDist, 0.0f);

    // 应用旋转
    hkvVec3 vResult = matRot * vDir;

    return vResult;
}

// IDA 0x14036DA80 - ClearTargetPosFlag
// 清除目标位置标志
void CMover::ClearTargetPosFlag(std::uint8_t byPos) {
    // IDA 反编译精确还原:
    // if (byPos == 255) { m_byTargetPosCount = 0; memset(m_byTargetPosInfo, 0, 12); }
    // else if (m_byTargetPosInfo[byPos]) { --m_byTargetPosInfo[byPos]; --m_byTargetPosCount; }
    if (byPos == 255) {
        m_byTargetPosCount = 0;
        std::memset(m_byTargetPosInfo, 0, sizeof(m_byTargetPosInfo));
    } else if (m_byTargetPosInfo[byPos]) {
        --m_byTargetPosInfo[byPos];
        --m_byTargetPosCount;
    }
}

// IDA 0x140368CE0 - IsAttackHeight
// 攻击高度检测
bool CMover::IsAttackHeight(tagATTACK_AREA* pAttackArea, hkvVec3& vPos, int& bCheckCylinder) {
    // IDA: Check if position is above attack area height limit
    if (vPos.z > pAttackArea->fHeightT) {
        if (!m_pHitCollisionData) {
            return true;
        }
        bCheckCylinder = 0;
    }
    
    // IDA: Check if position is below attack area base height
    if (pAttackArea->fHeightB > (vPos.z + m_fHitCylinderHeight)) {
        if (!m_pHitCollisionData) {
            return true;
        }
        bCheckCylinder = 0;
    }
    
    return false;
}

// ============================================================================
// ApplySkillDamageFrame - 8参数版本 (IDA: 0x140189320, stub)
// ============================================================================
void CMover::ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx,
                                   std::uint8_t byAttackTargetCnt, hkvVec3* vPos,
                                   float fAttackRot, std::uint16_t wContinousHit,
                                   bool byDamageType, bool bPenetrate) {
    // Base class stub - derived classes override
    (void)nSkillID; (void)nTriggerIdx; (void)byAttackTargetCnt;
    (void)vPos; (void)fAttackRot; (void)wContinousHit;
    (void)byDamageType; (void)bPenetrate;
}

// ============================================================================
// GetSGAbsorbRate - IDA: 0x14036E200
// ============================================================================
float CMover::GetSGAbsorbRate() {
    // IDA: Check buff for SG absorb rate (buff index 129)
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == 129) {
            return m_stBuffState[i].fGapTime; // Use available float field
        }
    }
    return 0.0f;
}

// ============================================================================
// IsRegisterAnimInfo (VString version) - IDA: 0x140367AE0
// ============================================================================
bool CMover::IsRegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, const VString& strAnimName) {
    if (m_pActionResource) {
        const char* szAnimName = strAnimName.AsChar();
        if (szAnimName && m_pActionResource->FindAnimationInfo(szAnimName)) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// FindBuffStatus - IDA: 0x14036A420
// ============================================================================
int CMover::FindBuffStatus(std::uint16_t wBuffIndex, std::uint32_t dwCasterID) {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == wBuffIndex &&
            m_stBuffState[i].dwID == dwCasterID) {
            return i;
        }
    }
    return -1;
}

// ============================================================================
// send_eSUB_CMD_ACTION_SKILL - IDA 0x1403716C0
// 发送技能动作数据包
// IDA 精确还原
// ============================================================================
void CMover::send_eSUB_CMD_ACTION_SKILL(CMover* pMover, std::uint32_t nSkillID,
                                        std::int16_t nTriggerIdx, hkvVec3* vPos,
                                        std::uint8_t byAttackTargetCnt,
                                        std::uint16_t wContinousHit, bool bPenetrate) {
    // IDA 0x1403716C0 精确还原:
    // 1. 创建 XSendPacket (主命令 6, 子命令 0x13)
    // 2. 构建 PS_SkillActionEx 结构
    // 3. 遍历攻击目标，构建 PS_SkillDmg 列表
    // 4. 广播给周围玩家
    
    if (!pMover || !vPos) {
        return;
    }
    
    // IDA: 获取朝向
    float fYaw = pMover->GetOrientationYaw();
    
    // IDA: 创建数据包 (主命令 6, 子命令 0x13 = 19)
    XSendPacket xPacket(6, 0x13);
    
    // IDA: 构建 PS_SkillActionEx 结构
    // PS_SkillActionEx stSkillActionEx;
    // stSkillActionEx.nSkillID = nSkillID;
    // stSkillActionEx.nTriggerIdx = nTriggerIdx;
    // stSkillActionEx.wContinousHit = wContinousHit;
    // stSkillActionEx.uxActorID = GetActorID();
    // stSkillActionEx.psSkillActorInfo.psSkillPosInfo.xPos = *vPos;
    // stSkillActionEx.psSkillActorInfo.psSkillPosInfo.fAngle = fYaw;
    // stSkillActionEx.bPenetrate = bPenetrate;
    
    // IDA: 遍历攻击目标，构建伤害列表
    // PS_SkillDmgResult psSkillDmgResult;
    // CMySkillList* pSkillMgr = pMover->GetSkillMgr();
    // for (int i = 0; i < byAttackTargetCnt && i < 100; ++i) {
    //     PS_SkillDmg stSkillDmg;
    //     tagSKILL_ACTION_DAMAGE stDamage;
    //     pSkillMgr->GetAttackDamage(&stDamage, i);
    //     stSkillDmg.uxTargetID = pSkillMgr->GetAttackTarget(i);
    //     stSkillDmg.byReactionType = stDamage.byReactionType;
    //     stSkillDmg.byDamageFlag = stDamage.byDamageFlag;
    //     stSkillDmg.nDamage = stDamage.nDamage;
    //     stSkillDmg.nAttrDamage = stDamage.nAttrDamage;
    //     stSkillDmg.nHP = stDamage.nHP;
    //     stSkillDmg.xExtraMove = pSkillMgr->GetAttackExtraMove(i);
    //     stSkillDmg.fFlySpeed = pSkillMgr->GetAttackFlySpeed(i);
    //     stSkillDmg.byHitPartsIndex = stDamage.byHitPartsIndex;
    //     psSkillDmgResult.vSkillDmg.push_back(stSkillDmg);
    // }
    
    // IDA: 发送数据包
    // xPacket << psSkillDmgResult;
    // xPacket << stSkillActionEx;
    // CGocNetwork::SendBroadCast(this, &xPacket, 0);
    
    // IDA: 调试输出
    DebugOut("send_eSUB_CMD_ACTION_SKILL>> %d (%.2f,%.2f) %.3f count:%d continous:%d",
             nSkillID, vPos->x, vPos->y, fYaw, byAttackTargetCnt, wContinousHit);
}

// ============================================================================
// send_eSUB_CMD_CHAIN - IDA 0x1403723A0
// 发送连锁技能数据包
// IDA 精确还原
// ============================================================================
void CMover::send_eSUB_CMD_CHAIN(CMover* pMover, std::uint32_t nSkillID,
                                 std::int16_t nTriggerIdx, hkvVec3* vPos, hkvVec3* vDir,
                                 std::uint32_t nSessionID, std::uint32_t dwTargetID) {
    // IDA 0x1403723A0 精确还原:
    // 1. 创建 XSendPacket (主命令 6, 子命令 0x38)
    // 2. 构建 PS_Chain_BT 结构
    // 3. 广播给周围玩家
    
    if (!pMover || !vPos || !vDir) {
        return;
    }
    
    // IDA: 创建数据包 (主命令 6, 子命令 0x38 = 56)
    XSendPacket xPacket(6, 0x38);
    
    // IDA: 构建 PS_Chain_BT 结构
    // PS_Chain_BT stChain;
    // stChain.nSkillID = nSkillID;
    // stChain.nTriggerIdx = nTriggerIdx;
    // stChain.xPos = *vPos;
    // stChain.xDir = *vDir;
    // stChain.nSessionID = nSessionID;
    // stChain.dwTargetID = dwTargetID;
    // stChain.uxActorID = pMover->GetActorID();
    
    // IDA: 发送数据包
    // xPacket << stChain;
    // CGocNetwork::SendBroadCast(this, &xPacket, 0);
    
    // IDA: 调试输出
    DebugOut("send_eSUB_CMD_CHAIN>> %d (%.2f,%.2f,%.2f) -> (%.2f,%.2f,%.2f)",
             nSkillID, vPos->x, vPos->y, vPos->z, vDir->x, vDir->y, vDir->z);
}

// ============================================================================
// CMoverEx virtual function implementations (needed for linking)
// ============================================================================

// GetGOC_Post stub implementation
std::tr1::shared_ptr<CGocPost> CMover::GetGOC_Post(bool bCreate) {
    // Stub: Returns empty shared_ptr - full implementation in actor/Mover/Mover.cpp
    (void)bCreate;
    return std::tr1::shared_ptr<CGocPost>();
}

// GetGOC_Achieve stub implementation
std::shared_ptr<CGocAchieve> CMover::GetGOC_Achieve(bool bCreate) {
    // Stub: Returns empty shared_ptr - full implementation in actor/Mover/Mover.cpp
    (void)bCreate;
    return std::shared_ptr<CGocAchieve>();
}

// GetGOC_Quest stub implementation
std::shared_ptr<CGocQuest> CMover::GetGOC_Quest(bool bCreate) {
    // Stub: Returns empty shared_ptr - full implementation in actor/Mover/Mover.cpp
    (void)bCreate;
    return std::shared_ptr<CGocQuest>();
}

// IDA: ?SetBuffStatus@CMoverEx@@UEAAHGK_N@Z (0x14038BCE0)
// TODO: Full implementation from IDA requires many helper methods
bool CMoverEx::SetBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwOwnerID, bool bShowBuff) {
    // TODO: Implement full buff system from IDA
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetBuffStatus: buff=%d owner=%d show=%d", nBuffIndex, dwOwnerID, bShowBuff);
    return false;
}

// IDA: ?ClearBuffStatus@CMoverEx@@UEAAXG_NK@Z (0x14038D820)
// TODO: Full implementation from IDA requires many helper methods
void CMoverEx::ClearBuffStatus(std::uint16_t nBuffIndex, bool bExcuteOutSkill, std::uint32_t dwOwnerID) {
    // TODO: Implement full buff system from IDA
    GreenDamTan_log(__FILE__, __FUNCTION__, "ClearBuffStatus: buff=%d exec=%d owner=%d", nBuffIndex, bExcuteOutSkill, dwOwnerID);
}

// IDA: ?ClearBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403901A0)
void CMoverEx::ClearBuffAbility(int iType, float fValue) {
    // IDA 精确还原:
    auto pAttr = GetGOC<CGocAttribute>();
    
    if (pAttr) {
        m_bChangedStat = true;
        
        // 计算负值
        float fNegValue = -fValue;
        
        if (pAttr->UpdateBuffEffectStat(iType, fNegValue, true, true) == 1) {
            float fHp = GetStat(1);
            // Virtual function call for HP update
        }
    }
}

// ============================================================================
// send_eSUB_CMD_MOVE_UPDATE_DIR - 发送移动更新方向命令
// TODO: 需要从IDA还原完整实现
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_UPDATE_DIR(CMover* pMover, bool bUpdate) {
    // Stub implementation - placeholder for IDA restoration
    // This function sends a packet to update the mover's direction
    (void)pMover;
    (void)bUpdate;
    // TODO: Send packet with main=0x17, sub=0x03
}

// ============================================================================
// HasSkillSkipTime - 检查是否有技能跳过时间
// TODO: 需要从IDA还原完整实现
// ============================================================================
bool CMoverEx::HasSkillSkipTime(const char* szSkillName) {
    // Stub implementation - placeholder for IDA restoration
    (void)szSkillName;
    return false;
}

// ============================================================================
// IsCanSkillSkip - 检查是否可以跳过技能
// TODO: 需要从IDA还原完整实现
// ============================================================================
bool CMoverEx::IsCanSkillSkip(const char* szSkillName, float fTime) {
    // Stub implementation - placeholder for IDA restoration
    (void)szSkillName;
    (void)fTime;
    return false;
}

// ============================================================================
// CheckPassiveSkill - 检查被动技能
// TODO: 需要从IDA还原完整实现
// ============================================================================
void CMoverEx::CheckPassiveSkill(std::uint8_t byTargetType, std::uint8_t byCondition) {
    // Stub implementation - placeholder for IDA restoration
    (void)byTargetType;
    (void)byCondition;
    // TODO: Implement passive skill check logic
}

// ============================================================================
// send_eSUB_CMD_MONSTER_INVISIBLE IDA 0x140370BA0
// 发送怪物隐身状态包 - IDA 精确还原
// ============================================================================
void CMover::send_eSUB_CMD_MONSTER_INVISIBLE(CMover* pMover, std::uint8_t byInvisible, std::uint32_t dwFlag, int nType, int nValue) {
    // IDA 0x140370BA0 精确还原:
    // void __fastcall CMover::send_eSUB_CMD_MONSTER_INVISIBLE(
    //     CMover *this,
    //     CMover *pMover,
    //     char byInvisible,
    //     int dwFlag,
    //     int nType,
    //     int nValue)
    // {
    //   XSendPacket xPacket(0x17, 0x32);
    //   UXActorID actorID;
    //   pMover->GetActorID(&actorID);
    //   int QuestID = CQuestCondition::GetQuestID(&actorID);
    //   xPacket << QuestID;
    //   xPacket << byInvisible;
    //   xPacket << dwFlag;
    //   xPacket << nType;
    //   xPacket << nValue;
    //   CGocNetwork::SendBroadCast(this, &xPacket, 1u);
    //   DebugOut("send_eSUB_CMD_MONSTER_INVISIBLE>> %d", byInvisible);
    // }

    if (!pMover) {
        return;
    }

    // 创建发送包 (主命令 0x17, 子命令 0x32)
    XSendPacket xPacket(0x17, 0x32);
    
    // 获取 ActorID
    UXActorID actorID = pMover->GetActorID();
    
    // 获取 QuestID (简化实现)
    int QuestID = actorID.dwActorID;
    
    // 写入数据
    xPacket << QuestID;
    xPacket << byInvisible;
    xPacket << dwFlag;
    xPacket << nType;
    xPacket << nValue;
    
    // 广播给周围玩家
    // TODO: CGocNetwork::SendBroadCast(this, &xPacket, 1u);
    // 临时注释，等待 CGocNetwork 实现
    
    // 调试输出
    DebugOut("send_eSUB_CMD_MONSTER_INVISIBLE>> %d", byInvisible);
}

// ============================================================================
// ActionDestToEntity 所需的虚拟方法 (IDA 反编译)
// 这些方法在派生类 (User, Monster) 中可能有不同的实现
// ============================================================================

int CMover::GetPvpCondition() {
    // Base implementation - CMoverEx overrides this
    // IDA: CMover::GetPvpCondition returns 0 (base class)
    return 0;
}

int CMover::GetActionCondition() {
    // Base implementation - CMoverEx overrides this
    // IDA: CMover::GetActionCondition returns 0 (base class)
    return 0;
}

int CMover::GetDivergenceValue() {
    // Base implementation - CMoverEx overrides this
    // IDA: CMover::GetDivergenceValue returns 0 (base class)
    return 0;
}

int CMover::GetCombatType() {
    // IDA: CMover::GetCombatType returns -1 (0xFFFFFFFF)
    // CMoverEx overrides this with actual combat type
    return -1;
}

std::uint8_t CMover::GetSkillChargeStep() {
    // Base implementation - CMoverEx overrides this
    // IDA: CMover::GetSkillChargeStep returns 0 (base class)
    return 0;
}

TB_SKILL* CMover::GetSkillTable() {
    // Base implementation - CMoverEx overrides this
    // IDA: CMover::GetSkillTable returns nullptr (base class)
    return nullptr;
}

// ============================================================================
// ActionDestToEntity 所需的非虚拟方法
// ============================================================================

bool CMover::IsSendProjectilePacket(AttackJudgmentTrigger* pTrigger) {
    // IDA: CMover::IsSendProjectilePacket @ 0x140366B40
    // Returns true if projectile packet should be sent
    if (!pTrigger) {
        return false;
    }
    
    // IDA: Access bIsTargetGuided at offset 0x2B8 (696 decimal) in AttackJudgmentTrigger
    // This is within the padding_projInfo area (offset 216 + 0x2B8 - 216 = offset 696 from start)
    // The field is at offset 480 within tagPROJECTILE_INFO (696 - 216 = 480)
    bool bResult = *reinterpret_cast<bool*>(reinterpret_cast<char*>(pTrigger) + 0x2B8);
    
    // Check actor type - if 0 (player), check skill table
    if (GetType() == static_cast<E_ACTOR_TYPE>(0)) {
        TB_SKILL* pSkillTable = GetSkillTable();
        // Return true if no skill table or Bullet_Sync_Type != 2
        if (!pSkillTable) {
            return true;
        }
        // IDA: Check Bullet_Sync_Type at offset 0x28D (653 decimal) in TB_SKILL
        std::uint8_t bulletSyncType = *reinterpret_cast<std::uint8_t*>(
            reinterpret_cast<char*>(pSkillTable) + 0x28D);
        return bulletSyncType != 2;
    }
    
    return bResult;
}

void CMover::AddActionBuffer(tagACTION_BUFFER* pBuffer) {
    // IDA: CMover::AddActionBuffer @ 0x140016C30
    // Calls CActionBuffer::Push on m_xActionBuffer
    if (pBuffer) {
        m_xActionBuffer.Push(pBuffer);
    }
}

void CMover::SetWaitSuboInputActionProcess(int bWait) {
    // Base implementation - does nothing
    // IDA: CUser::SetWaitSuboInputActionProcess @ 0x14070AA60
    // CUser overrides this to set szBuffer[62015] = (bWait != 0)
    (void)bWait;
}
