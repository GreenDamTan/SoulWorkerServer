// WorldMode.cpp
// CWorldMode 世界模式类实现
// 对齐 IDA ControlServer.exe

#include "WorldMode.h"
#include "ControlServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_TimeCompat.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include <cstdio>
#include <cstring>

// 对齐 IDA 0x140046BD0: CWorldMode 构造函数
// IDA 版本: 设置 vtable, 初始化成员: m_stInfo, m_nTableID=0, m_nNextModeID=0, m_nLimitTime=0,
//          m_bReserveFinish=0, m_uxCompleteMapID=0, m_nClearWaitTime=0, m_bSuccess=0, m_strKiller[0]=0
CWorldMode::CWorldMode()
{
    // ST_WORLD_MODE_INFO 默认构造已调用
    m_nTableID = 0;
    m_nNextModeID = 0;
    m_nLimitTime = 0;
    m_bReserveFinish = false;
    m_uxCompleteMapID.nMapID = 0;
    m_nClearWaitTime = 0;
    m_bSuccess = false;
    m_strKiller[0] = L'\0';
}

// 对齐 IDA: CWorldMode 析构函数
CWorldMode::~CWorldMode()
{
    Clear();
}

// 对齐 IDA 0x140047920: Clear 清理
// IDA 版本: 只重置 nState, bSuccess, m_uxCompleteMapID, m_bSuccess, m_strKiller[0], m_bReserveFinish
void CWorldMode::Clear()
{
    m_stInfo.nState = 0;
    m_stInfo.bSuccess = 0;
    m_uxCompleteMapID.nMapID = 0;  // UXMapID::operator=(nullptr)
    m_bSuccess = false;
    m_strKiller[0] = L'\0';
    m_bReserveFinish = false;
}

