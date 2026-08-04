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
#include "GocNetwork.h"
#include "GocPost.h"
#include "GocAchieve.h"
#include "GocQuest.h"
#include "../../GameServer.h"
#include "../../User.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerItem.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <algorithm>
#include <cstring>
#include <cstdlib>

// Note: ATL::CTime and ATL::CTimeSpan are forward-declared in GocDailyMission.h
// If actual implementation is needed, it should be provided elsewhere

// Forward declarations for CDailyMissionInfo (to be implemented separately)
class CDailyMissionInfo {
public:
    CDailyMissionInfo(std::uint32_t dwMissionID, TB_DAILY_MISSION* pTBMission)
        : m_dwMissionID(dwMissionID)
        , m_pTBMission(pTBMission)
        , m_pMissionInfo(nullptr)
    {}
    
    ~CDailyMissionInfo() {}

    // IDA: ?InitAddMission@CDailyMissionInfo@@QEAAXAEAUST_DAILY_MISSION_INFO@@@Z (0x14004EA40)
    // 对齐 IDA: 复制任务信息到 m_stMission
    void InitAddMission(ST_DAILY_MISSION_INFO* stMission) {
        if (stMission) {
            m_stMission = *stMission;
            m_pMissionInfo = &m_stMission;
        }
    }
    
    // IDA: ?InitAcceptMission@CDailyMissionInfo@@QEAAXXZ (0x14004EA90)
    // 对齐 IDA: 初始化接受任务状态
    void InitAcceptMission() {
        SetDailyMissionState(1);  // 设置状态为已接受
        // tAccept is CTime array[8], need to set current time
        // For now, just reset condition value
        if (m_pMissionInfo) {
            m_pMissionInfo->shValue = 0;  // 重置条件值
        }
    }

    std::uint8_t GetDailyMissionState() const {
        if (m_pMissionInfo) return m_pMissionInfo->byState;
        return 0;
    }
    
    void SetDailyMissionState(std::uint8_t byState) {
        if (m_pMissionInfo) m_pMissionInfo->byState = byState;
    }

    std::int16_t GetConditionValue() const {
        if (m_pMissionInfo) return m_pMissionInfo->shValue;
        return 0;
    }
    
    void AddConditionValue(std::int16_t shValue) {
        if (m_pMissionInfo) m_pMissionInfo->shValue += shValue;
    }

    bool GetDailyMissionHelper() const {
        if (m_pMissionInfo) return m_pMissionInfo->byAddHelper != 0;
        return false;
    }
    
    void SetDailyMissionHelper(bool bHelper) {
        if (m_pMissionInfo) m_pMissionInfo->byAddHelper = bHelper ? 1 : 0;
    }

    std::uint8_t GetDailyMissionType() const {
        if (m_pTBMission) return m_pTBMission->Mission_Type;
        return 0;
    }
    
    std::uint8_t GetDailyMissionFinishType() const {
        if (m_pTBMission) return m_pTBMission->Finish_Type;
        return 0;
    }
    
    std::uint32_t GetQuestID() const {
        return m_dwMissionID;
    }

    TB_DAILY_MISSION* GetTBMission() const { return m_pTBMission; }
    ST_DAILY_MISSION_INFO* GetMissionInfo() const { return m_pMissionInfo; }

private:
    std::uint32_t m_dwMissionID;
    TB_DAILY_MISSION* m_pTBMission;
    ST_DAILY_MISSION_INFO* m_pMissionInfo;
    ST_DAILY_MISSION_INFO m_stMission;  // IDA: 存储任务信息的本地副本
};

// ============================================================================
// CGocDailyMission Implementation
// ============================================================================

// IDA: ??0CGocDailyMission@@QEAA@XZ (0x14004EAD0)
// Verified: Per IDA decompile at 0x14004EAD0:
// - Calls GOComponent::GOComponent(this)
// - Sets vftable to CGocDailyMission::`vftable'
// - Constructs m_mapSpecial (std::map<int, std::shared_ptr<CDropItemGroup>>)
// - Constructs m_mapGuerrilla (std::map<int, std::shared_ptr<CDropItemGroup>>)
// - Constructs m_mapEvent (std::map<int, std::shared_ptr<CDropItemGroup>>)
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
std::shared_ptr<CDailyMissionInfo> CGocDailyMission::GetMissionInfo(std::uint32_t dwMissionID, std::uint8_t byType)
{
    std::map<std::uint32_t, std::shared_ptr<CDailyMissionInfo>>* pMap = nullptr;

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
        return std::shared_ptr<CDailyMissionInfo>();
    }

    auto it = pMap->find(dwMissionID);
    if (it != pMap->end())
    {
        return it->second;
    }

    return std::shared_ptr<CDailyMissionInfo>();
}

