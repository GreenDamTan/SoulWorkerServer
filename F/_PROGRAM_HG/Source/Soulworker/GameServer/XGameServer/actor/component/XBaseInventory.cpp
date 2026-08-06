#include "XBaseInventory.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/XItemFactory.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

#include <cstdlib>

// ============================================================================
// XBaseEquip Implementation
// ============================================================================

// IDA: 0x140308780
XBaseEquip::XBaseEquip()
    : m_pItem{}
    , m_bLock{}
    , m_mapSetItem()
    , m_byType(0)
{
}

// IDA: 0x1402FE2A0
std::shared_ptr<CItem> XBaseEquip::GetItem(std::int64_t biSerial) {
    for (int i = 0; i < 20; ++i) {
        if (m_pItem[i] && m_pItem[i]->GetSerial() == biSerial) {
            return m_pItem[i];
        }
    }
    return nullptr;
}

// IDA: 0x1402FE500
std::shared_ptr<CItem> XBaseEquip::GetItem(std::uint32_t dwItemID) {
    for (int i = 0; i < 20; ++i) {
        if (m_pItem[i] && m_pItem[i]->GetID() == static_cast<int>(dwItemID)) {
            return m_pItem[i];
        }
    }
    return nullptr;
}

// IDA: 0x1402FE440. Only costume-use items contribute broach state.
void XBaseEquip::GetBroachList(PS_ITEM_BROACH_LIST& stBroachList) {
    for (int i = 0; i < 20; ++i) {
        if (m_pItem[i] && m_pItem[i]->GetClassifyTable()->Item_Use_Type == 1) {
            m_pItem[i]->GetBroachList(stBroachList);
        }
    }
}

// IDA: 0x1402FC030
bool XBaseEquip::Equip(int nSlot, std::shared_ptr<CItem> pItem) {
    m_pItem[nSlot] = std::move(pItem);
    if (m_pItem[nSlot]) {
        m_pItem[nSlot]->SetSlot(nSlot);
        m_pItem[nSlot]->SetInvenType(m_byType);
    }
    return true;
}

// IDA: 0x1402FC130
bool XBaseEquip::AddItem(std::int16_t shSlot, std::shared_ptr<CItem> pItem) {
    m_pItem[shSlot] = std::move(pItem);
    if (m_pItem[shSlot]) {
        m_pItem[shSlot]->SetSlot(shSlot);
        m_pItem[shSlot]->SetInvenType(m_byType);
    }
    return true;
}

// IDA: 0x140301640
XShapeEquip::XShapeEquip()
    : XBaseEquip()
{
    m_byType = 0;
}

// IDA: 0x1403019B0
XAbilityEquip::XAbilityEquip()
    : XBaseEquip()
{
    m_byType = 1;
}

// IDA: 0x140301C60
XLookEquip::XLookEquip()
    : XBaseEquip()
{
    m_byType = 3;
}

// IDA: 0x140301830
void XShapeEquip::GetCancelSlot(
    int nSlotType,
    unsigned int nCancelBit,
    std::vector<stEMPTYSLOT>& vecCancelSlot) {
    for (int i = 0; i < 14; ++i) {
        const int nBit = 1 << (14 - i - 1);
        (void)nBit;

        if (!m_pItem[i] || GetLock(static_cast<std::int16_t>(i)) ||
            nSlotType - 11 == i) {
            continue;
        }

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(
            static_cast<unsigned int>(m_pItem[i]->GetID()));
        char* pEnd = nullptr;
        const unsigned int nTempCancelBit = static_cast<unsigned int>(
            std::strtoul(pTBItem->Item_Slot_Disable, &pEnd, 2));
        (void)pEnd;
        if ((nCancelBit & nTempCancelBit) == 0) {
            continue;
        }

        stEMPTYSLOT stCancelSlot{};
        stCancelSlot.byInvenType = 0;
        stCancelSlot.shSlotPos = static_cast<std::int16_t>(i);
        vecCancelSlot.push_back(stCancelSlot);
    }
}

