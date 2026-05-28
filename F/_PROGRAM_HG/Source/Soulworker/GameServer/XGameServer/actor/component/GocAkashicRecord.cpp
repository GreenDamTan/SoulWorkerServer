// CGocAkashicRecord - Game Object Component for Akashic Record System
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140018B80 - 0x140021DC3

#include "GocAkashicRecord.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "XCore/XServer/XSendPacket.h"
#include "XCore/XServer/XSendDBPacket.h"
#include "XCore/XServer/TXSingleton.h"
#include "XSCommon/Table/DBLoadTable.h"
#include "XGameServer/User.h"
#include "XGameServer/GameServer.h"
#include "Actor/Component/GocNetwork.h"
#include "Actor/Component/GocInventory.h"
#include "Actor/Mover/CMover.h"
#include "Actor/Mover/CMySkillList.h"
#include <cstring>
#include <cstdlib>
#include <algorithm>

// Forward declarations for external helpers
class CGocInventory;

// Constructor (0x140018B80)
CGocAkashicRecord::CGocAkashicRecord()
    : GOComponent()
    , m_bLoad(false)
    , m_bUserLoad(false)
    , m_bDisassembleAkashic(false)
    , m_byDeckCount(0)
    , m_byActiveDeck(0)
{
    memset(m_psQuickSlotCard, 0, sizeof(m_psQuickSlotCard));
}

// Destructor (0x140018C70)
CGocAkashicRecord::~CGocAkashicRecord()
{
    Clear();
}

// Initialize
bool CGocAkashicRecord::Initialize()
{
    return Init();
}

void CGocAkashicRecord::Shutdown()
{
    Clear();
}

void CGocAkashicRecord::Update(float fDeltaTime)
{
    (void)fDeltaTime;
}

// Init (0x140018CF0)
bool CGocAkashicRecord::Init()
{
    Clear();
    return true;
}

// Clear (0x140018D10)
void CGocAkashicRecord::Clear()
{
    m_bLoad = false;
    m_mapAkashic.clear();
    m_mapAkashicPassive.clear();
    m_vecSortedAkashicPassive.clear();
    m_bUserLoad = false;
    m_bDisassembleAkashic = false;
    memset(m_psQuickSlotCard, 0, sizeof(m_psQuickSlotCard));
    m_setAkashicGetInfo.clear();
    m_byDeckCount = 0;
    m_byActiveDeck = 0;
}

// SendDBAkashicRecordLoad (0x140018DB0)
// Sends a request to the database to load the player's Akashic records
void CGocAkashicRecord::SendDBAkashicRecordLoad()
{
    // Get the owner object (CUser)
    IXObject* pObject = GetOwnerObject();
    if (!pObject)
        return;

    // Get UCID from user
    CUser* pUser = dynamic_cast<CUser*>(pObject);
    if (!pUser)
        return;

    // Create DB packet - main=0x21 (DB_GAME), sub=0x35 (DB_AKASHIC_RECORD_LOAD)
    XSendDBPacket xSendDBPacket(pObject, 0x21, 0x35);

    // Write UCID to packet
    // TODO: Need to get UCID from CUser
    // xSendDBPacket << pUser->GetUCID();

    // Send to GameServer's DB connection
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer)
    {
        pGameServer->SendDBGame(&xSendDBPacket);
    }
}

// ResAkashicRecordLoad (0x140018EC0)
// Handles the database response containing the player's Akashic records
void CGocAkashicRecord::ResAkashicRecordLoad(ST_AKASHIC_LIST& stAkashicList)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    int nCount = static_cast<int>(stAkashicList.vecInfo.size());

    for (int i = 0; i < nCount; ++i)
    {
        ST_AKASHIC_RECORD& stInfo = stAkashicList.vecInfo[i];

        // Get the Akashic record table entry
        TB_AKASHIC_RECORDS* pTBAkashic = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(stInfo.dwAkashicID);
        if (!pTBAkashic)
        {
            // Log error - failed to load akashic
            continue;
        }

        // Create the akashic record entry
        ST_AKASHIC_RECORD stAkashicRecord;
        stAkashicRecord.dwAkashicID = stInfo.dwAkashicID;
        stAkashicRecord.nPosition = pTBAkashic->Array_Index;
        stAkashicRecord.nAkashicExp = stInfo.nAkashicExp;

        // Insert into map using position as key
        m_mapAkashic[stAkashicRecord.nPosition] = stAkashicRecord;
    }

    m_bLoad = true;
}

