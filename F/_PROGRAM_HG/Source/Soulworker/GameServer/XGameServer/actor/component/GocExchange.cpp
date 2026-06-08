// GocExchange.cpp - CGocExchange implementation
// IDA decompiled functions from GameServer.exe

#include "GocExchange.h"
#include "GocInventory.h"
#include "Mover.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/XSendPacket.h"
#include "Soulworker/GameServer/XCore/XServer/LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <ATLTime.h>

// Forward declarations for external functions
class CGocNetwork;
class XGameServer;

// External function stubs (to be replaced with actual implementations)
namespace {
    // TODO: 需要实现 - 这些函数需要从其他组件获取
    CMover* GetOwnerMover(CGocExchange* pThis) {
        return reinterpret_cast<CMover*>(pThis->GetOwnerGO());
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
// IDA 反编译 (完整还原):
// void __fastcall CGocExchange::ReqExchangeSearch(CGocExchange *this, PS_EXCHANGE_SEARCH_REQ *psSearch)
// {
//   XGameServer *v2; // rax
//   CMover *v3; // rax
//   ATL::CTimeSpan tSpan; // [rsp+30h] [rbp-10078h] BYREF
//   XSendDBPacket xSendDBPacket; // [rsp+40h] [rbp-10068h] BYREF
//   ATL::CTime tCurr; // [rsp+10060h] [rbp-48h] BYREF
//   TB_ITEM *pTB_ITEM; // [rsp+10068h] [rbp-40h]
//   TB_ITEM_CLASSIFY *TB_CLASSIFY; // [rsp+10070h] [rbp-38h]
//
//   // 1. 检查交易所功能是否开启
//   v2 = TXSingleton<XGameServer>::Instance();
//   if (!XResourceMgr::GetServerContents(&v2->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE)) {
//     v3 = (CMover *)std::list<CBattleZone *>::size((VChunkLocker *)this);
//     CGocNetwork::SendErrorMessage(v3, 0x2Bu, 1u, 0xC3B8u);  // 错误码 0xC3B8 = 交易所功能未开启
//     return;
//   }
//   // 2. 检查搜索冷却时间 (1秒)
//   ATL::CTime::GetTickCount(&tCurr);
//   ATL::CTimeSpan::CTimeSpan(&tSpan, 0, 0, 0, 1);  // 1秒时间间隔
//   if ((__int64)tCurr >= (__int64)tSpan + this->m_n64LastSearchTime) {
//     this->m_n64LastSearchTime = (__int64)tCurr;
//     // 3. 修正等级范围
//     if (psSearch->nItemGradeMin <= -1)
//       psSearch->nItemGradeMax = -1;
//     // 4. 如果指定了物品ID,查找物品分类
//     if (psSearch->dwItemID) {
//       v5 = TXSingleton<XGameServer>::Instance();
//       pTB_ITEM = XResourceMgr::GetTB_ITEM(&v5->m_xResourceMgr, psSearch->dwItemID);
//       if (!pTB_ITEM || (TB_CLASSIFY = XResourceMgr::GetTB_ITEM_CLASSIFY(...)) == nullptr) {
//         v6 = (CMover *)std::list<CBattleZone *>::size((VChunkLocker *)this);
//         CGocNetwork::SendErrorMessage(v6, 0x2Bu, 1u, 0xCB24u);  // 错误码 0xCB24 = 物品不存在
//         return;
//       }
//       psSearch->nCategoryID = TB_CLASSIFY->GroupID;
//       psSearch->nSubCategoryID = TB_CLASSIFY->SubGroupID;
//     }
//     else if (psSearch->nCategoryID == -1 || psSearch->nSubCategoryID == -1) {
//       // 5. 重置所有搜索参数为默认值
//       psSearch->nCategoryID = -1;
//       psSearch->nSubCategoryID = -1;
//       psSearch->nUseClass = -1;
//       psSearch->nLevelMin = -1;
//       psSearch->nLevelMax = 68;
//       psSearch->nItemGradeMin = -1;
//       psSearch->nItemGradeMax = -1;
//       psSearch->nPrice = -1;
//       psSearch->nUpgrade = -1;
//     }
//     // 6. 发送DB请求 (Main=0x27, Sub=1)
//     XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x27u, 1u);
//     operator<<(&xSendDBPacket, psSearch);
//     XGameServer::SendDBGame(v8, &xSendDBPacket);
//   }
// }
// ============================================================================
void CGocExchange::ReqExchangeSearch(PS_EXCHANGE_SEARCH_REQ& psSearch)
{
    // TODO: 汇编还原 - 需要完整实现
    // 1. 检查 E_SERVER_OPTION_ITEM_EXCHANGE 是否启用
    // 2. 检查搜索冷却时间 (1秒)
    // 3. 如果指定了 dwItemID, 查找 TB_ITEM 和 TB_ITEM_CLASSIFY 获取分类
    // 4. 重置无效的搜索参数
    // 5. 发送 DB 请求 (Main=0x27, Sub=1)

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer)
        return;

    // TODO: 需人工审查 - 需要实现完整逻辑
}

// ============================================================================
// IDA: ?ReqExchangeSellRegister@CGocExchange@@QEAAXAEAUPS_EXCHANGE_SELL_REGISTER_REQ@@@Z (0x140076830)
// Registers an item for sale on the exchange
// ============================================================================
void CGocExchange::ReqExchangeSellRegister(PS_EXCHANGE_SELL_REGISTER_REQ& psReq)
{
    // TODO: 汇编还原 - 需要完整实现
    // 这是一个大型函数（0x140076830 - 0x140078D58，约 900 字节）
    // IDA 伪代码摘要:
    // 1. Verify exchange feature is enabled
    // 2. Check item validity and ownership
    // 3. Check cash items (CheckCashItem)
    // 4. Validate price and count
    // 5. Create DB registration packet
    // 6. Send to DB server

    // Placeholder implementation
}

// ============================================================================
// IDA: ?ReqExchangeItemBuy@CGocExchange@@QEAAXAEAUPS_EXCHANGE_ITEM_BUY_REQ@@@Z (0x140078D80)
// Sends item buy request to DB
// IDA 反编译:
// void __fastcall CGocExchange::ReqExchangeItemBuy(CGocExchange *this, PS_EXCHANGE_ITEM_BUY_REQ *psReq)
// {
//   // 1. Check E_SERVER_OPTION_ITEM_EXCHANGE
//   XGameServer *v4 = TXSingleton<XGameServer>::Instance();
//   if (!XResourceMgr::GetServerContents(&v4->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE)) {
//     CMover *v5 = (CMover *)std::list<CBattleZone *>::size((VChunkLocker *)this);
//     CGocNetwork::SendErrorMessage(v5, 0x2Bu, 6u, 0xC3B8u);
//     return;
//   }
//   // 2. Check m_bSendBuyPacket flag
//   if (this->m_bSendBuyPacket) {
//     LogHelper::LogError("game.contents", "ReqExchangeItemBuy() Aready Packet (UCID:%d)", UCID);
//     return;
//   }
//   // 3. Get CUser from owner via RTTI
//   pUser = (CUser *)_RTDynamicCast_0(v7, 0, &CMover RTTI, &CUser RTTI, 0);
//   if (!pUser) { LogHelper::LogError("game.contents", "ReqExchangeItemBuy() User NULL"); return; }
//   // 4. Check GM power (if GM_STATE_GM_GAMEMASTER and not KOR, send error)
//   if (CUser::GetGMPower(pUser) == 1) {
//     Option = XServer::GetOption(v8);
//     if (XOption::GetNationType(Option) != NATION_TYPE_KOR) {
//       CGocNetwork::SendErrorMessage(v11, 0x2Bu, 6u, 0xCCBFu);
//       return;
//     }
//   }
//   // 5. Check block type
//   if (CUser::GetBlockType(pUser)) {
//     // Send error 50111
//   }
//   // 6. Check second password if enabled
//   if (XResourceMgr::GetServerContents(&v13->m_xResourceMgr, E_SERVER_OPTION_SECOND_PW)) {
//     if (!CGocInventory::CheckTradePasswordState(v15, &nTradePWCheck)) {
//       // Send error 52002
//     }
//   }
//   // 7. Send DB packet (main=0x27, sub=9)
//   XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x27u, 9u);
//   operator<<(&xSendDBPacket, psReq);
//   XGameServer::SendDBGame(v17, &xSendDBPacket);
//   this->m_bSendBuyPacket = 1;
// }
// ============================================================================
void CGocExchange::ReqExchangeItemBuy(PS_EXCHANGE_ITEM_BUY_REQ& psReq)
{
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer)
        return;

