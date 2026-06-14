#include "Timer.h"
#include "Maze.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// ============================================================================
// LogicTimer Implementation
// ============================================================================

LogicTimer::LogicTimer()
    : id_(0)
    , type_(0)
    , enable_(true)
    , timer_(0.0f)
    , elapsedTimes_(0.0f)
    , originTime_(0.0f)
    , customData_(nullptr)
    , userString_(nullptr)
    , eventString_(nullptr)
    , readyString_(nullptr)
    , m_nParam1(0)
    , m_nParam2(0)
    , m_nParam3(0)
    , m_nEventStep(0)
    , m_bFinishStep(false)
    , m_bReady(false)
    , m_bPause(false)
    , m_bCallReadyScript(false)
    , m_nNextLeftTime(0)
    , m_nNextTotalTime(0)
{
}

LogicTimer::LogicTimer(const LogicTimer& other)
    : id_(other.id_)
    , type_(other.type_)
    , enable_(other.enable_)
    , timer_(other.timer_)
    , elapsedTimes_(other.elapsedTimes_)
    , originTime_(other.originTime_)
    , customData_(other.customData_)
    , userString_(other.userString_)
    , eventString_(other.eventString_)
    , readyString_(other.readyString_)
    , m_nParam1(other.m_nParam1)
    , m_nParam2(other.m_nParam2)
    , m_nParam3(other.m_nParam3)
    , m_nEventStep(other.m_nEventStep)
    , m_bFinishStep(other.m_bFinishStep)
    , m_bReady(other.m_bReady)
    , m_bPause(other.m_bPause)
    , m_bCallReadyScript(other.m_bCallReadyScript)
    , m_nNextLeftTime(other.m_nNextLeftTime)
    , m_nNextTotalTime(other.m_nNextTotalTime)
{
}

LogicTimer::~LogicTimer()
{
}

void LogicTimer::SetTimer(float fTarget, bool bReset)
{
    // IDA: LogicTimer::SetTimer (0x140353C30)
    timer_ = fTarget;
    if (bReset)
    {
        elapsedTimes_ = 0.0f;
    }
    enable_ = true;
}

void LogicTimer::SetPause(bool bPause)
{
    // IDA: LogicTimer::SetPause (0x140353580)
    m_bPause = bPause;
}

void LogicTimer::SetReady(bool bReady)
{
    // IDA: LogicTimer::SetReady (0x1403535B0)
    m_bReady = bReady;
}

void LogicTimer::SetCallReadyScript(bool bCall)
{
    // IDA: LogicTimer::SetCallReadyScript (0x1403535E0)
    m_bCallReadyScript = bCall;
}

void LogicTimer::SetFinishStep(bool bFinish)
{
    // IDA: LogicTimer::SetFinishStep (0x1403537D0)
    m_bFinishStep = bFinish;
}

void LogicTimer::Disable(bool bDisable)
{
    // IDA: LogicTimer::Disable (0x140353680)
    enable_ = !bDisable;
}

TimerResult_e LogicTimer::Update(float fElapsed)
{
    // IDA: LogicTimer::Update (0x1403536C0)
    if (!enable_)
        return TIMER_ERROR;
    
    elapsedTimes_ += fElapsed;
    
    if (elapsedTimes_ < timer_)
        return TIMER_CONTINUE;
    
    Disable(true);
    return COMPLETE;
}

float LogicTimer::GetTimer() const
{
    // IDA: LogicTimer::GetTimer (0x140353AF0)
    return timer_;
}

float LogicTimer::GetMaxTimer() const
{
    // IDA: LogicTimer::GetMaxTimer (0x140353B00)
    return timer_;
}

float LogicTimer::GetLeftTime() const
{
    // IDA: LogicTimer::GetLeftTime (0x140353740)
    float leftTime = timer_ - elapsedTimes_;
    return leftTime >= 0.0f ? leftTime : 0.0f;
}

float LogicTimer::GetOriginTime() const
{
    // IDA: LogicTimer::GetOriginTime (0x140353510)
    return originTime_;
}

int LogicTimer::GetNextLeftTime() const
{
    // IDA: LogicTimer::GetNextLeftTime (0x1403535A0)
    return static_cast<int>(GetLeftTime());
}

bool LogicTimer::IsPause() const
{
    // IDA: LogicTimer::IsPause (0x140353730)
    return m_bPause;
}

