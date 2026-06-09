// ThreadLocalData_Stub.cpp
// Stub implementation for ThreadLocalData static methods
// This file provides minimal implementations to resolve linker errors

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XGameServer/VaccumCube.h"

// Static instances for stub implementation
static ThreadLocalData* s_pInstance = nullptr;
static VDefaultTimer* s_pTimer = nullptr;

// Stub ThreadLocalData class for linkage
class ThreadLocalData {
public:
    static ThreadLocalData* GetInstance();
    static VDefaultTimer* GetTimer();

    CVaccumCube* CreateVaccumCubeObject(XVec3 vPos);
    void DeleteVaccumCubeObject(CVaccumCube* pVaccumCube);
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

CVaccumCube* ThreadLocalData::CreateVaccumCubeObject(XVec3 vPos) {
    CVaccumCube* pVaccum = CVaccumCube::CreateObject();
    if (pVaccum) {
        pVaccum->SetPosition(hkvVec3(vPos.x, vPos.y, vPos.z));
    }
    return pVaccum;
}

void ThreadLocalData::DeleteVaccumCubeObject(CVaccumCube* pVaccumCube) {
    delete pVaccumCube;
}
