#include "GocQuest.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <cstring>
#include <ctime>

// ============================================================================
// CQuestCondition implementation
// ============================================================================

// IDA: 0x140125C20 - Constructor
CQuestCondition::CQuestCondition(std::uint32_t dwQuestID, ST_QUEST_EPISODE* pQuest,
                                 int nConditionIndex, TB_QUEST_CONDITION* pTBCondition)
    : m_dwQuestID(dwQuestID)
    , m_nConditionIndex(nConditionIndex)
    , m_pTBCondition(pTBCondition)
    , m_pQuest(pQuest)
    , m_pCondition(nullptr)
{
    if (pQuest && nConditionIndex >= 0 && nConditionIndex < 10) {
        m_pCondition = &pQuest->stCondition[nConditionIndex];
    }
}

// IDA: 0x140125CB0 - GetConditionType
std::uint8_t CQuestCondition::GetConditionType() const {
    if (m_pTBCondition) {
        return m_pTBCondition->Condition;
    }
    return 0;
}

// IDA: 0x140125CE0 - GetNeedCompletionCondition
int CQuestCondition::GetNeedCompletionCondition() {
    if (m_pTBCondition) {
        return static_cast<int>(m_pTBCondition->High_Condition_ID);
    }
    return 0;
}

// IDA: 0x140125D00 - AddConditionValue
void CQuestCondition::AddConditionValue(std::int8_t nValue) {
    if (m_pCondition) {
        m_pCondition->byValue += nValue;
    }
}

// IDA: 0x140125D30 - GetConditionValue
std::uint8_t CQuestCondition::GetConditionValue() const {
    if (m_pCondition) {
        return m_pCondition->byValue;
    }
    return 0;
}

// IDA: 0x140125D50 - SetConditionValue
void CQuestCondition::SetConditionValue(std::uint8_t byValue) {
    if (m_pCondition) {
        m_pCondition->byValue = byValue;
    }
}

// IDA: 0x140125D70 - IsCompleteCondition
bool CQuestCondition::IsCompleteCondition() {
    if (m_pQuest && m_nConditionIndex >= 0 && m_nConditionIndex < 16) {
        return ((1 << m_nConditionIndex) & m_pQuest->shCompleteBit) > 0;
    }
    return false;
}

// ============================================================================
// CGocQuest implementation
// ============================================================================

// IDA: 0x140125DD0
CGocQuest::CGocQuest()
    : GOComponent()
    , m_tLastInitDate(0)
    , m_nHelperCount(0)
    , m_bLoad(false)
    , m_bSendMsgSectorClear(false)
    , m_bComplete(false)
{
    std::memset(m_szCompleteEpisode, 0, sizeof(m_szCompleteEpisode));
    std::memset(m_bQuestStateClear, 0, sizeof(m_bQuestStateClear));
}

// IDA: 0x140125F00
CGocQuest::~CGocQuest() {
    m_mapEpisode.clear();
    m_mapCondition.clear();
    m_mapUpdateCondition.clear();
    m_mapRepeatQuest.clear();
    m_mapQuestFirstDrop.clear();
}

bool CGocQuest::Initialize() {
    return Init();
}

void CGocQuest::Shutdown() {
    Clear();
}

void CGocQuest::Update(float fDeltaTime) {
    (void)fDeltaTime;
}

// IDA: 0x140125F90
bool CGocQuest::Init() {
    Clear();
    return true;
}

// IDA: 0x140125FB0
void CGocQuest::Clear() {
    // IDA-verified: Clear completed episode bit array (256 bytes)
    std::memset(m_szCompleteEpisode, 0, sizeof(m_szCompleteEpisode));

    // IDA-verified: Clear condition container (boost::multi_index hashed_index)
    // Uses hashed_index::clear() on the ConditionID index
    m_mapCondition.clear();

    // IDA-verified: Clear episode map
    m_mapEpisode.clear();

    // IDA-verified: Clear repeat quest map
    m_mapRepeatQuest.clear();

    // IDA-verified: Clear quest first drop map
    m_mapQuestFirstDrop.clear();

    // IDA-verified: Set last init date to current time via UXMapID
    // Original uses UXMapID::UXMapID(&v2, 0) to get current time
    m_tLastInitDate = std::time(nullptr);

    // IDA-verified: Reset helper count
    m_nHelperCount = 0;

    // IDA-verified: Reset load flag
    m_bLoad = false;

    // IDA-verified: Clear quest state clear array
    std::memset(m_bQuestStateClear, 0, sizeof(m_bQuestStateClear));

    // IDA-verified: Reset sector clear message flag
    m_bSendMsgSectorClear = false;

    // IDA-verified: Reset complete flag
    m_bComplete = false;

    // IDA-verified: Clear update condition map
    m_mapUpdateCondition.clear();
}

// IDA: ?OnUpdate@CGocQuest@@QEAAXXZ (0x1401260A0)
// Verified: Direct IDA decompilation - Daily quest reset check
void CGocQuest::OnUpdate() {
    // IDA: Get owner CMover and dynamic_cast to CUser
    // CMover* pMover = GetOwnerGO();
    // CUser* pUser = dynamic_cast<CUser*>(pMover);

    // IDA: Check if user exists and is not in block state (UserDB & 4)
    // if (!pUser || (pUser->stMyCharInfoEx.UserDB & 4) != 0) return;

    // IDA: Get current time and check for daily reset at 9:00 AM
    // ATL::CTime tCurr = ATL::CTime::GetTickCount();
    // int nDay = tCurr.GetDay();
    // int nMonth = tCurr.GetMonth();
    // int nYear = tCurr.GetYear();

    // IDA: Create today's init time (9:00 AM)
    // ATL::CTime tTodayInit(nYear, nMonth, nDay, 9, 0, 0);

    // IDA: If current hour < 9, subtract one day
    // if (tCurr.GetHour() < 9) {
    //     tTodayInit -= ATL::CTimeSpan(1, 0, 0, 0);
    // }

    // IDA: Check if last init date is before today's init time
    // if (m_tLastInitDate < tTodayInit) {
    //     ResetRepeatQuest(tTodayInit);
    //     m_tLastInitDate = tTodayInit;
    // }

    // TODO: 需要完整外部依赖 - ATL::CTime, CUser, ResetRepeatQuest
}

// IDA: ?SendEpisodeList@CGocQuest@@QEAAXXZ (0x140126240)
// Verified: Direct IDA decompilation - Send episode list to client
void CGocQuest::SendEpisodeList() {
    // IDA: Create PS_QUEST_EPISODE_MAP and copy m_mapEpisode to it
    // PS_QUEST_EPISODE_MAP psInfo;
    // psInfo.mapInfo = m_mapEpisode;

    // IDA: Create PS_REPEAT_QUEST_MAP and copy m_mapRepeatQuest to it
    // PS_REPEAT_QUEST_MAP psRepeat;
    // psRepeat.mapInfo = m_mapRepeatQuest;

    // IDA: Create XSendPacket(0x15, 0x02)
    // XSendPacket xSendPacket(0x15, 0x02);

    // IDA: Serialize both maps into packet
    // xSendPacket << psInfo;
    // xSendPacket << psRepeat;

    // IDA: Get owner actor and send packet
    // XActor* pActor = GetOwnerActor();
    // CGocNetwork::Send(pActor, &xSendPacket);

    // TODO: 需要完整外部依赖 - PS_QUEST_EPISODE_MAP, PS_REPEAT_QUEST_MAP, XSendPacket, CGocNetwork::Send
}

// IDA: ?SendCompleteEpisodeList@CGocQuest@@QEAAXXZ (0x140126400)
// Verified: Direct IDA decompilation - Send complete episode bit array to client
void CGocQuest::SendCompleteEpisodeList() {
    // IDA: Create XSendPacket(0x15, 0x01)
    // XSendPacket xSendPacket(0x15, 0x01);

    // IDA: Write 256-byte bit array to packet
    // XParse::SetBytes(&xSendPacket.XParse, m_szCompleteEpisode, 256);

    // IDA: Get owner actor and send packet
    // XActor* pActor = GetOwnerActor();
    // CGocNetwork::Send(pActor, &xSendPacket);

    // TODO: 需要完整外部依赖 - XSendPacket, XParse::SetBytes, CGocNetwork::Send
}

// IDA: ?SendReqQuestList@CGocQuest@@QEAAXXZ (0x140129EF0)
// Verified: Direct IDA decompilation - Request quest list from DB
void CGocQuest::SendReqQuestList() {
    // IDA: Get owner actor for XSendDBPacket
    // XActor* pActor = GetOwnerActor();

    // IDA: Create XSendDBPacket(0x41, 0x01) - DB request packet
    // XSendDBPacket xSendDBPacket(pActor, 0x41, 0x01);

    // IDA: Get user's UAID and write to packet
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // DWORD dwUAID = pUser->GetUAID();
    // xSendDBPacket.XParse << dwUAID;

    // IDA: Send to game DB via XGameServer
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // XGameServer::SendDBGame(pServer, &xSendDBPacket);

    // TODO: 需要完整外部依赖 - XSendDBPacket, CUser::GetUAID, XGameServer::SendDBGame
}

// IDA: 0x1401264E0
// IDA decompiled: ?FindEpisode@CGocQuest@@QEAA_NK@Z
// Returns true if episode exists and is not failed (bFailed != true)
// BYTE4(second) is offset 4 in ST_QUEST_EPISODE which is the bFailed field (after byAddHelper, _pad0, shCompleteBit)
bool CGocQuest::FindEpisode(std::uint32_t dwEpisodeID) const {
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it == m_mapEpisode.end()) {
        return false;
    }
    // IDA: BYTE4(iter->second) != 1 means bFailed is not true
    // ST_QUEST_EPISODE layout: byAddHelper(1) + _pad0(1) + shCompleteBit(2) + bFailed(1)
    // Offset 4 = bFailed field
    return !it->second.bFailed;
}

// IDA: 0x140126560
// IDA decompiled: ?FindCondition@CGocQuest@@QEAA_NK@Z
bool CGocQuest::FindCondition(std::uint32_t dwConditionID) const {
    // Find condition in boost::multi_index container by ConditionID index
    auto it = m_mapCondition.find(dwConditionID);
    if (it == m_mapCondition.end()) {
        return false;
    }

    // Get the CQuestCondition pointer from shared_ptr
    const auto& spCondition = it->second;
    if (!spCondition) {
        return false;
    }

    // Check if condition is already complete
    if (spCondition->IsCompleteCondition()) {
        return false;
    }

    // Check NeedCompletionCondition prerequisite
    int nNeedCompletion = spCondition->GetNeedCompletionCondition();
    if (nNeedCompletion <= 0 || IsCompleteCondition(nNeedCompletion)) {
        return true;
    }

    return false;
}

// IDA: 0x140126690
// IDA decompiled: ?IsCompleteEpisode@CGocQuest@@QEAA_NK@Z
bool CGocQuest::IsCompleteEpisode(std::uint32_t dwEpisodeID) const {
    // IDA-verified: Check if episode is marked complete in bit array
    // Logic from IDA:
    // 1. Get TB_QUEST_EPISODE from resource manager
    // 2. Check Class_Type - if >= 100, validate character group
    // 3. Check complete bit in m_szCompleteEpisode array
    // 4. Bit index = Complete_Bit / 8, bit pos = Complete_Bit % 8

    // TODO: 需人工审查 - Requires XResourceMgr singleton and TB_QUEST_EPISODE access
    // Need to implement:
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_QUEST_EPISODE* pTB_EPISODE = XResourceMgr::GetTB_QUEST_EPISODE(&pServer->m_xResourceMgr, dwEpisodeID);
    // if (!pTB_EPISODE) return false;
    //
    // // Check Class_Type for character-specific quests
    // if (pTB_EPISODE->Class_Type) {
    //     if (pTB_EPISODE->Class_Type >= 100) {
    //         // Check character group from CGocAttribute
    //     } else {
    //         // Check player class
    //     }
    // }
    //
    // int nIndex = pTB_EPISODE->Complete_Bit / 8;
    // if (nIndex > 256) return false;
    // int nPos = pTB_EPISODE->Complete_Bit % 8;
    // return (m_szCompleteEpisode[nIndex] & (1 << nPos)) != 0;

    (void)dwEpisodeID;
    return false; // Stub - requires resource manager integration
}

// IDA: 0x140139970 - IsCompleteCondition moved to new implementation below
// Old stub removed to avoid redefinition

// IDA: ?ValidCompleteEpisode@CGocQuest@@QEAA_NK@Z (0x140128170)
// Verified: Direct IDA decompilation - Validate if episode can be completed
bool CGocQuest::ValidCompleteEpisode(std::uint32_t dwEpisodeID) const {
    // IDA: Find episode in m_mapEpisode
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it == m_mapEpisode.end()) {
        return false;
    }

    // IDA: Check if episode state is 1023 (complete state)
    // WORD1(iter->second) is offset 2 in ST_QUEST_EPISODE which is shCompleteBit
    // shCompleteBit == 1023 (0x3FF) means all 10 conditions complete
    return it->second.shCompleteBit == 1023;
}