// 对齐 IDA 0x140046CD0: Init 初始化
// IDA 版本: 复杂的时间计算函数,涉及 TB_MODE_DISTRICT6 和 TB_MODE_DISTRICT6_DATE 表
void CWorldMode::Init(ST_WORLD_MODE_INFO& stInfo)
{
    // 对齐 IDA: 复制 stInfo 到 m_stInfo
    std::memcpy(&m_stInfo, &stInfo, sizeof(m_stInfo));
    m_bSuccess = stInfo.bSuccess != 0;

    // 对齐 IDA: ATL::CTime::GetTickCount(&tCurTime)
    GreenDamTan::CTimeCompat tCurTime = GreenDamTan::GetCurrentTimeCompat();
    std::int64_t nCurTime = GreenDamTan::GetTimeAsInt64(tCurTime);

    // 对齐 IDA: 获取 XControlServer 实例
    auto pControlServer = XControlServer::Instance();
    if (!pControlServer) {
        return;
    }

    // 对齐 IDA: XResourceMgr::GetTB_MODE_DISTRICT6(&v2->m_xResourceMgr, m_stInfo.nModeID)
    auto pTBMode = pControlServer->GetResourceMgr().GetTB_MODE_DISTRICT6(static_cast<std::uint8_t>(m_stInfo.nModeID));
    if (pTBMode) {
        // 对齐 IDA: this->m_nStartType = pTBMode->Start_Type
        m_nStartType = pTBMode->Start_Type;
        // 对齐 IDA: this->m_nNextModeID = pTBMode->After_Mode_ID
        m_nNextModeID = pTBMode->After_Mode_ID;
        // 对齐 IDA: this->m_nLimitTime = pTBMode->Limit_Time
        m_nLimitTime = pTBMode->Limit_Time;
    }

    // 对齐 IDA: XResourceMgr::GetTB_MODE_DISTRICT6_DATE(&v3->m_xResourceMgr, m_stInfo.nModeDateID)
    auto pTBModeDate = pControlServer->GetResourceMgr().GetTB_MODE_DISTRICT6_DATE(static_cast<std::uint16_t>(m_stInfo.nModeDateID));
    if (!pTBModeDate) {
        // 对齐 IDA: 打印错误日志
        LogHelper::LogInfo("game.contents", "WORLD_MODE Init error - TB_MODE_DISTRICT6_DATE[%d]", m_stInfo.nModeDateID);
        return;
    }

    // 对齐 IDA: 检查当前星期是否匹配 Day_Type
    int nDayOfWeek = GreenDamTan::GetDayOfWeekCompat(tCurTime);
    if (nDayOfWeek == pTBModeDate->Day_Type) {
        // 对齐 IDA: 提取 Start_Time 的小时和分钟
        int nStartH = 22, nStartM = 0;
        GetEventTime(pTBModeDate->Start_Time, &nStartH, &nStartM);

        // 对齐 IDA: 计算模式持续时间
        m_nModeTime = 60 * (pTBModeDate->End_Time - pTBModeDate->Start_Time);

        // 对齐 IDA: 构建开始时间 CTime
        int nDay = GreenDamTan::GetDayCompat(tCurTime);
        int nMonth = GreenDamTan::GetMonthCompat(tCurTime);
        int nYear = GreenDamTan::GetYearCompat(tCurTime);

        GreenDamTan::CTimeCompat tTempTime = GreenDamTan::MakeTimeCompat(nYear, nMonth, nDay, nStartH, nStartM, 0);
        m_stInfo.biModeStartTime = GreenDamTan::GetTimeAsInt64(tTempTime);

        // 对齐 IDA: 提取 End_Time 的小时和分钟
        int nEndH = 23, nEndM = 0;
        GetEventTime(pTBModeDate->End_Time, &nEndH, &nEndM);

        // 对齐 IDA: 构建结束时间 CTime
        int nEndDay = GreenDamTan::GetDayCompat(tCurTime);
        int nEndMonth = GreenDamTan::GetMonthCompat(tCurTime);
        int nEndYear = GreenDamTan::GetYearCompat(tCurTime);
        tTempTime = GreenDamTan::MakeTimeCompat(nEndYear, nEndMonth, nEndDay, nEndH, nEndM, 0);
        m_stInfo.biModeEndTime = GreenDamTan::GetTimeAsInt64(tTempTime);
    }

    // 对齐 IDA: 根据状态处理
    if (m_stInfo.nState == 1) {
        // 对齐 IDA: 运行中状态,检查是否超时
        if (m_stInfo.nFinishTime <= nCurTime) {
            Clear();
        }
    } else if (m_stInfo.nState == 2) {
        // 对齐 IDA: 完成状态,检查是否需要清理
        std::int64_t nClearTime = m_stInfo.nFinishTime + 300;
        if (nClearTime <= nCurTime) {
            Clear();
        }
    }

    // 对齐 IDA: 如果状态为待机(0)
    if (m_stInfo.nState == 0) {
        if (m_nStartType == 1) {
            // 对齐 IDA: 时间触发型模式
            if (m_stInfo.nStartTime <= nCurTime) {
                // 计算下一次开始时间
                // 对齐 IDA: 复杂的 CTime 计算逻辑
                int nHour = 22, nMin = 0;
                GetEventTime(pTBModeDate->Start_Time, &nHour, &nMin);

                int nTodayDay = GreenDamTan::GetDayCompat(tCurTime);
                int nTodayMonth = GreenDamTan::GetMonthCompat(tCurTime);
                int nTodayYear = GreenDamTan::GetYearCompat(tCurTime);

                GreenDamTan::CTimeCompat tStartDate = GreenDamTan::MakeTimeCompat(nTodayYear, nTodayMonth, nTodayDay, nHour, nMin, 0);

                // 对齐 IDA: 计算下一次开始的偏移天数
                int nStartDay = 0;
                int nTargetDay = pTBModeDate->Day_Type;
                int nCurrentDay = nDayOfWeek;

                if (nTargetDay >= nCurrentDay) {
                    if (nTargetDay <= nCurrentDay) {
                        // 对齐 IDA: 检查是否已经过了今天的开始时间
                        std::int64_t nStartTime = GreenDamTan::GetTimeAsInt64(tStartDate);
                        if (nStartTime < nCurTime) {
                            nStartDay = 7;
                        }
                    } else {
                        nStartDay = nTargetDay - nCurrentDay;
                    }
                } else {
                    nStartDay = 7 - (nCurrentDay - nTargetDay);
                }

                // 对齐 IDA: 加上天数偏移
                GreenDamTan::CTimeSpanCompat span = GreenDamTan::MakeTimeSpanCompat(nStartDay, 0, 0, 0);
                GreenDamTan::CTimeCompat tNextStartTime = GreenDamTan::AddTimeSpanCompat(tStartDate, span);
                m_stInfo.nStartTime = GreenDamTan::GetTimeAsInt64(tNextStartTime);

                // 对齐 IDA: 发送日志
                SendLogNextModeTime(&tNextStartTime);

                LogHelper::LogInfo("game.contents", "[WORLD_MODE %d] Next Start Time calculated",
                    m_stInfo.nModeID);
            }
        } else {
            // 对齐 IDA: 非时间触发型,设置清除等待时间
            m_nClearWaitTime = pTBModeDate->Booster_Limit_Time;
        }
    }

    char buf[256];
    snprintf(buf, sizeof(buf), "ModeID=%d ModeDateID=%d State=%d StartType=%d",
        m_stInfo.nModeID, m_stInfo.nModeDateID, m_stInfo.nState, m_nStartType);
    GreenDamTan_log("WorldMode.cpp", __FUNCTION__, buf);
}

