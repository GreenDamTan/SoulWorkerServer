#include "Mover.h"
#include "Soulworker/GameServer/XGameServer/MySkillList.h"
#include "Soulworker/GameServer/XCore/ThreadLocalData.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XCore/XArea/XMaze.h"
#include "Soulworker/GameServer/XCore/XArea/XDistrict.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XCore/XArea/DohHavokNavMeshInstance.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XGameServer/Ai.h"
#include "Soulworker/GameServer/XGameServer/ActionResMgr.h"  // For AttackJudgmentTrigger
#include "Soulworker/GameServer/XGameServer/GameServer.h"  // For XGameServer
#include "Soulworker/GameServer/XGameServer/Monster.h"  // For CMonster
#include "Soulworker/GameServer/XGameServer/Sector.h"  // For CSector
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"  // For TB_STATUS
#include <cstdarg>
#include <cstdio>
#include <algorithm>  // for std::clamp

// Component headers for GetGOC wrappers
#include "Soulworker/GameServer/XGameServer/actor/component/GOComponent.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocSkill.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocParty.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocForce.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAchieve.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocPost.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocFriend.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocBooster.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAkashicRecord.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocWeeklyMission.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocClassEvent.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocHelper.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEvent.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocLeague.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocExchange.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocDailyMission.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNpcAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocSoulMetry.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNpcCredit.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttendance.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocMyRoom.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"  // For CGocNetwork

/**
 * @brief CMover constructor
 * IDA: ??0CMover@@QEAA@XZ (0x1403649E0)
 */
CMover::CMover()
    : m_fLastUpdateTime(0.0f)
    , m_fLastDebugTime(0.0f)
    , m_fAbility(nullptr)
    , m_dwTargetID(0)
    , m_dwHitID(0)
    , m_fCapsuleRadius(0.0f)
    , m_fCapsuleHeight(0.0f)
    , m_fDieDelayTime(0.0f)
    , m_fDieDelayMaxTime(0.0f)
    , m_fDieFadeTime(0.0f)
    , m_nDamage(0)
    , m_cWeightRank(0)
    , m_byDmgMontionFlag(0)
    , m_fMoving(0)
    , m_bGazeMoving(0)
    , m_fMoveSpeed(0.0f)
    , m_byMoveDir(0)
    , m_nBuffTotalCnt(0)
    , m_nBuffCnt(0)
    , m_nDebuffCnt(0)
    , m_nHitCallBuffIndex(0)
    , m_fLastSendMoveTime(0.0f)
    , m_byDefaultAnimStep(0)
    , m_fAnimSpeed(1.0f)
    , m_fRestoreAnimSpeed(1.0f)
    , m_eRestoreAnimSpeedType(ANIM_SPEED_TYPE_NORMAL)
    , m_nMotionClass(0)
    , m_nHitStatus(0)
    , m_nHitAnimCount(0)
    , m_nTurnStatus(0)
    , m_bAnimChanged(0)
    , m_bSkipAnimOffset(0)
    , m_bAnimPlay(0)
    , m_nAnimationIdx(0)
    , m_nAnimGroup(0)
    , m_fAnimationTime(0.0f)
    , m_fAnimPercentTime(0.0f)
    , m_pCurMotionEvent(nullptr)
    , m_pActionResource(nullptr)
    , m_pAkashicActionInfo(nullptr)
    , m_nCurSkillTableIdx(0)
    , m_nParentSkillTableIdx(0)
    , m_fReactionRate(0.0f)
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
    , m_byRestoreDefenceType(0)
    , m_DefanseChangeFlag(0)
    , m_byDefaultDefenseType(0)
    , m_byRestoreDefenceTypeByTrigger(0)
    , m_byDefenseTypeDisableFlag(0)
    , m_fSkillCoolDownRate(1.0f)
    , m_fSkillReflectRate(0.0f)
    , m_fSkillBloodRate(0.0f)
    , m_nAllowBloodCount(0)
    , m_fSkillBloodDebuffRate(0.0f)
    , m_dwBloodDebuffOwnerID(0)
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
    , m_mShaderState(0)
    , m_bInvincibleActor(0)
    , m_mapAnimInfoKey(nullptr)
    , m_mapAnimInfoString(nullptr)
    , m_pHitCollisionData(nullptr)
    , m_fHitCylinderRadius(0.0f)
    , m_fHitCylinderHeight(0.0f)
    , m_byTargetPosCount(0)
    , m_byTargetDestPos(0)
    , m_fMaxSuperArmorGage(0.0f)
    , m_fCurSuperArmorGage(0.0f)
    , m_fRegenSuperArmorTime(0.0f)
    , m_fDefRegenSuperArmorTime(0.0f)
    , m_fRecoverySuperArmorTime(0.0f)
    , m_dwCellID(0)
    , m_uiForcedState(0)
    , m_fForcedStateApplyTime(0.0f)
    , m_fProtectionAggroRatio(0.0f)
    , m_pSkillMgr(nullptr)
    , m_bTraceUser(0)
    , m_bPublicTransportRiding(false)
    , m_shCurRandomShootProjectileDirX(0.0f)
    , m_shCurRandomShootProjectileDirY(0.0f)
    , m_nRandomTrapIndex(0)
    , m_dwImmunityStatus(0)
    , m_bIgnoreAggroDebuff(false)
    , m_fBossAttackAddRate(0.0f)
    , m_fBossAttackedDownRate(0.0f)
    , m_fSoulCostDownRate(0.0f)
    , m_fAllAttackAddRate(0.0f)
    , m_bNoSkillCostSG(false)
{
    // Initialize move pos structures
    memset(&m_stMovePos, 0, sizeof(m_stMovePos));
    memset(&m_stMoveGap, 0, sizeof(m_stMoveGap));
    memset(&m_stMoveOffset, 0, sizeof(m_stMoveOffset));
    memset(&m_stExtMovingVal, 0, sizeof(m_stExtMovingVal));
    memset(&m_stTimeSlow, 0, sizeof(m_stTimeSlow));
    memset(m_stBuffState, 0, sizeof(m_stBuffState));
    memset(m_byTargetPosInfo, 0, sizeof(m_byTargetPosInfo));
}

// ============================================================================
// SOptionEffect - Constructor and Destructor
// IDA: 0x1403A1B90 (constructor), 0x1403A1C00 (destructor)
// ============================================================================

SOptionEffect::SOptionEffect()
    : dwEquipedIndex(0)
    , dwTargetMoverID(0)
    , fOptionValue(0.0f)
    , fReferanceValue(0.0f)
    , fAppliedValue(0.0f)
    , pOptionTable(nullptr)
    , dwLifeTime(0)
    , fCurTime(0.0f)
{
}

SOptionEffect::~SOptionEffect()
{
    dwEquipedIndex = 0;
    dwTargetMoverID = 0;
    fOptionValue = 0.0f;
    fReferanceValue = 0.0f;
    fAppliedValue = 0.0f;
    pOptionTable = nullptr;
    dwLifeTime = 0;
    fCurTime = 0.0f;
}

// ============================================================================
// CMover::GetGOC_Xxx wrapper functions
// These are explicit instantiations of the GetGOC<T> template
// ============================================================================

