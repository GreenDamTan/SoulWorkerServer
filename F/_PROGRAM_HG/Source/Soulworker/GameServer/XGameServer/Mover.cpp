#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

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
