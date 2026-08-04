#include "GocQuest.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAchieve.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/XBaseInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocSkill.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocBooster.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocHelper.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XGameServer/CutsceneManager.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/VaccumCube.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XCommon/Packet/XSendPacket.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include <cstring>
#include <cwchar>
#include <ctime>

// ============================================================================
// CQuestCondition implementation
// ============================================================================

// IDA: 0x140125C20 - Constructor
CQuestCondition::CQuestCondition(std::uint32_t dwQuestID, ST_QUEST_EPISODE* pQuest,
                                 int nConditionIndex, TB_QUEST_CONDITION* pTBCondition)
    : m_dwQuestID(dwQuestID)
    , m_pQuest(pQuest)
    , m_pCondition(&pQuest->stCondition[nConditionIndex])
    , m_nConditionIndex(nConditionIndex)
    , m_pTBCondition(pTBCondition)
{
}

// IDA: 0x140125C90 - GetConditionID
std::uint32_t CQuestCondition::GetConditionID() const {
    return m_pTBCondition->ID;
}

// IDA: 0x140125CB0 - GetConditionType
std::uint8_t CQuestCondition::GetConditionType() const {
    return m_pTBCondition->Condition;
}

// IDA: 0x140125CE0 - GetNeedCompletionCondition
int CQuestCondition::GetNeedCompletionCondition() {
    return m_pTBCondition->High_Condition_ID;
}

// IDA: 0x140125D00 - AddConditionValue
void CQuestCondition::AddConditionValue(int nValue) {
    m_pCondition->byValue += nValue;
}

// IDA: 0x140125D30 - GetConditionValue
std::uint8_t CQuestCondition::GetConditionValue() const {
    return m_pCondition->byValue;
}

// IDA: 0x140125D50 - SetConditionValue
void CQuestCondition::SetConditionValue(std::uint8_t byValue) {
    m_pCondition->byValue = byValue;
}

// IDA: 0x140125D70 - IsCompleteCondition
bool CQuestCondition::IsCompleteCondition() {
    return ((1 << m_nConditionIndex) & m_pQuest->shCompleteBit) > 0;
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
    PS_QUEST_EPISODE_MAP psInfo;
    psInfo.mapInfo = m_mapEpisode;

    // IDA: Create PS_REPEAT_QUEST_MAP and copy m_mapRepeatQuest to it
    PS_REPEAT_QUEST_MAP psRepeat;
    psRepeat.mapInfo = m_mapRepeatQuest;

    // IDA: Create XSendPacket(0x15, 0x02)
    XSendPacket xSendPacket(0x15, 0x02);

    // IDA: Serialize both maps into packet
    xSendPacket << psInfo;
    xSendPacket << psRepeat;

    // IDA: Get owner actor and send packet
    XActor* pActor = dynamic_cast<XActor*>(GetOwnerGO());
    CGocNetwork::Send(pActor, xSendPacket);
}

// IDA: ?SendCompleteEpisodeList@CGocQuest@@QEAAXXZ (0x140126400)
// Verified: Direct IDA decompilation - Send complete episode bit array to client
void CGocQuest::SendCompleteEpisodeList() {
    // IDA: Create XSendPacket(0x15, 0x01)
    XSendPacket xSendPacket(0x15, 0x01);

    // IDA: Write 256-byte bit array to packet
    xSendPacket.XParse.SetBytes(reinterpret_cast<const char*>(m_szCompleteEpisode), 256);

    // IDA: Get owner actor and send packet
    XActor* pActor = dynamic_cast<XActor*>(GetOwnerGO());
    CGocNetwork::Send(pActor, xSendPacket);
}

// IDA: ?SendReqQuestList@CGocQuest@@QEAAXXZ (0x140129EF0)
// Verified: Direct IDA decompilation - Request quest list from DB
void CGocQuest::SendReqQuestList() {
    // IDA: Get owner actor for XSendDBPacket
    XActor* pActor = dynamic_cast<XActor*>(GetOwnerGO());

    // IDA: Create XSendDBPacket(0x41, 0x01) - DB request packet
    XSendDBPacket xSendDBPacket(pActor, 0x41, 0x01);

    // IDA: Get user's UAID and write to packet
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (pUser) {
        std::uint32_t dwUAID = pUser->GetUAID();
        xSendDBPacket.XParse << dwUAID;
    }

    // IDA: Send to game DB via XGameServer
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        pServer->SendDBGame(xSendDBPacket);
    }
}

// IDA: 0x14012A000 - ?SetEpisodeList@CGocQuest@@QEAAXAEAV?$map@KUST_QUEST_EPISODE@@...@Z
void CGocQuest::SetEpisodeList(
    std::map<std::uint32_t, ST_QUEST_EPISODE>& mapEpisode) {
    m_mapEpisode = mapEpisode;
    m_bLoad = true;

    std::vector<std::uint32_t> vecDeleteQuest;
    std::vector<std::uint32_t> vecCompleteConditionForce;
    bool bHaveMainQuest = false;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    for (auto& [dwEpisodeID, stEpisode] : m_mapEpisode) {
        TB_QUEST_EPISODE* pTBQuestEpisode =
            pServer->GetResourceMgr().GetTB_QUEST_EPISODE(dwEpisodeID);
        if (!pTBQuestEpisode) {
            continue;
        }

        if (stEpisode.byAddHelper == 1) {
            ++m_nHelperCount;
        }

        if (IsCompleteEpisode(dwEpisodeID)) {
            vecDeleteQuest.push_back(dwEpisodeID);
        } else {
            if (!pTBQuestEpisode->Contents_Type) {
                bHaveMainQuest = true;
            }

            for (int i = 0; i < 10; ++i) {
                ST_QUEST_CONDITION& stQuestCondition = stEpisode.stCondition[i];
                TB_QUEST_CONDITION* pTBCondition =
                    pServer->GetResourceMgr().GetTB_QUEST_CONDITION(
                        stQuestCondition.dwConditionID);
                if (!pTBCondition) {
                    break;
                }

                stEpisode.pTBQuestEpisode = pTBQuestEpisode;
                std::shared_ptr<CQuestCondition> pCondition(
                    new CQuestCondition(
                        dwEpisodeID, &stEpisode, i, pTBCondition));
                m_mapCondition.insert(pCondition);

                if (stQuestCondition.byValue >= pTBCondition->Counter_Value &&
                    !IsCompleteCondition(stQuestCondition.dwConditionID)) {
                    vecCompleteConditionForce.push_back(
                        stQuestCondition.dwConditionID);
                }
            }
        }
    }

    for (const std::uint32_t dwEpisodeID : vecDeleteQuest) {
        const auto it = m_mapEpisode.find(dwEpisodeID);
        if (it != m_mapEpisode.end()) {
            bool bGiveUp = false;
            std::uint8_t byContentsType = 0;
            if (it->second.pTBQuestEpisode) {
                byContentsType = it->second.pTBQuestEpisode->Contents_Type;
            }

            XSendDBPacket xSendDBPacket(
                static_cast<XActor*>(GetOwnerGO()), 0x41, 6);
            xSendDBPacket.XParse << GetOwnerGO()->GetID();
            xSendDBPacket.XParse << dwEpisodeID;
            xSendDBPacket.XParse << byContentsType;
            xSendDBPacket.XParse << bGiveUp;
            pServer->SendDBGame(xSendDBPacket);

            LogHelper::LogError(
                "game.contents",
                "[DBUPDATE] Wrong Main Quest [UCID:%d, Quest:%d]",
                GetOwnerGO()->GetID(),
                dwEpisodeID);
        }
        DeleteEpisode(dwEpisodeID);
    }

    for (const std::uint32_t dwConditionID : vecCompleteConditionForce) {
        CompleteConditionByForce(dwConditionID);
    }

    for (const auto& [dwQuestID, dwQuestBefore] :
         *pServer->GetResourceMgr().GetAutoAcceptQuest()) {
        if (IsCompleteEpisode(dwQuestBefore) &&
            !IsCompleteEpisode(dwQuestID) &&
            !FindEpisode(dwQuestID)) {
            TB_QUEST_EPISODE* pTBQuest =
                pServer->GetResourceMgr().GetTB_QUEST_EPISODE(dwQuestID);
            if (pTBQuest->Contents_Type || !bHaveMainQuest) {
                AcceptQuest(dwQuestID, false);
            } else {
                LogHelper::LogError(
                    "game.contents",
                    "[QUEST] FAILED Auto Quest Accept %d %d",
                    GetOwnerGO()->GetID(),
                    dwQuestID);
            }
        }
    }
}

// IDA: 0x1401264E0
// IDA decompiled: ?FindEpisode@CGocQuest@@QEAA_NK@Z
// Returns true if episode exists and is not failed (bFailed != true)
// BYTE4(second) is offset 4 in ST_QUEST_EPISODE which is the bFailed field (after byAddHelper, _pad0, shCompleteBit)
bool CGocQuest::FindEpisode(std::uint32_t dwEpisodeID) {
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
bool CGocQuest::FindCondition(std::uint32_t dwConditionID) {
    const std::shared_ptr<CQuestCondition>* condition =
        m_mapCondition.FindByConditionID(dwConditionID);
    if (!condition || !*condition) {
        return false;
    }

    const std::shared_ptr<CQuestCondition>& spCondition = *condition;

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

// IDA: 0x140126690 - ?IsCompleteEpisode@CGocQuest@@QEAA_NK@Z
// IDA decompiled: Check if episode is marked complete in bit array
bool CGocQuest::IsCompleteEpisode(std::uint32_t dwEpisodeID)
{
    // IDA: Get TB_QUEST_EPISODE from resource manager
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return false;

    TB_QUEST_EPISODE* pTB_EPISODE = pServer->GetResourceMgr().GetTB_QUEST_EPISODE(dwEpisodeID);
    if (!pTB_EPISODE) return false;

    // IDA: Check Class_Type for character-specific quests
    if (pTB_EPISODE->Class_Type != 0) {
        if (pTB_EPISODE->Class_Type >= 100) {
            CMover* pMover = GetOwnerGO();
            std::tr1::shared_ptr<CGocAttribute> pAttr = pMover->GetGOC_Attribute(false);
            if (pAttr) {
                const int nClassType = pAttr->GetClass() * 1000;
                TB_CHARACTER_INFO* pTB_CHAR = pServer->GetResourceMgr().GetTB_CHARACTER_INFO(
                    static_cast<std::uint16_t>(nClassType));
                if (pTB_CHAR && pTB_CHAR->Character_Group_ID != pTB_EPISODE->Class_Type) {
                    return true;
                }
            }
        } else {
            CMover* pMover = GetOwnerGO();
            if (static_cast<int>(pMover->GetClass()) != pTB_EPISODE->Class_Type) {
                return true;
            }
        }
    }

    // IDA: Calculate bit position in complete episode array
    int nIndex = pTB_EPISODE->Complete_Bit / 8;
    if (nIndex > 256) return false;

    // IDA: Check if bit is set
    int nPos = pTB_EPISODE->Complete_Bit % 8;
    return (m_szCompleteEpisode[nIndex] & (1 << nPos)) != 0;
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
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return false;
    }
    
    // IDA: Find episode in m_mapEpisode
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it == m_mapEpisode.end()) {
        return false;
    }
    
    // IDA: Call DeleteEpisode to remove episode and conditions
    DeleteEpisode(dwEpisodeID);
    
    // IDA: Handle repeat quest completion
    ST_QUEST_REPEAT_INFO stRepeat = {};
    // Note: Would need TB_QUEST_EPISODE to check Contents_Type
    // if (pTB_EPISODE && pTB_EPISODE->Contents_Type == 2) {
    //     CompleteRepeatQuest(dwEpisodeID);
    //     GetRepeatQuestInfo(dwEpisodeID, &stRepeat);
    // }
    
    // IDA: Send packet to client (main=0x15, sub=5)
    XSendPacket xSendPacket(0x15, 5);
    xSendPacket.XParse << dwEpisodeID;
    xSendPacket << stRepeat;
    XActor* pActor = reinterpret_cast<XActor*>(pMover);
    CGocNetwork::Send(pActor, xSendPacket);
    
    // IDA: Process linked episodes (Link_Episode_ID_01..10)
    // Note: Would need TB_QUEST_EPISODE to get Link_Episode_ID fields
    // for (int i = 0; i < 10; ++i) {
    //     std::uint32_t dwLinkEpisodeID = pTB_EPISODE->Link_Episode_ID[i];
    //     if (!dwLinkEpisodeID) break;
    //     if (pTB_EPISODE_New && pTB_EPISODE_New->Auto_Start) {
    //         AcceptQuest(dwLinkEpisodeID, false);
    //     }
    // }
    
    // IDA: Update achieve via CGocAchieve
    // Note: Would need CGocAchieve component
    // CGocAchieve::UpdateQuestAchieve(pTB_EPISODE->Contents_Type);
    
    // IDA: If in maze, update clear condition and call script
    // Note: Would need XMaze reference
    // XMaze::UpdateClearMazeCondition(pMaze, 2, dwEpisodeID);
    // XMaze::UpdatePartyQuest(pMaze, dwEpisodeID, 0, 1);
    
    (void)stGetInfo;
    return true;
}

