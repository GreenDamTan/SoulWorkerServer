// TradeProcess.h
// Trade system packet handler for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include "../../XCore/XServer/TXProcess.h"

class CUser;
class XPacket;

class CTradeProcess : public TXProcess<CUser>
{
public:
    CTradeProcess();
    virtual ~CTradeProcess();

    // Main packet parser
    virtual bool Parse(XPacket& xPacket) override;

    // Database response parser
    bool DBTradeParse(XPacket& xPacket);

    // Trade request handlers
    bool ReqTrade(XPacket& xPacket);
    bool ReqTradeAccept(XPacket& xPacket);
    bool ReqTradeUpdateItem(XPacket& xPacket);
    bool ReqTradeUpdateMoney(XPacket& xPacket);
    bool ReqTradeCheckBtn(XPacket& xPacket);
    bool ReqTradeConfirm(XPacket& xPacket);
    bool ReqTradeCancel(XPacket& xPacket);

    // Private shop request handlers
    bool ReqPrivateShopStart(XPacket& xPacket);
    bool ReqPrivateShopItem(XPacket& xPacket);
    bool ReqPrivateShopState(XPacket& xPacket);
    bool ReqPrivateShopSelect(XPacket& xPacket);
    bool ReqPrivateShopName(XPacket& xPacket);

    // Database response handlers
    bool ResTradeItemConfirm(XPacket& xPacket);
    bool ResPrivateShopStart(XPacket& xPacket);
    bool ResPrivateShopBuy(XPacket& xPacket);

    // Helper functions
    void TradeItemUnLock(CUser* pUser1, CUser* pUser2);
    void SendTradeResult(CUser* pUser1, CUser* pUser2, unsigned char byResult);
    void SendTradeCancel(CUser* pUser1, CUser* pUser2, unsigned int dwResult, short nReason);
    void SendPrivateShopState(CUser* pUser, unsigned char byState);
};
