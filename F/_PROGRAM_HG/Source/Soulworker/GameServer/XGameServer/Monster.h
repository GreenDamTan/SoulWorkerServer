#pragma once

#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XGameServer/GroupAggro.h"
#include "Soulworker/GameServer/XGameServer/STMonsterInfo.h"
#include "Soulworker/GameServer/XGameServer/TraceHPState.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <map>

// 前置声明
struct TB_MONSTER;
struct TB_SKILL;
struct VMonsterSpawnInfo;
class CAi;
class CMoverEx;
class CTraceHPState;
class VString;
class VType;  // Vision Engine 类型系统

// ============================================================================
// FSMSTATES - FSM 状态枚举 (用于 ChangeAiState)
// ============================================================================
enum FSMSTATES {
    FSMSTATES_IDLE = 0,
    FSMSTATES_PATROL = 1,
    FSMSTATES_CHASE = 2,
    FSMSTATES_ATTACK = 3,
    FSMSTATES_RETURN = 4,
    FSMSTATES_RECOVERY = 5,   // 恢复状态 (用于召唤物)
    FSMSTATES_DIE = 6,
    FSMSTATES_WAIT = 8,       // 等待状态
    FSMSTATES_BEFORESTATE = 9, // 前一状态
    FSMSTATES_SUCIDE = 30,    // 自杀状态
    FSMSTATES_SELECT_ACTION = 36,  // 选择动作状态
};

// ============================================================================
// tagACTION_DAMAGE - 伤害动作数据结构
// IDA 确认大小: 86 bytes
// ============================================================================
struct tagACTION_DAMAGE {
    std::uint32_t dwID;                      // offset 0: 攻击者ID
    std::uint8_t byReactionType;             // offset 4: 反应类型
    std::uint8_t byAttackCollision;          // offset 5: 攻击碰撞
    std::uint8_t byAttackRank;               // offset 6: 攻击等级
    std::uint8_t byDamageFlag;               // offset 7: 伤害标志
    std::int32_t nDamage;                    // offset 8: 伤害值
    std::int32_t nAttrDamage;                // offset 12: 属性伤害
    float fAttackRot;                        // offset 16: 攻击旋转
    hkvVec3 vExtraMove;                      // offset 20: 额外移动向量
    std::int32_t iBattleMaterialLinkID;      // offset 32: 战斗材质链接ID
    float fEffectHeightOffset;               // offset 36: 特效高度偏移
    std::int32_t bTraceFXView;               // offset 40: 追踪特效视图
    float fTraceFXRot;                       // offset 44: 追踪特效旋转
    float fFlySpeed;                         // offset 48: 飞行速度
    std::uint8_t byWeakAttackStiffenRatio;   // offset 52: 弱攻击僵直比率
    std::int32_t nContinueAttack;            // offset 53: 连续攻击
    std::int32_t nSkillID;                   // offset 57: 技能ID
    std::uint8_t byDefenseType;              // offset 61: 防御类型
    float fSuperArmorGage;                   // offset 62: 超级护甲值
    // tagHIT_SHADER sHitShader;             // offset 66: 击中着色器 (13 bytes)
    char sHitShader[13];                     // offset 66: 击中着色器占位
    bool bPenetrate;                         // offset 79: 是否穿透
    bool bApplyEffectWhenPenetrate;          // offset 80: 穿透时应用特效
    float fDamageRate;                       // offset 81: 伤害倍率
    std::uint8_t byHitPartsIndex;            // offset 85: 击中部位索引
};

// ============================================================================
// tagDamageMeter - 仇恨计量结构
// IDA 确认大小: 8 bytes
// ============================================================================
struct tagDamageMeter {
    float fAggro;    // 仇恨值
    float fTime;     // 最后更新时间
};

// CMonster 继承自 CMoverEx (60392 bytes)
// IDA 确认大小: 60896 bytes
class CMonster : public CMoverEx {
public:
    CMonster();
    virtual ~CMonster();

    // === IDA 反编译确认的方法 ===

    // CreateObject IDA 0x140362300 - 创建怪物对象 (静态工厂方法)
    static CMonster* CreateObject();

    // GetClassTypeId IDA 0x140362360 - 获取类类型ID (静态方法)
    static VType* GetClassTypeId();

    // GetTypeId IDA 0x140362370 - 获取类型ID (虚函数)
    virtual VType* GetTypeId() const;

    // stMonsterInfo IDA 0x140280C40 - 获取怪物信息结构引用
    STMonsterInfo& stMonsterInfo();

    // GetParentID IDA 0x14009F170 - 获取父 ActorID
    UXActorID GetParentID();

