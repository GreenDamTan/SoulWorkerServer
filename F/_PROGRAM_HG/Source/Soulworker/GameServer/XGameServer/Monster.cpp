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
// GetHP IDA 0x140364D60
// ============================================================================
int CMonster::GetHP() {
    // IDA 0x140364D60: return this->m_stMonsterInfo.nHP
    return m_stMonsterInfo.GetHP();
}

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
// GetTableID IDA 0x140364AD0
// ============================================================================
int CMonster::GetTableID() {
    // IDA 0x140364AD0:
    // if (m_pMobTableRef) return m_pMobTableRef->ID;
    // else return 0;
    if (m_pMobTableRef) {
        return m_pMobTableRef->ID;
    }
    return 0;
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
    // 5. 清理 AI 和脚本实例
    // 6. 重置所有成员变量

    // 调用基类 Reset
    CMoverEx::Reset();

    // 清空伤害计量
    m_arDamageMeter.clear();

    // 重置资源指针
    m_pActionResource = nullptr;
    m_pMobTableRef = nullptr;
    m_pSector = nullptr;

    // 重置 AI
    if (m_pAi) {
        delete m_pAi;
        m_pAi = nullptr;
    }

    // 重置脚本实例
    m_pScriptInst = nullptr;

    // 重置成员变量
    m_nNextSkillID = 0;
    m_shLastSendMoveYaw = 0;
    m_bNeedSendMoveStop = 0;
    m_fInitYaw = 0.0f;
    m_fLastAggroCheckTime = 0.0f;
    m_bChangedAggro = 0;
    m_dwGuardID = 0xFFFFFFFF;
    m_byGazeAnimType = 0;
    m_nSpawnBoxID = 0;
    m_nGroupID = 0;
    m_bShowChangedAI = false;
    m_dwSpawnedTime64 = 0;
    m_bSuicide = 0;
    m_dwGuardMonsterID = 0;
    m_nMoveType = 1;
    m_dwKillerID = 0xFFFFFFFF;
    m_pWeaponTableRef = nullptr;
    m_fLifeTime = 0.0f;
    m_nHPPerSec = 0;
    m_nLastTime = 0;
    m_nProtectSkill = 0;
    m_eSkillType = 0;
    m_nProtectSkillDamage = 0;
    m_bApplyLevel = 0;
    m_bySummonType = 0xFF;
    m_fSummonLifeTime = -1.0f;
    m_nHitCount = 0;
    m_naCount = 0;
    m_bChangeAiScript = false;
    m_bCallScriptDie = false;
    m_fLastDamageCountTime = 0.0f;
    m_fBossHPLogTime = 0.0f;
    m_nCheckBossHP = 0;
    m_bReserveDie = false;
    m_fElapsedDieTime = 0.0f;
    m_byAngleAttackType = 0;
    m_dwTickCountReceiveWrongPos = 0;

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

    // TODO: 获取 GOC 组件并初始化
    // CMover::GetGOC<CGocInventory>(this, &pAttr, 0);
    // if (pAttr) { CGocInventory::Init(pAttr); }

    // XActor::SetInfo(&this->XActor);
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
    //     SetInvincibleActor(0);
    //   }
    // }

    if (GetHP() <= 0 || m_byPhaseMotionStep != 2) {
        if (!CheckSuperArmorMotion(nMotionClass) || m_byPhaseMotionStep) {
            // CMoverEx::ChangeMotion(nMotionClass, bResetPlay, 0);
            // TODO: 需要实现 CMoverEx::ChangeMotion
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
}

// ============================================================================
// CheckSuperArmorMotion
// ============================================================================
bool CMonster::CheckSuperArmorMotion(std::int16_t nMotionClass) {
    // TODO: 需要从 IDA 反编译确认实现
    // 检查是否是超级护甲动作
    return false;
}

// ============================================================================
// CheckProtectSkillUI
// ============================================================================
void CMonster::CheckProtectSkillUI() {
    // TODO: 需要从 IDA 反编译确认实现
    // 检查保护技能 UI
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
    // 2. 检查移动并发送移动包
    // 3. 处理死亡延迟和淡出时间
    // 4. 更新AI
    // 5. 更新仇恨
    // 6. 处理召唤物生命周期
    // 7. Boss HP日志记录

    // TODO: 调用基类 ThinkFunction
    // CMoverEx::ThinkFunction();

    // TODO: 实现完整的 ThinkFunction 逻辑
    // 当前为简化版本，仅处理基本状态

    // 获取帧时间 (暂时使用 0.016f 作为默认值)
    float fDeltaTime = 0.016f;

    // 处理死亡延迟时间
    if (m_fDieDelayTime > 0.0f && m_eDieType == DIE_TYPE_DELAY) {
        m_fDieDelayTime -= fDeltaTime;
        m_fDieDelayMaxTime -= fDeltaTime;

        if (m_fDieDelayTime <= 0.0f || m_fDieDelayMaxTime <= 0.0f) {
            m_fDieDelayTime = 0.0f;
            m_fDieDelayMaxTime = 0.0f;
            m_eDieType = DIE_TYPE_NORMAL;
            // TODO: XActor::ClearStatus(2);
            // TODO: 检查飞行状态并调用 RealDie
        }
    }
    // 处理死亡淡出时间
    else if (m_fDieFadeTime > 0.0f) {
        m_fDieFadeTime -= fDeltaTime;

        if (m_fDieFadeTime <= 0.0f) {
            m_fDieFadeTime = 0.0f;
            // TODO: 检查是否还有动作在执行
        }
    }

    // AI更新
    if (m_pAi && IsCanAI()) {
        // TODO: 调用 CAi::Update
        // TODO: 仇恨检查
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

    // 死亡状态超时检查
    // TODO: if (XActor::IsStatus(2)) { ... }
}

// ============================================================================
// OnDie IDA 0x140356980 -> 0x140357988
// 死亡处理 - 处理掉落、经验、任务等
// ============================================================================
void CMonster::OnDie(XActor* pOwnerActor, float fDamage) {
    // IDA 反编译确认的流程:
    // 1. 检查 m_bOnDie 防止重复调用
    // 2. 设置死亡标志和HP
    // 3. 处理保护技能
    // 4. 处理自杀情况
    // 5. 处理掉落物品
    // 6. 处理护送任务
    // 7. 分配经验
    // 8. 发送死亡包

    if (m_bOnDie) {
        return;
    }

    m_bOnDie = true;

    // 处理保护技能
    if (m_nProtectSkill) {
        // TODO: CMonster::SendNoticePacket(this, m_nProtectSkill, -1, -1.0f);
    }

    // 设置死亡原因
    int nHP = GetHP();
    // TODO: CMoverEx::SetDieReason(0x10, nHP);
    SetHP(0);

    // 自杀处理
    if (m_bSuicide) {
        // TODO: 记录自杀日志
    } else {
        // 处理击杀者
        if (pOwnerActor) {
            // TODO: 检查击杀者是否是怪物
            // TODO: 处理掉落
            // TODO: 处理护送任务
            // TODO: 分配经验
        }

        // 处理玩家击杀
        // TODO: 更新玩家记录、发送日志
    }

    // 处理游戏模式
    // TODO: CMonster::ProcessGameMode(this);

    // 调用脚本死亡前处理
    // TODO: XMaze::CallScriptPreDieMonster

    // 发送死亡包
    // TODO: XSendPacket 发送死亡信息

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::OnDie called");

    (void)pOwnerActor;  // 暂时避免未使用警告
    (void)fDamage;
}

// ============================================================================
// RealDie IDA 0x14035A200 -> 0x14035A5D6
// 执行死亡 - 真正的死亡处理
// ============================================================================
void CMonster::RealDie(std::int16_t nChangeMotion) {
    // IDA 反编译确认的流程:
    // 1. 检查是否已经死亡状态
    // 2. 调用基类 RealDie
    // 3. 处理Boss死亡时连带死亡召唤物
    // 4. 清除保护技能

    // TODO: if (XActor::IsStatus(2)) {
    //     return;  // 已经是死亡状态
    // }

    // TODO: 调用基类 RealDie
    // CMoverEx::RealDie(nChangeMotion);

    // Boss 死亡处理 - 连带死亡召唤物
    if (IsBoss()) {
        // TODO: 检查是否还有残留Boss怪物
        // TODO: 遍历所有怪物，找到父ID匹配的召唤物并杀死它们
    }

    // 清除保护技能
    if (m_nProtectSkill > 0) {
        m_nProtectSkill = 0;
        // TODO: CMonster::SendNoticePacket(this, m_nProtectSkill, -1, -1.0f);
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CMonster::RealDie called");
}

// ============================================================================
// IsBoss IDA 0x140358570
// ============================================================================
bool CMonster::IsBoss() {
    // TODO: 需要从 IDA 反编译确认实现
    // 检查是否是 Boss
    if (m_pMobTableRef) {
        // 暂时返回 false，需要确认实现
        return false;
    }
    return false;
}

// ============================================================================
// IsCanAI IDA 0x140358860
// ============================================================================
bool CMonster::IsCanAI() {
    // TODO: 需要从 IDA 反编译确认实现
    // 检查是否可以执行 AI
    return true;
}

// ============================================================================
// IsFollowObject IDA 推测
// ============================================================================
bool CMonster::IsFollowObject() {
    // TODO: 需要从 IDA 反编译确认实现
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
// GetTopAggroValue IDA 0x140361640
// 获取最高仇恨值
// ============================================================================
float CMonster::GetTopAggroValue() {
    // IDA 反编译确认流程:
    // 遍历 m_arDamageMeter，找到最高的仇恨值

    float maxAggro = 0.0f;

    for (const auto& pair : m_arDamageMeter) {
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
    // IDA 反编译确认流程:
    // 1. 获取当前时间
    // 2. 遍历 m_arDamageMeter
    // 3. 对每个目标，检查是否是玩家且进行了治疗
    // 4. 计算治疗仇恨并累加

    float fCurrTime = 0.0f;  // TODO: 获取实际时间

    for (auto& pair : m_arDamageMeter) {
        std::uint32_t dwID = pair.first;
        tagDamageMeter& dmgMeter = pair.second;

        // 获取目标 Mover
        CMoverEx* pMover = nullptr;  // TODO: CMover::GetMoverObject(this, dwID)
        if (pMover) {
            // 检查是否是玩家且进行了治疗
            // TODO: 需要实现 XActor::IsPlayer 和 CMoverEx::GetAmountOfHeal
            // if (XActor::IsPlayer(&pMover->XActor) && CMoverEx::GetAmountOfHeal(pMover) > 0.0f) {
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
    // IDA 反编译确认流程:
    // 1. 根据技能类型计算基础仇恨
    // 2. 根据技能属性子类型和怪物倾向计算额外仇恨
    // 3. 根据目标等级顺序计算等级修正

    if (!pSkillRef) {
        return 0.0f;
    }

    // 计算基础伤害仇恨
    float fBaseAggro = 0.0f;
    // IDA: if (pSkillRef->Agro_Type) fBaseAggro = fDamage + (fDamage * pSkillRef->Agro_Value * 0.01f)
    // else fBaseAggro = fDamage + pSkillRef->Agro_Value

    // 计算DAP (Damage Aggro Point)
    float DAP = (fDamage + fBaseAggro) * 0.0002f;

    // 检查技能属性子类型
    // IDA: if (pSkillRef->Skill_Attribute_Sub >= 4) return 0.0f

    // 计算SPAP (Skill Point Aggro Point)
    float SPAP = 0.0f;
    // IDA: SPAP = DAP * m_pMobTableRef->Melee_Skill_Inclination[pSkillRef->Skill_Attribute_Sub] * 0.01f

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
    // IDA 反编译确认流程:
    // 1. 如果 fTime 和 fDist 都为 0，直接返回
    // 2. 查找当前目标ID在 m_arDamageMeter 中的记录
    // 3. 检查时间超时或距离超时
    // 4. 如果超时，从列表中移除

    if (fTime == 0.0f && fDist == 0.0f) {
        return;
    }

    // 查找当前目标
    auto it = m_arDamageMeter.find(m_dwTargetID);
    if (it == m_arDamageMeter.end()) {
        return;
    }

    bool bRemoved = false;
    float fCurrTime = 0.0f;  // TODO: 获取实际时间

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
        // CMover* pMover = CMover::GetMoverObject(this, m_dwTargetID);
        // if (pMover) {
        //     float fDistSq = 距离计算;
        //     if (fDistSq > fDist * fDist) {
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
    // IDA 反编译确认流程:
    // 1. 获取当前怪物的 SpawnBoxID
    // 2. 检查是否与 GuardID 匹配
    // 3. 如果匹配，扫描附近的怪物
    // 4. 对符合条件的怪物应用保护仇恨

    std::uint32_t dwSpawnBoxID = static_cast<std::uint32_t>(m_nSpawnBoxID);

    if (dwSpawnBoxID != m_dwGuardID || m_dwGuardID == 0xFFFFFFFF) {
        return;
    }

    // 计算保护仇恨比例
    float fAggroRatio = fAggro * m_fProtectionAggroRatio;

    // TODO: 扫描附近的怪物并应用仇恨
    // 需要实现 XArea::ScanGridOrigin
    // 对每个符合条件的怪物调用 ApplyAggroValue(pMonster, dwID, fAggroRatio, true)
}

// ============================================================================
// DamageAggressive IDA 0x14035FC60
// 伤害激怒处理 - 选择仇恨最高的目标
// ============================================================================
void CMonster::DamageAggressive() {
    // IDA 反编译确认流程:
    // 1. 遍历 m_arDamageMeter 找到仇恨最高的目标
    // 2. 对所有目标的仇恨值进行衰减
    // 3. 如果最高仇恨目标改变，切换目标

    float fCurrTime = 0.0f;  // TODO: 获取实际时间
    float fResetTime = 0.0f;
    if (m_pAi) {
        // TODO: fResetTime = CAi::GetDmgAggroReseTime(m_pAi);
    }

    std::uint32_t dwTopID = 0xFFFFFFFF;
    float fTopDamage = 0.0f;

    // 遍历所有伤害计量记录
    for (auto it = m_arDamageMeter.begin(); it != m_arDamageMeter.end(); ) {
        float fAggro = it->second.fAggro;

        // 找最高仇恨
        if (fAggro > fTopDamage) {
            fTopDamage = fAggro;
            dwTopID = it->first;
        }

        // 计算衰减
        float fLeftTime = fCurrTime - it->second.fTime;
        if (fLeftTime > 0.0f) {
            if (fResetTime <= fLeftTime) {
                it->second.fAggro = 0.0f;
            } else {
                float fRate = fLeftTime / fResetTime;
                it->second.fAggro -= fAggro * fRate;
            }
        }

        // 基础衰减
        it->second.fAggro -= fAggro * 0.5f;

        // 如果仇恨值过低，清零
        if (fResetTime >= it->second.fAggro) {
            it->second.fAggro = 0.0f;
        }

        ++it;
    }

    m_bChangedAggro = 0;

    // 检查是否需要切换目标
    if (dwTopID == m_dwTargetID || dwTopID == 0xFFFFFFFF) {
        // 目标未改变或没有有效目标
        if (m_pAi && dwTopID == 0xFFFFFFFF) {
            // TODO: if (CAi::IsEnableClearTarget(m_pAi)) {
            //     ChangeTarget(UXActorID(0xFFFFFFFF));
            // }
        }
    } else {
        // 切换到新目标
        ChangeTarget(UXActorID(dwTopID));
    }
}

// ============================================================================
// UpdateDamageAggressive IDA 0x14035F5B0
// 更新伤害激怒
// ============================================================================
void CMonster::UpdateDamageAggressive(CMoverEx* pMover, int nDamage) {
    // IDA 反编译: AddDamageMeter + DamageAggressive
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
