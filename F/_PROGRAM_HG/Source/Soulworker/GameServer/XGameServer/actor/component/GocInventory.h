#pragma once

#include "GOComponent.h"
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
struct ST_PRIVATE_SHOP_LIST;
struct ST_PRIVATE_SHOP_INFO;
class XGameServer;
class XOption;

// Enum for inventory types (from IDA)
enum eINVEN_TYPE : unsigned char {
    E_INVEN_TYPE_ABILITY_EQUIP = 1,
    E_INVEN_TYPE_SHAPE_EQUIP = 2,
    E_INVEN_TYPE_LOOK_EQUIP = 3,
};

// Enum for trade states
enum E_TRADE_STATE {
    E_TRADE_STATE_NONE = 0,
};

// Nation type enum
enum NATION_TYPE {
    NATION_TYPE_JPN = 1,
};

// Simple placeholder structs for compilation
struct ST_PRIVATE_SHOP_ITEM {
    std::shared_ptr<CItem> pItem;
    std::int64_t biMoney = 0;
};

struct ST_MY_TRADE_INFO {
    std::int64_t biMoney = 0;
    std::list<int> listInfo;
};

struct ST_USE_ITEM_INFO {
    int nItemID = 0;
};

struct ST_BOOSTER_INFO {
    int nData = 0;
};

struct ST_USER_LAST_RANKING_INFO {
    int nData = 0;
};

struct ST_LOG_GAME {
    int _nUAID = 0;
    int _nUCID = 0;
    short _sMainType = 0;
    short _sSubType = 0;
    int nParam1 = 0;
    int nParam2 = 0;
    int nParam3 = 0;
    int nParam4 = 0;
    int nParam5 = 0;
    int nParam6 = 0;
    wchar_t szComment[51] = {0};
};