std::shared_ptr<CGocEntity> CMover::GetGOC_Entity(bool bCreateIfNull) {
    std::shared_ptr<CGocEntity> result;
    GetGOC<CGocEntity>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocNetwork> CMover::GetGOC_Network(bool bCreateIfNull) {
    std::shared_ptr<CGocNetwork> result;
    GetGOC<CGocNetwork>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocInventory> CMover::GetGOC_Inventory(bool bCreateIfNull) {
    std::shared_ptr<CGocInventory> result;
    GetGOC<CGocInventory>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocAttribute> CMover::GetGOC_Attribute(bool bCreateIfNull) {
    std::shared_ptr<CGocAttribute> result;
    GetGOC<CGocAttribute>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocSkill> CMover::GetGOC_Skill(bool bCreateIfNull) {
    std::shared_ptr<CGocSkill> result;
    GetGOC<CGocSkill>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocParty> CMover::GetGOC_Party(bool bCreateIfNull) {
    std::shared_ptr<CGocParty> result;
    GetGOC<CGocParty>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocForce> CMover::GetGOC_Force(bool bCreateIfNull) {
    std::shared_ptr<CGocForce> result;
    GetGOC<CGocForce>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocQuest> CMover::GetGOC_Quest(bool bCreateIfNull) {
    std::shared_ptr<CGocQuest> result;
    GetGOC<CGocQuest>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocAchieve> CMover::GetGOC_Achieve(bool bCreateIfNull) {
    std::shared_ptr<CGocAchieve> result;
    GetGOC<CGocAchieve>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocPost> CMover::GetGOC_Post(bool bCreateIfNull) {
    std::shared_ptr<CGocPost> result;
    GetGOC<CGocPost>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocFriend> CMover::GetGOC_Friend(bool bCreateIfNull) {
    std::shared_ptr<CGocFriend> result;
    GetGOC<CGocFriend>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocBooster> CMover::GetGOC_Booster(bool bCreateIfNull) {
    std::shared_ptr<CGocBooster> result;
    GetGOC<CGocBooster>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocRecode> CMover::GetGOC_Recode(bool bCreateIfNull) {
    std::shared_ptr<CGocRecode> result;
    GetGOC<CGocRecode>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocAkashicRecord> CMover::GetGOC_AkashicRecord(bool bCreateIfNull) {
    std::shared_ptr<CGocAkashicRecord> result;
    GetGOC<CGocAkashicRecord>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocWeeklyMission> CMover::GetGOC_WeeklyMission(bool bCreateIfNull) {
    std::shared_ptr<CGocWeeklyMission> result;
    GetGOC<CGocWeeklyMission>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocClassEvent> CMover::GetGOC_ClassEvent(bool bCreateIfNull) {
    std::shared_ptr<CGocClassEvent> result;
    GetGOC<CGocClassEvent>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocHelper> CMover::GetGOC_Helper(bool bCreateIfNull) {
    std::shared_ptr<CGocHelper> result;
    GetGOC<CGocHelper>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocEvent> CMover::GetGOC_Event(bool bCreateIfNull) {
    std::shared_ptr<CGocEvent> result;
    GetGOC<CGocEvent>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocLeague> CMover::GetGOC_League(bool bCreateIfNull) {
    std::shared_ptr<CGocLeague> result;
    GetGOC<CGocLeague>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocExchange> CMover::GetGOC_Exchange(bool bCreateIfNull) {
    std::shared_ptr<CGocExchange> result;
    GetGOC<CGocExchange>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocDailyMission> CMover::GetGOC_DailyMission(bool bCreateIfNull) {
    std::shared_ptr<CGocDailyMission> result;
    GetGOC<CGocDailyMission>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocNpcAttribute> CMover::GetGOC_NpcAttribute(bool bCreateIfNull) {
    std::shared_ptr<CGocNpcAttribute> result;
    GetGOC<CGocNpcAttribute>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocSoulMetry> CMover::GetGOC_SoulMetry(bool bCreateIfNull) {
    std::shared_ptr<CGocSoulMetry> result;
    GetGOC<CGocSoulMetry>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocNpcCredit> CMover::GetGOC_NpcCredit(bool bCreateIfNull) {
    std::shared_ptr<CGocNpcCredit> result;
    GetGOC<CGocNpcCredit>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocAttendance> CMover::GetGOC_Attendance(bool bCreateIfNull) {
    std::shared_ptr<CGocAttendance> result;
    GetGOC<CGocAttendance>(&result, bCreateIfNull);
    return result;
}

std::shared_ptr<CGocMyroom> CMover::GetGOC_Myroom(bool bCreateIfNull) {
    std::shared_ptr<CGocMyroom> result;
    GetGOC<CGocMyroom>(&result, bCreateIfNull);
    return result;
}

/**
 * @brief CMover destructor
 */
CMover::~CMover() {
    // Clean up
    m_setHitID.clear();
    m_listSummonMob.clear();
    m_setAllowPassiveType.clear();
    m_listDefenseChangeInfo.clear();
    m_mapFilterData.clear();
    m_vTraceBoneName.clear();
    m_vecDelayedProjectile.clear();
    m_GOComponentTable.clear();
    m_vContinuousMelee.clear();
    m_mapMeleeDebuff.clear();
    m_mapSkillUnlock.clear();
}

// ============================================================================
// CMover Static Methods
// ============================================================================

// IDA: ?GetMoverObject@CMover@@QEAAPEAV1@K@Z @ 0x14036D1E0
// IDA 精确还原 - 通过ID获取Mover对象
CMover* CMover::GetMoverObject(std::uint32_t dwID) {
    if (dwID == 0xFFFFFFFF) {
        return nullptr;
    }

    XArea* pArea = GetArea();
    if (!pArea) {
        return nullptr;
    }

    XActor* pActor = pArea->FindActor(dwID);
    if (!pActor) {
        return nullptr;
    }

    return dynamic_cast<CMover*>(pActor);
}

// ============================================================================
// CMover Virtual Methods - Stubs
// ============================================================================

// IDA: ?IsDie@CMover@@QEAAHXZ @ 0x140366E40
// IDA精确还原 - 判断是否死亡(XActor::IsDieStatus || GetHP <= 0)
bool CMover::IsDie() const {
    return XActor::IsDieStatus() || GetHP() <= 0;
}

// IDA: ?IsHitDown@CMover@@QEAAHXZ @ 0x140367270
// IDA 精确还原 - 判断是否处于倒地状态
bool CMover::IsHitDown() const {
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 3;
    }
    if (m_nMotionClass >= 18 && m_nMotionClass <= 21) {
        if (m_nHitStatus == 1 || m_nHitStatus == 2 || m_nHitStatus == 3) {
            return true;
        }
        return (m_bLanded && m_nHitStatus != 5 && m_nHitStatus != 0) || m_nMotionClass == 13;
    }
    return m_nMotionClass == 13;
}

// IDA: ?IsCounterAttackHit@CMover@@UEAAHXZ @ 0x140367360
// IDA 精确还原 - 判断是否处于反击命中状态
bool CMover::IsCounterAttackHit() const {
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 4;
    }
    return !IsHitDown()
        && m_nMotionClass >= 17
        && m_nMotionClass <= 23
        && m_nMotionClass != 22
        && m_nHitStatus != 5;
}

// IDA: ?GetLevel@CMover@@UEAAEXZ @ 0x140366CB0
// IDA 精确还原 - 通过 CGocAttribute 获取 Level
std::uint8_t CMover::GetLevel() {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);
    if (!pAttr) {
        return 0;
    }
    return pAttr->GetLevel();
}

// IDA: ?GetClass@CMover@@UEAAEXZ @ 0x140366C30
// IDA 精确还原 - 通过 CGocAttribute 获取 Class
std::uint8_t CMover::GetClass() {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);
    if (!pAttr) {
        return 0;
    }
    return pAttr->GetClass();
}

// IDA: ?GetLevelForStat@CMover@@UEAAEXZ @ 0x140366D30
// IDA 精确还原 - 通过 CGocAttribute 获取用于属性计算的等级
std::uint8_t CMover::GetLevelForStat() {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);
    if (!pAttr) {
        return 0;
    }
    return pAttr->GetLevelForStat();
}

// IDA: ?GetOrientationYaw@CMover@@UEAAMXZ @ 0x140375220
// IDA 精确还原 - 获取朝向偏航角
float CMover::GetOrientationYaw() {
    return m_vOrientation.x;
}

// IDA: ?GetItemRateFlag@CMover@@UEAAEXZ
std::uint8_t CMover::GetItemRateFlag() {
    // TODO: Implement from IDA
    return 0;
}

// IDA: ?GetDamageCalc@CMover@@UEAAHHEMH_N@Z @ 0x140375000
// IDA 精确还原 - 基类伤害计算（返回0）
int CMover::GetDamageCalc(int nAP, std::uint8_t byType, float fReduceRate) {
    return 0;
}

// IDA: ?GetBossAttackAddRate@CMover@@QEAAMXZ @ 0x1403A2730
// IDA 精确还原 - 获取Boss攻击加成率
float CMover::GetBossAttackAddRate() {
    return m_fBossAttackAddRate;
}

// IDA: ?GetSkillCoolDownRate@CMover@@QEAAMXZ @ 0x1402C7240
// IDA 精确还原 - 获取技能冷却速率
float CMover::GetSkillCoolDownRate() const {
    return m_fSkillCoolDownRate;
}

// IDA: ?GetSkillBloodRate@CMover@@QEAAMXZ @ 0x1403A2410
// IDA 精确还原 - 获取技能吸血率
float CMover::GetSkillBloodRate() const {
    return m_fSkillBloodRate;
}

// Note: GetMaxSuperArmorGage, GetCurSuperArmorGage, SetCurSuperArmorGage
// are implemented in MoverLinkStubs.cpp

// IDA: ?GetDamageCalc@CMoverEx@@UEAAHHEMH_N@Z @ 0x140388170
// IDA 精确还原 - 伤害计算（CMoverEx版本）
int CMoverEx::GetDamageCalc(int nAP, std::uint8_t byType, float fReduceRate, int iItemRateResult, bool bIgnoreInvinsible) {
    // 检查是否免疫伤害
    if ((!m_pCurSkillTableRef ||
         (m_pCurSkillTableRef->Skill_Type != 6 && m_pCurSkillTableRef->Skill_Type != 7)) &&
        !IsExceptionalDamage() &&
        m_byDefenseType == 3 &&
        (!bIgnoreInvinsible || IsImmunityStatus())) {
        return 0;
    }

    // 计算防御力
    float fDefence;
    if (byType == 2) {
        fDefence = m_fAbility[25];  // 魔法防御
    } else {
        fDefence = m_fAbility[24];  // 物理防御
    }
    fDefence += static_cast<float>(iItemRateResult);
    if (fDefence < 0.0f) {
        fDefence = 0.0f;
    }

    // 应用减免率
    if (fReduceRate > 100.0f) {
        fReduceRate = 100.0f;
    }
    float fDefenceRate = (100.0f - fReduceRate) * 0.01f;
    fDefence *= fDefenceRate;

    // 获取等级用于计算
    int nLevel = GetLevelForStat();
    if (fDefence <= 0.0f) {
        fDefence = 1.0f;
    }

    // 计算最终伤害
    int iDamage = static_cast<int>(static_cast<float>(nAP) * (1.0f - fDefence / (fDefence + static_cast<float>(50 * nLevel))));

    // 检查防御类型1（20%伤害）
    if (m_byDefenseType == 1) {
        iDamage = static_cast<int>(static_cast<float>(iDamage) * 0.2f);
    }

    // 最低伤害为1
    if (iDamage <= 0) {
        return 1;
    }

    return iDamage;
}

// IDA: ?IsActivateSkillUnlockBuff@CMover@@QEAA_NPEAUTB_SKILL@@@Z @ 0x140367560
// IDA 精确还原 - 检查技能是否激活了解锁Buff
bool CMover::IsActivateSkillUnlockBuff(const TB_SKILL* pSkill) {
    if (!pSkill) {
        return false;
    }

    // IDA: 遍历 m_mapSkillUnlock 检查 second 是否匹配 Skill_Group
    // IDA code: *((_DWORD *)&...->first + 1) == pTBSkill->Skill_Group
    // This accesses the second field of the pair (value in map)
    for (auto it = m_mapSkillUnlock.begin(); it != m_mapSkillUnlock.end(); ++it) {
        if (it->second == pSkill->Skill_Group) {
            return true;
        }
    }
    return false;
}

// IDA: ?ActionProcess@CMover@@UEAAHF@Z @ 0x1401892D0
// IDA 精确还原 - 动作处理（基类返回1）
int CMover::ActionProcess(std::int16_t nAction) {
    return 1;
}

// IDA: ?Damage@CMover@@UEAAXKEEEHMAEAVhkvVec3@@@Z @ 0x140189300
// IDA 精确还原 - 伤害处理（基类空实现）
void CMover::Damage(std::uint32_t dwAttackerID, std::uint8_t byReactionType, std::uint8_t byAttackCollision) {
    // Empty base implementation - overridden by CMonster/CUser
}

// IDA: ?DamageProcessHP@CMover@@UEAAHKHHEE@Z @ 0x1401892E0
// IDA 精确还原 - HP伤害处理（基类返回1）
int CMover::DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage) {
    return 1;
}

// Extended version - also base class
int CMover::DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage,
                            std::uint8_t byDamageFlag, int nInvokeType, int nUnk) {
    return 1;
}

// IDA: ?GetSkillMgr@CMover@@UEAAPEAVCMySkillList@@XZ @ 0x140365BE0
CMySkillList* CMover::GetSkillMgr() {
    return m_pSkillMgr;
}

// IDA: ?CalcTargetDamage@CMover@@UEAAXPEAV1@HPEAUTB_SKILL@@MM@Z
// IDA 精确还原 - 计算目标伤害（基类空实现，由 CMoverEx 重写）
void CMover::CalcTargetDamage(CMover* pAttacker, int nDamage, TB_SKILL* pSkill, float fRate, float fDamage) {
    // 基类空实现 - 由 CMoverEx::CalcTargetDamage (0x140388670) 重写
    (void)pAttacker;
    (void)nDamage;
    (void)pSkill;
    (void)fRate;
    (void)fDamage;
}

// IDA: ?OnUpdate@CMoverEx@@UEAAXM@Z
// IDA 精确还原 - 更新函数（调用 ThinkFunction）
void CMoverEx::OnUpdate(float fDeltaTime) {
    // OnUpdate 通常调用 ThinkFunction 进行主要逻辑处理
    // 具体实现见 CUser::OnUpdate (0x1406ED290)
    ThinkFunction();
}

// IDA: ?GetBossAttackedDownRate@CMover@@QEAAMXZ @ 0x1403A2710
// IDA 精确还原 - 获取Boss被攻击下降率
float CMover::GetBossAttackedDownRate() {
    return m_fBossAttackedDownRate;
}

// IDA: ?GetAllAttackAddRate@CMover@@QEAAMXZ @ 0x1403A26F0
// IDA 精确还原 - 获取所有攻击加成率
float CMover::GetAllAttackAddRate() {
    return m_fAllAttackAddRate;
}

// IDA: ?ClearBuffStatusBySlot@CMover@@UEAAXG_N@Z @ 0x140377550
// IDA 精确还原 - 清除指定槽位Buff状态（基类空实现）
void CMover::ClearBuffStatusBySlot(std::uint16_t nBuffSlot, bool bExcuteOutSkill) {
    // Empty base implementation - overridden by CMoverEx
}

// IDA: ?ClearBuffStatus@CMover@@UEAAXG_NK@Z @ 0x140374FC0
// IDA 精确还原 - 清除Buff状态（基类空实现）
void CMover::ClearBuffStatus(std::uint16_t nBuffIndex, bool bExcuteOutSkill, std::uint32_t dwOwnerID) {
    // Empty base implementation - overridden by CMoverEx
}

// IDA: ?SetBuffStatus@CMover@@UEAAHGK_N@Z @ 0x140374FE0
// IDA 精确还原 - 设置Buff状态（基类返回false）
bool CMover::SetBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwOwnerID, bool bShowBuff) {
    return false;  // Base class returns false
}

// IDA: ?SetBuffStatus@CMoverEx@@UEAAHGK_N@Z @ 0x14038BCE0
// IDA 精确还原 - 设置Buff状态（CMoverEx版本）
bool CMoverEx::SetBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwOwnerID, bool bShowBuff) {
    // 检查怪物类型和标志
    if (GetType() == 2) {  // Monster type
        CMonster* pMonster = dynamic_cast<CMonster*>(this);
        if (pMonster && (pMonster->GetMonsterFlag() & 1) != 0) {
            return false;
        }
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(nBuffIndex);
    if (!pBuffTable) {
        return false;
    }

    // 检查是否可应用Buff
    if (!IsCanApplyBuff(nBuffIndex, nullptr)) {
        return false;
    }

    // 检查Buff槽位上限
    if (m_nBuffTotalCnt >= 50) {
        return false;
    }

    bool bCheckPolicy = false;
    std::uint8_t bySystem_Type = pBuffTable->System_Type;

    // 免疫状态检查
    if (IsImmunityStatus() && pBuffTable->Buff_Type == 1 && bySystem_Type != 3) {
        return false;
    }

    // 系统类型检查
    if (bySystem_Type == 1) {
        if (GetType() == 2) {  // Monster type
            CMonster* pMonster = dynamic_cast<CMonster*>(this);
            if (pMonster) {
                TB_MONSTER* pMobRef = pMonster->GetMobTableRef();
                if (pMobRef) {
                    std::uint8_t byMonsterRank = pMobRef->Monster_Rank;
                    if (byMonsterRank == 3 || byMonsterRank == 4) {
                        return false;  // Boss monsters
                    }
                }
            }
        }
        bCheckPolicy = true;
    } else if (bySystem_Type == 0) {
        bCheckPolicy = true;
    }

    // 防御类型检查
    if (bCheckPolicy && m_byDefenseType == 3 && pBuffTable->Buff_Type == 1) {
        return false;
    }

    // Buff类别检查
    std::uint8_t byCategory = GetBuffCategory(pBuffTable->EffectType_01);

    // 控制类Debuff检查
    if (byCategory == 2 && pBuffTable->EffectType_01 != 124 && IsStatus(0x8000000u)) {
        return false;
    }
    if (byCategory == 1 && IsStatus(0x8000000u)) {
        return false;
    }

    // 检查被动Debuff
    if (CheckPassDebuff(nBuffIndex) == 1) {
        return false;
    }

    // 设置命中调用Buff
    if (pBuffTable->Hit_Call_Buff && !m_nHitCallBuffIndex) {
        m_nHitCallBuffIndex = pBuffTable->Hit_Call_Buff;
    }

    // 获取OwnerID
    if (!dwOwnerID) {
        UXActorID actorID = GetActorID();
        dwOwnerID = actorID.parts.dwID;
    }

    float fTime = static_cast<float>(pBuffTable->Buff_Time) * 0.001f;
    bool bShouldDie = false;

    // 查找同组Buff
    int iIndex = FindBuffByGroupID(pBuffTable->Buff_Group, dwOwnerID);

    if (iIndex == -1) {
        // 新Buff
        // 检查被动技能 (EffectType >= 111 for player)
        if (pBuffTable->EffectType_01 >= 0x6F && GetType() == 0) {
            CheckPassiveSkill(1, 45);
        }

        iIndex = GetEmptyBuffSlot();
        UpdateBuffCount(pBuffTable->Buff_Type, 1);
        LoadBuffStatus(iIndex, nBuffIndex, fTime, true, dwOwnerID, bShowBuff);

        // 播放设置Buff动画
        if (pBuffTable->SetBuffActionName && pBuffTable->SetBuffActionName[0] != '0') {
            VString strAnimName(pBuffTable->SetBuffActionName);
            std::uint32_t dwKey = GetAnimIndex(strAnimName);
            if (dwKey != static_cast<std::uint32_t>(-1)) {
                m_nBuffMotion = AnimKeyToMotion(dwKey);
                if (!IsHit()) {
                    ChangeMotion(m_nBuffMotion, 1, 0);
                }
            }
        }

        bShouldDie = UpdateBuffAbility(m_stBuffState[iIndex], true);
    } else {
        // 已存在的Buff - 刷新/叠加
        bool bChangeControlDebuff = false;
        bool bDontRemoveBuff = false;

        // 控制类Debuff切换
        if (byCategory == 2 && m_stBuffState[iIndex].byEffectType != pBuffTable->EffectType_01) {
            bChangeControlDebuff = true;
            bDontRemoveBuff = true;
            bShouldDie = UpdateBuffAbility(m_stBuffState[iIndex], false);
        }

        // 特殊效果类型13
        if (m_stBuffState[iIndex].byEffectType == 13 && pBuffTable->EffectType_01 == 13) {
            bDontRemoveBuff = true;
        }

        // 保存当前属性
        bool bSaveCurStat = IsCheckCurStat(pBuffTable);
        float fStat[4] = {0.0f, 0.0f, 0.0f, 0.0f};

        if (bSaveCurStat) {
            fStat[0] = GetStat(1);
            fStat[1] = GetStat(2);
            fStat[2] = GetStat(3);
            fStat[3] = GetStat(16);
        }

        // 设置Buff叠加
        int nResult = SetBuffOverlap(iIndex, pBuffTable, dwOwnerID, bDontRemoveBuff);
        if (nResult != 0) {
            return nResult > 0;
        }

        // 更新Buff能力
        if (bChangeControlDebuff || !pBuffTable->EffectType_01 ||
            pBuffTable->EffectType_01 == 21 ||
            (pBuffTable->EffectType_01 >= 0x1C && pBuffTable->EffectType_01 <= 0x1F)) {
            bShouldDie = UpdateBuffAbility(m_stBuffState[iIndex], true);

            // 恢复保存的属性
            if (bSaveCurStat) {
                if (fStat[0] > GetStat(1)) SetStat(1, fStat[0]);
                if (fStat[1] > GetStat(2)) SetStat(2, fStat[1]);
                if (fStat[2] > GetStat(3)) SetStat(3, fStat[2]);
                if (fStat[3] > GetStat(16)) SetStat(16, fStat[3]);
            }
        }
    }

    // 特殊效果处理
    if (pBuffTable->EffectType_01 == 6) {
        UpdateDefenseDisableBuff();
    }

    // 效果类型123/124清除相关Buff
    if (pBuffTable->EffectType_01 == 123 || pBuffTable->EffectType_01 == 124) {
        for (int iType = 111; iType <= 114; ++iType) {
            int iTempIndex = FindBuffByEffectType(iType, 0);
            if (iTempIndex != -1) {
                ClearBuffStatusBySlot(iTempIndex, false);
            }
        }
    }

    // 发送Buff更新包
    send_eSUB_CMD_BUFF_UPDATE(
        this,
        nBuffIndex,
        m_stBuffState[iIndex].fLifeTime,
        m_stBuffState[iIndex].byCount,
        m_stBuffState[iIndex].dwID,
        m_stBuffState[iIndex].bySendType,
        m_stBuffState[iIndex].bShow);

    SendUpdateBuffAbility();

    // 死亡检查
    if (bShouldDie) {
        SetDie(12, 0);
    }

    return true;
}

// IDA: ?IsClearBuff@CMover@@UEAAHGE@Z @ 0x1403774F0
// IDA 精确还原 - 检查是否可清除Buff（基类返回true）
bool CMover::IsClearBuff(std::uint16_t nBuffID, std::uint8_t byReason) {
    return true;  // Base class always returns true
}

// IDA: ?IsClearBuff@CMoverEx@@UEAAHGE@Z @ 0x1403903D0
// IDA 精确还原 - 检查是否可清除Buff（CMoverEx版本）
bool CMoverEx::IsClearBuff(std::uint16_t nBuffID, std::uint8_t byReason) {
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return true;
    }

    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(nBuffID);
    if (pBuffTable) {
        return (byReason & pBuffTable->Delete_Type) == 0;
    }

    LogHelper::LogDebug("game.contents", "IsClearBuff>> not exist buff id. (%d)", nBuffID);
    return true;
}

// IDA: ?IsCheckCurStat@CMoverEx@@QEAA_NPEAUTB_BUFF@@@Z @ 0x14038C7A0
// IDA 精确还原 - 检查是否需要检查当前属性
bool CMoverEx::IsCheckCurStat(TB_BUFF* pNewBuff) {
    if (!pNewBuff) {
        return false;
    }
    // 效果类型不为0则返回false
    if (pNewBuff->EffectType_01) {
        return false;
    }
    // 检查各状态效果是否达到最大值
    if (pNewBuff->EffectType_Status_01 && IsMaxValStat(pNewBuff->EffectType_Status_01)) {
        return true;
    }
    if (pNewBuff->EffectType_Status_02 && IsMaxValStat(pNewBuff->EffectType_Status_02)) {
        return true;
    }
    return pNewBuff->EffectType_Status_03 && IsMaxValStat(pNewBuff->EffectType_Status_03);
}

// IDA: ?IsMaxValStat@CMoverEx@@QEAAHE@Z @ 0x14038C860
// IDA 精确还原 - 检查是否为最大值属性类型
bool CMoverEx::IsMaxValStat(std::uint8_t byStatType) {
    return byStatType == 15
        || byStatType == 16
        || byStatType == 19
        || byStatType == 20
        || byStatType == 23
        || byStatType == 24
        || byStatType == 29
        || byStatType == 30;
}

// IDA: ?GetBuffCategory@CMover@@QEAAEE@Z @ 0x14036B000
// IDA 精确还原 - 获取Buff类别
std::uint8_t CMover::GetBuffCategory(std::uint8_t byBuffEffectType) const {
    // 被动技能类别 (111-114)
    if (byBuffEffectType >= 0x6F && byBuffEffectType <= 0x72u) {
        return 1;
    }
    // 控制类Debuff (121-126)
    if (byBuffEffectType >= 0x79u && byBuffEffectType <= 0x7Eu) {
        return 2;
    }
    // 特殊类别 (131-132)
    if (byBuffEffectType >= 0x83u && byBuffEffectType <= 0x84u) {
        return 3;
    }
    // 效果类型133返回1
    return byBuffEffectType == 133 ? 1 : 0;
}

// IDA: ?CheckPassDebuff@CMover@@QEAAHG@Z @ 0x14036A750
// IDA 精确还原 - 检查被动Debuff
bool CMover::CheckPassDebuff(std::uint16_t nBuffIndex) const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex && m_stBuffState[i].byEffectType == 22) {
            for (int j = 0; j < 3; ++j) {
                if (static_cast<std::uint16_t>(static_cast<int>(m_stBuffState[i].fSkillVal[j])) == nBuffIndex) {
                    return true;
                }
            }
        }
    }
    return false;
}

// IDA: ?IsHit@CMover@@QEAAHXZ @ 0x140367230
// IDA 精确还原 - 检查是否处于受击状态
bool CMover::IsHit() const {
    return m_nMotionClass >= 15 && m_nMotionClass <= 23;
}

// IDA: ?CheckBuffByLocation@CMover@@UEAAXPEAVXArea@@@Z @ 0x14036ABF0
// IDA 精确还原 - 根据位置检查Buff
void CMover::CheckBuffByLocation(XArea* pArea) {
    if (m_nBuffTotalCnt) {
        for (std::uint8_t i = 0; i < 50; ++i) {  // 0x32 = 50
            if (m_stBuffState[i].nBuffIndex) {
                if (!IsCanApplyBuff(m_stBuffState[i].nBuffIndex, pArea)) {
                    ClearBuffStatusBySlot(i, false);
                }
            }
        }
    }
}

// IDA: ?IsCanApplyBuff@CMover@@UEAA_NGPEAVXArea@@@Z (0x14036ACC0)
// IDA 精确还原 - 检查是否可应用Buff（区域限制检查）
bool CMover::IsCanApplyBuff(std::uint16_t nBuffIndex, XArea* pArea) const {
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return false;
    }

    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(nBuffIndex);
    if (!pBuffTable) {
        return false;
    }

    std::uint8_t byAlloLocation = pBuffTable->AllowLocation_Type;

    // 检查是否允许所有位置（位0）
    if ((byAlloLocation & 1) != 0) {
        return true;
    }

    // 获取当前区域
    if (!pArea) {
        pArea = GetArea();
    }

    // 检查迷宫（位1）
    if ((byAlloLocation & 2) != 0) {
        XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
        if (pMaze) {
            return true;
        }
    }

    // 检查区域（位2）- 非战斗区域
    if ((byAlloLocation & 4) != 0) {
        XDistrict* pDistrict = dynamic_cast<XDistrict*>(pArea);
        CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
        if (pDistrict && !pBattleZone) {
            return true;
        }
    }

    // 检查战斗区域（位3）
    if ((byAlloLocation & 8) != 0) {
        CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
        if (pBattleZone) {
            return true;
        }
    }

    return false;
}

// IDA: ?LoadBuffStatus@CMoverEx@@QEAAXHGMEK_N@Z (0x14038B9C0)
// IDA 精确还原 - 加载Buff状态
void CMoverEx::LoadBuffStatus(std::uint16_t nBuffSlot, std::uint16_t nBuffIndex, float fLifeTime, std::uint8_t byCount, std::uint32_t dwOwnerID, bool bShow) {
    if (nBuffSlot >= 50) {
        return;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(nBuffIndex);
    if (!pBuffTable) {
        return;
    }

    tagBUFF_STATE& stBuff = m_stBuffState[nBuffSlot];
    stBuff.nGroupID = pBuffTable->Buff_Group;
    stBuff.nBuffIndex = nBuffIndex;
    stBuff.dwID = dwOwnerID;
    stBuff.byEffectType = pBuffTable->EffectType_01;
    stBuff.byStatType[0] = pBuffTable->EffectType_Status_01;
    stBuff.fSkillVal[0] = static_cast<float>(pBuffTable->Option_Value_01);
    stBuff.iSkillVal = pBuffTable->Option_Value_01;
    stBuff.byStatType[1] = pBuffTable->EffectType_Status_02;
    stBuff.fSkillVal[1] = static_cast<float>(pBuffTable->Option_Value_02);
    stBuff.byStatType[2] = pBuffTable->EffectType_Status_03;
    stBuff.fSkillVal[2] = static_cast<float>(pBuffTable->Option_Value_03);
    stBuff.fGapTime = static_cast<float>(pBuffTable->DotDelay_Time) * 0.001f;
    stBuff.fGapTimeCheck = 0.0f;
    stBuff.byGapApplyCount = (!pBuffTable->EffectType_01 && pBuffTable->DotDelay_Time) ? 1 : 0;
    stBuff.fLifeTime = fLifeTime;
    stBuff.byBuffLV = pBuffTable->Buff_Grade;
    stBuff.byCount = 1;
    stBuff.byBuffType = pBuffTable->Buff_Type;
    stBuff.bySendType = pBuffTable->Buff_Send_Type;
    stBuff.bShow = bShow;
    stBuff.bySystemType = pBuffTable->System_Type;
}

// IDA: ?CheckBuffGrade@CMoverEx@@QEAAHHPEAUTB_BUFF@@@Z (0x14038C8D0)
// IDA 精确还原 - 检查Buff等级是否可替换
int CMoverEx::CheckBuffGrade(int iIndex, TB_BUFF* pNewBuff) {
    if (!pNewBuff) {
        return 0;
    }

    // IDA: 效果类型21(控制类debuff)使用严格小于比较
    if (pNewBuff->EffectType_01 == 21) {
        if (m_stBuffState[iIndex].byBuffLV < pNewBuff->Buff_Grade) {
            return 1;
        }
    } else {
        // IDA: 其他类型使用小于等于比较
        if (m_stBuffState[iIndex].byBuffLV <= pNewBuff->Buff_Grade) {
            return 1;
        }
    }
    return 0;
}

// IDA: ?SetBuffOverlap@CMoverEx@@QEAAHHPEAUTB_BUFF@@KH@Z (0x14038CA00)
// IDA 精确还原 - 设置Buff叠加
int CMoverEx::SetBuffOverlap(int iIndex, TB_BUFF* pNewBuff, std::uint32_t dwOwnerID, int bDontRemoveBuff) {
    // IDA: 检查Buff等级
    if (CheckBuffGrade(iIndex, pNewBuff) != 1) {
        return -1;
    }

    std::uint8_t byCount = m_stBuffState[iIndex].byCount;
    std::uint8_t byOverlap = pNewBuff->Overlap_Count;
    if (byOverlap > 10) {
        byOverlap = 10;
    }
    if (!byOverlap) {
        return -1;
    }

    // IDA: 处理叠加计数
    if (byOverlap != 1) {
        if (byCount + 1 >= byOverlap && pNewBuff->Change_Buff) {
            // IDA: 达到上限且需要变换Buff
            ClearBuffStatusBySlot(iIndex, 0);
            SetBuffStatus(pNewBuff->Change_Buff, dwOwnerID, true);
            return -1;
        }
        if (byCount + 1 > byOverlap) {
            return -1;
        }
    }

    m_stBuffState[iIndex].dwID = dwOwnerID;
    float fTime = static_cast<float>(pNewBuff->Buff_Time) * 0.001f;

    // IDA: 根据叠加类型处理
    if (pNewBuff->Overlap_Type == 0 || pNewBuff->Overlap_Type == 2) {
        // IDA: 类型0和2 - 时间累加
        if (byOverlap == 1) {
            m_stBuffState[iIndex].fLifeTime = fTime;
            m_stBuffState[iIndex].byCount = 1;
        } else {
            // IDA: 时间累加比例
            static const float fTimeRate[] = {0.5f, 0.25f, 0.125f, 0.0625f, 0.03125f,
                                               0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f};
            m_stBuffState[iIndex].fLifeTime += fTime * fTimeRate[byCount - 1];
            ++m_stBuffState[iIndex].byCount;
        }
    } else if (pNewBuff->Overlap_Type == 1) {
        // IDA: 类型1 - 重置时间
        m_stBuffState[iIndex].fLifeTime = fTime;
        if (byOverlap == 1) {
            m_stBuffState[iIndex].byCount = 1;
        } else {
            ++m_stBuffState[iIndex].byCount;
        }
    }

    // IDA: 处理相同Buff和新Buff的情况
    if (m_stBuffState[iIndex].nBuffIndex == pNewBuff->Buff_Index) {
        send_eSUB_CMD_BUFF_UPDATE(this, pNewBuff->Buff_Index,
                                  m_stBuffState[iIndex].fLifeTime,
                                  m_stBuffState[iIndex].byCount,
                                  m_stBuffState[iIndex].dwID,
                                  m_stBuffState[iIndex].bySendType,
                                  m_stBuffState[iIndex].bShow);

        if (!pNewBuff->Overlap_Type && byOverlap > 1 && !bDontRemoveBuff) {
            UpdateBuffAbility(m_stBuffState[iIndex], 0);
        }

        // IDA: 累加技能值
        m_stBuffState[iIndex].fSkillVal[0] += static_cast<float>(pNewBuff->Option_Value_01);
        m_stBuffState[iIndex].fSkillVal[1] += static_cast<float>(pNewBuff->Option_Value_02);
        m_stBuffState[iIndex].fSkillVal[2] += static_cast<float>(pNewBuff->Option_Value_03);
        m_stBuffState[iIndex].iSkillVal += pNewBuff->Option_Value_01;

        return (pNewBuff->Overlap_Type || byOverlap <= 1) ? 1 : 0;
    } else {
        // IDA: 不同Buff - 替换
        if (!bDontRemoveBuff) {
            UpdateBuffAbility(m_stBuffState[iIndex], 0);
        }

        m_stBuffState[iIndex].byCount = 1;
        m_stBuffState[iIndex].fLifeTime = fTime;

        send_eSUB_CMD_BUFF_CHANGE(this, m_stBuffState[iIndex].nBuffIndex,
                                  pNewBuff->Buff_Index,
                                  m_stBuffState[iIndex].fLifeTime,
                                  m_stBuffState[iIndex].byCount,
                                  m_stBuffState[iIndex].dwID,
                                  m_stBuffState[iIndex].bySendType);

        // IDA: 更新Buff状态
        m_stBuffState[iIndex].nBuffIndex = pNewBuff->Buff_Index;
        m_stBuffState[iIndex].byEffectType = pNewBuff->EffectType_01;
        m_stBuffState[iIndex].byStatType[0] = pNewBuff->EffectType_Status_01;
        m_stBuffState[iIndex].fSkillVal[0] = static_cast<float>(pNewBuff->Option_Value_01);
        m_stBuffState[iIndex].iSkillVal = pNewBuff->Option_Value_01;
        m_stBuffState[iIndex].byStatType[1] = pNewBuff->EffectType_Status_02;
        m_stBuffState[iIndex].fSkillVal[1] = static_cast<float>(pNewBuff->Option_Value_02);
        m_stBuffState[iIndex].byStatType[2] = pNewBuff->EffectType_Status_03;
        m_stBuffState[iIndex].fSkillVal[2] = static_cast<float>(pNewBuff->Option_Value_03);
        m_stBuffState[iIndex].fGapTime = static_cast<float>(pNewBuff->DotDelay_Time) * 0.001f;
        m_stBuffState[iIndex].fGapTimeCheck = 0.0f;
        m_stBuffState[iIndex].byBuffLV = pNewBuff->Buff_Grade;
    }

    return 0;
}

// IDA: ?UpdateDefenseType@CMoverEx@@UEAAXXZ @ 0x14037CDF0
// IDA 精确还原 - 更新防御类型
void CMoverEx::UpdateDefenseType() {
    if (IsInvincibleActor()) {
        SetSimpleDefenseType(3);
    } else {
        ApplyDefenseChangeInfo();
        if (m_byDefenseType != 3) {
            // 检查防御类型禁用标志
            if (((m_byDefenseTypeDisableFlag & 1) != 0 && m_byDefenseType == 1) ||
                ((m_byDefenseTypeDisableFlag & 2) != 0 && m_byDefenseType == 2) ||
                ((m_byDefenseTypeDisableFlag & 0x10) != 0 && m_byDefenseType == 5)) {
                SetSimpleDefenseType(0);
            } else if (m_stDefenseChangeInfoByTrigger.fChangeTime <= 0.0f && m_byRestoreDefenceType != 4) {
                m_byRestoreDefenceType = 4;
                if (XActor::GetType() == eActorMonster) {
                    m_fCurSuperArmorGage = m_fMaxSuperArmorGage;
                    send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE(this, m_fCurSuperArmorGage, m_fMaxSuperArmorGage);
                    // ActiveSuperArmorSkill is CMonster virtual function - cast to CMonster
                    CMonster* pMonster = static_cast<CMonster*>(this);
                    pMonster->ActiveSuperArmorSkill();
                }
            }
        }
    }
}

// IDA: ?ProcessExtraMoving@CMover@@UEAAXXZ (0x14036BC20)
// IDA 精确还原 - 处理额外移动
void CMover::ProcessExtraMoving() {
    // 检查是否有额外移动
    if (m_stExtMovingVal.IsZero()) {
        return;
    }

    // 保存当前位置为上一帧位置
    m_vPrevPos = GetPosition();

    // 检查剩余时间
    if (m_stExtMovingVal.fRemainTime <= 0.0f) {
        ReleaseExtraMoving();
        return;
    }

    // 计算目标位置与当前位置的差值
    float fDiffX = m_stExtMovingVal.x - m_vPosition.x;
    float fDiffY = m_stExtMovingVal.y - m_vPosition.y;

    // 获取时间增量
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fDeltaTime = pTimer->GetTimeDifference();

    // 检查是否需要移动（距离足够大）
    if (std::fabsf(fDiffX) >= 3.0f || std::fabsf(fDiffY) >= 3.0f) {
        // 计算X方向的移动量
        float fDeltaX;
        if (fDiffX <= 0.0f) {
            float fMoveX = (fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffX;
            fDeltaX = (fMoveX <= fDiffX) ? fDiffX : fMoveX;
        } else {
            float fMoveX = (fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffX;
            fDeltaX = (fDiffX <= fMoveX) ? fDiffX : fMoveX;
        }

        // 计算Y方向的移动量
        float fDeltaY;
        if (fDiffY <= 0.0f) {
            float fMoveY = (fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffY;
            fDeltaY = (fMoveY <= fDiffY) ? fDiffY : fMoveY;
        } else {
            float fMoveY = (fDeltaTime / m_stExtMovingVal.fMovingTime) * fDiffY;
            fDeltaY = (fDiffY <= fMoveY) ? fDiffY : fMoveY;
        }

        // 更新剩余时间
        m_stExtMovingVal.fRemainTime -= fDeltaTime;

        // 计算目标位置
        hkvVec3 vDestPos = m_vPrevPos + hkvVec3(fDeltaX, fDeltaY, 0.0f);

        // 如果不在飞行状态，获取高度
        if (!IsFlying()) {
            GetHeight(&vDestPos, 200.0f);
        }

        // 检查移动碰撞
        if (CheckMoveCollision(vDestPos)) {
            hkvVec3 vCurPos = GetPosition();
            send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, vCurPos, false);
        } else {
            // 检查目标位置是否有效
            if (!CheckMoveDestPos(&vDestPos, 0, 0)) {
                send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, vDestPos, false);
            }
            // 执行移动 - hkvVec3 继承自 XVec3
            Move(vDestPos);
        }
    } else {
        // 距离太小，清除额外移动
        m_stExtMovingVal.Clear();
    }
}

// IDA: ?ReleaseExtraMoving@CMover@@UEAAXXZ (0x14036C120)
// IDA 精确还原 - 释放额外移动
void CMover::ReleaseExtraMoving() {
    // 检查是否有额外移动
    if (m_stExtMovingVal.IsZero()) {
        return;
    }

    // 如果移动时间很短（0.1秒），直接移动到目标位置
    if (m_stExtMovingVal.fMovingTime == 0.1f) {
        hkvVec3 vExtraPos(m_stExtMovingVal.x, m_stExtMovingVal.y, m_vPosition.z);
        Move(vExtraPos);
    }

    // 清除额外移动数据
    m_stExtMovingVal.Clear();
}

// IDA: ?CheckMonsterInteractObject@CMoverEx@@UEAAHPEAVCMover@@@Z (0x14037FEC0)
// IDA 精确还原 - 检查怪物交互对象
int CMover::CheckMonsterInteractObject(CMover* pMover) {
    if (!pMover) {
        return false;
    }

    // 双方都必须是怪物类型（Type == 2）
    if (XActor::GetType() != 2 || pMover->XActor::GetType() != 2) {
        return false;
    }

    // 检查自身是否是交互对象
    CMonster* pOwner = dynamic_cast<CMonster*>(this);
    if (pOwner && pOwner->IsMonsterInteractObject()) {
        return true;
    }

    // 检查目标是否是交互对象
    CMonster* pTarget = dynamic_cast<CMonster*>(pMover);
    if (pTarget && pTarget->IsMonsterInteractObject()) {
        return true;
    }

    return false;
}

// IDA: ?DebugOut@CMover@@QEAAXPEADZZ (0x140374130)
// IDA 精确还原 - 调试输出
void CMover::DebugOut(const char* szFormat, ...) {
    if (!m_bTraceUser) {
        return;
    }

    char szBuffer[2048];
    char szOutput[2048];

    va_list va;
    va_start(va, szFormat);
    vsprintf_s(szBuffer, sizeof(szBuffer), szFormat, va);
    va_end(va);

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fTime = pTimer->GetTime();

    sprintf_s(szOutput, sizeof(szOutput), "%.3f -> %s", fTime, szBuffer);
    // 使用 LogHelper 输出
    LogHelper::LogDebug("game.debug", "%s", szOutput);
}

// CMoverEx::DebugOut 调用基类实现
void CMoverEx::DebugOut(const char* szFormat, ...) {
    if (!m_bTraceUser) {
        return;
    }

    char szBuffer[2048];
    char szOutput[2048];

    va_list va;
    va_start(va, szFormat);
    vsprintf_s(szBuffer, sizeof(szBuffer), szFormat, va);
    va_end(va);

    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fTime = pTimer->GetTime();

    sprintf_s(szOutput, sizeof(szOutput), "%.3f -> %s", fTime, szBuffer);
    // 使用 LogHelper 输出
    LogHelper::LogDebug("game.debug", "%s", szOutput);
}

// IDA: ?CheckReactionTarget@CMover@@UEAA_NHPEAV1@_N@Z @ 0x14036CE70
// IDA 精确还原 - 检查反应目标
bool CMover::CheckReactionTarget(int iTargetType, CMover* pTargetMover, bool bChekcForChain) {
    // 检查目标是否为系统Actor
    if (pTargetMover) {
        CMoverEx* pMoverEx = dynamic_cast<CMoverEx*>(pTargetMover);
        if (pMoverEx) {
            if (pMoverEx->IsSystemActor()) {
                return false;
            }
        }
    }

    bool bResult = true;
    switch (iTargetType) {
    case 1:
        // 自己
        bResult = (this == pTargetMover);
        break;
    case 2:
        // 除自己外
        if (this != pTargetMover) {
            if (!bChekcForChain) {
                bResult = IsFriend(pTargetMover);
            } else {
                bResult = IsFriendForChain(pTargetMover);
            }
        } else {
            bResult = true;
        }
        break;
    case 3:
        // 敌人
        if (this == pTargetMover) {
            bResult = false;
        } else {
            if (bChekcForChain) {
                bResult = IsEnemyForChain(pTargetMover);
            } else {
                bResult = IsEnemy(pTargetMover);
            }
            if (!bResult) {
                bResult = CheckMonsterInteractObject(pTargetMover);
            }
        }
        break;
    case 4:
        // 除自己外全部
        bResult = (this != pTargetMover);
        break;
    case 6:
        // 友方
        if (this == pTargetMover) {
            bResult = false;
        } else if (bChekcForChain) {
            bResult = IsFriendForChain(pTargetMover);
        } else {
            bResult = IsFriend(pTargetMover);
        }
        break;
    case 7:
        // 队友（包括自己）
        if (this != pTargetMover) {
            bResult = IsParty(pTargetMover);
        } else {
            bResult = true;
        }
        break;
    case 8:
        // 队友（不包括自己）
        if (this == pTargetMover) {
            bResult = false;
        } else {
            bResult = IsParty(pTargetMover);
        }
        break;
    default:
        break;
    }
    return bResult;
}

// IDA: ?Move@CMover@@UEAAGAEAUXVec3@@M@Z @ 0x14036DDD0
// IDA 精确还原 - 移动到指定位置
std::uint16_t CMover::Move(hkvVec3& vNextPos, float fRot) {
    if (m_pArea) {
        // Per PDB: MoveActor 参数为 XVec3& (布局与 hkvVec3 等价)
        m_pArea->MoveActor(this, *reinterpret_cast<XVec3*>(&vNextPos), fRot);
    } else {
        LogHelper::LogDebug("game.contents", "No Area when send move!!");
    }
    return 0;
}

// IDA: ?CheckMoveDestPos@CMover@@UEAA_NAEAVhkvVec3@@HH@Z (0x14036DEE0)
// IDA 精确还原 - 检查目标位置是否可以移动
bool CMover::CheckMoveDestPos(hkvVec3* vDestPos, int bFlying, int bDontCareCurve) {
    // 获取当前区域
    XArea* pArea = GetArea();
    if (!pArea) {
        return true;
    }

    // 获取导航网格
    DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    if (!pNavMesh) {
        return true;
    }

    // 获取碰撞半径
    float fRadius = GetHavokCapsuleRadius();

    // 获取当前位置
    hkvVec3 vPos = GetPosition();
    hkvVec3 vNextPos = *vDestPos;

    // 如果是飞行状态，忽略Z轴
    if (bFlying == 1) {
        hkvVec3 vPosFlat = vPos;
        hkvVec3 vNextPosFlat = vNextPos;
        vPosFlat.z = 0.0f;
        vNextPosFlat.z = 0.0f;

        if (vPosFlat == vNextPosFlat) {
            return true;
        }
    }

    // 检查是否可以直接移动
    // TODO: 需要实现 XMaze::CheckCanDirectMove2
    bool bResult = true;  // 暂时允许所有移动

    if (!bResult) {
        // 计算移动方向
        hkvVec3 vDirection = vNextPos - vPos;
        vDirection.z = 0.0f;
        vDirection.normalizeIfNotZero(0.000001f);
        vDirection *= fRadius;

        // 调整目标位置
        *vDestPos -= vDirection;

        // 获取高度
        if (pNavMesh->GetHeight(vDestPos, 200.0f)) {
            if (bFlying == 1) {
                vDestPos->z = vPos.z;
            }
        } else {
            // 无法获取高度，使用当前位置
            *vDestPos = vPos;
        }
    }

    return bResult;
}

// IDA: ?MessageFunction@CMoverEx@@UEAAXH_J0@Z
// IDA 精确还原 - 处理消息，当 iID==0x10000 且 iParamA==1 时清除动作
void CMoverEx::MessageFunction(int nMsg, __int64 wParam, __int64 lParam) {
    if (nMsg == 0x10000 && wParam == 1) {
        ClearMotion();
    }
}

// IDA: ?GetSectorID@CMonster@@UEAAHXZ (0x140357C40)
// IDA 精确还原 - 获取区域ID
int CMoverEx::GetSectorID() {
    if (m_pSector) {
        return m_pSector->GetSectorBoxUniqueID();
    }
    return -1;
}

// IDA: ?Damage@CMoverEx@@UEAAXAEAUtagACTION_DAMAGE@@HAEA_N@Z (0x140385F70)
// IDA 精确还原 - 处理伤害
void CMoverEx::Damage(tagACTION_DAMAGE& dmgInfo, int nSkillID, bool& bSABreaked) {
    // Check status conditions - if dead or in knockdown/delay die, return
    if (!XActor::IsStatus(4u) || m_eDieType == DIE_TYPE_KNOCKDOWN || m_eDieType == DIE_TYPE_DELAY) {
        return;
    }

    // Check if has area
    if (!GetArea()) {
        return;
    }

    bool bFlyAction = false;

    // Normalize attack rotation to [-180, 180]
    if (dmgInfo.fAttackRot >= -180.0f) {
        if (dmgInfo.fAttackRot > 180.0f) {
            dmgInfo.fAttackRot = dmgInfo.fAttackRot - 360.0f;
        }
    } else {
        dmgInfo.fAttackRot = dmgInfo.fAttackRot + 360.0f;
    }

    // Set hit ID and update quest hited count
    if (dmgInfo.nDamage > 0) {
        SetHitID(dmgInfo.dwID);
        // TODO: Add hited count for quest if this is a player (Type == 0)
    }

    // Enter battle pose if not already
    if (!IsBattlePose()) {
        ChangeBattlePose(true, false);
    }

    // Check phase motion step
    if (m_byPhaseMotionStep == 1 && m_nMotionClass != m_nPlayPhaseMotion && m_nPlayPhaseMotion != -1) {
        SetupPhaseMotion();
        return;
    }

    // Check phase motion step != 2
    if (m_byPhaseMotionStep == 2) {
        return;
    }

    // Set damage and super armor gage
    m_nDamage = dmgInfo.nDamage;
    bool bShowSABreakMotion = false;
    SetCurSuperArmorGage(dmgInfo.fSuperArmorGage);

    // Find attacker actor
    CMoverEx* pActor = nullptr;
    CUser* pAttackerUser = nullptr;

    // TODO: Find actor by hit ID from area
    // pActor = GetArea()->FindActor(m_dwHitID);

    // Get attacker user if actor is monster (Type == 2)
    if (pActor) {
        if (XActor::GetType() != 0) {
            if (pActor->XActor::GetType() == 2) {
                CMoverEx* pOwnerPlayer = pActor->GetOwnerPlayer();
                // pAttackerUser = dynamic_cast<CUser*>(pOwnerPlayer);
            }
        } else {
            // pAttackerUser = dynamic_cast<CUser*>(pActor);
        }
    }

    // Initialize super armor break flag
    bSABreaked = false;

    // Handle defense type
    if (dmgInfo.byDefenseType != 4) {
        // Check super armor break condition
        if (XActor::GetType() == 2 && GetMaxSuperArmorGage() > 0.0f && GetRestoreDefenseType() == 4 && dmgInfo.fSuperArmorGage <= 0.0f) {
            bSABreaked = true;
            SetCurSuperArmorGage(0.0f);
            SetRestoreDefenseType();

            if (m_bySABreakMotionType == 1) {
                m_fSABreakLoopMotionTime = m_fSABreakLoopTime;
                bShowSABreakMotion = true;
            }
            ChangeDefenseTypeForce(0, m_fSABreakTime);

            // Check passive skill and option effect for attacker
            if (pActor /* && !(GetMonsterFlag() & 2) */) {
                if (pAttackerUser) {
                    pAttackerUser->CheckPassiveSkill(1, 36);
                    pAttackerUser->CheckOptionEffectInvoke(EFFECT_CONDITION_MAKE_SA_BREAK, this, static_cast<float>(dmgInfo.nDamage), EFFECT_INVOKE_DONT_CARE);
                } else {
                    pActor->CheckPassiveSkill(1, 36);
                    pActor->CheckOptionEffectInvoke(EFFECT_CONDITION_MAKE_SA_BREAK, this, static_cast<float>(dmgInfo.nDamage), EFFECT_INVOKE_DONT_CARE);
                }
            }
        } else {
            SetSimpleDefenseType(dmgInfo.byDefenseType);
        }
    }

    // Set battle state time for hited user
    // CUser* pHitedUser = dynamic_cast<CUser*>(this);
    // if (pHitedUser) pHitedUser->SetBattleStateTime(7.0f);

    // Update hit damage and check link skill
    if (pActor) {
        if (dmgInfo.nDamage > 0) {
            std::uint8_t byClass = 0;
            if (pAttackerUser) {
                byClass = pAttackerUser->GetClass();
            }
            // UXActorID originID = pActor->GetOriginID();
            // UpdateHitDamage(originID, dmgInfo.nDamage, byClass);
        }

        // TODO: GetLinkSkillDuration not implemented
        // if (pActor->GetLinkSkillDuration() > 0.0f) {
        //     pActor->SetLinkSkillOn(true);
        // }

        // Set battle state time and log for attacker user
        if (pAttackerUser) {
            pAttackerUser->SetBattleStateTime(7.0f);
            // TODO: Log game event
        }

        // Check passive skills for hit target
        // if (!(pActor->GetMonsterFlag() & 2)) {
            CheckPassiveSkill(1, 16);  // OnHit
            CheckPassiveSkill(2, 17);  // OnDamaged
            CheckPassiveSkill(6, 18);  // OnAttacked

            if ((dmgInfo.byDamageFlag & 1) != 0) {
                CheckPassiveSkill(1, 14);  // Critical hit
            } else {
                if ((dmgInfo.byDamageFlag & 4) != 0) {
                    CheckPassiveSkill(1, 21);  // Super armor hit
                }
                // Note: IsBoss_Named_Raid is in CMonster, not CMoverEx
                // if (pActor->IsBoss_Named_Raid()) {
                //     CheckPassiveSkill(1, 53);  // Boss hit
                // }
            }
        // }

        // Determine damage result
        unsigned int dwDamageResult = 1;
        if (IsInvincibleActor()) {
            dwDamageResult = 4;
        } else if ((dmgInfo.byDamageFlag & 1) != 0) {
            dwDamageResult = 2;  // Critical
        } else if ((dmgInfo.byDamageFlag & 4) != 0) {
            dwDamageResult = 3;  // Super armor
        }

        // Check option effect for jump attack
        // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // if (pServer->GetResourceMgr().GetTB_SKILL(nSkillID) && (dwDamageResult == 1 || dwDamageResult == 3)) {
        //     CheckOptionEffectInvoke(EFFECT_CONDITION_DAMAGED_BY_JUMP_ATTACK, pActor, static_cast<float>(dmgInfo.nDamage), EFFECT_INVOKE_DONT_CARE);
        // }
    }

    // Release invoked option effect for attack success
    // TODO: ReleaseInvokedOptionEffect not implemented
    // if ((dmgInfo.byDamageFlag & 1) == 0) {
    //     if (pAttackerUser) {
    //         pAttackerUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_SUCCESS);
    //     } else if (pActor) {
    //         pActor->ReleaseInvokedOptionEffect(EFFECT_CONDITION_ATTACK_SUCCESS);
    //     }
    // }

    // Process HP damage - use 6-argument version
    bool isDamageHP = DamageProcessHP(dmgInfo.dwID, nSkillID, dmgInfo.nDamage, static_cast<std::uint8_t>(dmgInfo.byDamageFlag), 0, dmgInfo.byHitPartsIndex) != 0;

    // Check if not in aura skill (Skill_Type != 8)
    if (!m_pCurSkillTableRef || m_pCurSkillTableRef->Skill_Type != 8) {
        if (isDamageHP) {
            // Handle death
            SetDieReason(1, dmgInfo.nDamage);
            ChangeDefenseTypeForce(0);

            // Check kill effect for attacker
            if (pAttackerUser) {
                pAttackerUser->CheckOptionEffectInvoke(EFFECT_CONDITION_KILL, this, static_cast<float>(dmgInfo.nDamage), EFFECT_INVOKE_DONT_CARE);
            } else if (pActor) {
                pActor->CheckOptionEffectInvoke(EFFECT_CONDITION_KILL, this, static_cast<float>(dmgInfo.nDamage), EFFECT_INVOKE_DONT_CARE);
            }

            // Update monster last damage type
            // UpdateMonsterLastDamageType(dmgInfo, nSkillID);

            // Handle flying death
            if (IsFlying()) {
                m_bFlyDie = true;
                SetDie(12, 0);
                m_fDieDelayTime = 1.0f;
                if (m_fDieDelayMaxTime <= 0.0f) {
                    m_fDieDelayMaxTime = 5.0f;
                }
            } else {
                // Check fly die and motion class
                if (m_bFlyDie && m_nMotionClass == 13) {
                    return;
                }

                // Store previous die type and check die type
                DIE_TYPE ePrevDieType = m_eDieType;
                if (m_eDieType <= DIE_TYPE_DELAY) {
                    // CheckDieType(dmgInfo.byReactionType, dmgInfo.byDamageFlag, dmgInfo.vExtraMove, dmgInfo.nContinueAttack);
                }

                // Clear status if die type changed
                if (ePrevDieType != m_eDieType) {
                    XActor::ClearStatus(2u);
                    XActor::ClearStatus(4u);
                }

                SetDie(12, 0);

                // Handle delay die type
                if (m_eDieType == DIE_TYPE_DELAY) {
                    m_fDieDelayTime = 1.0f;
                    if (m_fDieDelayMaxTime <= 0.0f) {
                        m_fDieDelayMaxTime = 5.0f;
                    }
                } else if (m_eDieType == DIE_TYPE_KNOCKDOWN) {
                    // Handle knockdown
                    m_fDieDelayTime = 0.0f;
                    m_fDieDelayMaxTime = 0.0f;
                    bFlyAction = true;
                    int nMotion = 18;
                    m_nHitStatus = 0;
                    ChangeDefenseTypeForce(0);
                    XActor::ClearStatus(2u);
                    XActor::SetStatus(4u);
                } else {
                    // Normal die
                    m_fDieDelayTime = 0.0f;
                    m_fDieDelayMaxTime = 0.0f;
                    if (m_fDieFadeTime <= 0.0f) {
                        m_fDieFadeTime = 1.5f;
                    }
                }
            }
        } else {
            // Not dead, handle counter animation
            if ((dmgInfo.byDamageFlag & 8) != 0) {
                PlayCounterAnim();
                return;
            }

            // Check defense type and immunity
            if (m_byDefenseType != 3 || ((dmgInfo.byDamageFlag & 0x10) != 0 && !IsImmunityStatus())) {
                // Get damage motion
                int nMotion = GetDamageMotion(dmgInfo.byReactionType, 0.0f, dmgInfo.byAttackCollision, static_cast<EFFECT_INVOKE_TYPE>(dmgInfo.byAttackRank));

                if (bShowSABreakMotion || m_byDefenseType != 5) {
                    // Check attack collision for monster
                    if (XActor::GetType() == 0 && dmgInfo.byAttackCollision == 1 && IsMoveMotion(m_nMotionClass) && (nMotion == 15 || nMotion == 16)) {
                        dmgInfo.byAttackCollision = 0;
                    }

                    // Check status flags
                    if (XActor::IsStatus(0x10000000u) || XActor::IsStatus(0x20000000u)) {
                        dmgInfo.byAttackCollision = 0;
                    }

                    // Set hit call buff status
                    if (m_nHitCallBuffIndex > 0) {
                        SetBuffStatus(m_nHitCallBuffIndex, 0, true);
                    }

                    // Check phase motion again
                    if (m_byPhaseMotionStep == 1 && m_nMotionClass != m_nPlayPhaseMotion && m_nPlayPhaseMotion != -1) {
                        SetupPhaseMotion();
                        return;
                    }

                    // Note: DecEndurance not found - skip for now
                    // DecEndurance(dmgInfo.dwID);

                    // Note: GetStartAnimationInx not found - use nMotion directly
                    int nAnimIndex = nMotion;

                    // Check if reaction is enabled
                    bool bEnableReaction = true;
                    if (m_byDefenseType == 3 || m_byDefenseType == 2 || m_byDefenseType == 5) {
                        bEnableReaction = IsDamageMotionDisplay(dmgInfo.byAttackCollision) != 0;
                    }

                    if (!bEnableReaction) {
                        dmgInfo.vExtraMove = hkvVec3::ZeroVector();
                    } else if ((isDamageHP || !dmgInfo.bPenetrate || dmgInfo.bApplyEffectWhenPenetrate) && bEnableReaction) {
                        // Handle reaction types
                        if ((dmgInfo.byReactionType == 2 || dmgInfo.byReactionType == 3 || dmgInfo.byReactionType == 4) && dmgInfo.vExtraMove.z <= 0.0f) {
                            dmgInfo.byReactionType = 1;
                            m_fJumpDelayTime = 0.0f;
                        }

                        switch (dmgInfo.byReactionType) {
                            case 2:  // Fly up
                                bFlyAction = true;
                                {
                                    float fFlyHeight = dmgInfo.vExtraMove.z;
                                    if (m_fJumpDelta > 0.0f) {
                                        fFlyHeight -= m_fJumpDelta;
                                        if (fFlyHeight < 50.0f) fFlyHeight = 50.0f;
                                    }
                                    InitJumpData(fFlyHeight, 0);
                                    hkvVec3 vDist = dmgInfo.vExtraMove;
                                    vDist.z = 0.0f;
                                    m_fFlyDistance = vDist.getLength();
                                    m_fFlyMoveDist = 0.0f;
                                    m_fFlySpeed = (dmgInfo.fFlySpeed <= 0.0f) ? m_fFlyDistance : dmgInfo.fFlySpeed;
                                }
                                break;

                            case 3:  // Fly back
                                bFlyAction = true;
                                InitJumpData(dmgInfo.vExtraMove.z, 0);
                                {
                                    hkvVec3 vDir = dmgInfo.vExtraMove;
                                    vDir.z = 0.0f;
                                    m_fFlyDistance = vDir.getLength();
                                    m_fFlyMoveDist = 0.0f;
                                    m_fFlySpeed = (dmgInfo.fFlySpeed <= 0.0f) ? m_fFlyDistance : dmgInfo.fFlySpeed;
                                }
                                m_fFlyingStayTime = 0.3f;
                                m_fGroundDownTime = GetGroundDownTime();
                                if (IsFlying() || m_bSkipReplayTime) {
                                    // Move to position
                                    m_fJumpDelayTime = 0.05f;
                                } else {
                                    m_fJumpDelayTime = 0.2f;
                                }
                                SetMoveingInFly(true);
                                break;

                            case 4:  // Fly down
                                bFlyAction = true;
                                {
                                    float fFlyHeight = dmgInfo.vExtraMove.z;
                                    if (m_fJumpDelta > 0.0f) {
                                        fFlyHeight -= m_fJumpDelta;
                                        if (fFlyHeight < 50.0f) fFlyHeight = 50.0f;
                                    }
                                    InitJumpData(fFlyHeight, 0);
                                    hkvVec3 vDist = dmgInfo.vExtraMove;
                                    vDist.z = 0.0f;
                                    m_fFlyDistance = vDist.getLength();
                                    m_fFlyMoveDist = 0.0f;
                                    m_fFlySpeed = (dmgInfo.fFlySpeed <= 0.0f) ? m_fFlyDistance : dmgInfo.fFlySpeed;
                                }
                                break;

                            default:  // Normal hit
                                if (IsFlying() && nMotion >= 18 && nMotion <= 21) {
                                    hkvVec3 vDist = dmgInfo.vExtraMove;
                                    vDist.z = 0.0f;
                                    m_fFlyDistance = vDist.getLength();
                                    m_fFlyMoveDist = 0.0f;
                                    m_fFlySpeed = 0.0f;
                                    SetMoveingInFly(false);
                                    InitJumpData(dmgInfo.vExtraMove.z, 0);
                                    if (m_fFlyVelocity == 0.0f) {
                                        m_fJumpDelayTime = 0.3f;
                                        m_fFlyingStayTime = 0.0f;
                                    } else {
                                        m_fJumpDelayTime = 0.0f;
                                        m_fFlyingStayTime = 0.3f;
                                    }
                                    m_fGroundDownTime = GetGroundDownTime();
                                }
                                break;
                        }

                        if (dmgInfo.byReactionType == 2 || dmgInfo.byReactionType == 4) {
                            m_fFlyingStayTime = 0.3f;
                            m_fGroundDownTime = GetGroundDownTime();
                            SetMoveingInFly(true);
                        }
                    }

                    // Clear flying stay time for knockdown death
                    if (m_eDieType == DIE_TYPE_KNOCKDOWN && GetHP() == 0) {
                        m_fFlyingStayTime = 0.0f;
                    }

                    // Check show SA break motion
                    if (bShowSABreakMotion) {
                        nMotion = 24;
                        m_bShowSABreakMotion = true;
                    } else if (!isDamageHP && dmgInfo.bPenetrate && !dmgInfo.bApplyEffectWhenPenetrate ||
                               nMotion == -1 ||
                               (nMotion != m_nPlayPhaseMotion || m_nMotionClass == m_nPlayPhaseMotion) && !IsDamageMotionDisplay(dmgInfo.byAttackCollision)) {
                        // Skip to end handling
                    } else {
                        // Apply damage motion
                        m_fSkillBlendEndTime = 0.0f;
                        m_bySkillAnimStep = 0;
                        MoveingValueClear();

                        if (XActor::IsStatus(1u)) {
                            CancelAttackFromDamage();
                        }

                        ChangeMotion(nMotion, true, 0);

                        // Set direction for monster
                        if (m_byStandType != 2 && m_byStandType != 3 && XActor::GetType() != 0 && !IsHitDown()) {
                            float fYaw = dmgInfo.fAttackRot + 180.0f;
                            if (fYaw > 180.0f) fYaw -= 360.0f;
                            SetDirectionYaw(fYaw, 2);
                        }

                        // Handle fly action
                        if (bFlyAction || (nMotion >= 18 && nMotion <= 21)) {
                            if (bFlyAction) {
                                m_bLanded = false;
                                m_fFlyYaw = dmgInfo.fAttackRot;
                                SetFlyState(true);
                            }
                            if (m_bCollisionEnable) {
                                m_bRestoreCollision = true;
                                SetCollisionEnable(false, 0);
                            }
                        }
                    }
                }
            }
        }
    }

    // Handle extra moving for non-defense, non-fly cases
    if (!m_byDefenseType && !bFlyAction && (dmgInfo.vExtraMove.x != 0.0f || dmgInfo.vExtraMove.y != 0.0f) && (isDamageHP || !dmgInfo.bPenetrate || dmgInfo.bApplyEffectWhenPenetrate)) {
        // CMoverEx* pAttacker = GetMoverObject(dmgInfo.dwID);
        // if (!pAttacker || !pAttacker->GetCheckAttachToAttacker()) {
            if (m_byStandType != 2 && m_byStandType != 3 && !m_bShowSABreakMotion && !m_bSABreakLoopMotion) {
                // Note: AddExtraMoving requires 3 arguments - use default values
                AddExtraMoving(0.0f, 0.0f, 0.0f);
            }
        // }
    }

    // Clear buff by effect type 0x7B
    int iIndex = FindBuffByEffectType(0x7B, 0);
    if (iIndex != -1) {
        ClearBuffStatusBySlot(iIndex, true);
    }

    // Reset super armor regen time
    m_fRegenSuperArmorTime = m_fDefRegenSuperArmorTime;
    dmgInfo.byDefenseType = m_byDefenseType;
}

// IDA: ?PlayCounterAnim@CMoverEx@@QEAAXXZ (0x140398330)
void CMoverEx::PlayCounterAnim() {
    m_fCounterDuration = 0.0f;
    m_bEnableCounuter = false;
    m_bCounterSuccessFrame = true;

    if (m_szCounterAnimName[0] != '\0') {
        VString strAnimName(m_szCounterAnimName);
        unsigned int dwAnimIndex = GetAnimIndex(strAnimName);
        std::int16_t nMotionClass = AnimKeyToMotion(dwAnimIndex);
        ChangeMotion(nMotionClass, true, 0);
        m_bExistSuboCombo = false;
        m_fSuboComboWaitTime = 0.0f;
        m_fSuboComboCheckTime = 0.0f;
        m_iSuboComboMaxCount = -1;
        m_iSuboComboCheckCount = 0;
    }
}

// IDA: ?GetGroundDownTime@CMoverEx@@QEAAMXZ (0x140381B50)
float CMoverEx::GetGroundDownTime() {
    if (XActor::GetType() != 0) {  // Not player
        return static_cast<float>(rand() % 21) * 0.1f + 1.0f;
    }
    return 3.0f;
}

// IDA: ?CancelAttackFromDamage@CMover@@UEAAXXZ - base empty implementation
// Note: CMonster overrides at 0x14035B520

// IDA: ?ReapllyBuffAll@CMoverEx@@UEAAXXZ (0x14039E7B0)
void CMoverEx::ReapllyBuffAll() {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != 0 &&
            m_stBuffState[i].nBuffIndex != -1 &&
            (m_stBuffState[i].byEffectType == 0 || m_stBuffState[i].byEffectType == 23)) {
            UpdateBuffAbility(m_stBuffState[i], 1);
        }
    }
}

// IDA: ?UpdateSkillAnimInfo@CMoverEx@@QEAAXPEAUTB_SKILL@@@Z (0x14037EE30)
// IDA 精确还原 - 更新技能动画信息
void CMoverEx::UpdateSkillAnimInfo(TB_SKILL* pSkillTableRef) {
    if (!pSkillTableRef) {
        return;
    }

    // Helper lambda to check if string is not "0"
    auto isNotZero = [](const char* str) -> bool {
        if (!str) return false;
        return strcmp(str, "0") != 0;
    };

    // Check if Ani_Res_Extra is not "0"
    if (isNotZero(pSkillTableRef->Ani_Res_Extra)) {
        m_bySkillAnimCount = 4;
        m_bySkillAnimStep = 0;
    } else {
        // Check if Ani_Res_Start is not "0"
        if (isNotZero(pSkillTableRef->Ani_Res_Start)) {
            // Check if Ani_Res_Loop is not "0"
            if (isNotZero(pSkillTableRef->Ani_Res_Loop)) {
                m_bySkillAnimCount = 3;
            } else {
                m_bySkillAnimCount = 2;
            }
            m_bySkillAnimStep = 0;
        } else {
            m_bySkillAnimCount = 1;
            m_bySkillAnimStep = 3;
        }
    }
}

// IDA: ?ChangeAngleAttackName@CMoverEx@@UEAAXEAEAVVString@@@Z (0x140188FA0)
// IDA 精确还原 - 改变角度攻击名称 (基类空实现)
void CMoverEx::ChangeAngleAttackName(int nAngleValue, const char* szAnimName) {
    // Base class implementation is empty - CMonster overrides this
}

// IDA: ?GetAttackInputEvent@CMoverEx@@QEAAPEAVExtraInputTrigger@@PEBD@Z (0x140381540)
// IDA 精确还原 - 获取攻击输入事件
ExtraInputTrigger* CMoverEx::GetAttackInputEvent(const char* szTriggerName) {
    const VAnimationInfo* pActionInfo = GetActionDesc(szTriggerName);
    return reinterpret_cast<ExtraInputTrigger*>(XActionResMgr::RetrieveEvent(2, 0, pActionInfo));
}

// IDA: ?ChargeSkillNextStep@CMoverEx@@UEAAXXZ (0x14037EB60)
void CMoverEx::ChargeSkillNextStep() {
    if (!m_pCurSkillTableRef) {
        return;
    }

    if (m_bySkillChargeStep + 1 == m_bySkillChargeMaxStep) {
        ChargeSkillEnd();
    } else {
        ++m_bySkillChargeStep;
        // IDA: mov eax, [rax+rcx*4+250h] ; Time_Value[m_bySkillChargeStep] as int
        // cvtsi2ss + mulss 0.001f
        m_fSkillChargeChangeTime = static_cast<float>(m_pCurSkillTableRef->Time_Value[m_bySkillChargeStep]) * 0.001f;
        m_bySkillAnimStep = 2;

        const char* pSkillAnimName = GetSkillAnimName(m_pCurSkillTableRef, m_bySkillAnimStep);
        VString strSkillName(pSkillAnimName);
        unsigned int dwNewAnimIndex = GetAnimIndex(strSkillName);
        std::int16_t nMotionClass = AnimKeyToMotion(dwNewAnimIndex);
        ChangeMotion(nMotionClass, true, 3);
    }
}

// IDA: ?SetMoveTrapPos@CMoverEx@@UEAAXH@Z @ 0x140395A20
// IDA 精确还原 - 设置移动陷阱位置
void CMoverEx::SetMoveTrapPos(int nUseZAxis) {
    unsigned int TargetID = GetTargetID();
    CMover* pTarget = GetMoverObject(TargetID);
    if (pTarget) {
        m_vTrapPos = pTarget->GetPosition();
        GetHeight(&m_vTrapPos, 200.0f);
    } else {
        m_vTrapPos = GetPosition();
    }
}

// IDA: ?SetCheckEntityAttach@CMoverEx@@QEAAX_NMMMVhkvVec3@@H@Z @ 0x1403968E0
// IDA 精确还原 - 设置检查实体附加
void CMoverEx::SetCheckEntityAttach(bool bCheck, float fDuration, float fDistance, float fAngle, hkvVec3* vOffset, int iApplyWeightRank) {
    m_bCheckAttachToAttacker = bCheck;
    m_fAttachedDuration = fDuration;
    if (m_bCheckAttachToAttacker) {
        m_fAttachedDistance = fDistance;
        if (vOffset) {
            m_vAttachedOffset = *vOffset;
        }
        m_fAttachedAngle = fAngle;
        m_iApplyWeightRank = iApplyWeightRank;
    }
}

// IDA: ?SetAttached@CMoverEx@@QEAAXPEAV1@@Z
// IDA 精确还原 - 设置附加目标
void CMoverEx::SetAttached(CMoverEx* pAttachedOwner) {
    m_pAttachedOwner = pAttachedOwner;
}

// IDA: ?CheckAttachedEntity@CMoverEx@@QEAAXXZ @ 0x1403969A0
// IDA 精确还原 - 检查附加实体
void CMoverEx::CheckAttachedEntity() {
    // IDA: Get position and orientation
    hkvVec3 vPos = GetPosition();
    float fYaw = GetOrientationYaw();

    // IDA: Create rotation matrix from Euler angles
    hkvMat3 matRot;
    // TODO: VisMath_cl::EulerToMatrix(fYaw, 0.0f, 0.0f, &matRot);

    // IDA: Calculate direction vector (rotated -Y axis)
    hkvVec3 vDir(0.0f, -1.0f, 0.0f);
    // TODO: vDir = matRot * vDir;

    // IDA: Setup attack area info
    tagATTACK_AREA stAreaInfo;
    stAreaInfo.byType = 0;
    stAreaInfo.vCenterPos = vPos;
    stAreaInfo.fAttackerRot = fYaw;
    stAreaInfo.fRadiusStart = 0.0f;
    stAreaInfo.fRadiusEnd = m_fAttachedDistance;
    stAreaInfo.fAngle = m_fAttachedAngle / 2.0f;
    stAreaInfo.vAttackerDir.x = vDir.x;
    stAreaInfo.vAttackerDir.y = vDir.y;
    stAreaInfo.byHitPartsIndex = 0;
    stAreaInfo.fHeightB = vPos.z;
    stAreaInfo.fHeightT = vPos.z + GetCapsuleHeight();

    // IDA: Scan for nearby game objects
    std::vector<CMover*> vecGameObjList;
    XArea* pArea = GetArea();
    if (pArea) {
        pArea->ScanGridOrigin(static_cast<XActor*>(this), 2, 3, vecGameObjList);
    }

    // IDA: Iterate through found objects
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMover* pMover = *it;
        if (!pMover) continue;

        // IDA: RTTI cast to CMoverEx
        CMoverEx* pOtherActor = dynamic_cast<CMoverEx*>(pMover);
        if (!pOtherActor) continue;

        // IDA: Check if valid target
        if (!pOtherActor->IsLive()) continue;
        if (pOtherActor->IsStatus(2)) continue;
        if (pOtherActor->IsHitDown()) continue;
        if (pOtherActor == this) continue;
        if (pOtherActor->GetAttached()) continue;

        // IDA: Check reaction target
        if (CheckReactionTarget(3, pOtherActor, true) != 1) continue;

        // IDA: Check if in attack area
        if (pOtherActor->IsAttackDecision(&stAreaInfo)) continue;

        // IDA: Check weight rank
        // TODO: GetWeightRank not implemented
        // if (pOtherActor->GetWeightRank() >= m_iApplyWeightRank ||
        if (pOtherActor->GetStandType() == 2 ||
            pOtherActor->GetStandType() == 3) {
            // IDA: Skip animation offset and send move packet
            m_bSkipAnimOffset = true;
            send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, m_vPosition, true);
            return;
        }

        // IDA: Attach the entity
        pOtherActor->SetAttached(this);
        pOtherActor->MoveingValueClear();
        pOtherActor->send_eSUB_CMD_MOVE_STOP(pOtherActor);

        // IDA: Send attached move packet
        // TODO: GetAttachedDir, GetAttachedDirDist, send_eSUB_CMD_MOVE_ATTACED_BT
        // hkvVec3 vAttachedDir = pOtherActor->GetAttachedDir();
        // float fAttachedDist = pOtherActor->GetAttachedDirDist();
        // send_eSUB_CMD_MOVE_ATTACED_BT(this, pOtherActor, vAttachedDir, fAttachedDist);
    }
}

// IDA: ?ClearAllAttachedEntity@CMoverEx@@QEAAXXZ @ 0x140397200
// IDA 精确还原 - 清除所有附加实体
void CMoverEx::ClearAllAttachedEntity() {
    // IDA: Scan for nearby game objects
    std::vector<CMover*> vecGameObjList;
    XArea* pArea = GetArea();
    if (pArea) {
        pArea->ScanGridOrigin(static_cast<XActor*>(this), 2, 3, vecGameObjList);
    }

    // IDA: Iterate through found objects
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMover* pMover = *it;
        if (!pMover) continue;

        // IDA: RTTI cast to CMoverEx
        CMoverEx* pOtherActor = dynamic_cast<CMoverEx*>(pMover);
        if (!pOtherActor) continue;

        // IDA: Check if attached
        if (!pOtherActor->GetAttached()) continue;

        // IDA: Detach and send packets
        pOtherActor->SetAttached(nullptr);
        // TODO: send_eSUB_CMD_MOVE_ATTACED_END_BT(this, pOtherActor);
        pOtherActor->send_eSUB_CMD_MOVE_STOP(pOtherActor);
    }
}

// IDA: ?ApplySkillDamageFrame@CMoverEx@@UEAAXGGEAEAVhkvVec3@@MEME_N@Z
// IDA 精确还原 - 应用技能伤害帧（CMoverEx版本）
// TODO: 需要完善实现 - 这是一个复杂的函数，需要完整实现技能伤害逻辑
void CMoverEx::ApplySkillDamageFrame(std::uint16_t nSkillID, std::int16_t nTriggerIdx,
                                        std::uint8_t byAttackTargetCnt, hkvVec3& vPos,
                                        float fAttackRot, std::uint16_t wContinousHit,
                                        std::uint8_t byDamageType, bool bPenetrate, bool bChain) {
    // 获取技能表
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return;

    TB_SKILL* pSkillTable = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
    if (!pSkillTable) return;

    // 获取触发器
    AttackJudgmentTrigger* pTrigger = GetAttackJudgmentEvent(nTriggerIdx);
    if (!pTrigger) return;

    // 计算延迟时间
    float fDelayTime = 0.0f;
    if (pTrigger->StartTime > m_fAnimationTime) {
        fDelayTime = pTrigger->StartTime - m_fAnimationTime;
    }
    if (pTrigger->sAttackType) {
        fDelayTime = 0.0f;
    }

    // 遍历攻击目标
    for (int i = 0; i < static_cast<int>(byAttackTargetCnt) && i < 100; ++i) {
        unsigned int AttackTarget = m_pSkillMgr->GetAttackTarget(i);
        CMoverEx* pMover = static_cast<CMoverEx*>(GetMoverObject(AttackTarget));
        if (pMover) {
            // TODO: 完整实现需要 IsAttackPassive, GeneralSkillDamage, GrapAttach 等
            // 暂时跳过具体伤害计算
        }
    }
}

// IDA: ?UpdateBuffAbility@CMoverEx@@UEAA_NAEAUtagBUFF_STATE@@H@Z (0x14038E5F0)
// IDA 精确还原 - 更新Buff能力值（复杂函数，部分实现）
bool CMoverEx::UpdateBuffAbility(tagBUFF_STATE& stBuff, int bSet) {
    bool bShouldDie = false;

    if (bSet) {
        // 设置Buff效果
        switch (stBuff.byEffectType) {
        case 0:  // 普通属性变化
            for (int j = 0; j < 3; ++j) {
                if (stBuff.byStatType[j]) {
                    SetBuffAbility(stBuff.byStatType[j], stBuff.fSkillVal[j]);
                }
            }
            break;

        case 2:  // 昏迷
            XActor::SetStatus(0x100000u);
            break;

        case 3:  // 睡眠
            XActor::SetStatus(0x200000u);
            break;

        case 4:  // 防御类型变化
            AddDefenseChangeInfo(1, 2, stBuff.nBuffIndex, 0.0f);
            break;

        case 5:  // 无敌
            AddDefenseChangeInfo(1, 3, stBuff.nBuffIndex, 0.0f);
            if (stBuff.byStatType[0] == 1) {
                SetImmunityStatus(1);
            }
            break;

        case 7:  // 技能冷却率
            if (stBuff.byStatType[0] == 66) {
                m_fSkillCoolDownRate = stBuff.fSkillVal[0];
                m_bIgnoreSkillCost = true;
            }
            break;

        case 8:  // 技能反射率
            m_fSkillReflectRate = stBuff.fSkillVal[0] * 0.01f;
            break;

        case 9:  // 吸血
            m_fSkillBloodRate = stBuff.fSkillVal[0] * 0.01f;
            m_nAllowBloodCount = stBuff.byStatType[0];
            break;

        case 11:  // 清除同类型Buff
            for (int k = 0; k < 50; ++k) {
                if (m_stBuffState[k].nBuffIndex &&
                    m_stBuffState[k].byEffectType == static_cast<std::uint8_t>(stBuff.fSkillVal[0])) {
                    ClearBuffStatusBySlot(k, false);
                }
            }
            break;

        case 12:  // 流血Debuff
            m_fSkillBloodDebuffRate = stBuff.fSkillVal[0] * 0.01f;
            m_dwBloodDebuffOwnerID = stBuff.dwID;
            break;

        case 13:  // 固定最大伤害
            ++m_byFixedMaxDamage;
            break;

        case 14:  // 多倍伤害
            m_fMultipleDamageOnce = stBuff.fSkillVal[0] * 0.01f;
            break;

        case 15:  // 多倍吸收SG
            m_fMultipleAbsorbSG = 2.0f;
            break;

        case 16:  // 超级护甲
            m_fBuffSuperArmorRate = stBuff.fSkillVal[0] * 0.01f;
            break;

        case 17:  // 技能冷却减少
            m_fSkillCoolDownRate += stBuff.fSkillVal[0] * 0.01f;
            if (m_pSkillMgr) {
                m_pSkillMgr->ReduceSkillCooltime(m_fSkillCoolDownRate);
            }
            break;

        case 18:  // 体力消耗减少
            m_fDecreaseStaminaRate = stBuff.fSkillVal[0] * 0.01f;
            break;

        case 19:  // 金币增加
            m_fBuffAddGoldRate = stBuff.fSkillVal[0] * 0.01f;
            break;

        case 20:  // FP恢复
            {
                std::tr1::shared_ptr<CGocAttribute> pAttr;
                GetGOC<CGocAttribute>(&pAttr, 0);
                if (pAttr.use_count() != -1 && pAttr) {
                    pAttr->FPRestore(static_cast<std::int16_t>(stBuff.fSkillVal[0]));
                }
            }
            break;

        case 21:  // 强制目标
            m_dwAggroTargetID = stBuff.dwID;
            // TODO: RTTI cast to CMonster and ChangeTarget
            break;

        case 23:  // 百分比属性
            for (int m = 0; m < 3; ++m) {
                std::uint8_t byIndex = stBuff.byStatType[m];
                if (byIndex) {
                    if (stBuff.fSkillVal[m] != 0.0f) {
                        stBuff.fSkillVal[m] = stBuff.fSkillVal[m] / 100.0f;
                    }
                    float fAbility = GetStat(byIndex);
                    stBuff.fSkillVal[m] = stBuff.fSkillVal[m] * fAbility;
                    AddBuffAbility(byIndex, stBuff.fSkillVal[m]);
                }
            }
            break;

        case 25:  // 吸收SG
            m_fSkillAbsorbSGRate = stBuff.fSkillVal[0] * 0.0001f;
            m_nAllowAbsorbSGCount = stBuff.byStatType[0];
            break;

        case 26:  // HP百分比伤害
            {
                float fMaxHP = static_cast<float>(GetMaxHP());
                float fDamage = fMaxHP * stBuff.fSkillVal[0] * 0.01f;
                if (DamageProcessHP(stBuff.dwID, 0, static_cast<int>(fDamage), 0, 0, 0)) {
                    SetDieReason(2, static_cast<int>(fDamage));
                    SetHP(0);
                    bShouldDie = true;
                }
                if (fDamage > 0.0f) {
                    send_eSUB_CMD_BUFF_DAMAGE(this, stBuff.nBuffIndex, static_cast<int>(fDamage), GetHP(), stBuff.dwID);
                }
            }
            break;

        case 28:  // Boss攻击增加
            m_fBossAttackAddRate += stBuff.fSkillVal[0] * 0.01f;
            break;

        case 29:  // Boss受伤减少
            m_fBossAttackedDownRate += stBuff.fSkillVal[0] * 0.01f;
            break;

        case 30:  // 灵魂消耗减少
            m_fSoulCostDownRate += stBuff.fSkillVal[0] * 0.01f;
            break;

        case 31:  // 全攻击增加
            m_fAllAttackAddRate += stBuff.fSkillVal[0] * 0.01f;
            break;

        case 36:  // 动作条件
            SetActionCondition(stBuff.iSkillVal);
            break;

        case 121:
        case 122:
        case 123:
        case 125:
        case 126:
            {
                int v10 = stBuff.byEffectType - 121;
                std::uint32_t dwFlag = 0x1000000 << v10;
                XActor::SetStatus(dwFlag);
            }
            break;

        case 124:  // 冻结
            SetFreeze(true);
            break;

        case 131:
            XActor::SetStatus(0x40000000u);
            break;

        case 132:
            XActor::SetStatus(0x80000000);
            for (int ii = 0; ii < 3; ++ii) {
                if (stBuff.byStatType[ii]) {
                    SetBuffAbility(stBuff.byStatType[ii], stBuff.fSkillVal[ii]);
                }
            }
            break;

        default:
            break;
        }
    } else {
        // 清除Buff效果
        switch (stBuff.byEffectType) {
        case 0:  // 普通属性变化
            for (int jj = 0; jj < 3; ++jj) {
                if ((stBuff.fGapTime <= 0.0f || stBuff.byStatType[jj] < 13 || stBuff.byStatType[jj] > 30) &&
                    stBuff.byStatType[jj]) {
                    ClearBuffAbility(stBuff.byStatType[jj], stBuff.fSkillVal[jj]);
                }
            }
            break;

        case 2:  // 昏迷
            XActor::ClearStatus(0x100000u);
            break;

        case 3:  // 睡眠
            XActor::ClearStatus(0x200000u);
            break;

        case 4:  // 防御类型变化
            RemoveDefenseChangeInfo(1, stBuff.nBuffIndex);
            break;

        case 5:  // 无敌
            RemoveDefenseChangeInfo(1, stBuff.nBuffIndex);
            if (stBuff.byStatType[0] == 1 && !IsHaveImunityInvincibleBuff(stBuff.nBuffIndex)) {
                ClearImmunityStatus(1);
            }
            break;

        case 7:  // 技能冷却率
            m_fSkillCoolDownRate = 0.0f;
            m_bIgnoreSkillCost = false;
            break;

        case 8:  // 技能反射率
            m_fSkillReflectRate = 0.0f;
            break;

        case 9:  // 吸血
            m_fSkillBloodRate = 0.0f;
            m_nAllowBloodCount = 0;
            break;

        case 12:  // 流血Debuff
            m_fSkillBloodDebuffRate = 0.0f;
            m_dwBloodDebuffOwnerID = static_cast<std::uint32_t>(-1);
            break;

        case 13:  // 固定最大伤害
            --m_byFixedMaxDamage;
            break;

        case 14:  // 多倍伤害
            m_fMultipleDamageOnce = 0.0f;
            m_bApplyMultipleDamageOnce = false;
            break;

        case 15:  // 多倍吸收SG
            m_fMultipleAbsorbSG = 1.0f;
            break;

        case 16:  // 超级护甲
            m_fBuffSuperArmorRate = 0.0f;
            break;

        case 17:  // 技能冷却减少
            m_fSkillCoolDownRate -= stBuff.fSkillVal[0] * 0.01f;
            break;

        case 18:  // 体力消耗减少
            m_fDecreaseStaminaRate = 0.0f;
            break;

        case 19:  // 金币增加
            m_fBuffAddGoldRate = 0.0f;
            break;

        case 21:  // 强制目标
            m_dwAggroTargetID = 0;
            break;

        case 23:  // 百分比属性
            if (stBuff.fGapTime == 0.0f) {
                for (int kk = 0; kk < 3; ++kk) {
                    if (stBuff.byStatType[kk]) {
                        ReduceBuffAbility(stBuff.byStatType[kk], stBuff.fSkillVal[kk]);
                    }
                }
            }
            break;

        case 25:  // 吸收SG
            m_fSkillAbsorbSGRate = 0.0f;
            m_nAllowAbsorbSGCount = 0;
            break;

        case 28:  // Boss攻击增加
            m_fBossAttackAddRate -= stBuff.fSkillVal[0] * 0.01f;
            break;

        case 29:  // Boss受伤减少
            m_fBossAttackedDownRate -= stBuff.fSkillVal[0] * 0.01f;
            break;

        case 30:  // 灵魂消耗减少
            m_fSoulCostDownRate -= stBuff.fSkillVal[0] * 0.01f;
            break;

        case 31:  // 全攻击增加
            m_fAllAttackAddRate -= stBuff.fSkillVal[0] * 0.01f;
            break;

        case 36:  // 动作条件
            ClearActionCondition(stBuff.iSkillVal);
            break;

        case 121:
        case 122:
        case 123:
        case 125:
        case 126:
            {
                int v17 = stBuff.byEffectType - 121;
                std::uint32_t dwStatus = 0x1000000 << v17;
                XActor::ClearStatus(dwStatus);
            }
            break;

        case 124:  // 冻结
            SetFreeze(false);
            break;

        case 131:
            XActor::ClearStatus(0x40000000u);
            break;

        case 132:
            XActor::ClearStatus(0x80000000);
            for (int mm = 0; mm < 3; ++mm) {
                if (stBuff.byStatType[mm]) {
                    ClearBuffAbility(stBuff.byStatType[mm], stBuff.fSkillVal[mm]);
                }
            }
            break;

        default:
            break;
        }
    }

    m_fMoveSpeed = GetMoveSpeed();
    return bShouldDie;
}

// IDA: ?SetBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403900C0)
// IDA 精确还原 - 设置Buff能力值
void CMoverEx::SetBuffAbility(int iType, float fValue) {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);
    if (pAttr.use_count() != -1 && pAttr) {
        int iStatType = pAttr->UpdateBuffEffectStat(iType, fValue, true, false);
        m_bChangedStat = true;
        if (iStatType == 1) {
            float fHP = GetStat(1);
            SetHpInfo(static_cast<int>(fHP));
        }
    }
}

// IDA: ?SetImmunityStatus@CMover@@QEAAXK@Z (0x1402A4F90)
// IDA 精确还原 - 设置免疫状态
void CMover::SetImmunityStatus(std::uint32_t dwStatus) {
    m_dwImmunityStatus |= dwStatus;
}

// IDA: ?ClearBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403901A0)
// IDA 精确还原 - 清除Buff能力值
void CMoverEx::ClearBuffAbility(int iType, float fValue) {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);
    if (pAttr.use_count() != -1 && pAttr) {
        m_bChangedStat = true;
        float fNegValue = -fValue;
        if (pAttr->UpdateBuffEffectStat(iType, fNegValue, true, true) == 1) {
            float fHP = GetStat(1);
            SetHpInfo(static_cast<int>(fHP));
        }
    }
}

// IDA: ?ReduceBuffAbility@CMoverEx@@UEAAXHM@Z (0x140390330)
// IDA 精确还原 - 减少Buff能力值
void CMoverEx::ReduceBuffAbility(unsigned int iType, float fValue) {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);
    if (pAttr.use_count() != -1 && pAttr) {
        m_bChangedStat = true;
        float fNegValue = -fValue;
        pAttr->UpdateAddStat(iType, fNegValue, true);
    }
}

// IDA: ?SetFreeze@CMoverEx@@UEAAXH@Z (0x140390A10)
// IDA 精确还原 - 设置冻结状态
void CMoverEx::SetFreeze(bool bFreeze) {
    StopMoving(true);
    if (bFreeze) {
        AnimPause();
        XActor::SetStatus(0x8000000u);
        InitJumpData(0.0f, true);
    } else {
        XActor::ClearStatus(0x8000000u);
        PostSkillProcess();
        ChangeMotion(1, true, 6);
    }
}

// IDA: ?send_eSUB_CMD_BUFF_DAMAGE@CMover@@QEAAXPEAV1@GHHK@Z (0x140372F60)
// IDA 精确还原 - 发送Buff伤害包
void CMover::send_eSUB_CMD_BUFF_DAMAGE(CMover* pMover, std::int16_t wBuffID, int nDamage, int nCurHP, std::uint32_t dwOwnerID) {
    XSendPacket xPacket(6, 0x17);
    UXActorID uxActorID = pMover->GetActorID();
    int nQuestID = CQuestCondition::GetQuestID(uxActorID);
    xPacket << nQuestID;
    xPacket << wBuffID;
    xPacket << nDamage;
    xPacket << nCurHP;
    xPacket << dwOwnerID;
    CGocNetwork::SendBroadCast(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);
    DebugOut("send_eSUB_CMD_BUFF_DAMAGE>> %d, %d, %d", static_cast<std::uint16_t>(wBuffID), nDamage, dwOwnerID);
}

// IDA: ?SendUpdateBuffAbility@CMoverEx@@UEAAXXZ (0x14038D780)
// IDA 精确还原 - 发送更新Buff能力值
void CMoverEx::SendUpdateBuffAbility() {
    if (m_bChangedStat) {
        m_bChangedStat = false;
        std::tr1::shared_ptr<CGocAttribute> pAttr;
        GetGOC<CGocAttribute>(&pAttr, 0);
        if (pAttr.use_count() != -1 && pAttr) {
            // IDA: v6->__vftable[4].Finalize(v6) - 调用虚函数表第5个槽位
            // TODO: 确认 Finalize 虚函数的具体实现
            pAttr->CalculateChangedEffect(true);
        }
    }
}

// IDA: ?AddBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403902A0)
// IDA 精确还原 - 添加Buff能力值
void CMoverEx::AddBuffAbility(unsigned int iType, float fValue) {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);
    if (pAttr.use_count() != -1 && pAttr) {
        m_bChangedStat = true;
        pAttr->UpdateAddStat(iType, fValue, true);
    }
}

// IDA: ?GetActionDesc@CMover@@QEAAPEBVVAnimationInfo@@PEBD@Z (0x14036C920)
// IDA 精确还原 - 获取动画描述信息
const VAnimationInfo* CMover::GetActionDesc(const char* szAnimName) const {
    // Check if we have akashic action info and the name matches
    if (m_pAkashicActionInfo) {
        if (strcmp(szAnimName, reinterpret_cast<const char*>(m_pAkashicActionInfo)) == 0) {
            return m_pAkashicActionInfo;
        }
    }

    // Check if we have action resource
    if (!m_pActionResource) {
        return nullptr;
    }

    // Get action desc from action manager
    XGameServer* pServer = XGameServer::Instance();
    return pServer->GetActionManager().GetActionDesc(m_pActionResource, szAnimName);
}

// IDA: ?GetAnimStirng@CMover@@QEAAPEADK@Z (0x1403688D0)
// 精确还原: 根据动画键获取动画名称字符串
const char* CMover::GetAnimStirng(std::uint32_t dwAnimKey) {
    if (!m_mapAnimInfoString) {
        return nullptr;
    }

    auto it = m_mapAnimInfoString->find(dwAnimKey);
    if (it == m_mapAnimInfoString->end()) {
        return nullptr;
    }

    return it->second.AsChar();
}

// IDA: ?GetAnimIndex@CMover@@QEAAKVVString@@@Z (0x140368960)
// 精确还原: 根据动画名称获取动画索引
std::uint32_t CMover::GetAnimIndex(const VString& strAnimName) {
    if (!m_mapAnimInfoKey) {
        return static_cast<std::uint32_t>(-1);
    }

    auto it = m_mapAnimInfoKey->find(strAnimName);
    if (it == m_mapAnimInfoKey->end()) {
        return static_cast<std::uint32_t>(-1);
    }

    return it->second;
}

// IDA: ?AnimKeyToMotion@CMover@@QEAAFK@Z (0x140368A80)
// 精确还原: 将动画键转换为动作类 (除以 1000)
std::int16_t CMover::AnimKeyToMotion(std::uint32_t dwAnimKey) {
    return static_cast<std::int16_t>(dwAnimKey / 1000);
}

// IDA: ?GetStat@CMover@@QEAAMH@Z (0x140166360)
// IDA 精确还原 - 获取属性值
float CMover::GetStat(int iIndex) {
    return m_fAbility[iIndex];
}

// IDA: ?GetCurrentAnimationLength@CMover@@QEAAMXZ (0x140368B90)
// 精确还原: 获取当前动画长度
float CMover::GetCurrentAnimationLength() const {
    if (m_pCurMotionEvent) {
        return m_pCurMotionEvent->fAnimationLength;
    }
    return 0.0f;
}

// IDA: ?SetMoveSpeed@CMover@@QEAAXM@Z
void CMover::SetMoveSpeed(float fSpeed) {
    m_fMoveSpeed = fSpeed;
}

// IDA: ?SetDieFadeTime@CMover@@QEAAXM@Z @ 0x1403A2370
void CMover::SetDieFadeTime(float fVal) {
    m_fDieFadeTime = fVal;
}

// IDA: ?GetMoveSpeed@CMover@@QEAAMXZ
float CMover::GetMoveSpeed() {
    return m_fMoveSpeed;
}

// IDA: ?IsMoving@CMover@@QEAAHXZ @ 0x14027A610
// IDA 精确还原 - 检查是否移动中
bool CMover::IsMoving() {
    return m_fMoving != 0;
}

// IDA: ?GetPosition@CMover@@QEAA?AVhkvVec3@@XZ
// IDA 精确还原 - 获取位置（继承自 VisObject3D_cl）
hkvVec3 CMover::GetPosition() const {
    return VisObject3D_cl::GetPosition();
}

// IDA: ?GetMotionClass@CMover@@QEAAFXZ
std::int16_t CMover::GetMotionClass() {
    return m_nMotionClass;
}

// IDA: ?SetAnimSpeed@CMover@@QEAAXM@Z
void CMover::SetAnimSpeed(float fSpeed) {
    m_fAnimSpeed = fSpeed;
}

// IDA: ?InitFunction@CMover@@UEAAXXZ @ 0x140366C00
// IDA 精确还原 - 初始化函数
void CMover::InitFunction() {
    m_fLastUpdateTime = 0.0f;
    m_fLastDebugTime = 0.0f;
}

// IDA: ?Destroy@CMover@@UEAAXXZ @ 0x140366A80
// IDA 精确还原 - 清理 CMover 资源
void CMover::Destroy() {
    Reset();
    AllBuffClear(false);

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

    m_sPublicTransportPath.Stop();
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

// VisTypedEngineObject_cl::TriggerScriptEvent_2
void VisTypedEngineObject_cl::TriggerScriptEvent_2(const char* pEvent, const char* pParam) {
    // TODO: Implement - Vision Engine method
}

// IDA: ?ResetAkashicActionInfo@CMover@@QEAAXXZ @ 0x14036C610
// IDA 精确还原 - 重置 Akashic 动作信息
void CMover::ResetAkashicActionInfo() {
    if (m_pAkashicActionInfo) {
        // TODO: VArray::RemoveAll for arOffsetDeltaFrames, arTranslationFrames, arTriggers
        delete m_pAkashicActionInfo;
        m_pAkashicActionInfo = nullptr;
    }
}

// Additional CMover methods
void CMover::SetInvincibleActor(bool bInvincible) {
    m_bInvincibleActor = bInvincible;
}

void CMover::SetMoveingInFly(int nFlag) {
    m_bMoveingInFly = nFlag != 0;
}

// IDA: ?SceneChanged@CMover@@UEAAXXZ @ 0x14036CAB0
// IDA 精确还原 - 场景改变回调
void CMover::SceneChanged() {
    MoveingValueClear();
}

void CMoverEx::MoveingValueClear() {
    m_fMoving = 0;
    m_stMovePos.Clear();
    m_stMoveGap.Clear();
    m_stMoveOffset.Clear();
}

// Note: IsSystemActor is implemented in MoverEx.cpp

// IDA: ?AllBuffClear@CMover@@QEAAXE@Z (0x14036aa40)
// IDA 精确还原 - 清除所有Buff
void CMover::AllBuffClear(std::uint8_t byReason) {
    if (!m_nBuffTotalCnt) {
        return;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    for (std::uint8_t i = 0; i < 50; ++i) {
        tagBUFF_STATE& stBuff = m_stBuffState[i];

        if (stBuff.IsLife()) {
            // 活跃Buff - 检查是否可清除
            if (!byReason || IsClearBuff(stBuff.nBuffIndex, byReason)) {
                ClearBuffStatusBySlot(i, false);
            }
        } else if (stBuff.nBuffIndex) {
            // 非活跃但有效Buff ID - 检查永久Buff
            TB_BUFF* pBuffRef = pServer->GetResourceMgr().GetTB_BUFF(stBuff.nBuffIndex);
            if (pBuffRef) {
                if (!pBuffRef->Buff_Time && IsClearBuff(stBuff.nBuffIndex, byReason)) {
                    ClearBuffStatusBySlot(i, false);
                }
            }
        }
    }
}

// IDA: ?UpdateBuffCount@CMover@@QEAAXEH@Z (0x14036AE80)
// IDA 精确还原 - 更新Buff计数
void CMover::UpdateBuffCount(std::uint8_t byBuffType, int nAddCount) {
    if (nAddCount <= 0) {
        // 减少计数，确保不为负
        int newTotal = m_nBuffTotalCnt + nAddCount;
        m_nBuffTotalCnt = (newTotal >= 0) ? newTotal : 0;

        if (byBuffType == 0) {
            // Buff类型
            int newBuff = m_nBuffCnt + nAddCount;
            m_nBuffCnt = (newBuff >= 0) ? newBuff : 0;
        } else if (byBuffType == 1) {
            // Debuff类型
            int newDebuff = m_nDebuffCnt + nAddCount;
            m_nDebuffCnt = (newDebuff >= 0) ? newDebuff : 0;
        }
    } else {
        // 增加计数
        m_nBuffTotalCnt += nAddCount;
        if (byBuffType == 0) {
            m_nBuffCnt += nAddCount;
        } else if (byBuffType == 1) {
            m_nDebuffCnt += nAddCount;
        }
    }
}

bool CMover::IsKnockDown() const {
    // IDA: ?IsKnockDown@CMover@@QEAAHXZ @ 0x1403671C0
    // IDA 精确还原 - 判断是否处于击倒状态
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 3;
    }
    return m_nMotionClass >= 18 && m_nMotionClass <= 21;
}

// IDA: ?IsGazeMoving@CMover@@QEAAHXZ @ 0x140375200
// IDA 精确还原 - 获取凝视移动状态
int CMover::IsGazeMoving() {
    return m_bGazeMoving;
}

std::uint32_t CMoverEx::GetID() {
    return XActor::GetActorID().dwActorID;
}

// Per IDA 0x140367080: CMover::IsFlying
// 精确还原 - 检查是否在飞行状态
// 逻辑:
//   1. 如果 m_fForcedStateApplyTime > 0，返回 m_uiForcedState == 1
//   2. 如果 m_bLanded 为 true，返回 false
//   3. 获取当前位置，检查高度差是否超过 5 单位
bool CMover::IsFlying() const {
    // IDA: 检查强制状态
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 1;
    }

    // IDA: 检查是否已着陆
    if (m_bLanded) {
        return false;
    }

    // IDA: 获取当前位置并检查高度
    hkvVec3 vPos = GetPosition();
    float fZ = vPos.z;

    hkvVec3 vGroundPos = vPos;
    if (const_cast<CMover*>(this)->GetHeight(&vGroundPos, 300.0f)) {
        if (fZ > vGroundPos.z + 5.0f) {
            return true;
        }
    } else {
        // IDA: DebugOut for failed GetHeight
        // DebugOut("IsFlying>> GetHeight failed. (%.2f,%.2f,%.2f)", vPos.x, vPos.y, vPos.z);
    }

    return false;
}

// IDA: ?GetItemRateResultWeapon@CMover@@QEAAHEV?$shared_ptr@VCGocAttribute@@@tr1@std@@_N@Z @ 0x140367760
// IDA 精确还原 - 获取武器物品掉落率结果
std::uint32_t CMover::GetItemRateResultWeapon(std::uint8_t byTargetLevel, std::tr1::shared_ptr<CGocAttribute> pAttr, bool bCritical) {
    if (!pAttr) {
        return 0;
    }

    const SItemRateInfo* pItemRateInfo = pAttr->GetItemRateInfo(1);  // WEAPON_SLOT_TYPE = 1
    if (!pItemRateInfo) {
        return 0;
    }

    int iLevelGap = byTargetLevel - pItemRateInfo->wItemLevel;
    if (iLevelGap < 0) {
        iLevelGap = 0;
    }

    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        return 0;
    }

    TB_WEAPON_RATE* tbWeaponRate = pServer->GetResourceMgr().GetTB_WEAPON_RATE(iLevelGap);
    if (!tbWeaponRate) {
        return 0;
    }

    TB_ITEM_RANK_RATE* tbRankRate = pServer->GetResourceMgr().GetTB_ITEM_RANK_RATE(pItemRateInfo->byItemRank);
    if (!tbRankRate) {
        return 0;
    }

    int iItemValue = bCritical
        ? (pItemRateInfo->iItemValueCritical + pItemRateInfo->iItemValue)
        : pItemRateInfo->iItemValue;

    float fResult = (float)iItemValue * tbWeaponRate->Weapon_Mob_Applyrate * tbRankRate->Item_Rank_Rate - (float)iItemValue;
    return static_cast<std::uint32_t>(fResult);
}

// IDA: ?GetItemRateResultGear@CMover@@QEAAHEV?$shared_ptr@VCGocAttribute@@@tr1@std@@@Z @ 0x140367780
// IDA 精确还原 - 获取装备道具比率结果
std::uint32_t CMover::GetItemRateResultGear(std::uint8_t byTargetLevel, std::shared_ptr<CGocAttribute> pAttr) {
    int arSlotType[4] = {151, 161, 171, 181};  // 装备槽类型
    int iResult = 0;

    for (int i = 0; i < 4; ++i) {
        const SItemRateInfo* pItemRateInfo = pAttr->GetItemRateInfo(arSlotType[i]);
        if (pItemRateInfo) {
            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            TB_GEAR_RATE* tbGearRate = pServer->GetResourceMgr().GetTB_GEAR_RATE(0);
            if (tbGearRate) {
                TB_ITEM_RANK_RATE* tbRankRate = pServer->GetResourceMgr().GetTB_ITEM_RANK_RATE(pItemRateInfo->byItemRank);
                if (tbRankRate) {
                    iResult += static_cast<int>(
                        static_cast<float>(pItemRateInfo->iItemValue) * tbGearRate->Gear_Mob_Applyrate * tbRankRate->Item_Rank_Rate
                        - static_cast<float>(pItemRateInfo->iItemValue)
                    );
                }
            }
        }
    }
    return static_cast<std::uint32_t>(iResult);
}

// IDA: ?GetEmptyBuffSlot@CMover@@QEAAHXZ (0x14036A810)
// IDA 精确还原 - 获取空的Buff槽位
int CMover::GetEmptyBuffSlot() const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == 0) {
            return i;
        }
    }
    return 0;  // IDA: 返回0表示无空槽位（而非-1）
}

