// HiddenEvent.cpp
// CHiddenEvent - Hidden event class implementation
// IDA: ?UpdateCondition@CHiddenEvent@@QEAA_NHKK@Z (0x1402a9dd0)

#include "HiddenEvent.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <map>

CHiddenEvent::CHiddenEvent() {
    Clear();
}

CHiddenEvent::~CHiddenEvent() {
    Clear();
}

void CHiddenEvent::Clear() {
    m_pTB_Hidden_Event = nullptr;
    m_pTB_Event_Condition = nullptr;
    SetHiddenEventState(0);
    SetConditionCnt(0);
    m_dwReward_Item = 0;
    m_byReward_Count = 0;
}

bool CHiddenEvent::Init(TB_HIDDEN_EVENT* pTB_Hidden_Event, TB_EVENT_CONDITION* pTB_Event_Condition) {
    if (!pTB_Hidden_Event || !pTB_Event_Condition) {
        return false;
    }
    m_pTB_Hidden_Event = pTB_Hidden_Event;
    m_pTB_Event_Condition = pTB_Event_Condition;
    SetHiddenEventState(0);
    return true;
}

bool CHiddenEvent::SelectEventCondition(int nEventBoxID) {
    // TODO: 需人工审查：重复条件指针和零权重时的随机选择尚未做运行核对。
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_HIDDEN_EVENT* pTB_Hidden_Event = pServer->GetResourceMgr().GetTB_HIDDEN_EVENT(
        static_cast<std::uint16_t>(nEventBoxID));
    if (!pTB_Hidden_Event) {
        return false;
    }

    std::map<TB_EVENT_CONDITION*, std::uint16_t> mapEvent;
    unsigned int dwMaxRate = 0;
    for (int i = 0; i < 5; ++i) {
        TB_EVENT_CONDITION* pTB = TXSingleton<XGameServer>::Instance()->GetResourceMgr().GetTB_EVENT_CONDITION(
            pTB_Hidden_Event->uniEvent_Condition_ID[i]);
        if (pTB) {
            dwMaxRate += pTB_Hidden_Event->uniCondition_Select_Rate[i];
            mapEvent.insert({pTB, pTB_Hidden_Event->uniCondition_Select_Rate[i]});
        }
    }

    TB_EVENT_CONDITION* pTB_Event_Condition = nullptr;
    int nRandom = TXSingleton<XGameServer>::Instance()->nRand(1, dwMaxRate);
    int nCal = 0;
    for (const auto& event : mapEvent) {
        nCal += event.second;
        if (nRandom <= nCal) {
            pTB_Event_Condition = event.first;
            break;
        }
    }
    mapEvent.clear();
    if (pTB_Event_Condition) {
        Init(pTB_Hidden_Event, pTB_Event_Condition);
        return true;
    }
    return false;
}

void CHiddenEvent::Start() {
    SetHiddenEventState(1);
    SetConditionCnt(0);
}

void CHiddenEvent::SetHiddenEventState(int nState) {
    m_nState = nState;
}

std::uint8_t CHiddenEvent::GetHiddenEventState() {
    return static_cast<std::uint8_t>(m_nState);
}

int CHiddenEvent::GetConditionCnt() {
    // PDB 中与 CGameWorldMode::GetModeDateID 共用同一段机器码；IDA 读取对象偏移 +0x1C。
    return m_nConditionCnt;
}

void CHiddenEvent::SetConditionCnt(int nValue) {
    m_nConditionCnt = nValue;
}

void CHiddenEvent::AddConditionCnt(int nValue) {
    m_nConditionCnt += nValue;
}

