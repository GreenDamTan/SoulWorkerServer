// CGocAkashicRecord - Game Object Component for Akashic Record System
// Restored from GameServer.exe IDA decompilation (port 10004)
// Address range: 0x140018B80 - 0x14001D82D

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

// Helper macro for getting owner user via RTTI dynamic cast
// IDA pattern: _RTDynamicCast_0(v1, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0)
#define GET_OWNER_USER() \
    ([](CGocAkashicRecord* pThis) -> CUser* { \
        VChunkFile* v = std::list<CBattleZone*>::size((VChunkLocker*)pThis); \
        return (CUser*)_RTDynamicCast_0(v, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0); \
    })(this)

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
    m_byActiveDeck = 0;
}

// SendDBAkashicRecordLoad (0x140018DB0)
// IDA: Sends DB request packet (main=0x21, sub=0x35) to load player's Akashic records
void CGocAkashicRecord::SendDBAkashicRecordLoad()
{
    VChunkFile* v7 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v7 ? (IXObject*)&v7[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x21u, 0x35u);

    // Get UCID from owner user via VBitmask
    DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    VBitmask* v1 = (VBitmask*)(*(__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                     p_m_ChunkSizeTempMemOfs, (char*)nullptr);
    int QuestID = CQuestCondition::GetQuestID(v1);
    xSendDBPacket.XParse << QuestID;

    XGameServer* v3 = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBGame(v3, &xSendDBPacket);
}

// ResAkashicRecordLoad (0x140018EC0)
// IDA: Handles DB response, loads ST_AKASHIC_LIST into m_mapAkashic
void CGocAkashicRecord::ResAkashicRecordLoad(ST_AKASHIC_LIST& stAkashicList)
{
    XGameServer* v2 = TXSingleton<XGameServer>::Instance();
    int nCount = static_cast<int>(stAkashicList.vecInfo.size());

    for (int i = 0; i < nCount; ++i)
    {
        ST_AKASHIC_RECORD& stInfo = stAkashicList.vecInfo[i];
        TB_AKASHIC_RECORDS* pTB_Akashic = XResourceMgr::GetTB_AKASHIC_RECORDS(&v2->m_xResourceMgr, stInfo.dwAkashicID);

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

    VChunkFile* v11 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    XActor* pActor = v11 ? (XActor*)&v11[3].m_ChunkSizeTempMemOfs : nullptr;
    if (pActor)
    {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// AddAkashicRecord (0x140019170)
// IDA: Adds new akashic record, sends DB packet (main=0x21, sub=0x34) and client response (main=8, sub=0x58)
bool CGocAkashicRecord::AddAkashicRecord(std::uint32_t nItemID, int nAkashicExp)
{
    XGameServer* v5 = TXSingleton<XGameServer>::Instance();
    TB_AKASHIC_RECORDS* pTBAkashic = XResourceMgr::GetTB_AKASHIC_RECORDS(&v5->m_xResourceMgr, nItemID);
    if (!pTBAkashic)
        return false;

    ST_AKASHIC_RECORD stAkashicInfo;
    stAkashicInfo.dwAkashicID = pTBAkashic->ID;
    stAkashicInfo.nPosition = pTBAkashic->Array_Index;
    stAkashicInfo.nAkashicExp = nAkashicExp;

    m_mapAkashic[stAkashicInfo.nPosition] = stAkashicInfo;

    std::uint8_t byState = 0;

    // Get UCID from owner
    DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    std::uint32_t dwUCID = *(_DWORD*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                             p_m_ChunkSizeTempMemOfs, (char*)nullptr) & 0x1FFFFFFF;

    // Send DB packet - main=0x21, sub=0x34
    VChunkFile* v30 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v30 ? (IXObject*)&v30[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x21u, 0x34u);
    PS_DB_AKASHIC_USE psDBAkashicUse;
    psDBAkashicUse.dwUCID = dwUCID;
    psDBAkashicUse.dwAkashicID = stAkashicInfo.dwAkashicID;
    psDBAkashicUse.byState = byState;
    psDBAkashicUse.nAkashicExp = stAkashicInfo.nAkashicExp;
    xSendDBPacket << psDBAkashicUse;

    XGameServer* v8 = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBGame(v8, &xSendDBPacket);

    // Send packet to client - main=8, sub=0x58
    XSendPacket xSendPacket(8u, 0x58u);
    xSendPacket << stAkashicInfo;
    xSendPacket.XParse << static_cast<std::uint8_t>(1);  // Success flag

    VChunkFile* v32 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    XActor* pActor = v32 ? (XActor*)&v32[3].m_ChunkSizeTempMemOfs : nullptr;
    if (pActor)
    {
        CGocNetwork::Send(pActor, &xSendPacket);
    }

    // Send DB log
    SendDBLog(2, pTBAkashic->ID, 0, byState, 0);

    // Send statistics - main=0xF0, sub=4
    ST_STATISTICS_AKASHIC stStatistics;
    stStatistics.byFlag = 1;
    stStatistics.dwUCID = dwUCID;
    stStatistics.dwAkashicID = pTBAkashic->ID;

    VChunkFile* v35 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* v36 = v35 ? (IXObject*)&v35[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBStatistics(v36, 0xF0u, 4u);
    xSendDBStatistics << stStatistics;
    XGameServer* v10 = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBStatistics(v10, &xSendDBStatistics);

    return true;
}

// Reset (0x14001D0D0)
// IDA: Resets all akashic records, sends DB reset (main=0x21, sub=0x37) and empty list to client
void CGocAkashicRecord::Reset()
{
    VChunkFile* v18 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v18 ? (IXObject*)&v18[3].m_ChunkSizeTempMemOfs : nullptr;

    // Send DB reset packet - main=0x21, sub=0x37
    XSendDBPacket xSendDBPacket(pObject, 0x21u, 0x37u);

    DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    int* v3 = (int*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                p_m_ChunkSizeTempMemOfs, (char*)nullptr);
    xSendDBPacket.XParse << *v3;

    XGameServer* v4 = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBGame(v4, &xSendDBPacket);

    // Send empty list to client
    ST_AKASHIC_LIST stAkashicList;
    XSendPacket xSendPacket(8u, 0x57u);
    xSendPacket << stAkashicList;

    VChunkFile* v21 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    XActor* pActor = v21 ? (XActor*)&v21[3].m_ChunkSizeTempMemOfs : nullptr;
    if (pActor)
    {
        CGocNetwork::Send(pActor, &xSendPacket);
    }

    // Clear local data
    m_mapAkashic.clear();

    // Send statistics log
    ST_STATISTICS_AKASHIC stStatistics;
    stStatistics.byFlag = 4;

    DynArray_cl<int>* v23 = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    VBitmask* v5 = (VBitmask*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))v23->__vftable[7].dtr_DynArray_cl<int>)(
                     v23, (char*)nullptr);
    stStatistics.dwUCID = CQuestCondition::GetQuestID(v5);

    VChunkFile* v24 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* v25 = v24 ? (IXObject*)&v24[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBStatistics(v25, 0xF0u, 4u);
    xSendDBStatistics << stStatistics;

    XGameServer* v6 = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBStatistics(v6, &xSendDBStatistics);
}

// UpdateAkashicPassiveList (0x14001B880)
// IDA: Updates passive akashic card list based on current quick slot configuration
void CGocAkashicRecord::UpdateAkashicPassiveList()
{
    XGameServer* v1 = TXSingleton<XGameServer>::Instance();

    // Clear the passive list
    m_mapAkashicPassive.clear();

    // Check each card slot in active deck
    PS_QUICKSLOT_CARD* v15 = &m_psQuickSlotCard[m_byActiveDeck];
    for (int i = 0; i < 5; ++i)
    {
        std::uint32_t dwCardID = *(&v15->nCard_1 + i);

        if (!dwCardID)
            continue;

        TB_AKASHIC_RECORDS* pTB_AKashic = XResourceMgr::GetTB_AKASHIC_RECORDS(&v1->m_xResourceMgr, dwCardID);
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
    VChunkFile* v1 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v1, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
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
    VChunkFile* v2 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v2, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
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
    UXActorID actorID;
    pUser->GetActorID(&actorID);
    psSendData.uxUseActorID = actorID.dwActorID;

    PS_TICKCOUNT_INFO psTick;
    XSendPacket xSendRet(6u, 0x21u);
    xSendRet.XParse << static_cast<std::int32_t>(0);
    xSendRet << psSendData;
    xSendRet << psTick;

    CGocNetwork::Send(static_cast<XActor*>(pUser), &xSendRet);
}

// RegisterAllAkashicRecord (0x14001CD60)
// IDA: Registers all akashic records from resource manager and sends to client/DB
void CGocAkashicRecord::RegisterAllAkashicRecord()
{
    ST_AKASHIC_LIST stAkashicList;

    // Iterate through all Akashic records in resource manager
    // IDA: iterates through m_mapTB_AKASHIC_RECORDS at offset +1462 from XGameServer
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    auto& mapAkashicRecords = pGameServer->m_xResourceMgr.m_mapTB_AKASHIC_RECORDS;

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

            DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
            VBitmask* v8 = (VBitmask*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                             p_m_ChunkSizeTempMemOfs, (char*)nullptr);
            stStatistics.dwUCID = CQuestCondition::GetQuestID(v8);
            stStatistics.dwAkashicID = stAkashicRecord.dwAkashicID;

            VChunkFile* v25 = std::list<CBattleZone*>::size((VChunkLocker*)this);
            IXObject* pObject = v25 ? (IXObject*)&v25[3].m_ChunkSizeTempMemOfs : nullptr;

            XSendDBPacket xSendDBStatistics(pObject, 0xF0u, 4u);
            xSendDBStatistics << stStatistics;
            XGameServer* v9 = TXSingleton<XGameServer>::Instance();
            XGameServer::SendDBStatistics(v9, &xSendDBStatistics);
        }
    }

    SendAkasicRecordList();

    // Send DB save packet - main=0x21, sub=0x36
    VChunkFile* v27 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* v28 = v27 ? (IXObject*)&v27[3].m_ChunkSizeTempMemOfs : nullptr;
    XSendDBPacket xSendDBPacket(v28, 0x21u, 0x36u);

    DynArray_cl<int>* v29 = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    std::uint32_t dwUCID = *(_DWORD*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))v29->__vftable[7].dtr_DynArray_cl<int>)(
                             v29, (char*)nullptr) & 0x1FFFFFFF;
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket << stAkashicList;

    XGameServer* v11 = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBGame(v11, &xSendDBPacket);
}

// LoadQuickSlotCard (0x14001BFC0)
// IDA: Loads quick slot card data from database response
bool CGocAkashicRecord::LoadQuickSlotCard(PS_QUICKSLOT_CARD_VEC& psCardDeck)
{
    XGameServer* v2 = TXSingleton<XGameServer>::Instance();

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
    if (byActiveDeck >= 5 || byActiveDeck > m_byDeckCount - 1)
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

        // Validate each card slot - IDA uses std::set<int> for tracking effect groups
        std::set<std::uint32_t> setCardEffectGroups;

        for (int i = 0; i < 5; ++i)
        {
            std::uint32_t dwCardID = psQuickSlotCard.nCard[i];

            if (!dwCardID)
            {
                m_psQuickSlotCard[k].nCard[i] = 0;
                continue;
            }

            TB_AKASHIC_RECORDS* pTB_AkashicRecord = XResourceMgr::GetTB_AKASHIC_RECORDS(&v2->m_xResourceMgr, dwCardID);
            if (!pTB_AkashicRecord)
            {
                m_psQuickSlotCard[k].nCard[i] = 0;
                continue;
            }

            // Check if player owns this card (IDA: FindPCAkashic or m_mapAkashic lookup)
            TB_AKASHIC_RECORDS* pTB_AKASHIC_RECORDS = XResourceMgr::FindPCAkashic(&v2->m_xResourceMgr, dwCardID);
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
                m_psQuickSlotCard[k].nCard[i] = 0;
                continue;
            }

            // Check for duplicate effect group
            std::uint32_t dwEffectGroup = pTB_AkashicRecord->Akashic_Effect_Group;
            auto bResult = setCardEffectGroups.insert(dwEffectGroup);
            if (!bResult.second)
            {
                // Duplicate - clear slot
                m_psQuickSlotCard[k].nCard[i] = 0;
                continue;
            }

            // Copy card to slot
            m_psQuickSlotCard[k].nCard[i] = dwCardID;
        }
    }

    // Update passive list
    UpdateAkashicPassiveList();

    return true;
}