// IDA: 0x140301F10
void XLookEquip::GetCancelSlot(
    int nSlotType,
    unsigned int nCancelBit,
    std::vector<stEMPTYSLOT>& vecCancelSlot) {
    for (int i = 0; i < 14; ++i) {
        const int nBit = 1 << (14 - i - 1);
        (void)nBit;

        if (!m_pItem[i] || GetLock(static_cast<std::int16_t>(i)) ||
            nSlotType - 11 == i) {
            continue;
        }

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(
            static_cast<unsigned int>(m_pItem[i]->GetID()));
        if (!pTBItem) {
            continue;
        }

        char* pEnd = nullptr;
        const unsigned int nTempCancelBit = static_cast<unsigned int>(
            std::strtoul(pTBItem->Item_Slot_Disable, &pEnd, 2));
        (void)pEnd;
        if ((nCancelBit & nTempCancelBit) == 0) {
            continue;
        }

        stEMPTYSLOT stCancelSlot{};
        stCancelSlot.byInvenType = 3;
        stCancelSlot.shSlotPos = static_cast<std::int16_t>(i);
        vecCancelSlot.push_back(stCancelSlot);
    }
}

// ============================================================================
// XBaseInventory Implementation
// ============================================================================

XBaseInventory::XBaseInventory() {
    // IDA 0x140308610: Initialize item array and multi_index container
    // Stub implementation - just initialize arrays
    for (int i = 0; i < 400; ++i) {
        m_pItem[i] = nullptr;
        m_bLock[i] = 0;
    }
}

XBaseInventory::~XBaseInventory() {
    // IDA 0x140307B20: Destructor
    for (int i = 0; i < 400; ++i) {
        m_pItem[i].reset();
    }
}

std::int16_t XBaseInventory::GetEmptySlot() {
    // IDA 0x1402FEEB0: Find first empty unlocked slot
    for (int i = 0; i < m_shOpenSlot; ++i) {
        if (!m_pItem[i] && !m_bLock[i]) {
            return static_cast<std::int16_t>(i);
        }
    }
    return -1;  // No empty slot found
}

// IDA: 0x1402FEF30
void XBaseInventory::GetEmptySlot(
    int nNeedCount,
    std::vector<stEMPTYSLOT>& vecEmptySlot) {
    if (nNeedCount <= 0) {
        return;
    }

    for (int i = 0; i < m_shOpenSlot; ++i) {
        if (!m_pItem[i] && !m_bLock[i]) {
            if (vecEmptySlot.size() == static_cast<std::size_t>(nNeedCount)) {
                return;
            }

            stEMPTYSLOT stEmptySlot;
            stEmptySlot.byInvenType = m_byType;
            stEmptySlot.shSlotPos = static_cast<std::int16_t>(i);
            vecEmptySlot.push_back(stEmptySlot);
        }
    }
}

// IDA: 0x1402FEE00
bool XBaseInventory::CheckEmptySlotCount(std::uint16_t wNeedCount) {
    if (wNeedCount == 0) {
        return false;
    }

    std::uint16_t wCount = 0;
    for (int i = 0; i < m_shOpenSlot; ++i) {
        if (!m_pItem[i] && !m_bLock[i] &&
            wNeedCount == ++wCount) {
            return true;
        }
    }

    return false;
}

std::int16_t XBaseInventory::GetEmptySlotCount() {
    // IDA 0x1402FECD0: Count empty slots
    std::int16_t count = 0;
    for (int i = 0; i < m_shOpenSlot; ++i) {
        if (!m_pItem[i]) {
            ++count;
        }
    }
    m_shEmptySlotCount = count;
    return count;
}

std::uint8_t XBaseInventory::GetLock(std::int16_t shSlot) {
    // IDA 0x1402FE670: Get lock status
    if (shSlot < 0 || shSlot >= m_shOpenSlot) {
        return 0;
    }
    return m_bLock[shSlot];
}

std::shared_ptr<CItem> XBaseInventory::GetSlotInfo(std::int16_t shSlot) {
    // IDA 0x1402FE5E0: Get item at slot
    if (shSlot < 0 || shSlot >= m_shOpenSlot) {
        return nullptr;
    }
    return m_pItem[shSlot];
}

std::shared_ptr<CItem> XBaseInventory::GetItem(int nItemID) {
    // IDA 0x1402FE6A0: Find item by ID
    for (int i = 0; i < m_shOpenSlot; ++i) {
        if (m_pItem[i] && m_pItem[i]->GetID() == nItemID) {
            return m_pItem[i];
        }
    }
    return nullptr;
}

