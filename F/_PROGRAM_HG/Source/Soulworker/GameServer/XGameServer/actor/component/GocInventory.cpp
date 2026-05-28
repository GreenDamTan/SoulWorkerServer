#include "GocInventory.h"

// ============================================================================
// CGocInventory - Game Object Component for actor inventory
//
// IDA Evidence from GameServer.exe (port 10004):
// - Constructor: 0x14009F7B0 - Initializes all inventory components
// - Destructor: 0x14009FD40 - Destructs all inventory components
// - Init: 0x1400A00C0 - Initializes inventories, banks, clears state
// - ClearInven: 0x1400A0000 - Clears money, BP, ether, cash
// - GetMoney: 0x140026700 - Returns m_nInvenMoney
// - SetInvenMoney: 0x1400A2340 - Sets money and updates CUser
// - AddMoney: 0x1400A24C0 - Adds money with logging
// - GetInvenPtr: 0x1400A2170 - Returns inventory by type
// - GetTBInvenPtr: 0x1400A2260 - Returns TB inventory by type
// - GetFamilyID: 0x1400262C0 - Returns 7
// ============================================================================

CGocInventory::CGocInventory()
    : GOComponent()
    , m_nInvenMoney(0)
    , m_nBankMoney(0)
    , m_nBP(0)
    , m_biEther(0)
    , m_nCash(0)
    , m_nLimitMonsterBP(0)
    , m_nLimitPVPBP(0)
    , m_biFriendPoint(0)
    , m_biRecycle(0)
    , m_nInventorySize(0)
    , m_nUsedSlots(0)
    , m_nCurrentWeight(0)
    , m_nMaxWeight(0)
{
    // Constructor 0x14009F7B0 initializes:
    // - GOComponent base
    // - m_ShapeEquip, m_AbilityEquip, m_LookEquip (XShapeEquip, XAbilityEquip, XLookEquip)
    // - m_CommonInven, m_CostumeInven, m_CashInven, m_CubeInven (XInventory)
    // - m_Bank[3], m_AccountBank[3] (XBank arrays)
    // - m_mapEquipInfo with mappings: 1->m_AbilityEquip, 2->m_ShapeEquip, 3->m_LookEquip
    // - m_liPrivateShopItem (empty list)
    // - m_stTradeInfo, m_uxTradeActorID

    // Initialize equip info map per IDA
    m_mapEquipInfo[E_INVEN_TYPE_ABILITY_EQUIP] = m_AbilityEquip;
    m_mapEquipInfo[E_INVEN_TYPE_SHAPE_EQUIP] = m_ShapeEquip;
    m_mapEquipInfo[E_INVEN_TYPE_LOOK_EQUIP] = m_LookEquip;
}

CGocInventory::~CGocInventory() {
    // Destructor 0x14009FD40 - destructs all members in reverse order
    // Note: Using void* placeholders, actual destructors would be called for:
    // - m_mpSlot, m_mpItemMakeLimit, m_mpOverlappedSlot, m_mapLogDisassemble
    // - m_mapLimitItemInfo, m_mpEndranceLog, m_mpCashBuyCount, m_mpAppearanceList
    // - m_mpCashItemDate, m_mpUseItemInfo, m_mpSaveGroupCooltime, m_mpGroupCoolTime
    // - m_listRepurchaseBroach, m_listRepurchaseSocket, m_listRepurchaserItem
    // - m_stTradeInfo, m_mapEquipInfo
    // - m_AccountBank[3], m_Bank[3]
    // - m_CubeInven, m_CashInven, m_CostumeInven, m_CommonInven
    // - m_LookEquip, m_AbilityEquip, m_ShapeEquip
    // - m_liPrivateShopItem

    m_liPrivateShopItem.clear();
    m_mapEquipInfo.clear();
}

bool CGocInventory::Initialize() {
    return true;
}

void CGocInventory::Shutdown() {
}

void CGocInventory::Update(float fDeltaTime) {
    (void)fDeltaTime;
}

// ============================================================================
// Currency functions (IDA verified)
// ============================================================================

// IDA: 0x140026700
// __int64 __fastcall CGocInventory::GetMoney(CGocInventory *this)
// {
//   return this->m_nInvenMoney;
// }
std::int64_t CGocInventory::GetMoney() const {
    return m_nInvenMoney;
}

// IDA: 0x1400A2340
// void __fastcall CGocInventory::SetInvenMoney(CGocInventory *this, __int64 nMoney, bool bSend)
// {
//   VChunkFile *v3; // rax
//   CUser *v4; // rax
//   this->m_nInvenMoney = nMoney;
//   v3 = std::list<CBattleZone *>::size((VChunkLocker *)this);
//   v4 = (CUser *)_RTDynamicCast_0(v3, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
//   CUser::stMyCharInfoEx(v4)->biMoney = this->m_nInvenMoney;
// }
void CGocInventory::SetInvenMoney(std::int64_t nMoney, bool bSend) {
    m_nInvenMoney = nMoney;
    // TODO: Get CUser from GOComponent hierarchy and update stMyCharInfoEx()->biMoney
    // This requires accessing the actor/owner of this component
    // VChunkFile* v3 = ...;
    // CUser* v4 = dynamic_cast<CUser*>(...);
    // if (v4) CUser::stMyCharInfoEx(v4)->biMoney = m_nInvenMoney;
    (void)bSend; // Parameter is unused in IDA
}

// IDA: 0x1400A24C0
// bool __fastcall CGocInventory::AddMoney(CGocInventory *this, __int64 biMoney,
//                                          unsigned __int8 byLogType, int nLogValue1,
//                                          int LogValue2, bool bLog)
// Adds money with logging and DB update
bool CGocInventory::AddMoney(std::int64_t biMoney, std::uint8_t byLogType,
                              int nLogValue1, int LogValue2, bool bLog) {
    // Check overflow (IDA: biMoney + this->m_nInvenMoney < 0)
    if (biMoney + m_nInvenMoney < 0)
        return false;

    // Set new money value (IDA: CGocInventory::SetInvenMoney(this, biMoney + this->m_nInvenMoney, 0))
    SetInvenMoney(biMoney + m_nInvenMoney, false);

    // IDA: Send PS_DB_GOLD_UPDATE packet to DB
    // PS_DB_GOLD_UPDATE psGold;
    // psGold.dwActorID = GetActorID();
    // psGold.nAddGold = biMoney;
    // psGold.nTotalGold = m_nInvenMoney;
    // psGold.nBonus = 0;
    // psGold.byType = 0;
    // XSendDBPacket xSendDBPacket(pObject, 3, 0x31);
    // xSendDBPacket << psGold;
    // XGameServer::SendDBGame(&xSendDBPacket);

    // IDA: Update money supply
    // XGameServer::SetMoneySupply(TXSingleton<XGameServer>::Instance(), biMoney);

    // IDA: Set log money for CUser
    // CUser* pUser = GetCUser();
    // if (pUser) CUser::SetLogMoney(pUser, biMoney);

    // IDA: Send game log if !bLog
    // if (!bLog) {
    //     ST_LOG_GAME stLog;
    //     stLog._nUAID = pUser->GetUAID();
    //     stLog._nUCID = GetUCID();
    //     stLog._sMainType = 10;
    //     stLog._sSubType = 1;
    //     stLog.nParam1 = GetClass();
    //     stLog.nParam2 = byLogType;
    //     stLog.nParam3 = nLogValue1;
    //     stLog.nParam4 = LogValue2;
    //     stLog.nParam5 = biMoney;
    //     stLog.nParam6 = m_nInvenMoney;
    //     wcscpy_s(stLog.szComment, L"MONEY");
    //     XGameServer::SendDBLog(&stLog);
    // }

    (void)byLogType;
    (void)nLogValue1;
    (void)LogValue2;
    (void)bLog;

    return true;
}

