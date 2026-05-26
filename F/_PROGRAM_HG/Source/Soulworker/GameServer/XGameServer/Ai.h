#pragma once

#include <cstdint>

// 前置声明
class CMonster;

// CAi - AI控制器类
// IDA 确认的 AI 行为控制类
class CAi {
public:
    CAi();
    virtual ~CAi();

    // AI 行为选择
    virtual void Update(float fDeltaTime);
    virtual int SelectAction();  // IDA 0x14026ADF0
    virtual void FindTargetBySkill();  // IDA 0x14027CAA0
    virtual void FuncSpawnAggro();  // IDA 0x140265F40

    // 设置所属怪物
    void SetOwner(CMonster* pOwner);

protected:
    CMonster* m_pOwner;
    float m_fUpdateTimer;
    int m_nCurrentAction;
    int m_nTargetID;
    float m_fActionTimer;
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
