#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <memory>

// Vision Engine 类型 stub 定义
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"

// 前置声明 - Vision Engine 类型
class VisBaseEntity_cl;
class XActor;
struct VAnimationInfo;
struct VActionResourceLump;
class VPublicTransport_cl;

// 前置声明 - 游戏类型
struct TB_SKILL;
class AttackJudgmentTrigger;
struct tagEXTRA_MOVEPOS;
struct XVec3;

// TODO: 推测结果 - 来自 IDA struct CMover (58592 bytes)
// CMover 继承自 VisBaseEntity_cl (872 bytes) + XActor (104 bytes)
// 是 Vision Engine 的核心实体类
class CMover {
    // XActionResMgr 需要访问 protected ClearActionBuffer
    friend class XActionResMgr;

public:
    CMover();
    virtual ~CMover();

    // 基础方法
    virtual void OnUpdate(float fDelta);
    virtual void OnDamage(int nDamage, CMover* pAttacker);
    virtual void OnDie();

    // 死亡/伤害相关 (IDA 反编译)
    virtual void SetDie(std::int16_t nMotionClass, int bSuicide, bool bSendPacket);
    virtual void Damage(std::uint32_t dwID, std::uint8_t byReactionType, std::uint8_t byAttackCollision);
    virtual void ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx, std::uint8_t byAttackTargetCnt);
    virtual void SetHP(int nHP);
    virtual int DamageProcessHP(std::uint32_t dwID, int nSkillID, int nDamage, int nUnk1, std::uint8_t byUnk1, std::uint8_t byUnk2);
    virtual int ActionProcess(std::int16_t nTriggerIdx);
    virtual int ClearBuffProcess(int nSkillID, class AttackJudgmentTrigger* pTrigger, hkvVec3& vCurPos);

    // 获取器 (IDA 反编译)
    std::uint32_t GetTargetID() const;
    void SetTargetID(std::uint32_t dwID) { m_dwTargetID = dwID; }
    std::uint8_t GetDefenseType() const;
    const VAnimationInfo* GetCurMotionEvent() const;
    virtual hkvVec3 GetSkillDestPos();
    virtual VString GetActionResourceFN();
    virtual int GetVariableType();  // 返回 E_ACTOR_TYPE

    // 位置/移动 (IDA 反编译)
    virtual void SetPositionXVec3(const hkvVec3& vPos);

    // 位置/移动
    hkvVec3 GetPosition() const;
    void SetPosition(const hkvVec3& vPos);
    hkvVec3& GetPositionXVec3();
    float GetMoveSpeed() const { return m_fMoveSpeed; }
    void SetMoveSpeed(float fSpeed) { m_fMoveSpeed = fSpeed; }
    float GetMoveSpeed();  // IDA 0x1406C5C30

    // 移动状态
    bool IsMoving();       // IDA 0x14027A610
    bool IsGazeMoving();   // IDA 0x140375200

    // 目标位置
    std::uint8_t GetTargetDestPos();  // IDA 0x140280C80
    void SetTargetDestPos(std::uint8_t byPos);  // IDA 0x140280C60

    // 额外移动 (击退、拉扯等效果)
    void SetKeepMovingExtra(int bKeepMoving);  // IDA 0x1402C7420
    virtual void ProcessExtraMoving();  // IDA 0x14036BC20
    virtual void ReleaseExtraMoving();  // IDA 0x14036C120
    // Note: ClearExtraMoving is declared above at line 60
    virtual void AddExtraMoving(float x, float y, float fTime);  // IDA 0x14036C210
    virtual void SetExtraMoving(float x, float y, float fTime);  // IDA 0x14036C380

    // 能力值/状态
    float GetStat(int iIndex) const;
    virtual int GetHP();
    virtual int GetMaxHP();
    virtual std::uint8_t GetLevel();
    virtual std::uint8_t GetClass();

    // 初始化
    virtual void InitFunction();

    // 状态检查
    bool IsDie();
    bool IsFlying();
    bool IsKnockDown();
    bool IsHit();
    bool IsHitDown();
    bool IsGeneralHit();
    bool IsFlyHit();
    bool IsCounterAttackHit();
    bool IsDashing();

    // 动画控制
    void SetAnimSpeed(float fSpeed);
    void SetSlowTime(float fTime, float fSpeed);
    float GetCurrentAnimationLength();
    void SetCurrentSequenceTime(float fTime);
    void SetCurrentSequencePosition(float fPos);
    int AnimKeyToMotion(unsigned int dwAnimKey);
    void CheckAnimationEnd();
    char* GetAnimStirng(unsigned int dwAnimKey);

    // 物理设置
    void SetupPhysicsAndBound(float fCollisionRadius, float fCollisionHeight);
    void SetupAnimation();

    // 仇恨相关
    void SetProtectionAggroRatio(float ratio);

    // 碰撞相关
    int GetHitCollisionCount();
    bool IsDamageMotionDisplay(std::uint8_t byAttackCollision);
    bool IsActivateSkillUnlockBuff(TB_SKILL* pTBSkill);

    // 移动/碰撞检测
    CMover* CheckMoveCollision(hkvVec3& vDestPos);
    void RemoveTargetDestPos();
    bool CheckMoveDestPos(hkvVec3& vDestPos, bool bFlying, int nFlag);
    bool GetHeight(hkvVec3* vPos, float fMaxDist);
    float GetHavokCapsuleRadius();  // IDA 0x140276870
    void ClearMotion();

    // 静态函数 - 获取 Mover 对象
    static CMover* GetMoverObject(std::uint32_t dwID);

    // 移动数据包发送函数 (IDA 反编译)
    void send_eSUB_CMD_MOVE(CMover* pMover, float fTargetPosX, float fTargetPosY, std::uint8_t byRunBit);  // IDA 0x14036EAC0
    void send_eSUB_CMD_MOVE_STOP(CMover* pMover);  // IDA 0x14036EE90
    void send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(CMover* pMover, const hkvVec3& vPos, bool bFlag);  // IDA 0x140370100

    // 目标位置标志
    void ClearTargetPosFlag(CMover* pTarget, std::uint8_t byPos);

    // 攻击高度检测
    bool IsAttackHeight(void* pAttackArea, hkvVec3& vPos, int& bCheckCylinder);

    // 动画注册
    bool IsRegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, void* strAnimName);

    // 无敌状态
    void SetInvincibleActor(int bEnable);

    // 表 ID
    virtual int GetTableID();
    const char* GetTableIDString();

    // 技能管理器
    CMySkillList* GetSkillMgr();

    // 技能表索引
    void SetCurSkillTableIdx(int nIdx);
    int GetCurSkillTableIdx() const { return m_nCurSkillTableIdx; }

    // 技能等级 (基类返回0，子类CMoverEx override)
    virtual std::uint8_t GetSkillLevel();

    // Hit Collision / Action Buffer
    void SetHitCollisionData(void* pData);
    void SetHitCylinder(float fRadius, float fHeight);
    void AddActionBuffer(void* xAction);

    // Trace Bone Name
    void ClearTraceBoneName();
    void RegisterTraceBoneName(const VString& strBoneName);

    // Skill Cost
    void SetNoSkillCostSG(bool bCost);

    // Weight Rank
    void SetWeightRank(std::uint8_t cVal);

    // Damage Motion Flag
    void SetDmgMotionFlag(std::uint8_t byFlag);

    // 重置所有状态
    void Reset();
    void Destroy();
    void AllBuffClear(int nFlag);

protected:
    // 辅助函数 (供 Reset/Destroy 内部调用)
    void ResetAkashicActionInfo();
    void ClearActionBuffer();
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
