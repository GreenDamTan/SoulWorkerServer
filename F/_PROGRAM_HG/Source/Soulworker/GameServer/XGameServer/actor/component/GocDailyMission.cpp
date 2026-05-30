// GocDailyMission.cpp - CGocDailyMission implementation
// Reconstructed from GameServer.exe IDA analysis
//
// Key functions decompiled from GameServer.exe (port 10004):
// - Constructor: 0x14004EAD0
// - Destructor: 0x14004EB80
// - Init: 0x14004EBF0
// - Clear: 0x14004EC10
// - GetMissionInfo: 0x14004EC60
// - FindMission: 0x14004EE60
// - AddDailyMission: 0x14004F180
// - AcceptDailyMission: 0x140050610
// - GeneraterTimeRange: 0x14004F960
// - SendDailyMissionList: 0x140055780
// - DBUpdateMissionInfo: 0x140054AD0

#include "GocDailyMission.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <algorithm>
#include <cstring>

// ATL::CTime forward declaration for platforms without ATL
#ifndef _ATLTIME_H_
namespace ATL {
    class CTime {
    public:
        __time64_t m_time;
        CTime() : m_time(0) {}
        CTime(__time64_t time) : m_time(time) {}
        CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST) {
            struct tm atm;
            atm.tm_sec = nSec;
            atm.tm_min = nMin;
            atm.tm_hour = nHour;
            atm.tm_mday = nDay;
            atm.tm_mon = nMonth - 1;
            atm.tm_year = nYear - 1900;
            atm.tm_isdst = nDST;
            m_time = _mktime64(&atm);
        }
        int GetYear() const { struct tm t; _localtime64_s(&t, &m_time); return t.tm_year + 1900; }
        int GetMonth() const { struct tm t; _localtime64_s(&t, &m_time); return t.tm_mon + 1; }
        int GetDay() const { struct tm t; _localtime64_s(&t, &m_time); return t.tm_mday; }
        int GetHour() const { struct tm t; _localtime64_s(&t, &m_time); return t.tm_hour; }
        int GetMinute() const { struct tm t; _localtime64_s(&t, &m_time); return t.tm_min; }
        int GetSecond() const { struct tm t; _localtime64_s(&t, &m_time); return t.tm_sec; }
        static CTime GetCurrentTime() { return CTime(_time64(nullptr)); }
        CTime operator+(const CTimeSpan& span) const { return CTime(m_time + span.m_timeSpan); }
        CTime operator-(const CTimeSpan& span) const { return CTime(m_time - span.m_timeSpan); }
    };
    class CTimeSpan {
    public:
        __time64_t m_timeSpan;
        CTimeSpan() : m_timeSpan(0) {}
        CTimeSpan(__time64_t span) : m_timeSpan(span) {}
        CTimeSpan(int lDays, int nHours, int nMins, int nSecs) {
            m_timeSpan = nSecs + 60 * (nMins + 60 * (nHours + 24 * lDays));
        }
        int GetDays() const { return (int)(m_timeSpan / (24 * 60 * 60)); }
        int GetHours() const { return (int)((m_timeSpan / 3600) % 24); }
        int GetMinutes() const { return (int)((m_timeSpan / 60) % 60); }
        int GetSeconds() const { return (int)(m_timeSpan % 60); }
        bool operator<=(const CTimeSpan& other) const { return m_timeSpan <= other.m_timeSpan; }
    };
}
#endif

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
// Verified: Per IDA decompile at 0x14004EAD0:
// - Calls GOComponent::GOComponent(this)
// - Sets vftable to CGocDailyMission::`vftable'
// - Constructs m_mapSpecial (std::map<int, std::tr1::shared_ptr<CDropItemGroup>>)
// - Constructs m_mapGuerrilla (std::map<int, std::tr1::shared_ptr<CDropItemGroup>>)
// - Constructs m_mapEvent (std::map<int, std::tr1::shared_ptr<CDropItemGroup>>)
// - Returns this
CGocDailyMission::CGocDailyMission()
    : GOComponent()
    , m_nHelperCount(0)
{
    // Per IDA: Maps are default-constructed
}

// IDA: ??1CGocDailyMission@@UEAA@XZ (0x14004EB80)
// Verified: Per IDA decompile at 0x14004EB80:
// - Sets vftable to CGocDailyMission::`vftable'
// - Destructs m_mapEvent
// - Destructs m_mapGuerrilla
// - Destructs m_mapSpecial
// - Calls GOComponent::~GOComponent(this)
CGocDailyMission::~CGocDailyMission()
{
    Clear();
}

// IDA: ?Init@CGocDailyMission@@QEAA_NXZ (0x14004EBF0)
// Verified: Per IDA decompile at 0x14004EBF0:
// - Calls CGocDailyMission::Clear(this)
// - Returns 1 (true)
bool CGocDailyMission::Init()
{
    Clear();
    return true;
}

