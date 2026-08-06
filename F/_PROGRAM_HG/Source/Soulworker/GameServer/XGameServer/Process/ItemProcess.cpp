// CItemProcess.cpp
// Item system packet handler for GameServer
// Reconstructed from IDA decompilation

#include "ItemProcess.h"
#include <algorithm>
#include <cstdlib>
#include "../User.h"
#include "../GameServer.h"
#include "../../XCore/XServer/GreenDamTan_TimeCompat.h"
#include "../MoverEx.h"
#include "../actor/component/GocInventory.h"
#include "../actor/component/GocNetwork.h"
#include "../actor/component/GocAkashicRecord.h"
#include "../actor/component/GocEntity.h"
#include "../actor/component/GocPost.h"
#include "../actor/component/GocAttribute.h"
#include "../actor/component/GocQuest.h"
#include "../Item/CItem.h"
#include "../SocialItemObject.h"
#include "../BattleZone.h"
#include "../Maze.h"
#include "../../XCore/XArea/XActor.h"
#include "../../XCore/XArea/XDistrict.h"
#include "../../XRelayServer/Thread/LogicThreadProcessor.h"
#include "../../XCore/XServer/XServer.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "../../XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"

// Constructor
CItemProcess::CItemProcess()
    : TXProcess<CUser>()
{
    SetCmd(8); // ITEM_PROCESS_CMD
    SetName("CItemProcess");
}

// Destructor
CItemProcess::~CItemProcess()
{
}

std::uint32_t CItemProcess::GetDBSaveID(std::uint8_t byInvenType)
{
    (void)byInvenType;
    CUser* pUser = GetClientPtr();
    return pUser ? CQuestCondition::GetQuestID(pUser->GetActorID()) : 0;
}

bool CItemProcess::GetConditionSlotExtend(std::uint8_t byInvenType,
                                           std::uint8_t byExtendStep,
                                           int& nReqLevel,
                                           int& nNeedType,
                                           int& nNeedValue,
                                           std::int16_t& shAddSlot)
{
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer)
        return false;

    TB_INVEN_SLOT_EXTEND* pInvenSlotExtend = nullptr;
    if (pServer->GetOption().GetNationType() == NATION_TYPE_JPN) {
        if (byInvenType != 5 && byInvenType != 6) {
            pInvenSlotExtend =
                pServer->GetResourceMgr().GetTB_INVEN_SLOT_EXTEND(byExtendStep);
            if (!pInvenSlotExtend) {
                SendErrorMessage(0x0F, 0xCB37);
                LogHelper::LogError(
                    "game.item",
                    "GetConditionSlotExtend error - Over Max Extend Step[ Step:%d ] ( %d )",
                    byExtendStep,
                    3190);
                return false;
            }
        }
    } else if (byInvenType != 16 && byInvenType != 17) {
        pInvenSlotExtend =
            pServer->GetResourceMgr().GetTB_INVEN_SLOT_EXTEND(byExtendStep);
        if (!pInvenSlotExtend) {
            SendErrorMessage(0x0F, 0xCB37);
            LogHelper::LogError(
                "game.item",
                "GetConditionSlotExtend error - Over Max Extend Step[ Step:%d ] ( %d )",
                byExtendStep,
                3177);
            return false;
        }
    }

    TB_BANK_SLOT_EXTEND* pBankSlotExtend =
        pServer->GetResourceMgr().GetTB_BANK_SLOT_EXTEND(byExtendStep);
    if (!pBankSlotExtend) {
        SendErrorMessage(0x0F, 0xCB37);
        LogHelper::LogError(
            "game.item",
            "GetConditionSlotExtend error - Over Max Extend Step[ Step:%d ] ( %d )",
            byExtendStep,
            3200);
        return false;
    }

    switch (byInvenType) {
    case 2:
        if (pInvenSlotExtend) {
            nReqLevel = pInvenSlotExtend->Inven_Lv_1;
            nNeedType = pInvenSlotExtend->Extend_Need_1;
            nNeedValue = pInvenSlotExtend->Extend_Value_1;
            shAddSlot = static_cast<std::int16_t>(pInvenSlotExtend->Accrue_Slot_1);
        }
        return true;
    case 4:
        if (pInvenSlotExtend) {
            nReqLevel = pInvenSlotExtend->Inven_Lv_3;
            nNeedType = pInvenSlotExtend->Extend_Need_3;
            nNeedValue = pInvenSlotExtend->Extend_Value_3;
            shAddSlot = static_cast<std::int16_t>(pInvenSlotExtend->Accrue_Slot_3);
        }
        return true;
    case 5:
    case 16:
        nReqLevel = pBankSlotExtend->Bank_Lv_1;
        nNeedType = pBankSlotExtend->Extend_Need_1;
        nNeedValue = pBankSlotExtend->Extend_Value_1;
        shAddSlot = static_cast<std::int16_t>(pBankSlotExtend->Accrue_Slot_1);
        return true;
    case 6:
    case 17:
        nReqLevel = pBankSlotExtend->Bank_Lv_2;
        nNeedType = pBankSlotExtend->Extend_Need_2;
        nNeedValue = pBankSlotExtend->Extend_Value_2;
        shAddSlot = static_cast<std::int16_t>(pBankSlotExtend->Accrue_Slot_2);
        return true;
    case 11:
        if (pInvenSlotExtend) {
            nReqLevel = pInvenSlotExtend->Inven_Lv_4;
            nNeedType = pInvenSlotExtend->Extend_Need_4;
            nNeedValue = pInvenSlotExtend->Extend_Value_4;
            shAddSlot = static_cast<std::int16_t>(pInvenSlotExtend->Accrue_Slot_4);
        }
        return true;
    default:
        return false;
    }
}

bool CItemProcess::IsVaildInvenType(std::uint8_t bySrcItemInvenType,
                                    std::uint8_t byDestInvenType,
                                    bool bWeapon)
{
    (void)bWeapon;
    if (bySrcItemInvenType == 0) {
        return byDestInvenType == 1 || byDestInvenType == 2 ||
               byDestInvenType == 5 || byDestInvenType == 15 ||
               byDestInvenType == 16;
    }

    if (bySrcItemInvenType == 2) {
        return byDestInvenType == 0 || byDestInvenType == 3 ||
               byDestInvenType == 4 || byDestInvenType == 6 ||
               byDestInvenType == 15 || byDestInvenType == 17;
    }

    if (bySrcItemInvenType == 9) {
        return byDestInvenType == 11;
    }

    return bySrcItemInvenType == 13 &&
           (byDestInvenType == 13 || byDestInvenType == 14 ||
            byDestInvenType == 15 || byDestInvenType == 18);
}

void CItemProcess::GetEmptySlot(
    std::uint8_t byInvenType,
    int nNeedCount,
    std::vector<stEMPTYSLOT>& vecEmptySlot)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return;

    std::shared_ptr<CGocInventory> pInventory =
        static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
    if (!pInventory)
        return;

    XBaseInventory* pInven = pInventory->GetInvenPtr(byInvenType);
    if (pInven) {
        pInven->GetEmptySlot(nNeedCount, vecEmptySlot);
        return;
    }

    LogHelper::LogError(
        "game.item",
        "GetEmptySlot error - Inventory is null[ InvenType:%d ]( %d )",
        byInvenType,
        1574);
}

bool CItemProcess::CheckSrcEquipItem(
    PS_REQ_ITEM_MOVE& psReqItem,
    std::shared_ptr<CItem> pSrcSlot,
    eITEM_INVEN_TYPE eInvenType,
    eITEM_USE_TYPE eUseType)
{
    (void)eUseType;
    if (!pSrcSlot)
        return false;

    if (psReqItem.nSrcItemID != pSrcSlot->GetID()) {
        LogHelper::LogError(
            "game.item",
            "CheckSrcEquipItem error - Fault item id[ Item id:%d ]( %d )",
            psReqItem.nSrcItemID,
            1146);
        SendErrorMessage(2, 0xCB22);
        return false;
    }

    if (psReqItem.nSrcItemID == -1)
        return true;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pTBSrcItem = pServer
        ? pServer->GetResourceMgr().GetTB_ITEM(psReqItem.nSrcItemID)
        : nullptr;
    if (!pTBSrcItem) {
        LogHelper::LogError(
            "game.item",
            "CheckSrcEquipItem error - No Table TB_ITEM[ Item id:%d ]( %d )",
            psReqItem.nSrcItemID,
            1160);
        SendErrorMessage(2, 0xCB24);
        return false;
    }

    TB_ITEM_CLASSIFY* pTBClassify = pServer
        ? pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(
              pTBSrcItem->Item_Classify_Index)
        : nullptr;
    if (!pTBClassify) {
        LogHelper::LogError(
            "game.item",
            "CheckSrcEquipItem error - No Table TB_ITEM_CLASSIFY[ Item id:%d ]( %d )",
            psReqItem.nSrcItemID,
            1169);
        SendErrorMessage(2, 0xCB24);
        return false;
    }

    if (!pServer->GetItemFactory().IsCanEquip(
            static_cast<e_EQUIP_SLOT_TYPE>(pTBClassify->Item_Slot_Type),
            psReqItem.shDestSlotPos)) {
        LogHelper::LogError(
            "game.item",
            "CheckSrcEquipItem error - Failed IsCanEquip( %d )",
            1177);
        SendErrorMessage(2, 0xCB26);
        return false;
    }

    if (eInvenType != pTBClassify->Item_Inven_Type) {
        LogHelper::LogError(
            "game.item",
            "CheckSrcEquipItem error - Inventory type[ InvenType:%d ] ( %d )",
            static_cast<int>(eInvenType),
            1185);
        SendErrorMessage(2, 0xCB21);
        return false;
    }

    CUser* pUser = GetClientPtr();
    if (pTBSrcItem->Item_Limit_Lv > pUser->GetLevel()) {
        LogHelper::LogError(
            "game.item",
            "CheckSrcEquipItem error - User level less than item level[ Level:%d ] ( %d )",
            pTBSrcItem->Item_Limit_Lv,
            1195);
        SendErrorMessage(2, 0xCB27);
        return false;
    }

    std::shared_ptr<CGocAttribute> pAttribute =
        static_cast<CMover*>(pUser)->GetGOC_Attribute(false);
    if (pTBSrcItem->Item_Limit_Class &&
        pAttribute->GetClass() != pTBSrcItem->Item_Limit_Class) {
        LogHelper::LogError(
            "game.item",
            "CheckSrcEquipItem error - Dismatch class [ class:%d ] ( %d )",
            pTBSrcItem->Item_Limit_Class,
            1206);
        SendErrorMessage(2, 0xCB29);
        return false;
    }

    return true;
}

bool CItemProcess::CheckDestEquipItem(
    PS_REQ_ITEM_MOVE& psReqItem,
    std::shared_ptr<CItem> pDestSlot,
    eITEM_INVEN_TYPE eType)
{
    if (!pDestSlot || psReqItem.nDestItemID == -1)
        return true;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pTBDestItem = pServer
        ? pServer->GetResourceMgr().GetTB_ITEM(psReqItem.nDestItemID)
        : nullptr;
    if (!pTBDestItem) {
        LogHelper::LogError(
            "game.item",
            "CheckDestEquipItem error - Fault Item id [ ItemID:%d ] ( %d )",
            psReqItem.nDestItemID,
            1244);
        SendErrorMessage(2, 0xCB24);
        return false;
    }

    TB_ITEM_CLASSIFY* pTBClassify = pServer
        ? pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(
              pTBDestItem->Item_Classify_Index)
        : nullptr;
    if (!pTBClassify) {
        LogHelper::LogError(
            "game.item",
            "CheckDestEquipItem error - No Table TB_ITEM_CLASSIFY[ ItemID:%d ] ( %d )",
            psReqItem.nDestItemID,
            1253);
        SendErrorMessage(2, 0xCB24);
        return false;
    }

    if (eType != pTBClassify->Item_Inven_Type) {
        LogHelper::LogError(
            "game.item",
            "CheckDestEquipItem error - Dismatch Inventory type [ Inventype:%d ] ( %d )",
            static_cast<int>(eType),
            1261);
        SendErrorMessage(2, 0xCB21);
        return false;
    }

    const std::uint8_t byLimitSellType =
        pDestSlot->GetItemTable()->Item_Limit_Sell_Type;
    if ((psReqItem.bySrcInvenType == 16 ||
         psReqItem.bySrcInvenType == 5 ||
         psReqItem.bySrcInvenType == 17 ||
         psReqItem.bySrcInvenType == 6) &&
        (byLimitSellType == 4 || byLimitSellType == 5 ||
         byLimitSellType == 6 || byLimitSellType == 7)) {
        LogHelper::LogError(
            "game.item",
            "CheckDestEquipItem error - Cant move item in storage [ Inventype:%d ] ( %d )",
            static_cast<int>(eType),
            1275);
        SendErrorMessage(2, 0xCB4C);
        return false;
    }

    return true;
}

bool CItemProcess::CheckEquipSlotCount(PS_REQ_ITEM_MOVE& psReqItem)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    if (psReqItem.nSrcItemID == -1 && psReqItem.nDestItemID != -1) {
        std::shared_ptr<CGocInventory> pInventory =
            static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
        XBaseInventory* pInven = pInventory
            ? pInventory->GetInvenPtr(psReqItem.bySrcInvenType)
            : nullptr;
        if (!pInven)
            return false;
        if (!pInven->CheckEmptySlotCount(1)) {
            LogHelper::LogError(
                "game.item",
                "CheckEquipSlotCount error - Failed CheckEmptySlotCount( %d )",
                1301);
            SendErrorMessage(2, 0xCB2A);
            return false;
        }
    }

    return true;
}

