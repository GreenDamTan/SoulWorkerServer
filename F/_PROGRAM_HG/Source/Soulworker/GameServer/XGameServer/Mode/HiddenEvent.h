// HiddenEvent.h
// CHiddenEvent - Hidden event class for event management
// IDA: ?UpdateCondition@CHiddenEvent@@QEAA_NHKK@Z (0x1402a9dd0)
// IDA: ?CheckResult@CHiddenEvent@@QEAA_NXZ (0x1402aa3d0)

#pragma once

#include <cstdint>

struct TB_HIDDEN_EVENT;
struct TB_EVENT_CONDITION;

// CHiddenEvent - Hidden event management class
class CHiddenEvent {
public:
    CHiddenEvent();
    virtual ~CHiddenEvent();

    void Clear();
    bool Init(TB_HIDDEN_EVENT* pTB_Hidden_Event, TB_EVENT_CONDITION* pTB_Event_Condition);
    bool SelectEventCondition(int nEventBoxID);

    // IDA: ?Start@CHiddenEvent@@QEAAXXZ (0x1402a9da0)
    void Start();

    // IDA: ?UpdateCondition@CHiddenEvent@@QEAA_NHKK@Z (0x1402a9dd0)
    // Updates the event condition based on target type and value
    bool UpdateCondition(int nTarget, unsigned int dwObject, unsigned int dwValue);

    // IDA: ?SetHiddenEventState@CHiddenEvent@@QEAAXH@Z
    void SetHiddenEventState(int nState);

    // IDA: ?GetHiddenEventState@CHiddenEvent@@QEAAEXZ (0x1402aa710)
    std::uint8_t GetHiddenEventState();

    bool SetRewardItem();
    bool GetRewardItem(std::uint16_t& wHiddenID, unsigned int& nItemID, std::int16_t& shCount);
    std::uint16_t GetHiddenEventID();
    unsigned int GetEventConditionID();

    // IDA: ?CheckResult@CHiddenEvent@@QEAA_NXZ (0x1402aa3d0)
    // Checks the event result - implemented in .cpp
    bool CheckResult();

    int GetConditionCnt();

    // IDA: ?SetConditionCnt@CHiddenEvent@@QEAAXH@Z
    void SetConditionCnt(int nValue);

    // IDA: ?AddConditionCnt@CHiddenEvent@@QEAAXH@Z
    void AddConditionCnt(int nValue);

private:
    TB_HIDDEN_EVENT* m_pTB_Hidden_Event;
    TB_EVENT_CONDITION* m_pTB_Event_Condition;
    int m_nState;
    int m_nConditionCnt;
    unsigned int m_dwReward_Item;
    std::uint8_t m_byReward_Count;
};

static_assert(sizeof(CHiddenEvent) == 40, "CHiddenEvent size must match GameServer PDB");
