// ManagerStubs.h
// 各种管理器存根

#pragma once

#include <cstdint>
#include <cstring>
#include <ctime>
#include <functional>
#include <map>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"
#include "Soulworker/GameServer/XLoginServer/RouletteEventManager.h"

// 辅助函数：向所有线程广播 (1,9) 系统事件同步包（ChangeEventValue/DeleteEventValue 使用）
// 实现位于 GameSockets.cpp；避免 ManagerStubs.h 直接依赖 ThreadLocalData.h
//（XCore 旧版与 XGameServer 新版 ThreadLocalData 同名冲突）。
void GreenDamTan_BroadcastSystemEvent(const PS_SYNC_SYSTEM_EVENT& stEvent);

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

    int nEventID;
    int nTotalCount;
    union {
        int nMyCount;
        int nBoosterID;
    };
    union {
        std::int64_t biLastRegisterDate;
        std::int64_t biStart;
    };
    union {
        std::int64_t biDailyRewardDate;
        std::int64_t biEnd;
    };
};
#endif

// 前置声明
struct TB_DAILY_MISSION;

// CDailyMissionMgr - 每日任务管理器存根
class CDailyMissionMgr {
public:
    CDailyMissionMgr() = default;
    ~CDailyMissionMgr() = default;

    void InsertMission(TB_DAILY_MISSION* pMission) {
        // TODO: 对齐 IDA 实现 - 插入每日任务
    }

    // IDA: ?GenerateDailyMission@CDailyMissionMgr@@QEAAXEAEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x1401E8950)
    // Generates daily missions of specified type (1=Special, 2=Guerrilla, 3=Event)
    void GenerateDailyMission(std::uint8_t byType, std::vector<std::uint32_t>& vecMissionList) {
        // TODO: Implement from IDA - select random missions from table by type
        vecMissionList.clear();
    }
};

// CTimeEventMgr - 时间事件管理器存根
class CTimeEventMgr {
public:
    CTimeEventMgr() = default;
    ~CTimeEventMgr() = default;

    static void Update(CTimeEventMgr* pMgr) {
        // TODO: 对齐 IDA 实现
    }

    void CheckTimeEvent(std::vector<ST_GM_TIME_EVENT_INFO>& vecBoostID) {
        vecBoostID.clear();

        const std::int64_t tCurr = static_cast<std::int64_t>(std::time(nullptr));
        std::vector<std::uint32_t> vecDelList;

        {
            CFAutoSlimWriteLock lock(&m_rwTimeEventLock);
            for (const auto& pair : m_mapTimeEvent) {
                const ST_GM_TIME_EVENT_INFO& info = pair.second;
                const bool bAlwaysDelete = (info.byteUse != 0);
                const bool bActive = !bAlwaysDelete && info.nStartDate <= tCurr && tCurr < info.nEndDate;
                if (bActive) {
                    vecBoostID.push_back(info);
                } else if (bAlwaysDelete || info.nEndDate < tCurr) {
                    vecDelList.push_back(pair.first);
                }
            }
        }

        {
            CFAutoSlimWriteLock lock(&m_rwTimeEventLock);
            for (std::uint32_t key : vecDelList) {
                auto iterFind = m_mapTimeEvent.find(key);
                if (iterFind != m_mapTimeEvent.end()) {
                    m_mapTimeEvent.erase(iterFind);
                }
            }
        }
    }

    // IDA: ?AddTimeEvent@CTimeEventMgr@@QEAA_NAEAUST_GM_TIME_EVENT_INFO@@@Z (0x1406E0A80)
    // 按 dwNo 查找 m_mapTimeEvent；找到时 byteFlag 非零则删除，否则覆盖；
    // 未找到且 byteFlag 为零则插入。始终返回 true。
    bool AddTimeEvent(ST_GM_TIME_EVENT_INFO& stInfo) {
        CFAutoSlimWriteLock lock(&m_rwTimeEventLock);
        auto iter = m_mapTimeEvent.find(stInfo.dwNo);
        if (iter != m_mapTimeEvent.end()) {
            if (stInfo.byteFlag) {
                m_mapTimeEvent.erase(iter);
            } else {
                iter->second = stInfo;
            }
        } else if (!stInfo.byteFlag) {
            m_mapTimeEvent[stInfo.dwNo] = stInfo;
        }
        return true;
    }

