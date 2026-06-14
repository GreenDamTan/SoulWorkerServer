// ThreadLocalData_Stub.cpp
// Stub implementation for ThreadLocalData static methods
// This file provides minimal implementations to resolve linker errors

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XGameServer/VaccumCube.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/Npc.h"
#include "Soulworker/GameServer/XGameServer/XMonsterMgr.h"

// Static instances for stub implementation
static ThreadLocalData* s_pInstance = nullptr;
static VDefaultTimer* s_pTimer = nullptr;

// ============================================================================
// ThreadLocalData Constructor - IDA @ 0x1406D0A50
// ============================================================================
ThreadLocalData::ThreadLocalData()
    : m_nMazeCount(0)
    , m_nMonsterCount(0)
    , m_nModeMazeCount(0)
    , m_bInitPool(false)
    , m_bLoadWorld(false)
    , m_bReqSyncWorld(false)
    , m_nLogSynctime(0)
    , m_nReportSynctime(0)
    , m_nThreadCount(0)
    , m_nOwnerThreadIndex(0)
    , m_xPartyMgr(nullptr)
    , m_xForceMgr(nullptr)
    , m_xMonsterMgr(new XMonsterMgr())
    , m_xNpcMgr(nullptr)
    , m_xAkashicMgr(nullptr)
    , m_xProjectileMgr(nullptr)
    , m_xTrapMgr(nullptr)
    , m_xChainLightningMgr(nullptr)
    , m_xInteractionObjectMgr(nullptr)
    , m_xVaccumCubeObjectMgr(nullptr)
    , m_xSocialItemObjectMgr(nullptr)
    , m_xGameModeMgr(nullptr)
    , m_xLuaScript(nullptr)
    , m_xScriptManager(nullptr)
    , m_DohHavokResourceManager(nullptr)
    , m_xDistrictPool(nullptr)
    , m_xBattleZonePool(nullptr)
    , m_xMazePool(nullptr)
    , m_xMyRoomPool(nullptr)
    , m_xModeMazePool(nullptr)
    , m_pDistrictCreator(nullptr)
    , m_pBattleZoneCreator(nullptr)
    , m_pMazeCreator(nullptr)
    , m_pMyRoomCreator(nullptr)
    , m_pModeMazeCreator(nullptr)
{
}

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

// ============================================================================
// CreateMonster - Stub implementation for linker
// IDA: ?CreateMonster@ThreadLocalData@@QEAAPEAVCMonster@@PEAVXArea@@TUXMapID@@HUXVec3@@M@Z
// ============================================================================
CMonster* ThreadLocalData::CreateMonster(XArea* pArea, UXMapID uxMapID, int nMonsterID, XVec3 vPos, float fRot) {
    // IDA: Create monster using monster manager
    CMonster* pMonster = nullptr;
    if (m_xMonsterMgr) {
        pMonster = m_xMonsterMgr->Create(uxMapID, nMonsterID, &vPos, fRot);
    }

    if (pMonster) {
        // Increment monster count
        ++m_nMonsterCount;

        // Set area for the monster
        // pMonster->SetArea(pArea);

        // Generate event object
        // pMonster->GenerateEventObject();

        // Check height and adjust position
        // hkvVec3 vCheckPos(vPos.x, vPos.y, vPos.z);
        // if (pMonster->GetHeight(&vCheckPos, 200.0f)) {
        //     vPos.z = vCheckPos.z;
        //     pMonster->Move(&vPos);
        // }

        // Set initial yaw
        // pMonster->SetInitYaw(fRot);
    }

    return pMonster;
}

// ============================================================================
// GetScriptManager - Stub implementation for linker
// IDA: ?GetScriptManager@ThreadLocalData@@QEAAAEAVVScriptResourceManager@@XZ
// ============================================================================
VScriptResourceManager& ThreadLocalData::GetScriptManager() {
    static VScriptResourceManager s_instance;
    return s_instance;
}

// ============================================================================
// DeleteMonster - Stub implementation for linker
// IDA: ?DeleteMonster@ThreadLocalData@@QEAAXPEAVCMonster@@@Z
// ============================================================================
void ThreadLocalData::DeleteMonster(CMonster* pMonster) {
    // IDA: Delete monster using monster manager
    if (!pMonster) return;
    if (m_xMonsterMgr) {
        m_xMonsterMgr->Delete(pMonster);
    }
}

// ============================================================================
// CreateNpc - Stub implementation for linker
// IDA: ?CreateNpc@ThreadLocalData@@QEAAPEAVCNpc@@PEAVXArea@@TUXMapID@@KHUXVec3@@MH@Z
// ============================================================================
CNpc* ThreadLocalData::CreateNpc(XArea* pArea, UXMapID uxMapID, std::uint32_t nSectorID, int nNpcID, XVec3* vPos, float fRot, int nUnused) {
    // IDA: Create NPC - currently stubbed
    // TODO: Implement using XNpcMgr when available
    (void)pArea;
    (void)uxMapID;
    (void)nSectorID;
    (void)nNpcID;
    (void)vPos;
    (void)fRot;
    (void)nUnused;
    return nullptr;
}

// ============================================================================
// DeleteNpc - Stub implementation for linker
// ============================================================================
void ThreadLocalData::DeleteNpc(CNpc* pNpc) {
    // IDA: Delete NPC using NPC manager
    if (!pNpc) return;
    // TODO: Implement using XNpcMgr when available
    delete pNpc;
}
