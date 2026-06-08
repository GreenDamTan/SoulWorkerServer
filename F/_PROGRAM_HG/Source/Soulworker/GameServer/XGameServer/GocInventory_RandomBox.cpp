// Random Box / Gacha / Loot Box System Implementation
// Decompiled from GameServer.exe IDA port 10004
// 
// This file contains the random box opening mechanics, gacha pull system,
// loot table management, probability calculations, and reward distribution.

#include "GocInventory.h"
#include "XGameServer.h"
#include "XResourceMgr.h"
#include "XItemFactory.h"
#include "CItem.h"
#include "CUser.h"
#include "LogHelper.h"
#include "XSendDBPacket.h"
#include "XPacket.h"

// ============================================================================
// Random Box System
// ============================================================================

bool CGocInventory::CanRandomBoxUse(std::tr1::shared_ptr<CItem> pItem)
{
    // Address: 0x1400B4BE0
    // Check if random box can be used
    
    if (!pItem || pItem->GetItemTable() == nullptr)
    {
        return false;
    }
    
    TB_ITEM* pTBItem = pItem->GetItemTable();
    if (pTBItem->Item_Effect_Type != 6) // Random Box type
    {
        return false;
    }
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM_RANDOMBOX* pTBRandomBox = pServer->m_xResourceMgr.GetTB_ITEM_RANDOMBOX_With_Lock(pTBItem->Item_Effect_ID);
    
    if (!pTBRandomBox)
    {
        return false;
    }
    
    return true;
}