    // GetAi IDA 0x14009F1A0 - 获取 AI 对象
    CAi* GetAi();

    // SetAi IDA 0x1406E0410 - 设置 AI 对象
    void SetAi(CAi* pAi);

    // ThinkFunction IDA 0x140358B00 -> 0x140359C4D - 思考函数 (主更新循环)
    virtual void ThinkFunction();

    // OnDie IDA 0x140356980 -> 0x140357988 - 死亡处理
    virtual void OnDie(XActor* pOwnerActor, float fDamage);

    // RealDie IDA 0x14035A200 -> 0x14035A5D6 - 执行死亡
    virtual void RealDie(std::int16_t nChangeMotion);

    // IsBoss IDA 0x140358570 - 检查是否是Boss
    bool IsBoss();

    // IsBoss_Named_Raid IDA 0x1403585C0 - 检查是否是Boss/命名/Raid怪物
    bool IsBoss_Named_Raid();

    // IsCanAI IDA 0x140358860 - 检查是否可以执行AI
    bool IsCanAI();

    // IsCanMove IDA 0x140358640 - 检查是否可以移动
    bool IsCanMove(bool isCheckTurnMotion);

    // IsCanDirection IDA 0x140358740 - 检查是否可以转向
    bool IsCanDirection();

    // IsCanAttack IDA 0x140358A20 - 检查是否可以攻击
    bool IsCanAttack();

    // IsCanHit IDA 0x1403589B0 - 检查是否可以被击中
    int IsCanHit(int nDownAttack, int bPassiveType);

    // IsFollower - 检查是否是跟随者
    bool IsFollower();

    // IsFollowObject - 检查是否跟随对象
    bool IsFollowObject();

    // SetSummonType IDA 0x14009F1E0 - 设置召唤类型
    void SetSummonType(std::uint8_t byType);

    // GetGroupAggro IDA 0x140198DC0 - 获取群体仇恨对象
    CGroupAggro* GetGroupAggro();

    // GetCallScriptDie IDA 0x140199230 - 获取脚本死亡调用标志
    bool GetCallScriptDie();

    // NotifyRemoved IDA 0x14018BBB0 - 通知移除（虚函数）
    virtual void NotifyRemoved();

    // GetTableID IDA 0x140364AD0 - 获取表 ID
    int GetTableID();

    // GetHP IDA 0x140364D60 - 获取当前HP
    int GetHP();

    // SetHP IDA 0x14035CD60 - 设置当前HP (虚函数，重写 CMover::SetHP)
    virtual void SetHP(int nVal) override;

    // SetHpInfo IDA 0x14035CDF0 - 设置HP信息 (虚函数)
    virtual void SetHpInfo(int nVal);

    // GetActorID IDA 0x1403559E0 - 获取ActorID
    UXActorID GetActorID();

    // GetMobTableRef IDA 0x140016EF0 - 获取怪物表引用
    TB_MONSTER* GetMobTableRef();

    // SetTablePtr IDA 0x1403558A0 - 设置表指针
    void SetTablePtr(TB_MONSTER* pTBMonster);

    // Reset IDA 0x140354D20 - 重置
    void Reset();

    // Init IDA 0x140355900 - 初始化
    void Init();

    // 动画相关
    virtual void ChangeMotion(std::int16_t nMotionClass, int bResetPlay, int iCallPos);
    bool CheckSuperArmorMotion(std::int16_t nMotionClass);
    void CheckProtectSkillUI();

    // ========================================================================
    // AI/Aggro 相关函数
    // ========================================================================

    // GetAggroList IDA 0x14027A5F0 - 获取仇恨列表
    std::map<std::uint32_t, tagDamageMeter>& GetAggroList();

    // GetAggroValue IDA 0x1403615D0 - 获取指定目标的仇恨值
    float GetAggroValue(std::uint32_t dwID);

    // ApplyAggroValue IDA 0x1403614A0 - 应用仇恨值
    void ApplyAggroValue(std::uint32_t dwID, float fAggro, bool isPlus);

    // GetTopAggroValue IDA 0x140361640 - 获取最高仇恨值
    float GetTopAggroValue();

    // UpdateHealAggro IDA 0x14035FB20 - 更新治疗仇恨
    void UpdateHealAggro();

    // CalcSkillAggroPoint IDA 0x14035F7F0 - 计算技能仇恨点数
    float CalcSkillAggroPoint(CMoverEx* pUser, float fDamage, TB_SKILL* pSkillRef);

    // CalcDotAggroPoint IDA 0x14035F9E0 - 计算DoT仇恨点数
    float CalcDotAggroPoint(float fDamage);

