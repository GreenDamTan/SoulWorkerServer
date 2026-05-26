#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

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
// ============================================================================
UXActorID CMonster::GetParentID() {
    // IDA 0x14009F170: return this->m_stMonsterInfo.uxParentActorID
    // TODO: 需要 STMonsterInfo 结构定义
    UXActorID result = {};
    return result;
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