bool CGocInventory::RandomBoxUse(bool bReduceItem, std::tr1::shared_ptr<CItem> pItem, 
                                  int nItemIDparClass, unsigned char byCount, 
                                  bool bTool, int nToolItemID)
{
    // Address: 0x1400B4CC0
    // Main random box opening function with probability-based item selection
    
    // Validate item
    if (!pItem && !bTool)
    {
        CUser* pUser = GetUser();
        if (pUser)
        {
            CGocNetwork::SendErrorMessage(pUser, 8, 0x11, 0xCB2B);
        }
        LogHelper::LogError("game.item", "RandomBoxUse error - Item is NULL");
        return false;
    }
    
    // Validate count (1-10)
    if (!byCount || byCount > 10)
    {
        CUser* pUser = GetUser();
        if (pUser)
        {
            CGocNetwork::SendErrorMessage(pUser, 8, 0x11, 0xCB2B);
        }
        LogHelper::LogError("game.item", "RandomBoxUse error - Invalid count: %d", byCount);
        return false;
    }
    
    // Get item ID
    int nItemID = 0;
    unsigned __int64 biItemSerial = 0;
    
    if (bTool)
    {
        nItemID = nToolItemID;
    }
    else
    {
        nItemID = pItem->GetItemTable()->Item_ID;
        if (pItem->GetItemTable()->Item_Effect_Type == 7)
        {
            nItemID = nItemIDparClass;
        }
        biItemSerial = pItem->GetSerial();
    }
    
    // Get random box table
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM_RANDOMBOX* pTBRandomBox = pServer->m_xResourceMgr.GetTB_ITEM_RANDOMBOX_With_Lock(nItemID);
    
    if (!pTBRandomBox)
    {
        CUser* pUser = GetUser();
        if (pUser)
        {
            CGocNetwork::SendErrorMessage(pUser, 8, 0x11, 0xCB2B);
        }
        LogHelper::LogError("game.item", "RandomBoxUse error - No TB_ITEM_RANDOMBOX table");
        return false;
    }
    
    // Calculate rewards
    int nBP = 0;
    int nZenny = 0;
    int nEther = 0;
    
    ST_GET_INFO stGetInfo;
    ST_CREATE_ITEMS stAddItemList;
    
    // Process each box opening
    for (int k = 0; k < byCount; ++k)
    {
        int nMaxRate = pTBRandomBox->Rate_Sum;
        bool bGetItem[18] = {false};
        
        // Give items based on probability
        for (int j = 0; j < pTBRandomBox->Give_Count; ++j)
        {
            // Generate random number
            int nRate = pServer->m_xItemFactory.nRand(1, nMaxRate);
            
            // Check each reward slot (15 items + 3 currency types)
            for (int m = 0; m < 18; ++m)
            {
                int* pRate = &pTBRandomBox->Gold_Rate + m;
                if (*pRate && !bGetItem[m])
                {
                    if (nRate <= *pRate)
                    {
                        // Currency rewards (Gold=0, BP=1, Ether=2)
                        if (m < 3)
                        {
                            bGetItem[m] = true;
                            nMaxRate -= *pRate;
                            
                            if (m == 0)
                            {
                                nZenny += pTBRandomBox->RB_Gold;
                            }
                            else if (m == 1)
                            {
                                nBP += pTBRandomBox->RB_BP;
                            }
                            else if (m == 2)
                            {
                                nEther += pTBRandomBox->RB_Ether;
                            }
                            break;
                        }
                        
                        // Item rewards (slots 3-17 map to items 0-14)
                        unsigned char byItemPos = m - 3;
                        int nRewardItemID = *(&pTBRandomBox->Item_01 + byItemPos);
                        short shRewardCount = *(&pTBRandomBox->Count_01 + byItemPos);
                        
                        if (nRewardItemID && shRewardCount)
                        {
                            TB_ITEM* pTBBoxItem = pServer->m_xResourceMgr.GetTB_ITEM(nRewardItemID);
                            if (pTBBoxItem)
                            {
                                bGetItem[m] = true;
                                nMaxRate -= *pRate;
                                
                                // Handle currency items
                                if (pTBBoxItem->Item_ID == 0x2FAF0801) // Zenny
                                {
                                    nZenny += shRewardCount;
                                }
                                else if (pTBBoxItem->Item_ID == 0x2FAF0803) // BP
                                {
                                    nBP += shRewardCount;
                                }
                                else if (pTBBoxItem->Item_ID == 0x2FAF0804) // Ether
                                {
                                    nEther += shRewardCount;
                                }
                                else
                                {
                                    // Regular item
                                    ST_CREATE_ITEM stAddItem;
                                    stAddItem.nItemID = nRewardItemID;
                                    stAddItem.shCount = shRewardCount;
                                    stAddItemList.push_back(stAddItem);
                                    stGetInfo.stGetItem.push_back(stAddItem);
                                }
                                break;
                            }
                        }
                    }
                    else
                    {
                        nRate -= *pRate;
                    }
                }
            }
        }
    }
    
    // Tool mode - just store results
    if (bTool)
    {
        for (size_t n = 0; n < stAddItemList.size(); ++n)
        {
            m_stToolRandomBoxRes.push_back(stAddItemList[n]);
        }
        return true;
    }
    
    // Add currency rewards
    if (nBP > 0)
    {
        if (!CheckOverMoney(E_PRICE_TYPE_BP, nBP))
        {
            CUser* pUser = GetUser();
            if (pUser)
            {
                CGocNetwork::SendErrorMessage(pUser, 8, 0x11, 0xCB59);
            }
            return false;
        }
        AddBP(nBP, 0x1D);
        stGetInfo.nBP = nBP;
    }
    
    if (nZenny > 0)
    {
        if (!CheckOverMoney(E_PRICE_TYPE_GOLD, nZenny))
        {
            CUser* pUser = GetUser();
            if (pUser)
            {
                CGocNetwork::SendErrorMessage(pUser, 8, 0x11, 0xCB58);
            }
            return false;
        }
        AddMoney(nZenny, 0x1D, nItemID, 0, 0);
        stGetInfo.nMoney = nZenny;
    }
    
    if (nEther > 0)
    {
        if (!CheckOverMoney(E_PRICE_TYPE_ETHER, nEther))
        {
            CUser* pUser = GetUser();
            if (pUser)
            {
                CGocNetwork::SendErrorMessage(pUser, 8, 0x11, 0xCB5A);
            }
            return false;
        }
        AddEther(nEther, 0x1D, 1);
        stGetInfo.nEther = nEther;
    }
    
    // Create items and send to database
    PS_RES_STORAGE_INFO vecCreateItem;
    PS_RES_STORAGE_INFO vecUpdateItem;
    
    if (CreateItem2(&stAddItemList, 0x16, 1, &vecCreateItem, &vecUpdateItem, nullptr))
    {
        // Send DB packet
        XSendDBPacket xSendDBPacket(GetUser(), 0x21, 0x12);
        xSendDBPacket << GetQuestID();
        xSendDBPacket << vecUpdateItem;
        xSendDBPacket << vecCreateItem;
        xSendDBPacket << stGetInfo;
        xSendDBPacket << nItemID;
        xSendDBPacket << 0;
        xSendDBPacket << PS_GACHA_LIST();
        xSendDBPacket << (unsigned char)23;
        xSendDBPacket << biItemSerial;
        
        pServer->SendDBGame(&xSendDBPacket);
        return true;
    }
    
    return false;
}

