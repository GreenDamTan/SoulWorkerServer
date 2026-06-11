#pragma once

#include <cstdint>
#include <memory>
#include <vector>

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
    XBaseEquip() = default;
    virtual ~XBaseEquip() = default;
    
    // Stub methods
    virtual std::shared_ptr<CItem> GetItem(std::int64_t biSerial) { return nullptr; }
};

/**
 * @brief XShapeEquip - Shape equipment class stub
 *
 * IDA Evidence from GameServer.exe:
 * - Constructor called in CGocInventory constructor
 */
class XShapeEquip : public XBaseEquip {
public:
    XShapeEquip() : XBaseEquip() {}
    virtual ~XShapeEquip() = default;
};

/**
 * @brief XAbilityEquip - Ability equipment class stub
 *
 * IDA Evidence from GameServer.exe:
 * - Constructor called in CGocInventory constructor
 */
class XAbilityEquip : public XBaseEquip {
public:
    XAbilityEquip() : XBaseEquip() {}
    virtual ~XAbilityEquip() = default;
};

/**
 * @brief XLookEquip - Look equipment class stub
 *
 * IDA Evidence from GameServer.exe:
 * - Constructor called in CGocInventory constructor
 */
class XLookEquip : public XBaseEquip {
public:
    XLookEquip() : XBaseEquip() {}
    virtual ~XLookEquip() = default;
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
    
    // === Operations ===
    
    // AddItem - IDA 0x1402FE950
    virtual bool AddItem(std::int16_t shSlot, std::shared_ptr<CItem> pItem);
    
    // RemoveItem - IDA 0x1402FF790
    virtual bool RemoveItem(std::int16_t shSlot);
    
    // ReduceItem - IDA 0x1402FF670
    virtual std::int16_t ReduceItem(std::int16_t shSlot, std::int16_t shCount);
    
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
