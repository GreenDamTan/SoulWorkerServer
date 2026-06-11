#include "XBaseInventory.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

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

bool XBaseInventory::RemoveItem(std::int16_t shSlot) {
    // IDA 0x1402FF790: Remove item from slot
    if (shSlot < 0 || shSlot >= m_shOpenSlot) {
        return false;
    }
    m_pItem[shSlot].reset();
    return true;
}

std::int16_t XBaseInventory::ReduceItem(std::int16_t shSlot, std::int16_t shCount) {
    // IDA 0x1402FF670: Reduce item count
    if (shSlot < 0 || shSlot >= m_shOpenSlot || !m_pItem[shSlot]) {
        return 0;
    }
    // TODO: Implement count reduction when CItem methods are available
    return shCount;
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