// ============================================================================
// Gacha System
// ============================================================================

bool CGocInventory::UseGacha(unsigned short dwGetID, int nCount,
                              PS_GACHA_LIST* stGachaList, ST_GET_INFO* stGetInfo,
                              PS_RES_STORAGE_INFO* vecCreateItem,
                              PS_RES_STORAGE_INFO* vecUpdateItem, bool bTool)
{
    // Address: 0x1400BAAD0
    // Gacha pull system with random group selection
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    
    // Get random group
    TB_RANDOM_GET_GROUP* pTB_RandomGroup = pServer->m_xResourceMgr.GetTB_RANDOM_GET_GROUP(dwGetID);
    if (!pTB_RandomGroup)
    {
        return false;
    }
    
    // Get gacha group
    int nGroupID = pTB_RandomGroup->Gacha_Group_ID;
    TB_GACHA_GROUP* pTBGachaGroup = pServer->m_xResourceMgr.GetGachaGroupTable(nGroupID);
    
    if (!pTBGachaGroup)
    {
        LogHelper::LogError("game.contents", "[GACHA] No TB_GACHA_GROUP Table %d", nGroupID);
        return false;
    }
    
    // Random group selection (20 tiers)
    int nRandomGroupRate = pServer->m_xItemFactory.nRand(1, 10000);
    unsigned int dwIndex = 0;
    
    for (int i = 0; i < 20; ++i)
    {
        nRandomGroupRate -= *(&pTBGachaGroup->G_Chance_01 + i);
        if (nRandomGroupRate <= 0)
        {
            dwIndex = *(&pTB_RandomGroup->Group_ID_01 + i);
            break;
        }
    }
    
    // Get random get table
    TB_RANDOM_GET* pTB_RandomGet = pServer->m_xResourceMgr.GetTB_RANDOM_GET_With_Lock(dwIndex);
    if (!pTB_RandomGet)
    {
        return false;
    }
    
    // Random item selection (30 items per tier)
    int nRandomGetRate = pServer->m_xItemFactory.nRand(1, 10000);
    
    for (int k = 0; k < 30; ++k)
    {
        nRandomGetRate -= *(&pTB_RandomGet->I_Chance_01 + k);
        if (nRandomGetRate <= 0)
        {
            // Get item
            int nItemID = *(&pTB_RandomGet->Item_ID_01 + k);
            TB_ITEM* tbItem = pServer->m_xResourceMgr.GetTB_ITEM(nItemID);
            
            if (!tbItem)
            {
                return false;
            }
            
            // Create gacha info
            PS_GACHA_INFO stGachaInfo;
            stGachaInfo.nItemID = nItemID;
            stGachaInfo.nType = *(&pTB_RandomGet->Item_Get_type_01 + k);
            
            ST_CREATE_ITEM stAddItem;
            stAddItem.nItemID = nItemID;
            stAddItem.shCount = 1;
            
            stGetInfo->stGetItem.push_back(stAddItem);
            stGachaList->push_back(stGachaInfo);
            
            // Tool mode
            if (bTool)
            {
                PS_TOOL_ITEM_INFO psToolInfo;
                psToolInfo.nItemID = nItemID;
                psToolInfo.nCount = 1;
                psToolInfo.nValue1 = pTBGachaGroup->Gacha_Chance_ID;
                psToolInfo.nValue2 = pTB_RandomGet->Random_Get_ID;
                m_stToolItemInfo.vecInfo.push_back(psToolInfo);
                return true;
            }
            
            // Reduce source item
            ST_CREATE_ITEMS stReduceItems;
            ST_CREATE_ITEM stReduce;
            stReduce.nItemID = pTB_RandomGroup->Group_Des;
            stReduce.shCount = 1;
            stReduceItems.push_back(stReduce);
            
            if (!ReduceItem2(&stReduceItems, 0x1C, vecUpdateItem))
            {
                return false;
            }
            
            // Add gacha item
            if (!AddItem2(tbItem, 1, 0x1C, 0, vecCreateItem, vecUpdateItem))
            {
                return false;
            }
            
            return true;
        }
    }
    
    return false;
}