// ReqDisassembleAkashic (0x140019550)
// IDA: Complex function handling akashic disassembly with cost calculation and item creation
// TODO: 汇编还原 - Requires TB_AKASHIC_DISASSEMBLE, TB_ITEM, TB_ITEM_CLASSIFY tables and CGocInventory methods
bool CGocAkashicRecord::ReqDisassembleAkashic(std::vector<std::uint32_t>& psList)
{
    VChunkFile* v2 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v2, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (!pUser)
    {
        LogHelper::LogError("game.item", "ReqDisassembleAkashic error - NULL pUser");
        return false;
    }

    // Get UCID
    UXActorID actorID;
    pUser->GetActorID(&actorID);
    std::uint32_t dwUCID = actorID.dwActorID;

    // Get inventory component
    std::tr1::shared_ptr<CGocInventory> pInvenPtr;
    pUser->GetGOC<CGocInventory>(&pInvenPtr, 0);
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

    // TODO: 汇编还原 - Full implementation needed
    // Key steps from IDA:
    // 1. Validate each akashic to disassemble
    // 2. Check ownership (m_mapAkashic lookup)
    // 3. Check limit types (Akashic_Limit_Type & 2)
    // 4. Get TB_AKASHIC_DISASSEMBLE table entry
    // 5. Calculate costs (money, ether, BP based on Dis_CostType)
    // 6. Create disassembly items
    // 7. Validate inventory space
    // 8. Send DB packet (main=0x81, sub=0x14)
    // 9. Deduct costs and lock items

    return false; // TODO: 汇编还原 - Full implementation needed
}

