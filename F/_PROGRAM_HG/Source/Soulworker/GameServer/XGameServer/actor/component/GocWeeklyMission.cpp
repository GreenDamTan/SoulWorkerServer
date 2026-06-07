#include "GocWeeklyMission.h"
#include "Soulworker/GameServer/XGameServer/XGameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XGameServer/Actor/CUser.h"
#include "Soulworker/GameServer/XGameServer/Actor/CMover.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/GameServer/XCore/XServer/XSendDBPacket.h"
#include "Soulworker/GameServer/XCore/XServer/LogHelper.h"
#include <ATL/CTime.h>
#include <tr1/memory>

// TODO: 需要人工审查 - 需要定义 CWeeklyMission_Group 和 CWeeklyMission_Day 类
// 这些类在 IDA 中有定义，需要单独还原

// ============================================================================
// CGocWeeklyMission Implementation
// ============================================================================

// IDA: ??0CGocWeeklyMission@@QEAA@XZ (0x14017D160)
// Constructor
CGocWeeklyMission::CGocWeeklyMission()
    : GOComponent()
    , m_bLoad(false)
{
    // IDA: calls GOComponent::GOComponent(this) and initializes m_mapWeekMissionGroup
    // The map is default-constructed
}

// IDA: ??1CGocWeeklyMission@@UEAA@XZ (0x14017D1F0)
// Verified: Direct IDA decompilation - destructor
CGocWeeklyMission::~CGocWeeklyMission()
{
    // IDA: 设置虚表指针
    // IDA: 析构 m_mapWeekMissionGroup
    // IDA: 调用基类析构函数
    m_mapWeekMissionGroup.clear();
}

// IDA: ?GetFamilyID@CGocWeeklyMission@@SAHXZ (0x140039070)
// Returns the component family ID
int CGocWeeklyMission::GetFamilyID()
{
    // IDA: simply returns 25
    return 25;
}

// IDA: ?Init@CGocWeeklyMission@@QEAA_NXZ (0x14017D240)
// Initialize the component
bool CGocWeeklyMission::Init()
{
    // IDA: calls Clear() and returns true
    Clear();
    return true;
}

// IDA: ?Clear@CGocWeeklyMission@@QEAAXXZ (0x14017D260)
// Clear all weekly mission data
void CGocWeeklyMission::Clear()
{
    // IDA: sets m_bLoad = 0, iterates through m_mapWeekMissionGroup,
    // calls Clear() on each valid CWeeklyMission_Group, then clears the map

    m_bLoad = false;

    for (auto it = m_mapWeekMissionGroup.begin(); it != m_mapWeekMissionGroup.end(); ++it)
    {
        if (it->second)
        {
            // TODO: 需要人工审查 - CWeeklyMission_Group::Clear 需要实现
            // it->second->Clear();
        }
    }

    m_mapWeekMissionGroup.clear();
}

// IDA: ?GetWeeklyMissionGroupID@CGocWeeklyMission@@QEAAX_N000AEAV?$set@EU?$less@E@std@@V?$allocator@E@2@@std@@@Z (0x14017D310)
// Get weekly mission group IDs based on character/account flags
void CGocWeeklyMission::GetWeeklyMissionGroupID(bool bNewCharacter, bool bComeBackCharacter,
                                                 bool bNewAccount, bool bComeBackAccount,
                                                 std::set<uint8_t>& setGroupList)
{
    // IDA: Gets weekly event group IDs from XResourceMgr, then filters based on
    // Event_Type and Check_Mission_Type

    auto* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    std::list<uint8_t> listDBTable;
    XResourceMgr::GetWeeklyEventGroupID(&pGameServer->m_xResourceMgr, listDBTable);

    if (listDBTable.empty())
        return;

    for (uint8_t groupId : listDBTable)
    {
        TB_WEEK_GROUP* pTB_WEEK_GROUP = XResourceMgr::GetTB_WEEK_GROUP(&pGameServer->m_xResourceMgr, groupId);
        if (!pTB_WEEK_GROUP)
        {
            LogHelper::LogError("game.contents",
                "<GetWeeklyMissionGroupID> TB_WEEK_GROUP is NULL < GroupID:%d >", groupId);
            continue;
        }

        switch (pTB_WEEK_GROUP->Event_Type)
        {
        case 1: // New character/account event
            if (pTB_WEEK_GROUP->Check_Mission_Type == 1)
            {
                if (bNewCharacter)
                    setGroupList.insert(pTB_WEEK_GROUP->mission_group);
            }
            else if (pTB_WEEK_GROUP->Check_Mission_Type == 2)
            {
                if (bNewAccount)
                    setGroupList.insert(pTB_WEEK_GROUP->mission_group);
            }
            else
            {
                LogHelper::LogError("game.contents",
                    "<GetWeeklyMissionGroupID> TB_WEEK_GROUP is NULL < GroupID:%d, Check:%d, Type:%d >",
                    groupId, pTB_WEEK_GROUP->Check_Mission_Type, pTB_WEEK_GROUP->Event_Type);
            }
            break;

        case 2: // Time-based event
            {
                ATL::CTime tNowDate = ATL::CTime::GetTickCount();
                int nDay = tNowDate.GetDay();
                int nMonth = tNowDate.GetMonth();
                int nYear = tNowDate.GetYear();

                ATL::CTime tTodayDate(nYear, nMonth, nDay, 0, 0, 0, -1);
                ATL::CTime tStartDate(pTB_WEEK_GROUP->Event_Start_Year, pTB_WEEK_GROUP->Event_Start_Month,
                                      pTB_WEEK_GROUP->Event_Start_Day, 0, 0, 0, -1);
                ATL::CTime tEndDate(pTB_WEEK_GROUP->Event_End_Year, pTB_WEEK_GROUP->Event_End_Month,
                                    pTB_WEEK_GROUP->Event_End_Day, 0, 0, 0, -1);

                if (tStartDate <= tTodayDate && tTodayDate <= tEndDate)
                {
                    setGroupList.insert(pTB_WEEK_GROUP->mission_group);
                }
            }
            break;

        case 3: // Comeback character/account event
            if (pTB_WEEK_GROUP->Check_Mission_Type == 1)
            {
                if (bComeBackCharacter)
                    setGroupList.insert(pTB_WEEK_GROUP->mission_group);
            }
            else if (pTB_WEEK_GROUP->Check_Mission_Type == 2)
            {
                if (bComeBackAccount)
                    setGroupList.insert(pTB_WEEK_GROUP->mission_group);
            }
            else
            {
                LogHelper::LogError("game.contents",
                    "<GetWeeklyMissionGroupID> TB_WEEK_GROUP is NULL < GroupID:%d, Check:%d, Type:%d >",
                    groupId, pTB_WEEK_GROUP->Check_Mission_Type, pTB_WEEK_GROUP->Event_Type);
            }
            break;

        default:
            LogHelper::LogError("game.contents",
                "<GetWeeklyMissionGroupID> TB_WEEK_GROUP is NULL < GroupID:%d, Type:%d >",
                groupId, pTB_WEEK_GROUP->Event_Type);
            break;
        }
    }
}