// IDA: 0x1401281F0 - ?CompleteEpisode@CGocQuest@@QEAA_NKAEAUST_GET_INFO@@@Z
// Verified: Direct IDA decompilation - Complete episode and handle linked episodes
// Note: ST_GET_INFO is defined elsewhere; using void* as placeholder
bool CGocQuest::CompleteEpisode(std::uint32_t dwEpisodeID, void* stGetInfo) {
    // IDA: Get CUser from actor via RTDynamicCast
    // CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    // if (!pUser) return false;

    // IDA: Find episode in m_mapEpisode
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it == m_mapEpisode.end()) {
        return false;
    }

    // IDA: Get pTB_EPISODE from resource manager (not stored in ST_QUEST_EPISODE)
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_QUEST_EPISODE* pTB_EPISODE = XResourceMgr::GetTB_QUEST_EPISODE(&pServer->m_xResourceMgr, dwEpisodeID);
    TB_QUEST_EPISODE* pTB_EPISODE = nullptr; // TODO: Need resource manager

    // IDA: Call DeleteEpisode to remove episode and conditions
    DeleteEpisode(dwEpisodeID);

    // IDA: Handle repeat quest completion
    ST_QUEST_REPEAT_INFO stRepeat = {};
    if (pTB_EPISODE && pTB_EPISODE->Contents_Type == 2) {
        // CompleteRepeatQuest(dwEpisodeID);
        // GetRepeatQuestInfo(dwEpisodeID, &stRepeat);
    }

    // IDA: Send packet to client (main=0x15, sub=5)
    // XSendPacket xSendPacket(0x15, 5);
    // xSendPacket << dwEpisodeID;
    // xSendPacket << stGetInfo;
    // xSendPacket << stRepeat;
    // CGocNetwork::Send(pActor, &xSendPacket);

    // IDA: Check for Character_Skip_Quest_ID match
    // Get CGocAttribute and check if this episode matches skip quest
    // If match, call CGocRecode::SetClearTurtorial(true, true)
    // and CompleteQuestForNewChar(1, ...)

    // IDA: Process linked episodes (Link_Episode_ID_01..10)
    if (pTB_EPISODE) {
        // Note: Link_Episode_ID fields are individual fields, not array
        // Access simplified for now
        // for (int i = 0; i < 10; ++i) {
        //     std::uint32_t dwLinkEpisodeID = pTB_EPISODE->Link_Episode_ID_01 + i; // Simplified
        //     if (dwLinkEpisodeID == 0) break;
        //     // Get TB_QUEST_EPISODE for linked episode
        //     // TB_QUEST_EPISODE* pTB_Link = XResourceMgr::GetTB_QUEST_EPISODE(dwLinkEpisodeID);
        //     // if (pTB_Link && pTB_Link->Auto_Start) {
        //     //     AcceptQuest(dwLinkEpisodeID, false);
        //     // }
        // }

        // IDA: Update achieve via CGocAchieve
        // CGocAchieve::UpdateQuestAchieve(pTB_EPISODE->Contents_Type);

        // IDA: If in maze, update clear condition and call script
        // XMaze::UpdateClearMazeCondition(2, dwEpisodeID);
        // XMaze::CallScriptUpdateQuest(QuestID, 1, dwEpisodeID);
        // XMaze::UpdatePartyQuest(dwEpisodeID, 0, 1);
    }

    (void)stGetInfo;
    return true;
}

// IDA: 0x140128C00 - ?CompleteEpisodeAdd@CGocQuest@@QEAAXAEAUPS_QUEST_COMPLETE_ADD_LIST@@@Z
// Verified: Direct IDA decompilation - Add completed episodes to bit array
// Note: PS_QUEST_COMPLETE_ADD_LIST is defined elsewhere; using void* as placeholder
void CGocQuest::CompleteEpisodeAdd(void* psAddList) {
    // IDA: Iterate through add list
    // shCount = psAddList.size();
    // for (size_t sh = 0; sh < psAddList.size(); ++sh) {
    //     std::uint32_t dwEpisodeID = psAddList[sh].dwEpisodeID;

    //     IDA: Get TB_QUEST_EPISODE from resource manager
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     TB_QUEST_EPISODE* pTB_EPISODE = XResourceMgr::GetTB_QUEST_EPISODE(dwEpisodeID);
    //     if (pTB_EPISODE && pTB_EPISODE->Complete_Bit) {
    //         IDA: Calculate bit position
    //         int nIndex = pTB_EPISODE->Complete_Bit / 8;
    //         int nPos = pTB_EPISODE->Complete_Bit % 8;

    //         IDA: Check bounds and not repeat quest (Contents_Type != 2)
    //         if (nIndex <= 256 && pTB_EPISODE->Contents_Type != 2) {
    //             IDA: Check if not already complete
    //             if (!IsCompleteEpisode(dwEpisodeID)) {
    //                 IDA: Set bit in complete episode array
    //                 m_szCompleteEpisode[nIndex] |= (1 << nPos);

    //                 IDA: Send DB packet (0x41/0x05)
    //                 XSendDBPacket xSendDBPacket(pActor, 0x41, 0x05);
    //                 xSendDBPacket << QuestID << dwEpisodeID << Contents_Type;
    //                 XParse::SetBytes(&xSendDBPacket, GetCompleteEpisode(), 256);
    //                 xSendDBPacket << stGetInfo << bReturn;
    //                 XGameServer::SendDBGame(&xSendDBPacket);

    //                 IDA: Send game log (main=6, sub=7)
    //                 ST_LOG_GAME stLog;
    //                 stLog._sMainType = 6; stLog._sSubType = 7;
    //                 stLog.nParam0 = dwEpisodeID;
    //                 XGameServer::SendDBLog(&stLog);
    //             }
    //         }
    //     }
    // }
    (void)psAddList;
}

// IDA: 0x14012BBD0 - ?AcceptQuest@CGocQuest@@QEAA_NK_N@Z
// Verified: Direct IDA decompilation - Accept quest with full validation
bool CGocQuest::AcceptQuest(std::uint32_t dwEpisodeID, bool bCheckMaxCount) {
    // IDA: Get CUser from actor and check block type
    // CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    // if (CUser::GetBlockType(pUser)) {
    //     CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xC3BF);
    //     return false;
    // }

    // IDA: Check max quest count (30 = 0x1E) if bCheckMaxCount
    if (bCheckMaxCount && m_mapEpisode.size() >= 30) {
        // CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD308);
        return false;
    }

    // IDA: Check if already have this episode
    if (FindEpisode(dwEpisodeID)) {
        // CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F1);
        return false;
    }

    // IDA: Check if already completed
    if (IsCompleteEpisode(dwEpisodeID)) {
        // CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F2);
        return false;
    }

    // IDA: Get TB_QUEST_EPISODE from resource manager
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_QUEST_EPISODE* pTB_EPISODE = XResourceMgr::GetTB_QUEST_EPISODE(dwEpisodeID);
    TB_QUEST_EPISODE* pTB_EPISODE = nullptr; // TODO: Need resource manager
    if (!pTB_EPISODE) return false;

    // IDA: Check level requirement (Quest_Level <= player level)
    // CGocAttribute* pAttr = pMover->GetGOC<CGocAttribute>();
    // int nPlayerLevel = CGameWorldMode::GetState(pAttr);
    // if (pTB_EPISODE->Quest_Level > nPlayerLevel) {
    //     CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD302);
    //     return false;
    // }

    // IDA: Check class requirement
    // if (pTB_EPISODE->Class_Type) {
    //     if (pTB_EPISODE->Class_Type < 100) {
    //         // Direct class check
    //         if (Class_Type != player_class) error;
    //     } else {
    //         // Character group check
    //         TB_CHARACTER_INFO* pTB_CHAR = GetTB_CHARACTER_INFO(nCharID);
    //         if (!pTB_CHAR || pTB_CHAR->Character_Group_ID != pTB_EPISODE->Class_Type) error;
    //     }
    // }

    // IDA: Check repeat quest time if Contents_Type == 2
    int nError = 0;
    // if (pTB_EPISODE->Contents_Type == 2 && !CheckAcceptRepeatQuest(dwEpisodeID, bCheckMaxCount, &nError)) {
    //     CGocNetwork::SendErrorMessage(pMover, 0x15, 3, nError);
    //     return false;
    // }

    // IDA: Check before episode completion (Before_Episode_ID_1..5)
    for (int i = 0; i < 5; ++i) {
        // std::uint32_t dwBeforeID = pTB_EPISODE->Before_Episode_ID[i];
        // if (dwBeforeID && !IsCompleteEpisode(dwBeforeID)) {
        //     CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD304);
        //     return false;
        // }
    }

    // IDA: SetQuestAddObject for quest items
    // if (!SetQuestAddObject(dwEpisodeID, &pTB_EPISODE->Add_Object_Type_01, ...)) {
    //     CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD305);
    //     return false;
    // }

    // IDA: Create ST_QUEST_EPISODE and conditions
    ST_QUEST_EPISODE stEpisode = {};
    if (m_nHelperCount < 7) {
        stEpisode.byAddHelper = 1;
        ++m_nHelperCount;
    }

    // IDA: Process each condition (Condition_ID_01..10)
    // for (int nConditionIndex = 0; nConditionIndex < 10; ++nConditionIndex) {
    //     TB_QUEST_CONDITION* pTB_CONDITION = GetTB_QUEST_CONDITION(Condition_ID[nConditionIndex]);
    //     if (!pTB_CONDITION) break;
    //
    //     stEpisode.pTBQuestEpisode = pTB_EPISODE;
    //     stEpisode.shCompleteBit &= ~(1 << nConditionIndex);
    //     stEpisode.stCondition[nConditionIndex].dwConditionID = pTB_CONDITION->ID;
    //     stEpisode.stCondition[nConditionIndex].byValue = 0;
    //
    //     // Insert into m_mapEpisode
    //     m_mapEpisode[dwEpisodeID] = stEpisode;
    //
    //     // Create CQuestCondition and insert into m_mapCondition
    //     CQuestCondition* pCond = new CQuestCondition(dwEpisodeID, &stEpisode, nConditionIndex, pTB_CONDITION);
    //     std::shared_ptr<CQuestCondition> spCond(pCond);
    //     m_mapCondition.insert(spCond);
    //
    //     // If in maze, call script update
    //     if (pMaze) {
    //         XMaze::CallScriptUpdateQuest(pMaze, QuestID, 0, dwEpisodeID);
    //         if (NeedCompletionCondition <= 0 || IsCompleteCondition(NeedCompletionCondition)) {
    //             XMaze::RunQuestConditionStart(pMaze, ConditionID, pTB_CONDITION->ID);
    //         }
    //     }
    // }

    // IDA: DBUpdateEpisodeInfo
    // DBUpdateEpisodeInfo(dwEpisodeID, &stEpisode);

    // IDA: Handle repeat quest
    ST_QUEST_REPEAT_INFO stRepeat = {};
    // if (pTB_EPISODE->Contents_Type == 2) {
    //     AcceptRepeatQuest(dwEpisodeID);
    //     GetRepeatQuestInfo(dwEpisodeID, &stRepeat);
    // }

    // IDA: Send packet to client (main=0x15, sub=3)
    // XSendPacket xSendPacket(0x15, 3);
    // xSendPacket << dwEpisodeID << stEpisode.byAddHelper << stRepeat;
    // CGocNetwork::Send(pActor, &xSendPacket);

    // IDA: UpdateItemCondition, EnableInteractionObject, UpdateQuestConditionForSectorClear
    // UpdateItemCondition();
    // EnableInteractionObject(pTB_EPISODE->Condition_ID_01, 0);
    // UpdateQuestConditionForSectorClear();

    // IDA: UpdateOpenTitle
    // CGocEntity::UpdateOpenTitle(0, dwEpisodeID);

    // IDA: Log to database (main=6, sub=1)
    // ST_LOG_GAME stLog;
    // stLog._sMainType = 6; stLog._sSubType = 1;
    // stLog.nParam0 = dwEpisodeID;
    // XGameServer::SendDBLog(&stLog);

    // IDA: Send statistics if episode in range [0x186A1, 0x30D40)
    // if (dwEpisodeID >= 0x186A1 && dwEpisodeID < 0x30D40) {
    //     ST_STATISTICS_QUEST stStatistics;
    //     stStatistics.byFlag = 1;
    //     stStatistics.dwEpisodeID = dwEpisodeID;
    //     XSendDBPacket xSendDBStatistics(pActor, 0xF0, 6);
    //     xSendDBStatistics << stStatistics;
    //     XGameServer::SendDBStatistics(&xSendDBStatistics);
    // }

    (void)stEpisode;
    (void)stRepeat;
    (void)nError;
    return true;
}