// IDA: 0x1400279C0
std::int64_t CGocInventory::GetEther() const {
    return m_biEther;
}

// IDA: 0x1400279E0
std::int64_t CGocInventory::GetBP() const {
    return m_nBP;
}

// IDA: 0x140027A00
bool CGocInventory::IsUseMoney(std::int64_t nAmount) const {
    return m_nInvenMoney >= nAmount;
}

// IDA: 0x1400278B0
bool CGocInventory::AddBindMoney(std::int64_t nAmount, std::uint8_t byType,
                                   int nParam1, int nParam2, bool bLog) {
    // TODO: Implement per IDA
    (void)nAmount;
    (void)byType;
    (void)nParam1;
    (void)nParam2;
    (void)bLog;
    return false;
}

// ============================================================================
// Initialization and cleanup (IDA verified)
// ============================================================================

// IDA: 0x1400A0000
// void __fastcall CGocInventory::ClearInven(CGocInventory *this)
// {
//   this->m_nInvenMoney = 0;
//   this->m_nBankMoney = 0;
//   this->m_nBP = 0;
//   this->m_biEther = 0;
//   this->m_nCash = 0;
//   this->m_nLimitMonsterBP = 0;
//   this->m_nLimitPVPBP = 0;
// }
void CGocInventory::ClearInven() {
    m_nInvenMoney = 0;
    m_nBankMoney = 0;
    m_nBP = 0;
    m_biEther = 0;
    m_nCash = 0;
    m_nLimitMonsterBP = 0;
    m_nLimitPVPBP = 0;
}

// IDA: 0x1400A00C0 - Comprehensive initialization
bool CGocInventory::Init() {
    // Clear currency values
    ClearInven();

    // Initialize trade state
    InitTarde();

    // Clear trade info
    ClearTradeInfo();

    // Set trade state to none
    SetTradeState(E_TRADE_STATE_NONE);

    // Set trade actor ID to default (0)
    SetTradeActorID(0);

    // Initialize equip components (per IDA)
    // m_ShapeEquip.Init()
    // m_AbilityEquip.Init()
    // m_LookEquip.Init()

    // Initialize inventories with their type IDs (per IDA)
    // m_CommonInven.Init(2)
    // m_CostumeInven.Init(4)
    // m_CashInven.Init(13)
    // m_CubeInven.Init(11)

    // Initialize banks based on nation type (per IDA)
    // Note: For JPN nation, banks use types 5,6,14; otherwise 16,17,18
    // For now, use non-JPN defaults
    // m_Bank[0].Init(16)
    // m_Bank[1].Init(17)
    // m_Bank[2].Init(18)
    // m_AccountBank[0].Init(16)
    // m_AccountBank[1].Init(17)
    // m_AccountBank[2].Init(18)

    // Clear lists (per IDA)
    m_listRepurchaserItem.clear();
    m_listRepurchaseSocket.clear();
    m_listRepurchaseBroach.clear();

    // Initialize item cool time
    InitItemCoolTime();

    // Clear quick slot items (per IDA)
    std::memset(m_nQuickSlotItem, 0, sizeof(m_nQuickSlotItem));

    // Reset additional state (per IDA)
    m_pEnduranceTable = nullptr;
    m_mpCashItemDate.clear();
    m_bAbsoluteUpgade = false;
    m_szHanBillNo[0] = '\0';
    m_byTradePassword = 0;
    m_biUseItemUpdateDate = 0;
    m_dw64UpdateTick = 0;
    m_biFriendPoint = 0;
    m_nMazeNeedItemID = 0;
    m_mpUseItemInfo.clear();
    m_mpAppearanceList.clear();
    std::memset(m_stCashSet, 0, sizeof(m_stCashSet));
    m_bProcessBilling = false;
    m_mpCashBuyCount.clear();
    m_mpEndranceLog.clear();
    m_nEquipSlot = 0;
    m_liPrivateShopItem.clear();
    m_dw64TradeTick = 0;
    m_bReqBroachRemove = false;
    m_bLoadCash = false;
    m_bReadyLoadCash = false;
    m_bReqSocketRemove = false;
    m_bReqItemRefine = false;
    m_bReqLeagueNameChange = false;
    m_bReqSocketExchange = false;
    m_bReqSocketUpgrade = false;
    m_bReqSocketExtract = false;
    m_bReqShopBuy = false;
    m_bRenovateItem = false;
    m_mapLimitItemInfo.clear();
    m_tItemInitDate = 0;
    m_mapLogDisassemble.clear();
    m_mpOverlappedSlot.clear();
    m_mpSlot.clear();
    m_mpItemMakeLimit.clear();
    m_tMakeInitDate = 0;
    std::memset(&m_stUseWarpItem, 0, sizeof(m_stUseWarpItem));
    m_biDropEther = 0;
    m_nDyePoint = 0;
    m_nRenovatePoint = 0;
    m_nRefinePoint = 0;
    m_dw64WaitTick = 0;
    std::memset(m_nCashMileage, 0, sizeof(m_nCashMileage));
    m_mpSaveGroupCooltime.clear();

    return true;
}

// IDA: 0x1400A0080
// void __fastcall CGocInventory::ClearTradeInfo(CGocInventory *this)
// {
//   this->m_stTradeInfo.biMoney = 0;
//   std::list<unsigned long>::clear(&this->m_stTradeInfo.listInfo);
// }
void CGocInventory::ClearTradeInfo() {
    m_stTradeInfo.biMoney = 0;
    m_stTradeInfo.listInfo.clear();
}

