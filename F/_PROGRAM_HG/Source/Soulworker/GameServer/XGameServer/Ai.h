#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"  // for tagCOOLTIME
#include "Soulworker/GameServer/XGameServer/FsmClass.h"     // for CFsmClass

// 前置声明
class CMonster;
class CMoverEx;
class CMover;
struct TB_SKILL;
class CFsmTransition;
class VString;

// ============================================================================
// E_PATHFIND_RESULT - 寻路结果枚举
// ============================================================================
enum E_PATHFIND_RESULT {
    PATHFIND_FAILED = 0,
    PATHFIND_SUCCESS = 2,
    PATHFIND_IN_PROGRESS = 1
};

// ============================================================================
// E_DAMAGE_TYPE - 伤害类型枚举 (IDA confirmed from AddEscapePoint)
// ============================================================================
enum E_DAMAGE_TYPE {
    eDAMAGE_TYPE_SKILL = 0,
    eDAMAGE_TYPE_PROJECTTILE = 1,
    eDAMAGE_TYPE_TRAP = 2
};

// ============================================================================
// E_ESCAPE_STATE - 逃脱状态枚举 (IDA confirmed)
// ============================================================================
enum E_ESCAPE_STATE {
    eESCAPE_STATE_NONE = 0,
    eESCAPE_STATE_HIT = 1
};

// ============================================================================
// E_MOVESIDE_TYPE - 移动侧向类型枚举
// ============================================================================
enum E_MOVESIDE_TYPE {
    eMOVESIDE_RANDOM = 0,
    eMOVESIDE_LEFT = 1,
    eMOVESIDE_RIGHT = 2
};

// ============================================================================
// E_FSMDATATYPE - FSM 数据类型枚举
// ============================================================================
enum E_FSMDATATYPE {
    FSMDTYPE_NONE = -1,
    FSMDTYPE_INT = 0,
    FSMDTYPE_FLOAT = 1,
    FSMDTYPE_RANDOMINT = 2,
    FSMDTYPE_RANDOMFLOAT = 3
};

// ============================================================================
// E_FSMCONDITIONS - FSM 条件函数枚举
// ============================================================================
enum E_FSMCONDITIONS {
    CONDITION_NONE = 0,
    CONDITION_EQUAL = 1,           // "="
    CONDITION_NOT_EQUAL = 2,       // "!="
    CONDITION_GREATER_THAN = 3,    // ">"
    CONDITION_LESS_THAN = 4,       // "<"
    CONDITION_BIT_EQUAL = 5,       // "&"
    CONDITION_RANGE_TRUE = 6,      // "<>"
    CONDITION_RANGE_FALSE = 7,     // "!<>"
    CONDITION_RANGE_EQUAL = 8      // "&="
};

// ============================================================================
// E_FSMVARIABLES - FSM 变量名枚举
// ============================================================================
enum E_FSMVARIABLES {
    FSMVAR_TARGET = 1,
    FSMVAR_DISTANCE = 2,
    FSMVAR_DISTANCE_CAPSULE = 3,
    FSMVAR_HEALTH = 10,
    FSMVAR_TARGET_NUMBER = 4,
    FSMVAR_RANDOM = 12,
    FSMVAR_RANDOM_PREVALUE = 13,
    FSMVAR_IS_MOVING = 14,
    FSMVAR_IS_ATTACK = 9,
    FSMVAR_IS_CHANGE_HP = 11,
    FSMVAR_TARGET_HEALTH = 15,
    FSMVAR_IS_REQUEST_HELP_NUM = 16,
    FSMVAR_IS_MONSTER_COUNT = 20,
    FSMVAR_IS_SPAWN_MONSTER_COUNT = 21,
    FSMVAR_IS_SECTOR_MONSTER_COUNT = 22,
    FSMVAR_IS_SECTOR_MONSTER_COUNT_BY_ID = 23,
    FSMVAR_IS_PRE_SKILL_DAMAGE_COUNT = 24,
    FSMVAR_IS_TARGET_SKILL = 25,
    FSMVAR_IS_TARGET_STATE = 26,
    FSMVAR_IS_TARGET_DAMAGE_STATE = 27,
    FSMVAR_IS_TARGET_MOVE_STATE = 28,
    FSMVAR_IS_TARGET_BUFF_INDEX = 29,
    FSMVAR_TARGET_ATTACKER = 30,
    FSMVAR_TARGET_COMBO = 31,
    FSMVAR_TARGET_DIRECTION = 5,
    FSMVAR_TARGET_LOOK = 32,
    FSMVAR_STATE_TIME = 8,
    FSMVAR_CREATE_POS_DISTANCE = 6,
    FSMVAR_PATROL_STATE = 33,
    FSMVAR_LAST_SKILL_TIME = 34,
    FSMVAR_LAST_DAMAGE_TIME = 35,
    FSMVAR_SPAWN_TIME = 36,
    FSMVAR_ACTIVATE_TIME = 37,
    FSMVAR_GUARD_DISTANCE = 38,
    FSMVAR_GUARD_DISTANCE_CAPSULE = 39,
    FSMVAR_FUZZY = 48,
    FSMVAR_STATE_END_TIME = 49,
    FSMVAR_STATE_FAIL_COUNT = 50,
    FSMVAR_FRIEND_COUNT = 51,
    FSMVAR_ENEMY_COUNT = 52,
    FSMVAR_USER_COUNT = 56,
    FSMVAR_AGGRO_COUNT = 53,
    FSMVAR_HIT_COUNT = 54,
    FSMVAR_GUARD_HEALTH = 40,
    FSMVAR_GUARD_FRIEND_COUNT = 41,
    FSMVAR_GUARD_ENEMY_COUNT = 42,
    FSMVAR_GLOBAL_COOLTIME = 45,
    FSMVAR_GROUP_COOLTIME = 46,
    FSMVAR_SKILL_COOLTIME = 47,
    FSMVAR_MOVE_DISTANCE_AFTER_SKILL = 55
};

// ============================================================================
// E_PROTECT_STATE - 保护状态枚举
// ============================================================================
enum E_PROTECT_STATE {
    ePROTECT_NONE = 0,
    ePROTECT_ACTIVE = 1,
    ePROTECT_PENDING = 2
};

// ============================================================================
// StateVarInfo - 状态变量信息 (IDA 确认结构)
// IDA: size=72, DataList[5][2] at offset 0, NextStates at offset 40
// ============================================================================
struct StateVarInfo {
    int DataList[5][2];           // [0] 40 bytes - 5组数据，每组2个int
    std::map<int, int> NextStates; // [40] 32 bytes - 状态映射表