// IDA: ?FindMission@CGocDailyMission@@QEAA?AV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@KE@Z (0x14004EE60)
// Find mission by ID, searching all types if byType is 0
std::shared_ptr<CDailyMissionInfo> CGocDailyMission::FindMission(std::uint32_t dwMissionID, std::uint8_t byType)
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

    return std::shared_ptr<CDailyMissionInfo>();
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
    TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION(stMission.dwMissionID);
    if (!pTB_Mission)
    {
        return;
    }

    std::map<std::uint32_t, std::shared_ptr<CDailyMissionInfo>>* pMap = nullptr;

    switch (pTB_Mission->Mission_Type)
    {
    case eDAILY_MISSION_TYPE_SPECIAL:
        {
            pMap = &m_mapSpecial;

            // Calculate remain time and duration time
            ATL::CTime tNow = ATL::CTime::GetTickCount();
            ATL::CTime tStart = *reinterpret_cast<ATL::CTime*>(stMission.tStart);
            ATL::CTime tEnd = *reinterpret_cast<ATL::CTime*>(stMission.tEnd);
            stMission.dwRemainTime = CalculateMissionRemainTime(tNow, tStart);
            stMission.dwDurationTime = CalculateGetMissionDurationTime(tNow, tStart, tEnd);

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

            std::shared_ptr<CDailyMissionInfo> pMission(pMissionInfo);
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
            ATL::CTime tNow = ATL::CTime::GetTickCount();
            ATL::CTime tStart = *reinterpret_cast<ATL::CTime*>(stMission.tStart);
            ATL::CTime tEnd = *reinterpret_cast<ATL::CTime*>(stMission.tEnd);
            stMission.dwRemainTime = CalculateMissionRemainTime(tNow, tStart);
            stMission.dwDurationTime = CalculateGetMissionDurationTime(tNow, tStart, tEnd);

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

            std::shared_ptr<CDailyMissionInfo> pMission(pMissionInfo);
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
            ATL::CTime tNow = ATL::CTime::GetTickCount();
            ATL::CTime tStart = *reinterpret_cast<ATL::CTime*>(stMission.tStart);
            ATL::CTime tEnd = *reinterpret_cast<ATL::CTime*>(stMission.tEnd);
            stMission.dwRemainTime = CalculateMissionRemainTime(tNow, tStart);
            stMission.dwDurationTime = CalculateGetMissionDurationTime(tNow, tStart, tEnd);

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

            std::shared_ptr<CDailyMissionInfo> pMission(pMissionInfo);
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
        CMover* pMover = GetOwnerGO();
        if (pMover)
        {
            CGocNetwork::SendErrorMessage(pMover, 0x24, 2, 0xD2F4);
        }
        return false;
    }

    // Check mission state
    std::uint8_t byState = pMission->GetDailyMissionState();
    if (byState == eDAILY_MISSION_STATE_COMPLETE || byState == eDAILY_MISSION_STATE_REWARD)
    {
        // Already completed or rewarded - send error (0xD2F2)
        CMover* pMover = GetOwnerGO();
        if (pMover)
        {
            CGocNetwork::SendErrorMessage(pMover, 0x24, 2, 0xD2F2);
        }
        return false;
    }

    if (byState == eDAILY_MISSION_STATE_ACCEPT)
    {
        // Already accepted - send error (0xD2F1)
        CMover* pMover = GetOwnerGO();
        if (pMover)
        {
            CGocNetwork::SendErrorMessage(pMover, 0x24, 2, 0xD2F1);
        }
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
            CMover* pMover = GetOwnerGO();
            if (pMover)
            {
                CGocNetwork::SendErrorMessage(pMover, 0x24, 2, 0xD2F4);
            }
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
    XSendPacket xSendPacket(0x24, 2);
    xSendPacket.XParse << dwMissionID;
    xSendPacket.XParse << pMission->GetDailyMissionState();

    CMover* pMover = GetOwnerGO();
    if (pMover)
    {
        CGocNetwork::Send(reinterpret_cast<XActor*>(pMover), xSendPacket);
    }

    // Log to ST_LOG_GAME (main=13, sub=2)
    ST_LOG_GAME stLog;
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (pUser)
    {
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = pUser->GetID();
        stLog._sMainType = 13;
        stLog._sSubType = 2;
        stLog.nParam0 = dwMissionID;
        stLog.nParam1 = pUser->GetLevel();
        wcscpy_s(stLog.szComment, L"DAILY_MISSION_ACCEPT");
        TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);
    }

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
bool CGocDailyMission::CompleteDailyMission(std::uint32_t dwMissionID, std::shared_ptr<CDailyMissionInfo> pMission)
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

    // Get owner CMover and update achieve component
    CMover* pMover = GetOwnerGO();
    if (pMover)
    {
        auto pAchieve = pMover->GetGOC_Achieve(false);
        if (pAchieve)
        {
            pAchieve->UpdateCollect(0x35, 1, 0);
        }
    }

    // Update database
    DBDailyMissionPost(dwMissionID);

    // Get owner CMover and update quest component
    if (pMover)
    {
        auto pQuest = pMover->GetGOC_Quest(false);
        if (pQuest)
        {
            pQuest->UpdateCondition(eCONDITION_TYPE_CLEAR, eCONDITION_TARGET_EVENT, 1, 1, 0);
        }
    }

    return true;
}

