#pragma once

#include <string>
#include <list>
#include <map>

// Forward declarations
class XMaze;
class IVScriptInstance;
class VDefaultTimer;

// Note: CFsmCondition and CFsmTransition are defined in FsmClass.h
// LogicTimer inherits from CFsmCondition

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

// STCasualRaidTime - Casual raid timer structure (also defined in Maze.h)
// IDA struct size: 12 bytes
struct STCasualRaidTime
{
    int nIntValue;                  // offset 0 - integer value (timer type)
    float fFloatValue;              // offset 4 - float value (remaining time)
    float fWaitSendTime;            // offset 8 - wait send time

    STCasualRaidTime() : nIntValue(0), fFloatValue(0.0f), fWaitSendTime(0.0f) {}
    void reset() { nIntValue = 0; fFloatValue = 0.0f; fWaitSendTime = 0.0f; }
};

// VHashString - simple string hash utility
class VHashString
{
public:
    static int GetHash(const char* szString)
    {
        if (!szString)
            return 0;

        int hash = 0;
        while (*szString)
        {
            hash = hash * 31 + static_cast<int>(*szString);
            szString++;
        }
        return hash;
    }
};

// Forward declaration for inheritance
class CFsmCondition;

// LogicTimer class - main timer implementation
// 从 IDA 反编译: LogicTimer 用于迷宫/关卡中的定时器逻辑
// IDA struct size: 88 bytes
// Note: LogicTimer inherits from CFsmCondition (defined in FsmClass.h)
class LogicTimer
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
    int GetType() const { return type_; }

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

    // ID access (returns id_ field - first 4 bytes)
    int GetID() const { return id_; }
    void SetID(int nID) { id_ = nID; }

    // GroupID access (same as id_ for compatibility)
    int GetGroupID() const { return id_; }
    void SetGroupID(int nID) { id_ = nID; }

private:
    // IDA struct layout (88 bytes total)
    int id_;                         // offset 0 - timer ID
    int type_;                       // offset 4 - timer type
    bool enable_;                    // offset 8 - enabled flag
    // Padding at offset 9-11
    float timer_;                    // offset 12 - current timer value (max time target)
    float elapsedTimes_;             // offset 16 - elapsed time
    float originTime_;               // offset 20 - origin time
    void* customData_;               // offset 24 - custom data pointer
    // VString is 8 bytes (pointer + length or similar)
    // Using const char* for simplicity - actual VString implementation needed
    const char* userString_;         // offset 32 - user string (script function name)
    const char* eventString_;        // offset 40 - event string
    const char* readyString_;        // offset 48 - ready string
    int m_nParam1;                   // offset 56 - parameter 1 (total steps)
    int m_nParam2;                   // offset 60 - parameter 2
    int m_nParam3;                   // offset 64 - parameter 3
    int m_nEventStep;                // offset 68 - current event step
    bool m_bFinishStep;              // offset 72 - finish step flag
    bool m_bReady;                   // offset 73 - ready flag
    bool m_bPause;                   // offset 74 - pause flag
    bool m_bCallReadyScript;         // offset 75 - call ready script flag
    int m_nNextLeftTime;             // offset 76 - next left time
    int m_nNextTotalTime;            // offset 80 - next total time
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

// Forward declaration
class IVScriptInstance;

// TimerManager class - manages timers for XMaze
class TimerManager
{
public:
    TimerManager();
    ~TimerManager();

    // Create timer functions
    int AddTimer(int nID, float fTarget);
    int AddTimerEx(const char* szLuaFunction, float fTarget, int nParam1, int nParam2, int nParam3);
    int AddEventTimer(const char* szReady, const char* szEvent, const char* szUser,
                      float fTarget, int nType, int nParam1, int nParam2, int nParam3);
    int AddTimeStepTimer(int nID, float fTime, int nSpawnBoxID, const char* szTimeout);

    // Destroy timer functions
    void RemoveTimer(int nID);
    float RemoveTimerEx(const char* szLuaFunction);
    void ClearAllTimers();

    // Set timer callback
    void StartEventTimer(const char* szLuaFunction);
    void StartTimeStepTimer();

    // Get remaining time
    float GetTimerLeftTime(int nID) const;

    // Pause/Resume timer
    void PauseAllTimers(bool bPause);

    // Timer update
    void UpdateTimers(float fElapsed);

    // Casual raid timer functions
    void ShowCasualRaidTimer(int nType, float fMaxTime, float fLeftTime);
    void SetupCasualRaidTimer();
    void SendCasualRaidTimer();
    void UpdateCasualRaidTimer(float fElapsed);

    // Access to timer lists
    std::list<LogicTimer>& GetLogicTimers();
    std::list<LogicTimer>& GetWaitLogicTimers();
    std::map<int, ST_TIME_STEP_TIMER>& GetTimeStepTimers();

private:
    std::list<LogicTimer> m_arLogicTimers;
    std::list<LogicTimer> m_arWaitLogicTimers;
    std::map<int, ST_TIME_STEP_TIMER> m_mapTimeStepTimer;
    STCasualRaidTime m_stCasualRaidTime;
    XMaze* m_pMaze;
    IVScriptInstance* m_pScriptInstance;
};

// Note: CFsmTransition is defined in FsmClass.h
// Note: IVTimer, VDefaultTimer, STCasualRaidTime, and ThreadLocalData are defined in:
// - VisionEngineTypes.h (IVTimer, VDefaultTimer, ThreadLocalData)
// - Maze.h (STCasualRaidTime)
