#include "GocAppearance.h"
#include "Soulworker/GameServer/XGameServer/XGameServer.h"
#include "Soulworker/GameServer/XCore/XServer/XResourceMgr.h"
#include "Soulworker/GameServer/XCore/Util/LogHelper.h"
#include <memory>

// PS_DB_USE_ITEM_APPREARANCE structure for DB communication
struct PS_DB_USE_ITEM_APPREARANCE
{
    unsigned int dwUCID = 0;
    unsigned int dwItemID = 0;
    __int64 biSerial = 0;
    PS_RES_STORAGE_INFO psUpdateItemList;
    ST_APPEARANCE_INFO stAppearanceInfo;

    PS_DB_USE_ITEM_APPREARANCE() = default;
    ~PS_DB_USE_ITEM_APPREARANCE() = default;
};

// Packet operators for appearance structures (declared elsewhere)
// XPacket& operator>>(XPacket& packet, ST_APPEARANCE_LIST& list);
// XPacket& operator<<(XPacket& packet, const ST_APPEARANCE_INFO& info);

CGocAppearance::CGocAppearance()
{
}

CGocAppearance::~CGocAppearance()
{
}

void CGocAppearance::Init()
{
    GOComponent::Init();
    m_mpAppearanceList.clear();
}

void CGocAppearance::OnUpdate()
{
    OnAppearanceUpdate();
}

void CGocAppearance::LogOut()
{
    // Save appearance data to database on logout
    // This would send the current appearance state to DB
}

// =====================================================
// IDA: 0x1400BB160 - SendDBAppearanceLoad
// =====================================================
void CGocAppearance::SendDBAppearanceLoad()
{
    // IDA decompiled logic:
    // 1. Get CUser from parent object
    // 2. Create XSendDBPacket with packet type 0x21, sub-type 0x29
    // 3. Send UCID to DB
    // 4. Send via XGameServer::SendDBGame
    
    CUser* pUser = GetOwner<CUser>();
    if (!pUser)
        return;

    XSendDBPacket xSendDBPacket(pUser, 0x21, 0x29);
    xSendDBPacket.XParse << pUser->GetUCID();
    
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer)
    {
        pGameServer->SendDBGame(&xSendDBPacket);
    }
}

// =====================================================
// IDA: 0x1400BB270 - LoadAppearanceList
// =====================================================
void CGocAppearance::LoadAppearanceList(ST_APPEARANCE_LIST& stList)
{
    // IDA decompiled logic:
    // 1. Iterate through stList and add each appearance to m_mpAppearanceList
    // 2. Check equipped appearances for validity/expiration
    // 3. If any changes, send update to DB and broadcast to nearby players

    bool bChange = false;

    // Add all appearances from list
    for (size_t i = 0; i < stList.vecInfo.size(); ++i)
    {
        const ST_APPEARANCE_INFO& info = stList.vecInfo[i];
        m_mpAppearanceList[info.wAppearanceID] = info.biEndDate;
    }

    // Get current date for expiration check
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    __int64 biCurDate = pGameServer ? (__int64)pGameServer->GetCurDate() : 0;

    // Check equipped appearances
    CUser* pUser = GetOwner<CUser>();
    if (!pUser)
        return;

    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();
    if (!pCharInfo)
        return;

    // Check each of 4 appearance slots
    for (int j = 0; j < 4; ++j)
    {
        unsigned short wID = pCharInfo->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[j];
        if (wID != 0)
        {
            auto iter = m_mpAppearanceList.find(wID);
            if (iter == m_mpAppearanceList.end())
            {
                // Appearance not owned, clear slot
                pCharInfo->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[j] = 0;
                bChange = true;
            }
            else if (iter->second > 0 && biCurDate > iter->second)
            {
                // Appearance expired, clear slot
                pCharInfo->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[j] = 0;
                bChange = true;
            }
        }
    }

    // If appearance changed, sync to DB and broadcast
    if (bChange)
    {
        XSendDBPacket xSendDBPacket(pUser, 0x21, 0x2C);
        xSendDBPacket.XParse << pUser->GetUCID();
        xSendDBPacket.XParse << pCharInfo->stBaseInfo.uAppearanceEx.biAppearance;
        
        if (pGameServer)
        {
            pGameServer->SendDBGame(&xSendDBPacket);
        }
        
        SendEquipAppearance();
    }
}

