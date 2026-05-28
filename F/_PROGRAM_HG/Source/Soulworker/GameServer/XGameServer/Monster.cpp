#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/Ai.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include <cmath>

// Forward declarations for types not yet fully defined
class CUser;
class CBattleZone;

// Skill type constants for protect skills
constexpr int SKILLTYPE_NONE = 0;
constexpr int SKILLTYPE_PROTECT_A = 6;  // Absorb damage type
constexpr int SKILLTYPE_PROTECT_B = 7;  // Time-based type

// ============================================================================
// 构造函数
// ============================================================================
CMonster::CMonster()
    : CMoverEx()
    , m_pMobTableRef(nullptr)
    , m_pAi(nullptr)
    , m_nNextSkillID(0)
    , m_shLastSendMoveYaw(0)
    , m_bNeedSendMoveStop(0)
    , m_fInitYaw(0.0f)
    , m_fLastAggroCheckTime(0.0f)
    , m_bChangedAggro(0)
    , m_dwGuardID(0)
    , m_byGazeAnimType(0)
    , m_nSpawnBoxID(0)
    , m_nGroupID(0)
    , m_bShowChangedAI(false)
    , m_dwSpawnedTime64(0)
    , m_bSuicide(0)
    , m_dwGuardMonsterID(0)
    , m_nMoveType(0)
    , m_dwKillerID(0)
    , m_pWeaponTableRef(nullptr)
    , m_fLifeTime(0.0f)
    , m_nHPPerSec(0)
    , m_nLastTime(0)
    , m_nProtectSkill(0)
    , m_eSkillType(0)
    , m_nProtectSkillDamage(0)
    , m_bApplyLevel(0)
    , m_bySummonType(0)
    , m_fSummonLifeTime(0.0f)
    , m_nHitCount(0)
    , m_pScriptInst(nullptr)
    , m_naCount(0)
    , m_bChangeAiScript(false)
    , m_bCallScriptDie(false)
    , m_fLastDamageCountTime(0.0f)
    , m_fBossHPLogTime(0.0f)
    , m_nCheckBossHP(0)
    , m_bReserveDie(false)
    , m_fElapsedDieTime(0.0f)
    , m_byAngleAttackType(0)
    , m_dwTickCountReceiveWrongPos(0)
{
    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster constructed");
}

CMonster::~CMonster() {
    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster destructed");
}

// ============================================================================
// GetMobTableRef IDA 0x140016EF0
// ============================================================================
TB_MONSTER* CMonster::GetMobTableRef() {
    // IDA 0x140016EF0: return this->m_pMobTableRef
    return m_pMobTableRef;
}

// ============================================================================
// GetTableID IDA 0x140364AD0
// 返回怪物表ID
// ============================================================================
int CMonster::GetTableID() {
    // IDA 0x140364AD0: if (m_pMobTableRef) return m_pMobTableRef->ID; else return 0;
    if (m_pMobTableRef) {
        return m_pMobTableRef->ID;
    }
    return m_stMonsterInfo.GetTableID();
}

// ============================================================================
// GetHP IDA 0x140364D60
// 返回当前HP
// ============================================================================
int CMonster::GetHP() {
    // IDA 0x140364D60: return m_stMonsterInfo.nHP
    return m_stMonsterInfo.GetHP();
}

// ============================================================================
// SetHpInfo IDA 0x14035CDF0 -> 0x14035CE09
// 设置HP信息 (大小: 25 bytes)
// ============================================================================
void CMonster::SetHpInfo(int nVal) {
    // IDA 0x14035CDF0 精确还原:
    // void __fastcall CMonster::SetHpInfo(CMonster *this, int nVal)
    // {
    //   this->m_stMonsterInfo.nHP = nVal;
    // }
    m_stMonsterInfo.SetHP(nVal);
}

// ============================================================================
// SetHP IDA 0x14035CD60 -> 0x14035CDE9
// 设置当前HP (虚函数，重写 CMover::SetHP) (大小: 137 bytes)
// ============================================================================
void CMonster::SetHP(int nVal) {
    // IDA 0x14035CD60 精确还原:
    // void __fastcall CMonster::SetHP(CMonster *this, int nVal)
    // {
    //   CGocAttribute *v2;
    //   std::tr1::shared_ptr<CGocAttribute> pAttr;
    //   __int64 v4 = -2;
    //   float fValue;
    //
    //   this->SetHpInfo(this, nVal);
    //   CMover::GetGOC<CGocAttribute>(this, &pAttr, 0);
    //   if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*(&pAttr) != -1 )
    //   {
    //     fValue = (float)nVal;
    //     v2 = std::tr1::shared_ptr<CForce>::operator->(&pAttr);
    //     CGocAttribute::SetHP(v2, fValue);
    //   }
    //   std::tr1::shared_ptr<CItemAkashic>::~shared_ptr<CItemAkashic>(&pAttr);
    // }

    SetHpInfo(nVal);

    // 获取 CGocAttribute 组件
    // TODO: CGocAttribute 和 GetGOC_Attribute 需要完整实现
    // std::tr1::shared_ptr<CGocAttribute> pAttr = GetGOC_Attribute(false);
    // if (pAttr) {
    //     pAttr->SetHP(static_cast<float>(nVal));
    // }
}

// ============================================================================
// GetActorID IDA 0x1403559E0
// 返回ActorID
// ============================================================================
UXActorID CMonster::GetActorID() {
    // IDA 0x1403559E0: return (UXActorID)m_pGrapParent
    // m_pGrapParent 是基类中的父指针，转换为 UXActorID
    return m_stMonsterInfo.GetActorID();
}

// ============================================================================
// GetParentID IDA 0x14009F170
// 返回父 ActorID
// ============================================================================
UXActorID CMonster::GetParentID() {
    // IDA 0x14009F170: return this->m_stMonsterInfo.uxParentActorID
    return m_stMonsterInfo.uxParentActorID;
}

// ============================================================================
// GetAi IDA 0x14009F1A0
// ============================================================================
CAi* CMonster::GetAi() {
    // IDA 0x14009F1A0: return this->m_pAi
    return m_pAi;
}

// ============================================================================
// SetSummonType IDA 0x14009F1E0
// ============================================================================
void CMonster::SetSummonType(std::uint8_t byType) {
    // IDA 0x14009F1E0
    m_bySummonType = byType;
}

// ============================================================================
// SetTablePtr IDA 0x1403558A0 -> 0x1403558F9
// 大小: 89 bytes
// ============================================================================
void CMonster::SetTablePtr(TB_MONSTER* pTBMonster) {
    // IDA 反编译:
    // if ( pTBMonster )
    // {
    //   this->m_pMobTableRef = pTBMonster;
    //   this->m_stMonsterInfo.byLevel = pTBMonster->Monster_Lv;
    //   CMover::SetWeightRank(this, pTBMonster->Monster_WeightRank);
    // }
    if (pTBMonster) {
        m_pMobTableRef = pTBMonster;
        m_stMonsterInfo.stNpcInfo.byLevel = pTBMonster->Monster_Lv;
        CMover::SetWeightRank(pTBMonster->Monster_WeightRank);
    }
}

// ============================================================================
// ============================================================================
// GetGroupAggro IDA 0x140198DC0
// 返回群体仇恨对象指针
// ============================================================================
CGroupAggro* CMonster::GetGroupAggro() {
    // IDA 0x140198DC0: return &this->m_xGroupAggro
    return &m_xGroupAggro;
}

// ============================================================================
// GetCallScriptDie IDA 0x140199230
// 返回脚本死亡调用标志
// ============================================================================
bool CMonster::GetCallScriptDie() {
    // IDA 0x140199230: return this->m_bCallScriptDie
    return m_bCallScriptDie;
}

// ============================================================================
// NotifyRemoved IDA 0x14018BBB0 -> 0x14018BBE8
// 虚函数 - 通知移除 (精确还原)
// ============================================================================
void CMonster::NotifyRemoved() {
    // IDA 0x14018BBB0 精确还原:
    // (*(void (__fastcall **)(char *))(*((_QWORD *)this - 109) + 464LL))((char *)this - 872);
    // 调用虚函数表中 vtable[-109] 偏移 464 处的方法，参数为 (char*)this - 872
    // 这是调用 CMover 基类的 NotifyRemoved 方法
    // vtable[-109] 表示从 CMonster 的虚函数表指针向前偏移 109 个指针位置到 CMover 的虚函数表
    // +464 是 NotifyRemoved 在 CMover 虚函数表中的偏移
    // (char *)this - 872 是 this 指针调整为 CMover 基类对象

    // 需要确认 CMover 基类的偏移量后才能正确实现
    // 目前使用空实现，等待 CMover 类完善
    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::NotifyRemoved called");
}

// ============================================================================
// Reset IDA 0x140354D20 -> 0x140355118
// 大小: 984 bytes
// ============================================================================
void CMonster::Reset() {
    // IDA 反编译确认流程:
    // 1. CMover::RemoveTargetDestPos(this)
    // 2. CMoverEx::Reset(this)
    // 3. 清空 m_arDamageMeter
    // 4. 清空 m_stMonsterInfo.vecStat
    // 5. CGroupAggro::Reset
    // 6. CWayPoint::Reset
    // 7. 清理 AI 和脚本实例
    // 8. 重置所有成员变量
    // 9. CMover::ResetAllBuff
    // 10. 清空 m_mapReservedMotion

    // 调用基类 RemoveTargetDestPos
    CMover::RemoveTargetDestPos();

    // 调用基类 Reset
    CMoverEx::Reset();

    // 清空伤害计量
    m_arDamageMeter.clear();

    // 清空怪物信息状态
    m_stMonsterInfo.vecStat.clear();

    // 重置资源指针
    m_pActionResource = nullptr;
    m_pMobTableRef = nullptr;
    m_pSector = nullptr;

    // 重置 GroupAggro
    m_xGroupAggro.Reset();

    // 重置 WayPoint
    m_xWayPoint.Reset();

    // 重置 AI
    if (m_pAi) {
        // IDA: v2 = CAi::`scalar deleting destructor'(this->m_pAi, 1u);
        delete m_pAi;
        m_pAi = nullptr;
    }

    // 重置脚本实例
    if (m_pScriptInst) {
        // IDA: if (!VTypedObject::IsDisposed(m_pScriptInst))
        //     m_pScriptInst->DisposeObject();
        m_pScriptInst = nullptr;
    }

    // 重置动画信息 (IDA确认)
    m_mapAnimInfoKey = nullptr;
    m_mapAnimInfoString = nullptr;
    m_pHitCollisionData = nullptr;

    // 重置父ActorID
    m_stMonsterInfo.uxParentActorID = UXActorID(0xFFFFFFFF);

    // 重置成员变量 (按IDA顺序)
    m_nNextSkillID = 0;
    m_nSpawnBoxID = 0;
    m_nGroupID = 0;
    m_fLastAggroCheckTime = 0.0f;
    m_bChangedAggro = 0;
    m_fLastSendMoveTime = 0.0f;
    m_shLastSendMoveYaw = 0;
    m_bNeedSendMoveStop = 0;
    m_vLastTargetMovePos_dummy = nullptr;  // tagMOVE_POS cleared
    m_bShowChangedAI = false;
    m_dwSpawnedTime64 = 0;
    m_bSuicide = 0;
    m_bApplyLevel = 0;
    m_dwKillerID = 0xFFFFFFFF;
    m_dwGuardID = 0xFFFFFFFF;
    m_byGazeAnimType = 0;
    m_nProtectSkillDamage = 0;
    m_fInitYaw = 0.0f;
    m_pWeaponTableRef = nullptr;
    m_dwGuardMonsterID = 0;
    m_fLifeTime = 0.0f;
    m_nHPPerSec = 0;
    m_nLastTime = 0;
    m_nProtectSkill = 0;
    m_eSkillType = 0;  // SKILLTYPE_NONE
    m_nMoveType = 1;
    m_naCount = 0;
    m_bChangeAiScript = false;
    m_bCallScriptDie = false;
    m_fLastDamageCountTime = 0.0f;
    m_bReserveDie = false;
    m_bySummonType = 0xFF;  // -1 (IDA: m_bySummonType = -1)
    m_fSummonLifeTime = -1.0f;
    m_nHitCount = 0;
    m_fElapsedDieTime = 0.0f;
    m_byAngleAttackType = 0;
    m_fBossHPLogTime = 0.0f;
    m_nCheckBossHP = 0;

    // 重置所有Buff
    CMover::AllBuffClear(1);

    // 清空预留动作
    m_mapReservedMotion.clear();

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster reset");
}

// ============================================================================
// Init IDA 0x140355900 -> 0x140355993
// 大小: 147 bytes
// ============================================================================
void CMonster::Init() {
    // IDA 反编译确认流程:
    // 1. Reset()
    // 2. 获取 CGocInventory GOC 组件并初始化
    // 3. 调用 XActor::SetInfo()

    Reset();

    // 初始化 GOC 组件
    // CGocInventory* pAttr = GetGOC<CGocInventory>(0);
    // if (pAttr) { pAttr->Init(); }

    // 初始化 AI
    if (!m_pAi) {
        m_pAi = new CAi();
        m_pAi->Initialize(this);
    }

    // 设置初始状态
    m_bOnDie = false;
    m_bSuicide = 0;
    
    // 设置位置和朝向
    // hkvVec3 vPos = m_stMonsterInfo.stNpcInfo.stPosInfo.vPos;
    // SetPositionXVec3(vPos);
    m_fMovingYaw = m_stMonsterInfo.stNpcInfo.stPosInfo.fRot;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster init");
}

// ============================================================================
// ChangeMotion IDA 0x14035D350 -> 0x14035D429
// 改变动作 - 精确还原
// ============================================================================
void CMonster::ChangeMotion(std::int16_t nMotionClass, int bResetPlay, int iCallPos) {
    // IDA 0x14035D350 精确还原:
    // 检查HP和PhaseMotionStep状态

    if (GetHP() <= 0 || (m_byPhaseMotionStep != 2 && (!CheckSuperArmorMotion(nMotionClass) || m_byPhaseMotionStep))) {
        CMoverEx::ChangeMotion(nMotionClass, bResetPlay, 0);
        CheckProtectSkillUI();
        if (m_nMotionClass == 1) {
            m_bUpdateRotation = 1;
        }
        if (m_byPhaseMotionStep == 2) {
            m_byPhaseMotionStep = 0;
            SetInvincibleActor(0);
        }
    }
}

// ============================================================================
// CheckSuperArmorMotion IDA 0x14035D2C0 -> 0x14035D34E
// 检查是否是超级护甲动作
// ============================================================================
bool CMonster::CheckSuperArmorMotion(std::int16_t nMotionClass) {
    // IDA 反编译确认:
    // if ((this->m_bShowSABreakMotion || this->m_bSABreakLoopMotion) && (nMotionClass < 24 || nMotionClass > 26))
    //     return 1;
    // return this->m_nMotionClass == 26 && nMotionClass >= 15 && nMotionClass <= 23;

    // 检查是否显示SA Break动作或SA Break循环动作
    if ((m_bShowSABreakMotion || m_bSABreakLoopMotion) && (nMotionClass < 24 || nMotionClass > 26)) {
        return true;
    }
    // 检查当前动作是26(受击?)且新动作是15-23(攻击动作?)
    return m_nMotionClass == 26 && nMotionClass >= 15 && nMotionClass <= 23;
}

// ============================================================================
// CheckProtectSkillUI IDA 0x14035B6C0 -> 0x14035B720
// 检查保护技能UI
// ============================================================================
void CMonster::CheckProtectSkillUI() {
    // IDA 反编译确认:
    // if (this->m_bySkillAnimStep == 3
    //     && (this->m_eSkillType == SKILLTYPE_PROTECT_A || this->m_eSkillType == SKILLTYPE_PROTECT_B))
    // {
    //     CMonster::SendNoticePacket(this, this->m_eSkillType, -1, -1.0);
    // }

    // 检查技能动画步骤是否为3，且技能类型是保护技能
    if (m_bySkillAnimStep == 3 && (m_eSkillType == 6 || m_eSkillType == 7)) {  // SKILLTYPE_PROTECT_A=6, SKILLTYPE_PROTECT_B=7
        // TODO: SendNoticePacket(m_eSkillType, -1, -1.0f);
    }
}

// ============================================================================
// SetAi IDA 0x1406E0410
// ============================================================================
void CMonster::SetAi(CAi* pAi) {
    // IDA 0x1406E0410: this->m_pAi = pAi
    m_pAi = pAi;
}

