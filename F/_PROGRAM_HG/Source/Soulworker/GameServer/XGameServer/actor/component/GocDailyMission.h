#pragma once

#include "GOComponent.h"
#include <map>
#include <vector>
#include <memory>
#include <cstdint>

// Forward declarations for ATL::CTime
namespace ATL {
    class CTime;
    class CTimeSpan;
}

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
    eDAILY_MISSION_FINISH_COLLECT = 4,           // Collect items
    eDAILY_MISSION_FINISH_MYROOM = 5,            // My room activities
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
 * @brief E_DAILY_MISSION_CONDITION - Daily mission condition type enumeration
 */
enum E_DAILY_MISSION_CONDITION : std::uint8_t {
    eDAILY_MISSION_CONDITION_NONE = 0,
    eDAILY_MISSION_CONDITION_DECORATION = 1,  // Place decoration
    eDAILY_MISSION_CONDITION_FURNITURE = 2,   // Place furniture
    eDAILY_MISSION_CONDITION_VISIT = 3,       // Visit other room
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
 * - m_mapSpecial: std::map<int, std::shared_ptr<CDailyMissionInfo>>
 * - m_mapGuerrilla: std::map<int, std::shared_ptr<CDailyMissionInfo>>
 * - m_mapEvent: std::map<int, std::shared_ptr<CDailyMissionInfo>>
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
    std::shared_ptr<CDailyMissionInfo> GetMissionInfo(std::uint32_t dwMissionID, std::uint8_t byType);

    // IDA: ?FindMission@CGocDailyMission@@QEAA?AV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@KE@Z (0x14004EE60)
    std::shared_ptr<CDailyMissionInfo> FindMission(std::uint32_t dwMissionID, std::uint8_t byType);

    // IDA: ?AddDailyMission@CGocDailyMission@@QEAAXAEAUST_DAILY_MISSION_INFO@@@Z (0x14004F180)
    void AddDailyMission(ST_DAILY_MISSION_INFO& stMission);

    // IDA: ?AcceptDailyMission@CGocDailyMission@@QEAA_NK@Z (0x140050610)
    bool AcceptDailyMission(std::uint32_t dwMissionID);

    // IDA: ?CompleteDailyMission@CGocDailyMission@@QEAA_NKV?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@@Z (0x140054960)
    bool CompleteDailyMission(std::uint32_t dwMissionID, std::shared_ptr<CDailyMissionInfo> pMission);

    // IDA: ?UpdateKillType@CGocDailyMission@@QEAAXKK@Z (0x1400539D0)
    void UpdateKillType(std::uint32_t dwObjectID, std::uint32_t dwCondition);

    // IDA: ?UpdateMazeClearType@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_TARGET@@KFK_N@Z (0x140053D00)
    void UpdateMazeClearType(E_DAILY_MISSION_TARGET eTarget, std::uint32_t dwObjectID,
                            std::int16_t shRank, std::uint32_t dwTime, bool bPartyWith);

    // IDA: ?UpdateFriendType@CGocDailyMission@@QEAAXAEAV?$vector@UST_DAILY_MISSION_FRIEND_RES@@V?$allocator@UST_DAILY_MISSION_FRIEND_RES@@@std@@@std@@@Z (0x140054080)
    void UpdateFriendType(std::vector<ST_DAILY_MISSION_INFO>& vecMission);

    // IDA: ?UpdateCollectType@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_TARGET@@KH@Z (0x140054360)
    void UpdateCollectType(E_DAILY_MISSION_TARGET eTarget, std::uint32_t dwObjectID, int nAddCount);

    // IDA: ?UpdateMyRoomType@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_CONDITION@@M@Z (0x140054670)
    void UpdateMyRoomType(E_DAILY_MISSION_CONDITION eCondition, float fValue);

    // IDA: ?CheatChangeMission@CGocDailyMission@@QEAA_NKK@Z (0x1400559D0)
    bool CheatChangeMission(std::uint32_t dwTargetID, std::uint32_t dwNewMissionID);

    // IDA: ?CheatDeleteAllMission@CGocDailyMission@@QEAA_NXZ (0x140055A50)
    bool CheatDeleteAllMission();

    // IDA: ?CheatDeleteMission@CGocDailyMission@@QEAA_NK@Z (0x140055AF0)
    bool CheatDeleteMission(std::uint32_t dwMissionID);

    // IDA: ?CheatAddMission@CGocDailyMission@@QEAA_NK@Z (0x140055DB0)
    bool CheatAddMission(std::uint32_t dwMissionID);

    // IDA: ?CheatChangeGuerillaMission@CGocDailyMission@@QEAA_NKPEA_W0@Z (0x140055F90)
    bool CheatChangeGuerillaMission(std::uint32_t dwNewMissionID, const wchar_t* szStart, const wchar_t* szEnd);

    // IDA: ?SetNewDailyMissionList@CGocDailyMission@@QEAAXAEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x14004EF80)
    void SetNewDailyMissionList(std::vector<std::uint32_t>& vecNewMission);

    // IDA: ?GenerateDailyMission@CGocDailyMission@@QEAA_N_N00AEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x1400517B0)
    bool GenerateDailyMission(bool bSpecial, bool bGuerrilla, bool bEvent,
                              std::vector<std::uint32_t>& vecNewMission);

