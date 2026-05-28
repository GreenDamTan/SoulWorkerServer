// ItemProcess.h
// Item system packet handler for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include "../../XCore/XServer/TXProcess.h"

class CUser;
class XPacket;

class CItemProcess : public TXProcess<CUser>
{
public:
    CItemProcess();
    virtual ~CItemProcess();

    // Main packet parser
    virtual bool Parse(XPacket& xPacket) override;

    // Database response parser
    bool DBItemParse(XPacket& xPacket);

    // Item request handlers
    bool ReqInventoryInfo(XPacket& xPacket);
    bool ReqItemMove(XPacket& xPacket);
    bool ReqItemCombine(XPacket& xPacket);
    bool ReqItemDivide(XPacket& xPacket);
    bool ReqItemBreak(XPacket& xPacket);
    bool ReqItemAddSlot(XPacket& xPacket);
    bool ReqBankInfo(XPacket& xPacket);
    bool ReqItemUse(XPacket& xPacket);
    bool ReqItemMoveMoney(XPacket& xPacket);
    bool ReqItemLineUp(XPacket& xPacket);
    bool ReqItemUpdateQuickSlotCard(XPacket& xPacket);
    bool ReqItemUpdateQuickSlotItem(XPacket& xPacket);
    bool ReqReloadUpdateCash(XPacket& xPacket);
    bool ReqMazeRewardItem(XPacket& xPacket);
    bool ReqItemAppearanceEquip(XPacket& xPacket);
    bool ReqItemNameChange(XPacket& xPacket);
    bool ReqItemUseSelect(XPacket& xPacket);
    bool ReqItemResealPackage(XPacket& xPacket);
    bool ReqCardDeckPageOpen(XPacket& xPacket);

    // Database response handlers
    bool ResInventoryInfo(XPacket& xPacket);
    bool ResItemMove(XPacket& xPacket);
    bool ResItemCombine(XPacket& xPacket);
    bool ResItemDivide(XPacket& xPacket);
    bool ResItemBreak(XPacket& xPacket);
    bool ResItemReduce(XPacket& xPacket);
    bool ResItemCreate(XPacket& xPacket);
    bool ResItemAddSlot(XPacket& xPacket);
    bool ResBankInfo(XPacket& xPacket);
    bool ResLoadQuickSlot(XPacket& xPacket);
    bool ResItemUpdate(XPacket& xPacket);
    bool ResItemUse(XPacket& xPacket);
    bool ResMazeRewardItem(XPacket& xPacket);
    bool ResEquipSlotOpen(XPacket& xPacket);
    bool ResItemUseInfoSelect(XPacket& xPacket);
    bool ResItemUseInfoUpdate(XPacket& xPacket);
    bool ResItemAppearacneLoad(XPacket& xPacket);
    bool ResItemNameChange(XPacket& xPacket);
    bool ResItemSocketLoad(XPacket& xPacket);
    bool ResItemBroachLoad(XPacket& xPacket);
    bool ResLoadAkashicLoad(XPacket& xPacket);
    bool ResItemUpdateEx(XPacket& xPacket);
    bool ResCreatePostRestoreItem(XPacket& xPacket);
    bool ResItemUseAppearance(XPacket& xPacket);
    bool ResItemMoveMoney(XPacket& xPacket);
    bool ResItemLimitLoad(XPacket& xPacket);
    bool ResItemCooltimeLoad(XPacket& xPacket);
    bool ResItemUpdatePos(XPacket& xPacket);
    bool ResItemUpdateUseCount(XPacket& xPacket);
    bool ResItemNetCafeCostume(XPacket& xPacket);
    bool ResItemMakeLimitLoad(XPacket& xPacket);
    bool ResItemMakeLimitReset(XPacket& xPacket);
    bool ResCardDeckPageOpen(XPacket& xPacket);
    bool ResItemResealPackageInfo(XPacket& xPacket);
    bool ResItemResealPackage(XPacket& xPacket);
    bool ResItemResealPackageUse(XPacket& xPacket);
};
