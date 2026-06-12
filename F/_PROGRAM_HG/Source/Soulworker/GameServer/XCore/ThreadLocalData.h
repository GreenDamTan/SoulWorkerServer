// ThreadLocalData.h
// Thread-local storage for game server
// IDA decompilation from GameServer.exe

#pragma once

#include <cstdint>

// Forward declarations - use VDefaultTimer from VisionEngineTypes.h
class VDefaultTimer;
class IVTimer;

/**
 * @brief ThreadLocalData - Thread-local storage for timer and other per-thread data
 * 
 * Used to access per-thread timer and other thread-specific data.
 * IDA: ?GetTimer@ThreadLocalData@@SAPEAVVDefaultTimer@@XZ (0x1406D1A80)
 */
class ThreadLocalData {
public:
    // Get timer instance for current thread
    // IDA: ?GetTimer@ThreadLocalData@@SAPEAVVDefaultTimer@@XZ
    static VDefaultTimer* GetTimer();
    
    // Alternative: Get base timer interface
    static IVTimer* GetTimerInterface();

private:
    ThreadLocalData() = delete;
    ~ThreadLocalData() = delete;
};
