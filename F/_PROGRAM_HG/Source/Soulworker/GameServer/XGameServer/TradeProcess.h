#pragma once

// Trade process handler for GameServer
// Handles packet processing for player trading system
// 
// This process handles:
// - Trade requests (RequestTrade)
// - Trade acceptance (AcceptTrade)
// - Trade item updates (AddTradeItem)
// - Trade money updates
// - Trade confirmation (ConfirmTrade)
// - Trade cancellation (CancelTrade)
// - Private shop operations
//
// Based on IDA analysis from GameServer.exe (CTradeProcess class)

#include <string>

// Forward declarations
class CUser;
class XPacket;

// Trade process class - handles packet processing for trade system
class CTradeProcess : public TXProcess<CUser>
{
public:
    CTradeProcess();
    virtual ~CTradeProcess();
    
    // Packet parsing
    bool Parse(XPacket& packet) override;
    
    // Trade request handlers
    bool ReqTrade(XPacket& packet);              // Request trade with player
    bool ReqTradeAccept(XPacket& packet);         // Accept/reject trade request
    bool ReqTradeUpdateItem(XPacket& packet);     // Add/remove item from trade
    bool ReqTradeUpdateMoney(XPacket& packet);    // Update money in trade
    bool ReqTradeCheckBtn(XPacket& packet);       // Check trade button state
    bool ReqTradeConfirm(XPacket& packet);        // Confirm trade
    bool ReqTradeCancel(XPacket& packet);         // Cancel trade
    
    // Private shop handlers
    bool ReqPrivateShopStart(XPacket& packet);    // Start private shop
    bool ReqPrivateShopItem(XPacket& packet);     // Update private shop item
    bool ReqPrivateShopState(XPacket& packet);    // Set private shop state
    bool ReqPrivateShopSelect(XPacket& packet);   // Select private shop to view
    bool ReqPrivateShopName(XPacket& packet);     // Set private shop name
    
    // Database response handlers
    bool DBTradeParse(XPacket& packet);
    bool ResTradeItemConfirm(XPacket& packet);    // Trade confirm DB response
    bool ResPrivateShopStart(XPacket& packet);    // Private shop start response
    bool ResPrivateShopBuy(XPacket& packet);      // Private shop buy response
    
    // Helper functions
    void TradeItemUnLock(CUser* pUser1, CUser* pUser2);
    void SendTradeResult(CUser* pUser1, CUser* pUser2, unsigned char byResult);
    void SendTradeCancel(CUser* pUser1, CUser* pUser2, long nCause, int nError);
    void SendPrivateShopState(CUser* pUser, unsigned char byState);
    
private:
    // Trade state tracking
    bool m_bTradeConfirm;
    int m_nTradeState;
};