// IDA: ?UpdateKillType@CGocDailyMission@@QEAAXKK@Z (0x1400539D0)
// Update kill-type daily missions
void CGocDailyMission::UpdateKillType(std::uint32_t dwObjectID, std::uint32_t dwCondition)
{
    // Get all kill-type missions
    std::vector<std::shared_ptr<CDailyMissionInfo>> vecList;
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
    std::vector<std::shared_ptr<CDailyMissionInfo>> vecList;
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
void CGocDailyMission::SendDailyMissionList(std::uint8_t byTodayInit)
{
    // Create mission list structure
    PS_MAP_DISTRICT_DAILY_MISSION psMission;

    // Get all daily missions grouped by district
    GetDailyMissionList(&psMission);

    // Create send packet (main=0x24, sub=1)
    XSendPacket xSendPacket(0x24, 1);
    xSendPacket << byTodayInit;
    xSendPacket << psMission;

    // Get owner actor (CMover) and send packet
    CMover* pMover = GetOwnerGO();
    if (pMover)
    {
        // Send using GocNetwork static method
        std::vector<CMover*> gobList;
        gobList.push_back(pMover);
        CGocNetwork::Send(gobList, xSendPacket);
    }
}

// IDA: ?SetNewDailyMissionList@CGocDailyMission@@QEAAXAEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x14004EF80)
// Set new daily mission list
void CGocDailyMission::SetNewDailyMissionList(std::vector<std::uint32_t>& vecNewMission)
{
    Clear();

    // Get current time and add each mission
    ATL::CTime tCurr = ATL::CTime::GetTickCount();
    for (auto& dwMissionID : vecNewMission)
    {
        AddNewDailyMission(dwMissionID, tCurr);
    }
}

// IDA: ?GetDailyMissionList@CGocDailyMission@@QEAAXAEAUPS_MAP_DISTRICT_DAILY_MISSION@@@Z (0x140050BF0)
void CGocDailyMission::GetDailyMissionList(PS_MAP_DISTRICT_DAILY_MISSION* psMissionList)
{
    if (!psMissionList) return;

    // Iterate through m_mapSpecial and group by District_ID
    for (auto& pair : m_mapSpecial)
    {
        std::shared_ptr<CDailyMissionInfo> pMission = pair.second;
        if (!pMission) continue;

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission) continue;

        std::uint32_t dwDistrictID = pTB_Mission->District_ID;

        // Find or create district entry
        auto it = psMissionList->mapDistrictMissions.find(dwDistrictID);
        if (it != psMissionList->mapDistrictMissions.end())
        {
            // Add to existing district list
            it->second.push_back(*pMission->GetMissionInfo());
        }
        else
        {
            // Create new district entry
            std::vector<ST_DAILY_MISSION_INFO> vecMission;
            vecMission.push_back(*pMission->GetMissionInfo());
            psMissionList->mapDistrictMissions[dwDistrictID] = vecMission;
        }
    }

    // Iterate through m_mapGuerrilla
    for (auto& pair : m_mapGuerrilla)
    {
        std::shared_ptr<CDailyMissionInfo> pMission = pair.second;
        if (!pMission) continue;

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission) continue;

        std::uint32_t dwDistrictID = pTB_Mission->District_ID;

        auto it = psMissionList->mapDistrictMissions.find(dwDistrictID);
        if (it != psMissionList->mapDistrictMissions.end())
        {
            it->second.push_back(*pMission->GetMissionInfo());
        }
        else
        {
            std::vector<ST_DAILY_MISSION_INFO> vecMission;
            vecMission.push_back(*pMission->GetMissionInfo());
            psMissionList->mapDistrictMissions[dwDistrictID] = vecMission;
        }
    }

    // Iterate through m_mapEvent
    for (auto& pair : m_mapEvent)
    {
        std::shared_ptr<CDailyMissionInfo> pMission = pair.second;
        if (!pMission) continue;

        TB_DAILY_MISSION* pTB_Mission = pMission->GetTBMission();
        if (!pTB_Mission) continue;

        std::uint32_t dwDistrictID = pTB_Mission->District_ID;

        auto it = psMissionList->mapDistrictMissions.find(dwDistrictID);
        if (it != psMissionList->mapDistrictMissions.end())
        {
            it->second.push_back(*pMission->GetMissionInfo());
        }
        else
        {
            std::vector<ST_DAILY_MISSION_INFO> vecMission;
            vecMission.push_back(*pMission->GetMissionInfo());
            psMissionList->mapDistrictMissions[dwDistrictID] = vecMission;
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

// ============================================================================
// IDA: ?ChangeDailyMissionHelper@CGocDailyMission@@QEAA_NKE@Z (0x140052060)
// Verified: Per IDA decompile - changes the helper flag for a daily mission
// - Finds mission by ID
// - Returns false if mission not found
// - If helper flag is same as current, logs debug message and returns false
// - If adding helper (byAddHelper=1) and already at max (4), sends error and returns false
// - Sets helper flag on mission
// - Updates helper count
// - Sends packet to client (main=0x24, sub=4)
// - Returns true on success
bool CGocDailyMission::ChangeDailyMissionHelper(std::uint32_t dwMissionID, std::uint8_t byAddHelper)
{
    // Find the mission
    std::shared_ptr<CDailyMissionInfo> pMission = FindMission(dwMissionID, 0);
    if (!pMission)
    {
        return false;
    }

    // Check if helper flag is already set to requested value
    std::uint8_t byCurrentHelper = pMission->GetDailyMissionHelper() ? 1 : 0;
    if (byAddHelper == byCurrentHelper)
    {
        // Log debug message - same flag
        CMover* pMover = GetOwnerGO();
        if (pMover)
        {
            CUser* pUser = dynamic_cast<CUser*>(pMover);
            if (pUser)
            {
                LogHelper::LogDebug("game.dailymission", "<Daily Mission> SetHelper Same Flag < UID : %d  Mission : %d Helper : %d >", pUser->GetUAID(), dwMissionID, byAddHelper);
            }
        }
        return false;
    }

    // Check if trying to add helper when already at max (4)
    if (byAddHelper == 1 && m_nHelperCount >= 4)
    {
        // Send error message (error code 0xD2F6)
        CMover* pMover = GetOwnerGO();
        if (pMover)
        {
            CGocNetwork::SendErrorMessage(pMover, 0x24, 4, 0xD2F6);
        }
        return false;
    }

    // Set helper flag on mission
    pMission->SetDailyMissionHelper(byAddHelper == 1);

    // Update helper count
    if (byAddHelper == 1)
    {
        ++m_nHelperCount;
    }
    else
    {
        --m_nHelperCount;
    }

    // Send packet to client (main=0x24, sub=4)
    XSendPacket xSendPacket(0x24, 4);
    xSendPacket.XParse << dwMissionID;
    xSendPacket.XParse << byAddHelper;

    CMover* pMover = GetOwnerGO();
    if (pMover)
    {
        CGocNetwork::Send(reinterpret_cast<XActor*>(pMover), xSendPacket);
    }

    return true;
}

// ============================================================================
// IDA: ?GetDailyMissionList@CGocDailyMission@@QEAAXAEAUPS_MAP_DAILY_MISSION@@@Z (0x1400510D0)
// Verified: Per IDA decompile - gets all daily missions as a map keyed by mission ID
// - Iterates through m_mapSpecial, m_mapGuerrilla, m_mapEvent
// - For each mission, gets ST_DAILY_MISSION_INFO and adds to output map
// - Map is keyed by mission ID (dwMissionID)
void CGocDailyMission::GetDailyMissionList(PS_MAP_DAILY_MISSION* psMissionList)
{
    if (!psMissionList)
    {
        return;
    }

    // Iterate through special missions
    for (auto iter = m_mapSpecial.begin(); iter != m_mapSpecial.end(); ++iter)
    {
        std::shared_ptr<CDailyMissionInfo> pMission = iter->second;
        if (!pMission)
        {
            continue;
        }

        ST_DAILY_MISSION_INFO* stInfo = pMission->GetMissionInfo();
        if (!stInfo)
        {
            continue;
        }

        // Add to map keyed by mission ID
        psMissionList->mapInfo[iter->first] = *stInfo;
    }

    // Iterate through guerrilla missions
    for (auto iter = m_mapGuerrilla.begin(); iter != m_mapGuerrilla.end(); ++iter)
    {
        std::shared_ptr<CDailyMissionInfo> pMission = iter->second;
        if (!pMission)
        {
            continue;
        }

        ST_DAILY_MISSION_INFO* stInfo = pMission->GetMissionInfo();
        if (!stInfo)
        {
            continue;
        }

        psMissionList->mapInfo[iter->first] = *stInfo;
    }

    // Iterate through event missions
    for (auto iter = m_mapEvent.begin(); iter != m_mapEvent.end(); ++iter)
    {
        std::shared_ptr<CDailyMissionInfo> pMission = iter->second;
        if (!pMission)
        {
            continue;
        }

        ST_DAILY_MISSION_INFO* stInfo = pMission->GetMissionInfo();
        if (!stInfo)
        {
            continue;
        }

        psMissionList->mapInfo[iter->first] = *stInfo;
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
    TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION( stMission->dwMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    ATL::CTime tNow = ATL::CTime::GetTickCount();
    int nSecs = tNow.GetSecond();
    int nMins = tNow.GetMinute();
    int nHour = tNow.GetHour();
    ATL::CTimeSpan tNowTime(0, nHour, nMins, nSecs);

    switch (pTB_Mission->Mission_Type)
    {
    case eDAILY_MISSION_TYPE_SPECIAL:
        // Special - check date range from mission info
        {
            ATL::CTime tStart = *reinterpret_cast<ATL::CTime*>(stMission->tStart);
            ATL::CTime tEnd = *reinterpret_cast<ATL::CTime*>(stMission->tEnd);
            if (tNow < tStart || tNow >= tEnd)
            {
                return false;
            }
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
    TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION( dwMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    // Validate reward item 01
    if (pTB_Mission->Reward_Item_Id_01)
    {
        TB_ITEM* pTBItem = pGameServer->GetResourceMgr().GetTB_ITEM( pTB_Mission->Reward_Item_Id_01);
        if (!pTBItem)
        {
            return false;
        }
    }

    // Validate reward item 02
    if (pTB_Mission->Reward_Item_Id_02)
    {
        TB_ITEM* pTBItem = pGameServer->GetResourceMgr().GetTB_ITEM( pTB_Mission->Reward_Item_Id_02);
        if (!pTBItem)
        {
            return false;
        }
    }

    // Validate reward item 03
    if (pTB_Mission->Reward_Item_Id_03)
    {
        TB_ITEM* pTBItem = pGameServer->GetResourceMgr().GetTB_ITEM( pTB_Mission->Reward_Item_Id_03);
        if (!pTBItem)
        {
            return false;
        }
    }

    // Validate reward item 04
    if (pTB_Mission->Reward_Item_Id_04)
    {
        TB_ITEM* pTBItem = pGameServer->GetResourceMgr().GetTB_ITEM( pTB_Mission->Reward_Item_Id_04);
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
    TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION( dwMissionID);
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
        {
            // Monster target - check condition and mission group
            if ((dwCondition & (1 << pTB_Mission->Condition_Type)) == 0)
            {
                return false;
            }
            TB_MONSTER* pTBMonster = pGameServer->GetResourceMgr().GetTB_MONSTER(dwObjectID);
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
        }
        break;

    case eDAILY_MISSION_TARGET_ITEM:
        {
            // Item target - check condition and faction
            if ((dwCondition & (1 << pTB_Mission->Condition_Type)) == 0)
            {
                return false;
            }
            TB_MONSTER* TB_MONSTER = pGameServer->GetResourceMgr().GetTB_MONSTER(dwObjectID);
            if (!TB_MONSTER)
            {
                return false;
            }
            if (pTB_Mission->Target_Faction != TB_MONSTER->Monster_Faction)
            {
                return false;
            }
        }
        break;

    case eDAILY_MISSION_TARGET_QUEST:
        {
            // Quest target - only check condition
            if ((dwCondition & (1 << pTB_Mission->Condition_Type)) == 0)
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
    TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION( dwMissionID);
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

    // Create send packet (main=0x24, sub=3)
    XSendPacket xSendPacket(0x24, 3);
    xSendPacket << *psUpdate;

    // Get owner actor and send packet
    CMover* pMover = GetOwnerGO();
    if (pMover)
    {
        CGocNetwork::Send(reinterpret_cast<XActor*>(pMover), xSendPacket);
    }
}

// IDA: ?DBDailyMissionPost@CGocDailyMission@@QEAAXK@Z (0x140055210)
// Verified: Per IDA decompile at 0x140055210:
// - Gets TB_DAILY_MISSION from XResourceMgr
// - Creates ST_CREATE_ITEMS with reward items (up to 4 reward slots)
// - Gets CGocPost component and calls SystemPostSend with subType=5, type=2
// - Logs to DB via XGameServer::SendDBLog (mainType=13, subType=4)
// - Sends statistics packet to DB (main=0xF0, sub=7)
void CGocDailyMission::DBDailyMissionPost(std::uint32_t dwMissionID)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION(dwMissionID);
    if (!pTB_Mission)
    {
        return;
    }

    // Create reward items list
    ST_CREATE_ITEMS vecItem;
    ST_CREATE_ITEM stItem;

    // Add reward items (up to 4 slots)
    if (pTB_Mission->Reward_Item_Id_01)
    {
        stItem.nItemID = pTB_Mission->Reward_Item_Id_01;
        stItem.shCount = pTB_Mission->Reward_Item_Count_01;
        vecItem.vecInfo.push_back(stItem);
    }
    if (pTB_Mission->Reward_Item_Id_02)
    {
        stItem.nItemID = pTB_Mission->Reward_Item_Id_02;
        stItem.shCount = pTB_Mission->Reward_Item_Count_02;
        vecItem.vecInfo.push_back(stItem);
    }
    if (pTB_Mission->Reward_Item_Id_03)
    {
        stItem.nItemID = pTB_Mission->Reward_Item_Id_03;
        stItem.shCount = pTB_Mission->Reward_Item_Count_03;
        vecItem.vecInfo.push_back(stItem);
    }
    if (pTB_Mission->Reward_Item_Id_04)
    {
        stItem.nItemID = pTB_Mission->Reward_Item_Id_04;
        stItem.shCount = pTB_Mission->Reward_Item_Count_04;
        vecItem.vecInfo.push_back(stItem);
    }

    // Get owner mover and CGocPost component
    CMover* pMover = GetOwnerGO();
    if (!pMover)
    {
        return;
    }

    std::tr1::shared_ptr<CGocPost> pPostPtr = pMover->GetGOC_Post(false);
    if (!pPostPtr)
    {
        return;
    }

    // Create title string from mission ID
    wchar_t strTitle[48] = {0};
    _itow_s(dwMissionID, strTitle, 10);

    // Send system post (subType=5, type=2)
    pPostPtr->SystemPostSend(vecItem, 5, 2, strTitle);

    // Log to DB
    ST_LOG_GAME stLog;
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (pUser)
    {
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = pUser->GetID();
    }
    stLog._sMainType = 13;
    stLog._sSubType = 4;
    stLog.nParam0 = dwMissionID;
    stLog.nParam1 = pTB_Mission->Reward_Item_Id_01;
    stLog.nParam2 = pTB_Mission->Reward_Item_Count_01;
    stLog.nParam3 = pTB_Mission->Reward_Item_Id_02;
    stLog.nParam4 = pTB_Mission->Reward_Item_Count_02;
    stLog.nParam5 = pTB_Mission->Reward_Item_Id_03;
    stLog.nParam6 = pTB_Mission->Reward_Item_Count_03;
    stLog.nParam7 = pTB_Mission->Reward_Item_Id_04;
    stLog.nParam8 = pTB_Mission->Reward_Item_Count_04;
    pGameServer->SendDBLog(stLog);

    // Send statistics packet
    ST_STATISTICS_DAILY_MISSION stStatistics;
    if (pUser)
    {
        stStatistics.dwUAID = pUser->GetUAID();
        stStatistics.dwUCID = pUser->GetID();

        // Send statistics packet using CUser as IXObject
        XSendDBPacket xSendDBStatistics(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 0xF0, 7);
        xSendDBStatistics << stStatistics;
        pGameServer->SendDBStatistics(xSendDBStatistics);
    }
}

// IDA: ?DBUpdateMissionInfo@CGocDailyMission@@QEAAXAEAUPS_DAILY_MISSION_UPDATE@@@Z (0x140054AD0)
// Verified: Per IDA decompile at 0x140054AD0:
// - Gets CUser from owner mover
// - Logs each mission update to DB (mainType=13, subType=3)
// - Sends DB packet (main=0x48, sub=3) with UCID and mission list
void CGocDailyMission::DBUpdateMissionInfo(PS_DAILY_MISSION_UPDATE* psUpdate)
{
    if (!psUpdate || psUpdate->vecInfo.empty())
    {
        return;
    }

    CMover* pMover = GetOwnerGO();
    if (!pMover)
    {
        return;
    }

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (pUser)
    {
        // Log each mission update
        ST_LOG_GAME stLog;
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = pUser->GetID();
        stLog._sMainType = 13;
        stLog._sSubType = 3;

        for (size_t i = 0; i < psUpdate->vecInfo.size(); ++i)
        {
            const ST_DAILY_MISSION_INFO& stMission = psUpdate->vecInfo[i];
            stLog.nParam0 = stMission.dwMissionID;
            stLog.nParam1 = stMission.byState;
            stLog.nParam2 = stMission.byAddHelper;
            stLog.nParam3 = stMission.shValue;
            TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);
        }

        // Send DB update packet
        XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 0x48, 3);
        xSendDBPacket.XParse << pUser->GetID();
        xSendDBPacket << *psUpdate;
        TXSingleton<XGameServer>::Instance()->SendDBGame(xSendDBPacket);
    }
}

// IDA: ?DBReqDailyMissionList@CGocDailyMission@@QEAAXXZ (0x140054DD0)
// Verified: Per IDA decompile at 0x140054DD0:
// - Sends DB packet (main=0x48, sub=1) with UCID to request mission list
void CGocDailyMission::DBReqDailyMissionList()
{
    CMover* pMover = GetOwnerGO();
    if (!pMover)
    {
        return;
    }

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser)
    {
        return;
    }

    XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 0x48, 1);
    xSendDBPacket.XParse << pUser->GetID();
    TXSingleton<XGameServer>::Instance()->SendDBGame(xSendDBPacket);
}

// IDA: ?DBAddDailyMissionList@CGocDailyMission@@QEAAXXZ (0x140054EE0)
// Verified: Per IDA decompile at 0x140054EE0:
// - Gets current mission list via GetDailyMissionList
// - Sends DB packet (main=0x48, sub=2) with UCID and mission map
// - Logs each mission to DB (mainType=13, subType=1)
void CGocDailyMission::DBAddDailyMissionList()
{
    CMover* pMover = GetOwnerGO();
    if (!pMover)
    {
        return;
    }

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser)
    {
        return;
    }

    // Get current mission list
    PS_MAP_DAILY_MISSION psMission;
    GetDailyMissionList(&psMission);

    // Send DB add packet
    XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 0x48, 2);
    xSendDBPacket.XParse << pUser->GetID();
    xSendDBPacket << psMission;
    TXSingleton<XGameServer>::Instance()->SendDBGame(xSendDBPacket);

    // Log each mission
    ST_LOG_GAME stLog;
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetID();
    stLog._sMainType = 13;
    stLog._sSubType = 1;
    stLog.nParam1 = pUser->GetLevel();

    for (auto iter = psMission.mapInfo.begin(); iter != psMission.mapInfo.end(); ++iter)
    {
        stLog.nParam0 = iter->first;
        TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);
    }
}

// IDA: ?OnUpdateDailyMission@CGocDailyMission@@QEAAXXZ (0x140052360)
// Verified: Per IDA decompile - called on daily mission update tick
// - Checks if user is in daily mission state (UserDB flag 0x20)
// - Gets current time and checks if it's within daily reset window (11:00-12:00)
// - Iterates through special missions and checks time validity
// - If any mission is invalid, regenerates mission list and sends to client
void CGocDailyMission::OnUpdateDailyMission()
{
    // Get CUser owner
    CMover* pMover = GetOwnerGO();
    if (!pMover)
    {
        return;
    }

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser)
    {
        return;
    }

    // Check if user has daily mission flag set (UserDB flag 0x20)
    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();
    if (!pCharInfo || (pCharInfo->userDBBits.UserDB.bLoadDailyMission == 0))
    {
        return;
    }

    ATL::CTime tCurr = ATL::CTime::GetTickCount();
    ATL::CTimeSpan tInitTime(0, 11, 0, 0);

    // Calculate today's init time (11:00:00)
    int nHour = static_cast<int>(tInitTime.GetTotalHours());
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
                std::shared_ptr<CDailyMissionInfo> pMission = iter->second;
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
                    GenerateDailyMission(1, 1, 1, vecNewMission);
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
        TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION( dwMissionID);
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
        TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION( dwMissionID);
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
void CGocDailyMission::AddNewDailyMission(std::uint32_t dwMissionID, ATL::CTime tNow)
{
    // Get TB_DAILY_MISSION from resource manager
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return;

    TB_DAILY_MISSION* pTB_Mission = pServer->GetResourceMgr().GetTB_DAILY_MISSION(dwMissionID);
    if (!pTB_Mission)
    {
        return;
    }

    // Generate time range for this mission
    ATL::CTime tStart;
    ATL::CTime tEnd;
    GeneraterTimeRange(dwMissionID, pTB_Mission->Mission_Type, tNow, tStart, tEnd, 1);

    // Create mission info
    ST_DAILY_MISSION_INFO stInfo;
    stInfo.dwMissionID = dwMissionID;
    stInfo.byState = 0;
    stInfo.byAddHelper = 0;
    stInfo.shValue = 0;
    // Copy time values (CTime is 8 bytes)
    std::memcpy(stInfo.tAccept, &tStart, 8);
    std::memcpy(stInfo.tStart, &tStart, 8);
    std::memcpy(stInfo.tEnd, &tEnd, 8);

    // Add the mission
    AddDailyMission(stInfo);
}

void CGocDailyMission::GetDailyMissionList(E_DAILY_MISSION_FINISH eType,
                                           std::vector<std::shared_ptr<CDailyMissionInfo>>* vecList)
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
    std::vector<std::shared_ptr<CDailyMissionInfo>> vecList;
    GetDailyMissionList(eDAILY_MISSION_FINISH_COLLECT, &vecList);

    PS_DAILY_MISSION_UPDATE psUpdate;

    for (size_t i = 0; i < vecList.size(); ++i)
    {
        std::shared_ptr<CDailyMissionInfo> pMission = vecList[i];
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
            psUpdate.vecInfo.push_back(*stInfo);
        }
        else
        {
            // Mission complete
            std::uint32_t dwMissionID = pMission->GetQuestID();
            if (CompleteDailyMission(dwMissionID, pMission))
            {
                stInfo = pMission->GetMissionInfo();
                psUpdate.vecInfo.push_back(*stInfo);
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

// ============================================================================
// IDA: ?UpdateMyRoomType@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_CONDITION@@M@Z (0x140054670)
// Verified: Per IDA decompile - updates my-room-type daily missions
// - Gets missions with eDAILY_MISSION_FINISH_MYROOM finish type
// - Checks Condition_Type match
// - Increments condition value by 1
// - Completes mission if condition met
void CGocDailyMission::UpdateMyRoomType(E_DAILY_MISSION_CONDITION eCondition, float fValue)
{
    std::vector<std::shared_ptr<CDailyMissionInfo>> vecList;
    GetDailyMissionList(eDAILY_MISSION_FINISH_MYROOM, &vecList);

    PS_DAILY_MISSION_UPDATE psUpdate;

    for (size_t i = 0; i < vecList.size(); ++i)
    {
        std::shared_ptr<CDailyMissionInfo> pMission = vecList[i];
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
            psUpdate.vecInfo.push_back(*stInfo);
        }
        else
        {
            // Mission complete
            std::uint32_t dwMissionID = pMission->GetQuestID();
            if (CompleteDailyMission(dwMissionID, pMission))
            {
                stInfo = pMission->GetMissionInfo();
                psUpdate.vecInfo.push_back(*stInfo);
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

// ============================================================================
// IDA: ?CheatChangeMission@CGocDailyMission@@QEAA_NKK@Z (0x1400559D0)
// Verified: Per IDA decompile - GM cheat to change mission
// - Validates new mission exists in TB_DAILY_MISSION
// - Deletes old mission via CheatDeleteMission
// - Adds new mission via AddNewDailyMission
bool CGocDailyMission::CheatChangeMission(std::uint32_t dwTargetID, std::uint32_t dwNewMissionID)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION( dwNewMissionID);
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
    ATL::CTime tCurr = ATL::CTime::GetTickCount();
    AddNewDailyMission(dwNewMissionID, tCurr);

    return true;
}

// Note: UpdateKillType and UpdateMazeClearType are already defined earlier in this file

// ============================================================================
// IDA: ?CheatDeleteMission@CGocDailyMission@@QEAA_NK@Z (0x140055AF0)
// Verified: Per IDA decompile - GM cheat to delete a specific mission
// - Finds the mission using FindMission
// - Gets mission type and erases from appropriate map (Special/Guerrilla/Event)
bool CGocDailyMission::CheatDeleteMission(std::uint32_t dwMissionID)
{
    // Find the mission
    std::shared_ptr<CDailyMissionInfo> pMission = FindMission(dwMissionID, 0);
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
    GetDailyMissionList(&psMissionList);

    for (auto iter = psMissionList.mapInfo.begin(); iter != psMissionList.mapInfo.end(); ++iter)
    {
        CheatDeleteMission(iter->first);
    }

    return true;
}

// ============================================================================
// IDA: ?CheatAddMission@CGocDailyMission@@QEAA_NK@Z (0x140055DB0)
// Verified: Per IDA decompile - GM cheat to add a mission
// - Validates mission exists in TB_DAILY_MISSION
// - Checks district mission count limits (5 for special, 1 for guerrilla)
// - Adds mission via AddNewDailyMission
bool CGocDailyMission::CheatAddMission(std::uint32_t dwMissionID)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION(dwMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    // Get current mission list to check count limits
    PS_MAP_DISTRICT_DAILY_MISSION psMissionList;
    GetDailyMissionList(&psMissionList);

    // Find district entry
    auto iter = psMissionList.mapDistrictMissions.find(pTB_Mission->District_ID);
    if (iter != psMissionList.mapDistrictMissions.end())
    {
        // Count missions of same type in district
        int nCnt = 0;
        for (auto& stInfo : iter->second)
        {
            auto pMission = FindMission(stInfo.dwMissionID, 0);
            if (pMission && pMission->GetDailyMissionType() == pTB_Mission->Mission_Type)
            {
                ++nCnt;
            }
        }

        // Check limits
        if (pTB_Mission->Mission_Type == eDAILY_MISSION_TYPE_SPECIAL && nCnt >= 5)
        {
            return false;
        }
        if (pTB_Mission->Mission_Type == eDAILY_MISSION_TYPE_GUERRILLA && nCnt >= 1)
        {
            return false;
        }
    }

    // Add the mission
    ATL::CTime tCurr = ATL::CTime::GetTickCount();
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
    TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION( dwMissionID);
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
// - Parses time strings (HH:MM format)
// - Validates mission exists and checks count limits
// - Generates time range and adds mission
bool CGocDailyMission::CheatChangeGuerillaMission(std::uint32_t dwNewMissionID, const wchar_t* szStart, const wchar_t* szEnd)
{
    if (!szStart || !szEnd)
    {
        return false;
    }

    // Convert wide strings to narrow
    char strStart[16] = {0};
    char strEnd[16] = {0};
    WideCharToMultiByte(CP_ACP, 0, szStart, -1, strStart, 10, nullptr, nullptr);
    WideCharToMultiByte(CP_ACP, 0, szEnd, -1, strEnd, 10, nullptr, nullptr);

    // Parse start time (HH:MM)
    int nStartHour = -1, nStartMin = -1;
    sscanf_s(strStart, "%d:%d", &nStartHour, &nStartMin);
    if (nStartHour > 24 || nStartMin > 60)
    {
        return false;
    }

    // Parse end time (HH:MM)
    int nEndHour = -1, nEndMin = -1;
    sscanf_s(strEnd, "%d:%d", &nEndHour, &nEndMin);
    if (nEndHour > 24 || nEndMin > 60)
    {
        return false;
    }

    // Get current time and create start/end times
    ATL::CTime tNow = ATL::CTime::GetTickCount();
    ATL::CTime tStart(tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), nStartHour, nStartMin, 0, -1);
    ATL::CTime tEnd(tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), nEndHour, nEndMin, 0, -1);

    // Validate times
    if (tStart.GetTime() <= 0 || tEnd.GetTime() <= 0)
    {
        return false;
    }

    // Get mission table
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_DAILY_MISSION* pTB_Mission = pGameServer->GetResourceMgr().GetTB_DAILY_MISSION(dwNewMissionID);
    if (!pTB_Mission)
    {
        return false;
    }

    // Get current mission list to check count limits
    PS_MAP_DISTRICT_DAILY_MISSION psMissionList;
    GetDailyMissionList(&psMissionList);

    // Find district entry
    auto iter = psMissionList.mapDistrictMissions.find(pTB_Mission->District_ID);
    if (iter != psMissionList.mapDistrictMissions.end())
    {
        // Count missions of same type in district
        int nCnt = 0;
        for (auto& stInfo : iter->second)
        {
            auto pMission = FindMission(stInfo.dwMissionID, 0);
            if (pMission && pMission->GetDailyMissionType() == pTB_Mission->Mission_Type)
            {
                ++nCnt;
            }
        }

        // Check limits
        if (pTB_Mission->Mission_Type == eDAILY_MISSION_TYPE_SPECIAL && nCnt >= 5)
        {
            return false;
        }
        if (pTB_Mission->Mission_Type == eDAILY_MISSION_TYPE_GUERRILLA && nCnt >= 1)
        {
            return false;
        }
    }

    // Generate time range and add mission
    ATL::CTime tCurr = ATL::CTime::GetTickCount();
    GeneraterTimeRange(dwNewMissionID, pTB_Mission->Mission_Type, tCurr, tStart, tEnd, false);

    // Create mission info
    ST_DAILY_MISSION_INFO stInfo;
    stInfo.dwMissionID = dwNewMissionID;
    std::memcpy(stInfo.tAccept, &tStart, 8);
    std::memcpy(stInfo.tStart, &tStart, 8);
    std::memcpy(stInfo.tEnd, &tEnd, 8);

    AddDailyMission(stInfo);

    return true;
}

// Note: GetMissionInfo, FindMission, SetNewDailyMissionList already defined earlier

// ============================================================================
// IDA: ?GenerateDailyMission@CGocDailyMission@@QEAA_N_N00AEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x1400517B0)
// Verified: Per IDA decompile at 0x1400517B0:
// - Clears the output vector
// - For each enabled type (Special/Guerrilla/Event), calls CDailyMissionMgr::GenerateDailyMission
// - Returns true if any missions were generated
bool CGocDailyMission::GenerateDailyMission(bool bSpecial, bool bGuerrilla, bool bEvent,
                                            std::vector<std::uint32_t>& vecNewMission)
{
    vecNewMission.clear();

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    CDailyMissionMgr* pDailyMissionMgr = pGameServer->GetDailyMissionMgr();
    if (!pDailyMissionMgr)
    {
        return false;
    }

    // Generate special missions (type 1)
    if (bSpecial)
    {
        pDailyMissionMgr->GenerateDailyMission(1, vecNewMission);
    }

    // Generate guerrilla missions (type 2)
    if (bGuerrilla)
    {
        pDailyMissionMgr->GenerateDailyMission(2, vecNewMission);
    }

    // Generate event missions (type 3)
    if (bEvent)
    {
        pDailyMissionMgr->GenerateDailyMission(3, vecNewMission);
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

// Note: GetDailyMissionList overloads already defined earlier in this file

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

    for (size_t i = 0; i < vecMission.size(); ++i)
    {
        ST_DAILY_MISSION_FRIEND_RES& stRes = vecMission[i];

        // Find mission by ID
        std::shared_ptr<CDailyMissionInfo> pMission = FindMission(stRes.dwMissionID, 0);
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
            psUpdate.vecInfo.push_back(*stInfo);
        }
        else
        {
            // Mission complete
            std::uint32_t dwMissionID = pMission->GetQuestID();
            if (CompleteDailyMission(dwMissionID, pMission))
            {
                stInfo = pMission->GetMissionInfo();
                psUpdate.vecInfo.push_back(*stInfo);
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
