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
}

// IDA: ?GetUseDisassemble@CGocAkashicRecord@@QEAA_NXZ (0x14048D010)
// Verified: Direct IDA decompilation - simple getter for m_bDisassembleAkashic
bool CGocAkashicRecord::GetUseDisassemble() const
{
    return m_bDisassembleAkashic;
}

// IDA: ?SetUseDisassemble@CGocAkashicRecord@@QEAAX_N@Z (0x14048CFF0)
// Verified: Direct IDA decompilation - simple setter for m_bDisassembleAkashic
void CGocAkashicRecord::SetUseDisassemble(bool bDisassemble)
{
    m_bDisassembleAkashic = bDisassemble;
}
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
    std::shared_ptr<CGocInventory> pInvenPtr;
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

        XGameServer* v7 = TXSingleton<XGameServer>::Instance();
        TB_AKASHIC_RECORDS* pTBAkashicRecord = XResourceMgr::GetTB_AKASHIC_RECORDS(&v7->m_xResourceMgr, dwAkashicID);
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
        XGameServer* v10 = TXSingleton<XGameServer>::Instance();
        TB_AKASHIC_DISASSEMBLE* pTB_Disassemble = XResourceMgr::GetTB_AKASHIC_DISASSEMBLE(&v10->m_xResourceMgr, pTBAkashicRecord->Akashic_Disassemble);
        if (!pTB_Disassemble)
        {
            LogHelper::LogError("game.item", "ReqDisassembleAkashic error - No Table TB_AKASHIC_DISASSEMBLE[UCID:%d, ID:%d]", dwUCID, dwAkashicID);
            return false;
        }

        // Get disassemble result item
        XGameServer* v11 = TXSingleton<XGameServer>::Instance();
        TB_ITEM* pTB_Item = XResourceMgr::GetTB_ITEM(&v11->m_xResourceMgr, pTB_Disassemble->Dis_Item);
        if (!pTB_Item)
        {
            LogHelper::LogError("game.item", "ReqDisassembleAkashic error - No Table TB_ITEM[UCID:%d, ID:%d]", dwUCID, dwAkashicID);
            return false;
        }

        // Get item classify for inventory type
        XGameServer* v12 = TXSingleton<XGameServer>::Instance();
        TB_ITEM_CLASSIFY* pTB_ItemClassify = XResourceMgr::GetTB_ITEM_CLASSIFY(&v12->m_xResourceMgr, pTB_Item->Item_Classify_Index);
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

            XGameServer* v15 = TXSingleton<XGameServer>::Instance();
            XItemFactory::CreateItem(&v15->m_xItemFactory, &stCreateItem, &stCreateItem);

            std::uint8_t byInvenType = pBaseInven->GetInvenType();
            pInven->AddItem(byInvenType, shSlotPos, &stCreateItem, 1);

            // Build storage info for response
            PS_STORAGE_INFO psStorageInfo;
            psStorageInfo.byInvenType = byInvenType;
            psStorageInfo.shSlotPos = shSlotPos;
            psStorageInfo.stItem = stCreateItem;
            psCreateItemList.vecInfo.push_back(psStorageInfo);

            // Lock the slot
            pInven->SetLock(byInvenType, shSlotPos, 0x18u);
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
    if (!pInven->CreateItem2(&stAddItems, 0x18u, 0, &psCreateItemList, &psUpdateItemList, &stLog))
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

    VChunkFile* v25_data = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v25_data ? (IXObject*)&v25_data[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x81u, 0x14u);
    xSendDBPacket << psDBDisassemble;
    XGameServer::SendDBGame(TXSingleton<XGameServer>::Instance(), &xSendDBPacket);

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
    VChunkFile* v4 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v4, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (!pUser)
        return false;

    // Get inventory component
    std::shared_ptr<CGocInventory> pInvenPtr;
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
        XGameServer::SendDBStatistics(TXSingleton<XGameServer>::Instance(), &xSendDBStatistics);

        // Send log
        SendDBLog(3, dwAkashicID, 0, byState, 0);
    }

    // TODO: Call CGocInventory::SendCreateItem and SendUpdateItem
    // These need the PS_RES_STORAGE_INFO from the DB response
    // CGocInventory* pInven = pInvenPtr.get();
    // pInven->SendCreateItem(&psCreateItemList);
    // pInven->SendUpdateItem(&psUpdateItemList);

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
        pQuickSlotCard[i] = m_psQuickSlotCard[m_byActiveDeck].nCard[i];
    }
}