// 对齐 IDA 0x140047990: Update 更新
// IDA: 返回值为 bool (char), 检查状态并触发相应操作
bool CWorldMode::Update(bool bModeOn)
{
    // 对齐 IDA: ATL::CTime::GetTickCount(&tCurTime)
    // IDA 使用 GetTickCount 获取当前时间戳
    GreenDamTan::CTimeCompat tCurTime = GreenDamTan::GetCurrentTimeCompat();
    std::int64_t nCurTime = GreenDamTan::GetTimeAsInt64(tCurTime);

    // 对齐 IDA: switch (m_stInfo.nState)
    switch (m_stInfo.nState) {
    case 0:  // 对齐 IDA: 待机状态
        // 对齐 IDA: if (m_nStartType == 1 && bModeOn)
        // 对齐 IDA: && m_stInfo.nStartTime <= nCurTime
        // 对齐 IDA: && m_stInfo.nFinishTime <= m_stInfo.nStartTime
        if (m_nStartType == 1 && bModeOn) {
            if (m_stInfo.nStartTime <= nCurTime && m_stInfo.nFinishTime <= m_stInfo.nStartTime) {
                // 对齐 IDA: CWorldMode::StartMode(this)
                StartMode();
                return true;  // 对齐 IDA: return 1
            }
        }
        break;

    case 1:  // 对齐 IDA: 运行状态
        {
            // 对齐 IDA: nFinishTime = m_stInfo.nStartTime + m_nLimitTime
            std::int64_t nFinishTime = m_stInfo.nStartTime + m_nLimitTime;
            // 对齐 IDA: if (nFinishTime <= nCurTime || m_bReserveFinish)
            if (nFinishTime <= nCurTime || m_bReserveFinish) {
                // 对齐 IDA: CWorldMode::FinishMode(this)
                FinishMode();
                return true;  // 对齐 IDA: return 1
            }
        }
        break;

    case 2:  // 对齐 IDA: 完成状态
        {
            // 对齐 IDA: nClearTime = m_stInfo.nFinishTime + m_nClearWaitTime
            std::int64_t nClearTime = m_stInfo.nFinishTime + m_nClearWaitTime;
            // 对齐 IDA: if (nClearTime <= nCurTime)
            if (nClearTime <= nCurTime) {
                // 对齐 IDA: CWorldMode::ClearMode(this)
                ClearMode();
            }
        }
        break;
    }

    // 对齐 IDA: m_bReserveFinish = 0
    m_bReserveFinish = false;
    return false;  // 对齐 IDA: return 0
}