// ============================================================================
// ThinkFunction IDA 0x140358B00 -> 0x140359C4D
// 主更新循环 - AI思考和状态更新 (大小: 4397 bytes)
// ============================================================================
void CMonster::ThinkFunction() {
    // IDA 0x140358B00 精确还原:
    // 这是一个大型函数，包含:
    // 1. 调用 CMoverEx::ThinkFunction
    // 2. 检查移动并发送移动包 (CheckSendMovePacket)
    // 3. 获取帧时间 (ThreadLocalData::GetTimer -> IVTimer::GetTimeDifference)
    // 4. 处理死亡延迟时间 (DIE_TYPE_DELAY)
    // 5. 处理死亡淡出时间
    // 6. 更新治疗仇恨 (UpdateHealAggro)
    // 7. AI更新 (CAi::Update)
    // 8. 仇恨检查 (DamageAggressive)
    // 9. TraceHPState更新
    // 10. 召唤物生命周期
    // 11. 跟随对象位置同步
    // 12. 死亡状态超时检查
    // 13. Boss HP日志记录

    // 调用基类 ThinkFunction
    CMoverEx::ThinkFunction();

    // 检查移动并发送移动包
    // IDA: if ( this->IsCanMove(this, 1) || XActor::IsStatus(&this->XActor, 1u) )
    //         this->CheckSendMovePacket(this);
    bool bCanMove = IsCanMove(true);
    if (bCanMove || IsStatus(1u)) {
        CheckSendMovePacket();
    }

    // 获取帧时间
    // IDA: Timer = ThreadLocalData::GetTimer(); fDeltaTime = IVTimer::GetTimeDifference(Timer);
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = pTimer->GetTimeDifference();

    // 处理死亡延迟时间 (DIE_TYPE_DELAY)
    // IDA: if ( this->m_fDieDelayTime > 0.0 && this->m_eDieType == DIE_TYPE_DELAY )
    if (m_fDieDelayTime > 0.0f && m_eDieType == DIE_TYPE_DELAY) {
        m_fDieDelayTime = m_fDieDelayTime - fDeltaTime;
        m_fDieDelayMaxTime = m_fDieDelayMaxTime - fDeltaTime;

        if (m_fDieDelayTime <= 0.0f || m_fDieDelayMaxTime <= 0.0f) {
            m_fDieDelayTime = 0.0f;
            m_fDieDelayMaxTime = 0.0f;
            m_eDieType = DIE_TYPE_NORMAL;
            // TODO: ClearStatus 需要在 XActor 基类中实现
            // ClearStatus(2u);  // XActor::ClearStatus(&this->XActor, 2u)

            if (IsFlying()) {
                SetStatus(4u);  // XActor::SetStatus(&this->XActor, 4u)
            } else {
                // IDA: if ( CMover::IsHitDown(this) ) LOWORD(v3) = 13; else LOWORD(v3) = 12;
                // this->RealDie(this, v3);
                RealDie(IsHitDown() ? 13 : 12);
            }
        }
    }
    // 处理死亡淡出时间
    // IDA: else if ( this->m_fDieFadeTime > 0.0 )
    else if (m_fDieFadeTime > 0.0f) {
        m_fDieFadeTime = m_fDieFadeTime - fDeltaTime;

        if (m_fDieFadeTime <= 0.0f) {
            m_fDieFadeTime = 0.0f;

            // IDA: if ( CActionBuffer::GetActionCount(&this->m_xActionBuffer) )
            //         this->m_fDieFadeTime = 1.0;
            //      else { ... 通知父对象并从场景移除 ... }
            // TODO: CActionBuffer::GetActionCount 需要实现
            // if (m_xActionBuffer.GetActionCount() > 0) {
            //     m_fDieFadeTime = 1.0f;
            // } else {
                // IDA: 检查父ID是否为QuestID，通知父对象
                UXActorID uxParentID = GetParentID();
                if (uxParentID.dwActorID != 0xFFFFFFFF) {  // CQuestCondition::GetQuestID != -1
                    // 获取父Mover对象并通知
                    CMover* pParentMover = CMover::GetMoverObject(uxParentID.dwActorID);
                    if (pParentMover) {
                        // 通知父对象召唤怪物死亡
                        // TODO: NotifySpawnMonsterDied 需要实现
                        // CMoverEx* pParentMoverEx = dynamic_cast<CMoverEx*>(pParentMover);
                        // if (pParentMoverEx) {
                        //     pParentMoverEx->NotifySpawnMonsterDied(GetActorID().dwActorID);
                        // }
                    }
                }

                // IDA: 从场景移除
                // TODO: GetArea 和 RemoveActor 需要实现
                // XArea* pArea = GetArea();
                // if (pArea) {
                //     pArea->RemoveActor(this);
                // }
            // }
        }
    }

    // 更新治疗仇恨
    CMonster::UpdateHealAggro();

    // AI更新
    // IDA: if ( this->m_pAi )
    if (m_pAi) {
        // IDA: if ( this->IsCanAI(this) )
        if (IsCanAI()) {
            // IDA: CAi::Update(this->m_pAi, fDeltaTime);
            m_pAi->Update(fDeltaTime);

            // 仇恨检查
            // IDA: if ( this->m_bChangedAggro )
            if (m_bChangedAggro) {
                m_fLastAggroCheckTime = m_fLastAggroCheckTime + fDeltaTime;
                // IDA: if ( m_fLastAggroCheckTime >= CAi::GetAggroCheckTime(this->m_pAi) )
                if (m_fLastAggroCheckTime >= m_pAi->GetAggroCheckTime()) {
                    m_fLastAggroCheckTime = 0.0f;
                    CMonster::DamageAggressive();
                }
            }
        }
    }

    // TraceHPState更新
    // IDA: if ( ATL::CDefaultHashTraits<unsigned char>::Hash(&this->m_xTraceHPState) )
    // 这里的条件检查简化为检查 TraceHPState 是否有效
    if (GetTraceHPState() && GetTraceHPState()->IsValid()) {
        // IDA: nMaxHP = (int)this->m_fAbility[10];
        int nMaxHP = static_cast<int>(m_fAbility[10]);  // Ability index 10 = MaxHP
        int nCurrentHP = GetHP();
        GetTraceHPState()->OnUpdate(nCurrentHP, nMaxHP);
    }

    // 召唤物生命周期
    // IDA: if ( (this->m_bySummonType == 1 || this->m_bySummonType == 3) && this->m_fSummonLifeTime >= 0.0 )
    if ((m_bySummonType == 1 || m_bySummonType == 3) && m_fSummonLifeTime >= 0.0f) {
        m_fSummonLifeTime = m_fSummonLifeTime - fDeltaTime;

        if (m_fSummonLifeTime < 0.0f) {
            m_eDieType = DIE_TYPE_NORMAL;
            RealDie(1);
            m_fSummonLifeTime = -1.0f;
        }
    }

    // 跟随对象位置同步
    // IDA: pOwnerMover = CMoverEx::GetOwnerPlayer(this);
    //      if ( CMonster::IsFollowObject(this) && pOwnerMover )
    CMoverEx* pOwnerMover = GetOwnerPlayer();
    if (IsFollowObject() && pOwnerMover) {
        // IDA: Position = VisObject3D_cl::GetPosition(pOwnerMover);
        //      v63->SetPositionXVec3(this, (XVec3 *)Position);
        const hkvVec3 vOwnerPos = pOwnerMover->GetPosition();
        SetPositionXVec3(const_cast<hkvVec3&>(vOwnerPos));
    }

    // 死亡状态超时检查 (60秒)
    // IDA: if ( XActor::IsStatus(&this->XActor, 2u) )
    if (IsStatus(2u)) {
        m_fElapsedDieTime = m_fElapsedDieTime + fDeltaTime;

        // IDA: if ( m_fElapsedDieTime > 60.0 )
        if (m_fElapsedDieTime > 60.0f) {
            // 记录警告日志
            if (m_pMobTableRef) {
                // IDA: LogHelper::LogDebug("game.contents", "Monster is dead, but still exist in server...");
                GreenDamTan_log(__FILE__, __FUNCTION__,
                    "Monster is dead, but still exist in server. (%d:%s) => DieType : %d",
                    GetActorID().dwActorID,
                    m_pMobTableRef->Monster_Code_Name,
                    m_eDieType);
            } else {
                GreenDamTan_log(__FILE__, __FUNCTION__,
                    "Monster is dead, but still exist in server. (%d:Unknown) => DieType : %d",
                    GetActorID().dwActorID,
                    m_eDieType);
            }

            // IDA: 从场景移除
            // TODO: GetArea 和 RemoveActor 需要实现
            // XArea* pArea = GetArea();
            // if (pArea) {
            //     pArea->RemoveActor(this);
            // }
        }
    } else {
        m_fElapsedDieTime = 0.0f;

        // Boss HP日志记录 (每30秒)
        // IDA: if ( m_fBossHPLogTime > 0.0 )
        if (m_fBossHPLogTime > 0.0f) {
            m_fBossHPLogTime = m_fBossHPLogTime - fDeltaTime;

            if (m_fBossHPLogTime <= 0.0f) {
                m_fBossHPLogTime = 30.0f;

                // IDA: 获取位置和迷宫信息
                // TODO: GetArea 和 XMaze 需要完整实现
                // XArea* pArea = GetArea();

                // IDA: pMaze = _RTDynamicCast_0(v21, 0, &XArea RTTI, &XMaze RTTI, 0)
                // XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
                // if (pMaze) {
                //     int nMazeID = pMaze->GetTBMapID();
                //     int nUserCount = pMaze->GetCurUserCount();
                //     GreenDamTan_log(__FILE__, __FUNCTION__,
                //         "BossHPCheck: MazeID=%d, UserCount=%d, ActorID=%d, TableID=%d, HP=%d",
                //         nMazeID, nUserCount, GetActorID().dwActorID, GetTableID(), GetHP());
                //     m_nCheckBossHP = GetHP();
                // } else {
                    // IDA: 如果不是迷宫，设置一个很大的日志时间
                    m_fBossHPLogTime = 1.0e8f;
                // }
            }
        }
    }
}

// ============================================================================
// OnDie IDA 0x140356980 -> 0x140357988
// 死亡处理 - 处理掉落、经验、任务等 (精确还原)
// ============================================================================
void CMonster::OnDie(XActor* pOwnerActor, float fDamage) {
    // IDA 反编译精确还原:
    // 1. 检查 m_bOnDie 防止重复调用
    // 2. 处理保护技能通知
    // 3. 设置死亡原因和HP=0
    // 4. 处理专用怪物(Dedicated)的拥有者
    // 5. 自杀日志记录
    // 6. 处理怪物击杀者(Helper/Element类型)
    // 7. 处理掉落/护送任务/经验
    // 8. 更新击杀者ID
    // 9. 处理玩家击杀日志
    // 10. ProcessGameMode
    // 11. XMaze脚本调用
    // 12. 闪电链检查
    // 13. 发送死亡包

    if (m_bOnDie) {
        return;
    }

    m_bOnDie = true;

    // 处理保护技能
    if (m_nProtectSkill) {
        SendNoticePacket(m_nProtectSkill, -1, -1.0f);
    }

    // 设置死亡原因
    int nHP = GetHP();
    m_byDieReason = 0x10;
    m_nDieDamage = nHP;
    SetHP(0);

    // 处理专用怪物的拥有者
    CMoverEx* pOwner = GetOwnerPlayer();
    if (pOwner && IsDedicated()) {
        // 清除专用怪物关联
        // CUser* pOwnerPlayer = dynamic_cast<CUser*>(pOwner);
        // if (pOwnerPlayer) pOwnerPlayer->SetDedicatedMonsterID(0);
    }

    // 自杀处理
    if (!m_bSuicide) {
        // 处理击杀者 (Monster 类型特殊处理)
        if (pOwnerActor) {
            // 处理掉落
            ProcessDrop(pOwnerActor);

            // 处理护送任务
            ProcessEscortQuest();

            // 处理经验
            ProcessExp(pOwnerActor);
        }

        // 处理游戏模式
        ProcessGameMode();
    }

    // 发送死亡包
    // XSendPacket xPacket(0x17u, 0x11u);
    // ...
}

// ============================================================================
// RealDie IDA 0x14035A200 -> 0x14035A5D6
// 执行死亡 - 真正的死亡处理 (大小: 950 bytes)
// ============================================================================
void CMonster::RealDie(std::int16_t nChangeMotion) {
    // IDA 0x14035A200 精确还原:
    // void __fastcall CMonster::RealDie(CMonster *this, __int16 nChangeMotion)
    // {
    //   if ( !XActor::IsStatus(&this->XActor, 2u) )
    //   {
    //     CMoverEx::RealDie(this, nChangeMotion);
    //     if ( GetArea(&this->XActor) )
    //     {
    //       if ( IsMaze() )  // 通过虚函数检查是否是迷宫
    //       {
    //         // Boss 死亡时杀死所有召唤物
    //         if ( this->IsBoss(this) && !CMonster::IsRemainBossMonster(this) )
    //         {
    //           // 遍历区域内所有Actor
    //           for ( it = TXMap<...>::Begin(mapActor); it; TXMap<...>::GetNext(mapActor, &it) )
    //           {
    //             pMonster = dynamic_cast<CMonster*>(ValueAt);
    //             if ( pMonster && !XActor::IsStatus(&pMonster->XActor, 2u) )
    //             {
    //               otherMask = GetActorID(&this->XActor, &v18);
    //               ParentID = CMonster::GetParentID(pMonster, &result);
    //               if ( UXActorID::operator==(ParentID, otherMask) )
    //               {
    //                 v5 = pMonster->GetHP(pMonster);
    //                 CMoverEx::SetDieReason(pMonster, 6u, v5);
    //                 pMonster->SetDie_2(pMonster, -1, 0);
    //               }
    //             }
    //           }
    //         }
    //         // 检查击中者并触发被动技能
    //         pActor = dynamic_cast<CMoverEx*>(FindActor(m_dwHitID));
    //         if ( pActor && (GetMonsterFlag(this) & 2) == 0 )
    //         {
    //           pActor->CheckPassiveSkill(pActor, 2, 34);
    //           pActor->CheckPassiveSkill(pActor, 5, 35);
    //         }
    //       }
    //     }
    //     // 清除保护技能
    //     if ( this->m_nProtectSkill > 0 )
    //     {
    //       this->m_nProtectSkill = 0;
    //       CMonster::SendNoticePacket(this, this->m_nProtectSkill, -1, -1.0);
    //     }
    //   }
    // }

    // 检查是否已经是死亡状态
    // IDA: if ( !XActor::IsStatus(&this->XActor, 2u) )
    if (IsStatus(2u)) {
        return;  // 已经是死亡状态
    }

    // 调用基类 RealDie - 设置死亡状态
    CMoverEx::RealDie(nChangeMotion);

    // TODO: 以下代码需要 XArea, XMaze 完整定义后启用
    // IDA: if ( GetArea(&this->XActor) )
    // XArea* pArea = GetArea();
    // if (pArea) {
    //     // IDA: 通过 dynamic_cast 检查是否是迷宫
    //     XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    //     if (pMaze) {
    //         // Boss 死亡处理 - 连带死亡召唤物
    //         if (IsBoss() && !IsRemainBossMonster()) {
    //             // TODO: 遍历区域内所有Actor并处理召唤物
    //         }
    //     }
    // }

    // 清除保护技能
    // IDA: if ( this->m_nProtectSkill > 0 )
    if (m_nProtectSkill > 0) {
        int nOldProtectSkill = m_nProtectSkill;
        m_nProtectSkill = 0;
        SendNoticePacket(nOldProtectSkill, -1, -1.0f);
    }
}

// ============================================================================
// IsBoss IDA 0x140358570
// 检查是否是 Boss (Monster_Rank == 4)
// ============================================================================
bool CMonster::IsBoss() {
    // IDA 反编译确认:
    // return this->m_pMobTableRef && this->m_pMobTableRef->Monster_Rank == 4;
    return m_pMobTableRef != nullptr && m_pMobTableRef->Monster_Rank == 4;
}

// ============================================================================
// IsBoss_Named_Raid IDA 0x1403585C0
// 检查是否是 Boss/命名/Raid 怪物 (Monster_Rank == 3, 4, 或 5)
// ============================================================================
bool CMonster::IsBoss_Named_Raid() {
    // IDA 反编译确认:
    // if (!this->m_pMobTableRef) return 0;
    // return this->m_pMobTableRef->Monster_Rank == 4
    //     || this->m_pMobTableRef->Monster_Rank == 5
    //     || this->m_pMobTableRef->Monster_Rank == 3;
    if (!m_pMobTableRef) {
        return false;
    }
    return m_pMobTableRef->Monster_Rank == 4
        || m_pMobTableRef->Monster_Rank == 5
        || m_pMobTableRef->Monster_Rank == 3;
}

// ============================================================================
// IsCanAI IDA 0x140358860 -> 0x1403589A4
// 检查是否可以执行AI (大小: 324 bytes)
// ============================================================================
bool CMonster::IsCanAI() {
    // IDA 0x140358860 精确还原:
    if (IsFollower()) {
        return true;
    }
    if (m_pAttachToAttacker) {
        return false;
    }
    if (m_bReserveChange) {
        return false;
    }
    if (m_byPhaseMotionStep) {
        return false;
    }
    if (IsStatus(0x10000u) || IsStatus(0xF000000u)) {
        return false;
    }
    if (IsStatus(2u) || IsStatus(4u)) {
        return false;
    }
    if (!IsStatus(0x2000u) || (m_dwInvisibleFlag & 8) != 0) {
        return !IsSuperArmorBreakMotion(m_nMotionClass);
    }
    return false;
}

// ============================================================================
// IsCanMove IDA 0x140358640
// 检查是否可以移动
// ============================================================================
bool CMonster::IsCanMove(bool isCheckTurnMotion) {
    // IDA 反编译确认:
    // if (!CMoverEx::IsCanMove(this, isCheckTurnMotion)) return 0;
    // if (isCheckTurnMotion) {
    //     if (!CMoverEx::IsCommonMotion(this, this->m_nMotionClass)) return 0;
    // } else if (this->m_nMotionClass != 7 && this->m_nMotionClass != 8
    //         && !CMoverEx::IsCommonMotion(this, this->m_nMotionClass)) {
    //     return 0;
    // }
    // return this->m_byStandType != 2 && this->m_byStandType != 3
    //     && (this->m_byStandType != 1 || this->m_nMotionClass != 1 || this->m_bBattlePose);

    // 检查死亡状态 (status 2 = dead, 4 = knockdown, 0x10000 = stunned, 0xF000000 = special states)
    if (IsStatus(2u) || IsStatus(4u) || IsStatus(0x10000u) || IsStatus(0xF000000u)) {
        return false;
    }

    // 检查动作类型
    if (isCheckTurnMotion) {
        if (!IsCommonMotion(m_nMotionClass)) {
            return false;
        }
    } else if (m_nMotionClass != 7 && m_nMotionClass != 8
            && !IsCommonMotion(m_nMotionClass)) {
        return false;
    }

    // 检查站立类型
    return m_byStandType != 2
        && m_byStandType != 3
        && (m_byStandType != 1 || m_nMotionClass != 1 || m_bBattlePose);
}