// IDA: ?GetWeekMission@CGocWeeklyMission@@QEAAXAEAV?$vector@UST_WEEKLY_MISSION_GROUP_INFO@@V?$allocator@UST_WEEKLY_MISSION_GROUP_INFO@@@std@@@std@@@Z (0x14017D7C0)
// Verified: Direct IDA decompilation - get all weekly mission group info
void CGocWeeklyMission::GetWeekMission(std::vector<ST_WEEKLY_MISSION_GROUP_INFO>& vecGroupInfo)
{
    for (auto it = m_mapWeekMissionGroup.begin(); it != m_mapWeekMissionGroup.end(); ++it)
    {
        if (it->second)
        {
            ST_WEEKLY_MISSION_GROUP_INFO stGroupInfo;
            it->second->GetWeekMission_Mission(&stGroupInfo);
            vecGroupInfo.push_back(stGroupInfo);
        }
    }
}

// IDA: ?LoadWeeklyMission@CGocWeeklyMission@@QEAAXAEAUPS_DB_WEEKLY_MISSION_GROUP_INFO_RES@@@Z (0x14017D8A0)
// Verified: Direct IDA decompilation - load weekly mission data from DB response
void CGocWeeklyMission::LoadWeeklyMission(PS_DB_WEEKLY_MISSION_GROUP_INFO_RES& psRes)
{
    if (psRes.stGroupInfo.byGroupID == 0)
        return;

    if (psRes.nError != 0)
    {
        if (psRes.bLast)
            m_bLoad = true;
        return;
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    TB_WEEK_GROUP* pTB_WEEK_GROUP = XResourceMgr::GetTB_WEEK_GROUP(&pGameServer->m_xResourceMgr, psRes.stGroupInfo.byGroupID);
    if (!pTB_WEEK_GROUP)
        return;

    // Check if it's a comeback event or play count is acceptable
    if (pTB_WEEK_GROUP->Event_Type == 3 || psRes.nPlayCount <= 8)
    {
        if (SetupWeeklyMissionGroup(psRes))
        {
            if (CheckGroupReset(psRes.stGroupInfo.byGroupID))
            {
                DeleteWeeklyMissionGroup(psRes.stGroupInfo.byGroupID);

                PS_DB_WEEKLY_MISSION_GROUP_INFO_RES psResTemp;
                psResTemp.stGroupInfo.byGroupID = psRes.stGroupInfo.byGroupID;
                psResTemp.stGroupInfo.byGroupType = 3;
                SetupWeeklyMissionGroup(psResTemp);
                SendToDBWeeklyMissionReset(psResTemp.stGroupInfo.byGroupID);
            }
        }
        else
        {
            DeleteWeeklyMissionGroup(psRes.stGroupInfo.byGroupID);
        }
    }

    if (psRes.bLast)
        m_bLoad = true;

    if (m_bLoad)
    {
        IsCompleteMission();
        SendWeeklyMissionLoad();
    }
}

// IDA: ?SetupWeeklyMissionGroup@CGocWeeklyMission@@QEAA_NAEAUPS_DB_WEEKLY_MISSION_GROUP_INFO_RES@@@Z (0x14017DA80)
// Verified: Direct IDA decompilation - setup a weekly mission group from DB data
bool CGocWeeklyMission::SetupWeeklyMissionGroup(PS_DB_WEEKLY_MISSION_GROUP_INFO_RES& psRes)
{
    if (!psRes.stGroupInfo.byGroupID)
    {
        CUser* pErrUser = dynamic_cast<CUser*>(GetOwnerGO());
        if (pErrUser)
        {
            LogHelper::LogError("game.contents",
                "<SetupWeeklyMissionGroup> GroupID ERROR < UCID : %d, Group : %d >",
                pErrUser->GetQuestID(), psRes.stGroupInfo.byGroupID);
        }
        return false;
    }

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
    {
        LogHelper::LogError("game.contents",
            "<SetupWeeklyMissionGroup> pUser ERROR < UCID : %d, Group : %d >",
            psRes.dwUCID, psRes.stGroupInfo.byGroupID);
        return false;
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return false;

    TB_WEEK_GROUP* pTB_WEEK_GROUP = XResourceMgr::GetTB_WEEK_GROUP(&pGameServer->m_xResourceMgr, psRes.stGroupInfo.byGroupID);
    if (!pTB_WEEK_GROUP)
    {
        LogHelper::LogError("game.contents",
            "<SetupWeeklyMissionGroup> pTB_WEEK_GROUP NULL < UCID : %d, Group : %d >",
            pUser->GetQuestID(), psRes.stGroupInfo.byGroupID);
        return false;
    }

    ATL::CTime tCurr = ATL::CTime::GetTickCount();
    ATL::CTime tStartDate(2000, 1, 1, 0, 0, 0, -1);
    ATL::CTime tEndDate(2000, 1, 1, 0, 0, 0, -1);

    // Calculate start/end dates based on event type
    if (pTB_WEEK_GROUP->Event_Type == 1)
    {
        // New character/account event
        if (pTB_WEEK_GROUP->Check_Mission_Type == 1)
        {
            ATL::CTime tCreateDate = pUser->GetCreateDate();
            ATL::CTime tMissionStartDate(2017, 6, 15, 9, 0, 0, -1);
            if (tMissionStartDate < tCreateDate)
            {
                tStartDate = ATL::CTime(tCreateDate.GetYear(), tCreateDate.GetMonth(), tCreateDate.GetDay(), 0, 0, 0, -1);
                tEndDate = tStartDate + ATL::CTimeSpan(19, 23, 59, 0);
            }
        }
        else if (pTB_WEEK_GROUP->Check_Mission_Type == 2)
        {
            ATL::CTime tAccountCreateDate = pUser->GetAccountCreateDate();
            ATL::CTime tMissionStartDate(2017, 6, 15, 9, 0, 0, -1);
            if (tMissionStartDate < tAccountCreateDate)
            {
                tStartDate = ATL::CTime(tAccountCreateDate.GetYear(), tAccountCreateDate.GetMonth(), tAccountCreateDate.GetDay(), 0, 0, 0, -1);
                tEndDate = tStartDate + ATL::CTimeSpan(19, 23, 59, 0);
            }
        }
    }
    else if (pTB_WEEK_GROUP->Event_Type == 3)
    {
        // Comeback event
        if (pTB_WEEK_GROUP->Check_Mission_Type == 1)
        {
            ATL::CTime tCheckDate(2010, 1, 1, 0, 0, 0, -1);
            ATL::CTime tLastComeBackDate = pUser->GetLastComeBackDate();
            if (tCheckDate < tLastComeBackDate)
            {
                ATL::CTime tComeBackDate = tLastComeBackDate;
                ATL::CTime tComeBackEndDate = ATL::CTime(tComeBackDate.GetYear(), tComeBackDate.GetMonth(), tComeBackDate.GetDay(), 0, 0, 0, -1) + ATL::CTimeSpan(9, 23, 59, 0);
                if (tCurr < tComeBackEndDate)
                {
                    tStartDate = ATL::CTime(tComeBackDate.GetYear(), tComeBackDate.GetMonth(), tComeBackDate.GetDay(), 0, 0, 0, -1);
                    tEndDate = tComeBackEndDate;
                }
            }
        }
        else if (pTB_WEEK_GROUP->Check_Mission_Type == 2)
        {
            ATL::CTime tCheckDate(2010, 1, 1, 0, 0, 0, -1);
            ATL::CTime tLastAccountComeBackDate = pUser->GetLastAccountComeBackDate();
            if (tCheckDate < tLastAccountComeBackDate)
            {
                ATL::CTime tComeBackDate = tLastAccountComeBackDate;
                ATL::CTime tComeBackEndDate = ATL::CTime(tComeBackDate.GetYear(), tComeBackDate.GetMonth(), tComeBackDate.GetDay(), 0, 0, 0, -1) + ATL::CTimeSpan(9, 23, 59, 0);
                if (tCurr < tComeBackEndDate)
                {
                    tStartDate = ATL::CTime(tComeBackDate.GetYear(), tComeBackDate.GetMonth(), tComeBackDate.GetDay(), 0, 0, 0, -1);
                    tEndDate = tComeBackEndDate;
                }
            }
        }
    }
    else
    {
        // Time-based event
        if (pTB_WEEK_GROUP->Event_Start_Year <= 2000 || !pTB_WEEK_GROUP->Event_Start_Month || !pTB_WEEK_GROUP->Event_Start_Day)
        {
            LogHelper::LogError("game.contents",
                "<SetupWeeklyMissionGroup> StartDate Error < UCID : %d, Group : %d >",
                pUser->GetQuestID(), psRes.stGroupInfo.byGroupID);
            return false;
        }
        if (pTB_WEEK_GROUP->Event_End_Year <= 2000 || !pTB_WEEK_GROUP->Event_End_Month || !pTB_WEEK_GROUP->Event_End_Day)
        {
            LogHelper::LogError("game.contents",
                "<SetupWeeklyMissionGroup> EndDate Error < UCID : %d, Group : %d >",
                pUser->GetQuestID(), psRes.stGroupInfo.byGroupID);
            return false;
        }
        tStartDate = ATL::CTime(pTB_WEEK_GROUP->Event_Start_Year, pTB_WEEK_GROUP->Event_Start_Month, pTB_WEEK_GROUP->Event_Start_Day, 0, 0, 0, -1);
        tEndDate = ATL::CTime(pTB_WEEK_GROUP->Event_End_Year, pTB_WEEK_GROUP->Event_End_Month, pTB_WEEK_GROUP->Event_End_Day, 9, 0, 0, -1);
    }

    // Check current time is within valid range
    if (tCurr < tStartDate || tEndDate < tCurr)
    {
        return false;
    }

    // Create CWeeklyMission_Group
    std::shared_ptr<CWeeklyMission_Group> pGroup(new CWeeklyMission_Group(
        psRes.stGroupInfo.byGroupID,
        pTB_WEEK_GROUP->Event_Type,
        psRes.stGroupInfo.byRewardState,
        tStartDate.GetTime(),
        tEndDate.GetTime()));

    if (!pGroup)
    {
        LogHelper::LogError("game.contents",
            "<SetupWeeklyMissionGroup> ERROR < UCID : %d, Group : %d >",
            pUser->GetQuestID(), psRes.stGroupInfo.byGroupID);
        return false;
    }

    // Populate mission days
    uint16_t wDayID_First = 100 * psRes.stGroupInfo.byGroupID + 1;
    for (int i = 0; i < pTB_WEEK_GROUP->grop_count; ++i)
    {
        TB_WEEK_DAY* pTB_WEEK_DAY = XResourceMgr::GetTB_WEEK_DAY(&pGameServer->m_xResourceMgr, wDayID_First);
        if (!pTB_WEEK_DAY)
        {
            LogHelper::LogError("game.contents",
                "<SetupWeeklyMissionGroup> pTB_WEEK_DAY NULL < UCID : %d, Group : %d, DayID : %d >",
                pUser->GetQuestID(), psRes.stGroupInfo.byGroupID, wDayID_First);
            return false;
        }

        if (pTB_WEEK_DAY->mission_group != psRes.stGroupInfo.byGroupID)
        {
            LogHelper::LogError("game.contents",
                "<SetupWeeklyMissionGroup> GroupID Different < UCID : %d, Group : %d, DayID : %d >",
                pUser->GetQuestID(), psRes.stGroupInfo.byGroupID, wDayID_First);
            return false;
        }

        ST_WEEKLY_MISSION_DAY_INFO stDayInfo;
        stDayInfo.wDayID = wDayID_First;

        // Find day info in response
        auto it_day = psRes.stGroupInfo.mapList.find(wDayID_First);
        if (it_day != psRes.stGroupInfo.mapList.end())
        {
            stDayInfo = it_day->second;
        }
        else
        {
            // Create default missions
            for (int mission = 0; mission < 11; ++mission)
            {
                TB_WEEK_MISSION* pTB_WEEK_MISSION = XResourceMgr::GetTB_WEEK_MISSION(&pGameServer->m_xResourceMgr, (&pTB_WEEK_DAY->mission_id_1)[mission]);
                if (!pTB_WEEK_MISSION)
                    break;

                ST_WEEKLY_MISSION_INFO stMissionInfo;
                stMissionInfo.dwMissionID = (&pTB_WEEK_DAY->mission_id_1)[mission];
                stMissionInfo.shValue = 0;
                stMissionInfo.byState = 0;
                stMissionInfo.biDate = 0;
                stDayInfo.vecList.push_back(stMissionInfo);
            }
        }

        unsigned int dwUCID = pUser->GetQuestID();
        if (!pGroup->AddWeekMissionDay(dwUCID, &stDayInfo))
        {
            LogHelper::LogError("game.contents",
                "<SetupWeeklyMissionGroup> CreateWeekMissionDay Fail < UCID : %d, Group : %d, DayID : %d >",
                pUser->GetQuestID(), psRes.stGroupInfo.byGroupID, wDayID_First);
            return false;
        }

        ++wDayID_First;
    }

    // Insert into m_mapWeekMissionGroup
    m_mapWeekMissionGroup[psRes.stGroupInfo.byGroupID] = pGroup;
    return true;
}

// IDA: ?CheckWeeklyMissionUpdate@CGocWeeklyMission@@QEAAXEHH@Z (0x14017EE40)
// Verified: Direct IDA decompilation - check and update weekly mission progress
void CGocWeeklyMission::CheckWeeklyMissionUpdate(uint8_t byType, int nObjectID, int nCount)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    if (!XResourceMgr::GetServerContents(&pGameServer->m_xResourceMgr, E_SERVER_OPTION_SOUL_WEEKLY_MISSION))
        return;

    CMover* pMover = GetOwnerGO();
    if (!pMover)
        return;

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser)
        return;

    PS_DB_WEEKLY_MISSION_UPDATE psUpdate;
    std::vector<ST_WEEKLY_MISSION_INFO> vecList;

    for (auto it = m_mapWeekMissionGroup.begin(); it != m_mapWeekMissionGroup.end(); ++it)
    {
        if (it->second)
        {
            unsigned int dwUCID = pUser->GetQuestID();
            unsigned int dwUAID = pUser->GetUAID();

            CWeeklyMission_Group* pGroup = it->second.get();
            if (pGroup)
            {
                // TODO: 需要人工审查 - CWeeklyMission_Group::CheckWeeklyMissionUpdate_Group 需要实现
                // pGroup->CheckWeeklyMissionUpdate_Group(dwUAID, dwUCID, byType, nObjectID, nCount, psUpdate.vecUpdate, vecList);
            }
        }
    }

    SendToDBWeeklyMissionUpdate(&psUpdate);

    for (size_t i = 0; i < vecList.size(); ++i)
    {
        SendWeeklyMissionUpdate(&vecList[i]);
    }
}