bool LogicTimer::IsReady() const
{
    // IDA: LogicTimer::IsReady (0x140353630)
    return m_bReady;
}

bool LogicTimer::IsCallReadyScript() const
{
    // IDA: LogicTimer::IsCallReadyScript (0x140353620)
    return m_bCallReadyScript;
}

bool LogicTimer::IsFinishStep() const
{
    // IDA: LogicTimer::IsFinishStep (0x140353500)
    return m_bFinishStep;
}

bool LogicTimer::IsLastStep() const
{
    // IDA: LogicTimer::IsLastStep (0x140353550)
    return m_nEventStep == m_nParam1;
}

void LogicTimer::SetTimerEx(float fTime)
{
    // IDA: LogicTimer::SetTimerEx
    elapsedTimes_ = fTime;
}

void LogicTimer::AddMaxTime(float fTime)
{
    // IDA: LogicTimer::AddMaxTime (0x1403537A0)
    timer_ += fTime;
}

void LogicTimer::SetNextTime(int nLeftTime, int nTotalTime)
{
    // IDA: LogicTimer::SetNextTime (0x140353B10)
    m_nNextLeftTime = nLeftTime;
    m_nNextTotalTime = nTotalTime;
}

void LogicTimer::SetOriginTime(float fTime)
{
    // IDA: LogicTimer::SetOriginTime (0x140353BA0)
    originTime_ = fTime;
}

int LogicTimer::GetStep() const
{
    // IDA: LogicTimer::GetStep (0x1403537F0)
    return m_nEventStep;
}

int LogicTimer::MoveNextStep()
{
    // IDA: LogicTimer::MoveNextStep (0x140353520)
    m_nEventStep++;
    return m_nEventStep;
}

void LogicTimer::SetType(int nType)
{
    // IDA: LogicTimer::SetType (0x140276920)
    type_ = nType;
}

void LogicTimer::SetParam(int nParam1, int nParam2, int nParam3)
{
    // IDA: LogicTimer::SetParam (0x140353BC0)
    m_nParam1 = nParam1;
    m_nParam2 = nParam2;
    m_nParam3 = nParam3;
}

int LogicTimer::GetParam2() const
{
    // IDA: LogicTimer::GetParam2 (0x140353640)
    return m_nParam2;
}

int LogicTimer::GetParam3() const
{
    // IDA: LogicTimer::GetParam3 (0x140353650)
    return m_nParam3;
}

void LogicTimer::SetUserString(const char* szString)
{
    // IDA: LogicTimer::SetUserString (0x140353C00)
    userString_ = szString;
}

const char* LogicTimer::GetUserString() const
{
    // IDA: LogicTimer::GetUserString (0x140353660)
    return userString_;
}

void LogicTimer::SetEventString(const char* szString)
{
    // IDA: LogicTimer::SetEventString (0x140353B70)
    eventString_ = szString;
}

const char* LogicTimer::GetEventString() const
{
    // IDA: LogicTimer::GetEventString (0x1403534E0)
    return eventString_;
}

void LogicTimer::SetReadyString(const char* szString)
{
    // IDA: LogicTimer::SetReadyString (0x140353B40)
    readyString_ = szString;
}

const char* LogicTimer::GetReadyString() const
{
    // IDA: LogicTimer::GetReadyString (0x140353600)
    return readyString_;
}

// ============================================================================
// ST_TIME_STEP_TIMER Implementation
// ============================================================================

ST_TIME_STEP_TIMER::ST_TIME_STEP_TIMER()
    : fTime(0.0f)
    , nSpawnBoxID(0)
    , nDeathMotion(0)
{
}

ST_TIME_STEP_TIMER::ST_TIME_STEP_TIMER(const ST_TIME_STEP_TIMER& other)
    : fTime(other.fTime)
    , nSpawnBoxID(other.nSpawnBoxID)
    , nDeathMotion(other.nDeathMotion)
    , strTimeout(other.strTimeout)
{
}

ST_TIME_STEP_TIMER::~ST_TIME_STEP_TIMER()
{
}

// Note: CFsmTransition, CFsmCondition, IVTimer, VDefaultTimer, ThreadLocalData
// are implemented in FsmClass.h and VisionEngineTypes.h

