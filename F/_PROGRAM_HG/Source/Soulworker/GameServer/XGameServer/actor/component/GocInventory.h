#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerItem.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCashShop.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include "Soulworker/GameServer/XGameServer/actor/component/XBaseInventory.h"
#include "Soulworker/GameServer/XGameServer/ShopStructures.h"
#include <cstdint>
#include <list>
#include <map>
#include <vector>
#include <memory>
#include <cstring>

// Forward declarations
class CItem;
class XBaseInventory;
class XBaseEquip;
class XInventory;
class XBank;
class XShapeEquip;
class XAbilityEquip;
class XLookEquip;
struct STItem;
struct TB_ITEM;
struct TB_ITEM_CLASSIFY;
struct TB_ITEM_ENDURANCE;
struct ST_PRIVATE_SHOP_LIST;
struct ST_PRIVATE_SHOP_INFO;
class XGameServer;
class XOption;

// Equipment container keys recovered from CGocInventory::GetEquipPtr callers.
enum eINVEN_TYPE : unsigned char {
    E_INVEN_TYPE_SHAPE_EQUIP = 0,
    E_INVEN_TYPE_ABILITY_EQUIP = 1,
    E_INVEN_TYPE_LOOK_EQUIP = 3,
};

// PDB eTRADE_STATE values used by CGocInventory trade transitions.
enum eTRADE_STATE {
    E_TRADE_STATE_NONE = 0,
    E_TRADE_STATE_READY_BEFORE = 1,
    E_TRADE_STATE_READY = 2,
    E_TRADE_STATE_UPDATE = 3,
    E_TRADE_STATE_CHECK = 4,
    E_TRADE_STATE_CHECK_BOTH = 5,
    E_TRADE_STATE_CONFIRM = 6,
};

static_assert(sizeof(eTRADE_STATE) == 4,
              "eTRADE_STATE size must match GameServer PDB");

// Local structs not in shared headers
struct ST_PRIVATE_SHOP_ITEM {
    std::shared_ptr<CItem> pItem;
    std::int64_t biMoney = 0;
};

// 复核注意：GameServer PDB UDT 0x13926 的大小是 4；不要套用 GocTrade.h 中旧的三字段推断。
struct PS_REQ_ITEM_TRADE {
    std::uint8_t byInvenType = 0;
    std::int16_t shSlotPos = 0;

    bool operator==(const PS_REQ_ITEM_TRADE& other) const {
        return byInvenType == other.byInvenType &&
               shSlotPos == other.shSlotPos;
    }
};

static_assert(sizeof(PS_REQ_ITEM_TRADE) == 4,
              "PS_REQ_ITEM_TRADE size must match GameServer PDB");

struct ST_MY_TRADE_INFO {
    std::int64_t biMoney = 0;
    std::list<PS_REQ_ITEM_TRADE> listInfo;
};

static_assert(sizeof(ST_MY_TRADE_INFO) == 32,
              "ST_MY_TRADE_INFO size must match GameServer PDB");

// Slot info for inventory slot management (placeholder)
struct ST_INVENTORY_SLOT_INFO {
    int nData = 0;
};

/**
 * @brief CGocInventory - Game Object Component for actor inventory
 *
 * Handles actor inventory, items, cash, money, etc.
 * Estimated ~100 functions based on PDB evidence.
 *
 * IDA Evidence from GameServer.exe:
 * - Constructor: 0x14009F7B0
 * - Init: 0x1400A00C0
 * - ClearInven: 0x1400A0000
 * - GetMoney: 0x140026700
 * - AddMoney: 0x1400A24C0
 * - SetInvenMoney: 0x1400A2340
 * - GetInvenPtr: 0x1400A2170
 * - AddPrivateShopItem: 0x1400B0D80
 * - PrivateShopItemList: 0x1400B11D0
 */
class CGocInventory : public GOComponent {
public:
    CGocInventory();
    virtual ~CGocInventory();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // === Currency functions (IDA verified) ===

    // GetMoney - 0x140026700
    // Returns: this->m_nInvenMoney
    std::int64_t GetMoney() const;

    // SetInvenMoney - 0x1400A2340
    // Sets m_nInvenMoney and updates CUser::stMyCharInfoEx()->biMoney
    // IDA: void __fastcall CGocInventory::SetInvenMoney(CGocInventory *this, __int64 nMoney, bool bSend)
    void SetInvenMoney(std::int64_t nMoney, bool bSend = false);

    // AddMoney - 0x1400A24C0
    // Adds money with logging and DB update
    bool AddMoney(std::int64_t biMoney, std::uint8_t byLogType, int nLogValue1, int LogValue2, bool bLog);

    // GetEther - 0x1400279C0
    std::int64_t GetEther() const;

    // GetBP - 0x1400279E0
    std::int64_t GetBP() const;

    // IsUseMoney - 0x140027A00
    bool IsUseMoney(std::int64_t nAmount) const;

    // AddBindMoney - 0x1400278B0
    bool AddBindMoney(std::int64_t nAmount, std::uint8_t byType, int nParam1, int nParam2, bool bLog);

    // === Inventory management ===

    // GetInvenPtr - 0x1400A2170
    // Returns inventory pointer by type (switch on byInvenType)
    XBaseInventory* GetInvenPtr(std::uint8_t byInvenType);

    // GetTBInvenPtr - 0x1400A2260
    XBaseInventory* GetTBInvenPtr(std::uint8_t byInvenType);

    // GetEquipPtr - 0x1400A2340
    XBaseEquip* GetEquipPtr(std::uint8_t byEquipType);

    // GetSlotItem - 0x1400A61F0
    // Gets item from inventory/equipment slot by type and position
    // Routes to GetEquipPtr or GetInvenPtr based on type
    std::shared_ptr<CItem> GetSlotItem(std::uint8_t byInvenType, std::uint16_t shSlotPos, bool& bLock);

    // GetItem - 0x1400AD750
    // Gets item from inventory by type and item ID
    std::shared_ptr<CItem> GetItem(std::uint8_t byInvenType, int nItemID);

    // GetInvenItem - 0x1400B1420
    // Gets item from all inventory types by serial ID
    // Searches: Common (2), Costume (4), Cube (0xB), Cash (0xD)
    std::shared_ptr<CItem> GetInvenItem(std::int64_t biSerial);

    // GetEquipItem - 0x1400B1680
    // Gets item from all equipment types by serial ID
    // Searches: Shape (0), Ability (1), Look (3)
    std::shared_ptr<CItem> GetEquipItem(std::int64_t biSerial);

    // GetBankItem - 0x1400B1850
    // Gets item from bank by serial ID
    // For NationType==2: searches Bank[0] (5), Bank[1] (6), Bank[2] (0xE)
    // For other nations: searches AccountBank[0] (0x10), AccountBank[1] (0x11), AccountBank[2] (0x12)
    std::shared_ptr<CItem> GetBankItem(std::int64_t biSerial);

    // GetSimpleEmptySlotCount - 0x140068290
    int GetSimpleEmptySlotCount() const;

    // === Initialization and cleanup (IDA verified) ===

    // Init - 0x1400A00C0
    // Initializes all inventory components, banks, and clears state
    bool Init();

    // ClearInven - 0x1400A0000
    // Clears: m_nInvenMoney, m_nBankMoney, m_nBP, m_biEther, m_nCash, m_nLimitMonsterBP, m_nLimitPVPBP
    void ClearInven();

    // ClearTradeInfo - 0x1400A0080
    void ClearTradeInfo();

    // InitTarde - 0x1400A0850
    void InitTarde();

    // InitItemCoolTime - 0x1400A08B0
    void InitItemCoolTime();

    // SetInventory - 0x1400A08E0
    // IDA: void __fastcall CGocInventory::SetInventory(CGocInventory *this,
    //         unsigned __int8 byCommonStep, unsigned __int8 byConsumeStep,
    //         unsigned __int8 byCustume, unsigned __int8 byCube,
    //         __int64 biMoney, __int64 biBP, __int64 biEther,
    //         __int64 biFriendPoint, __int64 biRecycle)
    // Initializes inventory extend steps and sets currency values
    void SetInventory(std::uint8_t byCommonStep, std::uint8_t byConsumeStep,
                      std::uint8_t byCustume, std::uint8_t byCube,
                      std::int64_t biMoney, std::int64_t biBP, std::int64_t biEther,
                      std::int64_t biFriendPoint, std::int64_t biRecycle);

    // === Private Shop functions (IDA verified) ===

    // AddPrivateShopItem - 0x1400B0D80
    // Adds item to private shop list (max 5 items)
    bool AddPrivateShopItem(std::shared_ptr<CItem> pItem, std::int64_t biMoney, bool& bExist);

    // DelPrivateShopItem - 0x1400B1000
    bool DelPrivateShopItem(std::shared_ptr<CItem> pItem);

    // PrivateShopItemList - 0x1400B11D0
    // Populates ST_PRIVATE_SHOP_LIST with shop items
    void PrivateShopItemList(ST_PRIVATE_SHOP_LIST& stList);

    // ClearPrivateShopList - 0x1400B1330
    void ClearPrivateShopList();

    // === Money/Bank operations ===

    // SetBankMoney - 0x1400A23B0
    // IDA: void __fastcall CGocInventory::SetBankMoney(CGocInventory *this, __int64 nMoney, bool bSend)
    void SetBankMoney(std::int64_t nMoney, bool bSend = false);

    // SendBankMoney - 0x1400A23E0
    void SendBankMoney();

    // SendMoney - 0x1400A2D70: Send current money to client
    // SendMoney - 0x1400A2E60: Send PS_GOLD_UPDATE structure to client
    void SendMoney();
    void SendMoney(PS_GOLD_UPDATE& stGold);

    // SetBP - 0x1400A2F30
    // IDA: void __fastcall CGocInventory::SetBP(CGocInventory *this, __int64 nBP, bool bSend)
    void SetBP(std::int64_t nBP, bool bSend = false);

    // AddBP - 0x1400A3000
    bool AddBP(std::int64_t nBP, std::uint8_t byLogType);

    // InitLimitBP - 0x1400A2FA0
    void InitLimitBP();

    // SetLimitBP - 0x1400A2FD0
    void SetLimitBP(int nLimitMonsterBP, int nLimitPVPBP);

    // SendBP - 0x1400A3C20 - sends BP update to client (main=8, sub=0x31)
    void SendBP(PS_BP_UPDATE& stBPUpdate);

    // SetEther - 0x1400A3CF0
    // IDA: void __fastcall CGocInventory::SetEther(CGocInventory *this, __int64 biEther, bool bSend)
    void SetEther(std::int64_t biEther, bool bSend = false);

