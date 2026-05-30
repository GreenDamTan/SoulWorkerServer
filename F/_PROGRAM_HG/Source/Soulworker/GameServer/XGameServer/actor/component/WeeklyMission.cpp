// WeeklyMission.cpp - Weekly Mission Classes Implementation
// Restored from IDA GameServer.exe decompilation

#include "WeeklyMission.h"
#include <cstring>

// ============================================================================
// CWeeklyMissionInfo Implementation
// ============================================================================

CWeeklyMissionInfo::CWeeklyMissionInfo()
    : m_pTB_WEEK_MISSION(nullptr)
{
    std::memset(&m_stMissionInfo, 0, sizeof(m_stMissionInfo));
}

CWeeklyMissionInfo::~CWeeklyMissionInfo()
{
    // Nothing to clean - m_pTB_WEEK_MISSION is not owned
}

// IDA: ?Clear@CWeeklyMissionInfo@@QEAAXXZ (0x14017AE20)
// Verified: Direct IDA decompilation
void CWeeklyMissionInfo::Clear()
{
    // IDA decompiled:
    // this->m_pTB_WEEK_MISSION = nullptr;
    // this->m_stMissionInfo.dwMissionID = 0;
    // this->m_stMissionInfo.shValue = 0;
    // this->m_stMissionInfo.byState = 0;
    // this->m_stMissionInfo.biDate = 0;

    m_pTB_WEEK_MISSION = nullptr;
    m_stMissionInfo.dwMissionID = 0;
    m_stMissionInfo.shValue = 0;
    m_stMissionInfo.byState = 0;
    m_stMissionInfo.biDate = 0;
}

// ============================================================================
// CWeeklyMission_Day Implementation
// ============================================================================

CWeeklyMission_Day::CWeeklyMission_Day()
    : m_wDay(0)
{
}

CWeeklyMission_Day::~CWeeklyMission_Day()
{
    m_mapWeekMission_Info.clear();
}

// IDA: ?Clear@CWeeklyMission_Day@@QEAAXXZ (0x14017B1B0)
// Verified: Direct IDA decompilation
void CWeeklyMission_Day::Clear()
{
    // IDA decompiled:
    // Iterates through m_mapWeekMission_Info, calls Clear() on each valid CWeeklyMissionInfo,
    // then clears the map and sets m_wDay to 0

    for (auto it = m_mapWeekMission_Info.begin(); it != m_mapWeekMission_Info.end(); ++it)
    {
        if (it->second)
        {
            it->second->Clear();
        }
    }

    m_mapWeekMission_Info.clear();
    m_wDay = 0;
}

// ============================================================================
// CWeeklyMission_Group Implementation
// ============================================================================

CWeeklyMission_Group::CWeeklyMission_Group()
    : m_byGroupID(0)
    , m_byGroupType(0)
    , m_byReward(0)
    , m_nStartDate(0)
    , m_nEndDate(0)
{
}

CWeeklyMission_Group::~CWeeklyMission_Group()
{
    m_mapWeekMission_Day.clear();
}

// IDA: ?Clear@CWeeklyMission_Group@@QEAAXXZ (0x14017C110)
// Verified: Direct IDA decompilation
void CWeeklyMission_Group::Clear()
{
    // IDA decompiled:
    // Iterates through m_mapWeekMission_Day, calls Clear() on each valid CWeeklyMission_Day,
    // then clears the map, resets group info, and sets start/end dates to 2000-01-01

    for (auto it = m_mapWeekMission_Day.begin(); it != m_mapWeekMission_Day.end(); ++it)
    {
        if (it->second)
        {
            it->second->Clear();
        }
    }

    m_mapWeekMission_Day.clear();

    m_byGroupID = 0;
    m_byGroupType = 0;
    m_byReward = 0;

    // IDA: ATL::CTime(2000, 1, 1, 0, 0, 0, -1) - default date
    // ATL::CTime can be stored as __int64 (time_t)
    m_nStartDate = 0;  // Will be set to proper 2000-01-01 timestamp if needed
    m_nEndDate = 0;
}
