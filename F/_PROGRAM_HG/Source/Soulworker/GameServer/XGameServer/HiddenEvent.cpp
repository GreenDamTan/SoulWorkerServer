// HiddenEvent.cpp
// CHiddenEvent - Hidden event class implementation
// IDA: ?UpdateCondition@CHiddenEvent@@QEAA_NHKK@Z (0x1402a9dd0)

#include "HiddenEvent.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

CHiddenEvent::CHiddenEvent()
    : CGameWorldMode()
    , m_pTB_Event_Condition(nullptr)
    , m_nEventState(0)
    , m_nConditionCnt(0)
{
}

CHiddenEvent::~CHiddenEvent() {
    m_pTB_Event_Condition = nullptr;
}

// IDA: ?UpdateCondition@CHiddenEvent@@QEAA_NHKK@Z (0x1402a9dd0)
// Updates the event condition based on target type and value
bool CHiddenEvent::UpdateCondition(char nTarget, unsigned int dwObject, int dwValue) {
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
                if (GetConditionCnt() < dwValue) {
                    SetConditionCnt(dwValue);
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