std::shared_ptr<CItem> XBaseInventory::GetItem(std::int64_t biSerial) {
    // IDA 0x1402FE760: Find item by serial
    for (int i = 0; i < m_shOpenSlot; ++i) {
        if (m_pItem[i] && m_pItem[i]->GetSerial() == biSerial) {
            return m_pItem[i];
        }
    }
    return nullptr;
}

// IDA: 0x1402FFFE0. The original dereferences the classify table after the
// item-null test and dispatches through CItem's broach-list virtual slot.
void XBaseInventory::GetBroachList(PS_ITEM_BROACH_LIST& stBroachList) {
    for (int i = 0; i < m_shOpenSlot; ++i) {
        if (m_pItem[i] && m_pItem[i]->GetClassifyTable()->Item_Use_Type == 1) {
            m_pItem[i]->GetBroachList(stBroachList);
        }
    }
}

void XBaseInventory::GetSameItems(int nItemID, std::vector<std::shared_ptr<CItem>>* pVecItems, std::int16_t shExcludeSlot) {
    // IDA 0x1402FF170: Find all items with matching ID
    if (!pVecItems) return;

    pVecItems->clear();

    for (int i = 0; i < m_shOpenSlot; ++i) {
        // Skip excluded slot
        if (shExcludeSlot >= 0 && i == shExcludeSlot) continue;

        if (m_pItem[i] && m_pItem[i]->GetID() == nItemID) {
            pVecItems->push_back(m_pItem[i]);
        }
    }
}

bool XBaseInventory::AddItem(std::int16_t shSlot, std::shared_ptr<CItem> pItem) {
    // IDA 0x1402FE950: Add item to slot
    if (shSlot < 0 || shSlot >= m_shOpenSlot) {
        return false;
    }
    if (m_pItem[shSlot]) {
        return false;  // Slot not empty
    }
    m_pItem[shSlot] = pItem;
    return true;
}

// IDA 0x140300190: reserve existing stacks before materializing a new item.
bool XBaseInventory::AddItemCount(TB_ITEM* pTBItem,
                                  std::int16_t shAddCount,
                                  std::uint8_t byLock,
                                  bool bOption,
                                  PS_RES_STORAGE_INFO& psCreateItem,
                                  PS_RES_STORAGE_INFO& psUpdateItem) {
    std::int16_t shEmptyPos = -1;

    for (int i = 0; i < m_shOpenSlot; ++i) {
        if (!m_pItem[i]) {
            if (byLock == m_bLock[i]) {
                continue;
            }

            if (m_bLock[i]) {
                TXSingleton<XGameServer>::Instance()->SendItemLockLog(
                    0, m_byType, static_cast<std::int16_t>(i),
                    m_bLock[i], 28, byLock);
            } else if (shEmptyPos == -1) {
                shEmptyPos = static_cast<std::int16_t>(i);
            }
            continue;
        }

        if (m_bLock[i] == 1 ||
            m_pItem[i]->GetID() != static_cast<int>(pTBItem->Item_ID) ||
            m_pItem[i]->GetCount() == pTBItem->Item_Stack_Max ||
            !m_pItem[i]->IsAkashicRecordStack()) {
            continue;
        }

        PS_STORAGE_INFO stInfo{};
        stInfo.byInvenType = m_byType;
        stInfo.shSlotPos = static_cast<std::uint16_t>(i);
        m_pItem[i]->GetItem(&stInfo.stItem);

        const int nSaveCount = m_pItem[i]->GetCount() + shAddCount;
        if (pTBItem->Item_Stack_Max >= nSaveCount) {
            stInfo.stItem.sCount = static_cast<std::int16_t>(nSaveCount);
            shAddCount = 0;
        } else {
            stInfo.stItem.sCount = pTBItem->Item_Stack_Max;
            shAddCount = static_cast<std::int16_t>(
                nSaveCount - pTBItem->Item_Stack_Max);
        }

        SetLock(static_cast<std::int16_t>(i), byLock);
        psUpdateItem.vecItem.push_back(stInfo);
        if (shAddCount == 0) {
            return true;
        }
    }

    if (shEmptyPos == -1) {
        return false;
    }

    PS_STORAGE_INFO stCreate{};
    stCreate.byInvenType = m_byType;
    stCreate.shSlotPos = static_cast<std::uint16_t>(shEmptyPos);

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    pServer->GetItemFactory().CreateItem(
        stCreate.stItem,
        pTBItem->Item_ID,
        shAddCount,
        bOption,
        &pServer->GetResourceMgr(),
        byLock == 57);

    if (pTBItem->Item_Use_Period_Type == 1 &&
        pTBItem->Item_Use_Period_Value != 0) {
        stCreate.stItem.nCashDate =
            pServer->GetCurDate() + 60 * pTBItem->Item_Use_Period_Value;
    } else if (pTBItem->Item_Use_Period_Type == 5 &&
               pTBItem->Item_Use_Period_Value != 0) {
        if (pTBItem->Item_Stack_Max != 1) {
            LogHelper::LogError(
                "game.item",
                "AddItemCount error - Fault stack max [ ItemID:%d ]",
                pTBItem->Item_ID);
            return false;
        }

        const unsigned int dwValue = pTBItem->Item_Use_Period_Value;
        const int nYear = dwValue / 1000000 + 2000;
        const int nMonth = dwValue % 1000000 / 10000;
        const int nDay = dwValue % 1000000 % 10000 / 100;
        const int nHour = dwValue % 1000000 % 10000 % 100 % 100;
        const ATL::CTime tDate(nYear, nMonth, nDay, nHour, 0, 0, -1);
        stCreate.stItem.nCashDate = tDate.GetTime();
    }

    psCreateItem.vecItem.push_back(stCreate);
    SetLock(shEmptyPos, byLock);
    return true;
}

