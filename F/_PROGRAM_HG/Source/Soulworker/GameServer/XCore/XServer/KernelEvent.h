// KernelEvent.h
// CKernelEvent - Kernel event synchronization primitive
// Simplified implementation for cross-platform compatibility

#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <condition_variable>
#include <mutex>
#endif

// CKernelEvent - Windows-style kernel event object
// Used for thread synchronization
class CKernelEvent {
public:
    CKernelEvent() {
#ifdef _WIN32
        m_hEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
#else
        m_signaled = false;
#endif
    }

    ~CKernelEvent() {
#ifdef _WIN32
        if (m_hEvent) {
            CloseHandle(m_hEvent);
        }
#endif
    }

    // Set event to signaled state
    void Set() {
#ifdef _WIN32
        if (m_hEvent) {
            SetEvent(m_hEvent);
        }
#else
        std::lock_guard<std::mutex> lock(m_mutex);
        m_signaled = true;
        m_cv.notify_one();
#endif
    }

    // Wait for event to be signaled
    void Wait() {
#ifdef _WIN32
        if (m_hEvent) {
            WaitForSingleObject(m_hEvent, INFINITE);
        }
#else
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] { return m_signaled; });
        m_signaled = false;
#endif
    }

    // Wait with timeout in milliseconds
    bool Wait(unsigned int dwMilliseconds) {
#ifdef _WIN32
        if (m_hEvent) {
            return WaitForSingleObject(m_hEvent, dwMilliseconds) == WAIT_OBJECT_0;
        }
        return false;
#else
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_cv.wait_for(lock, std::chrono::milliseconds(dwMilliseconds),
                             [this] { return m_signaled; });
#endif
    }

private:
#ifdef _WIN32
    HANDLE m_hEvent;
#else
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_signaled;
#endif
};
