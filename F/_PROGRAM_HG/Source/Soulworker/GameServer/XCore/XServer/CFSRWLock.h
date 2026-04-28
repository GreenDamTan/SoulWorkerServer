// CFSRWLock.h
// 跨平台读写锁封装

#pragma once

#define CFSRWLOCK_H_ALREADY_DEFINED

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

    // 兼容接口：lock/unlock/lock_shared/unlock_shared
    void lock() { LockWrite(); }
    void unlock() { UnlockWrite(); }
    void lock_shared() { LockRead(); }
    void unlock_shared() { UnlockRead(); }

private:
    std::shared_mutex m_mutex;
};

// 自动读锁 - 支持引用和指针两种方式
class CFAutoSlimReadLock {
public:
    explicit CFAutoSlimReadLock(CFSRWLock& lock) : m_pLock(&lock) {
        m_pLock->LockRead();
    }
    explicit CFAutoSlimReadLock(CFSRWLock* lock) : m_pLock(lock) {
        if (m_pLock) m_pLock->LockRead();
    }
    ~CFAutoSlimReadLock() {
        if (m_pLock) m_pLock->UnlockRead();
    }

private:
    CFSRWLock* m_pLock = nullptr;
};

// 自动写锁 - 支持引用和指针两种方式
class CFAutoSlimWriteLock {
public:
    explicit CFAutoSlimWriteLock(CFSRWLock& lock) : m_pLock(&lock) {
        m_pLock->LockWrite();
    }
    explicit CFAutoSlimWriteLock(CFSRWLock* lock) : m_pLock(lock) {
        if (m_pLock) m_pLock->LockWrite();
    }
    ~CFAutoSlimWriteLock() {
        if (m_pLock) m_pLock->UnlockWrite();
    }

private:
    CFSRWLock* m_pLock = nullptr;
};