// IDA: ?FindBuffByEffectType@CMover@@QEAAHEG@Z (0x14036A560)
// 精确还原 - 按效果类型查找Buff
int CMover::FindBuffByEffectType(std::uint8_t byBuffEffect, std::uint16_t nExceptBuffIndex) const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != nExceptBuffIndex &&
            m_stBuffState[i].nBuffIndex != 0 &&
            m_stBuffState[i].byEffectType == byBuffEffect) {
            return i;
        }
    }
    return -1;
}

// IDA: ?FindBuffByGroupID@CMover@@QEAAHGK@Z (0x14036A4C0)
// IDA 精确还原 - 按组ID查找Buff
int CMover::FindBuffByGroupID(std::uint16_t nGroupID, std::uint32_t dwAttackerID) const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nGroupID == nGroupID) {
            // IDA: 效果类型10需要匹配攻击者ID
            if (m_stBuffState[i].byEffectType != 10) {
                return i;
            }
            if (m_stBuffState[i].dwID == dwAttackerID) {
                return i;
            }
        }
    }
    return -1;
}

bool CMover::IsImmunityStatus() const {
    return m_dwImmunityStatus != 0;
}

// IDA: ?CanUseItem@CMover@@QEAAHKAEAK@Z (0x14036B530)
// 精确还原 - 检查是否可以使用物品
bool CMover::CanUseItem(std::uint32_t dwID, std::uint32_t& dwError) const {
    XGameServer* pServer = XGameServer::Instance();
    TB_ITEM* pItemRef = pServer->GetResourceMgr().GetTB_ITEM(dwID);
    XArea* pArea = GetArea();

    if (!pItemRef) {
        return false;
    }

    TB_ITEM_CLASSIFY* pItemClass = pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pItemRef->Item_Classify_Index);
    if (!pItemClass) {
        return false;
    }

    // Item_Sub_Type == 2 (consumable)
    if (pItemRef->Item_Sub_Type == 2) {
        if (pItemRef->Item_Effect_Type == 1) {
            // Self-use revival item
            if (!pArea || !pArea->IsRevive()) {
                dwError = 52607;
                return false;
            }
            if (!IsDie()) {
                dwError = 52608;
                return false;
            }
        } else if (pItemRef->Item_Effect_Type == 2) {
            // Party-use item
            if (!pArea || !pArea->IsRevive()) {
                dwError = 52607;
                return false;
            }

            // Check party for dead users
            std::tr1::shared_ptr<CGocParty> pParty;
            const_cast<CMover*>(this)->GetGOC<CGocParty>(&pParty, 0);
            if (pParty && pParty->IsParty()) {
                if (!pParty->IsExistDeadUser()) {
                    dwError = 52604;
                    return false;
                }
            }

            // Check force for dead users
            std::tr1::shared_ptr<CGocForce> pForce;
            const_cast<CMover*>(this)->GetGOC<CGocForce>(&pForce, 0);
            if (pForce && pForce->IsParty()) {
                if (!pForce->IsExistDeadUser()) {
                    dwError = 52604;
                    return false;
                }
            }

            // If not in party and not dead
            if ((!pParty || !pParty->IsParty()) && (!pForce || !pForce->IsParty())) {
                if (!IsDie()) {
                    dwError = 52608;
                    return false;
                }
            }
        }
    } else if (pItemRef->Item_Sub_Type == 1) {
        // Buff item
        TB_BUFF* pBuff = pServer->GetResourceMgr().GetTB_BUFF(pItemRef->Item_Effect_ID);
        if (!pBuff) {
            dwError = 52607;
            return false;
        }

        if (pItemRef->Item_Effect_Type == 1) {
            // Self-use buff
            if (pBuff->EffectType_01 == 1) {
                if (!pArea || !pArea->IsRevive()) {
                    dwError = 52607;
                    return false;
                }
                // Check if already has revival buff
                if (const_cast<CMover*>(this)->FindBuffByEffectType(1, 0) != -1) {
                    dwError = 52609;
                    return false;
                }
            }
        } else if (pItemRef->Item_Effect_Type == 2) {
            // Party-use buff
            if (pBuff->EffectType_01 == 1) {
                if (!pArea || !pArea->IsRevive()) {
                    dwError = 52607;
                    return false;
                }

                std::tr1::shared_ptr<CGocParty> pParty;
                const_cast<CMover*>(this)->GetGOC<CGocParty>(&pParty, 0);
                if (pParty && pParty->IsParty()) {
                    // TODO: CGocParty needs NeedReviveBuffUser or equivalent check
                    // For now, check IsExistDeadUser
                    if (!pParty->IsExistDeadUser()) {
                        dwError = 52604;
                        return false;
                    }
                }

                std::tr1::shared_ptr<CGocForce> pForce;
                const_cast<CMover*>(this)->GetGOC<CGocForce>(&pForce, 0);
                if (pForce && pForce->IsParty()) {
                    if (!pForce->NeedReviveBuffUser()) {
                        dwError = 52604;
                        return false;
                    }
                }

                // If not in party, check self buff
                if ((!pParty || !pParty->IsParty()) && (!pForce || !pForce->IsParty())) {
                    if (const_cast<CMover*>(this)->FindBuffByEffectType(1, 0) != -1) {
                        dwError = 52609;
                        return false;
                    }
                }
            }
        }
    }

    // TODO: XArea needs IsModeCondition method
    // return pArea ? pArea->IsModeCondition() : true;
    return true;
}