// SendAkasicRecordList (0x140019000)
// Sends the current Akashic record list to the client
void CGocAkashicRecord::SendAkasicRecordList()
{
    // Build the list to send
    ST_AKASHIC_LIST stAkashicList;

    // Iterate through all stored akashic records
    for (auto it = m_mapAkashic.begin(); it != m_mapAkashic.end(); ++it)
    {
        stAkashicList.vecInfo.push_back(it->second);
    }

    // Create send packet - main=8, sub=0x57
    XSendPacket xSendPacket(8, 0x57);
    xSendPacket << stAkashicList;

    // Get owner actor and send
    XActor* pActor = GetOwnerActor();
    if (pActor)
    {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// AddAkashicRecord (0x140019170)
// Adds a new Akashic record to the player's collection
bool CGocAkashicRecord::AddAkashicRecord(std::uint32_t nItemID, int nAkashicExp)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return false;

    // Get the Akashic record table entry
    TB_AKASHIC_RECORDS* pTBAkashic = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(nItemID);
    if (!pTBAkashic)
        return false;

    // Create the akashic record entry
    ST_AKASHIC_RECORD stAkashicInfo;
    stAkashicInfo.dwAkashicID = pTBAkashic->ID;
    stAkashicInfo.nPosition = pTBAkashic->Array_Index;
    stAkashicInfo.nAkashicExp = nAkashicExp;

    // Insert into map
    m_mapAkashic[stAkashicInfo.nPosition] = stAkashicInfo;

    // Get owner for UCID
    IXObject* pObject = GetOwnerObject();
    CUser* pUser = pObject ? dynamic_cast<CUser*>(pObject) : nullptr;

    // Send DB packet to save - main=0x21, sub=0x34
    if (pObject)
    {
        XSendDBPacket xSendDBPacket(pObject, 0x21, 0x34);

        // Write akashic use data
        PS_DB_AKASHIC_USE psDBAkashicUse;
        psDBAkashicUse.dwUCID = 0; // TODO: Get from user
        psDBAkashicUse.dwAkashicID = stAkashicInfo.dwAkashicID;
        psDBAkashicUse.byState = 0;
        psDBAkashicUse.nAkashicExp = stAkashicInfo.nAkashicExp;
        xSendDBPacket << psDBAkashicUse;

        pGameServer->SendDBGame(&xSendDBPacket);
    }

    // Send packet to client - main=8, sub=0x58
    XSendPacket xSendPacket(8, 0x58);
    xSendPacket << stAkashicInfo;
    xSendPacket << static_cast<std::uint8_t>(1); // Success flag

    XActor* pActor = GetOwnerActor();
    if (pActor)
    {
        CGocNetwork::Send(pActor, &xSendPacket);
    }

    // Send DB log
    SendDBLog(2, pTBAkashic->ID, 0, 0, 0);

    // Send statistics - main=0xF0, sub=4
    if (pObject)
    {
        ST_STATISTICS_AKASHIC stStatistics;
        stStatistics.byFlag = 1;
        stStatistics.dwAkashicID = pTBAkashic->ID;

        XSendDBPacket xSendDBStatistics(pObject, 0xF0, 4);
        xSendDBStatistics << stStatistics;
        pGameServer->SendDBStatistics(&xSendDBStatistics);
    }

    return true;
}

// Reset (0x14001D0D0)
// Resets all Akashic records for the player
void CGocAkashicRecord::Reset()
{
    IXObject* pObject = GetOwnerObject();

    // Send DB reset packet - main=0x21, sub=0x37
    if (pObject)
    {
        XSendDBPacket xSendDBPacket(pObject, 0x21, 0x37);
        // Write UCID
        // xSendDBPacket << pUser->GetUCID();

        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        if (pGameServer)
        {
            pGameServer->SendDBGame(&xSendDBPacket);
        }
    }

    // Send empty list to client
    ST_AKASHIC_LIST stAkashicList;
    XSendPacket xSendPacket(8, 0x57);
    xSendPacket << stAkashicList;

    XActor* pActor = GetOwnerActor();
    if (pActor)
    {
        CGocNetwork::Send(pActor, &xSendPacket);
    }

    // Clear local data
    m_mapAkashic.clear();

    // Send statistics log
    if (pObject)
    {
        ST_STATISTICS_AKASHIC stStatistics;
        stStatistics.byFlag = 4;

        XSendDBPacket xSendDBStatistics(pObject, 0xF0, 4);
        xSendDBStatistics << stStatistics;

        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        if (pGameServer)
        {
            pGameServer->SendDBStatistics(&xSendDBStatistics);
        }
    }
}

// UpdateAkashicPassiveList (0x14001B880)
// Updates the passive Akashic card list based on current quick slot configuration
void CGocAkashicRecord::UpdateAkashicPassiveList()
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    // Clear the passive list
    m_mapAkashicPassive.clear();

    // Check each card slot in active deck
    for (int i = 0; i < 5; ++i)
    {
        PS_QUICKSLOT_CARD& rCard = m_psQuickSlotCard[m_byActiveDeck];
        std::uint32_t dwCardID = 0;

        // Get card ID based on slot index
        switch (i)
        {
        case 0: dwCardID = rCard.nCard_1; break;
        case 1: dwCardID = rCard.nCard_2; break;
        case 2: dwCardID = rCard.nCard_3; break;
        case 3: dwCardID = rCard.nCard_4; break;
        case 4: dwCardID = rCard.nCard_5; break;
        }

        if (!dwCardID)
            continue;

        TB_AKASHIC_RECORDS* pTBAkashic = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwCardID);
        if (pTBAkashic && pTBAkashic->Type == 4)
        {
            // Type 4 = Passive Akashic
            m_mapAkashicPassive[pTBAkashic->ID] = pTBAkashic;
        }
    }

    // Clear sorted list and rebuild
    m_vecSortedAkashicPassive.clear();

    // Build sorted list by cooltime group
    for (auto it = m_mapAkashicPassive.begin(); it != m_mapAkashicPassive.end(); ++it)
    {
        TB_AKASHIC_RECORDS* pAkashicTB = it->second;
        if (!pAkashicTB)
            continue;

        // Check if this cooltime group already exists in sorted list
        bool bExistCooltimeGroup = false;
        for (size_t j = 0; j < m_vecSortedAkashicPassive.size(); ++j)
        {
            TB_AKASHIC_RECORDS* pExisting = m_vecSortedAkashicPassive[j];
            if (pExisting && pExisting->CoolTime_Group == pAkashicTB->CoolTime_Group)
            {
                bExistCooltimeGroup = true;
                break;
            }
        }

        if (!bExistCooltimeGroup)
        {
            // Get all passive akashics with same cooltime group and add them
            int iIndex = 0;
            while (TB_AKASHIC_RECORDS* pTemp = GetPassiveAkashicByGrade(pAkashicTB->CoolTime_Group, iIndex))
            {
                m_vecSortedAkashicPassive.push_back(pTemp);
                ++iIndex;
            }
        }
    }
}

