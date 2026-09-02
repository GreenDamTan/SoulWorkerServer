// CGocAkashicRecord - Game Object Component for Akashic Record System
// Restored from GameServer.exe IDA decompilation (port 10004)
// Address range: 0x140018B80 - 0x14001D82D

#include "GocAkashicRecord.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerItem.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/XItemFactory.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/XBaseInventory.h"
#include "Soulworker/GameServer/XGameServer/UtilFunc.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <map>

// Forward declarations for external helpers
class CGocInventory;

// Note: GetOwnerMover(), GetOwnerObject(), GetOwnerActor() are inline in header
// Note: GetUseDisassemble() and SetUseDisassemble() are inline in header

// Constructor (0x140018B80)
// IDA: Initializes base class, vtable, all member containers, and zeros quick slot cards
CGocAkashicRecord::CGocAkashicRecord()
    : GOComponent()
    , m_bLoad(false)
    , m_bUserLoad(false)
    , m_bDisassembleAkashic(false)
    , m_byDeckCount(0)
    , m_byActiveDeck(0)
{
    // IDA: std::map default construction for m_mapAkashic, m_mapAkashicPassive
    // IDA: std::vector default construction for m_vecSortedAkashicPassive
    // IDA: std::set default construction for m_setAkashicGetInfo
    // IDA: vector constructor iterator for m_psQuickSlotCard[5]
    memset(m_psQuickSlotCard, 0, sizeof(m_psQuickSlotCard));
}

// Destructor (0x140018C70)
// IDA: Calls destructors in reverse order: set, vector, two maps, then base
CGocAkashicRecord::~CGocAkashicRecord()
{
    // IDA: Clear is not called here - destructors are called directly
    m_setAkashicGetInfo.clear();
    m_vecSortedAkashicPassive.clear();
    m_mapAkashicPassive.clear();
    m_mapAkashic.clear();
}

// Initialize - GOComponent interface
bool CGocAkashicRecord::Initialize()
{
    return Init();
}

// Shutdown - GOComponent interface
void CGocAkashicRecord::Shutdown()
{
    Clear();
}

// Update - GOComponent interface (no-op for this component)
void CGocAkashicRecord::Update(float fDeltaTime)
{
    (void)fDeltaTime;
}

// Init (0x140018CF0)
// IDA: Simply calls Clear and returns true
bool CGocAkashicRecord::Init()
{
    Clear();
    return true;
}

// Clear (0x140018D10)
// IDA: Resets all member variables to initial state
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
}

// Note: GetUseDisassemble and SetUseDisassemble are now inline in header

// SendDBAkashicRecordLoad (0x140018DB0)
// IDA: Sends DB request packet (main=0x21, sub=0x35) to load player's Akashic records
void CGocAkashicRecord::SendDBAkashicRecordLoad()
{
    CMover* pMover = GetOwnerMover();
    if (!pMover) return;

    // Cast to CUser to get UCID
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) return;

    XSendDBPacket xSendDBPacket(pMover, 0x21u, 0x35u);
    xSendDBPacket.XParse << pUser->GetUCID();

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    pServer->SendDBGame(xSendDBPacket);
}

// ResAkashicRecordLoad (0x140018EC0)
// IDA: Handles DB response, loads ST_AKASHIC_LIST into m_mapAkashic
void CGocAkashicRecord::ResAkashicRecordLoad(ST_AKASHIC_LIST& stAkashicList)
{
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    int nCount = static_cast<int>(stAkashicList.vecInfo.size());

    for (int i = 0; i < nCount; ++i)
    {
        ST_AKASHIC_RECORD& stInfo = stAkashicList.vecInfo[i];
        TB_AKASHIC_RECORDS* pTB_Akashic = pServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(stInfo.dwAkashicID);

        if (!pTB_Akashic)
        {
            LogHelper::LogError("game.contents", "ResAkashicRecordLoad error - Failed load akashic list( %d )", 58);
            return;
        }

        ST_AKASHIC_RECORD stAkashicRecord;
        stAkashicRecord.dwAkashicID = stInfo.dwAkashicID;
        stAkashicRecord.nPosition = pTB_Akashic->Array_Index;
        stAkashicRecord.nAkashicExp = stInfo.nAkashicExp;

        m_mapAkashic[stAkashicRecord.nPosition] = stAkashicRecord;
    }

    m_bLoad = true;
}

// SendAkasicRecordList (0x140019000)
// IDA: Sends packet (main=8, sub=0x57) with current akashic list to client
void CGocAkashicRecord::SendAkasicRecordList()
{
    ST_AKASHIC_LIST stAkashicList;

    // Iterate through all stored akashic records
    for (auto it = m_mapAkashic.begin(); it != m_mapAkashic.end(); ++it)
    {
        stAkashicList.vecInfo.push_back(it->second);
    }

    XSendPacket xSendPacket(8u, 0x57u);
    xSendPacket << stAkashicList;

    CMover* pMover = GetOwnerMover();
    if (pMover)
    {
        CGocNetwork::Send(pMover, xSendPacket);
    }
}

// AddAkashicRecord (0x140019170)
// IDA: Adds new akashic record, sends DB packet (main=0x21, sub=0x34) and client response (main=8, sub=0x58)
bool CGocAkashicRecord::AddAkashicRecord(std::uint32_t nItemID, int nAkashicExp)
{
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_AKASHIC_RECORDS* pTBAkashic = pServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(nItemID);
    if (!pTBAkashic)
        return false;

    ST_AKASHIC_RECORD stAkashicInfo;
    stAkashicInfo.dwAkashicID = pTBAkashic->ID;
    stAkashicInfo.nPosition = pTBAkashic->Array_Index;
    stAkashicInfo.nAkashicExp = nAkashicExp;

    m_mapAkashic[stAkashicInfo.nPosition] = stAkashicInfo;

    std::uint8_t byState = 0;

    // Get UCID from owner
    CMover* pMover = GetOwnerMover();
    CUser* pUser = pMover ? dynamic_cast<CUser*>(pMover) : nullptr;
    std::uint32_t dwUCID = pUser ? pUser->GetUCID() : 0;

    // Send DB packet - main=0x21, sub=0x34
    XSendDBPacket xSendDBPacket(pMover, 0x21u, 0x34u);
    PS_DB_AKASHIC_USE psDBAkashicUse;
    psDBAkashicUse.dwUCID = dwUCID;
    psDBAkashicUse.dwAkashicID = stAkashicInfo.dwAkashicID;
    psDBAkashicUse.byState = byState;
    psDBAkashicUse.nAkashicExp = stAkashicInfo.nAkashicExp;
    xSendDBPacket << psDBAkashicUse;

    pServer->SendDBGame(xSendDBPacket);

    // Send packet to client - main=8, sub=0x58
    XSendPacket xSendPacket(8u, 0x58u);
    xSendPacket << stAkashicInfo;
    xSendPacket.XParse << static_cast<std::uint8_t>(1);  // Success flag

    if (pMover)
    {
        CGocNetwork::Send(pMover, xSendPacket);
    }

    // Send DB log
    SendDBLog(2, pTBAkashic->ID, 0, byState, 0);

    // Send statistics - main=0xF0, sub=4
    ST_STATISTICS_AKASHIC stStatistics;
    stStatistics.byFlag = 1;
    stStatistics.dwUCID = dwUCID;
    stStatistics.dwAkashicID = pTBAkashic->ID;

    XSendDBPacket xSendDBStatistics(pMover, 0xF0u, 4u);
    xSendDBStatistics << stStatistics;
    pServer->SendDBStatistics(xSendDBStatistics);

    return true;
}

// Reset (0x14001D0D0)
// IDA: Resets all akashic records, sends DB reset (main=0x21, sub=0x37) and empty list to client
void CGocAkashicRecord::Reset()
{
    CMover* pMover = GetOwnerMover();
    CUser* pUser = pMover ? dynamic_cast<CUser*>(pMover) : nullptr;
    std::uint32_t dwUCID = pUser ? pUser->GetUCID() : 0;

    // Send DB reset packet - main=0x21, sub=0x37
    XSendDBPacket xSendDBPacket(pMover, 0x21u, 0x37u);
    xSendDBPacket.XParse << dwUCID;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    pServer->SendDBGame(xSendDBPacket);

    // Send empty list to client
    ST_AKASHIC_LIST stAkashicList;
    XSendPacket xSendPacket(8u, 0x57u);
    xSendPacket << stAkashicList;

    if (pMover)
    {
        CGocNetwork::Send(pMover, xSendPacket);
    }

    // Clear local data
    m_mapAkashic.clear();

    // Send statistics log
    ST_STATISTICS_AKASHIC stStatistics;
    stStatistics.byFlag = 4;
    stStatistics.dwUCID = dwUCID;

    XSendDBPacket xSendDBStatistics(pMover, 0xF0u, 4u);
    xSendDBStatistics << stStatistics;

    pServer->SendDBStatistics(xSendDBStatistics);
}

// UpdateAkashicPassiveList (0x14001B880)
// IDA: Updates passive akashic card list based on current quick slot configuration
void CGocAkashicRecord::UpdateAkashicPassiveList()
{
    XGameServer* v1 = TXSingleton<XGameServer>::Instance();

    // Clear the passive list
    m_mapAkashicPassive.clear();

    // Check each card slot in active deck
    PS_QUICKSLOT_CARD* pDeck = &m_psQuickSlotCard[m_byActiveDeck];
    for (int i = 0; i < 5; ++i)
    {
        std::uint32_t dwCardID = pDeck->uniCard[i];

        if (!dwCardID)
            continue;

        TB_AKASHIC_RECORDS* pTB_AKashic = v1->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwCardID);
        if (pTB_AKashic && pTB_AKashic->Type == 4)
        {
            // Type 4 = Passive Akashic - store pointer in map
            m_mapAkashicPassive[pTB_AKashic->ID] = pTB_AKashic;
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
            TB_AKASHIC_RECORDS* pAddedAkashicPassive = m_vecSortedAkashicPassive[j];
            if (pAddedAkashicPassive && pAddedAkashicPassive->CoolTime_Group == pAkashicTB->CoolTime_Group)
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
// IDA: Gets passive akashic by cooltime group and index, sorted by ID
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
        // Sort the list by ID (IDA pattern uses std::sort with lambda)
        std::sort(vecSortedAkashicPassive.begin(), vecSortedAkashicPassive.end(),
            [](TB_AKASHIC_RECORDS* a, TB_AKASHIC_RECORDS* b) {
                return a->ID < b->ID;
            });

        return vecSortedAkashicPassive[iIndex];
    }

    return nullptr;
}

