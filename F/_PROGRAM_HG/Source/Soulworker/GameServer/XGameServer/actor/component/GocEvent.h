#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerLogin.h"
#include <cstdint>
#include <map>
#include <vector>
#include <ctime>

// Forward declarations
class CUser;
class CMover;
struct PS_WORLD_EVENT_INFO_RES;
struct ST_WORLD_EVENT_REWARD_INFO;
struct PS_WORLD_EVENT_INFO_REQ;
struct PS_WORLD_EVENT_REGISTER_REQ;
struct PS_WORLD_EVENT_REWARD_REQ;
struct PS_WORLD_EVENT_DAILY_REWARD_REQ;
struct PS_DB_WORLD_EVENT_INFO_RES;
struct PS_DB_WORLD_EVENT_REGISTER_RES;
struct PS_DB_WORLD_EVENT_REWARD;
struct PS_DB_WORLD_EVENT_DAILY_REWARD;
struct PS_ROULETTE_EVENT_UPDATE_SERVER;
struct PS_NETCAFE_MISSION_LIST;

/**
 * @brief ST_WORLD_EVENT_BOOSTER - 世界事件增益信息
 * 来自 IDA CGocEvent::SetWorldEventInfo
 */
#ifndef GREENDAMTAN_ST_WORLD_EVENT_BOOSTER_DEFINED
#define GREENDAMTAN_ST_WORLD_EVENT_BOOSTER_DEFINED
struct ST_WORLD_EVENT_BOOSTER {
    ST_WORLD_EVENT_BOOSTER()
        : nEventID(0)
        , nTotalCount(0)
        , nMyCount(0)
        , biLastRegisterDate(0)
        , biDailyRewardDate(0)
    {
    }

    int nEventID;                    // 事件ID
    int nTotalCount;                 // 总计数
    union {
        int nMyCount;                // 我的计数
        int nBoosterID;              // IDA ST_WORLD_EVENT_BOOSTER +0x08
    };
    union {
        std::int64_t biLastRegisterDate; // 最后注册日期
        std::int64_t biStart;            // IDA ST_WORLD_EVENT_BOOSTER +0x10
    };
    union {
        std::int64_t biDailyRewardDate;  // 每日奖励日期
        std::int64_t biEnd;              // IDA ST_WORLD_EVENT_BOOSTER +0x18
    };
};
#endif

/**
 * @brief ST_LEVEL_UP_EVENT_DATA - 升级事件数据 (用于世界事件奖励)
 * 来自 IDA CGocEvent::AddWorldEventReward
 */
struct ST_LEVEL_UP_EVENT_DATA {
    int nRewardIndex = 0;      // 奖励索引
    std::uint8_t byRewardType = 0;   // 奖励类型
    std::uint8_t byRewardState = 0;  // 奖励状态
    std::uint8_t _pad0[2] = {};
};

/**
 * @brief ST_NETCAFE_MISSION_INFO - 网吧任务信息
 * 来自 IDA CGocEvent::LoadNetCafeMission
 */
struct ST_NETCAFE_MISSION_INFO {
    std::uint32_t dwID = 0;            // 任务ID
    std::int64_t nStartTime = 0;       // 开始时间
    std::int64_t nEndTime = 0;         // 结束时间
    std::uint32_t dwValue = 0;         // 当前值
    std::int64_t nUpdateTime = 0;      // 更新时间
};

/**
 * @brief CGocEvent - Game Object Component for event handling
 *
 * Handles game events including:
 * - World Events (世界事件)
 * - Roulette Events (轮盘事件)
 * - NetCafe Missions (网吧任务)
 * - Account Events (账号事件)
 *
 * 来自 IDA 反编译 GameServer.exe (port 10004)
 */
class CGocEvent : public GOComponent {
public:
    CGocEvent();
    virtual ~CGocEvent();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Static
    static int GetFamilyID();

    // Initialization and cleanup
    void Init();
    void LogOut();

    // Account Event functions
    void RequestLoadAccountEvent();
    void LoadAccountEvent(PS_ACCOUNT_EVENT_LIST& stEventList);
    bool CheckAccountEvent(unsigned int dwEventID);

