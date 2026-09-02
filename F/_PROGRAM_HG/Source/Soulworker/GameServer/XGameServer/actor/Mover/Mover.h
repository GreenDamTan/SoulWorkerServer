#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <memory>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>  // for SYSTEMTIME
#endif

#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"  // for hkvVec3, hkvMat3, SDefenseChangeInfo, VString, CActionBuffer
#include "Soulworker/GameServer/XCore/VisionEngineTypes/VisBaseEntity.h"  // for VisBaseEntity_cl base class
#include "Soulworker/GameServer/XCore/VisionEngineTypes/VPublicTransport_cl.h"  // for VPublicTransport_cl member
#include "Soulworker/GameServer/XGameServer/BuffState.h"  // for tagBUFF_STATE, tagMOVE_POS, tagEXTRA_MOVEPOS, tagTIME_SLOW
#include "Soulworker/GameServer/XCore/VisionEngineTypes/SHitPartsInfo.h"  // for SHitPartsInfo
#include "Soulworker/GameServer/XGameServer/actor/component/GOComponent.h"  // for GOComponent
#include "Soulworker/GameServer/XGameServer/StatusEffect.h"  // for EFFECT_CONDITION_TYPE, EFFECT_INVOKE_TYPE, EFFECT_STATUS_TYPE
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"  // for UXActorID, XVec3

// Forward declarations for types defined in other headers (avoid circular dependencies)
struct tagACTION_DAMAGE;  // defined in Monster.h
struct TB_BUFF;  // defined in DBLoadTable.h with macro guards

// Forward declarations
class VType;  // Vision Engine type system
class VAnimationInfo;
class VActionResourceLump;
class TB_SKILL;
class TB_AKASHIC_RECORDS;
class TB_AURA;
class TB_DIVERGENCE;
class TB_DECK_BONUS;
class CMySkillList;
class CWayPoint;
class CSector;
class VGameTrapObject;
class hkaiPointCloudSilhouetteGenerator;
class VPublicTransport_cl;
class AttackJudgmentTrigger;
class SubordinationComboTrigger;
class DetachTrigger;
class LinkSkillTrigger;
class CheckAttackSkillTrigger;  // defined in VisionEngineTypes.h
struct TB_MONSTER;
struct TB_NPC;
struct TB_CHARACTER_INFO;
struct tagHIT_COLLISION_DATA;
struct SDelayedProjectile;
struct SContinuousMelee;
struct SFilterData;
struct SHitPartsInfo;

// ST_MONSTER_DAMAGE_INFO - Monster damage info (size 24 bytes per PDB/IDA)
struct ST_MONSTER_DAMAGE_INFO {
    std::uint32_t dwUCID = 0;
    std::uint32_t _pad0 = 0;
    std::int64_t nDamage = 0;
    std::uint8_t byClass = 0;
    std::uint8_t _pad1[7] = {};
};
static_assert(sizeof(ST_MONSTER_DAMAGE_INFO) == 24, "ST_MONSTER_DAMAGE_INFO size must match PDB");

// Forward declaration for action trigger (used in SContinuousMelee)
class VAnimationInfo;

/**
 * @brief SContinuousMeleeInfo - Continuous melee info inside trigger
 * IDA: Inferred from CheckContinuousMelee usage
 */
struct SContinuousMeleeInfo {
    float fIntervalTime = 0.0f;     // Interval between attacks
};

/**
 * @brief SContinuousMelee - Continuous melee attack structure
 * IDA: Inferred from CheckContinuousMelee (0x140373EE0)
 * Used in m_vContinuousMelee vector
 */
struct SContinuousMelee {
    float fRemainIntervalTime = 0.0f;   // Remaining interval time
    float fRemainLifeTime = 0.0f;       // Remaining lifetime
    AttackJudgmentTrigger* pTrigger = nullptr; // Attack judgment trigger pointer
    std::uint32_t nSkillID = 0;         // Skill ID
};

/**
 * @brief SDelayBuff - Delayed buff structure
 * IDA: Used in m_vecDelayBuff
 */
struct SDelayBuff {
    std::uint32_t dwBuffIndex = 0;
    std::uint32_t dwOwnerID = 0;
    float fDelayTime = 0.0f;
    // TODO: 汇编还原 - need to verify actual structure from IDA
};

struct tagACTION_BUFFER;
// tagEXTRA_MOVEPOS and tagTIME_SLOW are defined in VisionEngineTypes.h (included at line 11)

// Forward declaration for TB_CREATEOPTION
struct TB_CREATEOPTION;

/**
 * @brief SOptionEffect - Option effect structure
 * IDA: 0x1403A1B90 (constructor), 0x1403A1C00 (destructor)
 */
struct SOptionEffect {
    std::uint32_t dwEquipedIndex = 0;     // Equipped option index
    std::uint32_t dwTargetMoverID = 0;    // Target mover ID
    float fOptionValue = 0.0f;            // Option value
    float fReferanceValue = 0.0f;         // Reference value
    float fAppliedValue = 0.0f;           // Applied value
    TB_CREATEOPTION* pOptionTable = nullptr;  // Option table pointer
    std::uint32_t dwLifeTime = 0;         // Lifetime
    float fCurTime = 0.0f;                // Current time

    // Constructor - IDA 0x1403A1B90
    SOptionEffect();

    // Destructor - IDA 0x1403A1C00
    ~SOptionEffect();
};

/**
 * @brief ST_MOVE - Move packet structure
 * IDA: struct at 0x14036EAC0 (send_eSUB_CMD_MOVE)
 * Size: 56 bytes
 */
struct ST_MOVE {
    std::uint32_t dwActorID = 0;        // offset 0
    std::uint32_t _padding0 = 0;        // offset 4 (padding for alignment)
    std::int64_t nMapID = 0;            // offset 8
    float fPosX = 0.0f;                 // offset 16
    float fPosY = 0.0f;                 // offset 20
    float fPosZ = 0.0f;                 // offset 24
    float fYaw = 0.0f;                  // offset 28
    float fTargetPosX = 0.0f;           // offset 32
    float fTargetPosY = 0.0f;           // offset 36
    std::uint8_t byRunBit = 0;          // offset 40
    std::uint8_t _padding1[3] = {};     // offset 41-43 (padding)
    float fPitch = 0.0f;                // offset 44
    float fMoveSpeed = 0.0f;            // offset 48
    std::uint8_t byChangeMotion = 0;    // offset 52
    bool bShouldUpdatePos = false;      // offset 53
    std::uint8_t _padding2[2] = {};     // offset 54-55 (padding)
};

static_assert(sizeof(ST_MOVE) == 56, "ST_MOVE size must match IDA");
static_assert(offsetof(ST_MOVE, dwActorID) == 0, "ST_MOVE.dwActorID offset mismatch");
static_assert(offsetof(ST_MOVE, nMapID) == 8, "ST_MOVE.nMapID offset mismatch");
static_assert(offsetof(ST_MOVE, fPosX) == 16, "ST_MOVE.fPosX offset mismatch");
static_assert(offsetof(ST_MOVE, fYaw) == 28, "ST_MOVE.fYaw offset mismatch");
static_assert(offsetof(ST_MOVE, fTargetPosX) == 32, "ST_MOVE.fTargetPosX offset mismatch");
static_assert(offsetof(ST_MOVE, byRunBit) == 40, "ST_MOVE.byRunBit offset mismatch");
static_assert(offsetof(ST_MOVE, fPitch) == 44, "ST_MOVE.fPitch offset mismatch");
static_assert(offsetof(ST_MOVE, fMoveSpeed) == 48, "ST_MOVE.fMoveSpeed offset mismatch");
static_assert(offsetof(ST_MOVE, byChangeMotion) == 52, "ST_MOVE.byChangeMotion offset mismatch");
static_assert(offsetof(ST_MOVE, bShouldUpdatePos) == 53, "ST_MOVE.bShouldUpdatePos offset mismatch");

/**
 * @brief ST_MOVE_STOP - Move stop packet structure
 * IDA: struct at 0x14036EE90 (send_eSUB_CMD_MOVE_STOP)
 * Size: 40 bytes
 */
struct ST_MOVE_STOP {
    std::uint32_t dwActorID = 0;        // offset 0
    std::uint32_t _padding0 = 0;        // offset 4 (padding for alignment)
    std::int64_t nMapID = 0;            // offset 8
    float fPosX = 0.0f;                 // offset 16
    float fPosY = 0.0f;                 // offset 20
    float fPosZ = 0.0f;                 // offset 24
    float fYaw = 0.0f;                  // offset 28
    float fPitch = 0.0f;                // offset 32
    bool bCheckCanMove = false;         // offset 36
    std::uint8_t _padding1[3] = {};     // offset 37-39 (padding)
};

static_assert(sizeof(ST_MOVE_STOP) == 40, "ST_MOVE_STOP size must match IDA");
static_assert(offsetof(ST_MOVE_STOP, dwActorID) == 0, "ST_MOVE_STOP.dwActorID offset mismatch");
static_assert(offsetof(ST_MOVE_STOP, nMapID) == 8, "ST_MOVE_STOP.nMapID offset mismatch");
static_assert(offsetof(ST_MOVE_STOP, fPosX) == 16, "ST_MOVE_STOP.fPosX offset mismatch");
static_assert(offsetof(ST_MOVE_STOP, fYaw) == 28, "ST_MOVE_STOP.fYaw offset mismatch");
static_assert(offsetof(ST_MOVE_STOP, fPitch) == 32, "ST_MOVE_STOP.fPitch offset mismatch");
static_assert(offsetof(ST_MOVE_STOP, bCheckCanMove) == 36, "ST_MOVE_STOP.bCheckCanMove offset mismatch");

/**
 * @brief ST_MOVE_BATTLE - Battle move packet structure
 * IDA: struct at 0x14036F1E0 (send_eSUB_CMD_MOVE_BATTLE)
 * Size: 28 bytes
 */
struct ST_MOVE_BATTLE {
    std::uint32_t dwActorID = 0;        // offset 0
    float fPosX = 0.0f;                 // offset 4
    float fPosY = 0.0f;                 // offset 8
    float fPosZ = 0.0f;                 // offset 12
    float fYaw = 0.0f;                  // offset 16
    std::int32_t bBattlePose = 0;       // offset 20 (int, not bool per IDA)
    std::int32_t bPlayMotion = 0;       // offset 24 (int, not bool per IDA)
};

static_assert(sizeof(ST_MOVE_BATTLE) == 28, "ST_MOVE_BATTLE size must match IDA");
static_assert(offsetof(ST_MOVE_BATTLE, dwActorID) == 0, "ST_MOVE_BATTLE.dwActorID offset mismatch");
static_assert(offsetof(ST_MOVE_BATTLE, fPosX) == 4, "ST_MOVE_BATTLE.fPosX offset mismatch");
static_assert(offsetof(ST_MOVE_BATTLE, fYaw) == 16, "ST_MOVE_BATTLE.fYaw offset mismatch");
static_assert(offsetof(ST_MOVE_BATTLE, bBattlePose) == 20, "ST_MOVE_BATTLE.bBattlePose offset mismatch");
static_assert(offsetof(ST_MOVE_BATTLE, bPlayMotion) == 24, "ST_MOVE_BATTLE.bPlayMotion offset mismatch");

/**
 * @brief PS_MOVING_TARGET - Moving target packet structure
 * IDA: struct at 0x140373890 (send_eSUB_CMD_SKILL_MOVING_TARGET)
 * Size: 24 bytes
 * Note: UXActorID and XVec3 defined in PSCommon.h
 */
struct PS_MOVING_TARGET {
    UXActorID uxActorID;                // offset 0, size 4
    float fYaw = 0.0f;                  // offset 4
    float fDuration = 0.0f;             // offset 8
    XVec3 xExtraMove;                   // offset 12, size 12
};

static_assert(sizeof(PS_MOVING_TARGET) == 24, "PS_MOVING_TARGET size must match IDA");
static_assert(offsetof(PS_MOVING_TARGET, uxActorID) == 0, "PS_MOVING_TARGET.uxActorID offset mismatch");
static_assert(offsetof(PS_MOVING_TARGET, fYaw) == 4, "PS_MOVING_TARGET.fYaw offset mismatch");
static_assert(offsetof(PS_MOVING_TARGET, fDuration) == 8, "PS_MOVING_TARGET.fDuration offset mismatch");
static_assert(offsetof(PS_MOVING_TARGET, xExtraMove) == 12, "PS_MOVING_TARGET.xExtraMove offset mismatch");

/**
 * @brief PS_MOVING_TARGET_LIST - Moving target list packet structure
 * IDA: struct at 0x140373890 (send_eSUB_CMD_SKILL_MOVING_TARGET)
 * Size: 32 bytes (std::vector is typically 24-32 bytes depending on platform)
 */
struct PS_MOVING_TARGET_LIST {
    std::vector<PS_MOVING_TARGET> vecMovingTarget;
};

static_assert(sizeof(PS_MOVING_TARGET_LIST) == 32, "PS_MOVING_TARGET_LIST size must match IDA");

/**
 * @brief DIE_TYPE - Death type enumeration
 */
enum DIE_TYPE : std::int32_t {
    DIE_TYPE_NORMAL = 0,
    DIE_TYPE_FLY = 1,
    DIE_TYPE_KNOCKDOWN = 2,
    DIE_TYPE_DELAY = 3,
    DIE_TYPE_DESTROY = 4,
    DIE_TYPE_NOFADEOUT = 5,
    DIE_TYPE_STAY = 6,
};

/**
 * @brief ANIM_SPEED_TYPE - Animation speed type enumeration
 */
enum ANIM_SPEED_TYPE : std::int32_t {
    AST_NONE = 0,
    AST_ATTACK = 1,
    ANIM_SPEED_TYPE_NORMAL = 2,
    ANIM_SPEED_TYPE_SLOW = 3,
    ANIM_SPEED_TYPE_FAST = 4,
};

/**
 * @brief E_TEST_DAMAGE - Test damage type enumeration
 */
enum E_TEST_DAMAGE : std::int32_t {
    eTestDamage_None = 0,
    eTestDamage_Normal = 1,
    eTestDamage_Critical = 2,
};

/**
 * @brief CMover - Base class for movable entities
 *
 * CMover is the base class for all movable entities in the game,
 * including players, monsters, and NPCs. It inherits from both
 * VisBaseEntity_cl (Vision Engine) and XActor (game framework).
 *
 * IDA: struct CMover (58592 bytes)
 * Inherits from: VisBaseEntity_cl (872 bytes) + XActor (104 bytes)
 */
class CMover : public VisBaseEntity_cl, public XActor {
public:
    // IDA: ??0CMover@@QEAA@XZ (0x1403649E0)
    CMover();

    // Virtual destructor
    virtual ~CMover();