// ThinkAkashicPassive (0x14001C640)
// IDA: Processes passive akashic effects (called each update tick for condition 46 = random trigger)
void CGocAkashicRecord::ThinkAkashicPassive()
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (!pUser)
        return;

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
            int Invoke_Rate = pAkashicTB->Invoke_Rate;
            if (Invoke_Rate > std::rand() % 10000)
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
// IDA: Checks and activates passive skills based on condition
bool CGocAkashicRecord::CheckPassiveSkill(std::uint8_t byCondition)
{
    CMover* pMover = GetOwnerMover();
    CUser* pUser = dynamic_cast<CUser*>(pMover);
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

        int Invoke_Rate = pAkashicTB->Invoke_Rate;
        if (Invoke_Rate > std::rand() % 10000)
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
// IDA: Sends akashic record response to client (main=6, sub=0x21)
void CGocAkashicRecord::SendAkasicRecordRes(CUser* pUser, TB_AKASHIC_RECORDS* pAkashicTB)
{
    if (!pUser || !pAkashicTB)
        return;

    PS_RES_AkashicRecord psSendData;
    psSendData.dwAkashicID = pAkashicTB->ID;

    // Get ActorID from user
    UXActorID actorID = pUser->GetActorID();
    psSendData.uxUseActorID = actorID.dwActorID;

    PS_TICKCOUNT_INFO psTick;
    XSendPacket xSendRet(6u, 0x21u);
    xSendRet.XParse << static_cast<std::int32_t>(0);
    xSendRet << psSendData;
    xSendRet << psTick;

    CGocNetwork::Send(static_cast<XActor*>(pUser), xSendRet);
}

// RegisterAllAkashicRecord (0x14001CD60)
// IDA: Registers all akashic records from resource manager and sends to client/DB
void CGocAkashicRecord::RegisterAllAkashicRecord()
{
    ST_AKASHIC_LIST stAkashicList;

    // Get owner user for DB operations
    CMover* pMover = GetOwnerMover();
    CUser* pUser = dynamic_cast<CUser*>(pMover);

    // Iterate through all Akashic records in resource manager
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    auto& mapAkashicRecords = pGameServer->GetResourceMgr().GetAkashicRecordsRows();

    for (auto it = mapAkashicRecords.begin(); it != mapAkashicRecords.end(); ++it)
    {
        TB_AKASHIC_RECORDS& rAkashic = it->second;

        // Check if Akashic_Limit_Type & 8 is NOT set
        if ((rAkashic.Akashic_Limit_Type & 8) == 0)
        {
            ST_AKASHIC_RECORD stAkashicRecord;
            stAkashicRecord.dwAkashicID = rAkashic.ID;
            stAkashicRecord.nPosition = rAkashic.Array_Index;

            m_mapAkashic[stAkashicRecord.nPosition] = stAkashicRecord;
            stAkashicList.vecInfo.push_back(stAkashicRecord);

            // Send statistics
            ST_STATISTICS_AKASHIC stStatistics;
            stStatistics.byFlag = 1;
            stStatistics.dwUCID = pUser ? pUser->GetUCID() : 0;
            stStatistics.dwAkashicID = stAkashicRecord.dwAkashicID;

            XSendDBPacket xSendDBStatistics(pMover, 0xF0u, 4u);
            xSendDBStatistics << stStatistics;
            pGameServer->SendDBStatistics(xSendDBStatistics);
        }
    }

    SendAkasicRecordList();

    // Send DB save packet - main=0x21, sub=0x36
    XSendDBPacket xSendDBPacket(pMover, 0x21u, 0x36u);
    std::uint32_t dwUCID = pUser ? pUser->GetUCID() : 0;
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket << stAkashicList;

    pGameServer->SendDBGame(xSendDBPacket);
}

// LoadQuickSlotCard (0x14001BFC0)
// IDA: Loads quick slot card data from database response
bool CGocAkashicRecord::LoadQuickSlotCard(PS_QUICKSLOT_CARD_VEC& psCardDeck)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();

    // Set deck count from vector size
    if (psCardDeck.vecInfo.size() && psCardDeck.vecInfo.size() <= 5)
    {
        m_byDeckCount = static_cast<std::uint8_t>(psCardDeck.vecInfo.size());
    }
    else
    {
        m_byDeckCount = 0;
    }

    // Set active deck (IDA: LOBYTE check)
    std::uint8_t byActiveDeck = psCardDeck.byActivePage;
    if (byActiveDeck >= 5 || (m_byDeckCount > 0 && byActiveDeck > m_byDeckCount - 1))
        byActiveDeck = 0;
    m_byActiveDeck = byActiveDeck;

    // Process each deck
    for (int k = 0; k < m_byDeckCount; ++k)
    {
        PS_QUICKSLOT_CARD psQuickSlotCard = psCardDeck.vecInfo[k];

        if (psQuickSlotCard.byPage >= 5 || psQuickSlotCard.byPage >= m_byDeckCount)
            continue;

        // Copy card data
        m_psQuickSlotCard[psQuickSlotCard.byPage] = psQuickSlotCard;

        // Validate each card slot - IDA uses std::set for tracking effect groups
        std::set<std::uint32_t> setCardEffectGroups;

        for (int i = 0; i < 5; ++i)
        {
            std::uint32_t dwCardID = psQuickSlotCard.uniCard[i];

            if (!dwCardID)
            {
                m_psQuickSlotCard[k].uniCard[i] = 0;
                continue;
            }

            TB_AKASHIC_RECORDS* pTB_AkashicRecord = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwCardID);
            if (!pTB_AkashicRecord)
            {
                m_psQuickSlotCard[k].uniCard[i] = 0;
                continue;
            }

            // Check if player owns this card (IDA: FindPCAkashic or m_mapAkashic lookup)
            TB_AKASHIC_RECORDS* pTB_AKASHIC_RECORDS = pGameServer->GetResourceMgr().FindPCAkashic(dwCardID);
            bool bHasCard = false;
            if (pTB_AKASHIC_RECORDS)
            {
                bHasCard = true;
            }
            else
            {
                int nKey = pTB_AkashicRecord->Array_Index;
                auto it = m_mapAkashic.find(nKey);
                bHasCard = (it != m_mapAkashic.end());
            }

            if (!bHasCard)
            {
                // Send error message (IDA: 0xCB8D)
                CMover* pMover = GetOwnerMover();
                if (pMover)
                {
                    CGocNetwork::SendErrorMessage(pMover, 8u, 0x28u, 0xCB8Du);
                }
                m_psQuickSlotCard[k].uniCard[i] = 0;
                continue;
            }

            // Check for duplicate effect group
            std::uint32_t dwEffectGroup = pTB_AkashicRecord->Akashic_Effect_Group;
            auto bResult = setCardEffectGroups.insert(dwEffectGroup);
            if (!bResult.second)
            {
                // Duplicate - clear slot
                m_psQuickSlotCard[k].uniCard[i] = 0;
                continue;
            }

            // Copy card to slot
            m_psQuickSlotCard[k].uniCard[i] = dwCardID;
        }
    }

    // Update passive list
    UpdateAkashicPassiveList();

    return true;
}