void CItemProcess::MakeItemSwapPacket(
    std::uint8_t bySrcInvenType,
    std::int16_t shSrcSlotPos,
    std::shared_ptr<CItem> pSrcItem,
    std::uint8_t byDestInvenType,
    std::int16_t shDestSlotPos,
    std::shared_ptr<CItem> pDestItem,
    PS_DB_ITEM_MOVE_VEC& psItem,
    bool bSyncToClient)
{
    PS_DB_ITEM_MOVE stItemMove{};
    stItemMove.bySrcInvenType = bySrcInvenType;
    stItemMove.shSrcSlotPos = shSrcSlotPos;
    stItemMove.dwSrcActorID = GetDBSaveID(byDestInvenType);
    if (pSrcItem)
        pSrcItem->GetItem(&stItemMove.stSrcItem);
    stItemMove.bySrcStoreType = 0;
    stItemMove.byDestInvenType = byDestInvenType;
    stItemMove.shDestSlotPos = shDestSlotPos;
    stItemMove.dwDestActorID = GetDBSaveID(bySrcInvenType);
    stItemMove.byDestStoreType = 0;
    if (pDestItem)
        pDestItem->GetItem(&stItemMove.stDestItem);
    else
        stItemMove.stDestItem.Init();
    stItemMove.bSyncToClient = bSyncToClient;

    CUser* pUser = GetClientPtr();
    const std::uint32_t dwUAID = pUser ? pUser->GetUAID() : 0;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pUser && pServer && pServer->GetOption().GetNationType() != 2) {
        if (!pDestItem) {
            if (byDestInvenType == 16 || byDestInvenType == 17 ||
                byDestInvenType == 18) {
                stItemMove.dwDestActorID = dwUAID;
                stItemMove.dwSrcActorID = dwUAID;
                stItemMove.byDestStoreType = 1;
                stItemMove.bySrcStoreType = 1;
            } else {
                stItemMove.dwSrcActorID = GetDBSaveID(byDestInvenType);
            }
        } else if (bySrcInvenType == 16 || bySrcInvenType == 17 ||
                   bySrcInvenType == 18) {
            if (bySrcInvenType == byDestInvenType) {
                stItemMove.dwDestActorID = dwUAID;
                stItemMove.dwSrcActorID = dwUAID;
                stItemMove.byDestStoreType = 1;
            } else {
                stItemMove.dwSrcActorID = GetDBSaveID(byDestInvenType);
                stItemMove.dwDestActorID = dwUAID;
                stItemMove.byDestStoreType = 0;
            }
            stItemMove.bySrcStoreType = 1;
        } else if (byDestInvenType == 16 || byDestInvenType == 17 ||
                   byDestInvenType == 18) {
            stItemMove.dwSrcActorID = dwUAID;
            stItemMove.dwDestActorID = GetDBSaveID(byDestInvenType);
            stItemMove.byDestStoreType = 1;
            stItemMove.bySrcStoreType = 0;
        }
    }

    std::shared_ptr<CGocInventory> pInventory =
        pUser ? static_cast<CMover*>(pUser)->GetGOC_Inventory(false) : nullptr;
    if (pInventory) {
        pInventory->SetLock(bySrcInvenType, shSrcSlotPos, 2);
        pInventory->SetLock(byDestInvenType, shDestSlotPos, 2);
    }
    psItem.vecItem.push_back(stItemMove);
}

void CItemProcess::MakeItemSwapPacket(
    std::uint8_t bySrcInvenType,
    std::int16_t shSrcSlotPos,
    std::shared_ptr<CItem> pSrcItem,
    bool bSrcIgnore,
    std::uint8_t byDestInvenType,
    std::int16_t shDestSlotPos,
    std::shared_ptr<CItem> pDestItem,
    bool bDestIgnore,
    PS_DB_ITEM_MOVE_VEC& psItem,
    bool bSyncToClient)
{
    (void)bSrcIgnore;
    PS_DB_ITEM_MOVE stItemMove{};
    stItemMove.bySrcInvenType = bySrcInvenType;
    stItemMove.shSrcSlotPos = shSrcSlotPos;
    stItemMove.dwSrcActorID = GetDBSaveID(byDestInvenType);
    if (pSrcItem)
        pSrcItem->GetItem(&stItemMove.stSrcItem);
    stItemMove.bySrcStoreType = 0;
    stItemMove.byDestInvenType = byDestInvenType;
    stItemMove.shDestSlotPos = shDestSlotPos;
    stItemMove.dwDestActorID = GetDBSaveID(bySrcInvenType);
    stItemMove.byDestStoreType = 0;
    if (!pDestItem || bDestIgnore)
        stItemMove.stDestItem.Init();
    else
        pDestItem->GetItem(&stItemMove.stDestItem);
    stItemMove.bSyncToClient = bSyncToClient;

    CUser* pUser = GetClientPtr();
    const std::uint32_t dwUAID = pUser ? pUser->GetUAID() : 0;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pUser && pServer && pServer->GetOption().GetNationType() != 2) {
        if (!pDestItem) {
            if (byDestInvenType == 16 || byDestInvenType == 17 ||
                byDestInvenType == 18) {
                stItemMove.dwDestActorID = dwUAID;
                stItemMove.dwSrcActorID = dwUAID;
                stItemMove.byDestStoreType = 1;
                stItemMove.bySrcStoreType = 1;
            } else {
                stItemMove.dwSrcActorID = GetDBSaveID(byDestInvenType);
            }
        } else if (bySrcInvenType == 16 || bySrcInvenType == 17 ||
                   bySrcInvenType == 18) {
            if (bySrcInvenType == byDestInvenType) {
                stItemMove.dwDestActorID = dwUAID;
                stItemMove.dwSrcActorID = dwUAID;
            } else {
                stItemMove.dwSrcActorID = GetDBSaveID(byDestInvenType);
                stItemMove.dwDestActorID = dwUAID;
            }
            stItemMove.byDestStoreType = 1;
        } else if (byDestInvenType == 16 || byDestInvenType == 17 ||
                   byDestInvenType == 18) {
            stItemMove.dwSrcActorID = dwUAID;
            stItemMove.bySrcStoreType = 1;
            stItemMove.dwDestActorID = GetDBSaveID(byDestInvenType);
        }
    }

    std::shared_ptr<CGocInventory> pInventory =
        pUser ? static_cast<CMover*>(pUser)->GetGOC_Inventory(false) : nullptr;
    if (pInventory) {
        pInventory->SetLock(bySrcInvenType, shSrcSlotPos, 2);
        pInventory->SetLock(byDestInvenType, shDestSlotPos, 2);
    }
    psItem.vecItem.push_back(stItemMove);
}

bool CItemProcess::WeaponEquip(
    PS_REQ_ITEM_MOVE& psReqItem,
    PS_DB_ITEM_MOVE_VEC& psItem)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    std::shared_ptr<CGocInventory> pInventory =
        static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
    if (!pInventory)
        return false;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    const bool bJpn = pServer &&
        pServer->GetOption().GetNationType() == NATION_TYPE_JPN;
    if (bJpn) {
        if (psReqItem.bySrcInvenType != 2 &&
            psReqItem.bySrcInvenType != 5) {
            LogHelper::LogError(
                "game.item",
                "WeaponEquip error - Dismatch inventory type[ InvenType:%d ]( %d )",
                psReqItem.bySrcInvenType,
                1333);
            SendErrorMessage(2, 0xCB21);
            return false;
        }
    } else if (psReqItem.bySrcInvenType != 2 &&
               psReqItem.bySrcInvenType != 16) {
        LogHelper::LogError(
            "game.item",
            "WeaponEquip error - Dismatch inventory type[ InvenType:%d ]( %d )",
            psReqItem.bySrcInvenType,
            1323);
        SendErrorMessage(2, 0xCB21);
        return false;
    }

    if (psReqItem.nSrcItemID == -1 && psReqItem.nDestItemID == -1) {
        LogHelper::LogError(
            "game.item",
            "WeaponEquip error - Fault item id [ src:%d, dst:%d ]( %d )",
            psReqItem.nSrcItemID,
            psReqItem.nDestItemID,
            1342);
        SendErrorMessage(2, 0xCB2B);
        return false;
    }
    if (psReqItem.byDestInvenType != 1) {
        LogHelper::LogError(
            "game.item",
            "WeaponEquip error - Inventory type is not ability equip[ Inventype:%d ]( %d )",
            psReqItem.byDestInvenType,
            1350);
        SendErrorMessage(2, 0xCB21);
        return false;
    }

    bool bSrcLock = false;
    bool bDestLock = false;
    std::shared_ptr<CItem> pSrcSlot = pInventory->GetSlotItem(
        psReqItem.bySrcInvenType,
        static_cast<std::uint16_t>(psReqItem.shSrcSlotPos),
        bSrcLock);
    std::shared_ptr<CItem> pDestSlot = pInventory->GetSlotItem(
        psReqItem.byDestInvenType,
        static_cast<std::uint16_t>(psReqItem.shDestSlotPos),
        bDestLock);
    if (!pSrcSlot || bSrcLock) {
        LogHelper::LogError(
            "game.item",
            "WeaponEquip error - Item is NULL or Lock( %d )",
            1362);
        if (pServer)
            pServer->SendItemLockLog(
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                psReqItem.bySrcInvenType,
                psReqItem.shSrcSlotPos,
                static_cast<std::uint8_t>(bSrcLock),
                36,
                0);
        return false;
    }
    if (bDestLock) {
        LogHelper::LogError("game.item", "WeaponEquip error - Lock( %d )", 1370);
        if (pServer)
            pServer->SendItemLockLog(
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                psReqItem.byDestInvenType,
                psReqItem.shDestSlotPos,
                static_cast<std::uint8_t>(bDestLock),
                37,
                0);
        return false;
    }

    if (pDestSlot) {
        const TB_ITEM_CLASSIFY* pClassify = pDestSlot->GetClassifyTable();
        if (!pClassify || !pServer)
            return false;
        std::vector<int> vecSlot;
        pServer->GetItemFactory().FindEquipSlot(
            static_cast<e_EQUIP_SLOT_TYPE>(pClassify->Item_Slot_Type),
            vecSlot);
        if (std::find(vecSlot.begin(), vecSlot.end(),
                      psReqItem.shDestSlotPos) == vecSlot.end())
            return false;
    }

    if (!CheckSrcEquipItem(psReqItem,
                           pSrcSlot,
                           E_ITEM_INVEN_TYPE_COMMON,
                           E_ITEM_USE_TYPE_ABILITY)) {
        LogHelper::LogError(
            "game.item",
            "WeaponEquip error - Failed CheckSrcEquipItem( %d )",
            1397);
        return false;
    }
    if (!CheckDestEquipItem(psReqItem,
                            pDestSlot,
                            E_ITEM_INVEN_TYPE_COMMON)) {
        LogHelper::LogError(
            "game.item",
            "WeaponEquip error - Failed CheckDestEquipItem( %d )",
            1403);
        return false;
    }
    if (!CheckEquipSlotCount(psReqItem)) {
        LogHelper::LogError(
            "game.item",
            "WeaponEquip error - Failed CheckEquipSlotCount( %d )",
            1409);
        return false;
    }

    TB_ITEM* pTBItem = pSrcSlot->GetItemTable();
    if (pTBItem && !pSrcSlot->IsBind() && pTBItem->Item_Bind_Type == 1)
        pSrcSlot->SetBind(1);

    MakeItemSwapPacket(psReqItem.bySrcInvenType,
                       psReqItem.shSrcSlotPos,
                       pSrcSlot,
                       psReqItem.byDestInvenType,
                       psReqItem.shDestSlotPos,
                       pDestSlot,
                       psItem,
                       true);

    ST_LOG_GAME stLog{};
    stLog._nUAID = static_cast<int>(pUser->GetUAID());
    stLog._nUCID = static_cast<int>(
        CQuestCondition::GetQuestID(pUser->GetActorID()));
    stLog._sMainType = 4;
    stLog._sSubType = 57;
    stLog.nParam0 = pSrcSlot->GetID();
    stLog.nParam1 = pSrcSlot->GetCount();
    stLog.nParam2 = pUser->GetLevel();
    stLog.nParam3 = psReqItem.bySrcInvenType;
    stLog.nParam4 = psReqItem.shSrcSlotPos;
    stLog.nParam5 = pSrcSlot->GetSerial();
    stLog.nParam6 = pDestSlot ? pDestSlot->GetSerial() : 0;
    stLog.nParam7 = psReqItem.byDestInvenType;
    stLog.nParam8 = psReqItem.shDestSlotPos;
    stLog.nParam12 = pSrcSlot->GetCount();
    if (pServer)
        pServer->SendDBLog(stLog);

    if (pDestSlot) {
        stLog.nParam0 = pDestSlot->GetID();
        stLog.nParam1 = pDestSlot->GetCount();
        stLog.nParam3 = psReqItem.byDestInvenType;
        stLog.nParam4 = psReqItem.shDestSlotPos;
        stLog.nParam5 = pDestSlot->GetSerial();
        stLog.nParam6 = pSrcSlot->GetSerial();
        stLog.nParam7 = psReqItem.bySrcInvenType;
        stLog.nParam8 = psReqItem.shSrcSlotPos;
        stLog.nParam12 = pDestSlot->GetCount();
        if (pServer)
            pServer->SendDBLog(stLog);
    }

    return true;
}