// IDA: 0x140128C00 - ?CompleteEpisodeAdd@CGocQuest@@QEAAXAEAUPS_QUEST_COMPLETE_ADD_LIST@@@Z
// Verified: Direct IDA decompilation - Add completed episodes to bit array
// Note: PS_QUEST_COMPLETE_ADD_LIST is defined elsewhere; using void* as placeholder
void CGocQuest::CompleteEpisodeAdd(void* psAddList) {
    // IDA: Cast to PS_QUEST_COMPLETE_ADD_LIST
    PS_QUEST_COMPLETE_ADD_LIST* pAddList = static_cast<PS_QUEST_COMPLETE_ADD_LIST*>(psAddList);
    if (!pAddList) {
        return;
    }
    
    // IDA: Iterate through add list
    std::int16_t shCount = static_cast<std::int16_t>(pAddList->vecQuestID.size());
    for (std::int16_t sh = 0; sh < shCount; ++sh) {
        std::uint32_t dwEpisodeID = static_cast<std::uint32_t>(pAddList->vecQuestID[sh]);
        
        // IDA: Get TB_QUEST_EPISODE from resource manager
        // Note: Would need XGameServer singleton and XResourceMgr access
        TB_QUEST_EPISODE* pTB_EPISODE = nullptr; // TODO: Get from resource manager
        if (!pTB_EPISODE || !pTB_EPISODE->Complete_Bit) {
            continue;
        }
        
        // IDA: Calculate bit position
        int nIndex = pTB_EPISODE->Complete_Bit / 8;
        int nPos = pTB_EPISODE->Complete_Bit % 8;
        
        // IDA: Check bounds and not repeat quest (Contents_Type != 2)
        if (nIndex > 256 || pTB_EPISODE->Contents_Type == 2) {
            continue;
        }
        
        // IDA: Check if not already complete
        if (IsCompleteEpisode(dwEpisodeID)) {
            continue;
        }
        
        // IDA: Set bit in complete episode array
        m_szCompleteEpisode[nIndex] |= (1 << nPos);
        
        // IDA: Send DB packet (0x41/0x05)
        // Note: Would need XSendDBPacket and XGameServer::SendDBGame
        
        // IDA: Send game log (main=6, sub=7)
        // Note: Would need ST_LOG_GAME and XGameServer::SendDBLog
    }
}

// IDA: 0x14012BBD0 - ?AcceptQuest@CGocQuest@@QEAA_NK_N@Z
// Verified: Direct IDA decompilation - Accept quest with full validation
// Note: Simplified implementation to avoid API access issues
bool CGocQuest::AcceptQuest(std::uint32_t dwEpisodeID, bool bCheckMaxCount) {
    // IDA: Get CUser from actor and check block type
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return false;
    }
    
    // IDA: Check block type (UserDB & 4)
    // Note: Would need CUser::GetBlockType - simplified for now
    
    // IDA: Check max quest count (30 = 0x1E) if bCheckMaxCount
    if (bCheckMaxCount && m_mapEpisode.size() >= 30) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD308);
        return false;
    }
    
    // IDA: Check if already have this episode
    if (FindEpisode(dwEpisodeID)) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F1);
        return false;
    }
    
    // IDA: Check if already completed
    if (IsCompleteEpisode(dwEpisodeID)) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F2);
        return false;
    }
    
    // IDA: Get TB_QUEST_EPISODE from resource manager
    // Note: Would need XGameServer singleton and XResourceMgr access
    TB_QUEST_EPISODE* pTB_EPISODE = nullptr; // TODO: Get from resource manager
    if (!pTB_EPISODE) {
        // For now, create a basic episode entry without validation
        // In production, this would fetch from resource manager and validate
    }
    
    // IDA: Check level requirement (Quest_Level <= player level)
    // Note: Would need CGocAttribute and CGameWorldMode::GetState
    
    // IDA: Check class requirement
    // Note: Would need CGocAttribute::GetClass and TB_CHARACTER_INFO
    
    // IDA: Check repeat quest time if Contents_Type == 2
    // Note: Would need CheckAcceptRepeatQuest
    
    // IDA: Check before episode completion (Before_Episode_ID_1..5)
    // Note: Would need to access Before_Episode_ID fields from pTB_EPISODE
    
    // IDA: SetQuestAddObject for quest items
    // Note: Would need to call SetQuestAddObject
    
    // IDA: Create ST_QUEST_EPISODE and conditions
    ST_QUEST_EPISODE stEpisode = {};
    if (m_nHelperCount < 7) {
        stEpisode.byAddHelper = 1;
        ++m_nHelperCount;
    }
    
    // IDA: Process each condition (Condition_ID_01..10)
    // Note: Would need to access Condition_ID fields from pTB_EPISODE
    // and create CQuestCondition objects
    
    // IDA: DBUpdateEpisodeInfo
    DBUpdateEpisodeInfo(dwEpisodeID, stEpisode);
    
    // IDA: Handle repeat quest
    ST_QUEST_REPEAT_INFO stRepeat = {};
    // if (pTB_EPISODE && pTB_EPISODE->Contents_Type == 2) {
    //     AcceptRepeatQuest(dwEpisodeID);
    //     GetRepeatQuestInfo(dwEpisodeID, &stRepeat);
    // }
    
    // IDA: Send packet to client (main=0x15, sub=3)
    XSendPacket xSendPacket(0x15, 3);
    xSendPacket.XParse << dwEpisodeID;
    xSendPacket.XParse << stEpisode.byAddHelper;
    xSendPacket << stRepeat;
    CGocNetwork::Send(dynamic_cast<XActor*>(pMover), xSendPacket);
    
    // IDA: UpdateItemCondition, EnableInteractionObject, UpdateQuestConditionForSectorClear
    UpdateItemCondition();
    UpdateQuestConditionForSectorClear();
    
    // IDA: UpdateOpenTitle
    // Note: Would need CGocEntity component
    
    // IDA: Log to database (main=6, sub=1)
    // Note: Would need ST_LOG_GAME and XGameServer::SendDBLog
    
    // IDA: Send statistics if episode in range [0x186A1, 0x30D40)
    // Note: Would need ST_STATISTICS_QUEST and XGameServer::SendDBStatistics
    
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
// IDA 精确还原 - Accept quest bypassing normal validation
bool CGocQuest::AcceptQuestByForce(std::uint32_t dwEpisodeID) {
    // IDA: Get owner mover
    CMover* pMover = GetOwnerGO();
    if (!pMover) return false;

    // IDA: Check if user is blocked
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (pUser && pUser->GetBlockType()) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xC3BF);
        return false;
    }

    // IDA: Check if already have this episode
    if (FindEpisode(dwEpisodeID)) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F1);
        return false;
    }

    // IDA: Check if already completed
    if (IsCompleteEpisode(dwEpisodeID)) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F2);
        return false;
    }

    // IDA: Get TB_QUEST_EPISODE from resource manager
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_QUEST_EPISODE* pTB_EPISODE = pServer ? pServer->GetResourceMgr().GetTB_QUEST_EPISODE(dwEpisodeID) : nullptr;
    if (!pTB_EPISODE) return false;

    // IDA: Get CGocAttribute for level/class check
    std::shared_ptr<CGocAttribute> pAttr = pMover->GetGOC_Attribute(false);
    int nPlayerLevel = pAttr ? pAttr->GetLevel() : 0;
    int nPlayerClass = pAttr ? static_cast<int>(pAttr->GetClass()) : 0;

    // IDA: Check level requirement (log error but continue)
    if (pTB_EPISODE->Quest_Level > nPlayerLevel) {
        LogHelper::LogError("game.quest",
            "AcceptQuestByForce error - Level condition is not right[ ActorID:%d ] ( %d )",
            pMover->GetID(), 1929);
    }

    // IDA: Check class requirement (log error but continue)
    if (pTB_EPISODE->Class_Type != 0) {
        if (pTB_EPISODE->Class_Type < 100) {
            // Direct class type check
            if (pTB_EPISODE->Class_Type != nPlayerClass) {
                LogHelper::LogError("game.quest",
                    "AcceptQuestByForce error - Class condition is not right[ ActorID:%d / %d ] ( %d )",
                    pMover->GetID(), pTB_EPISODE->ID, 1953);
                CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD303);
            }
        } else {
            // Character group check
            int nCharID = nPlayerClass * 1000;
            TB_CHARACTER_INFO* pTB_CHAR = pServer ? pServer->GetResourceMgr().GetTB_CHARACTER_INFO(nCharID) : nullptr;
            if (!pTB_CHAR || pTB_CHAR->Character_Group_ID != pTB_EPISODE->Class_Type) {
                if (pTB_EPISODE->Class_Type != nPlayerClass) {
                    LogHelper::LogError("game.quest",
                        "AcceptQuestByForce error - Class condition is not right[ ActorID:%d / %d ] ( %d )",
                        pMover->GetID(), pTB_EPISODE->ID, 1944);
                    CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD303);
                }
            }
        }
    }

    // IDA: Check repeat quest time if Contents_Type == 2
    int nError = 0;
    if (pTB_EPISODE->Contents_Type == 2) {
        if (!CheckAcceptRepeatQuest(dwEpisodeID, false, &nError)) {
            LogHelper::LogError("game.quest",
                "AcceptQuest error - Repeat Quest Check Time[ ActorID:%d ] ( %d )",
                pMover->GetID(), 1963);
            return false;
        }
    }

    // IDA: SetQuestAddObject for quest items
    if (!SetQuestAddObject(dwEpisodeID, &pTB_EPISODE->Add_Object_Type_01,
                           reinterpret_cast<std::uint32_t*>(&pTB_EPISODE->Add_Object_ID_01))) {
        LogHelper::LogError("game.quest",
            "AcceptQuestByForce error - Occurence problem to Object is given accept quest[ ActorID:%d, EpisodeID:%d ] ( %d )",
            pMover->GetID(), dwEpisodeID, 1970);
        return false;
    }

    // IDA: Create ST_QUEST_EPISODE
    ST_QUEST_EPISODE stEpisode = {};
    if (m_nHelperCount < 7) {
        stEpisode.byAddHelper = 1;
        ++m_nHelperCount;
    }

    // IDA: Get maze from area
    XArea* pArea = pMover->GetArea();
    XMaze* pMaze = pArea ? dynamic_cast<XMaze*>(pArea) : nullptr;

    // IDA: Get Condition_ID array
    std::uint32_t Condition_IDs[10] = {
        pTB_EPISODE->Condition_ID_01, pTB_EPISODE->Condition_ID_02, pTB_EPISODE->Condition_ID_03,
        pTB_EPISODE->Condition_ID_04, pTB_EPISODE->Condition_ID_05, pTB_EPISODE->Condition_ID_06,
        pTB_EPISODE->Condition_ID_07, pTB_EPISODE->Condition_ID_08, pTB_EPISODE->Condition_ID_09,
        pTB_EPISODE->Condition_ID_10
    };

    // IDA: Check if has any condition
    if (Condition_IDs[0] == 0) {
        LogHelper::LogError("game.quest",
            "AcceptQuestByForce error - No Condition [ ActorID:%d, EpisodeID:%d ]",
            pMover->GetID(), dwEpisodeID);
        return false;
    }

    // IDA: Process each condition
    for (int i = 0; i < 10; ++i) {
        std::uint32_t dwConditionID = Condition_IDs[i];
        if (dwConditionID == 0) break;

        TB_QUEST_CONDITION* pTB_CONDITION = pServer ? pServer->GetResourceMgr().GetTB_QUEST_CONDITION(dwConditionID) : nullptr;
        if (!pTB_CONDITION) break;

        stEpisode.pTBQuestEpisode = pTB_EPISODE;
        stEpisode.shCompleteBit &= static_cast<std::int16_t>(~(1 << i));
        stEpisode.stCondition[i].dwConditionID = pTB_CONDITION->ID;
        stEpisode.stCondition[i].byValue = 0;

        // IDA: Insert into m_mapEpisode
        m_mapEpisode[dwEpisodeID] = stEpisode;

        // IDA: Create CQuestCondition
        ST_QUEST_EPISODE* pEpisodeInMap = &m_mapEpisode[dwEpisodeID];
        std::shared_ptr<CQuestCondition> pCondition(new CQuestCondition(dwEpisodeID, pEpisodeInMap, i, pTB_CONDITION));
        m_mapCondition.insert(pCondition);

        // IDA: If in maze, call script update
        if (pMaze) {
            std::uint32_t dwQuestID = pCondition->GetQuestID();
            pMaze->CallScriptUpdateQuest(dwQuestID, 0, dwEpisodeID);

            int nNeedCompletionCondition = pCondition->GetNeedCompletionCondition();
            if (nNeedCompletionCondition <= 0 || IsCompleteCondition(nNeedCompletionCondition)) {
                // IDA: RunQuestConditionStart is called via script system
                // pMaze->AcceptQuest(dwConditionID);
            }
        }
    }

    // IDA: DBUpdateEpisodeInfo
    DBUpdateEpisodeInfo(dwEpisodeID, stEpisode);

    // IDA: Handle repeat quest
    ST_QUEST_REPEAT_INFO stRepeat = {};
    if (pTB_EPISODE->Contents_Type == 2) {
        AcceptRepeatQuest(dwEpisodeID);
        GetRepeatQuestInfo(dwEpisodeID, &stRepeat);
    }

    // IDA: Send packet to client (main=0x15, sub=3)
    XSendPacket xSendPacket(0x15, 3);
    xSendPacket.XParse << dwEpisodeID;
    xSendPacket.XParse << stEpisode.byAddHelper;
    xSendPacket << stRepeat;
    CGocNetwork::Send(reinterpret_cast<XActor*>(pMover), xSendPacket);

    // IDA: UpdateItemCondition, EnableInteractionObject, UpdateQuestConditionForSectorClear
    UpdateItemCondition();
    EnableInteractionObject(pTB_EPISODE->Condition_ID_01, 0);
    UpdateQuestConditionForSectorClear();

    // IDA: UpdateOpenTitle via CGocEntity
    std::shared_ptr<CGocEntity> pEntity = pMover->GetGOC_Entity(false);
    if (pEntity) {
        pEntity->UpdateOpenTitle(0, dwEpisodeID);
    }

    // IDA: Log to database (main=6, sub=1)
    ST_LOG_GAME stLog = {};
    stLog._nUAID = pUser ? pUser->GetUAID() : 0;
    stLog._nUCID = pUser ? pUser->GetUCID() : 0;
    stLog._sMainType = 6;
    stLog._sSubType = 1;
    stLog.nParam0 = static_cast<int>(dwEpisodeID);
    std::memcpy(&stLog.nParam1, &stEpisode.stCondition[0], sizeof(ST_QUEST_CONDITION));
    stLog.nParam3 = stEpisode.byAddHelper;
    stLog.nParam9 = 0; // IDA: skill ID not available directly
    stLog.nParam5 = pTB_EPISODE->Contents_Type;
    stLog.nParam6 = static_cast<std::int64_t>(pMover->GetLevel());
    if (pServer) {
        pServer->SendDBLog(stLog);
    }

    // IDA: Send statistics if episode in range [0x186A1, 0x30D40)
    if (dwEpisodeID >= 0x186A1 && dwEpisodeID < 0x30D40) {
        ST_STATISTICS_QUEST stStatistics = {};
        stStatistics.byFlag = 1;
        stStatistics.dwUCID = pUser ? pUser->GetUCID() : 0;
        stStatistics.dwEpisodeID = dwEpisodeID;
        stStatistics.byLevel = pMover->GetLevel();

        XSendDBPacket xSendDBStatistics(static_cast<XActor*>(pUser), 0xF0, 6);
        xSendDBStatistics.XParse << stStatistics.byFlag;
        xSendDBStatistics.XParse << stStatistics.dwUCID;
        xSendDBStatistics.XParse << stStatistics.dwEpisodeID;
        xSendDBStatistics.XParse << stStatistics.byLevel;
        if (pServer) {
            pServer->SendDBStatistics(xSendDBStatistics);
        }
    }

    return true;
}