// Forward declare UXActorID as uint64_t for now
using UXActorID = std::uint64_t;

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
    void SetInventory(std::uint8_t byCommon, std::uint8_t byCostume, std::uint8_t byCash,
                      std::uint8_t byCube, std::int64_t nMoney, std::int64_t nBankMoney,
                      std::int64_t nBP, std::int64_t nEther);

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

    // SendMoney - 0x1400A2D70 / 0x1400A2E60
    void SendMoney();

    // SetBP - 0x1400A2F30
    // IDA: void __fastcall CGocInventory::SetBP(CGocInventory *this, __int64 nBP, bool bSend)
    void SetBP(std::int64_t nBP, bool bSend = false);

    // AddBP - 0x1400A3000
    bool AddBP(std::int64_t nBP, std::uint8_t byLogType);

    // InitLimitBP - 0x1400A2FA0
    void InitLimitBP();

    // SetLimitBP - 0x1400A2FD0
    void SetLimitBP(int nLimitMonsterBP, int nLimitPVPBP);

    // SetEther - 0x1400A3CF0
    // IDA: void __fastcall CGocInventory::SetEther(CGocInventory *this, __int64 biEther, bool bSend)
    void SetEther(std::int64_t biEther, bool bSend = false);

    // AddEther - 0x1400A3D60
    bool AddEther(std::int64_t biEther, std::uint8_t byLogType, bool bLog);

    // SendEther - 0x1400A4450
    void SendEther(std::int64_t biResultEther);

    // SetMileage - 0x1400A0A40
    void SetMileage(int nDyePoint, int nRenovatePoint, int nRefinePoint);

    // SetTotalFriendPoint - 0x1400A4BF0
    void SetTotalFriendPoint(std::int64_t nPoint, bool bSend = false);

    // AddTotalFriendPoint - 0x1400A4C80
    bool AddTotalFriendPoint(std::int64_t nPoint, bool bSendDB);

    // SendTotalFriendPoint - 0x1400A4E30
    void SendTotalFriendPoint();

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

    // === Equipment functions ===
    int GetEquippedItem(int nEquipSlot) const;
    bool EquipItem(int nSlotIndex, int nEquipSlot);
    bool UnequipItem(int nEquipSlot);

    // === Inventory operations ===
    void SortInventory();

    // SetEquipItem - 0x1400A1380
    void SetEquipItem(/*PS_RES_STORAGE_INFO*/ void* pInfo, int nIndex);

    // === Weight/encumbrance ===
    int GetCurrentWeight() const;
    int GetMaxWeight() const;
    bool IsOverencumbered() const;

    // === Quest items ===
    bool IsQuestItem(int nItemId) const;
    int GetQuestItemCount() const;

    // === Static functions ===

    // GetFamilyID - 0x1400262C0
    static int GetFamilyID();

    // === Additional helper functions (from IDA) ===

    // SetTradeState - 0x1400A0070
    void SetTradeState(E_TRADE_STATE eState);

    // SetTradeActorID - 0x1400A0078
    void SetTradeActorID(UXActorID actorID);

    // UpdateTradeUnLock - 0x1400A0080
    void UpdateTradeUnLock();

    // SetLock - 0x1400A2100
    void SetLock(std::uint8_t byInvenType, int nSlot, std::uint8_t byLock);

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

    // Inventory components (from constructor 0x14009F7B0)
    // These are the actual member objects - using void* placeholders for now
    // since we don't have full definitions of XShapeEquip, XAbilityEquip, etc.
    void* m_ShapeEquip = nullptr;        // XShapeEquip
    void* m_AbilityEquip = nullptr;      // XAbilityEquip
    void* m_LookEquip = nullptr;         // XLookEquip
    void* m_CommonInven = nullptr;       // XInventory (type 2)
    void* m_CostumeInven = nullptr;      // XInventory (type 4)
    void* m_CashInven = nullptr;         // XInventory (type 13)
    void* m_CubeInven = nullptr;         // XInventory (type 11)
    void* m_Bank[3] = {nullptr, nullptr, nullptr};           // XBank[3]
    void* m_AccountBank[3] = {nullptr, nullptr, nullptr};    // XBank[3]

    // Equip info map (from constructor)
    std::map<std::uint8_t, void*> m_mapEquipInfo;

    // Private shop list
    std::list<ST_PRIVATE_SHOP_ITEM> m_liPrivateShopItem;

    // Trade info (from InitTarde, ClearTradeInfo)
    ST_MY_TRADE_INFO m_stTradeInfo;
    UXActorID m_uxTradeActorID = 0;
    std::uint64_t m_dw64TradeTick = 0;

    // Group cool time map (from InitItemCoolTime)
    std::map<int, void*> m_mpGroupCoolTime;

    // Additional members from Init
    std::list<int> m_listRepurchaserItem;
    std::vector<int> m_listRepurchaseSocket;
    std::vector<int> m_listRepurchaseBroach;
    int m_nQuickSlotItem[10] = {0};       // Quick slot items
    void* m_pEnduranceTable = nullptr;
    std::map<int, void*> m_mpCashItemDate;
    bool m_bAbsoluteUpgade = false;
    char m_szHanBillNo[64] = {0};
    std::uint8_t m_byTradePassword = 0;
    std::int64_t m_biUseItemUpdateDate = 0;
    std::uint64_t m_dw64UpdateTick = 0;
    int m_nMazeNeedItemID = 0;
    std::map<std::uint32_t, void*> m_mpUseItemInfo;
    std::map<int, void*> m_mpAppearanceList;
    char m_stCashSet[9 * 104] = {0};      // 9 PS_CASH_SET objects
    bool m_bProcessBilling = false;
    std::map<std::uint32_t, void*> m_mpCashBuyCount;
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
    std::map<int, void*> m_mapLogDisassemble;
    std::map<std::uint16_t, ST_USER_LAST_RANKING_INFO> m_mpOverlappedSlot;
    std::map<std::uint16_t, ST_USER_LAST_RANKING_INFO> m_mpSlot;
    std::map<std::uint32_t, void*> m_mpItemMakeLimit;
    std::int64_t m_tMakeInitDate = 0;
    char m_stUseWarpItem[64] = {0};       // PS_ITEM_RENOVATE_COMPLETE
    std::int64_t m_biDropEther = 0;
    int m_nDyePoint = 0;
    int m_nRenovatePoint = 0;
    int m_nRefinePoint = 0;
    std::uint64_t m_dw64WaitTick = 0;
    int m_nCashMileage[4] = {0};          // Cash mileage array
    std::map<std::uint32_t, void*> m_mpSaveGroupCooltime;

    int m_nInventorySize = 0;
    int m_nUsedSlots = 0;
    int m_nCurrentWeight = 0;
    int m_nMaxWeight = 0;
};