    StateVarInfo() {
        std::memset(DataList, 0, sizeof(DataList));
    }

    StateVarInfo& operator=(const StateVarInfo& other) {
        if (this != &other) {
            std::memcpy(DataList, other.DataList, sizeof(DataList));
            NextStates = other.NextStates;
        }
        return *this;
    }
};

// ============================================================================
// VPList - 前置声明，使用 VisionEngineTypes.h 中的定义
// ============================================================================
class VPList;

// ============================================================================
// CAi - AI控制器类
// IDA 确认的 AI 行为控制类
// ============================================================================
class CAi {
public:
    CAi();
    virtual ~CAi();

    // === IDA 反编译确认的方法 ===

    // Initialize IDA 0x1402623F0 -> 0x140263160
    void Initialize(CMonster* pMonster);

    // Update - 更新AI
    virtual void Update(float fDeltaTime);

    // SelectAction IDA 0x14026ADF0 -> 0x14026B4B2 - AI行为选择
    void SelectAction();

    // SelectActionToFuzzyType IDA 0x14026B4C0 -> 0x14026B517
    int SelectActionToFuzzyType(int nSelect);

    // SelectActionToState IDA 0x14026B520 -> 0x14026B5A8
    int SelectActionToState(int nSelect);

    // StateToSelectAction IDA 0x14026B5B0 -> 0x14026B670
    int StateToSelectAction(int nState);

    // FindTargetBySkill IDA 0x14027CAA0 -> 0x14027CB35
    CMoverEx* FindTargetBySkill();

    // FuncSpawnAggro IDA 0x140265F40 -> 0x140266339 - 仇恨生成
    void FuncSpawnAggro();

    // FuncDamageProcess IDA 0x14026A3D0 - 伤害处理
    void FuncDamageProcess();

    // CheckSkillCondition IDA 0x140269930 -> 0x140269B22
    bool CheckSkillCondition(unsigned int nSkillIndex, int nSkillGroup);

    // SetSpawnAggro IDA 0x140261D70
    void SetSpawnAggro(float fDistance, float fValue);

    // SetDmgAggressive IDA 0x140260D80 - 设置伤害激进度
    void SetDmgAggressive(int nDmgAggressive, float fCheckTime);

    // SetDmgAggroReset IDA 0x140260DC0 - 设置伤害仇恨重置
    void SetDmgAggroReset(float fTime, float fDist);

    // SetSkillGroupInfo IDA 0x140261650 - 设置技能组信息
    void SetSkillGroupInfo(unsigned int nGroupID, int nSortType, int nSkill1, int nSkill2, int nSkill3);

    // SetIdleMotionInfo IDA 0x1402613C0 - 设置空闲动作信息
    void SetIdleMotionInfo(int nChance, float fCheckTime);

    // SetEscapeInfo IDA 0x140261450 - 设置逃脱信息
    void SetEscapeInfo(float fMaxPoint, int nProbability, float fMinDist, float fMaxDist,
                       float fResetTime, std::uint8_t byEscapeType, const char* szEscapeVal);

    // AddEscapePoint IDA 0x14026A460 - 添加逃脱点数
    void AddEscapePoint(E_DAMAGE_TYPE eType);

    // IsGuardMonster IDA 0x140265A20 -> 0x140265ACB
    bool IsGuardMonster(CMover* pMover);

    // SetFuzzyScript IDA 0x1402656F0 - 设置模糊脚本
    void SetFuzzyScript(const char* szFilename);

    // CheckProtectState
    void CheckProtectState();

    // CheckFuzzyData
    int CheckFuzzyData(int iType, int nRate);

    // ChangeAiState
    void ChangeAiState(int nState);

    // GetConditionIntData
    int GetConditionIntData(int eVarName, int nValue);

    // GetConditionFloatData
    float GetConditionFloatData(int eVarName, int nValue);

    // GetAIFuzzyValue IDA 0x14026C360 - 获取AI模糊值
    void GetAIFuzzyValue(float* pfValue);

    // GetAIActionValue IDA 0x14026C400 - 获取AI动作值
    void GetAIActionValue(int* pnValue);

    // CheckSkillGroupCondition
    bool CheckSkillGroupCondition(unsigned int nSkillIndex, int nSkillGroup);

    // _ConditionHealth
    int _ConditionHealth(int nValue);

    // FuncStartState IDA 0x14026A850 -> 0x14026AB0F - 启动状态
    void FuncStartState();

    // FuncSearchTarget IDA 0x140265AD0 -> 0x140265F39 - 搜索目标
    void FuncSearchTarget();

    // FuncAttackSkill IDA 0x140268D80 -> 0x140269174 - 攻击技能
    bool FuncAttackSkill();

    // ClearTarget - 清除目标
    void ClearTarget();

    // CheckInitMaze - 检查初始化迷宫
    void CheckInitMaze();

    // CheckDelegateSkill - 检查代理技能
    void CheckDelegateSkill(unsigned int nSkillIndex);

    // GetDmgAggroReseTime IDA 0x140261D90 - 获取伤害仇恨重置时间
    float GetDmgAggroReseTime();

    // IsEnableClearTarget IDA 0x140261DA0 - 检查是否允许清除目标
    bool IsEnableClearTarget();

    // GetPathFindingFailedCount IDA 0x140354510 - 获取寻路失败计数
    int GetPathFindingFailedCount();

    // GetSkillDestPos IDA 0x140354530 - 获取技能目标位置
    hkvVec3 GetSkillDestPos();

    // IsPatrolMonster IDA 0x1403659C0 - 是否是巡逻怪物
    bool IsPatrolMonster();

    // === 状态相关函数 ===

    // Reset - 重置状态机
    void Reset();

    // SetState - 设置FSM状态
    void SetState(int nState);

    // GetState - 获取当前FSM状态
    int GetState() const;

    // EvaluateTransition - 评估状态转换
    int EvaluateTransition();

    // FuncCheckAggro - 检查仇恨值
    void FuncCheckAggro();

    // GetAggroCheckTime - 获取仇恨检查时间间隔
    float GetAggroCheckTime() const;

    // FuncCheckReturnPos IDA 0x14026A200 -> 0x14026A306 - 检查返回位置
    bool FuncCheckReturnPos();

    // IsProtectState IDA 0x14026B960 -> 0x14026B97B - 检查保护状态
    bool IsProtectState();

    // CheckStateLifeTime IDA 0x14026AB10 -> 0x14026AB56 - 检查状态生命周期
    void CheckStateLifeTime();

    // FuncFindEnemy IDA 0x14026B710 -> 0x14026B7E1 - 寻找敌人
    void FuncFindEnemy(float fElapsedTime);

