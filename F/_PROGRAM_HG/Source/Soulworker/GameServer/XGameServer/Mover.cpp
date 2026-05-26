#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"

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
    : m_fLastUpdateTime(0.0f)
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
    , m_stMovePos_dummy(nullptr)
    , m_stMoveGap_dummy(nullptr)
    , m_stMoveOffset_dummy(nullptr)
    , m_stExtMovingVal_dummy(nullptr)
    , m_stTimeSlow_dummy(nullptr)
    , m_vPrevPos_dummy(nullptr)
    , m_vCreatePos_dummy(nullptr)
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
// 析构函数 IDA 0x140366760 (PDB public: 0x140365760) -> 0x140366938
// 大小: 472 bytes
// ============================================================================
CMover::~CMover() {
    // IDA 反编译确认:
    // 1. vtable 恢复为 CMover 的 vtable (多继承)
    // 2. Destroy() 清理资源
    // 3. 逆序销毁成员容器
    // 4. XActor 和 VisBaseEntity_cl 基类析构

    // 编译器在构造/析构时会自动恢复 vtable 到 CMover 层级
    // 这里仅保持与 IDA 语义一致的 Destroy() 调用
    Destroy();

    // 成员容器由编译器自动逆序析构
    // 基类 XActor::~XActor() 和 VisBaseEntity_cl::~VisBaseEntity_cl() 由编译器自动调用
    GreenDamTan_log(__FILE__, __FUNCTION__, "CMover destructed");
}

// ============================================================================
// OnUpdate IDA 0x140366F60 -> 0x140366F9E
// 大小: 62 bytes
// 调用 VisBaseEntity_cl::OnUpdate(fDelta)
// ============================================================================
void CMover::OnUpdate(float fDelta) {
    // IDA 反编译:
    // (*(void (__fastcall **)(char *))(*((_QWORD *)this - 109) + 272LL))((char *)this - 872);
    // 通过 vtable 偏移 272 调用 VisBaseEntity_cl::OnUpdate
    // 简化实现: 编译器自动解析到基类
    // TODO: 需要确认 VisBaseEntity_cl 虚表布局后取消注释
    // VisBaseEntity_cl::OnUpdate(fDelta);
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
    // TODO: 需要访问 VisObject3D_cl 基类的 m_vPosition 成员
    return hkvVec3(0.0f, 0.0f, 0.0f);
}

