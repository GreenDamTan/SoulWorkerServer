// LogicThreadManager_Stub.cpp
// Active GameServer landing for the PDB-backed logic-thread boundary.

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"
#include "Soulworker/GameServer/XCore/XServer/ThreadBase.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_TimeCompat.h"
#include "Soulworker/GameServer/XCore/HavokTypes.h"

#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"

#include <cstdint>
#include <cstdio>
#include <utility>

namespace {
thread_local int g_logicThreadIndex = 0;

class GreenDamTan_LogicWorker final : public CThreadBase {
public:
    explicit GreenDamTan_LogicWorker(const char* name)
        : m_name(name ? name : "") {
        m_arg.pevStop = nullptr;
        m_arg.pevSignal = nullptr;
        m_arg.m_pbStop = nullptr;
        m_arg.pArg = nullptr;
        m_arg.strThreadName = m_name;
        m_arg.pProcInstance = &m_proc;
    }

    ~GreenDamTan_LogicWorker() override {
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

static GreenDamTan_LogicWorker* AsLogicWorker(CThreadBase* thread) {
    return static_cast<GreenDamTan_LogicWorker*>(thread);
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

    g_logicThreadIndex = static_cast<int>(reinterpret_cast<std::intptr_t>(pArg->pArg));
    LogHelper::LogInfo(
        "game.system",
        "Start LocalThread ( %d )",
        g_logicThreadIndex);
    OnInitializeThread();
    m_initEvent.Set();

    hkMemoryRouter memoryRouter;
    while (!m_bInit) {
        hkMemorySystem* memorySystem = hkMemorySystem::getInstancePtr();
        if (memorySystem) {
            memorySystem->threadInit(
                memoryRouter,
                "gameThread",
                hkMemorySystem::Flags(hkMemorySystem::FLAG_ALL));
            hkBaseSystem::initThread(&memoryRouter);
            m_bInit = true;
        }
    }

    if (!m_bInitPool && m_bInit) {
        CLogicThreadManager& manager = CLogicThreadManager::Instance();
        if (manager.GetCommonThreadIndex() > g_logicThreadIndex) {
            ThreadLocalData::GetInstance()->InitPool();
        }
        m_bInitPool = true;
    }

    CLogicThreadManager& manager = CLogicThreadManager::Instance();
    for (;;) {
        if (manager.GetCommonThreadIndex() < g_logicThreadIndex) {
            const int jobCount = static_cast<int>(m_concurrentQueue.UnsafeSize());
            if (m_nMaxJobCount < jobCount) {
                m_nMaxJobCount = jobCount;
                m_bShowJobCount = true;
            }
        }

        std::tr1::function<void()> job;
        while (m_concurrentQueue.TryPop(job)) {
            job();
        }

        VDefaultTimer* timer = ThreadLocalData::GetTimer();
        timer->Update();
        OnUpdate(timer->GetTimeDifference());
        CheckFPS();
        if (pArg->pevStop && pArg->pevStop->Wait(1)) {
            break;
        }
    }

    OnFinalizeThread();
    hkBaseSystem::quitThread();
    hkMemorySystem::getInstance().threadQuit(
        memoryRouter,
        hkMemorySystem::Flags(hkMemorySystem::FLAG_ALL));
    return 0;
}

void CLogicThreadProc::OnInitializeThread() {
    const int ownerThreadIndex = CLogicThreadManager::GetCurThreadIndex();
    CLogicThreadManager& manager = CLogicThreadManager::Instance();
    ThreadLocalData* data = ThreadLocalData::CreateInstance(
        manager.GetThreadCount(),
        ownerThreadIndex);
    m_nIndex = ownerThreadIndex;
    if (data) {
        data->Initialize();
    }
    m_fSumTickElapsed = 0.0f;
    m_nFrame = 0;
    m_nPrintCount = 0;
    m_dwFpsTick = 0;
    m_nTickOverCount = 0;
    m_nMinTick = 0;
    m_bInit = false;
    m_bInitPool = false;
    m_nMaxJobCount = 0;
    m_bShowJobCount = false;
    m_dwMaxElapsedTick = 0;
    m_dwPrevTick = 0;
}

void CLogicThreadProc::OnFinalizeThread() {
    ThreadLocalData* data = ThreadLocalData::GetInstance();
    if (data) {
        data->Clear();
    }
    ThreadLocalData::DestroyInstance();
    m_bInit = false;
    m_bInitPool = false;
}

void CLogicThreadProc::OnUpdate(float fElapsed) {
    m_fSumTickElapsed += fElapsed;
    if (m_fSumTickElapsed >= 0.0166f) {
        VDefaultTimer* timer = ThreadLocalData::GetTimer();
        if (timer) {
            timer->SetTimeDifference(m_fSumTickElapsed);
        }
        ThreadLocalData* data = ThreadLocalData::GetInstance();
        if (data) {
            data->Update(m_fSumTickElapsed);
        }
        m_fSumTickElapsed = 0.0f;
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
    const std::uint64_t nowTick = GreenDamTan::GetTickCount64Compat();
    if (m_dwFpsTick >= nowTick) {
        ++m_nFrame;
    } else {
        const std::uint64_t elapsed =
            nowTick - (m_dwFpsTick - 1000);
        const float fps = elapsed == 0
            ? 0.0f
            : static_cast<float>(m_nFrame) /
                  static_cast<float>(elapsed) * 1000.0f;
        ++m_nPrintCount;
        if (m_nPrintCount == 10) {
            std::printf(
                "<THREAD %d> FPS:%6.2f / FRAME:%d \\n",
                CLogicThreadManager::GetCurThreadIndex(),
                fps,
                m_nFrame);
            m_nPrintCount = 0;
        }
        m_dwFpsTick = nowTick + 1000;
        m_nFrame = 0;
    }

    if (!m_nMinTick) {
        m_dwMaxElapsedTick = 0;
        m_dwPrevTick = 0;
        m_nMinTick = GreenDamTan::GetTickCount64Compat();
    }

    const std::uint32_t elapsedTick = static_cast<std::uint32_t>(
        nowTick - static_cast<std::uint32_t>(m_dwPrevTick));
    if (elapsedTick > 0x10) {
        ++m_nTickOverCount;
    }
    if (m_dwMaxElapsedTick < elapsedTick) {
        m_dwMaxElapsedTick = elapsedTick;
    }
    if (m_nMinTick < nowTick) {
        LogHelper::LogInfo(
            "game.system",
            "<TICK %d> Delay Count ( %d, %d )",
            CLogicThreadManager::GetCurThreadIndex(),
            m_nTickOverCount,
            m_dwMaxElapsedTick);
        m_nTickOverCount = 0;
        m_dwMaxElapsedTick = 0;
        m_nMinTick = nowTick + 60000;
        if (m_bShowJobCount) {
            LogHelper::LogInfo(
                "game.system",
                "<SG_BILL> JOB count ( %d )",
                m_nMaxJobCount);
            m_bShowJobCount = false;
        }
    }
    m_dwPrevTick = nowTick;
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
    m_nCommonWorkerThreadNumber = m_nWorkerThreadNumber;
    m_nSGCommonWorkerThreadNumber = m_nWorkerThreadNumber + 1;
    m_bShutdown = false;

    for (int index = 0; index < workerCount + 2; ++index) {
        CThreadBase* workerBase = CreateWorkerThread(
            (std::string("ConcurrentJobProcessor Thread ") +
             std::to_string(index)).c_str());
        m_pWorkerThreadList[index] = workerBase;
        if (!workerBase || !AsLogicWorker(workerBase)->Start(index)) {
            Clear();
            return false;
        }
        AsLogicWorker(workerBase)->m_proc.WaitForInit();
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
            AsLogicWorker(m_pWorkerThreadList[index])->StopAndJoin();
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
            AsLogicWorker(m_pWorkerThreadList[threadID])->m_proc.AddJob(job);
            return true;
        }
        return false;
    }
    return false;
}

bool CLogicThreadManager::DoCommonJob(std::tr1::function<void()> job) {
    if (m_nWorkerThreadNumber &&
        m_pWorkerThreadList[m_nCommonWorkerThreadNumber]) {
        AsLogicWorker(m_pWorkerThreadList[m_nCommonWorkerThreadNumber])
            ->m_proc.AddJob(job);
        return true;
    }
    return false;
}

bool CLogicThreadManager::DoSGCommonJob(
    std::tr1::function<void()> job) {
    if (m_nWorkerThreadNumber &&
        m_pWorkerThreadList[m_nSGCommonWorkerThreadNumber]) {
        AsLogicWorker(m_pWorkerThreadList[m_nSGCommonWorkerThreadNumber])
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
            AsLogicWorker(m_pWorkerThreadList[index])->m_proc.AddJob(
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
            !AsLogicWorker(m_pWorkerThreadList[index])->m_proc.IsReady()) {
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
    return AsLogicWorker(m_pWorkerThreadList[threadID])->m_proc.GetLastFpsTick();
}

int CLogicThreadManager::GetCurThreadIndex() {
    return g_logicThreadIndex;
}

CThreadBase* CLogicThreadManager::CreateWorkerThread(const char* name) {
    return new GreenDamTan_LogicWorker(name);
}

void CLogicThreadManager::Clear() {
    for (int index = 0; index < m_nWorkerThreadNumber + 2; ++index) {
        if (m_pWorkerThreadList[index]) {
            delete AsLogicWorker(m_pWorkerThreadList[index]);
            m_pWorkerThreadList[index] = nullptr;
        }
        m_nThreadUserList[index] = 0;
    }
}