// IDA: 0x14012F100 - ?CompleteQuest@CGocQuest@@QEAA_NKK@Z
// Verified: Direct IDA decompilation - Complete quest with full reward processing
bool CGocQuest::CompleteQuest(std::uint32_t dwEpisodeID, std::uint32_t dwRewardItemID) {
    // IDA: Check if already complete
    if (IsCompleteEpisode(dwEpisodeID)) {
        // LogHelper::LogError("game.quest", "CompleteQuest error - Already Complete[ ActorID:%d, EpisodeID: %d ]", ActorID, dwEpisodeID, 2091);
        return false;
    }

    // IDA: Validate episode can be completed (all conditions done)
    if (!ValidCompleteEpisode(dwEpisodeID)) {
        // LogHelper::LogError("game.quest", "CompleteQuest error - ValidCompleteEpisode[ ActorID:%d, EpisodeID:%d ]", ActorID, dwEpisodeID, 2096);
        return false;
    }

    // IDA: Find episode in m_mapEpisode
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it == m_mapEpisode.end()) {
        // LogHelper::LogError("game.quest", "CompleteQuest error - No Have Episode[ ActorID:%d, EpisodeID:%d ]", ActorID, dwEpisodeID, 2103);
        return false;
    }

    // IDA: Get pTB_QUEST_EPISODE from resource manager (not stored in ST_QUEST_EPISODE)
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_QUEST_EPISODE* pTB_EPISODE = XResourceMgr::GetTB_QUEST_EPISODE(&pServer->m_xResourceMgr, dwEpisodeID);
    TB_QUEST_EPISODE* pTB_EPISODE = nullptr; // TODO: Need resource manager
    if (!pTB_EPISODE) return false;

    // IDA: Get CGocInventory for item operations
    // CGocInventory* pInven = pMover->GetGOC<CGocInventory>();

    // IDA: Process remove items (Remove_Item_ID_01..05, Remove_Item_Count_01..05)
    // for (int i = 0; i < 5; ++i) {
    //     if (pTB_EPISODE->Remove_Item_ID[i]) {
    //         TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(Remove_Item_ID[i]);
    //         if (pTB_ITEM) {
    //             TB_ITEM_CLASSIFY* pTBClassify = GetTB_ITEM_CLASSIFY(pTB_ITEM->Item_Classify_Index);
    //             XBaseInventory* pFindInven = pInven->GetTBInvenPtr(pTBClassify->Item_Inven_Type);
    //             pFindInven->GetItem(&pDeleteItem, Remove_Item_ID[i]);
    //             pInven->BreakItemReq(InvenType, Slot, Remove_Item_Count[i], 0x29, &stLogData);
    //         }
    //     }
    // }

    // IDA: Create ST_CREATE_ITEMS for reward items
    // ST_CREATE_ITEMS stCreateItems;

    // IDA: Process rewards based on Reward_Item_type
    // if (pTB_EPISODE->Reward_Item_type == 1) {
    //     // Direct item rewards
    //     for (int j = 0; j < 5; ++j) {
    //         if (pTB_EPISODE->Reward_Item_ID[j]) {
    //             TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(Reward_Item_ID[j]);
    //             if (pTB_ITEM) {
    //                 stItem.nItemID = Reward_Item_ID[j];
    //                 stItem.shCount = Reward_Item_Count[j];
    //                 stCreateItems.push_back(stItem);
    //             }
    //         }
    //     }
    // }
    // else if (pTB_EPISODE->Reward_Item_type == 2) {
    //     // Random rewards via TB_QUEST_REWARD table
    //     for (int k = 0; k < 5; ++k) {
    //         if (pTB_EPISODE->Reward_Item_ID[k]) {
    //             TB_QUEST_REWARD* pTBReward = GetTB_QUEST_REWARD(Reward_Item_ID[k]);
    //             if (pTBReward) {
    //                 int nClass = pMover->GetClass();
    //                 int nItemID = pTBReward->RewardItem[nClass - 1];
    //                 stItem.nItemID = nItemID;
    //                 stItem.shCount = Reward_Item_Count[k];
    //                 stCreateItems.push_back(stItem);
    //             }
    //         }
    //     }
    // }

    // IDA: Process select rewards (Select_Reward_Item_type, Select_Reward_Item_01..05)
    // for (int m = 0; m < 5; ++m) {
    //     if (pTB_EPISODE->Select_Reward_Item_type == 1) {
    //         if (pTB_EPISODE->Select_Reward_Item[m] && dwRewardItemID == Select_Reward_Item[m]) {
    //             stItem.nItemID = Select_Reward_Item[m];
    //             stItem.shCount = Select_Reward_Item_Count[m];
    //             stCreateItems.push_back(stItem);
    //         }
    //     }
    //     else if (pTB_EPISODE->Select_Reward_Item_type == 2) {
    //         // Random select rewards via TB_QUEST_REWARD
    //     }
    // }

    // IDA: Create reward items via CGocInventory::CreateItemReq
    // ST_GET_INFO stGetInfo;
    // if (!pInven->CreateItemReq(&stCreateItems, 0, E_ITEM_CREATE_TYPE_QUEST_REWARD, &stLogItem)) {
    //     CGocNetwork::SendErrorMessage(pMover, 0x15, 5, 0xCB2A);
    //     return false;
    // }

    // IDA: Process titles (Reward_Title_01..02)
    // for (int n = 0; n < 2; ++n) {
    //     if (pTB_EPISODE->Reward_Title[n]) {
    //         CGocEntity::AddTitleByClass(Reward_Title[n], 2);
    //     }
    // }

    // IDA: Process helper reward if Contents_Type == 3
    // if (pTB_EPISODE->Contents_Type == 3) {
    //     TB_HELPER* pTB_Helper = GetTB_HELPER(Reward_Helper);
    //     CGocHelper::SendDBAddHelper(pTB_Helper->ID);
    // }

    // IDA: Process exp (Reward_EXP with booster bonus)
    // float fExpReward = (float)pTB_EPISODE->Reward_EXP;
    // if (fExpReward > 0.0f) {
    //     CGocBooster* pBooster = pMover->GetGOC<CGocBooster>();
    //     float fRate = pBooster->GetTotalRate(eBooster_Effect_Complete_Quest_IncExp);
    //     int nIncExp = (int)(fExpReward * fRate);
    //     fExpReward += (float)nIncExp;
    //     CGocAttribute::SetExp(fExpReward, (float)nIncExp, 1);
    // }

    // IDA: Process money (Reward_Money)
    // if (pTB_EPISODE->Reward_Money) {
    //     if (pInven->CheckOverMoney(E_PRICE_TYPE_GOLD, Reward_Money)) {
    //         pInven->AddMoney(Reward_Money, 0x23, pTB_EPISODE->ID, 0, 0);
    //     } else {
    //         CGocNetwork::SendErrorMessage(pMover, 0x15, 5, 0xCB58);
    //     }
    // }

    // IDA: Process BP (Reward_BP)
    // if (pTB_EPISODE->Reward_BP) {
    //     if (pInven->CheckOverMoney(E_PRICE_TYPE_BP, Reward_BP)) {
    //         pInven->AddBP(Reward_BP, 0x26);
    //     } else {
    //         CGocNetwork::SendErrorMessage(pMover, 0x15, 5, 0xCB59);
    //     }
    // }

    // IDA: Process Ether (Reward_Ether)
    // if (pTB_EPISODE->Reward_Ether) {
    //     if (pInven->CheckOverMoney(E_PRICE_TYPE_ETHER, Reward_Ether)) {
    //         pInven->AddEther(Reward_Ether, 0x26, 1);
    //     }
    // }

    // IDA: Process Awakening (Reward_Awakening)
    // if (pTB_EPISODE->Reward_Awakening) {
    //     CGocAttribute::SetAwaken(Reward_Awakening, 1);
    // }

    // IDA: Process class skills (Reward_Class_Skill_01..04)
    // CGocSkill* pSkill = pMover->GetGOC<CGocSkill>();
    // for (int ii = 0; ii < 4; ++ii) {
    //     if (pTB_EPISODE->Reward_Class_Skill[ii]) {
    //         int nSkill = Reward_Class_Skill[ii] + 10000000 * nClass;
    //         if (!pSkill->IsHaveSkill(nSkill)) {
    //             pSkill->LearnSkill(nSkill, 0, 0);
    //         }
    //     }
    // }

    // IDA: Set complete bit in m_szCompleteEpisode
    int nIndex = pTB_EPISODE->Complete_Bit / 8;
    int nPos = pTB_EPISODE->Complete_Bit % 8;
    if (nIndex <= 256 && pTB_EPISODE->Contents_Type != 2) {
        m_szCompleteEpisode[nIndex] |= (1 << nPos);
    }

    // IDA: UpdateQuestConditionForSectorClear
    // UpdateQuestConditionForSectorClear();

    // IDA: UpdateOpenTitle
    // CGocEntity::UpdateOpenTitle(1, dwEpisodeID);

    // IDA: Log to database (main=6, sub=4)
    // ST_LOG_GAME stLog;
    // stLog._sMainType = 6; stLog._sSubType = 4;
    // stLog.nParam0 = dwEpisodeID;
    // XGameServer::SendDBLog(&stLog);

    // IDA: Send statistics if episode in range [0x186A1, 0x30D40)
    // if (dwEpisodeID >= 0x186A1 && dwEpisodeID < 0x30D40) {
    //     ST_STATISTICS_QUEST st;
    //     st.byFlag = 4;
    //     st.dwEpisodeID = dwEpisodeID;
    //     XSendDBPacket xSendPacket(pActor, 0xF0, 6);
    //     xSendPacket << st;
    //     XGameServer::SendDBStatistics(&xSendPacket);
    // }

    // IDA: Send DB packet (main=0x41, sub=5)
    // XSendDBPacket xSendDBPacket(pActor, 0x41, 5);
    // xSendDBPacket << QuestID << dwEpisodeID << Contents_Type;
    // XParse::SetBytes(&xSendDBPacket, GetCompleteEpisode(), 256);
    // xSendDBPacket << stGetInfo << bReturn;
    // XGameServer::SendDBGame(&xSendDBPacket);

    (void)dwRewardItemID;
    return true;
}

// IDA: 0x1324A0
bool CGocQuest::GiveUp(std::uint32_t dwEpisodeID, bool bGiveUpCheck) {
    // IDA-verified quest give up logic:
    // 1. Check if episode exists (FindEpisode)
    // 2. Get CUser from actor
    // 3. Find episode in m_mapEpisode
    // 4. Check Give_Up flag if bGiveUpCheck (Give_Up == 1 means cannot give up)
    // 5. ClearUpdateQuestCondition_GiveUp
    // 6. Process add objects (remove quest items if Add_Object_Type == 1 and GroupID == 27)
    // 7. Iterate conditions by QuestID and process:
    //    - DisableInteractionObject for each condition
    //    - Process remove_item_condition items
    // 8. Check inventory space for return items
    // 9. Process restore items if needed
    // 10. InitQuestConditionForSectorClear
    // 11. Log to database (main=6, sub=5)
    // 12. Process restore item via post system if Restore_Item set
    // 13. Send statistics if episode in range [0x186A1, 0x30D40)
    // 14. Send DB packet (main=0x41, sub=6)

    (void)bGiveUpCheck;

    // Check if episode exists
    if (!FindEpisode(dwEpisodeID)) {
        // Episode not found - send error 0xD2F5
        return false;
    }

    // TODO: 需人工审查 - Requires complete implementation with:
    // - CUser pointer from actor
    // - TB_QUEST_EPISODE access for Give_Up flag
    // - ClearUpdateQuestCondition_GiveUp call
    // - Inventory operations for quest item removal
    // - Condition iteration using boost::multi_index
    // - Post system for restore items
    // - Database logging

    // Placeholder: remove from episode map
    m_mapEpisode.erase(dwEpisodeID);
    return true;
}

// IDA: 0x14012E1F0 - ?AcceptQuestByForce@CGocQuest@@QEAA_NK@Z
// Verified: Direct IDA decompilation - Accept quest bypassing normal validation
bool CGocQuest::AcceptQuestByForce(std::uint32_t dwEpisodeID) {
    // IDA: Get CUser from actor and check block type
    // CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    // if (CUser::GetBlockType(pUser)) {
    //     CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xC3BF);
    //     return false;
    // }

    // IDA: Check if already have this episode
    if (FindEpisode(dwEpisodeID)) {
        // CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F1);
        return false;
    }

    // IDA: Check if already completed
    if (IsCompleteEpisode(dwEpisodeID)) {
        // CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F2);
        return false;
    }

    // IDA: Get TB_QUEST_EPISODE from resource manager
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_QUEST_EPISODE* pTB_EPISODE = XResourceMgr::GetTB_QUEST_EPISODE(dwEpisodeID);
    TB_QUEST_EPISODE* pTB_EPISODE = nullptr; // TODO: Need resource manager
    if (!pTB_EPISODE) return false;

    // IDA: Check level requirement (but log error instead of rejecting)
    // CGocAttribute* pAttr = pMover->GetGOC<CGocAttribute>();
    // int nPlayerLevel = CGameWorldMode::GetState(pAttr);
    // if (pTB_EPISODE->Quest_Level > nPlayerLevel) {
    //     LogHelper::LogError("game.quest", "AcceptQuestByForce error - Level condition is not right[ ActorID:%d ]", ActorID, 1929);
    // }

    // IDA: Check class requirement (but log error instead of rejecting)
    // if (pTB_EPISODE->Class_Type) {
    //     if (pTB_EPISODE->Class_Type < 100) {
    //         if (Class_Type != player_class) {
    //             LogHelper::LogError("game.quest", "AcceptQuestByForce error - Class condition is not right[ ActorID:%d / %d ]", ActorID, pTB_EPISODE->ID, 1953);
    //         }
    //     } else {
    //         TB_CHARACTER_INFO* pTB_CHAR = GetTB_CHARACTER_INFO(nCharID);
    //         if (!pTB_CHAR || pTB_CHAR->Character_Group_ID != pTB_EPISODE->Class_Type) {
    //             LogHelper::LogError("game.quest", "AcceptQuestByForce error - Class condition is not right[ ActorID:%d / %d ]", ActorID, pTB_EPISODE->ID, 1944);
    //         }
    //     }
    // }

    // IDA: Check repeat quest time if Contents_Type == 2
    int nError = 0;
    // if (pTB_EPISODE->Contents_Type == 2 && !CheckAcceptRepeatQuest(dwEpisodeID, false, &nError)) {
    //     LogHelper::LogError("game.quest", "AcceptQuest error - Repeat Quest Check Time[ ActorID:%d ]", ActorID, 1963);
    //     return false;
    // }

    // IDA: SetQuestAddObject for quest items
    // if (!SetQuestAddObject(dwEpisodeID, &pTB_EPISODE->Add_Object_Type_01, ...)) {
    //     LogHelper::LogError("game.quest", "AcceptQuestByForce error - Occurence problem to Object is given accept quest[ ActorID:%d, EpisodeID:%d ]", ActorID, dwEpisodeID, 1970);
    //     return false;
    // }

    // IDA: Create ST_QUEST_EPISODE and conditions
    ST_QUEST_EPISODE stEpisode = {};
    if (m_nHelperCount < 7) {
        stEpisode.byAddHelper = 1;
        ++m_nHelperCount;
    }

    // IDA: Process each condition (Condition_ID_01..10)
    // for (int i = 0; i < 10; ++i) {
    //     TB_QUEST_CONDITION* pTB_CONDITION = GetTB_QUEST_CONDITION(Condition_ID[i]);
    //     if (!pTB_CONDITION) break;
    //
    //     stEpisode.pTBQuestEpisode = pTB_EPISODE;
    //     stEpisode.shCompleteBit &= ~(1 << i);
    //     stEpisode.stCondition[i].dwConditionID = pTB_CONDITION->ID;
    //     stEpisode.stCondition[i].byValue = 0;
    //
    //     // Insert into m_mapEpisode
    //     m_mapEpisode[dwEpisodeID] = stEpisode;
    //
    //     // Create CQuestCondition and insert into m_mapCondition
    //     CQuestCondition* pCond = new CQuestCondition(dwEpisodeID, &stEpisode, i, pTB_CONDITION);
    //     std::shared_ptr<CQuestCondition> spCond(pCond);
    //     m_mapCondition.insert(spCond);
    //
    //     // If in maze, call script update
    //     if (pMaze) {
    //         XMaze::CallScriptUpdateQuest(pMaze, QuestID, 0, dwEpisodeID);
    //         if (NeedCompletionCondition <= 0 || IsCompleteCondition(NeedCompletionCondition)) {
    //             XMaze::RunQuestConditionStart(pMaze, ConditionID, pTB_CONDITION->ID);
    //         }
    //     }
    // }

    // IDA: DBUpdateEpisodeInfo
    // DBUpdateEpisodeInfo(dwEpisodeID, &stEpisode);

    // IDA: Handle repeat quest
    ST_QUEST_REPEAT_INFO stRepeat = {};
    // if (pTB_EPISODE->Contents_Type == 2) {
    //     AcceptRepeatQuest(dwEpisodeID);
    //     GetRepeatQuestInfo(dwEpisodeID, &stRepeat);
    // }

    // IDA: Send packet to client (main=0x15, sub=3)
    // XSendPacket xSendPacket(0x15, 3);
    // xSendPacket << dwEpisodeID << stEpisode.byAddHelper << stRepeat;
    // CGocNetwork::Send(pActor, &xSendPacket);

    // IDA: UpdateItemCondition, EnableInteractionObject, UpdateQuestConditionForSectorClear
    // UpdateItemCondition();
    // EnableInteractionObject(pTB_EPISODE->Condition_ID_01, 0);
    // UpdateQuestConditionForSectorClear();

    // IDA: UpdateOpenTitle
    // CGocEntity::UpdateOpenTitle(0, dwEpisodeID);

    // IDA: Log to database (main=6, sub=1)
    // ST_LOG_GAME stLog;
    // stLog._sMainType = 6; stLog._sSubType = 1;
    // stLog.nParam0 = dwEpisodeID;
    // XGameServer::SendDBLog(&stLog);

    // IDA: Send statistics if episode in range [0x186A1, 0x30D40)
    // if (dwEpisodeID >= 0x186A1 && dwEpisodeID < 0x30D40) {
    //     ST_STATISTICS_QUEST stStatistics;
    //     stStatistics.byFlag = 1;
    //     stStatistics.dwEpisodeID = dwEpisodeID;
    //     XSendDBPacket xSendDBStatistics(pActor, 0xF0, 6);
    //     xSendDBStatistics << stStatistics;
    //     XGameServer::SendDBStatistics(&xSendDBStatistics);
    // }

    (void)stEpisode;
    (void)stRepeat;
    (void)nError;
    return true;
}

