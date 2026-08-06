// GocExchange.cpp - CGocExchange implementation
// IDA decompiled functions from GameServer.exe

#include "GocExchange.h"
#include "GocInventory.h"
#include "GocNetwork.h"
#include "Mover.h"
#include "User.h"
#include "GameServer.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/XSendPacket.h"
#include "Soulworker/GameServer/XCore/XServer/LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerExchange.h"
#include <ATLTime.h>

// Helper function to get owner mover from component
namespace {
    CMover* GetOwnerMover(CGocExchange* pThis) {
        return pThis->GetOwnerGO();
    }
}

// ============================================================================
// IDA: ??0CGocExchange@@QEAA@XZ (0x1400751B0)
// Constructor
// ============================================================================
CGocExchange::CGocExchange()
    : GOComponent()
    , m_bLoadMyList(false)
    , m_bLoadMyInterestList(false)
    , m_n64LastSearchTime(0)
    , m_nWaitTime(1800)
    , m_bSendBuyPacket(false)
{
    // IDA shows: map and vector are default constructed, then Clear() is called
    Clear();
}

// ============================================================================
// IDA: ??1CGocExchange@@UEAA@XZ (0x140075280)
// Destructor
// ============================================================================
CGocExchange::~CGocExchange()
{
    Clear();
    m_vecMyInterestList.clear();
    m_mapMyList.clear();
    // Base class destructor called automatically
}

// ============================================================================
// IDA: ?Init@CGocExchange@@QEAA_NXZ (0x1400752E0)
// Initialize - returns true after clearing
// ============================================================================
bool CGocExchange::Initialize()
{
    Clear();
    return true;
}

// ============================================================================
// IDA: ?Clear@CGocExchange@@QEAAXXZ (0x140075300)
// Clear all state
// ============================================================================
void CGocExchange::Clear()
{
    m_bLoadMyList = false;
    m_bLoadMyInterestList = false;
    m_mapMyList.clear();
    m_vecMyInterestList.clear();
    m_n64LastSearchTime = 0;
    m_nWaitTime = 1800;
    m_bSendBuyPacket = false;
}

// ============================================================================
// IDA: ?GetFamilyID@CGocExchange@@SAHXZ (0x1401F35F0)
// Returns component family ID: 20
// ============================================================================
int CGocExchange::GetFamilyID()
{
    return 20;
}

// ============================================================================
// IDA: ?GetExchangeMyList@CGocExchange@@QEAAXAEAV?$vector@UST_MY_EXCHANGE_ITEM@@V?$allocator@UST_MY_EXCHANGE_ITEM@@@std@@@std@@@Z (0x140075370)
// Copies m_mapMyList values to output vector
// ============================================================================
void CGocExchange::GetExchangeMyList(std::vector<ST_MY_EXCHANGE_ITEM>& vecMyList)
{
    vecMyList.clear();
    for (auto it = m_mapMyList.begin(); it != m_mapMyList.end(); ++it)
    {
        vecMyList.push_back(it->second);
    }
}

// ============================================================================
// IDA: ?GetExchangeMyInterestList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_INTEREST_LIST_RES@@@Z (0x1400753F0)
// Copies m_vecMyInterestList to output response
// IDA 反编译:
// void __fastcall CGocExchange::GetExchangeMyInterestList(CGocExchange *this, PS_EXCHANGE_INTEREST_LIST_RES *psReq)
// {
//   std::vector<unsigned long>::operator=(&psReq->vecItemList, &this->m_vecMyInterestList);
// }
// ============================================================================
void CGocExchange::GetExchangeMyInterestList(PS_EXCHANGE_INTEREST_LIST_RES& psRes)
{
    psRes.vecItemList = m_vecMyInterestList;
}

// ============================================================================
// IDA: ?ReqExchangeSearch@CGocExchange@@QEAAXAEAUPS_EXCHANGE_SEARCH_REQ@@@Z (0x140075F90)
// Sends exchange search request to DB
// ============================================================================
void CGocExchange::ReqExchangeSearch(PS_EXCHANGE_SEARCH_REQ& psSearch)
{
    // IDA decompiled from 0x140075F90
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!XResourceMgr::GetServerContents(&pServer->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE))
    {
        CMover* pMover = GetOwnerMover(this);
        CGocNetwork::SendErrorMessage(pMover, 0x2B, 1, 0xC3B8);
        return;
    }

    // Check search cooldown (1 second)
    ATL::CTime tCurr = ATL::CTime::GetTickCount();
    ATL::CTimeSpan tSpan(0, 0, 0, 1);

    if ((__int64)tCurr >= (__int64)tSpan + m_n64LastSearchTime)
    {
        m_n64LastSearchTime = (__int64)tCurr;

        // Fix grade range
        if (psSearch.nItemGradeMin <= -1)
            psSearch.nItemGradeMax = -1;

        // If item ID specified, lookup item classification
        if (psSearch.dwItemID)
        {
            TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, psSearch.dwItemID);
            if (!pTB_ITEM)
            {
                CMover* pMover = GetOwnerMover(this);
                CGocNetwork::SendErrorMessage(pMover, 0x2B, 1, 0xCB24);
                return;
            }

            TB_ITEM_CLASSIFY* pTB_CLASSIFY = XResourceMgr::GetTB_ITEM_CLASSIFY(&pServer->m_xResourceMgr, pTB_ITEM->Item_Classify_Index);
            if (!pTB_CLASSIFY)
            {
                CMover* pMover = GetOwnerMover(this);
                CGocNetwork::SendErrorMessage(pMover, 0x2B, 1, 0xCB24);
                return;
            }

            psSearch.nCategoryID = pTB_CLASSIFY->GroupID;
            psSearch.nSubCategoryID = pTB_CLASSIFY->SubGroupID;
        }
        else if (psSearch.nCategoryID == -1 || psSearch.nSubCategoryID == -1)
        {
            // Reset all search parameters to defaults
            psSearch.nCategoryID = -1;
            psSearch.nSubCategoryID = -1;
            psSearch.nUseClass = -1;
            psSearch.nLevelMin = -1;
            psSearch.nLevelMax = 68;
            psSearch.nItemGradeMin = -1;
            psSearch.nItemGradeMax = -1;
            psSearch.nPrice = -1;
            psSearch.nUpgrade = -1;
        }

        // Send DB request (Main=0x27, Sub=1)
        CMover* pMover = GetOwnerMover(this);
        XSendDBPacket xSendDBPacket(pMover, 0x27, 1);
        xSendDBPacket << psSearch;
        pServer->SendDBGame(xSendDBPacket);
    }
}