    // TODO: 检查 E_SERVER_OPTION_ITEM_EXCHANGE
    // 如果交易所功能未开启，发送错误消息 0xC3B8 (50040)

    // Check if already sending buy packet
    if (m_bSendBuyPacket)
    {
        // TODO: 需要获取 UCID for logging
        LogHelper::LogError("game.contents", "ReqExchangeItemBuy() Aready Packet");
        return;
    }

    // Get owner mover
    CMover* pMover = GetOwnerMover(this);
    if (!pMover)
    {
        LogHelper::LogError("game.contents", "ReqExchangeItemBuy() User NULL");
        return;
    }

    // TODO: 需人工审查 - 完整实现需要:
    // - CUser RTTI cast
    // - GM power check (GM_STATE_GM_GAMEMASTER)
    // - Block type check (send error 50111 if blocked)
    // - Second password check (E_SERVER_OPTION_SECOND_PW)

    // Send DB request (Main=0x27, Sub=9)
    // TODO: 需要实现 XSendDBPacket 发送
    // XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x27u, 9u);
    // operator<<(&xSendDBPacket, psReq);
    // XGameServer::SendDBGame(pServer, &xSendDBPacket);

    // Set flag indicating buy packet is being processed
    m_bSendBuyPacket = true;

    (void)psReq;
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
        // Send update packet to client
        // TODO: 需要实现 XSendPacket 发送

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
    // IDA 反编译结果摘要:
    // Checks three cash item types:
    // - byCash_Commission: Item ID 0x31B5D488 (834000008) - Commission discount
    // - byCash_Count: Item ID 0x31B5D489 (834000009) - Count boost
    // - byCash_Expire: Item ID 0x31B5D48A (834000010) - Expire extension