    // FuncEscapeProcess - 处理逃跑逻辑
    void FuncEscapeProcess(float fElapsedTime);

    // StartAttackSkill IDA 0x14027E3A0 -> 0x14027E872 - 开始攻击技能
    void StartAttackSkill(int nSkillIndex);

    // FuncEndState - 结束状态
    void FuncEndState();

    // GetSkillIndex - 获取技能索引
    int GetSkillIndex(int nSkillIndex);

    // GetSuicideTime IDA 0x14019D210 - 获取自杀时间
    float GetSuicideTime();

    // GetTargetSightDistance IDA 0x14019D1F0 - 获取目标视野距离
    float GetTargetSightDistance();

    // SetEnalbeHelperWarp IDA 0x140091E70 - 设置助手传送启用
    void SetEnalbeHelperWarp(bool bEnable);

    // SetSuicideTime IDA 0x140260E00 - 设置自杀时间
    void SetSuicideTime(float fTime);

    // SetEnableClearTarget IDA 0x140260E20 - 设置是否允许清除目标
    void SetEnableClearTarget(int nEnable);

    // SetAiCheckTime IDA 0x140260B10 - 设置AI检查时间
    void SetAiCheckTime(float fTime);

    // SetPatrolMonster IDA 0x140260B30 - 设置巡逻怪物标志
    void SetPatrolMonster(int nCheck);

    // SetRunDistance IDA 0x140260C60 - 设置逃跑距离
    void SetRunDistance(float fDistance);

    // SetReturnDistance IDA 0x140260C80 - 设置返回距离
    void SetReturnDistance(float fDistance, int nHP);

    // SetTakeTargetInfo IDA 0x140260B60 - 设置目标获取信息
    void SetTakeTargetInfo(float fDistance);

    // SetMoveInfoToTarget IDA 0x140260B90 - 设置目标移动信息
    void SetMoveInfoToTarget(float fDelay, float fDistance);

    // SetRequestHelpInfo IDA 0x140260CC0 - 设置请求帮助信息
    void SetRequestHelpInfo(int nCount, float fDistance, int nMonsterID, int nMonsterCount);

    // SetFirstAttacker IDA 0x140260C10 - 设置首次攻击者
    void SetFirstAttacker(int nFirstAttacker, float fDelay);

    // SetRunawayInfo IDA 0x140260D20 - 设置逃跑信息
    void SetRunawayInfo(int nHP, int nCount, float fMinTime, float fMaxTime);

    // SetDelegateSkill IDA 0x140260E50 - 设置代理技能
    void SetDelegateSkill(const char* szSkillID, const char* szMonsterID);

    // RegisterStateFunctions IDA 0x140263160 - 注册状态函数
    void RegisterStateFunctions(int nState);

    // RegisterStateVars IDA 0x1402657D0 - 注册状态变量
    void RegisterStateVars(int _nState, int _nVariable, int nValue1, int nValue2);

    // AddDelegateTarget IDA 0x140260F20 - 添加代理目标
    void AddDelegateTarget(int nIndex, const char* szMobID1, const char* szMobID2, const char* szMobID3, const char* szMobID4, const char* szMobID5);

    // SetCommonAction IDA 0x140261400 - 设置通用动作
    void SetCommonAction(unsigned int nIndex, const char* szActionName);

    // CheckSkillGroupOrder IDA 0x140269CE0 - 检查技能组顺序
    void CheckSkillGroupOrder();

    // FuncCommonAction IDA 0x140269F40 - 执行通用动作
    void FuncCommonAction(int nActionIndex);

    // SetSkillGroupRate IDA 0x140261590 - 设置技能组比率
    void SetSkillGroupRate(int nSkillRate1, int nSkillRate2, int nSkillRate3, int nSkillRate4, int nSkillRate5,
                           int nSkillRate6, int nSkillRate7, int nSkillRate8, int nSkillRate9, int nSkillRate10);

    // SetReservedCondition IDA 0x140261750 - 设置保留条件
    void SetReservedCondition(unsigned int nIndex, unsigned int nVariable, const char* szConditionString, float fFloatData1, float fFloatData2);

    // SetDeathAction IDA 0x140261C00 - 设置死亡动作
    void SetDeathAction(const char* szActionName);

    // GetDeathActionMotion IDA 0x140261C30 - 获取死亡动作Motion
    std::int16_t GetDeathActionMotion();

    // SetRecoverySkill IDA 0x140261B20 - 设置恢复技能
    void SetRecoverySkill(const char* szTableID);

    // SetSuperArmorSkill IDA 0x140261B90 - 设置超级护甲技能
    void SetSuperArmorSkill(const char* szTableID);

    // SetProtectInfo IDA 0x140261DD0 - 设置保护信息
    void SetProtectInfo(float fEffectDist, float fTimeOut);

    // SetSkillCooltime IDA 0x140261F40 - 设置技能冷却时间
    void SetSkillCooltime(TB_SKILL* pSkillTable);

    // IsVarNeedCondition IDA 0x1402616F0 - 检查变量是否需要条件
    bool IsVarNeedCondition(int _nVariable);

    // CopyConditionData IDA 0x14025F8E0 - 复制条件数据
    void CopyConditionData(const CAi& Other);

    // CopyFullData IDA 0x14025FE10 - 复制完整AI数据
    void CopyFullData(const CAi& Other);

    // RegisterConditionsEx IDA 0x140263E10 - 注册扩展条件
    void RegisterConditionsEx(int _nState, int _nOutPutState, unsigned int _nVariable,
                              const char* _szConditionString, const char* _szData1,
                              const char* _szData2, int _nTransitionIndex);

    // RegisterSkillConditions IDA 0x140264460 - 注册技能条件
    void RegisterSkillConditions(int _nSkillIndex, unsigned int _nVariable,
                                 const char* _szConditionString, float _fFloatData1,
                                 float _fFloatData2, int _nSkillGroup);

    // RegisterActionAfterSkill IDA 0x140264910 - 注册技能后动作
    void RegisterActionAfterSkill(int _nSkillIndex, int _nNextState, unsigned int _nVariable,
                                  const char* _szConditionString, float _fFloatData1,
                                  float _fFloatData2, int _nSkillGroup);

    // === Patrol Functions ===
    // Patrol - 开始巡逻模式，设置路径点
    void Patrol();
    
    // CheckPatrol - 检查巡逻条件，获取下一个路径点
    bool CheckPatrol();
    
    // SetPatrolPoint - 从表格设置巡逻路径点
    void SetPatrolPoint(int nIndex, float fX, float fY, float fZ);

    // === Chase Functions ===
    // Chase - 开始追击目标
    void Chase();
    