// IDA: ?ReqWeeklyMissionReward@CGocWeeklyMission@@QEAAXAEAUPS_WEEKLY_MISSION_REWARD_REQ@@@Z (0x14017F0E0)
// Verified: Direct IDA decompilation - request reward for completed weekly mission
void CGocWeeklyMission::ReqWeeklyMissionReward(PS_WEEKLY_MISSION_REWARD_REQ& psReq)
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return;

    if (!m_bLoad)
    {
        LogHelper::LogError("game.contents", "<ReqWeeklyMissionReward> Load ERROR < UID : %d >", pUser->GetUAID());
        return;
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    TB_WEEK_DAY* pTB_WEEK_DAY = XResourceMgr::GetTB_WEEK_DAY(&pGameServer->m_xResourceMgr, psReq.wDayID);
    if (!pTB_WEEK_DAY)
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionReward> TB_WEEK_DAY ERROR < UID : %d, Day : %d >",
            pUser->GetUAID(), psReq.wDayID);
        return;
    }

    TB_WEEK_GROUP* pTB_WEEK_GROUP = XResourceMgr::GetTB_WEEK_GROUP(&pGameServer->m_xResourceMgr, pTB_WEEK_DAY->mission_group);
    if (!pTB_WEEK_GROUP)
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionReward> TB_WEEK_GROUP ERROR < UID : %d, Group : %d >",
            pUser->GetUAID(), pTB_WEEK_DAY->mission_group);
        return;
    }

    TB_WEEK_MISSION* pTB_WEEK_MISSION = XResourceMgr::GetTB_WEEK_MISSION(&pGameServer->m_xResourceMgr, psReq.dwMissionID);
    if (!pTB_WEEK_MISSION)
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionReward> TB_WEEK_MISSION ERROR < UID : %d, Mission : %d >",
            pUser->GetUAID(), psReq.dwMissionID);
        return;
    }

    // Find the group in m_mapWeekMissionGroup
    auto it = m_mapWeekMissionGroup.find(pTB_WEEK_DAY->mission_group);
    if (it == m_mapWeekMissionGroup.end())
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionReward> Group ERROR < UID : %d, Group : %d, Day : %d >",
            pUser->GetUAID(), pTB_WEEK_DAY->mission_group, psReq.wDayID);
        return;
    }

    if (!it->second)
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionReward> pointer ERROR < UID : %d, Group : %d, Day : %d, Mission : %d >",
            pUser->GetUAID(), pTB_WEEK_DAY->mission_group, psReq.wDayID, psReq.dwMissionID);
        return;
    }

    // Find the day
    std::shared_ptr<CWeeklyMission_Day> pDay;
    it->second->FindWeekMissionDayPtr(pDay, psReq.wDayID);
    if (!pDay)
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionReward> pDay ERROR < UID : %d, Group : %d, Day : %d, Mission : %d >",
            pUser->GetUAID(), pTB_WEEK_DAY->mission_group, psReq.wDayID, psReq.dwMissionID);
        return;
    }

    // Check mission completion
    if (!pDay->IsCompleteMission(psReq.dwMissionID))
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionReward> Mission ERROR < UID : %d, Group : %d, Day : %d, Mission : %d >",
            pUser->GetUAID(), pTB_WEEK_DAY->mission_group, psReq.wDayID, psReq.dwMissionID);
        return;
    }

    // Check if already received reward
    if (pDay->IsReceivedReward(psReq.dwMissionID))
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionReward> Received ERROR < UID : %d, Group : %d, Day : %d, Mission : %d >",
            pUser->GetUAID(), pTB_WEEK_DAY->mission_group, psReq.wDayID, psReq.dwMissionID);
        return;
    }

    // Get inventory component
    CMover* pMover = GetOwnerGO();
    std::shared_ptr<CGocInventory> pInvenPtr;
    pMover->GetGOC<CGocInventory>(pInvenPtr, false);
    if (!pInvenPtr)
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionReward> Inven ERROR < UID : %d, Group : %d, Day : %d, Mission : %d >",
            pUser->GetUAID(), pTB_WEEK_DAY->mission_group, psReq.wDayID, psReq.dwMissionID);
        return;
    }

    // Prepare reward request
    PS_DB_WEEKLY_MISSION_REWARD_REQ psMission;
    ST_CREATE_ITEMS stCreateItems;
    unsigned int dwRewardMoney = 0;
    unsigned int dwRewardBP = 0;
    unsigned int dwRewardEther = 0;

    // Process reward items
    for (int i = 0; i < 3; ++i)
    {
        unsigned int nItemID = (&pTB_WEEK_MISSION->reward_item_id_1)[i];
        unsigned short nCount = (&pTB_WEEK_MISSION->reward_count_1)[i];
        if (!nItemID || !nCount)
            break;

        TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, nItemID);
        if (!pTB_ITEM)
            break;

        switch (pTB_ITEM->Item_ID)
        {
        case 0x2FAF0801: // Money
            dwRewardMoney += nCount;
            {
                ST_CREATE_ITEM stAddItem;
                stAddItem.nItemID = nItemID;
                stAddItem.shCount = nCount;
                psMission.vecItemList.push_back(stAddItem);
            }
            break;
        case 0x2FAF0803: // BP
            dwRewardBP += nCount;
            {
                ST_CREATE_ITEM stAddItem;
                stAddItem.nItemID = nItemID;
                stAddItem.shCount = nCount;
                psMission.vecItemList.push_back(stAddItem);
            }
            break;
        case 0x2FAF0804: // Ether
            dwRewardEther += nCount;
            {
                ST_CREATE_ITEM stAddItem;
                stAddItem.nItemID = nItemID;
                stAddItem.shCount = nCount;
                psMission.vecItemList.push_back(stAddItem);
            }
            break;
        default:
            {
                ST_CREATE_ITEM stAddItem;
                stAddItem.nItemID = nItemID;
                stAddItem.shCount = nCount;
                stCreateItems.push_back(stAddItem);
                psMission.vecItemList.push_back(stAddItem);
            }
            break;
        }
    }

    // Handle account-level rewards via mail
    if (pTB_WEEK_GROUP->Check_Mission_Type == 2)
    {
        ST_ACCOUNT_POST_DATA stAccountPostData;
        stAccountPostData.byMainType = 4;
        stAccountPostData.bySubType = pGameServer->GetSystemPostTableIndex(0xD, 0);
        stAccountPostData.dwUAID = pUser->GetUAID();
        stAccountPostData.biRegTime = pGameServer->GetCurDate();
        stAccountPostData.biDelDate = pGameServer->GetCurDate() + 29454;

        for (size_t j = 0; j < stCreateItems.size(); ++j)
        {
            stAccountPostData.stItemList[j].xSerial = 0;
            stAccountPostData.stItemList[j].nItemID = stCreateItems[j].nItemID;
            stAccountPostData.stItemList[j].sCount = stCreateItems[j].shCount;
        }

        if (dwRewardMoney)
            stAccountPostData.biMoney = dwRewardMoney;

        if (!stAccountPostData.bySubType)
        {
            LogHelper::LogError("game.contents",
                "Send WeeklyMission Reward Post error - Check TB_SystemMail(UAID:%d) (%d)",
                pUser->GetUAID(), 0);
        }

        IXObject* pObject = GetOwnerGO();
        XSendDBPacket xSendDBPacket(pObject, 6, 0x18);
        xSendDBPacket << stAccountPostData;
        pGameServer->SendDBGame(&xSendDBPacket);
    }
    else
    {
        // Give items directly
        ST_LOG_GAME stLog;
        stLog._sSubType = 97;
        stLog.nParam2 = it->first;
        stLog.nParam3 = psReq.wDayID;
        stLog.nParam4 = psReq.dwMissionID;
        psMission.byFlag = 42;

        if (!pInvenPtr->CreateItem2(&stCreateItems, 0x52, 0, &psMission.stCreateItem, &psMission.stUpdateItem, &stLog))
        {
            CGocNetwork::SendErrorMessage(pMover, 0x32, 3, 0xCB2A);
            LogHelper::LogError("game.contents",
                "<ReqWeeklyMissionReward> CreateItem2 ERROR < UID : %d, Group : %d, Day : %d, Mission : %d >",
                pUser->GetUAID(), pTB_WEEK_DAY->mission_group, psReq.wDayID, psReq.dwMissionID);
            return;
        }

        if (dwRewardMoney)
            pInvenPtr->AddMoney(dwRewardMoney, 0x37, psReq.dwMissionID, 0, 0);

        if (dwRewardBP)
            pInvenPtr->AddBP(dwRewardBP, 0x37);

        if (dwRewardEther)
            pInvenPtr->AddEther(dwRewardEther, 0x37, 1);
    }

    // Set received reward flag
    pDay->SetReceivedReward(psReq.dwMissionID);

    // Prepare DB update request
    psMission.stMission.dwUAID = pUser->GetUAID();
    psMission.stMission.dwUCID = pUser->GetQuestID();
    psMission.stMission.byGroupID = it->first;
    psMission.stMission.nDayID = psReq.wDayID;
    pDay->GetWeekMission_Mission(psReq.dwMissionID, &psMission.stMission);

    SendToDBWeeklyMissionReward(&psMission);
}

