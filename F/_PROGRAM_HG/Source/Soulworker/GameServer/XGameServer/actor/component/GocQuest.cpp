#include "GocQuest.h"
#include <cstring>
#include <ctime>

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
    // Clear completed episode bit array
    std::memset(m_szCompleteEpisode, 0, sizeof(m_szCompleteEpisode));

    // Clear quest state clear array
    std::memset(m_bQuestStateClear, 0, sizeof(m_bQuestStateClear));

    // Clear all containers
    m_mapCondition.clear();
    m_mapEpisode.clear();
    m_mapRepeatQuest.clear();
    m_mapQuestFirstDrop.clear();
    m_mapUpdateCondition.clear();

    // Reset time to current time
    m_tLastInitDate = std::time(nullptr);

    // Reset counters
    m_nHelperCount = 0;
    m_bLoad = false;
    m_bSendMsgSectorClear = false;
    m_bComplete = false;
}

// IDA: 0x1401250A0
void CGocQuest::OnUpdate() {
    // TODO: 汇编还原 - Need to implement full quest update logic
    // This function checks quest timers, daily resets, etc.
}

// IDA: 0x140126240
void CGocQuest::SendEpisodeList() {
    // Send episode list packet to client (main=0x15, sub=0x02)
    // Packet contains current episode map and repeat quest info
    // TODO: 需人工审查 - Requires XSendPacket, PS_QUEST_EPISODE_MAP, PS_REPEAT_QUEST_MAP types
    // Decompiled logic:
    // 1. Create PS_QUEST_EPISODE_MAP and copy m_mapEpisode to it
    // 2. Create PS_REPEAT_QUEST_MAP and copy m_mapRepeatQuest to it
    // 3. Create XSendPacket(0x15, 0x02)
    // 4. Serialize both maps into packet
    // 5. Send via CGocNetwork::Send()
}

// IDA: 0x140126400
void CGocQuest::SendCompleteEpisodeList() {
    // Send complete episode bit array to client (main=0x15, sub=0x01)
    // IDA-verified: Send 256-byte bit array of completed episodes
    // TODO: 需人工审查 - Requires XSendPacket, XParse, CGocNetwork::Send
    // Decompiled logic:
    // 1. Create XSendPacket(0x15, 0x01)
    // 2. XParse::SetBytes(packet, m_szCompleteEpisode, 256)
    // 3. Send via CGocNetwork::Send()
}

// IDA: 0x140128EF0
void CGocQuest::SendReqQuestList() {
    // TODO: 汇编还原 - Need XSendPacket and packet serialization
    // Sends the request quest list to the client
}

// IDA: 0x1401264E0
bool CGocQuest::FindEpisode(std::uint32_t dwEpisodeID) const {
    // IDA-verified: Find episode in active quest map
    // Returns true if episode exists and state is not failed (state != 1)
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it == m_mapEpisode.end()) {
        return false;
    }
    // Check episode state - state is stored in second.m_eObjectFlags (BYTE4)
    // Episode state 1 = failed, so we return false for failed episodes
    if (it->second == nullptr) {
        return false;
    }
    // State is at offset +4 in the structure (BYTE4 of __vftable field in IDA)
    // TODO: 需人工审查 - Need complete ST_QUEST_EPISODE type to access byState field
    // The state byte is: *(reinterpret_cast<const std::uint8_t*>(it->second) + 4)
    return true; // Stub: should check byState != 1
}

// IDA: 0x140126560
bool CGocQuest::FindCondition(std::uint32_t dwConditionID) const {
    auto it = m_mapCondition.find(dwConditionID);
    if (it == m_mapCondition.end()) {
        return false;
    }
    // TODO: Check if condition is complete using CQuestCondition::IsCompleteCondition
    return true;
}

// IDA: 0x140126690
bool CGocQuest::IsCompleteEpisode(std::uint32_t dwEpisodeID) const {
    // IDA-verified: Check if episode is marked complete in bit array
    // Logic:
    // 1. Get TB_QUEST_EPISODE from resource manager
    // 2. Check Class_Type - if >= 100, validate character group
    // 3. Check complete bit in m_szCompleteEpisode array
    // 4. Bit index = Complete_Bit / 8, bit pos = Complete_Bit % 8

    // TODO: 需人工审查 - Requires XResourceMgr, TB_QUEST_EPISODE, XGameServer singleton
    // For now, check the bit array directly
    // Need to call XResourceMgr::GetTB_QUEST_EPISODE() to get the Complete_Bit value

    // Placeholder: Check bit array for episode ID as index (not correct, need table lookup)
    // Correct logic from IDA:
    // pTB_EPISODE = XResourceMgr::GetTB_QUEST_EPISODE(dwEpisodeID)
    // if (!pTB_EPISODE) return false
    // nIndex = pTB_EPISODE->Complete_Bit / 8
    // nPos = pTB_EPISODE->Complete_Bit % 8
    // if (nIndex > 256) return false
    // return (m_szCompleteEpisode[nIndex] & (1 << nPos)) != 0

    (void)dwEpisodeID;
    return false; // Stub - requires resource manager integration
}

