#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"

#ifdef AddJob
#undef AddJob
#endif
#include <chrono>
#include <utility>

CLogicThreadProc::CLogicThreadProc() = default;

// 对齐 IDA: UEAA = public virtual destructor
CLogicThreadProc::~CLogicThreadProc() = default;

// 对齐 IDA: QEAAK = public, returns unsigned long
std::uint32_t CLogicThreadProc::ThreadProc(int threadIndex) {
    LogHelper::LogDebug("game.relay",
                        "GreenDamTan_log LogicThreadProcessor.cpp::CLogicThreadProc::ThreadProc start threadIndex=%d",
                        threadIndex);
    m_nIndex = threadIndex;
    OnInitializeThread();

    {
        std::lock_guard<std::mutex> lock(m_initEvent.lock);
        m_initEvent.signaled = true;
    }
    m_initEvent.cv.notify_all();

    while (!stopRequested_.load()) {
        std::function<void()> job;
        if (TryDequeueJob(job)) {
            job();
        }

        OnUpdate();

        const auto now = std::chrono::steady_clock::now().time_since_epoch();
        m_dwPrevTick = static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
        ++m_nFrame;

        std::unique_lock<std::mutex> lock(queueMutex_);
        queueCv_.wait_for(lock, std::chrono::milliseconds(1), [this]() {
            return stopRequested_.load() || !m_concurrentQueue.empty();
        });
    }

    OnFinalizeThread();
    return 0;
}

void CLogicThreadProc::OnInitializeThread() {
    m_bInit = true;
}

void CLogicThreadProc::OnFinalizeThread() {
    m_bInit = false;
}

void CLogicThreadProc::OnUpdate() {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    if (m_nIndex == 0) {
        relayServer.GetPartyManager().Clear();
        relayServer.GetPartyMatchingMgr().OnUpdate();
        relayServer.GetForceManager().Clear();
        relayServer.GetForceMatchingMgr().OnUpdate();
        CModeMazeMatchingMgr::Instance().OnUpdate();
    } else if (m_nIndex == 1) {
        relayServer.GetLeagueManager().OnUpdate();
    } else if (m_nIndex == 2) {
        relayServer.GetRecruitManager().OnUpdate();
    }
}

void CLogicThreadProc::AddJob(std::function<void()> job) {
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        m_concurrentQueue.push_back(std::move(job));
    }
    queueCv_.notify_one();
}

bool CLogicThreadProc::TryDequeueJob(std::function<void()>& job) {
    std::lock_guard<std::mutex> lock(queueMutex_);
    if (m_concurrentQueue.empty()) {
        return false;
    }
    job = std::move(m_concurrentQueue.front());
    m_concurrentQueue.pop_front();
    return true;
}

void CLogicThreadProc::RequestStop() {
    stopRequested_.store(true);
    queueCv_.notify_all();
}

// 对齐 IDA: 缺失的存根方法
bool CLogicThreadProc::InitData() {
    m_bInit = true;
    return true;
}

bool CLogicThreadProc::LoadData() {
    return true;
}

void CLogicThreadProc::WaitForInit() {
    std::unique_lock<std::mutex> lock(m_initEvent.lock);
    m_initEvent.cv.wait(lock, [this]() { return m_initEvent.signaled; });
}

void CLogicThreadProc::CheckFPS() {
    // 对齐 IDA: FPS 检查逻辑
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    m_dwFpsTick = static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

CLogicThreadManager& CLogicThreadManager::Instance() {
    static CLogicThreadManager instance;
    return instance;
}

// 对齐 IDA: UEAA = public virtual destructor
CLogicThreadManager::~CLogicThreadManager() {
    End();
}

bool CLogicThreadManager::IsShutdown() {
    return !started_.load();
}

int CLogicThreadManager::GetCurThreadIndex() {
    return 0;  // 对齐 IDA: 返回当前线程索引
}

class CThreadBase* CLogicThreadManager::CreateWorkerThread(const char* name) {
    // 对齐 IDA: 虚方法存根
    (void)name;
    return nullptr;
}

void CLogicThreadManager::Clear() {
    // 对齐 IDA: 私有清理方法
    workers_.clear();
}

bool CLogicThreadManager::Start(int workerCount) {
    if (started_.exchange(true)) {
        return true;
    }

    if (workerCount <= 0) {
        workerCount = 1;
    }

    workers_.reserve(static_cast<std::size_t>(workerCount));
    for (int index = 0; index < workerCount; ++index) {
        GreenDamTan_WorkerThread worker;
        worker.m_strThreadName = "RelayLogicThread" + std::to_string(index);
        worker.proc = std::make_unique<CLogicThreadProc>();
        CLogicThreadProc* proc = worker.proc.get();
        worker.m_hHandle = std::thread([proc, index]() {
            proc->ThreadProc(index);
        });
        workers_.push_back(std::move(worker));
    }
    return true;
}

void CLogicThreadManager::End() {
    if (!started_.exchange(false)) {
        return;
    }

    for (auto& worker : workers_) {
        if (worker.proc) {
            worker.proc->RequestStop();
        }
    }
    for (auto& worker : workers_) {
        if (worker.m_hHandle.joinable()) {
            worker.m_hHandle.join();
        }
    }
    workers_.clear();
}

bool CLogicThreadManager::DoJob(std::int64_t instanceID, std::function<void()> job) {
    if (!started_.load() || workers_.empty()) {
        return false;
    }

    const std::int64_t workerCount = static_cast<std::int64_t>(workers_.size());
    const std::int64_t normalizedIndex = workerCount > 0 ? (instanceID % workerCount + workerCount) % workerCount : 0;
    auto& worker = workers_[static_cast<std::size_t>(normalizedIndex)];
    if (!worker.proc) {
        return false;
    }

    worker.proc->AddJob(std::move(job));
    return true;
}