// IDA: ?ReqWeeklyMissionRewardWeek@CGocWeeklyMission@@QEAAXAEAUPS_WEEKLY_MISSION_REWARD_WEEK_REQ@@@Z (0x1401802A0)
// Verified: Direct IDA decompilation - request weekly mission group reward
void CGocWeeklyMission::ReqWeeklyMissionRewardWeek(PS_WEEKLY_MISSION_REWARD_WEEK_REQ& psReq)
{
    if (!m_bLoad)
    {
        CUser* pErrUser = dynamic_cast<CUser*>(GetOwnerGO());
        if (pErrUser)
        {
            LogHelper::LogError("game.contents", "<ReqWeeklyMissionRewardWeek> Load ERROR < UID : %d >", pErrUser->GetUAID());
        }
        return;
    }

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    TB_WEEK_GROUP* pTB_WEEK_GROUP = XResourceMgr::GetTB_WEEK_GROUP(&pGameServer->m_xResourceMgr, psReq.byGroupID);
    if (!pTB_WEEK_GROUP)
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionRewardWeek> Table ERROR < UID : %d, Group : %d >",
            pUser->GetUAID(), psReq.byGroupID);
        return;
    }

    // Find the group in m_mapWeekMissionGroup
    auto it = m_mapWeekMissionGroup.find(psReq.byGroupID);
    if (it == m_mapWeekMissionGroup.end())
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionRewardWeek> Group ERROR < UID : %d, Group : %d >",
            pUser->GetUAID(), psReq.byGroupID);
        return;
    }

    if (!it->second)
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionRewardWeek> ERROR < UID : %d, Group : %d >",
            pUser->GetUAID(), psReq.byGroupID);
        return;
    }

    // Check if already received group reward
    if (it->second->GetGroupReward() == 2)
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionRewardWeek> Group Reward ERROR < UID : %d, Group : %d, Reward : %d >",
            pUser->GetUAID(), psReq.byGroupID, it->second->GetGroupReward());
        return;
    }

    // Check if all day rewards received
    bool bCheckDayReward = it->second->IsReceivedRewardMission_Day();
    if (!bCheckDayReward)
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionRewardWeek> Day Reward ERROR < UID : %d, Group : %d, Reward : %d >",
            pUser->GetUAID(), psReq.byGroupID, bCheckDayReward);
        return;
    }

    // Get inventory component
    CMover* pMover = GetOwnerGO();
    std::shared_ptr<CGocInventory> pInvenPtr;
    pMover->GetGOC<CGocInventory>(pInvenPtr, false);
    if (!pInvenPtr)
    {
        LogHelper::LogError("game.contents",
            "<ReqWeeklyMissionReward> Inven ERROR < UID : %d, Group : %d >",
            pUser->GetUAID(), psReq.byGroupID);
        return;
    }

    // Prepare group reward request
    PS_DB_WEEKLY_MISSION_REWARD_WEEK_REQ psGroupReward;
    psGroupReward.dwUAID = pUser->GetUAID();
    psGroupReward.dwUCID = pUser->GetQuestID();
    psGroupReward.byGroupID = it->second->GetGroupID();

    unsigned int dwRewardMoney = 0;
    unsigned int dwRewardBP = 0;
    unsigned int dwRewardEther = 0;
    ST_CREATE_ITEMS stCreateItems;

    // Process group reward item
    if (pTB_WEEK_GROUP->reward_item_id && pTB_WEEK_GROUP->reward_count)
    {
        TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, pTB_WEEK_GROUP->reward_item_id);
        if (pTB_ITEM)
        {
            switch (pTB_ITEM->Item_ID)
            {
            case 0x2FAF0801: // Money
                dwRewardMoney += pTB_WEEK_GROUP->reward_count;
                {
                    ST_CREATE_ITEM stAddItem;
                    stAddItem.nItemID = pTB_WEEK_GROUP->reward_item_id;
                    stAddItem.shCount = pTB_WEEK_GROUP->reward_count;
                    psGroupReward.vecItemList.push_back(stAddItem);
                }
                break;
            case 0x2FAF0803: // BP
                dwRewardBP += pTB_WEEK_GROUP->reward_count;
                {
                    ST_CREATE_ITEM stAddItem;
                    stAddItem.nItemID = pTB_WEEK_GROUP->reward_item_id;
                    stAddItem.shCount = pTB_WEEK_GROUP->reward_count;
                    psGroupReward.vecItemList.push_back(stAddItem);
                }
                break;
            case 0x2FAF0804: // Ether
                dwRewardEther += pTB_WEEK_GROUP->reward_count;
                {
                    ST_CREATE_ITEM stAddItem;
                    stAddItem.nItemID = pTB_WEEK_GROUP->reward_item_id;
                    stAddItem.shCount = pTB_WEEK_GROUP->reward_count;
                    psGroupReward.vecItemList.push_back(stAddItem);
                }
                break;
            default:
                {
                    ST_CREATE_ITEM stAddItem;
                    stAddItem.nItemID = pTB_WEEK_GROUP->reward_item_id;
                    stAddItem.shCount = pTB_WEEK_GROUP->reward_count;
                    stCreateItems.push_back(stAddItem);
                    psGroupReward.vecItemList.push_back(stAddItem);
                }
                break;
            }

            // Handle account-level rewards via mail
            if (pTB_WEEK_GROUP->Check_Mission_Type == 2)
            {
                ST_ACCOUNT_POST_DATA stAccountPostData;
                stAccountPostData.byMainType = 4;
                stAccountPostData.bySubType = pGameServer->GetSystemPostTableIndex(0xD, 1);
                stAccountPostData.dwUAID = pUser->GetUAID();
                stAccountPostData.biRegTime = pGameServer->GetCurDate();
                stAccountPostData.biDelDate = pGameServer->GetCurDate() + 29454;

                for (size_t i = 0; i < stCreateItems.size(); ++i)
                {
                    stAccountPostData.stItemList[i].xSerial = 0;
                    stAccountPostData.stItemList[i].nItemID = stCreateItems[i].nItemID;
                    stAccountPostData.stItemList[i].sCount = stCreateItems[i].shCount;
                }

                if (dwRewardMoney)
                    stAccountPostData.biMoney = dwRewardMoney;

                if (!stAccountPostData.bySubType)
                {
                    LogHelper::LogError("game.contents",
                        "Send WeeklyMission Reward Post error - Check TB_SystemMail(UAID:%d) (%d)",
                        pUser->GetUAID(), 0);
                }

                IXObject* pObject = GetOwnerGO();
                XSendDBPacket xSendDBPacket(pObject, 6, 0x18);
                xSendDBPacket << stAccountPostData;
                pGameServer->SendDBGame(&xSendDBPacket);
            }
            else
            {
                // Give items directly
                ST_LOG_GAME stLog;
                stLog._sSubType = 98;
                stLog.nParam2 = it->second->GetGroupID();
                psGroupReward.byFlag = 43;

                if (!pInvenPtr->CreateItem2(&stCreateItems, 0x53, 0, &psGroupReward.stCreateItem, &psGroupReward.stUpdateItem, &stLog))
                {
                    CGocNetwork::SendErrorMessage(pMover, 0x32, 4, 0xCB2A);
                    LogHelper::LogError("game.contents",
                        "<ReqWeeklyMissionRewardWeek> CreateItem2 ERROR < UID : %d, Group : %d >",
                        pUser->GetUAID(), psReq.byGroupID);
                    return;
                }

                if (dwRewardMoney)
                    pInvenPtr->AddMoney(dwRewardMoney, 0x38, psReq.byGroupID, 0, 0);

                if (dwRewardBP)
                    pInvenPtr->AddBP(dwRewardBP, 0x38);

                if (dwRewardEther)
                    pInvenPtr->AddEther(dwRewardEther, 0x38, 1);

                // Set group reward received
                it->second->SetGroupReward(2);
                SendToDBWeeklyMissionRewardWeek(&psGroupReward);
            }
        }
    }
}