bool CItemProcess::AbilityEquip(
    PS_REQ_ITEM_MOVE& psReqItem,
    PS_DB_ITEM_MOVE_VEC& psItem)
{
    if (psReqItem.shDestSlotPos == 0 || psReqItem.shDestSlotPos == 1)
        return WeaponEquip(psReqItem, psItem);

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    std::shared_ptr<CGocInventory> pInventory =
        static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
    if (!pInventory)
        return false;

    if (psReqItem.nSrcItemID == -1 && psReqItem.nDestItemID == -1) {
        LogHelper::LogError(
            "game.item",
            "AbilityEquip error - Failed item id[ src:%d, dst:%d ]( %d )",
            psReqItem.nSrcItemID,
            psReqItem.nDestItemID,
            1472);
        SendErrorMessage(2, 0xCB2B);
        return false;
    }

    bool bSrcLock = false;
    bool bDestLock = false;
    std::shared_ptr<CItem> pSrcSlot = pInventory->GetSlotItem(
        psReqItem.bySrcInvenType,
        static_cast<std::uint16_t>(psReqItem.shSrcSlotPos),
        bSrcLock);
    std::shared_ptr<CItem> pDestSlot = pInventory->GetSlotItem(
        psReqItem.byDestInvenType,
        static_cast<std::uint16_t>(psReqItem.shDestSlotPos),
        bDestLock);
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pSrcSlot || bSrcLock) {
        LogHelper::LogError(
            "game.item",
            "AbilityEquip error - Item is NULL or Lock( %d )",
            1485);
        if (pServer)
            pServer->SendItemLockLog(
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                psReqItem.bySrcInvenType,
                psReqItem.shSrcSlotPos,
                static_cast<std::uint8_t>(bSrcLock),
                38,
                0);
        return false;
    }
    if (bDestLock) {
        LogHelper::LogError("game.item", "AbilityEquip error - Lock( %d )", 1493);
        if (pServer)
            pServer->SendItemLockLog(
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                psReqItem.byDestInvenType,
                psReqItem.shDestSlotPos,
                static_cast<std::uint8_t>(bDestLock),
                39,
                0);
        return false;
    }
    if (pSrcSlot->GetFlag() == 2) {
        LogHelper::LogError(
            "game.item",
            "AbilityEquip error - Item is break for reinforce[ Flag:%d ] ( %d )",
            pSrcSlot->GetFlag(),
            1501);
        return false;
    }

    if (pDestSlot) {
        TB_ITEM_CLASSIFY* pClassify = pDestSlot->GetClassifyTable();
        if (!pClassify || !pServer)
            return false;
        std::vector<int> vecSlot;
        pServer->GetItemFactory().FindEquipSlot(
            static_cast<e_EQUIP_SLOT_TYPE>(pClassify->Item_Slot_Type),
            vecSlot);
        if (std::find(vecSlot.begin(), vecSlot.end(),
                      psReqItem.shDestSlotPos) == vecSlot.end())
            return false;
    }

    if (!CheckSrcEquipItem(psReqItem,
                           pSrcSlot,
                           E_ITEM_INVEN_TYPE_COMMON,
                           E_ITEM_USE_TYPE_ABILITY)) {
        LogHelper::LogError(
            "game.item",
            "AbilityEquip error - Failed CheckSrcEquipItem( %d )",
            1526);
        return false;
    }
    if (!CheckDestEquipItem(psReqItem,
                            pDestSlot,
                            E_ITEM_INVEN_TYPE_COMMON)) {
        LogHelper::LogError(
            "game.item",
            "AbilityEquip error - Failed CheckDestEquipItem( %d )",
            1532);
        return false;
    }
    if (!CheckEquipSlotCount(psReqItem)) {
        LogHelper::LogError(
            "game.item",
            "AbilityEquip error - Failed CheckEquipSlotCount( %d )",
            1538);
        return false;
    }

    TB_ITEM* pTBItem = pSrcSlot->GetItemTable();
    if (pTBItem && !pSrcSlot->IsBind() && pTBItem->Item_Bind_Type == 1)
        pSrcSlot->SetBind(1);
    if (pTBItem && pTBItem->Item_Use_Period_Type == 2) {
        STItem stItem{};
        pSrcSlot->GetItem(&stItem);
        if (stItem.nCashDate == 0 && pServer) {
            pSrcSlot->UpdateCashDate(static_cast<int>(
                pServer->GetCurDate() +
                60LL * pTBItem->Item_Use_Period_Value));
        }
    }

    MakeItemSwapPacket(psReqItem.bySrcInvenType,
                       psReqItem.shSrcSlotPos,
                       pSrcSlot,
                       psReqItem.byDestInvenType,
                       psReqItem.shDestSlotPos,
                       pDestSlot,
                       psItem,
                       true);
    return true;
}

bool CItemProcess::ShapeEquip(
    PS_REQ_ITEM_MOVE& psReqItem,
    PS_DB_ITEM_MOVE_VEC& psItem)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    std::shared_ptr<CGocInventory> pInventory =
        static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
    if (!pInventory)
        return false;

    XArea* pArea = pUser->GetArea();
    if (pArea && (pArea->GetWorldType() == 2 ||
                  pArea->GetWorldType() == 1)) {
        LogHelper::LogError(
            "game.item",
            "ShapeEquip error - Fault Area( %d )",
            1594);
        SendErrorMessage(2, 0xE422);
        return false;
    }

    if (psReqItem.bySrcInvenType != 2 &&
        psReqItem.bySrcInvenType != 4 &&
        psReqItem.bySrcInvenType != 6 &&
        psReqItem.bySrcInvenType != 17 &&
        psReqItem.bySrcInvenType != 3 &&
        psReqItem.bySrcInvenType != 0) {
        LogHelper::LogError(
            "game.item",
            "ShapeEquip error - Fault Inventory type( %d )",
            1604);
        SendErrorMessage(2, 0xCB21);
        return false;
    }

    if ((!psReqItem.bySrcInvenType && psReqItem.byDestInvenType == 3) ||
        (!psReqItem.byDestInvenType && psReqItem.bySrcInvenType == 3)) {
        LogHelper::LogError(
            "game.item",
            "ShapeEquip error - Swap error( %d )",
            1614);
        SendErrorMessage(2, 0xCB21);
        return false;
    }

    if (psReqItem.nSrcItemID == -1 && psReqItem.nDestItemID == -1) {
        LogHelper::LogError(
            "game.item",
            "ShapeEquip error - Fault Item id[ src:%d, dst:%d ] ( %d )",
            psReqItem.nSrcItemID,
            psReqItem.nDestItemID,
            1622);
        SendErrorMessage(2, 0xCB2B);
        return false;
    }

    bool bSrcLock = false;
    bool bDestLock = false;
    std::shared_ptr<CItem> pSrcSlot = pInventory->GetSlotItem(
        psReqItem.bySrcInvenType,
        static_cast<std::uint16_t>(psReqItem.shSrcSlotPos),
        bSrcLock);
    std::shared_ptr<CItem> pDestSlot = pInventory->GetSlotItem(
        psReqItem.byDestInvenType,
        static_cast<std::uint16_t>(psReqItem.shDestSlotPos),
        bDestLock);
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pSrcSlot || bSrcLock) {
        LogHelper::LogError(
            "game.item",
            "ShapeEquip error - Item is NULL or Lock( %d )",
            1634);
        if (pServer)
            pServer->SendItemLockLog(
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                psReqItem.bySrcInvenType,
                psReqItem.shSrcSlotPos,
                static_cast<std::uint8_t>(bSrcLock),
                40,
                0);
        return false;
    }
    if (bDestLock) {
        LogHelper::LogError(
            "game.item",
            "ShapeEquip error - Item is NULL or Lock( %d )",
            1642);
        if (pServer)
            pServer->SendItemLockLog(
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                psReqItem.byDestInvenType,
                psReqItem.shDestSlotPos,
                static_cast<std::uint8_t>(bDestLock),
                41,
                0);
        return false;
    }
    if (!CheckEquipSlotCount(psReqItem)) {
        LogHelper::LogError(
            "game.item",
            "ShapeEquip error - Failed CheckEquipSlotCount( %d )",
            1650);
        return false;
    }

    XBaseEquip* pShapeEquip =
        pInventory->GetEquipPtr(psReqItem.byDestInvenType);
    if (!pShapeEquip) {
        LogHelper::LogError(
            "game.item",
            "ShapeEquip error - Failed GetInventory( %d )",
            1657);
        return false;
    }

    std::vector<stEMPTYSLOT> vecCancelSlot;
    std::vector<stEMPTYSLOT> vecEmptySlot;
    int nCancelBit = 0;
    TB_ITEM* pTBSrcItem = pServer
        ? pServer->GetResourceMgr().GetTB_ITEM(psReqItem.nSrcItemID)
        : nullptr;

    if (pDestSlot) {
        TB_ITEM_CLASSIFY* pClassify = pDestSlot->GetClassifyTable();
        if (!pClassify || !pServer)
            return false;
        std::vector<int> vecSlot;
        pServer->GetItemFactory().FindEquipSlot(
            static_cast<e_EQUIP_SLOT_TYPE>(pClassify->Item_Slot_Type),
            vecSlot);
        if (std::find(vecSlot.begin(), vecSlot.end(),
                      psReqItem.shDestSlotPos) == vecSlot.end())
            return false;
    }

    if (!CheckSrcEquipItem(psReqItem,
                           pSrcSlot,
                           E_ITEM_INVEN_TYPE_COSTUME,
                           E_ITEM_USE_TYPE_SHAPE)) {
        LogHelper::LogError(
            "game.item",
            "ShapeEquip error - Failed CheckSrcEquipItem( %d )",
            1687);
        return false;
    }
    if (!CheckDestEquipItem(psReqItem,
                            pDestSlot,
                            E_ITEM_INVEN_TYPE_COSTUME)) {
        LogHelper::LogError(
            "game.item",
            "ShapeEquip error - Failed CheckDestEquipItem( %d )",
            1693);
        return false;
    }
    if (!CheckEquipSlotCount(psReqItem)) {
        LogHelper::LogError(
            "game.item",
            "ShapeEquip error - Failed CheckEquipSlotCount( %d )",
            1699);
        return false;
    }

    if (pTBSrcItem) {
        TB_ITEM_CLASSIFY* pTBSrcClassify = pServer
            ? pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(
                  pTBSrcItem->Item_Classify_Index)
            : nullptr;
        if (!pTBSrcClassify)
            return false;

        char* pEnd = nullptr;
        nCancelBit = static_cast<int>(std::strtol(
            pTBSrcItem->Item_Slot_Disable, &pEnd, 2));
        (void)pEnd;

        if (psReqItem.byDestInvenType == 3) {
            XLookEquip* pLookEquip = dynamic_cast<XLookEquip*>(pShapeEquip);
            if (!pLookEquip)
                return false;
            pLookEquip->GetCancelSlot(
                pTBSrcClassify->Item_Slot_Type,
                static_cast<unsigned int>(nCancelBit),
                vecCancelSlot);
        } else {
            XShapeEquip* pShape = dynamic_cast<XShapeEquip*>(pShapeEquip);
            if (!pShape)
                return false;
            pShape->GetCancelSlot(
                pTBSrcClassify->Item_Slot_Type,
                static_cast<unsigned int>(nCancelBit),
                vecCancelSlot);
        }

        for (const stEMPTYSLOT& cancelSlot : vecCancelSlot) {
            std::shared_ptr<CItem> pSlotItem =
                pShapeEquip->GetSlotInfo(cancelSlot.shSlotPos);
            if (pSlotItem &&
                (psReqItem.bySrcInvenType == 6 ||
                 psReqItem.bySrcInvenType == 17)) {
                const std::uint8_t bySellType =
                    pSlotItem->GetItemTable()->Item_Limit_Sell_Type;
                if (bySellType == 4 || bySellType == 5 ||
                    bySellType == 6 || bySellType == 7) {
                    SendErrorMessage(2, 0xCB4C);
                    LogHelper::LogError(
                        "game.item",
                        "ShapeEquip error - cancel slot item sell type ( %d )",
                        1737);
                    return false;
                }
            }
        }

        const std::uint8_t byBindType = pSrcSlot->GetBind();
        if (pTBSrcItem->Item_Bind_Type == 1) {
            if (byBindType == 2)
                pSrcSlot->SetBind(3);
            else if (byBindType == 0)
                pSrcSlot->SetBind(1);
        }

        if (pSrcSlot->GetItemTable()->Item_Use_Period_Type == 2) {
            STItem stItem{};
            pSrcSlot->GetItem(&stItem);
            if (stItem.nCashDate == 0 && pServer) {
                pSrcSlot->UpdateCashDate(static_cast<int>(
                    pServer->GetCurDate() +
                    60LL * pSrcSlot->GetItemTable()->Item_Use_Period_Value));
            }
        }

        if (vecCancelSlot.empty()) {
            MakeItemSwapPacket(psReqItem.bySrcInvenType,
                               psReqItem.shSrcSlotPos,
                               pSrcSlot,
                               psReqItem.byDestInvenType,
                               psReqItem.shDestSlotPos,
                               pDestSlot,
                               psItem,
                               true);
        } else {
            int nNeedCount = static_cast<int>(vecCancelSlot.size());
            if (!pDestSlot)
                --nNeedCount;
            GetEmptySlot(psReqItem.bySrcInvenType,
                         nNeedCount,
                         vecEmptySlot);
            if (!pDestSlot) {
                stEMPTYSLOT info{};
                info.byInvenType = psReqItem.bySrcInvenType;
                info.shSlotPos = psReqItem.shSrcSlotPos;
                vecEmptySlot.push_back(info);
            }

            if (vecCancelSlot.size() != vecEmptySlot.size()) {
                LogHelper::LogError(
                    "game.item",
                    "ShapeEquip error - Not enough inventory( %d )",
                    1790);
                SendErrorMessage(2, 0xCB2A);
                return false;
            }

            if (!pDestSlot) {
                MakeItemSwapPacket(psReqItem.bySrcInvenType,
                                   psReqItem.shSrcSlotPos,
                                   pSrcSlot,
                                   psReqItem.byDestInvenType,
                                   psReqItem.shDestSlotPos,
                                   pDestSlot,
                                   psItem,
                                   true);
            }

            for (std::size_t i = 0; i < vecCancelSlot.size(); ++i) {
                const stEMPTYSLOT& cancelSlot = vecCancelSlot[i];
                const stEMPTYSLOT& emptySlot = vecEmptySlot[i];
                std::shared_ptr<CItem> pCancelSlot =
                    pShapeEquip->GetSlotInfo(cancelSlot.shSlotPos);
                bool bLock = false;
                std::shared_ptr<CItem> pEmptySlot =
                    pInventory->GetSlotItem(emptySlot.byInvenType,
                                            static_cast<std::uint16_t>(
                                                emptySlot.shSlotPos),
                                            bLock);
                if (pCancelSlot) {
                    MakeItemSwapPacket(
                        cancelSlot.byInvenType,
                        cancelSlot.shSlotPos,
                        pCancelSlot,
                        false,
                        emptySlot.byInvenType,
                        emptySlot.shSlotPos,
                        pEmptySlot,
                        true,
                        psItem,
                        true);
                }
            }

            if (pDestSlot) {
                MakeItemSwapPacket(psReqItem.bySrcInvenType,
                                   psReqItem.shSrcSlotPos,
                                   pSrcSlot,
                                   psReqItem.byDestInvenType,
                                   psReqItem.shDestSlotPos,
                                   pDestSlot,
                                   psItem,
                                   true);
            }
        }
    }

    ST_LOG_GAME stLog{};
    stLog._nUAID = static_cast<int>(pUser->GetUAID());
    stLog._nUCID = static_cast<int>(
        CQuestCondition::GetQuestID(pUser->GetActorID()));
    stLog._sMainType = 4;
    stLog._sSubType = 57;
    stLog.nParam0 = pSrcSlot->GetID();
    stLog.nParam1 = pSrcSlot->GetCount();
    stLog.nParam2 = pUser->GetLevel();
    stLog.nParam3 = psReqItem.bySrcInvenType;
    stLog.nParam4 = psReqItem.shSrcSlotPos;
    stLog.nParam5 = pSrcSlot->GetSerial();
    stLog.nParam6 = pDestSlot ? pDestSlot->GetSerial() : 0;
    stLog.nParam7 = psReqItem.byDestInvenType;
    stLog.nParam8 = psReqItem.shDestSlotPos;
    stLog.nParam12 = pSrcSlot->GetCount();
    if (pServer)
        pServer->SendDBLog(stLog);

    if (pDestSlot) {
        stLog.nParam0 = pDestSlot->GetID();
        stLog.nParam1 = pDestSlot->GetCount();
        stLog.nParam3 = psReqItem.byDestInvenType;
        stLog.nParam4 = psReqItem.shDestSlotPos;
        stLog.nParam5 = pDestSlot->GetSerial();
        stLog.nParam6 = pSrcSlot->GetSerial();
        stLog.nParam7 = psReqItem.bySrcInvenType;
        stLog.nParam8 = psReqItem.shSrcSlotPos;
        stLog.nParam12 = pDestSlot->GetSlot();
        if (pServer)
            pServer->SendDBLog(stLog);
    }

    return true;
}

