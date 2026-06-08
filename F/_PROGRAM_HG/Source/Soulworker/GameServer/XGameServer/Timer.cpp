#include "Timer.h"
#include "Maze.h"
#include "Script.h"
#include <algorithm>
#include <cmath>

// ============================================================================
// LogicTimer Implementation
// ============================================================================

LogicTimer::LogicTimer()
    : timer_(0.0f)
    , maxTimer_(0.0f)
    , elapsedTimes_(0.0f)
    , originTime_(0.0f)
    , type_(0)
    , step_(0)
    , bPause_(false)
    , bReady_(false)
    , bCallReadyScript_(false)
    , bFinishStep_(false)
    , bDisable_(false)
    , param1_(0)
    , param2_(0)
    , param3_(0)
{
}

LogicTimer::LogicTimer(const LogicTimer& other)
    : timer_(other.timer_)
    , maxTimer_(other.maxTimer_)
    , elapsedTimes_(other.elapsedTimes_)
    , originTime_(other.originTime_)
    , type_(other.type_)
    , step_(other.step_)
    , bPause_(other.bPause_)
    , bReady_(other.bReady_)
    , bCallReadyScript_(other.bCallReadyScript_)
    , bFinishStep_(other.bFinishStep_)
    , bDisable_(other.bDisable_)
    , param1_(other.param1_)
    , param2_(other.param2_)
    , param3_(other.param3_)
    , userString_(other.userString_)
    , eventString_(other.eventString_)
    , readyString_(other.readyString_)
{
}

LogicTimer::~LogicTimer()
{
}

void LogicTimer::SetTimer(float fTarget, bool bReset)
{
    // 从 IDA: SetTimer 设置定时器目标时间
    maxTimer_ = fTarget;
    if (bReset)
    {
        timer_ = 0.0f;
        elapsedTimes_ = 0.0f;
    }
    bPause_ = false;
}

void LogicTimer::SetPause(bool bPause)
{
    // 从 IDA: SetPause 设置暂停状态
    bPause_ = bPause;
}

void LogicTimer::SetReady(bool bReady)
{
    // 从 IDA: SetReady 设置准备状态
    bReady_ = bReady;
}

void LogicTimer::SetCallReadyScript(bool bCall)
{
    // 从 IDA: SetCallReadyScript 设置是否调用准备脚本
    bCallReadyScript_ = bCall;
}

void LogicTimer::SetFinishStep(bool bFinish)
{
    // 从 IDA: SetFinishStep 设置是否完成步骤
    bFinishStep_ = bFinish;
}

void LogicTimer::Disable(bool bDisable)
{
    // 从 IDA: Disable 设置禁用状态
    bDisable_ = bDisable;
}

TimerResult_e LogicTimer::Update(float fElapsed)
{
    // 从 IDA: Update 更新定时器
    if (bPause_ || bDisable_)
        return TIMER_CONTINUE;
    
    timer_ += fElapsed;
    elapsedTimes_ += fElapsed;
    
    // 检查是否完成
    if (timer_ >= maxTimer_)
    {
        return COMPLETE;
    }
    
    return TIMER_CONTINUE;
}

float LogicTimer::GetTimer() const
{
    // 从 IDA: GetTimer 获取当前计时器时间
    return timer_;
}

float LogicTimer::GetMaxTimer() const
{
    // 从 IDA: GetMaxTimer 获取最大计时器时间
    return maxTimer_;
}

float LogicTimer::GetLeftTime() const
{
    // 从 IDA: GetLeftTime 获取剩余时间 (0x140353740)
    float leftTime = maxTimer_ - timer_;
    return leftTime >= 0.0f ? leftTime : 0.0f;
}

float LogicTimer::GetOriginTime() const
{
    // 从 IDA: GetOriginTime 获取原始时间
    return originTime_;
}

int LogicTimer::GetNextLeftTime() const
{
    // 从 IDA: GetNextLeftTime 获取下一个剩余时间(整数)
    return static_cast<int>(GetLeftTime());
}

bool LogicTimer::IsPause() const
{
    // 从 IDA: IsPause 检查是否暂停
    return bPause_;
}

bool LogicTimer::IsReady() const
{
    // 从 IDA: IsReady 检查是否准备好
    return bReady_;
}

bool LogicTimer::IsCallReadyScript() const
{
    // 从 IDA: IsCallReadyScript 检查是否调用准备脚本
    return bCallReadyScript_;
}