    // CalcHealAggroPoint IDA 0x14035FA00 - 计算治疗仇恨点数
    float CalcHealAggroPoint(CMoverEx* pUser);

    // CheckDamageAggroReset IDA 0x14035FF80 - 检查伤害仇恨重置
    void CheckDamageAggroReset(float fDist, float fTime);

    // CheckProtectAggro IDA 0x140361260 - 检查保护仇恨
    void CheckProtectAggro(std::uint32_t dwID, float fAggro);

    // DamageAggressive IDA 0x14035FC60 - 伤害激怒处理
    void DamageAggressive();

    // UpdateDamageAggressive IDA 0x14035F5B0 - 更新伤害激怒
    void UpdateDamageAggressive(CMoverEx* pMover, int nDamage);

    // ChangeTarget - 改变目标
    void ChangeTarget(UXActorID uxTargetID);

    // AddDamageMeter - 添加伤害计量
    void AddDamageMeter(CMoverEx* pMover, int nDamage, void* pSkillRef);

    // ========================================================================
    // 死亡/动作相关函数
    // ========================================================================

    // SetDie IDA 0x14035CE10 - 设置死亡状态
    virtual void SetDie(std::int16_t nMotion, int bSuicide);

    // CheckDieType IDA 0x14035CF70 - 检查死亡类型
    virtual void CheckDieType(unsigned char& byReactionType, unsigned char byDamageFlag,
                              hkvVec3& vExtraMove);

    // ActionProcess IDA 0x14035D660 - 动作处理
    virtual int ActionProcess(std::int16_t nTriggerIdx);

    // ========================================================================
    // 伤害相关函数
    // ========================================================================

    // Damage IDA 0x14035B590 - 伤害处理
    virtual void Damage(tagACTION_DAMAGE& dmgInfo, unsigned int nSkillID, bool* bSABreaked);

    // DamageProcessHP IDA 0x14035BF70 - HP伤害处理
    virtual bool DamageProcessHP(unsigned int dwID, int nSkillID, int nDamage,
                                  unsigned char byDamageFlag, unsigned char byHitParts);

    // _DamageProcessHP IDA 0x14035C050 - 内部HP伤害处理
    bool _DamageProcessHP(unsigned int dwID, int nSkillID, int nDamage,
                          unsigned char byDamageFlag, unsigned char byHitParts);

    // DamageProcess - 伤害处理包装函数
    void DamageProcess(CMover* pAttacker, int nDamage, int nSkillID,
                       unsigned char byAttackType, unsigned char byElementType,
                       unsigned char byHitType, int nAttrDamage);

    // ProcessSkillAttack - 处理技能攻击
    void ProcessSkillAttack(int nSkillID, CMoverEx* pTarget, float fDamage);

    // CheckProtectDamage IDA 0x14035B860 - 检查保护伤害
    void CheckProtectDamage(tagACTION_DAMAGE& dmgInfo);

    // OnDamageForMaze IDA 0x14035BC60 - 迷宫伤害处理
    void OnDamageForMaze();

    // ClearBuffProcess IDA 0x14035E200 - 清除Buff处理
    virtual int ClearBuffProcess(int nSkillID, AttackJudgmentTrigger* pTrigger, hkvVec3 vCurPos);

    // ApplySkillDamageFrame IDA 0x14035E920 - 应用技能伤害帧
    virtual void ApplySkillDamageFrame(unsigned int nSkillID, std::int16_t nTriggerIdx,
                                        unsigned char byAttackTargetCnt, hkvVec3& vPos,
                                        float fAttackRot, int nContinueAttack,
                                        unsigned char byDamageType, bool bPenetrate);

    // NotifyPhaseChanged IDA 0x14035ED40 - 通知阶段变化
    virtual void NotifyPhaseChanged(unsigned char byOldPhase);

    // GetDeathMotion IDA 0x14035F380 - 获取死亡动作
    std::int16_t GetDeathMotion();

    // ChangeAiScript IDA 0x14035F3D0 - 切换AI脚本
    int ChangeAiScript(const char* szAiName);

    // SetupAnimInfo IDA 0x1403601D0 - 设置动画信息
    virtual void SetupAnimInfo();

    // ForceActionSkill IDA 0x14035BD80 - 强制执行技能动作
    void ForceActionSkill(unsigned int nSkillID);

    // SetupScriptTraceHP IDA 0x14035BCA0 - 设置脚本追踪HP
    void SetupScriptTraceHP(const VMonsterSpawnInfo* pMonsterSpawn);

    // QuickTurn IDA 0x14035D290 - 快速转向
    void QuickTurn();