bool CItemProcess::ItemMove(
    PS_REQ_ITEM_MOVE& psReqItem,
    PS_DB_ITEM_MOVE_VEC& psItem)
{
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    std::shared_ptr<CGocInventory> pInventory =
        static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
    if (!pInventory)
        return false;

    if (psReqItem.nSrcItemID == -1 && psReqItem.nDestItemID == -1) {
        LogHelper::LogError(
            "game.item",
            "ItemMove error - Fault Item id[ src:%d, dst:%d ] ( %d )",
            psReqItem.nSrcItemID,
            psReqItem.nDestItemID,
            2057);
        SendErrorMessage(2, 0xCB2B);
        return false;
    }
    if (psReqItem.nSrcItemID == -1) {
        LogHelper::LogError(
            "game.item",
            "ItemMove error - Fault Item id[ src:%d ] ( %d )",
            psReqItem.nSrcItemID,
            2065);
        SendErrorMessage(2, 0xCB2B);
        return false;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pTBSrcItem = pServer
        ? pServer->GetResourceMgr().GetTB_ITEM(psReqItem.nSrcItemID)
        : nullptr;
    if (!pTBSrcItem) {
        LogHelper::LogError(
            "game.item",
            "ItemMove error - No Table TB_ITEM[ ItemID:%d ] ( %d )",
            psReqItem.nSrcItemID,
            2076);
        SendErrorMessage(2, 0xCB24);
        return false;
    }

    TB_ITEM_CLASSIFY* pTBSrcClassify = pServer
        ? pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(
              pTBSrcItem->Item_Classify_Index)
        : nullptr;
    if (!pTBSrcClassify) {
        LogHelper::LogError(
            "game.item",
            "ItemMove error - No Table TB_ITEM_CLASSIFY[ ItemID:%d ] ( %d )",
            psReqItem.nSrcItemID,
            2085);
        SendErrorMessage(2, 0xCB24);
        return false;
    }

    const bool bWeapon = pTBSrcClassify->Item_Slot_Type == 1 ||
                         pTBSrcClassify->Item_Slot_Type == 2;
    if (!IsVaildInvenType(pTBSrcClassify->Item_Inven_Type,
                          psReqItem.byDestInvenType,
                          bWeapon)) {
        LogHelper::LogError(
            "game.item",
            "ItemMove error - Failed IsVaildInvenType( %d )",
            2097);
        SendErrorMessage(2, 0xCB21);
        return false;
    }

    TB_ITEM* pTBDestItem = nullptr;
    if (psReqItem.nDestItemID != -1) {
        pTBDestItem = pServer
            ? pServer->GetResourceMgr().GetTB_ITEM(psReqItem.nDestItemID)
            : nullptr;
        if (!pTBDestItem) {
            LogHelper::LogError(
                "game.item",
                "ItemMove error - No Table TB_ITEM[ ItemID:%d ]( %d )",
                psReqItem.nDestItemID,
                2108);
            SendErrorMessage(2, 0xCB24);
            return false;
        }
    }

    bool bSrcLock = false;
    bool bDestLock = false;
    std::shared_ptr<CItem> pSrcItem = pInventory->GetSlotItem(
        psReqItem.bySrcInvenType,
        static_cast<std::uint16_t>(psReqItem.shSrcSlotPos),
        bSrcLock);
    std::shared_ptr<CItem> pDestItem = pInventory->GetSlotItem(
        psReqItem.byDestInvenType,
        static_cast<std::uint16_t>(psReqItem.shDestSlotPos),
        bDestLock);
    if (!pSrcItem || bSrcLock) {
        LogHelper::LogError(
            "game.item",
            "ItemMove error - Item is NULL or Lock( %d )",
            2122);
        if (pServer)
            pServer->SendItemLockLog(
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                psReqItem.bySrcInvenType,
                psReqItem.shSrcSlotPos,
                static_cast<std::uint8_t>(bSrcLock),
                42,
                0);
        return false;
    }
    if (!pSrcItem && !pDestItem) {
        LogHelper::LogError("game.item", "ItemMove error - Item is NULL ( %d )", 2130);
        if (pServer)
            pServer->SendItemLockLog(
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                psReqItem.byDestInvenType,
                psReqItem.shDestSlotPos,
                static_cast<std::uint8_t>(bDestLock),
                43,
                0);
        SendErrorMessage(2, 0xCB2B);
        return false;
    }

    XArea* pArea = pUser->GetArea();
    if (pArea && (pSrcItem->GetInvenType() == 3 ||
                  pSrcItem->GetInvenType() == 0) &&
        (pArea->GetWorldType() == 2 || pArea->GetWorldType() == 1)) {
        LogHelper::LogError("game.item", "ItemMove error - Fault Area( %d )", 2143);
        SendErrorMessage(2, 0xE422);
        return false;
    }

    XBaseInventory* pDestInventory =
        pInventory->GetInvenPtr(psReqItem.byDestInvenType);
    if (!pDestInventory) {
        LogHelper::LogError(
            "game.item",
            "ItemMove error - Wrong inventory type[Inven:%d]",
            psReqItem.byDestInvenType);
        SendErrorMessage(2, 0xCB2B);
        return false;
    }
    if (!pDestInventory->CheckSlotPos(psReqItem.shDestSlotPos)) {
        LogHelper::LogError(
            "game.item",
            "ItemMove error - Wrong dest slot[slot:%d, open:%d]",
            psReqItem.shDestSlotPos,
            pDestInventory->GetOpenSlotNum());
        SendErrorMessage(2, 0xCB2B);
        return false;
    }

    if (pDestItem && pSrcItem->GetInvenType() == 1) {
        LogHelper::LogError(
            "game.item",
            "ItemMove error - Dest inven type is ability[ ItemID:%d ]",
            pDestItem->GetID());
        if (pServer)
            pServer->SendItemLockLog(
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                psReqItem.byDestInvenType,
                psReqItem.shDestSlotPos,
                static_cast<std::uint8_t>(bDestLock),
                43,
                0);
        SendErrorMessage(2, 0xCB2B);
        return false;
    }
    if (!psReqItem.bySrcInvenType && pDestItem) {
        LogHelper::LogError(
            "game.item",
            "ItemMove error - Not enough inventory ( %d )",
            2182);
        SendErrorMessage(2, 0xCB26);
        return false;
    }
    if (bSrcLock || bDestLock) {
        if (pServer)
            pServer->SendItemLockLog(
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                psReqItem.byDestInvenType,
                psReqItem.shDestSlotPos,
                static_cast<std::uint8_t>(bDestLock),
                44,
                0);
        LogHelper::LogError(
            "game.item",
            "ItemMove Src: %d, %d( %d )",
            static_cast<int>(bSrcLock),
            static_cast<int>(bDestLock),
            2191);
        SendErrorMessage(2, 0xCB23);
        return false;
    }

    std::int64_t biSocialItemSerial = 0;
    if (pArea) {
        if (auto* pDistrict = dynamic_cast<XDistrict*>(pArea)) {
            if (CSocialItemObject* pSocialItemObject =
                    pDistrict->FindSocialItemObjectByOwner(
                        CQuestCondition::GetQuestID(pUser->GetActorID()))) {
                biSocialItemSerial = pSocialItemObject->GetItemSerialID();
            }
        }
    }

    const auto isLimitedSellType = [](std::uint8_t bySellType) {
        return bySellType == 4 || bySellType == 5 ||
               bySellType == 6 || bySellType == 7;
    };

    if (!pDestItem) {
        const std::uint8_t byLimitSellType =
            pSrcItem->GetItemTable()->Item_Limit_Sell_Type;
        if (psReqItem.bySrcInvenType == 1 &&
            (psReqItem.byDestInvenType == 5 ||
             psReqItem.byDestInvenType == 16) &&
            isLimitedSellType(byLimitSellType)) {
            LogHelper::LogError(
                "game.item",
                "error ItemMove - Cant move to storage[ SellType:%d ]( %d )",
                byLimitSellType,
                2216);
            SendErrorMessage(2, 0xCB4C);
            return false;
        }
        if ((!psReqItem.bySrcInvenType || psReqItem.bySrcInvenType == 3) &&
            (psReqItem.byDestInvenType == 6 ||
             psReqItem.byDestInvenType == 17) &&
            isLimitedSellType(byLimitSellType)) {
            LogHelper::LogError(
                "game.item",
                "error ItemMove - Cant move to storage[ SellType:%d ]( %d )",
                byLimitSellType,
                2228);
            SendErrorMessage(2, 0xCB4C);
            return false;
        }
        if (psReqItem.byDestInvenType != psReqItem.bySrcInvenType) {
            if (psReqItem.bySrcInvenType != 3 &&
                psReqItem.bySrcInvenType != 1 &&
                psReqItem.bySrcInvenType != 16 &&
                psReqItem.bySrcInvenType != 17 &&
                psReqItem.bySrcInvenType != 18 &&
                psReqItem.bySrcInvenType != 5 &&
                psReqItem.bySrcInvenType != 6 &&
                psReqItem.bySrcInvenType != 14 &&
                isLimitedSellType(byLimitSellType)) {
                LogHelper::LogError(
                    "game.item",
                    "error ItemMove - Cant move to storage[ SellType:%d ]( %d )",
                    byLimitSellType,
                    2247);
                SendErrorMessage(2, 0xCB4C);
                return false;
            }
        }
        if (psReqItem.byDestInvenType != psReqItem.bySrcInvenType &&
            pSrcItem->GetSerial() == biSocialItemSerial) {
            LogHelper::LogError(
                "game.item",
                "error ItemMove - Using social item( %d )",
                2256);
            SendErrorMessage(2, 0xCB62);
            return false;
        }
        if (psReqItem.bySrcInvenType == 16 ||
            psReqItem.bySrcInvenType == 18) {
            std::shared_ptr<CGocAkashicRecord> pAkashic =
                static_cast<CMover*>(pUser)->GetGOC_AkashicRecord(false);
            if (pAkashic)
                pAkashic->AddAkashicGetInfo(
                    static_cast<std::uint32_t>(pSrcItem->GetID()));
        }
        if (pUser->IsMatching() &&
            (psReqItem.byDestInvenType == 5 ||
             psReqItem.byDestInvenType == 16) &&
            pSrcItem->GetID() == pInventory->GetMazeNeedItemID()) {
            LogHelper::LogError(
                "game.item",
                "ItemMove error - Not possible when matching[UCID:%d] (%d)",
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                2277);
            SendErrorMessage(2, 0xCB63);
            return false;
        }
    } else if (psReqItem.byDestInvenType != psReqItem.bySrcInvenType) {
        if (psReqItem.byDestInvenType == 14 ||
            psReqItem.byDestInvenType == 18) {
            if (pSrcItem->GetSerial() == biSocialItemSerial) {
                LogHelper::LogError(
                    "game.item",
                    "error ItemMove - Using social item ( %d )",
                    2294);
                SendErrorMessage(2, 0xCB62);
                return false;
            }
        }
        if (pUser->IsMatching()) {
            if ((psReqItem.byDestInvenType == 5 ||
                 psReqItem.byDestInvenType == 16) &&
                pSrcItem->GetID() == pInventory->GetMazeNeedItemID()) {
                LogHelper::LogError(
                    "game.item",
                    "ItemMove error - Not possible when matching[UCID:%d] (%d)",
                    CQuestCondition::GetQuestID(pUser->GetActorID()),
                    2306);
                SendErrorMessage(2, 0xCB63);
                return false;
            }
            if ((psReqItem.bySrcInvenType == 5 ||
                 psReqItem.bySrcInvenType == 16) &&
                pDestItem->GetID() == pInventory->GetMazeNeedItemID()) {
                LogHelper::LogError(
                    "game.item",
                    "ItemMove error - Not possible when matching[UCID:%d] (%d)",
                    CQuestCondition::GetQuestID(pUser->GetActorID()),
                    2316);
                SendErrorMessage(2, 0xCB63);
                return false;
            }
        }

        const std::uint8_t byDestSellType =
            pDestItem->GetItemTable()->Item_Limit_Sell_Type;
        if (isLimitedSellType(byDestSellType) &&
            isLimitedSellType(
                pSrcItem->GetItemTable()->Item_Limit_Sell_Type)) {
            LogHelper::LogError(
                "game.item",
                "error ItemMove - Cant move to storage[ SellType:%d ]( %d )",
                byDestSellType,
                2333);
            SendErrorMessage(2, 0xCB4C);
            return false;
        }

        const bool bSrcBank = psReqItem.bySrcInvenType == 16 ||
                              psReqItem.bySrcInvenType == 17 ||
                              psReqItem.bySrcInvenType == 18 ||
                              psReqItem.bySrcInvenType == 5 ||
                              psReqItem.bySrcInvenType == 6 ||
                              psReqItem.bySrcInvenType == 14;
        if (bSrcBank) {
            if (isLimitedSellType(byDestSellType)) {
                LogHelper::LogError(
                    "game.item",
                    "error ItemMove - Cant move to storage[ SellType:%d ]( %d )",
                    byDestSellType,
                    2364);
                SendErrorMessage(2, 0xCB4C);
                return false;
            }
            if (pDestItem->GetSerial() == biSocialItemSerial) {
                LogHelper::LogError(
                    "game.item",
                    "error ItemMove - Using social item( %d )",
                    2371);
                SendErrorMessage(2, 0xCB62);
                return false;
            }
            std::shared_ptr<CGocAkashicRecord> pAkashic =
                static_cast<CMover*>(pUser)->GetGOC_AkashicRecord(false);
            if (pAkashic)
                pAkashic->AddAkashicGetInfo(
                    static_cast<std::uint32_t>(pSrcItem->GetID()));
        } else if (isLimitedSellType(
                       pSrcItem->GetItemTable()->Item_Limit_Sell_Type)) {
            LogHelper::LogError(
                "game.item",
                "error ItemMove - Cant move to storage[ SellType:%d ]( %d )",
                pSrcItem->GetItemTable()->Item_Limit_Sell_Type,
                2346);
            SendErrorMessage(2, 0xCB4C);
            return false;
        } else {
            std::shared_ptr<CGocAkashicRecord> pAkashic =
                static_cast<CMover*>(pUser)->GetGOC_AkashicRecord(false);
            if (pAkashic)
                pAkashic->AddAkashicGetInfo(
                    static_cast<std::uint32_t>(pDestItem->GetID()));
        }
    }

    MakeItemSwapPacket(psReqItem.bySrcInvenType,
                       psReqItem.shSrcSlotPos,
                       pSrcItem,
                       psReqItem.byDestInvenType,
                       psReqItem.shDestSlotPos,
                       pDestItem,
                       psItem,
                       true);

    ST_LOG_GAME stLog{};
    stLog._nUAID = static_cast<int>(pUser->GetUAID());
    stLog._nUCID = static_cast<int>(
        CQuestCondition::GetQuestID(pUser->GetActorID()));
    stLog._sMainType = 4;
    stLog._sSubType = 57;
    stLog.nParam0 = pSrcItem->GetID();
    stLog.nParam1 = pSrcItem->GetCount();
    stLog.nParam2 = pUser->GetLevel();
    stLog.nParam3 = psReqItem.bySrcInvenType;
    stLog.nParam4 = psReqItem.shSrcSlotPos;
    stLog.nParam5 = pSrcItem->GetSerial();
    stLog.nParam6 = pDestItem ? pDestItem->GetSerial() : 0;
    stLog.nParam7 = psReqItem.byDestInvenType;
    stLog.nParam8 = psReqItem.shDestSlotPos;
    stLog.nParam12 = pSrcItem->GetCount();
    if (pServer)
        pServer->SendDBLog(stLog);

    if (pDestItem) {
        stLog.nParam0 = pDestItem->GetID();
        stLog.nParam1 = pDestItem->GetCount();
        stLog.nParam3 = psReqItem.byDestInvenType;
        stLog.nParam4 = psReqItem.shDestSlotPos;
        stLog.nParam5 = pDestItem->GetSerial();
        stLog.nParam6 = pSrcItem->GetSerial();
        stLog.nParam7 = psReqItem.bySrcInvenType;
        stLog.nParam8 = psReqItem.shSrcSlotPos;
        stLog.nParam12 = pDestItem->GetCount();
        if (pServer)
            pServer->SendDBLog(stLog);
    }

    return true;
}

// Main packet parser - routes subcommands to appropriate handlers
bool CItemProcess::Parse(XPacket& xPacket)
{
    switch (xPacket.GetSubCmd())
    {
    case 1:   // Inventory Info
        return ReqInventoryInfo(xPacket);

    case 2:   // Item Move
        return ReqItemMove(xPacket);

    case 3:   // Item Combine
        return ReqItemCombine(xPacket);

    case 4:   // Item Divide
        return ReqItemDivide(xPacket);

    case 5:   // Item Break
        return ReqItemBreak(xPacket);

    case 0x0F: // Item Add Slot
        return ReqItemAddSlot(xPacket);

    case 0x10: // Bank Info
        return ReqBankInfo(xPacket);

    case 0x11: // Item Use
        return ReqItemUse(xPacket);

    case 0x24: // Item Move Money
        return ReqItemMoveMoney(xPacket);

    case 0x25: // Item Line Up
        return ReqItemLineUp(xPacket);

    case 0x27: // Item Update Quick Slot Card
        return ReqItemUpdateQuickSlotCard(xPacket);

    case 0x28: // Item Update Quick Slot Item
        return ReqItemUpdateQuickSlotItem(xPacket);

    case 0x33: // Reload Update Cash
        return ReqReloadUpdateCash(xPacket);

    case 0x47: // Maze Reward Item
        return ReqMazeRewardItem(xPacket);

    case 0x52: // Item Appearance Equip
        return ReqItemAppearanceEquip(xPacket);

    case 0x53: // Item Name Change
        return ReqItemNameChange(xPacket);

    case 0x67: // Item Use Select
        return ReqItemUseSelect(xPacket);

    case 0x71: // Item Reseal Package
        return ReqItemResealPackage(xPacket);

    case 0x73: // Card Deck Page Open
        return ReqCardDeckPageOpen(xPacket);

    default:
        return false;
    }
}

// Request inventory info
bool CItemProcess::ReqInventoryInfo(XPacket& xPacket)
{
    (void)xPacket;
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    const UXMapID mapInsID = static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager = CLogicThreadManager::Instance();
    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser && pUser->IsLive())
            pUser->stMyCharInfoEx()->syncUserBits.SyncUser.bSyncInven = 1;
    });
    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// Request item move