    // CheckChase - 检查追击条件和范围
    bool CheckChase();
    
    // SetChaseTarget - 设置追击目标和参数
    void SetChaseTarget(std::uint32_t dwTargetID, float fRange, float fSpeed);

    // === Flee Functions ===
    // Flee - 开始逃跑
    void Flee();
    
    // CheckFlee - 检查逃跑条件和安全性
    bool CheckFlee();
    
    // SetFleePoint - 计算并设置逃跑目的地
    void SetFleePoint(float fDistance);

    // === Skill AI Functions ===
    // SelectSkill - 选择适合当前情况的技能
    int SelectSkill();
    
    // CheckSkillRange - 检查目标是否在技能范围内
    bool CheckSkillRange(int nSkillIndex);
    
    // ProcessSkillAI - 处理技能使用逻辑
    void ProcessSkillAI();
    
    // UseSkill - 使用指定技能
    bool UseSkill(int nSkillIndex);

    // === Group AI Functions ===
    // GroupAggro - 与组内成员共享仇恨
    void GroupAggro();
    
    // GroupTarget - 协调目标选择
    void GroupTarget();
    
    // GroupAction - 执行协调动作
    void GroupAction();
    
    // IsLeader - 检查是否为组长
    bool IsLeader() const;
    
    // === Owner/Target Functions ===
    // GetOwner - 获取所属怪物
    CMonster* GetOwner() const;
    
    // GetTarget - 获取当前目标
    std::uint32_t GetTarget() const;
    
    // SetTarget - 设置目标
    void SetTarget(std::uint32_t dwTargetID);
    
    // === Behavior Functions ===
    // Idle - 进入空闲状态
    void Idle();

    // === Movement Functions (IDA confirmed) ===
    // IsMoveRadius IDA 0x140266CF0 - 检查移动半径
    bool IsMoveRadius(hkvVec3& vTargetPos, float fRadius);

    // MoveToPos IDA 0x140268750 - 移动到指定位置
    void MoveToPos(hkvVec3& vMyPos, hkvVec3& vTargetPos);

    // FuncGazeMove IDA 0x140266340 - 注视移动
    E_PATHFIND_RESULT FuncGazeMove(CMover* _pTarget, float _fMinDist, float _fMaxDist,
                                    float _fMinMove, float _fMaxMove, E_MOVESIDE_TYPE _eMoveSide);

    // FuncRunWalkToMovePos IDA 0x140266F00 - 跑步行走到移动位置
    E_PATHFIND_RESULT FuncRunWalkToMovePos(CMover* _pTarget, float _fMinDistance, float _fMaxDistance,
                                            int _nMinAngle, int _nMaxAngle, unsigned char _byDirType,
                                            E_MOVESIDE_TYPE _eMoveSide);

    // FuncRunWalkToTargetPos IDA 0x1402678D0 - 跑步行走到目标位置
    E_PATHFIND_RESULT FuncRunWalkToTargetPos(CMover* _pTarget, float _fMinDistance, float _fMaxDistance,
                                              int _nMinAngle, int _nMaxAngle, unsigned char _byDirType,
                                              E_MOVESIDE_TYPE _eMoveSide);

    // FuncRunWalkToPos IDA 0x1402688B0 - 跑步行走到位置
    E_PATHFIND_RESULT FuncRunWalkToPos(hkvVec3& vMyPos, hkvVec3& vTargetPos,
                                        unsigned char byRunBit, int bCheckWayPoint);

    // === State Machine Functions (Round 8 Phase 3) ===
    
    // ChangeState - Change AI state (IDA confirmed)
    void ChangeState(int nNewState);
    
    // SearchTarget - Search for target (returns CMover*)
    CMover* SearchTarget();
    
    // ProcessSkillAttack - Process skill attack AI
    void ProcessSkillAttack();
    
    // FuncIdleProcess - Process idle state
    void FuncIdleProcess();
    
    // IsInAttackRange - Check if target in attack range
    bool IsInAttackRange();
    
    // IsInSightRange - Check if target in sight
    bool IsInSightRange();
    
    // IsLowHP - Check if HP below threshold
    bool IsLowHP();
    
    // HasValidTarget - Check if has valid attack target
    bool HasValidTarget();
    
    // CanUseSkill - Check if can use specific skill
    bool CanUseSkill(int nSkillID);

    // Destroy IDA 0x14025F240 - 销毁AI对象并清理所有资源
    void Destroy();

protected:
    // === IDA 确认的成员变量 ===

    // m_pMonster - 所属怪物
    CMonster* m_pMonster;

    // m_pStateMachine - 状态机 (CFsmClass<CAi>* 大小 0x58)
    CFsmClass<CAi>* m_pStateMachine;

    // m_pScriptInst - script instance pointer used by Destroy cleanup.
    void* m_pScriptInst;

    // m_nStatePreHP - 状态前HP
    int m_nStatePreHP;

    // m_nPreRandomValue - 预随机值 (IDA: used by _ConditionRandom, _ConditionRandomPrevalue)
    int m_nPreRandomValue;

    // m_arSkillTransition[10] - 技能转换数组
    CFsmTransition* m_arSkillTransition[10];

    // m_arSkillGroupTransition[10][10] - 技能组转换数组
    CFsmTransition* m_arSkillGroupTransition[10][10];

    // m_arActionAfterSkillTransition[10][10] - 技能后动作转换数组
    CFsmTransition* m_arActionAfterSkillTransition[10][10];

    // m_arCancelSkillTransition - 取消技能转换
    CFsmTransition* m_arCancelSkillTransition;

    // m_mapStateVars - 状态变量映射
    std::map<int, StateVarInfo> m_mapStateVars;

    // SelectAction 相关
    int m_nSelectActionCount;
    int m_arSelectActionRate[7];
    int m_arSelectActionResult[7];

    // Super Armor 相关
    int m_nSuperArmorSkillIndex;            // 超级装甲技能索引

    // Escape 相关
    float m_fEscapePoint;                   // 逃脱点数上限
    int m_nEscapePercent;                   // 逃脱概率
    float m_fEscapeSkillMinDist;            // 逃脱技能最小距离
    float m_fEscapeSkillMaxDist;            // 逃脱技能最大距离
    float m_fEscapePointResetTime;          // 逃脱点数重置时间
    std::uint8_t m_byEscapeType;            // 逃脱类型
    std::uint32_t m_dwEscapeValue;          // 逃脱值
    float m_fCurEscapePoint;                // 当前逃脱点数
    float m_fLastEscapePointTime;           // 最后逃脱点数时间
    E_ESCAPE_STATE m_eEscapeState;          // 逃脱状态