    // AddEther - 0x1400A3D60
    bool AddEther(std::int64_t biEther, std::uint8_t byLogType, bool bLog);

    // SendEther - 0x1400A4450
    void SendEther(std::int64_t biResultEther);

    // DropEtherLog - 0x1400A4210
    void DropEtherLog();

    // SetMileage - 0x1400A0A40
    void SetMileage(int nDyePoint, int nRenovatePoint, int nRefinePoint);

    // InventoryInfoReq - 0x1400A0A90
    // Sends DB requests to load inventory/bank/socket/broach/package data
    void InventoryInfoReq(bool bInven, bool bBank, int dwUAID);

    // SetBankStep - 0x1400A1290
    // Initializes bank extend steps based on nation type
    void SetBankStep(std::uint8_t byCommonStep, std::uint8_t byCostume,
                     std::uint8_t byAccountCommonStep, std::uint8_t byAccountFashionStep);

    // GetMazeNeedItemID - 0x140082DD0
    int GetMazeNeedItemID() const;

    // SetMazeNeedItemID - 0x1400855C0
    void SetMazeNeedItemID(int nItemID);

    // SetTotalFriendPoint - 0x1400A4BF0
    void SetTotalFriendPoint(std::int64_t nPoint, bool bSend = false);

    // AddTotalFriendPoint - 0x1400A4C80
    bool AddTotalFriendPoint(std::int64_t nPoint, bool bSendDB);

    // SendTotalFriendPoint - 0x1400A4E30
    void SendTotalFriendPoint();

    // ClearRepurchaser - 0x1400A4F10
    void ClearRepurchaser();

    // PushRepurchaserItem - 0x1400A4F60
    void PushRepurchaserItem(PS_RES_STORAGE_INFO* stUpdateList, bool bSync);

    // EraseRepurchaserItem - 0x1400A5490
    void EraseRepurchaserItem(STItem* stItem);

    // AddDropMoney - 0x1400A2890
    bool AddDropMoney(std::int64_t nMoney, int nType, std::int64_t& nAddMoney,
                      std::uint8_t byLogType, int nParam1, int nParam2);

    // === Item functions ===
    int GetInventorySize() const;
    int GetUsedSlots() const;
    int GetFreeSlots() const;

    // HasItem, GetItemCount, AddItem, RemoveItem, MoveItem
    bool HasItem(int nItemId) const;
    int GetItemCount(int nItemId) const;
    bool AddItem(int nItemId, int nCount);
    bool RemoveItem(int nItemId, int nCount);
    bool MoveItem(int nFromSlot, int nToSlot);

    // RemoveItem - 0x1400A6DA0 (IDA verified)
    // Removes item from equipment or inventory by slot position
    bool RemoveItem(std::uint8_t byInvenType, std::int16_t shSlot);

    // RemoveItemBySlot - alias for RemoveItem(byInvenType, shSlot)
    bool RemoveItemBySlot(std::uint8_t byInvenType, std::int16_t shSlot) {
        return RemoveItem(byInvenType, shSlot);
    }

    // SaveQuickSlot - 0x1400A6EA0 (IDA verified)
    // Saves quick slot items to DB
    void SaveQuickSlot();

    // DivideItem - 0x1400A6390 (IDA verified)
    // Divides/splits an item stack
    bool DivideItem(PS_DB_ITEM_MOVE& stItemMove);

    // === Equipment functions (IDA verified) ===
    
    // GetEquippedItem - Get item ID at equipment slot
    int GetEquippedItem(int nEquipSlot) const;
    
    // EquipItem - 0x1400A5960 (wrapper for Equip)
    // Equips item from inventory slot to equipment slot
    bool EquipItem(int nSlotIndex, int nEquipSlot);
    
    // UnequipItem - 0x1400A5B10 (wrapper for Unequip)
    // Unequips item from equipment slot
    bool UnequipItem(int nEquipSlot);
    
    // GetEquipItemBySerial - 0x1400B1680 (IDA verified)
    // Gets item from all equipment types by serial ID
    // Searches: Shape (0), Ability (1), Look (3)
    std::shared_ptr<CItem> GetEquipItemBySerial(std::int64_t biSerial);

    // Equip - 0x1400A5960 (IDA verified)
    // Handles equipping an item from equipment slot
    void Equip(std::uint8_t byEquipType, std::int16_t shSlot);

    // Unequip - 0x1400A5B10 (IDA verified)
    // Full implementation with correct signature
    void Unequip(std::uint8_t byInvenType, std::int16_t shSlot);

    // ExchangeEquipSlot - 0x1400A5F30 (IDA verified)
    // Exchanges equipment slots within same equipment type
    void ExchangeEquipSlot(std::uint8_t byScrInvenType, std::int16_t shSrcSlot,
                           std::uint8_t byDestInvenType, std::int16_t shDestSlot);

    // IsValidMoveMoney - 0x1400A6060 (IDA verified)
    // Validates and processes money move request between inventory and bank
    bool IsValidMoveMoney(PS_REQ_MOVE_MONEY* psMoveMoney);

    // === Empty slot functions (IDA verified) ===
    
    // GetEmptySlot - Finds an empty slot in the specified inventory type
    // Returns -1 if no empty slot found
    std::int16_t GetEmptySlot(std::uint8_t byInvenType);
    
    // GetEmptySlotCount - Returns number of empty slots in inventory
    std::int16_t GetEmptySlotCount(std::uint8_t byInvenType) const;

    // === Inventory operations ===
    void SortInventory();

    // SetEquipItem - 0x1400A1380
    void SetEquipItem(ST_PRIVATE_SHOP_LIST* stEquipItem, int byInvenType);

    // === Weight/encumbrance ===
    int GetCurrentWeight() const;
    int GetMaxWeight() const;
    bool IsOverencumbered() const;

    // === Endurance functions (IDA verified) ===

    // AtkDecEndurance - 0x1400A7110
    // Decreases endurance on attack for equipped items
    void AtkDecEndurance();

    // DefDecEndurance - 0x1400A7340
    // Decreases endurance on defense for equipped items
    void DefDecEndurance();

    // DieDecEndurance - 0x1400A7540
    // Decreases endurance on death for equipped items
    void DieDecEndurance();

    // SendDecEndurance - 0x1400A7740
    // Sends endurance decrease update to client and DB
    void SendDecEndurance(ST_ENDURANCE_LIST* pstUpdateList);

    // SetEndurance - 0x1400A7910
    // Sets endurance value on an item, validates serial matches
    bool SetEndurance(std::uint8_t byInvenType, std::int16_t shSlotPos, STItem* pstItem);

    // SetInventoryInfos - 0x1400A7AE0
    // Loads inventory items from DB response
    void SetInventoryInfos(std::uint8_t byInvenType, PS_RES_STORAGE_INFO* psResInfo);

    // SendInventory - 0x1400A83F0
    // Sends inventory data to client
    void SendInventory();

    // SendBank - 0x1400A8770
    // Sends bank data to client
    void SendBank();

    // === Item use functions (IDA verified) ===

    // LineUp - 0x1400A96B0
    // Reorganizes/lines up items in inventory, sends update packets
    void LineUp(std::uint8_t byInvenType);

    // UseItem - 0x1400A9A30
    // Uses an item with various types (Akashic, Title, PackageBox, etc.)
    bool UseItem(std::uint8_t byInvenType, std::int16_t shSlot, std::uint8_t byCount,
                 std::int16_t shUseCount, int* nItemID);

    // CanUseItem - 0x1400AB0E0
    // Validates if item can be used (level, class, cooldown, area checks)
    bool CanUseItem(std::uint8_t byInvenType, std::int16_t shSlot);

    // SendQuickSlotInfo - 0x1400ACE80
    // Sends quick slot item info to client (main=8, sub=0x26)
    void SendQuickSlotInfo();

    // === Item movement and management (IDA verified) ===

    // AddItem - 0x1400A6B60 (STItem version)
    // PDB: ?AddItem@CGocInventory@@QEAA_NEFUSTItem@@_N@Z
    bool AddItem(std::uint8_t byInvenType, std::int16_t shSlot,
                 STItem stItem, bool);

    // AddItem - 0x1400A6920 (shared_ptr version)
    // Routes item to equipment (types 0,1,3) or inventory/bank (other types)
    bool AddItem(std::uint8_t byInvenType, std::int16_t shSlot, std::shared_ptr<CItem> pItem);

    // MoveItem - 0x1400A8AF0
    // Moves items between slots with support for equipment exchange
    bool MoveItem(/*PS_DB_ITEM_MOVE_VEC*/ void* stItems, int nTicknum);

    // SetQuickSlotItem - 0x1400ACA50
    // Updates quick slot items
    // PDB ABI is a non-const reference; do not change this to by-value.
    bool SetQuickSlotItem(PS_QUICKSLOT_UPDATE_ITEM& stUpdateSlot);

    // === Repurchaser functions (IDA verified) ===

    // IsRepurchaserItem - 0x1400A56D0
    // Checks if item is in repurchaser list
    bool IsRepurchaserItem(std::int64_t biItemSerial, int nItemID, std::int16_t shCount, STItem* stOutItem);

    // SendRepurchaseList - 0x1400A57A0
    // Sends repurchase list to client (main=9, sub=3)
    void SendRepurchaseList();

    // === Quick slot functions (IDA verified) ===

    // LoadQuickSlotItem - 0x1400ACD50
    // Loads quick slot items from DB response
    bool LoadQuickSlotItem(PS_QUICKSLOT_ITEM stQuickSlotItem);

    // === Item creation functions (IDA verified) ===

    // CreateItemPtr - 0x1400AD030
    // Constructs the item subtype selected by its table classification.
    std::shared_ptr<CItem> CreateItemPtr(STItem stItem);

    // CreateItemReq - 0x1400AD7E0
    // Creates item request with logging
    bool CreateItemReq(int nItemID, std::int16_t sCount, bool bAddOption,
                       eITEM_CREATE_TYPE eCreateType, ST_LOG_GAME& stLogData);

    // === Update functions (IDA verified) ===

    // OnUpdate - 0x1400AE280
    // Periodic update function (checks trade, cash item dates, etc.)
    void OnUpdate();

    // === Trade functions (IDA verified) ===

    // UpdateTradeUnLock - 0x1400AED60
    // Unlocks all items in trade list
    void UpdateTradeUnLock();

    // IsTradeCheck - 0x1400AEEE0
    // Checks trade state and timeout
    void IsTradeCheck();

    // === Item breaking/disposal functions (IDA verified) ===

    // BreakItemReq - 0x1400ADB20
    // Breaks/disposes an item with logging
    bool BreakItemReq(std::uint8_t byInvenType, std::int16_t shSlotPos, int nCount,
                      std::uint8_t byBreakLock, ST_LOG_GAME* stLogData);

