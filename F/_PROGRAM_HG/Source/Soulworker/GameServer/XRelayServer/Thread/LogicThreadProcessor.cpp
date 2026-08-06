#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <windows.h>
#endif

#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

#include "Soulworker/GameServer/XCore/XServer/ThreadBase.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"

#include <chrono>
#include <cstdint>
#include <thread>
#include <utility>

namespace {
thread_local int g_relayLogicThreadIndex = 0;

class GreenDamTan_RelayLogicWorker final : public CThreadBase {
public:
    explicit GreenDamTan_RelayLogicWorker(const char* name)
        : m_proc()
        , m_arg()
        , m_thread()
        , m_name(name ? name : "") {
        m_arg.pevStop = nullptr;
        m_arg.pevSignal = nullptr;
        m_arg.m_pbStop = nullptr;
        m_arg.pArg = nullptr;
        m_arg.strThreadName = m_name;
        m_arg.pProcInstance = &m_proc;
    }

    ~GreenDamTan_RelayLogicWorker() override {
        StopAndJoin();
    }

    bool Start(int index) {
        m_arg.pevStop = &GetStopEvent();
        m_arg.pArg = reinterpret_cast<void*>(
            static_cast<std::intptr_t>(index));
        m_thread = std::thread([this]() {
            m_proc.ThreadProc(&m_arg);
        });
        return true;
    }

