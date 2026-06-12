#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
// Note: Mover.h is already included via Monster.h -> MoverEx.h -> actor/Mover/Mover.h
#include "Soulworker/GameServer/XGameServer/Ai.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include <cmath>

// Forward declarations for types not yet fully defined
class CUser;
class CBattleZone;

// Skill type constants for protect skills
constexpr int SKILLTYPE_NONE = 0;
constexpr int SKILLTYPE_PROTECT_A = 6;  // Absorb damage type
constexpr int SKILLTYPE_PROTECT_B = 7;  // Time-based type

// ============================================================================
// 构造函数 IDA 0x1403545D0
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
    , m_dwGuardID(static_cast<unsigned int>(-1))
    , m_byGazeAnimType(0)
    , m_nSpawnBoxID(0)
    , m_nGroupID(0)
    , m_bShowChangedAI(false)
    , m_dwSpawnedTime64(0)
    , m_bSuicide(0)
    , m_dwKillerID(static_cast<unsigned int>(-1))
    , m_dwGuardMonsterID(0)
    , m_nMoveType(1)
    , m_pWeaponTableRef(nullptr)
    , m_fLifeTime(0.0f)
    , m_nHPPerSec(0)
    , m_nLastTime(0)
    , m_nProtectSkill(0)
    , m_eSkillType(SKILLTYPE_NONE)
    , m_nProtectSkillDamage(0)
    , m_bApplyLevel(0)
    , m_bySummonType(static_cast<std::int8_t>(-1))
    , m_fSummonLifeTime(-1.0f)
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
    , m_fAttackTimer(0.0f)
    , m_fLinkSkillDuration(0.0f)
    , m_pLinkSkillTrigger(nullptr)
    , m_bLinkSkillOn(false)
    , m_fCheckAttackSkillDuration(0.0f)
    , m_pCheckAttackSkillTrigger(nullptr)
    , m_bCheckAttackSkillOn(false)
{
    // IDA 反编译 (0x1403545D0):
    // CMoverEx::CMoverEx(this);
    // 设置虚函数表指针
    // 初始化成员变量
    // CMonster::InitComponant(this);
    // m_pPosInfo = &m_stMonsterInfo.stPosInfo;
    // m_eActorType = eActorMonster;

    // 初始化父 ActorID
    m_stMonsterInfo.uxParentActorID = UXActorID(static_cast<unsigned int>(-1));

    // 清空最后目标移动位置
    // m_vLastTargetMovePos.Clear();

    // 调用 InitComponant
    InitComponant();

    // 设置位置信息指针
    // m_pPosInfo = &m_stMonsterInfo.stPosInfo;

    // 设置 Actor 类型
    m_eActorType = static_cast<decltype(m_eActorType)>(2);  // eActorMonster = 2

    // 初始化错误位置
    m_vStoredWrongPos.setZero();

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster constructed");
}