    // IDA: ?CheckValueEvent@CTimeEventMgr@@QEAA_NW4ENUM_GM_VALUE_EVENT_TYPE@@AEAUST_GM_VALUE_EVENT_INFO@@@Z (0x1406E1100)
    // 校验数值事件：类型 1..3，事件已注册且在 [nStartDate, nEndDate) 期间内则拷贝并返回 true。
    bool CheckValueEvent(int eEventType, ST_GM_VALUE_EVENT_INFO& stInfo) {
        if (eEventType <= 0 || eEventType >= 4) {
            return false;
        }
        const std::int64_t tCurr = static_cast<std::int64_t>(std::time(nullptr));
        CFAutoSlimReadLock lock(&m_rwValueEventLock);
        if (m_ValueEvent[eEventType].byEventType
            && m_ValueEvent[eEventType].nStartDate <= tCurr
            && tCurr < m_ValueEvent[eEventType].nEndDate) {
            stInfo = m_ValueEvent[eEventType];
            return true;
        }
        return false;
    }

    // IDA: ?GetEventValue@CTimeEventMgr@@QEAAHH@Z (0x1406E1630)
    int GetEventValue(int nEventType) {
        ST_GM_VALUE_EVENT_INFO stInfo{};
        if (CheckValueEvent(nEventType, stInfo)) {
            return stInfo.dwEvent_Val;
        }
        return 0;
    }

    // IDA: ?AddValueEvent@CTimeEventMgr@@QEAAXAEAUPS_GM_VALUE_EVENT_LIST@@@Z (0x1406E0E70)
    // 写锁下清零 m_nEventState；对列表内类型 1..3 的事件，与现有 m_ValueEvent 逐字段相
    // 等则保持 state=2（无需变更），否则整体覆盖并置 state=1；随后按 state 对类型 1..3
    // 调用 ChangeEventValue（新值）或 DeleteEventValue（原事件被移除）。
    void AddValueEvent(PS_GM_VALUE_EVENT_LIST& psList) {
        CFAutoSlimWriteLock lock(&m_rwValueEventLock);
        std::memset(m_nEventState, 0, sizeof(m_nEventState));
        for (std::size_t i = 0; i < psList.vecEvent.size(); ++i) {
            const ST_GM_VALUE_EVENT_INFO& stInfo = psList.vecEvent[i];
            if (stInfo.byEventType && stInfo.byEventType < 4) {
                if (m_ValueEvent[stInfo.byEventType].dwNo == stInfo.dwNo
                    && m_ValueEvent[stInfo.byEventType].byEventType == stInfo.byEventType
                    && m_ValueEvent[stInfo.byEventType].dwEvent_Val == stInfo.dwEvent_Val
                    && m_ValueEvent[stInfo.byEventType].nStartDate == stInfo.nStartDate
                    && m_ValueEvent[stInfo.byEventType].nEndDate == stInfo.nEndDate) {
                    m_nEventState[stInfo.byEventType] = 2;
                } else {
                    m_ValueEvent[stInfo.byEventType] = stInfo;
                    m_nEventState[stInfo.byEventType] = 1;
                }
            }
        }
        for (int nEventType = 1; nEventType < 4; ++nEventType) {
            if (m_nEventState[nEventType] == 1) {
                ChangeEventValue(m_ValueEvent[nEventType]);
            } else if (!m_nEventState[nEventType] && m_ValueEvent[nEventType].byEventType) {
                DeleteEventValue(nEventType);
            }
        }
    }

    // IDA: ?ChangeEventValue@CTimeEventMgr@@QEAAXAEAUST_GM_VALUE_EVENT_INFO@@@Z (0x1406E1300)
    // 数值事件类型 2 时经 DoJobAllThread 广播 (1,9) 系统事件同步包到所有线程。
    void ChangeEventValue(ST_GM_VALUE_EVENT_INFO& stInfo) {
        if (stInfo.byEventType == 2) {
            PS_SYNC_SYSTEM_EVENT stEvent;
            stEvent.nEventType = 2;
            stEvent.nEventValue = stInfo.dwEvent_Val;
            CLogicThreadManager::Instance().DoJobAllThread([stEvent]() {
                GreenDamTan_BroadcastSystemEvent(stEvent);
            });
        }
        LogHelper::LogDebug("game.system", "[SYSTEM_EVENT] ChangeEventValue %d %d",
            stInfo.byEventType, stInfo.dwEvent_Val);
    }

