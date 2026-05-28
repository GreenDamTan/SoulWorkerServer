// GocDailyMission.cpp - CGocDailyMission implementation
// Reconstructed from GameServer.exe IDA analysis

#include "GocDailyMission.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <algorithm>

// Forward declarations for CDailyMissionInfo (to be implemented separately)
class CDailyMissionInfo {
public:
    CDailyMissionInfo(std::uint32_t dwMissionID, TB_DAILY_MISSION* pTBMission);
    ~CDailyMissionInfo();

    void InitAddMission(ST_DAILY_MISSION_INFO* stMission);
    void InitAcceptMission();

    std::uint8_t GetDailyMissionState() const;
    void SetDailyMissionState(std::uint8_t byState);

    std::int16_t GetConditionValue() const;
    void AddConditionValue(std::int16_t shValue);

    bool GetDailyMissionHelper() const;
    void SetDailyMissionHelper(bool bHelper);

    TB_DAILY_MISSION* GetTBMission() const { return m_pTBMission; }
    ST_DAILY_MISSION_INFO* GetMissionInfo() const { return m_pMissionInfo; }

private:
    std::uint32_t m_dwMissionID;
    TB_DAILY_MISSION* m_pTBMission;
    ST_DAILY_MISSION_INFO* m_pMissionInfo;
};

// ============================================================================
// CGocDailyMission Implementation
// ============================================================================

// IDA: ??0CGocDailyMission@@QEAA@XZ (0x14004EAD0)
// Constructor - initializes base class and member maps
CGocDailyMission::CGocDailyMission()
    : GOComponent()
    , m_nHelperCount(0)
{
    // Maps are default constructed
}

// IDA: ??1CGocDailyMission@@UEAA@XZ (0x14004EB80)
// Destructor - clears all mission maps
CGocDailyMission::~CGocDailyMission()
{
    Clear();
}

// IDA: ?Init@CGocDailyMission@@QEAA_NXZ (0x14004EBF0)
// Initialize the component by clearing existing data
bool CGocDailyMission::Init()
{
    Clear();
    return true;
}

// IDA: ?Clear@CGocDailyMission@@QEAAXXZ (0x14004EC10)
// Clear all mission maps and reset helper count
void CGocDailyMission::Clear()
{
    m_mapSpecial.clear();
    m_mapGuerrilla.clear();
    m_mapEvent.clear();
    m_nHelperCount = 0;
}

// IDA: ?GetMissionInfo@CGocDailyMission@@QEAA?AV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@KE@Z (0x14004EC60)
// Get mission info by mission ID and type
std::tr1::shared_ptr<CDailyMissionInfo> CGocDailyMission::GetMissionInfo(std::uint32_t dwMissionID, std::uint8_t byType)
{
    std::map<std::uint32_t, std::tr1::shared_ptr<CDailyMissionInfo>>* pMap = nullptr;

    switch (byType)
    {
    case eDAILY_MISSION_TYPE_SPECIAL:
        pMap = &m_mapSpecial;
        break;
    case eDAILY_MISSION_TYPE_GUERRILLA:
        pMap = &m_mapGuerrilla;
        break;
    case eDAILY_MISSION_TYPE_EVENT:
        pMap = &m_mapEvent;
        break;
    default:
        return std::tr1::shared_ptr<CDailyMissionInfo>();
    }

    auto it = pMap->find(dwMissionID);
    if (it != pMap->end())
    {
        return it->second;
    }

    return std::tr1::shared_ptr<CDailyMissionInfo>();
}

// IDA: ?FindMission@CGocDailyMission@@QEAA?AV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@KE@Z (0x14004EE60)
// Find mission by ID, searching all types if byType is 0
std::tr1::shared_ptr<CDailyMissionInfo> CGocDailyMission::FindMission(std::uint32_t dwMissionID, std::uint8_t byType)
{
    if (byType != 0)
    {
        return GetMissionInfo(dwMissionID, byType);
    }

    // Search all mission types
    for (std::uint8_t byMissionType = eDAILY_MISSION_TYPE_SPECIAL;
         byMissionType <= eDAILY_MISSION_TYPE_EVENT;
         ++byMissionType)
    {
        auto pMission = FindMission(dwMissionID, byMissionType);
        if (pMission && pMission->GetDailyMissionType())
        {
            return pMission;
        }
    }

    return std::tr1::shared_ptr<CDailyMissionInfo>();
}