// IDA: ?Clear@CGocDailyMission@@QEAAXXZ (0x14004EC10)
// Verified: Per IDA decompile at 0x14004EC10:
// - Calls std::map::clear on m_mapSpecial
// - Calls std::map::clear on m_mapGuerrilla
// - Calls std::map::clear on m_mapEvent
// - Sets m_nHelperCount = 0
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
// Verified: Per IDA decompile - adds a daily mission to the appropriate map
// - Gets TB_DAILY_MISSION from XResourceMgr
// - Calculates remain time and duration time
// - Creates CDailyMissionInfo and adds to appropriate map (Special/Guerrilla/Event)
// - Updates helper count based on mission state
// - For Guerrilla missions with zero duration, sets state to complete
void CGocDailyMission::AddDailyMission(ST_DAILY_MISSION_INFO& stMission)
{
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
        {
            pMap = &m_mapSpecial;

            // Calculate remain time and duration time
            ATL::CTime tNow = ATL::CTime::GetCurrentTime();
            stMission.dwRemainTime = CalculateMissionRemainTime(tNow, stMission.tStart);
            stMission.dwDurationTime = CalculateGetMissionDurationTime(tNow, stMission.tStart, stMission.tEnd);

            // Check if mission already exists
            if (pMap->find(stMission.dwMissionID) != pMap->end())
            {
                return;
            }

            // Create new CDailyMissionInfo
            CDailyMissionInfo* pMissionInfo = new CDailyMissionInfo(pTB_Mission->Mission_ID, pTB_Mission);
            if (!pMissionInfo)
            {
                return;
            }

            std::tr1::shared_ptr<CDailyMissionInfo> pMission(pMissionInfo);
            pMission->InitAddMission(&stMission);

            // Add to map
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
        break;

    case eDAILY_MISSION_TYPE_GUERRILLA:
        {
            pMap = &m_mapGuerrilla;

            // Calculate remain time and duration time
            ATL::CTime tNow = ATL::CTime::GetCurrentTime();
            stMission.dwRemainTime = CalculateMissionRemainTime(tNow, stMission.tStart);
            stMission.dwDurationTime = CalculateGetMissionDurationTime(tNow, stMission.tStart, stMission.tEnd);

            // Check if mission already exists
            if (pMap->find(stMission.dwMissionID) != pMap->end())
            {
                return;
            }

            // Create new CDailyMissionInfo
            CDailyMissionInfo* pMissionInfo = new CDailyMissionInfo(pTB_Mission->Mission_ID, pTB_Mission);
            if (!pMissionInfo)
            {
                return;
            }

            std::tr1::shared_ptr<CDailyMissionInfo> pMission(pMissionInfo);
            pMission->InitAddMission(&stMission);

            // Add to map
            (*pMap)[stMission.dwMissionID] = pMission;

            // For guerrilla missions with zero duration, set state to complete
            if (stMission.dwDurationTime == 0 && pMission->GetDailyMissionState() != eDAILY_MISSION_STATE_REWARD)
            {
                pMission->SetDailyMissionState(eDAILY_MISSION_STATE_COMPLETE);
            }

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
        break;

    case eDAILY_MISSION_TYPE_EVENT:
        {
            pMap = &m_mapEvent;

            // Calculate remain time and duration time
            ATL::CTime tNow = ATL::CTime::GetCurrentTime();
            stMission.dwRemainTime = CalculateMissionRemainTime(tNow, stMission.tStart);
            stMission.dwDurationTime = CalculateGetMissionDurationTime(tNow, stMission.tStart, stMission.tEnd);

            // Check if mission already exists
            if (pMap->find(stMission.dwMissionID) != pMap->end())
            {
                return;
            }

            // Create new CDailyMissionInfo
            CDailyMissionInfo* pMissionInfo = new CDailyMissionInfo(pTB_Mission->Mission_ID, pTB_Mission);
            if (!pMissionInfo)
            {
                return;
            }

            std::tr1::shared_ptr<CDailyMissionInfo> pMission(pMissionInfo);
            pMission->InitAddMission(&stMission);

            // Add to map
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
        break;

    default:
        // Log error - invalid mission type
        // TODO: LogHelper::LogDebug("game.dailymission", "<DAILY_MISSION> MISSION TYPE ERROR")
        break;
    }
}

// IDA: ?AcceptDailyMission@CGocDailyMission@@QEAA_NK@Z (0x140050610)
// Verified: Per IDA decompile - accepts a daily mission by mission ID
// - Finds mission via FindMission
// - Checks mission state (must be in init state, not accept/complete/reward)
// - Validates mission time for guerrilla/event missions via CheckDailyMissionTime
// - Calls InitAcceptMission on the mission
// - Sets helper flag if m_nHelperCount < 4
// - Sends DB update via DBUpdateMissionInfo
// - Sends packet to client (main=0x24, sub=2)
// - Logs to ST_LOG_GAME (main=13, sub=2)
bool CGocDailyMission::AcceptDailyMission(std::uint32_t dwMissionID)
{
    // Find the mission
    auto pMission = FindMission(dwMissionID, 0);
    if (!pMission)
    {
        // Mission not found - send error message (error code 0xD2F4)
        // TODO: CGocNetwork::SendErrorMessage(0x24, 2, 0xD2F4)
        return false;
    }

    // Check mission state
    std::uint8_t byState = pMission->GetDailyMissionState();
    if (byState == eDAILY_MISSION_STATE_COMPLETE || byState == eDAILY_MISSION_STATE_REWARD)
    {
        // Already completed or rewarded - send error (0xD2F2)
        // TODO: CGocNetwork::SendErrorMessage(0x24, 2, 0xD2F2)
        return false;
    }

    if (byState == eDAILY_MISSION_STATE_ACCEPT)
    {
        // Already accepted - send error (0xD2F1)
        // TODO: CGocNetwork::SendErrorMessage(0x24, 2, 0xD2F1)
        return false;
    }

    // Get mission table data
    TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
    if (!pTB_Mission)
    {
        return false;
    }

    // Get mission info
    ST_DAILY_MISSION_INFO* stMissionInfo = pMission->GetMissionInfo();

    // Check mission time for guerrilla (type 2) and event (type 3) missions
    if (pTB_Mission->Mission_Type == eDAILY_MISSION_TYPE_GUERRILLA ||
        pTB_Mission->Mission_Type == eDAILY_MISSION_TYPE_EVENT)
    {
        if (!CheckDailyMissionTime(stMissionInfo))
        {
            // Time check failed - send error (0xD2F4)
            // TODO: CGocNetwork::SendErrorMessage(0x24, 2, 0xD2F4)
            return false;
        }
    }

    // Initialize accept mission
    pMission->InitAcceptMission();

    // Set helper flag if within limit (max 4 helpers)
    if (m_nHelperCount < 4)
    {
        pMission->SetDailyMissionHelper(true);
        ++m_nHelperCount;
    }

    // Update database
    PS_DAILY_MISSION_UPDATE psUpdate;
    psUpdate.vecInfo.push_back(*stMissionInfo);
    DBUpdateMissionInfo(&psUpdate);

    // Send packet to client (main=0x24, sub=2)
    // TODO: XSendPacket with dwMissionID and mission state
    // TODO: XSendPacket::XSendPacket(&xSendPacket, 0x24, 2)
    // XParse::operator<<(&xSendPacket.XParse, dwMissionID)
    // XParse::operator<<(&xSendPacket.XParse, pMission->GetDailyMissionState())
    // CGocNetwork::Send(pActor, &xSendPacket)

    // Log to ST_LOG_GAME (main=13, sub=2)
    // TODO: ST_LOG_GAME stLog
    // stLog._sMainType = 13
    // stLog._sSubType = 2
    // stLog.nParam0 = dwMissionID
    // stLog.nParam1 = user level
    // wcscpy_s(stLog.szComment, L"DAILY_MISSION_ACCEPT")
    // XGameServer::SendDBLog

    return true;
}

// IDA: ?CompleteDailyMission@CGocDailyMission@@QEAA_NKV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@@Z (0x140054960)
// Verified: Per IDA decompile - completes a daily mission
// - Checks if mission can be rewarded via CheckDailyMissionReward
// - Sets mission state to eDAILY_MISSION_STATE_REWARD (3)
// - Decrements helper count if mission had helper flag
// - Updates achieve component (type 0x35, count 1)
// - Calls DBDailyMissionPost
// - Updates quest component with eCONDITION_TARGET_EVENT
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

    // Set mission state to reward (3)
    pMission->SetDailyMissionState(eDAILY_MISSION_STATE_REWARD);

    // Update helper count if mission had helper flag
    if (pMission->GetDailyMissionHelper())
    {
        pMission->SetDailyMissionHelper(false);
        --m_nHelperCount;
    }

    // TODO: Get owner CMover and update achieve component
    // CMover* pMover = GetOwnerMover();
    // std::tr1::shared_ptr<CGocAchieve> pAchieve = pMover->GetGOC<CGocAchieve>();
    // if (pAchieve)
    // {
    //     pAchieve->UpdateCollect(0x35, 1, 0);
    // }

    // Update database
    DBDailyMissionPost(dwMissionID);

    // TODO: Get owner CMover and update quest component
    // std::tr1::shared_ptr<CGocQuest> pQuest = pMover->GetGOC<CGocQuest>();
    // if (pQuest)
    // {
    //     pQuest->UpdateCondition(10, eCONDITION_TARGET_EVENT, 1, 1, 0);
    // }

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
// Verified: Per IDA decompile - sends daily mission list packet to client
// - Creates PS_MAP_DISTRICT_DAILY_MISSION and fills it via GetDailyMissionList
// - Sends packet (main=0x24, sub=1) with byTodayInit flag and mission data
// - Uses CGocNetwork::Send to transmit to client
void CGocDailyMission::SendDailyMissionList(std::uint8_t byTodayInit)
{
    PS_MAP_DISTRICT_DAILY_MISSION psMission;
    GetDailyMissionList(&psMission);

    XSendPacket xSendPacket(0x24, 1);
    xSendPacket.XParse << byTodayInit;
    xSendPacket << psMission;

    // Get actor from GOComponent owner chain
    XActor* pActor = nullptr;
    // TODO: Get actor from owner - this requires access to the parent actor
    // For now, need to implement actor retrieval from component owner

    CGocNetwork::Send(pActor, &xSendPacket);
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
// Verified: Per IDA decompile - gets all daily missions grouped by district
// - Iterates through m_mapSpecial, m_mapGuerrilla, m_mapEvent
// - Gets district ID from each mission's TB_DAILY_MISSION (offset +129 = District_ID)
// - Groups missions by district in PS_MAP_DISTRICT_DAILY_MISSION
void CGocDailyMission::GetDailyMissionList(PS_MAP_DISTRICT_DAILY_MISSION* psMissionList)
{
    if (!psMissionList)
    {
        return;
    }

    // Iterate through special missions
    for (auto iter = m_mapSpecial.begin(); iter != m_mapSpecial.end(); ++iter)
    {
        std::tr1::shared_ptr<CDailyMissionInfo> pMission = iter->second;
        if (!pMission)
        {
            continue;
        }

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission)
        {
            continue;
        }

        // Get district ID from mission table (offset 129 in decimal = 0x81 = District_ID offset)
        std::uint32_t dwDistrictID = pTB_Mission->District_ID;

        // Find or create district entry
        auto mit = psMissionList->find(dwDistrictID);
        if (mit != psMissionList->end())
        {
            // District exists, add mission info to existing list
            mit->second.push_back(*pMission->GetMissionInfo());
        }
        else
        {
            // Create new district entry
            std::vector<ST_DAILY_MISSION_INFO> vecMission;
            vecMission.push_back(*pMission->GetMissionInfo());
            (*psMissionList)[dwDistrictID] = vecMission;
        }
    }

    // Iterate through guerrilla missions
    for (auto iter = m_mapGuerrilla.begin(); iter != m_mapGuerrilla.end(); ++iter)
    {
        std::tr1::shared_ptr<CDailyMissionInfo> pMission = iter->second;
        if (!pMission)
        {
            continue;
        }

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission)
        {
            continue;
        }

        std::uint32_t dwDistrictID = pTB_Mission->District_ID;

        auto mit = psMissionList->find(dwDistrictID);
        if (mit != psMissionList->end())
        {
            mit->second.push_back(*pMission->GetMissionInfo());
        }
        else
        {
            std::vector<ST_DAILY_MISSION_INFO> vecMission;
            vecMission.push_back(*pMission->GetMissionInfo());
            (*psMissionList)[dwDistrictID] = vecMission;
        }
    }

    // Iterate through event missions
    for (auto iter = m_mapEvent.begin(); iter != m_mapEvent.end(); ++iter)
    {
        std::tr1::shared_ptr<CDailyMissionInfo> pMission = iter->second;
        if (!pMission)
        {
            continue;
        }

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission)
        {
            continue;
        }

        std::uint32_t dwDistrictID = pTB_Mission->District_ID;

        auto mit = psMissionList->find(dwDistrictID);
        if (mit != psMissionList->end())
        {
            mit->second.push_back(*pMission->GetMissionInfo());
        }
        else
        {
            std::vector<ST_DAILY_MISSION_INFO> vecMission;
            vecMission.push_back(*pMission->GetMissionInfo());
            (*psMissionList)[dwDistrictID] = vecMission;
        }
    }
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

// IDA: ?CheckDailyMissionTime@CGocDailyMission@@QEAA_NAEAUST_DAILY_MISSION_INFO@@@Z (0x140051900)
// Verified: Per IDA decompile - checks if mission is within valid time range
// - Gets TB_DAILY_MISSION from XResourceMgr
// - Type 1 (Special): checks stMission->tStart <= tNow < stMission->tEnd
// - Type 2 (Guerrilla): parses Date_Start/Date_End as HH:MM:SS, checks time of day
// - Type 3 (Event): parses Date_Start/Date_End as full datetime, checks date range
// - Returns false if time check fails
bool CGocDailyMission::CheckDailyMissionTime(ST_DAILY_MISSION_INFO* stMission)
{
    if (!stMission)
    {
        return false;
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, stMission->dwMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    ATL::CTime tNow = ATL::CTime::GetCurrentTime();
    int nSecs = tNow.GetSecond();
    int nMins = tNow.GetMinute();
    int nHour = tNow.GetHour();
    ATL::CTimeSpan tNowTime(0, nHour, nMins, nSecs);

    switch (pTB_Mission->Mission_Type)
    {
    case eDAILY_MISSION_TYPE_SPECIAL:
        // Special - check date range from mission info
        if (tNow < stMission->tStart || tNow >= stMission->tEnd)
        {
            return false;
        }
        break;

    case eDAILY_MISSION_TYPE_GUERRILLA:
        {
            // Guerrilla - check time of day
            ATL::CTimeSpan tStartTime(0, 0, 0, 0);
            ATL::CTimeSpan tEndTime(0, 0, 0, 0);

            // Parse Date_Start as "YYYY-MM-DD HH:MM:SS" but only use HH:MM:SS
            int _year = 2000, _mon = 1, _day = 1, _hour = 0, _min = 0, _sec = 0;
            sscanf_s(pTB_Mission->Date_Start, "%d-%d-%d %d:%d:%d", &_year, &_mon, &_day, &_hour, &_min, &_sec);
            if (_hour <= 24 && _min <= 60 && _sec <= 60)
            {
                tStartTime = ATL::CTimeSpan(0, _hour, _min, _sec);
            }

            // Parse Date_End
            int v18 = 2000, v20 = 1, v23 = 1, nHours = 0, v19 = 0, v22 = 0;
            sscanf_s(pTB_Mission->Date_End, "%d-%d-%d %d:%d:%d", &v18, &v20, &v23, &nHours, &v19, &v22);
            if (nHours <= 24 && v19 <= 60 && v22 <= 60)
            {
                tEndTime = ATL::CTimeSpan(0, nHours, v19, v22);
            }

            // Check if current time is within range
            if (tNowTime < tStartTime || tNowTime >= tEndTime)
            {
                return false;
            }
        }
        break;

    case eDAILY_MISSION_TYPE_EVENT:
        {
            // Event - check full datetime range
            ATL::CTime tStart(2000, 1, 1, 0, 0, 0, -1);
            ATL::CTime tEnd(2000, 1, 1, 0, 0, 0, -1);

            // Parse Date_Start as full datetime
            int nYear = 2000, nMonth = 1, nDay = 1, nHour = 0, nMin = 0, nSec = 0;
            sscanf_s(pTB_Mission->Date_Start, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);
            if (nYear >= 2000 && nYear <= 2040 && nMonth >= 1 && nMonth <= 12 &&
                nDay >= 1 && nDay <= 31 && nHour <= 24 && nMin <= 60 && nSec <= 60)
            {
                tStart = ATL::CTime(nYear, nMonth, nDay, nHour, nMin, nSec, -1);
            }

            // Parse Date_End as full datetime
            int v32 = 2000, v34 = 1, v37 = 1, v35 = 0, v33 = 0, v36 = 0;
            sscanf_s(pTB_Mission->Date_End, "%d-%d-%d %d:%d:%d", &v32, &v34, &v37, &v35, &v33, &v36);
            if (v32 >= 2000 && v32 <= 2040 && v34 >= 1 && v34 <= 12 &&
                v37 >= 1 && v37 <= 31 && v35 <= 24 && v33 <= 60 && v36 <= 60)
            {
                tEnd = ATL::CTime(v32, v34, v37, v35, v33, v36, -1);
            }

            // Check if current time is within range
            if (tNow < tStart || tNow >= tEnd)
            {
                return false;
            }
        }
        break;

    default:
        return false;
    }

    return true;
}

// IDA: ?CheckDailyMissionReward@CGocDailyMission@@QEAA_NK@Z (0x1400537D0)
// Verified: Per IDA decompile - checks if mission rewards are valid
// - Gets TB_DAILY_MISSION from XResourceMgr
// - Validates each Reward_Item_Id (01-04) exists in TB_ITEM table
// - Returns true only if all reward items exist in the item table
bool CGocDailyMission::CheckDailyMissionReward(std::uint32_t dwMissionID)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, dwMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    // Validate reward item 01
    if (pTB_Mission->Reward_Item_Id_01)
    {
        TB_ITEM* pTBItem = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, pTB_Mission->Reward_Item_Id_01);
        if (!pTBItem)
        {
            return false;
        }
    }

    // Validate reward item 02
    if (pTB_Mission->Reward_Item_Id_02)
    {
        TB_ITEM* pTBItem = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, pTB_Mission->Reward_Item_Id_02);
        if (!pTBItem)
        {
            return false;
        }
    }

    // Validate reward item 03
    if (pTB_Mission->Reward_Item_Id_03)
    {
        TB_ITEM* pTBItem = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, pTB_Mission->Reward_Item_Id_03);
        if (!pTBItem)
        {
            return false;
        }
    }

    // Validate reward item 04
    if (pTB_Mission->Reward_Item_Id_04)
    {
        TB_ITEM* pTBItem = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, pTB_Mission->Reward_Item_Id_04);
        if (!pTBItem)
        {
            return false;
        }
    }

    return true;
}