// ============================================================================
// IDA: ?ReqExchangeSellRegister@CGocExchange@@QEAAXAEAUPS_EXCHANGE_SELL_REGISTER_REQ@@@Z (0x140076830)
// Registers an item for sale on the exchange
// ============================================================================
void CGocExchange::ReqExchangeSellRegister(PS_EXCHANGE_SELL_REGISTER_REQ& psReq)
{
    // IDA: 0x140076830 - Large function (~9.5KB, 291 basic blocks)
    // This function handles registering items for sale on the exchange marketplace.
    // Due to the function's complexity, this is a partial implementation based on IDA analysis.

    // 1. Verify exchange feature is enabled
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!XResourceMgr::GetServerContents(&pServer->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE))
    {
        CMover* pMover = GetOwnerMover(this);
        CGocNetwork::SendErrorMessage(pMover, 0x2B, 5, 0xC3B8);
        return;
    }

    // 2. Get owner mover and validate
    CMover* pMover = GetOwnerMover(this);
    if (!pMover)
    {
        LogHelper::LogError("game.contents", "ReqExchangeSellRegister() Mover NULL");
        return;
    }

    // 3. Cast to CUser and validate
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser)
    {
        LogHelper::LogError("game.contents", "ReqExchangeSellRegister() User NULL");
        return;
    }

    // 4. Check GM power (GM_STATE_GM_GAMEMASTER)
    if (CUser::GetGMPower(pUser) == 1)
    {
        XOption* pOption = XServer::GetOption(pServer);
        if (pOption && pOption->GetNationType() != NATION_TYPE_KOR)
        {
            LogHelper::LogError("game.item", "ReqExchangeSellRegister error - GM_STATE_GM_GAMEMASTER ( %d )", pMover->GetActorID().GetID());
            CGocNetwork::SendErrorMessage(pMover, 0x2B, 5, 0xCCBF);
            return;
        }
    }

    // 5. Check block type
    if (CUser::GetBlockType(pUser))
    {
        LogHelper::LogDebug("game.contents", "ReqExchangeSellRegister - BlockType");
        PS_EXCHANGE_SELL_REGISTER_RES psResult;
        psResult.nResult = 50111;

        XSendPacket xSendPacket(0x2B, 5);
        xSendPacket << psResult;
        CGocNetwork::Send(pMover, xSendPacket);
        return;
    }

    // 6. Check second password if enabled
    if (XResourceMgr::GetServerContents(&pServer->m_xResourceMgr, E_SERVER_OPTION_SECOND_PW))
    {
        std::tr1::shared_ptr<CGocInventory> pMyInven;
        CMover::GetGOC<CGocInventory>(pMover, &pMyInven, false);
        if (pMyInven)
        {
            int nTradePWCheck = 0;
            CGocInventory* pInvenPtr = pMyInven.operator->();
            if (!CGocInventory::CheckTradePasswordState(pInvenPtr, &nTradePWCheck))
            {
                LogHelper::LogDebug("game.contents", "ReqExchangeSellRegister - SecondPW Check Fail");
                PS_EXCHANGE_SELL_REGISTER_RES psResult;
                psResult.nResult = 52002;

                XSendPacket xSendPacket(0x2B, 5);
                xSendPacket << psResult;
                CGocNetwork::Send(pMover, xSendPacket);
                return;
            }
        }
    }

    // 7. Get inventory component and validate item
    std::tr1::shared_ptr<CGocInventory> pInvenPtr;
    CMover::GetGOC<CGocInventory>(pMover, &pInvenPtr, false);
    if (!pInvenPtr)
    {
        LogHelper::LogError("game.contents", "ReqExchangeSellRegister() Inventory NULL");
        return;
    }

    CGocInventory* pInven = pInvenPtr.operator->();

    // 8. Get item from inventory
    std::tr1::shared_ptr<CItem> pItem;
    pInven->GetItemPtr(&pItem, psReq.xSerial);
    if (!pItem)
    {
        LogHelper::LogError("game.contents", "ReqExchangeSellRegister() Item NULL (Serial: %lld)", psReq.xSerial);
        PS_EXCHANGE_SELL_REGISTER_RES psResult;
        psResult.nResult = 52002;

        XSendPacket xSendPacket(0x2B, 5);
        xSendPacket << psResult;
        CGocNetwork::Send(pMover, xSendPacket);
        return;
    }

    // 9. Validate item ownership and lock status
    if (pItem->GetLock())
    {
        LogHelper::LogError("game.contents", "ReqExchangeSellRegister() Item Locked (Serial: %lld)", psReq.xSerial);
        PS_EXCHANGE_SELL_REGISTER_RES psResult;
        psResult.nResult = 52002;

        XSendPacket xSendPacket(0x2B, 5);
        xSendPacket << psResult;
        CGocNetwork::Send(pMover, xSendPacket);
        return;
    }

    // 10. Get item table data
    TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, pItem->GetItemID());
    if (!pTB_ITEM)
    {
        LogHelper::LogError("game.contents", "ReqExchangeSellRegister() TB_ITEM NULL (ItemID: %d)", pItem->GetItemID());
        PS_EXCHANGE_SELL_REGISTER_RES psResult;
        psResult.nResult = 52002;

        XSendPacket xSendPacket(0x2B, 5);
        xSendPacket << psResult;
        CGocNetwork::Send(pMover, xSendPacket);
        return;
    }

    // 11. Validate price
    if (psReq.nPrice_One <= 0)
    {
        LogHelper::LogError("game.contents", "ReqExchangeSellRegister() Invalid Price (Price: %lld)", psReq.nPrice_One);
        PS_EXCHANGE_SELL_REGISTER_RES psResult;
        psResult.nResult = 58302;

        XSendPacket xSendPacket(0x2B, 5);
        xSendPacket << psResult;
        CGocNetwork::Send(pMover, xSendPacket);
        return;
    }

    // 12. Validate count
    if (psReq.sCount <= 0 || psReq.sCount > pItem->GetCount())
    {
        LogHelper::LogError("game.contents", "ReqExchangeSellRegister() Invalid Count (Count: %d, ItemCount: %d)", psReq.sCount, pItem->GetCount());
        PS_EXCHANGE_SELL_REGISTER_RES psResult;
        psResult.nResult = 52014;

        XSendPacket xSendPacket(0x2B, 5);
        xSendPacket << psResult;
        CGocNetwork::Send(pMover, xSendPacket);
        return;
    }

    // 13. Check cash items if specified
    std::int64_t xSerial_Commission = 0;
    std::int64_t xSerial_Count = 0;
    std::int64_t xSerial_Expire = 0;

    if (psReq.byCash_Commission || psReq.byCash_Count || psReq.byCash_Expire)
    {
        if (!CheckCashItem(psReq, xSerial_Commission, xSerial_Count, xSerial_Expire))
        {
            LogHelper::LogDebug("game.contents", "ReqExchangeSellRegister() CheckCashItem Failed");
            PS_EXCHANGE_SELL_REGISTER_RES psResult;
            psResult.nResult = 58303;

            XSendPacket xSendPacket(0x2B, 5);
            xSendPacket << psResult;
            CGocNetwork::Send(pMover, xSendPacket);
            return;
        }
    }

    // 14. Lock the item
    pItem->SetLock(1);

    // 15. Lock cash items if specified
    if (xSerial_Commission)
    {
        std::tr1::shared_ptr<CItem> pCashItem;
        pInven->GetItemPtr(&pCashItem, xSerial_Commission);
        if (pCashItem)
            pCashItem->SetLock(1);
    }

    if (xSerial_Count)
    {
        std::tr1::shared_ptr<CItem> pCashItem;
        pInven->GetItemPtr(&pCashItem, xSerial_Count);
        if (pCashItem)
            pCashItem->SetLock(1);
    }

    if (xSerial_Expire)
    {
        std::tr1::shared_ptr<CItem> pCashItem;
        pInven->GetItemPtr(&pCashItem, xSerial_Expire);
        if (pCashItem)
            pCashItem->SetLock(1);
    }

    // 16. Create DB registration packet
    PS_DB_EXCHANGE_SELL_REGISTER psDBReq;
    memset(&psDBReq, 0, sizeof(psDBReq));

    psDBReq.dwUCID = pMover->GetActorID().GetID();
    psDBReq.dwItemID = pItem->GetItemID();
    psDBReq.xSerial = psReq.xSerial;
    psDBReq.sCount = psReq.sCount;
    psDBReq.nPrice_One = psReq.nPrice_One;
    psDBReq.byInvenType = pItem->GetInvenType();
    psDBReq.xSerial_Commission = xSerial_Commission;
    psDBReq.xSerial_Count = xSerial_Count;
    psDBReq.xSerial_Expire = xSerial_Expire;
    psDBReq.byCash_Commission = psReq.byCash_Commission;
    psDBReq.byCash_Count = psReq.byCash_Count;
    psDBReq.byCash_Expire = psReq.byCash_Expire;

    // Copy item info
    pItem->GetItem(&psDBReq.stCreateItem.stItem);

    // 17. Send DB request (Main=0x27, Sub=2)
    XSendDBPacket xSendDBPacket(pMover, 0x27, 2);
    xSendDBPacket << psDBReq;
    pServer->SendDBGame(xSendDBPacket);
}

// ============================================================================
// IDA: ?ReqExchangeItemBuy@CGocExchange@@QEAAXAEAUPS_EXCHANGE_ITEM_BUY_REQ@@@Z (0x140078D80)
// Sends item buy request to DB
// ============================================================================
void CGocExchange::ReqExchangeItemBuy(PS_EXCHANGE_ITEM_BUY_REQ& psReq)
{
    // IDA decompiled from 0x140078D80
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!XResourceMgr::GetServerContents(&pServer->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE))
    {
        CMover* pMover = GetOwnerMover(this);
        CGocNetwork::SendErrorMessage(pMover, 0x2B, 6, 0xC3B8);
        return;
    }

    // Check if already sending buy packet
    if (m_bSendBuyPacket)
    {
        CMover* pMover = GetOwnerMover(this);
        if (pMover)
        {
            LogHelper::LogError("game.contents", "ReqExchangeItemBuy() Aready Packet (UCID:%d)", pMover->GetActorID().GetID());
        }
        return;
    }

    // Get owner mover and cast to CUser
    CMover* pMover = GetOwnerMover(this);
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser)
    {
        LogHelper::LogError("game.contents", "ReqExchangeItemBuy() User NULL");
        return;
    }

    // Check GM power (GM_STATE_GM_GAMEMASTER)
    if (CUser::GetGMPower(pUser) == 1)
    {
        XOption* pOption = XServer::GetOption(pServer);
        if (pOption && pOption->GetNationType() != NATION_TYPE_KOR)
        {
            LogHelper::LogError("game.item", "ReqExchangeItemBuy error - GM_STATE_GM_GAMEMASTER ( %d )", pMover->GetActorID().GetID());
            CGocNetwork::SendErrorMessage(pMover, 0x2B, 6, 0xCCBF);
            return;
        }
    }

    // Check block type
    if (CUser::GetBlockType(pUser))
    {
        LogHelper::LogDebug("game.contents", "ReqExchangeItemBuy - BlockType");
        PS_EXCHANGE_ITEM_BUY_RES psResult;
        psResult.nResult = 50111;
        psResult.dwExchangeID = psReq.dwExchangeID;
        psResult.shBuyCount = psReq.shCount;

        XSendPacket xSendPacket(0x2B, 6);
        xSendPacket << psResult;
        CGocNetwork::Send(pMover, xSendPacket);
        return;
    }

    // Check second password if enabled
    if (XResourceMgr::GetServerContents(&pServer->m_xResourceMgr, E_SERVER_OPTION_SECOND_PW))
    {
        std::tr1::shared_ptr<CGocInventory> pMyInven;
        CMover::GetGOC<CGocInventory>(pMover, &pMyInven, false);
        if (pMyInven)
        {
            int nTradePWCheck = 0;
            CGocInventory* pInvenPtr = pMyInven.operator->();
            if (!CGocInventory::CheckTradePasswordState(pInvenPtr, &nTradePWCheck))
            {
                LogHelper::LogDebug("game.contents", "ReqExchangeItemBuy - SecondPW Check Fail");
                PS_EXCHANGE_ITEM_BUY_RES psResult;
                psResult.nResult = 52002;
                psResult.dwExchangeID = psReq.dwExchangeID;
                psResult.shBuyCount = psReq.shCount;

                XSendPacket xSendPacket(0x2B, 6);
                xSendPacket << psResult;
                CGocNetwork::Send(pMover, xSendPacket);
                return;
            }
        }
    }

    // Send DB request (Main=0x27, Sub=9)
    XSendDBPacket xSendDBPacket(pMover, 0x27, 9);
    xSendDBPacket << psReq;
    pServer->SendDBGame(xSendDBPacket);
    m_bSendBuyPacket = true;
}