// ReqDisassembleAkashic (0x140019550)
// IDA: Complex function handling akashic disassembly with cost calculation and item creation
bool CGocAkashicRecord::ReqDisassembleAkashic(std::vector<std::uint32_t>& psList)
{
    CMover* pMover = GetOwnerMover();
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser)
    {
        LogHelper::LogError("game.item", "ReqDisassembleAkashic error - NULL pUser");
        return false;
    }

    // Get UCID
    std::uint32_t dwUCID = pUser->GetUCID();

    // Get inventory component
    std::shared_ptr<CGocInventory> pInvenPtr = pMover->GetGOC_Inventory(false);
    if (!pInvenPtr)
    {
        LogHelper::LogError("game.item", "ReqDisassembleAkashic error - NULL pInvenPtr[UCID:%d]", dwUCID);
        return false;
    }

    if (psList.empty())
    {
        LogHelper::LogError("game.item", "ReqDisassembleAkashic error - Empty disassemble akashic[UCID:%d]", dwUCID);
        return false;
    }

    if (psList.size() > 10)
    {
        LogHelper::LogError("game.item", "ReqDisassembleAkashic error - Max disassemble akashic[UCID:%d, Count:%d]", dwUCID, psList.size());
        return false;
    }

    if (!m_byDeckCount || m_byDeckCount > 5)
    {
        LogHelper::LogError("game.contents", "ReqDisassembleAkashic error - Fault DeckCount[UCID:%d, Deck:%d]", dwUCID, m_byDeckCount);
        return false;
    }

    // Initialize tracking variables
    PS_RES_STORAGE_INFO psUpdateItemList;
    PS_RES_STORAGE_INFO psCreateItemList;
    ST_CREATE_ITEMS stAddItems;
    std::set<std::uint32_t> setCheckDisassemble;

    int nDecMoney = 0;
    int nDecEther = 0;
    int nDecBP = 0;

    // Process each akashic to disassemble
    for (size_t i = 0; i < psList.size(); ++i)
    {
        std::uint32_t dwAkashicID = psList[i];

        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        TB_AKASHIC_RECORDS* pTBAkashicRecord = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwAkashicID);
        if (!pTBAkashicRecord)
        {
            LogHelper::LogError("game.item", "ReqDisassembleAkashic error - No Table TB_AKASHIC_RECORDS[UCID:%d, ID:%d]", dwUCID, dwAkashicID);
            return false;
        }

        int nAkashicKey = pTBAkashicRecord->Array_Index;

        // Check if player owns this akashic
        auto itAkashic = m_mapAkashic.find(nAkashicKey);
        if (itAkashic == m_mapAkashic.end())
        {
            LogHelper::LogError("game.item", "ReqDisassembleAkashic error - Not exist akashic list[UCID:%d, AkashicID:%d]", dwUCID, dwAkashicID);
            return false;
        }

        // Check if already requested disassemble
        if (setCheckDisassemble.find(dwAkashicID) != setCheckDisassemble.end())
        {
            LogHelper::LogError("game.item", "ReqDisassembleAkashic error - Already request disassemble akashic[UCID:%d, AkashicID:%d]", dwUCID, dwAkashicID);
            return false;
        }

        // Check limit type - cannot disassemble if Akashic_Limit_Type & 2
        if ((pTBAkashicRecord->Akashic_Limit_Type & 2) != 0)
        {
            LogHelper::LogError("game.item", "ReqDisassembleAkashic error - Akashic_Limit_Type[UCID:%d, AkashicID:%d]", dwUCID, dwAkashicID);
            return false;
        }

        // Get disassemble table entry
        TB_AKASHIC_DISASSEMBLE* pTB_Disassemble = pGameServer->GetResourceMgr().GetTB_AKASHIC_DISASSEMBLE(pTBAkashicRecord->Akashic_Disassemble);
        if (!pTB_Disassemble)
        {
            LogHelper::LogError("game.item", "ReqDisassembleAkashic error - No Table TB_AKASHIC_DISASSEMBLE[UCID:%d, ID:%d]", dwUCID, dwAkashicID);
            return false;
        }

        // Get disassemble result item
        TB_ITEM* pTB_Item = pGameServer->GetResourceMgr().GetTB_ITEM(pTB_Disassemble->Dis_Item);
        if (!pTB_Item)
        {
            LogHelper::LogError("game.item", "ReqDisassembleAkashic error - No Table TB_ITEM[UCID:%d, ID:%d]", dwUCID, dwAkashicID);
            return false;
        }

        // Get item classify for inventory type
        TB_ITEM_CLASSIFY* pTB_ItemClassify = pGameServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTB_Item->Item_Classify_Index);
        if (!pTB_ItemClassify)
        {
            LogHelper::LogError("game.item", "ReqDisassembleAkashic error - No Table TB_ITEM_CLASSIFY[UCID:%d, ID:%d]", dwUCID, dwAkashicID);
            return false;
        }

        // Get inventory pointer
        CGocInventory* pInven = pInvenPtr.get();
        XBaseInventory* pBaseInven = pInven->GetTBInvenPtr(pTB_ItemClassify->Item_Inven_Type);
        if (!pBaseInven)
        {
            LogHelper::LogError("game.item", "ReqDisassembleAkashic error - NULL pInven[UCID:%d, ID:%d]", dwUCID, dwAkashicID);
            return false;
        }

        // Accumulate cost based on Dis_CostType
        // 0 = Money, 1 = BP, 2 = Ether
        std::uint8_t Dis_CostType = pTB_Disassemble->Dis_CostType;
        if (Dis_CostType == 0)
        {
            nDecMoney += pTB_Disassemble->Cost_Value;
        }
        else if (Dis_CostType == 1)
        {
            nDecBP += pTB_Disassemble->Cost_Value;
        }
        else if (Dis_CostType == 2)
        {
            nDecEther += pTB_Disassemble->Cost_Value;
        }
        else
        {
            LogHelper::LogError("game.item", "ReqDisassembleAkashic error - Dis_CostType [UCID:%d, ID:%d]", dwUCID, dwAkashicID);
            return false;
        }

        // Check if akashic has exp (nAkashicExp > 0)
        int nAkashicExp = itAkashic->second.nAkashicExp;
        if (nAkashicExp <= 0)
        {
            // No exp - add to simple create list
            ST_CREATE_ITEM stAddItem;
            stAddItem.nItemID = pTB_Item->Item_ID;
            stAddItem.shCount = 1;
            stAddItems.vecInfo.push_back(stAddItem);
        }
        else
        {
            // Has exp - need to create item in empty slot
            std::int16_t shSlotPos = pBaseInven->GetEmptySlot_2();
            if (shSlotPos == -1)
            {
                CMover* pMover = GetOwnerMover();
                CGocNetwork::SendErrorMessage(pMover, 8u, 0x15u, 0xCB2Au);
                LogHelper::LogError("game.item", "ReqDisassembleAkashic error - Not enough inventory -1[ ActorID:%d, AkashicID:%d ] ( %d )", dwUCID, dwAkashicID, 273);
                return false;
            }

            // Create item with exp
            STItem stCreateItem;
            stCreateItem.nItemID = pTB_Item->Item_ID;
            stCreateItem.sCount = 1;
            stCreateItem.nExp = nAkashicExp;
            if (pTB_Item->Item_Bind_Type == 2)
                stCreateItem.bBindType = 1;

            XGameServer* pGS = TXSingleton<XGameServer>::Instance();
            pGS->GetItemFactory().CreateItem(stCreateItem, stCreateItem);

            std::uint8_t byInvenType = pBaseInven->GetInvenType();
            pInvenPtr.get()->AddItem(byInvenType, shSlotPos, stCreateItem, true);

            // Build storage info for response
            PS_STORAGE_INFO psStorageInfo;
            psStorageInfo.byInvenType = byInvenType;
            psStorageInfo.shSlotPos = shSlotPos;
            psStorageInfo.stItem = stCreateItem;
            psCreateItemList.vecItem.push_back(psStorageInfo);

            // Lock the slot
            pInvenPtr.get()->SetLock(byInvenType, shSlotPos, 0x18u);
        }

        // Add to check set
        setCheckDisassemble.insert(dwAkashicID);
    }

    // Validate resources
    __int64 biMoney = nDecMoney;
    CGocInventory* pInven = pInvenPtr.get();
    if (!pInven->IsUseMoney(biMoney))
    {
        return false;
    }

    __int64 nBP = pInven->GetBP();
    if (nDecBP > nBP)
    {
        return false;
    }

    __int64 biEther = pInven->GetEther();
    if (nDecEther > biEther)
    {
        return false;
    }

    // Create items
    ST_LOG_GAME stLog;
    stLog._sSubType = 55;
    if (!pInven->CreateItem2(stAddItems, 0x18u, false,
                             psCreateItemList, psUpdateItemList, stLog))
    {
        CMover* pMover = GetOwnerMover();
        CGocNetwork::SendErrorMessage(pMover, 8u, 0x15u, 0xCB2Au);
        LogHelper::LogError("game.item", "ReqDisassembleAkashic error - Faild CreateItem2[ Actor:%d ] ( %d )", dwUCID, 328);
        return false;
    }

    // Send DB packet - main=0x81, sub=0x14
    PS_DB_AKASHIC_DISASSEMBLE psDBDisassemble;
    psDBDisassemble.dwUCID = dwUCID;
    psDBDisassemble.byState = 1;
    psDBDisassemble.byFlag = 20;
    psDBDisassemble.psList = psList;
    psDBDisassemble.psCreateItemList = psCreateItemList;
    psDBDisassemble.psUpdateItemList = psUpdateItemList;

    IXObject* pObject = pUser ? static_cast<IXObject*>(static_cast<XActor*>(pUser)) : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x81u, 0x14u);
    xSendDBPacket << psDBDisassemble;
    TXSingleton<XGameServer>::Instance()->SendDBGame(xSendDBPacket);

    // Deduct costs
    if (nDecMoney > 0)
    {
        pInven->AddBindMoney(-nDecMoney, 0x1Fu, 0, 0, 0);
    }
    else if (nDecEther > 0)
    {
        pInven->AddEther(-nDecEther, 0x1Fu, 1);
    }
    else if (nDecBP > 0)
    {
        pInven->AddBP(-nDecBP, 0x1Fu);
    }

    // Clear cards from quickslot
    for (size_t j = 0; j < psList.size(); ++j)
    {
        DisassembleQuickSlotCard(psList[j]);
    }

    return true;
}

// ResDisassembleAkashic (0x14001A920)
// IDA: Handles disassembly response from database
bool CGocAkashicRecord::ResDisassembleAkashic(std::vector<std::uint32_t>& psList, std::uint8_t byState)
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (!pUser)
        return false;

    // Get inventory component
    std::shared_ptr<CGocInventory> pInvenPtr = dynamic_cast<CUser*>(GetOwnerMover())->GetGOC_Inventory(false);
    if (!pInvenPtr)
        return false;

    ST_AKASHIC_LIST stDisassembleList;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();

    // Process each disassembled akashic
    for (size_t i = 0; i < psList.size(); ++i)
    {
        std::uint32_t dwAkashicID = psList[i];

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
        stStatistics.dwUCID = pUser->GetUCID();
        stStatistics.dwAkashicID = dwAkashicID;

        IXObject* pObject = pUser ? static_cast<IXObject*>(static_cast<XActor*>(pUser)) : nullptr;

        XSendDBPacket xSendDBStatistics(pObject, 0xF0u, 4u);
        xSendDBStatistics << stStatistics;
        TXSingleton<XGameServer>::Instance()->SendDBStatistics(xSendDBStatistics);

        // Send log
        SendDBLog(3, dwAkashicID, 0, byState, 0);
    }

    // NOTE: PS_RES_STORAGE_INFO from DB response would be passed as additional parameters
    // For now, the DB packet 0x81 0x14 sends the create/update lists separately
    // CGocInventory::SendCreateItem and SendUpdateItem will be called by packet handler;

    // Send disassemble result to client
    XSendPacket xSendPacket(0x18u, 0x15u);
    xSendPacket.XParse << static_cast<std::uint8_t>(1);
    xSendPacket << stDisassembleList;

    XActor* pActor = static_cast<XActor*>(pUser);
    if (pActor)
    {
        CGocNetwork::Send(pActor, xSendPacket);
    }

    return true;
}

// SendDBLog (0x14001D390)
// IDA: Sends log entry to database (main=21)
void CGocAkashicRecord::SendDBLog(std::int16_t shSubType, int nParam0, int nParam1, int nParam4, int nParam3)
{
    ST_LOG_GAME stLog;
    stLog._sMainType = 21;
    stLog._sSubType = shSubType;

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (pUser)
    {
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = pUser->GetUCID();
    }

    stLog.nParam0 = nParam0;
    stLog.nParam1 = nParam1;

    CMover* pMover = GetOwnerMover();
    stLog.nParam2 = pMover ? static_cast<std::uint8_t>(pMover->GetActorID().dwActorID & 0xFF) : 0;

    stLog.nParam3 = nParam3;
    stLog.nParam4 = nParam4;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    pGameServer->SendDBLog(stLog);
}