// ============================================================================
// Package Box System
// ============================================================================

bool CGocInventory::CanPackageBoxUse(std::tr1::shared_ptr<CItem> pItem)
{
    // Address: 0x1400B2CA0
    // Check if package box can be used
    
    if (!pItem || pItem->GetItemTable() == nullptr)
    {
        return false;
    }
    
    TB_ITEM* pTBItem = pItem->GetItemTable();
    if (pTBItem->Item_Effect_Type != 5) // Package Box type
    {
        return false;
    }
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM_PACKAGE* pTBPackage = pServer->m_xResourceMgr.GetTB_ITEM_PACKAGE(pTBItem->Item_Effect_ID);
    
    if (!pTBPackage)
    {
        return false;
    }
    
    return true;
}

bool CGocInventory::PackageBoxUse(bool bReduceItem, std::tr1::shared_ptr<CItem> pItem,
                                   unsigned char byCount, int nItemIDparClass)
{
    // Address: 0x1400B2D80
    // Package box opening with fixed item sets
    
    // Validate count
    if (!byCount || byCount > 10)
    {
        CUser* pUser = GetUser();
        if (pUser)
        {
            CGocNetwork::SendErrorMessage(pUser, 8, 0x11, 0xCB2B);
        }
        LogHelper::LogError("game.item", "PackageBoxUse error - Invalid count: %d", byCount);
        return false;
    }
    
    // Get item ID
    int nItemID = pItem->GetItemTable()->Item_ID;
    if (pItem->GetItemTable()->Item_Effect_Type == 7)
    {
        nItemID = nItemIDparClass;
    }
    
    unsigned __int64 biItemSerial = pItem->GetSerial();
    
    // Get package table
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM_PACKAGE* pTBPackage = pServer->m_xResourceMgr.GetTB_ITEM_PACKAGE(nItemID);
    
    if (!pTBPackage)
    {
        CUser* pUser = GetUser();
        if (pUser)
        {
            CGocNetwork::SendErrorMessage(pUser, 8, 0x11, 0xCB2B);
        }
        LogHelper::LogError("game.item", "PackageBoxUse error - No TB_ITEM_PACKAGE table");
        return false;
    }
    
    // Collect all items from package
    ST_GET_INFO stGetInfo;
    ST_CREATE_ITEMS stCreateItemList;
    
    for (int k = 0; k < byCount; ++k)
    {
        for (int i = 0; i < 15; ++i)
        {
            int nRewardItemID = *(&pTBPackage->Item_01 + i);
            short shRewardCount = *(&pTBPackage->Count_01 + i);
            
            if (nRewardItemID && shRewardCount)
            {
                TB_ITEM* pTB_Item = pServer->m_xResourceMgr.GetTB_ITEM(nRewardItemID);
                if (!pTB_Item)
                {
                    return false;
                }
                
                ST_CREATE_ITEM stAddItem;
                stAddItem.nItemID = nRewardItemID;
                
                // Check if upgrade item
                if (*(&pTBPackage->Unpacking_Function_Type_01 + i) == 1)
                {
                    stAddItem.shCount = 1;
                    stAddItem.byUpgrade = (unsigned char)shRewardCount;
                }
                else
                {
                    stAddItem.shCount = shRewardCount;
                    stAddItem.byUpgrade = 0;
                }
                
                stCreateItemList.push_back(stAddItem);
                stGetInfo.stGetItem.push_back(stAddItem);
            }
        }
    }
    
    // Create items
    PS_RES_STORAGE_INFO vecCreateItem;
    PS_RES_STORAGE_INFO vecUpdateItem;
    
    // Add items
    for (size_t j = 0; j < stCreateItemList.size(); ++j)
    {
        ST_CREATE_ITEM& stPackageCreate = stCreateItemList[j];
        TB_ITEM* pTBItem = pServer->m_xResourceMgr.GetTB_ITEM(stPackageCreate.nItemID);
        
        if (!pTBItem)
        {
            return false;
        }
        
        if (!AddItemUpgradeCount(pTBItem, stPackageCreate.shCount, 
                                  stPackageCreate.byUpgrade, 0x15, 1,
                                  &vecCreateItem, &vecUpdateItem))
        {
            return false;
        }
    }
    
    // Add currency rewards
    int nGold = byCount * pTBPackage->P_Gold;
    int nBP = byCount * pTBPackage->P_BP;
    int nEther = byCount * pTBPackage->P_Ether;
    
    stGetInfo.nMoney = nGold;
    stGetInfo.nBP = nBP;
    stGetInfo.nEther = nEther;
    
    // Send DB packet
    XSendDBPacket xSendDBPacket(GetUser(), 0x21, 0x12);
    xSendDBPacket << GetQuestID();
    xSendDBPacket << vecUpdateItem;
    xSendDBPacket << vecCreateItem;
    xSendDBPacket << stGetInfo;
    xSendDBPacket << nItemID;
    xSendDBPacket << 0;
    xSendDBPacket << PS_GACHA_LIST();
    xSendDBPacket << (unsigned char)0;
    xSendDBPacket << biItemSerial;
    
    pServer->SendDBGame(&xSendDBPacket);
    
    AddMoney(nGold, 0x1C, nItemID, 0, 0);
    AddBP(nBP, 0x1C);
    AddEther(nEther, 0x1C, 1);
    
    return true;
}