// ChangeActiveDeck (0x140020910)
// IDA: Changes active deck, validates, sends DB update and client response
int CGocAkashicRecord::ChangeActiveDeck(PS_DECK_ACTIVE& stActive)
{
    // Get UCID from owner
    DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    VBitmask* v4 = (VBitmask*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                     p_m_ChunkSizeTempMemOfs, (char*)nullptr);
    std::uint32_t dwUCID = CQuestCondition::GetQuestID(v4);

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
        std::uint32_t dwOldCard = m_psQuickSlotCard[m_byActiveDeck].nCard[k];
        std::uint32_t dwNewCard = m_psQuickSlotCard[stActive.byActivePage].nCard[k];
        if (dwOldCard != dwNewCard)
        {
            RemoveExistBuff(dwOldCard);
        }
    }

    // Update active deck
    m_byActiveDeck = stActive.byActivePage;

    // Send DB update packet - main=0x44, sub=0x13
    VChunkFile* v19 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v19 ? (IXObject*)&v19[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x44u, 0x13u);
    xSendDBPacket.XParse << static_cast<int>(dwUCID);
    xSendDBPacket << stActive;

    XGameServer* v6 = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBGame(v6, &xSendDBPacket);

    // Send client response - main=6, sub=0x54
    int nResult = 0;
    XSendPacket xSendPacket(6u, 0x54u);
    xSendPacket.XParse << nResult;
    xSendPacket << stActive;

    VChunkFile* v21 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    XActor* pActor = v21 ? (XActor*)&v21[3].m_ChunkSizeTempMemOfs : nullptr;
    if (pActor)
    {
        CGocNetwork::Send(pActor, &xSendPacket);
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
    DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    VBitmask* v4 = (VBitmask*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                     p_m_ChunkSizeTempMemOfs, (char*)nullptr);
    std::uint32_t dwUCID = CQuestCondition::GetQuestID(v4);

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
        if (!UtilFunc::IsUsableNameFilter(psInfo.szDeckName))
        {
            LogHelper::LogError("game.contents", "ChangeDeckName error - IsUsableNameFilter[UCID:%d]", dwUCID);
            return 58412;
        }

        // Update deck name
        wcscpy_s(m_psQuickSlotCard[psInfo.byDeckPage].szDeckName, psInfo.szDeckName);
    }

    // Send DB update packet - main=0x44, sub=0x14
    VChunkFile* v32 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v32 ? (IXObject*)&v32[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x44u, 0x14u);
    xSendDBPacket.XParse << static_cast<int>(dwUCID);
    xSendDBPacket << stChange;

    XGameServer::SendDBGame(TXSingleton<XGameServer>::Instance(), &xSendDBPacket);

    // Send client response - main=6, sub=0x55
    PS_RES_DECK_NAME psRes;
    psRes.vecName = stChange;
    psRes.nResult = 0;

    XSendPacket xSendPacket(6u, 0x55u);
    xSendPacket << psRes;

    VChunkFile* v34 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    XActor* pActor = v34 ? (XActor*)&v34[3].m_ChunkSizeTempMemOfs : nullptr;
    if (pActor)
    {
        CGocNetwork::Send(pActor, &xSendPacket);
    }

    return 0;
}

