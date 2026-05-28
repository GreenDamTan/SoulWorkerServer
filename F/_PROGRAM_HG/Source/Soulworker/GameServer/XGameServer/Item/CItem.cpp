#include "CItem.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

// ============================================================================
// CItem - Item class for GameServer
//
// IDA Evidence from GameServer.exe (port 10004):
// - GetSlot: 0x1400264F0 - Returns m_nSlot
// - GetCount: 0x140074910 - Returns m_stItem.sCount
// - GetItem: 0x140074920 - Returns STItem copy
// - GetInvenType: 0x1400266E0 - Returns m_byInvenType
// - SetCount: 0x140082D00 - Sets m_stItem.sCount
// - GetItemTable: 0x14009EF20 - Returns m_pItemTable
// - GetClassifyTable: 0x14009EF40 - Returns m_pClassifyTable
// ============================================================================

CItem::CItem()
    : m_stItem(nullptr)
    , m_pItemTable(nullptr)
    , m_pClassifyTable(nullptr)
    , m_nSlot(0)
    , m_byInvenType(0)
{
}

CItem::~CItem() {
    // Note: m_stItem is typically managed externally
}

// ============================================================================
// Accessors (IDA verified)
// ============================================================================

// IDA: 0x1400264F0
// __int64 __fastcall CItem::GetSlot(CItem *this)
// { return (unsigned int)this->m_nSlot; }
int CItem::GetSlot() const {
    return m_nSlot;
}

// IDA: 0x140074910
// __int64 __fastcall CItem::GetCount(CItem *this)
// { return (unsigned int)this->m_stItem.sCount; }
int CItem::GetCount() const {
    if (m_stItem) {
        return m_stItem->sCount;
    }
    return 0;
}

// IDA: 0x140082D00
// void __fastcall CItem::SetCount(CItem *this, __int16 nCount)
// { this->m_stItem.sCount = nCount; }
void CItem::SetCount(std::int16_t nCount) {
    if (m_stItem) {
        m_stItem->sCount = nCount;
    }
}

// IDA: 0x1400266E0
// __int64 __fastcall CItem::GetInvenType(CItem *this)
// { return this->m_byInvenType; }
std::uint8_t CItem::GetInvenType() const {
    return m_byInvenType;
}

// IDA: 0x140074920
// STItem *__fastcall CItem::GetItem(CItem *this, STItem *result)
// { qmemcpy(result, &this->m_stItem, sizeof(STItem)); return result; }
STItem* CItem::GetItem(STItem* pOutItem) const {
    if (m_stItem && pOutItem) {
        *pOutItem = *m_stItem;
    }
    return pOutItem;
}

// IDA: 0x14009EF20
// TB_ITEM *__fastcall CItem::GetItemTable(CItem *this)
// { return this->m_pItemTable; }
TB_ITEM* CItem::GetItemTable() const {
    return m_pItemTable;
}

// IDA: 0x14009EF40
// TB_ITEM_CLASSIFY *__fastcall CItem::GetClassifyTable(CItem *this)
// { return this->m_pClassifyTable; }
TB_ITEM_CLASSIFY* CItem::GetClassifyTable() const {
    return m_pClassifyTable;
}

// IDA: 0x140082D70
std::uint8_t CItem::GetFlag() const {
    if (m_stItem) {
        return m_stItem->eFlag;
    }
    return 0;
}

// IDA: 0x140082D80
std::uint8_t CItem::GetEndurance() const {
    if (m_stItem) {
        return m_stItem->byEndurance;
    }
    return 0;
}

int CItem::GetID() const {
    if (m_stItem) {
        return m_stItem->nItemID;
    }
    return -1;
}

std::int64_t CItem::GetSerial() const {
    if (m_stItem) {
        return m_stItem->xSerial;
    }
    return -1;
}

// ============================================================================
// Mutators
// ============================================================================

void CItem::SetSlot(int nSlot) {
    m_nSlot = nSlot;
}

void CItem::SetInvenType(std::uint8_t byType) {
    m_byInvenType = byType;
}