std::uint32_t CMover::GetID() {
    return XActor::GetActorID().dwActorID;
}

// IDA: ?CheckPassiveSkillByHit@CMoverEx@@UEAAXPEAVCMoverEx@@PEAUTB_SKILL@@E@Z
// Base class virtual function - default implementation does nothing
// Overridden by CMonster (0x140361B10) and CUser (0x1406F0480)
void CMoverEx::CheckPassiveSkillByHit(CMoverEx* pMover, TB_SKILL* pSkillTable, std::uint8_t byResult) {
    // Base class does nothing - derived classes implement specific behavior
}

// IDA: ?GetSGAbsorbRate@CMover@@QEAAMXZ (0x14036e200)
// IDA 精确还原 - 获取SG吸收率
float CMover::GetSGAbsorbRate() {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);

    if (!pAttr) {
        return 0.0f;
    }

    return pAttr->GetStatusTable()->Con_SG_Absorb_Rate;
}

// IDA: ?SetContinousCost@CMover@@QEAAXHM@Z (0x14036e330)
// IDA 精确还原 - 设置持续消耗
void CMover::SetContinousCost(int iIndex, float fVal) {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);

    if (pAttr) {
        pAttr->SetContinousCost(iIndex, fVal);
    }
}

// IDA: ?GetDecreaseStaminaRate@CMover@@QEAAMXZ (0x1402C7EE0)
// IDA 精确还原 - 获取耐力减少率
float CMover::GetDecreaseStaminaRate() const {
    return m_fDecreaseStaminaRate;
}