// GetPassiveAkashicByGrade (0x14001BAF0)
// Gets a passive Akashic record by cooltime group and index
TB_AKASHIC_RECORDS* CGocAkashicRecord::GetPassiveAkashicByGrade(std::uint32_t dwCoolTimeGroup, int iIndex)
{
    std::vector<TB_AKASHIC_RECORDS*> vecSortedAkashicPassive;
    int iGroupCount = 0;

    // Collect all passive akashics with matching cooltime group
    for (auto it = m_mapAkashicPassive.begin(); it != m_mapAkashicPassive.end(); ++it)
    {
        TB_AKASHIC_RECORDS* pTempAkashicTB = it->second;
        if (pTempAkashicTB && pTempAkashicTB->CoolTime_Group == dwCoolTimeGroup)
        {
            vecSortedAkashicPassive.push_back(pTempAkashicTB);
            ++iGroupCount;
        }
    }

    // Check if requested index is valid
    if (iGroupCount >= iIndex + 1)
    {
        // Sort the list (by some criteria - likely priority or ID)
        std::sort(vecSortedAkashicPassive.begin(), vecSortedAkashicPassive.end(),
            [](TB_AKASHIC_RECORDS* a, TB_AKASHIC_RECORDS* b) {
                return a->ID < b->ID;
            });

        TB_AKASHIC_RECORDS* pResult = vecSortedAkashicPassive[iIndex];
        return pResult;
    }

    return nullptr;
}