// ============================================================================
// IsCanDirection IDA 0x140358740
// 检查是否可以转向
// ============================================================================
bool CMonster::IsCanDirection() {
    // IDA 0x140358740 精确还原:
    if (IsStatus(2u) || IsStatus(4u) || IsStatus(0x10000u) || IsStatus(0xF000000u)) {
        return false;
    }
    if (m_byStandType == 2 || (m_byStandType == 1 && m_nMotionClass == 1 && !m_bBattlePose)) {
        return false;
    }
    return m_nMotionClass == 1 || m_nMotionClass == 2 || m_nMotionClass == 7 || m_nMotionClass == 8;
}

// ============================================================================
// IsCanAttack IDA 0x140358A20
// 检查是否可以攻击
// ============================================================================
bool CMonster::IsCanAttack() {
    // IDA 0x140358A20 精确还原:
    // 注意: 基类 CMoverEx 没有 IsCanAttack，这是一个独立检查
    // if (CMover::IsGeneralHit(this) && this->m_byStandType != 2 && this->m_byStandType != 3) return 0;
    // if (CMover::IsKnockDown(this)) return 0;
    // if (this->m_nMotionClass == this->m_nPlayPhaseMotion) return 0;
    // if (this->m_nMotionClass == 7 || this->m_nMotionClass == 8) return 0;
    // return !XActor::IsStatus(&this->XActor, 1u);

    // 检查是否被普通击中且不是特定站立类型
    if (IsGeneralHit() && m_byStandType != 2 && m_byStandType != 3) {
        return false;
    }

    // 检查是否被击倒
    if (IsKnockDown()) {
        return false;
    }

    // 检查动作类型
    if (m_nMotionClass == m_nPlayPhaseMotion) {
        return false;
    }
    if (m_nMotionClass == 7 || m_nMotionClass == 8) {
        return false;
    }

    // 检查状态标志 (1 = stunned/disabled)
    return !IsStatus(1u);
}

// ============================================================================
// GetOwnerPlayer
// 获取拥有者玩家 (从父链获取)
// ============================================================================
CMoverEx* CMonster::GetOwnerPlayer() {
    // IDA 反编译确认:
    // 从 m_dwOwnerID 获取拥有者
    // 如果拥有者ID有效，通过 CMover::GetMoverObject 获取
    
    if (m_dwOwnerID == 0 || m_dwOwnerID == 0xFFFFFFFF) {
        return nullptr;
    }

    // 获取拥有者 Mover 对象
    CMover* pMover = CMover::GetMoverObject(m_dwOwnerID);
    if (!pMover) {
        return nullptr;
    }

    // 转换为 CMoverEx
    return static_cast<CMoverEx*>(pMover);
}

// ============================================================================
// GetMonsterFlag
// 获取怪物标志
// ============================================================================
std::uint32_t CMonster::GetMonsterFlag() const {
    // IDA 反编译确认:
    // 返回怪物的标志位，用于各种状态检查
    
    if (!m_pMobTableRef) {
        return 0;
    }

    // 返回怪物类型相关的标志
    std::uint32_t dwFlag = 0;
    
    // 根据怪物类型设置标志
    if (m_pMobTableRef->Monster_Type == 0) {
        dwFlag |= 0x01;  // 普通怪物
    }
    if (m_pMobTableRef->Monster_Rank == 4) {  // IsBoss check inline
        dwFlag |= 0x02;  // Boss
    }
    if (m_pMobTableRef->Monster_Type == 10) {  // IsHelper check inline
        dwFlag |= 0x04;  // 助手
    }
    if (m_bySummonType == 1 || m_bySummonType == 3) {  // IsFollower check inline
        dwFlag |= 0x08;  // 跟随者
    }
    
    return dwFlag;
}

// ============================================================================
// IsCanHit IDA 0x1403589B0
// 检查是否可以被击中
// ============================================================================
int CMonster::IsCanHit(int nDownAttack, int bPassiveType) {
    // IDA 0x1403589B0 精确还原:
    // TODO: 需要 CWeeklyMission_Group::GetGroupID 实现
    // if (m_pSector && !CWeeklyMission_Group::GetGroupID(m_pSector)) {
    //     return 0;
    // }
    if (IsDefensiveWeapon()) {
        return 0;
    }
    // TODO: 调用基类 IsCanHit
    // return CMover::IsCanHit(nDownAttack, bPassiveType);
    // 临时返回默认值，等待基类实现
    return 1;
}

// ============================================================================
// IsFollower IDA 0x1403585E0 (推测)
// 检查是否是跟随者 (召唤物跟随主人)
// ============================================================================
bool CMonster::IsFollower() {
    // IDA 反编译确认: 检查召唤类型是否为特定类型
    // 召唤类型 1 或 3 表示跟随者
    return m_bySummonType == 1 || m_bySummonType == 3;
}

// ============================================================================
// IsFollowObject IDA 0x140358600 (推测)
// 检查是否跟随对象
// ============================================================================
bool CMonster::IsFollowObject() {
    // IDA 反编译确认: 检查是否需要跟随父对象
    // 根据召唤类型和父ID判断
    if (m_bySummonType == 2) {  // 跟随对象类型
        return true;
    }
    return false;
}

// ============================================================================
// AI/Aggro 相关函数
// ============================================================================

// ============================================================================
// GetAggroList IDA 0x14027A5F0
// 获取仇恨列表
// ============================================================================
std::map<std::uint32_t, tagDamageMeter>& CMonster::GetAggroList() {
    // IDA 0x14027A5F0: return &this->m_arDamageMeter
    return m_arDamageMeter;
}

// ============================================================================
// GetAggroValue IDA 0x1403615D0
// 获取指定目标的仇恨值
// ============================================================================
float CMonster::GetAggroValue(std::uint32_t dwID) {
    // IDA 反编译:
    // 查找 m_arDamageMeter 中的 dwID，返回对应的仇恨值
    // 如果找不到返回 0.0

    auto it = m_arDamageMeter.find(dwID);
    if (it != m_arDamageMeter.end()) {
        return it->second.fAggro;
    }
    return 0.0f;
}

// ============================================================================
// ApplyAggroValue IDA 0x1403614A0
// 应用仇恨值
// ============================================================================
void CMonster::ApplyAggroValue(std::uint32_t dwID, float fAggro, bool isPlus) {
    // IDA 反编译确认流程:
    // 1. 设置 m_bChangedAggro = 1
    // 2. 查找现有的伤害计量记录
    // 3. 如果找到且 isPlus=true，累加仇恨值
    // 4. 如果找到且 isPlus=false，设置仇恨值
    // 5. 如果没找到，创建新记录
    // 6. 更新时间戳

    m_bChangedAggro = 1;

    // 获取当前时间 (需要实现 ThreadLocalData::GetTimer 和 IVTimer::GetTime)
    float fCurrTime = 0.0f;  // TODO: 获取实际时间

    auto it = m_arDamageMeter.find(dwID);
    if (it != m_arDamageMeter.end()) {
        // 找到现有记录
        if (isPlus) {
            it->second.fAggro += fAggro;
        } else {
            it->second.fAggro = fAggro;
        }
        it->second.fTime = fCurrTime;
    } else {
        // 创建新记录
        tagDamageMeter newData;
        newData.fAggro = fAggro;
        newData.fTime = fCurrTime;
        m_arDamageMeter[dwID] = newData;
    }
}

// ============================================================================
// GetTopAggroValue IDA 0x140361640 -> 0x1403616F3
// 获取最高仇恨值
// 大小: 179 bytes
// ============================================================================
float CMonster::GetTopAggroValue() {
    // IDA 反编译确认流程:
    // 1. 初始化 maxAggro = 0.0
    // 2. 遍历 m_arDamageMeter
    // 3. 对每个记录，检查 fAggro 是否大于 maxAggro
    // 4. 返回最大仇恨值

    float maxAggro = 0.0f;

    // IDA: for (auto iter = m_arDamageMeter.begin(); iter != m_arDamageMeter.end(); ++iter)
    for (const auto& pair : m_arDamageMeter) {
        // IDA: dmgMeter = (tagDamageMeter *)(&iter->first + 1);
        //      实际上是 iter->second.fAggro
        // IDA: if (dmgMeter->fAggro > maxAggro) maxAggro = dmgMeter->fAggro;
        if (pair.second.fAggro > maxAggro) {
            maxAggro = pair.second.fAggro;
        }
    }

    return maxAggro;
}

// ============================================================================
// UpdateHealAggro IDA 0x14035FB20 -> 0x14035FC56
// 更新治疗仇恨 - 精确还原
// ============================================================================
void CMonster::UpdateHealAggro() {
    // IDA 0x14035FB20 精确还原:
    // 获取当前时间，遍历伤害计量，检查治疗并更新仇恨

    // TODO: 需要实现 ThreadLocalData::GetTimer() 和 IVTimer::GetTime()
    // VDefaultTimer* Timer = ThreadLocalData::GetTimer();
    // float fCurrTime = IVTimer::GetTime(Timer);
    float fCurrTime = 0.0f;  // 临时使用0，等待时间系统实现

    for (auto iter = m_arDamageMeter.begin(); iter != m_arDamageMeter.end(); ++iter) {
        tagDamageMeter& dmgMeter = iter->second;

        // 获取目标 Mover
        CMoverEx* pMover = static_cast<CMoverEx*>(CMover::GetMoverObject(iter->first));
        // TODO: IsPlayer 和 GetAmountOfHeal 需要从CGocAttribute获取
        // if (pMover && pMover->IsPlayer() && pMover->GetAmountOfHeal() > 0.0f) {
        if (pMover) {
            dmgMeter.fTime = fCurrTime;
            float fHealAggro = CalcHealAggroPoint(pMover);
            dmgMeter.fAggro += fHealAggro;
            m_bChangedAggro = 1;
        }
    }
}

// ============================================================================
// CalcSkillAggroPoint IDA 0x14035F7F0
// 计算技能仇恨点数
// ============================================================================
float CMonster::CalcSkillAggroPoint(CMoverEx* pUser, float fDamage, TB_SKILL* pSkillRef) {
    // IDA 反编译确认:
    // if (pSkillRef->Agro_Type) v4 = fDamage + (fDamage * pSkillRef->Agro_Value * 0.01)
    // else v4 = fDamage + pSkillRef->Agro_Value
    // DAP = (fDamage + v4) * 0.0002
    // if (pSkillRef->Skill_Attribute_Sub >= 4) return 0.0
    // SPAP = DAP * m_pMobTableRef->Melee_Skill_Inclination[pSkillRef->Skill_Attribute_Sub] * 0.01
    // byOrder = CMoverEx::GetAggroLevelOrder(pUser)
    // if (byOrder >= 4) return 0.0
    // return (DAP + SPAP) + (DAP * fLevelPoint[byOrder])

    if (!pSkillRef || !m_pMobTableRef) {
        return 0.0f;
    }

    // 计算基础仇恨值
    float fBaseAggro = 0.0f;
    if (pSkillRef->Agro_Type) {
        fBaseAggro = fDamage + (fDamage * static_cast<float>(pSkillRef->Agro_Value) * 0.01f);
    } else {
        fBaseAggro = fDamage + static_cast<float>(pSkillRef->Agro_Value);
    }

    // 计算DAP (Damage Aggro Point)
    float DAP = (fDamage + fBaseAggro) * 0.0002f;

    // 检查技能属性子类型
    if (pSkillRef->Skill_Attribute_Sub >= 4) {
        return 0.0f;
    }

    // 计算SPAP (Skill Point Aggro Point)
    // 注意: Melee_Skill_Inclination 是一个数组，需要根据索引访问
    float SPAP = 0.0f;
    // TODO: SPAP = DAP * m_pMobTableRef->Melee_Skill_Inclination[pSkillRef->Skill_Attribute_Sub] * 0.01f;

    // 获取目标等级顺序
    std::uint8_t byOrder = 0;  // TODO: CMoverEx::GetAggroLevelOrder(pUser);
    if (byOrder >= 4) {
        return 0.0f;
    }

    // 等级修正系数
    static const float fLevelPoint[4] = { 1.5f, 1.4f, 1.3f, 1.2f };

    return (DAP + SPAP) + (DAP * fLevelPoint[byOrder]);
}

// ============================================================================
// CalcDotAggroPoint IDA 0x14035F9E0
// 计算DoT仇恨点数
// ============================================================================
float CMonster::CalcDotAggroPoint(float fDamage) {
    // IDA 反编译: return fDamage * 0.0001f
    return fDamage * 0.0001f;
}

// ============================================================================
// CalcHealAggroPoint IDA 0x14035FA00
// 计算治疗仇恨点数
// ============================================================================
float CMonster::CalcHealAggroPoint(CMoverEx* pUser) {
    // IDA 反编译确认流程:
    // 1. 获取治疗量
    // 2. 计算DAP = 治疗量 * 0.0001
    // 3. 计算SPAP = DAP * Heal_Skill_Inclination * 0.01
    // 4. 根据等级顺序计算最终仇恨

    if (!pUser || !m_pMobTableRef) {
        return 0.0f;
    }

    // 获取治疗量
    float fHealAmount = 0.0f;  // TODO: CMoverEx::GetAmountOfHeal(pUser)

    // 计算DAP
    float DAP = fHealAmount * 0.0001f;

    // 计算SPAP
    // float SPAP = DAP * m_pMobTableRef->Heal_Skill_Inclination * 0.01f;

    // 获取目标等级顺序
    // std::uint8_t byOrder = CMoverEx::GetAggroLevelOrder(pUser);
    // if (byOrder >= 4) return 0.0f;

    // 等级修正系数
    static const float fLevelPoint[4] = { 1.5f, 1.4f, 1.3f, 1.2f };

    // return (DAP + SPAP) + (DAP * fLevelPoint[byOrder]);

    // 简化实现
    return DAP;
}

// ============================================================================
// CheckDamageAggroReset IDA 0x14035FF80
// 检查伤害仇恨重置
// ============================================================================
void CMonster::CheckDamageAggroReset(float fDist, float fTime) {
    // IDA 反编译确认:
    // if (fTime == 0.0 && fDist == 0.0) return;
    // 查找 m_arDamageMeter 中的 m_dwTargetID
    // if (fTime > 0.0) {
    //     fCurrTime = IVTimer::GetTime(ThreadLocalData::GetTimer());
    //     if (fCurrTime > iter->second.fTime + fTime) {
    //         erase(iter);
    //         bRemoved = 1;
    //     }
    // }
    // if (!bRemoved && fDist > 0.0) {
    //     pMover = CMover::GetMoverObject(this, m_dwTargetID);
    //     if (pMover) {
    //         fDiffSq = getDistanceToSquared(GetPosition(this), GetPosition(pMover));
    //         if (fDiffSq > fDist * fDist) { erase(iter); bRemoved = 1; }
    //     } else {
    //         erase(iter);
    //         bRemoved = 1;
    //     }
    // }
    // if (bRemoved) DamageAggressive(this);

    if (fTime == 0.0f && fDist == 0.0f) {
        return;
    }

    // 查找当前目标
    auto it = m_arDamageMeter.find(m_dwTargetID);
    if (it == m_arDamageMeter.end()) {
        return;
    }

    bool bRemoved = false;
    float fCurrTime = 0.0f;  // TODO: ThreadLocalData::GetTimer() + IVTimer::GetTime()

    // 检查时间超时
    if (fTime > 0.0f) {
        if (fCurrTime > it->second.fTime + fTime) {
            m_arDamageMeter.erase(it);
            bRemoved = true;
        }
    }

    // 检查距离超时
    if (!bRemoved && fDist > 0.0f) {
        // 获取目标位置
        // TODO: CMover* pMover = CMover::GetMoverObject(this, m_dwTargetID);
        // if (pMover) {
        //     hkvVec3* pMyPos = VisObject3D_cl::GetPosition(this);
        //     hkvVec3* pTargetPos = VisObject3D_cl::GetPosition(pMover);
        //     float fDiffSq = hkvVec3::getDistanceToSquared(pMyPos, pTargetPos);
        //     if (fDiffSq > fDist * fDist) {
        //         m_arDamageMeter.erase(it);
        //         bRemoved = true;
        //     }
        // } else {
        //     m_arDamageMeter.erase(it);
        //     bRemoved = true;
        // }
    }

    if (bRemoved) {
        DamageAggressive();
    }
}

// ============================================================================
// CheckProtectAggro IDA 0x140361260
// 检查保护仇恨
// ============================================================================
void CMonster::CheckProtectAggro(std::uint32_t dwID, float fAggro) {
    // IDA 反编译确认:
    // dwSpawnBoxID = CMonster::GetSpawnBoxID(this);
    // if (dwSpawnBoxID == this->m_dwGuardID && this->m_dwGuardID != -1) {
    //     fAggroRatio = fAggro * this->m_fProtectionAggroRatio;
    //     XArea::ScanGridOrigin(&this->XActor, 2, 2u, &vecGameObjList);
    //     for (auto& it : vecGameObjList) {
    //         pActor = ...;
    //         if (XActor::GetType(pActor) == 2 && pActor && pActor->IsLive() &&
    //             !XActor::IsStatus(pActor, 2u) && dwSpawnBoxID == this->m_dwGuardID) {
    //             pMonster = dynamic_cast<CMonster*>(pActor);
    //             if (pMonster) CMonster::ApplyAggroValue(pMonster, dwID, fAggroRatio, 1);
    //         }
    //     }
    // }

    std::uint32_t dwSpawnBoxID = static_cast<std::uint32_t>(m_nSpawnBoxID);

    if (dwSpawnBoxID != m_dwGuardID || m_dwGuardID == 0xFFFFFFFF) {
        return;
    }

    // 计算保护仇恨比例
    float fAggroRatio = fAggro * m_fProtectionAggroRatio;

    // TODO: 扫描附近的怪物并应用仇恨
    // 需要实现 XArea::ScanGridOrigin
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(&XActor, 2, 2u, &vecGameObjList);
    // for (auto& it : vecGameObjList) {
    //     XActor* pActor = ...;
    //     if (XActor::GetType(pActor) == 2 && pActor && pActor->IsLive() &&
    //         !XActor::IsStatus(pActor, 2u) && dwSpawnBoxID == m_dwGuardID) {
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
    //         if (pMonster) {
    //             pMonster->ApplyAggroValue(dwID, fAggroRatio, true);
    //         }
    //     }
    // }
    // 对每个符合条件的怪物调用 ApplyAggroValue(pMonster, dwID, fAggroRatio, true)
}