    // Runaway 相关
    int m_nRunawayHP;
    int m_nRunawayCurrentCount;
    int m_nRunawayMaxCount;
    float m_fRunawayDuration;             // 逃跑持续时间

    // Damage Aggressive 相关
    int m_nDmgAggressive;                 // 伤害激进度
    float m_fAggroCheckTime;              // 仇恨检查时间
    float m_fDmgAggroResetDist;           // 伤害仇恨重置距离
    float m_fDmgAggroResetTime;           // 伤害仇恨重置时间

    // Protect 相关
    E_PROTECT_STATE m_eProtectState;

    // Fuzzy 相关
    float m_fFuzzyCheckTime;
    float m_fFuzzyDelayTime;

    // SpawnAggro 相关
    float m_fSpawnAggroDistance;
    float m_fSpawnAggroValue;

    // 仇恨重置时间相关
    float m_fDmgAggroReseTime;            // 伤害仇恨重置时间
    bool m_bEnableClearTarget;            // 是否允许清除目标

    // 当前技能引用
    TB_SKILL* m_pCurSkillRef;

    // 条件函数指针类型
    typedef int (CAi::*ConditionIntFunc)(int);
    typedef float (CAi::*ConditionFloatFunc)(int);

    // 条件函数指针数组 (IDA: 0x39 = 57 个元素)
    ConditionIntFunc m_arConditionIntFuncs[0x39];
    ConditionFloatFunc m_arConditionFloatFuncs[0x39];

    // === FuncStartState 相关成员 ===

    // 状态时间相关
    float m_fStateTime;                    // 状态时间
    float m_fStateLifeTime;                // 状态生命周期
    float m_fStateTempFloat[2];            // 状态临时浮点数
    float m_fStateMoveDistMin;             // 状态移动距离最小值
    float m_fStateMoveDistMax;             // 状态移动距离最大值
    float m_fStateTargetDistMin;           // 状态目标距离最小值
    float m_fStateTargetDistMax;           // 状态目标距离最大值
    float m_fStateAngleMin;                // 状态角度最小值
    float m_fStateAngleMax;                // 状态角度最大值
    float m_fStateMoveDistSum;             // 状态移动距离总和

    // 状态移动起始位置 (hkvVec3 简化为 3 个 float)
    float m_vStateMoveStartPos[3];

    // === FuncSearchTarget 相关成员 ===

    // 目标视野距离
    float m_fTargetSightDistance;

    // === FuncAttackSkill 相关成员 ===

    // 前技能伤害计数
    int m_nPreSkillDamageCount;

    // 最后技能时间
    float m_fLastSkillTime;

    // === FuncCheckReturnPos 相关成员 ===
    float m_fReturnDistance;               // 返回距离

    // === FuncFindEnemy 相关成员 ===
    bool m_bPatrolMonster;                 // 是否是巡逻怪物
    float m_fSearchTargetTime;             // 搜索目标计时
    float m_fDelaySearchTarget;            // 搜索目标延迟

    // === StartAttackSkill 相关成员 ===
    bool m_bSetSkillGroup;                 // 是否设置技能组
    int m_nSelectGroupSkill;               // 选择的技能组索引
    int m_nSelectSkillIndex;               // 选择的技能索引
    bool m_bSkillActivate;                 // 技能是否激活
    float m_vGazeTargetPos[3];             // 注视目标位置
    float m_vSkillMoveDestPos[3];          // 技能移动目标位置

    // === Suicide 相关成员 ===
    float m_fSuicideTime;                  // 自杀时间

    // === Helper Warp 相关成员 ===
    bool m_bEnableHelperWarp;              // 是否启用助手传送

    // === AI Check Time 相关成员 ===
    float m_fAiCheckTime;                  // AI检查时间

    // === PathFinding 相关成员 ===
    int m_nPathFindingFailedTotalCount;    // 寻路失败总计数

    // === Run Distance 相关成员 ===
    float m_fRunDistance;                  // 逃跑距离

    // === Return HP 相关成员 ===
    int m_nReturnHP;                       // 返回HP

    // === Move Info To Target 相关成员 ===
    float m_fMoveDelayToTarget;            // 目标移动延迟
    float m_fMoveDistanceToTarget;         // 目标移动距离

    // === Request Help 相关成员 ===
    int m_nRequestHelpCnt;                 // 请求帮助计数
    float m_fRequestHelpDistance;          // 请求帮助距离
    int m_nRequestHelpMonsterID;           // 请求帮助怪物ID
    int m_nRequestHelpMonsterCount;        // 请求帮助怪物数量

    // === First Attacker 相关成员 ===
    bool m_bIsFirstAttacker;               // 是否首次攻击者

    // === Runaway 相关成员 ===
    float m_fRunwayMinTimeOut;             // 逃跑最小超时时间
    float m_fRunwayMaxTimeOut;             // 逃跑最大超时时间
    float m_fRunawayCheckTime;             // 逃跑检查时间

    // === Delegate Skill 相关成员 ===
    struct DelegateTarget {
        int nIndex;                           // 索引
        int nMobID1;                          // 怪物ID 1
        int nMobID2;                          // 怪物ID 2
        int nMobID3;                          // 怪物ID 3
        int nMobID4;                          // 怪物ID 4
        int nMobID5;                          // 怪物ID 5
    };

    struct {
        int nSkillID;                         // 代理技能ID
        int nDelegateMobID;                   // 代理怪物ID
        std::vector<DelegateTarget> vecTarget; // 代理目标列表
    } m_stDelegateSkill;

    // === State Data 相关成员 ===
    std::vector<int> m_vecStateData;       // 状态数据向量

    // === Skill Group 相关成员 ===
    int m_nCustomSkillID[10];              // 自定义技能ID (IDA offset 932)
    int m_nSkillGroupRatio[10];            // 技能组比率 (IDA offset 972)
    int m_nCurrentAction;                  // 当前动作索引 (IDA offset 1012)
    VString m_strCommonActions[10];        // 通用动作名称数组 (IDA offset 1032)
    int m_nSkillGroupID[10][3];            // 技能组ID (IDA offset 1012)
    int m_nSkillSortType[10];              // 技能排序类型 (IDA offset 1132)
    int m_nSkillOrder[10];                 // 技能顺序 (IDA offset 1172)
    VString m_strDeathAnim;                // 死亡动画名称 (IDA offset 1216)
    int m_nRecoverySkill;                  // 恢复技能ID (IDA offset 1224)

    // === Reserved Condition 相关成员 ===
    // m_arReservedCondition[20] - 保留条件数组 (CFsmCondition需要实现)

    // === Protect 相关成员 ===
    float m_fProtectEffectDist;           // 保护效果距离
    float m_fProtectWaitTimeOut;          // 保护等待超时
    float m_fProtectWaitTime;             // 保护等待时间