// IDA: ?AddDailyMission@CGocDailyMission@@QEAAXAEAUST_DAILY_MISSION_INFO@@@Z (0x14004F180)
// Add a daily mission to the appropriate map based on mission type
void CGocDailyMission::AddDailyMission(ST_DAILY_MISSION_INFO& stMission)
{
    // Get the mission table data
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, stMission.dwMissionID);

    if (!pTB_Mission)
    {
        return;
    }

    std::map<std::uint32_t, std::tr1::shared_ptr<CDailyMissionInfo>>* pMap = nullptr;

    switch (pTB_Mission->Mission_Type)
    {
    case eDAILY_MISSION_TYPE_SPECIAL:
        pMap = &m_mapSpecial;
        break;
    case eDAILY_MISSION_TYPE_GUERRILLA:
        pMap = &m_mapGuerrilla;
        break;
    case eDAILY_MISSION_TYPE_EVENT:
        pMap = &m_mapEvent;
        break;
    default:
        // Log error - invalid mission type
        return;
    }

    // Check if mission already exists
    if (pMap->find(stMission.dwMissionID) != pMap->end())
    {
        return;  // Mission already added
    }

    // Create new CDailyMissionInfo
    CDailyMissionInfo* pMissionInfo = new CDailyMissionInfo(pTB_Mission->Mission_ID, pTB_Mission);
    if (!pMissionInfo)
    {
        return;
    }

    std::tr1::shared_ptr<CDailyMissionInfo> pMission(pMissionInfo);
    pMission->InitAddMission(&stMission);

    // Add to appropriate map
    (*pMap)[stMission.dwMissionID] = pMission;

    // Update helper count if mission is in accept state
    if (pMission->GetDailyMissionState() == eDAILY_MISSION_STATE_ACCEPT)
    {
        if (pMission->GetDailyMissionHelper())
        {
            ++m_nHelperCount;
        }
    }
    else
    {
        pMission->SetDailyMissionHelper(false);
    }
}

// IDA: ?AcceptDailyMission@CGocDailyMission@@QEAA_NK@Z (0x140050610)
// Accept a daily mission by mission ID
bool CGocDailyMission::AcceptDailyMission(std::uint32_t dwMissionID)
{
    // Find the mission
    auto pMission = FindMission(dwMissionID, 0);
    if (!pMission)
    {
        // Mission not found - send error message
        return false;
    }

    // Check mission state
    std::uint8_t byState = pMission->GetDailyMissionState();
    if (byState == eDAILY_MISSION_STATE_ACCEPT)
    {
        // Already accepted
        return false;
    }

    if (byState == eDAILY_MISSION_STATE_COMPLETE || byState == eDAILY_MISSION_STATE_REWARD)
    {
        // Already completed or rewarded
        return false;
    }

    // Get mission table data
    TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
    if (!pTB_Mission)
    {
        return false;
    }

    // Check mission time for guerrilla and event missions
    if (pTB_Mission->Mission_Type == eDAILY_MISSION_TYPE_GUERRILLA ||
        pTB_Mission->Mission_Type == eDAILY_MISSION_TYPE_EVENT)
    {
        if (!CheckDailyMissionTime(pMission->GetMissionInfo()))
        {
            return false;
        }
    }

    // Initialize accept mission
    pMission->InitAcceptMission();

    // Set helper flag if within limit
    if (m_nHelperCount < 4)
    {
        pMission->SetDailyMissionHelper(true);
        ++m_nHelperCount;
    }

    // Update database
    PS_DAILY_MISSION_UPDATE psUpdate;
    psUpdate.vecInfo.push_back(*pMission->GetMissionInfo());
    DBUpdateMissionInfo(&psUpdate);

    // Send packet to client
    // TODO: Implement SendDailyMissionList packet sending

    return true;
}