    // IDA: ?DeleteEventValue@CTimeEventMgr@@QEAAXH@Z (0x1406E1460)
    // 数值事件类型 2 时经 DoJobAllThread 广播 (1,9) 系统事件同步包（值清零）到所有线程，
    // 随后将该类型事件注销。
    void DeleteEventValue(int nEventType) {
        if (nEventType == 2) {
            PS_SYNC_SYSTEM_EVENT stEvent;
            stEvent.nEventType = 2;
            stEvent.nEventValue = 0;
            CLogicThreadManager::Instance().DoJobAllThread([stEvent]() {
                GreenDamTan_BroadcastSystemEvent(stEvent);
            });
        }
        m_ValueEvent[nEventType].byEventType = 0;
        LogHelper::LogDebug("game.system", "[SYSTEM_EVENT] DeleteEventValue %d", nEventType);
    }

    // IDA: ?SetRouletteEvent@CTimeEventMgr@@QEAAXAEAUPS_GM_ROULETTE_EVENT@@@Z (0x1406E17B0)
    // 写锁下整体覆盖轮盘活动信息。
    void SetRouletteEvent(PS_GM_ROULETTE_EVENT& psInfo) {
        CFAutoSlimWriteLock lock(&m_rwRouletteEventLock);
        m_psRouletteEventInfo = psInfo;
    }

    // IDA: ?GetRouletteEventInfo@CTimeEventMgr@@QEAA_NAEAUPS_ROULETTE_EVENT@@@Z (0x1406E1950)
    // 轮盘活动在 [biStartDate, biEndDate) 期间内时填充客户端 PS_ROULETTE_EVENT 并返回 true。
    bool GetRouletteEventInfo(PS_ROULETTE_EVENT& psInfo) {
        const std::int64_t tCurr = static_cast<std::int64_t>(std::time(nullptr));
        if (m_psRouletteEventInfo.biStartDate > tCurr || tCurr >= m_psRouletteEventInfo.biEndDate) {
            return false;
        }
        std::wcscpy(psInfo.szTitle, m_psRouletteEventInfo.szTitle);
        psInfo.nEventID = m_psRouletteEventInfo.nEventID;
        psInfo.byUseType = m_psRouletteEventInfo.byUseType;
        psInfo.nDayLimit = m_psRouletteEventInfo.nUseCount;
        psInfo.biStartDate = m_psRouletteEventInfo.biStartDate;
        psInfo.biEndDate = m_psRouletteEventInfo.biEndDate;
        psInfo.byCostType = m_psRouletteEventInfo.byCostType;
        psInfo.nCostID = m_psRouletteEventInfo.nCostID;
        psInfo.nCostCount = m_psRouletteEventInfo.nCostCount;
        psInfo.psRewardItemList.vecInfo.clear();
        for (const ST_GM_ROULETTE_EVENT_ITEM& stReward : m_psRouletteEventInfo.psRewardList.vecInfo) {
            ST_ROULETTE_EVENT_ITEM stItem;
            stItem.nRewardID = stReward.nRewradIndex;
            stItem.nItemID = stReward.nItemID;
            stItem.nCount = stReward.nCount;
            psInfo.psRewardItemList.vecInfo.push_back(stItem);
        }
        return true;
    }

    // IDA: ?GetRouletteEventReward@CTimeEventMgr@@QEAA_NAEAUPS_GM_ROULETTE_EVENT@@@Z (0x1406E1B40)
    // 轮盘活动有效（nEventID 非零且在日期窗口内）时整体返回轮盘信息并返回 true。
    bool GetRouletteEventReward(PS_GM_ROULETTE_EVENT& psReward) {
        const std::int64_t tCurr = static_cast<std::int64_t>(std::time(nullptr));
        if (!m_psRouletteEventInfo.nEventID
            || m_psRouletteEventInfo.biStartDate > tCurr
            || tCurr >= m_psRouletteEventInfo.biEndDate) {
            return false;
        }
        psReward = m_psRouletteEventInfo;
        return true;
    }