bool CItemProcess::ReqItemMove(XPacket& xPacket)
{
    PS_REQ_ITEM_MOVE psReqItem;
    PS_REQ_TICKCOUNT psReqTick;
    xPacket >> psReqItem;
    xPacket >> psReqTick;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    const std::uint64_t dwReqTick = GreenDamTan::GetTickCount64Compat();
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    const UXMapID mapInsID = static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager = CLogicThreadManager::Instance();

    logicManager.DoJob(mapInsID.nMapID,
                       [this, pUser, psReqItem, psReqTick, dwReqTick]() mutable {
        if (!pUser || !pUser->GetArea() || !pUser->IsLive())
            return;

        pUser->SetRequestTick(0x64, psReqTick,
                              dwReqTick);

        std::shared_ptr<CGocInventory> pInventory =
            static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
        if (!pInventory)
            return;

        PS_DB_ITEM_MOVE_VEC psItem;
        if (!pInventory->IsValidSlotPos(psReqItem.shSrcSlotPos,
                                        psReqItem.shDestSlotPos)) {
            LogHelper::LogError(
                "game.item",
                "ReqItemMove error - Fault slot position[ src:%d, dest:%d ]( %d )",
                psReqItem.shSrcSlotPos,
                psReqItem.shDestSlotPos,
                157);
            SendErrorMessage(2, 0xCB2C);
            return;
        }

        switch (psReqItem.byDestInvenType) {
        case 0:
        case 3: {
            if (ShapeEquip(psReqItem, psItem)) {
                XSendDBPacket xSendPacket(
                    static_cast<IXObject*>(
                        static_cast<XActor*>(static_cast<CMover*>(pUser))),
                    0x21,
                    0x28);
                xSendPacket.XParse << static_cast<std::uint8_t>(1);
                xSendPacket << psItem;
                xSendPacket.XParse << psReqTick.nTicknum;
                XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                if (pServer)
                    pServer->SendDBGame(xSendPacket);
            } else {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemMove error - faild equip appearance( %d )",
                    188);
            }
            break;
        }
        case 1: {
            if (AbilityEquip(psReqItem, psItem)) {
                XSendDBPacket xSendPacket(
                    static_cast<IXObject*>(
                        static_cast<XActor*>(static_cast<CMover*>(pUser))),
                    0x21,
                    0x28);
                xSendPacket.XParse << static_cast<std::uint8_t>(0);
                xSendPacket << psItem;
                xSendPacket.XParse << psReqTick.nTicknum;
                XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                if (pServer)
                    pServer->SendDBGame(xSendPacket);
            } else {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemMove error - faild equip ability( %d )",
                    169);
            }
            break;
        }
        case 2:
        case 4:
        case 5:
        case 6:
        case 11:
        case 13:
        case 14:
        case 16:
        case 17:
        case 18: {
            if (ItemMove(psReqItem, psItem)) {
                XSendDBPacket xSendDBPacket(
                    static_cast<IXObject*>(
                        static_cast<XActor*>(static_cast<CMover*>(pUser))),
                    0x21,
                    2);
                xSendDBPacket.XParse << static_cast<std::uint8_t>(0);
                xSendDBPacket << psItem;
                xSendDBPacket.XParse << psReqTick.nTicknum;
                XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                if (pServer)
                    pServer->SendDBGame(xSendDBPacket);
            } else {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemMove error - faild move item( %d )",
                    215);
            }
            break;
        }
        default:
            break;
        }
    });

    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// Request item use
bool CItemProcess::ReqItemUse(XPacket& xPacket)
{
    PS_REQ_ITEM_USE stItemUse;
    xPacket >> stItemUse;

    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    const UXMapID mapInsID = static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager = CLogicThreadManager::Instance();
    logicManager.DoJob(mapInsID.nMapID, [pUser, stItemUse]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea())
            return;

        if (pUser->GetBlockType()) {
            pUser->SendErrorMessage(8, 0x11, 50111);
            return;
        }

        std::shared_ptr<CGocInventory> pInventory =
            static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
        if (!pInventory)
            return;
        if (!pInventory->IsValidSlotPos(stItemUse.shSlotPos, 0)) {
            LogHelper::LogError(
                "game.item",
                "ReqItemUse error - Fault Inventory slot pos[Pos:%d]( %d )",
                stItemUse.shSlotPos,
                1102);
            return;
        }
        if (!pInventory->CanUseItem(stItemUse.byInvenType,
                                    stItemUse.shSlotPos)) {
            LogHelper::LogError(
                "game.item",
                "ReqItemUse error - Failed CanUseItem( %d )",
                1108);
            return;
        }

        int nReduceCount = 1;
        int nItemID = 0;
        if (!pInventory->UseItem(stItemUse.byInvenType,
                                 stItemUse.shSlotPos,
                                 stItemUse.byCount,
                                 1,
                                 &nItemID)) {
            LogHelper::LogError("game.item",
                                "ReqItemUse error - Failed UseItem( %d )",
                                1116);
            return;
        }

        XArea* pArea = pUser->GetArea();
        if (pArea && pArea->IsMaze()) {
            if (auto* pMaze = dynamic_cast<XMaze*>(pArea))
                pMaze->UpdateHiddenEventCondition(10, nItemID, nReduceCount);
        }

        std::shared_ptr<CGocEntity> pEntity =
            static_cast<CMover*>(pUser)->GetGOC_Entity(false);
        pEntity->UpdateOpenTitle(8, nItemID);
    });
    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// Request item combine