// Helper methods - need IDA addresses

void CGocWeeklyMission::DeleteWeeklyMissionGroup(uint8_t byGroupID)
{
    // TODO: 需要人工审查 - 需要从IDA获取地址和实现
    auto it = m_mapWeekMissionGroup.find(byGroupID);
    if (it != m_mapWeekMissionGroup.end())
    {
        m_mapWeekMissionGroup.erase(it);
    }
}

// IDA: ?CheckGroupReset@CGocWeeklyMission@@QEAA_NE@Z (0x140183930)
// Verified: Direct IDA decompilation - check if group needs reset
bool CGocWeeklyMission::CheckGroupReset(uint8_t byGroupID)
{
    auto it = m_mapWeekMissionGroup.find(byGroupID);
    if (it == m_mapWeekMissionGroup.end())
        return false;

    if (!it->second)
        return false;

    return it->second->CheckWeeklyMissionReset_Group();
}

// IDA: ?SendToDBWeeklyMissionReset@CGocWeeklyMission@@QEAAXE@Z (0x140182B40)
// Verified: Direct IDA decompilation - send weekly mission reset to DB
void CGocWeeklyMission::SendToDBWeeklyMissionReset(uint8_t byGroupID)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    if (!XResourceMgr::GetServerContents(&pGameServer->m_xResourceMgr, E_SERVER_OPTION_SOUL_WEEKLY_MISSION))
        return;

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return;

    bool bAccount = false;
    TB_WEEK_GROUP* pTB_WEEK_GROUP = XResourceMgr::GetTB_WEEK_GROUP(&pGameServer->m_xResourceMgr, byGroupID);
    if (pTB_WEEK_GROUP)
    {
        bAccount = (pTB_WEEK_GROUP->Check_Mission_Type == 2);
    }

    IXObject* pObject = GetOwnerGO();
    XSendDBPacket xSendDBPacket(pObject, 0x4A, 5);
    xSendDBPacket << pUser->GetQuestID();
    xSendDBPacket << byGroupID;
    xSendDBPacket << pUser->GetUAID();
    xSendDBPacket << bAccount;
    pGameServer->SendDBGame(&xSendDBPacket);
}