    // === Logout/trade cancel functions (IDA verified) ===

    // LogOut - 0x1400AF190
    // Handles logout during trade
    void LogOut(bool bLogout);

    // SendTradeCancel - 0x1400AF370
    // Sends trade cancel notification
    void SendTradeCancel(int nCause);

    // === Helper item functions (IDA verified) ===

    // ConvertHelperInvenSlot - 0x1400AF660
    // Converts helper slot type (240-242) to inventory slot index (0-2)
    // Returns -1 (0xFFFFFFFFLL) for invalid slot types
    std::int64_t ConvertHelperInvenSlot(std::int16_t shSlotType);

    // IsHelperItem - 0x1400AF6C0
    // Checks if item is a helper item (slot type 240-242)
    bool IsHelperItem(std::uint32_t dwItemID);

    // === Trade item functions (IDA verified) ===

    // PopTradeItem - 0x1400AE6A0
    // Removes item from trade list
    bool PopTradeItem(PS_REQ_ITEM_TRADE stInfo);

    // SetTradeState - 0x1400AF760
    // Sets trade state and initializes timeout
    void SetTradeState(eTRADE_STATE eState);

    // === Item update/sync functions (IDA verified) ===

    // SendUpdateItem - 0x1400AF7A0 (list version)
    // Sends update for multiple items
    void SendUpdateItem(PS_RES_STORAGE_INFO& stItemList);

    // SendUpdateItem - 0x1400B00B0 (single item version)
    // Sends update for single item (main=8, sub=0x12)
    void SendUpdateItem(std::uint8_t byInvenType, std::int16_t shSlotPos,
                        std::int16_t shCount, std::uint8_t byType);

    // SendBreakItem - 0x1400AFFC0
    // Sends item break/delete notification (main=8, sub=5)
    void SendBreakItem(std::uint8_t byInvenType, std::int16_t shSlotPos);

    // === DB sync functions (IDA verified) ===

    // SendUpdateItemToDB - 0x1400AF900
    // Sends item updates to database (main=0x21, sub=0x11 or sub=5)
    void SendUpdateItemToDB(PS_RES_STORAGE_INFO& stItemList);

    // SendUserUpdateItem - 0x1400AFC40
    // Sends user item update notification (main=8, sub=0x15)
    void SendUserUpdateItem(PS_RES_STORAGE_INFO& psUpdateItem, bool bUnLock);

    // SendCreateItem - 0x1400AFDF0
    // Sends item creation notification (main=8, sub=6)
    void SendCreateItem(PS_RES_STORAGE_INFO& stItemList);

    // SendDivideItem - 0x1400B0230
    // Sends item divide/split notification (main=8, sub=4)
    void SendDivideItem(PS_DB_ITEM_MOVE& stItemMove);

    // === Quest items ===
    bool IsQuestItem(int nItemId) const;
    int GetQuestItemCount() const;

    // === Static functions ===

    // GetFamilyID - 0x1400262C0
    static int GetFamilyID();

    // === Additional helper functions (from IDA) ===

    // SetTradeActorID - 0x1400A0078
    void SetTradeActorID(UXActorID actorID);

    // SetLock - 0x1400A7020 (IDA verified)
    // Sets lock flag on inventory/equipment slot, returns true if successful
    bool SetLock(std::uint8_t byInvenType, std::int16_t shSlotPos, std::uint8_t byFlag);

    // === Appearance functions (IDA verified) ===

    // SendDBAppearanceLoad - 0x1400BB160
    // Sends DB request to load appearance list (main=0x21, sub=0x29)
    void SendDBAppearanceLoad();

    // LoadAppearanceList - 0x1400BB270
    // Loads appearance list from DB, validates entries, updates character info if expired
    void LoadAppearanceList(/*ST_APPEARANCE_LIST*/ void* stList);

    // IsHaveAppearance - 0x1400BB6E0
    // Checks if player has appearance (equipped or in list, not expired)
    bool IsHaveAppearance(std::uint16_t wAppearanceID);

    // EquipAppearance - 0x1400BB9D0
    // Equips appearance, validates expiry, syncs to DB
    void EquipAppearance(/*UAppearanceEx*/ void* reqAppearance);

    // SendEquipAppearance - 0x1400BBC70
    // Broadcasts equipped appearance to nearby players (main=8, sub=0x52)
    void SendEquipAppearance();

    // AddAppearance - 0x1400BB640
    // Adds multiple appearances from list, calls AddAppearance for each
    void AddAppearance(ST_APPEARANCE_LIST stList);

    // AddAppearance (single) - 0x1400BB800
    // Adds single appearance by ID with end date
    void AddAppearance(std::uint16_t wAppearanceID, std::int64_t biEndDate);

    // UpdateAppearance - 0x1400BB8C0
    // Updates appearance end date based on add seconds
    void UpdateAppearance(std::uint16_t wAppearanceID, std::int64_t biAddSec, std::int64_t& biEndDate);

    // OnAppearanceUpdate - 0x1400BBDD0
    // Checks appearance expiry, removes expired, syncs to DB and broadcasts
    void OnAppearanceUpdate();

    // SendDBUseInfo - 0x1400BC0E0
    // Sends item use info to DB (main=0x21, sub=0x26)
    void SendDBUseInfo();

    // ItemSocketLoad - 0x1400BC1F0
    // Loads item sockets from DB, applies to items
    void ItemSocketLoad(/*PS_ITEM_SOCKET_LIST*/ void* stSocketList);

    // ItemBroachLoad - 0x1400BC460
    // Loads item broaches from DB, applies to items
    void ItemBroachLoad(/*PS_ITEM_BROACH_LIST*/ void* stBroachList);

    // GetSocketList - 0x1400BC6D0
    // Gets socket list from inventory/equipment by type
    void GetSocketList(std::uint8_t byInvenType, /*PS_ITEM_SOCKET_LIST*/ void* stSocketList);

    // GetBroachList - 0x1400BC770
    // Equipment accepts shape/look (0/3); inventory accepts costume/bank (4/6/17).
    void GetBroachList(std::uint8_t byInvenType,
                       PS_ITEM_BROACH_LIST& stBroachList);

    // GetPackageList - 0x1400BC820
    // Gets package list from inventory by type (types 13-14)
    void GetPackageList(std::uint8_t byInvenType, /*PS_ITEM_PACKAGE_LIST*/ void* psPackageList);

    // SendDBSocketLoad - 0x1400BC880
    // Sends socket load request to DB (main=0x21, sub=0x30)
    void SendDBSocketLoad(bool bInven);

    // CanUseItem_AkashicRecord - 0x1400BCA40
    // Checks if akashic record item can be used (types 2, 13)
    bool CanUseItem_AkashicRecord(std::uint8_t byInvenType, std::int16_t shSlot);

    // UseItem_AkashicRecord - 0x1400BCD40
    // Uses akashic record item, adds to akashic card
    bool UseItem_AkashicRecord(std::uint8_t byInvenType, std::int16_t shSlot);

    // GetQuickSlotItem - 0x1400BD210
    // Gets quick slot items (4 items)
    void GetQuickSlotItem(int* pQuickSlotItem);

    // SendDBBroachLoad - 0x1400BD260
    // Sends broach load request to DB (main=0x21, sub=0x31)
    void SendDBBroachLoad(bool bInven);

    // UnLockList - 0x1400BD420
    // Unlocks items from storage info list
    void UnLockList(PS_RES_STORAGE_INFO psUnlockList);

    // CanItemFPUse - 0x1400B7730
    // Checks if FP item can be used (level check, usage check, FP cap)
    bool CanItemFPUse(std::uint8_t byInvenType, std::int16_t shSlot);

    // ItemFPUse - 0x1400B7AA0
    // Uses FP item, restores FP, sends to DB
    bool ItemFPUse(std::uint8_t byInvenType, std::int16_t shSlot);

    // UseItemInfo - 0x1400B7F90
    // Updates item use info by type, sends to DB
    void UseItemInfo(int nType);

    // CheatSetCashDate - 0x1400B8750
    // GM cheat to set cash item date (inven type 4)
    void CheatSetCashDate(std::int16_t shSlot, int nVal);

    // SendFurniture - 0x1400B88E0
    // Sends furniture inventory to client (main=8, sub=0x4A)
    void SendFurniture();

    // SendControlServerTradePassword - 0x1400BA760
    // Sends trade password check to control server (main=0xF3, sub=0x27)
    void SendControlServerTradePassword();

    // GM_ChangeSecondPW - 0x1400BA860
    // GM command to change second password
    void GM_ChangeSecondPW(wchar_t* strPW);

    // GM_ChangeTradePW - 0x1400BA8E0
    // GM command to change trade password, validates and sends to DB
    void GM_ChangeTradePW(wchar_t* strPW);

    // UseGacha - 0x1400BAAD0
    // Uses gacha system, random item selection based on tables
    bool UseGacha(std::uint16_t dwGetID, int nCount,
                  /*PS_GACHA_LIST*/ void* stGachaList,
                  /*ST_GET_INFO*/ void* stGetInfo,
                  /*PS_RES_STORAGE_INFO*/ void* vecCreateItem,
                  /*PS_RES_STORAGE_INFO*/ void* vecUpdateItem,
                  bool bTool);

    // SendSocketUpdate - 0x1400C0830
    // Sends socket update to client (main=8, sub=0x61)
    void SendSocketUpdate(/*PS_ITEM_SOCKET_LIST*/ void* stSocketList);

    // === Item add/reduce functions (IDA verified) ===

    // AddItem2 (private) - 0x1400BD4C0
    // Adds multiple items from list, calls AddItem2 for each
    bool AddItem2(ST_CREATE_ITEMS stCreateItems,
                  std::uint8_t byLock, bool bOption,
                  PS_RES_STORAGE_INFO& psCreateItem,
                  PS_RES_STORAGE_INFO& psUpdateItem);

    // AddItem2 (public) - 0x1400BD6C0
    // Adds single item by TB_ITEM pointer
    bool AddItem2(TB_ITEM* pTBItem, std::int16_t shAddCount,
                  std::uint8_t byLock, bool bOption,
                  PS_RES_STORAGE_INFO& psCreateItem,
                  PS_RES_STORAGE_INFO& psUpdateItem);

    // AddItemCheck - 0x1400BDAA0
    // Checks if item can be added to existing stack
    bool AddItemCheck(PS_RES_STORAGE_INFO& psItemList,
                      TB_ITEM* pTBItem, std::int16_t& shAddCount);