// 对齐 IDA 0x140047AC0: IsFinish 是否完成
bool CWorldMode::IsFinish() const
{
    return m_stInfo.nState == 2;  // 状态 2 表示完成
}

// 对齐 IDA 0x140047AF0: StartMode 启动模式
void CWorldMode::StartMode()
{
    Clear();  // 先清理之前的状态

    m_stInfo.nState = 1;  // 设置为运行状态

    // 获取当前时间
    std::int64_t nCurTime = GreenDamTan::GetCurrentTime();
    m_stInfo.nStartTime = nCurTime;
    m_stInfo.nFinishTime = nCurTime + m_nLimitTime;

    // 发送包到所有 GameServer (0xFB/0x01)
    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        XSendPacket xSendPacket(0xFB, 0x01);
        xSendPacket << m_stInfo;
        pControlServer->SendPacketAll(xSendPacket, false);

        // 发送到 DB Game (0x49/0x07)
        XSendDBPacket xSendDBPacket(0, 0x49, 0x07);
        xSendDBPacket.XParse << pControlServer->GetOption().GetGroupID();
        xSendDBPacket << m_stInfo;
        pControlServer->SendDBGame(xSendDBPacket);
    }

    GreenDamTan_log("WorldMode.cpp", __FUNCTION__, "Mode Started - State=1");
}

// 对齐 IDA 0x140047E10: FinishMode 完成模式
void CWorldMode::FinishMode()
{
    // 时间触发型标记为成功
    if (m_nStartType == 1) {
        m_bSuccess = true;
    }

    m_stInfo.nState = 2;  // 设置为完成状态
    m_stInfo.bSuccess = m_bSuccess;

    // 获取当前时间
    std::int64_t nCurTime = GreenDamTan::GetCurrentTime();
    m_stInfo.nFinishTime = nCurTime;

    // 构建完成包 (PS_WORLD_MODE_FINISH) - 使用公共头文件定义
    PS_WORLD_MODE_FINISH stFinish;
    stFinish.nModeID = m_stInfo.nModeID;
    stFinish.uxMapID = m_uxCompleteMapID;
    stFinish.nFinishTime = m_stInfo.nFinishTime;
    stFinish.nModeDateID = (m_nStartType == 1) ? 0 : m_stInfo.nModeDateID;
    stFinish.nMonsterClearCount = m_stInfo.nMonsterClearCount;
    wcscpy_s(stFinish.strKiller, m_strKiller);
    stFinish.bSuccess = m_bSuccess;

    // 发送包到所有 GameServer (0xFB/0x04)
    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        XSendPacket xSendPacket(0xFB, 0x04);
        xSendPacket << stFinish;
        pControlServer->SendPacketAll(xSendPacket, false);

        // 发送到 DB Game (0x49/0x07)
        XSendDBPacket xSendDBPacket(0, 0x49, 0x07);
        xSendDBPacket.XParse << pControlServer->GetOption().GetGroupID();
        xSendDBPacket << m_stInfo;
        pControlServer->SendDBGame(xSendDBPacket);
    }

    char buf[256];
    snprintf(buf, sizeof(buf), "ModeID=%d ModeDateID=%d State=2 Success=%d - Mode Finished",
        m_stInfo.nModeID, m_stInfo.nModeDateID, m_bSuccess ? 1 : 0);
    GreenDamTan_log("WorldMode.cpp", __FUNCTION__, buf);
}