// ============================================================================
// Item Count Box System
// ============================================================================

bool CGocInventory::CanUseItemCountBox(std::tr1::shared_ptr<CItem> pItem)
{
    // Address: 0x1400DA370
    // Check if item count box can be used
    
    if (!pItem || pItem->GetItemTable() == nullptr)
    {
        return false;
    }
    
    TB_ITEM* pTBItem = pItem->GetItemTable();
    if (pTBItem->Item_Effect_Type != 8) // Item Count Box type
    {
        return false;
    }
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM_COUNTBOX* pTB_Countbox = pServer->m_xResourceMgr.GetTB_ITEM_COUNTBOX(pTBItem->Item_Effect_ID);
    
    if (!pTB_Countbox)
    {
        return false;
    }
    
    return true;
}

bool CGocInventory::UseItemCountBox(bool bReduceItem, std::tr1::shared_ptr<CItem> pItem)
{
    // Address: 0x1400DA570
    // Item count box system - progressive item rewards
    
    unsigned int dwUCID = GetQuestID();
    
    if (!pItem)
    {
        LogHelper::LogError("game.item", "UseItemCountBox error - Item is NULL[UCID:%d]", dwUCID);
        return false;
    }
    
    TB_ITEM* pTBItem = pItem->GetItemTable();
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    
    TB_ITEM_COUNTBOX* pTB_Countbox = pServer->m_xResourceMgr.GetTB_ITEM_COUNTBOX(pTBItem->Item_Effect_ID);
    if (!pTB_Countbox)
    {
        LogHelper::LogError("game.item", "UseItemCountBox error - No TB_ITEM_COUNTBOX[UCID:%d]", dwUCID);
        return false;
    }
    
    // Get current use count
    unsigned char byUseCount = pItem->GetUseCount();
    if (byUseCount + 1 > pTB_Countbox->Usage_Cnt)
    {
        LogHelper::LogError("game.item", "UseItemCountBox error - Over use count[UCID:%d]", dwUCID);
        return false;
    }
    
    // Get group for current usage
    unsigned short wGroupID = *(&pTB_Countbox->Usage_Group_1_ID + byUseCount);
    TB_ITEM_COUNTBOX_GROUP* pTB_Countbox_Group = pServer->m_xResourceMgr.GetTB_ITEM_COUNTBOX_GROUP(wGroupID);
    
    if (!pTB_Countbox_Group)
    {
        LogHelper::LogError("game.item", "UseItemCountBox error - No TB_ITEM_COUNTBOX_GROUP[UCID:%d]", dwUCID);
        return false;
    }
    
    // Collect items from group
    ST_GET_INFO stGetInfo;
    ST_CREATE_ITEMS stCreateItems;
    
    for (int i = 0; i < 15; ++i)
    {
        unsigned int dwItemID = *(&pTB_Countbox_Group->Item_1 + i);
        unsigned short shItemCount = *(&pTB_Countbox_Group->Item_Cnt_1 + i);
        
        if (!dwItemID || !shItemCount)
        {
            break;
        }
        
        ST_CREATE_ITEM stCreateItem;
        stCreateItem.nItemID = dwItemID;
        stCreateItem.shCount = shItemCount;
        
        stGetInfo.stGetItem.push_back(stCreateItem);
        stCreateItems.push_back(stCreateItem);
    }
    
    // Increment use count
    ++byUseCount;
    pItem->SetUseCount(byUseCount);
    
    // Create items
    PS_RES_STORAGE_INFO psCreateItemList;
    PS_RES_STORAGE_INFO psUpdateItemList;
    
    if (!CreateItem2(&stCreateItems, 0x7E, 0, &psCreateItemList, &psUpdateItemList, nullptr))
    {
        return false;
    }
    
    // Send DB packet
    PS_DB_ITEM_COUNTBOX psDBUpdateItem;
    psDBUpdateItem.dwUCID = dwUCID;
    psDBUpdateItem.psUpdateItemList = psUpdateItemList;
    psDBUpdateItem.psCreateItemList = psCreateItemList;
    psDBUpdateItem.stGetInfo = stGetInfo;
    psDBUpdateItem.dwUseItemID = pItem->GetItemTable()->Item_ID;
    psDBUpdateItem.biUseItemSerial = pItem->GetSerial();
    
    XSendDBPacket xSendDBPacket(GetUser(), 0x21, 0x48);
    xSendDBPacket << psDBUpdateItem;
    pServer->SendDBGame(&xSendDBPacket);
    
    return true;
}