// ============================================================================
// IDA: ?SellMyExchangeItem@CGocExchange@@QEAAXAEAUPS_EXCHANGE_PRICE_HISTORY_UPDATE@@@Z (0x140075420)
// Handles selling of player's exchange item
// ============================================================================
void CGocExchange::SellMyExchangeItem(PS_EXCHANGE_PRICE_HISTORY_UPDATE& psUpdate)
{
    // IDA 反编译结果摘要:
    // 1. Iterate m_mapMyList to find item by dwExchangeID
    // 2. Update item count (decrease by sSellCount)
    // 3. If count reaches 0, erase from map
    // 4. Send PS_EXCHANGE_MY_LIST_RES packet (main=0x2B, sub=8)

    std::int64_t nSerial = 0;

    // Find the item by exchange ID
    for (auto it = m_mapMyList.begin(); it != m_mapMyList.end(); ++it)
    {
        if (it->second.dwExchangeID == psUpdate.dwExchangeID)
        {
            // Update count
            it->second.sInitCount -= psUpdate.sSellCount;
            nSerial = it->first;
            break;
        }
    }

    if (nSerial == 0)
        return;

    // Find and update the item
    auto itDel = m_mapMyList.find(nSerial);
    if (itDel != m_mapMyList.end())
    {
        // Send update packet to client (Main=0x2B, Sub=8)
        PS_EXCHANGE_MY_LIST_RES psList;
        psList.vecMyList.push_back(itDel->second);

        XSendPacket xSendPacket(0x2B, 8);
        xSendPacket << psList;
        CGocNetwork::Send(pMover, xSendPacket);

        // If count <= 0, remove from list
        if (itDel->second.sInitCount <= 0)
        {
            m_mapMyList.erase(itDel);
        }
    }
}

// ============================================================================
// IDA: ?CheckCashItem@CGocExchange@@QEAA_NAEAUPS_EXCHANGE_SELL_REGISTER_REQ@@AEA_J11@Z (0x1400756B0)
// Checks if cash items are available for exchange registration
// ============================================================================
bool CGocExchange::CheckCashItem(PS_EXCHANGE_SELL_REGISTER_REQ& psReq,
                                  std::int64_t& xSerial_Commission,
                                  std::int64_t& xSerial_Count,
                                  std::int64_t& xSerial_Expire)
{
    // IDA decompiled from 0x1400756B0
    // Cash item IDs:
    // - Commission discount: 0x31B5D488 (834000008)
    // - Count boost: 0x31B5D489 (834000009)
    // - Expire extension: 0x31B5D48A (834000010)

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();

    // Check commission cash item
    if (psReq.byCash_Commission)
    {
        CMover* pMover = GetOwnerMover(this);
        std::tr1::shared_ptr<CGocInventory> pInven;
        CMover::GetGOC<CGocInventory>(pMover, &pInven, false);
        if (!pInven)
            return false;

        TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, 0x31B5D488u);
        if (!pTB_ITEM)
            return false;

        TB_ITEM_CLASSIFY* pTB_CLASSIFY = XResourceMgr::GetTB_ITEM_CLASSIFY(&pServer->m_xResourceMgr, pTB_ITEM->Item_Classify_Index);
        if (!pTB_CLASSIFY)
            return false;

        CGocInventory* pInvenPtr = pInven.operator->();
        XBank* pBaseInven = CGocInventory::GetInvenPtr(pInvenPtr, pTB_CLASSIFY->Item_Inven_Type);
        if (!pBaseInven)
            return false;

        std::vector<std::tr1::shared_ptr<CItem>> vecItem;
        pBaseInven->GetSameItems_2(834000008, &vecItem, -1);

        bool bFind = false;
        for (size_t i = 0; i < vecItem.size(); ++i)
        {
            CItem* pItem = vecItem[i].operator->();
            if (pItem && pItem->GetCount() > 0)
            {
                __int16 Slot = pItem->GetSlot();
                if (!pBaseInven->GetLock(Slot))
                {
                    xSerial_Commission = pItem->GetSerial();
                    bFind = true;
                    break;
                }
            }
        }

        if (!bFind)
            return false;
    }

    // Check count cash item
    if (psReq.byCash_Count)
    {
        CMover* pMover = GetOwnerMover(this);
        std::tr1::shared_ptr<CGocInventory> pInven;
        CMover::GetGOC<CGocInventory>(pMover, &pInven, false);
        if (!pInven)
            return false;

        TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, 0x31B5D489u);
        if (!pTB_ITEM)
            return false;

        TB_ITEM_CLASSIFY* pTB_CLASSIFY = XResourceMgr::GetTB_ITEM_CLASSIFY(&pServer->m_xResourceMgr, pTB_ITEM->Item_Classify_Index);
        if (!pTB_CLASSIFY)
            return false;

        CGocInventory* pInvenPtr = pInven.operator->();
        XBank* pBaseInven = CGocInventory::GetInvenPtr(pInvenPtr, pTB_CLASSIFY->Item_Inven_Type);
        if (!pBaseInven)
            return false;

        std::vector<std::tr1::shared_ptr<CItem>> vecItem;
        pBaseInven->GetSameItems_2(834000009, &vecItem, -1);

        bool bFind = false;
        for (size_t j = 0; j < vecItem.size(); ++j)
        {
            CItem* pItem = vecItem[j].operator->();
            if (pItem && pItem->GetCount() > 0)
            {
                __int16 Slot = pItem->GetSlot();
                if (!pBaseInven->GetLock(Slot))
                {
                    xSerial_Count = pItem->GetSerial();
                    bFind = true;
                    break;
                }
            }
        }

        if (!bFind)
            return false;
    }

    // Check expire cash item
    if (psReq.byCash_Expire)
    {
        CMover* pMover = GetOwnerMover(this);
        std::tr1::shared_ptr<CGocInventory> pInven;
        CMover::GetGOC<CGocInventory>(pMover, &pInven, false);
        if (!pInven)
            return false;

        TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, 0x31B5D48Au);
        if (!pTB_ITEM)
            return false;

        TB_ITEM_CLASSIFY* pTB_CLASSIFY = XResourceMgr::GetTB_ITEM_CLASSIFY(&pServer->m_xResourceMgr, pTB_ITEM->Item_Classify_Index);
        if (!pTB_CLASSIFY)
            return false;

        CGocInventory* pInvenPtr = pInven.operator->();
        XBank* pBaseInven = CGocInventory::GetInvenPtr(pInvenPtr, pTB_CLASSIFY->Item_Inven_Type);
        if (!pBaseInven)
            return false;

        std::vector<std::tr1::shared_ptr<CItem>> vecItem;
        pBaseInven->GetSameItems_2(834000010, &vecItem, -1);

        bool bFind = false;
        for (size_t k = 0; k < vecItem.size(); ++k)
        {
            CItem* pItem = vecItem[k].operator->();
            if (pItem && pItem->GetCount() > 0)
            {
                __int16 Slot = pItem->GetSlot();
                if (!pBaseInven->GetLock(Slot))
                {
                    xSerial_Expire = pItem->GetSerial();
                    bFind = true;
                    break;
                }
            }
        }

        if (!bFind)
            return false;
    }

    return true;
}

// ============================================================================
// IDA: ?ResExchangeSearch@CGocExchange@@QEAAXAEAUPS_EXCHANGE_SEARCH_RES@@@Z (0x140079590)
// Send search results to client
// IDA 反编译:
// void __fastcall CGocExchange::ResExchangeSearch(CGocExchange *this, PS_EXCHANGE_SEARCH_RES *psSearch)
// {
//   XSendPacket xSendPacket;
//   XSendPacket::XSendPacket(&xSendPacket, 0x2Bu, 1u);  // Main=0x2B, Sub=1
//   operator<<(&xSendPacket, psSearch);
//   // 获取 owner actor
//   CGocNetwork::Send(pActor, &xSendPacket);
// }
// ============================================================================
void CGocExchange::ResExchangeSearch(PS_EXCHANGE_SEARCH_RES& psSearch)
{
    // IDA: 0x140079590 - Send search results to client
    CMover* pMover = GetOwnerMover(this);
    if (!pMover)
        return;
    
    XSendPacket xSendPacket(0x2B, 1);
    xSendPacket << psSearch;
    CGocNetwork::Send(pMover, xSendPacket);
}

// ============================================================================
// IDA: ?SendExchangeMyList@CGocExchange@@QEAAXXZ (0x14007D380)
// Send my exchange list to client
// IDA 反编译:
// void __fastcall CGocExchange::SendExchangeMyList(CGocExchange *this)
// {
//   PS_EXCHANGE_MY_LIST_RES psList;
//   XSendPacket xSendPacket;
//
//   PS_EXCHANGE_MY_LIST_RES::PS_EXCHANGE_MY_LIST_RES(&psList);
//   CGocExchange::GetExchangeMyList(this, &psList.vecMyList);
//   XSendPacket::XSendPacket(&xSendPacket, 0x2Bu, 8u);  // Main=0x2B, Sub=8
//   operator<<(&xSendPacket, &psList);
//   CGocNetwork::Send(pActor, &xSendPacket);
//   PS_EXCHANGE_MY_LIST_RES::~PS_EXCHANGE_MY_LIST_RES(&psList);
// }
// ============================================================================
// Shutdown and Update - not in IDA, placeholder implementations
// ============================================================================
void CGocExchange::Shutdown()
{
    Clear();
}

void CGocExchange::Update(float fDeltaTime)
{
    // No update logic needed based on IDA analysis
    (void)fDeltaTime;
}

// ============================================================================
// IDA: ?ReqExchangePriceHistory@CGocExchange@@QEAAXAEAUPS_EXCHANGE_PRICE_HISTORY_REQ@@@Z (0x1400762E0)
// Request price history for an item
// ============================================================================
void CGocExchange::ReqExchangePriceHistory(PS_EXCHANGE_PRICE_HISTORY_REQ& psHistory)
{
    // IDA decompiled from 0x1400762E0
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!XResourceMgr::GetServerContents(&pServer->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE))
    {
        CMover* pMover = GetOwnerMover(this);
        CGocNetwork::SendErrorMessage(pMover, 0x2B, 2, 0xC3B8);
        return;
    }

    // Get UCID from owner
    CMover* pMover = GetOwnerMover(this);
    if (pMover)
    {
        psHistory.dwUCID = pMover->GetActorID().GetID();
    }

    // Send packet to community server (Main=0xF3, Sub=0x28)
    XSendPacket xSendPacket(0xF3, 0x28);
    xSendPacket << psHistory;

    // Get CUser via RTTI
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (pUser)
    {
        CCommunitySocket::SendCmd(&pServer->m_communitySocket, xSendPacket, pUser, 0x2B, 2);
    }
}