    // === Cooltime 相关成员 ===
    std::map<int, tagCOOLTIME> m_mapCooltimeList; // 冷却时间列表
    float m_fGlobalCooltime;              // 全局冷却时间

    // === Time 相关成员 ===
    float m_fSumElapsedTime;              // 累计经过时间
    float m_fActivateTime;                // 激活时间
    float m_fLastDamageTime;              // 最后伤害时间
    float m_fStateEndTime[43];            // 状态结束时间数组 (IDA: 43个元素)
    int m_nStateFailCount[43];            // 状态失败计数数组 (IDA: offset 0x10C0, 43个元素)
    float m_fRecoveryCheckTime;           // 恢复检查时间

    // === Patrol 相关成员 ===
    struct PatrolPoint {
        float fX;
        float fY;
        float fZ;
    };
    std::vector<PatrolPoint> m_vecPatrolPoints; // 巡逻路径点列表
    int m_nCurrentPatrolIndex;           // 当前巡逻点索引
    bool m_bPatrolForward;               // 巡逻方向（正向/反向）
    float m_fPatrolWaitTime;             // 巡逻等待时间

    // === Chase 相关成员 ===
    std::uint32_t m_dwChaseTargetID;     // 追击目标ID
    float m_fChaseRange;                 // 追击范围
    float m_fChaseSpeed;                 // 追击速度
    bool m_bChasing;                     // 是否正在追击

    // === Flee 相关成员 ===
    float m_vFleeDestPos[3];             // 逃跑目的地
    float m_fFleeSpeed;                  // 逃跑速度
    float m_fFleeSafetyDistance;         // 逃跑安全距离
    bool m_bFleeing;                     // 是否正在逃跑

    // === Escort 相关成员 ===
    bool m_bEscortMonster;               // 是否为护送怪物
    bool m_bStartEscort;                 // 是否已开始护送

    // === Move Fail 相关成员 ===
    std::uint8_t m_byFailMoveCount;      // 移动失败计数

    // === Helper 相关成员 ===
    float m_fHelperFarDistance;          // 助手远距离阈值
    float m_fHelperFarBattleDist;        // 助手战斗时远距离阈值
    int m_nCheckHelperFarCount;          // 检查助手远距离计数

    // === Position Check 相关成员 ===
    float m_fCheckValidPositionTime;     // 检查有效位置时间

    // === Skill AI 相关成员 ===
    int m_nSelectedSkillIndex;           // 已选择的技能索引
    float m_fSkillRangeMin;              // 技能最小范围
    float m_fSkillRangeMax;              // 技能最大范围

    // === Group AI 相关成员 ===
    int m_nGroupID;                      // 组ID
    std::uint32_t m_dwGroupTargetID;     // 组共享目标ID
    bool m_bGroupLeader;                 // 是否为组长

    // === Fuzzy Data 相关成员 (IDA 确认) ===
    struct FuzzyData {
        std::vector<void*> vConditions;  // 条件列表
        float fValue;                    // 模糊值
        int nType;                       // 类型

        // IDA 确认的构造函数和析构函数
        FuzzyData() : fValue(0.0f), nType(0) {}
        ~FuzzyData() {}
    };
    FuzzyData m_arFuzzy[4];              // 模糊数据数组 (4个)

    // === 条件函数指针向量 (IDA 确认) ===
    std::vector<ConditionIntFunc> m_xAiGetConditionDataIntFunc;
    std::vector<ConditionFloatFunc> m_xAiGetConditionDataFloatFunc;

    // === FSM Data 相关成员 (IDA 确认) ===
    std::vector<FsmData> m_vecFsmData;       // FSM 数据向量
    std::vector<FsmDataEx> m_vecFsmDataEx;   // FSM 扩展数据向量

    // === Fuzzy Script 相关成员 ===
    VString m_strFuzzyScript;            // 模糊脚本字符串

    // === Maze 相关成员 (IDA 确认) ===
    class XMaze* m_pMaze;                // 迷宫指针

    // === Reserved Condition 相关成员 (IDA 确认) ===
    CFsmCondition m_arReservedCondition[20]; // 保留条件数组

    // === Protected Member Functions ===
    // _CombineReservedConditions IDA 0x1402642F0 - 组合保留条件
    void _CombineReservedConditions(int _nState, int _nOutPutState,
                                    int nIndex1, int nIndex2, int nIndex3,
                                    int nIndex4, int nIndex5);

    // === AI Condition Functions - IDA 精确还原 ===
    // _ConditionIsTarget IDA 0x1402779F0 - 检查是否有目标
    int _ConditionIsTarget(int _nVal);

    // _ConditionTargetHealth IDA 0x140277AD0 - 获取目标HP百分比
    int _ConditionTargetHealth(int _nVal);

    // _ConditionGuardHealth IDA 0x140277B90 - 获取守护对象HP百分比
    int _ConditionGuardHealth(int _nVal);

    // _ConditionTargetNumber IDA 0x140277C70 - 获取目标周围同目标敌人数量
    int _ConditionTargetNumber(int _nVal);

    // _ConditionRandom IDA 0x140277EC0 - 获取随机值(0-9999)
    int _ConditionRandom(int _nVal);

    // _ConditionRandomPrevalue IDA 0x140277F00 - 获取上一次随机值
    int _ConditionRandomPrevalue(int _nVal);

    // _ConditionIsMoving IDA 0x140277F20 - 检查是否在移动
    int _ConditionIsMoving(int _nVal);

    // _ConditionIsAttack IDA 0x140277F80 - 检查是否在攻击状态
    int _ConditionIsAttack(int _nVal);

    // _ConditionIsChangeHP IDA 0x140277FE0 - 检查HP是否变化
    int _ConditionIsChangeHP(int _nVal);

    // _ConditionIsRequestHelpNum IDA 0x140278040 - 获取请求帮助次数
    int _ConditionIsRequestHelpNum(int _nVal);

    // _ConditionIsMonsterCount IDA 0x140278060 - 获取区域内怪物数量
    int _ConditionIsMonsterCount(int _nVal);

    // _ConditionIsSpawnMonsterCount IDA 0x1402780E0 - 获取召唤怪物数量
    int _ConditionIsSpawnMonsterCount(int _nVal);

    // _ConditionIsSectorMonsterCount IDA 0x1402782F0 - 获取扇区内怪物数量
    int _ConditionIsSectorMonsterCount(int _nVal);

    // _ConditionIsSectorMonsterCountByID IDA 0x140278350 - 获取扇区内指定ID怪物数量
    int _ConditionIsSectorMonsterCountByID(unsigned int _nVal);