    // World Event functions
    std::uint8_t SetWorldEventInfo(PS_WORLD_EVENT_INFO_RES& psRes, std::int64_t biLastRegisterDate, std::int64_t biDailyRewardDate);
    std::uint8_t SetWorldEventInfo(int nEventID, int nTotalCount, int nMyCount, std::int64_t biLastRegisterDate, std::int64_t biDailyRewardDate);
    bool FindWorldEventReward(int nRewardIndex);
    bool AddWorldEventReward(ST_LEVEL_UP_EVENT_DATA& stInfo);
    int GetWorldEventTotalCount(int nEventID);
    int GetWorldEventMyCount(int nEventID);
    std::int64_t GetWorldEventLastResisterDate(int nEventID);
    std::int64_t GetWorldEventDailyRewardDate(int nEventID);

    // World Event Request/Response
    int ReqWorldEventInfo(PS_WORLD_EVENT_INFO_REQ& psReq);
    int ReqWorldEventRegister(PS_WORLD_EVENT_REGISTER_REQ& psReq);
    int ReqWorldEventReward(PS_WORLD_EVENT_REWARD_REQ& psReq);
    int ReqWorldEventDailyReward(PS_WORLD_EVENT_DAILY_REWARD_REQ& psReq);
    void ResWorldEventInfo(PS_DB_WORLD_EVENT_INFO_RES& psRes);
    void ResWorldEventRegister(PS_DB_WORLD_EVENT_REGISTER_RES& psRes);
    void ResWorldEventReward(PS_DB_WORLD_EVENT_REWARD& psRes);
    void ResWorldEventDailyReward(PS_DB_WORLD_EVENT_DAILY_REWARD& psRes);

    // Roulette Event functions
    void SendDBRouletteInfo(std::uint8_t byUseType, int nEventID);
    void LoadRouletteEventInfo(PS_ROULETTE_INFO& psInfo);
    void SendRouletteEventInfo();
    void SetRouletteDayCount(int nCount);
    int IsRouletteEvent();
    void InitRouletteDayCount(std::int64_t biInitTime);
    bool IsUseRoulette();
    int GetRouletteDayCount();
    void ResetUseRoulette();

    // NetCafe Mission functions
    void SetStartNetCafeMission(bool bStart);
    void LoadNetCafeMission(PS_NETCAFE_MISSION_LIST& psInfo);
    bool GetNetCafeMissionTime(unsigned int dwID, std::time_t& tCurr, std::time_t& tStart, std::time_t& tEnd);
    void SendNetCafeMissionInfo(unsigned int dwID);
    void DBUpdateNetCafeMission(unsigned int dwID, bool bInit);
    void AllDBUpdateNetCafeMission();
    void Cheat_NetCafeMission_PlayTime(unsigned int dwID, bool bClear, int nValue);

protected:
    // World Event data
    std::map<int, ST_WORLD_EVENT_BOOSTER> m_mapWorldEvent;          // 世界事件信息map
    std::map<int, ST_LEVEL_UP_EVENT_DATA> m_mapWorldEventReward;    // 世界事件奖励map
    bool m_bWorldEventDBCall = false;                               // 世界事件DB调用标志

    // Roulette Event data
    // IDA: m_psRouletteInfo 是嵌入结构，不是指针
    PS_ROULETTE_INFO m_psRouletteInfo;                              // 轮盘信息
    bool m_bRouletteUse = false;                                    // 轮盘使用标志

    // NetCafe Mission data
    std::map<unsigned long, ST_NETCAFE_MISSION_INFO> m_mapNetCafeMission;  // 网吧任务map
    bool m_bNetCafeMission = false;                                 // 网吧任务标志
    int m_nNetCafeNextDay = 0;                                      // 下一天时间
    std::uint64_t m_dw64NetCafeUpdateTick = 0;                      // 更新tick
    int m_nNetCafeMission_InitHour = 9;                             // 初始化小时 (默认9点)
    std::uint64_t m_dw64NetCafeDBUpdateTick = 0;                    // DB更新tick
};
