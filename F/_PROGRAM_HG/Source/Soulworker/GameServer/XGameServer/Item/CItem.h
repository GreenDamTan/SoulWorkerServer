#pragma once

#include <cstdint>
#include <memory>

// Include full STItem definition
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

// Forward declarations
struct TB_ITEM;
struct TB_ITEM_CLASSIFY;
struct TB_AKASHIC_RECORDS;
struct ST_ITEM_SOCKET;
struct PS_ITEM_PACKAGE;

/**
 * @brief CItem - Item base class for GameServer
 *
 * Represents an item instance in the game.
 * This is a virtual base class with derived classes:
 * - CItemEquip: Equipment items
 * - CItemAkashic: Akashic items
 * - CItemCostume: Costume items
 *
 * IDA Evidence from GameServer.exe:
 * - Constructor: 0x140281950
 * - Destructor: 0x1400FA350
 * - GetSlot: 0x1400264F0
 * - GetCount: 0x140074910
 * - GetItem: 0x140074920
 * - GetInvenType: 0x1400266E0
 * - SetCount: 0x140082D00
 * - GetItemTable: 0x14009EF20
 * - GetClassifyTable: 0x14009EF40
 * - CanUse: 0x1400FA1F0 (virtual)
 */
class CItem {
public:
    CItem();
    virtual ~CItem();

    // === Virtual functions ===

    // CanUse - 0x1400FA1F0
    // _BOOL8 __fastcall CItem::CanUse(CItem *this)
    // { return this->m_pClassifyTable->GroupID == 17; }
    virtual bool CanUse();

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
    // __int64 __fastcall CItem::GetFlag(CItem *this)
    // { return this->m_stItem.eFlag; }
    std::uint8_t GetFlag() const;

    // GetEndurance - 0x140082D80
    // __int64 __fastcall CItem::GetEndurance(CItem *this)
    // { return this->m_stItem.byEndurance; }
    std::uint8_t GetEndurance() const;

    // GetID - Returns item ID from m_stItem.nItemID
    int GetID() const;

    // GetSerial - Returns item serial from m_stItem.xSerial
    std::int64_t GetSerial() const;

    // GetItemTitleID - 0x1400F8120
    // __int64 __fastcall CItem::GetItemTitleID(CItem *this)
    // { return (unsigned int)this->m_stItem.nTitleID; }
    int GetItemTitleID() const;

    // GetUseCount - 0x1400F8990
    // __int64 __fastcall CItem::GetUseCount(CItem *this)
    // { return this->m_stItem.byUseCount; }
    std::uint8_t GetUseCount() const;

    // GetItemExp - 0x1400F90A0
    // __int64 __fastcall CItem::GetItemExp(CItem *this)
    // { return (unsigned int)this->m_stItem.nExp; }
    int GetItemExp() const;

    // GetBind - 0x1400F9E80
    // __int64 __fastcall CItem::GetBind(CItem *this)
    // { return this->m_stItem.bBindType; }
    std::uint8_t GetBind() const;

    // === Mutators ===

    // SetSlot - Sets slot position
    void SetSlot(int nSlot);

    // SetInvenType - Sets inventory type
    void SetInvenType(std::uint8_t byType);

    // SetItem - 0x1400F77E0
    // void __fastcall CItem::SetItem(CItem *this, STItem *stItem)
    // { STItem::operator=(&this->m_stItem, stItem); }
    void SetItem(STItem& stItem);

    // SetEndurance - 0x1400F9E90
    // void __fastcall CItem::SetEndurance(CItem *this, unsigned __int8 byEndurance)
    // { this->m_stItem.byEndurance = byEndurance; }
    void SetEndurance(std::uint8_t byEndurance);

    // UpdateCashDate - 0x1400F9630
    // void __fastcall CItem::UpdateCashDate(CItem *this, int nDate)
    // { this->m_stItem.nCashDate = nDate; }
    void UpdateCashDate(int nDate);

    // SetItemTable - Sets item table pointer
    void SetItemTable(TB_ITEM* pTable);

    // SetClassifyTable - Sets classify table pointer
    void SetClassifyTable(TB_ITEM_CLASSIFY* pTable);

protected:
    // Member variables (from IDA analysis of constructor 0x140281950)
    STItem m_stItem;                           // +0x8: Item data structure (direct member, not pointer)
    TB_ITEM* m_pItemTable = nullptr;           // Item table reference
    TB_ITEM_CLASSIFY* m_pClassifyTable = nullptr; // Classify table reference
    std::int32_t m_nSlot = -1;                 // Slot position (initialized to -1)
    std::int32_t m_nOrder = 0;                 // Order
    std::uint8_t m_byInvenType = 0;            // Inventory type
    bool m_bEraseOnLineUp = true;              // Erase on line up flag (initialized to 1/true)
    PS_ITEM_PACKAGE* m_psPackageInfo = nullptr; // Package info pointer
};

/**
 * @brief CItemEquip - Equipment item class
 *
 * IDA Evidence:
 * - Constructor: 0x1400FA0C0
 * - Destructor: 0x1400F5000
 * - SetSocketActive: 0x1400FA050
 */
class CItemEquip : public CItem {
public:
    CItemEquip();
    virtual ~CItemEquip();

    // SetSocketActive - 0x1400FA050
    // void __fastcall CItemEquip::SetSocketActive(CItemEquip *this, unsigned __int8 bySocketPos)
    // { this->m_stItem.bySocketActiveCount = bySocketPos; }
    virtual void SetSocketActive(std::uint8_t bySocketPos);

private:
    // m_stSocketData[4] - Socket data array (each element is 0x38 bytes = ST_ITEM_SOCKET)
    // Constructor initializes this array
    // TODO: 需要定义 ST_ITEM_SOCKET 结构
    std::uint8_t m_stSocketData[4 * 0x38] = {}; // Placeholder
};

/**
 * @brief CItemAkashic - Akashic item class
 *
 * IDA Evidence:
 * - Constructor: 0x1400F9D30
 * - Destructor: 0x1400F4F50
 * - CanUse: 0x1400F9CF0
 * - SetAkashicTable: 0x1400F9CD0
 */
class CItemAkashic : public CItem {
public:
    CItemAkashic();
    virtual ~CItemAkashic();

    // CanUse - 0x1400F9CF0
    // _BOOL8 __fastcall CItemAkashic::CanUse(CItemAkashic *this)
    // { return this->m_pClassifyTable->GroupID == 20; }
    virtual bool CanUse() override;

    // SetAkashicTable - 0x1400F9CD0
    // void __fastcall CItemAkashic::SetAkashicTable(CItemAkashic *this, TB_AKASHIC_RECORDS *pTBItem)
    // { this->m_pAkashicTable = pTBItem; }
    void SetAkashicTable(TB_AKASHIC_RECORDS* pTable);

    // GetAkashicTable - Returns m_pAkashicTable
    TB_AKASHIC_RECORDS* GetAkashicTable() const;

private:
    TB_AKASHIC_RECORDS* m_pAkashicTable = nullptr;
};

/**
 * @brief CItemCostume - Costume item class
 *
 * Derived from CItem for costume equipment.
 */
class CItemCostume : public CItem {
public:
    CItemCostume() = default;
    virtual ~CItemCostume() = default;
};
