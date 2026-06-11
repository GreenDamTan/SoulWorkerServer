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
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

// Buff 状态结构 (IDA 还原的完整定义)
#include "Soulworker/GameServer/XGameServer/BuffState.h"

// 前置声明 - Vision Engine 类型
class VisBaseEntity_cl;
class XActor;
class XArea;
struct VAnimationInfo;
struct VActionResourceLump;
class VPublicTransport_cl;

// 前置声明 - 游戏类型
struct TB_SKILL;
class AttackJudgmentTrigger;
struct tagEXTRA_MOVEPOS;
struct XVec3;

// E_ACTOR_TYPE 枚举前向声明
enum E_ACTOR_TYPE : std::int32_t;

// TODO: 推测结果 - 来自 IDA struct CMover (58592 bytes)
// CMover 继承自 VisBaseEntity_cl (872 bytes) + XActor (104 bytes)
// 是 Vision Engine 的核心实体类
// 注意: 完整的CMover定义在actor/Mover/Mover.h中，此文件为简化版本
// 使用actor/Mover/Mover.h中定义的完整CMover类
class CMover {
    // XActionResMgr 需要访问 protected ClearActionBuffer
    friend class XActionResMgr;

public:
    CMover();
    virtual ~CMover();

    // Vision Engine RTTI
    static VType* classCMover;  // 静态类型对象
    static VType* GetClassTypeId();  // IDA 0x1403743a0
    virtual VType* GetTypeId() const;  // IDA 0x1403743b0

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

    // Actor 类型相关方法 (代理到 m_eActorType)
    E_ACTOR_TYPE GetType() const { return static_cast<E_ACTOR_TYPE>(m_eActorType); }
    std::uint32_t GetOriginID() const { return m_uxOriginID; }
    void SetOriginID(std::uint32_t dwID) { m_uxOriginID = dwID; }
    std::uint8_t GetNation() const { return m_byNation; }
    void SetNation(std::uint8_t byNation) { m_byNation = byNation; }
    XArea* GetArea() const;  // 返回区域对象

    // 位置/移动 (IDA 反编译)
    virtual void SetPositionXVec3(const hkvVec3& vPos);

    // 位置/移动
    hkvVec3 GetPosition() const;
    void SetPosition(const hkvVec3& vPos);
    hkvVec3& GetPositionXVec3();
    hkvVec3& GetCreatePos();          // IDA 0x1402762B0
    tagEXTRA_MOVEPOS& GetExtraMovePos();  // IDA 0x140276330

    // Move - 位置移动 (IDA 0x14036DDD0)
    virtual void Move(const hkvVec3& vDestPos);
    float GetMoveSpeed() const { return m_fMoveSpeed; }
    void SetMoveSpeed(float fSpeed) { m_fMoveSpeed = fSpeed; }
    float GetMoveSpeed();  // IDA 0x1406C5C30

    // DebugOut - 调试输出 (IDA 反编译中使用)
    void DebugOut(const char* szFormat, ...);

    // 移动状态
    bool IsMoving();       // IDA 0x14027A610
    bool IsGazeMoving();   // IDA 0x140375200
    void MoveingValueClear();  // IDA 0x1402A4BE0 - 清除移动相关值

    // Actor 状态检查 (来自 XActor 基类) - 声明为成员函数，实现在 .cpp
    virtual UXActorID GetActorID() const;
    std::uint32_t GetID() const { return GetActorID().GetID(); }
    bool IsStatus(std::uint32_t dwStatus) const;
    std::uint32_t GetStatus() const;
    void SetStatus(std::uint32_t dwStatus);
    void ClearStatus(std::uint32_t dwStatus);

    template<typename T>
    std::tr1::shared_ptr<T> GetGOC(bool bCreate = false) {
        (void)bCreate;
        return std::tr1::shared_ptr<T>();
    }

    // GOC accessor stubs for components
    std::tr1::shared_ptr<class CGocPost> GetGOC_Post(bool bCreate = false);

    // 目标位置
    std::uint8_t GetTargetDestPos();  // IDA 0x140280C80
    void SetTargetDestPos(std::uint8_t byPos);  // IDA 0x140280C60

    // Cell ID
    std::uint32_t GetCellID();  // IDA 0x140280CC0
    void SetCellID(std::uint32_t dwID);  // IDA 0x140280CE0