// RegisterAllAkashicRecord (0x14001CD60)
// Registers all Akashic records from the resource manager and sends to client/DB
void CGocAkashicRecord::RegisterAllAkashicRecord()
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    ST_AKASHIC_LIST stAkashicList;

    // Iterate through all Akashic records in resource manager
    // TODO: Need proper iteration over TB_AKASHIC_RECORDS map
    // for (auto& akashicPair : pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS_Map())
    // {
    //     TB_AKASHIC_RECORDS& rAkashic = akashicPair.second;
    //
    //     // Skip if has special limit type flag
    //     if (rAkashic.Akashic_Limit_Type & 8)
    //         continue;
    //
    //     ST_AKASHIC_RECORD stAkashicRecord;
    //     stAkashicRecord.dwAkashicID = rAkashic.ID;
    //     stAkashicRecord.nPosition = rAkashic.Array_Index;
    //     stAkashicRecord.nAkashicExp = 0;
    //
    //     m_mapAkashic[stAkashicRecord.nPosition] = stAkashicRecord;
    //     stAkashicList.vecInfo.push_back(stAkashicRecord);
    //
    //     // Send statistics
    //     IXObject* pObject = GetOwnerObject();
    //     if (pObject)
    //     {
    //         ST_STATISTICS_AKASHIC stStatistics;
    //         stStatistics.byFlag = 1;
    //         stStatistics.dwAkashicID = stAkashicRecord.dwAkashicID;
    //
    //         XSendDBPacket xSendDBStatistics(pObject, 0xF0, 4);
    //         xSendDBStatistics << stStatistics;
    //         pGameServer->SendDBStatistics(&xSendDBStatistics);
    //     }
    // }

    // Send list to client
    SendAkasicRecordList();

    // Send DB save packet - main=0x21, sub=0x36
    IXObject* pObject = GetOwnerObject();
    if (pObject)
    {
        XSendDBPacket xSendDBPacket(pObject, 0x21, 0x36);
        // xSendDBPacket << pUser->GetUCID();
        xSendDBPacket << stAkashicList;

        pGameServer->SendDBGame(&xSendDBPacket);
    }
}

// SendDBLog (0x14001D390)
// Sends a log entry to the database for Akashic actions
void CGocAkashicRecord::SendDBLog(std::int16_t shSubType, int nParam0, int nParam1, int nParam4, int nParam3)
{
    IXObject* pObject = GetOwnerObject();
    CUser* pUser = pObject ? dynamic_cast<CUser*>(pObject) : nullptr;

    ST_LOG_GAME stLog;
    stLog._sMainType = 21;
    stLog._sSubType = shSubType;

    if (pUser)
    {
        // stLog._nUAID = pUser->GetUAID();
        // stLog._nUCID = pUser->GetUCID();
    }

    stLog.nParam0 = nParam0;
    stLog.nParam1 = nParam1;
    stLog.nParam2 = 0; // Level or other parameter
    stLog.nParam3 = nParam3;
    stLog.nParam4 = nParam4;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer)
    {
        pGameServer->SendDBLog(&stLog);
    }
}