// ============================================================================
// TimerManager Implementation
// ============================================================================

TimerManager::TimerManager()
    : m_pMaze(nullptr)
    , m_pScriptInstance(nullptr)
{
}

TimerManager::~TimerManager()
{
    ClearAllTimers();
}

int TimerManager::AddTimer(int nID, float fTarget)
{
    // 从 IDA: XMaze::AddTimer (0x1403292E0)
    // CreateTimer 实现 - 创建简单的定时器

    LogicTimer newTimer;
    newTimer.SetID(nID);
    newTimer.SetTimer(fTarget, true);
    newTimer.SetType(TIMER_TYPE_NONE);

    m_arWaitLogicTimers.push_back(newTimer);

    return nID;
}

int TimerManager::AddTimerEx(const char* szLuaFunction, float fTarget, int nParam1, int nParam2, int nParam3)
{
    // 从 IDA: XMaze::AddTimerEx (0x140329370)
    // CreateTimer 实现 - 创建带Lua回调的定时器

    if (!szLuaFunction)
        return -1;

    LogicTimer newTimer;

    // 使用哈希函数生成ID
    int nID = VHashString::GetHash(szLuaFunction);
    newTimer.SetID(nID);
    newTimer.SetTimer(fTarget, true);
    newTimer.SetUserString(szLuaFunction);
    newTimer.SetParam(nParam1, nParam2, nParam3);
    newTimer.SetType(TIMER_TYPE_SCRIPT);

    m_arWaitLogicTimers.push_back(newTimer);

    return nID;
}

int TimerManager::AddEventTimer(const char* szReady, const char* szEvent, const char* szUser,
                                  float fTarget, int nType, int nParam1, int nParam2, int nParam3)
{
    // 从 IDA: XMaze::AddEventTimer (0x140329440)
    // CreateTimer 实现 - 创建事件定时器

    LogicTimer newTimer;

    if (szUser)
    {
        int nID = VHashString::GetHash(szUser);
        newTimer.SetID(nID);
    }

    newTimer.SetTimer(fTarget, true);
    newTimer.SetType(nType);
    newTimer.SetParam(nParam1, nParam2, nParam3);

    if (szReady)
        newTimer.SetReadyString(szReady);
    if (szEvent)
        newTimer.SetEventString(szEvent);
    if (szUser)
        newTimer.SetUserString(szUser);

    m_arWaitLogicTimers.push_back(newTimer);

    return newTimer.GetGroupID();
}

int TimerManager::AddTimeStepTimer(int nID, float fTime, int nSpawnBoxID, const char* szTimeout)
{
    // 从 IDA: XMaze::AddTimeStepTimer (0x140340360)
    // CreateTimer 实现 - 创建时间步进定时器

    ST_TIME_STEP_TIMER stInfo;
    stInfo.fTime = fTime;
    stInfo.nSpawnBoxID = nSpawnBoxID;
    if (szTimeout)
        stInfo.strTimeout = szTimeout;

    m_mapTimeStepTimer[nID] = stInfo;

    return nID;
}

void TimerManager::RemoveTimer(int nID)
{
    // 从 IDA: XMaze::RemoveTimer (0x140329760)
    // DestroyTimer 实现 - 移除定时器

    // 查找并移除等待列表中的定时器
    auto itWait = std::find_if(m_arWaitLogicTimers.begin(), m_arWaitLogicTimers.end(),
        [nID](const LogicTimer& timer) {
            return timer.GetGroupID() == nID;
        });

    if (itWait != m_arWaitLogicTimers.end())
    {
        itWait->SetTimer(0.0f, false);  // 设置为0触发完成
    }

    // 查找并移除活动列表中的定时器
    auto itActive = std::find_if(m_arLogicTimers.begin(), m_arLogicTimers.end(),
        [nID](const LogicTimer& timer) {
            return timer.GetGroupID() == nID;
        });
    
    if (itActive != m_arLogicTimers.end())
    {
        itActive->SetTimer(0.0f, false);
    }
}

