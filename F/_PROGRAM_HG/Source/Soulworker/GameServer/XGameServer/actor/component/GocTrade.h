#pragma once

// Trade system component for GameServer
// Handles player-to-player trading functionality
// 
// This component manages:
// - Trade requests and acceptance
// - Trade item management
// - Trade confirmation
// - Trade cancellation
// - Trade password verification
//
// Based on IDA analysis from GameServer.exe

#include <string>
#include <vector>
#include <list>
#include <map>

// Forward declarations
class CUser;
class CItem;
class XPacket;
class XSendPacket;

// Trade states enumeration
enum eTRADE_STATE
{
    TRADE_STATE_NONE = 0,           // No active trade
    TRADE_STATE_REQUESTING = 1,     // Trade request sent
    TRADE_STATE_TRADING = 2,        // In trade session
    TRADE_STATE_CONFIRMING = 3,     // Confirming trade
    TRADE_STATE_LOCKED = 4          // Trade locked/complete
};

// Trade item structure
struct ST_TRADE_ITEM
{
    unsigned long dwItemSerial;     // Item serial ID
    unsigned long dwItemID;         // Item template ID
    long long nMoney;               // Money amount
    char szItemName[64];            // Item name
    
    ST_TRADE_ITEM();
    ~ST_TRADE_ITEM();
};

// Trade item list structure
struct ST_TRADE_ITEM_LIST
{
    std::vector<ST_TRADE_ITEM> vecTradeItems;
    
    ST_TRADE_ITEM_LIST();
    ST_TRADE_ITEM_LIST(const ST_TRADE_ITEM_LIST& other);
    ~ST_TRADE_ITEM_LIST();
};

// Trade request packet structure
struct PS_REQ_ITEM_TRADE
{
    unsigned long dwItemSerial;
    unsigned char bySlotType;
    unsigned long dwItemCount;
    
    PS_REQ_ITEM_TRADE();
    bool operator==(const PS_REQ_ITEM_TRADE& other) const;
};

// Trade password request structure
struct PS_TRADE_PW_REQ
{
    char szPassword[32];
    char szNewPassword[32];
    unsigned char byType;  // 0: check, 1: create, 2: change
    
    PS_TRADE_PW_REQ();
};

// Trade result structure
struct PS_TRADE_RESULT
{
    unsigned char byResult;
    unsigned long dwError;
    
    PS_TRADE_RESULT();
};

// Trade DB confirmation structure
struct PS_TRADE_DB_CONFIRM
{
    unsigned long dwActorID1;
    unsigned long dwActorID2;
    ST_TRADE_ITEM_LIST stItemList1;
    ST_TRADE_ITEM_LIST stItemList2;
    
    PS_TRADE_DB_CONFIRM();
    PS_TRADE_DB_CONFIRM(const PS_TRADE_DB_CONFIRM& other);
    ~PS_TRADE_DB_CONFIRM();
};

// My trade info structure
struct ST_MY_TRADE_INFO
{
    unsigned long dwTradeID;
    unsigned long dwTargetID;
    eTRADE_STATE eState;
    ST_TRADE_ITEM_LIST stItemList;
    
    ST_MY_TRADE_INFO();
    ~ST_MY_TRADE_INFO();
};

// Trade component class
class CGocTrade
{
public:
    CGocTrade();
    virtual ~CGocTrade();
    
    // Initialization
    bool Init();
    void Clear();
    
    // Trade state management
    eTRADE_STATE GetTradeState() const { return m_eTradeState; }
    void SetTradeState(eTRADE_STATE eState);
    
    // Trade partner management
    unsigned long GetTradeActorID() const { return m_dwTradeActorID; }
    void SetTradeActorID(unsigned long dwActorID);
    
    // Trade money
    long long GetTradeMoney() const { return m_nTradeMoney; }
    void SetTradeMoney(long long nMoney) { m_nTradeMoney = nMoney; }
    
    // Trade password management
    unsigned char GetTradePasswordState() const { return m_byTradePWState; }
    void SetTradePasswordState(unsigned char byState);
    
    // Trade password operations
    bool CheckTradePasswordState(int& nResult);
    bool CheckTradePassword(PS_TRADE_PW_REQ& psReq);
    bool IsValidTradePassword(char* szPassword, int& nResult);
    
    // Trade item operations
    bool AddTradeItem(std::tr1::shared_ptr<CItem> pItem, unsigned long dwMoney, bool& bResult);
    bool RemoveTradeItem(std::tr1::shared_ptr<CItem> pItem);
    
    // Trade item list management
    void GetTradeItemList(ST_TRADE_ITEM_LIST& stList);
    void SetTradeItemList(const ST_TRADE_ITEM_LIST& stList);
    
    // Trade confirmation
    bool SetTradeConfirm(ST_TRADE_ITEM_LIST& stList, void* pSocketList, void* pBroachList, void* pPackageList);
    bool CanTradeConfirm(ST_TRADE_ITEM_LIST stList, ST_TRADE_ITEM_LIST& stResult);
    
    // Trade unlock
    void UpdateTradeUnLock();
    void UpdateTradeUnLock(ST_TRADE_ITEM_LIST stList);
    
    // Trade check
    void IsTradeCheck();
    
    // Trade cancel
    void SendTradeCancel(int nCause);
    
    // Trade password sending
    void SendControlServerTradePassword();
    void GM_ChangeTradePW(wchar_t* wszPW);
    
private:
    eTRADE_STATE m_eTradeState;         // Current trade state
    unsigned long m_dwTradeActorID;     // Trade partner actor ID
    long long m_nTradeMoney;            // Trade money amount
    unsigned char m_byTradePWState;     // Trade password state
    unsigned char m_byTradePWCheck;     // Trade password check flag
    
    std::list<PS_REQ_ITEM_TRADE> m_listTradeItems;  // Trade item requests
    
    ST_TRADE_ITEM_LIST m_stMyTradeList;   // My trade items
    ST_TRADE_ITEM_LIST m_stOtherTradeList; // Partner's trade items
};