    // IDA: ?FriendCheckDailyMission@CGocDailyMission@@QEAAXV?$vector@KV?$allocator@K@std@@@std@@EEAEAV?$vector@UST_DAILY_MISSION_FRIEND_RES@@V?$allocator@UST_DAILY_MISSION_FRIEND_RES@@@std@@@3@@Z (0x140053910)
    void FriendCheckDailyMission(std::vector<std::uint32_t>& vecReq,
                                 std::uint8_t byClass, std::uint8_t byLevel,
                                 std::vector<ST_DAILY_MISSION_FRIEND_RES>& vecRes);

    // IDA: ?UpdateFriendType@CGocDailyMission@@QEAAXAEAV?$vector@UST_DAILY_MISSION_FRIEND_RES@@V?$allocator@UST_DAILY_MISSION_FRIEND_RES@@@std@@@std@@@Z (0x140054080)
    void UpdateFriendType(std::vector<ST_DAILY_MISSION_FRIEND_RES>& vecMission);

protected:
    // Helper methods (to be implemented based on IDA analysis)
    bool CheckDailyMissionTime(ST_DAILY_MISSION_INFO* stInfo);
    bool CheckDailyMissionReward(std::uint32_t dwMissionID);
    bool CheckUpdateKillType(std::uint32_t dwMissionID, std::uint32_t dwObjectID, std::uint32_t dwCondition);
    bool CheckUpdateMazeClearType(std::uint32_t dwMissionID, std::uint32_t dwObjectID,
                                  std::int16_t shRank, std::uint32_t dwTime);
    bool CheckUpdateFriendType(std::uint32_t dwMissionID, std::uint8_t byClass,
                               std::uint8_t byLevel, ST_DAILY_MISSION_FRIEND_RES& stResult);
    bool CheckDailyMissionInfo(std::map<std::uint32_t, ST_DAILY_MISSION_INFO>& mapMission);

    void DBUpdateMissionInfo(PS_DAILY_MISSION_UPDATE* psUpdate);
    void DBDailyMissionPost(std::uint32_t dwMissionID);
    void SendDailyMissionUpdateList(PS_DAILY_MISSION_UPDATE* psUpdate);

    // IDA: ?SetDailyMissionList@CGocDailyMission@@QEAAXAEAV?$map@KUST_DAILY_MISSION_INFO@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKUST_DAILY_MISSION_INFO@@@std@@@3@@std@@@Z (0x14004F100)
    void SetDailyMissionList(std::map<std::uint32_t, ST_DAILY_MISSION_INFO>& mapInfo);

    // IDA: ?GetDailyMissionList@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_FINISH@@AEAV?$vector@V?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@V?$allocator@V?$shared_ptr@VCDailyMissionInfo@@@tr1@std@@@3@@std@@@Z (0x140051350)
    void GetDailyMissionList(E_DAILY_MISSION_FINISH eType,
                             std::vector<std::shared_ptr<CDailyMissionInfo>>* vecList);

    // IDA: ?GetDailyMissionList@CGocDailyMission@@QEAAXW4E_DAILY_MISSION_FINISH@@AEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x140051580)
    void GetDailyMissionList(E_DAILY_MISSION_FINISH eType,
                             std::vector<std::uint32_t>* vecList);

    // IDA: ?CalculateMissionRemainTime@CGocDailyMission@@QEAAKVCTime@ATL@@0@Z (0x140050510)
    std::uint32_t CalculateMissionRemainTime(ATL::CTime tNow, ATL::CTime tStart);

    // IDA: ?CalculateGetMissionDurationTime@CGocDailyMission@@QEAAKVCTime@ATL@@00@Z (0x140050570)
    std::uint32_t CalculateGetMissionDurationTime(ATL::CTime tNow, ATL::CTime tStart, ATL::CTime tEnd);

    // IDA: ?AddNewDailyMission@CGocDailyMission@@QEAAXKVCTime@ATL@@@Z (0x14004F000)
    void AddNewDailyMission(std::uint32_t dwMissionID, ATL::CTime tNow);

    // IDA: ?GeneraterTimeRange@CGocDailyMission@@QEAAXKEVCTime@ATL@@AEAV23@1_N@Z (0x14004F960)
    void GeneraterTimeRange(std::uint32_t dwMissionID, std::uint8_t byType,
                            ATL::CTime tNow, ATL::CTime& tStart, ATL::CTime& tEnd,
                            bool bUseTableDate);

    void GetDailyMissionList(E_DAILY_MISSION_FINISH_TYPE eType,
                            std::vector<std::shared_ptr<CDailyMissionInfo>>* vecList);

private:
    // IDA: member variables (offsets relative to GOComponent base)
    // Maps storing mission info by mission ID, keyed by mission type
    std::map<std::uint32_t, std::shared_ptr<CDailyMissionInfo>> m_mapSpecial;      // Special missions (type 1)
    std::map<std::uint32_t, std::shared_ptr<CDailyMissionInfo>> m_mapGuerrilla;    // Guerrilla missions (type 2)
    std::map<std::uint32_t, std::shared_ptr<CDailyMissionInfo>> m_mapEvent;        // Event missions (type 3)
    int m_nHelperCount;  // Number of active helper missions (max 4)
};
