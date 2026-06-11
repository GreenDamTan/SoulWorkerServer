// ThreadLocalData.h
// Thread-local storage for game server
// IDA decompilation from GameServer.exe

#pragma once

#include <cstdint>

// Forward declarations
class CTimer;

/**
 * @brief ThreadLocalData - Thread-local storage for timer and other per-thread data
 * 
 * Used to access per-thread timer and other thread-specific data.
 */
class ThreadLocalData {
public:
    // Get timer instance for current thread
    // IDA: ?GetTimer@ThreadLocalData@@QEAAPEAVCTimer@@XZ
    static CTimer* GetTimer();

private:
    ThreadLocalData() = delete;
    ~ThreadLocalData() = delete;
};

/**
 * @brief CTimer - Timer class for measuring time differences
 */
class CTimer {
public:
    CTimer() : m_fTime(0.0f) {}
    ~CTimer() {}

    // Get time difference since last call
    float GetTimeDifference() { 
        float fDiff = m_fTime;
        m_fTime = 0.0f;
        return fDiff;
    }

    // Update timer
    void Update(float fElapsed) { m_fTime += fElapsed; }

private:
    float m_fTime;
};
