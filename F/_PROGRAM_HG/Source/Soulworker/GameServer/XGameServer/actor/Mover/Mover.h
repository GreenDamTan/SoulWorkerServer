#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <memory>
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"  // for hkvVec3, hkvMat3, SDefenseChangeInfo, VString, CActionBuffer
#include "Soulworker/GameServer/XGameServer/BuffState.h"  // for tagBUFF_STATE, tagMOVE_POS, tagEXTRA_MOVEPOS, tagTIME_SLOW
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"  // for UXActorID, XVec3

// Forward declarations
class VisBaseEntity_cl;
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
class CheckAttackSkillTrigger;
struct TB_MONSTER;
struct TB_NPC;
struct TB_CHARACTER_INFO;
struct tagHIT_COLLISION_DATA;
struct ST_MONSTER_DAMAGE_INFO;
struct SDelayedProjectile;
struct SContinuousMelee;
struct SFilterData;
struct SHitPartsInfo;
struct SDelayBuff;
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
};

/**
 * @brief ANIM_SPEED_TYPE - Animation speed type enumeration
 */
enum ANIM_SPEED_TYPE : std::int32_t {
    ANIM_SPEED_TYPE_NORMAL = 0,
    ANIM_SPEED_TYPE_SLOW = 1,
    ANIM_SPEED_TYPE_FAST = 2,
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
    // IDA: ?ActionProcess@CMover@@UEAAHF@Z @ 0x1401892D0 - returns true always
    virtual bool ActionProcess(std::int16_t nAction);
    // IDA: ?Damage@CMover@@UEAAXKEEEHMAEAVhkvVec3@@@Z @ 0x140189300 - empty stub
    virtual void Damage(std::uint32_t dwAttackerID, std::uint8_t byReactionType, std::uint8_t byAttackCollision);
    // IDA: ?DamageProcessHP@CMover@@UEAAHKHHEE@Z @ 0x1401892E0 - returns true always
    virtual bool DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage);
    // IDA: ?SetDie@CMover@@UEAAXFH_N@Z @ 0x140188FE0 - empty stub
    virtual void SetDie(std::int16_t nMotionClass, int bSuicide, bool bSendPacket);
    // IDA: ?SetHP@CMover@@UEAAXH@Z @ 0x140189230 - empty stub
    virtual void SetHP(int bFreeze);
    // IDA: ?GetStat@CMover@@QEAAMH@Z @ 0x140166360 - returns m_fAbility[iIndex]
    virtual float GetStat(int iIndex);
    virtual void CalcTargetDamage(CMover* pAttacker, int nDamage, TB_SKILL* pSkill, float fRate, float fDamage);
    void AddActionBuffer(tagACTION_BUFFER& stBuffer);

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
    void ClearExtraMoving();

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

    // IDA: ?GetLevelForStat@CMover@@UEAAEXZ @ 0x140366D30
    virtual std::uint8_t GetLevelForStat();

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
    // IDA: ?FindBuffStatus@CMover@@QEAAHGK@Z @ 0x14036A420
    int FindBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwAttackerID) const;
    // IDA: ?FindBuffByGroupID@CMover@@QEAAHGK@Z @ 0x14036A4C0
    int FindBuffByGroupID(std::uint16_t nGroupID, std::uint32_t dwAttackerID) const;
    // IDA: ?FindBuffByEffectType@CMover@@QEAAHEG@Z @ 0x14036A560
    int FindBuffByEffectType(std::uint8_t byBuffEffect, std::uint16_t nExceptBuffIndex) const;
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
    // IDA: ?CanUseItem@CMover@@QEAAHKAEAK@Z @ 0x14036B8D0
    bool CanUseItem(std::uint32_t dwID, std::uint32_t& dwError) const;
    // IDA: ?ProcessExtraMoving@CMover@@UEAAXXZ @ 0x14036BC20
    virtual void ProcessExtraMoving();
    // IDA: ?ReleaseExtraMoving@CMover@@UEAAXXZ @ 0x14036C120
    virtual void ReleaseExtraMoving();
    // IDA: ?ChangeSequence@CMover@@QEAAXKKH@Z @ 0x14036C500
    void ChangeSequence(std::uint32_t dwOldAnimID, std::uint32_t dwNewAnimID, int bResetPlay);
    // IDA: ?CreateAkashicActionInfo@CMover@@QEAAXPEBD@Z @ 0x14036C800
    void CreateAkashicActionInfo(const char* szAnimName);
    // IDA: ?GetActionDesc@CMover@@QEAAPEBVVAnimationInfo@@PEBD@Z @ 0x14036C920
    const VAnimationInfo* GetActionDesc(const char* szAnimName) const;
    // IDA: ?ChangeActionTrigger@CMover@@QEAAXPEBD@Z @ 0x14036CA80
    void ChangeActionTrigger(const char* szAnimName);
    // IDA: ?SetMovePosition@CMover@@QEAAXMM@Z @ 0x14036CC00
    void SetMovePosition(float fXpos, float fYpos);
    // IDA: ?MoveingClientStop@CMover@@QEAAXXZ @ 0x14036CD40
    void MoveingClientStop();
    // IDA: ?IsEnemy@CMover@@UEAAHPEAV1@@Z @ 0x14036CD80
    virtual bool IsEnemy(CMover* pMover) const;
    // IDA: ?CheckReactionTarget@CMover@@UEAAHHPEAV1@_N@Z @ 0x14036CE70
    virtual bool CheckReactionTarget(int iTargetType, CMover* pTargetMover, bool bChekcForChain = false);
    // IDA: ?GetHeight@CMover@@QEAA_NAEAVhkvVec3@@M@Z @ 0x14036D130
    bool GetHeight(hkvVec3* vPos, float fTestHeight);
    // IDA: ?GetMoverObject@CMover@@QEAAPEAV1@K@Z @ 0x14036D200
    CMover* GetMoverObject(std::uint32_t dwID);
    // IDA: ?SetFlyState@CMover@@QEAAX_N@Z @ 0x14036D300
    void SetFlyState(bool bFly);
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
    // IDA: ?Move@CMover@@UEAAGAEAUXVec3@@M@Z @ 0x14036DE00
    virtual std::uint16_t Move(hkvVec3& vNextPos, float fRot = 0.0f);
    // IDA: ?CheckMoveDestPos@CMover@@UEAA_NAEAVhkvVec3@@HH@Z @ 0x14036DF00
    virtual bool CheckMoveDestPos(hkvVec3* vDestPos, int bFlying, int bDontCareCurve);
    // IDA: ?GetSGAbsorbRate@CMover@@QEAAMXZ @ 0x14036E200
    float GetSGAbsorbRate();
    // IDA: ?SetStat@CMover@@QEAAXHM@Z @ 0x14036E300
    void SetStat(std::uint32_t iIndex, float fVal);
    // IDA: ?CreateRandomTrapIndex@CMover@@QEAAHXZ @ 0x14036E400
    int CreateRandomTrapIndex();
    // IDA: ?SendUpdateStat@CMover@@QEAAXH@Z @ 0x14036E500
    void SendUpdateStat(std::uint32_t iIndex);
    // IDA: ?DeleteDelayedProjectile@CMover@@QEAAXPEAUSDelayedProjectile@@@Z @ 0x14036E600
    void DeleteDelayedProjectile(SDelayedProjectile* pDelayedProjectile);
    // IDA: ?CheckDelayedProjectile@CMover@@QEAAXM@Z @ 0x14036E700
    void CheckDelayedProjectile(float fDeltaTime);
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
    // IDA: ?send_eSUB_CMD_MOVE_GRAP@CMover@@QEAAXPEAV1@VhkvVec3@@@Z @ 0x1403706E0
    void send_eSUB_CMD_MOVE_GRAP(CMover* pMover, hkvVec3 vPos);
    // IDA: ?send_eSUB_CMD_MOVE_ATTACED_BT@CMover@@QEAAXPEAV1@0VhkvVec3@@M@Z @ 0x140370800
    void send_eSUB_CMD_MOVE_ATTACED_BT(CMover* pAttackerMover, CMover* pTargetMover,
                                        hkvVec3 vAttachDir, float fAttachedDirDist);
    // IDA: ?send_eSUB_CMD_SKILL_MOVING_TARGET@CMover@@QEAAXPEAV1@AEAV?$vector@UPS_MOVING_TARGET@@V?$allocator@UPS_MOVING_TARGET@@@std@@@std@@@Z @ 0x140373890
    void send_eSUB_CMD_SKILL_MOVING_TARGET(std::vector<PS_MOVING_TARGET>& vecMovingTargetList);
    
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

    void SendUpdateStat(int nStatType);

    // IDA: ?SetHitCylinder@CMover@@QEAAXMM@Z (0x14015BF0)
    void SetHitCylinder(float fRadius, float fHeight);

    // IDA: ?SetAnimInfoKey@CMover@@QEAAXPEAV?$map@VVString@@K...@@Z (0x140154D0)
    void SetAnimInfoKey(std::map<class VString, std::uint32_t>* pMap);

    // IDA: ?SetAnimInfoString@CMover@@QEAAXPEAV?$map@KVString@@...@@Z (0x140154B0)
    void SetAnimInfoString(std::map<std::uint32_t, class VString>* pMap);

    // IDA: ?SetHitCollisionData@CMover@@QEAAXPEAUtagHIT_COLLISION_DATA@@@Z (0x14015BD0)
    void SetHitCollisionData(tagHIT_COLLISION_DATA* pData);

    // IDA: ?GetHitCollisionCount@CMover@@QEAAHXZ (0x14066B90)
    int GetHitCollisionCount();

    // IDA: ?GetTableIDString@CMover@@QEAAPEBDXZ (0x1406CE70)
    const char* GetTableIDString();

    // IDA: ?SetNoSkillCostSG@CMover@@QEAAX_N@Z (0x140478E0)
    void SetNoSkillCostSG(bool bNoCost);

    // IDA: ?IsAllowPassiveType@CMover@@QEAAHH@Z (0x14063670)
    bool IsAllowPassiveType(int nType);

    // IDA: ?SetSimpleDefenseType@CMover@@QEAAXE@Z (0x14053350)
    void SetSimpleDefenseType(std::uint8_t byType);

    // IDA: ?IsKnockDown@CMover@@QEAAHXZ (0x140661C0)
    int IsKnockDown_int();

    // IDA: ?IsImmunityStatus@CMover@@QEAAHXZ @ 0x140364700
    bool IsImmunityStatus() const;

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
    void SetIgnoreAggroDebuff(bool bApply);

    // IDA: ?MoveingValueClear@CMover@@QEAAXXZ @ 0x1402A4BE0
    void MoveingValueClear();

    // IDA: ?GetBuffStatus@CMover@@QEAAPEAUtagBUFF_STATE@@XZ (0x140529C0)
    tagBUFF_STATE* GetBuffStatus();

    // ============================================================================
    // Buff System Functions
    // ============================================================================

    // IDA: ?AllBuffClear@CMover@@QEAAXE@Z (0x14036AA40)
    // 清除所有Buff，参考SetDie函数第1292行
    void AllBuffClear(std::uint8_t byReason);

    // IDA: ?FindBuffByEffectType@CMover@@QEAAHEG@Z (0x14036A560)
    // 按效果类型查找Buff，用于伤害计算
    int FindBuffByEffectType(std::uint8_t byBuffEffect, std::uint16_t nExceptBuffIndex);

    // IDA: ?ClearBuffStatusBySlot@CMover@@UEAAXG_N@Z (0x140377550)
    // 清除指定槽位的Buff状态，用于伤害计算
    virtual void ClearBuffStatusBySlot(std::uint16_t nBuffSlot, bool bExcuteOutSkill);

    // IDA: ?ClearBuffStatus@CMover@@UEAAXG_NK@Z (0x140374FC0)
    // 清除指定Buff状态
    virtual void ClearBuffStatus(std::uint16_t nBuffIndex, bool bExcuteOutSkill, std::uint32_t dwOwnerID);

    // IDA: ?SetBuffStatus@CMover@@UEAAHGK_N@Z (0x140374FE0)
    // 设置Buff状态（核心Buff添加函数）
    virtual bool SetBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwOwnerID, bool bShowBuff);

    // IDA: ?IsClearBuff@CMover@@UEAAHGE@Z (0x1403774F0)
    // 检查是否可清除Buff
    virtual bool IsClearBuff(std::uint16_t nBuffIndex, std::uint8_t byReason);

    // IDA: ?FindBuffByGroupID@CMover@@QEAAHGK@Z (0x14036A4C0)
    // 按组ID查找Buff
    int FindBuffByGroupID(std::uint16_t nGroupID, std::uint32_t dwOwnerID);

    // IDA: ?GetEmptyBuffSlot@CMover@@QEAAHXZ (0x14036A810)
    // 获取空Buff槽位
    int GetEmptyBuffSlot();

    // IDA: ?UpdateBuffCount@CMover@@QEAAXEH@Z
    // 更新Buff计数
    void UpdateBuffCount(std::uint8_t byBuffType, int nDelta);

    // IDA: ?GetBloodDebuffOwnerID@CMover@@QEAAKXZ (0x140A13B0)
    std::uint32_t GetBloodDebuffOwnerID();

    // IDA: ?DeleteDelayedProjectile@CMover@@QEAAXPEAUSDelayedProjectile@@@Z (0x1406D550)
    void DeleteDelayedProjectile(SDelayedProjectile* pProjectile);

    // IDA: ?CheckContinuousMelee@CMover@@QEAAXM@Z (0x14072EE0)
    void CheckContinuousMelee(float fTime);

    // IDA: ?ClearTargetPosFlag@CMover@@QEAAXE@Z (0x1406CA80)
    void ClearTargetPosFlag(std::uint8_t byFlag);

    // IDA: ?FindTargetPos@CMover@@QEAAEPEAV1@@Z (0x1406C380)
    std::uint8_t FindTargetPos(CMover* pTarget);

    // IDA: ?GetHitList@CMover@@QEAAXAEAV?$list@UST_MONSTER_DAMAGE_INFO@@...@@Z (0x14073220)
    void GetHitList(std::list<ST_MONSTER_DAMAGE_INFO>& listHit);

    // IDA: ?IsAttackHeight@CMover@@QEAAEQEAUtagATTACK_AREA@@AEAVhkvVec3@@AEAH@Z (0x14067CE0)
    std::uint8_t IsAttackHeight(const struct tagATTACK_AREA& stArea, hkvVec3& vPos, int& nResult);

    // IDA: ?GetItemRateResultWeapon@CMover@@QEAAHEV?$shared_ptr@VCGocAttribute@@@tr1@std@@_N@Z (0x140665F0)
    std::uint8_t GetItemRateResultWeapon(std::uint8_t byType, std::shared_ptr<class CGocAttribute> pAttr, bool bCheck);