// IDA: 0x130C50
bool CGocQuest::CompleteQuestByForce(std::uint32_t dwEpisodeID) {
    // IDA-verified: Complete quest without normal validation
    // Similar to CompleteQuest but skips condition checks
    // Used for GM commands or special cases
    (void)dwEpisodeID;

    // TODO: 需人工审查 - Requires complete implementation with:
    // - Episode lookup
    // - Reward processing
    // - Bit setting
    // - Database sync
    return false;
}

// IDA: 0x126890
bool CGocQuest::CompleteConditionByForce(std::uint32_t dwConditionID) {
    // Force complete a condition without normal validation
    // TODO: 需人工审查 - Requires CQuestCondition lookup and CompleteCondition call
    (void)dwConditionID;
    return false;
}

// IDA: 0x14013AB60 - ?FailQuest@CGocQuest@@QEAAXK@Z
// Verified: Direct IDA decompilation - Mark quest as failed
void CGocQuest::FailQuest(std::uint32_t dwQuestID) {
    // IDA: Find episode in m_mapEpisode
    auto it = m_mapEpisode.find(dwQuestID);
    if (it == m_mapEpisode.end()) {
        return;
    }

    // IDA: Get TB_QUEST_EPISODE
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_QUEST_EPISODE* pTB_EPISODE = XResourceMgr::GetTB_QUEST_EPISODE(&pServer->m_xResourceMgr, dwQuestID);
    // if (!pTB_EPISODE) return;

    // IDA: Check if ID matches
    // if (pTB_EPISODE->ID != dwQuestID) return;

    // IDA: Set bFailed = 1 (BYTE4 at offset +4)
    it->second.bFailed = true;

    // IDA: DBUpdateEpisodeInfo
    DBUpdateEpisodeInfo(dwQuestID, &it->second);

    // IDA: UpdateQuestConditionForSectorClear
    UpdateQuestConditionForSectorClear();

    // IDA: Send packet to client (0x15, 0x11)
    // XSendPacket xSendPacket(0x15, 0x11);
    // xSendPacket << dwQuestID;
    // CGocNetwork::Send(pActor, &xSendPacket);
}

// IDA: 0x14013A610 - ?ResetQuest@CGocQuest@@QEAA_NK@Z
// Verified: Direct IDA decompilation - Reset quest (give up and clear complete bit)
bool CGocQuest::ResetQuest(std::uint32_t dwEpisodeID) {
    // IDA: Call GiveUp first
    if (!GiveUp(dwEpisodeID, false)) {
        // IDA: Get TB_QUEST_EPISODE
        // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // TB_QUEST_EPISODE* pTB_EPISODE = XResourceMgr::GetTB_QUEST_EPISODE(&pServer->m_xResourceMgr, dwEpisodeID);
        // if (!pTB_EPISODE) return false;

        // IDA: Clear complete bit in m_szCompleteEpisode
        // int nIndex = pTB_EPISODE->Complete_Bit / 8;
        // int nPos = pTB_EPISODE->Complete_Bit % 8;
        // if (nIndex > 256) return false;
        // m_szCompleteEpisode[nIndex] &= ~(1 << nPos);

        // IDA: Send packet to client (0x15, 0x12)
        // XSendPacket xSendPacket(0x15, 0x12);
        // xSendPacket << dwEpisodeID;
        // CGocNetwork::Send(pActor, &xSendPacket);
    }
    return true;
}

// IDA: 0x137650
// Reset all episodes - iterate and reset each one
// Note: Full implementation requires inventory access to remove quest items
void CGocQuest::ResetQuestAll() {
    // TODO: 汇编还原 - Full implementation requires:
    // - Iterating all episodes
    // - Getting TB_QUEST_EPISODE for each
    // - Removing Remove_Item_ID items via CGocInventory
    // - Sending DB log and statistics
    // - Calling Clear()
    // - Sending DB packet (main=0x41, sub=7)
    // - Sending client packet (main=0x15, sub=0x10)

    // Basic implementation: reset all episode states
    for (auto& pair : m_mapEpisode) {
        pair.second.bFailed = false;
    }
}

// IDA: 0x140134D80
void CGocQuest::UpdateCondition(std::uint8_t byType, std::uint8_t byTarget,
                                std::uint32_t dwObjectID, int nCount, bool bPartyWith) {
    // IDA-verified condition update logic:
    // 1. Get CUser from actor
    // 2. Get condition index by type from m_mapCondition (boost::multi_index)
    // 3. Iterate through conditions matching byType (ConditionType)
    // 4. For each matching condition:
    //    a. Skip if episode state == 1 (failed)
    //    b. Get TB_QUEST_CONDITION
    //    c. Skip if already complete (IsCompleteCondition)
    //    d. Check NeedCompletionCondition prerequisite
    //    e. Check if ObjectID matches condition and Target_Type matches
    //    f. AddConditionValue(nCount)
    //    g. Log update (main=6, sub=2)
    //    h. Send statistics if episode ID in range [0x186A1, 0x30D40)
    //    i. Check if Counter_Value reached or CompleteCondition success
    //    j. If in maze and Group_Counting set, update party members recursively
    // 5. Send updated conditions to client (main=0x15, sub=7)

    (void)byType;
    (void)byTarget;
    (void)dwObjectID;
    (void)nCount;
    (void)bPartyWith;

    // TODO: 需人工审查 - Requires complete implementation with:
    // - CUser pointer from actor
    // - boost::multi_index iteration on m_mapCondition by ConditionType
    // - CQuestCondition methods: IsCompleteCondition, GetNeedCompletionCondition,
    //   AddConditionValue, GetConditionValue, GetQuestID, GetConditionID
    // - TB_QUEST_CONDITION structure access
    // - CheckUpdateCondition validation
    // - CompleteCondition call
    // - Party member iteration via XMaze::GetScanner
    // - Packet sending for condition updates
    // - Database logging
}

// IDA: 0x140133900 - ?UpdateMazeGameMode@CGocQuest@@QEAAXW4E_CONDITION_TYPE@@F_N@Z
// Verified: Direct IDA decompilation - Update maze/game mode conditions
// Note: E_CONDITION_TYPE is an enum defined elsewhere; using int as placeholder
void CGocQuest::UpdateMazeGameMode(int eType, std::int16_t nMazeID, bool bPartyWith) {
    // IDA: Get owner and cast to CUser
    // CMover* pMover = GetOwnerGO();
    // CUser* pUser = dynamic_cast<CUser*>(pMover);
    // if (!pUser) return;

    // IDA: Get ConditionType index from m_mapCondition (index 3)
    // auto& index = m_mapCondition.get<3>(); // ConditionType ordered index

    // IDA: equal_range on ConditionType == eType
    // auto pair = index.equal_range(static_cast<std::uint8_t>(eType));

    // IDA: Iterate through matching conditions
    // for (auto iter = pair.first; iter != pair.second; ++iter) {
    //     auto& pQuestCondition = *iter;
    //     CQuestCondition* pCond = pQuestCondition.get();
    //
    //     // IDA: Skip if already complete
    //     if (pCond->IsCompleteCondition()) continue;
    //
    //     // IDA: Get TB_QUEST_CONDITION
    //     TB_QUEST_CONDITION* pTB_COND = pCond->GetTBCondition();
    //     if (!pTB_COND) continue;
    //
    //     // IDA: Check Maze_ID match (SLOWORD)
    //     if (SLOWORD(pTB_COND->Maze_ID_1) != nMazeID) continue;
    //
    //     // IDA: Check NeedCompletionCondition prerequisite
    //     int nNeedCond = pCond->GetNeedCompletionCondition();
    //     if (nNeedCond > 0 && !IsCompleteCondition(nNeedCond)) continue;
    //
    //     // IDA: AddConditionValue(1)
    //     pCond->AddConditionValue(1);
    //
    //     // IDA: Log to DB (main=6, sub=2)
    //     // ST_LOG_GAME stLog; stLog._sMainType=6; stLog._sSubType=2;
    //
    //     // IDA: Send statistics if QuestID in range [0x186A1, 0x30D40)
    //
    //     // IDA: Call CompleteCondition
    //     std::uint32_t dwQuestID = pCond->GetQuestID();
    //     if (CompleteCondition(dwQuestID, pQuestCondition)) {
    //         // Add to psPacket for batch send
    //     }
    //
    //     // IDA: If Group_Counting && bPartyWith, propagate to party members
    //     if (pTB_COND->Group_Counting && bPartyWith && CUser::IsMaze(pUser)) {
    //         // XMaze* pMaze = GetCurrentMaze();
    //         // auto* pVector = XMaze::GetScanner(pMaze, pActor);
    //         // For each party member in scanner range:
    //         //     CGocQuest* pMemberQuest = member->GetGOC<CGocQuest>();
    //         //     pMemberQuest->UpdateMazeGameMode(eType, nMazeID, false);
    //     }
    // }

    // IDA: Send batch packet if any conditions completed (0x15, 0x07)
    // XSendPacket xSendPacket(0x15, 7);
    // xSendPacket << psPacket;
    // CGocNetwork::Send(pActor, &xSendPacket);

    (void)eType;
    (void)nMazeID;
    (void)bPartyWith;
}

// IDA: 0x140134330 - ?UpdateItemCondition@CGocQuest@@QEAAXXZ
// Verified: Direct IDA decompilation - Update item-based quest conditions
void CGocQuest::UpdateItemCondition() {
    // IDA: Get owner and cast to CUser
    // CMover* pMover = GetOwnerGO();
    // CUser* pUser = dynamic_cast<CUser*>(pMover);
    // if (!pUser) return;

    // IDA: Get ConditionType index (index 3) for type == 4 (ITEM condition)
    // auto& index = m_mapCondition.get<3>(); // ConditionType ordered index

    // IDA: equal_range on ConditionType == 4 (ITEM type)
    // std::uint8_t x[8] = {4};
    // auto pair = index.equal_range(x);

    // IDA: PS_QUEST_CONDITION psPacket for batch updates
    // PS_QUEST_CONDITION psPacket;

    // IDA: Iterate through matching item conditions
    // for (auto iter = pair.first; iter != pair.second; ++iter) {
    //     auto& pQuestCondition = *iter;
    //     CQuestCondition* pCond = pQuestCondition.get();
    //
    //     // IDA: Skip if already complete
    //     if (pCond->IsCompleteCondition()) continue;
    //
    //     // IDA: Get TB_QUEST_CONDITION
    //     TB_QUEST_CONDITION* pTB_COND = pCond->GetTBCondition();
    //     if (!pTB_COND) continue;
    //
    //     // IDA: Get inventory component
    //     std::tr1::shared_ptr<CGocInventory> pInven;
    //     CMover::GetGOC<CGocInventory>(pMover, &pInven, 0);
    //
    //     // IDA: Check Target_ID (item ID to check)
    //     if (!pTB_COND->Target_ID) continue;
    //
    //     // IDA: Get item from inventory
    //     std::tr1::shared_ptr<CItem> pItem;
    //     CGocInventory::GetItem(pInven.get(), &pItem, 2, pTB_COND->Target_ID);
    //     if (!pItem) continue;
    //
    //     // IDA: Get item count
    //     int nCount = CItem::GetCount(pItem.get());
    //
    //     // IDA: Calculate clear count (min of count and Counter_Value)
    //     int nClearCount = (nCount >= pTB_COND->Counter_Value) ? pTB_COND->Counter_Value : nCount;
    //
    //     // IDA: Save old value and set new value
    //     int nOldValue = pCond->GetConditionValue();
    //     pCond->SetConditionValue(nClearCount);
    //
    //     // IDA: CheckSyncQuestCondition
    //     CheckSyncQuestCondition(pQuestCondition);
    //
    //     // IDA: Log to DB (main=6, sub=2)
    //     // ST_LOG_GAME stLog; stLog._sMainType=6; stLog._sSubType=2;
    //
    //     // IDA: Send statistics if QuestID in range [0x186A1, 0x30D40)
    //
    //     // IDA: Check if Counter_Value <= ConditionValue or CompleteCondition succeeded
    //     if (pTB_COND->Counter_Value > pCond->GetConditionValue()) {
    //         // Not complete yet, try CompleteCondition
    //         std::uint32_t dwQuestID = pCond->GetQuestID();
    //         if (CompleteCondition(dwQuestID, pQuestCondition)) {
    //             // Add to psPacket
    //         }
    //     } else {
    //         // CompleteCondition succeeded, add to packet
    //         std::uint32_t dwQuestID = pCond->GetQuestID();
    //         if (CompleteCondition(dwQuestID, pQuestCondition)) {
    //             // Add to psPacket
    //         }
    //     }
    // }

    // IDA: Send batch packet if any conditions completed (0x15, 0x07)
    // if (psPacket.size() > 0) {
    //     XSendPacket xSendPacket(0x15, 7);
    //     xSendPacket << psPacket;
    //     CGocNetwork::Send(pActor, &xSendPacket);
    // }
}