// IDA: 0x140138970
bool CGocQuest::IsCompleteCondition(int nConditionID) const {
    auto it = m_mapCondition.find(static_cast<std::uint32_t>(nConditionID));
    if (it == m_mapCondition.end()) {
        return false;
    }
    // TODO: Call CQuestCondition::IsCompleteCondition on the condition object
    return false;
}

// IDA: 0x140127170
bool CGocQuest::ValidCompleteEpisode(std::uint32_t dwEpisodeID) const {
    // TODO: 汇编还原 - Validate if episode can be completed
    return IsCompleteEpisode(dwEpisodeID);
}

// IDA: 0x14012BBD0
bool CGocQuest::AcceptQuest(std::uint32_t dwEpisodeID, bool bCheckMaxCount) {
    // IDA-verified quest acceptance logic:
    // 1. Get CUser from actor and check block type
    // 2. Check max quest count (30 = 0x1E) if bCheckMaxCount
    // 3. Check if already have this episode (FindEpisode)
    // 4. Check if already completed (IsCompleteEpisode)
    // 5. Get TB_QUEST_EPISODE from resource manager
    // 6. Check level requirement (Quest_Level <= player level)
    // 7. Check class requirement (Class_Type)
    // 8. Check repeat quest time if Contents_Type == 2
    // 9. Check before episode completion (Before_Episode_ID_1..5)
    // 10. SetQuestAddObject for quest items
    // 11. Create ST_QUEST_EPISODE and conditions
    // 12. Insert into m_mapEpisode and m_mapCondition
    // 13. Send packet to client (main=0x15, sub=0x03)
    // 14. Log to database (main=6, sub=1)
    // 15. Send statistics if episode in range [0x186A1, 0x30D40)

    (void)bCheckMaxCount;

    // Quick validation checks
    if (FindEpisode(dwEpisodeID)) {
        // Already have this episode - send error 0xD2F1
        return false;
    }

    if (IsCompleteEpisode(dwEpisodeID)) {
        // Already completed - send error 0xD2F2
        return false;
    }

    // TODO: 需人工审查 - Requires complete implementation with:
    // - CUser pointer from actor
    // - XResourceMgr::GetTB_QUEST_EPISODE()
    // - TB_QUEST_EPISODE structure
    // - Condition creation
    // - Packet sending
    // - Database logging

    return false;
}

// IDA: 0x14012F100
bool CGocQuest::CompleteQuest(std::uint32_t dwEpisodeID, std::uint32_t dwRewardItemID) {
    // IDA-verified quest completion logic:
    // 1. Check if already complete (IsCompleteEpisode)
    // 2. Validate episode can be completed (ValidCompleteEpisode)
    // 3. Find episode in m_mapEpisode
    // 4. Process remove items (Remove_Item_ID_01..05, Remove_Item_Count_01..05)
    // 5. Process rewards based on Reward_Item_type:
    //    - Type 1: Direct item rewards (Reward_Item_ID_01..05)
    //    - Type 2: Random rewards via TB_QUEST_REWARD table
    // 6. Process select rewards (Select_Reward_Item_type, Select_Reward_Item_01..05)
    // 7. Create reward items via CGocInventory::CreateItemReq
    // 8. Process titles (Reward_Title_01..02)
    // 9. Process helper reward if Contents_Type == 3
    // 10. Process exp (Reward_EXP with booster bonus)
    // 11. Process money (Reward_Money)
    // 12. Process BP (Reward_BP)
    // 13. Process Ether (Reward_Ether)
    // 14. Process Awakening (Reward_Awakening)
    // 15. Process class skills (Reward_Class_Skill_01..04)
    // 16. Set complete bit in m_szCompleteEpisode
    // 17. Update quest condition for sector clear
    // 18. Send DB packet (main=0x41, sub=5)
    // 19. Log to database (main=6, sub=4)
    // 20. Send statistics if episode in range [0x186A1, 0x30D40)

    (void)dwRewardItemID;
    (void)dwEpisodeID;

    // Check if already complete
    if (IsCompleteEpisode(dwEpisodeID)) {
        return false;
    }

    // TODO: 需人工审查 - Requires complete implementation with:
    // - ValidCompleteEpisode check
    // - Episode lookup in m_mapEpisode
    // - TB_QUEST_EPISODE structure access
    // - Inventory operations
    // - Reward processing
    // - Database logging

    return false;
}

