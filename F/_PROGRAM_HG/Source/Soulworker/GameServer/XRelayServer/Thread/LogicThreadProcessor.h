#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#ifdef AddJob
#undef AddJob
#endif

class CLogicThreadProc {
public:
    // 中文说明：
    // 1. `LogicThreadProcessor.cpp` 已由 PDB/files 与 strings.txt 双重确认存在。
    // 2. 本轮先恢复逻辑线程的最小作业队列/调度承接面，保持 `DoJob -> AddJob -> ThreadProc` 边界。
    explicit CLogicThreadProc();

    std::uint64_t ThreadProc(int threadIndex);
    void OnInitializeThread();
    void OnFinalizeThread();
    void OnUpdate();

    void AddJob(std::function<void()> job);
    bool TryDequeueJob(std::function<void()>& job);
    void RequestStop();
    bool IsStopRequested() const { return stopRequested_.load(); }

private:
    struct GreenDamTan_InitEvent {
        std::mutex lock;
        std::condition_variable cv;
        bool signaled = false;
    };

    GreenDamTan_InitEvent m_initEvent;
    mutable std::mutex queueMutex_;
    std::condition_variable queueCv_;
    std::deque<std::function<void()>> m_concurrentQueue;
    std::atomic<bool> stopRequested_{false};
    bool m_bInit = false;
    int m_nIndex = 0;
    int m_nFrame = 0;
    int m_nPrintCount = 0;
    std::uint64_t m_dwFpsTick = 0;
    std::uint64_t m_dwPrevTick = 0;
};

class CLogicThreadManager {
public:
    static CLogicThreadManager& Instance();

    bool Start(int workerCount = 1);
    void End();
    bool DoJob(std::int64_t instanceID, std::function<void()> job);

private:
    friend class CLogicThreadProc;

    struct GreenDamTan_WorkerThread {
        std::string m_strThreadName;
        std::thread m_hHandle;
        std::unique_ptr<CLogicThreadProc> proc;
    };

    CLogicThreadManager() = default;
    ~CLogicThreadManager();
    CLogicThreadManager(const CLogicThreadManager&) = delete;
    CLogicThreadManager& operator=(const CLogicThreadManager&) = delete;

    std::vector<GreenDamTan_WorkerThread> workers_;
    std::atomic<bool> started_{false};
};