    CMover* pMover = GetOwnerMover(this);
    if (!pMover)
        return false;

    // TODO: 需要实现完整逻辑
    // 对于每个 byCash_X 标志:
    // 1. 获取 CGocInventory 组件
    // 2. 查找 TB_ITEM 和 TB_ITEM_CLASSIFY
    // 3. 获取对应背包
    // 4. 查找匹配的物品 (GetSameItems_2)
    // 5. 检查物品数量和锁定状态
    // 6. 获取物品序列号

    // Placeholder - 简化实现
    if (psReq.byCash_Commission)
    {
        // TODO: 检查 commission 物品
    }

    if (psReq.byCash_Count)
    {
        // TODO: 检查 count 物品
    }

    if (psReq.byCash_Expire)
    {
        // TODO: 检查 expire 物品
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
    // TODO: 需要完整实现 - 需要 CGocNetwork::Send 和 XSendPacket
    (void)psSearch;
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
void CGocExchange::SendExchangeMyList()
{
    // TODO: 需要完整实现
    // 1. 构建 PS_EXCHANGE_MY_LIST_RES
    // 2. 调用 GetExchangeMyList 填充数据
    // 3. 发送包 (Main=0x2B, Sub=8)
}

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
// IDA 反编译:
// void __fastcall CGocExchange::ReqExchangePriceHistory(CGocExchange *this, PS_EXCHANGE_PRICE_HISTORY_REQ *psHistory)
// {
//   XGameServer *v4 = TXSingleton<XGameServer>::Instance();
//   if (XResourceMgr::GetServerContents(&v4->m_xResourceMgr, E_SERVER_OPTION_ITEM_EXCHANGE)) {
//     // Get UCID from owner
//     psHistory->dwUCID = CQuestCondition::GetQuestID(v6);
//     XSendPacket::XSendPacket(&xSendPacket, 0xF3u, 0x28u);
//     operator<<(&xSendPacket, psHistory);
//     // Get CUser via RTTI
//     CCommunitySocket::SendCmd(&v8->m_communitySocket, &xSendPacket, pUser, 0x2Bu, 2u);
//   } else {
//     CGocNetwork::SendErrorMessage(v5, 0x2Bu, 2u, 0xC3B8u);
//   }
// }
// ============================================================================
void CGocExchange::ReqExchangePriceHistory(PS_EXCHANGE_PRICE_HISTORY_REQ& psHistory)
{
    // TODO: 需要完整实现
    // 1. 检查交易所功能是否开启
    // 2. 获取 owner 的 UCID
    // 3. 发送包到社区服务器 (Main=0xF3, Sub=0x28)
    // 4. 通过 CCommunitySocket::SendCmd 发送
    (void)psHistory;
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
    if (!pServer)
        return;

    // TODO: 检查 E_SERVER_OPTION_ITEM_EXCHANGE
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
// IDA 反编译摘要:
// 1. Check E_SERVER_OPTION_ITEM_EXCHANGE
// 2. If byReason == 1 (add), check max 10 items
// 3. Check if item already exists
// 4. Send DB request (Main=0x27, Sub=4)
// ============================================================================
void CGocExchange::ReqExchangeInterestItem(PS_EXCHANGE_INTEREST_ITEM_REQ& psReq)
{
    // TODO: 需要完整实现
    // 1. 检查交易所功能是否开启
    // 2. 如果 byReason == 1 (添加), 检查是否已满10个
    // 3. 检查物品是否已存在
    // 4. 发送 DB 请求 (Main=0x27, Sub=4)
    (void)psReq;
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
        // TODO: Set UserDB flag via CUser
        // *v3 |= 4u;
    }

    // Send to client if exchange is enabled
    // TODO: Check E_SERVER_OPTION_ITEM_EXCHANGE
    SendExchangeMyInterestList();
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

    // TODO: Send response to client (Main=0x2B, Sub=4)
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
    // TODO: 需要完整实现
    // 1. 构建 PS_EXCHANGE_INTEREST_LIST_RES
    // 2. 获取 owner 的 UCID
    // 3. 调用 GetExchangeMyInterestList 填充数据
    // 4. 发送包 (Main=0x2B, Sub=3)
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
    // TODO: 需要完整实现
    // 1. 构建 PS_EXCHANGE_MY_LIST_REQ
    // 2. 获取 owner 的 UCID
    // 3. 发送 DB 包 (Main=0x27, Sub=8)
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
    // TODO: 需要完整实现
    // 1. 构建 PS_EXCHANGE_INTEREST_LIST_REQ
    // 2. 获取 owner 的 UCID
    // 3. 发送 DB 包 (Main=0x27, Sub=3)
}

// ============================================================================
// IDA: ?ResExchangeItemBuyCheck@CGocExchange@@QEAAXAEAUPS_EXCHANGE_ITEM_BUY_REQ@@_JH@Z (0x14007ADE0)
// Check exchange item buy before processing
// IDA 反编译 (关键逻辑):
// void __fastcall CGocExchange::ResExchangeItemBuyCheck(
//     CGocExchange *this, PS_EXCHANGE_ITEM_BUY_REQ *psReq, __int64 biPrice, int nPackageCount)
// {
//   this->m_bSendBuyPacket = 0;
//   PS_DB_EXCHANGE_ITEM_BUY::PS_DB_EXCHANGE_ITEM_BUY(&psDBReq);
//   psDBReq.dwExchangeID = psReq->dwExchangeID;
//   psDBReq.shCount = psReq->shCount;
//   psDBReq.dwUCID = CQuestCondition::GetQuestID(v6);
//
//   // Error check: biPrice < 0
//   if (biPrice < 0) {
//     // Send error 58310
//     psResult.nResult = 58310;
//     XSendPacket::XSendPacket(&xSendPacket, 0x2Bu, 6u);
//     CGocNetwork::Send(pActor, &xSendPacket);
//     return;
//   }
//   // Get TB_ITEM
//   pTB_ITEM = XResourceMgr::GetTB_ITEM(&v8->m_xResourceMgr, psReq->dwItemID);
//   if (!pTB_ITEM || !biPrice) {
//     // Send error 52002
//   }
//   // Check count validity
//   if (psDBReq.shCount <= 0 || pTB_ITEM->Item_Stack_Max < psReq->shCount) {
//     // Send error 52014
//   }
//   // Check money
//   if (CGocInventory::GetMoney(v13) < biPrice) {
//     // Send error 52251 (not enough money)
//   }
//   // Check package count
//   if (CGocInventory::IsResealPackage(v15, pTB_ITEM->Item_ID)) {
//     if (!CGocInventory::IsResealPackageCount(v16, pTB_ITEM->Item_ID, nPackageCount)) {
//       // Send error 52253
//     }
//   }
//   // Deduct money
//   nMoney = CGocInventory::GetMoney(v18) - biPrice;
//   CGocInventory::SetInvenMoney(v19, nMoney, 0);
//   // Generate serials for posts
//   XItemFactory::GeneratSerial(&v20->m_xItemFactory, &result);
//   // Send DB request (Main=0x27, Sub=6)
//   XSendDBPacket::XSendDBPacket(&xSendDBPacket, pObject, 0x27u, 6u);
//   operator<<(&xSendDBPacket, &psDBReq);
//   XGameServer::SendDBGame(v27, &xSendDBPacket);
//   this->m_bSendBuyPacket = 1;
// }
// ============================================================================
void CGocExchange::ResExchangeItemBuyCheck(PS_EXCHANGE_ITEM_BUY_REQ& psReq, std::int64_t biPrice, int nPackageCount)
{
    m_bSendBuyPacket = false;

    // Validate price
    if (biPrice < 0)
    {
        // TODO: Send error packet 58310
        // PS_EXCHANGE_ITEM_BUY_RES psResult;
        // psResult.nResult = 58310;
        // psResult.dwExchangeID = psReq.dwExchangeID;
        // psResult.shBuyCount = psReq.shCount;
        // Send packet (Main=0x2B, Sub=6)
        return;
    }

    // Get item table data
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer)
        return;

    // TODO: 需要实现 XResourceMgr::GetTB_ITEM
    // TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, psReq.dwItemID);
    // if (!pTB_ITEM || !biPrice) {
    //     // Send error 52002
    //     return;
    // }

    // TODO: 需要实现完整逻辑:
    // - Check shCount validity (<= 0 or > Item_Stack_Max)
    // - Check player money via CGocInventory::GetMoney
    // - Check package count if IsResealPackage
    // - Deduct money via CGocInventory::SetInvenMoney
    // - Generate post serials via XItemFactory::GeneratSerial
    // - Send DB packet (Main=0x27, Sub=6)

    m_bSendBuyPacket = true;

    (void)psReq;
    (void)biPrice;
    (void)nPackageCount;
}

// ============================================================================
// IDA: ?ResExchangeItemBuy@CGocExchange@@QEAAXAEAUPS_DB_EXCHANGE_ITEM_BUY@@@Z (0x14007B710)
// Handle exchange item buy response from DB
// IDA 反编译 (关键逻辑):
// void __fastcall CGocExchange::ResExchangeItemBuy(CGocExchange *this, PS_DB_EXCHANGE_ITEM_BUY *psRes)
// {
//   this->m_bSendBuyPacket = 0;
//   psResult.nResult = psRes->nResult;
//   psResult.dwExchangeID = psRes->dwExchangeID;
//   psResult.shBuyCount = psRes->shCount;
//
//   if (psRes->nResult) {
//     // Error - refund money
//     nMoney = psRes->nSellPrice + CGocInventory::GetMoney(v30);
//     CGocInventory::SetInvenMoney(v31, nMoney, 0);
//     // Map error codes: 1->58305, 2->52251, 3->52002, 4->52014, 5->58304
//   } else {
//     // Success - add post items
//     CGocPost::AddRecvPost(v9, v66);
//     // Send post packet (Main=0x20, Sub=9)
//     // Update money and send to client
//     // Send price history update (Main=0xF3, Sub=0x29)
//     // Update achieve CGocAchieve::UpdateCollect(v20, 0x37u, 1, 0)
//     // Send logs and statistics
//   }
//   // Send result packet (Main=0x2B, Sub=6)
//   XSendPacket::XSendPacket(&xSendPacket, 0x2Bu, 6u);
//   operator<<(&xSendPacket, &psResult);
//   CGocNetwork::Send(v83, &xSendPacket);
// }
// ============================================================================
void CGocExchange::ResExchangeItemBuy(PS_DB_EXCHANGE_ITEM_BUY& psRes)
{
    m_bSendBuyPacket = false;

    if (psRes.nResult)
    {
        // Error case - refund money
        // TODO: 需要实现 CGocInventory 操作
        // CMover* pMover = GetOwnerMover(this);
        // CMover::GetGOC<CGocInventory>(pMover, &pInvenPtr, 0);
        // nMoney = psRes.nSellPrice + CGocInventory::GetMoney(v30);
        // CGocInventory::SetInvenMoney(v31, nMoney, 0);

        // Map error codes
        int nResult = psRes.nResult;
        if (nResult == 1)
            nResult = 58305;
        else if (nResult == 2)
            nResult = 52251;
        else if (nResult == 3)
            nResult = 52002;
        else if (nResult == 4)
            nResult = 52014;
        else if (nResult == 5)
            nResult = 58304;

        // TODO: Send error result packet
    }
    else
    {
        // Success case
        // TODO: 需要实现完整逻辑:
        // - Add post items via CGocPost::AddRecvPost
        // - Send post packet (Main=0x20, Sub=9)
        // - Update money display via CGocInventory::SendMoney
        // - Send price history update (Main=0xF3, Sub=0x29)
        // - Update achieve CGocAchieve::UpdateCollect(0x37, 1, 0)
        // - Send logs via XGameServer::SendDBLog
        // - Send statistics via XGameServer::SendDBStatistics
    }

    // Send result packet (Main=0x2B, Sub=6)
    // TODO: 需要实现 XSendPacket 发送

    (void)psRes;
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
    // psResult.nResult = psRes.nResult;
    // psResult.xSerial = psRes.stPost.stItemList[0].xSerial;

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

    // Add post item to player
    // TODO: 需要实现 CGocPost::AddRecvPost
    // CMover* pMover = GetOwnerMover(this);
    // CMover::GetGOC<CGocPost>(pMover, &pRecvPost, 0);
    // CGocPost::AddRecvPost(v10, &psRes.stPost);

    // Send post packet (Main=0x20, Sub=9)
    // TODO: 需要实现 XSendPacket 发送

    // Send recall result packet (Main=0x2B, Sub=7)
    // TODO: 需要实现 XSendPacket 发送

    // Log and statistics
    if (stExchangeInfo.byState == 2)
    {
        // Send statistics (Main=0xF0, Sub=3)
        // TODO: 需要实现 XSendDBPacket 发送
    }

    (void)psRes;
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
            // TODO: Send statistics packet (Main=0xF0, Sub=0x11)
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
                        // TODO: Send statistics packet (Main=0xF0, Sub=0x11)
                    }
                    
                    // Log item change
                    // TODO: Send log packet (Main=4, Sub=46)
                }
            }
        }
        
        // Send user log
        // TODO: Implement user-specific logging
        
        // Send statistics
        // TODO: Send exchange statistics (Main=0xF0, Sub=3)
        
        // Send update item packet
        PS_RES_STORAGE_INFO psResStorage;
        psResStorage.stUpdateItem = *(std::vector<PS_STORAGE_INFO>*)&psRes.stUpdateItem;
        pInvenPtr->SendUpdateItem(&psResStorage);
        
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