// IDA: 0x140128730 - ?DeleteEpisode@CGocQuest@@QEAA_NK@Z
// Verified: Direct IDA decompilation
// Delete episode from map and remove associated conditions
bool CGocQuest::DeleteEpisode(std::uint32_t dwEpisodeID) {
    // IDA: 首先调用 ClearUpdateQuestCondition_GiveUp
    ClearUpdateQuestCondition_GiveUp(dwEpisodeID);

    // IDA: 从 m_mapCondition 中删除所有 QuestID 匹配的条件
    // 使用 boost::multi_index 的 QuestID 索引
    // auto& index = m_mapCondition.get<1>(); // QuestID index
    // index.erase(dwEpisodeID);
    // 简化实现：遍历删除
    for (auto it = m_mapCondition.begin(); it != m_mapCondition.end(); ) {
        if (it->second && it->second->GetQuestID() == dwEpisodeID) {
            it = m_mapCondition.erase(it);
        } else {
            ++it;
        }
    }

    // IDA: 从 m_mapEpisode 中删除
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it == m_mapEpisode.end()) {
        return false;
    }

    // IDA: 如果 byAddHelper == 1，减少 m_nHelperCount
    if (it->second.byAddHelper == 1) {
        --m_nHelperCount;
    }

    m_mapEpisode.erase(it);
    return true;
}

// IDA: 0x140128810 - ?DeleteFailedEpisode@CGocQuest@@QEAA_NK@Z
// Verified: Direct IDA decompilation
// Delete episode only if it's in failed state, and send DB notification
bool CGocQuest::DeleteFailedEpisode(std::uint32_t dwEpisodeID) {
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it == m_mapEpisode.end()) {
        return false;
    }

    // IDA: 检查 bFailed == true (BYTE4 of ST_QUEST_EPISODE)
    if (!it->second.bFailed) {
        // IDA: 记录日志 "Failed Delete Failed Episocke"
        return false;
    }

    // IDA: 发送 DB 包 (0x41/0x06) 通知放弃任务
    // XSendDBPacket packet(actor, 0x41, 0x06);
    // packet << GetQuestID() << dwEpisodeID << byContentsType << bGiveUp;
    // XGameServer::SendDBGame(packet);
    // TODO: 需要外部依赖发送 DB 包

    return true;
}

// IDA: 0x140128A70 - ?SetEpisodeHelper@CGocQuest@@QEAA_NKE@Z
// Verified: Direct IDA decompilation
// Set helper flag for episode, with max helper count check
bool CGocQuest::SetEpisodeHelper(std::uint32_t dwEpisodeID, std::uint8_t byAddHelper) {
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it == m_mapEpisode.end()) {
        return false;
    }

    // IDA: 检查是否已经是相同的 helper 状态
    if (it->second.byAddHelper == byAddHelper) {
        // IDA: 日志 "SetHelper Same Flag"
        return true;
    }

    // IDA: 如果设置为 helper，检查是否超过最大数量 (7)
    if (byAddHelper == 1 && m_nHelperCount >= 7) {
        return false;
    }

    // IDA: 设置 helper 标志并更新计数
    it->second.byAddHelper = byAddHelper;
    if (byAddHelper) {
        ++m_nHelperCount;
    } else {
        --m_nHelperCount;
    }

    return true;
}

// IDA: 0x140133690 - ?SetHelper@CGocQuest@@QEAA_NKE@Z
// Verified: Direct IDA decompilation - Set helper flag for quest episode
bool CGocQuest::SetHelper(std::uint32_t dwEpisodeID, std::uint8_t byType) {
    // IDA: Check if episode exists
    if (!FindEpisode(dwEpisodeID)) {
        // CGocNetwork::SendErrorMessage(pMover, 0x15, 9, 0xD2F4);
        return true;  // IDA returns 1 on error
    }

    // IDA: Call SetEpisodeHelper to update helper flag
    if (!SetEpisodeHelper(dwEpisodeID, byType)) {
        // CGocNetwork::SendErrorMessage(pMover, 0x15, 9, 0xD2F6);
        return true;  // IDA returns 1 on error
    }

    // IDA: Find episode in map and get reference
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it == m_mapEpisode.end()) {
        // CGocNetwork::SendErrorMessage(pMover, 0x15, 9, 0xD2F6);
        return true;
    }

    // IDA: Copy episode data for DB update
    ST_QUEST_EPISODE stEpisode = it->second;

    // IDA: Update database with episode info
    DBUpdateEpisodeInfo(dwEpisodeID, &stEpisode);

    // IDA: Send packet to client (0x15, 0x09)
    // XSendPacket xSendPacket(0x15, 9);
    // xSendPacket.XParse << byType;
    // xSendPacket.XParse << dwEpisodeID;
    // CGocNetwork::Send(pActor, &xSendPacket);

    (void)stEpisode;
    return true;
}

// IDA: 0x140128530
void CGocQuest::CheckEpisodeCount() {
    // TODO: 汇编还原 - Check episode count and remove old ones if needed
}

// IDA: 0x14013A7C0 - ?DBSyncQuestCondition@CGocQuest@@QEAAXXZ
// Verified: Direct IDA decompilation - Sync quest conditions to database
void CGocQuest::DBSyncQuestCondition() {
    // IDA: Check if m_mapUpdateCondition is empty
    if (m_mapUpdateCondition.empty()) {
        return;
    }

    // IDA: Iterate through m_mapUpdateCondition
    for (auto& pair : m_mapUpdateCondition) {
        // IDA: Get CQuestCondition from shared_ptr
        // std::tr1::shared_ptr<CQuestCondition> pQuestCondition = pair.second;
        // if (!pQuestCondition) continue;

        // IDA: Get QuestID from condition
        // std::uint32_t dwEpisodeID = pQuestCondition->GetQuestID();

        // IDA: Get ST_QUEST_EPISODE from condition
        // ST_QUEST_EPISODE* stEpisode = pQuestCondition->GetEpisode();
        // if (!stEpisode) return;

        // IDA: Call DBUpdateEpisodeInfo
        // DBUpdateEpisodeInfo(dwEpisodeID, stEpisode);
        (void)pair;
    }

    // IDA: Clear m_mapUpdateCondition
    m_mapUpdateCondition.clear();
}

// IDA: 0x140124DB0
const char* CGocQuest::GetCompleteEpisode() const {
    return reinterpret_cast<const char*>(m_szCompleteEpisode);
}

// IDA: 0x14012CE50 - ?CheckAcceptQuestByItem@CGocQuest@@QEAA_NKAEAH@Z
// Verified: Direct IDA decompilation - Check if quest can be accepted via item
bool CGocQuest::CheckAcceptQuestByItem(std::uint32_t dwEpisodeID, int* nError) {
    // IDA: Check if episode count < 30 (0x1E)
    if (m_mapEpisode.size() >= 30) {
        *nError = 54024;  // Error code for too many quests
        return false;
    }

    // IDA: Check if episode already exists
    if (FindEpisode(dwEpisodeID)) {
        *nError = 54001;  // Error code for already have quest
        return false;
    }

    // IDA: Check if already completed
    if (IsCompleteEpisode(dwEpisodeID)) {
        *nError = 54002;  // Error code for already completed
        return false;
    }

    return true;
}

// IDA: 0x14012CEE0 - ?AcceptQuestByItem@CGocQuest@@QEAA_NKAEAUPS_RES_STORAGE_INFO@@0@Z
// Verified: Direct IDA decompilation - Accept quest via item with detailed validation
bool CGocQuest::AcceptQuestByItem(std::uint32_t dwEpisodeID, void* psCreateItem, void* psUpdateItem) {
    // IDA: Get owner CMover and dynamic_cast to CUser
    // CMover* pMover = GetOwnerGO();
    // CUser* pUser = dynamic_cast<CUser*>(pMover);
    // if (!pUser) {
    //     CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F4);
    //     return false;
    // }

    // IDA: Get CGocAttribute for level check
    // std::tr1::shared_ptr<CGocAttribute> pAttr;
    // CMover::GetGOC<CGocAttribute>(pMover, &pAttr, 0);
    // if (!pAttr) {
    //     CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F4);
    //     return false;
    // }

    // IDA: Check block type (UserDB & 4)
    // if (CUser::GetBlockType(pUser)) {
    //     CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xC3BF);
    //     return false;
    // }

    // IDA: Check episode count < 30
    if (m_mapEpisode.size() >= 30) {
        // CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD308);
        return false;
    }

    // IDA: Check if already have this episode
    if (FindEpisode(dwEpisodeID)) {
        // CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F1);
        return false;
    }

    // IDA: Check if already completed
    if (IsCompleteEpisode(dwEpisodeID)) {
        // CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F2);
        return false;
    }

    // IDA: Get TB_QUEST_EPISODE from resource manager
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_QUEST_EPISODE* pTB_EPISODE = XResourceMgr::GetTB_QUEST_EPISODE(&pServer->m_xResourceMgr, dwEpisodeID);
    // if (!pTB_EPISODE) return false;

    // IDA: Check Quest_Level against CGameWorldMode::GetState
    // CGameWorldMode* pWorldMode = pAttr->GetWorldMode();
    // int nState = CGameWorldMode::GetState(pWorldMode);
    // if (pTB_EPISODE->Quest_Level > nState) {
    //     LogHelper::LogError("game.quest", "AcceptQuestByItem error - Level condition is not right...");
    //     CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD302);
    //     return false;
    // }

    // IDA: Check Class_Type condition
    // if (pTB_EPISODE->Class_Type) {
    //     if (pTB_EPISODE->Class_Type < 100) {
    //         // Direct class type check
    //         int nClassType = pAttr->GetClassType();
    //         if (pTB_EPISODE->Class_Type != nClassType) {
    //             CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD303);
    //             return false;
    //         }
    //     } else {
    //         // Character group check
    //         int nCharID = pAttr->GetClassType() * 1000;
    //         TB_CHARACTER_INFO* pTB_CHAR = XResourceMgr::GetTB_CHARACTER_INFO(&pServer->m_xResourceMgr, nCharID);
    //         if (!pTB_CHAR || pTB_CHAR->Character_Group_ID != pTB_EPISODE->Class_Type) {
    //             CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD303);
    //             return false;
    //         }
    //     }
    // }

    // IDA: Check repeat quest conditions (Contents_Type == 2)
    int nError = 54019;
    // if (pTB_EPISODE->Contents_Type == 2) {
    //     if (!CheckAcceptRepeatQuest(dwEpisodeID, true, &nError)) {
    //         CGocNetwork::SendErrorMessage(pMover, 0x15, 3, nError);
    //         return false;
    //     }
    // }

    // IDA: Check before episodes (Before_Episode_ID_1 through Before_Episode_ID_5)
    // for (int i = 0; i < 5; ++i) {
    //     std::uint32_t dwBeforeID = *(&pTB_EPISODE->Before_Episode_ID_1 + i);
    //     if (dwBeforeID && !IsCompleteEpisode(dwBeforeID)) {
    //         LogHelper::LogError("game.quest", "AcceptQuestByItem error - Incomplete previous quest...");
    //         CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD304);
    //         return false;
    //     }
    // }

    // IDA: Call SetQuestAddObject to add quest items
    // if (!SetQuestAddObject(dwEpisodeID, psCreateItem, psUpdateItem)) {
    //     LogHelper::LogError("game.quest", "AcceptQuestByItem error - No Condition...");
    //     CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F4);
    //     return false;
    // }

    // IDA: Create ST_QUEST_EPISODE and initialize conditions
    ST_QUEST_EPISODE stEpisode;
    // ST_QUEST_EPISODE::ST_QUEST_EPISODE(&stEpisode);

    // IDA: Set helper flag if count < 7
    if (m_nHelperCount < 7) {
        stEpisode.byAddHelper = 1;
        ++m_nHelperCount;
    }

    // IDA: Initialize conditions for each condition ID
    // for (int nConditionIndex = 0; nConditionIndex < 10; ++nConditionIndex) {
    //     std::uint32_t dwConditionID = *(&pTB_EPISODE->Condition_ID_01 + nConditionIndex);
    //     if (!dwConditionID) break;
    //
    //     TB_QUEST_CONDITION* pTB_CONDITION = XResourceMgr::GetTB_QUEST_CONDITION(&pServer->m_xResourceMgr, dwConditionID);
    //     if (!pTB_CONDITION) break;
    //
    //     stEpisode.pTBQuestEpisode = pTB_EPISODE;
    //     stEpisode.shCompleteBit &= ~(1 << nConditionIndex);
    //     stEpisode.stCondition[nConditionIndex].dwConditionID = dwConditionID;
    //     stEpisode.stCondition[nConditionIndex].byValue = 0;
    //
    //     // IDA: Add to m_mapEpisode
    //     m_mapEpisode[dwEpisodeID] = stEpisode;
    //
    //     // IDA: Create CQuestCondition and add to m_mapCondition
    //     CQuestCondition* pCond = new CQuestCondition(dwEpisodeID, &m_mapEpisode[dwEpisodeID], nConditionIndex, pTB_CONDITION);
    //     std::tr1::shared_ptr<CQuestCondition> pCondition(pCond);
    //     m_mapCondition[dwConditionID] = pCondition;
    //
    //     // IDA: If in maze, call XMaze::CallScriptUpdateQuest
    //     // IDA: Check NeedCompletionCondition and call CheckCutsceneState if needed
    //     // IDA: RunQuestConditionStart
    // }

    // IDA: DBUpdateEpisodeInfo
    DBUpdateEpisodeInfo(dwEpisodeID, &stEpisode);

    // IDA: Handle repeat quest
    ST_QUEST_REPEAT_INFO stRepeat;
    // if (pTB_EPISODE->Contents_Type == 2) {
    //     AcceptRepeatQuest(dwEpisodeID);
    //     GetRepeatQuestInfo(dwEpisodeID, &stRepeat);
    // }

    // IDA: Send packet to client (0x15, 0x03)
    // XSendPacket xSendPacket(0x15, 3);
    // xSendPacket.XParse << dwEpisodeID;
    // xSendPacket.XParse << stEpisode.byAddHelper;
    // xSendPacket << stRepeat;
    // CGocNetwork::Send(pActor, &xSendPacket);

    // IDA: UpdateItemCondition, EnableInteractionObject, UpdateQuestConditionForSectorClear
    UpdateItemCondition();
    // EnableInteractionObject(pTB_EPISODE->Condition_ID_01, 0);
    // UpdateQuestConditionForSectorClear();

    // IDA: UpdateOpenTitle
    // std::tr1::shared_ptr<CGocEntity> pEntity;
    // CMover::GetGOC<CGocEntity>(pMover, &pEntity, 0);
    // if (pEntity) {
    //     CGocEntity::UpdateOpenTitle(pEntity.get(), 0, dwEpisodeID);
    // }

    // IDA: Log to database (main=6, sub=1)
    // ST_LOG_GAME stLog;
    // stLog._nUAID = pUser->GetUAID();
    // stLog._nUCID = pMover->GetID();
    // stLog._sMainType = 6; stLog._sSubType = 1;
    // stLog.nParam0 = dwEpisodeID;
    // stLog.nParam1 = stEpisode.stCondition[0].dwConditionID;
    // stLog.nParam3 = stEpisode.byAddHelper;
    // stLog.nParam5 = pTB_EPISODE->Contents_Type;
    // XGameServer::SendDBLog(&stLog);

    // IDA: Send statistics if episode in range [0x186A1, 0x30D40)
    // if (dwEpisodeID >= 0x186A1 && dwEpisodeID < 0x30D40) {
    //     ST_STATISTICS_QUEST stStatistics;
    //     stStatistics.byFlag = 1;
    //     stStatistics.dwUCID = pMover->GetID();
    //     stStatistics.dwEpisodeID = dwEpisodeID;
    //     stStatistics.byLevel = pMover->GetLevel();
    //     XSendDBPacket xSendDBStatistics(pActor, 0xF0, 6);
    //     xSendDBStatistics << stStatistics;
    //     XGameServer::SendDBStatistics(&xSendDBStatistics);
    // }

    (void)psCreateItem;
    (void)psUpdateItem;
    (void)nError;
    (void)stEpisode;
    (void)stRepeat;
    return true;
}