bool XBaseInventory::RemoveItem(std::int16_t shSlot) {
    // IDA 0x1402FF790: Remove item from slot
    if (shSlot < 0 || shSlot >= m_shOpenSlot) {
        return false;
    }
    m_pItem[shSlot].reset();
    return true;
}

// IDA: 0x1402FF670
// PDB: ?ReduceItem@XBaseInventory@@UEAAHFH@Z
int XBaseInventory::ReduceItem(std::int16_t shSlot, int nCount) {
    if (!CheckSlotPos(shSlot)) {
        return -1;
    }

    std::shared_ptr<CItem> pItem = m_pItem[shSlot];
    if (!pItem) {
        return -1;
    }

    const int nItemCount = pItem->GetCount();
    if (nItemCount < nCount) {
        return -1;
    }

    pItem->SetCount(nItemCount - nCount);
    return pItem->GetCount();
}

void XBaseInventory::SetLock(std::int16_t shSlot, std::uint8_t byLock) {
    // IDA 0x1403087F0: Set lock status
    if (shSlot >= 0 && shSlot < m_shOpenSlot) {
        m_bLock[shSlot] = byLock;
    }
}

bool XBaseInventory::CheckSlotPos(std::int16_t shSlot) {
    // IDA 0x1402FECA0: Check if slot position is valid
    return shSlot >= 0 && shSlot < m_shOpenSlot;
}

bool XBaseInventory::IsEmptySlot(std::int16_t shSlot) {
    // IDA 0x1402FF500: Check if slot is empty
    if (shSlot < 0 || shSlot >= m_shOpenSlot) {
        return false;
    }
    return !m_pItem[shSlot];
}

bool XBaseInventory::GetSlotInfos(PS_RES_STORAGE_INFO& stInfo) {
    // IDA 0x1402FE820: Get all slot info
    // TODO: Implement when PS_RES_STORAGE_INFO is fully defined
    return false;
}

void XBaseInventory::InitSimpleEmptySlot() {
    // IDA 0x140303400: Initialize empty slot count
    GetEmptySlotCount();
}

bool XBaseInventory::AddExtendSlot(std::uint8_t byStep, std::int16_t shSlot) {
    // IDA 0x1402FF5B0: Add extended slot
    // TODO: Implement slot extension logic
    return false;
}

bool XBaseInventory::CheckAddExtendSlot(std::uint8_t byStep, std::int16_t shSlot) {
    // IDA 0x1402FF560: Check if can add extended slot
    // TODO: Implement slot extension check
    return false;
}