// ResDisassembleAkashic (0x14001A920)
// IDA: Handles disassembly response from database
bool CGocAkashicRecord::ResDisassembleAkashic(std::vector<std::uint32_t>& psList, std::uint8_t byState)
{
    VChunkFile* v4 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v4, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (!pUser)
        return false;

    // Get inventory component
    std::tr1::shared_ptr<CGocInventory> pInvenPtr;
    pUser->GetGOC<CGocInventory>(&pInvenPtr, 0);
    if (!pInvenPtr)
        return false;

    ST_AKASHIC_LIST stDisassembleList;

    XGameServer* v7 = TXSingleton<XGameServer>::Instance();

    // Process each disassembled akashic
    for (size_t i = 0; i < psList.size(); ++i)
    {
        std::uint32_t dwAkashicID = psList[i];

        TB_AKASHIC_RECORDS* pTBAkashic = XResourceMgr::GetTB_AKASHIC_RECORDS(&v7->m_xResourceMgr, dwAkashicID);
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

        DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
        VBitmask* v9 = (VBitmask*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                         p_m_ChunkSizeTempMemOfs, (char*)nullptr);
        stStatistics.dwUCID = CQuestCondition::GetQuestID(v9);
        stStatistics.dwAkashicID = dwAkashicID;

        VChunkFile* v43 = std::list<CBattleZone*>::size((VChunkLocker*)this);
        IXObject* pObject = v43 ? (IXObject*)&v43[3].m_ChunkSizeTempMemOfs : nullptr;

        XSendDBPacket xSendDBStatistics(pObject, 0xF0u, 4u);
        xSendDBStatistics << stStatistics;
        XGameServer* v10 = TXSingleton<XGameServer>::Instance();
        XGameServer::SendDBStatistics(v10, &xSendDBStatistics);

        // Send log
        SendDBLog(3, dwAkashicID, 0, byState, 0);
    }

    // TODO: Call CGocInventory::SendCreateItem and SendUpdateItem

    // Send disassemble result to client
    XSendPacket xSendPacket(0x18u, 0x15u);
    xSendPacket.XParse << static_cast<std::uint8_t>(1);
    xSendPacket << stDisassembleList;

    CGocNetwork::Send(static_cast<XActor*>(pUser), &xSendPacket);

    return true;
}

