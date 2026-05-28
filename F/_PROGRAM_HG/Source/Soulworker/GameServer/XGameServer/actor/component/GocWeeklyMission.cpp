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
// Destructor
CGocWeeklyMission::~CGocWeeklyMission()
{
    // IDA: sets vftable, destroys m_mapWeekMissionGroup, then calls GOComponent::~GOComponent
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
// Get all weekly mission group info
void CGocWeeklyMission::GetWeekMission(std::vector<ST_WEEKLY_MISSION_GROUP_INFO>& vecGroupInfo)
{
    // IDA: iterates through m_mapWeekMissionGroup, calls GetWeekMission_Mission on each group
    for (auto it = m_mapWeekMissionGroup.begin(); it != m_mapWeekMissionGroup.end(); ++it)
    {
        if (it->second)
        {
            ST_WEEKLY_MISSION_GROUP_INFO stGroupInfo;
            // TODO: 需要人工审查 - CWeeklyMission_Group::GetWeekMission_Mission 需要实现
            // it->second->GetWeekMission_Mission(&stGroupInfo);
            vecGroupInfo.push_back(stGroupInfo);
        }
    }
}

// IDA: ?LoadWeeklyMission@CGocWeeklyMission@@QEAAXAEAUPS_DB_WEEKLY_MISSION_GROUP_INFO_RES@@@Z (0x14017D8A0)
// Load weekly mission data from DB response
void CGocWeeklyMission::LoadWeeklyMission(PS_DB_WEEKLY_MISSION_GROUP_INFO_RES& psRes)
{
    // IDA: Validates group ID, checks error, gets TB_WEEK_GROUP, validates event type,
    // calls SetupWeeklyMissionGroup, handles reset if needed

    if (psRes.stGroupInfo.byGroupID == 0)
        return;

    if (psRes.nError != 0)
    {
        if (psRes.bLast)
            m_bLoad = true;
        return;
    }

    auto* pGameServer = TXSingleton<XGameServer>::Instance();
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
                psResTemp.stGroupInfo.byRewardState = 3;
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
// Setup a weekly mission group from DB data
bool CGocWeeklyMission::SetupWeeklyMissionGroup(PS_DB_WEEKLY_MISSION_GROUP_INFO_RES& psRes)
{
    // IDA: Complex function that validates group, gets user, checks dates,
    // creates CWeeklyMission_Group, populates mission days

    // TODO: 需人工审查 - 此函数非常复杂，需要完整的还原
    // 主要逻辑:
    // 1. Validate group ID
    // 2. Get owner CUser via RTTI
    // 3. Get TB_WEEK_GROUP from resource
    // 4. Calculate start/end dates based on event type
    // 5. Check current time is within valid range
    // 6. Create CWeeklyMission_Group
    // 7. For each day in group, create CWeeklyMission_Day with missions
    // 8. Insert into m_mapWeekMissionGroup

    return false; // TODO: 需人工审查 - 完整实现
}

// IDA: ?CheckWeeklyMissionUpdate@CGocWeeklyMission@@QEAAXEHH@Z (0x14017EE40)
// Check and update weekly mission progress
void CGocWeeklyMission::CheckWeeklyMissionUpdate(uint8_t byType, int nObjectID, int nCount)
{
    // IDA: Checks server option, gets user, iterates groups,
    // calls CheckWeeklyMissionUpdate_Group, sends updates to DB and client

    auto* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer)
        return;

    if (!XResourceMgr::GetServerContents(&pGameServer->m_xResourceMgr, E_SERVER_OPTION_SOUL_WEEKLY_MISSION))
        return;

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return;

    PS_DB_WEEKLY_MISSION_UPDATE psUpdate;
    std::vector<ST_WEEKLY_MISSION_INFO> vecList;

    for (auto it = m_mapWeekMissionGroup.begin(); it != m_mapWeekMissionGroup.end(); ++it)
    {
        if (it->second)
        {
            // TODO: 需要人工审查 - CWeeklyMission_Group::CheckWeeklyMissionUpdate_Group 需要实现
            // it->second->CheckWeeklyMissionUpdate_Group(pUser->GetUAID(), pUser->GetUCID(),
            //     byType, nObjectID, nCount, psUpdate.vecUpdate, vecList);
        }
    }

    SendToDBWeeklyMissionUpdate(&psUpdate);

    for (size_t i = 0; i < vecList.size(); ++i)
    {
        SendWeeklyMissionUpdate(&vecList[i]);
    }
}

// IDA: ?ReqWeeklyMissionReward@CGocWeeklyMission@@QEAAXAEAUPS_WEEKLY_MISSION_REWARD_REQ@@@Z (0x14017F0E0)
// Request reward for completed weekly mission
void CGocWeeklyMission::ReqWeeklyMissionReward(PS_WEEKLY_MISSION_REWARD_REQ& psReq)
{
    // IDA: Very complex function that validates mission completion,
    // gets rewards, creates items or sends mail, updates DB

    // TODO: 需人工审查 - 此函数非常复杂，需要完整的还原
    // 主要逻辑:
    // 1. Check m_bLoad
    // 2. Get TB_WEEK_DAY, TB_WEEK_GROUP, TB_WEEK_MISSION
    // 3. Find the group and day in m_mapWeekMissionGroup
    // 4. Check IsCompleteMission and !IsReceivedReward
    // 5. Get CGocInventory component
    // 6. Process rewards (money, BP, ether, items)
    // 7. If Check_Mission_Type == 2 (account), send via mail
    // 8. Otherwise give items directly
    // 9. Set received reward flag
    // 10. Send DB update
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

bool CGocWeeklyMission::CheckGroupReset(uint8_t byGroupID)
{
    // TODO: 需要人工审查 - 需要从IDA获取地址和实现
    return false;
}

void CGocWeeklyMission::SendToDBWeeklyMissionReset(uint8_t byGroupID)
{
    // TODO: 需要人工审查 - 需要从IDA获取地址和实现
}

void CGocWeeklyMission::SendToDBWeeklyMissionUpdate(PS_DB_WEEKLY_MISSION_UPDATE* psUpdate)
{
    // TODO: 需要人工审查 - 需要从IDA获取地址和实现
}

void CGocWeeklyMission::SendToDBWeeklyMissionReward(PS_DB_WEEKLY_MISSION_REWARD_REQ* psMission)
{
    // TODO: 需要人工审查 - 需要从IDA获取地址和实现
}

void CGocWeeklyMission::SendWeeklyMissionLoad()
{
    // TODO: 需要人工审查 - 需要从IDA获取地址和实现
}

void CGocWeeklyMission::SendWeeklyMissionUpdate(ST_WEEKLY_MISSION_INFO* pInfo)
{
    // TODO: 需要人工审查 - 需要从IDA获取地址和实现
}

void CGocWeeklyMission::IsCompleteMission()
{
    // TODO: 需要人工审查 - 需要从IDA获取地址和实现
}