// IDA: 0x1400A0850
// Sets trade state, actor ID, unlocks trade, and resets tick
void CGocInventory::InitTarde() {
    SetTradeState(E_TRADE_STATE_NONE);
    SetTradeActorID(0);
    UpdateTradeUnLock();
    m_dw64TradeTick = 0;
}

// IDA: 0x1400A08B0
// Clears the group cool time map
void CGocInventory::InitItemCoolTime() {
    m_mpGroupCoolTime.clear();
}

// IDA: 0x1400A08E0
// void __fastcall CGocInventory::SetInventory(
//         CGocInventory *this,
//         unsigned __int8 byCommonStep,
//         unsigned __int8 byConsumeStep,
//         unsigned __int8 byCustume,
//         unsigned __int8 byCube,
//         __int64 biMoney,
//         __int64 biBP,
//         __int64 biEther,
//         __int64 biFriendPoint,
//         __int64 biRecycle)
// {
//   this->m_CommonInven.InitExtendStep(&this->m_CommonInven, byCommonStep, 36, 0);
//   this->m_CostumeInven.InitExtendStep(&this->m_CostumeInven, byCustume, 48, 2);
//   this->m_CashInven.InitExtendStep(&this->m_CashInven, 0, 384, 13);
//   this->m_CubeInven.InitExtendStep(&this->m_CubeInven, byCube, 48, 11);
//   this->m_nInvenMoney = biMoney;
//   this->m_nBP = biBP;
//   this->m_biEther = biEther;
//   this->m_biFriendPoint = biFriendPoint;
//   this->m_biRecycle = biRecycle;
// }
void CGocInventory::SetInventory(std::uint8_t byCommon, std::uint8_t byCostume,
                                  std::uint8_t byCash, std::uint8_t byCube,
                                  std::int64_t nMoney, std::int64_t nBankMoney,
                                  std::int64_t nBP, std::int64_t nEther) {
    // Initialize inventory step sizes (per IDA)
    // m_CommonInven.InitExtendStep(byCommon, 36, 0)
    // m_CostumeInven.InitExtendStep(byCostume, 48, 2)
    // m_CashInven.InitExtendStep(0, 384, 13)
    // m_CubeInven.InitExtendStep(byCube, 48, 11)

    // Set money values (per IDA)
    m_nInvenMoney = nMoney;
    m_nBP = nBP;
    m_biEther = nEther;
    // Note: IDA signature has additional params biFriendPoint, biRecycle
    // but current signature doesn't include them

    (void)byCommon;
    (void)byCostume;
    (void)byCash;
    (void)byCube;
    (void)nBankMoney;  // Note: IDA signature doesn't use this param
}

// ============================================================================
// Inventory management
// ============================================================================

// IDA: 0x1400A2170
// Returns inventory pointer by type
// switch (byInvenType):
//   case 2: return &m_CommonInven
//   case 4: return &m_CostumeInven
//   case 5/0x10: return &m_Bank[0]
//   case 6/0x11: return &m_Bank[1]
//   case 0xB: return &m_CubeInven
//   case 0xD: return &m_CashInven
//   case 0xE/0x12: return &m_Bank[2]
//   default: return nullptr
XBaseInventory* CGocInventory::GetInvenPtr(std::uint8_t byInvenType) {
    switch (byInvenType) {
        case 2:
            return static_cast<XBaseInventory*>(m_CommonInven);
        case 4:
            return static_cast<XBaseInventory*>(m_CostumeInven);
        case 5:
        case 0x10:
            return static_cast<XBaseInventory*>(m_Bank[0]);
        case 6:
        case 0x11:
            return static_cast<XBaseInventory*>(m_Bank[1]);
        case 0xB:  // 11
            return static_cast<XBaseInventory*>(m_CubeInven);
        case 0xD:  // 13
            return static_cast<XBaseInventory*>(m_CashInven);
        case 0xE:  // 14
        case 0x12: // 18
            return static_cast<XBaseInventory*>(m_Bank[2]);
        default:
            return nullptr;
    }
}

// IDA: 0x1400A2260
// XInventory *__fastcall CGocInventory::GetTBInvenPtr(CGocInventory *this, unsigned __int8 byTBInvenType)
// {
//   switch ( byTBInvenType )
//   {
//     case 0u: return &this->m_CommonInven;
//     case 2u: return &this->m_CostumeInven;
//     case 9u: return &this->m_CubeInven;
//     case 0xDu: return &this->m_CashInven;
//   }
//   return nullptr;
// }
XBaseInventory* CGocInventory::GetTBInvenPtr(std::uint8_t byTBInvenType) {
    switch (byTBInvenType) {
        case 0:
            return static_cast<XBaseInventory*>(m_CommonInven);
        case 2:
            return static_cast<XBaseInventory*>(m_CostumeInven);
        case 9:
            return static_cast<XBaseInventory*>(m_CubeInven);
        case 0xD:  // 13
            return static_cast<XBaseInventory*>(m_CashInven);
        default:
            return nullptr;
    }
}

// IDA: 0x1400A2340 (different overload)
XBaseEquip* CGocInventory::GetEquipPtr(std::uint8_t byEquipType) {
    // TODO: Implement per IDA
    (void)byEquipType;
    return nullptr;
}

// IDA: 0x140068290
int CGocInventory::GetSimpleEmptySlotCount() const {
    // TODO: Implement per IDA
    return m_nInventorySize - m_nUsedSlots;
}

// ============================================================================
// Private Shop functions (IDA verified)
// ============================================================================

// IDA: 0x1400B0D80
// Adds item to private shop list (max 5 items)
// Complex function that:
// 1. Iterates existing items to check if same item exists
// 2. If exists, updates money and returns true with bExist=true
// 3. If shop full (>=5 items), returns false
// 4. Otherwise adds new item, sets lock on item, returns true
bool CGocInventory::AddPrivateShopItem(std::shared_ptr<CItem> pItem,
                                        std::int64_t biMoney, bool& bExist) {
    bExist = false;

    // Check if item already exists in shop list (per IDA)
    for (auto& shopItem : m_liPrivateShopItem) {
        // Compare item pointers (simplified - IDA uses item size comparison)
        if (shopItem.pItem && shopItem.pItem == pItem) {
            // Item exists, update money
            shopItem.biMoney = biMoney;
            bExist = true;
            return true;
        }
    }

    // Check if shop has room (max 5 items per IDA)
    if (m_liPrivateShopItem.size() >= 5) {
        return false;
    }

    // Add item to list (per IDA)
    ST_PRIVATE_SHOP_ITEM newItem;
    newItem.pItem = pItem;
    newItem.biMoney = biMoney;
    m_liPrivateShopItem.push_back(newItem);

    // Set lock on item (per IDA: SetLock(InvenType, Slot, 0xE))
    // Note: Would need CItem::GetSlot() and CItem::GetInvenType() for full impl
    // CGocInventory::SetLock(this, InvenType, Slot, 0xE);

    return true;
}