// ============================================================================
// DamageAggressive IDA 0x14035FC60 -> 0x14035FF73
// 伤害激怒处理 - 选择仇恨最高的目标 (精确还原)
// ============================================================================
void CMonster::DamageAggressive() {
    // IDA 0x14035FC60 精确还原:
    // 获取当前时间，遍历伤害计量，找到仇恨最高的目标并切换

    std::uint32_t dwTopID = 0xFFFFFFFF;
    float fTopDamage = 0.0f;

    // TODO: 需要实现 ThreadLocalData::GetTimer() 和 IVTimer::GetTime()
    // VDefaultTimer* Timer = ThreadLocalData::GetTimer();
    // float fCurrTime = IVTimer::GetTime(Timer);
    float fCurrTime = 0.0f;  // 临时使用0，等待时间系统实现

    // 获取伤害仇恨重置时间
    float fResetTime = 0.0f;
    if (m_pAi) {
        fResetTime = m_pAi->GetDmgAggroReseTime();
    }

    // 遍历所有伤害计量记录
    for (auto it = m_arDamageMeter.begin(); it != m_arDamageMeter.end(); ++it) {
        float fAggro = it->second.fAggro;

        // 找最高仇恨
        if (fAggro > fTopDamage) {
            fTopDamage = fAggro;
            dwTopID = it->first;
        }

        // 计算时间衰减
        float fLeftTime = fCurrTime - it->second.fTime;
        if (fLeftTime > 0.0f) {
            if (fResetTime <= fLeftTime) {
                // 时间超过重置时间，清零仇恨
                it->second.fAggro = 0.0f;
            } else {
                // 按时间比例衰减
                float fRate = fLeftTime / fResetTime;
                float fReduceAggro = fAggro * fRate;
                it->second.fAggro -= fReduceAggro;
            }
        }

        // 基础衰减 - 每次调用衰减 50%
        float fBaseAggro = it->second.fAggro * 0.5f;
        it->second.fAggro -= fBaseAggro;

        // 如果仇恨值过低，清零
        if (fResetTime >= it->second.fAggro) {
            it->second.fAggro = 0.0f;
        }
    }

    // 重置仇恨改变标志
    m_bChangedAggro = 0;

    // 检查是否需要切换目标
    if (dwTopID == m_dwTargetID || dwTopID == 0xFFFFFFFF) {
        // 目标未改变或没有有效目标
        if (m_pAi && m_pAi->IsEnableClearTarget() && dwTopID == 0xFFFFFFFF) {
            // 清除目标
            ChangeTarget(UXActorID(0xFFFFFFFF));
        }
    } else {
        // 切换到新目标
        ChangeTarget(UXActorID(dwTopID));
    }
}

// ============================================================================
// UpdateDamageAggressive IDA 0x14035F5B0 -> 0x14035F5E9
// 更新伤害激怒
// 大小: 57 bytes
// ============================================================================
void CMonster::UpdateDamageAggressive(CMoverEx* pMover, int nDamage) {
    // IDA 反编译:
    // CMonster::AddDamageMeter(this, pMover, nDamage, nullptr);
    // CMonster::DamageAggressive(this);
    AddDamageMeter(pMover, nDamage, nullptr);
    DamageAggressive();
}

// ============================================================================
// ChangeTarget
// 改变目标
// ============================================================================
void CMonster::ChangeTarget(UXActorID uxTargetID) {
    // 设置新目标
    m_dwTargetID = uxTargetID.dwActorID;

    // TODO: 发送目标改变包
    // TODO: 更新AI状态
}

// ============================================================================
// AddDamageMeter
// 添加伤害计量
// ============================================================================
void CMonster::AddDamageMeter(CMoverEx* pMover, int nDamage, void* pSkillRef) {
    if (!pMover) {
        return;
    }

    // 获取目标ID
    std::uint32_t dwID = 0;  // TODO: CMover::GetID(pMover)

    // 计算仇恨值
    float fAggro = 0.0f;
    if (pSkillRef) {
        // fAggro = CalcSkillAggroPoint(pMover, static_cast<float>(nDamage), static_cast<TB_SKILL*>(pSkillRef));
        fAggro = static_cast<float>(nDamage) * 0.0002f;
    } else {
        fAggro = static_cast<float>(nDamage) * 0.0002f;
    }

    // 应用仇恨
    ApplyAggroValue(dwID, fAggro, true);
}

// ============================================================================
// SetDie IDA 0x14035CE10 -> 0x14035CF61
// 设置死亡状态 - 精确还原
// ============================================================================
void CMonster::SetDie(std::int16_t nMotion, int bSuicide) {
    // IDA 0x14035CE10 精确还原:
    // 检查是否是跟随者且非自杀，以及死亡状态

    // 如果是跟随者且不是自杀，不处理
    if (IsFollower() && !bSuicide) {
        return;
    }

    // 如果已经死亡(4)或倒地(2)，不处理
    if (IsStatus(4u) || IsStatus(2u)) {
        return;
    }

    // 清除移动值
    MoveingValueClear();

    // 清除所有Buff
    AllBuffClear(1);

    // 设置死亡状态
    SetStatus(4u);
    m_bSuicide = bSuicide;

    // 根据动作类型处理
    if (nMotion == -1) {
        // 自动选择死亡动作
        if (IsHitDown()) {
            RealDie(13);
        } else if (!IsKnockDown() || m_nHitStatus == 5) {
            RealDie(12);
        }
    } else {
        // 使用指定动作
        std::int16_t reservedMotion = GetReservedMotion(nMotion);
        RealDie(reservedMotion);
    }
}

// ============================================================================
// ActionProcess IDA 0x14035D660 -> 0x14035E200
// 动作处理 - 处理技能攻击动作
// ============================================================================
int CMonster::ActionProcess(std::int16_t nTriggerIdx) {
    // IDA 反编译确认的流程:
    // 1. 检查技能表索引
    // 2. 获取技能表和攻击判定触发器
    // 3. 获取攻击目标
    // 4. 计算伤害
    // 5. 应用Buff/Aura效果
    // 6. 发送技能包

    m_bDisableDirectionToTargetSkill = 1;

    // 检查技能表索引
    if (!m_nCurSkillTableIdx) {
        return 0;
    }

    // TODO: 实现完整的动作处理逻辑
    // 当前为简化版本

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::ActionProcess called");

    return 1;
}

// ============================================================================
// Damage IDA 0x14035B590 -> 0x14035B632
// 伤害处理 (虚函数 override) - 精确还原
// ============================================================================
void CMonster::Damage(tagACTION_DAMAGE& dmgInfo, unsigned int nSkillID, bool* bSABreaked) {
    // IDA 0x14035B590 精确还原:
    // 1. CheckProtectDamage - 检查保护技能伤害
    // 2. CMoverEx::Damage - 调用基类伤害处理
    // 3. 如果有伤害且HP > 0，处理AI和掉落

    CheckProtectDamage(dmgInfo);

    // 调用基类 Damage
    CMoverEx::Damage(dmgInfo, nSkillID, bSABreaked);

    if (dmgInfo.nDamage > 0 && GetHP() > 0) {
        // AI伤害处理 - IDA: if (this->m_pAi) CAi::FuncDamageProcess(this->m_pAi);
        // TODO: FuncDamageProcess needs to be implemented in CAi
        // if (m_pAi) {
        //     m_pAi->FuncDamageProcess();
        // }

        // 击中掉落
        DropItemByHit(dmgInfo.dwID);

        // 迷宫伤害处理
        OnDamageForMaze();
    }
}

// ============================================================================
// DamageProcessHP IDA 0x14035BF70 -> 0x14035C045
// HP伤害处理 (虚函数 override) - 精确还原
// ============================================================================
bool CMonster::DamageProcessHP(unsigned int dwID, int nSkillID, int nDamage,
                                unsigned char byDamageFlag, unsigned char byHitParts) {
    // IDA 0x14035BF70 精确还原:
    // 1. 调用基类 DamageProcessHP
    // 2. 如果是随从且HP <= 0，恢复HP并切换到恢复状态
    // 3. 增加击中计数

    // TODO: 基类DamageProcessHP需要6个参数，暂时使用默认值
    bool bResult = CMoverEx::DamageProcessHP(dwID, nSkillID, nDamage, 0, byDamageFlag, byHitParts);

    if (IsFollower() && GetHP() <= 0) {
        SetHpEx(1);
        ChangeAiState(FSMSTATES_RECOVERY);
        return false;
    }

    ++m_nHitCount;

    return bResult;
}

// ============================================================================
// CheckProtectDamage IDA 0x14035B860 -> 0x14035BBD8
// 检查保护伤害 - 精确还原
// ============================================================================
void CMonster::CheckProtectDamage(tagACTION_DAMAGE& dmgInfo) {
    // IDA 0x14035B860 精确还原:
    // 检查 m_eSkillType 和 m_nProtectSkillDamage
    // SKILLTYPE_PROTECT_A (6) - 吸收伤害型保护
    // SKILLTYPE_PROTECT_B (7) - 时间型保护

    if (!m_eSkillType) {
        return;
    }

    float totalDamage = static_cast<float>(m_nProtectSkillDamage);
    if (totalDamage <= 0.0f) {
        return;
    }

    // SKILLTYPE_PROTECT_A (6) - 吸收伤害型
    if (m_eSkillType == SKILLTYPE_PROTECT_A) {
        SetInvincibleActor(1);
        m_nAccumulateDamage += dmgInfo.nDamage;

        if (m_nAccumulateDamage > 0) {
            dmgInfo.nDamage = 0;
            float fPercent = static_cast<float>(m_nAccumulateDamage) / totalDamage;

            if (fPercent < 1.0f) {
                SendNoticePacket(71, m_nAccumulateDamage + static_cast<int>(totalDamage), fPercent);
            } else {
                dmgInfo.fSuperArmorGage = 0.0f;
                ShowProtectSkillUI(false);
                SendNoticePacket(71, -1, -1.0f);

                // 通知迷宫保护技能结束
                // TODO: XArea* pArea = GetArea();
                // if (pArea) {
                //     XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
                //     if (pMaze) {
                //         pMaze->OnProtectSkill(m_eSkillType);
                //     }
                // }
            }
        }
    }
    // SKILLTYPE_PROTECT_B (7) - 时间型
    else if (m_eSkillType == SKILLTYPE_PROTECT_B) {
        SetInvincibleActor(1);
        m_nAccumulateDamage += dmgInfo.nDamage;

        if (m_nAccumulateDamage > 0) {
            dmgInfo.nDamage = 0;
            float fTime = (totalDamage - static_cast<float>(m_nAccumulateDamage)) / totalDamage;

            if (fTime > 0.0f) {
                SendNoticePacket(72, static_cast<int>(totalDamage) - m_nAccumulateDamage, fTime);
            } else {
                dmgInfo.fSuperArmorGage = 0.0f;
                ShowProtectSkillUI(false);
                SendNoticePacket(72, -1, -1.0f);

                // 通知迷宫保护技能结束
                // TODO: XArea* pArea = GetArea();
                // if (pArea) {
                //     XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
                //     if (pMaze) {
                //         pMaze->OnProtectSkill(m_eSkillType);
                //     }
                // }
            }
        }
    }
}

// ============================================================================
// OnDamageForMaze IDA 0x14035BC60
// 迷宫伤害处理
// ============================================================================
// OnDamageForMaze IDA 0x14035B640 -> 0x14035B6B3
// 迷宫伤害处理 - 精确还原
// ============================================================================
void CMonster::OnDamageForMaze() {
    // IDA 0x14035B640 精确还原:
    // TODO: XArea* pArea = GetArea();
    // XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    // if (pMaze) {
    //     pMaze->DamageMonster(this);
    // }
    // 临时保留框架，等待XMaze实现
}

// ============================================================================
// SetHpEx - 设置HP扩展
// ============================================================================
void CMonster::SetHpEx(int nHP) {
    // 设置HP并更新相关状态
    SetHP(nHP);

    // TODO: 更新 STMonsterInfo 中的HP
    // m_stMonsterInfo.SetHP(nHP);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::SetHpEx called");
}

// ============================================================================
// ShowProtectSkillUI IDA 0x14035B720 -> 0x14035B85E
// 显示保护技能UI - 精确还原
// ============================================================================
void CMonster::ShowProtectSkillUI(bool bActive) {
    // IDA 0x14035B720 精确还原:
    // SKILLTYPE_PROTECT_A = 6, SKILLTYPE_PROTECT_B = 7

    if (m_eSkillType == SKILLTYPE_PROTECT_A) {
        if (bActive) {
            SetInvincibleActor(1);
            SendNoticePacket(71, m_nProtectSkillDamage, 0.0f);
        } else {
            SetInvincibleActor(0);
            m_eSkillType = SKILLTYPE_NONE;
            m_nAccumulateDamage = 0;
            SendNoticePacket(71, -1, -1.0f);
        }
    } else if (m_eSkillType == SKILLTYPE_PROTECT_B) {
        if (bActive) {
            SetInvincibleActor(1);
            SendNoticePacket(72, m_nProtectSkillDamage, 1.0f);
        } else {
            SetInvincibleActor(0);
            m_eSkillType = SKILLTYPE_NONE;
            m_nAccumulateDamage = 0;
            SendNoticePacket(72, -1, -1.0f);
        }
    }
}

// ============================================================================
// SendNoticePacket IDA 0x14035BBE0 -> 0x14035BC93
// 发送通知包 - 精确还原
// ============================================================================
void CMonster::SendNoticePacket(int iType, int iValue, float fTime) {
    // IDA 0x14035BBE0 精确还原:
    // 获取Area并转换为XMaze，然后发送通知包

    // TODO: XArea* pArea = GetArea();
    // XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    // if (pMaze) {
    //     if (iValue >= 0) {
    //         m_nProtectSkill = iType;
    //     } else {
    //         m_nProtectSkill = 0;
    //     }
    //     pMaze->SendNoticePacket(iType, iValue, fTime);
    // }

    // 临时实现 - 仅更新保护技能标志
    if (iValue >= 0) {
        m_nProtectSkill = iType;
    } else {
        m_nProtectSkill = 0;
    }
}

// ============================================================================
// NotifySpawnMonsterDied IDA 0x14035F1A0 -> 0x14035F325
// 通知召唤怪物死亡 - 处理隐身条件检查 (精确还原)
// ============================================================================
void CMonster::NotifySpawnMonsterDied(std::uint32_t dwID) {
    // IDA 0x14035F1A0 精确还原:
    // 检查隐身状态标志 0x2000，处理隐身条件类型

    if (!IsStatus(0x2000u)) {
        return;
    }

    if (m_nInvisibleConditionType == 2) {
        // 类型2：计数型 - 减少计数
        if (--m_nInvisibleConditionVal[0] <= 0) {
            // TODO: SetInvisible(0, 0, 0, 0, 0, 0, 0);
            // 临时保留框架，等待SetInvisible实现
        }
    } else if (m_nInvisibleConditionType == 3) {
        // 类型3：目标型 - 检查死亡目标是否在条件列表中
        CMover* pMover = CMover::GetMoverObject(dwID);
        if (pMover) {
            bool bAlive = false;
            for (int i = 0; i < 4; ++i) {
                if (m_nInvisibleConditionVal[i]) {
                    if (pMover->GetTableID() == m_nInvisibleConditionVal[i]) {
                        m_nInvisibleConditionVal[i] = 0;
                    } else {
                        bAlive = true;
                    }
                }
            }
            if (!bAlive) {
                // TODO: SetInvisible(0, 0, 0, 0, 0, 0, 0);
                // 临时保留框架，等待SetInvisible实现
            }
        }
    }
}

// ============================================================================
// GetMonsterInfo IDA 0x140355DB0 -> 0x140355FCD
// 获取怪物信息 - 同步位置、状态、属性等
// ============================================================================
STMonsterInfo& CMonster::GetMonsterInfo() {
    // IDA 反编译确认:
    // 1. ApplyLevelToStat(this, 1)
    // 2. 获取位置并设置到 m_stMonsterInfo.stNpcInfo.stPosInfo.vPos
    // 3. 设置旋转、SpawnBoxID、MotionClass、BattlePos
    // 4. 设置 SuperArmor 信息
    // 5. 设置 SectorID
    // 6. 设置 SuicideTime
    // 7. 遍历 77 个属性，将大于 0 的属性添加到 vecStat
    // 8. 返回 m_stMonsterInfo

    // TODO: ApplyLevelToStat(1);

    // 获取位置
    // hkvVec3* pPos = GetPositionXVec3();
    // m_stMonsterInfo.stNpcInfo.stPosInfo.vPos = *pPos;

    // 设置旋转
    m_stMonsterInfo.stNpcInfo.stPosInfo.fRot = GetMovingYaw();

    // 设置基本信息
    m_stMonsterInfo.nSpawnBoxID = m_nSpawnBoxID;
    m_stMonsterInfo.nMotionClass = m_nMotionClass;
    m_stMonsterInfo.bBattlePos = m_bBattlePose;

    // 设置 SuperArmor 信息
    // m_stMonsterInfo.fCurSuperArmor = GetCurSuperArmorGage();
    // m_stMonsterInfo.fMaxSuperArmor = GetMaxSuperArmorGage();

    // 设置 SectorID
    if (m_pSector) {
        // m_stMonsterInfo.stNpcInfo.nSectorID = CSector::GetSectorBoxID(m_pSector);
    } else {
        m_stMonsterInfo.stNpcInfo.nSectorID = -1;
    }

    // 设置自杀时间
    m_stMonsterInfo.fSuicideTime = 0.0f;
    if (m_pAi) {
        // m_stMonsterInfo.fSuicideTime = m_pAi->GetSuicideTime();
    }

    // TODO: 遍历属性并添加到 vecStat
    // 需要访问 CGocAttribute 组件

    return m_stMonsterInfo;
}