// AddAkashicGetInfo (0x14001D500)
// Adds Akashic get info and sends to DB if new
void CGocAkashicRecord::AddAkashicGetInfo(std::uint32_t dwAkashicID)
{
    IXObject* pObject = GetOwnerObject();
    CUser* pUser = pObject ? dynamic_cast<CUser*>(pObject) : nullptr;
    if (!pUser)
        return;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    // Get the item table entry
    TB_ITEM* pTB_AkashicItem = pGameServer->GetResourceMgr().GetTB_ITEM(dwAkashicID);
    if (!pTB_AkashicItem)
        return;

    // Check if this is an Akashic item (Item_Effect_Type == 8)
    if (pTB_AkashicItem->Item_Effect_Type != 8)
        return;

    // Get the Akashic record
    TB_AKASHIC_RECORDS* pTB_AkashicRecords = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwAkashicID);
    if (!pTB_AkashicRecords)
        return;

    std::uint32_t dwAkashicGroupID = pTB_AkashicRecords->Akashic_Group;

    // Check if we already have this group
    if (m_setAkashicGetInfo.find(dwAkashicGroupID) != m_setAkashicGetInfo.end())
        return;

    // Add to set
    m_setAkashicGetInfo.insert(dwAkashicGroupID);

    // Send DB packet - main=0x81, sub=0x31
    PS_DB_AKASHIC_GETINFO psDBAkashicGetInfo;
    psDBAkashicGetInfo.dwUCID = 0; // TODO: Get from user
    psDBAkashicGetInfo.dwAkashicGroupID = dwAkashicGroupID;

    XSendDBPacket xSendDBPacket(pObject, 0x81, 0x31);
    xSendDBPacket << psDBAkashicGetInfo;
    pGameServer->SendDBGame(&xSendDBPacket);

    // Send to client - main=0x18, sub=0x34
    XSendPacket xSendPacket(0x18, 0x34);
    xSendPacket << dwAkashicGroupID;

    XActor* pActor = GetOwnerActor();
    if (pActor)
    {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// ThinkAkashicPassive (0x14001C640)
// Processes passive Akashic effects (called each update tick)
void CGocAkashicRecord::ThinkAkashicPassive()
{
    IXObject* pObject = GetOwnerObject();
    CUser* pUser = pObject ? dynamic_cast<CUser*>(pObject) : nullptr;
    if (!pUser)
        return;

    // Get skill manager
    CMySkillList* pSkillMgr = pUser->GetSkillMgr();
    if (!pSkillMgr)
        return;

    // Check user status - skip if in certain states
    if (pUser->IsStatus(2) || pUser->IsStatus(4))
        return;

    if (!m_bUserLoad)
        return;

    // Iterate through sorted passive akashics
    for (size_t i = 0; i < m_vecSortedAkashicPassive.size(); ++i)
    {
        TB_AKASHIC_RECORDS* pAkashicTB = m_vecSortedAkashicPassive[i];
        if (!pAkashicTB)
            continue;

        // Check if can apply buff
        if (!pUser->IsCanApplyBuff(pAkashicTB->Skill_ID, nullptr))
            continue;

        // Check for Invoke_Condition == 46 (random trigger)
        if (pAkashicTB->Invoke_Condition == 46)
        {
            int nInvokeRate = pAkashicTB->Invoke_Rate;
            int nRandom = std::rand() % 10000;

            if (nInvokeRate > nRandom)
            {
                // Check cooltime
                float fCooltime = pSkillMgr->GetCooltime(E_COOLTIME_AKASHIC,
                    pAkashicTB->CoolTime_Group, pAkashicTB->CoolTime_Global, 1);

                if (fCooltime == 0.0f)
                {
                    // Apply buff
                    if (pUser->SetBuffStatus(pAkashicTB->Skill_ID, 0, true) == 1)
                    {
                        SendAkasicRecordRes(pUser, pAkashicTB);
                    }

                    // Set cooltime
                    pSkillMgr->SetAkashicCooltime(pAkashicTB);
                }
            }
        }
    }
}

// CheckPassiveSkill (0x14001CA80)
// Checks and activates passive skills based on condition
bool CGocAkashicRecord::CheckPassiveSkill(std::uint8_t byCondition)
{
    IXObject* pObject = GetOwnerObject();
    CUser* pUser = pObject ? dynamic_cast<CUser*>(pObject) : nullptr;
    if (!pUser)
        return false;

    // For certain conditions, check if player is dead
    if (byCondition != 31 && byCondition != 35)
    {
        if (pUser->IsDie())
            return false;
    }

    CMySkillList* pSkillMgr = pUser->GetSkillMgr();
    if (!pSkillMgr)
        return false;

    // Iterate through passive akashics
    for (auto it = m_mapAkashicPassive.begin(); it != m_mapAkashicPassive.end(); ++it)
    {
        TB_AKASHIC_RECORDS* pAkashicTB = it->second;
        if (!pAkashicTB)
            continue;

        // Check if condition matches
        if (pAkashicTB->Invoke_Condition != byCondition)
            continue;

        int nInvokeRate = pAkashicTB->Invoke_Rate;
        int nRandom = std::rand() % 10000;

        if (nInvokeRate > nRandom)
        {
            // Check cooltime
            float fCooltime = pSkillMgr->GetCooltime(E_COOLTIME_AKASHIC,
                pAkashicTB->CoolTime_Group, pAkashicTB->CoolTime_Global, 1);

            if (fCooltime == 0.0f)
            {
                // Apply buff
                if (pUser->SetBuffStatus(pAkashicTB->Skill_ID, 0, true))
                {
                    SendAkasicRecordRes(pUser, pAkashicTB);
                }

                // Set cooltime
                pSkillMgr->SetAkashicCooltime(pAkashicTB);
            }
        }
    }

    return true;
}

// SendAkasicRecordRes (0x14001C8C0)
// Sends Akashic record response to client
void CGocAkashicRecord::SendAkasicRecordRes(CUser* pUser, TB_AKASHIC_RECORDS* pTBAkashic)
{
    if (!pUser || !pTBAkashic)
        return;

    PS_RES_AkashicRecord psRes;
    psRes.dwAkashicID = pTBAkashic->ID;
    // Fill other fields

    XSendPacket xSendPacket(8, 0x59);
    xSendPacket << psRes;

    CGocNetwork::Send(static_cast<XActor*>(pUser), &xSendPacket);
}

// LoadQuickSlotCard (0x14001BFC0)
// Loads quick slot card data from database response
bool CGocAkashicRecord::LoadQuickSlotCard(PS_QUICKSLOT_CARD_VEC& psCardDeck)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return false;

    // Set deck count from vector size
    size_t nDeckSize = psCardDeck.second.size();
    if (nDeckSize > 0 && nDeckSize <= 5)
    {
        m_byDeckCount = static_cast<std::uint8_t>(nDeckSize);
    }
    else
    {
        m_byDeckCount = 0;
    }

    // Set active deck
    std::uint8_t byActiveDeck = static_cast<std::uint8_t>(psCardDeck.first);
    if (byActiveDeck >= 5 || byActiveDeck > m_byDeckCount - 1)
        byActiveDeck = 0;
    m_byActiveDeck = byActiveDeck;

    // Process each deck
    for (int k = 0; k < m_byDeckCount; ++k)
    {
        PS_QUICKSLOT_CARD& psQuickSlotCard = psCardDeck.second[k];

        if (psQuickSlotCard.byPage >= 5 || psQuickSlotCard.byPage >= m_byDeckCount)
            continue;

        // Copy card data
        m_psQuickSlotCard[psQuickSlotCard.byPage] = psQuickSlotCard;

        // Validate each card slot
        std::set<std::uint32_t> setCardEffectGroups;

        for (int i = 0; i < 5; ++i)
        {
            std::uint32_t dwCardID = 0;
            switch (i)
            {
            case 0: dwCardID = psQuickSlotCard.nCard_1; break;
            case 1: dwCardID = psQuickSlotCard.nCard_2; break;
            case 2: dwCardID = psQuickSlotCard.nCard_3; break;
            case 3: dwCardID = psQuickSlotCard.nCard_4; break;
            case 4: dwCardID = psQuickSlotCard.nCard_5; break;
            }

            if (!dwCardID)
            {
                // Clear slot
                switch (i)
                {
                case 0: m_psQuickSlotCard[k].nCard_1 = 0; break;
                case 1: m_psQuickSlotCard[k].nCard_2 = 0; break;
                case 2: m_psQuickSlotCard[k].nCard_3 = 0; break;
                case 3: m_psQuickSlotCard[k].nCard_4 = 0; break;
                case 4: m_psQuickSlotCard[k].nCard_5 = 0; break;
                }
                continue;
            }

            TB_AKASHIC_RECORDS* pTBAkashicRecord = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwCardID);
            if (!pTBAkashicRecord)
            {
                // Clear invalid card
                switch (i)
                {
                case 0: m_psQuickSlotCard[k].nCard_1 = 0; break;
                case 1: m_psQuickSlotCard[k].nCard_2 = 0; break;
                case 2: m_psQuickSlotCard[k].nCard_3 = 0; break;
                case 3: m_psQuickSlotCard[k].nCard_4 = 0; break;
                case 4: m_psQuickSlotCard[k].nCard_5 = 0; break;
                }
                continue;
            }

            // Check if player owns this card
            bool bHasCard = false;
            TB_AKASHIC_RECORDS* pFindPCAkashic = pGameServer->GetResourceMgr().FindPCAkashic(dwCardID);
            if (pFindPCAkashic)
            {
                bHasCard = true;
            }
            else
            {
                // Check if in akashic map
                auto it = m_mapAkashic.find(pTBAkashicRecord->Array_Index);
                bHasCard = (it != m_mapAkashic.end());
            }

            if (!bHasCard)
            {
                // Send error and clear slot
                XActor* pActor = GetOwnerActor();
                if (pActor)
                {
                    CGocNetwork::SendErrorMessage(pActor, 8, 0x28, 0xCB8D);
                }
                // Clear slot
                switch (i)
                {
                case 0: m_psQuickSlotCard[k].nCard_1 = 0; break;
                case 1: m_psQuickSlotCard[k].nCard_2 = 0; break;
                case 2: m_psQuickSlotCard[k].nCard_3 = 0; break;
                case 3: m_psQuickSlotCard[k].nCard_4 = 0; break;
                case 4: m_psQuickSlotCard[k].nCard_5 = 0; break;
                }
                continue;
            }

            // Check for duplicate effect group
            std::uint32_t dwEffectGroup = pTBAkashicRecord->Akashic_Effect_Group;
            if (setCardEffectGroups.find(dwEffectGroup) != setCardEffectGroups.end())
            {
                // Duplicate - clear slot
                switch (i)
                {
                case 0: m_psQuickSlotCard[k].nCard_1 = 0; break;
                case 1: m_psQuickSlotCard[k].nCard_2 = 0; break;
                case 2: m_psQuickSlotCard[k].nCard_3 = 0; break;
                case 3: m_psQuickSlotCard[k].nCard_4 = 0; break;
                case 4: m_psQuickSlotCard[k].nCard_5 = 0; break;
                }
                continue;
            }

            setCardEffectGroups.insert(dwEffectGroup);
        }
    }

    // Update passive list
    UpdateAkashicPassiveList();

    return true;
}