// IDA: 0x1400B1000
// Removes item from shop list, unlocks item
bool CGocInventory::DelPrivateShopItem(std::shared_ptr<CItem> pItem) {
    // Find item in list (per IDA)
    for (auto it = m_liPrivateShopItem.begin(); it != m_liPrivateShopItem.end(); ++it) {
        if (it->pItem && it->pItem == pItem) {
            // Found item, unlock it (per IDA: SetLock(InvenType, Slot, 0))
            // CGocInventory::SetLock(this, InvenType, Slot, 0);

            // Remove from list
            m_liPrivateShopItem.erase(it);
            return true;
        }
    }
    return false;
}

// IDA: 0x1400B11D0
// Populates ST_PRIVATE_SHOP_LIST with shop items
void CGocInventory::PrivateShopItemList(ST_PRIVATE_SHOP_LIST& stList) {
    // Iterate m_liPrivateShopItem and populate stList (per IDA)
    // Note: Would need ST_PRIVATE_SHOP_INFO and STItem for full impl
    (void)stList;

    // Per IDA: for each item in m_liPrivateShopItem:
    //   ST_PRIVATE_SHOP_INFO stInfo;
    //   stInfo.biMoney = shopItem.biMoney;
    //   stInfo.stItem = CItem::GetItem(shopItem.pItem);
    //   stList.vecInfo.push_back(stInfo);
}

// IDA: 0x1400B1330
// Clears shop list, unlocks all items first
void CGocInventory::ClearPrivateShopList() {
    // Unlock all items first (per IDA)
    for (auto& shopItem : m_liPrivateShopItem) {
        if (shopItem.pItem) {
            // Per IDA: SetLock(InvenType, Slot, 0)
            // int Slot = CItem::GetSlot(shopItem.pItem);
            // std::uint8_t InvenType = CItem::GetInvenType(shopItem.pItem);
            // SetLock(InvenType, Slot, 0);
        }
    }

    // Clear the list
    m_liPrivateShopItem.clear();
}

// ============================================================================
// Trade helper functions (from IDA)
// ============================================================================

// IDA: 0x1400A0070
void CGocInventory::SetTradeState(E_TRADE_STATE eState) {
    // TODO: Implement trade state tracking
    (void)eState;
}

// IDA: 0x1400A0078
void CGocInventory::SetTradeActorID(UXActorID actorID) {
    m_uxTradeActorID = actorID;
}

// IDA: 0x1400A0080
void CGocInventory::UpdateTradeUnLock() {
    // TODO: Implement trade unlock update per IDA
}

// IDA: 0x1400A2100
// void __fastcall CGocInventory::SetLock(
//         CGocInventory *this,
//         unsigned __int8 byInvenType,
//         unsigned __int16 shSlotPos,
//         unsigned __int8 byFlag)
// Switch on byInvenType:
//   case 0,1,3: GetEquipPtr(byInvenType)->SetLock(shSlotPos, byFlag)
//   case 2,4,5,6,0xB,0xD,0xE,0x10,0x11,0x12: GetInvenPtr(byInvenType)->SetLock(shSlotPos, byFlag)
void CGocInventory::SetLock(std::uint8_t byInvenType, int nSlot, std::uint8_t byLock) {
    switch (byInvenType) {
        case 0:
        case 1:
        case 3: {
            // Equipment types - use GetEquipPtr
            // XBaseEquip* pEquip = GetEquipPtr(byInvenType);
            // if (pEquip) pEquip->SetLock(nSlot, byLock);
            break;
        }
        case 2:
        case 4:
        case 5:
        case 6:
        case 0xB:  // 11
        case 0xD:  // 13
        case 0xE:  // 14
        case 0x10: // 16
        case 0x11: // 17
        case 0x12: // 18
        {
            // Inventory types - use GetInvenPtr
            // XBank* pInven = GetInvenPtr(byInvenType);
            // if (pInven) pInven->SetLock(nSlot, byLock);
            break;
        }
        default:
            break;
    }
    (void)nSlot;
    (void)byLock;
}

// ============================================================================
// Money/Bank operations
// ============================================================================

// IDA: 0x1400A23B0
// void __fastcall CGocInventory::SetBankMoney(CGocInventory *this, __int64 nMoney, bool bSend)
// {
//   this->m_nBankMoney = nMoney;
// }
void CGocInventory::SetBankMoney(std::int64_t nMoney, bool bSend) {
    m_nBankMoney = nMoney;
    // Note: IDA shows bSend parameter is unused
    (void)bSend;
}

// IDA: 0x1400A23E0
// void __fastcall CGocInventory::SendBankMoney(CGocInventory *this)
// Sends PS_GOLD_UPDATE packet with bank money (main=8, sub=0x21)
void CGocInventory::SendBankMoney() {
    // IDA: XSendPacket::XSendPacket(&xSendPacket, 8u, 0x21u)
    // XParse::operator<<(&xSendPacket.XParse, this->m_nBankMoney)
    // CGocNetwork::Send(pActor, &xSendPacket)

    // TODO: Implement packet sending
    // XSendPacket xSendPacket(8, 0x21);
    // xSendPacket << m_nBankMoney;
    // GetActor()->Send(&xSendPacket);
}

// IDA: 0x1400A2D70
// void __fastcall CGocInventory::SendMoney(CGocInventory *this)
// Sends PS_GOLD_UPDATE packet with inventory money (main=8, sub=0x20)
void CGocInventory::SendMoney() {
    // IDA: PS_GOLD_UPDATE stGold;
    // stGold.biTotalMoney = this->m_nInvenMoney;
    // stGold.nAddBonusMoney = 0;
    // stGold.byType = 0;
    // XSendPacket::XSendPacket(&xSendPacket, 8u, 0x20u);
    // operator<<(&xSendPacket, &stGold);
    // CGocNetwork::Send(pActor, &xSendPacket);

    // TODO: Implement packet sending
    // PS_GOLD_UPDATE stGold;
    // stGold.biTotalMoney = m_nInvenMoney;
    // stGold.nAddBonusMoney = 0;
    // stGold.byType = 0;
    // XSendPacket xSendPacket(8, 0x20);
    // xSendPacket << stGold;
    // GetActor()->Send(&xSendPacket);
}

// IDA: 0x1400A2890
bool CGocInventory::AddDropMoney(std::int64_t nMoney, int nType,
                                  std::int64_t& nAddMoney,
                                  std::uint8_t byLogType, int nParam1, int nParam2) {
    // TODO: Implement per IDA
    (void)nMoney;
    (void)nType;
    (void)nAddMoney;
    (void)byLogType;
    (void)nParam1;
    (void)nParam2;
    return false;
}