// IDA: ?SendToDBWeeklyMissionUpdate@CGocWeeklyMission@@QEAAXAEAUPS_DB_WEEKLY_MISSION_UPDATE@@@Z (0x140182410)
// Verified: Direct IDA decompilation - sends weekly mission update to DB
void CGocWeeklyMission::SendToDBWeeklyMissionUpdate(PS_DB_WEEKLY_MISSION_UPDATE* psUpdate)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) return;

    if (!XResourceMgr::GetServerContents(&pGameServer->m_xResourceMgr, E_SERVER_OPTION_SOUL_WEEKLY_MISSION)) {
        return;
    }

    CMover* pMover = GetOwnerGO();
    if (!pMover) return;

    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (!pUser) return;

    if (psUpdate->vecUpdate.empty()) {
        return;
    }

    // IDA: 遍历更新列表并写入日志
    for (size_t i = 0; i < psUpdate->vecUpdate.size(); ++i) {
        ST_DB_WEEKLY_MISSION_UPDATE& stUpdate = psUpdate->vecUpdate[i];

        TB_WEEK_GROUP* pTB_WEEK_GROUP = pGameServer->GetResourceMgr().GetTB_WEEK_GROUP(stUpdate.byGroupID);
        if (pTB_WEEK_GROUP) {
            bool bAccount = pTB_WEEK_GROUP->Check_Mission_Type == 2;
            stUpdate.bAccount = bAccount;

            // IDA: 写入日志
            ST_LOG_GAME stLog;
            stLog._nUAID = pUser->GetUAID();
            stLog._nUCID = pUser->GetQuestID();
            stLog._sMainType = 25;
            stLog._sSubType = 40;
            stLog.nParam0 = pTB_WEEK_GROUP->mission_group;
            stLog.nParam3 = stUpdate.byDay;
            stLog.nParam4 = stUpdate.byMission;
            stLog.nParam5 = stUpdate.byGroupID;
            stLog.nParam6 = pUser->GetLevel();

            pGameServer->SendDBLog(&stLog);
        }
    }

    // IDA: 发送 DB 包 (main=0x4A, sub=2)
    XSendDBPacket xSendDBPacket(pUser->GetObject(), 0x4A, 2);
    xSendDBPacket << *psUpdate;
    pGameServer->SendDBGame(&xSendDBPacket);
}