// IDA: ?CompleteDailyMission@CGocDailyMission@@QEAA_NKV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@@Z (0x140054960)
// Complete a daily mission
bool CGocDailyMission::CompleteDailyMission(std::uint32_t dwMissionID, std::tr1::shared_ptr<CDailyMissionInfo> pMission)
{
    if (!pMission)
    {
        return false;
    }

    // Check if mission can be rewarded
    if (!CheckDailyMissionReward(dwMissionID))
    {
        return false;
    }

    // Set mission state to reward
    pMission->SetDailyMissionState(eDAILY_MISSION_STATE_REWARD);

    // Update helper count
    if (pMission->GetDailyMissionHelper())
    {
        pMission->SetDailyMissionHelper(false);
        --m_nHelperCount;
    }

    // TODO: Update achieve component
    // TODO: Update quest component

    // Update database
    DBDailyMissionPost(dwMissionID);

    return true;
}

// IDA: ?UpdateKillType@CGocDailyMission@@QEAAXKK@Z (0x1400539D0)
// Update kill-type daily missions
void CGocDailyMission::UpdateKillType(std::uint32_t dwObjectID, std::uint32_t dwCondition)
{
    // Get all kill-type missions
    std::vector<std::tr1::shared_ptr<CDailyMissionInfo>> vecList;
    GetDailyMissionList(eDAILY_MISSION_FINISH_KILL, &vecList);

    PS_DAILY_MISSION_UPDATE psUpdate;

    for (auto& pMission : vecList)
    {
        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission)
        {
            continue;
        }

        // Check if mission is in accept state
        if (pMission->GetDailyMissionState() != eDAILY_MISSION_STATE_ACCEPT)
        {
            continue;
        }

        // Check mission time
        if (!CheckDailyMissionTime(pMission->GetMissionInfo()))
        {
            continue;
        }

        // Check if this kill matches the mission condition
        if (!CheckUpdateKillType(pMission->GetMissionInfo()->dwMissionID, dwObjectID, dwCondition))
        {
            continue;
        }

        // Increment condition value
        pMission->AddConditionValue(1);

        // Check if mission is complete
        std::int16_t shConditionValue = pMission->GetConditionValue();
        if (pTB_Mission->Mission_Count > shConditionValue)
        {
            // Not complete yet - add to update list
            psUpdate.vecInfo.push_back(*pMission->GetMissionInfo());
        }
        else
        {
            // Mission complete
            if (CompleteDailyMission(pTB_Mission->Mission_ID, pMission))
            {
                psUpdate.vecInfo.push_back(*pMission->GetMissionInfo());
            }
        }
    }

    // Send updates if any
    if (!psUpdate.vecInfo.empty())
    {
        DBUpdateMissionInfo(&psUpdate);
        SendDailyMissionUpdateList(&psUpdate);
    }
}

