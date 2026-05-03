#include "Soulworker/GameServer/XDBAgent/DBAgent.h"

#include <thread>

XDBThread::XDBThread() {
    m_nLastSessionID = 0;
    m_nThreadID = 0;
    m_bStart = false;
    m_pDBConnect = nullptr;
    m_pDBStmt = nullptr;
    m_hasJobEvent = false;
    m_byType = 0;
}

XDBThread::~XDBThread() {
    SetStart(false);
    m_jobCv.notify_all();
    if (m_worker.joinable()) {
        m_worker.join();
    }
    Clear();
}

void XDBThread::SetStart(bool bStart) {
    m_bStart = bStart;
}

bool XDBThread::IsStart() {
    return m_bStart.load();
}

void XDBThread::Clear() {
    if (m_pDBStmt) {
        m_pDBStmt->Clear();
        delete m_pDBStmt;
        m_pDBStmt = nullptr;
    }
    m_hasJobEvent = false;
}

unsigned int XDBThread::_BackendThreadStartingPoint(void* ptr) {
    if (auto* pThread = static_cast<XDBThread*>(ptr)) {
        pThread->SetStart(true);
        pThread->_DBWorkingThread();
        pThread->SetStart(false);
    }
    return 0;
}

void XDBThread::_DBWorkingThread() {
    while (IsStart()) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_jobCv.wait(lock, [this]() { return !m_jobs.empty() || !IsStart(); });
            if (!IsStart()) {
                break;
            }
            job = std::move(m_jobs.front());
            m_jobs.pop();
        }
        while (job) {
            job();
            std::this_thread::yield();
            std::unique_lock<std::mutex> lock(m_queueMutex);
            if (m_jobs.empty()) {
                break;
            }
            job = std::move(m_jobs.front());
            m_jobs.pop();
        }
    }
}

bool XDBThread::CreateThread(XDBConnect* pDBConnect, std::uint8_t byType) {
    if (!pDBConnect) {
        return false;
    }
    m_pDBConnect = pDBConnect;
    m_pDBStmt = new XDBStmt();
    if (!m_pDBStmt) {
        return false;
    }
    m_hasJobEvent = true;
    m_byType = byType;
    m_pDBStmt->Init(m_pDBConnect, reinterpret_cast<void*>(1), nullptr);
    m_worker = std::thread([this]() { _BackendThreadStartingPoint(this); });
    return m_worker.joinable();
}

void XDBThread::AddJob(std::function<void()> func) {
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_jobs.push(std::move(func));
    }
    m_jobCv.notify_one();
}

XDBStmt* XDBThread::GetDBStmt() {
    return m_pDBStmt;
}