#pragma once

#include <string>
#include <list>
#include <map>

// Timer result enumeration
enum TimerResult_e
{
    TIMER_CONTINUE = 0,
    COMPLETE = 1,
    TIMER_ERROR = 2
};

// Timer type enumeration
enum TimerType_e
{
    TIMER_TYPE_NONE = 0,
    TIMER_TYPE_READY = 1,
    TIMER_TYPE_SCRIPT = 2,
    TIMER_TYPE_TIME_STEP = 3
};

// Forward declarations
class XMaze;
class IVScriptInstance;
class CFsmCondition;

// LogicTimer class - main timer implementation
// 从 IDA 反编译: LogicTimer 用于迷宫/关卡中的定时器逻辑
class LogicTimer : public CFsmCondition
{
public:
    LogicTimer();
    LogicTimer(const LogicTimer& other);
    ~LogicTimer();

    // Timer control
    void SetTimer(float fTarget, bool bReset);
    void SetPause(bool bPause);
    void SetReady(bool bReady);
    void SetCallReadyScript(bool bCall);
    void SetFinishStep(bool bFinish);
    void Disable(bool bDisable);
    
    // Timer update
    TimerResult_e Update(float fElapsed);
    
    // Timer state query
    float GetTimer() const;
    float GetMaxTimer() const;
    float GetLeftTime() const;
    float GetOriginTime() const;
    int GetNextLeftTime() const;
    bool IsPause() const;
    bool IsReady() const;
    bool IsCallReadyScript() const;
    bool IsFinishStep() const;
    bool IsLastStep() const;
    
    // Timer modification
    void SetTimerEx(float fTime);
    void AddMaxTime(float fTime);
    void SetNextTime(int nLeftTime, int nTotalTime);
    void SetOriginTime(float fTime);
    
    // Step management
    int GetStep() const;
    int MoveNextStep();
    void SetType(int nType);
    
    // Parameter management
    void SetParam(int nParam1, int nParam2, int nParam3);
    int GetParam2() const;
    int GetParam3() const;
    
    // String management
    void SetUserString(const char* szString);
    const char* GetUserString() const;
    void SetEventString(const char* szString);
    const char* GetEventString() const;
    void SetReadyString(const char* szString);
    const char* GetReadyString() const;

private:
    float timer_;                    // 当前计时器时间
    float maxTimer_;                 // 最大计时器时间
    float elapsedTimes_;            // 已经过的时间
    float originTime_;              // 原始时间
    int type_;                      // 计时器类型 (0=普通, 1=准备, 2=脚本, 3=时间步进)
    int step_;                      // 当前步骤
    bool bPause_;                   // 是否暂停
    bool bReady_;                   // 是否准备好
    bool bCallReadyScript_;         // 是否调用准备脚本
    bool bFinishStep_;              // 是否完成步骤
    bool bDisable_;                 // 是否禁用
    int param1_;                    // 参数1
    int param2_;                    // 参数2
    int param3_;                    // 参数3
    std::string userString_;        // 用户字符串 (脚本函数名)
    std::string eventString_;       // 事件字符串
    std::string readyString_;       // 准备字符串
};

// Time step timer structure
// 从 IDA: ST_TIME_STEP_TIMER 用于时间步进定时器
struct ST_TIME_STEP_TIMER
{
    float fTime;                    // 时间
    int nSpawnBoxID;                // 生成盒子ID
    int nDeathMotion;               // 死亡动作
    std::string strTimeout;         // 超时脚本
    
    ST_TIME_STEP_TIMER();
    ST_TIME_STEP_TIMER(const ST_TIME_STEP_TIMER& other);
    ~ST_TIME_STEP_TIMER();
};

// FSM Timer functions (for AI state machine)
class CFsmTransition;

namespace FSMTimer
{
    // Timer reset functions
    void ResetTimer(CFsmTransition* pTransition);
    void ResetAttackTimer(CFsmTransition* pTransition);
    void ResetMoveTimer(CFsmTransition* pTransition);
    
    // Timer query functions
    float GetTimer(CFsmTransition* pTransition);
    float GetAttackTimer(CFsmTransition* pTransition);
    float GetMoveTimer(CFsmTransition* pTransition);
    
    // Timer update functions
    void UpdateAttackTimer(CFsmTransition* pTransition, float fElapsed);
    void UpdateMoveTimer(CFsmTransition* pTransition, float fElapsed);
}

// Interface for timer system (abstract base class)
class IVTimer
{
public:
    IVTimer();
    virtual ~IVTimer();
    
    virtual void Update() = 0;
    virtual void Init() = 0;
    virtual void DeleteThis() = 0;
    
    // Time difference (for server time sync)
    float GetTimeDifference() const;
    void SetTimeDifference(float fDiff);
    
private:
    float timeDifference_;          // 时间差 (用于服务器时间同步)
};

// Default timer implementation
class VDefaultTimer : public IVTimer
{
public:
    VDefaultTimer(bool bAutoDelete);
    virtual ~VDefaultTimer();
    
    virtual void Update() override;
    virtual void Init() override;
    virtual void DeleteThis() override;
    
    // Serialization
    // virtual void SerializeX(VArchive& ar) override;
    
private:
    bool bAutoDelete_;              // 是否自动删除
};

// Timer manager for XMaze
// 从 IDA: XMaze 中的定时器管理
class TimerManager
{
public:
    TimerManager();
    ~TimerManager();
    
    // Create timer functions (CreateTimer implementation)
    int AddTimer(int nID, float fTarget);
    int AddTimerEx(const char* szLuaFunction, float fTarget, int nParam1, int nParam2, int nParam3);
    int AddEventTimer(const char* szReady, const char* szEvent, const char* szUser, float fTarget, int nType, int nParam1, int nParam2, int nParam3);
    int AddTimeStepTimer(int nID, float fTime, int nSpawnBoxID, const char* szTimeout);
    
    // Destroy timer functions (DestroyTimer implementation)
    void RemoveTimer(int nID);
    float RemoveTimerEx(const char* szLuaFunction);
    void ClearAllTimers();
    
    // Set timer callback (SetTimer implementation)
    void StartEventTimer(const char* szLuaFunction);
    void StartTimeStepTimer();
    
    // Get remaining time (GetRemainingTime implementation)
    float GetTimerLeftTime(int nID) const;
    
    // Pause/Resume timer (PauseTimer implementation)
    void PauseAllTimers(bool bPause);
    
    // Timer update (called each frame)
    void UpdateTimers(float fElapsed);
    
    // Casual raid timer functions
    void ShowCasualRaidTimer(int nType, float fMaxTime, float fLeftTime);
    void SetupCasualRaidTimer();
    void SendCasualRaidTimer();
    void UpdateCasualRaidTimer(float fElapsed);
    
    // Access to timer lists (for serialization)
    std::list<LogicTimer>& GetLogicTimers();
    std::list<LogicTimer>& GetWaitLogicTimers();
    std::map<int, ST_TIME_STEP_TIMER>& GetTimeStepTimers();
    
private:
    std::list<LogicTimer> m_arLogicTimers;          // 活动定时器列表
    std::list<LogicTimer> m_arWaitLogicTimers;      // 等待定时器列表
    std::map<int, ST_TIME_STEP_TIMER> m_mapTimeStepTimer; // 时间步进定时器映射
    XMaze* m_pMaze;                                 // 迷宫指针 (用于脚本调用)
    IVScriptInstance* m_pScriptInstance;            // 脚本实例指针
};

// Thread-local timer (for per-thread timer access)
class ThreadLocalData
{
public:
    static VDefaultTimer* GetTimer();
};