// ============================================================================
// Item functions (stub implementations)
// ============================================================================

int CGocInventory::GetInventorySize() const {
    return m_nInventorySize;
}

int CGocInventory::GetUsedSlots() const {
    return m_nUsedSlots;
}

int CGocInventory::GetFreeSlots() const {
    return m_nInventorySize - m_nUsedSlots;
}

bool CGocInventory::HasItem(int nItemId) const {
    // TODO: Implement item lookup
    (void)nItemId;
    return false;
}

int CGocInventory::GetItemCount(int nItemId) const {
    // TODO: Implement item count lookup
    (void)nItemId;
    return 0;
}

bool CGocInventory::AddItem(int nItemId, int nCount) {
    // TODO: Implement item addition
    (void)nItemId;
    (void)nCount;
    return false;
}

bool CGocInventory::RemoveItem(int nItemId, int nCount) {
    // TODO: Implement item removal
    (void)nItemId;
    (void)nCount;
    return false;
}

bool CGocInventory::MoveItem(int nFromSlot, int nToSlot) {
    // TODO: Implement item movement
    (void)nFromSlot;
    (void)nToSlot;
    return false;
}

// ============================================================================
// Equipment functions (IDA verified)
// ============================================================================

int CGocInventory::GetEquippedItem(int nEquipSlot) const {
    // TODO: Implement equipped item lookup
    (void)nEquipSlot;
    return 0;
}

bool CGocInventory::EquipItem(int nSlotIndex, int nEquipSlot) {
    // TODO: Implement item equipping per IDA 0x1400A4960 (Equip function)
    (void)nSlotIndex;
    (void)nEquipSlot;
    return false;
}

// IDA: 0x1400A5B10 - CGocInventory::Unequip
// void __fastcall CGocInventory::Unequip(CGocInventory *this, unsigned __int8 byInvenType, __int16 shSlot)
// Handles unequipping from different equipment types:
//   byInvenType 0 = Shape equip (stShapeEquipItemInfo)
//   byInvenType 1 = Ability equip
//   byInvenType 3 = Look equip (stLookEquipIemInfo)
// Updates CUser::stMyCharInfoEx equipment info arrays and handles set items
bool CGocInventory::UnequipItem(int nEquipSlot) {
    // Note: The full IDA function takes byInvenType and shSlot as parameters
    // This is a simplified wrapper - the actual signature should be:
    // void Unequip(uint8_t byInvenType, int16_t shSlot)

    // TODO: Implement full logic per IDA 0x1400A5B10:
    // 1. GetEquipPtr(byInvenType) to get equipment container
    // 2. XBaseEquip::GetSlotInfo(pEquip, &pItem, shSlot) to get item
    // 3. If byInvenType != 3 (look equip), handle set item count:
    //    - Get Item_SetItem_ID from item table
    //    - Call XBaseEquip::GetSetItemCount()
    //    - Call item's unequip handler via vtable with bUnequip=true
    // 4. Update CUser::stMyCharInfoEx arrays based on type:
    //    - byInvenType == 0: stShapeEquipItemInfo[shSlot].biSerial=-1, nItemID=-1, nDyeID=0
    //    - byInvenType == 3: stLookEquipIemInfo[shSlot].biSerial=-1, nItemID=-1, nDyeID=0
    //    - shSlot == 0: STEquipBase::Init(&stSoulWeapon)
    //    - shSlot == 1: STEquipBase::Init(&stSubWeapon)

    (void)nEquipSlot;
    return false;  // TODO: 需人工审查 - Implement full logic per IDA
}

// Full Unequip implementation with correct signature (IDA 0x1400A5B10)
void CGocInventory::Unequip(std::uint8_t byInvenType, std::int16_t shSlot) {
    // IDA-verified implementation outline:
    // 1. Get equipment pointer
    // XBaseEquip* pEquip = GetEquipPtr(byInvenType);
    // if (!pEquip) return;

    // 2. Get item from slot
    // std::tr1::shared_ptr<CItem> pItem;
    // pEquip->GetSlotInfo(&pItem, shSlot);
    // if (!pItem) return;

    // 3. Handle set items for non-look equipment
    // if (byInvenType != 3) {
    //     uint8_t bySetCount = 0;
    //     TB_ITEM* pItemTable = pItem->GetItemTable();
    //     if (pItemTable && pItemTable->Item_SetItem_ID) {
    //         int nItemID = pItem->GetCurID();
    //         bySetCount = pEquip->GetSetItemCount(nItemID, pItemTable->Item_SetItem_ID);
    //     }
    //     // Call unequip handler via vtable
    //     pItem->UnequipHandler(/* params */, true, bySetCount);
    // }

    // 4. Update CUser equipment info arrays
    // CUser* pUser = GetCUser();  // RTTI cast from owner
    // if (pUser) {
    //     STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();
    //     if (byInvenType == 0 && shSlot <= 13) {
    //         pCharInfo->stShapeEquipItemInfo[shSlot].biSerial = -1;
    //         pCharInfo->stShapeEquipItemInfo[shSlot].nItemID = -1;
    //         pCharInfo->stShapeEquipItemInfo[shSlot].nDyeID = 0;
    //     } else if (byInvenType == 3 && shSlot <= 13) {
    //         pCharInfo->stLookEquipIemInfo[shSlot].biSerial = -1;
    //         pCharInfo->stLookEquipIemInfo[shSlot].nItemID = -1;
    //         pCharInfo->stLookEquipIemInfo[shSlot].nDyeID = 0;
    //     } else if (shSlot == 0) {
    //         STEquipBase::Init(&pCharInfo->stSoulWeapon);
    //     } else if (shSlot == 1) {
    //         STEquipBase::Init(&pCharInfo->stSubWeapon);
    //     }
    // }

    (void)byInvenType;
    (void)shSlot;
    // TODO: 需人工审查 - Full implementation requires CUser, XBaseEquip, CItem classes
}

// IDA: 0x1400A1380
void CGocInventory::SetEquipItem(void* pInfo, int nIndex) {
    // TODO: Implement per IDA
    (void)pInfo;
    (void)nIndex;
}

// ============================================================================
// Inventory operations
// ============================================================================

void CGocInventory::SortInventory() {
    // TODO: Implement inventory sorting
}

// ============================================================================
// Weight/encumbrance
// ============================================================================

int CGocInventory::GetCurrentWeight() const {
    return m_nCurrentWeight;
}

int CGocInventory::GetMaxWeight() const {
    return m_nMaxWeight;
}

bool CGocInventory::IsOverencumbered() const {
    return m_nCurrentWeight > m_nMaxWeight;
}

// ============================================================================
// Quest items
// ============================================================================