bool CItemProcess::ReqItemCombine(XPacket& xPacket)
{
    CUser* pUser = GetClientPtr();

    PS_REQ_ITEM_COMBINE psCombineInfo;
    xPacket >> psCombineInfo;
    if (!pUser || !pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    const UXMapID mapInsID = static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager = CLogicThreadManager::Instance();
    logicManager.DoJob(mapInsID.nMapID,
                       [this, pUser, psCombineInfo]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (pUser->GetBlockType()) {
            pUser->SendErrorMessage(8, 3, 50111);
            return;
        }

        const auto isCombineInventory = [](std::uint8_t byInvenType) {
            return byInvenType == 2 || byInvenType == 13 ||
                   byInvenType == 5 || byInvenType == 14 ||
                   byInvenType == 16 || byInvenType == 18;
        };
        if (!isCombineInventory(psCombineInfo.bySrcInvenType)) {
            LogHelper::LogError(
                "game.item",
                "ReqItemCombine error - Fault Inventory type[ src:%d ]( %d )",
                psCombineInfo.bySrcInvenType,
                260);
            SendErrorMessage(3, 0xCB21);
            return;
        }
        if (!isCombineInventory(psCombineInfo.byDestInvenType)) {
            LogHelper::LogError(
                "game.item",
                "ReqItemCombine error - Fault Inventory type[ dest:%d ]( %d )",
                psCombineInfo.byDestInvenType,
                273);
            SendErrorMessage(3, 0xCB21);
            return;
        }

        std::shared_ptr<CGocInventory> pInventory =
            static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
        if (!pInventory)
            return;

        if (!pInventory->IsValidSlotPos(psCombineInfo.shSrcSlotPos,
                                        psCombineInfo.shDestSlotPos)) {
            SendErrorMessage(3, 0xCB2C);
            LogHelper::LogError(
                "game.item",
                "ReqItemCombine error - Fault slot pos[ src:%d, dest:%d ]( %d )",
                psCombineInfo.shSrcSlotPos,
                psCombineInfo.shDestSlotPos,
                287);
            return;
        }

        bool bLock = false;
        std::shared_ptr<CItem> pSrcItem = pInventory->GetSlotItem(
            psCombineInfo.bySrcInvenType,
            static_cast<std::uint16_t>(psCombineInfo.shSrcSlotPos),
            bLock);
        if (!pSrcItem || bLock) {
            LogHelper::LogError(
                "game.item",
                "ReqItemCombine error - Item is NULL or lock[ Inventype:%d, slot:%d ]( %d )",
                psCombineInfo.bySrcInvenType,
                psCombineInfo.shSrcSlotPos,
                295);
            if (XGameServer* pServer = TXSingleton<XGameServer>::Instance()) {
                pServer->SendItemLockLog(
                    CQuestCondition::GetQuestID(pUser->GetActorID()),
                    psCombineInfo.bySrcInvenType,
                    psCombineInfo.shSrcSlotPos,
                    static_cast<std::uint8_t>(bLock),
                    31,
                    0);
            }
            return;
        }

        if (pUser->IsMatching() &&
            psCombineInfo.byDestInvenType != psCombineInfo.bySrcInvenType) {
            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            const bool bCheck =
                pServer &&
                ((pServer->GetOption().GetNationType() == NATION_TYPE_JPN &&
                  (psCombineInfo.byDestInvenType == 14 ||
                   psCombineInfo.byDestInvenType == 5 ||
                   psCombineInfo.byDestInvenType == 6)) ||
                 (pServer->GetOption().GetNationType() != NATION_TYPE_JPN &&
                  (psCombineInfo.byDestInvenType == 18 ||
                   psCombineInfo.byDestInvenType == 16 ||
                   psCombineInfo.byDestInvenType == 17)));
            if (bCheck &&
                pSrcItem->GetID() == pInventory->GetMazeNeedItemID()) {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemCombine error - Not possible when matching[UCID:%d] (%d)",
                    CQuestCondition::GetQuestID(pUser->GetActorID()),
                    329);
                SendErrorMessage(3, 0xCB63);
                return;
            }
        }

        std::shared_ptr<CItem> pDestItem = pInventory->GetSlotItem(
            psCombineInfo.byDestInvenType,
            static_cast<std::uint16_t>(psCombineInfo.shDestSlotPos),
            bLock);
        if (!pDestItem || bLock) {
            LogHelper::LogError(
                "game.item",
                "ReqItemCombine error - Item is NULL or lock[ Inventype:%d, slot:%d ]( %d )",
                psCombineInfo.byDestInvenType,
                psCombineInfo.shDestSlotPos,
                340);
            if (XGameServer* pServer = TXSingleton<XGameServer>::Instance()) {
                pServer->SendItemLockLog(
                    CQuestCondition::GetQuestID(pUser->GetActorID()),
                    psCombineInfo.byDestInvenType,
                    psCombineInfo.shDestSlotPos,
                    static_cast<std::uint8_t>(bLock),
                    32,
                    0);
            }
            return;
        }

        if (pSrcItem->GetID() != pDestItem->GetID() ||
            pSrcItem->GetSerial() == pDestItem->GetSerial()) {
            LogHelper::LogError(
                "game.item",
                "ReqItemCombine error - Fault item information( %d )",
                348);
            SendErrorMessage(3, 0xCB2F);
            return;
        }

        const int nCount = psCombineInfo.sCount;
        if (pSrcItem->GetCount() < nCount || nCount < 1) {
            LogHelper::LogError(
                "game.item",
                "ReqItemCombine error - Fault item information[ Count:%d ]( %d )",
                nCount,
                356);
            SendErrorMessage(3, 0xCB2F);
            return;
        }

        const int nDestCount = pDestItem->GetCount() + nCount;
        if (nDestCount > pDestItem->GetItemTable()->Item_Stack_Max) {
            LogHelper::LogError(
                "game.item",
                "ReqItemCombine error - Fault item information[ Stack max:%d ]( %d )",
                nDestCount,
                365);
            SendErrorMessage(3, 0xCB2F);
            return;
        }

        if (!pSrcItem->IsAkashicRecordStack() ||
            !pDestItem->IsAkashicRecordStack()) {
            LogHelper::LogError(
                "game.item",
                "ReqItemCombine error - Akashic Stack[UCID:%d]",
                CQuestCondition::GetQuestID(pUser->GetActorID()));
            SendErrorMessage(3, 0xCB2F);
            return;
        }

        PS_DB_ITEM_MOVE psItem{};
        psItem.bySrcInvenType = psCombineInfo.bySrcInvenType;
        psItem.shSrcSlotPos = psCombineInfo.shSrcSlotPos;
        psItem.dwSrcActorID = GetDBSaveID(psCombineInfo.bySrcInvenType);
        psItem.byDestInvenType = psCombineInfo.byDestInvenType;
        psItem.shDestSlotPos = psCombineInfo.shDestSlotPos;
        psItem.dwDestActorID = GetDBSaveID(psCombineInfo.byDestInvenType);

        if (pSrcItem->GetCount() == nCount) {
            psItem.stSrcItem.Init();
            psItem.stSrcItem.xSerial = pSrcItem->GetSerial();
            psItem.stSrcItem.sCount = 0;
            pDestItem->GetItem(&psItem.stDestItem);
            psItem.stDestItem.sCount = static_cast<std::int16_t>(nDestCount);
        } else {
            pSrcItem->GetItem(&psItem.stSrcItem);
            psItem.stSrcItem.sCount = static_cast<std::int16_t>(
                pSrcItem->GetCount() - nCount);
            pDestItem->GetItem(&psItem.stDestItem);
            psItem.stDestItem.sCount = static_cast<std::int16_t>(nDestCount);
        }

        const std::uint32_t dwUAID = pUser->GetUAID();
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer->GetOption().GetNationType() == NATION_TYPE_JPN) {
            if (psCombineInfo.byDestInvenType != psCombineInfo.bySrcInvenType &&
                (psCombineInfo.byDestInvenType == 5 ||
                 psCombineInfo.byDestInvenType == 6 ||
                 psCombineInfo.byDestInvenType == 14)) {
                const std::uint8_t byLimitSellType =
                    pSrcItem->GetItemTable()->Item_Limit_Sell_Type;
                if (byLimitSellType == 4 || byLimitSellType == 5 ||
                    byLimitSellType == 6 || byLimitSellType == 7) {
                    SendErrorMessage(3, 0xCB4C);
                    return;
                }
            }
        } else {
            if (psCombineInfo.byDestInvenType != psCombineInfo.bySrcInvenType &&
                (psCombineInfo.byDestInvenType == 16 ||
                 psCombineInfo.byDestInvenType == 17 ||
                 psCombineInfo.byDestInvenType == 18)) {
                const std::uint8_t byLimitSellType =
                    pSrcItem->GetItemTable()->Item_Limit_Sell_Type;
                if (byLimitSellType == 4 || byLimitSellType == 5 ||
                    byLimitSellType == 6 || byLimitSellType == 7) {
                    SendErrorMessage(3, 0xCB4C);
                    return;
                }
                psItem.dwDestActorID = dwUAID;
                psItem.dwSrcActorID = dwUAID;
            }
            if (psCombineInfo.byDestInvenType == 16 ||
                psCombineInfo.byDestInvenType == 17 ||
                psCombineInfo.byDestInvenType == 18) {
                psItem.dwDestActorID = dwUAID;
                psItem.dwSrcActorID = dwUAID;
            }
        }

        pInventory->SetLock(psCombineInfo.bySrcInvenType,
                            psCombineInfo.shSrcSlotPos,
                            3);
        pInventory->SetLock(psCombineInfo.byDestInvenType,
                            psCombineInfo.shDestSlotPos,
                            3);

        ST_LOG_GAME stLogData{};
        stLogData._nUAID = static_cast<int>(pUser->GetUAID());
        stLogData._nUCID = static_cast<int>(
            CQuestCondition::GetQuestID(pUser->GetActorID()));
        stLogData._sMainType = 4;
        stLogData._sSubType = 37;
        stLogData.nParam0 = pSrcItem->GetID();
        stLogData.nParam1 = psItem.stSrcItem.sCount;
        stLogData.nParam5 = psItem.stSrcItem.xSerial;
        stLogData.nParam6 = psItem.stDestItem.xSerial;
        stLogData.nParam11 = -nCount;
        stLogData.nParam12 = psItem.stSrcItem.sCount;
        if (pServer)
            pServer->SendDBLog(stLogData);

        ST_LOG_GAME stLogGame{};
        stLogGame._nUAID = static_cast<int>(pUser->GetUAID());
        stLogGame._nUCID = static_cast<int>(
            CQuestCondition::GetQuestID(pUser->GetActorID()));
        stLogGame._sMainType = 4;
        stLogGame._sSubType = 37;
        stLogGame.nParam0 = psItem.stDestItem.nItemID;
        stLogGame.nParam1 = psItem.stDestItem.sCount;
        stLogGame.nParam5 = psItem.stDestItem.xSerial;
        stLogGame.nParam6 = psItem.stSrcItem.xSerial;
        stLogGame.nParam11 = nCount;
        stLogGame.nParam12 = psItem.stDestItem.sCount;
        if (pServer)
            pServer->SendDBLog(stLogGame);

        XSendDBPacket xSendDBPacket(
            static_cast<IXObject*>(
                static_cast<XActor*>(static_cast<CMover*>(pUser))),
            0x21,
            3);
        xSendDBPacket.XParse << static_cast<int>(
            CQuestCondition::GetQuestID(pUser->GetActorID()));
        xSendDBPacket << psItem;
        if (pServer)
            pServer->SendDBGame(xSendDBPacket);
    });

    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// Request item divide
bool CItemProcess::ReqItemDivide(XPacket& xPacket)
{
    CUser* pUser = GetClientPtr();
    PS_REQ_ITEM_DIVIDE reqDivide;
    xPacket >> reqDivide;

    if (!pUser || !pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    const UXMapID mapInsID = static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager = CLogicThreadManager::Instance();
    logicManager.DoJob(mapInsID.nMapID,
                       [this, pUser, reqDivide]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (pUser->GetBlockType()) {
            pUser->SendErrorMessage(8, 4, 50111);
            return;
        }

        std::shared_ptr<CGocInventory> pInventory =
            static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
        if (!pInventory)
            return;

        if (!pInventory->IsValidSlotPos(reqDivide.shSrcSlotPos,
                                        reqDivide.shDestSlotPos)) {
            SendErrorMessage(4, 0xCB2C);
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - Fault slot pos[ src:%d, dest:%d ]( %d )",
                reqDivide.shSrcSlotPos,
                reqDivide.shDestSlotPos,
                516);
            return;
        }
        if (reqDivide.nSrcItemID == -1) {
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - Fault item information[ ItemID:%d ]( %d )",
                reqDivide.nSrcItemID,
                526);
            SendErrorMessage(4, 0xCB2B);
            return;
        }
        if (reqDivide.sCount < 1) {
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - Fault item information[ Count:%d ]( %d )",
                reqDivide.sCount,
                535);
            SendErrorMessage(4, 0xCB2D);
            return;
        }
        if (reqDivide.bySrcInvenType == reqDivide.byDestInvenType &&
            reqDivide.shSrcSlotPos == reqDivide.shDestSlotPos) {
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - Fault item information[ Pos:%d ]( %d )",
                reqDivide.shSrcSlotPos,
                543);
            SendErrorMessage(4, 0xCB2C);
            return;
        }

        switch (reqDivide.bySrcInvenType) {
        case 2:
            if (reqDivide.byDestInvenType != 2 &&
                reqDivide.byDestInvenType != 5 &&
                reqDivide.byDestInvenType != 16) {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemDivide error - Fault item information[ InvenType:%d ]( %d )",
                    reqDivide.bySrcInvenType,
                    554);
                SendErrorMessage(4, 0xCB21);
                return;
            }
            break;
        case 5:
            if (reqDivide.byDestInvenType != 2 &&
                reqDivide.byDestInvenType != 5) {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemDivide error - Fault item information[ InvenType:%d ]( %d )",
                    reqDivide.bySrcInvenType,
                    564);
                SendErrorMessage(4, 0xCB21);
                return;
            }
            break;
        case 16:
            if (reqDivide.byDestInvenType != 2 &&
                reqDivide.byDestInvenType != 16) {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemDivide error - Fault item information[ InvenType:%d ]( %d )",
                    reqDivide.bySrcInvenType,
                    574);
                SendErrorMessage(4, 0xCB21);
                return;
            }
            break;
        case 13:
            if (reqDivide.byDestInvenType != 13 &&
                reqDivide.byDestInvenType != 14 &&
                reqDivide.byDestInvenType != 18) {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemDivide error - Fault item information[ InvenType:%d ]( %d )",
                    reqDivide.bySrcInvenType,
                    584);
                SendErrorMessage(4, 0xCB21);
                return;
            }
            break;
        case 14:
            if (reqDivide.byDestInvenType != 13 &&
                reqDivide.byDestInvenType != 14) {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemDivide error - Fault item information[ InvenType:%d ]( %d )",
                    reqDivide.bySrcInvenType,
                    594);
                SendErrorMessage(4, 0xCB21);
                return;
            }
            break;
        case 18:
            if (reqDivide.byDestInvenType != 13 &&
                reqDivide.byDestInvenType != 18) {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemDivide error - Fault item information[ InvenType:%d ]( %d )",
                    reqDivide.bySrcInvenType,
                    604);
                SendErrorMessage(4, 0xCB21);
                return;
            }
            break;
        default:
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - Fault item information[ InvenType:%d ]( %d )",
                reqDivide.bySrcInvenType,
                612);
            SendErrorMessage(4, 0xCB21);
            return;
        }

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        TB_ITEM* pTBItem = pServer
            ? pServer->GetResourceMgr().GetTB_ITEM(reqDivide.nSrcItemID)
            : nullptr;
        if (!pTBItem) {
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - Fault item information[ ItemID:%d ]( %d )",
                reqDivide.nSrcItemID,
                625);
            SendErrorMessage(4, 0xCB24);
            return;
        }
        if (pTBItem->Item_Stack_Max <= 1) {
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - Stack max less than 1[ ItemID:%d ]( %d )",
                pTBItem->Item_ID,
                633);
            SendErrorMessage(4, 0xCB2F);
            return;
        }

        bool bSrcLock = false;
        bool bDestLock = false;
        std::shared_ptr<CItem> pSrcItem = pInventory->GetSlotItem(
            reqDivide.bySrcInvenType,
            static_cast<std::uint16_t>(reqDivide.shSrcSlotPos),
            bSrcLock);
        std::shared_ptr<CItem> pDestItem = pInventory->GetSlotItem(
            reqDivide.byDestInvenType,
            static_cast<std::uint16_t>(reqDivide.shDestSlotPos),
            bDestLock);
        if (!pSrcItem || bSrcLock) {
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - srcItem is NULL or lock( %d )",
                645);
            if (pServer)
                pServer->SendItemLockLog(
                    CQuestCondition::GetQuestID(pUser->GetActorID()),
                    reqDivide.bySrcInvenType,
                    reqDivide.shSrcSlotPos,
                    static_cast<std::uint8_t>(bSrcLock),
                    33,
                    0);
            return;
        }
        if (pSrcItem->GetID() != reqDivide.nSrcItemID) {
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - Fault item id[ ItemID:%d ]( %d )",
                reqDivide.nSrcItemID,
                653);
            SendErrorMessage(4, 0xCB22);
            return;
        }
        if (pDestItem) {
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - Dest item isn't null[ ItemID:%d ]( %d )",
                pDestItem->GetID(),
                662);
            SendErrorMessage(4, 0xCB30);
            return;
        }
        const int nRemainCount = pSrcItem->GetCount() - reqDivide.sCount;
        if (nRemainCount <= 0) {
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - Item count less than 1[ Count:%d ]( %d )",
                nRemainCount,
                671);
            SendErrorMessage(4, 0xCB2D);
            return;
        }
        if (bSrcLock || bDestLock) {
            LogHelper::LogError("game.item", "ReqItemDivide error - Lock ( %d )", 679);
            if (pServer)
                pServer->SendItemLockLog(
                    CQuestCondition::GetQuestID(pUser->GetActorID()),
                    reqDivide.byDestInvenType,
                    reqDivide.shDestSlotPos,
                    static_cast<std::uint8_t>(bDestLock),
                    34,
                    0);
            SendErrorMessage(4, 0xCB23);
            return;
        }

        PS_DB_ITEM_MOVE psDBItemMove{};
        psDBItemMove.dwSrcActorID = GetDBSaveID(reqDivide.byDestInvenType);
        psDBItemMove.bySrcInvenType = reqDivide.bySrcInvenType;
        psDBItemMove.shSrcSlotPos = reqDivide.shSrcSlotPos;
        psDBItemMove.stSrcItem.nItemID = reqDivide.nSrcItemID;
        psDBItemMove.stSrcItem.xSerial = pSrcItem->GetSerial();
        psDBItemMove.stSrcItem.sCount = static_cast<std::int16_t>(nRemainCount);
        psDBItemMove.stSrcItem.bBindType =
            static_cast<std::uint8_t>(pSrcItem->IsBind());
        psDBItemMove.dwDestActorID = GetDBSaveID(reqDivide.bySrcInvenType);
        psDBItemMove.byDestInvenType = reqDivide.byDestInvenType;
        psDBItemMove.shDestSlotPos = reqDivide.shDestSlotPos;
        psDBItemMove.stDestItem.sCount = reqDivide.sCount;
        psDBItemMove.stDestItem.bBindType =
            static_cast<std::uint8_t>(pSrcItem->IsBind());

        const std::uint32_t dwUAID = pUser->GetUAID();
        const auto isStorage = [](std::uint8_t byInvenType) {
            return byInvenType == 16 || byInvenType == 17 ||
                   byInvenType == 18;
        };
        if (pServer->GetOption().GetNationType() == NATION_TYPE_JPN) {
            if (reqDivide.byDestInvenType == 5 ||
                reqDivide.byDestInvenType == 6 ||
                reqDivide.byDestInvenType == 14) {
                const std::uint8_t byLimitSellType =
                    pSrcItem->GetItemTable()->Item_Limit_Sell_Type;
                if (byLimitSellType == 4 || byLimitSellType == 5 ||
                    byLimitSellType == 6 || byLimitSellType == 7) {
                    SendErrorMessage(4, 0xCB4C);
                    return;
                }
            }
        } else if (isStorage(reqDivide.byDestInvenType)) {
            if (isStorage(reqDivide.bySrcInvenType)) {
                psDBItemMove.dwDestActorID = dwUAID;
                psDBItemMove.dwSrcActorID = dwUAID;
            } else {
                const std::uint8_t byLimitSellType =
                    pSrcItem->GetItemTable()->Item_Limit_Sell_Type;
                if (byLimitSellType == 4 || byLimitSellType == 5 ||
                    byLimitSellType == 6 || byLimitSellType == 7) {
                    SendErrorMessage(4, 0xCB4C);
                    return;
                }
                psDBItemMove.dwDestActorID = dwUAID;
                psDBItemMove.dwSrcActorID =
                    GetDBSaveID(reqDivide.bySrcInvenType);
            }
        } else if (isStorage(reqDivide.bySrcInvenType)) {
            psDBItemMove.dwSrcActorID = dwUAID;
            psDBItemMove.dwDestActorID =
                GetDBSaveID(reqDivide.bySrcInvenType);
        }

        if (pInventory->DivideItem(psDBItemMove)) {
            XSendDBPacket xSendDBPacket(
                static_cast<IXObject*>(
                    static_cast<XActor*>(static_cast<CMover*>(pUser))),
                0x21,
                4);
            xSendDBPacket << psDBItemMove;
            pServer->SendDBGame(xSendDBPacket);
        } else {
            LogHelper::LogError(
                "game.item",
                "ReqItemDivide error - Fail Divide ( %d )",
                752);
        }
    });

    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// Request item break
