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
struct ST_ITEM_BROACH;
struct PS_ITEM_PACKAGE;
class CMover;

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

    // CanUse - 0x1400FA1F0 (virtual)
    // _BOOL8 __fastcall CItem::CanUse(CItem *this)
    // { return this->m_pClassifyTable->GroupID == 17; }
    virtual bool CanUse();

    // Init - 0x1402819E0
    // Initialize item from STItem data, loads TB_ITEM and TB_ITEM_CLASSIFY tables
    // Returns true on success, false if tables not found
    bool Init(const STItem& stItem);

    // SetOrder - 0x140281B50 (virtual)
    // Sets item order for sorting based on item properties and player class
    virtual void SetOrder(int nMyClass);

    // UnsetEffect - 0x140281C90 (virtual)
    // Virtual stub - removes item effects from mover
    virtual void UnsetEffect(CMover* pObject, bool bSend, std::uint8_t bySetCount);

    // SetSocketItem - 0x1400FA230 (virtual)
    // Virtual stub - sets socket item data
    virtual bool SetSocketItem(ST_ITEM_SOCKET stItemSocket, bool bLoad);

    // SetSocketEffect - 0x1400FA250 (virtual)
    // Virtual stub - applies socket effects to mover
    virtual void SetSocketEffect(CMover* pObject, std::uint8_t bySocketPos);

    // GetSocketItem - 0x1400FA260 (virtual)
    // Virtual stub - gets socket item data
    virtual ST_ITEM_SOCKET* GetSocketItem(std::uint8_t bySocketPos);

    // CompareEquipedSocket - 0x1400FA270 (virtual)
    // Virtual stub - compares equipped socket
    virtual void CompareEquipedSocket(int nSocketType, int nPropertyType, std::uint8_t& bySocketPos);

    // IsOpposite - 0x1400FA290 (virtual)
    // Virtual stub - checks if types are opposite
    virtual bool IsOpposite(int nType, int nCompareType);

    // SetBroach - 0x1400FA2B0 (virtual)
    // Virtual stub - sets broach data
    virtual void SetBroach(ST_ITEM_BROACH stBroach);

    // CanBroachActive - 0x1400FA2D0 (virtual)
    // Virtual stub - checks if broach can be activated
    virtual bool CanBroachActive(std::uint8_t byCheckRank);

    // CanBroachEquip - 0x1400FA2E0 (virtual)
    // Virtual stub - checks if broach can be equipped
    virtual bool CanBroachEquip(std::uint8_t byEquipPos, std::uint32_t dwClassifyIndex);

    // GetBroachInfo - 0x1400FA300 (virtual)
    // Virtual stub - gets broach info
    virtual void GetBroachInfo(ST_ITEM_BROACH& stBroach, int nIndex, int& nResult);

    // SetEnduranceEffect - 0x140281D50
    // Sets endurance effect on mover (complex function with stat calculations)
    void SetEnduranceEffect(CMover* pObject, bool bSend, std::uint8_t byPrevEndurance);

    // SetEffectSetItem - 0x1402825C0
    // Applies set item effects when wearing multiple pieces of a set
    void SetEffectSetItem(CMover* pObject, std::uint8_t bySetCount, bool bChange);

    // UnSetEffectSetItem - 0x140282930
    // Removes set item effects
    void UnSetEffectSetItem(CMover* pObject, std::uint8_t bySetCount, bool bChange);

    // UnSealDecEffect - 0x140282CF0
    // Decreases seal count
    void UnSealDecEffect(std::uint8_t byValue);

    // RestoreDecEffect - 0x140282D50
    // Decreases restore count
    void RestoreDecEffect(std::uint8_t byValue);

    // UpgradeDecEffect - 0x140282DA0
    // Decreases upgrade count
    void UpgradeDecEffect(std::uint8_t byValue);

    // UpdateSkillOptionEffectItem - 0x140282DF0
    // Updates skill option effects on item
    void UpdateSkillOptionEffectItem(bool bSet, CMover* pObject, int nIndex);

    // UpdateSkillOptionEffectSetItem - 0x1402830F0
    // Updates skill option effects for set items
    void UpdateSkillOptionEffectSetItem(bool bSet, std::uint8_t bySetCount, CMover* pObject, int nIndex);

    // UpdateSkillOptionEffectItemPart - 0x140283430
    // Updates skill option effect for specific skill group
    void UpdateSkillOptionEffectItemPart(int nSkillGroupID, CMover* pObject);

    // SetEffectTitleItemValue - 0x140283560
    // Sets title item value effect
    void SetEffectTitleItemValue(bool bWeapon, float fValue, bool bAdd);

    // SetEffectTitleItem - 0x140283700
    // Sets title item effects on mover
    void SetEffectTitleItem(CMover* pObject, bool bEquip);

    // SetSocketOption - 0x140283D20 (virtual)
    // Sets socket option data (copies 5 ST_EXTEND_OPTION structures)
    virtual void SetSocketOption(ST_EXTEND_OPTION* stOption);

    // RefineItemAbility - 0x140283D80
    // Refines item ability with random value generation
    bool RefineItemAbility(std::uint32_t nEffectID, bool bUsePrevent, int* nPrevValue, int* nChangedValue, bool* bPrevent);

    // SetPackageList - 0x140284380
    // Sets package list info
    void SetPackageList(PS_ITEM_PACKAGE& psInfo);

    // GetPackageInfo - 0x1402843C0
    // Gets package info
    void GetPackageInfo(PS_ITEM_PACKAGE& psInfo);

    // GetRevertBind - 0x140284460
    // Gets revert bind type based on current bind and equip state
    bool GetRevertBind(std::uint8_t& byBindType, bool bEquip);

    // GetUpgradeLimit - 0x1404EB7E0
    // Returns upgrade limit
    std::uint8_t GetUpgradeLimit() const;

    // UpgradeLimitInc - 0x1404EB7F0
    // Increments upgrade limit by specified count
    void UpgradeLimitInc(std::uint8_t byCount);

    // SetEraseLineUp - 0x140564300
    // Sets erase on line up flag
    void SetEraseLineUp(bool bFlag);

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

    // IsBind - 0x140284410
    // Returns true if item is bound
    bool IsBind() const;

    // SetBind - 0x140284440
    // Sets bind type
    void SetBind(std::uint8_t eBindType);

    // IsAkashicRecordStack - 0x140284500
    // Returns true if item is an Akashic Record stack (GroupID == 20 and Exp == 0)
    bool IsAkashicRecordStack() const;

    // GetOrder - 0x140307550
    // Returns item order for sorting
    std::int64_t GetOrder() const;

    // GetPackageList - 0x140284350
    // Gets package list info
    void GetPackageList(PS_ITEM_PACKAGE_LIST& psList);

    // GetUpgrade - 0x1403086C0
    // Returns item upgrade level
    std::uint8_t GetUpgrade() const;

    // GetDyeID - 0x1403086D0
    // Returns dye ID
    int GetDyeID() const;

    // SetUpgrade - 0x140407130
    // Sets upgrade level
    void SetUpgrade(std::uint8_t byUp);

    // SetDyeID - 0x1404EA7C0
    // Sets dye ID
    void SetDyeID(int nDyeID);

    // SetItemTitle - 0x1404EA890
    // Sets item title ID
    void SetItemTitle(int nTitleID);

    // SetRestoreCount - 0x1404EB050
    // Sets restore count
    void SetRestoreCount(std::uint8_t byCount);

    // SetFlag - 0x1404EB070
    // Sets item flag
    void SetFlag(std::uint8_t byFlag);

    // UpgradeCountInc - 0x1404EB090
    // Increments upgrade count
    void UpgradeCountInc();

    // GetRestoreCount - 0x1404EB5C0
    // Returns restore count
    std::uint8_t GetRestoreCount() const;

    // === Mutators ===

    // SetUseCount - 0x14019D1D0
    // void __fastcall CItem::SetUseCount(CItem *this, unsigned __int8 byUseCount)
    // { this->m_stItem.byUseCount = byUseCount; }
    void SetUseCount(std::uint8_t byUseCount);

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
    std::int64_t m_nOrder = 0;                 // Order (for sorting, uses int64 for large values)
    std::uint8_t m_byInvenType = 0;            // Inventory type
    bool m_bEraseOnLineUp = true;              // Erase on line up flag (initialized to 1/true)
    PS_ITEM_PACKAGE* m_psPackageInfo = nullptr; // Package info pointer

    // Additional members from IDA Init function (0x1402819E0)
    float m_fCurEnduranceRate = 0.0f;          // Current endurance rate
    std::int32_t m_nTitleValue[2] = {0, 0};    // Title values array
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