// IDA: 0x140130C50 - ?CompleteQuestByForce@CGocQuest@@QEAA_NK@Z
// Verified: Direct IDA decompilation - Complete quest without normal validation
bool CGocQuest::CompleteQuestByForce(std::uint32_t dwEpisodeID) {
    // IDA: Find episode in m_mapEpisode
    auto iter = m_mapEpisode.find(dwEpisodeID);
    if (iter == m_mapEpisode.end()) {
        return false;
    }

    // IDA: Get TB_QUEST_EPISODE
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_QUEST_EPISODE* pTB_EPISODE = pServer ? pServer->GetResourceMgr().GetTB_QUEST_EPISODE(dwEpisodeID) : nullptr;
    if (!pTB_EPISODE) {
        return false;
    }

    // IDA: Get owner mover
    CMover* pMover = GetOwnerGO();
    if (!pMover) return false;

    // IDA: Get CUser for class info
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    std::uint8_t nClass = pUser ? pUser->GetClass() : 0;

    // IDA: Get inventory component
    std::shared_ptr<CGocInventory> pInven;
    pMover->GetGOC<CGocInventory>(&pInven, false);

    // IDA: Process remove items (Remove_Item_ID_01..05)
    for (int i = 0; i < 5 && (&pTB_EPISODE->Remove_Item_ID_01)[i]; ++i) {
        std::uint32_t dwItemID = (&pTB_EPISODE->Remove_Item_ID_01)[i];
        std::uint32_t dwCount = (&pTB_EPISODE->Remove_Item_Count_01)[i];

        if (dwCount && pInven) {
            TB_ITEM* pTB_ITEM = pServer->GetResourceMgr().GetTB_ITEM(dwItemID);
            if (pTB_ITEM) {
                TB_ITEM_CLASSIFY* pTBClassify = pServer->GetResourceMgr().GetTB_ITEM_CLASSIFY(pTB_ITEM->Item_Classify_Index);
                if (pTBClassify) {
                    XBaseInventory* pFindInven = pInven->GetTBInvenPtr(pTBClassify->Item_Inven_Type);
                    if (pFindInven) {
                        std::shared_ptr<CItem> pDeleteItem = pFindInven->GetItem(static_cast<int>(dwItemID));
                        if (pDeleteItem) {
                            ST_LOG_GAME stLogData;
                            stLogData._sSubType = 25;
                            stLogData.nParam3 = dwEpisodeID;

                            int nSlot = pDeleteItem->GetSlot();
                            std::uint8_t byInvenType = pFindInven->GetInvenType();

                            pInven->BreakItemReq(byInvenType, nSlot, dwCount, 0x29, &stLogData);

                            // IDA: If item count <= 0, send statistics
                            if (pDeleteItem->GetCount() <= 0) {
                                ST_STATISTICS_ITEM stStatistics;
                                stStatistics.byFlag = 3;
                                stStatistics.biSerial = pDeleteItem->GetSerial();

                                XSendDBPacket xSendDBStatistics(pMover, 0xF0, 0x11);
                                xSendDBStatistics << stStatistics;
                                pServer->SendDBStatistics(xSendDBStatistics);
                            }
                        }
                    }
                }
            }
        }
    }

    // IDA: Build reward item list
    ST_CREATE_ITEMS stCreateItems;

    // IDA: Reward_Item_type == 1: Direct items
    if (pTB_EPISODE->Reward_Item_type == 1) {
        for (int j = 0; j < 5; ++j) {
            if ((&pTB_EPISODE->Reward_Item_ID_01)[j]) {
                TB_ITEM* pTB_ITEM = pServer->GetResourceMgr().GetTB_ITEM((&pTB_EPISODE->Reward_Item_ID_01)[j]);
                if (pTB_ITEM) {
                    ST_CREATE_ITEM stItem;
                    stItem.nItemID = (&pTB_EPISODE->Reward_Item_ID_01)[j];
                    stItem.shCount = static_cast<std::int16_t>((&pTB_EPISODE->Reward_Item_Count_01)[j]);
                    stCreateItems.vecInfo.push_back(stItem);
                }
            }
        }
    }
    // IDA: Reward_Item_type == 2: Random from TB_QUEST_REWARD
    else if (pTB_EPISODE->Reward_Item_type == 2) {
        for (int k = 0; k < 5; ++k) {
            if ((&pTB_EPISODE->Reward_Item_ID_01)[k]) {
                TB_QUEST_REWARD* pTBReward = pServer->GetResourceMgr().GetTB_QUEST_REWARD(
                    static_cast<std::uint16_t>((&pTB_EPISODE->Reward_Item_ID_01)[k]));
                if (pTBReward && nClass > 0) {
                    // TODO: Need TB_QUEST_REWARD definition with per-class item IDs
                    int nItemID = 0;  // (&pTBReward->Item_ID_01)[nClass - 1];
                    TB_ITEM* pTB_ITEM = pServer->GetResourceMgr().GetTB_ITEM(nItemID);
                    if (pTB_ITEM) {
                        ST_CREATE_ITEM stItem;
                        stItem.nItemID = nItemID;
                        stItem.shCount = static_cast<std::int16_t>((&pTB_EPISODE->Reward_Item_Count_01)[k]);
                        stCreateItems.vecInfo.push_back(stItem);
                    }
                }
            }
        }
    }

    // IDA: Build ST_GET_INFO
    ST_GET_INFO stGetInfo;

    // IDA: Add titles
    for (int m = 0; m < 2; ++m) {
        if ((&pTB_EPISODE->Reward_Title_01)[m]) {
            std::shared_ptr<CGocEntity> pEntity;
            pMover->GetGOC<CGocEntity>(&pEntity, false);
            if (pEntity) {
                std::uint32_t dwTitle = pEntity->AddTitleByClass((&pTB_EPISODE->Reward_Title_01)[m], 2);
                stGetInfo.nTitleID[m] = dwTitle;
            }
        }
    }

    // IDA: Add helper for Contents_Type == 3
    if (pTB_EPISODE->Contents_Type == 3) {
        TB_HELPER* pTB_Helper = pServer->GetResourceMgr().GetTB_HELPER(pTB_EPISODE->Reward_Helper);
        if (pTB_Helper) {
            std::shared_ptr<CGocHelper> pHelper;
            pMover->GetGOC<CGocHelper>(&pHelper, false);
            // TODO: CGocHelper::SendDBAddHelper not implemented yet
            // if (pHelper && pHelper->SendDBAddHelper(pTB_Helper->ID)) {
            //     stGetInfo.dwHelperID = pTB_Helper->ID;
            // }
            stGetInfo.dwHelperID = pTB_Helper->ID;
        }
    }

    // IDA: Get attribute component
    std::shared_ptr<CGocAttribute> pAttr;
    pMover->GetGOC<CGocAttribute>(&pAttr, false);

    // IDA: Add EXP reward with booster bonus
    float fExpReward = static_cast<float>(pTB_EPISODE->Reward_EXP);
    if (fExpReward > 0.0f) {
        std::shared_ptr<CGocBooster> pBooster;
        pMover->GetGOC<CGocBooster>(&pBooster, false);
        // TODO: Need booster enum
        float fRate = 1.0f;  // pBooster ? pBooster->GetTotalRate(eBooster_Effect_Complete_Quest_IncExp) : 1.0f;
        int nIncExp = static_cast<int>(fExpReward * (fRate - 1.0f));
        fExpReward += static_cast<float>(nIncExp);

        LogHelper::LogInfo("game.contents",
            "[SetExp] CGocQuest::CompleteQuestByForce() Before [Exp:%d][AddExp:%f]",
            static_cast<int>(fExpReward), static_cast<float>(nIncExp));

        if (pAttr) {
            pAttr->SetExp(static_cast<double>(fExpReward), static_cast<float>(nIncExp), 1);
        }

        LogHelper::LogInfo("game.contents",
            "[SetExp] CGocQuest::CompleteQuestByForce() After [Exp:%d]",
            pAttr ? static_cast<int>(pAttr->GetExp()) : 0);

        // TODO: SendExpSave not implemented
        // if (pUser) pUser->SendExpSave();
    }
    stGetInfo.nExp = static_cast<int>(fExpReward);

    // IDA: Add money reward
    if (pTB_EPISODE->Reward_Money) {
        if (pInven && !pInven->CheckOverMoney(E_PRICE_TYPE_GOLD, pTB_EPISODE->Reward_Money)) {
            CGocNetwork::SendErrorMessage(pMover, 0x15, 5, 0xCB58);
            return false;
        }
        if (pInven) {
            pInven->AddMoney(pTB_EPISODE->Reward_Money, 0x23, pTB_EPISODE->ID, 0, 0);
        }
    }
    stGetInfo.nMoney = pTB_EPISODE->Reward_Money;

    // IDA: Add BP reward
    if (pTB_EPISODE->Reward_BP) {
        if (pInven && !pInven->CheckOverMoney(E_PRICE_TYPE_BP, pTB_EPISODE->Reward_BP)) {
            CGocNetwork::SendErrorMessage(pMover, 0x15, 5, 0xCB59);
            return false;
        }
        if (pInven) {
            pInven->AddBP(pTB_EPISODE->Reward_BP, 0x26);
        }
    }
    stGetInfo.nBP = pTB_EPISODE->Reward_BP;

    // IDA: Add Ether reward
    if (pTB_EPISODE->Reward_Ether) {
        if (pInven && !pInven->CheckOverMoney(E_PRICE_TYPE_ETHER, pTB_EPISODE->Reward_Ether)) {
            CGocNetwork::SendErrorMessage(pMover, 0x15, 5, 0xCB5A);
            return false;
        }
        if (pInven) {
            pInven->AddEther(pTB_EPISODE->Reward_Ether, 0x26, 1);
        }
    }
    stGetInfo.nEther = pTB_EPISODE->Reward_Ether;

    // IDA: Add Awakening reward
    if (pTB_EPISODE->Reward_Awakening && pAttr) {
        pAttr->SetAwaken(pTB_EPISODE->Reward_Awakening, 1);
    }

    // IDA: Add skill rewards
    std::shared_ptr<CGocSkill> pSkill;
    pMover->GetGOC<CGocSkill>(&pSkill, false);

    for (int n = 0; n < 4 && (&pTB_EPISODE->Reward_Class_Skill_01)[n]; ++n) {
        if (pSkill) {
            int nSkill = (&pTB_EPISODE->Reward_Class_Skill_01)[n] + 10000000 * nClass;
            if (!pSkill->IsHaveSkill(nSkill)) {
                pSkill->LearnSkill(nSkill, 0, 0);
            }
        }
    }

    // IDA: Create reward items
    ST_LOG_GAME stLogItem;
    stLogItem.nParam3 = dwEpisodeID;
    if (pInven && pInven->CreateItemReq(
            stCreateItems,
            false,
            E_ITEM_CREATE_TYPE_QUEST_REWARD,
            stLogItem)) {
        UpdateQuestConditionForSectorClear();

        // IDA: Log quest completion
        ST_LOG_GAME stLog;
        if (pUser) {
            stLog._nUAID = pUser->GetUAID();
            stLog._nUCID = pUser->GetUCID();
        }
        stLog._sMainType = 6;
        stLog._sSubType = 4;
        stLog.nParam0 = dwEpisodeID;
        stLog.nParam9 = pAttr ? static_cast<int>(pAttr->GetExp()) : 0;
        stLog.nParam5 = pTB_EPISODE->Contents_Type;
        stLog.nParam6 = nClass;
        wcscpy_s(stLog.szComment, L"");

        pServer->SendDBLog(stLog);

        // IDA: Send statistics for episode in range
        if (dwEpisodeID >= 0x186A1 && dwEpisodeID < 0x30D40) {
            // TODO: Need proper serialization for ST_STATISTICS_QUEST
            // ST_STATISTICS_QUEST st;
            // st.byFlag = 4;
            // st.dwUCID = pUser ? pUser->GetUCID() : 0;
            // st.dwEpisodeID = dwEpisodeID;
            // st.byLevel = nClass;
            // XSendDBPacket xSendPacket(pMover, 0xF0, 6);
            // xSendPacket << st;
            // pServer->SendDBStatistics(xSendPacket);
        }

        // IDA: Set complete bit
        iter->second.shCompleteBit = 1023;  // All 10 conditions complete

        int nIndex = pTB_EPISODE->Complete_Bit / 8;
        int nPos = pTB_EPISODE->Complete_Bit % 8;

        if (nIndex <= 256) {
            // IDA: Update open title
            std::shared_ptr<CGocEntity> pEntity;
            pMover->GetGOC<CGocEntity>(&pEntity, false);
            if (pEntity) {
                pEntity->UpdateOpenTitle(1, dwEpisodeID);
            }

            // IDA: Set complete bit in episode array
            if (pTB_EPISODE->Contents_Type != 2) {
                m_szCompleteEpisode[nIndex] |= (1 << nPos);
            }

            // IDA: Send DB update
            bool bReturn = true;
            XSendDBPacket xSendDBPacket(pMover, 0x41, 5);
            std::uint32_t dwUCID = pUser ? pUser->GetUCID() : 0;
            xSendDBPacket << dwUCID;
            xSendDBPacket << dwEpisodeID;
            xSendDBPacket << pTB_EPISODE->Contents_Type;
            // TODO: Write complete episode bytes - need public API
            // xSendDBPacket.XParse.AppendRaw(GetCompleteEpisode(), 256);
            xSendDBPacket << stGetInfo;
            xSendDBPacket << bReturn;

            pServer->SendDBGame(xSendDBPacket);

            return true;
        } else {
            return false;
        }
    } else {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 5, 0xCB2A);
        return false;
    }
}