// IDA: ?GetIgnoreSkillCost@CMover@@QEAAHXZ (0x1402C7F00)
// IDA 精确还原 - 获取忽略技能消耗标志
int CMover::GetIgnoreSkillCost() const {
    return m_bIgnoreSkillCost;
}

// IDA: ?SetAllowPassiveType@CMover@@QEAAXH@Z (0x14036e3b0)
// IDA 精确还原 - 设置允许的被动技能类型
void CMover::SetAllowPassiveType(int nType) {
    m_setAllowPassiveType.insert(nType);
}

// IDA: ?SendUpdateStat@CMover@@QEAAXH@Z (0x14036e4a0)
// IDA 精确还原 - 发送属性更新
void CMover::SendUpdateStat(std::uint32_t iIndex) {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);

    if (pAttr) {
        pAttr->SendUpdateStat(static_cast<int>(iIndex));
    }
}

// IDA: ?InitialObjectInfo@CMover@@UEAAXKHVhkvVec3@@M@Z (0x140366AD0)
// IDA 精确还原 - 初始化对象信息
void CMover::InitialObjectInfo(std::uint32_t dwID, int nTableIdx, hkvVec3 vPos, float fRot) {
    // IDA: Get CGocAttribute component and calculate ability
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);

    if (pAttr) {
        m_fAbility = pAttr->GetFinalStats();
    }
}

void CMover::ThinkFunction() {
    // IDA: ?ThinkFunction@CMover@@UEAAXXZ @ 0x140366FA0
    m_bAnimChanged = 0;
    if (m_pSkillMgr) {
        m_pSkillMgr->ThinkFunction();
    }
    if (m_bTraceUser) {
        send_eSUB_CMD_MOVE_TRACE(this, 0.0f);
        float fDebugTime = m_fLastDebugTime + 0.5f;
        VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
        if (pTimer && pTimer->GetTime() > fDebugTime) {
            VDefaultTimer* pTimer2 = ThreadLocalData::GetTimer();
            m_fLastDebugTime = pTimer2->GetTime();
        }
    }
    VDefaultTimer* pTimer3 = ThreadLocalData::GetTimer();
    float fDeltaTime = pTimer3 ? pTimer3->GetTimeDifference() : 0.0f;
    CheckDelayedProjectile(fDeltaTime);
    CheckContinuousMelee(fDeltaTime);
}

// IDA: ?AnimKeyToMotion@CMover@@QEAAFK@Z @ 0x140368A90
// IDA 精确还原 - 从动画Key计算Motion (dwAnimKey / 1000)
std::int16_t CMoverEx::AnimKeyToMotion(std::uint32_t dwKey) {
    return static_cast<std::int16_t>(dwKey / 1000);
}

// IDA: ?send_eSUB_CMD_MOVE_IDLE@CMover@@QEAAXPEAV1@M@Z @ 0x14036FD50
// IDA 精确还原 - 发送空闲移动包
void CMover::send_eSUB_CMD_MOVE_IDLE(CMover* pMover, float fMoveDelayTime) {
    XSendPacket xPacket(5, 0x0C);  // main=5, sub=12

    UXActorID actorID = pMover->GetActorID();
    int QuestID = CQuestCondition::GetQuestID(actorID);

    xPacket << QuestID;

    hkvVec3 vPos = pMover->GetPosition();
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;

    float fYaw = pMover->GetMovingYaw();
    xPacket << fYaw;

    int nAnimIdx = pMover->GetAnimationIdx();
    xPacket << nAnimIdx;

    char byDefaultAnimStep = pMover->GetDefaultAnimStep();
    xPacket << byDefaultAnimStep;

    xPacket << fMoveDelayTime;

    CGocNetwork::SendBroadCastAfterLoading(this, xPacket, static_cast<E_BROADCAST_TYPE>(0));
}

// IDA: ?SetSlowTime@CMover@@QEAAXMM@Z @ 0x140368B30
// IDA 精确还原 - 设置慢动作效果
void CMover::SetSlowTime(float fTime, float fSpeed) {
    // 如果是新设置慢动作且当前没有慢动作，保存恢复速度
    if (fTime > 0.0f && m_stTimeSlow.fTime == 0.0f) {
        m_fRestoreAnimSpeed = m_fAnimSpeed;
        // 检查是否处于攻击状态
        if (XActor::IsStatus(STATUS_ATTACK) || XActor::IsStatus(0x400u)) {
            m_eRestoreAnimSpeedType = AST_ATTACK;
        } else {
            m_eRestoreAnimSpeedType = AST_NONE;
        }
    }
    m_stTimeSlow.fTime = fTime;
    m_stTimeSlow.fSpeed = fSpeed;
    SetAnimSpeed(fSpeed);
}

// IDA: ?SetReactionRate@CMover@@QEAAXM@Z @ 0x1402C7400
// IDA 精确还原 - 设置反应速率
void CMover::SetReactionRate(float fRate) {
    m_fReactionRate = fRate;
}

// IDA: ?GetHeight@CMover@@QEAA_NAEAVhkvVec3@@M@Z @ 0x14036D130
// IDA 精确还原 - 获取高度（通过导航网格）
bool CMover::GetHeight(hkvVec3* pPos, float fTestHeight) {
    XArea* pArea = GetArea();
    if (!pArea) {
        return false;
    }
    DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    if (!pNavMesh) {
        return false;
    }
    return pNavMesh->GetHeight(pPos, fTestHeight);
}