bool CGocInventory::IsQuestItem(int nItemId) const {
    // TODO: Implement quest item check
    (void)nItemId;
    return false;
}

int CGocInventory::GetQuestItemCount() const {
    // TODO: Implement quest item count
    return 0;
}

// ============================================================================
// Static functions
// ============================================================================

// IDA: 0x1400262C0
// __int64 __fastcall CGocInventory::GetFamilyID()
// {
//   return 7;
// }
int CGocInventory::GetFamilyID() {
    return 7;
}

// ============================================================================
// Cash functions (IDA verified)
// ============================================================================

// IDA: 0x1400F7940
// __int64 __fastcall CGocInventory::GetCash(CGocInventory *this)
// {
//   return (unsigned int)this->m_nCash;
// }
int CGocInventory::GetCash() const {
    return static_cast<int>(m_nCash);
}

// IDA: 0x1400A49A0
// void __fastcall CGocInventory::SetCash(CGocInventory *this, int nCash, bool bSyncDB)
// {
//   this->m_bLoadCash = 1;
//   this->m_nCash = nCash;
//   if ( bSyncDB )
//   {
//     // Send DB packet: main=2, sub=0x51
//     // XSendDBPacket xSendDBPacket(pObject, 2, 0x51);
//     // xSendDBPacket << pUser->GetUAID();
//     // xSendDBPacket << nCash;
//     // XGameServer::SendDBAccount(&xSendDBPacket);
//   }
// }
void CGocInventory::SetCash(int nCash, bool bSyncDB) {
    // Set load flag and cash value (IDA verified)
    m_bLoadCash = true;
    m_nCash = nCash;

    if (bSyncDB) {
        // TODO: Send DB sync packet (main=2, sub=0x51)
        // Need to get CUser from component hierarchy to get UAID
        // VChunkFile* v8 = std::list<CBattleZone *>::size((VChunkLocker *)this);
        // CUser* pUser = dynamic_cast<CUser*>(...);
        // if (pUser) {
        //     int UAID = pUser->GetUAID();
        //     XSendDBPacket xSendDBPacket(pObject, 2, 0x51);
        //     xSendDBPacket << UAID;
        //     xSendDBPacket << nCash;
        //     XGameServer::Instance()->SendDBAccount(&xSendDBPacket);
        // }
    }
}

// IDA: 0x1400A4800
// char __fastcall CGocInventory::AddCash(CGocInventory *this, int nCash, unsigned __int8 byLogType)
// {
//   if ( !nCash )
//     return 1;
//   if ( nCash + this->m_nCash < 0 )
//     return 0;
//   this->m_nCash += nCash;
//   // Send DB packet: main=2, sub=0x41
//   // XSendDBPacket xSendDBPacket(pObject, 2, 0x41);
//   // xSendDBPacket << pUser->GetUAID();
//   // xSendDBPacket << nCash;
//   // return XGameServer::SendDBAccount(&xSendDBPacket);
//   return 1;
// }
bool CGocInventory::AddCash(int nCash, std::uint8_t byLogType) {
    // No change needed (IDA verified)
    if (nCash == 0)
        return true;

    // Check for overflow (IDA verified)
    if (nCash + m_nCash < 0)
        return false;

    // Update cash (IDA verified)
    m_nCash += nCash;

    // IDA: Send DB sync packet (main=2, sub=0x41)
    // VChunkFile* v10 = std::list<CBattleZone *>::size((VChunkLocker *)this);
    // CUser* pUser = dynamic_cast<CUser*>(...);
    // if (pUser) {
    //     int UAID = pUser->GetUAID();
    //     XSendDBPacket xSendDBPacket(pObject, 2, 0x41);
    //     xSendDBPacket << UAID;
    //     xSendDBPacket << nCash;
    //     return XGameServer::Instance()->SendDBAccount(&xSendDBPacket);
    // }

    (void)byLogType;  // Note: byLogType is unused in IDA decompiled output
    return true;
}

// IDA: 0x1400A4B10
// void __fastcall CGocInventory::SendCash(CGocInventory *this, int nResultCash)
// Sends cash update packet to client (main=8, sub=0x33)
void CGocInventory::SendCash(int nResultCash) {
    // IDA: XSendPacket::XSendPacket(&xSendPacket, 8u, 0x33u)
    // XParse::operator<<(&xSendPacket.XParse, nResultCash)
    // CGocNetwork::Send(pActor, &xSendPacket)

    // TODO: Implement packet sending
    // XSendPacket xSendPacket(8, 0x33);
    // xSendPacket << nResultCash;
    //
    // VChunkFile* v9 = std::list<CBattleZone *>::size((VChunkLocker *)this);
    // XActor* pActor = v9 ? (XActor*)&v9[3].m_ChunkSizeTempMemOfs : nullptr;
    // CGocNetwork::Send(pActor, &xSendPacket);

    (void)nResultCash;
}

// IDA: 0x1400A4530
// void __fastcall CGocInventory::LoadCash(CGocInventory *this)
// {
//   if ( !this->m_bLoadCash )
//   {
//     // Check billing type
//     // if ( billingType == BILLING_TYPE_REAL )
//     //   CGocInventory::InitWeMadeBilling(this);
//     // else
//     //   Send DB request (main=2, sub=0x40)
//   }
// }
void CGocInventory::LoadCash() {
    if (!m_bLoadCash) {
        // TODO: Check billing type from XOption
        // XOption* pOption = XGameServer::Instance()->GetOption();
        // if (pOption->GetBillingType() == BILLING_TYPE_REAL) {
        //     InitWeMadeBilling();
        // } else {
        //     Send DB request (main=2, sub=0x40)
        //     XSendDBPacket xSendDBPacket(pObject, 2, 0x40);
        //     xSendDBPacket << pUser->GetUAID();
        //     XGameServer::SendDBAccount(&xSendDBPacket);
        // }
    }
}

// IDA: 0x1400A4690
// void __fastcall CGocInventory::ReloadCash(CGocInventory *this)
// {
//   this->m_bLoadCash = 0;
//   // Same as LoadCash logic
//   this->m_bLoadCash = 1;
// }
void CGocInventory::ReloadCash() {
    m_bLoadCash = false;

    // TODO: Same logic as LoadCash
    // XOption* pOption = XGameServer::Instance()->GetOption();
    // if (pOption->GetBillingType() == BILLING_TYPE_REAL) {
    //     InitWeMadeBilling();
    // } else {
    //     Send DB request (main=2, sub=0x40)
    // }

    m_bLoadCash = true;
}

// IDA: 0x140068690
// void __fastcall CGocInventory::SetReadyLoadCash(CGocInventory *this, bool bFlag)
// {
//   this->m_bReadyLoadCash = bFlag;
// }
void CGocInventory::SetReadyLoadCash(bool bFlag) {
    m_bReadyLoadCash = bFlag;
}