    // _ConditionIsPreSkillDamageCount IDA 0x140278420 - 获取预技能伤害计数
    int _ConditionIsPreSkillDamageCount(int _nVal);

    // _ConditionIsTargetSkill IDA 0x140278440 - 检查目标是否正在攻击自己
    int _ConditionIsTargetSkill(int _nVal);

    // _ConditionIsTargetState IDA 0x1402784D0 - 获取目标状态
    int _ConditionIsTargetState(int _nVal);

    // _ConditionIsTargetDamageState IDA 0x140278560 - 获取目标受伤状态
    int _ConditionIsTargetDamageState(int _nVal);

    // _ConditionIsTargetMoveState IDA 0x140278610 - 获取目标移动状态
    int _ConditionIsTargetMoveState(int _nVal);

    // _ConditionIsTargetBuffIndex IDA 0x1402786B0 - 检查目标是否有指定Buff
    int _ConditionIsTargetBuffIndex(unsigned short _nVal);

    // _ConditionTargetAttacker IDA 0x140278730 - 获取攻击目标的敌人数量
    int _ConditionTargetAttacker(int _nVal);

    // _ConditionTargetCombo IDA 0x140278980 - 获取目标连击数
    int _ConditionTargetCombo(int _nVal);

    // _ConditionPatrolState IDA 0x1402789F0 - 获取巡逻状态
    int _ConditionPatrolState(int _nVal);

    // _ConditionStateFailCount IDA 0x140278A40 - 获取状态失败计数
    int _ConditionStateFailCount(unsigned int _nVal);

    // _ConditionFriendCount IDA 0x140278A70 - 获取范围内的友方数量
    int _ConditionFriendCount(int _nVal);

    // _ConditionEnemyCount IDA 0x140278D20 - 获取范围内的敌人数量
    int _ConditionEnemyCount(int _nVal);

    // _ConditionUserCount IDA 0x140278FB0 - 获取范围内的玩家数量
    int _ConditionUserCount(int _nVal);

    // _ConditionAggroCount IDA 0x140279200 - 获取仇恨列表大小
    int _ConditionAggroCount(int _nVal);

    // _ConditionHitCount IDA 0x140279240 - 获取被击中次数
    int _ConditionHitCount(int _nVal);

    // _ConditionGuardFriendCount IDA 0x140279270 - 获取守护对象周围的友方数量
    int _ConditionGuardFriendCount(int _nVal);

    // _ConditionGuardEnemyCount IDA 0x1402795B0 - 获取守护对象周围的敌人数量
    int _ConditionGuardEnemyCount(int _nVal);

    // === Float-returning Condition Functions ===

    // _ConditionTargetDistance IDA 0x1402798F0 - 获取与目标的距离
    float _ConditionTargetDistance(int _nVal);

    // _ConditionTargetDirection IDA 0x1402799A0 - 获取目标方向角度差
    float _ConditionTargetDirection(int _nVal);

    // _ConditionTargetLook IDA 0x140279AD0 - 获取目标注视角度差
    float _ConditionTargetLook(int _nVal);

    // _ConditionTargetDistanceCapsule IDA 0x140279C00 - 获取与目标的胶囊体距离
    float _ConditionTargetDistanceCapsule(int _nVal);

    // _ConditionStateTime IDA 0x140279CD0 - 获取状态时间
    float _ConditionStateTime(int _nVal);

    // _ConditionCreatePosDistance IDA 0x140279CF0 - 获取与创建位置的距离
    float _ConditionCreatePosDistance(int _nVal);

    // _ConditionLastSkillTime IDA 0x140279D60 - 获取最后技能时间
    float _ConditionLastSkillTime(int _nVal);

    // _ConditionLastDamageTime IDA 0x140279D80 - 获取最后伤害时间
    float _ConditionLastDamageTime(int _nVal);

    // _ConditionSpawnTime IDA 0x140279DA0 - 获取生成时间
    float _ConditionSpawnTime(int _nVal);

    // _ConditionActivateTime IDA 0x140279DE0 - 获取激活时间
    float _ConditionActivateTime(int _nVal);

    // _ConditionGuardDistance IDA 0x140279E00 - 获取守护对象距离
    float _ConditionGuardDistance(int _nVal);

    // _ConditionGuardDistanceCapsule IDA 0x140279EE0 - 获取守护对象胶囊体距离
    float _ConditionGuardDistanceCapsule(int _nVal);

    // _ConditionFuzzy IDA 0x14027A1C0 - 计算模糊条件值
    float _ConditionFuzzy(int _nVal);

    // CalcFuzzyValue IDA 0x14027A410 - 计算模糊值
    float CalcFuzzyValue(float fA, float fB, float fC);

    // _ConditionGroupCooltime IDA 0x14027A040 - 检查技能组冷却时间
    int _ConditionGroupCooltime(int _nVal);

    // _ConditionSkillCooltime IDA 0x14027A160 - 检查技能冷却时间
    bool _ConditionSkillCooltime(unsigned int nSkillID);

    // _ConditionStateEndTime IDA 0x14027A4F0 - 获取状态结束时间
    float _ConditionStateEndTime(int _nVal);

    // _ConditionMoveDistanceAfterSkill IDA 0x14027A570 - 获取技能后移动距离
    float _ConditionMoveDistanceAfterSkill(int _nVal);

    // _ConditionGlobalCooltime IDA 0x140279FE0 - 检查全局冷却时间
    int _ConditionGlobalCooltime(int _nVal);

    // GetCooltime - 获取冷却时间
    float GetCooltime(int nCooltimeGroup);

    // === AI State Functions ===
    // _StartWait IDA 0x14027A950 - 开始等待状态
    void _StartWait();

    // _UpdateWait IDA 0x14027AA10 - 更新等待状态
    void _UpdateWait(float fElapsedTime);

    // _StartPatrol IDA 0x14027AA60 - 开始巡逻状态
    void _StartPatrol();

    // _UpdatePatrol IDA 0x14027B0A0 - 更新巡逻状态
    void _UpdatePatrol(float fElapsedTime);

    // _StartSelectAction IDA 0x14027CB40 - 开始选择动作状态
    void _StartSelectAction();

    // _UpdateSelectAction IDA 0x14027CBE0 - 更新选择动作状态
    void _UpdateSelectAction(float fElapsedTime);

    // _StartBattleMove IDA 0x14027CFA0 - 开始战斗移动状态
    void _StartBattleMove();

    // _UpdateBattleMove IDA 0x14027D0A0 - 更新战斗移动状态
    void _UpdateBattleMove(float fElapsedTime);