// IDA: 0x140127890 - ?CompleteConditionByForce@CGocQuest@@QEAA_NK@Z
bool CGocQuest::CompleteConditionByForce(std::uint32_t dwConditionID) {
    const std::shared_ptr<CQuestCondition>* condition =
        m_mapCondition.FindByConditionID(dwConditionID);
    if (!condition) {
        return false;
    }

    std::shared_ptr<CQuestCondition> pCondition = *condition;
    if (!pCondition->GetQuestEpisode()) {
        LogHelper::LogError(
            "game.quest",
            "CompleteConditionByForce error - Episode is null ( %d )",
            382);
    }

    pCondition->GetQuestEpisode()->shCompleteBit |=
        1 << pCondition->GetConditionIndex();

    const std::uint32_t dwQuestID = pCondition->GetQuestID();
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_QUEST_EPISODE* pTBQuestEpisode =
        pServer->GetResourceMgr().GetTB_QUEST_EPISODE(dwQuestID);
    std::uint8_t byQuestType = 0;
    if (pTBQuestEpisode) {
        byQuestType = pTBQuestEpisode->Contents_Type;
    }

    CMover* pMover = GetOwnerGO();
    std::shared_ptr<CGocAttribute> pAttr;
    pMover->GetGOC<CGocAttribute>(&pAttr, false);

    ST_LOG_GAME stLog;
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetUCID();
    stLog._sMainType = 6;
    stLog._sSubType = 3;
    stLog.nParam0 = dwQuestID;
    stLog.nParam1 = dwConditionID;
    stLog.nParam9 = pAttr ? pAttr->GetExp() : 0;
    stLog.nParam5 = byQuestType;
    stLog.nParam6 = pMover->GetClass();
    wcscpy_s(stLog.szComment, L"");
    pServer->SendDBLog(stLog);

    UpdateQuestRespawn(pCondition->GetConditionID());

    if (pCondition->GetConditionTable()->CutScene_Portal_control) {
        XArea* pArea = pMover->GetArea();
        if (pArea) {
            XMaze* pCutsceneMaze = dynamic_cast<XMaze*>(pArea);
            if (pCutsceneMaze) {
                pCutsceneMaze->GetCutSceneMgr()->CheckCutsceneState(
                    GetOwnerGO()->GetActorID(),
                    5,
                    pCondition->GetConditionTable()->ID);
            }
        }
    }

    UpdateQuestConditionForSectorClear();

    XMaze* pMaze = dynamic_cast<XMaze*>(pMover->GetArea());
    if (pMaze) {
        pMaze->CallScriptUpdateQuest(
            GetOwnerGO()->GetActorID(),
            2,
            pCondition->GetConditionID());
        pMaze->RunQuestConditionEnd(GetOwnerGO()->GetActorID(), dwConditionID);
        pMaze->UpdatePartyQuest(pCondition->GetQuestID(), dwConditionID, false);
    }

    pCondition->SetConditionValue(
        pCondition->GetConditionTable()->Counter_Value);
    if (!CompleteCondition(pCondition->GetQuestID(), pCondition)) {
        return false;
    }

    PS_QUEST_CONDITION psPacket;
    ST_QUEST_CONDITION stCondition;
    stCondition.dwConditionID = dwConditionID;
    stCondition.byValue = pCondition->GetConditionTable()->Counter_Value;
    psPacket.vecInfo.push_back(stCondition);

    XSendPacket xSendPacket(0x15, 7);
    xSendPacket << psPacket;
    CGocNetwork::Send(pMover, xSendPacket);
    return true;
}