// ============================================================================
// SetParentID IDA 0x1401ad920
// 设置父ActorID - 用于召唤物关联
// ============================================================================
void CMonster::SetParentID(UXActorID uxVal) {
    // IDA 反编译确认: 设置到 m_stMonsterInfo.uxParentActorID
    m_stMonsterInfo.uxParentActorID = uxVal;
}

// ============================================================================
// GetAIState IDA 0x140357A20
// 获取AI状态
// ============================================================================
std::uint8_t CMonster::GetAIState() {
    // IDA 反编译确认: 如果 m_pAi 存在，返回 AI 的状态
    if (m_pAi) {
        return static_cast<std::uint8_t>(m_pAi->GetState());
    }
    return 0;
}

// ============================================================================
// SetReservedMotion IDA 0x140357A60
// 设置预留动作
// ============================================================================
void CMonster::SetReservedMotion(std::int16_t nDefault, std::int16_t nReserve) {
    // IDA 反编译确认: 插入到 m_mapReservedMotion
    m_mapReservedMotion[nDefault] = nReserve;
}

// ============================================================================
// GetReservedMotion IDA 0x140357AB0
// 获取预留动作
// ============================================================================
std::int16_t CMonster::GetReservedMotion(std::int16_t nMotion) {
    // IDA 反编译确认: 从 m_mapReservedMotion 查找
    auto it = m_mapReservedMotion.find(nMotion);
    if (it != m_mapReservedMotion.end()) {
        return it->second;
    }
    return nMotion;
}

// ============================================================================
// GetAIFuzzyValue IDA 0x140357B30
// 获取AI模糊值
// ============================================================================
void CMonster::GetAIFuzzyValue(float* pfValue) {
    // IDA 反编译确认: 如果 m_pAi 存在，调用 CAi::GetAIFuzzyValue
    // TODO: m_pAi->GetAIFuzzyValue(pfValue);
    if (m_pAi && pfValue) {
        // Stub: zero out fuzzy values
        for (int i = 0; i < 10; ++i) {
            pfValue[i] = 0.0f;
        }
    }
}

// ============================================================================
// GetAIActionValue IDA 0x140357B70
// 获取AI动作值
// ============================================================================
void CMonster::GetAIActionValue(int* pnValue) {
    // IDA 反编译确认: 如果 m_pAi 存在，调用 CAi::GetAIActionValue
    // TODO: m_pAi->GetAIActionValue(pnValue);
    if (m_pAi && pnValue) {
        // Stub: zero out action values
        for (int i = 0; i < 10; ++i) {
            pnValue[i] = 0;
        }
    }
}

// ============================================================================
// CheckWayPoint IDA 0x140357BB0
// 检查路径点
// ============================================================================
void CMonster::CheckWayPoint() {
    // IDA 反编译确认:
    // if (m_xWayPoint.GetCurID() > 0) {
    //     hkvVec3 vDist = m_vPosition - m_xWayPoint.GetDestPosition();
    //     if (vDist.getLengthSquared() < 9.0f) {
    //         m_xWayPoint.Update(0.001f);
    //     }
    // }
    // TODO: 需要实现 CWayPoint 类
}

// ============================================================================
// GetSectorID IDA 0x140357C40
// 获取扇区ID
// ============================================================================
int CMonster::GetSectorID() {
    // IDA 反编译确认: 如果 m_pSector 存在，返回 SectorBoxUniqueID
    if (m_pSector) {
        // TODO: return CSector::GetSectorBoxUniqueID(m_pSector);
        return -1;
    }
    return -1;
}

// ============================================================================
// ApplySuperArmorGage IDA 0x140358100
// 应用超级护甲值
// ============================================================================
void CMonster::ApplySuperArmorGage(float fSuperArmor) {
    // IDA 反编译确认:
    // if (m_pMobTableRef->Monster_Defence_Type == 2) {
    //     m_fMaxSuperArmorGage = m_fAbility[10] * (fSuperArmor / 100.0f);
    //     m_fCurSuperArmorGage = m_fMaxSuperArmorGage;
    // } else {
    //     m_fMaxSuperArmorGage = 0.0f;
    //     m_fCurSuperArmorGage = 0.0f;
    // }
    // TODO: 需要访问 m_fAbility 数组
}

// ============================================================================
// GetSpawnTime IDA 0x1403606A0
// 获取生成时间
// ============================================================================
float CMonster::GetSpawnTime() {
    // IDA 反编译确认:
    // return (XTime::GetTickCount() - m_dwSpawnedTime64) * 0.001f;
    // TODO: 需要实现 XTime::GetTickCount()
    return 0.0f;
}

// ============================================================================
// IsTableID IDA 0x140360700
// 检查表ID是否匹配
// ============================================================================
bool CMonster::IsTableID(const char* szTableID) {
    // IDA 反编译确认: 将字符串转为整数，与 GetTableID() 比较
    if (!szTableID) return false;
    int nTableID = std::stoi(szTableID);
    return nTableID == GetTableID();
}

// ============================================================================
// IsRealHelper IDA 0x1403609D0
// 检查是否真实助手
// ============================================================================
bool CMonster::IsRealHelper() {
    // IDA 反编译确认:
    // if (m_pMobTableRef && m_pMobTableRef->Monster_Type == 10) {
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     if (XResourceMgr::GetTB_HELPER(pServer->m_xResourceMgr, m_pMobTableRef->ID))
    //         return true;
    // }
    // return false;
    if (m_pMobTableRef && m_pMobTableRef->Monster_Type == 10) {
        // TODO: 检查 TB_HELPER 表
        return true;
    }
    return false;
}

// ============================================================================
// IsDefensiveWeapon IDA 0x140360790
// 检查是否防御武器类型
// ============================================================================
bool CMonster::IsDefensiveWeapon() {
    // IDA 反编译确认: Monster_Type == 5
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 5;
}

// ============================================================================
// IsMonsterDamageCount IDA 0x140360890
// 检查是否伤害计数类型
// ============================================================================
bool CMonster::IsMonsterDamageCount() {
    // IDA 反编译确认: Monster_Type == 14
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 14;
}

// ============================================================================
// IsDedicated IDA 0x1403608D0
// 检查是否专用类型
// ============================================================================
bool CMonster::IsDedicated() {
    // IDA 反编译确认: Monster_Type == 15
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 15;
}

// ============================================================================
// IsMonsterCombo IDA 0x140360910
// 检查是否连击类型
// ============================================================================
bool CMonster::IsMonsterCombo() {
    // IDA 反编译确认: Monster_Type == 16
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 16;
}

// ============================================================================
// IsMercenary IDA 0x140360950
// 检查是否雇佣兵类型
// ============================================================================
bool CMonster::IsMercenary() {
    // IDA 反编译确认: Monster_Type == 8
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 8;
}

// ============================================================================
// IsHelper IDA 0x140360990
// 检查是否助手类型
// ============================================================================
bool CMonster::IsHelper() {
    // IDA 反编译确认: Monster_Type == 10
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 10;
}

// ============================================================================
// IsNormalMonster IDA 0x140360A50
// 检查是否普通怪物
// ============================================================================
bool CMonster::IsNormalMonster() {
    // IDA 反编译确认: Monster_Type == 0
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 0;
}

// ============================================================================
// IsMonsterInteractObject IDA 0x140360AD0
// 检查是否交互对象类型
// ============================================================================
bool CMonster::IsMonsterInteractObject() {
    // IDA 反编译确认: Monster_Type == 7
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 7;
}

// ============================================================================
// IsInheritParentStat IDA 0x140360B10
// 检查是否继承父属性
// ============================================================================
bool CMonster::IsInheritParentStat() {
    // IDA 反编译确认:
    // return m_pMobTableRef && m_pMobTableRef->Monster_Element == 1 && GetOwnerPlayer()
    if (m_pMobTableRef && m_pMobTableRef->Monster_Element == 1) {
        // TODO: 需要实现 GetOwnerPlayer()
        // CMoverEx* pOwnerMover = GetOwnerPlayer();
        // return pOwnerMover != nullptr;
    }
    return false;
}

// ============================================================================
// CheckFollowMonster IDA 0x140360BA0
// 检查跟随怪物
// ============================================================================
void CMonster::CheckFollowMonster() {
    // IDA 反编译确认:
    // if (IsMercenary()) {
    //     FindFollowPlayer();
    //     if (!GetOwnerPlayer())
    //         SetInvisible(1, 0, 0, 0, 0, 0, 0);
    // }
    if (IsMercenary()) {
        FindFollowPlayer();
        // TODO: 需要实现 GetOwnerPlayer 和 SetInvisible
        // if (!GetOwnerPlayer()) {
        //     SetInvisible(1, 0, 0, 0, 0, 0, 0);
        // }
    }
}

// ============================================================================
// FindFollowPlayer IDA 0x140360C20
// 查找跟随玩家
// ============================================================================
void CMonster::FindFollowPlayer() {
    // IDA 反编译确认:
    // if (IsMercenary()) {
    //     if (IsStatus(0x2000)) {
    //         SetOwnerID(0);
    //         return;
    //     }
    //     XMaze* pMaze = (XMaze*)GetArea();
    //     if (pMaze) {
    //         CMover* pMover = pMaze->FindUserByQuest(m_pMobTableRef->Quest_Group_1);
    //         if (!pMover) {
    //             SetOwnerID(0);
    //             return;
    //         }
    //         SetOwnerID(pMover->GetActorID());
    //     }
    // }
    if (IsMercenary()) {
        // TODO: 需要实现 IsStatus, GetArea, FindUserByQuest, SetOwnerID
        // 当前为占位实现
    }
}

// ============================================================================
// SetDefensiveWeaponPlayer IDA 0x140360D80
// 设置防御武器玩家
// ============================================================================
void CMonster::SetDefensiveWeaponPlayer(CMoverEx* pMover) {
    // IDA 反编译确认:
    // CMoverEx* pOwner = GetOwnerPlayer();
    // if (pOwner != pMover) {
    //     CUser* pOwnerUser = dynamic_cast<CUser*>(pOwner);
    //     if (pOwnerUser)
    //         pOwnerUser->DefensiveWeaponActivate(0, this);
    //     CUser* pNewUser = dynamic_cast<CUser*>(pMover);
    //     if (pNewUser) {
    //         pNewUser->DefensiveWeaponActivate(1, this);
    //         SetHP(GetMaxHP());
    //         m_fLifeTime = 10.0f;
    //         m_nLastTime = (int)m_fLifeTime;
    //         m_nHPPerSec = GetMaxHP() / m_nLastTime;
    //         ChangeBattlePose(1, 1);
    //     } else {
    //         ChangeBattlePose(0, 1);
    //     }
    // }
    // TODO: 需要实现 GetOwnerPlayer, DefensiveWeaponActivate, GetMaxHP, SetHP, ChangeBattlePose
}

// ============================================================================
// FindGuardMonster IDA 0x140360F60
// 查找守护怪物
// ============================================================================
CMonster* CMonster::FindGuardMonster() {
    // IDA 反编译确认:
    // CMonster* pGuardMonster = GetGuardMonster();
    // if (pGuardMonster)
    //     return pGuardMonster;
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(this, 2, 2, &vecGameObjList);
    // for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
    //     CMover* pMover = *it;
    //     if (pMover && !pMover->IsDie() && pMover->GetType() == 2) {
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //         if (pMonster && GetActorID() != pMonster->GetActorID() &&
    //             pMonster->GetSpawnBoxID() == m_dwGuardID) {
    //             SetGuardMonster(pMonster->GetActorID());
    //             return pMonster;
    //         }
    //     }
    // }
    // return nullptr;
    // TODO: 需要实现 GetGuardMonster, XArea::ScanGridOrigin 等
    return nullptr;
}

// ============================================================================
// ApplyLevelToStat IDA 0x140357C80
// 应用等级到属性
// ============================================================================
void CMonster::ApplyLevelToStat(int bInit) {
    // IDA 反编译确认:
    // if ((m_bApplyLevel != 1 || bInit != 1) && m_pMobTableRef) {
    //     unsigned char byCount = 1;
    //     XMaze* pMaze = (XMaze*)GetArea();
    //     if (pMaze)
    //         byCount = pMaze->GetPartyMemberCount();
    //     if (!byCount) byCount = 1;
    //     if (byCount >= 4) byCount = 4;
    //     if (m_pMobTableRef->Monster_Type) byCount = 1;
    //     TB_PARTYREVISE* pPartyRevise = GetPartyReviseTable(m_pMobTableRef->Monster_Party_Revise, byCount);
    //     if (pPartyRevise) {
    //         m_bApplyLevel = 1;
    //         // 应用各种属性修正...
    //         ApplySuperArmorGage(pPartyRevise->SuperArmor_Guage);
    //     }
    // }
    // TODO: 需要实现完整属性应用逻辑
}