// ============================================================================
// Cash Mileage functions
// ============================================================================

// IDA: 0x1400E5140
// __int64 __fastcall CGocInventory::GetCashMileage(CGocInventory *this, E_CASH_MILEAGE_TYPE eType)
// {
//   switch ( eType )
//   {
//     case E_CASH_MILEAGE_AKASHIC: return (unsigned int)this->m_nCashMileage[0];
//     case E_CASH_MILEAGE_BROACH:  return (unsigned int)this->m_nCashMileage[1];
//     case E_CASH_MILEAGE_TAG:     return (unsigned int)this->m_nCashMileage[2];
//   }
//   return 0xFFFFFFFFLL;
// }
int CGocInventory::GetCashMileage(int eType) const {
    // IDA verified: switch on eType
    switch (eType) {
        case 0:  // E_CASH_MILEAGE_AKASHIC
            return m_nCashMileage[0];
        case 1:  // E_CASH_MILEAGE_BROACH
            return m_nCashMileage[1];
        case 2:  // E_CASH_MILEAGE_TAG
            return m_nCashMileage[2];
        default:
            return -1;  // 0xFFFFFFFF (IDA verified)
    }
}

// IDA: 0x1400E4EA0
// void __fastcall CGocInventory::SetCashMileage(CGocInventory *this, int *pCashMileage, bool bSend)
// {
//   PS_CASH_MILEAGE_LIST psList;
//   for ( i = 0; i < 3; ++i )
//   {
//     this->m_nCashMileage[i] = pCashMileage[i];
//     psInfo.byMileageType = i;
//     psInfo.nCashMileage = this->m_nCashMileage[i];
//     psList.push_back(psInfo);
//   }
//   if ( bSend )
//   {
//     // Send packet (main=3, sub=0x7B)
//     XSendPacket xSendPacket(3, 0x7B);
//     xSendPacket << psList;
//     CGocNetwork::Send(pActor, &xSendPacket);
//   }
// }
void CGocInventory::SetCashMileage(int* pCashMileage, bool bSend) {
    if (!pCashMileage)
        return;

    // Update mileage values (IDA verified)
    for (int i = 0; i < 3; ++i) {
        m_nCashMileage[i] = pCashMileage[i];
    }

    if (bSend) {
        // IDA: Send packet to client (main=3, sub=0x7B)
        // PS_CASH_MILEAGE_LIST psList;
        // for (int i = 0; i < 3; ++i) {
        //     PS_CASH_MILEAGE psInfo;
        //     psInfo.byMileageType = i;
        //     psInfo.nCashMileage = m_nCashMileage[i];
        //     psList.push_back(psInfo);
        // }
        // XSendPacket xSendPacket(3, 0x7B);
        // xSendPacket << psList;
        // CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// IDA: 0x1400E5020
// void __fastcall CGocInventory::SetCashMileage(CGocInventory *this, PS_CASH_MILEAGE psUpdateInfo)
// {
//   this->m_nCashMileage[psUpdateInfo.byMileageType] = psUpdateInfo.nCashMileage;
//   // Send packet (main=3, sub=0x7B)
// }
void CGocInventory::SetCashMileage(void* psUpdateInfo) {
    // TODO: Implement with PS_CASH_MILEAGE struct
    // PS_CASH_MILEAGE* pInfo = static_cast<PS_CASH_MILEAGE*>(psUpdateInfo);
    // if (pInfo->byMileageType < 3) {
    //     m_nCashMileage[pInfo->byMileageType] = pInfo->nCashMileage;
    //     // Send packet to client (main=3, sub=0x7B)
    // }
    (void)psUpdateInfo;
}

// IDA: 0x1400E5500
// void __fastcall CGocInventory::SendDBCashMileageUpdate(CGocInventory *this, PS_DB_CASH_MILEAGE_LIST *psDBList)
// {
//   // Send DB packet (main=2, sub=0x68)
//   // XSendDBPacket xSendDBPacket(pObject, 2, 0x68);
//   // xSendDBPacket << psDBList;
//   // XGameServer::SendDBAccount(&xSendDBPacket);
//   // SendCashMileageLog(3, psDBList, 0);
// }
void CGocInventory::SendDBCashMileageUpdate(void* psDBList) {
    // TODO: Send DB packet (main=2, sub=0x68)
    // PS_DB_CASH_MILEAGE_LIST* pList = static_cast<PS_DB_CASH_MILEAGE_LIST*>(psDBList);
    // XSendDBPacket xSendDBPacket(pObject, 2, 0x68);
    // xSendDBPacket << pList;
    // XGameServer::SendDBAccount(&xSendDBPacket);
    // SendCashMileageLog(3, pList, 0);
    (void)psDBList;
}

// ============================================================================
// Cash Buy Count functions
// ============================================================================

// IDA: 0x1400C33F0
// Loads cash buy count list from DB, filters expired entries
void CGocInventory::LoadCashBuyCount(void* psList) {
    // TODO: Implement per IDA
    // Iterate PS_CASH_BUY_COUNT_LIST
    // For each entry, check if biEndDate >= current date or biEndDate == 0
    // If valid, insert into m_mpCashBuyCount map
    (void)psList;
}

// IDA: 0x1400C3500
// Updates buy count for a cash shop item with limit checking
bool CGocInventory::UpdateCashBuyCount(int nCashShopIndex, int nBuyCount,
                                        std::uint8_t byLimitType, int nLimitCount,
                                        void* psList) {
    // TODO: Implement per IDA
    // 1. Check IsBuyCashLimitCount for valid limit type
    // 2. Find existing entry in m_mpCashBuyCount
    // 3. Check if new count would exceed limit
    // 4. Update or add entry to psList
    (void)nCashShopIndex;
    (void)nBuyCount;
    (void)byLimitType;
    (void)nLimitCount;
    (void)psList;
    return false;
}

// IDA: 0x1400E5AD0
// Checks buy limit type and calculates end date for the limit period
bool CGocInventory::IsBuyCashLimitCount(int eLimitType, std::int64_t& biEndDate) {
    biEndDate = 0;

    // E_CASH_SHOP_BUY enum values:
    // 1 = E_CASH_SHOP_BUY_LIMIT (no limit)
    // 2 = E_CASH_SHOP_BUY_LIMIT_DAY
    // 3 = E_CASH_SHOP_BUY_LIMIT_WEEK
    // 4 = E_CASH_SHOP_BUY_LIMIT_MONTH
    // 5 = E_CASH_SHOP_BUY_LIMIT_ACCOUNT
    // 6 = E_CASH_SHOP_BUY_LIMIT_ACCOUNT_DAY
    // 7 = E_CASH_SHOP_BUY_LIMIT_ACCOUNT_WEEK
    // 8 = E_CASH_SHOP_BUY_LIMIT_ACCOUNT_MONTH

    switch (eLimitType) {
        case 1:  // E_CASH_SHOP_BUY_LIMIT
        case 5:  // E_CASH_SHOP_BUY_LIMIT_ACCOUNT
            // No date limit
            return true;

        case 2:  // E_CASH_SHOP_BUY_LIMIT_DAY
        case 6:  // E_CASH_SHOP_BUY_LIMIT_ACCOUNT_DAY
            // Get daily reset time from XGameServer
            // biEndDate = XGameServer::GetUpdateDate(9);
            return true;

        case 3:  // E_CASH_SHOP_BUY_LIMIT_WEEK
        case 7:  // E_CASH_SHOP_BUY_LIMIT_ACCOUNT_WEEK
            // Calculate next weekly reset (Wednesday 9:00)
            // TODO: Implement week calculation per IDA
            return true;

        case 4:  // E_CASH_SHOP_BUY_LIMIT_MONTH
        case 8:  // E_CASH_SHOP_BUY_LIMIT_ACCOUNT_MONTH
            // Calculate next monthly reset (1st day 9:00)
            // TODO: Implement month calculation per IDA
            return true;

        default:
            // Invalid limit type
            return false;
    }
}

// IDA: 0x1400E5FA0
// Initializes cash item buy count, clears expired entries
void CGocInventory::OnInitItemCashCount() {
    // TODO: Implement per IDA
    // 1. Check if this is a CUser (RTTI check)
    // 2. Iterate m_mpCashBuyCount
    // 3. For entries with biEndDate < current update date and biEndDate != 0:
    //    - Remove from map
    //    - Add to PS_CASH_BUY_COUNT_LIST with nBuyCount = 0
    // 4. Send packet to client (main=9, sub=0x31)
}

// ============================================================================
// Cash Item Set functions
// ============================================================================

// IDA: 0x1400B89E0
// Adds cash item set list to m_stCashSet array (max 9 sets)
void CGocInventory::AddCashItemSet(void* stCashSetList) {
    // TODO: Implement per IDA
    // Iterate PS_CASH_SET_LIST
    // For each PS_CASH_SET, if bySetNo < 9, copy to m_stCashSet[bySetNo]
    (void)stCashSetList;
}

// IDA: 0x1400B8B10
// Deletes a cash item set by index, syncs to DB
bool CGocInventory::DelCashItemSet(std::uint8_t bySetNo) {
    // Check valid range
    if (bySetNo >= 9)
        return false;

    // Clear the set
    std::memset(&m_stCashSet[bySetNo], 0, 104);  // sizeof(PS_CASH_SET) = 104

    // TODO: Send DB packet (main=0x22, sub=0x23)
    // XSendDBPacket xSendDBPacket(pObject, 0x22, 0x23);
    // xSendDBPacket << GetUCID();
    // xSendDBPacket << bySetNo;
    // XGameServer::SendDBGame(&xSendDBPacket);

    return true;
}

// IDA: 0x1400B8C90
// Updates a cash item set, syncs to DB
bool CGocInventory::UpdateCashItemSet(void* stCashSet) {
    // TODO: Implement per IDA
    // PS_CASH_SET* pSet = static_cast<PS_CASH_SET*>(stCashSet);
    // if (pSet->bySetNo >= 9)
    //     return false;
    // Copy to m_stCashSet[pSet->bySetNo]
    // Send DB packet (main=0x22, sub=0x22)
    (void)stCashSet;
    return false;
}

// IDA: 0x1400C8960
// Sends cash buy count list to client
void CGocInventory::SendCashCount() {
    // TODO: Implement per IDA
    // Build PS_CASH_BUY_COUNT_LIST from m_mpCashBuyCount
    // Send packet (main=9, sub=0x30)
}

// ============================================================================
// Package Box / Random Box functions (IDA verified)
// ============================================================================

// IDA: 0x1400B2D80 - CGocInventory::PackageBoxUse
// Complex function (~0x1E31 bytes) that uses a package box item
// Creates contained items based on TB_ITEM_PACKAGE, handles upgrades, rewards
bool CGocInventory::PackageBoxUse(bool bReduceItem, std::shared_ptr<CItem> pItem,
                                   std::uint8_t byCount, int nItemIDparClass) {
    // IDA decompilation shows this is a complex function that:
    // 1. Validates count (must be 1-10)
    // 2. Gets item ID from pItem (check Item_Effect_Type for override)
    // 3. Gets TB_ITEM_PACKAGE table entry
    // 4. Iterates package contents (15 items max)
    // 5. For each item in package:
    //    - Get TB_ITEM and TB_ITEM_CLASSIFY
    //    - Handle upgrade type (Unpacking_Function_Type == 1)
    //    - Build ST_CREATE_ITEM list
    //    - Count required inventory slots by type
    // 6. Check IsEmptyInventory for each inventory type
    // 7. Reduce the package item
    // 8. Add items using AddItemUpgradeCount
    // 9. Send DB packet and add rewards (gold, BP, ether)

    // Validate count (IDA: 0x1400B2DB5)
    if (byCount == 0 || byCount > 10) {
        // IDA: CGocNetwork::SendErrorMessage(v5, 8u, 0x11u, 0xCB2Bu)
        // LogHelper::LogError("game.item", "PackageBoxUse error - Req Count[ ActorID:%d, Count:%d ] ( %d )", ...)
        return false;
    }

    // TODO: Full implementation requires:
    // - CItem methods (GetItemTable, GetCurID, GetCount, GetSlot, GetInvenType)
    // - XResourceMgr methods (GetTB_ITEM_PACKAGE, GetTB_ITEM, GetTB_ITEM_CLASSIFY, GetTB_REINFORCE)
    // - XSendDBPacket, XSendPacket for DB/client communication
    // - CGocNetwork::SendErrorMessage
    // - CGocInventory::IsEmptyInventory, ReduceItem3, ReduceItem2, AddItemUpgradeCount
    // - CGocInventory::UpdateItemEnd, AddItemEnd, CheckOverMoney, AddMoney, AddBP, AddEther
    // - ST_CREATE_ITEM, ST_CREATE_ITEMS, ST_GET_INFO, PS_RES_STORAGE_INFO structures

    (void)bReduceItem;
    (void)pItem;
    (void)nItemIDparClass;

    return false;  // TODO: 需人工审查 - Implement full logic per IDA 0x1400B2D80
}

// Random Box functions are part of PackageBoxUse
// CanRandomBoxUse and RandomBoxUse are not separate functions in the PDB
// They are internal code blocks within PackageBoxUse (0x1400B2D80)