// IsOverlapCard (0x14001fc30)
// IDA: Checks if cards have overlapping effect groups
bool CGocAkashicRecord::IsOverlapCard(std::uint32_t* uniCard)
{
    std::set<std::uint32_t> setCard;

    for (int i = 0; i < 5; ++i)
    {
        if (uniCard[i])
        {
            XGameServer* v2 = TXSingleton<XGameServer>::Instance();
            TB_AKASHIC_RECORDS* pTB_AkashicRecords = XResourceMgr::GetTB_AKASHIC_RECORDS(&v2->m_xResourceMgr, uniCard[i]);
            if (!pTB_AkashicRecords)
            {
                return false;
            }

            std::uint32_t dwEffectGroup = pTB_AkashicRecords->Akashic_Effect_Group;
            auto bResult = setCard.insert(dwEffectGroup);
            if (!bResult.second)
            {
                // Duplicate effect group found
                DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
                std::uint32_t* v4 = (std::uint32_t*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                                     p_m_ChunkSizeTempMemOfs, (char*)nullptr);
                LogHelper::LogError("game.contents", "IsOverlapCard error - [UCID:%d, ID:%d, GroupID:%d]",
                    *v4, pTB_AkashicRecords->ID, pTB_AkashicRecords->Akashic_Effect_Group);
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
    DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    VBitmask* v3 = (VBitmask*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                     p_m_ChunkSizeTempMemOfs, (char*)nullptr);
    std::uint32_t dwUCID = CQuestCondition::GetQuestID(v3);

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
    XGameServer* v4 = TXSingleton<XGameServer>::Instance();
    TB_AKASHIC_SLOT_EXTEND* pTB_Extend = XResourceMgr::GetTB_AKASHIC_SLOT_EXTEND(&v4->m_xResourceMgr, byExtendDeck);
    if (!pTB_Extend)
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - No Table TB_AKASHIC_SLOT_EXTEND[UCID:%d]", dwUCID);
        return 52004;
    }

    // Get required item
    XGameServer* v5 = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pTB_Item = XResourceMgr::GetTB_ITEM(&v5->m_xResourceMgr, pTB_Extend->Extend_Akashic_Need_Item_ID);
    if (!pTB_Item)
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - No Table TB_ITEM[UCID:%d, ItemID:%d]", dwUCID, pTB_Extend->Extend_Akashic_Need_Item_ID);
        return 52004;
    }

    // Get item classify
    XGameServer* v6 = TXSingleton<XGameServer>::Instance();
    TB_ITEM_CLASSIFY* pTB_Item_Classify = XResourceMgr::GetTB_ITEM_CLASSIFY(&v6->m_xResourceMgr, pTB_Item->Item_Classify_Index);
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

    if (!pInven->ReduceItem2(pTB_Item, shNeedCount, 0x88u, &psUpdateItemList))
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - Failed ReduceItem2[UCID:%d]", dwUCID);
        return 58414;
    }

    // Update item end
    ST_LOG_GAME stLog;
    stLog._sSubType = 137;
    stLog.nParam3 = 1;
    stLog.nParam7 = byExtendDeck;

    if (!pInven->UpdateItemEnd(0x88u, &psUpdateItemList, &stLog))
    {
        LogHelper::LogError("game.contents", "OpenCardDeck error - Failed UpdateItemEnd[UCID:%d]", dwUCID);
        pInven->UnLockList(&psUpdateItemList);
        return 58414;
    }

    // Send DB packet - main=0x21, sub=0x57
    PS_DB_CARD_DECK_OPEN psDBDeck;
    psDBDeck.dwUCID = dwUCID;
    psDBDeck.psUpdateItemList = psUpdateItemList;
    psDBDeck.byCardDeckCount = byExtendDeck;
    psDBDeck.psCardDeck.byPage = byOpenPage;

    VChunkFile* v46 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v46 ? (IXObject*)&v46[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x21u, 0x57u);
    xSendDBPacket << psDBDeck;
    XGameServer::SendDBGame(TXSingleton<XGameServer>::Instance(), &xSendDBPacket);

    return 0;
}