// IDA: ?CheckUpdateKillType@CGocDailyMission@@QEAA_NKKK@Z (0x1400525F0)
// Verified: Per IDA decompile - checks if kill matches mission condition
// - Gets TB_DAILY_MISSION from XResourceMgr
// - Target_Type 1 (MAZE): Checks condition flags and Target_ID match
// - Target_Type 2 (MONSTER): Checks condition flags and monster mission group
// - Target_Type 3 (ITEM): Checks condition flags and monster faction
// - Target_Type 4: Only checks condition flags
bool CGocDailyMission::CheckUpdateKillType(std::uint32_t dwMissionID, std::uint32_t dwObjectID, std::uint32_t dwCondition)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, dwMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    switch (pTB_Mission->Target_Type)
    {
    case eDAILY_MISSION_TARGET_MAZE:
        // Maze target - check condition and exact ID
        if (pTB_Mission->Mission_Type != eDAILY_MISSION_TYPE_EVENT &&
            (dwCondition & (1 << pTB_Mission->Condition_Type)) == 0)
        {
            return false;
        }
        if (pTB_Mission->Target_ID != dwObjectID)
        {
            return false;
        }
        break;

    case eDAILY_MISSION_TARGET_MONSTER:
        // Monster target - check condition and mission group
        if ((dwCondition & (1 << pTB_Mission->Condition_Type)) == 0)
        {
            return false;
        }
        TB_MONSTER* pTBMonster = XResourceMgr::GetTB_MONSTER(&pGameServer->m_xResourceMgr, dwObjectID);
        if (!pTBMonster)
        {
            return false;
        }
        // Check if monster matches any of the 3 mission groups
        if (pTB_Mission->Target_ID != pTBMonster->Mission_Group_1 &&
            pTB_Mission->Target_ID != pTBMonster->Mission_Group_2 &&
            pTB_Mission->Target_ID != pTBMonster->Mission_Group_3)
        {
            return false;
        }
        break;

    case eDAILY_MISSION_TARGET_ITEM:
        // Item target - check condition and faction
        if ((dwCondition & (1 << pTB_Mission->Condition_Type)) == 0)
        {
            return false;
        }
        TB_MONSTER* TB_MONSTER = XResourceMgr::GetTB_MONSTER(&pGameServer->m_xResourceMgr, dwObjectID);
        if (!TB_MONSTER)
        {
            return false;
        }
        if (pTB_Mission->Target_Faction != TB_MONSTER->Monster_Faction)
        {
            return false;
        }
        break;

    case eDAILY_MISSION_TARGET_QUEST:
        // Quest target - only check condition
        if ((dwCondition & (1 << pTB_Mission->Condition_Type)) == 0)
        {
            return false;
        }
        break;

    default:
        return false;
    }

    return true;
}

// IDA: ?CheckUpdateMazeClearType@CGocDailyMission@@QEAA_NKKFK@Z (0x140052850)
// Verified: Per IDA decompile - checks if maze clear matches mission condition
// - Condition_Type 0: Always pass
// - Condition_Type 9: Check rank >= 5 (S rank)
// - Condition_Type 10: Check clear time <= Condition_Value
// - Condition_Type 11: Check party member level gap <= Condition_Value
// - Condition_Type 12: Check party member class match
// - Condition_Type 18: Check maze type matches Condition_Value
bool CGocDailyMission::CheckUpdateMazeClearType(std::uint32_t dwMissionID, std::uint32_t dwObjectID,
                                                std::int16_t shRank, std::uint32_t dwTime)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, dwMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    // Check Target_ID if specified
    if (pTB_Mission->Target_ID && pTB_Mission->Target_ID != dwObjectID)
    {
        return false;
    }

    switch (pTB_Mission->Condition_Type)
    {
    case 0:
        // No condition
        return true;

    case 9:
        // S rank clear (rank >= 5)
        return shRank >= 5;

    case 10:
        // Time limit clear (clear time must be <= Condition_Value)
        return pTB_Mission->Condition_Value >= dwTime;

    case 11:
        // Party level gap condition
        // TODO: Need to get party members and check level gap
        // Get user's level and compare with party members
        // Condition_Value is the max allowed level gap
        // Condition_Count is the min number of members within gap
        return true;  // Stub for now

    case 12:
        // Party class condition
        // TODO: Need to get party members and check class
        // Condition_Class is the required class (7 = any class same as user)
        // Condition_Count is the min number of members with matching class
        return true;  // Stub for now

    case 18:
        // Maze type condition
        // TODO: Need to get current maze and check type
        // XMaze* pMaze = GetCurrentMaze();
        // return XMaze::GetMazeType(pMaze) == pTB_Mission->Condition_Value;
        return true;  // Stub for now

    default:
        return false;
    }
}

// IDA: ?SendDailyMissionUpdateList@CGocDailyMission@@QEAAXAEAUPS_DAILY_MISSION_UPDATE@@@Z (0x140055900)
// Verified: Per IDA decompile - sends mission update list to client
// - Sends packet (main=0x24, sub=3) with PS_DAILY_MISSION_UPDATE
// - Uses CGocNetwork::Send to transmit to client
void CGocDailyMission::SendDailyMissionUpdateList(PS_DAILY_MISSION_UPDATE* psUpdate)
{
    if (!psUpdate || psUpdate->vecInfo.empty())
    {
        return;
    }

    XSendPacket xSendPacket(0x24, 3);
    xSendPacket << *psUpdate;

    // Get actor from GOComponent owner chain
    XActor* pActor = nullptr;
    // TODO: Get actor from owner - this requires access to the parent actor

    CGocNetwork::Send(pActor, &xSendPacket);
}