    // Create position
    void SetCreatePos(const hkvVec3& vPos);  // IDA 0x140280D40

    // 额外移动 (击退、拉扯等效果)
    void SetKeepMovingExtra(int bKeepMoving);  // IDA 0x1402C7420
    virtual void ProcessExtraMoving();  // IDA 0x14036BC20
    virtual void ReleaseExtraMoving();  // IDA 0x14036C120
    virtual void ClearExtraMoving();  // IDA 0x140189390
    virtual void AddExtraMoving(float x, float y, float fTime);  // IDA 0x14036C210
    virtual void SetExtraMoving(float x, float y, float fTime);  // IDA 0x14036C380

    // 能力值/状态
    float GetStat(int iIndex);
    virtual int GetHP() const;
    virtual int GetMaxHP();
    virtual std::uint8_t GetLevel();
    virtual std::uint8_t GetClass();

    // 初始化
    virtual void InitFunction();

    // 思考函数
    virtual void ThinkFunction();

    // 获取动作类
    short GetMotionClass();

    // 状态检查
    bool IsDie() const;
    bool IsLive() const;  // 返回 !IsDie()
    bool IsFlying();
    bool IsKnockDown();
    bool IsHit();
    bool IsMoveingInFly();  // IDA 0x1403a2850
    bool IsTraceUser();     // IDA 0x140406e70

    // 等级获取 (IDA 反编译)
    virtual std::uint8_t GetLevelForStat();  // IDA 0x140366D30

    // 敌对关系检查 IDA 0x14036CD70 - 实现在 actor/Mover/Mover.cpp
    virtual bool IsEnemy(CMover* pMover) const;
    virtual int IsFriend(CMover* pMover);  // IDA 0x140380940 - CMoverEx override
    virtual int IsParty(CMover* pMover);   // IDA 0x140380AD0 - CMoverEx override
    virtual int IsEnemyForChain(CMover* pMover);  // IDA 0x140380760 - CMoverEx override
    virtual int IsFriendForChain(CMover* pMover); // IDA 0x140380AA0 - CMoverEx override
    bool IsHitDown();
    bool IsGeneralHit();
    bool IsFlyHit();
    bool IsCounterAttackHit();
    bool IsDashing();
    bool IsAllowPassiveType(int nType);  // IDA 0x140364670

    // 动画控制
    void SetAnimSpeed(float fSpeed);
    void SetSlowTime(float fTime, float fSpeed);
    void SetReactionRate(float fRate);  // IDA: ?SetReactionRate@CMover@@QEAAXM@Z
    float GetCurrentAnimationLength();
    void SetCurrentSequenceTime(float fTime);
    void SetCurrentSequencePosition(float fPos);
    int AnimKeyToMotion(unsigned int dwAnimKey);
    unsigned int GetAnimIndex(const VString& strAnimName);  // IDA 0x140368960
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

    // 设置移动位置 (IDA 0x14036CC00)
    void SetMovePosition(float fXpos, float fYpos);

    // 获取移动偏航角 (待实现)
    float GetMovingYaw() const;

    // 静态函数 - 获取 Mover 对象
    static CMover* GetMoverObject(std::uint32_t dwID);

    // 移动数据包发送函数 (IDA 反编译)
    void send_eSUB_CMD_MOVE(CMover* pMover, float fTargetPosX, float fTargetPosY, std::uint8_t byRunBit);  // IDA 0x14036EAC0
    void send_eSUB_CMD_MOVE_STOP(CMover* pMover);  // IDA 0x14036EE90
    void send_eSUB_CMD_MOVE_IGNORE_MOTION_DELTA(CMover* pMover, const hkvVec3& vPos, bool bFlag);  // IDA 0x140370100
    void send_eSUB_CMD_JUMP(CMover* pMover, float fJumpHeight);  // 跳跃数据包
    void send_eSUB_CMD_MOVE_IDLE(CMover* pMover, float fTime);  // 空闲移动包
    void send_eSUB_CMD_MOVE_BATTLE(CMover* pMover, bool bPlayMotion);
    void send_eSUB_CMD_ACTIVE_SKILL(CMover* pMover, std::uint32_t nSkillID, std::uint8_t byAngleAttackType);
    void BroadcastMove(const hkvVec3& vPos);  // 广播移动位置

