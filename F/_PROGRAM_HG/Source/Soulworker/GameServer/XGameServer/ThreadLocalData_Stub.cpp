// ThreadLocalData_Stub.cpp
// Stub implementation for ThreadLocalData static methods
// This file provides minimal implementations to resolve linker errors

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"

// Static instances for stub implementation
static ThreadLocalData* s_pInstance = nullptr;
static VDefaultTimer* s_pTimer = nullptr;

// Stub ThreadLocalData class for linkage
class ThreadLocalData {
public:
    static ThreadLocalData* GetInstance();
    static VDefaultTimer* GetTimer();
};

ThreadLocalData* ThreadLocalData::GetInstance() {
    if (!s_pInstance) {
        s_pInstance = new ThreadLocalData();
    }
    return s_pInstance;
}

VDefaultTimer* ThreadLocalData::GetTimer() {
    if (!s_pTimer) {
        s_pTimer = new VDefaultTimer();
    }
    return s_pTimer;
}