// IDA: ?UpdateMazeClearType@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_TARGET@@KFK_N@Z (0x140053D00)
// Update maze-clear-type daily missions
void CGocDailyMission::UpdateMazeClearType(E_DAILY_MISSION_TARGET eTarget, std::uint32_t dwObjectID,
                                           std::int16_t shRank, std::uint32_t dwTime, bool bPartyWith)
{
    // Get all maze-clear-type missions
    std::vector<std::tr1::shared_ptr<CDailyMissionInfo>> vecList;
    GetDailyMissionList(eDAILY_MISSION_FINISH_MAZE_CLEAR, &vecList);

    if (bPartyWith)
    {
        // Also include party maze clear missions
        GetDailyMissionList(eDAILY_MISSION_FINISH_PARTY_MAZE_CLEAR, &vecList);
    }

    PS_DAILY_MISSION_UPDATE psUpdate;

    for (auto& pMission : vecList)
    {
        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission)
        {
            continue;
        }

        // Check if mission is in accept state and target type matches
        if (pMission->GetDailyMissionState() != eDAILY_MISSION_STATE_ACCEPT ||
            pTB_Mission->Target_Type != eTarget)
        {
            continue;
        }

        // Check mission time
        if (!CheckDailyMissionTime(pMission->GetMissionInfo()))
        {
            continue;
        }

        // Check if this maze clear matches the mission condition
        if (!CheckUpdateMazeClearType(pMission->GetMissionInfo()->dwMissionID, dwObjectID, shRank, dwTime))
        {
            continue;
        }

        // Increment condition value
        pMission->AddConditionValue(1);

        // Check if mission is complete
        std::int16_t shConditionValue = pMission->GetConditionValue();
        if (pTB_Mission->Mission_Count > shConditionValue)
        {
            // Not complete yet - add to update list
            psUpdate.vecInfo.push_back(*pMission->GetMissionInfo());
        }
        else
        {
            // Mission complete
            if (CompleteDailyMission(pTB_Mission->Mission_ID, pMission))
            {
                psUpdate.vecInfo.push_back(*pMission->GetMissionInfo());
            }
        }
    }

    // Send updates if any
    if (!psUpdate.vecInfo.empty())
    {
        DBUpdateMissionInfo(&psUpdate);
        SendDailyMissionUpdateList(&psUpdate);
    }
}

// IDA: ?SendDailyMissionList@CGocDailyMission@@QEAAXE@Z (0x140055780)
// Send daily mission list to client
void CGocDailyMission::SendDailyMissionList(std::uint8_t byTodayInit)
{
    PS_MAP_DISTRICT_DAILY_MISSION psMission;
    GetDailyMissionList(&psMission);

    // TODO: Implement XSendPacket and send to client
}

// IDA: ?SetNewDailyMissionList@CGocDailyMission@@QEAAXAEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x14004EF80)
// Set new daily mission list
void CGocDailyMission::SetNewDailyMissionList(std::vector<std::uint32_t>& vecNewMission)
{
    Clear();

    // TODO: Get current time and add each mission
    for (auto& dwMissionID : vecNewMission)
    {
        // AddNewDailyMission(dwMissionID, tCurr);
    }
}

// IDA: ?GetDailyMissionList@CGocDailyMission@@QEAAXAEAUPS_MAP_DISTRICT_DAILY_MISSION@@@Z (0x140050BF0)
// Get all daily missions grouped by district
void CGocDailyMission::GetDailyMissionList(PS_MAP_DISTRICT_DAILY_MISSION* psMissionList)
{
    if (!psMissionList)
    {
        return;
    }

    // Iterate through all mission maps and add to district map
    // TODO: Implement full logic from IDA
}

// IDA: ?UpdateFriendType@CGocDailyMission@@QEAAXAEAV?$vector@UST_DAILY_MISSION_FRIEND_RES@@V?$allocator@UST_DAILY_MISSION_FRIEND_RES@@@std@@@std@@@Z (0x140054080)
// Update friend-type daily missions
void CGocDailyMission::UpdateFriendType(std::vector<ST_DAILY_MISSION_INFO>& vecMission)
{
    PS_DAILY_MISSION_UPDATE psUpdate;

    for (auto& stMission : vecMission)
    {
        auto pMission = FindMission(stMission.dwMissionID, 0);
        if (!pMission)
        {
            continue;
        }

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission)
        {
            continue;
        }

        // Check if mission is in accept state and target type is friend
        if (pMission->GetDailyMissionState() != eDAILY_MISSION_STATE_ACCEPT ||
            pTB_Mission->Target_Type != eDAILY_MISSION_TARGET_FRIEND)
        {
            continue;
        }

        // Check mission time
        if (!CheckDailyMissionTime(pMission->GetMissionInfo()))
        {
            continue;
        }

        // Check friend condition
        // TODO: Implement friend check logic

        // Increment condition value
        pMission->AddConditionValue(1);

        // Check if mission is complete
        std::int16_t shConditionValue = pMission->GetConditionValue();
        if (pTB_Mission->Mission_Count > shConditionValue)
        {
            psUpdate.vecInfo.push_back(*pMission->GetMissionInfo());
        }
        else
        {
            if (CompleteDailyMission(pTB_Mission->Mission_ID, pMission))
            {
                psUpdate.vecInfo.push_back(*pMission->GetMissionInfo());
            }
        }
    }

    // Send updates if any
    if (!psUpdate.vecInfo.empty())
    {
        DBUpdateMissionInfo(&psUpdate);
        SendDailyMissionUpdateList(&psUpdate);
    }
}