    // ActionAttack IDA 0x14035D460 - 执行攻击动作
    void ActionAttack();

    // PostSkillProcess IDA 0x14035D620 - 技能后处理
    virtual void PostSkillProcess();

    // DisableSummonMonster IDA 0x14035BDD0 - 禁用召唤怪物
    int DisableSummonMonster();

    // SetHpEx - 设置HP扩展
    void SetHpEx(int nHP);

    // ShowProtectSkillUI IDA 0x14035B720 - 显示保护技能UI
    void ShowProtectSkillUI(bool bActive);

    // SendNoticePacket IDA 0x14035BBE0 - 发送通知包
    void SendNoticePacket(int iType, int iValue, float fTime);

    // NotifySpawnMonsterDied IDA 0x14035F1A0 - 通知召唤怪物死亡
    virtual void NotifySpawnMonsterDied(std::uint32_t dwID);

    // GetMonsterInfo IDA 0x140355DB0 - 获取怪物信息
    STMonsterInfo& GetMonsterInfo();

    // ========================================================================
    // 简单 Getter/Setter 函数
    // ========================================================================

    // GetSpawnBoxID IDA 0x140276230 - 获取生成盒ID
    int GetSpawnBoxID() { return m_nSpawnBoxID; }

    // GetGuardID IDA 0x140276250 - 获取守护者ID
    std::uint32_t GetGuardID() { return m_dwGuardID; }

    // SetSpawnBoxID IDA 0x1401ad950 - 设置生成盒ID
    void SetSpawnBoxID(int nVal) { m_nSpawnBoxID = nVal; }

    // SetParentID IDA 0x1401ad920 - 设置父ActorID
    void SetParentID(UXActorID uxVal);

    // GetGazeAnimType IDA 0x140276850 - 获取注视动画类型
    std::uint8_t GetGazeAnimType() { return m_byGazeAnimType; }

    // GetHitCount IDA 0x14027a5d0 - 获取击中计数
    int GetHitCount() { return m_nHitCount; }

    // IsReserveDie IDA 0x140280c00 - 检查是否预留死亡
    bool IsReserveDie() { return m_bReserveDie; }

    // ResetHitCount IDA 0x1402763d0 - 重置击中计数
    void ResetHitCount() { m_nHitCount = 0; }

    // GetMoveType IDA 0x140280df0 - 获取移动类型
    int GetMoveType() { return m_nMoveType; }

    // SetSummonLifeTime IDA 0x1401ad000 - 设置召唤存活时间
    void SetSummonLifeTime(float fSummonLifeTime) { m_fSummonLifeTime = fSummonLifeTime; }

    // SetMoveType IDA 0x1401adc00 - 设置移动类型
    void SetMoveType(int nType) { m_nMoveType = nType; }

    // GetGroupID IDA 0x140353AD0 - 获取组ID
    int GetGroupID() { return m_nGroupID; }

    // SetGroupID IDA 0x140354230 - 设置组ID
    void SetGroupID(int nVal) { m_nGroupID = nVal; }

    // SetGuardID IDA 0x140353CA0 - 设置守护者ID
    void SetGuardID(std::uint32_t dwID) { m_dwGuardID = dwID; }

    // SetCallScriptDie IDA 0x140354270 - 设置脚本死亡调用标志
    void SetCallScriptDie(bool bCall) { m_bCallScriptDie = bCall; }

    // SetReserveDie IDA 0x1403542B0 - 设置预留死亡标志
    void SetReserveDie(bool bDie) { m_bReserveDie = bDie; }

    // SetNextSkillID IDA 0x140364630 - 设置下一个技能ID
    void SetNextSkillID(int nID) { m_nNextSkillID = nID; }

    // GetTraceHPState IDA 0x140364760 - 获取追踪HP状态对象
    CTraceHPState* GetTraceHPState() { return reinterpret_cast<CTraceHPState*>(m_xTraceHPState_dummy); }

    // GetLevel IDA 0x140364B10 - 获取等级
    std::uint8_t GetLevel() { return m_stMonsterInfo.stNpcInfo.byLevel; }

    // SetLevel IDA 0x140364300 - 设置等级
    void SetLevel(std::uint8_t byVal) { m_stMonsterInfo.stNpcInfo.byLevel = byVal; }

    // SetInitYaw IDA 0x1406E03F0 - 设置初始朝向
    void SetInitYaw(float fYaw) { m_fInitYaw = fYaw; }

    // IsSuicide IDA 0x1403642A0 - 检查是否自杀
    bool IsSuicide() { return m_bSuicide != 0; }

