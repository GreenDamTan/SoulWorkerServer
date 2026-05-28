#pragma once

#include <cstdint>
#include <memory>

// Forward declarations
struct STItem;
struct TB_ITEM;
struct TB_ITEM_CLASSIFY;

/**
 * @brief CItem - Item class for GameServer
 *
 * Represents an item instance in the game.
 *
 * IDA Evidence from GameServer.exe:
 * - GetSlot: 0x1400264F0 - Returns m_nSlot
 * - GetCount: 0x140074910 - Returns m_stItem.sCount
 * - GetItem: 0x140074920 - Returns STItem copy
 * - GetInvenType: 0x1400266E0 - Returns m_byInvenType
 * - SetCount: 0x140082D00 - Sets m_stItem.sCount
 * - GetItemTable: 0x14009EF20 - Returns m_pItemTable
 * - GetClassifyTable: 0x14009EF40 - Returns m_pClassifyTable
 */
class CItem {
public:
    CItem();
    ~CItem();

    // === Accessors (IDA verified) ===

    // GetSlot - 0x1400264F0
    // __int64 __fastcall CItem::GetSlot(CItem *this)
    // { return (unsigned int)this->m_nSlot; }
    int GetSlot() const;

    // GetCount - 0x140074910
    // __int64 __fastcall CItem::GetCount(CItem *this)
    // { return (unsigned int)this->m_stItem.sCount; }
    int GetCount() const;

    // SetCount - 0x140082D00
    // void __fastcall CItem::SetCount(CItem *this, __int16 nCount)
    // { this->m_stItem.sCount = nCount; }
    void SetCount(std::int16_t nCount);

    // GetInvenType - 0x1400266E0
    // __int64 __fastcall CItem::GetInvenType(CItem *this)
    // { return this->m_byInvenType; }
    std::uint8_t GetInvenType() const;

    // GetItem - 0x140074920
    // STItem *__fastcall CItem::GetItem(CItem *this, STItem *result)
    // { qmemcpy(result, &this->m_stItem, sizeof(STItem)); return result; }
    STItem* GetItem(STItem* pOutItem) const;

    // GetItemTable - 0x14009EF20
    // TB_ITEM *__fastcall CItem::GetItemTable(CItem *this)
    // { return this->m_pItemTable; }
    TB_ITEM* GetItemTable() const;

    // GetClassifyTable - 0x14009EF40
    // TB_ITEM_CLASSIFY *__fastcall CItem::GetClassifyTable(CItem *this)
    // { return this->m_pClassifyTable; }
    TB_ITEM_CLASSIFY* GetClassifyTable() const;

    // GetFlag - 0x140082D70
    std::uint8_t GetFlag() const;

    // GetEndurance - 0x140082D80
    std::uint8_t GetEndurance() const;

    // GetID - Returns item ID
    int GetID() const;

    // GetSerial - Returns item serial
    std::int64_t GetSerial() const;

    // === Mutators ===

    void SetSlot(int nSlot);
    void SetInvenType(std::uint8_t byType);

protected:
    // Member variables (from IDA analysis)
    STItem* m_stItem = nullptr;              // Item data structure
    TB_ITEM* m_pItemTable = nullptr;          // Item table reference
    TB_ITEM_CLASSIFY* m_pClassifyTable = nullptr; // Classify table reference
    std::int32_t m_nSlot = 0;                  // Slot position
    std::uint8_t m_byInvenType = 0;            // Inventory type
};