// IDA: 0x1401324A0
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

// IDA: 0x14012E1F0
bool CGocQuest::AcceptQuestByForce(std::uint32_t dwEpisodeID) {
    // IDA-verified: Accept quest without normal validation
    // Similar to AcceptQuest but skips level/class/before episode checks
    (void)dwEpisodeID;

    // TODO: 需人工审查 - Requires complete implementation with:
    // - Episode lookup in resource manager
    // - Condition creation
    // - Packet sending
    // - Database logging
    return false;
}

// IDA: 0x140130C50
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

// IDA: 0x140126890
bool CGocQuest::CompleteConditionByForce(std::uint32_t dwConditionID) {
    // Force complete a condition without normal validation
    // TODO: 需人工审查 - Requires CQuestCondition lookup and CompleteCondition call
    (void)dwConditionID;
    return false;
}

// IDA: 0x140139B60
void CGocQuest::FailQuest(std::uint32_t dwQuestID) {
    // Mark episode as failed (state = 2)
    // IDA-verified: Set episode state to failed
    auto it = m_mapEpisode.find(dwQuestID);
    if (it != m_mapEpisode.end() && it->second != nullptr) {
        // TODO: 需人工审查 - Set byState = 2 on ST_QUEST_EPISODE
        // it->second->byState = 2;
    }
}

// IDA: 0x140139610
bool CGocQuest::ResetQuest(std::uint32_t dwEpisodeID) {
    // Reset episode to initial state
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it != m_mapEpisode.end() && it->second != nullptr) {
        // TODO: 需人工审查 - Reset episode state and conditions
        // Reset byState to 0, reset condition values
        return true;
    }
    return false;
}