    // SetGuardMonster IDA 0x1403644C0 - 设置守护怪物ID
    void SetGuardMonster(std::uint32_t dwActorID) { m_dwGuardMonsterID = dwActorID; }

    // ========================================================================
    // AI 相关函数
    // ========================================================================

    // GetAIState IDA 0x140357A20 - 获取AI状态
    std::uint8_t GetAIState();

    // ChangeAiState IDA 0x140357A20 - 改变AI状态
    void ChangeAiState(int nNewState);

    // SetReservedMotion IDA 0x140357A60 - 设置预留动作
    void SetReservedMotion(std::int16_t nDefault, std::int16_t nReserve);

    // GetReservedMotion IDA 0x140357AB0 - 获取预留动作
    std::int16_t GetReservedMotion(std::int16_t nMotion);

    // GetAIFuzzyValue IDA 0x140357B30 - 获取AI模糊值
    void GetAIFuzzyValue(float* pfValue);

    // GetAIActionValue IDA 0x140357B70 - 获取AI动作值
    void GetAIActionValue(int* pnValue);

    // CheckWayPoint IDA 0x140357BB0 - 检查路径点
    void CheckWayPoint();

    // GetSectorID IDA 0x140357C40 - 获取扇区ID
    int GetSectorID();

    // ApplySuperArmorGage IDA 0x140358100 - 应用超级护甲值
    void ApplySuperArmorGage(float fSuperArmor);

    // ========================================================================
    // 怪物类型检查函数
    // ========================================================================

    // GetSpawnTime IDA 0x1403606A0 - 获取生成时间
    float GetSpawnTime();

    // IsTableID IDA 0x140360700 - 检查表ID是否匹配
    bool IsTableID(const char* szTableID);

    // IsDefensiveWeapon IDA 0x140360790 - 检查是否防御武器类型
    bool IsDefensiveWeapon();

    // IsEscort IDA 0x1403607D0 - 检查是否护卫类型
    bool IsEscort();

    // IsMonsterDamageCount IDA 0x140360890 - 检查是否伤害计数类型
    bool IsMonsterDamageCount();

    // IsDedicated IDA 0x1403608D0 - 检查是否专用类型
    bool IsDedicated();

    // IsMonsterCombo IDA 0x140360910 - 检查是否连击类型
    bool IsMonsterCombo();

    // IsMercenary IDA 0x140360950 - 检查是否雇佣兵类型
    bool IsMercenary();

    // IsHelper IDA 0x140360990 - 检查是否助手类型
    bool IsHelper();

    // IsRealHelper IDA 0x1403609D0 - 检查是否真实助手
    bool IsRealHelper();

    // IsNormalMonster IDA 0x140360A50 - 检查是否普通怪物
    bool IsNormalMonster();

    // IsDefenseObject IDA 0x140360A90 - 检查是否防御对象
    bool IsDefenseObject();

    // ========================================================================
    // 怪物辅助函数
    // ========================================================================

    // IsMonsterInteractObject IDA 0x140360AD0 - 检查是否交互对象类型
    bool IsMonsterInteractObject();

    // IsInheritParentStat IDA 0x140360B10 - 检查是否继承父属性
    bool IsInheritParentStat();

    // IsExceptionalDamage IDA 0x140360B70 - 检查是否例外伤害
    bool IsExceptionalDamage();

    // IsApplySilhouet IDA 0x140360F10 - 检查是否应用轮廓
    bool IsApplySilhouet();

    // IsApplyForceReaction IDA 0x140361210 - 检查是否应用力反应
    bool IsApplyForceReaction(std::uint8_t byCheckRank);

    // IsCanDamagedMonster IDA 0x140361700 - 检查是否可被伤害的怪物
    bool IsCanDamagedMonster();

    // GetMonsterFlag IDA 0x140361750 - 获取怪物标志 (虚函数重写)
    std::uint8_t GetMonsterFlag();

    // GetVariableType IDA 0x140361780 - 获取变量类型 (虚函数重写)
    int GetVariableType();

    // UpdateLinkSkill IDA 0x140361840 - 更新链接技能
    void UpdateLinkSkill(float fDeltaTime);

    // UpdateCheckAttackSkill IDA 0x140361950 - 更新检查攻击技能
    void UpdateCheckAttackSkill(float fDeltaTime);

    // CheckPassiveSkill IDA 0x140361A60 - 检查被动技能
    void CheckPassiveSkill(std::uint8_t byTargetType, std::uint8_t byCondition);

    // CheckPassiveSkillByHit IDA 0x140361B10 - 检查被动技能 (击中时)
    void CheckPassiveSkillByHit(CMoverEx* pMover, TB_SKILL* pSkillTable, std::uint8_t byResult);

