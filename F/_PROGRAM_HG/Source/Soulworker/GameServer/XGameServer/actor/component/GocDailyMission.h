#pragma once

#include "GOComponent.h"
#include <map>
#include <vector>
#include <memory>
#include <cstdint>

// Forward declarations
class CDailyMissionInfo;
class CMover;
class XGameServer;
struct TB_DAILY_MISSION;
struct ST_DAILY_MISSION_INFO;
struct PS_DAILY_MISSION_UPDATE;
struct PS_MAP_DISTRICT_DAILY_MISSION;

/**
 * @brief E_DAILY_MISSION_TYPE - Daily mission type enumeration
 *
 * Mission types determine how missions are categorized and managed.
 */
enum E_DAILY_MISSION_TYPE : std::uint8_t {
    eDAILY_MISSION_TYPE_SPECIAL = 1,      // Special missions
    eDAILY_MISSION_TYPE_GUERRILLA = 2,    // Guerrilla missions
    eDAILY_MISSION_TYPE_EVENT = 3,        // Event missions
};

/**
 * @brief E_DAILY_MISSION_FINISH_TYPE - Daily mission finish type enumeration
 */
enum E_DAILY_MISSION_FINISH_TYPE : std::uint8_t {
    eDAILY_MISSION_FINISH_KILL = 1,              // Kill target
    eDAILY_MISSION_FINISH_MAZE_CLEAR = 2,        // Clear maze
    eDAILY_MISSION_FINISH_PARTY_MAZE_CLEAR = 3,  // Clear maze with party
};

/**
 * @brief E_DAILY_MISSION_TARGET - Daily mission target type enumeration
 */
enum E_DAILY_MISSION_TARGET : std::uint8_t {
    eDAILY_MISSION_TARGET_NONE = 0,
    eDAILY_MISSION_TARGET_MAZE = 1,
    eDAILY_MISSION_TARGET_MONSTER = 2,
    eDAILY_MISSION_TARGET_ITEM = 3,
    eDAILY_MISSION_TARGET_QUEST = 4,
    eDAILY_MISSION_TARGET_PVP = 5,
    eDAILY_MISSION_TARGET_FRIEND = 6,
};

/**
 * @brief E_DAILY_MISSION_STATE - Daily mission state enumeration
 */
enum E_DAILY_MISSION_STATE : std::uint8_t {
    eDAILY_MISSION_STATE_NONE = 0,       // Not started
    eDAILY_MISSION_STATE_ACCEPT = 1,     // Accepted/in progress
    eDAILY_MISSION_STATE_COMPLETE = 2,   // Completed
    eDAILY_MISSION_STATE_REWARD = 3,     // Reward claimed
};

/**
 * @brief CGocDailyMission - Game Object Component for daily mission system
 *
 * Handles daily mission tracking, completion, and rewards.
 * Manages three types of missions: Special, Guerrilla, and Event.
 *
 * IDA layout (from decompiled functions):
 * - Inherits from GOComponent (offset 0, 16 bytes including vtable)
 * - m_mapSpecial: std::map<int, std::tr1::shared_ptr<CDailyMissionInfo>>
 * - m_mapGuerrilla: std::map<int, std::tr1::shared_ptr<CDailyMissionInfo>>
 * - m_mapEvent: std::map<int, std::tr1::shared_ptr<CDailyMissionInfo>>
 * - m_nHelperCount: int
 */
class CGocDailyMission : public GOComponent {
public:
    // IDA: ??0CGocDailyMission@@QEAA@XZ (0x14004EAD0)
    CGocDailyMission();

    // IDA: ??1CGocDailyMission@@UEAA@XZ (0x14004EB80)
    virtual ~CGocDailyMission();

    // GOComponent interface - stub implementations for now
    bool Initialize() override { return Init(); }
    void Shutdown() override { Clear(); }
    void Update(float fDeltaTime) override { (void)fDeltaTime; }

    // IDA: ?Init@CGocDailyMission@@QEAA_NXZ (0x14004EBF0)
    bool Init();

    // IDA: ?Clear@CGocDailyMission@@QEAAXXZ (0x14004EC10)
    void Clear();

    // IDA: ?GetFamilyID@CGocDailyMission@@SAHXZ (0x1400F5070)
    static int GetFamilyID() { return 15; }

    // IDA: ?GetMissionInfo@CGocDailyMission@@QEAA?AV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@KE@Z (0x14004EC60)
    std::tr1::shared_ptr<CDailyMissionInfo> GetMissionInfo(std::uint32_t dwMissionID, std::uint8_t byType);

