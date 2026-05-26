#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"

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