    // StoreWrongPosInfo IDA 0x140361BA0 - 存储错误位置信息
    void StoreWrongPosInfo(const hkvVec3& vPos, std::uint16_t wCount);

    // CheckFollowMonster IDA 0x140360BA0 - 检查跟随怪物
    void CheckFollowMonster();

    // FindFollowPlayer IDA 0x140360C20 - 查找跟随玩家
    void FindFollowPlayer();

    // SetDefensiveWeaponPlayer IDA 0x140360D80 - 设置防御武器玩家
    void SetDefensiveWeaponPlayer(CMoverEx* pMover);

    // FindGuardMonster IDA 0x140360F60 - 查找守护怪物
    CMonster* FindGuardMonster();

    // ApplyLevelToStat IDA 0x140357C80 - 应用等级到属性
    void ApplyLevelToStat(int bInit);

    // ========================================================================
    // 怪物属性获取函数
    // ========================================================================

    // GetAttackPower IDA 0x140364B80 - 获取攻击力
    int GetAttackPower();

    // GetDefensePower IDA 0x140364BC0 - 获取防御力
    int GetDefensePower();

    // GetMoveSpeed IDA 0x140364C00 - 获取移动速度
    float GetMoveSpeed();

    // GetAttackRange IDA 0x140364C40 - 获取攻击范围
    float GetAttackRange();

    // ========================================================================
    // 怪物初始化/信息函数
    // ========================================================================

    // InitialObjectInfo IDA 0x140355120 - 初始化对象信息
    virtual void InitialObjectInfo(unsigned int dwID, unsigned int nTableIdx, hkvVec3 vPos, float fRot);

    // GenerateEventObject IDA 0x1403556D0 - 生成事件对象
    virtual void GenerateEventObject();

    // SetInfo IDA 0x140355730 - 设置信息
    virtual void SetInfo();

    // SetSyncInfo IDA 0x140355B10 - 设置同步信息
    virtual void SetSyncInfo();

    // ========================================================================
    // 怪物处理函数
    // ========================================================================

    // ProcessExp IDA 0x140355FD0 - 处理经验
    void ProcessExp(XActor* pActor);

    // DropItemByHit IDA 0x140356290 - 击中掉落物品
    void DropItemByHit(unsigned int dwAtkUser);

    // ProcessDrop IDA 0x140356550 - 处理掉落
    void ProcessDrop(XActor* pAtk);

    // ProcessEscortQuest IDA 0x140356750 - 处理护送任务
    void ProcessEscortQuest();

    // ProcessGameMode IDA 0x140356810 - 处理游戏模式
    void ProcessGameMode();

    // ========================================================================
    // 怪物移动/方向函数
    // ========================================================================

    // GetSkillDestPos IDA 0x14035A5E0 - 获取技能目标位置
    virtual hkvVec3 GetSkillDestPos();

    // ChangeAngleAttackName IDA 0x14035A6C0 - 改变角度攻击名称
    virtual void ChangeAngleAttackName(std::uint8_t bySkillAngle, VString& strSkillName);

    // IsRemainBossMonster IDA 0x14035A950 - 检查是否剩余Boss怪物
    bool IsRemainBossMonster();

    // MoveTick IDA 0x14035AB20 - 移动tick
    virtual bool MoveTick();

    // StopMoving IDA 0x14035AB60 - 停止移动
    virtual void StopMoving(bool bSendPacket);

    // IsNoRotate IDA 0x14035B260 - 检查是否不旋转
    bool IsNoRotate();

    // IsCanRotate IDA 0x14035B2A0 - 检查是否可以旋转
    bool IsCanRotate();

    // SetDirectionTo IDA 0x14035B2F0 - 设置方向到目标
    virtual void SetDirectionTo(const hkvVec3& vTarget);

    // SetDirectionYaw IDA 0x14035B370 - 设置方向偏航
    virtual void SetDirectionYaw(float fYaw, std::uint8_t byType);

    // UpdateTargetRotation IDA 0x14035B470 - 更新目标旋转
    virtual void UpdateTargetRotation(bool isGaze);

    // CancelAttackFromDamage IDA 0x14035B520 - 从伤害取消攻击
    virtual void CancelAttackFromDamage();

    // ========================================================================
    // 怪物信息/包发送函数
    // ========================================================================

    // SetInfoPacket IDA 0x140355d60 - 设置信息包
    virtual void SetInfoPacket(void* pPacket);

    // ========================================================================
    // 怪物组件/信息函数
    // ========================================================================

    // InitComponant IDA 0x1403559a0 - 初始化组件
    void InitComponant();

