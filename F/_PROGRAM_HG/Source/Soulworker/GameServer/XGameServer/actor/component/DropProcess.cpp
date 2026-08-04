#include "DropProcess.h"
#include "User.h"
#include "XPacket.h"
#include "XSendPacket.h"
#include "XSendDBPacket.h"
#include "XItemFactory.h"
#include "GocNetwork.h"
#include "GocInventory.h"
#include "GocParty.h"
#include "GocRecode.h"
#include "GocBooster.h"
#include "GocAttribute.h"
#include "GocQuest.h"
#include "CLogicThreadManager.h"
#include "XGameServer.h"
#include "XWorldManager.h"
#include "XWorldResMgr.h"
#include "TB_DROPRATE_MOB.h"
#include "TB_DROPRATE_LEVEL.h"
#include "TB_DROPRATE_MEMBER.h"
#include "TB_DROP.h"
#include "TB_DROP_GROUP.h"
#include "TB_ITEM.h"
#include "TB_MONSTER.h"
#include "TB_MAZE_INFO.h"
#include "PS_REQ_PICKUP.h"
#include "PS_RES_PICKUP.h"
#include "PS_DROP_INFOS.h"
#include "PS_TOOL_MAZE_DROP.h"
#include "ST_DROP_ITEM_INFO.h"
#include "ST_LOG_GAME.h"
#include "LogHelper.h"
#include "VEventObjectResource.h"
#include "VisObject3D_cl.h"

// CDropItemGroup implementation

CDropItemGroup::CDropItemGroup()
    : m_dwDisappearTime(0)
{
}

CDropItemGroup::CDropItemGroup(PS_DROP_INFOS& stDropInfos)
    : m_dwDisappearTime(GetTickCount64() + DROP_ITEM_DISAPPEAR_TIME)
{
    // Copy all drop items from the vector to the map
    for (auto it = stDropInfos.vecInfo.begin(); it != stDropInfos.vecInfo.end(); ++it)
    {
        m_mapDropItem.insert(std::make_pair(it->xSerial, *it));
    }
}

CDropItemGroup::~CDropItemGroup()
{
}

bool CDropItemGroup::IsDisappeared(DWORD dwCurrentTime) const
{
    return dwCurrentTime >= m_dwDisappearTime;
}

// CDropProcess implementation

CDropProcess::CDropProcess()
    : TXProcess<CUser>()
    , m_nGroupID(0)
{
    // Set command to 0x14
    SetCmd(0x14);
    
    // Set component name
    SetName("CDropProcess");
    
    // Call Init
    Init();
}

CDropProcess::~CDropProcess()
{
    Clear();
}

void CDropProcess::Init()
{
    // Clear drop item map
    m_mapDropInfo.clear();
    m_nGroupID = 0;
}

bool CDropProcess::Clear()
{
    // Clear drop item map
    m_mapDropInfo.clear();
    
    // Clear tool drop item list
    m_listToolDropItem.clear();
    
    return true;
}

bool CDropProcess::Parse(XPacket& packet)
{
    // Parse packet command
    BYTE ucMainCmd = 0;
    BYTE ucSubCmd = 0;
    
    packet >> ucMainCmd >> ucSubCmd;
    
    switch (ucSubCmd)
    {
    case 1: // Pickup request
        return ReqItemPickUp(packet);
        break;
        
    default:
        break;
    }
    
    return true;
}

bool CDropProcess::ReqItemPickUp(XPacket& packet)
{
    // Read pickup request packet
    PS_REQ_PICKUP reqPickup;
    packet >> reqPickup;
    
    // Get user pointer
    CUser* pUser = GetClientPtr();
    if (!pUser)
    {
        return false;
    }
    
    // Create lambda for async pickup processing
    auto func = [this, pUser, reqPickup]() 
    {
        // TODO: Implement pickup logic
        // 1. Check if item exists in m_mapDropInfo
        // 2. Check pickup distance
        // 3. Check inventory space
        // 4. Add item to inventory
        // 5. Remove from m_mapDropInfo
        // 6. Send response to client
        
        // For now, send success response
        __int64 xSerial = reqPickup.xSerial;
        int nResult = 0; // Success
        
        SendResPickup(pUser, xSerial, nResult);
    };
    
    // Increment job count for async processing
    pUser->IncrementJobCount();
    
    // Get map instance ID
    TUXMapID mapID = pUser->GetMapInsID();
    
    // Get party pointer (for loot distribution)
    std::tr1::shared_ptr<CParty> pParty = pUser->GetParty();
    
    // Post job to logic thread manager
    CLogicThreadManager::Instance()->DoJob(mapID, func);
    
    // Post decrement job count
    auto decFunc = [pUser]() 
    {
        pUser->DecrementJobCount();
    };
    
    CLogicThreadManager::Instance()->DoJob(mapID, decFunc);
    
    return true;
}

