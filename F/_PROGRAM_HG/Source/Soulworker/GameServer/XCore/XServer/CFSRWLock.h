// CFSRWLock.h
// 跨平台读写锁封装

#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <mutex>
#include <shared_mutex>

// 对齐 ControlServer.exe IDA
class CFSRWLock {
public:
    CFSRWLock() = default;
    ~CFSRWLock() = default;

    void LockRead() {
        m_mutex.lock_shared();
    }

    void UnlockRead() {
        m_mutex.unlock_shared();
    }

    void LockWrite() {
        m_mutex.lock();
    }

    void UnlockWrite() {
        m_mutex.unlock();
    }

private:
    std::shared_mutex m_mutex;
};

// 自动读锁
class CFAutoSlimReadLock {
public:
    explicit CFAutoSlimReadLock(CFSRWLock& lock) : m_lock(lock) {
        m_lock.LockRead();
    }
    ~CFAutoSlimReadLock() {
        m_lock.UnlockRead();
    }

private:
    CFSRWLock& m_lock;
};

// 自动写锁
class CFAutoSlimWriteLock {
public:
    explicit CFAutoSlimWriteLock(CFSRWLock& lock) : m_lock(lock) {
        m_lock.LockWrite();
    }
    ~CFAutoSlimWriteLock() {
        m_lock.UnlockWrite();
    }

private:
    CFSRWLock& m_lock;
};
