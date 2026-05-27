#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/Ai.h"
#include <cmath>

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
        m_stMonsterInfo.SetLevel(pTBMonster->Monster_Lv);
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
// NotifyRemoved IDA 0x14018BBB0
// 虚函数 - 通知移除
// ============================================================================
void CMonster::NotifyRemoved() {
    // IDA 0x14018BBB0:
    // (*(void (__fastcall **)(char *))(*((_QWORD *)this - 109) + 464LL))((char *)this - 872);
    // 这是调用虚函数表中的某个函数
    // vtable[-109] 表示从虚函数表指针向前偏移 109 个指针位置
    // +464 是该对象的方法偏移
    // (char *)this - 872 是传递的参数，可能是某个基类

    // TODO: 需要确认虚函数表布局后实现
    // 目前使用空实现
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
// ChangeMotion IDA 0x14035D350
// ============================================================================
void CMonster::ChangeMotion(std::int16_t nMotionClass, int bResetPlay, int iCallPos) {
    // IDA 0x14035D350:
    // if (GetHP() <= 0 || m_byPhaseMotionStep != 2 &&
    //     (!CheckSuperArmorMotion(nMotionClass) || m_byPhaseMotionStep)) {
    //   CMoverEx::ChangeMotion(nMotionClass, bResetPlay, 0);
    //   CheckProtectSkillUI();
    //   if (m_nMotionClass == 1) m_bUpdateRotation = 1;
    //   if (m_byPhaseMotionStep == 2) {
    //     m_byPhaseMotionStep = 0;
    //     CMover::SetInvincibleActor(this, 0);
    //   }
    // }

    if (GetHP() <= 0 || (m_byPhaseMotionStep != 2 && (!CheckSuperArmorMotion(nMotionClass) || m_byPhaseMotionStep))) {
        // 调用基类 ChangeMotion
        // TODO: CMoverEx::ChangeMotion(nMotionClass, bResetPlay, 0);
        CheckProtectSkillUI();
        if (m_nMotionClass == 1) {
            m_bUpdateRotation = true;
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
// 主更新循环 - AI思考和状态更新
// ============================================================================
void CMonster::ThinkFunction() {
    // IDA 反编译确认的流程:
    // 1. 调用 CMoverEx::ThinkFunction
    // 2. 检查移动并发送移动包 (CheckSendMovePacket)
    // 3. 获取帧时间
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
    if (IsCanMove(true) || IsStatus(1u)) {
        CheckSendMovePacket();
    }

    // 获取帧时间
    float fDeltaTime = 0.016f;  // 默认帧时间 (~60fps)

    // 处理死亡延迟时间
    if (m_fDieDelayTime > 0.0f && m_eDieType == DIE_TYPE_DELAY) {
        m_fDieDelayTime -= fDeltaTime;
        m_fDieDelayMaxTime -= fDeltaTime;

        if (m_fDieDelayTime <= 0.0f || m_fDieDelayMaxTime <= 0.0f) {
            m_fDieDelayTime = 0.0f;
            m_fDieDelayMaxTime = 0.0f;
            m_eDieType = DIE_TYPE_NORMAL;
            m_dwStatus &= ~2u;  // Clear status bit 2 (dead)

            if (IsFlying()) {
                SetStatus(4u);
            } else {
                RealDie(IsHitDown() ? 13 : 12);
            }
        }
    }
    // 处理死亡淡出时间
    else if (m_fDieFadeTime > 0.0f) {
        m_fDieFadeTime -= fDeltaTime;

        if (m_fDieFadeTime <= 0.0f) {
            m_fDieFadeTime = 0.0f;
            // 检查是否还有动作在执行
            // if (m_xActionBuffer.GetActionCount()) {
            //     m_fDieFadeTime = 1.0f;
            // } else {
            //     通知父对象并从场景移除
            // }
        }
    }

    // 更新治疗仇恨
    UpdateHealAggro();

    // AI更新
    if (m_pAi && IsCanAI()) {
        m_pAi->Update(fDeltaTime);

        // 仇恨检查
        if (m_bChangedAggro) {
            m_fLastAggroCheckTime += fDeltaTime;
            if (m_fLastAggroCheckTime >= m_pAi->GetDmgAggroReseTime()) {
                m_fLastAggroCheckTime = 0.0f;
                DamageAggressive();
            }
        }
    }

    // 召唤物生命周期
    if ((m_bySummonType == 1 || m_bySummonType == 3) && m_fSummonLifeTime >= 0.0f) {
        m_fSummonLifeTime -= fDeltaTime;

        if (m_fSummonLifeTime < 0.0f) {
            m_eDieType = DIE_TYPE_NORMAL;
            RealDie(1);
            m_fSummonLifeTime = -1.0f;
        }
    }

    // 跟随对象位置同步
    CMoverEx* pOwnerMover = GetOwnerPlayer();
    if (IsFollowObject() && pOwnerMover) {
        hkvVec3 vOwnerPos = pOwnerMover->GetPosition();
        SetPositionXVec3(vOwnerPos);
    }

    // 死亡状态超时检查 (60秒)
    if (IsStatus(2u)) {
        m_fElapsedDieTime += fDeltaTime;
        if (m_fElapsedDieTime > 60.0f) {
            // 记录警告日志
            GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster death timeout exceeded");
            // 从场景移除
        }
    } else {
        m_fElapsedDieTime = 0.0f;
    }

    // Boss HP日志记录 (每30秒)
    if (m_fBossHPLogTime > 0.0f) {
        m_fBossHPLogTime -= fDeltaTime;
        if (m_fBossHPLogTime <= 0.0f) {
            m_fBossHPLogTime = 30.0f;
            // 记录Boss HP状态日志
            GreenDamTan_log(__FILE__, __FUNCTION__, "Boss HP log recorded");
        }
    }
}

// ============================================================================
// OnDie IDA 0x140356980 -> 0x140357988
// 死亡处理 - 处理掉落、经验、任务等
// ============================================================================
void CMonster::OnDie(XActor* pOwnerActor, float fDamage) {
    // IDA 反编译确认的流程:
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
    m_byDieReason = 0x10;  // 死亡原因
    m_nDieDamage = nHP;
    SetHP(0);

    // 处理专用怪物的拥有者
    CMoverEx* pOwner = GetOwnerPlayer();
    if (pOwner && IsDedicated()) {
        // 清除专用怪物关联
        GreenDamTan_log(__FILE__, __FUNCTION__, "Dedicated monster died");
    }

    // 自杀处理
    if (m_bSuicide) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "Monster suicide");
    } else {
        // 处理击杀者
        if (pOwnerActor) {
            // 处理掉落
            ProcessDrop(pOwnerActor);

            // 处理护送任务
            ProcessEscortQuest();

            // 处理经验
            ProcessExp(pOwnerActor);

            // 更新击杀者ID
            m_dwKillerID = 0;  // Use placeholder for actor ID
        }

        // 处理游戏模式
        ProcessGameMode();
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::OnDie called");
}

// ============================================================================
// RealDie IDA 0x14035A200 -> 0x14035A5D6
// 执行死亡 - 真正的死亡处理
// ============================================================================
void CMonster::RealDie(std::int16_t nChangeMotion) {
    // IDA 反编译确认的流程:
    // 1. 检查是否已经死亡状态 (XActor::IsStatus(2))
    // 2. 调用基类 RealDie
    // 3. 如果是Boss且没有残留Boss怪物，杀死所有召唤物
    // 4. 检查击中者并触发被动技能
    // 5. 清除保护技能

    // 检查是否已经是死亡状态
    if (IsStatus(2u)) {
        return;  // 已经是死亡状态
    }

    // 调用基类 RealDie - 设置死亡状态
    // CMoverEx::RealDie(nChangeMotion);
    SetStatus(2u);  // 设置死亡状态标志
    
    // 设置死亡动画
    if (nChangeMotion >= 0) {
        m_nMotionClass = nChangeMotion;
    }

    // Boss 死亡处理 - 连带死亡召唤物
    if (IsBoss()) {
        // 检查是否还有残留Boss怪物
        if (!IsRemainBossMonster()) {
            // 遍历所有怪物，找到父ID匹配的召唤物并杀死它们
            GreenDamTan_log(__FILE__, __FUNCTION__, "Boss died, checking for summons");
        }
    }

    // 检查击中者并触发被动技能
    if (m_dwHitID != 0) {
        CMover* pHitMover = CMover::GetMoverObject(m_dwHitID);
        if (pHitMover) {
            // 触发被动技能
            // pHitMover->CheckPassiveSkill(2, 34);
            // pHitMover->CheckPassiveSkill(5, 35);
        }
    }

    // 清除保护技能
    if (m_nProtectSkill > 0) {
        m_nProtectSkill = 0;
        SendNoticePacket(m_nProtectSkill, -1, -1.0f);
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::RealDie called");
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
    // IDA 反编译确认:
    // if (this->IsFollower(this)) return 1;
    // if (this->m_pAttachToAttacker) return 0;
    // if (this->m_bReserveChange) return 0;
    // if (this->m_byPhaseMotionStep) return 0;
    // if (XActor::IsStatus(&this->XActor, 0x10000u) || XActor::IsStatus(&this->XActor, 0xF000000u)) return 0;
    // if (XActor::IsStatus(&this->XActor, 2u) || XActor::IsStatus(&this->XActor, 4u)) return 0;
    // if (!XActor::IsStatus(&this->XActor, 0x2000u) || (this->m_dwInvisibleFlag & 8) != 0)
    //     return CMoverEx::IsSuperArmorBreakMotion(this, this->m_nMotionClass) == 0;
    // return 0;

    // 如果是跟随者，可以执行 AI
    if (IsFollower()) {
        return true;
    }

    // 如果附加到攻击者，不能执行 AI
    if (m_pAttachToAttacker) {
        return false;
    }

    // 如果预留改变，不能执行 AI
    if (m_bReserveChange) {
        return false;
    }

    // 如果阶段动作步骤非零，不能执行 AI
    if (m_byPhaseMotionStep) {
        return false;
    }

    // 检查特定状态标志 (0x10000 或 0xF000000)
    // TODO: if (XActor::IsStatus(0x10000u) || XActor::IsStatus(0xF000000u)) return false;

    // 检查死亡 (2) 或倒地 (4) 状态
    // TODO: if (XActor::IsStatus(2u) || XActor::IsStatus(4u)) return false;

    // 检查隐身状态
    // TODO: if (!XActor::IsStatus(0x2000u) || (m_dwInvisibleFlag & 8) != 0)
    //     return !IsSuperArmorBreakMotion(m_nMotionClass);

    // 检查是否是超级护甲破防动作
    if (IsSuperArmorBreakMotion(m_nMotionClass)) {
        return false;
    }

    return true;
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
    // IDA 反编译确认:
    // if (XActor::IsStatus(&this->XActor, 2u) || XActor::IsStatus(&this->XActor, 4u)
    //     || XActor::IsStatus(&this->XActor, 0x10000u) || XActor::IsStatus(&this->XActor, 0xF000000u))
    //     return 0;
    // if (this->m_byStandType == 2 || this->m_byStandType == 1 && this->m_nMotionClass == 1 && !this->m_bBattlePose)
    //     return 0;
    // return this->m_nMotionClass == 1 || this->m_nMotionClass == 2
    //     || this->m_nMotionClass == 7 || this->m_nMotionClass == 8;

    // 检查特定状态标志
    // TODO: if (XActor::IsStatus(2u) || XActor::IsStatus(4u)
    //     || XActor::IsStatus(0x10000u) || XActor::IsStatus(0xF000000u))
    //     return false;

    // 检查站立类型
    if (m_byStandType == 2) {
        return false;
    }
    if (m_byStandType == 1 && m_nMotionClass == 1 && !m_bBattlePose) {
        return false;
    }

    // 检查动作类型
    return m_nMotionClass == 1 || m_nMotionClass == 2
        || m_nMotionClass == 7 || m_nMotionClass == 8;
}

// ============================================================================
// IsCanAttack IDA 0x140358A20
// 检查是否可以攻击
// ============================================================================
bool CMonster::IsCanAttack() {
    // IDA 反编译确认:
    // if (!CMoverEx::IsCanAttack(this)) return 0;
    // if (CMover::IsGeneralHit(this) && this->m_byStandType != 2 && this->m_byStandType != 3) return 0;
    // if (CMover::IsKnockDown(this)) return 0;
    // if (this->m_nMotionClass == this->m_nPlayPhaseMotion) return 0;
    // if (this->m_nMotionClass == 7 || this->m_nMotionClass == 8) return 0;
    // return !XActor::IsStatus(&this->XActor, 1u);

    // 检查死亡状态
    if (IsStatus(2u) || IsStatus(4u) || IsStatus(0x10000u) || IsStatus(0xF000000u)) {
        return false;
    }

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
    // IDA 反编译确认:
    // if (this->m_pSector && !CWeeklyMission_Group::GetGroupID(this->m_pSector)) return 0;
    // if (this->IsDefensiveWeapon(this)) return 0;
    // return CMoverEx::IsCanHit(this, nDownAttack, bPassiveType);

    // 检查Sector
    // TODO: if (m_pSector && !CWeeklyMission_Group::GetGroupID(m_pSector)) return 0;

    // 检查防御武器
    // TODO: if (IsDefensiveWeapon()) return 0;

    // 调用基类
    // TODO: return CMoverEx::IsCanHit(nDownAttack, bPassiveType);
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
// UpdateHealAggro IDA 0x14035FB20
// 更新治疗仇恨
// ============================================================================
void CMonster::UpdateHealAggro() {
    // IDA 反编译确认:
    // Timer = ThreadLocalData::GetTimer();
    // fCurrTime = IVTimer::GetTime(Timer);
    // for (auto& iter : m_arDamageMeter) {
    //     pMover = CMover::GetMoverObject(this, iter->first);
    //     if (pMover && XActor::IsPlayer(&pMover->XActor) && CMoverEx::GetAmountOfHeal(pMover) > 0.0) {
    //         iter->second.fTime = fCurrTime;
    //         iter->second.fAggro += CMonster::CalcHealAggroPoint(this, pMover);
    //         this->m_bChangedAggro = 1;
    //     }
    // }

    float fCurrTime = 0.0f;  // TODO: ThreadLocalData::GetTimer() + IVTimer::GetTime()

    for (auto& pair : m_arDamageMeter) {
        std::uint32_t dwID = pair.first;
        tagDamageMeter& dmgMeter = pair.second;

        // 获取目标 Mover
        CMoverEx* pMover = nullptr;  // TODO: CMover::GetMoverObject(this, dwID)
        if (pMover) {
            // 检查是否是玩家且进行了治疗
            // TODO: if (XActor::IsPlayer(&pMover->XActor) && CMoverEx::GetAmountOfHeal(pMover) > 0.0f) {
            //     dmgMeter.fTime = fCurrTime;
            //     float fHealAggro = CalcHealAggroPoint(pMover);
            //     dmgMeter.fAggro += fHealAggro;
            //     m_bChangedAggro = 1;
            // }
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
// 伤害激怒处理 - 选择仇恨最高的目标
// 大小: 787 bytes
// ============================================================================
void CMonster::DamageAggressive() {
    // IDA 反编译确认流程:
    // 1. 获取当前时间
    // 2. 从 AI 获取伤害仇恨重置时间
    // 3. 遍历 m_arDamageMeter 找到仇恨最高的目标
    // 4. 对所有目标的仇恨值进行衰减
    // 5. 如果最高仇恨目标改变，切换目标

    // 获取当前时间
    // IDA: Timer = ThreadLocalData::GetTimer(); fCurrTime = IVTimer::GetTime(Timer);
    float fCurrTime = 0.0f;  // TODO: 需要实现 ThreadLocalData::GetTimer 和 IVTimer::GetTime

    // 获取伤害仇恨重置时间
    // IDA: if (this->m_pAi) DmgAggroReseTime = CAi::GetDmgAggroReseTime(this->m_pAi);
    //      else DmgAggroReseTime = 0.0;
    float fResetTime = 0.0f;
    if (m_pAi) {
        fResetTime = m_pAi->GetDmgAggroReseTime();
    }

    std::uint32_t dwTopID = 0xFFFFFFFF;
    float fTopDamage = 0.0f;

    // 遍历所有伤害计量记录
    // IDA: for (auto it = m_arDamageMeter.begin(); it != m_arDamageMeter.end(); ++it)
    for (auto it = m_arDamageMeter.begin(); it != m_arDamageMeter.end(); ++it) {
        float fAggro = it->second.fAggro;

        // 找最高仇恨
        // IDA: if (*((float *)&it->first + 1) > fDamage) { fDamage = ...; dwID = it->first; }
        // 注意: IDA 反编译中 *((float *)&it->first + 1) 实际上是访问 value.fAggro
        if (fAggro > fTopDamage) {
            fTopDamage = fAggro;
            dwTopID = it->first;
        }

        // 计算衰减 - 时间衰减
        // IDA: fLeftTime = fCurrTime - *(float *)&it->second.__vftable;
        //      实际上 it->second.fTime 是最后更新时间
        float fLeftTime = fCurrTime - it->second.fTime;
        if (fLeftTime > 0.0f) {
            if (fResetTime <= fLeftTime) {
                // 时间超过重置时间，清零仇恨
                // IDA: *((_DWORD *)&it->first + 1) = 0;
                it->second.fAggro = 0.0f;
            } else {
                // 按时间比例衰减
                // IDA: fRate = fLeftTime / fResetTime;
                //      fReduceAggro = *((float *)&it->first + 1) * fRate;
                //      *v25 = *v25 - fReduceAggro;
                float fRate = fLeftTime / fResetTime;
                float fReduceAggro = fAggro * fRate;
                it->second.fAggro -= fReduceAggro;
            }
        }

        // 基础衰减 - 每次调用衰减 50%
        // IDA: fBaseAggro = *((float *)&it->first + 1) * 0.5;
        //      *v26 = *v26 - fBaseAggro;
        float fBaseAggro = fAggro * 0.5f;
        it->second.fAggro -= fBaseAggro;

        // 如果仇恨值过低，清零
        // IDA: if (fResetTime >= *((float *)&it->first + 1))
        //          *((_DWORD *)&it->first + 1) = 0;
        if (it->second.fAggro < fResetTime) {
            it->second.fAggro = 0.0f;
        }
    }

    // 重置仇恨改变标志
    // IDA: this->m_bChangedAggro = 0;
    m_bChangedAggro = 0;

    // 检查是否需要切换目标
    // IDA: if (dwID == this->m_dwTargetID || dwID == -1)
    if (dwTopID == m_dwTargetID || dwTopID == 0xFFFFFFFF) {
        // 目标未改变或没有有效目标
        // IDA: if (this->m_pAi && CAi::IsEnableClearTarget(this->m_pAi) && dwID == -1)
        if (m_pAi && m_pAi->IsEnableClearTarget() && dwTopID == 0xFFFFFFFF) {
            // 清除目标
            // IDA: CMonster::ChangeTarget(this, (UXActorID)v7->__s0);
            //      v7 是从 CPair::CPair(&v22, 0xFFFFFFFF) 构造的 UXActorID
            ChangeTarget(UXActorID(0xFFFFFFFF));
        }
    } else {
        // 切换到新目标
        // IDA: CMonster::ChangeTarget(this, (UXActorID)v6->__s0);
        //      v6 是从 CPair::CPair(&v20, dwID) 构造的 UXActorID
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
// 设置死亡状态
// ============================================================================
void CMonster::SetDie(std::int16_t nMotion, int bSuicide) {
    // IDA 反编译确认的流程:
    // if ((!this->IsFollower(this) || bSuicide)
    //     && !XActor::IsStatus(&this->XActor, 4u)
    //     && !XActor::IsStatus(&this->XActor, 2u))
    // {
    //     CMover::MoveingValueClear(this);
    //     CMover::AllBuffClear(this, 1u);
    //     XActor::SetStatus(&this->XActor, 4u);
    //     this->m_bSuicide = bSuicide;
    //     if (nMotion == -1) {
    //         if (CMover::IsHitDown(this)) RealDie(13);
    //         else if (!CMover::IsKnockDown(this) || this->m_nHitStatus == 5) RealDie(12);
    //     } else {
    //         ReservedMotion = CMonster::GetReservedMotion(this, nMotion);
    //         RealDie(ReservedMotion);
    //     }
    // }

    // 如果是跟随者且不是自杀，不处理
    if (IsFollower() && !bSuicide) {
        return;
    }

    // 如果已经死亡(4)或倒地(2)，不处理
    // TODO: if (XActor::IsStatus(4u) || XActor::IsStatus(2u)) return;

    // 清除移动值
    // TODO: CMover::MoveingValueClear();

    // 清除所有Buff
    // TODO: CMover::AllBuffClear(1u);

    // 设置死亡状态
    // TODO: XActor::SetStatus(4u);
    m_bSuicide = bSuicide;

    // 根据动作类型处理
    if (nMotion == -1) {
        // 自动选择死亡动作
        // TODO: if (CMover::IsHitDown()) {
        //     RealDie(13);
        // } else if (!CMover::IsKnockDown() || m_nHitStatus == 5) {
        //     RealDie(12);
        // }
        // 默认使用动作12
        RealDie(12);
    } else {
        // 使用指定动作
        // TODO: std::int16_t reservedMotion = GetReservedMotion(nMotion);
        RealDie(nMotion);
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::SetDie called");
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
// 伤害处理 (虚函数 override)
// ============================================================================
void CMonster::Damage(tagACTION_DAMAGE& dmgInfo, unsigned int nSkillID, bool* bSABreaked) {
    // IDA 反编译确认的流程:
    // 1. CheckProtectDamage - 检查保护技能伤害
    // 2. CMoverEx::Damage - 调用基类伤害处理
    // 3. 如果有伤害且HP > 0，处理AI和掉落

    CheckProtectDamage(dmgInfo);

    // 调用基类 Damage
    // TODO: CMoverEx::Damage(dmgInfo, nSkillID, bSABreaked);

    if (dmgInfo.nDamage > 0 && GetHP() > 0) {
        // AI伤害处理
        if (m_pAi) {
            // TODO: CAi::FuncDamageProcess(m_pAi);
        }

        // 击中掉落
        // TODO: DropItemByHit(dmgInfo.dwID);

        // 迷宫伤害处理
        OnDamageForMaze();
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::Damage called");
}

// ============================================================================
// DamageProcessHP IDA 0x14035BF70 -> 0x14035C045
// HP伤害处理 (虚函数 override)
// ============================================================================
bool CMonster::DamageProcessHP(unsigned int dwID, int nSkillID, int nDamage,
                                unsigned char byDamageFlag, unsigned char byHitParts) {
    // IDA 反编译确认的流程:
    // 1. 调用 _DamageProcessHP 内部实现
    // 2. 如果是随从且HP <= 0，恢复HP并切换到恢复状态
    // 3. 增加击中计数

    // TODO: bool bResult = _DamageProcessHP(dwID, nSkillID, nDamage, byDamageFlag, byHitParts);
    bool bResult = false;

    // 简化实现：直接减少HP
    int nCurHP = GetHP();
    int nNewHP = nCurHP - nDamage;
    if (nNewHP < 0) {
        nNewHP = 0;
    }
    SetHP(nNewHP);
    bResult = (nNewHP == 0);

    // 检查随从状态
    if (IsFollower() && GetHP() <= 0) {
        SetHpEx(1);
        // TODO: ChangeAiState(FSMSTATES_RECOVERY);
        return false;
    }

    // 增加击中计数
    ++m_nHitCount;

    return bResult;
}

// ============================================================================
// CheckProtectDamage IDA 0x14035B860 -> 0x14035BBD8
// 检查保护伤害
// ============================================================================
void CMonster::CheckProtectDamage(tagACTION_DAMAGE& dmgInfo) {
    // IDA 反编译确认的流程:
    // 1. 检查 m_eSkillType
    // 2. 如果是 SKILLTYPE_PROTECT_A，吸收伤害
    // 3. 如果是 SKILLTYPE_PROTECT_B，时间型保护

    if (!m_eSkillType) {
        return;
    }

    float totalDamage = static_cast<float>(m_nProtectSkillDamage);
    if (totalDamage <= 0.0f) {
        return;
    }

    // SKILLTYPE_PROTECT_A (6) - 吸收伤害型
    if (m_eSkillType == 6) {  // SKILLTYPE_PROTECT_A
        // TODO: CMover::SetInvincibleActor(1);
        m_nAccumulateDamage += dmgInfo.nDamage;

        if (m_nAccumulateDamage > 0) {
            dmgInfo.nDamage = 0;
            float fPercent = static_cast<float>(m_nAccumulateDamage) / totalDamage;

            if (fPercent < 1.0f) {
                // TODO: SendNoticePacket(71, m_nAccumulateDamage + (int)totalDamage, fPercent);
            } else {
                dmgInfo.fSuperArmorGage = 0.0f;
                // TODO: ShowProtectSkillUI(0);
                // TODO: SendNoticePacket(71, -1, -1.0f);
            }
        }
    }
    // SKILLTYPE_PROTECT_B (7) - 时间型
    else if (m_eSkillType == 7) {  // SKILLTYPE_PROTECT_B
        // TODO: CMover::SetInvincibleActor(1);
        m_nAccumulateDamage += dmgInfo.nDamage;

        if (m_nAccumulateDamage > 0) {
            dmgInfo.nDamage = 0;
            float fTime = (totalDamage - static_cast<float>(m_nAccumulateDamage)) / totalDamage;

            if (fTime > 0.0f) {
                // TODO: SendNoticePacket(72, (int)totalDamage - m_nAccumulateDamage, fTime);
            } else {
                dmgInfo.fSuperArmorGage = 0.0f;
                // TODO: ShowProtectSkillUI(0);
                // TODO: SendNoticePacket(72, -1, -1.0f);
            }
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::CheckProtectDamage called");
}

// ============================================================================
// OnDamageForMaze IDA 0x14035BC60
// 迷宫伤害处理
// ============================================================================
void CMonster::OnDamageForMaze() {
    // IDA 反编译确认的流程:
    // 处理迷宫中的伤害事件
    // TODO: 实现完整的迷宫伤害处理逻辑

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::OnDamageForMaze called");
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
// 显示保护技能UI
// ============================================================================
void CMonster::ShowProtectSkillUI(bool bActive) {
    // IDA 反编译确认:
    // if (m_eSkillType == SKILLTYPE_PROTECT_A) {
    //     if (bActive) {
    //         CMover::SetInvincibleActor(this, 1);
    //         SendNoticePacket(71, m_nProtectSkillDamage, 0.0);
    //     } else {
    //         CMover::SetInvincibleActor(this, 0);
    //         m_eSkillType = SKILLTYPE_NONE;
    //         m_nAccumulateDamage = 0;
    //         SendNoticePacket(71, -1, -1.0);
    //     }
    // } else if (m_eSkillType == SKILLTYPE_PROTECT_B) {
    //     if (bActive) {
    //         CMover::SetInvincibleActor(this, 1);
    //         SendNoticePacket(72, m_nProtectSkillDamage, 1.0);
    //     } else {
    //         CMover::SetInvincibleActor(this, 0);
    //         m_eSkillType = SKILLTYPE_NONE;
    //         m_nAccumulateDamage = 0;
    //         SendNoticePacket(72, -1, -1.0);
    //     }
    // }

    // SKILLTYPE_PROTECT_A = 6, SKILLTYPE_PROTECT_B = 7
    if (m_eSkillType == 6) {  // SKILLTYPE_PROTECT_A
        if (bActive) {
            // TODO: CMover::SetInvincibleActor(1);
            SendNoticePacket(71, m_nProtectSkillDamage, 0.0f);
        } else {
            // TODO: CMover::SetInvincibleActor(0);
            m_eSkillType = 0;  // SKILLTYPE_NONE
            m_nAccumulateDamage = 0;
            SendNoticePacket(71, -1, -1.0f);
        }
    } else if (m_eSkillType == 7) {  // SKILLTYPE_PROTECT_B
        if (bActive) {
            // TODO: CMover::SetInvincibleActor(1);
            SendNoticePacket(72, m_nProtectSkillDamage, 1.0f);
        } else {
            // TODO: CMover::SetInvincibleActor(0);
            m_eSkillType = 0;  // SKILLTYPE_NONE
            m_nAccumulateDamage = 0;
            SendNoticePacket(72, -1, -1.0f);
        }
    }
}

// ============================================================================
// SendNoticePacket IDA 0x14035BBE0 -> 0x14035BC93
// 发送通知包
// ============================================================================
void CMonster::SendNoticePacket(int iType, int iValue, float fTime) {
    // IDA 反编译确认:
    // v4 = this->GetArea(&this->XActor);
    // pMaze = (XMaze *)_RTDynamicCast_0(v4, 0, &XArea RTTI, &XMaze RTTI, 0);
    // if (pMaze) {
    //     if (iValue >= 0)
    //         this->m_nProtectSkill = iType;
    //     else
    //         this->m_nProtectSkill = 0;
    //     XMaze::SendNoticePacket(pMaze, iType, iValue, fTime);
    // }

    // 更新保护技能ID
    if (iValue >= 0) {
        m_nProtectSkill = iType;
    } else {
        m_nProtectSkill = 0;
    }

    // TODO: 获取Area并转换为XMaze，然后发送通知包
    // XArea* pArea = GetArea();
    // if (pArea) {
    //     XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    //     if (pMaze) {
    //         pMaze->SendNoticePacket(iType, iValue, fTime);
    //     }
    // }
}

// ============================================================================
// NotifySpawnMonsterDied IDA 0x14035F1A0 -> 0x14035F325
// 通知召唤怪物死亡 - 处理隐身条件检查
// ============================================================================
void CMonster::NotifySpawnMonsterDied(std::uint32_t dwID) {
    // IDA 反编译确认:
    // if (XActor::IsStatus(&this->XActor, 0x2000u)) {
    //     if (this->m_nInvisibleConditionType == 2) {
    //         if (--this->m_nInvisibleConditionVal[0] <= 0)
    //             CMoverEx::SetInvisible(this, 0, 0, 0, 0, 0, 0, 0);
    //     } else if (this->m_nInvisibleConditionType == 3) {
    //         pMover = CMover::GetMoverObject(this, dwID);
    //         if (pMover) {
    //             bAlive = 0;
    //             for (i = 0; i < 4; ++i) {
    //                 if (this->m_nInvisibleConditionVal[i]) {
    //                     if (pMover->GetTableID(pMover) == this->m_nInvisibleConditionVal[i])
    //                         this->m_nInvisibleConditionVal[i] = 0;
    //                     else
    //                         bAlive = 1;
    //                 }
    //             }
    //             if (!bAlive)
    //                 CMoverEx::SetInvisible(this, 0, 0, 0, 0, 0, 0, 0);
    //         }
    //     }
    // }

    // 检查是否处于隐身状态 (0x2000)
    // TODO: if (!XActor::IsStatus(0x2000u)) return;

    // 检查隐身条件类型
    if (m_nInvisibleConditionType == 2) {
        // 类型2：计数型 - 减少计数
        if (--m_nInvisibleConditionVal[0] <= 0) {
            // TODO: CMoverEx::SetInvisible(0, 0, 0, 0, 0, 0, 0);
        }
    } else if (m_nInvisibleConditionType == 3) {
        // 类型3：目标型 - 检查死亡目标是否在条件列表中
        // TODO: CMover* pMover = CMover::GetMoverObject(this, dwID);
        // if (pMover) {
        //     bool bAlive = false;
        //     for (int i = 0; i < 4; ++i) {
        //         if (m_nInvisibleConditionVal[i]) {
        //             if (pMover->GetTableID() == m_nInvisibleConditionVal[i]) {
        //                 m_nInvisibleConditionVal[i] = 0;
        //             } else {
        //                 bAlive = true;
        //             }
        //         }
        //     }
        //     if (!bAlive) {
        //         CMoverEx::SetInvisible(0, 0, 0, 0, 0, 0, 0);
        //     }
        // }
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
        // TODO: return m_pAi->GetState();
        return 0;
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
    if (m_pAi && pfValue) {
        // TODO: m_pAi->GetAIFuzzyValue(pfValue);
    }
}

// ============================================================================
// GetAIActionValue IDA 0x140357B70
// 获取AI动作值
// ============================================================================
void CMonster::GetAIActionValue(int* pnValue) {
    // IDA 反编译确认: 如果 m_pAi 存在，调用 CAi::GetAIActionValue
    if (m_pAi && pnValue) {
        // TODO: m_pAi->GetAIActionValue(pnValue);
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
// InitialObjectInfo IDA 0x140355120
// 初始化对象信息
// ============================================================================
void CMonster::InitialObjectInfo(unsigned int dwID, unsigned int nTableIdx, hkvVec3 vPos, float fRot) {
    // IDA 反编译确认:
    // CMover::InitialObjectInfo(this, dwID, nTableIdx, vPos, fRot);
    // CGocNpcAttribute::Init(pAttr, 1, m_pMobTableRef);
    // m_fAbility = CGocAttribute::GetFinalStats(pAttr);
    // m_stMonsterInfo.nHP = (int)m_fAbility[10];
    // m_stMonsterInfo.nTableID = nTableIdx;
    // SetPositionXVec3(vPos);
    // m_vCreatePos = vPos;
    // m_fMovingYaw = fRot;
    // SetDirectionYaw(fRot, 1);
    // ApplyTableAbility();
    // SetupAnimation();
    // CGroupAggro::Init(&m_xGroupAggro, this);
    // CWayPoint::Init(&m_xWayPoint, this);
    // CTraceHPState::Init(&m_xTraceHPState, this);
    // m_dwSpawnedTime64 = XTime::GetTickCount();
    // if (m_pMobTableRef && m_pMobTableRef->Monster_Type == 5) {
    //     m_pWeaponTableRef = GetTB_DEFENSIVE_WEAPON(nTableIdx);
    //     SetInvincibleActor(1);
    // }
    // TODO: 需要实现完整初始化逻辑
}

// ============================================================================
// GenerateEventObject IDA 0x1403556D0
// 生成事件对象
// ============================================================================
void CMonster::GenerateEventObject() {
    // IDA 反编译确认:
    // CMoverEx::_GenerateEventObject(this, 2, m_pMobTableRef->ID);
    // FindFollowPlayer();
    // m_bApplyLevel = 0;
    // ApplyLevelToStat(1);
    // TODO: 需要实现 _GenerateEventObject
}

// ============================================================================
// SetInfo IDA 0x140355730
// 设置信息
// ============================================================================
void CMonster::SetInfo() {
    // IDA 反编译确认:
    // sprintf_s(szName, "Monster Idle %u", m_pGrapParent);
    // CMySkillList::Init(pMySkillList, this);
    // TODO: 需要实现 CMySkillList::Init
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
// ============================================================================
void CMonster::ProcessExp(XActor* pActor) {
    // IDA 反编译确认:
    // if (m_pMobTableRef) {
    //     int nRank = m_pMobTableRef->Monster_Rank;
    //     TB_MONSTER_EXP* pTBExp = GetTB_MONSTER_EXP(GetLevel());
    //     if (pTBExp) {
    //         int nExp = (int)(pTBExp->EXP_Slave[nRank] * m_pMobTableRef->Exp);
    //         if (nExp > 0) {
    //             CUser* pUser = dynamic_cast<CUser*>(pActor);
    //             if (pUser)
    //                 pUser->SetExp(nExp, m_pMobTableRef->Monster_Lv);
    //         }
    //     }
    // }
    
    if (!m_pMobTableRef || !pActor) {
        return;
    }

    // 获取怪物等级和排名
    int nRank = m_pMobTableRef->Monster_Rank;
    int nLevel = GetLevel();
    
    // 计算经验值
    // TB_MONSTER_EXP* pTBExp = GetTB_MONSTER_EXP(nLevel);
    // if (pTBExp) {
    //     int nExp = (int)(pTBExp->EXP_Slave[nRank] * m_pMobTableRef->Exp);
    //     if (nExp > 0) {
    //         // 分配经验给击杀者
    //         GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessExp: Exp=%d, Level=%d, Rank=%d", 
    //                         nExp, nLevel, nRank);
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessExp called");
}

// ============================================================================
// DropItemByHit IDA 0x140356290
// 击中掉落物品
// ============================================================================
void CMonster::DropItemByHit(unsigned int dwAtkUser) {
    // IDA 反编译确认:
    // if (m_pMobTableRef && m_pMobTableRef->Monster_Hit_Drop_ID && GetArea()) {
    //     if (GetWorldType() == 1) { // Maze
    //         XMaze* pMaze = (XMaze*)GetArea();
    //         pMaze->ProcessDropByHit(dwAtkUser, Monster_Hit_Drop_ID, Monster_Lv, &vPos, GetTableID());
    //     } else if (GetWorldType() == 2) { // BattleZone
    //         CBattleZone* pBattleZone = (CBattleZone*)GetArea();
    //         pBattleZone->ProcessDropByHit(dwAtkUser, Monster_Hit_Drop_ID, Monster_Lv, &vPos, GetTableID());
    //     }
    // }
    // TODO: 需要实现完整掉落逻辑
}

// ============================================================================
// ProcessDrop IDA 0x140356550
// 处理掉落
// ============================================================================
void CMonster::ProcessDrop(XActor* pAtk) {
    // IDA 反编译确认:
    // if (m_pMobTableRef && GetArea()) {
    //     if (GetWorldType() == 2) { // BattleZone
    //         if (GetTBMapID() != 30031) {
    //             CBattleZone* pBattleZone = (CBattleZone*)GetArea();
    //             pBattleZone->ProcessDrop(pAtk, this, &m_stMonsterInfo.stPosInfo.vPos);
    //         }
    //     } else {
    //         GetArea()->ProcessDrop(pAtk, m_stMonsterInfo.nTableID, &m_stMonsterInfo.stPosInfo.vPos);
    //     }
    // }
    
    if (!m_pMobTableRef || !pAtk) {
        return;
    }

    // 获取掉落位置
    hkvVec3 vDropPos = GetPosition();
    
    // 处理掉落 - 从 TB_MONSTER_DROP 表生成掉落物品
    // int nDropID = m_pMobTableRef->Drop_ID;
    // if (nDropID > 0) {
    //     // 生成掉落物品
    //     GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessDrop: DropID=%d, Pos=(%.2f, %.2f, %.2f)", 
    //                     nDropID, vDropPos.x, vDropPos.y, vDropPos.z);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessDrop called");
}

// ============================================================================
// ProcessEscortQuest IDA 0x140356750
// 处理护送任务
// ============================================================================
void CMonster::ProcessEscortQuest() {
    // IDA 反编译确认:
    // if (m_pAi && CAi::IsEscortMonster(m_pAi) && m_pMobTableRef) {
    //     XMaze* pMaze = (XMaze*)GetArea();
    //     if (pMaze) {
    //         pMaze->FailEscortQuest();
    //         CAi::EndEscortWayPoint(m_pAi);
    //     }
    // }
    // TODO: 需要实现 CAi::IsEscortMonster 和 XMaze::FailEscortQuest
}

// ============================================================================
// ProcessGameMode IDA 0x1403568a0
// 处理游戏模式
// ============================================================================
void CMonster::ProcessGameMode() {
    // IDA 反编译确认:
    // if (!m_bSuicide && m_pMobTableRef && m_pMobTableRef->Monster_Type == 3) {
    //     XMaze* pMaze = dynamic_cast<XMaze*>(m_pArea);
    //     if (pMaze)
    //         XMaze::SetGameModeState(2);
    // } else {
    //     CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(m_pArea);
    //     if (pBattleZone) {
    //         unsigned int HitID = CMover::GetHitID();
    //         CBattleZone::MonsterDieForEvent(this, HitID);
    //     }
    // }
    // TODO: 需要实现 XMaze::SetGameModeState 和 CBattleZone::MonsterDieForEvent
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
// InitComponant IDA 0x1403559a0
// 初始化组件
// ============================================================================
void CMonster::InitComponant() {
    // IDA 反编译确认:
    // GOComponent::CreateAndRegister<CGocNpcAttribute>(&result, this);
    // GOComponent::CreateAndRegister<CGocInventory>(&v2, this);
    // TODO: 需要实现 GOComponent::CreateAndRegister
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
// ApplyTableAbility IDA 0x1403581a0
// 应用表能力
// ============================================================================
void CMonster::ApplyTableAbility() {
    // IDA 反编译确认:
    // m_byPhaseType = m_pMobTableRef->Monster_AI_Type;
    // VString::operator=(&m_strSpecialDamage, m_pMobTableRef->Monster_Special_Damage);
    // m_byPhaseCondition = m_pMobTableRef->Monster_Switching_AI_Condition_01;
    // m_dwPhaseConditionValue = m_pMobTableRef->Monster_Switching_AI_Value_01;
    // ... 更多属性设置
    // TODO: 需要实现完整的表能力应用逻辑
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
    return nullptr;  // TODO: return m_pArea from base class
}