// 对齐 IDA 0x140048350: ClearMode 清除模式
void CWorldMode::ClearMode()
{
    m_stInfo.nState = 0;  // 设置为待机状态

    // 获取当前时间
    std::int64_t nCurTime = GreenDamTan::GetCurrentTime();
    m_stInfo.nFinishTime = nCurTime;

    // 非时间触发型需要设置下一次事件时间
    if (m_nStartType == 0) {
        SetNextEventTime();
    }

    // 发送清除包到所有 GameServer (0xFB/0x03)
    ST_WORLD_MODE_INFO stInfo;
    stInfo.nModeID = m_stInfo.nModeID;

    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        XSendPacket xSendPacket(0xFB, 0x03);
        xSendPacket << stInfo;
        pControlServer->SendPacketAll(xSendPacket, false);

        // 发送到 DB Game (0x49/0x07)
        XSendDBPacket xSendDBPacket(0, 0x49, 0x07);
        xSendDBPacket.XParse << pControlServer->GetOption().GetGroupID();
        xSendDBPacket << m_stInfo;
        pControlServer->SendDBGame(xSendDBPacket);
    }

    Clear();  // 清理内部状态

    GreenDamTan_log("WorldMode.cpp", __FUNCTION__, "Mode Cleared");
}

// 对齐 IDA 0x140048590: GetNextModeID 获取下一个模式ID
int CWorldMode::GetNextModeID() const
{
    return m_nNextModeID;
}

// 对齐 IDA 0x1400485A0: GetModeInfo 获取模式信息
void CWorldMode::GetModeInfo(ST_WORLD_MODE_INFO& stInfo) const
{
    stInfo = m_stInfo;
}

// 对齐 IDA 0x1400485F0: ReqModeComplete 请求完成
bool CWorldMode::ReqModeComplete(PS_WORLD_MODE_COMPLETE& stComplete)
{
    // 对齐 IDA: if (m_stInfo.nState != 1) return 0;
    if (m_stInfo.nState != 1) {
        return false;
    }

    // 对齐 IDA: if (stComplete->bFinish) m_bReserveFinish = 1;
    if (stComplete.bFinish) {
        m_bReserveFinish = true;
    }

    // 对齐 IDA: m_bSuccess = 1;
    m_bSuccess = true;

    // 对齐 IDA: 复制 uxMapID 和 strKiller
    m_uxCompleteMapID = stComplete.uxMapID;

    // 复制击杀者名称
    for (int i = 0; i < 21 && stComplete.strKiller[i]; ++i) {
        m_strKiller[i] = stComplete.strKiller[i];
    }
    m_strKiller[20] = L'\0';

    GreenDamTan_log("WorldMode.cpp", __FUNCTION__, "Mode Complete");

    return true;
}

// 对齐 IDA 0x140048BD0: AddMonsterCount 增加怪物计数
// 只有非时间触发型 (m_nStartType == 0) 且运行中 (state == 1) 才有效
bool CWorldMode::AddMonsterCount()
{
    // 对齐 IDA: if (m_nStartType || m_stInfo.nState != 1) return 0;
    if (m_nStartType != 0 || m_stInfo.nState != 1) {
        return false;
    }

    ++m_stInfo.nMonsterClearCount;

    // 发送 DB 包更新计数
    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        // XSendDBPacket xSendDBPacket(0, 0x49, 0x07);
        // xSendDBPacket << pControlServer->GetOption()->GetGroupID();
        // xSendDBPacket << m_stInfo;
        // pControlServer->SendDBGame(&xSendDBPacket);
    }

    return true;
}

// 对齐 IDA 0x140048CE0: GetModeStartTime 获取模式开始时间
// 注意: 返回 biModeStartTime (模式时间), 不是 nStartTime
void CWorldMode::GetModeStartTime(std::int64_t& nTime) const
{
    nTime = m_stInfo.biModeStartTime;
}