    // GetID IDA 0x140355a10 - 获取ID
    virtual unsigned int GetID();

    // SetPosInfo IDA 0x140355a30 - 设置位置信息
    virtual void SetPosInfo(hkvVec3 vPos, float fRot);

    // SetPositionXVec3 IDA 0x140355ad0 - 设置位置
    virtual void SetPositionXVec3(hkvVec3& vPos);

    // GetActionResourceFN IDA 0x140357990 - 获取动作资源文件名
    virtual VString GetActionResourceFN();

    // ApplyTableAbility IDA 0x1403581a0 - 应用表能力
    void ApplyTableAbility();

    // ========================================================================
    // 怪物移动/包发送函数
    // ========================================================================

    // UpdateSendMoveData IDA 0x14035abc0 - 更新发送移动数据
    void UpdateSendMoveData();

    // CheckSendMovePacket IDA 0x14035ac40 - 检查发送移动包
    virtual void CheckSendMovePacket();

    // CheckTurnOrMovePacket IDA 0x14035af30 - 检查转向或移动包
    virtual void CheckTurnOrMovePacket();

    // StartMoving IDA 0x14035b1e0 - 开始移动
    virtual int StartMoving();

    // ========================================================================
    // 怪物动作/战斗函数
    // ========================================================================

    // ChangeBattlePose IDA 0x14035a0a0 - 改变战斗姿态
    virtual void ChangeBattlePose(bool bBattle, bool bPlayMotion);

    // ActiveSuperArmorSkill IDA 0x14035a690 - 激活超级护甲技能
    virtual void ActiveSuperArmorSkill();

    // ProcessSkillAnimation IDA 0x140359e20 - 处理技能动画
    virtual void ProcessSkillAnimation(float fDeltaTime);

    // ActionBufferProcess IDA 0x140359fa0 - 动作缓冲处理
    virtual int ActionBufferProcess(void* pAction);

    // DebugMessage IDA 0x140359c50 - 调试消息
    virtual void DebugMessage();

    // ========================================================================
    // 怪物辅助函数
    // ========================================================================

    // GetOwnerPlayer - 获取拥有者玩家 (从父链获取)
    CMoverEx* GetOwnerPlayer();

    // GetTargetID - 获取当前目标ID
    std::uint32_t GetTargetID() const { return m_dwTargetID; }

    // GetMonsterFlag - 获取怪物标志
    std::uint32_t GetMonsterFlag() const;

    // ========================================================================
    // Round 5 Phase 3 - 新增函数
    // ========================================================================

    // === AI Functions ===

    // SelectAction IDA 0x140357D70 - 选择AI动作
    int SelectAction();

    // FindTarget IDA 0x140357E50 - 查找攻击目标
    CMoverEx* FindTarget();

    // CheckAggro IDA 0x140357F80 - 检查仇恨列表
    void CheckAggro();

    // UpdateAI IDA 0x140358040 - 更新AI状态
    void UpdateAI(float fDeltaTime);

    // === Combat Functions ===

    // Attack IDA 0x14035D950 - 执行攻击
    void Attack(CMoverEx* pTarget, int nSkillID, float fDamage);

    // AttackProcess - 处理攻击帧，击中检测，伤害应用
    void AttackProcess(float fDeltaTime);

    // ProcessAttack - 处理攻击结果，连击，冷却
    void ProcessAttack();

    // Die IDA 0x14035A5D6 - 处理死亡
    void Die(int nMotion, bool bSuicide);

    // Respawn IDA 0x140354F80 - 重生怪物
    void Respawn(const hkvVec3& vPos, float fRot);

    // === State Functions ===

    // IsAlive IDA 0x140364D90 - 检查是否存活
    bool IsAlive();

    // IsAggro IDA 0x1403615A0 - 检查是否有仇恨
    bool IsAggro() const;

    // === Target Functions ===

    // SetTarget IDA 0x140361700 - 设置目标
    void SetTarget(std::uint32_t dwTargetID);

    // GetTarget IDA 0x140361780 - 返回当前目标
    CMoverEx* GetTarget();

    // ClearTarget IDA 0x140361800 - 清除目标
    void ClearTarget();

    // HasTarget IDA 0x140361850 - 检查是否有目标
    bool HasTarget() const;

    // === Other Functions ===

    // GetZone IDA 0x1403559C0 - 获取当前区域
    void* GetZone();

    // ========================================================================
    // 扩展函数 - Buff (增益/减益相关)
    // ========================================================================

    // AddBuff - 添加Buff，设置持续时间，应用效果
    void AddBuff(int nBuffID, int nSkillID, float fDuration, CMoverEx* pCaster);