// IsCombineAkashic (0x14001daf0)
// IDA: Complex function for akashic combination with success rate calculation
// TODO: 汇编还原 - Full implementation requires TB_AKASHIC_COMBINATION, TB_AKASHIC_RANDOM_GROUP tables
int CGocAkashicRecord::IsCombineAkashic(PS_ITEM_SLOT_INFO& psMainInfo, PS_ITEM_SLOT_INFOS& psNeedInfos,
                                         PS_RES_STORAGE_INFO& psCreateItemList, PS_RES_STORAGE_INFO& psUpdateItemList,
                                         std::uint8_t& bySuccess, int& nCreateAkashicID)
{
    // Clear output
    psCreateItemList.vecInfo.clear();
    psUpdateItemList.vecInfo.clear();
    bySuccess = 0;
    nCreateAkashicID = 0;

    // Get UCID from owner
    DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    VBitmask* v7 = (VBitmask*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                     p_m_ChunkSizeTempMemOfs, (char*)nullptr);
    std::uint32_t dwUCID = CQuestCondition::GetQuestID(v7);

    // Get inventory component
    CMover* v8 = GetOwnerMover();
    std::shared_ptr<CGocInventory> pInvenPtr;
    v8->GetGOC<CGocInventory>(&pInvenPtr, 0);
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

    // TODO: Full implementation requires:
    // 1. Get main akashic item and validate
    // 2. Get TB_AKASHIC_RECORDS for main item
    // 3. Check Rare_Point < 6
    // 4. Get TB_AKASHIC_COMBINATION table
    // 5. Validate money >= Combination_Need_Zeny
    // 6. Process need items and calculate combination points
    // 7. Calculate success rate based on points
    // 8. If success, get result from TB_AKASHIC_RANDOM_GROUP
    // 9. Reduce items and create result

    return 0; // TODO: Full implementation
}

// IsComposeHiddenAkashic (0x14001ef10)
// IDA: Compose hidden akashic from two items
// TODO: 汇编还原 - Full implementation requires TB_AKASHIC_MAKE table
int CGocAkashicRecord::IsComposeHiddenAkashic(PS_ITEM_SLOT_INFO& psMainInfo, PS_ITEM_SLOT_INFO& psNeedInfo,
                                               PS_RES_STORAGE_INFO& psCreateItemList, PS_RES_STORAGE_INFO& psUpdateItemList,
                                               int& nCreateAkashicID)
{
    // Clear output
    psCreateItemList.vecInfo.clear();
    psUpdateItemList.vecInfo.clear();
    nCreateAkashicID = 0;

    // Get UCID from owner
    DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    VBitmask* v6 = (VBitmask*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                     p_m_ChunkSizeTempMemOfs, (char*)nullptr);
    std::uint32_t dwUCID = CQuestCondition::GetQuestID(v6);

    // Get inventory component
    CMover* v7 = GetOwnerMover();
    std::shared_ptr<CGocInventory> pInvenPtr;
    v7->GetGOC<CGocInventory>(&pInvenPtr, 0);
    if (!pInvenPtr)
    {
        return 52522;
    }

    // TODO: Full implementation requires:
    // 1. Get main akashic item and validate
    // 2. Get need akashic item and validate
    // 3. Get TB_AKASHIC_RECORDS for both
    // 4. Get TB_AKASHIC_MAKE table
    // 5. Validate Hidden_Need_Item matches or Akashic_Group matches
    // 6. Validate money >= Hidden_Need_Gold
    // 7. Reduce items and create hidden akashic

    return 0; // TODO: Full implementation
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
// IDA: Gets akashic ID from specific slot with ownership validation
std::uint32_t CGocAkashicRecord::GetAkashicIDFromSlot(std::uint32_t nSlot)
{
    // IDA: Validate slot index
    if (nSlot > 4)
        return 0;

    // IDA: Validate active deck
    if (m_byActiveDeck > m_byDeckCount - 1 || m_byActiveDeck >= 5)
        return 0;

    // IDA: Get card ID from active deck slot
    std::uint32_t dwAkashicID = m_psQuickSlotCard[m_byActiveDeck].nCard[nSlot];

    // IDA: Get TB_AKASHIC_RECORDS from resource manager
    XGameServer* pGameServer = XGameServer::Instance();
    TB_AKASHIC_RECORDS* pTBAkashic = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwAkashicID);
    if (!pTBAkashic)
        return 0;

    // IDA: Check if player owns this akashic
    int nKey = pTBAkashic->Array_Index;
    auto it = m_mapAkashic.find(nKey);
    if (it == m_mapAkashic.end())
        return 0;

    return dwAkashicID;
}

