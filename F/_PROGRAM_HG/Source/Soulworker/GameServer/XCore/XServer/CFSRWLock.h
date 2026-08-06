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
    CFSRWLock() {
#ifdef _WIN32
        InitializeSRWLock(&m_SharedLock);
#else
        m_SharedLock = new std::shared_mutex();
#endif
    }

    ~CFSRWLock() {
#ifndef _WIN32
        delete m_SharedLock;
        m_SharedLock = nullptr;
#endif
    }

    void LockRead() {
#ifdef _WIN32
        AcquireSRWLockShared(&m_SharedLock);
#else
        m_SharedLock->lock_shared();
#endif
    }

    void UnlockRead() {
#ifdef _WIN32
        ReleaseSRWLockShared(&m_SharedLock);
#else
        m_SharedLock->unlock_shared();
#endif
    }

    void LockWrite() {
#ifdef _WIN32
        AcquireSRWLockExclusive(&m_SharedLock);
#else
        m_SharedLock->lock();
#endif
    }

    void UnlockWrite() {
#ifdef _WIN32
        ReleaseSRWLockExclusive(&m_SharedLock);
#else
        m_SharedLock->unlock();
#endif
    }

    // 兼容接口：lock/unlock/lock_shared/unlock_shared
    void lock() { LockWrite(); }
    void unlock() { UnlockWrite(); }
    void lock_shared() { LockRead(); }
    void unlock_shared() { UnlockRead(); }

private:
#ifdef _WIN32
    SRWLOCK m_SharedLock{};
#else
    std::shared_mutex* m_SharedLock = nullptr;
#endif
};

#ifdef _WIN32
static_assert(sizeof(SRWLOCK) == 8, "SRWLOCK size must match GameServer PDB");
#endif
static_assert(sizeof(CFSRWLock) == 8, "CFSRWLock size must match GameServer PDB");

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