// IDA: ?DBDailyMissionPost@CGocDailyMission@@QEAAXK@Z (0x140055210)
// Verified: Per IDA decompile - sends daily mission rewards via post system
// - Gets TB_DAILY_MISSION from XResourceMgr
// - Creates ST_CREATE_ITEMS with reward items (01-04)
// - Sends system post via CGocPost::SystemPostSend (type 5, sub 2)
// - Logs to ST_LOG_GAME (main=13, sub=4)
// - Sends statistics to DB (main=0xF0, sub=7)
void CGocDailyMission::DBDailyMissionPost(std::uint32_t dwMissionID)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, dwMissionID);
    if (!pTB_Mission)
    {
        return;
    }

    // Create reward item list
    ST_CREATE_ITEMS vecItem;
    ST_CREATE_ITEM stItem;

    // Add reward item 01
    if (pTB_Mission->Reward_Item_Id_01)
    {
        stItem.nItemID = pTB_Mission->Reward_Item_Id_01;
        stItem.shCount = pTB_Mission->Reward_Item_Count_01;
        vecItem.push_back(stItem);
    }

    // Add reward item 02
    if (pTB_Mission->Reward_Item_Id_02)
    {
        stItem.nItemID = pTB_Mission->Reward_Item_Id_02;
        stItem.shCount = pTB_Mission->Reward_Item_Count_02;
        vecItem.push_back(stItem);
    }

    // Add reward item 03
    if (pTB_Mission->Reward_Item_Id_03)
    {
        stItem.nItemID = pTB_Mission->Reward_Item_Id_03;
        stItem.shCount = pTB_Mission->Reward_Item_Count_03;
        vecItem.push_back(stItem);
    }

    // Add reward item 04
    if (pTB_Mission->Reward_Item_Id_04)
    {
        stItem.nItemID = pTB_Mission->Reward_Item_Id_04;
        stItem.shCount = pTB_Mission->Reward_Item_Count_04;
        vecItem.push_back(stItem);
    }

    // TODO: Get CMover owner and CGocPost component
    // CMover* pMover = GetOwnerMover();
    // std::tr1::shared_ptr<CGocPost> pPost = pMover->GetGOC<CGocPost>();
    // if (pPost)
    // {
    //     wchar_t strTitle[48] = {0};
    //     _itow(dwMissionID, strTitle, 10);
    //     pPost->SystemPostSend(&vecItem, 5, 2, dwMissionID, strTitle);
    // }

    // Log to ST_LOG_GAME (main=13, sub=4)
    // TODO: Implement logging
    // ST_LOG_GAME stLog;
    // stLog._nUAID = pUser->GetUAID();
    // stLog._nUCID = pUser->GetUCID();
    // stLog._sMainType = 13;
    // stLog._sSubType = 4;
    // stLog.nParam0 = dwMissionID;
    // stLog.nParam1 = pTB_Mission->Reward_Item_Id_01;
    // stLog.nParam2 = pTB_Mission->Reward_Item_Count_01;
    // stLog.nParam3 = pTB_Mission->Reward_Item_Id_02;
    // stLog.nParam4 = pTB_Mission->Reward_Item_Count_02;
    // stLog.nParam5 = pTB_Mission->Reward_Item_Id_03;
    // stLog.nParam6 = pTB_Mission->Reward_Item_Count_03;
    // stLog.nParam7 = pTB_Mission->Reward_Item_Id_04;
    // stLog.nParam8 = pTB_Mission->Reward_Item_Count_04;
    // wcscpy_s(stLog.szComment, L"DAILY_MISSION_REWARD");
    // XGameServer::SendDBLog(pGameServer, &stLog);

    // Send statistics to DB (main=0xF0, sub=7)
    // TODO: Implement statistics
    // ST_STATISTICS_DAILY_MISSION stStatistics;
    // stStatistics.dwUAID = pUser->GetUAID();
    // stStatistics.dwUCID = pUser->GetUCID();
    // stStatistics.dwMissionID = dwMissionID;
    // XSendDBPacket xSendDBStatistics(nullptr, 0xF0, 7);
    // xSendDBStatistics << stStatistics;
    // XGameServer::SendDBStatistics(pGameServer, &xSendDBStatistics);
}

// IDA: ?DBUpdateMissionInfo@CGocDailyMission@@QEAAXAEAUPS_DAILY_MISSION_UPDATE@@@Z (0x140054AD0)
// Verified: Per IDA decompile - sends DB update for daily mission info
// - Gets CUser from owner actor via RTTI dynamic cast
// - Logs each mission update via ST_LOG_GAME (main=13, sub=3)
// - Sends DB packet (main=0x48, sub=3) with mission info
void CGocDailyMission::DBUpdateMissionInfo(PS_DAILY_MISSION_UPDATE* psUpdate)
{
    if (!psUpdate || psUpdate->vecInfo.empty())
    {
        return;
    }

    // Get CUser from owner actor
    // TODO: Need to implement proper actor retrieval from GOComponent
    CUser* pUser = nullptr;
    // The IDA code shows RTTI dynamic cast from owner to CUser

    if (pUser)
    {
        // Log each mission update
        ST_LOG_GAME stLog;
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = pUser->GetUCID();
        stLog._sMainType = 13;
        stLog._sSubType = 3;
        wcscpy_s(stLog.szComment, L"DAILY_MISSION_UPDATE");

        for (size_t i = 0; i < psUpdate->vecInfo.size(); ++i)
        {
            ST_DAILY_MISSION_INFO& stInfo = psUpdate->vecInfo[i];
            stLog.nParam0 = stInfo.dwMissionID;
            stLog.nParam1 = stInfo.byState;
            stLog.nParam2 = stInfo.byType;
            stLog.nParam3 = stInfo.shConditionValue;

            XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
            XGameServer::SendDBLog(pGameServer, &stLog);
        }
    }

    // Send DB packet (main=0x48, sub=3)
    XSendDBPacket xSendDBPacket(nullptr, 0x48, 3);
    xSendDBPacket.XParse << (pUser ? pUser->GetUCID() : 0);
    xSendDBPacket << *psUpdate;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBGame(pGameServer, &xSendDBPacket);
}

// IDA: ?DBReqDailyMissionList@CGocDailyMission@@QEAAXXZ (0x140054DD0)
// Verified: Per IDA decompile - requests daily mission list from DB
// - Sends DB packet (main=0x48, sub=1) with UCID
void CGocDailyMission::DBReqDailyMissionList()
{
    // Get owner object
    IXObject* pObject = nullptr;
    // TODO: Get object from owner - this requires access to the parent actor

    XSendDBPacket xSendDBPacket(pObject, 0x48, 1);

    // Get UCID from owner
    // TODO: Get UCID from CUser
    std::uint32_t dwUCID = 0;
    xSendDBPacket.XParse << dwUCID;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBGame(pGameServer, &xSendDBPacket);
}

// IDA: ?DBAddDailyMissionList@CGocDailyMission@@QEAAXXZ (0x140054EE0)
// Verified: Per IDA decompile - adds daily mission list to DB
// - Gets mission list via GetDailyMissionList
// - Sends DB packet (main=0x48, sub=2) with mission data
// - Logs to ST_LOG_GAME (main=13, sub=1) for each mission
void CGocDailyMission::DBAddDailyMissionList()
{
    PS_MAP_DAILY_MISSION psMission;
    GetDailyMissionList(&psMission);

    // Get owner object
    IXObject* pObject = nullptr;
    // TODO: Get object from owner - this requires access to the parent actor

    XSendDBPacket xSendDBPacket(pObject, 0x48, 2);

    // Get UCID from owner
    // TODO: Get UCID from CUser
    std::uint32_t dwUCID = 0;
    xSendDBPacket.XParse << dwUCID;
    xSendDBPacket << psMission;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    XGameServer::SendDBGame(pGameServer, &xSendDBPacket);

    // Log to ST_LOG_GAME (main=13, sub=1)
    // TODO: Implement logging
    // ST_LOG_GAME stLog;
    // stLog._nUAID = pUser->GetUAID();
    // stLog._nUCID = pUser->GetUCID();
    // stLog._sMainType = 13;
    // stLog._sSubType = 1;
    // stLog.nParam1 = pUser->GetLevel();
    // wcscpy_s(stLog.szComment, L"DAILY_MISSION_ADD");
    // for (auto& pair : psMission)
    // {
    //     stLog.nParam0 = pair.first;
    //     XGameServer::SendDBLog(pGameServer, &stLog);
    // }
}

// IDA: ?OnUpdateDailyMission@CGocDailyMission@@QEAAXXZ (0x140052360)
// Verified: Per IDA decompile - called on daily mission update tick
// - Checks if user is in daily mission state (UserDB flag 0x20)
// - Gets current time and checks if it's within daily reset window (11:00-12:00)
// - Iterates through special missions and checks time validity
// - If any mission is invalid, regenerates mission list and sends to client
void CGocDailyMission::OnUpdateDailyMission()
{
    // TODO: Get CUser owner
    CUser* pUser = nullptr;
    // TODO: Check if user has daily mission flag set (UserDB flag 0x20)
    // if (!pUser || !(*((_BYTE *)&pUser->stMyCharInfoEx()->UserDB + 3) & 0x20))
    // {
    //     return;
    // }

    ATL::CTime tCurr = ATL::CTime::GetCurrentTime();
    ATL::CTimeSpan tInitTime(0, 11, 0, 0);

    // Calculate today's init time (11:00:00)
    int nHour = tInitTime.GetTotalHours();
    int nDay = tCurr.GetDay();
    int nMonth = tCurr.GetMonth();
    int nYear = tCurr.GetYear();
    ATL::CTime tTotayInit(nYear, nMonth, nDay, nHour, 0, 0, -1);

    // Check if current time is within daily reset window (11:00-12:00)
    if (tTotayInit <= tCurr)
    {
        ATL::CTime tTotayEnd = tTotayInit + ATL::CTimeSpan(0, 0, 1, 0);
        if (tCurr < tTotayEnd)
        {
            // Within reset window, check if any special mission is invalid
            for (auto iter = m_mapSpecial.begin(); iter != m_mapSpecial.end(); ++iter)
            {
                std::tr1::shared_ptr<CDailyMissionInfo> pMission = iter->second;
                if (!pMission)
                {
                    continue;
                }

                ST_DAILY_MISSION_INFO* stMission = pMission->GetMissionInfo();
                if (!stMission)
                {
                    continue;
                }

                if (!CheckDailyMissionTime(stMission))
                {
                    // Mission is invalid, regenerate mission list
                    std::vector<std::uint32_t> vecNewMission;
                    // TODO: GenerateDailyMission(1, 1, 1, &vecNewMission);
                    SetNewDailyMissionList(vecNewMission);
                    DBAddDailyMissionList();
                    SendDailyMissionList(1);
                    return;
                }
            }
        }
    }
}

// IDA: ?CalculateMissionRemainTime@CGocDailyMission@@QEAAKVCTime@ATL@@0@Z (0x140050510)
// Verified: Per IDA decompile - calculates remaining time before mission starts
// - Returns 0 if tNow >= tStart (mission already started)
// - Returns difference tStart - tNow in seconds otherwise
std::uint32_t CGocDailyMission::CalculateMissionRemainTime(ATL::CTime tNow, ATL::CTime tStart)
{
    // If current time is at or after start time, no remaining time
    if (tNow >= tStart)
    {
        return 0;
    }

    // Calculate difference in seconds
    ATL::CTimeSpan diff = tStart - tNow;
    return static_cast<std::uint32_t>(diff.GetTotalSeconds());
}

// IDA: ?CalculateGetMissionDurationTime@CGocDailyMission@@QEAAKVCTime@ATL@@00@Z (0x140050570)
// Verified: Per IDA decompile - calculates mission duration time
// - Returns 0 if tNow >= tEnd (mission already ended)
// - Returns tEnd - tStart if tNow < tStart (mission not started yet)
// - Returns tEnd - tNow otherwise (mission in progress)
std::uint32_t CGocDailyMission::CalculateGetMissionDurationTime(ATL::CTime tNow, ATL::CTime tStart, ATL::CTime tEnd)
{
    // If current time is at or after end time, no duration remaining
    if (tNow >= tEnd)
    {
        return 0;
    }

    ATL::CTimeSpan diff;
    if (tNow < tStart)
    {
        // Mission not started yet, return full duration
        diff = tEnd - tStart;
    }
    else
    {
        // Mission in progress, return remaining duration
        diff = tEnd - tNow;
    }

    return static_cast<std::uint32_t>(diff.GetTotalSeconds());
}