// AddAkashicGetInfo (0x14001D500)
// IDA: Adds akashic get info and sends to DB if new
void CGocAkashicRecord::AddAkashicGetInfo(std::uint32_t dwAkashicID)
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (!pUser)
        return;

    // Get ActorID for UCID
    std::uint32_t dwUCID = pUser->GetUCID();

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pTB_AkashicItem = pGameServer->GetResourceMgr().GetTB_ITEM(dwAkashicID);
    if (!pTB_AkashicItem)
        return;

    // Check if this is an Akashic item (Item_Effect_Type == 8)
    if (pTB_AkashicItem->Item_Effect_Type != 8)
        return;

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
    IXObject* pObject = pUser ? static_cast<IXObject*>(static_cast<XActor*>(pUser)) : nullptr;

    PS_DB_AKASHIC_GETINFO psDBAkashicGetInfo;
    psDBAkashicGetInfo.dwUCID = dwUCID;
    psDBAkashicGetInfo.dwAkashicGroupID = dwAkashicGroupID;

    XSendDBPacket xSendDBPacket(pObject, 0x81u, 0x31u);
    xSendDBPacket << psDBAkashicGetInfo;
    pGameServer->SendDBGame(xSendDBPacket);

    // Send to client - main=0x18, sub=0x34
    XSendPacket xSendPacket(0x18u, 0x34u);
    xSendPacket.XParse << dwAkashicGroupID;

    XActor* pActor = static_cast<XActor*>(pUser);
    if (pActor)
    {
        CGocNetwork::Send(pActor, xSendPacket);
    }
}

// SetQuickSlotCard
bool CGocAkashicRecord::SetQuickSlotCard(PS_QUICKSLOT_UPDATE_CARD_VEC& psUpdate)
{
    if (m_mapAkashic.empty() || psUpdate.vecInfo.size() > m_byDeckCount ||
        psUpdate.vecInfo.size() != 1)
        return false;

    const PS_QUICKSLOT_UPDATE_CARD& psUpdateCardInfo = psUpdate.vecInfo.front();
    if (psUpdateCardInfo.byPage >= 5 ||
        psUpdateCardInfo.byPage >= m_byDeckCount ||
        !IsOverlapCard(const_cast<std::uint32_t*>(psUpdateCardInfo.uniCard)))
        return false;

    PS_QUICKSLOT_CARD psQuickSlotCard =
        m_psQuickSlotCard[psUpdateCardInfo.byPage];
    bool bChange = false;
    for (int i = 0; i < 5; ++i) {
        if (psQuickSlotCard.uniCard[i] != psUpdateCardInfo.uniCard[i]) {
            if (m_byActiveDeck == psUpdateCardInfo.byPage)
                RemoveExistBuff(psQuickSlotCard.uniCard[i]);
            psQuickSlotCard.uniCard[i] = psUpdateCardInfo.uniCard[i];
            bChange = true;
        }
    }

    if (bChange) {
        m_psQuickSlotCard[psUpdateCardInfo.byPage] = psQuickSlotCard;
        SaveQuickSlot(psUpdateCardInfo.byPage);
    }

    XSendPacket xSendPacket(8, 0x27);
    xSendPacket.XParse << static_cast<std::uint8_t>(0);
    xSendPacket << psUpdate;
    CGocNetwork::Send(GetOwnerActor(), xSendPacket);
    UpdateAkashicPassiveList();
    return true;
}

// GetQuickSlotCard (0x14001AF40)
void CGocAkashicRecord::GetQuickSlotCard(PS_QUICKSLOT_UPDATE_CARD_VEC& psInfo)
{
    psInfo.vecInfo.clear();
    for (std::uint8_t i = 0; i < m_byDeckCount; ++i) {
        PS_QUICKSLOT_UPDATE_CARD psUpdate;
        psUpdate.byPage = m_psQuickSlotCard[i].byPage;
        for (int k = 0; k < 5; ++k)
            psUpdate.uniCard[k] = m_psQuickSlotCard[i].uniCard[k];
        psInfo.vecInfo.push_back(psUpdate);
    }
}

// GetQuickSlotCard (0x14001CCB0)
// IDA: Gets 5 card slots from active deck into array
void CGocAkashicRecord::GetQuickSlotCard(std::uint32_t* pQuickSlotCard)
{
    // IDA: Validate active deck bounds
    if (m_byActiveDeck >= 5u || m_byActiveDeck > m_byDeckCount - 1)
        m_byActiveDeck = 0;

    // IDA: Copy 5 card slots from active deck
    for (int i = 0; i < 5; ++i)
    {
        pQuickSlotCard[i] = m_psQuickSlotCard[m_byActiveDeck].uniCard[i];
    }
}

// GetQuickSlotInfo (0x14001C5C0)
// IDA: Fills card vector with active deck page and all decks, then refreshes passive list
void CGocAkashicRecord::GetQuickSlotInfo(PS_QUICKSLOT_CARD_VEC& psQuickSlotCard)
{
    // IDA: psQuickSlotCard->byActivePage = this->m_byActiveDeck;
    psQuickSlotCard.byActivePage = m_byActiveDeck;

    // IDA: for (i = 0; i < this->m_byDeckCount; ++i)
    //          std::vector<PS_QUICKSLOT_CARD>::push_back(&psQuickSlotCard->vecInfo, &this->m_psQuickSlotCard[i]);
    for (std::uint8_t i = 0; i < m_byDeckCount; ++i) {
        psQuickSlotCard.vecInfo.push_back(m_psQuickSlotCard[i]);
    }

    // IDA: CGocAkashicRecord::UpdateAkashicPassiveList(this);
    UpdateAkashicPassiveList();
}

// ChangeActiveDeck (0x140020910)
// IDA: Changes active deck, validates, sends DB update and client response
int CGocAkashicRecord::ChangeActiveDeck(PS_DECK_ACTIVE& stActive)
{
    // Get UCID from owner
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    std::uint32_t dwUCID = pUser ? pUser->GetUCID() : 0;

    // Check if already active
    if (m_byActiveDeck == stActive.byActivePage)
    {
        LogHelper::LogError("game.contents", "ChangeActiveDeck error - Active[UCID:%d]", dwUCID);
        return 58413;
    }

    // Validate deck index
    if (stActive.byActivePage >= 5u || stActive.byActivePage > m_byDeckCount - 1)
    {
        LogHelper::LogError("game.contents",
            "ChangeActiveDeck error - Over deck count[UCID:%d, Req:%d, Count:%d]",
            dwUCID, stActive.byActivePage, m_byDeckCount);
        return 58413;
    }

    // Remove buffs from cards that are different between old and new deck
    for (int k = 0; k < 5; ++k)
    {
        std::uint32_t dwOldCard = m_psQuickSlotCard[m_byActiveDeck].uniCard[k];
        std::uint32_t dwNewCard = m_psQuickSlotCard[stActive.byActivePage].uniCard[k];
        if (dwOldCard != dwNewCard)
        {
            RemoveExistBuff(dwOldCard);
        }
    }

    // Update active deck
    m_byActiveDeck = stActive.byActivePage;

    // Send DB update packet - main=0x44, sub=0x13
    IXObject* pObject = pUser ? static_cast<IXObject*>(static_cast<XActor*>(pUser)) : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x44u, 0x13u);
    xSendDBPacket.XParse << static_cast<int>(dwUCID);
    xSendDBPacket << stActive;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    pGameServer->SendDBGame(xSendDBPacket);

    // Send client response - main=6, sub=0x54
    int nResult = 0;
    XSendPacket xSendPacket(6u, 0x54u);
    xSendPacket.XParse << nResult;
    xSendPacket << stActive;

    XActor* pActor = pUser ? static_cast<XActor*>(pUser) : nullptr;
    if (pActor)
    {
        CGocNetwork::Send(pActor, xSendPacket);
    }

    // Update passive list
    UpdateAkashicPassiveList();

    return 0;
}

// ChangeDeckName (0x140020ca0)
// IDA: Changes deck name, validates name, sends DB update
int CGocAkashicRecord::ChangeDeckName(PS_DECK_NAME_VEC& stChange)
{
    // Get UCID from owner
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    std::uint32_t dwUCID = pUser ? pUser->GetUCID() : 0;

    // Validate deck count
    if (stChange.vecInfo.size() > m_byDeckCount)
    {
        LogHelper::LogError("game.contents", "ChangeDeckName error - Over deck count[UCID:%d, Count:%d]", dwUCID, m_byDeckCount);
        return 58412;
    }

    // Process each deck name
    for (size_t i = 0; i < stChange.vecInfo.size(); ++i)
    {
        PS_DECK_NAME& psInfo = stChange.vecInfo[i];

        // Validate deck page
        if (psInfo.byDeckPage > m_byDeckCount - 1)
        {
            LogHelper::LogError("game.contents", "ChangeDeckName error - Fault deck page[UCID:%d, Count:%d]", dwUCID, m_byDeckCount);
            return 58412;
        }

        // Validate name length (must be 2-13 characters)
        size_t nLen = wcslen(psInfo.szDeckName);
        if (nLen < 2 || nLen > 13)
        {
            LogHelper::LogError("game.contents", "ChangeDeckName error - Fault Name size[UCID:%d]", dwUCID);
            return 58412;
        }

        // Check name filter
        // Per IDA 0x140020CA0: UtilFunc::IsUsableNameFilter(psInfo.szDeckName)
        if (!UtilFunc::IsUsableNameFilter(psInfo.szDeckName))
        {
            LogHelper::LogError("game.contents", "ChangeDeckName error - IsUsableNameFilter[UCID:%d]", dwUCID);
            return 58412;
        }

        // Update deck name
        wcscpy_s(m_psQuickSlotCard[psInfo.byDeckPage].szDeckName, psInfo.szDeckName);
    }

    // Send DB update packet - main=0x44, sub=0x14
    IXObject* pObject = pUser ? static_cast<IXObject*>(static_cast<XActor*>(pUser)) : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x44u, 0x14u);
    xSendDBPacket.XParse << static_cast<int>(dwUCID);
    xSendDBPacket << stChange;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    pGameServer->SendDBGame(xSendDBPacket);

    // Send client response - main=6, sub=0x55
    PS_RES_DECK_NAME psRes;
    psRes.vecName = stChange;
    psRes.nResult = 0;

    XSendPacket xSendPacket(6u, 0x55u);
    xSendPacket << psRes;

    XActor* pActor = pUser ? static_cast<XActor*>(pUser) : nullptr;
    if (pActor)
    {
        CGocNetwork::Send(pActor, xSendPacket);
    }

    return 0;
}

