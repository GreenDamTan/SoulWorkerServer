// ShopProcess.h
// Shop system packet handler for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include "../../XCore/XServer/TXProcess.h"

class CUser;
class XPacket;

class CShopProcess : public TXProcess<CUser>
{
public:
    CShopProcess();
    virtual ~CShopProcess();

    // Main packet parser
    virtual bool Parse(XPacket& xPacket) override;

    // Database response parser
    bool DBShopParse(XPacket& xPacket);

    // Shop request handlers
    bool ReqItemCashBuy(XPacket& xPacket);
    bool ReqBuyItem(XPacket& xPacket);
    bool ReqSellItem(XPacket& xPacket);
    bool ReqRepurchaserList(XPacket& xPacket);
    bool ReqRepurchaser(XPacket& xPacket);
    bool ReqItemCashSet(XPacket& xPacket);
    bool ReqItemCashSetDel(XPacket& xPacket);
    bool ReqShopCashGift(XPacket& xPacket);
    bool ReqGacha(XPacket& xPacket);

    // Database response handlers
    bool ResItemBuy(XPacket& xPacket);
    bool ResItemSell(XPacket& xPacket);
    bool ResItemRepurchaser(XPacket& xPacket);
    bool ResItemRepurchaserDelete(XPacket& xPacket);
    bool ResNpcCreditGradeLoad(XPacket& xPacket);
    bool ResNpcCreditGradeUpdate(XPacket& xPacket);
    bool ResShopItemLoad(XPacket& xPacket);
    bool ResShopItemUpdate(XPacket& xPacket);
    bool ResShopCashItemBuy(XPacket& xPacket);
    bool ResShopCashSetLoad(XPacket& xPacket);
    bool ResShopCashSet(XPacket& xPacket);
    bool ResShopCashSetDel(XPacket& xPacket);
    bool ResShopCashGiftCheck(XPacket& xPacket);
    bool ResShopCashGift(XPacket& xPacket);
    bool ResCashShopBuyCountLoad(XPacket& xPacket);
    bool ResShopAccountItemUpdate(XPacket& xPacket);
    bool ResShopAccountMileageUpdateCheck(XPacket& xPacket);

    // Helper functions
    int GetShopID(int nShopID);
    int GetShopID(int nShopID, unsigned char byType);
};
