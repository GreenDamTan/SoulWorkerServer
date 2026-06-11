// HiddenEvent.h
// CHiddenEvent - Hidden event class for event management
// IDA: ?UpdateCondition@CHiddenEvent@@QEAA_NHKK@Z (0x1402a9dd0)
// IDA: ?CheckResult@CHiddenEvent@@QEAA_NXZ (0x1402aa3d0)

#pragma once

#include "GameWorldMode.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_EVENT_CONDITION.h"
#include <cstdint>

// Forward declarations
struct TB_EVENT_CONDITION;

// CHiddenEvent - Hidden event management class
class CHiddenEvent : public CGameWorldMode {
public:
    CHiddenEvent();
    virtual ~CHiddenEvent();

    // IDA: ?UpdateCondition@CHiddenEvent@@QEAA_NHKK@Z (0x1402a9dd0)
    // Updates the event condition based on target type and value
    bool UpdateCondition(char nTarget, unsigned int dwObject, int dwValue);

    // IDA: ?CheckResult@CHiddenEvent@@QEAA_NXZ (0x1402aa3d0)
    // Checks the event result - implemented in .cpp
    bool CheckResult();

    // IDA: ?GetHiddenEventState@CHiddenEvent@@QEAAHXZ
    int GetHiddenEventState() const { return m_nEventState; }

    // IDA: ?SetHiddenEventState@CHiddenEvent@@QEAAXH@Z
    void SetHiddenEventState(int nState) { m_nEventState = nState; }

    // IDA: ?AddConditionCnt@CHiddenEvent@@QEAAXH@Z
    void AddConditionCnt(int nValue) { m_nConditionCnt += nValue; }

    // IDA: ?SetConditionCnt@CHiddenEvent@@QEAAXH@Z
    void SetConditionCnt(int nValue) { m_nConditionCnt = nValue; }

    // IDA: ?GetConditionCnt@CHiddenEvent@@QEAAHXZ
    int GetConditionCnt() const { return m_nConditionCnt; }

    // IDA: ?Start@CHiddenEvent@@QEAAXXZ (0x1402a9da0)
    void Start() {
        m_nEventState = 1;
        m_nConditionCnt = 0;
    }

    // Accessors
    TB_EVENT_CONDITION* GetEventCondition() const { return m_pTB_Event_Condition; }
    void SetEventCondition(TB_EVENT_CONDITION* pCondition) { m_pTB_Event_Condition = pCondition; }

private:
    TB_EVENT_CONDITION* m_pTB_Event_Condition;
    int m_nEventState;
    int m_nConditionCnt;
};