void CDropProcess::SendResPickup(CUser* pUser, __int64 xSerial, int nResult)
{
    // Create response packet
    PS_RES_PICKUP stPickup;
    stPickup.xSerial = xSerial;
    stPickup.nResult = nResult;
    
    // Create send packet (main cmd 0x14, sub cmd 0x02)
    XSendPacket xSendPacket(0x14, 0x02);
    xSendPacket << stPickup;
    
    // Send to user
    XActor* pActor = pUser ? pUser->GetActorPtr() : nullptr;
    CGocNetwork::Send(pActor, xSendPacket);
}

void CDropProcess::UpdateDropItem()
{
    // Get current time
    DWORD dwCurrentTime = GetTickCount();
    
    // Iterate through drop items and remove expired ones
    auto it = m_mapDropInfo.begin();
    while (it != m_mapDropInfo.end())
    {
        std::tr1::shared_ptr<CDropItemGroup> pDropGroup = it->second;
        if (pDropGroup && pDropGroup->IsDisappeared(dwCurrentTime))
        {
            // Remove expired item
            it = m_mapDropInfo.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void CDropProcess::ClearToolDropItem()
{
    m_listToolDropItem.clear();
}

bool CDropProcess::IsPickupDropItem(XVec3 vPos, int nObjectID, float& fDist)
{
    fDist = 0.0f;
    
    // Get user pointer
    CUser* pUser = GetClientPtr();
    if (!pUser)
    {
        return false;
    }
    
    // Get player position
    XVec3 playerPos = VisObject3D_cl::GetPosition(&pUser->CMoverEx);
    
    // Calculate distance
    fDist = playerPos.getDistanceTo(vPos);
    
    // Special item IDs have extended pickup distance
    // Items 800000001-800000004 have special pickup distance
    if (nObjectID == 800000001)
    {
        return fDist <= 1700.0f;
    }
    else if (nObjectID > 800000001 && nObjectID <= 800000004)
    {
        return fDist <= 1700.0f;
    }
    
    // Normal pickup distance
    return fDist <= 1200.0f;
}

bool CDropProcess::MakeDropItems(CUser* pUser, int nDropID, XVec3& vPos, float fDropRate, bool bBoss, int nMonsterID, int nMapID, int nClass, float fTooldropBuff)
{
    // Validate user if provided
    if (pUser)
    {
        if (!pUser->GetArea())
            return false;
    }
    
    // Get drop table
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DROP* pTB_DROP = XResourceMgr::GetTB_DROP_With_Lock(&pGameServer->m_xResourceMgr, nDropID);
    if (!pTB_DROP)
        return false;
    
    // Get indulgence drop rate
    float fIndulgenceRate = 1.0f;
    if (pUser)
    {
        std::tr1::shared_ptr<CGocRecode> pRecode;
        CMover::GetGOC<CGocRecode>(&pUser->CMoverEx, pRecode, false);
        if (pRecode)
        {
            fIndulgenceRate = pRecode->GetIndulgenceDropRate();
        }
    }
    
    ST_DROP_ITEM_INFO stDropItem;
    PS_DROP_INFOS psDropInfos;
    psDropInfos.bBossDrop = bBoss;
    
    if (fDropRate > 0.0f)
    {
        // Gold drop
        int nDropRateRand = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, 10000);
        float fGoldChance = (float)pTB_DROP->Gold_Chance * fDropRate * fIndulgenceRate;
        int nGoldSum = 0;
        
        if (fGoldChance >= (float)nDropRateRand)
        {
            int nJpMulti = 1;
            if (pTB_DROP->Gold_Jp_Rate && pTB_DROP->Gold_Jp_Multi)
            {
                int nJpRate = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, 10000);
                if (nJpRate <= pTB_DROP->Gold_Jp_Rate)
                {
                    nJpMulti = pTB_DROP->Gold_Jp_Multi;
                    psDropInfos.nJpID = 800000001;
                    psDropInfos.byJpMulti = nJpMulti;
                }
            }
            
            float fRate = 0.0f;
            if (pUser)
            {
                std::tr1::shared_ptr<CGocAttribute> pAttr;
                std::tr1::shared_ptr<CGocBooster> pBoosterAttr;
                CMover::GetGOC<CGocAttribute>(&pUser->CMoverEx, pAttr, false);
                CMover::GetGOC<CGocBooster>(&pUser->CMoverEx, pBoosterAttr, false);
                
                if (pAttr)
                    fRate = pAttr->GetSpecialEffect(116) * 0.01f;
                if (pBoosterAttr)
                    fRate += pBoosterAttr->GetTotalRate(eBooster_Effect_IncGoldDrop);
                fRate += pUser->CMoverEx.GetBuffAddGoldRate();
            }
            
            for (int i = 0; i < nJpMulti; ++i)
            {
                UXSerial serial;
                XItemFactory::GeneratSerial(&pGameServer->m_xItemFactory, serial);
                stDropItem.xSerial = serial.xSerial;
                stDropItem.nObjectID = 800000001;
                
                int nMin = pTB_DROP->Gold_Min;
                int nMax = pTB_DROP->Gold_Max;
                int nBase = XItemFactory::nRand(&pGameServer->m_xItemFactory, nMin, nMax);
                int nBonus = (int)((float)nBase * fRate);
                stDropItem.nBonus = nBonus;
                stDropItem.nCount = nBonus + nBase;
                stDropItem.xPos = vPos;
                
                psDropInfos.vecInfo.push_back(stDropItem);
                nGoldSum += stDropItem.nCount;
            }
        }
        
        // BP drop
        nDropRateRand = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, 10000);
        float fBPChance = (float)pTB_DROP->BP_Chance * fDropRate * fIndulgenceRate;
        
        if (pUser)
        {
            std::tr1::shared_ptr<CGocBooster> pBooster;
            CMover::GetGOC<CGocBooster>(&pUser->CMoverEx, pBooster, false);
            if (pBooster)
                fBPChance = pBooster->GetEffectValue(eBooster_Effect_IncBattlePoint, fBPChance);
        }
        
        if (fBPChance >= (float)nDropRateRand)
        {
            int nGetBP = XItemFactory::nRand(&pGameServer->m_xItemFactory, pTB_DROP->BP_Min, pTB_DROP->BP_Max);
            
            if (pUser)
            {
                std::tr1::shared_ptr<CGocInventory> pInven;
                CMover::GetGOC<CGocInventory>(&pUser->CMoverEx, pInven, false);
                if (pInven)
                {
                    XOption* pOption = pGameServer->GetOption();
                    if (pOption && pOption->GetNationType() == 2)
                    {
                        pInven->AddBP(nGetBP, 0x1A);
                    }
                    else
                    {
                        std::wstring strTempName;
                        pInven->AddLimitBP(nGetBP, strTempName, 0x1A);
                    }
                }
            }
            else
            {
                stDropItem.nObjectID = 800000003;
                stDropItem.nCount = nGetBP;
                psDropInfos.vecInfo.push_back(stDropItem);
            }
        }
        
        // Soul Value (SV) drop
        nDropRateRand = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, 10000);
        float fSoulChance = (float)pTB_DROP->SV_Chance * fDropRate * fIndulgenceRate;
        
        if (fSoulChance >= (float)nDropRateRand)
        {
            float fSVAddRat = 0.0f;
            if (pUser)
            {
                std::tr1::shared_ptr<CGocAttribute> pAttr;
                CMover::GetGOC<CGocAttribute>(&pUser->CMoverEx, pAttr, false);
                if (pAttr)
                    fSVAddRat = pAttr->GetSpecialEffect(117);
            }
            
            int wSVMin = (int)((float)pTB_DROP->SV_Min + (float)pTB_DROP->SV_Min * fSVAddRat * 0.01f);
            int wSVMax = (int)((float)pTB_DROP->SV_Max + (float)pTB_DROP->SV_Max * fSVAddRat * 0.01f);
            
            UXSerial serial;
            XItemFactory::GeneratSerial(&pGameServer->m_xItemFactory, serial);
            stDropItem.xSerial = serial.xSerial;
            stDropItem.nObjectID = 800000002;
            stDropItem.nCount = XItemFactory::nRand(&pGameServer->m_xItemFactory, wSVMin, wSVMax);
            stDropItem.xPos = vPos;
            stDropItem.nBonus = 0;
            
            psDropInfos.vecInfo.push_back(stDropItem);
            nGoldSum += stDropItem.nCount;
        }
        
        // Ether drop
        nDropRateRand = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, 10000);
        float fEtherChance = (float)pTB_DROP->Et_Chance * fDropRate * fIndulgenceRate;
        
        if (pUser)
        {
            std::tr1::shared_ptr<CGocBooster> pBooster;
            CMover::GetGOC<CGocBooster>(&pUser->CMoverEx, pBooster, false);
            if (pBooster)
                fEtherChance = pBooster->GetEffectValue(eBooster_Effect_IncEtherDrop, fEtherChance);
        }
        
        if (pTB_DROP->Et_Unit && fEtherChance >= (float)nDropRateRand)
        {
            int nCount = pTB_DROP->Et_Max / pTB_DROP->Et_Unit;
            if (pTB_DROP->Et_Max % pTB_DROP->Et_Unit)
                ++nCount;
            if (nCount <= 0)
                nCount = 1;
            
            int nMinRate = pTB_DROP->Et_Min / nCount;
            int Et_Jp_Multi = 1;
            
            if (pTB_DROP->Et_Jp_Rate && pTB_DROP->Et_Jp_Multi && !psDropInfos.nJpID)
            {
                int nJpRate = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, 10000);
                if (nJpRate <= pTB_DROP->Et_Jp_Rate)
                {
                    Et_Jp_Multi = pTB_DROP->Et_Jp_Multi;
                    psDropInfos.nJpID = 800000004;
                    psDropInfos.byJpMulti = Et_Jp_Multi;
                }
            }
            
            for (int j = 0; j < Et_Jp_Multi; ++j)
            {
                int nTotalCount = 0;
                for (int k = 0; k < nCount && nTotalCount < pTB_DROP->Et_Max; ++k)
                {
                    int nGiveCount = XItemFactory::nRand(&pGameServer->m_xItemFactory, nMinRate, pTB_DROP->Et_Unit);
                    nTotalCount += nGiveCount;
                    int nGap = 0;
                    if (nTotalCount > pTB_DROP->Et_Max)
                        nGap = nTotalCount - pTB_DROP->Et_Max;
                    
                    UXSerial serial;
                    XItemFactory::GeneratSerial(&pGameServer->m_xItemFactory, serial);
                    stDropItem.xSerial = serial.xSerial;
                    stDropItem.nObjectID = 800000004;
                    stDropItem.nCount = nGiveCount - nGap;
                    stDropItem.xPos = vPos;
                    stDropItem.nBonus = 0;
                    
                    psDropInfos.vecInfo.push_back(stDropItem);
                    nGoldSum += stDropItem.nCount;
                }
                
                if (nTotalCount < pTB_DROP->Et_Min)
                {
                    int nGap = pTB_DROP->Et_Min - nTotalCount;
                    UXSerial serial;
                    XItemFactory::GeneratSerial(&pGameServer->m_xItemFactory, serial);
                    stDropItem.xSerial = serial.xSerial;
                    stDropItem.nObjectID = 800000004;
                    stDropItem.nCount = nGap;
                    stDropItem.xPos = vPos;
                    stDropItem.nBonus = 0;
                    
                    psDropInfos.vecInfo.push_back(stDropItem);
                    nGoldSum += stDropItem.nCount;
                }
            }
        }
        
        // Group drops (21 groups)
        for (int m = 0; m < 21; ++m)
        {
            int nGroupDropRate = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, 10000);
            float fGroupChance = (float)(&pTB_DROP->G_Chance_01)[m] * fDropRate * fIndulgenceRate;
            
            if (pUser)
            {
                std::tr1::shared_ptr<CGocBooster> pBooster;
                CMover::GetGOC<CGocBooster>(&pUser->CMoverEx, pBooster, false);
                if (pBooster)
                    fGroupChance = pBooster->GetEffectValue(eBooster_Effect_IncItemDrop, fGroupChance);
            }
            else if (fTooldropBuff != 0.0f)
            {
                fGroupChance += fGroupChance * fTooldropBuff;
            }
            
            if (fGroupChance >= (float)nGroupDropRate)
            {
                TB_DROP_GROUP* pTB_DROP_GROUP = XResourceMgr::GetTB_DROP_GROUP_With_Lock(&pGameServer->m_xResourceMgr, (&pTB_DROP->Group_ID_01)[m]);
                if (pTB_DROP_GROUP)
                {
                    int nItemRand = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, 10000);
                    for (int n = 0; n < 10; ++n)
                    {
                        nItemRand -= (&pTB_DROP_GROUP->I_Chance_01)[n];
                        if (nItemRand <= 0)
                        {
                            UXSerial serial;
                            XItemFactory::GeneratSerial(&pGameServer->m_xItemFactory, serial);
                            stDropItem.xSerial = serial.xSerial;
                            stDropItem.nObjectID = (&pTB_DROP_GROUP->Item_ID_01)[n];
                            
                            int nRandomCount = XItemFactory::nRand(&pGameServer->m_xItemFactory, (&pTB_DROP_GROUP->Item_Min_Cnt_01)[n], (&pTB_DROP_GROUP->Item_Max_Cnt_01)[n]);
                            if (nRandomCount > 0)
                            {
                                TB_ITEM* pDropItem = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, stDropItem.nObjectID);
                                if (pDropItem && pDropItem->Item_Stack_Max < nRandomCount)
                                    nRandomCount = pDropItem->Item_Stack_Max;
                            }
                            
                            stDropItem.nCount = nRandomCount;
                            stDropItem.xPos = vPos;
                            stDropItem.nBonus = 0;
                            
                            if (stDropItem.nCount > 0)
                            {
                                psDropInfos.vecInfo.push_back(stDropItem);
                            }
                            break;
                        }
                    }
                }
            }
        }
        
        // Character-specific drops (10 groups)
        for (int ii = 0; ii < 10; ++ii)
        {
            int nCharRand = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, 10000);
            float fOrigin = (float)(&pTB_DROP->C_Chance_01)[ii] * fDropRate;
            
            if (pUser)
            {
                std::tr1::shared_ptr<CGocBooster> pBooster;
                CMover::GetGOC<CGocBooster>(&pUser->CMoverEx, pBooster, false);
                if (pBooster)
                    fOrigin = pBooster->GetEffectValue(eBooster_Effect_IncItemDrop, fOrigin);
            }
            else if (fTooldropBuff != 0.0f)
            {
                fOrigin += fOrigin * fTooldropBuff;
            }
            
            if (fOrigin >= (float)nCharRand)
            {
                TB_DROP_GROUP_CHARACTER* pTB_DROP_GROUP_CHAR = XResourceMgr::GetTB_DROP_GROUP_CHARACTER_With_Lock(&pGameServer->m_xResourceMgr, (&pTB_DROP->C_Group_ID_01)[ii]);
                if (pTB_DROP_GROUP_CHAR)
                {
                    int nOpenCharCount = XResourceMgr::GetOpenCharacterCount(&pGameServer->m_xResourceMgr);
                    int nDefaultRate = 100 / nOpenCharCount;
                    int nRest = 100 % nOpenCharCount;
                    int nClassRate = nRest + pTB_DROP_GROUP_CHAR->My_Item_Select_Value;
                    int nTotalRate = nClassRate - nRest + 100;
                    
                    int nCharItemRand = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, nTotalRate);
                    for (int jj = 0; jj < 9; ++jj)
                    {
                        if (XResourceMgr::IsOpenCharacter(&pGameServer->m_xResourceMgr, jj + 1))
                        {
                            int nCurRate = nDefaultRate;
                            if (nClass == jj + 1)
                                nCurRate += nClassRate;
                            
                            nCharItemRand -= nCurRate;
                            if (nCharItemRand <= 0)
                            {
                                UXSerial serial;
                                XItemFactory::GeneratSerial(&pGameServer->m_xItemFactory, serial);
                                stDropItem.xSerial = serial.xSerial;
                                stDropItem.nObjectID = (&pTB_DROP_GROUP_CHAR->Item_Char_ID_01)[jj];
                                
                                int nStackMax = 1;
                                TB_ITEM* pTBItem = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, stDropItem.nObjectID);
                                if (pTBItem && pTBItem->Item_Stack_Max < nStackMax)
                                    nStackMax = pTBItem->Item_Stack_Max;
                                
                                stDropItem.nCount = nStackMax;
                                stDropItem.xPos = vPos;
                                stDropItem.nBonus = 0;
                                
                                if (stDropItem.nCount > 0)
                                {
                                    psDropInfos.vecInfo.push_back(stDropItem);
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Quest drops
    std::tr1::shared_ptr<CGocQuest> pQuest;
    if (pUser)
    {
        CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, pQuest, false);
    }
    
    if (!pUser || pQuest)
    {
        for (int kk = 0; kk < 2; ++kk)
        {
            if (pQuest)
            {
                if ((&pTB_DROP->Q_Item_Drop_Type_01)[kk])
                {
                    if ((&pTB_DROP->Q_Item_Drop_Type_01)[kk] == 1)
                    {
                        if (pQuest->CheckQuestFirstDropItem((&pTB_DROP->Quest_ID_01)[kk], (&pTB_DROP->Q_Item_ID_01)[kk]))
                        {
                            if (pQuest->FindCondition((&pTB_DROP->Quest_ID_01)[kk]))
                            {
                                int nQuestRand = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, 10000);
                                if (nQuestRand <= (&pTB_DROP->Q_Chance_01)[kk])
                                {
                                    UXSerial serial;
                                    XItemFactory::GeneratSerial(&pGameServer->m_xItemFactory, serial);
                                    stDropItem.xSerial = serial.xSerial;
                                    stDropItem.nObjectID = (&pTB_DROP->Q_Item_ID_01)[kk];
                                    stDropItem.nCount = 1;
                                    stDropItem.xPos = vPos;
                                    stDropItem.nBonus = 0;
                                    
                                    if (stDropItem.nCount > 0)
                                    {
                                        psDropInfos.vecInfo.push_back(stDropItem);
                                        pQuest->AddQuestFirstDropItem((&pTB_DROP->Quest_ID_01)[kk], (&pTB_DROP->Q_Item_ID_01)[kk], 1);
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    int nNeedItemCount = pQuest->GetNeedConditionItemCount((&pTB_DROP->Quest_ID_01)[kk], (&pTB_DROP->Q_Item_ID_01)[kk]);
                    if (nNeedItemCount > 0)
                    {
                        int nQuestRand = XItemFactory::nRand(&pGameServer->m_xItemFactory, 1, 10000);
                        if (nQuestRand <= (&pTB_DROP->Q_Chance_01)[kk])
                        {
                            int nAddCount = XItemFactory::nRand(&pGameServer->m_xItemFactory, (&pTB_DROP->Q_Item_Min_01)[kk], (&pTB_DROP->Q_Item_Max_01)[kk]);
                            if (nNeedItemCount < nAddCount)
                                nAddCount = nNeedItemCount;
                            
                            pQuest->UpdateCondition(eCONDITION_TYPE_COLLECT, eCONDITION_TARGET_ITEM, (&pTB_DROP->Q_Item_ID_01)[kk], nAddCount, 1);
                        }
                    }
                }
            }
        }
    }
    
    // Create drop item group if there are items
    if (!psDropInfos.vecInfo.empty())
    {
        m_nGroupID++;
        psDropInfos.nGroup = m_nGroupID;
        
        CDropItemGroup* pDropGroup = new CDropItemGroup(psDropInfos);
        std::tr1::shared_ptr<CDropItemGroup> pItem(pDropGroup);
        m_mapDropInfo[psDropInfos.nGroup] = pItem;
        
        if (pUser)
        {
            XSendPacket xSendPacket(0x14, 0x01);
            xSendPacket << psDropInfos;
            Send(&xSendPacket);
        }
        else
        {
            // Add to tool drop item list
            for (auto it = psDropInfos.vecInfo.begin(); it != psDropInfos.vecInfo.end(); ++it)
            {
                ST_DROP_ITEM_INFO stTestInfo;
                stTestInfo.nObjectID = it->nObjectID;
                stTestInfo.nCount = it->nCount;
                stTestInfo.nBonus = it->nBonus;
                m_listToolDropItem.push_back(stTestInfo);
            }
        }
    }
    
    return true;
}

void CDropProcess::MazeDrop(PS_TOOL_MAZE_DROP& stMazeDrop)
{
    unsigned short wMapID = stMazeDrop.nMapID;
    int nPCLv = stMazeDrop.nPcLevel;
    int nSpawnRateGrouton = 0;
    int nClass = stMazeDrop.nClass;
    int nMemberCount = stMazeDrop.nPartyMemberCount;
    int nLevelDiff = stMazeDrop.nPartyLevelDiff;
    int nMaxLevel = stMazeDrop.nPartyMaxLevel;
    int nDifficulty = stMazeDrop.nDifficulty;
    
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    VEventObjectResource* pResource = XWorldResMgr::GetResource(&pGameServer->m_xWorldResMgr, wMapID);
    
    if (!pResource)
        return;
    
    VMap<int, void*> ObjectMap(10);
    int nMapLevel = 0;
    
    TB_MAZE_INFO* pTB_MazeInfo = XResourceMgr::GetTB_MAZE_INFO(&pGameServer->m_xResourceMgr, wMapID);
    if (pTB_MazeInfo)
        nMapLevel = pTB_MazeInfo->Maze_Difficulty_Type;
    
    CFAutoSlimWriteLock _autolock(&pGameServer->m_rwMapLock);
    const VMap<int, void*>* pMap = pResource->GetMap(nMapLevel, 1, 0);
    ObjectMap = *pMap;
    _autolock.~CFAutoSlimWriteLock();
    
    void* Iter = ObjectMap.GetStartPosition();
    while (Iter)
    {
        int iKey;
        void* pValue;
        ObjectMap.GetNextPair(&Iter, &iKey, &pValue);
        
        const VEventObjectInfo* pInfo = (const VEventObjectInfo*)pValue;
        if (!pInfo)
            continue;
        
        if (pInfo->eType != eEventObjectType_Box)
            continue;
        
        const VEventBoxInfo* pBoxInfo = (const VEventBoxInfo*)pInfo;
        if (!pBoxInfo)
            continue;
        
        if (pBoxInfo->eBoxType != eEventBoxType_MonsterSpawn)
            continue;
        
        VMonsterSpawnInfo* pMonsterSpawn = (VMonsterSpawnInfo*)pBoxInfo;
        if (!pMonsterSpawn)
            continue;
        
        int nProb = XWorldManager::Instance()->RandProb();
        bool bSpawn = false;
        
        for (int i = 0; i < stMazeDrop.nChallengeCount; ++i)
        {
            for (int k = 0; k < 10; ++k)
            {
                if (!pMonsterSpawn->m_stMonsterInfo[k].m_iID)
                    continue;
                
                if (pMonsterSpawn->m_iWaitCreationSequenceType == 2)
                {
                    if (bSpawn)
                        break;
                    nProb -= pMonsterSpawn->m_stMonsterInfo[k].m_iChance;
                    if (nProb <= 0)
                        bSpawn = true;
                }
                else
                {
                    bSpawn = false;
                    nProb = XWorldManager::Instance()->RandProb();
                    
                    if (pMonsterSpawn->m_stMonsterInfo[k].m_iType != 1)
                    {
                        TB_MONSTER* pTBMonster = XResourceMgr::GetTB_MONSTER(&pGameServer->m_xResourceMgr, pMonsterSpawn->m_stMonsterInfo[k].m_iID);
                        if (pTBMonster)
                        {
                            if (pTBMonster->Monster_Type == 4 && nSpawnRateGrouton > 0)
                            {
                                if (nProb <= nSpawnRateGrouton)
                                    bSpawn = true;
                            }
                            else if (nProb <= pMonsterSpawn->m_stMonsterInfo[k].m_iChance)
                            {
                                bSpawn = true;
                            }
                        }
                    }
                }
                
                if (bSpawn)
                {
                    for (int j = 0; j < pMonsterSpawn->m_iMaxEntityCount; ++j)
                    {
                        int m_iType = pMonsterSpawn->m_stMonsterInfo[k].m_iType;
                        if (!m_iType || m_iType == 2 || m_iType == 4)
                        {
                            int nMonsterID = pMonsterSpawn->m_stMonsterInfo[k].m_iID;
                            TB_MONSTER* pTB_MONSTER = XResourceMgr::GetTB_MONSTER(&pGameServer->m_xResourceMgr, nMonsterID);
                            if (pTB_MONSTER)
                            {
                                int nDropID = pTB_MONSTER->Monster_Drop_ID;
                                int nMonsterLv = pTB_MONSTER->Monster_Lv;
                                XVec3 vPos;
                                bool bBoss = false;
                                
                                float fAddValue = GetDropAddValue(nMemberCount, nLevelDiff, nMaxLevel, nPCLv, nMonsterLv, nDropID);
                                MakeDropItems(nullptr, nDropID, vPos, fAddValue, bBoss, nMonsterID, wMapID, nClass, stMazeDrop.fDropBuff);
                            }
                        }
                    }
                }
            }
        }
    }
}

bool CDropProcess::MakeDropItemDemensionShutter_PVP(CUser* pUser, unsigned int dwDieUCID, int nDropPoint, XVec3& vPos)
{
    if (!pUser || nDropPoint <= 0)
        return false;
    
    PS_DROP_INFOS psDropInfos;
    psDropInfos.bBossDrop = false;
    
    ST_DROP_ITEM_INFO stDropItem;
    
    // Generate serial
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    UXSerial serial;
    XItemFactory::GeneratSerial(&pGameServer->m_xItemFactory, serial);
    stDropItem.xSerial = serial.xSerial;
    stDropItem.nObjectID = 800009998;
    stDropItem.nCount = nDropPoint;
    stDropItem.xPos = vPos;
    stDropItem.nBonus = 0;
    
    psDropInfos.vecInfo.push_back(stDropItem);
    
    // Increment group ID
    m_nGroupID++;
    psDropInfos.nGroup = m_nGroupID;
    
    // Create drop item group
    CDropItemGroup* pDropGroup = new CDropItemGroup(psDropInfos);
    std::tr1::shared_ptr<CDropItemGroup> pItem(pDropGroup);
    m_mapDropInfo[psDropInfos.nGroup] = pItem;
    
    // Send packet
    XSendPacket xSendPacket(0x14, 0x01);
    xSendPacket << psDropInfos;
    Send(&xSendPacket);
    
    return true;
}

void CDropProcess::GetDropInfos(std::map<int, std::tr1::shared_ptr<CDropItemGroup>>& mapDropInfo)
{
    // Copy drop info map
    mapDropInfo = m_mapDropInfo;
}

void CDropProcess::SetDropInfos(std::map<int, std::tr1::shared_ptr<CDropItemGroup>>& mapDropInfo)
{
    // Set drop info map
    m_mapDropInfo = mapDropInfo;
}

void CDropProcess::GetMonsterDropInfo(PS_DROP_INFOS& stDropInfos)
{
    // Copy all items from tool drop item list to the output structure
    for (auto it = m_listToolDropItem.begin(); it != m_listToolDropItem.end(); ++it)
    {
        stDropInfos.vecInfo.push_back(*it);
    }
}

float CDropProcess::GetDropAddValue(int nMemberCount, int nLevelDiff, int nMaxLevel, int nPCLevel, int nMonsterLv, int nDropID)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    
    if (nMemberCount > 0)
    {
        // Party drop rate calculation
        TB_DROPRATE_LEVEL* pDropLevel = XResourceMgr::GetTB_DROPRATE_LEVEL(&pGameServer->m_xResourceMgr, nLevelDiff);
        if (!pDropLevel)
        {
            LogHelper::LogError("game.contents", "DropItem error - No Table TB_DROPRATE_LEVEL[ LevelDiff:%d ] ( %d )", nLevelDiff, 1543);
            return 0.0f;
        }
        float fDropLevel = pDropLevel->DropRate_LevelInterval_Value;
        
        TB_DROPRATE_MEMBER* pDropMember = XResourceMgr::GetTB_DROPRATE_MEMBER(&pGameServer->m_xResourceMgr, nMemberCount);
        if (!pDropMember)
        {
            LogHelper::LogError("game.contents", "DropItem error - No Table TB_DROPRATE_MEMBER[ PartyCount:%d ] ( %d )", nMemberCount, 1555);
            return 0.0f;
        }
        float fDropMember = pDropMember->DropRate_Member_Value;
        
        int nMonsterLvDiff = nMaxLevel - nMonsterLv;
        if (nMonsterLvDiff < 0)
            nMonsterLvDiff = 0;
        
        TB_DROPRATE_MOB* pDropMob = XResourceMgr::GetTB_DROPRATE_MOB(&pGameServer->m_xResourceMgr, nMonsterLvDiff);
        if (!pDropMob)
        {
            LogHelper::LogError("game.contents", "DropItem error - No Table TB_DROPRATE_MOB[ MonsterLvDiff:%d ] ( %d )", nMonsterLvDiff, 1568);
            return 0.0f;
        }
        float fDropMob = pDropMob->DropRate_MobInterval_Value;
        
        if (!IsApplyDropRate(nDropID))
            fDropMob = 1.0f;
        
        return fDropLevel * fDropMember * fDropMob;
    }
    else
    {
        // Solo drop rate calculation
        int nLevelDiff = nPCLevel - nMonsterLv;
        if (nLevelDiff < 0)
            nLevelDiff = 0;
        
        TB_DROPRATE_MOB* pRate = XResourceMgr::GetTB_DROPRATE_MOB(&pGameServer->m_xResourceMgr, nLevelDiff);
        if (!pRate)
        {
            LogHelper::LogError("game.contents", "ProcessDrop error - No Table TB_DROPRATE_MOB[ LevelDiff:%d ] ( %d )", nLevelDiff, 1528);
            return 0.0f;
        }
        
        if (IsApplyDropRate(nDropID))
            return pRate->DropRate_MobInterval_Value;
        else
            return 1.0f;
    }
}

bool CDropProcess::IsApplyDropRate(int nDropID)
{
    // Get TB_DROP table
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DROP* pTB_Drop = XResourceMgr::GetTB_DROP(&pGameServer->m_xResourceMgr, nDropID);
    
    // If table doesn't exist or Drop_Rate_Clear is not 1, apply drop rate
    return !pTB_Drop || pTB_Drop->Drop_Rate_Clear != 1;
}


