#pragma once

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <mutex>
#include <string>
#include <utility>

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_StdCompat.h"

class CThreadBase;

#ifdef AddJob
#undef AddJob
#endif

class CKernelEvent;
class ThreadLocalData;

struct THREAD_PROC_ARG {
    CKernelEvent* pevStop;
    CKernelEvent* pevSignal;
    volatile int* m_pbStop;
    void* pArg;
    std::string strThreadName;
    void* pProcInstance;
};

static_assert(sizeof(THREAD_PROC_ARG) == 80);

class GreenDamTan_ThreadEvent {
private:
    struct State {
        std::mutex mutex;
        std::condition_variable condition;
        bool signaled = false;
    };

    State* m_state;
    std::uint64_t m_reserved[2];

public:
    GreenDamTan_ThreadEvent();
    ~GreenDamTan_ThreadEvent();
    GreenDamTan_ThreadEvent(const GreenDamTan_ThreadEvent&) = delete;
    GreenDamTan_ThreadEvent& operator=(const GreenDamTan_ThreadEvent&) = delete;

    void Set();
    void Wait();
};

static_assert(sizeof(GreenDamTan_ThreadEvent) == 24);

class GreenDamTan_ConcurrentQueue {
private:
    struct State {
        mutable std::mutex mutex;
        std::deque<std::tr1::function<void()>> jobs;
    };

    State* m_state;
    std::uint64_t m_reserved[4];

public:
    GreenDamTan_ConcurrentQueue();
    ~GreenDamTan_ConcurrentQueue();
    GreenDamTan_ConcurrentQueue(const GreenDamTan_ConcurrentQueue&) = delete;
    GreenDamTan_ConcurrentQueue& operator=(const GreenDamTan_ConcurrentQueue&) = delete;

    void Push(const std::tr1::function<void()>& job);
    bool TryPop(std::tr1::function<void()>& job);
    std::size_t UnsafeSize() const;
};

static_assert(sizeof(GreenDamTan_ConcurrentQueue) == 40);

class CLogicThreadProc {
public:
    explicit CLogicThreadProc();
    virtual ~CLogicThreadProc();

    unsigned long ThreadProc(const THREAD_PROC_ARG* pArg);
    virtual void OnInitializeThread();
    virtual void OnFinalizeThread();
    virtual void OnUpdate(float fElapsed);

    void WaitForInit();
    bool IsReady();
    std::uint64_t GetLastFpsTick();
    void AddJob(std::tr1::function<void()> job);
    void CheckFPS();

private:
    GreenDamTan_ThreadEvent m_initEvent;
    int m_nIndex;
    float m_fSumTickElapsed;
    bool m_bInit;
    bool m_bInitPool;
    int m_nFrame;
    std::uint64_t m_dwFpsTick;
    std::uint64_t m_dwPrevTick;
    std::uint64_t m_dwMaxElapsedTick;
    int m_nPrintCount;
    std::uint64_t m_nMinTick;
    int m_nTickOverCount;
    int m_nMaxJobCount;
    bool m_bShowJobCount;
    GreenDamTan_ConcurrentQueue m_concurrentQueue;
};

static_assert(sizeof(CLogicThreadProc) == 144);

class CLogicThreadManager {
public:
    static CLogicThreadManager& Instance();

    CLogicThreadManager();
    virtual ~CLogicThreadManager();

    bool Start(int workerCount = 1);
    void End();
    bool DoJob(std::int64_t instanceID, std::tr1::function<void()> job);
    bool DoCommonJob(std::tr1::function<void()> job);
    bool DoSGCommonJob(std::tr1::function<void()> job);
    void DoJobAllThread(std::tr1::function<void()> job);
    int GetThreadCount();
    bool IsReady();
    bool IsShutdown();
    int GetCommonThreadIndex();
    std::uint64_t GetLastFpsTick(int threadID);
    static int GetCurThreadIndex();

    virtual CThreadBase* CreateWorkerThread(const char* name);

private:
    void Clear();

    bool m_isStart;
    bool m_isReady;
    bool m_bShutdown;
    std::uint8_t m_reserved;
    int m_nWorkerThreadNumber;
    CThreadBase* m_pWorkerThreadList[20];
    int m_nThreadUserList[20];
    int m_nCommonWorkerThreadNumber;
    int m_nSGCommonWorkerThreadNumber;
};

static_assert(sizeof(CLogicThreadManager) == 264);