float TimerManager::RemoveTimerEx(const char* szLuaFunction)
{
    // 从 IDA: XMaze::RemoveTimerEx (0x1403295A0)
    // DestroyTimer 实现 - 移除定时器并返回剩余时间
    
    if (!szLuaFunction)
        return 0.0f;
    
    float fLeftTime = 0.0f;
    int nID = VHashString::GetHash(szLuaFunction);
    
    // 查找定时器
    auto it = std::find_if(m_arLogicTimers.begin(), m_arLogicTimers.end(),
        [nID](const LogicTimer& timer) {
            return timer.GetGroupID() == nID;
        });
    
    if (it != m_arLogicTimers.end())
    {
        // 计算剩余时间
        float maxTime = it->GetMaxTimer();
        float currentTime = it->GetTimer();
        fLeftTime = maxTime - currentTime;
        
        if (fLeftTime < 0.0f)
            fLeftTime = 0.0f;
        
        // 设置为0触发完成
        it->SetTimer(0.0f, false);
    }
    
    return fLeftTime;
}

void TimerManager::ClearAllTimers()
{
    // 从 IDA: XMaze::ClearAllTimers (0x140329720)
    // DestroyTimer 实现 - 清除所有定时器
    
    m_arLogicTimers.clear();
    m_arWaitLogicTimers.clear();
    m_mapTimeStepTimer.clear();
}

void TimerManager::StartEventTimer(const char* szLuaFunction)
{
    // 从 IDA: XMaze::StartEventTimer (0x140336490)
    // SetTimer 实现 - 启动事件定时器
    
    if (!szLuaFunction)
        return;
    
    int nID = VHashString::GetHash(szLuaFunction);
    
    // 查找定时器并设置为活动状态
    auto it = std::find_if(m_arWaitLogicTimers.begin(), m_arWaitLogicTimers.end(),
        [nID](const LogicTimer& timer) {
            return timer.GetGroupID() == nID;
        });
    
    if (it != m_arWaitLogicTimers.end())
    {
        it->SetPause(false);
    }
}

void TimerManager::StartTimeStepTimer()
{
    // 从 IDA: XMaze::StartTimeStepTimer (0x1403404B0)
    // SetTimer 实现 - 启动时间步进定时器
    
    if (m_mapTimeStepTimer.empty())
        return;
    
    // 获取第一个步进定时器
    auto itFirst = m_mapTimeStepTimer.begin();
    if (itFirst != m_mapTimeStepTimer.end())
    {
        LogicTimer newTimer;
        newTimer.SetID(itFirst->first);
        newTimer.SetTimer(itFirst->second.fTime, true);
        newTimer.SetType(TIMER_TYPE_TIME_STEP);
        newTimer.SetParam(static_cast<int>(m_mapTimeStepTimer.size()), 
                          itFirst->second.nSpawnBoxID, 
                          itFirst->second.nDeathMotion);
        newTimer.SetEventString(itFirst->second.strTimeout.c_str());
        newTimer.SetReady(true);
        
        m_arWaitLogicTimers.push_back(newTimer);
    }
}

float TimerManager::GetTimerLeftTime(int nID) const
{
    // GetRemainingTime 实现 - 获取剩余时间
    
    auto it = std::find_if(m_arLogicTimers.begin(), m_arLogicTimers.end(),
        [nID](const LogicTimer& timer) {
            return timer.GetGroupID() == nID;
        });
    
    if (it != m_arLogicTimers.end())
    {
        return it->GetLeftTime();
    }
    
    return 0.0f;
}

void TimerManager::PauseAllTimers(bool bPause)
{
    // 从 IDA: XMaze::PauseAlltimer (0x14033AE10)
    // PauseTimer 实现 - 暂停/恢复所有定时器
    
    for (auto& timer : m_arLogicTimers)
    {
        timer.SetPause(bPause);
    }
    
    for (auto& timer : m_arWaitLogicTimers)
    {
        timer.SetPause(bPause);
    }
}