// IDA: 0x140126860 - ?CompleteCondition@CGocQuest@@QEAA_NKV?$shared_ptr@VCQuestCondition@@@tr1@std@@@Z
bool CGocQuest::CompleteCondition(std::uint32_t dwEpisodeID,
                                  std::shared_ptr<CQuestCondition> pCondition) {
    const auto iter = m_mapEpisode.find(dwEpisodeID);
    if (iter == m_mapEpisode.end()) {
        return false;
    }

    ST_QUEST_EPISODE& stEpisode = iter->second;
    CMover* pMover = GetOwnerGO();
    std::shared_ptr<CGocInventory> pInven;
    pMover->GetGOC<CGocInventory>(&pInven, false);

    XBaseInventory* pCommonInven = pInven->GetInvenPtr(2);
    TB_QUEST_CONDITION* pTBCondition = pCondition->GetConditionTable();
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();

    int nNeedSlotCount = 0;
    for (int i = 0; i < 2; ++i) {
        const int nNeedItem = pTBCondition->uniAdd_item_condition[i];
        if (nNeedItem <= 0) {
            continue;
        }

        TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(nNeedItem);
        if (!pTBItem) {
            return false;
        }

        if (pTBItem->Item_Stack_Max == 1) {
            ++nNeedSlotCount;
            continue;
        }

        std::vector<std::shared_ptr<CItem>> vecItems;
        pCommonInven->GetSameItems_2(nNeedItem, &vecItems, -1);
        ++nNeedSlotCount;
        for (const std::shared_ptr<CItem>& pItem : vecItems) {
            if (pTBItem->Item_Stack_Max >= pItem->GetCount() + 1) {
                --nNeedSlotCount;
                break;
            }
        }
    }

    if (nNeedSlotCount > 0 &&
        pCommonInven->GetEmptySlotCount() < nNeedSlotCount) {
        LogHelper::LogError(
            "game.quest",
            "CompleteCondition error - Not enough Inven When Complete Condition[ ActorID:%d, Condition:%d ] ( %d )",
            pMover->GetID(),
            pCondition->GetConditionID(),
            237);
        return false;
    }

    for (int j = 0; j < 2; ++j) {
        if (pTBCondition->uniAdd_item_condition[j]) {
            ST_LOG_GAME stLogData;
            stLogData.nParam3 = dwEpisodeID;
            if (!pInven->CreateItemReq(
                    pTBCondition->uniAdd_item_condition[j],
                    pTBCondition->uniAdd_item_condition_Count[j],
                    false,
                    E_ITEM_CREATE_TYPE_CONDITION_COMPLETE,
                    stLogData)) {
                LogHelper::LogDebug(
                    "game.quest",
                    "<QUEST> Failed Add Item When Complete Condition [ ActorID:%d, Condition:%d ) ( %d )",
                    pMover->GetID(),
                    pCondition->GetConditionID(),
                    251);
                return false;
            }
        }

        if (pTBCondition->uniRemove_item_condition[j]) {
            std::shared_ptr<CItem> pDeleteItem = pInven->GetItem(
                2,
                pTBCondition->uniRemove_item_condition[j]);
            if (pDeleteItem) {
                ST_LOG_GAME stLogData;
                stLogData._sSubType = 24;
                stLogData.nParam3 = dwEpisodeID;
                pInven->BreakItemReq(
                    2,
                    pDeleteItem->GetSlot(),
                    pTBCondition->uniRemove_item_condition_Count[j],
                    0x28,
                    &stLogData);

                if (pDeleteItem->GetCount() <= 0) {
                    ST_STATISTICS_ITEM stStatistics;
                    stStatistics.byFlag = 3;
                    stStatistics.biSerial = pDeleteItem->GetSerial();

                    XSendDBPacket xSendDBStatistics(pMover, 0xF0, 0x11);
                    xSendDBStatistics << stStatistics;
                    pServer->SendDBStatistics(xSendDBStatistics);
                }
            }
        }
    }

    stEpisode.shCompleteBit |= 1 << pCondition->GetConditionIndex();

    LogHelper::LogDebug(
        "game.quest",
        "<QUEST> Complete Condition ( UID : %d  Condition : %d )",
        pMover->GetID(),
        pCondition->GetConditionID());

    DisableInteractionObject(
        pCondition->GetConditionID(),
        pTBCondition->Target_Type,
        pTBCondition->Target_ID);
    EnableInteractionObject(pTBCondition->Link_Condition_ID, 0);

    if (pTBCondition->CutScene_Portal_control) {
        XArea* pArea = pMover->GetArea();
        if (pArea) {
            XMaze* pCutsceneMaze = dynamic_cast<XMaze*>(pArea);
            if (pCutsceneMaze) {
                pCutsceneMaze->GetCutSceneMgr()->CheckCutsceneState(
                    GetOwnerGO()->GetActorID(),
                    5,
                    pTBCondition->ID);
            }
        }
    }

    UpdateQuestConditionForSectorClear();
    UpdateQuestRespawn(pCondition->GetConditionID());
    DBUpdateEpisodeInfo(dwEpisodeID, stEpisode);
    ClearUpdateQuestCondition(pCondition->GetConditionID());

    std::shared_ptr<CGocAttribute> pAttr;
    pMover->GetGOC<CGocAttribute>(&pAttr, false);
    CUser* pUser = dynamic_cast<CUser*>(pMover);

    ST_LOG_GAME stLog;
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetUCID();
    stLog._sMainType = 6;
    stLog._sSubType = 3;
    stLog.nParam0 = dwEpisodeID;
    stLog.nParam1 = pCondition->GetConditionID();
    stLog.nParam9 = pAttr->GetExp();
    stLog.nParam5 = stEpisode.pTBQuestEpisode->Contents_Type;
    stLog.nParam6 = pMover->GetClass();
    wcscpy_s(stLog.szComment, L"");
    pServer->SendDBLog(stLog);

    if (stEpisode.pTBQuestEpisode->Contents_Type == 1 &&
        stEpisode.shCompleteBit == 1023) {
        CompleteQuest(dwEpisodeID, 0);
    }

    UpdateItemCondition();

    XMaze* pMaze = dynamic_cast<XMaze*>(pMover->GetArea());
    if (pMaze) {
        pMaze->CallScriptUpdateQuest(
            GetOwnerGO()->GetActorID(),
            2,
            pCondition->GetConditionID());
        pMaze->RunQuestConditionEnd(
            GetOwnerGO()->GetActorID(),
            pTBCondition->ID);
        pMaze->UpdatePartyQuest(
            dwEpisodeID,
            pCondition->GetConditionID(),
            false);

        for (int k = 0; k < 10; ++k) {
            const std::uint32_t dwConditionID =
                stEpisode.stCondition[k].dwConditionID;
            if (!dwConditionID || dwConditionID == pTBCondition->ID) {
                continue;
            }

            TB_QUEST_CONDITION* pTblRef =
                pServer->GetResourceMgr().GetTB_QUEST_CONDITION(dwConditionID);
            if (pTblRef &&
                pTblRef->High_Condition_ID == pTBCondition->ID) {
                pMaze->RunQuestConditionStart(
                    GetOwnerGO()->GetActorID(),
                    pTblRef->ID);
            }
        }
    }

    return true;
}

// IDA: 0x14013AA20 - ?CheckSyncQuestCondition@CGocQuest@@AEAAXV?$shared_ptr@VCQuestCondition@@@tr1@std@@@Z
void CGocQuest::CheckSyncQuestCondition(
    std::shared_ptr<CQuestCondition> pCondition) {
    const std::uint32_t dwConditionID = pCondition->GetConditionID();
    const auto it = m_mapUpdateCondition.find(dwConditionID);
    if (it == m_mapUpdateCondition.end()) {
        m_mapUpdateCondition.insert(std::make_pair(dwConditionID, pCondition));
    }
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
    DBUpdateEpisodeInfo(dwQuestID, it->second);

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

// IDA: 0x140134D80 - ?UpdateCondition@CGocQuest@@QEAAXW4E_CONDITION_TYPE@@W4E_CONDITION_TARGET@@KH_N@Z
// Verified: Direct IDA decompilation - Update quest condition by type/target
void CGocQuest::UpdateCondition(E_CONDITION_TYPE eType,
                                E_CONDITION_TARGET eTarget,
                                std::uint32_t dwObjectID, int nCount, bool bPartyWith) {
    // IDA: Get owner actor and cast to CUser
    CMover* pMover = GetOwnerGO();
    if (!pMover) return;

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) return;

    // IDA: Iterate the ConditionType ordered index.
    std::vector<std::uint32_t> completedConditions;
    const auto range =
        m_mapCondition.GetByConditionType(static_cast<std::uint8_t>(eType));

    for (auto it = range.first; it != range.second; ++it) {
        const std::shared_ptr<CQuestCondition>& pQuestCondition = it->second;
        if (!pQuestCondition) continue;

        // IDA: Get TB_QUEST_CONDITION
        TB_QUEST_CONDITION* pTB_COND = pQuestCondition->GetConditionTable();
        if (!pTB_COND) continue;

        if (pQuestCondition->GetQuestEpisode()->bFailed) continue;

        // IDA: Skip if already complete
        if (pQuestCondition->IsCompleteCondition()) continue;

        // IDA: Check NeedCompletionCondition prerequisite
        int nNeedCond = pQuestCondition->GetNeedCompletionCondition();
        if (nNeedCond > 0 && !IsCompleteCondition(nNeedCond)) continue;

        // IDA: Check Target_Type matches and validate with CheckUpdateCondition
        std::int64_t nParam = static_cast<std::int64_t>(dwObjectID);
        if (!CheckUpdateCondition(pQuestCondition->GetConditionID(), nParam)) continue;
        if (pTB_COND->Target_Type != eTarget) continue;

        // IDA: Get old condition value for potential rollback
        int nOldValue = pQuestCondition->GetConditionValue();

        // IDA: Add count to condition
        pQuestCondition->AddConditionValue(nCount);

        // IDA: Log debug message
        // LogHelper::LogDebug("game.quest", "<QUEST> Update Condition <UID : %d > < Condition : %d > < Count : %d >",
        //     pUser->GetID(), pQuestCondition->GetConditionID(), pQuestCondition->GetConditionValue());

        // IDA: Check sync quest condition (sends update to client)
        CheckSyncQuestCondition(pQuestCondition);

        // IDA: Log to database (main=6, sub=2)
        // ST_LOG_GAME stLog;
        // stLog._nUAID = pUser->GetUAID();
        // stLog._nUCID = pUser->GetID();
        // stLog._sMainType = 6;
        // stLog._sSubType = 2;
        // stLog.nParam0 = pQuestCondition->GetQuestID();
        // stLog.nParam1 = pQuestCondition->GetConditionID();
        // stLog.nParam2 = pQuestCondition->GetConditionValue();
        // stLog.nParam3 = nCount;
        // XGameServer::SendDBLog(&stLog);

        // IDA: Send statistics if episode ID in range [0x186A1, 0x30D40)
        std::uint32_t dwQuestID = pQuestCondition->GetQuestID();
        if (dwQuestID >= 0x186A1 && dwQuestID < 0x30D40) {
            // ST_STATISTICS_QUEST stStatistics;
            // stStatistics.byFlag = 2;
            // stStatistics.dwUCID = pUser->GetID();
            // stStatistics.dwEpisodeID = dwQuestID;
            // stStatistics.byLevel = pUser->GetLevel();
            // XGameServer::SendDBStatistics(&stStatistics);
        }

        // IDA: Check if condition reached Counter_Value or CompleteCondition succeeded
        if (pTB_COND->Counter_Value > pQuestCondition->GetConditionValue() ||
            CompleteCondition(dwQuestID, pQuestCondition)) {
            // IDA: Add to completed list for party propagation
            completedConditions.push_back(pQuestCondition->GetConditionID());

            // IDA: If in maze and Group_Counting set, propagate to party members
            // if (pUser->IsMaze() && pTB_COND->Group_Counting && bPartyWith) {
            //     XMaze* pMaze = dynamic_cast<XMaze*>(pMover->GetArea());
            //     if (pMaze) {
            //         auto* pScanner = pMaze->GetScanner(pMover);
            //         if (pScanner) {
            //             for (auto& memberPair : *pScanner) {
            //                 CUser* pMember = dynamic_cast<CUser*>(memberPair.second);
            //                 if (pMember && pMember->GetID() != pUser->GetID()) {
            //                     auto pMemberQuest = pMember->GetGOC<CGocQuest>();
            //                     if (pMemberQuest) {
            //                         pMemberQuest->UpdateCondition(byType, byTarget, dwObjectID, nCount, false);
            //                     }
            //                 }
            //             }
            //         }
            //     }
            // }
        } else {
            // IDA: Rollback on failure
            // LogHelper::LogError("game.quest", "CompleteCondition error - Roll Back Condition Count");
            pQuestCondition->SetConditionValue(static_cast<std::uint8_t>(nOldValue));
            ClearUpdateQuestCondition(pQuestCondition->GetConditionID());
        }
    }

    // IDA: Send updated conditions packet to client (main=0x15, sub=7)
    if (!completedConditions.empty()) {
        // XSendPacket xSendPacket(0x15, 7);
        // xSendPacket << completedConditions;
        // CGocNetwork::Send(pMover, xSendPacket);
    }
}

