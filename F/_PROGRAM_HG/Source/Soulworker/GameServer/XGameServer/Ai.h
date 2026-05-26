#pragma once

#include <cstdint>
#include <map>
#include <vector>

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

    // 当前技能引用
    TB_SKILL* m_pCurSkillRef;

    // 条件函数指针数组 (简化)
    void* m_arConditionIntFuncs[64];
    void* m_arConditionFloatFuncs[64];
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