// IDA: ?SendToDBWeeklyMissionReward@CGocWeeklyMission@@QEAAXAEAUPS_DB_WEEKLY_MISSION_REWARD_REQ@@@Z (0x1401827C0)
// Verified: Direct IDA decompilation - send weekly mission reward request to DB
void CGocWeeklyMission::SendToDBWeeklyMissionReward(PS_DB_WEEKLY_MISSION_REWARD_REQ* psMission)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    if (!XResourceMgr::GetServerContents(&pGameServer->m_xResourceMgr, E_SERVER_OPTION_SOUL_WEEKLY_MISSION))
        return;

    if (!m_bLoad)
    {
        CUser* pErrUser = dynamic_cast<CUser*>(GetOwnerGO());
        if (pErrUser)
        {
            LogHelper::LogError("game.contents", "<SendToDBWeeklyMissionReward> Load ERROR < UID : %d >", pErrUser->GetUAID());
        }
        return;
    }

    // Get account flag from TB_WEEK_GROUP
    TB_WEEK_GROUP* pTB_WEEK_GROUP = XResourceMgr::GetTB_WEEK_GROUP(&pGameServer->m_xResourceMgr, psMission->stMission.byGroupID);
    if (pTB_WEEK_GROUP)
    {
        psMission->bAccount = (pTB_WEEK_GROUP->Check_Mission_Type == 2);
    }

    IXObject* pObject = GetOwnerGO();
    XSendDBPacket xSendDBPacket(pObject, 0x4A, 3);
    xSendDBPacket << *psMission;
    pGameServer->SendDBGame(&xSendDBPacket);
}

// IDA: ?SendToDBWeeklyMissionRewardWeek@CGocWeeklyMission@@QEAAXAEAUPS_DB_WEEKLY_MISSION_REWARD_WEEK_REQ@@@Z (0x140182980)
// Verified: Direct IDA decompilation - send weekly mission group reward request to DB
void CGocWeeklyMission::SendToDBWeeklyMissionRewardWeek(PS_DB_WEEKLY_MISSION_REWARD_WEEK_REQ* psWeek)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    if (!XResourceMgr::GetServerContents(&pGameServer->m_xResourceMgr, E_SERVER_OPTION_SOUL_WEEKLY_MISSION))
        return;

    if (!m_bLoad)
    {
        CUser* pErrUser = dynamic_cast<CUser*>(GetOwnerGO());
        if (pErrUser)
        {
            LogHelper::LogError("game.contents", "<SendToDBWeeklyMissionRewardWeek> Load ERROR < UID : %d >", pErrUser->GetUAID());
        }
        return;
    }

    // Get account flag from TB_WEEK_GROUP
    TB_WEEK_GROUP* pTB_WEEK_GROUP = XResourceMgr::GetTB_WEEK_GROUP(&pGameServer->m_xResourceMgr, psWeek->byGroupID);
    if (pTB_WEEK_GROUP)
    {
        psWeek->bAccount = (pTB_WEEK_GROUP->Check_Mission_Type == 2);
    }

    IXObject* pObject = GetOwnerGO();
    XSendDBPacket xSendDBPacket(pObject, 0x4A, 4);
    xSendDBPacket << *psWeek;
    pGameServer->SendDBGame(&xSendDBPacket);
}