    // 碰撞控制
    void SetCollisionEnable(bool bEnable, bool bUnk);
    void StopMoving(bool bStop);

    // 动作切换
    virtual void ChangeMotion(std::int16_t wType);  // IDA 0x1402AC570 (基类空实现)

    // 死亡处理
    void ProcessDie();  // 处理死亡流程

    // 地面高度
    float GetGroundHeight(const hkvVec3& vPos, float fMaxDist = 200.0f);  // 获取地面高度

    // 目标位置标志
    void ClearTargetPosFlag(CMover* pTarget, std::uint8_t byPos);

    // 攻击高度检测
    bool IsAttackHeight(void* pAttackArea, hkvVec3& vPos, int& bCheckCylinder);

    // 动画注册
    bool IsRegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, void* strAnimName);

    // 无敌状态
    void SetInvincibleActor(int bEnable);
    int IsInvincibleActor();  // IDA 0x1401B4840

    // 免疫状态
    void SetImmunityStatus(std::uint32_t dwStatus);  // IDA 0x1402A4F90

    // 超级护甲
    float GetCurSuperArmorGage();  // IDA 0x1402A5030
    float GetMaxSuperArmorGage();  // IDA 0x1402A5050

    // 移动位置和方向 (IDA 反编译)
    tagMOVE_POS GetMovePos();  // IDA 0x1403751D0 - returns by value
    virtual float GetOrientationYaw();  // IDA 0x140375220 (虚函数)

    // 血液减益相关 (IDA 反编译)
    std::uint32_t GetBloodDebuffOwnerID();  // IDA 0x1403A23B0
    float GetSkillBloodDebuffRate();  // IDA 0x1403A23D0
    int GetAllowBloodCount();  // IDA 0x1403A23F0
    float GetSkillBloodRate();  // IDA 0x1403A2410

    // Buff 状态相关 (IDA 反编译)
    int GetBuffStatusCount(int nVal);  // IDA 0x1403A26D0
    tagBUFF_STATE* GetBuffStatus(int nVal);  // IDA 0x14070AB00
    int FindBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwAttackerID);
    virtual std::uint16_t GetComboCount() { return 0; }
    float GetAllAttackAddRate();  // IDA 0x1403A2470
    float GetBossAttackedDownRate();  // IDA 0x1403A2490
    float GetBossAttackAddRate();  // IDA 0x1403A24B0
    float GetBuffSuperArmorRate();  // IDA 0x1403A24D0
    float GetHavokCapsuleHeight();  // IDA 0x1403A24F0
    float GetBuffAddGoldRate();  // IDA 0x1403A2510
    std::uint8_t GetDefaultAnimStep();  // IDA 0x1403751B0

    // 重置所有 Buff (IDA 反编译)
    void ResetAllBuff();  // IDA 0x14036A860

    // Buff 槽位管理 (IDA 反编译)
    int GetEmptyBuffSlot();  // IDA 0x14036A810

    // 物品使用检查 (IDA 反编译)
    bool CanUseItem(std::uint32_t dwID, std::uint32_t& dwError);  // IDA 0x14036B530

    // 设置函数 (IDA 反编译)
    void SetDieFadeTime(float fTime);  // IDA 0x1403A2550
    void AnimPause();  // IDA 0x1403a2390 - 暂停动画
    virtual void ApplyBuffStatus(std::int16_t nIndex, float fElapsedTime);  // IDA 0x1403774d0 - 基类空实现
    void SetRestoreDefenseType();  // IDA 0x1403a2830 - 无参数版本
    void SetHitStatus(int nHitStatus);  // IDA 0x1403e1bf0
    void SetTraceUser(bool bTrace);  // IDA 0x140406e90
    void SetOrientationYaw(float fYaw);  // IDA 0x1402C7C60

    // 召唤怪物列表 (IDA 反编译)
    void AddSummonMobList(std::uint32_t dwMobID);  // IDA 0x1402C7CC0

    // 技能消耗相关 (IDA 反编译)
    int GetIgnoreSkillCost();  // IDA 0x1402C7F00
    bool IsNoSkillCostSG();  // IDA 0x1402C7F20
    float GetDecreaseStaminaRate();  // IDA 0x1402C7EE0

    // 随机射击方向 (IDA 反编译)
    std::int16_t GetCurRandomShootProjectileDirY();  // IDA 0x1402C7A20
    std::int16_t GetCurRandomShootProjectileDirX();  // IDA 0x1402C7A40

    // 死亡延迟时间 (IDA 反编译)
    float GetDieDelayTime();  // IDA 0x1402C7BD0

    // Soul 消耗相关 (IDA 反编译)
    float GetSoulCostDownRate();  // IDA 0x1402C7200

    // 重量等级 (IDA 反编译)
    std::uint8_t GetWeightRank();  // IDA 0x1402C72D0

    // 伤害动作标志 (IDA 反编译)
    std::uint8_t GetDmgMotionFlag();  // IDA 0x1402C7310

    // 移动客户端停止 (IDA 反编译)
    void MoveingClientStop();  // IDA 0x14036DE80

    // 反应目标检查 (IDA 反编译)
    int CheckReactionTarget(int iTargetType, CMover* pTargetMover, bool bCheckForChain);  // IDA 0x14036CE70

    void ScanGridOrigin(int nRange, unsigned int uFlag, std::vector<CMover*>* vecOut);

    virtual void ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx,
                                       std::uint8_t byAttackTargetCnt, hkvVec3* vPos,
                                       float fDirYaw, std::uint16_t wContinousHit,
                                       bool byDamageType, bool bPenetrate);
    void send_eSUB_CMD_ACTION_SKILL(CMover* pMover, std::uint32_t nSkillID,
                                    std::int16_t nTriggerIdx, hkvVec3* vPos,
                                    std::uint8_t byAttackTargetCnt,
                                    std::uint16_t wContinousHit, bool bPenetrate);
    void send_eSUB_CMD_CHAIN(CMover* pMover, std::uint32_t nSkillID,
                             std::int16_t nTriggerIdx, hkvVec3* vPos, hkvVec3* vDir,
                             std::uint32_t nSessionID, std::uint32_t dwTargetID);

    // 怪物交互对象检查 (IDA 反编译)
    // 基类返回0，子类CMonster可能重写
    int CheckMonsterInteractObject(CMover* pMover);  // IDA 0x140360AD0 (CMonster::IsMonsterInteractObject)

    // 目标位置槽位管理 (IDA 反编译)
    std::uint8_t FindTargetPos(CMover* pMover);  // IDA 0x14036D380
    std::uint8_t FindTargetPos(float fAngleMin, float fAngleMax, int eIgnoreMoveSide);  // IDA 0x14036D6F0
    hkvVec3 GetTargetPos(std::uint8_t byPos, float fDist);  // IDA 0x14036D930
    float GetTargetAngle(std::uint8_t byPos);  // IDA 0x14036DA00
    void ClearTargetPosFlag(std::uint8_t byPos);  // IDA 0x14036DB00
    std::uint32_t GetHitID() const { return m_dwHitID; }

    // 方向角度计算 (IDA 反编译)
    static float GetYawFromVector(const hkvVec3& vDir);  // IDA 0x14036DC00
    static bool IsValidPos(const hkvVec3& vPos);  // IDA 0x14036DD00
    static bool IsValidPos(float fX, float fY);  // IDA 0x14036DD40
    static bool IsValidRot(float fRot);  // IDA 0x14036DD80

    // 移动检查 (IDA 反编译)
    bool CheckMoveDestPos(hkvVec3& vDestPos, int bFlying, int bDontCareCurve);  // IDA 0x14036DEE0

    // SG 吸收率
    float GetSGAbsorbRate();  // IDA 0x14036E200

    // 持续消耗
    void SetContinousCost(int nSkillID, float fCost);  // IDA 0x14036E2E0

    // 延迟投射物
    void DeleteDelayedProjectile(int nIndex);  // IDA 0x14036E390
    void CheckDelayedProjectile();  // IDA 0x14036E460
    int GetUsedDelayedProjectile();  // IDA 0x14036E5A0

    // 过滤数据
    void SetFilterData(std::uint32_t dwID, std::uint8_t byType, float fValue);  // IDA 0x14036E640

    // 同步移动
    void SyncMove();  // IDA 0x14036E6E0

    // 忽略仇恨减益
    void SetIgnoreAggroDebuff(int bApply);  // IDA 0x1402A67F0

    // GOC 组件访问 (IDA 反编译)
    std::shared_ptr<class CGocAttribute> GetGOC_Attribute(bool bCreateIfNull) const;  // IDA 0x1404BFD0

    // 随机陷阱索引
    int GetRandomTrapIndex() const { return m_nRandomTrapIndex; }
    void SetRandomTrapIndex(int nIndex) { m_nRandomTrapIndex = nIndex; }

    // 恢复防御类型
    std::uint8_t GetRestoreDefenseType();  // IDA 0x140276290

    // 动画索引
    std::uint32_t GetAnimationIdx();  // IDA 0x1402762D0

    // 攻击者计数
    std::uint8_t GetAttackerCount();  // IDA 0x1402762F0

    // 飞行移动状态
    void SetMoveingInFly(int bFlying);

    // 场景切换
    virtual void SceneChanged();

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

    // 伤害计算 (基类返回0，子类重写)
    virtual int GetDamageCalc(int nAP, std::uint8_t byType, float fReduceRate);
    virtual std::uint8_t GetItemRateFlag();
    std::uint32_t GetItemRateResultWeapon(std::uint8_t byTargetLevel, std::tr1::shared_ptr<class CGocAttribute> pAttr, bool bCritical);
    std::uint32_t GetItemRateResultGear(std::uint8_t byTargetLevel, std::tr1::shared_ptr<class CGocAttribute> pAttr);
    bool IsImmunityStatus() const;

    // 技能冷却速率 (IDA 0x1402C7240)
    float GetSkillCoolDownRate() const;
    void SetSkillCoolDownRate(float fRate);

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

    // On Ground
    void SetOnGround(int bGround, float fPosZ);  // IDA 0x14052a1b0

    // Random Shoot Projectile Direction
    void SetCurRandomShootProjectileDirY(std::int16_t shVal);  // IDA 0x1405fa280
    void SetCurRandomShootProjectileDirX(std::int16_t shVal);  // IDA 0x1405fa2b0

    // 重置所有状态
    void Reset();
    void Destroy();
    void AllBuffClear(std::uint8_t byReason = 0);  // IDA 0x14036AA40
    
    // Buff 添加/移除
    bool AddBuff(int nBuffID, int nDuration, std::uint32_t dwSourceID = 0, int bNotify = 1);
    void RemoveBuff(int nBuffID, int bNotify = 1);
    virtual bool SetBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwOwnerID, bool bShowBuff);