    // ReduceItem2 (public, list) - 0x1400BDBF0
    // PDB: ?ReduceItem2@CGocInventory@@QEAA_NUST_CREATE_ITEMS@@EAEAUPS_RES_STORAGE_INFO@@@Z
    bool ReduceItem2(ST_CREATE_ITEMS stReduceItems,
                     std::uint8_t byLock,
                     PS_RES_STORAGE_INFO& psUpdateItem);

    // ReduceItem2 (public, single) - 0x1400BDDB0
    // PDB: ?ReduceItem2@CGocInventory@@QEAA_NPEAUTB_ITEM@@FEAEAUPS_RES_STORAGE_INFO@@@Z
    bool ReduceItem2(TB_ITEM* pTBItem, std::int16_t shReduceCount,
                     std::uint8_t byLock,
                     PS_RES_STORAGE_INFO& psUpdateItem);

    // ReduceItem3 - 0x1400BDF10
    // PDB: ?ReduceItem3@CGocInventory@@QEAA_NEFFEAEAUPS_RES_STORAGE_INFO@@@Z
    bool ReduceItem3(std::uint8_t byInvenType, std::int16_t shSlotPos,
                     std::int16_t shDelCount, std::uint8_t byLock,
                     PS_RES_STORAGE_INFO& psUpdateItem);

    // ItemUseEffect - 0x1400BE230
    // PDB: ?ItemUseEffect@CGocInventory@@QEAA_NUPS_ITEM_SLOT_INFO@@0@Z
    // Uses effect items on target items (seal reduction 'Z', restoration '[', upgrade reduction '\\')
    bool ItemUseEffect(PS_ITEM_SLOT_INFO stUseItem,
                       PS_ITEM_SLOT_INFO stSelectItem);

    // ReduceItemCheck - 0x1400BEAE0
    // PDB: ?ReduceItemCheck@CGocInventory@@QEAA_NPEAUTB_ITEM@@AEAFEAEAUPS_RES_STORAGE_INFO@@@Z
    bool ReduceItemCheck(TB_ITEM* pTBItem, std::int16_t& shReduceCount,
                         std::uint8_t byLock,
                         PS_RES_STORAGE_INFO& psUpdateItem);

    // CreateItem2 (with ST_LOG_GAME) - 0x1400BEC70
    // Creates items with AddItem2, UpdateItemEnd, AddItemEnd chain
    bool CreateItem2(ST_CREATE_ITEMS stCreateItems,
                     std::uint8_t byLock, bool bOption,
                     PS_RES_STORAGE_INFO& psCreateItem,
                     PS_RES_STORAGE_INFO& psUpdateItem,
                     ST_LOG_GAME stLogGame);

    // AddItemEnd - 0x1400BEEE0
    // Finalizes item addition, sends statistics to DB, logs item creation
    bool AddItemEnd(std::uint8_t byCurLock,
                    PS_RES_STORAGE_INFO psCreateItem,
                    ST_LOG_GAME stLogGame);

    // UpdateItemEnd - 0x1400BF260
    // Finalizes item update after modification
    bool UpdateItemEnd(std::uint8_t byCurLock,
                       PS_RES_STORAGE_INFO psUpdateItem,
                       ST_LOG_GAME stLogGame);

    // === FP (Fatigue Point) item functions (IDA verified) ===

    // CanUseItemFPUseFree - 0x1400BF7B0
    // Checks if FP item can be used (level check, FP cap check)
    bool CanUseItemFPUseFree(std::uint8_t byInvenType, std::int16_t shSlot);

    // ItemFPUseFree - 0x1400BFAD0
    // Uses FP item, restores FP, sends to DB
    bool ItemFPUseFree(std::uint8_t byInvenType, std::int16_t shSlot);

    // === Grave/Infinite Tower item functions (IDA verified) ===

    // CanUseGraveInitItem - 0x1400BFFC0
    // Checks if grave init item can be used (inventory type, level, infinite tower limit)
    bool CanUseGraveInitItem(std::uint8_t byInvenType, std::int16_t shSlot);

    // UseGraveInitItem - 0x1400C0230
    // Uses grave init item, reduces item, sends DB packet
    bool UseGraveInitItem(std::uint8_t byInvenType, std::int16_t shSlot);

    // === Broach update functions (IDA verified) ===

    // SendBroachUpdate - 0x1400C0910
    // Sends broach update packet to client (main=8, sub=0x62)
    void SendBroachUpdate(/*PS_ITEM_BROACH_LIST*/ void* stBroachList);

    // === Package info functions (IDA verified) ===

    // SendPackageInfo - 0x1400C0A30
    // Sends package info packet to client (main=8, sub=0x72)
    void SendPackageInfo(/*PS_ITEM_PACKAGE_LIST*/ void* psPackageList);

    // === GM Cheat functions (IDA verified) ===

    // ItemMakeCheatByLevel - 0x1400C0B20
    // GM cheat to create items by level/class/rank
    bool ItemMakeCheatByLevel(std::uint8_t byLevel, std::uint8_t byClass,
                               bool bAddOption, std::uint8_t byUpgrade,
                               std::uint8_t byRank);

    // ItemMakeCheatOption - 0x1400C2690
    // GM cheat to create items with specific options
    bool ItemMakeCheatOption(int nType, int nItemID, int nOptionType, int nValue);

    // === Cash Buy Count functions (IDA verified) ===

    // LoadCashBuyCount - 0x1400C33F0
    // Loads cash buy count list from DB response
    void LoadCashBuyCount(PS_CASH_BUY_COUNT_LIST* psList);

    // UpdateCashBuyCount - 0x1400C3500
    // Updates buy count for a cash shop item
    bool UpdateCashBuyCount(int nCashShopIndex, int nBuyCount, std::uint8_t byLimitType,
                            int nLimitCount, PS_CASH_BUY_COUNT_LIST* psList);

    // SendUpdateCashBuyCount - 0x1400C3750
    // Sends cash buy count update to client (main=9, sub=0x31)
    void SendUpdateCashBuyCount(PS_CASH_BUY_COUNT_LIST* psList);

    // SendDBCashBuyCount - 0x1400C39B0
    // Sends cash buy count to DB (main=0x22, sub=0x26)
    void SendDBCashBuyCount();

    // === Endurance log functions (IDA verified) ===

    // SetEnduranceLog - 0x1400C7D90
    // Sets endurance log entry
    void SetEnduranceLog(/*PS_ITEM_ENDURANCE_INFO*/ void* psEnduranceInfo);

    // SendEnduranceLog - 0x1400C7E70
    // Sends endurance log to DB (main=4, sub=60)
    void SendEnduranceLog(int nMazeID);

    // === Money check functions (IDA verified) ===

    // CheckOverMoney - 0x1400C80E0
    // Checks if adding money would overflow the cap
    bool CheckOverMoney(int ePriceType, std::int64_t biValue);

    // CheckOverMoneyDrop - 0x1400C9D40
    // Checks and caps money overflow for drops based on currency type
    void CheckOverMoneyDrop(int ePriceType, std::int64_t& biValue);

    // === Recycle functions (IDA verified) ===

    // AddRecycle - 0x1400C9E60
    // Adds recycle points with DB sync and optional logging
    void AddRecycle(std::int64_t biRecycle, std::uint8_t byLogType, int nLogValue1, int nLogValue2, bool bLog);

    // SetRecycle - 0x1400CA190
    // Sets recycle value and syncs to user info
    void SetRecycle(std::int64_t biRecycle);

    // SendRecycle - 0x1400CA210
    // Sends recycle update to client (main=8, sub=0x63)
    void SendRecycle(/*PS_DB_RECYCLE_UPDATE*/ void* psDBRecycleInfo);

    // === Item reinforce functions (IDA verified) ===

    // ApplyItemReinforceOption - 0x1400CA300
    // Applies reinforce option stats to character attributes
    void ApplyItemReinforceOption(std::uint8_t byUpgrade, std::uint32_t dwReinforceOptionID, bool bEquip, bool bCalc);

    // === Wealth logging functions (IDA verified) ===

    // WealthLog - 0x1400CA5B0
    // Logs wealth changes (money, recycle) to DB
    void WealthLog(int nMoney, std::int16_t shSubType, std::uint8_t byLogType, /*PS_RES_STORAGE_INFO*/ void* stUpdateItemInfo);

    // === Skill option item functions (IDA verified) ===

    // CheckEquipSkillOptionItem - 0x1400CA990
    // Checks and updates skill option effects from equipped items
    void CheckEquipSkillOptionItem(int nSkillGroupIndex, bool bClear);

    // CheckEquipSkillOptionItemPart - 0x1400CACE0
    // Checks and updates skill option effects from equipped items (partial)
    void CheckEquipSkillOptionItemPart(int nSkillGroupIndex);

    // === Socket/Broach send functions (IDA verified) ===

    // SendSocketLoad - 0x1400C82C0
    // Sends socket load data to client (calls SendSocketInfo for each inventory type)
    void SendSocketLoad();

    // SendBroachLoad - 0x1400C8420
    // Sends broach load data to client (calls SendBroachInfo for each inventory type)
    void SendBroachLoad();

    // SendSocketInfo - 0x1400C85A0
    // Sends socket info packet to client (main=8, sub=0x55)
    void SendSocketInfo(/*PS_ITEM_SOCKET_LIST*/ void* psList, std::uint8_t byFlag);

    // SendBroachInfo - 0x1400C86A0
    // Consumes and destroys a caller-constructed stack-list temporary after send.
    void SendBroachInfo(PS_ITEM_BROACH_LIST* psList, std::uint8_t byFlag);

    // SendUseInfo - 0x1400C87A0
    // Sends item use info to client (main=8, sub=0x48)
    void SendUseInfo();

    // === Cash item date functions (IDA verified) ===

    // OnUpdateCashItemDate - 0x1400B8230
    // Updates cash item dates, removes expired items
    void OnUpdateCashItemDate();

    // GetCurItemsExp - 0x1400B8F40
    // Gets total exp from akashic items for compose
    int GetCurItemsExp(/*PS_ITEM_SLOT_INFO*/ void* stAkashicInfo,
                       /*std::vector<PS_ITEM_SLOT_INFO>*/ void* stInfo,
                       int& nNeedGold, bool& bSuccess);

    // === Cash functions (IDA verified) ===

    // GetCash - 0x1400F7940
    // Returns this->m_nCash
    int GetCash() const;

    // SetCash - 0x1400A49A0
    // Sets m_nCash and optionally syncs to DB
    void SetCash(int nCash, bool bSyncDB = false);

    // AddCash - 0x1400A4800
    // Adds cash with DB sync, returns false if overflow
    bool AddCash(int nCash, std::uint8_t byLogType);

    // SendCash - 0x1400A4B10
    // Sends cash update packet to client (main=8, sub=0x33)
    void SendCash(int nResultCash);