// 对齐 IDA 0x140048D00: SetStartTime 设置开始时间
// 同时设置 biModeStartTime 为当前时间, biModeEndTime = biModeStartTime + m_nModeTime
void CWorldMode::SetStartTime(std::int64_t nTime)
{
    m_stInfo.nStartTime = nTime;

    // 对齐 IDA: 设置 biModeStartTime = 当前时间
    std::int64_t nCurTime = GreenDamTan::GetCurrentTime();
    m_stInfo.biModeStartTime = nCurTime;
    m_stInfo.biModeEndTime = m_stInfo.biModeStartTime + m_nModeTime;
}

// 对齐 IDA 0x14004D470: SetClearTime 设置清除时间
void CWorldMode::SetClearTime()
{
    // 对齐 IDA: m_nClearWaitTime = 0
    m_nClearWaitTime = 0;
}

// 对齐 IDA 0x14004D490: SetFinishTime 设置完成时间
void CWorldMode::SetFinishTime(std::int64_t nTime)
{
    m_stInfo.nFinishTime = nTime;
}

// 对齐 IDA 0x1400486E0: SetNextEventTime 设置下一次事件时间
// 计算非时间触发型模式的下一次开始时间
void CWorldMode::SetNextEventTime()
{
    // 对齐 IDA: 如果 nStartTime == 0, 打印日志并返回
    if (m_stInfo.nStartTime == 0) {
        LogHelper::LogInfo("game.contents", "[WORLD_MODE %d %d] NEXT 0",
            m_stInfo.nModeID, m_stInfo.nModeDateID);
        return;
    }

    // 对齐 IDA: 获取开始时间的 CTime
    GreenDamTan::CTimeCompat tStartTime(m_stInfo.nStartTime);
    std::int64_t nCurrentTime = GreenDamTan::GetCurrentTime();

    // 对齐 IDA: if (tStartTime <= CTime::GetTickCount())
    if (GreenDamTan::GetTimeAsInt64(tStartTime) <= nCurrentTime) {
        // 对齐 IDA: 从 XControlServer 获取 XResourceMgr
        auto pControlServer = XControlServer::Instance();
        if (!pControlServer) {
            return;
        }

        // 对齐 IDA: XResourceMgr::GetTB_MODE_DISTRICT6 检查是否存在
        auto pTBMode = pControlServer->GetResourceMgr().GetTB_MODE_DISTRICT6(static_cast<std::uint8_t>(m_stInfo.nModeID));
        if (!pTBMode) {
            return;
        }

        // 对齐 IDA: XResourceMgr::GetTB_MODE_DISTRICT6_DATE
        auto pTBModeDate = pControlServer->GetResourceMgr().GetTB_MODE_DISTRICT6_DATE(static_cast<std::uint16_t>(m_stInfo.nModeDateID));
        if (!pTBModeDate) {
            return;
        }

        // 对齐 IDA: 提取开始时间的小时和分钟
        int nStartH = 22, nStartM = 0;
        GetEventTime(pTBModeDate->Start_Time, &nStartH, &nStartM);

        // 对齐 IDA: 计算下一次开始天数
        int nStartDay = 7;  // 默认7天后

        // 对齐 IDA: 构建当天的开始时间
        int nDay = GreenDamTan::GetDayCompat(tStartTime);
        int nMonth = GreenDamTan::GetMonthCompat(tStartTime);
        int nYear = GreenDamTan::GetYearCompat(tStartTime);

        GreenDamTan::CTimeCompat tTempTime = GreenDamTan::MakeTimeCompat(nYear, nMonth, nDay, nStartH, nStartM, 0);

        // 对齐 IDA: if (tTempTime > this->m_stInfo.nStartTime) nStartDay = 0
        if (GreenDamTan::GetTimeAsInt64(tTempTime) > m_stInfo.nStartTime) {
            nStartDay = 0;
        }

        // 对齐 IDA: tStartTime = tStartTime + CTimeSpan(nStartDay, 0, 0, 0)
        GreenDamTan::CTimeSpanCompat span(nStartDay, 0, 0, 0);
        tTempTime = GreenDamTan::AddTimeSpanCompat(tTempTime, span);

        // 对齐 IDA: 更新 m_stInfo.nStartTime
        std::int64_t nNextStartTime = GreenDamTan::GetTimeAsInt64(tTempTime);
        m_stInfo.nStartTime = nNextStartTime;

        // 对齐 IDA: 发送日志
        SendLogNextModeTime(&nNextStartTime);

        // 对齐 IDA: 打印下一次开始时间
        int nSecond = GreenDamTan::CTimeCompat(nNextStartTime).GetSecond();
        int nMinute = GreenDamTan::CTimeCompat(nNextStartTime).GetMinute();
        int nHour = GreenDamTan::CTimeCompat(nNextStartTime).GetHour();
        int nNextDay = GreenDamTan::GetDayCompat(GreenDamTan::CTimeCompat(nNextStartTime));
        LogHelper::LogInfo("game.contents",
            "[WORLD_MODE %d %d] Next Start Time %d D %d H %d M %d S",
            m_stInfo.nModeID, m_stInfo.nModeDateID,
            nNextDay, nHour, nMinute, nSecond);
    }
}