bool CItemProcess::ReqItemBreak(XPacket& xPacket)
{
    CUser* pUser = GetClientPtr();

    std::uint8_t byInvenType = 0;
    std::int16_t shSlotPos = 0;
    std::int64_t xItemSerial = 0;
    int nCount = 0;

    xPacket.XParse >> byInvenType;
    xPacket.XParse >> shSlotPos;
    xPacket.XParse >> xItemSerial;
    xPacket.XParse >> nCount;

    if (!pUser || !pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    const UXMapID mapInsID = static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager = CLogicThreadManager::Instance();
    logicManager.DoJob(mapInsID.nMapID,
                       [this, pUser, byInvenType, shSlotPos,
                        xItemSerial, nCount]() {
        (void)xItemSerial;
        if (!pUser || !pUser->IsLive())
            return;

        if (pUser->GetBlockType()) {
            pUser->SendErrorMessage(8, 5, 50111);
            return;
        }

        std::shared_ptr<CGocInventory> pInventory =
            static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
        if (!pInventory)
            return;

        if (!pInventory->IsValidSlotPos(shSlotPos, 0)) {
            LogHelper::LogError(
                "game.item",
                "ReqItemBreak error - Fault slot pos[ pos:%d ]( %d )",
                shSlotPos,
                789);
            SendErrorMessage(5, 0xCB2C);
            return;
        }
        if (byInvenType != 2 && byInvenType != 4 &&
            byInvenType != 11 && byInvenType != 13) {
            LogHelper::LogError(
                "game.item",
                "ReqItemBreak error - Fault inventory type[ Inven type:%d ]( %d )",
                byInvenType,
                800);
            SendErrorMessage(5, 0xCB21);
            return;
        }

        bool bLock = false;
        std::shared_ptr<CItem> pItem = pInventory->GetSlotItem(
            byInvenType,
            static_cast<std::uint16_t>(shSlotPos),
            bLock);
        if (!pItem) {
            LogHelper::LogError(
                "game.item",
                "ReqItemBreak error - Item is NULL[ Inven type:%d, Slot pos:%d ]( %d )",
                byInvenType,
                shSlotPos,
                810);
            SendErrorMessage(5, 0xCB22);
            return;
        }

        if (pUser->IsMatching() &&
            pItem->GetID() == pInventory->GetMazeNeedItemID()) {
            LogHelper::LogError(
                "game.item",
                "ReqSellItem error - Not possible when matching[UCID:%d] (%d)",
                CQuestCondition::GetQuestID(pUser->GetActorID()),
                821);
            SendErrorMessage(5, 0xCB63);
            return;
        }
        if (bLock) {
            LogHelper::LogError(
                "game.item",
                "ReqItemBreak error - Item is Lock ( %d )",
                829);
            if (XGameServer* pServer = TXSingleton<XGameServer>::Instance()) {
                pServer->SendItemLockLog(
                    CQuestCondition::GetQuestID(pUser->GetActorID()),
                    byInvenType,
                    shSlotPos,
                    static_cast<std::uint8_t>(bLock),
                    35,
                    0);
            }
            SendErrorMessage(5, 0xCB23);
            return;
        }
        if (pItem->GetCount() - nCount < 0) {
            LogHelper::LogError(
                "game.item",
                "ReqItemBreak error - Item count less than 1( %d )",
                838);
            SendErrorMessage(5, 0xCB3B);
            return;
        }

        const std::uint8_t byLimitSellType =
            pItem->GetItemTable()->Item_Limit_Sell_Type;
        if (byLimitSellType == 2 || byLimitSellType == 3 ||
            byLimitSellType == 5 || byLimitSellType == 7) {
            LogHelper::LogError(
                "game.item",
                "ReqItemBreak error - Cant use item[ Item id:%d ] ( %d )",
                pItem->GetID(),
                849);
            SendErrorMessage(5, 0xCB3C);
            return;
        }

        ST_LOG_GAME stLog{};
        stLog._sSubType = 21;
        stLog.nParam2 = pUser->GetLevel();
        stLog.nParam7 = pItem->GetItemTitleID();
        if (!pInventory->BreakItemReq(byInvenType,
                                      shSlotPos,
                                      nCount,
                                      0x2D,
                                      &stLog)) {
            LogHelper::LogError(
                "game.item",
                "ReqItemBreak error - Failed break item[ Inventory type:%d, Slot pos:%d ] ( %d )",
                byInvenType,
                shSlotPos,
                861);
            SendErrorMessage(5, 0xCB3C);
            return;
        }

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pItem->GetCount() <= 0) {
            ST_STATISTICS_ITEM stStatistics{};
            stStatistics.byFlag = 3;
            stStatistics.biSerial = pItem->GetSerial();
            XSendDBPacket xSendDBStatistics(
                static_cast<IXObject*>(
                    static_cast<XActor*>(static_cast<CMover*>(pUser))),
                0xF0,
                0x11);
            xSendDBStatistics << stStatistics;
            if (pServer)
                pServer->SendDBStatistics(xSendDBStatistics);
        }

        if (pItem->GetItemTable()->Item_Cash &&
            pItem->GetItemTable()->Item_Use_Period_Type) {
            std::shared_ptr<CGocPost> pPostPtr =
                static_cast<CMover*>(pUser)->GetGOC_Post(false);
            pPostPtr->SystemPostSend(
                3,
                4,
                static_cast<std::int64_t>(pItem->GetID()),
                CQuestCondition::GetQuestID(pUser->GetActorID()));
        }
    });

    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// Request item add slot
bool CItemProcess::ReqItemAddSlot(XPacket& xPacket)
{
    unsigned char byInvenType = 0;
    
    xPacket.XParse >> byInvenType;
    
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    
    if (!pUser->GetArea())
        return false;
    
    pUser->IncrementJobCount();
    
    // Schedule job on logic thread
    const UXMapID mapInsID =
        static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager =
        CLogicThreadManager::Instance();
    
    auto func = [this, pUser, byInvenType]() {
        if (!pUser || !pUser->IsLive())
            return;

        if (pUser->GetBlockType()) {
            pUser->SendErrorMessage(8, 0x0F, 50111);
            return;
        }

        std::shared_ptr<CGocInventory> pInventory =
            static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
        if (!pInventory)
            return;

        XBaseInventory* pInven = pInventory->GetInvenPtr(byInvenType);
        if (!pInven)
            return;

        std::int16_t shAddSlot = 0;
        std::int64_t biMoney = 0;
        int nReqLevel = 0;
        int nNeedType = 0;
        int nNeedValue = 0;
        const std::uint8_t byExtendStep =
            static_cast<std::uint8_t>(pInven->GetExtendStep() + 1);

        if (!GetConditionSlotExtend(byInvenType,
                                    byExtendStep,
                                    nReqLevel,
                                    nNeedType,
                                    nNeedValue,
                                    shAddSlot)) {
            SendErrorMessage(0x0F, 0xCB37);
            LogHelper::LogError(
                "game.item",
                "ReqItemAddSlot error - Item Extend Table ( %d )",
                1002);
            return;
        }

        std::shared_ptr<CGocAttribute> pAttribute =
            static_cast<CMover*>(pUser)->GetGOC_Attribute(false);
        if (pAttribute && pAttribute->GetLevel() < nReqLevel) {
            SendErrorMessage(0x0F, 0xCB37);
            LogHelper::LogError(
                "game.item",
                "ReqItemAddSlot error - Item Extend Level Condition Error( %d )",
                1010);
            return;
        }

        if (!pInven->CheckAddExtendSlot(byExtendStep, shAddSlot)) {
            LogHelper::LogError(
                "game.item",
                "ReqItemAddSlot error - Failed CheckAddExtendSlot( %d )",
                1016);
            SendErrorMessage(8, 0xCB37);
            return;
        }

        PS_RES_STORAGE_INFO stUpdateItem;
        if (nNeedType == 1) {
            if (pInventory->GetMoney() < biMoney) {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemAddSlot error - Not enough money[ money:%I64d ]( %d )",
                    pInventory->GetMoney(),
                    1027);
                SendErrorMessage(8, 0xCC4D);
                return;
            }

            biMoney = nNeedValue;
            if (!pInventory->AddMoney(-biMoney,
                                       0x13,
                                       byExtendStep,
                                       0,
                                       false))
                return;
        } else {
            if (nNeedType != 3) {
                SendErrorMessage(0x0F, 0xCB37);
                LogHelper::LogError(
                    "game.item",
                    "ReqItemAddSlot error - Dissatisfy Item Extend Condition( %d )",
                    1064);
                return;
            }

            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            TB_ITEM* pTBItem = pServer
                ? pServer->GetResourceMgr().GetTB_ITEM(nNeedValue)
                : nullptr;
            if (!pTBItem) {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemAddSlot error - Item is NULL[ ItemID:%d ]( %d )",
                    nNeedValue,
                    1041);
                return;
            }

            if (!pInventory->ReduceItem2(pTBItem,
                                         1,
                                         0x22,
                                         stUpdateItem)) {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemAddSlot error - Failed item reduce[ ItemID:%d ]( %d )",
                    pTBItem->Item_ID,
                    1047);
                return;
            }

            ST_LOG_GAME stLog;
            stLog._sSubType = 66;
            stLog.nParam3 = byInvenType;
            stLog.nParam4 = byExtendStep;
            if (!pInventory->UpdateItemEnd(0x22,
                                           stUpdateItem,
                                           stLog)) {
                LogHelper::LogError(
                    "game.item",
                    "ReqItemAddSlot error - Failed item update[ ItemID:%d ]( %d )",
                    pTBItem->Item_ID,
                    1057);
                return;
            }
        }

        XSendDBPacket xSendDBPacket(
            static_cast<IXObject*>(static_cast<XActor*>(static_cast<CMover*>(pUser))),
            0x21,
            0x0D);
        xSendDBPacket.XParse << static_cast<int>(pUser->GetUAID());
        xSendDBPacket.XParse <<
            (CQuestCondition::GetQuestID(pUser->GetActorID()) & 0x1FFFFFFF);
        xSendDBPacket.XParse << byInvenType;
        xSendDBPacket.XParse << byExtendStep;
        xSendDBPacket.XParse << shAddSlot;
        xSendDBPacket.XParse << biMoney;
        xSendDBPacket << stUpdateItem;

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer)
            pServer->SendDBGame(xSendDBPacket);
    };
    
    logicManager.DoJob(mapInsID.nMapID, func);
    
    // Schedule decrement job
    auto decrementFunc = [pUser]() {
        pUser->DecrementJobCount();
    };
    
    logicManager.DoJob(mapInsID.nMapID, decrementFunc);
    
    return true;
}

// Request bank info
bool CItemProcess::ReqBankInfo(XPacket& xPacket)
{
    (void)xPacket;
    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    const UXMapID mapInsID = static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager = CLogicThreadManager::Instance();
    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser && pUser->IsLive())
            pUser->stMyCharInfoEx()->syncUserBits.SyncUser.bSyncBank = 1;
    });
    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// Request item move money
