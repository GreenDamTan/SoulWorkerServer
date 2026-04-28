// DayEventManager.cpp
// CDayEventMgr 日常活动管理器实现
// 对齐 ControlServer.exe IDA

#include "DayEventManager.h"
#include "Soulworker/GameServer/XControlServer/ControlServer.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

#ifdef _WIN32
#include <intrin.h>
#endif
#include <algorithm>
#include <cstdlib>
#include <list>
#include <vector>
#include <random>

// ============================================================================
// Update - 对齐 IDA 0x14002D9A0
// ============================================================================
void CDayEventMgr::Update(unsigned __int64 dw64Tick)
{
    // 对齐 IDA: CDayEventMgr::Update
    // 检查是否到达更新时间
    if (m_dw64Tick <= dw64Tick) {
        if (m_dw64Tick) {
            // 已初始化，检查是否在9点需要更新
            ATL::CTime tCurr = ATL::CTime::GetTickCount();
            if (tCurr.GetHour() == 9) {
                // 检查 m_tLastSetTime + 1天 <= 当前时间
                ATL::CTimeSpan tOneDay(1, 0, 0, 0);
                ATL::CTime tNextSetTime = m_tLastSetTime + tOneDay;
                if (tNextSetTime <= tCurr) {
                    SetDayEvent();
                    SendDayEventAllGameServer();
                }
            }
        } else {
            // 首次初始化，检查 LogDB 连接
            XControlServer* pServer = XControlServer::Instance();
            if (pServer && pServer->IsLogDBConnection()) {
                SetDayEvent();
                SendDayEventAllGameServer();
            }
        }
        // 设置下次更新时间 (60秒后)
        m_dw64Tick = dw64Tick + 60000;
    }
}