// Additional missing functions - stubs
// IDA: ?CheckMoveCollision@CMover@@QEAAPEAV1@AEAVhkvVec3@@@Z @ 0x1403681B0
// IDA 精确还原 - 检查移动碰撞
CMover* CMoverEx::CheckMoveCollision(hkvVec3* pDestPos) {
    // 只对 Monster 类型进行检查
    if (XActor::GetType() != eActorMonster) {
        return nullptr;
    }

    // 检查碰撞是否启用
    if (!m_bCollisionEnable) {
        return nullptr;
    }

    // 检查是否保持额外移动
    if (m_bKeepMovingExtra) {
        return nullptr;
    }

    // 尝试转换为 CMonster
    CMonster* pMonster = dynamic_cast<CMonster*>(this);
    if (!pMonster) {
        return nullptr;
    }

    // 跟随者不检查碰撞
    if (pMonster->IsFollower()) {
        return nullptr;
    }

    // 巡逻怪物不检查碰撞
    CAi* pAi = pMonster->GetAi();
    if (pAi && pAi->IsPatrolMonster()) {
        return nullptr;
    }

    // 搜索附近的目标
    float fNearestDist = 5000.0f;
    CMover* pNearestTarget = nullptr;
    hkvVec3 vNearestPos;

    std::vector<CMover*> vecGameObjList;
    XArea* pArea = GetArea();
    if (pArea) {
        XArea::ScanGridOrigin(this, 3, 3u, vecGameObjList);
    }

    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMover* pOtherActor = *it;
        if (!pOtherActor) {
            continue;
        }

        // 检查目标类型
        bool bCheckActor = false;
        E_ACTOR_TYPE eOtherType = static_cast<E_ACTOR_TYPE>(pOtherActor->XActor::GetType());

        if (eOtherType == eActorUser) {
            bCheckActor = true;
        } else if (eOtherType == eActorMonster) {
            CMonster* pOtherMonster = dynamic_cast<CMonster*>(pOtherActor);
            if (pOtherMonster && pOtherMonster->IsDefenseObject()) {
                bCheckActor = true;
            }
        }

        if (bCheckActor) {
            // 检查是否存活
            if (!pOtherActor->IsLive() || pOtherActor->IsStatus(STATUS_DIE)) {
                continue;
            }

            // 计算距离
            hkvVec3 vOtherPos = pOtherActor->GetPosition();
            hkvVec3 vOffset = *pDestPos - vOtherPos;
            vOffset.z = 0.0f;
            float fDist = vOffset.getLength();

            if (fNearestDist > fDist) {
                vNearestPos = vOtherPos;
                fNearestDist = fDist;
                pNearestTarget = pOtherActor;
            }
        }
    }

    // 检查最近的目标是否在碰撞范围内
    if (pNearestTarget) {
        hkvVec3 vOffset = *pDestPos - vNearestPos;
        vOffset.z = 0.0f;
        float fDist = vOffset.getLength();
        float fOtherRadius = pNearestTarget->GetHavokCapsuleRadius();
        float fCollisionDist = (fOtherRadius + m_fCapsuleRadius + 5.0f) + 5.0f;

        if (fCollisionDist >= fDist) {
            return pNearestTarget;
        }
    }

    return nullptr;
}

// IDA: ?send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA@CMover@@QEAAXPEAV1@VhkvVec3@@_N@Z
// IDA 精确还原 - 发送忽略动作增量的移动包
void CMover::send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(CMover* pMover, hkvVec3 vPos, bool bForced) {
    if (bForced || pMover->GetType() == 2) {  // 2 = Monster type
        // 清除额外移动值
        m_stExtMovingVal.Clear();

        // 获取偏航角
        float fYaw = pMover->GetOrientationYaw();
        if (fYaw < -360.0f || fYaw > 360.0f) {
            LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA>> Wrong yaw : %.2f", fYaw);
            fYaw = 0.0f;
        }

        // 构造移动忽略包
        ST_MOVE_IGNORE_MOTION_DELTA stMoveIgnore;
        UXActorID actorID = pMover->GetActorID();
        stMoveIgnore.dwActorID = actorID.parts.dwID;
        stMoveIgnore.fPosX = vPos.x;
        stMoveIgnore.fPosY = vPos.y;
        stMoveIgnore.fPosZ = vPos.z;
        stMoveIgnore.fYaw = fYaw;
        stMoveIgnore.fPitch = pMover->GetLookPitch();
        stMoveIgnore.bForced = bForced;

        // 发送广播包
        XSendPacket xPacket(5u, 0x13u);
        xPacket << stMoveIgnore;
        CGocNetwork::SendBroadCastAfterLoading(this, xPacket, E_BROADCAST_TYPE::eAll);

        m_fLastSendMoveTime = 0.0f;

        DebugOut("send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA>> (%.2f,%.2f) %.3f", vPos.x, vPos.y);
    }
}

// IDA: ?GetAnimIndex@CMover@@QEAAKVVString@@@Z @ 0x1403689A0
// IDA 精确还原 - 从动画名称获取动画索引
std::uint32_t CMoverEx::GetAnimIndex(const VString& strAnimName) {
    if (m_mapAnimInfoKey) {
        auto it = m_mapAnimInfoKey->find(strAnimName);
        if (it != m_mapAnimInfoKey->end()) {
            return it->second;
        }
        return 0xFFFFFFFF;  // -1 表示未找到
    }
    return 0xFFFFFFFF;
}

// IDA: ?IsDamageMotionDisplay@CMover@@UEAAHE@Z @ 0x140367BD0
// IDA 精确还原 - 判断是否显示受伤动作
bool CMover::IsDamageMotionDisplay(std::uint8_t byAttackCollision) const {
    if (!byAttackCollision) {
        return false;
    }

    switch (m_byDefenseType) {
    case 1:
        // 防御类型1：只在攻击类型为2或4时显示受伤
        return byAttackCollision == 2 || byAttackCollision == 4;
    case 2:
        // 防御类型2：只在攻击类型为3或4时显示受伤
        return byAttackCollision == 3 || byAttackCollision == 4;
    case 3:
    case 5:
        // 防御类型3和5：不显示受伤
        return false;
    default:
        // 其他类型：显示受伤
        return true;
    }
}

void CMover::SetCurSkillTableIdx(int nIndex) {
    m_nCurSkillTableIdx = nIndex;
}

void CMoverEx::SetKeepMovingExtra(int nFlag) {
    m_bKeepMovingExtra = nFlag != 0;
}

void CMover::SetCollisionEnable(int nEnable, int nFlag) {
    m_bCollisionEnable = nEnable != 0;
}

// IDA: ?IsDashing@CMover@@QEAAHXZ @ 0x1403674F0
// IDA 精确还原 - 判断是否正在冲刺
bool CMover::IsDashing() const {
    if (m_fForcedStateApplyTime <= 0.0f) {
        return XActor::IsStatus(0x800u);
    }
    return m_uiForcedState == 2;
}

// IDA: ?ClearActionBuffer@CMover@@QEAAXXZ @ 0x140378840
// IDA 精确还原 - 清除动作缓冲区
void CMoverEx::ClearActionBuffer() {
    m_xActionBuffer.Clear();
}

// IDA: ?SetCurrentSequenceTime@CMover@@QEAAXM@Z @ 0x140368C40
// IDA 精确还原 - 设置当前动画序列时间
void CMover::SetCurrentSequenceTime(float fTime) {
    m_fAnimationTime = fTime;
    if (m_pCurMotionEvent) {
        if (m_pCurMotionEvent->fAnimationLength > 0.0f) {
            m_fAnimPercentTime = fTime / m_pCurMotionEvent->fAnimationLength;
        }
    }
}

void CMover::SetPositionXVec3(const hkvVec3& vPos) {
    m_vPosition = vPos;
}

// IDA: ?IsAllowPassiveType@CMover@@QEAAHH@Z @ 0x140364670
// IDA 精确还原 - 检查是否允许被动类型
bool CMover::IsAllowPassiveType(int nType) {
    // 如果 m_setAllowPassiveType 为空，返回 true
    if (m_setAllowPassiveType.empty()) {
        return true;
    }
    // 否则检查 nType 是否在集合中
    return m_setAllowPassiveType.find(nType) != m_setAllowPassiveType.end();
}

// IDA: ?send_eSUB_CMD_MONSTER_INVISIBLE@CMover@@QEAAXPEAV1@EKHH@Z @ 0x140370BA0
// IDA 精确还原 - 发送怪物隐身状态包
void CMover::send_eSUB_CMD_MONSTER_INVISIBLE(CMover* pMover, std::uint8_t byInvisible, std::uint32_t dwFlag, int nType, int nValue) {
    XSendPacket xPacket(0x17u, 0x32u);

    UXActorID actorID = pMover->GetActorID();
    xPacket << actorID.parts.dwID;
    xPacket << byInvisible;
    xPacket << dwFlag;
    xPacket << nType;
    xPacket << nValue;

    CGocNetwork::SendBroadCast(this, xPacket, E_BROADCAST_TYPE::eAll);

    DebugOut("send_eSUB_CMD_MONSTER_INVISIBLE>> %d", byInvisible);
}

// IDA: ?SetHP@CMover@@UEAAXH@Z @ 0x140189230
// IDA 精确还原 - 设置HP（基类空实现，由子类重写）
void CMover::SetHP(int nHP) {
    // Empty base implementation - overridden by CMonster/CUser
}

int CMover::GetHP() const {
    // IDA: ?GetHP@CMover@@UEAAHXZ @ 0x140366E10
    // IDA 精确还原 - 通过 CGocAttribute 获取当前 HP
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    const_cast<CMover*>(this)->GetGOC<CGocAttribute>(&pAttr, 0);
    if (!pAttr) {
        return 0;
    }
    return pAttr->GetHP();
}

// IDA: ?GetComboCount@CMover@@QEAAHXZ - base class returns 0
// IDA: ?GetComboCount@CUser@@QEAAHXZ - overridden in CUser to return actual combo count
int CMover::GetComboCount() {
    return 0;  // Base class returns 0, overridden by CUser
}

int CMover::GetTableID() {
    // TODO: IDA - base class returns 0, overridden by CMonster/CUser/CNpc
    return 0;
}

// IDA: ?GetTableIDString@CMover@@QEAAPEBDXZ @ 0x14036DE70
// IDA 精确还原 - 获取TableID字符串表示
const char* CMover::GetTableIDString() {
    if (m_strTableID.IsEmpty()) {
        int nTableID = GetTableID();
        m_strTableID.Format("%d", nTableID);
    }
    return m_strTableID.AsChar();
}

// IDA: ?IsAttackHeight@CMover@@QEAAEQEAUtagATTACK_AREA@@AEAVhkvVec3@@AEAH@Z @ 0x140368D50
// IDA 精确还原 - 检查攻击高度范围
std::uint8_t CMover::IsAttackHeight(const tagATTACK_AREA& stArea, hkvVec3& vPos, int& nResult) {
    // 检查是否超出上限高度
    if (vPos.z > stArea.fHeightT) {
        if (!m_pHitCollisionData) {
            return 1;  // 超出高度，不在攻击范围
        }
        nResult = 0;  // 不检查圆柱碰撞
    }
    // 检查是否低于下限高度
    if (stArea.fHeightB > (vPos.z + m_fHitCylinderHeight)) {
        if (!m_pHitCollisionData) {
            return 1;  // 低于高度，不在攻击范围
        }
        nResult = 0;  // 不检查圆柱碰撞
    }
    return 0;  // 在攻击高度范围内
}

float CMover::GetHitCylinderRadius() {
    return m_fHitCylinderRadius;
}

tagHIT_COLLISION_DATA* CMover::GetHitCollisionData() {
    return m_pHitCollisionData;
}

// Additional stubs
float CMover::GetHitCylinderHeight() { return m_fHitCylinderHeight; }
float CMover::GetCapsuleHeight() { return m_fCapsuleHeight; }
float CMover::GetCapsuleRadius() { return m_fCapsuleRadius; }
// ============================================================================
// CMover::AddExtraMoving
// IDA: ?AddExtraMoving@CMover@@UEAAXMMM@Z @ 0x14036C210
// IDA 精确还原 - 添加额外移动
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

    CheckMoveDestPos(&vDestPos, 0, 0);

    m_stExtMovingVal.x = vDestPos.x;
    m_stExtMovingVal.y = vDestPos.y;

    float fMovingTime = (fTime <= m_stExtMovingVal.fMovingTime) ? m_stExtMovingVal.fMovingTime : fTime;
    m_stExtMovingVal.fMovingTime = fMovingTime;
    m_stExtMovingVal.fRemainTime = fMovingTime + 0.2f;
}
int CMover::IsOnGroundState() { return m_bOnGround ? 1 : 0; }
void CMover::SetOnGroundState(int nState) { m_bOnGround = nState != 0; }
void CMover::SetGroundPosZ(float fZ) { m_fGroundPosZ = fZ; }
void CMover::SetCapsuleRadius(float fRadius) { m_fCapsuleRadius = fRadius; }
void CMover::SetCapsuleHeight(float fHeight) { m_fCapsuleHeight = fHeight; }
void CMover::SetHitCylinderRadius(float fRadius) { m_fHitCylinderRadius = fRadius; }
void CMover::SetHitCylinderHeight(float fHeight) { m_fHitCylinderHeight = fHeight; }
void CMover::SetHitCollisionData(tagHIT_COLLISION_DATA* pData) { m_pHitCollisionData = pData; }
void CMover::RemoveTargetDestPos() { m_byTargetDestPos = 0xFF; }
void CMover::SetWeightRank(signed char cRank) { m_cWeightRank = cRank; }

// IDA: ?IsGeneralHit@CMover@@QEAAHXZ @ 0x140367410
// IDA 精确还原 - 判断是否处于普通受击状态
bool CMover::IsGeneralHit() const {
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 4;
    }
    return m_nMotionClass >= 15 && m_nMotionClass <= 17;
}
int CMover::GetCurSkillTableIdx() const { return m_nCurSkillTableIdx; }

// ============================================================================
// CMover::CreateRandomTrapIndex
// IDA: ?CreateRandomTrapIndex@CMover@@QEAAHXZ @ 0x14036E3E0
// 精确还原 - 创建随机陷阱索引
// ============================================================================
int CMover::CreateRandomTrapIndex() {
    m_nRandomTrapIndex = std::rand() % 1000000;
    return m_nRandomTrapIndex;
}

// ============================================================================

// ============================================================================
// CMover::send_eSUB_CMD_ACTIVE_SKILL
// IDA: ?send_eSUB_CMD_ACTIVE_SKILL@CMover@@QEAAXPEAV1@HE@Z @ 0x1403714A0
// 精确还原 - 发送激活技能广播包 (main=6, sub=0x10)
// ============================================================================
void CMover::send_eSUB_CMD_ACTIVE_SKILL(CMover* pMover, std::uint32_t nSkillID, std::uint8_t byAngleAttackType) {
    // IDA: XSendPacket::XSendPacket(&xPacket, 6u, 0x10u)
    XSendPacket xPacket(6, 0x10);

    // IDA: Get target mover's position and yaw
    float fYaw = pMover->GetOrientationYaw();
    hkvVec3 vPos = GetPosition();

    // IDA: Create PS_SkillActive_BT structure
    PS_SkillActive_BT btInfo{};
    btInfo.uxUseActorID = pMover->GetActorID();
    btInfo.uxActorID = GetActorID();
    btInfo.nSkillID = nSkillID;
    btInfo.psSkillPosInfo.nMotionClass = -1;
    btInfo.psSkillPosInfo.fAngle = fYaw;
    btInfo.psSkillPosInfo.xPos.x = vPos.x;
    btInfo.psSkillPosInfo.xPos.y = vPos.y;
    btInfo.psSkillPosInfo.xPos.z = vPos.z;
    btInfo.nRandomKey = CreateRandomTrapIndex();
    btInfo.byAngleAttackType = byAngleAttackType;

    // IDA: operator<<(&xPacket, &btInfo)
    xPacket << btInfo;

    // IDA: CGocNetwork::SendBroadCast(this, &xPacket, 1u)
    CGocNetwork::SendBroadCast(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);

    // IDA: DebugOut
    DebugOut("send_eSUB_CMD_ACTIVE_SKILL>> %d (%.2f,%.2f) %.3f", nSkillID, vPos.x, vPos.y, fYaw);
}
// ============================================================================
// CMover::IsRegisterAnimInfo
// IDA: ?IsRegisterAnimInfo@CMover@@QEAAHFFVVString@@H@Z @ 0x140367AE0
// IDA 精确还原 - 检查动画信息是否已注册
// ============================================================================
bool CMover::IsRegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, const VString& strAnimName) {
    if (m_pActionResource) {
        const char* pszAnimName = strAnimName.AsChar();
        if (m_pActionResource->FindAnimationInfo(pszAnimName)) {
            return true;
        }
    }
    return false;
}
// ============================================================================
// CMover::SetOrientationYaw
// IDA: ?SetOrientationYaw@CMover@@QEAAXM@Z @ 0x1402C7C60
// IDA 精确还原 - 设置方向偏航角
// ============================================================================
void CMover::SetOrientationYaw(float fYaw) {
    SetOrientation(fYaw, m_vOrientation.y, m_vOrientation.z);
}

// IDA: ?ChangeSequence@CMover@@QEAAXKKH@Z @ 0x14036C500
// 切换动画序列
void CMover::ChangeSequence(std::uint32_t dwOldAnimID, std::uint32_t dwNewAnimID, int bResetPlay) {
    // IDA 精确还原:
    m_fAnimPercentTime = 0.0f;
    m_fAnimationTime = 0.0f;
    m_bAnimPlay = 1;

    if (dwOldAnimID != dwNewAnimID) {
        m_bAnimChanged = 1;
    }

    m_vAnimLastDelta = hkvVec3::ZeroVector();

    const char* pszAnimString = GetAnimStirng(dwNewAnimID);
    if (pszAnimString) {
        ChangeActionTrigger(pszAnimString);
        if (IsFixedLoopAnim(dwNewAnimID)) {
            if (m_pCurMotionEvent) {
                m_pCurMotionEvent->eEndofAnimation = END_OF_ANIM_NONE;  // LOOP = 0
            }
        }
    }
}

// IDA: ?IsFixedLoopAnim@CMover@@QEAAHK@Z @ 0x14036C550
// 检查是否为固定循环动画
bool CMover::IsFixedLoopAnim(std::uint32_t dwAnimID) {
    std::int16_t nMotion = AnimKeyToMotion(dwAnimID);
    if (nMotion == 1 || nMotion == 3 || nMotion == 5 || nMotion == 4 || nMotion == 10) {
        return true;
    }
    if (nMotion >= 18 && nMotion <= 21) {
        std::int16_t nHitStatus = dwAnimID % 100;
        if (nHitStatus == 2 || nHitStatus == 6) {
            return true;
        }
    }
    return false;
}