// ReqDisassembleAkashic (0x140019550)
// Requests disassembly of Akashic cards
bool CGocAkashicRecord::ReqDisassembleAkashic(PS_AKASHIC_DISASSEMBLE_LIST& psList)
{
    // TODO: Implement full disassembly logic per IDA
    // This is a complex function that:
    // 1. Validates each akashic to disassemble
    // 2. Checks ownership and limit types
    // 3. Calculates costs (money, ether, BP)
    // 4. Creates disassembly items
    // 5. Sends DB packets and client responses

    return false; // TODO: 汇编还原 - Full implementation needed
}

// ResDisassembleAkashic (0x14001A920)
// Handles disassembly response from database
bool CGocAkashicRecord::ResDisassembleAkashic(PS_DB_AKASHIC_DISASSEMBLE& psDBDisassemble)
{
    IXObject* pObject = GetOwnerObject();
    CUser* pUser = pObject ? dynamic_cast<CUser*>(pObject) : nullptr;
    if (!pUser)
        return false;

    // Get inventory component
    // TODO: Get CGocInventory from user

    // Build disassemble list for client
    ST_AKASHIC_LIST stDisassembleList;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return false;

    // Process each disassembled akashic
    for (size_t i = 0; i < psDBDisassemble.psList.size(); ++i)
    {
        std::uint32_t dwAkashicID = psDBDisassemble.psList[i].dwAkashicID;

        TB_AKASHIC_RECORDS* pTBAkashic = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwAkashicID);
        if (!pTBAkashic)
            return false;

        int nAkashicKey = pTBAkashic->Array_Index;

        // Find and remove from map
        auto it = m_mapAkashic.find(nAkashicKey);
        if (it != m_mapAkashic.end())
        {
            ST_AKASHIC_RECORD stAkashicRecord = it->second;
            m_mapAkashic.erase(it);
            stDisassembleList.vecInfo.push_back(stAkashicRecord);
        }

        // Send statistics
        ST_STATISTICS_AKASHIC stStatistics;
        stStatistics.byFlag = 3;
        stStatistics.dwAkashicID = dwAkashicID;

        XSendDBPacket xSendDBStatistics(pObject, 0xF0, 4);
        xSendDBStatistics << stStatistics;
        pGameServer->SendDBStatistics(&xSendDBStatistics);

        // Send log
        SendDBLog(3, dwAkashicID, 0, psDBDisassemble.byState, 0);
    }

    // Send create item response
    // TODO: Call CGocInventory::SendCreateItem

    // Send update item response
    // TODO: Call CGocInventory::SendUpdateItem

    // Send disassemble result to client
    XSendPacket xSendPacket(0x18, 0x15);
    xSendPacket << static_cast<std::uint8_t>(1);
    xSendPacket << stDisassembleList;

    CGocNetwork::Send(static_cast<XActor*>(pUser), &xSendPacket);

    return true;
}

// SetQuickSlotCard
void CGocAkashicRecord::SetQuickSlotCard(PS_QUICKSLOT_CARD* pCard)
{
    if (pCard && pCard->bySlotIndex < 5)
    {
        m_psQuickSlotCard[pCard->bySlotIndex] = *pCard;
    }
}

// GetQuickSlotCard
void CGocAkashicRecord::GetQuickSlotCard(PS_QUICKSLOT_CARD* pCard)
{
    if (pCard && pCard->bySlotIndex < 5)
    {
        *pCard = m_psQuickSlotCard[pCard->bySlotIndex];
    }
}

// GetOwnerMover - Helper function
CMover* CGocAkashicRecord::GetOwnerMover() const
{
    // TODO: Implement based on GOComponent owner mechanism
    return nullptr;
}

// GetOwnerObject - Helper function
IXObject* CGocAkashicRecord::GetOwnerObject() const
{
    // TODO: Implement based on GOComponent owner mechanism
    return nullptr;
}

// GetOwnerActor - Helper function
XActor* CGocAkashicRecord::GetOwnerActor() const
{
    // TODO: Implement based on GOComponent owner mechanism
    return nullptr;
}