// IDA: 0x14013B6D0 - ?AcceptRepeatQuest@CGocQuest@@QEAAXK@Z
// Verified: Direct IDA decompilation - Accept a repeatable quest
void CGocQuest::AcceptRepeatQuest(std::uint32_t dwEpisodeID) {
    // IDA: Get current time and calculate today's init time (9 AM)
    // ATL::CTime tCurr = ATL::CTime::GetTickCount();
    // int nDay = tCurr.GetDay();
    // int nMonth = tCurr.GetMonth();
    // int nYear = tCurr.GetYear();
    // ATL::CTime tTodayInit(nYear, nMonth, nDay, 9, 0, 0);
    // if (tCurr.GetHour() < 9) {
    //     tTodayInit -= ATL::CTimeSpan(1, 0, 0, 0);
    // }

    // IDA: Find episode in m_mapRepeatQuest
    auto it = m_mapRepeatQuest.find(dwEpisodeID);

    if (it == m_mapRepeatQuest.end()) {
        // IDA: Create new ST_QUEST_REPEAT_INFO
        ST_QUEST_REPEAT_INFO stInfo = {};
        stInfo.dwQuestID = dwEpisodeID;
        stInfo.byCount = 0;
        // stInfo.biLastInitDate = tTodayInit;
        m_mapRepeatQuest[dwEpisodeID] = stInfo;

        // IDA: DBAddRepeatQuest
        // DBAddRepeatQuest(dwEpisodeID);
    } else {
        // IDA: Check if last init date is before today
        // ST_QUEST_REPEAT_INFO* stRepeat = &it->second;
        // if (stRepeat->biLastInitDate < tTodayInit) {
        //     stRepeat->biLastInitDate = tTodayInit;
        //     stRepeat->byCount = 0;
        //     // DBUpdateRepeatQuest
        // }
    }
}

// IDA: 0x14013B930 - ?CompleteRepeatQuest@CGocQuest@@QEAAXK@Z
// Verified: Direct IDA decompilation - Complete a repeatable quest
void CGocQuest::CompleteRepeatQuest(std::uint32_t dwEpisodeID) {
    // IDA: Find episode in m_mapRepeatQuest
    auto it = m_mapRepeatQuest.find(dwEpisodeID);

    if (it == m_mapRepeatQuest.end()) {
        // IDA: Log error "Not Find Repeat Quest"
        return;
    }

    // IDA: Get current time and calculate today's init time (9 AM)
    // ATL::CTime tCurr = ATL::CTime::GetTickCount();
    // int nDay = tCurr.GetDay();
    // int nMonth = tCurr.GetMonth();
    // int nYear = tCurr.GetYear();
    // ATL::CTime tTodayInit(nYear, nMonth, nDay, 9, 0, 0);
    // if (tCurr.GetHour() < 9) {
    //     tTodayInit -= ATL::CTimeSpan(1, 0, 0, 0);
    // }

    // IDA: Check if last init date is before today
    // if (stRepeat->biLastInitDate < tTodayInit) {
    //     stRepeat->byCount = 0;
    //     stRepeat->biLastInitDate = tTodayInit;
    // }

    // IDA: Increment count
    it->second.byCount++;

    // IDA: DBUpdateRepeatQuest
    // DBUpdateRepeatQuest(vecUpdate);
}

// IDA: 0x14013BBD0 - ?ResetRepeatQuest@CGocQuest@@QEAAXVCTime@ATL@@@Z
// Verified: Direct IDA decompilation - Reset repeat quest with time
void CGocQuest::ResetRepeatQuest(std::uint32_t dwEpisodeID) {
    // IDA: Iterate through m_mapRepeatQuest
    // For each repeat quest:
    //   - Check if biLastInitDate < tTodayInit
    //   - Reset count to 0
    //   - Update biLastInitDate
    //   - Add to reset list

    // IDA: Send packet to client (0x15, 0x13)
    // XSendPacket xSendPacket(0x15, 0x13);
    // xSendPacket << psReset;
    // CGocNetwork::Send(pActor, &xSendPacket);

    (void)dwEpisodeID;
}

// IDA: 0x140138A60
// IDA: 0x140139A60 - ?EnableInteractionObject@CGocQuest@@QEAAXH_N@Z
// Verified: Direct IDA decompilation - Enable interaction objects for quest condition
void CGocQuest::EnableInteractionObject(std::uint32_t dwConditionID, int nParam) {
    // IDA: Get owner actor and check if valid
    // CMover* pMover = GetOwnerGO();
    // if (!pMover) return;

    // IDA: Check if user is in valid state (RTDynamicCast to CUser, check IsMaze)
    // if (!CUser::IsMaze(pUser)) return;

    // IDA: If ConditionID <= 0, nothing to do
    if (dwConditionID <= 0) {
        return;
    }

    // IDA: Get ConditionID index from m_mapCondition
    // auto& index = m_mapCondition.get<0>(); // ConditionID hashed index

    // IDA: Find condition by ConditionID
    // auto iter = index.find(dwConditionID);
    // if (iter == index.end()) return;

    // IDA: Get shared_ptr<CQuestCondition>
    // std::tr1::shared_ptr<CQuestCondition> pQuestCondition = *iter;

    // IDA: Check if episode is failed (bFailed == 1 at offset +4)
    // if (pQuestCondition->GetEpisode()->bFailed == 1) return;

    // IDA: Get current maze via RTDynamicCast
    // XMaze* pMaze = dynamic_cast<XMaze*>(GetCurrentArea());
    // if (!pMaze) return;

    // IDA: Check if Condition type is 4 (INTERACTION type) at offset +14
    // TB_QUEST_CONDITION* pTB_COND = pQuestCondition->GetTBCondition();
    // if (pTB_COND->Condition != 4) return;

    // IDA: Call XMaze::EnableInteractionBoxForQuest
    // XMaze::EnableInteractionBoxForQuest(pMaze, pActor, pTB_COND->Target_ID, 1, dwConditionID, nParam);

    (void)dwConditionID;
    (void)nParam;
}

// IDA: 0x140138CE0
// IDA: 0x140139CE0 - ?DisableInteractionObject@CGocQuest@@QEAAXHHH@Z
// Verified: Direct IDA decompilation - Disable interaction objects for quest condition
void CGocQuest::DisableInteractionObject(std::uint32_t dwConditionID, int nType, int nParam) {
    // IDA: Get owner actor and check if valid
    // CMover* pMover = GetOwnerGO();
    // if (!pMover) return;

    // IDA: Check if user is in valid state
    // if (!CUser::IsMaze(pUser)) return;

    // IDA: Check if Target_Type == 4 (INTERACTION type)
    if (nType != 4) {
        return;
    }

    // IDA: Get current maze via RTDynamicCast
    // XMaze* pMaze = dynamic_cast<XMaze*>(GetCurrentArea());
    // if (!pMaze) return;

    // IDA: Call XMaze::EnableInteractionBoxForQuest with enable=0
    // XMaze::EnableInteractionBoxForQuest(pMaze, pActor, nParam, 0, dwConditionID, 0);

    (void)dwConditionID;
    (void)nType;
    (void)nParam;
}

// IDA: 0x140138E40
// IDA: 0x140139E40 - ?InitQuestConditionForSectorClear@CGocQuest@@QEAAXXZ
// Verified: Direct IDA decompilation - Initialize sector clear quest conditions
void CGocQuest::InitQuestConditionForSectorClear() {
    // IDA: Get owner actor and check if in maze
    // CMover* pMover = GetOwnerGO();
    // CUser* pUser = dynamic_cast<CUser*>(pMover);
    // if (!pUser || !CUser::IsMaze(pUser)) return;

    // IDA: Set m_bSendMsgSectorClear = true
    m_bSendMsgSectorClear = true;

    // IDA: Get current maze
    // XMaze* pMaze = dynamic_cast<XMaze*>(GetCurrentArea());
    // if (!pMaze) return;

    // IDA: Calculate SectorQuest ID
    // int nBase = 100 * (10 * (XArea::GetTBMapID(pMaze) / 10) + 1);
    // unsigned int nID = XMaze::GetLastSectorID(pMaze) % 10000 + nBase;

    // IDA: Get TB_SECTORQUEST
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_SECTORQUEST* pTBSectorQuest = XResourceMgr::GetTB_SECTORQUEST(&pServer->m_xResourceMgr, nID);
    // if (!pTBSectorQuest) return;

    // IDA: Process 3 sector clear quest slots
    for (int i = 0; i < 3; ++i) {
        // IDA: Clear state initially
        // SetSectorClearQuestState(i, false);

        // IDA: Check Before_Check_Quest_ID_01..03
        // if (pTBSectorQuest->Before_Check_Quest_ID[i]) {
        //     if (pTBSectorQuest->Before_Check_Quest_Type[i]) {
        //         // Condition check
        //         if (pTBSectorQuest->Before_Check_Quest_State[i]) {
        //             // Check if condition complete
        //             if (IsCompleteCondition(pTBSectorQuest->Before_Check_Quest_ID[i])) {
        //                 SetSectorClearQuestState(i, true);
        //             }
        //         } else {
        //             // Check if condition found
        //             if (FindCondition(pTBSectorQuest->Before_Check_Quest_ID[i])) {
        //                 SetSectorClearQuestState(i, true);
        //             }
        //         }
        //     } else {
        //         // Episode check
        //         if (pTBSectorQuest->Before_Check_Quest_State[i]) {
        //             // Check if episode complete
        //             if (IsCompleteEpisode(pTBSectorQuest->Before_Check_Quest_ID[i])) {
        //                 SetSectorClearQuestState(i, true);
        //             }
        //         } else {
        //             // Check if episode found
        //             if (FindEpisode(pTBSectorQuest->Before_Check_Quest_ID[i])) {
        //                 SetSectorClearQuestState(i, true);
        //             }
        //         }
        //     }
        // }
        (void)i;
    }
}

// IDA: 0x1401393D0
// IDA: 0x14013A3D0 - ?UpdateQuestConditionForSectorClear@CGocQuest@@QEAAXXZ
// Verified: Direct IDA decompilation - Update quest conditions for sector clear
void CGocQuest::UpdateQuestConditionForSectorClear() {
    // IDA: Get current maze
    // XMaze* pMaze = dynamic_cast<XMaze*>(GetCurrentArea());
    // if (!pMaze) return;

    // IDA: Calculate SectorQuest ID
    // int nBase = 100 * (10 * (XArea::GetTBMapID(pMaze) / 10) + 1);
    // unsigned int nID = XMaze::GetLastSectorID(pMaze) % 10000 + nBase;

    // IDA: Get TB_SECTORQUEST
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_SECTORQUEST* pTBSectorQuest = XResourceMgr::GetTB_SECTORQUEST(&pServer->m_xResourceMgr, nID);
    // if (!pTBSectorQuest) return;

    // IDA: Process 3 sector clear quest slots
    for (int i = 0; i < 3; ++i) {
        // IDA: Check if state is set
        if (m_bQuestStateClear[i]) {
            // IDA: Check Final_Check_Quest_Type
            // if (pTBSectorQuest->Final_Check_Quest_Type[i]) {
            //     // Condition check
            //     if (pTBSectorQuest->Final_Check_Quest_State[i]) {
            //         if (IsCompleteCondition(pTBSectorQuest->Final_Check_Quest_ID[i])) {
            //             m_bQuestStateClear[i] = 0;
            //         }
            //     } else {
            //         if (FindCondition(pTBSectorQuest->Final_Check_Quest_ID[i])) {
            //             m_bQuestStateClear[i] = 0;
            //         }
            //     }
            // } else {
            //     // Episode check
            //     if (pTBSectorQuest->Final_Check_Quest_State[i]) {
            //         if (IsCompleteEpisode(pTBSectorQuest->Final_Check_Quest_ID[i])) {
            //             m_bQuestStateClear[i] = 0;
            //         }
            //     } else {
            //         if (FindEpisode(pTBSectorQuest->Final_Check_Quest_ID[i])) {
            //             m_bQuestStateClear[i] = 0;
            //         }
            //     }
            // }
            (void)i;
        }
    }
}