    void StopAndJoin() {
        StopThread();
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    CLogicThreadProc m_proc;
    THREAD_PROC_ARG m_arg;
    std::thread m_thread;
    std::string m_name;
};

static GreenDamTan_RelayLogicWorker* AsRelayWorker(CThreadBase* thread) {
    return static_cast<GreenDamTan_RelayLogicWorker*>(thread);
}
}

GreenDamTan_ThreadEvent::GreenDamTan_ThreadEvent()
    : m_state(new State())
    , m_reserved{0, 0} {
}

GreenDamTan_ThreadEvent::~GreenDamTan_ThreadEvent() {
    delete m_state;
}

void GreenDamTan_ThreadEvent::Set() {
    std::lock_guard<std::mutex> lock(m_state->mutex);
    m_state->signaled = true;
    m_state->condition.notify_all();
}

void GreenDamTan_ThreadEvent::Wait() {
    std::unique_lock<std::mutex> lock(m_state->mutex);
    m_state->condition.wait(lock, [this]() {
        return m_state->signaled;
    });
    m_state->signaled = false;
}

GreenDamTan_ConcurrentQueue::GreenDamTan_ConcurrentQueue()
    : m_state(new State())
    , m_reserved{0, 0, 0, 0} {
}

GreenDamTan_ConcurrentQueue::~GreenDamTan_ConcurrentQueue() {
    delete m_state;
}

void GreenDamTan_ConcurrentQueue::Push(
    const std::tr1::function<void()>& job) {
    std::lock_guard<std::mutex> lock(m_state->mutex);
    m_state->jobs.push_back(job);
}

bool GreenDamTan_ConcurrentQueue::TryPop(
    std::tr1::function<void()>& job) {
    std::lock_guard<std::mutex> lock(m_state->mutex);
    if (m_state->jobs.empty()) {
        return false;
    }
    job = std::move(m_state->jobs.front());
    m_state->jobs.pop_front();
    return true;
}

std::size_t GreenDamTan_ConcurrentQueue::UnsafeSize() const {
    std::lock_guard<std::mutex> lock(m_state->mutex);
    return m_state->jobs.size();
}

CLogicThreadProc::CLogicThreadProc()
    : m_initEvent()
    , m_nIndex(0)
    , m_fSumTickElapsed(0.0f)
    , m_bInit(false)
    , m_bInitPool(false)
    , m_nFrame(0)
    , m_dwFpsTick(0)
    , m_dwPrevTick(0)
    , m_dwMaxElapsedTick(0)
    , m_nPrintCount(0)
    , m_nMinTick(0)
    , m_nTickOverCount(0)
    , m_nMaxJobCount(0)
    , m_bShowJobCount(false)
    , m_concurrentQueue() {
}

CLogicThreadProc::~CLogicThreadProc() = default;

unsigned long CLogicThreadProc::ThreadProc(const THREAD_PROC_ARG* pArg) {
    if (!pArg) {
        return 0;
    }

    g_relayLogicThreadIndex = static_cast<int>(
        reinterpret_cast<std::intptr_t>(pArg->pArg));
    m_nIndex = g_relayLogicThreadIndex;
    OnInitializeThread();
    m_initEvent.Set();
    m_bInit = true;
    m_bInitPool = true;

    while (!pArg->pevStop || !pArg->pevStop->Wait(1)) {
        std::tr1::function<void()> job;
        while (m_concurrentQueue.TryPop(job)) {
            job();
        }

        const std::size_t jobCount = m_concurrentQueue.UnsafeSize();
        if (jobCount > static_cast<std::size_t>(m_nMaxJobCount)) {
            m_nMaxJobCount = static_cast<int>(jobCount);
            m_bShowJobCount = true;
        }

        OnUpdate(0.0166f);
        CheckFPS();
    }

    OnFinalizeThread();
    return 0;
}

void CLogicThreadProc::OnInitializeThread() {
    m_fSumTickElapsed = 0.0f;
    m_nFrame = 0;
    m_nPrintCount = 0;
    m_dwFpsTick = 0;
    m_dwPrevTick = 0;
    m_dwMaxElapsedTick = 0;
    m_nMinTick = 0;
    m_nTickOverCount = 0;
    m_nMaxJobCount = 0;
    m_bShowJobCount = false;
    m_bInit = false;
    m_bInitPool = false;
}

void CLogicThreadProc::OnFinalizeThread() {
    m_bInitPool = false;
    m_bInit = false;
}

void CLogicThreadProc::OnUpdate(float fElapsed) {
    m_fSumTickElapsed += fElapsed;
    XRelayServer* relayServer = TXSingleton<XRelayServer>::Instance();
    if (!relayServer) {
        return;
    }

    if (m_nIndex == 0) {
        relayServer->GetPartyManager().Clear();
        relayServer->GetPartyMatchingMgr().OnUpdate();
        relayServer->GetForceManager().Clear();
        relayServer->GetForceMatchingMgr().OnUpdate();
        CModeMazeMatchingMgr::Instance().OnUpdate();
    } else if (m_nIndex == 1) {
        relayServer->GetLeagueManager().OnUpdate();
    } else if (m_nIndex == 2) {
        relayServer->GetRecruitManager().OnUpdate();
    }
}

void CLogicThreadProc::WaitForInit() {
    m_initEvent.Wait();
}

bool CLogicThreadProc::IsReady() {
    return m_bInit && m_bInitPool;
}

std::uint64_t CLogicThreadProc::GetLastFpsTick() {
    return m_dwFpsTick;
}

void CLogicThreadProc::AddJob(std::tr1::function<void()> job) {
    m_concurrentQueue.Push(job);
}

void CLogicThreadProc::CheckFPS() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    m_dwPrevTick = static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
    if (!m_dwFpsTick || m_dwPrevTick >= m_dwFpsTick) {
        m_dwFpsTick = m_dwPrevTick + 1000;
        m_nFrame = 0;
    } else {
        ++m_nFrame;
    }
}

CLogicThreadManager& CLogicThreadManager::Instance() {
    static CLogicThreadManager instance;
    return instance;
}

CLogicThreadManager::CLogicThreadManager()
    : m_isStart(false)
    , m_isReady(false)
    , m_bShutdown(false)
    , m_reserved(0)
    , m_nWorkerThreadNumber(0)
    , m_pWorkerThreadList{}
    , m_nThreadUserList{}
    , m_nCommonWorkerThreadNumber(0)
    , m_nSGCommonWorkerThreadNumber(0) {
}

CLogicThreadManager::~CLogicThreadManager() {
    End();
}

bool CLogicThreadManager::Start(int workerCount) {
    if (m_isStart) {
        return false;
    }

    m_nWorkerThreadNumber = workerCount;
    m_nCommonWorkerThreadNumber = workerCount;
    m_nSGCommonWorkerThreadNumber = workerCount + 1;
    m_bShutdown = false;

    for (int index = 0; index < workerCount + 2; ++index) {
        const std::string name =
            "ConcurrentJobProcessor Thread " + std::to_string(index);
        CThreadBase* workerBase = CreateWorkerThread(name.c_str());
        m_pWorkerThreadList[index] = workerBase;
        if (!workerBase || !AsRelayWorker(workerBase)->Start(index)) {
            Clear();
            return false;
        }
        AsRelayWorker(workerBase)->m_proc.WaitForInit();
    }

    m_isStart = true;
    return true;
}

