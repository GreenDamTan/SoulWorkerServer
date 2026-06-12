// LogicThreadManager_Stub.cpp
// Stub implementation of CLogicThreadManager for GameServer
// The full implementation is in XRelayServer/Thread/LogicThreadProcessor.cpp
// This stub avoids pulling in XRelayServer headers that conflict with GameServer headers

#include <atomic>
#include <functional>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <string>

// Minimal stub for CLogicThreadProc - just needs to be complete type
class CLogicThreadProc {
public:
    CLogicThreadProc() = default;
    ~CLogicThreadProc() = default;
};

// Minimal CLogicThreadManager implementation for GameServer
// This matches the class definition in LogicThreadProcessor.h
class CLogicThreadManager {
public:
    static CLogicThreadManager& Instance();

    bool Start(int workerCount = 1);
    void End();
    bool DoJob(std::int64_t instanceID, std::function<void()> job);
    bool IsShutdown();
    static int GetCurThreadIndex();

protected:
    virtual class CThreadBase* CreateWorkerThread(const char* name);

private:
    friend class CLogicThreadProc;

    struct GreenDamTan_WorkerThread {
        std::string m_strThreadName;
        std::thread m_hHandle;
        std::unique_ptr<CLogicThreadProc> proc;
    };

    CLogicThreadManager() = default;
    virtual ~CLogicThreadManager();
    CLogicThreadManager(const CLogicThreadManager&) = delete;
    CLogicThreadManager& operator=(const CLogicThreadManager&) = delete;

    void Clear();

    std::vector<GreenDamTan_WorkerThread> workers_;
    std::atomic<bool> started_{false};
};

// Stub for CThreadBase
class CThreadBase {
public:
    virtual ~CThreadBase() = default;
};

// Implementation
CLogicThreadManager& CLogicThreadManager::Instance() {
    static CLogicThreadManager instance;
    return instance;
}

CLogicThreadManager::~CLogicThreadManager() {
    End();
}

bool CLogicThreadManager::Start(int workerCount) {
    if (started_.exchange(true)) {
        return true;
    }
    // Minimal stub - no actual worker threads for GameServer
    // GameServer uses this for async job dispatch but we'll execute synchronously
    return true;
}

void CLogicThreadManager::End() {
    if (!started_.exchange(false)) {
        return;
    }
    workers_.clear();
}

bool CLogicThreadManager::DoJob(std::int64_t instanceID, std::function<void()> job) {
    if (!started_.load()) {
        return false;
    }
    // Execute synchronously in stub mode
    if (job) {
        job();
    }
    return true;
}

bool CLogicThreadManager::IsShutdown() {
    return !started_.load();
}

int CLogicThreadManager::GetCurThreadIndex() {
    return 0;
}

CThreadBase* CLogicThreadManager::CreateWorkerThread(const char* name) {
    return nullptr;
}

void CLogicThreadManager::Clear() {
    workers_.clear();
}