// Helper method implementations (stubs)
bool CGocDailyMission::CheckDailyMissionTime(ST_DAILY_MISSION_INFO* stInfo)
{
    // TODO: Implement time check logic from IDA
    return true;
}

bool CGocDailyMission::CheckDailyMissionReward(std::uint32_t dwMissionID)
{
    // TODO: Implement reward check logic from IDA
    return true;
}

bool CGocDailyMission::CheckUpdateKillType(std::uint32_t dwMissionID, std::uint32_t dwObjectID, std::uint32_t dwCondition)
{
    // TODO: Implement kill type check logic from IDA
    return true;
}

bool CGocDailyMission::CheckUpdateMazeClearType(std::uint32_t dwMissionID, std::uint32_t dwObjectID,
                                                std::int16_t shRank, std::uint32_t dwTime)
{
    // TODO: Implement maze clear type check logic from IDA
    return true;
}

void CGocDailyMission::DBUpdateMissionInfo(PS_DAILY_MISSION_UPDATE* psUpdate)
{
    // TODO: Implement database update logic
}

void CGocDailyMission::DBDailyMissionPost(std::uint32_t dwMissionID)
{
    // TODO: Implement database post logic
}

void CGocDailyMission::SendDailyMissionUpdateList(PS_DAILY_MISSION_UPDATE* psUpdate)
{
    // TODO: Implement packet sending logic
}

std::uint32_t CGocDailyMission::CalculateMissionRemainTime(void* tNow, void* tStart)
{
    // TODO: Implement time calculation logic from IDA
    return 0;
}

std::uint32_t CGocDailyMission::CalculateGetMissionDurationTime(void* tNow, void* tStart, void* tEnd)
{
    // TODO: Implement duration calculation logic from IDA
    return 0;
}

void CGocDailyMission::AddNewDailyMission(std::uint32_t dwMissionID, void* tCurr)
{
    // TODO: Implement new mission addition logic from IDA
}

void CGocDailyMission::GetDailyMissionList(E_DAILY_MISSION_FINISH_TYPE eType,
                                           std::vector<std::tr1::shared_ptr<CDailyMissionInfo>>* vecList)
{
    if (!vecList)
    {
        return;
    }

    // Get missions from all maps that match the finish type
    // TODO: Implement full logic from IDA

    // Check special missions
    for (auto& pair : m_mapSpecial)
    {
        TB_DAILY_MISSION* pTB_Mission = pair.second->GetTBMission();
        if (pTB_Mission && pTB_Mission->Finish_Type == eType)
        {
            vecList->push_back(pair.second);
        }
    }

    // Check guerrilla missions
    for (auto& pair : m_mapGuerrilla)
    {
        TB_DAILY_MISSION* pTB_Mission = pair.second->GetTBMission();
        if (pTB_Mission && pTB_Mission->Finish_Type == eType)
        {
            vecList->push_back(pair.second);
        }
    }

    // Check event missions
    for (auto& pair : m_mapEvent)
    {
        TB_DAILY_MISSION* pTB_Mission = pair.second->GetTBMission();
        if (pTB_Mission && pTB_Mission->Finish_Type == eType)
        {
            vecList->push_back(pair.second);
        }
    }
}
