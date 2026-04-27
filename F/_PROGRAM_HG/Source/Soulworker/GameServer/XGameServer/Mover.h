#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>

// 前置声明 - Vision Engine 类型
class VisBaseEntity_cl;
class XActor;
struct VAnimationInfo;
struct VActionResourceLump;
struct VString;
struct hkvVec3;
struct tagBUFF_STATE;
struct tagMOVE_POS;
struct tagEXTRA_MOVEPOS;
struct tagTIME_SLOW;
struct SFilterData;
struct SDefenseChangeInfo;
struct SDelayedProjectile;
class CMySkillList;
class CActionBuffer;
class VPublicTransport_cl;

// TODO: 推测结果 - 来自 IDA struct CMover (58592 bytes)
// CMover 继承自 VisBaseEntity_cl (872 bytes) + XActor (104 bytes)
// 是 Vision Engine 的核心实体类
class CMover {
public:
    CMover();
    virtual ~CMover();

    // 基础方法
    virtual void OnUpdate(float fDelta);
    virtual void OnDamage(int nDamage, CMover* pAttacker);
    virtual void OnDie();

    // 位置/移动
    hkvVec3 GetPosition() const;
    void SetPosition(const hkvVec3& vPos);
    float GetMoveSpeed() const { return m_fMoveSpeed; }
    void SetMoveSpeed(float fSpeed) { m_fMoveSpeed = fSpeed; }

    // 目标
    std::uint32_t GetTargetID() const { return m_dwTargetID; }
    void SetTargetID(std::uint32_t dwID) { m_dwTargetID = dwID; }

protected:
    // === IDA 确认的成员变量 ===
    // offset 0-871: VisBaseEntity_cl 基类
    // offset 872-975: XActor 基类

    // offset 976: m_fLastUpdateTime (float)
    float m_fLastUpdateTime;

    // offset 980: m_fLastDebugTime (float)
    float m_fLastDebugTime;

    // offset 984: m_fAbility (float*)
    float* m_fAbility;

    // offset 992-996: Target/Hit IDs
    std::uint32_t m_dwTargetID;
    std::uint32_t m_dwHitID;

    // offset 1000: m_setHitID (std::map<ulong, ST_MONSTER_DAMAGE_INFO>, 32 bytes)
    std::map<std::uint32_t, void*> m_setHitID;  // TODO: 需人工审查

    // offset 1032: m_listSummonMob (std::list<ulong>, 24 bytes)
    std::list<std::uint32_t> m_listSummonMob;

    // offset 1056-1068: Capsule 尺寸
    float m_fCapsuleRadius;
    float m_fCapsuleHeight;

    // offset 1072-1080: Die 延时
    float m_fDieDelayTime;
    float m_fDieDelayMaxTime;
    float m_fDieFadeTime;

    // offset 1080-1084: Weight/Damage
    int m_nDamage;
    std::uint8_t m_cWeightRank;
    std::uint8_t m_byDmgMontionFlag;

    // offset 1084-1092: Moving 状态
    int m_bMoving;
    int m_bGazeMoving;
    float m_fMoveSpeed;
    std::uint8_t m_byMoveDir;

    // offset 1100: m_vMoveStopCheckPos (hkvVec3, 12 bytes)
    hkvVec3* m_vMoveStopCheckPos_dummy;  // TODO: 需人工审查 - 实际是 hkvVec3

    // offset 1112-1124: Buff 计数
    int m_nBuffTotalCnt;
    int m_nBuffCnt;
    int m_nDebuffCnt;

    // offset 1124: m_stBuffState (tagBUFF_STATE[50], 3500 bytes)
    tagBUFF_STATE* m_stBuffState;  // 实际是数组 [50]

    // offset 4624-4760: 多个 Buff/Move 相关字段
    int m_nHitCallBuffIndex;
    float m_fLastSendMoveTime;
    tagMOVE_POS* m_stMovePos_dummy;      // 8 bytes
    tagMOVE_POS* m_stMoveGap_dummy;      // 8 bytes
    tagMOVE_POS* m_stMoveOffset_dummy;   // 8 bytes
    tagEXTRA_MOVEPOS* m_stExtMovingVal_dummy;  // 17 bytes
    tagTIME_SLOW* m_stTimeSlow_dummy;    // 8 bytes

    // offset 4684-4708: 位置
    hkvVec3* m_vPrevPos_dummy;
    hkvVec3* m_vCreatePos_dummy;

    // offset 4708-4780: 动画相关
    std::uint8_t m_byDefaultAnimStep;
    float m_fAnimSpeed;
    float m_fRestoreAnimSpeed;
    int m_eRestoreAnimSpeedType;  // ANIM_SPEED_TYPE enum
    std::int16_t m_nMotionClass;
    std::int16_t m_nHitStatus;
    std::int16_t m_nHitAnimCount;
    std::int16_t m_nTurnStatus;
    int m_bAnimChanged;
    int m_bSkipAnimOffset;
    int m_bAnimPlay;
    std::uint32_t m_nAnimationIdx;
    std::int16_t m_nAnimGroup;

    // offset 4752-4776: 动画时间
    float m_fAnimationTime;
    float m_fAnimPercentTime;
    hkvVec3* m_vAnimLastDelta_dummy;

    // offset 4776-4800: 动画资源
    VAnimationInfo* m_pCurMotionEvent;
    VActionResourceLump* m_pActionResource;
    VAnimationInfo* m_pAkashicActionInfo;