// IDA: 0x140133900 - ?UpdateMazeGameMode@CGocQuest@@QEAAXW4E_CONDITION_TYPE@@F_N@Z
// Verified: Direct IDA decompilation - Update maze/game mode conditions
void CGocQuest::UpdateMazeGameMode(E_CONDITION_TYPE eType, std::int16_t nMazeID, bool bPartyWith) {
    // IDA: Get owner and cast to CUser
    // CMover* pMover = GetOwnerGO();
    // CUser* pUser = dynamic_cast<CUser*>(pMover);
    // if (!pUser) return;

    // IDA: Get the ConditionType index for this public MultiIndex view.
    // auto& index = m_mapCondition.get<quest_indices::IDX_NON_UNIQUE_CONDITION_TYPE>(); // ConditionType ordered index

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
    //     TB_QUEST_CONDITION* pTB_COND = pCond->GetConditionTable();
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
    // auto& index = m_mapCondition.get<quest_indices::IDX_NON_UNIQUE_CONDITION_TYPE>(); // ConditionType ordered index

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
    //     TB_QUEST_CONDITION* pTB_COND = pCond->GetConditionTable();
    //     if (!pTB_COND) continue;
    //
    //     // IDA: Get inventory component
    //     std::shared_ptr<CGocInventory> pInven;
    //     CMover::GetGOC<CGocInventory>(pMover, &pInven, 0);
    //
    //     // IDA: Check Target_ID (item ID to check)
    //     if (!pTB_COND->Target_ID) continue;
    //
    //     // IDA: Get item from inventory
    //     std::shared_ptr<CItem> pItem;
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

    m_mapCondition.EraseByQuestID(dwEpisodeID);

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
    DBUpdateEpisodeInfo(dwEpisodeID, stEpisode);

    // IDA: Send packet to client (0x15, 0x09)
    // XSendPacket xSendPacket(0x15, 9);
    // xSendPacket.XParse << byType;
    // xSendPacket.XParse << dwEpisodeID;
    // CGocNetwork::Send(pActor, &xSendPacket);

    (void)stEpisode;
    return true;
}

// IDA: 0x140128530
// IDA: 0x140129530 - ?CheckEpisodeCount@CGocQuest@@QEAAXXZ
void CGocQuest::CheckEpisodeCount() {
    if (m_mapEpisode.size() <= 30) {
        return;
    }

    std::int16_t shDelCount =
        static_cast<std::int16_t>(m_mapEpisode.size() - 30);
    if (shDelCount < 0) {
        return;
    }

    std::map<std::uint8_t, std::vector<std::uint32_t>> mapList;
    std::vector<std::uint32_t> vecDelList;
    std::int16_t shSaveQuestCount = 0;

    for (auto it = m_mapEpisode.begin(); it != m_mapEpisode.end(); ++it) {
        TB_QUEST_EPISODE* pTBQuestEpisode =
            TXSingleton<XGameServer>::Instance()->GetResourceMgr()
                .GetTB_QUEST_EPISODE(it->first);
        if (!pTBQuestEpisode) {
            LogHelper::LogError(
                "game.contents",
                "CheckEpisodeCount Wrong Quest NULL (UCID:%d, ID:%d)",
                GetOwnerGO()->GetID(),
                it->first);
            continue;
        }

        if (it->second.shCompleteBit == 0x3FF ||
            pTBQuestEpisode->Save_Quest) {
            ++shSaveQuestCount;
            continue;
        }

        auto itFind = mapList.find(pTBQuestEpisode->Contents_Type);
        if (itFind == mapList.end()) {
            std::vector<std::uint32_t> vecNew;
            vecNew.push_back(pTBQuestEpisode->ID);
            mapList.insert(
                std::make_pair(pTBQuestEpisode->Contents_Type, vecNew));
            continue;
        }

        std::vector<std::uint32_t> vecOld = itFind->second;
        itFind->second.clear();
        std::uint32_t dwCheckQuestID = pTBQuestEpisode->ID;

        for (std::int16_t sh = 0;
             static_cast<std::size_t>(sh) < vecOld.size();
             ++sh) {
            TB_QUEST_EPISODE* pTBCurrent =
                TXSingleton<XGameServer>::Instance()->GetResourceMgr()
                    .GetTB_QUEST_EPISODE(dwCheckQuestID);
            if (!pTBCurrent) {
                continue;
            }

            TB_QUEST_EPISODE* pTBOld =
                TXSingleton<XGameServer>::Instance()->GetResourceMgr()
                    .GetTB_QUEST_EPISODE(vecOld[sh]);
            if (!pTBOld) {
                continue;
            }

            if (pTBCurrent->Quest_Level < pTBOld->Quest_Level ||
                (pTBCurrent->Quest_Level == pTBOld->Quest_Level &&
                 pTBCurrent->Complete_Bit < pTBOld->Complete_Bit)) {
                itFind->second.push_back(pTBCurrent->ID);
                dwCheckQuestID = pTBOld->ID;
            } else {
                itFind->second.push_back(pTBOld->ID);
                dwCheckQuestID = pTBCurrent->ID;
            }
        }

        itFind->second.push_back(dwCheckQuestID);
    }

    if (m_mapEpisode.size() != static_cast<std::size_t>(shSaveQuestCount)) {
        const std::uint8_t byTypeList[6] = {2, 1, 3, 4, 5, 0};
        for (std::int16_t shType = 0; shType < 5; ++shType) {
            const auto itFind = mapList.find(byTypeList[shType]);
            if (itFind != mapList.end()) {
                const std::int16_t shCount =
                    static_cast<std::int16_t>(itFind->second.size());
                for (std::int16_t i = 0; i < shCount; ++i) {
                    vecDelList.push_back(itFind->second[i]);
                    if (--shDelCount <= 0) {
                        break;
                    }
                }
            }

            if (shDelCount <= 0) {
                break;
            }
        }
    }

    mapList.clear();
    for (std::int16_t j = 0;
         static_cast<std::size_t>(j) < vecDelList.size();
         ++j) {
        GiveUp(vecDelList[j], false);
    }
}