    // Virtual interface functions
    virtual void InitFunction();
    virtual void Destroy();
    // IDA: ?ThinkFunction@CMover@@UEAAXXZ
    virtual void ThinkFunction();
    // IDA: ?InitialObjectInfo@CMover@@UEAAXKHVhkvVec3@@M@Z (0x140366AD0)
    virtual void InitialObjectInfo(std::uint32_t dwID, int nTableIdx, hkvVec3 vPos, float fRot);
    // IDA: ?ActionProcess@CMover@@UEAAHF@Z @ 0x1401892D0 - returns 1 (int, not bool)
    virtual int ActionProcess(std::int16_t nAction);
    // IDA: ?Damage@CMover@@UEAAXKEEEHMAEAVhkvVec3@@@Z @ 0x140189300 - empty stub
    virtual void Damage(std::uint32_t dwAttackerID, std::uint8_t byReactionType, std::uint8_t byAttackCollision);
    // IDA: ?DamageProcessHP@CMover@@UEAAHKHHEE@Z @ 0x1401892E0 - returns 1 (int, not bool)
    // Base version with 3 parameters - returns 1
    virtual int DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage);
    // Extended version with 6 parameters (for CMonster/CUser overrides)
    // Note: Not virtual in base, derived classes have their own implementations
    int DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage,
                        std::uint8_t byDamageFlag, int nInvokeType, int nUnk);
    // IDA: ?SetDie@CMover@@UEAAXFH_N@Z @ 0x140188FE0 - empty stub
    virtual void SetDie(std::int16_t nMotionClass, int bSuicide, bool bSendPacket);
    // IDA: ?SetHP@CMover@@UEAAXH@Z @ 0x140189230 - empty stub
    virtual void SetHP(int bFreeze);
    // IDA: ?GetHP@CMover@@UEAAHXZ @ 0x140365DC0 - returns current HP
    virtual int GetHP() const;
    // IDA: ?GetStat@CMover@@QEAAMH@Z @ 0x140166360 - returns m_fAbility[iIndex]
    virtual float GetStat(int iIndex);
    // IDA: ?SetHpInfo@CMonster@@UEAAXH@Z (0x14035CDF0) - virtual, overridden by CMonster
    virtual void SetHpInfo(int nHP) { (void)nHP; }
    // IDA: ?GetSkillMgr@CMover@@UEAAPEAVCMySkillList@@XZ @ 0x140365BE0 - returns m_pSkillMgr
    virtual CMySkillList* GetSkillMgr();
    virtual void CalcTargetDamage(CMover* pAttacker, int nDamage, TB_SKILL* pSkill, float fRate, float fDamage);
    void AddActionBuffer(tagACTION_BUFFER* xAction);

    // Component access (template functions)
    // IDA pattern: ??$GetGOC@V{ComponentType}@@@CMover@@QEAA?AV?$shared_ptr@V{ComponentType}@@@tr1@std@@_N@Z
    // Template uses T::GetFamilyID() to index into m_GOComponentTable
    template<typename T>
    void GetGOC(std::shared_ptr<T>* result, bool bCanNotExist);

    template<typename T>
    bool SetGOC(std::shared_ptr<T> pComponent);

    // GetGOC specializations
    // IDA: ??$GetGOC@VCGocEntity@@@CMover@@QEAA?AV?$shared_ptr@VCGocEntity@@@tr1@std@@_N@Z (0x14022780)
    std::shared_ptr<class CGocEntity> GetGOC_Entity(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocNetwork@@@CMover@@QEAA?AV?$shared_ptr@VCGocNetwork@@@tr1@std@@_N@Z
    // Family ID = 1 (GOC_NETWORK)
    std::shared_ptr<class CGocNetwork> GetGOC_Network(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocInventory@@@CMover@@QEAA?AV?$shared_ptr@VCGocInventory@@@tr1@std@@_N@Z (0x140223D0)
    std::shared_ptr<class CGocInventory> GetGOC_Inventory(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocAttribute@@@CMover@@QEAA?AV?$shared_ptr@VCGocAttribute@@@tr1@std@@_N@Z (0x1404BFD0)
    std::shared_ptr<class CGocAttribute> GetGOC_Attribute(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocSkill@@@CMover@@QEAA?AV?$shared_ptr@VCGocSkill@@@tr1@std@@_N@Z (0x14045A60)
    std::shared_ptr<class CGocSkill> GetGOC_Skill(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocParty@@@CMover@@QEAA?AV?$shared_ptr@VCGocParty@@@tr1@std@@_N@Z (0x14045850)
    std::shared_ptr<class CGocParty> GetGOC_Party(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocForce@@@CMover@@QEAA?AV?$shared_ptr@VCGocForce@@@tr1@std@@_N@Z (0x14045900)
    std::shared_ptr<class CGocForce> GetGOC_Force(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocQuest@@@CMover@@QEAA?AV?$shared_ptr@VCGocQuest@@@tr1@std@@_N@Z (0x140577B0)
    std::shared_ptr<class CGocQuest> GetGOC_Quest(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocAchieve@@@CMover@@QEAA?AV?$shared_ptr@VCGocAchieve@@@tr1@std@@_N@Z (0x140457A0)
    std::shared_ptr<class CGocAchieve> GetGOC_Achieve(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocPost@@@CMover@@QEAA?AV?$shared_ptr@VCGocPost@@@tr1@std@@_N@Z (0x14035820)
    std::shared_ptr<class CGocPost> GetGOC_Post(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocFriend@@@CMover@@QEAA?AV?$shared_ptr@VCGocFriend@@@tr1@std@@_N@Z (0x1409BA20)
    std::shared_ptr<class CGocFriend> GetGOC_Friend(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocBooster@@@CMover@@QEAA?AV?$shared_ptr@VCGocBooster@@@tr1@std@@_N@Z (0x14045DD0)
    std::shared_ptr<class CGocBooster> GetGOC_Booster(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocRecode@@@CMover@@QEAA?AV?$shared_ptr@VCGocRecode@@@tr1@std@@_N@Z (0x14045D20)
    std::shared_ptr<class CGocRecode> GetGOC_Recode(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocAkashicRecord@@@CMover@@QEAA?AV?$shared_ptr@VCGocAkashicRecord@@@tr1@std@@_N@Z (0x14045C70)
    std::shared_ptr<class CGocAkashicRecord> GetGOC_AkashicRecord(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocWeeklyMission@@@CMover@@QEAA?AV?$shared_ptr@VCGocWeeklyMission@@@tr1@std@@_N@Z (0x14045BC0)
    std::shared_ptr<class CGocWeeklyMission> GetGOC_WeeklyMission(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocClassEvent@@@CMover@@QEAA?AV?$shared_ptr@VCGocClassEvent@@@tr1@std@@_N@Z (0x14045B10)
    std::shared_ptr<class CGocClassEvent> GetGOC_ClassEvent(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocHelper@@@CMover@@QEAA?AV?$shared_ptr@VCGocHelper@@@tr1@std@@_N@Z (0x140459B0)
    std::shared_ptr<class CGocHelper> GetGOC_Helper(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocEvent@@@CMover@@QEAA?AV?$shared_ptr@VCGocEvent@@@tr1@std@@_N@Z (0x14066360)
    std::shared_ptr<class CGocEvent> GetGOC_Event(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocLeague@@@CMover@@QEAA?AV?$shared_ptr@VCGocLeague@@@tr1@std@@_N@Z (0x1402B3150)
    std::shared_ptr<class CGocLeague> GetGOC_League(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocExchange@@@CMover@@QEAA?AV?$shared_ptr@VCGocExchange@@@tr1@std@@_N@Z (0x14021D980)
    std::shared_ptr<class CGocExchange> GetGOC_Exchange(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocDailyMission@@@CMover@@QEAA?AV?$shared_ptr@VCGocDailyMission@@@tr1@std@@_N@Z (0x140EDC60)
    std::shared_ptr<class CGocDailyMission> GetGOC_DailyMission(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocNpcAttribute@@@CMover@@QEAA?AV?$shared_ptr@VCGocNpcAttribute@@@tr1@std@@_N@Z (0x1409B880)
    std::shared_ptr<class CGocNpcAttribute> GetGOC_NpcAttribute(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocSoulMetry@@@CMover@@QEAA?AV?$shared_ptr@VCGocSoulMetry@@@tr1@std@@_N@Z (0x14015FEA0)
    std::shared_ptr<class CGocSoulMetry> GetGOC_SoulMetry(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocNpcCredit@@@CMover@@QEAA?AV?$shared_ptr@VCGocNpcCredit@@@tr1@std@@_N@Z (0x140EDE40)
    std::shared_ptr<class CGocNpcCredit> GetGOC_NpcCredit(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocAttendance@@@CMover@@QEAA?AV?$shared_ptr@VCGocAttendance@@@tr1@std@@_N@Z (0x1403C5C20)
    std::shared_ptr<class CGocAttendance> GetGOC_Attendance(bool bCreateIfNull);

    // IDA: ??$GetGOC@VCGocMyroom@@@CMover@@QEAA?AV?$shared_ptr@VCGocMyroom@@@tr1@std@@_N@Z (0x14021E3C0)
    std::shared_ptr<class CGocMyroom> GetGOC_Myroom(bool bCreateIfNull);

    // Utility functions
    std::uint32_t GetTargetID() const { return m_dwTargetID; }
    void SetTargetID(std::uint32_t dwID) { m_dwTargetID = dwID; }

    std::uint32_t GetHitID() const { return m_dwHitID; }
    void SetHitID(std::uint32_t dwID) { m_dwHitID = dwID; }

    // IDA: ?GetDefenseType@CMover@@QEAAEXZ @ 0x14019B970
    std::uint8_t GetDefenseType() const { return m_byDefenseType; }

    // IDA: ?GetCurMotionEvent@CMover@@QEAAPEBVVAnimationInfo@@XZ @ 0x140199E30
    const VAnimationInfo* GetCurMotionEvent() const { return m_pCurMotionEvent; }

    // IDA: ?ClearExtraMoving@CMover@@UEAAXXZ @ 0x140189390
    virtual void ClearExtraMoving();

    // IDA: ?IsDie@CMover@@QEAAHXZ @ 0x140366E40
    bool IsDie() const;
    // IsLive is the inverse of IsDie
    bool IsLive() const { return !IsDie(); }
    // IDA: ?IsKnockDown@CMover@@QEAAHXZ @ 0x1403671C0
    bool IsKnockDown() const;
    // IDA: ?IsFlying@CMover@@QEAAHXZ @ 0x140367080
    bool IsFlying() const;
    // IDA: ?IsHit@CMover@@QEAAHXZ @ 0x140367230
    bool IsHit() const;
    // IDA: ?IsHitDown@CMover@@QEAAHXZ @ 0x140367270
    bool IsHitDown() const;
    // IDA: ?IsGeneralHit@CMover@@QEAAHXZ @ 0x140367410
    bool IsGeneralHit() const;
    // IDA: ?IsFlyHit@CMover@@QEAAHXZ @ 0x140367480
    bool IsFlyHit() const;
    // IDA: ?IsDashing@CMover@@QEAAHXZ @ 0x1403674F0
    bool IsDashing() const;
    // IDA: ?IsCounterAttackHit@CMover@@UEAAHXZ @ 0x140367360
    bool IsCounterAttackHit() const;
    // IDA: ?IsMoving@CMover@@QEAAHXZ @ 0x140279610
    bool IsMoving();

    // IDA: ?GetLevelForStat@CMover@@UEAAEXZ @ 0x140366D30
    virtual std::uint8_t GetLevelForStat();

    // IDA: ?GetMaxHP@CMover@@UEAAHXZ @ 0x140365E90
    virtual int GetMaxHP();
    // IDA: ?GetComboCount@CMover@@UEAAHXZ @ 0x1400F2CF0
    virtual int GetComboCount();
    // IDA: ?GetMovingYaw@CMover@@UEAAMXZ @ 0x140374220
    virtual float GetMovingYaw();
    // IDA: ?GetOrientationYaw@CMover@@UEAAMXZ @ 0x140374220
    virtual float GetOrientationYaw();
    // IDA: ?GetHavokCapsuleRadius@CMover@@QEAAMXZ @ 0x140275870
    float GetHavokCapsuleRadius();
    // Super armor gage getters
    // IDA: ?GetMaxSuperArmorGage@CMover@@QEAAMXZ @ 0x1402A5050
    float GetMaxSuperArmorGage() const;
    // IDA: ?GetCurSuperArmorGage@CMover@@QEAAMXZ @ 0x1402A5030
    float GetCurSuperArmorGage() const;
    // IDA: ?SetCurSuperArmorGage@CMover@@QEAAXM@Z @ 0x140353C60
    void SetCurSuperArmorGage(float fCurSuperArmorGage);
    // IDA: ?GetRecoverySuperArmorTime@CMover@@QEAAMXZ (0x140353FE0)
    // 精确还原: return m_fRecoverySuperArmorTime
    float GetRecoverySuperArmorTime();
    // IDA: ?GetCreatePos@CMover@@QEAAAEAVhkvVec3@@XZ @ 0x1402752B0
    hkvVec3& GetCreatePos();
    void SetCreatePos(const hkvVec3& vPos);
    // IDA: ?GetMotionClass@CMover@@QEAAFXZ @ 0x140275270
    std::int16_t GetMotionClass();
    // IDA: ?SetHitStatus@CMover@@QEAAXF@Z @ 0x1403E1BF0
    void SetHitStatus(std::int16_t nHitStatus);
    // IDA: ?GetSkillCoolDownRate@CMover@@QEAAMXZ @ 0x1402C7240
    float GetSkillCoolDownRate() const;
    // IDA: ?GetSkillBloodRate@CMover@@QEAAMXZ @ 0x1403A2410
    float GetSkillBloodRate() const;
    // IDA: ?GetCellID@CMover@@QEAAKXZ @ 0x140280CC0
    std::uint32_t GetCellID() const;
    // IDA: ?SetCellID@CMover@@QEAAXK@Z @ 0x140280CE0
    void SetCellID(std::uint32_t dwID);
    // IDA: ?GetHitList@CMover@@QEAAXAEAV?$list@UST_MONSTER_DAMAGE_INFO@@...@@Z (0x140374220)
    void GetHitList(std::list<struct ST_MONSTER_DAMAGE_INFO>& listHit);
    // IDA: ?GetAnimationIdx@CMover@@QEAAKXZ @ 0x140275370
    std::uint32_t GetAnimationIdx();
    // GetDefaultAnimStep - getter for m_byDefaultAnimStep
    std::uint8_t GetDefaultAnimStep() const { return m_byDefaultAnimStep; }
    // IDA: ?GetTargetDestPos@CMover@@QEAAEXZ @ 0x14027FC80
    std::uint8_t GetTargetDestPos();
    // IDA: ?SetCurSkillTableIdx@CMover@@QEAAXH@Z @ 0x1402753F0
    void SetCurSkillTableIdx(int nIdx);
    // IDA: ?SetCollisionEnable@CMover@@QEAAXHH@Z @ 0x14036C340
    void SetCollisionEnable(int nEnable, int nUnk);
    // IDA: ?SetMoveingInFly@CMover@@QEAAXH@Z
    void SetMoveingInFly(int bFlying);
    // IDA: ?SceneChanged@CMover@@QEAAXXZ
    void SceneChanged();
    // IDA: ?SetPositionXVec3@CMover@@QEAAXAEAVhkvVec3@@@Z
    void SetPositionXVec3(const hkvVec3& vPos);
    // IDA: ?SetOrientationYaw@CMover@@QEAAXM@Z @ 0x1402C6C60
    void SetOrientationYaw(float fYaw);
    // IDA: ?GetTableID@CMover@@UEAAHXZ @ 0x1400F2CF0
    virtual int GetTableID();
    // IDA: ?GetLevel@CMover@@UEAAEXZ @ 0x140366CB0
    virtual std::uint8_t GetLevel();
    // IDA: ?GetClass@CMover@@UEAAEXZ @ 0x140366C30
    virtual std::uint8_t GetClass();

    // ID methods
    // IDA: ?GetID@CMover@@QEAAKXZ - returns ActorID
    std::uint32_t GetID();

    // Item rate methods
    // IDA: ?GetItemRateFlag@CMover@@UEAAEXZ
    virtual std::uint8_t GetItemRateFlag();

    // Damage calculation methods
    // IDA: ?GetDamageCalc@CMover@@UEAAHHEMH_N@Z @ 0x140375000
    virtual int GetDamageCalc(int nAP, std::uint8_t byType, float fReduceRate);

    // Boss rate methods
    // IDA: ?GetBossAttackAddRate@CMover@@UEAAMXZ
    virtual float GetBossAttackAddRate();
    // IDA: ?GetBossAttackedDownRate@CMover@@UEAAMXZ
    virtual float GetBossAttackedDownRate();
    // IDA: ?GetAllAttackAddRate@CMover@@UEAAMXZ
    virtual float GetAllAttackAddRate();

    // Expose XActor::GetArea() - CMover inherits from XActor
    using XActor::GetArea;

    // IDA: ?SetupPhysicsAndBound@CMover@@QEAAXMM@Z @ 0x140367910
    void SetupPhysicsAndBound(float fCollisionRadius, float fCollisionHeight);
    // IDA: ?IsDamageMotionDisplay@CMover@@UEAAHE@Z @ 0x140367BD0
    bool IsDamageMotionDisplay(std::uint8_t byAttackCollision) const;
    // IDA: ?IsRegisterAnimInfo@CMover@@QEAAHFFVVString@@H@Z @ 0x140367AE0
    bool IsRegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, const class VString& strAnimName);
    // IDA: ?GetBoneCurrentWorldSpaceTranslation@CMover@@QEAAHHVhkvVec3@@AEAV2@@Z @ 0x140368690
    bool GetBoneCurrentWorldSpaceTranslation(int idx, const hkvVec3& vBoneCenterPos, hkvVec3& vPos);
    // IDA: ?CheckAnimationEnd@CMover@@QEAAXXZ @ 0x140367C80
    void CheckAnimationEnd();
    // IDA: ?CheckMoveCollision@CMover@@QEAAPEAV1@AEAVhkvVec3@@@Z @ 0x1403681B0
    CMover* CheckMoveCollision(hkvVec3& vDestPos);
    // IDA: ?GetItemRateResultWeapon@CMover@@QEAAHEV?$shared_ptr@VCGocAttribute@@@tr1@std@@_N@Z @ 0x1403675F0
    std::uint32_t GetItemRateResultWeapon(std::uint8_t byTargetLevel, std::shared_ptr<class CGocAttribute> pAttr, bool bCritical);
    // IDA: ?GetItemRateResultGear@CMover@@QEAAHEV?$shared_ptr@VCGocAttribute@@@tr1@std@@@Z @ 0x140367780
    std::uint32_t GetItemRateResultGear(std::uint8_t byTargetLevel, std::shared_ptr<class CGocAttribute> pAttr);
    // IDA: ?IsActivateSkillUnlockBuff@CMover@@QEAA_NPEAUTB_SKILL@@@Z @ 0x140367550
    bool IsActivateSkillUnlockBuff(const struct TB_SKILL* pTBSkill);
    // IDA: ?SetupAnimation@CMover@@QEAAXXZ @ 0x140367980
    void SetupAnimation();

    // IDA: ?SetAnimSpeed@CMover@@QEAAXM@Z @ 0x140368CC0
    void SetAnimSpeed(float fSpeed);
    // IDA: ?SetCurrentSequenceTime@CMover@@QEAAXM@Z @ 0x140368BE0
    void SetCurrentSequenceTime(float fTime);
    // IDA: ?SetCurrentSequencePosition@CMover@@QEAAXM@Z @ 0x140368C60
    void SetCurrentSequencePosition(float fPos);
    // IDA: ?GetBoneYaw@CMover@@QEAAMH@Z @ 0x140368880
    float GetBoneYaw(int idx);
    // IDA: ?GetCurrentAnimationLength@CMover@@QEAAMXZ @ 0x140368B90
    float GetCurrentAnimationLength() const;
    // IDA: ?SetSlowTime@CMover@@QEAAXMM@Z @ 0x140368AA0
    void SetSlowTime(float fTime, float fSpeed);
    // IDA: ?GetAnimStirng@CMover@@QEAAPEADK@Z @ 0x1403688D0
    const char* GetAnimStirng(std::uint32_t dwAnimKey);
    // IDA: ?GetAnimIndex@CMover@@QEAAKVVString@@@Z @ 0x140368960
    std::uint32_t GetAnimIndex(const class VString& strAnimName);
    // IDA: ?AnimKeyToMotion@CMover@@QEAAFK@Z @ 0x140368A80
    std::int16_t AnimKeyToMotion(std::uint32_t dwAnimKey);
    // IDA: ?IsFixedLoopAnim@CMover@@QEAAHK@Z @ 0x14036C550
    bool IsFixedLoopAnim(std::uint32_t dwAnimID);
    // IDA: ?FindBuffStatus@CMover@@QEAAHGK@Z @ 0x14036A420
    int FindBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwAttackerID) const;
    // IDA: ?FindBuffByGroupID@CMover@@QEAAHGK@Z @ 0x14036A4C0
    int FindBuffByGroupID(std::uint16_t nGroupID, std::uint32_t dwAttackerID) const;
    // IDA: ?FindBuffByEffectType@CMover@@QEAAHEG@Z @ 0x14036A560
    int FindBuffByEffectType(std::uint8_t byBuffEffect, std::uint16_t nExceptBuffIndex) const;
    // IDA: ?GetEmptyBuffSlot@CMover@@QEAAHXZ @ 0x14036A810
    int GetEmptyBuffSlot() const;
    // IDA: ?IsHaveImunityInvincibleBuff@CMover@@QEAAHG@Z @ 0x14036A640
    bool IsHaveImunityInvincibleBuff(std::uint16_t nExceptBuffIndex) const;
    // IDA: ?GetBuffAllByGroup@CMover@@QEAAHGAEAV?$vector@HV?$allocator@H@std@@@std@@@Z @ 0x14036A700
    bool GetBuffAllByGroup(std::uint16_t nGroupID, std::vector<int>& vecBuffIndex) const;
    // IDA: ?CheckPassDebuff@CMover@@QEAAHG@Z @ 0x14036A750
    bool CheckPassDebuff(std::uint16_t nBuffIndex) const;
    // IDA: ?ResetAllBuff@CMover@@QEAAXXZ @ 0x14036A860
    void ResetAllBuff();
    // IDA: ?ClearBuffByType@CMover@@QEAAXE@Z @ 0x14036A920
    void ClearBuffByType(std::uint8_t byType);
    // IDA: ?ClearBuffByEffectType@CMover@@QEAAXE@Z @ 0x14036A990
    void ClearBuffByEffectType(std::uint8_t byType);
    // IDA: ?AllBuffClear@CMover@@QEAAXE@Z @ 0x14036AAB0
    void AllBuffClear(std::uint8_t byReason);
    // IDA: ?ClearBuffStatusBySlot@CMover@@UEAAXG_N@Z @ 0x140377550
    virtual void ClearBuffStatusBySlot(std::uint16_t nBuffSlot, bool bExcuteOutSkill);
    // IDA: ?ClearBuffStatus@CMover@@UEAAXG_NK@Z @ 0x140374FC0
    virtual void ClearBuffStatus(std::uint16_t nBuffIndex, bool bExcuteOutSkill, std::uint32_t dwOwnerID);
    // IDA: ?SetBuffStatus@CMover@@UEAAHGK_N@Z @ 0x140374FE0
    virtual bool SetBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwOwnerID, bool bShowBuff);
    // IDA: ?IsClearBuff@CMover@@UEAAHGE@Z @ 0x1403774F0
    virtual bool IsClearBuff(std::uint16_t nBuffIndex, std::uint8_t byReason);
    // IDA: ?CheckBuffByLocation@CMover@@UEAAXPEAVXArea@@@Z @ 0x14036ABF0
    virtual void CheckBuffByLocation(XArea* pArea);
    // IDA: ?IsCanApplyBuff@CMover@@UEAAHGPEAVXArea@@@Z @ 0x14036ACD0
    virtual bool IsCanApplyBuff(std::uint16_t nBuffIndex, XArea* pArea) const;
    // IDA: ?UpdateBuffCount@CMover@@QEAAXEH@Z @ 0x14036AE80
    void UpdateBuffCount(std::uint8_t byBuffType, int nAddCount);
    // IDA: ?GetBuffCategory@CMover@@QEAAEE@Z @ 0x14036B000
    std::uint8_t GetBuffCategory(std::uint8_t byBuffEffectType) const;
    // IDA: ?GetResistStatIndexByBuff@CMover@@QEAAHE@Z @ 0x14036B070
    int GetResistStatIndexByBuff(std::uint8_t byBuffEffectType) const;
    // IDA: ?SetBuffTime@CMover@@QEAAXFME@Z @ 0x14036B0F0
    void SetBuffTime(std::int16_t nIndex, float fTime, std::uint8_t byCount);
    // IDA: ?UpdateDefenseDisableBuff@CMover@@QEAAXXZ @ 0x14036B4D0
    void UpdateDefenseDisableBuff();
    // IDA: ?UpdateDefenseType@CMover@@UEAAXXZ - virtual, overridden by CMoverEx
    virtual void UpdateDefenseType();
    // IDA: ?CanUseItem@CMover@@QEAAHKAEAK@Z @ 0x14036B8D0
    bool CanUseItem(std::uint32_t dwID, std::uint32_t& dwError) const;
    // IDA: ?ProcessExtraMoving@CMover@@UEAAXXZ @ 0x14036BC20
    virtual void ProcessExtraMoving();
    // IDA: ?ReleaseExtraMoving@CMover@@UEAAXXZ @ 0x14036C120
    virtual void ReleaseExtraMoving();
    // IDA: ?CancelAttackFromDamage@CMover@@UEAAXXZ @ 0x14035B520 (CMonster override)
    virtual void CancelAttackFromDamage() {}
    // IDA: ?ChangeSequence@CMover@@QEAAXKKH@Z @ 0x14036C500
    void ChangeSequence(std::uint32_t dwOldAnimID, std::uint32_t dwNewAnimID, int bResetPlay);
    // IDA: ?CreateAkashicActionInfo@CMover@@QEAAXPEBD@Z @ 0x14036C800
    void CreateAkashicActionInfo(const char* szAnimName);
    // IDA: ?GetActionDesc@CMover@@QEAAPEBVVAnimationInfo@@PEBD@Z @ 0x14036C920
    const VAnimationInfo* GetActionDesc(const char* szAnimName) const;
    // IDA: ?ChangeActionTrigger@CMover@@QEAAXPEBD@Z @ 0x14036CA80
    void ChangeActionTrigger(const char* szAnimName);
    // IDA: ?DeleteActionBuffer@CMover@@UEAAXXZ - virtual, overridden by CMoverEx
    virtual void DeleteActionBuffer() {}
    // IDA: ?SetMovePosition@CMover@@QEAAXMM@Z @ 0x14036CC00
    void SetMovePosition(float fXpos, float fYpos);
    // IDA: ?MoveingClientStop@CMover@@QEAAXXZ @ 0x14036CD40
    void MoveingClientStop();
    // IDA: ?IsEnemy@CMover@@UEAAHPEAV1@@Z @ 0x14036CD80
    virtual bool IsEnemy(CMover* pMover) const;
    // IDA: ?IsFriend@CMover@@UEAAHPEAV1@@Z
    virtual int IsFriend(CMover* pMover);
    // IDA: ?IsFriendForChain@CMover@@UEAAHPEAV1@@Z
    virtual int IsFriendForChain(CMover* pMover);
    // IDA: ?IsEnemyForChain@CMover@@UEAAHPEAV1@@Z
    virtual int IsEnemyForChain(CMover* pMover);
    // IDA: ?IsParty@CMover@@UEAAHPEAV1@@Z
    virtual int IsParty(CMover* pMover);
    // IDA: ?CheckMonsterInteractObject@CMover@@UEAAHPEAV1@@Z
    virtual int CheckMonsterInteractObject(CMover* pMover);
    // IDA: ?DebugOut@CMover@@UEAAXPEBDZZ - virtual debug output
    virtual void DebugOut(const char* szFormat, ...);
    // IDA: ?IsGazeMoving@CMover@@QEAAHXZ (0x140375200) - returns m_bGazeMoving
    int IsGazeMoving();
    // IDA: ?IsBattlePose@CMoverEx@@UEAA_NXZ (0x140189000) - virtual, overridden by CMoverEx
    virtual bool IsBattlePose();
    // IDA: ?GetLookPitch@CMoverEx@@UEAAMXZ (0x140189270) - virtual, overridden by CMoverEx
    virtual float GetLookPitch();
    // IDA: ?SetLookPitch@CMoverEx@@UEAAXM@Z (0x14037F560) - virtual set pitch
    virtual void SetLookPitch(float fPitch);

    // Virtual methods for ActionDestToEntity (IDA 0x14000a280)
    // These are called via vtable in ActionDestToEntity
    // IDA: ?GetPvpCondition@CMoverEx@@UEAAHXZ (0x1401891C0) - virtual, overridden by CMoverEx
    virtual int GetPvpCondition() { return 0; }
    // IDA: ?GetActionCondition@CMoverEx@@UEAAHXZ (0x1401891E0) - virtual, overridden by CMoverEx
    virtual int GetActionCondition() { return 0; }
    // IDA: ?GetDivergenceValue@CMoverEx@@UEAAHXZ (0x140398CF0) - virtual, overridden by CMoverEx
    virtual int GetDivergenceValue() { return 0; }
    // IDA: ?GetCombatType@CMoverEx@@UEAAHXZ - virtual, overridden by CMoverEx
    virtual int GetCombatType() { return 0; }
    // IDA: ?GetSkillChargeStep@CMoverEx@@UEAAEXZ - virtual, overridden by CMoverEx
    virtual std::uint8_t GetSkillChargeStep() { return 0; }
    // IDA: ?GetSkillLevel@CMoverEx@@UEAAEXZ - virtual, overridden by CMoverEx
    virtual std::uint8_t GetSkillLevel() { return 0; }
    // IDA: ?IsSendProjectilePacket@CMoverEx@@UEAA_NPEAUAttackJudgmentTrigger@@@Z (0x140366B40) - virtual, overridden by CMoverEx
    virtual bool IsSendProjectilePacket(AttackJudgmentTrigger* pTrigger) { (void)pTrigger; return false; }
    // IDA: ?GetSkillLoopTime@CMoverEx@@UEAAMXZ (0x14015ED0) - virtual, overridden by CMoverEx
    virtual float GetSkillLoopTime() { return 0.0f; }
    // IDA: ?SetWaitSuboInputActionProcess@CMoverEx@@UEAAXH@Z - virtual, overridden by CUser
    virtual void SetWaitSuboInputActionProcess(int bWait) { (void)bWait; }
    // IDA: ?CheckReactionTarget@CMover@@UEAAHHPEAV1@_N@Z @ 0x14036CE70
    virtual bool CheckReactionTarget(int iTargetType, CMover* pTargetMover, bool bChekcForChain = false);
    // IDA: ?GetHeight@CMover@@QEAA_NAEAVhkvVec3@@M@Z @ 0x14036D130
    bool GetHeight(hkvVec3* vPos, float fTestHeight);
    // IDA: ?GetExtraMovePos@CMover@@QEAAAEAUtagEXTRA_MOVEPOS@@XZ @ 0x140529C0
    tagEXTRA_MOVEPOS& GetExtraMovePos() { return m_stExtMovingVal; }
    const tagEXTRA_MOVEPOS& GetExtraMovePos() const { return m_stExtMovingVal; }
    // IDA: ?GetMoverObject@CMover@@QEAAPEAV1@K@Z @ 0x14036D200
    CMover* GetMoverObject(std::uint32_t dwID);
    // IDA: ?SetFlyState@CMover@@QEAAX_N@Z @ 0x14036D300
    void SetFlyState(bool bFly);
    // IDA: ?SetInvincibleActor@CMover@@QEAAX_N@Z
    void SetInvincibleActor(bool bInvincible);
    // IDA: ?IsInvincibleActor@CMover@@QEAA_NXZ
    bool IsInvincibleActor() const;
    // IDA: ?SetMoveSpeed@CMover@@QEAAXM@Z
    void SetMoveSpeed(float fSpeed);
    // IDA: ?GetMoveSpeed@CMover@@QEAAMXZ
    float GetMoveSpeed();
    // IDA: ?SetDieFadeTime@CMover@@QEAAXM@Z @ 0x1403A2370
    void SetDieFadeTime(float fTime);
    // IDA: ?FindTargetPos@CMover@@QEAAEPEAV1@@Z @ 0x14036D400
    std::uint8_t FindTargetPos(CMover* pMover);
    // IDA: ?FindTargetPos@CMover@@QEAAEMMW4E_MOVESIDE_TYPE@@@Z @ 0x14036D700
    std::uint8_t FindTargetPos(float fAngleMin, float fAngleMax, int eIgnoreMoveSide);
    // IDA: ?GetTargetAngle@CMover@@QEAAME@Z @ 0x14036DA00
    float GetTargetAngle(std::uint8_t byPos) const;
    // IDA: ?SetTargetPosFlag@CMover@@QEAAXE@Z @ 0x14036DA30
    void SetTargetPosFlag(std::uint8_t byPos);
    // IDA: ?ClearTargetPosFlag@CMover@@QEAAXE@Z @ 0x14036DB00
    void ClearTargetPosFlag(std::uint8_t byPos);
    // IDA: ?GetYawFromVector@CMover@@SAMAEBVhkvVec3@@@Z @ 0x14036DC00
    static float GetYawFromVector(const hkvVec3& vDir);
    // IDA: ?IsValidPos@CMover@@SA_NAEAUXVec3@@@Z @ 0x14036DD00
    static bool IsValidPos(const hkvVec3& vPos);
    // IDA: ?IsValidPos@CMover@@SA_NMM@Z @ 0x14036DD40
    static bool IsValidPos(float fX, float fY);
    // IDA: ?SetReactionRate@CMover@@QEAAXM@Z
    void SetReactionRate(float fRate);
    // IDA: ?ResetAkashicActionInfo@CMover@@QEAAXXZ @ 0x14036C610
    void ResetAkashicActionInfo();
    // IDA: ?Move@CMover@@UEAAGAEAUXVec3@@M@Z @ 0x14036DE00
    virtual std::uint16_t Move(hkvVec3& vNextPos, float fRot = 0.0f);
    // IDA: ?CheckMoveDestPos@CMover@@UEAA_NAEAVhkvVec3@@HH@Z @ 0x14036DF00
    virtual bool CheckMoveDestPos(hkvVec3* vDestPos, int bFlying, int bDontCareCurve);
    // IDA: ?GetSGAbsorbRate@CMover@@QEAAMXZ @ 0x14036E200
    float GetSGAbsorbRate();
    // IDA: ?SetStat@CMover@@QEAAXHM@Z @ 0x14036E300
    void SetStat(std::uint32_t iIndex, float fVal);
    // IDA: ?SetContinousCost@CMover@@QEAAXHM@Z @ 0x14036E330
    void SetContinousCost(int iIndex, float fVal);
    // IDA: ?SetAllowPassiveType@CMover@@QEAAXH@Z @ 0x14036E3B0
    void SetAllowPassiveType(int nType);
    // IDA: ?CreateRandomTrapIndex@CMover@@QEAAHXZ @ 0x14036E400
    int CreateRandomTrapIndex();
    // IDA: ?SendUpdateStat@CMover@@QEAAXH@Z @ 0x14036E500
    void SendUpdateStat(std::uint32_t iIndex);
    // IDA: ?DeleteDelayedProjectile@CMover@@QEAAXPEAUSDelayedProjectile@@@Z @ 0x14036E600
    void DeleteDelayedProjectile(SDelayedProjectile* pDelayedProjectile);
    // IDA: ?CheckDelayedProjectile@CMover@@QEAAXM@Z @ 0x14036E700
    void CheckDelayedProjectile(float fDeltaTime);
    // IDA: ?CheckContinuousMelee@CMover@@QEAAXM@Z @ 0x140373EE0
    void CheckContinuousMelee(float fDeltaTime);
    // IDA: ?GetFilterData@CMover@@QEAAXHAEAH00@Z @ 0x14036E900
    void GetFilterData(std::uint32_t nSkillID, int* nFilterData1, int* nFilterData2, int* nFilterData3);
    // IDA: ?SetFilterData@CMover@@QEAAXHHHH@Z @ 0x14036EA00
    void SetFilterData(std::uint32_t nSkillID, std::uint32_t nFilterData1,
                       std::uint32_t nFilterData2, std::uint32_t nFilterData3);
    // IDA: ?send_eSUB_CMD_MOVE@CMover@@QEAAXPEAV1@MME@Z @ 0x14036EB00
    void send_eSUB_CMD_MOVE(CMover* pMover, float fTargetPosx, float fTargetPosy, std::uint8_t byRunBit);
    // IDA: ?send_eSUB_CMD_MOVE_STOP@CMover@@QEAAXPEAV1@@Z @ 0x14036EE90
    void send_eSUB_CMD_MOVE_STOP(CMover* pMover);
    // IDA: ?send_eSUB_CMD_MOVE_BATTLE@CMover@@QEAAXPEAV1@_N@Z @ 0x14036F300
    void send_eSUB_CMD_MOVE_BATTLE(CMover* pMover, bool bPlayMotion);
    // IDA: ?send_eSUB_CMD_MOVE_GAZE@CMover@@QEAAXPEAV1@@Z @ 0x14036F500
    void send_eSUB_CMD_MOVE_GAZE(CMover* pMover);
    // IDA: ?send_eSUB_CMD_MOVE_TRACE@CMover@@QEAAXPEAV1@@Z @ 0x14036F700
    void send_eSUB_CMD_MOVE_TRACE(CMover* pMover, float a3 = 0.0f);
    // IDA: ?send_eSUB_CMD_MOVE_IDLE@CMover@@QEAAXPEAV1@M@Z @ 0x14036FD50
    void send_eSUB_CMD_MOVE_IDLE(CMover* pMover, float fMoveDelayTime);
    // IDA: ?send_eSUB_CMD_MOVE_INFO@CMover@@QEAAXPEAV1@KK@Z @ 0x14036FEF0
    void send_eSUB_CMD_MOVE_INFO(CMover* pMover, std::uint32_t dwType, std::uint32_t dwVal);
    // IDA: ?send_eSUB_CMD_MOVE_STIFFEN@CMover@@QEAAXPEAV1@MM@Z @ 0x14036FFF0
    void send_eSUB_CMD_MOVE_STIFFEN(CMover* pMover, float fRate, float fTime);
    // IDA: ?send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA@CMover@@QEAAXPEAV1@VhkvVec3@@_N@Z @ 0x140370100
    void send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(CMover* pMover, hkvVec3 vPos, bool bForced);
    // IDA: ?send_eSUB_CMD_MOVE_UPDATE_DIR@CMover@@QEAAXPEAV1@_N@Z @ 0x140370390
    void send_eSUB_CMD_MOVE_UPDATE_DIR(CMover* pMover, bool bDirect);
    // IDA: ?send_eSUB_CMD_MOVE_DROP@CMover@@QEAAXPEAV1@_N@Z @ 0x140370570
    void send_eSUB_CMD_MOVE_DROP(CMover* pMover, bool bSync);
    // IDA: ?SyncMove@CMover@@QEAAXXZ @ 0x14036EA30
    void SyncMove();
    // IDA: ?send_eSUB_CMD_MOVE_GRAP@CMover@@QEAAXPEAV1@VhkvVec3@@@Z @ 0x1403706E0
    void send_eSUB_CMD_MOVE_GRAP(CMover* pMover, hkvVec3 vPos);
    // IDA: ?send_eSUB_CMD_MOVE_ATTACED_BT@CMover@@QEAAXPEAV1@0VhkvVec3@@M@Z @ 0x140370800
    void send_eSUB_CMD_MOVE_ATTACED_BT(CMover* pAttackerMover, CMover* pTargetMover,
                                        hkvVec3 vAttachDir, float fAttachedDirDist);
    // IDA: ?send_eSUB_CMD_SKILL_DEFENCE_TYPE@CMover@@QEAAXPEAV1@E_N@Z @ 0x140371EA0
    void send_eSUB_CMD_SKILL_DEFENCE_TYPE(CMover* pMover, std::uint8_t byDefenceType, bool bAdd);
    // IDA: ?send_eSUB_CMD_SKILL_MOVING_TARGET@CMover@@QEAAXPEAV1@AEAV?$vector@UPS_MOVING_TARGET@@V?$allocator@UPS_MOVING_TARGET@@@std@@@std@@@Z @ 0x140373890
    void send_eSUB_CMD_SKILL_MOVING_TARGET(std::vector<PS_MOVING_TARGET>& vecMovingTargetList);

    // IDA: ?send_eSUB_CMD_ACTIVE_SKILL@CMover@@QEAAXPEAV1@KE@Z
    void send_eSUB_CMD_ACTIVE_SKILL(CMover* pMover, std::uint32_t nSkillID, std::uint8_t byAngleAttackType);

    // IDA: ?send_eSUB_CMD_ACTION_SKILL@CMover@@QEAAXPEAV1@HFAEAVhkvVec3@@EG_N@Z @ 0x1403716C0
    void send_eSUB_CMD_ACTION_SKILL(CMover* pMover, std::uint32_t nSkillID, std::int16_t nTriggerIdx,
                                     hkvVec3* vPos, std::uint8_t byAttackTargetCnt,
                                     std::uint16_t wContinousHit, bool bPenetrate);

    // IDA: ?send_eSUB_CMD_CHAIN@CMover@@QEAAXPEAV1@KGPAVhkvVec3@@0HK@Z @ 0x1403723A0
    void send_eSUB_CMD_CHAIN(CMover* pMover, std::uint32_t nSkillID, std::int16_t nTriggerIdx,
                              hkvVec3* vPos, hkvVec3* vDir, std::uint32_t nSessionID, std::uint32_t dwTargetID);

    // === Move Idle Info ===
    // IDA: ?GetMoveIdleInfo@CMover@@QEAAXAEAUPS_MOVE_IDLE@@M@Z @ 0x140373B50
    void GetMoveIdleInfo(PS_MOVE_IDLE& stMoveIdle, float fMoveDelayTime);
    
    // === Network Sync Functions ===
    // IDA: ?send_eSUB_CMD_SKILL_SYNC_POSITION@CMover@@QEAAXPEAV1@AEAVhkvVec3@@@Z @ 0x1403733E0
    void send_eSUB_CMD_SKILL_SYNC_POSITION(CMover* pMover, const hkvVec3& vPos);
    // IDA: ?send_eSUB_CMD_BUFF_UPDATE@CMover@@QEAAXPEAV1@GMEKE_N@Z @ 0x1403729E0
    void send_eSUB_CMD_BUFF_UPDATE(CMover* pMover, std::int16_t wBuffID, float fTime,
                                    std::int8_t byCount, std::uint32_t dwOwnerID,
                                    std::uint8_t bySendType, bool bShow);
    // IDA: ?send_eSUB_CMD_BUFF_CHANGE@CMover@@QEAAXPEAV1@GGMEKE@Z @ 0x140372BB0
    void send_eSUB_CMD_BUFF_CHANGE(CMover* pMover, std::int16_t wBuffID, std::int16_t wNewBuffID,
                                    float fTime, std::int8_t byCount, std::uint32_t dwOwnerID,
                                    std::uint8_t bySendType);
    // IDA: ?send_eSUB_CMD_BUFF_DELETE@CMover@@QEAAXPEAV1@GK_NE@Z @ 0x140372D90
    void send_eSUB_CMD_BUFF_DELETE(CMover* pMover, std::int16_t wBuffID, std::uint32_t dwOwnerID,
                                    bool bExcuteOutSkill, std::uint8_t bySendType);
    // IDA: ?send_eSUB_CMD_BUFF_DAMAGE@CMover@@QEAAXPEAV1@GHHK@Z (0x140372F60)
    void send_eSUB_CMD_BUFF_DAMAGE(CMover* pMover, std::int16_t wBuffID, int nDamage, int nCurHP, std::uint32_t dwOwnerID);

    // IDA: ?send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE@CMover@@QEAAXPEAV1@MM@Z
    // 发送超级护甲值包
    void send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE(CMover* pMover, float fCurGage, float fMaxGage);
    
    // IDA: ?CollisionShereToLine@CMover@@QEAAHAEAVhkvVec3@@M00@Z @ 0x14036A080
    bool CollisionShereToLine(const hkvVec3& vSphereCenter, float fRadius,
                               const hkvVec3& vLineStart, const hkvVec3& vLineEnd);
    // IDA: ?FindLineCircleIntersections@CMover@@QEAAHMMMMMMM@Z @ 0x14036A120
    int FindLineCircleIntersections(float cx, float cy, float radius,
                                     float x1, float y1, float x2, float y2);
    // IDA: ?CollisionCylinderToBox@CMover@@QEAAHAEAVhkvVec3@@M00M@Z @ 0x140369B60
    bool CollisionCylinderToBox(const hkvVec3& vCylinderCenter, float fRadius,
                                 const hkvVec3& vBoxCenter, const hkvVec3& vBoxSize,
                                 float fRotation);
    // IDA: ?IsInRectCircle@CMover@@QEAAHAEAVhkvVec3@@00M@Z @ 0x140369CA0
    bool IsInRectCircle(const hkvVec3& vLeftTop, const hkvVec3& vRightBottom,
                         const hkvVec3& vCircleCenter, float fRadius);
    // IDA: ?IsAttackDecision@CMover@@QEAAEQEAUtagATTACK_AREA@@@Z @ 0x140368D70
    std::uint8_t IsAttackDecision(struct tagATTACK_AREA* pAttackArea);

    // IDA: ?SetHitCylinder@CMover@@QEAAXMM@Z (0x14015BF0)
    void SetHitCylinder(float fRadius, float fHeight);

    // IDA: ?SetAnimInfoKey@CMover@@QEAAXPEAV?$map@VVString@@K...@@Z (0x140154D0)
    void SetAnimInfoKey(std::map<class VString, std::uint32_t>* pMap);

    // IDA: ?SetAnimInfoString@CMover@@QEAAXPEAV?$map@KVString@@...@@Z (0x140154B0)
    void SetAnimInfoString(std::map<std::uint32_t, class VString>* pMap);

    // IDA: ?send_eSUB_CMD_MONSTER_INVISIBLE@CMover@@QEAAXPEAV1@EKHH@Z @ 0x140370BA0
    void send_eSUB_CMD_MONSTER_INVISIBLE(CMover* pMover, std::uint8_t byInvisible, std::uint32_t dwFlag,
                                          int nType, int nValue);

    // Trace bone methods
    void ClearTraceBoneName();
    void RegisterTraceBoneName(const class VString& strBoneName);
    int GetTraceBoneListIndex(const char* pBoneName);

    // Action buffer methods
    void ClearActionBuffer();

    // Random trap methods
    int GetRandomTrapIndex();

    // IDA: ?SetHitCollisionData@CMover@@QEAAXPEAUtagHIT_COLLISION_DATA@@@Z (0x14015BD0)
    void SetHitCollisionData(tagHIT_COLLISION_DATA* pData);

    // Attack height check
    // IDA: ?IsAttackHeight@CMover@@QEAAQEAUtagATTACK_AREA@@AEAVhkvVec3@@AEAH@Z (0x140368CE0)
    std::uint8_t IsAttackHeight(const struct tagATTACK_AREA& stArea, hkvVec3& vPos, int& nResult);

    // IDA: ?GetHitCollisionCount@CMover@@QEAAHXZ (0x14066B90)
    int GetHitCollisionCount();

    // IDA: ?GetHitCollisionData@CMover@@QEAAPEAUtagHIT_COLLISION_DATA@@XZ
    tagHIT_COLLISION_DATA* GetHitCollisionData();

    // IDA: ?GetHitCylinderHeight@CMover@@QEAAMXZ
    float GetHitCylinderHeight();

    // IDA: ?GetHitCylinderRadius@CMover@@QEAAMXZ
    float GetHitCylinderRadius();

    // IDA: ?GetCapsuleHeight@CMover@@QEAAMXZ
    float GetCapsuleHeight();

    // IDA: ?GetCapsuleRadius@CMover@@QEAAMXZ
    float GetCapsuleRadius();

    // IDA: ?SetCapsuleRadius@CMover@@QEAAXM@Z
    void SetCapsuleRadius(float fRadius);

    // IDA: ?SetCapsuleHeight@CMover@@QEAAXM@Z
    void SetCapsuleHeight(float fHeight);

    // IDA: ?SetHitCylinderRadius@CMover@@QEAAXM@Z
    void SetHitCylinderRadius(float fRadius);

    // IDA: ?SetHitCylinderHeight@CMover@@QEAAXM@Z
    void SetHitCylinderHeight(float fHeight);

    // IDA: ?AddExtraMoving@CMover@@QEAAXMMM@Z
    void AddExtraMoving(float fX, float fY, float fDuration);

    // IDA: ?IsOnGroundState@CMover@@QEAAHXZ
    int IsOnGroundState();

    // IDA: ?SetOnGroundState@CMover@@QEAAXH@Z
    void SetOnGroundState(int bOnGround);

    // IDA: ?SetGroundPosZ@CMover@@QEAAXM@Z
    void SetGroundPosZ(float fPosZ);

    // IDA: ?ClearMotion@CMover@@QEAAXXZ (0x14036D0F0)
    // 清除动画状态（基类空实现，CMoverEx有完整实现）
    void ClearMotion();

    // IDA: ?GetTableIDString@CMover@@QEAAPEBDXZ (0x1406CE70)
    const char* GetTableIDString();

    // Position from VisObject3D_cl
    hkvVec3 GetPosition() const;

    // IDA: ?ScanGridOrigin@CMover@@QEAAXHHAEBV?$vector@PEAVXActor@@V?$allocator@PEAVXActor@@@std@@@std@@@Z
    void ScanGridOrigin(int nRange, int nType, std::vector<XActor*>* pResultList);

    // IDA: ?SetNoSkillCostSG@CMover@@QEAAX_N@Z (0x140478E0)
    void SetNoSkillCostSG(bool bNoCost);

    // IDA: ?RemoveTargetDestPos@CMover@@QEAAXXZ
    void RemoveTargetDestPos();

    // IDA: ?IsAllowPassiveType@CMover@@QEAAHH@Z (0x14063670)
    bool IsAllowPassiveType(int nType);

    // IDA: ?SetSimpleDefenseType@CMover@@QEAAXE@Z (0x14053350)
    void SetSimpleDefenseType(std::uint8_t byType);

    // IDA: ?IsKnockDown@CMover@@QEAAHXZ (0x140661C0)
    int IsKnockDown_int();

    // IDA: ?IsImmunityStatus@CMover@@QEAAHXZ @ 0x140364700
    bool IsImmunityStatus() const;

    // IDA: ?SetImmunityStatus@CMover@@QEAAXK@Z (0x1402A4F90)
    void SetImmunityStatus(std::uint32_t dwStatus);

    // IDA: ?ClearImmunityStatus@CMover@@QEAAXK@Z @ 0x140353040
    void ClearImmunityStatus(std::uint32_t dwStatus);

    // IDA: ?GetJumpSpeed@CMover@@QEAAMXZ @ 0x1402C7330
    float GetJumpSpeed() const;

    // IDA: ?GetDieDelayTime@CMover@@QEAAMXZ @ 0x1402C7BD0
    float GetDieDelayTime() const;

    // IDA: ?GetDecreaseStaminaRate@CMover@@QEAAMXZ @ 0x1402C7EE0
    float GetDecreaseStaminaRate() const;

    // IDA: ?GetIgnoreSkillCost@CMover@@QEAAHXZ @ 0x1402C7F00
    int GetIgnoreSkillCost() const;

    // IDA: ?IsNoSkillCostSG@CMover@@QEAA_NXZ @ 0x1402C7F20
    bool IsNoSkillCostSG() const { return m_bNoSkillCostSG; }

    // IDA: ?AddSummonMobList@CMover@@QEAAXK@Z @ 0x1402C7CC0
    void AddSummonMobList(std::uint32_t dwMobID);

    // IDA: ?SetProtectionAggroRatio@CMover@@QEAAXM@Z @ 0x1403655C0
    void SetProtectionAggroRatio(float ratio);

    // IDA: ?SetDmgMotionFlag@CMover@@QEAAXE@Z @ 0x1403655E0
    void SetDmgMotionFlag(std::uint8_t byFlag);

    // IDA: ?SetWeightRank@CMover@@QEAAXD@Z @ 0x140364D40
    void SetWeightRank(std::int8_t cVal);

    // IDA: ?SetParentSkillTableIdx@CMover@@QEAAXH@Z @ 0x140364610
    void SetParentSkillTableIdx(int nVal);

    // IDA: ?GetCurSkillTableIdx@CMover@@QEAAHXZ @ 0x140364650
    int GetCurSkillTableIdx() const;

    // IDA: ?SetIgnoreAggroDebuff@CMover@@QEAAXH@Z @ 0x1402A67F0
    void SetIgnoreAggroDebuff(int bApply);

    // IDA: ?GetBloodDebuffOwnerID@CMover@@QEAAKXZ @ 0x1403A23B0
    std::uint32_t GetBloodDebuffOwnerID();

    // IDA: ?MoveingValueClear@CMover@@QEAAXXZ @ 0x1402A4BE0
    void MoveingValueClear();

    // IDA: ?StopMoving@CMover@@UEAAX_N@Z - virtual function, overridden by CMonster/CUser/CNpc
    virtual void StopMoving(bool bSendPacket);

    // IDA: ?GetBuffStatus@CMover@@QEAAPEAUtagBUFF_STATE@@XZ (0x140529C0)
    tagBUFF_STATE* GetBuffStatus();
    // IDA: ?GetBuffStatus@CMover@@QEAAPEAUtagBUFF_STATE@@H@Z (0x14070AB00)
    tagBUFF_STATE* GetBuffStatus(int nVal);

    // IDA: ?GetPositionXVec3@CMover@@QEAAAEAUXVec3@@XZ @ 0x1402A5080
    // 返回 &m_vPosition (hkvVec3 布局与 XVec3 等价: 3 floats)
    XVec3& GetPositionXVec3() {
        return *reinterpret_cast<XVec3*>(&m_vPosition);
    }

protected:
    // IDA: offset 976, size 4
    float m_fLastUpdateTime;

    // Position member (from VisObject3D_cl)
    hkvVec3 m_vPosition;

    // IDA: offset 980, size 4
    float m_fLastDebugTime;

    // IDA: offset 984, size 8
    float* m_fAbility;

    // IDA: offset 992, size 4
    std::uint32_t m_dwTargetID;

    // IDA: offset 996, size 4
    std::uint32_t m_dwHitID;

    // IDA: offset 1000, size 32
    std::map<std::uint32_t, ST_MONSTER_DAMAGE_INFO> m_setHitID;

    // IDA: offset 1032, size 24
    std::list<std::uint32_t> m_listSummonMob;

    // IDA: offset 1056, size 4
    float m_fCapsuleRadius;

    // IDA: offset 1060, size 4
    float m_fCapsuleHeight;

    // IDA: offset 1064, size 4
    float m_fDieDelayTime;

    // IDA: offset 1068, size 4
    float m_fDieDelayMaxTime;

    // IDA: offset 1072, size 4
    float m_fDieFadeTime;

    // IDA: offset 1076, size 4
    int m_nDamage;

    // IDA: offset 1080, size 1
    std::uint8_t m_cWeightRank;

    // IDA: offset 1081, size 1
    std::uint8_t m_byDmgMontionFlag;

    // IDA: offset 1084, size 4
    int m_fMoving;

    // IDA: offset 1088, size 4
    int m_bGazeMoving;

    // IDA: offset 1092, size 4
    float m_fMoveSpeed;

    // IDA: offset 1096, size 1
    std::uint8_t m_byMoveDir;

    // IDA: offset 1100, size 12
    hkvVec3 m_vMoveStopCheckPos;

    // IDA: Moving yaw angle (shared with CMoverEx)
    float m_fMovingYaw = 0.0f;

    // IDA: offset 1112, size 4
    int m_nBuffTotalCnt;

    // IDA: offset 1116, size 4
    int m_nBuffCnt;

    // IDA: offset 1120, size 4
    int m_nDebuffCnt;

    // IDA: offset 1124, size 3500 (50 * 70)
    tagBUFF_STATE m_stBuffState[50];

    // IDA: offset 4624, size 4
    int m_nHitCallBuffIndex;

    // IDA: offset 4628, size 4
    float m_fLastSendMoveTime;

    // IDA: offset 4632, size 8
    tagMOVE_POS m_stMovePos;

    // IDA: offset 4640, size 8
    tagMOVE_POS m_stMoveGap;

    // IDA: offset 4648, size 8
    tagMOVE_POS m_stMoveOffset;

    // IDA: offset 4656, size 17
    tagEXTRA_MOVEPOS m_stExtMovingVal;

    // IDA: offset 4673, size 8
    tagTIME_SLOW m_stTimeSlow;

    // IDA: offset 4684, size 12
    hkvVec3 m_vPrevPos;

    // IDA: offset 4696, size 12
    hkvVec3 m_vCreatePos;

    // IDA: offset 4708, size 1
    std::uint8_t m_byDefaultAnimStep;

    // IDA: offset 4712, size 4
    float m_fAnimSpeed;

    // IDA: offset 4716, size 4
    float m_fRestoreAnimSpeed;

    // IDA: offset 4720, size 4
    ANIM_SPEED_TYPE m_eRestoreAnimSpeedType;

    // IDA: offset 4724, size 2
    std::int16_t m_nMotionClass;

    // IDA: offset 4726, size 2
    std::int16_t m_nHitStatus;

    // IDA: offset 4728, size 2
    std::int16_t m_nHitAnimCount;

    // IDA: offset 4730, size 2
    std::int16_t m_nTurnStatus;

    // IDA: offset 4732, size 4
    int m_bAnimChanged;

    // IDA: offset 4736, size 4
    int m_bSkipAnimOffset;

    // IDA: offset 4740, size 4
    int m_bAnimPlay;

    // IDA: offset 4744, size 4
    std::uint32_t m_nAnimationIdx;

    // IDA: offset 4748, size 2
    std::int16_t m_nAnimGroup;

    // IDA: offset 4752, size 4
    float m_fAnimationTime;

    // IDA: offset 4756, size 4
    float m_fAnimPercentTime;

    // IDA: offset 4760, size 12
    hkvVec3 m_vAnimLastDelta;

    // IDA: offset 4776, size 8
    VAnimationInfo* m_pCurMotionEvent;

    // IDA: offset 4784, size 8
    VActionResourceLump* m_pActionResource;

    // IDA: offset 4792, size 8
    VAnimationInfo* m_pAkashicActionInfo;

    // IDA: offset 4800, size 4
    int m_nCurSkillTableIdx;

    // IDA: offset 4804, size 4
    int m_nParentSkillTableIdx;

    // IDA: offset 4808, size 4
    float m_fReactionRate;

    // IDA: offset 4812, size 4
    float m_fFlySpeed;

    // IDA: offset 4816, size 4
    float m_fFlyDistance;

    // IDA: offset 4820, size 4
    float m_fFlyMoveDist;

    // IDA: offset 4824, size 4
    float m_fFlyYaw;

    // IDA: offset 4828, size 4
    int m_bLanded;

    // IDA: offset 4832, size 4
    int m_bOnGround;

    // IDA: offset 4836, size 4
    float m_fGroundPosZ;

    // IDA: offset 4840, size 4
    int m_bRestoreCollision;

    // IDA: offset 4844, size 4
    int m_bKeepMovingExtra;

    // IDA: offset 4848, size 1
    bool m_bMoveingInFly;

    // IDA: offset 4852, size 4
    float m_fGroundDownTime;

    // IDA: offset 4856, size 4
    float m_fHitLoopMaxTime;

    // IDA: offset 4860, size 1
    std::uint8_t m_byDownContinueDamage;

    // IDA: offset 4861, size 1
    std::uint8_t m_byDefenseType;

    // IDA: offset 4864, size 32
    std::set<int> m_setAllowPassiveType;

    // IDA: offset 4896, size 1
    std::uint8_t m_byRestoreDefenceType;

    // IDA: offset 4900, size 12
    SDefenseChangeInfo m_stDefenseChangeInfoByTrigger;

    // IDA: offset 4912, size 12
    SDefenseChangeInfo m_stDefenseChangeInfoByEffect;

    // IDA: offset 4928, size 24
    std::list<SDefenseChangeInfo*> m_listDefenseChangeInfo;

    // IDA: offset 4952, size 4
    std::uint32_t m_DefanseChangeFlag;

    // IDA: offset 4956, size 1
    std::uint8_t m_byDefaultDefenseType;

    // IDA: offset 4957, size 1
    std::uint8_t m_byRestoreDefenceTypeByTrigger;

    // IDA: offset 4958, size 1
    std::uint8_t m_byDefenseTypeDisableFlag;

    // IDA: offset 4960, size 4
    float m_fSkillCoolDownRate;

    // IDA: offset 4964, size 4
    float m_fSkillReflectRate;

    // IDA: offset 4968, size 4
    float m_fSkillBloodRate;

    // IDA: offset 4972, size 4
    int m_nAllowBloodCount;

    // IDA: offset 4976, size 4
    float m_fSkillBloodDebuffRate;

    // IDA: offset 4980, size 4
    std::uint32_t m_dwBloodDebuffOwnerID;

    // IDA: offset 4984, size 4
    float m_fSkillAbsorbSGRate;

    // IDA: offset 4988, size 4
    int m_nAllowAbsorbSGCount;

    // IDA: offset 4992, size 1
    bool m_bCheckSendAbsorbSG;

    // IDA: offset 4993, size 1
    bool m_bCheckSendHP;

    // IDA: offset 4996, size 4
    float m_fBuffSuperArmorRate;

    // IDA: offset 5000, size 4
    int m_bIgnoreSkillCost;

    // IDA: offset 5004, size 4
    float m_fDecreaseStaminaRate;

    // IDA: offset 5008, size 4
    float m_fBuffAddGoldRate;

    // IDA: offset 5012, size 4
    std::uint32_t m_dwAggroTargetID;

    // IDA: offset 5016, size 4
    int m_bCollisionEnable;

    // IDA: offset 5020, size 4
    float m_fCheckCollisionTime;

    // Collision data
    hkvVec3 m_vCollisionPoint;
    CMover* m_pCollisionTarget;
    float m_fCollisionTime;

    // IDA: offset 5024, size 4
    int m_mShaderState;

    // IDA: offset 5028, size 4
    int m_bInvincibleActor;

    // IDA: offset 5032, size 8
    std::map<class VString, std::uint32_t>* m_mapAnimInfoKey;

    // IDA: offset 5040, size 8
    std::map<std::uint32_t, class VString>* m_mapAnimInfoString;

    // IDA: offset 5048, size 32
    std::map<std::uint32_t, SFilterData> m_mapFilterData;

    // IDA: offset 5080, size 8
    tagHIT_COLLISION_DATA* m_pHitCollisionData;

    // IDA: offset 5088, size 52912
    CActionBuffer m_xActionBuffer;

    // IDA: offset 58000, size 4
    float m_fHitCylinderRadius;

    // IDA: offset 58004, size 4
    float m_fHitCylinderHeight;

    // IDA: offset 58008, size 32
    std::vector<class VString> m_vTraceBoneName;

    // IDA: offset 58040, size 12
    std::uint8_t m_byTargetPosInfo[12];

    // IDA: offset 58052, size 1
    std::uint8_t m_byTargetPosCount;

    // IDA: offset 58053, size 1
    std::uint8_t m_byTargetDestPos;

    // IDA: offset 58056, size 4
    float m_fMaxSuperArmorGage;

    // IDA: offset 58060, size 4
    float m_fCurSuperArmorGage;

    // IDA: offset 58064, size 4
    float m_fRegenSuperArmorTime;

    // IDA: offset 58068, size 4
    float m_fDefRegenSuperArmorTime;

    // IDA: offset 58072, size 4
    float m_fRecoverySuperArmorTime;

    // IDA: offset 58076, size 4
    std::uint32_t m_dwCellID;

    // IDA: offset 58080, size 8
    class VString m_strTableID;

    // IDA: offset 58088, size 4
    std::uint32_t m_uiForcedState;

    // IDA: offset 58092, size 4
    float m_fForcedStateApplyTime;

    // IDA: offset 58096, size 4
    float m_fProtectionAggroRatio;

    // IDA: offset 58104, size 32
    std::vector<SDelayedProjectile*> m_vecDelayedProjectile;

    // IDA: offset 58136, size 32
    std::vector<std::shared_ptr<class GOComponent>> m_GOComponentTable;

    // IDA: offset 58168, size 8
    CMySkillList* m_pSkillMgr;

    // IDA: offset 58176, size 4
    int m_bTraceUser;

    // IDA: offset 58184, size 264
    VPublicTransport_cl m_sPublicTransportPath;

    // IDA: offset 58448, size 1
    bool m_bPublicTransportRiding;

    // IDA: offset 58452, size 4
    float m_shCurRandomShootProjectileDirX;

    // IDA: offset 58456, size 4
    float m_shCurRandomShootProjectileDirY;

    // IDA: offset 58460, size 4
    int m_nRandomTrapIndex;

    // IDA: offset 58464, size 4
    std::uint32_t m_dwImmunityStatus;

    // IDA: offset 58468, size 1
    bool m_bIgnoreAggroDebuff;

    // IDA: offset 58472, size 32
    std::vector<SContinuousMelee*> m_vContinuousMelee;

    // IDA: offset 58504, size 4
    float m_fBossAttackAddRate;

    // IDA: offset 58508, size 4
    float m_fBossAttackedDownRate;

    // IDA: offset 58512, size 4
    float m_fSoulCostDownRate;

    // IDA: offset 58516, size 4
    float m_fAllAttackAddRate;

    // IDA: offset 58520, size 32
    std::map<std::uint32_t, std::vector<std::uint32_t>> m_mapMeleeDebuff;

    // IDA: offset 58552, size 32
    std::map<std::uint32_t, std::uint32_t> m_mapSkillUnlock;

    // IDA: offset 58584, size 1
    bool m_bNoSkillCostSG;

    // IDA: ?AnimPause@CMover@@QEAAXXZ (0x1403A2390)
    void AnimPause();
};

// TODO: 需人工审查 - CMover size check disabled until class is complete
// static_assert(sizeof(CMover) >= 58592, "CMover size check - at least 58592 bytes expected");

// ============================================================================
// CMover::GetGOC<T> template implementation
// IDA pattern: ??$GetGOC@V{ComponentType}@@@CMover@@QEAA?AV?$shared_ptr@V{ComponentType}@@@tr1@std@@_N@Z
// ============================================================================
template<typename T>
void CMover::GetGOC(std::shared_ptr<T>* result, bool bCanNotExist) {
    if (!result) return;
    
    // Get family ID from the component type
    int familyID = T::GetFamilyID();
    
    // Check bounds
    if (familyID < 0 || static_cast<size_t>(familyID) >= m_GOComponentTable.size()) {
        if (!bCanNotExist) {
            std::printf("Not Exist Component %d\n", familyID);
        }
        result->reset();
        return;
    }
    
    // Get component from table
    auto& pGOC = m_GOComponentTable[familyID];
    
    // Check if component exists
    if (!pGOC) {
        if (!bCanNotExist) {
            std::printf("Not Exist Component %d\n", familyID);
        }
        result->reset();
        return;
    }
    
    // Cast to requested type using static_pointer_cast (matches IDA std::tr1::static_pointer_cast)
    *result = std::static_pointer_cast<T>(pGOC);
}

// ============================================================================
// CMover::SetGOC<T> template implementation
// IDA pattern: ??$SetGOC@V?$shared_ptr@V{ComponentType}@@@tr1@std@@@CMover@@QEAA_NV?$
// Returns: true if set successfully, false if slot already occupied
// ============================================================================
template<typename T>
bool CMover::SetGOC(std::shared_ptr<T> pComponent) {
    if (!pComponent) return false;
    
    // Get family ID from the component type
    int familyID = T::GetFamilyID();
    
    // Check bounds
    if (familyID < 0 || static_cast<size_t>(familyID) >= m_GOComponentTable.size()) {
        return false;
    }
    
    // Check if slot is already occupied
    auto& pGOC = m_GOComponentTable[familyID];
    if (pGOC) {
        // Slot already occupied
        return false;
    }
    
    // Set the component
    pGOC = std::static_pointer_cast<GOComponent>(pComponent);
    
    // Set owner
    pComponent->SetOwnerGO(this);
    
    return true;
}

/**
 * @brief CMoverEx - Extended mover class for player characters
 *
 * CMoverEx extends CMover with additional functionality for player-specific
 * features like skill animation, movement control, and combat mechanics.
 *
 * IDA: struct CMoverEx (60392 bytes)
 * Inherits from: CMover (58592 bytes)
 */
class CMoverEx : public CMover {
public:
    // IDA: ??0CMoverEx@@QEAA@XZ (0x140379260)
    CMoverEx();

    // Virtual destructor
    virtual ~CMoverEx();

    // Expose CMover's GetArea (which uses XActor::GetArea)
    using CMover::GetArea;

    // Virtual interface overrides
    virtual void InitFunction();
    virtual void Destroy();
    virtual void OnUpdate(float fDelta);
    virtual void MessageFunction(int nMsg, __int64 wParam, __int64 lParam);
    virtual std::int16_t GetNextMotion();
    virtual bool MoveTick();
    // IDA: ?StartMoving@CMoverEx@@UEAAHXZ @ 0x1403833D0 - returns int
    virtual int StartMoving();
    virtual int GetSectorID();

    // Sector methods
    CSector* GetSector() const { return m_pSector; }
    void SetSector(CSector* pSector) { m_pSector = pSector; }

    // IDA: ?GetMaxHP@CMoverEx@@UEAAHXZ @ 0x140188410
    virtual int GetMaxHP() override;
    // IDA: ?GetMovingYaw@CMoverEx@@UEAAMXZ @ 0x140188290
    virtual float GetMovingYaw() override;
    virtual void SetSkillTable(TB_SKILL* pSkill);
    // IDA: ?SetDie@CMoverEx@@UEAAXFH_N@Z - override from CMover
    virtual void SetDie(std::int16_t nMotionClass, int bSuicide, bool bSendPacket) override;
    virtual void Damage(tagACTION_DAMAGE& stDamage, int nType, bool& bResult);
    virtual bool IsCanAttack();
    virtual void ReapllyBuffAll();
    virtual void ChargeSkillNextStep();

    // Motion and AI methods
    // IDA: ?ChangeMotion@CMoverEx@@UEAAXFHH@Z @ 0x14037B310
    virtual void ChangeMotion(std::int16_t nMotionClass, int bResetPlay, int iCallPos);
    // IDA: ?ChangeMotion@CMoverEx@@UEAAXFH@Z @ 0x14037B310 (overload with default iCallPos=0)
    void ChangeMotion(std::int16_t nMotionClass, int bResetPlay) {
        ChangeMotion(nMotionClass, bResetPlay, 0);
    }
    // IDA: ?ChangeMotion@CMoverEx@@UEAAXPEBDH@Z @ 0x14037C290 - Change motion by animation name
    void ChangeMotion(const char* pszMotionName, int bResetPlay);
    // Alias for compatibility (ChangeMotion_2 in IDA)
    void ChangeMotion_2(const char* pszMotionName, int bResetPlay) {
        ChangeMotion(pszMotionName, bResetPlay);
    }
    // IDA: ?ThinkFunction@CMoverEx@@UEAAXXZ
    virtual void ThinkFunction() override;
    // IDA: ?RealDie@CMoverEx@@UEAAXF@Z
    virtual void RealDie(std::int16_t nChangeMotion);
    // IDA: ?SetLookPitch@CMoverEx@@UEAAXM@Z (0x14037F560) - override from CMover
    virtual void SetLookPitch(float fPitch) override;
    // IDA: ?IsSuperArmorBreakMotion@CMoverEx@@QEAA_NF@Z
    bool IsSuperArmorBreakMotion(std::int16_t nMotionClass);
    // IDA: ?IsCommonMotion@CMoverEx@@QEAA_NF@Z
    bool IsCommonMotion(std::int16_t nMotionClass);
    // IDA: ?IsCanMove@CMoverEx@@QEAA_N_N@Z
    bool IsCanMove(bool bCheckTurnMotion);
    // IDA: ?SetDieReason@CMoverEx@@QEAAXEH@Z @ 0x14039D710
    void SetDieReason(std::uint8_t byDieReason, int nDamage);
    // IDA: ?GetAggroLevelOrder@CMoverEx@@QEAAEXZ
    std::uint8_t GetAggroLevelOrder();
    // IDA: ?GetMoveDistAfterSkill@CMoverEx@@QEBAMXZ @ 0x1402795B0
    float GetMoveDistAfterSkill() const;
    // IDA: ?GetStandType@CMoverEx@@QEAAEXZ @ 0x140276410
    std::uint8_t GetStandType();
    // IDA: ?SetIdleMotionInfo@CMoverEx@@QEAAXHM@Z @ 0x140276960
    void SetIdleMotionInfo(int nChance, float fCheckTime);
    // IDA: ?SetBattlePose@CMoverEx@@QEAAX_N@Z @ 0x140198E50
    void SetBattlePose(bool bBattle);
    // IDA: ?CheckUseSkill@CMoverEx@@QEAAHEEPEAUTB_SKILL@@@Z @ 0x14037FBD0
    int CheckUseSkill(std::uint8_t byCheckVal, std::uint8_t byNormalVal, TB_SKILL* pTBSkill);
    // IDA: ?Reset@CMoverEx@@UEAAXXZ @ 0x14037F2A0
    virtual void Reset();
    // IDA: ?DeleteActionBuffer@CMoverEx@@UEAAXXZ @ 0x1403989A0
    virtual void DeleteActionBuffer();
    // IDA: ?ExcuteActionTrigger@CMoverEx@@UEAAXE@Z @ 0x140398B60
    virtual void ExcuteActionTrigger(std::uint8_t byCode);
    // IDA: ?SetGazeMoving@CMoverEx@@QEAAXH@Z @ 0x14038FAC0
    void SetGazeMoving(int bGaze);
    void SetGazeMoving(bool bGaze);  // overload for bool
    // IDA: ?SetOnDie@CMoverEx@@QEAAX_N@Z @ 0x140352C80
    void SetOnDie(bool bOnDie);
    // IDA: ?OnDie@CMoverEx@@UEAAXPEAVXActor@@@Z - virtual OnDie handler
    virtual void OnDie(XActor* pOwnerActor);
    // IDA: ?ClearGrapProcess@CMoverEx@@QEAAXXZ (0x1403995B0)
    void ClearGrapProcess();
    // IDA: ?GetAmountOfHeal@CMoverEx@@QEAAMXZ
    float GetAmountOfHeal();

    // Skill animation methods
    void UpdateSkillAnimInfo(TB_SKILL* pSkillTableRef);
    int GetRandomDamage(std::uint8_t bySkillAttribute, int iItemRateResult);
    bool IsExceptionalDamage();
    // IDA: ?CheckPassiveSkillByHit@CMoverEx@@UEAAXPEAVCMoverEx@@PEAUTB_SKILL@@E@Z
    // Virtual function - overridden by CMonster (0x140361B10) and CUser (0x1406F0480)
    virtual void CheckPassiveSkillByHit(CMoverEx* pMover, TB_SKILL* pSkillTable, std::uint8_t byResult);
    void NotifyPhaseChanged(std::uint8_t byOldPhase);
    void CheckOptionEffectInvoke(EFFECT_CONDITION_TYPE eConditionType, CMoverEx* pMover,
                                 float fParam, EFFECT_INVOKE_TYPE eInvokeType);
    float GetTotalOptionEffectValue(EFFECT_STATUS_TYPE eStatusType);
    float GetMultipleAbsorbSG();
    void CheckBuffDamage(CMoverEx* pTargetMover, CMoverEx* pAttacker, int nIndex, int nDamage);
    void _GenerateEventObject(int eSelfTypeA, int iSelfIDA);

    // Virtual methods for monster/player differentiation
    // IDA: ?GetMonsterFlag@CMonster@@UEAAEXZ - override in CMonster
    virtual std::uint8_t GetMonsterFlag() { return 0; }
    // IDA: ?IsBoss_Named_Raid@CMonster@@UEAAHXZ - override in CMonster
    virtual int IsBoss_Named_Raid() { return 0; }

    // Combat and control methods
    void ChangeCombatType(int nType, float fParam1, float fParam2);
    void ChangeInitMotion();
    void SetCombatType(int nType);
    int GetCombatType();
    bool IsControlMonster();
    void SetControlMonsterFlag(bool bFlag);
    void SetControlMonster(const hkvVec3& vPos, std::uint32_t dwID1, std::uint32_t dwID2,
                           float fParam1, float fParam2, char* pStr, bool bFlag);
    TB_SKILL* GetSkillTable();
    std::uint8_t GetSkillLevel();
    std::uint8_t GetSkillChargeStep();
    void ChangeBattlePose(bool bPose1, bool bPose2);
    float GetLookPitch();
    void CheckDieType(std::uint8_t& byReactionType, std::uint8_t byDamageFlag, hkvVec3& vExtraMove);
    bool IsCounterSuccessFrame();
    void SetSilhoutte(hkaiPointCloudSilhouetteGenerator* pSilhouette);

    // Movement methods
    void MoveToPosition(const hkvVec3& vTargetPos, float fSpeed, bool bRun);
    void StopMove();
    void SetMoveSpeed(float fSpeed);
    float GetMoveSpeed();
    void UpdatePosition(float fDeltaTime);
    void SetDefWalkSpeed(float fSpeed);
    float GetDefWalkSpeed();
    void SetDefRunSpeed(float fSpeed);
    float GetDefRunSpeed();

    // Condition methods
    void SetPvpCondition(int nCondition);
    int GetPvpCondition();
    void SetActionCondition(int nCondition);
    int GetActionCondition();
    void ClearActionCondition(int nCondition);
    bool IsActionCondition(int nCondition);
    bool IsPvpCondition(int nType);

    // Waypoint methods
    void SetWayPointID(int nID);
    void SetAggroLevelOrder(std::uint8_t byLevel);
    void SetUpdateRotation(bool bUpdate);
    class CWayPoint* GetWayPoint();
    // IDA: ?GetWayPointID@CMoverEx@@QEAAHXZ @ 0x140280dc0
    int GetWayPointID();

    // Amount methods
    void SetAmountOfHeal(float fAmount);

    // Animation methods
    void SetAnimationSpeed(float fSpeed);
    bool IsAnimationEnd();
    float GetAnimationTime();
    bool CheckMovingAttackAnimation();

    // Damage methods
    float GetMultipleDamageOnce();
    bool GetApplyMultipleDamageOnce();

    // Motion check methods
    bool IsJumpMotionExceptEnd(short nMotion);
    bool IsMoveDirMotion(short nMotion);
    bool IsChangeAnimByPhaseStepMotion(short nMotion);
    short GetMoveMotion();

    // Idle check method
    void CheckIdleTime();

    // Hit freeze methods
    void SetHitFreezeTime(float fTime);

    // Stiffen methods
    void UpdateStiffen(float fDeltaTime);

    // Phase motion methods
    bool CheckPhaseMotion(std::uint8_t byAttackCollision);
    void CheckPhaseMotionStep(short nMotion);

    // Animation speed methods
    float GetRestoreAnimSpeed();

    // Skill methods
    void CancelSkill();

    // Movement methods
    void MoveTo(const hkvVec3& vTargetPos, float fSpeed, bool bRun);
    void JumpTo(const hkvVec3& vTargetPos, float fHeight);
    void TeleportTo(const hkvVec3& vTargetPos);
    void MoveDirection(const hkvVec3& vDirection, float fSpeed, float fDuration);
    void SetPosition(const hkvVec3& vPos);
    void SyncPosition();

    // Mover state methods
    void SetMoverState(std::uint32_t dwStateFlags);
    std::uint32_t GetMoverState();
    void ResetMoverState();
    bool IsMoverState(std::uint32_t dwStateFlag);
    void PushMoverState(std::uint32_t dwStateFlags);

    // Extra movement methods
    void SetKeepMovingExtra(int nValue);

    // Action buffer methods
    void ClearActionBuffer();

    // Sequence methods - inherited from CMover
    // void SetCurrentSequenceTime(float fTime);

    // Position send methods
    void SendPosition(std::uint32_t dwClientID);

    // Velocity methods
    hkvVec3 GetVelocity() const;

    // Collision methods
    void OnCollision(CMover* pOther, const hkvVec3& vCollisionPoint);
    void ProcessCollision();
    void SetCollision(bool bEnable);
    bool IsColliding() const;
    void GetCollisionInfo(hkvVec3& vPoint, CMover** ppTarget);

    // Damage motion methods
    std::int16_t GetDamageMotion(std::uint8_t byReactionType, float fAttackRot,
                                  std::uint8_t byAttackCollision, std::uint8_t byCheckRank);

    // Damage methods (CMoverEx overrides)
    // Virtual function - see declaration above

    // IDA: ?PlayCounterAnim@CMoverEx@@QEAAXXZ (0x140398330)
    void PlayCounterAnim();

    // IDA: ?GetGroundDownTime@CMoverEx@@QEAAMXZ (0x140381B50)
    float GetGroundDownTime();

    // IDA: ?GetRestoreDefenseType@CMover@@QEAAEXZ (0x140276290)
    std::uint8_t GetRestoreDefenseType() const { return m_byRestoreDefenceType; }

    // IDA: ?SetRestoreDefenseType@CMover@@QEAAXXZ (0x1403A2830)
    void SetRestoreDefenseType() { m_byRestoreDefenceType = m_byDefenseType; }
    // IDA: ?GetDamageCalc@CMoverEx@@UEAAHHEMH_N@Z @ 0x140388170
    // Note: This is a separate overload, not an override (different number of parameters)
    using CMover::GetDamageCalc;
    int GetDamageCalc(int nAP, std::uint8_t byType, float fReduceRate, int iItemRateResult, bool bIgnoreInvinsible);

    // Enemy/Friend check methods (additional)
    bool IsEnemy(CMover* pMover) const;
    int IsFriend(CMover* pMover);
    int IsFriendForChain(CMover* pMover);
    int IsParty(CMover* pMover);

    // IDA: ?GetUpperMotionName@CMoverEx@@QEAA?AVVString@@PEBD@Z (0x140381750)
    VString* GetUpperMotionName(VString* pResult, const char* szMotionName);
    const char* GetUpperMotionName(const char* szMotionName);  // simplified version

    // Skip motion methods
    void ExcuteSkipMotionTrigger(unsigned int nSkillID, float fCamYaw);

    // Invisible methods
    void SetInvisible(int bHide, std::uint32_t dwFlag, int nType, int nValue,
                      int nExtVal1, int nExtVal2, int nExtVal3);

    // Movement state methods
    bool IsMoving();
    hkvVec3 GetMoveDirection();

    // Update methods
    void UpdateAttackKeyPress(int bPress);
    void UpdatePreTargetSkill();
    void UpdateTargetByPretarget();
    void SceneChanged();

    // Skill methods
    void ChargeSkillEnd();
    int GetPreTargetListCount();

    // Super armor methods
    float GetCalcChargingMultiple();
    std::uint8_t GetSABreakType();
    void SetSABreakTime(float fBreakTime);
    void SetSABreakLoopTime(float fBreakLoopTime);
    void SetSABreakType(std::uint8_t byType);
    void AddAmountOfHeal(float fHeal);

    // Attached/trap methods
    CMoverEx* GetAttached();
    hkvVec3 GetCameraDir();
    void SetMouseOnTrap(VGameTrapObject* pTrap);
    VGameTrapObject* GetMouseOnTrap();
    hkvVec3 GetTrapPos() const;
    void SetApplyParentRotation(int bApply);
    void SetApplyMultipleDamageOnce(bool bApplyMultipleDamageOnce);

    // Deck bonus methods
    TB_DECK_BONUS* GetCurDeckBouns();

    // Motion check methods
    bool IsMoveMotion(std::int16_t nMotionClass);

    // Animation processing
    void ProcessSkillAnimation(float fDeltaTime);
    void ProcessAnimationDuring();  // IDA: ?ProcessAnimationDuring@CMoverEx@@UEAAXXZ (0x140384780)

    // Direction methods
    void SetDirectionTo(const hkvVec3* vTarget);  // IDA: CMover::SetDirectionTo @ 0x14036CAD0
    void SetDirectionTo(const hkvVec3& vTarget);  // Overload

    // Jump/Flight methods
    void UpdateJumpHeight();  // IDA: ?UpdateJumpHeight@CMoverEx@@UEAAXXZ
    bool IsMoveingInFly();    // IDA: check if moving in flight
    void ProcessMoveingInFly();  // IDA: process movement in flight

    // Grap methods
    void CheckGrapDamage();  // IDA: check grappling damage

    // Skill skip methods
    bool HasSkillSkipTime(const char* szAnimName);
    bool HasSkillSkipTime(const VAnimationInfo* pActionInfo);
    bool IsCanSkillSkip(const char* szAnimName, float fTime);

    // Passive skill methods
    void CheckPassiveSkill(std::uint8_t byTargetType, std::uint8_t byCondition);

    // Position methods
    // IDA 真实 vtable ABI: ?SetPositionXVec3@CMover@@UEAAXAEAUXVec3@@@Z (0x1401893C0)
    // 真签名为 virtual (XVec3&)，CMoverEx 无 override（IDA names 无记录）。
    // 早前在此层臆造的 const hkvVec3& 声明已删除，调用统一走 CMover 基类实现。
    // TODO: 需人工审查 - CMover 层签名仍是恢复期 const hkvVec3&，
    //   完整 vtable 槽位 (XVec3&) ABI 修正留给独立 Mover 批次。

    // System actor methods
    int IsSystemActor();
    // IDA: ?SetSystemActor@CMoverEx@@QEAAXH@Z (0x1402A4A70)
    void SetSystemActor(bool bVal) { m_bSystemActor = bVal ? 1 : 0; }

    // Control type methods
    std::uint8_t GetControlType(TB_SKILL* pSkillTable);
    std::uint8_t GetCameraLock(TB_SKILL* pSkillTable);

    // Skill methods
    void ChargeSkillStart();
    bool IsCanSkill();
    void PreSkillProcess(std::uint32_t nSkillID, int bNormalAttack);
    void PostSkillProcess();

    // IDA: ?InitJumpData@CMoverEx@@QEAAXM_N@Z (0x140395BA0)
    void InitJumpData(float fTime, bool bInit);

    // Target damage - IDA signature match
    void CalcTargetDamage(CMover* pTargetMover, int nIndex, bool bAllowAbsorbSG,
                          TB_SKILL* pSkillTable, struct AttackJudgmentTrigger* pActionEvent,
                          float fChainDamageRate, bool bDontCalcByResult,
                          std::uint8_t byFixResult, bool bSummonDamageOnceBuff);

    // Absorb SG methods
    void SetAllowAbsorbSG(bool bAllow);

    // Grap target methods
    CMoverEx* GetGrapTarget();

    // Divergence methods
    void SetCurDivergenceTable(TB_DIVERGENCE* pCurDivTable, std::uint32_t dwSkillID);
    TB_DIVERGENCE* GetCurDivergenceTable();
    std::uint32_t GetCurDivergenceSKillID();

    // Akashic methods
    float GetSummonAkashicYaw();
    // IDA: ?SummonAkashic@CMoverEx@@QEAAXPEAVAkashicTrigger@@@Z
    void SummonAkashic(class AkashicTrigger* pTrigger);

    // Warp methods
    // IDA: ?WarpToPoint@CMoverEx@@QEAAXHHM@Z
    void WarpToPoint(int iPattern, int iPoint, float fYaw);

    // Trap methods
    // IDA: ?SetMoveTrapPos@CMoverEx@@UEAAXH@Z
    virtual void SetMoveTrapPos(int nUseZAxis);

    // Subordination combo methods
    // IDA: ?SetSubordinationCombo@CMoverEx@@QEAAXPEAVSubordinationComboTrigger@@@Z
    void SetSubordinationCombo(class SubordinationComboTrigger* pTrigger);
    // IDA: ?PlaySuboAnim@CMoverEx@@QEAA_NH_N@Z
    bool PlaySuboAnim(std::uint32_t iSuboAnimIdx, bool bPlayForce);

    // Entity attachment methods
    // IDA: ?SetCheckEntityAttach@CMoverEx@@UEAAX_NMMMVhkvVec3@@H@Z
    virtual void SetCheckEntityAttach(bool bCheck, float fDuration, float fDistance, float fAngle, hkvVec3* vOffset, int iApplyWeightRank);
    // IDA: ?CheckAttachedEntity@CMoverEx@@UEAAXXZ
    virtual void CheckAttachedEntity();
    // IDA: ?TraceAttachedOwner@CMoverEx@@QEAAXXZ
    void TraceAttachedOwner();
    // IDA: ?ClearAllAttachedEntity@CMoverEx@@UEAAXXZ
    virtual void ClearAllAttachedEntity();
    // IDA: ?SetAttached@CMoverEx@@QEAAXPEAV1@@Z
    void SetAttached(CMoverEx* pAttachedOwner);

    // Change mob methods
    std::uint32_t GetChangeMobTableID();

    // ============================================================================
    // Missing methods for MySkillList (IDA verification needed)
    // ============================================================================
    // IDA: ?IsEnemyForChain@CMoverEx@@QEAA_NPEAV1@@Z
    int IsEnemyForChain(CMover* pMover);
    // IDA: ?IsCanHit@CMoverEx@@QEAA_NHH@Z
    bool IsCanHit(int nTargetStatus, int nPassiveType);
    // IDA: ?GetID@CMoverEx@@QEAAKXZ - returns ActorID
    std::uint32_t GetID();
    // IDA: ?GetAllowAbsorbSG@CMoverEx@@QEAA_NXZ
    bool GetAllowAbsorbSG();
    // IDA: ?ApplySkillDamageFrame@CMoverEx@@UEAAXGGEAEAVhkvVec3@@MEME_N@Z
    virtual void ApplySkillDamageFrame(std::uint16_t nSkillID, std::int16_t nTriggerIdx,
                                        std::uint8_t byAttackTargetCnt, hkvVec3& vPos,
                                        float fAttackRot, std::uint16_t wContinousHit,
                                        std::uint8_t byDamageType, bool bPenetrate, bool bChain);
    // IDA: ?CalcTargetDamage_2@CMoverEx@@... - full version with more parameters
    void CalcTargetDamage_2(CMover* pTarget, int nIndex, bool bAllowAbsorbSG,
                            TB_SKILL* pSkillTable, struct AttackJudgmentTrigger* pActionEvent,
                            float fDamageRate, bool bUnk1, int nUnk2, bool bUnk3);

    // Buff ability methods for attacker (from MoverLinkStubs)
    void ApplyBuffAbilityForAttacker(std::uint32_t dwAttackerID);
    void ClearBuffAbilityForAttacker(std::uint32_t dwAttackerID);

    // ============================================================================
    // Buff System Overrides (CMoverEx implementations)
    // ============================================================================

    // IDA: ?ClearBuffStatusBySlot@CMoverEx@@UEAAXG_N@Z (0x14038DA80)
    // 清除指定槽位的Buff状态（CMoverEx实现）
    virtual void ClearBuffStatusBySlot(std::uint16_t nBuffSlot, bool bExcuteOutSkill) override;

    // IDA: ?SetBuffStatus@CMoverEx@@UEAAHGK_N@Z (0x14038BCE0)
    // 设置Buff状态（CMoverEx实现，核心Buff添加函数）
    virtual bool SetBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwOwnerID, bool bShowBuff) override;

    // IDA: ?IsClearBuff@CMoverEx@@UEAAHGE@Z (0x1403903D0)
    // 检查是否可清除Buff（CMoverEx实现）
    virtual bool IsClearBuff(std::uint16_t nBuffIndex, std::uint8_t byReason) override;

    // IDA: ?CheckBuffGrade@CMoverEx@@QEAAHHPEAUTB_BUFF@@@Z (0x14038C8D0)
    // 检查Buff等级是否可替换
    int CheckBuffGrade(int iIndex, TB_BUFF* pNewBuff);

    // IDA: ?SetBuffOverlap@CMoverEx@@QEAAHHPEAUTB_BUFF@@KH@Z (0x14038CA00)
    // 设置Buff重叠逻辑
    int SetBuffOverlap(int iIndex, TB_BUFF* pBuffTable, std::uint32_t dwOwnerID, int bDontRemoveBuff);

    // IDA: ?LoadBuffStatus@CMoverEx@@QEAAXHGMEK_N@Z (0x14038B9C0)
    // 加载Buff状态
    void LoadBuffStatus(std::uint16_t nBuffSlot, std::uint16_t nBuffIndex, float fTime, std::uint8_t byCount, std::uint32_t dwOwnerID, bool bShowBuff);

    // IDA: ?UpdateBuffAbility@CMoverEx@@UEAA_NAEAUtagBUFF_STATE@@H@Z (0x14038E5F0)
    // 更新Buff能力值
    virtual bool UpdateBuffAbility(tagBUFF_STATE& stBuffState, int nType);

    // IDA: ?SetBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403900C0)
    // 设置Buff能力值
    virtual void SetBuffAbility(int nIndex, float fValue);

    // IDA: ?AddBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403902A0)
    // 添加Buff能力值
    virtual void AddBuffAbility(unsigned int nIndex, float fValue);

    // IDA: ?SendUpdateBuffAbility@CMoverEx@@QEAAXXZ
    // 发送更新Buff能力值
    void SendUpdateBuffAbility();

    // IDA: ?UpdateDefenseType@CMoverEx@@QEAAXXZ
    // 更新防御类型
    void UpdateDefenseType();

    // IDA: ?AddDefenseChangeInfo@CMoverEx@@QEAAHEEKM@Z (0x14037CF80)
    // 添加防御类型变更信息
    int AddDefenseChangeInfo(std::uint8_t byType, std::uint8_t byDefenseType, std::uint32_t dwID, float fTime);

    // IDA: ?RemoveDefenseChangeInfo@CMoverEx@@QEAAHEK@Z (0x14037D100)
    // 移除防御类型变更信息
    int RemoveDefenseChangeInfo(std::uint8_t byType, std::uint32_t dwID);

    // IDA: ?ApplyDefenseChangeInfo@CMoverEx@@QEAAHXZ (0x14037D5B0)
    // 应用防御类型变更信息
    bool ApplyDefenseChangeInfo();

    // IDA: ?ClearBuffAbility@CMoverEx@@UEAAXHM@Z (0x1403901A0)
    // 清除Buff能力值
    virtual void ClearBuffAbility(int iType, float fValue);

    // IDA: ?ReduceBuffAbility@CMoverEx@@UEAAXHM@Z (0x140390330)
    // 减少Buff能力值
    virtual void ReduceBuffAbility(unsigned int iType, float fValue);

    // IDA: ?SetFreeze@CMoverEx@@UEAAXH@Z (0x140390A10)
    // 设置冻结状态
    void SetFreeze(bool bFreeze);

    // IDA: ?IsCanApplyBuff@CMoverEx@@QEAA_NGPEAUTB_BUFF@@@Z
    // 检查是否可应用Buff
    bool IsCanApplyBuff(std::uint16_t nBuffIndex, TB_BUFF* pBuffTable);

    // IDA: ?IsMaxValStat@CMoverEx@@QEAAHE@Z (0x14038C860)
    // Check if stat type uses max value
    bool IsMaxValStat(std::uint8_t byStatType);

    // IDA: ?IsCheckCurStat@CMoverEx@@QEAA_NPEAUTB_BUFF@@@Z
    // 检查当前属性
    bool IsCheckCurStat(TB_BUFF* pBuffTable);

    // IDA: ?ApplySkillMove@CMoverEx@@QEAAXMM@Z (0x140396520)
    void ApplySkillMove(float fDistance, float fTime);

    // IDA: ?SetAkashicObject@CMoverEx@@QEAAXPEAV1@@Z (0x1409A950)
    void SetAkashicObject(CMoverEx* pObject);

    // IDA: ?SetCameraDir@CMoverEx@@QEAAXAEAVhkvVec3@@@Z (0x1405F92E0)
    void SetCameraDir(const hkvVec3& vDir);

    // IDA: ?GetSkillAnimCount@CMoverEx@@QEAAEXZ (0x1405FA330)
    std::uint8_t GetSkillAnimCount() const;

    // IDA: ?GetSkillAnimStep@CMoverEx@@QEAAEXZ (0x1405FA350)
    std::uint8_t GetSkillAnimStep() const;

    // IDA: ?SetLastDamageType@CMoverEx@@QEAAXE@Z (0x14070AAC0)
    void SetLastDamageType(std::uint8_t byType);

    // IDA: ?SetAkashicRecord@CMoverEx@@QEAAXPEAUTB_AKASHIC_RECORDS@@M@Z (0x1405FA370)
    void SetAkashicRecord(TB_AKASHIC_RECORDS* pTblRef, float fYaw);

    // IDA: ?SetCurDeckBouns@CMoverEx@@QEAAXPEAUTB_DECK_BONUS@@@Z (0x1405FA520)
    void SetCurDeckBouns(TB_DECK_BONUS* pDeckBonus);

    // IDA: ?ResetPvpCondition@CMoverEx@@QEAAXXZ (0x14070A6A0)
    void ResetPvpCondition();

    // IDA: ?GetCheckAttachToAttacker@CMoverEx@@QEAA_NXZ (0x1403A27B0)
    bool GetCheckAttachToAttacker() const;

    // IDA: ?SetLinkSkillOn@CMoverEx@@QEAAX_N@Z (0x1403A27D0)
    void SetLinkSkillOn(bool bSkillOn);

    // IDA: ?GetLinkSkillDuration@CMoverEx@@QEAAMXZ (0x1403A27F0)
    float GetLinkSkillDuration() const;

    // IDA: ?GetSilhoutte@CMoverEx@@QEAAPEAVhkaiPointCloudSilhouetteGenerator@@XZ (0x140ACC30)
    hkaiPointCloudSilhouetteGenerator* GetSilhoutte();

    // IDA: ?GetAttackJudgmentEvent@CMoverEx@@QEAAPEAVAttackJudgmentTrigger@@H@Z (0x140381500)
    AttackJudgmentTrigger* GetAttackJudgmentEvent(int nEventID);

    // IDA: ?GetAttackJudgmentEvent@CMoverEx@@QEAAPEAVAttackJudgmentTrigger@@PEBDH@Z (0x1403814C0)
    AttackJudgmentTrigger* GetAttackJudgmentEvent(const char* pAnimName, int iIndex);

    // IDA: ?GetAttackJudgmentEvent@CMoverEx@@QEAAPEAVAttackJudgmentTrigger@@PEAUTB_SKILL@@EH@Z (0x140381460)
    AttackJudgmentTrigger* GetAttackJudgmentEvent(struct TB_SKILL* pSkillTableRef, std::uint8_t byStep, int iIndex);

    // IDA: ?SetupPhaseMotion@CMoverEx@@QEAAXXZ (0x14084E20)
    void SetupPhaseMotion();

    // IDA: ?ReturnFromPrefab@CMoverEx@@QEAAXXZ (0x14093E50)
    void ReturnFromPrefab();

    // IDA: ?IsCanOptionEffectExcute@CMoverEx@@QEAA_NPEAUTB_CREATEOPTION@@PEAV1@M@Z (0x1409A010)
    bool IsCanOptionEffectExcute(struct TB_CREATEOPTION* pOption, CMoverEx* pMover, float fTime);

    // IDA: ?GetMotionChangeLog@CMoverEx@@QEAA?AV?$basic_string@DU?$char_traits@D@std@@...@@Z (0x1409DB10)
    std::string GetMotionChangeLog();

    // IDA: ?GetDieType@CMoverEx@@QEAA?AW4DIE_TYPE@@XZ (0x140C6BF0)
    DIE_TYPE GetDieType();

    // IDA: ?GetChangeMobNewID@CMoverEx@@QEAAKXZ (0x14052A00)
    std::uint32_t GetChangeMobNewID();

    // IDA: ?GetHitPartsInfo@CMoverEx@@QEAAAEAUSHitPartsInfo@@H@Z (0x14052850)
    SHitPartsInfo& GetHitPartsInfo(int nIndex);

    // IDA: ?SetHitPartsInfo@CMoverEx@@QEAAXHKEHH@Z (0x140399CC0)
    // 精确还原: iIndex<2 时写 m_sHitParts[iIndex].dwTableID/byPartsID
    //   + SetHitPartsHP(iIndex, iCurHP, iMaxHP)
    void SetHitPartsInfo(int iIndex, unsigned int dwTableID, unsigned char byPartsID,
                         int iCurHP, int iMaxHP);

    // IDA: ?SetHitPartsHP@CMoverEx@@QEAAXHHH@Z
    // (SetHitPartsInfo 尾部调用; HP 写入 m_sHitParts[iIndex].iCurHP/iMaxHP)
    void SetHitPartsHP(int iIndex, int iCurHP, int iMaxHP);

    // IDA: ?GetAddMoneyFromOptionEffect@CMoverEx@@QEAAHXZ (0x140F9000)
    int GetAddMoneyFromOptionEffect();

    // IDA: ?ResetAddMoneyFromOptionEffect@CMoverEx@@QEAAXXZ (0x140F8FE0)
    void ResetAddMoneyFromOptionEffect();

    // IDA: ?GetAddExpFromOptionEffect@CMoverEx@@QEAAHXZ (0x14048270)
    int GetAddExpFromOptionEffect();

    // IDA: ?ResetAddExpFromOptionEffect@CMoverEx@@QEAAXXZ (0x14048250)
    void ResetAddExpFromOptionEffect();

    // IDA: ?GetAddEtherFromOptionEffect@CMoverEx@@QEAAHXZ (0x140F8F90)
    int GetAddEtherFromOptionEffect();

    // IDA: ?ResetAddEtherFromOptionEffect@CMoverEx@@QEAAXXZ (0x140F8F70)
    void ResetAddEtherFromOptionEffect();

    // IDA: ?SetOwnerID@CMoverEx@@QEAAXK@Z (0x1409E1C0)
    void SetOwnerID(std::uint32_t dwID);

    // IDA: ?GetOwnerID@CMoverEx@@QEAAKXZ (0x1401AD020)
    std::uint32_t GetOwnerID();

    // IDA: ?GetOwnerPlayer@CMoverEx@@QEAAPEAV1@XZ (0x140398BF0)
    CMoverEx* GetOwnerPlayer();

    // IDA: ?GetSkillLoopTime@CMoverEx@@QEAAMXZ (0x14015ED0)
    float GetSkillLoopTime();

    // IDA: ?GetShieldHP@CMoverEx@@QEAAEXZ (0x1403A2790)
    std::uint8_t GetShieldHP() const;

    // IDA: ?IsBattlePose@CMoverEx@@UEAA_NXZ (0x140189000)
    bool IsBattlePose();

    // IDA: ?GetAkashicTriggerTime@CMoverEx@@UEAAMXZ (0x140189260)
    float GetAkashicTriggerTime();

    // IDA: ?SetMovingYaw@CMoverEx@@IEAAXM@Z (0x1407E3E0)
    void SetMovingYaw(float fYaw);

    // IDA: ?GetBloodDebuffOwnerID@CMoverEx@@QEAAKXZ
    std::uint32_t GetBloodDebuffOwnerID_Ex();

    void RemoveAllOptionEffect();
    void RemoveAllDefenseChangeInfo();
    // IDA: ?ChangeDefenseTypeForce@CMoverEx@@QEAAXEM@Z (0x14037d710)
    void ChangeDefenseTypeForce(std::uint8_t byDefenseType, float fTime);
    // IDA: ?ChangeDefenseTypeForce@CMoverEx@@QEAAXE@Z (0x14037d760)
    void ChangeDefenseTypeForce(std::uint8_t byDefenseType);
    bool IsJumpMotion(std::int16_t nMotionClass);
    bool IsCanMovingAnim();
    const char* GetSkillAnimName(TB_SKILL* pSkill, std::uint8_t byStep);
    std::uint32_t GetAnimIndex(const class VString& strAnimName);
    std::int16_t AnimKeyToMotion(std::uint32_t dwKey);
    void ClearMotion();
    CMover* CheckMoveCollision(hkvVec3* pPos);
    // Inherit GetHeight from CMover which returns bool
    using CMover::GetHeight;
    void MoveingValueClear();
    void DebugOut(const char* szFormat, ...);

    // Skip motion trigger check
    bool IsExcuteSkipMotionTrigger(std::uint8_t byTriggerType);  // IDA: ?IsExcuteSkipMotionTrigger@CMoverEx@@QEAAHE@Z

    // Motion after fly
    void ChangeMotionAfterFly();  // IDA: CMoverEx::ChangeMotionAfterFly

    // Force reaction check
    bool IsApplyForceReaction(std::uint8_t byCheckRank = 0);  // IDA: CMoverEx::IsApplyForceReaction

    // Grap raise check
    bool CheckGrapRaise();  // IDA: CMoverEx::CheckGrapRaise

    // Rotation update
    void UpdateRotation(float fDeltaTime);  // IDA: CMoverEx::UpdateRotation
    bool GetApplyParentRotation();  // IDA: CMoverEx::GetApplyParentRotation
    bool CheckKeepLookTarget();  // IDA: CMoverEx::CheckKeepLookTarget - returns true if should skip rotation update
    bool IsDirectionToTargetSkill();  // IDA: CMoverEx::IsDirectionToTargetSkill
    void UpdateTargetRotation(float fDiffYaw, float fDeltaTime);  // IDA: CMoverEx::UpdateTargetRotation
    // IDA: ?SetDirectionYaw@CMoverEx@@UEAAXME@Z (0x14037F400)
    void SetDirectionYaw(float fYaw, std::uint8_t byType = 0);
    float CalcRotationBlending(float fCurrYaw, float fTargetYaw, float fDeltaVal);  // IDA: CMoverEx::CalcRotationBlending - 3 params
    // IDA: ?SetDie@CMoverEx@@UEAAXFH@Z (0x140397520)
    // Note: IDA shows signature (short nMotion, int bSuicide) - second param is unused in function body
    void SetDie(std::int16_t nMotion, int bSuicide = 0);
    void ChangeAngleAttackName(int nAngleValue, const char* szAnimName);  // IDA: CMoverEx::ChangeAngleAttackName

    // Attack input event
    class ExtraInputTrigger* GetAttackInputEvent(const char* szTriggerName);  // IDA: CMoverEx::GetAttackInputEvent

    // Akashic
    void ChangeToAkashicData(const char* szAnimName, int nGroupID);  // IDA: CMoverEx::ChangeToAkashicData

    // Skill check
    bool CheckAttackSkillEnable();  // IDA: CMoverEx::CheckAttackSkillEnable
    void CheckAttackSkillEnable(CMoverEx* pMoverEx);  // IDA: CMoverEx::CheckAttackSkillEnable (with param)
    void SetKeepMovingExtra(bool bKeep);  // IDA: CMoverEx::SetKeepMovingExtra
    void ReleaseInvokedOptionEffect(int nConditionType);  // IDA: CMoverEx::ReleaseInvokedOptionEffect
    bool CheckSkillSkipType(int nType);  // IDA: CMoverEx::CheckSkillSkipType

    // Calculate fly velocity and height
    float CalcFlyVelocity(float fDesiredHeight);  // IDA: CMoverEx::CalcFlyVelocity
    float CalcFlyHeight(float fJumpHeight, float fJumpTime);  // IDA: CMoverEx::CalcFlyHeight

protected:
    // IDA: offset 58592, size 4
    float m_fLastChangeAnimationTime;

    // IDA: offset 58596, size 1
    bool m_bBattlePose;

    // IDA: offset 58600, size 4
    float m_fMovingYaw;

    // IDA: offset 58604, size 4
    float m_fSummonAkashicYaw;

    // IDA: offset 58608, size 4
    float m_fLookPitch;

    // IDA: offset 58612, size 4
    float m_fCheckMoveStopTime;

    // IDA: offset 58616, size 4
    float m_fMoveDelayTime;

    // IDA: offset 58620, size 4
    int m_bCancelMoving;

    // IDA: offset 58624, size 4
    float m_fLastStartRotTime;

    // IDA: offset 58628, size 4
    float m_fStartRotWaitTime;

    // IDA: offset 58632, size 1
    bool m_bStartRotation;

    // IDA: offset 58633, size 1
    bool m_bQuickTurn;

    // IDA: offset 58634, size 1
    bool m_bKeepLookTarget;

    // IDA: offset 58636, size 4
    float m_fDefWalkSpeed;

    // IDA: offset 58640, size 4
    float m_fDefRunSpeed;

    // IDA: offset 58644, size 4
    float m_fDefTurnSpeed;

    // IDA: offset 58648, size 4
    float m_fBackupTurnSpeed;

    // IDA: offset 58652, size 4
    float m_fTurnSpeedRate;

    // IDA: offset 58656, size 4
    float m_fQuickTurnSpeed;

    // IDA: offset 58660, size 4
    float m_fHitFreezeTime;

    // IDA: offset 58664, size 4
    int m_iStiffenCount;

    // IDA: offset 58668, size 4
    float m_fStiffenRate;

    // IDA: offset 58672, size 4
    float m_fStiffenTime;

    // IDA: offset 58676, size 4
    float m_fStiffenDelayTime;

    // IDA: offset 58680, size 4
    float m_fStiffenImmuneTime;

    // IDA: offset 58684, size 4
    int m_bAnimSpeedTrigger;

    // IDA: offset 58688, size 4
    float m_fFlyVelocity;

    // IDA: offset 58692, size 4
    float m_fJumpDelta;

    // IDA: offset 58696, size 4
    float m_fJumpDelayTime;

    // IDA: offset 58700, size 4
    float m_fFlyingStayTime;

    // IDA: offset 58704, size 4
    float m_fJumpTime;

    // IDA: offset 58708, size 4
    float m_fJumpHeight;

    // IDA: offset 58712, size 4
    float m_fPrevJumpHeight;

    // IDA: offset 58716, size 4
    float m_fFlyDirValue;

    // IDA: offset 58720, size 4
    float m_fFlyGravity;

    // IDA: offset 58724, size 4
    float m_fFlyMaxHeight;

    // IDA: offset 58728, size 4
    int m_bJumpAnim;

    // IDA: offset 58732, size 1
    std::uint8_t m_byAniProcessLinkType;

    // IDA: offset 58733, size 1
    std::uint8_t m_byMoveDirAnim;

    // IDA: offset 58736, size 8
    TB_SKILL* m_pCurSkillTableRef;

    // IDA: offset 58744, size 4
    int m_bAttackKeyPress;

    // IDA: offset 58748, size 1
    std::uint8_t m_bySkillAnimStep;

    // IDA: offset 58749, size 1
    std::uint8_t m_bySkillAnimCount;

    // IDA: offset 58752, size 4
    float m_fSkillLoopTime;

    // IDA: offset 58756, size 4
    float m_fSkillBlendEndTime;

    // IDA: offset 58760, size 1
    std::uint8_t m_bySkillChargeStep;

    // IDA: offset 58761, size 1
    std::uint8_t m_bySkillChargeMaxStep;

    // IDA: offset 58764, size 4
    float m_fSkillChargeChangeTime;

    // IDA: offset 58768, size 4
    int m_bHasTurnMotion;

    // IDA: offset 58772, size 4
    int m_bHasBigTurn;

    // IDA: offset 58776, size 4
    int m_bSkipReplayTime;

    // IDA: offset 58780, size 4
    int m_nShieldHP;

    // IDA: offset 58784, size 8
    class VString m_strSpecialDamage;

    // IDA: offset 58792, size 1
    std::uint8_t m_byPhaseType;

    // IDA: offset 58793, size 1
    std::uint8_t m_byPhaseStep;

    // IDA: offset 58794, size 1
    std::uint8_t m_byMaxPhaseStep;

    // IDA: offset 58800, size 8
    class VString m_strPhaseChangeAnim;

    // IDA: offset 58808, size 1
    std::uint8_t m_byPhaseCondition;

    // IDA: offset 58812, size 4
    std::uint32_t m_dwPhaseConditionValue;

    // IDA: offset 58816, size 4
    float m_fPhaseDurationTime;

    // IDA: offset 58820, size 2
    std::int16_t m_nPlayPhaseMotion;

    // IDA: offset 58822, size 2
    std::int16_t m_nBuffMotion;

    // IDA: offset 58824, size 1
    std::uint8_t m_byStandType;

    // IDA: offset 58825, size 1
    std::uint8_t m_byBattleModeAnim;

    // IDA: offset 58828, size 4
    float m_fIdleCheckTime;

    // IDA: offset 58832, size 4
    int m_nIdleMotionChance;

    // IDA: offset 58836, size 12
    hkvVec3 m_vTrapPos;

    // IDA: offset 58848, size 8
    VGameTrapObject* m_pMouseOnTrap;

    // IDA: offset 58856, size 8
    hkaiPointCloudSilhouetteGenerator* m_pSilhouet;

    // IDA: offset 58864, size 12
    hkvVec3 m_vCamDir;

    // IDA: offset 58880, size 8
    TB_AKASHIC_RECORDS* m_pCurAkashicRecord;

    // IDA: offset 58888, size 8
    CMoverEx* m_pAkashicObject;

    // IDA: offset 58896, size 8
    TB_AURA* m_pAuraSkill;

    // IDA: offset 58904, size 4
    float m_fAuraCheckTime;

    // IDA: offset 58908, size 4
    float m_fAuraLifeTime;

    // IDA: offset 58912, size 1
    std::uint8_t m_byAggroLevelOrder;

    // IDA: offset 58916, size 4
    float m_fAmountOfHeal;

    // IDA: offset 58920, size 1
    std::uint8_t m_byDieReason;

    // IDA: offset 58924, size 4
    int m_nDieDamage;

    // IDA: offset 58928, size 12
    hkvVec3 m_vBackupSkillPos;

    // IDA: offset 58940, size 12
    hkvVec3 m_vBackupSkillRotate;

    // IDA: offset 58952, size 4
    int m_bMoveSkillPrefab;

    // IDA: offset 58960, size 48
    class VPList m_EventObjectList;

    // IDA: offset 59008, size 48
    class VPList m_CommonPosBoxList;

    // IDA: offset 59056, size 12
    hkvVec3 m_vBeforePos;

    // IDA: offset 59068, size 4
    std::uint32_t m_dwInvisibleFlag;

    // IDA: offset 59072, size 4
    float m_fInvisibleEndTime;

    // IDA: offset 59076, size 4
    int m_nInvisibleConditionType;

    // IDA: offset 59080, size 16
    int m_nInvisibleConditionVal[4];

    // IDA: offset 59096, size 32
    std::vector<std::uint32_t> m_vPreTargetList;

    // IDA: offset 59128, size 4
    int m_iLastWarpPoint;

    // IDA: offset 59132, size 4
    int m_bSystemActor;

    // IDA: offset 59136, size 4
    int m_bChangedStat;

    // IDA: offset 59140, size 4
    float m_fSkillTotalChargeTime;

    // IDA: offset 59144, size 1
    bool m_bExistSuboCombo;

    // IDA: offset 59148, size 4
    float m_fSuboComboWaitTime;

    // IDA: offset 59152, size 4
    float m_fSuboComboCheckTime;

    // IDA: offset 59156, size 4
    int m_iSuboComboMaxCount;

    // IDA: offset 59160, size 4
    int m_iSuboComboCheckCount;

    // IDA: offset 59164, size 4
    int m_iCurLeftSuboDescCount;

    // IDA: offset 59168, size 4
    int m_iCurRightSuboDescCount;

    // IDA: offset 59172, size 4
    int m_iMaxLeftSuboDescCount;

    // IDA: offset 59176, size 4
    int m_iMaxRightSuboDescCount;

    // IDA: offset 59184, size 8
    SubordinationComboTrigger* m_pSuboComboTrigger;

    // IDA: offset 59192, size 1
    bool m_bCheckAttachToAttacker;

    // IDA: offset 59200, size 8
    CMoverEx* m_pAttachToAttacker;

    // IDA: offset 59208, size 12
    hkvVec3 m_vAttachedDir;

    // IDA: offset 59220, size 4
    float m_fAttachedDirDist;

    // IDA: offset 59224, size 4
    float m_fAttachedDuration;

    // IDA: offset 59228, size 4
    float m_fAttachedDistance;

    // IDA: offset 59232, size 12
    hkvVec3 m_vAttachedOffset;

    // IDA: offset 59244, size 4
    float m_fAttachedAngle;

    // IDA: offset 59248, size 4
    int m_iApplyWeightRank;

    // IDA: offset 59252, size 8
    CMoverEx* m_pAttachedOwner = nullptr;

    // IDA: offset 59260, size 4
    int m_bDisableDirectionToTargetSkill;

    // IDA: offset 59256, size 4
    DIE_TYPE m_eDieType;

    // IDA: offset 59260, size 1
    bool m_bFlyDie;

    // IDA: offset 59261, size 1
    bool m_bCounterSuccessFrame;

    // IDA: offset 59262, size 1
    bool m_bEnableCounuter;

    // IDA: offset 59264, size 4
    float m_fMinCounterRange;

    // IDA: offset 59268, size 4
    float m_fMaxCounterRange;

    // IDA: offset 59272, size 4
    float m_fCounterAngle;

    // IDA: offset 59276, size 128
    char m_szCounterAnimName[128];

    // IDA: offset 59404, size 4
    int m_iCounterProbability;

    // IDA: offset 59408, size 4
    float m_fCounterDuration;

    // IDA: offset 59412, size 1
    bool m_bChargingStart;

    // IDA: offset 59416, size 4
    float m_fChargingInputAnimSpeed;

    // IDA: offset 59420, size 4
    float m_fChargingInputMaxAddMultiple;

    // IDA: offset 59424, size 4
    float m_fChargingInputDuration;

    // IDA: offset 59428, size 4
    float m_fChargingInputTime;

    // IDA: offset 59432, size 4
    float m_fChargingInputPressTime;

    // IDA: offset 59436, size 4
    float m_fChargingInputCalcMultiple;

    // IDA: offset 59440, size 1
    bool m_bControlMonster;

    // IDA: offset 59444, size 4
    float m_fLeftChargingValue;

    // IDA: offset 59448, size 4
    float m_fRightChargingValue;

    // IDA: offset 59452, size 1
    std::uint8_t m_byLastDamageType;

    // IDA: offset 59453, size 1
    bool m_bOnDie;

    // IDA: offset 59454, size 1
    bool m_bDieAttack;

    // IDA: offset 59456, size 56
    CWayPoint m_xWayPoint;

    // IDA: offset 59512, size 8
    CSector* m_pSector;

    // IDA: offset 59520, size 4
    float m_fSkillSkipCoolTime;

    // IDA: offset 59524, size 4
    float m_fUpdateStatBatchTime;

    // IDA: offset 59528, size 8
    TB_DIVERGENCE* m_pCurDivergenceTableRef;

    // IDA: offset 59536, size 4
    std::uint32_t m_dwDivergenceSkillID;

    // IDA: offset 59540, size 1
    std::uint8_t m_byGrapStep;
    void SetGrapStep(std::uint8_t byStep) { m_byGrapStep = byStep; }

    // IDA: offset 59544, size 8
    class VString m_szAttachBoneName;

    // IDA: offset 59552, size 8
    CMoverEx* m_pGrapParent;
    void SetGrapTarget(CMoverEx* pTarget) { m_pGrapTarget = pTarget; }

    // IDA: offset 59560, size 8
    CMoverEx* m_pGrapTarget;

    // IDA: offset 59568, size 4
    float m_fDropSpeedX;

    // IDA: offset 59572, size 4
    float m_fDropSpeedY;

    // IDA: offset 59576, size 4
    float m_fDropSpeedZ;

    // IDA: offset 59580, size 12
    hkvVec3 m_vGrapDropPos;

    // IDA: offset 59592, size 8
    tagMOVE_POS m_stDropOffset;

    // IDA: offset 59600, size 8
    DetachTrigger* m_pDetachTrigger;

    // IDA: offset 59608, size 4
    float m_fAutoRotaionSpeed;

    // IDA: offset 59612, size 4
    float m_fAutoRotaionTime;

    // IDA: offset 59616, size 4
    std::uint32_t m_dwOwnerID;

    // IDA: offset 59620, size 1
    bool m_bAllowAbsorbSG;

    // IDA: offset 59624, size 4
    int m_nAccumulateDamage;

    // IDA: offset 59632, size 8
    TB_DECK_BONUS* m_pCurDeckBonusRef;

    // IDA: offset 59640, size 512
    char m_szTempSkillAnimName[512];

    // IDA: offset 60152, size 26 (2 * 13)
    SHitPartsInfo m_sHitParts[2];

    // IDA: offset 60178, size 1
    bool m_bReserveChange;

    // IDA: offset 60180, size 4
    std::uint32_t m_dwChangeMobNewID;

    // IDA: offset 60184, size 4
    std::uint32_t m_dwChangeMobTableID;

    // IDA: offset 60188, size 4
    float m_fMoveDistAfterSkill;

    // IDA: offset 60192, size 4
    E_TEST_DAMAGE m_eTestDamageType;

    // IDA: offset 60196, size 1
    std::uint8_t m_byFixedMaxDamage;

    // IDA: offset 60197, size 1
    bool m_bUpdateRotation;

    // IDA: offset 60198, size 1
    std::uint8_t m_byPhaseMotionStep;

    // IDA: offset 60200, size 4
    float m_fPhaseStepMaxTime;

    // IDA: offset 60204, size 4
    float m_fMultipleDamageOnce;

    // IDA: offset 60208, size 1
    bool m_bApplyMultipleDamageOnce;

    // IDA: offset 60212, size 4
    float m_fMultipleAbsorbSG;

    // IDA: offset 60216, size 1
    std::uint8_t m_bySABreakMotionType;

    // IDA: offset 60220, size 4
    float m_fSABreakTime;

    // IDA: offset 60224, size 4
    float m_fSABreakLoopTime;

    // IDA: offset 60228, size 1
    bool m_bSABreakLoopMotion;

    // IDA: offset 60229, size 1
    bool m_bShowSABreakMotion;

    // IDA: offset 60232, size 4
    float m_fSABreakLoopMotionTime;

    // IDA: offset 60236, size 4
    int m_nAddMoneyFromOptionEffect;

    // IDA: offset 60240, size 4
    int m_nAddEtherFromOptionEffect;

    // IDA: offset 60244, size 4
    int m_nAddExpFromOptionEffect;

    // IDA: offset 60248, size 4
    int m_nCombatType;

    // IDA: offset 60252, size 1
    bool m_bLinkSkillOn;

    // IDA: offset 60256, size 4
    float m_fLinkSkillDuration;

    // IDA: offset 60264, size 8
    LinkSkillTrigger* m_pLinkSkillTrigger;

    // IDA: offset 60272, size 1
    bool m_bCheckAttackSkillOn;

    // IDA: offset 60276, size 4
    float m_fCheckAttackSkillDuration;

    // IDA: offset 60280, size 8
    CheckAttackSkillTrigger* m_pCheckAttackSkillTrigger;

    // IDA: offset 60288, size 32
    std::vector<SOptionEffect*> m_vecOptionEffect;

    // IDA: offset 60320, size 32
    std::vector<SDelayBuff> m_vecDelayBuff;

    // IDA: offset 60352, size 1
    bool m_bSummonMonsterApplyRot;

    // IDA: offset 60354, size 2
    std::int16_t m_nNowChangingMotionClass;

    // IDA: offset 60356, size 4
    int m_iChangeMotionPos;

    // IDA: offset 60360, size 4
    int m_iChangeMotionType;

    // IDA: offset 60364, size 16
    struct _SYSTEMTIME m_iChangeMotionTime;

    // IDA: offset 60380, size 4
    int m_iActionCondition;

    // IDA: offset 60384, size 4
    int m_iPvpCondition;
};

// TODO: 需人工审查 - CMoverEx size check disabled until class is complete
// static_assert(sizeof(CMoverEx) >= 60392, "CMoverEx size check - at least 60392 bytes expected");

// ============================================================================
// GOComponent::Register<T> template implementation
// IDA pattern: ??$Register@V{ComponentType}@@@GOComponent@@SAXPEAVCMover@@V?$shared_ptr@V{ComponentType}@@@tr1@std@@@Z
// Note: Must be after CMover definition
// ============================================================================
template<typename T>
void GOComponent::Register(CMover* pOwner, std::shared_ptr<T> pComponent) {
    if (!pOwner || !pComponent) return;
    
    // Call CMover::SetGOC to register the component
    pOwner->SetGOC<T>(pComponent);
    
    // Initialize the component (IDA shows Init call)
    pComponent->Init(pOwner);
}

// ============================================================================
// GOComponent::CreateAndRegister<T> template implementation
// IDA pattern: ??$CreateAndRegister@V{ComponentType}@@@GOComponent@@SA?AV?$shared_ptr@V{ComponentType}@@@tr1@std@@PEAVCMover@@@Z
// Creates a new component instance and registers it with the owner
// ============================================================================
template<typename T>
std::shared_ptr<T> GOComponent::CreateAndRegister(CMover* pOwner) {
    if (!pOwner) return std::shared_ptr<T>();
    
    // Create new component instance
    auto pComponent = std::make_shared<T>();
    if (!pComponent) return std::shared_ptr<T>();
    
    // Register with owner
    Register<T>(pOwner, pComponent);
    
    return pComponent;
}