    // LoadCash - 0x1400A4530
    // Loads cash from DB (sends DB request if not already loaded)
    void LoadCash();

    // ReloadCash - 0x1400A4690
    // Forces reload of cash from DB
    void ReloadCash();

    // SetReadyLoadCash - 0x140068690
    // Sets the ready load cash flag
    void SetReadyLoadCash(bool bFlag);

    // SendCashCount - 0x1400C8960
    // Sends cash buy count list to client (main=9, sub=0x30)
    void SendCashCount();

    // SendCashSet - 0x1400C8B00
    // Sends cash set list to client (main=9, sub=0x22)
    void SendCashSet();

    // SendAppearacne - 0x1400C8C30
    // Sends appearance list to client (main=8, sub=0x50)
    void SendAppearacne();

    // === League inventory functions ===

    // MoveItemToLeagueInven - 0x1400C8DD0
    // Moves item to/from league inventory (complex function with 3 types)
    bool MoveItemToLeagueInven(/*PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME*/ void* psItemMoveForServer);

    // === Cash Mileage functions ===

    // GetCashMileage - 0x1400E5140
    // Returns cash mileage by type (0=Akashic, 1=Broach, 2=Tag)
    int GetCashMileage(E_CASH_MILEAGE_TYPE eType);

    // SetCashMileage (array) - 0x1400E4EA0
    // Sets cash mileage from array and optionally sends to client
    void SetCashMileage(int* pCashMileage, bool bSend = false);

    // SetCashMileage (single) - 0x1400E5020
    // Updates single cash mileage value and sends to client
    void SetCashMileage(PS_CASH_MILEAGE psUpdateInfo);

    // SendCashMileageLog - 0x1400E51A0
    // Sends cash mileage log to DB
    void SendCashMileageLog(std::uint8_t bySubType, /*PS_DB_CASH_MILEAGE_LIST*/ void* psList, unsigned int dwRecvUAID);

    // SendDBCashMileageUpdate - 0x1400E5500
    // Sends cash mileage update to DB (main=2, sub=0x68)
    void SendDBCashMileageUpdate(/*PS_DB_CASH_MILEAGE_LIST*/ void* psDBList);

    // SetRenovatePoint - 0x1400E5630
    // Sets renovate point value
    void SetRenovatePoint(int nPoint);

    // AddRenovatePoint - 0x1400E56B0
    // Adds renovate points with logging
    bool AddRenovatePoint(int nPoint, std::uint8_t bySubType);

    // SetRefinePoint - 0x1400E5890
    // Sets refine point value
    void SetRefinePoint(int nPoint);

    // AddRefinePoint - 0x1400E5910
    // Adds refine points with logging
    bool AddRefinePoint(int nPoint);

    // === Batch 15: Package functions (IDA verified) ===

    // SendDBPackageLoad - 0x1400E61F0
    // Sends package load request to DB
    void SendDBPackageLoad(bool bInven);

    // ItemPackageLoad - 0x1400E63B0
    // Loads item package list from DB
    void ItemPackageLoad(/*PS_ITEM_PACKAGE_LIST*/ void* psList);

    // SendPackageLoad - 0x1400E64F0
    // Sends package load to client
    void SendPackageLoad();

    // IsResealPackage - 0x1400E6A90
    // Checks if package ID exists in TB_REPACKAGECOSTUME
    bool IsResealPackage(int nPackageID);

    // IsResealPackageCount - 0x1400E6AD0
    // Checks if count matches item count in repackage table
    bool IsResealPackageCount(int nPackageID, int nCount);

    // === Batch 16: Simple getters/setters (IDA verified) ===

    // GetRefinePoint - 0x1400F7810
    // Returns this->m_nRefinePoint
    int GetRefinePoint() const;

    // SetRemoveSocket - 0x1400F7920
    // Sets m_bReqSocketRemove flag
    void SetRemoveSocket(bool bRemove);

    // GetHanBillNo - 0x1400F7B90
    // Returns pointer to m_szHanBillNo
    char* GetHanBillNo();

    // GetTradePasswordState - 0x1400F93B0
    // Returns m_byTradePassword
    std::uint8_t GetTradePasswordState();

    // === Batch 17: Trade and Cash Item functions (IDA verified) ===

    // GetTradeActorID - 0x1400F9C20
    // Returns m_uxTradeActorID
    UXActorID GetTradeActorID() const;

    // GetTradeState - 0x1400F9C50
    // Returns m_eTradeState
    eTRADE_STATE GetTradeState() const;

    // GetTradeMoney - 0x1400F9CB0
    // Returns m_stTradeInfo.biMoney
    std::int64_t GetTradeMoney() const;

    // SetCashItemDate - 0x1400FA4D0
    // Inserts serial/date pair into m_mpCashItemDate
    void SetCashItemDate(std::int64_t biSerial, int nCashDate);

    // === Batch 18: More getters/setters (IDA verified) ===

    // SetTradePasswordState - 0x1401E7F20
    // Sets m_byTradePassword
    void SetTradePasswordState(std::uint8_t byState);

    // GetBankMoney - 0x14048CE70
    // Returns m_nBankMoney
    std::int64_t GetBankMoney() const;

    // GetDyePoint - 0x1404EA7E0
    // Returns m_nDyePoint
    int GetDyePoint() const;

    // SetSocketExtract - 0x1404EAAD0
    // Sets m_bReqSocketExtract
    void SetSocketExtract(bool bExtract);

    // GetSocketExtract - 0x1404EAAF0
    // Returns m_bReqSocketExtract
    bool GetSocketExtract() const;

    // === Batch 19: Socket operations (IDA verified) ===

    // SetSocketUpgrade - 0x1404EAB10
    // Sets m_bReqSocketUpgrade
    void SetSocketUpgrade(bool bUpgrade);

    // GetSocketUpgrade - 0x1404EAB30
    // Returns m_bReqSocketUpgrade
    bool GetSocketUpgrade();

    // SetSocketExchange - 0x1404EAB80
    // Sets m_bReqSocketExchange
    void SetSocketExchange(bool bExchange);

    // GetSocketExchange - 0x1404EABA0
    // Returns m_bReqSocketExchange
    bool GetSocketExchange() const;

    // SetItemRefineReq - 0x1404EAC00
    // Sets m_bReqItemRefine
    void SetItemRefineReq(bool bRefine);

    // === Batch 20: More getters/setters (IDA verified) ===

    // GetItemRefineReq - 0x1404EAC20
    // Returns m_bReqItemRefine
    bool GetItemRefineReq() const;

    // SetRemoveBroach - 0x1404EAE70
    // Sets m_bReqBroachRemove
    void SetRemoveBroach(bool bRemove);

    // GetRemoveBroach - 0x1404EAE90
    // Returns m_bReqBroachRemove
    bool GetRemoveBroach() const;

    // GetRenovatePoint - 0x1404EAF50
    // Returns m_nRenovatePoint
    int GetRenovatePoint() const;

    // GetRemoveSocket - 0x1404EB900
    // Returns m_bReqSocketRemove
    bool GetRemoveSocket() const;

    // === Batch 21: League/Renovate/Absolute upgrade getters/setters (IDA verified) ===

    // SetReqLeagueNameChange - 0x14025CAE0
    // Sets m_bReqLeagueNameChange
    void SetReqLeagueNameChange(bool bChange);

    // SetRenovatePointItem - 0x1403E13D0
    // Sets m_bRenovateItem
    void SetRenovatePointItem(bool bRenovate);

    // CheatGetAbsoluteUpgrade - 0x1404070B0
    // Returns m_bAbsoluteUpgade
    bool CheatGetAbsoluteUpgrade();

    // CheatSetAbsoluteUpgrade - 0x1404070D0
    // Sets m_bAbsoluteUpgade
    void CheatSetAbsoluteUpgrade(bool bVal);

    // GetReqLeagueNameChange - 0x140504200
    // Returns m_bReqLeagueNameChange
    bool GetReqLeagueNameChange();

    // === Batch 22: Billing/Recycle/Mileage getters/setters (IDA verified) ===

    // SetProcessBilling - 0x14050A060
    // Sets m_bProcessBilling
    void SetProcessBilling(bool bUse);

    // GetRecycle - 0x1405DACB0
    // Returns m_biRecycle
    std::int64_t GetRecycle();

    // SetMileageShopBuyItem - 0x1405DACD0
    // Sets m_bReqShopBuy
    void SetMileageShopBuyItem(bool bBuy);

    // GetMileageShopBuyItem - 0x1405DACF0
    // Returns m_bReqShopBuy
    bool GetMileageShopBuyItem();

    // IsProcessBilling - 0x1405DAD10
    // Returns m_bProcessBilling
    bool IsProcessBilling();

    // === Batch 23: Tool clear/get functions (IDA verified) ===

    // ClearToolDisassemlbe - 0x14060D770
    // Clears m_stToolDisassemble vector
    void ClearToolDisassemlbe();

    // ClearToolSoulstoneInfo - 0x14060D790
    // Clears m_stToolSoulstone.vecInfo vector
    void ClearToolSoulstoneInfo();

    // ClearToolGachaInfo - 0x14060D9B0
    // Clears m_stToolItemInfo.vecInfo vector
    void ClearToolGachaInfo();

    // ClearToolRandomBoxInfo - 0x14060DA00
    // Clears m_stToolRandomBoxRes vector
    void ClearToolRandomBoxInfo();

    // GetPrivateShopItemCount - 0x140622430
    // Returns size of m_liPrivateShopItem list
    std::int16_t GetPrivateShopItemCount();

    // === Batch 24: Tool get/Trade functions (IDA verified) ===

    // GetToolSoulstone - 0x14060D950
    // Copies m_stToolSoulstone to output param
    void GetToolSoulstone(PS_RES_TOOL_SOULSTONE& psToolInfo);

    // GetToolGachaInfo - 0x14060D9D0
    // Copies m_stToolItemInfo to output param
    void GetToolGachaInfo(PS_RES_TOOL_DROP_INFO& stItemInfo);

    // SetTradeMoney - 0x1406225F0
    // Sets m_stTradeInfo.biMoney
    void SetTradeMoney(std::int64_t biMoney);

    // PushTradeInfo - 0x1406227A0
    // Pushes trade info to m_stTradeInfo.listInfo
    void PushTradeInfo(PS_REQ_ITEM_TRADE stInfo);

    // GetTradeInfoSize - 0x1406227D0
    // Returns size of m_stTradeInfo.listInfo
    int GetTradeInfoSize();

    // === Batch 25: ToolRandomBox/Endurance functions (IDA verified) ===
    // Note: SetCashMileage, GetCashMileage, SendDBCashMileageUpdate already declared earlier