// SaveQuickSlotAll (0x14001B4A0)
// IDA: Saves all quick slot cards to database
void CGocAkashicRecord::SaveQuickSlotAll()
{
    // IDA: Get UCID from owner
    DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    VBitmask* v1 = (VBitmask*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                     p_m_ChunkSizeTempMemOfs, (char*)nullptr);
    std::uint32_t dwUCID = CQuestCondition::GetQuestID(v1);

    // IDA: Build save packet
    PS_QUICKSLOT_UPDATE_CARD_VEC psSaveSlot;
    for (int i = 0; i < m_byDeckCount; ++i)
    {
        PS_QUICKSLOT_UPDATE_CARD psCard;
        psCard.byPage = static_cast<std::uint8_t>(i);
        for (int k = 0; k < 5; ++k)
        {
            psCard.nCard[k] = m_psQuickSlotCard[i].nCard[k];
        }
        psSaveSlot.vecInfo.push_back(psCard);
    }

    // IDA: Send DB packet (main=0x21, sub=0x56)
    VChunkFile* v12 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v12 ? (IXObject*)&v12[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x21u, 0x56u);
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket << psSaveSlot;

    XGameServer* pGameServer = XGameServer::Instance();
    XGameServer::SendDBGame(pGameServer, &xSendDBPacket);
}

// SaveQuickSlot (0x14001B6B0)
// IDA: Saves a single quick slot page to database
void CGocAkashicRecord::SaveQuickSlot(std::uint8_t byPage)
{
    // IDA: Get UCID from owner
    DynArray_cl<int>* p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone*>::size((VChunkLocker*)this)[3].m_ChunkSizeTempMemOfs;
    VBitmask* v2 = (VBitmask*)((__int64(__fastcall*)(DynArray_cl<int>*, _BYTE*))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(
                     p_m_ChunkSizeTempMemOfs, (char*)nullptr);
    std::uint32_t dwUCID = CQuestCondition::GetQuestID(v2);

    // IDA: Build save packet for single page
    PS_QUICKSLOT_UPDATE_CARD_VEC psSaveSlot;
    PS_QUICKSLOT_UPDATE_CARD psCard;
    psCard.byPage = byPage;
    for (int k = 0; k < 5; ++k)
    {
        psCard.nCard[k] = m_psQuickSlotCard[byPage].nCard[k];
    }
    psSaveSlot.vecInfo.push_back(psCard);

    // IDA: Send DB packet (main=0x21, sub=0x56)
    VChunkFile* v12 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    IXObject* pObject = v12 ? (IXObject*)&v12[3].m_ChunkSizeTempMemOfs : nullptr;

    XSendDBPacket xSendDBPacket(pObject, 0x21u, 0x56u);
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket << psSaveSlot;

    XGameServer* pGameServer = XGameServer::Instance();
    XGameServer::SendDBGame(pGameServer, &xSendDBPacket);
}

// OverlappedAkashic (0x14001BE10)
// IDA: Checks if akashic can be overlapped (already owned check)
bool CGocAkashicRecord::OverlappedAkashic(std::uint32_t dwID)
{
    // IDA: Get TB_AKASHIC_RECORDS from resource manager
    XGameServer* pGameServer = XGameServer::Instance();
    TB_AKASHIC_RECORDS* pTBAkashic = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwID);
    if (!pTBAkashic)
        return false;

    // IDA: Get owner user
    VChunkFile* v4 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v4, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (!pUser)
        return false;

    // IDA: Check if player already owns this akashic
    int nKey = pTBAkashic->Array_Index;
    auto it = m_mapAkashic.find(nKey);
    if (it == m_mapAkashic.end())
    {
        // Not owned - can overlap
        return true;
    }

    // Already owned - send error message
    CUser::SendErrorMessage(pUser, 8u, 2u, 0x178Cu);
    return false;
}

// RemoveExistBuff (0x14001BF00)
// IDA: Removes existing buff from user when card is removed
void CGocAkashicRecord::RemoveExistBuff(std::uint32_t dwExistCard)
{
    if (!dwExistCard)
        return;

    // IDA: Get TB_AKASHIC_RECORDS from resource manager
    XGameServer* pGameServer = XGameServer::Instance();
    TB_AKASHIC_RECORDS* pTBAkashicRecord = pGameServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(dwExistCard);
    if (!pTBAkashicRecord)
        return;

    // IDA: Get owner user and clear buff
    VChunkFile* v3 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v3, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (pUser)
    {
        pUser->ClearBuffStatus(&pUser->CMoverEx, pTBAkashicRecord->Skill_ID, 0, 0);
    }
}