    // RemoveBuff - 移除Buff，恢复效果
    void RemoveBuff(int nBuffID);

    // ProcessBuff - 更新Buff计时器，过期处理
    void ProcessBuff(float fDeltaTime);

    // ========================================================================
    // 扩展函数 - Movement (移动相关)
    // ========================================================================

    // MoveTo - 设置目标位置，开始移动
    void MoveTo(const hkvVec3& vDestPos, float fSpeed);

    // StopMove - 停止当前移动，清除路径
    void StopMove();

    // ProcessMove - 更新位置，检查到达，寻路
    void ProcessMove(float fDeltaTime);

    // ========================================================================
    // 扩展函数 - Skill (技能相关)
    // ========================================================================

    // UseSkill - 触发技能，设置冷却，动画
    void UseSkill(int nSkillID, CMoverEx* pTarget);

    // CancelSkill - 取消当前技能，清理
    void CancelSkill();

    // ProcessSkill - 技能帧处理，效果
    void ProcessSkill(float fDeltaTime);

protected:
    // === IDA 确认的成员变量 (offset from CMoverEx end, 60392+) ===

    // offset 60392: m_mapReservedMotion (std::map<short,short>, 32 bytes)
    std::map<std::int16_t, std::int16_t> m_mapReservedMotion;

    // offset 60424: m_stMonsterInfo (STMonsterInfo, 128 bytes)
    STMonsterInfo m_stMonsterInfo;

    // offset 60552: m_pMobTableRef (TB_MONSTER*)
    TB_MONSTER* m_pMobTableRef;

    // offset 60560: m_pAi (CAi*)
    CAi* m_pAi;

    // offset 60568: m_arDamageMeter (std::map<ulong,tagDamageMeter>, 32 bytes)
    std::map<std::uint32_t, tagDamageMeter> m_arDamageMeter;

    // offset 60600-60608: NextSkill/SendMove
    int m_nNextSkillID;
    std::int16_t m_shLastSendMoveYaw;
    int m_bNeedSendMoveStop;

    // offset 60612-60620: Move/Yaw
    void* m_vLastTargetMovePos_dummy;  // tagMOVE_POS
    float m_fInitYaw;

    // offset 60624-60640: Debug
    hkvVec3 m_vDebugMessagePos;

    // offset 60640-60672: GroupAggro
    CGroupAggro m_xGroupAggro;

    // offset 60672-60680: Aggro
    float m_fLastAggroCheckTime;
    int m_bChangedAggro;
    std::uint32_t m_dwGuardID;

    // offset 60684: GazeAnimType
    std::uint8_t m_byGazeAnimType;

    // offset 60688-60744: TraceHPState
    void* m_xTraceHPState_dummy[7];  // CTraceHPState (56 bytes)

    // offset 60744-60752: Spawn/Group
    int m_nSpawnBoxID;
    int m_nGroupID;
    bool m_bShowChangedAI;

    // offset 60760-60768: SpawnedTime
    std::uint64_t m_dwSpawnedTime64;
    int m_bSuicide;

    // offset 60772-60780: Guard/Killer
    std::uint32_t m_dwGuardMonsterID;
    int m_nMoveType;
    std::uint32_t m_dwKillerID;

    // offset 60784-60792: Weapon
    void* m_pWeaponTableRef;  // TB_DEFENSIVE_WEAPON*

    // offset 60792-60800: LifeTime
    float m_fLifeTime;
    int m_nHPPerSec;
    int m_nLastTime;

    // offset 60804-60816: ProtectSkill
    int m_nProtectSkill;
    int m_eSkillType;  // tagTB_SKILL_TYPE
    int m_nProtectSkillDamage;
    int m_bApplyLevel;

    // offset 60820-60828: Summon
    std::uint8_t m_bySummonType;
    float m_fSummonLifeTime;
    int m_nHitCount;

    // offset 60832-60840: Script
    void* m_pScriptInst;  // IVScriptInstance*
    int m_naCount;
    bool m_bChangeAiScript;
    bool m_bCallScriptDie;

    // offset 60848-60856: DamageCount
    float m_fLastDamageCountTime;
    float m_fBossHPLogTime;
    int m_nCheckBossHP;

    // offset 60860-60868: ReserveDie
    bool m_bReserveDie;
    float m_fElapsedDieTime;
    std::uint8_t m_byAngleAttackType;

    // offset 60872-60896: TickCount/WrongPos
    std::uint64_t m_dwTickCountReceiveWrongPos;
    hkvVec3 m_vStoredWrongPos;

    // Total size: 60896 bytes (verified from IDA)
};