    // GetToolRandomBoxInfo - 0x14060DA20
    // Copies m_stToolRandomBoxRes to output param
    void GetToolRandomBoxInfo(ST_CREATE_ITEMS& stItems);

    // SetTableItemEndurance - 0x14070AEF0
    // Sets m_pEnduranceTable
    void SetTableItemEndurance(TB_ITEM_ENDURANCE* pTable);

    // === Batch 26: Equipment stat calculation (IDA verified) ===

    // CalculateEquipStat - 0x1400E85D0
    // Calculates equipment stats for given inventory type
    void CalculateEquipStat(std::uint8_t byInvenType);

    // AddItemUpgradeCount - 0x1400E77A0
    // Adds item with upgrade count (private helper)
    bool AddItemUpgradeCount(/*TB_ITEM*/ void* pTBItem, std::int16_t shAddCount,
                              std::uint8_t byUpgrade, std::uint8_t byLock, bool bOption,
                              /*PS_RES_STORAGE_INFO*/ void* psCreateItem,
                              /*PS_RES_STORAGE_INFO*/ void* psUpdateItem);

    // === Batch 27: Billing response functions (IDA verified, complex) ===

    // BuyItemByHanBillingRes2 - 0x1400C6090
    // Handles Han billing response for item purchase
    bool BuyItemByHanBillingRes2(/*PS_HAN_BILLING_ORDER_NO_VEC*/ void* stHanBilling,
                                  int nDecCash, /*PS_CASH_BUY_COUNT_LIST*/ void* psCashbuyList);

    // GiftItemByHanBillingRes2 - 0x1400C72B0
    // Handles Han billing response for gift purchase
    bool GiftItemByHanBillingRes2(/*PS_HAN_BILLING_ORDER_NO_VEC*/ void* stHanBilling,
                                   int nDecCash, /*PS_CASH_BUY_COUNT_LIST*/ void* psCashbuyList);

    // BuyItemByWMBillingReq - 0x1400CBCC0
    // Sends WM billing request for item purchase
    int BuyItemByWMBillingReq(unsigned int dwUAID, /*ST_CASH_ITEM_BUY_LIST*/ void* stCashItemList,
                               bool bGift, unsigned int dwRecvUCID, int nRecvClass,
                               const char* szRecvAccountID, const wchar_t* szRecvCharacterID,
                               unsigned int dwRecvUAID);

    // BuyItemByWMBillingRes - 0x1400CD290
    // Handles WM billing response for item purchase
    bool BuyItemByWMBillingRes(/*PS_HAN_BILLING_ORDER_NO_VEC*/ void* stHanBilling,
                                /*PS_CASH_BUY_COUNT_LIST*/ void* psCashbuyList);

    // GiftItemByWMBillingRes - 0x1400CE340
    // Handles WM billing response for gift purchase
    bool GiftItemByWMBillingRes(/*PS_HAN_BILLING_ORDER_NO_VEC*/ void* stHanBilling,
                                 /*PS_CASH_BUY_COUNT_LIST*/ void* psCashbuyList);

    // Note: SetTradeActorID already declared earlier

    // IsBuyCashLimitCount - 0x1400E5AD0
    // Checks if buy limit type is valid and calculates end date
    bool IsBuyCashLimitCount(E_CASH_SHOP_BUY eLimitType, std::int64_t& biEndDate);

    // OnInitItemCashCount - 0x1400E5FA0
    // Initializes cash item buy count, clears expired entries
    void OnInitItemCashCount();

    // === Cash Item Set functions ===

    // AddCashItemSet - 0x1400B89E0
    // Adds cash item set list to m_stCashSet array
    void AddCashItemSet(PS_CASH_SET_LIST* stCashSetList);

    // DelCashItemSet - 0x1400B8B10
    // Deletes a cash item set by index, syncs to DB
    bool DelCashItemSet(std::uint8_t bySetNo);

    // UpdateCashItemSet - 0x1400B8C90
    // Updates a cash item set, syncs to DB
    bool UpdateCashItemSet(PS_CASH_SET* stCashSet);

    // === Random Box / Package Box functions (IDA verified) ===

    // PackageBoxUse - 0x1400B2D80 (covers 0x1400B3BE0, 0x1400B3CC0)
    // Uses a package box item, creates contained items
    // Note: The addresses 0x1400B3BE0 and 0x1400B3CC0 point within this function
    bool PackageBoxUse(bool bReduceItem, std::shared_ptr<CItem> pItem,
                       std::uint8_t byCount, int nItemIDparClass);

    // === Equipment slot functions (IDA verified) ===

    // CanEquipSlotOpen - 0x1400B6570
    // Checks if equipment slot can be opened (validates item level, slot position)
    bool CanEquipSlotOpen(std::uint8_t byInvenType, std::int16_t shSlot);

    // CheckEquipSlotOpen - 0x1400B6E90
    // Checks if equipment slot at position is opened (bitwise check on m_nEquipSlot)
    bool CheckEquipSlotOpen(std::uint8_t byPos);

    // ChangeEquipSlotPos - 0x1400B6D20
    // Converts slot type to equip position and bit, returns true if slot not yet opened
    bool ChangeEquipSlotPos(std::uint8_t bySlotType, std::uint8_t& byEquipPos, int& nEquipPosBit);

    // SendEquipSlotOpen - 0x1400B70D0
    // Sends equipment slot open status to client (main=8, sub=0x14)
    void SendEquipSlotOpen();

    // SetEquipSlot - 0x1400B71B0
    // Sets equipment slot value and recalculates
    void SetEquipSlot(int nEquipSlot);

    // EquipSlotOpenCalc - 0x1400B71E0
    // Calculates and unlocks equipment slots based on m_nEquipSlot bits
    void EquipSlotOpenCalc();

    // EquipSlotOpen (bool return) - 0x1400B6810
    // Opens equipment slot using item from inventory
    bool EquipSlotOpen(std::uint8_t byInvenType, std::int16_t shSlot);

    // EquipSlotOpen (void return) - 0x1400B7090
    // Sets the equip slot bit if not already set
    void EquipSlotOpen(int nPosBit);

    // === Inventory slot functions (IDA verified) ===

    // InitEmptySlot - 0x1400CB000
    // Initializes empty slots for inventory types 2, 13, 4, 11
    void InitEmptySlot();

    // IsValidSlotPos - 0x1400CED20
    // Validates slot positions (must be >= 0 and <= 400)
    bool IsValidSlotPos(std::int16_t shSrcPos, std::int16_t shDestPos);

    // IsEmptyInventory - 0x1400CB0A0
    // Checks if inventory has enough empty slots for each type
    bool IsEmptyInventory(int nCommonCnt, int nFashionCnt, int nCashCnt, int nCubeCnt);

    // IsEmptyInventory (2-param) - 0x1400D5860
    // Checks if specific inventory type has enough empty slots
    // nInvenType: 0=common, 1=fashion, 2=cash, 3=cube
    bool IsEmptyInventory(int nInvenType, int nNeedInven);

    // === Socket upgrade functions (IDA verified) ===

    // UpgradeSocket (4-param) - 0x1400D29F0
    // Upgrades socket on equipment item
    int UpgradeSocket(/*PS_ITEM_SLOT_INFO*/ void* psEquipItemInfo, std::uint32_t nItemSocketID, std::uint8_t bySocketIndex, bool bTool);

    // UpgradeSocket (1-param) - 0x1400D34E0
    // Upgrades socket item itself (consumes item to create upgraded version)
    int UpgradeSocket(/*PS_ITEM_SLOT_INFO*/ void* psSocketItemInfo);

    // === Socket exchange functions (IDA verified) ===

    // ChangeActiveBroachEffect - 0x1400CB320
    // Zero clears the effect; nonzero values must belong to equipped broach data.
    void ChangeActiveBroachEffect(PS_ACTIVE_BROACH_EFFECT& psBroach);

    // === Item logging functions (IDA verified) ===

    // ItemLog - 0x1400C5470
    void ItemLog(std::shared_ptr<CItem> pItem, STItem stResultItem,
                 ST_LOG_GAME stLog);

    // ItemLogCharLevel - 0x1400C5E00
    // Sets character level in log based on sub type
    void ItemLogCharLevel(int nLevel, ST_LOG_GAME& stLog);

    // ItemLogItemType - 0x1400C5F10
    // Sets item type value in log based on use type and slot type
    void ItemLogItemType(int nItemUseType, int nSlotType, ST_LOG_GAME& stLog);

    // ItemLogBeforeCount - 0x1400C6060
    // Sets before count in log (only if sub type < 60)
    void ItemLogBeforeCount(int nCount, ST_LOG_GAME& stLog);

    // === Socket exchange functions (IDA verified) ===

    // ExchangeSocket - 0x1400D2140
    // Exchanges socket items using TB_FRAGMENT_EXCHANGE
    int ExchangeSocket(std::uint8_t byCount, std::uint32_t dwExchangeID, std::uint8_t byIndex);

    // === Tool soulstone info functions (IDA verified) ===

    // InsertToolSoulstoneInfo (ST_CREATE_ITEM) - 0x1400D5630
    // Inserts soulstone info to m_stToolSoulstone
    void InsertToolSoulstoneInfo(/*ST_CREATE_ITEM*/ void* stItem, std::uint8_t byRateType);

    // InsertToolSoulstoneInfo (int) - 0x1400D56C0
    // Inserts soulstone info with ID directly
    void InsertToolSoulstoneInfo(int nID, std::uint8_t byFlag);

    // === Socket extract functions (IDA verified) ===

    // IsValidExtractSocket - 0x1400D3F70
    // Validates socket extraction and builds extract info
    int IsValidExtractSocket(std::uint32_t dwUCID, std::uint32_t nSocketItemID, bool bUseItem,
                             /*PS_DB_SOCKET_EXTRACT*/ void* psDBExtractInfo, std::int64_t& biCost, bool bTool);

    // ExtractSocket (4-param) - 0x1400D4BE0
    // Extracts socket from equipment item
    int ExtractSocket(/*PS_ITEM_SLOT_INFO*/ void* psEquipItemInfo, std::uint32_t nItemSocketID,
                      std::uint8_t bySocketIndex, bool bUseItem);

    // ExtractSocket (2-param) - 0x1400D4FF0
    // Extracts socket item itself
    int ExtractSocket(/*PS_ITEM_SLOT_INFO*/ void* psItemInfo, bool bUseItem);

    // === Tool disassemble functions (IDA verified) ===

    // GetToolDisassemble - 0x1400D5750
    // Gets tool disassemble info from m_stToolDisassemble
    void GetToolDisassemble(/*PS_RES_TOOL_DROP_INFO*/ void* psToolInfo);