// 对齐 IDA 0x140048B50: GetEventTime 获取事件时间
// 从完整时间值 (如 2200 = 22:00) 提取小时和分钟
void CWorldMode::GetEventTime(int nFullTime, int* pHour, int* pMin)
{
    // 对齐 IDA: if (nFullTime < 2400)
    if (nFullTime < 2400) {
        // 对齐 IDA: if (nFullTime > 0)
        if (nFullTime > 0) {
            *pHour = nFullTime / 100;
            *pMin = nFullTime % 100;
        } else {
            // 对齐 IDA: 默认值为 1:00
            *pHour = 1;
            *pMin = 0;
        }
    } else {
        // 对齐 IDA: 超过 2400 时使用 23:59
        *pHour = 23;
        *pMin = 59;
    }
}

// 对齐 IDA 0x1400489D0: SendLogNextModeTime 发送下一次模式时间日志
void CWorldMode::SendLogNextModeTime(void* pTime)
{
    // 对齐 IDA: 构建 ST_LOG_GAME 结构
    ST_LOG_GAME stLog{};
    stLog._sMainType = 25;
    stLog._sSubType = 6;
    stLog.nParam0 = m_stInfo.nModeID;
    // 对齐 IDA: 从 ATL::CTime 提取时间参数
    // stLog.nParam1 = ATL::CTime::GetMonth(tTime);
    // stLog.nParam2 = ATL::CTime::GetDay(tTime);
    // stLog.nParam3 = ATL::CTime::GetHour(tTime);
    // stLog.nParam4 = ATL::CTime::GetMinute(tTime);
    // stLog.nParam5 = ATL::CTime::GetSecond(tTime);
    // stLog.nParam6 = (__int64)tTime;
    stLog.nParam7 = m_stInfo.nModeDateID;

    // 对齐 IDA: wcscpy_s(stLog.szComment, L"NEXT_D6_MODE")
    wcscpy_s(stLog.szComment, L"NEXT_D6_MODE");

    // 对齐 IDA: 发送 DB 日志
    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        // pControlServer->SendDBLog(&stLog);
    }
}