protected:
    // IDA: offset 976, size 4
    float m_fLastUpdateTime;

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
};

static_assert(sizeof(CMover) >= 58592, "CMover size check - at least 58592 bytes expected");

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
    virtual void InitFunction() override;
    virtual void Destroy() override;
    virtual void MessageFunction(int nMsg, __int64 wParam, __int64 lParam) override;
    virtual std::int16_t GetNextMotion() override;
    virtual bool MoveTick() override;
    virtual bool StartMoving() override;
    virtual int GetSectorID() override;
    virtual int GetMaxHP() override;
    virtual void SetSkillTable(TB_SKILL* pSkill) override;
    virtual void SetDie(int nMotionClass, std::int8_t cDieReason) override;
    virtual void Damage(tagACTION_DAMAGE& stDamage, int nType, bool& bResult) override;
    virtual bool IsCanAttack() override;
    virtual void ReapllyBuffAll() override;
    virtual void ChargeSkillNextStep() override;

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
    virtual void AddBuffAbility(int nIndex, float fValue);

    // IDA: ?SendUpdateBuffAbility@CMoverEx@@QEAAXXZ
    // 发送更新Buff能力值
    void SendUpdateBuffAbility();

    // IDA: ?UpdateDefenseType@CMoverEx@@QEAAXXZ
    // 更新防御类型
    void UpdateDefenseType();

    // IDA: ?IsCanApplyBuff@CMoverEx@@QEAA_NGPEAUTB_BUFF@@@Z
    // 检查是否可应用Buff
    bool IsCanApplyBuff(std::uint16_t nBuffIndex, TB_BUFF* pBuffTable);

    // IDA: ?IsCheckCurStat@CMoverEx@@QEAA_NPEAUTB_BUFF@@@Z
    // 检查当前属性
    bool IsCheckCurStat(TB_BUFF* pBuffTable);

    // IDA: ?ApplySkillMove@CMoverEx@@QEAAXMM@Z (0x140396520)
    void ApplySkillMove(float fDistance, float fTime);

    // IDA: ?SetAkashicObject@CMoverEx@@QEAAXPEAV1@@Z (0x1409A950)
    void SetAkashicObject(CMoverEx* pObject);

    // IDA: ?SetCameraDir@CMoverEx@@QEAAXAEAVhkvVec3@@@Z (0x1405F92E0)
    void SetCameraDir(hkvVec3& vDir);

    // IDA: ?GetSilhoutte@CMoverEx@@QEAAPEAVhkaiPointCloudSilhouetteGenerator@@XZ (0x140ACC30)
    hkaiPointCloudSilhouetteGenerator* GetSilhoutte();

    // IDA: ?GetAttackJudgmentEvent@CMoverEx@@QEAAPEAVAttackJudgmentTrigger@@H@Z (0x14080500)
    AttackJudgmentTrigger* GetAttackJudgmentEvent(int nIndex);

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
    int GetShieldHP();

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
    bool IsJumpMotion(std::int16_t nMotionClass);
    bool IsCanMovingAnim();
    int GetControlType(TB_SKILL* pSkill);
    const char* GetSkillAnimName(TB_SKILL* pSkill, std::uint8_t byStep);
    std::uint32_t GetAnimIndex(const class VString& strAnimName);
    std::int16_t AnimKeyToMotion(std::uint32_t dwKey);
    void ClearMotion();
    void StopMoving(bool bSend);
    CMover* CheckMoveCollision(hkvVec3* pPos);
    void GetHeight(hkvVec3* pPos, float fMaxHeight);
    void MoveingValueClear();
    void DebugOut(const char* szFormat, ...);

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

    // IDA: offset 59252, size 4
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

    // IDA: offset 59544, size 8
    class VString m_szAttachBoneName;

    // IDA: offset 59552, size 8
    CMoverEx* m_pGrapParent;

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

static_assert(sizeof(CMoverEx) >= 60392, "CMoverEx size check - at least 60392 bytes expected");

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