    // InsertToolDisassemble - 0x1400D5830
    // Inserts disassemble info to m_stToolDisassemble
    void InsertToolDisassemble(/*ST_QUEST_FIRST_DROP_ITEM*/ void* psInfo);

    // === Warp item functions (IDA verified) ===

    // DeleteUsedWarpItem - 0x1400D6D50
    // Deletes used warp item after teleport
    void DeleteUsedWarpItem();

    // ClearUsedWarpItem - 0x1400D73D0
    // Clears used warp item state with optional logging
    void ClearUsedWarpItem(std::uint8_t byLogType);

    // CheckMoveMap - 0x1400D76F0
    // Checks if map move is valid (maze, district, roguelike)
    int CheckMoveMap(int nMoveType, /*PS_ENTER_MAP_REQ*/ void* psEnterMap);

    // === Item limit functions (IDA verified) ===

    // LoadLimitItemInfo - 0x1400D8740
    // Loads item limit info from DB
    void LoadLimitItemInfo(/*ST_ITEM_LIMIT_LIST*/ void* stItemLimitList);

    // UpdateLimitItemInfo - 0x1400D89F0
    // Updates item limit count
    bool UpdateLimitItemInfo(int nItemID, std::int16_t shCount, std::int16_t& shRemainCount);

    // === Batch 4: Item selection and limit functions (IDA verified) ===

    // UseItemSelect - 0x1400D95A0
    // Processes item selection use (select items from list)
    int UseItemSelect(/*PS_REQ_USE_ITEM_SELECT*/ void* psUseInfo);

    // InitLimitItemInfo - 0x1400DB6A0
    // Initializes item limit info with date reset
    void InitLimitItemInfo(/*ATL::CTime*/ std::int64_t tTodayInit, bool bCheat);

    // SendDBUpdateLimitItem - 0x1400DB840
    // Sends item limit update to DB (main=0x21, sub=0x40)
    void SendDBUpdateLimitItem(/*PS_ITEM_LIMIT*/ void* psLimitInfo);

    // ReduceItemList - 0x1400DB970
    // PDB: ?ReduceItemList@CGocInventory@@QEAA_NAEAUST_CREATE_ITEMS@@EAEAUST_LOG_GAME@@@Z
    bool ReduceItemList(ST_CREATE_ITEMS& stReduceItemList,
                        std::uint8_t byLock,
                        ST_LOG_GAME& stLog);

    // ReviveCash - 0x1400D8CE0
    // Revives/resets cash item billing
    bool ReviveCash();

    // === Batch 5: Trade and item creation functions (IDA verified) ===

    // SendCombineItem - 0x1400B07E0
    // Sends combine item result to client and statistics DB
    void SendCombineItem(/*PS_DB_ITEM_MOVE*/ void* psDBItemMove, float a3);

    // CreateItemReq - 0x1400B0A60
    // Creates items from request with log and DB update
    bool CreateItemReq(ST_CREATE_ITEMS stCreateItem, bool bAddOption,
                       eITEM_CREATE_TYPE eCreateType, ST_LOG_GAME& stLogData);

    // SetTradeConfirm - 0x1400AE760
    // Sets trade confirm item list with socket/broach/package info
    bool SetTradeConfirm(/*ST_TRADE_ITEM_LIST*/ void* stTradeItemList, /*PS_ITEM_SOCKET_LIST*/ void* stSocketList, /*PS_ITEM_BROACH_LIST*/ void* stBroachList, /*PS_ITEM_PACKAGE_LIST*/ void* psPackageList);

    // CanTradeConfirm - 0x1400AEBE0
    // Checks if trade can be confirmed with target item list
    bool CanTradeConfirm(/*ST_TRADE_ITEM_LIST*/ void* stMyItemList, /*ST_TRADE_ITEM_LIST*/ void* stTargetItemList);

    // UpdateTradeUnLock - 0x1400AEE00
    // Unlocks trade items after trade completes
    void UpdateTradeUnLock(/*ST_TRADE_ITEM_LIST*/ void* stTargetItemList);

    // === Batch 6: Item shop and random option functions (IDA verified) ===

    // SendDBLimitItemInfo - 0x1400DBC90
    // Sends item limit info to DB (main=0x21, sub=0x41)
    void SendDBLimitItemInfo();

    // ReduceItemShop - 0x1400DBDF0
    // Reduces item count for shop purchases
    bool ReduceItemShop(TB_ITEM* pTBItem, int nReduceCount, std::uint8_t byLock, PS_RES_STORAGE_INFO* psUpdateItem);

    // ReduceItemCheckShop - 0x1400DBF40
    // Checks and reduces item count for shop
    bool ReduceItemCheckShop(TB_ITEM* pTBItem, int& nReduceCount, std::uint8_t byLock, PS_RES_STORAGE_INFO* psUpdateItem);

    // CheckRandomOption - 0x1400DC0C0
    // Checks and reorders random options
    bool CheckRandomOption(STItem* stOptinChangeInfo);

    // IsRandomItemTitle - 0x1400DCA20
    // Checks if item title is random
    bool IsRandomItemTitle(int nTitleGroupID);

    // === Batch 7: Disassemble log and dye functions (IDA verified) ===

    // AddDisassembleLog - 0x1400DCAA0
    // Adds disassemble log entry by serial
    void AddDisassembleLog(std::int64_t biSerial, ST_CREATE_ITEM stLogDisInfo);

    // GetDisassembleLog - 0x1400DCB30
    // Gets disassemble log entry by serial
    void GetDisassembleLog(std::int64_t biSerial, ST_CREATE_ITEM& stLogDisInfo);

    // ClearDissassembleLog - 0x1400DCBB0
    // Clears all disassemble logs
    void ClearDissassembleLog();

    // CheckDyeInfo - 0x1400DDA40
    // Checks dye info for item dyeing
    bool CheckDyeInfo(int eDyeType, int nID, int nSelectDyeID, int nOriDyeID, int& nDye, /*ST_CREATE_ITEMS*/ void* stNeedItems);

    // IsValidDyeInfo - 0x1400DDF10
    // Validates dye info for item
    bool IsValidDyeInfo(int nItemID, int nDyeID);

    // === Batch 8: Slot validation and item make limit functions (IDA verified) ===

    // IsValidSlotItem - 0x1400DF110
    // Validates if slot item matches expected serial
    bool IsValidSlotItem(std::uint8_t byInvenType, std::int16_t shSlot, STItem* stItem);

    // AddItemEmptySlot - 0x1400DF260
    // Adds overlapped items to empty slots
    void AddItemEmptySlot();

    // SendDBItemMakeLimitInfo - 0x1400DF930
    // Sends item make limit info to DB (main=0x21, sub=0x51)
    void SendDBItemMakeLimitInfo();

    // LoadItemMakeLimitInfo - 0x1400DFAB0
    // Loads item make limit info from DB
    void LoadItemMakeLimitInfo(/*PS_ITEM_MAKE_LIMIT_LIST*/ void* psList);

    // OnInitItemMakeLimit - 0x1400DFD80
    // Initializes item make limit with daily reset
    void OnInitItemMakeLimit();

    // === Batch 9: Item make limit helper functions (IDA verified) ===

    // UpdateItemMakeLimit - 0x1400E02B0
    // Updates item make limit info and sends DB update
    void UpdateItemMakeLimit(/*TB_MAKE*/ void* pTB_Make, std::uint8_t byMakeCount, /*PS_ITEM_MAKE_LIMIT_INFO*/ void* psMakeLimitInfo, /*PS_DB_ITEM_MAKE_LIMIT_UPDATE*/ void* psUpdate);

    // CheckMakeLimitCount - 0x1400E06A0
    // Checks if make limit count allows more crafting
    bool CheckMakeLimitCount(int nMakeIndex, std::uint8_t byMakeCount, /*PS_ITEM_MAKE_LIMIT_INFO*/ void* psInfo);

    // GetMakeLimitEndDate - 0x1400E09C0
    // Gets end date for make limit period
    bool GetMakeLimitEndDate(/*TB_MAKE*/ void* pTB_Make, std::int64_t& biEndDate);

    // IsValidEquipItem - 0x1400E0CB0
    // Validates if item can be equipped in slot
    bool IsValidEquipItem(PS_STORAGE_INFO* psInfo);

    // GetMakeLimitInitDay - 0x1400E0DE0
    // Gets init day offset for make limit reset period
    int GetMakeLimitInitDay(int eType, /*ATL::CTime*/ std::int64_t tCurr);

    // === Batch 10: Dye and common value functions (IDA verified) ===

    // ShowMakeLimitInfo - 0x1400E0FA0
    // Shows make limit info to player via chat notice
    void ShowMakeLimitInfo(int nMakeIndex);

    // SetDyePoint - 0x1400E11F0
    // Sets dye point value and updates CUser::stMyCharInfoEx
    void SetDyePoint(int nDyePoint);

    // AddDyePoint - 0x1400E1270
    // Adds dye points with logging, returns false if overflow
    bool AddDyePoint(int nDyePoint);

    // IsHiddenDye - 0x1400E1430
    // Checks if dye ID is hidden (Hidden_Info == 1)
    bool IsHiddenDye(int nDyeID);

    // GetCommonValue - 0x1400E1480
    // Gets common value from TB_COMMON table
    float GetCommonValue(int nIndex);

    // === Batch 11: Gesture functions (IDA verified) ===

    // CheckAddDyePoint - 0x1400E14D0
    // Checks and calculates dye points to add based on dye type
    bool CheckAddDyePoint(/*E_DYE_TYPE*/ int eDyeType, int& nAddPoint);

    // LoadGesture - 0x1400E1830
    // Loads gesture slot info, validates against costume gestures
    void LoadGesture(/*PS_GESTURE_SLOT*/ void* psGestureInfo);

    // CheckGestureInfo - 0x1400E1E30
    // Validates gesture slot info against user class and costume
    bool CheckGestureInfo(/*PS_GESTURE_SLOT*/ void* psGestureInfo);

    // UpdateGesture - 0x1400E2120
    // Updates gesture slots, removes invalid ones
    void UpdateGesture();

    // IsUseGesture - 0x1400E2460
    // Checks if gesture ID can be used by player
    bool IsUseGesture(int nGestureID);

    // === Batch 12: Equalizer and Item Title functions (IDA verified) ===

    // CheckEqualizerInfo - 0x1400E2A80
    // Validates equalizer ID against costume equalizer info
    bool CheckEqualizerInfo(int nEqualizerID);

    // UpdateEqualizerInfo - 0x1400E2C80
    // Updates equalizer info if current one is invalid
    void UpdateEqualizerInfo();