// ============================================================================
// IDA: ?ReqExchangeInterestList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_INTEREST_LIST_REQ@@@Z (0x140076450)
// Request interest list
// IDA 反编译:
// void __fastcall CGocExchange::ReqExchangeInterestList(CGocExchange *this, PS_EXCHANGE_INTEREST_LIST_REQ *psReq)
// {
//   XGameServer *v2 = TXSingleton<XGameServer>::Instance();
//   if (XResourceMgr::GetServerContents(&v2->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE)) {
//     if (this->m_bLoadMyInterestList)
//       CGocExchange::SendExchangeMyInterestList(this);
//     else
//       CGocExchange::DBReqExchangeInterestList(this);
//   } else {
//     v3 = (CMover *)std::list<CBattleZone *>::size((VChunkLocker *)this);
//     CGocNetwork::SendErrorMessage(v3, 0x2Bu, 3u, 0xC3B8u);
//   }
// }
// ============================================================================
void CGocExchange::ReqExchangeInterestList(PS_EXCHANGE_INTEREST_LIST_REQ& psReq)
{
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!XResourceMgr::GetServerContents(&pServer->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE))
    {
        CMover* pMover = GetOwnerMover(this);
        CGocNetwork::SendErrorMessage(pMover, 0x2B, 3, 0xC3B8);
        return;
    }

    // 如果已加载，直接发送给客户端
    if (m_bLoadMyInterestList)
    {
        SendExchangeMyInterestList();
    }
    else
    {
        // 否则从DB请求
        DBReqExchangeInterestList();
    }
    (void)psReq;
}

// ============================================================================
// IDA: ?ReqExchangeInterestItem@CGocExchange@@QEAAXAEAUPS_EXCHANGE_INTEREST_ITEM_REQ@@@Z (0x1400764D0)
// Add/remove interest item
// ============================================================================
void CGocExchange::ReqExchangeInterestItem(PS_EXCHANGE_INTEREST_ITEM_REQ& psReq)
{
    // IDA decompiled from 0x1400764D0
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!XResourceMgr::GetServerContents(&pServer->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE))
    {
        CMover* pMover = GetOwnerMover(this);
        CGocNetwork::SendErrorMessage(pMover, 0x2B, 4, 0xC3B8);
        return;
    }

    if (!m_bLoadMyInterestList)
        return;

    if (psReq.byReason == 1)
    {
        // Add item - check max 10 items
        if (m_vecMyInterestList.size() >= 10)
        {
            PS_EXCHANGE_INTEREST_ITEM_RES psResult;
            psResult.dwUCID = psReq.dwExitUCID;
            psResult.dwItemID = psReq.dwExitUAID;
            psResult.nResult = 58306;

            XSendPacket xSendPacket(0x2B, 4);
            xSendPacket << psResult;
            CMover* pMover = GetOwnerMover(this);
            CGocNetwork::Send(pMover, xSendPacket);
            return;
        }

        // Check if item already exists
        for (size_t i = 0; i < m_vecMyInterestList.size(); ++i)
        {
            if (psReq.dwExitUAID == m_vecMyInterestList[i])
                return;
        }
    }
    else
    {
        // Remove item - check if exists
        bool bRet = false;
        for (size_t j = 0; j < m_vecMyInterestList.size(); ++j)
        {
            if (psReq.dwExitUAID == m_vecMyInterestList[j])
            {
                bRet = true;
                break;
            }
        }

        if (!bRet)
            return;
    }

    // Send DB request (Main=0x27, Sub=4)
    CMover* pMover = GetOwnerMover(this);
    XSendDBPacket xSendDBPacket(pMover, 0x27, 4);
    xSendDBPacket << psReq;
    pServer->SendDBGame(xSendDBPacket);
}

// ============================================================================
// IDA: ?ResExchangeMyInterestList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_INTEREST_LIST_RES@@@Z (0x140079660)
// Handle interest list response from DB
// IDA 反编译:
// void __fastcall CGocExchange::ResExchangeMyInterestList(CGocExchange *this, PS_EXCHANGE_INTEREST_LIST_RES *psMyList)
// {
//   std::vector<float>::clear((std::vector<float> *)&this->m_vecMyInterestList);
//   std::vector<unsigned long>::operator=(&this->m_vecMyInterestList, &psMyList->vecItemList);
//   this->m_bLoadMyInterestList = 1;
//   if (__PAIR64__(this->m_bLoadMyInterestList, this->m_bLoadMyList) == 0x100000001LL) {
//     // Both lists loaded, set UserDB flag
//     pUser = (CUser *)_RTDynamicCast_0(v2, 0, &CMover RTTI, &CUser RTTI, 0);
//     if (pUser) {
//       *v3 |= 4u;  // Set UserDB flag
//     }
//   }
//   if (XResourceMgr::GetServerContents(&v4->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE))
//     CGocExchange::SendExchangeMyInterestList(this);
// }
// ============================================================================
void CGocExchange::ResExchangeMyInterestList(PS_EXCHANGE_INTEREST_LIST_RES& psMyList)
{
    // Clear and copy interest list
    m_vecMyInterestList.clear();
    m_vecMyInterestList = psMyList.vecItemList;
    m_bLoadMyInterestList = true;

    // Check if both lists are loaded (m_bLoadMyList == true && m_bLoadMyInterestList == true)
    if (m_bLoadMyList && m_bLoadMyInterestList)
    {
        // Set UserDB flag via CUser
        CMover* pMover = GetOwnerMover(this);
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser)
        {
            pUser->SetUserDBFlag(4);  // Set UserDB flag bit 2
        }
    }

    // Send to client if exchange is enabled
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (XResourceMgr::GetServerContents(&pServer->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE))
    {
        SendExchangeMyInterestList();
    }
}

// ============================================================================
// IDA: ?ResExchangeMyInterestItem@CGocExchange@@QEAAXAEAUPS_EXCHANGE_INTEREST_ITEM_RES@@@Z (0x140079750)
// Handle interest item response from DB
// IDA 反编译:
// void __fastcall CGocExchange::ResExchangeMyInterestItem(CGocExchange *this, PS_EXCHANGE_INTEREST_ITEM_RES *psResult)
// {
//   if (!psResult->nSyncCount) {
//     if (psResult->bSync) {
//       // Add item
//       std::vector<unsigned long>::push_back(&this->m_vecMyInterestList, &psResult->dwUCID);
//     } else {
//       // Remove item - find and erase
//       for (auto& item : this->m_vecMyInterestList) {
//         if (item == psResult->dwUCID) {
//           erase(item);
//           break;
//         }
//       }
//     }
//   }
//   // Send response to client (Main=0x2B, Sub=4)
//   XSendPacket::XSendPacket(&xSendPacket, 0x2Bu, 4u);
//   operator<<(&xSendPacket, psResult);
//   CGocNetwork::Send(pActor, &xSendPacket);
// }
// ============================================================================
void CGocExchange::ResExchangeMyInterestItem(PS_EXCHANGE_INTEREST_ITEM_RES& psResult)
{
    // If not a sync count operation
    if (psResult.nSyncCount == 0)
    {
        if (psResult.bSync)
        {
            // Add item to interest list
            m_vecMyInterestList.push_back(psResult.dwUCID);
        }
        else
        {
            // Remove item from interest list
            for (auto it = m_vecMyInterestList.begin(); it != m_vecMyInterestList.end(); ++it)
            {
                if (*it == psResult.dwUCID)
                {
                    m_vecMyInterestList.erase(it);
                    break;
                }
            }
        }
    }

    // Send response to client (Main=0x2B, Sub=4)
    CMover* pMover = GetOwnerMover(this);
    XSendPacket xSendPacket(0x2B, 4);
    xSendPacket << psResult;
    CGocNetwork::Send(pMover, xSendPacket);
}

// ============================================================================
// IDA: ?SendExchangeMyInterestList@CGocExchange@@QEAAXXZ (0x14007D470)
// Send interest list to client
// IDA 反编译:
// void __fastcall CGocExchange::SendExchangeMyInterestList(CGocExchange *this)
// {
//   PS_EXCHANGE_INTEREST_LIST_RES psList;
//   PS_EXCHANGE_INTEREST_LIST_RES::PS_EXCHANGE_INTEREST_LIST_RES(&psList);
//   // Get UCID from owner
//   psList.dwUCID = CQuestCondition::GetQuestID(v3);
//   CGocExchange::GetExchangeMyInterestList(this, &psList);
//   XSendPacket::XSendPacket(&xSendPacket, 0x2Bu, 3u);  // Main=0x2B, Sub=3
//   operator<<(&xSendPacket, &psList);
//   CGocNetwork::Send(pActor, &xSendPacket);
// }
// ============================================================================
void CGocExchange::SendExchangeMyInterestList()
{
    // IDA: 0x14007D470 - Send interest list to client
    CMover* pMover = GetOwnerMover(this);
    if (!pMover)
        return;
    
    PS_EXCHANGE_INTEREST_LIST_RES psList;
    psList.dwUCID = pMover->GetActorID().GetID();
    GetExchangeMyInterestList(psList);
    
    XSendPacket xSendPacket(0x2B, 3);
    xSendPacket << psList;
    CGocNetwork::Send(pMover, xSendPacket);
}