// SendDBLog (0x14001D390)
// IDA: Sends log entry to database (main=21)
void CGocAkashicRecord::SendDBLog(std::int16_t shSubType, int nParam0, int nParam1, int nParam4, int nParam3)
{
    ST_LOG_GAME stLog;
    stLog._sMainType = 21;
    stLog._sSubType = shSubType;

    VChunkFile* v6 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v6, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (pUser)
    {
        stLog._nUAID = pUser->GetUAID();

        DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
        stLog._nUCID = *(_DWORD*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                         p_m_ChunkSizeTempMemOfs, (char*)nullptr);
    }

    stLog.nParam0 = nParam0;
    stLog.nParam1 = nParam1;

    VChunkFile* v12 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    stLog.nParam2 = ((unsigned __int8(__fastcall*)(VChunkFile*))v12->__vftable[5].OnStartLoading)(v12);

    stLog.nParam3 = nParam3;
    stLog.nParam4 = nParam4;

    XGameServer* v7 = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBLog(v7, &stLog);
}

// AddAkashicGetInfo (0x14001D500)
// IDA: Adds akashic get info and sends to DB if new
void CGocAkashicRecord::AddAkashicGetInfo(std::uint32_t dwAkashicID)
{
    VChunkFile* v4 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v4, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (!pUser)
        return;

    // Get ActorID for UCID
    UXActorID actorID;
    pUser->GetActorID(&actorID);
    std::uint32_t dwUCID = actorID.dwActorID;

    XGameServer* v6 = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pTB_AkashicItem = XResourceMgr::GetTB_ITEM(&v6->m_xResourceMgr, dwAkashicID);
    if (!pTB_AkashicItem)
        return;

    // Check if this is an Akashic item (Item_Effect_Type == 8)
    if (pTB_AkashicItem->Item_Effect_Type != 8)
        return;

    XGameServer* v7 = TXSingleton<XGameServer>::Instance();
    TB_AKASHIC_RECORDS* pTB_AkashicRecords = XResourceMgr::GetTB_AKASHIC_RECORDS(&v7->m_xResourceMgr, dwAkashicID);
    if (!pTB_AkashicRecords)
        return;

    std::uint32_t dwAkashicGroupID = pTB_AkashicRecords->Akashic_Group;

    // Check if we already have this group
    if (m_setAkashicGetInfo.find(dwAkashicGroupID) != m_setAkashicGetInfo.end())
        return;

    // Add to set
    m_setAkashicGetInfo.insert(dwAkashicGroupID);

    // Send DB packet - main=0x81, sub=0x31
    VChunkFile* v27 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v27 ? (IXObject*)&v27[3].m_ChunkSizeTempMemOfs : nullptr;

    PS_DB_AKASHIC_GETINFO psDBAkashicGetInfo;
    psDBAkashicGetInfo.dwUCID = dwUCID;
    psDBAkashicGetInfo.dwAkashicGroupID = dwAkashicGroupID;

    XSendDBPacket xSendDBPacket(pObject, 0x81u, 0x31u);
    xSendDBPacket << psDBAkashicGetInfo;
    XGameServer* v9 = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBGame(v9, &xSendDBPacket);

    // Send to client - main=0x18, sub=0x34
    XSendPacket xSendPacket(0x18u, 0x34u);
    xSendPacket.XParse << dwAkashicGroupID;

    VChunkFile* v29 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    XActor* pActor = v29 ? (XActor*)&v29[3].m_ChunkSizeTempMemOfs : nullptr;
    if (pActor)
    {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// SetQuickSlotCard
void CGocAkashicRecord::SetQuickSlotCard(PS_QUICKSLOT_CARD* pCard)
{
    if (pCard && pCard->byPage < 5)
    {
        m_psQuickSlotCard[pCard->byPage] = *pCard;
    }
}

// GetQuickSlotCard
void CGocAkashicRecord::GetQuickSlotCard(PS_QUICKSLOT_CARD* pCard)
{
    if (pCard && pCard->byPage < 5)
    {
        *pCard = m_psQuickSlotCard[pCard->byPage];
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

// GetOwnerMover - Helper function
// TODO: Implement based on GOComponent owner mechanism
CMover* CGocAkashicRecord::GetOwnerMover() const
{
    // IDA pattern: std::list<CBattleZone*>::size((VChunkLocker*)this)
    // Returns m_pOwner from GOComponent base
    return m_pOwner;
}

// GetOwnerObject - Helper function
// TODO: Implement based on GOComponent owner mechanism
IXObject* CGocAkashicRecord::GetOwnerObject() const
{
    // IDA pattern: v7[3].m_ChunkSizeTempMemOfs
    VChunkFile* v7 = std::list<CBattleZone*>::size((VChunkLocker*)const_cast<CGocAkashicRecord*>(this));
    return v7 ? (IXObject*)&v7[3].m_ChunkSizeTempMemOfs : nullptr;
}

// GetOwnerActor - Helper function
// TODO: Implement based on GOComponent owner mechanism
XActor* CGocAkashicRecord::GetOwnerActor() const
{
    // IDA pattern: same as GetOwnerObject but cast to XActor
    VChunkFile* v7 = std::list<CBattleZone*>::size((VChunkLocker*)const_cast<CGocAkashicRecord*>(this));
    return v7 ? (XActor*)&v7[3].m_ChunkSizeTempMemOfs : nullptr;
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
        stDeckName.byPage = static_cast<std::uint8_t>(i);
        std::wcscpy_s(stDeckName.szDeckName, m_psQuickSlotCard[i].szDeckName);
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

// LoadQuickSlotCard (0x14001BFC0)
// IDA: Loads quick slot card data from packet
bool CGocAkashicRecord::LoadQuickSlotCard(PS_QUICKSLOT_CARD_VEC& stQuickSlotCardVec)
{
    // IDA: Copy active page and deck data
    m_byActiveDeck = stQuickSlotCardVec.byActivePage;

    // Copy each deck from the vector
    for (size_t i = 0; i < stQuickSlotCardVec.vecInfo.size() && i < 5; ++i)
    {
        m_psQuickSlotCard[i] = stQuickSlotCardVec.vecInfo[i];
    }

    return true;
}

// GetQuickSlotInfo (0x14001C5C0)
// IDA: Gets quick slot info for all decks
void CGocAkashicRecord::GetQuickSlotInfo(PS_QUICKSLOT_CARD_VEC& stQuickSlotCardVec)
{
    // IDA: Build response with active page and all decks
    stQuickSlotCardVec.byActivePage = m_byActiveDeck;
    stQuickSlotCardVec.vecInfo.clear();

    for (int i = 0; i < 5; ++i)
    {
        stQuickSlotCardVec.vecInfo.push_back(m_psQuickSlotCard[i]);
    }
}

// LoadAkashicGetInfo (0x14001D990)
// IDA: Loads akashic get info from packet
void CGocAkashicRecord::LoadAkashicGetInfo(PS_AKASHIC_GETINFO_LIST& stAkashicGetInfoList)
{
    // IDA: Insert each group ID into the set
    for (const auto& info : stAkashicGetInfoList.vecInfo)
    {
        m_setAkashicGetInfo.insert(info.dwAkashicGroupID);
    }
}

// SendAkasicRecordRes (0x14001C8C0)
// IDA: Sends akashic record response to client
void CGocAkashicRecord::SendAkasicRecordRes(CUser* pUser, TB_AKASHIC_RECORDS* pTBAkashic)
{
    if (!pUser || !pTBAkashic)
    {
        return;
    }

    // IDA: Build response packet (main=0x18, sub=0x12)
    // Send akashic record info to client
    XSendPacket xSendPacket(0x18u, 0x12u);
    xSendPacket.XParse << pTBAkashic->Akashic_ID;
    xSendPacket.XParse << pTBAkashic->Akashic_Group;
    xSendPacket.XParse << pTBAkashic->Akashic_Grade;

    CGocNetwork::Send(static_cast<XActor*>(pUser), &xSendPacket);
}

// GetAkashicIDFromSlot (0x14001BCD0)
// IDA: Gets akashic ID from specific slot
std::uint32_t CGocAkashicRecord::GetAkashicIDFromSlot(std::uint8_t bySlot, std::uint8_t byPage)
{
    // IDA: Return card ID from specified deck and slot
    if (byPage < 5 && bySlot < 5)
    {
        return m_psQuickSlotCard[byPage].nCard[bySlot];
    }
    return 0;
}