// IsOverlapCard (0x14001fc30)
// IDA: Checks if cards have overlapping effect groups
bool CGocAkashicRecord::IsOverlapCard(std::uint32_t* uniCard)
{
    std::set<std::uint32_t> setCard;

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    std::uint32_t dwUCID = pUser ? pUser->GetUCID() : 0;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    for (int i = 0; i < 5; ++i)
    {
        if (uniCard[i])
        {
            TB_AKASHIC_RECORDS* pTB_AkashicRecords = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(uniCard[i]);
            if (!pTB_AkashicRecords)
            {
                return false;
            }

            std::uint32_t dwEffectGroup = pTB_AkashicRecords->Akashic_Effect_Group;
            auto bResult = setCard.insert(dwEffectGroup);
            if (!bResult.second)
            {
                // Duplicate effect group found
                LogHelper::LogError("game.contents", "IsOverlapCard error - [UCID:%d, ID:%d, GroupID:%d]",
                    dwUCID, pTB_AkashicRecords->ID, pTB_AkashicRecords->Akashic_Effect_Group);
                return false;
            }
        }
    }

    return true;
}

// OpenCardDeck (0x1400212d0)
// IDA: Opens new card deck slot
int CGocAkashicRecord::OpenCardDeck()
{
    // Get inventory component
    CMover* v1 = GetOwnerMover();
    std::shared_ptr<CGocInventory> pInvenPtr;
    v1->GetGOC<CGocInventory>(&pInvenPtr, 0);
    if (!pInvenPtr)
    {
        return 58411;
    }

    // Get UCID
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    std::uint32_t dwUCID = pUser ? pUser->GetUCID() : 0;

    std::uint8_t byExtendDeck = m_byDeckCount + 1;

    // Check max deck limit
    if (byExtendDeck > 5)
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - Max card deck[UCID:%d, DeckCount:%d]", dwUCID, m_byDeckCount);
        return 58411;
    }

    std::uint8_t byOpenPage = m_byDeckCount;
    if (byOpenPage == 0)
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - Fault deck[UCID:%d, DeckCount:%d]", dwUCID, m_byDeckCount);
        return 58411;
    }

    // Get extend table
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_AKASHIC_SLOT_EXTEND* pTB_Extend = pGameServer->GetResourceMgr().GetTB_AKASHIC_SLOT_EXTEND(byExtendDeck);
    if (!pTB_Extend)
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - No Table TB_AKASHIC_SLOT_EXTEND[UCID:%d]", dwUCID);
        return 52004;
    }

    // Get required item
    TB_ITEM* pTB_Item = pGameServer->GetResourceMgr().GetTB_ITEM(pTB_Extend->Extend_Akashic_Need_Item_ID);
    if (!pTB_Item)
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - No Table TB_ITEM[UCID:%d, ItemID:%d]", dwUCID, pTB_Extend->Extend_Akashic_Need_Item_ID);
        return 52004;
    }

    // Get item classify
    TB_ITEM_CLASSIFY* pTB_Item_Classify = pGameServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTB_Item->Item_Classify_Index);
    if (!pTB_Item_Classify)
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - No Table TB_ITEM_CLASSIFY[UCID:%d, ItemID:%d]", dwUCID, pTB_Extend->Extend_Akashic_Need_Item_ID);
        return 52004;
    }

    // Check item use type
    if (pTB_Item_Classify->Item_Use_Type != 123)
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - Item UseType[UCID:%d, ItemID:%d]", dwUCID, pTB_Extend->Extend_Akashic_Need_Item_ID);
        return 52011;
    }

    // Reduce items
    PS_RES_STORAGE_INFO psUpdateItemList;
    CGocInventory* pInven = pInvenPtr.get();
    std::int16_t shNeedCount = pTB_Extend->Extend_Akashic_Need_Item_Num;

    if (!pInven->ReduceItem2(pTB_Item, shNeedCount, 0x88u, psUpdateItemList))
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - Failed ReduceItem2[UCID:%d]", dwUCID);
        return 58414;
    }

    // Update item end
    ST_LOG_GAME stLog;
    stLog._sSubType = 137;
    stLog.nParam3 = 1;
    stLog.nParam7 = byExtendDeck;

    if (!pInven->UpdateItemEnd(0x88u, psUpdateItemList, stLog))
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - Failed UpdateItemEnd[UCID:%d]", dwUCID);
        pInven->UnLockList(psUpdateItemList);
        return 58414;
    }

    // Send DB packet - main=0x21, sub=0x57
    PS_DB_CARD_DECK_OPEN psDBDeck;
    psDBDeck.dwUCID = dwUCID;
    psDBDeck.psUpdateItemList = psUpdateItemList;
    psDBDeck.byCardDeckCount = byExtendDeck;
    psDBDeck.psCardDeck.byPage = byOpenPage;

    IXObject* pObject = pUser ? static_cast<IXObject*>(static_cast<XActor*>(pUser)) : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x21u, 0x57u);
    xSendDBPacket << psDBDeck;
    TXSingleton<XGameServer>::Instance()->SendDBGame(xSendDBPacket);

    return 0;
}