protected:
    // Buff 辅助函数 (供 AllBuffClear 内部调用)
    bool IsClearBuff(int nBuffIndex, std::uint8_t byReason);
    void ClearBuffStatusBySlot(std::uint8_t bySlot, int bNotify);
    
    // 辅助函数 (供 Reset/Destroy 内部调用)
    void ResetAkashicActionInfo();
    void ClearActionBuffer();
    // === IDA 确认的成员变量 ===
    // offset 0-871: VisBaseEntity_cl 基类
    // offset 872-975: XActor 基类

    // IDA 0x1402A5080 确认: m_vPosition 在 CMover 偏移量 0 的位置
    // (继承自 VisObject3D_cl，位于 VisBaseEntity_cl 基类中)
    hkvVec3 m_vPosition;  // 从 VisObject3D_cl 继承

    // Actor 状态 (来自 XActor 基类)
    std::uint32_t m_dwStatus;

    // Actor 类型 (来自 XActor 基类)
    // IDA 0x140189240 确认: GetVariableType 返回 this->m_eActorType
    std::int32_t m_eActorType;  // E_ACTOR_TYPE enum

    // Origin ID 和 Nation (来自 XActor 基类)
    std::uint32_t m_uxOriginID;  // UXActorID
    std::uint8_t m_byNation;

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
    tagMOVE_POS m_stMovePos;      // 8 bytes
    tagMOVE_POS m_stMoveGap;      // 8 bytes
    tagMOVE_POS m_stMoveOffset;   // 8 bytes
    tagEXTRA_MOVEPOS m_stExtMovingVal;  // 17 bytes - IDA 反编译确认
    tagTIME_SLOW m_stTimeSlow;    // 8 bytes

    // offset 4684-4708: 位置
    hkvVec3 m_vPrevPos;   // IDA 反编译确认
    hkvVec3 m_vCreatePos;
    hkvVec3 m_vOrientation;  // IDA 0x140375220: 方向向量，GetOrientationYaw返回x分量

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
    std::uint8_t m_byTargetPosInfo[12];  // IDA: 目标位置计数数组
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