bool LogicTimer::IsFinishStep() const
{
    // 从 IDA: IsFinishStep 检查是否完成步骤
    return bFinishStep_;
}

bool LogicTimer::IsLastStep() const
{
    // 从 IDA: IsLastStep 检查是否最后一步
    // TODO: 需要从 step_ 和其他信息判断
    return false;
}

void LogicTimer::SetTimerEx(float fTime)
{
    // 从 IDA: SetTimerEx 设置计时器时间
    timer_ = fTime;
}

void LogicTimer::AddMaxTime(float fTime)
{
    // 从 IDA: AddMaxTime 添加最大时间
    maxTimer_ += fTime;
}

void LogicTimer::SetNextTime(int nLeftTime, int nTotalTime)
{
    // 从 IDA: SetNextTime 设置下一个时间
    // TODO: 用于步骤之间的时间设置
    elapsedTimes_ = static_cast<float>(nLeftTime);
    maxTimer_ = static_cast<float>(nTotalTime);
}

void LogicTimer::SetOriginTime(float fTime)
{
    // 从 IDA: SetOriginTime 设置原始时间
    originTime_ = fTime;
}

int LogicTimer::GetStep() const
{
    // 从 IDA: GetStep 获取当前步骤
    return step_;
}

int LogicTimer::MoveNextStep()
{
    // 从 IDA: MoveNextStep 移动到下一步
    step_++;
    return step_;
}

void LogicTimer::SetType(int nType)
{
    // 从 IDA: SetType 设置定时器类型
    type_ = nType;
}

void LogicTimer::SetParam(int nParam1, int nParam2, int nParam3)
{
    // 从 IDA: SetParam 设置参数
    param1_ = nParam1;
    param2_ = nParam2;
    param3_ = nParam3;
}

int LogicTimer::GetParam2() const
{
    // 从 IDA: GetParam2 获取参数2
    return param2_;
}

int LogicTimer::GetParam3() const
{
    // 从 IDA: GetParam3 获取参数3
    return param3_;
}

void LogicTimer::SetUserString(const char* szString)
{
    // 从 IDA: SetUserString 设置用户字符串
    if (szString)
        userString_ = szString;
}

const char* LogicTimer::GetUserString() const
{
    // 从 IDA: GetUserString 获取用户字符串
    return userString_.c_str();
}

void LogicTimer::SetEventString(const char* szString)
{
    // 从 IDA: SetEventString 设置事件字符串
    if (szString)
        eventString_ = szString;
}

const char* LogicTimer::GetEventString() const
{
    // 从 IDA: GetEventString 获取事件字符串
    return eventString_.c_str();
}

void LogicTimer::SetReadyString(const char* szString)
{
    // 从 IDA: SetReadyString 设置准备字符串
    if (szString)
        readyString_ = szString;
}

