// FSRWLock.h
// CFSRWLock - Windows Slim Read/Write Lock wrapper
// IDA decompilation from GameServer.exe
//
// Functions:
// - CFSRWLock::CFSRWLock (0x14019c4a0)
// - CFSRWLock::lock (0x14019c3f0)
// - CFSRWLock::unlock (0x14019c3b0)

#pragma once

#ifdef _WIN32
#include <windows.h>
#else
// Linux fallback using pthread_rwlock
#include <pthread.h>
#endif

// ============================================================================
// CFSRWLock - Windows SRWLock wrapper class
// Simple exclusive lock wrapper for Slim Read/Write Lock
// ============================================================================
class CFSRWLock {
public:
    // === Constructor ===
    // IDA: ??0CFSRWLock@@QEAA@XZ @ 0x14019C4A0
    CFSRWLock() {
#ifdef _WIN32
        InitializeSRWLock(&m_SharedLock);
#else
        pthread_rwlock_init(&m_rwlock, nullptr);
#endif
    }

    // === Destructor ===
    ~CFSRWLock() {
#ifndef _WIN32
        pthread_rwlock_destroy(&m_rwlock);
#endif
    }

    // === Lock Operations ===

    // lock - Acquire exclusive lock
    // IDA: ?lock@CFSRWLock@@QEAAXXZ @ 0x14019C3F0
    void lock() {
#ifdef _WIN32
        AcquireSRWLockExclusive(&m_SharedLock);
#else
        pthread_rwlock_wrlock(&m_rwlock);
#endif
    }

    // unlock - Release exclusive lock
    // IDA: ?unlock@CFSRWLock@@QEAAXXZ @ 0x14019C3B0
    void unlock() {
#ifdef _WIN32
        ReleaseSRWLockExclusive(&m_SharedLock);
#else
        pthread_rwlock_unlock(&m_rwlock);
#endif
    }

    // === Shared Lock Operations (for read-write semantics) ===

    // lock_shared - Acquire shared lock (reader)
    void lock_shared() {
#ifdef _WIN32
        AcquireSRWLockShared(&m_SharedLock);
#else
        pthread_rwlock_rdlock(&m_rwlock);
#endif
    }

    // unlock_shared - Release shared lock (reader)
    void unlock_shared() {
#ifdef _WIN32
        ReleaseSRWLockShared(&m_SharedLock);
#else
        pthread_rwlock_unlock(&m_rwlock);
#endif
    }

private:
#ifdef _WIN32
    SRWLOCK m_SharedLock;
#else
    pthread_rwlock_t m_rwlock;
#endif
};

// ============================================================================
// CFSRWLockGuard - RAII wrapper for exclusive lock
// ============================================================================
class CFSRWLockGuard {
public:
    explicit CFSRWLockGuard(CFSRWLock& lock) : m_lock(lock) {
        m_lock.lock();
    }

    ~CFSRWLockGuard() {
        m_lock.unlock();
    }

private:
    CFSRWLock& m_lock;
    // Non-copyable
    CFSRWLockGuard(const CFSRWLockGuard&) = delete;
    CFSRWLockGuard& operator=(const CFSRWLockGuard&) = delete;
};

// ============================================================================
// CFSRWLockSharedGuard - RAII wrapper for shared lock
// ============================================================================
class CFSRWLockSharedGuard {
public:
    explicit CFSRWLockSharedGuard(CFSRWLock& lock) : m_lock(lock) {
        m_lock.lock_shared();
    }

    ~CFSRWLockSharedGuard() {
        m_lock.unlock_shared();
    }

private:
    CFSRWLock& m_lock;
    // Non-copyable
    CFSRWLockSharedGuard(const CFSRWLockSharedGuard&) = delete;
    CFSRWLockSharedGuard& operator=(const CFSRWLockSharedGuard&) = delete;
};