// IDA: 0x140137650
void CGocQuest::ResetQuestAll() {
    // Reset all episodes
    for (auto& pair : m_mapEpisode) {
        if (pair.second != nullptr) {
            // TODO: 需人工审查 - Reset each episode state
        }
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

// IDA: 0x140136820
bool CGocQuest::UpdateCondition(std::uint32_t dwConditionID, int nCount, bool bPartyWith) {
    // IDA-verified: Update specific condition by ID
    // Similar logic but filters by ConditionID instead of Type
    (void)dwConditionID;
    (void)nCount;
    (void)bPartyWith;

    // TODO: 需人工审查 - Requires CQuestCondition lookup and update
    return false;
}

// IDA: 0x140133330
void CGocQuest::UpdateItemCondition() {
    // Check inventory and update item-based conditions
    // IDA-verified: Iterate inventory items and update conditions with type == item condition
    // TODO: 需人工审查 - Requires inventory iteration and condition type matching
}

// IDA: 0x140127730
bool CGocQuest::DeleteEpisode(std::uint32_t dwEpisodeID) {
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it != m_mapEpisode.end()) {
        // Also remove associated conditions from m_mapCondition
        // TODO: 需人工审查 - Iterate conditions by QuestID and erase them
        m_mapEpisode.erase(it);
        return true;
    }
    return false;
}

// IDA: 0x140127810
bool CGocQuest::DeleteFailedEpisode(std::uint32_t dwEpisodeID) {
    auto it = m_mapEpisode.find(dwEpisodeID);
    if (it != m_mapEpisode.end() && it->second != nullptr) {
        // Check if episode state is failed (byState == 1)
        // TODO: 需人工审查 - Check it->second->byState == 1 before erasing
        m_mapEpisode.erase(it);
        return true;
    }
    return false;
}

// IDA: 0x140127A70
bool CGocQuest::SetEpisodeHelper(std::uint32_t dwEpisodeID, std::uint8_t byAddHelper) {
    // TODO: 汇编还原 - Set helper flag for episode
    (void)dwEpisodeID;
    (void)byAddHelper;
    return true;
}

// IDA: 0x140132690
bool CGocQuest::SetHelper(std::uint32_t dwEpisodeID, std::uint8_t byHelper) {
    // TODO: 汇编还原 - Set helper count for episode
    (void)dwEpisodeID;
    (void)byHelper;
    return true;
}

// IDA: 0x140128530
void CGocQuest::CheckEpisodeCount() {
    // TODO: 汇编还原 - Check episode count and remove old ones if needed
}

// IDA: 0x1401397C0
void CGocQuest::DBSyncQuestCondition() {
    // TODO: 汇编还原 - Send quest condition sync to database
}

// IDA: 0x140124DB0
const char* CGocQuest::GetCompleteEpisode() const {
    return reinterpret_cast<const char*>(m_szCompleteEpisode);
}

// IDA: 0x14013A090
bool CGocQuest::CheckAcceptRepeatQuest(std::uint32_t dwEpisodeID, bool bCheck, int* pnError) {
    // TODO: 汇编还原 - Check if repeat quest can be accepted
    (void)dwEpisodeID;
    (void)bCheck;
    (void)pnError;
    return true;
}

// IDA: 0x14013A6D0
void CGocQuest::AcceptRepeatQuest(std::uint32_t dwEpisodeID) {
    // TODO: 汇编还原 - Accept a repeatable quest
    (void)dwEpisodeID;
}

// IDA: 0x14013A930
void CGocQuest::CompleteRepeatQuest(std::uint32_t dwEpisodeID) {
    // TODO: 汇编还原 - Complete a repeatable quest
    (void)dwEpisodeID;
}

// IDA: 0x14013ABD0
void CGocQuest::ResetRepeatQuest(std::uint32_t dwEpisodeID) {
    // TODO: 汇编还原 - Reset a repeatable quest
    (void)dwEpisodeID;
}

// IDA: 0x140138A60
void CGocQuest::EnableInteractionObject(std::uint32_t dwConditionID, int nParam) {
    // TODO: 汇编还原 - Enable interaction objects for condition
    (void)dwConditionID;
    (void)nParam;
}

// IDA: 0x140138CE0
void CGocQuest::DisableInteractionObject(std::uint32_t dwConditionID, int nType, int nParam) {
    // TODO: 汇编还原 - Disable interaction objects for condition
    (void)dwConditionID;
    (void)nType;
    (void)nParam;
}

// IDA: 0x1401393D0
void CGocQuest::UpdateQuestConditionForSectorClear() {
    // TODO: 汇编还原 - Update conditions for sector clear
}

// IDA: 0x140138E40
void CGocQuest::InitQuestConditionForSectorClear() {
    // TODO: 汇编还原 - Initialize conditions for sector clear
}

// IDA: 0x14013B260
void CGocQuest::AddQuestFirstDropItem(std::uint32_t dwEpisodeID, int nItemID, int nCount) {
    // TODO: 汇编还原 - Add first drop item for quest
    (void)dwEpisodeID;
    (void)nItemID;
    (void)nCount;
}

// IDA: 0x14013B1B0
void CGocQuest::SetQuestFirstDropItem(std::uint32_t dwEpisodeID, int nItemID, int nCount) {
    // TODO: 汇编还原 - Set first drop item for quest
    (void)dwEpisodeID;
    (void)nItemID;
    (void)nCount;
}

// IDA: 0x14013B370
bool CGocQuest::CheckQuestFirstDropItem(std::uint32_t dwEpisodeID) {
    // TODO: 汇编还原 - Check first drop item
    (void)dwEpisodeID;
    return false;
}

// IDA: 0x14030F530
bool CGocQuest::IsSendMsgSectorClear() const {
    return m_bSendMsgSectorClear;
}

// IDA: 0x140139200
void CGocQuest::SetSectorClearQuestState(bool b) {
    m_bSendMsgSectorClear = b;
}

// IDA: 0x140139FF0
void CGocQuest::GetRepeatQuestInfo(std::uint32_t dwEpisodeID, ST_QUEST_REPEAT_INFO* pInfo) {
    // TODO: 汇编还原 - Get repeat quest info
    (void)dwEpisodeID;
    (void)pInfo;
}

// IDA: 0x140139D20
void CGocQuest::SetRepeatQuestList(std::uint32_t dwEpisodeID, const ST_QUEST_REPEAT_INFO* pInfo) {
    // TODO: 汇编还原 - Set repeat quest info
    (void)dwEpisodeID;
    (void)pInfo;
}
