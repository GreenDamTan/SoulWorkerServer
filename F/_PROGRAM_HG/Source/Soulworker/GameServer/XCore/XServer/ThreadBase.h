// ThreadBase.h
// CThreadBase - Base class for threads with stop event
// Simplified implementation for cross-platform compatibility

#pragma once

#include "KernelEvent.h"
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#else
#include <thread>
#include <pthread.h>
#endif

// CThreadBase - Base class for worker threads
// Provides stop event and stop flag for graceful shutdown
class CThreadBase {
public:
    CThreadBase() : m_bStopFlag(0) {}
    virtual ~CThreadBase() = default;

    // IDA: ?StopThread@CThreadBase@@QEAAXXZ (0x1401ef5d0)
    // Stop the thread by setting stop event and flag
    void StopThread() {
        CKernelEvent::Set(&m_evStop);  // Set stop event
        m_bStopFlag = 1;               // Set stop flag
    }

    // Check if thread should stop
    bool IsStopRequested() const {
        return m_bStopFlag != 0;
    }

    // Wait for stop event
    void WaitForStop() {
        m_evStop.Wait();
    }

    // Wait for stop with timeout
    bool WaitForStop(unsigned int dwMilliseconds) {
        return m_evStop.Wait(dwMilliseconds);
    }

    // Get stop event for external use
    CKernelEvent& GetStopEvent() { return m_evStop; }
    const CKernelEvent& GetStopEvent() const { return m_evStop; }

protected:
    CKernelEvent m_evStop;    // Stop event for synchronization
    std::int32_t m_bStopFlag; // Stop flag (0 = running, 1 = stop requested)
};