// ============================================================================
// SetPosition IDA 0x140189790 (通过 hkvVec3 设置 VisObject3D_cl 位置)
// ============================================================================
void CMover::SetPosition(const hkvVec3& vPos) {
    // TODO: VisObject3D_cl::SetPosition(this, vPos);
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

void CMover::AllBuffClear(int nFlag) {
    // TODO: 汇编还原 - IDA 0x14036694F Destroy 内调用
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

float CMover::GetStat(int iIndex) const {
    // IDA 0x140166360 - 从能力数组返回值
    if (m_fAbility) {
        return m_fAbility[iIndex];
    }
    return 0.0f;
}

CMySkillList* CMover::GetSkillMgr() {
    // IDA 0x140366BE0
    return m_pSkillMgr;
}

// ============================================================================
// SetCurSkillTableIdx IDA 0x1402763F0
// ============================================================================
void CMover::SetCurSkillTableIdx(int nIdx) {
    // IDA 0x1402763F0: this->m_nCurSkillTableIdx = nIdx
    m_nCurSkillTableIdx = nIdx;
}

// ============================================================================
// GetSkillLevel IDA 0x1401ACFA0
// 基类返回 0 - 由子类 CMoverEx override
// ============================================================================
std::uint8_t CMover::GetSkillLevel() {
    // IDA 0x1401ACFA0: return 0
    // 基类返回 0，子类会 override
    return 0;
}

// ============================================================================
// SetHitCollisionData IDA 0x140016BD0
// ============================================================================
void CMover::SetHitCollisionData(void* pData) {
    // IDA 0x140016BD0: this->m_pHitCollisionData = pData
    m_pHitCollisionData = pData;
}

// ============================================================================
// SetHitCylinder IDA 0x140016BF0
// ============================================================================
void CMover::SetHitCylinder(float fRadius, float fHeight) {
    // IDA 0x140016BF0
    m_fHitCylinderRadius = fRadius;
    m_fHitCylinderHeight = fHeight;
}

// ============================================================================
// AddActionBuffer IDA 0x140016C30
// ============================================================================
void CMover::AddActionBuffer(void* xAction) {
    // IDA 0x140016C30: CActionBuffer::Push(&this->m_xActionBuffer, xAction)
    // TODO: 需要 CActionBuffer::Push 实现
}

// ============================================================================
// SetNoSkillCostSG IDA 0x1400488E0
// ============================================================================
void CMover::SetNoSkillCostSG(bool bCost) {
    // IDA 0x1400488E0
    m_bNoSkillCostSG = bCost;
}

// ============================================================================
// SetWeightRank IDA 0x140364D40
// ============================================================================
void CMover::SetWeightRank(std::uint8_t cVal) {
    // IDA 0x140364D40: this->m_cWeightRank = cVal
    m_cWeightRank = cVal;
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
// ============================================================================
bool CMover::IsDie() {
    // IDA 0x140366E40: XActor::IsDieStatus || GetHP() <= 0
    // TODO: 需要 XActor::IsDieStatus 实现
    // return XActor::IsDieStatus(&this->XActor) || GetHP() <= 0;
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
    // IDA 0x140367080:
    // if (m_fForcedStateApplyTime > 0.0) return m_uiForcedState == 1;
    // if (m_bLanded) return false;
    // 检查高度差
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 1;
    }
    if (m_bLanded) {
        return false;
    }
    // TODO: 需要高度检测实现
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
        // TODO: 需要 XActor::IsStatus 实现
        // return XActor::IsStatus(&this->XActor, 0x800);
        return false;
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
        // TODO: 需要 VAnimationInfo 结构定义
        // return m_pCurMotionEvent->fAnimationLength;
        return 0.0f;
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
    if (m_pCurMotionEvent) {
        // TODO: 需要 VAnimationInfo 结构定义
        // if (m_pCurMotionEvent->fAnimationLength > 0.0f) {
        //     m_fAnimPercentTime = fTime / m_pCurMotionEvent->fAnimationLength;
        // }
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
        // TODO: 需要 VAnimationInfo 结构定义
        // m_fAnimationTime = m_pCurMotionEvent->fAnimationLength * fPos;
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
// 大型复杂函数，需要完整实现
// ============================================================================
void CMover::CheckAnimationEnd() {
    // IDA 0x140367C80: 大型函数 (1315 bytes)
    // 1. 检查动画状态
    // 2. 更新动画时间
    // 3. 处理动画结束
    // 4. 计算偏移量
    // 5. 碰撞检测
    // TODO: 需要完整实现
    if (!m_bAnimChanged && m_bAnimPlay && m_pCurMotionEvent) {
        // 简化实现
    }
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
// 需要 XActionResMgr 相关实现
// ============================================================================
void CMover::SetupAnimation() {
    // IDA 0x140367980: 大型函数 (352 bytes)
    // 1. GetActionResourceFN 获取动作资源文件名
    // 2. 从 XGameServer 获取资源
    // 3. SetAnimInfoToActor 设置动画信息
    // 4. SetupAnimInfo 设置动画信息
    // 5. 设置 m_nHitAnimCount
    // TODO: 需要完整实现
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
// SetInvincibleActor IDA 0x1401B4820
// ============================================================================
void CMover::SetInvincibleActor(int bEnable) {
    // IDA 0x1401B4820: this->m_bInvincibleActor = bEnable
    m_bInvincibleActor = bEnable;
}

// ============================================================================
// GetTableID IDA (virtual function, base implementation)
// ============================================================================
int CMover::GetTableID() {
    // 基类实现，子类会 override
    return 0;
}

// ============================================================================
// GetTableIDString IDA 0x14036DE70
// ============================================================================
const char* CMover::GetTableIDString() {
    // IDA 0x14036DE70:
    // if (VString::IsEmpty(&m_strTableID)) {
    //   int id = GetTableID();
    //   VString::Format(&m_strTableID, "%d", id);
    // }
    // return VString::AsChar(&m_strTableID);
    // TODO: 需要 VString 完整实现
    return "";
}

// ============================================================================
// GetAnimStirng IDA 0x1403688D0
// ============================================================================
char* CMover::GetAnimStirng(unsigned int dwAnimKey) {
    // IDA 0x1403688D0:
    // if (!m_mapAnimInfoString) return nullptr;
    // auto it = m_mapAnimInfoString->find(dwAnimKey);
    // if (it == m_mapAnimInfoString->end()) return nullptr;
    // return VString::GetChar(&it->second);
    if (!m_mapAnimInfoString) {
        return nullptr;
    }
    auto it = m_mapAnimInfoString->find(dwAnimKey);
    if (it == m_mapAnimInfoString->end()) {
        return nullptr;
    }
    // TODO: 需要 VString::GetChar 实现
    return nullptr;
}

// ============================================================================
// CheckMoveCollision IDA 0x1403681B0
// 大型复杂函数，简化实现
// ============================================================================
CMover* CMover::CheckMoveCollision(hkvVec3& vDestPos) {
    // IDA 0x1403681B0: 大型函数 (1236 bytes)
    // 1. 检查是否是 Monster 类型
    // 2. 检查碰撞是否启用
    // 3. 扫描周围 Actor
    // 4. 找最近的碰撞目标
    // TODO: 需要完整实现
    return nullptr;
}

// ============================================================================
// RemoveTargetDestPos IDA 0x14036DB20
// ============================================================================
void CMover::RemoveTargetDestPos() {
    // IDA 0x14036DB20:
    // if (m_byTargetDestPos != 255 && m_dwTargetID != -1) {
    //   CMover* pTarget = GetMoverObject(m_dwTargetID);
    //   if (pTarget) ClearTargetPosFlag(pTarget, m_byTargetDestPos);
    // }
    // m_byTargetDestPos = 255;
    if (m_byTargetDestPos != 0xFF && m_dwTargetID != 0xFFFFFFFF) {
        // TODO: 需要 GetMoverObject 和 ClearTargetPosFlag 实现
        // CMover* pTarget = GetMoverObject(m_dwTargetID);
        // if (pTarget) ClearTargetPosFlag(pTarget, m_byTargetDestPos);
    }
    m_byTargetDestPos = 0xFF;
}

// ============================================================================
// CheckMoveDestPos IDA (待确认地址)
// ============================================================================
bool CMover::CheckMoveDestPos(hkvVec3& vDestPos, bool bFlying, int nFlag) {
    // TODO: 需要从 IDA 反编译确认实现
    return true;
}

// ============================================================================
// GetHeight IDA (待确认地址)
// ============================================================================
bool CMover::GetHeight(hkvVec3* vPos, float fMaxDist) {
    // TODO: 需要从 IDA 反编译确认实现
    // 用于 IsFlying 检测高度
    return false;
}

// ============================================================================
// GetHavokCapsuleRadius IDA (待确认地址)
// ============================================================================
float CMover::GetHavokCapsuleRadius() {
    // TODO: 需要从 IDA 反编译确认实现
    // 返回 Havok 物理胶囊半径
    return m_fCapsuleRadius;
}

// ============================================================================
// ClearMotion IDA (待确认地址)
// ============================================================================
void CMover::ClearMotion() {
    // TODO: 需要从 IDA 反编译确认实现
    // 清除当前动画状态
    m_bAnimPlay = 0;
    m_pCurMotionEvent = nullptr;
}

// ============================================================================
// IsAttackHeight IDA 0x140368CE0
// ============================================================================
bool CMover::IsAttackHeight(void* pAttackArea, hkvVec3& vPos, int& bCheckCylinder) {
    // IDA 0x140368CE0:
    // tagATTACK_AREA* pArea = (tagATTACK_AREA*)pAttackArea;
    // if (vPos.z > pArea->fHeightT) {
    //   if (!m_pHitCollisionData) return true;
    //   bCheckCylinder = false;
    // }
    // if (pArea->fHeightB > (vPos.z + m_fHitCylinderHeight)) {
    //   if (!m_pHitCollisionData) return true;
    //   bCheckCylinder = false;
    // }
    // return false;

    // TODO: 需要 tagATTACK_AREA 结构定义
    // struct tagATTACK_AREA {
    //   float fHeightT;
    //   float fHeightB;
    //   // ... other fields
    // };

    // 简化实现，需要完整结构定义后修正
    return false;
}

// ============================================================================
// IsRegisterAnimInfo IDA 0x140367AE0
// ============================================================================
bool CMover::IsRegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, void* strAnimName) {
    // IDA 0x140367AE0:
    // if (m_pActionResource) {
    //   const char* pAnimName = VString::AsChar((VString*)strAnimName);
    //   if (VActionResourceLump::FindAnimationInfo(m_pActionResource, pAnimName)) {
    //     VString::~VString((VString*)strAnimName);
    //     return true;
    //   }
    // }
    // VString::~VString((VString*)strAnimName);
    // return false;

    if (!m_pActionResource) {
        return false;
    }
    // TODO: 需要 VActionResourceLump::FindAnimationInfo 实现
    return false;
}

// ============================================================================
// GetMoverObject - 静态函数，根据 ID 获取 Mover 对象
// ============================================================================
CMover* CMover::GetMoverObject(std::uint32_t dwID) {
    // TODO: 需要从全局对象管理器获取
    return nullptr;
}

// ============================================================================
// ClearTargetPosFlag IDA (待确认地址)
// ============================================================================
void CMover::ClearTargetPosFlag(CMover* pTarget, std::uint8_t byPos) {
    // TODO: 需要从 IDA 反编译确认实现
}

// ============================================================================
// GetTargetID IDA 0x140198DE0
// ============================================================================
std::uint32_t CMover::GetTargetID() const {
    // IDA 0x140198DE0: return this->m_dwTargetID
    return m_dwTargetID;
}

// ============================================================================
// GetDefenseType IDA 0x14019B970
// ============================================================================
std::uint8_t CMover::GetDefenseType() const {
    // IDA 0x14019B970: return this->m_byDefenseType
    return m_byDefenseType;
}

// ============================================================================
// GetCurMotionEvent IDA 0x140199E30
// ============================================================================
const VAnimationInfo* CMover::GetCurMotionEvent() const {
    // IDA 0x140199E30: return this->m_pCurMotionEvent
    return m_pCurMotionEvent;
}

// ============================================================================
// GetSkillDestPos IDA 0x140189120
// ============================================================================
hkvVec3 CMover::GetSkillDestPos() {
    // IDA 0x140189120: return hkvVec3::ZeroVector()
    return hkvVec3(0.0f, 0.0f, 0.0f);
}

// ============================================================================
// GetActionResourceFN IDA 0x140189150
// ============================================================================
VString CMover::GetActionResourceFN() {
    // IDA 0x140189150: return empty VString
    // 基类返回空字符串，子类 (CMoverEx/CMonster/CUser) 会 override
    return VString();
}

// ============================================================================
// GetVariableType IDA 0x140189240
// ============================================================================
int CMover::GetVariableType() {
    // IDA 0x140189240: return this->m_eActorType (E_ACTOR_TYPE enum)
    // TODO: 需要 m_eActorType 成员变量定义
    return 0;
}

// ============================================================================
// SetPositionXVec3 IDA 0x1401893C0
// ============================================================================
void CMover::SetPositionXVec3(const hkvVec3& vPos) {
    // IDA 0x1401893C0: memcpy(&this->m_vPosition, vPos, 12)
    // TODO: 需要访问 VisObject3D_cl 基类的 m_vPosition 成员
    // m_vPosition = vPos;
}

// ============================================================================
// ClearExtraMoving IDA 0x140189390
// ============================================================================
void CMover::ClearExtraMoving() {
    // IDA 0x140189390: tagEXTRA_MOVEPOS::Clear(&this->m_stExtMovingVal)
    // TODO: 需要 tagEXTRA_MOVEPOS::Clear 实现
    // tagEXTRA_MOVEPOS::Clear(m_stExtMovingVal_dummy);
}

// ============================================================================
// SetDie IDA 0x140188FE0
// 基类空实现 - 由子类 CMoverEx/CMonster/CUser override
// ============================================================================
void CMover::SetDie(std::int16_t nMotionClass, int bSuicide, bool bSendPacket) {
    // IDA 0x140188FE0: 空函数
    // 基类空实现，子类会 override
}

// ============================================================================
// SetHP IDA 0x140189230
// 基类空实现 - 由子类 override
// ============================================================================
void CMover::SetHP(int nHP) {
    // IDA 0x140189230: 空函数
    // 基类空实现，子类会 override
}

// ============================================================================
// Damage IDA 0x140189300
// 基类空实现 - 由子类 override
// ============================================================================
void CMover::Damage(std::uint32_t dwID, std::uint8_t byReactionType, std::uint8_t byAttackCollision) {
    // IDA 0x140189300: 空函数
    // 基类空实现，子类会 override
}

// ============================================================================
// ApplySkillDamageFrame IDA 0x140189320
// 基类空实现 - 由子类 override
// ============================================================================
void CMover::ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx, std::uint8_t byAttackTargetCnt) {
    // IDA 0x140189320: 空函数
    // 基类空实现，子类会 override
}

// ============================================================================
// DamageProcessHP IDA 0x1401892E0
// 基类返回 1 - 由子类 override
// ============================================================================
int CMover::DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage, int nUnk1, std::uint8_t byUnk1, std::uint8_t byUnk2) {
    // IDA 0x1401892E0: return 1
    // 基类返回 1，子类会 override
    return 1;
}

// ============================================================================
// ActionProcess IDA 0x1401892D0
// 基类返回 1 - 由子类 override
// ============================================================================
int CMover::ActionProcess(std::int16_t nTriggerIdx) {
    // IDA 0x1401892D0: return 1
    // 基类返回 1，子类会 override
    return 1;
}

// ============================================================================
// ClearBuffProcess IDA 0x1401892B0
// 基类返回 1 - 由子类 override
// ============================================================================
int CMover::ClearBuffProcess(int nSkillID, AttackJudgmentTrigger* pTrigger, hkvVec3& vCurPos) {
    // IDA 0x1401892B0: return 1
    // 基类返回 1，子类会 override
    return 1;
}