void CLogicThreadManager::End() {
    if (!m_isStart) {
        return;
    }

    m_bShutdown = true;
    for (int index = 0; index < m_nWorkerThreadNumber + 2; ++index) {
        if (m_pWorkerThreadList[index]) {
            AsRelayWorker(m_pWorkerThreadList[index])->StopAndJoin();
        }
    }
    Clear();
    m_isStart = false;
    m_isReady = false;
    m_nWorkerThreadNumber = 0;
}

bool CLogicThreadManager::DoJob(
    std::int64_t instanceID,
    std::tr1::function<void()> job) {
    if (m_nWorkerThreadNumber) {
        const std::int64_t threadID = instanceID % m_nWorkerThreadNumber;
        if (threadID >= 0 && threadID < m_nWorkerThreadNumber &&
            m_pWorkerThreadList[threadID]) {
            AsRelayWorker(m_pWorkerThreadList[threadID])->m_proc.AddJob(job);
            return true;
        }
    }
    return false;
}

bool CLogicThreadManager::DoCommonJob(
    std::tr1::function<void()> job) {
    if (m_nWorkerThreadNumber &&
        m_pWorkerThreadList[m_nCommonWorkerThreadNumber]) {
        AsRelayWorker(m_pWorkerThreadList[m_nCommonWorkerThreadNumber])
            ->m_proc.AddJob(job);
        return true;
    }
    return false;
}

bool CLogicThreadManager::DoSGCommonJob(
    std::tr1::function<void()> job) {
    if (m_nWorkerThreadNumber &&
        m_pWorkerThreadList[m_nSGCommonWorkerThreadNumber]) {
        AsRelayWorker(m_pWorkerThreadList[m_nSGCommonWorkerThreadNumber])
            ->m_proc.AddJob(job);
        return true;
    }
    return false;
}

void CLogicThreadManager::DoJobAllThread(
    std::tr1::function<void()> job) {
    for (int index = 0; index < m_nWorkerThreadNumber; ++index) {
        if (m_pWorkerThreadList[index]) {
            std::tr1::function<void()> copiedJob(job);
            AsRelayWorker(m_pWorkerThreadList[index])->m_proc.AddJob(
                std::move(copiedJob));
        }
    }
}

int CLogicThreadManager::GetThreadCount() {
    return m_nWorkerThreadNumber;
}

bool CLogicThreadManager::IsReady() {
    if (m_bShutdown) {
        return false;
    }
    for (int index = 0; index < m_nWorkerThreadNumber + 2; ++index) {
        if (!m_pWorkerThreadList[index] ||
            !AsRelayWorker(m_pWorkerThreadList[index])->m_proc.IsReady()) {
            return false;
        }
    }
    return true;
}

bool CLogicThreadManager::IsShutdown() {
    return m_bShutdown;
}

int CLogicThreadManager::GetCommonThreadIndex() {
    return m_nCommonWorkerThreadNumber;
}

std::uint64_t CLogicThreadManager::GetLastFpsTick(int threadID) {
    if (threadID < 0 || threadID >= m_nWorkerThreadNumber + 2 ||
        !m_pWorkerThreadList[threadID]) {
        return 0;
    }
    return AsRelayWorker(m_pWorkerThreadList[threadID])->m_proc.GetLastFpsTick();
}

int CLogicThreadManager::GetCurThreadIndex() {
    return g_relayLogicThreadIndex;
}

CThreadBase* CLogicThreadManager::CreateWorkerThread(const char* name) {
    return new GreenDamTan_RelayLogicWorker(name);
}

void CLogicThreadManager::Clear() {
    for (int index = 0; index < m_nWorkerThreadNumber + 2; ++index) {
        if (m_pWorkerThreadList[index]) {
            delete AsRelayWorker(m_pWorkerThreadList[index]);
            m_pWorkerThreadList[index] = nullptr;
        }
        m_nThreadUserList[index] = 0;
    }
}