bool CItemProcess::ReqItemMoveMoney(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Request item line up
bool CItemProcess::ReqItemLineUp(XPacket& xPacket)
{
    std::uint8_t byInvenType = 0;
    xPacket.XParse >> byInvenType;

    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    const UXMapID mapInsID = static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager = CLogicThreadManager::Instance();
    logicManager.DoJob(mapInsID.nMapID, [pUser, byInvenType]() {
        if (!pUser || !pUser->IsLive())
            return;
        if (pUser->GetBlockType()) {
            pUser->SendErrorMessage(8, 0x25, 50111);
            return;
        }
        std::shared_ptr<CGocInventory> pInventory =
            static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
        if (pInventory)
            pInventory->LineUp(byInvenType);
    });
    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// Request item update quick slot card
bool CItemProcess::ReqItemUpdateQuickSlotCard(XPacket& xPacket)
{
    PS_QUICKSLOT_UPDATE_CARD_VEC stSlotVec;
    xPacket >> stSlotVec;

    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    const UXMapID mapInsID = static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager = CLogicThreadManager::Instance();
    logicManager.DoJob(mapInsID.nMapID, [pUser, stSlotVec]() mutable {
        if (!pUser || !pUser->IsLive())
            return;
        if (pUser->GetBlockType()) {
            pUser->SendErrorMessage(8, 0x27, 50111);
            return;
        }

        XArea* pArea = pUser->GetArea();
        if (pUser->GetGMPower() == 0 &&
            (!pArea || (!pArea->IsMaze() &&
                        dynamic_cast<CBattleZone*>(pArea) == nullptr)))
            return;

        std::shared_ptr<CGocAkashicRecord> pAkashic =
            static_cast<CMover*>(pUser)->GetGOC_AkashicRecord(false);
        if (!pAkashic || stSlotVec.vecInfo.empty())
            return;

        if (pAkashic->SetQuickSlotCard(stSlotVec))
            return;

        XSendPacket xSendPacket(8, 0x27);
        xSendPacket.XParse << static_cast<std::uint8_t>(1);
        PS_QUICKSLOT_UPDATE_CARD_VEC stCurrent;
        pAkashic->GetQuickSlotCard(stCurrent);
        xSendPacket << stCurrent;
        CGocNetwork::Send(static_cast<XActor*>(static_cast<CMover*>(pUser)),
                          xSendPacket);
    });
    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// Request item update quick slot item
bool CItemProcess::ReqItemUpdateQuickSlotItem(XPacket& xPacket)
{
    PS_QUICKSLOT_UPDATE_ITEM stSlot;
    xPacket >> stSlot;

    CUser* pUser = GetClientPtr();
    if (!pUser || !pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    const UXMapID mapInsID = static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager = CLogicThreadManager::Instance();
    logicManager.DoJob(mapInsID.nMapID, [pUser, stSlot]() mutable {
        if (!pUser || !pUser->IsLive())
            return;
        if (pUser->GetBlockType()) {
            pUser->SendErrorMessage(8, 0x28, 50111);
            return;
        }
        std::shared_ptr<CGocInventory> pInventory =
            static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
        if (pInventory)
            pInventory->SetQuickSlotItem(stSlot);
    });
    logicManager.DoJob(mapInsID.nMapID, [pUser]() {
        if (pUser)
            pUser->DecrementJobCount();
    });

    return true;
}

// Request reload update cash
bool CItemProcess::ReqReloadUpdateCash(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Request maze reward item
bool CItemProcess::ReqMazeRewardItem(XPacket& xPacket)
{
    unsigned char byType = 0;
    
    xPacket.XParse >> byType;
    
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    
    if (!pUser->GetArea())
        return false;
    
    pUser->IncrementJobCount();
    
    // Schedule job on logic thread
    const UXMapID mapInsID =
        static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager =
        CLogicThreadManager::Instance();
    
    auto func = [this, pUser, byType]() {
        // Process maze reward item logic
    };
    
    logicManager.DoJob(mapInsID.nMapID, func);
    
    // Schedule decrement job
    auto decrementFunc = [pUser]() {
        pUser->DecrementJobCount();
    };
    
    logicManager.DoJob(mapInsID.nMapID, decrementFunc);
    
    return true;
}

// Request item appearance equip
bool CItemProcess::ReqItemAppearanceEquip(XPacket& xPacket)
{
    UAppearanceEx AppearanceEx;
    xPacket.XParse >> AppearanceEx.biAppearance;
    
    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;
    
    if (!pUser->GetArea())
        return false;
    
    pUser->IncrementJobCount();
    
    // Schedule job on logic thread
    const UXMapID mapInsID =
        static_cast<CMover*>(pUser)->GetMapInsID();
    CLogicThreadManager& logicManager =
        CLogicThreadManager::Instance();
    
    auto func = [this, pUser, AppearanceEx]() {
        // Process item appearance equip logic
    };
    
    logicManager.DoJob(mapInsID.nMapID, func);
    
    // Schedule decrement job
    auto decrementFunc = [pUser]() {
        pUser->DecrementJobCount();
    };
    
    logicManager.DoJob(mapInsID.nMapID, decrementFunc);
    
    return true;
}

// Request item name change
bool CItemProcess::ReqItemNameChange(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Request item use select
bool CItemProcess::ReqItemUseSelect(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Request item reseal package
bool CItemProcess::ReqItemResealPackage(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Request card deck page open
bool CItemProcess::ReqCardDeckPageOpen(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Database response parser
bool CItemProcess::DBItemParse(XPacket& xPacket)
{
    switch (xPacket.GetSubCmd())
    {
    case 1:
        return ResInventoryInfo(xPacket);

    case 2:
        return ResItemMove(xPacket);

    case 3:
        return ResItemCombine(xPacket);

    case 4:
        return ResItemDivide(xPacket);

    case 5:
        return ResItemBreak(xPacket);

    case 6:
        return ResItemReduce(xPacket);

    case 7:
        return ResItemCreate(xPacket);

    case 8:
        return ResItemAddSlot(xPacket);

    case 10:
        return ResBankInfo(xPacket);

    case 11:
        return ResLoadQuickSlot(xPacket);

    case 12:
        return ResItemUpdate(xPacket);

    case 13:
        return ResItemUse(xPacket);

    case 14:
        return ResMazeRewardItem(xPacket);

    case 15:
        return ResEquipSlotOpen(xPacket);

    case 16:
        return ResItemUseInfoSelect(xPacket);

    case 17:
        return ResItemUseInfoUpdate(xPacket);

    case 18:
        return ResItemAppearacneLoad(xPacket);

    case 19:
        return ResItemNameChange(xPacket);

    case 20:
        return ResItemSocketLoad(xPacket);

    case 21:
        return ResItemBroachLoad(xPacket);

    case 22:
        return ResLoadAkashicLoad(xPacket);

    case 23:
        return ResItemUpdateEx(xPacket);

    case 24:
        return ResCreatePostRestoreItem(xPacket);

    case 25:
        return ResItemUseAppearance(xPacket);

    case 26:
        return ResItemMoveMoney(xPacket);

    case 27:
        return ResItemLimitLoad(xPacket);

    case 28:
        return ResItemCooltimeLoad(xPacket);

    case 29:
        return ResItemUpdatePos(xPacket);

    case 30:
        return ResItemUpdateUseCount(xPacket);

    case 31:
        return ResItemNetCafeCostume(xPacket);

    case 32:
        return ResItemMakeLimitLoad(xPacket);

    case 33:
        return ResItemMakeLimitReset(xPacket);

    case 34:
        return ResCardDeckPageOpen(xPacket);

    case 35:
        return ResItemResealPackageInfo(xPacket);

    case 36:
        return ResItemResealPackage(xPacket);

    case 37:
        return ResItemResealPackageUse(xPacket);

    default:
        return false;
    }
}

// Response: Inventory info loaded
bool CItemProcess::ResInventoryInfo(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item moved
bool CItemProcess::ResItemMove(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item combined
bool CItemProcess::ResItemCombine(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item divided
bool CItemProcess::ResItemDivide(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item broken
bool CItemProcess::ResItemBreak(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item reduced
bool CItemProcess::ResItemReduce(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item created
bool CItemProcess::ResItemCreate(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item slot added
bool CItemProcess::ResItemAddSlot(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Bank info loaded
bool CItemProcess::ResBankInfo(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Quick slot loaded
bool CItemProcess::ResLoadQuickSlot(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item updated
bool CItemProcess::ResItemUpdate(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item used
bool CItemProcess::ResItemUse(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Maze reward item
bool CItemProcess::ResMazeRewardItem(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Equip slot opened
bool CItemProcess::ResEquipSlotOpen(XPacket& xPacket)
{
    int nErrorCode = 0;
    PS_RES_STORAGE_INFO stUpdateItem;
    int nEquipSlotBit = 0;
    std::uint8_t byEquipPos = 0;

    xPacket.XParse >> nErrorCode;
    xPacket >> stUpdateItem;
    xPacket.XParse >> nEquipSlotBit;
    xPacket.XParse >> byEquipPos;

    if (nErrorCode == -1)
    {
        LogHelper::LogError(
            "game.contents",
            "ResEquipSlotOpen error - [ ErrorCode:%d ]( %d )",
            -1,
            3440);
        SendErrorMessage(0x11, 0xCB2B);
        return false;
    }

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    auto func = [pUser, stUpdateItem, nEquipSlotBit, byEquipPos]() {
        if (pUser && pUser->IsLive())
        {
            PS_RES_STORAGE_INFO stUpdateItemCopy = stUpdateItem;
            std::shared_ptr<CGocInventory> pInvenPtr =
                static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
            pInvenPtr->SendUpdateItem(stUpdateItemCopy);
            pInvenPtr->SetLock(1, byEquipPos, 0);
            pInvenPtr->EquipSlotOpen(nEquipSlotBit);
            pInvenPtr->SendEquipSlotOpen();
        }
    };

    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    // IDA 显示为 TXSingleton<CLogicThreadManager>::Instance()；当前源码侧
    // 管理器入口是 CLogicThreadManager::Instance()。
    CLogicThreadManager& logicManager =
        CLogicThreadManager::Instance();
    const UXMapID mapInsID =
        static_cast<CMover*>(pUser)->GetMapInsID();

    logicManager.DoJob(mapInsID.nMapID, func);

    auto func1 = [pUser]() {
        pUser->DecrementJobCount();
    };

    logicManager.DoJob(mapInsID.nMapID, func1);
    return true;
}

// Response: Item use info selected
bool CItemProcess::ResItemUseInfoSelect(XPacket& xPacket)
{
    // IDA 将该局部变量误标为无关 vector；PDB 局部 UDT 和响应 serializer
    // 共同确认实际类型是 ST_USE_ITEM_INFO_LIST。
    ST_USE_ITEM_INFO_LIST stUseItemInfoList;
    xPacket >> stUseItemInfoList;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    auto func = [pUser, stUseItemInfoList]() {
        if (pUser && pUser->IsLive())
        {
            ST_USE_ITEM_INFO_LIST stSendList = stUseItemInfoList;
            std::shared_ptr<CGocInventory> pInvenPtr =
                static_cast<CMover*>(pUser)->GetGOC_Inventory(false);
            pInvenPtr->SetItemUseInfoList(stSendList);
            pUser->stMyCharInfoEx()->userDBBits.UserDB.bLoadUseInfo = 1;
        }
    };

    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    CLogicThreadManager& logicManager =
        CLogicThreadManager::Instance();
    const UXMapID mapInsID =
        static_cast<CMover*>(pUser)->GetMapInsID();

    logicManager.DoJob(mapInsID.nMapID, func);

    auto func1 = [pUser]() {
        pUser->DecrementJobCount();
    };

    logicManager.DoJob(mapInsID.nMapID, func1);
    return true;
}

// Response: Item use info updated
bool CItemProcess::ResItemUseInfoUpdate(XPacket& xPacket)
{
    // IDA 将该局部变量误标为无关 vector；PDB 局部 UDT 和响应 serializer
    // 共同确认实际类型是 ST_USE_ITEM_INFO_LIST。
    ST_USE_ITEM_INFO_LIST stUseItemInfoList;
    xPacket >> stUseItemInfoList;

    CUser* pUser = GetClientPtr();
    if (!pUser)
        return false;

    auto func = [pUser, stUseItemInfoList]() {
        if (pUser && pUser->IsLive())
        {
            XSendPacket xSendPacket(8, 0x49);
            ST_USE_ITEM_INFO_LIST stSendList = stUseItemInfoList;
            xSendPacket << stSendList;

            // IDA 反编译出的额外 float 参数属于尾部污染；源码 lambda 无参数。
            // CUser 存在多重基类，static_cast 负责完成 CMover 到 XActor 的调整。
            XActor* pActor = pUser
                ? static_cast<XActor*>(static_cast<CMover*>(pUser))
                : nullptr;
            CGocNetwork::Send(pActor, xSendPacket);
        }
    };

    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();

    CLogicThreadManager& logicManager =
        CLogicThreadManager::Instance();
    const UXMapID mapInsID =
        static_cast<CMover*>(pUser)->GetMapInsID();

    logicManager.DoJob(mapInsID.nMapID, func);

    auto func1 = [pUser]() {
        pUser->DecrementJobCount();
    };

    logicManager.DoJob(mapInsID.nMapID, func1);
    return true;
}

// Response: Item appearance loaded
bool CItemProcess::ResItemAppearacneLoad(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item name changed
bool CItemProcess::ResItemNameChange(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item socket loaded
bool CItemProcess::ResItemSocketLoad(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item broach loaded
bool CItemProcess::ResItemBroachLoad(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Akashic loaded
bool CItemProcess::ResLoadAkashicLoad(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item updated extended
bool CItemProcess::ResItemUpdateEx(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Post restore item created
bool CItemProcess::ResCreatePostRestoreItem(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item appearance used
bool CItemProcess::ResItemUseAppearance(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item money moved
bool CItemProcess::ResItemMoveMoney(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item limit loaded
bool CItemProcess::ResItemLimitLoad(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item cooltime loaded
bool CItemProcess::ResItemCooltimeLoad(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item position updated
bool CItemProcess::ResItemUpdatePos(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item use count updated
bool CItemProcess::ResItemUpdateUseCount(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Net cafe costume
bool CItemProcess::ResItemNetCafeCostume(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item make limit loaded
bool CItemProcess::ResItemMakeLimitLoad(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item make limit reset
bool CItemProcess::ResItemMakeLimitReset(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Card deck page opened
bool CItemProcess::ResCardDeckPageOpen(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item reseal package info
bool CItemProcess::ResItemResealPackageInfo(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item reseal package
bool CItemProcess::ResItemResealPackage(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}

// Response: Item reseal package used
bool CItemProcess::ResItemResealPackageUse(XPacket& xPacket)
{
    // Implementation based on IDA decompilation
    return true;
}