    // _StartGaze IDA 0x14027D210 - 开始注视状态
    void _StartGaze();

    // _UpdateGaze IDA 0x14027D3A0 - 更新注视状态
    void _UpdateGaze(float fElapsedTime);

    // _EndGaze IDA 0x14027D3F0 - 结束注视状态
    void _EndGaze();

    // _StartMove IDA 0x14027D450 - 开始移动状态
    void _StartMove();

    // _UpdateMove IDA 0x14027D5D0 - 更新移动状态
    void _UpdateMove(float fElapsedTime);

    // _StartSucide IDA 0x14027D680 - 开始自杀状态
    void _StartSucide();

    // _StartReturn IDA 0x14027D730 - 开始返回状态
    void _StartReturn();

    // _UpdateReturn IDA 0x14027D7D0 - 更新返回状态
    void _UpdateReturn(float fElapsedTime);

    // _EndReturn IDA 0x14027DAA0 - 结束返回状态
    void _EndReturn();

    // _StartRecovery IDA 0x14027DB30 - 开始恢复状态
    void _StartRecovery();

    // _UpdateRecovery IDA 0x14027DCD0 - 更新恢复状态
    void _UpdateRecovery(float fElapsedTime);

    // _EndRecovery IDA 0x14027DE50 - 结束恢复状态
    void _EndRecovery();

    // _StartTraceMove IDA 0x14027E080 - 开始追踪移动状态
    void _StartTraceMove();

    // _UpdateTraceMove IDA 0x14027E0F0 - 更新追踪移动状态
    void _UpdateTraceMove(float fElapsedTime);

    // _StartAttack IDA 0x14027E370 - 开始攻击状态
    void _StartAttack();

    // _UpdateAttackSkill IDA 0x14027E9C0 - 更新攻击技能状态
    void _UpdateAttackSkill(float fElapsedTime);

    // _EndAttackSkill IDA 0x14027F440 - 结束攻击技能状态
    void _EndAttackSkill();

    // _StartCommonAction1~10 IDA 0x14027F5A0~0x14027F6DA - 开始通用动作
    void _StartCommonAction1();
    void _StartCommonAction2();
    void _StartCommonAction3();
    void _StartCommonAction4();
    void _StartCommonAction5();
    void _StartCommonAction6();
    void _StartCommonAction7();
    void _StartCommonAction8();
    void _StartCommonAction9();
    void _StartCommonAction10();

    // _UpdateCommonAction IDA 0x14027F6E0 - 更新通用动作状态
    void _UpdateCommonAction(float fElapsedTime);

    // _StartRequestHelp IDA 0x14027F7A0 - 开始请求帮助
    void _StartRequestHelp();

    // _StartRunaway IDA 0x14027FD60 - 开始逃跑
    void _StartRunaway();

    // _UpdateRunaway IDA 0x14027FF10 - 更新逃跑状态
    void _UpdateRunaway(float fElapsedTime);

    // _StartRequestSupport IDA 0x140280210 - 开始请求支援
    void _StartRequestSupport();

    // _UpdateRequestSupport IDA 0x140280300 - 更新请求支援状态
    void _UpdateRequestSupport(float fElapsedTime);

    // _UpdateProtection IDA 0x1402803A0 - 更新保护状态
    void _UpdateProtection(float fElapsedTime);

    // _StartProtectMove IDA 0x1402803C0 - 开始保护移动
    void _StartProtectMove();

    // _UpdateProtectGaze IDA 0x1402804F0 - 更新保护注视
    void _UpdateProtectGaze(float fElapsedTime);

    // _StartProtectGaze IDA 0x140280540 - 开始保护注视
    void _StartProtectGaze();

    // _StartProtectWait IDA 0x140280660 - 开始保护等待
    void _StartProtectWait();

    // _UpdateProtectWait IDA 0x140280680 - 更新保护等待
    void _UpdateProtectWait(float fElapsedTime);

    // ResetSelectSkillIndex IDA 0x140280B50 - 重置选择技能索引
    void ResetSelectSkillIndex();

    // CheckValidPositionByTime IDA 0x14027BED0 - 按时间检查有效位置
    int CheckValidPositionByTime(float fElapsedTime);

    // _EndProtectMove IDA 0x14027B8B0 - 结束保护移动
    void _EndProtectMove();

    // CheckPatrolAttack IDA 0x14027B900 - 检查巡逻攻击
    bool CheckPatrolAttack();

public:
    // ActiveSuperArmorSkill IDA 0x14027BA70 - 激活超级装甲技能
    void ActiveSuperArmorSkill();

    // CheckHelperFarDist IDA 0x14027BB10 - 检查助手远距离
    int CheckHelperFarDist(bool bBattle, bool bForce);

    // SetTargetSightDistance IDA 0x14027BE90 - 设置目标视野距离
    void SetTargetSightDistance(float fDistance);

    // WarpHelperToUser IDA 0x14027BD60 - 传送助手到用户
    void WarpHelperToUser(const hkvVec3& vPos);

    // CheckValidPositionByCount IDA 0x14027C0E0 - 按计数检查有效位置
    int CheckValidPositionByCount();

    // CheckEscortWayPoint IDA 0x14027C770 - 检查护送路径点
    void CheckEscortWayPoint();

    // EndEscortWayPoint IDA 0x14027C9D0 - 结束护送路径点
    void EndEscortWayPoint();

    bool IsEscortMonster() const { return m_bEscortMonster; }

    // === Protected Helper Functions ===
    // RandomBetweenF - 生成两个值之间的随机浮点数
    float RandomBetweenF(float fMin, float fMax);

};

// AI 行为类型枚举
enum E_AI_ACTION_TYPE {
    AI_ACTION_NONE = 0,
    AI_ACTION_IDLE = 1,
    AI_ACTION_PATROL = 2,
    AI_ACTION_CHASE = 3,
    AI_ACTION_ATTACK = 4,
    AI_ACTION_SKILL = 5,
    AI_ACTION_FLEE = 6,
    AI_ACTION_DEAD = 7
};

// AI 状态枚举 (从 SelectActionToState 映射)
enum E_AI_STATE {
    AI_STATE_IDLE = 4,
    AI_STATE_ATTACK = 5,
    AI_STATE_SKILL = 6,
    AI_STATE_SPECIAL = 7,
    AI_STATE_WAIT = 8,
    AI_STATE_PATROL = 29,
    AI_STATE_CHASE = 32,      // 追击状态
    AI_STATE_RUNAWAY = 35,
    AI_STATE_FLEE = 35,       // 逃跑状态 (同 RUNAWAY)
    AI_STATE_PROTECTION = 39,
    AI_STATE_DEFAULT = 36
};