// IDA: ?UpdateCondition@CHiddenEvent@@QEAA_NHKK@Z (0x1402a9dd0)
// Updates the event condition based on target type and value
bool CHiddenEvent::UpdateCondition(int nTarget, unsigned int dwObject, unsigned int dwValue) {
    // Check event state must be 2 (running)
    if (GetHiddenEventState() != 2) {
        return false;
    }

    // Check if condition table exists
    if (!m_pTB_Event_Condition) {
        return false;
    }

    // Check target type matches
    if (m_pTB_Event_Condition->Target_Type != nTarget) {
        return false;
    }

    bool bChange = false;
    unsigned char conditionType = m_pTB_Event_Condition->Condition_Type;

    switch (conditionType) {
        case 1: // Count-based condition
            if (m_pTB_Event_Condition->Target_ID && m_pTB_Event_Condition->Target_ID != dwObject) {
                return false;
            }
            AddConditionCnt(dwValue);
            bChange = true;
            if (static_cast<int>(m_pTB_Event_Condition->Finish_Count) <= GetConditionCnt()) {
                SetHiddenEventState(3);
            }
            break;

        case 2: // Value-based condition
            if (m_pTB_Event_Condition->Target_Type == 7) {
                if (GetConditionCnt() < static_cast<int>(dwValue)) {
                    SetConditionCnt(static_cast<int>(dwValue));
                }
            } else {
                AddConditionCnt(dwValue);
            }
            bChange = true;
            if (static_cast<int>(m_pTB_Event_Condition->Finish_Count) <= GetConditionCnt()) {
                SetHiddenEventState(3);
            }
            break;

        case 3: // Item-based condition
            if (m_pTB_Event_Condition->Target_Type == 10) {
                if (m_pTB_Event_Condition->Target_ID) {
                    if (m_pTB_Event_Condition->Target_ID != dwObject) {
                        return false;
                    }
                } else {
                    // Check item type
                    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                    TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(dwObject);
                    if (!pTBItem) {
                        return false;
                    }
                    TB_ITEM_CLASSIFY* pTBClassify = pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTBItem->Item_Classify_Index);
                    if (!pTBClassify) {
                        return false;
                    }
                    if (pTBClassify->Item_Use_Type != 11) {
                        return false;
                    }
                }
            }
            AddConditionCnt(dwValue);
            bChange = true;
            if (static_cast<int>(m_pTB_Event_Condition->Finish_Count) <= GetConditionCnt()) {
                SetHiddenEventState(3);
            }
            break;

        default:
            if (conditionType > 3 && conditionType <= 5) {
                if (m_pTB_Event_Condition->Target_Type == 10) {
                    if (m_pTB_Event_Condition->Target_ID) {
                        if (m_pTB_Event_Condition->Target_ID != dwObject) {
                            return false;
                        }
                    } else {
                        // Check item type
                        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                        TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(dwObject);
                        if (!pTBItem) {
                            return false;
                        }
                        TB_ITEM_CLASSIFY* pTBClassify = pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTBItem->Item_Classify_Index);
                        if (!pTBClassify) {
                            return false;
                        }
                        if (pTBClassify->Item_Use_Type != 11) {
                            return false;
                        }
                    }
                }
                AddConditionCnt(dwValue);
                bChange = true;

                if (m_pTB_Event_Condition->Condition_Type == 4) {
                    if (static_cast<int>(m_pTB_Event_Condition->Finish_Count) <= GetConditionCnt()) {
                        SetHiddenEventState(3);
                    }
                } else if (m_pTB_Event_Condition->Condition_Type == 5) {
                    if (static_cast<int>(m_pTB_Event_Condition->Finish_Count) < GetConditionCnt()) {
                        SetHiddenEventState(4);
                    }
                }
            }
            break;
    }

    return bChange;
}

bool CHiddenEvent::SetRewardItem() {
    if (!m_pTB_Hidden_Event || !m_pTB_Event_Condition) {
        return false;
    }
    if (GetHiddenEventState() != 3) {
        return false;
    }
    m_dwReward_Item = m_pTB_Hidden_Event->Reward_Item;
    m_byReward_Count = m_pTB_Hidden_Event->Reward_Count;
    return true;
}

bool CHiddenEvent::GetRewardItem(std::uint16_t& wHiddenID, unsigned int& nItemID, std::int16_t& shCount) {
    wHiddenID = 0;
    nItemID = 0;
    shCount = 0;
    if (!m_pTB_Hidden_Event || !m_pTB_Event_Condition) {
        return false;
    }
    if (GetHiddenEventState() != 3) {
        return false;
    }
    nItemID = m_dwReward_Item;
    shCount = m_byReward_Count;
    wHiddenID = m_pTB_Hidden_Event->Hidden_Event_ID;
    return true;
}

std::uint16_t CHiddenEvent::GetHiddenEventID() {
    if (m_pTB_Hidden_Event) {
        return m_pTB_Hidden_Event->Hidden_Event_ID;
    }
    return 0;
}

unsigned int CHiddenEvent::GetEventConditionID() {
    if (m_pTB_Event_Condition) {
        return m_pTB_Event_Condition->Event_Condition_ID;
    }
    return 0;
}

// IDA: ?CheckResult@CHiddenEvent@@QEAA_NXZ (0x1402aa3d0)
// Checks the event result
bool CHiddenEvent::CheckResult() {
    if (!m_pTB_Event_Condition) {
        return false;
    }

    // Check condition type 5 (special condition)
    if (m_pTB_Event_Condition->Condition_Type == 5) {
        if (GetHiddenEventState() == 2) {
            SetHiddenEventState(3);
            return true;
        }
    } else {
        if (GetHiddenEventState() == 2) {
            SetHiddenEventState(4);
            return true;
        }
    }
    return false;
}