// ============================================================================
// SetDayEvent - 对齐 IDA 0x14002DAF0
// ============================================================================
void CDayEventMgr::SetDayEvent()
{
    // 对齐 IDA: CDayEventMgr::SetDayEvent
    CFAutoSlimWriteLock lock(&m_rwEventLock);
    LogHelper::LogInfo("game.system", "[DayEvent] SetDayEvent Start");
    m_mapDayEvent.clear();

    // 获取当前时间，如果早于9点则减1天
    ATL::CTime tSetDay = ATL::CTime::GetTickCount();
    if (tSetDay.GetHour() < 9) {
        ATL::CTimeSpan tOneDay(1, 0, 0, 0);
        tSetDay = tSetDay - tOneDay;
    }

    // 获取星期几 (ATL::CTime::GetDayOfWeek 返回 1=周日, 2=周一, ..., 7=周六)
    // 但 IDA 中逻辑期望 1=周一, ..., 7=周日
    int nDayOfWeek = tSetDay.GetDayOfWeek();
    // 转换: ATL的 GetDayOfWeek 返回 1=周日, 2=周一...
    // 转换为 1=周一, 2=周二..., 7=周日
    int nDay = nDayOfWeek - 1;
    if (nDay < 1) nDay = 7;  // 周日从 1 变为 7

    // 获取日常活动迷宫ID列表
    XControlServer* pServer = XControlServer::Instance();
    if (!pServer) {
        LogHelper::LogError("game.system", "[DayEvent] SetDayEvent - XControlServer Instance NULL");
        return;
    }

    std::list<unsigned int> listMazeID;
    pServer->GetResourceMgr().GetDayEvent(nDay, listMazeID);

    // 遍历每个迷宫ID
    for (auto it = listMazeID.begin(); it != listMazeID.end(); ++it) {
        unsigned int dwMazeID = *it;

        // 获取 TB_DAY_EVENT 配置
        TB_DAY_EVENT* pTB_DAY_EVENT = pServer->GetResourceMgr().GetTB_DAY_EVENT(dwMazeID);
        if (!pTB_DAY_EVENT) {
            LogHelper::LogError("game.contents", "[DayEvent] SetDayEvent - ERROR [Maze :%u]", dwMazeID);
            continue;
        }

        // 随机事件数量
        int nEventCount_Ori = pServer->GetItemFactory().nRand(pTB_DAY_EVENT->Event_Count_Min, pTB_DAY_EVENT->Event_Count_Max);
        int nEventCount = nEventCount_Ori;

        if (nEventCount <= 0 && pTB_DAY_EVENT->Event_Count_Min > 0) {
            LogHelper::LogDebug("game.contents", "[DayEvent] SetDayEvent - Zero Event Count [Maze :%u, Count :%d]", dwMazeID, nEventCount);
            continue;
        }

        // 选择迷宫列表
        std::vector<ST_DAY_EVENT_INFO> vecSelectMazeEP;
        vecSelectMazeEP.reserve(nEventCount);

        // 先处理固定选择
        for (int i = 0; i < 4 && nEventCount > 0; ++i) {
            unsigned short wMazeID = 0;
            unsigned short wBoosterID = 0;
            switch (i) {
                case 0: wMazeID = pTB_DAY_EVENT->Fix_Maze_Select_1; wBoosterID = pTB_DAY_EVENT->Fix_Maze_Booster_Group_1; break;
                case 1: wMazeID = pTB_DAY_EVENT->Fix_Maze_Select_2; wBoosterID = pTB_DAY_EVENT->Fix_Maze_Booster_Group_2; break;
                case 2: wMazeID = pTB_DAY_EVENT->Fix_Maze_Select_3; wBoosterID = pTB_DAY_EVENT->Fix_Maze_Booster_Group_3; break;
                case 3: wMazeID = pTB_DAY_EVENT->Fix_Maze_Select_4; wBoosterID = pTB_DAY_EVENT->Fix_Maze_Booster_Group_4; break;
            }
            if (wMazeID && wBoosterID) {
                ST_DAY_EVENT_INFO stInfo{};
                stInfo.wMazeID = wMazeID;
                stInfo.wBoosterID = wBoosterID;
                vecSelectMazeEP.push_back(stInfo);
                --nEventCount;
            }
        }

        // 随机选择池
        std::vector<ST_DAY_EVENT_INFO> vecMazeEP;
        if (nEventCount > 0) {
            for (int j = 0; j < 10; ++j) {
                unsigned short wMazeID = 0;
                unsigned short wBoosterID = 0;
                switch (j) {
                    case 0: wMazeID = pTB_DAY_EVENT->Maze_Select_1; wBoosterID = pTB_DAY_EVENT->Maze_Booster_Group_1; break;
                    case 1: wMazeID = pTB_DAY_EVENT->Maze_Select_2; wBoosterID = pTB_DAY_EVENT->Maze_Booster_Group_2; break;
                    case 2: wMazeID = pTB_DAY_EVENT->Maze_Select_3; wBoosterID = pTB_DAY_EVENT->Maze_Booster_Group_3; break;
                    case 3: wMazeID = pTB_DAY_EVENT->Maze_Select_4; wBoosterID = pTB_DAY_EVENT->Maze_Booster_Group_4; break;
                    case 4: wMazeID = pTB_DAY_EVENT->Maze_Select_5; wBoosterID = pTB_DAY_EVENT->Maze_Booster_Group_5; break;
                    case 5: wMazeID = pTB_DAY_EVENT->Maze_Select_6; wBoosterID = pTB_DAY_EVENT->Maze_Booster_Group_6; break;
                    case 6: wMazeID = pTB_DAY_EVENT->Maze_Select_7; wBoosterID = pTB_DAY_EVENT->Maze_Booster_Group_7; break;
                    case 7: wMazeID = pTB_DAY_EVENT->Maze_Select_8; wBoosterID = pTB_DAY_EVENT->Maze_Booster_Group_8; break;
                    case 8: wMazeID = pTB_DAY_EVENT->Maze_Select_9; wBoosterID = pTB_DAY_EVENT->Maze_Booster_Group_9; break;
                    case 9: wMazeID = pTB_DAY_EVENT->Maze_Select_10; wBoosterID = pTB_DAY_EVENT->Maze_Booster_Group_10; break;
                }
                if (wMazeID && wBoosterID) {
                    ST_DAY_EVENT_INFO stInfo{};
                    stInfo.wMazeID = wMazeID;
                    stInfo.wBoosterID = wBoosterID;
                    vecMazeEP.push_back(stInfo);
                }
            }

            if (vecMazeEP.empty()) {
                LogHelper::LogError("game.contents", "[DayEvent] SetDayEvent - Error Left Event Count [Maze :%u, Count :%d]", dwMazeID, nEventCount);
                continue;
            }

            // 随机打乱
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(vecMazeEP.begin(), vecMazeEP.end(), g);

            // 选择剩余数量
            for (size_t k = 0; k < vecMazeEP.size() && nEventCount > 0; ++k) {
                vecSelectMazeEP.push_back(vecMazeEP[k]);
                --nEventCount;
            }
        }

        // 检查事件数量
        if (nEventCount_Ori != static_cast<int>(vecSelectMazeEP.size())) {
            LogHelper::LogError("game.contents", "[DayEvent] SetDayEvent - Error Wrong Event Count [Count :%d, SelectCount :%zu]", nEventCount_Ori, vecSelectMazeEP.size());
            continue;
        }

        // 处理增益
        for (size_t m = 0; m < vecSelectMazeEP.size(); ++m) {
            ST_DAY_EVENT_INFO& stInfo = vecSelectMazeEP[m];

            // 获取 TB_DAY_EVENT_BOOSTER
            TB_DAY_EVENT_BOOSTER* pBooster = pServer->GetResourceMgr().GetTB_DAY_EVENT_BOOSTER(stInfo.wBoosterID);
            if (!pBooster) {
                LogHelper::LogError("game.contents", "[DayEvent] SetDayEvent - Error Booster NULL [Maze :%u, Booster :%u]", stInfo.wMazeID, stInfo.wBoosterID);
                continue;
            }

            // 随机增益
            int nBoosterRate = pServer->GetItemFactory().nRand(1, 10000);
            unsigned short wSelectedBoosterID = 0;

            // 遍历增益列表
            const unsigned short* pBoosterIDs[10] = {
                &pBooster->Booster_ID_01, &pBooster->Booster_ID_02, &pBooster->Booster_ID_03, &pBooster->Booster_ID_04, &pBooster->Booster_ID_05,
                &pBooster->Booster_ID_06, &pBooster->Booster_ID_07, &pBooster->Booster_ID_08, &pBooster->Booster_ID_09, &pBooster->Booster_ID_10
            };
            const unsigned short* pBoosterRates[10] = {
                &pBooster->Booster_Rate_01, &pBooster->Booster_Rate_02, &pBooster->Booster_Rate_03, &pBooster->Booster_Rate_04, &pBooster->Booster_Rate_05,
                &pBooster->Booster_Rate_06, &pBooster->Booster_Rate_07, &pBooster->Booster_Rate_08, &pBooster->Booster_Rate_09, &pBooster->Booster_Rate_10
            };

            for (int n = 0; n < 10; ++n) {
                if (*pBoosterIDs[n] && *pBoosterRates[n]) {
                    nBoosterRate -= *pBoosterRates[n];
                    if (nBoosterRate <= 0) {
                        wSelectedBoosterID = *pBoosterIDs[n];
                        break;
                    }
                }
            }

            if (!wSelectedBoosterID) {
                LogHelper::LogDebug("game.contents", "[DayEvent] SetDayEvent - No booster selected [Maze :%u]", stInfo.wMazeID);
                continue;
            }

            // 检查是否已存在
            auto itFind = m_mapDayEvent.find(stInfo.wMazeID);
            if (itFind != m_mapDayEvent.end()) {
                LogHelper::LogError("game.contents", "[DayEvent] SetDayEvent - Error Maze EP ID [Maze :%u]", stInfo.wMazeID);
                continue;
            }

            // 插入日常活动
            ST_DAY_EVENT_INFO stNewInfo{};
            stNewInfo.wMazeID = stInfo.wMazeID;
            stNewInfo.wBoosterID = wSelectedBoosterID;
            m_mapDayEvent[stNewInfo.wMazeID] = stNewInfo;

            // 发送日志
            ST_LOG_GAME stLog{};
            stLog._sMainType = 25;
            stLog._sSubType = 10;
            stLog.nParam0 = static_cast<int>(stNewInfo.wMazeID);
            stLog.nParam1 = static_cast<int>(stNewInfo.wBoosterID);
            pServer->SendDBLog(stLog);
        }
    }

    // 设置最后更新时间为当天 9:00
    ATL::CTime tSetTime(static_cast<int>(tSetDay.GetYear()),
                        static_cast<int>(tSetDay.GetMonth()),
                        static_cast<int>(tSetDay.GetDay()),
                        9, 0, 0, -1);
    m_tLastSetTime = tSetTime;

    LogHelper::LogInfo("game.system", "[DayEvent] SetDayEvent End");
}

// ============================================================================
// SendDayEventAllGameServer - 对齐 IDA 0x14002E4D0
// ============================================================================
void CDayEventMgr::SendDayEventAllGameServer()
{
    // 对齐 IDA: CDayEventMgr::SendDayEventAllGameServer
    XControlServer* pServer = XControlServer::Instance();
    if (!pServer) return;

    PS_DAY_EVENT_LIST psList;
    GetDayEvent(psList);

    XSendPacket xSendPacket(0xF2, 0x66);
    operator<<(xSendPacket, psList);
    pServer->SendPacketAll(xSendPacket, 0);
}

// ============================================================================
// GetDayEvent - 对齐 IDA 0x14002DA90
// ============================================================================
void CDayEventMgr::GetDayEvent(PS_DAY_EVENT_LIST& psList)
{
    // 对齐 IDA: CDayEventMgr::GetDayEvent
    CFAutoSlimReadLock lock(&m_rwEventLock);
    psList.mapList = m_mapDayEvent;
}