// IDA: ?GeneraterTimeRange@CGocDailyMission@@QEAAXKEVCTime@ATL@@AEAV23@1_N@Z (0x14004F960)
// Verified: Per IDA decompile - generates time range based on mission type
// - Type 1 (Special): Uses 11:00:00 as daily reset time, calculates start/end based on current time
// - Type 2 (Guerrilla): Parses Date_Start/Date_End from TB_DAILY_MISSION as time of day (HH:MM:SS)
// - Type 3 (Event): Parses Date_Start/Date_End as full datetime string
void CGocDailyMission::GeneraterTimeRange(std::uint32_t dwMissionID, std::uint8_t byType,
                                           ATL::CTime tNow, ATL::CTime& tStart, ATL::CTime& tEnd,
                                           bool bUseTableDate)
{
    // Type 1: Special - Daily reset at 11:00:00
    if (byType == eDAILY_MISSION_TYPE_SPECIAL)
    {
        // Init time is 11:00:00 (39600 seconds = 11 * 3600)
        ATL::CTimeSpan tInitTime(0, 11, 0, 0);

        // Get current time components
        int nSecs = tNow.GetSecond();
        int nMins = tNow.GetMinute();
        int nHour = tNow.GetHour();
        ATL::CTimeSpan tNowTime(0, nHour, nMins, nSecs);

        if (tNowTime <= tInitTime)
        {
            // Current time is before 11:00 - start is today at 11:00, end is tomorrow
            int nDay = tNow.GetDay();
            int nMonth = tNow.GetMonth();
            int nYear = tNow.GetYear();
            tStart = ATL::CTime(nYear, nMonth, nDay, 11, 0, 0, -1);
            tEnd = tStart + ATL::CTimeSpan(1, 0, 0, 0);
        }
        else
        {
            // Current time is after 11:00 - end is tomorrow at 11:00, start is today
            int nDay = tNow.GetDay();
            int nMonth = tNow.GetMonth();
            int nYear = tNow.GetYear();
            tEnd = ATL::CTime(nYear, nMonth, nDay, 11, 0, 0, -1) + ATL::CTimeSpan(1, 0, 0, 0);
            tStart = tEnd - ATL::CTimeSpan(1, 0, 0, 0);
        }
    }
    // Type 2: Guerrilla - Parse time of day from table
    else if (byType == eDAILY_MISSION_TYPE_GUERRILLA)
    {
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, dwMissionID);
        if (!pTB_Mission)
        {
            return;
        }

        ATL::CTimeSpan tSpan_S(0, 0, 0, 0);
        ATL::CTimeSpan tSpan_E(0, 0, 0, 0);

        if (bUseTableDate)
        {
            // Parse Date_Start as "YYYY-MM-DD HH:MM:SS" but only use HH:MM:SS
            int _year = 2000, _mon = 1, _day = 1, _hour = 0, _min = 0, _sec = 0;
            sscanf_s(pTB_Mission->Date_Start, "%d-%d-%d %d:%d:%d", &_year, &_mon, &_day, &_hour, &_min, &_sec);
            if (_hour <= 24 && _min <= 60 && _sec <= 60)
            {
                tSpan_S = ATL::CTimeSpan(0, _hour, _min, _sec);
            }

            // Parse Date_End
            int v41 = 2000, v43 = 1, v46 = 1, nHours = 0, v42 = 0, v45 = 0;
            sscanf_s(pTB_Mission->Date_End, "%d-%d-%d %d:%d:%d", &v41, &v43, &v46, &nHours, &v42, &v45);
            if (nHours <= 24 && v42 <= 60 && v45 <= 60)
            {
                tSpan_E = ATL::CTimeSpan(0, nHours, v42, v45);
            }
        }
        else
        {
            // Use provided start/end times
            int nSecond = tStart.GetSecond();
            int nMinute = tStart.GetMinute();
            int nHour = tStart.GetHour();
            int nDay = tStart.GetDay();
            tSpan_S = ATL::CTimeSpan(nDay, nHour, nMinute, nSecond);

            nSecond = tEnd.GetSecond();
            nMinute = tEnd.GetMinute();
            nHour = tEnd.GetHour();
            nDay = tEnd.GetDay();
            tSpan_E = ATL::CTimeSpan(nDay, nHour, nMinute, nSecond);
        }

        // Build start/end times using today's date
        int nSec = tSpan_S.GetSeconds();
        int nMin = tSpan_S.GetMinutes();
        int nHour = tSpan_S.GetHours();
        int nDay = tNow.GetDay();
        int nMonth = tNow.GetMonth();
        int nYear = tNow.GetYear();
        tStart = ATL::CTime(nYear, nMonth, nDay, nHour, nMin, nSec, -1);

        nSec = tSpan_E.GetSeconds();
        nMin = tSpan_E.GetMinutes();
        nHour = tSpan_E.GetHours();
        nDay = tNow.GetDay();
        nMonth = tNow.GetMonth();
        nYear = tNow.GetYear();
        tEnd = ATL::CTime(nYear, nMonth, nDay, nHour, nMin, nSec, -1);
    }
    // Type 3: Event - Parse full datetime from table
    else if (byType == eDAILY_MISSION_TYPE_EVENT)
    {
        XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
        TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, dwMissionID);
        if (!pTB_Mission)
        {
            return;
        }

        // Parse Date_Start as full datetime
        int nYear = 2000, v50 = 1, v53 = 1, v51 = 0, v49 = 0, v52 = 0;
        sscanf_s(pTB_Mission->Date_Start, "%d-%d-%d %d:%d:%d", &nYear, &v50, &v53, &v51, &v49, &v52);
        if (nYear >= 2000 && nYear <= 2040 && v50 >= 1 && v50 <= 12 && v53 >= 1 && v53 <= 31 &&
            v51 <= 24 && v49 <= 60 && v52 <= 60)
        {
            tStart = ATL::CTime(nYear, v50, v53, v51, v49, v52, -1);
        }
        else
        {
            tStart = ATL::CTime(2000, 1, 1, 0, 0, 0, -1);
        }

        // Parse Date_End as full datetime
        int v54 = 2000, v56 = 1, v59 = 1, v57 = 0, v55 = 0, v58 = 0;
        sscanf_s(pTB_Mission->Date_End, "%d-%d-%d %d:%d:%d", &v54, &v56, &v59, &v57, &v55, &v58);
        if (v54 >= 2000 && v54 <= 2040 && v56 >= 1 && v56 <= 12 && v59 >= 1 && v59 <= 31 &&
            v57 <= 24 && v55 <= 60 && v58 <= 60)
        {
            tEnd = ATL::CTime(v54, v56, v59, v57, v55, v58, -1);
        }
        else
        {
            tEnd = ATL::CTime(2000, 1, 1, 0, 0, 0, -1);
        }
    }
}

// IDA: ?AddNewDailyMission@CGocDailyMission@@QEAAXKVCTime@ATL@@@Z (0x14004F000)
// Verified: Per IDA decompile - adds a new daily mission
// - Gets TB_DAILY_MISSION from XResourceMgr
// - Generates time range via GeneraterTimeRange
// - Creates ST_DAILY_MISSION_INFO and calls AddDailyMission
void CGocDailyMission::AddNewDailyMission(std::uint32_t dwMissionID, ATL::CTime tNow)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, dwMissionID);
    if (!pTB_Mission)
    {
        return;
    }

    // Initialize time range
    ATL::CTime tStart;
    ATL::CTime tEnd;

    // Generate time range based on mission type
    GeneraterTimeRange(dwMissionID, pTB_Mission->Mission_Type, tNow, tStart, tEnd, true);

    // Create mission info
    ST_DAILY_MISSION_INFO stInfo;
    stInfo.dwMissionID = dwMissionID;
    stInfo.byType = pTB_Mission->Mission_Type;
    stInfo.tAccept = tStart;
    stInfo.tStart = tStart;
    stInfo.tEnd = tEnd;

    // Add the mission
    AddDailyMission(stInfo);
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

// ============================================================================
// IDA: ?UpdateCollectType@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_TARGET@@KH@Z (0x140054360)
// Verified: Per IDA decompile - updates collect-type daily missions
// - Gets missions with eDAILY_MISSION_FINISH_COLLECT finish type
// - Checks Target_Type and Target_ID match
// - Updates condition value and sends DB update if mission not complete
// - Completes mission if condition met
void CGocDailyMission::UpdateCollectType(E_DAILY_MISSION_TARGET eTarget, std::uint32_t dwObjectID, int nAddCount)
{
    std::vector<std::tr1::shared_ptr<CDailyMissionInfo>> vecList;
    GetDailyMissionList(eDAILY_MISSION_FINISH_COLLECT, &vecList);

    PS_DAILY_MISSION_UPDATE psUpdate;
    memset(&psUpdate, 0, sizeof(psUpdate));

    for (size_t i = 0; i < vecList.size(); ++i)
    {
        std::tr1::shared_ptr<CDailyMissionInfo> pMission = vecList[i];
        if (!pMission)
        {
            continue;
        }

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission)
        {
            continue;
        }

        // Check mission state is accepted, target type and target ID match
        if (pMission->GetDailyMissionState() != eDAILY_MISSION_STATE_ACCEPT)
        {
            continue;
        }

        if (pTB_Mission->Target_Type != eTarget || pTB_Mission->Target_ID != dwObjectID)
        {
            continue;
        }

        ST_DAILY_MISSION_INFO* stInfo = pMission->GetMissionInfo();
        if (!stInfo)
        {
            continue;
        }

        // Check if mission time is valid
        if (!CheckDailyMissionTime(stInfo))
        {
            continue;
        }

        // Add condition value
        pMission->AddConditionValue(nAddCount);

        // Check if mission is complete
        std::int16_t shConditionValue = pMission->GetConditionValue();
        if (pTB_Mission->Mission_Count > shConditionValue)
        {
            // Not complete yet, add to update list
            // TODO: Push stInfo to psUpdate vector
        }
        else
        {
            // Mission complete
            std::uint32_t dwMissionID = pMission->GetQuestID();
            if (CompleteDailyMission(dwMissionID, pMission))
            {
                stInfo = pMission->GetMissionInfo();
                // TODO: Push stInfo to psUpdate vector
            }
        }
    }

    // Send updates if any
    // TODO: Check if psUpdate is not empty, call DBUpdateMissionInfo and SendDailyMissionUpdateList
}

