#pragma once

#include "GOComponent.h"
#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <vector>

// Forward declarations
class CUser;
class CMover;
class CWeeklyMission_Group;
class CWeeklyMission_Day;
struct PS_DB_WEEKLY_MISSION_GROUP_INFO_RES;
struct PS_WEEKLY_MISSION_REWARD_REQ;
struct PS_WEEKLY_MISSION_REWARD_WEEK_REQ;
struct ST_WEEKLY_MISSION_GROUP_INFO;
struct ST_WEEKLY_MISSION_INFO;
struct PS_DB_WEEKLY_MISSION_UPDATE;
struct PS_DB_WEEKLY_MISSION_REWARD_REQ;
struct PS_DB_WEEKLY_MISSION_REWARD_WEEK_REQ;

/**
 * @brief CGocWeeklyMission - Game Object Component for weekly mission system
 *
 * Handles weekly mission tracking, completion, and rewards.
 * IDA: Constructor at 0x14017D160
 */
class CGocWeeklyMission : public GOComponent {
public:
    // IDA: ??0CGocWeeklyMission@@QEAA@XZ (0x14017D160)
    CGocWeeklyMission();

    // IDA: ??1CGocWeeklyMission@@UEAA@XZ (0x14017D1F0)
    virtual ~CGocWeeklyMission();

    // Static method
    // IDA: ?GetFamilyID@CGocWeeklyMission@@SAHXZ (0x140039070)
    static int GetFamilyID();

    // IDA: ?Init@CGocWeeklyMission@@QEAA_NXZ (0x14017D240)
    bool Init();

    // IDA: ?Clear@CGocWeeklyMission@@QEAAXXZ (0x14017D260)
    void Clear();

    // IDA: ?GetWeeklyMissionGroupID@CGocWeeklyMission@@QEAAX_N000AEAV?$set@EU?$less@E@std@@V?$allocator@E@2@@std@@@Z (0x14017D310)
    void GetWeeklyMissionGroupID(bool bNewCharacter, bool bComeBackCharacter,
                                  bool bNewAccount, bool bComeBackAccount,
                                  std::set<uint8_t>& setGroupList);

    // IDA: ?GetWeekMission@CGocWeeklyMission@@QEAAXAEAV?$vector@UST_WEEKLY_MISSION_GROUP_INFO@@V?$allocator@UST_WEEKLY_MISSION_GROUP_INFO@@@std@@@std@@@Z (0x14017D7C0)
    void GetWeekMission(std::vector<ST_WEEKLY_MISSION_GROUP_INFO>& vecGroupInfo);

    // IDA: ?LoadWeeklyMission@CGocWeeklyMission@@QEAAXAEAUPS_DB_WEEKLY_MISSION_GROUP_INFO_RES@@@Z (0x14017D8A0)
    void LoadWeeklyMission(PS_DB_WEEKLY_MISSION_GROUP_INFO_RES& psRes);

    // IDA: ?SetupWeeklyMissionGroup@CGocWeeklyMission@@QEAA_NAEAUPS_DB_WEEKLY_MISSION_GROUP_INFO_RES@@@Z (0x14017DA80)
    bool SetupWeeklyMissionGroup(PS_DB_WEEKLY_MISSION_GROUP_INFO_RES& psRes);

    // IDA: ?CheckWeeklyMissionUpdate@CGocWeeklyMission@@QEAAXEHH@Z (0x14017EE40)
    void CheckWeeklyMissionUpdate(uint8_t byType, int nObjectID, int nCount);

    // IDA: ?ReqWeeklyMissionReward@CGocWeeklyMission@@QEAAXAEAUPS_WEEKLY_MISSION_REWARD_REQ@@@Z (0x14017F0E0)
    void ReqWeeklyMissionReward(PS_WEEKLY_MISSION_REWARD_REQ& psReq);

    // IDA: ?ReqWeeklyMissionRewardWeek@CGocWeeklyMission@@QEAAXAEAUPS_WEEKLY_MISSION_REWARD_WEEK_REQ@@@Z (0x1401802A0)
    void ReqWeeklyMissionRewardWeek(PS_WEEKLY_MISSION_REWARD_WEEK_REQ& psReq);

    // Helper methods (need to verify addresses from IDA)
    void DeleteWeeklyMissionGroup(uint8_t byGroupID);
    bool CheckGroupReset(uint8_t byGroupID);
    void SendToDBWeeklyMissionReset(uint8_t byGroupID);
    void SendToDBWeeklyMissionUpdate(PS_DB_WEEKLY_MISSION_UPDATE* psUpdate);
    void SendToDBWeeklyMissionReward(PS_DB_WEEKLY_MISSION_REWARD_REQ* psMission);
    void SendToDBWeeklyMissionRewardWeek(PS_DB_WEEKLY_MISSION_REWARD_WEEK_REQ* psMission);
    void SendWeeklyMissionLoad();
    void SendWeeklyMissionUpdate(ST_WEEKLY_MISSION_INFO* pInfo);
    void IsCompleteMission();

protected:
    // IDA: m_bLoad at offset determined by class layout
    bool m_bLoad = false;

    // IDA: m_mapWeekMissionGroup - map of group ID to weekly mission group
    // Type: std::map<int, std::tr1::shared_ptr<CWeeklyMission_Group>>
    std::map<uint8_t, std::shared_ptr<CWeeklyMission_Group>> m_mapWeekMissionGroup;
};