// ============================================================================
// IDA: ?SendExchangeMyList@CGocExchange@@QEAAXXZ (0x14007D380)
// Send my exchange list to client
// IDA 反编译:
// void __fastcall CGocExchange::SendExchangeMyList(CGocExchange *this)
// {
//   PS_EXCHANGE_MY_LIST_RES psList;
//   PS_EXCHANGE_MY_LIST_RES::PS_EXCHANGE_MY_LIST_RES(&psList);
//   CGocExchange::GetExchangeMyList(this, &psList.vecMyList);
//   XSendPacket::XSendPacket(&xSendPacket, 0x2Bu, 8u);  // Main=0x2B, Sub=8
//   operator<<(&xSendPacket, &psList);
//   CGocNetwork::Send(pActor, &xSendPacket);
//   PS_EXCHANGE_MY_LIST_RES::~PS_EXCHANGE_MY_LIST_RES(&psList);
// }
// ============================================================================
void CGocExchange::SendExchangeMyList()
{
    // IDA: 0x14007D380 - Send my exchange list to client
    CMover* pMover = GetOwnerMover(this);
    if (!pMover)
        return;
    
    PS_EXCHANGE_MY_LIST_RES psList;
    GetExchangeMyList(psList.vecMyList);
    
    XSendPacket xSendPacket(0x2B, 8);
    xSendPacket << psList;
    CGocNetwork::Send(pMover, xSendPacket);
}

// ============================================================================
// IDA: ?DBReqExchangeMyList@CGocExchange@@QEAAXXZ (0x14007D5B0)
// Request my exchange list from DB
// IDA 反编译:
// void __fastcall CGocExchange::DBReqExchangeMyList(CGocExchange *this)
// {
//   PS_EXCHANGE_MY_LIST_REQ psMyList;
//   PS_RECRUIT_DELETE::PS_RECRUIT_DELETE(&psMyList);
//   // Get UCID from owner
//   psMyList.dwUCID = CQuestCondition::GetQuestID(v1);
//   XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x27u, 8u);  // Main=0x27, Sub=8
//   operator<<(&xSendDBPacket, &psMyList);
//   XGameServer::SendDBGame(v2, &xSendDBPacket);
// }
// ============================================================================
void CGocExchange::DBReqExchangeMyList()
{
    // IDA: 0x14007D5B0 - Request my exchange list from DB
    CMover* pMover = GetOwnerMover(this);
    if (!pMover)
        return;
    
    PS_EXCHANGE_MY_LIST_REQ psMyList;
    memset(&psMyList, 0, sizeof(psMyList));
    psMyList.dwUCID = pMover->GetActorID().GetID();
    
    XSendDBPacket xSendDBPacket(pMover, 0x27, 8);
    xSendDBPacket << psMyList;
    XGameServer::Instance()->SendDBGame(xSendDBPacket);
}

// ============================================================================
// IDA: ?DBReqExchangeInterestList@CGocExchange@@QEAAXXZ (0x14007D6D0)
// Request interest list from DB
// IDA 反编译:
// void __fastcall CGocExchange::DBReqExchangeInterestList(CGocExchange *this)
// {
//   PS_EXCHANGE_INTEREST_LIST_REQ psReq;
//   PS_RECRUIT_DELETE::PS_RECRUIT_DELETE(&psReq);
//   // Get UCID from owner
//   psReq.dwUCID = CQuestCondition::GetQuestID(v1);
//   XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x27u, 3u);  // Main=0x27, Sub=3
//   operator<<(&xSendDBPacket, &psReq);
//   XGameServer::SendDBGame(v2, &xSendDBPacket);
// }
// ============================================================================
void CGocExchange::DBReqExchangeInterestList()
{
    // IDA: 0x14007D6D0 - Request interest list from DB
    CMover* pMover = GetOwnerMover(this);
    if (!pMover)
        return;
    
    PS_EXCHANGE_INTEREST_LIST_REQ psReq;
    memset(&psReq, 0, sizeof(psReq));
    psReq.dwUCID = pMover->GetActorID().GetID();
    
    XSendDBPacket xSendDBPacket(pMover, 0x27, 3);
    xSendDBPacket << psReq;
    XGameServer::Instance()->SendDBGame(xSendDBPacket);
}

// ============================================================================
// IDA: ?ResExchangeItemBuyCheck@CGocExchange@@QEAAXAEAUPS_EXCHANGE_ITEM_BUY_REQ@@_JH@Z (0x14007ADE0)
// Check exchange item buy before processing
// ============================================================================
void CGocExchange::ResExchangeItemBuyCheck(PS_EXCHANGE_ITEM_BUY_REQ& psReq, std::int64_t biPrice, int nPackageCount)
{
    // IDA decompiled from 0x14007ADE0
    m_bSendBuyPacket = false;

    PS_DB_EXCHANGE_ITEM_BUY psDBReq;
    memset(&psDBReq, 0, sizeof(psDBReq));
    psDBReq.dwExchangeID = psReq.dwExchangeID;
    psDBReq.shCount = psReq.shCount;

    CMover* pMover = GetOwnerMover(this);
    if (pMover)
    {
        psDBReq.dwUCID = pMover->GetActorID().GetID();
    }

    // Validate price
    if (biPrice < 0)
    {
        PS_EXCHANGE_ITEM_BUY_RES psResult;
        psResult.nResult = 58310;
        psResult.dwExchangeID = psReq.dwExchangeID;
        psResult.shBuyCount = psReq.shCount;

        XSendPacket xSendPacket(0x2B, 6);
        xSendPacket << psResult;
        CGocNetwork::Send(pMover, xSendPacket);
        return;
    }

    // Get item table data
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, psReq.dwItemID);
    if (!pTB_ITEM || !biPrice)
    {
        PS_EXCHANGE_ITEM_BUY_RES psResult;
        psResult.nResult = 52002;
        psResult.dwExchangeID = psReq.dwExchangeID;
        psResult.shBuyCount = psReq.shCount;

        XSendPacket xSendPacket(0x2B, 6);
        xSendPacket << psResult;
        CGocNetwork::Send(pMover, xSendPacket);
        return;
    }

    // Check count validity
    if (psDBReq.shCount <= 0)
    {
        PS_EXCHANGE_ITEM_BUY_RES psResult;
        psResult.nResult = 52014;
        psResult.dwExchangeID = psReq.dwExchangeID;
        psResult.shBuyCount = psReq.shCount;

        XSendPacket xSendPacket(0x2B, 6);
        xSendPacket << psResult;
        CGocNetwork::Send(pMover, xSendPacket);
        return;
    }

    if (pTB_ITEM->Item_Stack_Max < psReq.shCount)
    {
        PS_EXCHANGE_ITEM_BUY_RES psResult;
        psResult.nResult = 52014;
        psResult.dwExchangeID = psReq.dwExchangeID;
        psResult.shBuyCount = psReq.shCount;

        XSendPacket xSendPacket(0x2B, 6);
        xSendPacket << psResult;
        CGocNetwork::Send(pMover, xSendPacket);
        return;
    }

    psDBReq.byItemType = (pTB_ITEM->Item_Stack_Max != 1) ? 1 : 0;

    // Get inventory component
    std::tr1::shared_ptr<CGocInventory> pInvenPtr;
    CMover::GetGOC<CGocInventory>(pMover, &pInvenPtr, false);
    if (pInvenPtr)
    {
        CGocInventory* pInven = pInvenPtr.operator->();

        // Check money
        if (CGocInventory::GetMoney(pInven) < biPrice)
        {
            PS_EXCHANGE_ITEM_BUY_RES psResult;
            psResult.nResult = 52251;
            psResult.dwExchangeID = psReq.dwExchangeID;
            psResult.shBuyCount = psReq.shCount;

            XSendPacket xSendPacket(0x2B, 6);
            xSendPacket << psResult;
            CGocNetwork::Send(pMover, xSendPacket);
            return;
        }

        // Check package count if reseal package
        if (CGocInventory::IsResealPackage(pInven, pTB_ITEM->Item_ID))
        {
            if (!CGocInventory::IsResealPackageCount(pInven, pTB_ITEM->Item_ID, nPackageCount))
            {
                PS_EXCHANGE_ITEM_BUY_RES psResult;
                psResult.nResult = 52253;
                psResult.dwExchangeID = psReq.dwExchangeID;
                psResult.shBuyCount = psReq.shCount;

                XSendPacket xSendPacket(0x2B, 6);
                xSendPacket << psResult;
                CGocNetwork::Send(pMover, xSendPacket);
                return;
            }
        }

        // Deduct money
        __int64 nMoney = CGocInventory::GetMoney(pInven) - biPrice;
        CGocInventory::SetInvenMoney(pInven, nMoney, 0);
    }

    // Generate post serials
    UXSerial result;
    XItemFactory::GeneratSerial(&pServer->m_xItemFactory, &result);
    psDBReq.stPost[0].biSerial = result.GetSerial();

    psDBReq.stPost[0].byPostSubType = XGameServer::GetSystemPostTableIndex(pServer, 2, 1);

    UXSerial v47;
    XItemFactory::GeneratSerial(&pServer->m_xItemFactory, &v47);
    psDBReq.stPost[1].biSerial = v47.GetSerial();

    psDBReq.stPost[1].byPostSubType = XGameServer::GetSystemPostTableIndex(pServer, 2, 2);

    unsigned char byPostSubType_No_Commission = XGameServer::GetSystemPostTableIndex(pServer, 2, 5);

    // Send DB request (Main=0x27, Sub=6)
    XSendDBPacket xSendDBPacket(pMover, 0x27, 6);
    xSendDBPacket << psDBReq;
    xSendDBPacket.XParse << byPostSubType_No_Commission;
    pServer->SendDBGame(xSendDBPacket);
    m_bSendBuyPacket = true;
}