// IsCombineAkashic (0x14001daf0)
// IDA: Complex function for akashic combination with success rate calculation
int CGocAkashicRecord::IsCombineAkashic(PS_ITEM_SLOT_INFO& psMainInfo, PS_ITEM_SLOT_INFOS& psNeedInfos,
                                         PS_RES_STORAGE_INFO& psCreateItemList, PS_RES_STORAGE_INFO& psUpdateItemList,
                                         std::uint8_t& bySuccess, int& nCreateAkashicID)
{
    // Clear output
    psCreateItemList.vecItem.clear();
    psUpdateItemList.vecItem.clear();
    bySuccess = 0;
    nCreateAkashicID = 0;

    // Get owner mover and cast to CUser
    CMover* pMover = GetOwnerMover();
    if (!pMover) return 52522;

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) return 52522;

    std::uint32_t dwUCID = pUser->GetUCID();

    // Get inventory component
    std::shared_ptr<CGocInventory> pInvenPtr = pMover->GetGOC_Inventory(false);
    if (!pInvenPtr)
    {
        return 52522;
    }

    // Validate main item inventory type (must be 2 or 13)
    if (psMainInfo.byInvenType != 2 && psMainInfo.byInvenType != 13)
    {
        LogHelper::LogError("game.item", "IsCombineAkashic error - Fault main item inventory[UCID:%d, InvenType:%d]", dwUCID, psMainInfo.byInvenType);
        return 52011;
    }

    // Get main akashic item
    bool bLock = false;
    std::shared_ptr<CItem> pMainAkashicItemPtr = pInvenPtr->GetSlotItem(psMainInfo.byInvenType, psMainInfo.shSlotPos, bLock);
    if (!pMainAkashicItemPtr || bLock)
    {
        LogHelper::LogError("game.item", "IsCombineAkashic error - Wrong main item[UCID:%d, InvenType:%d, Slot:%d]",
            dwUCID, psMainInfo.byInvenType, psMainInfo.shSlotPos);
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        pServer->SendItemLockLog(dwUCID, psMainInfo.byInvenType, psMainInfo.shSlotPos, bLock ? 1 : 0, 61, 0);
        return 52011;
    }

    // Get main akashic item ID
    CItem* pMainItem = pMainAkashicItemPtr.get();
    std::uint32_t dwMainID = static_cast<std::uint32_t>(pMainItem->GetID());

    // Get TB_AKASHIC_RECORDS for main item
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_AKASHIC_RECORDS* pTB_AkashicRecords = pServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwMainID);
    if (!pTB_AkashicRecords)
    {
        LogHelper::LogError("game.item", "IsCombineAkashic error - No Table TB_AKASHIC_RECORDS[UCID:%d, ID:%d]", dwUCID, dwMainID);
        return 52522;
    }

    // Check rare point limit
    if (pTB_AkashicRecords->Rare_Point >= 6u)
    {
        LogHelper::LogError("game.item", "IsCombineAkashic error - Rare point[UCID:%d, ID:%d]", dwUCID, dwMainID);
        return 52522;
    }

    // Get combination info
    int nCombineID = pTB_AkashicRecords->Rare_Point + 1;
    TB_AKASHIC_COMBINATION* pTB_AkashicCombine = pServer->GetResourceMgr().GetTB_AKASHIC_COMBINATION(static_cast<std::uint8_t>(nCombineID));
    if (!pTB_AkashicCombine)
    {
        LogHelper::LogError("game.item", "IsCombineAkashic error - No Table TB_AKASHIC_COMBINATION[UCID%d, ID:%d]", dwUCID, pTB_AkashicRecords->ID);
        return 52522;
    }

    // Check money
    std::int64_t biNeedMoney = pTB_AkashicCombine->Combination_Need_Zeny;
    std::int64_t biMoney = pInvenPtr->GetMoney();
    if (biMoney < biNeedMoney)
    {
        LogHelper::LogError("game.item", "IsCombineAkashic error - Shortage money[UCID:%d]", dwUCID);
        return 52081;
    }

    // Build reduce item map and calculate points
    std::map<std::pair<std::uint8_t, std::int16_t>, ST_CREATE_ITEM> mpReduceItems;
    std::uint32_t dwPoint = 0;

    for (std::size_t i = 0; i < psNeedInfos.vecInfo.size(); ++i)
    {
        const PS_ITEM_SLOT_INFO& psInfo = psNeedInfos.vecInfo[i];

        // Get need item
        bLock = false;
        std::shared_ptr<CItem> pNeedItemPtr = pInvenPtr->GetSlotItem(psInfo.byInvenType, psInfo.shSlotPos, bLock);
        if (!pNeedItemPtr || bLock)
        {
            LogHelper::LogError("game.item", "IsCombineAkashic error - Wrong need item[UCID:%d, InvenType:%d, Slot:%d]",
                dwUCID, psInfo.byInvenType, psInfo.shSlotPos);
            return 52011;
        }

        CItem* pNeedItem = pNeedItemPtr.get();
        std::uint8_t byNeedInvenType = pNeedItem->GetInvenType();
        if (byNeedInvenType != 2 && byNeedInvenType != 13)
        {
            LogHelper::LogError("game.item", "IsCombineAkashic error - Fault material item inventory[UCID%d, InvenType:%d]",
                dwUCID, psInfo.byInvenType);
            return 52011;
        }

        // Get TB_AKASHIC_RECORDS for need item
        std::uint32_t dwNeedID = static_cast<std::uint32_t>(pNeedItem->GetID());
        TB_AKASHIC_RECORDS* pTB_NeedAkashic = pServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwNeedID);
        if (!pTB_NeedAkashic)
        {
            LogHelper::LogError("game.item", "IsCombineAkashic error - No Table TB_AKASHIC_RECORDS[UCID%d, ID:%d]", dwUCID, dwNeedID);
            return 52522;
        }

        // Add to reduce map
        auto key = std::make_pair(psInfo.byInvenType, psInfo.shSlotPos);
        auto it = mpReduceItems.find(key);
        if (it != mpReduceItems.end())
        {
            it->second.shCount++;
        }
        else
        {
            ST_CREATE_ITEM stReduceItem;
            stReduceItem.nItemID = static_cast<std::int32_t>(dwNeedID);
            stReduceItem.shCount = 1;
            mpReduceItems[key] = stReduceItem;
        }

        dwPoint += pTB_NeedAkashic->Akashic_Combination_Point;
    }

    // Calculate success rate
    float fNeedPoint = static_cast<float>(pTB_AkashicCombine->Combination_Need_Point);
    if (fNeedPoint == 0.0f)
    {
        LogHelper::LogError("game.item", "IsCombineAkashic error - NeedPoint 0[UCID%d, Index:%d]",
            dwUCID, pTB_AkashicRecords->Rare_Point);
        return 52522;
    }

    // Clamp point
    if (static_cast<float>(dwPoint) > fNeedPoint)
        dwPoint = static_cast<std::uint32_t>(fNeedPoint);

    // Calculate rate: (point/needPoint)^3 * successRate
    float fRate = static_cast<float>(dwPoint) / fNeedPoint;
    fRate = std::pow(fRate, 3);
    int nSuccessRate = static_cast<int>(static_cast<float>(pTB_AkashicCombine->Combination_Success_Rate) * fRate);

    // Random check
    int nRand = pServer->GetItemFactory().nRand(1, 10000);
    int nRate = nRand - nSuccessRate;

    // Build create items
    ST_CREATE_ITEMS stCreateItems;

    if (nRate > 0)
    {
        // Failure
        bySuccess = 1;
    }
    else
    {
        // Success - get random group
        TB_AKASHIC_RANDOM_GROUP* pTB_AkashicCombineGroup = pServer->GetResourceMgr().GetTB_AKASHIC_RANDOM_GROUP(
            pTB_AkashicCombine->Success_Group);
        if (!pTB_AkashicCombineGroup)
        {
            LogHelper::LogError("game.item", "IsCombineAkashic error - No Table TB_AKASHIC_RANDOM_GROUP[UCID%d, Index:%d]",
                dwUCID, pTB_AkashicCombine->Success_Group);
            return 52522;
        }

        // Select group
        int nCombineGroupRate = pServer->GetItemFactory().nRand(1, 10000);
        int nCombineGroupID = 0;

        for (int j = 0; j < 10; ++j)
        {
            if (pTB_AkashicCombineGroup->uniAkashic_Group[j] == 0)
            {
                LogHelper::LogError("game.item", "IsCombineAkashic error - Group ID 0[UCID%d, Index:%d]",
                    dwUCID, pTB_AkashicCombine->Success_Group);
                return 52522;
            }

            nCombineGroupRate -= pTB_AkashicCombineGroup->uniChance[j];
            if (nCombineGroupRate <= 0)
            {
                nCombineGroupID = pTB_AkashicCombineGroup->uniAkashic_Group[j];
                break;
            }
        }

        // Get group in
        TB_AKASHIC_RANDOM_GROUP_IN* pTB_AkashicCombineGroupIn = pServer->GetResourceMgr().GetTB_AKASHIC_RANDOM_GROUP_IN(
            nCombineGroupID);
        if (!pTB_AkashicCombineGroupIn)
        {
            LogHelper::LogError("game.item", "IsCombineAkashic error - No Table TB_AKASHIC_RANDOM_GROUP_IN[UCID%d, Index:%d]",
                dwUCID, nCombineGroupID);
            return 52522;
        }

        // Select record
        nRate = pServer->GetItemFactory().nRand(1, 10000);
        for (int k = 0; k < 10; ++k)
        {
            if (pTB_AkashicCombineGroupIn->uniAkashic_Record_ID[k] == 0)
            {
                LogHelper::LogError("game.item", "IsCombineAkashic error - GroupIn ID 0[UCID%d, Index:%d]",
                    dwUCID, pTB_AkashicCombineGroupIn->Akashic_Group_ID);
                return 52522;
            }

            nRate -= pTB_AkashicCombineGroupIn->unichance[k];
            if (nRate <= 0)
            {
                ST_CREATE_ITEM stCreateItem;
                stCreateItem.nItemID = static_cast<std::int32_t>(pTB_AkashicCombineGroupIn->uniAkashic_Record_ID[k]);
                stCreateItem.shCount = 1;
                stCreateItems.vecInfo.push_back(stCreateItem);
                nCreateAkashicID = static_cast<int>(pTB_AkashicCombineGroupIn->uniAkashic_Record_ID[k]);

                // Add main item to reduce list
                std::int16_t shMainSlot = pMainItem->GetSlot();
                std::uint8_t byMainInven = pMainItem->GetInvenType();
                auto mainKey = std::make_pair(byMainInven, shMainSlot);
                auto mainIt = mpReduceItems.find(mainKey);
                if (mainIt != mpReduceItems.end())
                {
                    mainIt->second.shCount++;
                }
                else
                {
                    ST_CREATE_ITEM stMainReduce;
                    stMainReduce.nItemID = static_cast<std::int32_t>(dwMainID);
                    stMainReduce.shCount = 1;
                    mpReduceItems[mainKey] = stMainReduce;
                }
                break;
            }
        }
    }

    // Reduce items
    for (const auto& pair : mpReduceItems)
    {
        std::uint8_t byInvenType = pair.first.first;
        std::int16_t shSlotPos = pair.first.second;
        std::int16_t shCount = pair.second.shCount;

        if (!pInvenPtr->ReduceItem3(byInvenType, shSlotPos, shCount, 0x7Cu, psUpdateItemList))
        {
            LogHelper::LogError("game.item", "IsCombineAkashic error - ReduceItem3 [ inven:%d, pos:%d, count:%d ]",
                byInvenType, shSlotPos, static_cast<int>(shCount));
            return 52522;
        }
    }

    // Create log
    ST_LOG_GAME stLog;
    stLog._sSubType = 120;
    stLog.nParam3 = 1;
    stLog.nParam6 = static_cast<std::int64_t>(pMainItem->GetID());
    stLog.nParam8 = bySuccess;

    // Create items
    if (pInvenPtr->CreateItem2(stCreateItems, 0x7Cu, false,
                               psCreateItemList, psUpdateItemList, stLog))
    {
        // Deduct money
        pInvenPtr->AddBindMoney(-biNeedMoney, 0x3Fu, 1, 0, 0);
        return 0;
    }
    else
    {
        LogHelper::LogError("game.item", "IsCombineAkashic error - Failed CreateItem2[UCID%d]", dwUCID);
        return 52522;
    }
}