// IDA: ?SetupAnimation@CMover@@QEAAXXZ @ 0x140367A50
// IDA 精确还原 - 设置动画资源
// TODO: 依赖 GetActionResourceFN, VResourceManager, XActionResMgr 等
void CMover::SetupAnimation() {
    // IDA 流程:
    // 1. GetActionResourceFN 获取资源文件名
    // 2. VResourceManager::GetResourceByName 获取动作资源
    // 3. XActionResMgr::SetAnimInfoToActor 设置动画信息
    // 4. SetupAnimInfo 设置动画信息
    // 5. GetAnimIndex(18, 2, 1) 获取受击动画数量
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_INFO
// IDA: ?send_eSUB_CMD_MOVE_INFO@CMover@@QEAAXPEAV1@KK@Z @ 0x14036FEF0
// IDA 精确还原 - 发送移动信息数据包 (5,0xD):
// pMover 的 ActorID 转 QuestID + dwType + dwVal ->
// SendBroadCastAfterLoading(this, packet, 0)
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_INFO(CMover* pMover, std::uint32_t dwType, std::uint32_t dwVal) {
    XSendPacket xPacket(5, 0xD);
    int nQuestID = CQuestCondition::GetQuestID(pMover->GetActorID());
    xPacket.XParse << nQuestID;
    xPacket.XParse << dwType;
    xPacket.XParse << dwVal;
    CGocNetwork::SendBroadCastAfterLoading(this, xPacket, static_cast<E_BROADCAST_TYPE>(0));
}

// ============================================================================
// CMover::SyncMove
// IDA: ?SyncMove@CMover@@QEAAXXZ @ 0x14036EA30
// 已精确还原 - m_stMovePos 非 Zero 时:
// byRunBit = IsStatus(0x100) ->
// send_eSUB_CMD_MOVE(this, m_stMovePos.x, m_stMovePos.y, byRunBit)
// ============================================================================
void CMover::SyncMove() {
    if (!m_stMovePos.IsZero()) {
        bool byRunBit = IsStatus(0x100u);
        send_eSUB_CMD_MOVE(this, m_stMovePos.x, m_stMovePos.y, byRunBit);
    }
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_STOP
// IDA: ?send_eSUB_CMD_MOVE_STOP@CMover@@QEAAXPEAV1@@Z @ 0x14036EE90
// IDA 精确还原 - 发送停止移动数据包
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_STOP(CMover* pMover) {
    // IDA: 获取移动偏航角
    float fYaw = pMover->GetMovingYaw();

    // IDA: 验证偏航角范围
    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    // IDA: 获取当前位置
    hkvVec3 vPos = pMover->GetPosition();
    XVec3 curPos(vPos.x, vPos.y, vPos.z);

    // IDA: 获取地图ID
    TUXMapID uxMapID;
    uxMapID.nMapID = 0;
    XArea* pArea = pMover->GetArea();
    if (pArea) {
        uxMapID = pArea->GetInstanceID();
    }

    // IDA: 创建 ST_MOVE_STOP 结构体
    ST_MOVE_STOP stMoveStop;
    stMoveStop.dwActorID = pMover->GetID();
    stMoveStop.nMapID = uxMapID.nMapID;
    stMoveStop.fPosX = curPos.x;
    stMoveStop.fPosY = curPos.y;
    stMoveStop.fPosZ = curPos.z;
    stMoveStop.fYaw = fYaw;
    stMoveStop.fPitch = pMover->GetLookPitch();
    stMoveStop.bCheckCanMove = false;

    // IDA: 创建数据包 - main=5, sub=4
    XSendPacket xPacket(5, 4);

    // IDA: 手动序列化 ST_MOVE_STOP 结构体字段
    xPacket << stMoveStop.dwActorID;
    xPacket << stMoveStop.nMapID;
    xPacket << stMoveStop.fPosX;
    xPacket << stMoveStop.fPosY;
    xPacket << stMoveStop.fPosZ;
    xPacket << stMoveStop.fYaw;
    xPacket << stMoveStop.fPitch;

    // IDA: 广播给其他玩家
    CGocNetwork::SendBroadCastAfterLoading(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);

    // IDA: 重置最后发送移动时间
    m_fLastSendMoveTime = 0.0f;

    // IDA: 调试输出
    DebugOut("send_eSUB_CMD_MOVE_STOP>> (%.2f,%.2f) %.3f", curPos.x, curPos.y, fYaw);
}
void CMoverEx::SetGazeMoving(int nFlag) { m_bGazeMoving = nFlag != 0; }

// IDA: ?send_eSUB_CMD_MOVE@CMover@@QEAAXPEAV1@MME@Z (0x14036EAC0)
// IDA 精确还原 - 发送移动数据包
void CMover::send_eSUB_CMD_MOVE(CMover* pMover, float fTargetPosx, float fTargetPosy, std::uint8_t byRunBit) {
    // IDA: 获取移动偏航角
    float fYaw = pMover->GetMovingYaw();

    // IDA: 验证偏航角范围
    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    // IDA: 获取移动速度
    m_fMoveSpeed = pMover->GetMoveSpeed();

    // IDA: 获取当前位置
    hkvVec3 vPos = pMover->GetPosition();

    // IDA: 获取地图ID
    TUXMapID uxMapID;
    uxMapID.nMapID = 0;
    XArea* pArea = pMover->GetArea();
    if (pArea) {
        uxMapID = pArea->GetInstanceID();
    }

    // IDA: 创建数据包 - main=5, sub=2
    XSendPacket xPacket(5, 2);

    // IDA: 手动序列化 ST_MOVE 结构体字段
    std::uint32_t dwActorID = pMover->GetID();
    xPacket << dwActorID;
    xPacket << uxMapID.nMapID;
    xPacket << vPos.x;
    xPacket << vPos.y;
    xPacket << vPos.z;
    xPacket << fYaw;
    xPacket << fTargetPosx;
    xPacket << fTargetPosy;
    xPacket << byRunBit;
    float fPitch = pMover->GetLookPitch();
    xPacket << fPitch;
    xPacket << m_fMoveSpeed;
    std::uint8_t byChangeMotion = 0;
    xPacket << byChangeMotion;

    // IDA: 广播给其他玩家
    CGocNetwork::SendBroadCastAfterLoading(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);

    // IDA: 重置最后发送移动时间
    m_fLastSendMoveTime = 0.0f;

    // IDA: 调试输出
    DebugOut("send_eSUB_CMD_MOVE>> (%.2f,%.2f) -> (%.2f,%.2f) -> Rot:%.3f / %d",
             vPos.x, vPos.y, fTargetPosx, fTargetPosy, fYaw, byRunBit);
}

// IDA: ?send_eSUB_CMD_SKILL_DEFENCE_TYPE@CMover@@QEAAXPEAV1@E_N@Z (0x140371EA0)
// IDA 精确还原 - 发送技能防御类型包
void CMover::send_eSUB_CMD_SKILL_DEFENCE_TYPE(CMover* pMover, std::uint8_t byDefenceType, bool bAdd) {
    // IDA: 创建数据包 - main=6, sub=0x7D (125)
    XSendPacket xPacket(6, 0x7D);

    // IDA: 获取 ActorID 并获取 QuestID
    UXActorID uxActorID = pMover->GetActorID();
    int nQuestID = CQuestCondition::GetQuestID(uxActorID);

    // IDA: 序列化数据
    xPacket << nQuestID;
    xPacket << byDefenceType;
    xPacket << static_cast<std::uint8_t>(bAdd ? 1 : 0);

    // IDA: 发送数据包 (使用 SendAfterLoading 发送给单个 actor)
    CGocNetwork::SendAfterLoading(this, xPacket);

    // IDA: 调试输出
    DebugOut("send_eSUB_CMD_SKILL_DEFENCE_TYPE");
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_UPDATE_DIR
// IDA: ?send_eSUB_CMD_MOVE_UPDATE_DIR@CMover@@QEAAXPEAV1@_N@Z @ 0x140370390
// IDA 精确还原 - 发送更新方向数据包
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_UPDATE_DIR(CMover* pMover, bool bDirect) {
    // IDA: 创建数据包 - main=5, sub=0x14 (20)
    XSendPacket xPacket(5, 0x14);

    // IDA: 获取移动偏航角
    float fYaw = pMover->GetMovingYaw();

    // IDA: 验证偏航角范围
    if (fYaw < -360.0f || fYaw > 360.0f) {
        LogHelper::LogDebug("game.contents", "send_eSUB_CMD_MOVE_UPDATE_DIR>> Wrong yaw : %.2f", fYaw);
        fYaw = 0.0f;
    }

    // IDA: 序列化数据
    std::uint32_t dwActorID = pMover->GetID();
    float fPitch = pMover->GetLookPitch();
    xPacket << dwActorID;
    xPacket << fYaw;
    xPacket << fPitch;
    xPacket << bDirect;

    // IDA: 广播给其他玩家
    CGocNetwork::SendBroadCastAfterLoading(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);

    // IDA: 重置最后发送移动时间
    m_fLastSendMoveTime = 0.0f;

    // IDA: 调试输出
    DebugOut("send_eSUB_CMD_MOVE_UPDATE_DIR>> %.3f", fYaw);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_BATTLE
// IDA: ?send_eSUB_CMD_MOVE_BATTLE@CMover@@QEAAXPEAV1@_N@Z
// IDA 精确还原 - 发送战斗移动数据包
// ============================================================================
// ============================================================================
// CMover::send_eSUB_CMD_MOVE_BATTLE
// IDA: ?send_eSUB_CMD_MOVE_BATTLE@CMover@@QEAAXPEAV1@_N@Z @ 0x14036F1E0
// IDA 精确还原 - 发送战斗移动数据包
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_BATTLE(CMover* pMover, bool bPlayMotion) {
    // IDA: 创建 ST_MOVE_BATTLE 结构体
    ST_MOVE_BATTLE stMoveBattle;
    stMoveBattle.dwActorID = pMover->GetID();

    // IDA: 获取位置
    hkvVec3 vPos = pMover->GetPosition();
    stMoveBattle.fPosX = vPos.x;
    stMoveBattle.fPosY = vPos.y;
    stMoveBattle.fPosZ = vPos.z;

    // IDA: 获取偏航角
    stMoveBattle.fYaw = pMover->GetMovingYaw();

    // IDA: 获取战斗姿态和播放动作标志
    stMoveBattle.bBattlePose = pMover->IsBattlePose() ? 1 : 0;
    stMoveBattle.bPlayMotion = bPlayMotion ? 1 : 0;

    // IDA: 创建数据包 - main=5, sub=8
    XSendPacket xPacket(5, 8);

    // IDA: 手动序列化字段
    xPacket << stMoveBattle.dwActorID;
    xPacket << stMoveBattle.fPosX;
    xPacket << stMoveBattle.fPosY;
    xPacket << stMoveBattle.fPosZ;
    xPacket << stMoveBattle.fYaw;
    xPacket << stMoveBattle.bBattlePose;
    xPacket << stMoveBattle.bPlayMotion;

    // IDA: 广播给其他玩家
    CGocNetwork::SendBroadCastAfterLoading(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);

    // IDA: 调试输出
    DebugOut("send_eSUB_CMD_MOVE_BATTLE>> (%.2f,%.2f) %.3f (%d/%d)",
             vPos.x, vPos.y, stMoveBattle.fYaw, stMoveBattle.bBattlePose, stMoveBattle.bPlayMotion);
}

// IDA: ?GetSkillAnimName@CMoverEx@@QEAAPEBDPEAUTB_SKILL@@E@Z (0x14037EF50)
// 精确还原: 根据技能和步骤获取动画名称
const char* CMoverEx::GetSkillAnimName(TB_SKILL* pSkillTableRef, std::uint8_t byStep) {
    if (!pSkillTableRef) {
        return nullptr;
    }

    const char* pSkillName = nullptr;
    if (byStep == 0) {
        pSkillName = pSkillTableRef->Ani_Res_Start;
    } else if (byStep == 1) {
        pSkillName = pSkillTableRef->Ani_Res_Loop;
    } else if (byStep == 2) {
        pSkillName = pSkillTableRef->Ani_Res_Extra;
    } else {
        pSkillName = pSkillTableRef->Ani_Res_End;
    }

    memset(m_szTempSkillAnimName, 0, sizeof(m_szTempSkillAnimName));

    // Control type 2 or 5 with step 3 (charging skills)
    std::uint8_t byControlType = GetControlType(pSkillTableRef);
    if ((byControlType == 2 || byControlType == 5) && byStep == 3) {
        std::uint8_t byChargeStep = GetSkillChargeStep();
        sprintf(m_szTempSkillAnimName, "%s_%02d", pSkillName, byChargeStep + 1);
        VString strAnimName(m_szTempSkillAnimName);
        if (GetAnimIndex(strAnimName) != static_cast<unsigned int>(-1)) {
            return m_szTempSkillAnimName;
        }
    }
    // Control type 8 with step 3 (directional charging)
    else if (byControlType == 8 && byStep == 3) {
        if (m_fLeftChargingValue < static_cast<float>(pSkillTableRef->Charging_Max_Value)) {
            if (m_fRightChargingValue < static_cast<float>(pSkillTableRef->Charging_Max_Value)) {
                sprintf(m_szTempSkillAnimName, "%s", pSkillName);
            } else {
                sprintf(m_szTempSkillAnimName, "%s_Attack02", pSkillName);
            }
        } else {
            sprintf(m_szTempSkillAnimName, "%s_Attack01", pSkillName);
        }
        VString strAnimName(m_szTempSkillAnimName);
        if (GetAnimIndex(strAnimName) != static_cast<unsigned int>(-1)) {
            return m_szTempSkillAnimName;
        }
    }
    // Directional skills (Skill_Direction == 1)
    else if (pSkillTableRef->Skill_Direction == 1) {
        const char* szDir[4] = { "_F", "_L", "_R", "_B" };
        sprintf(m_szTempSkillAnimName, "%s%s", pSkillName, szDir[m_byMoveDir]);
        VString strAnimName(m_szTempSkillAnimName);
        if (GetAnimIndex(strAnimName) != static_cast<unsigned int>(-1)) {
            return m_szTempSkillAnimName;
        }
    }

    return pSkillName;
}

// ============================================================================
// CMoverEx::HasSkillSkipTime (const char* version)
// IDA: ?HasSkillSkipTime@CMoverEx@@QEAA_NPEBD@Z @ 0x140381680
// 精确还原 - 检查动画是否有技能跳过时间
// ============================================================================
bool CMoverEx::HasSkillSkipTime(const char* szTriggerName) {
    const VAnimationInfo* pActionInfo = GetActionDesc(szTriggerName);
    return HasSkillSkipTime(pActionInfo);
}

// ============================================================================
// CMoverEx::HasSkillSkipTime (VAnimationInfo* version)
// IDA: ?HasSkillSkipTime@CMoverEx@@QEAA_NPEBVVAnimationInfo@@@Z @ 0x1403816C0
// 精确还原 - 检查动画信息是否有技能跳过时间
// ============================================================================
bool CMoverEx::HasSkillSkipTime(const VAnimationInfo* pActionInfo) {
    if (!pActionInfo) {
        return false;
    }

    // IDA: Iterate through triggers and check for ExtraInputTrigger with SkipTime > 0
    for (int i = 0; i < pActionInfo->arTriggers.GetLength(); ++i) {
        ExtraInputTrigger* pTrigger = static_cast<ExtraInputTrigger*>(
            XActionResMgr::RetrieveEvent(2, i, const_cast<VAnimationInfo*>(pActionInfo)));
        if (!pTrigger) {
            break;
        }
        if (pTrigger->SkipTime > 0.0f) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// CMoverEx::IsCanSkillSkip
// IDA: ?IsCanSkillSkip@CMoverEx@@QEAA_NPEBDM@Z @ 0x140381530
// 精确还原 - 检查是否可以技能跳过
// ============================================================================
bool CMoverEx::IsCanSkillSkip(const char* pszAnim, float fTime) {
    // TODO: 需要完整实现，暂时返回 false
    return false;
}

// ============================================================================
// CMoverEx::CheckPassiveSkill
// IDA: ?CheckPassiveSkill@CMoverEx@@UEAAXEE@Z @ 0x140188FC0
// IDA 精确还原 - 检查被动技能（基类空实现，派生类重写）
// ============================================================================
void CMoverEx::CheckPassiveSkill(std::uint8_t byTargetType, std::uint8_t byCondition) {
    // 基类空实现 - 派生类 CMonster, CUser 有自己的实现
}

// IDA: ?IsFlyHit@CMover@@QEAAHXZ @ 0x140367480
// IDA 精确还原 - 检查是否处于空中受击状态
bool CMover::IsFlyHit() const {
    if (m_fForcedStateApplyTime > 0.0f) {
        return m_uiForcedState == 4;
    }
    return m_nMotionClass >= 18 && m_nMotionClass <= 21;
}
// IDA: ?FindBuffStatus@CMover@@QEBAHGK@Z @ 0x14036A420
// IDA 精确还原 - 查找Buff状态槽位
int CMover::FindBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwAttackerID) const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex == nBuffIndex) {
            // If effect type is not 10, return immediately
            if (m_stBuffState[i].byEffectType != 10) {
                return i;
            }
            // If effect type is 10, check attacker ID
            if (m_stBuffState[i].dwID == dwAttackerID) {
                return i;
            }
        }
    }
    return -1;
}

// IDA: ?GetYawFromVector@CMover@@SAMAEBVhkvVec3@@@Z @ 0x14036DC00
// IDA 精确还原 - 从向量计算Yaw角度
float CMover::GetYawFromVector(const hkvVec3& vDir) {
    // 基准向量 (0, -1, 0) 指向下方
    hkvVec3 n1(0.0f, -1.0f, 0.0f);
    hkvVec3 n2(vDir.x, vDir.y, 0.0f);

    // 归一化
    hkvVec3 normalized = n2.getNormalized();

    // 计算点积
    float calcValue = n1.dot(normalized);
    calcValue = std::clamp(calcValue, -1.0f, 1.0f);

    // 计算角度（弧度转角度）
    float fDegree = hkvMath::Rad2Deg(std::acos(calcValue));

    // 处理X轴方向
    if (vDir.x < 0.0f) {
        fDegree = (180.0f - fDegree) + 180.0f;
    }

    // 规范化到 [-180, 180]
    if (fDegree > 180.0f) {
        return fDegree - 360.0f;
    }
    return fDegree;
}

// IDA: ?GetHavokCapsuleRadius@CMover@@QEAAMXZ @ 0x140276870
// IDA 精确还原 - 获取Havok胶囊半径
float CMover::GetHavokCapsuleRadius() {
    return m_fCapsuleRadius;
}

hkvVec3& CMover::GetCreatePos() { return m_vCreatePos; }
float CMoverEx::GetMoveDistAfterSkill() const { return m_fMoveDistAfterSkill; }
// ============================================================================
// CMover::SetMovePosition
// IDA: ?SetMovePosition@CMover@@QEAAXMM@Z @ 0x14036CB40
// IDA 精确还原 - 设置移动目标位置
// ============================================================================
void CMover::SetMovePosition(float fXpos, float fYpos) {
    m_stMovePos.x = fXpos;
    m_stMovePos.y = fYpos;
    m_stMoveGap = m_stMovePos;

    hkvVec3 vPos = GetPosition();
    m_stMoveOffset.x = vPos.x - fXpos;
    m_stMoveOffset.y = vPos.y - fYpos;
    m_vMoveStopCheckPos = vPos;
}
std::uint8_t CMover::GetTargetDestPos() { return m_byTargetDestPos; }
// ============================================================================
// CMover::GetAnimationIdx
// IDA: ?GetAnimationIdx@CMover@@QEAAKXZ @ 0x140276370
// IDA 精确还原 - 获取动画索引
// ============================================================================
std::uint32_t CMover::GetAnimationIdx() {
    return m_nAnimationIdx;
}

// ============================================================================
// CMoverEx::CalcTargetDamage_2
// IDA: ?CalcTargetDamage_2@CMoverEx@@... - 计算目标伤害
// TODO: 需要完整实现
// ============================================================================
void CMoverEx::CalcTargetDamage_2(CMover* pTarget, int nIndex, bool bAllowAbsorbSG,
    TB_SKILL* pSkillTable, AttackJudgmentTrigger* pActionEvent,
    float fDamageRate, bool bUnk1, int nUnk2, bool bUnk3) {
    // TODO: 完整实现需要大量依赖
}

// ============================================================================
// CMover::send_eSUB_CMD_ACTION_SKILL
// IDA: ?send_eSUB_CMD_ACTION_SKILL@CMover@@QEAAXPEAV1@HFAEAVhkvVec3@@EG_N@Z @ 0x1403716C0
// 精确还原 - 发送技能动作包 (main=6, sub=0x13)
// ============================================================================
void CMover::send_eSUB_CMD_ACTION_SKILL(CMover* pMover, std::uint32_t nSkillID,
    std::int16_t nTriggerIdx, hkvVec3* vPos, std::uint8_t byAttackTargetCnt,
    std::uint16_t wContinousHit, bool bPenetrate) {

    // IDA: XSendPacket::XSendPacket(&xPacket, 6u, 0x13u)
    XSendPacket xPacket(6, 0x13);

    // IDA: Get yaw
    float fYaw = pMover->GetOrientationYaw();

    // IDA: Build packet structure (PS_SkillActionEx)
    // Note: This is a complex structure that requires additional type definitions
    // For now, send basic skill action info
    UXActorID uxActorID = GetActorID();
    xPacket << nSkillID;
    xPacket << nTriggerIdx;
    xPacket << wContinousHit;
    xPacket << uxActorID.dwActorID;
    xPacket << vPos->x;
    xPacket << vPos->y;
    xPacket << vPos->z;
    xPacket << fYaw;
    xPacket << static_cast<std::uint8_t>(bPenetrate ? 1 : 0);
    xPacket << byAttackTargetCnt;

    // IDA: CGocNetwork::SendBroadCast(this, &xPacket, 0)
    CGocNetwork::SendBroadCast(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);

    DebugOut("send_eSUB_CMD_ACTION_SKILL>> %d (%.2f,%.2f) %.3f count:%d continous:%d",
        nSkillID, vPos->x, vPos->y, fYaw, byAttackTargetCnt, wContinousHit);
}

// Additional stubs
void CMover::ScanGridOrigin(int nRange, int nType, std::vector<XActor*>* pResult) { if (pResult) pResult->clear(); }

// ============================================================================
// CMover::send_eSUB_CMD_CHAIN
// IDA: ?send_eSUB_CMD_CHAIN@CMover@@QEAAXPEAV1@HFAEAVhkvVec3@@1HK@Z @ 0x1403723A0
// IDA 精确还原 - 发送连锁技能广播包 (main=6, sub=0x38)
// ============================================================================
void CMover::send_eSUB_CMD_CHAIN(CMover* pMover, std::uint32_t nSkillID, std::int16_t nTriggerIdx,
                                  hkvVec3* vPos, hkvVec3* vDir, std::uint32_t nSessionID, std::uint32_t dwTargetID) {
    XSendPacket xPacket(6, 0x38);

    PS_Chain_BT stChain;
    stChain.nSkillID = nSkillID;
    stChain.nTriggerIdx = nTriggerIdx;
    stChain.xPos.x = vPos->x;
    stChain.xPos.y = vPos->y;
    stChain.xPos.z = vPos->z;
    stChain.xDir.x = vDir->x;
    stChain.xDir.y = vDir->y;
    stChain.xDir.z = vDir->z;
    stChain.nSessionID = nSessionID;
    stChain.dwTargetID = dwTargetID;
    stChain.uxActorID.dwActorID = pMover->GetActorID().dwActorID;

    xPacket << stChain;
    CGocNetwork::SendBroadCast(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);

    DebugOut("send_eSUB_CMD_CHAIN>> %d (%.2f,%.2f,%.2f) -> (%.2f,%.2f,%.2f)",
        nSkillID, vPos->x, vPos->y, vPos->z, vDir->x, vDir->y, vDir->z);
}
// ============================================================================
// CMover::ResetAllBuff
// IDA: ?ResetAllBuff@CMover@@QEAAXXZ @ 0x14036A860
// IDA 精确还原 - 重置所有Buff状态
// ============================================================================
void CMover::ResetAllBuff() {
    m_nBuffTotalCnt = 0;
    m_nBuffCnt = 0;
    m_nDebuffCnt = 0;
    for (std::uint8_t i = 0; i < 50; ++i) {
        m_stBuffState[i].Clear();
    }
}
void CMover::SetHitCylinder(float fRadius, float fHeight) { m_fHitCylinderRadius = fRadius; m_fHitCylinderHeight = fHeight; }
void CMover::ClearTraceBoneName() { m_vTraceBoneName.clear(); }
// ============================================================================
// CMover::GetTraceBoneListIndex
// IDA: ?GetTraceBoneListIndex@CMover@@QEAAHPEAD@Z @ 0x140373CE0
// IDA 精确还原 - 获取骨骼追踪列表索引
// ============================================================================
int CMover::GetTraceBoneListIndex(const char* pBoneName) {
    int iSize = static_cast<int>(m_vTraceBoneName.size());
    for (int i = 0; i < iSize; ++i) {
        if (m_vTraceBoneName[i].CompareNoCase(pBoneName)) {
            return i;
        }
    }
    return -1;
}

// ============================================================================
// CMover::RegisterTraceBoneName
// IDA: ?RegisterTraceBoneName@CMover@@QEAAXVVString@@@Z @ 0x140373C30
// IDA 精确还原 - 注册骨骼追踪名称
// ============================================================================
void CMover::RegisterTraceBoneName(const VString& strName) {
    VString strLower = strName;
    strLower.ToLower();

    if (GetTraceBoneListIndex(strLower.AsChar()) < 0) {
        m_vTraceBoneName.push_back(strLower);
    }
}
// ============================================================================
// CMover::AddActionBuffer
// IDA: ?AddActionBuffer@CMover@@QEAAXAEAUtagACTION_BUFFER@@@Z @ 0x140016C30
// IDA 精确还原 - 添加动作到缓冲区
// ============================================================================
void CMover::AddActionBuffer(tagACTION_BUFFER* xAction) {
    m_xActionBuffer.Push(xAction);
}

void CMover::ClearActionBuffer() { m_xActionBuffer.Clear(); }

// IDA: ?GetRandomTrapIndex@CMover@@QEAAHXZ @ 0x14036E440
// IDA 精确还原 - 获取随机陷阱索引
int CMover::GetRandomTrapIndex() {
    if (m_nRandomTrapIndex >= 1000000) {
        m_nRandomTrapIndex = 0;
    }
    unsigned int nIndex = m_nRandomTrapIndex;
    m_nRandomTrapIndex = nIndex + 1;
    return static_cast<int>(nIndex);
}

bool CMover::IsInvincibleActor() const { return m_bInvincibleActor; }

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_GAZE
// IDA: ?send_eSUB_CMD_MOVE_GAZE@CMover@@QEAAXPEAV1@@Z @ 0x14036F480
// IDA 精确还原 - 发送凝视移动包 (main=5, sub=0xA)
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_GAZE(CMover* pMover) {
    XSendPacket xPacket(5, 0x0A);

    // Actor ID
    xPacket.XParse << pMover->GetActorID().dwActorID;

    // Position
    hkvVec3 vPos = pMover->GetPosition();
    xPacket.XParse << vPos.x;
    xPacket.XParse << vPos.y;
    xPacket.XParse << vPos.z;

    // Yaw
    float fYaw = pMover->GetMovingYaw();
    xPacket.XParse << fYaw;

    // IsGazeMoving flag
    int nIsGazeMoving = pMover->IsGazeMoving();
    xPacket.XParse << nIsGazeMoving;

    CGocNetwork::SendBroadCastAfterLoading(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);

    DebugOut("send_eSUB_CMD_MOVE_GAZE>> (%.2f,%.2f) %.3f (%d)",
        vPos.x, vPos.y, fYaw, nIsGazeMoving);
}

// ============================================================================
// CMover::send_eSUB_CMD_MOVE_TRACE
// IDA: ?send_eSUB_CMD_MOVE_TRACE@CMover@@QEAAXPEAV1@@Z @ 0x14036F6B0
// IDA 精确还原 - 发送追踪移动包 (main=5, sub=0xB)
// ============================================================================
void CMover::send_eSUB_CMD_MOVE_TRACE(CMover* pMover, float fParam) {
    XSendPacket xPacket(5, 0x0B);

    // Actor ID
    xPacket.XParse << pMover->GetActorID().dwActorID;

    // Position
    hkvVec3 vPos = pMover->GetPosition();
    xPacket.XParse << vPos.x;
    xPacket.XParse << vPos.y;
    xPacket.XParse << vPos.z;

    // Yaw and Pitch
    float fYaw = pMover->GetMovingYaw();
    float fPitch = pMover->GetLookPitch();
    xPacket.XParse << fYaw;
    xPacket.XParse << fPitch;

    // Animation index and defense type
    int nAnimIdx = pMover->GetAnimationIdx();
    std::uint8_t byDefenseType = pMover->GetDefenseType();
    xPacket.XParse << nAnimIdx;
    xPacket.XParse << byDefenseType;

    // Projectile positions (from skill manager)
    // TODO: Need to get projectile list from skill manager
    // For now, send 0 count
    xPacket.XParse << (std::uint8_t)0;

    // AI state - only CMonster has AI
    std::uint8_t byAIState = 0;
    float fFuzzy[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    int nAction[7] = {0, 0, 0, 0, 0, 0, 0};

    // AI-specific data (only for CMonster)
    std::int16_t shWayPointID = 0;
    std::uint8_t byAggroCount = 0;

    CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    if (pMonster) {
        // Get AI state for monsters
        byAIState = pMonster->GetAIState();
        pMonster->GetAIFuzzyValue(fFuzzy);
        pMonster->GetAIActionValue(nAction);

        // Get waypoint ID
        CWayPoint* pWayPoint = pMonster->GetWayPoint();
        if (pWayPoint) {
            shWayPointID = pWayPoint->GetCurID();
        }
    }

    xPacket.XParse << byAIState;

    // AI fuzzy values (4 floats)
    xPacket.XParse << fFuzzy[0];
    xPacket.XParse << fFuzzy[1];
    xPacket.XParse << fFuzzy[2];
    xPacket.XParse << fFuzzy[3];

    // AI action values (7 ints)
    xPacket.XParse << nAction[0];
    xPacket.XParse << nAction[1];
    xPacket.XParse << nAction[2];
    xPacket.XParse << nAction[3];
    xPacket.XParse << nAction[4];
    xPacket.XParse << nAction[5];
    xPacket.XParse << nAction[6];

    // Move position
    xPacket.XParse << m_stMovePos.x;
    xPacket.XParse << m_stMovePos.y;

    // Waypoint and aggro (for CMonster only)
    if (pMonster) {
        xPacket.XParse << shWayPointID;

        // Get aggro list count
        auto& aggroList = pMonster->GetAggroList();
        byAggroCount = static_cast<std::uint8_t>(aggroList.size());
        xPacket.XParse << byAggroCount;

        // Send aggro entries (actor ID and threat value)
        for (auto itor = aggroList.begin(); itor != aggroList.end(); ++itor) {
            xPacket.XParse << itor->first;
            xPacket.XParse << itor->second.fAggro;
        }
    } else {
        xPacket.XParse << shWayPointID;
        xPacket.XParse << byAggroCount;
    }

    CGocNetwork::SendBroadCastAfterLoading(this, xPacket, E_BROADCAST_TYPE::eAll);

    DebugOut("send_eSUB_CMD_MOVE_TRACE>> (%.2f,%.2f,%.2f) yaw:%.3f pitch:%.3f",
        vPos.x, vPos.y, vPos.z, fYaw, fPitch);
}
// ============================================================================
// CMover::CheckDelayedProjectile
// IDA: ?CheckDelayedProjectile@CMover@@QEAAXM@Z @ 0x14036E640
// IDA 精确还原 - 检查延迟投射物
// ============================================================================
void CMover::CheckDelayedProjectile(float fDeltaTime) {
    for (auto itor = m_vecDelayedProjectile.begin(); itor != m_vecDelayedProjectile.end(); ++itor) {
        SDelayedProjectile* pTempDelayedProjectile = *itor;
        if (!pTempDelayedProjectile) {
            break;
        }

        pTempDelayedProjectile->fCurTime += fDeltaTime;

        if (pTempDelayedProjectile->bUsed || pTempDelayedProjectile->fCurTime < pTempDelayedProjectile->fCreateDelayTime) {
            continue;
        }

        // 创建动作缓冲区
        tagACTION_BUFFER xAction(1, 0.0f);
        xAction.pActionTrigger = pTempDelayedProjectile->pTrigger;

        float fYaw = GetOrientationYaw();
        xAction << pTempDelayedProjectile->pTrigger->EventID;
        xAction << pTempDelayedProjectile->nSkillID;
        xAction << 0;
        xAction << static_cast<std::uint8_t>(1);
        xAction << fYaw;

        AddActionBuffer(&xAction);
        pTempDelayedProjectile->bUsed = 1;
    }
}
// ============================================================================
// CMover::CheckContinuousMelee
// IDA: ?CheckContinuousMelee@CMover@@QEAAXM@Z @ 0x140373EE0
// IDA 精确还原 - 检查连续近战攻击
// ============================================================================
void CMover::CheckContinuousMelee(float fDeltaTime) {
    for (auto itor = m_vContinuousMelee.begin(); itor != m_vContinuousMelee.end(); ) {
        SContinuousMelee* pContinuousMelee = *itor;
        if (!pContinuousMelee) {
            break;
        }

        // 减少间隔时间
        pContinuousMelee->fRemainIntervalTime -= fDeltaTime;

        // 检查是否到达攻击时间
        if (pContinuousMelee->fRemainIntervalTime <= 0.0f) {
            // 创建动作缓冲区
            tagACTION_BUFFER xAction(1, 0.0f);
            xAction.pActionTrigger = pContinuousMelee->pTrigger;

            float fYaw = GetOrientationYaw();
            xAction << pContinuousMelee->pTrigger->EventID;
            xAction << pContinuousMelee->nSkillID;
            xAction << static_cast<std::uint8_t>(1);
            xAction << false;
            xAction << fYaw;

            AddActionBuffer(&xAction);

            // 重置间隔时间
            pContinuousMelee->fRemainIntervalTime = pContinuousMelee->pTrigger->sContinuousMeleeInfo.fIntervalTime;
        }

        // 减少生命时间
        pContinuousMelee->fRemainLifeTime -= fDeltaTime;

        if (pContinuousMelee->fRemainLifeTime > 0.0f) {
            ++itor;
        } else {
            // 删除过期的连续近战
            delete pContinuousMelee;
            itor = m_vContinuousMelee.erase(itor);
        }
    }
}

// IDA: ?GetHitList@CMover@@QEAAXAEAV?$list@UST_MONSTER_DAMAGE_INFO@@V?$allocator@UST_MONSTER_DAMAGE_INFO@@@std@@@std@@@Z (0x140374220)
// IDA 精确还原 - 获取伤害列表
void CMover::GetHitList(std::list<ST_MONSTER_DAMAGE_INFO>& listHit) {
    // IDA: Iterate through m_setHitID map and copy damage info to list
    for (auto it = m_setHitID.begin(); it != m_setHitID.end(); ++it) {
        ST_MONSTER_DAMAGE_INFO stInfo;
        stInfo.dwUCID = it->second.dwUCID;
        stInfo.nDamage = it->second.nDamage;
        stInfo.byClass = it->second.byClass;
        listHit.push_back(stInfo);
    }
}

// ============================================================================
// CMover::SetDie
// IDA: ?SetDie@CMover@@UEAAXFH_N@Z @ 0x140188FE0
// IDA 精确还原 - 设置死亡状态（基类空实现，派生类重写）
// ============================================================================
void CMover::SetDie(std::int16_t nMotion, int bSuicide, bool bSend) {
    // 基类空实现 - 派生类 CMoverEx, CMonster, CUser 有自己的实现
}

// IDA: ?GetMaxHP@CMover@@UEAAHXZ @ 0x140366EE0
// IDA 精确还原 - 通过 CGocAttribute 获取 MaxHP (stat type 10)
int CMover::GetMaxHP() {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);
    if (!pAttr) {
        return 0;
    }
    return pAttr->GetStat(10);  // STAT_MAX_HP = 10
}
float CMover::GetMovingYaw() { return m_fMovingYaw; }

// CGoc* static stubs
int CGocWeeklyMission::GetFamilyID() { return 0; }
int CGocClassEvent::GetFamilyID() { return 0; }
int CGocHelper::GetFamilyID() { return 0; }
int CGocExchange::GetFamilyID() { return 0; }
int CGocSoulMetry::GetFamilyID() { return 0; }

// CMover virtual stubs
// ClearBuffStatusBySlot is implemented above at line 616
// SetBuffStatus is implemented above at line 627
// IsClearBuff is implemented above at line 645
// UpdateDefenseType is implemented above at line 673 for CMoverEx
// ============================================================================
// CMover::IsEnemy
// IDA: ?IsEnemy@CMover@@UEAAHPEAV1@@Z @ 0x14036CD70
// IDA 精确还原 - 检查是否为敌人（类型不同则为敌人）
// ============================================================================
bool CMover::IsEnemy(CMover* pMover) const {
    int myType = XActor::GetType();
    int otherType = pMover->XActor::GetType();
    return myType != otherType;
}

// ============================================================================
// CMover::IsFriend
// IDA: ?IsFriend@CMover@@UEAAHPEAV1@@Z @ 0x14036CDF0
// IDA 精确还原 - 检查是否为友方（类型相同则为友方）
// ============================================================================
int CMover::IsFriend(CMover* pMover) {
    int myType = XActor::GetType();
    int otherType = pMover->XActor::GetType();
    return myType == otherType;
}

// ============================================================================
// CMover::IsFriendForChain
// IDA: ?IsFriendForChain@CMover@@UEAAHPEAV1@@Z @ 0x14036CE40
// IDA 精确还原 - 链式技能友方检查
// ============================================================================
int CMover::IsFriendForChain(CMover* pMover) {
    return IsFriend(pMover);
}

// ============================================================================
// CMover::IsEnemyForChain
// IDA: ?IsEnemyForChain@CMover@@UEAAHPEAV1@@Z @ 0x14036CDC0
// IDA 精确还原 - 链式技能敌方检查
// ============================================================================
int CMover::IsEnemyForChain(CMover* pMover) {
    return IsEnemy(pMover);
}

int CMover::IsParty(CMover* pMover) { /* TODO */ return 0; }
// DebugOut is implemented above
// ============================================================================
// CMover::IsBattlePose
// IDA: Base class returns false, CMoverEx overrides at 0x140189000
// IDA 精确还原 - 检查是否处于战斗姿态（基类返回false）
// ============================================================================
bool CMover::IsBattlePose() {
    return false;
}
// ============================================================================
// CMover::GetLookPitch
// IDA: Base class returns 0.0f, CMoverEx overrides at 0x140189270
// IDA 精确还原 - 获取视角俯仰角（基类返回0）
// ============================================================================
float CMover::GetLookPitch() {
    return 0.0f;
}
// ============================================================================
// CMover::SetLookPitch
// IDA: Base class does nothing, CMoverEx overrides at 0x14037F560
// IDA 精确还原 - 设置视角俯仰角（基类空实现）
// ============================================================================
void CMover::SetLookPitch(float fPitch) {
    (void)fPitch;  // Base class does nothing
}

// IDA: ?send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE@CMover@@QEAAXPEAV1@MM@Z @ 0x1403711E0
// IDA 精确还原 - 发送超级护甲值包
void CMover::send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE(CMover* pMover, float fCurGage, float fMaxGage) {
    XSendPacket xPacket(0x17, 0x53);  // main=23, sub=83

    UXActorID actorID = pMover->GetActorID();
    int QuestID = CQuestCondition::GetQuestID(actorID);

    xPacket << QuestID;
    xPacket << fCurGage;
    xPacket << fMaxGage;

    CGocNetwork::SendBroadCast(this, xPacket, static_cast<E_BROADCAST_TYPE>(0));

    DebugOut("send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE>> %.2f / %.2f", fCurGage, fMaxGage);
}

// ============================================================================
// ReleaseInvokedOptionEffect - IDA: CMoverEx::ReleaseInvokedOptionEffect
// 精确还原 - 释放已触发的选项效果
// ============================================================================
void CMoverEx::ReleaseInvokedOptionEffect(int nConditionType) {
    // TODO: 完整实现需要遍历 m_listOptionEffect 并移除匹配的效果
    (void)nConditionType;
}

// IDA: ?SetSimpleDefenseType@CMover@@QEAAXE@Z (0x14053350)
// 设置简单防御类型
void CMover::SetSimpleDefenseType(std::uint8_t byType) {
    m_byDefenseType = byType;
}

// IDA: ?UpdateDefenseType@CMover@@UEAAXXZ
// 更新防御类型（基类空实现）
void CMover::UpdateDefenseType() {
    // Base class does nothing
}

// IDA: ?ClearBuffStatusBySlot@CMoverEx@@UEAAXG_N@Z (0x14038DA80)
// IDA 精确还原 - 清除指定槽位的Buff状态
void CMoverEx::ClearBuffStatusBySlot(std::uint16_t nBuffSlot, bool bExcuteOutSkill) {
    if (nBuffSlot >= 50) {
        return;
    }

    std::uint16_t nBuffIndex = m_stBuffState[nBuffSlot].nBuffIndex;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    TB_BUFF* pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(nBuffIndex);
    if (!pBuffTable) {
        return;
    }

    std::uint32_t dwOwnerID = m_stBuffState[nBuffSlot].dwID;

    // 发送Buff删除包
    send_eSUB_CMD_BUFF_DELETE(this, m_stBuffState[nBuffSlot].nBuffIndex, dwOwnerID, bExcuteOutSkill,
                              m_stBuffState[nBuffSlot].bySendType);

    // 更新Buff能力（清除效果）
    UpdateBuffAbility(m_stBuffState[nBuffSlot], false);

    // 清除Buff状态
    m_stBuffState[nBuffSlot].Clear();

    // 更新Buff计数
    UpdateBuffCount(pBuffTable->Buff_Type, -1);

    // 处理防御类型变化
    if (pBuffTable->EffectType_01 == 4 || pBuffTable->EffectType_01 == 5) {
        UpdateDefenseType();
    }

    // 处理防御禁用Buff
    if (pBuffTable->EffectType_01 == 6) {
        UpdateDefenseDisableBuff();
    }

    // 处理Buff动作
    if (m_nMotionClass == m_nBuffMotion) {
        m_nBuffMotion = -1;
        ClearMotion();
    }

    // 发送更新Buff能力
    SendUpdateBuffAbility();

    // 执行调用Buff
    if (bExcuteOutSkill && pBuffTable->Call_Out_Buff && pBuffTable->Call_Out_Buff != nBuffIndex) {
        SetBuffStatus(pBuffTable->Call_Out_Buff, dwOwnerID, true);
    }

    // 清除Hit调用Buff索引
    if (m_nHitCallBuffIndex == pBuffTable->Hit_Call_Buff) {
        m_nHitCallBuffIndex = 0;
    }
}

// Note: ApplyDefenseChangeInfo is now implemented in MoverLinkStubs.cpp

// IDA: ?ClearMotion@CMover@@QEAAXXZ (0x14036D0F0)
// 清除动画状态（基类空实现）
void CMover::ClearMotion() {
    // Base class empty implementation - CMoverEx has full implementation
}

// IDA: ?CheckMoveCollision@CMover@@QEAAPEAV1@AEAVhkvVec3@@@Z (0x1403681B0)
// IDA 精确还原 - 检查移动碰撞
CMover* CMover::CheckMoveCollision(hkvVec3& vDestPos) {
    // IDA: 检查 Actor 类型是否为 2 (monster)
    if (XActor::GetType() != 2) {
        return nullptr;
    }

    // IDA: 检查碰撞是否启用
    if (!m_bCollisionEnable) {
        return nullptr;
    }

    // IDA: 检查是否保持额外移动
    if (m_bKeepMovingExtra) {
        return nullptr;
    }

    // IDA: RTTI dynamic_cast 到 CMonster
    CMonster* pMonster = dynamic_cast<CMonster*>(this);
    if (!pMonster) {
        return nullptr;
    }

    // IDA: 检查是否是跟随者
    if (pMonster->IsFollower()) {
        return nullptr;
    }

    // IDA: 检查是否是巡逻怪物
    CAi* pAi = pMonster->GetAi();
    if (pAi && pAi->IsPatrolMonster()) {
        return nullptr;
    }

    // IDA: 初始化最近距离和目标
    float nearFactor = 5000.0f;
    CMover* pClosestTargetEntity = nullptr;
    hkvVec3 vClosestPos(0.0f, 0.0f, 0.0f);

    // IDA: 获取当前位置
    hkvVec3 vPos = vDestPos;

    // IDA: 扫描附近的 Actor
    std::vector<CMover*> vecGameObjList;
    XArea::ScanGridOrigin(this, 2, 3u, vecGameObjList);

    // IDA: 遍历找到的对象
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMover* pOtherActor = *it;
        if (!pOtherActor) {
            continue;
        }

        // IDA: 检查是否是有效碰撞目标
        bool bCheckActor = false;

        // IDA: 类型 0 (玩家) 是有效目标
        if (pOtherActor->XActor::GetType() == 0) {
            bCheckActor = true;
        } else if (pOtherActor->XActor::GetType() == 2) {
            // IDA: 类型 2 (怪物) 需要检查是否是防御对象
            CMonster* pOtherMonster = dynamic_cast<CMonster*>(pOtherActor);
            if (pOtherMonster && pOtherMonster->IsDefenseObject()) {
                bCheckActor = true;
            }
        }

        // IDA: 检查目标是否有效
        if (bCheckActor) {
            // IDA: 检查是否存活且不是状态 2 (死亡)
            if (pOtherActor->IsLive() && !pOtherActor->IsStatus(2u)) {
                // IDA: 获取目标位置
                const hkvVec3& otherPos = pOtherActor->GetPosition();

                // IDA: 计算距离 (忽略 Z 轴)
                hkvVec3 vOffset = vPos - otherPos;
                vOffset.z = 0.0f;
                float targetDist = vOffset.getLength();

                // IDA: 更新最近目标
                if (nearFactor > targetDist) {
                    vClosestPos = otherPos;
                    nearFactor = targetDist;
                    pClosestTargetEntity = pOtherActor;
                }
            }
        }
    }

    // IDA: 检查最近目标是否在碰撞范围内
    if (pClosestTargetEntity) {
        // IDA: 计算到最近目标的距离
        hkvVec3 v20 = vPos - vClosestPos;
        v20.z = 0.0f;
        float fDist = v20.getLength();

        // IDA: 获取目标胶囊半径
        float fOtherRadius = pClosestTargetEntity->GetHavokCapsuleRadius();

        // IDA: 检查是否在碰撞范围内 (半径之和 + 5 + 5)
        if ((fOtherRadius + m_fCapsuleRadius + 5.0f + 5.0f) >= fDist) {
            return pClosestTargetEntity;
        }
    }

    return nullptr;
}

// IDA: ?ClearExtraMoving@CMover@@UEAAXXZ (0x140189390)
// IDA 精确还原 - 清除额外移动数据
void CMover::ClearExtraMoving() {
    m_stExtMovingVal.Clear();
}

// IDA: ?GetBoneYaw@CMover@@QEAAMH@Z (0x140368880)
// IDA 精确还原 - 获取骨骼偏航角
float CMover::GetBoneYaw(int idx) {
    if (m_pCurMotionEvent) {
        return VAnimationInfo::GetBoneRotaion(m_pCurMotionEvent, idx, m_fAnimationTime);
    }
    return 0.0f;
}

// IDA: ?CheckAnimationEnd@CMover@@QEAAXXZ (0x140367c80)
// IDA 精确还原 - 检查动画结束
void CMover::CheckAnimationEnd() {
    // 检查动画状态
    if (!m_bAnimChanged && m_bAnimPlay && m_pCurMotionEvent && m_pCurMotionEvent->fAnimationLength > 0.0f) {
        // 获取时间增量
        VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
        float fDeltaTime = pTimer->GetTimeDifference();
        float fPrevTime = m_fAnimationTime;

        // 更新动画时间
        m_fAnimationTime = fPrevTime + fDeltaTime * m_fAnimSpeed;

        // 检查动画是否结束
        if (m_fAnimationTime >= m_pCurMotionEvent->fAnimationLength) {
            if (m_pCurMotionEvent->eEndofAnimation) {
                // 动画结束，停止在最后一帧
                m_fAnimationTime = m_pCurMotionEvent->fAnimationLength;
                m_fAnimPercentTime = 1.0f;
                this->ClearMotion();
                return;
            }
            // 循环动画
            m_fAnimationTime = m_fAnimationTime - m_pCurMotionEvent->fAnimationLength;
        }

        // 处理动画偏移（如果不跳过）
        if (!m_bSkipAnimOffset) {
            hkvVec3 vOffset;
            VAnimationInfo::GetOffsetDelta(m_pCurMotionEvent, &vOffset, fPrevTime, m_fAnimationTime);

            if (!vOffset.isZero(0.0f)) {
                // 根据朝向旋转偏移
                hkvMat3 matRot;
                matRot.setFromEulerAngles(0.0f, 0.0f, GetOrientationYaw());
                vOffset = matRot.transformDirection(vOffset);

                // 计算目标位置
                hkvVec3 vDestPos = GetPosition() + vOffset;

                // 检查是否在飞行
                bool bFlying = IsFlying();
                if (!bFlying) {
                    GetHeight(&vDestPos, 200.0f);
                }

                // 检查碰撞
                CMover* pCollideActor = CheckMoveCollision(vDestPos);
                if (pCollideActor) {
                    // 检查是否是怪物与任务目标碰撞
                    CMonster* pMonster = dynamic_cast<CMonster*>(this);
                    if (pMonster) {
                        std::uint32_t TargetID = GetTargetID();
                        UXActorID ActorID = pCollideActor->GetActorID();
                        std::uint32_t QuestID = CQuestCondition::GetQuestID(ActorID);
                        if (TargetID == QuestID) {
                            m_bSkipAnimOffset = 1;
                            send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, GetPosition(), false);
                            return;
                        }
                    }
                }

                // 检查移动目标位置
                if (!CheckMoveDestPos(&vDestPos, bFlying, 0)) {
                    m_bSkipAnimOffset = 1;
                    send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(this, vDestPos, false);
                }

                // 执行移动
                Move(vDestPos);
            }
        }

        // 更新动画百分比
        m_fAnimPercentTime = m_fAnimationTime / m_pCurMotionEvent->fAnimationLength;
    }
}