// ============================================================================
// IDA: ?ResExchangeItemBuy@CGocExchange@@QEAAXAEAUPS_DB_EXCHANGE_ITEM_BUY@@@Z (0x14007B710)
// Handle exchange item buy response from DB
// ============================================================================
void CGocExchange::ResExchangeItemBuy(PS_DB_EXCHANGE_ITEM_BUY& psRes)
{
    // IDA decompiled from 0x14007B710
    m_bSendBuyPacket = false;

    PS_EXCHANGE_ITEM_BUY_RES psResult;
    psResult.nResult = psRes.nResult;
    psResult.dwExchangeID = psRes.dwExchangeID;
    psResult.shBuyCount = psRes.shCount;

    CMover* pMover = GetOwnerMover(this);

    if (psRes.nResult)
    {
        // Error case - refund money
        std::tr1::shared_ptr<CGocInventory> pInvenPtr;
        CMover::GetGOC<CGocInventory>(pMover, &pInvenPtr, false);
        if (pInvenPtr)
        {
            CGocInventory* pInven = pInvenPtr.operator->();
            __int64 nMoney = psRes.nSellPrice + CGocInventory::GetMoney(pInven);
            CGocInventory::SetInvenMoney(pInven, nMoney, 0);
        }

        // Map error codes
        int nResult = psRes.nResult;
        if (nResult == 1)
            psResult.nResult = 58305;
        else if (nResult == 2)
            psResult.nResult = 52251;
        else if (nResult == 3)
            psResult.nResult = 52002;
        else if (nResult == 4)
            psResult.nResult = 52014;
        else if (nResult == 5)
            psResult.nResult = 58304;
    }
    else
    {
        // Success case - add post items
        std::tr1::shared_ptr<CGocPost> pRecvPost;
        CMover::GetGOC<CGocPost>(pMover, &pRecvPost, false);
        if (pRecvPost)
        {
            unsigned short wPostCount = psRes.nRecvCount[0];
            CGocPost* pPost = pRecvPost.operator->();
            pPost->SetConditionValue(wPostCount);

            // Check random options
            std::tr1::shared_ptr<CGocInventory> pResultInven;
            CMover::GetGOC<CGocInventory>(pMover, &pResultInven, false);
            if (pResultInven)
            {
                CGocInventory* pInven = pResultInven.operator->();
                for (int i = 0; i < 5; ++i)
                {
                    STItem* pStOptinChangeInfo = &psRes.stPost[0].stItemList[i];
                    if (CGocInventory::CheckRandomOption(pInven, pStOptinChangeInfo))
                    {
                        LogHelper::LogError("game.item", "ResExchangeItemBuy - Change item random option[UCID:%d]", pMover->GetActorID().GetID());
                    }
                }
            }

            // Add post
            ST_POST_DATA stPostData;
            ST_POST_DATA::ST_POST_DATA(&stPostData, psRes.stPost);
            pPost->AddRecvPost(&stPostData);

            // Send post packet (Main=0x20, Sub=9)
            XSendPacket packet(0x20, 9);
            packet.XParse << wPostCount;
            packet << psRes.stPost;
            CGocNetwork::Send(pMover, packet);
        }

        // Send money update
        std::tr1::shared_ptr<CGocInventory> pInvenPtr;
        CMover::GetGOC<CGocInventory>(pMover, &pInvenPtr, false);
        if (pInvenPtr)
        {
            CGocInventory* pInven = pInvenPtr.operator->();
            CGocInventory::SendMoney(pInven);

            // Log money change
            CUser* pUser = dynamic_cast<CUser*>(pMover);
            if (pUser)
            {
                ST_LOG_GAME stLog;
                stLog._nUAID = pUser->GetUAID();
                stLog._nUCID = pMover->GetActorID().GetID();
                stLog._sMainType = 10;
                stLog._sSubType = 1;
                stLog.nParam1 = pUser->GetLevel();
                stLog.nParam2 = 44;
                stLog.nParam5 = -psRes.nSellPrice;
                stLog.nParam6 = CGocInventory::GetMoney(pInven);
                wcscpy_s(stLog.szComment, L"MONEY");
                XGameServer::Instance()->SendDBLog(stLog);
            }
        }

        // Send price history update (Main=0xF3, Sub=0x29)
        PS_EXCHANGE_PRICE_HISTORY_UPDATE psUpdate;
        psUpdate.dwSellerUCID = psRes.dwSellerUCID;
        psUpdate.dwItemID = psRes.stPost[0].stItemList[0].nItemID;
        psUpdate.dwExchangeID = psRes.dwExchangeID;
        psUpdate.sSellCount = psRes.shCount;

        __int64 nSellPrice = (psRes.nSellPrice > 0) ? psRes.nSellPrice : 1;
        int shCount = (psRes.shCount > 0) ? psRes.shCount : 1;
        psUpdate.nPrice_One = nSellPrice / shCount;

        ATL::CTime tCurr = ATL::CTime::GetTickCount();
        psUpdate.tRegDate = tCurr.GetTime();

        ST_POST_DATA::operator=(&psUpdate.stPost, &psRes.stPost[1]);
        psUpdate.wSellerRecvPostCount = psRes.nRecvCount[1];
        wcscpy_s(psUpdate.strBuyerName, psRes.stPost[0].stCharInfo.strName);

        XSendPacket v43(0xF3, 0x29);
        v43 << psUpdate;
        XGameServer::Instance()->m_communitySocket.SendCheck(v43);

        // Update achieve
        std::tr1::shared_ptr<CGocAchieve> pAchieve;
        CMover::GetGOC<CGocAchieve>(pMover, &pAchieve, false);
        if (pAchieve)
        {
            CGocAchieve* pAchievePtr = pAchieve.operator->();
            CGocAchieve::UpdateCollect(pAchievePtr, 0x37, 1, 0);
        }

        // Log exchange buy
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser)
        {
            ST_LOG_GAME stLogGame;
            stLogGame._nUAID = pUser->GetUAID();
            stLogGame._nUCID = pMover->GetActorID().GetID();
            stLogGame._sMainType = 18;
            stLogGame._sSubType = 3;
            stLogGame.nParam0 = pUser->GetLevel();
            stLogGame.nParam1 = psUpdate.dwItemID;
            stLogGame.nParam2 = psUpdate.sSellCount;
            stLogGame.nParam3 = psRes.dwSellerUCID;
            stLogGame.nParam4 = psRes.dwExchangeID;
            stLogGame.nParam5 = psRes.stPost[0].stItemList[0].xSerial;
            stLogGame.nParam6 = psUpdate.nPrice_One;
            wcscpy_s(stLogGame.szComment, L"EXCHANGE_BUY");
            XGameServer::Instance()->SendDBLog(stLogGame);

            ST_LOG_GAME stLog_buy;
            stLog_buy._nUAID = psRes.dwSellerUAID;
            stLog_buy._nUCID = psRes.dwSellerUCID;
            stLog_buy._sMainType = 18;
            stLog_buy._sSubType = 6;
            stLog_buy.nParam0 = 0;
            stLog_buy.nParam1 = psUpdate.dwItemID;
            stLog_buy.nParam2 = psUpdate.sSellCount;
            stLog_buy.nParam3 = pMover->GetActorID().GetID();
            stLog_buy.nParam4 = psRes.dwExchangeID;
            stLog_buy.nParam5 = psRes.stPost[0].stItemList[0].xSerial;
            stLog_buy.nParam6 = psUpdate.nPrice_One;
            wcscpy_s(stLog_buy.szComment, L"EXCHANGE_SELL");
            XGameServer::Instance()->SendDBLog(stLog_buy);
        }

        // Send statistics
        ST_STATISTICS_ITEM_EXCHANGE stStatistics;
        stStatistics.byFlag = 3;
        stStatistics.dwEXC_ID = psRes.dwExchangeID;
        stStatistics.dwUCID = pMover->GetActorID().GetID();
        stStatistics.biSerial = psRes.stPost[0].stItemList[0].xSerial;

        XSendDBPacket xSendDBStatistics(pMover, 0xF0, 3);
        xSendDBStatistics << stStatistics;
        XGameServer::Instance()->SendDBStatistics(xSendDBStatistics);
    }

    // Send result packet (Main=0x2B, Sub=6)
    XSendPacket xSendPacket(0x2B, 6);
    xSendPacket << psResult;
    CGocNetwork::Send(pMover, xSendPacket);
}