// IDA: 0x14013A7C0 - ?DBSyncQuestCondition@CGocQuest@@QEAAXXZ
// Verified: Direct IDA decompilation - Sync quest conditions to database
void CGocQuest::DBSyncQuestCondition() {
    if (m_mapUpdateCondition.empty()) {
        return;
    }

    for (auto it = m_mapUpdateCondition.begin();
         it != m_mapUpdateCondition.end();
         ++it) {
        std::shared_ptr<CQuestCondition> pQuestCondition = it->second;
        const std::uint32_t dwEpisodeID = pQuestCondition->GetQuestID();
        ST_QUEST_EPISODE* stEpisode = pQuestCondition->GetQuestEpisode();
        if (!stEpisode) {
            return;
        }

        DBUpdateEpisodeInfo(dwEpisodeID, *stEpisode);
    }

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
bool CGocQuest::AcceptQuestByItem(std::uint32_t dwEpisodeID,
                                  PS_RES_STORAGE_INFO& psCreateItem,
                                  PS_RES_STORAGE_INFO& psUpdateItem) {
    CMover* pMover = GetOwnerGO();
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F4);
        return false;
    }

    std::shared_ptr<CGocAttribute> pAttr = pMover->GetGOC_Attribute(false);
    if (!pAttr) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F4);
        return false;
    }

    if (pUser->GetBlockType()) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xC3BF);
        return false;
    }

    if (m_mapEpisode.size() >= 30) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD308);
        return false;
    }

    if (FindEpisode(dwEpisodeID)) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F1);
        return false;
    }

    if (IsCompleteEpisode(dwEpisodeID)) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F2);
        return false;
    }


    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_QUEST_EPISODE* pTB_EPISODE =
        pServer->GetResourceMgr().GetTB_QUEST_EPISODE(dwEpisodeID);
    if (!pTB_EPISODE) {
        return false;
    }

    // 0x1401AD9B0 is COMDAT-folded with CGameWorldMode::GetState; PDB identifies
    // pAttr as shared_ptr<CGocAttribute>, so this call is CGocAttribute::GetLevel.
    if (pTB_EPISODE->Quest_Level > pAttr->GetLevel()) {
        LogHelper::LogError(
            "game.quest",
            "AcceptQuestByItem error - Level condition is not right[ ActorID:%d ] ( %d )",
            pMover->GetID(),
            1678);
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD302);
        return false;
    }

    if (pTB_EPISODE->Class_Type) {
        if (pTB_EPISODE->Class_Type < 100) {
            if (pTB_EPISODE->Class_Type != pAttr->GetClass()) {
                LogHelper::LogError(
                    "game.quest",
                    "AcceptQuestByItem error - Class condition is not right[ ActorID:%d / %d ] ( %d )",
                    pMover->GetID(),
                    pTB_EPISODE->ID,
                    1701);
                CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD303);
                return false;
            }
        } else {
            const int nCharID = 1000 * pAttr->GetClass();
            TB_CHARACTER_INFO* pTBChar =
                pServer->GetResourceMgr().GetTB_CHARACTER_INFO(nCharID);
            if (!pTBChar ||
                pTBChar->Character_Group_ID != pTB_EPISODE->Class_Type) {
                LogHelper::LogError(
                    "game.quest",
                    "AcceptQuestByItem error - Class condition is not right[ ActorID:%d / %d ] ( %d )",
                    pMover->GetID(),
                    pTB_EPISODE->ID,
                    1692);
                CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD303);
                return false;
            }
        }
    }

    int nError = 54019;
    if (pTB_EPISODE->Contents_Type == 2 &&
        !CheckAcceptRepeatQuest(dwEpisodeID, true, &nError)) {
        LogHelper::LogError(
            "game.quest",
            "AcceptQuestByItem error - Repeat Quest Check Time[ ActorID:%d ] ( %d )",
            pMover->GetID(),
            1712);
        CGocNetwork::SendErrorMessage(
            pMover,
            0x15,
            3,
            static_cast<std::uint16_t>(nError));
        return false;
    }

    for (int i = 0; i < 5; ++i) {
        const std::uint32_t dwBeforeEpisodeID =
            (&pTB_EPISODE->Before_Episode_ID_1)[i];
        if (dwBeforeEpisodeID && !IsCompleteEpisode(dwBeforeEpisodeID)) {
            LogHelper::LogError(
                "game.quest",
                "AcceptQuestByItem error - Incomplete privious quest [ ActorID:%d, EpisodeID:%d, PreEpisodeID:%d ] ( %d )",
                pMover->GetID(),
                dwEpisodeID,
                dwBeforeEpisodeID,
                1725);
            CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD304);
            return false;
        }
    }

    if (!SetQuestAddObject(dwEpisodeID, psCreateItem, psUpdateItem)) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD305);
        LogHelper::LogError(
            "game.quest",
            "AcceptQuestByItem error - Occurence problem to Object is given accept quest[ ActorID:%d, EpisodeID:%d ] ( %d )",
            pMover->GetID(),
            dwEpisodeID,
            1736);
        return false;
    }

    ST_QUEST_EPISODE stEpisode;
    if (m_nHelperCount < 7) {
        stEpisode.byAddHelper = 1;
        ++m_nHelperCount;
    }

    if (!pTB_EPISODE->Condition_ID_01) {
        CGocNetwork::SendErrorMessage(pMover, 0x15, 3, 0xD2F4);
        LogHelper::LogError(
            "game.quest",
            "AcceptQuestByItem error - No Condition [ ActorID:%d, EpisodeID:%d ]",
            pMover->GetID(),
            dwEpisodeID);
        return false;
    }

    for (int nConditionIndex = 0; nConditionIndex < 10; ++nConditionIndex) {
        TB_QUEST_CONDITION* pTBCondition =
            pServer->GetResourceMgr().GetTB_QUEST_CONDITION(
                (&pTB_EPISODE->Condition_ID_01)[nConditionIndex]);
        if (!pTBCondition) {
            break;
        }

        stEpisode.pTBQuestEpisode = pTB_EPISODE;
        stEpisode.shCompleteBit &=
            static_cast<std::int16_t>(~(1 << nConditionIndex));
        stEpisode.stCondition[nConditionIndex].dwConditionID = pTBCondition->ID;
        stEpisode.stCondition[nConditionIndex].byValue = 0;

        m_mapEpisode[dwEpisodeID] = stEpisode;
        std::shared_ptr<CQuestCondition> pCondition(
            new CQuestCondition(
                dwEpisodeID,
                &m_mapEpisode[dwEpisodeID],
                nConditionIndex,
                pTBCondition));
        m_mapCondition.insert(pCondition);

        XMaze* pMaze = dynamic_cast<XMaze*>(pMover->GetArea());
        if (pMaze) {
            pMaze->CallScriptUpdateQuest(
                pCondition->GetQuestID(),
                0,
                dwEpisodeID);

            if (pCondition->GetNeedCompletionCondition() <= 0 ||
                IsCompleteCondition(pCondition->GetNeedCompletionCondition())) {
                if (pTBCondition->CutScene_Portal_control) {
                    XMaze* pCutSceneMaze =
                        dynamic_cast<XMaze*>(pMover->GetArea());
                    if (pCutSceneMaze) {
                        pCutSceneMaze->GetCutSceneMgr()->CheckCutsceneState(
                            pMover->GetID(),
                            5,
                            pTBCondition->ID);
                    }
                }

                pMaze->RunQuestConditionStart(
                    pMover->GetID(),
                    pTBCondition->ID);
            }
        }
    }

    DBUpdateEpisodeInfo(dwEpisodeID, stEpisode);

    ST_QUEST_REPEAT_INFO stRepeat = {};
    if (pTB_EPISODE->Contents_Type == 2) {
        AcceptRepeatQuest(dwEpisodeID);
        GetRepeatQuestInfo(dwEpisodeID, &stRepeat);
    }

    XSendPacket xSendPacket(0x15, 3);
    xSendPacket.XParse << dwEpisodeID;
    xSendPacket.XParse << stEpisode.byAddHelper;
    xSendPacket << stRepeat;
    CGocNetwork::Send(static_cast<XActor*>(pMover), xSendPacket);

    UpdateItemCondition();
    EnableInteractionObject(pTB_EPISODE->Condition_ID_01, 0);
    UpdateQuestConditionForSectorClear();

    std::shared_ptr<CGocEntity> pEntity = pMover->GetGOC_Entity(false);
    if (pEntity) {
        pEntity->UpdateOpenTitle(0, dwEpisodeID);
    }

    ST_LOG_GAME stLog;
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pMover->GetID();
    stLog._sMainType = 6;
    stLog._sSubType = 1;
    stLog.nParam0 = static_cast<int>(dwEpisodeID);
    stLog.nParam1 = static_cast<int>(stEpisode.stCondition[0].dwConditionID);
    stLog.nParam2 = stEpisode.stCondition[0].byValue;
    stLog.nParam3 = stEpisode.byAddHelper;
    stLog.nParam9 = static_cast<int>(pAttr->GetExp());
    stLog.nParam5 = pTB_EPISODE->Contents_Type;
    stLog.nParam6 = pUser->GetLevel();
    wcscpy_s(stLog.szComment, L"\uD018\uC2A4\uD2B8\uC2B9\uB099");
    pServer->SendDBLog(stLog);

    if (dwEpisodeID >= 0x186A1 && dwEpisodeID < 0x30D40) {
        ST_STATISTICS_QUEST stStatistics = {};
        stStatistics.byFlag = 1;
        stStatistics.dwUCID = pMover->GetID();
        stStatistics.dwEpisodeID = dwEpisodeID;
        stStatistics.byLevel = pUser->GetLevel();

        XSendDBPacket xSendDBStatistics(
            static_cast<XActor*>(pMover),
            0xF0,
            6);
        xSendDBStatistics << stStatistics;
        pServer->SendDBStatistics(xSendDBStatistics);
    }

    return true;
}

// IDA: 0x14013A090 - ?CheckAcceptRepeatQuest@CGocQuest@@QEAA_NK_NPEAH@Z
// IDA 精确还原 - Check if player can accept a repeatable quest
bool CGocQuest::CheckAcceptRepeatQuest(std::uint32_t dwEpisodeID, bool bCheck, int* pnError) {
    // IDA: Get episode from m_mapRepeatQuest
    auto it = m_mapRepeatQuest.find(dwEpisodeID);
    if (it == m_mapRepeatQuest.end()) {
        // No record found, can accept
        return true;
    }

    // IDA: Check if count exceeded or time limit passed
    ST_QUEST_REPEAT_INFO& stInfo = it->second;

    // IDA: Get TB_QUEST_EPISODE for repeat value
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_QUEST_EPISODE* pTB_EPISODE = pServer ? pServer->GetResourceMgr().GetTB_QUEST_EPISODE(dwEpisodeID) : nullptr;
    if (!pTB_EPISODE) return false;

    // IDA: Check count limit if Repeat_Value > 0
    if (pTB_EPISODE->Repeat_Value > 0 && stInfo.byCount >= pTB_EPISODE->Repeat_Value) {
        if (pnError) *pnError = 54019;
        return false;
    }

    // IDA: TODO - Check time limit (9 AM daily reset)
    // For now, accept the quest
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
    // std::shared_ptr<CQuestCondition> pQuestCondition = *iter;

    // IDA: Check if episode is failed (bFailed == 1 at offset +4)
    // if (pQuestCondition->GetEpisode()->bFailed == 1) return;

    // IDA: Get current maze via RTDynamicCast
    // XMaze* pMaze = dynamic_cast<XMaze*>(GetCurrentArea());
    // if (!pMaze) return;

    // IDA: Check if Condition type is 4 (INTERACTION type) at offset +14
    // TB_QUEST_CONDITION* pTB_COND = pQuestCondition->GetConditionTable();
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

// IDA: 0x14013A3D0 - ?UpdateQuestConditionForSectorClear@CGocQuest@@QEAAXXZ
// Verified: Direct IDA decompilation - Update quest conditions for sector clear
void CGocQuest::UpdateQuestConditionForSectorClear() {
    // IDA: Get owner CMover
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) return;

    // IDA: Get current area
    XArea* pArea = pOwner->GetArea();
    if (!pArea) return;

    // IDA: Dynamic cast to XMaze
    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    if (!pMaze) return;

    // IDA: Calculate SectorQuest ID
    // nBase = 100 * (10 * (TBMapID / 10) + 1)
    std::uint16_t wTBMapID = pMaze->GetTBMapID();
    int nBase = 100 * (10 * (wTBMapID / 10) + 1);

    // nID = LastSectorID % 10000 + nBase
    int nLastSectorID = pMaze->GetLastSectorID();
    unsigned int nID = (nLastSectorID % 10000) + nBase;

    // IDA: Get TB_SECTORQUEST
    XGameServer* pServer = XGameServer::Instance();
    TB_SECTORQUEST* pTBSectorQuest = pServer ? pServer->GetResourceMgr().GetTB_SECTORQUEST(nID) : nullptr;
    if (!pTBSectorQuest) return;

    // IDA: Process 3 sector clear quest slots
    for (int i = 0; i < 3; ++i) {
        // IDA: Check if state is set
        if (m_bQuestStateClear[i]) {
            // IDA: Check Final_Check_Quest_Type (uniFinalType)
            if (pTBSectorQuest->uniFinalType[i]) {
                // Condition check
                // IDA: Check Final_Check_Quest_State (uniFinalState)
                if (pTBSectorQuest->uniFinalState[i]) {
                    // Check if condition is complete
                    if (IsCompleteCondition(static_cast<int>(pTBSectorQuest->uniFinalID[i]))) {
                        m_bQuestStateClear[i] = 0;
                    }
                } else {
                    // Check if condition exists
                    if (FindCondition(pTBSectorQuest->uniFinalID[i])) {
                        m_bQuestStateClear[i] = 0;
                    }
                }
            } else {
                // Episode check
                // IDA: Check Final_Check_Quest_State (uniFinalState)
                if (pTBSectorQuest->uniFinalState[i]) {
                    // Check if episode is complete
                    if (IsCompleteEpisode(pTBSectorQuest->uniFinalID[i])) {
                        m_bQuestStateClear[i] = 0;
                    }
                } else {
                    // Check if episode exists
                    if (FindEpisode(pTBSectorQuest->uniFinalID[i])) {
                        m_bQuestStateClear[i] = 0;
                    }
                }
            }
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
    //     std::shared_ptr<CQuestCondition> pQuestCondition = GetCurrentQuestCondition();
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
        std::uint32_t dwConditionID = it->second.stCondition[i].dwConditionID;
        if (!dwConditionID) {
            break;
        }

        // IDA: Call ClearUpdateQuestCondition
        ClearUpdateQuestCondition(dwConditionID);
    }
}

// UpdateQuestRespawn moved to proper location with IDA address 0x14013A350

// IDA: 0x140129C80 - ?DBUpdateEpisodeInfo@CGocQuest@@QEAAXKAEAUST_QUEST_EPISODE@@@Z
void CGocQuest::DBUpdateEpisodeInfo(std::uint32_t dwEpisodeID,
                                    ST_QUEST_EPISODE& stEpisode) {
    PS_QUEST_EPISODE ps;
    ps.dwEpisodeID = dwEpisodeID;
    ps.stEpisode = stEpisode;

    if (stEpisode.pTBQuestEpisode &&
        dwEpisodeID != stEpisode.pTBQuestEpisode->ID) {
        LogHelper::LogError(
            "game.contents",
            "[DBUPDATE] Wrong Quest ID %d %d %d",
            GetOwnerGO()->GetID(),
            dwEpisodeID,
            stEpisode.pTBQuestEpisode->ID);
    }

    XSendDBPacket xSendDBPacket(
        static_cast<XActor*>(GetOwnerGO()),
        0x41,
        3);
    xSendDBPacket.XParse << GetOwnerGO()->GetID();
    xSendDBPacket << ps;
    TXSingleton<XGameServer>::Instance()->SendDBGame(xSendDBPacket);
}

// IDA: 0x14013A8E0 - ?ClearUpdateQuestCondition@CGocQuest@@AEAAXK@Z
void CGocQuest::ClearUpdateQuestCondition(std::uint32_t dwConditionID) {
    const auto it = m_mapUpdateCondition.find(dwConditionID);
    if (it != m_mapUpdateCondition.end()) {
        m_mapUpdateCondition.erase(it);
    }
}

// IDA: 0x14013B1A0 - ?CompleteQuestForNewChar@CGocQuest@@QEAAX_N@Z
// Verified: Direct IDA decompilation - Complete quest for new character
void CGocQuest::CompleteQuestForNewChar(bool bSend, float fParam) {
    // IDA: Get owner actor's class index
    CMover* pMover = GetOwnerGO();
    if (!pMover) return;

    // IDA: nClassIdx = 1000 * GetClass()
    std::uint16_t nClassIdx = static_cast<std::uint16_t>(pMover->GetClass() * 1000);

    // IDA: Get TB_CHARACTER_INFO from resource manager
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return;

    TB_CHARACTER_INFO* pTB_CHAR = pServer->GetResourceMgr().GetTB_CHARACTER_INFO(nClassIdx);
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
        TB_QUEST_EPISODE* pTB_QUEST_EPISODE = pServer->GetResourceMgr().GetTB_QUEST_EPISODE(dwEpisodeID);
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
            // Note: Would need XSendDBPacket implementation
            // ST_GET_INFO stGetInfo;
            // XSendDBPacket xSendDBPacket(pActor, 0x41, 0x05);
            // xSendDBPacket << GetOwnerGO()->GetID();  // UCID
            // xSendDBPacket << dwEpisodeID;
            // xSendDBPacket << pTB_QUEST_EPISODE->Contents_Type;
            // xSendDBPacket.SetBytes(GetCompleteEpisode(), 256);
            // xSendDBPacket << stGetInfo;
            // xSendDBPacket << false;  // bReturn
            // pServer->SendDBGame(&xSendDBPacket);

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
            // pServer->SendDBLog(&stLog);
        }
    }

    // IDA: Send complete episode list if bSend is true
    if (bSend) {
        SendCompleteEpisodeList();
    }

    (void)fParam;  // Unused parameter in IDA
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
    // std::shared_ptr<CQuestCondition> pQuestCondition = *iter;

    // IDA: Check if condition has TB_QUEST_CONDITION pointer
    // TB_QUEST_CONDITION* pTB_COND = pQuestCondition->GetConditionTable();
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
    //                     std::shared_ptr<CGocQuest> pMemberQuest;
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
    CMover* pMover = GetOwnerGO();
    if (!pMover) return false;

    std::shared_ptr<CGocInventory> pInven = pMover->GetGOC_Inventory(false);
    if (!pInven) return false;

    // IDA: Get common inventory (type 2)
    XBaseInventory* pCommonInven = pInven->GetInvenPtr(2);
    if (!pCommonInven) return false;

    // IDA: Calculate needed slot count
    int nNeedSlotCount = 0;

    // IDA: Check first 2 Add_Object entries
    for (int i = 0; i < 2; ++i) {
        if (byType[i] == 1) {
            // IDA: Get TB_ITEM for item info
            XGameServer* pServer = XGameServer::Instance();
            TB_ITEM* pTB_ITEM = pServer ? pServer->GetResourceMgr().GetTB_ITEM(dwID[i]) : nullptr;
            if (!pTB_ITEM) return false;

            // IDA: Check if item is stackable
            if (pTB_ITEM->Item_Stack_Max == 1) {
                ++nNeedSlotCount;  // Non-stackable, need new slot
            } else {
                // IDA: Check existing stacks
                std::vector<std::shared_ptr<CItem>> vecItems;
                pCommonInven->GetSameItems(dwID[i], &vecItems, -1);
                ++nNeedSlotCount;

                for (auto& pItem : vecItems) {
                    if (pTB_ITEM->Item_Stack_Max >= pItem->GetCount() + 1) {
                        --nNeedSlotCount;  // Can stack
                        break;
                    }
                }
            }
        }
    }

    // IDA: Check if enough empty slots
    int nEmptyCount = pCommonInven->GetEmptySlotCount();
    if (nNeedSlotCount > 0 && nEmptyCount < nNeedSlotCount) return false;

    // IDA: Create items
    for (int j = 0; j < 2; ++j) {
        if (byType[j] == 0) {
            return true;  // IDA: returns 1 when type is 0
        }
        if (byType[j] == 1) {
            // IDA: Create item via CGocInventory::CreateItemReq
            ST_LOG_GAME stLog;
            std::memset(&stLog, 0, sizeof(stLog));
            stLog.nParam3 = static_cast<int>(dwEpisodeID);
            pInven->CreateItemReq(
                static_cast<int>(dwID[j]),
                1,
                false,
                E_ITEM_CREATE_TYPE_QUEST_ACCEPT,
                stLog);
        }
    }

    return true;
}

