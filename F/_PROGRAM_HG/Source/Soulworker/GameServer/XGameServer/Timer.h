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

// CFsmCondition base class - IDA struct size: 40 bytes
class CFsmCondition
{
public:
    CFsmCondition() 
        : m_eVariable(0)
        , m_eVariableDataType(0)
        , m_eConditionFunction(0)
        , m_nSkillGroup(0)
        , m_nSkillIndex(0)
        , m_bDestroy(false)
    {
        m_nValue[0] = 0;
        m_nValue[1] = 0;
        m_fValue[0] = 0.0f;
        m_fValue[1] = 0.0f;
    }
    
    virtual ~CFsmCondition() {}
    
    void SetVariables(int nID) { m_nValue[0] = nID; }
    int GetGroupID() const { return m_nValue[0]; }
    
protected:
    int m_eVariable;              // offset 0
    int m_eVariableDataType;      // offset 4
    int m_eConditionFunction;     // offset 8
    int m_nValue[2];              // offset 12
    float m_fValue[2];            // offset 20
    int m_nSkillGroup;            // offset 28
    int m_nSkillIndex;            // offset 32
    bool m_bDestroy;              // offset 36
};

// LogicTimer class - main timer implementation
// 从 IDA 反编译: LogicTimer 用于迷宫/关卡中的定时器逻辑
// IDA struct size: 88 bytes
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
    // IDA struct layout (88 bytes total)
    // Offset 0-7: inherited from CFsmCondition (id_, type_, enable_)
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

// Casual raid timer structure
// IDA struct size: 12 bytes
struct STCasualRaidTime
{
    int nIntValue;                  // offset 0 - integer value (timer type)
    float fFloatValue;              // offset 4 - float value (remaining time)
    float fWaitSendTime;            // offset 8 - wait send time
    
    STCasualRaidTime() : nIntValue(0), fFloatValue(0.0f), fWaitSendTime(0.0f) {}
    void reset() { nIntValue = 0; fFloatValue = 0.0f; fWaitSendTime = 0.0f; }
};

// FSM Timer functions (for AI state machine)
// IDA struct CFsmTransition size: 48 bytes
class CFsmTransition
{
public:
    void ResetTimer();
    void ResetAttackTimer();
    void ResetMoveTimer();
    
    float GetTimer() const;
    float GetAttackTimer() const;
    float GetMoveTimer() const;
    
    void AddAttackTime(float fTime);
    void AddMoveTime(float fTime);
    
private:
    float m_fTimer;                 // offset 0 - general timer
    float m_fAttackTimer;           // offset 4 - attack timer
    float m_fMoveTimer;             // offset 8 - move timer
    int m_eOutputStateName;         // offset 12 - output state name
    // std::vector<CFsmCondition*> at offset 16 (32 bytes)
};

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
// IDA struct size: 72 bytes
class IVTimer
{
public:
    IVTimer();
    virtual ~IVTimer();
    
    virtual void Update() = 0;
    virtual void Init() = 0;
    virtual void DeleteThis() = 0;
    
    // Time access
    float GetTime() const;
    
    // Time difference (for server time sync)
    float GetTimeDifference() const;
    void SetTimeDifference(float fDiff);
    
private:
    // IDA struct layout (72 bytes)
    // VRefCounter at offset 0 (16 bytes) - simplified
    void* vtable_;                   // offset 0 - vtable pointer
    int refCount_;                   // offset 8 - reference count
    int padding0_;                   // offset 12
    bool m_bFirstStart;              // offset 16 - first start flag
    bool m_bDisabled;                // offset 17 - disabled flag
    bool m_bFrozen;                  // offset 18 - frozen flag
    bool m_bSlowMotionEnabled;       // offset 19 - slow motion enabled
    float m_fDivCountsPerSecond;     // offset 20 - division counts per second
    unsigned long long m_iOldCount;  // offset 24 - old count
    unsigned long long m_iStartTimerValue; // offset 32 - start timer value
    unsigned long long m_iForcedCountNumber; // offset 40 - forced count number
    float m_fTime;                   // offset 48 - current time
    float m_fTimeDifference;         // offset 52 - time difference
    float m_fMaxTimeDifference;      // offset 56 - max time difference
    float m_fSlowMotionScale;        // offset 60 - slow motion scale
    unsigned long long m_iCurrentTimerTickPos; // offset 64 - current timer tick position
};

// Default timer implementation
// IDA struct size: 80 bytes (inherits IVTimer 72 bytes + 1 bool)
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
    bool m_bDeleteObject;           // offset 72 - auto delete flag
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
    STCasualRaidTime m_stCasualRaidTime;            // 休闲副本定时器
    XMaze* m_pMaze;                                 // 迷宫指针 (用于脚本调用)
    IVScriptInstance* m_pScriptInstance;            // 脚本实例指针
};

// Thread-local timer (for per-thread timer access)
class ThreadLocalData
{
public:
    static VDefaultTimer* GetTimer();
};