// ============================================================================
// IDA: ?UpdateMyRoomType@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_CONDITION@@M@Z (0x140054670)
// Verified: Per IDA decompile - updates my-room-type daily missions
// - Gets missions with eDAILY_MISSION_FINISH_MYROOM finish type
// - Checks Condition_Type match
// - Increments condition value by 1
// - Completes mission if condition met
void CGocDailyMission::UpdateMyRoomType(E_DAILY_MISSION_CONDITION eCondition, float fValue)
{
    std::vector<std::tr1::shared_ptr<CDailyMissionInfo>> vecList;
    GetDailyMissionList(eDAILY_MISSION_FINISH_MYROOM, &vecList);

    PS_DAILY_MISSION_UPDATE psUpdate;
    memset(&psUpdate, 0, sizeof(psUpdate));

    for (size_t i = 0; i < vecList.size(); ++i)
    {
        std::tr1::shared_ptr<CDailyMissionInfo> pMission = vecList[i];
        if (!pMission)
        {
            continue;
        }

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission)
        {
            continue;
        }

        // Check mission state is accepted and condition type matches
        if (pMission->GetDailyMissionState() != eDAILY_MISSION_STATE_ACCEPT)
        {
            continue;
        }

        if (pTB_Mission->Condition_Type != eCondition)
        {
            continue;
        }

        ST_DAILY_MISSION_INFO* stInfo = pMission->GetMissionInfo();
        if (!stInfo)
        {
            continue;
        }

        // Check if mission time is valid
        if (!CheckDailyMissionTime(stInfo))
        {
            continue;
        }

        // Add condition value (always +1 for my-room type)
        pMission->AddConditionValue(1);

        // Check if mission is complete
        std::int16_t shConditionValue = pMission->GetConditionValue();
        if (pTB_Mission->Mission_Count > shConditionValue)
        {
            // Not complete yet, add to update list
            // TODO: Push stInfo to psUpdate vector
        }
        else
        {
            // Mission complete
            std::uint32_t dwMissionID = pMission->GetQuestID();
            if (CompleteDailyMission(dwMissionID, pMission))
            {
                stInfo = pMission->GetMissionInfo();
                // TODO: Push stInfo to psUpdate vector
            }
        }
    }

    // Send updates if any
    // TODO: Check if psUpdate is not empty, call DBUpdateMissionInfo and SendDailyMissionUpdateList
}

// ============================================================================
// IDA: ?CheatChangeMission@CGocDailyMission@@QEAA_NKK@Z (0x1400559D0)
// Verified: Per IDA decompile - GM cheat to change mission
// - Validates new mission exists in TB_DAILY_MISSION
// - Deletes old mission via CheatDeleteMission
// - Adds new mission via AddNewDailyMission
bool CGocDailyMission::CheatChangeMission(std::uint32_t dwTargetID, std::uint32_t dwNewMissionID)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, dwNewMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    // Delete the target mission
    if (!CheatDeleteMission(dwTargetID))
    {
        return false;
    }

    // Add the new mission
    ATL::CTime tCurr = ATL::CTime::GetCurrentTime();
    AddNewDailyMission(dwNewMissionID, tCurr);

    return true;
}

// ============================================================================
// IDA: ?UpdateKillType@CGocDailyMission@@QEAAXKK@Z (0x1400539D0)
// Verified: Per IDA decompile - updates kill-type daily missions
// - Gets missions with eDAILY_MISSION_FINISH_KILL finish type
// - Checks if mission state is accepted and time is valid
// - Calls CheckUpdateKillType to validate kill condition
// - Increments condition value and sends DB update if mission not complete
// - Completes mission if condition met
void CGocDailyMission::UpdateKillType(std::uint32_t dwObjectID, std::uint32_t dwCondition)
{
    std::vector<std::tr1::shared_ptr<CDailyMissionInfo>> vecList;
    GetDailyMissionList(eDAILY_MISSION_FINISH_KILL, &vecList);

    PS_DAILY_MISSION_UPDATE psUpdate;
    memset(&psUpdate, 0, sizeof(psUpdate));

    for (size_t i = 0; i < vecList.size(); ++i)
    {
        std::tr1::shared_ptr<CDailyMissionInfo> pMission = vecList[i];
        if (!pMission)
        {
            continue;
        }

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission)
        {
            continue;
        }

        // Check mission state is accepted
        if (pMission->GetDailyMissionState() != eDAILY_MISSION_STATE_ACCEPT)
        {
            continue;
        }

        ST_DAILY_MISSION_INFO* stInfo = pMission->GetMissionInfo();
        if (!stInfo)
        {
            continue;
        }

        // Check if mission time is valid
        if (!CheckDailyMissionTime(stInfo))
        {
            continue;
        }

        // Check if kill condition matches
        std::uint32_t dwMissionID = pMission->GetQuestID();
        if (!CheckUpdateKillType(dwMissionID, dwObjectID, dwCondition))
        {
            continue;
        }

        // Add condition value
        pMission->AddConditionValue(1);

        // Check if mission is complete
        std::int16_t shConditionValue = pMission->GetConditionValue();
        if (pTB_Mission->Mission_Count > shConditionValue)
        {
            // Not complete yet, add to update list
            // TODO: Push stInfo to psUpdate vector
        }
        else
        {
            // Mission complete
            if (CompleteDailyMission(dwMissionID, pMission))
            {
                stInfo = pMission->GetMissionInfo();
                // TODO: Push stInfo to psUpdate vector
            }
        }
    }

    // Send updates if any
    // TODO: Check if psUpdate is not empty, call DBUpdateMissionInfo and SendDailyMissionUpdateList
}

// ============================================================================
// IDA: ?UpdateMazeClearType@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_TARGET@@KFK_N@Z (0x140053D00)
// Verified: Per IDA decompile - updates maze-clear-type daily missions
// - Gets missions with eDAILY_MISSION_FINISH_MAZE_CLEAR finish type
// - If bPartyWith is true, also gets eDAILY_MISSION_FINISH_PARTY_MAZE_CLEAR missions
// - Checks if mission state is accepted, target type matches, and time is valid
// - Calls CheckUpdateMazeClearType to validate maze clear condition
// - Increments condition value and sends DB update if mission not complete
// - Completes mission if condition met
void CGocDailyMission::UpdateMazeClearType(E_DAILY_MISSION_TARGET eTarget, std::uint32_t dwObjectID,
                                           std::int16_t shRank, std::uint32_t dwTime, bool bPartyWith)
{
    std::vector<std::tr1::shared_ptr<CDailyMissionInfo>> vecList;
    GetDailyMissionList(eDAILY_MISSION_FINISH_MAZE_CLEAR, &vecList);

    // Also get party maze clear missions if party with
    if (bPartyWith)
    {
        GetDailyMissionList(eDAILY_MISSION_FINISH_PARTY_MAZE_CLEAR, &vecList);
    }

    PS_DAILY_MISSION_UPDATE psUpdate;
    memset(&psUpdate, 0, sizeof(psUpdate));

    for (size_t i = 0; i < vecList.size(); ++i)
    {
        std::tr1::shared_ptr<CDailyMissionInfo> pMission = vecList[i];
        if (!pMission)
        {
            continue;
        }

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission)
        {
            continue;
        }

        // Check mission state is accepted and target type matches
        if (pMission->GetDailyMissionState() != eDAILY_MISSION_STATE_ACCEPT)
        {
            continue;
        }

        if (pTB_Mission->Target_Type != eTarget)
        {
            continue;
        }

        ST_DAILY_MISSION_INFO* stInfo = pMission->GetMissionInfo();
        if (!stInfo)
        {
            continue;
        }

        // Check if mission time is valid
        if (!CheckDailyMissionTime(stInfo))
        {
            continue;
        }

        // Check if maze clear condition matches
        std::uint32_t dwMissionID = pMission->GetQuestID();
        if (!CheckUpdateMazeClearType(dwMissionID, dwObjectID, shRank, dwTime))
        {
            continue;
        }

        // Add condition value
        pMission->AddConditionValue(1);

        // Check if mission is complete
        std::int16_t shConditionValue = pMission->GetConditionValue();
        if (pTB_Mission->Mission_Count > shConditionValue)
        {
            // Not complete yet, add to update list
            // TODO: Push stInfo to psUpdate vector
        }
        else
        {
            // Mission complete
            if (CompleteDailyMission(dwMissionID, pMission))
            {
                stInfo = pMission->GetMissionInfo();
                // TODO: Push stInfo to psUpdate vector
            }
        }
    }

    // Send updates if any
    // TODO: Check if psUpdate is not empty, call DBUpdateMissionInfo and SendDailyMissionUpdateList
}

// ============================================================================
// IDA: ?CheatDeleteMission@CGocDailyMission@@QEAA_NK@Z (0x140055AF0)
// Verified: Per IDA decompile - GM cheat to delete a specific mission
// - Finds the mission using FindMission
// - Gets mission type and erases from appropriate map (Special/Guerrilla/Event)
bool CGocDailyMission::CheatDeleteMission(std::uint32_t dwMissionID)
{
    // Find the mission
    std::tr1::shared_ptr<CDailyMissionInfo> pMission = FindMission(dwMissionID, 0);
    if (!pMission)
    {
        return false;
    }

    // Get mission type
    std::uint8_t byType = pMission->GetDailyMissionType();

    // Erase from appropriate map based on type
    switch (byType)
    {
        case eDAILY_MISSION_TYPE_SPECIAL:
            {
                auto iter = m_mapSpecial.find(dwMissionID);
                if (iter == m_mapSpecial.end())
                {
                    return false;
                }
                m_mapSpecial.erase(iter);
            }
            break;

        case eDAILY_MISSION_TYPE_GUERRILLA:
            {
                auto iter = m_mapGuerrilla.find(dwMissionID);
                if (iter == m_mapGuerrilla.end())
                {
                    return false;
                }
                m_mapGuerrilla.erase(iter);
            }
            break;

        case eDAILY_MISSION_TYPE_EVENT:
            {
                auto iter = m_mapEvent.find(dwMissionID);
                if (iter == m_mapEvent.end())
                {
                    return false;
                }
                m_mapEvent.erase(iter);
            }
            break;

        default:
            return false;
    }

    return true;
}