// IDA: ?ResWeeklyMissionReward@CGocWeeklyMission@@QEAAXAEAUPS_DB_WEEKLY_MISSION_REWARD_RES@@@Z (0x1401810A0)
// Verified: Direct IDA decompilation - handle DB response for weekly mission reward
void CGocWeeklyMission::ResWeeklyMissionReward(PS_DB_WEEKLY_MISSION_REWARD_RES& psRes)
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return;

    if (psRes.nError)
    {
        CMover* pMover = GetOwnerGO();
        CGocNetwork::SendErrorMessage(pMover, 0x32, 3, 0xCB36);
        LogHelper::LogError("game.contents",
            "<ResWeeklyMissionReward> SQL ERROR < UID : %d, Group : %d, Day : %d, Mission : %d, Error : %d >",
            pUser->GetUAID(), psRes.psReq.stMission.byGroupID, psRes.psReq.stMission.nDayID,
            psRes.psReq.stMission.dwMissionID, psRes.nError);
        return;
    }

    PS_WEEKLY_MISSION_REWARD_RES psReward;
    psReward.byGroupID = psRes.psReq.stMission.byGroupID;
    psReward.wDayID = psRes.psReq.stMission.nDayID;
    psReward.dwMissionID = psRes.psReq.stMission.dwMissionID;

    // If not account-level, send items to player
    if (!psRes.psReq.stMission.bAccount)
    {
        CMover* pMover = GetOwnerGO();
        std::shared_ptr<CGocInventory> pInvenPtr;
        pMover->GetGOC<CGocInventory>(pInvenPtr, false);
        if (pInvenPtr)
        {
            pInvenPtr->SendUpdateItem(&psRes.psReq.stUpdateItem);
            pInvenPtr->SendCreateItem(&psRes.psReq.stCreateItem);
        }
        psReward.vecItemList = psRes.psReq.vecItemList;
    }

    // Send reward response to client
    XSendPacket xSendPacket(0x32, 3);
    xSendPacket << psReward;
    CMover* pMover = GetOwnerGO();
    if (pMover)
        CGocNetwork::Send(pMover, &xSendPacket);

    // Log reward items
    ST_LOG_GAME stLog;
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetQuestID();
    stLog._sMainType = 25;
    stLog._sSubType = 41;
    stLog.nParam0 = psReward.byGroupID;
    stLog.nParam3 = psReward.dwMissionID;
    stLog.nParam6 = pUser->GetLevel();

    for (size_t i = 0; i < psRes.psReq.vecItemList.size(); ++i)
    {
        stLog.nParam1 = psRes.psReq.vecItemList[i].nItemID;
        stLog.nParam2 = psRes.psReq.vecItemList[i].shCount;
        XGameServer::Instance()->SendDBLog(&stLog);
    }
}

// IDA: ?ResWeeklyMissionRewardWeek@CGocWeeklyMission@@QEAAXAEAUPS_DB_WEEKLY_MISSION_REWARD_WEEK_RES@@@Z (0x140181590)
// Verified: Direct IDA decompilation - handle DB response for weekly mission group reward
void CGocWeeklyMission::ResWeeklyMissionRewardWeek(PS_DB_WEEKLY_MISSION_REWARD_WEEK_RES& psRes)
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return;

    if (psRes.nError)
    {
        CMover* pMover = GetOwnerGO();
        CGocNetwork::SendErrorMessage(pMover, 0x32, 4, 0xCB36);
        LogHelper::LogError("game.contents",
            "<ResWeeklyMissionRewardWeek> SQL ERROR < UID : %d, Group : %d, Error : %d >",
            pUser->GetUAID(), psRes.psReq.byGroupID, psRes.nError);
        return;
    }

    PS_WEEKLY_MISSION_REWARD_WEEK_RES psReward;
    psReward.byGroupID = psRes.psReq.byGroupID;

    // If not account-level, send items to player
    if (!psRes.psReq.bAccount)
    {
        CMover* pMover = GetOwnerGO();
        std::shared_ptr<CGocInventory> pInvenPtr;
        pMover->GetGOC<CGocInventory>(pInvenPtr, false);
        if (pInvenPtr)
        {
            pInvenPtr->SendUpdateItem(&psRes.psReq.stUpdateItem);
            pInvenPtr->SendCreateItem(&psRes.psReq.stCreateItem);
        }
        psReward.vecItemList = psRes.psReq.vecItemList;
    }

    // Send reward response to client
    XSendPacket xSendPacket(0x32, 4);
    xSendPacket << psReward;
    CMover* pMover = GetOwnerGO();
    if (pMover)
        CGocNetwork::Send(pMover, &xSendPacket);

    // Log reward items
    ST_LOG_GAME stLog;
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetQuestID();
    stLog._sMainType = 25;
    stLog._sSubType = 42;
    stLog.nParam0 = psReward.byGroupID;
    stLog.nParam6 = pUser->GetLevel();

    for (size_t i = 0; i < psRes.psReq.vecItemList.size(); ++i)
    {
        stLog.nParam1 = psRes.psReq.vecItemList[i].nItemID;
        stLog.nParam2 = psRes.psReq.vecItemList[i].shCount;
        XGameServer::Instance()->SendDBLog(&stLog);
    }
}

void CGocWeeklyMission::SendWeeklyMissionLoad()
{
    // TODO: 需要人工审查 - 需要从IDA获取地址和实现
}

// IDA: ?SendWeeklyMissionUpdate@CGocWeeklyMission@@QEAAXAEAUST_WEEKLY_MISSION_INFO@@@Z (0x140182F60)
// Verified: Direct IDA decompilation - sends weekly mission update to client
void CGocWeeklyMission::SendWeeklyMissionUpdate(ST_WEEKLY_MISSION_INFO* stUpdate)
{
    // IDA: 发送包 (main=0x32, sub=2)
    XSendPacket xSendPacket(0x32, 2);
    xSendPacket << *stUpdate;

    CMover* pMover = GetOwnerGO();
    if (pMover) {
        CGocNetwork::Send(pMover, &xSendPacket);
    }
}

// IDA: ?IsCompleteMission@CGocWeeklyMission@@QEAA_NXZ (0x14017ED50)
// Verified: Direct IDA decompilation - checks if all weekly missions are complete
bool CGocWeeklyMission::IsCompleteMission()
{
    // IDA: 如果未加载，返回 false
    if (!m_bLoad) {
        return false;
    }

    // IDA: 遍历所有周任务组
    for (auto it = m_mapWeekMissionGroup.begin(); it != m_mapWeekMissionGroup.end(); ++it) {
        if (it->second) {
            // IDA: 获取 UCID
            CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
            if (!pUser) continue;

            unsigned int dwUCID = pUser->GetQuestID();

            // IDA: 调用 CWeeklyMission_Group::IsCompleteMission_Group
            CWeeklyMission_Group* pGroup = it->second.get();
            if (pGroup) {
                pGroup->IsCompleteMission_Group(dwUCID);
            }
        }
    }

    return true;
}

// IDA: ?DeleteWeeklyMissionGroup@CGocWeeklyMission@@QEAA_NE@Z (0x14017EC80)
// Verified: Direct IDA decompilation - deletes a weekly mission group by ID
bool CGocWeeklyMission::DeleteWeeklyMissionGroup(unsigned char byGroupID)
{
    // IDA: 查找指定组ID
    auto it = m_mapWeekMissionGroup.find(byGroupID);
    if (it == m_mapWeekMissionGroup.end()) {
        return true;
    }

    // IDA: 如果组存在，调用 Clear 清理
    if (it->second) {
        CWeeklyMission_Group* pGroup = it->second.get();
        if (pGroup) {
            pGroup->Clear();
        }
    }

    // IDA: 从 map 中删除
    m_mapWeekMissionGroup.erase(it);

    return true;
}