// 对齐 IDA 0x140048D60: InitMode 初始化模式
// IDA 显示这个函数:
// 1. 调用 Clear()
// 2. 设置 nState=2, nFinishTime=nInitTime, bSuccess=1, nMonsterClearCount=0
// 3. 发送 0xFB/0x04 包 (PS_WORLD_MODE_FINISH)
// 4. 设置 nState=0, nFinishTime=nInitTime
// 5. 发送 0xFB/0x03 包 (ST_WORLD_MODE_INFO)
// 6. 发送 DB 包 0x49/0x07
// 7. 发送 ST_LOG_GAME (MainType=25, SubType=8, szComment=L"INIT_D6_MODE")
// 8. 调用 Init(m_stInfo)
void CWorldMode::InitMode(std::int64_t nInitTime)
{
    // 对齐 IDA: CWorldMode::Clear(this)
    Clear();

    // 对齐 IDA: 设置状态为完成
    m_stInfo.nState = 2;
    m_stInfo.nFinishTime = nInitTime;
    m_stInfo.bSuccess = 1;
    m_stInfo.nMonsterClearCount = 0;

    // 对齐 IDA: 构建 PS_WORLD_MODE_FINISH 包 - 使用公共头文件定义
    PS_WORLD_MODE_FINISH stFinish;
    stFinish.nModeID = m_stInfo.nModeID;
    stFinish.uxMapID = m_uxCompleteMapID;
    stFinish.nFinishTime = m_stInfo.nFinishTime;
    stFinish.nModeDateID = m_stInfo.nModeDateID;
    stFinish.nMonsterClearCount = m_stInfo.nMonsterClearCount;
    wcscpy_s(stFinish.strKiller, m_strKiller);
    stFinish.bSuccess = true;

    // 对齐 IDA: 发送 0xFB/0x04 包到所有服务器
    auto pControlServer = XControlServer::Instance();
    if (pControlServer) {
        XSendPacket xSendPacket(0xFB, 0x04);
        xSendPacket << stFinish;
        pControlServer->SendPacketAll(xSendPacket, false);
    }

    // 对齐 IDA: 设置状态为待机
    m_stInfo.nState = 0;
    m_stInfo.nFinishTime = nInitTime;

    // 对齐 IDA: 构建 ST_WORLD_MODE_INFO 包
    ST_WORLD_MODE_INFO stInfo;
    stInfo.nModeID = m_stInfo.nModeID;

    // 对齐 IDA: 发送 0xFB/0x03 包到所有服务器
    if (pControlServer) {
        XSendPacket xSendPacket2(0xFB, 0x03);
        xSendPacket2 << stInfo;
        pControlServer->SendPacketAll(xSendPacket2, false);

        // 对齐 IDA: 发送 DB 包 0x49/0x07
        XSendDBPacket xSendDBPacket(0, 0x49, 0x07);
        xSendDBPacket.XParse << pControlServer->GetOption().GetGroupID();
        xSendDBPacket << m_stInfo;
        pControlServer->SendDBGame(xSendDBPacket);
    }

    // 对齐 IDA: 发送 ST_LOG_GAME 日志
    ST_LOG_GAME stLog{};
    stLog._sMainType = 25;
    stLog._sSubType = 8;
    // 对齐 IDA: 从 CTime 提取时间参数
    GreenDamTan::CTimeCompat tInitTime(nInitTime);
    stLog.nParam0 = GreenDamTan::GetDayOfWeekCompat(tInitTime);
    stLog.nParam1 = GreenDamTan::GetMonthCompat(tInitTime);
    stLog.nParam2 = GreenDamTan::GetDayCompat(tInitTime);
    stLog.nParam3 = tInitTime.GetHour();
    stLog.nParam4 = tInitTime.GetMinute();
    stLog.nParam5 = tInitTime.GetSecond();
    stLog.nParam6 = nInitTime;
    stLog.nParam7 = m_stInfo.nModeDateID;
    stLog.nParam8 = m_stInfo.nMonsterClearCount;
    wcscpy_s(stLog.szComment, L"INIT_D6_MODE");

    if (pControlServer) {
        pControlServer->SendDBLog(stLog);
    }

    // 对齐 IDA: 调用 Init(m_stInfo)
    Init(m_stInfo);

    GreenDamTan_log("WorldMode.cpp", __FUNCTION__, "Mode Initialized");
}
