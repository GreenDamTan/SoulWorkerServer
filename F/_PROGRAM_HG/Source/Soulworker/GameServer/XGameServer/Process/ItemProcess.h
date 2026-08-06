// ItemProcess.h
// Item system packet handler for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include "../../XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/GameServer/XGameServer/XItemFactory.h"
#include <memory>
#include <vector>

class CItem;
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

    // IDA: ?GetDBSaveID@CItemProcess@@QEAAKE@Z (0x140462F40)
    std::uint32_t GetDBSaveID(std::uint8_t byInvenType);

    // IDA: ?GetConditionSlotExtend@CItemProcess@@QEAA_NEEAEAH00AEAF@Z (0x1404630A0)
    bool GetConditionSlotExtend(std::uint8_t byInvenType,
                                std::uint8_t byExtendStep,
                                int& nReqLevel,
                                int& nNeedType,
                                int& nNeedValue,
                                std::int16_t& shAddSlot);

    // Item move helpers
    bool IsVaildInvenType(std::uint8_t bySrcItemInvenType,
                          std::uint8_t byDestInvenType,
                          bool bWeapon);
    void GetEmptySlot(std::uint8_t byInvenType,
                      int nNeedCount,
                      std::vector<stEMPTYSLOT>& vecEmptySlot);
    bool CheckSrcEquipItem(PS_REQ_ITEM_MOVE& psReqItem,
                           std::shared_ptr<CItem> pSrcSlot,
                           eITEM_INVEN_TYPE eInvenType,
                           eITEM_USE_TYPE eUseType);
    bool CheckDestEquipItem(PS_REQ_ITEM_MOVE& psReqItem,
                            std::shared_ptr<CItem> pDestSlot,
                            eITEM_INVEN_TYPE eType);
    bool CheckEquipSlotCount(PS_REQ_ITEM_MOVE& psReqItem);
    void MakeItemSwapPacket(std::uint8_t bySrcInvenType,
                            std::int16_t shSrcSlotPos,
                            std::shared_ptr<CItem> pSrcItem,
                            std::uint8_t byDestInvenType,
                            std::int16_t shDestSlotPos,
                            std::shared_ptr<CItem> pDestItem,
                            PS_DB_ITEM_MOVE_VEC& psItem,
                            bool bSyncToClient);
    void MakeItemSwapPacket(std::uint8_t bySrcInvenType,
                            std::int16_t shSrcSlotPos,
                            std::shared_ptr<CItem> pSrcItem,
                            bool bSrcIgnore,
                            std::uint8_t byDestInvenType,
                            std::int16_t shDestSlotPos,
                            std::shared_ptr<CItem> pDestItem,
                            bool bDestIgnore,
                            PS_DB_ITEM_MOVE_VEC& psItem,
                            bool bSyncToClient);
    bool WeaponEquip(PS_REQ_ITEM_MOVE& psReqItem,
                     PS_DB_ITEM_MOVE_VEC& psItem);
    bool AbilityEquip(PS_REQ_ITEM_MOVE& psReqItem,
                      PS_DB_ITEM_MOVE_VEC& psItem);
    bool ShapeEquip(PS_REQ_ITEM_MOVE& psReqItem,
                    PS_DB_ITEM_MOVE_VEC& psItem);
    bool ItemMove(PS_REQ_ITEM_MOVE& psReqItem,
                  PS_DB_ITEM_MOVE_VEC& psItem);

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