    CFSRWLock m_rwTimeEventLock;
    CFSRWLock m_rwValueEventLock;
    CFSRWLock m_rwRouletteEventLock;
    std::map<std::uint32_t, ST_GM_TIME_EVENT_INFO> m_mapTimeEvent;
    // IDA: m_ValueEvent: ST_GM_VALUE_EVENT_INFO[4] @ offset 56
    ST_GM_VALUE_EVENT_INFO m_ValueEvent[4];
    // IDA: m_nEventState: int[4] @ offset 216
    std::int32_t m_nEventState[4];
    // IDA: m_psRouletteEventInfo: PS_GM_ROULETTE_EVENT @ offset 232
    PS_GM_ROULETTE_EVENT m_psRouletteEventInfo;
};

// CDayEventMgr - 已在 DayEventManager.h 中完整定义
// 前置声明供 GameServer 使用
class CDayEventMgr;

// CWorldEventMgr - 世界事件管理器存根
class CWorldEventMgr {
public:
    CWorldEventMgr() = default;
    ~CWorldEventMgr() = default;

    static void Update(CWorldEventMgr* pMgr, std::uint64_t dwTick) {
        // TODO: 对齐 IDA 实现
    }

    void UpdateTotalCount(int nEventID, int nTotalCount);

    void CheckWorldEvent(std::vector<ST_WORLD_EVENT_BOOSTER>& vecBoostID) {
        vecBoostID.clear();

        const std::int64_t tCurr = static_cast<std::int64_t>(std::time(nullptr));
        CFAutoSlimWriteLock lock(&m_rwWorldEventLock);
        std::vector<int> vecDelList;

        for (const auto& pair : m_mapWorldEventBooster) {
            const ST_WORLD_EVENT_BOOSTER& info = pair.second;
            if (info.nBoosterID <= 0) {
                continue;
            }

            const bool bActive = info.biStart <= tCurr && tCurr < info.biEnd;
            if (bActive) {
                vecBoostID.push_back(info);
            } else if (info.biEnd < tCurr) {
                vecDelList.push_back(pair.first);
            }
        }

        for (int key : vecDelList) {
            auto iterFind = m_mapWorldEventBooster.find(key);
            if (iterFind != m_mapWorldEventBooster.end()) {
                m_mapWorldEventBooster.erase(iterFind);
            }
        }
    }

    CFSRWLock m_rwWorldEventLock;
    std::map<int, ST_WORLD_EVENT_BOOSTER> m_mapWorldEventBooster;
    std::uint64_t m_dw64CurrentTick = 0;
};

// CRankingMgr - 排行榜管理器存根
class CRankingMgr {
public:
    CRankingMgr() = default;
    ~CRankingMgr() = default;

    static std::uint32_t GetLastRewardID_Date(CRankingMgr* pMgr) {
        // TODO: 对齐 IDA 实现
        return 1;
    }
    static void LoadRankingListReq(CRankingMgr* pMgr, std::uint64_t dwTick) {
        // TODO: 对齐 IDA 实现
    }
};

// CCalculateStatus - 前向声明 (完整定义在 GocAttribute.h 中)
class CCalculateStatus;

// CLogicThreadManager - 前向声明 (完整定义在 LogicThreadProcessor.h 中)
class CLogicThreadManager;

// CGameLogThreadManager - 游戏日志线程管理器存根
class CGameLogThreadManager {
public:
    CGameLogThreadManager() = default;
    ~CGameLogThreadManager() = default;

    static CGameLogThreadManager* Instance() { return nullptr; }
    static void Start(CGameLogThreadManager* pMgr, const char* szName) {
        // TODO: 对齐 IDA 实现
    }
    static void End(CGameLogThreadManager* pMgr) {
        // TODO: 对齐 IDA 实现 - 结束日志线程
    }
};

// Forward declarations for managers defined in XRelayServer
class CPartyMatchingMgr;
class CForceMatchingMgr;
class CModeMazeMatchingMgr;
class CLeagueManager;
class CFriendRecruitManager;
