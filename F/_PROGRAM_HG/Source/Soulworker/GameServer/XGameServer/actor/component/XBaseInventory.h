#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// Forward declarations
class CItem;
class CMover;
struct TB_ITEM;
struct PS_RES_STORAGE_INFO;
struct PS_ITEM_SOCKET_LIST;
struct PS_ITEM_BROACH_LIST;
struct PS_ITEM_PACKAGE_LIST;

/**
 * @brief XBaseEquip - Base equipment class stub
 *
 * IDA Evidence: Equipment classes inherit from this
 */
class XBaseEquip {
public:
    XBaseEquip();
    virtual ~XBaseEquip() = default;

    std::shared_ptr<CItem> GetItem(std::int64_t biSerial);
    std::shared_ptr<CItem> GetItem(std::uint32_t dwItemID);
    void GetBroachList(PS_ITEM_BROACH_LIST& stBroachList);

    // IDA 0x1402FC030 / 0x1402FC130 directly replace the destination slot,
    // then update the item's slot and inventory type.
    virtual bool Equip(int nSlot, std::shared_ptr<CItem> pItem);
    virtual bool AddItem(std::int16_t shSlot, std::shared_ptr<CItem> pItem);

    virtual std::uint8_t GetLock(std::int16_t shSlot) {
        return shSlot >= 0 ? m_bLock[shSlot] : 6;
    }

    virtual void GetSlotInfo(std::shared_ptr<CItem>* ppItem, std::int16_t shSlot) {
        if (ppItem) {
            *ppItem = GetSlotInfo(shSlot);
        }
    }

    virtual std::shared_ptr<CItem> GetSlotInfo(std::int16_t shSlot) {
        if (shSlot < 0 || shSlot >= 20) {
            return nullptr;
        }
        return m_pItem[shSlot];
    }

    virtual std::uint8_t GetSetItemCount(unsigned int nCurID, unsigned int nSetItemID) {
        (void)nCurID;
        (void)nSetItemID;
        return 0;
    }

    virtual void MakeItemForSync(std::int16_t shSlot, void* pInfo) {
        (void)shSlot;
        (void)pInfo;
    }

    virtual bool Unequip(std::int16_t shSlot) {
        if (shSlot < 0 || shSlot >= 20) {
            return false;
        }
        m_pItem[shSlot].reset();
        return true;
    }

    virtual void SetLock(std::int16_t shSlot, std::uint8_t byFlag) {
        if (shSlot >= 0 && shSlot < 20) {
            m_bLock[shSlot] = byFlag;
        }
    }

    // IDA: ?GetInvenInfo@XBaseEquip@@QEAAXEAEAUPS_RES_STORAGE_INFO@@@Z @ 0x1402FDD50
    virtual void GetInvenInfo(std::uint8_t byInvenType, PS_RES_STORAGE_INFO& stInvenInfo);

protected:
    // PDB: XBaseEquip is 0x188 bytes. m_mapSetItem is preserved as a semantic
    // container; its current STL ABI is not claimed to match the original.
    std::shared_ptr<CItem> m_pItem[20];
    std::uint8_t m_bLock[20] = {};
    std::map<std::uint32_t, std::uint32_t> m_mapSetItem;
    std::uint8_t m_byType = 0;
};

/**
 * @brief XShapeEquip - Shape equipment class stub
 *
 * IDA Evidence from GameServer.exe:
 * - Constructor called in CGocInventory constructor
 */
class XShapeEquip : public XBaseEquip {
public:
    XShapeEquip();
    ~XShapeEquip() override = default;

    void GetCancelSlot(int nSlotType,
                       unsigned int nCancelBit,
                       std::vector<stEMPTYSLOT>& vecCancelSlot);
};

/**
 * @brief XAbilityEquip - Ability equipment class stub
 *
 * IDA Evidence from GameServer.exe:
 * - Constructor called in CGocInventory constructor
 */
class XAbilityEquip : public XBaseEquip {
public:
    XAbilityEquip();
    ~XAbilityEquip() override = default;
};