// IsComposeHiddenAkashic (0x14001ef10)
// IDA: Compose hidden akashic from two items
int CGocAkashicRecord::IsComposeHiddenAkashic(PS_ITEM_SLOT_INFO& psMainInfo, PS_ITEM_SLOT_INFO& psNeedInfo,
                                               PS_RES_STORAGE_INFO& psCreateItemList, PS_RES_STORAGE_INFO& psUpdateItemList,
                                               int& nCreateAkashicID)
{
    // Clear output
    psCreateItemList.vecItem.clear();
    psUpdateItemList.vecItem.clear();
    nCreateAkashicID = 0;

    // Get owner mover and cast to CUser
    CMover* pMover = GetOwnerMover();
    if (!pMover) return 52522;

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) return 52522;

    std::uint32_t dwUCID = pUser->GetUCID();

    // Get inventory component
    std::shared_ptr<CGocInventory> pInvenPtr = pMover->GetGOC_Inventory(false);
    if (!pInvenPtr)
    {
        return 52522;
    }

    // Get main akashic item
    bool bLock = false;
    std::shared_ptr<CItem> pMainAkashicItemPtr = pInvenPtr->GetSlotItem(psMainInfo.byInvenType, psMainInfo.shSlotPos, bLock);
    if (!pMainAkashicItemPtr || bLock)
    {
        LogHelper::LogError("game.item", "IsComposeHiddenAkashic error - Wrong main item[UCID:%d, InvenType:%d, Slot:%d]",
            dwUCID, psMainInfo.byInvenType, psMainInfo.shSlotPos);
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        pServer->SendItemLockLog(dwUCID, psMainInfo.byInvenType, psMainInfo.shSlotPos, bLock ? 1 : 0, 61, 0);
        return 52011;
    }

    // Get TB_AKASHIC_RECORDS for main item
    CItem* pMainItem = pMainAkashicItemPtr.get();
    std::uint32_t dwMainID = static_cast<std::uint32_t>(pMainItem->GetID());

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_AKASHIC_RECORDS* pTB_AkashicRecords = pServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwMainID);
    if (!pTB_AkashicRecords)
    {
        LogHelper::LogError("game.item", "IsComposeHiddenAkashic error - No Table TB_AKASHIC_RECORDS[UCID:%d, ID:%d]", dwUCID, dwMainID);
        return 52522;
    }

    // Get need akashic item
    bLock = false;
    std::shared_ptr<CItem> pNeedAkashicItemPtr = pInvenPtr->GetSlotItem(psNeedInfo.byInvenType, psNeedInfo.shSlotPos, bLock);
    if (!pNeedAkashicItemPtr || bLock)
    {
        LogHelper::LogError("game.item", "IsComposeHiddenAkashic error - Wrong need item[UCID:%d, Inven:%d, Slot:%d]",
            dwUCID, psNeedInfo.byInvenType, psNeedInfo.shSlotPos);
        return 52011;
    }

    // Get TB_AKASHIC_RECORDS for need item
    CItem* pNeedItem = pNeedAkashicItemPtr.get();
    std::uint32_t dwNeedID = static_cast<std::uint32_t>(pNeedItem->GetID());

    TB_AKASHIC_RECORDS* pTB_NeedAkashicRecords = pServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwNeedID);
    if (!pTB_NeedAkashicRecords)
    {
        LogHelper::LogError("game.item", "IsComposeHiddenAkashic error - No Table TB_AKASHIC_RECORDS[UCID:%d, ID:%d]", dwUCID, dwNeedID);
        return 52522;
    }

    // Get TB_AKASHIC_MAKE
    TB_AKASHIC_MAKE* pTB_AkashicMake = pServer->GetResourceMgr().GetTB_AKASHIC_MAKE(pTB_AkashicRecords->ID);
    if (!pTB_AkashicMake)
    {
        LogHelper::LogError("game.item", "IsComposeHiddenAkashic error - No Table TB_AKASHIC_MAKE[UCID:%d, id:%d]",
            dwUCID, pTB_AkashicRecords->ID);
        return 52522;
    }

    // Validate need item
    if (pTB_AkashicMake->Hidden_Need_Item != dwNeedID && pTB_AkashicRecords->Akashic_Group != pTB_NeedAkashicRecords->Akashic_Group)
    {
        LogHelper::LogError("game.item", "IsComposeHiddenAkashic error - Check need item id[UCID:%d, MainID:%d, NeedID:%d]",
            dwUCID, dwMainID, dwNeedID);
        return 52522;
    }

    // Check if hidden item exists in TB_ITEM
    TB_ITEM* pTB_HiddenItem = pServer->GetResourceMgr().GetTB_ITEM(pTB_AkashicMake->Create_Hidden);
    if (!pTB_HiddenItem)
    {
        LogHelper::LogError("game.item", "IsComposeHiddenAkashic error - Fault hidden type[UCID:%d, ID:%d]",
            dwUCID, dwMainID);
        return 52522;
    }

    // Check money
    std::int64_t biNeedMoney = pTB_AkashicMake->Hidden_Need_Gold;
    std::int64_t biMoney = pInvenPtr->GetMoney();
    if (biMoney < biNeedMoney)
    {
        LogHelper::LogError("game.item", "IsComposeHiddenAkashic error - Shortage money[UCID:%d]", dwUCID);
        return 52523;
    }

    // Build reduce item map
    std::map<std::pair<std::uint8_t, std::int16_t>, ST_CREATE_ITEM> mpReduceItems;

    // Add main item to reduce list
    ST_CREATE_ITEM stReduceItem;
    stReduceItem.nItemID = static_cast<std::int32_t>(dwMainID);
    stReduceItem.shCount = 1;
    std::int16_t shMainSlot = pMainItem->GetSlot();
    std::uint8_t byMainInven = pMainItem->GetInvenType();
    mpReduceItems[std::make_pair(byMainInven, shMainSlot)] = stReduceItem;

    // Add need item to reduce list
    ST_CREATE_ITEM stNeedReduce;
    stNeedReduce.nItemID = static_cast<std::int32_t>(dwNeedID);
    stNeedReduce.shCount = 1;
    std::int16_t shNeedSlot = pNeedItem->GetSlot();
    std::uint8_t byNeedInven = pNeedItem->GetInvenType();
    auto needKey = std::make_pair(byNeedInven, shNeedSlot);
    auto needIt = mpReduceItems.find(needKey);
    if (needIt != mpReduceItems.end())
    {
        needIt->second.shCount++;
    }
    else
    {
        mpReduceItems[needKey] = stNeedReduce;
    }

    // Reduce items
    for (const auto& pair : mpReduceItems)
    {
        std::uint8_t byInvenType = pair.first.first;
        std::int16_t shSlotPos = pair.first.second;
        std::int16_t shCount = pair.second.shCount;

        if (!pInvenPtr->ReduceItem3(byInvenType, shSlotPos, shCount, 0x7Cu, psUpdateItemList))
        {
            LogHelper::LogError("game.item", "IsComposeHiddenAkashic error - Failed ReduceItem3 [UCID:%d, Inven:%d, Pos:%d, Count:%d]",
                dwUCID, byInvenType, shSlotPos, static_cast<int>(shCount));
            return 52522;
        }
    }

    // Build create items
    ST_CREATE_ITEMS stCreateItems;
    ST_CREATE_ITEM stCreateItem;
    stCreateItem.nItemID = static_cast<std::int32_t>(pTB_AkashicMake->Create_Hidden);
    stCreateItem.shCount = 1;
    stCreateItems.vecInfo.push_back(stCreateItem);
    nCreateAkashicID = static_cast<int>(pTB_AkashicMake->Create_Hidden);

    // Create log
    ST_LOG_GAME stLog;
    stLog._sSubType = 120;
    stLog.nParam3 = 2;
    stLog.nParam6 = static_cast<std::int64_t>(dwMainID);

    // Create items
    if (pInvenPtr->CreateItem2(stCreateItems, 0x7Cu, false,
                               psCreateItemList, psUpdateItemList, stLog))
    {
        // Deduct money
        pInvenPtr->AddBindMoney(-biNeedMoney, 0x3Fu, 2, 0, 0);
        return 0;
    }
    else
    {
        LogHelper::LogError("game.item", "IsComposeHiddenAkashic error - Failed CreateItem2[UCID%d]", dwUCID);
        return 52522;
    }
}

// DisassembleQuickSlotCard - Helper to remove card from quickslot
void CGocAkashicRecord::DisassembleQuickSlotCard(std::uint32_t dwAkashicID)
{
    // Iterate all decks and slots to find and clear this card
    for (int k = 0; k < 5; ++k)
    {
        for (int i = 0; i < 5; ++i)
        {
            std::uint32_t* pCardSlot = &m_psQuickSlotCard[k].nCard_1 + i;
            if (*pCardSlot == dwAkashicID)
            {
                *pCardSlot = 0;
            }
        }
    }
}

// Note: GetOwnerMover(), GetOwnerObject(), GetOwnerActor() are inline in header
// But GetOwnerObject and GetOwnerActor need dynamic_cast

// GetOwnerObject - Returns owner as IXObject via dynamic_cast
IXObject* CGocAkashicRecord::GetOwnerObject() const
{
    return dynamic_cast<IXObject*>(m_pOwner);
}

// GetOwnerActor - Returns owner as XActor via dynamic_cast
XActor* CGocAkashicRecord::GetOwnerActor() const
{
    return dynamic_cast<XActor*>(m_pOwner);
}

// GetAkashicID (0x14001AED0)
// IDA: Gets the active Akashic ID from the current deck
void CGocAkashicRecord::GetAkashicID(std::uint32_t& dwAkashicID)
{
    // IDA: Returns the first card ID from the active deck
    std::uint8_t byActivePage = m_byActiveDeck;
    if (byActivePage < 5)
    {
        dwAkashicID = m_psQuickSlotCard[byActivePage].nCard_1;
    }
    else
    {
        dwAkashicID = 0;
    }
}

// GetDeckName (0x140021200)
// IDA: Gets the deck name vector for all decks
void CGocAkashicRecord::GetDeckName(PS_DECK_NAME_VEC& stDeckNameVec)
{
    // IDA: Iterate through all 5 decks and collect their names
    for (int i = 0; i < 5; ++i)
    {
        PS_DECK_NAME stDeckName;
        stDeckName.byDeckPage = static_cast<std::uint8_t>(i);
        wcscpy_s(stDeckName.szDeckName, m_psQuickSlotCard[i].szDeckName);
        stDeckNameVec.vecInfo.push_back(stDeckName);
    }
}

// SetDeckPageInfo (0x140021970)
// IDA: Sets deck page information (cards and name)
void CGocAkashicRecord::SetDeckPageInfo(PS_QUICKSLOT_CARD& stCard, std::uint8_t byPage)
{
    // IDA: Update the specified deck page
    if (byPage < 5)
    {
        stCard.byPage = byPage;
        m_psQuickSlotCard[byPage] = stCard;
    }
}

// OverlappedAkashic (0x14001BE10)
// IDA: true if the player already owns the card (by Array_Index), else error 0x178C
bool CGocAkashicRecord::OverlappedAkashic(std::uint32_t dwID)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_AKASHIC_RECORDS* pTBAkashicRecord = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwID);
    if (!pTBAkashicRecord)
        return false;

    int nKey = pTBAkashicRecord->Array_Index;
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (!pUser)
        return false;

    auto it = m_mapAkashic.find(nKey);
    if (it != m_mapAkashic.end())
        return true;

    pUser->SendErrorMessage(8, 2, 0x178C);
    return false;
}

// RemoveExistBuff (0x14001BF00)
// IDA: Removes existing buff from user when card is removed
void CGocAkashicRecord::RemoveExistBuff(std::uint32_t dwExistCard)
{
    if (!dwExistCard)
        return;

    // IDA: Get TB_AKASHIC_RECORDS from resource manager
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_AKASHIC_RECORDS* pTBAkashicRecord = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwExistCard);
    if (!pTBAkashicRecord)
        return;

    // IDA: Get owner user and clear buff
    // TODO: 需人工审查 - ClearBuffStatus signature needs verification from IDA
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (pUser)
    {
        // pUser->ClearBuffStatus(pTBAkashicRecord->Skill_ID, 0, 0);
    }
}

// IDA: ?CheckEventNetCafeAkashicRecord@CGocAkashicRecord@@QEAAXXZ (0x14001FDC0)
// IDA 精确还原 - 检查网吧事件的 Akashic Record 状态
void CGocAkashicRecord::CheckEventNetCafeAkashicRecord()
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (!pUser)
        return;

    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();

    // IDA: Check if bLoadAkashicRecord (bit 5 of UserDB byte 0) is not set
    // or server has NETCAFE option enabled
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if ((pCharInfo->userDBBits.UserDB.bLoadAkashicRecord) ||
        pGameServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_NETCAFE))
    {
        bool bSend = false;
        // IDA: If bLoadAkashicRecord is set, check if bSyncAkashicRecord is not set
        if (pCharInfo->userDBBits.UserDB.bLoadAkashicRecord)
        {
            bSend = !pCharInfo->syncUserBits.SyncUser.bSyncAkashicRecord;
        }

        // IDA: Get CGocEntity component
        std::tr1::shared_ptr<CGocEntity> pEntity;
        GetOwnerMover()->GetGOC(&pEntity, false);
        if (pEntity)
        {
            CGocEntity* pGocEntity = pEntity.get();
            if (pGocEntity->IsLoadNetCafe())
            {
                if (pGocEntity->GetNetCafe())
                {
                    EventNetCafeAkashicRecordDelete(false);
                    EventNetCafeAkashicRecordAdd(bSend);
                }
                else
                {
                    EventNetCafeAkashicRecordDelete(bSend);
                }
            }
        }
    }
    else
    {
        EventNetCafeAkashicRecordDelete(false);
    }
}