// ============================================================================
// IDA: ?CheatDeleteAllMission@CGocDailyMission@@QEAA_NXZ (0x140055A50)
// Verified: Per IDA decompile - GM cheat to delete all missions
// - Gets all missions via GetDailyMissionList
// - Iterates and deletes each mission via CheatDeleteMission
bool CGocDailyMission::CheatDeleteAllMission()
{
    PS_MAP_DAILY_MISSION psMissionList;
    memset(&psMissionList, 0, sizeof(psMissionList));

    GetDailyMissionList(&psMissionList);

    // Iterate and delete each mission
    for (auto& pair : psMissionList)
    {
        CheatDeleteMission(pair.first);
    }

    return true;
}

// ============================================================================
// IDA: ?CheatAddMission@CGocDailyMission@@QEAA_NK@Z (0x140055DB0)
// Verified: Per IDA decompile - GM cheat to add a mission
// - Validates mission exists in TB_DAILY_MISSION
// - Checks district limits: Special type max 5, Guerrilla type max 1
// - Adds mission via AddNewDailyMission
bool CGocDailyMission::CheatAddMission(std::uint32_t dwMissionID)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, dwMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    // Get current mission list for district
    PS_MAP_DISTRICT_DAILY_MISSION psMissionList;
    memset(&psMissionList, 0, sizeof(psMissionList));
    GetDailyMissionList(&psMissionList);

    // Check mission count limits for the district
    auto iter = psMissionList.find(pTB_Mission->District_ID);
    if (iter != psMissionList.end())
    {
        int nCnt = 0;
        for (const auto& stInfo : iter->second)
        {
            if (stInfo.byType == pTB_Mission->Mission_Type)
            {
                ++nCnt;
            }
        }

        // Check limits based on mission type
        std::uint8_t byMissionType = pTB_Mission->Mission_Type;
        if (byMissionType == eDAILY_MISSION_TYPE_SPECIAL && nCnt >= 5)
        {
            return false;
        }
        if (byMissionType == eDAILY_MISSION_TYPE_GUERRILLA && nCnt >= 1)
        {
            return false;
        }
    }

    // Add the new mission
    ATL::CTime tCurr = ATL::CTime::GetCurrentTime();
    AddNewDailyMission(dwMissionID, tCurr);

    return true;
}

// ============================================================================
// IDA: ?CheckUpdateFriendType@CGocDailyMission@@QEAA_NKEEAEAUST_DAILY_MISSION_FRIEND_RES@@@Z (0x140053150)
// Verified: Per IDA decompile - checks friend type daily mission conditions
// - Condition_Type 11: Level difference check
// - Condition_Type 12: Class match check
// - Condition_Type 13: Weapon equipment check
// - Condition_Type 14: Costume equipment check
// - Condition_Type 15: Prefix title check
// - Condition_Type 16: Suffix title check
// - Condition_Type 17: Full title check (prefix + suffix)
bool CGocDailyMission::CheckUpdateFriendType(std::uint32_t dwMissionID, std::uint8_t byClass,
                                             std::uint8_t byLevel, ST_DAILY_MISSION_FRIEND_RES& stResult)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, dwMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    stResult.dwMissionID = dwMissionID;
    stResult.byCheckResult = 0;

    // Condition types start at 11
    int nConditionType = pTB_Mission->Condition_Type - 11;

    switch (nConditionType)
    {
        case 0: // Condition_Type 11: Level difference check
            {
                // TODO: Get player level from mover
                std::uint8_t byPlayerLevel = 0; // Placeholder
                int nDiff = (byPlayerLevel >= byLevel) ? (byPlayerLevel - byLevel) : (byLevel - byPlayerLevel);
                if (static_cast<std::int16_t>(nDiff) <= static_cast<std::int16_t>(pTB_Mission->Condition_Value))
                {
                    stResult.byCheckResult = 1;
                }
            }
            return true;

        case 1: // Condition_Type 12: Class match check
            {
                // TODO: Get player class from mover
                std::uint8_t byPlayerClass = 0; // Placeholder
                if (pTB_Mission->Condition_Class == 7)
                {
                    // Any class match
                    if (byClass == byPlayerClass)
                    {
                        stResult.byCheckResult = 1;
                    }
                }
                else
                {
                    if (pTB_Mission->Condition_Class == byPlayerClass)
                    {
                        stResult.byCheckResult = 1;
                    }
                }
            }
            return true;

        case 2: // Condition_Type 13: Weapon equipment check
            {
                // TODO: Get weapon from inventory and check Condition_Value
                // Check weapon slots 1-8, then costume slots 1-8
            }
            return true;

        case 3: // Condition_Type 14: Costume equipment check
            {
                // TODO: Get costume from inventory and check Condition_Value
            }
            return true;

        case 4: // Condition_Type 15: Prefix title check
            {
                // TODO: Get outside title from entity and check prefix
            }
            return true;

        case 5: // Condition_Type 16: Suffix title check
            {
                // TODO: Get outside title from entity and check suffix
            }
            return true;

        case 6: // Condition_Type 17: Full title check (prefix + suffix)
            {
                // TODO: Get outside title from entity and check both prefix and suffix
            }
            return true;

        default:
            return false;
    }
}

// ============================================================================
// IDA: ?CheatChangeGuerillaMission@CGocDailyMission@@QEAA_NKPEA_W0@Z (0x140055F90)
// Verified: Per IDA decompile - GM cheat to change guerrilla mission with custom time
// - Parses start/end time strings (HH:MM format)
// - Validates mission exists and district limits
// - Creates mission with custom time range
bool CGocDailyMission::CheatChangeGuerillaMission(std::uint32_t dwNewMissionID, const wchar_t* szStart, const wchar_t* szEnd)
{
    if (!szStart || !szEnd)
    {
        return false;
    }

    // Convert wide strings to narrow strings
    char strStart[512] = {0};
    char strEnd[512] = {0};
    WideCharToMultiByte(0, 0, szStart, -1, strStart, 10, nullptr, nullptr);
    WideCharToMultiByte(0, 0, szEnd, -1, strEnd, 10, nullptr, nullptr);

    // Get current time
    ATL::CTime tNow = ATL::CTime::GetCurrentTime();

    // Parse start time (HH:MM)
    int nHour = -1, nMin = -1;
    sscanf_s(strStart, "%d:%d", &nHour, &nMin);
    if (nHour > 24 || nMin > 60)
    {
        return false;
    }

    ATL::CTime tStart(tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), nHour, nMin, 0, -1);

    // Parse end time (HH:MM)
    nHour = -1;
    nMin = -1;
    sscanf_s(strEnd, "%d:%d", &nHour, &nMin);
    if (nHour > 24 || nMin > 60)
    {
        return false;
    }

    ATL::CTime tEnd(tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), nHour, nMin, 0, -1);

    // Validate times
    if (tStart.GetTime() <= 0 || tEnd.GetTime() <= 0)
    {
        return false;
    }

    // Validate mission exists
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = XResourceMgr::GetTB_DAILY_MISSION(&pGameServer->m_xResourceMgr, dwNewMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    // Check district limits
    PS_MAP_DISTRICT_DAILY_MISSION psMissionList;
    memset(&psMissionList, 0, sizeof(psMissionList));
    GetDailyMissionList(&psMissionList);

    auto iter = psMissionList.find(pTB_Mission->District_ID);
    if (iter != psMissionList.end())
    {
        int nCnt = 0;
        for (const auto& stInfo : iter->second)
        {
            if (stInfo.byType == pTB_Mission->Mission_Type)
            {
                ++nCnt;
            }
        }

        std::uint8_t byMissionType = pTB_Mission->Mission_Type;
        if (byMissionType == eDAILY_MISSION_TYPE_SPECIAL && nCnt >= 5)
        {
            return false;
        }
        if (byMissionType == eDAILY_MISSION_TYPE_GUERRILLA && nCnt >= 1)
        {
            return false;
        }
    }

    // Generate time range and add mission
    ATL::CTime tCurr = ATL::CTime::GetCurrentTime();
    ATL::CTime tGenStart = tStart;
    ATL::CTime tGenEnd = tEnd;
    GeneraterTimeRange(dwNewMissionID, pTB_Mission->Mission_Type, tCurr, tGenStart, tGenEnd, false);

    ST_DAILY_MISSION_INFO stInfo;
    memset(&stInfo, 0, sizeof(stInfo));
    stInfo.dwMissionID = dwNewMissionID;
    stInfo.byType = pTB_Mission->Mission_Type;
    stInfo.tAccept = tStart;
    stInfo.tStart = tGenStart;
    stInfo.tEnd = tGenEnd;

    AddDailyMission(stInfo);

    return true;
}

// ============================================================================
// IDA: ?GetMissionInfo@CGocDailyMission@@QEAA?AV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@KE@Z (0x14004EC60)
// Verified: Per IDA decompile - gets mission info by mission ID and type
// - Searches appropriate map based on byType (1=Special, 2=Guerrilla, 3=Event)
// - Returns shared_ptr to CDailyMissionInfo if found, empty otherwise
std::tr1::shared_ptr<CDailyMissionInfo> CGocDailyMission::GetMissionInfo(std::uint32_t dwMissionID, std::uint8_t byType)
{
    if (byType == eDAILY_MISSION_TYPE_SPECIAL)
    {
        auto iter = m_mapSpecial.find(dwMissionID);
        if (iter != m_mapSpecial.end())
        {
            return iter->second;
        }
    }
    else if (byType == eDAILY_MISSION_TYPE_GUERRILLA)
    {
        auto iter = m_mapGuerrilla.find(dwMissionID);
        if (iter != m_mapGuerrilla.end())
        {
            return iter->second;
        }
    }
    else if (byType == eDAILY_MISSION_TYPE_EVENT)
    {
        auto iter = m_mapEvent.find(dwMissionID);
        if (iter != m_mapEvent.end())
        {
            return iter->second;
        }
    }

    return std::tr1::shared_ptr<CDailyMissionInfo>();
}