    // offset 4800-4812: 技能表索引
    int m_nCurSkillTableIdx;
    int m_nParentSkillTableIdx;

    // offset 4808-4836: 反应/飞行
    float m_fReactionRate;
    float m_fFlySpeed;
    float m_fFlyDistance;
    float m_fFlyMoveDist;
    float m_fFlyYaw;

    // offset 4828-4848: 地面/碰撞
    int m_bLanded;
    int m_bOnGround;
    float m_fGroundPosZ;
    int m_bRestoreCollision;
    int m_bKeepMovingExtra;
    bool m_bMoveingInFly;
    float m_fGroundDownTime;
    float m_fHitLoopMaxTime;
    std::uint8_t m_byDownContinueDamage;
    std::uint8_t m_byDefenseType;

    // offset 4864: m_setAllowPassiveType (std::set<int>, 32 bytes)
    std::set<int> m_setAllowPassiveType;

    // offset 4896-4958: Defense Change 相关
    std::uint8_t m_byRestoreDefenceType;
    SDefenseChangeInfo m_stDefenseChangeInfoByTrigger;
    SDefenseChangeInfo m_stDefenseChangeInfoByEffect;
    std::list<SDefenseChangeInfo*> m_listDefenseChangeInfo;
    std::uint32_t m_DefanseChangeFlag;
    std::uint8_t m_byDefaultDefenseType;
    std::uint8_t m_byRestoreDefenceTypeByTrigger;
    std::uint8_t m_byDefenseTypeDisableFlag;

    // offset 4960-5024: Skill Rate 相关
    float m_fSkillCoolDownRate;
    float m_fSkillReflectRate;
    float m_fSkillBloodRate;
    int m_nAllowBloodCount;
    float m_fSkillBloodDebuffRate;
    std::uint32_t m_dwBloodDebuffOwnerID;
    float m_fSkillAbsorbSGRate;
    int m_nAllowAbsorbSGCount;
    bool m_bCheckSendAbsorbSG;
    bool m_bCheckSendHP;
    float m_fBuffSuperArmorRate;
    int m_bIgnoreSkillCost;
    float m_fDecreaseStaminaRate;
    float m_fBuffAddGoldRate;

    // offset 5012-5032: Aggro/Collision
    std::uint32_t m_dwAggroTargetID;
    int m_bCollisionEnable;
    float m_fCheckCollisionTime;

    // offset 5024-5080: Shader/Invincible
    int m_mShaderState;
    int m_bInvincibleActor;
    std::map<VString, std::uint32_t>* m_mapAnimInfoKey;
    std::map<std::uint32_t, VString>* m_mapAnimInfoString;
    std::map<std::uint32_t, SFilterData> m_mapFilterData;

    // offset 5080-58000: Hit Collision / Action Buffer
    void* m_pHitCollisionData;  // tagHIT_COLLISION_DATA*
    CActionBuffer m_xActionBuffer;  // 大型结构 (52912 bytes!)

    // offset 58000-58056: Cylinder 尺寸
    float m_fHitCylinderRadius;
    float m_fHitCylinderHeight;
    std::vector<VString> m_vTraceBoneName;

    // offset 58040-58100: Target Pos 相关
    std::uint8_t m_byTargetPosInfo_dummy[12];
    std::uint8_t m_byTargetPosCount;
    std::uint8_t m_byTargetDestPos;

    // offset 58056-58100: Super Armor
    float m_fMaxSuperArmorGage;
    float m_fCurSuperArmorGage;
    float m_fRegenSuperArmorTime;
    float m_fDefRegenSuperArmorTime;
    float m_fRecoverySuperArmorTime;
    std::uint32_t m_dwCellID;

    // offset 58100-58136: Table ID / State
    VString m_strTableID;
    std::uint32_t m_uiForcedState;
    float m_fForcedStateApplyTime;
    float m_fProtectionAggroRatio;

    // offset 58136-58200: Projectile / GO Component
    std::vector<SDelayedProjectile*> m_vecDelayedProjectile;
    std::vector<std::tr1::shared_ptr<void>> m_GOComponentTable;

    // offset 58168-58184: Skill Mgr
    CMySkillList* m_pSkillMgr;
    int m_bTraceUser;

    // offset 58184-58500: Public Transport
    std::uint8_t m_sPublicTransportPath_dummy[264];  // VPublicTransport_cl

    // offset 58500-58592: Random / Immunity
    bool m_bPublicTransportRiding;
    float m_fShCurRandomShootProjectileDirX_unused;  // padding
    float m_shCurRandomShootProjectileDirX;
    float m_shCurRandomShootProjectileDirY;
    int m_nRandomTrapIndex;
    std::uint32_t m_dwImmunityStatus;
    bool m_bIgnoreAggroDebuff;

    // offset 58500-58592: Melee / Map
    std::vector<void*> m_vContinuousMelee;  // std::vector<SContinuousMelee*>
    float m_fBossAttackAddRate;
    float m_fBossAttackedDownRate;
    float m_fSoulCostDownRate;
    float m_fAllAttackAddRate;
    std::map<std::uint32_t, std::vector<std::uint32_t>> m_mapMeleeDebuff;
    std::map<std::uint32_t, std::uint32_t> m_mapSkillUnlock;
    bool m_bNoSkillCostSG;

    // Total size: 58592 bytes (verified from IDA)
};
