#pragma once

#include <cstdint>
#include <map>
#include <memory>

// Forward declarations
struct TB_WEEK_MISSION;

// ST_WEEKLY_MISSION_INFO is defined in PSServerDB.h

/**
 * @brief CWeeklyMissionInfo - Weekly mission info class
 * IDA: Clear at 0x14017AE20
 */
class CWeeklyMissionInfo {
public:
    CWeeklyMissionInfo();
    ~CWeeklyMissionInfo();

    // IDA: ?Clear@CWeeklyMissionInfo@@QEAAXXZ (0x14017AE20)
    void Clear();

    // IDA: ?UpdateDate@CWeeklyMissionInfo@@QEAAX_J@Z (0x140168250)
    void UpdateDate(std::int64_t nDate);

    // IDA: ?CheckWeeklyMissionUpdate_Mission@CWeeklyMissionInfo@@QEAAXEHHAEAUST_WEEKLY_MISSION_INFO@@@Z (0x14017AE60)
    void CheckWeeklyMissionUpdate_Mission(std::uint8_t byType, std::uint32_t nObjectID, std::int16_t nCount, struct ST_WEEKLY_MISSION_INFO* pstMissionInfo);

    // IDA: ?CheckWeeklyMissionReset_Mission@CWeeklyMissionInfo@@QEAA_N_J@Z (0x14017B180)
    bool CheckWeeklyMissionReset_Mission(std::int64_t nStartDate);

    // Accessors
    std::uint32_t GetMissionID() const { return m_stMissionInfo.dwMissionID; }
    std::int16_t GetValue() const { return m_stMissionInfo.shValue; }
    std::uint8_t GetState() const { return m_stMissionInfo.byState; }
    std::int64_t GetDate() const { return m_stMissionInfo.biDate; }

    void SetMissionID(std::uint32_t dwID) { m_stMissionInfo.dwMissionID = dwID; }
    void SetValue(std::int16_t shValue) { m_stMissionInfo.shValue = shValue; }
    void SetState(std::uint8_t byState) { m_stMissionInfo.byState = byState; }
    void SetDate(std::int64_t biDate) { m_stMissionInfo.biDate = biDate; }

protected:
    // Mission info structure (matches ST_WEEKLY_MISSION_INFO layout)
    struct MISSION_INFO {
        std::uint32_t dwMissionID = 0;
        std::int16_t shValue = 0;
        std::uint8_t byState = 0;
        std::uint8_t _padding = 0;
        std::int64_t biDate = 0;
    };

    TB_WEEK_MISSION* m_pTB_WEEK_MISSION = nullptr;
    MISSION_INFO m_stMissionInfo;
};

/**
 * @brief CWeeklyMission_Day - Weekly mission day class
 * IDA: Clear at 0x14017B1B0
 */
class CWeeklyMission_Day {
public:
    CWeeklyMission_Day();
    ~CWeeklyMission_Day();

    // IDA: ?Clear@CWeeklyMission_Day@@QEAAXXZ (0x14017B1B0)
    void Clear();

    // IDA: ?CheckWeeklyMissionReset_Day@CWeeklyMission_Day@@QEAA_N_J@Z (0x14017BDC0)
    bool CheckWeeklyMissionReset_Day(std::int64_t nStartDate);

    // IDA: ?GetWeekMission_Mission@CWeeklyMission_Day@@QEAAXAEAUST_WEEKLY_MISSION_DAY_INFO@@@Z
    void GetWeekMission_Mission(struct ST_WEEKLY_MISSION_DAY_INFO* pstDayInfo);

    // IDA: ?IsReceivedRewardMission_Day@CWeeklyMission_Day@@QEAA_NXZ
    bool IsReceivedRewardMission_Day();

    // IDA: ?IsCompleteMission_Day@CWeeklyMission_Day@@QEAA_NXZ
    bool IsCompleteMission_Day();

    // Accessors
    std::uint16_t GetDay() const { return m_wDay; }
    void SetDay(std::uint16_t wDay) { m_wDay = wDay; }

protected:
    // Map of mission ID to mission info
    // IDA: m_mapWeekMission_Info is std::map<unsigned long, std::shared_ptr<CWeeklyMissionInfo>>
    std::map<std::uint32_t, std::shared_ptr<CWeeklyMissionInfo>> m_mapWeekMission_Info;

    std::uint16_t m_wDay = 0;
};

/**
 * @brief CWeeklyMission_Group - Weekly mission group class
 * IDA: Clear at 0x14017C110
 */
class CWeeklyMission_Group {
public:
    CWeeklyMission_Group();
    ~CWeeklyMission_Group();

    // IDA: ?Clear@CWeeklyMission_Group@@QEAAXXZ (0x14017C110)
    void Clear();

    // IDA: ?GetGroupReward@CWeeklyMission_Group@@QEAAEXZ (0x14017ADB0)
    std::uint8_t GetGroupReward() const { return m_byReward; }

    // IDA: ?SetGroupReward@CWeeklyMission_Group@@QEAAXE@Z (0x14017ADC0)
    void SetGroupReward(std::uint8_t byReward) { m_byReward = byReward; }

    // IDA: ?GetWeekMission_Mission@CWeeklyMission_Group@@QEAAXAEAUST_WEEKLY_MISSION_GROUP_INFO@@@Z (0x14017C7D0)
    void GetWeekMission_Mission(struct ST_WEEKLY_MISSION_GROUP_INFO* pstGroupInfo);

    // IDA: ?IsReceivedRewardMission_Day@CWeeklyMission_Group@@QEAA_NXZ (0x14017C9A0)
    bool IsReceivedRewardMission_Day();

    // IDA: ?IsCompleteMission_Group@CWeeklyMission_Group@@QEAA_NK@Z (0x14017CAA0)
    bool IsCompleteMission_Group(std::uint32_t dwUCID);

    // IDA: ?CheckWeeklyMissionReset_Group@CWeeklyMission_Group@@QEAA_NXZ (0x14017CEB0)
    bool CheckWeeklyMissionReset_Group();

    // Accessors
    std::uint8_t GetGroupID() const { return m_byGroupID; }
    std::uint8_t GetGroupType() const { return m_byGroupType; }
    std::int64_t GetStartDate() const { return m_nStartDate; }
    std::int64_t GetEndDate() const { return m_nEndDate; }

    void SetGroupID(std::uint8_t byID) { m_byGroupID = byID; }
    void SetGroupType(std::uint8_t byType) { m_byGroupType = byType; }
    void SetStartDate(std::int64_t nDate) { m_nStartDate = nDate; }
    void SetEndDate(std::int64_t nDate) { m_nEndDate = nDate; }

protected:
    // Map of day index to day info
    // IDA: m_mapWeekMission_Day is std::map<unsigned short, std::shared_ptr<CWeeklyMission_Day>>
    std::map<std::uint16_t, std::shared_ptr<CWeeklyMission_Day>> m_mapWeekMission_Day;

    std::uint8_t m_byGroupID = 0;
    std::uint8_t m_byGroupType = 0;
    std::uint8_t m_byReward = 0;
    std::int64_t m_nStartDate = 0;  // ATL::CTime stored as __int64
    std::int64_t m_nEndDate = 0;    // ATL::CTime stored as __int64
};