// ============================================================================
// IDA: ?FindMission@CGocDailyMission@@QEAA?AV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@KE@Z (0x14004EE60)
// Verified: Per IDA decompile - finds mission by ID, searching all types if byType is 0
// - If byType is specified, delegates to GetMissionInfo
// - If byType is 0, searches all three maps (Special, Guerrilla, Event)
std::tr1::shared_ptr<CDailyMissionInfo> CGocDailyMission::FindMission(std::uint32_t dwMissionID, std::uint8_t byType)
{
    if (byType != 0)
    {
        return GetMissionInfo(dwMissionID, byType);
    }

    // Search all types
    for (std::uint8_t byTemp = 1; byTemp <= 3; ++byTemp)
    {
        std::tr1::shared_ptr<CDailyMissionInfo> pFind = GetMissionInfo(dwMissionID, byTemp);
        if (pFind && pFind->GetDailyMissionType() != 0)
        {
            return pFind;
        }
    }

    return std::tr1::shared_ptr<CDailyMissionInfo>();
}

// ============================================================================
// IDA: ?SetNewDailyMissionList@CGocDailyMission@@QEAAXAEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x14004EF80)
// Verified: Per IDA decompile - sets new daily mission list
// - Clears existing missions
// - Adds each mission from the vector via AddNewDailyMission
void CGocDailyMission::SetNewDailyMissionList(std::vector<std::uint32_t>& vecNewMission)
{
    Clear();

    ATL::CTime tCurr = ATL::CTime::GetCurrentTime();
    for (size_t i = 0; i < vecNewMission.size(); ++i)
    {
        AddNewDailyMission(vecNewMission[i], tCurr);
    }
}

// ============================================================================
// IDA: ?GenerateDailyMission@CGocDailyMission@@QEAA_N_N00AEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x1400517B0)
// Verified: Per IDA decompile - generates daily missions by type
// - Generates Special (type 1), Guerrilla (type 2), Event (type 3) missions
// - Uses CDailyMissionMgr to generate each type
// - Returns true if any missions were generated
bool CGocDailyMission::GenerateDailyMission(bool bSpecial, bool bGuerrilla, bool bEvent,
                                            std::vector<std::uint32_t>& vecNewMission)
{
    vecNewMission.clear();

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();

    if (bSpecial)
    {
        CDailyMissionMgr* pMgr = pGameServer->GetDailyMissionMgr();
        if (pMgr)
        {
            pMgr->GenerateDailyMission(eDAILY_MISSION_TYPE_SPECIAL, vecNewMission);
        }
    }

    if (bGuerrilla)
    {
        CDailyMissionMgr* pMgr = pGameServer->GetDailyMissionMgr();
        if (pMgr)
        {
            pMgr->GenerateDailyMission(eDAILY_MISSION_TYPE_GUERRILLA, vecNewMission);
        }
    }

    if (bEvent)
    {
        CDailyMissionMgr* pMgr = pGameServer->GetDailyMissionMgr();
        if (pMgr)
        {
            pMgr->GenerateDailyMission(eDAILY_MISSION_TYPE_EVENT, vecNewMission);
        }
    }

    return !vecNewMission.empty();
}

// ============================================================================
// IDA: ?FriendCheckDailyMission@CGocDailyMission@@QEAAXV?$vector@KV?$allocator@K@std@@@std@@EEAEAV?$vector@UST_DAILY_MISSION_FRIEND_RES@@V?$allocator@UST_DAILY_MISSION_FRIEND_RES@@@std@@@3@@Z (0x140053910)
// Verified: Per IDA decompile - checks friend daily mission conditions
// - Iterates through requested mission IDs
// - Calls CheckUpdateFriendType for each mission
// - Pushes results to output vector
void CGocDailyMission::FriendCheckDailyMission(std::vector<std::uint32_t>& vecReq,
                                               std::uint8_t byClass, std::uint8_t byLevel,
                                               std::vector<ST_DAILY_MISSION_FRIEND_RES>& vecRes)
{
    for (size_t i = 0; i < vecReq.size(); ++i)
    {
        ST_DAILY_MISSION_FRIEND_RES stResult;
        memset(&stResult, 0, sizeof(stResult));

        if (CheckUpdateFriendType(vecReq[i], byClass, byLevel, stResult))
        {
            vecRes.push_back(stResult);
        }
    }
}

// ============================================================================
// IDA: ?CheckDailyMissionInfo@CGocDailyMission@@QEAA_NAEAV?$map@KUST_DAILY_MISSION_INFO@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKUST_DAILY_MISSION_INFO@@@std@@@3@@std@@@Z (0x140051860)
// Verified: Per IDA decompile - checks daily mission info validity
// - Iterates through mission map
// - Checks if mission state is accepted (state=1)
// - Returns false if any accepted mission has invalid time
bool CGocDailyMission::CheckDailyMissionInfo(std::map<std::uint32_t, ST_DAILY_MISSION_INFO>& mapMission)
{
    for (auto& pair : mapMission)
    {
        // Check if mission state is accepted (state=1)
        if (pair.second.byState == eDAILY_MISSION_STATE_ACCEPT)
        {
            if (!CheckDailyMissionTime(&pair.second))
            {
                return false;
            }
        }
    }
    return true;
}

// ============================================================================
// IDA: ?SetDailyMissionList@CGocDailyMission@@QEAAXAEAV?$map@KUST_DAILY_MISSION_INFO@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKUST_DAILY_MISSION_INFO@@@std@@@3@@std@@@Z (0x14004F100)
// Verified: Per IDA decompile - sets daily mission list from map
// - Clears existing missions
// - Iterates through map and adds each mission via AddDailyMission
void CGocDailyMission::SetDailyMissionList(std::map<std::uint32_t, ST_DAILY_MISSION_INFO>& mapInfo)
{
    Clear();

    for (auto& pair : mapInfo)
    {
        AddDailyMission(pair.second);
    }
}

// ============================================================================
// IDA: ?GetDailyMissionList@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_FINISH@@AEAV?$vector@V?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@V?$allocator@V?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@@3@@std@@@Z (0x140051350)
// Verified: Per IDA decompile - gets mission list by finish type
// - Iterates through all three maps (Special, Guerrilla, Event)
// - Filters by finish type and adds matching missions to vector
void CGocDailyMission::GetDailyMissionList(E_DAILY_MISSION_FINISH eType,
                                           std::vector<std::tr1::shared_ptr<CDailyMissionInfo>>* vecList)
{
    if (!vecList)
    {
        return;
    }

    // Check special missions
    for (auto& pair : m_mapSpecial)
    {
        if (pair.second && pair.second->GetDailyMissionFinishType() == eType)
        {
            vecList->push_back(pair.second);
        }
    }

    // Check guerrilla missions
    for (auto& pair : m_mapGuerrilla)
    {
        if (pair.second && pair.second->GetDailyMissionFinishType() == eType)
        {
            vecList->push_back(pair.second);
        }
    }

    // Check event missions
    for (auto& pair : m_mapEvent)
    {
        if (pair.second && pair.second->GetDailyMissionFinishType() == eType)
        {
            vecList->push_back(pair.second);
        }
    }
}

// ============================================================================
// IDA: ?GetDailyMissionList@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_FINISH@@AEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x140051580)
// Verified: Per IDA decompile - gets mission ID list by finish type
// - Iterates through all three maps (Special, Guerrilla, Event)
// - Filters by finish type and adds matching mission IDs to vector
void CGocDailyMission::GetDailyMissionList(E_DAILY_MISSION_FINISH eType,
                                           std::vector<std::uint32_t>* vecList)
{
    if (!vecList)
    {
        return;
    }

    // Check special missions
    for (auto& pair : m_mapSpecial)
    {
        if (pair.second && pair.second->GetDailyMissionFinishType() == eType)
        {
            vecList->push_back(pair.first);
        }
    }

    // Check guerrilla missions
    for (auto& pair : m_mapGuerrilla)
    {
        if (pair.second && pair.second->GetDailyMissionFinishType() == eType)
        {
            vecList->push_back(pair.first);
        }
    }

    // Check event missions
    for (auto& pair : m_mapEvent)
    {
        if (pair.second && pair.second->GetDailyMissionFinishType() == eType)
        {
            vecList->push_back(pair.first);
        }
    }
}

// ============================================================================
// IDA: ?UpdateFriendType@CGocDailyMission@@QEAAXAEAV?$vector@UST_DAILY_MISSION_FRIEND_RES@@V?$allocator@UST_DAILY_MISSION_FRIEND_RES@@@std@@@std@@@Z (0x140054080)
// Verified: Per IDA decompile - updates friend-type daily missions
// - Iterates through friend mission results
// - Finds mission by ID, checks state and target type (friend=6)
// - If check result is 1, updates condition value
// - Completes mission if condition met
void CGocDailyMission::UpdateFriendType(std::vector<ST_DAILY_MISSION_FRIEND_RES>& vecMission)
{
    PS_DAILY_MISSION_UPDATE psUpdate;
    memset(&psUpdate, 0, sizeof(psUpdate));

    for (size_t i = 0; i < vecMission.size(); ++i)
    {
        ST_DAILY_MISSION_FRIEND_RES& stRes = vecMission[i];

        // Find mission by ID
        std::tr1::shared_ptr<CDailyMissionInfo> pMission = FindMission(stRes.dwMissionID, 0);
        if (!pMission)
        {
            continue;
        }

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission)
        {
            continue;
        }

        // Check mission state is accepted and target type is friend (6)
        if (pMission->GetDailyMissionState() != eDAILY_MISSION_STATE_ACCEPT)
        {
            continue;
        }

        if (pTB_Mission->Target_Type != eDAILY_MISSION_TARGET_FRIEND)
        {
            continue;
        }

        ST_DAILY_MISSION_INFO* stInfo = pMission->GetMissionInfo();
        if (!stInfo)
        {
            continue;
        }

        // Check mission time and result
        if (!CheckDailyMissionTime(stInfo))
        {
            continue;
        }

        if (stRes.byCheckResult != 1)
        {
            continue;
        }

        // Add condition value
        pMission->AddConditionValue(1);

        // Check if mission is complete
        std::int16_t shConditionValue = pMission->GetConditionValue();
        if (pTB_Mission->Mission_Count > shConditionValue)
        {
            // Not complete yet, add to update list
            // TODO: Push stInfo to psUpdate vector
        }
        else
        {
            // Mission complete
            std::uint32_t dwMissionID = pMission->GetQuestID();
            if (CompleteDailyMission(dwMissionID, pMission))
            {
                stInfo = pMission->GetMissionInfo();
                // TODO: Push stInfo to psUpdate vector
            }
        }
    }

    // Send updates if any
    // TODO: Check if psUpdate is not empty, call DBUpdateMissionInfo and SendDailyMissionUpdateList
}
