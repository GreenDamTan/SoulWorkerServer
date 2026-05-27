#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"  // for tagCOOLTIME

// 前置声明
class CMonster;
class CMoverEx;
class CMover;
struct TB_SKILL;
class CFsmTransition;
class CFsmClass;

// ============================================================================
// E_FSMDATATYPE - FSM 数据类型枚举
// ============================================================================
enum E_FSMDATATYPE {
    FSMDTYPE_INT = 0,
    FSMDTYPE_FLOAT = 1,
    FSMDTYPE_RANDOMINT = 2,
    FSMDTYPE_RANDOMFLOAT = 3
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
// StateVarInfo - 状态变量信息
// ============================================================================
struct StateVarInfo {
    int nValue;
    int nMaxValue;
    int nDefaultValue;
    int nReserved;
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

    // CheckSkillCondition IDA 0x140269930 -> 0x140269B22
    bool CheckSkillCondition(unsigned int nSkillIndex, int nSkillGroup);

    // SetSpawnAggro IDA 0x140261D70
    void SetSpawnAggro(float fDistance, float fValue);

    // IsGuardMonster IDA 0x140265A20 -> 0x140265ACB
    bool IsGuardMonster(CMover* pMover);

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

    // === 状态相关函数 ===

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

    // AddDelegateTarget IDA 0x140260F20 - 添加代理目标
    void AddDelegateTarget(int nIndex, const char* szMobID1, const char* szMobID2, const char* szMobID3, const char* szMobID4, const char* szMobID5);

    // SetCommonAction IDA 0x140261400 - 设置通用动作
    void SetCommonAction(unsigned int nIndex, const char* szActionName);

    // SetSkillGroupRate IDA 0x140261590 - 设置技能组比率
    void SetSkillGroupRate(int nSkillRate1, int nSkillRate2, int nSkillRate3, int nSkillRate4, int nSkillRate5,
                           int nSkillRate6, int nSkillRate7, int nSkillRate8, int nSkillRate9, int nSkillRate10);

    // SetReservedCondition IDA 0x140261750 - 设置保留条件
    void SetReservedCondition(unsigned int nIndex, unsigned int nVariable, const char* szConditionString, float fFloatData1, float fFloatData2);

    // SetDeathAction IDA 0x140261C00 - 设置死亡动作
    void SetDeathAction(const char* szActionName);

    // SetProtectInfo IDA 0x140261DD0 - 设置保护信息
    void SetProtectInfo(float fEffectDist, float fTimeOut);

    // SetSkillCooltime IDA 0x140261F40 - 设置技能冷却时间
    void SetSkillCooltime(/* TB_SKILL* pSkillTable */);

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

protected:
    // === IDA 确认的成员变量 ===

    // m_pMonster - 所属怪物
    CMonster* m_pMonster;

    // m_pStateMachine - 状态机
    CFsmClass* m_pStateMachine;

    // m_nStatePreHP - 状态前HP
    int m_nStatePreHP;

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

    // Runaway 相关
    int m_nRunawayHP;
    int m_nRunawayCurrentCount;
    int m_nRunawayMaxCount;

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
    int m_nSkillGroupRatio[10];            // 技能组比率

    // === Reserved Condition 相关成员 ===
    // m_arReservedCondition[20] - 保留条件数组 (CFsmCondition需要实现)

    // === Protect 相关成员 ===
    float m_fProtectEffectDist;           // 保护效果距离
    float m_fProtectWaitTimeOut;          // 保护等待超时

    // === Cooltime 相关成员 ===
    std::map<int, tagCOOLTIME> m_mapCooltimeList; // 冷却时间列表
    float m_fGlobalCooltime;              // 全局冷却时间

    // === Time 相关成员 ===
    float m_fSumElapsedTime;              // 累计经过时间
    float m_fActivateTime;                // 激活时间
    float m_fLastDamageTime;              // 最后伤害时间

    // === Protected Member Functions ===
    // _CombineReservedConditions IDA 0x1402642F0 - 组合保留条件
    void _CombineReservedConditions(int _nState, int _nOutPutState,
                                    int nIndex1, int nIndex2, int nIndex3,
                                    int nIndex4, int nIndex5);

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
    AI_STATE_RUNAWAY = 35,
    AI_STATE_PROTECTION = 39,
    AI_STATE_DEFAULT = 36
};
