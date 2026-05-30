#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerExchange.h"
#include <cstdint>
#include <map>
#include <vector>

// Forward declarations
class CMover;
class CUser;
class CGocInventory;
struct TB_ITEM;
struct TB_ITEM_CLASSIFY;
struct PS_EXCHANGE_SELL_REGISTER_REQ;

/**
 * @brief CGocExchange - Game Object Component for item exchange/trading
 *
 * Handles player-to-player trading and item exchange functionality.
 * This component manages the player's exchange items and interest list.
 *
 * IDA struct CGocExchange (104 bytes):
 * - offset 0: GOComponent base (16 bytes)
 * - offset 16: m_bLoadMyList (bool, 1 byte)
 * - offset 17: m_bLoadMyInterestList (bool, 1 byte)
 * - offset 24: m_mapMyList (std::map<__int64, ST_MY_EXCHANGE_ITEM>, 32 bytes)
 * - offset 56: m_vecMyInterestList (std::vector<unsigned long>, 32 bytes)
 * - offset 88: m_n64LastSearchTime (__int64, 8 bytes)
 * - offset 96: m_nWaitTime (int, 4 bytes)
 * - offset 100: m_bSendBuyPacket (bool, 1 byte)
 */
class CGocExchange : public GOComponent {
public:
    // IDA: ??0CGocExchange@@QEAA@XZ (0x1400751B0)
    CGocExchange();

    // IDA: ??1CGocExchange@@UEAA@XZ (0x140075280)
    virtual ~CGocExchange();

    // GOComponent interface
    // IDA: ?Init@CGocExchange@@QEAA_NXZ (0x1400752E0)
    bool Initialize() override;

    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // === Static functions ===

    // IDA: ?GetFamilyID@CGocExchange@@SAHXZ (0x1401F35F0)
    // Returns: 20 (component family ID)
    static int GetFamilyID();

    // === State management ===

    // IDA: ?Clear@CGocExchange@@QEAAXXZ (0x140075300)
    void Clear();

    // === My Exchange List ===

    // IDA: ?GetExchangeMyList@CGocExchange@@QEAAXAEAV?$vector@UST_MY_EXCHANGE_ITEM@@V?$allocator@UST_MY_EXCHANGE_ITEM@@@std@@@std@@@Z (0x140075370)
    // Copies m_mapMyList values to output vector
    void GetExchangeMyList(std::vector<ST_MY_EXCHANGE_ITEM>& vecMyList);

    // === My Interest List ===

    // IDA: ?GetExchangeMyInterestList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_INTEREST_LIST_RES@@@Z (0x1400753F0)
    // Copies m_vecMyInterestList to output response
    void GetExchangeMyInterestList(PS_EXCHANGE_INTEREST_LIST_RES& psRes);

    // === Exchange operations ===

    // IDA: ?ReqExchangeSearch@CGocExchange@@QEAAXAEAUPS_EXCHANGE_SEARCH_REQ@@@Z (0x140075F90)
    // Sends exchange search request to DB
    void ReqExchangeSearch(PS_EXCHANGE_SEARCH_REQ& psSearch);

    // IDA: ?ReqExchangeSellRegister@CGocExchange@@QEAAXAEAUPS_EXCHANGE_SELL_REGISTER_REQ@@@Z (0x140076830)
    // Registers an item for sale on the exchange
    void ReqExchangeSellRegister(PS_EXCHANGE_SELL_REGISTER_REQ& psReq);

    // IDA: ?ReqExchangeItemBuy@CGocExchange@@QEAAXAEAUPS_EXCHANGE_ITEM_BUY_REQ@@@Z (0x140078D80)
    // Sends item buy request to DB
    void ReqExchangeItemBuy(PS_EXCHANGE_ITEM_BUY_REQ& psReq);

    // IDA: ?ResExchangeItemBuyCheck@CGocExchange@@QEAAXAEAUPS_EXCHANGE_ITEM_BUY_REQ@@_JH@Z (0x14007ADE0)
    // Check exchange item buy before processing
    void ResExchangeItemBuyCheck(PS_EXCHANGE_ITEM_BUY_REQ& psReq, std::int64_t biPrice, int nPackageCount);

    // IDA: ?ResExchangeItemBuy@CGocExchange@@QEAAXAEAUPS_DB_EXCHANGE_ITEM_BUY@@@Z (0x14007B710)
    // Handle exchange item buy response from DB
    void ResExchangeItemBuy(PS_DB_EXCHANGE_ITEM_BUY& psRes);

    // IDA: ?ResExchangeItemRecall@CGocExchange@@QEAAXAEAUPS_DB_EXCHANGE_ITEM_RECALL_RES@@@Z (0x14007C9C0)
    // Handle exchange item recall response from DB
    void ResExchangeItemRecall(PS_DB_EXCHANGE_ITEM_RECALL_RES& psRes);

    // IDA: ?SellMyExchangeItem@CGocExchange@@QEAAXAEAUPS_EXCHANGE_PRICE_HISTORY_UPDATE@@@Z (0x140075420)
    // Handles selling of player's exchange item
    void SellMyExchangeItem(PS_EXCHANGE_PRICE_HISTORY_UPDATE& psUpdate);

    // IDA: ?ResExchangeSearch@CGocExchange@@QEAAXAEAUPS_EXCHANGE_SEARCH_RES@@@Z (0x140079590)
    // Send search results to client
    void ResExchangeSearch(PS_EXCHANGE_SEARCH_RES& psSearch);

    // IDA: ?SendExchangeMyList@CGocExchange@@QEAAXXZ (0x14007D380)
    // Send my exchange list to client
    void SendExchangeMyList();