// IDA: 0x1403008F0
// PDB: ?DelItemCount@XBaseInventory@@QEAA_NPEAUTB_ITEM@@FEAEAUPS_RES_STORAGE_INFO@@@Z
bool XBaseInventory::DelItemCount(TB_ITEM* pTBItem,
                                  std::int16_t shDelCount,
                                  std::uint8_t byLock,
                                  PS_RES_STORAGE_INFO& psUpdateItem) {
    for (int i = 0; i < m_shOpenSlot; ++i) {
        if (!m_pItem[i] ||
            m_pItem[i]->GetID() != static_cast<int>(pTBItem->Item_ID) ||
            m_bLock[i]) {
            continue;
        }

        PS_STORAGE_INFO stInfo{};
        stInfo.byInvenType = m_byType;
        stInfo.shSlotPos = static_cast<std::uint16_t>(i);
        m_pItem[i]->GetItem(&stInfo.stItem);

        const int nItemCount = m_pItem[i]->GetCount();
        if (shDelCount <= nItemCount) {
            stInfo.stItem.sCount = static_cast<std::int16_t>(
                nItemCount - shDelCount);
            shDelCount = 0;
        } else {
            stInfo.stItem.sCount = 0;
            shDelCount = static_cast<std::int16_t>(
                shDelCount - nItemCount);
        }

        if (stInfo.stItem.sCount != 0) {
            SetLock(static_cast<std::int16_t>(i), byLock);
        } else {
            SetLock(static_cast<std::int16_t>(i), 1);
        }

        psUpdateItem.vecItem.push_back(stInfo);
        if (shDelCount == 0) {
            return true;
        }
    }

    return false;
}

bool XBaseInventory::DelItemCountShop(TB_ITEM* pTBItem, int nDelCount, std::uint8_t byLock, PS_RES_STORAGE_INFO* psUpdateItem) {
    // IDA 0x140300BC0: Delete item count for shop purchases
    if (!pTBItem || !psUpdateItem) {
        return false;
    }

    // Iterate through all open slots
    for (int i = 0; i < m_shOpenSlot; ++i) {
        // Check if slot has an item
        if (!m_pItem[i]) {
            continue;
        }

        // Check if item ID matches and slot is not locked
        if (m_pItem[i]->GetID() != pTBItem->Item_ID || m_bLock[i]) {
            continue;
        }

        // Create storage info for this item
        PS_STORAGE_INFO stInfo;
        stInfo.byInvenType = m_byType;
        stInfo.shSlotPos = static_cast<std::int16_t>(i);

        // Get item data
        STItem itemData;
        m_pItem[i]->GetItem(&itemData);
        stInfo.stItem = itemData;

        // Reduce count
        std::int16_t currentCount = m_pItem[i]->GetCount();
        if (nDelCount <= currentCount) {
            stInfo.stItem.sCount = static_cast<std::int16_t>(currentCount - nDelCount);
            nDelCount = 0;
        } else {
            stInfo.stItem.sCount = 0;
            nDelCount -= currentCount;
        }

        // Set lock based on remaining count
        if (stInfo.stItem.sCount > 0) {
            SetLock(static_cast<std::int16_t>(i), byLock);
        } else {
            SetLock(static_cast<std::int16_t>(i), 1);
        }

        // Add to update list
        psUpdateItem->vecItem.push_back(stInfo);

        // If all items deleted, return success
        if (nDelCount == 0) {
            return true;
        }
    }

    return false;
}

// ============================================================================
// XBank Implementation
// ============================================================================

XBank::XBank() : XBaseInventory() {
    // IDA 0x1403029C0: Constructor calls XBaseInventory constructor
}

XBank::~XBank() {
    // IDA 0x140302A30: Destructor
}

void XBank::Init(int nSize) {
    // IDA 0x140302A60: Initialize bank with size
    m_shOpenSlot = static_cast<std::int16_t>(nSize);
}

void XBank::InitExtendStep(std::uint8_t byStep, int nBaseSize, int nIndex) {
    // IDA 0x140302B20: Initialize extend step
    m_byExtendStep = byStep;
    m_shOpenSlot = static_cast<std::int16_t>(nBaseSize);
}

bool XBank::LineUp(CMover* pMover, void* pstLineUpVec) {
    // IDA 0x140302BF0: Line up items in bank
    // TODO: Implement when CMover and line up structures are available
    return false;
}

// ============================================================================
// XInventory Implementation
// ============================================================================

XInventory::XInventory() : XBaseInventory() {
}

XInventory::~XInventory() {
}