// ============================================================================
// Random Option System
// ============================================================================

bool CGocInventory::CheckRandomOption(STItem* stOptinChangeInfo)
{
    // Address: 0x1400DC0C0
    // Random option validation
    
    if (!stOptinChangeInfo)
    {
        return false;
    }
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pTB_Item = pServer->m_xResourceMgr.GetTB_ITEM(stOptinChangeInfo->nItemID);
    
    if (!pTB_Item)
    {
        return false;
    }
    
    TB_ITEM_CLASSIFY* pTB_ItemClassify = pServer->m_xResourceMgr.GetTB_ITEM_CLASSIFY(pTB_Item->Item_Classify_Index);
    if (!pTB_ItemClassify)
    {
        return false;
    }
    
    // Check if item use type is random option
    if (pTB_ItemClassify->Item_Use_Type == 51)
    {
        return false;
    }
    
    // Collect existing options
    std::vector<ST_EXTEND_OPTION> vecTemp;
    bool bOption = false;
    
    for (int i = 0; i < 5; ++i)
    {
        if (stOptinChangeInfo->stExtendOption[i].byType)
        {
            bOption = true;
            vecTemp.push_back(stOptinChangeInfo->stExtendOption[i]);
        }
    }
    
    // Reorder options
    if (bOption)
    {
        memset(stOptinChangeInfo->stExtendOption, 0, sizeof(stOptinChangeInfo->stExtendOption));
        for (size_t j = 0; j < vecTemp.size(); ++j)
        {
            stOptinChangeInfo->stExtendOption[j] = vecTemp[j];
        }
    }
    
    return bOption;
}

bool CGocInventory::IsRandomItemTitle(int nItemID)
{
    // Address: 0x1400DCA20
    // Check if item has random title
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pTB_Item = pServer->m_xResourceMgr.GetTB_ITEM(nItemID);
    
    if (!pTB_Item)
    {
        return false;
    }
    
    // Check for random title flag
    return (pTB_Item->Item_Title_Type != 0);
}

// ============================================================================
// Vaccum Cube Random Functions
// ============================================================================

int CVaccumGroup::GetRandomValue()
{
    // Address: 0x140191A90
    // Generate random value 1-10000
    
    XWorldManager* pWorldMgr = TXSingleton<XWorldManager>::Instance();
    return pWorldMgr->nRand(1, 10000);
}

int CVaccumCube::GetRandomKey()
{
    // Address: 0x140194520
    // Get stored random key
    
    return m_nRandomKey;
}

void CVaccumCube::SetRandomKey(int nKey)
{
    // Address: 0x1401945F0
    // Set random key
    
    m_nRandomKey = nKey;
}