// IDA: ?SetBuffTime@CMover@@QEAAXFME@Z (0x14036b0f0)
// IDA 精确还原 - 设置Buff时间
void CMover::SetBuffTime(std::int16_t nIndex, float fTime, std::uint8_t byCount) {
    if (nIndex < 0 || nIndex >= 50) {
        return;
    }

    m_stBuffState[nIndex].fLifeTime = fTime;
    m_stBuffState[nIndex].byCount = byCount;

    send_eSUB_CMD_BUFF_UPDATE(
        this,
        static_cast<std::int16_t>(m_stBuffState[nIndex].nBuffIndex),
        fTime,
        static_cast<std::int8_t>(byCount),
        m_stBuffState[nIndex].dwID,
        m_stBuffState[nIndex].bySendType,
        m_stBuffState[nIndex].bShow);
}

// IDA: ?send_eSUB_CMD_BUFF_UPDATE@CMover@@QEAAXPEAV1@GMEKE_N@Z (0x1403729E0)
// IDA 精确还原 - 发送Buff更新包
void CMover::send_eSUB_CMD_BUFF_UPDATE(CMover* pMover, std::int16_t wBuffID, float fTime,
                                        std::int8_t byCount, std::uint32_t dwOwnerID,
                                        std::uint8_t bySendType, bool bShow) {
    // IDA: 创建数据包 - main=6, sub=0x14
    XSendPacket xPacket(6, 0x14);

    // IDA: 获取 ActorID 并获取 QuestID
    UXActorID uxActorID = pMover->GetActorID();
    int nQuestID = CQuestCondition::GetQuestID(uxActorID);

    // IDA: 序列化数据
    xPacket << nQuestID;
    xPacket << wBuffID;
    xPacket << fTime;
    xPacket << byCount;
    xPacket << dwOwnerID;
    xPacket << bShow;

    // IDA: 根据 sendType 发送
    if (bySendType) {
        CGocNetwork::Send(this, xPacket);
    } else {
        CGocNetwork::SendBroadCast(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);
    }

    DebugOut("send_eSUB_CMD_BUFF_UPDATE>> %d / %.2f / %d", static_cast<std::uint16_t>(wBuffID), fTime, static_cast<std::uint8_t>(byCount));
}

// IDA: ?send_eSUB_CMD_BUFF_CHANGE@CMover@@QEAAXPEAV1@GGMEKE@Z (0x140372BB0)
// IDA 精确还原 - 发送Buff变更包
void CMover::send_eSUB_CMD_BUFF_CHANGE(CMover* pMover, std::int16_t wBuffID, std::int16_t wNewBuffID,
                                        float fTime, std::int8_t byCount, std::uint32_t dwOwnerID,
                                        std::uint8_t bySendType) {
    // IDA: 创建数据包 - main=6, sub=0x16
    XSendPacket xPacket(6, 0x16);

    // IDA: 获取 ActorID 并获取 QuestID
    UXActorID uxActorID = pMover->GetActorID();
    int nQuestID = CQuestCondition::GetQuestID(uxActorID);

    // IDA: 序列化数据
    xPacket << nQuestID;
    xPacket << wBuffID;
    xPacket << wNewBuffID;
    xPacket << fTime;
    xPacket << byCount;
    xPacket << dwOwnerID;

    // IDA: 根据 sendType 发送
    if (bySendType) {
        CGocNetwork::SendAfterLoading(this, xPacket);
    } else {
        CGocNetwork::SendBroadCast(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);
    }

    DebugOut("eSUB_CMD_BUFF_CHANGE>> %d->%d / %.2f / %d", wBuffID, wNewBuffID, fTime, byCount);
}

// IDA: ?SetStat@CMover@@QEAAXHM@Z (0x14036e290)
// IDA 精确还原 - 设置属性值
void CMover::SetStat(std::uint32_t iIndex, float fVal) {
    std::tr1::shared_ptr<CGocAttribute> pAttr;
    GetGOC<CGocAttribute>(&pAttr, 0);

    if (pAttr) {
        pAttr->SetStat(iIndex, fVal, false);  // Third parameter is bSync
        if (iIndex == 3) {
            pAttr->SetFlagUseST();
        }
    }
}

// ============================================================================
// CMover::GetMoveIdleInfo
// IDA: ?GetMoveIdleInfo@CMover@@QEAAXAEAUPS_MOVE_IDLE@@M@Z @ 0x140373B50
// ============================================================================
void CMover::GetMoveIdleInfo(PS_MOVE_IDLE& stMoveIdle, float fMoveDelayTime) {
    stMoveIdle.dwActorID = GetID();
    hkvVec3 vPos = GetPosition();
    stMoveIdle.fX = vPos.x;
    stMoveIdle.fY = vPos.y;
    stMoveIdle.fZ = vPos.z;
    stMoveIdle.fMoveingYaw = GetMovingYaw();
    stMoveIdle.dwAnimationIdx = GetAnimationIdx();
    stMoveIdle.fMoveDelayTime = fMoveDelayTime;
}

// ============================================================================
// CMoverEx::CalcFlyVelocity
// IDA: ?CalcFlyVelocity@CMoverEx@@QEAAMM@Z (0x140395CA0)
// IDA 精确还原 - 计算飞行速度
// ============================================================================
float CMoverEx::CalcFlyVelocity(float fDesiredHeight) {
    if (!IsFlying() || m_fJumpDelta < 0.0f) {
        m_fJumpDelta = 0.0f;
    }

    if (fDesiredHeight + m_fJumpDelta <= m_fFlyMaxHeight) {
        return std::sqrt(fDesiredHeight * 2.0f * m_fFlyGravity);
    }

    float fAdjustedHeight = m_fFlyMaxHeight - m_fJumpDelta;
    if (fAdjustedHeight > 0.0f) {
        return std::sqrt(fAdjustedHeight * 2.0f * m_fFlyGravity);
    }

    return 0.0f;
}

// ============================================================================
// CMoverEx::CalcFlyHeight
// IDA: ?CalcFlyHeight@CMoverEx@@QEAAMMM@Z (0x140395D60)
// IDA 精确还原 - 计算飞行高度
// ============================================================================
float CMoverEx::CalcFlyHeight(float fDesiredHeight, float fTime) {
    (void)fDesiredHeight;  // Not used in calculation
    return m_fFlyVelocity * fTime - 0.5f * m_fFlyGravity * fTime * fTime;
}

// ============================================================================
// CMover::SetupPhysicsAndBound
// IDA: ?SetupPhysicsAndBound@CMover@@QEAAXMM@Z @ 0x140367910
// IDA 精确还原 - 设置物理边界
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
// CMover::GetCellID
// IDA: ?GetCellID@CMover@@QEAAKXZ @ 0x140280CC0
// ============================================================================
std::uint32_t CMover::GetCellID() const {
    return m_dwCellID;
}

// ============================================================================
// CMover::SetCellID
// IDA: ?SetCellID@CMover@@QEAAXK@Z @ 0x140280CE0
// ============================================================================
void CMover::SetCellID(std::uint32_t dwID) {
    m_dwCellID = dwID;
}

// ============================================================================
// CMover::SetAnimInfoKey
// IDA: ?SetAnimInfoKey@CMover@@QEAAXPEAV?$map@VVString@@K...@@Z @ 0x140154D0
// Sets animation info key map
// ============================================================================
void CMover::SetAnimInfoKey(std::map<class VString, std::uint32_t>* mapInfo) {
    m_mapAnimInfoKey = mapInfo;
}

// ============================================================================
// CMover::SetAnimInfoString
// IDA: ?SetAnimInfoString@CMover@@QEAAXPEAV?$map@KVString@@...@@Z @ 0x140154B0
// Sets animation info string map
// ============================================================================
void CMover::SetAnimInfoString(std::map<std::uint32_t, class VString>* mapInfo) {
    m_mapAnimInfoString = mapInfo;
}

// End of file

// End of file - CMoverEx functions are in MoverEx.cpp
// Collision functions are in MoverCollision.cpp


// ============================================================================
// GetBuffStatus(int) - 按索引获取 buff 状态 (IDA 0x14070AB00)
// ============================================================================
tagBUFF_STATE* CMover::GetBuffStatus(int nVal) {
    return &m_stBuffState[nVal];
}