    // IDA: ?ReqExchangePriceHistory@CGocExchange@@QEAAXAEAUPS_EXCHANGE_PRICE_HISTORY_REQ@@@Z (0x1400762E0)
    // Request price history for an item
    void ReqExchangePriceHistory(PS_EXCHANGE_PRICE_HISTORY_REQ& psHistory);

    // IDA: ?ReqExchangeInterestList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_INTEREST_LIST_REQ@@@Z (0x140076450)
    // Request interest list
    void ReqExchangeInterestList(PS_EXCHANGE_INTEREST_LIST_REQ& psReq);

    // IDA: ?ReqExchangeInterestItem@CGocExchange@@QEAAXAEAUPS_EXCHANGE_INTEREST_ITEM_REQ@@@Z (0x1400764D0)
    // Add/remove interest item
    void ReqExchangeInterestItem(PS_EXCHANGE_INTEREST_ITEM_REQ& psReq);

    // IDA: ?ResExchangeMyInterestList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_INTEREST_LIST_RES@@@Z (0x140079660)
    // Handle interest list response from DB
    void ResExchangeMyInterestList(PS_EXCHANGE_INTEREST_LIST_RES& psMyList);

    // IDA: ?ResExchangeMyInterestItem@CGocExchange@@QEAAXAEAUPS_EXCHANGE_INTEREST_ITEM_RES@@@Z (0x140079750)
    // Handle interest item response from DB
    void ResExchangeMyInterestItem(PS_EXCHANGE_INTEREST_ITEM_RES& psResult);

    // IDA: ?SendExchangeMyInterestList@CGocExchange@@QEAAXXZ (0x14007D470)
    // Send interest list to client
    void SendExchangeMyInterestList();

    // IDA: ?DBReqExchangeMyList@CGocExchange@@QEAAXXZ (0x14007D5B0)
    // Request my exchange list from DB
    void DBReqExchangeMyList();

    // IDA: ?DBReqExchangeInterestList@CGocExchange@@QEAAXXZ (0x14007D6D0)
    // Request interest list from DB
    void DBReqExchangeInterestList();

    // IDA: ?ReqExchangeItemRecall@CGocExchange@@QEAAXAEAUPS_EXCHANGE_ITEM_RECALL_REQ@@@Z (0x140079250)
    // Recall exchange item
    void ReqExchangeItemRecall(PS_EXCHANGE_ITEM_RECALL_REQ& psReq);

    // IDA: ?ReqExchangeMyList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_MY_LIST_REQ@@@Z (0x140079510)
    // Request my exchange list
    void ReqExchangeMyList(PS_EXCHANGE_MY_LIST_REQ& psMyList);

    // IDA: ?ResExchangeSellRegister@CGocExchange@@QEAAXAEAUPS_DB_EXCHANGE_SELL_REGISTER@@@Z (0x140079930)
    // Handle sell register response from DB
    void ResExchangeSellRegister(PS_DB_EXCHANGE_SELL_REGISTER& psRes);

    // IDA: ?ResExchangeMyList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_MY_LIST_RES@@@Z (0x14007D0D0)
    // Handle my list response from DB
    void ResExchangeMyList(PS_EXCHANGE_MY_LIST_RES& psMyList);

    // IDA: ?SendExchangePriceList@CGocExchange@@QEAAXAEAUPS_EXCHANGE_PRICE_HISTORY_RES@@@Z (0x14007D2B0)
    // Send price history list to client
    void SendExchangePriceList(PS_EXCHANGE_PRICE_HISTORY_RES& psList);

    // IDA: ?CheckCashItem@CGocExchange@@QEAA_NAEAUPS_EXCHANGE_SELL_REGISTER_REQ@@AEA_J11@Z (0x1400756B0)
    // Checks if cash items are available for exchange registration
    bool CheckCashItem(PS_EXCHANGE_SELL_REGISTER_REQ& psReq,
                       std::int64_t& xSerial_Commission,
                       std::int64_t& xSerial_Count,
                       std::int64_t& xSerial_Expire);

    // === Load state ===

    bool IsLoadMyList() const { return m_bLoadMyList; }
    void SetLoadMyList(bool bLoad) { m_bLoadMyList = bLoad; }

    bool IsLoadMyInterestList() const { return m_bLoadMyInterestList; }
    void SetLoadMyInterestList(bool bLoad) { m_bLoadMyInterestList = bLoad; }

    // === Send buy packet state ===

    bool IsSendBuyPacket() const { return m_bSendBuyPacket; }
    void SetSendBuyPacket(bool bSend) { m_bSendBuyPacket = bSend; }

    // === Wait time ===

    int GetWaitTime() const { return m_nWaitTime; }
    void SetWaitTime(int nWaitTime) { m_nWaitTime = nWaitTime; }

    // === Last search time ===

    std::int64_t GetLastSearchTime() const { return m_n64LastSearchTime; }
    void SetLastSearchTime(std::int64_t nTime) { m_n64LastSearchTime = nTime; }

protected:
    // IDA: offset 16, size 1
    bool m_bLoadMyList;
    // IDA: offset 17, size 1
    bool m_bLoadMyInterestList;
    // IDA: offset 24, size 32
    std::map<std::int64_t, ST_MY_EXCHANGE_ITEM> m_mapMyList;
    // IDA: offset 56, size 32
    std::vector<std::uint32_t> m_vecMyInterestList;
    // IDA: offset 88, size 8
    std::int64_t m_n64LastSearchTime;
    // IDA: offset 96, size 4
    int m_nWaitTime;
    // IDA: offset 100, size 1
    bool m_bSendBuyPacket;
};

static_assert(sizeof(CGocExchange) >= 104, "CGocExchange size check - at least 104 bytes expected");
