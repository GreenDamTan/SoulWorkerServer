#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"

// 前置声明 - ThreadLocalData 和 VDefaultTimer
class VDefaultTimer {
public:
    float GetTimeDifference() { return 0.0f; }
};
class ThreadLocalData {
public:
    static VDefaultTimer* GetTimer() {
        static VDefaultTimer s_timer;
        return &s_timer;
    }
};

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
    // 遍历所有 50 个 buff 槽位 (m_stBuffState[50])
    // 根据 byReason 参数判断是否清除特定类型的 buff
    
    if (m_nBuffTotalCnt == 0) {
        return;
    }
    
    for (std::uint8_t i = 0; i < 50; ++i) {
        if (m_stBuffState[i].byActive) {
            // buff 正在生效
            if (!byReason || IsClearBuff(m_stBuffState[i].dwBuffID, byReason)) {
                ClearBuffStatusBySlot(i, 0);
            }
        } else if (m_stBuffState[i].dwBuffID != 0) {
            // buff 已失效但槽位未清空
            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                // TODO: 需要获取 TB_BUFF 表
                // TB_BUFF* pBuffRef = pServer->GetTB_BUFF(m_stBuffState[i].dwBuffID);
                // if (pBuffRef && pBuffRef->Buff_Time == 0) {
                //     // 永久性 buff (Buff_Time == 0)
                //     if (!byReason || IsClearBuff(m_stBuffState[i].dwBuffID, byReason)) {
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
    
    // 清除 buff 状态
    m_stBuffState[bySlot].dwBuffID = 0;
    m_stBuffState[bySlot].dwSourceID = 0;
    m_stBuffState[bySlot].fRemainTime = 0.0f;
    m_stBuffState[bySlot].byBuffType = 0;
    m_stBuffState[bySlot].byActive = 0;
    
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
        if (m_stBuffState[i].dwBuffID == static_cast<std::uint32_t>(nBuffID)) {
            // 已存在，更新持续时间
            m_stBuffState[i].fRemainTime = static_cast<float>(nDuration) / 1000.0f;
            m_stBuffState[i].dwSourceID = dwSourceID;
            return true;
        }
    }
    
    // 查找空闲槽位
    for (std::uint8_t i = 0; i < 50; ++i) {
        if (m_stBuffState[i].dwBuffID == 0 || !m_stBuffState[i].byActive) {
            // 找到空闲槽位，添加 buff
            m_stBuffState[i].dwBuffID = static_cast<std::uint32_t>(nBuffID);
            m_stBuffState[i].dwSourceID = dwSourceID;
            m_stBuffState[i].fRemainTime = static_cast<float>(nDuration) / 1000.0f;
            m_stBuffState[i].byActive = 1;
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
        if (m_stBuffState[i].dwBuffID == static_cast<std::uint32_t>(nBuffID)) {
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
// ClearTraceBoneName IDA 0x140016C10
// 清除追踪骨骼名称列表
// ============================================================================
void CMover::ClearTraceBoneName() {
    // IDA 0x140016C10: std::vector<VString>::clear(&this->m_vTraceBoneName)
    m_vTraceBoneName.clear();
}

// ============================================================================
// RegisterTraceBoneName IDA 0x140016C20
// 注册追踪骨骼名称
// ============================================================================
void CMover::RegisterTraceBoneName(const VString& strBoneName) {
    // IDA 0x140016C20: std::vector<VString>::push_back(&this->m_vTraceBoneName, &strBoneName)
    m_vTraceBoneName.push_back(strBoneName);
}

// ============================================================================
// GetSkillCoolDownRate IDA 0x1402C7240
// ============================================================================
float CMover::GetSkillCoolDownRate() const {
    // IDA 0x1402C7240: return this->m_fSkillCoolDownRate
    return m_fSkillCoolDownRate;
}

// ============================================================================
// SetSkillCoolDownRate
// ============================================================================
void CMover::SetSkillCoolDownRate(float fRate) {
    // IDA: this->m_fSkillCoolDownRate = fRate
    m_fSkillCoolDownRate = fRate;
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
    // Decompilation: return XActor::IsDieStatus(&this->XActor) || this->GetHP(this) <= 0;
    // XActor::IsDieStatus checks the actor status flags for death state
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
// SetInvincibleActor IDA 0x1401B4820
// ============================================================================
void CMover::SetInvincibleActor(int bEnable) {
    // IDA 0x1401B4820: this->m_bInvincibleActor = bEnable
    m_bInvincibleActor = bEnable;
}

// ============================================================================
// SetMoveingInFly - 设置飞行移动状态
// ============================================================================
void CMover::SetMoveingInFly(int bFlying) {
    // TODO: IDA 验证具体实现
}

// ============================================================================
// SceneChanged - 场景切换通知
// ============================================================================
void CMover::SceneChanged() {
    // TODO: IDA 验证具体实现
}

// ============================================================================
// GetTableID IDA (virtual function, base implementation)
// ============================================================================
int CMover::GetTableID() {
    // 基类实现，子类会 override
    return 0;
}

// ============================================================================
// ThinkFunction - 思考函数 (基类实现)
// ============================================================================
void CMover::ThinkFunction() {
    // 基类实现，子类会 override
}

// ============================================================================
// GetMotionClass - IDA 0x140276270
// 返回当前动作类 (m_nMotionClass)
// ============================================================================
short CMover::GetMotionClass() {
    // IDA 0x140276270: movzx eax, word ptr [rcx+1274h]
    // 直接返回 m_nMotionClass 成员变量
    return m_nMotionClass;
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
    return const_cast<char*>(it->second.AsChar());
}

// ============================================================================
// CheckMoveCollision IDA 0x1403681B0
// 检查移动碰撞 - 扫描周围Actor找最近的碰撞目标
// ============================================================================
CMover* CMover::CheckMoveCollision(hkvVec3& vDestPos) {
    // IDA 0x1403681B0: 大型函数 (1236 bytes)
    // 1. 检查是否是 Monster 类型 (GetType == 2)
    // 2. 检查碰撞是否启用
    // 3. 检查是否在额外移动中
    // 4. 检查是否是跟随者
    // 5. 检查是否是巡逻怪物
    // 6. 扫描周围 Actor
    // 7. 找最近的碰撞目标

    // 检查是否是 Monster 类型
    if (GetVariableType() != 2) {  // E_ACTOR_TYPE_MONSTER
        return nullptr;
    }

    // 检查碰撞是否启用
    if (!m_bCollisionEnable) {
        return nullptr;
    }

    // 检查是否在额外移动中
    if (m_bKeepMovingExtra) {
        return nullptr;
    }

    // TODO: 需要完整的 CMonster RTTI 检查
    // 简化实现: 直接扫描周围 Actor
    // 完整实现需要:
    // - CMonster::IsFollower 检查
    // - CAi::IsPatrolMonster 检查
    // - XArea::ScanGridOrigin 扫描周围 Actor
    // - 遍历并检查每个 Actor 的碰撞半径

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
// 返回实体高度 (用于飞行检测和地面高度获取)
// ============================================================================
bool CMover::GetHeight(hkvVec3* vPos, float fMaxDist) {
    // IDA 反编译: 获取地面高度
    // 1. 检查是否在强制状态
    // 2. 调用物理引擎获取地面高度
    // 3. 更新 vPos->z 为地面高度

    if (!vPos) {
        return false;
    }

    // 简化实现: 使用当前地面高度或胶囊高度
    // 实际实现需要 Havok 物理引擎的射线检测
    if (m_bOnGround) {
        vPos->z = m_fGroundPosZ;
        return true;
    }

    // 返回胶囊高度作为默认值
    vPos->z = m_fCapsuleHeight;
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
// ClearMotion
// 清除当前动画播放状态 (被 CheckAnimationEnd/MoverEx::ClearMotion 调用)
// ============================================================================
void CMover::ClearMotion() {
    // 清除动画播放标志和当前动画事件指针
    // IDA: 在 CheckAnimationEnd 中调用此函数以结束非循环动画
    m_bAnimPlay = 0;
    m_pCurMotionEvent = nullptr;
    m_fAnimationTime = 0.0f;
    m_fAnimPercentTime = 0.0f;
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
    return m_eActorType;
}

// ============================================================================
// SetPositionXVec3 IDA 0x1401893C0
// ============================================================================
void CMover::SetPositionXVec3(const hkvVec3& vPos) {
    // IDA 0x1401893C0: memcpy(&this->m_vPosition, vPos, 12)
    m_vPosition = vPos;
}

// ============================================================================
// Move IDA 0x14036DDD0
// 位置移动 - 调用 XArea::MoveActor 进行实际移动
// ============================================================================
void CMover::Move(const hkvVec3& vDestPos) {
    // IDA 反编译: 调用 m_pArea->MoveActor_2 进行实际移动
    // TODO: 需要 XArea::MoveActor_2 实现
}

// ============================================================================
// SetDie IDA 0x140188FE0
// 设置死亡状态 (基类实现)
// ============================================================================
void CMover::SetDie(std::int16_t nMotionClass, int bSuicide, bool bSendPacket) {
    // IDA 反编译: 基类实现设置死亡状态
    // 子类 (CMoverEx/CMonster/CUser) 会 override 此函数以添加:
    // - 死亡动画触发
    // - 仇恨清除
    // - 掉落物生成
    // - 区域通知

    // 设置死亡状态标志
    m_dwStatus |= 0x00000002;  // 死亡状态标志

    // 清除移动状态
    m_bMoving = 0;
    m_bGazeMoving = 0;

    // 清除目标
    m_dwTargetID = 0xFFFFFFFF;

    // 重置死亡延迟时间
    m_fDieDelayTime = -1.0f;
    m_fDieDelayMaxTime = -1.0f;
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

// ============================================================================
// Movement Functions - IDA 反编译实现
// ============================================================================

// ============================================================================
// GetMoveSpeed IDA 0x1406C5C30
// ============================================================================
float CMover::GetMoveSpeed() {
    // IDA 0x1406C5C30: return this->m_fMoveSpeed
    return m_fMoveSpeed;
}

// ============================================================================
// IsMoving IDA 0x14027A610
// ============================================================================
bool CMover::IsMoving() {
    // IDA 0x14027A610: return this->m_bMoving
    // Note: IDA shows m_fMoving, but it's actually m_bMoving (int)
    return m_bMoving != 0;
}

// ============================================================================
// IsGazeMoving IDA 0x140375200
// ============================================================================
bool CMover::IsGazeMoving() {
    // IDA 0x140375200: return this->m_bGazeMoving
    return m_bGazeMoving != 0;
}

// ============================================================================
// GetTargetDestPos IDA 0x140280C80
// ============================================================================
std::uint8_t CMover::GetTargetDestPos() {
    // IDA 0x140280C80: return this->m_byTargetDestPos
    return m_byTargetDestPos;
}

// ============================================================================
// SetTargetDestPos IDA 0x140280C60
// ============================================================================
void CMover::SetTargetDestPos(std::uint8_t byPos) {
    // IDA 0x140280C60: this->m_byTargetDestPos = byPos
    m_byTargetDestPos = byPos;
}

// ============================================================================
// SetKeepMovingExtra IDA 0x1402C7420
// ============================================================================
void CMover::SetKeepMovingExtra(int bKeepMoving) {
    // IDA 0x1402C7420: this->m_bKeepMovingExtra = bKeepMoving
    m_bKeepMovingExtra = bKeepMoving;
}

// ============================================================================
// ProcessExtraMoving IDA 0x14036BC20
// 处理额外移动 (击退、拉扯等效果)
// 大型函数 (1262 bytes)
// ============================================================================
void CMover::ProcessExtraMoving() {
    if (!reinterpret_cast<tagMOVE_POS*>(&m_stExtMovingVal)->IsZero()) {
        m_vPrevPos = GetPosition();
        if (m_stExtMovingVal.fRemainTime <= 0.0f) {
            ReleaseExtraMoving();
        } else {
            float fDiffX = m_stExtMovingVal.x - m_vPosition.x;
            float fDiffY = m_stExtMovingVal.y - m_vPosition.y;
            float fDeltaTime = ThreadLocalData::GetTimer()->GetTimeDifference();
            if (fabsf(fDiffX) >= 3.0f || fabsf(fDiffY) >= 3.0f) {
                float fDeltaX = (fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffX;
                if (fDiffX <= 0.0f) {
                    if (fDeltaX < fDiffX) fDeltaX = fDiffX;
                } else {
                    if (fDeltaX > fDiffX) fDeltaX = fDiffX;
                }
                float fDeltaY = (fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffY;
                if (fDiffY <= 0.0f) {
                    if (fDeltaY < fDiffY) fDeltaY = fDiffY;
                } else {
                    if (fDeltaY > fDiffY) fDeltaY = fDiffY;
                }
                m_stExtMovingVal.fRemainTime -= fDeltaTime;
                hkvVec3 vDestPos = m_vPrevPos + hkvVec3(fDeltaX, fDeltaY, 0.0f);
                if (!IsFlying()) GetHeight(&vDestPos, 200.0f);
                if (CheckMoveCollision(vDestPos)) {
                    send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, GetPosition(), 0);
                } else {
                    if (!CheckMoveDestPos(vDestPos, false, 0))
                        send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, vDestPos, 0);
                    Move(vDestPos);
                }
            } else {
                m_stExtMovingVal.Clear();
            }
        }
    }
}

// ============================================================================
// ReleaseExtraMoving IDA 0x14036C120
// 释放额外移动效果
// 大型函数 (236 bytes)
// ============================================================================
void CMover::ReleaseExtraMoving() {
    if (!reinterpret_cast<tagMOVE_POS*>(&m_stExtMovingVal)->IsZero()) {
        if (m_stExtMovingVal.fMovingTime == 0.1f) {
            hkvVec3 vExtraPos(m_stExtMovingVal.x, m_stExtMovingVal.y, m_vPosition.z);
            Move(hkvVec3(vExtraPos.x, vExtraPos.y, vExtraPos.z));
        }
        m_stExtMovingVal.Clear();
    }
}

// ============================================================================
// AddExtraMoving IDA 0x14036C210
// 添加额外移动增量
// 大型函数 (363 bytes)
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
    CheckMoveDestPos(vDestPos, false, 0);
    m_stExtMovingVal.x = vDestPos.x;
    m_stExtMovingVal.y = vDestPos.y;
    m_stExtMovingVal.fMovingTime = (fTime <= m_stExtMovingVal.fMovingTime) ? m_stExtMovingVal.fMovingTime : fTime;
    m_stExtMovingVal.fRemainTime = m_stExtMovingVal.fMovingTime + 0.2f;
}

// ============================================================================
// SetExtraMoving IDA 0x14036C380
// 设置额外移动目标
// 大型函数 (181 bytes)
// ============================================================================
void CMover::SetExtraMoving(float x, float y, float fTime) {
    hkvVec3 vDestPos(x, y, m_vPosition.z);
    CheckMoveDestPos(vDestPos, false, 0);
    m_stExtMovingVal.x = vDestPos.x;
    m_stExtMovingVal.y = vDestPos.y;
    m_stExtMovingVal.fMovingTime = fTime;
    m_stExtMovingVal.fRemainTime = fTime + 0.2f;
}

// ============================================================================
// ChangeMotion IDA 0x1402AC570 (基类空实现)
// 改变动作类 - 子类 CMoverEx/CMonster/CUser 会 override
// ============================================================================
void CMover::ChangeMotion(std::int16_t wType) {
    // IDA 0x1402AC570: 空函数
    // 基类空实现，子类会 override
    // CMoverEx::ChangeMotion (0x14037C310) - 完整实现
    // CMonster::ChangeMotion (0x14035D350) - 完整实现
    // CUser::ChangeMotion (0x1406F1AA0) - 完整实现
}

// ============================================================================
// Movement Packet Functions
// 这些函数用于向周围玩家广播移动状态
// ============================================================================

// ============================================================================
// send_eSUB_CMD_MOVE IDA 0x14036EAC0
// 大型函数 (807 bytes) - 广播移动数据包
// ============================================================================
void CMover::send_eSUB_CMD_MOVE(CMover* pMover, float fTargetPosX, float fTargetPosY, std::uint8_t byRunBit) {
    // IDA 反编译核心逻辑:
    // 1. 获取移动朝向 (GetMovingYaw)
    // 2. 验证朝向值有效性 (-360 到 360)
    // 3. 获取移动速度
    // 4. 获取当前位置
    // 5. 获取地图 ID
    // 6. 填充 ST_MOVE 结构
    // 7. 创建 XSendPacket (main=5, sub=2)
    // 8. 广播给周围玩家
    // 9. 重置 m_fLastSendMoveTime = 0

    // TODO: 需要完整的 XSendPacket 和 ST_MOVE 结构实现
    GreenDamTan_log(__FILE__, __FUNCTION__, "send_eSUB_CMD_MOVE - TODO: need packet structures");
}

// ============================================================================
// send_eSUB_CMD_MOVE_STOP IDA 0x14036EE90
// 大型函数 (837 bytes) - 广播停止移动数据包
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_STOP(CMover* pMover) {
    // IDA 反编译核心逻辑:
    // 1. 获取移动朝向 (GetMovingYaw)
    // 2. 验证朝向值有效性
    // 3. 获取当前位置
    // 4. 获取地图 ID
    // 5. 填充 ST_MOVE_STOP 结构
    // 6. 创建 XSendPacket (main=5, sub=4)
    // 7. 广播给周围玩家
    // 8. 重置 m_fLastSendMoveTime = 0

    // TODO: 需要完整的 XSendPacket 和 ST_MOVE_STOP 结构实现
    GreenDamTan_log(__FILE__, __FUNCTION__, "send_eSUB_CMD_MOVE_STOP - TODO: need packet structures");
}

// ============================================================================
// send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA IDA 0x140370100
// 广播忽略动画增量移动数据包 (用于额外移动效果)
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(CMover* pMover, const hkvVec3& vPos, bool bFlag) {
    // IDA 反编译: 用于 ProcessExtraMoving 中
    // 当碰撞检测失败或移动目标无效时发送

    // TODO: 需要完整实现
    GreenDamTan_log(__FILE__, __FUNCTION__, "send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA - TODO");
}