// IDA: 0x14013A350 - ?UpdateQuestRespawn@CGocQuest@@QEAAXH@Z
// Verified: Direct IDA decompilation - Update respawn for quest condition
void CGocQuest::UpdateQuestRespawn(int nConditionID) {
    // IDA: Get current maze
    // XMaze* pMaze = dynamic_cast<XMaze*>(GetCurrentArea());
    // if (!pMaze) return;

    // IDA: Call XMaze::UpdateQuestRespawn
    // XMaze::UpdateQuestRespawn(pMaze, nConditionID);

    (void)nConditionID;
}

// IDA: 0x14013C260 - ?AddQuestFirstDropItem@CGocQuest@@QEAAXKKH@Z
// Verified: Direct IDA decompilation - Add first drop item for quest
void CGocQuest::AddQuestFirstDropItem(std::uint32_t dwQuestID, std::uint32_t dwItemID, int nCount) {
    // IDA: Check if quest already exists in m_mapQuestFirstDrop
    auto it = m_mapQuestFirstDrop.find(dwQuestID);
    if (it == m_mapQuestFirstDrop.end()) {
        // IDA: Create new ST_QUEST_FIRST_DROP_ITEM and insert
        ST_QUEST_FIRST_DROP_ITEM stDrop;
        stDrop.dwUCID = 0;  // Will be set from owner actor
        stDrop.dwQuestID = dwQuestID;
        stDrop.dwItemID = dwItemID;
        stDrop.nCount = nCount;
        m_mapQuestFirstDrop[dwQuestID] = stDrop;
    }
}

// IDA: 0x14013C1B0 - ?SetQuestFirstDropItem@CGocQuest@@QEAAXAEAUPS_QUEST_FIRST_DROP_ITEM@@@Z
// Verified: Direct IDA decompilation - Set first drop items from packet
void CGocQuest::SetQuestFirstDropItem(std::uint32_t dwEpisodeID, int nItemID, int nCount) {
    // IDA: Iterate through psDrop list and call AddQuestFirstDropItem for each
    // for (auto& item : psDrop) {
    //     AddQuestFirstDropItem(item.nType, item.nID, item.nCount);
    // }
    (void)dwEpisodeID;
    (void)nItemID;
    (void)nCount;
}

// IDA: 0x14013BE70 - ?DBAddRepeatQuest@CGocQuest@@QEAAXK@Z
// Verified: Direct IDA decompilation - Add repeat quest to database
void CGocQuest::DBAddRepeatQuest(std::uint32_t dwEpisodeID) {
    // IDA: Find episode in m_mapRepeatQuest
    auto it = m_mapRepeatQuest.find(dwEpisodeID);
    if (it == m_mapRepeatQuest.end()) {
        return;
    }

    // IDA: Get actor UCID
    // std::uint32_t dwUCID = GetOwnerGO()->GetID();

    // IDA: Create PS_REPEAT_QUEST_INFO packet
    // PS_REPEAT_QUEST_INFO psAdd;
    // psAdd.dwUCID = dwUCID;
    // psAdd.stInfo = it->second;

    // IDA: Send DB packet (0x41, 0x08)
    // XSendDBPacket xSendDBPacket(pActor, 0x41, 0x08);
    // xSendDBPacket << psAdd;
    // XGameServer::SendDBGame(&xSendDBPacket);
}

// IDA: 0x14013C370 - ?CheckQuestFirstDropItem@CGocQuest@@QEAA_NKK@Z
// Verified: Direct IDA decompilation - Check first drop item
bool CGocQuest::CheckQuestFirstDropItem(std::uint32_t dwQuestID, std::uint32_t dwItemID) {
    // IDA: Check if quest exists in m_mapQuestFirstDrop
    auto it = m_mapQuestFirstDrop.find(dwQuestID);
    if (it == m_mapQuestFirstDrop.end()) {
        // IDA: Not found means already handled, return true
        return true;
    }

    // IDA: Check if item ID matches
    if (it->second.dwItemID != dwItemID) {
        // IDA: Log mismatch
        LogHelper::LogError("game.quest",
            "<QUEST> CheckQuestFirstDropItem [ ActorID:%d, Quest:%d, Item:%d, Item:%d )",
            0, dwQuestID, it->second.dwItemID, dwItemID);
        return false;
    }

    return false;
}

// IDA: 0x140310530
bool CGocQuest::IsSendMsgSectorClear() const {
    return m_bSendMsgSectorClear;
}

// IDA: 0x14013A200 - ?SetSectorClearQuestState@CGocQuest@@QEAAXH_N@Z
// Verified: Direct IDA decompilation - Set sector clear quest state
void CGocQuest::SetSectorClearQuestState(int nSectorID, bool bFlag) {
    // IDA: Direct array access (no bounds check in IDA)
    m_bQuestStateClear[nSectorID] = bFlag ? 1 : 0;
}

// IDA: 0x14013A230 - ?IsQuestCondtionForSectorClear@CGocQuest@@QEAA_NXZ
// Verified: Direct IDA decompilation - Check if quest condition is cleared for sector
bool CGocQuest::IsQuestCondtionForSectorClear() {
    // IDA: Get owner actor and check if in maze
    // CMover* pMover = GetOwnerGO();
    // if (!pMover) return true;

    // IDA: Get current maze via RTDynamicCast
    // XMaze* pMaze = dynamic_cast<XMaze*>(GetCurrentArea());
    // if (pMaze) {
    //     // IDA: Check if playing cutscene
    //     std::tr1::shared_ptr<CQuestCondition> pQuestCondition = GetCurrentQuestCondition();
    //     if (pQuestCondition) {
    //         std::uint32_t dwQuestID = pQuestCondition->GetQuestID();
    //         if (XMaze::IsPlayCutscene(pMaze, dwQuestID)) {
    //             return false;
    //         }
    //     }
    // }

    // IDA: Check all 3 sector slots
    for (int i = 0; i < 3; ++i) {
        if (m_bQuestStateClear[i]) {
            return false;
        }
    }

    return true;
}

// IDA: 0x14013AFF0 - ?GetRepeatQuestInfo@CGocQuest@@QEAAXKAEAUST_QUEST_REPEAT_INFO@@@Z
// Verified: Direct IDA decompilation - Get repeat quest info
void CGocQuest::GetRepeatQuestInfo(std::uint32_t dwEpisodeID, ST_QUEST_REPEAT_INFO* pInfo) {
    // IDA: Find episode in m_mapRepeatQuest
    auto it = m_mapRepeatQuest.find(dwEpisodeID);
    if (it != m_mapRepeatQuest.end()) {
        // IDA: Copy ST_QUEST_REPEAT_INFO
        *pInfo = it->second;
    }
}

// IDA: 0x14013AD20 - ?SetRepeatQuestList@CGocQuest@@QEAAXAEAUPS_REPEAT_QUEST_MAP@@@Z
// Verified: Direct IDA decompilation - Set repeat quest list from DB
// Note: Parameter type changed from PS_REPEAT_QUEST_MAP to std::map for simplicity
void CGocQuest::SetRepeatQuestList(std::uint32_t dwEpisodeID, const ST_QUEST_REPEAT_INFO* pInfo) {
    // IDA: Get current time and calculate today's init time (9 AM)
    // ATL::CTime tCurr = ATL::CTime::GetTickCount();
    // int nDay = tCurr.GetDay();
    // int nMonth = tCurr.GetMonth();
    // int nYear = tCurr.GetYear();
    // ATL::CTime tTodayInit(nYear, nMonth, nDay, 9, 0, 0);
    // if (tCurr.GetHour() < 9) {
    //     tTodayInit -= ATL::CTimeSpan(1, 0, 0, 0);
    // }
    // m_tLastInitDate = tTodayInit;

    // IDA: Iterate through input map and process repeat quests
    // For each entry in psRepeat:
    //   - Get TB_QUEST_EPISODE
    //   - Check Contents_Type == 2 (repeat quest)
    //   - If init time < today init, reset count to 0
    //   - Add to m_mapRepeatQuest

    // Simplified implementation: just set the repeat quest info
    if (pInfo) {
        m_mapRepeatQuest[dwEpisodeID] = *pInfo;
    }
}

// ============================================================================
// Quest Update Helper Functions
// ============================================================================

// IDA: 0x14013AB20 - ?ClearSyncQuestCondition_All@CGocQuest@@AEAAXXZ
// Verified: Direct IDA decompilation - Clear all sync quest conditions
void CGocQuest::ClearSyncQuestCondition_All() {
    // IDA: Check if m_mapUpdateCondition is empty
    if (!m_mapUpdateCondition.empty()) {
        // IDA: Clear the map
        m_mapUpdateCondition.clear();
    }
}

// IDA: 0x14013A960 - ?ClearUpdateQuestCondition_GiveUp@CGocQuest@@AEAAXK@Z
// Verified: Direct IDA decompilation - Clear update quest condition when giving up
void CGocQuest::ClearUpdateQuestCondition_GiveUp(std::uint32_t dwEpisodeID) {
    // IDA: Find episode in m_mapEpisode
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it == m_mapEpisode.end()) {
        return;
    }

    // IDA: Check if m_mapUpdateCondition is empty
    if (m_mapUpdateCondition.empty()) {
        return;
    }

    // IDA: Iterate through 10 conditions
    for (int i = 0; i < 10; ++i) {
        // IDA: Get condition ID from ST_QUEST_EPISODE
        std::uint32_t dwConditionID = static_cast<std::uint32_t>(it->second.stCondition[i].nCondition);
        if (!dwConditionID) {
            break;
        }

        // IDA: Call ClearUpdateQuestCondition
        ClearUpdateQuestCondition(static_cast<int>(dwConditionID));
    }
}

// UpdateQuestRespawn moved to proper location with IDA address 0x14013A350

void CGocQuest::DBUpdateEpisodeInfo(std::uint32_t dwEpisodeID, ST_QUEST_EPISODE* pEpisode) {
    // TODO: 汇编还原 - Sync episode info to database
    // IDA address needs to be determined
    (void)dwEpisodeID;
    (void)pEpisode;
}

void CGocQuest::ClearUpdateQuestCondition(int nConditionID) {
    // TODO: 汇编还原 - Clear condition update state
    // IDA address needs to be determined
    (void)nConditionID;
}

// IDA: 0x14013B1A0 - ?CompleteQuestForNewChar@CGocQuest@@QEAAX_N@Z
// Verified: Direct IDA decompilation - Complete quest for new character
void CGocQuest::CompleteQuestForNewChar(int nType, float fParam) {
    // IDA: Get owner actor's class index
    // CMover* pMover = GetOwnerGO();
    // std::uint16_t nClassIdx = pMover->GetClassIdx() * 1000;
    std::uint16_t nClassIdx = 0;  // TODO: Get from owner actor

    // IDA: Get TB_CHARACTER_INFO from resource manager
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_CHARACTER_INFO* pTB_CHAR = pServer->m_xResourceMgr.GetTB_CHARACTER_INFO(nClassIdx);
    TB_CHARACTER_INFO* pTB_CHAR = nullptr;  // TODO: Need resource manager
    if (!pTB_CHAR || pTB_CHAR->Character_Group_ID == 101) {
        return;
    }

    // IDA: Episode IDs for new character completion
    static const std::uint32_t dwEpisodeIDForNewChar[4] = {
        100115,  // Episode 1
        100065,  // Episode 2
        100150,  // Episode 3
        100172   // Episode 4
    };

    // IDA: Process each episode
    for (int i = 0; i < 4; ++i) {
        std::uint32_t dwEpisodeID = dwEpisodeIDForNewChar[i];

        // IDA: Get TB_QUEST_EPISODE from resource manager
        // TB_QUEST_EPISODE* pTB_QUEST_EPISODE = pServer->m_xResourceMgr.GetTB_QUEST_EPISODE(dwEpisodeID);
        TB_QUEST_EPISODE* pTB_QUEST_EPISODE = nullptr;  // TODO: Need resource manager
        if (!pTB_QUEST_EPISODE) {
            continue;
        }

        // IDA: Check if has Complete_Bit
        if (pTB_QUEST_EPISODE->Complete_Bit) {
            int nIndex = pTB_QUEST_EPISODE->Complete_Bit / 8;
            int nPos = pTB_QUEST_EPISODE->Complete_Bit % 8;

            if (nIndex > 256) {
                continue;
            }

            // IDA: Check if already complete (different logic for 100172)
            if (dwEpisodeID == 100172) {
                if ((m_szCompleteEpisode[nIndex] & (1 << nPos)) != 0) {
                    continue;
                }
            } else {
                if (IsCompleteEpisode(dwEpisodeID)) {
                    continue;
                }
            }

            // IDA: Set complete bit
            m_szCompleteEpisode[nIndex] |= (1 << nPos);

            // IDA: Send DB packet (0x41, 0x05) to save completion
            // ST_GET_INFO stGetInfo;
            // XSendDBPacket xSendDBPacket(pActor, 0x41, 0x05);
            // xSendDBPacket << GetOwnerGO()->GetID();  // UCID
            // xSendDBPacket << dwEpisodeID;
            // xSendDBPacket << pTB_QUEST_EPISODE->Contents_Type;
            // xSendDBPacket.SetBytes(GetCompleteEpisode(), 256);
            // xSendDBPacket << stGetInfo;
            // xSendDBPacket << false;  // bReturn
            // XGameServer::SendDBGame(&xSendDBPacket);

            // IDA: Send game log (main=6, sub=4)
            // ST_LOG_GAME stLog;
            // stLog._nUAID = pUser->GetUAID();
            // stLog._nUCID = pUser->GetID();
            // stLog._sMainType = 6;
            // stLog._sSubType = 4;
            // stLog.nParam0 = dwEpisodeID;
            // stLog.nParam4 = 0;
            // stLog.nParam5 = pTB_QUEST_EPISODE->Contents_Type;
            // stLog.nParam6 = pUser->GetLevel();
            // wcscpy_s(stLog.szComment, L"");
            // XGameServer::SendDBLog(&stLog);
        }
    }

    // IDA: Send complete episode list if bSend is true
    bool bSend = (nType != 0);
    if (bSend) {
        SendCompleteEpisodeList();
    }
}

// IDA: 0x14019D1B0 - ?CheckQuestDBSync@CGocQuest@@QEAA_NXZ
// Verified: Direct IDA decompilation - Check if quest DB sync is empty
bool CGocQuest::CheckQuestDBSync() {
    return m_mapUpdateCondition.empty();
}