// ============================================================================
// IDA: ?ResExchangeItemRecall@CGocExchange@@QEAAXAEAUPS_DB_EXCHANGE_ITEM_RECALL_RES@@@Z (0x14007C9C0)
// Handle exchange item recall response from DB
// IDA 反编译 (关键逻辑):
// void __fastcall CGocExchange::ResExchangeItemRecall(CGocExchange *this, PS_DB_EXCHANGE_ITEM_RECALL_RES *psRes)
// {
//   psResult.nResult = psRes->nResult;
//   psResult.xSerial = psRes->stPost.stItemList[0].xSerial;
//   // Find in m_mapMyList
//   std::_Tree::find(&this->m_mapMyList, &iter, &psRes->stPost.stItemList[0].xSerial);
//   if (iter != end) {
//     stExchangeInfo.nPrice_One = iter->second.nPrice_One;
//     stExchangeInfo.dwExchangeID = iter->second.dwExchangeID;
//     stExchangeInfo.byState = iter->second.byState;
//     // Erase from map
//     std::_Tree::erase(&this->m_mapMyList, &v35, iter);
//   }
//   // Add post item
//   CGocPost::AddRecvPost(v10, v44);
//   // Send post packet (Main=0x20, Sub=9)
//   // Send recall result (Main=0x2B, Sub=7)
//   // Log if CUser
//   if (stExchangeInfo.byState == 2) {
//     // Send statistics (Main=0xF0, Sub=3)
//   }
// }
// ============================================================================
void CGocExchange::ResExchangeItemRecall(PS_DB_EXCHANGE_ITEM_RECALL_RES& psRes)
{
    // Build result
    PS_EXCHANGE_ITEM_RECALL_RES psResult;
    psResult.nResult = psRes.nResult;
    psResult.xSerial = psRes.stPost.stItemList[0].xSerial;

    // Find and remove from m_mapMyList
    auto it = m_mapMyList.find(psRes.stPost.stItemList[0].xSerial);
    ST_MY_EXCHANGE_ITEM stExchangeInfo = {};

    if (it != m_mapMyList.end())
    {
        stExchangeInfo.nPrice_One = it->second.nPrice_One;
        stExchangeInfo.dwExchangeID = it->second.dwExchangeID;
        stExchangeInfo.byState = it->second.byState;
        m_mapMyList.erase(it);
    }

    CMover* pMover = GetOwnerMover(this);

    // Add post item to player
    std::tr1::shared_ptr<CGocPost> pRecvPost;
    CMover::GetGOC<CGocPost>(pMover, &pRecvPost, false);
    if (pRecvPost)
    {
        CGocPost* pPost = pRecvPost.operator->();
        pPost->SetConditionValue(psRes.wPostCount);

        // Check random options
        std::tr1::shared_ptr<CGocInventory> pInvenPtr;
        CMover::GetGOC<CGocInventory>(pMover, &pInvenPtr, false);
        if (pInvenPtr)
        {
            CGocInventory* pInven = pInvenPtr.operator->();
            for (int i = 0; i < 5; ++i)
            {
                STItem* stOptinChangeInfo = &psRes.stPost.stItemList[i];
                if (CGocInventory::CheckRandomOption(pInven, stOptinChangeInfo))
                {
                    LogHelper::LogError("game.item", "ResExchangeItemRecall - Change item random option[UCID:%d]", pMover->GetActorID().GetID());
                }
            }
        }

        // Add post
        ST_POST_DATA stPostData;
        ST_POST_DATA::ST_POST_DATA(&stPostData, &psRes.stPost);
        pPost->AddRecvPost(&stPostData);

        // Send post packet (Main=0x20, Sub=9)
        XSendPacket packet(0x20, 9);
        packet.XParse << psRes.wPostCount;
        packet << psRes.stPost;
        CGocNetwork::Send(pMover, packet);
    }

    // Send recall result packet (Main=0x2B, Sub=7)
    XSendPacket xSendPacket(0x2B, 7);
    xSendPacket << psResult;
    CGocNetwork::Send(pMover, xSendPacket);

    // Log if CUser
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (pUser)
    {
        ST_LOG_GAME stLog;
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = pMover->GetActorID().GetID();
        stLog._sMainType = 18;
        stLog._sSubType = 2;
        stLog.nParam0 = pUser->GetLevel();
        stLog.nParam1 = psRes.stPost.stItemList[0].nItemID;
        stLog.nParam2 = psRes.stPost.stItemList[0].sCount;
        stLog.nParam3 = stExchangeInfo.byState;
        stLog.nParam4 = stExchangeInfo.dwExchangeID;
        stLog.nParam5 = psRes.stPost.stItemList[0].xSerial;
        stLog.nParam6 = stExchangeInfo.nPrice_One;
        XGameServer::Instance()->SendDBLog(stLog);
    }

    // Send statistics if state == 2
    if (stExchangeInfo.byState == 2)
    {
        ST_STATISTICS_ITEM_EXCHANGE stStatistics;
        stStatistics.byFlag = 3;
        stStatistics.dwEXC_ID = stExchangeInfo.dwExchangeID;
        stStatistics.dwUCID = pMover->GetActorID().GetID();
        stStatistics.biSerial = psRes.stPost.stItemList[0].xSerial;

        XSendDBPacket xSendDBStatistics(pMover, 0xF0, 3);
        xSendDBStatistics << stStatistics;
        XGameServer::Instance()->SendDBStatistics(xSendDBStatistics);
    }
}

// ============================================================================
// IDA: ?ReqExchangeItemRecall@CGocExchange@@QEAAXAEAUPS_EXCHANGE_ITEM_RECALL_REQ@@@Z (0x140079250)
// Recall exchange item
// IDA 反编译摘要:
// 1. Check E_SERVER_OPTION_ITEM_EXCHANGE
// 2. Find item in m_mapMyList by xSerial
// 3. Create PS_DB_EXCHANGE_ITEM_RECALL_REQ with post serial
// 4. Send DB request (Main=0x27, Sub=7)
// ============================================================================
void CGocExchange::ReqExchangeItemRecall(PS_EXCHANGE_ITEM_RECALL_REQ& psReq)
{
    // IDA: 0x140079250 - Recall item from exchange marketplace
    // Decompile analysis shows:
    // 1. Check E_SERVER_OPTION_ITEM_EXCHANGE server option
    // 2. Find item in m_mapMyList by psReq.xSerial
    // 3. If found, create PS_DB_EXCHANGE_ITEM_RECALL_REQ and send to DB (Main=0x27, Sub=7)
    // 4. Calculate post expiry and set post subtype based on time remaining
    // 5. If not found, log error
    
    XGameServer* pServer = XGameServer::Instance();
    if (!XResourceMgr::GetServerContents(&pServer->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE))
    {
        CMover* pMover = GetOwnerMover(this);
        CGocNetwork::SendErrorMessage(pMover, 0x2B, 7, 0xC3B8);
        return;
    }
    
    auto iter = m_mapMyList.find(psReq.xSerial);
    if (iter == m_mapMyList.end())
    {
        int iCount = static_cast<int>(m_mapMyList.size());
        LogHelper::LogError("game.contents", "ReqExchangeItemRecall() No Item : %d, (map size : %d)", 
                           psReq.xSerial, iCount);
        return;
    }
    
    PS_DB_EXCHANGE_ITEM_RECALL_REQ psDBReq;
    memset(&psDBReq, 0, sizeof(psDBReq));
    
    psDBReq.dwUCID = GetOwnerMover(this)->GetUCID();
    psDBReq.byType = iter->second.byType;
    psDBReq.nPostSerial = XGameServer::Instance()->m_xItemFactory.GenerateSerial();
    psDBReq.dwExchangeID = iter->second.dwExchangeID;
    
    ATL::CTime tCurr = ATL::CTime::GetTickCount();
    if (iter->second.nExpireDate >= tCurr.GetTime())
    {
        psDBReq.byPostSubType = XGameServer::Instance()->GetSystemPostTableIndex(2, 3);
    }
    else
    {
        psDBReq.byPostSubType = XGameServer::Instance()->GetSystemPostTableIndex(2, 4);
    }
    
    XSendDBPacket xSendDBPacket(GetOwnerMover(this), 0x27, 7);
    xSendDBPacket << psDBReq;
    XGameServer::Instance()->SendDBGame(xSendDBPacket);
}

// ============================================================================
// IDA: ?ReqExchangeMyList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_MY_LIST_REQ@@@Z (0x140079510)
// Request my exchange list
// ============================================================================
void CGocExchange::ReqExchangeMyList(PS_EXCHANGE_MY_LIST_REQ& psMyList)
{
    if (m_bLoadMyList)
        SendExchangeMyList();
    else
        DBReqExchangeMyList();
    (void)psMyList;
}