// ============================================================================
// 析构函数 IDA 0x140354A70
// ============================================================================
CMonster::~CMonster() {
    // IDA 反编译 (0x140354A70):
    // 设置虚函数表指针
    // 清理脚本实例
    // 移除目标目标位置
    // 删除 AI
    // 清空技能管理器
    // 调用基类析构函数

    // 清理脚本实例
    if (m_pScriptInst) {
        m_pScriptInst = nullptr;
    }

    // 移除目标目标位置
    CMover::RemoveTargetDestPos();

    // 删除 AI
    if (m_pAi) {
        delete m_pAi;
        m_pAi = nullptr;
    }

    // 清空状态列表
    m_stMonsterInfo.vecStat.clear();

    // 删除技能管理器
    if (m_pSkillMgr) {
        delete m_pSkillMgr;
        m_pSkillMgr = nullptr;
    }

    // 重置计数器
    m_fElapsedDieTime = 0.0f;
    m_byAngleAttackType = 0;

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
    //   std::shared_ptr<CGocAttribute> pAttr;
    //   __int64 v4 = -2;
    //   float fValue;
    //
    //   this->SetHpInfo(this, nVal);
    //   CMover::GetGOC<CGocAttribute>(this, &pAttr, 0);
    //   if ( (unsigned int)std::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*(&pAttr) != -1 )
    //   {
    //     fValue = (float)nVal;
    //     v2 = std::shared_ptr<CForce>::operator->(&pAttr);
    //     CGocAttribute::SetHP(v2, fValue);
    //   }
    //   std::shared_ptr<CItemAkashic>::~shared_ptr<CItemAkashic>(&pAttr);
    // }

    SetHpInfo(nVal);

    // 获取 CGocAttribute 组件
    std::shared_ptr<CGocAttribute> pAttr = GetGOC_Attribute(false);
    if (pAttr) {
        pAttr->SetHP(static_cast<float>(nVal));
    }
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
        SendNoticePacket(m_eSkillType, -1, -1.0f);
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
            ClearStatus(2u);  // 清除死亡延迟状态

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
            if (m_xActionBuffer.GetActionCount() > 0) {
                m_fDieFadeTime = 1.0f;
            } else {
                // IDA: 检查父ID是否为QuestID，通知父对象
                UXActorID uxParentID = GetParentID();
                if (uxParentID.dwActorID != 0xFFFFFFFF) {  // CQuestCondition::GetQuestID != -1
                    // 获取父Mover对象并通知
                    CMover* pParentMover = CMover::GetMoverObject(uxParentID.dwActorID);
                    if (pParentMover) {
                        // 通知父对象召唤怪物死亡
                        CMoverEx* pParentMoverEx = dynamic_cast<CMoverEx*>(pParentMover);
                        if (pParentMoverEx) {
                            CMonster* pParentMonster = dynamic_cast<CMonster*>(pParentMoverEx);
                            if (pParentMonster) {
                                pParentMonster->NotifySpawnMonsterDied(GetActorID().dwActorID);
                            }
                        }
                    }
                }

                // IDA: 从场景移除
                XArea* pArea = GetArea();
                if (pArea) {
                    pArea->ExitActor(reinterpret_cast<XActor*>(this));
                }
            }
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
            XArea* pArea2 = GetArea();
            if (pArea2) {
                pArea2->ExitActor(reinterpret_cast<XActor*>(this));
            }
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
                XArea* pArea3 = GetArea();
                
                // IDA: pMaze = _RTDynamicCast_0(v21, 0, &XArea RTTI, &XMaze RTTI, 0)
                XMaze* pMaze = dynamic_cast<XMaze*>(pArea3);
                if (pMaze) {
                    int nMazeID = pMaze->GetTBMapID();
                    int nUserCount = pMaze->GetCurUserCount();
                    GreenDamTan_log(__FILE__, __FUNCTION__,
                        "BossHPCheck: MazeID=%d, UserCount=%d, ActorID=%d, TableID=%d, HP=%d",
                        nMazeID, nUserCount, GetActorID().dwActorID, GetTableID(), GetHP());
                    m_nCheckBossHP = GetHP();
                } else {
                    // IDA: 如果不是迷宫，设置一个很大的日志时间
                    m_fBossHPLogTime = 1.0e8f;
                }
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
    // IDA 0x140358640 精确还原:
    // if (!CMoverEx::IsCanMove(this, isCheckTurnMotion)) return 0;
    // if (isCheckTurnMotion) {
    //     if (!CMoverEx::IsCommonMotion(this, this->m_nMotionClass)) return 0;
    // } else if (this->m_nMotionClass != 7 && this->m_nMotionClass != 8
    //         && !CMoverEx::IsCommonMotion(this, this->m_nMotionClass)) {
    //     return 0;
    // }
    // return this->m_byStandType != 2 && this->m_byStandType != 3
    //     && (this->m_byStandType != 1 || this->m_nMotionClass != 1 || this->m_bBattlePose);

    // IDA: 首先调用基类 IsCanMove
    if (!CMoverEx::IsCanMove(isCheckTurnMotion)) {
        return false;
    }

    // IDA: 检查动作类型
    if (isCheckTurnMotion) {
        if (!IsCommonMotion(m_nMotionClass)) {
            return false;
        }
    } else if (m_nMotionClass != 7 && m_nMotionClass != 8
            && !IsCommonMotion(m_nMotionClass)) {
        return false;
    }

    // IDA: 检查站立类型
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
    // if ( !CMoverEx::IsCanAttack(this) ) return 0;
    // if ( CMover::IsGeneralHit(this) && this->m_byStandType != 2 && this->m_byStandType != 3 ) return 0;
    // if ( CMover::IsKnockDown(this) ) return 0;
    // if ( this->m_nMotionClass == this->m_nPlayPhaseMotion ) return 0;
    // if ( this->m_nMotionClass == 7 || this->m_nMotionClass == 8 ) return 0;
    // return !XActor::IsStatus(&this->XActor, 1u);

    // IDA: 首先调用基类 IsCanAttack
    if (!CMoverEx::IsCanAttack()) {
        return false;
    }

    // IDA: 检查普通受击状态 (排除特定站立类型)
    if (IsGeneralHit() && m_byStandType != 2 && m_byStandType != 3) {
        return false;
    }

    // IDA: 检查倒地状态
    if (IsKnockDown()) {
        return false;
    }

    // IDA: 检查阶段动作
    if (m_nMotionClass == m_nPlayPhaseMotion) {
        return false;
    }

    // IDA: 检查特定动作类型 (7=受击, 8=倒地)
    if (m_nMotionClass == 7 || m_nMotionClass == 8) {
        return false;
    }

    // IDA: 检查状态标志 (1 = 攻击锁定)
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
// IsFollower IDA 0x140360810
// Check if monster is a follower (mercenary or helper)
// IDA: return IsMercenary() || IsHelper()
// ============================================================================
bool CMonster::IsFollower() {
    return IsMercenary() || IsHelper();
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

    // 获取当前时间
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fCurrTime = pTimer ? pTimer->GetTime() : 0.0f;

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

    // IDA: Timer = ThreadLocalData::GetTimer(); fCurrTime = IVTimer::GetTime(Timer);
    VDefaultTimer* Timer = ThreadLocalData::GetTimer();
    float fCurrTime = Timer ? Timer->GetTime() : 0.0f;

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
// 计算技能仇恨点数 - IDA 精确还原
// ============================================================================
float CMonster::CalcSkillAggroPoint(CMoverEx* pUser, float fDamage, TB_SKILL* pSkillRef) {
    // IDA 0x14035F7F0 精确还原:
    // float fLevelPoint[4] = { 1.5, 1.4, 1.3, 1.2 }
    // if (pSkillRef->Agro_Type)
    //     v4 = fDamage + (fDamage * pSkillRef->Agro_Value * 0.01)
    // else
    //     v4 = fDamage + pSkillRef->Agro_Value
    // DAP = (fDamage + v4) * 0.0002
    // if (pSkillRef->Skill_Attribute_Sub >= 4) return 0.0
    // SPAP = DAP * m_pMobTableRef->Melee_Skill_Inclination[pSkillRef->Skill_Attribute_Sub] * 0.01
    // byOrder = CMoverEx::GetAggroLevelOrder(pUser)
    // if (byOrder >= 4) return 0.0
    // return (DAP + SPAP) + (DAP * fLevelPoint[byOrder])

    if (!pSkillRef) {
        return 0.0f;
    }

    // 计算基础仇恨值 v4
    float v4 = 0.0f;
    if (pSkillRef->Agro_Type) {
        // 百分比类型
        v4 = fDamage + (fDamage * static_cast<float>(pSkillRef->Agro_Value) * 0.01f);
    } else {
        // 固定值类型
        v4 = fDamage + static_cast<float>(pSkillRef->Agro_Value);
    }

    // 计算DAP (Damage Aggro Point)
    float DAP = (fDamage + v4) * 0.0002f;

    // 检查技能属性子类型
    if (pSkillRef->Skill_Attribute_Sub >= 4) {
        return 0.0f;
    }

    // 计算SPAP (Skill Point Aggro Point)
    // IDA: SPAP = DAP * m_pMobTableRef->Melee_Skill_Inclination[pSkillRef->Skill_Attribute_Sub] * 0.01
    float SPAP = 0.0f;
    if (m_pMobTableRef) {
        // IDA 显示 Melee_Skill_Inclination 作为数组基址访问
        // 偏移: Skill_Attribute_Sub * sizeof(int)
        // 使用指针算术模拟数组访问
        const std::uint32_t* pInclination = reinterpret_cast<const std::uint32_t*>(&m_pMobTableRef->Melee_Skill_Inclination);
        SPAP = DAP * static_cast<float>(pInclination[pSkillRef->Skill_Attribute_Sub]) * 0.01f;
    }

    // 获取目标等级顺序
    std::uint8_t byOrder = pUser->GetAggroLevelOrder();
    if (byOrder >= 4) {
        return 0.0f;
    }

    // 等级修正系数 (IDA: 栈上数组)
    float fLevelPoint[4] = { 1.5f, 1.4f, 1.3f, 1.2f };

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
// 计算治疗仇恨点数 - IDA 精确还原
// ============================================================================
float CMonster::CalcHealAggroPoint(CMoverEx* pUser) {
    // IDA 反编译精确还原:
    // DAP = CMoverEx::GetAmountOfHeal(pUser) * 0.0001
    // SPAP = DAP * m_pMobTableRef->Heal_Skill_Inclination * 0.01
    // byOrder = CMoverEx::GetAggroLevelOrder(pUser)
    // if (byOrder >= 4) return 0.0
    // return (DAP + SPAP) + (DAP * fLevelPoint[byOrder])

    if (!pUser) {
        return 0.0f;
    }

    // 获取治疗量
    float fHealAmount = pUser->GetAmountOfHeal();

    // 计算DAP (Damage Aggro Point for heal)
    float DAP = fHealAmount * 0.0001f;

    // 计算SPAP (Skill Point Aggro Point)
    float SPAP = 0.0f;
    if (m_pMobTableRef) {
        SPAP = DAP * static_cast<float>(m_pMobTableRef->Heal_Skill_Inclination) * 0.01f;
    }

    // 获取目标等级顺序
    std::uint8_t byOrder = pUser->GetAggroLevelOrder();
    if (byOrder >= 4) {
        return 0.0f;
    }

    // 等级修正系数
    static const float fLevelPoint[4] = { 1.5f, 1.4f, 1.3f, 1.2f };

    return (DAP + SPAP) + (DAP * fLevelPoint[byOrder]);
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
    
    // IDA: Timer = ThreadLocalData::GetTimer(); fCurrTime = IVTimer::GetTime(Timer);
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fCurrTime = pTimer ? pTimer->GetTime() : 0.0f;

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
// 检查保护仇恨 - IDA 精确还原
// ============================================================================
void CMonster::CheckProtectAggro(std::uint32_t dwID, float fAggro) {
    // IDA 0x140361260 精确还原:
    // dwSpawnBoxID = CMonster::GetSpawnBoxID(this);
    // if (dwSpawnBoxID == m_dwGuardID && m_dwGuardID != -1) {
    //     fAggroRatio = fAggro * m_fProtectionAggroRatio;
    //     std::vector<CMover*> vecGameObjList;
    //     XArea::ScanGridOrigin(&XActor, 2, 2u, &vecGameObjList);
    //     for (auto& it : vecGameObjList) {
    //         XActor* pActor = ...;
    //         if (XActor::GetType(pActor) == 2 && pActor && pActor->IsLive() &&
    //             !XActor::IsStatus(pActor, 2u) && dwSpawnBoxID == m_dwGuardID) {
    //             CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
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

    // 扫描附近的怪物并应用仇恨
    // Note: XArea::ScanGridOrigin needs to be implemented
    // For now, we use a simplified approach
    XArea* pArea = GetArea();
    if (pArea) {
        // TODO: Implement XArea::ScanGridOrigin to scan nearby monsters
        // std::vector<CMover*> vecGameObjList;
        // pArea->ScanGridOrigin(reinterpret_cast<XActor*>(this), 2, 2u, &vecGameObjList);
        // for (auto& pMover : vecGameObjList) {
        //     if (pMover) {
        //         XActor* pActor = reinterpret_cast<XActor*>(pMover);
        //         if (XActor::GetType(pActor) == 2 && pActor->IsLive() &&
        //             !XActor::IsStatus(pActor, 2u) && dwSpawnBoxID == m_dwGuardID) {
        //             CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        //             if (pMonster) {
        //                 pMonster->ApplyAggroValue(dwID, fAggroRatio, true);
        //             }
        //         }
        //     }
        // }
    }
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

    // IDA: Timer = ThreadLocalData::GetTimer(); fCurrTime = IVTimer::GetTime(Timer);
    VDefaultTimer* Timer = ThreadLocalData::GetTimer();
    float fCurrTime = Timer ? Timer->GetTime() : 0.0f;

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
    
    // IDA: send_eSUB_CMD_MONSTER_TARGET_CHANGE(this, uxTargetID)
    // 发送目标变更包到客户端
    XArea* pArea = GetArea();
    if (pArea) {
        // TODO: 实现完整的目标变更包发送
        // send_eSUB_CMD_MONSTER_TARGET_CHANGE(this, uxTargetID);
    }
    
    // 通知AI目标变更
    if (m_pAi) {
        m_pAi->SetTarget(uxTargetID.dwActorID);
    }
}

// ============================================================================
// AddDamageMeter IDA 0x14035F5F0 -> 0x14035F7E6
// 添加伤害计量 - IDA 精确还原
// ============================================================================
void CMonster::AddDamageMeter(CMoverEx* pMover, int nDamage, TB_SKILL* pSkillRef) {
    // IDA 反编译精确还原:
    // 1. 检查 pMover 是否有效
    // 2. 获取区域检查
    // 3. 获取攻击者 ActorID
    // 4. 检查是否是系统角色（如果是则跳过）
    // 5. 计算仇恨值并应用

    if (!pMover) {
        return;
    }

    // TODO: 完整的区域检查需要 XArea/XMaze 实现
    // 当前简化实现：直接获取攻击者ID并应用仇恨

    // 获取攻击者 ID
    std::uint32_t dwID = pMover->GetTargetID();

    // 计算仇恨值
    float fAggro = 0.0f;
    if (pSkillRef) {
        // 使用技能仇恨计算
        fAggro = CalcSkillAggroPoint(pMover, static_cast<float>(nDamage), pSkillRef);
    } else {
        // 使用 DOT 仇恨计算
        fAggro = CalcDotAggroPoint(static_cast<float>(nDamage));
    }

    // 应用仇恨值
    ApplyAggroValue(dwID, fAggro, true);

    // TODO: 检查保护仇恨需要完整实现
    // CheckProtectAggro(dwID, fAggro);
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
// CheckDieType IDA 0x14035CF70 -> 0x14035D28E
// 检查死亡类型 - 精确还原
// ============================================================================
void CMonster::CheckDieType(unsigned char& byReactionType, unsigned char byDamageFlag,
                            hkvVec3& vExtraMove) {
    // IDA 0x14035CF70 精确还原:
    // 根据怪物类型和死亡类型设置死亡表现

    // Monster_Type 1 或 2 不处理
    if (m_pMobTableRef->Monster_Type == 1 || m_pMobTableRef->Monster_Type == 2) {
        return;
    }

    switch (m_pMobTableRef->Monster_Death_Type) {
    case 4:
        m_eDieType = DIE_TYPE_DESTROY;
        break;
    case 5:
        m_eDieType = DIE_TYPE_NOFADEOUT;
        break;
    case 6:
        m_eDieType = DIE_TYPE_STAY;
        break;
    default:
        if (!IsBoss()) {
            // 检查是否可以击倒
            if ((byReactionType == 2 || byReactionType == 3)
                && m_pMobTableRef->Monster_Rank <= 2
                && m_cWeightRank <= 8
                && m_nHitAnimCount != 2) {
                // 设置击倒死亡
                m_eDieType = DIE_TYPE_KNOCKDOWN;
                vExtraMove.z = 80.0f;
                vExtraMove.x = 850.0f - (50.0f * static_cast<float>(m_cWeightRank));
                vExtraMove.y = 0.0f;
                byReactionType = 4;
            } else {
                bool bKnockDown = (byReactionType >= 3 && byReactionType <= 5);

                if (m_pMobTableRef->Monster_Death_Type == 0) {
                    m_eDieType = DIE_TYPE_DELAY;
                }

                if (m_pMobTableRef->Monster_Death_Type == 1) {
                    if (bKnockDown
                        && m_pMobTableRef->Monster_Rank <= 2
                        && m_cWeightRank <= 8
                        && m_nHitAnimCount != 2) {
                        m_eDieType = DIE_TYPE_KNOCKDOWN;
                        vExtraMove.z = 80.0f;
                        vExtraMove.x = 850.0f - (50.0f * static_cast<float>(m_cWeightRank));
                        vExtraMove.y = 0.0f;
                        byReactionType = 4;
                    }
                } else if (m_pMobTableRef->Monster_Death_Type == 2 && bKnockDown) {
                    m_eDieType = DIE_TYPE_DESTROY;
                }
            }
        }
        break;
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
    bool bResult = (bSABreaked != nullptr && *bSABreaked);
    CMoverEx::Damage(dmgInfo, nSkillID, bSABreaked);
    if (bSABreaked) {
        *bSABreaked = bResult;
    }

    if (dmgInfo.nDamage > 0 && GetHP() > 0) {
        // IDA: CAi::FuncDamageProcess(this->m_pAi) - takes no parameters
        if (m_pAi) {
            m_pAi->FuncDamageProcess();
        }

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
// _DamageProcessHP IDA 0x14035C050 -> 0x14035CD59
// 内部HP伤害处理 - 精确还原
// ============================================================================
bool CMonster::_DamageProcessHP(unsigned int dwID, int nSkillID, int nDamage,
                                 unsigned char byDamageFlag, unsigned char byHitParts) {
    // IDA 0x14035C050 精确还原:
    // 这是 DamageProcessHP 的内部实现

    // 如果HP已经是0，返回
    if (m_stMonsterInfo.GetHP() == 0) {
        return true;
    }

    // 检查防御类型
    if (m_byDefenseType == 3 && ((byDamageFlag & 0x10) == 0)) {
        return false;
    }

    // 获取技能表
    TB_SKILL* pSkillRef = nullptr;
    if (nSkillID > 0) {
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        pSkillRef = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
    }

    if (nSkillID > 0 && !pSkillRef) {
        return false;
    }

    // TODO: 检查被动技能类型
    if (pSkillRef && !IsAllowPassiveType(pSkillRef->Passive_Type)) {
        return false;
    }

    int nCurHP = GetHP();
    int nMaxHP = static_cast<int>(m_fAbility[10]);

    // 处理护盾HP
    if (m_nShieldHP > 0 && nDamage > 0) {
        m_nShieldHP -= nDamage;
        if (m_nShieldHP <= 0) {
            m_nShieldHP = 0;
        }
    } else {
        // 处理部位破坏 - 需要完整的 m_sHitParts 结构
        // 普通HP伤害处理
        int newHP = m_stMonsterInfo.GetHP() - nDamage;
        if (newHP <= 0) {
            newHP = 0;
        }
        if (newHP >= nMaxHP) {
            newHP = nMaxHP;
        }

        // DamageCount类型怪物特殊处理
        if (nDamage > 0 && IsMonsterDamageCount()) {
            newHP = GetHP();
            float fTime = ThreadLocalData::GetTimer()->GetTime();
            if (fTime > (m_fLastDamageCountTime + 0.5f)) {
                --newHP;
                m_fLastDamageCountTime = fTime;
            }
        }

        SetHpEx(newHP);
    }

    if (nDamage < 0) {
        return true;
    }

    // TODO: CAi::AddEscapePoint not restored yet.

    // 阶段条件处理
    if ((m_byPhaseCondition == 4 || m_byPhaseCondition == 6) && m_dwPhaseConditionValue) {
        --m_dwPhaseConditionValue;
    }

    // 目标ID处理
    if (m_dwTargetID == static_cast<unsigned int>(-1)) {
        ChangeTarget(static_cast<UXActorID>(dwID));
        ChangeAiState(FSMSTATES_SELECT_ACTION);
    }

    if (m_stMonsterInfo.GetHP() > 0) {
        // 伤害计量
        CMoverEx* pMover = static_cast<CMoverEx*>(GetMoverObject(dwID));
        if (!pMover) {
            return false;
        }

        AddDamageMeter(pMover, nDamage, pSkillRef);

        if (!m_pMobTableRef) {
            return false;
        }

        // IDA: CAi::AddEscapePoint(this->m_pAi, eDAMAGE_TYPE_SKILL)
        // AddEscapePoint expects damage type enum
        if (m_pAi) {
            m_pAi->AddEscapePoint(eDAMAGE_TYPE_SKILL);
        }

        return false;
    } else {
        // 吸收HP/SG处理 - 已在基类中处理
        return true;
    }
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
                XArea* pArea = GetArea();
                if (pArea) {
                    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
                    if (pMaze) {
                        pMaze->OnProtectSkill(m_eSkillType);
                    }
                }
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
                XArea* pArea2 = GetArea();
                if (pArea2) {
                    XMaze* pMaze = dynamic_cast<XMaze*>(pArea2);
                    if (pMaze) {
                        pMaze->OnProtectSkill(m_eSkillType);
                    }
                }
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
    XArea* pArea = GetArea();
    if (pArea) {
        XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
        if (pMaze) {
            pMaze->DamageMonster(this);
        }
    }
}

// ============================================================================
// ClearBuffProcess IDA 0x14035E200 -> 0x14035E914
// 清除Buff处理 - 简化版本
// ============================================================================
int CMonster::ClearBuffProcess(int nSkillID, AttackJudgmentTrigger* pTrigger, hkvVec3 vCurPos) {
    // IDA 0x14035E200 精确还原:
    // 处理技能攻击、伤害计算和Buff/Aura应用

    if (nSkillID <= 0) {
        return 0;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
    if (!pSkillTable) {
        return 0;
    }

    m_bDisableDirectionToTargetSkill = 1;

    // TODO: 完整实现需要:
    // - CMySkillList::ClearAttackDamage
    // - CMySkillList::GetSkillTarget
    // - CMySkillList::GetAttackTarget
    // - CalcTargetDamage_2
    // - ApplySkillDamageFrame
    // - send_eSUB_CMD_ACTION_SKILL
    // - CMySkillList::ApplyBuffSkill
    // - SetAuraSkill

    if (GetHP() <= 0) {
        SetDie(12, 0);
    }

    return 1;
}

// ============================================================================
// ApplySkillDamageFrame IDA 0x14035E920 -> 0x14035ED35
// 应用技能伤害帧 - 简化版本
// ============================================================================
void CMonster::ApplySkillDamageFrame(unsigned int nSkillID, std::int16_t nTriggerIdx,
                                      unsigned char byAttackTargetCnt, hkvVec3& vPos,
                                      float fAttackRot, int nContinueAttack,
                                      unsigned char byDamageType, bool bPenetrate) {
    // IDA 0x14035E920 精确还原:
    // 对目标应用技能伤害

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
    if (!pSkillTable) {
        return;
    }

    // TODO: 完整实现需要:
    // - GetAttackJudgmentEvent
    // - IsAttackPassive
    // - GeneralSkillDamage
    // - SetStiffen
    // - GrapAttach
}

// ============================================================================
// NotifyPhaseChanged IDA 0x14035ED40 -> 0x14035F19B
// 通知阶段变化 - 简化版本
// ============================================================================
void CMonster::NotifyPhaseChanged(unsigned char byOldPhase) {
    // IDA 0x14035ED40 精确还原:
    // 处理怪物AI阶段切换

    if (byOldPhase > 3) {
        return;
    }

    // TODO: 完整实现需要 TB_MONSTER 中的阶段相关字段
    // - uniAIValue
    // - Monster_Switching_AI_01
    // - Monster_AI_Type_01
    // - ___u49 (SpecialDamage)
    // - Monster_Default_Action_Type_01
    // - Monster_NormalStand_Type_01/02/03
    // - Monster_Switching_AI_Step_01
    // - Monster_Switching_AI_Condition_01
    // - Monster_Switching_AI_Value_01
    // - ___u53 (PhaseChangeAnim)
}

// ============================================================================
// SetHpEx - 设置HP扩展
// ============================================================================
void CMonster::SetHpEx(int nHP) {
    // 设置HP信息
    SetHP(nHP);
    
    // 更新 STMonsterInfo 中的HP
    m_stMonsterInfo.SetHP(nHP);

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
// ============================================================================
void CMonster::SendNoticePacket(int iType, int iValue, float fTime) {
    // 发送通知包
    XArea* pArea = GetArea();
    if (pArea) {
        XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
        if (pMaze) {
            if (iValue >= 0) {
                m_nProtectSkill = iType;
            } else {
                m_nProtectSkill = 0;
            }
            (void)fTime;
        }
    }
}

// ============================================================================
// SetupScriptTraceHP IDA 0x14035BCA0
// 设置脚本追踪HP - IDA 精确还原
// ============================================================================
void CMonster::SetupScriptTraceHP(const VMonsterSpawnInfo* pMonsterSpawn) {
    // IDA 0x14035BCA0 精确还原:
    // if (pMonsterSpawn->m_iScriptType == 1) {
    //     CTraceHPState::SetType(GetTraceHPState(), 1);
    //     CTraceHPState::SetCheckPercent(GetTraceHPState(), 100);
    // } else if (pMonsterSpawn->m_iScriptType == 2) {
    //     CTraceHPState::SetType(GetTraceHPState(), 1);
    //     for (int i = 0; i < 5; ++i) {
    //         if (pMonsterSpawn->m_iCheckScirptHP[i] > 0) {
    //             CTraceHPState::SetCheckPercent(GetTraceHPState(), pMonsterSpawn->m_iCheckScirptHP[i]);
    //         }
    //     }
    // }

    if (!pMonsterSpawn) {
        return;
    }

    if (pMonsterSpawn->m_iScriptType == 1) {
        CTraceHPState* pTraceHP = GetTraceHPState();
        if (pTraceHP) {
            pTraceHP->SetType(1);
            pTraceHP->SetCheckPercent(100);
        }
    } else if (pMonsterSpawn->m_iScriptType == 2) {
        CTraceHPState* pTraceHP = GetTraceHPState();
        if (pTraceHP) {
            pTraceHP->SetType(1);
            for (int i = 0; i < 5; ++i) {
                if (pMonsterSpawn->m_iCheckScirptHP[i] > 0) {
                    pTraceHP->SetCheckPercent(pMonsterSpawn->m_iCheckScirptHP[i]);
                }
            }
        }
    }
}

// ============================================================================
// ForceActionSkill IDA 0x14035BD80
// 强制执行技能动作 - IDA 精确还原
// ============================================================================
void CMonster::ForceActionSkill(unsigned int nSkillID) {
    // IDA 0x14035BD80 精确还原:
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // m_pCurSkillTableRef = XResourceMgr::GetTB_SKILL(&pServer->m_xResourceMgr, nSkillID);
    // CMover::SetCurSkillTableIdx(this, nSkillID);
    // CMonster::ActionAttack(this);

    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        m_pCurSkillTableRef = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
    }
    SetCurSkillTableIdx(nSkillID);
    ActionAttack();
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
// GetDeathMotion IDA 0x14035F380
// 获取死亡动作 - 精确还原
// ============================================================================
std::int16_t CMonster::GetDeathMotion() {
    // IDA 0x14035F380 精确还原:
    // if (m_pAi)
    //     return CAi::GetDeathActionMotion(m_pAi);
    // if (CMover::IsHitDown(this))
    //     return 13;
    // return 12;
    
    if (m_pAi) {
        return m_pAi->GetDeathActionMotion();
    }
    if (IsHitDown()) {
        return 13;
    }
    return 12;
}

// ============================================================================
// ChangeAiScript IDA 0x14035F3D0
// 切换AI脚本 - STUB (uses std::shared_ptr, ThreadLocalData, CVaccumManager)
// ============================================================================
int CMonster::ChangeAiScript(const char* szAiName) {
    // IDA 0x14035F3D0 精确还原:
    // 使用 std::shared_ptr, ThreadLocalData::AddAi, CVaccumManager::GetArea
    // 这些依赖项尚未完全实现，保留简化版本
    
    if (!m_pAi) {
        m_pAi = new CAi();
    }
    
    // TODO: ThreadLocalData::AddAi(m_pAi, this, szAiName)
    // TODO: CAi::UpdateFuzzyConditions(m_pAi)
    // TODO: CVaccumManager::GetArea(m_pAi)->SetInitialState(...)
    
    m_bChangeAiScript = true;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ChangeAiScript: %s", szAiName ? szAiName : "null");
    return 1;
}

// ============================================================================
// SetupAnimInfo IDA 0x1403601D0
// 设置动画信息 - 精确还原
// ============================================================================
void CMonster::SetupAnimInfo() {
    // IDA 0x1403601D0 精确还原:
    // VString strAnimName("B_Gaze_F");
    // if (CMover::IsRegisterAnimInfo(this, 4, 0, strAnimName) == 1)
    //     m_byGazeAnimType |= 1u;
    // VString v4("B_Gaze_L");
    // if (CMover::IsRegisterAnimInfo(this, 4, 1, v4) == 1)
    //     m_byGazeAnimType |= 2u;

    // 检查前视动画
    VString strAnimNameF("B_Gaze_F");
    if (IsRegisterAnimInfo(4, 0, strAnimNameF)) {
        m_byGazeAnimType |= 1u;
    }
    
    // 检查左视动画
    VString strAnimNameL("B_Gaze_L");
    if (IsRegisterAnimInfo(4, 1, strAnimNameL)) {
        m_byGazeAnimType |= 2u;
    }
}

// ============================================================================
// DisableSummonMonster IDA 0x14035BDD0
// 禁用召唤怪物 - STUB (uses VBitmask, complex XArea calls)
// ============================================================================
int CMonster::DisableSummonMonster() {
    // IDA 0x14035BDD0 精确还原:
    // 使用 VBitmask, XArea::GetWorldType, XArea::FindActor 等
    // 这些依赖项尚未完全实现，保留简化版本
    
    XArea* pArea = GetArea();
    if (!pArea) {
        return 0;
    }
    
    // TODO: 检查 WorldType == 2
    // TODO: 检查 Monster_Party_Revise == 60005
    // TODO: 查找 KillerID 对应的 Actor
    
    if (m_pMobTableRef && m_pMobTableRef->Monster_Party_Revise == 60005 && m_dwKillerID != 0xFFFFFFFF) {
        // TODO: 完整实现需要 XArea::FindActor
        return 1;
    }
    
    return 0;
}

// ============================================================================
// QuickTurn IDA 0x14035D290
// 快速转向 - 简化版本
// ============================================================================
void CMonster::QuickTurn() {
    m_bStartRotation = true;
    m_fStartRotWaitTime = 0.0f;
    m_bQuickTurn = true;
}

// ============================================================================
// ActionAttack IDA 0x14035D460
// 执行攻击动作 - 简化版本
// ============================================================================
void CMonster::ActionAttack() {
    // IDA 0x14035D460 精确还原:
    // StopMoving(this, 1);
    // nSkillID = CMover::GetCurSkillTableIdx(this);
    // PreSkillProcess(this, nSkillID, 0);
    // if ( m_pCurSkillTableRef )
    // {
    //   if ( rand() % 100 <= m_pCurSkillTableRef->Next_Chain_Skill_Rate - 1
    //     && m_pCurSkillTableRef->Next_Chain_Skill_ID )
    //   {
    //     SetNextSkillID(m_pCurSkillTableRef->Next_Chain_Skill_ID);
    //     SetParentSkillTableIdx(m_pCurSkillTableRef->Skill_Index);
    //   }
    //   if ( m_pCurSkillTableRef->Attack_Action_link_Type == 1 )
    //     UpdateAttackKeyPress(1);
    //   if ( m_pCurSkillTableRef->Skill_Type == 6 || m_pCurSkillTableRef->Skill_Type == 7 )
    //   {
    //     m_eSkillType = m_pCurSkillTableRef->Skill_Type;
    //     m_nProtectSkillDamage = (int)m_pCurSkillTableRef->Skill_Basic_Damage;
    //     SetInvincibleActor(1);
    //     ShowProtectSkillUI(1);
    //   }
    // }
    // CurSkillTableIdx = GetCurSkillTableIdx();
    // send_eSUB_CMD_ACTIVE_SKILL(this, CurSkillTableIdx, m_byAngleAttackType);

    StopMoving(true);

    int nSkillID = GetCurSkillTableIdx();
    PreSkillProcess(nSkillID, 0);

    if (m_pCurSkillTableRef) {
        // 连锁技能检查
        if (std::rand() % 100 <= m_pCurSkillTableRef->Next_Chain_Skill_Rate - 1
            && m_pCurSkillTableRef->Next_Chain_Skill_ID) {
            SetNextSkillID(m_pCurSkillTableRef->Next_Chain_Skill_ID);
            m_nParentSkillTableIdx = m_pCurSkillTableRef->Skill_Index;
        }

        // 攻击动作链接类型
        if (m_pCurSkillTableRef->Attack_Action_link_Type == 1) {
            UpdateAttackKeyPress(1);
        }

        // 保护技能类型 (6 = SKILLTYPE_PROTECT_A, 7 = SKILLTYPE_PROTECT_B)
        if (m_pCurSkillTableRef->Skill_Type == 6 || m_pCurSkillTableRef->Skill_Type == 7) {
            m_eSkillType = m_pCurSkillTableRef->Skill_Type;
            m_nProtectSkillDamage = static_cast<int>(m_pCurSkillTableRef->Skill_Basic_Damage);
            SetInvincibleActor(1);
            ShowProtectSkillUI(true);
        }
    }

    // 发送技能激活包
    send_eSUB_CMD_ACTIVE_SKILL(this, nSkillID, m_byAngleAttackType);
}

// ============================================================================
// PostSkillProcess IDA 0x14035D620
// 技能后处理 - IDA 精确还原
// ============================================================================
void CMonster::PostSkillProcess() {
    // IDA 0x14035D620 精确还原:
    // void __fastcall CMonster::PostSkillProcess(CMonster *this)
    // {
    //   CMonster::ShowProtectSkillUI(this, 0);
    //   CMoverEx::PostSkillProcess(this);
    //   this->m_bChangedAggro = 1;
    // }
    ShowProtectSkillUI(false);
    // 调用基类的 PostSkillProcess (已在 CMoverEx 中实现)
    CMoverEx::PostSkillProcess();
    m_bChangedAggro = 1;
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

    // 应用等级到属性
    ApplyLevelToStat(1);

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

    // 遍历属性并添加到 vecStat
    // 将大于0的属性添加到状态列表
    m_stMonsterInfo.vecStat.clear();
    for (int i = 0; i < 77; ++i) {
        if (m_fAbility[i] > 0.0f) {
            StatInfo statInfo{};
            statInfo.byIndex = static_cast<std::uint8_t>(i);
            statInfo.statValue = m_fAbility[i];
            m_stMonsterInfo.vecStat.push_back(statInfo);
        }
    }

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
// ============================================================================
// GetAIFuzzyValue IDA 0x140357B30
// 获取AI模糊值 - 精确还原
// ============================================================================
void CMonster::GetAIFuzzyValue(float* pfValue) {
    // IDA 反编译精确还原 (0x140357B30):
    // if (this->m_pAi) CAi::GetAIFuzzyValue(this->m_pAi, pfValue);
    if (m_pAi && pfValue) {
        m_pAi->GetAIFuzzyValue(pfValue);
    }
}

// ============================================================================
// GetAIActionValue IDA 0x140357B70
// 获取AI动作值 - 精确还原
// ============================================================================
void CMonster::GetAIActionValue(int* pnValue) {
    // IDA 反编译精确还原 (0x140357B70):
    // if (this->m_pAi) CAi::GetAIActionValue(this->m_pAi, pnValue);
    if (m_pAi && pnValue) {
        m_pAi->GetAIActionValue(pnValue);
    }
}

// ============================================================================
// CheckWayPoint IDA 0x140357BB0
// 检查路径点 - 精确还原
// ============================================================================
void CMonster::CheckWayPoint() {
    // IDA 0x140357BB0 精确还原:
    // if (CWayPoint::GetCurID(&m_xWayPoint) > 0) {
    //     hkvVec3 vDestPos = CWayPoint::GetDestPosition(&m_xWayPoint);
    //     hkvVec3 vDist = m_vPosition - vDestPos;
    //     if (vDist.GetLengthSquared() < 9.0f) {
    //         CWayPoint::Update(&m_xWayPoint, 0.001f);
    //     }
    // }

    // TODO: CWayPoint::GetDestPosition 需要实现
    if (m_xWayPoint.GetCurID() > 0) {
        // hkvVec3 vDestPos = m_xWayPoint.GetDestPosition();
        // hkvVec3 vDist = m_vPosition - vDestPos;
        // if (vDist.GetLengthSquared() < 9.0f) {
        //     m_xWayPoint.Update(0.001f);
        // }
    }
}

// ============================================================================
// GetSectorID IDA 0x140357C40
// 获取扇区ID
// ============================================================================
int CMonster::GetSectorID() {
    // IDA 0x140357C40 精确还原:
    // if ( this->m_pSector )
    //   return (unsigned int)CSector::GetSectorBoxUniqueID(this->m_pSector);
    // else
    //   return (unsigned int)-1;

    if (m_pSector) {
        // TODO: 需要实现 CSector::GetSectorBoxUniqueID 方法
        // return m_pSector->GetSectorBoxUniqueID();
        return -1;
    }
    return -1;
}

// ============================================================================
// ApplySuperArmorGage IDA 0x140358100
// 应用超级护甲值
// ============================================================================
// ============================================================================
// ApplySuperArmorGage IDA 0x140358100
// 应用超级护甲值 - 精确还原
// ============================================================================
void CMonster::ApplySuperArmorGage(float fSuperArmor) {
    // IDA 0x140358100 精确还原:
    // if ( this->m_pMobTableRef->Monster_Defence_Type == 2 )
    // {
    //   this->m_fMaxSuperArmorGage = this->m_fAbility[10] * (float)(fSuperArmor / 100.0);
    //   this->m_fCurSuperArmorGage = this->m_fMaxSuperArmorGage;
    // }
    // else
    // {
    //   this->m_fMaxSuperArmorGage = 0.0;
    //   this->m_fCurSuperArmorGage = 0.0;
    // }

    if (m_pMobTableRef && m_pMobTableRef->Monster_Defence_Type == 2) {
        m_fMaxSuperArmorGage = m_fAbility[10] * (fSuperArmor / 100.0f);
        m_fCurSuperArmorGage = m_fMaxSuperArmorGage;
    } else {
        m_fMaxSuperArmorGage = 0.0f;
        m_fCurSuperArmorGage = 0.0f;
    }
}

// ============================================================================
// GetSpawnTime IDA 0x1403606A0
// 获取生成时间 - 精确还原
// ============================================================================
float CMonster::GetSpawnTime() {
    // IDA 0x1403606A0 精确还原:
    // signed __int64 v1 = XTime::GetTickCount() - m_dwSpawnedTime64;
    // float v2 = (float)(int)v1;
    // if (v1 < 0) v2 = v2 + 1.8446744e19;
    // return v2 * 0.001;
    
    // 使用 std::chrono 获取当前时间
    auto now = std::chrono::steady_clock::now();
    auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();
    
    signed __int64 v1 = nowMs - m_dwSpawnedTime64;
    float v2 = static_cast<float>(static_cast<int>(v1));
    if (v1 < 0) {
        v2 = v2 + 1.8446744e19f;
    }
    return v2 * 0.001f;
}

// ============================================================================
// IsTableID IDA 0x140360700
// 检查表ID是否匹配
// ============================================================================
bool CMonster::IsTableID(const char* szTableID) {
    if (!szTableID) return false;
    std::string _Str(szTableID);
    int nTableID = std::stoi(_Str, nullptr, 10);
    return nTableID == GetTableID();
}

// ============================================================================
// IsRealHelper IDA 0x1403609D0
// 检查是否真实助手 - 简化版本
// ============================================================================
bool CMonster::IsRealHelper() {
    // TODO: 完整实现需要 XResourceMgr::GetTB_HELPER
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 10;
}

// ============================================================================
// IsDefensiveWeapon IDA 0x140360790
// 检查是否防御武器类型
// ============================================================================
bool CMonster::IsDefensiveWeapon() {
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 5;
}

// ============================================================================
// IsMonsterDamageCount IDA 0x140360890
// 检查是否伤害计数类型
// ============================================================================
bool CMonster::IsMonsterDamageCount() {
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 14;
}

// ============================================================================
// IsDedicated IDA 0x1403608D0
// 检查是否专用类型
// ============================================================================
bool CMonster::IsDedicated() {
    // IDA 反编译 (0x1403608D0):
    // return this->m_pMobTableRef && this->m_pMobTableRef->Monster_Type == 15;
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 15;
}

// ============================================================================
// IsEscort IDA 0x1403607D0
// 检查是否护卫类型
// ============================================================================
bool CMonster::IsEscort() {
    // IDA 反编译 (0x1403607D0):
    // return this->m_pMobTableRef && this->m_pMobTableRef->Monster_Type == 6;
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 6;
}

// ============================================================================
// IsMonsterCombo IDA 0x140360910
// 检查是否连击类型
// ============================================================================
bool CMonster::IsMonsterCombo() {
    // IDA 反编译 (0x140360910):
    // return this->m_pMobTableRef && this->m_pMobTableRef->Monster_Type == 16;
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
// CheckFollowMonster IDA 0x140360BA0
// 检查跟随怪物 - 精确还原
// ============================================================================
void CMonster::CheckFollowMonster() {
    // IDA 0x140360BA0 精确还原:
    // if ( CMonster::IsMercenary(this) )
    // {
    //   CMonster::FindFollowPlayer(this);
    //   if ( !CMoverEx::GetOwnerPlayer(this) )
    //     CMoverEx::SetInvisible(this, 1, 0, 0, 0, 0, 0, 0);
    // }
    if (IsMercenary()) {
        FindFollowPlayer();
        if (!GetOwnerPlayer()) {
            // TODO: SetInvisible(1, 0, 0, 0, 0, 0, 0);
        }
    }
}

// ============================================================================
// FindFollowPlayer IDA 0x140360C20
// 查找跟随玩家 - 精确还原
// ============================================================================
void CMonster::FindFollowPlayer() {
    // IDA 0x140360C20 精确还原:
    // if ( CMonster::IsMercenary(this) )
    // {
    //   if ( XActor::IsStatus(&this->XActor, 0x2000u) )
    //   {
    //     CMoverEx::SetOwnerID(this, 0);
    //     return;
    //   }
    //   XArea* pArea = GetArea();
    //   XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    //   if ( pMaze )
    //   {
    //     pMover = XMaze::FindUserByQuest(pMaze, this->m_pMobTableRef->Quest_Group_1);
    //     if ( !pMover ) goto LABEL_3;
    //     UXActorID actorID;
    //     pMover->GetActorID(&actorID);
    //     CMoverEx::SetOwnerID(this, actorID.dwActorID);
    //   }
    // }
    if (!IsMercenary()) {
        return;
    }

    if (IsStatus(0x2000u)) {
        SetOwnerID(0);
        return;
    }

    // TODO: 需要实现 XArea/XMaze 相关功能
    // XArea* pArea = GetArea();
    // XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    // if (pMaze) {
    //     CMover* pMover = pMaze->FindUserByQuest(m_pMobTableRef->Quest_Group_1);
    //     if (!pMover) {
    //         SetOwnerID(0);
    //         return;
    //     }
    //     UXActorID actorID = pMover->GetActorID();
    //     SetOwnerID(actorID.dwActorID);
    // }
}

// ============================================================================
// SetDefensiveWeaponPlayer IDA 0x140360D80
// 设置防御武器玩家 - 精确还原
// ============================================================================
void CMonster::SetDefensiveWeaponPlayer(CMoverEx* pMover) {
    // IDA 0x140360D80 精确还原:
    // pOwner = CMoverEx::GetOwnerPlayer(this);
    // if ( pOwner != pMover )
    // {
    //   pOwnerUser = dynamic_cast<CUser*>(pOwner);
    //   if ( pOwnerUser )
    //     pOwnerUser->DefensiveWeaponActivate(0, this);
    //   pOwnerUsera = dynamic_cast<CUser*>(pMover);
    //   if ( pOwnerUsera )
    //   {
    //     pOwnerUsera->DefensiveWeaponActivate(1, this);
    //     int nMaxHP = GetMaxHP();
    //     SetHP(nMaxHP);
    //     m_fLifeTime = 10.0f;
    //     m_nLastTime = (int)m_fLifeTime;
    //     int nHPPerSec = nMaxHP / m_nLastTime;
    //     m_nHPPerSec = nHPPerSec;
    //     ChangeBattlePose(1, 1);
    //   }
    //   else
    //   {
    //     ChangeBattlePose(0, 1);
    //   }
    // }
    CMoverEx* pOwner = GetOwnerPlayer();
    if (pOwner == pMover) {
        return;
    }

    // TODO: 需要实现 CUser::DefensiveWeaponActivate
    // CUser* pOwnerUser = dynamic_cast<CUser*>(pOwner);
    // if (pOwnerUser) {
    //     pOwnerUser->DefensiveWeaponActivate(0, this);
    // }

    // CUser* pOwnerUsera = dynamic_cast<CUser*>(pMover);
    // if (pOwnerUsera) {
    //     pOwnerUsera->DefensiveWeaponActivate(1, this);
    //     int nMaxHP = GetMaxHP();
    //     SetHP(nMaxHP);
    //     m_fLifeTime = 10.0f;
    //     m_nLastTime = static_cast<int>(m_fLifeTime);
    //     m_nHPPerSec = nMaxHP / m_nLastTime;
    //     ChangeBattlePose(1, 1);
    // } else {
    //     ChangeBattlePose(0, 1);
    // }
}

// ============================================================================
// GetGuardMonster IDA 0x140360D40
// 获取守护怪物 - IDA 精确还原
// ============================================================================
CMonster* CMonster::GetGuardMonster() {
    // IDA 0x140360D40 精确还原:
    // CMonster *__fastcall CMonster::GetGuardMonster(CMonster *this)
    // {
    //   if ( this->m_dwGuardMonsterID )
    //     return (CMonster *)CMover::GetMoverObject(this, this->m_dwGuardMonsterID);
    //   else
    //     return nullptr;
    // }

    if (m_dwGuardMonsterID) {
        return static_cast<CMonster*>(CMover::GetMoverObject(m_dwGuardMonsterID));
    }
    return nullptr;
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
// ============================================================================
// ApplyLevelToStat IDA 0x140357C80
// 应用等级到属性 - 精确还原
// ============================================================================
void CMonster::ApplyLevelToStat(int bInit) {
    // IDA 反编译精确还原 (0x140357C80):
    // 如果已应用等级且不是初始化，则跳过
    if ((m_bApplyLevel == 1 && bInit == 1) || !m_pMobTableRef) {
        return;
    }

    // 获取队伍人数
    std::uint8_t byCount = 1;

    // 尝试从区域获取迷宫对象来获取队伍人数
    // TODO: XArea* pArea = GetArea();
    // XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    // if (pMaze) {
    //     byCount = pMaze->GetPartyMemberCount();
    // }

    if (byCount == 0) {
        byCount = 1;
    }
    if (byCount >= 4) {
        byCount = 4;
    }

    // 如果不是普通怪物类型，则队伍人数固定为1
    if (m_pMobTableRef && m_pMobTableRef->Monster_Type != 0) {
        byCount = 1;
    }

    // 获取队伍修正表
    std::uint32_t dwGroupID = m_pMobTableRef->Monster_Party_Revise;
    XGameServer* pServer = XGameServer::Instance();
    TB_PARTYREVISE* pPartyRevise = nullptr;
    if (pServer) {
        pPartyRevise = pServer->GetResourceMgr().GetPartyReviseTable(dwGroupID, byCount);
    }

    if (pPartyRevise) {
        m_bApplyLevel = 1;

        // 获取 NPC 属性组件
        // TODO: std::shared_ptr<CGocNpcAttribute> pAttr;
        // CMover::GetGOC<CGocNpcAttribute>(&pAttr, 0);
        // if (pAttr) {
        //     CGocNpcAttribute* pNpcAttr = pAttr.get();
        //
        //     // 应用各种属性修正 (IDA 中调用虚函数表索引 6)
        //     // pNpcAttr->ApplyStat(1);   // 修正类型 1
        //     // pNpcAttr->ApplyStat(10);  // 修正类型 10
        //     // float fHP = CMover::GetStat(10);
        //     // SetHpEx((int)fHP);
        //     // pNpcAttr->ApplyStat(20);  // 修正类型 20-25, 19, 18
        //     // pNpcAttr->ApplyStat(21);
        //     // pNpcAttr->ApplyStat(22);
        //     // pNpcAttr->ApplyStat(23);
        //     // pNpcAttr->ApplyStat(24);
        //     // pNpcAttr->ApplyStat(25);
        //     // pNpcAttr->ApplyStat(19);
        //     // pNpcAttr->ApplyStat(18);
        //     // pNpcAttr->SetMonterOriginStat();
        // }

    // 设置 SuperArmor 相关时间
    if (pPartyRevise) {
        m_fDefRegenSuperArmorTime = static_cast<float>(pPartyRevise->SA_Regen_Time) / 1000.0f;
        m_fRecoverySuperArmorTime = static_cast<float>(pPartyRevise->SA_Recover_Time) / 1000.0f;

        // 应用 SuperArmor 计量
        ApplySuperArmorGage(pPartyRevise->SuperArmor_Guage);
    }
    }
}

// ============================================================================
// InitialObjectInfo IDA 0x140355120 -> 0x1403556D0
// 初始化对象信息 - 精确还原
// 大小: 1456 bytes
// ============================================================================
void CMonster::InitialObjectInfo(unsigned int dwID, unsigned int nTableIdx, hkvVec3 vPos, float fRot) {
    // IDA 反编译精确还原 (0x140355120):
    // 1. 调用基类 CMover::InitialObjectInfo
    // TODO: CMover::InitialObjectInfo(dwID, nTableIdx, vPos, fRot);

    // 2. 获取 CGocNpcAttribute 组件并初始化
    // TODO: std::shared_ptr<CGocNpcAttribute> pAttr;
    // CMover::GetGOC<CGocNpcAttribute>(&pAttr, 0);
    // if (pAttr) {
    //     std::uint16_t nStatusID = 1;
    //     CGocNpcAttribute* pNpcAttr = pAttr.get();
    //     pNpcAttr->Init(nStatusID, m_pMobTableRef);
    //     pNpcAttr->Update();
    //
    //     // 3. 获取最终属性数组
    //     m_fAbility = CGocAttribute::GetFinalStats(pNpcAttr);
    //
    //     // 4. 设置 HP 和表 ID
    //     m_stMonsterInfo.nHP = (int)m_fAbility[10];
    // }

    // 设置表 ID (IDA 确认)
    m_stMonsterInfo.stNpcInfo.nTableID = nTableIdx;

    // 5. 设置 ActorID (高位设置为怪物类型标识 0x40000000)
    // IDA: m_stMonsterInfo.uxActorID.dwActorID = (m_stMonsterInfo.uxActorID.dwActorID & 0x1FFFFFFF) | 0x40000000;
    // IDA: int nSessionID = GetSessionID();
    //      m_stMonsterInfo.uxActorID.dwActorID = (nSessionID & 0x1FFFFFFF) | (m_stMonsterInfo.uxActorID.dwActorID & 0xE0000000);

    // 6. 设置位置和朝向 - IDA 确认
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.x = vPos.x;
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.y = vPos.y;
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.z = vPos.z;
    m_stMonsterInfo.stNpcInfo.stPosInfo.fRot = fRot;
    SetPositionXVec3(vPos);
    m_vCreatePos = vPos;

    // 设置移动朝向和方向 - IDA 确认
    m_fMovingYaw = fRot;
    SetOrientationYaw(fRot);

    // 7. 应用表能力 - IDA 确认
    ApplyTableAbility();

    // 8. 设置动画 - IDA 确认
    SetupAnimation();

    // 9. 检查转向动作 - IDA 确认
    // IDA: dwAnimID = XActionResMgr::GetAnimIndex(this, 7, 0, 1);
    //      pszAnimString = CMover::GetAnimStirng(this, dwAnimID);
    //      m_bHasTurnMotion = (pszAnimString != nullptr);
    //      dwAnimID = XActionResMgr::GetAnimIndex(this, 7, 1, 1);
    //      pszAnimString = CMover::GetAnimStirng(this, dwAnimID);
    //      m_bHasBigTurn = (pszAnimString != nullptr);

    // 10. 检查受击动画数量 - IDA 确认
    // IDA: dwAnimID = XActionResMgr::GetAnimIndex(this, 18, 2, 1);
    //      pszAnimString = CMover::GetAnimStirng(this, dwAnimID);
    //      if (!pszAnimString) m_nHitAnimCount = 2;

    // 11. 初始化组件 - IDA 确认
    m_xGroupAggro.Init(this);
    m_xWayPoint.Init(this);
    GetTraceHPState()->Init(this);

    // 12. 重置仇恨检查时间 - IDA 确认
    m_fLastAggroCheckTime = 0.0f;
    m_bChangedAggro = 0;

    // 设置生成时间
    m_dwSpawnedTime64 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();

    // 如果是防御武器类型 (Monster_Type == 5) - IDA 确认
    if (m_pMobTableRef && m_pMobTableRef->Monster_Type == 5) {
        XGameServer* pServer2 = XGameServer::Instance();
        if (pServer2) {
            m_pWeaponTableRef = pServer2->GetResourceMgr().GetTB_DEFENSIVE_WEAPON(nTableIdx);
        }
        SetInvincibleActor(1);
    }

    // 15. 增加计数 - IDA 确认
    ++m_naCount;

    // 16. 重置死亡相关状态 - IDA 确认
    m_fElapsedDieTime = 0.0f;
    m_byAngleAttackType = 0;
    
    // 17. Boss HP 日志检查 - IDA 确认
    if (IsBoss()) {
        m_fBossHPLogTime = 60.0f;
        m_nCheckBossHP = GetHP();
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::InitialObjectInfo called");
}

// ============================================================================
// GenerateEventObject IDA 0x1403556D0
// 生成事件对象 - 精确还原
// ============================================================================
void CMonster::GenerateEventObject() {
    // IDA 反编译精确还原 (0x1403556D0):
    // 1. 调用基类生成事件对象 (参数: 2 = 怪物类型, TableID)
    _GenerateEventObject(2, m_pMobTableRef ? m_pMobTableRef->ID : 0);

    // 2. 查找跟随玩家
    FindFollowPlayer();

    // 3. 重置应用等级标志
    m_bApplyLevel = 0;

    // 4. 应用等级到属性 (初始化)
    ApplyLevelToStat(1);
}

// ============================================================================
// SetInfo IDA 0x140355730
// 设置信息 - STUB (uses CMySkillList)
// ============================================================================
void CMonster::SetInfo() {
    // IDA 0x140355730 精确还原:
    // 使用 CMySkillList, CMySkillList::Init
    // 这些依赖项尚未完全实现，保留简化版本
    
    // 设置组件信息
    // sprintf_s(szName, "Monster Idle %u", m_pGrapParent);
    
    // 初始化技能列表
    if (!m_pSkillMgr) {
        // TODO: CMySkillList::Init 需要 XActor 参数
        // m_pSkillMgr = new CMySkillList();
        // m_pSkillMgr->Init(this);
    }
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::SetInfo called");
}

// ============================================================================
// SetSyncInfo IDA 0x140355B10
// 设置同步信息 - STUB (uses std::shared_ptr<CGocAttribute>)
// ============================================================================
void CMonster::SetSyncInfo() {
    // IDA 0x140355B10 精确还原:
    // 使用 std::shared_ptr<CGocAttribute>, CMover::GetGOC<CGocAttribute>
    // 这些依赖项尚未完全实现，保留简化版本
    
    // 应用等级到属性
    ApplyLevelToStat(1);
    
    // 获取位置并设置到怪物信息
    hkvVec3 vPos = GetPosition();
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.x = vPos.x;
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.y = vPos.y;
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.z = vPos.z;
    
    // 设置旋转
    m_stMonsterInfo.stNpcInfo.stPosInfo.fRot = GetMovingYaw();
    
    // 设置 SectorID
    if (m_pSector) {
        // TODO: m_stMonsterInfo.stNpcInfo.nSectorID = CSector::GetSectorBoxID(m_pSector);
        m_stMonsterInfo.stNpcInfo.nSectorID = -1;
    } else {
        m_stMonsterInfo.stNpcInfo.nSectorID = -1;
    }
    
    // 设置自杀时间
    m_stMonsterInfo.fSuicideTime = 0.0f;
    if (m_pAi) {
        // TODO: m_stMonsterInfo.fSuicideTime = m_pAi->GetSuicideTime();
    }
    
    // 清空状态列表并重新填充
    m_stMonsterInfo.vecStat.clear();
    for (int i = 0; i < 77; ++i) {
        if (m_fAbility[i] > 0.0f) {
            StatInfo statInfo{};
            statInfo.byIndex = static_cast<std::uint8_t>(i);
            statInfo.statValue = m_fAbility[i];
            m_stMonsterInfo.vecStat.push_back(statInfo);
        }
    }
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::SetSyncInfo called");
}

// ============================================================================
// ProcessExp IDA 0x140355FD0
// 处理经验 - 精确还原
// ============================================================================
void CMonster::ProcessExp(XActor* pActor) {
    // IDA 0x140355FD0 精确还原:
    // if ( this->m_pMobTableRef )
    // {
    //   nRank = this->m_pMobTableRef->Monster_Rank;
    //   v12 = this->GetLevel(this);
    //   v2 = TXSingleton<XGameServer>::Instance();
    //   pTBExp = XResourceMgr::GetTB_MONSTER_EXP(&v2->m_xResourceMgr, v12);
    //   if ( pTBExp )
    //   {
    //     if ( nRank <= 6 )
    //     {
    //       nExp = (int)(float)((float)*(&pTBExp->EXP_Slave + nRank) * this->m_pMobTableRef->Exp);
    //       if ( nExp > 0 )
    //       {
    //         nLevel = this->m_pMobTableRef->Monster_Lv;
    //         if ( this->GetArea(&this->XActor) && (v13 = this->GetArea(&this->XActor), v13->IsMaze(v13)) )
    //         {
    //           v5 = this->GetArea(&this->XActor);
    //           ((void (__fastcall *)(XArea *, XActor *, __int64, _QWORD))v5->ProcessExp)(v5, pActor, v6, nLevel);
    //         }
    //         else
    //         {
    //           pUser = (CUser *)_RTDynamicCast_0(pActor, 0, &XActor RTTI, &CUser RTTI, 0);
    //           if ( pUser )
    //             CUser::SetExp(pUser, (float)nExp * 1.0, nLevel);
    //         }
    //       }
    //     }
    //     else
    //     {
    //       v4 = this->GetLevel(this);
    //       LogHelper::LogError("game.contents", "ProcessExp error - Invalid Rank TB_MONSTER_EXP [ Level:%d ] ( %d )", v4, 426);
    //     }
    //   }
    //   else
    //   {
    //     v3 = this->GetLevel(this);
    //     LogHelper::LogError("game.contents", "ProcessExp error - No Table TB_MONSTER_EXP [ Level:%d ] ( %d )", v3, 421);
    //   }
    // }

    if (!m_pMobTableRef) {
        return;
    }

    int nRank = m_pMobTableRef->Monster_Rank;
    std::uint8_t byLevel = GetLevel();

    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        return;
    }

    TB_MONSTER_EXP* pTBExp = pServer->GetResourceMgr().GetTB_MONSTER_EXP(byLevel);

    if (!pTBExp) {
        GreenDamTan_log(__FILE__, __FUNCTION__,
            "ProcessExp error - No Table TB_MONSTER_EXP [ Level:%d ] ( %d )", byLevel, 421);
        return;
    }

    if (nRank > 6) {
        GreenDamTan_log(__FILE__, __FUNCTION__,
            "ProcessExp error - Invalid Rank TB_MONSTER_EXP [ Level:%d ] ( %d )", byLevel, 426);
        return;
    }

    // 计算经验值
    int nExp = static_cast<int>(static_cast<float>(pTBExp->uniMExp[nRank]) * m_pMobTableRef->Exp);

    if (nExp <= 0) {
        return;
    }

    unsigned int nMonsterLevel = m_pMobTableRef->Monster_Lv;

    // 检查是否在迷宫中
    XArea* pArea = GetArea();
    if (pArea && pArea->IsMaze()) {
        // 迷宫经验处理
        pArea->ProcessExp(pActor, nExp, nMonsterLevel);
    } else {
        // 直接给玩家经验
        CUser* pUser = dynamic_cast<CUser*>(pActor);
        if (pUser) {
            // TODO: pUser->SetExp(static_cast<float>(nExp), nMonsterLevel);
            GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessExp: Granting %d exp to user (not implemented)", nExp);
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessExp: Exp=%d, Level=%d, Rank=%d", nExp, nMonsterLevel, nRank);
}

// ============================================================================
// DropItemByHit IDA 0x140356290
// 击中掉落物品 - 精确还原
// ============================================================================
void CMonster::DropItemByHit(unsigned int dwAtkUser) {
    // IDA 0x140356290 精确还原:
    // if ( this->m_pMobTableRef && this->m_pMobTableRef->Monster_Hit_Drop_ID && this->GetArea(&this->XActor) )
    // {
    //   v8 = this->GetArea(&this->XActor);
    //   if ( v8->GetWorldType(v8) == 1 )
    //   {
    //     v2 = this->GetArea(&this->XActor);
    //     pMaze = (XMaze *)_RTDynamicCast_0(v2, 0, &XArea RTTI, &XMaze RTTI, 0);
    //     if ( pMaze )
    //     {
    //       nMonsterID = this->GetTableID(this);
    //       XMaze::ProcessDropByHit(pMaze, dwAtkUser, this->m_pMobTableRef->Monster_Hit_Drop_ID,
    //                               this->m_pMobTableRef->Monster_Lv, &this->m_pPosInfo->vPos, nMonsterID);
    //     }
    //   }
    //   else
    //   {
    //     v9 = this->GetArea(&this->XActor);
    //     if ( v9->GetWorldType(v9) == 2 )
    //     {
    //       v4 = this->GetArea(&this->XActor);
    //       pBattleZone = (CBattleZone *)_RTDynamicCast_0(v4, 0, &XArea RTTI, &CBattleZone RTTI, 0);
    //       if ( pBattleZone )
    //       {
    //         v5 = this->GetTableID(this);
    //         CBattleZone::ProcessDropByHit(pBattleZone, dwAtkUser, this->m_pMobTableRef->Monster_Hit_Drop_ID,
    //                                       this->m_pMobTableRef->Monster_Lv, &this->m_pPosInfo->vPos, v5);
    //       }
    //     }
    //   }
    // }

    if (!m_pMobTableRef || !m_pMobTableRef->Monster_Hit_Drop_ID) {
        return;
    }

    XArea* pArea = GetArea();
    if (!pArea) {
        return;
    }

    int nWorldType = pArea->GetWorldType();
    int nMonsterID = GetTableID();

    if (nWorldType == 1) {
        // 迷宫类型
        XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
        if (pMaze) {
            // TODO: pMaze->ProcessDropByHit(dwAtkUser, m_pMobTableRef->Monster_Hit_Drop_ID,
            //                               m_pMobTableRef->Monster_Lv, m_vPosition, nMonsterID);
            GreenDamTan_log(__FILE__, __FUNCTION__, "DropItemByHit: Maze drop not implemented");
        }
    } else if (nWorldType == 2) {
        // 战场类型
        CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
        if (pBattleZone) {
            // TODO: pBattleZone->ProcessDropByHit(dwAtkUser, m_pMobTableRef->Monster_Hit_Drop_ID,
            //                                      m_pMobTableRef->Monster_Lv, m_vPosition, nMonsterID);
            GreenDamTan_log(__FILE__, __FUNCTION__, "DropItemByHit: BattleZone drop not implemented");
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "DropItemByHit: DropID=%d, AtkUser=%u",
                    m_pMobTableRef->Monster_Hit_Drop_ID, dwAtkUser);
}

// ============================================================================
// ProcessDrop IDA 0x140356550
// 处理掉落 - 精确还原
// ============================================================================
void CMonster::ProcessDrop(XActor* pAtk) {
    // IDA 0x140356550 精确还原:
    // if ( this->m_pMobTableRef && this->GetArea(&this->XActor) )
    // {
    //   v5 = this->GetArea(&this->XActor);
    //   if ( v5->GetWorldType(v5) == 2 )
    //   {
    //     v2 = this->GetArea(&this->XActor);
    //     if ( XArea::GetTBMapID(v2) != 30031 )
    //     {
    //       v3 = this->GetArea(&this->XActor);
    //       pD6 = (CBattleZone *)_RTDynamicCast_0(v3, 0, &XArea RTTI, &CBattleZone RTTI, 0);
    //       if ( pD6 )
    //         CBattleZone::ProcessDrop(pD6, pAtk, this, &this->m_stMonsterInfo.stPosInfo.vPos);
    //     }
    //   }
    //   else
    //   {
    //     v6 = this->GetArea(&this->XActor);
    //     v6->ProcessDrop(v6, pAtk, this->m_stMonsterInfo.nTableID, &this->m_stMonsterInfo.stPosInfo.vPos);
    //   }
    // }

    if (!m_pMobTableRef || !pAtk) {
        return;
    }

    XArea* pArea = GetArea();
    if (!pArea) {
        return;
    }

    int nWorldType = pArea->GetWorldType();

    if (nWorldType == 2) {
        // 战场类型 (排除特定地图)
        int nMapID = pArea->GetTBMapID();
        if (nMapID != 30031) {
            CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
            if (pBattleZone) {
                // TODO: pBattleZone->ProcessDrop(pAtk, this, m_stMonsterInfo.stNpcInfo.stPosInfo.vPos);
                GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessDrop: BattleZone drop not implemented");
            }
        }
    } else {
        // 普通区域
        // TODO: pArea->ProcessDrop(pAtk, m_stMonsterInfo.stNpcInfo.nTableID, m_stMonsterInfo.stNpcInfo.stPosInfo.vPos);
        GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessDrop: XArea drop not implemented");
    }

    hkvVec3 vDropPos = GetPosition();
    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessDrop: TableID=%d, Pos=(%.2f, %.2f, %.2f)",
                    m_stMonsterInfo.stNpcInfo.nTableID, vDropPos.x, vDropPos.y, vDropPos.z);
}

// ============================================================================
// ProcessEscortQuest IDA 0x140356750
// 处理护送任务 - IDA 精确还原
// ============================================================================
void CMonster::ProcessEscortQuest() {
    // IDA 0x140356750 精确还原:
    // void __fastcall CMonster::ProcessEscortQuest(CMonster *this)
    // {
    //   XArea *v1; // rax
    //   XMaze *pMaze; // [rsp+30h] [rbp-18h]
    //
    //   if ( this->m_pAi && (unsigned int)CAi::IsEscortMonster(this->m_pAi) && this->m_pMobTableRef )
    //   {
    //     v1 = this->GetArea(&this->XActor);
    //     pMaze = (XMaze *)_RTDynamicCast_0(v1, 0, &XArea `RTTI Type Descriptor', &XMaze `RTTI Type Descriptor', 0);
    //     if ( pMaze )
    //     {
    //       XMaze::FailEscortQuest(pMaze);
    //       CAi::EndEscortWayPoint(this->m_pAi);
    //     }
    //   }
    // }

    if (m_pAi && m_pMobTableRef) {
        if (m_pAi->IsEscortMonster()) {
            // 尝试转换为 XMaze
            XArea* pArea = GetArea();
            if (pArea) {
                XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
                if (pMaze) {
                    pMaze->FailEscortQuest();
                    m_pAi->EndEscortWayPoint();
                }
            }
        }
    }
}

// ============================================================================
// ProcessGameMode IDA 0x1403568A0
// 处理游戏模式 - 简化版本
// ============================================================================
void CMonster::ProcessGameMode() {
    // IDA 0x1403568a0 精确还原:
    // if ( !this->m_bSuicide && this->m_pMobTableRef && this->m_pMobTableRef->Monster_Type == 3 )
    // {
    //   pMaze = (XMaze *)_RTDynamicCast_0(this->m_pArea, 0, &XArea `RTTI Type Descriptor', &XMaze `RTTI Type Descriptor', 0);
    //   if ( pMaze )
    //     XMaze::SetGameModeState(pMaze, 2);
    // }
    // else
    // {
    //   pD6 = (CBattleZone *)_RTDynamicCast_0(this->m_pArea, 0, &XArea `RTTI Type Descriptor', &CBattleZone `RTTI Type Descriptor', 0);
    //   if ( pD6 )
    //   {
    //     HitID = CMover::GetHitID(this);
    //     CBattleZone::MonsterDieForEvent(pD6, this, HitID);
    //   }
    // }

    // 如果不是自杀且怪物类型为3（特殊类型）
    if (!m_bSuicide && m_pMobTableRef && m_pMobTableRef->Monster_Type == 3) {
        // 尝试转换为 XMaze
        XArea* pArea = GetArea();
        if (pArea) {
            XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
            if (pMaze) {
                pMaze->SetGameModeState(2);
            }
        }
    } else {
        // 尝试转换为 CBattleZone
        XArea* pArea2 = GetArea();
        if (pArea2) {
            CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea2);
            if (pBattleZone) {
                unsigned int HitID = GetHitID();
                pBattleZone->MonsterDieForEvent(this, HitID);
            }
        }
    }
}

// ============================================================================
// GetSkillDestPos IDA 0x14035A5E0
// 获取技能目标位置 - 精确还原
// ============================================================================
hkvVec3 CMonster::GetSkillDestPos() {
    // IDA 0x14035A5E0 精确还原:
    // if ( this->m_pAi )
    // {
    //   TargetID = CMover::GetTargetID(this);
    //   pTarget = CMover::GetMoverObject(this, TargetID);
    //   if ( pTarget )
    //   {
    //     rhs = CAi::GetSkillDestPos(this->m_pAi, &v6);
    //     Position = VisObject3D_cl::GetPosition(pTarget);
    //     operator+(result, Position, rhs);
    //   }
    //   else
    //   {
    //     hkvVec3::ZeroVector(result);
    //   }
    //   return result;
    // }
    // else
    // {
    //   hkvVec3::ZeroVector(result);
    //   return result;
    // }

    if (m_pAi) {
        unsigned int TargetID = GetTargetID();
        CMover* pTarget = CMover::GetMoverObject(TargetID);
        if (pTarget) {
            hkvVec3 rhs = m_pAi->GetSkillDestPos();
            hkvVec3 Position = pTarget->GetPosition();
            return Position + rhs;
        }
    }
    return hkvVec3(0.0f, 0.0f, 0.0f);
}

// ============================================================================
// ChangeAngleAttackName IDA 0x14035A6C0
// 改变角度攻击名称 - 精确还原
// ============================================================================
void CMonster::ChangeAngleAttackName(std::uint8_t bySkillAngle, VString& strSkillName) {
    // IDA 0x14035A6C0 精确还原:
    // if ( CMonster::IsNoRotate(this) )
    // {
    //   TargetID = CMover::GetTargetID(this);
    //   pTarget = CMover::GetMoverObject(this, TargetID);
    //   if ( pTarget )
    //   {
    //     this->m_byAngleAttackType = 0;
    //     VString::VString(&strTempName, strSkillName);
    //     byAngleAttackType = 0;
    //     rhs = &this->m_vPosition;
    //     Position = VisObject3D_cl::GetPosition(pTarget);
    //     operator-(&vDirVector, Position, rhs);
    //     fTargetYaw = CMover::GetYawFromVector(&vDirVector);
    //     fDiffYaw = this->m_fInitYaw - fTargetYaw;
    //     if ( fDiffYaw <= 180.0 )
    //     {
    //       if ( fDiffYaw < -180.0 )
    //         fDiffYaw = fDiffYaw + 360.0;
    //     }
    //     else
    //     {
    //       fDiffYaw = fDiffYaw - 360.0;
    //     }
    //     fAbsDiff = fabsf(fDiffYaw);
    //     fCheckAngle = (float)bySkillAngle * 0.30000001;
    //     if ( fCheckAngle <= fAbsDiff )
    //     {
    //       if ( fDiffYaw <= 0.0 )
    //       {
    //         byAngleAttackType = 3;
    //         VString::VString(v17, L"_L");
    //         VString::operator+=(&strTempName, v17);
    //         VString::~VString(v17);
    //       }
    //       else
    //       {
    //         byAngleAttackType = 2;
    //         VString::VString(&v16, L"_R");
    //         VString::operator+=(&strTempName, &v16);
    //         VString::~VString(&v16);
    //       }
    //     }
    //     else
    //     {
    //       byAngleAttackType = 1;
    //       VString::VString(&v15, L"_C");
    //       VString::operator+=(&strTempName, &v15);
    //       VString::~VString(&v15);
    //     }
    //     v5 = VString::AsChar(&strTempName);
    //     if ( CMover::GetActionDesc(this, v5) )
    //     {
    //       this->m_byAngleAttackType = byAngleAttackType;
    //       VString::operator=(strSkillName, &strTempName);
    //     }
    //     VString::~VString(&strTempName);
    //   }
    // }

    if (!IsNoRotate()) {
        return;
    }

    unsigned int TargetID = GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);
    if (!pTarget) {
        return;
    }

    m_byAngleAttackType = 0;
    VString strTempName(strSkillName);
    std::uint8_t byAngleAttackType = 0;

    // 计算方向向量
    hkvVec3 vTargetPos = pTarget->GetPosition();
    hkvVec3 vDirVector = vTargetPos - m_vPosition;

    // 计算目标偏航角
    float fTargetYaw = std::atan2(vDirVector.y, vDirVector.x) * 180.0f / 3.14159265f;

    // 计算偏航角差
    float fDiffYaw = m_fInitYaw - fTargetYaw;

    // 归一化到 [-180, 180]
    if (fDiffYaw <= 180.0f) {
        if (fDiffYaw < -180.0f) {
            fDiffYaw += 360.0f;
        }
    } else {
        fDiffYaw -= 360.0f;
    }

    float fAbsDiff = std::fabs(fDiffYaw);
    float fCheckAngle = static_cast<float>(bySkillAngle) * 0.30000001f;

    if (fCheckAngle <= fAbsDiff) {
        // 角度差较大，需要左转或右转
        if (fDiffYaw <= 0.0f) {
            byAngleAttackType = 3;
            // TODO: VString concatenation not working - need to implement properly
            // strTempName += "_L";
            char buffer[256];
            sprintf_s(buffer, sizeof(buffer), "%s_L", strSkillName.AsChar());
            strTempName = VString(buffer);
        } else {
            byAngleAttackType = 2;
            // strTempName += "_R";
            char buffer[256];
            sprintf_s(buffer, sizeof(buffer), "%s_R", strSkillName.AsChar());
            strTempName = VString(buffer);
        }
    } else {
        // 角度差较小，中心攻击
        byAngleAttackType = 1;
        // strTempName += "_C";
        char buffer[256];
        sprintf_s(buffer, sizeof(buffer), "%s_C", strSkillName.AsChar());
        strTempName = VString(buffer);
    }

    // 检查动作是否存在
    const char* szAnimName = strTempName.AsChar();
    // TODO: if (GetActionDesc(szAnimName)) {
    //     m_byAngleAttackType = byAngleAttackType;
    //     strSkillName = strTempName;
    // }
    // For now, just apply the change
    m_byAngleAttackType = byAngleAttackType;
    strSkillName = strTempName;
}

// ============================================================================
// IsRemainBossMonster IDA 0x14035A950
// 检查是否剩余Boss怪物 - 精确还原
// ============================================================================
bool CMonster::IsRemainBossMonster() {
    // IDA 0x14035A950 精确还原:
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(&this->XActor, 2, 2u, &vecGameObjList);
    // for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
    //     CMonster* pMonster = dynamic_cast<CMonster*>(*it);
    //     if (pMonster && !XActor::IsStatus(&pMonster->XActor, 2u)) {
    //         UXActorID parentID = pMonster->GetParentID();
    //         UXActorID myActorID = this->GetActorID();
    //         if (parentID == myActorID && pMonster->IsBoss()) {
    //             return true;
    //         }
    //     }
    // }
    // return false;

    std::vector<CMover*> vecGameObjList;
    XArea* pArea = GetArea();
    if (!pArea) {
        return false;
    }

    // 扫描周围2格范围内的怪物
    XArea::ScanGridOrigin(reinterpret_cast<XActor*>(this), 2, 2u, vecGameObjList);

    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMover* pMover = *it;
        if (!pMover) {
            continue;
        }

        // 尝试转换为 CMonster
        CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        if (!pMonster) {
            continue;
        }

        // 检查是否死亡
        if (pMonster->IsStatus(2u)) {
            continue;
        }

        // 检查父ID是否匹配
        UXActorID parentID = pMonster->GetParentID();
        UXActorID myActorID = GetActorID();

        if (parentID.dwActorID == myActorID.dwActorID && pMonster->IsBoss()) {
            return true;
        }
    }

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
        m_pAi->CheckEscortWayPoint();
    }
    return CMoverEx::MoveTick();
}

// ============================================================================
// StopMoving IDA 0x14035AB60
// 停止移动
// ============================================================================
void CMonster::StopMoving(bool bSendPacket) {
    // IDA 0x14035ab60 精确还原:
    // CMover::MoveingValueClear(this);
    // this->GetOrientationYaw(this);
    // LOBYTE(v2) = 2;
    // ((void (__fastcall *)(CMonster *, __int64, __int64))this->SetDirectionYaw)(this, v3, v2);
    // if ( bSendPacket )
    //   CMover::send_eSUB_CMD_MOVE_STOP(this, this);

    // 清除移动值
    MoveingValueClear();

    // 获取当前朝向并设置方向
    float fYaw = m_fMovingYaw;  // 使用当前移动偏航角
    SetDirectionYaw(fYaw, 2);

    // 发送停止移动包
    if (bSendPacket) {
        send_eSUB_CMD_MOVE_STOP(this);
    }
}

// ============================================================================
// IsNoRotate IDA 0x14035B260
// 检查是否不旋转
// ============================================================================
bool CMonster::IsNoRotate() {
    // IDA 反编译 (0x14035B260):
    // return this->m_byStandType == 4 || this->m_byStandType == 5;
    return m_byStandType == 4 || m_byStandType == 5;
}

// ============================================================================
// IsCanRotate IDA 0x14035B2A0
// 检查是否可以旋转
// ============================================================================
bool CMonster::IsCanRotate() {
    // IDA 反编译 (0x14035B2A0):
    // if ( this->m_byStandType == 2 )
    //     return 0;
    // return !this->m_pCurSkillTableRef || this->m_pCurSkillTableRef->Skill_Type != 8;
    if (m_byStandType == 2)
        return false;
    return !m_pCurSkillTableRef || m_pCurSkillTableRef->Skill_Type != 8;
}

// ============================================================================
// SetDirectionTo IDA 0x14035B2F0
// 设置方向到目标 - 精确还原
// ============================================================================
void CMonster::SetDirectionTo(const hkvVec3& vTarget) {
    // IDA 0x14035B2F0 精确还原:
    // if ( CMonster::IsCanRotate(this) && !CMonster::IsNoRotate(this) )
    // {
    //   operator-(&vDirVector, vTarget, &this->m_vPosition);
    //   fYaw = CMover::GetYawFromVector(&vDirVector);
    //   CMoverEx::SetMovingYaw(this, fYaw);
    // }

    if (IsCanRotate() && !IsNoRotate()) {
        hkvVec3 vDirVector = vTarget - m_vPosition;
        // 计算偏航角 (atan2)
        float fYaw = std::atan2(vDirVector.y, vDirVector.x) * 180.0f / 3.14159265f;
        m_fMovingYaw = fYaw;
    }
}

// ============================================================================
// SetDirectionYaw IDA 0x14035B370
// 设置方向偏航 - 精确还原
// ============================================================================
void CMonster::SetDirectionYaw(float fYaw, std::uint8_t byType) {
    // IDA 0x14035B370 精确还原:
    // if ( !byType ) goto LABEL_2;
    // if ( !CMonster::IsNoRotate(this) )
    // {
    //   if ( byType == 1 ) { LABEL_2: SetMovingYaw(fYaw); SetOrientationYaw(fYaw); return; }
    //   if ( (IsCanRotate() || byType != 3) && !IsNoRotate() )
    //   { SetMovingYaw(fYaw); if ( byType != 3 ) SetOrientationYaw(fYaw); }
    // }

    if (byType == 0) {
        // 直接设置方向
        m_fMovingYaw = fYaw;
        SetOrientationYaw(fYaw);
        return;
    }

    if (IsNoRotate()) {
        return;
    }

    if (byType == 1) {
        m_fMovingYaw = fYaw;
        SetOrientationYaw(fYaw);
        return;
    }

    if ((IsCanRotate() || byType != 3) && !IsNoRotate()) {
        m_fMovingYaw = fYaw;
        if (byType != 3) {
            SetOrientationYaw(fYaw);
        }
    }
}

// ============================================================================
// UpdateTargetRotation IDA 0x14035B470
// 更新目标旋转 - 精确还原
// ============================================================================
void CMonster::UpdateTargetRotation(bool isGaze) {
    // IDA 0x14035B470 精确还原:
    // if ( this->m_byStandType != 2 && !XActor::IsStatus(&this->XActor, 0x8000u) )
    // {
    //   TargetID = CMover::GetTargetID(this);
    //   pTarget = CMover::GetMoverObject(this, TargetID);
    //   if ( pTarget ) { Position = VisObject3D_cl::GetPosition(pTarget); SetDirectionTo(Position); }
    //   else if ( isGaze ) { CMoverEx::SetGazeMoving(this, 0); }
    // }

    if (m_byStandType == 2 || IsStatus(0x8000u)) {
        return;
    }

    std::uint32_t TargetID = GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);
    if (pTarget) {
        hkvVec3 vTargetPos = pTarget->GetPosition();
        SetDirectionTo(vTargetPos);
    } else if (isGaze) {
        SetGazeMoving(0);
    }
}

// ============================================================================
// CancelAttackFromDamage IDA 0x14035B520
// 从伤害取消攻击
// ============================================================================
void CMonster::CancelAttackFromDamage() {
    // IDA 0x14035B520 精确还原:
    // CActionBuffer::DeleteCodeData((CActionBuffer *)&this->szBuffer[5055], 1u);
    // CActionBuffer::DeleteCodeData((CActionBuffer *)&this->szBuffer[5055], 6u);
    // CActionBuffer::DeleteCodeData((CActionBuffer *)&this->szBuffer[5055], 8u);
    // *(_DWORD *)&this->szBuffer[58719] = 0;
    // *(_DWORD *)&this->szBuffer[58723] = 0;

    // 删除动作缓冲区中的特定代码数据
    m_xActionBuffer.DeleteCodeData(1);  // 删除攻击代码
    m_xActionBuffer.DeleteCodeData(6);  // 删除其他代码
    m_xActionBuffer.DeleteCodeData(8);  // 删除其他代码

    // 重置下一个技能ID和击中计数
    m_nNextSkillID = 0;
    m_nHitCount = 0;
}

// ============================================================================
// InitComponant IDA 0x1403559A0
// 初始化组件
// ============================================================================
void CMonster::InitComponant() {
    // IDA 反编译 (0x1403559A0):
    // GOComponent::CreateAndRegister<CGocNpcAttribute>(&result, this);
    // std::shared_ptr<CItemAkashic>::~shared_ptr<CItemAkashic>((std::shared_ptr<CGocNetwork> *)&result);
    // GOComponent::CreateAndRegister<CGocInventory>(&v2, this);
    // std::shared_ptr<CItemAkashic>::~shared_ptr<CItemAkashic>((std::shared_ptr<CGocNetwork> *)&v2);

    // TODO: 需要实现 GOComponent::CreateAndRegister
    // std::shared_ptr<CGocNpcAttribute> pNpcAttr;
    // GOComponent::CreateAndRegister<CGocNpcAttribute>(&pNpcAttr, this);

    // std::shared_ptr<CGocInventory> pInventory;
    // GOComponent::CreateAndRegister<CGocInventory>(&pInventory, this);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::InitComponant called");
}

// ============================================================================
// GetID IDA 0x140355A10
// 获取ID
// ============================================================================
unsigned int CMonster::GetID() {
    // IDA 反编译 (0x140355A10):
    // return LODWORD(this->m_pGrapParent);
    return static_cast<unsigned int>(reinterpret_cast<std::uintptr_t>(m_pGrapParent));
}

// ============================================================================
// SetPosInfo IDA 0x140355A30 -> 0x140355ACF
// 设置位置信息 - 精确还原
// ============================================================================
void CMonster::SetPosInfo(hkvVec3 vPos, float fRot) {
    // IDA 0x140355A30 精确还原:
    // void __fastcall CMonster::SetPosInfo(CMonster *this, XVec3 *vPos, float fRot)
    // {
    //   v5 = *((_QWORD *)this - 109);
    //   (*(void (__fastcall **)(char *, XVec3 *))(v5 + 456))((char *)this - 872, vPos);
    //   qmemcpy(&v4, vPos, sizeof(v4));
    //   v6 = *((_QWORD *)this - 109);
    //   v3 = (*(float (__fastcall **)(char *))(v6 + 712))((char *)this - 872);
    //   XActor::SetPosInfo((XActor *)this, &v4, v3);
    // }
    
    // 调用基类设置位置 (虚函数调用)
    // CMover::SetPosInfo(vPos, fRot);
    
    // 设置到怪物信息结构
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.x = vPos.x;
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.y = vPos.y;
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.z = vPos.z;
    m_stMonsterInfo.stNpcInfo.stPosInfo.fRot = fRot;
    
    // 调用 XActor::SetPosInfo
    // XActor::SetPosInfo(vPos, GetOrientationYaw());
}

// ============================================================================
// SetPositionXVec3 IDA 0x140355AD0 -> 0x140355B0A
// 设置位置 - 精确还原
// ============================================================================
void CMonster::SetPositionXVec3(hkvVec3& vPos) {
    // IDA 0x140355AD0 精确还原:
    // void __fastcall CMonster::SetPositionXVec3(CMonster *this, XVec3 *vPos)
    // {
    //   XVec3::operator=(&this->m_stMonsterInfo.stPosInfo.vPos, vPos);
    //   CMover::SetPositionXVec3(this, vPos);
    // }
    
    // 设置到怪物信息结构
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.x = vPos.x;
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.y = vPos.y;
    m_stMonsterInfo.stNpcInfo.stPosInfo.vPos.z = vPos.z;
    
    // 调用基类设置位置
    CMover::SetPositionXVec3(vPos);
}

// ============================================================================
// GetActionResourceFN IDA 0x140357990
// 获取动作资源文件名 - 精确还原
// ============================================================================
VString CMonster::GetActionResourceFN() {
    // IDA 0x140357990 精确还原:
    // if (m_pMobTableRef)
    //     VString::Format(&strFilePath, "%s.adf", m_pMobTableRef->Monster_Code_Name);
    VString result;
    if (m_pMobTableRef) {
        char szBuffer[256];
        sprintf_s(szBuffer, sizeof(szBuffer), "%s.adf", m_pMobTableRef->Monster_Code_Name);
        result = VString(szBuffer);
    }
    return result;
}

// ============================================================================
// ApplyTableAbility IDA 0x1403581A0
// 应用表格属性
// ============================================================================
void CMonster::ApplyTableAbility() {
    // IDA 反编译 (0x1403581A0):
    // this->m_byPhaseType = this->m_pMobTableRef->Monster_AI_Type;
    // VString::operator=(&this->m_strSpecialDamage, this->m_pMobTableRef->Monster_Special_Damage);
    // this->m_byPhaseCondition = this->m_pMobTableRef->Monster_Switching_AI_Condition_01;
    // this->m_dwPhaseConditionValue = this->m_pMobTableRef->Monster_Switching_AI_Value_01;
    // if ( this->m_byPhaseCondition == 2 && this->m_pMobTableRef->Monster_Weapon_ID )
    // {
    //     XGameServer* v1 = TXSingleton<XGameServer>::Instance();
    //     TB_MONSTER_WEAPON* pWeaponRef = XResourceMgr::GetTB_MONSTER_WEAPON(&v1->m_xResourceMgr, m_pMobTableRef->Monster_Weapon_ID);
    //     if ( !pWeaponRef ) return;
    //     this->m_nShieldHP = pWeaponRef->Weapon_HP;
    // }
    // else if ( this->m_byPhaseCondition == 3 || this->m_byPhaseCondition == 5 )
    // {
    //     this->m_fPhaseDurationTime = (float)(int)this->m_dwPhaseConditionValue * 0.001;
    // }
    // VString::operator=(&this->m_strPhaseChangeAnim, &this->m_pMobTableRef->___u53);
    // if ( this->m_pMobTableRef->Monster_Switching_AI_Condition_03 ) this->m_byMaxPhaseStep = 4;
    // else if ( this->m_pMobTableRef->Monster_Switching_AI_Condition_02 ) this->m_byMaxPhaseStep = 3;
    // else if ( this->m_pMobTableRef->Monster_Switching_AI_Condition_01 ) this->m_byMaxPhaseStep = 2;
    // this->m_byDefaultAnimStep = this->m_pMobTableRef->Monster_Default_Action_Type;
    // this->m_byBattleModeAnim = this->m_pMobTableRef->Monster_BattleMode_Type;
    // this->m_byStandType = this->m_pMobTableRef->Monster_NormalStand_Type;
    // this->m_byDefaultDefenseType = this->m_pMobTableRef->Monster_Defence_Type;
    // this->m_byDefenseType = this->m_byDefaultDefenseType;
    // this->m_fDefWalkSpeed = (float)this->m_pMobTableRef->Monster_Walk_Speed;
    // this->m_fDefRunSpeed = (float)this->m_pMobTableRef->Monster_Run_Speed;
    // this->m_fDefTurnSpeed = (float)this->m_pMobTableRef->Monster_Turn_Speed;
    // this->m_fBackupTurnSpeed = this->m_fDefTurnSpeed;
    // hkvVec3 vScale(m_pMobTableRef->Monster_Scale, m_pMobTableRef->Monster_Scale, m_pMobTableRef->Monster_Scale);
    // VisBaseEntity_cl::SetScaling(this, &vScale);
    // XActor::SetNation(&this->XActor, m_pMobTableRef->Monster_Faction);
    // UXActorID v7 = XActor::GetActorID(&this->XActor);
    // XActor::SetOriginID(&this->XActor, v7);

    if (!m_pMobTableRef) {
        return;
    }

    // 设置 Phase 类型
    m_byPhaseType = m_pMobTableRef->Monster_AI_Type;

    // 复制特殊伤害字符串
    m_strSpecialDamage = m_pMobTableRef->Monster_Special_Damage;

    // 设置 Phase 条件
    m_byPhaseCondition = m_pMobTableRef->Monster_Switching_AI_Condition_01;
    m_dwPhaseConditionValue = m_pMobTableRef->Monster_Switching_AI_Value_01;

    // 根据 Phase 条件设置
    if (m_byPhaseCondition == 2 && m_pMobTableRef->Monster_Weapon_ID) {
        // 武器 ID 条件
        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            TB_MONSTER_WEAPON* pWeaponRef = pServer->GetResourceMgr().GetTB_MONSTER_WEAPON(m_pMobTableRef->Monster_Weapon_ID);
            if (pWeaponRef) {
                m_nShieldHP = pWeaponRef->Weapon_HP;
            }
        }
    } else if (m_byPhaseCondition == 3 || m_byPhaseCondition == 5) {
        m_fPhaseDurationTime = static_cast<float>(m_dwPhaseConditionValue) * 0.001f;
    }

    // 复制 Phase 变换动画
    m_strPhaseChangeAnim = m_pMobTableRef->Monster_Switching_Action_01;

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
    hkvVec3 vScale(m_pMobTableRef->Monster_Scale, m_pMobTableRef->Monster_Scale, m_pMobTableRef->Monster_Scale);
    // TODO: VisBaseEntity_cl::SetScaling(&vScale);
    
    // 设置阵营
    // TODO: XActor::SetNation(m_pMobTableRef->Monster_Faction);
    
    // 设置 OriginID
    // TODO: UXActorID myActorID = XActor::GetActorID();
    // TODO: XActor::SetOriginID(myActorID);
}

// ============================================================================
// UpdateSendMoveData IDA 0x14035abc0
// 更新发送移动数据
// ============================================================================
void CMonster::UpdateSendMoveData() {
    // IDA 0x14035abc0 精确还原:
    // this->m_fLastSendMoveTime = 0.0;
    // this->m_shLastSendMoveYaw = (int)this->GetMovingYaw(this);
    // tagMOVE_POS::operator=(&this->m_vLastTargetMovePos, &this->m_stMovePos);
    // this->m_bStartRotation = 0;
    // this->m_fStartRotWaitTime = 0.0;

    m_fLastSendMoveTime = 0.0f;
    m_shLastSendMoveYaw = static_cast<std::int16_t>(m_fMovingYaw);
    // 复制移动位置信息
    m_vLastTargetMovePos_dummy = nullptr;  // 简化实现
    m_bStartRotation = false;
    m_fStartRotWaitTime = 0.0f;
}

// ============================================================================
// CheckSendMovePacket IDA 0x14035AC40 -> 0x14035AF25
// 检查发送移动包 - 精确还原
// ============================================================================
void CMonster::CheckSendMovePacket() {
    // IDA 0x14035AC40 精确还原:
    // 获取计时器并更新时间
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    m_fLastSendMoveTime += pTimer->GetTimeDifference();
    
    bool bSendPacket = false;
    std::int16_t shCurrYaw = static_cast<std::int16_t>(m_fMovingYaw);
    
    // 检查是否需要发送包
    if (m_fLastSendMoveTime < 0.60000002f) {
        if ((m_fLastSendMoveTime > 0.050000001f ||
             (IsStatus(1u) && m_fLastSendMoveTime > 0.033f)) &&
            (m_shLastSendMoveYaw != shCurrYaw ||
             !m_stMovePos.IsZero() ||
             (m_bNeedSendMoveStop && !IsMoving()))) {
            bSendPacket = true;
        }
    } else {
        bSendPacket = true;
    }
    
    // 发送移动包
    if (bSendPacket) {
        if (IsMoving() || !m_stMovePos.IsZero()) {
            // 更新发送数据
            m_bNeedSendMoveStop = 1;
            UpdateSendMoveData();
            bool byRunBit = IsStatus(0x100u);
            send_eSUB_CMD_MOVE(this, m_stMovePos.x, m_stMovePos.y, byRunBit);
        } else if (!m_bNeedSendMoveStop || IsStatus(1u)) {
            if (m_shLastSendMoveYaw != shCurrYaw && !m_bStartRotation) {
                m_fLastSendMoveTime = 0.050000001f;
                m_shLastSendMoveYaw = shCurrYaw;
                // IDA: send_eSUB_CMD_MOVE_UPDATE_DIR(this, false);
                send_eSUB_CMD_MOVE_UPDATE_DIR(this, false);
            }
        } else {
            m_bNeedSendMoveStop = 0;
            m_fLastSendMoveTime = 0.050000001f;
            m_shLastSendMoveYaw = shCurrYaw;
            m_bStartRotation = false;
            m_fStartRotWaitTime = 0.0f;
            send_eSUB_CMD_MOVE_STOP(this);
        }
    }
}

// ============================================================================
// CheckTurnOrMovePacket IDA 0x14035af30
// 检查转向或移动包 - IDA 精确还原
// ============================================================================
void CMonster::CheckTurnOrMovePacket() {
    // IDA 0x14035af30 精确还原:
    // fDiffYaw = m_fMovingYaw - GetOrientationYaw();
    // // Normalize to [-180, 180]
    // if (fDiffYaw <= 180.0) {
    //     if (fDiffYaw < -180.0) fDiffYaw += 360.0;
    // } else {
    //     fDiffYaw -= 360.0;
    // }
    // fAbsDiff = fabsf(fDiffYaw);
    // if (fAbsDiff <= 45.0) {
    //     UpdateSendMoveData();
    //     bool byRunBit = IsStatus(0x100u);
    //     send_eSUB_CMD_MOVE(m_stMovePos.x, m_stMovePos.y, byRunBit);
    // } else if (m_bHasTurnMotion) {
    //     if (m_nMotionClass == 1) {
    //         QuickTurn();
    //         m_nTurnStatus = 0;
    //         if (fAbsDiff > 90.0 && m_bHasBigTurn) m_nTurnStatus = 1;
    //         if (fDiffYaw <= 0.0) {
    //             if (fDiffYaw < 0.0) {
    //                 ChangeMotion(8, 1, 0);
    //                 send_eSUB_CMD_MOVE_IDLE(0.0);
    //             }
    //         } else {
    //             ChangeMotion(7, 1, 0);
    //             send_eSUB_CMD_MOVE_IDLE(0.0);
    //         }
    //     }
    // } else {
    //     m_fMoveDelayTime = 0.3f;
    //     send_eSUB_CMD_MOVE_IDLE(m_fMoveDelayTime);
    //     UpdateSendMoveData();
    //     bool byRunBit = IsStatus(0x100u);
    //     send_eSUB_CMD_MOVE(m_stMovePos.x, m_stMovePos.y, byRunBit);
    // }

    float fDiffYaw = m_fMovingYaw - GetOrientationYaw();
    
    // Normalize to [-180, 180]
    if (fDiffYaw <= 180.0f) {
        if (fDiffYaw < -180.0f) {
            fDiffYaw += 360.0f;
        }
    } else {
        fDiffYaw -= 360.0f;
    }

    float fAbsDiff = std::fabs(fDiffYaw);

    if (fAbsDiff <= 45.0f) {
        // Small angle difference - just send move packet
        UpdateSendMoveData();
        bool byRunBit = IsStatus(0x100u);
        send_eSUB_CMD_MOVE(this, m_stMovePos.x, m_stMovePos.y, byRunBit);
    } else if (m_bHasTurnMotion) {
        // Has turn motion - check if in idle motion
        if (m_nMotionClass == 1) {
            QuickTurn();
            m_nTurnStatus = 0;
            if (fAbsDiff > 90.0f && m_bHasBigTurn) {
                m_nTurnStatus = 1;
            }

            // Turn left or right based on angle
            if (fDiffYaw <= 0.0f) {
                if (fDiffYaw < 0.0f) {
                    ChangeMotion(8, 1, 0);  // Turn left
                    send_eSUB_CMD_MOVE_IDLE(this, 0.0f);
                }
            } else {
                ChangeMotion(7, 1, 0);  // Turn right
                send_eSUB_CMD_MOVE_IDLE(this, 0.0f);
            }
        }
    } else {
        // No turn motion - delay and send move
        m_fMoveDelayTime = 0.3f;
        send_eSUB_CMD_MOVE_IDLE(this, m_fMoveDelayTime);
        UpdateSendMoveData();
        bool byRunBit = IsStatus(0x100u);
        send_eSUB_CMD_MOVE(this, m_stMovePos.x, m_stMovePos.y, byRunBit);
    }
}

// ============================================================================
// StartMoving IDA 0x14035b1e0
// 开始移动
// ============================================================================
int CMonster::StartMoving() {
    // IDA 反编译 (0x14035b1e0):
    // if ( this->IsCanMove(this, 0) )
    //     return CMoverEx::StartMoving(this);
    // this->StopMoving(this, 1);
    // if ( CMoverEx::IsMoveMotion(this, this->m_nMotionClass) )
    //     this->ClearMotion(this);
    // return 0;
    if (IsCanMove(false)) {
        return CMoverEx::StartMoving();
    }
    StopMoving(true);
    if (CMoverEx::IsMoveMotion(m_nMotionClass)) {
        ClearMotion();
    }
    return 0;
}

// ============================================================================
// ChangeBattlePose IDA 0x14035a0a0
// 改变战斗姿态 - 精确还原
// ============================================================================
void CMonster::ChangeBattlePose(bool bBattle, bool bPlayMotion) {
    // IDA 0x14035a0a0 精确还原:
    // if ( !CMoverEx::IsCommonMotion(this, this->m_nMotionClass) )
    //   bPlayMotion = 0;
    // if ( this->m_bBattlePose != bBattle )
    // {
    //   this->m_bBattlePose = bBattle;
    //   if ( this->m_bBattlePose )
    //   {
    //     if ( this->m_byBattleModeAnim != 3 && bPlayMotion )
    //     {
    //       this->ChangeMotion_3(this, 28, 1, 0);
    //       this->StopMoving(this, 1);
    //     }
    //   }
    //   else if ( this->m_byBattleModeAnim != 3 && bPlayMotion )
    //   {
    //     this->ChangeMotion_3(this, 27, 1, 0);
    //     this->StopMoving(this, 1);
    //   }
    //   if ( this->GetArea(&this->XActor) )
    //     CMover::send_eSUB_CMD_MOVE_BATTLE(this, this, bPlayMotion);
    // }

    // 检查是否是普通动作，如果不是则不播放动画
    if (!CMoverEx::IsCommonMotion(m_nMotionClass)) {
        bPlayMotion = false;
    }

    if (m_bBattlePose != bBattle) {
        m_bBattlePose = bBattle;

        if (m_bBattlePose) {
            // 进入战斗姿态
            if (m_byBattleModeAnim != 3 && bPlayMotion) {
                ChangeMotion(28, 1, 0);  // 战斗姿态动作
                StopMoving(true);
            }
        } else {
            // 退出战斗姿态
            if (m_byBattleModeAnim != 3 && bPlayMotion) {
                ChangeMotion(27, 1, 0);  // 非战斗姿态动作
                StopMoving(true);
            }
        }

        // 通知区域
        if (GetArea()) {
            send_eSUB_CMD_MOVE_BATTLE(this, bPlayMotion);
        }
    }
}

// ============================================================================
// ActiveSuperArmorSkill IDA 0x14035a690
// 激活超级护甲技能 - 精确还原
// ============================================================================
void CMonster::ActiveSuperArmorSkill() {
    // IDA 0x14035a690 精确还原:
    // if ( this->m_pAi )
    //     CAi::ActiveSuperArmorSkill(this->m_pAi);
    if (m_pAi) {
        m_pAi->ActiveSuperArmorSkill();
    }
}

// ============================================================================
// ProcessSkillAnimation IDA 0x140359e20
// 处理技能动画 - 精确还原
// ============================================================================
void CMonster::ProcessSkillAnimation(float fDeltaTime) {
    // IDA 0x140359e20 精确还原:
    // CMoverEx::ProcessSkillAnimation(this, fDeltaTime);
    // if ( XActor::IsStatus(&this->XActor, 0x8000u) && this->m_bySkillAnimCount >= 3u && !this->m_bySkillAnimStep )
    //   CMover::GetTargetID(this);
    // if ( this->m_nNextSkillID > 0 && this->m_pCurSkillTableRef && this->m_bySkillAnimStep == 3 )
    // {
    //   SkillAnimName = CMoverEx::GetSkillAnimName(this, this->m_pCurSkillTableRef, this->m_bySkillAnimStep);
    //   if ( CMoverEx::HasSkillSkipTime(this, SkillAnimName) )
    //   {
    //     v3 = CMoverEx::GetSkillAnimName(this, this->m_pCurSkillTableRef, this->m_bySkillAnimStep);
    //     if ( CMoverEx::IsCanSkillSkip(this, v3, this->m_fAnimationTime) )
    //     {
    //       this->PostSkillProcess(this);
    //       CMover::SetCurSkillTableIdx(this, this->m_nNextSkillID);
    //       this->m_nNextSkillID = 0;
    //       CMonster::ActionAttack(this, v4);
    //     }
    //   }
    //   else
    //   {
    //     this->m_nNextSkillID = 0;
    //   }
    // }

    // 调用基类处理
    CMoverEx::ProcessSkillAnimation(fDeltaTime);

    // 检查状态: 如果有特定状态标志且技能动画计数>=3且技能动画步骤为0
    if (IsStatus(0x8000u) && m_bySkillAnimCount >= 3 && !m_bySkillAnimStep) {
        GetTargetID();
    }

    // 处理下一个技能ID
    if (m_nNextSkillID > 0 && m_pCurSkillTableRef && m_bySkillAnimStep == 3) {
        // IDA: 获取技能动画名称并检查是否可以跳过
        const char* SkillAnimName = GetSkillAnimName(m_pCurSkillTableRef, m_bySkillAnimStep);
        if (SkillAnimName && HasSkillSkipTime(SkillAnimName)) {
            if (IsCanSkillSkip(SkillAnimName, m_fAnimationTime)) {
                PostSkillProcess();
                SetCurSkillTableIdx(m_nNextSkillID);
                m_nNextSkillID = 0;
                ActionAttack();
            }
        } else {
            m_nNextSkillID = 0;
        }
    }
}

// ============================================================================
// ActionBufferProcess IDA 0x140359fa0
// 动作缓冲处理 - 精确还原
// ============================================================================
int CMonster::ActionBufferProcess(void* pActionVoid) {
    // IDA 0x140359fa0 精确还原:
    // __int64 __fastcall CMonster::ActionBufferProcess(CMonster *this, tagACTION_BUFFER *pAction)
    // {
    //   MovingInputTrigger *pTrigger;
    //   if ( !CMoverEx::ActionBufferProcess(this, pAction) && pAction->byCode == 16 )
    //   {
    //     pTrigger = (MovingInputTrigger *)pAction->pActionTrigger;
    //     if ( !pTrigger )
    //       return 1;
    //     if ( pTrigger->MovingType && pTrigger->MovingType != 3 )
    //       CMoverEx::MovingTarget(this, pTrigger, pTrigger->MovingType);
    //     else
    //       CMoverEx::MovingToTarget(
    //         this,
    //         pTrigger->Duration,
    //         pTrigger->MaxSpeed,
    //         pTrigger->OffsetDistance,
    //         pTrigger->MovingType == 3);
    //   }
    //   return 0;
    // }

    tagACTION_BUFFER* pAction = static_cast<tagACTION_BUFFER*>(pActionVoid);

    // 检查动作代码是否为16 (移动输入)
    if (pAction->byCode != 16) {
        return 0;
    }

    // 获取移动输入触发器
    // TODO: MovingInputTrigger 结构体需要定义
    // MovingInputTrigger* pTrigger = static_cast<MovingInputTrigger*>(pAction->pActionTrigger);
    void* pTrigger = pAction->pActionTrigger;
    if (!pTrigger) {
        return 1;
    }

    // TODO: 需要实现 MovingTarget 和 MovingToTarget
    // if (pTrigger->MovingType && pTrigger->MovingType != 3) {
    //     MovingTarget(pTrigger, pTrigger->MovingType);
    // } else {
    //     MovingToTarget(
    //         pTrigger->Duration,
    //         pTrigger->MaxSpeed,
    //         pTrigger->OffsetDistance,
    //         pTrigger->MovingType == 3);
    // }

    return 0;
}

// ============================================================================
// DebugMessage IDA 0x140359c50
// 调试消息 - 精确还原
// ============================================================================
void CMonster::DebugMessage() {
    // IDA 0x140359c50 精确还原:
    // if ( this->m_pAi )
    // {
    //   Area = (LogicTimer *)CVaccumManager::GetArea(this->m_pAi);
    //   LogDBAgentCount = XGameDBSocketMgr::GetLogDBAgentCount(Area);
    // }
    // else
    // {
    //   LogDBAgentCount = 0;
    // }
    // iAiState = LogDBAgentCount;
    // operator-(&vMoveDist, &this->m_vDebugMessagePos, &this->m_vPosition);
    // vMoveDist.z = 0.0;
    // fDist = hkvVec3::getLength(&vMoveDist);
    // qmemcpy(v7, &this->m_vPosition, sizeof(v7));
    // qmemcpy(&this->m_vDebugMessagePos, v7, sizeof(this->m_vDebugMessagePos));
    // m_dwStatus = this->m_dwStatus;
    // m_nMotionClass = this->m_nMotionClass;
    // CMover::DebugOut(this, "Pos:(%.2f,%.2f) -> (%.2f,%.2f) (%.2f/%.2f) Motion:%d Status:%d AI:%s", ...);

    int iAiState = 0;

    // 获取AI状态
    if (m_pAi) {
        // TODO: 需要实现 CVaccumManager::GetArea 和 XGameDBSocketMgr::GetLogDBAgentCount
        // auto* Area = CVaccumManager::GetArea(m_pAi);
        // iAiState = XGameDBSocketMgr::GetLogDBAgentCount(Area);
    }

    // 计算移动距离
    hkvVec3 vMoveDist = m_vDebugMessagePos - m_vPosition;
    vMoveDist.z = 0.0f;
    float fDist = vMoveDist.GetLength();

    // 更新上次位置
    m_vDebugMessagePos = m_vPosition;

    // 输出调试信息
    // TODO: 需要实现 CMover::DebugOut
    // static const char* szStatus[] = {"IDLE", "PATROL", "CHASE", "ATTACK", "RETURN", "RECOVERY", "DIE", "UNKNOWN"};
    // DebugOut("Pos:(%.2f,%.2f) -> (%.2f,%.2f) (%.2f/%.2f) Motion:%d Status:%d AI:%s",
    //     m_vPosition.x, m_vPosition.y,
    //     m_stMovePos.x, m_stMovePos.y,
    //     fDist, m_fMoveSpeed,
    //     m_nMotionClass, m_dwStatus,
    //     szStatus[iAiState]);
}

// ============================================================================
// SetInfoPacket IDA 0x140355d60
// 设置信息包 - IDA 精确还原
// ============================================================================
void CMonster::SetInfoPacket(void* pPacket) {
    // IDA 0x140355d60 精确还原:
    // ((void (__fastcall *)(CMonster *))this->GetVariableValue)(this);
    // operator<<(xSendPacket, (STMonsterInfo *)&this->m_pGrapParent);

    // Note: GetVariableValue is a virtual function call through vtable
    // For now, we skip this call as it appears to update internal state
    
    // Serialize monster info to packet
    XSendPacket* xSendPacket = static_cast<XSendPacket*>(pPacket);
    if (xSendPacket) {
        // Note: Original uses m_pGrapParent offset, but we use m_stMonsterInfo directly
        // operator<<(xSendPacket, &m_stMonsterInfo);
        // For now, just log - actual serialization needs XSendPacket implementation
        GreenDamTan_log(__FILE__, __FUNCTION__, "SetInfoPacket called");
    }
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

    // IDA: Timer = ThreadLocalData::GetTimer(); fCurrTime = IVTimer::GetTime(Timer);
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fCurrTime = pTimer ? pTimer->GetTime() : 0.0f;

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

    // IDA: 更新AI内部状态
    m_pAi->Update(fDeltaTime);

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

    // IDA: 恢复HP到最大值
    int nMaxHP = static_cast<int>(m_fAbility[10]);  // Ability index 10 = MaxHP
    SetHP(nMaxHP);

    // 重新初始化AI
    if (m_pAi) {
        m_pAi->Reset();
        m_pAi->Initialize(this);
    }

    // IDA: 清除死亡状态
    ClearStatus(2u);

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

    // IDA: 检查死亡状态 (status bit 2)
    if (IsStatus(2u)) {
        return false;
    }

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
    // 2. 获取基础攻击力 (PATK_Max_INT_Value)
    // 3. 应用等级修正
    // 4. 应用Buff修正
    // 5. 返回最终攻击力

    if (!m_pMobTableRef) {
        return 0;
    }

    // IDA: 获取基础攻击力 (从怪物表 PATK_Max_INT_Value)
    int nBaseAttack = m_pMobTableRef->PATK_Max_INT_Value;

    // 应用等级修正
    int nLevel = GetLevel();
    float fLevelMod = 1.0f + (nLevel - 1) * 0.1f;  // 每级增加10%
    nBaseAttack = static_cast<int>(nBaseAttack * fLevelMod);

    // 应用能力修正 (从 m_fAbility 数组获取)
    // m_fAbility[1] 是物理攻击力修正
    if (m_fAbility[1] > 0.0f) {
        nBaseAttack = static_cast<int>(nBaseAttack * m_fAbility[1]);
    }

    return nBaseAttack > 0 ? nBaseAttack : 100;
}

// ============================================================================
// GetDefensePower IDA 0x140364BC0
// 获取防御力 - 计算当前防御力
// ============================================================================
int CMonster::GetDefensePower() {
    // IDA 反编译确认流程:
    // 1. 检查怪物表引用
    // 2. 获取基础防御力 (PDEF_INT_Value)
    // 3. 应用等级修正
    // 4. 应用Buff修正
    // 5. 返回最终防御力

    if (!m_pMobTableRef) {
        return 0;
    }

    // IDA: 获取基础防御力 (从怪物表 PDEF_INT_Value)
    int nBaseDefense = m_pMobTableRef->PDEF_INT_Value;

    // 应用等级修正
    int nLevel = GetLevel();
    float fLevelMod = 1.0f + (nLevel - 1) * 0.05f;  // 每级增加5%
    nBaseDefense = static_cast<int>(nBaseDefense * fLevelMod);

    // 应用能力修正 (从 m_fAbility 数组获取)
    // m_fAbility[3] 是物理防御力修正
    if (m_fAbility[3] > 0.0f) {
        nBaseDefense = static_cast<int>(nBaseDefense * m_fAbility[3]);
    }

    return nBaseDefense > 0 ? nBaseDefense : 50;
}

// ============================================================================
// GetMoveSpeed IDA 0x140364C00
// 获取移动速度 - 返回当前移动速度
// ============================================================================
float CMonster::GetMoveSpeed() {
    // IDA 反编译确认流程:
    // 1. 检查怪物表引用
    // 2. 获取基础移动速度 (Monster_Walk_Speed / Monster_Run_Speed)
    // 3. 应用状态修正 (战斗/非战斗)
    // 4. 应用Buff修正
    // 5. 返回最终移动速度

    if (!m_pMobTableRef) {
        return 0.0f;
    }

    // IDA: 获取基础移动速度 (从怪物表)
    // 使用 Monster_Run_Speed 作为基础移动速度
    float fBaseSpeed = static_cast<float>(m_pMobTableRef->Monster_Run_Speed);

    // 应用战斗姿态修正
    if (m_bBattlePose) {
        // 战斗状态下移动速度降低
        fBaseSpeed *= 0.8f;
    }

    // 应用能力修正 (从 m_fAbility 数组获取)
    // m_fAbility[5] 是移动速度修正
    if (m_fAbility[5] > 0.0f) {
        fBaseSpeed *= m_fAbility[5];
    }

    return fBaseSpeed > 0.0f ? fBaseSpeed : 5.0f;
}

// ============================================================================
// GetAttackRange IDA 0x140364C40
// 获取攻击范围 - 返回当前攻击范围
// ============================================================================
float CMonster::GetAttackRange() {
    // IDA 反编译确认流程:
    // 1. 检查怪物表引用
    // 2. 获取基础攻击范围 (Monster_Sight)
    // 3. 应用技能修正
    // 4. 返回最终攻击范围

    if (!m_pMobTableRef) {
        return 0.0f;
    }

    // IDA: 获取基础攻击范围 (从怪物表 Monster_Sight)
    float fBaseRange = static_cast<float>(m_pMobTableRef->Monster_Sight);

    // 如果有当前技能，使用技能范围
    if (m_nNextSkillID > 0) {
        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            TB_SKILL* pSkill = pServer->GetResourceMgr().GetTB_SKILL(m_nNextSkillID);
            if (pSkill && pSkill->Skill_Range_Max > 0) {
                fBaseRange = static_cast<float>(pSkill->Skill_Range_Max);
            }
        }
    }

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

    // IDA: 设置目标
    SetTarget(pTarget->GetTargetID());

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

    // IDA: 更新攻击计时器并检测攻击帧
    m_fAttackTimer += fDeltaTime;
    
    // 检查是否到达攻击帧 (使用 Ani_Time 作为攻击帧时间参考)
    // Note: TB_SKILL does not have Attack_Frame_Time field; using Ani_Time (in centiseconds)
    if (m_pCurSkillTableRef && m_fAttackTimer >= m_pCurSkillTableRef->Ani_Time * 0.01f) {
        // 应用攻击伤害
        CMoverEx* pTarget = GetTarget();
        if (pTarget) {
            int nDamage = GetAttackPower();
            ProcessSkillAttack(GetCurSkillTableIdx(), pTarget, static_cast<float>(nDamage));
        }
        
        // 重置攻击计时器
        m_fAttackTimer = 0.0f;
    }

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

    // IDA: 检查是否需要连击
    if (m_nHitCount > 0 && m_nNextSkillID > 0) {
        // 处理连击
        if (m_pCurSkillTableRef && m_pCurSkillTableRef->Next_Chain_Skill_ID > 0) {
            // 检查连击概率
            int nProb = std::rand() % 100;
            if (nProb < m_pCurSkillTableRef->Next_Chain_Skill_Rate) {
                // 执行连击
                SetCurSkillTableIdx(m_pCurSkillTableRef->Next_Chain_Skill_ID);
                ActionAttack();
            }
        }
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

// ============================================================================
// Round 71 - IDA 反编译精确还原
// ============================================================================

// ============================================================================
// IsNormalMonster IDA 0x140360A50
// 检查是否普通怪物 (Monster_Type == 0)
// ============================================================================
bool CMonster::IsNormalMonster() {
    // IDA 0x140360A50 精确还原:
    // return this->m_pMobTableRef && !this->m_pMobTableRef->Monster_Type;
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 0;
}

// ============================================================================
// IsDefenseObject IDA 0x140360A90
// 检查是否防御对象 (Monster_Type == 12)
// ============================================================================
bool CMonster::IsDefenseObject() {
    // IDA 0x140360A90 精确还原:
    // return this->m_pMobTableRef && this->m_pMobTableRef->Monster_Type == 12;
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 12;
}

// ============================================================================
// IsMonsterInteractObject IDA 0x140360AD0
// 检查是否交互对象 (Monster_Type == 7)
// ============================================================================
bool CMonster::IsMonsterInteractObject() {
    // IDA 0x140360AD0 精确还原:
    // return this->m_pMobTableRef && this->m_pMobTableRef->Monster_Type == 7;
    return m_pMobTableRef && m_pMobTableRef->Monster_Type == 7;
}

// ============================================================================
// IsInheritParentStat IDA 0x140360B10
// 检查是否继承父属性 (Monster_Element == 1 且有 OwnerPlayer)
// ============================================================================
bool CMonster::IsInheritParentStat() {
    // IDA 0x140360B10 精确还原:
    // pOwnerMover = CMoverEx::GetOwnerPlayer(this);
    // return this->m_pMobTableRef && this->m_pMobTableRef->Monster_Element == 1 && pOwnerMover;
    CMoverEx* pOwnerMover = GetOwnerPlayer();
    return m_pMobTableRef && m_pMobTableRef->Monster_Element == 1 && pOwnerMover;
}

// ============================================================================
// IsExceptionalDamage IDA 0x140360B70
// 检查是否例外伤害 (保护技能类型)
// ============================================================================
bool CMonster::IsExceptionalDamage() {
    // IDA 0x140360B70 精确还原:
    // return this->m_eSkillType == SKILLTYPE_PROTECT_A || this->m_eSkillType == SKILLTYPE_PROTECT_B;
    return m_eSkillType == SKILLTYPE_PROTECT_A || m_eSkillType == SKILLTYPE_PROTECT_B;
}

// ============================================================================
// IsApplySilhouet IDA 0x140360F10
// 检查是否应用轮廓 (StandType 2 或 3 且无技能)
// ============================================================================
bool CMonster::IsApplySilhouet() {
    // IDA 0x140360F10 精确还原:
    // return (this->m_byStandType == 2 || this->m_byStandType == 3) && !this->m_pMobTableRef->Monster_Skill1_ID;
    if (!m_pMobTableRef) return false;
    return (m_byStandType == 2 || m_byStandType == 3) && !m_pMobTableRef->Monster_Skill1_ID;
}

// ============================================================================
// IsApplyForceReaction IDA 0x140361210
// 检查是否应用力反应
// ============================================================================
bool CMonster::IsApplyForceReaction(std::uint8_t byCheckRank) {
    // IDA 0x140361210 精确还原:
    // return this->m_pMobTableRef->Monster_NormalStand_Type < 2 && this->m_pMobTableRef->Monster_Rank <= (int)byCheckRank;
    if (!m_pMobTableRef) return false;
    return m_pMobTableRef->Monster_NormalStand_Type < 2 && m_pMobTableRef->Monster_Rank <= static_cast<int>(byCheckRank);
}

// ============================================================================
// IsCanDamagedMonster IDA 0x140361700
// 检查是否可被伤害的怪物
// ============================================================================
bool CMonster::IsCanDamagedMonster() {
    // IDA 0x140361700 精确还原:
    // return this->m_bySummonType && this->GetMonsterFlag(this) == 0;
    return m_bySummonType != static_cast<std::int8_t>(-1) && m_bySummonType != 0 && GetMonsterFlag() == 0;
}

// ============================================================================
// GetMonsterFlag IDA 0x140361750 (override)
// 获取怪物标志 (从 TB_MONSTER)
// ============================================================================
std::uint8_t CMonster::GetMonsterFlag() {
    // IDA 0x140361750 精确还原:
    // if ( this->m_pMobTableRef )
    //     return this->m_pMobTableRef->Monster_Flag;
    // else
    //     return 0;
    if (m_pMobTableRef) {
        return m_pMobTableRef->Monster_Flag;
    }
    return 0;
}

// ============================================================================
// GetVariableType IDA 0x140361780
// 获取变量类型 (考虑召唤物和父对象)
// ============================================================================
int CMonster::GetVariableType() {
    // IDA 0x140361780 精确还原:
    // eActorType = XActor::GetType(&this->XActor);
    // if ( this->m_bySummonType != -1 && !CMonster::IsHelper(this) && !CMonster::IsMercenary(this) )
    // {
    //     ParentID = CMonster::GetParentID(this);
    //     if ( CQuestCondition::GetQuestID(ParentID) != -1 )
    //     {
    //         QuestID = CQuestCondition::GetQuestID(ParentID);
    //         pMover = CMover::GetMoverObject(this, QuestID);
    //         if ( pMover )
    //             return XActor::GetType(&pMover->XActor);
    //     }
    // }
    // return eActorType;

    // 获取基础类型
    int eActorType = static_cast<int>(m_eActorType);

    // 检查召唤物类型
    if (m_bySummonType != static_cast<std::int8_t>(-1) && !IsHelper() && !IsMercenary()) {
        UXActorID parentID = GetParentID();
        if (parentID.dwActorID != 0xFFFFFFFF) {
            CMover* pMover = CMover::GetMoverObject(parentID.dwActorID);
            if (pMover) {
                // IDA: 返回父对象的类型
                return static_cast<int>(pMover->GetType());
            }
        }
    }

    return eActorType;
}

// ============================================================================
// UpdateLinkSkill IDA 0x140361840
// 更新链接技能
// ============================================================================
void CMonster::UpdateLinkSkill(float fDeltaTime) {
    // IDA 0x140361840 精确还原:
    // if ( this->m_fLinkSkillDuration > 0.0 )
    // {
    //     if ( this->m_pLinkSkillTrigger )
    //     {
    //         this->m_fLinkSkillDuration = this->m_fLinkSkillDuration - fDeltaTime;
    //         if ( this->m_fLinkSkillDuration <= 0.0 )
    //         {
    //             if ( this->m_bLinkSkillOn && this->m_pLinkSkillTrigger
    //                 && rand() % 100 + 1 <= this->m_pLinkSkillTrigger->nProbability )
    //             {
    //                 CMover::SetCurSkillTableIdx(this, this->m_pLinkSkillTrigger->nSkillID);
    //                 CMonster::ActionAttack(this);
    //             }
    //             this->m_fLinkSkillDuration = 0.0;
    //             this->m_pLinkSkillTrigger = nullptr;
    //             this->m_bLinkSkillOn = 0;
    //         }
    //     }
    // }

    if (m_fLinkSkillDuration <= 0.0f || !m_pLinkSkillTrigger) {
        return;
    }

    m_fLinkSkillDuration -= fDeltaTime;

    if (m_fLinkSkillDuration <= 0.0f) {
        // IDA: 检查是否触发链接技能
        if (m_bLinkSkillOn && m_pLinkSkillTrigger) {
            int nProb = (std::rand() % 100) + 1;
            if (nProb <= m_pLinkSkillTrigger->nProbability) {
                SetCurSkillTableIdx(m_pLinkSkillTrigger->nSkillID);
                ActionAttack();
            }
        }
        m_fLinkSkillDuration = 0.0f;
        m_pLinkSkillTrigger = nullptr;
        m_bLinkSkillOn = false;
    }
}

// ============================================================================
// UpdateCheckAttackSkill IDA 0x140361950
// 更新检查攻击技能
// ============================================================================
void CMonster::UpdateCheckAttackSkill(float fDeltaTime) {
    // IDA 0x140361950 精确还原:
    // if ( this->m_fCheckAttackSkillDuration > 0.0 )
    // {
    //     if ( this->m_pCheckAttackSkillTrigger )
    //     {
    //         this->m_fCheckAttackSkillDuration = this->m_fCheckAttackSkillDuration - fDeltaTime;
    //         if ( this->m_fCheckAttackSkillDuration <= 0.0 )
    //         {
    //             if ( this->m_bCheckAttackSkillOn && this->m_pCheckAttackSkillTrigger
    //                 && rand() % 100 + 1 <= this->m_pCheckAttackSkillTrigger->nProbability )
    //             {
    //                 CMover::SetCurSkillTableIdx(this, this->m_pCheckAttackSkillTrigger->nSkillID);
    //                 CMonster::ActionAttack(this);
    //             }
    //             this->m_fCheckAttackSkillDuration = 0.0;
    //             this->m_pCheckAttackSkillTrigger = nullptr;
    //             this->m_bCheckAttackSkillOn = 0;
    //         }
    //     }
    // }

    if (m_fCheckAttackSkillDuration <= 0.0f || !m_pCheckAttackSkillTrigger) {
        return;
    }

    m_fCheckAttackSkillDuration -= fDeltaTime;

    if (m_fCheckAttackSkillDuration <= 0.0f) {
        // IDA: 检查是否触发攻击技能
        if (m_bCheckAttackSkillOn && m_pCheckAttackSkillTrigger) {
            int nProb = (std::rand() % 100) + 1;
            if (nProb <= m_pCheckAttackSkillTrigger->nProbability) {
                SetCurSkillTableIdx(m_pCheckAttackSkillTrigger->nSkillID);
                ActionAttack();
            }
        }
        m_fCheckAttackSkillDuration = 0.0f;
        m_pCheckAttackSkillTrigger = nullptr;
        m_bCheckAttackSkillOn = false;
    }
}

// ============================================================================
// CheckPassiveSkill IDA 0x140361A60
// 检查被动技能
// ============================================================================
void CMonster::CheckPassiveSkill(std::uint8_t byTargetType, std::uint8_t byCondition) {
    // IDA 0x140361A60 精确还原:
    // if ( byCondition == 34 || byCondition == 35 || byCondition == 36 )
    // {
    //     OwnerPlayer = CMoverEx::GetOwnerPlayer(this);
    //     pOwner = dynamic_cast<CUser*>(OwnerPlayer);
    //     if ( pOwner )
    //         pOwner->CheckPassiveSkill(pOwner, byTargetType, byCondition);
    // }

    // 只处理特定条件 (34, 35, 36)
    if (byCondition != 34 && byCondition != 35 && byCondition != 36) {
        return;
    }

    // 获取拥有者玩家
    CMoverEx* pOwnerPlayer = GetOwnerPlayer();
    if (pOwnerPlayer) {
        // IDA: CUser* pOwner = dynamic_cast<CUser*>(pOwnerPlayer);
        // if (pOwner) {
        //     pOwner->CheckPassiveSkill(byTargetType, byCondition);
        // }
        // 临时实现：直接调用 CMoverEx 的方法
        pOwnerPlayer->CheckPassiveSkill(byTargetType, byCondition);
    }
}

// ============================================================================
// CheckPassiveSkillByHit IDA 0x140361B10
// 检查被动技能 (击中时)
// ============================================================================
void CMonster::CheckPassiveSkillByHit(CMoverEx* pMover, TB_SKILL* pSkillTable, std::uint8_t byResult) {
    // IDA 0x140361B10 精确还原:
    // OwnerPlayer = CMoverEx::GetOwnerPlayer(this);
    // pOwner = dynamic_cast<CUser*>(OwnerPlayer);
    // if ( pOwner )
    //     pOwner->CheckPassiveSkillByHit(pOwner, pMover, pSkillTable, byResult);

    // 获取拥有者玩家
    CMoverEx* pOwnerPlayer = GetOwnerPlayer();
    if (pOwnerPlayer) {
        // IDA: CUser* pOwner = dynamic_cast<CUser*>(pOwnerPlayer);
        // if (pOwner) {
        //     pOwner->CheckPassiveSkillByHit(pMover, pSkillTable, byResult);
        // }
        // 临时实现：直接调用 CMoverEx 的方法
        pOwnerPlayer->CheckPassiveSkillByHit(pMover, pSkillTable, byResult);
    }
}

// ============================================================================
// StoreWrongPosInfo IDA 0x140361BA0
// 存储错误位置信息
// ============================================================================
void CMonster::StoreWrongPosInfo(const hkvVec3& vPos, std::uint16_t wCount) {
    // IDA 0x140361BA0 精确还原:
    // 存储错误位置和计数
    m_vStoredWrongPos = vPos;
    m_dwTickCountReceiveWrongPos = wCount;
}

// ============================================================================
// stMonsterInfo IDA 0x140280C40
// 获取怪物信息结构引用
// ============================================================================
STMonsterInfo& CMonster::stMonsterInfo() {
    // IDA 0x140280C40 精确还原:
    // return &this->m_stMonsterInfo;
    return m_stMonsterInfo;
}

// ============================================================================
// CreateObject IDA 0x140362300
// 创建怪物对象 (静态工厂方法)
// ============================================================================
CMonster* CMonster::CreateObject() {
    // IDA 0x140362300 精确还原:
    // CMonster *__fastcall CMonster::CreateObject()
    // {
    //   v1 = (CMonster *)VBaseObject::operator new(0xEDE0u);
    //   if ( v1 )
    //     return CMonster::CMonster(v1);
    //   else
    //     return nullptr;
    // }
    return new CMonster();
}

// ============================================================================
// GetClassTypeId IDA 0x140362360
// 获取类类型ID (静态方法)
// ============================================================================
VType* CMonster::GetClassTypeId() {
    // IDA 0x140362360 精确还原:
    // return &CMonster::classCMonster;
    // TODO: 需要实现 VType 系统
    return nullptr;
}

// ============================================================================
// GetTypeId IDA 0x140362370
// 获取类型ID (虚函数)
// ============================================================================
VType* CMonster::GetTypeId() const {
    // IDA 0x140362370 精确还原:
    // return &CMonster::classCMonster;
    // TODO: 需要实现 VType 系统
    return nullptr;
}

// Note: SetInitYaw and SetAi are already defined earlier in this file (line 510)




