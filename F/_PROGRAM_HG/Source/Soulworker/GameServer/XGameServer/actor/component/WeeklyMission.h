#pragma once

#include <cstdint>
#include <map>
#include <memory>

// Forward declarations
struct TB_WEEK_MISSION;
struct ST_WEEKLY_MISSION_INFO;

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

    // Accessors
    std::uint32_t GetMissionID() const { return m_stMissionInfo.dwMissionID; }
    std::int16_t GetValue() const { return m_stMissionInfo.shValue; }
    std::uint8_t GetState() const { return m_stMissionInfo.byState; }
    std::int64_t GetDate() const { return m_stMissionInfo.biDate; }

protected:
    // Mission info structure
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

    // Accessors
    std::uint16_t GetDay() const { return m_wDay; }
    void SetDay(std::uint16_t wDay) { m_wDay = wDay; }

protected:
    // Map of mission ID to mission info
    // IDA: m_mapWeekMission_Info is std::map<unsigned long, std::tr1::shared_ptr<CWeeklyMissionInfo>>
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

    // Accessors
    std::uint8_t GetGroupID() const { return m_byGroupID; }
    std::uint8_t GetGroupType() const { return m_byGroupType; }
    std::uint8_t GetReward() const { return m_byReward; }
    std::int64_t GetStartDate() const { return m_nStartDate; }
    std::int64_t GetEndDate() const { return m_nEndDate; }

    void SetGroupID(std::uint8_t byID) { m_byGroupID = byID; }
    void SetGroupType(std::uint8_t byType) { m_byGroupType = byType; }
    void SetReward(std::uint8_t byReward) { m_byReward = byReward; }
    void SetStartDate(std::int64_t nDate) { m_nStartDate = nDate; }
    void SetEndDate(std::int64_t nDate) { m_nEndDate = nDate; }

protected:
    // Map of day index to day info
    // IDA: m_mapWeekMission_Day is std::map<unsigned short, std::tr1::shared_ptr<CWeeklyMission_Day>>
    std::map<std::uint16_t, std::shared_ptr<CWeeklyMission_Day>> m_mapWeekMission_Day;

    std::uint8_t m_byGroupID = 0;
    std::uint8_t m_byGroupType = 0;
    std::uint8_t m_byReward = 0;
    std::int64_t m_nStartDate = 0;  // ATL::CTime stored as __int64
    std::int64_t m_nEndDate = 0;    // ATL::CTime stored as __int64
};