/**
 * CheckEventNetCafeAkashicRecord (0x14001FDC0)
 * IDA: Check net cafe event for akashic records - add/remove PC cafe bonus akashics
 */
void CGocAkashicRecord::CheckEventNetCafeAkashicRecord()
{
    // IDA: Get owner user
    VChunkFile* v1 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v1, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (!pUser)
        return;

    // IDA: Check if net cafe feature is enabled
    STMyCharInfoEx* pCharInfo = CUser::stMyCharInfoEx(pUser);
    if ((pCharInfo->UserDB.byNetCafeFlag & 2) == 0)
    {
        // Not net cafe user - check server option
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        if (!XResourceMgr::GetServerContents(&pGameServer->m_xResourceMgr, E_SERVER_OPTION_NETCAFE))
        {
            // Net cafe disabled - delete all net cafe akashics
            EventNetCafeAkashicRecordDelete(false);
            return;
        }
    }

    // IDA: Determine if we need to send update
    bool bSend = false;
    if ((pCharInfo->UserDB.byNetCafeFlag & 2) != 0)
    {
        // Net cafe user - check if sync needed
        bSend = (pCharInfo->SyncUser.byNetCafeFlag & 2) == 0;
    }

    // IDA: Get CGocEntity to check net cafe status
    CMover* pMover = (CMover*)std::list<CBattleZone*>::size((VChunkLocker*)this);
    std::shared_ptr<CGocEntity> pEntity;
    pMover->GetGOC<CGocEntity>(&pEntity, false);

    if (pEntity)
    {
        CGocEntity* pEnt = pEntity.get();
        if (pEnt && pEnt->IsLoadNetCafe())
        {
            if (pEnt->GetNetCafe())
            {
                // In net cafe - add bonus akashics, then delete old ones
                EventNetCafeAkashicRecordDelete(false);
                EventNetCafeAkashicRecordAdd(bSend);
            }
            else
            {
                // Not in net cafe - delete net cafe akashics
                EventNetCafeAkashicRecordDelete(bSend);
            }
        }
    }
}

/**
 * CheckEventNetCafeQuickSlot (0x14001FF40)
 * IDA: Check net cafe event for quick slot cards
 */