// ============================================================================
// IDA: ?ResExchangeSellRegister@CGocExchange@@QEAAXAEAUPS_DB_EXCHANGE_SELL_REGISTER@@@Z (0x140079930)
// Handle sell register response from DB (complex ~4KB)
// ============================================================================
void CGocExchange::ResExchangeSellRegister(PS_DB_EXCHANGE_SELL_REGISTER& psRes)
{
    // IDA: 0x140079930 - Handle sell register response from DB (complex ~4KB function)
    // Full IDA decompile analysis:
    // 1. Get inventory component and validate
    // 2. Unlock items in psRes->stUpdateItem list
    // 3. Unlock commission, count, and expire items if provided
    // 4. If nResult is success:
    //    - Add money if nAddCostMoney > 0
    // 5. If nResult is failure:
    //    - Create ST_MY_EXCHANGE_ITEM from psRes->stCreateItem
    //    - Set expiry and open dates based on current time + wait times
    //    - Set state to 2 (selling)
    //    - Copy item info from inventory
    //    - Insert into m_mapMyList
    //    - Handle commission/count/expire items (delete or reduce count)
    //    - Update inventory items
    //    - Log and send statistics
    // 6. Send result packet (Main=0x2B, Sub=5)
    
    // Get inventory component
    CMover* pMover = GetOwnerMover(this);
    std::tr1::shared_ptr<CGocInventory> pInven;
    CMover::GetGOC<CGocInventory>(pMover, &pInven, false);
    
    if (!pInven)
        return;
    
    CGocInventory* pInvenPtr = pInven.operator->();
    XBaseInventory* pBaseInven = pInvenPtr->GetInvenPtr(psRes.byInvenType);
    if (!pBaseInven)
        return;
    
    // Unlock items in update list
    for (size_t i = 0; i < psRes.stUpdateItem.size(); ++i)
    {
        unsigned int dwActorID = psRes.stUpdateItem[i].uxActorID.dwActorID;
        pBaseInven->SetLock(HIWORD(dwActorID), 0);
    }
    
    // Unlock commission/count/expire items
    if (psRes.xSerial_Commission)
    {
        std::tr1::shared_ptr<CItem> pItem;
        pInvenPtr->GetItemPtr(&pItem, psRes.xSerial_Commission);
        if (pItem)
        {
            int Slot = pItem->GetSlot();
            unsigned char InvenType = pItem->GetInvenType();
            pInvenPtr->SetLock(InvenType, Slot, 0);
        }
    }
    
    if (psRes.xSerial_Count)
    {
        std::tr1::shared_ptr<CItem> pItem;
        pInvenPtr->GetItemPtr(&pItem, psRes.xSerial_Count);
        if (pItem)
        {
            int Slot = pItem->GetSlot();
            unsigned char InvenType = pItem->GetInvenType();
            pInvenPtr->SetLock(InvenType, Slot, 0);
        }
    }
    
    if (psRes.xSerial_Expire)
    {
        std::tr1::shared_ptr<CItem> pItem;
        pInvenPtr->GetItemPtr(&pItem, psRes.xSerial_Expire);
        if (pItem)
        {
            int Slot = pItem->GetSlot();
            unsigned char InvenType = pItem->GetInvenType();
            pInvenPtr->SetLock(InvenType, Slot, 0);
        }
    }
    
    // Handle result
    if (psRes.nResult)
    {
        // Success - add money if provided
        if (psRes.nAddCostMoney > 0)
        {
            pInvenPtr->AddMoney(psRes.nAddCostMoney, 0x2C, 0, 0, 0);
        }
    }
    else
    {
        // Failure - add item to my list
        XBaseInventory* pInven = pInvenPtr->GetInvenPtr(psRes.byInvenType);
        if (!pInven)
            return;
        
        ATL::CTime tCurr = ATL::CTime::GetTickCount();
        
        ST_MY_EXCHANGE_ITEM stSell;
        memset(&stSell, 0, sizeof(stSell));
        
        stSell.dwExchangeID = psRes.dwExchangeID;
        stSell.nExpireDate = tCurr.GetTime() + (ATL::CTimeSpan(0, psRes.nAddHour, 0, psRes.nWaitTime)).GetTotalSeconds();
        stSell.nOpenDate = tCurr.GetTime() + (ATL::CTimeSpan(0, 0, 0, psRes.nWaitTime)).GetTotalSeconds();
        stSell.byState = 2;  // Selling state
        stSell.sInitCount = psRes.stCreateItem.stItem.sCount;
        stSell.nPrice_One = psRes.nPrice_One;
        stSell.stItem = psRes.stCreateItem.stItem;
        
        // Get exchange item from inventory
        std::tr1::shared_ptr<CItem> pExchangeItem;
        pInven->GetItem(&pExchangeItem, psRes.stCreateItem.stItem.xSerial);
        
        if (!pExchangeItem)
        {
            // Send statistics for missing item
            ST_STATISTICS_ITEM_EXCHANGE stMissingStats;
            stMissingStats.byFlag = 0x11;
            stMissingStats.dwEXC_ID = psRes.dwExchangeID;
            stMissingStats.dwUCID = GetOwnerMover(this)->GetActorID().GetID();
            stMissingStats.biSerial = psRes.stCreateItem.stItem.xSerial;

            XSendDBPacket xSendMissingStats(GetOwnerMover(this), 0xF0, 0x11);
            xSendMissingStats << stMissingStats;
            XGameServer::Instance()->SendDBStatistics(xSendMissingStats);
        }
        else
        {
            // Copy socket and broach info from item
            pExchangeItem->GetSocketList(&stSell.vecSocketList);
            pExchangeItem->GetBroachInfo(&stSell.stBroachInfo);
            pExchangeItem->GetPackageInfo(&stSell.psPackageList);
        }
        
        // Insert into my list
        m_mapMyList[stSell.stItem.xSerial] = stSell;
        
        // Handle commission/count/expire items (reduce count or delete)
        __int64 xSerialExchangeCash[3] = {psRes.xSerial_Commission, psRes.xSerial_Count, psRes.xSerial_Expire};
        
        for (int j = 0; j < 3; ++j)
        {
            if (xSerialExchangeCash[j])
            {
                std::tr1::shared_ptr<CItem> pItem;
                pInvenPtr->GetItemPtr(&pItem, xSerialExchangeCash[j]);
                if (pItem)
                {
                    PS_STORAGE_INFO psInfo;
                    memset(&psInfo, 0, sizeof(psInfo));
                    psInfo.byInvenType = pItem->GetInvenType();
                    psInfo.shSlotPos = pItem->GetSlot();
                    STItem stItem;
                    pItem->GetItem(&stItem);
                    psInfo.stItem = stItem;
                    psInfo.stItem.sCount = pItem->GetCount() - 1;
                    
                    psRes.stUpdateItem.push_back(*(PS_PRIVATE_SHOP_INFO*)&psInfo);
                }
            }
        }
        
        // Update inventory items
        for (size_t k = 0; k < psRes.stUpdateItem.size(); ++k)
        {
            XBaseInventory* pUpdateBaseInven = pInvenPtr->GetInvenPtr(psRes.stUpdateItem[k].byType);
            if (pUpdateBaseInven)
            {
                std::tr1::shared_ptr<CItem> pItem;
                pUpdateBaseInven->GetItem(&pItem, psRes.stUpdateItem[k].stPosInfo.uxMapID.nMapID);
                
                if (pItem)
                {
                    int nLogValue = 0;
                    if (psRes.stUpdateItem[k].stPosInfo.vPos.x > 0)
                    {
                        // Reduce count
                        nLogValue = psRes.stUpdateItem[k].stPosInfo.vPos.x - pItem->GetCount();
                        pItem->SetCount(psRes.stUpdateItem[k].stPosInfo.vPos.x);
                    }
                    else
                    {
                        // Remove item
                        nLogValue = -(int)pItem->GetCount();
                        pUpdateBaseInven->RemoveItem(HIWORD(psRes.stUpdateItem[k].uxActorID.dwActorID));
                        
                        // Send statistics for removed item
                        ST_STATISTICS_ITEM_EXCHANGE stRemoveStats;
                        stRemoveStats.byFlag = 0x11;
                        stRemoveStats.dwEXC_ID = psRes.dwExchangeID;
                        stRemoveStats.dwUCID = GetOwnerMover(this)->GetActorID().GetID();
                        stRemoveStats.biSerial = pItem->GetSerial();

                        XSendDBPacket xSendRemoveStats(GetOwnerMover(this), 0xF0, 0x11);
                        xSendRemoveStats << stRemoveStats;
                        XGameServer::Instance()->SendDBStatistics(xSendRemoveStats);
                    }
                    
                    // Log item change
                    ST_LOG_GAME stItemLog;
                    stItemLog._nUAID = 0;
                    stItemLog._nUCID = GetOwnerMover(this)->GetActorID().GetID();
                    stItemLog._sMainType = 4;
                    stItemLog._sSubType = 46;
                    stItemLog.nParam0 = pItem->GetItemID();
                    stItemLog.nParam1 = nLogValue;
                    stItemLog.nParam2 = pItem->GetCount();
                    XGameServer::Instance()->SendDBLog(stItemLog);
                }
            }
        }
        
        // Send user log
        CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover(this));
        if (pUser)
        {
            ST_LOG_GAME stUserLog;
            stUserLog._nUAID = pUser->GetUAID();
            stUserLog._nUCID = GetOwnerMover(this)->GetActorID().GetID();
            stUserLog._sMainType = 18;
            stUserLog._sSubType = 1;
            stUserLog.nParam0 = pUser->GetLevel();
            stUserLog.nParam1 = psRes.stCreateItem.stItem.nItemID;
            stUserLog.nParam2 = psRes.stCreateItem.stItem.sCount;
            stUserLog.nParam3 = psRes.dwExchangeID;
            stUserLog.nParam4 = 0;
            stUserLog.nParam5 = psRes.stCreateItem.stItem.xSerial;
            stUserLog.nParam6 = psRes.nPrice_One;
            XGameServer::Instance()->SendDBLog(stUserLog);
        }
        
        // Send exchange statistics
        ST_STATISTICS_ITEM_EXCHANGE stExchangeStats;
        stExchangeStats.byFlag = 3;
        stExchangeStats.dwEXC_ID = psRes.dwExchangeID;
        stExchangeStats.dwUCID = GetOwnerMover(this)->GetActorID().GetID();
        stExchangeStats.biSerial = stSell.stItem.xSerial;

        XSendDBPacket xSendExchangeStats(GetOwnerMover(this), 0xF0, 3);
        xSendExchangeStats << stExchangeStats;
        XGameServer::Instance()->SendDBStatistics(xSendExchangeStats);
        
        // Send update item packet
        PS_RES_STORAGE_INFO psResStorage;
        psResStorage.vecItem = *(std::vector<PS_STORAGE_INFO>*)&psRes.stUpdateItem;
        pInvenPtr->SendUpdateItem(psResStorage);
        
        // Send my list update packet (Main=0x2B, Sub=8)
        PS_EXCHANGE_MY_LIST_RES psList;
        psList.vecMyList.push_back(stSell);
        
        XSendPacket packet(0x2B, 8);
        packet << psList;
        CGocNetwork::Send(GetOwnerMover(this), packet);
    }
    
    // Send result packet (Main=0x2B, Sub=5)
    PS_EXCHANGE_SELL_REGISTER_RES psResult;
    psResult.nResult = psRes.nResult;
    
    XSendPacket xSendPacket(0x2B, 5);
    xSendPacket << psResult;
    CGocNetwork::Send(GetOwnerMover(this), xSendPacket);
}

// ============================================================================
// IDA: ?ResExchangeMyList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_MY_LIST_RES@@@Z (0x14007D0D0)
// Handle my list response from DB
// ============================================================================
void CGocExchange::ResExchangeMyList(PS_EXCHANGE_MY_LIST_RES& psMyList)
{
    for (const auto& item : psMyList.vecMyList)
        m_mapMyList[item.stItem.xSerial] = item;
    if (psMyList.bLast)
        m_bLoadMyList = true;
}

// ============================================================================
// IDA: ?SendExchangePriceList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_PRICE_HISTORY_RES@@@Z (0x14007D2B0)
// Send price history list to client (Main=0x2B, Sub=2)
// ============================================================================
void CGocExchange::SendExchangePriceList(PS_EXCHANGE_PRICE_HISTORY_RES& psList)
{
    // IDA: 0x14007D2B0 - Send price history list to client
    // Simple wrapper that sends packet (Main=0x2B, Sub=2)
    
    XSendPacket xSendPacket(0x2B, 2);
    xSendPacket << psList;
    CGocNetwork::Send(GetOwnerMover(this), xSendPacket);
}