    // IsReplaceItem - 0x1400E2F80
    // Checks if item belongs to similar item group
    bool IsReplaceItem(int nItemID, unsigned int dwGroupID);

    // ChangeItemTitle - 0x1400E3810
    // Changes item title with success rate calculation
    int ChangeItemTitle(/*PS_REQ_ITEM_TITLE_CHANGE*/ void* psChangeInfo, int& nTitleID, bool& bSuccess, /*PS_RES_STORAGE_INFO*/ void* psUpdateItemList);

    // Note: SetCashMileage already declared earlier at line 923

    // === Socket detach functions (IDA verified) ===

    // DetachSocketItem - 0x1400CED70
    // Detaches socket from equipment item
    int DetachSocketItem(/*PS_REQ_SOCKET_DETACH*/ void* psDetachInfo, /*PS_RES_STORAGE_INFO*/ void* psUpdateItemList, /*PS_RES_STORAGE_INFO*/ void* psCreateItemList, std::int64_t& biSerial);

    // === Item refine functions (IDA verified) ===

    // RefineItemAbility - 0x1400CFB40
    // Refines item ability using materials
    int RefineItemAbility(/*PS_REQ_ITEM_REFINE*/ void* psRefine, int nNpcGroupID, std::uint32_t nNpcID);

    // === WeMade billing functions (IDA verified) ===

    // InitWeMadeBilling - 0x1400CB6D0
    // Initializes WeMade billing system
    void InitWeMadeBilling();

    // === Repurchase functions (IDA verified) ===

    // AddRepurchaseSocketInfo - 0x1400D1DC0
    // Adds socket info to repurchase list
    void AddRepurchaseSocketInfo(/*PS_ITEM_SOCKET_LIST*/ void* psSocketList);

    // AddRepurchaseBroachInfo - 0x1400D1F80
    // Adds broach info to repurchase list
    void AddRepurchaseBroachInfo(/*PS_ITEM_BROACH_LIST*/ void* psBroachList);

    // === Item cooldown functions (IDA verified) ===

    // SendDBItemCooltimeInfo - 0x1400B9610
    // Sends item cooldown info to database (main=0x21, sub=0x46)
    void SendDBItemCooltimeInfo();

    // SendItemCoolTimeInfo - 0x1400B98E0
    // Sends item cooldown info to client (main=8, sub=0x66)
    void SendItemCoolTimeInfo();

    // LoadCoolTime - 0x1400B9770
    // Loads cooldown list from DB response
    void LoadCoolTime(PS_ITEM_COOMTIME_LIST& psCooltimeList);

    // AddCoolTime - 0x1400B9AB0
    // Adds cooldown for group ID, optionally syncs to DB
    void AddCoolTime(std::uint16_t byGroupID, std::uint64_t dwValue, bool bSendDB);

    // GetCoolTime - 0x1400B9E20
    // Gets cooldown remaining time for group ID
    std::uint64_t GetCoolTime(std::uint16_t byGroupID, bool bSave);

    // === Trade password functions (IDA verified) ===

    // CheckTradePasswordState - 0x1400B9F00
    // Checks trade password state, returns error ID if locked
    bool CheckTradePasswordState(int& nErrorID);

    // CheckTradePassword - 0x1400B9F60
    // Validates trade password and sends response
    bool CheckTradePassword(PS_TRADE_PW_REQ& psTrade);

    // IsValidTradePassword - 0x1400BA530
    // Validates password format (4 digits, 0-7, no sequences)
    bool IsValidTradePassword(char* strPassword, int& nErrorID);

    // SendDBCashItemSet - 0x1400B8E30
    // Sends cash item set to DB (main=0x22, sub=0x21)
    void SendDBCashItemSet();

    // === Item use info functions (IDA verified) ===

    // SetItemUseInfoList - 0x1400B7370
    // Sets item use info list from DB response
    void SetItemUseInfoList(ST_USE_ITEM_INFO_LIST& stUseItemInfoList);

    // OnInitItemUseInfoDate - 0x1400B74F0
    // Initializes item use info date, clears expired entries
    void OnInitItemUseInfoDate();

    // CanUseItemInfo - 0x1400B76B0
    // Checks if item use info type can be used (count < 3)
    bool CanUseItemInfo(int nType);

    // === Inventory info functions (IDA verified) ===

    // GetInvenInfo - 0x1400B2120
    // Gets inventory info by type, routes to GetEquipPtr or GetInvenPtr
    void GetInvenInfo(std::uint8_t byInvenType, PS_RES_STORAGE_INFO& stInvenInfo);

    // === Item creation functions (IDA verified) ===

    // LogCreateItemLog - 0x1400B1FA0
    // Sets log subtype based on create type
    void LogCreateItemLog(int nCreateType, ST_LOG_GAME& stLogData);

    // ItemMakeCheat - 0x1400B2430
    // GM cheat to create items with optional upgrade
    bool ItemMakeCheat(int nItemID, std::int16_t sCount, bool bAddOption, std::uint8_t byUpgrade);

protected:
    // === Member variables (from IDA constructor 0x14009F7B0 analysis) ===

    // Currency (from ClearInven 0x1400A0000)
    std::int64_t m_nInvenMoney = 0;      // Inventory money
    std::int64_t m_nBankMoney = 0;       // Bank money
    std::int64_t m_nBP = 0;              // BP points
    std::int64_t m_biEther = 0;          // Ether
    std::int64_t m_nCash = 0;            // Cash
    std::int64_t m_nLimitMonsterBP = 0;  // Monster BP limit
    std::int64_t m_nLimitPVPBP = 0;      // PVP BP limit
    std::int64_t m_biFriendPoint = 0;    // Friend points (from SetInventory)
    std::int64_t m_biRecycle = 0;        // Recycle points (from SetInventory)

    // Constructor 0x14009F7B0 embeds these three equipment objects. Their
    // addresses are indexed by the recovered 0/1/3 equipment-type keys.
    XShapeEquip m_ShapeEquip;
    XAbilityEquip m_AbilityEquip;
    XLookEquip m_LookEquip;
    void* m_CommonInven = nullptr;       // XInventory (type 2)
    void* m_CostumeInven = nullptr;      // XInventory (type 4)
    void* m_CashInven = nullptr;         // XInventory (type 13)
    void* m_CubeInven = nullptr;         // XInventory (type 11)
    void* m_Bank[3] = {nullptr, nullptr, nullptr};           // XBank[3]
    void* m_AccountBank[3] = {nullptr, nullptr, nullptr};    // XBank[3]

    // PDB/IDA confirm the keyed equipment map; the remaining inventory
    // placeholders belong to a separate recovery chain.
    std::map<std::uint8_t, XBaseEquip*> m_mapEquipInfo;

    // Private shop list
    std::list<ST_PRIVATE_SHOP_ITEM> m_liPrivateShopItem;

    // Trade info (from InitTarde, ClearTradeInfo)
    ST_MY_TRADE_INFO m_stTradeInfo;
    UXActorID m_uxTradeActorID = 0;
    std::uint64_t m_dw64TradeTick = 0;
    eTRADE_STATE m_eTradeState = E_TRADE_STATE_NONE;

    // Group cool time map (from InitItemCoolTime)
    std::map<std::uint16_t, std::uint64_t> m_mpGroupCoolTime;

    // Additional members from Init
    std::list<STItem> m_listRepurchaserItem;  // IDA: std::list<STItem>
    PS_ITEM_SOCKET_LIST m_listRepurchaseSocket;  // IDA: PS_ITEM_SOCKET_LIST
    PS_ITEM_BROACH_LIST m_listRepurchaseBroach;  // IDA: PS_ITEM_BROACH_LIST
    int m_nQuickSlotItem[4] = {0};        // Quick slot items
    TB_ITEM_ENDURANCE* m_pEnduranceTable = nullptr;
    std::map<std::int64_t, int> m_mpCashItemDate;
    bool m_bAbsoluteUpgade = false;
    char m_szHanBillNo[64] = {0};
    std::uint8_t m_byTradePassword = 0;
    std::int64_t m_biUseItemUpdateDate = 0;
    std::uint64_t m_dw64UpdateTick = 0;
    int m_nMazeNeedItemID = 0;
    std::map<int, ST_USE_ITEM_INFO> m_mpUseItemInfo;
    std::map<unsigned long, std::int64_t> m_mpAppearanceList;
    char m_stCashSet[9 * 104] = {0};      // 9 PS_CASH_SET objects
    bool m_bProcessBilling = false;
    std::map<int, PS_CASH_BUY_COUNT> m_mpCashBuyCount;
    std::map<std::uint32_t, void*> m_mpEndranceLog;
    int m_nEquipSlot = 0;
    bool m_bReqBroachRemove = false;
    bool m_bLoadCash = false;
    bool m_bReadyLoadCash = false;
    bool m_bReqSocketRemove = false;
    bool m_bReqItemRefine = false;
    bool m_bReqLeagueNameChange = false;
    bool m_bReqSocketExchange = false;
    bool m_bReqSocketUpgrade = false;
    bool m_bReqSocketExtract = false;
    bool m_bReqShopBuy = false;
    bool m_bRenovateItem = false;
    std::map<int, void*> m_mapLimitItemInfo;
    std::int64_t m_tItemInitDate = 0;
    std::map<std::int64_t, ST_CREATE_ITEM> m_mapLogDisassemble;
    std::multimap<std::uint8_t, STItem> m_mpOverlappedSlot;  // IDA: std::multimap<unsigned char,STItem>
    std::map<std::uint16_t, ST_INVENTORY_SLOT_INFO> m_mpSlot;
    std::map<std::uint32_t, void*> m_mpItemMakeLimit;
    std::int64_t m_tMakeInitDate = 0;
    char m_stUseWarpItem[64] = {0};       // PS_ITEM_RENOVATE_COMPLETE
    std::int64_t m_biDropEther = 0;
    int m_nDyePoint = 0;
    int m_nRenovatePoint = 0;
    int m_nRefinePoint = 0;
    std::uint64_t m_dw64WaitTick = 0;
    int m_nCashMileage[3] = {0};          // Cash mileage array
    std::map<std::uint16_t, PS_ITEM_COOLTIME_INFO> m_mpSaveGroupCooltime;

    // Tool-related members (from IDA ClearTool* functions)
    std::vector<void*> m_stToolDisassemble;       // ST_ITEM_PACKAGE_PARTS vector
    PS_RES_TOOL_SOULSTONE m_stToolSoulstone;
    PS_RES_TOOL_DROP_INFO m_stToolItemInfo;
    ST_CREATE_ITEMS m_stToolRandomBoxRes;

    int m_nInventorySize = 0;
    int m_nUsedSlots = 0;
    int m_nCurrentWeight = 0;
    int m_nMaxWeight = 0;
};