// =====================================================
// IDA: 0x1400BB640 - AddAppearance (list version)
// =====================================================
void CGocAppearance::AddAppearance(ST_APPEARANCE_LIST& stList)
{
    // IDA decompiled logic:
    // Iterate through list and call AddAppearance for each item

    for (size_t i = 0; i < stList.vecInfo.size(); ++i)
    {
        const ST_APPEARANCE_INFO& info = stList.vecInfo[i];
        AddAppearance(info.wAppearanceID, info.biEndDate);
    }
}

// =====================================================
// IDA: 0x1400BB800 - AddAppearance (single item)
// =====================================================
void CGocAppearance::AddAppearance(unsigned short wAppearanceID, __int64 biEndDate)
{
    // IDA decompiled logic:
    // 1. Check if appearance already exists in map
    // 2. If exists, update end date
    // 3. If not exists, insert new entry

    auto iter = m_mpAppearanceList.find(wAppearanceID);
    if (iter != m_mpAppearanceList.end())
    {
        // Update existing
        iter->second = biEndDate;
    }
    else
    {
        // Insert new
        m_mpAppearanceList[wAppearanceID] = biEndDate;
    }
}

// =====================================================
// IDA: 0x1400BB8C0 - UpdateAppearance
// =====================================================
void CGocAppearance::UpdateAppearance(unsigned short wAppearanceID, __int64 biAddSec, __int64& biEndDate)
{
    // IDA decompiled logic:
    // 1. If biAddSec is 0, set biEndDate to 0
    // 2. Get current date
    // 3. Check if appearance exists in map
    // 4. Calculate new end date based on current or existing expiration

    if (biAddSec == 0)
    {
        biEndDate = 0;
        return;
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    __int64 biCurDate = pGameServer ? (__int64)pGameServer->GetCurDate() : 0;

    auto iter = m_mpAppearanceList.find(wAppearanceID);
    if (iter != m_mpAppearanceList.end())
    {
        __int64 biDate = iter->second;
        if (biCurDate <= biDate)
        {
            // Extend from existing end date
            biEndDate = biAddSec + biDate;
        }
        else
        {
            // Start from current date
            biEndDate = biAddSec + biCurDate;
        }
    }
    else
    {
        // New appearance, start from current date
        biEndDate = biAddSec + biCurDate;
    }
}

// =====================================================
// IDA: 0x1400BB6E0 - IsHaveAppearance
// =====================================================
bool CGocAppearance::IsHaveAppearance(unsigned short wAppearanceID)
{
    // IDA decompiled logic:
    // 1. Check if appearance is equipped in any slot
    // 2. Check if appearance exists in map
    // 3. If exists, verify it hasn't expired

    CUser* pUser = GetOwner<CUser>();
    if (!pUser)
        return false;

    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();
    if (!pCharInfo)
        return false;

    // Check equipped slots
    for (int i = 0; i < 4; ++i)
    {
        if (pCharInfo->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[i] == wAppearanceID)
            return true;
    }

    // Check appearance list
    auto iter = m_mpAppearanceList.find(wAppearanceID);
    if (iter != m_mpAppearanceList.end())
    {
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        __int64 biCurDate = pGameServer ? (__int64)pGameServer->GetCurDate() : 0;
        __int64 biTime = iter->second;
        
        // If biTime is 0, it's permanent
        // Otherwise, check if still valid
        if (biTime == 0 || biTime > biCurDate)
            return true;
    }

    return false;
}

// =====================================================
// IDA: 0x1400BB9D0 - EquipAppearance
// =====================================================
void CGocAppearance::EquipAppearance(UAppearanceEx& reqAppearance)
{
    // IDA decompiled logic:
    // 1. Get current date
    // 2. Validate each requested appearance ID
    // 3. Clear invalid/expired appearances
    // 4. Update player's appearance data
    // 5. Send to DB and broadcast

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    __int64 biCurDate = pGameServer ? (__int64)pGameServer->GetCurDate() : 0;

    // Validate each slot
    for (int i = 0; i < 4; ++i)
    {
        unsigned short wID = reqAppearance.stAppearance.wAppearanceID[i];
        if (wID != 0)
        {
            auto iter = m_mpAppearanceList.find(wID);
            if (iter == m_mpAppearanceList.end())
            {
                // Not owned
                reqAppearance.stAppearance.wAppearanceID[i] = 0;
            }
            else if (iter->second > 0 && biCurDate > iter->second)
            {
                // Expired
                reqAppearance.stAppearance.wAppearanceID[i] = 0;
            }
        }
    }

    // Update player appearance
    CUser* pUser = GetOwner<CUser>();
    if (!pUser)
        return;

    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();
    if (!pCharInfo)
        return;

    pCharInfo->stBaseInfo.uAppearanceEx = reqAppearance;

    // Send to DB
    XSendDBPacket xSendDBPacket(pUser, 0x21, 0x2C);
    xSendDBPacket.XParse << pUser->GetUCID();
    xSendDBPacket.XParse << reqAppearance.biAppearance;
    
    if (pGameServer)
    {
        pGameServer->SendDBGame(&xSendDBPacket);
    }

    // Broadcast to nearby players
    SendEquipAppearance();
}

// =====================================================
// IDA: 0x1400BBC70 - SendEquipAppearance
// =====================================================
void CGocAppearance::SendEquipAppearance()
{
    // IDA decompiled logic:
    // 1. Get CUser
    // 2. Create XSendPacket with packet type 8, sub-type 0x52
    // 3. Write UCID and appearance data
    // 4. Broadcast to nearby players

    CUser* pUser = GetOwner<CUser>();
    if (!pUser)
        return;

    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();
    if (!pCharInfo)
        return;

    XSendPacket xSendPacket(8, 0x52);
    xSendPacket.XParse << pUser->GetUCID();
    xSendPacket.XParse << pCharInfo->stBaseInfo.uAppearanceEx.biAppearance;

    // Broadcast to nearby players
    CGocNetwork* pNetwork = pUser->GetComponent<CGocNetwork>();
    if (pNetwork)
    {
        pNetwork->BroadcastNearby(nullptr, &xSendPacket);
    }
}

// =====================================================
// IDA: 0x1400BBDD0 - OnAppearanceUpdate
// =====================================================
void CGocAppearance::OnAppearanceUpdate()
{
    // IDA decompiled logic:
    // 1. Get current date
    // 2. Check each equipped appearance slot
    // 3. Clear expired appearances
    // 4. If changes occurred, sync to DB and broadcast

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    __int64 biCurDate = pGameServer ? (__int64)pGameServer->GetCurDate() : 0;

    CUser* pUser = GetOwner<CUser>();
    if (!pUser)
        return;

    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();
    if (!pCharInfo)
        return;

    bool bChange = false;

    // Check each slot
    for (int i = 0; i < 4; ++i)
    {
        unsigned short wID = pCharInfo->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[i];
        if (wID != 0)
        {
            auto iter = m_mpAppearanceList.find(wID);
            if (iter == m_mpAppearanceList.end())
            {
                // Not owned
                pCharInfo->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[i] = 0;
                bChange = true;
            }
            else if (iter->second > 0 && biCurDate > iter->second)
            {
                // Expired
                pCharInfo->stBaseInfo.uAppearanceEx.stAppearance.wAppearanceID[i] = 0;
                bChange = true;
            }
        }
    }

    // Sync if changed
    if (bChange)
    {
        XSendDBPacket xSendDBPacket(pUser, 0x21, 0x2C);
        xSendDBPacket.XParse << pUser->GetUCID();
        xSendDBPacket.XParse << pCharInfo->stBaseInfo.uAppearanceEx.biAppearance;
        
        if (pGameServer)
        {
            pGameServer->SendDBGame(&xSendDBPacket);
        }

        SendEquipAppearance();
    }
}

// =====================================================
// IDA: 0x1400DC260 - CanUseItemAppearance
// =====================================================
bool CGocAppearance::CanUseItemAppearance(const std::tr1::shared_ptr<CItem>& pItem)
{
    // IDA decompiled logic:
    // 1. Check item validity
    // 2. Get appearance ID from item table
    // 3. Check if player already has this appearance
    // 4. Return true if can use, false otherwise

    CUser* pUser = GetOwner<CUser>();
    if (!pUser)
    {
        LogHelper::LogError("game.item", "CanUseItemAppearance error - Invalid User");
        return false;
    }

    unsigned int dwUCID = pUser->GetUCID();

    if (!pItem || pItem->IsInvalid())
    {
        LogHelper::LogError("game.item", "CanUseItemAppearance error - Invalid Item[UCID:%d]", dwUCID);
        return false;
    }

    TB_ITEM* pTBItem = pItem->GetItemTable();
    if (!pTBItem)
        return false;

    unsigned short wAppearanceID = pTBItem->Item_Model_ID;

    // Check if already has this appearance
    if (IsHaveAppearance(wAppearanceID))
    {
        // Send error message
        CGocNetwork::SendErrorMessage(pUser, 8, 0x11, 0xCB64);
        LogHelper::LogError("game.item", 
            "CanUseItemAppearance error - Overlapped Appearance[UCID:%d, ItemID:%d, AppearanceID:%d]",
            dwUCID, pItem->GetItemID(), wAppearanceID);
        return false;
    }

    return true;
}

// =====================================================
// IDA: 0x1400DC450 - UseItemAppearance
// =====================================================
bool CGocAppearance::UseItemAppearance(const std::tr1::shared_ptr<CItem>& pItem)
{
    // IDA decompiled logic:
    // 1. Validate user and item
    // 2. Reduce item count
    // 3. Update item end
    // 4. Add/update appearance
    // 5. Send to DB

    CUser* pUser = GetOwner<CUser>();
    if (!pUser)
    {
        LogHelper::LogError("game.item", "UseItemAppearance error - Invalid User");
        return false;
    }

    unsigned int dwUCID = pUser->GetUCID();

    if (!pItem || pItem->IsInvalid())
    {
        LogHelper::LogError("game.item", "UseItemAppearance error - Invalid Item[UCID:%d]", dwUCID);
        return false;
    }

    TB_ITEM* pTBItem = pItem->GetItemTable();
    if (!pTBItem)
    {
        LogHelper::LogError("game.item", 
            "UseItemAppearance error - No Table TB_Item[UCID:%d, ItemID:%d]",
            dwUCID, pItem->GetItemID());
        return false;
    }

    PS_RES_STORAGE_INFO psUpdateItemList;
    PS_RES_STORAGE_INFO psCreateItemList;

    // Reduce item count
    CGocInventory* pInventory = pUser->GetComponent<CGocInventory>();
    if (!pInventory)
        return false;

    if (!pInventory->ReduceItem2(pTBItem, 1, 0x7A, &psUpdateItemList))
    {
        pInventory->UnLockList(&psUpdateItemList);
        LogHelper::LogError("game.item", "UseItemAppearance error - Failed ReduceItem2[UCID:%d]", dwUCID);
        return false;
    }

    // Log item usage
    ST_LOG_GAME stLog;
    stLog._sSubType = 22;
    stLog.nParam3 = pItem->GetItemID();
    stLog.nParam6 = pItem->GetSerial();

    if (!pInventory->UpdateItemEnd(0x7A, &psUpdateItemList, &stLog))
    {
        pInventory->UnLockList(&psUpdateItemList);
        LogHelper::LogError("game.item", "UseItemAppearance error - Failed UpdateItemEnd[UCID:%d]", dwUCID);
        return false;
    }

    // Calculate appearance end date
    ST_APPEARANCE_INFO stAppearanceInfo;
    stAppearanceInfo.wAppearanceID = pTBItem->Item_Model_ID;
    
    UpdateAppearance(stAppearanceInfo.wAppearanceID,
        60 * pTBItem->Item_Use_Period_Value,
        stAppearanceInfo.biEndDate);

    // Add appearance to list
    AddAppearance(stAppearanceInfo.wAppearanceID, stAppearanceInfo.biEndDate);

    // Send to DB
    PS_DB_USE_ITEM_APPREARANCE psDBItemInfo;
    psDBItemInfo.dwUCID = dwUCID;
    psDBItemInfo.psUpdateItemList = psUpdateItemList;
    psDBItemInfo.stAppearanceInfo = stAppearanceInfo;
    psDBItemInfo.dwItemID = pItem->GetItemID();
    psDBItemInfo.biSerial = pItem->GetSerial();

    XSendDBPacket xSendDBPacket(pUser, 0x21, 0x2A);
    // Serialize psDBItemInfo to packet
    // operator<<(&xSendDBPacket, &psDBItemInfo);

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer)
    {
        pGameServer->SendDBGame(&xSendDBPacket);
    }

    return true;
}