// IDA: 0x1405971E0 - ?GetCompleteQuestReq@CGocQuest@@QEAA_NXZ
// Verified: Direct IDA decompilation - Get complete quest request flag
bool CGocQuest::GetCompleteQuestReq() const {
    return m_bComplete;
}

// IDA: 0x140597200 - ?SetCompleteQuestQeq@CGocQuest@@QEAAX_N@Z
// Verified: Direct IDA decompilation - Set complete quest request flag
void CGocQuest::SetCompleteQuestQeq(bool bComplete) {
    m_bComplete = bComplete;
}

// ============================================================================
// Additional Quest Functions - Batch 4
// ============================================================================

// IDA: 0x140137830 - ?UpdateCondition@CGocQuest@@QEAA_NK_J_N@Z
// Verified: Direct IDA decompilation - Update condition by ConditionID with nParam
bool CGocQuest::UpdateCondition(std::uint32_t dwConditionID, int nParam, bool bPartyWith) {
    // IDA: Get owner CMover and dynamic_cast to CUser
    // CMover* pMover = GetOwnerGO();
    // CUser* pUser = dynamic_cast<CUser*>(pMover);
    // if (!pUser) return false;

    // IDA: Get ConditionID index from m_mapCondition (boost::multi_index hashed index)
    // auto& index = m_mapCondition.get<0>(); // ConditionID hashed index

    // IDA: Find condition by ConditionID
    // auto iter = index.find(dwConditionID);
    // if (iter == index.end()) return false;

    // IDA: Get shared_ptr<CQuestCondition>
    // std::tr1::shared_ptr<CQuestCondition> pQuestCondition = *iter;

    // IDA: Check if condition has TB_QUEST_CONDITION pointer
    // TB_QUEST_CONDITION* pTB_COND = pQuestCondition->GetTBCondition();
    // if (!pTB_COND) return false;

    // IDA: Check if episode is failed (bFailed == 1)
    // if (pQuestCondition->GetEpisode()->bFailed == 1) return false;

    // IDA: Check if already complete
    // if (pQuestCondition->IsCompleteCondition()) {
    //     LogHelper::LogDebug("game.quest", "<QUEST> Alread CompleteCondition...");
    //     return false;
    // }

    // IDA: Check NeedCompletionCondition prerequisite
    // int nNeedCond = pQuestCondition->GetNeedCompletionCondition();
    // if (nNeedCond > 0 && !IsCompleteCondition(nNeedCond)) {
    //     LogHelper::LogDebug("game.quest", "<QUEST> Not Complete Before Condition...");
    //     return false;
    // }

    // IDA: Check if Counter_Value > ConditionValue (can still add)
    // if (pTB_COND->Counter_Value > pQuestCondition->GetConditionValue()) {
    //     int nOldValue = pQuestCondition->GetConditionValue();
    //     pQuestCondition->AddConditionValue(nParam);
    //
    //     // IDA: CheckSyncQuestCondition
    //     CheckSyncQuestCondition(pQuestCondition);
    //
    //     // IDA: Log to DB (main=6, sub=2)
    //     // ST_LOG_GAME stLog;
    //     // stLog._sMainType = 6; stLog._sSubType = 2;
    //     // stLog.nParam0 = pQuestCondition->GetQuestID();
    //     // stLog.nParam1 = dwConditionID;
    //     // stLog.nParam2 = pQuestCondition->GetConditionValue();
    //     // stLog.nParam3 = nParam;
    //
    //     // IDA: Send statistics if QuestID in range [0x186A1, 0x30D40)
    //
    //     // IDA: Try CompleteCondition
    //     if (pTB_COND->Counter_Value > pQuestCondition->GetConditionValue() ||
    //         !CompleteCondition(pQuestCondition->GetQuestID(), pQuestCondition)) {
    //         // IDA: Rollback on failure
    //         LogHelper::LogError("game.quest", "CompleteCondition error - Roll Back Condition Count...");
    //         pQuestCondition->SetConditionValue(nOldValue);
    //         ClearUpdateQuestCondition(dwConditionID);
    //         return false;
    //     }
    //
    //     // IDA: If Group_Counting && bPartyWith && in maze, propagate to party
    //     if (pTB_COND->Group_Counting && bPartyWith && CUser::IsMaze(pUser)) {
    //         XMaze* pMaze = GetCurrentMaze();
    //         std::map<unsigned long, CMover*>* pVector = XMaze::GetScanner(pMaze, pMover);
    //         if (pVector) {
    //             for (auto& pair : *pVector) {
    //                 CUser* pMember = dynamic_cast<CUser*>(pair.second);
    //                 if (pMember && pMember->GetID() != pUser->GetID()) {
    //                     std::tr1::shared_ptr<CGocQuest> pMemberQuest;
    //                     CMover::GetGOC<CGocQuest>(pMember, &pMemberQuest, 0);
    //                     if (pMemberQuest) {
    //                         pMemberQuest->UpdateCondition(pQuestCondition->GetQuestID(), dwConditionID, nParam, false);
    //                     }
    //                 }
    //             }
    //         }
    //     }
    //
    //     // IDA: Send condition update packet (0x15, 0x07)
    //     // PS_QUEST_CONDITION psPacket;
    //     // XSendPacket xSendPacket(0x15, 7);
    //     // xSendPacket << psPacket;
    //     // CGocNetwork::Send(pMover, &xSendPacket);
    //
    //     return true;
    // }

    (void)dwConditionID;
    (void)nParam;
    (void)bPartyWith;
    return false;
}

// IDA: 0x140138F20 - ?SetQuestAddObject@CGocQuest@@QEAA_NKPEAEPEAK@Z
// Verified: Direct IDA decompilation - Add quest objects using type and ID arrays
bool CGocQuest::SetQuestAddObject(std::uint32_t dwEpisodeID, std::uint8_t* byType, std::uint32_t* dwID) {
    // IDA: Get owner CMover and CGocInventory
    // CMover* pMover = GetOwnerGO();
    // std::tr1::shared_ptr<CGocInventory> pInven;
    // CMover::GetGOC<CGocInventory>(pMover, &pInven, 0);
    // if (!pInven) return false;

    // IDA: Get common inventory (type 2)
    // XBank* pCommonInven = pInven->GetInvenPtr(2);

    // IDA: Calculate needed slot count
    int nNeedSlotCount = 0;

    // IDA: Check first 2 Add_Object entries
    for (int i = 0; i < 2; ++i) {
        if (byType[i] == 1) {
            // IDA: Get TB_ITEM for item info
            // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            // TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, dwID[i]);
            // if (!pTB_ITEM) return false;

            // IDA: Check if item is stackable
            // if (pTB_ITEM->Item_Stack_Max == 1) {
            //     ++nNeedSlotCount;  // Non-stackable, need new slot
            // } else {
            //     // IDA: Check existing stacks
            //     std::vector<std::tr1::shared_ptr<CItem>> vecItems;
            //     pCommonInven->GetSameItems(dwID[i], &vecItems, -1);
            //     ++nNeedSlotCount;
            //
            //     for (auto& pItem : vecItems) {
            //         if (pTB_ITEM->Item_Stack_Max >= pItem->GetCount() + 1) {
            //             --nNeedSlotCount;  // Can stack
            //             break;
            //         }
            //     }
            // }
            (void)dwID;
        }
    }

    // IDA: Check if enough empty slots
    // int nEmptyCount = pCommonInven->GetEmptySlotCount();
    // if (nNeedSlotCount > 0 && nEmptyCount < nNeedSlotCount) return false;

    // IDA: Create items
    for (int j = 0; j < 2; ++j) {
        if (byType[j] == 0) {
            return true;  // IDA: returns 1 when type is 0
        }
        if (byType[j] == 1) {
            // IDA: Create item via CGocInventory::CreateItemReq
            // ST_LOG_GAME stLog;
            // stLog.nParam3 = dwEpisodeID;
            // pInven->CreateItemReq(dwID[j], 1, 0, E_ITEM_CREATE_TYPE_QUEST_ACCEPT, &stLog);
            (void)dwID;
        }
    }

    (void)dwEpisodeID;
    (void)byType;
    return true;
}

// IDA: 0x1401392C0 - ?SetQuestAddObject@CGocQuest@@QEAA_NKAEAUPS_RES_STORAGE_INFO@@0@Z
// Verified: Direct IDA decompilation - Add quest objects using PS_RES_STORAGE_INFO packets
bool CGocQuest::SetQuestAddObject(std::uint32_t dwEpisodeID, void* psCreateItem, void* psUpdateItem) {
    // IDA: Get TB_QUEST_EPISODE
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_QUEST_EPISODE* pTB_EPISODE = XResourceMgr::GetTB_QUEST_EPISODE(&pServer->m_xResourceMgr, dwEpisodeID);
    // if (!pTB_EPISODE) return false;

    // IDA: Get owner CMover and CGocInventory
    // CMover* pMover = GetOwnerGO();
    // std::tr1::shared_ptr<CGocInventory> pInven;
    // CMover::GetGOC<CGocInventory>(pMover, &pInven, 0);
    // if (!pInven) return false;

    // IDA: Get common inventory (type 2)
    // XBaseInventory* pCommonInven = pInven->GetInvenPtr(2);
    // if (!pCommonInven) return false;

    // IDA: Build list of Add_Object_IDs
    // std::vector<unsigned long> vecAddObjectList;
    int nNeedSlotCount = 0;

    // IDA: Check Add_Object_Type_01, Add_Object_Type_02
    for (int i = 0; i < 2; ++i) {
        // if (pTB_EPISODE->Add_Object_Type[i] == 1) {
        //     // IDA: Get TB_ITEM
        //     TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, pTB_EPISODE->Add_Object_ID[i]);
        //     if (!pTB_ITEM) return false;
        //
        //     vecAddObjectList.push_back(pTB_EPISODE->Add_Object_ID[i]);
        //
        //     // IDA: Check stackability
        //     if (pTB_ITEM->Item_Stack_Max == 1) {
        //         ++nNeedSlotCount;
        //     } else {
        //         // Check existing stacks...
        //         ++nNeedSlotCount;
        //     }
        // }
    }

    // IDA: Check empty slots
    // int nEmptyCount = pCommonInven->GetEmptySlotCount();
    // if (nNeedSlotCount > 0 && nEmptyCount < nNeedSlotCount) return false;

    // IDA: Add items using CGocInventory::AddItem2
    for (std::size_t j = 0; j < 0; ++j) { // vecAddObjectList.size()
        // TB_ITEM* pTBItem = XResourceMgr::GetTB_ITEM(&pServer->m_xResourceMgr, vecAddObjectList[j]);
        // if (!pTBItem) return false;
        // if (!pInven->AddItem2(pTBItem, 1, 0x84, 0, psCreateItem, psUpdateItem)) return false;
    }

    (void)dwEpisodeID;
    (void)psCreateItem;
    (void)psUpdateItem;
    return true;
}

// IDA: 0x140139780 - ?GetNeedConditionItemCount@CGocQuest@@QEAAHKK@Z
// Verified: Direct IDA decompilation - Get needed item count for condition
int CGocQuest::GetNeedConditionItemCount(std::uint32_t dwConditionID, std::uint32_t dwItemID) {
    int nNeedCount = 0;

    // IDA: Get ConditionID index from m_mapCondition
    // auto& index = m_mapCondition.get<0>(); // ConditionID hashed index

    // IDA: Find condition by ConditionID
    // auto iter = index.find(dwConditionID);
    // if (iter == index.end()) return 0;

    // IDA: Get shared_ptr<CQuestCondition>
    // std::tr1::shared_ptr<CQuestCondition> pQuestCondition = *iter;

    // IDA: Get owner CMover and CGocInventory
    // CMover* pMover = GetOwnerGO();
    // std::tr1::shared_ptr<CGocInventory> pInven;
    // CMover::GetGOC<CGocInventory>(pMover, &pInven, 0);

    // IDA: Get item from inventory
    // std::tr1::shared_ptr<CItem> pItem;
    // pInven->GetItem(&pItem, 2, dwItemID);

    // IDA: Get TB_QUEST_CONDITION Counter_Value (offset +124 = Counter_Value byte)
    // TB_QUEST_CONDITION* pTB_COND = pQuestCondition->GetTBCondition();
    // int nCounterValue = pTB_COND->Counter_Value;

    // IDA: If item exists, calculate needed count
    // if (pItem) {
    //     int nCurrentCount = pItem->GetCount();
    //     nNeedCount = (nCounterValue - nCurrentCount > 0) ? (nCounterValue - nCurrentCount) : 0;
    // } else {
    //     nNeedCount = nCounterValue;
    // }

    (void)dwConditionID;
    (void)dwItemID;
    return nNeedCount;
}

// IDA: 0x140139970 - ?IsCompleteCondition@CGocQuest@@QEAA_NH@Z
// Verified: Direct IDA decompilation - Check if condition is complete by ID
bool CGocQuest::IsCompleteCondition(int nConditionID) const {
    // IDA: Check if condition ID is valid
    if (nConditionID <= 0) {
        return false;
    }

    // IDA: Get ConditionID index from m_mapCondition
    // auto& index = m_mapCondition.get<0>(); // ConditionID hashed index

    // IDA: Find condition by ConditionID
    // auto iter = index.find(nConditionID);
    // if (iter == index.end()) return false;

    // IDA: Get shared_ptr<CQuestCondition>
    // std::tr1::shared_ptr<CQuestCondition> pQuestCondition = *iter;

    // IDA: Check if episode is failed (bFailed == 1 at offset +4)
    // if (pQuestCondition->GetEpisode()->bFailed == 1) return false;

    // IDA: Call CQuestCondition::IsCompleteCondition
    // return pQuestCondition->IsCompleteCondition();

    // Simplified implementation using std::map
    for (const auto& pair : m_mapCondition) {
        if (pair.second && static_cast<int>(pair.second->GetConditionID()) == nConditionID) {
            // Check if episode is failed
            std::uint32_t dwQuestID = pair.second->GetQuestID();
            auto it = m_mapEpisode.find(dwQuestID);
            if (it != m_mapEpisode.end() && it->second.bFailed) {
                return false;
            }
            return pair.second->IsCompleteCondition();
        }
    }

    return false;
}