/**
 * @brief XLookEquip - Look equipment class stub
 *
 * IDA Evidence from GameServer.exe:
 * - Constructor called in CGocInventory constructor
 */
class XLookEquip : public XBaseEquip {
public:
    XLookEquip();
    ~XLookEquip() override = default;

    void GetCancelSlot(int nSlotType,
                       unsigned int nCancelBit,
                       std::vector<stEMPTYSLOT>& vecCancelSlot);
};

/**
 * @brief XBaseInventory - Base inventory class
 *
 * IDA Evidence from GameServer.exe (port 10004):
 * - Constructor: 0x140308610
 * - Destructor: 0x140307B20
 * - GetEmptySlot: 0x1402FEEB0
 * - GetItem: 0x1402FE6A0, 0x1402FE760
 * - AddItem: 0x1402FE950
 * - RemoveItem: 0x1402FF790
 * - GetSimpleEmptySlotCount: 0x140068290
 * - GetInvenType: 0x140027CD0
 */
class XBaseInventory {
public:
    // Constructor - IDA 0x140308610
    XBaseInventory();
    
    // Destructor - IDA 0x140307B20
    virtual ~XBaseInventory();
    
    // === Getters ===
    
    // GetInvenType - IDA 0x140027CD0
    std::uint8_t GetInvenType() const { return m_byType; }
    
    // GetSimpleEmptySlotCount - IDA 0x140068290
    std::int16_t GetSimpleEmptySlotCount() const { return m_shEmptySlotCount; }
    
    // GetOpenSlotNum - IDA 0x140307BC0
    std::int16_t GetOpenSlotNum() const { return m_shOpenSlot; }
    
    // GetExtendStep - IDA 0x140307BE0
    std::uint8_t GetExtendStep() const { return m_byExtendStep; }
    
    // GetEmptySlot - IDA 0x1402FEEB0
    virtual std::int16_t GetEmptySlot();

    // GetEmptySlot - IDA 0x1402FEF30
    virtual void GetEmptySlot(
        int nNeedCount,
        std::vector<stEMPTYSLOT>& vecEmptySlot);

    // CheckEmptySlotCount - IDA 0x1402FEE00
    virtual bool CheckEmptySlotCount(std::uint16_t wNeedCount);

    // GetEmptySlot_2 - alias for GetEmptySlot (IDA shows same function)
    std::int16_t GetEmptySlot_2() { return GetEmptySlot(); }

    // GetEmptySlotCount - IDA 0x1402FECD0
    virtual std::int16_t GetEmptySlotCount();
    
    // GetLock - IDA 0x1402FE670
    virtual std::uint8_t GetLock(std::int16_t shSlot);
    
    // GetSlotInfo - IDA 0x1402FE5E0
    virtual std::shared_ptr<CItem> GetSlotInfo(std::int16_t shSlot);
    
    // GetItem by ID - IDA 0x1402FE6A0
    std::shared_ptr<CItem> GetItem(int nItemID);

    // GetItem by Serial - IDA 0x1402FE760
    std::shared_ptr<CItem> GetItem(std::int64_t biSerial);

    // IDA 0x1402FFFE0: enumerate broach-capable item instances only.
    void GetBroachList(PS_ITEM_BROACH_LIST& stBroachList);

    // GetSameItems - IDA 0x1402FF170 - Find all items with matching ID
    // Returns items in a vector of shared_ptr<CItem>
    void GetSameItems(int nItemID, std::vector<std::shared_ptr<CItem>>* pVecItems, std::int16_t shExcludeSlot = -1);

    // GetSameItems_2 - alias for GetSameItems (IDA shows same signature)
    void GetSameItems_2(int nItemID, std::vector<std::shared_ptr<CItem>>* pVecItems, std::int16_t shExcludeSlot = -1) {
        GetSameItems(nItemID, pVecItems, shExcludeSlot);
    }
    
    // === Operations ===
    
    // AddItem - IDA 0x1402FE950
    virtual bool AddItem(std::int16_t shSlot, std::shared_ptr<CItem> pItem);