// IDA: ?CheckEventNetCafeQuickSlot@CGocAkashicRecord@@QEAAXXZ (0x14001FF40)
// IDA 精确还原 - 检查网吧事件的 QuickSlot 状态
void CGocAkashicRecord::CheckEventNetCafeQuickSlot()
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (!pUser)
        return;

    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();

    // IDA: Check if bLoadQuickSlot (bit 17, byte 2 bit 1) is not set
    // or server has NETCAFE option enabled
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if ((pCharInfo->userDBBits.UserDB.bLoadQuickSlot) ||
        pGameServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_NETCAFE))
    {
        bool bSend = false;
        // IDA: If bLoadQuickSlot is set, check sync status
        if (pCharInfo->userDBBits.UserDB.bLoadQuickSlot)
        {
            bSend = !pCharInfo->syncUserBits.SyncUser.bSyncQuickSlot;
        }

        // IDA: Get CGocEntity component
        std::tr1::shared_ptr<CGocEntity> pEntity;
        GetOwnerMover()->GetGOC(&pEntity, false);
        if (pEntity)
        {
            CGocEntity* pGocEntity = pEntity.get();
            if (pGocEntity->IsLoadNetCafe())
            {
                if (pGocEntity->GetNetCafe())
                {
                    // IDA: Get CGocInventory and send quick slot info
                    std::tr1::shared_ptr<CGocInventory> pInven;
                    GetOwnerMover()->GetGOC(&pInven, false);
                    if (pInven)
                    {
                        pInven->SendQuickSlotInfo();
                    }
                }
                else
                {
                    EventNetCafeQuickSlotDelete(bSend);
                }
            }
        }
    }
    else
    {
        EventNetCafeQuickSlotDelete(false);
    }
}

// IDA: ?EventNetCafeAkashicRecordAdd@CGocAkashicRecord@@QEAAX_N@Z (0x140020100)
// IDA 精确还原 - 添加网吧 Akashic Record
void CGocAkashicRecord::EventNetCafeAkashicRecordAdd(bool bSend)
{
    bool bChange = false;

    // IDA: Get PC Akashic map from resource manager
    std::map<std::uint32_t, TB_AKASHIC_RECORDS*> mapList;
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    pGameServer->GetResourceMgr().GetPCAkashic(mapList);

    // IDA: Iterate through PC Akashic records
    for (auto it = mapList.begin(); it != mapList.end(); ++it)
    {
        TB_AKASHIC_RECORDS* pTB_Akashic = it->second;
        if (!pTB_Akashic)
            continue;

        std::uint32_t dwAkashicID = pTB_Akashic->ID;

        // IDA: Check if already in m_mapAkashic
        auto itHave = m_mapAkashic.find(dwAkashicID);
        if (itHave == m_mapAkashic.end())
        {
            // IDA: Add new akashic record
            ST_AKASHIC_RECORD stInfo;
            stInfo.dwAkashicID = pTB_Akashic->ID;
            stInfo.nPosition = pTB_Akashic->Array_Index;
            stInfo.nAkashicExp = 0;

            m_mapAkashic[stInfo.nPosition] = stInfo;
            bChange = true;
        }
    }

    // IDA: Send updated list if changed and bSend is true
    if (bSend && bChange)
    {
        SendAkasicRecordList();
    }
}

// IDA: ?EventNetCafeAkashicRecordDelete@CGocAkashicRecord@@QEAAX_N@Z (0x1400202A0)
// IDA 精确还原 - 删除网吧 Akashic Record
void CGocAkashicRecord::EventNetCafeAkashicRecordDelete(bool bSend)
{
    bool bChange = false;
    ST_AKASHIC_LIST stAkashicDelList;

    // IDA: Iterate through m_mapAkashic to find PC akashics
    for (auto it = m_mapAkashic.begin(); it != m_mapAkashic.end(); ++it)
    {
        ST_AKASHIC_RECORD& stRecord = it->second;

        // IDA: Check if this is a PC akashic
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        if (pGameServer->GetResourceMgr().FindPCAkashic(stRecord.dwAkashicID))
        {
            stAkashicDelList.vecInfo.push_back(stRecord);
        }
    }

    // IDA: Delete found PC akashics from m_mapAkashic
    for (size_t i = 0; i < stAkashicDelList.vecInfo.size(); ++i)
    {
        ST_AKASHIC_RECORD& stInfo = stAkashicDelList.vecInfo[i];
        m_mapAkashic.erase(stInfo.nPosition);
        bChange = true;
    }

    // IDA: Send updated list if changed and bSend is true
    if (bSend && bChange)
    {
        SendAkasicRecordList();
    }
}

// IDA: ?EventNetCafeQuickSlotDelete@CGocAkashicRecord@@QEAAX_N@Z (0x140020420)
// IDA 精确还原 - 删除网吧 QuickSlot 卡片
void CGocAkashicRecord::EventNetCafeQuickSlotDelete(bool bSend)
{
    bool bChange = false;
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();

    // IDA: Iterate through all decks and card slots
    for (int k = 0; k < m_byDeckCount; ++k)
    {
        for (int i = 0; i < 5; ++i)
        {
            std::uint32_t dwCardID = m_psQuickSlotCard[k].uniCard[i];
            if (dwCardID)
            {
                // IDA: Check if this is a PC akashic
                if (pGameServer->GetResourceMgr().FindPCAkashic(dwCardID))
                {
                    // IDA: Remove buff if active deck
                    if (m_byActiveDeck == k)
                    {
                        RemoveExistBuff(dwCardID);
                    }

                    m_psQuickSlotCard[k].uniCard[i] = 0;
                    bChange = true;
                }
            }
        }
    }

    // IDA: Update passive list and save if changed
    if (bChange)
    {
        UpdateAkashicPassiveList();
        SaveQuickSlotAll();
    }

    // IDA: Send quick slot info if bSend and changed
    if (bSend && bChange)
    {
        std::tr1::shared_ptr<CGocInventory> pInven;
        GetOwnerMover()->GetGOC(&pInven, false);
        if (pInven)
        {
            pInven->SendQuickSlotInfo();
        }
    }
}

// IDA: ?SaveQuickSlotAll@CGocAkashicRecord@@QEAAXXZ (0x14001B4A0)
// IDA 精确还原 - 保存所有 QuickSlot 到数据库
void CGocAkashicRecord::SaveQuickSlotAll()
{
    // IDA: Get UCID from owner
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (!pUser)
        return;
    std::uint32_t dwUCID = pUser->GetUCID();

    // IDA: Build save slot packet
    PS_QUICKSLOT_UPDATE_CARD_VEC psSaveSlot;

    for (int i = 0; i < m_byDeckCount; ++i)
    {
        PS_QUICKSLOT_UPDATE_CARD psCard;
        psCard.byPage = static_cast<std::uint8_t>(i);

        // IDA: Copy 5 cards from deck
        for (int k = 0; k < 5; ++k)
        {
            psCard.uniCard[k] = m_psQuickSlotCard[i].uniCard[k];
        }

        psSaveSlot.vecInfo.push_back(psCard);
    }

    // IDA: Send DB packet - main=0x21, sub=0x56
    XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 0x21u, 0x56u);
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket << psSaveSlot;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    pGameServer->SendDBGame(xSendDBPacket);
}

// IDA: ?SaveQuickSlot@CGocAkashicRecord@@QEAAXE@Z (0x14001B2A0)
// IDA 精确还原 - 保存指定页面的 QuickSlot 到数据库
void CGocAkashicRecord::SaveQuickSlot(std::uint8_t byPage)
{
    if (byPage >= 5)
        return;

    // IDA: Get UCID from owner
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerMover());
    if (!pUser)
        return;
    std::uint32_t dwUCID = pUser->GetUCID();

    // IDA: Build save slot packet
    PS_QUICKSLOT_UPDATE_CARD_VEC psSaveSlot;

    PS_QUICKSLOT_UPDATE_CARD psCard;
    psCard.byPage = byPage;

    // IDA: Copy 5 cards from deck
    for (int k = 0; k < 5; ++k)
    {
        psCard.uniCard[k] = m_psQuickSlotCard[byPage].uniCard[k];
    }

    psSaveSlot.vecInfo.push_back(psCard);

    // IDA: Send DB packet - main=0x21, sub=0x56
    XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 0x21u, 0x56u);
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket << psSaveSlot;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    pGameServer->SendDBGame(xSendDBPacket);
}

// IDA: ?ResetRoguelikeMode@CGocAkashicRecord@@QEAAXXZ (0x140020780)
// IDA 精确还原 - 重置 Roguelike 模式下的 Akashic 状态
void CGocAkashicRecord::ResetRoguelikeMode()
{
    // IDA: Build update card info for active deck
    PS_QUICKSLOT_UPDATE_CARD_VEC psUpdateCardInfo;

    PS_QUICKSLOT_UPDATE_CARD stDeck;
    stDeck.byPage = m_byActiveDeck;

    // IDA: Copy 5 cards from active deck
    for (int i = 0; i < 5; ++i)
    {
        stDeck.uniCard[i] = m_psQuickSlotCard[m_byActiveDeck].uniCard[i];
    }

    psUpdateCardInfo.vecInfo.push_back(stDeck);

    // IDA: Send packet - main=8, sub=0x27
    XSendPacket xSendPacket(8u, 0x27u);
    xSendPacket.XParse << static_cast<std::uint8_t>(0);
    xSendPacket << psUpdateCardInfo;

    CMover* pMover = GetOwnerMover();
    if (pMover)
    {
        CGocNetwork::Send(pMover, xSendPacket);
    }

    // IDA: Update passive list
    UpdateAkashicPassiveList();
}

// IDA: ?DisassembleAkashicForCheat@CGocAkashicRecord@@QEAAXH@Z (0x140021C90)
// IDA 精确还原 - GM 命令：分解指定数量的 Akashic
void CGocAkashicRecord::DisassembleAkashicForCheat(int nDisCount)
{
    // IDA: Build list of akashic IDs to disassemble
    std::vector<std::uint32_t> psList;

    // IDA: Iterate through m_mapAkashic and collect up to nDisCount items
    for (auto it = m_mapAkashic.begin(); it != m_mapAkashic.end(); ++it)
    {
        const ST_AKASHIC_RECORD& stInfo = it->second;
        psList.push_back(stInfo.dwAkashicID);

        // IDA: Stop when we have enough items
        if (static_cast<int>(psList.size()) >= nDisCount)
            break;
    }

    // IDA: Call ReqDisassembleAkashic with the collected list
    ReqDisassembleAkashic(psList);
}