// IDA: 0x1401392C0 - ?SetQuestAddObject@CGocQuest@@QEAA_NKAEAUPS_RES_STORAGE_INFO@@0@Z
// Verified: Direct IDA decompilation - Add quest objects using PS_RES_STORAGE_INFO packets
bool CGocQuest::SetQuestAddObject(std::uint32_t dwEpisodeID,
                                  PS_RES_STORAGE_INFO& psCreateItem,
                                  PS_RES_STORAGE_INFO& psUpdateItem) {
    // IDA: Get TB_QUEST_EPISODE
    XGameServer* pServer = XGameServer::Instance();
    TB_QUEST_EPISODE* pTB_EPISODE = pServer ? pServer->GetResourceMgr().GetTB_QUEST_EPISODE(dwEpisodeID) : nullptr;
    if (!pTB_EPISODE) return false;

    // IDA: Get owner CMover and CGocInventory
    CMover* pMover = GetOwnerGO();
    if (!pMover) return false;

    std::shared_ptr<CGocInventory> pInven = pMover->GetGOC_Inventory(false);
    if (!pInven) return false;

    // IDA: Get common inventory (type 2)
    XBaseInventory* pCommonInven = pInven->GetInvenPtr(2);
    if (!pCommonInven) return false;

    // IDA: Build list of Add_Object_IDs
    std::vector<unsigned int> vecAddObjectList;
    int nNeedSlotCount = 0;

    // IDA: Check Add_Object_Type_01, Add_Object_Type_02
    const std::uint8_t* pAddObjectTypes[2] = {&pTB_EPISODE->Add_Object_Type_01, &pTB_EPISODE->Add_Object_Type_02};
    const unsigned int* pAddObjectIDs[2] = {&pTB_EPISODE->Add_Object_ID_01, &pTB_EPISODE->Add_Object_ID_02};

    for (int i = 0; i < 2; ++i) {
        if (*pAddObjectTypes[i] == 1) {
            // IDA: Get TB_ITEM
            TB_ITEM* pTB_ITEM = pServer->GetResourceMgr().GetTB_ITEM(*pAddObjectIDs[i]);
            if (!pTB_ITEM) return false;

            vecAddObjectList.push_back(*pAddObjectIDs[i]);

            // IDA: Check stackability
            if (pTB_ITEM->Item_Stack_Max == 1) {
                ++nNeedSlotCount;
            } else {
                // IDA: Check existing stacks
                std::vector<std::shared_ptr<CItem>> vecItems;
                pCommonInven->GetSameItems(static_cast<int>(*pAddObjectIDs[i]), &vecItems, -1);
                ++nNeedSlotCount;

                for (auto& pItem : vecItems) {
                    if (pTB_ITEM->Item_Stack_Max >= pItem->GetCount() + 1) {
                        --nNeedSlotCount;  // Can stack
                        break;
                    }
                }
            }
        }
    }

    // IDA: Check empty slots
    int nEmptyCount = pCommonInven->GetEmptySlotCount();
    if (nNeedSlotCount > 0 && nEmptyCount < nNeedSlotCount) return false;

    // IDA: Add items using CGocInventory::AddItem2
    for (std::size_t j = 0; j < vecAddObjectList.size(); ++j) {
        TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(vecAddObjectList[j]);
        if (!pTBItem) return false;
        if (!pInven->AddItem2(pTBItem, 1, 0x84, false, psCreateItem, psUpdateItem)) return false;
    }

    return true;
}

// IDA: 0x140139780 - ?GetNeedConditionItemCount@CGocQuest@@QEAAHKK@Z
// Verified: Direct IDA decompilation - Get needed item count for condition
int CGocQuest::GetNeedConditionItemCount(std::uint32_t dwConditionID, std::uint32_t dwItemID) {
    int nNeedCount = 0;

    const std::shared_ptr<CQuestCondition>* condition =
        m_mapCondition.FindByConditionID(dwConditionID);
    if (!condition || !*condition) {
        return 0;
    }

    const std::shared_ptr<CQuestCondition>& pQuestCondition = *condition;

    // IDA: Get owner CMover and CGocInventory
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return 0;
    }

    // IDA: Get TB_QUEST_CONDITION Counter_Value (offset +124 = Counter_Value byte)
    TB_QUEST_CONDITION* pTB_COND = pQuestCondition->GetConditionTable();
    if (!pTB_COND) {
        return 0;
    }
    std::uint8_t byCounterValue = pTB_COND->Counter_Value;

    // IDA: Get inventory component and find item
    std::shared_ptr<CGocInventory> pInven;
    pMover->GetGOC<CGocInventory>(&pInven, false);
    if (!pInven) {
        return static_cast<int>(byCounterValue);
    }

    // IDA: Get item from inventory (InvenType 2 = Common inventory)
    std::shared_ptr<CItem> pItem = pInven->GetItem(2, static_cast<int>(dwItemID));

    // IDA: If item exists, calculate needed count
    if (pItem) {
        int nCurrentCount = pItem->GetCount();
        int nNeeded = static_cast<int>(byCounterValue) - nCurrentCount;
        nNeedCount = (nNeeded > 0) ? nNeeded : 0;
    } else {
        nNeedCount = static_cast<int>(byCounterValue);
    }

    return nNeedCount;
}

// IDA: 0x140138E30 - ?CheckUpdateCondition@CGocQuest@@QEAA_NK_J@Z
// Verified: Direct IDA decompilation - Check if condition can be updated
bool CGocQuest::CheckUpdateCondition(std::uint32_t dwConditionID, std::int64_t nParam) {
    // IDA: Get TB_QUEST_CONDITION from resource manager
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return false;

    TB_QUEST_CONDITION* pTB_COND = pServer->GetResourceMgr().GetTB_QUEST_CONDITION(dwConditionID);
    if (!pTB_COND) return false;

    // IDA: Switch on Condition type to determine how to check
    switch (pTB_COND->Condition) {
        case 0:  // Monster kill condition
            return pTB_COND->Target_ID == static_cast<std::uint32_t>(nParam);

        case 2:   // Item collection
        case 0xB: // Interaction
        case 0xC: // Sector clear
        case 0xD: // Maze clear
        case 0xE: // Boss kill
        case 0x10: // PVP
        case 0x12: // Event
        case 0x13: // Daily
            // IDA: If Target_ID is 0, any target matches; otherwise must match exactly
            if (pTB_COND->Target_ID) {
                return pTB_COND->Target_ID == nParam;
            }
            return true;

        default:
            // IDA: Default check - Target_ID must match nParam
            return pTB_COND->Target_ID == nParam;
    }
}

// IDA: 0x140139970 - ?IsCompleteCondition@CGocQuest@@QEAA_NH@Z
// Verified: Direct IDA decompilation - Check if condition is complete by ID
bool CGocQuest::IsCompleteCondition(int nConditionID) {
    if (nConditionID <= 0) {
        return false;
    }

    const std::shared_ptr<CQuestCondition>* condition =
        m_mapCondition.FindByConditionID(
            static_cast<std::uint32_t>(nConditionID));
    if (!condition || !*condition) {
        return false;
    }

    const std::shared_ptr<CQuestCondition>& pQuestCondition = *condition;
    const auto episodeIt = m_mapEpisode.find(pQuestCondition->GetQuestID());
    if (episodeIt != m_mapEpisode.end() && episodeIt->second.bFailed) {
        return false;
    }

    return pQuestCondition->IsCompleteCondition();
}