void CGocAkashicRecord::CheckEventNetCafeQuickSlot()
{
    // IDA: Get owner user
    VChunkFile* v1 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    CUser* pUser = (CUser*)_RTDynamicCast_0(v1, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    if (!pUser)
        return;

    // IDA: Check if net cafe feature is enabled
    STMyCharInfoEx* pCharInfo = CUser::stMyCharInfoEx(pUser);
    if ((pCharInfo->UserDB.byNetCafeFlag & 0x20) == 0)
    {
        // Not net cafe user - check server option
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        if (!XResourceMgr::GetServerContents(&pGameServer->m_xResourceMgr, E_SERVER_OPTION_NETCAFE))
        {
            // Net cafe disabled - delete all net cafe quick slots
            EventNetCafeQuickSlotDelete(false);
            return;
        }
    }

    // IDA: Determine if we need to send update
    bool bSend = false;
    if ((pCharInfo->UserDB.byNetCafeFlag & 0x20) != 0)
    {
        // Net cafe user - check if sync needed
        bSend = (pCharInfo->SyncUser.byNetCafeFlag & 0x20) == 0;
    }

    // IDA: Get CGocEntity to check net cafe status
    CMover* pMover = (CMover*)std::list<CBattleZone*>::size((VChunkLocker*)this);
    std::shared_ptr<CGocEntity> pEntity;
    pMover->GetGOC<CGocEntity>(&pEntity, false);

    if (pEntity)
    {
        CGocEntity* pEnt = pEntity.get();
        if (pEnt && pEnt->IsLoadNetCafe())
        {
            if (pEnt->GetNetCafe())
            {
                // In net cafe - send quick slot info
                CMover* pMover2 = (CMover*)std::list<CBattleZone*>::size((VChunkLocker*)this);
                std::shared_ptr<CGocInventory> pInven;
                pMover2->GetGOC<CGocInventory>(&pInven, false);
                if (pInven)
                {
                    pInven->SendQuickSlotInfo();
                }
            }
            else
            {
                // Not in net cafe - delete net cafe quick slots
                EventNetCafeQuickSlotDelete(bSend);
            }
        }
    }
}

/**
 * EventNetCafeAkashicRecordAdd (0x140020100)
 * IDA: Add PC cafe bonus akashic records to player
 */
void CGocAkashicRecord::EventNetCafeAkashicRecordAdd(bool bSend)
{
    bool bChange = false;

    // IDA: Get PC akashic list from resource manager
    std::map<std::uint32_t, TB_AKASHIC_RECORDS*> mapList;
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    XResourceMgr::GetPCAkashic(&pGameServer->m_xResourceMgr, &mapList);

    // IDA: Iterate through all PC akashics and add missing ones
    for (auto iter = mapList.begin(); iter != mapList.end(); ++iter)
    {
        TB_AKASHIC_RECORDS* pTB_Akashic = iter->second;
        if (!pTB_Akashic)
            continue;

        int nKey = pTB_Akashic->ID;

        // Check if already owned
        auto iterHave = m_mapAkashic.find(nKey);
        if (iterHave == m_mapAkashic.end())
        {
            // Not owned - add it
            ST_AKASHIC_RECORD stInfo = {};
            stInfo.dwAkashicID = pTB_Akashic->ID;
            stInfo.nPosition = pTB_Akashic->Array_Index;
            stInfo.nAkashicExp = 0;

            m_mapAkashic[nKey] = stInfo;
            bChange = true;
        }
    }

    // IDA: Send update if changed and requested
    if (bSend && bChange)
    {
        SendAkasicRecordList();
    }
}

/**
 * EventNetCafeAkashicRecordDelete (0x1400202A0)
 * IDA: Remove PC cafe bonus akashic records from player
 */
void CGocAkashicRecord::EventNetCafeAkashicRecordDelete(bool bSend)
{
    bool bChange = false;

    // IDA: Build list of PC akashics to delete
    ST_AKASHIC_LIST stAkashicDelList;

    for (auto iter = m_mapAkashic.begin(); iter != m_mapAkashic.end(); ++iter)
    {
        std::uint32_t dwAkashicID = iter->second.dwAkashicID;

        // Check if this is a PC cafe akashic
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        if (XResourceMgr::FindPCAkashic(&pGameServer->m_xResourceMgr, dwAkashicID))
        {
            stAkashicDelList.vecInfo.push_back(iter->second);
        }
    }

    // IDA: Delete the PC akashics
    for (size_t i = 0; i < stAkashicDelList.vecInfo.size(); ++i)
    {
        int nKey = stAkashicDelList.vecInfo[i].nPosition;
        m_mapAkashic.erase(nKey);
        bChange = true;
    }

    // IDA: Send update if changed and requested
    if (bSend && bChange)
    {
        SendAkasicRecordList();
    }
}

/**
 * EventNetCafeQuickSlotDelete (0x140020420)
 * IDA: Remove PC cafe quick slot cards from decks
 */
void CGocAkashicRecord::EventNetCafeQuickSlotDelete(bool bSend)
{
    bool bChange = false;

    // IDA: Check all decks and slots for PC cafe cards
    for (int k = 0; k < m_byDeckCount; ++k)
    {
        for (int i = 0; i < 5; ++i)
        {
            std::uint32_t dwCardID = m_psQuickSlotCard[k].nCard[i];
            if (dwCardID)
            {
                // Check if this is a PC cafe akashic
                XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
                if (XResourceMgr::FindPCAkashic(&pGameServer->m_xResourceMgr, dwCardID))
                {
                    // Remove from active deck buff if needed
                    if (m_byActiveDeck == k)
                    {
                        RemoveExistBuff(dwCardID);
                    }

                    // Clear the slot
                    m_psQuickSlotCard[k].nCard[i] = 0;
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

    // IDA: Send quick slot info if requested
    if (bSend && bChange)
    {
        CMover* pMover = (CMover*)std::list<CBattleZone*>::size((VChunkLocker*)this);
        std::shared_ptr<CGocInventory> pInven;
        pMover->GetGOC<CGocInventory>(&pInven, false);
        if (pInven)
        {
            pInven->SendQuickSlotInfo();
        }
    }
}

/**
 * ResetRoguelikeMode (0x140020780)
 * IDA: Reset roguelike mode - send current deck to client and update passive list
 */
void CGocAkashicRecord::ResetRoguelikeMode()
{
    // IDA: Build update card packet with current active deck
    PS_QUICKSLOT_UPDATE_CARD_VEC psUpdateCardInfo;
    PS_QUICKSLOT_UPDATE_CARD stDeck = {};
    stDeck.byPage = m_byActiveDeck;

    for (int i = 0; i < 5; ++i)
    {
        stDeck.nCard[i] = m_psQuickSlotCard[m_byActiveDeck].nCard[i];
    }

    psUpdateCardInfo.vecInfo.push_back(stDeck);

    // IDA: Send packet to client
    XSendPacket xSendPacket(8, 0x27);
    xSendPacket.XParse << 0;
    xSendPacket << psUpdateCardInfo;

    VChunkFile* v9 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    XActor* pActor = v9 ? (XActor*)&v9[3].m_ChunkSizeTempMemOfs : nullptr;
    CGocNetwork::Send(pActor, &xSendPacket);

    // IDA: Update passive list
    UpdateAkashicPassiveList();
}

/**
 * DisassembleQuickSlotCard (0x1400219E0)
 * IDA: Remove a specific akashic card from all quick slot decks
 */
void CGocAkashicRecord::DisassembleQuickSlotCard(std::uint32_t dwAkashicID)
{
    // IDA: Build update card list
    PS_QUICKSLOT_UPDATE_CARD_VEC vecUpdateCard;

    for (int i = 0; i < m_byDeckCount; ++i)
    {
        PS_QUICKSLOT_UPDATE_CARD psUpdate = {};
        psUpdate.byPage = m_psQuickSlotCard[i].byPage;
        bool bSlot = false;

        for (int k = 0; k < 5; ++k)
        {
            if (m_psQuickSlotCard[i].nCard[k] == dwAkashicID)
            {
                bSlot = true;
                // Remove buff if on active deck
                if (m_byActiveDeck == psUpdate.byPage)
                {
                    RemoveExistBuff(m_psQuickSlotCard[i].nCard[k]);
                }
                psUpdate.nCard[k] = 0;
                m_psQuickSlotCard[i].nCard[k] = 0;
            }
            else
            {
                psUpdate.nCard[k] = m_psQuickSlotCard[i].nCard[k];
            }
        }

        if (bSlot)
        {
            vecUpdateCard.vecInfo.push_back(psUpdate);
        }
    }

    // IDA: If any slots were updated, save and notify client
    if (!vecUpdateCard.vecInfo.empty())
    {
        SaveQuickSlotAll();
        UpdateAkashicPassiveList();

        XSendPacket xSendPacket(8, 0x27);
        xSendPacket.XParse << 0;
        xSendPacket << vecUpdateCard;

        VChunkFile* v12 = std::list<CBattleZone*>::size((VChunkLocker*)this);
        XActor* pActor = v12 ? (XActor*)&v12[3].m_ChunkSizeTempMemOfs : nullptr;
        CGocNetwork::Send(pActor, &xSendPacket);
    }
}

/**
 * DisassembleAkashicForCheat (0x140021C90)
 * IDA: GM cheat to disassemble N akashic records
 */
void CGocAkashicRecord::DisassembleAkashicForCheat(int nDisCount)
{
    // IDA: Build list of akashics to disassemble
    std::vector<std::uint32_t> psList;

    for (auto iter = m_mapAkashic.begin(); iter != m_mapAkashic.end(); ++iter)
    {
        ST_AKASHIC_RECORD stInfo = iter->second;
        psList.push_back(stInfo.dwAkashicID);

        if (psList.size() >= static_cast<size_t>(nDisCount))
            break;
    }

    // IDA: Request disassemble
    ReqDisassembleAkashic(psList);
}