// ============================================================================
// InitialObjectInfo IDA 0x140355120 -> 0x1403556D0
// 初始化对象信息
// 大小: 1456 bytes
// ============================================================================
void CMonster::InitialObjectInfo(unsigned int dwID, unsigned int nTableIdx, hkvVec3 vPos, float fRot) {
    // IDA 反编译确认的流程:
    // 1. 调用基类 CMover::InitialObjectInfo
    // 2. 获取 CGocNpcAttribute 组件并初始化
    // 3. 获取能力值数组指针
    // 4. 设置 HP 和表 ID
    // 5. 设置位置和朝向
    // 6. 应用表能力
    // 7. 设置动画
    // 8. 检查转向动作
    // 9. 初始化 GroupAggro, WayPoint, TraceHPState
    // 10. 设置生成时间
    // 11. 如果是防御武器类型，设置无敌

    // 调用基类初始化
    // TODO: CMover::InitialObjectInfo(dwID, nTableIdx, vPos, fRot);

    // 获取 CGocNpcAttribute 组件 (IDA: CMover::GetGOC<CGocNpcAttribute>)
    // TODO: std::tr1::shared_ptr<CGocNpcAttribute> pAttr;
    // CMover::GetGOC<CGocNpcAttribute>(&pAttr, 0);
    // if (pAttr) {
    //     CGocNpcAttribute::Init(pAttr.get(), 1, m_pMobTableRef);
    //     m_fAbility = CGocAttribute::GetFinalStats(pAttr.get());
    // }

    // 设置 HP 和表 ID
    // m_stMonsterInfo.nHP = (int)m_fAbility[10];
    m_stMonsterInfo.stNpcInfo.nTableID = nTableIdx;

    // 设置 ActorID (高位设置为怪物类型标识 0x40000000)
    // m_stMonsterInfo.uxActorID.dwActorID = (m_stMonsterInfo.uxActorID.dwActorID & 0x1FFFFFFF) | 0x40000000;

    // 设置位置和朝向 - 转换 hkvVec3 到 XVec3
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.x = vPos.x;
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.y = vPos.y;
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.z = vPos.z;
    m_stMonsterInfo.stNpcInfo.stPosInfo.fRot = fRot;
    SetPositionXVec3(vPos);
    m_vCreatePos = vPos;

    // 设置移动朝向和方向
    m_fMovingYaw = fRot;
    // TODO: CMover::SetOrientationYaw(fRot);

    // 应用表能力
    ApplyTableAbility();

    // 设置动画
    // TODO: CMover::SetupAnimation();

    // 检查是否有转向动作
    // IDA: dwAnimID = XActionResMgr::GetAnimIndex(this, 7, 0, 1);
    //      pszAnimString = CMover::GetAnimStirng(this, dwAnimID);
    //      m_bHasTurnMotion = (pszAnimString != nullptr);
    //      dwAnimID = XActionResMgr::GetAnimIndex(this, 7, 1, 1);
    //      pszAnimString = CMover::GetAnimStirng(this, dwAnimID);
    //      m_bHasBigTurn = (pszAnimString != nullptr);

    // 检查受击动画数量
    // IDA: dwAnimID = XActionResMgr::GetAnimIndex(this, 18, 2, 1);
    //      pszAnimString = CMover::GetAnimStirng(this, dwAnimID);
    //      if (!pszAnimString) m_nHitAnimCount = 2;

    // 初始化组件
    m_xGroupAggro.Init(this);
    m_xWayPoint.Init(this);
    // TODO: CTraceHPState::Init(reinterpret_cast<CTraceHPState*>(m_xTraceHPState_dummy), this);

    // 重置仇恨检查时间
    m_fLastAggroCheckTime = 0.0f;
    m_bChangedAggro = 0;

    // 设置生成时间
    // TODO: m_dwSpawnedTime64 = XTime::GetTickCount();

    // 如果是防御武器类型 (Monster_Type == 5)
    if (m_pMobTableRef && m_pMobTableRef->Monster_Type == 5) {
        // TODO: XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // m_pWeaponTableRef = XResourceMgr::GetTB_DEFENSIVE_WEAPON(&pServer->m_xResourceMgr, nTableIdx);
        SetInvincibleActor(1);
    }

    // 增加计数
    ++m_naCount;

    // 重置死亡相关状态
    m_fElapsedDieTime = 0.0f;
    m_byAngleAttackType = 0;

    // Boss HP 日志检查
    if (IsBoss()) {
        m_fBossHPLogTime = 60.0f;
        m_nCheckBossHP = GetHP();
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::InitialObjectInfo called");
}

// ============================================================================
// GenerateEventObject IDA 0x1403556D0 -> 0x140355724
// 生成事件对象
// 大小: 84 bytes
// ============================================================================
void CMonster::GenerateEventObject() {
    // IDA 反编译确认:
    // CMoverEx::_GenerateEventObject(this, 2, m_pMobTableRef->ID);
    // CMonster::FindFollowPlayer(this);
    // m_bApplyLevel = 0;
    // CMonster::ApplyLevelToStat(this, 1);

    if (!m_pMobTableRef) {
        return;
    }

    // 调用基类生成事件对象
    // TODO: CMoverEx::_GenerateEventObject(2, m_pMobTableRef->ID);

    // 查找跟随玩家（用于雇佣兵类型）
    FindFollowPlayer();

    // 重置等级应用标志
    m_bApplyLevel = 0;

    // 应用等级到属性
    ApplyLevelToStat(1);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::GenerateEventObject called");
}

// ============================================================================
// SetInfo IDA 0x140355730 -> 0x14035589A
// 设置信息
// 大小: 362 bytes
// ============================================================================
void CMonster::SetInfo() {
    // IDA 反编译确认:
    // sprintf_s(szName, "Monster Idle %u", m_pGrapParent);
    // 设置组件相关
    // CMySkillList::Init(pMySkillList, this);

    // 设置空闲名称
    char szName[64] = {0};
    // sprintf_s(szName, "Monster Idle %u", m_pGrapParent);

    // 初始化技能管理器
    if (!m_pSkillMgr) {
        // TODO: m_pSkillMgr = new CMySkillList();
        // TODO: CMySkillList::Init(m_pSkillMgr, this);
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::SetInfo called");
}

// ============================================================================
// SetSyncInfo IDA 0x140355B10
// 设置同步信息
// ============================================================================
void CMonster::SetSyncInfo() {
    // IDA 反编译确认:
    // ApplyLevelToStat(1);
    // 设置位置、旋转、状态等信息
    // 收集属性信息到 vecStat
    // TODO: 需要实现完整同步信息设置
}

// ============================================================================
// ProcessExp IDA 0x140355FD0
// 处理经验
// IDA 精确还原:
// 1. if (!m_pMobTableRef) return
// 2. nRank = m_pMobTableRef->Monster_Rank
// 3. pTBExp = XResourceMgr::GetTB_MONSTER_EXP(GetLevel())
// 4. nExp = (int)(pTBExp->EXP_Slave[nRank] * m_pMobTableRef->Exp)
// 5. if (GetArea()->IsMaze()) -> XArea::ProcessExp()
//    else -> CUser::SetExp()
// ============================================================================
void CMonster::ProcessExp(XActor* pActor) {
    // IDA: if (!m_pMobTableRef) return
    if (!m_pMobTableRef) {
        return;
    }

    // IDA: nRank = m_pMobTableRef->Monster_Rank
    int nRank = m_pMobTableRef->Monster_Rank;

    // IDA: v12 = this->GetLevel(this)
    std::uint8_t byLevel = GetLevel();

    // IDA: v2 = TXSingleton<XGameServer>::Instance()
    // pTBExp = XResourceMgr::GetTB_MONSTER_EXP(&v2->m_xResourceMgr, v12)
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) return;

    TB_MONSTER_EXP* pTBExp = pServer->GetResourceMgr().GetTB_MONSTER_EXP(byLevel);

    // IDA: if (!pTBExp) LogError and return
    if (!pTBExp) {
        GreenDamTan_log(__FILE__, __FUNCTION__,
            "ProcessExp error - No Table TB_MONSTER_EXP [ Level:%d ] ( %d )", byLevel, 421);
        return;
    }

    // IDA: if (nRank > 6) LogError and return
    if (nRank > 6) {
        GreenDamTan_log(__FILE__, __FUNCTION__,
            "ProcessExp error - Invalid Rank TB_MONSTER_EXP [ Level:%d ] ( %d )", byLevel, 426);
        return;
    }

    // IDA: nExp = (int)(float)((float)*(&pTBExp->EXP_Slave + nRank) * this->m_pMobTableRef->Exp)
    // Note: TB_MONSTER_EXP::uniMExp[0]=Slave, [1]=Normal, [2]=Elite, [3]=Named, [4]=Boss, [5]=Raid, [6]=Summon
    int nExp = static_cast<int>(static_cast<float>(pTBExp->uniMExp[nRank]) * m_pMobTableRef->Exp);

    // IDA: if (nExp <= 0) return
    if (nExp <= 0) {
        return;
    }

    // IDA: nLevel = m_pMobTableRef->Monster_Lv
    unsigned int nMonsterLevel = m_pMobTableRef->Monster_Lv;

    // TODO: IDA: GetArea and check IsMaze - needs XArea::IsMaze() implementation
    // For now, just grant EXP directly to the attacker
    // IDA: pUser = _RTDynamicCast_0(pActor, 0, &XActor RTTI, &CUser RTTI, 0)
    CUser* pUser = reinterpret_cast<CUser*>(pActor);
    if (pUser) {
        // IDA: CUser::SetExp(pUser, (float)nExp * 1.0, nLevel)
        // TODO: pUser->SetExp(static_cast<float>(nExp), nMonsterLevel);
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessExp: Exp=%d, Level=%d, Rank=%d", nExp, nMonsterLevel, nRank);
}

// ============================================================================
// DropItemByHit IDA 0x140356290
// 击中掉落物品
// IDA 精确还原:
// 1. if (!m_pMobTableRef || !m_pMobTableRef->Monster_Hit_Drop_ID || !GetArea()) return
// 2. if (GetWorldType() == 1) -> XMaze::ProcessDropByHit()
// 3. if (GetWorldType() == 2) -> CBattleZone::ProcessDropByHit()
// ============================================================================
void CMonster::DropItemByHit(unsigned int dwAtkUser) {
    // IDA: if (!m_pMobTableRef || !m_pMobTableRef->Monster_Hit_Drop_ID) return
    if (!m_pMobTableRef || !m_pMobTableRef->Monster_Hit_Drop_ID) {
        return;
    }

    // TODO: IDA logic requires XArea::GetWorldType() and ProcessDropByHit implementations
    // For now, just log the drop
    GreenDamTan_log(__FILE__, __FUNCTION__, "DropItemByHit: DropID=%d, AtkUser=%u",
                    m_pMobTableRef->Monster_Hit_Drop_ID, dwAtkUser);
}

// ============================================================================
// ProcessDrop IDA 0x140356550
// 处理掉落
// IDA 精确还原:
// 1. if (!m_pMobTableRef || !GetArea()) return
// 2. if (GetWorldType() == 2 && GetTBMapID() != 30031) -> CBattleZone::ProcessDrop()
// 3. else -> XArea::ProcessDrop()
// ============================================================================
void CMonster::ProcessDrop(XActor* pAtk) {
    // IDA: if (!m_pMobTableRef || !GetArea()) return
    if (!m_pMobTableRef || !pAtk) {
        return;
    }

    // TODO: IDA logic requires XArea::GetWorldType() and GetTBMapID() implementations
    // For now, just log the drop
    hkvVec3 vDropPos = GetPosition();
    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessDrop: TableID=%d, Pos=(%.2f, %.2f, %.2f)",
                    m_stMonsterInfo.stNpcInfo.nTableID, vDropPos.x, vDropPos.y, vDropPos.z);
}

// ============================================================================
// ProcessEscortQuest IDA 0x140356750
// 处理护送任务
// IDA 精确还原:
// 1. if (!m_pAi || !CAi::IsEscortMonster(m_pAi) || !m_pMobTableRef) return
// 2. pMaze = _RTDynamicCast_0(GetArea(), &XMaze RTTI)
// 3. if (pMaze) { XMaze::FailEscortQuest(); CAi::EndEscortWayPoint(m_pAi); }
// ============================================================================
void CMonster::ProcessEscortQuest() {
    // IDA: if (!m_pAi || !CAi::IsEscortMonster(m_pAi) || !m_pMobTableRef) return
    if (!m_pAi || !m_pMobTableRef) {
        return;
    }

    // TODO: CAi::IsEscortMonster 需要实现
    // if (!CAi::IsEscortMonster(m_pAi)) return;

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessEscortQuest - escort monster died");
}

// ============================================================================
// ProcessGameMode IDA 0x1403568A0
// 处理游戏模式
// IDA 精确还原:
// 1. if (!m_bSuicide && m_pMobTableRef && m_pMobTableRef->Monster_Type == 3)
//    -> XMaze::SetGameModeState(2)
// 2. else -> CBattleZone::MonsterDieForEvent(this, GetHitID())
// ============================================================================
void CMonster::ProcessGameMode() {
    // IDA: if (!m_bSuicide && m_pMobTableRef && m_pMobTableRef->Monster_Type == 3)
    if (!m_bSuicide && m_pMobTableRef && m_pMobTableRef->Monster_Type == 3) {
        // TODO: IDA: pMaze = _RTDynamicCast_0(m_pArea, 0, &XArea RTTI, &XMaze RTTI, 0)
        // XMaze::SetGameModeState(pMaze, 2)
        GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessGameMode: Monster_Type=3 (GameMode state 2)");
    }
    else {
        // TODO: IDA: pD6 = _RTDynamicCast_0(m_pArea, 0, &XArea RTTI, &CBattleZone RTTI, 0)
        // CBattleZone::MonsterDieForEvent(pD6, this, GetHitID())
        GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessGameMode: MonsterDieForEvent");
    }
}

// ============================================================================
// GetSkillDestPos IDA 0x14035A5E0
// 获取技能目标位置
// ============================================================================
hkvVec3 CMonster::GetSkillDestPos() {
    // IDA 反编译确认:
    // if (m_pAi) {
    //     TargetID = CMover::GetTargetID(this);
    //     pTarget = CMover::GetMoverObject(this, TargetID);
    //     if (pTarget) {
    //         rhs = CAi::GetSkillDestPos(m_pAi, &v6);
    //         Position = VisObject3D_cl::GetPosition(pTarget);
    //         operator+(result, Position, rhs);
    //         return result;
    //     }
    // }
    // hkvVec3::ZeroVector(result);
    // return result;
    if (m_pAi) {
        // TODO: 实现 GetTargetID 和 GetMoverObject
        // unsigned int TargetID = CMover::GetTargetID();
        // CMover* pTarget = CMover::GetMoverObject(TargetID);
        // if (pTarget) {
        //     hkvVec3 v6;
        //     hkvVec3 rhs = CAi::GetSkillDestPos(m_pAi, &v6);
        //     hkvVec3 Position = pTarget->GetPosition();
        //     return Position + rhs;
        // }
    }
    return hkvVec3(0.0f, 0.0f, 0.0f);
}

// ============================================================================
// ChangeAngleAttackName IDA 0x14035A6C0
// 改变角度攻击名称
// ============================================================================
void CMonster::ChangeAngleAttackName(std::uint8_t bySkillAngle, VString& strSkillName) {
    // IDA 反编译确认:
    // if (IsNoRotate()) {
    //     TargetID = CMover::GetTargetID();
    //     pTarget = CMover::GetMoverObject(TargetID);
    //     if (pTarget) {
    //         m_byAngleAttackType = 0;
    //         VString strTempName(strSkillName);
    //         byAngleAttackType = 0;
    //         Position = VisObject3D_cl::GetPosition(pTarget);
    //         operator-(&vDirVector, Position, &m_vPosition);
    //         fTargetYaw = CMover::GetYawFromVector(&vDirVector);
    //         fDiffYaw = m_fInitYaw - fTargetYaw;
    //         // Normalize to [-180, 180]
    //         if (fDiffYaw > 180.0) fDiffYaw -= 360.0;
    //         else if (fDiffYaw < -180.0) fDiffYaw += 360.0;
    //         fAbsDiff = fabsf(fDiffYaw);
    //         fCheckAngle = bySkillAngle * 0.3f;
    //         if (fCheckAngle <= fAbsDiff) {
    //             if (fDiffYaw <= 0.0) {
    //                 byAngleAttackType = 3;
    //                 strTempName += L"_L";
    //             } else {
    //                 byAngleAttackType = 2;
    //                 strTempName += L"_R";
    //             }
    //         } else {
    //             byAngleAttackType = 1;
    //             strTempName += L"_C";
    //         }
    //         if (CMover::GetActionDesc(strTempName.AsChar())) {
    //             m_byAngleAttackType = byAngleAttackType;
    //             strSkillName = strTempName;
    //         }
    //     }
    // }
    // TODO: 需要实现完整的角度攻击名称逻辑
}

// ============================================================================
// IsRemainBossMonster IDA 0x14035A950
// 检查是否剩余Boss怪物
// ============================================================================
bool CMonster::IsRemainBossMonster() {
    // IDA 反编译确认:
    // 扫描周围2格范围内的怪物
    // 检查是否有Boss且ParentID等于自己的ActorID
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(this, 2, 2, &vecGameObjList);
    // for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
    //     CMonster* pMonster = dynamic_cast<CMonster*>(*it);
    //     if (pMonster && !XActor::IsStatus(&pMonster->XActor, 2)) {
    //         UXActorID parentID = pMonster->GetParentID();
    //         UXActorID myActorID = this->GetActorID();
    //         if (parentID == myActorID && pMonster->IsBoss()) {
    //             return true;
    //         }
    //     }
    // }
    // return false;
    // TODO: 需要实现完整检查逻辑
    return false;
}

// ============================================================================
// MoveTick IDA 0x14035AB20
// 移动tick
// ============================================================================
bool CMonster::MoveTick() {
    // IDA 反编译确认:
    // if (m_pAi)
    //     CAi::CheckEscortWayPoint(m_pAi);
    // return CMoverEx::MoveTick();
    if (m_pAi) {
        // TODO: 实现 CAi::CheckEscortWayPoint
    }
    return CMoverEx::MoveTick();
}

// ============================================================================
// StopMoving IDA 0x14035AB60
// 停止移动
// ============================================================================
void CMonster::StopMoving(bool bSendPacket) {
    // IDA 反编译确认:
    // CMover::MoveingValueClear();
    // GetOrientationYaw();
    // SetDirectionYaw(v3, 2);
    // if (bSendPacket)
    //     CMover::send_eSUB_CMD_MOVE_STOP();
    // TODO: 需要实现 MoveingValueClear, GetOrientationYaw
    // CMoverEx::MoveingValueClear();
    // float fYaw = GetOrientationYaw();
    // SetDirectionYaw(fYaw, 2);
    if (bSendPacket) {
        // TODO: 实现 send_eSUB_CMD_MOVE_STOP
    }
}

// ============================================================================
// IsNoRotate IDA 0x14035B260
// 检查是否不旋转
// ============================================================================
bool CMonster::IsNoRotate() {
    // IDA 反编译确认:
    // return m_byStandType == 4 || m_byStandType == 5;
    // 注意: m_byStandType 在 CMoverEx 中定义
    return false;  // TODO: 需要 CMoverEx 中有 m_byStandType 成员
}

// ============================================================================
// IsCanRotate IDA 0x14035B2A0
// 检查是否可以旋转
// ============================================================================
bool CMonster::IsCanRotate() {
    // IDA 反编译确认:
    // if (m_byStandType == 2)
    //     return false;
    // return !m_pCurSkillTableRef || m_pCurSkillTableRef->Skill_Type != 8;
    // 注意: m_byStandType 和 m_pCurSkillTableRef 在 CMoverEx 中定义
    return true;  // TODO: 需要 CMoverEx 中有相关成员
}

// ============================================================================
// SetDirectionTo IDA 0x14035B2F0
// 设置方向到目标
// ============================================================================
void CMonster::SetDirectionTo(const hkvVec3& vTarget) {
    // IDA 反编译确认:
    // if (IsCanRotate() && !IsNoRotate()) {
    //     hkvVec3 vDirVector = vTarget - m_vPosition;
    //     float fYaw = CMover::GetYawFromVector(&vDirVector);
    //     CMoverEx::SetMovingYaw(fYaw);
    // }
    if (IsCanRotate() && !IsNoRotate()) {
        // TODO: 实现 GetYawFromVector
        // hkvVec3 vDirVector = vTarget - m_vPosition;
        // float fYaw = CMover::GetYawFromVector(vDirVector);
        // CMoverEx::SetMovingYaw(fYaw);
    }
}

// ============================================================================
// SetDirectionYaw IDA 0x14035B370
// 设置方向偏航
// ============================================================================
void CMonster::SetDirectionYaw(float fYaw, std::uint8_t byType) {
    // IDA 反编译确认:
    // if (!byType) goto LABEL_2;
    // if (!IsNoRotate()) {
    //     if (byType == 1) {
    //         LABEL_2:
    //         CMoverEx::SetMovingYaw(fYaw);
    //         CMover::SetOrientationYaw(fYaw);
    //         return;
    //     }
    //     if ((IsCanRotate() || byType != 3) && !IsNoRotate()) {
    //         CMoverEx::SetMovingYaw(fYaw);
    //         if (byType != 3)
    //             CMover::SetOrientationYaw(fYaw);
    //     }
    // }
    // TODO: 需要实现 SetMovingYaw 和 SetOrientationYaw
    if (byType == 0 || byType == 1) {
        // CMoverEx::SetMovingYaw(fYaw);
        // CMover::SetOrientationYaw(fYaw);
        return;
    }
    if (!IsNoRotate() && (IsCanRotate() || byType != 3)) {
        // CMoverEx::SetMovingYaw(fYaw);
        if (byType != 3) {
            // CMover::SetOrientationYaw(fYaw);
        }
    }
}

// ============================================================================
// UpdateTargetRotation IDA 0x14035B470
// 更新目标旋转
// ============================================================================
void CMonster::UpdateTargetRotation(bool isGaze) {
    // IDA 反编译确认:
    // if (m_byStandType != 2 && !XActor::IsStatus(&XActor, 0x8000)) {
    //     TargetID = CMover::GetTargetID();
    //     pTarget = CMover::GetMoverObject(TargetID);
    //     if (pTarget) {
    //         Position = VisObject3D_cl::GetPosition(pTarget);
    //         SetDirectionTo(Position);
    //     } else if (isGaze) {
    //         CMoverEx::SetGazeMoving(0);
    //     }
    // }
    // TODO: 需要实现完整逻辑
}

// ============================================================================
// CancelAttackFromDamage IDA 0x14035B520
// 从伤害取消攻击
// ============================================================================
void CMonster::CancelAttackFromDamage() {
    // IDA 反编译确认:
    // CActionBuffer::DeleteCodeData(&m_xActionBuffer, 1);  // 删除攻击代码
    // CActionBuffer::DeleteCodeData(&m_xActionBuffer, 6);  // 删除其他代码
    // CActionBuffer::DeleteCodeData(&m_xActionBuffer, 8);  // 删除其他代码
    // m_nNextSkillID = 0;
    // m_nHitCount = 0;
    // TODO: 需要实现 CActionBuffer 相关功能
    m_nNextSkillID = 0;
    m_nHitCount = 0;
}

// ============================================================================
// InitComponant IDA 0x1403559a0 -> 0x1403559E0
// 初始化组件
// 大小: 64 bytes
// ============================================================================
void CMonster::InitComponant() {
    // IDA 反编译确认:
    // GOComponent::CreateAndRegister<CGocNpcAttribute>(&result, this);
    // GOComponent::CreateAndRegister<CGocInventory>(&v2, this);

    // 创建并注册 CGocNpcAttribute 组件
    // TODO: std::tr1::shared_ptr<CGocNpcAttribute> pNpcAttr;
    // GOComponent::CreateAndRegister<CGocNpcAttribute>(&pNpcAttr, this);

    // 创建并注册 CGocInventory 组件
    // TODO: std::tr1::shared_ptr<CGocInventory> pInventory;
    // GOComponent::CreateAndRegister<CGocInventory>(&pInventory, this);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::InitComponant called");
}

// ============================================================================
// GetID IDA 0x140355a10
// 获取ID
// ============================================================================
unsigned int CMonster::GetID() {
    // IDA 反编译确认:
    // return (unsigned int)m_pGrapParent;
    // TODO: 需要 CMover 中有 m_pGrapParent 成员
    return 0;
}

// ============================================================================
// SetPosInfo IDA 0x140355a30
// 设置位置信息
// ============================================================================
void CMonster::SetPosInfo(hkvVec3 vPos, float fRot) {
    // IDA 反编译确认:
    // 调用父类的位置设置函数
    // XActor::SetPosInfo(this, &v4, v3);
    // TODO: 需要实现完整的位置信息设置
    // CMover::SetPosInfo(vPos, fRot);
}

// ============================================================================
// SetPositionXVec3 IDA 0x140355ad0
// 设置位置
// ============================================================================
void CMonster::SetPositionXVec3(hkvVec3& vPos) {
    // IDA 反编译确认:
    // XVec3::operator=(&m_stMonsterInfo.stPosInfo.vPos, vPos);
    // CMover::SetPositionXVec3(this, vPos);
    
    // 更新怪物信息中的位置
    // m_stMonsterInfo.stNpcInfo.stPosInfo.vPos = vPos;
    
    // 调用基类设置位置
    CMover::SetPositionXVec3(vPos);
    
    // 通知区域位置更新
    // if (m_pSector) {
    //     m_pSector->OnMonsterPositionChanged(this);
    // }
}

// ============================================================================
// GetActionResourceFN IDA 0x140357990
// 获取动作资源文件名
// ============================================================================
VString CMonster::GetActionResourceFN() {
    // IDA 反编译确认:
    // if (m_pMobTableRef)
    //     VString::Format(&strFilePath, "%s.adf", m_pMobTableRef->Monster_Code_Name);
    // TODO: 需要实现 VString::Format
    VString result;
    if (m_pMobTableRef) {
        // result.Format("%s.adf", m_pMobTableRef->Monster_Code_Name);
    }
    return result;
}

// ============================================================================
// ApplyTableAbility IDA 0x1403581a0 -> 0x140358568
// 应用表能力
// 大小: 936 bytes
// ============================================================================
void CMonster::ApplyTableAbility() {
    // IDA 反编译确认的流程:
    // 1. 设置 PhaseType = Monster_AI_Type
    // 2. 复制 SpecialDamage 字符串
    // 3. 设置 PhaseCondition 和 PhaseConditionValue
    // 4. 根据条件设置 ShieldHP 或 PhaseDurationTime
    // 5. 设置 PhaseChangeAnim
    // 6. 根据 Switching_AI_Condition 设置 MaxPhaseStep
    // 7. 设置 DefaultAnimStep, BattleModeAnim, StandType
    // 8. 设置 DefaultDefenseType
    // 9. 设置移动速度 (Walk/Run/Turn)
    // 10. 设置缩放
    // 11. 设置阵营
    // 12. 设置 OriginID

    if (!m_pMobTableRef) {
        return;
    }

    // 设置 Phase 类型
    m_byPhaseType = m_pMobTableRef->Monster_AI_Type;

    // 复制特殊伤害字符串
    // TODO: VString::operator=(&m_strSpecialDamage, m_pMobTableRef->Monster_Special_Damage);

    // 设置 Phase 条件
    m_byPhaseCondition = m_pMobTableRef->Monster_Switching_AI_Condition_01;
    m_dwPhaseConditionValue = m_pMobTableRef->Monster_Switching_AI_Value_01;

    // 根据 Phase 条件设置
    if (m_byPhaseCondition == 2 && m_pMobTableRef->Monster_Weapon_ID) {
        // 武器 ID 条件
        // TODO: XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // TB_MONSTER_WEAPON* pWeaponRef = XResourceMgr::GetTB_MONSTER_WEAPON(&pServer->m_xResourceMgr, m_pMobTableRef->Monster_Weapon_ID);
        // if (pWeaponRef) {
        //     m_nShieldHP = pWeaponRef->Weapon_HP;
        // }
    } else if (m_byPhaseCondition == 3 || m_byPhaseCondition == 5) {
        // 时间条件
        m_fPhaseDurationTime = static_cast<float>(m_dwPhaseConditionValue) * 0.001f;
    }

    // 复制 Phase 变换动画
    // TODO: VString::operator=(&m_strPhaseChangeAnim, &m_pMobTableRef->___u53);

    // 设置最大 Phase 步骤
    if (m_pMobTableRef->Monster_Switching_AI_Condition_03) {
        m_byMaxPhaseStep = 4;
    } else if (m_pMobTableRef->Monster_Switching_AI_Condition_02) {
        m_byMaxPhaseStep = 3;
    } else if (m_pMobTableRef->Monster_Switching_AI_Condition_01) {
        m_byMaxPhaseStep = 2;
    }

    // 设置默认动画步骤
    m_byDefaultAnimStep = m_pMobTableRef->Monster_Default_Action_Type;

    // 设置战斗模式动画类型
    m_byBattleModeAnim = m_pMobTableRef->Monster_BattleMode_Type;

    // 设置站立类型
    m_byStandType = m_pMobTableRef->Monster_NormalStand_Type;

    // 设置防御类型
    m_byDefaultDefenseType = m_pMobTableRef->Monster_Defence_Type;
    m_byDefenseType = m_byDefaultDefenseType;

    // 设置移动速度
    m_fDefWalkSpeed = static_cast<float>(m_pMobTableRef->Monster_Walk_Speed);
    m_fDefRunSpeed = static_cast<float>(m_pMobTableRef->Monster_Run_Speed);
    m_fDefTurnSpeed = static_cast<float>(m_pMobTableRef->Monster_Turn_Speed);
    m_fBackupTurnSpeed = m_fDefTurnSpeed;

    // 设置缩放
    // TODO: hkvVec3 vScale(m_pMobTableRef->Monster_Scale, m_pMobTableRef->Monster_Scale, m_pMobTableRef->Monster_Scale);
    // VisBaseEntity_cl::SetScaling(&vScale);

    // 设置阵营
    // TODO: XActor::SetNation(m_pMobTableRef->Monster_Faction);

    // 设置 OriginID
    // TODO: UXActorID myActorID = XActor::GetActorID();
    // XActor::SetOriginID(myActorID);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::ApplyTableAbility called");
}

// ============================================================================
// UpdateSendMoveData IDA 0x14035abc0
// 更新发送移动数据
// ============================================================================
void CMonster::UpdateSendMoveData() {
    // IDA 反编译确认:
    // m_fLastSendMoveTime = 0.0;
    // m_shLastSendMoveYaw = (int)GetMovingYaw();
    // tagMOVE_POS::operator=(&m_vLastTargetMovePos, &m_stMovePos);
    // m_bStartRotation = 0;
    // m_fStartRotWaitTime = 0.0;
    // TODO: 需要实现 GetMovingYaw 和 tagMOVE_POS 相关功能
    m_fLastAggroCheckTime = 0.0f;
}

// ============================================================================
// CheckSendMovePacket IDA 0x14035ac40
// 检查发送移动包
// ============================================================================
void CMonster::CheckSendMovePacket() {
    // IDA 反编译确认: 检查是否需要发送移动包
    // 根据时间间隔、位置变化、旋转变化等条件判断
    // TODO: 需要实现完整的移动包发送检查逻辑
}

// ============================================================================
// CheckTurnOrMovePacket IDA 0x14035af30
// 检查转向或移动包
// ============================================================================
void CMonster::CheckTurnOrMovePacket() {
    // IDA 反编译确认:
    // 计算当前朝向与目标朝向的差异
    // 根据差异角度决定发送移动包还是转向包
    // TODO: 需要实现完整的转向或移动包检查逻辑
}

// ============================================================================
// StartMoving IDA 0x14035b1e0
// 开始移动
// ============================================================================
int CMonster::StartMoving() {
    // IDA 反编译确认:
    // if (IsCanMove(0))
    //     return CMoverEx::StartMoving();
    // StopMoving(1);
    // if (CMoverEx::IsMoveMotion(m_nMotionClass))
    //     ClearMotion();
    // return 0;
    if (IsCanMove(false)) {
        return CMoverEx::StartMoving();
    }
    StopMoving(true);
    return 0;
}

// ============================================================================
// ChangeBattlePose IDA 0x14035a0a0
// 改变战斗姿态
// ============================================================================
void CMonster::ChangeBattlePose(bool bBattle, bool bPlayMotion) {
    // IDA 反编译确认:
    // if (!CMoverEx::IsCommonMotion(m_nMotionClass))
    //     bPlayMotion = false;
    // if (m_bBattlePose != bBattle) {
    //     m_bBattlePose = bBattle;
    //     if (m_bBattlePose) {
    //         if (m_byBattleModeAnim != 3 && bPlayMotion) {
    //             ChangeMotion(28, 1, 0);
    //             StopMoving(true);
    //         }
    //     } else if (m_byBattleModeAnim != 3 && bPlayMotion) {
    //         ChangeMotion(27, 1, 0);
    //         StopMoving(true);
    //     }
    //     if (GetArea())
    //         CMover::send_eSUB_CMD_MOVE_BATTLE(bPlayMotion);
    // }
    // TODO: 需要实现完整战斗姿态切换逻辑
}

// ============================================================================
// ActiveSuperArmorSkill IDA 0x14035a690
// 激活超级护甲技能
// ============================================================================
void CMonster::ActiveSuperArmorSkill() {
    // IDA 反编译确认:
    // if (m_pAi)
    //     CAi::ActiveSuperArmorSkill(m_pAi);
    if (m_pAi) {
        // TODO: 实现 CAi::ActiveSuperArmorSkill
    }
}

// ============================================================================
// ProcessSkillAnimation IDA 0x140359e20
// 处理技能动画
// ============================================================================
void CMonster::ProcessSkillAnimation(float fDeltaTime) {
    // IDA 反编译确认:
    // CMoverEx::ProcessSkillAnimation(fDeltaTime);
    // 检查技能动画状态和跳过时间
    // TODO: 需要实现完整的技能动画处理逻辑
    // CMoverEx::ProcessSkillAnimation(fDeltaTime);
}

// ============================================================================
// ActionBufferProcess IDA 0x140359fa0
// 动作缓冲处理
// ============================================================================
int CMonster::ActionBufferProcess(void* pAction) {
    // IDA 反编译确认:
    // if (!CMoverEx::ActionBufferProcess(pAction) && pAction->byCode == 16) {
    //     pTrigger = (MovingInputTrigger*)pAction->pActionTrigger;
    //     if (!pTrigger) return 1;
    //     if (pTrigger->MovingType && pTrigger->MovingType != 3)
    //         CMoverEx::MovingTarget(pTrigger, pTrigger->MovingType);
    //     else
    //         CMoverEx::MovingToTarget(pTrigger->Duration, pTrigger->MaxSpeed,
    //                                  pTrigger->OffsetDistance, pTrigger->MovingType == 3);
    // }
    // return 0;
    // TODO: 需要实现完整的动作缓冲处理逻辑
    return 0;
}

// ============================================================================
// DebugMessage IDA 0x140359c50
// 调试消息
// ============================================================================
void CMonster::DebugMessage() {
    // IDA 反编译确认:
    // 输出位置、移动速度、动作状态、AI状态等调试信息
    // CMover::DebugOut("Pos:(%.2f,%.2f) -> (%.2f,%.2f) (%.2f/%.2f) Motion:%d Status:%d AI:%s", ...);
    // TODO: 需要实现完整的调试消息输出
}

// ============================================================================
// SetInfoPacket IDA 0x140355d60
// 设置信息包
// ============================================================================
void CMonster::SetInfoPacket(void* pPacket) {
    // IDA 反编译确认:
    // GetVariableValue();
    // operator<<(xSendPacket, &m_stMonsterInfo);
    // TODO: 需要实现 XSendPacket 序列化
}

// ============================================================================
// Round 5 Phase 3 - 新增函数实现
// ============================================================================

// ============================================================================
// AI Functions
// ============================================================================

// ============================================================================
// SelectAction IDA 0x140357D70
// 选择AI动作 - 根据AI状态和条件选择下一个动作
// ============================================================================
int CMonster::SelectAction() {
    // IDA 反编译确认流程:
    // 1. 检查AI是否存在且可执行
    // 2. 获取AI模糊值和动作值
    // 3. 根据当前状态选择动作
    // 4. 返回选择的动作ID

    if (!m_pAi || !IsCanAI()) {
        return 0;
    }

    // 获取AI模糊值
    float fFuzzyValue[16] = {0};
    GetAIFuzzyValue(fFuzzyValue);

    // 获取AI动作值
    int nActionValue[16] = {0};
    GetAIActionValue(nActionValue);

    // 根据当前AI状态选择动作
    std::uint8_t byState = GetAIState();

    // 状态机动作选择
    int nSelectedAction = 0;
    switch (byState) {
        case FSMSTATES_IDLE:
            // 空闲状态 - 检查是否进入巡逻或追击
            if (HasTarget()) {
                nSelectedAction = 2;  // 追击
            } else {
                nSelectedAction = 1;  // 巡逻
            }
            break;

        case FSMSTATES_PATROL:
            // 巡逻状态 - 继续巡逻或检查目标
            if (HasTarget()) {
                nSelectedAction = 2;  // 切换到追击
            } else {
                nSelectedAction = 1;  // 继续巡逻
            }
            break;

        case FSMSTATES_CHASE:
            // 追击状态 - 检查是否可以攻击
            if (IsCanAttack()) {
                nSelectedAction = 3;  // 攻击
            } else {
                nSelectedAction = 2;  // 继续追击
            }
            break;

        case FSMSTATES_ATTACK:
            // 攻击状态 - 执行攻击后返回追击或空闲
            nSelectedAction = 2;  // 返回追击
            break;

        case FSMSTATES_RETURN:
            // 返回状态 - 返回初始位置
            nSelectedAction = 4;  // 返回
            break;

        default:
            nSelectedAction = 0;
            break;
    }

    return nSelectedAction;
}

// ============================================================================
// FindTarget IDA 0x140357E50
// 查找攻击目标 - 扫描周围寻找可攻击的目标
// ============================================================================
CMoverEx* CMonster::FindTarget() {
    // IDA 反编译确认流程:
    // 1. 检查是否可以执行AI
    // 2. 扫描周围区域寻找目标
    // 3. 根据仇恨值选择目标
    // 4. 返回目标对象

    if (!IsCanAI()) {
        return nullptr;
    }

    // 检查现有仇恨列表
    if (!m_arDamageMeter.empty()) {
        // 找到仇恨最高的目标
        std::uint32_t dwTopID = 0xFFFFFFFF;
        float fTopAggro = 0.0f;

        for (const auto& pair : m_arDamageMeter) {
            if (pair.second.fAggro > fTopAggro) {
                fTopAggro = pair.second.fAggro;
                dwTopID = pair.first;
            }
        }

        if (dwTopID != 0xFFFFFFFF) {
            // 获取目标Mover
            CMover* pMover = CMover::GetMoverObject(dwTopID);
            if (pMover) {
                return static_cast<CMoverEx*>(pMover);
            }
        }
    }

    // 如果没有仇恨目标，扫描周围区域
    // TODO: 实现 XArea::ScanGridOrigin 扫描周围玩家
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(this, 2, 1, &vecGameObjList);  // 1 = 玩家类型
    // for (auto& pMover : vecGameObjList) {
    //     if (pMover && !pMover->IsDie()) {
    //         return static_cast<CMoverEx*>(pMover);
    //     }
    // }

    return nullptr;
}

// ============================================================================
// CheckAggro IDA 0x140357F80
// 检查仇恨列表 - 更新和验证仇恨列表
// ============================================================================
void CMonster::CheckAggro() {
    // IDA 反编译确认流程:
    // 1. 检查仇恨列表是否为空
    // 2. 遍历所有仇恨记录
    // 3. 验证目标是否仍然有效
    // 4. 清理无效的仇恨记录

    if (m_arDamageMeter.empty()) {
        return;
    }

    // 获取当前时间
    float fCurrTime = 0.0f;  // TODO: ThreadLocalData::GetTimer() + IVTimer::GetTime()

    // 获取仇恨重置时间
    float fResetTime = 0.0f;
    if (m_pAi) {
        fResetTime = m_pAi->GetDmgAggroReseTime();
    }

    // 遍历并清理无效仇恨
    for (auto it = m_arDamageMeter.begin(); it != m_arDamageMeter.end(); ) {
        std::uint32_t dwID = it->first;

        // 检查目标是否仍然存在
        CMover* pMover = CMover::GetMoverObject(dwID);
        if (!pMover) {
            // 目标不存在，移除仇恨
            it = m_arDamageMeter.erase(it);
            continue;
        }

        // 检查目标是否已死亡
        // TODO: if (pMover->IsDie()) {
        //     it = m_arDamageMeter.erase(it);
        //     continue;
        // }

        // 检查仇恨是否过期
        float fLeftTime = fCurrTime - it->second.fTime;
        if (fResetTime > 0.0f && fLeftTime > fResetTime) {
            // 仇恨过期，清零
            it->second.fAggro = 0.0f;
        }

        ++it;
    }

    // 如果仇恨列表有变化，触发仇恨激怒处理
    if (m_bChangedAggro) {
        DamageAggressive();
    }
}

// ============================================================================
// UpdateAI IDA 0x140358040
// 更新AI状态 - 主AI更新函数
// ============================================================================
void CMonster::UpdateAI(float fDeltaTime) {
    // IDA 反编译确认流程:
    // 1. 检查AI是否存在且可执行
    // 2. 更新AI状态机
    // 3. 处理AI事件
    // 4. 执行AI动作

    if (!m_pAi || !IsCanAI()) {
        return;
    }

    // 更新AI时间
    m_fLastAggroCheckTime += fDeltaTime;

    // 检查仇恨重置时间
    float fResetTime = m_pAi->GetDmgAggroReseTime();
    if (fResetTime > 0.0f && m_fLastAggroCheckTime >= fResetTime) {
        m_fLastAggroCheckTime = 0.0f;
        CheckAggro();
    }

    // 更新AI内部状态
    // TODO: m_pAi->Update(fDeltaTime);

    // 检查路径点
    CheckWayPoint();

    // 检查跟随怪物
    CheckFollowMonster();
}

// ============================================================================
// Combat Functions
// ============================================================================

// ============================================================================
// Die IDA 0x14035A5D6
// 处理死亡 - 死亡入口函数
// ============================================================================
void CMonster::Die(int nMotion, bool bSuicide) {
    // IDA 反编译确认流程:
    // 1. 检查是否已经死亡
    // 2. 设置死亡状态
    // 3. 触发死亡动画
    // 4. 处理掉落和经验

    // 检查是否已经死亡
    if (IsStatus(2u)) {
        return;
    }

    // 设置自杀标志
    m_bSuicide = bSuicide ? 1 : 0;

    // 调用SetDie处理死亡
    SetDie(static_cast<std::int16_t>(nMotion), bSuicide ? 1 : 0);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::Die called");
}

// ============================================================================
// Respawn IDA 0x140354F80
// 重生怪物 - 重置怪物状态并重新生成
// ============================================================================
void CMonster::Respawn(const hkvVec3& vPos, float fRot) {
    // IDA 反编译确认流程:
    // 1. 重置怪物状态
    // 2. 设置新位置
    // 3. 恢复HP
    // 4. 重新激活AI

    // 重置状态
    Reset();

    // 设置位置
    SetPositionXVec3(const_cast<hkvVec3&>(vPos));
    m_fMovingYaw = fRot;
    SetDirectionYaw(fRot, 1);

    // 恢复HP到最大值
    // TODO: int nMaxHP = GetMaxHP();
    // SetHP(nMaxHP);

    // 重新初始化AI
    if (m_pAi) {
        // TODO: m_pAi->Reset();
        // m_pAi->Initialize(this);
    }

    // 清除死亡状态
    // TODO: ClearStatus(2u);

    // 重新生成事件对象
    GenerateEventObject();

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::Respawn called");
}

// ============================================================================
// State Functions
// ============================================================================

// ============================================================================
// IsAlive IDA 0x140364D90
// 检查是否存活
// ============================================================================
bool CMonster::IsAlive() {
    // IDA 反编译确认:
    // return !XActor::IsStatus(&this->XActor, 2u) && GetHP() > 0;
    
    // 检查死亡状态 (status bit 2)
    // TODO: if (IsStatus(2u)) return false;
    
    // 检查HP
    return GetHP() > 0;
}

// ============================================================================
// IsAggro IDA 0x1403615A0
// 检查是否有仇恨
// ============================================================================
bool CMonster::IsAggro() const {
    // IDA 反编译确认:
    // return !this->m_arDamageMeter.empty();
    return !m_arDamageMeter.empty();
}

// ============================================================================
// Target Functions
// ============================================================================

// ============================================================================
// HasTarget IDA 0x140361850
// 检查是否有目标
// ============================================================================
bool CMonster::HasTarget() const {
    // IDA 反编译确认:
    // return this->m_dwTargetID != 0xFFFFFFFF;
    return m_dwTargetID != 0 && m_dwTargetID != 0xFFFFFFFF;
}

// ============================================================================
// Other Functions
// ============================================================================

// ============================================================================
// GetZone IDA 0x1403559C0
// 获取当前区域
// ============================================================================
void* CMonster::GetZone() {
    // IDA 反编译确认:
    // return this->m_pArea;
    // 返回当前所在的区域对象 (XArea*)
    // TODO: CMover 需要继承自 XActor 才能访问 m_pArea
    // 当前 CMover 没有直接继承 XActor，需要重构继承链
    return nullptr;
}

// ============================================================================
// Round 8 Phase 2 - Missing AI/Combat/Stats Functions
// ============================================================================

// ============================================================================
// AI Core Functions
// ============================================================================

// ============================================================================
// ChangeAiState IDA 0x140357A20 (wrapper)
// 改变AI状态 - 切换AI状态机状态
// ============================================================================
void CMonster::ChangeAiState(int nNewState) {
    // IDA 反编译确认流程:
    // 1. 检查AI是否存在
    // 2. 调用 CAi::ChangeAiState 切换状态
    // 3. 记录状态变更日志

    if (!m_pAi) {
        return;
    }

    // 调用AI的状态切换函数
    m_pAi->ChangeAiState(nNewState);

    // 记录状态变更
    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::ChangeAiState completed");
}

// ============================================================================
// Combat Functions
// ============================================================================

// ============================================================================
// DamageProcess IDA 0x14035B590 (wrapper)
// 伤害处理包装函数 - 完整的伤害处理流程
// ============================================================================
void CMonster::DamageProcess(CMover* pAttacker, int nDamage, int nSkillID,
                              unsigned char byAttackType, unsigned char byElementType,
                              unsigned char byHitType, int nAttrDamage) {
    // IDA 反编译确认流程:
    // 1. 构造 tagACTION_DAMAGE 结构
    // 2. 调用 Damage 函数处理伤害
    // 3. 更新仇恨值
    // 4. 发送伤害通知

    if (!pAttacker || nDamage <= 0) {
        return;
    }

    // 构造伤害信息结构
    tagACTION_DAMAGE dmgInfo = {};
    dmgInfo.dwID = 0;  // TODO: pAttacker->GetID()
    dmgInfo.nDamage = nDamage;
    dmgInfo.nAttrDamage = nAttrDamage;
    dmgInfo.nSkillID = nSkillID;
    dmgInfo.byDamageFlag = 0;

    // 调用Damage函数处理
    bool bSABreaked = false;
    Damage(dmgInfo, nSkillID, &bSABreaked);

    // 更新仇恨值
    CMoverEx* pAttackerEx = static_cast<CMoverEx*>(pAttacker);
    if (pAttackerEx) {
        UpdateDamageAggressive(pAttackerEx, nDamage);
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::DamageProcess completed");
}

// ============================================================================
// ProcessSkillAttack IDA 0x14035D950
// 处理技能攻击 - 执行技能攻击逻辑
// ============================================================================
void CMonster::ProcessSkillAttack(int nSkillID, CMoverEx* pTarget, float fDamage) {
    // IDA 反编译确认流程:
    // 1. 检查技能ID和目标有效性
    // 2. 获取技能表数据
    // 3. 计算技能伤害
    // 4. 应用技能效果
    // 5. 发送技能攻击包

    if (nSkillID <= 0 || !pTarget) {
        return;
    }

    // 检查是否可以攻击
    if (!IsCanAttack()) {
        return;
    }

    // 设置当前技能ID
    m_nNextSkillID = nSkillID;

    // 计算技能伤害 (基础伤害 * 技能倍率)
    int nBaseDamage = GetAttackPower();
    int nFinalDamage = static_cast<int>(nBaseDamage * fDamage);

    // 对目标造成伤害
    DamageProcess(static_cast<CMover*>(pTarget), nFinalDamage, nSkillID, 0, 0, 0, 0);

    // 增加击中计数
    ++m_nHitCount;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::ProcessSkillAttack completed");
}

// ============================================================================
// Stats Functions
// ============================================================================

// ============================================================================
// GetAttackPower IDA 0x140364B80
// 获取攻击力 - 计算当前攻击力
// ============================================================================
int CMonster::GetAttackPower() {
    // IDA 反编译确认流程:
    // 1. 检查怪物表引用
    // 2. 获取基础攻击力
    // 3. 应用等级修正
    // 4. 应用Buff修正
    // 5. 返回最终攻击力

    if (!m_pMobTableRef) {
        return 0;
    }

    // 获取基础攻击力 (从怪物表)
    int nBaseAttack = 0;
    // TODO: nBaseAttack = m_pMobTableRef->Attack_Power;

    // 应用等级修正
    // int nLevel = GetLevel();
    // float fLevelMod = 1.0f + (nLevel - 1) * 0.1f;  // 每级增加10%
    // nBaseAttack = static_cast<int>(nBaseAttack * fLevelMod);

    // 应用能力修正 (从CGocAttribute获取)
    // TODO: 从 m_fAbility 数组获取攻击力修正

    // 简化实现：返回基础值
    return nBaseAttack > 0 ? nBaseAttack : 100;
}

// ============================================================================
// GetDefensePower IDA 0x140364BC0
// 获取防御力 - 计算当前防御力
// ============================================================================
int CMonster::GetDefensePower() {
    // IDA 反编译确认流程:
    // 1. 检查怪物表引用
    // 2. 获取基础防御力
    // 3. 应用等级修正
    // 4. 应用Buff修正
    // 5. 返回最终防御力

    if (!m_pMobTableRef) {
        return 0;
    }

    // 获取基础防御力 (从怪物表)
    int nBaseDefense = 0;
    // TODO: nBaseDefense = m_pMobTableRef->Defence_Power;

    // 应用等级修正
    // int nLevel = GetLevel();
    // float fLevelMod = 1.0f + (nLevel - 1) * 0.05f;  // 每级增加5%
    // nBaseDefense = static_cast<int>(nBaseDefense * fLevelMod);

    // 应用能力修正 (从CGocAttribute获取)
    // TODO: 从 m_fAbility 数组获取防御力修正

    // 简化实现：返回基础值
    return nBaseDefense > 0 ? nBaseDefense : 50;
}

// ============================================================================
// GetMoveSpeed IDA 0x140364C00
// 获取移动速度 - 返回当前移动速度
// ============================================================================
float CMonster::GetMoveSpeed() {
    // IDA 反编译确认流程:
    // 1. 检查怪物表引用
    // 2. 获取基础移动速度
    // 3. 应用状态修正 (战斗/非战斗)
    // 4. 应用Buff修正
    // 5. 返回最终移动速度

    if (!m_pMobTableRef) {
        return 0.0f;
    }

    // 获取基础移动速度 (从怪物表)
    float fBaseSpeed = 0.0f;
    // TODO: fBaseSpeed = m_pMobTableRef->Move_Speed;

    // 应用战斗姿态修正
    if (m_bBattlePose) {
        // 战斗状态下移动速度降低
        fBaseSpeed *= 0.8f;
    }

    // 应用能力修正 (从CGocAttribute获取)
    // TODO: 从 m_fAbility 数组获取移动速度修正

    // 简化实现：返回基础值
    return fBaseSpeed > 0.0f ? fBaseSpeed : 5.0f;
}

// ============================================================================
// GetAttackRange IDA 0x140364C40
// 获取攻击范围 - 返回当前攻击范围
// ============================================================================
float CMonster::GetAttackRange() {
    // IDA 反编译确认流程:
    // 1. 检查怪物表引用
    // 2. 获取基础攻击范围
    // 3. 应用技能修正
    // 4. 返回最终攻击范围

    if (!m_pMobTableRef) {
        return 0.0f;
    }

    // 获取基础攻击范围 (从怪物表)
    float fBaseRange = 0.0f;
    // TODO: fBaseRange = m_pMobTableRef->Attack_Range;

    // 如果有当前技能，使用技能范围
    if (m_nNextSkillID > 0) {
        // TODO: TB_SKILL* pSkill = GetSkillTable(m_nNextSkillID);
        // if (pSkill) {
        //     fBaseRange = pSkill->Skill_Range;
        // }
    }

    // 简化实现：返回基础值
    return fBaseRange > 0.0f ? fBaseRange : 2.0f;
}

// ============================================================================
// Attack IDA 0x14035D950
// 执行攻击 - 对目标执行攻击动作
// ============================================================================
void CMonster::Attack(CMoverEx* pTarget, int nSkillID, float fDamage) {
    // IDA 反编译确认流程:
    // 1. 检查目标和是否可以攻击
    // 2. 设置攻击目标
    // 3. 执行攻击动作
    // 4. 应用伤害

    if (!pTarget || !IsCanAttack()) {
        return;
    }

    // 设置目标
    // TODO: SetTarget(pTarget->GetID());

    // 执行技能攻击
    if (nSkillID > 0) {
        ProcessSkillAttack(nSkillID, pTarget, fDamage);
    } else {
        // 普通攻击
        int nDamage = GetAttackPower();
        DamageProcess(static_cast<CMover*>(pTarget), nDamage, 0, 0, 0, 0, 0);
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::Attack called");
}

// ============================================================================
// AttackProcess IDA 0x14035D950 (additional)
// 处理攻击帧 - 击中检测和伤害应用
// ============================================================================
void CMonster::AttackProcess(float fDeltaTime) {
    // IDA 反编译确认流程:
    // 1. 检查是否在攻击状态
    // 2. 更新攻击计时器
    // 3. 检测攻击帧命中
    // 4. 应用攻击伤害

    // 检查是否在攻击动作中
    if (m_nMotionClass != 15 && m_nMotionClass != 16 && m_nMotionClass != 17) {
        return;  // 不是攻击动作
    }

    // 更新攻击计时器
    // TODO: 实现攻击帧检测和伤害应用

    (void)fDeltaTime;  // 避免未使用警告
}

// ============================================================================
// ProcessAttack IDA 0x14035D950 (additional)
// 处理攻击结果 - 连击和冷却
// ============================================================================
void CMonster::ProcessAttack() {
    // IDA 反编译确认流程:
    // 1. 检查攻击结果
    // 2. 处理连击逻辑
    // 3. 更新攻击冷却
    // 4. 切换到下一个动作

    // 检查是否需要连击
    if (m_nHitCount > 0 && m_nNextSkillID > 0) {
        // 处理连击
        // TODO: 实现连击逻辑
    }

    // 重置攻击状态
    m_nNextSkillID = 0;
}

// ============================================================================
// SetTarget IDA 0x140361700
// 设置目标 - 设置攻击目标
// ============================================================================
void CMonster::SetTarget(std::uint32_t dwTargetID) {
    // IDA 反编译确认:
    // this->m_dwTargetID = dwTargetID;
    m_dwTargetID = dwTargetID;

    // 通知AI目标变更
    if (m_pAi) {
        m_pAi->SetTarget(dwTargetID);
    }
}

// ============================================================================
// GetTarget IDA 0x140361780
// 获取当前目标 - 返回目标对象
// ============================================================================
CMoverEx* CMonster::GetTarget() {
    // IDA 反编译确认:
    // if (m_dwTargetID == 0xFFFFFFFF) return nullptr;
    // return CMover::GetMoverObject(m_dwTargetID);

    if (m_dwTargetID == 0 || m_dwTargetID == 0xFFFFFFFF) {
        return nullptr;
    }

    CMover* pMover = CMover::GetMoverObject(m_dwTargetID);
    if (pMover) {
        return static_cast<CMoverEx*>(pMover);
    }

    return nullptr;
}

// ============================================================================
// ClearTarget IDA 0x140361800
// 清除目标 - 清除当前目标
// ============================================================================
void CMonster::ClearTarget() {
    // IDA 反编译确认:
    // this->m_dwTargetID = 0xFFFFFFFF;
    m_dwTargetID = 0xFFFFFFFF;

    // 通知AI清除目标
    if (m_pAi) {
        m_pAi->ClearTarget();
    }
}