    // IDA: ?FindMission@CGocDailyMission@@QEAA?AV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@KE@Z (0x14004EE60)
    std::tr1::shared_ptr<CDailyMissionInfo> FindMission(std::uint32_t dwMissionID, std::uint8_t byType);

    // IDA: ?AddDailyMission@CGocDailyMission@@QEAAXAEAUST_DAILY_MISSION_INFO@@@Z (0x14004F180)
    void AddDailyMission(ST_DAILY_MISSION_INFO& stMission);

    // IDA: ?AcceptDailyMission@CGocDailyMission@@QEAA_NK@Z (0x140050610)
    bool AcceptDailyMission(std::uint32_t dwMissionID);

    // IDA: ?CompleteDailyMission@CGocDailyMission@@QEAA_NKV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@@Z (0x140054960)
    bool CompleteDailyMission(std::uint32_t dwMissionID, std::tr1::shared_ptr<CDailyMissionInfo> pMission);

    // IDA: ?UpdateKillType@CGocDailyMission@@QEAAXKK@Z (0x1400539D0)
    void UpdateKillType(std::uint32_t dwObjectID, std::uint32_t dwCondition);

    // IDA: ?UpdateMazeClearType@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_TARGET@@KFK_N@Z (0x140053D00)
    void UpdateMazeClearType(E_DAILY_MISSION_TARGET eTarget, std::uint32_t dwObjectID,
                            std::int16_t shRank, std::uint32_t dwTime, bool bPartyWith);

    // IDA: ?SendDailyMissionList@CGocDailyMission@@QEAAXE@Z (0x140055780)
    void SendDailyMissionList(std::uint8_t byTodayInit);

    // IDA: ?SetNewDailyMissionList@CGocDailyMission@@QEAAXAEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x14004EF80)
    void SetNewDailyMissionList(std::vector<std::uint32_t>& vecNewMission);

    // IDA: ?GetDailyMissionList@CGocDailyMission@@QEAAXAEAUPS_MAP_DISTRICT_DAILY_MISSION@@@Z (0x140050BF0)
    void GetDailyMissionList(PS_MAP_DISTRICT_DAILY_MISSION* psMissionList);

    // IDA: ?UpdateFriendType@CGocDailyMission@@QEAAXAEAV?$vector@UST_DAILY_MISSION_FRIEND_RES@@V?$allocator@UST_DAILY_MISSION_FRIEND_RES@@@std@@@std@@@Z (0x140054080)
    void UpdateFriendType(std::vector<ST_DAILY_MISSION_INFO>& vecMission);

protected:
    // Helper methods (to be implemented based on IDA analysis)
    bool CheckDailyMissionTime(ST_DAILY_MISSION_INFO* stInfo);
    bool CheckDailyMissionReward(std::uint32_t dwMissionID);
    bool CheckUpdateKillType(std::uint32_t dwMissionID, std::uint32_t dwObjectID, std::uint32_t dwCondition);
    bool CheckUpdateMazeClearType(std::uint32_t dwMissionID, std::uint32_t dwObjectID,
                                  std::int16_t shRank, std::uint32_t dwTime);

    void DBUpdateMissionInfo(PS_DAILY_MISSION_UPDATE* psUpdate);
    void DBDailyMissionPost(std::uint32_t dwMissionID);
    void SendDailyMissionUpdateList(PS_DAILY_MISSION_UPDATE* psUpdate);

    std::uint32_t CalculateMissionRemainTime(void* tNow, void* tStart);
    std::uint32_t CalculateGetMissionDurationTime(void* tNow, void* tStart, void* tEnd);
    void AddNewDailyMission(std::uint32_t dwMissionID, void* tCurr);

    void GetDailyMissionList(E_DAILY_MISSION_FINISH_TYPE eType,
                            std::vector<std::tr1::shared_ptr<CDailyMissionInfo>>* vecList);

private:
    // IDA: member variables (offsets relative to GOComponent base)
    // Maps storing mission info by mission ID, keyed by mission type
    std::map<std::uint32_t, std::tr1::shared_ptr<CDailyMissionInfo>> m_mapSpecial;      // Special missions (type 1)
    std::map<std::uint32_t, std::tr1::shared_ptr<CDailyMissionInfo>> m_mapGuerrilla;    // Guerrilla missions (type 2)
    std::map<std::uint32_t, std::tr1::shared_ptr<CDailyMissionInfo>> m_mapEvent;        // Event missions (type 3)
    int m_nHelperCount;  // Number of active helper missions (max 4)
};