void TimerManager::UpdateTimers(float fElapsed)
{
    // 从 IDA: XMaze::UpdateTimer (0x140335170)
    // 定时器更新主循环
    
    // 将等待列表中的定时器移到活动列表
    auto itWait = m_arWaitLogicTimers.begin();
    while (itWait != m_arWaitLogicTimers.end())
    {
        m_arLogicTimers.push_back(*itWait);
        itWait = m_arWaitLogicTimers.erase(itWait);
    }
    
    // 更新活动定时器
    auto it = m_arLogicTimers.begin();
    while (it != m_arLogicTimers.end())
    {
        LogicTimer& timer = *it;
        
        // 跳过暂停的定时器
        if (timer.IsPause())
        {
            ++it;
            continue;
        }
        
        // 跳过禁用的定时器
        if (timer.IsReady() && !timer.IsCallReadyScript())
        {
            // 调用准备脚本
            if (m_pScriptInstance)
            {
                // TODO: 调用脚本
                // m_pScriptInstance->ExecuteFunctionArg(timer.GetReadyString(), ...);
            }
            timer.SetCallReadyScript(true);
        }
        
        // 更新定时器
        TimerResult_e result = timer.Update(fElapsed);
        
        if (result == COMPLETE)
        {
            // 定时器完成
            if (timer.GetMaxTimer() > 0.0f && m_pScriptInstance)
            {
                // 调用完成回调
                const char* szUserString = timer.GetUserString();
                if (szUserString && strlen(szUserString) > 0)
                {
                    // TODO: 调用Lua函数
                    // m_pScriptInstance->ExecuteFunctionArg(szUserString, ...);
                }
                else
                {
                    // 调用默认回调
                    // m_pScriptInstance->ExecuteFunctionArg("OnTimerComplete", ...);
                }
            }
            
            // 移除完成的定时器
            it = m_arLogicTimers.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void TimerManager::ShowCasualRaidTimer(int nType, float fMaxTime, float fLeftTime)
{
    // IDA: XMaze::ShowCasualRaidTimer (0x14032B390)
    // Sends packet (main=0x11, sub=0x38) to broadcast casual raid timer
    // Packet format: iType(0) + nValue + fTime + fTime2
    
    // TODO: Requires XSendPacket and XMaze::SendBroadCast implementation
    // XSendPacket xPacket(0x11, 0x38);
    // xPacket << 0;           // iType
    // xPacket << nType;       // nValue
    // xPacket << fMaxTime;    // fTime
    // xPacket << fLeftTime;   // fTime2
    // m_pMaze->SendBroadCast(&xPacket, nullptr, eAll);
    
    // Store timer values
    // m_stCasualRaidTime.nIntValue = nType;
    // m_stCasualRaidTime.fFloatValue = fMaxTime;
}

void TimerManager::SetupCasualRaidTimer()
{
    // IDA: XMaze::SetupCasualRaidTimer (0x14032B4B0)
    // Sets up wait time before sending timer to clients
    if (m_stCasualRaidTime.fFloatValue > 0.0f)
    {
        m_stCasualRaidTime.fWaitSendTime = 5.0f;
    }
}

void TimerManager::SendCasualRaidTimer()
{
    // IDA: XMaze::SendCasualRaidTimer (0x14032B4E0)
    // Sends the casual raid timer if active
    if (m_stCasualRaidTime.fFloatValue > 0.0f)
    {
        ShowCasualRaidTimer(m_stCasualRaidTime.nIntValue, m_stCasualRaidTime.fFloatValue, 0.0f);
    }
}

void TimerManager::UpdateCasualRaidTimer(float fElapsed)
{
    // IDA: XMaze::UpdateCasualRaidTimer (0x14031D850)
    // Updates casual raid timer countdown
    
    // Update main timer
    if (m_stCasualRaidTime.fFloatValue > 0.0f)
    {
        m_stCasualRaidTime.fFloatValue -= fElapsed;
        if (m_stCasualRaidTime.fFloatValue <= 0.0f)
        {
            m_stCasualRaidTime.reset();
        }
    }
    
    // Update wait send time
    if (m_stCasualRaidTime.fWaitSendTime > 0.0f)
    {
        m_stCasualRaidTime.fWaitSendTime -= fElapsed;
        if (m_stCasualRaidTime.fWaitSendTime <= 0.0f)
        {
            SendCasualRaidTimer();
            m_stCasualRaidTime.fWaitSendTime = 0.0f;
        }
    }
}

std::list<LogicTimer>& TimerManager::GetLogicTimers()
{
    return m_arLogicTimers;
}

std::list<LogicTimer>& TimerManager::GetWaitLogicTimers()
{
    return m_arWaitLogicTimers;
}

std::map<int, ST_TIME_STEP_TIMER>& TimerManager::GetTimeStepTimers()
{
    return m_mapTimeStepTimer;
}

// Note: ThreadLocalData::GetTimer is implemented in VisionEngineTypes.cpp