    // AddItemCount - IDA 0x140300190
    bool AddItemCount(TB_ITEM* pTBItem, std::int16_t shAddCount,
                      std::uint8_t byLock, bool bOption,
                      PS_RES_STORAGE_INFO& psCreateItem,
                      PS_RES_STORAGE_INFO& psUpdateItem);
    
    // RemoveItem - IDA 0x1402FF790
    virtual bool RemoveItem(std::int16_t shSlot);
    
    // ReduceItem - IDA 0x1402FF670
    virtual int ReduceItem(std::int16_t shSlot, int nCount);

    // DelItemCount - IDA 0x1403008F0
    bool DelItemCount(TB_ITEM* pTBItem, std::int16_t shDelCount,
                      std::uint8_t byLock,
                      PS_RES_STORAGE_INFO& psUpdateItem);

    // SetLock - IDA 0x1403087F0
    virtual void SetLock(std::int16_t shSlot, std::uint8_t byLock);
    
    // CheckSlotPos - IDA 0x1402FECA0
    virtual bool CheckSlotPos(std::int16_t shSlot);
    
    // IsEmptySlot - IDA 0x1402FF500
    virtual bool IsEmptySlot(std::int16_t shSlot);
    
    // GetSlotInfos - IDA 0x1402FE820
    virtual bool GetSlotInfos(PS_RES_STORAGE_INFO& stInfo);
    
    // InitSimpleEmptySlot - IDA 0x140303400
    void InitSimpleEmptySlot();
    
    // AddExtendSlot - IDA 0x1402FF5B0
    virtual bool AddExtendSlot(std::uint8_t byStep, std::int16_t shSlot);
    
    // CheckAddExtendSlot - IDA 0x1402FF560
    virtual bool CheckAddExtendSlot(std::uint8_t byStep, std::int16_t shSlot);

    // DelItemCountShop - IDA 0x140300BC0
    // Deletes item count for shop purchases
    virtual bool DelItemCountShop(TB_ITEM* pTBItem, int nDelCount, std::uint8_t byLock, PS_RES_STORAGE_INFO* psUpdateItem);

protected:
    // Member variables (from IDA constructor and methods)
    std::uint8_t m_byType = 0;                    // Inventory type
    std::uint8_t m_byExtendStep = 0;              // Extend step
    std::int16_t m_shOpenSlot = 0;                // Number of open slots
    std::int16_t m_shEmptySlotCount = 0;          // Count of empty slots
    
    // Item array (400 slots max based on constructor)
    std::shared_ptr<CItem> m_pItem[400];          // Item array
    std::uint8_t m_bLock[400] = {0};              // Lock flags
    
    // Note: The actual implementation uses boost::multi_index_container
    // For now, we use a simple array as a stub
};

/**
 * @brief XBank - Bank storage class (inherits from XBaseInventory)
 *
 * IDA Evidence from GameServer.exe (port 10004):
 * - Constructor: 0x1403029C0
 * - Destructor: 0x140302A30
 * - Init: 0x140302A60
 * - InitExtendStep: 0x140302B20
 */
class XBank : public XBaseInventory {
public:
    // Constructor - IDA 0x1403029C0
    XBank();
    
    // Destructor - IDA 0x140302A30
    virtual ~XBank();
    
    // Init - IDA 0x140302A60
    virtual void Init(int nSize);
    
    // InitExtendStep - IDA 0x140302B20
    virtual void InitExtendStep(std::uint8_t byStep, int nBaseSize, int nIndex);
    
    // LineUp - IDA 0x140302BF0
    virtual bool LineUp(class CMover* pMover, void* pstLineUpVec);
};

/**
 * @brief XInventory - Inventory storage class (inherits from XBaseInventory)
 *
 * IDA Evidence: Similar to XBank but with additional inventory-specific methods
 */
class XInventory : public XBaseInventory {
public:
    XInventory();
    virtual ~XInventory();
    
    // Additional inventory-specific methods can be added here
};