const char* LogicTimer::GetReadyString() const
{
    // 从 IDA: GetReadyString 获取准备字符串
    return readyString_.c_str();
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

// ============================================================================
// FSM Timer Functions Implementation
// ============================================================================

namespace FSMTimer
{
    void ResetTimer(CFsmTransition* pTransition)
    {
        // 从 IDA: CFsmTransition::ResetTimer (0x14025DF50)
        // TODO: 需要 CFsmTransition 定义
        // pTransition->m_fTimer = 0.0f;
    }
    
    void ResetAttackTimer(CFsmTransition* pTransition)
    {
        // 从 IDA: CFsmTransition::ResetAttackTimer (0x14025DFA0)
        // TODO: 需要 CFsmTransition 定义
        // pTransition->m_fAttackTimer = 0.0f;
    }
    
    void ResetMoveTimer(CFsmTransition* pTransition)
    {
        // 从 IDA: CFsmTransition::ResetMoveTimer (0x14025E000)
        // TODO: 需要 CFsmTransition 定义
        // pTransition->m_fMoveTimer = 0.0f;
    }
    
    float GetTimer(CFsmTransition* pTransition)
    {
        // 从 IDA: CFsmTransition::GetTimer (0x14025E0B0)
        // TODO: 需要 CFsmTransition 定义
        // return pTransition->m_fTimer;
        return 0.0f;
    }
    
    float GetAttackTimer(CFsmTransition* pTransition)
    {
        // 从 IDA: CFsmTransition::GetAttackTimer (0x14025DFC0)
        // TODO: 需要 CFsmTransition 定义
        // return pTransition->m_fAttackTimer;
        return 0.0f;
    }
    
    float GetMoveTimer(CFsmTransition* pTransition)
    {
        // 从 IDA: CFsmTransition::GetMoveTimer (0x14025E020)
        // TODO: 需要 CFsmTransition 定义
        // return pTransition->m_fMoveTimer;
        return 0.0f;
    }
    
    void UpdateAttackTimer(CFsmTransition* pTransition, float fElapsed)
    {
        // 从 IDA: CFsmState::UpdateAttackTransitionTimer (0x1402740C0)
        // TODO: 需要 CFsmState 和 CFsmTransition 定义
    }
    
    void UpdateMoveTimer(CFsmTransition* pTransition, float fElapsed)
    {
        // 从 IDA: CFsmState::UpdateMoveTransitionTimer (0x140274140)
        // TODO: 需要 CFsmState 和 CFsmTransition 定义
    }
}

// ============================================================================
// IVTimer Implementation
// ============================================================================

IVTimer::IVTimer()
    : timeDifference_(0.0f)
{
}

IVTimer::~IVTimer()
{
}

float IVTimer::GetTimeDifference() const
{
    // 从 IDA: IVTimer::GetTimeDifference (0x140049010)
    return timeDifference_;
}

void IVTimer::SetTimeDifference(float fDiff)
{
    // 从 IDA: IVTimer::SetTimeDifference (0x1406D08D0)
    timeDifference_ = fDiff;
}

// ============================================================================
// VDefaultTimer Implementation
// ============================================================================

VDefaultTimer::VDefaultTimer(bool bAutoDelete)
    : bAutoDelete_(bAutoDelete)
{
}

VDefaultTimer::~VDefaultTimer()
{
}

void VDefaultTimer::Update()
{
    // 从 IDA: VDefaultTimer::Update
    // TODO: 实现默认定时器更新逻辑
}

void VDefaultTimer::Init()
{
    // 从 IDA: VDefaultTimer::Init
    // TODO: 实现默认定时器初始化逻辑
}

void VDefaultTimer::DeleteThis()
{
    // 从 IDA: VDefaultTimer::DeleteThis (0x1406E0720)
    if (bAutoDelete_)
    {
        delete this;
    }
}

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
    newTimer.SetVariables(nID);  // 从 CFsmCondition 继承
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
    newTimer.SetVariables(nID);
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
        newTimer.SetVariables(nID);
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
    
    return newTimer.GetGroupID();  // 从 CFsmCondition 继承
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
            return timer.GetGroupID() == nID;  // 从 CFsmCondition 继承
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
        newTimer.SetVariables(itFirst->first);
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
    // 从 IDA: XMaze::ShowCasualRaidTimer (0x14032B390)
    // 显示休闲副本定时器
    // TODO: 发送包到客户端显示定时器UI
}

void TimerManager::SetupCasualRaidTimer()
{
    // 从 IDA: XMaze::SetupCasualRaidTimer (0x14032B4B0)
    // 设置休闲副本定时器
    // TODO: 初始化休闲副本定时器
}

void TimerManager::SendCasualRaidTimer()
{
    // 从 IDA: XMaze::SendCasualRaidTimer (0x14032B4E0)
    // 发送休闲副本定时器到客户端
    // TODO: 发送定时器信息到客户端
}

void TimerManager::UpdateCasualRaidTimer(float fElapsed)
{
    // 从 IDA: XMaze::UpdateCasualRaidTimer (0x14031D850)
    // 更新休闲副本定时器
    // TODO: 更新休闲副本特定定时器
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

// ============================================================================
// ThreadLocalData Implementation
// ============================================================================

VDefaultTimer* ThreadLocalData::GetTimer()
{
    // 从 IDA: ThreadLocalData::GetTimer (0x1406D1A80)
    // TODO: 实现线程局部存储的定时器访问
    static VDefaultTimer s_defaultTimer(true);
    return &s_defaultTimer;
}

// ============================================================================
// Helper Functions for VHashString
// ============================================================================

class VHashString
{
public:
    static int GetHash(const char* szString)
    {
        // 简单的字符串哈希函数
        if (!szString)
            return 0;
        
        int hash = 0;
        while (*szString)
        {
            hash = hash * 31 + (*szString);
            szString++;
        }
        return hash;
    }
};
