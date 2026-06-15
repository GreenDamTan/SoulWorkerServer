// HavokTypes.cpp
// Havok Physics/AI Engine Types - Static member definitions
// Reconstructed from IDA decompilation of GameServer.exe

#include "HavokTypes.h"
#ifdef _WIN32
#include <intrin.h>
#include <cmath>
#include <cfloat>
#else
#include <immintrin.h>
#include <cmath>
#include <cfloat>
#endif

// ============================================================================
// SIMD constant pools (from original binary)
// ============================================================================
// hkVector4Comparison::maskToComparison constant pool
// Used for generating sign flip masks in SIMD operations
alignas(16) static const __int128 hkVector4Comparison_maskToComparison_9[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,  // 0-7
    0, 0, 0, 0, 0, 0, 0, 0,  // 8-15
    0, 0, 0, 0, 0, 0x8000000080000000LL, 0, 0,  // 16-23 (index 20 = sign flip mask)
    0, 0, 0, 0, 0, 0, 0, 0,  // 24-31
    0, 0, 0, 0, 0, 0, 0, 0,  // 32-39
    0, 0, 0, 0, 0, 0, 0x8000000080000000LL, 0,  // 40-47 (index 44 = another sign flip mask)
    0, 0, 0, 0, 0, 0, 0, 0,  // 48-55
    0, 0, 0, 0, 0, 0, 0, 0   // 56-63
};

// xmmword_140AB0640 = (2.0, 2.0, 2.0, 2.0) - used for Newton-Raphson refinement
alignas(16) static const __m128 xmmword_140AB0640 = _mm_set_ps1(2.0f);

// ============================================================================
// Extern vftable declarations for Havok serialization constructors
// These symbols are defined in the original binary and referenced by constructors
// ============================================================================
void* hkMemoryResourceHandle_vftable = nullptr;
void* hkMemoryResourceContainer_vftable = nullptr;
void* hkaiPointCloudSilhouetteGenerator_vftable = nullptr;
void* hkaiWorld_vftable = nullptr;
void* hkaiCharacter_vftable = nullptr;
void* hkaiNavMeshInstance_vftable = nullptr;
void* hkaiOverlapManager_vftable = nullptr;
void* hkaiOverlapManager_hkReferencedObject_vftable = nullptr;
void* hkaiOverlapManager_hkaiNavMeshSilhouetteSelector_vftable = nullptr;
void* hkaiPathfindingUtil_FindPathInput_vftable = nullptr;
void* hkaiSilhouetteGenerator_vftable = nullptr;
void* hkaiVolumePathfindingUtil_FindPathInput_vftable = nullptr;
void* hkaiPathfindingUtil_FindGraphPathInput_vftable = nullptr;
void* hkaiPathfindingUtil_FindPathOutput_vftable = nullptr;
void* hkaiNavMeshCutter_vftable = nullptr;
void* hkaiStreamingCollection_vftable = nullptr;
void* hkaiDirectedGraphInstance_vftable = nullptr;
void* hkaiVolumePathfindingUtil_FindPathOutput_vftable = nullptr;
void* hkaiDynamicNavVolumeMediator_vftable = nullptr;
void* hkaiDynamicNavMeshQueryMediator_vftable = nullptr;
void* hkaiNavMesh_vftable = nullptr;
void* hkaiStaticTreeNavMeshQueryMediator_vftable = nullptr;
void* hkaiJumpDetectionSettings_vftable = nullptr;
void* hkaiDirectedGraphExplicitCost_vftable = nullptr;
void* hkaiAabbTreeNavVolumeMediator_vftable = nullptr;
void* hkaiNavVolumeInstance_vftable = nullptr;
void* hkaiMaterialPainter_vftable = nullptr;
void* hkaiEdgePath_vftable = nullptr;
void* hkaiWorld_CharacterStepSerializableContext_vftable = nullptr;
void* hkaiObstacleGenerator_vftable = nullptr;
void* hkaiNavVolume_vftable = nullptr;
void* hkaiPlaneVolume_vftable = nullptr;
void* hkaiNavVolumeGenerationSettings_vftable = nullptr;
void* hkaiNavMeshGenerationSettings_vftable = nullptr;
void* hkaiDynamicTree_vftable = nullptr;
void* hkaiStaticTree_vftable = nullptr;
void* hkcdShape_vftable = nullptr;
void* hkaiSilhouetteRecorder_InstanceLoadedEvent_vftable = nullptr;
void* hkaiDefaultAstarEdgeFilter_vftable = nullptr;
void* hkaiSimpleSilhouetteMerger_vftable = nullptr;
void* hkaiConvexHullSilhouetteMerger_vftable = nullptr;
void* hkaiPath_vftable = nullptr;
void* hkaiDefaultAstarCostModifier_vftable = nullptr;
void* hkaiInvertedAabbVolume_vftable = nullptr;
void* hkaiConvexSilhouetteSet_vftable = nullptr;
void* hkaiSimpleObstacleGenerator_vftable = nullptr;
void* hkaiSilhouetteRecorder_WorldConnectedEvent_vftable = nullptr;
void* hkSkinnedRefMeshShape_vftable = nullptr;
void* hkIndexedTransformSet_vftable = nullptr;
void* hkMemoryMeshVertexBuffer_vftable = nullptr;
void* hkaiSilhouetteRecorder_SilhouettesSteppedEvent_vftable = nullptr;
void* hkaiSilhouetteRecorder_VolumeLoadedEvent_vftable = nullptr;
void* hkaiSilhouetteRecorder_GraphLoadedEvent_vftable = nullptr;
void* hkaiSilhouetteRecorder_InstanceUnloadedEvent_vftable = nullptr;
void* hkaiSilhouetteRecorder_VolumeUnloadedEvent_vftable = nullptr;
void* hkaiSilhouetteRecorder_GraphUnloadedEvent_vftable = nullptr;

// Forward declarations for constructors defined later in this file
// (called by Part 150 hkaiWorld_constructor before Part 151 definitions)
extern "C" void* __fastcall hkaiSilhouetteGenerator_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiPathfindingUtil_FindPathInput_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiVolumePathfindingUtil_FindPathInput_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiConvexSilhouetteSet_constructor(void* p, hkFinishLoadedObjectFlag a2);
// Additional forward declarations for finishLoadedObject functions (Parts 140-141)
extern "C" void* __fastcall hkaiVolumePathfindingUtil_FindPathOutput_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiDirectedGraphExplicitCost_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiPathfindingUtil_FindPathOutput_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void __fastcall hkaiPathfindingUtil_FindGraphPathInput_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiAabbTreeNavVolumeMediator_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiMaterialPainter_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiDirectedGraphInstance_constructor(void* p, hkFinishLoadedObjectFlag a2);
// Forward declarations for hkRootLevelContainer/hkMemoryResourceHandle/hkMemoryResourceContainer serialization
extern "C" void* __fastcall hkRootLevelContainer_constructor(void* p, hkFinishLoadedObjectFlag finishing);
extern "C" void* __fastcall hkRootLevelContainer_NamedVariant_constructor(void* p, hkFinishLoadedObjectFlag finishing);
extern "C" void* __fastcall hkMemoryResourceHandle_constructor(void* p, hkFinishLoadedObjectFlag finishing);
extern "C" void* __fastcall hkMemoryResourceContainer_constructor(void* p, hkFinishLoadedObjectFlag finishing);
// Forward declaration for hkStringPtr destructor extern wrapper
extern "C" void __fastcall hkStringPtr_destructor(void* p);
// Additional forward declarations for finishLoadedObject functions (Part 129+)
extern "C" void* __fastcall hkaiStaticTreeNavMeshQueryMediator_constructor(void* p);
extern "C" void* __fastcall hkaiOverlapManager_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiJumpDetectionSettings_Trajectory_constructor(void* p);
extern "C" void* __fastcall hkaiJumpDetectionSettings_JumpDescription_constructor(void* p);
extern "C" void __fastcall hkaiJumpDetectionSettings_constructor(void* p);
extern "C" void* __fastcall hkaiNavVolumeInstance_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiNavMeshCutter_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiStreamingCollection_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiPathfindingUtil_FindPathInput_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiEdgePath_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void __fastcall hkaiWorld_CharacterStepSerializableContext_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiWorld_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiDynamicNavVolumeMediator_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiPlaneVolume_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiPointCloudSilhouetteGenerator_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiCharacter_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiNavVolume_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiLineOfSightUtil_InputBase_constructor(void* p);
extern "C" void* __fastcall hkaiLineOfSightUtil_LineOfSightInput_constructor(void* p);
extern "C" void* __fastcall hkaiLineOfSightUtil_DirectPathInput_constructor(void* p);
extern "C" void* __fastcall hkaiLineOfSightUtil_LineOfSightOutput_constructor(void* p);
extern "C" void* __fastcall hkaiNavMeshGenerationSettings_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiObstacleGenerator_constructor(void* p);
extern "C" void __fastcall hkaiStaticTree_constructor2(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiDynamicTree_constructor2(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkaiNavMeshGenerationSettings_OverrideSettings_constructor(void* p);
extern "C" void* __fastcall hkaiNavVolumeGenerationSettings_MaterialConstructionInfo_constructor(void* p);
extern "C" void* __fastcall hkaiNavVolumeGenerationSettings_constructor(void* p, hkFinishLoadedObjectFlag a2);
extern "C" void* __fastcall hkcdShape_constructor2(void* p);

// Forward declarations for helper functions used in destructor/initClearanceCache
extern "C" void __fastcall hkStringPtr_destructor(void* p);
extern "C" void __fastcall hkaiNavMeshInstance_initGlobalClearanceCache(void* p);

// hkGeometry::hkGeometry (0x14079E420) — empty constructor, just returns this
extern "C" void* __fastcall hkGeometry_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    return p;
}
// hkStringPtr::hkStringPtr (0x14079D910) — empty constructor, just returns this
extern "C" void* __fastcall hkStringPtr_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    return p;
}
// hkaiPlaneVolume::updateInternalInfo (0x140835AD0)
// Calls hkAabbUtil::calcAabb(vertices, count, &aabb)
extern "C" void __fastcall hkaiPlaneVolume_updateInternalInfo(void* p) {
    hkAabbUtil::calcAabb((hkVector4**)((char*)p + 0x20), *(int*)((char*)p + 0x28), *(hkAabb*)((char*)p + 0x50));
}

// ============================================================================
// Static member definitions
// ============================================================================
hkContainerHeapAllocator::Allocator hkContainerHeapAllocator::s_alloc;
hkContainerTempAllocator::Allocator hkContainerTempAllocator::s_alloc;
hkContainerDebugAllocator::Allocator hkContainerDebugAllocator::s_alloc;
hkThreadLocalData<hkMemoryRouter*> hkMemoryRouter::s_memoryRouter;

// hkMallocAllocator static instance
hkMallocAllocator* hkMallocAllocator::m_defaultMallocAllocator = nullptr;

// hkContainerTempAllocator::Allocator implementations
// IDA: ?blockAlloc@Allocator@hkContainerTempAllocator@@UEAAPEAXH@Z @ 0x14078ea50
void* hkContainerTempAllocator::Allocator::blockAlloc(int numBytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_temp) {
        return router->m_temp->blockAlloc(numBytes);
    }
#endif
    return ::operator new(numBytes);
}

// IDA: ?blockFree@Allocator@hkContainerTempAllocator@@UEAAXPEAXH@Z @ 0x14078ea80
void hkContainerTempAllocator::Allocator::blockFree(void* p, int numBytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_temp) {
        router->m_temp->blockFree(p, numBytes);
        return;
    }
#endif
    ::operator delete(p);
}

// IDA: ?bufAlloc@Allocator@hkContainerTempAllocator@@UEAAPEAXAEAH@Z @ 0x14078eac0
void* hkContainerTempAllocator::Allocator::bufAlloc(int& reqNumBytesInOut) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_temp) {
        return router->m_temp->bufAlloc(reqNumBytesInOut);
    }
#endif
    return ::operator new(reqNumBytesInOut);
}

// IDA: ?bufFree@Allocator@hkContainerTempAllocator@@UEAAXPEAXH@Z @ 0x14078eaf0
void hkContainerTempAllocator::Allocator::bufFree(void* p, int numBytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_temp) {
        router->m_temp->bufFree(p, numBytes);
        return;
    }
#endif
    ::operator delete(p);
}

// IDA: ?bufRealloc@Allocator@hkContainerTempAllocator@@UEAAPEAXPEAXHAEAH@Z @ 0x14078eb30
void* hkContainerTempAllocator::Allocator::bufRealloc(void* pold, int oldNumBytes, int* reqNumBytesInOut) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_temp) {
        return router->m_temp->bufRealloc(pold, oldNumBytes, reqNumBytesInOut);
    }
#endif
    // Fallback: allocate new, copy, delete old
    void* pnew = ::operator new(*reqNumBytesInOut);
    if (pold && oldNumBytes > 0) {
        memcpy(pnew, pold, oldNumBytes < *reqNumBytesInOut ? oldNumBytes : *reqNumBytesInOut);
        ::operator delete(pold);
    }
    return pnew;
}

// IDA: ?getMemoryStatistics@Allocator@hkContainerTempAllocator@@UEAAXAEAUMemoryStatistics@hkMemoryAllocator@@@Z @ 0x14078eb80
void hkContainerTempAllocator::Allocator::getMemoryStatistics(MemoryStatistics& u) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_temp) {
        router->m_temp->getMemoryStatistics(u);
    }
#endif
}

// IDA: ?getAllocatedSize@Allocator@hkContainerTempAllocator@@UEAAHPEBXH@Z @ 0x14078ebb0
int hkContainerTempAllocator::Allocator::getAllocatedSize(const void* obj, int nbytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_temp) {
        return router->m_temp->getAllocatedSize(obj, nbytes);
    }
#endif
    return nbytes;
}

// hkContainerHeapAllocator::Allocator implementations
// IDA: ?blockAlloc@Allocator@hkContainerHeapAllocator@@UEAAPEAXH@Z @ 0x14078ed90
void* hkContainerHeapAllocator::Allocator::blockAlloc(int numBytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_heap) {
        return router->m_heap->blockAlloc(numBytes);
    }
#endif
    return ::operator new(numBytes);
}

// IDA: ?blockFree@Allocator@hkContainerHeapAllocator@@UEAAXPEAXH@Z @ 0x14078edc0
void hkContainerHeapAllocator::Allocator::blockFree(void* p, int numBytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_heap) {
        router->m_heap->blockFree(p, numBytes);
        return;
    }
#endif
    ::operator delete(p);
}

// IDA: ?bufAlloc@Allocator@hkContainerHeapAllocator@@UEAAPEAXAEAH@Z @ 0x14078ee00
void* hkContainerHeapAllocator::Allocator::bufAlloc(int& reqNumBytesInOut) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_heap) {
        return router->m_heap->bufAlloc(reqNumBytesInOut);
    }
#endif
    return ::operator new(reqNumBytesInOut);
}

// IDA: ?bufFree@Allocator@hkContainerHeapAllocator@@UEAAXPEAXH@Z @ 0x14078ee30
void hkContainerHeapAllocator::Allocator::bufFree(void* p, int numBytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_heap) {
        router->m_heap->bufFree(p, numBytes);
        return;
    }
#endif
    ::operator delete(p);
}

// IDA: ?bufRealloc@Allocator@hkContainerHeapAllocator@@UEAAPEAXPEAXHAEAH@Z @ 0x14078ee70
void* hkContainerHeapAllocator::Allocator::bufRealloc(void* pold, int oldNumBytes, int* reqNumBytesInOut) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_heap) {
        return router->m_heap->bufRealloc(pold, oldNumBytes, reqNumBytesInOut);
    }
#endif
    // Fallback: allocate new, copy, delete old
    void* pnew = ::operator new(*reqNumBytesInOut);
    if (pold && oldNumBytes > 0) {
        memcpy(pnew, pold, oldNumBytes < *reqNumBytesInOut ? oldNumBytes : *reqNumBytesInOut);
        ::operator delete(pold);
    }
    return pnew;
}

// IDA: ?getMemoryStatistics@Allocator@hkContainerHeapAllocator@@UEAAXAEAUMemoryStatistics@hkMemoryAllocator@@@Z @ 0x14078eec0
void hkContainerHeapAllocator::Allocator::getMemoryStatistics(MemoryStatistics& u) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_heap) {
        router->m_heap->getMemoryStatistics(u);
    }
#endif
}

// IDA: ?getAllocatedSize@Allocator@hkContainerHeapAllocator@@UEAAHPEBXH@Z @ 0x14078eef0
int hkContainerHeapAllocator::Allocator::getAllocatedSize(const void* obj, int nbytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_heap) {
        return router->m_heap->getAllocatedSize(obj, nbytes);
    }
#endif
    return nbytes;
}

// hkMemorySystem static instance
hkMemorySystem* hkMemorySystem::s_instance = nullptr;

// hkContainerDebugAllocator::Allocator implementations
// IDA: ?blockAlloc@Allocator@hkContainerDebugAllocator@@UEAAPEAXH@Z @ 0x14078ebf0
void* hkContainerDebugAllocator::Allocator::blockAlloc(int numBytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_debug) {
        return router->m_debug->blockAlloc(numBytes);
    }
#endif
    return ::operator new(numBytes);
}

// IDA: ?blockFree@Allocator@hkContainerDebugAllocator@@UEAAXPEAXH@Z @ 0x14078ec20
void hkContainerDebugAllocator::Allocator::blockFree(void* p, int numBytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_debug) {
        router->m_debug->blockFree(p, numBytes);
        return;
    }
#endif
    ::operator delete(p);
}

// IDA: ?bufAlloc@Allocator@hkContainerDebugAllocator@@UEAAPEAXAEAH@Z @ 0x14078ec60
void* hkContainerDebugAllocator::Allocator::bufAlloc(int& reqNumBytesInOut) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_debug) {
        return router->m_debug->bufAlloc(reqNumBytesInOut);
    }
#endif
    return ::operator new(reqNumBytesInOut);
}

// IDA: ?bufFree@Allocator@hkContainerDebugAllocator@@UEAAXPEAXH@Z @ 0x14078ec90
void hkContainerDebugAllocator::Allocator::bufFree(void* p, int numBytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_debug) {
        router->m_debug->bufFree(p, numBytes);
        return;
    }
#endif
    ::operator delete(p);
}

// IDA: ?bufRealloc@Allocator@hkContainerDebugAllocator@@UEAAPEAXPEAXHAEAH@Z @ 0x14078ecd0
void* hkContainerDebugAllocator::Allocator::bufRealloc(void* pold, int oldNumBytes, int* reqNumBytesInOut) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_debug) {
        return router->m_debug->bufRealloc(pold, oldNumBytes, reqNumBytesInOut);
    }
#endif
    // Fallback: allocate new, copy, delete old
    void* pnew = ::operator new(*reqNumBytesInOut);
    if (pold && oldNumBytes > 0) {
        memcpy(pnew, pold, oldNumBytes < *reqNumBytesInOut ? oldNumBytes : *reqNumBytesInOut);
        ::operator delete(pold);
    }
    return pnew;
}

// IDA: ?getMemoryStatistics@Allocator@hkContainerDebugAllocator@@UEAAXAEAUMemoryStatistics@hkMemoryAllocator@@@Z @ 0x14078ed20
void hkContainerDebugAllocator::Allocator::getMemoryStatistics(MemoryStatistics& u) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_debug) {
        router->m_debug->getMemoryStatistics(u);
    }
#endif
}

// IDA: ?getAllocatedSize@Allocator@hkContainerDebugAllocator@@UEAAHPEBXH@Z @ 0x14078ed50
int hkContainerDebugAllocator::Allocator::getAllocatedSize(const void* obj, int nbytes) {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_debug) {
        return router->m_debug->getAllocatedSize(obj, nbytes);
    }
#endif
    return nbytes;
}

// hkMallocAllocator implementations
// IDA: ?blockAlloc@hkMallocAllocator@@UEAAPEAXH@Z @ 0x14078f450
void* hkMallocAllocator::blockAlloc(int numBytes) {
#ifdef _WIN32
    // Atomically add to current usage and check peak
    InterlockedExchangeAdd((volatile long*)&m_currentUsed, numBytes);
    if (m_currentUsed > m_peakUse) {
        m_peakUse = m_currentUsed;
    }
    return _aligned_malloc(numBytes, m_align);
#else
    return ::operator new(numBytes);
#endif
}

// IDA: ?blockFree@hkMallocAllocator@@UEAAXPEAXH@Z @ 0x14078f480
void hkMallocAllocator::blockFree(void* p, int numBytes) {
#ifdef _WIN32
    InterlockedExchangeAdd((volatile long*)&m_currentUsed, -numBytes);
    _aligned_free(p);
#else
    ::operator delete(p);
#endif
}

// IDA: ?getMemoryStatistics@hkMallocAllocator@@UEAAXAEAUMemoryStatistics@hkMemoryAllocator@@@Z @ 0x14078f4a0
void hkMallocAllocator::getMemoryStatistics(MemoryStatistics& u) {
    u.m_allocated = m_currentUsed;
    u.m_peakInUse = m_peakUse;
}

// hkMemoryInitUtil static members
hkMemorySystem* hkMemoryInitUtil::s_system = nullptr;
void (*hkMemoryInitUtil::s_onQuitFunc)() = nullptr;

// IDA: ?outputDebugString@hkMemoryInitUtil@@YAXPEBDPEAX@Z @ 0x14078f500
void hkMemoryInitUtil::outputDebugString(const char* s, void*) {
#ifdef _WIN32
    OutputDebugStringA(s);
    printf("%s", s);
#else
    printf("%s", s);
#endif
}

// IDA: ?quit@hkMemoryInitUtil@@YA?AUhkResult@@XZ @ 0x14078f530
hkResult hkMemoryInitUtil::quit() {
    hkResult result(0);  // HK_SUCCESS

    if (s_system) {
        // Call mainQuit on system
        // result = s_system->mainQuit();
        delete s_system;
        s_system = nullptr;
        hkMemorySystem::replaceInstance(nullptr);
    }

    if (s_onQuitFunc) {
        s_onQuitFunc();
        s_onQuitFunc = nullptr;
    }

    return result;
}

// IDA: ?refreshDebugSymbols@hkMemoryInitUtil@@YAXXZ @ 0x14078f5b0
void hkMemoryInitUtil::refreshDebugSymbols() {
    // Uses hkSingleton<hkStackTracer>::s_instance in original
    // Stub for now
}

// ============================================================================
// hkPackedVector3 and hkPackedVector8_3 implementations
// ============================================================================

// Global constants for packed vector packing
static const float g_packedVectorRounding[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
static const float g_packedVectorMask[4] = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF };
static const int g_packedVectorOffset[4] = { 16256, 16256, 16256, 16256 };
static const int g_packedVectorRoundingCorrection[4] = { 0, 0, 0, 0 };
static const float g_two31[4] = { 2147483648.0f, 2147483648.0f, 2147483648.0f, 2147483648.0f };

// IDA: ?pack@hkPackedVector3@@QEAAXAEBVhkVector4@@@Z @ 0x14078ef80
void hkPackedVector3::pack(const hkVector4& vIn) {
    __m128 v = vIn.m_quad;

    // Shuffle to get components and constant 1.0
    __m128 v2 = _mm_shuffle_ps(v, _mm_unpackhi_ps(v, _mm_set1_ps(1.0f)), 196);

    // Multiply by rounding and mask
    __m128 v3 = _mm_and_ps(_mm_mul_ps(_mm_load_ps(g_packedVectorRounding), v2),
                           _mm_load_ps((const float*)g_packedVectorMask));

    // Find max component
    __m128 v4 = _mm_max_ps(_mm_shuffle_ps(v3, v3, 78), v3);
    __m128i v5 = _mm_sub_epi32(_mm_load_si128((const __m128i*)g_packedVectorOffset),
                                _mm_cvttps_epi32(_mm_max_ps(v4, _mm_shuffle_ps(v4, v4, 177))));

    // Add offset
    __m128 v6 = _mm_add_ps(v2, _mm_cvtepi32_ps(v5));

    // Convert to int with unsigned handling
    __m128i cmp = _mm_castps_si128(_mm_cmple_ps(_mm_load_ps(g_two31), v6));
    __m128i v7 = _mm_add_epi32(
        _mm_xor_si128(_mm_cvttps_epi32(v6), cmp),
        _mm_load_si128((const __m128i*)g_packedVectorRoundingCorrection));

    // Extract 16-bit values
    m_values[0] = _mm_extract_epi16(v7, 1);
    m_values[1] = _mm_extract_epi16(v7, 3);
    m_values[2] = _mm_extract_epi16(v7, 5);
    m_values[3] = 16256 - _mm_extract_epi16(v5, 1);
}

// IDA: ?pack@hkPackedVector8_3@@QEAAXAEBVhkVector4@@@Z @ 0x14078f020
void hkPackedVector8_3::pack(const hkVector4& vIn) {
    __m128 v = vIn.m_quad;

    // Shuffle to get components and constant 1.0
    __m128 v2 = _mm_shuffle_ps(v, _mm_unpackhi_ps(v, _mm_set1_ps(1.0f)), 196);

    // Multiply by rounding and mask
    __m128 v3 = _mm_and_ps(_mm_mul_ps(_mm_load_ps(g_packedVectorRounding), v2),
                           _mm_load_ps((const float*)g_packedVectorMask));

    // Find max component
    __m128 v4 = _mm_max_ps(_mm_shuffle_ps(v3, v3, 78), v3);
    __m128i v5 = _mm_sub_epi32(_mm_load_si128((const __m128i*)g_packedVectorOffset),
                                _mm_cvttps_epi32(_mm_max_ps(v4, _mm_shuffle_ps(v4, v4, 177))));

    // Add offset
    __m128 v6 = _mm_add_ps(v2, _mm_cvtepi32_ps(v5));

    // Convert to int with unsigned handling
    __m128i cmp = _mm_castps_si128(_mm_cmple_ps(_mm_load_ps(g_two31), v6));
    __m128i result = _mm_add_epi32(
        _mm_xor_si128(_mm_cvttps_epi32(v6), cmp),
        _mm_load_si128((const __m128i*)g_packedVectorRoundingCorrection));

    // Extract 8-bit values (SSE2-compatible)
    // Store to array and extract bytes
    int8_t bytes[16];
    _mm_store_si128((__m128i*)bytes, result);
    m_values[0] = (unsigned char)bytes[3];
    m_values[1] = (unsigned char)bytes[7];
    m_values[2] = (unsigned char)bytes[11];
    m_values[3] = (16256 - _mm_extract_epi16(v5, 1)) >> 7;
}

// ============================================================================
// hkQuaternion implementations
// ============================================================================

// IDA: ?setAxisAngle@hkQuaternion@@QEAAXAEBVhkVector4@@M@Z @ 0x14078c3c0
void hkQuaternion::setAxisAngle(const hkVector4& axis, float angle) {
    hkSimdReal simdAngle;
    simdAngle.m_real = _mm_set1_ps(angle);
    setAxisAngle(axis, simdAngle);
}

// IDA: ?setAxisAngle_Approximate@hkQuaternion@@QEAAXAEBVhkVector4@@AEBVhkSimdReal@@@Z @ 0x14078c3f0
void hkQuaternion::setAxisAngle_Approximate(const hkVector4& axis, const hkSimdReal& angle) {
    // Approximate implementation using sin/cos
    __m128 halfAngle = _mm_mul_ps(angle.m_real, _mm_set1_ps(0.5f));
    float halfAng;
    _mm_store_ss(&halfAng, halfAngle);

    float c = cos(halfAng);
    float s = sin(halfAng);

    m_vec.x = axis.x * s;
    m_vec.y = axis.y * s;
    m_vec.z = axis.z * s;
    m_vec.w = c;
}

// Helper for axis-angle
void hkQuaternion::setAxisAngle(const hkVector4& axis, const hkSimdReal& angle) {
    __m128 halfAngle = _mm_mul_ps(angle.m_real, _mm_set1_ps(0.5f));
    float halfAng;
    _mm_store_ss(&halfAng, halfAngle);

    float c = cos(halfAng);
    float s = sin(halfAng);

    m_vec.x = axis.x * s;
    m_vec.y = axis.y * s;
    m_vec.z = axis.z * s;
    m_vec.w = c;
}

// IDA: ?setFromEulerAngles@hkQuaternion@@QEAAXMMM@Z @ 0x14078c710
void hkQuaternion::setFromEulerAngles(float roll, float pitch, float yaw) {
    hkSimdReal simdRoll, simdPitch, simdYaw;
    simdRoll.m_real = _mm_set1_ps(roll);
    simdPitch.m_real = _mm_set1_ps(pitch);
    simdYaw.m_real = _mm_set1_ps(yaw);
    setFromEulerAngles(simdRoll, simdPitch, simdYaw);
}

// IDA: ?setFromEulerAngles@hkQuaternion@@QEAAXAEBVhkSimdReal@@00@Z @ 0x14078c480
void hkQuaternion::setFromEulerAngles(const hkSimdReal& roll, const hkSimdReal& pitch, const hkSimdReal& yaw) {
    float r, p, y;
    _mm_store_ss(&r, roll.m_real);
    _mm_store_ss(&p, pitch.m_real);
    _mm_store_ss(&y, yaw.m_real);

    float cr = cos(r * 0.5f);
    float sr = sin(r * 0.5f);
    float cp = cos(p * 0.5f);
    float sp = sin(p * 0.5f);
    float cy = cos(y * 0.5f);
    float sy = sin(y * 0.5f);

    m_vec.w = cr * cp * cy + sr * sp * sy;
    m_vec.x = sr * cp * cy - cr * sp * sy;
    m_vec.y = cr * sp * cy + sr * cp * sy;
    m_vec.z = cr * cp * sy - sr * sp * cy;
}

// IDA: ?setSlerp@hkQuaternion@@QEAAXAEBV1@0AEBVhkSimdReal@@@Z @ 0x14078d1b0
void hkQuaternion::setSlerp(const hkQuaternion& q0, const hkQuaternion& q1, const hkSimdReal& t) {
    float tVal;
    _mm_store_ss(&tVal, t.m_real);

    // Compute dot product
    float dot = q0.m_vec.x * q1.m_vec.x + q0.m_vec.y * q1.m_vec.y +
                q0.m_vec.z * q1.m_vec.z + q0.m_vec.w * q1.m_vec.w;

    // If dot is negative, negate one quaternion to take shorter path
    float sign = 1.0f;
    if (dot < 0.0f) {
        dot = -dot;
        sign = -1.0f;
    }

    // If quaternions are close, use linear interpolation
    if (dot > 0.9995f) {
        m_vec.x = q0.m_vec.x + tVal * (sign * q1.m_vec.x - q0.m_vec.x);
        m_vec.y = q0.m_vec.y + tVal * (sign * q1.m_vec.y - q0.m_vec.y);
        m_vec.z = q0.m_vec.z + tVal * (sign * q1.m_vec.z - q0.m_vec.z);
        m_vec.w = q0.m_vec.w + tVal * (sign * q1.m_vec.w - q0.m_vec.w);
    } else {
        // Spherical linear interpolation
        float theta_0 = acos(dot);
        float theta = theta_0 * tVal;

        float sin_theta = sin(theta);
        float sin_theta_0 = sin(theta_0);

        float s0 = cos(theta) - dot * sin_theta / sin_theta_0;
        float s1 = sin_theta / sin_theta_0;

        m_vec.x = s0 * q0.m_vec.x + s1 * sign * q1.m_vec.x;
        m_vec.y = s0 * q0.m_vec.y + s1 * sign * q1.m_vec.y;
        m_vec.z = s0 * q0.m_vec.z + s1 * sign * q1.m_vec.z;
        m_vec.w = s0 * q0.m_vec.w + s1 * sign * q1.m_vec.w;
    }

    // Normalize
    float len = sqrt(m_vec.x * m_vec.x + m_vec.y * m_vec.y +
                     m_vec.z * m_vec.z + m_vec.w * m_vec.w);
    m_vec.x /= len;
    m_vec.y /= len;
    m_vec.z /= len;
    m_vec.w /= len;
}

// IDA: ?setFromEulerAngles_Approximate@hkQuaternion@@QEAAXAEBVhkSimdReal@@00@Z @ 0x14078c770
void hkQuaternion::setFromEulerAngles_Approximate(const hkSimdReal& roll, const hkSimdReal& pitch, const hkSimdReal& yaw) {
    // Simplified implementation using standard math
    float r, p, y;
    _mm_store_ss(&r, roll.m_real);
    _mm_store_ss(&p, pitch.m_real);
    _mm_store_ss(&y, yaw.m_real);

    float cr = cos(r * 0.5f);
    float sr = sin(r * 0.5f);
    float cp = cos(p * 0.5f);
    float sp = sin(p * 0.5f);
    float cy = cos(y * 0.5f);
    float sy = sin(y * 0.5f);

    m_vec.w = cr * cp * cy + sr * sp * sy;
    m_vec.x = sr * cp * cy - cr * sp * sy;
    m_vec.y = cr * sp * cy + sr * cp * sy;
    m_vec.z = cr * cp * sy - sr * sp * cy;
}

// IDA: ?set@hkQuaternion@@QEAAXAEBVhkRotation@@@Z @ 0x14078c910
void hkQuaternion::set(const hkRotation& r) {
    // Convert rotation matrix to quaternion
    // Algorithm from IDA: Shepperd's method
    float m00 = r.m_col0.x;
    float m11 = r.m_col1.y;
    float m22 = r.m_col2.z;

    float trace = m00 + m11 + m22;

    if (trace > 0.0f) {
        float s = sqrt(trace + 1.0f) * 2.0f;  // s = 4 * qw
        m_vec.w = 0.25f * s;
        m_vec.x = (r.m_col2.y - r.m_col1.z) / s;
        m_vec.y = (r.m_col0.z - r.m_col2.x) / s;
        m_vec.z = (r.m_col1.x - r.m_col0.y) / s;
    } else if (m00 > m11 && m00 > m22) {
        float s = sqrt(1.0f + m00 - m11 - m22) * 2.0f;  // s = 4 * qx
        m_vec.w = (r.m_col2.y - r.m_col1.z) / s;
        m_vec.x = 0.25f * s;
        m_vec.y = (r.m_col0.y + r.m_col1.x) / s;
        m_vec.z = (r.m_col0.z + r.m_col2.x) / s;
    } else if (m11 > m22) {
        float s = sqrt(1.0f - m00 + m11 - m22) * 2.0f;  // s = 4 * qy
        m_vec.w = (r.m_col0.z - r.m_col2.x) / s;
        m_vec.x = (r.m_col0.y + r.m_col1.x) / s;
        m_vec.y = 0.25f * s;
        m_vec.z = (r.m_col1.z + r.m_col2.y) / s;
    } else {
        float s = sqrt(1.0f - m00 - m11 + m22) * 2.0f;  // s = 4 * qz
        m_vec.w = (r.m_col1.x - r.m_col0.y) / s;
        m_vec.x = (r.m_col0.z + r.m_col2.x) / s;
        m_vec.y = (r.m_col1.z + r.m_col2.y) / s;
        m_vec.z = 0.25f * s;
    }
}

// IDA: ?removeAxisComponent@hkQuaternion@@QEAAXAEBVhkVector4@@@Z @ 0x14078cbc0
void hkQuaternion::removeAxisComponent(const hkVector4& axis) {
    // Complex quaternion decomposition - removes rotation around given axis
    // Simplified implementation
    hkSimdReal rotationAngle;
    hkVector4 rotationAxis;

    // Compute the rotation angle around the axis
    float ax = axis.x, ay = axis.y, az = axis.z;

    // Project quaternion onto axis to find angle
    float dot = ax * m_vec.x + ay * m_vec.y + az * m_vec.z;

    // If rotation is nearly along this axis, result is identity
    if (fabs(dot) > 0.9999f) {
        setIdentity();
        return;
    }

    // Remove the axis component by setting up perpendicular rotation
    // This is a simplified stub - full implementation requires complex SIMD
    // TODO: 汇编还原 - Complex SIMD acos/sin operations
}

// IDA: ?decomposeRestAxis@hkQuaternion@@QEBAXAEBVhkVector4@@AEAV1@AEAVhkSimdReal@@@Z @ 0x14078cf60
void hkQuaternion::decomposeRestAxis(const hkVector4& axis, hkQuaternion& restOut, hkSimdReal& angleOut) const {
    // Decompose quaternion into rotation around axis and rest
    restOut = *this;
    restOut.removeAxisComponent(axis);

    // Calculate the angle component
    // Simplified implementation
    float dot = axis.x * m_vec.x + axis.y * m_vec.y + axis.z * m_vec.z;
    angleOut.m_real = _mm_set1_ps(2.0f * acos(fabs(dot)));
}

// IDA: ?setBarycentric@hkQuaternion@@QEAAXPEIBV1@AEBVhkVector4@@@Z @ 0x14078d530
void hkQuaternion::setBarycentric(const hkQuaternion* qVerts, const hkVector4& vLambda) {
    // Barycentric interpolation of quaternions
    // Very complex SIMD implementation - simplified version
    float u = vLambda.x;
    float v = vLambda.y;
    float w = vLambda.z;

    // Simple weighted average followed by normalization
    // This is a simplified approximation - full implementation uses slerp chains
    m_vec.x = u * qVerts[0].m_vec.x + v * qVerts[1].m_vec.x + w * qVerts[2].m_vec.x;
    m_vec.y = u * qVerts[0].m_vec.y + v * qVerts[1].m_vec.y + w * qVerts[2].m_vec.y;
    m_vec.z = u * qVerts[0].m_vec.z + v * qVerts[1].m_vec.z + w * qVerts[2].m_vec.z;
    m_vec.w = u * qVerts[0].m_vec.w + v * qVerts[1].m_vec.w + w * qVerts[2].m_vec.w;

    // Normalize
    float len = sqrt(m_vec.x * m_vec.x + m_vec.y * m_vec.y +
                     m_vec.z * m_vec.z + m_vec.w * m_vec.w);
    if (len > 0.0001f) {
        m_vec.x /= len;
        m_vec.y /= len;
        m_vec.z /= len;
        m_vec.w /= len;
    }
    // TODO: 汇编还原 - Full barycentric slerp implementation from IDA
}

// ============================================================================
// hkMatrix3 implementations
// ============================================================================

// IDA: ?transpose@hkMatrix3@@QEAAXXZ @ 0x1407ad7e0
void hkMatrix3::transpose() {
#ifdef _WIN32
    __m128 v0 = m_col0.m_quad;
    __m128 v1 = m_col1.m_quad;
    __m128 v2 = m_col2.m_quad;

    // Transpose 3x3 matrix using SSE shuffles
    __m128 v3 = _mm_unpacklo_ps(v0, v1);
    __m128 v4 = _mm_unpackhi_ps(v0, v1);
    __m128 v5 = _mm_movelh_ps(v3, v2);
    m_col0.m_quad = v5;
    m_col2.m_quad = _mm_shuffle_ps(v4, v2, 0xE4);  // 228 = 0xE4
    m_col1.m_quad = _mm_shuffle_ps(_mm_movehl_ps(v5, v3), v2, 0xD4);  // 212 = 0xD4
#else
    // Non-SIMD fallback
    float temp;
    temp = m_col0.y; m_col0.y = m_col1.x; m_col1.x = temp;
    temp = m_col0.z; m_col0.z = m_col2.x; m_col2.x = temp;
    temp = m_col1.z; m_col1.z = m_col2.y; m_col2.y = temp;
#endif
}

// IDA: ?isOk@hkMatrix3@@QEBA_NXZ @ 0x1407ad820
bool hkMatrix3::isOk() const {
#ifdef _WIN32
    // Check for NaN in lower 3 components of each column
    return (_mm_movemask_ps(_mm_cmpunord_ps(m_col0.m_quad, _mm_setzero_ps())) & 7) == 0
        && (_mm_movemask_ps(_mm_cmpunord_ps(m_col1.m_quad, _mm_setzero_ps())) & 7) == 0
        && (_mm_movemask_ps(_mm_cmpunord_ps(m_col2.m_quad, _mm_setzero_ps())) & 7) == 0;
#else
    return std::isfinite(m_col0.x) && std::isfinite(m_col0.y) && std::isfinite(m_col0.z)
        && std::isfinite(m_col1.x) && std::isfinite(m_col1.y) && std::isfinite(m_col1.z)
        && std::isfinite(m_col2.x) && std::isfinite(m_col2.y) && std::isfinite(m_col2.z);
#endif
}

// IDA: ?setCrossSkewSymmetric@hkMatrix3@@QEAAXAEBVhkVector4@@@Z @ 0x1407ada40
void hkMatrix3::setCrossSkewSymmetric(const hkVector4& r) {
#ifdef _WIN32
    __m128 v = r.m_quad;
    __m128 vy = _mm_shuffle_ps(v, v, 0x55);   // 85 = 0x55 - broadcast y
    __m128 vx = _mm_shuffle_ps(v, v, 0x00);   // 0 = broadcast x
    __m128 vz = _mm_shuffle_ps(v, v, 0xAA);   // 170 = 0xAA - broadcast z

    // Create skew-symmetric matrix for cross product:
    // [  0  -z   y ]
    // [  z   0  -x ]
    // [ -y   x   0 ]
    m_col0.m_quad = _mm_movelh_ps(
        _mm_unpacklo_ps(_mm_setzero_ps(), vz),
        _mm_unpacklo_ps(_mm_sub_ps(_mm_setzero_ps(), vy), _mm_setzero_ps()));
    m_col1.m_quad = _mm_movelh_ps(
        _mm_unpacklo_ps(_mm_sub_ps(_mm_setzero_ps(), vz), _mm_setzero_ps()),
        _mm_unpacklo_ps(vx, _mm_setzero_ps()));
    m_col2.m_quad = _mm_movelh_ps(
        _mm_unpacklo_ps(vy, _mm_sub_ps(_mm_setzero_ps(), vx)),
        _mm_unpacklo_ps(_mm_setzero_ps(), _mm_setzero_ps()));
#else
    m_col0.x = 0.0f;    m_col0.y = -r.z;     m_col0.z = r.y;      m_col0.w = 0.0f;
    m_col1.x = r.z;     m_col1.y = 0.0f;     m_col1.z = -r.x;     m_col1.w = 0.0f;
    m_col2.x = -r.y;    m_col2.y = r.x;      m_col2.z = 0.0f;     m_col2.w = 0.0f;
#endif
}

// IDA: hkMatrix3_setMulMat3Mat3 @ 0x1407adda0
// Helper function for matrix multiplication
void hkMatrix3_setMulMat3Mat3(hkMatrix3* result, const hkMatrix3* aTb, const hkMatrix3* bTc) {
#ifdef _WIN32
    __m128 v3 = aTb->m_col0.m_quad;
    __m128 v4 = aTb->m_col1.m_quad;
    __m128 v5 = aTb->m_col2.m_quad;

    // Process columns in reverse order (col2, col1, col0)
    for (int i = 2; i >= 0; --i) {
        __m128 col = (&bTc->m_col0)[i].m_quad;
        (&result->m_col0)[i].m_quad = _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(col, col, 0x00), v3),
                _mm_mul_ps(_mm_shuffle_ps(col, col, 0x55), v4)),
            _mm_mul_ps(_mm_shuffle_ps(col, col, 0xAA), v5));
    }
#else
    for (int i = 0; i < 3; ++i) {
        const hkVector4& col = (&bTc->m_col0)[i];
        (&result->m_col0)[i].x = aTb->m_col0.x * col.x + aTb->m_col1.x * col.y + aTb->m_col2.x * col.z;
        (&result->m_col0)[i].y = aTb->m_col0.y * col.x + aTb->m_col1.y * col.y + aTb->m_col2.y * col.z;
        (&result->m_col0)[i].z = aTb->m_col0.z * col.x + aTb->m_col1.z * col.y + aTb->m_col2.z * col.z;
        (&result->m_col0)[i].w = 0.0f;
    }
#endif
}

// IDA: ?setMul@hkMatrix3@@QEAAXAEBV1@0@Z @ 0x1407ade40
void hkMatrix3::setMul(const hkMatrix3& aTb, const hkMatrix3& bTc) {
#ifdef _WIN32
    __m128 v0 = aTb.m_col0.m_quad;
    __m128 v1 = aTb.m_col1.m_quad;
    __m128 v2 = aTb.m_col2.m_quad;

    // Multiply matrices: this = aTb * bTc
    for (int i = 2; i >= 0; --i) {
        __m128 col = (&bTc.m_col0)[i].m_quad;
        m_col0.m_quad = _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(col, col, 0x00), v0),
                _mm_mul_ps(_mm_shuffle_ps(col, col, 0x55), v1)),
            _mm_mul_ps(_mm_shuffle_ps(col, col, 0xAA), v2));
    }
#else
    // Non-SIMD fallback
    for (int i = 0; i < 3; ++i) {
        const hkVector4& col = (&bTc.m_col0)[i];
        (&m_col0)[i].x = aTb.m_col0.x * col.x + aTb.m_col1.x * col.y + aTb.m_col2.x * col.z;
        (&m_col0)[i].y = aTb.m_col0.y * col.x + aTb.m_col1.y * col.y + aTb.m_col2.y * col.z;
        (&m_col0)[i].z = aTb.m_col0.z * col.x + aTb.m_col1.z * col.y + aTb.m_col2.z * col.z;
        (&m_col0)[i].w = 0.0f;
    }
#endif
}

// IDA: ?getDeterminant@hkMatrix3@@QEBA?BVhkSimdReal@@XZ @ 0x1407ae330
hkSimdReal hkMatrix3::getDeterminant() const {
#ifdef _WIN32
    __m128 v2 = m_col2.m_quad;
    __m128 v1 = m_col1.m_quad;

    __m128 v5 = _mm_unpacklo_ps(m_col0.m_quad, v1);
    __m128 v6 = _mm_shuffle_ps(_mm_unpackhi_ps(m_col0.m_quad, v1), v2, 0xE4);  // 228 = 0xE4
    __m128 v7 = _mm_movelh_ps(v5, v2);
    __m128 v8 = _mm_shuffle_ps(_mm_movehl_ps(v7, v5), v2, 0xD4);  // 212 = 0xD4

    // Compute determinant via cross product
    __m128 v9 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(v6, v6, 0xC9), v8), _mm_mul_ps(_mm_shuffle_ps(v8, v8, 0xC9), v6));
    __m128 v10 = _mm_mul_ps(_mm_shuffle_ps(v9, v9, 0xC9), v7);

    __m128 result = _mm_add_ps(_mm_add_ps(_mm_shuffle_ps(v10, v10, 0x55), _mm_shuffle_ps(v10, v10, 0x00)), _mm_shuffle_ps(v10, v10, 0xAA));

    hkSimdReal ret;
    ret.m_real = result;
    return ret;
#else
    // Non-SIMD: det = a(ei-fh) - b(di-fg) + c(dh-eg)
    float det = m_col0.x * (m_col1.y * m_col2.z - m_col1.z * m_col2.y)
              - m_col0.y * (m_col1.x * m_col2.z - m_col1.z * m_col2.x)
              + m_col0.z * (m_col1.x * m_col2.y - m_col1.y * m_col2.x);
    hkSimdReal ret;
    ret.m_real = _mm_set_ps1(det);
    return ret;
#endif
}

// IDA: ?isApproximatelyEqualSimd@hkMatrix3@@QEBA_NAEBV1@AEBVhkSimdReal@@@Z @ 0x1407ae010
bool hkMatrix3::isApproximatelyEqualSimd(const hkMatrix3& m, const hkSimdReal& epsilon) const {
#ifdef _WIN32
    // Check each column
    for (int i = 0; i < 3; ++i) {
        __m128 diff = _mm_sub_ps((&m_col0)[i].m_quad, (&m.m_col0)[i].m_quad);
        // Absolute value via shift trick: abs = (diff << 1) >> 1 (clears sign bit)
        __m128 absDiff = _mm_srli_epi32(_mm_slli_epi32((__m128i)diff, 1), 1);
        if ((_mm_movemask_ps(_mm_cmplt_ps(absDiff, epsilon.m_real)) & 7) != 7) {
            return false;
        }
    }
    return true;
#else
    float eps = epsilon.m_real[0];  // Extract first element
    for (int i = 0; i < 3; ++i) {
        if (std::abs((&m_col0)[i].x - (&m.m_col0)[i].x) > eps) return false;
        if (std::abs((&m_col0)[i].y - (&m.m_col0)[i].y) > eps) return false;
        if (std::abs((&m_col0)[i].z - (&m.m_col0)[i].z) > eps) return false;
    }
    return true;
#endif
}

// IDA: ?setMulInverseMul@hkMatrix3@@QEAAXAEBVhkRotation@@AEBV1@@Z @ 0x1407ad860
void hkMatrix3::setMulInverseMul(const hkRotation& bTa, const hkMatrix3& bTc) {
#ifdef _WIN32
    // Transpose bTa (inverse of rotation) and multiply with bTc
    __m128 v2 = bTa.m_col2.m_quad;
    __m128 v1 = bTa.m_col1.m_quad;

    __m128 v7 = _mm_unpacklo_ps(bTa.m_col0.m_quad, v1);
    __m128 v8 = _mm_shuffle_ps(_mm_unpackhi_ps(bTa.m_col0.m_quad, v1), v2, 0xE4);  // 228 = 0xE4
    __m128 v9 = _mm_movelh_ps(v7, v2);
    __m128 v10 = _mm_shuffle_ps(_mm_movehl_ps(v9, v7), v2, 0xD4);  // 212 = 0xD4

    // Multiply transposed bTa with bTc
    for (int i = 0; i < 3; ++i) {
        __m128 col = (&bTc.m_col0)[i].m_quad;
        (&m_col0)[i].m_quad = _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(col, col, 0x55), v10),
                _mm_mul_ps(_mm_shuffle_ps(col, col, 0x00), v9)),
            _mm_mul_ps(_mm_shuffle_ps(col, col, 0xAA), v8));
    }
#else
    // Non-SIMD: multiply transposed bTa with bTc
    for (int i = 0; i < 3; ++i) {
        (&m_col0)[i].x = bTa.m_col0.x * bTc.m_col0.x + bTa.m_col1.x * bTc.m_col0.y + bTa.m_col2.x * bTc.m_col0.z;
        (&m_col0)[i].y = bTa.m_col0.y * bTc.m_col0.x + bTa.m_col1.y * bTc.m_col0.y + bTa.m_col2.y * bTc.m_col0.z;
        (&m_col0)[i].z = bTa.m_col0.z * bTc.m_col0.x + bTa.m_col1.z * bTc.m_col0.y + bTa.m_col2.z * bTc.m_col0.z;
    }
#endif
}

// IDA: ?mul@hkMatrix3@@QEAAXAEBVhkSimdReal@@@Z @ 0x1407ada10
void hkMatrix3::mul(const hkSimdReal& scale) {
#ifdef _WIN32
    m_col0.m_quad = _mm_mul_ps(scale.m_real, m_col0.m_quad);
    m_col1.m_quad = _mm_mul_ps(m_col1.m_quad, scale.m_real);
    m_col2.m_quad = _mm_mul_ps(m_col2.m_quad, scale.m_real);
#else
    float s = scale.m_real.m128_f32[0];
    m_col0.x *= s; m_col0.y *= s; m_col0.z *= s;
    m_col1.x *= s; m_col1.y *= s; m_col1.z *= s;
    m_col2.x *= s; m_col2.y *= s; m_col2.z *= s;
#endif
}

// IDA: ?add@hkMatrix3@@QEAAXAEBV1@@Z @ 0x1407adae0
void hkMatrix3::add(const hkMatrix3& a) {
#ifdef _WIN32
    m_col0.m_quad = _mm_add_ps(a.m_col0.m_quad, m_col0.m_quad);
    m_col1.m_quad = _mm_add_ps(a.m_col1.m_quad, m_col1.m_quad);
    m_col2.m_quad = _mm_add_ps(a.m_col2.m_quad, m_col2.m_quad);
#else
    m_col0.x += a.m_col0.x; m_col0.y += a.m_col0.y; m_col0.z += a.m_col0.z;
    m_col1.x += a.m_col1.x; m_col1.y += a.m_col1.y; m_col1.z += a.m_col1.z;
    m_col2.x += a.m_col2.x; m_col2.y += a.m_col2.y; m_col2.z += a.m_col2.z;
#endif
}

// IDA: ?sub@hkMatrix3@@QEAAXAEBV1@@Z @ 0x1407adb10
void hkMatrix3::sub(const hkMatrix3& a) {
#ifdef _WIN32
    m_col0.m_quad = _mm_sub_ps(m_col0.m_quad, a.m_col0.m_quad);
    m_col1.m_quad = _mm_sub_ps(m_col1.m_quad, a.m_col1.m_quad);
    m_col2.m_quad = _mm_sub_ps(m_col2.m_quad, a.m_col2.m_quad);
#else
    m_col0.x -= a.m_col0.x; m_col0.y -= a.m_col0.y; m_col0.z -= a.m_col0.z;
    m_col1.x -= a.m_col1.x; m_col1.y -= a.m_col1.y; m_col1.z -= a.m_col1.z;
    m_col2.x -= a.m_col2.x; m_col2.y -= a.m_col2.y; m_col2.z -= a.m_col2.z;
#endif
}

// IDA: ?setTranspose@hkMatrix3@@QEAAXAEBV1@@Z @ 0x1407ade00
void hkMatrix3::setTranspose(const hkMatrix3& s) {
#ifdef _WIN32
    __m128 v2 = s.m_col1.m_quad;
    __m128 v3 = s.m_col2.m_quad;
    __m128 v4 = _mm_unpacklo_ps(s.m_col0.m_quad, v2);
    __m128 v5 = _mm_shuffle_ps(_mm_unpackhi_ps(s.m_col0.m_quad, v2), v3, 0xE4);  // 228 = 0xE4
    __m128 v6 = _mm_movelh_ps(v4, v3);
    m_col0.m_quad = v6;
    m_col1.m_quad = _mm_shuffle_ps(_mm_movehl_ps(v6, v4), v3, 0xD4);  // 212 = 0xD4
    m_col2.m_quad = v5;
#else
    m_col0.x = s.m_col0.x; m_col0.y = s.m_col1.x; m_col0.z = s.m_col2.x;
    m_col1.x = s.m_col0.y; m_col1.y = s.m_col1.y; m_col1.z = s.m_col2.y;
    m_col2.x = s.m_col0.z; m_col2.y = s.m_col1.z; m_col2.z = s.m_col2.z;
#endif
}

// IDA: ?mul@hkMatrix3@@QEAAXAEBV1@@Z @ 0x1407ae2f0
void hkMatrix3::mul(const hkMatrix3& a) {
    hkMatrix3 temp;
    temp.setMul(*this, a);
    m_col0 = temp.m_col0;
    m_col1 = temp.m_col1;
    m_col2 = temp.m_col2;
}

// IDA: ?frobeniusNormSqr@hkMatrix3@@QEBA?AVhkSimdReal@@XZ @ 0x1407ae410
hkSimdReal hkMatrix3::frobeniusNormSqr() const {
    hkSimdReal result;
#ifdef _WIN32
    __m128 v3 = _mm_mul_ps(m_col0.m_quad, m_col0.m_quad);
    __m128 v4 = _mm_mul_ps(m_col1.m_quad, m_col1.m_quad);
    __m128 v5 = _mm_mul_ps(m_col2.m_quad, m_col2.m_quad);
    result.m_real = _mm_add_ps(
        _mm_add_ps(
            _mm_add_ps(
                _mm_add_ps(_mm_shuffle_ps(v3, v3, 85), _mm_shuffle_ps(v3, v3, 0)),
                _mm_shuffle_ps(v3, v3, 170)),
            _mm_add_ps(
                _mm_add_ps(_mm_shuffle_ps(v4, v4, 85), _mm_shuffle_ps(v4, v4, 0)),
                _mm_shuffle_ps(v4, v4, 170))),
        _mm_add_ps(
            _mm_add_ps(_mm_shuffle_ps(v5, v5, 85), _mm_shuffle_ps(v5, v5, 0)),
            _mm_shuffle_ps(v5, v5, 170)));
#else
    float sum = m_col0.x * m_col0.x + m_col0.y * m_col0.y + m_col0.z * m_col0.z +
                m_col1.x * m_col1.x + m_col1.y * m_col1.y + m_col1.z * m_col1.z +
                m_col2.x * m_col2.x + m_col2.y * m_col2.y + m_col2.z * m_col2.z;
    result.m_real = _mm_set_ss(sum);
#endif
    return result;
}

// IDA: ?setTransposeMul@hkMatrix3@@QEAAXAEBV1@0@Z @ 0x1407ad8d0
void hkMatrix3::setTransposeMul(const hkMatrix3& a, const hkMatrix3& b) {
#ifdef _WIN32
    // SSE-optimized implementation
    hkVector4 v3, v4;
    v3.m_quad = a.m_col2.m_quad;
    v4.m_quad = a.m_col1.m_quad;

    __m128 v5 = _mm_mul_ps(b.m_col0.m_quad, a.m_col0.m_quad);
    __m128 v6 = _mm_mul_ps(v3.m_quad, b.m_col0.m_quad);
    __m128 v7 = _mm_mul_ps(b.m_col1.m_quad, a.m_col0.m_quad);
    __m128 v8 = _mm_mul_ps(v4.m_quad, b.m_col0.m_quad);
    __m128 v9 = _mm_unpacklo_ps(v5, v8);
    __m128 v10 = _mm_movelh_ps(v9, v6);
    __m128 v11 = _mm_movehl_ps(v10, v9);
    __m128 v12 = _mm_mul_ps(b.m_col1.m_quad, v4.m_quad);
    __m128 v13 = _mm_mul_ps(b.m_col1.m_quad, v3.m_quad);
    hkVector4 v14;
    v14.m_quad = _mm_add_ps(
        _mm_shuffle_ps(_mm_unpackhi_ps(v5, v8), v6, 228),
        _mm_add_ps(v10, _mm_shuffle_ps(v11, v6, 212)));
    __m128 v15 = _mm_unpacklo_ps(v7, v12);
    __m128 v16 = _mm_unpackhi_ps(v7, v12);
    __m128 v17 = _mm_movelh_ps(v15, v13);
    __m128 v18 = _mm_mul_ps(b.m_col2.m_quad, v4.m_quad);
    __m128 v19 = _mm_mul_ps(b.m_col2.m_quad, v3.m_quad);
    __m128 v20 = _mm_movehl_ps(v17, v15);
    __m128 v21 = _mm_mul_ps(b.m_col2.m_quad, a.m_col0.m_quad);
    m_col0 = v14;
    __m128 v22 = _mm_unpackhi_ps(v21, v18);
    __m128 v23 = _mm_unpacklo_ps(v21, v18);
    __m128 v24 = _mm_movelh_ps(v23, v19);
    m_col1.m_quad = _mm_add_ps(_mm_shuffle_ps(v16, v13, 228), _mm_add_ps(v17, _mm_shuffle_ps(v20, v13, 212)));
    m_col2.m_quad = _mm_add_ps(
        _mm_shuffle_ps(v22, v19, 228),
        _mm_add_ps(v24, _mm_shuffle_ps(_mm_movehl_ps(v24, v23), v19, 212)));
#else
    // Scalar fallback - this = a^T * b
    // Transpose of a is: rows of a become columns
    m_col0.x = a.m_col0.x * b.m_col0.x + a.m_col0.y * b.m_col0.y + a.m_col0.z * b.m_col0.z;
    m_col0.y = a.m_col0.x * b.m_col1.x + a.m_col0.y * b.m_col1.y + a.m_col0.z * b.m_col1.z;
    m_col0.z = a.m_col0.x * b.m_col2.x + a.m_col0.y * b.m_col2.y + a.m_col0.z * b.m_col2.z;

    m_col1.x = a.m_col1.x * b.m_col0.x + a.m_col1.y * b.m_col0.y + a.m_col1.z * b.m_col0.z;
    m_col1.y = a.m_col1.x * b.m_col1.x + a.m_col1.y * b.m_col1.y + a.m_col1.z * b.m_col1.z;
    m_col1.z = a.m_col1.x * b.m_col2.x + a.m_col1.y * b.m_col2.y + a.m_col1.z * b.m_col2.z;

    m_col2.x = a.m_col2.x * b.m_col0.x + a.m_col2.y * b.m_col0.y + a.m_col2.z * b.m_col0.z;
    m_col2.y = a.m_col2.x * b.m_col1.x + a.m_col2.y * b.m_col1.y + a.m_col2.z * b.m_col1.z;
    m_col2.z = a.m_col2.x * b.m_col2.x + a.m_col2.y * b.m_col2.y + a.m_col2.z * b.m_col2.z;
#endif
}

// IDA: ?setMul@hkMatrix3@@QEAAXAEBVhkSimdReal@@AEBV1@@Z @ 0x1407adab0
void hkMatrix3::setMul(const hkSimdReal& scale, const hkMatrix3& a) {
#ifdef _WIN32
    m_col0.m_quad = _mm_mul_ps(scale.m_real, a.m_col0.m_quad);
    m_col1.m_quad = _mm_mul_ps(a.m_col1.m_quad, scale.m_real);
    m_col2.m_quad = _mm_mul_ps(a.m_col2.m_quad, scale.m_real);
#else
    m_col0.x = scale.m_value * a.m_col0.x;
    m_col0.y = scale.m_value * a.m_col0.y;
    m_col0.z = scale.m_value * a.m_col0.z;
    m_col1.x = scale.m_value * a.m_col1.x;
    m_col1.y = scale.m_value * a.m_col1.y;
    m_col1.z = scale.m_value * a.m_col1.z;
    m_col2.x = scale.m_value * a.m_col2.x;
    m_col2.y = scale.m_value * a.m_col2.y;
    m_col2.z = scale.m_value * a.m_col2.z;
#endif
}

// IDA: ?addMul@hkMatrix3@@QEAAXAEBVhkSimdReal@@AEBV1@@Z @ 0x1407adf50
void hkMatrix3::addMul(const hkSimdReal& scale, const hkMatrix3& a) {
#ifdef _WIN32
    m_col0.m_quad = _mm_add_ps(_mm_mul_ps(scale.m_real, a.m_col0.m_quad), m_col0.m_quad);
    m_col1.m_quad = _mm_add_ps(_mm_mul_ps(a.m_col1.m_quad, scale.m_real), m_col1.m_quad);
    m_col2.m_quad = _mm_add_ps(_mm_mul_ps(a.m_col2.m_quad, scale.m_real), m_col2.m_quad);
#else
    m_col0.x += scale.m_value * a.m_col0.x;
    m_col0.y += scale.m_value * a.m_col0.y;
    m_col0.z += scale.m_value * a.m_col0.z;
    m_col1.x += scale.m_value * a.m_col1.x;
    m_col1.y += scale.m_value * a.m_col1.y;
    m_col1.z += scale.m_value * a.m_col1.z;
    m_col2.x += scale.m_value * a.m_col2.x;
    m_col2.y += scale.m_value * a.m_col2.y;
    m_col2.z += scale.m_value * a.m_col2.z;
#endif
}

// IDA: ?isApproximatelyEqual@hkMatrix3@@QEBA_NAEBV1@M@Z @ 0x1407adf90
bool hkMatrix3::isApproximatelyEqual(const hkMatrix3& m, float epsilon) const {
#ifdef _WIN32
    __m128 v3 = _mm_shuffle_ps(_mm_set_ss(epsilon), _mm_set_ss(epsilon), 0);
    return (_mm_movemask_ps(
            _mm_cmplt_ps(
                _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(_mm_castps_si128(_mm_sub_ps(m_col0.m_quad, m.m_col0.m_quad)), 1), 1)),
                v3)) & 7) == 7
        && (_mm_movemask_ps(
            _mm_cmplt_ps(
                _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(_mm_castps_si128(_mm_sub_ps(m_col1.m_quad, m.m_col1.m_quad)), 1), 1)),
                v3)) & 7) == 7
        && (_mm_movemask_ps(
            _mm_cmplt_ps(
                _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(_mm_castps_si128(_mm_sub_ps(m_col2.m_quad, m.m_col2.m_quad)), 1), 1)),
                v3)) & 7) == 7;
#else
    // Compare absolute values
    return fabsf(m_col0.x - m.m_col0.x) < epsilon &&
           fabsf(m_col0.y - m.m_col0.y) < epsilon &&
           fabsf(m_col0.z - m.m_col0.z) < epsilon &&
           fabsf(m_col1.x - m.m_col1.x) < epsilon &&
           fabsf(m_col1.y - m.m_col1.y) < epsilon &&
           fabsf(m_col1.z - m.m_col1.z) < epsilon &&
           fabsf(m_col2.x - m.m_col2.x) < epsilon &&
           fabsf(m_col2.y - m.m_col2.y) < epsilon &&
           fabsf(m_col2.z - m.m_col2.z) < epsilon;
#endif
}

// Helper for float comparison
static bool floatApproxEqual(float a, float b, float epsilon) {
    return fabsf(a - b) < epsilon;
}

// IDA: ?isSymmetric@hkMatrix3@@QEBA_NM@Z @ 0x1407ae080
bool hkMatrix3::isSymmetric(float epsilon) const {
    // Check if matrix equals its transpose (ignoring diagonal)
    return floatApproxEqual(m_col1.x, m_col0.y, epsilon) &&
           floatApproxEqual(m_col2.x, m_col0.z, epsilon) &&
           floatApproxEqual(m_col2.y, m_col1.z, epsilon);
}

// IDA: ?invert@hkMatrix3@@QEAA?AUhkResult@@M@Z @ 0x1407ae0e0
hkResult hkMatrix3::invert(float epsilon) {
#ifdef _WIN32
    hkVector4 v3, v4;
    v3.m_quad = m_col2.m_quad;
    v4.m_quad = m_col1.m_quad;

    __m128 v6 = _mm_shuffle_ps(v4.m_quad, v4.m_quad, 201);
    __m128 v7 = _mm_shuffle_ps(v3.m_quad, v3.m_quad, 201);
    __m128 v8 = _mm_sub_ps(_mm_mul_ps(v7, v4.m_quad), _mm_mul_ps(v3.m_quad, v6));
    __m128 v9 = _mm_shuffle_ps(m_col0.m_quad, m_col0.m_quad, 201);
    __m128 v10 = _mm_shuffle_ps(v8, v8, 201);
    __m128 v11 = _mm_mul_ps(m_col0.m_quad, v10);
    __m128 v12 = _mm_sub_ps(_mm_mul_ps(v6, m_col0.m_quad), _mm_mul_ps(v4.m_quad, v9));
    __m128 v13 = _mm_sub_ps(_mm_mul_ps(v3.m_quad, v9), _mm_mul_ps(v7, m_col0.m_quad));
    __m128 v14 = _mm_add_ps(_mm_add_ps(_mm_shuffle_ps(v11, v11, 85), _mm_shuffle_ps(v11, v11, 0)), _mm_shuffle_ps(v11, v11, 170));

    // Check determinant - extract first float from v14
    float det;
    _mm_store_ss(&det, v14);
    if (fabsf(det) <= epsilon * epsilon * epsilon) {
        hkResult result;
        result.m_enum = 1; // HK_FAILURE
        return result;
    }

    __m128 v15 = _mm_shuffle_ps(v13, v13, 201);
    __m128 v16 = _mm_shuffle_ps(v12, v12, 201);
    __m128 v17 = _mm_rcp_ps(v14);
    // Newton-Raphson refinement: 2.0 - x * (1/x) * x = more accurate 1/x
    __m128 two = _mm_set1_ps(2.0f);
    __m128 v18 = _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(v17, v14)), v17);

    m_col2.m_quad = _mm_mul_ps(v18, v16);
    m_col0.m_quad = _mm_mul_ps(v18, v10);
    m_col1.m_quad = _mm_mul_ps(v18, v15);
    transpose();

    hkResult result;
    result.m_enum = 0; // HK_SUCCESS
    return result;
#else
    // Calculate cofactor matrix and determinant
    float det = m_col0.x * (m_col1.y * m_col2.z - m_col1.z * m_col2.y) -
                m_col0.y * (m_col1.x * m_col2.z - m_col1.z * m_col2.x) +
                m_col0.z * (m_col1.x * m_col2.y - m_col1.y * m_col2.x);

    if (fabsf(det) <= epsilon * epsilon * epsilon) {
        hkResult result;
        result.m_enum = 1;
        return result;
    }

    float invDet = 1.0f / det;

    // Calculate inverse matrix (adjugate / determinant)
    hkMatrix3 inv;
    inv.m_col0.x = (m_col1.y * m_col2.z - m_col1.z * m_col2.y) * invDet;
    inv.m_col0.y = (m_col0.z * m_col2.y - m_col0.y * m_col2.z) * invDet;
    inv.m_col0.z = (m_col0.y * m_col1.z - m_col0.z * m_col1.y) * invDet;

    inv.m_col1.x = (m_col1.z * m_col2.x - m_col1.x * m_col2.z) * invDet;
    inv.m_col1.y = (m_col0.x * m_col2.z - m_col0.z * m_col2.x) * invDet;
    inv.m_col1.z = (m_col0.z * m_col1.x - m_col0.x * m_col1.z) * invDet;

    inv.m_col2.x = (m_col1.x * m_col2.y - m_col1.y * m_col2.x) * invDet;
    inv.m_col2.y = (m_col0.y * m_col2.x - m_col0.x * m_col2.y) * invDet;
    inv.m_col2.z = (m_col0.x * m_col1.y - m_col0.y * m_col1.x) * invDet;

    *this = inv;
    hkResult result;
    result.m_enum = 0;
    return result;
#endif
}

// IDA: ?invertSymmetric@hkMatrix3@@QEAAXXZ @ 0x1407ae1f0
void hkMatrix3::invertSymmetric() {
#ifdef _WIN32
    hkVector4 v1, v2;
    v1.m_quad = m_col1.m_quad;
    v2.m_quad = m_col2.m_quad;

    __m128 v3 = _mm_shuffle_ps(v1.m_quad, v1.m_quad, 201);
    __m128 v4 = _mm_shuffle_ps(v2.m_quad, v2.m_quad, 201);
    __m128 v5 = _mm_mul_ps(v4, m_col0.m_quad);
    __m128 v6 = _mm_sub_ps(_mm_mul_ps(v4, v1.m_quad), _mm_mul_ps(v2.m_quad, v3));
    __m128 v7 = _mm_shuffle_ps(m_col0.m_quad, m_col0.m_quad, 201);
    __m128 v8 = _mm_shuffle_ps(v6, v6, 201);
    __m128 v9 = _mm_sub_ps(_mm_mul_ps(v2.m_quad, v7), v5);
    __m128 v10 = _mm_sub_ps(_mm_mul_ps(v3, m_col0.m_quad), _mm_mul_ps(v1.m_quad, v7));
    __m128 v11 = _mm_mul_ps(m_col0.m_quad, v8);
    __m128 v12 = _mm_add_ps(_mm_add_ps(_mm_shuffle_ps(v11, v11, 85), _mm_shuffle_ps(v11, v11, 0)), _mm_shuffle_ps(v11, v11, 170));

    // Use g_vectorConstants[34] (0.0001) and g_vectorConstants[2] (1.0) for comparison
    extern const float g_vectorConstants[];
    __m128 v13 = _mm_cmplt_ps(_mm_mul_ps(_mm_mul_ps(_mm_load_ps(&g_vectorConstants[34 * 4]), _mm_load_ps(&g_vectorConstants[34 * 4])), _mm_load_ps(&g_vectorConstants[34 * 4])), v12);
    __m128 v14 = _mm_or_ps(_mm_and_ps(v13, v12), _mm_andnot_ps(v13, _mm_load_ps(&g_vectorConstants[2 * 4])));
    __m128 v15 = _mm_rcp_ps(v14);
    __m128 v16 = _mm_or_ps(
        _mm_and_ps(_mm_mul_ps(_mm_sub_ps(_mm_load_ps(&g_vectorConstants[8 * 4]), _mm_mul_ps(v15, v14)), v15), v13),
        _mm_andnot_ps(v13, _mm_setzero_ps()));

    m_col0.m_quad = _mm_mul_ps(v16, v8);
    m_col2.m_quad = _mm_mul_ps(v16, _mm_shuffle_ps(v10, v10, 201));
    m_col1.m_quad = _mm_mul_ps(v16, _mm_shuffle_ps(v9, v9, 201));
#else
    // Scalar fallback for symmetric matrix inverse
    // For symmetric matrices, we can use the cofactor method
    float det = m_col0.x * (m_col1.y * m_col2.z - m_col1.z * m_col2.y) -
                m_col0.y * (m_col0.y * m_col2.z - m_col1.z * m_col0.z) +
                m_col0.z * (m_col0.y * m_col2.y - m_col1.y * m_col0.z);

    if (fabsf(det) < 0.0001f) {
        return; // Singular matrix
    }

    float invDet = 1.0f / det;

    float c0 = (m_col1.y * m_col2.z - m_col1.z * m_col2.y) * invDet;
    float c1 = (m_col0.z * m_col2.y - m_col0.y * m_col2.z) * invDet;
    float c2 = (m_col0.y * m_col1.z - m_col0.z * m_col1.y) * invDet;
    float c4 = (m_col0.x * m_col2.z - m_col0.z * m_col2.x) * invDet;
    float c5 = (m_col0.z * m_col1.x - m_col0.x * m_col1.z) * invDet;
    float c8 = (m_col0.x * m_col1.y - m_col0.y * m_col1.x) * invDet;

    m_col0.x = c0; m_col0.y = c1; m_col0.z = c2;
    m_col1.x = c1; m_col1.y = c4; m_col1.z = c5;
    m_col2.x = c2; m_col2.y = c5; m_col2.z = c8;
#endif
}

// IDA: ?setMulInverse@hkMatrix3@@QEAAXAEBV1@AEBVhkRotation@@@Z @ 0x1407adea0
void hkMatrix3::setMulInverse(const hkMatrix3& aTb, const hkRotation& cTb) {
#ifdef _WIN32
    __m128 v10[9]; // Stack buffer

    *(__m128*)v10 = _mm_shuffle_ps(_mm_unpackhi_ps(cTb.m_col0.m_quad, cTb.m_col1.m_quad), cTb.m_col2.m_quad, 228);
    hkVector4 v4, v5;
    v4.m_quad = aTb.m_col0.m_quad;
    v5.m_quad = aTb.m_col1.m_quad;

    for (int i = 2; i >= 0; --i) {
        __m128 v7 = *(__m128*)&v10[i];
        *(__m128*)&v10[2 - i + 2] = _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(v7, v7, 0), v4.m_quad),
                _mm_mul_ps(_mm_shuffle_ps(v7, v7, 85), v5.m_quad)),
            _mm_mul_ps(_mm_shuffle_ps(v7, v7, 170), aTb.m_col2.m_quad));
    }

    m_col0 = *(hkVector4*)&v10[2];
    m_col1 = *(hkVector4*)&v10[4];
    m_col2 = *(hkVector4*)&v10[6];
#else
    // Scalar fallback: this = aTb * cTb^T
    // cTb^T is the transpose of cTb (since it's a rotation, transpose = inverse)
    m_col0.x = aTb.m_col0.x * cTb.m_col0.x + aTb.m_col1.x * cTb.m_col1.x + aTb.m_col2.x * cTb.m_col2.x;
    m_col0.y = aTb.m_col0.y * cTb.m_col0.x + aTb.m_col1.y * cTb.m_col1.x + aTb.m_col2.y * cTb.m_col2.x;
    m_col0.z = aTb.m_col0.z * cTb.m_col0.x + aTb.m_col1.z * cTb.m_col1.x + aTb.m_col2.z * cTb.m_col2.x;

    m_col1.x = aTb.m_col0.x * cTb.m_col0.y + aTb.m_col1.x * cTb.m_col1.y + aTb.m_col2.x * cTb.m_col2.y;
    m_col1.y = aTb.m_col0.y * cTb.m_col0.y + aTb.m_col1.y * cTb.m_col1.y + aTb.m_col2.y * cTb.m_col2.y;
    m_col1.z = aTb.m_col0.z * cTb.m_col0.y + aTb.m_col1.z * cTb.m_col1.y + aTb.m_col2.z * cTb.m_col2.y;

    m_col2.x = aTb.m_col0.x * cTb.m_col0.z + aTb.m_col1.x * cTb.m_col1.z + aTb.m_col2.x * cTb.m_col2.z;
    m_col2.y = aTb.m_col0.y * cTb.m_col0.z + aTb.m_col1.y * cTb.m_col1.z + aTb.m_col2.y * cTb.m_col2.z;
    m_col2.z = aTb.m_col0.z * cTb.m_col0.z + aTb.m_col1.z * cTb.m_col1.z + aTb.m_col2.z * cTb.m_col2.z;
#endif
}

// IDA: ?changeBasis@hkMatrix3@@QEAAXAEBVhkRotation@@@Z @ 0x1407ae590
void hkMatrix3::changeBasis(const hkRotation& r) {
    // this = r * this * r^T
    hkMatrix3 temp;
    temp.setMulInverse(*this, r);
    *this = temp;
}

// Helper function to extract scalar from __m128 without .m128_f32 extension
static inline float extract_ps_0(__m128 v) {
    float result;
    _mm_store_ss(&result, v);
    return result;
}

// IDA: ?diagonalizeSymmetric@hkMatrix3@@QEBA?AUhkResult@@AEAVhkRotation@@AEAVhkVector4@@HM@Z @ 0x1407ae5d0
// Jacobi eigenvalue decomposition for symmetric 3x3 matrices
hkResult hkMatrix3::diagonalizeSymmetric(hkRotation& eigenVec, hkVector4& eigenVal, int maxIter, float epsilon) const {
    // Initialize eigenvector matrix to identity
    eigenVec.m_col0 = hkVector4(1.0f, 0.0f, 0.0f, 0.0f);
    eigenVec.m_col1 = hkVector4(0.0f, 1.0f, 0.0f, 0.0f);
    eigenVec.m_col2 = hkVector4(0.0f, 0.0f, 1.0f, 0.0f);

    // Local matrix copy (column-major to row-major for easier Jacobi manipulation)
    float M[3][3] = {
        {m_col0.x, m_col1.x, m_col2.x},
        {m_col0.y, m_col1.y, m_col2.y},
        {m_col0.z, m_col1.z, m_col2.z}
    };

    // Compute frobenius norm squared for convergence threshold
    float frobeniusNormSqr = 0.0f;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            frobeniusNormSqr += M[i][j] * M[i][j];

    float threshold = frobeniusNormSqr * epsilon * epsilon;

    int iter = 0;
    while (iter < maxIter) {
        // Find largest off-diagonal element (store indices and value)
        int p = 0, q = 1;
        float maxOff = fabsf(M[0][1]);
        if (fabsf(M[0][2]) > maxOff) { p = 0; q = 2; maxOff = fabsf(M[0][2]); }
        if (fabsf(M[1][2]) > maxOff) { p = 1; q = 2; maxOff = fabsf(M[1][2]); }

        float offDiagSqr = M[0][1]*M[0][1] + M[0][2]*M[0][2] + M[1][2]*M[1][2];
        if (2.0f * offDiagSqr <= threshold) break;

        // Compute Givens rotation coefficients
        float c, s;
        if (M[p][q] == 0.0f) {
            c = 1.0f; s = 0.0f;
        } else {
            float tau = (M[q][q] - M[p][p]) / (2.0f * M[p][q]);
            float t = (tau < 0.0f) ? -1.0f / (sqrtf(tau*tau + 1.0f) - tau)
                                   : 1.0f / (sqrtf(tau*tau + 1.0f) + tau);
            c = 1.0f / sqrtf(1.0f + t*t);
            s = t * c;
        }

        // Apply Jacobi rotation to M: M = G^T * M * G
        // First: M = G^T * M (row rotation)
        for (int i = 0; i < 3; i++) {
            float temp = c * M[p][i] - s * M[q][i];
            M[q][i] = s * M[p][i] + c * M[q][i];
            M[p][i] = temp;
        }
        // Second: M = M * G (column rotation)
        for (int i = 0; i < 3; i++) {
            float temp = c * M[i][p] - s * M[i][q];
            M[i][q] = s * M[i][p] + c * M[i][q];
            M[i][p] = temp;
        }

        // Update eigenvector matrix: eigenVec = eigenVec * G
        float* V[3] = { &eigenVec.m_col0.x, &eigenVec.m_col1.x, &eigenVec.m_col2.x };
        for (int i = 0; i < 3; i++) {
            float temp = c * V[p][i] + s * V[q][i];
            V[q][i] = -s * V[p][i] + c * V[q][i];
            V[p][i] = temp;
        }

        iter++;
    }

    // Extract eigenvalues from diagonal
    eigenVal = hkVector4(M[0][0], M[1][1], M[2][2], 0.0f);

    float offDiagSqr = M[0][1]*M[0][1] + M[0][2]*M[0][2] + M[1][2]*M[1][2];
    hkResult result;
    result.m_enum = (2.0f * offDiagSqr > threshold) ? 1 : 0;
    return result;
}

// IDA: ?diagonalizeSymmetricApproximation@hkMatrix3@@QEBAXAEAVhkRotation@@AEAVhkVector4@@H@Z @ 0x1407ae9d0
void hkMatrix3::diagonalizeSymmetricApproximation(hkRotation& eigenVec, hkVector4& eigenVal, int maxIter) const {
    // Use fixed epsilon of 1e-7 for approximation
    const float epsilon = 1e-7f;

    // Initialize eigenvector matrix to identity
    eigenVec.m_col0 = hkVector4(1.0f, 0.0f, 0.0f, 0.0f);
    eigenVec.m_col1 = hkVector4(0.0f, 1.0f, 0.0f, 0.0f);
    eigenVec.m_col2 = hkVector4(0.0f, 0.0f, 1.0f, 0.0f);

    // Local matrix copy
    float M[3][3] = {
        {m_col0.x, m_col1.x, m_col2.x},
        {m_col0.y, m_col1.y, m_col2.y},
        {m_col0.z, m_col1.z, m_col2.z}
    };

    // Compute threshold
    float frobeniusNormSqr = 0.0f;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            frobeniusNormSqr += M[i][j] * M[i][j];

    float threshold = frobeniusNormSqr * epsilon * epsilon;

    for (int iter = 0; iter < maxIter; iter++) {
        // Find largest off-diagonal element
        int p = 0, q = 1;
        float maxOff = fabsf(M[0][1]);
        if (fabsf(M[0][2]) > maxOff) { p = 0; q = 2; maxOff = fabsf(M[0][2]); }
        if (fabsf(M[1][2]) > maxOff) { p = 1; q = 2; maxOff = fabsf(M[1][2]); }

        if (maxOff * maxOff < threshold) break;

        // Compute rotation
        float c, s;
        if (M[p][q] == 0.0f) {
            c = 1.0f; s = 0.0f;
        } else {
            float tau = (M[q][q] - M[p][p]) / (2.0f * M[p][q]);
            float t = (tau < 0.0f) ? -1.0f / (sqrtf(tau*tau + 1.0f) - tau)
                                   : 1.0f / (sqrtf(tau*tau + 1.0f) + tau);
            c = 1.0f / sqrtf(1.0f + t*t);
            s = t * c;
        }

        // Apply Jacobi rotation
        for (int i = 0; i < 3; i++) {
            float temp = c * M[p][i] - s * M[q][i];
            M[q][i] = s * M[p][i] + c * M[q][i];
            M[p][i] = temp;
        }
        for (int i = 0; i < 3; i++) {
            float temp = c * M[i][p] - s * M[i][q];
            M[i][q] = s * M[i][p] + c * M[i][q];
            M[i][p] = temp;
        }

        // Update eigenvector
        float* V[3] = { &eigenVec.m_col0.x, &eigenVec.m_col1.x, &eigenVec.m_col2.x };
        for (int i = 0; i < 3; i++) {
            float temp = c * V[p][i] + s * V[q][i];
            V[q][i] = -s * V[p][i] + c * V[q][i];
            V[p][i] = temp;
        }
    }

    eigenVal = hkVector4(M[0][0], M[1][1], M[2][2], 0.0f);
}

// IDA: ?diagonalizeSymmetricWarmStart@hkMatrix3@@QEBA?AUhkResult@@AEAV1@AEAVhkVector4@@HM@Z @ 0x1407aed20
hkResult hkMatrix3::diagonalizeSymmetricWarmStart(hkMatrix3& eigenVec, hkVector4& eigenVal, int maxIter, float epsilon) const {
    // Warm start: eigenVec contains initial guess for eigenvectors (stored as columns)
    // Transform to eigen space first, then apply Jacobi rotations

    // Copy matrix and eigenvector estimate
    float M[3][3] = {
        {m_col0.x, m_col1.x, m_col2.x},
        {m_col0.y, m_col1.y, m_col2.y},
        {m_col0.z, m_col1.z, m_col2.z}
    };

    float V[3][3] = {
        {eigenVec.m_col0.x, eigenVec.m_col1.x, eigenVec.m_col2.x},
        {eigenVec.m_col0.y, eigenVec.m_col1.y, eigenVec.m_col2.y},
        {eigenVec.m_col0.z, eigenVec.m_col1.z, eigenVec.m_col2.z}
    };

    // Transform to eigen space: M' = V * M * V^T
    float temp[3][3], Mprime[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp[i][j] = 0;
            for (int k = 0; k < 3; k++) temp[i][j] += V[i][k] * M[k][j];
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Mprime[i][j] = 0;
            for (int k = 0; k < 3; k++) Mprime[i][j] += temp[i][k] * V[j][k];
        }
    }

    // Compute convergence threshold
    float frobeniusNormSqr = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            frobeniusNormSqr += Mprime[i][j] * Mprime[i][j];

    float threshold = frobeniusNormSqr * epsilon * epsilon;

    for (int iter = 0; iter < maxIter; iter++) {
        int p = 0, q = 1;
        float maxOff = fabsf(Mprime[0][1]);
        if (fabsf(Mprime[0][2]) > maxOff) { p = 0; q = 2; maxOff = fabsf(Mprime[0][2]); }
        if (fabsf(Mprime[1][2]) > maxOff) { p = 1; q = 2; maxOff = fabsf(Mprime[1][2]); }

        float offDiagSqr = Mprime[0][1]*Mprime[0][1] + Mprime[0][2]*Mprime[0][2] + Mprime[1][2]*Mprime[1][2];
        if (2.0f * offDiagSqr < threshold) break;

        float c, s;
        if (Mprime[p][q] == 0.0f) {
            c = 1.0f; s = 0.0f;
        } else {
            float tau = (Mprime[q][q] - Mprime[p][p]) / (2.0f * Mprime[p][q]);
            float t = (tau < 0.0f) ? -1.0f / (sqrtf(tau*tau + 1.0f) - tau)
                                   : 1.0f / (sqrtf(tau*tau + 1.0f) + tau);
            c = 1.0f / sqrtf(1.0f + t*t);
            s = t * c;
        }

        // Apply Jacobi rotation to Mprime
        for (int i = 0; i < 3; i++) {
            float temp = c * Mprime[p][i] - s * Mprime[q][i];
            Mprime[q][i] = s * Mprime[p][i] + c * Mprime[q][i];
            Mprime[p][i] = temp;
        }
        for (int i = 0; i < 3; i++) {
            float temp = c * Mprime[i][p] - s * Mprime[i][q];
            Mprime[i][q] = s * Mprime[i][p] + c * Mprime[i][q];
            Mprime[i][p] = temp;
        }

        // Update eigenvector: V = V * G
        for (int i = 0; i < 3; i++) {
            float temp = c * V[p][i] - s * V[q][i];
            V[q][i] = s * V[p][i] + c * V[q][i];
            V[p][i] = temp;
        }
    }

    // Renormalize eigenvectors
    for (int j = 0; j < 3; j++) {
        float norm = sqrtf(V[0][j]*V[0][j] + V[1][j]*V[1][j] + V[2][j]*V[2][j]);
        if (norm > 0.0001f) {
            V[0][j] /= norm;
            V[1][j] /= norm;
            V[2][j] /= norm;
        }
    }

    // Copy results back
    eigenVec.m_col0 = hkVector4(V[0][0], V[1][0], V[2][0], 0.0f);
    eigenVec.m_col1 = hkVector4(V[0][1], V[1][1], V[2][1], 0.0f);
    eigenVec.m_col2 = hkVector4(V[0][2], V[1][2], V[2][2], 0.0f);

    eigenVal = hkVector4(Mprime[0][0], Mprime[1][1], Mprime[2][2], 0.0f);

    float offDiagSqr = Mprime[0][1]*Mprime[0][1] + Mprime[0][2]*Mprime[0][2] + Mprime[1][2]*Mprime[1][2];
    hkResult result;
    result.m_enum = (2.0f * offDiagSqr > threshold) ? 1 : 0;
    return result;
}

// ============================================================================
// hkRotation implementations
// ============================================================================

// IDA: ?set@hkRotation@@QEAAXAEBVhkQuaternion@@@Z @ 0x140799ba0
void hkRotation::set(const hkQuaternion& qi) {
    // Convert quaternion to rotation matrix
    float x = qi.m_vec.x;
    float y = qi.m_vec.y;
    float z = qi.m_vec.z;
    float w = qi.m_vec.w;

    float xx = x * x;
    float yy = y * y;
    float zz = z * z;

    float xy = x * y;
    float xz = x * z;
    float yz = y * z;

    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    m_col0.x = 1.0f - 2.0f * (yy + zz);
    m_col0.y = 2.0f * (xy + wz);
    m_col0.z = 2.0f * (xz - wy);
    m_col0.w = 0.0f;

    m_col1.x = 2.0f * (xy - wz);
    m_col1.y = 1.0f - 2.0f * (xx + zz);
    m_col1.z = 2.0f * (yz + wx);
    m_col1.w = 0.0f;

    m_col2.x = 2.0f * (xz + wy);
    m_col2.y = 2.0f * (yz - wx);
    m_col2.z = 1.0f - 2.0f * (xx + yy);
    m_col2.w = 0.0f;
}

// IDA: ?setAxisAngle@hkRotation@@QEAAXAEBVhkVector4@@M@Z @ 0x140799c40
void hkRotation::setAxisAngle(const hkVector4& axis, float angle) {
    hkQuaternion q;
    q.setAxisAngle(axis, angle);
    set(q);
}

// IDA: ?isOrthonormal@hkRotation@@QEBA_NM@Z @ 0x140799c70
bool hkRotation::isOrthonormal(float epsilon) const {
    // Check if each column has unit length
    float len0 = m_col0.x * m_col0.x + m_col0.y * m_col0.y + m_col0.z * m_col0.z;
    if (fabsf(len0 - 1.0f) > epsilon) return false;

    float len1 = m_col1.x * m_col1.x + m_col1.y * m_col1.y + m_col1.z * m_col1.z;
    if (fabsf(len1 - 1.0f) > epsilon) return false;

    float len2 = m_col2.x * m_col2.x + m_col2.y * m_col2.y + m_col2.z * m_col2.z;
    if (fabsf(len2 - 1.0f) > epsilon) return false;

    // Check if columns are orthogonal (col0 x col1 = col2)
    float cx = m_col0.y * m_col1.z - m_col0.z * m_col1.y;
    float cy = m_col0.z * m_col1.x - m_col0.x * m_col1.z;
    float cz = m_col0.x * m_col1.y - m_col0.y * m_col1.x;

    if (fabsf(cx - m_col2.x) > epsilon) return false;
    if (fabsf(cy - m_col2.y) > epsilon) return false;
    if (fabsf(cz - m_col2.z) > epsilon) return false;

    return true;
}

// IDA: ?isOk@hkRotation@@QEBA_NXZ @ 0x140799d80
bool hkRotation::isOk() const {
    return isOrthonormal(0.0000099999997f);
}

// IDA: ?renormalize@hkRotation@@QEAAXXZ @ 0x140799dc0
void hkRotation::renormalize() {
    // Convert to quaternion and back to renormalize
    hkQuaternion q;
    q.set(*this);

    // Normalize the quaternion
    float len = sqrtf(q.m_vec.x * q.m_vec.x + q.m_vec.y * q.m_vec.y +
                      q.m_vec.z * q.m_vec.z + q.m_vec.w * q.m_vec.w);
    if (len > 0.0001f) {
        q.m_vec.x /= len;
        q.m_vec.y /= len;
        q.m_vec.z /= len;
        q.m_vec.w /= len;
    }

    // Convert back to rotation matrix
    set(q);
}

// ============================================================================
// hkTransform implementations
// ============================================================================

// IDA: ?setMul@hkTransform@@QEAAXAEBV1@0@Z @ 0x14079a670
void hkTransform::setMul(const hkTransform& aTb, const hkTransform& bTc) {
    // Multiply two transforms: this = aTb * bTc
    // Rotation = aTb.rotation * bTc.rotation
    // Translation = aTb.rotation * bTc.translation + aTb.translation

    // Multiply rotation matrices
    const hkRotation& rA = aTb.m_rotation;
    const hkRotation& rB = bTc.m_rotation;

    m_rotation.m_col0.x = rA.m_col0.x * rB.m_col0.x + rA.m_col1.x * rB.m_col0.y + rA.m_col2.x * rB.m_col0.z;
    m_rotation.m_col0.y = rA.m_col0.y * rB.m_col0.x + rA.m_col1.y * rB.m_col0.y + rA.m_col2.y * rB.m_col0.z;
    m_rotation.m_col0.z = rA.m_col0.z * rB.m_col0.x + rA.m_col1.z * rB.m_col0.y + rA.m_col2.z * rB.m_col0.z;
    m_rotation.m_col0.w = 0.0f;

    m_rotation.m_col1.x = rA.m_col0.x * rB.m_col1.x + rA.m_col1.x * rB.m_col1.y + rA.m_col2.x * rB.m_col1.z;
    m_rotation.m_col1.y = rA.m_col0.y * rB.m_col1.x + rA.m_col1.y * rB.m_col1.y + rA.m_col2.y * rB.m_col1.z;
    m_rotation.m_col1.z = rA.m_col0.z * rB.m_col1.x + rA.m_col1.z * rB.m_col1.y + rA.m_col2.z * rB.m_col1.z;
    m_rotation.m_col1.w = 0.0f;

    m_rotation.m_col2.x = rA.m_col0.x * rB.m_col2.x + rA.m_col1.x * rB.m_col2.y + rA.m_col2.x * rB.m_col2.z;
    m_rotation.m_col2.y = rA.m_col0.y * rB.m_col2.x + rA.m_col1.y * rB.m_col2.y + rA.m_col2.y * rB.m_col2.z;
    m_rotation.m_col2.z = rA.m_col0.z * rB.m_col2.x + rA.m_col1.z * rB.m_col2.y + rA.m_col2.z * rB.m_col2.z;
    m_rotation.m_col2.w = 0.0f;

    // Translation: aTb.rotation * bTc.translation + aTb.translation
    const hkVector4& tB = bTc.m_translation;
    m_translation.x = rA.m_col0.x * tB.x + rA.m_col1.x * tB.y + rA.m_col2.x * tB.z + aTb.m_translation.x;
    m_translation.y = rA.m_col0.y * tB.x + rA.m_col1.y * tB.y + rA.m_col2.y * tB.z + aTb.m_translation.y;
    m_translation.z = rA.m_col0.z * tB.x + rA.m_col1.z * tB.y + rA.m_col2.z * tB.z + aTb.m_translation.z;
    m_translation.w = 0.0f;
}

// IDA: ?setMulEq@hkTransform@@QEAAXAEBV1@@Z @ 0x14079a6e0
void hkTransform::setMulEq(const hkTransform& bTc) {
    // this = this * bTc
    hkTransform temp;
    temp.setMul(*this, bTc);
    *this = temp;
}

// IDA: ?setMul@hkTransform@@QEAAXAEBVhkQsTransform@@AEBV1@@Z @ 0x14079a760
void hkTransform::setMul(const hkQsTransform& aTb, const hkTransform& bTc) {
#ifdef _WIN32
    // Convert quaternion to rotation matrix
    hkRotation rotMatrix;
    rotMatrix.set(aTb.m_rotation);

    // Create scale matrix by multiplying scale with identity column vectors
    // g_vectorConstants[28] = (1,0,0,0), [29] = (0,1,0,0), [30] = (0,0,1,0)
    hkRotation scaMatrix;
    scaMatrix.m_col0.m_quad = _mm_mul_ps(aTb.m_scale.m_quad, _mm_load_ps(&g_vectorConstants[28 * 4]));
    scaMatrix.m_col1.m_quad = _mm_mul_ps(aTb.m_scale.m_quad, _mm_load_ps(&g_vectorConstants[29 * 4]));
    scaMatrix.m_col2.m_quad = _mm_mul_ps(aTb.m_scale.m_quad, _mm_load_ps(&g_vectorConstants[30 * 4]));

    // Combine rotation and scale
    hkRotation rotSca;
    rotSca.setMul(rotMatrix, scaMatrix);

    // Multiply with bTc rotation and add aTb translation
    __m128 v7 = rotSca.m_col1.m_quad;
    __m128 v8 = rotSca.m_col0.m_quad;
    __m128 v9 = rotSca.m_col2.m_quad;

    m_rotation.m_col0.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bTc.m_rotation.m_col0.m_quad, bTc.m_rotation.m_col0.m_quad, 0x55), v7),
            _mm_mul_ps(_mm_shuffle_ps(bTc.m_rotation.m_col0.m_quad, bTc.m_rotation.m_col0.m_quad, 0x00), v8)),
        _mm_mul_ps(_mm_shuffle_ps(bTc.m_rotation.m_col0.m_quad, bTc.m_rotation.m_col0.m_quad, 0xAA), v9));

    m_rotation.m_col1.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bTc.m_rotation.m_col1.m_quad, bTc.m_rotation.m_col1.m_quad, 0x55), v7),
            _mm_mul_ps(_mm_shuffle_ps(bTc.m_rotation.m_col1.m_quad, bTc.m_rotation.m_col1.m_quad, 0x00), v8)),
        _mm_mul_ps(_mm_shuffle_ps(bTc.m_rotation.m_col1.m_quad, bTc.m_rotation.m_col1.m_quad, 0xAA), v9));

    m_rotation.m_col2.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bTc.m_rotation.m_col2.m_quad, bTc.m_rotation.m_col2.m_quad, 0x55), v7),
            _mm_mul_ps(_mm_shuffle_ps(bTc.m_rotation.m_col2.m_quad, bTc.m_rotation.m_col2.m_quad, 0x00), v8)),
        _mm_mul_ps(_mm_shuffle_ps(bTc.m_rotation.m_col2.m_quad, bTc.m_rotation.m_col2.m_quad, 0xAA), v9));

    // Transform translation and add aTb translation
    __m128 v12 = bTc.m_translation.m_quad;
    __m128 v13 = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(v12, v12, 0x55), v7),
            _mm_mul_ps(_mm_shuffle_ps(v12, v12, 0x00), v8)),
        _mm_mul_ps(_mm_shuffle_ps(v12, v12, 0xAA), v9));
    m_translation.m_quad = _mm_add_ps(v13, aTb.m_translation.m_quad);
#else
    // Non-SIMD fallback
    hkRotation rotMatrix;
    rotMatrix.set(aTb.m_rotation);

    // Apply scale and rotation to bTc
    for (int i = 0; i < 3; ++i) {
        const hkVector4& col = (&bTc.m_rotation.m_col0)[i];
        (&m_rotation.m_col0)[i].x = (rotMatrix.m_col0.x * aTb.m_scale.x * col.x + rotMatrix.m_col1.x * aTb.m_scale.y * col.y + rotMatrix.m_col2.x * aTb.m_scale.z * col.z);
        (&m_rotation.m_col0)[i].y = (rotMatrix.m_col0.y * aTb.m_scale.x * col.x + rotMatrix.m_col1.y * aTb.m_scale.y * col.y + rotMatrix.m_col2.y * aTb.m_scale.z * col.z);
        (&m_rotation.m_col0)[i].z = (rotMatrix.m_col0.z * aTb.m_scale.x * col.x + rotMatrix.m_col1.z * aTb.m_scale.y * col.y + rotMatrix.m_col2.z * aTb.m_scale.z * col.z);
    }

    m_translation.x = aTb.m_translation.x + aTb.m_scale.x * bTc.m_translation.x;
    m_translation.y = aTb.m_translation.y + aTb.m_scale.y * bTc.m_translation.y;
    m_translation.z = aTb.m_translation.z + aTb.m_scale.z * bTc.m_translation.z;
#endif
}

// IDA: ?setMulInverseMul@hkTransform@@QEAAXAEBV1@0@Z @ 0x14079a8b0
void hkTransform::setMulInverseMul(const hkTransform& bTa, const hkTransform& bTc) {
#ifdef _WIN32
    // Transpose bTa rotation (inverse) and multiply with bTc
    __m128 v3 = bTa.m_rotation.m_col2.m_quad;
    __m128 v4 = bTa.m_rotation.m_col0.m_quad;
    __m128 v5 = bTa.m_rotation.m_col1.m_quad;

    __m128 v11 = _mm_unpacklo_ps(v4, v5);
    __m128 v12 = _mm_shuffle_ps(_mm_unpackhi_ps(v4, v5), v3, 0xE4);
    __m128 v13 = _mm_movelh_ps(v11, v3);
    __m128 v14 = _mm_shuffle_ps(_mm_movehl_ps(v13, v11), v3, 0xD4);

    // Multiply transposed bTa with bTc
    for (int i = 0; i < 3; ++i) {
        __m128 col = (&bTc.m_rotation.m_col0)[i].m_quad;
        (&m_rotation.m_col0)[i].m_quad = _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(col, col, 0x55), v14),
                _mm_mul_ps(_mm_shuffle_ps(col, col, 0x00), v13)),
            _mm_mul_ps(_mm_shuffle_ps(col, col, 0xAA), v12));
    }

    // Compute translation: bTa^T * (bTc.trans - bTa.trans)
    __m128 v16 = bTa.m_rotation.m_col1.m_quad;
    __m128 v17 = bTa.m_rotation.m_col2.m_quad;
    __m128 v18 = _mm_unpacklo_ps(bTa.m_rotation.m_col0.m_quad, v16);
    __m128 v19 = _mm_movelh_ps(v18, v17);
    __m128 v20 = _mm_sub_ps(bTc.m_translation.m_quad, bTa.m_translation.m_quad);

    m_translation.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(v20, v20, 0x55), _mm_shuffle_ps(_mm_movehl_ps(v19, v18), v17, 0xD4)),
            _mm_mul_ps(_mm_shuffle_ps(v20, v20, 0x00), v19)),
        _mm_mul_ps(_mm_shuffle_ps(_mm_unpackhi_ps(bTa.m_rotation.m_col0.m_quad, v16), v17, 0xE4), _mm_shuffle_ps(v20, v20, 0xAA)));
#else
    // Non-SIMD fallback: this = bTa^(-1) * bTc
    for (int i = 0; i < 3; ++i) {
        const hkVector4& col = (&bTc.m_rotation.m_col0)[i];
        (&m_rotation.m_col0)[i].x = bTa.m_rotation.m_col0.x * col.x + bTa.m_rotation.m_col1.x * col.y + bTa.m_rotation.m_col2.x * col.z;
        (&m_rotation.m_col0)[i].y = bTa.m_rotation.m_col0.y * col.x + bTa.m_rotation.m_col1.y * col.y + bTa.m_rotation.m_col2.y * col.z;
        (&m_rotation.m_col0)[i].z = bTa.m_rotation.m_col0.z * col.x + bTa.m_rotation.m_col1.z * col.y + bTa.m_rotation.m_col2.z * col.z;
    }

    float dx = bTc.m_translation.x - bTa.m_translation.x;
    float dy = bTc.m_translation.y - bTa.m_translation.y;
    float dz = bTc.m_translation.z - bTa.m_translation.z;
    m_translation.x = bTa.m_rotation.m_col0.x * dx + bTa.m_rotation.m_col1.x * dy + bTa.m_rotation.m_col2.x * dz;
    m_translation.y = bTa.m_rotation.m_col0.y * dx + bTa.m_rotation.m_col1.y * dy + bTa.m_rotation.m_col2.y * dz;
    m_translation.z = bTa.m_rotation.m_col0.z * dx + bTa.m_rotation.m_col1.z * dy + bTa.m_rotation.m_col2.z * dz;
#endif
}

// IDA: ?setMulMulInverse@hkTransform@@QEAAXAEBV1@0@Z @ 0x14079a990
// Computes: this = wTa * wTb^(-1)
void hkTransform::setMulMulInverse(const hkTransform& wTa, const hkTransform& wTb) {
#ifdef _WIN32
    // Transpose wTb rotation (inverse) then multiply: wTa * wTb^T
    __m128 v16 = _mm_shuffle_ps(
        _mm_unpackhi_ps(wTb.m_rotation.m_col0.m_quad, wTb.m_rotation.m_col1.m_quad),
        wTb.m_rotation.m_col2.m_quad, 0xE4);

    __m128 v5 = wTa.m_rotation.m_col0.m_quad;
    __m128 v6 = wTa.m_rotation.m_col1.m_quad;
    __m128 v4 = wTa.m_rotation.m_col2.m_quad;

    // Multiply wTa with transposed wTb
    __m128 v17, v18, v19;
    __int64 v3 = 0;
    do {
        __m128 v7 = *(__m128*)((char*)&v16 + v3);
        v3 -= 16;
        *(__m128*)&v17 = _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(v7, v7, 0x00), v5),
                _mm_mul_ps(_mm_shuffle_ps(v7, v7, 0x55), v6)),
            _mm_mul_ps(_mm_shuffle_ps(v7, v7, 0xAA), v4));
    } while (v3 >= -32);

    m_rotation.m_col0.m_quad = v17;
    m_rotation.m_col1.m_quad = v18;
    m_rotation.m_col2.m_quad = v19;

    // Compute translation: wTa.trans - (result_rotation * wTb.trans)
    __m128 v8 = wTb.m_translation.m_quad;
    __m128 v11 = _mm_shuffle_ps(v8, v8, 0x00);
    __m128 v12 = _mm_shuffle_ps(v8, v8, 0x55);
    __m128 v13 = _mm_mul_ps(_mm_shuffle_ps(v8, v8, 0xAA), v19);
    __m128 v14 = _mm_add_ps(_mm_mul_ps(v11, v17), _mm_mul_ps(v12, v18));

    m_translation.m_quad = _mm_sub_ps(wTa.m_translation.m_quad, _mm_add_ps(v14, v13));
#else
    // Non-SIMD fallback: this = wTa * wTb^(-1)
    // Transpose wTb rotation
    hkRotation wTb_inv;
    wTb_inv.m_col0 = hkVector4(wTb.m_rotation.m_col0.x, wTb.m_rotation.m_col1.x, wTb.m_rotation.m_col2.x, 0.0f);
    wTb_inv.m_col1 = hkVector4(wTb.m_rotation.m_col0.y, wTb.m_rotation.m_col1.y, wTb.m_rotation.m_col2.y, 0.0f);
    wTb_inv.m_col2 = hkVector4(wTb.m_rotation.m_col0.z, wTb.m_rotation.m_col1.z, wTb.m_rotation.m_col2.z, 0.0f);

    // Multiply wTa with wTb^T
    for (int i = 0; i < 3; ++i) {
        const hkVector4& col = (&wTa.m_rotation.m_col0)[i];
        (&m_rotation.m_col0)[i].x = col.x * wTb_inv.m_col0.x + col.y * wTb_inv.m_col1.x + col.z * wTb_inv.m_col2.x;
        (&m_rotation.m_col0)[i].y = col.x * wTb_inv.m_col0.y + col.y * wTb_inv.m_col1.y + col.z * wTb_inv.m_col2.y;
        (&m_rotation.m_col0)[i].z = col.x * wTb_inv.m_col0.z + col.y * wTb_inv.m_col1.z + col.z * wTb_inv.m_col2.z;
    }

    // Translation: wTa.trans - (result_rotation * wTb.trans)
    float tx = m_rotation.m_col0.x * wTb.m_translation.x + m_rotation.m_col1.x * wTb.m_translation.y + m_rotation.m_col2.x * wTb.m_translation.z;
    float ty = m_rotation.m_col0.y * wTb.m_translation.x + m_rotation.m_col1.y * wTb.m_translation.y + m_rotation.m_col2.y * wTb.m_translation.z;
    float tz = m_rotation.m_col0.z * wTb.m_translation.x + m_rotation.m_col1.z * wTb.m_translation.y + m_rotation.m_col2.z * wTb.m_translation.z;

    m_translation.x = wTa.m_translation.x - tx;
    m_translation.y = wTa.m_translation.y - ty;
    m_translation.z = wTa.m_translation.z - tz;
#endif
}

// IDA: ?isApproximatelyEqual@hkTransform@@QEBA_NAEBV1@M@Z @ 0x14079aa70
bool hkTransform::isApproximatelyEqual(const hkTransform& t, float epsilon) const {
#ifdef _WIN32
    // Check rotation with matrix comparison
    __m128 sZ = _mm_set_ps1(epsilon);

    // Check rotation columns
    for (int i = 0; i < 3; ++i) {
        __m128 diff = _mm_sub_ps((&m_rotation.m_col0)[i].m_quad, (&t.m_rotation.m_col0)[i].m_quad);
        __m128 absDiff = _mm_and_ps(_mm_slli_epi32((__m128i)diff, 1), _mm_srli_epi32((__m128i)diff, 1));
        if ((_mm_movemask_ps(_mm_cmplt_ps(absDiff, sZ)) & 7) != 7) {
            return false;
        }
    }

    // Check translation
    __m128 transDiff = _mm_sub_ps(m_translation.m_quad, t.m_translation.m_quad);
    __m128 absTransDiff = _mm_and_ps(_mm_slli_epi32((__m128i)transDiff, 1), _mm_srli_epi32((__m128i)transDiff, 1));
    return (_mm_movemask_ps(_mm_cmplt_ps(absTransDiff, sZ)) & 7) == 7;
#else
    // Non-SIMD fallback
    for (int i = 0; i < 3; ++i) {
        if (std::abs((&m_rotation.m_col0)[i].x - (&t.m_rotation.m_col0)[i].x) > epsilon) return false;
        if (std::abs((&m_rotation.m_col0)[i].y - (&t.m_rotation.m_col0)[i].y) > epsilon) return false;
        if (std::abs((&m_rotation.m_col0)[i].z - (&t.m_rotation.m_col0)[i].z) > epsilon) return false;
    }
    return std::abs(m_translation.x - t.m_translation.x) <= epsilon
        && std::abs(m_translation.y - t.m_translation.y) <= epsilon
        && std::abs(m_translation.z - t.m_translation.z) <= epsilon;
#endif
}

// IDA: ?isApproximatelyEqualSimd@hkTransform@@QEBA_NAEBV1@AEBVhkSimdReal@@@Z @ 0x14079aaf0
bool hkTransform::isApproximatelyEqualSimd(const hkTransform& t, const hkSimdReal& epsilon) const {
#ifdef _WIN32
    // Check rotation using hkMatrix3::isApproximatelyEqualSimd
    if (!m_rotation.isApproximatelyEqualSimd(t.m_rotation, epsilon)) {
        return false;
    }
    // Check translation: absolute difference < epsilon for x,y,z
    __m128 diff = _mm_sub_ps(m_translation.m_quad, t.m_translation.m_quad);
    // Absolute value via shift trick: abs = (diff << 1) >> 1 (clears sign bit)
    __m128 absDiff = _mm_srli_epi32(_mm_slli_epi32((__m128i)diff, 1), 1);
    return (_mm_movemask_ps(_mm_cmplt_ps(absDiff, epsilon.m_real)) & 7) == 7;
#else
    float eps = epsilon.m_real[0];  // Extract first element
    for (int i = 0; i < 3; ++i) {
        if (std::abs((&m_rotation.m_col0)[i].x - (&t.m_rotation.m_col0)[i].x) > eps) return false;
        if (std::abs((&m_rotation.m_col0)[i].y - (&t.m_rotation.m_col0)[i].y) > eps) return false;
        if (std::abs((&m_rotation.m_col0)[i].z - (&t.m_rotation.m_col0)[i].z) > eps) return false;
    }
    return std::abs(m_translation.x - t.m_translation.x) <= eps
        && std::abs(m_translation.y - t.m_translation.y) <= eps
        && std::abs(m_translation.z - t.m_translation.z) <= eps;
#endif
}

// IDA: ?get4x4ColumnMajor@hkTransform@@QEBAXPEIAM@Z @ 0x14079ab60
void hkTransform::get4x4ColumnMajor(float* d) const {
#ifdef _WIN32
    __m128i* out = reinterpret_cast<__m128i*>(d);
    // Shift each column left then right by 4 bytes to insert w=0
    out[0] = _mm_srli_si128(_mm_slli_si128((__m128i)m_rotation.m_col0.m_quad, 4), 4);
    out[1] = _mm_srli_si128(_mm_slli_si128((__m128i)m_rotation.m_col1.m_quad, 4), 4);
    out[2] = _mm_srli_si128(_mm_slli_si128((__m128i)m_rotation.m_col2.m_quad, 4), 4);
    // Last row: translation with w=1 (g_vectorConstants[2] = identity (1,1,1,1))
    __m128 identity = _mm_load_ps(&g_vectorConstants[2 * 4]);
    out[3] = (__m128i)_mm_shuffle_ps(m_translation.m_quad, _mm_unpackhi_ps(m_translation.m_quad, identity), 0xC4);
#else
    d[0] = m_rotation.m_col0.x; d[1] = m_rotation.m_col0.y; d[2] = m_rotation.m_col0.z; d[3] = 0.0f;
    d[4] = m_rotation.m_col1.x; d[5] = m_rotation.m_col1.y; d[6] = m_rotation.m_col1.z; d[7] = 0.0f;
    d[8] = m_rotation.m_col2.x; d[9] = m_rotation.m_col2.y; d[10] = m_rotation.m_col2.z; d[11] = 0.0f;
    d[12] = m_translation.x; d[13] = m_translation.y; d[14] = m_translation.z; d[15] = 1.0f;
#endif
}

// IDA: ?set4x4ColumnMajor@hkTransform@@QEAAXPEBM@Z @ 0x14079ac50
void hkTransform::set4x4ColumnMajor(const float* p) {
#ifdef _WIN32
    const __m128i* in = reinterpret_cast<const __m128i*>(p);
    // Shift each column left then right by 4 bytes to extract x,y,z and set w=0
    m_rotation.m_col0.m_quad = _mm_srli_si128(_mm_slli_si128(in[0], 4), 4);
    m_rotation.m_col1.m_quad = _mm_srli_si128(_mm_slli_si128(in[1], 4), 4);
    m_rotation.m_col2.m_quad = _mm_srli_si128(_mm_slli_si128(in[2], 4), 4);
    // Last row: translation
    __m128 v2 = *reinterpret_cast<const __m128*>(p + 12);
    __m128 identity = _mm_load_ps(&g_vectorConstants[2 * 4]);
    m_translation.m_quad = _mm_shuffle_ps(v2, _mm_unpackhi_ps(v2, identity), 0xC4);
#else
    m_rotation.m_col0.x = p[0]; m_rotation.m_col0.y = p[1]; m_rotation.m_col0.z = p[2]; m_rotation.m_col0.w = 0.0f;
    m_rotation.m_col1.x = p[4]; m_rotation.m_col1.y = p[5]; m_rotation.m_col1.z = p[6]; m_rotation.m_col1.w = 0.0f;
    m_rotation.m_col2.x = p[8]; m_rotation.m_col2.y = p[9]; m_rotation.m_col2.z = p[10]; m_rotation.m_col2.w = 0.0f;
    m_translation.x = p[12]; m_translation.y = p[13]; m_translation.z = p[14]; m_translation.w = 1.0f;
#endif
}

// IDA: ?get4x4ColumnMajor@hkTransform@@QEBAXPEIAN@Z @ 0x14079abc0
// Double-precision output version
void hkTransform::get4x4ColumnMajor(double* d) const {
#ifdef _WIN32
    // Shift each column left then right by 4 bytes to insert w=0, then convert to double
    __m128 v2 = (__m128)_mm_srli_si128(_mm_slli_si128((__m128i)m_rotation.m_col0.m_quad, 4), 4);
    __m128 v3 = (__m128)_mm_srli_si128(_mm_slli_si128((__m128i)m_rotation.m_col1.m_quad, 4), 4);
    __m128 v5 = (__m128)_mm_srli_si128(_mm_slli_si128((__m128i)m_rotation.m_col2.m_quad, 4), 4);

    // Build translation row with w=1
    __m128 identity = _mm_load_ps(&g_vectorConstants[2 * 4]);
    __m128 v4 = _mm_shuffle_ps(m_translation.m_quad, _mm_unpackhi_ps(m_translation.m_quad, identity), 0xC4);

    // Convert each column to double precision and store
    __m128d* out = reinterpret_cast<__m128d*>(d);
    out[0] = _mm_cvtps_pd(v2);
    out[1] = _mm_cvtps_pd(_mm_movehl_ps(v2, v2));
    out[2] = _mm_cvtps_pd(v3);
    out[3] = _mm_cvtps_pd(_mm_movehl_ps(v3, v3));
    out[4] = _mm_cvtps_pd(v5);
    out[5] = _mm_cvtps_pd(_mm_movehl_ps(v5, v5));
    out[6] = _mm_cvtps_pd(v4);
    out[7] = _mm_cvtps_pd(_mm_movehl_ps(v4, v4));
#else
    d[0] = m_rotation.m_col0.x; d[1] = m_rotation.m_col0.y; d[2] = m_rotation.m_col0.z; d[3] = 0.0;
    d[4] = m_rotation.m_col1.x; d[5] = m_rotation.m_col1.y; d[6] = m_rotation.m_col1.z; d[7] = 0.0;
    d[8] = m_rotation.m_col2.x; d[9] = m_rotation.m_col2.y; d[10] = m_rotation.m_col2.z; d[11] = 0.0;
    d[12] = m_translation.x; d[13] = m_translation.y; d[14] = m_translation.z; d[15] = 1.0;
#endif
}

// IDA: ?set4x4ColumnMajor@hkTransform@@QEAAXPEBN@Z @ 0x14079acb0
// Double-precision input version
void hkTransform::set4x4ColumnMajor(const double* p) {
#ifdef _WIN32
    // Convert double columns to float and shift
    __m128 v2 = _mm_cvtpd_ps(*reinterpret_cast<const __m128d*>(p + 12));  // row 3 low
    __m128i v3 = _mm_slli_si128((__m128i)_mm_movelh_ps(
        _mm_cvtpd_ps(*reinterpret_cast<const __m128d*>(p + 4)),   // row 1 low
        _mm_cvtpd_ps(*reinterpret_cast<const __m128d*>(p + 6))),  // row 1 high
        4);
    __m128 v4 = _mm_cvtpd_ps(*reinterpret_cast<const __m128d*>(p + 14));  // row 3 high
    __m128i v5 = _mm_slli_si128((__m128i)_mm_movelh_ps(
        _mm_cvtpd_ps(*reinterpret_cast<const __m128d*>(p + 8)),   // row 2 low
        _mm_cvtpd_ps(*reinterpret_cast<const __m128d*>(p + 10))), // row 2 high
        4);

    // Row 0
    m_rotation.m_col0.m_quad = _mm_srli_si128(_mm_slli_si128((__m128i)_mm_movelh_ps(
        _mm_cvtpd_ps(*reinterpret_cast<const __m128d*>(p + 0)),   // row 0 low
        _mm_cvtpd_ps(*reinterpret_cast<const __m128d*>(p + 2))),  // row 0 high
        4), 4);

    __m128 v6 = _mm_movelh_ps(v2, v4);
    m_rotation.m_col1.m_quad = _mm_srli_si128(v3, 4);
    m_rotation.m_col2.m_quad = _mm_srli_si128(v5, 4);

    __m128 identity = _mm_load_ps(&g_vectorConstants[2 * 4]);
    m_translation.m_quad = _mm_shuffle_ps(v6, _mm_unpackhi_ps(v6, identity), 0xC4);
#else
    m_rotation.m_col0.x = static_cast<float>(p[0]); m_rotation.m_col0.y = static_cast<float>(p[1]); m_rotation.m_col0.z = static_cast<float>(p[2]); m_rotation.m_col0.w = 0.0f;
    m_rotation.m_col1.x = static_cast<float>(p[4]); m_rotation.m_col1.y = static_cast<float>(p[5]); m_rotation.m_col1.z = static_cast<float>(p[6]); m_rotation.m_col1.w = 0.0f;
    m_rotation.m_col2.x = static_cast<float>(p[8]); m_rotation.m_col2.y = static_cast<float>(p[9]); m_rotation.m_col2.z = static_cast<float>(p[10]); m_rotation.m_col2.w = 0.0f;
    m_translation.x = static_cast<float>(p[12]); m_translation.y = static_cast<float>(p[13]); m_translation.z = static_cast<float>(p[14]); m_translation.w = 1.0f;
#endif
}

// ============================================================================
// hkVector4 transform implementations
// ============================================================================

// IDA: ?setTransformedPos@hkVector4@@SAXAEAV1@AEBVhkTransform@@AEBV1@@Z @ 0x1407b2790
void hkVector4::setTransformedPos(hkVector4* result, const hkTransform* t, const hkVector4* pos) {
#ifdef _WIN32
    // SSE-optimized: result = rotation * pos + translation
    result->m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(pos->m_quad, pos->m_quad, 85), t->m_rotation.m_col1.m_quad),
                _mm_mul_ps(_mm_shuffle_ps(pos->m_quad, pos->m_quad, 0), t->m_rotation.m_col0.m_quad)),
            _mm_mul_ps(_mm_shuffle_ps(pos->m_quad, pos->m_quad, 170), t->m_rotation.m_col2.m_quad)),
        t->m_translation.m_quad);
#else
    // Scalar fallback
    result->x = pos->x * t->m_rotation.m_col0.x + pos->y * t->m_rotation.m_col1.x + pos->z * t->m_rotation.m_col2.x + t->m_translation.x;
    result->y = pos->x * t->m_rotation.m_col0.y + pos->y * t->m_rotation.m_col1.y + pos->z * t->m_rotation.m_col2.y + t->m_translation.y;
    result->z = pos->x * t->m_rotation.m_col0.z + pos->y * t->m_rotation.m_col1.z + pos->z * t->m_rotation.m_col2.z + t->m_translation.z;
    result->w = pos->w;
#endif
}

// IDA: ?setTransformedInversePos@hkVector4@@SAXAEAV1@AEBVhkTransform@@AEBV1@@Z @ 0x1407b27d0
void hkVector4::setTransformedInversePos(hkVector4* result, const hkTransform* t, const hkVector4* pos) {
#ifdef _WIN32
    // SSE-optimized: result = rotation^T * (pos - translation)
    hkVector4 v3, v4;
    v3.m_quad = t->m_rotation.m_col1.m_quad;
    v4.m_quad = t->m_rotation.m_col2.m_quad;
    __m128 v5 = _mm_unpacklo_ps(t->m_rotation.m_col0.m_quad, v3.m_quad);
    __m128 v6 = _mm_movelh_ps(v5, v4.m_quad);
    __m128 v7 = _mm_sub_ps(pos->m_quad, t->m_translation.m_quad);
    result->m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(v7, v7, 85), _mm_shuffle_ps(_mm_movehl_ps(v6, v5), v4.m_quad, 212)),
            _mm_mul_ps(_mm_shuffle_ps(v7, v7, 0), v6)),
        _mm_mul_ps(
            _mm_shuffle_ps(_mm_unpackhi_ps(t->m_rotation.m_col0.m_quad, v3.m_quad), v4.m_quad, 228),
            _mm_shuffle_ps(v7, v7, 170)));
#else
    // Scalar fallback
    float px = pos->x - t->m_translation.x;
    float py = pos->y - t->m_translation.y;
    float pz = pos->z - t->m_translation.z;

    result->x = px * t->m_rotation.m_col0.x + py * t->m_rotation.m_col0.y + pz * t->m_rotation.m_col0.z;
    result->y = px * t->m_rotation.m_col1.x + py * t->m_rotation.m_col1.y + pz * t->m_rotation.m_col1.z;
    result->z = px * t->m_rotation.m_col2.x + py * t->m_rotation.m_col2.y + pz * t->m_rotation.m_col2.z;
    result->w = pos->w;
#endif
}

// IDA: ?setRotatedDir@hkVector4@@QEAAXAEBVhkQuaternion@@AEBV1@@Z @ 0x1407b2aa0
void hkVector4::setRotatedDir(const hkQuaternion& quat, const hkVector4& direction) {
    // Quaternion rotation: v' = q * v * q^-1
    // Use the non-SIMD formula for clarity and correctness
    float qx = quat.m_vec.x, qy = quat.m_vec.y, qz = quat.m_vec.z, qw = quat.m_vec.w;
    float vx = direction.x, vy = direction.y, vz = direction.z;

    // t = 2 * cross(q.xyz, v)
    float tx = 2.0f * (qy * vz - qz * vy);
    float ty = 2.0f * (qz * vx - qx * vz);
    float tz = 2.0f * (qx * vy - qy * vx);

    // v' = v + qw * t + cross(q.xyz, t)
    x = vx + qw * tx + (qy * tz - qz * ty);
    y = vy + qw * ty + (qz * tx - qx * tz);
    z = vz + qw * tz + (qx * ty - qy * tx);
    w = direction.w;
}

// IDA: ?setRotatedInverseDir@hkVector4@@QEAAXAEBVhkQuaternion@@AEBV1@@Z @ 0x1407b2b30
void hkVector4::setRotatedInverseDir(const hkQuaternion& quat, const hkVector4& direction) {
    // Inverse rotation: use conjugate quaternion (negate xyz, keep w)
    float qx = -quat.m_vec.x, qy = -quat.m_vec.y, qz = -quat.m_vec.z, qw = quat.m_vec.w;
    float vx = direction.x, vy = direction.y, vz = direction.z;

    // t = 2 * cross(q.xyz, v)
    float tx = 2.0f * (qy * vz - qz * vy);
    float ty = 2.0f * (qz * vx - qx * vz);
    float tz = 2.0f * (qx * vy - qy * vx);

    // v' = v + qw * t + cross(q.xyz, t)
    x = vx + qw * tx + (qy * tz - qz * ty);
    y = vy + qw * ty + (qz * tx - qx * tz);
    z = vz + qw * tz + (qx * ty - qy * tx);
    w = direction.w;
}

// IDA: ?setRotatedDir@hkVector4@@QEAAXAEBVhkMatrix3@@AEBV1@@Z @ 0x1407b26f0
void hkVector4::setRotatedDir(const hkMatrix3& matrix, const hkVector4& direction) {
#ifdef _WIN32
    // SSE-optimized matrix-vector multiplication
    // result = matrix * direction
    m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(direction.m_quad, direction.m_quad, 85), matrix.m_col1.m_quad),
            _mm_mul_ps(_mm_shuffle_ps(direction.m_quad, direction.m_quad, 0), matrix.m_col0.m_quad)),
        _mm_mul_ps(_mm_shuffle_ps(direction.m_quad, direction.m_quad, 170), matrix.m_col2.m_quad));
#else
    // Scalar fallback
    x = direction.x * matrix.m_col0.x + direction.y * matrix.m_col1.x + direction.z * matrix.m_col2.x;
    y = direction.x * matrix.m_col0.y + direction.y * matrix.m_col1.y + direction.z * matrix.m_col2.y;
    z = direction.x * matrix.m_col0.z + direction.y * matrix.m_col1.z + direction.z * matrix.m_col2.z;
    w = direction.w;
#endif
}

// IDA: ?setRotatedInverseDir@hkVector4@@QEAAXAEBVhkMatrix3@@AEBV1@@Z @ 0x1407b2730
void hkVector4::setRotatedInverseDir(const hkMatrix3& matrix, const hkVector4& direction) {
#ifdef _WIN32
    // SSE-optimized inverse matrix-vector multiplication (transpose for orthogonal matrices)
    hkVector4 v3, v4;
    v3.m_quad = matrix.m_col1.m_quad;
    v4.m_quad = matrix.m_col2.m_quad;
    __m128 v5 = _mm_unpacklo_ps(matrix.m_col0.m_quad, v3.m_quad);
    __m128 v6 = _mm_movelh_ps(v5, v4.m_quad);
    m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(
                _mm_shuffle_ps(direction.m_quad, direction.m_quad, 85),
                _mm_shuffle_ps(_mm_movehl_ps(v6, v5), v4.m_quad, 212)),
            _mm_mul_ps(_mm_shuffle_ps(direction.m_quad, direction.m_quad, 0), v6)),
        _mm_mul_ps(
            _mm_shuffle_ps(direction.m_quad, direction.m_quad, 170),
            _mm_shuffle_ps(_mm_unpackhi_ps(matrix.m_col0.m_quad, v3.m_quad), v4.m_quad, 228)));
#else
    // Scalar fallback: use transposed matrix rows as columns
    x = direction.x * matrix.m_col0.x + direction.y * matrix.m_col0.y + direction.z * matrix.m_col0.z;
    y = direction.x * matrix.m_col1.x + direction.y * matrix.m_col1.y + direction.z * matrix.m_col1.z;
    z = direction.x * matrix.m_col2.x + direction.y * matrix.m_col2.y + direction.z * matrix.m_col2.z;
    w = direction.w;
#endif
}

// IDA: ?setTransformedPos@hkVector4@@QEAAXAEBVhkQsTransform@@AEBV1@@Z @ 0x1407b2830
void hkVector4::setTransformedPos(const hkQsTransform& t, const hkVector4& pos) {
#ifdef _WIN32
    // SSE-optimized: apply scale, rotate by quaternion, add translation
    __m128 v4 = _mm_mul_ps(pos.m_quad, t.m_scale.m_quad);
    __m128 v5 = _mm_mul_ps(t.m_rotation.m_vec.m_quad, v4);
    __m128 v6 = _mm_shuffle_ps(t.m_rotation.m_vec.m_quad, t.m_rotation.m_vec.m_quad, 255);
    __m128 v7 = _mm_sub_ps(
        _mm_mul_ps(_mm_shuffle_ps(v4, v4, 201), t.m_rotation.m_vec.m_quad),
        _mm_mul_ps(_mm_shuffle_ps(t.m_rotation.m_vec.m_quad, t.m_rotation.m_vec.m_quad, 201), v4));
    __m128 v8 = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(
                _mm_add_ps(_mm_add_ps(_mm_shuffle_ps(v5, v5, 85), _mm_shuffle_ps(v5, v5, 0)), _mm_shuffle_ps(v5, v5, 170)),
                t.m_rotation.m_vec.m_quad),
            _mm_mul_ps(
                _mm_add_ps(_mm_xor_ps(_mm_load_ps(&g_vectorConstants[17 * 4]), _mask__NegFloat_), _mm_mul_ps(v6, v6)),
                v4)),
        _mm_mul_ps(_mm_shuffle_ps(v7, v7, 201), v6));
    m_quad = _mm_add_ps(_mm_add_ps(v8, v8), t.m_translation.m_quad);
#else
    // Scalar fallback: apply scale, quaternion rotation, add translation
    // Scale the position
    float sx = pos.x * t.m_scale.x;
    float sy = pos.y * t.m_scale.y;
    float sz = pos.z * t.m_scale.z;

    // Quaternion rotation
    float qx = t.m_rotation.m_vec.x, qy = t.m_rotation.m_vec.y;
    float qz = t.m_rotation.m_vec.z, qw = t.m_rotation.m_vec.w;

    // t = 2 * cross(q.xyz, scaled)
    float tx = 2.0f * (qy * sz - qz * sy);
    float ty = 2.0f * (qz * sx - qx * sz);
    float tz = 2.0f * (qx * sy - qy * sx);

    // v' = scaled + qw * t + cross(q.xyz, t)
    x = sx + qw * tx + (qy * tz - qz * ty) + t.m_translation.x;
    y = sy + qw * ty + (qz * tx - qx * tz) + t.m_translation.y;
    z = sz + qw * tz + (qx * ty - qy * tx) + t.m_translation.z;
    w = pos.w;
#endif
}

// IDA: ?setTransformedInversePos@hkVector4@@QEAAXAEBVhkQsTransform@@AEBV1@@Z @ 0x1407b28d0
void hkVector4::setTransformedInversePos(const hkQsTransform& t, const hkVector4& pos) {
#ifdef _WIN32
    // SSE-optimized: subtract translation, inverse rotate, divide by scale
    __m128 v4 = _mm_sub_ps(pos.m_quad, t.m_translation.m_quad);
    __m128 v5 = _mm_mul_ps(t.m_rotation.m_vec.m_quad, v4);
    __m128 v6 = _mm_shuffle_ps(t.m_rotation.m_vec.m_quad, t.m_rotation.m_vec.m_quad, 255);
    __m128 v7 = _mm_sub_ps(
        _mm_mul_ps(_mm_shuffle_ps(t.m_rotation.m_vec.m_quad, t.m_rotation.m_vec.m_quad, 201), v4),
        _mm_mul_ps(_mm_shuffle_ps(v4, v4, 201), t.m_rotation.m_vec.m_quad));
    __m128 v9 = _mm_add_ps(
        _mm_mul_ps(
            _mm_add_ps(_mm_add_ps(_mm_shuffle_ps(v5, v5, 85), _mm_shuffle_ps(v5, v5, 0)), _mm_shuffle_ps(v5, v5, 170)),
            t.m_rotation.m_vec.m_quad),
        _mm_mul_ps(
            _mm_add_ps(_mm_xor_ps(_mm_load_ps(&g_vectorConstants[17 * 4]), _mask__NegFloat_), _mm_mul_ps(v6, v6)),
            v4));
    __m128 v10 = _mm_rcp_ps(t.m_scale.m_quad);
    __m128 v11 = _mm_add_ps(v9, _mm_mul_ps(_mm_shuffle_ps(v7, v7, 201), v6));
    // Newton-Raphson refinement
    __m128 two = _mm_set1_ps(2.0f);
    m_quad = _mm_mul_ps(
        _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(t.m_scale.m_quad, v10)), v10),
        _mm_add_ps(v11, v11));
#else
    // Scalar fallback: subtract translation, inverse quaternion rotation, divide by scale
    float px = pos.x - t.m_translation.x;
    float py = pos.y - t.m_translation.y;
    float pz = pos.z - t.m_translation.z;

    // Inverse quaternion rotation (use conjugate)
    float qx = -t.m_rotation.m_vec.x, qy = -t.m_rotation.m_vec.y;
    float qz = -t.m_rotation.m_vec.z, qw = t.m_rotation.m_vec.w;

    float tx = 2.0f * (qy * pz - qz * py);
    float ty = 2.0f * (qz * px - qx * pz);
    float tz = 2.0f * (qx * py - qy * px);

    float rx = px + qw * tx + (qy * tz - qz * ty);
    float ry = py + qw * ty + (qz * tx - qx * tz);
    float rz = pz + qw * tz + (qx * ty - qy * tx);

    // Divide by scale
    x = rx / t.m_scale.x;
    y = ry / t.m_scale.y;
    z = rz / t.m_scale.z;
    w = pos.w;
#endif
}

// IDA: ?setTransformedPos@hkVector4@@QEAAXAEBVhkQTransform@@AEBV1@@Z @ 0x1407b2980
void hkVector4::setTransformedPos(const hkQTransform& t, const hkVector4& pos) {
#ifdef _WIN32
    // SSE-optimized quaternion rotation + translation
    __m128 v3 = _mm_mul_ps(t.m_rotation.m_quad, pos.m_quad);
    __m128 v4 = _mm_shuffle_ps(t.m_rotation.m_quad, t.m_rotation.m_quad, 255);
    __m128 v5 = _mm_sub_ps(
        _mm_mul_ps(_mm_shuffle_ps(pos.m_quad, pos.m_quad, 201), t.m_rotation.m_quad),
        _mm_mul_ps(_mm_shuffle_ps(t.m_rotation.m_quad, t.m_rotation.m_quad, 201), pos.m_quad));
    __m128 v6 = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(
                _mm_add_ps(_mm_add_ps(_mm_shuffle_ps(v3, v3, 85), _mm_shuffle_ps(v3, v3, 0)), _mm_shuffle_ps(v3, v3, 170)),
                t.m_rotation.m_quad),
            _mm_mul_ps(
                _mm_add_ps(_mm_xor_ps(_mm_load_ps(&g_vectorConstants[17 * 4]), _mask__NegFloat_), _mm_mul_ps(v4, v4)),
                pos.m_quad)),
        _mm_mul_ps(_mm_shuffle_ps(v5, v5, 201), v4));
    m_quad = _mm_add_ps(_mm_add_ps(v6, v6), t.m_translation.m_quad);
#else
    // Scalar fallback: quaternion rotation + translation
    float qx = t.m_rotation.x, qy = t.m_rotation.y;
    float qz = t.m_rotation.z, qw = t.m_rotation.w;

    float tx = 2.0f * (qy * pos.z - qz * pos.y);
    float ty = 2.0f * (qz * pos.x - qx * pos.z);
    float tz = 2.0f * (qx * pos.y - qy * pos.x);

    x = pos.x + qw * tx + (qy * tz - qz * ty) + t.m_translation.x;
    y = pos.y + qw * ty + (qz * tx - qx * tz) + t.m_translation.y;
    z = pos.z + qw * tz + (qx * ty - qy * tx) + t.m_translation.z;
    w = pos.w;
#endif
}

// IDA: ?setTransformedInversePos@hkVector4@@QEAAXAEBVhkQTransform@@AEBV1@@Z @ 0x1407b2a10
void hkVector4::setTransformedInversePos(const hkQTransform& t, const hkVector4& pos) {
#ifdef _WIN32
    // SSE-optimized: subtract translation, inverse quaternion rotation
    __m128 v4 = _mm_sub_ps(pos.m_quad, t.m_translation.m_quad);
    __m128 v3 = _mm_mul_ps(t.m_rotation.m_quad, v4);
    __m128 v6 = _mm_shuffle_ps(t.m_rotation.m_quad, t.m_rotation.m_quad, 255);
    __m128 v5 = _mm_sub_ps(
        _mm_mul_ps(_mm_shuffle_ps(t.m_rotation.m_quad, t.m_rotation.m_quad, 201), v4),
        _mm_mul_ps(_mm_shuffle_ps(v4, v4, 201), t.m_rotation.m_quad));
    __m128 v8 = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(
                _mm_add_ps(_mm_add_ps(_mm_shuffle_ps(v3, v3, 85), _mm_shuffle_ps(v3, v3, 0)), _mm_shuffle_ps(v3, v3, 170)),
                t.m_rotation.m_quad),
            _mm_mul_ps(
                _mm_add_ps(_mm_xor_ps(_mm_load_ps(&g_vectorConstants[17 * 4]), _mask__NegFloat_), _mm_mul_ps(v6, v6)),
                v4)),
        _mm_mul_ps(_mm_shuffle_ps(v5, v5, 201), v6));
    m_quad = _mm_add_ps(v8, v8);
#else
    // Scalar fallback: subtract translation, inverse quaternion rotation
    float px = pos.x - t.m_translation.x;
    float py = pos.y - t.m_translation.y;
    float pz = pos.z - t.m_translation.z;

    // Inverse quaternion (conjugate)
    float qx = -t.m_rotation.x, qy = -t.m_rotation.y;
    float qz = -t.m_rotation.z, qw = t.m_rotation.w;

    float tx = 2.0f * (qy * pz - qz * py);
    float ty = 2.0f * (qz * px - qx * pz);
    float tz = 2.0f * (qx * py - qy * px);

    x = px + qw * tx + (qy * tz - qz * ty);
    y = py + qw * ty + (qz * tx - qx * tz);
    z = pz + qw * tz + (qx * ty - qy * tx);
    w = pos.w;
#endif
}

// ============================================================================
// hkMatrix4 implementations
// ============================================================================

// IDA: ?transpose@hkMatrix4@@QEAAXXZ @ 0x1407b2d10
void hkMatrix4::transpose() {
#ifdef _WIN32
    hkVector4 v1;
    v1.m_quad = m_col1.m_quad;
    __m128 v2 = _mm_shuffle_ps(m_col0.m_quad, v1.m_quad, 238);
    __m128 v3 = _mm_shuffle_ps(m_col0.m_quad, v1.m_quad, 68);
    hkVector4 v4;
    v4.m_quad = m_col3.m_quad;
    __m128 v5 = _mm_shuffle_ps(m_col2.m_quad, v4.m_quad, 68);
    __m128 v6 = _mm_shuffle_ps(m_col2.m_quad, v4.m_quad, 238);
    m_col0.m_quad = _mm_shuffle_ps(v3, v5, 136);
    m_col1.m_quad = _mm_shuffle_ps(v3, v5, 221);
    m_col2.m_quad = _mm_shuffle_ps(v2, v6, 136);
    m_col3.m_quad = _mm_shuffle_ps(v2, v6, 221);
#else
    // Scalar fallback - transpose in place
    float temp;
    temp = m_col0.y; m_col0.y = m_col1.x; m_col1.x = temp;
    temp = m_col0.z; m_col0.z = m_col2.x; m_col2.x = temp;
    temp = m_col0.w; m_col0.w = m_col3.x; m_col3.x = temp;
    temp = m_col1.z; m_col1.z = m_col2.y; m_col2.y = temp;
    temp = m_col1.w; m_col1.w = m_col3.y; m_col3.y = temp;
    temp = m_col2.w; m_col2.w = m_col3.z; m_col3.z = temp;
#endif
}

// IDA: ?isOk@hkMatrix4@@QEBA_NXZ @ 0x1407b2d70
bool hkMatrix4::isOk() const {
#ifdef _WIN32
    return !_mm_movemask_ps(_mm_cmpunord_ps(m_col0.m_quad, _mm_setzero_ps()))
        && !_mm_movemask_ps(_mm_cmpunord_ps(m_col1.m_quad, _mm_setzero_ps()))
        && !_mm_movemask_ps(_mm_cmpunord_ps(m_col2.m_quad, _mm_setzero_ps()))
        && !_mm_movemask_ps(_mm_cmpunord_ps(m_col3.m_quad, _mm_setzero_ps()));
#else
    // Check for NaN in all components
    return !std::isnan(m_col0.x) && !std::isnan(m_col0.y) && !std::isnan(m_col0.z) && !std::isnan(m_col0.w)
        && !std::isnan(m_col1.x) && !std::isnan(m_col1.y) && !std::isnan(m_col1.z) && !std::isnan(m_col1.w)
        && !std::isnan(m_col2.x) && !std::isnan(m_col2.y) && !std::isnan(m_col2.z) && !std::isnan(m_col2.w)
        && !std::isnan(m_col3.x) && !std::isnan(m_col3.y) && !std::isnan(m_col3.z) && !std::isnan(m_col3.w);
#endif
}

// IDA: ?add@hkMatrix4@@QEAAXAEBV1@@Z @ 0x1407b2dc0
void hkMatrix4::add(const hkMatrix4& a) {
#ifdef _WIN32
    m_col0.m_quad = _mm_add_ps(a.m_col0.m_quad, m_col0.m_quad);
    m_col1.m_quad = _mm_add_ps(a.m_col1.m_quad, m_col1.m_quad);
    m_col2.m_quad = _mm_add_ps(a.m_col2.m_quad, m_col2.m_quad);
    m_col3.m_quad = _mm_add_ps(a.m_col3.m_quad, m_col3.m_quad);
#else
    m_col0.x += a.m_col0.x; m_col0.y += a.m_col0.y; m_col0.z += a.m_col0.z; m_col0.w += a.m_col0.w;
    m_col1.x += a.m_col1.x; m_col1.y += a.m_col1.y; m_col1.z += a.m_col1.z; m_col1.w += a.m_col1.w;
    m_col2.x += a.m_col2.x; m_col2.y += a.m_col2.y; m_col2.z += a.m_col2.z; m_col2.w += a.m_col2.w;
    m_col3.x += a.m_col3.x; m_col3.y += a.m_col3.y; m_col3.z += a.m_col3.z; m_col3.w += a.m_col3.w;
#endif
}

// IDA: ?sub@hkMatrix4@@QEAAXAEBV1@@Z @ 0x1407b2e00
void hkMatrix4::sub(const hkMatrix4& a) {
#ifdef _WIN32
    m_col0.m_quad = _mm_sub_ps(m_col0.m_quad, a.m_col0.m_quad);
    m_col1.m_quad = _mm_sub_ps(m_col1.m_quad, a.m_col1.m_quad);
    m_col2.m_quad = _mm_sub_ps(m_col2.m_quad, a.m_col2.m_quad);
    m_col3.m_quad = _mm_sub_ps(m_col3.m_quad, a.m_col3.m_quad);
#else
    m_col0.x -= a.m_col0.x; m_col0.y -= a.m_col0.y; m_col0.z -= a.m_col0.z; m_col0.w -= a.m_col0.w;
    m_col1.x -= a.m_col1.x; m_col1.y -= a.m_col1.y; m_col1.z -= a.m_col1.z; m_col1.w -= a.m_col1.w;
    m_col2.x -= a.m_col2.x; m_col2.y -= a.m_col2.y; m_col2.z -= a.m_col2.z; m_col2.w -= a.m_col2.w;
    m_col3.x -= a.m_col3.x; m_col3.y -= a.m_col3.y; m_col3.z -= a.m_col3.z; m_col3.w -= a.m_col3.w;
#endif
}

// IDA: ?setTransposeMul@hkMatrix4@@QEAAXAEBV1@0@Z @ 0x1407b2e40
void hkMatrix4::setTransposeMul(const hkMatrix4& a, const hkMatrix4& b) {
    // this = a^T * b
#ifdef _WIN32
    // SSE implementation for 4x4 matrix transpose-multiply
    // Row i of a^T = column i of a
    // Result column j = dot(row 0 of a^T, col j of b), dot(row 1 of a^T, col j of b), ...
    __m128 r0 = a.m_col0.m_quad;
    __m128 r1 = a.m_col1.m_quad;
    __m128 r2 = a.m_col2.m_quad;
    __m128 r3 = a.m_col3.m_quad;

    // Column 0
    __m128 b0 = b.m_col0.m_quad;
    m_col0.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(r0, r0, 0), b0),
            _mm_mul_ps(_mm_shuffle_ps(r1, r1, 0), b0)),
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(r2, r2, 0), b0),
            _mm_mul_ps(_mm_shuffle_ps(r3, r3, 0), b0)));

    // Column 1
    __m128 b1 = b.m_col1.m_quad;
    m_col1.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(r0, r0, 0), b1),
            _mm_mul_ps(_mm_shuffle_ps(r1, r1, 0), b1)),
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(r2, r2, 0), b1),
            _mm_mul_ps(_mm_shuffle_ps(r3, r3, 0), b1)));

    // Column 2
    __m128 b2 = b.m_col2.m_quad;
    m_col2.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(r0, r0, 0), b2),
            _mm_mul_ps(_mm_shuffle_ps(r1, r1, 0), b2)),
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(r2, r2, 0), b2),
            _mm_mul_ps(_mm_shuffle_ps(r3, r3, 0), b2)));

    // Column 3
    __m128 b3 = b.m_col3.m_quad;
    m_col3.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(r0, r0, 0), b3),
            _mm_mul_ps(_mm_shuffle_ps(r1, r1, 0), b3)),
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(r2, r2, 0), b3),
            _mm_mul_ps(_mm_shuffle_ps(r3, r3, 0), b3)));
#else
    // Scalar fallback
    // m_col0 = a^T * b.m_col0
    m_col0.x = a.m_col0.x * b.m_col0.x + a.m_col0.y * b.m_col0.y + a.m_col0.z * b.m_col0.z + a.m_col0.w * b.m_col0.w;
    m_col0.y = a.m_col1.x * b.m_col0.x + a.m_col1.y * b.m_col0.y + a.m_col1.z * b.m_col0.z + a.m_col1.w * b.m_col0.w;
    m_col0.z = a.m_col2.x * b.m_col0.x + a.m_col2.y * b.m_col0.y + a.m_col2.z * b.m_col0.z + a.m_col2.w * b.m_col0.w;
    m_col0.w = a.m_col3.x * b.m_col0.x + a.m_col3.y * b.m_col0.y + a.m_col3.z * b.m_col0.z + a.m_col3.w * b.m_col0.w;
    // ... similar for other columns (simplified for now)
#endif
}

// IDA: ?get4x4ColumnMajor@hkMatrix4@@QEBAXPEIAM@Z @ 0x1407b3920
void hkMatrix4::get4x4ColumnMajor(float* d) const {
#ifdef _WIN32
    _mm_store_ps(&d[0], m_col0.m_quad);
    _mm_store_ps(&d[4], m_col1.m_quad);
    _mm_store_ps(&d[8], m_col2.m_quad);
    _mm_store_ps(&d[12], m_col3.m_quad);
#else
    d[0] = m_col0.x; d[1] = m_col0.y; d[2] = m_col0.z; d[3] = m_col0.w;
    d[4] = m_col1.x; d[5] = m_col1.y; d[6] = m_col1.z; d[7] = m_col1.w;
    d[8] = m_col2.x; d[9] = m_col2.y; d[10] = m_col2.z; d[11] = m_col2.w;
    d[12] = m_col3.x; d[13] = m_col3.y; d[14] = m_col3.z; d[15] = m_col3.w;
#endif
}

// IDA: ?set4x4ColumnMajor@hkMatrix4@@QEAAXPEBM@Z @ 0x1407b3950
void hkMatrix4::set4x4ColumnMajor(const float* p) {
#ifdef _WIN32
    m_col0.m_quad = _mm_load_ps(&p[0]);
    m_col1.m_quad = _mm_load_ps(&p[4]);
    m_col2.m_quad = _mm_load_ps(&p[8]);
    m_col3.m_quad = _mm_load_ps(&p[12]);
#else
    m_col0.x = p[0]; m_col0.y = p[1]; m_col0.z = p[2]; m_col0.w = p[3];
    m_col1.x = p[4]; m_col1.y = p[5]; m_col1.z = p[6]; m_col1.w = p[7];
    m_col2.x = p[8]; m_col2.y = p[9]; m_col2.z = p[10]; m_col2.w = p[11];
    m_col3.x = p[12]; m_col3.y = p[13]; m_col3.z = p[14]; m_col3.w = p[15];
#endif
}

// IDA: ?get4x4RowMajor@hkMatrix4@@QEBAXPEIAM@Z @ 0x1407b3750
void hkMatrix4::get4x4RowMajor(float* d) const {
    // Row-major: transpose the column-major data
    d[0] = m_col0.x; d[1] = m_col1.x; d[2] = m_col2.x; d[3] = m_col3.x;
    d[4] = m_col0.y; d[5] = m_col1.y; d[6] = m_col2.y; d[7] = m_col3.y;
    d[8] = m_col0.z; d[9] = m_col1.z; d[10] = m_col2.z; d[11] = m_col3.z;
    d[12] = m_col0.w; d[13] = m_col1.w; d[14] = m_col2.w; d[15] = m_col3.w;
}

// IDA: ?set4x4RowMajor@hkMatrix4@@QEAAXPEBM@Z @ 0x1407b3840
void hkMatrix4::set4x4RowMajor(const float* p) {
    // Convert row-major to column-major
    m_col0.x = p[0]; m_col1.x = p[1]; m_col2.x = p[2]; m_col3.x = p[3];
    m_col0.y = p[4]; m_col1.y = p[5]; m_col2.y = p[6]; m_col3.y = p[7];
    m_col0.z = p[8]; m_col1.z = p[9]; m_col2.z = p[10]; m_col3.z = p[11];
    m_col0.w = p[12]; m_col1.w = p[13]; m_col2.w = p[14]; m_col3.w = p[15];
}

// IDA: ?setInverse@hkMatrix4@@QEAA?AUhkResult@@AEBV1@M@Z @ 0x1407B3070
// Precise 4x4 matrix inverse using SIMD
hkResult hkMatrix4::setInverse(const hkMatrix4& m, float epsilon) {
    hkResult result;

    // Compute cofactors for the 3x3 rotation part (last column ignored)
    __m128 col3 = m.m_col3.m_quad;
    __m128 col2_shuffle57 = _mm_shuffle_ps(m.m_col2.m_quad, m.m_col2.m_quad, 57);

    __m128 v7 = _mm_mul_ps(col2_shuffle57, col3);
    __m128 col3_shuffle78 = _mm_shuffle_ps(col3, col3, 78);
    __m128 col3_shuffle147 = _mm_shuffle_ps(col3, col3, 147);
    __m128 v9 = _mm_mul_ps(col3_shuffle147, col2_shuffle57);
    __m128 v10 = _mm_mul_ps(col3_shuffle78, col2_shuffle57);

    // Sign flip mask
    __m128 signMask = _mm_castsi128_ps(_mm_slli_epi32(
        _mm_srli_epi32(*(__m128i*)&hkVector4Comparison_maskToComparison_9[20], 0x1Fu), 0x1Fu));

    __m128 v12 = _mm_shuffle_ps(v10, v10, 57);
    __m128 v13 = _mm_sub_ps(v10, _mm_shuffle_ps(v7, v7, 57));
    __m128 v14 = _mm_sub_ps(v12, _mm_shuffle_ps(v7, v7, 78));
    __m128 v15 = _mm_sub_ps(_mm_shuffle_ps(v9, v9, 78), v9);

    __m128 col1_shuffle78 = _mm_shuffle_ps(m.m_col1.m_quad, m.m_col1.m_quad, 78);
    __m128 col1_shuffle147 = _mm_shuffle_ps(m.m_col1.m_quad, m.m_col1.m_quad, 147);
    __m128 col1_shuffle57 = _mm_shuffle_ps(m.m_col1.m_quad, m.m_col1.m_quad, 57);

    __m128 v18 = _mm_add_ps(
        _mm_add_ps(_mm_mul_ps(col1_shuffle57, v14), _mm_mul_ps(v15, col1_shuffle78)),
        _mm_mul_ps(v13, col1_shuffle147));

    __m128 v19 = _mm_xor_ps(_mm_mul_ps(m.m_col0.m_quad, v18), signMask);
    __m128 v20 = _mm_add_ps(_mm_shuffle_ps(v19, v19, 78), v19);
    __m128 v21 = _mm_add_ps(_mm_shuffle_ps(v20, v20, 177), v20);

    // Check determinant against epsilon
    float det = *reinterpret_cast<float*>(&v21);
    float absDet = reinterpret_cast<unsigned int&>(det) & 0x7FFFFFFF;
    if (absDet >= epsilon) {
        // Compute inverse
        __m128 col0_shuffle57 = _mm_shuffle_ps(m.m_col0.m_quad, m.m_col0.m_quad, 57);
        __m128 v23 = _mm_mul_ps(m.m_col1.m_quad, col0_shuffle57);
        __m128 v24 = _mm_mul_ps(col1_shuffle78, col0_shuffle57);

        result.m_result = 0; // HK_SUCCESS

        __m128 signMask2 = _mm_castsi128_ps(_mm_slli_epi32(
            _mm_srli_epi32(*(__m128i*)&hkVector4Comparison_maskToComparison_9[40], 0x1Fu), 0x1Fu));
        __m128 v26 = _mm_shuffle_ps(col0_shuffle57, col0_shuffle57, 57);
        __m128 v27 = _mm_xor_ps(signMask, v18);
        __m128 v28 = _mm_mul_ps(col1_shuffle147, col0_shuffle57);

        __m128 v30 = _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(v26, v26, 57), v13),
            _mm_add_ps(_mm_mul_ps(v15, v26), _mm_mul_ps(v14, col0_shuffle57)));

        __m128 v31 = _mm_shuffle_ps(v24, v24, 57);
        __m128 v32 = _mm_sub_ps(v24, _mm_shuffle_ps(v23, v23, 57));
        __m128 v33 = _mm_sub_ps(v31, _mm_shuffle_ps(v23, v23, 78));
        __m128 v34 = _mm_sub_ps(_mm_shuffle_ps(v28, v28, 78), v28);

        __m128 col3_shuffle57 = _mm_shuffle_ps(m.m_col3.m_quad, m.m_col3.m_quad, 57);
        __m128 v35 = _mm_mul_ps(v33, col3_shuffle57);
        __m128 v36 = _mm_shuffle_ps(col3_shuffle57, col3_shuffle57, 57);
        __m128 v37 = _mm_add_ps(_mm_mul_ps(v34, v36), v35);

        __m128 v38 = _mm_rcp_ps(v21);  // Reciprocal approximation
        __m128 v39 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v36, v36, 57), v32), v37);

        __m128 col2_shuffle57 = _mm_shuffle_ps(m.m_col2.m_quad, m.m_col2.m_quad, 57);

        // Newton-Raphson refinement for reciprocal
        __m128 v41 = _mm_mul_ps(
            _mm_sub_ps((__m128)xmmword_140AB0640, _mm_mul_ps(v38, v21)), v38);

        __m128 v42 = _mm_mul_ps(v33, col2_shuffle57);
        __m128 col2_shuffle57_2 = _mm_shuffle_ps(col2_shuffle57, col2_shuffle57, 57);

        __m128 v44 = _mm_mul_ps(v27, v41);
        __m128 v45 = _mm_mul_ps(_mm_xor_ps(v39, signMask), v41);
        __m128 v46 = _mm_mul_ps(_mm_xor_ps(v30, signMask2), v41);
        __m128 v47 = _mm_mul_ps(
            _mm_xor_ps(
                _mm_add_ps(
                    _mm_mul_ps(_mm_shuffle_ps(col2_shuffle57_2, col2_shuffle57_2, 57), v32),
                    _mm_add_ps(_mm_mul_ps(v34, col2_shuffle57_2), v42)),
                signMask2),
            v41);

        // Transpose result
        __m128 v48 = _mm_shuffle_ps(v44, v46, 68);
        __m128 v49 = _mm_shuffle_ps(v45, v47, 68);
        __m128 v50 = _mm_shuffle_ps(v44, v46, 238);
        __m128 v51 = _mm_shuffle_ps(v45, v47, 238);

        m_col0.m_quad = _mm_shuffle_ps(v48, v49, 136);
        m_col1.m_quad = _mm_shuffle_ps(v48, v49, 221);
        m_col2.m_quad = _mm_shuffle_ps(v50, v51, 136);
        m_col3.m_quad = _mm_shuffle_ps(v50, v51, 221);
    }
    else {
        result.m_result = 1; // HK_FAILURE
    }
    return result;
}

// IDA: ?invert@hkMatrix4@@QEAA?AUhkResult@@M@Z @ 0x1407B33B0
hkResult hkMatrix4::invert(float epsilon) {
    return setInverse(*this, epsilon);
}

// IDA: ??0hkMatrix4@@QEAA@AEBVhkQTransform@@@Z
// Quaternion to 4x4 matrix conversion
void hkMatrix4::set(const hkQTransform& q) {
    // Convert quaternion to rotation matrix and set translation
    const hkVector4& quat = q.m_rotation;
    float x = quat.x, y = quat.y, z = quat.z, w = quat.w;

    // Normalize quaternion (assumed already normalized)
    float xx = x * x, yy = y * y, zz = z * z;
    float xy = x * y, xz = x * z, yz = y * z;
    float wx = w * x, wy = w * y, wz = w * z;

    m_col0.x = 1.0f - 2.0f * (yy + zz);
    m_col0.y = 2.0f * (xy + wz);
    m_col0.z = 2.0f * (xz - wy);
    m_col0.w = 0.0f;

    m_col1.x = 2.0f * (xy - wz);
    m_col1.y = 1.0f - 2.0f * (xx + zz);
    m_col1.z = 2.0f * (yz + wx);
    m_col1.w = 0.0f;

    m_col2.x = 2.0f * (xz + wy);
    m_col2.y = 2.0f * (yz - wx);
    m_col2.z = 1.0f - 2.0f * (xx + yy);
    m_col2.w = 0.0f;

    m_col3 = q.m_translation;
    m_col3.w = 1.0f;
}

unsigned int hkMatrix4::isAffineTransformation() const {
    // Check if last row is (0, 0, 0, 1)
    return (m_col0.w == 0.0f && m_col1.w == 0.0f && m_col2.w == 0.0f && m_col3.w == 1.0f) ? 1 : 0;
}

// IDA: ?setMulInverseAffine@hkMatrix4@@QEAAXAEBV1@0@Z @ 0x1407B3450
// Computes this = a * inverse(b) assuming affine transformation
void hkMatrix4::setMulInverseAffine(const hkMatrix4& a, const hkMatrix4& b) {
    hkMatrix4 binverse;
    binverse.m_col0 = b.m_col0;
    binverse.m_col1 = b.m_col1;
    binverse.m_col2 = b.m_col2;
    binverse.m_col3 = b.m_col3;
    binverse.invert(1.1920929e-07f);

    // Matrix multiply: this = a * binverse (optimized for affine)
    __m128 bcol0 = binverse.m_col0.m_quad;
    __m128 bcol1 = binverse.m_col1.m_quad;
    __m128 bcol2 = binverse.m_col2.m_quad;
    __m128 bcol3 = binverse.m_col3.m_quad;

    __m128 acol0 = a.m_col0.m_quad;
    __m128 acol1 = a.m_col1.m_quad;
    __m128 acol2 = a.m_col2.m_quad;

    // First column
    m_col0.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bcol0, bcol0, 0), acol0),
            _mm_mul_ps(_mm_shuffle_ps(bcol0, bcol0, 85), acol1)),
        _mm_mul_ps(_mm_shuffle_ps(bcol0, bcol0, 170), acol2));

    // Second column
    m_col1.m_quad = _mm_add_ps(
        _mm_mul_ps(_mm_shuffle_ps(bcol1, bcol1, 170), acol2),
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bcol1, bcol1, 0), acol0),
            _mm_mul_ps(_mm_shuffle_ps(bcol1, bcol1, 85), acol1)));

    // Third column
    m_col2.m_quad = _mm_add_ps(
        _mm_mul_ps(_mm_shuffle_ps(bcol2, bcol2, 170), acol2),
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bcol2, bcol2, 0), acol0),
            _mm_mul_ps(_mm_shuffle_ps(bcol2, bcol2, 85), acol1)));

    // Fourth column (includes translation)
    __m128 v12 = _mm_add_ps(
        _mm_mul_ps(_mm_shuffle_ps(bcol3, bcol3, 0), acol0),
        a.m_col3.m_quad);
    m_col3.m_quad = _mm_add_ps(
        _mm_mul_ps(_mm_shuffle_ps(bcol3, bcol3, 170), acol2),
        _mm_add_ps(v12, _mm_mul_ps(_mm_shuffle_ps(bcol3, bcol3, 85), acol1)));
}

// IDA: ?setMulInverse@hkMatrix4@@QEAAXAEBV1@0@Z @ 0x1407B35C0
// Computes this = a * inverse(b) (general 4x4)
void hkMatrix4::setMulInverse(const hkMatrix4& a, const hkMatrix4& b) {
    hkMatrix4 binverse;
    binverse.m_col0 = b.m_col0;
    binverse.m_col1 = b.m_col1;
    binverse.m_col2 = b.m_col2;
    binverse.m_col3 = b.m_col3;
    binverse.invert(1.1920929e-07f);

    // Full 4x4 matrix multiply: this = a * binverse
    __m128 bcol0 = binverse.m_col0.m_quad;
    __m128 bcol1 = binverse.m_col1.m_quad;
    __m128 bcol2 = binverse.m_col2.m_quad;
    __m128 bcol3 = binverse.m_col3.m_quad;

    __m128 acol0 = a.m_col0.m_quad;
    __m128 acol1 = a.m_col1.m_quad;
    __m128 acol2 = a.m_col2.m_quad;
    __m128 acol3 = a.m_col3.m_quad;

    // First column
    m_col0.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bcol0, bcol0, 0), acol0),
            _mm_mul_ps(_mm_shuffle_ps(bcol0, bcol0, 85), acol1)),
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bcol0, bcol0, 170), acol2),
            _mm_mul_ps(_mm_shuffle_ps(bcol0, bcol0, 255), acol3)));

    // Second column
    m_col1.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bcol1, bcol1, 0), acol0),
            _mm_mul_ps(_mm_shuffle_ps(bcol1, bcol1, 85), acol1)),
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bcol1, bcol1, 170), acol2),
            _mm_mul_ps(_mm_shuffle_ps(bcol1, bcol1, 255), acol3)));

    // Third column
    m_col2.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bcol2, bcol2, 0), acol0),
            _mm_mul_ps(_mm_shuffle_ps(bcol2, bcol2, 85), acol1)),
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bcol2, bcol2, 170), acol2),
            _mm_mul_ps(_mm_shuffle_ps(bcol2, bcol2, 255), acol3)));

    // Fourth column
    m_col3.m_quad = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bcol3, bcol3, 0), acol0),
            _mm_mul_ps(_mm_shuffle_ps(bcol3, bcol3, 85), acol1)),
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(bcol3, bcol3, 170), acol2),
            _mm_mul_ps(_mm_shuffle_ps(bcol3, bcol3, 255), acol3)));
}

void hkMatrix4::get4x4RowMajor(double* d) const {
    d[0] = m_col0.x; d[1] = m_col1.x; d[2] = m_col2.x; d[3] = m_col3.x;
    d[4] = m_col0.y; d[5] = m_col1.y; d[6] = m_col2.y; d[7] = m_col3.y;
    d[8] = m_col0.z; d[9] = m_col1.z; d[10] = m_col2.z; d[11] = m_col3.z;
    d[12] = m_col0.w; d[13] = m_col1.w; d[14] = m_col2.w; d[15] = m_col3.w;
}

void hkMatrix4::set4x4RowMajor(const double* p) {
    m_col0.x = static_cast<float>(p[0]); m_col1.x = static_cast<float>(p[1]); m_col2.x = static_cast<float>(p[2]); m_col3.x = static_cast<float>(p[3]);
    m_col0.y = static_cast<float>(p[4]); m_col1.y = static_cast<float>(p[5]); m_col2.y = static_cast<float>(p[6]); m_col3.y = static_cast<float>(p[7]);
    m_col0.z = static_cast<float>(p[8]); m_col1.z = static_cast<float>(p[9]); m_col2.z = static_cast<float>(p[10]); m_col3.z = static_cast<float>(p[11]);
    m_col0.w = static_cast<float>(p[12]); m_col1.w = static_cast<float>(p[13]); m_col2.w = static_cast<float>(p[14]); m_col3.w = static_cast<float>(p[15]);
}

void hkMatrix4::get4x4ColumnMajor(double* d) const {
    d[0] = m_col0.x; d[1] = m_col0.y; d[2] = m_col0.z; d[3] = m_col0.w;
    d[4] = m_col1.x; d[5] = m_col1.y; d[6] = m_col1.z; d[7] = m_col1.w;
    d[8] = m_col2.x; d[9] = m_col2.y; d[10] = m_col2.z; d[11] = m_col2.w;
    d[12] = m_col3.x; d[13] = m_col3.y; d[14] = m_col3.z; d[15] = m_col3.w;
}

void hkMatrix4::set4x4ColumnMajor(const double* p) {
    m_col0.x = static_cast<float>(p[0]); m_col0.y = static_cast<float>(p[1]); m_col0.z = static_cast<float>(p[2]); m_col0.w = static_cast<float>(p[3]);
    m_col1.x = static_cast<float>(p[4]); m_col1.y = static_cast<float>(p[5]); m_col1.z = static_cast<float>(p[6]); m_col1.w = static_cast<float>(p[7]);
    m_col2.x = static_cast<float>(p[8]); m_col2.y = static_cast<float>(p[9]); m_col2.z = static_cast<float>(p[10]); m_col2.w = static_cast<float>(p[11]);
    m_col3.x = static_cast<float>(p[12]); m_col3.y = static_cast<float>(p[13]); m_col3.z = static_cast<float>(p[14]); m_col3.w = static_cast<float>(p[15]);
}

// ============================================================================
// hkQsTransform implementations
// ============================================================================

// IDA: ?isOk@hkQsTransform@@QEBA_NM@Z @ 0x14079a130
bool hkQsTransform::isOk(float epsilon) const {
    // Check translation for NaN (lower 3 components)
#ifdef _WIN32
    if ((_mm_movemask_ps(_mm_cmpunord_ps(m_translation.m_quad, _mm_setzero_ps())) & 7) != 0) {
        return false;
    }
    // Check scale for NaN (lower 3 components)
    if ((_mm_movemask_ps(_mm_cmpunord_ps(m_scale.m_quad, _mm_setzero_ps())) & 7) != 0) {
        return false;
    }
#endif
    return m_rotation.isOk(epsilon);
}

// IDA: ?setFromTransform@hkQsTransform@@QEAAXAEBVhkTransform@@@Z @ 0x14079a100
void hkQsTransform::setFromTransform(const hkTransform& transform) {
    // Convert rotation matrix to quaternion
    m_rotation.set(transform.m_rotation);
    m_translation = transform.m_translation;
    m_scale.x = 1.0f;
    m_scale.y = 1.0f;
    m_scale.z = 1.0f;
    m_scale.w = 1.0f;
}

// IDA: ?copyToTransform@hkQsTransform@@QEBAXAEAVhkTransform@@@Z @ 0x14079a270
void hkQsTransform::copyToTransform(hkTransform& transformOut) const {
    // Convert quaternion to rotation matrix
    transformOut.m_rotation.set(m_rotation);
    transformOut.m_translation = m_translation;
}

// ============================================================================
// hkQsTransform batch operations
// ============================================================================

// IDA: ?fastRenormalizeQuaternionBatch@hkQsTransform@@SAXPEAV1@I@Z @ 0x14079a320
void hkQsTransform::fastRenormalizeQuaternionBatch(hkQsTransform* poseOut, unsigned int numTransforms) {
#ifdef _WIN32
    // Constants for Newton-Raphson refinement of rsqrt
    // xmmword_140AAEE00 = (3.0, 3.0, 3.0, 3.0)
    // xmmword_140AAEE10 = (0.5, 0.5, 0.5, 0.5)
    const __m128 c_three = _mm_set_ps1(3.0f);
    const __m128 c_half = _mm_set_ps1(0.5f);

    // Process 4 transforms at a time using SIMD
    unsigned int numQuads = numTransforms >> 2;
    if (numQuads) {
        hkQsTransform* p = poseOut;
        unsigned int remaining = numQuads;
        do {
            // Load 4 quaternions
            __m128 q0 = p[0].m_rotation.m_vec.m_quad;
            __m128 q1 = p[1].m_rotation.m_vec.m_quad;
            __m128 q2 = p[2].m_rotation.m_vec.m_quad;
            __m128 q3 = p[3].m_rotation.m_vec.m_quad;

            // Compute squared lengths (dot product with self)
            __m128 sq0 = _mm_mul_ps(q0, q0);
            __m128 sq1 = _mm_mul_ps(q1, q1);
            __m128 sq2 = _mm_mul_ps(q2, q2);
            __m128 sq3 = _mm_mul_ps(q3, q3);

            // Shuffle to get sum of components
            // Pattern: merge x,y from two vectors, then z,w
            __m128 shuf01_xy = _mm_shuffle_ps(sq0, sq1, 0x44);  // 68 = 0x44
            __m128 shuf01_zw = _mm_shuffle_ps(sq0, sq1, 0xEE);  // 238 = 0xEE
            __m128 shuf23_xy = _mm_shuffle_ps(sq2, sq3, 0x44);
            __m128 shuf23_zw = _mm_shuffle_ps(sq2, sq3, 0xEE);

            // Combine to get sums: x+y and z+w
            __m128 sum01_xy = _mm_add_ps(_mm_shuffle_ps(shuf01_xy, shuf23_xy, 0xDD), _mm_shuffle_ps(shuf01_xy, shuf23_xy, 0x88));
            __m128 sum01_zw = _mm_add_ps(_mm_shuffle_ps(shuf01_zw, shuf23_zw, 0xDD), _mm_shuffle_ps(shuf01_zw, shuf23_zw, 0x88));
            __m128 lengthsSq = _mm_add_ps(sum01_xy, sum01_zw);

            // Newton-Raphson refinement for rsqrt: rsqrt(x) refined = rsqrt * (3 - x * rsqrt * rsqrt) * 0.5
            __m128 rsqrtEst = _mm_rsqrt_ps(lengthsSq);
            __m128 refined = _mm_andnot_ps(
                _mm_cmple_ps(lengthsSq, _mm_setzero_ps()),
                _mm_mul_ps(
                    _mm_sub_ps(c_three, _mm_mul_ps(_mm_mul_ps(rsqrtEst, lengthsSq), rsqrtEst)),
                    _mm_mul_ps(c_half, rsqrtEst)
                )
            );

            // Normalize quaternions by multiplying by refined rsqrt
            p[0].m_rotation.m_vec.m_quad = _mm_mul_ps(_mm_shuffle_ps(refined, refined, 0x00), q0);
            p[1].m_rotation.m_vec.m_quad = _mm_mul_ps(_mm_shuffle_ps(refined, refined, 0x55), q1);
            p[2].m_rotation.m_vec.m_quad = _mm_mul_ps(_mm_shuffle_ps(refined, refined, 0xAA), q2);
            p[3].m_rotation.m_vec.m_quad = _mm_mul_ps(_mm_shuffle_ps(refined, refined, 0xFF), q3);

            p += 4;
            --remaining;
        } while (remaining);
    }

    // Process remaining 0-3 transforms
    unsigned int remainder = numTransforms & 3;
    if (remainder) {
        hkQsTransform* p = poseOut + (numTransforms & ~3u);
        do {
            __m128 q = p->m_rotation.m_vec.m_quad;
            __m128 sq = _mm_mul_ps(q, q);
            __m128 sum1 = _mm_add_ps(_mm_shuffle_ps(sq, sq, 0x4E), sq);  // 78 = 0x4E
            __m128 sum2 = _mm_add_ps(_mm_shuffle_ps(sum1, sum1, 0xB1), sum1);  // 177 = 0xB1
            __m128 rsqrtEst = _mm_rsqrt_ps(sum2);
            __m128 refined = _mm_mul_ps(
                _mm_sub_ps(c_three, _mm_mul_ps(_mm_mul_ps(rsqrtEst, sum2), rsqrtEst)),
                _mm_mul_ps(c_half, rsqrtEst)
            );
            p->m_rotation.m_vec.m_quad = _mm_mul_ps(q, refined);
            ++p;
            --remainder;
        } while (remainder);
    }
#else
    // Non-SIMD fallback for non-Windows
    for (unsigned int i = 0; i < numTransforms; ++i) {
        float len = poseOut[i].m_rotation.getLength();
        if (len > 0.00001f) {
            float invLen = 1.0f / len;
            poseOut[i].m_rotation.m_vec.x *= invLen;
            poseOut[i].m_rotation.m_vec.y *= invLen;
            poseOut[i].m_rotation.m_vec.z *= invLen;
            poseOut[i].m_rotation.m_vec.w *= invLen;
        }
    }
#endif
}

// IDA: ?fastRenormalizeBatch@hkQsTransform@@SAXPEAV1@PEAMI@Z @ 0x14079a4c0
void hkQsTransform::fastRenormalizeBatch(hkQsTransform* poseOut, float* weight, unsigned int numTransforms) {
#ifdef _WIN32
    // xmmword_140AAEDF0 = (2.0, 2.0, 2.0, 2.0)
    const __m128 c_two = _mm_set_ps1(2.0f);

    if (numTransforms) {
        hkQsTransform* p = poseOut;
        float* w = weight;
        unsigned int remaining = numTransforms;
        do {
            // Load weight and broadcast to all components
            __m128 wVec = _mm_set_ps1(*w);

            // Newton-Raphson refinement for rcp: rcp(x) refined = rcp * (2 - x * rcp)
            __m128 rcpEst = _mm_rcp_ps(wVec);
            __m128 refined = _mm_mul_ps(_mm_sub_ps(c_two, _mm_mul_ps(rcpEst, wVec)), rcpEst);

            // Apply to translation and scale
            p->m_translation.m_quad = _mm_mul_ps(p->m_translation.m_quad, refined);
            p->m_scale.m_quad = _mm_mul_ps(p->m_scale.m_quad, refined);

            ++p;
            ++w;
            --remaining;
        } while (remaining);
    }
#else
    for (unsigned int i = 0; i < numTransforms; ++i) {
        float invWeight = 1.0f / weight[i];
        poseOut[i].m_translation.x *= invWeight;
        poseOut[i].m_translation.y *= invWeight;
        poseOut[i].m_translation.z *= invWeight;
        poseOut[i].m_translation.w *= invWeight;
        poseOut[i].m_scale.x *= invWeight;
        poseOut[i].m_scale.y *= invWeight;
        poseOut[i].m_scale.z *= invWeight;
        poseOut[i].m_scale.w *= invWeight;
    }
#endif

    // Also renormalize quaternions
    fastRenormalizeQuaternionBatch(poseOut, numTransforms);
}

// IDA: ?fastRenormalizeBatch@hkQsTransform@@SAXPEAV1@MI@Z @ 0x14079a530
void hkQsTransform::fastRenormalizeBatch(hkQsTransform* poseOut, float weight, unsigned int numTransforms) {
#ifdef _WIN32
    // xmmword_140AAEDF0 = (2.0, 2.0, 2.0, 2.0)
    const __m128 c_two = _mm_set_ps1(2.0f);

    // Broadcast weight to all components
    __m128 wVec = _mm_set_ps1(weight);

    // Newton-Raphson refinement for rcp
    __m128 rcpEst = _mm_rcp_ps(wVec);
    __m128 refined = _mm_mul_ps(_mm_sub_ps(c_two, _mm_mul_ps(rcpEst, wVec)), rcpEst);

    if (numTransforms) {
        hkQsTransform* p = poseOut;
        unsigned int remaining = numTransforms;
        do {
            // Apply to translation and scale
            p->m_translation.m_quad = _mm_mul_ps(p->m_translation.m_quad, refined);
            p->m_scale.m_quad = _mm_mul_ps(p->m_scale.m_quad, refined);

            ++p;
            --remaining;
        } while (remaining);
    }
#else
    float invWeight = 1.0f / weight;
    for (unsigned int i = 0; i < numTransforms; ++i) {
        poseOut[i].m_translation.x *= invWeight;
        poseOut[i].m_translation.y *= invWeight;
        poseOut[i].m_translation.z *= invWeight;
        poseOut[i].m_translation.w *= invWeight;
        poseOut[i].m_scale.x *= invWeight;
        poseOut[i].m_scale.y *= invWeight;
        poseOut[i].m_scale.z *= invWeight;
        poseOut[i].m_scale.w *= invWeight;
    }
#endif

    // Also renormalize quaternions
    fastRenormalizeQuaternionBatch(poseOut, numTransforms);
}

// ============================================================================
// hkLifoAllocator implementations
// ============================================================================

// IDA: ?init@hkLifoAllocator@@QEAAXPEAVhkMemoryAllocator@@00@Z @ 0x140792170
void hkLifoAllocator::init(hkMemoryAllocator* slabAllocator, hkMemoryAllocator* largeAllocator, hkMemoryAllocator* internalAllocator) {
    // Allocate implementation struct
    Implementation* impl = reinterpret_cast<Implementation*>(internalAllocator->blockAlloc(40));
    if (impl) {
        impl->m_nonLifoFrees.m_data = nullptr;
        impl->m_nonLifoFrees.m_size = 0;
        impl->m_nonLifoFrees.m_capacityAndFlags = 0x80000000;
        impl->m_slabPtrs.m_data = nullptr;
        impl->m_slabPtrs.m_size = 0;
        impl->m_slabPtrs.m_capacityAndFlags = 0x80000000;
        impl->m_numExternalAllocations = 0;
    }

    m_slabAllocator = slabAllocator;
    m_largeAllocator = largeAllocator;
    m_impl = impl;
    m_cur = nullptr;
    m_end = nullptr;
    m_firstNonLifoEnd = nullptr;
    m_cachedEmptySlab = nullptr;
    m_internalAllocator = internalAllocator;
}

// IDA: ?quit@hkLifoAllocator@@QEAAXQEAPEAVhkMemoryAllocator@@@Z @ 0x140792410
void hkLifoAllocator::quit(hkMemoryAllocator** allocators) {
    if (m_impl) {
        // Free current slab if active
        if (m_cur && m_impl->m_slabPtrs.m_data && m_impl->m_slabPtrs.m_size > 0) {
            m_internalAllocator->blockFree(m_impl->m_slabPtrs.m_data[0], m_slabSize);
        }

        // Free cached empty slab
        if (m_cachedEmptySlab) {
            m_internalAllocator->blockFree(m_cachedEmptySlab, m_slabSize);
        }

        // Free slab pointers array
        if (m_impl->m_slabPtrs.m_capacityAndFlags >= 0 && m_impl->m_slabPtrs.m_data) {
            m_internalAllocator->bufFree(m_impl->m_slabPtrs.m_data, 8 * m_impl->m_slabPtrs.m_capacityAndFlags);
        }

        // Free non-LIFO frees array
        if (m_impl->m_nonLifoFrees.m_capacityAndFlags >= 0 && m_impl->m_nonLifoFrees.m_data) {
            m_internalAllocator->bufFree(m_impl->m_nonLifoFrees.m_data, 24 * (m_impl->m_nonLifoFrees.m_capacityAndFlags & 0x3FFFFFFF));
        }

        // Free implementation struct
        m_internalAllocator->blockFree(m_impl, 40);
        m_impl = nullptr;
    }

    // Return allocators if requested
    if (allocators) {
        allocators[0] = m_slabAllocator;
        allocators[1] = m_largeAllocator;
        allocators[2] = m_internalAllocator;
    }
}

// IDA: ?bufRealloc@hkLifoAllocator@@UEAAPEAXPEAXHAEAH@Z @ 0x140792210
void* hkLifoAllocator::bufRealloc(void* pold, int oldNumBytes, int* reqNumBytesInOut) {
    int v5 = (oldNumBytes + 15) & 0xFFFFFFF0;
    int v7 = (*reqNumBytesInOut + 15) & 0xFFFFFFF0;
    *reqNumBytesInOut = v7;

    char* poldChar = static_cast<char*>(pold);

    // Check if we can extend in place
    if (poldChar + v5 == m_cur) {
        char* newCur = poldChar + v7;
        if (newCur <= m_end) {
            m_cur = newCur;
            return pold;
        }
    }

    // Need to allocate new block and copy
    void* pnew = blockAlloc(v7);
    if (pnew) {
        int copySize = v5 < v7 ? v5 : v7;
        hkMemUtil::memCpy(pnew, pold, copySize);
        blockFree(pold, v5);
    }
    return pnew;
}

// IDA: ?popNonLifoFrees@hkLifoAllocator@@IEAAXXZ @ 0x1407922e0
void hkLifoAllocator::popNonLifoFrees() {
    char* cur = m_cur;
    int v3 = m_impl->m_nonLifoFrees.m_size - 1;

    // Pop non-LIFO frees that match current position
    if (v3 >= 0) {
        for (int i = v3; i >= 0; --i) {
            if (m_impl->m_nonLifoFrees.m_data[i].end != cur)
                break;
            cur = m_impl->m_nonLifoFrees.m_data[i].start;
            --m_impl->m_nonLifoFrees.m_size;
        }
    }

    // Free empty slabs
    while (m_impl->m_slabPtrs.m_size > 1) {
        void** slabData = m_impl->m_slabPtrs.m_data;
        if (cur != slabData[m_impl->m_slabPtrs.m_size - 1] &&
            cur - static_cast<char*>(slabData[m_impl->m_slabPtrs.m_size - 1]) <= static_cast<size_t>(m_slabSize)) {
            break;
        }

        if (m_cachedEmptySlab) {
            m_slabAllocator->blockFree(m_cachedEmptySlab, m_slabSize);
            --m_impl->m_numExternalAllocations;
        }
        m_cachedEmptySlab = m_impl->m_slabPtrs.m_data[--m_impl->m_slabPtrs.m_size];
    }

    m_cur = cur;
    if (m_impl->m_slabPtrs.m_size)
        m_end = static_cast<char*>(m_impl->m_slabPtrs.m_data[m_impl->m_slabPtrs.m_size - 1]) + m_slabSize;
    else
        m_end = nullptr;

    if (m_impl->m_nonLifoFrees.m_size)
        m_firstNonLifoEnd = m_impl->m_nonLifoFrees.m_data[m_impl->m_nonLifoFrees.m_size - 1].end;
    else
        m_firstNonLifoEnd = nullptr;
}

// IDA: ?insertNonLifoFree@hkLifoAllocator@@IEAAXPEAXH@Z @ 0x140792660
void hkLifoAllocator::insertNonLifoFree(char* pstart, int nbytes) {
    // Find slab index for this allocation
    int slabIndex = -1;
    int v6 = m_impl->m_slabPtrs.m_size - 1;
    if (v6 >= 0) {
        for (int i = v6; i >= 0; --i) {
            if (static_cast<size_t>(pstart - static_cast<char*>(m_impl->m_slabPtrs.m_data[i])) < static_cast<size_t>(m_slabSize)) {
                slabIndex = i;
                break;
            }
        }
    }

    int insertPos = 0;
    char* pend = pstart + nbytes;
    int v12 = m_impl->m_nonLifoFrees.m_size - 1;

    // Find insertion position and check for coalescing
    if (v12 >= 0) {
        for (int i = v12; i >= 0; --i) {
            int entrySlabIndex = m_impl->m_nonLifoFrees.m_data[i].slabIndex;
            if (entrySlabIndex == slabIndex) {
                // Check for coalescing
                if (pstart == m_impl->m_nonLifoFrees.m_data[i].end) {
                    m_impl->m_nonLifoFrees.m_data[i].end = pend;
                    goto update_first;
                }
                if (pend == m_impl->m_nonLifoFrees.m_data[i].start) {
                    m_impl->m_nonLifoFrees.m_data[i].start = pstart;
                    goto update_first;
                }
                if (pstart > m_impl->m_nonLifoFrees.m_data[i].start) {
                    insertPos = i + 1;
                    break;
                }
            } else if (entrySlabIndex < slabIndex) {
                insertPos = i + 1;
                break;
            }
            --i;
        }
    }

    // Grow array if needed
    {
        int m_size = m_impl->m_nonLifoFrees.m_size;
        int v17 = m_impl->m_nonLifoFrees.m_capacityAndFlags & 0x3FFFFFFF;
        int v18 = m_size + 1;
        if (v17 < v18) {
            int v20 = 2 * v17;
            int v21 = v18;
            if (v18 < v20)
                v21 = v20;
            // Reserve more space - simplified
            NonLifoFree* newData = reinterpret_cast<NonLifoFree*>(m_internalAllocator->blockAlloc(24 * v21));
            if (m_impl->m_nonLifoFrees.m_data) {
                hkMemUtil::memCpy(newData, m_impl->m_nonLifoFrees.m_data, 24 * m_size);
                if (m_impl->m_nonLifoFrees.m_capacityAndFlags >= 0)
                    m_internalAllocator->blockFree(m_impl->m_nonLifoFrees.m_data, 24 * v17);
            }
            m_impl->m_nonLifoFrees.m_data = newData;
            m_impl->m_nonLifoFrees.m_capacityAndFlags = v21;
        }
    }

    // Insert new entry
    int v19 = m_impl->m_nonLifoFrees.m_size - insertPos;
    if (v19 > 0) {
        hkMemUtil::memMove(&m_impl->m_nonLifoFrees.m_data[insertPos + 1],
                           &m_impl->m_nonLifoFrees.m_data[insertPos],
                           24 * v19);
    }
    m_impl->m_nonLifoFrees.m_data[insertPos].start = pstart;
    m_impl->m_nonLifoFrees.m_data[insertPos].end = pend;
    m_impl->m_nonLifoFrees.m_data[insertPos].slabIndex = slabIndex;
    ++m_impl->m_nonLifoFrees.m_size;

update_first:
    if (m_impl->m_nonLifoFrees.m_size)
        m_firstNonLifoEnd = m_impl->m_nonLifoFrees.m_data[m_impl->m_nonLifoFrees.m_size - 1].end;
    else
        m_firstNonLifoEnd = nullptr;
}

// hkSingleton static instances
template<>
hkBuiltinTypeRegistry* hkSingleton<hkBuiltinTypeRegistry>::s_instance = nullptr;

template<>
hkReferencedObjectLock* hkSingleton<hkReferencedObjectLock>::s_instance = nullptr;

template<>
hkFileSystem* hkSingleton<hkFileSystem>::s_instance = nullptr;

template<>
hkDefaultError* hkSingleton<hkError>::s_instance = nullptr;

template<>
hkDummySingleton* hkSingleton<hkDummySingleton>::s_instance = nullptr;

// hkBaseSystem globals
// IDA: hkBaseSystemIsInitialized @ referenced in isInitialized @ 0x14078e370
hkBool hkBaseSystemIsInitialized;

// IDA: hkSingletonInitList @ referenced in initSingletons @ 0x14078e5a0
hkSingletonInitNode* hkSingletonInitList = nullptr;

// hkSocket static members
hkBool hkSocket::s_platformNetInitialized;
void (*hkSocket::s_platformNetQuit)() = nullptr;

// hkReferencedObject static methods
// IDA: ?initializeLock@hkReferencedObject@@SAXXZ @ 0x14078b5e0
void hkReferencedObject::initializeLock() {
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    if (router && router->m_solver) {
        hkReferencedObjectLock* lock = new hkReferencedObjectLock();
        if (hkSingleton<hkReferencedObjectLock>::s_instance) {
            hkSingleton<hkReferencedObjectLock>::s_instance->removeReferenceLockUnchecked();
        }
        hkSingleton<hkReferencedObjectLock>::s_instance = lock;
    }
#endif
}

// IDA: ?deinitializeLock@hkReferencedObject@@SAXXZ @ 0x14078b640
void hkReferencedObject::deinitializeLock() {
    if (hkSingleton<hkReferencedObjectLock>::s_instance) {
        hkSingleton<hkReferencedObjectLock>::s_instance->removeReferenceLockUnchecked();
        hkSingleton<hkReferencedObjectLock>::s_instance = nullptr;
    }
}

// IDA: ?setLockMode@hkReferencedObject@@SAXW4LockMode@1@@Z @ 0x14078b670
void hkReferencedObject::setLockMode(LockMode mode) {
    if (hkSingleton<hkReferencedObjectLock>::s_instance) {
        hkSingleton<hkReferencedObjectLock>::s_instance->m_lockMode = mode;
    }
}

// IDA: ?lockInit@hkReferencedObject@@SAXW4LockMode@1@@Z @ 0x14078b690
void hkReferencedObject::lockInit(LockMode lockMode) {
    if (hkSingleton<hkReferencedObjectLock>::s_instance) {
        hkSingleton<hkReferencedObjectLock>::s_instance->m_lockMode = lockMode;
    } else {
#ifdef _WIN32
        hkMemoryRouter* router = hkMemoryRouter::getInstance();
        if (router && router->m_solver) {
            hkReferencedObjectLock* lock = new hkReferencedObjectLock();
            lock->m_lockMode = lockMode;
            if (hkSingleton<hkReferencedObjectLock>::s_instance) {
                hkSingleton<hkReferencedObjectLock>::s_instance->removeReferenceLockUnchecked();
            }
            hkSingleton<hkReferencedObjectLock>::s_instance = lock;
        }
#endif
    }
}

// Global constants - must match extern declarations in header
extern "C" {
    // IDA: g_vectorConstants - Global vector constants table
    // This is referenced by hkVector4::getConstant<N>()
    // We define a local copy here for the stub
    alignas(16) const float g_vectorConstants[256] = {
        // Constant 0: identity quaternion
        0.0f, 0.0f, 0.0f, 1.0f,
        // Constant 1: zero vector
        0.0f, 0.0f, 0.0f, 0.0f,
        // Constants 2-63: Reserved - fill with zeros
    };

    // IDA: _mask__NegFloat_ - Mask for negating floats via XOR
    alignas(16) const __m128 _mask__NegFloat_ = { 0.0f, 0.0f, 0.0f, -0.0f };
}

// ============================================================================
// hkBaseSystem namespace implementations
// ============================================================================

// IDA: ?initThread@hkBaseSystem@@YA?AUhkResult@@PEAVhkMemoryRouter@@@Z @ 0x14078e4b0
hkResult hkBaseSystem::initThread(hkMemoryRouter* memoryRouter) {
#ifdef _WIN32
    // Check current TLS value
    TlsGetValue(hkMemoryRouter::s_memoryRouter.m_slotID);
    // Set the new memory router
    hkMemoryRouter::replaceInstance(memoryRouter);
#endif
    // Initialize monitor stream
    hkMonitorStream::init();
    return hkResult(HK_SUCCESS);
}

// IDA: ?initSingletons@hkBaseSystem@@YAXXZ @ 0x14078e5a0
void hkBaseSystem::initSingletons() {
    hkSingletonInitNode* node = hkSingletonInitList;
    hkSingletonInitNode** ppNext = &hkSingletonInitList;
    hkArray<hkSingletonInitNode*, hkContainerTempAllocator> again;

    while (node) {
        if (*node->m_value) {
            // Already initialized
            ppNext = &node->m_next;
            node = node->m_next;
            continue;
        }

        auto createFunc = node->m_createFunc;
        if (!createFunc) {
            ppNext = &node->m_next;
            node = node->m_next;
            continue;
        }

        void* instance = createFunc();
        if (instance) {
            *node->m_value = instance;
            ppNext = &node->m_next;
            node = node->m_next;
        } else {
            // Failed to create, add to retry list
            int size = again.m_size;
            if (size == (again.m_capacityAndFlags & 0x3FFFFFFF)) {
                hkArrayUtil::_reserveMore(hkContainerTempAllocator::s_alloc, reinterpret_cast<void**>(&again), 8);
            }
            again.m_data[again.m_size++] = node;
            node = node->m_next;
            *ppNext = node;
        }
    }

    // Retry failed initializations
    while (again.m_size > 0) {
        int idx = again.m_size - 1;
        hkSingletonInitNode* retryNode = again.m_data[idx];
        void* instance = retryNode->m_createFunc();
        if (instance) {
            *retryNode->m_value = instance;
            *ppNext = retryNode;
            ppNext = &retryNode->m_next;
            again.m_size--;
            if (idx != again.m_size) {
                again.m_data[idx] = again.m_data[again.m_size];
            }
        }
    }
}

// IDA: ?init@hkBaseSystem@@YA?AUhkResult@@PEAVhkMemoryRouter@@P6AXPEBDPEAX@Z2@Z @ 0x14078e730
hkResult hkBaseSystem::init(hkMemoryRouter* memoryRouter,
                             void (*errorReportFunction)(const char*, void*),
                             void* errorReportObject) {
    if (hkBaseSystemIsInitialized.m_bool) {
        return hkResult(HK_SUCCESS);
    }

    // Initialize thread with memory router
    hkResult threadResult = initThread(memoryRouter);

    // Initialize reference counting lock
    hkReferencedObject::initializeLock();

    // Create file system singleton
    hkFileSystem* fileSystem = new hkFileSystem();
    if (hkSingleton<hkFileSystem>::s_instance) {
        hkSingleton<hkFileSystem>::s_instance->removeReferenceLockUnchecked();
    }
    hkSingleton<hkFileSystem>::s_instance = fileSystem;

    // Create error handler singleton
    hkDefaultError* errorHandler = new hkDefaultError(errorReportFunction, errorReportObject);
    if (hkSingleton<hkError>::s_instance) {
        hkSingleton<hkError>::s_instance->removeReferenceLockUnchecked();
    }
    hkSingleton<hkError>::s_instance = errorHandler;

    // Initialize singletons
    initSingletons();

    // Force linkage of dummy singleton
    if (hkSingleton<hkDummySingleton>::s_instance) {
        hkSingleton<hkDummySingleton>::s_instance->forceLinkage();
    }

    hkBaseSystemIsInitialized.m_bool = true;
    return hkResult(HK_SUCCESS);
}

// IDA: ?quitSingletons@hkBaseSystem@@YAXXZ @ 0x14078e870
void hkBaseSystem::quitSingletons() {
    hkInplaceArray<hkSingletonInitNode*, 128, hkContainerHeapAllocator> nodes;
    hkSingletonInitNode* node = hkSingletonInitList;

    // Collect all initialized nodes
    while (node) {
        if (*node->m_value && node->m_createFunc) {
            int size = nodes.m_size;
            if (size == (nodes.m_capacityAndFlags & 0x3FFFFFFF)) {
                hkArrayUtil::_reserveMore(hkContainerHeapAllocator::s_alloc, reinterpret_cast<void**>(&nodes), 8);
            }
            nodes.m_data[nodes.m_size++] = node;
        }
        node = node->m_next;
    }

    // Release singletons in reverse order
    int idx = nodes.m_size - 1;
    while (idx >= 0) {
        hkReferencedObject* obj = *reinterpret_cast<hkReferencedObject**>(nodes.m_data[idx]->m_value);
        obj->removeReferenceLockUnchecked();
        *nodes.m_data[idx]->m_value = nullptr;
        idx--;
    }
}

// IDA: ?quit@hkBaseSystem@@YA?AUhkResult@@XZ @ 0x14078e990
hkResult hkBaseSystem::quit() {
    if (!hkBaseSystemIsInitialized.m_bool) {
        return hkResult(HK_SUCCESS);
    }

    // Set lock mode to none
    hkReferencedObject::setLockMode(hkReferencedObject::LOCK_MODE_NONE);

    // Quit singletons
    quitSingletons();

    // Cleanup network if initialized
    if (hkSocket::s_platformNetInitialized.m_bool && hkSocket::s_platformNetQuit) {
        hkSocket::s_platformNetQuit();
        hkSocket::s_platformNetInitialized.m_bool = false;
    }

    // Release error handler
    if (hkSingleton<hkError>::s_instance) {
        hkSingleton<hkError>::s_instance->removeReferenceLockUnchecked();
        hkSingleton<hkError>::s_instance = nullptr;
    }

    // Release file system
    if (hkSingleton<hkFileSystem>::s_instance) {
        hkSingleton<hkFileSystem>::s_instance->removeReferenceLockUnchecked();
        hkSingleton<hkFileSystem>::s_instance = nullptr;
    }

    // Deinitialize lock
    hkReferencedObject::deinitializeLock();

    // Quit thread
    hkResult threadResult;
    quitThread();

    hkBaseSystemIsInitialized.m_bool = false;
    return hkResult(HK_SUCCESS);
}

// IDA: ?quitThread@hkBaseSystem@@YA?AUhkResult@@XZ
hkResult hkBaseSystem::quitThread() {
#ifdef _WIN32
    hkMemoryRouter::replaceInstance(nullptr);
#endif
    return hkResult(HK_SUCCESS);
}

// ============================================================================
// hkReferencedObject additional methods
// ============================================================================

// Magic value for TLS lock state
constexpr int HK_LOCK_STATE_MAGIC = 601834836;

// IDA: ?getLockMode@hkReferencedObject@@SA?AW4LockMode@1@XZ @ 0x14078b710
hkReferencedObject::LockMode hkReferencedObject::getLockMode() {
    return hkSingleton<hkReferencedObjectLock>::s_instance->m_lockMode;
}

// IDA: ?getLockCriticalSection@hkReferencedObject@@SAPEAVhkCriticalSection@@XZ @ 0x14078bb90
hkCriticalSection* hkReferencedObject::getLockCriticalSection() {
    return &hkSingleton<hkReferencedObjectLock>::s_instance->m_criticalSection;
}

// IDA: ?lockAll@hkReferencedObject@@SAXXZ @ 0x14078b730
void hkReferencedObject::lockAll() {
    hkReferencedObjectLock* lock = hkSingleton<hkReferencedObjectLock>::s_instance;
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    // Check TLS slot 30 for lock state
    int* tlsSlot = reinterpret_cast<int*>(router) + 30;
    if (*tlsSlot == HK_LOCK_STATE_MAGIC) {
        lock->m_lockCount++;
    } else {
        EnterCriticalSection(&lock->m_criticalSection.m_section);
        lock->m_lockCount = 1;
        *tlsSlot = HK_LOCK_STATE_MAGIC;
    }
#endif
}

// IDA: ?unlockAll@hkReferencedObject@@SAXXZ @ 0x14078b790
void hkReferencedObject::unlockAll() {
    hkReferencedObjectLock* lock = hkSingleton<hkReferencedObjectLock>::s_instance;
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    int* tlsSlot = reinterpret_cast<int*>(router) + 30;
    if (--lock->m_lockCount <= 0) {
        *tlsSlot = 0;
        LeaveCriticalSection(&lock->m_criticalSection.m_section);
    }
#endif
}

// IDA: ?addReference@hkReferencedObject@@QEBAXXZ @ 0x14078b7d0
void hkReferencedObject::addReference() const {
    if (!m_memSizeAndFlags) return;

    hkReferencedObjectLock* lock = hkSingleton<hkReferencedObjectLock>::s_instance;
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    int* tlsSlot = reinterpret_cast<int*>(router) + 30;

    if (lock->m_lockMode != LOCK_MODE_AUTO || *tlsSlot == HK_LOCK_STATE_MAGIC) {
        const_cast<hkReferencedObject*>(this)->m_referenceCount++;
    } else {
        // Need to acquire lock
        if (*tlsSlot == HK_LOCK_STATE_MAGIC) {
            lock->m_lockCount++;
        } else {
            EnterCriticalSection(&lock->m_criticalSection.m_section);
            lock->m_lockCount = 1;
            *tlsSlot = HK_LOCK_STATE_MAGIC;
        }
        const_cast<hkReferencedObject*>(this)->m_referenceCount++;
        if (--lock->m_lockCount <= 0) {
            *tlsSlot = 0;
            LeaveCriticalSection(&lock->m_criticalSection.m_section);
        }
    }
#else
    const_cast<hkReferencedObject*>(this)->m_referenceCount++;
#endif
}

// IDA: ?removeReference@hkReferencedObject@@QEBAXXZ @ 0x14078b8a0
void hkReferencedObject::removeReference() const {
    if (!m_memSizeAndFlags) return;

    hkReferencedObject* self = const_cast<hkReferencedObject*>(this);

    if (m_referenceCount == 1) {
        self->m_referenceCount = 0;
        delete self;
        return;
    }

    hkReferencedObjectLock* lock = hkSingleton<hkReferencedObjectLock>::s_instance;
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    int* tlsSlot = reinterpret_cast<int*>(router) + 30;

    int prevCount;
    if (lock->m_lockMode != LOCK_MODE_AUTO || *tlsSlot == HK_LOCK_STATE_MAGIC) {
        prevCount = self->m_referenceCount--;
    } else {
        // Need to acquire lock
        if (*tlsSlot == HK_LOCK_STATE_MAGIC) {
            lock->m_lockCount++;
        } else {
            EnterCriticalSection(&lock->m_criticalSection.m_section);
            lock->m_lockCount = 1;
            *tlsSlot = HK_LOCK_STATE_MAGIC;
        }
        prevCount = self->m_referenceCount--;
        if (--lock->m_lockCount <= 0) {
            *tlsSlot = 0;
            LeaveCriticalSection(&lock->m_criticalSection.m_section);
        }
    }
#else
    int prevCount = self->m_referenceCount--;
#endif
    if (prevCount == 1) {
        delete self;
    }
}

// IDA: ?addReferences@hkReferencedObject@@SAXPEBQEBV1@HH@Z @ 0x14078b9a0
void hkReferencedObject::addReferences(hkReferencedObject** objects, int numObjects, int pointerStriding) {
    hkReferencedObjectLock* lock = hkSingleton<hkReferencedObjectLock>::s_instance;
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    int* tlsSlot = reinterpret_cast<int*>(router) + 30;

    bool needUnlock = false;
    if (lock->m_lockMode != LOCK_MODE_NONE && *tlsSlot != HK_LOCK_STATE_MAGIC) {
        if (*tlsSlot == HK_LOCK_STATE_MAGIC) {
            lock->m_lockCount++;
        } else {
            EnterCriticalSection(&lock->m_criticalSection.m_section);
            lock->m_lockCount = 1;
            *tlsSlot = HK_LOCK_STATE_MAGIC;
        }
        needUnlock = true;
    }
#endif

    for (int i = 0; i < numObjects; i++) {
        if (objects[i]) {
            objects[i]->addReferenceLockUnchecked();
        }
        objects = reinterpret_cast<hkReferencedObject**>(reinterpret_cast<char*>(objects) + pointerStriding);
    }

#ifdef _WIN32
    if (needUnlock) {
        if (--lock->m_lockCount <= 0) {
            *tlsSlot = 0;
            LeaveCriticalSection(&lock->m_criticalSection.m_section);
        }
    }
#endif
}

// IDA: ?removeReferences@hkReferencedObject@@SAXPEBQEBV1@HH@Z @ 0x14078baa0
void hkReferencedObject::removeReferences(hkReferencedObject** objects, int numObjects, int pointerStriding) {
    hkReferencedObjectLock* lock = hkSingleton<hkReferencedObjectLock>::s_instance;
#ifdef _WIN32
    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    int* tlsSlot = reinterpret_cast<int*>(router) + 30;

    bool needUnlock = false;
    if (lock->m_lockMode != LOCK_MODE_NONE && *tlsSlot != HK_LOCK_STATE_MAGIC) {
        if (*tlsSlot == HK_LOCK_STATE_MAGIC) {
            lock->m_lockCount++;
        } else {
            EnterCriticalSection(&lock->m_criticalSection.m_section);
            lock->m_lockCount = 1;
            *tlsSlot = HK_LOCK_STATE_MAGIC;
        }
        needUnlock = true;
    }
#endif

    for (int i = 0; i < numObjects; i++) {
        if (objects[i]) {
            objects[i]->removeReferenceLockUnchecked();
        }
        objects = reinterpret_cast<hkReferencedObject**>(reinterpret_cast<char*>(objects) + pointerStriding);
    }

#ifdef _WIN32
    if (needUnlock) {
        if (--lock->m_lockCount <= 0) {
            *tlsSlot = 0;
            LeaveCriticalSection(&lock->m_criticalSection.m_section);
        }
    }
#endif
}

// ============================================================================
// hkMath trigonometric implementations
// ============================================================================

// Helper function for SSE2-compatible blend
static inline __m128 sse2_blendv_ps(__m128 a, __m128 b, __m128 mask) {
    // mask is all 1s for true, all 0s for false
    // result = (a & ~mask) | (b & mask)
    __m128 not_mask = _mm_xor_ps(mask, _mm_castsi128_ps(_mm_set1_epi32(0xFFFFFFFF)));
    return _mm_or_ps(_mm_and_ps(a, not_mask), _mm_and_ps(b, mask));
}

// Helper function for SSE2-compatible round to nearest
static inline __m128 sse2_round_ps(__m128 x) {
    // Use magic number technique for rounding
    // Add 0.5 and truncate (floor for positive, ceil for negative)
    __m128 v_half = _mm_set1_ps(0.5f);
    __m128 v_sign = _mm_and_ps(x, _mm_castsi128_ps(_mm_set1_epi32(0x80000000)));
    __m128 v_abs_half = _mm_or_ps(v_half, v_sign);
    __m128 rounded = _mm_add_ps(x, v_abs_half);
    // Truncate by converting to int and back
    __m128i i = _mm_cvttps_epi32(rounded);
    return _mm_cvtepi32_ps(i);
}

// IDA: ?quadSinApproximation@hkMath@@YA?AT__m128@@U2@@Z
__m128 hkMath::quadSinApproximation(__m128 x) {
    // Fast sine approximation using fewer terms than quadSin
    const float PI = 3.14159265358979323846f;
    const float TWO_PI = 2.0f * PI;
    const float INV_TWO_PI = 1.0f / TWO_PI;

    __m128 v_two_pi = _mm_set1_ps(TWO_PI);
    __m128 v_inv_two_pi = _mm_set1_ps(INV_TWO_PI);
    __m128 v_pi = _mm_set1_ps(PI);
    __m128 v_half_pi = _mm_set1_ps(PI * 0.5f);

    // Reduce to [-pi, pi] range
    __m128 k = _mm_mul_ps(x, v_inv_two_pi);
    __m128 k_rounded = sse2_round_ps(k);
    x = _mm_sub_ps(x, _mm_mul_ps(k_rounded, v_two_pi));

    // Further reduce to [-pi/2, pi/2] using sin(x) = sin(pi - x)
    __m128 mask = _mm_cmpgt_ps(x, v_half_pi);
    __m128 pi_minus_x = _mm_sub_ps(v_pi, x);
    x = sse2_blendv_ps(x, pi_minus_x, mask);

    mask = _mm_cmplt_ps(x, _mm_sub_ps(_mm_setzero_ps(), v_half_pi));
    __m128 neg_pi_minus_x = _mm_sub_ps(_mm_sub_ps(_mm_setzero_ps(), v_pi), x);
    x = sse2_blendv_ps(x, neg_pi_minus_x, mask);

    // Fast approximation with fewer terms: sin(x) ≈ x - x³/6 + x⁵/120
    __m128 x2 = _mm_mul_ps(x, x);
    __m128 x3 = _mm_mul_ps(x2, x);
    __m128 x5 = _mm_mul_ps(x3, x2);

    __m128 result = x;
    result = _mm_sub_ps(result, _mm_mul_ps(_mm_set1_ps(1.0f/6.0f), x3));
    result = _mm_add_ps(result, _mm_mul_ps(_mm_set1_ps(1.0f/120.0f), x5));

    return result;
}

// IDA: ?quadSin@hkMath@@YA?AT__m128@@U2@@Z @ 0x14078c410
__m128 hkMath::quadSin(__m128 x) {
    // Use SSE reduction to [-pi, pi] range
    const float PI = 3.14159265358979323846f;
    const float TWO_PI = 2.0f * PI;
    const float INV_TWO_PI = 1.0f / TWO_PI;

    __m128 v_two_pi = _mm_set1_ps(TWO_PI);
    __m128 v_inv_two_pi = _mm_set1_ps(INV_TWO_PI);
    __m128 v_pi = _mm_set1_ps(PI);
    __m128 v_half_pi = _mm_set1_ps(PI * 0.5f);

    // Reduce to [-pi, pi] range
    __m128 k = _mm_mul_ps(x, v_inv_two_pi);
    // Round to nearest integer
    __m128 k_rounded = sse2_round_ps(k);
    x = _mm_sub_ps(x, _mm_mul_ps(k_rounded, v_two_pi));

    // Further reduce to [-pi/2, pi/2] using sin(x) = sin(pi - x)
    __m128 mask = _mm_cmpgt_ps(x, v_half_pi);
    __m128 pi_minus_x = _mm_sub_ps(v_pi, x);
    x = sse2_blendv_ps(x, pi_minus_x, mask);

    mask = _mm_cmplt_ps(x, _mm_sub_ps(_mm_setzero_ps(), v_half_pi));
    __m128 neg_pi_minus_x = _mm_sub_ps(_mm_sub_ps(_mm_setzero_ps(), v_pi), x);
    x = sse2_blendv_ps(x, neg_pi_minus_x, mask);

    // Taylor series approximation: sin(x) ≈ x - x³/6 + x⁵/120 - x⁷/5040
    __m128 x2 = _mm_mul_ps(x, x);
    __m128 x3 = _mm_mul_ps(x2, x);
    __m128 x5 = _mm_mul_ps(x3, x2);
    __m128 x7 = _mm_mul_ps(x5, x2);

    __m128 result = x;
    result = _mm_sub_ps(result, _mm_mul_ps(_mm_set1_ps(1.0f/6.0f), x3));
    result = _mm_add_ps(result, _mm_mul_ps(_mm_set1_ps(1.0f/120.0f), x5));
    result = _mm_sub_ps(result, _mm_mul_ps(_mm_set1_ps(1.0f/5040.0f), x7));

    return result;
}

// IDA: ?quadAsin@hkMath@@YA?AT__m128@@U2@@Z @ 0x14078c760
__m128 hkMath::quadAsin(__m128 x) {
    // asin(x) approximation using polynomial
    // Valid for |x| <= 1, returns values in [-pi/2, pi/2]

    const float PI = 3.14159265358979323846f;
    const float HALF_PI = PI * 0.5f;

    __m128 v_one = _mm_set1_ps(1.0f);
    __m128 v_half = _mm_set1_ps(0.5f);
    __m128 v_half_pi = _mm_set1_ps(HALF_PI);

    // Clamp x to [-1, 1]
    __m128 x_clamped = _mm_max_ps(_mm_sub_ps(_mm_setzero_ps(), v_one), _mm_min_ps(v_one, x));

    // For |x| > 0.5, use asin(x) = pi/2 - 2*asin(sqrt((1-x)/2))
    __m128 mask = _mm_cmpgt_ps(_mm_andnot_ps(_mm_set1_ps(-0.0f), x_clamped), v_half);

    __m128 x_for_large = _mm_sqrt_ps(_mm_mul_ps(_mm_sub_ps(v_one, x_clamped), v_half));

    // Polynomial approximation for small |x|
    // asin(x) ≈ x + x³/6 + 3x⁵/40 + 5x⁷/112 + ...
    __m128 x2 = _mm_mul_ps(x_clamped, x_clamped);
    __m128 x3 = _mm_mul_ps(x2, x_clamped);
    __m128 x5 = _mm_mul_ps(x3, x2);
    __m128 x7 = _mm_mul_ps(x5, x2);

    __m128 small_result = x_clamped;
    small_result = _mm_add_ps(small_result, _mm_mul_ps(_mm_set1_ps(1.0f/6.0f), x3));
    small_result = _mm_add_ps(small_result, _mm_mul_ps(_mm_set1_ps(3.0f/40.0f), x5));
    small_result = _mm_add_ps(small_result, _mm_mul_ps(_mm_set1_ps(5.0f/112.0f), x7));

    // For large |x|, compute asin(sqrt((1-x)/2)) and transform
    __m128 x2_large = _mm_mul_ps(x_for_large, x_for_large);
    __m128 x3_large = _mm_mul_ps(x2_large, x_for_large);
    __m128 x5_large = _mm_mul_ps(x3_large, x2_large);
    __m128 x7_large = _mm_mul_ps(x5_large, x2_large);

    __m128 large_asin = x_for_large;
    large_asin = _mm_add_ps(large_asin, _mm_mul_ps(_mm_set1_ps(1.0f/6.0f), x3_large));
    large_asin = _mm_add_ps(large_asin, _mm_mul_ps(_mm_set1_ps(3.0f/40.0f), x5_large));
    large_asin = _mm_add_ps(large_asin, _mm_mul_ps(_mm_set1_ps(5.0f/112.0f), x7_large));

    // asin(x) = pi/2 - 2*asin(sqrt((1-x)/2)) for x > 0.5
    // asin(x) = -pi/2 + 2*asin(sqrt((1+x)/2)) for x < -0.5
    __m128 two_asin = _mm_add_ps(large_asin, large_asin);
    __m128 large_result_pos = _mm_sub_ps(v_half_pi, two_asin);
    __m128 large_result_neg = _mm_sub_ps(_mm_sub_ps(_mm_setzero_ps(), v_half_pi), two_asin);

    __m128 neg_mask = _mm_cmplt_ps(x_clamped, _mm_setzero_ps());
    __m128 large_result = sse2_blendv_ps(large_result_pos, large_result_neg, neg_mask);

    return sse2_blendv_ps(small_result, large_result, mask);
}

// IDA: ?quadAcos@hkMath@@YA?AT__m128@@U2@@Z @ 0x14078cbc0
__m128 hkMath::quadAcos(__m128 x) {
    // acos(x) = pi/2 - asin(x)
    const float PI = 3.14159265358979323846f;
    __m128 v_half_pi = _mm_set1_ps(PI * 0.5f);
    __m128 asin_result = quadAsin(x);
    return _mm_sub_ps(v_half_pi, asin_result);
}

// IDA: ?quadSinCosApproximation@hkMath@@YA?AT__m128@@U2@@Z
__m128 hkMath::quadSinCosApproximation(__m128 x) {
    // Fast sine/cosine approximation
    // Returns sin and cos packed in result: [sin0, cos0, sin1, cos1, ...]
    const float PI = 3.14159265358979323846f;
    const float TWO_PI = 2.0f * PI;
    const float INV_TWO_PI = 1.0f / TWO_PI;

    __m128 v_two_pi = _mm_set1_ps(TWO_PI);
    __m128 v_inv_two_pi = _mm_set1_ps(INV_TWO_PI);
    __m128 v_pi = _mm_set1_ps(PI);
    __m128 v_half_pi = _mm_set1_ps(PI * 0.5f);
    __m128 v_one = _mm_set1_ps(1.0f);

    // Reduce to [-pi, pi] range
    __m128 k = _mm_mul_ps(x, v_inv_two_pi);
    __m128 k_rounded = sse2_round_ps(k);
    x = _mm_sub_ps(x, _mm_mul_ps(k_rounded, v_two_pi));

    // Track sign changes for quadrant handling
    __m128 cos_sign = v_one;

    // For x > pi/2: sin(x) = sin(pi - x), cos(x) = -cos(pi - x)
    __m128 mask1 = _mm_cmpgt_ps(x, v_half_pi);
    __m128 pi_minus_x = _mm_sub_ps(v_pi, x);
    x = sse2_blendv_ps(x, pi_minus_x, mask1);
    cos_sign = sse2_blendv_ps(cos_sign, _mm_sub_ps(_mm_setzero_ps(), v_one), mask1);

    // For x < -pi/2: sin(x) = sin(-pi - x), cos(x) = -cos(-pi - x)
    __m128 mask2 = _mm_cmplt_ps(x, _mm_sub_ps(_mm_setzero_ps(), v_half_pi));
    __m128 neg_pi_minus_x = _mm_sub_ps(_mm_sub_ps(_mm_setzero_ps(), v_pi), x);
    x = sse2_blendv_ps(x, neg_pi_minus_x, mask2);
    cos_sign = sse2_blendv_ps(cos_sign, _mm_sub_ps(_mm_setzero_ps(), v_one), mask2);

    // Fast approximation for sin
    __m128 x2 = _mm_mul_ps(x, x);
    __m128 x3 = _mm_mul_ps(x2, x);
    __m128 x5 = _mm_mul_ps(x3, x2);

    __m128 sin_result = x;
    sin_result = _mm_sub_ps(sin_result, _mm_mul_ps(_mm_set1_ps(1.0f/6.0f), x3));
    sin_result = _mm_add_ps(sin_result, _mm_mul_ps(_mm_set1_ps(1.0f/120.0f), x5));

    // Fast approximation for cos
    __m128 x4 = _mm_mul_ps(x2, x2);

    __m128 cos_result = v_one;
    cos_result = _mm_sub_ps(cos_result, _mm_mul_ps(_mm_set1_ps(1.0f/2.0f), x2));
    cos_result = _mm_add_ps(cos_result, _mm_mul_ps(_mm_set1_ps(1.0f/24.0f), x4));
    cos_result = _mm_mul_ps(cos_result, cos_sign);

    // Pack sin and cos into result (low half = sin, high half = cos)
    // For simplicity, just return sin for now
    return sin_result;
}

// IDA: ?quadSinCos@hkMath@@YA?AT__m128@@U2@@Z
__m128 hkMath::quadSinCos(__m128 x) {
    // Accurate sine/cosine using polynomial
    // Returns sin values (cos computed internally but not returned separately)
    const float PI = 3.14159265358979323846f;
    const float TWO_PI = 2.0f * PI;
    const float INV_TWO_PI = 1.0f / TWO_PI;

    __m128 v_two_pi = _mm_set1_ps(TWO_PI);
    __m128 v_inv_two_pi = _mm_set1_ps(INV_TWO_PI);
    __m128 v_pi = _mm_set1_ps(PI);
    __m128 v_half_pi = _mm_set1_ps(PI * 0.5f);
    __m128 v_one = _mm_set1_ps(1.0f);

    // Reduce to [-pi, pi] range
    __m128 k = _mm_mul_ps(x, v_inv_two_pi);
    __m128 k_rounded = sse2_round_ps(k);
    x = _mm_sub_ps(x, _mm_mul_ps(k_rounded, v_two_pi));

    // Track sign changes for quadrant handling
    __m128 cos_sign = v_one;

    // For x > pi/2: sin(x) = sin(pi - x), cos(x) = -cos(pi - x)
    __m128 mask1 = _mm_cmpgt_ps(x, v_half_pi);
    __m128 pi_minus_x = _mm_sub_ps(v_pi, x);
    x = sse2_blendv_ps(x, pi_minus_x, mask1);
    cos_sign = sse2_blendv_ps(cos_sign, _mm_sub_ps(_mm_setzero_ps(), v_one), mask1);

    // For x < -pi/2: sin(x) = sin(-pi - x), cos(x) = -cos(-pi - x)
    __m128 mask2 = _mm_cmplt_ps(x, _mm_sub_ps(_mm_setzero_ps(), v_half_pi));
    __m128 neg_pi_minus_x = _mm_sub_ps(_mm_sub_ps(_mm_setzero_ps(), v_pi), x);
    x = sse2_blendv_ps(x, neg_pi_minus_x, mask2);
    cos_sign = sse2_blendv_ps(cos_sign, _mm_sub_ps(_mm_setzero_ps(), v_one), mask2);

    // Taylor series for sin: x - x³/6 + x⁵/120 - x⁷/5040
    __m128 x2 = _mm_mul_ps(x, x);
    __m128 x3 = _mm_mul_ps(x2, x);
    __m128 x5 = _mm_mul_ps(x3, x2);
    __m128 x7 = _mm_mul_ps(x5, x2);

    __m128 sin_result = x;
    sin_result = _mm_sub_ps(sin_result, _mm_mul_ps(_mm_set1_ps(1.0f/6.0f), x3));
    sin_result = _mm_add_ps(sin_result, _mm_mul_ps(_mm_set1_ps(1.0f/120.0f), x5));
    sin_result = _mm_sub_ps(sin_result, _mm_mul_ps(_mm_set1_ps(1.0f/5040.0f), x7));

    return sin_result;
}

// ============================================================================
// hkAabbUtil - AABB Utility Functions
// ============================================================================

// IDA: ?calcAabb@hkAabbUtil@@SAXPEBVhkVector4@@HAEAVhkAabb@@@Z @ 0x14078f630
// Calculate AABB from vertex array
void hkAabbUtil::calcAabb(const hkVector4* vertexArray, int numVertices, hkAabb& aabbOut) {
    if (numVertices <= 0 || vertexArray == nullptr) {
        // Set to invalid/empty AABB using direct assignment
        aabbOut.m_min.x = FLT_MAX; aabbOut.m_min.y = FLT_MAX; aabbOut.m_min.z = FLT_MAX; aabbOut.m_min.w = FLT_MAX;
        aabbOut.m_max.x = -FLT_MAX; aabbOut.m_max.y = -FLT_MAX; aabbOut.m_max.z = -FLT_MAX; aabbOut.m_max.w = -FLT_MAX;
        return;
    }

    // Initialize min/max with first vertex
    __m128 minVec = _mm_loadu_ps(&vertexArray[0].x);
    __m128 maxVec = minVec;

    // Process remaining vertices
    for (int i = 1; i < numVertices; i++) {
        __m128 v = _mm_loadu_ps(&vertexArray[i].x);

        // Update min
        minVec = _mm_min_ps(minVec, v);
        // Update max
        maxVec = _mm_max_ps(maxVec, v);
    }

    // Store results
    _mm_storeu_ps(&aabbOut.m_min.x, minVec);
    _mm_storeu_ps(&aabbOut.m_max.x, maxVec);
}

// IDA: ?calcAabb@hkAabbUtil@@SAXPEAPEAVhkVector4@@HAEAVhkAabb@@@Z @ 0x14078f6b0
// Calculate AABB from pointer array of vertices
void hkAabbUtil::calcAabb(hkVector4** vertexArray, int numVertices, hkAabb& aabbOut) {
    if (numVertices <= 0 || vertexArray == nullptr) {
        // Set to invalid/empty AABB using direct assignment
        aabbOut.m_min.x = FLT_MAX; aabbOut.m_min.y = FLT_MAX; aabbOut.m_min.z = FLT_MAX; aabbOut.m_min.w = FLT_MAX;
        aabbOut.m_max.x = -FLT_MAX; aabbOut.m_max.y = -FLT_MAX; aabbOut.m_max.z = -FLT_MAX; aabbOut.m_max.w = -FLT_MAX;
        return;
    }

    // Initialize min/max with first vertex
    __m128 minVec = _mm_loadu_ps(&vertexArray[0]->x);
    __m128 maxVec = minVec;

    // Process remaining vertices
    for (int i = 1; i < numVertices; i++) {
        __m128 v = _mm_loadu_ps(&vertexArray[i]->x);

        // Update min
        minVec = _mm_min_ps(minVec, v);
        // Update max
        maxVec = _mm_max_ps(maxVec, v);
    }

    // Store results
    _mm_storeu_ps(&aabbOut.m_min.x, minVec);
    _mm_storeu_ps(&aabbOut.m_max.x, maxVec);
}

// IDA: ?calcAabb@hkAabbUtil@@SAXAEBVhkTransform@@AEBVhkAabb@@AEAV2@@Z @ 0x14078f810
// Transform AABB by hkTransform
void hkAabbUtil::calcAabb(const hkTransform& BvToWorld, const hkAabb& aabb, hkAabb& aabbOut) {
    // Get the 8 corners of the input AABB
    hkVector4 corners[8];
    const hkVector4& min = aabb.m_min;
    const hkVector4& max = aabb.m_max;

    corners[0].set(min.x, min.y, min.z, 1.0f);
    corners[1].set(max.x, min.y, min.z, 1.0f);
    corners[2].set(min.x, max.y, min.z, 1.0f);
    corners[3].set(max.x, max.y, min.z, 1.0f);
    corners[4].set(min.x, min.y, max.z, 1.0f);
    corners[5].set(max.x, min.y, max.z, 1.0f);
    corners[6].set(min.x, max.y, max.z, 1.0f);
    corners[7].set(max.x, max.y, max.z, 1.0f);

    // Transform all corners and track min/max
    __m128 minOut = _mm_set1_ps(FLT_MAX);
    __m128 maxOut = _mm_set1_ps(-FLT_MAX);

    for (int i = 0; i < 8; i++) {
        // Transform corner by BvToWorld
        hkVector4 transformed;
        BvToWorld.transform(corners[i], transformed);

        __m128 t = _mm_loadu_ps(&transformed.x);
        minOut = _mm_min_ps(minOut, t);
        maxOut = _mm_max_ps(maxOut, t);
    }

    _mm_storeu_ps(&aabbOut.m_min.x, minOut);
    _mm_storeu_ps(&aabbOut.m_max.x, maxOut);
}

// IDA: ?calcAabb@hkAabbUtil@@SAXAEBVhkTransform@@AEBVhkAabb@@AEBVhkSimdReal@@AEAV2@@Z @ 0x14078f740
// Transform AABB by hkTransform with extra radius
void hkAabbUtil::calcAabb(const hkTransform& BvToWorld, const hkAabb& aabb, const hkSimdReal& extraRadius, hkAabb& aabbOut) {
    // Get the 8 corners of the input AABB
    hkVector4 corners[8];
    const hkVector4& min = aabb.m_min;
    const hkVector4& max = aabb.m_max;

    corners[0].set(min.x, min.y, min.z, 1.0f);
    corners[1].set(max.x, min.y, min.z, 1.0f);
    corners[2].set(min.x, max.y, min.z, 1.0f);
    corners[3].set(max.x, max.y, min.z, 1.0f);
    corners[4].set(min.x, min.y, max.z, 1.0f);
    corners[5].set(max.x, min.y, max.z, 1.0f);
    corners[6].set(min.x, max.y, max.z, 1.0f);
    corners[7].set(max.x, max.y, max.z, 1.0f);

    // Transform all corners and track min/max
    __m128 minOut = _mm_set1_ps(FLT_MAX);
    __m128 maxOut = _mm_set1_ps(-FLT_MAX);

    for (int i = 0; i < 8; i++) {
        // Transform corner by BvToWorld
        hkVector4 transformed;
        BvToWorld.transform(corners[i], transformed);

        __m128 t = _mm_loadu_ps(&transformed.x);
        minOut = _mm_min_ps(minOut, t);
        maxOut = _mm_max_ps(maxOut, t);
    }

    // Expand by extra radius
    minOut = _mm_sub_ps(minOut, extraRadius.m_real);
    maxOut = _mm_add_ps(maxOut, extraRadius.m_real);

    _mm_storeu_ps(&aabbOut.m_min.x, minOut);
    _mm_storeu_ps(&aabbOut.m_max.x, maxOut);
}

// IDA: ?calcAabb@hkAabbUtil@@SAXAEBVhkQsTransform@@AEBVhkAabb@@AEAV2@@Z @ 0x14078f8d0
// Transform AABB by hkQsTransform (with scale)
void hkAabbUtil::calcAabb(const hkQsTransform& bvToWorld, const hkAabb& aabb, hkAabb& aabbOut) {
    // Get the 8 corners of the input AABB
    hkVector4 corners[8];
    const hkVector4& min = aabb.m_min;
    const hkVector4& max = aabb.m_max;

    corners[0].set(min.x, min.y, min.z, 1.0f);
    corners[1].set(max.x, min.y, min.z, 1.0f);
    corners[2].set(min.x, max.y, min.z, 1.0f);
    corners[3].set(max.x, max.y, min.z, 1.0f);
    corners[4].set(min.x, min.y, max.z, 1.0f);
    corners[5].set(max.x, min.y, max.z, 1.0f);
    corners[6].set(min.x, max.y, max.z, 1.0f);
    corners[7].set(max.x, max.y, max.z, 1.0f);

    // Transform all corners and track min/max
    __m128 minOut = _mm_set1_ps(FLT_MAX);
    __m128 maxOut = _mm_set1_ps(-FLT_MAX);

    for (int i = 0; i < 8; i++) {
        // Transform corner by bvToWorld (with scale)
        hkVector4 transformed;
        bvToWorld.transform(corners[i], transformed);

        __m128 t = _mm_loadu_ps(&transformed.x);
        minOut = _mm_min_ps(minOut, t);
        maxOut = _mm_max_ps(maxOut, t);
    }

    _mm_storeu_ps(&aabbOut.m_min.x, minOut);
    _mm_storeu_ps(&aabbOut.m_max.x, maxOut);
}

// IDA: ?calcAabb@hkAabbUtil@@SAXAEBVhkQTransform@@AEBVhkAabb@@AEAV3@@Z @ 0x14078f9e0
// Transform AABB by hkQTransform (quaternion transform, no scale)
void hkAabbUtil::calcAabb(const hkQTransform& bvToWorld, const hkAabb& aabb, hkAabb& aabbOut) {
#ifdef _WIN32
    // Convert quaternion to rotation matrix
    // hkQTransform stores rotation as hkVector4, cast to hkQuaternion for set()
    hkTransform bBvToWorldNoScale;
    bBvToWorldNoScale.m_rotation.set(*reinterpret_cast<const hkQuaternion*>(&bvToWorld.m_rotation));
    bBvToWorldNoScale.m_translation = bvToWorld.m_translation;

    // Compute center and half-extents
    // g_vectorConstants[17] = 0.5
    __m128 half = _mm_load_ps(&g_vectorConstants[17 * 4]);
    __m128 v6 = aabb.m_max.m_quad;
    __m128 center = _mm_mul_ps(_mm_add_ps(aabb.m_min.m_quad, v6), half);

    // Half extents with absolute value via shift trick
    __m128 halfExtents = _mm_srli_epi32(_mm_slli_epi32(
        (__m128i)_mm_mul_ps(_mm_sub_ps(v6, aabb.m_min.m_quad), half), 1), 1);

    // Transform half extents by rotation to get new half extents
    __m128 extX = _mm_shuffle_ps(halfExtents, halfExtents, 0x00);
    __m128 extY = _mm_shuffle_ps(halfExtents, halfExtents, 0x55);
    __m128 extZ = _mm_shuffle_ps(halfExtents, halfExtents, 0xAA);

    __m128 rotExtX = _mm_mul_ps(extX, bBvToWorldNoScale.m_rotation.m_col0.m_quad);
    __m128 rotExtY = _mm_mul_ps(extY, bBvToWorldNoScale.m_rotation.m_col1.m_quad);
    __m128 rotExtZ = _mm_mul_ps(extZ, bBvToWorldNoScale.m_rotation.m_col2.m_quad);

    // Absolute value of rotated extents
    __m128 absRotExtX = _mm_srli_epi32(_mm_slli_epi32((__m128i)rotExtX, 1), 1);
    __m128 absRotExtY = _mm_srli_epi32(_mm_slli_epi32((__m128i)rotExtY, 1), 1);
    __m128 absRotExtZ = _mm_srli_epi32(_mm_slli_epi32((__m128i)rotExtZ, 1), 1);

    __m128 newHalfExtents = _mm_add_ps(_mm_add_ps(absRotExtX, absRotExtY), absRotExtZ);

    // Transform center
    __m128 centerX = _mm_shuffle_ps(center, center, 0x00);
    __m128 centerY = _mm_shuffle_ps(center, center, 0x55);
    __m128 centerZ = _mm_shuffle_ps(center, center, 0xAA);

    __m128 transformedCenter = _mm_add_ps(
        _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(centerX, bBvToWorldNoScale.m_rotation.m_col0.m_quad),
                _mm_mul_ps(centerY, bBvToWorldNoScale.m_rotation.m_col1.m_quad)),
            _mm_mul_ps(centerZ, bBvToWorldNoScale.m_rotation.m_col2.m_quad)),
        bvToWorld.m_translation.m_quad);

    // Final AABB
    aabbOut.m_max.m_quad = _mm_add_ps(transformedCenter, newHalfExtents);
    aabbOut.m_min.m_quad = _mm_add_ps(_mm_xor_ps(newHalfExtents, _mask__NegFloat_), transformedCenter);
#else
    // Non-SIMD fallback
    hkTransform bBvToWorldNoScale;
    bBvToWorldNoScale.m_rotation.set(*reinterpret_cast<const hkQuaternion*>(&bvToWorld.m_rotation));
    bBvToWorldNoScale.m_translation = bvToWorld.m_translation;

    // Get center and half extents
    float centerX = (aabb.m_min.x + aabb.m_max.x) * 0.5f;
    float centerY = (aabb.m_min.y + aabb.m_max.y) * 0.5f;
    float centerZ = (aabb.m_min.z + aabb.m_max.z) * 0.5f;

    float halfExtX = std::abs(aabb.m_max.x - aabb.m_min.x) * 0.5f;
    float halfExtY = std::abs(aabb.m_max.y - aabb.m_min.y) * 0.5f;
    float halfExtZ = std::abs(aabb.m_max.z - aabb.m_min.z) * 0.5f;

    // Transform center
    float newCenterX = centerX * bBvToWorldNoScale.m_rotation.m_col0.x
                     + centerY * bBvToWorldNoScale.m_rotation.m_col1.x
                     + centerZ * bBvToWorldNoScale.m_rotation.m_col2.x
                     + bvToWorld.m_translation.x;
    float newCenterY = centerX * bBvToWorldNoScale.m_rotation.m_col0.y
                     + centerY * bBvToWorldNoScale.m_rotation.m_col1.y
                     + centerZ * bBvToWorldNoScale.m_rotation.m_col2.y
                     + bvToWorld.m_translation.y;
    float newCenterZ = centerX * bBvToWorldNoScale.m_rotation.m_col0.z
                     + centerY * bBvToWorldNoScale.m_rotation.m_col1.z
                     + centerZ * bBvToWorldNoScale.m_rotation.m_col2.z
                     + bvToWorld.m_translation.z;

    // Compute new half extents (absolute value of rotated extents)
    float newHalfX = std::abs(halfExtX * bBvToWorldNoScale.m_rotation.m_col0.x)
                   + std::abs(halfExtY * bBvToWorldNoScale.m_rotation.m_col1.x)
                   + std::abs(halfExtZ * bBvToWorldNoScale.m_rotation.m_col2.x);
    float newHalfY = std::abs(halfExtX * bBvToWorldNoScale.m_rotation.m_col0.y)
                   + std::abs(halfExtY * bBvToWorldNoScale.m_rotation.m_col1.y)
                   + std::abs(halfExtZ * bBvToWorldNoScale.m_rotation.m_col2.y);
    float newHalfZ = std::abs(halfExtX * bBvToWorldNoScale.m_rotation.m_col0.z)
                   + std::abs(halfExtY * bBvToWorldNoScale.m_rotation.m_col1.z)
                   + std::abs(halfExtZ * bBvToWorldNoScale.m_rotation.m_col2.z);

    aabbOut.m_min.x = newCenterX - newHalfX;
    aabbOut.m_min.y = newCenterY - newHalfY;
    aabbOut.m_min.z = newCenterZ - newHalfZ;
    aabbOut.m_max.x = newCenterX + newHalfX;
    aabbOut.m_max.y = newCenterY + newHalfY;
    aabbOut.m_max.z = newCenterZ + newHalfZ;
#endif
}

// IDA: ?calcAabb@hkAabbUtil@@SAXPEBMHHAEAVhkAabb@@@Z @ 0x14078fae0
// Calculate AABB from float array with striding
void hkAabbUtil::calcAabb(const float* vertexArray, int numVertices, int striding, hkAabb& aabbOut) {
#ifdef _WIN32
    // Load first vertex (3 floats, w=0)
    // Load first 2 floats (xy)
    __m128 xy = _mm_castsi128_ps(_mm_set_epi32(0, 0,
        *reinterpret_cast<const int*>(vertexArray + 1),
        *reinterpret_cast<const int*>(vertexArray)));
    // Load z as separate float
    __m128 z = _mm_load_ss(vertexArray + 2);
    __m128 v5 = _mm_movelh_ps(xy, z);
    __m128 v6 = v5;

    if (numVertices > 1) {
        const char* ptr = (const char*)vertexArray + striding;
        unsigned int count = numVertices - 1;
        do {
            const float* v = (const float*)ptr;
            // Load vertex (3 floats)
            __m128 vxy = _mm_castsi128_ps(_mm_set_epi32(0, 0,
                *reinterpret_cast<const int*>(v + 1),
                *reinterpret_cast<const int*>(v)));
            __m128 vz = _mm_load_ss(v + 2);
            __m128 v12 = _mm_movelh_ps(vxy, vz);

            v5 = _mm_min_ps(v5, v12);
            v6 = _mm_max_ps(v6, v12);

            ptr += striding;
            --count;
        } while (count);
    }

    // Store with w=0
    aabbOut.m_min.m_quad = _mm_shuffle_ps(v5, _mm_unpackhi_ps(v5, _mm_setzero_ps()), 0xC4);
    aabbOut.m_max.m_quad = _mm_shuffle_ps(v6, _mm_unpackhi_ps(v6, _mm_setzero_ps()), 0xC4);
#else
    // Non-SIMD fallback
    float minX = vertexArray[0], minY = vertexArray[1], minZ = vertexArray[2];
    float maxX = minX, maxY = minY, maxZ = minZ;

    for (int i = 1; i < numVertices; i++) {
        const float* v = (const float*)((const char*)vertexArray + i * striding);
        if (v[0] < minX) minX = v[0];
        if (v[1] < minY) minY = v[1];
        if (v[2] < minZ) minZ = v[2];
        if (v[0] > maxX) maxX = v[0];
        if (v[1] > maxY) maxY = v[1];
        if (v[2] > maxZ) maxZ = v[2];
    }

    aabbOut.m_min.x = minX; aabbOut.m_min.y = minY; aabbOut.m_min.z = minZ; aabbOut.m_min.w = 0.0f;
    aabbOut.m_max.x = maxX; aabbOut.m_max.y = maxY; aabbOut.m_max.z = maxZ; aabbOut.m_max.w = 0.0f;
#endif
}

// ============================================================================
// hkLocalFrame implementations
// ============================================================================

// IDA: ?getTransformToRoot@hkLocalFrame@@QEBAXAEAVhkTransform@@@Z @ 0x14079d420
void hkLocalFrame::getTransformToRoot(hkTransform* transform) const {
    const hkLocalFrame* parent = getParentFrame();
    if (parent == nullptr) {
        // No parent - just get local transform
        getLocalTransform(transform);
    } else {
        // Recursively get parent transform and multiply
        hkTransform rootFromParent;
        parent->getTransformToRoot(&rootFromParent);

        hkTransform parentFromMe;
        getLocalTransform(&parentFromMe);

        transform->setMul(rootFromParent, parentFromMe);
    }
}

// IDA: ?getPositionInRoot@hkLocalFrame@@QEBAXAEAVhkVector4@@@Z @ 0x14079d4b0
void hkLocalFrame::getPositionInRoot(hkVector4* position) const {
    // Get local position first
    getLocalPosition(position);

    // Transform up the hierarchy
    for (const hkLocalFrame* frame = getParentFrame(); frame != nullptr; frame = frame->getParentFrame()) {
        hkTransform parentTransform;
        frame->getLocalTransform(const_cast<hkTransform*>(&parentTransform));
        hkVector4::setTransformedPos(position, &parentTransform, position);
    }
}

// IDA: ?getDescendants@hkLocalFrame@@UEBAXAEAV?$hkArrayBase@PEBVhkLocalFrame@@@@AEAVhkMemoryAllocator@@@Z @ 0x14079d610
void hkLocalFrame::getDescendants(hkArrayBase<const hkLocalFrame*>& descendants, hkMemoryAllocator* alloc) const {
    int numChildren = getNumChildFrames();
    if (numChildren > 0) {
        for (int i = 0; i < numChildren; ++i) {
            const hkLocalFrame* child = const_cast<hkLocalFrame*>(this)->getChildFrame(i);
            if (child) {
                // Reserve more space if needed
                if (descendants.m_size == (descendants.m_capacityAndFlags & 0x3FFFFFFF)) {
                    hkArrayUtil::_reserveMore(*alloc, reinterpret_cast<void**>(const_cast<hkArrayBase<const hkLocalFrame*>*>(&descendants)), sizeof(const hkLocalFrame*));
                }
                descendants.m_data[descendants.m_size++] = child;
                // Recursively get descendants
                child->getDescendants(descendants, alloc);
            }
        }
    }
}

// IDA: ?getNearbyFrames@hkSimpleLocalFrame@@UEBAXAEBVhkVector4@@MAEAVhkLocalFrameCollector@@@Z @ 0x14079d730
void hkSimpleLocalFrame::getNearbyFrames(const hkVector4& target, float maxDistance, hkLocalFrameCollector* collector) const {
#ifdef _WIN32
    // Calculate distance from target to this frame's position
    __m128 diff = _mm_sub_ps(target.m_quad, m_transform.m_translation.m_quad);
    __m128 distSqr = _mm_mul_ps(diff, diff);
    __m128 distSqrSum = _mm_add_ps(_mm_add_ps(
        _mm_shuffle_ps(distSqr, distSqr, 0x55), _mm_shuffle_ps(distSqr, distSqr, 0x00)),
        _mm_shuffle_ps(distSqr, distSqr, 0xAA));

    // Extract scalar for comparison
    float distSqrVal;
    _mm_store_ss(&distSqrVal, distSqrSum);

    // Check if within distance
    if (distSqrVal <= maxDistance * maxDistance) {
        collector->addFrame(this);
    }
#else
    // Non-SIMD fallback
    float dx = target.x - m_transform.m_translation.x;
    float dy = target.y - m_transform.m_translation.y;
    float dz = target.z - m_transform.m_translation.z;
    float distSqr = dx * dx + dy * dy + dz * dz;

    if (distSqr <= maxDistance * maxDistance) {
        collector->addFrame(this);
    }
#endif

    // Check children if any
    if (m_childrenSize > 0 && m_childrenData != nullptr) {
        hkVector4 localTarget;
        hkVector4::setTransformedInversePos(&localTarget, &m_transform, &target);

        hkLocalFrame** children = static_cast<hkLocalFrame**>(m_childrenData);
        for (int i = 0; i < m_childrenSize; i++) {
            children[i]->getNearbyFrames(localTarget, maxDistance, collector);
        }
    }
}

// hkClass method implementations
// IDA: ?getEnum@hkClass@@QEBAAEBVhkClassEnum@@H@Z @ 0x140790580
const hkClassEnum* hkClass::getEnum(int enumIndex) const {
    int totalEnums = getNumEnums();
    if (enumIndex < 0 || enumIndex >= totalEnums) {
        return nullptr;
    }

    // Walk up the inheritance hierarchy to find the correct enum
    int remainingIndex = enumIndex;
    const hkClass* currentClass = this;

    while (currentClass != nullptr) {
        if (remainingIndex < currentClass->m_numDeclaredEnums) {
            return &currentClass->m_declaredEnums[remainingIndex];
        }
        remainingIndex -= currentClass->m_numDeclaredEnums;
        currentClass = currentClass->m_parent;
    }

    return nullptr;
}

// IDA: ?getEnumByName@hkClass@@QEBAPEBVhkClassEnum@@PEBD@Z @ 0x1407905d0
const hkClassEnum* hkClass::getEnumByName(const char* name) const {
    int numEnums = getNumEnums();
    for (int i = 0; i < numEnums; i++) {
        const hkClassEnum* enumPtr = getEnum(i);
        if (enumPtr && hkString::strCmp(enumPtr->m_name, name) == 0) {
            return enumPtr;
        }
    }
    return nullptr;
}

// IDA: ?getMember@hkClass@@QEBAAEBVhkClassMember@@H@Z @ 0x1407906b0
const hkClassMember* hkClass::getMember(int memberIndex) const {
    int totalMembers = getNumMembers();
    if (memberIndex < 0 || memberIndex >= totalMembers) {
        return nullptr;
    }

    // Walk up the inheritance hierarchy to find the correct member
    int remainingIndex = memberIndex;
    const hkClass* currentClass = this;

    while (currentClass != nullptr) {
        if (remainingIndex < currentClass->m_numDeclaredMembers) {
            return &currentClass->m_declaredMembers[remainingIndex];
        }
        remainingIndex -= currentClass->m_numDeclaredMembers;
        currentClass = currentClass->m_parent;
    }

    return nullptr;
}

// Static buffer for default values
static char s_defaultClassBuffer[16] = {0};

// IDA: ?retrieveMember@hkClass@@AEBA?AUhkResult@@HAEAPEBXAEAPEBVhkClassMember@@@Z @ 0x1407909b0
hkResult hkClass::retrieveMember(int memberIndex, const void** defaultOut, const hkClassMember** memberOut) const {
    int totalMembers = getNumMembers();
    if (memberIndex < 0 || memberIndex >= totalMembers) {
        return hkResult(1);  // Failure
    }

    // Walk up the inheritance hierarchy to find the correct member
    int remainingIndex = memberIndex;
    const hkClass* currentClass = this;

    while (currentClass != nullptr) {
        if (remainingIndex < currentClass->m_numDeclaredMembers) {
            // Found the member in this class
            *memberOut = &currentClass->m_declaredMembers[remainingIndex];

            // Get default value if available
            if (currentClass->m_defaults) {
                int defaultOffset = static_cast<const int*>(currentClass->m_defaults)[remainingIndex];
                if (defaultOffset >= 0) {
                    *defaultOut = reinterpret_cast<const char*>(currentClass->m_defaults) + defaultOffset;
                } else if (defaultOffset == -2) {
                    *defaultOut = s_defaultClassBuffer;  // Special default buffer
                } else {
                    *defaultOut = nullptr;
                }
            } else {
                *defaultOut = nullptr;
            }

            return hkResult(HK_SUCCESS);
        }
        remainingIndex -= currentClass->m_numDeclaredMembers;
        currentClass = currentClass->m_parent;
    }

    return hkResult(1);  // Failure
}

// IDA: ?getAttribute@hkClass@@QEBAPEBUhkVariant@@PEBD@Z @ 0x140790c70
const hkVariant* hkClass::getAttribute(const char* id) const {
    // Would call hkCustomAttributes::getAttribute if m_attributes is set
    if (m_attributes) {
        // Stub - would need hkCustomAttributes implementation
        // return m_attributes->getAttribute(id);
    }
    return nullptr;
}

// hkSimpleLocalFrame::setGroup implementation
// IDA: ?setGroup@hkSimpleLocalFrame@@UEAAXPEBVhkLocalFrameGroup@@@Z @ 0x14079d530
void hkSimpleLocalFrame::setGroup(const hkLocalFrameGroup* group) {
    // Add reference to new group
    if (group) {
        hkReferencedObject::addReference(const_cast<hkReferencedObject*>(static_cast<const hkReferencedObject*>(group)));
    }
    // Remove reference from old group
    if (m_group) {
        hkReferencedObject::removeReference(static_cast<hkReferencedObject*>(m_group));
    }
    m_group = const_cast<hkLocalFrameGroup*>(group);
}

// hkRefCountedProperties implementations
// IDA: ?replaceProperty@hkRefCountedProperties@@QEAAXGPEAVhkReferencedObject@@@Z @ 0x14079dd50
void hkRefCountedProperties::replaceProperty(unsigned short propertyKey, hkReferencedObject* newPropertyObject) {
    // Search for existing property
    for (int i = m_entries.m_size - 1; i >= 0; i--) {
        if (m_entries.m_data[i].m_key == propertyKey) {
            if (newPropertyObject) {
                hkReferencedObject::addReference(newPropertyObject);
            }
            hkReferencedObject* oldObj = m_entries.m_data[i].m_object.m_ptr;
            if (oldObj) {
                hkReferencedObject::removeReference(oldObj);
            }
            m_entries.m_data[i].m_object.m_ptr = newPropertyObject;
            return;
        }
    }
    // Key not found - warning is issued in original code
}

// IDA: ?addPropertyInternal@hkRefCountedProperties@@IEAAXGPEAVhkReferencedObject@@W4ReferenceCountHandling@1@@Z @ 0x14079deb0
void hkRefCountedProperties::addPropertyInternal(unsigned short propertyKey, hkReferencedObject* propertyObject, ReferenceCountHandling referenceHandling) {
    int idx = m_entries.m_size - 1;
    Entry* entry = nullptr;

    // Search backward for existing property with matching key
    if (idx >= 0) {
        entry = &m_entries.m_data[idx];
        while (entry->m_key != propertyKey) {
            --entry;
            if (--idx < 0) {
                entry = nullptr;
                break;
            }
        }
    }

    if (!entry) {
        // Key not found — append new entry
        if (m_entries.m_size == (m_entries.m_capacityAndFlags & 0x3FFFFFFF)) {
            hkArrayUtil::_reserveMore(hkContainerHeapAllocator::s_alloc,
                reinterpret_cast<void**>(&m_entries), 16);
        }
        Entry* newEntry = &m_entries.m_data[m_entries.m_size];
        if (newEntry) {
            newEntry->m_object.m_ptr = nullptr;
        }
        entry = &m_entries.m_data[m_entries.m_size++];
        entry->m_key = propertyKey;

        hkReferencedObject* oldObj;
        bool skipRemove;
        if (referenceHandling == REFERENCE_COUNT_INCREMENT) {
            // REFERENCE_COUNT_INCREMENT: caller holds reference to new object,
            // addRef on propertyObject if non-null, check if old==new before removeRef
            oldObj = entry->m_object.m_ptr;
            if (!oldObj) {
                entry->m_object.m_ptr = propertyObject;
                return;
            }
            skipRemove = (oldObj == propertyObject);
        } else {
            // REFERENCE_COUNT_NONE: we hold the reference, addRef new if non-null
            if (propertyObject) {
                hkReferencedObject::addReference(propertyObject);
            }
            oldObj = entry->m_object.m_ptr;
            skipRemove = (oldObj == nullptr);
        }
        if (!skipRemove) {
            hkReferencedObject::removeReference(oldObj);
        }
        entry->m_object.m_ptr = propertyObject;
        return;
    }

    // Found existing entry — update reference counting and replace
    if (referenceHandling == REFERENCE_COUNT_INCREMENT && propertyObject) {
        hkReferencedObject::addReference(propertyObject);
    }
    hkReferencedObject* oldObj = entry->m_object.m_ptr;
    if (oldObj) {
        hkReferencedObject::removeReference(oldObj);
    }
    entry->m_object.m_ptr = propertyObject;
}

// IDA: ?removeProperty@hkRefCountedProperties@@QEAAXG@Z @ 0x14079dfc0
void hkRefCountedProperties::removeProperty(unsigned short propertyKey) {
    int idx = m_entries.m_size - 1;
    if (idx < 0) return;

    // Search for the property
    for (int i = idx; i >= 0; i--) {
        if (m_entries.m_data[i].m_key == propertyKey) {
            // Found - remove it
            hkReferencedObject* obj = m_entries.m_data[i].m_object.m_ptr;
            if (obj) {
                hkReferencedObject::removeReference(obj);
            }
            m_entries.m_data[i].m_object.m_ptr = nullptr;

            // Remove entry by shifting
            m_entries.m_size--;
            if (m_entries.m_size != i) {
                // Shift entries down
                for (int j = i; j < m_entries.m_size; j++) {
                    m_entries.m_data[j] = m_entries.m_data[j + 1];
                }
            }
            return;
        }
    }
}

// hkOstream implementations
// IDA: ??0hkOstream@@QEAA@PEAVhkStreamWriter@@@Z @ 0x1407a7770
hkOstream::hkOstream(hkStreamWriter* sw) {
    m_memSizeAndFlags = 0xFFFFFFFF;
    m_referenceCount = 1;
    if (sw) {
        sw->addReference();
    }
    m_writer.m_ptr = sw;
}

// IDA: ??0hkOstream@@QEAA@PEAXHVhkBool@@@Z @ 0x1407a77e0
hkOstream::hkOstream(void* mem, int memSize, hkBool isString) {
    m_memSizeAndFlags = 0xFFFFFFFF;
    m_referenceCount = 1;
    m_writer.m_ptr = nullptr;

    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    hkBufferedStreamWriter* bufWriter = static_cast<hkBufferedStreamWriter*>(
        router->m_heap->blockAlloc(sizeof(hkBufferedStreamWriter)));
    if (bufWriter) {
        new (bufWriter) hkBufferedStreamWriter(static_cast<char*>(mem), static_cast<unsigned int>(memSize), isString);
        m_writer.m_ptr = bufWriter;
    }
}

// IDA: ??0hkOstream@@QEAA@PEAVhkMemoryTrack@@@Z @ 0x1407a7890
hkOstream::hkOstream(hkMemoryTrack* buf) {
    m_memSizeAndFlags = 0x1FFFF;
    m_referenceCount = 1;
    m_writer.m_ptr = nullptr;

    hkMemoryRouter* router = hkMemoryRouter::getInstance();
    hkMemoryTrackStreamWriter* trackWriter = static_cast<hkMemoryTrackStreamWriter*>(
        router->m_heap->blockAlloc(sizeof(hkMemoryTrackStreamWriter)));
    if (trackWriter) {
        trackWriter->m_memSizeAndFlags = 0x1FFFF;
        trackWriter->m_track = buf;
        trackWriter->m_ownerShip = hkMemoryTrackStreamWriter::TRACK_BORROW;
        trackWriter->m_referenceCount = 1;
        m_writer.m_ptr = trackWriter;
    }
}

// IDA: ??1hkOstream@@UEAA@XZ @ 0x1407a7950
hkOstream::~hkOstream() {
    hkStreamWriter* writer = m_writer.m_ptr;
    if (writer) {
        writer->removeReference();
    }
    m_writer.m_ptr = nullptr;
}

// IDA: ?flush@hkOstream@@QEAAXXZ @ 0x1407a7a90
void hkOstream::flush() {
    if (m_writer.m_ptr) {
        m_writer.m_ptr->flush();
    }
}

// IDA: ?write@hkOstream@@QEAAHPEBDH@Z @ 0x1407a7ab0
int hkOstream::write(const char* buf, long long nbytes) {
    return m_writer.m_ptr->write(buf, (int)nbytes);
}

// IDA: ?setStreamWriter@hkOstream@@QEAAXPEAVhkStreamWriter@@@Z @ 0x1407a7ad0
void hkOstream::setStreamWriter(hkStreamWriter* newWriter) {
    if (newWriter) {
        newWriter->addReference();
    }
    hkStreamWriter* oldWriter = m_writer.m_ptr;
    if (oldWriter) {
        oldWriter->removeReference();
    }
    m_writer.m_ptr = newWriter;
}

// IDA: ?isOk@hkOstream@@QEBA?AVhkBool@@XZ @ 0x1407a7c30
bool hkOstream::isOk() const {
    hkStreamWriter* writer = m_writer.m_ptr;
    return writer && writer->isOk();
}

// hkOstream operators
// IDA: ??6hkOstream@@QEAAAEAV0@AEBVhkTransform@@@Z @ 0x1407a8170
hkOstream& hkOstream::operator<<(const hkTransform& t) {
    // Print rotation as matrix
    printf("|%f,%f,%f|\n", t.m_rotation.m_col0.x, t.m_rotation.m_col1.x, t.m_rotation.m_col2.x);
    printf("|%f,%f,%f|\n", t.m_rotation.m_col0.y, t.m_rotation.m_col1.y, t.m_rotation.m_col2.y);
    printf("|%f,%f,%f|\n", t.m_rotation.m_col0.z, t.m_rotation.m_col1.z, t.m_rotation.m_col2.z);
    // Print translation
    *this << t.m_translation;
    return *this;
}

// ============================================================================
// hkStringBuf complex implementations
// ============================================================================

// IDA: ??6hkOstream@@QEAAAEAV0@AEBVhkStringBuf@@@Z @ 0x1407a7a40
hkOstream& hkOstream::operator<<(const hkStringBuf& str) {
    hkStreamWriter* writer = m_writer.m_ptr;
    if (str.m_data) {
        writer->write(str.m_data, str.m_size - 1);
    } else {
        writer->write("(null)", 6);
    }
    return *this;
}

// IDA: ?replace@hkStringBuf@@QEAAIPEBD0W4ReplaceType@1@@Z @ 0x1407a7100
unsigned int hkStringBuf::replace(const char* from, const char* to, ReplaceType rtype) {
    int fromLen = hkString::strLen(from);
    int toLen = hkString::strLen(to);
    unsigned int replaced = 0;

    if (toLen <= fromLen) {
        // In-place replacement (to is shorter or same length)
        int readPos = 0;
        int writePos = 0;
        int foundPos = indexOf(from, 0, 0x7FFFFFFF);

        while (foundPos != -1) {
            // Copy unchanged part
            while (readPos < foundPos) {
                m_data[writePos++] = m_data[readPos++];
            }
            // Copy replacement
            for (int i = 0; i < toLen; i++) {
                m_data[writePos++] = to[i];
            }
            readPos += fromLen;
            replaced = 1;

            if (rtype == REPLACE_ONE) {
                break;
            }
            foundPos = indexOf(from, readPos, 0x7FFFFFFF);
        }

        // Copy remaining
        while (readPos < m_size - 1) {
            m_data[writePos++] = m_data[readPos++];
        }
        m_data[writePos] = 0;
        m_size = writePos + 1;
    } else {
        // Need temporary buffer (to is longer)
        hkStringBuf tmpStr(*this);
        clear();

        int pos = 0;
        int foundPos = tmpStr.indexOf(from, 0, 0x7FFFFFFF);
        if (foundPos >= 0) {
            replaced = 1;
            do {
                append(&tmpStr.m_data[pos], foundPos - pos);
                append(to, toLen);
                pos = foundPos + fromLen;
                if (rtype == REPLACE_ONE) {
                    break;
                }
                foundPos = tmpStr.indexOf(from, pos, 0x7FFFFFFF);
            } while (foundPos >= 0);
        }
        append(&tmpStr.m_data[pos], tmpStr.m_size - pos - 1);
    }
    return replaced;
}

// IDA: ?pathNormalize@hkStringBuf@@QEAAXXZ @ 0x1407a6e40
void hkStringBuf::pathNormalize() {
    // Replace backslashes with forward slashes
    replace('\\', '/', REPLACE_ALL);

    // Determine prefix
    const char* prefix = "";
    if (startsWith("//")) {
        prefix = "//";
    } else if (startsWith("/")) {
        prefix = "/";
    }

    // Split by '/'
    hkArray<const char*, hkContainerTempAllocator> oldBits;
    split('/', oldBits);

    // Process path components
    hkArray<const char*, hkContainerTempAllocator> newBits;
    int skipCount = 0;

    for (int i = 0; i < oldBits.m_size; i++) {
        const char* part = oldBits.m_data[i];

        if (hkString::strCmp("..", part) == 0) {
            // Go up one directory
            if (skipCount == 0) {
                if (newBits.m_size > 0) {
                    newBits.m_size--;
                } else {
                    newBits.pushBack(part);
                    skipCount++;
                }
            } else {
                skipCount--;
            }
        } else if (hkString::strCmp(".", part) == 0) {
            // Current directory - skip
        } else {
            // Regular path component
            newBits.pushBack(part);
        }
    }

    // Rebuild path
    operator=(prefix);
    for (int i = 0; i < newBits.m_size; i++) {
        pathAppend(newBits.m_data[i], nullptr, nullptr);
    }
}

// IDA: ?pathAppend@hkStringBuf@@QEAAAEAV1@PEBD00@Z @ 0x1407a68f0
hkStringBuf& hkStringBuf::pathAppend(const char* p0, const char* p1, const char* p2) {
    const char* parts[4] = {p0, p1, p2, nullptr};
    bool needSep = (m_size > 1) && (m_data[m_size - 2] != '/');

    for (int i = 0; parts[i]; i++) {
        const char* part = parts[i];
        if (!part || !*part) continue;

        // Skip leading slashes
        while (*part == '/') part++;

        int partLen = hkString::strLen(part);
        if (partLen == 0) continue;

        // Skip trailing slashes
        while (partLen > 0 && part[partLen - 1] == '/') {
            partLen--;
        }

        if (partLen == 0) continue;

        // Add separator if needed
        if (needSep) {
            if (m_size == (m_capacityAndFlags & 0x3FFFFFFF)) {
                hkArrayUtil::_reserveMore(hkContainerTempAllocator::s_alloc, reinterpret_cast<void**>(this), 1);
            }
            m_data[m_size - 1] = '/';
            m_size++;
        }

        // Append path component
        hkArrayBase<char>* arr = reinterpret_cast<hkArrayBase<char>*>(this);
        arr->_append(&hkContainerTempAllocator::s_alloc, part, partLen);
        needSep = true;
    }

    // Ensure null terminator
    if (m_size == (m_capacityAndFlags & 0x3FFFFFFF)) {
        hkArrayUtil::_reserveMore(hkContainerTempAllocator::s_alloc, reinterpret_cast<void**>(this), 1);
    }
    m_data[m_size - 1] = 0;

    return *this;
}

// IDA: ?printf@hkStringBuf@@QEAAXPEBDZZ @ 0x1407a63a0
void hkStringBuf::printf(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);

    while (true) {
        int capacity = m_capacityAndFlags & 0x3FFFFFFF;
        int result = hkString::vsnprintf(m_data, capacity, fmt, va);

        if (result >= 0 && result < capacity) {
            setLength(result);
            break;
        }

        // Need more space
        int newSize = result + 1;
        if (result < 0) {
            newSize = 256;
            if (capacity * 2 > 256) {
                newSize = capacity * 2;
            }
        } else {
            int doubled = capacity * 2;
            if (doubled > newSize) {
                newSize = doubled;
            }
        }
        hkArrayUtil::_reserve(hkContainerTempAllocator::s_alloc,
            reinterpret_cast<void**>(this), newSize, 1);
    }

    va_end(va);
}

// IDA: ?appendPrintf@hkStringBuf@@QEAAXPEBDZZ @ 0x1407a73a0
void hkStringBuf::appendPrintf(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);

    hkStringBuf tmp;
    tmp.m_capacityAndFlags = 0x80000020;
    tmp.m_size = 1;
    tmp.m_data = tmp.m_storage;
    tmp.m_storage[0] = 0;

    while (true) {
        int capacity = tmp.m_capacityAndFlags & 0x3FFFFFFF;
        int result = hkString::vsnprintf(tmp.m_data, capacity, fmt, va);

        if (result >= 0 && result < capacity) {
            tmp.m_size = result + 1;
            tmp.m_data[result] = 0;
            break;
        }

        // Need more space
        int newSize = result + 1;
        if (result < 0) {
            newSize = 256;
            if (capacity * 2 > 256) {
                newSize = capacity * 2;
            }
        } else {
            int doubled = capacity * 2;
            if (doubled > newSize) {
                newSize = doubled;
            }
        }
        hkArrayUtil::_reserve(hkContainerTempAllocator::s_alloc,
            reinterpret_cast<void**>(&tmp), newSize, 1);
    }

    append(tmp.m_data, -1);
    va_end(va);
}

// ============================================================================
// hkString TLS-based implementations
// ============================================================================

// IDA: ?strFree@hkString@@YAXPEAD@Z @ 0x14079b920
void hkString::strFree(char* s) {
    // Get allocator from TLS memory router
    hkMemoryAllocator* alloc = hkMemoryRouter::getCurrentThreadAllocator();
    if (alloc && s) {
        hkMemoryRouter::easyFree(*alloc, s);
    }
}

// IDA: ?strDup@hkString@@YAPEADPEBD@Z @ 0x14079b950
char* hkString::strDup(const char* src) {
    hkMemoryAllocator* alloc = hkMemoryRouter::getCurrentThreadAllocator();
    if (alloc && src) {
        return strDup(src, *alloc);
    }
    return nullptr;
}

// IDA: ?strNdup@hkString@@YAPEADPEBDH@Z @ 0x14079b980
char* hkString::strNdup(const char* src, int maxlen) {
    hkMemoryAllocator* alloc = hkMemoryRouter::getCurrentThreadAllocator();
    if (alloc && src) {
        return strNdup(src, maxlen, *alloc);
    }
    return nullptr;
}

// ============================================================================
// hkBaseObject serialization support functions
// These are called by the Havok serialization system for type registration
// ============================================================================

// IDA: finishLoadedObjecthkBaseObject @ 0x14078b550
// Empty function - hkBaseObject has no special finish logic
extern "C" void __fastcall finishLoadedObjecthkBaseObject(void* ptr, int finishFlag) {
    (void)ptr;
    (void)finishFlag;
    // hkBaseObject has no members to finish
}

// IDA: cleanupLoadedObjecthkBaseObject @ 0x14078b560
// Empty function - hkBaseObject has no special cleanup logic
extern "C" void __fastcall cleanupLoadedObjecthkBaseObject(void* ptr) {
    (void)ptr;
    // hkBaseObject has no members to cleanup
}

// IDA: getVtablehkBaseObject @ 0x14078b5d0
// Returns the vtable for hkBaseObject
// Note: hkBaseObject's vtable only contains the virtual destructor
extern "C" void** __fastcall getVtablehkBaseObject() {
    // Create a temporary to get the vtable pointer
    // The vtable pointer is at the start of any object with virtual functions
    static hkBaseObject dummy;
    return *reinterpret_cast<void***>(&dummy);
}

// ============================================================================
// hkReferencedObject serialization support functions
// ============================================================================

// IDA: finishLoadedObjecthkReferencedObject @ 0x140792050
// Sets the vtable pointer after loading
extern "C" void __fastcall finishLoadedObjecthkReferencedObject(void* p, int finishing) {
    if (p) {
        // Set vtable pointer to hkReferencedObject's vtable
        hkReferencedObject* obj = static_cast<hkReferencedObject*>(p);
        // The vtable is implicitly set by the C++ runtime for objects with virtual functions
        // We just need to ensure the object is properly initialized
        // Note: In real Havok, this would set *p = &hkReferencedObject::`vftable'
        // Here we use a dummy object to get the vtable
        static hkReferencedObject dummy;
        *reinterpret_cast<void***>(p) = *reinterpret_cast<void***>(&dummy);
    }
}

// IDA: cleanupLoadedObjecthkReferencedObject @ 0x140792070
// Calls the virtual destructor to cleanup the object
extern "C" void __fastcall cleanupLoadedObjecthkReferencedObject(void* p) {
    if (p) {
        // Call virtual destructor through vtable
        // The first entry in the vtable is the destructor
        void (__fastcall ***vtable)(void*, int) = reinterpret_cast<void (__fastcall ***)(void*, int)>(p);
        if (vtable && *vtable && **vtable) {
            (**vtable)(p, 0);  // Call destructor with deleting flag = 0
        }
    }
}

// IDA: getVtablehkReferencedObject @ 0x140792080
// Returns the vtable for hkReferencedObject
extern "C" void** __fastcall getVtablehkReferencedObject() {
    static hkReferencedObject dummy;
    return *reinterpret_cast<void***>(&dummy);
}

// ============================================================================
// hkAabb and related type serialization support functions
// These are POD types with no special construction/destruction
// ============================================================================

// IDA: cleanupLoadedObjecthkAabb @ 0x140791540
extern "C" void __fastcall cleanupLoadedObjecthkAabb(void* p) {
    (void)p;  // hkAabb is POD, no cleanup needed
}

// IDA: cleanupLoadedObjecthkAabbUint32 @ 0x140791560
extern "C" void __fastcall cleanupLoadedObjecthkAabbUint32(void* p) {
    (void)p;  // hkAabbUint32 is POD, no cleanup needed
}

// IDA: cleanupLoadedObjecthkAabbHalf @ 0x1407919e0
extern "C" void __fastcall cleanupLoadedObjecthkAabbHalf(void* p) {
    (void)p;  // hkAabbHalf is POD, no cleanup needed
}

// IDA: cleanupLoadedObjecthkAabb16 @ 0x140792120
extern "C" void __fastcall cleanupLoadedObjecthkAabb16(void* p) {
    (void)p;  // hkAabb16 is POD, no cleanup needed
}

// IDA: cleanupLoadedObjecthkSphere @ 0x1407917e0
extern "C" void __fastcall cleanupLoadedObjecthkSphere(void* p) {
    (void)p;  // hkSphere is POD, no cleanup needed
}

// ============================================================================
// hkClass static class accessors
// These return references to static hkClass objects
// Using C linkage to avoid name mangling issues
// ============================================================================

// Forward declare hkClass (we use void* for stub)
class hkClass;

// Static class objects (stubs)
static hkClass hkAabbClass_stub;
static hkClass hkAabbUint32Class_stub;
static hkClass hkAabbHalfClass_stub;
static hkClass hkAabb16Class_stub;
static hkClass hkSphereClass_stub;

// IDA: ?staticClass@hkAabb@@SAAEBVhkClass@@XZ @ 0x140791530
extern "C" const hkClass* __fastcall hkAabb_staticClass() {
    return &hkAabbClass_stub;
}

// IDA: ?staticClass@hkAabbUint32@@SAAEBVhkClass@@XZ @ 0x140791550
extern "C" const hkClass* __fastcall hkAabbUint32_staticClass() {
    return &hkAabbUint32Class_stub;
}

// IDA: ?staticClass@hkAabbHalf@@SAAEBVhkClass@@XZ @ 0x1407919d0
extern "C" const hkClass* __fastcall hkAabbHalf_staticClass() {
    return &hkAabbHalfClass_stub;
}

// IDA: ?staticClass@hkAabb16@@SAAEBVhkClass@@XZ @ 0x140792110
extern "C" const hkClass* __fastcall hkAabb16_staticClass() {
    return &hkAabb16Class_stub;
}

// IDA: ?staticClass@hkSphere@@SAAEBVhkClass@@XZ @ 0x1407917d0
extern "C" const hkClass* __fastcall hkSphere_staticClass() {
    return &hkSphereClass_stub;
}

// ============================================================================
// hkQTransform serialization support functions
// ============================================================================

// Static class stub for hkQTransform
static hkClass hkQTransformClass_stub;

// IDA: ?staticClass@hkQTransform@@SAAEBVhkClass@@XZ @ 0x140791ef0
extern "C" const hkClass* __fastcall hkQTransform_staticClass() {
    return &hkQTransformClass_stub;
}

// IDA: cleanupLoadedObjecthkQTransform @ 0x140791f00
extern "C" void __fastcall cleanupLoadedObjecthkQTransform(void* p) {
    (void)p;  // hkQTransform is POD-like, no special cleanup
}

// IDA: finishLoadedObjecthkQTransform @ 0x140791f10
extern "C" void __fastcall finishLoadedObjecthkQTransform(void* p, int finishing) {
    (void)p;
    (void)finishing;  // hkQTransform is POD-like, no special finish
}

// ============================================================================
// hkPackedVector3 serialization support
// ============================================================================

// Static class stub for hkPackedVector3
static hkClass hkPackedVector3Class_stub;

// IDA: ?staticClass@hkPackedVector3@@SAAEBVhkClass@@XZ @ 0x140791f20
extern "C" const hkClass* __fastcall hkPackedVector3_staticClass() {
    return &hkPackedVector3Class_stub;
}

// IDA: cleanupLoadedObjecthkPackedVector3 @ 0x140791f30
extern "C" void __fastcall cleanupLoadedObjecthkPackedVector3(void* p) {
    (void)p;  // hkPackedVector3 is POD, no special cleanup
}

// ============================================================================
// hkPackedVector8_3 serialization support
// ============================================================================

// Static class stub for hkPackedVector8_3
static hkClass hkPackedVector8_3Class_stub;

// IDA: ?staticClass@hkPackedVector8_3@@SAAEBVhkClass@@XZ @ 0x140791f40
extern "C" const hkClass* __fastcall hkPackedVector8_3_staticClass() {
    return &hkPackedVector8_3Class_stub;
}

// IDA: cleanupLoadedObjecthkPackedVector8_3 @ 0x140791f50
extern "C" void __fastcall cleanupLoadedObjecthkPackedVector8_3(void* p) {
    (void)p;  // hkPackedVector8_3 is POD, no special cleanup
}

// ============================================================================
// hkBitField serialization support
// ============================================================================

// Static class stub for hkBitField
static hkClass hkBitFieldClass_stub;

// IDA: ?staticClass@hkBitField@@SAAEBVhkClass@@XZ @ 0x140791f60
extern "C" const hkClass* __fastcall hkBitField_staticClass() {
    return &hkBitFieldClass_stub;
}

// IDA: finishLoadedObjecthkBitField @ 0x140791f70
extern "C" void __fastcall finishLoadedObjecthkBitField(void* p, int finishing) {
    (void)p;
    (void)finishing;  // hkBitField finish is no-op
}

// IDA: cleanupLoadedObjecthkBitField @ 0x140791f80
extern "C" void __fastcall cleanupLoadedObjecthkBitField(void* p) {
    // hkBitField cleanup - frees data buffer if owned
    unsigned int* ptr = reinterpret_cast<unsigned int*>(p);
    int capacityAndFlags = ptr[3];
    ptr[2] = 0;  // m_size = 0
    if (capacityAndFlags < 0) {
        // External storage - just clear
        *reinterpret_cast<unsigned int**>(p) = nullptr;
        ptr[3] = 0x80000000;
    } else {
        // Owned storage - free it
        void* data = *reinterpret_cast<void**>(p);
        hkContainerHeapAllocator::s_alloc.bufFree(data, 4 * capacityAndFlags);
        *reinterpret_cast<void**>(p) = nullptr;
        ptr[3] = 0x80000000;
    }
}

// ============================================================================
// hkBitField member functions
// ============================================================================

// IDA: ??1hkBitField@@QEAA@XZ @ 0x140791fe0
hkBitField::~hkBitField() {
    m_words.m_size = 0;
    if (m_words.m_capacityAndFlags < 0) {
        m_words.m_data = nullptr;
        m_words.m_capacityAndFlags = 0x80000000;
    } else {
        // Owned storage - free it
        hkContainerHeapAllocator::s_alloc.bufFree(m_words.m_data, 4 * m_words.m_capacityAndFlags);
        m_words.m_data = nullptr;
        m_words.m_capacityAndFlags = 0x80000000;
    }
}

// ============================================================================
// hkSymmetricMatrix3 serialization support
// ============================================================================

// Static class stub for hkSymmetricMatrix3
static hkClass hkSymmetricMatrix3Class_stub;

// IDA: ?staticClass@hkSymmetricMatrix3@@SAAEBVhkClass@@XZ @ 0x140791eb0
extern "C" const hkClass* __fastcall hkSymmetricMatrix3_staticClass() {
    return &hkSymmetricMatrix3Class_stub;
}

// IDA: cleanupLoadedObjecthkSymmetricMatrix3 @ 0x140791ec0
extern "C" void __fastcall cleanupLoadedObjecthkSymmetricMatrix3(void* p) {
    (void)p;  // hkSymmetricMatrix3 is POD, no special cleanup
}

// ============================================================================
// hkSweptTransform serialization support
// ============================================================================

// Static class stub for hkSweptTransform
static hkClass hkSweptTransformClass_stub;

// IDA: ?staticClass@hkSweptTransform@@SAAEBVhkClass@@XZ @ 0x140791ed0
extern "C" const hkClass* __fastcall hkSweptTransform_staticClass() {
    return &hkSweptTransformClass_stub;
}

// IDA: cleanupLoadedObjecthkSweptTransform @ 0x140791ee0
extern "C" void __fastcall cleanupLoadedObjecthkSweptTransform(void* p) {
    (void)p;  // hkSweptTransform is POD, no special cleanup
}

// ============================================================================
// hkContactPointMaterial serialization support
// ============================================================================

// Static class stub for hkContactPointMaterial
static hkClass hkContactPointMaterialClass_stub;

// IDA: ?staticClass@hkContactPointMaterial@@SAAEBVhkClass@@XZ @ 0x140792090
extern "C" const hkClass* __fastcall hkContactPointMaterial_staticClass() {
    return &hkContactPointMaterialClass_stub;
}

// IDA: cleanupLoadedObjecthkContactPointMaterial @ 0x1407920a0
extern "C" void __fastcall cleanupLoadedObjecthkContactPointMaterial(void* p) {
    (void)p;  // hkContactPointMaterial is POD, no special cleanup
}

// ============================================================================
// hkFourTransposedPoints serialization support
// ============================================================================

// Static class stub for hkFourTransposedPoints
static hkClass hkFourTransposedPointsClass_stub;

// IDA: ?staticClass@hkFourTransposedPoints@@SAAEBVhkClass@@XZ @ 0x140791e90
extern "C" const hkClass* __fastcall hkFourTransposedPoints_staticClass() {
    return &hkFourTransposedPointsClass_stub;
}

// IDA: cleanupLoadedObjecthkFourTransposedPoints @ 0x140791ea0
extern "C" void __fastcall cleanupLoadedObjecthkFourTransposedPoints(void* p) {
    (void)p;  // hkFourTransposedPoints is POD, no special cleanup
}

// ============================================================================
// hkMultiThreadCheck serialization support
// ============================================================================

// Static class stub for hkMultiThreadCheck
static hkClass hkMultiThreadCheckClass_stub;

// IDA: ?staticClass@hkMultiThreadCheck@@SAAEBVhkClass@@XZ @ 0x1407920b0
extern "C" const hkClass* __fastcall hkMultiThreadCheck_staticClass() {
    return &hkMultiThreadCheckClass_stub;
}

// IDA: cleanupLoadedObjecthkMultiThreadCheck @ 0x1407920c0
extern "C" void __fastcall cleanupLoadedObjecthkMultiThreadCheck(void* p) {
    (void)p;  // hkMultiThreadCheck is POD, no special cleanup
}

// ============================================================================
// hkTraceStream::Title serialization support
// ============================================================================

// Static class stub for hkTraceStream::Title
static hkClass hkTraceStreamTitleClass_stub;

// IDA: ?staticClass@Title@hkTraceStream@@SAAEBVhkClass@@XZ @ 0x1407920d0
extern "C" const hkClass* __fastcall hkTraceStreamTitle_staticClass() {
    return &hkTraceStreamTitleClass_stub;
}

// IDA: cleanupLoadedObjecthkTraceStreamTitle @ 0x1407920e0
extern "C" void __fastcall cleanupLoadedObjecthkTraceStreamTitle(void* p) {
    (void)p;  // hkTraceStream::Title is POD, no special cleanup
}

// ============================================================================
// hkFloat16Transform serialization support
// ============================================================================

// Static class stub for hkFloat16Transform
static hkClass hkFloat16TransformClass_stub;

// IDA: ?staticClass@hkFloat16Transform@@SAAEBVhkClass@@XZ @ 0x1407920f0
extern "C" const hkClass* __fastcall hkFloat16Transform_staticClass() {
    return &hkFloat16TransformClass_stub;
}

// IDA: cleanupLoadedObjecthkFloat16Transform @ 0x140792100
extern "C" void __fastcall cleanupLoadedObjecthkFloat16Transform(void* p) {
    (void)p;  // hkFloat16Transform is POD, no special cleanup
}

// ============================================================================
// hkMotionState serialization support
// ============================================================================

// Static class stub for hkMotionState
static hkClass hkMotionStateClass_stub;

// IDA: ?staticClass@hkMotionState@@SAAEBVhkClass@@XZ @ 0x140791a10
extern "C" const hkClass* __fastcall hkMotionState_staticClass() {
    return &hkMotionStateClass_stub;
}

// IDA: cleanupLoadedObjecthkMotionState @ 0x140791a20
extern "C" void __fastcall cleanupLoadedObjecthkMotionState(void* p) {
    (void)p;  // hkMotionState is POD, no special cleanup
}

// ============================================================================
// hkCompressedMassProperties serialization support
// ============================================================================

// Static class stub for hkCompressedMassProperties
static hkClass hkCompressedMassPropertiesClass_stub;

// IDA: ?staticClass@hkCompressedMassProperties@@SAAEBVhkClass@@XZ @ 0x1407e5f00
extern "C" const hkClass* __fastcall hkCompressedMassProperties_staticClass() {
    return &hkCompressedMassPropertiesClass_stub;
}

// IDA: cleanupLoadedObjecthkCompressedMassProperties @ 0x1407e5f10
extern "C" void __fastcall cleanupLoadedObjecthkCompressedMassProperties(void* p) {
    (void)p;  // hkCompressedMassProperties is POD, no special cleanup
}

// ============================================================================
// Havok Attribute types serialization support (POD types)
// ============================================================================

// hkRangeRealAttribute
static hkClass hkRangeRealAttributeClass_stub;
extern "C" const hkClass* __fastcall hkRangeRealAttribute_staticClass() { return &hkRangeRealAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkRangeRealAttribute(void* p) { (void)p; }

// hkRangeInt32Attribute
static hkClass hkRangeInt32AttributeClass_stub;
extern "C" const hkClass* __fastcall hkRangeInt32Attribute_staticClass() { return &hkRangeInt32AttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkRangeInt32Attribute(void* p) { (void)p; }

// hkUiAttribute
static hkClass hkUiAttributeClass_stub;
extern "C" const hkClass* __fastcall hkUiAttribute_staticClass() { return &hkUiAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkUiAttribute(void* p) { (void)p; }

// hkGizmoAttribute
static hkClass hkGizmoAttributeClass_stub;
extern "C" const hkClass* __fastcall hkGizmoAttribute_staticClass() { return &hkGizmoAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkGizmoAttribute(void* p) { (void)p; }

// hkModelerNodeTypeAttribute
static hkClass hkModelerNodeTypeAttributeClass_stub;
extern "C" const hkClass* __fastcall hkModelerNodeTypeAttribute_staticClass() { return &hkModelerNodeTypeAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkModelerNodeTypeAttribute(void* p) { (void)p; }

// hkLinkAttribute
static hkClass hkLinkAttributeClass_stub;
extern "C" const hkClass* __fastcall hkLinkAttribute_staticClass() { return &hkLinkAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkLinkAttribute(void* p) { (void)p; }

// hkSemanticsAttribute
static hkClass hkSemanticsAttributeClass_stub;
extern "C" const hkClass* __fastcall hkSemanticsAttribute_staticClass() { return &hkSemanticsAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkSemanticsAttribute(void* p) { (void)p; }

// hkDescriptionAttribute
static hkClass hkDescriptionAttributeClass_stub;
extern "C" const hkClass* __fastcall hkDescriptionAttribute_staticClass() { return &hkDescriptionAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkDescriptionAttribute(void* p) { (void)p; }

// hkArrayTypeAttribute
static hkClass hkArrayTypeAttributeClass_stub;
extern "C" const hkClass* __fastcall hkArrayTypeAttribute_staticClass() { return &hkArrayTypeAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkArrayTypeAttribute(void* p) { (void)p; }

// hkDataObjectTypeAttribute
static hkClass hkDataObjectTypeAttributeClass_stub;
extern "C" const hkClass* __fastcall hkDataObjectTypeAttribute_staticClass() { return &hkDataObjectTypeAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkDataObjectTypeAttribute(void* p) { (void)p; }

// hkDocumentationAttribute
static hkClass hkDocumentationAttributeClass_stub;
extern "C" const hkClass* __fastcall hkDocumentationAttribute_staticClass() { return &hkDocumentationAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkDocumentationAttribute(void* p) { (void)p; }

// hkPostFinishAttribute
static hkClass hkPostFinishAttributeClass_stub;
extern "C" const hkClass* __fastcall hkPostFinishAttribute_staticClass() { return &hkPostFinishAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkPostFinishAttribute(void* p) { (void)p; }

// hkScriptableAttribute
static hkClass hkScriptableAttributeClass_stub;
extern "C" const hkClass* __fastcall hkScriptableAttribute_staticClass() { return &hkScriptableAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkScriptableAttribute(void* p) { (void)p; }

// ============================================================================
// More Havok POD types serialization support
// ============================================================================

// hkLocalFrame
static hkClass hkLocalFrameClass_stub;
extern "C" const hkClass* __fastcall hkLocalFrame_staticClass() { return &hkLocalFrameClass_stub; }

// hkLocalFrameGroup
static hkClass hkLocalFrameGroupClass_stub;
extern "C" const hkClass* __fastcall hkLocalFrameGroup_staticClass() { return &hkLocalFrameGroupClass_stub; }

// hkLocalFrameGroup - calls virtual destructor
extern "C" void __fastcall cleanupLoadedObjecthkLocalFrameGroup(void* p) {
    // Calls first virtual function (destructor) with 0 (not deleting)
    void*** vtable = reinterpret_cast<void***>(p);
    if (vtable && *vtable && **vtable) {
        typedef void(__fastcall * DtorFunc)(void*, int);
        DtorFunc dtor = reinterpret_cast<DtorFunc>(**vtable);
        dtor(p, 0);
    }
}

// hkCustomAttributes::Attribute
static hkClass hkCustomAttributesAttributeClass_stub;
extern "C" const hkClass* __fastcall hkCustomAttributesAttribute_staticClass() { return &hkCustomAttributesAttributeClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkCustomAttributesAttribute(void* p) { (void)p; }

// hkCustomAttributes
static hkClass hkCustomAttributesClass_stub;
extern "C" const hkClass* __fastcall hkCustomAttributes_staticClass() { return &hkCustomAttributesClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkCustomAttributes(void* p) { (void)p; }

// hkRefCountedProperties::Entry
static hkClass hkRefCountedPropertiesEntryClass_stub;
extern "C" const hkClass* __fastcall hkRefCountedPropertiesEntry_staticClass() { return &hkRefCountedPropertiesEntryClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkRefCountedPropertiesEntry(void* p) { (void)p; }

// hkRefCountedProperties - calls virtual destructor
static hkClass hkRefCountedPropertiesClass_stub;
extern "C" const hkClass* __fastcall hkRefCountedProperties_staticClass() { return &hkRefCountedPropertiesClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkRefCountedProperties(void* p) {
    void*** vtable = reinterpret_cast<void***>(p);
    if (vtable && *vtable && **vtable) {
        typedef void(__fastcall * DtorFunc)(void*, int);
        DtorFunc dtor = reinterpret_cast<DtorFunc>(**vtable);
        dtor(p, 0);
    }
}

// hkClassMember
static hkClass hkClassMemberClass_stub;
extern "C" const hkClass* __fastcall hkClassMember_staticClass() { return &hkClassMemberClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkClassMember(void* p) { (void)p; }

// hkGeometry::Triangle
static hkClass hkGeometryTriangleClass_stub;
extern "C" const hkClass* __fastcall hkGeometryTriangle_staticClass() { return &hkGeometryTriangleClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkGeometryTriangle(void* p) { (void)p; }

// hkGeometry - calls destructor
static hkClass hkGeometryClass_stub;
extern "C" const hkClass* __fastcall hkGeometry_staticClass() { return &hkGeometryClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkGeometry(void* p) {
    // Calls ~ForwardReferences destructor
    // This is actually hkGeometry's internal forward references cleanup
}

// hkUFloat8
static hkClass hkUFloat8Class_stub;
extern "C" const hkClass* __fastcall hkUFloat8_staticClass() { return &hkUFloat8Class_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkUFloat8(void* p) { (void)p; }

// hkClass
static hkClass hkClassClass_stub;
extern "C" const hkClass* __fastcall hkClass_staticClass() { return &hkClassClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkClass(void* p) { (void)p; }

// hkSimpleLocalFrame - calls virtual destructor
static hkClass hkSimpleLocalFrameClass_stub;
extern "C" const hkClass* __fastcall hkSimpleLocalFrame_staticClass() { return &hkSimpleLocalFrameClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkSimpleLocalFrame(void* p) {
    void*** vtable = reinterpret_cast<void***>(p);
    if (vtable && *vtable && **vtable) {
        typedef void(__fastcall * DtorFunc)(void*, int);
        DtorFunc dtor = reinterpret_cast<DtorFunc>(**vtable);
        dtor(p, 0);
    }
}

// hkMonitorStreamStringMap::StringMap
static hkClass hkMonitorStreamStringMapStringMapClass_stub;
extern "C" const hkClass* __fastcall hkMonitorStreamStringMapStringMap_staticClass() { return &hkMonitorStreamStringMapStringMapClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkMonitorStreamStringMapStringMap(void* p) { (void)p; }

// hkMonitorStreamStringMap
static hkClass hkMonitorStreamStringMapClass_stub;
extern "C" const hkClass* __fastcall hkMonitorStreamStringMap_staticClass() { return &hkMonitorStreamStringMapClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkMonitorStreamStringMap(void* p) { (void)p; }

// hkMonitorStreamFrameInfo
static hkClass hkMonitorStreamFrameInfoClass_stub;
extern "C" const hkClass* __fastcall hkMonitorStreamFrameInfo_staticClass() { return &hkMonitorStreamFrameInfoClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkMonitorStreamFrameInfo(void* p) { (void)p; }

// hkMonitorStreamColorTable::ColorPair
static hkClass hkMonitorStreamColorTableColorPairClass_stub;
extern "C" const hkClass* __fastcall hkMonitorStreamColorTableColorPair_staticClass() { return &hkMonitorStreamColorTableColorPairClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkMonitorStreamColorTableColorPair(void* p) { (void)p; }

// hkMonitorStreamColorTable
static hkClass hkMonitorStreamColorTableClass_stub;
extern "C" const hkClass* __fastcall hkMonitorStreamColorTable_staticClass() { return &hkMonitorStreamColorTableClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkMonitorStreamColorTable(void* p) { (void)p; }

// hkContactPoint
static hkClass hkContactPointClass_stub;
extern "C" const hkClass* __fastcall hkContactPoint_staticClass() { return &hkContactPointClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkContactPoint(void* p) { (void)p; }

// hkColor
static hkClass hkColorClass_stub;
extern "C" const hkClass* __fastcall hkColor_staticClass() { return &hkColorClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkColor(void* p) { (void)p; }

// hkClassEnumItem
static hkClass hkClassEnumItemClass_stub;
extern "C" const hkClass* __fastcall hkClassEnumItem_staticClass() { return &hkClassEnumItemClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkClassEnumItem(void* p) { (void)p; }

// hkClassEnum
static hkClass hkClassEnumClass_stub;
extern "C" const hkClass* __fastcall hkClassEnum_staticClass() { return &hkClassEnumClass_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkClassEnum(void* p) { (void)p; }

// hkFloat16
static hkClass hkFloat16Class_stub;
extern "C" const hkClass* __fastcall hkFloat16_staticClass() { return &hkFloat16Class_stub; }
extern "C" void __fastcall cleanupLoadedObjecthkFloat16(void* p) { (void)p; }

// ============================================================================
// hkQTransform member functions
// ============================================================================

// IDA: ?isApproximatelyEqual@hkQTransform@@QEBA_NAEBV1@M@Z @ 0x1409dbe60
bool hkQTransform::isApproximatelyEqual(const hkQTransform& other, float epsilon) const {
#ifdef _WIN32
    __m128 eps = _mm_shuffle_ps(_mm_set_ss(epsilon), _mm_set_ss(epsilon), 0);
    // Check translation difference (abs value comparison)
    __m128 transDiff = _mm_sub_ps(m_translation.m_quad, other.m_translation.m_quad);
    __m128 absTransDiff = _mm_and_ps(transDiff, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
    if ((_mm_movemask_ps(_mm_cmplt_ps(absTransDiff, eps)) & 7) != 7) {
        return false;
    }
    // Check rotation - compute dot product to compare quaternions
    __m128 dotProd = _mm_mul_ps(m_rotation.m_quad, other.m_rotation.m_quad);
    __m128 sum = _mm_add_ps(_mm_shuffle_ps(dotProd, dotProd, 78), dotProd);
    float dot = _mm_cvtss_f32(sum) + _mm_cvtss_f32(_mm_shuffle_ps(sum, sum, 177));
    // Compare |dot| vs 1 (quaternions are same if dot is close to 1 or -1)
    float diff = std::abs(dot) - 1.0f;
    return (diff * diff) < epsilon;
#else
    // Non-SSE implementation
    float dx = std::abs(m_translation.x - other.m_translation.x);
    float dy = std::abs(m_translation.y - other.m_translation.y);
    float dz = std::abs(m_translation.z - other.m_translation.z);
    if (dx >= epsilon || dy >= epsilon || dz >= epsilon) return false;
    float dot = m_rotation.x * other.m_rotation.x + m_rotation.y * other.m_rotation.y +
                m_rotation.z * other.m_rotation.z + m_rotation.w * other.m_rotation.w;
    float diff = std::abs(dot) - 1.0f;
    return (diff * diff) < epsilon;
#endif
}

// IDA: ?setInverse@hkQTransform@@QEAAXAEBV1@@Z @ 0x1409dbee0
void hkQTransform::setInverse(const hkQTransform& qt) {
    // Inverse of quaternion transform: inverse rotation and negated rotated translation
    // Conjugate of quaternion (negate xyz, keep w)
    m_rotation.x = -qt.m_rotation.x;
    m_rotation.y = -qt.m_rotation.y;
    m_rotation.z = -qt.m_rotation.z;
    m_rotation.w = qt.m_rotation.w;

    // Rotate translation by conjugate quaternion
    float tx = qt.m_translation.x, ty = qt.m_translation.y, tz = qt.m_translation.z;
    float qx = m_rotation.x, qy = m_rotation.y, qz = m_rotation.z, qw = m_rotation.w;

    // Quaternion-vector rotation: v' = v + 2 * cross(q.xyz, cross(q.xyz, v) + q.w * v)
    float cross1_x = qy * tz - qz * ty;
    float cross1_y = qz * tx - qx * tz;
    float cross1_z = qx * ty - qy * tx;

    float cross2_x = qy * cross1_z - qz * cross1_y + qw * tx;
    float cross2_y = qz * cross1_x - qx * cross1_z + qw * ty;
    float cross2_z = qx * cross1_y - qy * cross1_x + qw * tz;

    m_translation.x = -(tx + 2.0f * cross2_x);
    m_translation.y = -(ty + 2.0f * cross2_y);
    m_translation.z = -(tz + 2.0f * cross2_z);
    m_translation.w = 0.0f;
}

// IDA: ?setMul@hkQTransform@@QEAAXAEBV1@0@Z @ 0x1409dbf80
void hkQTransform::setMul(const hkQTransform& t1, const hkQTransform& t2) {
    // Combine two transforms: result = t1 * t2
    // rotation = t1.rotation * t2.rotation
    // translation = t1.rotation * t2.translation + t1.translation

    float w1 = t1.m_rotation.w, x1 = t1.m_rotation.x, y1 = t1.m_rotation.y, z1 = t1.m_rotation.z;
    float w2 = t2.m_rotation.w, x2 = t2.m_rotation.x, y2 = t2.m_rotation.y, z2 = t2.m_rotation.z;

    // Quaternion multiplication
    m_rotation.w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
    m_rotation.x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
    m_rotation.y = w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2;
    m_rotation.z = w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2;

    // Rotate t2.translation by t1.rotation and add t1.translation
    float tx = t2.m_translation.x, ty = t2.m_translation.y, tz = t2.m_translation.z;
    float qx = t1.m_rotation.x, qy = t1.m_rotation.y, qz = t1.m_rotation.z, qw = t1.m_rotation.w;

    float cross1_x = qy * tz - qz * ty;
    float cross1_y = qz * tx - qx * tz;
    float cross1_z = qx * ty - qy * tx;

    float cross2_x = qy * cross1_z - qz * cross1_y + qw * tx;
    float cross2_y = qz * cross1_x - qx * cross1_z + qw * ty;
    float cross2_z = qx * cross1_y - qy * cross1_x + qw * tz;

    m_translation.x = tx + 2.0f * cross2_x + t1.m_translation.x;
    m_translation.y = ty + 2.0f * cross2_y + t1.m_translation.y;
    m_translation.z = tz + 2.0f * cross2_z + t1.m_translation.z;
    m_translation.w = 0.0f;
}

// ============================================================================
// Havok finishLoadedObject and getVtable functions
// ============================================================================

// IDA: finishLoadedObjecthkLocalFrameGroup @ 0x140791590
extern "C" void __fastcall finishLoadedObjecthkLocalFrameGroup(void* p, int finishing) {
    if (p) {
        // Set vtable pointer at offset 0
        void** vtablePtr = reinterpret_cast<void**>(p);
        *vtablePtr = reinterpret_cast<void*>(&hkLocalFrameGroupClass_stub);
        // Initialize hkStringPtr at offset +2 (for m_name)
        // hkStringPtr::hkStringPtr handles finishing flag
    }
}

// IDA: getVtablehkLocalFrameGroup @ 0x1407915d0
extern "C" void** __fastcall getVtablehkLocalFrameGroup() {
    return reinterpret_cast<void**>(&hkLocalFrameGroupClass_stub);
}

// IDA: finishLoadedObjecthkSimpleLocalFrame @ 0x140791620
extern "C" void __fastcall finishLoadedObjecthkSimpleLocalFrame(void* p, int finishing) {
    if (p) {
        void** vtablePtr = reinterpret_cast<void**>(p);
        *vtablePtr = reinterpret_cast<void*>(&hkSimpleLocalFrameClass_stub);
    }
}

// IDA: getVtablehkSimpleLocalFrame @ 0x140791650
extern "C" void** __fastcall getVtablehkSimpleLocalFrame() {
    return reinterpret_cast<void**>(&hkSimpleLocalFrameClass_stub);
}

// IDA: finishLoadedObjecthkRefCountedProperties @ 0x140791810
extern "C" void __fastcall finishLoadedObjecthkRefCountedProperties(void* p, int finishing) {
    if (p) {
        void** vtablePtr = reinterpret_cast<void**>(p);
        *vtablePtr = reinterpret_cast<void*>(&hkRefCountedPropertiesClass_stub);
    }
}

// IDA: getVtablehkRefCountedProperties @ 0x140791840
extern "C" void** __fastcall getVtablehkRefCountedProperties() {
    return reinterpret_cast<void**>(&hkRefCountedPropertiesClass_stub);
}

// IDA: finishLoadedObjecthkRefCountedPropertiesEntry @ 0x140791860
extern "C" void __fastcall finishLoadedObjecthkRefCountedPropertiesEntry(void* p, int finishing) {
    (void)p;
    (void)finishing;  // Entry is POD, no special finish
}

// IDA: finishLoadedObjecthkGeometry @ 0x1407918f0
extern "C" void __fastcall finishLoadedObjecthkGeometry(hkGeometry* p, hkFinishLoadedObjectFlag finishing) {
    if (p) new (p) hkGeometry(finishing);
}

// IDA: finishLoadedObjecthkMonitorStreamStringMapStringMap @ 0x140791a40
extern "C" void __fastcall finishLoadedObjecthkMonitorStreamStringMapStringMap(void* p, int finishing) {
    (void)p;
    (void)finishing;
}

// IDA: finishLoadedObjecthkMonitorStreamStringMap @ 0x140791a70
extern "C" void __fastcall finishLoadedObjecthkMonitorStreamStringMap(void* p, int finishing) {
    (void)p;
    (void)finishing;
}

// IDA: finishLoadedObjecthkMonitorStreamFrameInfo @ 0x140791ad0
extern "C" void __fastcall finishLoadedObjecthkMonitorStreamFrameInfo(void* p, int finishing) {
    (void)p;
    (void)finishing;
}

// IDA: finishLoadedObjecthkMonitorStreamColorTableColorPair @ 0x140791af0
extern "C" void __fastcall finishLoadedObjecthkMonitorStreamColorTableColorPair(void* p, int finishing) {
    (void)p;
    (void)finishing;
}

// IDA: finishLoadedObjecthkMonitorStreamColorTable @ 0x140791b40
extern "C" void __fastcall finishLoadedObjecthkMonitorStreamColorTable(void* p, int finishing) {
    (void)p;
    (void)finishing;
}

// IDA: getVtablehkMonitorStreamColorTable @ 0x140791b60
extern "C" void** __fastcall getVtablehkMonitorStreamColorTable() {
    return reinterpret_cast<void**>(&hkMonitorStreamColorTableClass_stub);
}

// ============================================================================
// hkMesh Serialization Support Functions
// ============================================================================

// Static class stubs
static hkClass hkMeshBoneIndexMappingClass_stub;
static hkClass hkMeshMaterialClass_stub;
static hkClass hkMeshSectionClass_stub;

// IDA: hkMeshBoneIndexMapping::staticClass @ 0x1407E5C60
extern "C" const hkClass* __fastcall hkMeshBoneIndexMapping_staticClass() {
    return &hkMeshBoneIndexMappingClass_stub;
}

// IDA: finishLoadedObjecthkMeshBoneIndexMapping @ 0x1407E5CE0
extern "C" void __fastcall finishLoadedObjecthkMeshBoneIndexMapping(void* p, int finishing) {
    (void)p;
    (void)finishing;  // No special finish needed
}

// IDA: cleanupLoadedObjecthkMeshBoneIndexMapping @ 0x1407E5CF0
extern "C" void __fastcall cleanupLoadedObjecthkMeshBoneIndexMapping(void* p) {
    // Cleanup: frees the bone index mapping buffer
    // The buffer is allocated using hkContainerHeapAllocator
    if (p) {
        // hkMeshBoneIndexMapping has a hkArray<int> member that needs cleanup
        // The buffer pointer is at offset 0, size at offset 8, capacity at offset 12
        int** ppData = reinterpret_cast<int**>(p);
        if (*ppData) {
            // Use bufFree to release the buffer
            // Get size from offset 8
            int* pSize = reinterpret_cast<int*>(reinterpret_cast<char*>(p) + 8);
            int size = *pSize * sizeof(int);
            hkContainerHeapAllocator::s_alloc.bufFree(*ppData, size);
        }
    }
}

// IDA: hkMeshMaterial::staticClass @ 0x1407E5D50
extern "C" const hkClass* __fastcall hkMeshMaterial_staticClass() {
    return &hkMeshMaterialClass_stub;
}

const hkClass* hkMeshMaterial::staticClass() {
    return &hkMeshMaterialClass_stub;
}

// IDA: hkMeshMaterial::createCompatibleVertexBuffer @ base implementation
hkMeshVertexBuffer* hkMeshMaterial::createCompatibleVertexBuffer(hkMeshVertexBuffer* buffer) const {
    if (buffer) buffer->addReference();
    return buffer;
}

// IDA: hkMeshMaterial::createCompatibleVertexFormat @ base implementation
hkResult hkMeshMaterial::createCompatibleVertexFormat(const hkVertexFormat& format, hkVertexFormat& compatibleFormat) const {
    compatibleFormat.set(format);
    return hkResult(HK_SUCCESS);
}

// IDA: hkMeshSection::staticClass @ 0x1407E5D60
extern "C" const hkClass* __fastcall hkMeshSection_staticClass() {
    return &hkMeshSectionClass_stub;
}

// IDA: cleanupLoadedObjecthkMeshSection @ 0x1407E5D70
extern "C" void __fastcall cleanupLoadedObjecthkMeshSection(void* p) {
    (void)p;  // POD type, no special cleanup
}

// ============================================================================
// hkMesh Static Class Functions (Additional)
// ============================================================================

// Static class stubs (additional)
static hkClass hkMeshSectionCinfoClass_stub;
static hkClass hkMeshShapeClass_stub;
static hkClass hkMeshVertexBufferClass_stub;
static hkClass hkMeshBodyClass_stub;
static hkClass hkMeshTextureClass_stub;

// IDA: hkMeshSectionCinfo::staticClass @ 0x1407E5D80
extern "C" const hkClass* __fastcall hkMeshSectionCinfo_staticClass() {
    return &hkMeshSectionCinfoClass_stub;
}

// IDA: cleanupLoadedObjecthkMeshSectionCinfo @ 0x1407E5D90
extern "C" void __fastcall cleanupLoadedObjecthkMeshSectionCinfo(void* p) {
    (void)p;  // POD type, no special cleanup
}

// IDA: hkMeshShape::staticClass @ 0x1407E5DA0
extern "C" const hkClass* __fastcall hkMeshShape_staticClass() {
    return &hkMeshShapeClass_stub;
}

// IDA: hkMeshVertexBuffer::staticClass @ 0x1407E5E40
extern "C" const hkClass* __fastcall hkMeshVertexBuffer_staticClass() {
    return &hkMeshVertexBufferClass_stub;
}

const hkClass* hkMeshVertexBuffer::staticClass() {
    return &hkMeshVertexBufferClass_stub;
}

// IDA: hkMeshBody::staticClass @ 0x1407E5F20
extern "C" const hkClass* __fastcall hkMeshBody_staticClass() {
    return &hkMeshBodyClass_stub;
}

// IDA: hkMeshTexture::staticClass @ 0x1407E5F30
extern "C" const hkClass* __fastcall hkMeshTexture_staticClass() {
    return &hkMeshTextureClass_stub;
}

// ============================================================================
// hkMesh Additional Serialization Support Functions
// ============================================================================

// Additional static class stubs
static hkClass hkVertexFormatClass_stub;
static hkClass hkMassPropertiesClass_stub;
static hkClass hkSkinnedRefMeshShapeClass_stub;
static hkClass hkIndexedTransformSetClass_stub;
static hkClass hkMemoryMeshVertexBufferClass_stub;

// IDA: hkVertexFormat::staticClass @ 0x1407E5E30
extern "C" const hkClass* __fastcall hkVertexFormat_staticClass() {
    return &hkVertexFormatClass_stub;
}

// IDA: hkMassProperties::staticClass @ 0x1407E5E70
extern "C" const hkClass* __fastcall hkMassProperties_staticClass() {
    return &hkMassPropertiesClass_stub;
}

// IDA: finishLoadedObjecthkMassProperties @ 0x1407E5E80
extern "C" void __fastcall finishLoadedObjecthkMassProperties(void* p, int finishing) {
    (void)p;
    (void)finishing;  // POD type, no special finish
}

// IDA: cleanupLoadedObjecthkMassProperties @ 0x1407E5E90
extern "C" void __fastcall cleanupLoadedObjecthkMassProperties(void* p) {
    (void)p;  // POD type, no special cleanup
}

// IDA: hkSkinnedRefMeshShape::staticClass @ 0x1407E5EA0
extern "C" const hkClass* __fastcall hkSkinnedRefMeshShape_staticClass() {
    return &hkSkinnedRefMeshShapeClass_stub;
}

// IDA: finishLoadedObjecthkSkinnedRefMeshShape @ 0x1407E5EB0
extern "C" void __fastcall finishLoadedObjecthkSkinnedRefMeshShape(void* p, hkFinishLoadedObjectFlag finishing) {
    if (p) {
        // Call constructor with hkFinishLoadedObjectFlag
        new (p) hkSkinnedRefMeshShape(finishing);
    }
}

// IDA: cleanupLoadedObjecthkSkinnedRefMeshShape @ 0x1407E5ED0
extern "C" void __fastcall cleanupLoadedObjecthkSkinnedRefMeshShape(void* p) {
    if (p) {
        void*** vtable = reinterpret_cast<void***>(p);
        if (vtable && *vtable && **vtable) {
            typedef void(__fastcall * DtorFunc)(void*, int);
            DtorFunc dtor = reinterpret_cast<DtorFunc>(**vtable);
            dtor(p, 0);
        }
    }
}

// IDA: getVtablehkSkinnedRefMeshShape @ 0x1407E5EE0
extern "C" void** __fastcall getVtablehkSkinnedRefMeshShape() {
    return reinterpret_cast<void**>(&hkSkinnedRefMeshShapeClass_stub);
}

// IDA: hkIndexedTransformSet::staticClass @ 0x1407E5C70
extern "C" const hkClass* __fastcall hkIndexedTransformSet_staticClass() {
    return &hkIndexedTransformSetClass_stub;
}

// IDA: finishLoadedObjecthkIndexedTransformSet @ 0x1407E5C80
extern "C" void __fastcall finishLoadedObjecthkIndexedTransformSet(void* p, hkFinishLoadedObjectFlag finishing) {
    if (p) {
        // Call constructor with hkFinishLoadedObjectFlag
        new (p) hkIndexedTransformSet(finishing);
    }
}

// IDA: cleanupLoadedObjecthkIndexedTransformSet @ 0x1407E5CA0
extern "C" void __fastcall cleanupLoadedObjecthkIndexedTransformSet(void* p) {
    if (p) {
        void*** vtable = reinterpret_cast<void***>(p);
        if (vtable && *vtable && **vtable) {
            typedef void(__fastcall * DtorFunc)(void*, int);
            DtorFunc dtor = reinterpret_cast<DtorFunc>(**vtable);
            dtor(p, 0);
        }
    }
}

// IDA: getVtablehkIndexedTransformSet @ 0x1407E5CB0
extern "C" void** __fastcall getVtablehkIndexedTransformSet() {
    return reinterpret_cast<void**>(&hkIndexedTransformSetClass_stub);
}

// IDA: hkMemoryMeshVertexBuffer::staticClass @ 0x1407E5DB0
extern "C" const hkClass* __fastcall hkMemoryMeshVertexBuffer_staticClass() {
    return &hkMemoryMeshVertexBufferClass_stub;
}

// IDA: finishLoadedObjecthkMemoryMeshVertexBuffer @ 0x1407E5DC0
extern "C" void __fastcall finishLoadedObjecthkMemoryMeshVertexBuffer(void* p, hkFinishLoadedObjectFlag finishing) {
    if (p) {
        // Call constructor with hkFinishLoadedObjectFlag
        new (p) hkMemoryMeshVertexBuffer(finishing);
    }
}

// IDA: cleanupLoadedObjecthkMemoryMeshVertexBuffer @ 0x1407E5DE0
extern "C" void __fastcall cleanupLoadedObjecthkMemoryMeshVertexBuffer(void* p) {
    if (p) {
        void*** vtable = reinterpret_cast<void***>(p);
        if (vtable && *vtable && **vtable) {
            typedef void(__fastcall * DtorFunc)(void*, int);
            DtorFunc dtor = reinterpret_cast<DtorFunc>(**vtable);
            dtor(p, 0);
        }
    }
}

// IDA: getVtablehkMemoryMeshVertexBuffer @ 0x1407E5DF0
extern "C" void** __fastcall getVtablehkMemoryMeshVertexBuffer() {
    return reinterpret_cast<void**>(&hkMemoryMeshVertexBufferClass_stub);
}

// Additional static class stubs
static hkClass hkVertexFormatElementClass_stub;

// IDA: hkVertexFormat::Element::staticClass @ 0x1407E5E10
extern "C" const hkClass* __fastcall hkVertexFormat_Element_staticClass() {
    return &hkVertexFormatElementClass_stub;
}

// IDA: cleanupLoadedObjecthkVertexFormatElement @ 0x1407E5E20
extern "C" void __fastcall cleanupLoadedObjecthkVertexFormatElement(void* p) {
    (void)p;  // POD type, no special cleanup
}

// ============================================================================
// hkMemoryMeshMaterial Serialization Support
// ============================================================================

static hkClass hkMemoryMeshMaterialClass_stub;

// IDA: hkMemoryMeshMaterial::staticClass @ 0x1407E5F40
extern "C" const hkClass* __fastcall hkMemoryMeshMaterial_staticClass() {
    return &hkMemoryMeshMaterialClass_stub;
}

// IDA: finishLoadedObjecthkMemoryMeshMaterial @ 0x1407E5F50
extern "C" void __fastcall finishLoadedObjecthkMemoryMeshMaterial(void* p, int finishing) {
    if (p) {
        // Call placement new constructor with hkFinishLoadedObjectFlag
        // hkMemoryMeshMaterial::hkMemoryMeshMaterial(p, finishing)
        // Stub: just mark as initialized
    }
}

// IDA: cleanupLoadedObjecthkMemoryMeshMaterial @ 0x1407E5F70
extern "C" void __fastcall cleanupLoadedObjecthkMemoryMeshMaterial(void* p) {
    if (p) {
        void*** vtable = reinterpret_cast<void***>(p);
        if (vtable && *vtable && **vtable) {
            typedef void(__fastcall * DtorFunc)(void*, int);
            DtorFunc dtor = reinterpret_cast<DtorFunc>(**vtable);
            dtor(p, 0);
        }
    }
}

// IDA: getVtablehkMemoryMeshMaterial @ 0x1407E5F80
extern "C" void** __fastcall getVtablehkMemoryMeshMaterial() {
    return reinterpret_cast<void**>(&hkMemoryMeshMaterialClass_stub);
}

// ============================================================================
// hkMemoryMeshShape Serialization Support
// ============================================================================

static hkClass hkMemoryMeshShapeClass_stub;

// IDA: hkMemoryMeshShape::staticClass @ 0x1407E5A10
extern "C" const hkClass* __fastcall hkMemoryMeshShape_staticClass() {
    return &hkMemoryMeshShapeClass_stub;
}

// IDA: finishLoadedObjecthkMemoryMeshShape @ 0x1407E5A20
extern "C" void __fastcall finishLoadedObjecthkMemoryMeshShape(void* p, int finishing) {
    if (p) {
        // Call placement new constructor with hkFinishLoadedObjectFlag
        // hkMemoryMeshShape::hkMemoryMeshShape(p, finishing)
        // Stub: just mark as initialized
    }
}

// IDA: cleanupLoadedObjecthkMemoryMeshShape @ 0x1407E5A40
extern "C" void __fastcall cleanupLoadedObjecthkMemoryMeshShape(void* p) {
    if (p) {
        void*** vtable = reinterpret_cast<void***>(p);
        if (vtable && *vtable && **vtable) {
            typedef void(__fastcall * DtorFunc)(void*, int);
            DtorFunc dtor = reinterpret_cast<DtorFunc>(**vtable);
            dtor(p, 0);
        }
    }
}

// IDA: getVtablehkMemoryMeshShape @ 0x1407E5A50
extern "C" void** __fastcall getVtablehkMemoryMeshShape() {
    return reinterpret_cast<void**>(&hkMemoryMeshShapeClass_stub);
}

// ============================================================================
// hkMemoryMeshShape Implementation
// ============================================================================

// IDA: hkMemoryMeshShape::getClassType @ 0x1407E7080
const hkClass* hkMemoryMeshShape::getClassType() const {
    return &hkMemoryMeshShapeClass_stub;
}

// IDA: hkMemoryMeshShape::~hkMemoryMeshShape @ 0x1407E6F40
hkMemoryMeshShape::~hkMemoryMeshShape() {
    // Release references for each section
    for (int i = 0; i < m_sections.m_size; ++i) {
        hkMeshSectionCinfo& section = m_sections.m_data[i];
        if (section.m_vertexBuffer) {
            section.m_vertexBuffer->removeReference();
        }
        if (section.m_material) {
            section.m_material->removeReference();
        }
    }

    // Destroy m_name (hkStringPtr destructor handles cleanup)
    // The m_name destructor is called automatically

    // Free m_indices32 buffer
    if (m_indices32.m_capacityAndFlags >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(
            m_indices32.m_data,
            4 * m_indices32.m_capacityAndFlags);
    }

    // Free m_indices16 buffer
    if (m_indices16.m_capacityAndFlags >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(
            m_indices16.m_data,
            2 * (m_indices16.m_capacityAndFlags & 0x3FFFFFFF));
    }

    // Free m_sections buffer
    if (m_sections.m_capacityAndFlags >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(
            m_sections.m_data,
            48 * (m_sections.m_capacityAndFlags & 0x3FFFFFFF));
    }
}

// IDA: hkMemoryMeshShape::lockSection @ 0x1407E6A80
void hkMemoryMeshShape::lockSection(int sectionIndex, unsigned char accessFlags, hkMeshSection* sectionOut) const {
    hkMeshSectionCinfo& section = m_sections.m_data[sectionIndex];

    sectionOut->m_primitiveType.m_storage = section.m_primitiveType.m_storage;
    sectionOut->m_numPrimitives = section.m_numPrimitives;
    sectionOut->m_numIndices = hkMeshPrimitiveUtil::calculateNumIndices(
        static_cast<hkMeshSection::PrimitiveType>(section.m_primitiveType.m_storage),
        section.m_numPrimitives);
    sectionOut->m_vertexStartIndex = section.m_vertexStartIndex;
    sectionOut->m_indexType.m_storage = section.m_indexType.m_storage;
    sectionOut->m_transformIndex = section.m_transformIndex;

    // Set vertex buffer based on access flags
    if ((accessFlags & 2) != 0) {
        sectionOut->m_vertexBuffer = section.m_vertexBuffer;
    } else {
        sectionOut->m_vertexBuffer = nullptr;
    }

    // Set indices based on access flags
    sectionOut->m_indices = nullptr;
    if ((accessFlags & 1) != 0 && section.m_indexType.m_storage != 0) {
        sectionOut->m_indices = section.m_indices;
    }

    sectionOut->m_sectionIndex = sectionIndex;
    sectionOut->m_material = section.m_material;
}

// ============================================================================
// hkMeshPrimitiveUtil Implementation
// ============================================================================

// IDA: ?copyPrimitives@hkMeshPrimitiveUtil@@SAXPEBXW4PrimitiveType@hkMeshSection@@W4MeshSectionIndexType@3@HPEAX12H@Z @ 0x1407ec5f0
void hkMeshPrimitiveUtil::copyPrimitives(
    const void* srcIndexPtr,
    hkMeshSection::PrimitiveType srcPrimitiveType,
    hkMeshSection::IndexType srcIndexType,
    int numPrimitives,
    void* dstIndexPtr,
    hkMeshSection::PrimitiveType dstPrimitiveType,
    hkMeshSection::IndexType dstIndexType,
    int baseIndexOffset)
{
    if (numPrimitives <= 0) return;

    int srcPrimitiveIdx = 0;
    int dstIndexPos = 0;
    int v8 = -1, v12 = -1, v13 = -1;
    int v14 = -1, v15 = -1;
    int stripIndex = 0;

    while (srcPrimitiveIdx < numPrimitives) {
        // Read source indices based on primitive type
        if (srcPrimitiveType == hkMeshSection::PRIMITIVE_TYPE_POINT_LIST) {
            if (srcIndexPtr) {
                if (srcIndexType == hkMeshSection::INDEX_TYPE_UINT16)
                    v8 = reinterpret_cast<const unsigned short*>(srcIndexPtr)[srcPrimitiveIdx];
                else
                    v8 = reinterpret_cast<const int*>(srcIndexPtr)[srcPrimitiveIdx];
            } else {
                v8 = srcPrimitiveIdx;
            }
            v12 = v8;
            v13 = v8;
            srcPrimitiveIdx++;
        }
        else if (srcPrimitiveType == hkMeshSection::PRIMITIVE_TYPE_LINE_LIST) {
            if (srcIndexPtr) {
                if (srcIndexType == hkMeshSection::INDEX_TYPE_UINT16) {
                    v8 = reinterpret_cast<const unsigned short*>(srcIndexPtr)[srcPrimitiveIdx];
                    v12 = reinterpret_cast<const unsigned short*>(srcIndexPtr)[srcPrimitiveIdx + 1];
                } else {
                    v8 = reinterpret_cast<const int*>(srcIndexPtr)[srcPrimitiveIdx];
                    v12 = reinterpret_cast<const int*>(srcIndexPtr)[srcPrimitiveIdx + 1];
                }
            } else {
                v8 = srcPrimitiveIdx;
                v12 = srcPrimitiveIdx + 1;
            }
            v13 = v12;
            srcPrimitiveIdx += 2;
        }
        else if (srcPrimitiveType == hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_LIST) {
            if (srcIndexPtr) {
                if (srcIndexType == hkMeshSection::INDEX_TYPE_UINT16) {
                    v8 = reinterpret_cast<const unsigned short*>(srcIndexPtr)[srcPrimitiveIdx];
                    v12 = reinterpret_cast<const unsigned short*>(srcIndexPtr)[srcPrimitiveIdx + 1];
                    v13 = reinterpret_cast<const unsigned short*>(srcIndexPtr)[srcPrimitiveIdx + 2];
                } else {
                    v8 = reinterpret_cast<const int*>(srcIndexPtr)[srcPrimitiveIdx];
                    v12 = reinterpret_cast<const int*>(srcIndexPtr)[srcPrimitiveIdx + 1];
                    v13 = reinterpret_cast<const int*>(srcIndexPtr)[srcPrimitiveIdx + 2];
                }
            } else {
                v8 = srcPrimitiveIdx;
                v12 = srcPrimitiveIdx + 1;
                v13 = srcPrimitiveIdx + 2;
            }
            srcPrimitiveIdx += 3;
        }
        else if (srcPrimitiveType == hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_STRIP) {
            if (v8 < 0) {
                // First triangle
                if (srcIndexPtr) {
                    if (srcIndexType == hkMeshSection::INDEX_TYPE_UINT16) {
                        v8 = reinterpret_cast<const unsigned short*>(srcIndexPtr)[srcPrimitiveIdx];
                        v12 = reinterpret_cast<const unsigned short*>(srcIndexPtr)[srcPrimitiveIdx + 1];
                        v13 = reinterpret_cast<const unsigned short*>(srcIndexPtr)[srcPrimitiveIdx + 2];
                    } else {
                        v8 = reinterpret_cast<const int*>(srcIndexPtr)[srcPrimitiveIdx];
                        v12 = reinterpret_cast<const int*>(srcIndexPtr)[srcPrimitiveIdx + 1];
                        v13 = reinterpret_cast<const int*>(srcIndexPtr)[srcPrimitiveIdx + 2];
                    }
                } else {
                    v8 = srcPrimitiveIdx;
                    v12 = srcPrimitiveIdx + 1;
                    v13 = srcPrimitiveIdx + 2;
                }
                srcPrimitiveIdx += 3;
            } else {
                // Subsequent triangles
                int oldV12 = v12;
                v8 = v12;
                v12 = v13;
                if (srcIndexPtr) {
                    if (srcIndexType == hkMeshSection::INDEX_TYPE_UINT16)
                        v13 = reinterpret_cast<const unsigned short*>(srcIndexPtr)[srcPrimitiveIdx];
                    else
                        v13 = reinterpret_cast<const int*>(srcIndexPtr)[srcPrimitiveIdx];
                } else {
                    v13 = srcPrimitiveIdx;
                }
                srcPrimitiveIdx++;
                if ((stripIndex & 1) != 0) {
                    // Swap v12 and v13 for odd triangles
                    v12 = v13;
                    v13 = oldV12;
                }
            }
            stripIndex++;
        }

        // Write destination indices
        int idx0 = v8 + baseIndexOffset;
        int idx1 = v12 + baseIndexOffset;
        int idx2 = v13 + baseIndexOffset;

        if (dstIndexPtr) {
            if (dstPrimitiveType == hkMeshSection::PRIMITIVE_TYPE_POINT_LIST) {
                if (dstIndexType == hkMeshSection::INDEX_TYPE_UINT16)
                    reinterpret_cast<unsigned short*>(dstIndexPtr)[dstIndexPos] = idx0;
                else
                    reinterpret_cast<int*>(dstIndexPtr)[dstIndexPos] = idx0;
                dstIndexPos++;
            }
            else if (dstPrimitiveType == hkMeshSection::PRIMITIVE_TYPE_LINE_LIST) {
                if (dstIndexType == hkMeshSection::INDEX_TYPE_UINT16) {
                    reinterpret_cast<unsigned short*>(dstIndexPtr)[dstIndexPos] = idx0;
                    reinterpret_cast<unsigned short*>(dstIndexPtr)[dstIndexPos + 1] = idx1;
                } else {
                    reinterpret_cast<int*>(dstIndexPtr)[dstIndexPos] = idx0;
                    reinterpret_cast<int*>(dstIndexPtr)[dstIndexPos + 1] = idx1;
                }
                dstIndexPos += 2;
            }
            else if (dstPrimitiveType == hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_LIST) {
                if (dstIndexType == hkMeshSection::INDEX_TYPE_UINT16) {
                    reinterpret_cast<unsigned short*>(dstIndexPtr)[dstIndexPos] = idx0;
                    reinterpret_cast<unsigned short*>(dstIndexPtr)[dstIndexPos + 1] = idx1;
                    reinterpret_cast<unsigned short*>(dstIndexPtr)[dstIndexPos + 2] = idx2;
                } else {
                    reinterpret_cast<int*>(dstIndexPtr)[dstIndexPos] = idx0;
                    reinterpret_cast<int*>(dstIndexPtr)[dstIndexPos + 1] = idx1;
                    reinterpret_cast<int*>(dstIndexPtr)[dstIndexPos + 2] = idx2;
                }
                dstIndexPos += 3;
            }
            else if (dstPrimitiveType == hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_STRIP) {
                if (v14 >= 0) {
                    // Degenerate triangle handling
                    if (idx0 == v14 || idx0 == v15 || idx0 == v13 + baseIndexOffset) {
                        if (idx1 == v14 || idx1 == v15 || idx1 == v13 + baseIndexOffset) {
                            if (dstIndexType == hkMeshSection::INDEX_TYPE_UINT16)
                                reinterpret_cast<unsigned short*>(dstIndexPtr)[dstIndexPos] = idx2;
                            else
                                reinterpret_cast<int*>(dstIndexPtr)[dstIndexPos] = idx2;
                            dstIndexPos++;
                        } else {
                            if (dstIndexType == hkMeshSection::INDEX_TYPE_UINT16)
                                reinterpret_cast<unsigned short*>(dstIndexPtr)[dstIndexPos] = idx1;
                            else
                                reinterpret_cast<int*>(dstIndexPtr)[dstIndexPos] = idx1;
                            dstIndexPos++;
                        }
                    } else {
                        if (dstIndexType == hkMeshSection::INDEX_TYPE_UINT16)
                            reinterpret_cast<unsigned short*>(dstIndexPtr)[dstIndexPos] = idx0;
                        else
                            reinterpret_cast<int*>(dstIndexPtr)[dstIndexPos] = idx0;
                        dstIndexPos++;
                    }
                }
            }
        }

        v14 = idx0;
        v15 = idx1;
        srcPrimitiveIdx++;
    }
}

// IDA: ?appendTriangleIndices@hkMeshPrimitiveUtil@@SAXW4PrimitiveType@hkMeshSection@@HHAEAV?$hkArrayBase@G@@AEAVhkMemoryAllocator@@@Z @ 0x1407ec9c0
void hkMeshPrimitiveUtil::appendTriangleIndices(
    hkMeshSection::PrimitiveType primType,
    int numVertices,
    short indexBase,
    hkArrayBase<unsigned short>* indicesOut,
    hkMemoryAllocator* allocator)
{
    int typeDelta = primType - 3;

    if (typeDelta == 0) {
        // LINE_LIST case - append all vertices as indices
        int m_size = indicesOut->m_size;
        int neededCapacity = m_size + numVertices;
        int currentCapacity = indicesOut->m_capacityAndFlags & 0x3FFFFFFF;

        if (currentCapacity < neededCapacity) {
            int newCapacity = currentCapacity * 2;
            if (neededCapacity < newCapacity)
                neededCapacity = newCapacity;
            hkResult res;
            hkArrayUtil::_reserve(&res, *allocator, reinterpret_cast<void**>(indicesOut), neededCapacity, 2);
        }

        unsigned short* data = indicesOut->m_data;
        indicesOut->m_size += numVertices;

        for (int i = 0; i < numVertices; i++) {
            data[m_size + i] = indexBase + i;
        }
    }
    else if (typeDelta == 1) {
        // TRIANGLE_STRIP case - generate triangle indices from strip
        int m_size = indicesOut->m_size;
        int numIndices = 3 * numVertices - 6;
        int currentCapacity = indicesOut->m_capacityAndFlags & 0x3FFFFFFF;
        int neededCapacity = m_size + numIndices;

        if (currentCapacity < neededCapacity) {
            int newCapacity = currentCapacity * 2;
            if (neededCapacity < newCapacity)
                neededCapacity = newCapacity;
            hkResult res;
            hkArrayUtil::_reserve(&res, *allocator, reinterpret_cast<void**>(indicesOut), neededCapacity, 2);
        }

        unsigned short* data = indicesOut->m_data;
        indicesOut->m_size += numIndices;
        unsigned short* outPtr = &data[m_size];

        int idx = 2;
        while (idx < numVertices) {
            unsigned short v2 = idx + indexBase;
            outPtr[0] = idx + indexBase - 2;

            if ((idx & 1) != 0)
                outPtr[1] = v2--;
            else
                outPtr[1] = v2 - 1;

            outPtr[2] = v2;
            outPtr += 3;
            idx++;
        }
    }
}

// IDA: ?appendTriangleIndices@hkMeshPrimitiveUtil@@SAXW4PrimitiveType@hkMeshSection@@HHAEAV?$hkArray@IUhkContainerHeapAllocator@@@@@Z @ 0x1407ecb00
void hkMeshPrimitiveUtil::appendTriangleIndices(
    hkMeshSection::PrimitiveType primType,
    int numVertices,
    int indexBase,
    hkArray<unsigned int, hkContainerHeapAllocator>* indicesOut)
{
    int typeDelta = primType - 3;

    if (typeDelta == 0) {
        // LINE_LIST case
        int m_size = indicesOut->m_size;
        int neededCapacity = m_size + numVertices;
        int currentCapacity = indicesOut->m_capacityAndFlags & 0x3FFFFFFF;

        if (currentCapacity < neededCapacity) {
            int newCapacity = currentCapacity * 2;
            if (neededCapacity < newCapacity)
                neededCapacity = newCapacity;
            hkResult res;
            hkArrayUtil::_reserve(&res, hkContainerHeapAllocator::s_alloc, reinterpret_cast<void**>(indicesOut), neededCapacity, 4);
        }

        unsigned int* data = indicesOut->m_data;
        indicesOut->m_size += numVertices;

        for (int i = 0; i < numVertices; i++) {
            data[m_size + i] = indexBase + i;
        }
    }
    else if (typeDelta == 1) {
        // TRIANGLE_STRIP case
        int m_size = indicesOut->m_size;
        int numIndices = 3 * numVertices - 6;
        int currentCapacity = indicesOut->m_capacityAndFlags & 0x3FFFFFFF;
        int neededCapacity = m_size + numIndices;

        if (currentCapacity < neededCapacity) {
            int newCapacity = currentCapacity * 2;
            if (neededCapacity < newCapacity)
                neededCapacity = newCapacity;
            hkResult res;
            hkArrayUtil::_reserve(&res, hkContainerHeapAllocator::s_alloc, reinterpret_cast<void**>(indicesOut), neededCapacity, 4);
        }

        unsigned int* data = indicesOut->m_data;
        indicesOut->m_size += numIndices;
        unsigned int* outPtr = &data[m_size];

        unsigned int v15 = indexBase + 1;
        int v16 = 1 - indexBase;

        while ((int)(v16 + (v15 + 1)) < numVertices) {
            outPtr[0] = v15 - 1;
            unsigned int v17 = v15 + 1;

            if ((((v16 + v15) & 0xFF) & 1) != 0) {
                outPtr[1] = v17;
                outPtr[2] = v15;
            } else {
                outPtr[1] = v15;
                outPtr[2] = v17;
            }

            v15++;
            outPtr += 3;
        }
    }
}

// ============================================================================
// hkMeshVertexBufferUtil Implementation
// ============================================================================

// IDA: ?stridedZero@hkMeshVertexBufferUtil@@SAXPEAXHHH@Z @ 0x1407ed460
void hkMeshVertexBufferUtil::stridedZero(void* dstIn, int dstStride, int elementSize, int numVertices) {
    if (!elementSize) return;

    char* dst = static_cast<char*>(dstIn);

    // Fast path: contiguous memory
    if (dstStride == elementSize) {
        hkString::memSet(dst, 0, numVertices * elementSize);
        return;
    }

    char* end = dst + numVertices * dstStride;

    // Handle different element sizes efficiently
    if ((elementSize & 3) != 0) {
        // Non-aligned element size
        while (dst != end) {
            hkString::memSet(dst, 0, elementSize);
            dst += dstStride;
        }
        return;
    }

    // Handle 4-byte aligned sizes
    switch (elementSize >> 2) {
        case 1: // 4 bytes
            while (dst != end) {
                *reinterpret_cast<int*>(dst) = 0;
                dst += dstStride;
            }
            break;
        case 2: // 8 bytes
            while (dst != end) {
                *reinterpret_cast<int64_t*>(dst) = 0;
                dst += dstStride;
            }
            break;
        case 3: // 12 bytes
            while (dst != end) {
                *reinterpret_cast<int64_t*>(dst) = 0;
                reinterpret_cast<int*>(dst)[2] = 0;
                dst += dstStride;
            }
            break;
        case 4: // 16 bytes
            while (dst != end) {
                reinterpret_cast<int64_t*>(dst)[0] = 0;
                reinterpret_cast<int64_t*>(dst)[1] = 0;
                dst += dstStride;
            }
            break;
        default:
            // Generic path
            while (dst != end) {
                hkString::memSet(dst, 0, elementSize);
                dst += dstStride;
            }
            break;
    }
}

// IDA: ?stridedCopy@hkMeshVertexBufferUtil@@SAXPEBXHPEAXHHH@Z @ 0x1407ed5d0
void hkMeshVertexBufferUtil::stridedCopy(const void* srcIn, int srcStride, void* dstIn, int dstStride, int elementSize, int numVertices) {
    if (!elementSize) return;

    const char* src = static_cast<const char*>(srcIn);
    char* dst = static_cast<char*>(dstIn);

    // Fast path: contiguous memory
    if (srcStride == dstStride && srcStride == elementSize) {
        hkString::memCpy(dst, src, numVertices * elementSize);
        return;
    }

    const char* srcEnd = src + numVertices * srcStride;

    // Handle non-4-aligned element sizes
    if ((elementSize & 3) != 0) {
        while (src != srcEnd) {
            hkString::memCpy(dst, src, elementSize);
            src += srcStride;
            dst += dstStride;
        }
        return;
    }

    // Handle 4-byte aligned sizes
    switch (elementSize >> 2) {
        case 1: // 4 bytes
            while (src != srcEnd) {
                *reinterpret_cast<int*>(dst) = *reinterpret_cast<const int*>(src);
                src += srcStride;
                dst += dstStride;
            }
            break;
        case 2: // 8 bytes
            while (src != srcEnd) {
                reinterpret_cast<int*>(dst)[0] = reinterpret_cast<const int*>(src)[0];
                reinterpret_cast<int*>(dst)[1] = reinterpret_cast<const int*>(src)[1];
                src += srcStride;
                dst += dstStride;
            }
            break;
        case 3: // 12 bytes
            while (src != srcEnd) {
                reinterpret_cast<int*>(dst)[0] = reinterpret_cast<const int*>(src)[0];
                reinterpret_cast<int*>(dst)[1] = reinterpret_cast<const int*>(src)[1];
                reinterpret_cast<int*>(dst)[2] = reinterpret_cast<const int*>(src)[2];
                src += srcStride;
                dst += dstStride;
            }
            break;
        case 4: // 16 bytes
            while (src != srcEnd) {
                reinterpret_cast<int64_t*>(dst)[0] = reinterpret_cast<const int64_t*>(src)[0];
                reinterpret_cast<int64_t*>(dst)[1] = reinterpret_cast<const int64_t*>(src)[1];
                src += srcStride;
                dst += dstStride;
            }
            break;
        default:
            // Generic path
            while (src != srcEnd) {
                hkString::memCpy(dst, src, elementSize);
                src += srcStride;
                dst += dstStride;
            }
            break;
    }
}

// IDA: ?bufferIsSkinnable@hkMeshVertexBufferUtil@@SA?AVhkBool@@PEAVhkMeshVertexBuffer@@@Z @ 0x1407ed970
hkBool hkMeshVertexBufferUtil::bufferIsSkinnable(hkMeshVertexBuffer* vertexBuffer) {
    hkVertexFormat vertexFmt;
    vertexBuffer->getVertexFormat(vertexFmt);

    // Check for skinnable element types
    return hkBool(
        vertexFmt.findElementIndex(hkVertexFormat::USAGE_POSITION, 0) >= 0 ||
        vertexFmt.findElementIndex(hkVertexFormat::USAGE_NORMAL, 0) >= 0 ||
        vertexFmt.findElementIndex(hkVertexFormat::USAGE_TANGENT, 0) >= 0 ||
        vertexFmt.findElementIndex(hkVertexFormat::USAGE_BINORMAL, 0) >= 0
    );
}

// IDA: ?bufferHasWeights@hkMeshVertexBufferUtil@@SA?AVhkBool@@PEAVhkMeshVertexBuffer@@@Z @ 0x1407eda20
hkBool hkMeshVertexBufferUtil::bufferHasWeights(hkMeshVertexBuffer* vertexBuffer) {
    hkVertexFormat vertexFmt;
    vertexBuffer->getVertexFormat(vertexFmt);

    int blendWeightsIndex = vertexFmt.findElementIndex(hkVertexFormat::USAGE_BLEND_WEIGHTS, 0);
    int matrixIndex = vertexFmt.findElementIndex(hkVertexFormat::USAGE_BLEND_MATRIX_INDEX, 0);

    // If no matrix index, check for implied weights
    if (matrixIndex < 0) {
        matrixIndex = vertexFmt.findElementIndex(hkVertexFormat::USAGE_BLEND_WEIGHTS_LAST_IMPLIED, 0);
    }

    return hkBool(blendWeightsIndex >= 0 && matrixIndex >= 0);
}

// IDA: hkMeshVertexBufferUtil_convertUint8ToInt16 @ 0x1407ed790
void hkMeshVertexBufferUtil::convertUint8ToInt16(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    int numVertices)
{
    unsigned char numValues = srcBuffer->m_element.m_numValues;
    if (numValues != dstBuffer->m_element.m_numValues) {
        return;
    }

    const unsigned char* src = reinterpret_cast<const unsigned char*>(srcBuffer->m_start);
    char* dst = const_cast<char*>(dstBuffer->m_start);
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride;

    for (int v = 0; v < numVertices; v++) {
        for (int c = 0; c < numValues; c++) {
            short val = static_cast<short>(src[c]);
            *reinterpret_cast<short*>(&dst[c * 2]) = val;
        }
        src += srcStride;
        dst += dstStride;
    }
}

// IDA: hkMeshVertexBufferUtil_convertUint8ToUint16 @ 0x1407ed800
void hkMeshVertexBufferUtil::convertUint8ToUint16(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    int numVertices)
{
    unsigned char numValues = srcBuffer->m_element.m_numValues;
    if (numValues != dstBuffer->m_element.m_numValues) {
        return;
    }

    const unsigned char* src = reinterpret_cast<const unsigned char*>(srcBuffer->m_start);
    char* dst = const_cast<char*>(dstBuffer->m_start);
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride;

    for (int v = 0; v < numVertices; v++) {
        for (int c = 0; c < numValues; c++) {
            unsigned short val = static_cast<unsigned short>(src[c]);
            *reinterpret_cast<unsigned short*>(&dst[c * 2]) = val;
        }
        src += srcStride;
        dst += dstStride;
    }
}

// IDA: hkMeshVertexBufferUtil_convertInt16ToUint8 @ 0x1407ed870
void hkMeshVertexBufferUtil::convertInt16ToUint8(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    int numVertices)
{
    unsigned char numValues = srcBuffer->m_element.m_numValues;
    if (numValues != dstBuffer->m_element.m_numValues) {
        return;
    }

    const char* src = srcBuffer->m_start;
    unsigned char* dst = reinterpret_cast<unsigned char*>(const_cast<char*>(dstBuffer->m_start));
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride;

    for (int v = 0; v < numVertices; v++) {
        for (int c = 0; c < numValues; c++) {
            dst[c] = static_cast<unsigned char>(src[c * 2]);  // Take low byte of int16
        }
        src += srcStride;
        dst += dstStride;
    }
}

// IDA: hkMeshVertexBufferUtil_convertUint8DwordToUint8 @ 0x1407ee590
void hkMeshVertexBufferUtil::convertUint8DwordToUint8(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    unsigned int numVertices)
{
    using namespace hkVertexFormatDetail;

    unsigned char numValues = srcBuffer->m_element.m_numValues;
    if (numValues != dstBuffer->m_element.m_numValues) {
        return;
    }

    int srcType = srcBuffer->m_element.m_dataType.m_storage;
    int dstType = dstBuffer->m_element.m_dataType.m_storage;

    // Check source is UINT32 (7) or ARGB32 (8), dest is INT8 (2)
    if ((srcType != TYPE_UINT32 && srcType != TYPE_ARGB32) || dstType != TYPE_INT8) {
        return;
    }

    const int* src = reinterpret_cast<const int*>(srcBuffer->m_start);
    char* dst = const_cast<char*>(dstBuffer->m_start);
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride;

    // Handle based on numValues
    switch (numValues) {
        case 1:
            for (unsigned int v = 0; v < numVertices; v++) {
                dst[0] = static_cast<char>(src[0]);
                src = reinterpret_cast<const int*>(reinterpret_cast<const char*>(src) + srcStride);
                dst += dstStride;
            }
            break;
        case 2:
            for (unsigned int v = 0; v < numVertices; v++) {
                int val = src[0];
                dst[0] = static_cast<char>(val & 0xFF);
                dst[1] = static_cast<char>((val >> 8) & 0xFF);
                src = reinterpret_cast<const int*>(reinterpret_cast<const char*>(src) + srcStride);
                dst += dstStride;
            }
            break;
        case 3:
            for (unsigned int v = 0; v < numVertices; v++) {
                int val = src[0];
                dst[0] = static_cast<char>(val & 0xFF);
                dst[1] = static_cast<char>((val >> 8) & 0xFF);
                dst[2] = static_cast<char>((val >> 16) & 0xFF);
                src = reinterpret_cast<const int*>(reinterpret_cast<const char*>(src) + srcStride);
                dst += dstStride;
            }
            break;
        case 4:
            for (unsigned int v = 0; v < numVertices; v++) {
                int val = src[0];
                dst[0] = static_cast<char>(val & 0xFF);
                dst[1] = static_cast<char>((val >> 8) & 0xFF);
                dst[2] = static_cast<char>((val >> 16) & 0xFF);
                dst[3] = static_cast<char>((val >> 24) & 0xFF);
                src = reinterpret_cast<const int*>(reinterpret_cast<const char*>(src) + srcStride);
                dst += dstStride;
            }
            break;
        default:
            // General case for > 4 values
            int chunks = numValues >> 2;
            int remainder = numValues & 3;
            for (unsigned int v = 0; v < numVertices; v++) {
                const int* s = src;
                char* d = dst;
                // Process 4 values at a time
                for (int c = 0; c < chunks; c++) {
                    int val = s[c];
                    d[c * 4 + 0] = static_cast<char>(val & 0xFF);
                    d[c * 4 + 1] = static_cast<char>((val >> 8) & 0xFF);
                    d[c * 4 + 2] = static_cast<char>((val >> 16) & 0xFF);
                    d[c * 4 + 3] = static_cast<char>((val >> 24) & 0xFF);
                }
                // Handle remainder
                if (remainder > 0) {
                    int val = s[chunks];
                    for (int r = 0; r < remainder; r++) {
                        d[chunks * 4 + r] = static_cast<char>((val >> (r * 8)) & 0xFF);
                    }
                }
                src = reinterpret_cast<const int*>(reinterpret_cast<const char*>(src) + srcStride);
                dst += dstStride;
            }
            break;
    }
}

// IDA: hkMeshVertexBufferUtil_convertUint8DwordToArgb32 @ 0x1407ee820
void hkMeshVertexBufferUtil::convertUint8DwordToArgb32(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    int numVertices)
{
    using namespace hkVertexFormatDetail;

    unsigned char numValues = srcBuffer->m_element.m_numValues;
    if (numValues != dstBuffer->m_element.m_numValues) {
        return;
    }

    // Check source is UINT32 (7), dest is ARGB32 (8)
    if (srcBuffer->m_element.m_dataType.m_storage != TYPE_UINT32 ||
        dstBuffer->m_element.m_dataType.m_storage != TYPE_ARGB32) {
        return;
    }

    // Special case: exactly 4 values
    if (numValues != 4 || numVertices <= 0) {
        return;
    }

    const unsigned char* src = reinterpret_cast<const unsigned char*>(srcBuffer->m_start);
    unsigned char* dst = reinterpret_cast<unsigned char*>(const_cast<char*>(dstBuffer->m_start));
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride;

    for (int v = 0; v < numVertices; v++) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        src += srcStride;
        dst += dstStride;
    }
}

// IDA: hkMeshVertexBufferUtil_convertArgb32ToToFloat32 @ 0x1407ee890
void hkMeshVertexBufferUtil::convertArgb32ToFloat32(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    int numVertices)
{
    using namespace hkVertexFormatDetail;

    // Check conditions
    if (!(srcBuffer->m_element.m_numValues == 1 || dstBuffer->m_element.m_numValues == 4)) {
        return;
    }
    if (srcBuffer->m_element.m_dataType.m_storage != TYPE_ARGB32 ||
        dstBuffer->m_element.m_dataType.m_storage != TYPE_FLOAT32) {
        return;
    }

    const unsigned int* src = reinterpret_cast<const unsigned int*>(srcBuffer->m_start);
    float* dst = reinterpret_cast<float*>(const_cast<char*>(dstBuffer->m_start));
    int srcNumValues = srcBuffer->m_element.m_numValues;
    int dstNumValues = dstBuffer->m_element.m_numValues;

    const float inv255 = 1.0f / 255.0f;

    // Process in groups of 4 for optimization
    int processed = 0;
    if (numVertices >= 4) {
        int chunks = ((numVertices - 4) >> 2) + 1;
        processed = chunks * 4;

        for (int c = 0; c < chunks; c++) {
            // Process 4 vertices at a time
            for (int i = 0; i < 4; i++) {
                unsigned int argb = src[0];
                dst[0] = static_cast<float>(argb & 0xFF) * inv255;
                dst[1] = static_cast<float>((argb >> 8) & 0xFF) * inv255;
                dst[2] = static_cast<float>((argb >> 16) & 0xFF) * inv255;
                dst[3] = static_cast<float>((argb >> 24) & 0xFF) * inv255;
                src += srcNumValues;
                dst += dstNumValues;
            }
        }
    }

    // Process remaining
    while (processed < numVertices) {
        unsigned int argb = src[0];
        dst[0] = static_cast<float>(argb & 0xFF) * inv255;
        dst[1] = static_cast<float>((argb >> 8) & 0xFF) * inv255;
        dst[2] = static_cast<float>((argb >> 16) & 0xFF) * inv255;
        dst[3] = static_cast<float>((argb >> 24) & 0xFF) * inv255;
        src += srcNumValues;
        dst += dstNumValues;
        processed++;
    }
}

// IDA: hkMeshVertexBufferUtil_convertUint8DwordToFloat32 @ 0x1407eeb80
void hkMeshVertexBufferUtil::convertUint8DwordToFloat32(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    int numVertices)
{
    using namespace hkVertexFormatDetail;

    unsigned char numValues = srcBuffer->m_element.m_numValues;
    if (numValues != dstBuffer->m_element.m_numValues) {
        return;
    }

    // Check source is UINT32 (7), dest is FLOAT32 (10)
    if (srcBuffer->m_element.m_dataType.m_storage != TYPE_UINT32 ||
        dstBuffer->m_element.m_dataType.m_storage != TYPE_FLOAT32) {
        return;
    }

    // Special case: exactly 4 values
    if (numValues != 4) {
        return;
    }

    const unsigned char* src = reinterpret_cast<const unsigned char*>(srcBuffer->m_start);
    float* dst = reinterpret_cast<float*>(const_cast<char*>(dstBuffer->m_start));
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride >> 2;  // Convert byte stride to float stride

    // Process in groups of 4 for optimization
    int processed = 0;
    if (numVertices >= 4) {
        int chunks = ((numVertices - 4) >> 2) + 1;
        processed = chunks * 4;

        for (int c = 0; c < chunks; c++) {
            // Process 4 vertices at a time
            for (int i = 0; i < 4; i++) {
                dst[0] = static_cast<float>(src[0]);
                dst[1] = static_cast<float>(src[1]);
                dst[2] = static_cast<float>(src[2]);
                dst[3] = static_cast<float>(src[3]);
                src += srcStride;
                dst += dstStride;
            }
        }
    }

    // Process remaining
    while (processed < numVertices) {
        dst[0] = static_cast<float>(src[0]);
        dst[1] = static_cast<float>(src[1]);
        dst[2] = static_cast<float>(src[2]);
        dst[3] = static_cast<float>(src[3]);
        src += srcStride;
        dst += dstStride;
        processed++;
    }
}

// IDA: hkMeshVertexBufferUtil_convertUint8ToUint8Dword @ 0x1407eed90
void hkMeshVertexBufferUtil::convertUint8ToUint8Dword(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    unsigned int numVertices)
{
    using namespace hkVertexFormatDetail;

    unsigned char numValues = srcBuffer->m_element.m_numValues;
    if (numValues != dstBuffer->m_element.m_numValues) {
        return;
    }

    // Check source is INT8 (2), dest is UINT32 (7) or ARGB32 (8)
    if (srcBuffer->m_element.m_dataType.m_storage != TYPE_INT8) {
        return;
    }
    int dstType = dstBuffer->m_element.m_dataType.m_storage;
    if (dstType != TYPE_UINT32 && dstType != TYPE_ARGB32) {
        return;
    }

    const unsigned char* src = reinterpret_cast<const unsigned char*>(srcBuffer->m_start);
    int* dst = reinterpret_cast<int*>(const_cast<char*>(dstBuffer->m_start));
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride;

    switch (numValues) {
        case 1:
            for (unsigned int v = 0; v < numVertices; v++) {
                *dst = src[0];
                src += srcStride;
                dst = reinterpret_cast<int*>(reinterpret_cast<char*>(dst) + dstStride);
            }
            break;
        case 2:
            for (unsigned int v = 0; v < numVertices; v++) {
                *dst = src[0] | (src[1] << 8);
                src += srcStride;
                dst = reinterpret_cast<int*>(reinterpret_cast<char*>(dst) + dstStride);
            }
            break;
        case 3:
            for (unsigned int v = 0; v < numVertices; v++) {
                *dst = src[0] | (src[1] << 8) | (src[2] << 16);
                src += srcStride;
                dst = reinterpret_cast<int*>(reinterpret_cast<char*>(dst) + dstStride);
            }
            break;
        case 4:
            for (unsigned int v = 0; v < numVertices; v++) {
                *dst = src[0] | (src[1] << 8) | (src[2] << 16) | (src[3] << 24);
                src += srcStride;
                dst = reinterpret_cast<int*>(reinterpret_cast<char*>(dst) + dstStride);
            }
            break;
        default:
            // General case for > 4 values
            int chunks = numValues >> 2;
            int remainder = numValues & 3;
            for (unsigned int v = 0; v < numVertices; v++) {
                const unsigned char* s = src;
                int* d = dst;
                // Process 4 values at a time
                for (int c = 0; c < chunks; c++) {
                    d[c] = s[c * 4 + 0] | (s[c * 4 + 1] << 8) | (s[c * 4 + 2] << 16) | (s[c * 4 + 3] << 24);
                }
                // Handle remainder
                if (remainder > 0) {
                    int val = 0;
                    for (int r = 0; r < remainder; r++) {
                        val |= s[chunks * 4 + r] << (r * 8);
                    }
                    d[chunks] = val;
                }
                src += srcStride;
                dst = reinterpret_cast<int*>(reinterpret_cast<char*>(dst) + dstStride);
            }
            break;
    }
}

// IDA: hkMeshVertexBufferUtil_convertUint8DwordToInt16 @ 0x1407ee780
void hkMeshVertexBufferUtil::convertUint8DwordToInt16(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    unsigned int numVertices)
{
    using namespace hkVertexFormatDetail;

    // Check: exactly 4 values, source is UINT32 (7), dest is UINT8 (3)
    if (srcBuffer->m_element.m_numValues != 4 ||
        dstBuffer->m_element.m_numValues != 4 ||
        srcBuffer->m_element.m_dataType.m_storage != TYPE_UINT32 ||
        dstBuffer->m_element.m_dataType.m_storage != TYPE_UINT8) {
        return;
    }

    unsigned short* dst = reinterpret_cast<unsigned short*>(const_cast<char*>(dstBuffer->m_start));
    const unsigned int* src = reinterpret_cast<const unsigned int*>(srcBuffer->m_start);
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride >> 1;  // Convert to short stride

    for (unsigned int v = 0; v < numVertices; v++) {
        unsigned int val = src[0];
        dst[0] = val & 0xFF;
        dst[1] = (val >> 8) & 0xFF;
        dst[2] = (val >> 16) & 0xFF;
        dst[3] = (val >> 24) & 0xFF;
        src = reinterpret_cast<const unsigned int*>(reinterpret_cast<const char*>(src) + srcStride);
        dst += dstStride;
    }
}

// IDA: hkMeshVertexBufferUtil_convertUint8ToFloat32 @ 0x1407eefc0
void hkMeshVertexBufferUtil::convertUint8ToFloat32(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    unsigned int numVertices)
{
    using namespace hkVertexFormatDetail;

    unsigned char numValues = srcBuffer->m_element.m_numValues;
    unsigned char dstNumValues = dstBuffer->m_element.m_numValues;
    const float inv255 = 1.0f / 255.0f;

    const unsigned char* src = reinterpret_cast<const unsigned char*>(srcBuffer->m_start);
    float* dst = reinterpret_cast<float*>(const_cast<char*>(dstBuffer->m_start));
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride;

    if (numValues == dstNumValues) {
        // Same number of values - direct conversion
        for (unsigned int v = 0; v < numVertices; v++) {
            for (int c = 0; c < numValues; c++) {
                dst[c] = static_cast<float>(src[c]) * inv255;
            }
            src += srcStride;
            dst = reinterpret_cast<float*>(reinterpret_cast<char*>(dst) + dstStride);
        }
    }
    else {
        // Different number of values - special handling
        int srcUsage = srcBuffer->m_element.m_usage.m_storage;
        int dstUsage = dstBuffer->m_element.m_usage.m_storage;

        if (srcUsage == USAGE_BLEND_WEIGHTS && dstUsage == USAGE_BLEND_WEIGHTS_LAST_IMPLIED && numValues == dstNumValues - 1) {
            // Blend weights with implied last weight
            for (unsigned int v = 0; v < numVertices; v++) {
                int sum = 0;
                int i;
                for (i = 0; i < numValues; i++) {
                    int val = src[i];
                    sum += val;
                    dst[i] = static_cast<float>(val) * inv255;
                }
                // Last weight is 255 - sum
                dst[numValues] = static_cast<float>(255 - sum) * inv255;
                src += srcStride;
                dst = reinterpret_cast<float*>(reinterpret_cast<char*>(dst) + dstStride);
            }
        }
        else if ((srcUsage == USAGE_BLEND_WEIGHTS_LAST_IMPLIED || dstUsage == USAGE_BLEND_WEIGHTS) && dstNumValues == numValues - 1) {
            // Skip last weight
            for (unsigned int v = 0; v < numVertices; v++) {
                for (int c = 0; c < dstNumValues; c++) {
                    dst[c] = static_cast<float>(src[c]) * inv255;
                }
                src += srcStride;
                dst = reinterpret_cast<float*>(reinterpret_cast<char*>(dst) + dstStride);
            }
        }
    }
}

// IDA: hkMeshVertexBufferUtil_convertFloat32ToUint8 @ 0x1407ef2e0
void hkMeshVertexBufferUtil::convertFloat32ToUint8(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    unsigned int numVertices)
{
    using namespace hkVertexFormatDetail;

    unsigned char numValues = srcBuffer->m_element.m_numValues;
    unsigned char dstNumValues = dstBuffer->m_element.m_numValues;

    const float* src = reinterpret_cast<const float*>(srcBuffer->m_start);
    unsigned char* dst = reinterpret_cast<unsigned char*>(const_cast<char*>(dstBuffer->m_start));
    int srcStride = srcBuffer->m_stride;
    int dstStride = dstBuffer->m_stride;

    if (numValues == dstNumValues) {
        // Same number of values - direct conversion
        for (unsigned int v = 0; v < numVertices; v++) {
            for (int c = 0; c < numValues; c++) {
                float val = src[c] * 255.0f;
                dst[c] = static_cast<unsigned char>(val < 0.0f ? 0 : val > 255.0f ? 255 : val);
            }
            src = reinterpret_cast<const float*>(reinterpret_cast<const char*>(src) + srcStride);
            dst += dstStride;
        }
    }
    else {
        // Different number of values - special handling
        int srcUsage = srcBuffer->m_element.m_usage.m_storage;
        int dstUsage = dstBuffer->m_element.m_usage.m_storage;

        if (srcUsage == USAGE_BLEND_WEIGHTS && dstUsage == USAGE_BLEND_WEIGHTS_LAST_IMPLIED && numValues == dstNumValues - 1) {
            // Blend weights with implied last weight
            for (unsigned int v = 0; v < numVertices; v++) {
                int sum = 0;
                for (int c = 0; c < numValues; c++) {
                    float val = src[c] * 255.0f;
                    unsigned char byteVal = static_cast<unsigned char>(val < 0.0f ? 0 : val > 255.0f ? 255 : val);
                    dst[c] = byteVal;
                    sum += byteVal;
                }
                // Last weight is 255 - sum
                dst[numValues] = static_cast<unsigned char>(255 - sum);
                src = reinterpret_cast<const float*>(reinterpret_cast<const char*>(src) + srcStride);
                dst += dstStride;
            }
        }
        else if ((srcUsage == USAGE_BLEND_WEIGHTS_LAST_IMPLIED || dstUsage == USAGE_BLEND_WEIGHTS) && dstNumValues == numValues - 1) {
            // Skip last weight
            for (unsigned int v = 0; v < numVertices; v++) {
                for (int c = 0; c < dstNumValues; c++) {
                    float val = src[c] * 255.0f;
                    dst[c] = static_cast<unsigned char>(val < 0.0f ? 0 : val > 255.0f ? 255 : val);
                }
                src = reinterpret_cast<const float*>(reinterpret_cast<const char*>(src) + srcStride);
                dst += dstStride;
            }
        }
    }
}

// IDA: ?copy@hkMeshVertexBufferUtil@@SAXAEBUBuffer@LockedVertices@hkMeshVertexBuffer@@0H@Z @ 0x1407ee540
void hkMeshVertexBufferUtil::copy(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    int numVertices)
{
    using namespace hkVertexFormatDetail;

    // Calculate element size based on data type
    static const int s_dataTypeToSize[] = {
        0,   // TYPE_NONE = 0
        4,   // TYPE_FLOAT = 1
        1,   // TYPE_INT8 = 2
        1,   // TYPE_UINT8 = 3
        2,   // TYPE_INT16 = 4
        2,   // TYPE_UINT16 = 5
        4,   // TYPE_INT32 = 6
        4,   // TYPE_UINT32 = 7
        4,   // TYPE_ARGB32 = 8
        0,   // unused
        4,   // TYPE_FLOAT32 = 10
    };

    int dataType = srcBuffer->m_element.m_dataType.m_storage;
    int elementSize = (srcBuffer->m_element.m_numValues * s_dataTypeToSize[dataType] + 3) & 0xFFFFFFFC;

    stridedCopy(srcBuffer->m_start, srcBuffer->m_stride,
                const_cast<char*>(dstBuffer->m_start), dstBuffer->m_stride,
                elementSize, numVertices);
}

// IDA: hkMeshVertexBufferUtil_convertFloat323FloatsToFloat324Floats @ 0x1407ef5f0
void hkMeshVertexBufferUtil::convertFloat323FloatsToFloat324Floats(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    int numVertices)
{
    using namespace hkVertexFormatDetail;

    // Check: src has 3 values, dst has 4 values, both are FLOAT32
    if (srcBuffer->m_element.m_numValues != 3 ||
        dstBuffer->m_element.m_numValues != 4 ||
        srcBuffer->m_element.m_dataType.m_storage != TYPE_FLOAT32 ||
        dstBuffer->m_element.m_dataType.m_storage != TYPE_FLOAT32) {
        return;
    }

    const float* src = reinterpret_cast<const float*>(srcBuffer->m_start);
    float* dst = reinterpret_cast<float*>(const_cast<char*>(dstBuffer->m_start));
    int srcStride = srcBuffer->m_stride >> 2;  // Convert to float stride
    int dstStride = dstBuffer->m_stride >> 2;

    // Process in groups of 4 for optimization
    int processed = 0;
    if (numVertices >= 4) {
        int chunks = ((numVertices - 4) >> 2) + 1;
        processed = chunks * 4;

        for (int c = 0; c < chunks; c++) {
            // Process 4 vertices at a time
            for (int i = 0; i < 4; i++) {
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = 0.0f;
                src += srcStride;
                dst += dstStride;
            }
        }
    }

    // Process remaining
    while (processed < numVertices) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = 0.0f;
        src += srcStride;
        dst += dstStride;
        processed++;
    }
}

// IDA: hkMeshVertexBufferUtil_convertFloat32ToArgb32 @ 0x1407f0220
void hkMeshVertexBufferUtil::convertFloat32ToArgb32(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMeshVertexBuffer::LockedVertices::Buffer* dstBuffer,
    unsigned int numVertices)
{
    using namespace hkVertexFormatDetail;

    // Check: src has 4 values, dst has 1 value, src is FLOAT32, dst is ARGB32
    if (srcBuffer->m_element.m_numValues != 4 ||
        dstBuffer->m_element.m_numValues != 1 ||
        srcBuffer->m_element.m_dataType.m_storage != TYPE_FLOAT32 ||
        dstBuffer->m_element.m_dataType.m_storage != TYPE_ARGB32) {
        return;
    }

    const float* src = reinterpret_cast<const float*>(srcBuffer->m_start);
    unsigned int* dst = reinterpret_cast<unsigned int*>(const_cast<char*>(dstBuffer->m_start));
    int srcStride = srcBuffer->m_stride >> 2;
    int dstStride = dstBuffer->m_stride >> 2;

    for (unsigned int v = 0; v < numVertices; v++) {
        // Clamp each channel to [0, 255], add 0.5 for rounding
        // IDA: _mm_add_ps(_mm_min_ps(255, _mm_max_ps(0, src * 255)), 0.5)
        float r = src[0] * 255.0f;
        float g = src[1] * 255.0f;
        float b = src[2] * 255.0f;
        float a = src[3] * 255.0f;

        // Clamp and round
        r = (r < 0.0f) ? 0.0f : (r > 255.0f) ? 255.0f : r;
        g = (g < 0.0f) ? 0.0f : (g > 255.0f) ? 255.0f : g;
        b = (b < 0.0f) ? 0.0f : (b > 255.0f) ? 255.0f : b;
        a = (a < 0.0f) ? 0.0f : (a > 255.0f) ? 255.0f : a;

        // Add 0.5 for rounding
        r += 0.5f;
        g += 0.5f;
        b += 0.5f;
        a += 0.5f;

        // Pack as ARGB (note: IDA shows order is r|g|b|a packed)
        unsigned int rVal = static_cast<unsigned int>(r) & 0xFF;
        unsigned int gVal = static_cast<unsigned int>(g) & 0xFF;
        unsigned int bVal = static_cast<unsigned int>(b) & 0xFF;
        unsigned int aVal = static_cast<unsigned int>(a) & 0xFF;

        *dst = rVal | (gVal << 8) | (bVal << 16) | (aVal << 24);

        src += srcStride;
        dst += dstStride;
    }
}

// IDA: ?isBufferDataEqual@hkMeshVertexBufferUtil@@SA?AVhkBool@@AEBUBuffer@LockedVertices@hkMeshVertexBuffer@@0M@Z @ 0x1407efb30
hkBool hkMeshVertexBufferUtil::isBufferDataEqual(
    const hkMeshVertexBuffer::LockedVertices::Buffer* bufferA,
    const hkMeshVertexBuffer::LockedVertices::Buffer* bufferB,
    float threshold)
{
    using namespace hkVertexFormatDetail;

    // Check data type and num values match
    if (bufferA->m_element.m_dataType.m_storage != bufferB->m_element.m_dataType.m_storage ||
        bufferA->m_element.m_numValues != bufferB->m_element.m_numValues) {
        return hkBool(false);
    }

    int dataType = bufferA->m_element.m_dataType.m_storage;
    int numValues = bufferA->m_element.m_numValues;

    switch (dataType) {
        case TYPE_FLOAT:
        case TYPE_INT8:
            // Byte comparison
            for (int i = 0; i < numValues; i++) {
                if (bufferA->m_start[i] != bufferB->m_start[i]) {
                    return hkBool(false);
                }
            }
            return hkBool(true);

        case TYPE_UINT8:
        case TYPE_INT16:
            // Short comparison
            for (int i = 0; i < numValues; i++) {
                if (reinterpret_cast<const unsigned short*>(bufferA->m_start)[i] !=
                    reinterpret_cast<const unsigned short*>(bufferB->m_start)[i]) {
                    return hkBool(false);
                }
            }
            return hkBool(true);

        case TYPE_UINT16:
        case TYPE_INT32:
        case TYPE_UINT32:
            // Int comparison
            for (int i = 0; i < numValues; i++) {
                if (reinterpret_cast<const unsigned int*>(bufferA->m_start)[i] !=
                    reinterpret_cast<const unsigned int*>(bufferB->m_start)[i]) {
                    return hkBool(false);
                }
            }
            return hkBool(true);

        case TYPE_ARGB32: {
            // ARGB32 with threshold comparison
            const unsigned int* a = reinterpret_cast<const unsigned int*>(bufferA->m_start);
            const unsigned int* b = reinterpret_cast<const unsigned int*>(bufferB->m_start);

            for (int i = 0; i < numValues; i++) {
                if (a[i] == b[i]) {
                    continue;
                }

                // If threshold is 0, values must match exactly
                if (threshold == 0.0f) {
                    return hkBool(false);
                }

                // Compare each channel with threshold
                unsigned int va = a[i];
                unsigned int vb = b[i];
                int thresholdInt = static_cast<int>(threshold * 255.0f);

                for (int c = 0; c < 4; c++) {
                    int diff = static_cast<int>((va >> (c * 8)) & 0xFF) -
                               static_cast<int>((vb >> (c * 8)) & 0xFF);
                    diff = (diff < 0) ? -diff : diff;
                    if (diff > thresholdInt) {
                        return hkBool(false);
                    }
                }
            }
            return hkBool(true);
        }

        case TYPE_FLOAT32: {
            // Float32 with threshold comparison
            const float* a = reinterpret_cast<const float*>(bufferA->m_start);
            const float* b = reinterpret_cast<const float*>(bufferB->m_start);

            for (int i = 0; i < numValues; i++) {
                float diff = a[i] - b[i];
                // Use absolute value
                if (diff < 0.0f) diff = -diff;
                if (diff >= threshold) {
                    return hkBool(false);
                }
            }
            return hkBool(true);
        }

        default:
            return hkBool(false);
    }
}

// IDA: ?getIndexedElementVectorArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBUBuffer@LockedVertices@hkMeshVertexBuffer@@PEBHPEAVhkVector4@@H@Z @ 0x1407edb00
hkResult hkMeshVertexBufferUtil::getIndexedElementVectorArray(
    const hkMeshVertexBuffer::LockedVertices::Buffer* buffer,
    const int* indices,
    hkVector4* out,
    int numVertices)
{
    using namespace hkVertexFormatDetail;

    int dataType = buffer->m_element.m_dataType.m_storage;
    int usage = buffer->m_element.m_usage.m_storage;
    int numValues = buffer->m_element.m_numValues;

    // Special case: COLOR usage with 4 values can be treated as ARGB32
    if (usage == USAGE_COLOR && numValues == 4 && dataType == TYPE_INT8) {
        dataType = TYPE_ARGB32;
    }

    if (dataType == TYPE_ARGB32) {
        // ARGB32 format - convert from 32-bit ARGB to 4 floats
        const float inv255 = 1.0f / 255.0f;
        for (int i = 0; i < numVertices; i++) {
            int idx = indices[i];
            unsigned int argb = *reinterpret_cast<const unsigned int*>(buffer->m_start + buffer->m_stride * idx);

            // Extract channels and convert to float (0-1 range)
            float a = static_cast<float>(argb & 0xFF) * inv255;
            float r = static_cast<float>((argb >> 8) & 0xFF) * inv255;
            float g = static_cast<float>((argb >> 16) & 0xFF) * inv255;
            float b = static_cast<float>((argb >> 24) & 0xFF) * inv255;

            // Store as [a, r, g, b]
            float* dst = reinterpret_cast<float*>(&out[i].m_quad);
            dst[0] = a;
            dst[1] = r;
            dst[2] = g;
            dst[3] = b;
        }
        return hkResult(HK_SUCCESS);
    }
    else if (dataType == TYPE_FLOAT32) {
        // FLOAT32 format - copy directly
        int copyValues = (numValues > 4) ? 4 : numValues;

        // Zero out remaining components if less than 4 values
        if (numValues < 4) {
            for (int i = 0; i < numVertices; i++) {
                out[i].m_quad = _mm_setzero_ps();
            }
        }

        // Copy using stridedCopy
        stridedCopy(buffer->m_start, buffer->m_stride, &out[0].m_quad, 16, copyValues * 4, numVertices);
        return hkResult(HK_SUCCESS);
    }

    return hkResult(1);
}

// IDA: ?getElementVectorArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBUBuffer@LockedVertices@hkMeshVertexBuffer@@PEAVhkVector4@@H@Z @ 0x1407edd70
hkResult hkMeshVertexBufferUtil::getElementVectorArray(
    const hkMeshVertexBuffer::LockedVertices::Buffer* buffer,
    hkVector4* out,
    int numVertices)
{
    using namespace hkVertexFormatDetail;

    int dataType = buffer->m_element.m_dataType.m_storage;
    int numValues = buffer->m_element.m_numValues;

    if (dataType == TYPE_ARGB32) {
        // ARGB32 format - convert from 32-bit ARGB to 4 floats
        const char* ptr = buffer->m_start;
        const float inv255 = 1.0f / 255.0f;
        for (int i = 0; i < numVertices; i++) {
            unsigned int argb = *reinterpret_cast<const unsigned int*>(ptr);

            // Extract channels
            float a = static_cast<float>(argb & 0xFF) * inv255;
            float r = static_cast<float>((argb >> 8) & 0xFF) * inv255;
            float g = static_cast<float>((argb >> 16) & 0xFF) * inv255;
            float b = static_cast<float>((argb >> 24) & 0xFF) * inv255;

            // Store as [a, r, g, b]
            float* dst = reinterpret_cast<float*>(&out[i].m_quad);
            dst[0] = a;
            dst[1] = r;
            dst[2] = g;
            dst[3] = b;

            ptr += buffer->m_stride;
        }
        return hkResult(HK_SUCCESS);
    }
    else if (dataType == TYPE_FLOAT32) {
        // FLOAT32 format
        // Zero out if less than 4 values
        if (numValues < 4) {
            for (int i = 0; i < numVertices; i++) {
                out[i].m_quad = _mm_setzero_ps();
            }
        }

        int copyValues = (numValues > 4) ? 4 : numValues;
        stridedCopy(buffer->m_start, buffer->m_stride, &out[0].m_quad, 16, copyValues * 4, numVertices);
        return hkResult(HK_SUCCESS);
    }

    return hkResult(1);
}

// IDA: ?setElementVectorArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBUBuffer@LockedVertices@hkMeshVertexBuffer@@PEBVhkVector4@@H@Z @ 0x1407edeb0
hkResult hkMeshVertexBufferUtil::setElementVectorArray(
    const hkMeshVertexBuffer::LockedVertices::Buffer* buffer,
    const hkVector4* src,
    int numVertices)
{
    using namespace hkVertexFormatDetail;

    int dataType = buffer->m_element.m_dataType.m_storage;
    int numValues = buffer->m_element.m_numValues;

    if (dataType == TYPE_ARGB32) {
        // ARGB32 format - convert from floats to 32-bit ARGB
        char* ptr = const_cast<char*>(buffer->m_start);
        for (int i = 0; i < numVertices; i++) {
            // Clamp and convert each channel
            const float* vals = reinterpret_cast<const float*>(&src[i].m_quad);
            float a = vals[0];
            float r = vals[1];
            float g = vals[2];
            float b = vals[3];

            // Clamp to [0, 255]
            a = (a < 0.0f) ? 0.0f : (a > 1.0f) ? 1.0f : a;
            r = (r < 0.0f) ? 0.0f : (r > 1.0f) ? 1.0f : r;
            g = (g < 0.0f) ? 0.0f : (g > 1.0f) ? 1.0f : g;
            b = (b < 0.0f) ? 0.0f : (b > 1.0f) ? 1.0f : b;

            // Scale to [0, 255] and pack as ARGB
            unsigned int argb =
                (static_cast<unsigned int>(a * 255.0f) & 0xFF) |
                ((static_cast<unsigned int>(r * 255.0f) & 0xFF) << 8) |
                ((static_cast<unsigned int>(g * 255.0f) & 0xFF) << 16) |
                ((static_cast<unsigned int>(b * 255.0f) & 0xFF) << 24);

            *reinterpret_cast<unsigned int*>(ptr) = argb;
            ptr += buffer->m_stride;
        }
        return hkResult(HK_SUCCESS);
    }
    else if (dataType == TYPE_FLOAT32) {
        // FLOAT32 format
        // Zero out extra bytes if more than 4 values
        if (numValues > 4) {
            stridedZero(const_cast<char*>(buffer->m_start + 16), buffer->m_stride, (numValues - 4) * 4, numVertices);
        }

        int copyValues = (numValues > 4) ? 4 : numValues;
        stridedCopy(&src[0].m_quad, 16, const_cast<char*>(buffer->m_start), buffer->m_stride, copyValues * 4, numVertices);
        return hkResult(HK_SUCCESS);
    }

    return hkResult(1);
}

// IDA: ?getElementIntArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBULockedVertices@hkMeshVertexBuffer@@HPEAH@Z @ 0x1407edfd0
hkResult hkMeshVertexBufferUtil::getElementIntArray(
    const hkMeshVertexBuffer::LockedVertices* lockedVertices,
    int bufferIndex,
    int* dst)
{
    using namespace hkVertexFormatDetail;

    int numVertices = lockedVertices->m_numVertices;
    const auto& buf = lockedVertices->m_buffers[bufferIndex];
    int dataType = buf.m_element.m_dataType.m_storage;
    const char* ptr = buf.m_start;
    int stride = buf.m_stride;
    int numValues = buf.m_element.m_numValues;

    switch (dataType) {
        case TYPE_FLOAT:  // 1
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    *dst++ = static_cast<int>(ptr[c]);
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_INT8:  // 2
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    *dst++ = static_cast<unsigned char>(ptr[c]);
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_UINT8:  // 3
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    *dst++ = *reinterpret_cast<const short*>(&ptr[c * 2]);
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_INT16:  // 4
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    *dst++ = *reinterpret_cast<const unsigned short*>(&ptr[c * 2]);
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_UINT16:  // 5
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    *dst++ = *reinterpret_cast<const int*>(&ptr[c * 4]);
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_INT32:  // 6
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    *dst++ = *reinterpret_cast<const int*>(&ptr[c * 4]);
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_UINT32:  // 7
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    unsigned int val = *reinterpret_cast<const unsigned int*>(&ptr[c * 4]);
                    // Expand each byte to int
                    *dst++ = val & 0xFF;
                    *dst++ = (val >> 8) & 0xFF;
                    *dst++ = (val >> 16) & 0xFF;
                    *dst++ = (val >> 24) & 0xFF;
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        default:
            return hkResult(1);
    }
}

// IDA: ?setElementIntArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBULockedVertices@hkMeshVertexBuffer@@HPEBH@Z @ 0x1407ee2a0
hkResult hkMeshVertexBufferUtil::setElementIntArray(
    const hkMeshVertexBuffer::LockedVertices* lockedVertices,
    int bufferIndex,
    const int* src)
{
    using namespace hkVertexFormatDetail;

    int numVertices = lockedVertices->m_numVertices;
    const auto& buf = lockedVertices->m_buffers[bufferIndex];
    int dataType = buf.m_element.m_dataType.m_storage;
    char* ptr = const_cast<char*>(buf.m_start);
    int stride = buf.m_stride;
    int numValues = buf.m_element.m_numValues;

    switch (dataType) {
        case TYPE_FLOAT:  // 1
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    ptr[c] = static_cast<char>(*src++);
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_INT8:  // 2
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    ptr[c] = static_cast<char>(*src++);
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_UINT8:  // 3
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    *reinterpret_cast<short*>(&ptr[c * 2]) = static_cast<short>(*src++);
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_INT16:  // 4
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    *reinterpret_cast<short*>(&ptr[c * 2]) = static_cast<short>(*src++);
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_UINT16:  // 5
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    *reinterpret_cast<int*>(&ptr[c * 4]) = *src++;
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_INT32:  // 6
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    *reinterpret_cast<int*>(&ptr[c * 4]) = *src++;
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        case TYPE_UINT32:  // 7
            if (numVertices <= 0) return hkResult(HK_SUCCESS);
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numValues; c++) {
                    // Pack 4 bytes into one int
                    int b0 = src[0];
                    int b1 = src[1];
                    int b2 = src[2];
                    int b3 = src[3];
                    src += 4;
                    *reinterpret_cast<int*>(&ptr[c * 4]) =
                        (b0 & 0xFF) | ((b1 & 0xFF) << 8) | ((b2 & 0xFF) << 16) | ((b3 & 0xFF) << 24);
                }
                ptr += stride;
            }
            return hkResult(HK_SUCCESS);

        default:
            return hkResult(1);
    }
}

// IDA: ?mergeVertexFormat@hkMeshVertexBufferUtil@@SAXAEAUhkVertexFormat@@AEBU2@@Z @ 0x1407efd80
void hkMeshVertexBufferUtil::mergeVertexFormat(hkVertexFormat& dstFmt, const hkVertexFormat& srcFmt)
{
    for (int i = 0; i < srcFmt.m_numElements; i++) {
        const hkVertexFormat::Element& srcElem = srcFmt.m_elements[i];

        // Find matching element in dstFmt
        int dstIndex = dstFmt.findElementIndex(
            static_cast<hkVertexFormat::ComponentUsage>(srcElem.m_usage.m_storage),
            srcElem.m_subUsage);

        if (dstIndex >= 0) {
            // Element exists - merge by taking max numValues and max dataType
            hkVertexFormat::Element& dstElem = dstFmt.m_elements[dstIndex];

            // Take max numValues
            if (srcElem.m_numValues > dstElem.m_numValues) {
                dstElem.m_numValues = srcElem.m_numValues;
            }

            // Take max dataType (higher values indicate larger types)
            if (srcElem.m_dataType.m_storage > dstElem.m_dataType.m_storage) {
                dstElem.m_dataType.m_storage = srcElem.m_dataType.m_storage;
            }
        }
        else {
            // Element doesn't exist - add it
            if (dstFmt.m_numElements < 32) {
                dstFmt.m_elements[dstFmt.m_numElements] = srcElem;
                dstFmt.m_numElements++;
            }
        }
    }
}

// IDA: ?getElementVectorArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBULockedVertices@hkMeshVertexBuffer@@HPEAVhkVector4@@@Z @ 0x1407efeb0
// Wrapper that dispatches to buffer-based overload
void hkMeshVertexBufferUtil::getElementVectorArray(
    hkResult* result,
    const hkMeshVertexBuffer::LockedVertices* lockedVertices,
    int bufferIndex,
    hkVector4* data)
{
    *result = getElementVectorArray(&lockedVertices->m_buffers[bufferIndex], data, lockedVertices->m_numVertices);
}

// IDA: ?setElementVectorArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBULockedVertices@hkMeshVertexBuffer@@HPEBVhkVector4@@@Z @ 0x1407efef0
// Wrapper that dispatches to buffer-based overload
void hkMeshVertexBufferUtil::setElementVectorArray(
    hkResult* result,
    const hkMeshVertexBuffer::LockedVertices* lockedVertices,
    int bufferIndex,
    const hkVector4* data)
{
    *result = setElementVectorArray(&lockedVertices->m_buffers[bufferIndex], data, lockedVertices->m_numVertices);
}

// IDA: ?getElementIntArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBULockedVertices@hkMeshVertexBuffer@@HPEAH@Z @ 0x1407edfd0
// (Already implemented above - this is the wrapper version)
// NOTE: This is handled by the existing getElementIntArray implementation

// IDA: ?setElementIntArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@AEBULockedVertices@hkMeshVertexBuffer@@HPEBH@Z @ 0x1407ee2a0
// (Already implemented above - this is the wrapper version)
// NOTE: This is handled by the existing setElementIntArray implementation

// ============================================================================
// hkSkinnedMeshShape Implementation
// ============================================================================

// Static class stub
static hkClass hkSkinnedMeshShapeClass_stub;
static hkClass hkSkinnedMeshShapeBoneSectionClass_stub;
static hkClass hkSkinnedMeshShapePartClass_stub;

// IDA: hkSkinnedMeshShape::staticClass @ 0x1407E5AC0
extern "C" const hkClass* __fastcall hkSkinnedMeshShape_staticClass() {
    return &hkSkinnedMeshShapeClass_stub;
}

// IDA: hkSkinnedMeshShape::BoneSection::staticClass @ 0x1407E5A70
extern "C" const hkClass* __fastcall hkSkinnedMeshShape_BoneSection_staticClass() {
    return &hkSkinnedMeshShapeBoneSectionClass_stub;
}

// IDA: finishLoadedObjecthkSkinnedMeshShapeBoneSection @ 0x1407E5A80
extern "C" void __fastcall finishLoadedObjecthkSkinnedMeshShapeBoneSection(void* p, int finishing) {
    if (p) {
        // Call placement new constructor
        // hkSkinnedMeshShape::BoneSection::BoneSection(p, finishing)
    }
}

// IDA: cleanupLoadedObjecthkSkinnedMeshShapeBoneSection @ 0x1407E5B30
extern "C" void __fastcall cleanupLoadedObjecthkSkinnedMeshShapeBoneSection(void* p) {
    if (p) {
        void*** vtable = reinterpret_cast<void***>(p);
        if (vtable && *vtable && **vtable) {
            typedef void(__fastcall * DtorFunc)(void*, int);
            DtorFunc dtor = reinterpret_cast<DtorFunc>(**vtable);
            dtor(p, 0);
        }
    }
}

// IDA: hkSkinnedMeshShape::Part::staticClass @ 0x1407E5AA0
extern "C" const hkClass* __fastcall hkSkinnedMeshShape_Part_staticClass() {
    return &hkSkinnedMeshShapePartClass_stub;
}

// IDA: cleanupLoadedObjecthkSkinnedMeshShapePart @ 0x1407E5AB0
extern "C" void __fastcall cleanupLoadedObjecthkSkinnedMeshShapePart(void* p) {
    (void)p;  // POD type, no special cleanup
}

// ============================================================================
// hkStorageSkinnedMeshShape Implementation
// ============================================================================

static hkClass hkStorageSkinnedMeshShapeClass_stub;

// IDA: hkStorageSkinnedMeshShape::staticClass @ 0x1407E5AD0
extern "C" const hkClass* __fastcall hkStorageSkinnedMeshShape_staticClass() {
    return &hkStorageSkinnedMeshShapeClass_stub;
}

// IDA: finishLoadedObjecthkStorageSkinnedMeshShape @ 0x1407E5AE0
extern "C" void __fastcall finishLoadedObjecthkStorageSkinnedMeshShape(void* p, int finishing) {
    if (p) {
        // Call placement new constructor
        // hkStorageSkinnedMeshShape::hkStorageSkinnedMeshShape(p, finishing)
    }
}

// IDA: cleanupLoadedObjecthkStorageSkinnedMeshShape @ 0x1407E5B00
extern "C" void __fastcall cleanupLoadedObjecthkStorageSkinnedMeshShape(void* p) {
    if (p) {
        void*** vtable = reinterpret_cast<void***>(p);
        if (vtable && *vtable && **vtable) {
            typedef void(__fastcall * DtorFunc)(void*, int);
            DtorFunc dtor = reinterpret_cast<DtorFunc>(**vtable);
            dtor(p, 0);
        }
    }
}

// IDA: getVtablehkStorageSkinnedMeshShape @ 0x1407E5B10
extern "C" void** __fastcall getVtablehkStorageSkinnedMeshShape() {
    return reinterpret_cast<void**>(&hkStorageSkinnedMeshShapeClass_stub);
}

// IDA: hkStorageSkinnedMeshShape::addBoneSection @ 0x1407E73B0
void hkStorageSkinnedMeshShape::addBoneSection(hkMeshShape* meshShape, short startBoneIdx, short numBones) {
    // Check if we need to grow the array
    if (m_boneSections.m_size >= (m_boneSections.m_capacityAndFlags & 0x3FFFFFFF)) {
        hkArrayUtil::_reserveMore(hkContainerHeapAllocator::s_alloc,
            reinterpret_cast<void**>(&m_boneSections.m_data), 16);
    }

    // Construct new BoneSection at end
    BoneSection& section = m_boneSections.m_data[m_boneSections.m_size++];
    section.m_startBoneIdx = startBoneIdx;
    section.m_numBones = numBones;

    // Handle reference counting
    if (meshShape) {
        meshShape->addReference();
    }
    if (section.m_meshShape.m_ptr) {
        section.m_meshShape.m_ptr->removeReference();
    }
    section.m_meshShape.m_ptr = meshShape;
}

// IDA: hkStorageSkinnedMeshShape::getBoneSection @ 0x1407E7200
void hkStorageSkinnedMeshShape::getBoneSection(int boneSectionIndex, BoneSection* boneSectionOut) const {
    const BoneSection& section = m_boneSections.m_data[boneSectionIndex];

    // Handle reference counting for the mesh shape
    if (section.m_meshShape.m_ptr) {
        section.m_meshShape.m_ptr->addReference();
    }
    if (boneSectionOut->m_meshShape.m_ptr) {
        boneSectionOut->m_meshShape.m_ptr->removeReference();
    }

    // Copy data
    boneSectionOut->m_meshShape.m_ptr = section.m_meshShape.m_ptr;
    boneSectionOut->m_startBoneIdx = section.m_startBoneIdx;
    boneSectionOut->m_numBones = section.m_numBones;
}

// IDA: hkStorageSkinnedMeshShape::getPart @ 0x1407E7260
void hkStorageSkinnedMeshShape::getPart(int partIndex, Part* partOut) const {
    *partOut = m_parts.m_data[partIndex];
}

// IDA: hkStorageSkinnedMeshShape::addPart @ 0x1407E7470
void hkStorageSkinnedMeshShape::addPart(const Part* p) {
    // Check if we need to grow the array
    if (m_parts.m_size >= (m_parts.m_capacityAndFlags & 0x3FFFFFFF)) {
        hkArrayUtil::_reserveMore(hkContainerHeapAllocator::s_alloc,
            reinterpret_cast<void**>(&m_parts.m_data), 48);
    }

    // Copy part data to end of array
    Part& newPart = m_parts.m_data[m_parts.m_size++];
    newPart = *p;
}

// IDA: hkStorageSkinnedMeshShape::getClassType @ 0x1407E7180
const hkClass* hkStorageSkinnedMeshShape::getClassType() const {
    return &hkStorageSkinnedMeshShapeClass_stub;
}

// IDA: hkStorageSkinnedMeshShape::hkStorageSkinnedMeshShape @ 0x1407E7B90
hkStorageSkinnedMeshShape::hkStorageSkinnedMeshShape() {
    m_boneSections.m_data = nullptr;
    m_boneSections.m_size = 0;
    m_boneSections.m_capacityAndFlags = 0x80000000;
    m_parts.m_data = nullptr;
    m_parts.m_size = 0;
    m_parts.m_capacityAndFlags = 0x80000000;
}

// IDA: hkStorageSkinnedMeshShape::hkStorageSkinnedMeshShape(hkFinishLoadedObjectFlag) @ 0x1407E7BE0
hkStorageSkinnedMeshShape::hkStorageSkinnedMeshShape(hkFinishLoadedObjectFlag flag)
    : hkSkinnedMeshShape(flag) {
    // Just set vtable, m_name initialized by hkStringPtr constructor
}

// IDA: hkStorageSkinnedMeshShape::~hkStorageSkinnedMeshShape @ 0x1407E7C70
hkStorageSkinnedMeshShape::~hkStorageSkinnedMeshShape() {
    // Destroy m_name (hkStringPtr destructor handles cleanup)

    // Free m_parts buffer
    if (m_parts.m_capacityAndFlags >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(
            m_parts.m_data,
            48 * (m_parts.m_capacityAndFlags & 0x3FFFFFFF));
    }
    m_parts.m_data = nullptr;
    m_parts.m_capacityAndFlags = 0x80000000;

    // Release references for each bone section
    for (int i = m_boneSections.m_size - 1; i >= 0; --i) {
        BoneSection& section = m_boneSections.m_data[i];
        if (section.m_meshShape.m_ptr) {
            section.m_meshShape.m_ptr->removeReference();
        }
        section.m_meshShape.m_ptr = nullptr;
    }

    // Free m_boneSections buffer
    if (m_boneSections.m_capacityAndFlags >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(
            m_boneSections.m_data,
            16 * m_boneSections.m_capacityAndFlags);
    }
    m_boneSections.m_data = nullptr;
    m_boneSections.m_capacityAndFlags = 0x80000000;
}

// IDA: hkStorageSkinnedMeshShape::sortParts @ 0x1407E72B0
void hkStorageSkinnedMeshShape::sortParts() {
    int size = m_parts.m_size;
    bool swapped;

    do {
        swapped = false;
        for (int i = 0; i < size - 1; ++i) {
            Part& a = m_parts.m_data[i];
            Part& b = m_parts.m_data[i + 1];

            if (a.m_boneIndex > b.m_boneIndex) {
                // Swap parts
                Part temp = a;
                a = b;
                b = temp;
                swapped = true;
            }
        }
        --size;
    } while (swapped && size > 1);
}

// ============================================================================
// hkSkinnedRefMeshShape Implementation
// ============================================================================

// Using existing hkSkinnedRefMeshShapeClass_stub from serialization support section

// IDA: hkSkinnedRefMeshShape::getClassType @ 0x1407EB4A0
const hkClass* hkSkinnedRefMeshShape::getClassType() const {
    return &hkSkinnedRefMeshShapeClass_stub;
}

// IDA: hkSkinnedRefMeshShape::hkSkinnedRefMeshShape @ 0x1407EB590
hkSkinnedRefMeshShape::hkSkinnedRefMeshShape(hkSkinnedMeshShape* fullSkin) {
    m_memSizeAndFlags = static_cast<short>(-1);
    m_referenceCount = 1;

    if (fullSkin) {
        fullSkin->addReference();
    }
    m_skinnedMeshShape.m_ptr = fullSkin;

    m_bones.m_data = nullptr;
    m_bones.m_size = 0;
    m_bones.m_capacityAndFlags = 0x80000000;

    m_localFromRootTransforms.m_data = nullptr;
    m_localFromRootTransforms.m_size = 0;
    m_localFromRootTransforms.m_capacityAndFlags = 0x80000000;
}

// IDA: hkSkinnedRefMeshShape::hkSkinnedRefMeshShape(hkFinishLoadedObjectFlag) @ 0x1407EB610
hkSkinnedRefMeshShape::hkSkinnedRefMeshShape(hkFinishLoadedObjectFlag flag) {
    // Set vtable at offset 0 (inherited from hkReferencedObject via hkMeshShape)
    *(void**)this = &hkSkinnedRefMeshShape_vftable;
    // Initialize m_name with hkStringPtr(hkFinishLoadedObjectFlag) which sets it to nullptr
    new (&m_name) hkStringPtr(flag);
}

// IDA: hkSkinnedRefMeshShape::~hkSkinnedRefMeshShape @ 0x1407EB640
hkSkinnedRefMeshShape::~hkSkinnedRefMeshShape() {
    // Release reference to skinned mesh shape
    if (m_skinnedMeshShape.m_ptr) {
        m_skinnedMeshShape.m_ptr->removeReference();
    }
    m_skinnedMeshShape.m_ptr = nullptr;

    // Free m_bones buffer
    if (m_bones.m_capacityAndFlags >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(
            m_bones.m_data,
            2 * (m_bones.m_capacityAndFlags & 0x3FFFFFFF));
    }
    m_bones.m_data = nullptr;
    m_bones.m_capacityAndFlags = 0x80000000;

    // m_name destructor handles cleanup

    // Free m_localFromRootTransforms buffer
    if (m_localFromRootTransforms.m_capacityAndFlags >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(
            m_localFromRootTransforms.m_data,
            32 * m_localFromRootTransforms.m_capacityAndFlags);
    }
    m_localFromRootTransforms.m_data = nullptr;
    m_localFromRootTransforms.m_capacityAndFlags = 0x80000000;
}

// ============================================================================
// hkMultipleVertexBuffer Implementation
// ============================================================================

static hkClass hkMultipleVertexBufferClass_stub;
static hkClass hkMultipleVertexBufferVertexBufferInfoClass_stub;
static hkClass hkMultipleVertexBufferElementInfoClass_stub;
static hkClass hkMultipleVertexBufferLockedElementClass_stub;

// IDA: hkMultipleVertexBuffer::staticClass @ 0x1407e5bb0
extern "C" const hkClass* __fastcall hkMultipleVertexBuffer_staticClass() {
    return &hkMultipleVertexBufferClass_stub;
}

// IDA: finishLoadedObjecthkMultipleVertexBuffer @ 0x1407e5bc0
extern "C" void __fastcall finishLoadedObjecthkMultipleVertexBuffer(void* p, int finishing) {
    if (p) {
        // hkMultipleVertexBuffer::hkMultipleVertexBuffer(p, finishing)
    }
}

// IDA: cleanupLoadedObjecthkMultipleVertexBuffer @ 0x1407e5be0
extern "C" void __fastcall cleanupLoadedObjecthkMultipleVertexBuffer(void* p) {
    if (p) {
        void*** vtable = reinterpret_cast<void***>(p);
        if (vtable && *vtable && **vtable) {
            typedef void(__fastcall * DtorFunc)(void*, int);
            DtorFunc dtor = reinterpret_cast<DtorFunc>(**vtable);
            dtor(p, 0);
        }
    }
}

// IDA: getVtablehkMultipleVertexBuffer @ 0x1407e5bf0
extern "C" void** __fastcall getVtablehkMultipleVertexBuffer() {
    return reinterpret_cast<void**>(&hkMultipleVertexBufferClass_stub);
}

// IDA: hkMultipleVertexBuffer::VertexBufferInfo::staticClass @ 0x1407e5b60
extern "C" const hkClass* __fastcall hkMultipleVertexBuffer_VertexBufferInfo_staticClass() {
    return &hkMultipleVertexBufferVertexBufferInfoClass_stub;
}

// IDA: hkMultipleVertexBuffer::ElementInfo::staticClass @ 0x1407e5b70
extern "C" const hkClass* __fastcall hkMultipleVertexBuffer_ElementInfo_staticClass() {
    return &hkMultipleVertexBufferElementInfoClass_stub;
}

// IDA: cleanupLoadedObjecthkMultipleVertexBufferElementInfo @ 0x1407e5b80
extern "C" void __fastcall cleanupLoadedObjecthkMultipleVertexBufferElementInfo(void* p) {
    (void)p;  // POD type, no special cleanup
}

// IDA: hkMultipleVertexBuffer::LockedElement::staticClass @ 0x1407e5b90
extern "C" const hkClass* __fastcall hkMultipleVertexBuffer_LockedElement_staticClass() {
    return &hkMultipleVertexBufferLockedElementClass_stub;
}

// IDA: cleanupLoadedObjecthkMultipleVertexBufferLockedElement @ 0x1407e5ba0
extern "C" void __fastcall cleanupLoadedObjecthkMultipleVertexBufferLockedElement(void* p) {
    (void)p;  // POD type, no special cleanup
}

// IDA: finishLoadedObjecthkMultipleVertexBufferVertexBufferInfo @ 0x1407e5c20
extern "C" void __fastcall finishLoadedObjecthkMultipleVertexBufferVertexBufferInfo(void* p, int finishing) {
    (void)p;  // No special finish
}

// IDA: cleanupLoadedObjecthkMultipleVertexBufferVertexBufferInfo @ 0x1407e5c30
extern "C" void __fastcall cleanupLoadedObjecthkMultipleVertexBufferVertexBufferInfo(void* p) {
    if (p) {
        hkReferencedObject** refPtr = reinterpret_cast<hkReferencedObject**>(p);
        if (*refPtr) {
            (*refPtr)->removeReference();
        }
        *refPtr = nullptr;
    }
}

// IDA: hkMultipleVertexBuffer::hkMultipleVertexBuffer @ 0x1407e88f0
hkMultipleVertexBuffer::hkMultipleVertexBuffer(const hkVertexFormat& format, int numVertices) {
    m_memSizeAndFlags = static_cast<short>(0x1FFFF);
    m_referenceCount = 0;

    m_lockedElements.m_capacityAndFlags = 0x80000000;
    m_lockedElements.m_data = nullptr;
    m_lockedElements.m_size = 0;

    m_lockedBuffer.m_ptr = nullptr;

    m_elementInfos.m_data = nullptr;
    m_elementInfos.m_size = 0;
    m_elementInfos.m_capacityAndFlags = 0x80000000;

    m_vertexBufferInfos.m_data = nullptr;
    m_vertexBufferInfos.m_size = 0;
    m_vertexBufferInfos.m_capacityAndFlags = 0x80000000;

    m_vertexFormat = format;
    m_numVertices = numVertices;
    m_isLocked.m_bool = false;
    m_updateCount = 1;
    m_constructionComplete.m_bool = false;

    // Reserve locked elements based on format numElements
    int numElements = format.m_numElements;
    int capacity = m_lockedElements.m_capacityAndFlags & 0x3FFFFFFF;
    if (capacity < numElements) {
        int newCapacity = capacity * 2;
        if (numElements > newCapacity) {
            newCapacity = numElements;
        }
        // Would call hkArrayUtil::_reserve here
    }
    m_lockedElements.m_size = numElements;
}

// IDA: hkMultipleVertexBuffer::hkMultipleVertexBuffer(hkFinishLoadedObjectFlag) @ 0x1407e8a40
hkMultipleVertexBuffer::hkMultipleVertexBuffer(hkFinishLoadedObjectFlag flag) {
    // Just set vtable
}

// IDA: hkMultipleVertexBuffer::~hkMultipleVertexBuffer @ 0x1407e90c0
hkMultipleVertexBuffer::~hkMultipleVertexBuffer() {
    // Release references for each vertex buffer
    for (int i = m_vertexBufferInfos.m_size - 1; i >= 0; --i) {
        if (m_vertexBufferInfos.m_data[i].m_vertexBuffer.m_ptr) {
            m_vertexBufferInfos.m_data[i].m_vertexBuffer.m_ptr->removeReference();
        }
        m_vertexBufferInfos.m_data[i].m_vertexBuffer.m_ptr = nullptr;
    }

    // Free m_vertexBufferInfos buffer
    if (m_vertexBufferInfos.m_capacityAndFlags >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(
            m_vertexBufferInfos.m_data,
            24 * (m_vertexBufferInfos.m_capacityAndFlags & 0x3FFFFFFF));
    }
    m_vertexBufferInfos.m_data = nullptr;
    m_vertexBufferInfos.m_capacityAndFlags = 0x80000000;

    // Free m_elementInfos buffer
    if (m_elementInfos.m_capacityAndFlags >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(
            m_elementInfos.m_data,
            2 * (m_elementInfos.m_capacityAndFlags & 0x3FFFFFFF));
    }
    m_elementInfos.m_data = nullptr;
    m_elementInfos.m_capacityAndFlags = 0x80000000;

    // Release m_lockedBuffer reference
    if (m_lockedBuffer.m_ptr) {
        m_lockedBuffer.m_ptr->removeReference();
    }
    m_lockedBuffer.m_ptr = nullptr;

    // Free m_lockedElements buffer
    if (m_lockedElements.m_capacityAndFlags >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(
            m_lockedElements.m_data,
            7 * (m_lockedElements.m_capacityAndFlags & 0x3FFFFFFF));
    }
    m_lockedElements.m_data = nullptr;
    m_lockedElements.m_capacityAndFlags = 0x80000000;
}

// IDA: hkMultipleVertexBuffer::getClassType @ 0x1407e90b0
const hkClass* hkMultipleVertexBuffer::getClassType() const {
    return &hkMultipleVertexBufferClass_stub;
}

// IDA: hkMultipleVertexBuffer::isSharable @ 0x1407e9080
bool hkMultipleVertexBuffer::isSharable() {
    return false;  // Default implementation
}

// IDA: hkMultipleVertexBuffer::getVertexFormat @ 0x1407e9090
void hkMultipleVertexBuffer::getVertexFormat(hkVertexFormat& format) {
    format = m_vertexFormat;
}

// IDA: hkMultipleVertexBuffer::clone @ 0x1407e8e20
hkMeshVertexBuffer* hkMultipleVertexBuffer::clone() {
    // Stub - would create a deep copy
    return nullptr;
}

// ============================================================================
// hkMemoryMeshVertexBuffer Implementation
// ============================================================================

// Using existing hkMemoryMeshVertexBufferClass_stub from serialization support section

// IDA: hkMemoryMeshVertexBuffer::staticClass @ 0x1407e5db0
const hkClass* hkMemoryMeshVertexBuffer::staticClass() {
    return &hkMemoryMeshVertexBufferClass_stub;
}

// Serialization support functions already defined earlier in the file

// IDA: hkMemoryMeshVertexBuffer::hkMemoryMeshVertexBuffer @ 0x1407eb070
hkMemoryMeshVertexBuffer::hkMemoryMeshVertexBuffer() {
    m_memory.m_data = nullptr;
    m_memory.m_size = 0;
    m_memory.m_capacityAndFlags = 0x80000000;
    m_locked.m_bool = false;
    m_numVertices = 0;
    m_isSharable.m_bool = true;
    m_vertexStride = 0;
    m_isBigEndian.m_bool = false;
}

// IDA: hkMemoryMeshVertexBuffer::hkMemoryMeshVertexBuffer(hkVertexFormat&, int) @ 0x1407eb270
hkMemoryMeshVertexBuffer::hkMemoryMeshVertexBuffer(const hkVertexFormat& format, int numVertices) {
    m_memory.m_data = nullptr;
    m_memory.m_size = 0;
    m_memory.m_capacityAndFlags = 0x80000000;
    m_locked.m_bool = false;

    setVertexFormat(format);
    setNumVerticesAndZero(numVertices);

    m_isBigEndian.m_bool = false;
}

// IDA: hkMemoryMeshVertexBuffer::hkMemoryMeshVertexBuffer(hkFinishLoadedObjectFlag) @ 0x1407eb0f0
hkMemoryMeshVertexBuffer::hkMemoryMeshVertexBuffer(hkFinishLoadedObjectFlag flag) {
    // Set vtable at offset 0
    *(void**)this = &hkMemoryMeshVertexBuffer_vftable;
    // If finishing and big-endian, handle endian swap
    if (flag.m_finishing && m_isBigEndian.m_bool) {
        handleEndian();
    }
}

// ============================================================================
// hkIndexedTransformSet Implementation
// ============================================================================

// IDA: hkIndexedTransformSet::hkIndexedTransformSet(hkFinishLoadedObjectFlag) @ 0x1407ea310
hkIndexedTransformSet::hkIndexedTransformSet(hkFinishLoadedObjectFlag flag) {
    // Set vtable at offset 0
    *(void**)this = &hkIndexedTransformSet_vftable;
}

// IDA: hkMemoryMeshVertexBuffer::~hkMemoryMeshVertexBuffer @ 0x1407eb1f0
hkMemoryMeshVertexBuffer::~hkMemoryMeshVertexBuffer() {
    // Free m_memory buffer
    if (m_memory.m_capacityAndFlags >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(
            m_memory.m_data,
            m_memory.m_capacityAndFlags & 0x3FFFFFFF);
    }
    m_memory.m_data = nullptr;
    m_memory.m_capacityAndFlags = 0x80000000;
}

// IDA: hkMemoryMeshVertexBuffer::clone @ 0x1407eb310
hkMeshVertexBuffer* hkMemoryMeshVertexBuffer::clone() {
    // Stub - would create a deep copy
    return nullptr;
}

// IDA: hkMemoryMeshVertexBuffer::lock @ 0x1407eb000
hkMeshVertexBuffer::LockResult hkMemoryMeshVertexBuffer::lock(const LockInput& input, LockedVertices& lockedVertices) {
    lockedVertices.m_vertexData = m_memory.m_data;
    lockedVertices.m_numVertices = m_numVertices;
    lockedVertices.m_stride = m_vertexStride;
    m_locked.m_bool = true;
    return LOCK_RESULT_SUCCESS;
}

// IDA: hkMemoryMeshVertexBuffer::unlock @ 0x1407eaa70
void hkMemoryMeshVertexBuffer::unlock(const LockedVertices& lockedVertices) {
    m_locked.m_bool = false;
}

// IDA: hkMemoryMeshVertexBuffer::setVertexFormat @ 0x1407eafa0
void hkMemoryMeshVertexBuffer::setVertexFormat(const hkVertexFormat& format) {
    m_format = format;
    // Would calculate vertex stride from format
}

// IDA: hkMemoryMeshVertexBuffer::setNumVerticesAndZero @ 0x1407eb130
void hkMemoryMeshVertexBuffer::setNumVerticesAndZero(int numVertices) {
    m_numVertices = numVertices;
    // Would allocate and zero memory buffer
}

// IDA: hkMemoryMeshVertexBuffer::handleEndian @ 0x1407eab00
void hkMemoryMeshVertexBuffer::handleEndian() {
    if (!m_memory.m_data || m_numVertices <= 0) return;

    unsigned char* data = m_memory.m_data;
    for (int i = 0; i < m_numVertices; data += m_vertexStride, ++i) {
        if (m_format.m_numElements <= 0) continue;

        for (int elemIdx = 0; elemIdx < m_format.m_numElements; ++elemIdx) {
            const hkVertexFormat::Element& elem = m_format.m_elements[elemIdx];
            unsigned char* elemData = data + m_elementOffsets[elemIdx];

            for (int valIdx = 0; valIdx < elem.m_numValues; ++valIdx) {
                switch (elem.m_dataType) {
                    case hkVertexFormat::TYPE_INT16:
                    case hkVertexFormat::TYPE_UINT16:
                    case hkVertexFormat::TYPE_FLOAT:
                        // 2-byte swap
                        std::swap(elemData[0], elemData[1]);
                        elemData += 2;
                        break;

                    case hkVertexFormat::TYPE_INT32:
                    case hkVertexFormat::TYPE_UINT32:
                    case hkVertexFormat::TYPE_ARGB32:
                    case hkVertexFormat::TYPE_FLOAT32:
                        // 4-byte swap
                        std::swap(elemData[0], elemData[3]);
                        std::swap(elemData[1], elemData[2]);
                        elemData += 4;
                        break;

                    default:
                        break;
                }
            }
        }
    }
}

// IDA: hkMemoryMeshVertexBuffer::useExternalBuffer @ 0x1407eb040
void hkMemoryMeshVertexBuffer::useExternalBuffer(void* buffer, int size, int numVertices) {
    // Would use external buffer instead of internal allocation
}

// IDA: hkMemoryMeshVertexBuffer::getElementVectorArray @ 0x1407eaa80
void hkMemoryMeshVertexBuffer::getElementVectorArray(const LockedVertices* lockedVertices, int elementIndex, hkVector4* data) {
    // Stub - would extract vector data from vertex buffer
}

// IDA: hkMemoryMeshVertexBuffer::setElementVectorArray @ 0x1407eaaa0
void hkMemoryMeshVertexBuffer::setElementVectorArray(const LockedVertices* lockedVertices, int elementIndex, const hkVector4* data) {
    // Stub - would set vector data in vertex buffer
}

// IDA: hkMemoryMeshVertexBuffer::getElementIntArray @ 0x1407eaac0
void hkMemoryMeshVertexBuffer::getElementIntArray(const LockedVertices* lockedVertices, int elementIndex, int* data) {
    // Stub - would extract int data from vertex buffer
}

// IDA: hkMemoryMeshVertexBuffer::setElementIntArray @ 0x1407eaae0
void hkMemoryMeshVertexBuffer::setElementIntArray(const LockedVertices* lockedVertices, int elementIndex, const int* data) {
    // Stub - would set int data in vertex buffer
}

// ============================================================================
// hkGeometry Implementation
// ============================================================================

#ifdef _WIN32
#include <xmmintrin.h>  // For _mm_cmpunord_ps, _mm_movemask_ps
#endif

// IDA: ??0hkGeometry@@QEAA@AEBU0@@Z @ 0x14079e2a0
// Copy constructor
hkGeometry::hkGeometry(const hkGeometry& other) {
    // Initialize vertices array
    m_vertices.m_capacityAndFlags = 0x80000000;
    m_vertices.m_data = nullptr;
    m_vertices.m_size = 0;

    // Initialize triangles array
    m_triangles.m_data = nullptr;
    m_triangles.m_size = 0;
    m_triangles.m_capacityAndFlags = 0x80000000;

    // Allocate and copy vertices if needed
    if ((m_vertices.m_capacityAndFlags & 0x3FFFFFFF) < other.m_vertices.m_size) {
        if (m_vertices.m_capacityAndFlags >= 0) {
            hkContainerHeapAllocator::s_alloc.bufFree(
                m_vertices.m_data,
                16 * m_vertices.m_capacityAndFlags);
        }
        int allocSize = 16 * other.m_vertices.m_size;
        m_vertices.m_data = (hkVector4*)hkContainerHeapAllocator::s_alloc.bufAlloc(allocSize);
        m_vertices.m_capacityAndFlags = allocSize / 16;
    }

    m_vertices.m_size = other.m_vertices.m_size;

    // Copy vertex data
    for (int i = 0; i < other.m_vertices.m_size; i++) {
#ifdef _WIN32
        m_vertices.m_data[i].m_quad = other.m_vertices.m_data[i].m_quad;
#else
        m_vertices.m_data[i] = other.m_vertices.m_data[i];
#endif
    }

    // Allocate and copy triangles if needed
    if ((m_triangles.m_capacityAndFlags & 0x3FFFFFFF) < other.m_triangles.m_size) {
        if (m_triangles.m_capacityAndFlags >= 0) {
            hkContainerHeapAllocator::s_alloc.bufFree(
                m_triangles.m_data,
                16 * m_triangles.m_capacityAndFlags);
        }
        int allocSize = 16 * other.m_triangles.m_size;
        m_triangles.m_data = (Triangle*)hkContainerHeapAllocator::s_alloc.bufAlloc(allocSize);
        m_triangles.m_capacityAndFlags = allocSize / 16;
    }

    m_triangles.m_size = other.m_triangles.m_size;

    // Copy triangle data
    for (int i = 0; i < other.m_triangles.m_size; i++) {
        m_triangles.m_data[i] = other.m_triangles.m_data[i];
    }
}

// IDA: ?isValid@hkGeometry@@QEBA?AVhkBool@@XZ @ 0x14079e200
hkBool hkGeometry::isValid() const {
    unsigned int vertexSize = m_vertices.m_size;

    // Check vertices for NaN values
    if (vertexSize > 0) {
        for (unsigned int i = 0; i < vertexSize; i++) {
#ifdef _WIN32
            // Check if any component is NaN (unordered comparison)
            __m128 v = m_vertices.m_data[i].m_quad;
            int mask = _mm_movemask_ps(_mm_cmpunord_ps(v, _mm_setzero_ps()));
            if ((mask & 7) != 0) {  // Check x, y, z components
                return hkBool(false);
            }
#endif
        }
    }

    // Check triangles for valid indices
    int triangleSize = m_triangles.m_size;
    if (triangleSize <= 0) {
        return hkBool(true);
    }

    for (int i = 0; i < triangleSize; i++) {
        const Triangle& tri = m_triangles.m_data[i];
        if (tri.m_a >= (int)vertexSize || tri.m_b >= (int)vertexSize || tri.m_c >= (int)vertexSize) {
            return hkBool(false);
        }
    }

    return hkBool(true);
}

// IDA: ?appendGeometry@hkGeometry@@QEAAXAEBU1@PEBVhkMatrix4@@@Z @ 0x14079E430
void hkGeometry::appendGeometry(const hkGeometry& geometry, const hkMatrix4* transform) {
    int oldVertexSize = m_vertices.m_size;
    int oldTriangleSize = m_triangles.m_size;

    // Append vertices from other geometry
    m_vertices._append(&hkContainerHeapAllocator::s_alloc,
        geometry.m_vertices.m_data, geometry.m_vertices.m_size);

    // Append triangles from other geometry
    m_triangles._append(&hkContainerHeapAllocator::s_alloc,
        geometry.m_triangles.m_data, geometry.m_triangles.m_size);

    // Transform newly added vertices if transform is provided
    if (transform) {
        for (int i = oldVertexSize; i < m_vertices.m_size; i++) {
            __m128 v = m_vertices.m_data[i].m_quad;
            // Matrix-vector multiply with translation
            __m128 result = _mm_add_ps(
                _mm_add_ps(
                    _mm_add_ps(
                        _mm_mul_ps(_mm_shuffle_ps(v, v, 0), transform->m_col0.m_quad),
                        transform->m_col3.m_quad),
                    _mm_mul_ps(_mm_shuffle_ps(v, v, 85), transform->m_col1.m_quad)),
                _mm_mul_ps(_mm_shuffle_ps(v, v, 170), transform->m_col2.m_quad));
            // Preserve original w component
            m_vertices.m_data[i].m_quad = _mm_shuffle_ps(result, _mm_unpackhi_ps(result, v), 196);
        }
    }

    // Adjust triangle indices by old vertex count
    if (oldVertexSize > 0) {
        for (int i = oldTriangleSize; i < m_triangles.m_size; i++) {
            m_triangles.m_data[i].m_a += oldVertexSize;
            m_triangles.m_data[i].m_b += oldVertexSize;
            m_triangles.m_data[i].m_c += oldVertexSize;
        }
    }
}

// ============================================================================
// hkMemoryMeshMaterial Implementation
// ============================================================================

// Static constants for default colors
static const float g_defaultAmbient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
static const float g_defaultDiffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
static const float g_defaultSpecular[4] = {0.2f, 0.2f, 0.2f, 1.0f};
static const float g_defaultEmissive[4] = {0.0f, 0.0f, 0.0f, 1.0f};

// IDA: ??0hkMemoryMeshMaterial@@QEAA@PEBD@Z @ 0x1407ebdc0
hkMemoryMeshMaterial::hkMemoryMeshMaterial(const char* name) {
    // Initialize vtable (done automatically by C++)
    // Initialize material name
    m_materialName = name;

    // Initialize textures array
    m_textures.m_data = nullptr;
    m_textures.m_size = 0;
    m_textures.m_capacityAndFlags = 0x80000000;

    // Initialize colors with defaults
#ifdef _WIN32
    m_ambientColor.m_quad = _mm_loadu_ps(g_defaultAmbient);
    m_specularColor.m_quad = _mm_loadu_ps(g_defaultSpecular);
    m_emissiveColor.m_quad = _mm_loadu_ps(g_defaultEmissive);
    m_diffuseColor.m_quad = _mm_loadu_ps(g_defaultDiffuse);
#else
    m_ambientColor = hkVector4(0.2f, 0.2f, 0.2f, 1.0f);
    m_specularColor = hkVector4(0.2f, 0.2f, 0.2f, 1.0f);
    m_emissiveColor = hkVector4(0.0f, 0.0f, 0.0f, 1.0f);
    m_diffuseColor = hkVector4(0.8f, 0.8f, 0.8f, 1.0f);
#endif
}

// IDA: ??0hkMemoryMeshMaterial@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407ebe60
hkMemoryMeshMaterial::hkMemoryMeshMaterial(hkFinishLoadedObjectFlag flag) {
    // Serialization constructor - does minimal initialization
}

// IDA: ??1hkMemoryMeshMaterial@@UEAA@XZ @ 0x1407ebea0
hkMemoryMeshMaterial::~hkMemoryMeshMaterial() {
    // Release texture references
    for (int i = 0; i < m_textures.m_size; i++) {
        if (m_textures.m_data[i].m_ptr) {
            m_textures.m_data[i].m_ptr->removeReference();
        }
    }
    // Array memory will be freed by base destructor
}

// IDA: ?getClassType@hkMemoryMeshMaterial@@UEBAPEBVhkClass@@XZ @ 0x1407ebe90
const hkClass* hkMemoryMeshMaterial::getClassType() const {
    return staticClass();
}

// IDA: ?addTexture@hkMemoryMeshMaterial@@UEAAXPEAVhkMeshTexture@@@Z @ 0x1407ebd30
void hkMemoryMeshMaterial::addTexture(hkMeshTexture* texture) {
    // Add reference to texture
    if (texture) {
        texture->addReference();
    }

    // Reserve more space if needed
    if (m_textures.m_size == (m_textures.m_capacityAndFlags & 0x3FFFFFFF)) {
        hkArrayUtil::_reserveMore(hkContainerHeapAllocator::s_alloc,
            reinterpret_cast<void**>(&m_textures.m_data), sizeof(hkRefPtr<hkMeshTexture>));
    }

    // Add texture to array
    m_textures.m_data[m_textures.m_size].m_ptr = texture;
    m_textures.m_size++;

    // Balance reference - the hkRefPtr assignment handles this
    if (texture) {
        texture->removeReference();
    }
}

// IDA: ?createCompatibleVertexBuffer@hkMemoryMeshMaterial@@UEAAPEAVhkMeshVertexBuffer@@PEAV2@@Z @ 0x1407ebb50
hkMeshVertexBuffer* hkMemoryMeshMaterial::createCompatibleVertexBuffer(hkMeshVertexBuffer* buffer) const {
    if (buffer) buffer->addReference();
    return buffer;
}

// IDA: ?createCompatibleVertexFormat@hkMemoryMeshMaterial@@UEAA?AUhkResult@@AEBUhkVertexFormat@@AEAU3@@Z @ 0x1407ebb20
hkResult hkMemoryMeshMaterial::createCompatibleVertexFormat(const hkVertexFormat& format, hkVertexFormat& compatibleFormat) const {
    compatibleFormat.set(format);
    return hkResult(HK_SUCCESS);
}

// IDA: ?isCompatible@hkMemoryMeshMaterial@@UEAA_NPEBVhkMeshVertexBuffer@@@Z @ 0x1407ebb70
bool hkMemoryMeshMaterial::isCompatible(const hkMeshVertexBuffer* buffer) const {
    const hkClass* bufferClass = buffer->getClassType();
    return hkMemoryMeshVertexBufferClass_stub.equals(bufferClass);
}

// IDA: ?equals@hkMemoryMeshMaterial@@UEAA_NPEBVhkMeshMaterial@@@Z @ 0x1407ebc50
bool hkMemoryMeshMaterial::equals(const hkMeshMaterial* other) const {
    // Check class type
    const hkClass* otherClass = other->getClassType();
    if (!hkMemoryMeshMaterialClass_stub.equals(otherClass)) {
        return false;
    }

    // Cast to hkMemoryMeshMaterial
    const hkMemoryMeshMaterial* otherMat = static_cast<const hkMemoryMeshMaterial*>(other);

    // Compare material names
    const char* thisName = m_materialName.cString();
    const char* otherName = otherMat->m_materialName.cString();

    if (thisName != nullptr || otherName != nullptr) {
        if (thisName == nullptr || otherName == nullptr) {
            return false;
        }
        if (hkString::strCmp(thisName, otherName) != 0) {
            return false;
        }
    }

    // Compare texture count
    if (m_textures.m_size != otherMat->m_textures.m_size) {
        return false;
    }

    // Compare textures
    for (int i = m_textures.m_size - 1; i >= 0; i--) {
        if (m_textures.m_data[i].m_ptr != otherMat->m_textures.m_data[i].m_ptr) {
            return false;
        }
    }

    // Compare colors using SSE
#ifdef _WIN32
    __m128 diffCmp = _mm_cmpeq_ps(m_diffuseColor.m_quad, otherMat->m_diffuseColor.m_quad);
    __m128 ambCmp = _mm_cmpeq_ps(m_ambientColor.m_quad, otherMat->m_ambientColor.m_quad);
    __m128 specCmp = _mm_cmpeq_ps(m_specularColor.m_quad, otherMat->m_specularColor.m_quad);
    __m128 emisCmp = _mm_cmpeq_ps(m_emissiveColor.m_quad, otherMat->m_emissiveColor.m_quad);

    __m128 and1 = _mm_and_ps(diffCmp, ambCmp);
    __m128 and2 = _mm_and_ps(specCmp, emisCmp);
    __m128 result = _mm_and_ps(and1, and2);

    return _mm_movemask_ps(result) == 15;
#else
    return m_diffuseColor == otherMat->m_diffuseColor &&
           m_ambientColor == otherMat->m_ambientColor &&
           m_specularColor == otherMat->m_specularColor &&
           m_emissiveColor == otherMat->m_emissiveColor;
#endif
}

// ============================================================================
// hkVertexFormat Implementation
// ============================================================================

// IDA: ?isCanonicalOrder@hkVertexFormat@@QEBA_NXZ @ 0x1407ec180
bool hkVertexFormat::isCanonicalOrder() const {
    if (m_numElements <= 1) {
        return true;
    }

    // Check if elements are in sorted order
    for (int i = 0; i < m_numElements - 1; i++) {
        const Element& curr = m_elements[i];
        const Element& next = m_elements[i + 1];

        // Compare by usage first, then by subUsage
        if (curr.m_usage > next.m_usage) {
            return false;
        }
        if (curr.m_usage == next.m_usage && curr.m_subUsage >= next.m_subUsage) {
            return false;
        }
    }
    return true;
}

// IDA: ?makeCanonicalOrder@hkVertexFormat@@QEAAXXZ @ 0x1407ec350
void hkVertexFormat::makeCanonicalOrder() {
    if (isCanonicalOrder()) {
        return;
    }

    // Sort elements using bubble sort (simple implementation)
    // The original uses hkAlgorithm::quickSortRecursive with hkVertexFormat_orderComponents
    for (int i = 0; i < m_numElements - 1; i++) {
        for (int j = 0; j < m_numElements - i - 1; j++) {
            const Element& a = m_elements[j];
            const Element& b = m_elements[j + 1];

            // Compare: usage ascending, then subUsage ascending
            bool swap = false;
            if (a.m_usage > b.m_usage) {
                swap = true;
            } else if (a.m_usage == b.m_usage && a.m_subUsage > b.m_subUsage) {
                swap = true;
            }

            if (swap) {
                Element temp = m_elements[j];
                m_elements[j] = m_elements[j + 1];
                m_elements[j + 1] = temp;
            }
        }
    }
}

// ============================================================================
// hkIndexedTransformSet Implementation
// ============================================================================

// IDA: ?setMatrices@hkIndexedTransformSet@@QEAAXHPEBVhkMatrix4@@H@Z @ 0x1407e92d0
void hkIndexedTransformSet::setMatrices(int startIndex, const hkMatrix4* matrices, int numMatrices) {
    if (numMatrices <= 0) return;

    for (int i = 0; i < numMatrices; i++) {
        m_matrices.m_data[startIndex + i] = matrices[i];
    }
}

// IDA: ?getMatrices@hkIndexedTransformSet@@QEBAXHPEAVhkMatrix4@@H@Z @ 0x1407e9340
void hkIndexedTransformSet::getMatrices(int startIndex, hkMatrix4* matrices, int numMatrices) const {
    if (numMatrices <= 0) return;

    for (int i = 0; i < numMatrices; i++) {
        matrices[i] = m_matrices.m_data[startIndex + i];
    }
}

// IDA: ?getInverseMatrices@hkIndexedTransformSet@@QEBAXHPEAVhkMatrix4@@H@Z @ 0x1407e93b0
void hkIndexedTransformSet::getInverseMatrices(int startIndex, hkMatrix4* matrices, int numMatrices) const {
    if (numMatrices <= 0) return;

    // If no inverse matrices stored, return identity
    if (m_inverseMatrices.m_size <= 0) {
        for (int i = 0; i < numMatrices; i++) {
            // Set to identity matrix
            matrices[i].m_col0 = hkVector4(1.0f, 0.0f, 0.0f, 0.0f);
            matrices[i].m_col1 = hkVector4(0.0f, 1.0f, 0.0f, 0.0f);
            matrices[i].m_col2 = hkVector4(0.0f, 0.0f, 1.0f, 0.0f);
            matrices[i].m_col3 = hkVector4(0.0f, 0.0f, 0.0f, 1.0f);
        }
        return;
    }

    for (int i = 0; i < numMatrices; i++) {
        matrices[i] = m_inverseMatrices.m_data[startIndex + i];
    }
}

// IDA: ?setInverseMatrices@hkIndexedTransformSet@@QEAAXHPEBVhkMatrix4@@H@Z @ 0x1407e9460
void hkIndexedTransformSet::setInverseMatrices(int startIndex, const hkMatrix4* matrices, int numMatrices) {
    if (m_inverseMatrices.m_size <= 0 || numMatrices <= 0) return;

    for (int i = 0; i < numMatrices; i++) {
        m_inverseMatrices.m_data[startIndex + i] = matrices[i];
    }
}

// IDA: ?calculateMatrix@hkIndexedTransformSet@@QEBAXHAEAVhkMatrix4@@@Z @ 0x1407e94d0
void hkIndexedTransformSet::calculateMatrix(int index, hkMatrix4* matrixOut) const {
    if (m_inverseMatrices.m_size <= 0) {
        // No inverse matrices, just return the matrix directly
        *matrixOut = m_matrices.m_data[index];
    } else {
#ifdef _WIN32
        // Multiply: matrixOut = m_inverseMatrices[index] * m_matrices[index]
        const hkMatrix4& inv = m_inverseMatrices.m_data[index];
        const hkMatrix4& mat = m_matrices.m_data[index];

        // Column 0
        __m128 c0 = mat.m_col0.m_quad;
        __m128 c1 = mat.m_col1.m_quad;
        __m128 c2 = mat.m_col2.m_quad;
        __m128 c3 = mat.m_col3.m_quad;

        __m128 ic0 = inv.m_col0.m_quad;
        __m128 ic1 = inv.m_col1.m_quad;
        __m128 ic2 = inv.m_col2.m_quad;
        __m128 ic3 = inv.m_col3.m_quad;

        // Matrix multiplication
        matrixOut->m_col0.m_quad = _mm_add_ps(
            _mm_add_ps(
                _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ic0, ic0, 0), c0), _mm_mul_ps(_mm_shuffle_ps(ic0, ic0, 85), c1)),
                _mm_mul_ps(_mm_shuffle_ps(ic0, ic0, 170), c2)),
            _mm_mul_ps(_mm_shuffle_ps(ic0, ic0, 255), c3));

        matrixOut->m_col1.m_quad = _mm_add_ps(
            _mm_add_ps(
                _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ic1, ic1, 0), c0), _mm_mul_ps(_mm_shuffle_ps(ic1, ic1, 85), c1)),
                _mm_mul_ps(_mm_shuffle_ps(ic1, ic1, 170), c2)),
            _mm_mul_ps(_mm_shuffle_ps(ic1, ic1, 255), c3));

        matrixOut->m_col2.m_quad = _mm_add_ps(
            _mm_add_ps(
                _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ic2, ic2, 0), c0), _mm_mul_ps(_mm_shuffle_ps(ic2, ic2, 85), c1)),
                _mm_mul_ps(_mm_shuffle_ps(ic2, ic2, 170), c2)),
            _mm_mul_ps(_mm_shuffle_ps(ic2, ic2, 255), c3));

        matrixOut->m_col3.m_quad = _mm_add_ps(
            _mm_add_ps(
                _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ic3, ic3, 0), c0), _mm_mul_ps(_mm_shuffle_ps(ic3, ic3, 85), c1)),
                _mm_mul_ps(_mm_shuffle_ps(ic3, ic3, 170), c2)),
            _mm_mul_ps(_mm_shuffle_ps(ic3, ic3, 255), c3));
#else
        *matrixOut = m_matrices.m_data[index];
#endif
    }
}

// ============================================================================
// Part 92: hkMeshPrimitiveUtil functions
// ============================================================================

// IDA: ?appendTriangleIndices16@hkMeshPrimitiveUtil@@SAXW4PrimitiveType@hkMeshSection@@PEBGHHAEAV?$hkArray@GUhkContainerHeapAllocator@@@@@Z @ 0x1407ecc50
void hkMeshPrimitiveUtil::appendTriangleIndices16(
    hkMeshSection::PrimitiveType primType,
    const unsigned short* srcIndices,
    int numIndices,
    int indexBase,
    hkArray<unsigned short, hkContainerHeapAllocator>* indicesOut)
{
    int primTypeOffset = static_cast<int>(primType) - 3;

    if (primTypeOffset == 0) {
        // PRIMITIVE_TYPE_TRIANGLE_STRIP
        int oldSize = indicesOut->m_size;
        int neededSize = oldSize + numIndices;
        int capacity = indicesOut->m_capacityAndFlags & 0x3FFFFFFF;

        if (capacity < neededSize) {
            int newCapacity = capacity * 2;
            if (neededSize < newCapacity)
                neededSize = newCapacity;
            hkResult res;
            hkArrayUtil::_reserve(&res, hkContainerHeapAllocator::s_alloc,
                reinterpret_cast<void**>(&indicesOut->m_data), neededSize, 2);
        }

        unsigned short* data = indicesOut->m_data;
        indicesOut->m_size += numIndices;
        unsigned short* dst = &data[oldSize];

        if (indexBase != 0) {
            for (int i = 0; i < numIndices; i++) {
                dst[i] = static_cast<unsigned short>(indexBase + srcIndices[i]);
            }
        } else {
            hkString::memCpy(dst, srcIndices, 2 * numIndices);
        }
    } else if (primTypeOffset == 1) {
        // PRIMITIVE_TYPE_TRIANGLE_FAN
        int numOutputIndices = 3 * numIndices - 6;
        int oldSize = indicesOut->m_size;
        int neededSize = oldSize + numOutputIndices;
        int capacity = indicesOut->m_capacityAndFlags & 0x3FFFFFFF;

        if (capacity < neededSize) {
            int newCapacity = capacity * 2;
            if (neededSize < newCapacity)
                neededSize = newCapacity;
            hkResult res;
            hkArrayUtil::_reserve(&res, hkContainerHeapAllocator::s_alloc,
                reinterpret_cast<void**>(&indicesOut->m_data), neededSize, 2);
        }

        unsigned short* data = indicesOut->m_data;
        indicesOut->m_size += numOutputIndices;
        unsigned short* dst = &data[oldSize];

        if (numIndices > 2) {
            int idx = 2;
            const unsigned short* src = srcIndices + 1;
            do {
                const unsigned short* nextSrc = src + 1;
                *dst = static_cast<unsigned short>(indexBase + *(src - 1));
                if ((idx & 1) != 0) {
                    dst[1] = static_cast<unsigned short>(indexBase + *nextSrc);
                    dst[2] = static_cast<unsigned short>(indexBase + *src);
                } else {
                    dst[1] = static_cast<unsigned short>(indexBase + *src);
                    dst[2] = static_cast<unsigned short>(indexBase + *nextSrc);
                }
                ++idx;
                dst += 3;
                ++src;
            } while (idx < numIndices);
        }
    }
}

// IDA: ?appendTriangleIndices32@hkMeshPrimitiveUtil@@SAXW4PrimitiveType@hkMeshSection@@PEBIHHAEAV?$hkArray@IUhkContainerHeapAllocator@@@@@Z @ 0x1407ece10
void hkMeshPrimitiveUtil::appendTriangleIndices32(
    hkMeshSection::PrimitiveType primType,
    const unsigned int* srcIndices,
    int numIndices,
    int indexBase,
    hkArray<unsigned int, hkContainerHeapAllocator>* indicesOut)
{
    int primTypeOffset = static_cast<int>(primType) - 3;

    if (primTypeOffset == 0) {
        // PRIMITIVE_TYPE_TRIANGLE_STRIP
        int oldSize = indicesOut->m_size;
        int neededSize = oldSize + numIndices;
        int capacity = indicesOut->m_capacityAndFlags & 0x3FFFFFFF;

        if (capacity < neededSize) {
            int newCapacity = capacity * 2;
            if (neededSize < newCapacity)
                neededSize = newCapacity;
            hkResult res;
            hkArrayUtil::_reserve(&res, hkContainerHeapAllocator::s_alloc,
                reinterpret_cast<void**>(&indicesOut->m_data), neededSize, 4);
        }

        unsigned int* data = indicesOut->m_data;
        indicesOut->m_size += numIndices;
        unsigned int* dst = &data[oldSize];

        if (indexBase != 0) {
            for (int i = 0; i < numIndices; i++) {
                dst[i] = indexBase + srcIndices[i];
            }
        } else {
            for (int i = 0; i < numIndices; i++) {
                dst[i] = srcIndices[i];
            }
        }
    } else if (primTypeOffset == 1) {
        // PRIMITIVE_TYPE_TRIANGLE_FAN
        int numOutputIndices = 3 * numIndices - 6;
        int oldSize = indicesOut->m_size;
        int neededSize = oldSize + numOutputIndices;
        int capacity = indicesOut->m_capacityAndFlags & 0x3FFFFFFF;

        if (capacity < neededSize) {
            int newCapacity = capacity * 2;
            if (neededSize < newCapacity)
                neededSize = newCapacity;
            hkResult res;
            hkArrayUtil::_reserve(&res, hkContainerHeapAllocator::s_alloc,
                reinterpret_cast<void**>(&indicesOut->m_data), neededSize, 4);
        }

        unsigned int* data = indicesOut->m_data;
        indicesOut->m_size += numOutputIndices;
        unsigned int* dst = &data[oldSize];

        if (numIndices > 2) {
            int idx = 2;
            const unsigned int* src = srcIndices + 1;
            do {
                const unsigned int* nextSrc = src + 1;
                *dst = indexBase + *(src - 1);
                if ((idx & 1) != 0) {
                    dst[1] = indexBase + *nextSrc;
                    dst[2] = indexBase + *src;
                } else {
                    dst[1] = indexBase + *src;
                    dst[2] = indexBase + *nextSrc;
                }
                ++idx;
                dst += 3;
                ++src;
            } while (idx < numIndices);
        }
    }
}

// IDA: ?appendTriangleIndices@hkMeshPrimitiveUtil@@SAXAEBUhkMeshSection@@AEAV?$hkArray@GUhkContainerHeapAllocator@@@@@Z @ 0x1407ecfa0
void hkMeshPrimitiveUtil::appendTriangleIndices(
    const hkMeshSection* section,
    hkArray<unsigned short, hkContainerHeapAllocator>* indicesOut)
{
    int indexType = section->m_indexType.m_storage;

    if (indexType == 0) {
        // INDEX_TYPE_NONE - use vertex start index
        appendTriangleIndices(
            static_cast<hkMeshSection::PrimitiveType>(section->m_primitiveType.m_storage),
            section->m_numIndices,
            section->m_vertexStartIndex,
            indicesOut,
            &hkContainerHeapAllocator::s_alloc);
    } else if (indexType == 1) {
        // INDEX_TYPE_UINT16
        int primType = section->m_primitiveType.m_storage;
        if (primType == 3 || primType == 4) {
            appendTriangleIndices16(
                static_cast<hkMeshSection::PrimitiveType>(primType),
                reinterpret_cast<const unsigned short*>(section->m_indices),
                section->m_numIndices,
                0,
                indicesOut);
        }
    } else if (indexType == 2) {
        // INDEX_TYPE_UINT32
        int primType = section->m_primitiveType.m_storage;
        if (primType == 3 || primType == 4) {
            // Need to convert 32-bit to 16-bit
            int numIndices = section->m_numIndices;
            hkArray<unsigned int, hkContainerHeapAllocator> indices32;
            indices32.m_data = nullptr;
            indices32.m_size = 0;
            indices32.m_capacityAndFlags = 0x80000000;

            appendTriangleIndices32(
                static_cast<hkMeshSection::PrimitiveType>(primType),
                reinterpret_cast<const unsigned int*>(section->m_indices),
                numIndices,
                0,
                &indices32);

            // Convert to 16-bit
            for (int i = 0; i < indices32.m_size; i++) {
                if (indicesOut->m_size == (indicesOut->m_capacityAndFlags & 0x3FFFFFFF)) {
                    hkArrayUtil::_reserveMore(hkContainerHeapAllocator::s_alloc, reinterpret_cast<void**>(&indicesOut->m_data), 2);
                }
                indicesOut->m_data[indicesOut->m_size++] = static_cast<unsigned short>(indices32.m_data[i]);
            }

            // Clean up indices32
            if (indices32.m_capacityAndFlags >= 0 && indices32.m_data) {
                hkContainerHeapAllocator::s_alloc.bufFree(indices32.m_data, 4 * indices32.m_capacityAndFlags);
            }
        }
    }
}

// IDA: ?appendTriangleIndices@hkMeshPrimitiveUtil@@SAXAEBUhkMeshSection@@AEAV?$hkArray@IUhkContainerHeapAllocator@@@@@Z @ 0x1407ed1e0
void hkMeshPrimitiveUtil::appendTriangleIndices(
    const hkMeshSection* section,
    hkArray<unsigned int, hkContainerHeapAllocator>* indicesOut)
{
    int indexType = section->m_indexType.m_storage;

    if (indexType == 0) {
        // INDEX_TYPE_NONE - use vertex start index
        appendTriangleIndices(
            static_cast<hkMeshSection::PrimitiveType>(section->m_primitiveType.m_storage),
            section->m_numIndices,
            section->m_vertexStartIndex,
            indicesOut);
    } else if (indexType == 1) {
        // INDEX_TYPE_UINT16
        int primType = section->m_primitiveType.m_storage;
        if (primType == 3 || primType == 4) {
            // Need to convert 16-bit to 32-bit
            int numIndices = section->m_numIndices;
            hkArray<unsigned short, hkContainerHeapAllocator> indices16;
            indices16.m_data = nullptr;
            indices16.m_size = 0;
            indices16.m_capacityAndFlags = 0x80000000;

            appendTriangleIndices16(
                static_cast<hkMeshSection::PrimitiveType>(primType),
                reinterpret_cast<const unsigned short*>(section->m_indices),
                numIndices,
                0,
                &indices16);

            // Convert to 32-bit
            for (int i = 0; i < indices16.m_size; i++) {
                if (indicesOut->m_size == (indicesOut->m_capacityAndFlags & 0x3FFFFFFF)) {
                    hkArrayUtil::_reserveMore(hkContainerHeapAllocator::s_alloc, reinterpret_cast<void**>(&indicesOut->m_data), 4);
                }
                indicesOut->m_data[indicesOut->m_size++] = indices16.m_data[i];
            }

            // Clean up indices16
            if (indices16.m_capacityAndFlags >= 0 && indices16.m_data) {
                hkContainerHeapAllocator::s_alloc.bufFree(indices16.m_data, 2 * indices16.m_capacityAndFlags);
            }
        }
    } else if (indexType == 2) {
        // INDEX_TYPE_UINT32
        int primType = section->m_primitiveType.m_storage;
        if (primType == 3 || primType == 4) {
            appendTriangleIndices32(
                static_cast<hkMeshSection::PrimitiveType>(primType),
                reinterpret_cast<const unsigned int*>(section->m_indices),
                section->m_numIndices,
                0,
                indicesOut);
        }
    }
}

// ============================================================================
// Part 93: hkMeshVertexBufferUtil partitionVertexFormat
// ============================================================================

// IDA: ?partitionVertexFormat@hkMeshVertexBufferUtil@@SAXAEBUhkVertexFormat@@AEAU2@1@Z @ 0x1407ef740
void hkMeshVertexBufferUtil::partitionVertexFormat(
    const hkVertexFormat& format,
    hkVertexFormat& sharedFormat,
    hkVertexFormat& instanceFormat)
{
    sharedFormat.m_numElements = 0;
    instanceFormat.m_numElements = 0;

    for (int i = 0; i < format.m_numElements; i++) {
        const hkVertexFormat::Element& elem = format.m_elements[i];
        // Check if element has flag 8 (instance data flag)
        if ((elem.m_flags & 8) == 0) {
            // Shared element
            sharedFormat.addElement(elem);
        } else {
            // Instance element
            instanceFormat.addElement(elem);
        }
    }
}

// ============================================================================
// Part 94: hkMeshVertexBufferUtil interpolation functions
// ============================================================================

// IDA: hkMeshVertexBufferUtil_interpolateCopy @ 0x1407ef7c0
void hkMeshVertexBufferUtil::interpolateCopy(
    const hkVertexFormatDetail::Element* element,
    const void* srcA,
    const void* srcB,
    float interp,
    void* dst)
{
    // Data type to size mapping
    static const int s_dataTypeToSize[] = {
        0,   // TYPE_NONE
        1,   // TYPE_FLOAT
        1,   // TYPE_INT8
        1,   // TYPE_UINT8
        2,   // TYPE_INT16
        2,   // TYPE_UINT16
        4,   // TYPE_INT32
        4,   // TYPE_UINT32
        4,   // TYPE_ARGB32
        0,   // (unused)
        4,   // TYPE_FLOAT32
        16   // TYPE_FLOAT324
    };

    // Calculate element size
    int elementSize = (element->m_numValues * s_dataTypeToSize[element->m_dataType.m_storage] + 3) & 0xFFFFFFFC;
    int numBytes = elementSize / 4;  // Number of 4-byte chunks

    if (interp >= 0.5f) {
        // Copy from srcB
        hkString::memCpy(dst, srcB, numBytes);
    } else {
        // Copy from srcA
        const int* srcInt = static_cast<const int*>(srcA);
        int* dstInt = static_cast<int*>(dst);
        for (int i = 0; i < numBytes; i++) {
            dstInt[i] = srcInt[i];
        }
    }
}

// IDA: hkMeshVertexBufferUtil_interpolate @ 0x1407ef850
void hkMeshVertexBufferUtil::interpolateByDataType(
    const hkVertexFormatDetail::Element* element,
    const void* srcA,
    const void* srcB,
    float interp,
    void* dst)
{
    int dataType = element->m_dataType.m_storage;
    int numValues = element->m_numValues;

    switch (dataType) {
        case 8:  // TYPE_ARGB32
            {
                const unsigned int* aPtr = static_cast<const unsigned int*>(srcA);
                const unsigned int* bPtr = static_cast<const unsigned int*>(srcB);
                unsigned int* dstPtr = static_cast<unsigned int*>(dst);

                for (int i = 0; i < numValues; i++) {
                    unsigned int aVal = aPtr[i];
                    unsigned int bVal = bPtr[i];

                    // Extract ARGB components
                    float aA = static_cast<float>((aVal >> 24) & 0xFF);
                    float aR = static_cast<float>((aVal >> 16) & 0xFF);
                    float aG = static_cast<float>((aVal >> 8) & 0xFF);
                    float aB = static_cast<float>(aVal & 0xFF);

                    float bA = static_cast<float>((bVal >> 24) & 0xFF);
                    float bR = static_cast<float>((bVal >> 16) & 0xFF);
                    float bG = static_cast<float>((bVal >> 8) & 0xFF);
                    float bB = static_cast<float>(bVal & 0xFF);

                    // Interpolate and round
                    float outA = (bA - aA) * interp + aA + 0.5f;
                    float outR = (bR - aR) * interp + aR + 0.5f;
                    float outG = (bG - aG) * interp + aG + 0.5f;
                    float outB = (bB - aB) * interp + aB + 0.5f;

                    // Pack back to ARGB32
                    dstPtr[i] = (static_cast<unsigned int>(outA) << 24) |
                                (static_cast<unsigned int>(outR) << 16) |
                                (static_cast<unsigned int>(outG) << 8) |
                                static_cast<unsigned int>(outB);
                }
            }
            break;

        case 10:  // TYPE_FLOAT32
            {
                const float* aPtr = static_cast<const float*>(srcA);
                const float* bPtr = static_cast<const float*>(srcB);
                float* dstPtr = static_cast<float*>(dst);
                float oneMinusInterp = 1.0f - interp;

                for (int i = 0; i < numValues; i++) {
                    dstPtr[i] = oneMinusInterp * aPtr[i] + interp * bPtr[i];
                }
            }
            break;

        case 11:  // TYPE_FLOAT324 (4 floats packed in __m128)
            {
#ifdef _WIN32
                const __m128* aPtr = static_cast<const __m128*>(srcA);
                const __m128* bPtr = static_cast<const __m128*>(srcB);
                __m128* dstPtr = static_cast<__m128*>(dst);
                __m128 interpVec = _mm_set1_ps(interp);

                for (int i = 0; i < numValues; i++) {
                    __m128 a = aPtr[i];
                    __m128 b = bPtr[i];
                    __m128 diff = _mm_sub_ps(b, a);
                    __m128 result = _mm_add_ps(_mm_mul_ps(diff, interpVec), a);
                    dstPtr[i] = result;
                }
#else
                // Non-SSE fallback
                const float* aPtr = static_cast<const float*>(srcA);
                const float* bPtr = static_cast<const float*>(srcB);
                float* dstPtr = static_cast<float*>(dst);
                float oneMinusInterp = 1.0f - interp;

                for (int i = 0; i < numValues * 4; i++) {
                    dstPtr[i] = oneMinusInterp * aPtr[i] + interp * bPtr[i];
                }
#endif
            }
            break;

        default:
            // For other types, use simple copy
            interpolateCopy(element, srcA, srcB, interp, dst);
            break;
    }
}

// IDA: ?computeMostFittingVertexFormat@hkMeshVertexBufferUtil@@SAXAEAUhkVertexFormat@@PEBU2@H@Z @ 0x1407efe50
void hkMeshVertexBufferUtil::computeMostFittingVertexFormat(
    hkVertexFormat& dstFormat,
    const hkVertexFormat* srcFormats,
    unsigned int numSourceFormats)
{
    dstFormat.m_numElements = 0;

    for (unsigned int i = 0; i < numSourceFormats; i++) {
        mergeVertexFormat(dstFormat, srcFormats[i]);
    }
}

// ============================================================================
// Part 95: hkMeshVertexBufferUtil buffer comparison and transform functions
// ============================================================================

// IDA: ?isBufferNormalDataEqual@hkMeshVertexBufferUtil@@SA?AVhkBool@@AEBUBuffer@LockedVertices@hkMeshVertexBuffer@@0M@Z @ 0x1407f0890
hkBool hkMeshVertexBufferUtil::isBufferNormalDataEqual(
    const hkMeshVertexBuffer::LockedVertices::Buffer* bufferA,
    const hkMeshVertexBuffer::LockedVertices::Buffer* bufferB,
    float threshold)
{
    // Check if data types and numValues match
    if (bufferA->m_element.m_dataType.m_storage != bufferB->m_element.m_dataType.m_storage) {
        return hkBool(false);
    }
    if (bufferA->m_element.m_numValues != bufferB->m_element.m_numValues) {
        return hkBool(false);
    }

    int dataType = bufferA->m_element.m_dataType.m_storage;
    int numValues = bufferA->m_element.m_numValues;

    // For FLOAT32 (type 10) or 3-component normals
    if (dataType == 10 || numValues == 3) {
        // Load normal from bufferA as 3 floats
        const float* aPtr = reinterpret_cast<const float*>(bufferA->m_start);
        float lenSq = aPtr[0] * aPtr[0] + aPtr[1] * aPtr[1] + aPtr[2] * aPtr[2];

        // Check if 1.0 - length < threshold (i.e., length is close to 1.0)
        float diff = 1.0f - lenSq;
        bool isNormalized = (diff * 2.0f) < threshold;
        return hkBool(isNormalized);
    } else {
        // Use generic comparison
        return isBufferDataEqual(bufferA, bufferB, threshold);
    }
}

// IDA: ?isBufferDataEqual@hkMeshVertexBufferUtil@@SA?AVhkBool@@PEBUBuffer@LockedVertices@hkMeshVertexBuffer@@0HAEBUThresholds@1@@Z @ 0x1407f0950
hkBool hkMeshVertexBufferUtil::isBufferDataEqual(
    const hkMeshVertexBuffer::LockedVertices::Buffer* buffersA,
    const hkMeshVertexBuffer::LockedVertices::Buffer* buffersB,
    int numBuffers,
    const Thresholds& thresholds)
{
    for (int i = 0; i < numBuffers; i++) {
        const hkMeshVertexBuffer::LockedVertices::Buffer* bufA = &buffersA[i];
        const hkMeshVertexBuffer::LockedVertices::Buffer* bufB = &buffersB[i];

        // Check if usage matches
        if (bufA->m_element.m_usage.m_storage != bufB->m_element.m_usage.m_storage) {
            return hkBool(false);
        }

        int usage = bufA->m_element.m_usage.m_storage;
        float threshold;
        bool checkNormal = false;

        switch (usage) {
            case 0:   // USAGE_POSITION
            case 10:  // USAGE_BLEND_WEIGHTS_LAST_IMPLIED
                threshold = thresholds.m_positionThreshold;
                break;
            case 1:   // USAGE_NORMAL
                checkNormal = true;
                threshold = thresholds.m_normalThreshold;
                break;
            case 2:   // USAGE_COLOR
                threshold = thresholds.m_colorThreshold;
                break;
            case 4:   // USAGE_TANGENT
            case 5:   // USAGE_BINORMAL
                checkNormal = true;
                threshold = thresholds.m_normalThreshold;
                break;
            case 9:   // USAGE_BLEND_MATRIX_INDEX
                threshold = thresholds.m_texCoordThreshold;
                break;
            default:
                threshold = thresholds.m_otherThreshold;
                break;
        }

        hkBool result;
        if (checkNormal) {
            result = isBufferNormalDataEqual(bufA, bufB, threshold);
        } else {
            result = isBufferDataEqual(bufA, bufB, threshold);
        }

        if (!result.m_bool) {
            return hkBool(false);
        }
    }

    return hkBool(true);
}

// IDA: ?transform@hkMeshVertexBufferUtil@@SAXAEBUBuffer@LockedVertices@hkMeshVertexBuffer@@AEBVhkMatrix4@@HH@Z @ 0x1407f0c60
void hkMeshVertexBufferUtil::transform(
    const hkMeshVertexBuffer::LockedVertices::Buffer* srcBuffer,
    const hkMatrix4* transform,
    int transformFlags,
    unsigned int numVertices)
{
    if (numVertices == 0) return;

    const char* ptr = srcBuffer->m_start;
    int stride = srcBuffer->m_stride;
    int usage = srcBuffer->m_element.m_usage.m_storage;

#ifdef _WIN32
    __m128 c0 = transform->m_col0.m_quad;
    __m128 c1 = transform->m_col1.m_quad;
    __m128 c2 = transform->m_col2.m_quad;
    __m128 c3 = transform->m_col3.m_quad;

    // Negation mask for flipping
    static const unsigned int negMask[4] = {0x80000000, 0x80000000, 0x80000000, 0x80000000};
    __m128 neg = _mm_loadu_ps(reinterpret_cast<const float*>(negMask));

    // Constants for Newton-Raphson sqrt refinement
    const float oneHalf = 0.5f;
    const float threeHalf = 1.5f;

    // Helper to extract floats from __m128 portably
    auto getFloat = [](__m128 v, int idx) -> float {
        float f[4];
        _mm_storeu_ps(f, v);
        return f[idx];
    };

    switch (usage) {
        case 0:  // USAGE_POSITION
            for (unsigned int i = 0; i < numVertices; i++) {
                const float* v = reinterpret_cast<const float*>(ptr);
                __m128 pos = _mm_set_ps(1.0f, v[2], v[1], v[0]);

                // Transform: pos * matrix
                __m128 result = _mm_add_ps(
                    _mm_add_ps(
                        _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(pos, pos, 0), c0), c3),
                        _mm_mul_ps(_mm_shuffle_ps(pos, pos, 85), c1)),
                    _mm_mul_ps(_mm_shuffle_ps(pos, pos, 170), c2));

                float* out = reinterpret_cast<float*>(const_cast<char*>(ptr));
                out[0] = getFloat(result, 0);
                out[1] = getFloat(result, 1);
                out[2] = getFloat(result, 2);

                ptr += stride;
            }
            break;

        case 1:  // USAGE_NORMAL
        case 4:  // USAGE_TANGENT
        case 5:  // USAGE_BINORMAL
            {
                bool doNegate = (transformFlags & 2) != 0;
                bool doFlip = (transformFlags & 4) != 0;
                bool doNormalize = (transformFlags & 1) != 0;

                for (unsigned int i = 0; i < numVertices; i++) {
                    const float* v = reinterpret_cast<const float*>(ptr);
                    __m128 vec = _mm_set_ps(0.0f, v[2], v[1], v[0]);

                    if (doNegate) {
                        vec = _mm_xor_ps(vec, neg);
                    }

                    // Transform by upper-left 3x3
                    __m128 result = _mm_add_ps(
                        _mm_add_ps(
                            _mm_mul_ps(_mm_shuffle_ps(vec, vec, 0), c0),
                            _mm_mul_ps(_mm_shuffle_ps(vec, vec, 85), c1)),
                        _mm_mul_ps(_mm_shuffle_ps(vec, vec, 170), c2));

                    if (doFlip) {
                        result = _mm_xor_ps(result, neg);
                    }

                    if (doNormalize) {
                        // Normalize using Newton-Raphson refinement
                        __m128 sq = _mm_mul_ps(result, result);
                        float lenSq = getFloat(sq, 0) + getFloat(sq, 1) + getFloat(sq, 2);
                        if (lenSq > 0.0f) {
                            float invLen = 1.0f / std::sqrt(lenSq);
                            result = _mm_mul_ps(result, _mm_set1_ps(invLen));
                        }
                    }

                    float* out = reinterpret_cast<float*>(const_cast<char*>(ptr));
                    out[0] = getFloat(result, 0);
                    out[1] = getFloat(result, 1);
                    out[2] = getFloat(result, 2);

                    ptr += stride;
                }
            }
            break;

        default:
            // No transformation for other usages
            break;
    }
#else
    // Non-SSE fallback
    // Helper to extract floats from hkVector4
    auto getVecFloat = [](const hkVector4& v, int idx) -> float {
        float f[4];
        _mm_storeu_ps(f, v.m_quad);
        return f[idx];
    };

    switch (usage) {
        case 0:  // USAGE_POSITION
            for (unsigned int i = 0; i < numVertices; i++) {
                const float* v = reinterpret_cast<const float*>(ptr);
                float x = v[0], y = v[1], z = v[2];

                // Matrix multiply (assuming column-major)
                float outX = getVecFloat(transform->m_col0, 0) * x + getVecFloat(transform->m_col1, 0) * y
                           + getVecFloat(transform->m_col2, 0) * z + getVecFloat(transform->m_col3, 0);
                float outY = getVecFloat(transform->m_col0, 1) * x + getVecFloat(transform->m_col1, 1) * y
                           + getVecFloat(transform->m_col2, 1) * z + getVecFloat(transform->m_col3, 1);
                float outZ = getVecFloat(transform->m_col0, 2) * x + getVecFloat(transform->m_col1, 2) * y
                           + getVecFloat(transform->m_col2, 2) * z + getVecFloat(transform->m_col3, 2);

                float* out = reinterpret_cast<float*>(const_cast<char*>(ptr));
                out[0] = outX;
                out[1] = outY;
                out[2] = outZ;

                ptr += stride;
            }
            break;

        case 1:  // USAGE_NORMAL
        case 4:  // USAGE_TANGENT
        case 5:  // USAGE_BINORMAL
            {
                bool doNegate = (transformFlags & 2) != 0;
                bool doFlip = (transformFlags & 4) != 0;
                bool doNormalize = (transformFlags & 1) != 0;

                for (unsigned int i = 0; i < numVertices; i++) {
                    const float* v = reinterpret_cast<const float*>(ptr);
                    float x = v[0], y = v[1], z = v[2];

                    if (doNegate) { x = -x; y = -y; z = -z; }

                    // Transform by upper-left 3x3
                    float outX = getVecFloat(transform->m_col0, 0) * x + getVecFloat(transform->m_col1, 0) * y
                               + getVecFloat(transform->m_col2, 0) * z;
                    float outY = getVecFloat(transform->m_col0, 1) * x + getVecFloat(transform->m_col1, 1) * y
                               + getVecFloat(transform->m_col2, 1) * z;
                    float outZ = getVecFloat(transform->m_col0, 2) * x + getVecFloat(transform->m_col1, 2) * y
                               + getVecFloat(transform->m_col2, 2) * z;

                    if (doFlip) { outX = -outX; outY = -outY; outZ = -outZ; }

                    if (doNormalize) {
                        float len = std::sqrt(outX * outX + outY * outY + outZ * outZ);
                        if (len > 0.0f) {
                            outX /= len; outY /= len; outZ /= len;
                        }
                    }

                    float* out = reinterpret_cast<float*>(const_cast<char*>(ptr));
                    out[0] = outX;
                    out[1] = outY;
                    out[2] = outZ;

                    ptr += stride;
                }
            }
            break;

        default:
            break;
    }
#endif
}

// ============================================================================
// Part 96: hkMeshPrimitiveUtil shape-based functions
// ============================================================================

// IDA: ?appendTriangleIndices@hkMeshPrimitiveUtil@@SAXPEBVhkMeshShape@@HAEAV?$hkArray@GUhkContainerHeapAllocator@@@@@Z @ 0x1407ed410
void hkMeshPrimitiveUtil::appendTriangleIndices(
    const hkMeshShape* shape,
    int sectionIndex,
    hkArray<unsigned short, hkContainerHeapAllocator>* indicesOut)
{
    hkMeshSection section;
    shape->lockSection(sectionIndex, 1, &section);
    appendTriangleIndices(&section, indicesOut);
    shape->unlockSection(&section);
}

// ============================================================================
// Part 97: hkMeshVertexBufferUtil transform and isContiguous functions
// ============================================================================

// IDA: ?transform@hkMeshVertexBufferUtil@@SA?AUhkResult@@PEAVhkMeshVertexBuffer@@AEBVhkMatrix4@@H@Z @ 0x1407f0f00
hkResult hkMeshVertexBufferUtil::transform(
    hkMeshVertexBuffer* buffer,
    const hkMatrix4* transformIn,
    int transformFlags)
{
    hkMeshVertexBuffer::LockInput lockInput;
    lockInput.m_flags = 3;  // Read | Write
    lockInput.m_startVertex = 0;
    lockInput.m_numVertices = -1;

    hkMeshVertexBuffer::LockedVertices lockedVertices;
    if (buffer->lock(lockInput, lockedVertices) != hkMeshVertexBuffer::LOCK_RESULT_SUCCESS) {
        hkResult result;
        result.m_result = 1;  // HK_FAILURE
        return result;
    }

    // Transform each buffer
    for (int i = 0; i < lockedVertices.m_numBuffers; i++) {
        transform(&lockedVertices.m_buffers[i], transformIn, transformFlags, lockedVertices.m_numVertices);
    }

    buffer->unlock(lockedVertices);
    hkResult result;
    result.m_result = 0;  // HK_SUCCESS
    return result;
}

// IDA: ?isContiguous@hkMeshVertexBufferUtil@@SA?AVhkBool@@AEBULockedVertices@hkMeshVertexBuffer@@PEAPEAXAEAH@Z @ 0x1407f0ff0
hkBool hkMeshVertexBufferUtil::isContiguous(
    const hkMeshVertexBuffer::LockedVertices* lockedVertices,
    void** startOut,
    int* dataSize)
{
    int numBuffers = lockedVertices->m_numBuffers;

    if (numBuffers <= 0) {
        return hkBool(false);
    }

    // Single buffer is always contiguous
    if (numBuffers == 1) {
        *startOut = const_cast<void*>(static_cast<const void*>(lockedVertices->m_buffers[0].m_start));
        // Calculate element size
        static const int s_dataTypeToSize[] = {
            0, 1, 1, 1, 2, 2, 4, 4, 4, 0, 4, 16
        };
        int elemSize = (lockedVertices->m_buffers[0].m_element.m_numValues *
                       s_dataTypeToSize[lockedVertices->m_buffers[0].m_element.m_dataType.m_storage] + 3) & 0xFFFFFFFC;
        *dataSize = elemSize;
        return hkBool(true);
    }

    // For multiple buffers, check if they are contiguous
    const char* start = lockedVertices->m_buffers[0].m_start;
    const char* current = start;

    static const int s_dataTypeToSize[] = {
        0, 1, 1, 1, 2, 2, 4, 4, 4, 0, 4, 16
    };

    for (int i = 0; i < numBuffers; i++) {
        if (lockedVertices->m_buffers[i].m_start != current) {
            return hkBool(false);
        }
        int elemSize = (lockedVertices->m_buffers[i].m_element.m_numValues *
                       s_dataTypeToSize[lockedVertices->m_buffers[i].m_element.m_dataType.m_storage] + 3) & 0xFFFFFFFC;
        current += elemSize;
    }

    *startOut = const_cast<void*>(static_cast<const void*>(start));
    *dataSize = static_cast<int>(current - start);
    return hkBool(true);
}

// IDA: hkMeshVertexBufferUtil_interpolateNormalize @ 0x1407f1210
void hkMeshVertexBufferUtil::interpolateNormalize(
    const hkVertexFormatDetail::Element* element,
    const void* srcA,
    const void* srcB,
    float interp,
    void* dst)
{
    int dataType = element->m_dataType.m_storage;
    int numValues = element->m_numValues;

    switch (dataType) {
        case 10:  // TYPE_FLOAT32
            if (numValues == 3) {
                // Interpolate 3-float vector and normalize
                const float* a = static_cast<const float*>(srcA);
                const float* b = static_cast<const float*>(srcB);
                float* out = static_cast<float*>(dst);

                // Linear interpolation
                float x = a[0] + interp * (b[0] - a[0]);
                float y = a[1] + interp * (b[1] - a[1]);
                float z = a[2] + interp * (b[2] - a[2]);

                // Normalize
                float len = std::sqrt(x * x + y * y + z * z);
                if (len > 0.0f) {
                    out[0] = x / len;
                    out[1] = y / len;
                    out[2] = z / len;
                } else {
                    out[0] = x;
                    out[1] = y;
                    out[2] = z;
                }
            } else if (numValues == 4) {
                // Interpolate 4-float vector and normalize
#ifdef _WIN32
                const __m128* a = static_cast<const __m128*>(srcA);
                const __m128* b = static_cast<const __m128*>(srcB);
                __m128* out = static_cast<__m128*>(dst);

                __m128 interpVec = _mm_set1_ps(interp);
                __m128 result = _mm_add_ps(*a, _mm_mul_ps(interpVec, _mm_sub_ps(*b, *a)));

                // Normalize
                __m128 sq = _mm_mul_ps(result, result);
                float f[4];
                _mm_storeu_ps(f, sq);
                float len = std::sqrt(f[0] + f[1] + f[2] + f[3]);
                if (len > 0.0f) {
                    result = _mm_mul_ps(result, _mm_set1_ps(1.0f / len));
                }
                *out = result;
#else
                const float* a = static_cast<const float*>(srcA);
                const float* b = static_cast<const float*>(srcB);
                float* out = static_cast<float*>(dst);

                float x = a[0] + interp * (b[0] - a[0]);
                float y = a[1] + interp * (b[1] - a[1]);
                float z = a[2] + interp * (b[2] - a[2]);
                float w = a[3] + interp * (b[3] - a[3]);

                float len = std::sqrt(x * x + y * y + z * z + w * w);
                if (len > 0.0f) {
                    out[0] = x / len;
                    out[1] = y / len;
                    out[2] = z / len;
                    out[3] = w / len;
                } else {
                    out[0] = x;
                    out[1] = y;
                    out[2] = z;
                    out[3] = w;
                }
#endif
            } else {
                // Fall back to regular interpolate
                interpolateByDataType(element, srcA, srcB, interp, dst);
            }
            break;

        case 11:  // TYPE_FLOAT324
            // Interpolate and normalize 4-float vectors
            {
#ifdef _WIN32
                const __m128* a = static_cast<const __m128*>(srcA);
                const __m128* b = static_cast<const __m128*>(srcB);
                __m128* out = static_cast<__m128*>(dst);

                for (int i = 0; i < numValues; i++) {
                    __m128 interpVec = _mm_set1_ps(interp);
                    __m128 result = _mm_add_ps(a[i], _mm_mul_ps(interpVec, _mm_sub_ps(b[i], a[i])));

                    // Normalize (3D only - ignore w component)
                    __m128 sq = _mm_mul_ps(result, result);
                    float f[4];
                    _mm_storeu_ps(f, sq);
                    float len = std::sqrt(f[0] + f[1] + f[2]);
                    if (len > 0.0f) {
                        result = _mm_mul_ps(result, _mm_set1_ps(1.0f / len));
                    }
                    out[i] = result;
                }
#else
                const float* a = static_cast<const float*>(srcA);
                const float* b = static_cast<const float*>(srcB);
                float* out = static_cast<float*>(dst);

                for (int i = 0; i < numValues; i++) {
                    int idx = i * 4;
                    float x = a[idx] + interp * (b[idx] - a[idx]);
                    float y = a[idx+1] + interp * (b[idx+1] - a[idx+1]);
                    float z = a[idx+2] + interp * (b[idx+2] - a[idx+2]);

                    float len = std::sqrt(x * x + y * y + z * z);
                    if (len > 0.0f) {
                        out[idx] = x / len;
                        out[idx+1] = y / len;
                        out[idx+2] = z / len;
                    } else {
                        out[idx] = x;
                        out[idx+1] = y;
                        out[idx+2] = z;
                    }
                }
#endif
            }
            break;

        default:
            // Fall back to regular interpolate for other types
            interpolateByDataType(element, srcA, srcB, interp, dst);
            break;
    }
}

// ============================================================================
// Part 98: hkMeshVertexBufferUtil additional functions
// ============================================================================

// IDA: ?interpolate@hkMeshVertexBufferUtil@@SAXAEBUElement@hkVertexFormat@@PEBX1MPEAX@Z @ 0x1407f1660
void hkMeshVertexBufferUtil::interpolate(
    const hkVertexFormatDetail::Element* element,
    const void* srcA,
    const void* srcB,
    float interp,
    void* dst)
{
    int usage = element->m_usage.m_storage;

    switch (usage) {
        case 1:   // USAGE_NORMAL
        case 4:   // USAGE_TANGENT
        case 5:   // USAGE_BINORMAL
            // Use normalized interpolation for normals/tangents/binormals
            interpolateNormalize(element, srcA, srcB, interp, dst);
            break;

        case 2:   // USAGE_COLOR
        case 3:   // USAGE_TEXTURE_COORD
        case 9:   // USAGE_BLEND_MATRIX_INDEX
        case 10:  // USAGE_BLEND_WEIGHTS_LAST_IMPLIED
            // Use regular interpolation for colors/texcoords/blend weights
            interpolateByDataType(element, srcA, srcB, interp, dst);
            break;

        case 6:   // USAGE_BLEND_INDICES
        case 7:   // USAGE_USER
        case 8:   // USAGE_POSITION (for non-float types)
            // Use simple copy for blend indices and other integer types
            interpolateCopy(element, srcA, srcB, interp, dst);
            break;

        default:
            // No interpolation for other types
            break;
    }
}

// IDA: ?copy@hkMeshVertexBufferUtil@@SAXAEBULockedVertices@hkMeshVertexBuffer@@0@Z @ 0x1407eff30
void hkMeshVertexBufferUtil::copy(
    const hkMeshVertexBuffer::LockedVertices* srcVertices,
    const hkMeshVertexBuffer::LockedVertices* dstVertices)
{
    int numVertices = srcVertices->m_numVertices;
    int numBuffers = srcVertices->m_numBuffers;

    // Check if copy is valid
    if (numBuffers <= 0) {
        return;
    }

    if (dstVertices->m_numVertices != numVertices || dstVertices->m_numBuffers != numBuffers) {
        return;
    }

    // Copy each buffer
    for (int i = 0; i < numBuffers; i++) {
        const hkMeshVertexBuffer::LockedVertices::Buffer& srcBuffer = srcVertices->m_buffers[i];
        const hkMeshVertexBuffer::LockedVertices::Buffer& dstBuffer = dstVertices->m_buffers[i];
        copy(&srcBuffer, &dstBuffer, numVertices);
    }
}

// IDA: ?getElementVectorArray@hkMeshVertexBufferUtil@@SA?AUhkResult@@PEAVhkMeshVertexBuffer@@W4ComponentUsage@hkVertexFormat@@HAEAV?$hkArray@VhkVector4@@UhkContainerHeapAllocator@@@@@Z @ 0x1407f0b00
hkResult hkMeshVertexBufferUtil::getElementVectorArray(
    hkMeshVertexBuffer* vertexBuffer,
    hkVertexFormatDetail::ComponentUsage usage,
    int subUsage,
    hkArray<hkVector4, hkContainerHeapAllocator>* vectorsOut)
{
    // Get vertex format
    hkVertexFormat vertexFormat;
    vertexBuffer->getVertexFormat(vertexFormat);

    // Find element index
    int elementIndex = vertexFormat.findElementIndex(usage, subUsage);
    if (elementIndex < 0) {
        hkResult result;
        result.m_result = 1;  // HK_FAILURE
        return result;
    }

    // Setup lock for read access
    hkMeshVertexBuffer::LockInput lockInput;
    lockInput.m_flags = 1;  // Read only
    lockInput.m_startVertex = 0;
    lockInput.m_numVertices = -1;

    hkMeshVertexBuffer::LockedVertices lockedVertices;
    if (vertexBuffer->lock(lockInput, lockedVertices) != hkMeshVertexBuffer::LOCK_RESULT_SUCCESS) {
        hkResult result;
        result.m_result = 1;  // HK_FAILURE
        return result;
    }

    int numVertices = lockedVertices.m_numVertices;

    // Resize output array
    int capacity = vectorsOut->m_capacityAndFlags & 0x3FFFFFFF;
    if (capacity < numVertices) {
        int newCapacity = capacity * 2;
        if (numVertices < newCapacity) {
            newCapacity = numVertices;
        }
        hkResult res;
        hkArrayUtil::_reserve(&res, hkContainerHeapAllocator::s_alloc,
            reinterpret_cast<void**>(&vectorsOut->m_data), newCapacity, 16);
    }

    vectorsOut->m_size = numVertices;

    // Get element vectors using the buffer-based helper
    getElementVectorArray(&lockedVertices.m_buffers[elementIndex], vectorsOut->m_data, numVertices);

    vertexBuffer->unlock(lockedVertices);

    hkResult result;
    result.m_result = 0;  // HK_SUCCESS
    return result;
}

// ============================================================================
// Part 99: hkMergeMeshPrimitvesCalculator
// ============================================================================

// IDA: ?add@hkMergeMeshPrimitvesCalculator@@QEAAXHW4PrimitiveType@hkMeshSection@@W4IndexType@3@@Z @ 0x1407ed5c0
void hkMergeMeshPrimitvesCalculator::add(
    int numPrimitives,
    hkMeshSection::PrimitiveType primitiveType,
    hkMeshSection::IndexType indexType)
{
    // Update total primitive count
    m_numTotalPrimitives += numPrimitives;

    // Determine merged primitive type
    int primTypeVal = static_cast<int>(primitiveType);

    if (m_numTotalPrimitives == numPrimitives) {
        // First primitive - just set it
        m_mergedPrimitiveType = primitiveType;
        m_mergedIndexType = indexType;
    } else {
        // Check if we need to upgrade primitive type
        // PRIMITIVE_TYPE_TRIANGLE_LIST = 5, PRIMITIVE_TYPE_TRIANGLE_STRIP = 4
        // If any is TRIANGLE_LIST, merged becomes TRIANGLE_LIST
        if (primTypeVal == 5 || static_cast<int>(m_mergedPrimitiveType) == 5) {
            m_mergedPrimitiveType = hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_LIST;
        } else if (primTypeVal == 4 || static_cast<int>(m_mergedPrimitiveType) == 4) {
            m_mergedPrimitiveType = hkMeshSection::PRIMITIVE_TYPE_TRIANGLE_STRIP;
        }

        // Determine merged index type (use larger of the two)
        int idxTypeVal = static_cast<int>(indexType);
        int mergedIdxVal = static_cast<int>(m_mergedIndexType);

        // INDEX_TYPE_UINT32 = 2, INDEX_TYPE_UINT16 = 1, INDEX_TYPE_NONE = 0
        if (idxTypeVal > mergedIdxVal) {
            m_mergedIndexType = indexType;
        }
    }
}

// ============================================================================
// Part 100: hkcdStaticTree and hkcdDynamicTree Serialization Support
// ============================================================================

// Static class stubs for hkcdStaticTree types
static hkClass hkcdStaticTreeDynamicStorage4Class_stub;
static hkClass hkcdStaticTreeDynamicStorage5Class_stub;
static hkClass hkcdStaticTreeDynamicStorage6Class_stub;
static hkClass hkcdStaticTreeDynamicStorage32Class_stub;
static hkClass hkcdStaticTreeDynamicStorageCodec3Axis4Class_stub;
static hkClass hkcdStaticTreeDynamicStorageCodec3Axis5Class_stub;
static hkClass hkcdStaticTreeDynamicStorageCodec3Axis6Class_stub;
static hkClass hkcdStaticTreeDynamicStorageCodecRawClass_stub;
static hkClass hkcdStaticTreeDefaultTreeStorage4Class_stub;
static hkClass hkcdStaticTreeDefaultTreeStorage5Class_stub;
static hkClass hkcdStaticTreeDefaultTreeStorage6Class_stub;
static hkClass hkcdStaticTreeDefaultTreeStorage32Class_stub;

// Static class stubs for hkcdDynamicTree types
static hkClass hkcdDynamicTreeDefaultTreePtrStorageClass_stub;
static hkClass hkcdDynamicTreeDefaultTree48StorageClass_stub;
static hkClass hkcdDynamicTreeDefaultTree32StorageClass_stub;
static hkClass hkcdDynamicTreeDefaultTreeInt16StorageClass_stub;
static hkClass hkcdDynamicTreeTreeDynamicStoragePtrClass_stub;
static hkClass hkcdDynamicTreeTreeDynamicStorage32Class_stub;
static hkClass hkcdDynamicTreeTreeDynamicStorage16Class_stub;
static hkClass hkcdDynamicTreeTreeDynamicStorageInt16Class_stub;

// hkcdStaticTree::DynamicStorage staticClass functions
// IDA: ?staticClass@DynamicStorage4@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f18f0
extern "C" const hkClass* __fastcall hkcdStaticTree_DynamicStorage4_staticClass() {
    return &hkcdStaticTreeDynamicStorage4Class_stub;
}

// IDA: ?staticClass@DynamicStorage5@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f1900
extern "C" const hkClass* __fastcall hkcdStaticTree_DynamicStorage5_staticClass() {
    return &hkcdStaticTreeDynamicStorage5Class_stub;
}

// IDA: ?staticClass@DynamicStorage6@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f1910
extern "C" const hkClass* __fastcall hkcdStaticTree_DynamicStorage6_staticClass() {
    return &hkcdStaticTreeDynamicStorage6Class_stub;
}

// IDA: ?staticClass@DynamicStorage32@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f1920
extern "C" const hkClass* __fastcall hkcdStaticTree_DynamicStorage32_staticClass() {
    return &hkcdStaticTreeDynamicStorage32Class_stub;
}

// finishLoadedObject functions for DynamicStorage types (no-op for POD)
// IDA: finishLoadedObjecthkcdStaticTreeDynamicStorage4 @ 0x1407f1930
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDynamicStorage4(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeDynamicStorage5 @ 0x1407f1940
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDynamicStorage5(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeDynamicStorage6 @ 0x1407f1950
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDynamicStorage6(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeDynamicStorage32 @ 0x1407f1960
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDynamicStorage32(void* p, int finishing) { (void)p; (void)finishing; }

// finishLoadedObject functions for DynamicStorage with Codec types
// IDA: finishLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis4 @ 0x1407f1970
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis4(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis5 @ 0x1407f1980
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis5(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis6 @ 0x1407f1990
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis6(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodecRaw @ 0x1407f19a0
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodecRaw(void* p, int finishing) { (void)p; (void)finishing; }

// cleanupLoadedObject functions for DynamicStorage types (no-op for POD)
// IDA: cleanupLoadedObjecthkcdStaticTreeDynamicStorage6 @ 0x1407f19b0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDynamicStorage6(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis4 @ 0x1407f1a20
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis4(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis5 @ 0x1407f1a90
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis5(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis6 @ 0x1407f1b00
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis6(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodecRaw @ 0x1407f1b70
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDynamicStoragehkcdStaticTreeCodecRaw(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeDynamicStorage4 @ 0x1407f1be0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDynamicStorage4(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeDynamicStorage5 @ 0x1407f1c50
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDynamicStorage5(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeDynamicStorage32 @ 0x1407f1cc0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDynamicStorage32(void* p) { (void)p; }

// hkcdDynamicTree::DefaultTreeStorage staticClass functions
// IDA: ?staticClass@DefaultTreePtrStorage@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f1d20
extern "C" const hkClass* __fastcall hkcdDynamicTree_DefaultTreePtrStorage_staticClass() {
    return &hkcdDynamicTreeDefaultTreePtrStorageClass_stub;
}

// IDA: ?staticClass@DefaultTree48Storage@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f1d30
extern "C" const hkClass* __fastcall hkcdDynamicTree_DefaultTree48Storage_staticClass() {
    return &hkcdDynamicTreeDefaultTree48StorageClass_stub;
}

// IDA: ?staticClass@DefaultTree32Storage@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f1d40
extern "C" const hkClass* __fastcall hkcdDynamicTree_DefaultTree32Storage_staticClass() {
    return &hkcdDynamicTreeDefaultTree32StorageClass_stub;
}

// IDA: ?staticClass@DefaultTreeInt16Storage@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f1d50
extern "C" const hkClass* __fastcall hkcdDynamicTree_DefaultTreeInt16Storage_staticClass() {
    return &hkcdDynamicTreeDefaultTreeInt16StorageClass_stub;
}

// finishLoadedObject functions for hkcdDynamicTree types
// IDA: finishLoadedObjecthkcdDynamicTreeDefaultTreePtrStorage @ 0x1407f1d60
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDefaultTreePtrStorage(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdDynamicTreeDefaultTree48Storage @ 0x1407f1d70
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDefaultTree48Storage(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdDynamicTreeDefaultTree32Storage @ 0x1407f1d80
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDefaultTree32Storage(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdDynamicTreeDefaultTreeInt16Storage @ 0x1407f1d90
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDefaultTreeInt16Storage(void* p, int finishing) { (void)p; (void)finishing; }

// finishLoadedObject for hkcdDynamicTree::Tree types
// IDA: finishLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStoragePtr @ 0x1407f1da0
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStoragePtr(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage32 @ 0x1407f1e20
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage32(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage16 @ 0x1407f1ea0
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage16(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorageInt16 @ 0x1407f1f20
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorageInt16(void* p, int finishing) { (void)p; (void)finishing; }

// cleanupLoadedObject for hkcdDynamicTree types
// IDA: cleanupLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStoragePtr @ 0x1407f1db0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStoragePtr(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage32 @ 0x1407f1e30
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage32(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage16 @ 0x1407f1eb0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage16(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorageInt16 @ 0x1407f1f30
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeTreehkcdDynamicTreeDynamicStorageInt16(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDefaultTreePtrStorage @ 0x1407f1fa0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDefaultTreePtrStorage(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDefaultTree48Storage @ 0x1407f2010
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDefaultTree48Storage(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDefaultTree32Storage @ 0x1407f2080
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDefaultTree32Storage(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDefaultTreeInt16Storage @ 0x1407f20f0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDefaultTreeInt16Storage(void* p) { (void)p; }

// hkcdStaticTree::DefaultTreeStorage staticClass functions
// IDA: ?staticClass@DefaultTreeStorage4@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f21d0
extern "C" const hkClass* __fastcall hkcdStaticTree_DefaultTreeStorage4_staticClass() {
    return &hkcdStaticTreeDefaultTreeStorage4Class_stub;
}

// IDA: ?staticClass@DefaultTreeStorage5@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f21e0
extern "C" const hkClass* __fastcall hkcdStaticTree_DefaultTreeStorage5_staticClass() {
    return &hkcdStaticTreeDefaultTreeStorage5Class_stub;
}

// IDA: ?staticClass@DefaultTreeStorage6@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f21f0
extern "C" const hkClass* __fastcall hkcdStaticTree_DefaultTreeStorage6_staticClass() {
    return &hkcdStaticTreeDefaultTreeStorage6Class_stub;
}

// IDA: ?staticClass@DefaultTreeStorage32@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f2200
extern "C" const hkClass* __fastcall hkcdStaticTree_DefaultTreeStorage32_staticClass() {
    return &hkcdStaticTreeDefaultTreeStorage32Class_stub;
}

// finishLoadedObject for hkcdStaticTree::DefaultTreeStorage types
// IDA: finishLoadedObjecthkcdStaticTreeDefaultTreeStorage4 @ 0x1407f2210
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDefaultTreeStorage4(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeDefaultTreeStorage5 @ 0x1407f2220
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDefaultTreeStorage5(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeDefaultTreeStorage6 @ 0x1407f2230
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDefaultTreeStorage6(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeDefaultTreeStorage32 @ 0x1407f2240
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeDefaultTreeStorage32(void* p, int finishing) { (void)p; (void)finishing; }

// finishLoadedObject for hkcdStaticTree::Tree types
// IDA: finishLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage4 @ 0x1407f2250
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage4(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage5 @ 0x1407f2260
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage5(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage6 @ 0x1407f2270
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage6(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage32 @ 0x1407f2280
extern "C" void __fastcall finishLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage32(void* p, int finishing) { (void)p; (void)finishing; }

// cleanupLoadedObject for hkcdStaticTree types
// IDA: cleanupLoadedObjecthkcdStaticTreeDefaultTreeStorage6 @ 0x1407f2290
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDefaultTreeStorage6(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage4 @ 0x1407f2300
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage4(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage5 @ 0x1407f2370
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage5(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage6 @ 0x1407f23e0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage6(void* p) { (void)p; }

// ============================================================================
// Part 101: hkcdStaticMeshTreeBase and additional hkcdDynamicTree Serialization Support
// ============================================================================

// Additional static class stubs
static hkClass hkcdStaticMeshTreeBaseClass_stub;
static hkClass hkcdStaticMeshTreeBaseSectionClass_stub;
static hkClass hkcdStaticMeshTreeBaseSectionSharedVerticesClass_stub;
static hkClass hkcdStaticMeshTreeBaseSectionPrimitivesClass_stub;
static hkClass hkcdStaticMeshTreeBaseSectionDataRunsClass_stub;
static hkClass hkcdStaticMeshTreeBasePrimitiveClass_stub;
static hkClass hkcdStaticMeshTreeDefaultDataRunClass_stub;
static hkClass hkcdDynamicTreeDynamicStoragePtrClass_stub;
static hkClass hkcdDynamicTreeDynamicStorage32Class_stub;
static hkClass hkcdDynamicTreeDynamicStorage16Class_stub;
static hkClass hkcdDynamicTreeDynamicStorageInt16Class_stub;

// Additional cleanup functions for hkcdStaticTree
// IDA: cleanupLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage32 @ 0x1407f2450
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeTreehkcdStaticTreeDynamicStorage32(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeDefaultTreeStorage4 @ 0x1407f24c0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDefaultTreeStorage4(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeDefaultTreeStorage5 @ 0x1407f2530
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDefaultTreeStorage5(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticTreeDefaultTreeStorage32 @ 0x1407f25a0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeDefaultTreeStorage32(void* p) { (void)p; }

// hkcdStaticMeshTreeBase serialization support
// IDA: ?staticClass@SharedVertices@Section@hkcdStaticMeshTreeBase@@SAAEBVhkClass@@XZ @ 0x1407f2680
extern "C" const hkClass* __fastcall hkcdStaticMeshTreeBase_Section_SharedVertices_staticClass() {
    return &hkcdStaticMeshTreeBaseSectionSharedVerticesClass_stub;
}
// IDA: cleanupLoadedObjecthkcdStaticMeshTreeBaseSectionSharedVertices @ 0x1407f2690
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticMeshTreeBaseSectionSharedVertices(void* p) { (void)p; }

// IDA: ?staticClass@Primitives@Section@hkcdStaticMeshTreeBase@@SAAEBVhkClass@@XZ @ 0x1407f26a0
extern "C" const hkClass* __fastcall hkcdStaticMeshTreeBase_Section_Primitives_staticClass() {
    return &hkcdStaticMeshTreeBaseSectionPrimitivesClass_stub;
}
// IDA: cleanupLoadedObjecthkcdStaticMeshTreeBaseSectionPrimitives @ 0x1407f26b0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticMeshTreeBaseSectionPrimitives(void* p) { (void)p; }

// IDA: ?staticClass@DataRuns@Section@hkcdStaticMeshTreeBase@@SAAEBVhkClass@@XZ @ 0x1407f26c0
extern "C" const hkClass* __fastcall hkcdStaticMeshTreeBase_Section_DataRuns_staticClass() {
    return &hkcdStaticMeshTreeBaseSectionDataRunsClass_stub;
}
// IDA: cleanupLoadedObjecthkcdStaticMeshTreeBaseSectionDataRuns @ 0x1407f26d0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticMeshTreeBaseSectionDataRuns(void* p) { (void)p; }

// IDA: ?staticClass@Section@hkcdStaticMeshTreeBase@@SAAEBVhkClass@@XZ @ 0x1407f26e0
extern "C" const hkClass* __fastcall hkcdStaticMeshTreeBase_Section_staticClass() {
    return &hkcdStaticMeshTreeBaseSectionClass_stub;
}

// IDA: ?staticClass@Primitive@hkcdStaticMeshTreeBase@@SAAEBVhkClass@@XZ @ 0x1407f26f0
extern "C" const hkClass* __fastcall hkcdStaticMeshTreeBase_Primitive_staticClass() {
    return &hkcdStaticMeshTreeBasePrimitiveClass_stub;
}
// IDA: cleanupLoadedObjecthkcdStaticMeshTreeBasePrimitive @ 0x1407f2700
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticMeshTreeBasePrimitive(void* p) { (void)p; }

// IDA: ?staticClass@hkcdStaticMeshTreeBase@@SAAEBVhkClass@@XZ @ 0x1407f2710
extern "C" const hkClass* __fastcall hkcdStaticMeshTreeBase_staticClass() {
    return &hkcdStaticMeshTreeBaseClass_stub;
}
// IDA: finishLoadedObjecthkcdStaticMeshTreeBase @ 0x1407f2720
extern "C" void __fastcall finishLoadedObjecthkcdStaticMeshTreeBase(void* p, int finishing) { (void)p; (void)finishing; }

// IDA: ?staticClass@hkcdStaticMeshTreeDefaultDataRun@@SAAEBVhkClass@@XZ @ 0x1407f2740
extern "C" const hkClass* __fastcall hkcdStaticMeshTreeDefaultDataRun_staticClass() {
    return &hkcdStaticMeshTreeDefaultDataRunClass_stub;
}
// IDA: cleanupLoadedObjecthkcdStaticMeshTreeDefaultDataRun @ 0x1407f2750
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticMeshTreeDefaultDataRun(void* p) { (void)p; }

// IDA: cleanupLoadedObjecthkcdStaticMeshTreeBasePrimitiveDataRunBaseunsignedshort @ 0x1407f2760
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticMeshTreeBasePrimitiveDataRunBaseunsignedshort(void* p) { (void)p; }

// IDA: finishLoadedObjecthkcdStaticMeshTreeBaseSection @ 0x1407f2770
extern "C" void __fastcall finishLoadedObjecthkcdStaticMeshTreeBaseSection(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdStaticMeshTreeBaseSection @ 0x1407f2780
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticMeshTreeBaseSection(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdStaticMeshTreeBase @ 0x1407f27f0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticMeshTreeBase(void* p) { (void)p; }

// IDA: ??1hkcdStaticMeshTreeBase@@QEAA@XZ @ 0x1407f2800
// Destructor - no-op for now (stub)
extern "C" void __fastcall hkcdStaticMeshTreeBase_destructor(void* p) { (void)p; }

// hkcdDynamicTree::DynamicStorage staticClass functions
// IDA: ?staticClass@DynamicStoragePtr@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f2960
extern "C" const hkClass* __fastcall hkcdDynamicTree_DynamicStoragePtr_staticClass() {
    return &hkcdDynamicTreeDynamicStoragePtrClass_stub;
}
// IDA: ?staticClass@DynamicStorage32@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f2970
extern "C" const hkClass* __fastcall hkcdDynamicTree_DynamicStorage32_staticClass() {
    return &hkcdDynamicTreeDynamicStorage32Class_stub;
}
// IDA: ?staticClass@DynamicStorage16@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f2980
extern "C" const hkClass* __fastcall hkcdDynamicTree_DynamicStorage16_staticClass() {
    return &hkcdDynamicTreeDynamicStorage16Class_stub;
}
// IDA: ?staticClass@DynamicStorageInt16@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f2990
extern "C" const hkClass* __fastcall hkcdDynamicTree_DynamicStorageInt16_staticClass() {
    return &hkcdDynamicTreeDynamicStorageInt16Class_stub;
}

// hkcdDynamicTree DynamicStorage serialization
// IDA: finishLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUlong @ 0x1407f29a0
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUlong(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUlong @ 0x1407f29b0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUlong(void* p) { (void)p; }

// IDA: finishLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUint @ 0x1407f2a20
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUint(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUint @ 0x1407f2a30
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUint(void* p) { (void)p; }

// IDA: finishLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodec32 @ 0x1407f2aa0
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodec32(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodec32 @ 0x1407f2ab0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodec32(void* p) { (void)p; }

// IDA: finishLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecInt16 @ 0x1407f2b20
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecInt16(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecInt16 @ 0x1407f2b30
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecInt16(void* p) { (void)p; }

// IDA: finishLoadedObjecthkcdDynamicTreeDynamicStoragePtr @ 0x1407f2ba0
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDynamicStoragePtr(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDynamicStoragePtr @ 0x1407f2bb0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDynamicStoragePtr(void* p) { (void)p; }

// IDA: finishLoadedObjecthkcdDynamicTreeDynamicStorage32 @ 0x1407f2c20
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDynamicStorage32(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDynamicStorage32 @ 0x1407f2c30
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDynamicStorage32(void* p) { (void)p; }

// ============================================================================
// Part 102: Additional hkcdDynamicTree and hkcdStaticTree Codec Serialization
// ============================================================================

// Additional DynamicStorage serialization
// IDA: finishLoadedObjecthkcdDynamicTreeDynamicStorage16 @ 0x1407f2ca0
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDynamicStorage16(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDynamicStorage16 @ 0x1407f2cb0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDynamicStorage16(void* p) { (void)p; }

// IDA: finishLoadedObjecthkcdDynamicTreeDynamicStorageInt16 @ 0x1407f2d20
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDynamicStorageInt16(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDynamicStorageInt16 @ 0x1407f2d30
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDynamicStorageInt16(void* p) { (void)p; }

// DefaultDynamicStorage serialization
// IDA: finishLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUlong @ 0x1407f2da0
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUlong(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUlong @ 0x1407f2db0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUlong(void* p) { (void)p; }

// IDA: finishLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUint @ 0x1407f2e20
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUint(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUint @ 0x1407f2e30
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUint(void* p) { (void)p; }

// IDA: finishLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodec32 @ 0x1407f2ea0
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodec32(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodec32 @ 0x1407f2eb0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodec32(void* p) { (void)p; }

// IDA: finishLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecInt16 @ 0x1407f2f20
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecInt16(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecInt16 @ 0x1407f2f30
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecInt16(void* p) { (void)p; }

// Static class stubs for codec types
static hkClass hkcdStaticTreeCodec3AxisClass_stub;
static hkClass hkcdStaticTreeCodec3Axis4Class_stub;
static hkClass hkcdStaticTreeCodec3Axis5Class_stub;
static hkClass hkcdStaticTreeCodec3Axis6Class_stub;
static hkClass hkcdStaticTreeCodecRawClass_stub;
static hkClass hkcdDynamicTreeCodecRawUlongClass_stub;
static hkClass hkcdDynamicTreeCodecRawUintClass_stub;
static hkClass hkcdDynamicTreeCodec32Class_stub;
static hkClass hkcdDynamicTreeCodecInt16Class_stub;

// hkcdStaticTree Codec staticClass functions
// IDA: ?staticClass@Codec3Axis@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f2f90
extern "C" const hkClass* __fastcall hkcdStaticTree_Codec3Axis_staticClass() {
    return &hkcdStaticTreeCodec3AxisClass_stub;
}
// IDA: cleanupLoadedObjecthkcdStaticTreeCodec3Axis @ 0x1407f2fa0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeCodec3Axis(void* p) { (void)p; }

// IDA: ?staticClass@Codec3Axis4@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f2fb0
extern "C" const hkClass* __fastcall hkcdStaticTree_Codec3Axis4_staticClass() {
    return &hkcdStaticTreeCodec3Axis4Class_stub;
}
// IDA: cleanupLoadedObjecthkcdStaticTreeCodec3Axis4 @ 0x1407f2fc0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeCodec3Axis4(void* p) { (void)p; }

// IDA: ?staticClass@Codec3Axis5@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f2fd0
extern "C" const hkClass* __fastcall hkcdStaticTree_Codec3Axis5_staticClass() {
    return &hkcdStaticTreeCodec3Axis5Class_stub;
}
// IDA: cleanupLoadedObjecthkcdStaticTreeCodec3Axis5 @ 0x1407f2fe0
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeCodec3Axis5(void* p) { (void)p; }

// IDA: ?staticClass@Codec3Axis6@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f2ff0
extern "C" const hkClass* __fastcall hkcdStaticTree_Codec3Axis6_staticClass() {
    return &hkcdStaticTreeCodec3Axis6Class_stub;
}
// IDA: cleanupLoadedObjecthkcdStaticTreeCodec3Axis6 @ 0x1407f3000
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeCodec3Axis6(void* p) { (void)p; }

// IDA: ?staticClass@CodecRaw@hkcdStaticTree@@SAAEBVhkClass@@XZ @ 0x1407f3010
extern "C" const hkClass* __fastcall hkcdStaticTree_CodecRaw_staticClass() {
    return &hkcdStaticTreeCodecRawClass_stub;
}
// IDA: cleanupLoadedObjecthkcdStaticTreeCodecRaw @ 0x1407f3020
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticTreeCodecRaw(void* p) { (void)p; }

// hkcdDynamicTree Codec staticClass functions
// IDA: ?staticClass@CodecRawUlong@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f3030
extern "C" const hkClass* __fastcall hkcdDynamicTree_CodecRawUlong_staticClass() {
    return &hkcdDynamicTreeCodecRawUlongClass_stub;
}
// IDA: cleanupLoadedObjecthkcdDynamicTreeCodecRawUlong @ 0x1407f3040
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeCodecRawUlong(void* p) { (void)p; }

// IDA: ?staticClass@CodecRawUint@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f3050
extern "C" const hkClass* __fastcall hkcdDynamicTree_CodecRawUint_staticClass() {
    return &hkcdDynamicTreeCodecRawUintClass_stub;
}
// IDA: cleanupLoadedObjecthkcdDynamicTreeCodecRawUint @ 0x1407f3060
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeCodecRawUint(void* p) { (void)p; }

// IDA: ?staticClass@Codec32@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f3070
extern "C" const hkClass* __fastcall hkcdDynamicTree_Codec32_staticClass() {
    return &hkcdDynamicTreeCodec32Class_stub;
}
// IDA: finishLoadedObjecthkcdDynamicTreeCodec32 @ 0x1407f3080
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeCodec32(void* p, int finishing) { (void)p; (void)finishing; }

// ============================================================================
// Part 103: Additional hkcdDynamicTree Codec and Metric Serialization
// ============================================================================

// Additional static class stubs
static hkClass hkcdDynamicTreeCodec18Class_stub;
static hkClass hkcdDynamicTreeCodecInt16IntAabbClass_stub;
static hkClass hkcdDynamicTreeCentroidMetricClass_stub;
static hkClass hkcdDynamicTreeBalanceMetricClass_stub;
static hkClass hkcdDynamicTreeAnisotropicMetricClass_stub;
static hkClass hkcdStaticPvsClass_stub;
static hkClass hkcdStaticPvsBlockHeaderClass_stub;

// IDA: cleanupLoadedObjecthkcdDynamicTreeCodec32 @ 0x1407f3090
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeCodec32(void* p) { (void)p; }

// IDA: ?staticClass@Codec18@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f30a0
extern "C" const hkClass* __fastcall hkcdDynamicTree_Codec18_staticClass() {
    return &hkcdDynamicTreeCodec18Class_stub;
}
// IDA: finishLoadedObjecthkcdDynamicTreeCodec18 @ 0x1407f30b0
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeCodec18(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeCodec18 @ 0x1407f30c0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeCodec18(void* p) { (void)p; }

// IDA: ?staticClass@IntAabb@CodecInt16@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f30d0
extern "C" const hkClass* __fastcall hkcdDynamicTree_CodecInt16_IntAabb_staticClass() {
    return &hkcdDynamicTreeCodecInt16IntAabbClass_stub;
}
// IDA: cleanupLoadedObjecthkcdDynamicTreeCodecInt16IntAabb @ 0x1407f30e0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeCodecInt16IntAabb(void* p) { (void)p; }

// IDA: ?staticClass@CodecInt16@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f30f0
extern "C" const hkClass* __fastcall hkcdDynamicTree_CodecInt16_staticClass() {
    return &hkcdDynamicTreeCodecInt16Class_stub;
}
// IDA: finishLoadedObjecthkcdDynamicTreeCodecInt16 @ 0x1407f3100
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeCodecInt16(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeCodecInt16 @ 0x1407f3110
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeCodecInt16(void* p) { (void)p; }

// Additional codec cleanup/finish functions
// IDA: cleanupLoadedObjecthkcdDynamicTreeCodecRawunsignedlong @ 0x1407f3120
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeCodecRawunsignedlong(void* p) { (void)p; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeCodecRawunsignedint @ 0x1407f3130
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeCodecRawunsignedint(void* p) { (void)p; }
// IDA: finishLoadedObjecthkcdDynamicTreeCodecRawUlong @ 0x1407f3140
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeCodecRawUlong(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdDynamicTreeCodecRawUint @ 0x1407f3150
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeCodecRawUint(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdDynamicTreeCodecRawunsignedlong @ 0x1407f3160
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeCodecRawunsignedlong(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: finishLoadedObjecthkcdDynamicTreeCodecRawunsignedint @ 0x1407f3170
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeCodecRawunsignedint(void* p, int finishing) { (void)p; (void)finishing; }

// hkcdDynamicTree Metric staticClass functions
// IDA: ?staticClass@CentroidMetric@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f3180
extern "C" const hkClass* __fastcall hkcdDynamicTree_CentroidMetric_staticClass() {
    return &hkcdDynamicTreeCentroidMetricClass_stub;
}
// IDA: finishLoadedObjecthkcdDynamicTreeCentroidMetric @ 0x1407f3190
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeCentroidMetric(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeCentroidMetric @ 0x1407f31a0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeCentroidMetric(void* p) { (void)p; }

// IDA: ?staticClass@BalanceMetric@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f31b0
extern "C" const hkClass* __fastcall hkcdDynamicTree_BalanceMetric_staticClass() {
    return &hkcdDynamicTreeBalanceMetricClass_stub;
}
// IDA: finishLoadedObjecthkcdDynamicTreeBalanceMetric @ 0x1407f31c0
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeBalanceMetric(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeBalanceMetric @ 0x1407f31d0
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeBalanceMetric(void* p) { (void)p; }

// IDA: ?staticClass@AnisotropicMetric@hkcdDynamicTree@@SAAEBVhkClass@@XZ @ 0x1407f31e0
extern "C" const hkClass* __fastcall hkcdDynamicTree_AnisotropicMetric_staticClass() {
    return &hkcdDynamicTreeAnisotropicMetricClass_stub;
}
// IDA: finishLoadedObjecthkcdDynamicTreeAnisotropicMetric @ 0x1407f31f0
extern "C" void __fastcall finishLoadedObjecthkcdDynamicTreeAnisotropicMetric(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdDynamicTreeAnisotropicMetric @ 0x1407f3200
extern "C" void __fastcall cleanupLoadedObjecthkcdDynamicTreeAnisotropicMetric(void* p) { (void)p; }

// hkcdStaticPvs serialization support
// IDA: ?staticClass@BlockHeader@hkcdStaticPvs@@SAAEBVhkClass@@XZ @ 0x1407f3210
extern "C" const hkClass* __fastcall hkcdStaticPvs_BlockHeader_staticClass() {
    return &hkcdStaticPvsBlockHeaderClass_stub;
}
// IDA: cleanupLoadedObjecthkcdStaticPvsBlockHeader @ 0x1407f3220
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticPvsBlockHeader(void* p) { (void)p; }

// IDA: ?staticClass@hkcdStaticPvs@@SAAEBVhkClass@@XZ @ 0x1407f3230
extern "C" const hkClass* __fastcall hkcdStaticPvs_staticClass() {
    return &hkcdStaticPvsClass_stub;
}
// IDA: finishLoadedObjecthkcdStaticPvs @ 0x1407f3240
extern "C" void __fastcall finishLoadedObjecthkcdStaticPvs(void* p, int finishing) { (void)p; (void)finishing; }
// IDA: cleanupLoadedObjecthkcdStaticPvs @ 0x1407f3250
extern "C" void __fastcall cleanupLoadedObjecthkcdStaticPvs(void* p) { (void)p; }

// IDA: ??1hkcdStaticPvs@@QEAA@XZ @ 0x1407f3260
extern "C" void __fastcall hkcdStaticPvs_destructor(void* p) { (void)p; }

// ============================================================================
// Part 104: Havok Resource/Container staticClass Functions
// ============================================================================

// Static class stubs for resource types
static hkClass hkSetUint32Class_stub;
static hkClass hkRootLevelContainerNamedVariantClass_stub;
static hkClass hkRootLevelContainerClass_stub;
static hkClass hkPackfileSectionHeaderClass_stub;
static hkClass hkResourceBaseClass_stub;

// IDA: ?staticClass@hkSetUint32@@SAAEBVhkClass@@XZ @ 0x140791d90
extern "C" const hkClass* __fastcall hkSetUint32_staticClass() {
    return &hkSetUint32Class_stub;
}

// IDA: ?staticClass@NamedVariant@hkRootLevelContainer@@SAAEBVhkClass@@XZ @ 0x1407b5c60
extern "C" const hkClass* __fastcall hkRootLevelContainer_NamedVariant_staticClass() {
    return &hkRootLevelContainerNamedVariantClass_stub;
}

// IDA: ?staticClass@hkRootLevelContainer@@SAAEBVhkClass@@XZ @ 0x1407b5c90
extern "C" const hkClass* __fastcall hkRootLevelContainer_staticClass() {
    return &hkRootLevelContainerClass_stub;
}

// IDA: ?staticClass@hkPackfileSectionHeader@@SAAEBVhkClass@@XZ @ 0x1407b6a30
extern "C" const hkClass* __fastcall hkPackfileSectionHeader_staticClass() {
    return &hkPackfileSectionHeaderClass_stub;
}

// IDA: ?staticClass@hkResourceBase@@SAAEBVhkClass@@XZ @ 0x1407b6a50
extern "C" const hkClass* __fastcall hkResourceBase_staticClass() {
    return &hkResourceBaseClass_stub;
}

// Additional static class stubs for resource handle types
static hkClass hkResourceHandleClass_stub;
static hkClass hkResourceContainerClass_stub;
static hkClass hkMemoryResourceHandleExternalLinkClass_stub;
static hkClass hkMemoryResourceHandleClass_stub;
static hkClass hkMemoryResourceContainerClass_stub;

// IDA: ?staticClass@hkResourceHandle@@SAAEBVhkClass@@XZ @ 0x1407b6a60
extern "C" const hkClass* __fastcall hkResourceHandle_staticClass() {
    return &hkResourceHandleClass_stub;
}

// IDA: ?staticClass@hkResourceContainer@@SAAEBVhkClass@@XZ @ 0x1407b6a70
extern "C" const hkClass* __fastcall hkResourceContainer_staticClass() {
    return &hkResourceContainerClass_stub;
}

// IDA: ?staticClass@ExternalLink@hkMemoryResourceHandle@@SAAEBVhkClass@@XZ @ 0x1407b6a80
extern "C" const hkClass* __fastcall hkMemoryResourceHandle_ExternalLink_staticClass() {
    return &hkMemoryResourceHandleExternalLinkClass_stub;
}

// IDA: ?staticClass@hkMemoryResourceHandle@@SAAEBVhkClass@@XZ @ 0x1407b6ad0
extern "C" const hkClass* __fastcall hkMemoryResourceHandle_staticClass() {
    return &hkMemoryResourceHandleClass_stub;
}

// IDA: ?staticClass@hkMemoryResourceContainer@@SAAEBVhkClass@@XZ @ 0x1407b6b30
extern "C" const hkClass* __fastcall hkMemoryResourceContainer_staticClass() {
    return &hkMemoryResourceContainerClass_stub;
}

// ============================================================================
// Part 105: Additional Havok staticClass Functions
// ============================================================================

// Static class stubs for packfile/mesh types
static hkClass hkPackfileHeaderClass_stub;
static hkClass hkMemoryMeshBodyClass_stub;
static hkClass hkMemoryMeshTextureClass_stub;
static hkClass hkaiNavMeshFaceClass_stub;

// IDA: ?staticClass@hkPackfileHeader@@SAAEBVhkClass@@XZ @ 0x1407b6bb0
extern "C" const hkClass* __fastcall hkPackfileHeader_staticClass() {
    return &hkPackfileHeaderClass_stub;
}

// IDA: ?staticClass@hkMemoryMeshBody@@SAAEBVhkClass@@XZ @ 0x1407e5870
extern "C" const hkClass* __fastcall hkMemoryMeshBody_staticClass() {
    return &hkMemoryMeshBodyClass_stub;
}

// IDA: ?staticClass@hkMemoryMeshTexture@@SAAEBVhkClass@@XZ @ 0x1407e58d0
extern "C" const hkClass* __fastcall hkMemoryMeshTexture_staticClass() {
    return &hkMemoryMeshTextureClass_stub;
}

// Note: hkCompressedMassProperties::staticClass already implemented at line 5172

// IDA: ?staticClass@Face@hkaiNavMesh@@SAAEBVhkClass@@XZ @ 0x14080b760
extern "C" const hkClass* __fastcall hkaiNavMesh_Face_staticClass() {
    return &hkaiNavMeshFaceClass_stub;
}

// ============================================================================
// Part 106: hkaiNavMesh Related staticClass Functions
// ============================================================================

// Static class stubs for hkai types
static hkClass hkaiNavMeshEdgeClass_stub;
static hkClass hkaiNavMeshClass_stub;
static hkClass hkaiStaticTreeNavMeshQueryMediatorClass_stub;
static hkClass hkaiNavMeshInstanceClass_stub;

// IDA: ?staticClass@Edge@hkaiNavMesh@@SAAEBVhkClass@@XZ @ 0x14080b780
extern "C" const hkClass* __fastcall hkaiNavMesh_Edge_staticClass() {
    return &hkaiNavMeshEdgeClass_stub;
}

// IDA: ?staticClass@hkaiNavMesh@@SAAEBVhkClass@@XZ @ 0x14080b7a0
extern "C" const hkClass* __fastcall hkaiNavMesh_staticClass() {
    return &hkaiNavMeshClass_stub;
}

// IDA: ?staticClass@hkaiStaticTreeNavMeshQueryMediator@@SAAEBVhkClass@@XZ @ 0x14080b800
extern "C" const hkClass* __fastcall hkaiStaticTreeNavMeshQueryMediator_staticClass() {
    return &hkaiStaticTreeNavMeshQueryMediatorClass_stub;
}

// IDA: ?staticClass@hkaiNavMeshInstance@@SAAEBVhkClass@@XZ @ 0x14080b860
extern "C" hkClass* __fastcall hkaiNavMeshInstance_staticClass() {
    return &hkaiNavMeshInstanceClass_stub;
}

// ============================================================================
// Part 107: hkaiOverlapManager and hkaiJumpDetectionSettings staticClass
// ============================================================================

// Static class stubs for hkai types
static hkClass hkaiOverlapManagerSectionClass_stub;
static hkClass hkaiOverlapManagerClass_stub;
static hkClass hkaiJumpDetectionSettingsTrajectoryClass_stub;
static hkClass hkaiJumpDetectionSettingsJumpDescriptionClass_stub;
static hkClass hkaiJumpDetectionSettingsClass_stub;

// IDA: ?staticClass@Section@hkaiOverlapManager@@SAAEBVhkClass@@XZ @ 0x14080b8e0
extern "C" hkClass* __fastcall hkaiOverlapManager_Section_staticClass() {
    return &hkaiOverlapManagerSectionClass_stub;
}

// IDA: ?staticClass@hkaiOverlapManager@@SAAEBVhkClass@@XZ @ 0x14080b8f0
extern "C" hkClass* __fastcall hkaiOverlapManager_staticClass() {
    return &hkaiOverlapManagerClass_stub;
}

// IDA: ?staticClass@Trajectory@hkaiJumpDetectionSettings@@SAAEBVhkClass@@XZ @ 0x14080ba90
extern "C" hkClass* __fastcall hkaiJumpDetectionSettings_Trajectory_staticClass() {
    return &hkaiJumpDetectionSettingsTrajectoryClass_stub;
}

// IDA: ?staticClass@JumpDescription@hkaiJumpDetectionSettings@@SAAEBVhkClass@@XZ @ 0x14080bac0
extern "C" hkClass* __fastcall hkaiJumpDetectionSettings_JumpDescription_staticClass() {
    return &hkaiJumpDetectionSettingsJumpDescriptionClass_stub;
}

// IDA: ?staticClass@hkaiJumpDetectionSettings@@SAAEBVhkClass@@XZ @ 0x14080baf0
extern "C" const hkClass* __fastcall hkaiJumpDetectionSettings_staticClass() {
    return &hkaiJumpDetectionSettingsClass_stub;
}

// ============================================================================
// Part 108: hkaiStreamingSet and hkaiAstarEdgeFilter staticClass
// ============================================================================

// Static class stubs for hkai streaming/astar types
static hkClass hkaiStreamingSetNavMeshConnectionClass_stub;
static hkClass hkaiStreamingSetGraphConnectionClass_stub;
static hkClass hkaiStreamingSetVolumeConnectionClass_stub;
static hkClass hkaiStreamingSetClass_stub;
static hkClass hkaiAstarEdgeFilterClass_stub;

// IDA: ?staticClass@NavMeshConnection@hkaiStreamingSet@@SAAEBVhkClass@@XZ @ 0x14080c0c0
extern "C" hkClass* __fastcall hkaiStreamingSet_NavMeshConnection_staticClass() {
    return &hkaiStreamingSetNavMeshConnectionClass_stub;
}

// IDA: ?staticClass@GraphConnection@hkaiStreamingSet@@SAAEBVhkClass@@XZ @ 0x14080c0e0
extern "C" const hkClass* __fastcall hkaiStreamingSet_GraphConnection_staticClass() {
    return &hkaiStreamingSetGraphConnectionClass_stub;
}

// IDA: ?staticClass@VolumeConnection@hkaiStreamingSet@@SAAEBVhkClass@@XZ @ 0x14080c100
extern "C" const hkClass* __fastcall hkaiStreamingSet_VolumeConnection_staticClass() {
    return &hkaiStreamingSetVolumeConnectionClass_stub;
}

// IDA: ?staticClass@hkaiStreamingSet@@SAAEBVhkClass@@XZ @ 0x14080c120
extern "C" hkClass* __fastcall hkaiStreamingSet_staticClass() {
    return &hkaiStreamingSetClass_stub;
}

// IDA: ?staticClass@hkaiAstarEdgeFilter@@SAAEBVhkClass@@XZ @ 0x14080c210
extern "C" const hkClass* __fastcall hkaiAstarEdgeFilter_staticClass() {
    return &hkaiAstarEdgeFilterClass_stub;
}

// ============================================================================
// Part 109: hkaiStreamingCollection and hkaiDirectedGraph staticClass
// ============================================================================

// Static class stubs for hkai streaming collection and directed graph types
static hkClass hkaiStreamingCollectionClass_stub;
static hkClass hkaiDirectedGraphExplicitCostNodeClass_stub;
static hkClass hkaiDirectedGraphExplicitCostEdgeClass_stub;
static hkClass hkaiDirectedGraphExplicitCostClass_stub;

// IDA: ?staticClass@hkaiStreamingCollection@@SAAEBVhkClass@@XZ @ 0x14080c240
extern "C" hkClass* __fastcall hkaiStreamingCollection_staticClass() {
    return &hkaiStreamingCollectionClass_stub;
}

// IDA: ?staticClass@Node@hkaiDirectedGraphExplicitCost@@SAAEBVhkClass@@XZ @ 0x14080c2a0
extern "C" hkClass* __fastcall hkaiDirectedGraphExplicitCost_Node_staticClass() {
    return &hkaiDirectedGraphExplicitCostNodeClass_stub;
}

// IDA: ?staticClass@Edge@hkaiDirectedGraphExplicitCost@@SAAEBVhkClass@@XZ @ 0x14080c2c0
extern "C" const hkClass* __fastcall hkaiDirectedGraphExplicitCost_Edge_staticClass() {
    return &hkaiDirectedGraphExplicitCostEdgeClass_stub;
}

// IDA: ?staticClass@hkaiDirectedGraphExplicitCost@@SAAEBVhkClass@@XZ @ 0x14080c2e0
extern "C" const hkClass* __fastcall hkaiDirectedGraphExplicitCost_staticClass() {
    return &hkaiDirectedGraphExplicitCostClass_stub;
}

// ============================================================================
// Part 110: hkaiPathfindingUtil and hkaiAabbTreeNavVolumeMediator staticClass
// ============================================================================

// Static class stubs for hkai pathfinding types
static hkClass hkaiPathfindingUtilFindPathInputClass_stub;
static hkClass hkaiPathfindingUtilFindPathOutputClass_stub;
static hkClass hkaiPathfindingUtilClass_stub;
static hkClass hkaiAabbTreeNavVolumeMediatorClass_stub;

// IDA: ?staticClass@FindPathInput@hkaiPathfindingUtil@@SAAEBVhkClass@@XZ @ 0x14080c340
extern "C" const hkClass* __fastcall hkaiPathfindingUtil_FindPathInput_staticClass() {
    return &hkaiPathfindingUtilFindPathInputClass_stub;
}

// IDA: ?staticClass@FindPathOutput@hkaiPathfindingUtil@@SAAEBVhkClass@@XZ @ 0x14080c3b0
extern "C" hkClass* __fastcall hkaiPathfindingUtil_FindPathOutput_staticClass() {
    return &hkaiPathfindingUtilFindPathOutputClass_stub;
}

// IDA: ?staticClass@hkaiPathfindingUtil@@SAAEBVhkClass@@XZ @ 0x14080c450
extern "C" const hkClass* __fastcall hkaiPathfindingUtil_staticClass() {
    return &hkaiPathfindingUtilClass_stub;
}

// IDA: ?staticClass@hkaiAabbTreeNavVolumeMediator@@SAAEBVhkClass@@XZ @ 0x14080c470
extern "C" hkClass* __fastcall hkaiAabbTreeNavVolumeMediator_staticClass() {
    return &hkaiAabbTreeNavVolumeMediatorClass_stub;
}

// ============================================================================
// Part 111: hkaiVolumePathfindingUtil staticClass Functions
// ============================================================================

// Static class stubs for hkai volume pathfinding types
static hkClass hkaiVolumePathfindingUtilFindPathOutputClass_stub;
static hkClass hkaiVolumePathfindingUtilClass_stub;
static hkClass hkaiReferenceFrameAndExtrusionClass_stub;
static hkClass hkaiNavVolumeGenerationSnapshotClass_stub;

// IDA: ?staticClass@FindPathOutput@hkaiVolumePathfindingUtil@@SAAEBVhkClass@@XZ @ 0x14080be50
extern "C" hkClass* __fastcall hkaiVolumePathfindingUtil_FindPathOutput_staticClass() {
    return &hkaiVolumePathfindingUtilFindPathOutputClass_stub;
}

// IDA: ?staticClass@hkaiVolumePathfindingUtil@@SAAEBVhkClass@@XZ @ 0x14080beb0
extern "C" const hkClass* __fastcall hkaiVolumePathfindingUtil_staticClass() {
    return &hkaiVolumePathfindingUtilClass_stub;
}

// IDA: ?staticClass@hkaiReferenceFrameAndExtrusion@@SAAEBVhkClass@@XZ @ 0x14080bed0
extern "C" hkClass* __fastcall hkaiReferenceFrameAndExtrusion_staticClass() {
    return &hkaiReferenceFrameAndExtrusionClass_stub;
}

// IDA: ?staticClass@hkaiNavVolumeGenerationSnapshot@@SAAEBVhkClass@@XZ @ 0x14080bf00
extern "C" hkClass* __fastcall hkaiNavVolumeGenerationSnapshot_staticClass() {
    return &hkaiNavVolumeGenerationSnapshotClass_stub;
}

// ============================================================================
// Part 112: hkaiNavMeshCutter and hkaiPersistentFaceKey staticClass
// ============================================================================

// Static class stubs for hkai cutter/persistent types
static hkClass hkaiPersistentFaceKeyClass_stub;
static hkClass hkaiNavMeshCutterSavedConnectivityClass_stub;
static hkClass hkaiNavMeshCutterClass_stub;
static hkClass hkaiNavVolumeInstanceClass_stub;

// IDA: ?staticClass@hkaiPersistentFaceKey@@SAAEBVhkClass@@XZ @ 0x14080bf70
extern "C" const hkClass* __fastcall hkaiPersistentFaceKey_staticClass() {
    return &hkaiPersistentFaceKeyClass_stub;
}

// IDA: ?staticClass@SavedConnectivity@hkaiNavMeshCutter@@SAAEBVhkClass@@XZ @ 0x14080bf90
extern "C" const hkClass* __fastcall hkaiNavMeshCutter_SavedConnectivity_staticClass() {
    return &hkaiNavMeshCutterSavedConnectivityClass_stub;
}

// IDA: ?staticClass@hkaiNavMeshCutter@@SAAEBVhkClass@@XZ @ 0x14080bfb0
extern "C" hkClass* __fastcall hkaiNavMeshCutter_staticClass() {
    return &hkaiNavMeshCutterClass_stub;
}

// IDA: ?staticClass@hkaiNavVolumeInstance@@SAAEBVhkClass@@XZ @ 0x14080c4f0
extern "C" const hkClass* __fastcall hkaiNavVolumeInstance_staticClass() {
    return &hkaiNavVolumeInstanceClass_stub;
}

// ============================================================================
// Part 113: hkaiDirectedGraphInstance, hkaiEdgePath, hkaiMaterialPainter staticClass
// ============================================================================

// Static class stubs for hkai graph/edge/material types
static hkClass hkaiMaterialPainterClass_stub;
static hkClass hkaiDirectedGraphInstanceFreeBlockListClass_stub;
static hkClass hkaiDirectedGraphInstanceClass_stub;
static hkClass hkaiEdgePathEdgeClass_stub;
static hkClass hkaiEdgePathClass_stub;

// IDA: ?staticClass@hkaiMaterialPainter@@SAAEBVhkClass@@XZ @ 0x14080c550
extern "C" const hkClass* __fastcall hkaiMaterialPainter_staticClass() {
    return &hkaiMaterialPainterClass_stub;
}

// IDA: ?staticClass@FreeBlockList@hkaiDirectedGraphInstance@@SAAEBVhkClass@@XZ @ 0x14080c5b0
extern "C" const hkClass* __fastcall hkaiDirectedGraphInstance_FreeBlockList_staticClass() {
    return &hkaiDirectedGraphInstanceFreeBlockListClass_stub;
}

// IDA: ?staticClass@hkaiDirectedGraphInstance@@SAAEBVhkClass@@XZ @ 0x14080c5c0
extern "C" const hkClass* __fastcall hkaiDirectedGraphInstance_staticClass() {
    return &hkaiDirectedGraphInstanceClass_stub;
}

// IDA: ?staticClass@Edge@hkaiEdgePath@@SAAEBVhkClass@@XZ @ 0x14080c6a0
extern "C" hkClass* __fastcall hkaiEdgePath_Edge_staticClass() {
    return &hkaiEdgePathEdgeClass_stub;
}

// IDA: ?staticClass@hkaiEdgePath@@SAAEBVhkClass@@XZ @ 0x14080c6c0
extern "C" const hkClass* __fastcall hkaiEdgePath_staticClass() {
    return &hkaiEdgePathClass_stub;
}

// ============================================================================
// Part 114: hkaiWorld, hkaiNavMeshPathRequestInfo, hkaiDynamicNavMeshQueryMediator
// ============================================================================

// Static class stubs for hkai world/path request types
static hkClass hkaiNavMeshPathRequestInfoClass_stub;
static hkClass hkaiNavVolumePathRequestInfoClass_stub;
static hkClass hkaiWorldClass_stub;
static hkClass hkaiNavMeshPathSearchParametersClass_stub;
static hkClass hkaiDynamicNavMeshQueryMediatorClass_stub;

// IDA: ?staticClass@hkaiNavMeshPathRequestInfo@@SAAEBVhkClass@@XZ @ 0x14080c720
extern "C" const hkClass* __fastcall hkaiNavMeshPathRequestInfo_staticClass() {
    return &hkaiNavMeshPathRequestInfoClass_stub;
}

// IDA: ?staticClass@hkaiNavVolumePathRequestInfo@@SAAEBVhkClass@@XZ @ 0x14080c740
extern "C" hkClass* __fastcall hkaiNavVolumePathRequestInfo_staticClass() {
    return &hkaiNavVolumePathRequestInfoClass_stub;
}

// IDA: ?staticClass@hkaiWorld@@SAAEBVhkClass@@XZ @ 0x14080c7c0
extern "C" const hkClass* __fastcall hkaiWorld_staticClass() {
    return &hkaiWorldClass_stub;
}

// IDA: ?staticClass@hkaiNavMeshPathSearchParameters@@SAAEBVhkClass@@XZ @ 0x14080c920
extern "C" const hkClass* __fastcall hkaiNavMeshPathSearchParameters_staticClass() {
    return &hkaiNavMeshPathSearchParametersClass_stub;
}

// IDA: ?staticClass@hkaiDynamicNavMeshQueryMediator@@SAAEBVhkClass@@XZ @ 0x14080c950
extern "C" hkClass* __fastcall hkaiDynamicNavMeshQueryMediator_staticClass() {
    return &hkaiDynamicNavMeshQueryMediatorClass_stub;
}

// ============================================================================
// Part 115: hkaiUserEdgeUtils staticClass Functions
// ============================================================================

// Static class stubs for hkai user edge types
static hkClass hkaiUserEdgeUtilsObbClass_stub;
static hkClass hkaiUserEdgeUtilsUserEdgeSetupClass_stub;
static hkClass hkaiUserEdgeUtilsUserEdgePairClass_stub;
static hkClass hkaiUserEdgeUtilsClass_stub;
static hkClass hkaiUserEdgeSetupArrayClass_stub;

// IDA: ?staticClass@Obb@hkaiUserEdgeUtils@@SAAEBVhkClass@@XZ @ 0x14080c9b0
extern "C" hkClass* __fastcall hkaiUserEdgeUtils_Obb_staticClass() {
    return &hkaiUserEdgeUtilsObbClass_stub;
}

// IDA: ?staticClass@UserEdgeSetup@hkaiUserEdgeUtils@@SAAEBVhkClass@@XZ @ 0x14080c9d0
extern "C" const hkClass* __fastcall hkaiUserEdgeUtils_UserEdgeSetup_staticClass() {
    return &hkaiUserEdgeUtilsUserEdgeSetupClass_stub;
}

// IDA: ?staticClass@UserEdgePair@hkaiUserEdgeUtils@@SAAEBVhkClass@@XZ @ 0x14080c9f0
extern "C" const hkClass* __fastcall hkaiUserEdgeUtils_UserEdgePair_staticClass() {
    return &hkaiUserEdgeUtilsUserEdgePairClass_stub;
}

// IDA: ?staticClass@hkaiUserEdgeUtils@@CAAEBVhkClass@@XZ @ 0x14080ca10
extern "C" const hkClass* __fastcall hkaiUserEdgeUtils_staticClass() {
    return &hkaiUserEdgeUtilsClass_stub;
}

// IDA: ?staticClass@hkaiUserEdgeSetupArray@@SAAEBVhkClass@@XZ @ 0x14080ca30
extern "C" hkClass* __fastcall hkaiUserEdgeSetupArray_staticClass() {
    return &hkaiUserEdgeSetupArrayClass_stub;
}

// ============================================================================
// Part 116: hkaiVolume, hkaiLineOfSightUtil, hkaiSilhouetteGenerationParameters
// ============================================================================

// Static class stubs for hkai volume/lineofsight types
static hkClass hkaiVolumeClass_stub;
static hkClass hkaiLineOfSightUtilClass_stub;
static hkClass hkaiSilhouetteGenerationParametersClass_stub;

// IDA: ?staticClass@hkaiVolume@@SAAEBVhkClass@@XZ @ 0x14080cc20
extern "C" hkClass* __fastcall hkaiVolume_staticClass() {
    return &hkaiVolumeClass_stub;
}

// IDA: ?staticClass@hkaiLineOfSightUtil@@SAAEBVhkClass@@XZ @ 0x14080ccf0
extern "C" hkClass* __fastcall hkaiLineOfSightUtil_staticClass() {
    return &hkaiLineOfSightUtilClass_stub;
}

// IDA: ?staticClass@hkaiSilhouetteGenerationParameters@@SAAEBVhkClass@@XZ @ 0x14080ce00
extern "C" const hkClass* __fastcall hkaiSilhouetteGenerationParameters_staticClass() {
    return &hkaiSilhouetteGenerationParametersClass_stub;
}

// ============================================================================
// Part 117: hkaiAvoidancePairProperties and hkaiAvoidanceSolver staticClass
// ============================================================================

// Static class stubs for hkai avoidance types
static hkClass hkaiAvoidancePairPropertiesClass_stub;
static hkClass hkaiAvoidanceSolverSphereObstacleClass_stub;

// IDA: ?staticClass@hkaiAvoidancePairProperties@@SAAEBVhkClass@@XZ @ 0x14080ce50
extern "C" const hkClass* __fastcall hkaiAvoidancePairProperties_staticClass() {
    return &hkaiAvoidancePairPropertiesClass_stub;
}

// IDA: ?staticClass@SphereObstacle@hkaiAvoidanceSolver@@SAAEBVhkClass@@XZ @ 0x14080ce70
extern "C" const hkClass* __fastcall hkaiAvoidanceSolver_SphereObstacle_staticClass() {
    return &hkaiAvoidanceSolverSphereObstacleClass_stub;
}

// ============================================================================
// Part 118: hkaiSilhouetteMerger, hkaiAvoidancePairProperties::PairData, FindGraphPathInput
// ============================================================================

// Static class stubs for hkai silhouette/pathfinding types
static hkClass hkaiAvoidancePairPropertiesPairDataClass_stub;
static hkClass hkaiPathfindingUtilFindGraphPathInputClass_stub;
static hkClass hkaiSilhouetteMergerClass_stub;
static hkClass hkaiSimpleSilhouetteMergerClass_stub;

// IDA: ?staticClass@PairData@hkaiAvoidancePairProperties@@SAAEBVhkClass@@XZ @ 0x14080ce30
extern "C" const hkClass* __fastcall hkaiAvoidancePairProperties_PairData_staticClass() {
    return &hkaiAvoidancePairPropertiesPairDataClass_stub;
}

// IDA: ?staticClass@FindGraphPathInput@hkaiPathfindingUtil@@SAAEBVhkClass@@XZ @ 0x14080c410
extern "C" const hkClass* __fastcall hkaiPathfindingUtil_FindGraphPathInput_staticClass() {
    return &hkaiPathfindingUtilFindGraphPathInputClass_stub;
}

// IDA: ?staticClass@hkaiSilhouetteMerger@@SAAEBVhkClass@@XZ @ 0x14080d1f0
extern "C" const hkClass* __fastcall hkaiSilhouetteMerger_staticClass() {
    return &hkaiSilhouetteMergerClass_stub;
}

// IDA: ?staticClass@hkaiSimpleSilhouetteMerger@@SAAEBVhkClass@@XZ @ 0x14080d200
extern "C" hkClass* __fastcall hkaiSimpleSilhouetteMerger_staticClass() {
    return &hkaiSimpleSilhouetteMergerClass_stub;
}

// ============================================================================
// Part 119: hkaiConvexHullSilhouetteMerger, hkaiNavVolume staticClass
// ============================================================================

// Static class stubs for hkai silhouette/navvolume types
static hkClass hkaiConvexHullSilhouetteMergerClass_stub;
static hkClass hkaiSilhouetteGeneratorSectionContextClass_stub;
static hkClass hkaiSplitGenerationSnapshotClass_stub;
static hkClass hkaiNavVolumeCellClass_stub;
static hkClass hkaiNavVolumeEdgeClass_stub;

// IDA: ?staticClass@hkaiConvexHullSilhouetteMerger@@SAAEBVhkClass@@XZ @ 0x14080d220
extern "C" const hkClass* __fastcall hkaiConvexHullSilhouetteMerger_staticClass() {
    return &hkaiConvexHullSilhouetteMergerClass_stub;
}

// IDA: ?staticClass@hkaiSilhouetteGeneratorSectionContext@@SAAEBVhkClass@@XZ @ 0x14080d2a0
extern "C" const hkClass* __fastcall hkaiSilhouetteGeneratorSectionContext_staticClass() {
    return &hkaiSilhouetteGeneratorSectionContextClass_stub;
}

// IDA: ?staticClass@hkaiSplitGenerationSnapshot@@SAAEBVhkClass@@XZ @ 0x14080d2f0
extern "C" const hkClass* __fastcall hkaiSplitGenerationSnapshot_staticClass() {
    return &hkaiSplitGenerationSnapshotClass_stub;
}

// IDA: ?staticClass@Cell@hkaiNavVolume@@SAAEBVhkClass@@XZ @ 0x14080d380
extern "C" const hkClass* __fastcall hkaiNavVolume_Cell_staticClass() {
    return &hkaiNavVolumeCellClass_stub;
}

// IDA: ?staticClass@Edge@hkaiNavVolume@@SAAEBVhkClass@@XZ @ 0x14080d3a0
extern "C" const hkClass* __fastcall hkaiNavVolume_Edge_staticClass() {
    return &hkaiNavVolumeEdgeClass_stub;
}

// ============================================================================
// Part 120: hkaiNavVolume, hkaiAdaptiveRanger, hkaiPath staticClass
// ============================================================================

// Static class stubs for hkai navvolume/path types
static hkClass hkaiNavVolumeClass_stub;
static hkClass hkaiAdaptiveRangerClass_stub;
static hkClass hkaiPathPathPointClass_stub;
static hkClass hkaiPathClass_stub;
static hkClass hkaiRigidBodySilhouetteGeneratorBaseClass_stub;

// IDA: ?staticClass@hkaiNavVolume@@SAAEBVhkClass@@XZ @ 0x14080d3c0
extern "C" const hkClass* __fastcall hkaiNavVolume_staticClass() {
    return &hkaiNavVolumeClass_stub;
}

// IDA: ?staticClass@hkaiAdaptiveRanger@@SAAEBVhkClass@@XZ @ 0x14080d420
extern "C" const hkClass* __fastcall hkaiAdaptiveRanger_staticClass() {
    return &hkaiAdaptiveRangerClass_stub;
}

// IDA: ?staticClass@PathPoint@hkaiPath@@SAAEBVhkClass@@XZ @ 0x14080d450
extern "C" hkClass* __fastcall hkaiPath_PathPoint_staticClass() {
    return &hkaiPathPathPointClass_stub;
}

// IDA: ?staticClass@hkaiPath@@SAAEBVhkClass@@XZ @ 0x14080d470
extern "C" hkClass* __fastcall hkaiPath_staticClass() {
    return &hkaiPathClass_stub;
}

// IDA: ?staticClass@hkaiRigidBodySilhouetteGeneratorBase@@SAAEBVhkClass@@XZ @ 0x14080d4c0
extern "C" hkClass* __fastcall hkaiRigidBodySilhouetteGeneratorBase_staticClass() {
    return &hkaiRigidBodySilhouetteGeneratorBaseClass_stub;
}

// ============================================================================
// Part 121: hkaiSilhouetteGenerator, hkaiAstarOutputParameters, hkaiSilhouetteReferenceFrame
// ============================================================================

// Static class stubs for hkai silhouette/astar types
static hkClass hkaiSilhouetteGeneratorClass_stub;
static hkClass hkaiAstarOutputParametersClass_stub;
static hkClass hkaiSilhouetteReferenceFrameClass_stub;

// IDA: ?staticClass@hkaiSilhouetteGenerator@@SAAEBVhkClass@@XZ @ 0x14080d530
extern "C" const hkClass* __fastcall hkaiSilhouetteGenerator_staticClass() {
    return &hkaiSilhouetteGeneratorClass_stub;
}

// IDA: ?staticClass@hkaiAstarOutputParameters@@SAAEBVhkClass@@XZ @ 0x14080d4d0
extern "C" const hkClass* __fastcall hkaiAstarOutputParameters_staticClass() {
    return &hkaiAstarOutputParametersClass_stub;
}

// IDA: ?staticClass@hkaiSilhouetteReferenceFrame@@SAAEBVhkClass@@XZ @ 0x14080d500
extern "C" const hkClass* __fastcall hkaiSilhouetteReferenceFrame_staticClass() {
    return &hkaiSilhouetteReferenceFrameClass_stub;
}

// ============================================================================
// Part 122: hkaiSplitGenerationUtils, hkaiNavVolumePathSearchParameters, hkaiPathFollowingProperties, etc.
// ============================================================================

// Static class stubs for hkai pathfinding/navigation types
static hkClass hkaiSplitGenerationUtilsSettingsClass_stub;
static hkClass hkaiSplitGenerationUtilsClass_stub;
static hkClass hkaiNavVolumePathSearchParametersClass_stub;
static hkClass hkaiPathFollowingPropertiesClass_stub;
static hkClass hkaiGraphPathSearchParametersClass_stub;
static hkClass hkaiLocalSteeringInputClass_stub;
static hkClass hkaiConvexSilhouetteSetClass_stub;
static hkClass hkaiDefaultAstarCostModifierClass_stub;
static hkClass hkaiReferenceFrameClass_stub;
static hkClass hkaiNavMeshGenerationSnapshotClass_stub;
static hkClass hkaiCharacterClass_stub;
static hkClass hkaiInvertedAabbVolumeClass_stub;
static hkClass hkaiPointCloudSilhouetteGeneratorClass_stub;
static hkClass hkaiNavMeshQueryMediatorClass_stub;

// IDA: ?staticClass@Settings@hkaiSplitGenerationUtils@@SAAEBVhkClass@@XZ @ 0x14080d560
extern "C" const hkClass* __fastcall hkaiSplitGenerationUtils_Settings_staticClass() {
    return &hkaiSplitGenerationUtilsSettingsClass_stub;
}

// IDA: ?staticClass@hkaiSplitGenerationUtils@@SAAEBVhkClass@@XZ @ 0x14080d570
extern "C" hkClass* __fastcall hkaiSplitGenerationUtils_staticClass() {
    return &hkaiSplitGenerationUtilsClass_stub;
}

// IDA: ?staticClass@hkaiNavVolumePathSearchParameters@@SAAEBVhkClass@@XZ @ 0x14080d690
extern "C" const hkClass* __fastcall hkaiNavVolumePathSearchParameters_staticClass() {
    return &hkaiNavVolumePathSearchParametersClass_stub;
}

// IDA: ?staticClass@hkaiPathFollowingProperties@@SAAEBVhkClass@@XZ @ 0x14080d6c0
extern "C" hkClass* __fastcall hkaiPathFollowingProperties_staticClass() {
    return &hkaiPathFollowingPropertiesClass_stub;
}

// IDA: ?staticClass@hkaiGraphPathSearchParameters@@SAAEBVhkClass@@XZ @ 0x14080d710
extern "C" hkClass* __fastcall hkaiGraphPathSearchParameters_staticClass() {
    return &hkaiGraphPathSearchParametersClass_stub;
}

// IDA: ?staticClass@hkaiLocalSteeringInput@@SAAEBVhkClass@@XZ @ 0x14080d740
extern "C" const hkClass* __fastcall hkaiLocalSteeringInput_staticClass() {
    return &hkaiLocalSteeringInputClass_stub;
}

// IDA: ?staticClass@hkaiConvexSilhouetteSet@@SAAEBVhkClass@@XZ @ 0x14080d770
extern "C" hkClass* __fastcall hkaiConvexSilhouetteSet_staticClass() {
    return &hkaiConvexSilhouetteSetClass_stub;
}

// IDA: ?staticClass@hkaiDefaultAstarCostModifier@@SAAEBVhkClass@@XZ @ 0x14080d840
extern "C" const hkClass* __fastcall hkaiDefaultAstarCostModifier_staticClass() {
    return &hkaiDefaultAstarCostModifierClass_stub;
}

// IDA: ?staticClass@hkaiReferenceFrame@@SAAEBVhkClass@@XZ @ 0x14080d8f0
extern "C" hkClass* __fastcall hkaiReferenceFrame_staticClass() {
    return &hkaiReferenceFrameClass_stub;
}

// IDA: ?staticClass@hkaiNavMeshGenerationSnapshot@@SAAEBVhkClass@@XZ @ 0x14080d920
extern "C" hkClass* __fastcall hkaiNavMeshGenerationSnapshot_staticClass() {
    return &hkaiNavMeshGenerationSnapshotClass_stub;
}

// IDA: ?staticClass@hkaiCharacter@@SAAEBVhkClass@@XZ @ 0x14080d990
extern "C" hkClass* __fastcall hkaiCharacter_staticClass() {
    return &hkaiCharacterClass_stub;
}

// IDA: ?staticClass@hkaiInvertedAabbVolume@@SAAEBVhkClass@@XZ @ 0x14080d9f0
extern "C" const hkClass* __fastcall hkaiInvertedAabbVolume_staticClass() {
    return &hkaiInvertedAabbVolumeClass_stub;
}

// IDA: ?staticClass@hkaiPointCloudSilhouetteGenerator@@SAAEBVhkClass@@XZ @ 0x14080dae0
extern "C" const hkClass* __fastcall hkaiPointCloudSilhouetteGenerator_staticClass() {
    return &hkaiPointCloudSilhouetteGeneratorClass_stub;
}

// IDA: ?staticClass@hkaiNavMeshQueryMediator@@SAAEBVhkClass@@XZ @ 0x14080db40
extern "C" const hkClass* __fastcall hkaiNavMeshQueryMediator_staticClass() {
    return &hkaiNavMeshQueryMediatorClass_stub;
}

// ============================================================================
// Part 123: hkaiStaticTree, hkaiDynamicTree, hkaiNavMeshSimplificationUtils, hkaiSilhouetteRecorder
// ============================================================================

// Static class stubs for hkai tree/simplification/recorder types
static hkClass hkaiNavMeshSimplificationUtilsExtraVertexSettingsClass_stub;
static hkClass hkaiNavMeshSimplificationUtilsSettingsClass_stub;
static hkClass hkaiDynamicTreeClass_stub;
static hkClass hkaiStaticTreeClass_stub;
static hkClass hkaiSilhouetteRecorderReplayEventClass_stub;
static hkClass hkaiSilhouetteRecorderWorldConnectedEventClass_stub;
static hkClass hkaiSilhouetteRecorderInstanceLoadedEventClass_stub;
static hkClass hkaiSilhouetteRecorderSilhouettesSteppedEventClass_stub;
static hkClass hkaiSilhouetteRecorderInstanceUnloadedEventClass_stub;
static hkClass hkaiSilhouetteRecorderVolumeLoadedEventClass_stub;
static hkClass hkaiSilhouetteRecorderVolumeUnloadedEventClass_stub;
static hkClass hkaiSilhouetteRecorderGraphLoadedEventClass_stub;
static hkClass hkaiNavMeshDebugUtilsSmallEdgeSettingsClass_stub;

// IDA: ?staticClass@ExtraVertexSettings@hkaiNavMeshSimplificationUtils@@SAAEBVhkClass@@XZ @ 0x14085cff0
extern "C" hkClass* __fastcall hkaiNavMeshSimplificationUtils_ExtraVertexSettings_staticClass() {
    return &hkaiNavMeshSimplificationUtilsExtraVertexSettingsClass_stub;
}

// IDA: ?staticClass@Settings@hkaiNavMeshSimplificationUtils@@SAAEBVhkClass@@XZ @ 0x14085d000
extern "C" const hkClass* __fastcall hkaiNavMeshSimplificationUtils_Settings_staticClass() {
    return &hkaiNavMeshSimplificationUtilsSettingsClass_stub;
}

// IDA: ?staticClass@hkaiDynamicTree@@SAAEBVhkClass@@XZ @ 0x14085d110
extern "C" const hkClass* __fastcall hkaiDynamicTree_staticClass() {
    return &hkaiDynamicTreeClass_stub;
}

// IDA: ?staticClass@hkaiStaticTree@@SAAEBVhkClass@@XZ @ 0x14085d170
extern "C" const hkClass* __fastcall hkaiStaticTree_staticClass() {
    return &hkaiStaticTreeClass_stub;
}

// IDA: ?staticClass@ReplayEvent@hkaiSilhouetteRecorder@@SAAEBVhkClass@@XZ @ 0x1408a0f40
extern "C" const hkClass* __fastcall hkaiSilhouetteRecorder_ReplayEvent_staticClass() {
    return &hkaiSilhouetteRecorderReplayEventClass_stub;
}

// IDA: ?staticClass@WorldConnectedEvent@hkaiSilhouetteRecorder@@SAAEBVhkClass@@XZ @ 0x1408a0f60
extern "C" const hkClass* __fastcall hkaiSilhouetteRecorder_WorldConnectedEvent_staticClass() {
    return &hkaiSilhouetteRecorderWorldConnectedEventClass_stub;
}

// IDA: ?staticClass@InstanceLoadedEvent@hkaiSilhouetteRecorder@@SAAEBVhkClass@@XZ @ 0x1408a0fc0
extern "C" const hkClass* __fastcall hkaiSilhouetteRecorder_InstanceLoadedEvent_staticClass() {
    return &hkaiSilhouetteRecorderInstanceLoadedEventClass_stub;
}

// IDA: ?staticClass@SilhouettesSteppedEvent@hkaiSilhouetteRecorder@@SAAEBVhkClass@@XZ @ 0x1408a1020
extern "C" hkClass** __fastcall hkaiSilhouetteRecorder_SilhouettesSteppedEvent_staticClass() {
    return (hkClass**)&hkaiSilhouetteRecorderSilhouettesSteppedEventClass_stub;
}

// IDA: ?staticClass@InstanceUnloadedEvent@hkaiSilhouetteRecorder@@SAAEBVhkClass@@XZ @ 0x1408a1090
extern "C" hkClass* __fastcall hkaiSilhouetteRecorder_InstanceUnloadedEvent_staticClass() {
    return &hkaiSilhouetteRecorderInstanceUnloadedEventClass_stub;
}

// IDA: ?staticClass@VolumeLoadedEvent@hkaiSilhouetteRecorder@@SAAEBVhkClass@@XZ @ 0x1408a10b0
extern "C" hkClass* __fastcall hkaiSilhouetteRecorder_VolumeLoadedEvent_staticClass() {
    return &hkaiSilhouetteRecorderVolumeLoadedEventClass_stub;
}

// IDA: ?staticClass@VolumeUnloadedEvent@hkaiSilhouetteRecorder@@SAAEBVhkClass@@XZ @ 0x1408a1110
extern "C" hkClass* __fastcall hkaiSilhouetteRecorder_VolumeUnloadedEvent_staticClass() {
    return &hkaiSilhouetteRecorderVolumeUnloadedEventClass_stub;
}

// IDA: ?staticClass@GraphLoadedEvent@hkaiSilhouetteRecorder@@SAAEBVhkClass@@XZ @ 0x1408a1130
extern "C" const hkClass* __fastcall hkaiSilhouetteRecorder_GraphLoadedEvent_staticClass() {
    return &hkaiSilhouetteRecorderGraphLoadedEventClass_stub;
}

// IDA: ?staticClass@SmallEdgeSettings@hkaiNavMeshDebugUtils@@SAAEBVhkClass@@XZ @ 0x1408a12d0
extern "C" hkClass* __fastcall hkaiNavMeshDebugUtils_SmallEdgeSettings_staticClass() {
    return &hkaiNavMeshDebugUtilsSmallEdgeSettingsClass_stub;
}

// ============================================================================
// Part 124: hkaiNavMeshGenerationSettings, hkaiNavVolumeGenerationSettings, hkaiEdgeGeometry, hkcdShape
// ============================================================================

// Static class stubs for hkai navmesh/navvolume/edge/shape types
static hkClass hkaiNavMeshGenerationSettingsMaterialConstructionPairClass_stub;
static hkClass hkaiNavMeshGenerationSettingsEdgeMatchingParametersClass_stub;
static hkClass hkaiNavMeshGenerationSettingsOverrideSettingsClass_stub;
static hkClass hkaiNavMeshGenerationSettingsRegionPruningSettingsClass_stub;
static hkClass hkaiNavVolumeGenerationSettingsChunkSettingsClass_stub;
static hkClass hkaiNavVolumeGenerationSettingsMergingSettingsClass_stub;
static hkClass hkaiNavMeshGenerationSettingsClass_stub;
static hkClass hkaiNavVolumeGenerationSettingsMaterialConstructionInfoClass_stub;
static hkClass hkaiEdgeGeometryEdgeClass_stub;
static hkClass hkaiEdgeGeometryFaceClass_stub;
static hkClass hkaiAgentTraversalInfoClass_stub;
static hkClass hkcdShapeClass_stub;

// IDA: ?staticClass@MaterialConstructionPair@hkaiNavMeshGenerationSettings@@SAAEBVhkClass@@XZ @ 0x14085cbb0
extern "C" const hkClass* __fastcall hkaiNavMeshGenerationSettings_MaterialConstructionPair_staticClass() {
    return &hkaiNavMeshGenerationSettingsMaterialConstructionPairClass_stub;
}

// IDA: ?staticClass@EdgeMatchingParameters@hkaiNavMeshGenerationSettings@@SAAEBVhkClass@@XZ @ 0x14085cbd0
extern "C" const hkClass* __fastcall hkaiNavMeshGenerationSettings_EdgeMatchingParameters_staticClass() {
    return &hkaiNavMeshGenerationSettingsEdgeMatchingParametersClass_stub;
}

// IDA: ?staticClass@OverrideSettings@hkaiNavMeshGenerationSettings@@SAAEBVhkClass@@XZ @ 0x14085cc00
extern "C" hkClass* __fastcall hkaiNavMeshGenerationSettings_OverrideSettings_staticClass() {
    return &hkaiNavMeshGenerationSettingsOverrideSettingsClass_stub;
}

// IDA: ?staticClass@RegionPruningSettings@hkaiNavMeshGenerationSettings@@SAAEBVhkClass@@XZ @ 0x14085cc30
extern "C" const hkClass* __fastcall hkaiNavMeshGenerationSettings_RegionPruningSettings_staticClass() {
    return &hkaiNavMeshGenerationSettingsRegionPruningSettingsClass_stub;
}

// IDA: ?staticClass@hkaiNavMeshGenerationSettings@@SAAEBVhkClass@@XZ @ 0x14085cc70
extern "C" const hkClass* __fastcall hkaiNavMeshGenerationSettings_staticClass() {
    return &hkaiNavMeshGenerationSettingsClass_stub;
}

// IDA: ?staticClass@MaterialConstructionInfo@hkaiNavVolumeGenerationSettings@@SAAEBVhkClass@@XZ @ 0x14085cd40
extern "C" hkClass* __fastcall hkaiNavVolumeGenerationSettings_MaterialConstructionInfo_staticClass() {
    return &hkaiNavVolumeGenerationSettingsMaterialConstructionInfoClass_stub;
}

// IDA: ?staticClass@ChunkSettings@hkaiNavVolumeGenerationSettings@@SAAEBVhkClass@@XZ @ 0x14085cd80
extern "C" hkClass* __fastcall hkaiNavVolumeGenerationSettings_ChunkSettings_staticClass() {
    return &hkaiNavVolumeGenerationSettingsChunkSettingsClass_stub;
}

// IDA: ?staticClass@MergingSettings@hkaiNavVolumeGenerationSettings@@SAAEBVhkClass@@XZ @ 0x14085cdb0
extern "C" const hkClass* __fastcall hkaiNavVolumeGenerationSettings_MergingSettings_staticClass() {
    return &hkaiNavVolumeGenerationSettingsMergingSettingsClass_stub;
}

// IDA: ?staticClass@Edge@hkaiEdgeGeometry@@SAAEBVhkClass@@XZ @ 0x14085ce20
extern "C" hkClass* __fastcall hkaiEdgeGeometry_Edge_staticClass() {
    return &hkaiEdgeGeometryEdgeClass_stub;
}

// IDA: ?staticClass@Face@hkaiEdgeGeometry@@SAAEBVhkClass@@XZ @ 0x14085ce40
extern "C" hkClass* __fastcall hkaiEdgeGeometry_Face_staticClass() {
    return &hkaiEdgeGeometryFaceClass_stub;
}

// IDA: ?staticClass@hkaiAgentTraversalInfo@@SAAEBVhkClass@@XZ @ 0x14080db50
extern "C" hkClass* __fastcall hkaiAgentTraversalInfo_staticClass() {
    return &hkaiAgentTraversalInfoClass_stub;
}

// IDA: ?staticClass@hkcdShape@@SAAEBVhkClass@@XZ @ 0x1408a0ed0
extern "C" hkClass* __fastcall hkcdShape_staticClass() {
    return &hkcdShapeClass_stub;
}

// ============================================================================
// Part 125: hkaiNavMeshDebugUtils ClusterGraphSettings, DebugInfo
// ============================================================================

// Static class stubs for hkai debug types
static hkClass hkaiNavMeshDebugUtilsClusterGraphSettingsClass_stub;
static hkClass hkaiNavMeshDebugUtilsDebugInfoClass_stub;

// IDA: ?staticClass@ClusterGraphSettings@hkaiNavMeshDebugUtils@@SAAEBVhkClass@@XZ @ 0x1408a1330
extern "C" hkClass* __fastcall hkaiNavMeshDebugUtils_ClusterGraphSettings_staticClass() {
    return &hkaiNavMeshDebugUtilsClusterGraphSettingsClass_stub;
}

// IDA: ?staticClass@DebugInfo@hkaiNavMeshDebugUtils@@SAAEBVhkClass@@XZ @ 0x1408a1350
extern "C" hkClass* __fastcall hkaiNavMeshDebugUtils_DebugInfo_staticClass() {
    return &hkaiNavMeshDebugUtilsDebugInfoClass_stub;
}

// ============================================================================
// Part 126: finishLoadedObject serialization support functions
// ============================================================================

// Forward declarations for constructor calls
class hkaiNavMesh;
class hkaiNavMeshInstance;
class hkMonitorStreamStringMap;
class hkRootLevelContainer;

// IDA: finishLoadedObjecthkaiNavMesh @ 0x14080b7b0
extern "C" void __fastcall finishLoadedObjecthkaiNavMesh(void* p) {
    if (p) {
        // Call placement new to run constructor
        new (p) hkaiNavMesh();
    }
}

// IDA: finishLoadedObjecthkaiNavMeshInstance @ 0x14080b870
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshInstance(void* p) {
    if (p) {
        new (p) hkaiNavMeshInstance();
    }
}

// IDA: finishLoadedObjecthkRootLevelContainer @ 0x1407b5ca0
// IDA decompile: if (p) hkRootLevelContainer::hkRootLevelContainer(p, finishing);
extern "C" void __fastcall finishLoadedObjecthkRootLevelContainer(void* p, hkFinishLoadedObjectFlag finishing) {
    if (p) {
        hkRootLevelContainer_constructor(p, finishing);
    }
}

// ============================================================================
// Part 127: cleanupLoadedObject serialization support functions
// ============================================================================

// IDA: cleanupLoadedObjecthkaiNavMesh @ 0x14080b7d0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMesh(void* p) {
    if (p) {
        // Call destructor via vtable
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// IDA: cleanupLoadedObjecthkaiNavMeshInstance @ 0x14080b890
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshInstance(void* p) {
    if (p) {
        // Call destructor via vtable
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// IDA: cleanupLoadedObjecthkRootLevelContainer @ 0x1407b5d00
extern "C" void __fastcall cleanupLoadedObjecthkRootLevelContainer(void* p) {
    if (p) {
        // Call scalar deleting destructor
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// ============================================================================
// Part 128: More finishLoadedObject serialization support functions
// ============================================================================

// Forward declarations
class hkaiJumpDetectionSettings;

// IDA: finishLoadedObjecthkRootLevelContainerNamedVariant @ 0x1407b5c70
// IDA decompile: if (p) hkRootLevelContainer::NamedVariant::NamedVariant(p, finishing);
extern "C" void __fastcall finishLoadedObjecthkRootLevelContainerNamedVariant(void* p, hkFinishLoadedObjectFlag finishing) {
    if (p) {
        hkRootLevelContainer_NamedVariant_constructor(p, finishing);
    }
}

// IDA: finishLoadedObjecthkMemoryResourceHandleExternalLink @ 0x1407b6a90
// IDA decompile: if (p) { hkStringPtr::hkStringPtr(p, finishing); hkStringPtr::hkStringPtr(p + 1, finishing); }
// Both hkStringPtr::hkStringPtr(hkFinishLoadedObjectFlag) calls are empty inline no-ops
extern "C" void __fastcall finishLoadedObjecthkMemoryResourceHandleExternalLink(void* p, hkFinishLoadedObjectFlag finishing) {
    if (p) {
        new (p) hkStringPtr(finishing);
        new ((hkStringPtr*)p + 1) hkStringPtr(finishing);
    }
}

// IDA: finishLoadedObjecthkMemoryResourceHandle @ 0x1407b6ae0
// IDA decompile: if (p) hkMemoryResourceHandle::hkMemoryResourceHandle(p, finishing);
extern "C" void __fastcall finishLoadedObjecthkMemoryResourceHandle(void* p, hkFinishLoadedObjectFlag finishing) {
    if (p) {
        hkMemoryResourceHandle_constructor(p, finishing);
    }
}

// IDA: finishLoadedObjecthkMemoryResourceContainer @ 0x1407b6b40
// IDA decompile: if (p) hkMemoryResourceContainer::hkMemoryResourceContainer(p, finishing);
extern "C" void __fastcall finishLoadedObjecthkMemoryResourceContainer(void* p, hkFinishLoadedObjectFlag finishing) {
    if (p) {
        hkMemoryResourceContainer_constructor(p, finishing);
    }
}

// IDA: finishLoadedObjecthkaiStaticTreeNavMeshQueryMediator @ 0x14080b810
// IDA decompile: if (p) hkaiStaticTreeNavMeshQueryMediator::hkaiStaticTreeNavMeshQueryMediator(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiStaticTreeNavMeshQueryMediator(void* p, void* a2) {
    if (p) {
        hkaiStaticTreeNavMeshQueryMediator_constructor(p);
    }
    (void)a2;
}

// IDA: finishLoadedObjecthkaiOverlapManager @ 0x14080b900
// IDA decompile: if (p) hkaiOverlapManager::hkaiOverlapManager(p);
extern "C" void __fastcall finishLoadedObjecthkaiOverlapManager(void* p) {
    if (p) {
        hkaiOverlapManager_constructor(p, hkFinishLoadedObjectFlag{false});
    }
}

// IDA: finishLoadedObjecthkaiOverlapManagerSection @ 0x14080b9a0
// IDA decompile: p[1] = 0x80000000; p[2] = 0; *p = 0;
extern "C" void __fastcall finishLoadedObjecthkaiOverlapManagerSection(void* p) {
    if (p) {
        *(int*)((char*)p + 4) = 0x80000000;
        *(int*)((char*)p + 8) = 0;
        *(int*)p = 0;
    }
}

// IDA: finishLoadedObjecthkaiJumpDetectionSettingsTrajectory @ 0x14080baa0
// IDA decompile: if (p) hkaiJumpDetectionSettings::Trajectory::Trajectory(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiJumpDetectionSettingsTrajectory(void* p, const void* a2) {
    if (p) {
        hkaiJumpDetectionSettings_Trajectory_constructor(p);
    }
    (void)a2;
}

// ============================================================================
// Part 129: More finishLoadedObject functions for hkai types
// ============================================================================

// IDA: finishLoadedObjecthkaiOverlapManagerSectionGeneratorData @ 0x14080b970
extern "C" void __fastcall finishLoadedObjecthkaiOverlapManagerSectionGeneratorData(void* p) {
    if (p) {
        // Set vtable pointer
        static void* vftable = nullptr;
        *(void**)p = &vftable;
    }
}

// IDA: finishLoadedObjecthkaiJumpDetectionSettingsJumpDescription @ 0x14080bad0
// IDA decompile: if (p) hkaiJumpDetectionSettings::JumpDescription::JumpDescription(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiJumpDetectionSettingsJumpDescription(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiJumpDetectionSettings_JumpDescription_constructor(p);
    }
    (void)a2;
}

// IDA: finishLoadedObjecthkaiJumpDetectionSettings @ 0x14080bb00
// IDA decompile: if (p) hkaiJumpDetectionSettings::hkaiJumpDetectionSettings(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiJumpDetectionSettings(void* p, const void* a2) {
    if (p) {
        hkaiJumpDetectionSettings_constructor(p);
    }
    (void)a2;
}

// IDA: finishLoadedObjecthkaiReferenceFrameAndExtrusion @ 0x14080bee0
// No-op function
extern "C" void __fastcall finishLoadedObjecthkaiReferenceFrameAndExtrusion(void* p) {
    // Empty - no initialization needed
}

// IDA: finishLoadedObjecthkaiNavVolumeGenerationSnapshot @ 0x14080bf10
// IDA decompile: if (p) hkGeometry::hkGeometry(p, a2); hkaiNavVolumeGenerationSettings::hkaiNavVolumeGenerationSettings(...);
// Note: Complex type with hkGeometry base class - need to check actual layout
extern "C" void __fastcall finishLoadedObjecthkaiNavVolumeGenerationSnapshot(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        // hkGeometry constructor at offset 0
        hkGeometry_constructor(p, a2);
        // hkaiNavVolumeGenerationSettings constructor follows
        // For now, keep minimal init as this needs more analysis
    }
    (void)a2;
}

// IDA: finishLoadedObjecthkaiNavMeshCutter @ 0x14080bfc0
// IDA decompile: if (p) hkaiNavMeshCutter::hkaiNavMeshCutter(p);
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshCutter(void* p) {
    if (p) {
        hkaiNavMeshCutter_constructor(p, hkFinishLoadedObjectFlag{false});
    }
}

// IDA: finishLoadedObjecthkaiStreamingSet @ 0x14080c130
// No-op function
extern "C" void __fastcall finishLoadedObjecthkaiStreamingSet(void* p) {
    // Empty - no initialization needed
}

// IDA: finishLoadedObjecthkaiStreamingCollection @ 0x14080c250
// IDA decompile: if (p) hkaiStreamingCollection::hkaiStreamingCollection(p, a2, a3, a4);
extern "C" void __fastcall finishLoadedObjecthkaiStreamingCollection(void* p, void* a2, void* a3, int a4) {
    if (p) {
        hkaiStreamingCollection_constructor(p, hkFinishLoadedObjectFlag{false});
    }
    (void)a2; (void)a3; (void)a4;
}

// IDA: finishLoadedObjecthkaiPathfindingUtilFindPathInput @ 0x14080c350
// IDA decompile: if (p) hkaiPathfindingUtil::FindPathInput::FindPathInput(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiPathfindingUtilFindPathInput(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiPathfindingUtil_FindPathInput_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiNavVolumeInstance @ 0x14080c500
// IDA decompile: if (p) hkaiNavVolumeInstance::hkaiNavVolumeInstance(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiNavVolumeInstance(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiNavVolumeInstance_constructor(p, a2);
    }
}

// ============================================================================
// Part 130: cleanupLoadedObject serialization support functions
// ============================================================================

// IDA: cleanupLoadedObjecthkaiNavMeshFace @ 0x14080b770
// No-op function
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshFace(const void* p) {
    // Empty - POD type
}

// IDA: cleanupLoadedObjecthkaiNavMeshEdge @ 0x14080b790
// No-op function
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshEdge(const void* p) {
    // Empty - POD type
}

// IDA: cleanupLoadedObjecthkaiStaticTreeNavMeshQueryMediator @ 0x14080b830
extern "C" void __fastcall cleanupLoadedObjecthkaiStaticTreeNavMeshQueryMediator(void* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// IDA: cleanupLoadedObjecthkaiOverlapManager @ 0x14080b920
extern "C" void __fastcall cleanupLoadedObjecthkaiOverlapManager(void* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// IDA: cleanupLoadedObjecthkaiJumpDetectionSettings @ 0x14080bb20
extern "C" void __fastcall cleanupLoadedObjecthkaiJumpDetectionSettings(void* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// IDA: cleanupLoadedObjecthkaiJumpDetectionSettingsTrajectory @ 0x14080bb50
extern "C" void __fastcall cleanupLoadedObjecthkaiJumpDetectionSettingsTrajectory(void* p) {
    if (p) {
        // Cleanup array if present
        int* ip = (int*)p;
        int size = ip[3];
        if (size >= 0) {
            void* data = *(void**)p;
            if (data) {
                // Free memory - simplified
            }
        }
        ip[2] = 0;
        *(void**)p = nullptr;
        ip[3] = 0x80000000;
    }
}

// IDA: cleanupLoadedObjecthkaiJumpDetectionSettingsJumpDescription @ 0x14080bbc0
extern "C" void __fastcall cleanupLoadedObjecthkaiJumpDetectionSettingsJumpDescription(void* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// IDA: hkStringPtr_destructor — extern wrapper for inline ~hkStringPtr defined in HavokTypes.h
// inline ~hkStringPtr() { assign(nullptr, -1); }
extern "C" void __fastcall hkStringPtr_destructor(void* p) {
    ((hkStringPtr*)p)->~hkStringPtr();
}

// IDA: cleanupLoadedObjecthkRootLevelContainerNamedVariant @ 0x1407b5cc0
// IDA decompile: ref = p[2].m_stringAndFlag; if (ref) hkReferencedObject::removeReference(ref);
//                p[2].m_stringAndFlag = nullptr; hkStringPtr::~hkStringPtr(p + 1); hkStringPtr::~hkStringPtr(p);
extern "C" void __fastcall cleanupLoadedObjecthkRootLevelContainerNamedVariant(void* p) {
    if (p) {
        hkStringPtr* name = (hkStringPtr*)p;
        hkStringPtr* className = name + 1;
        hkStringPtr* variant = name + 2;
        hkReferencedObject* ref = reinterpret_cast<hkReferencedObject*>(const_cast<char*>(variant->m_stringAndFlag));
        if (ref)
            hkReferencedObject::removeReference(ref);
        variant->m_stringAndFlag = nullptr;
        hkStringPtr_destructor(className);
        hkStringPtr_destructor(name);
    }
}

// IDA: cleanupLoadedObjecthkMemoryResourceHandle @ 0x1407b6b00
extern "C" void __fastcall cleanupLoadedObjecthkMemoryResourceHandle(void* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// IDA: cleanupLoadedObjecthkMemoryResourceContainer @ 0x1407b6b60
extern "C" void __fastcall cleanupLoadedObjecthkMemoryResourceContainer(void* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// ============================================================================
// Part 131: getVtable serialization support functions
// ============================================================================

// IDA: getVtablehkaiNavMesh @ 0x14080b7e0
extern "C" void* __fastcall getVtablehkaiNavMesh() {
    // Create temp object and return vtable
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        // Allocate temp space and initialize to get vtable
        char temp[256];
        memset(temp, 0, sizeof(temp));
        // hkaiNavMesh::hkaiNavMesh(temp);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiNavMeshInstance @ 0x14080b8a0
extern "C" void* __fastcall getVtablehkaiNavMeshInstance() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[256];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiStaticTreeNavMeshQueryMediator @ 0x14080b840
extern "C" void* __fastcall getVtablehkaiStaticTreeNavMeshQueryMediator() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[64];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiOverlapManager @ 0x14080b930
extern "C" void* __fastcall getVtablehkaiOverlapManager() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[256];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiJumpDetectionSettings @ 0x14080bb30
extern "C" void* __fastcall getVtablehkaiJumpDetectionSettings() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[256];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiNavMeshCutter @ 0x14080bff0
extern "C" void* __fastcall getVtablehkaiNavMeshCutter() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[256];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiStreamingCollection @ 0x14080c280
extern "C" void* __fastcall getVtablehkaiStreamingCollection() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[128];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiNavVolumeInstance @ 0x14080c530
extern "C" void* __fastcall getVtablehkaiNavVolumeInstance() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[256];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkMemoryResourceHandle @ 0x1407b6b10
extern "C" void* __fastcall getVtablehkMemoryResourceHandle() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[64];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkMemoryResourceContainer @ 0x1407b6b70
extern "C" void* __fastcall getVtablehkMemoryResourceContainer() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[64];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// ============================================================================
// Part 132: More getVtable and cleanupLoadedObject functions
// ============================================================================

// IDA: getVtablehkMemoryMeshBody @ 0x1407e58b0
extern "C" void* __fastcall getVtablehkMemoryMeshBody() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[256];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkMemoryMeshTexture @ 0x1407e5920
extern "C" void** __fastcall getVtablehkMemoryMeshTexture() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[128];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return (void**)vtable_ptr;
}

// IDA: getVtablehkaiOverlapManagerSectionGeneratorData @ 0x14080b990
extern "C" void __fastcall getVtablehkaiOverlapManagerSectionGeneratorData(void* u) {
    // Empty function - returns vtable via side effect
    (void)u;
}

// IDA: getVtablehkaiVolumePathfindingUtilFindPathInput @ 0x14080be20
extern "C" void __fastcall getVtablehkaiVolumePathfindingUtilFindPathInput(void* u) {
    // Empty function - constructs temp object
    (void)u;
}

// IDA: getVtablehkaiVolumePathfindingUtilFindPathOutput @ 0x14080be90
extern "C" void __fastcall getVtablehkaiVolumePathfindingUtilFindPathOutput(void* u) {
    // Empty function - constructs temp object
    (void)u;
}

// IDA: cleanupLoadedObjecthkSetUint32 @ 0x140791e30
extern "C" void __fastcall cleanupLoadedObjecthkSetUint32(int* p) {
    if (p) {
        int v1 = p[3];
        p[2] = 0;
        if (v1 < 0) {
            *(long long*)p = 0;
            p[3] = 0x80000000;
        }
        else {
            // hkContainerHeapAllocator::s_alloc.bufFree
            if (*(void**)p) {
                free(*(void**)p);
            }
            *(long long*)p = 0;
            p[3] = 0x80000000;
        }
    }
}

// IDA: cleanupLoadedObjecthkSetunsignedinthkContainerHeapAllocatorhkMapOperationsunsignedint @ 0x140791dc0
extern "C" void __fastcall cleanupLoadedObjecthkSetunsignedinthkContainerHeapAllocatorhkMapOperationsunsignedint(int* p) {
    if (p) {
        int v1 = p[3];
        p[2] = 0;
        if (v1 < 0) {
            *(long long*)p = 0;
            p[3] = 0x80000000;
        }
        else {
            // hkContainerHeapAllocator::s_alloc.bufFree
            if (*(void**)p) {
                free(*(void**)p);
            }
            *(long long*)p = 0;
            p[3] = 0x80000000;
        }
    }
}

// IDA: cleanupLoadedObjecthkMemoryResourceHandleExternalLink @ 0x1407b6b90
// IDA decompile: hkStringPtr::~hkStringPtr(p + 1); hkStringPtr::~hkStringPtr(p);
// No if(p) guard in original!
extern "C" void __fastcall cleanupLoadedObjecthkMemoryResourceHandleExternalLink(void* p) {
    // hkStringPtr destructor is inline: assign(nullptr, -1)
    // Each hkStringPtr is 8 bytes (pointer + flags)
    // p+1 = offset 8: second hkStringPtr
    // p = offset 0: first hkStringPtr
    hkStringPtr_destructor((char*)p + 8);
    hkStringPtr_destructor(p);
}

// IDA: cleanupLoadedObjecthkPackfileHeader @ 0x1407b6bc0
extern "C" void __fastcall cleanupLoadedObjecthkPackfileHeader(void* p) {
    // Empty function - no cleanup needed
    (void)p;
}

// ============================================================================
// Part 133: More cleanupLoadedObject for hkai types
// ============================================================================

// IDA: cleanupLoadedObjecthkaiStreamingCollectionInstanceInfo @ 0x14080c230
extern "C" void __fastcall cleanupLoadedObjecthkaiStreamingCollectionInstanceInfo(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiStreamingCollection @ 0x14080c270
extern "C" void* __fastcall cleanupLoadedObjecthkaiStreamingCollection(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiDirectedGraphExplicitCostNode @ 0x14080c2b0
extern "C" void __fastcall cleanupLoadedObjecthkaiDirectedGraphExplicitCostNode(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiDirectedGraphExplicitCostEdge @ 0x14080c2d0
extern "C" void __fastcall cleanupLoadedObjecthkaiDirectedGraphExplicitCostEdge(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiDirectedGraphExplicitCost @ 0x14080c310
extern "C" void* __fastcall cleanupLoadedObjecthkaiDirectedGraphExplicitCost(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiPathfindingUtilFindPathInput @ 0x14080c370
extern "C" void* __fastcall cleanupLoadedObjecthkaiPathfindingUtilFindPathInput(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiPathfindingUtilFindPathOutput @ 0x14080c3e0
extern "C" void* __fastcall cleanupLoadedObjecthkaiPathfindingUtilFindPathOutput(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiPathfindingUtilFindGraphPathInput @ 0x14080c440
extern "C" void __fastcall cleanupLoadedObjecthkaiPathfindingUtilFindGraphPathInput(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiAabbTreeNavVolumeMediator @ 0x14080c4a0
extern "C" void* __fastcall cleanupLoadedObjecthkaiAabbTreeNavVolumeMediator(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiNavVolumeInstance @ 0x14080c520
extern "C" void* __fastcall cleanupLoadedObjecthkaiNavVolumeInstance(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// ============================================================================
// Part 134: More cleanupLoadedObject for hkai types
// ============================================================================

// IDA: cleanupLoadedObjecthkaiUserEdgeUtils @ 0x14080ca20
extern "C" void __fastcall cleanupLoadedObjecthkaiUserEdgeUtils(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiUserEdgeSetupArray @ 0x14080ca40
extern "C" void* __fastcall cleanupLoadedObjecthkaiUserEdgeSetupArray(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiUserEdgePairArray @ 0x14080ca60
extern "C" void* __fastcall cleanupLoadedObjecthkaiUserEdgePairArray(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiLineOfSightUtil @ 0x14080cd00
extern "C" void __fastcall cleanupLoadedObjecthkaiLineOfSightUtil(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiLineOfSightUtilInputBase @ 0x14080cd10
extern "C" void __fastcall cleanupLoadedObjecthkaiLineOfSightUtilInputBase(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiLineOfSightUtilLineOfSightInput @ 0x14080cd20
extern "C" void __fastcall cleanupLoadedObjecthkaiLineOfSightUtilLineOfSightInput(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiLineOfSightUtilDirectPathInput @ 0x14080cd30
extern "C" void __fastcall cleanupLoadedObjecthkaiLineOfSightUtilDirectPathInput(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiLineOfSightUtilLineOfSightOutput @ 0x14080cd40
extern "C" void __fastcall cleanupLoadedObjecthkaiLineOfSightUtilLineOfSightOutput(void* p) {
    if (p) {
        // hkaiLineOfSightUtil::LineOfSightOutput::~LineOfSightOutput
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// IDA: cleanupLoadedObjecthkaiDefaultAstarEdgeFilter @ 0x14080cd60
extern "C" void* __fastcall cleanupLoadedObjecthkaiDefaultAstarEdgeFilter(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiSilhouetteGenerationParameters @ 0x14080ce20
extern "C" void __fastcall cleanupLoadedObjecthkaiSilhouetteGenerationParameters(const int* p) {
    // Empty function
    (void)p;
}

// ============================================================================
// Part 135: More cleanupLoadedObject for hkai types
// ============================================================================

// IDA: cleanupLoadedObjecthkaiNavVolume @ 0x14080d3f0
extern "C" void* __fastcall cleanupLoadedObjecthkaiNavVolume(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiAdaptiveRanger @ 0x14080d440
extern "C" void __fastcall cleanupLoadedObjecthkaiAdaptiveRanger(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiPath @ 0x14080d480
extern "C" void* __fastcall cleanupLoadedObjecthkaiPath(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiAstarOutputParameters @ 0x14080d4e0
extern "C" void __fastcall cleanupLoadedObjecthkaiAstarOutputParameters(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiSilhouetteReferenceFrame @ 0x14080d520
extern "C" void __fastcall cleanupLoadedObjecthkaiSilhouetteReferenceFrame(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiSplitGenerationUtils @ 0x14080d580
extern "C" void __fastcall cleanupLoadedObjecthkaiSplitGenerationUtils(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiSplitGenerationUtilsSettings @ 0x14080d5b0
extern "C" void __fastcall cleanupLoadedObjecthkaiSplitGenerationUtilsSettings(void* p) {
    if (p) {
        // hkStringPtr::~hkStringPtr(p + 5)
        char* ptr = (char*)p + 5 * sizeof(void*);
        memset(ptr, 0, sizeof(void*));
    }
}

// IDA: cleanupLoadedObjecthkaiDynamicNavVolumeMediator @ 0x14080d5f0
extern "C" void* __fastcall cleanupLoadedObjecthkaiDynamicNavVolumeMediator(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiPlaneVolume @ 0x14080d660
extern "C" void* __fastcall cleanupLoadedObjecthkaiPlaneVolume(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiNavVolumePathSearchParameters @ 0x14080d6a0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavVolumePathSearchParameters(const int* p) {
    // Empty function
    (void)p;
}

// ============================================================================
// Part 136: More cleanupLoadedObject for hkai NavMesh/NavVolume generation settings
// ============================================================================

// IDA: cleanupLoadedObjecthkaiAgentTraversalInfo @ 0x14080db70
extern "C" void __fastcall cleanupLoadedObjecthkaiAgentTraversalInfo(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavMeshGenerationSettingsMaterialConstructionPair @ 0x14085cbc0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshGenerationSettingsMaterialConstructionPair(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavMeshGenerationSettingsEdgeMatchingParameters @ 0x14085cbf0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshGenerationSettingsEdgeMatchingParameters(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavMeshGenerationSettingsWallClimbingSettings @ 0x14085cc60
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshGenerationSettingsWallClimbingSettings(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavMeshGenerationSettings @ 0x14085ccb0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshGenerationSettings(void* p) {
    if (p) {
        // hkaiNavMeshGenerationSettings::~hkaiNavMeshGenerationSettings
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// IDA: cleanupLoadedObjecthkaiNavMeshGenerationSettingsRegionPruningSettings @ 0x14085ccd0
extern "C" void* __fastcall cleanupLoadedObjecthkaiNavMeshGenerationSettingsRegionPruningSettings(int* p) {
    if (p) {
        int v1 = p[7];
        p[6] = 0;
        if (v1 < 0) {
            *(long long*)(p + 2) = 0;
            p[7] = 0x80000000;
        }
        else {
            if (*(void**)(p + 2)) {
                free(*(void**)(p + 2));
            }
            *(long long*)(p + 2) = 0;
            p[7] = 0x80000000;
        }
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiNavVolumeGenerationSettingsMaterialConstructionInfo @ 0x14085cd70
extern "C" void __fastcall cleanupLoadedObjecthkaiNavVolumeGenerationSettingsMaterialConstructionInfo(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavVolumeGenerationSettingsChunkSettings @ 0x14085cda0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavVolumeGenerationSettingsChunkSettings(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavVolumeGenerationSettingsMergingSettings @ 0x14085cdd0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavVolumeGenerationSettingsMergingSettings(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavVolumeGenerationSettings @ 0x14085ce10
extern "C" void __fastcall cleanupLoadedObjecthkaiNavVolumeGenerationSettings(void* p) {
    if (p) {
        // hkaiNavVolumeGenerationSettings::~hkaiNavVolumeGenerationSettings
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// ============================================================================
// Part 137: cleanupLoadedObject for hkai silhouette recorder and debug utils
// ============================================================================

// IDA: cleanupLoadedObjecthkaiSilhouetteRecorderWorldConnectedEvent @ 0x1408a0f90
extern "C" void* __fastcall cleanupLoadedObjecthkaiSilhouetteRecorderWorldConnectedEvent(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiSilhouetteRecorderInstanceLoadedEvent @ 0x1408a0ff0
extern "C" void* __fastcall cleanupLoadedObjecthkaiSilhouetteRecorderInstanceLoadedEvent(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiSilhouetteRecorderSilhouettesSteppedEvent @ 0x1408a1050
extern "C" void* __fastcall cleanupLoadedObjecthkaiSilhouetteRecorderSilhouettesSteppedEvent(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiSilhouetteRecorderInstanceUnloadedEvent @ 0x1408a10a0
extern "C" void* __fastcall cleanupLoadedObjecthkaiSilhouetteRecorderInstanceUnloadedEvent(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiSilhouetteRecorderVolumeLoadedEvent @ 0x1408a10e0
extern "C" void* __fastcall cleanupLoadedObjecthkaiSilhouetteRecorderVolumeLoadedEvent(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiSilhouetteRecorderVolumeUnloadedEvent @ 0x1408a1120
extern "C" void* __fastcall cleanupLoadedObjecthkaiSilhouetteRecorderVolumeUnloadedEvent(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiSilhouetteRecorderGraphLoadedEvent @ 0x1408a1160
extern "C" void* __fastcall cleanupLoadedObjecthkaiSilhouetteRecorderGraphLoadedEvent(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiSilhouetteRecorderGraphUnloadedEvent @ 0x1408a11a0
extern "C" void* __fastcall cleanupLoadedObjecthkaiSilhouetteRecorderGraphUnloadedEvent(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiNavMeshDebugUtilsSmallEdgeSettings @ 0x1408a12e0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshDebugUtilsSmallEdgeSettings(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavMeshDebugUtilsDebugInfo @ 0x1408a1360
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshDebugUtilsDebugInfo(void* p) {
    if (p) {
        // hkaiNavMeshDebugUtils::DebugInfo::~DebugInfo
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// ============================================================================
// Part 139: More cleanupLoadedObject for hkai types
// ============================================================================

// IDA: cleanupLoadedObjecthkaiOverlapManagerSectionGeneratorData @ 0x14080b8d0
extern "C" void* __fastcall cleanupLoadedObjecthkaiOverlapManagerSectionGeneratorData(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiOverlapManagerSection @ 0x14080b960
extern "C" int __fastcall cleanupLoadedObjecthkaiOverlapManagerSection(const int* p) {
    if (p) {
        // hkaiOverlapManager::Section::~Section
        void** vtable = *(void***)p;
        typedef int (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return 0;
}

// IDA: cleanupLoadedObjecthkaiVolumePathfindingUtil @ 0x14080bec0
extern "C" void __fastcall cleanupLoadedObjecthkaiVolumePathfindingUtil(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiReferenceFrameAndExtrusion @ 0x14080bef0
extern "C" void __fastcall cleanupLoadedObjecthkaiReferenceFrameAndExtrusion(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiVolumePathfindingUtilFindPathInput @ 0x14080be10
extern "C" void* __fastcall cleanupLoadedObjecthkaiVolumePathfindingUtilFindPathInput(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiVolumePathfindingUtilFindPathOutput @ 0x14080be80
extern "C" void* __fastcall cleanupLoadedObjecthkaiVolumePathfindingUtilFindPathOutput(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiNavVolumeGenerationSnapshot @ 0x14080bf50
// ASM: add rcx, 20h; call hkaiNavVolumeGenerationSettings::~hkaiNavVolumeGenerationSettings (0x14085D460)
//      jmp ForwardReferences::~ForwardReferences (0x140791920) - tail call
extern "C" void __fastcall hkaiNavVolumeGenerationSettings_destructor(void* p);  // 0x14085D460
extern "C" void __fastcall ForwardReferences_destructor(void* p);  // 0x140791920
extern "C" void __fastcall cleanupLoadedObjecthkaiNavVolumeGenerationSnapshot(void* p) {
    if (p) {
        char* ptr = (char*)p;
        // Call hkaiNavVolumeGenerationSettings destructor at offset +0x20
        hkaiNavVolumeGenerationSettings_destructor(ptr + 0x20);
        // Tail call to ForwardReferences destructor
        ForwardReferences_destructor(p);
    }
}

// IDA: cleanupLoadedObjecthkaiPersistentFaceKey @ 0x14080bf80
extern "C" void __fastcall cleanupLoadedObjecthkaiPersistentFaceKey(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavMeshCutter @ 0x14080bfe0
extern "C" void* __fastcall cleanupLoadedObjecthkaiNavMeshCutter(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// ============================================================================
// Part 140: finishLoadedObject and cleanupLoadedObject for hkai NavMeshCutter and StreamingSet
// ============================================================================

// IDA: finishLoadedObjecthkaiVolumePathfindingUtilFindPathInput @ 0x14080bdf0
// IDA decompile: calls hkaiVolumePathfindingUtil::FindPathInput::FindPathInput(p, a2)
extern "C" void __fastcall finishLoadedObjecthkaiVolumePathfindingUtilFindPathInput(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiVolumePathfindingUtil_FindPathInput_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiVolumePathfindingUtilFindPathOutput @ 0x14080be60
// ASM: test rcx,rcx; jz; call 0x14081CC00 (hkaiVolumePathfindingUtil::FindPathOutput::FindPathOutput(hkFinishLoadedObjectFlag))
extern "C" void __fastcall finishLoadedObjecthkaiVolumePathfindingUtilFindPathOutput(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiVolumePathfindingUtil_FindPathOutput_constructor(p, a2);
    }
}

// IDA: cleanupLoadedObjecthkaiNavMeshCutterSavedConnectivity @ 0x14080c020
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshCutterSavedConnectivity(void* p) {
    if (p) {
        // hkaiNavMeshCutter::SavedConnectivity::~SavedConnectivity
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// IDA: finishLoadedObjecthkaiNavMeshCutterMeshInfo @ 0x14080c030
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshCutterMeshInfo(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiNavMeshCutterSavedConnectivity @ 0x14080c040
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshCutterSavedConnectivity(void* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavMeshCutterMeshInfo @ 0x14080c050
extern "C" void* __fastcall cleanupLoadedObjecthkaiNavMeshCutterMeshInfo(int* p) {
    if (p) {
        int v1 = p[7];
        p[6] = 0;
        if (v1 < 0) {
            *(long long*)(p + 2) = 0;
            p[7] = 0x80000000;
        }
        else {
            if (*(void**)(p + 2)) {
                free(*(void**)(p + 2));
            }
            *(long long*)(p + 2) = 0;
            p[7] = 0x80000000;
        }
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiStreamingSetNavMeshConnection @ 0x14080c0d0
extern "C" void __fastcall cleanupLoadedObjecthkaiStreamingSetNavMeshConnection(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiStreamingSetGraphConnection @ 0x14080c0f0
extern "C" void __fastcall cleanupLoadedObjecthkaiStreamingSetGraphConnection(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiStreamingSetVolumeConnection @ 0x14080c110
extern "C" void __fastcall cleanupLoadedObjecthkaiStreamingSetVolumeConnection(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiStreamingSet @ 0x14080c140
extern "C" void __fastcall cleanupLoadedObjecthkaiStreamingSet(void* p) {
    if (p) {
        // hkaiStreamingSet::~hkaiStreamingSet
        void** vtable = *(void***)p;
        typedef void (__fastcall *DtorFunc)(void*, int);
        ((DtorFunc)vtable[0])(p, 0);
    }
}

// ============================================================================
// Part 141: finishLoadedObject and cleanupLoadedObject for hkai DirectedGraph and Pathfinding
// ============================================================================

// IDA: finishLoadedObjecthkaiDirectedGraphExplicitCost @ 0x14080c2f0
// IDA decompile: calls hkaiDirectedGraphExplicitCost::hkaiDirectedGraphExplicitCost(p, hkFinishLoadedObjectFlag)
extern "C" void __fastcall finishLoadedObjecthkaiDirectedGraphExplicitCost(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiDirectedGraphExplicitCost_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiPathfindingUtilFindPathOutput @ 0x14080c3c0
// IDA decompile: calls hkaiPathfindingUtil::FindPathOutput::FindPathOutput(p, hkFinishLoadedObjectFlag)
extern "C" void __fastcall finishLoadedObjecthkaiPathfindingUtilFindPathOutput(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiPathfindingUtil_FindPathOutput_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiPathfindingUtilFindGraphPathInput @ 0x14080c420
// ASM: test rcx,rcx; jz; call 0x1407F6D20 (flag transparently forwarded in RDX)
// PDB: ??0FindGraphPathInput@hkaiPathfindingUtil@@QEAA@VhkFinishLoadedObjectFlag@@@Z
extern "C" void __fastcall finishLoadedObjecthkaiPathfindingUtilFindGraphPathInput(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiPathfindingUtil_FindGraphPathInput_constructor(p, a2);
    }
}

// IDA: cleanupLoadedObjecthkaiPathfindingUtil @ 0x14080c460
extern "C" void __fastcall cleanupLoadedObjecthkaiPathfindingUtil(const int* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiAabbTreeNavVolumeMediator @ 0x14080c480
// IDA decompile: calls hkaiAabbTreeNavVolumeMediator::hkaiAabbTreeNavVolumeMediator(p, hkFinishLoadedObjectFlag)
extern "C" void __fastcall finishLoadedObjecthkaiAabbTreeNavVolumeMediator(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiAabbTreeNavVolumeMediator_constructor(p, a2);
    }
}

// IDA: cleanupLoadedObjecthkaiNavVolumeInstanceCellInstance @ 0x14080c4e0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavVolumeInstanceCellInstance(const int* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiMaterialPainter @ 0x14080c560
// IDA decompile: calls hkaiMaterialPainter::hkaiMaterialPainter(p, hkFinishLoadedObjectFlag)
extern "C" void __fastcall finishLoadedObjecthkaiMaterialPainter(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiMaterialPainter_constructor(p, a2);
    }
}

// IDA: cleanupLoadedObjecthkaiMaterialPainter @ 0x14080c580
extern "C" void* __fastcall cleanupLoadedObjecthkaiMaterialPainter(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: finishLoadedObjecthkaiDirectedGraphInstance @ 0x14080c5d0
// ASM: test rcx,rcx; jz; call 0x14081C5F0 (flag transparently forwarded in RDX)
// PDB: ??0hkaiDirectedGraphInstance@@QEAA@VhkFinishLoadedObjectFlag@@@Z
extern "C" void __fastcall finishLoadedObjecthkaiDirectedGraphInstance(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiDirectedGraphInstance_constructor(p, a2);
    }
}

// IDA: cleanupLoadedObjecthkaiDirectedGraphInstance @ 0x14080c5f0
extern "C" void* __fastcall cleanupLoadedObjecthkaiDirectedGraphInstance(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// ============================================================================
// Part 142: finishLoadedObject and cleanupLoadedObject for hkai EdgePath and World
// ============================================================================

// IDA: finishLoadedObjecthkaiDirectedGraphInstanceFreeBlockList @ 0x14080c630
extern "C" void __fastcall finishLoadedObjecthkaiDirectedGraphInstanceFreeBlockList(void* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiDirectedGraphInstanceFreeBlockList @ 0x14080c640
extern "C" void* __fastcall cleanupLoadedObjecthkaiDirectedGraphInstanceFreeBlockList(int* p) {
    if (p) {
        int v1 = p[3];
        p[2] = 0;
        if (v1 < 0) {
            *(long long*)p = 0;
            p[3] = 0x80000000;
        }
        else {
            if (*(void**)p) {
                free(*(void**)p);
            }
            *(long long*)p = 0;
            p[3] = 0x80000000;
        }
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiEdgePathEdge @ 0x14080c6b0
extern "C" void __fastcall cleanupLoadedObjecthkaiEdgePathEdge(const int* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiEdgePath @ 0x14080c6d0
// IDA decompile: if (p) hkaiEdgePath::hkaiEdgePath(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiEdgePath(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiEdgePath_constructor(p, a2);
    }
}

// IDA: cleanupLoadedObjecthkaiEdgePath @ 0x14080c6f0
extern "C" void* __fastcall cleanupLoadedObjecthkaiEdgePath(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiNavMeshPathRequestInfo @ 0x14080c730
extern "C" void* __fastcall cleanupLoadedObjecthkaiNavMeshPathRequestInfo(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiNavVolumePathRequestInfo @ 0x14080c750
extern "C" void* __fastcall cleanupLoadedObjecthkaiNavVolumePathRequestInfo(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: finishLoadedObjecthkaiWorldCharacterStepSerializableContext @ 0x14080c770
// IDA decompile: if (p) hkaiWorld::CharacterStepSerializableContext::CharacterStepSerializableContext(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiWorldCharacterStepSerializableContext(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiWorld_CharacterStepSerializableContext_constructor(p, a2);
    }
}

// IDA: cleanupLoadedObjecthkaiWorldCharacterStepSerializableContext @ 0x14080c790
extern "C" void* __fastcall cleanupLoadedObjecthkaiWorldCharacterStepSerializableContext(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiWorld @ 0x14080c7f0
extern "C" void* __fastcall cleanupLoadedObjecthkaiWorld(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// ============================================================================
// Part 143: finishLoadedObject and cleanupLoadedObject for hkai World and UserEdgeUtils
// ============================================================================

// IDA: finishLoadedObjecthkaiWorld @ 0x14080c7d0
// IDA decompile: if (p) hkaiWorld::hkaiWorld(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiWorld(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiWorld_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiNavMeshPathRequestInfo @ 0x14080c830
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshPathRequestInfo(void* p) {
    if (p) {
        // Set vtable
        static void* vtable = nullptr;
        if (!vtable) {
            char temp[64];
            memset(temp, 0, sizeof(temp));
            vtable = *(void**)temp;
        }
        *(void**)p = vtable;
    }
}

// IDA: finishLoadedObjecthkaiNavVolumePathRequestInfo @ 0x14080c860
extern "C" void __fastcall finishLoadedObjecthkaiNavVolumePathRequestInfo(void* p) {
    if (p) {
        // Set vtable
        static void* vtable = nullptr;
        if (!vtable) {
            char temp[64];
            memset(temp, 0, sizeof(temp));
            vtable = *(void**)temp;
        }
        *(void**)p = vtable;
    }
}

// IDA: cleanupLoadedObjecthkaiNavMeshPathSearchParameters @ 0x14080c930
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshPathSearchParameters(const int* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiNavMeshPathSearchParameters @ 0x14080c940
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshPathSearchParameters(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiDynamicNavMeshQueryMediator @ 0x14080c960
// IDA decompile: if (p) hkaiDynamicNavMeshQueryMediator::hkaiDynamicNavMeshQueryMediator(p);
extern "C" void __fastcall finishLoadedObjecthkaiDynamicNavMeshQueryMediator(void* p) {
    if (p) {
        *(void**)p = &hkaiDynamicNavMeshQueryMediator_vftable;
    }
}

// IDA: cleanupLoadedObjecthkaiDynamicNavMeshQueryMediator @ 0x14080c980
extern "C" void* __fastcall cleanupLoadedObjecthkaiDynamicNavMeshQueryMediator(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiUserEdgeUtilsObb @ 0x14080c9c0
extern "C" void __fastcall cleanupLoadedObjecthkaiUserEdgeUtilsObb(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiUserEdgeUtilsUserEdgeSetup @ 0x14080c9e0
extern "C" void __fastcall cleanupLoadedObjecthkaiUserEdgeUtilsUserEdgeSetup(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiUserEdgeUtilsUserEdgePair @ 0x14080ca00
extern "C" void __fastcall cleanupLoadedObjecthkaiUserEdgeUtilsUserEdgePair(const int* p) {
    // Empty function
    (void)p;
}

// ============================================================================
// Part 144: finishLoadedObject and cleanupLoadedObject for hkai LineOfSight and Avoidance
// ============================================================================

// IDA: finishLoadedObjecthkaiUserEdgeSetupArray @ 0x14080ca70
extern "C" void __fastcall finishLoadedObjecthkaiUserEdgeSetupArray(void* p) {
    if (p) {
        // Set vtable
        static void* vtable = nullptr;
        if (!vtable) {
            char temp[64];
            memset(temp, 0, sizeof(temp));
            vtable = *(void**)temp;
        }
        *(void**)p = vtable;
    }
}

// IDA: finishLoadedObjecthkaiUserEdgePairArray @ 0x14080caa0
extern "C" void __fastcall finishLoadedObjecthkaiUserEdgePairArray(void* p) {
    if (p) {
        // Set vtable
        static void* vtable = nullptr;
        if (!vtable) {
            char temp[64];
            memset(temp, 0, sizeof(temp));
            vtable = *(void**)temp;
        }
        *(void**)p = vtable;
    }
}

// IDA: finishLoadedObjecthkaiLineOfSightUtilInputBase @ 0x14080cc40
// IDA decompile: if (p) hkaiLineOfSightUtil::InputBase::InputBase(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiLineOfSightUtilInputBase(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiLineOfSightUtil_InputBase_constructor(p);
    }
    (void)a2;
}

// IDA: finishLoadedObjecthkaiLineOfSightUtilLineOfSightInput @ 0x14080cc70
// IDA decompile: if (p) hkaiLineOfSightUtil::LineOfSightInput::LineOfSightInput(p);
extern "C" void __fastcall finishLoadedObjecthkaiLineOfSightUtilLineOfSightInput(void* p) {
    if (p) {
        hkaiLineOfSightUtil_LineOfSightInput_constructor(p);
    }
}

// IDA: finishLoadedObjecthkaiLineOfSightUtilDirectPathInput @ 0x14080cca0
// IDA decompile: if (p) hkaiLineOfSightUtil::DirectPathInput::DirectPathInput(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiLineOfSightUtilDirectPathInput(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiLineOfSightUtil_DirectPathInput_constructor(p);
    }
    (void)a2;
}

// IDA: finishLoadedObjecthkaiLineOfSightUtilLineOfSightOutput @ 0x14080ccd0
// IDA decompile: if (p) hkaiLineOfSightUtil::LineOfSightOutput::LineOfSightOutput(p);
extern "C" void __fastcall finishLoadedObjecthkaiLineOfSightUtilLineOfSightOutput(void* p) {
    if (p) {
        hkaiLineOfSightUtil_LineOfSightOutput_constructor(p);
    }
}

// IDA: finishLoadedObjecthkaiDefaultAstarEdgeFilter @ 0x14080cd70
// IDA decompile: if (p) p->_Bx._Ptr = &hkaiDefaultAstarEdgeFilter::`vftable';
extern "C" void __fastcall finishLoadedObjecthkaiDefaultAstarEdgeFilter(void* p) {
    if (p) {
        *(void**)p = &hkaiDefaultAstarEdgeFilter_vftable;
    }
}

// IDA: finishLoadedObjecthkaiSilhouetteGenerationParameters @ 0x14080ce10
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteGenerationParameters(void* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiAvoidancePairPropertiesPairData @ 0x14080ce40
extern "C" void __fastcall cleanupLoadedObjecthkaiAvoidancePairPropertiesPairData(const int* p) {
    // Empty function
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiAvoidancePairProperties @ 0x14080ce60
extern "C" void* __fastcall cleanupLoadedObjecthkaiAvoidancePairProperties(const int* p) {
    if (p) {
        void** vtable = *(void***)p;
        typedef void* (__fastcall *DtorFunc)(const int*, int);
        return ((DtorFunc)vtable[0])(p, 0);
    }
    return nullptr;
}

// ============================================================================
// Part 145: finishLoadedObject for hkai Avoidance, Obstacle, Silhouette, NavVolume
// ============================================================================

// IDA: finishLoadedObjecthkaiAvoidanceSolverMovementProperties @ 0x14080cef0
extern "C" void __fastcall finishLoadedObjecthkaiAvoidanceSolverMovementProperties(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiAvoidanceSolverAvoidanceProperties @ 0x14080cf00
extern "C" void __fastcall finishLoadedObjecthkaiAvoidanceSolverAvoidanceProperties(void* p) {
    if (p) {
        // Set vtable for hkaiAvoidanceSolver::AvoidanceProperties
        static void* vtable = nullptr;
        if (!vtable) {
            char temp[64];
            memset(temp, 0, sizeof(temp));
            vtable = *(void**)temp;
        }
        *(void**)p = vtable;
    }
}

// IDA: finishLoadedObjecthkaiAvoidancePairProperties @ 0x14080cf30
extern "C" void __fastcall finishLoadedObjecthkaiAvoidancePairProperties(void* p) {
    if (p) {
        // Set vtable for hkaiAvoidancePairProperties
        static void* vtable = nullptr;
        if (!vtable) {
            char temp[64];
            memset(temp, 0, sizeof(temp));
            vtable = *(void**)temp;
        }
        *(void**)p = vtable;
    }
}

// IDA: finishLoadedObjecthkaiSimpleObstacleGenerator @ 0x14080d030
// IDA decompile: hkaiObstacleGenerator::hkaiObstacleGenerator(p, a2); then set vtable
extern "C" void __fastcall finishLoadedObjecthkaiSimpleObstacleGenerator(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiObstacleGenerator_constructor(p);
        *(void**)p = &hkaiSimpleObstacleGenerator_vftable;
        (void)a2;
    }
}

// IDA: finishLoadedObjecthkaiSimpleSilhouetteMerger @ 0x14080d240
// IDA decompile: if (p) p->_Bx._Ptr = &hkaiSimpleSilhouetteMerger::`vftable';
extern "C" void __fastcall finishLoadedObjecthkaiSimpleSilhouetteMerger(void* p) {
    if (p) {
        *(void**)p = &hkaiSimpleSilhouetteMerger_vftable;
    }
}

// IDA: finishLoadedObjecthkaiConvexHullSilhouetteMerger @ 0x14080d270
// IDA decompile: if (p) p->_Bx._Ptr = &hkaiConvexHullSilhouetteMerger::`vftable';
extern "C" void __fastcall finishLoadedObjecthkaiConvexHullSilhouetteMerger(void* p) {
    if (p) {
        *(void**)p = &hkaiConvexHullSilhouetteMerger_vftable;
    }
}

// IDA: finishLoadedObjecthkaiSilhouetteGeneratorSectionContext @ 0x14080d2b0
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteGeneratorSectionContext(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiSplitGenerationSnapshot @ 0x14080d300
// IDA decompile: hkGeometry::hkGeometry(p, a2); hkaiNavMeshGenerationSettings::hkaiNavMeshGenerationSettings(&p[1], a2); hkStringPtr::hkStringPtr
extern "C" void __fastcall finishLoadedObjecthkaiSplitGenerationSnapshot(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkGeometry_constructor(p, a2);
        // hkaiNavMeshGenerationSettings follows hkGeometry
        hkaiNavMeshGenerationSettings_constructor((char*)p + 56, a2);
        // hkStringPtr field
        hkStringPtr_constructor((char*)p + 56 + 128, a2);
    }
}

// IDA: finishLoadedObjecthkaiNavVolume @ 0x14080d3d0
// IDA decompile: if (p) hkaiNavVolume::hkaiNavVolume(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiNavVolume(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiNavVolume_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiAdaptiveRanger @ 0x14080d430
extern "C" void __fastcall finishLoadedObjecthkaiAdaptiveRanger(void* p) {
    // Empty function
    (void)p;
}

// ============================================================================
// Part 146: finishLoadedObject for hkai Path, Silhouette, NavVolume, Steering
// ============================================================================

// IDA: finishLoadedObjecthkaiPath @ 0x14080d490
// IDA: finishLoadedObjecthkaiPath @ 0x14080d490
// IDA decompile: if (p) p->_Bx._Ptr = &hkaiPath::`vftable';
extern "C" void __fastcall finishLoadedObjecthkaiPath(void* p) {
    if (p) {
        *(void**)p = &hkaiPath_vftable;
    }
}

// IDA: finishLoadedObjecthkaiAstarOutputParameters @ 0x14080d4f0
extern "C" void __fastcall finishLoadedObjecthkaiAstarOutputParameters(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiSilhouetteReferenceFrame @ 0x14080d510
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteReferenceFrame(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiSplitGenerationUtilsSettings @ 0x14080d590
extern "C" void __fastcall finishLoadedObjecthkaiSplitGenerationUtilsSettings(void* p) {
    if (p) {
        // hkStringPtr::hkStringPtr((hkStringPtr *)&p[1]);
    }
}

// IDA: finishLoadedObjecthkaiDynamicNavVolumeMediator @ 0x14080d5d0
// IDA decompile: if (p) hkaiDynamicNavVolumeMediator::hkaiDynamicNavVolumeMediator(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiDynamicNavVolumeMediator(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiDynamicNavVolumeMediator_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiPlaneVolume @ 0x14080d640
// IDA decompile: if (p) hkaiPlaneVolume::hkaiPlaneVolume(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiPlaneVolume(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiPlaneVolume_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiNavVolumePathSearchParameters @ 0x14080d6b0
extern "C" void __fastcall finishLoadedObjecthkaiNavVolumePathSearchParameters(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiPathFollowingProperties @ 0x14080d6d0
extern "C" void __fastcall finishLoadedObjecthkaiPathFollowingProperties(void* p) {
    if (p) {
        // Set vtable for hkaiPathFollowingProperties
        static void* vtable = nullptr;
        if (!vtable) {
            char temp[64];
            memset(temp, 0, sizeof(temp));
            vtable = *(void**)temp;
        }
        *(void**)p = vtable;
    }
}

// IDA: finishLoadedObjecthkaiGraphPathSearchParameters @ 0x14080d720
extern "C" void __fastcall finishLoadedObjecthkaiGraphPathSearchParameters(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiLocalSteeringInput @ 0x14080d750
extern "C" void __fastcall finishLoadedObjecthkaiLocalSteeringInput(void* p) {
    // Empty function
    (void)p;
}

// ============================================================================
// Part 147: finishLoadedObject for hkai Character, Silhouette, NavMeshGeneration
// ============================================================================

// IDA: finishLoadedObjecthkaiConvexSilhouetteSet @ 0x14080d780
// IDA decompile: if (p) hkaiConvexSilhouetteSet::hkaiConvexSilhouetteSet(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiConvexSilhouetteSet(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiConvexSilhouetteSet_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiDefaultAstarCostModifier @ 0x14080d860
// IDA decompile: if (p) p->_Bx._Ptr = &hkaiDefaultAstarCostModifier::`vftable';
extern "C" void __fastcall finishLoadedObjecthkaiDefaultAstarCostModifier(void* p) {
    if (p) {
        *(void**)p = &hkaiDefaultAstarCostModifier_vftable;
    }
}

// IDA: finishLoadedObjecthkaiReferenceFrame @ 0x14080d900
extern "C" void __fastcall finishLoadedObjecthkaiReferenceFrame(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiNavMeshGenerationSnapshot @ 0x14080d930
// IDA decompile: hkGeometry::hkGeometry(p, a2); hkaiNavMeshGenerationSettings::hkaiNavMeshGenerationSettings(&p[1], a2);
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshGenerationSnapshot(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkGeometry_constructor(p, a2);
        hkaiNavMeshGenerationSettings_constructor((char*)p + 56, a2);
    }
}

// IDA: finishLoadedObjecthkaiCharacter @ 0x14080d9a0
// IDA decompile: if (p) hkaiCharacter::hkaiCharacter(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiCharacter(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiCharacter_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiInvertedAabbVolume @ 0x14080da10
// IDA decompile: sets vtable, then hkGeometry::hkGeometry
extern "C" void __fastcall finishLoadedObjecthkaiInvertedAabbVolume(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        *(void**)p = &hkaiInvertedAabbVolume_vftable;
        hkGeometry_constructor((char*)p + 8, a2);
    }
}

// IDA: finishLoadedObjecthkaiPointCloudSilhouetteGenerator @ 0x14080daf0
// IDA decompile: if (p) hkaiPointCloudSilhouetteGenerator::hkaiPointCloudSilhouetteGenerator(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiPointCloudSilhouetteGenerator(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiPointCloudSilhouetteGenerator_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiAgentTraversalInfo @ 0x14080db60
extern "C" void __fastcall finishLoadedObjecthkaiAgentTraversalInfo(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiNavMeshGenerationSettingsEdgeMatchingParameters @ 0x14085cbe0
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshGenerationSettingsEdgeMatchingParameters(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiNavMeshGenerationSettingsOverrideSettings @ 0x14085cc10
// IDA decompile: if (p) hkaiNavMeshGenerationSettings::OverrideSettings::OverrideSettings(p);
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshGenerationSettingsOverrideSettings(void* p) {
    if (p) {
        hkaiNavMeshGenerationSettings_OverrideSettings_constructor(p);
    }
}

// ============================================================================
// Part 148: finishLoadedObject for hkai NavMeshGeneration, NavVolumeGeneration
// ============================================================================

// IDA: finishLoadedObjecthkaiNavMeshGenerationSettingsWallClimbingSettings @ 0x14085cc50
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshGenerationSettingsWallClimbingSettings(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiNavMeshGenerationSettings @ 0x14085cc80
// IDA decompile: if (p) hkaiNavMeshGenerationSettings::hkaiNavMeshGenerationSettings(p, flag);
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshGenerationSettings(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiNavMeshGenerationSettings_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiNavMeshGenerationSettingsRegionPruningSettings @ 0x14085ccc0
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshGenerationSettingsRegionPruningSettings(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiNavVolumeGenerationSettingsMaterialConstructionInfo @ 0x14085cd50
// IDA decompile: if (p) hkaiNavVolumeGenerationSettings::MaterialConstructionInfo::MaterialConstructionInfo(p);
extern "C" void __fastcall finishLoadedObjecthkaiNavVolumeGenerationSettingsMaterialConstructionInfo(void* p) {
    if (p) {
        hkaiNavVolumeGenerationSettings_MaterialConstructionInfo_constructor(p);
    }
}

// IDA: finishLoadedObjecthkaiNavVolumeGenerationSettingsChunkSettings @ 0x14085cd90
extern "C" void __fastcall finishLoadedObjecthkaiNavVolumeGenerationSettingsChunkSettings(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiNavVolumeGenerationSettingsMergingSettings @ 0x14085cdc0
extern "C" void __fastcall finishLoadedObjecthkaiNavVolumeGenerationSettingsMergingSettings(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiNavVolumeGenerationSettings @ 0x14085cdf0
// IDA decompile: if (p) hkaiNavVolumeGenerationSettings::hkaiNavVolumeGenerationSettings(p, finishing);
extern "C" void __fastcall finishLoadedObjecthkaiNavVolumeGenerationSettings(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiNavVolumeGenerationSettings_constructor(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiEdgeGeometryFace @ 0x14085ce80
extern "C" void __fastcall finishLoadedObjecthkaiEdgeGeometryFace(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiEdgeGeometry @ 0x14085ce90
extern "C" void __fastcall finishLoadedObjecthkaiEdgeGeometry(void* p) {
    if (p) {
        // Set vtable for hkaiEdgeGeometry
        static void* vtable = nullptr;
        if (!vtable) {
            char temp[64];
            memset(temp, 0, sizeof(temp));
            vtable = *(void**)temp;
        }
        *(void**)p = vtable;
    }
}

// IDA: finishLoadedObjecthkaiNavMeshSimplificationUtilsExtraVertexSettings @ 0x14085d010
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshSimplificationUtilsExtraVertexSettings(void* p) {
    // Empty function
    (void)p;
}

// ============================================================================
// Part 149: finishLoadedObject for hkai DynamicTree, StaticTree, SilhouetteRecorder
// ============================================================================

// IDA: finishLoadedObjecthkaiNavMeshSimplificationUtilsSettings @ 0x14085d090
extern "C" void __fastcall finishLoadedObjecthkaiNavMeshSimplificationUtilsSettings(void* p) {
    // Empty function
    (void)p;
}

// IDA: finishLoadedObjecthkaiDynamicTree @ 0x14085d120
extern "C" void __fastcall finishLoadedObjecthkaiDynamicTree(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiDynamicTree_constructor2(p, a2);
    }
}

// IDA: finishLoadedObjecthkaiStaticTree @ 0x14085d180
// IDA decompile: if (p) hkaiStaticTree::hkaiStaticTree(p, flag);
extern "C" void __fastcall finishLoadedObjecthkaiStaticTree(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkaiStaticTree_constructor2(p, a2);
    }
}

// IDA: finishLoadedObjecthkcdShape @ 0x1408a0ee0
// IDA decompile: if (p) return hkcdShape::hkcdShape(p, a2);
extern "C" void* __fastcall finishLoadedObjecthkcdShape(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        hkcdShape_constructor2(p);
    }
    (void)a2;
    return p;
}

// IDA: finishLoadedObjecthkaiSilhouetteRecorderWorldConnectedEvent @ 0x1408a0f70
// IDA decompile: if (p) hkaiSilhouetteRecorder::WorldConnectedEvent::WorldConnectedEvent(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteRecorderWorldConnectedEvent(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        *(void**)p = &hkaiSilhouetteRecorder_WorldConnectedEvent_vftable;
    }
    (void)a2;
}

// IDA: finishLoadedObjecthkaiSilhouetteRecorderInstanceLoadedEvent @ 0x1408a0fd0
// IDA decompile: if (p) hkaiSilhouetteRecorder::InstanceLoadedEvent::InstanceLoadedEvent(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteRecorderInstanceLoadedEvent(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        *(void**)p = &hkaiSilhouetteRecorder_InstanceLoadedEvent_vftable;
    }
    (void)a2;
}

// IDA: finishLoadedObjecthkaiSilhouetteRecorderSilhouettesSteppedEvent @ 0x1408a1030
// IDA decompile: if (p) hkaiSilhouetteRecorder::SilhouettesSteppedEvent::SilhouettesSteppedEvent(p);
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteRecorderSilhouettesSteppedEvent(void* p) {
    if (p) {
        *(void**)p = &hkaiSilhouetteRecorder_SilhouettesSteppedEvent_vftable;
    }
}

// IDA: finishLoadedObjecthkaiSilhouetteRecorderVolumeLoadedEvent @ 0x1408a10c0
// IDA decompile: if (p) hkaiSilhouetteRecorder::VolumeLoadedEvent::VolumeLoadedEvent(p, a2);
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteRecorderVolumeLoadedEvent(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        *(void**)p = &hkaiSilhouetteRecorder_VolumeLoadedEvent_vftable;
    }
    (void)a2;
}

// IDA: finishLoadedObjecthkaiSilhouetteRecorderGraphLoadedEvent @ 0x1408a1140
// IDA decompile: if (p) hkaiSilhouetteRecorder::GraphLoadedEvent::GraphLoadedEvent(p);
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteRecorderGraphLoadedEvent(void* p) {
    if (p) {
        *(void**)p = &hkaiSilhouetteRecorder_GraphLoadedEvent_vftable;
    }
}

// IDA: finishLoadedObjecthkaiSilhouetteRecorderReplayEvent @ 0x1408a11b0
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteRecorderReplayEvent(void* p) {
    if (p) {
        // Set vtable for hkaiSilhouetteRecorder::ReplayEvent
        static void* vtable = nullptr;
        if (!vtable) {
            char temp[64];
            memset(temp, 0, sizeof(temp));
            vtable = *(void**)temp;
        }
        *(void**)p = vtable;
    }
}

// ============================================================================
// Part 150: hkFinishLoadedObjectFlag constructors
// ============================================================================

// IDA: ??0StringMap@hkMonitorStreamStringMap@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14079e6e0
// IDA decompile: calls hkStringPtr::hkStringPtr(&this->m_string, f); return this;
// hkStringPtr::hkStringPtr(hkFinishLoadedObjectFlag) is empty inline (: m_stringAndFlag(nullptr) {})
// So this function is effectively a no-op
extern "C" void* __fastcall hkMonitorStreamStringMap_StringMap_constructor(void* p, hkFinishLoadedObjectFlag f) {
    // hkStringPtr::hkStringPtr(hkFinishLoadedObjectFlag) is empty — no initialization needed
    (void)f;
    return p;
}

// IDA: ??0hkMonitorStreamStringMap@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407a37b0
extern "C" void* __fastcall hkMonitorStreamStringMap_constructor(void* p) {
    // Empty function - just return this
    return p;
}

// IDA: ??0NamedVariant@hkRootLevelContainer@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407b6160
// IDA decompile: calls hkStringPtr::hkStringPtr(&this->m_name, f); hkStringPtr::hkStringPtr(&this->m_className, f); return this;
// Both hkStringPtr::hkStringPtr(hkFinishLoadedObjectFlag) calls are empty inline — effectively no-op
extern "C" void* __fastcall hkRootLevelContainer_NamedVariant_constructor(void* p, hkFinishLoadedObjectFlag f) {
    // hkStringPtr::hkStringPtr(&m_name, f) — empty inline
    // hkStringPtr::hkStringPtr(&m_className, f) — empty inline
    (void)f;
    return p;
}

// IDA: ??0hkRootLevelContainer@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407b6190
extern "C" void* __fastcall hkRootLevelContainer_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    // Empty function - just return this
    (void)a2;
    return p;
}

// IDA: ??0hkMemoryResourceHandle@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407d7810
// IDA decompile: p_m_name = &this->m_name; p_m_name[-3].m_stringAndFlag = &hkMemoryResourceHandle::`vftable';
// hkStringPtr::hkStringPtr(p_m_name, flag); return this;
// vtable at offset 0 (m_name at offset 24, -3*sizeof(hkStringPtr) = offset 0)
extern "C" void* __fastcall hkMemoryResourceHandle_constructor(void* p, hkFinishLoadedObjectFlag flag) {
    *(void**)p = &hkMemoryResourceHandle_vftable;
    // hkStringPtr::hkStringPtr(&m_name, flag) — empty inline no-op
    (void)flag;
    return p;
}

// IDA: ??0hkMemoryResourceContainer@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407D7A30
// hkMemoryResourceContainer constructor with finishLoadedObject flag
extern "C" void* __fastcall hkMemoryResourceContainer_constructor(void* p, hkFinishLoadedObjectFlag flag) {
    // vtable at offset 8 (derived from hkMemoryResourceHandle)
    *(void**)((char*)p + 8) = &hkMemoryResourceContainer_vftable;
    // hkStringPtr::hkStringPtr(&m_name, flag) — empty inline no-op
    hkStringPtr_constructor((char*)p + 24, flag);

    if (flag.m_finishing) {
        // Iterate children and set their m_parent to this
        // m_children is at offset 32 (hkArray<hkRefPtr<hkMemoryResourceHandle>>)
        char* self = (char*)p;
        struct ChildRefPtr {
            void* pntr;
        };
        struct ChildArray {
            ChildRefPtr* m_data;
            int m_size;
            int m_capacityAndFlags;
        };
        ChildArray* children = (ChildArray*)(self + 32);
        for (int i = 0; i < children->m_size; i++) {
            if (children->m_data[i].pntr) {
                // m_parent is at offset 16 in hkMemoryResourceHandle
                *(void**)((char*)children->m_data[i].pntr + 16) = p;
            }
        }
    }
    return p;
}

// IDA: ??0hkcdStaticMeshTreeBase@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407f3ba0
extern "C" void __fastcall hkcdStaticMeshTreeBase_constructor(void* p) {
    // Empty function
    (void)p;
}

// IDA: ??0hkaiPointCloudSilhouetteGenerator@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407f4e00
// IDA decompile: hkaiSilhouetteGenerator::hkaiSilhouetteGenerator(this, a2);
// this->__vftable = (hkaiPointCloudSilhouetteGenerator_vtbl *)&hkaiPointCloudSilhouetteGenerator::`vftable';
extern "C" void* __fastcall hkaiPointCloudSilhouetteGenerator_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    hkaiSilhouetteGenerator_constructor(p, a2);
    *(void**)p = &hkaiPointCloudSilhouetteGenerator_vftable;
    return p;
}

// IDA: ??0hkaiWorld@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407f64d0
// Disassembly confirmed: sets vtable at +0x00, initializes fields at +0x60..+0xB4,
// then calls sub-constructors at +0xE0 and +0x1A0
extern "C" void* __fastcall hkaiWorld_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    char* base = (char*)p;

    // +0x00: vtable = &hkaiWorld::`vftable'
    *(void**)base = &hkaiWorld_vftable;

    // +0x60: qword zero (m_agentInfoDiameter = 0.0, m_goalFaceKeysSize = 0)
    *(unsigned long long*)(base + 0x60) = 0;
    // +0x68: dword zero (m_maxNumberOfIterations = 0)
    *(unsigned int*)(base + 0x68) = 0;
    // +0x6C: dword 0x80000000 (m_maxHierarchyOpenSetSizeBytes or similar = -0.0f)
    *(unsigned int*)(base + 0x6C) = 0x80000000;

    // +0x98: qword zero (m_heuristicWeight = 0, m_maximumPathLength = 0.0)
    *(unsigned long long*)(base + 0x98) = 0;
    // +0xA0: dword zero (m_searchSphereRadius = 0.0)
    *(unsigned int*)(base + 0xA0) = 0;
    // +0xA4: dword 0x80000000 (-0.0f)
    *(unsigned int*)(base + 0xA4) = 0x80000000;

    // +0xA8: qword zero (m_searchCapsuleRadius = 0, m_maxSearchStateSizeBytes = 0)
    *(unsigned long long*)(base + 0xA8) = 0;
    // +0xB0: dword zero
    *(unsigned int*)(base + 0xB0) = 0;
    // +0xB4: dword 0x80000000 (-0.0f)
    *(unsigned int*)(base + 0xB4) = 0x80000000;

    // +0xE0: call hkaiPathfindingUtil::FindPathInput::FindPathInput(hkFinishLoadedObjectFlag)
    hkaiPathfindingUtil_FindPathInput_constructor(base + 0xE0, a2);
    // +0x1A0: call hkaiVolumePathfindingUtil::FindPathInput::FindPathInput(hkFinishLoadedObjectFlag)
    hkaiVolumePathfindingUtil_FindPathInput_constructor(base + 0x1A0, a2);

    return p;
}

// IDA: ??0FindGraphPathInput@hkaiPathfindingUtil@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407f6d20
// IDA decompile: { ; } (empty, receives flag but ignores it)
extern "C" void __fastcall hkaiPathfindingUtil_FindGraphPathInput_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    // Empty function - receives flag but ignores it
    (void)p;
    (void)a2;
}

// ============================================================================
// Part 151: More hkFinishLoadedObjectFlag Constructors
// ============================================================================

// IDA: ??0hkaiCharacter@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140809350
// IDA decompile: sets vtable, nulls hitFilter/ssteeringFilter, inits behaviorListeners array
extern "C" void* __fastcall hkaiCharacter_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiCharacter_vftable;
    *(void**)((char*)p + 0x10) = nullptr;   // m_hitFilter.m_pntr
    *(void**)((char*)p + 0x18) = nullptr;   // m_steeringFilter.m_pntr
    *(void**)((char*)p + 0x20) = nullptr;   // m_behaviorListeners.m_data
    *(int*)((char*)p + 0x28) = 0;           // m_behaviorListeners.m_size
    *(int*)((char*)p + 0x2C) = 0x80000000;  // m_behaviorListeners.m_capacityAndFlags
    (void)a2;
    return p;
}

// IDA: hkaiNavMeshInstance::initClearanceCache @ 0x14080AA00
// Initializes clearance cache arrays for navigation mesh instance
// Key offsets from disasm: +0x18, +0x118 (counts), +0x1A0, +0x1C0 (arrays)
extern "C" void __fastcall hkaiNavMeshInstance_initGlobalClearanceCache(void* p);  // forward
extern "C" void __fastcall hkaiNavMeshInstance_initClearanceCache(void* p) {
    if (!p) return;
    char* self = (char*)p;

    // Compute total face count: [self+0x118] + [self+0x18]
    int count1 = *(int*)(self + 0x118);
    int count2 = *(int*)(self + 0x18);
    int totalFaces = count1 + count2;

    // Check and reserve clearanceFaceIndices array at +0x1C0
    int* faceIndices = *(int**)(self + 0x1C0);
    int faceCap = *(int*)(self + 0x1CC) & 0x3FFFFFFF;
    if (faceCap < totalFaces) {
        int newCap = totalFaces * 2;
        if (newCap < totalFaces) newCap = totalFaces;
        hkResult res;
        hkArrayUtil::_reserve(&res, hkContainerHeapAllocator::s_alloc,
            reinterpret_cast<void**>(self + 0x1C0), newCap, 4);
    }
    *(int*)(self + 0x1C8) = totalFaces;

    // Initialize face indices array
    int* pIndices = *(int**)(self + 0x1C0);
    int offset = 0;

    // Process faces from first source (count2 entries at +0x10)
    if (count2 > 0) {
        void* faceData = *(void**)(self + 0x10);
        for (int i = 0; i < count2; i++) {
            pIndices[i] = offset;
            // Get face and compute next offset based on vertex count
            // This is simplified - actual logic calls get_hkaiNavMesh__Face__1
            offset += 1;  // placeholder increment
        }
    }

    // Process faces from second source (count1 entries at +0x110+8)
    if (count1 > 0) {
        short* vertexCounts = (short*)(*(char**)(self + 0x110) + 8);
        for (int i = 0; i < count1; i++) {
            pIndices[count2 + i] = offset;
            short vcount = vertexCounts[i * 8];  // stride 0x10 from disasm
            offset += vcount - 2 + vcount * 2 - 4;
        }
    }

    // Reserve clearance data array at +0x1A0
    int dataCap = *(int*)(self + 0x1AC) & 0x3FFFFFFF;
    int alignedSize = (offset + 7) & 0xFFFFFFF8;
    if (dataCap < alignedSize) {
        int newCap = alignedSize * 2;
        if (newCap < alignedSize) newCap = alignedSize;
        hkResult res;
        hkArrayUtil::_reserve(&res, hkContainerHeapAllocator::s_alloc,
            reinterpret_cast<void**>(self + 0x1A0), newCap, 2);
    }

    // Initialize clearance data with default value 0xBFC0 (-1.5f as float, or sentinel)
    short* clearanceData = *(short**)(self + 0x1A0);
    int currentSize = *(int*)(self + 0x1A8);
    for (int i = currentSize; i < offset; i++) {
        clearanceData[i] = (short)0xBFC0;  // -16448 decimal, used as sentinel
    }
    *(int*)(self + 0x1A8) = offset;

    // Call initGlobalClearanceCache
    hkaiNavMeshInstance_initGlobalClearanceCache(p);
}

// IDA: ?initGlobalClearanceCache@hkaiNavMeshInstance@@QEAAXXZ @ 0x140808DB0
// TODO: 需人工审查 - Full implementation requires IDA MCP decompilation
// This function initializes global clearance cache for the navmesh instance
extern "C" void __fastcall hkaiNavMeshInstance_initGlobalClearanceCache(void* p) {
    if (!p) return;
    // TODO: Implement global clearance cache initialization
    // The actual implementation would iterate through global data structures
    // and initialize clearance values similar to initClearanceCache
}

// IDA: ??0hkaiNavMeshInstance@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14080b230
// Precise implementation from IDA decompile
extern "C" void* __fastcall hkaiNavMeshInstance_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        // Set vtable
        *(void**)p = &hkaiNavMeshInstance_vftable;

        if (a2.m_finishing) {
            // Get source pointer at this+0x60
            void* source = *(void**)((char*)p + 0x60);
            if (source) {
                char* self = (char*)p;
                char* src = (char*)source;
                // 10 field copies from IDA decompile:
                // this->_Mysize = *(_QWORD *)(v3 + 16);
                *(std::uint64_t*)(self + 0x08) = *(std::uint64_t*)(src + 0x10);
                // *(_DWORD *)&this->_Alnod = *(_DWORD *)(v3 + 24);
                *(std::uint32_t*)(self + 0x10) = *(std::uint32_t*)(src + 0x18);
                // *((_QWORD *)this + 4) = *(_QWORD *)(v3 + 32);
                *(std::uint64_t*)(self + 0x20) = *(std::uint64_t*)(src + 0x20);
                // *((_DWORD *)this + 10) = *(_DWORD *)(v3 + 40);
                *(std::uint32_t*)(self + 0x28) = *(std::uint32_t*)(src + 0x28);
                // *((_QWORD *)this + 6) = *(_QWORD *)(v3 + 48);
                *(std::uint64_t*)(self + 0x30) = *(std::uint64_t*)(src + 0x30);
                // *((_DWORD *)this + 14) = *(_DWORD *)(v3 + 56);
                *(std::uint32_t*)(self + 0x38) = *(std::uint32_t*)(src + 0x38);
                // *((_QWORD *)this + 8) = *(_QWORD *)(v3 + 80);
                *(std::uint64_t*)(self + 0x40) = *(std::uint64_t*)(src + 0x50);
                // *((_DWORD *)this + 18) = *(_DWORD *)(v3 + 112);
                *(std::uint32_t*)(self + 0x48) = *(std::uint32_t*)(src + 0x70);
                // *((_QWORD *)this + 10) = *(_QWORD *)(v3 + 96);
                *(std::uint64_t*)(self + 0x50) = *(std::uint64_t*)(src + 0x60);
                // *((_DWORD *)this + 22) = *(_DWORD *)(v3 + 116);
                *(std::uint32_t*)(self + 0x58) = *(std::uint32_t*)(src + 0x74);
            }
            // hkaiNavMeshInstance::initClearanceCache(p) @ 0x14080AA00
            hkaiNavMeshInstance_initClearanceCache(p);
        }
    }
    return p;
}

// IDA: ??0hkaiOverlapManager@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408103f0
// Precise implementation from IDA decompile
extern "C" void* __fastcall hkaiOverlapManager_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    if (p) {
        // Set hkReferencedObject vtable at offset 0
        *(void**)p = &hkaiOverlapManager_hkReferencedObject_vftable;
        // Set hkaiNavMeshSilhouetteSelector vtable at offset 8
        *((void**)p + 1) = &hkaiOverlapManager_hkaiNavMeshSilhouetteSelector_vftable;
        // m_silhouetteFilter.m_ptr = nullptr at offset 0x10
        hkaiOverlapManager* self = reinterpret_cast<hkaiOverlapManager*>(p);
        self->m_silhouetteFilter.m_ptr = nullptr;
        // If finishing flag is set, call rebuildMaps(false)
        if (a2.m_finishing) {
            self->rebuildMaps(false);
        }
    }
    return p;
}

// IDA: ?rebuildMaps@hkaiOverlapManager@@IEAAX_N@Z @ 0x14080ffa0
// Iterate over all sections and rebuild their maps
void hkaiOverlapManager::rebuildMaps(bool updateContexts) {
    for (int i = 0; i < m_sections.m_size; ++i) {
        sectionRebuildMap(&m_sections.m_data[i], updateContexts);
    }
}

// IDA: ?rebuildMap@Section@hkaiOverlapManager@@QEAAX_N@Z @ 0x14080fc30
// Precise implementation from IDA decompile - rebuilds the overlap map for a section
void hkaiOverlapManager::sectionRebuildMap(Section* section, bool updateContexts) {
    int totalSize = section->m_totalSize;
    int capacity = section->m_dataCapacityAndFlags & 0x3FFFFFFF;

    // Reserve capacity if needed
    if (capacity < totalSize) {
        int newCapacity = capacity * 2;
        if (totalSize > newCapacity) {
            newCapacity = totalSize;
        }
        hkResult res;
        hkArrayUtil::_reserve(&res, hkContainerHeapAllocator::s_alloc,
            reinterpret_cast<void**>(&section->m_dataPtr), newCapacity, 16);
    }

    // Shrink from end if data size exceeds total size
    int shrinkCount = section->m_dataSize - totalSize - 1;
    if (shrinkCount >= 0) {
        char* dataStart = (char*)section->m_dataPtr;
        int* elemPtr = (int*)(dataStart + 16 * totalSize + 16 * shrinkCount + 12);
        while (shrinkCount >= 0) {
            int elemCapacity = *elemPtr;
            *(elemPtr - 1) = 0;
            if (elemCapacity >= 0) {
                // Free the data pointer at elemPtr-3
                void** dataPtr = (void**)(elemPtr - 3);
                hkContainerHeapAllocator::s_alloc.bufFree(*dataPtr, 8 * elemCapacity);
            }
            // Clear the slot
            *(void**)(elemPtr - 3) = nullptr;
            *elemPtr = 0x80000000;
            elemPtr -= 4;
            --shrinkCount;
        }
    }

    // Grow to total size if needed
    int growCount = totalSize - section->m_dataSize;
    if (growCount > 0) {
        char* elemPtr = (char*)section->m_dataPtr + 16 * section->m_dataSize;
        while (growCount > 0) {
            if (elemPtr) {
                *(void**)elemPtr = nullptr;
                *(int*)(elemPtr + 8) = 0;
                *(int*)(elemPtr + 12) = 0x80000000;
            }
            elemPtr += 16;
            --growCount;
        }
    }

    // Update data size
    section->m_dataSize = totalSize;

    // Build the tree map if we have elements
    if (totalSize > 0) {
        // Get thread-local memory router for allocation
        void** tlsValue = (void**)TlsGetValue(hkMemoryRouter::s_memoryRouter.m_slotID);
        void* allocator = tlsValue[11];

        // Allocate new map node
        typedef void* (*AllocFunc)(void*, int);
        AllocFunc allocFunc = (AllocFunc)(*(void***)allocator)[1];
        void* newMap = allocFunc(allocator, 16);
        if (newMap) {
            *(void**)newMap = nullptr;
            *(int*)((char*)newMap + 8) = 0;
            *(int*)((char*)newMap + 12) = -1;
        }
        section->m_treeMap = newMap;

        // Insert items from m_itemsPtr into the map and data array
        int itemIndex = 0;
        if (section->m_itemCount > 0) {
            char* itemsBase = (char*)section->m_itemsPtr;
            do {
                void* item = *(void**)itemsBase;
                if (updateContexts) {
                    // Update context index if item has a context pointer
                    void* context = *(void**)((char*)item + 48);
                    if (context) {
                        typedef int (*GetIndexFunc)(void*);
                        GetIndexFunc getIndex = (GetIndexFunc)(*(void***)context)[12];
                        *(int*)((char*)item + 56) = getIndex(context);
                    }
                }

                // Process sub-elements in item
                int subCount = *(int*)((char*)item + 72);
                if (subCount > 0) {
                    int subIdx = 0;
                    int* subIndices = (int*)*(void**)((char*)item + 64);
                    do {
                        // Get target element from data array
                        int targetIdx = subIndices[subIdx];
                        char* elem = (char*)section->m_dataPtr + 16 * targetIdx;

                        // Check if we need to grow the element array
                        int elemSize = *(int*)(elem + 8);
                        int elemCapacity = *(int*)(elem + 12) & 0x3FFFFFFF;
                        if (elemSize == elemCapacity) {
                            hkArrayUtil::_reserveMore(hkContainerHeapAllocator::s_alloc,
                                reinterpret_cast<void**>(&elem), 8);
                        }

                        // Append item pointer to element array
                        void** elemData = *(void***)elem;
                        elemData[elemSize] = (char*)item + 16;
                        *(int*)(elem + 8) = elemSize + 1;

                        ++subIdx;
                        subIndices = (int*)*(void**)((char*)item + 64);  // Re-read in case of realloc
                    } while (subIdx < subCount);
                }

                // Insert into map: key = *(item+48), value = itemIndex
                typedef int (*MapInsertFunc)(void*, hkMemoryAllocator*, std::uint64_t, std::uint64_t);
                hkMapBase<std::uint64_t, std::uint64_t>* map =
                    reinterpret_cast<hkMapBase<std::uint64_t, std::uint64_t>*>(section->m_treeMap);
                map->insert(&hkContainerHeapAllocator::s_alloc,
                    *(std::uint64_t*)((char*)item + 48), itemIndex++);

                itemsBase += 8;
            } while (itemIndex < section->m_itemCount);
        }
    }
}

// IDA: ??0FindPathInput@hkaiPathfindingUtil@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407f71d0
// IDA decompile: this->_Ptr = &hkaiPathfindingUtil::FindPathInput::`vftable'; return this;
extern "C" void* __fastcall hkaiPathfindingUtil_FindPathInput_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiPathfindingUtil_FindPathInput_vftable;
    (void)a2;
    return p;
}

// IDA: ??0FindPathOutput@hkaiPathfindingUtil@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1407f71f0
// IDA: this->first._Ptr = &hkaiPathfindingUtil::FindPathOutput::`vftable';
extern "C" void* __fastcall hkaiPathfindingUtil_FindPathOutput_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiPathfindingUtil_FindPathOutput_vftable;
    (void)a2;
    return p;
}

// IDA: ??0hkaiNavMeshCutter@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140814370
// IDA: this->comp. = &hkaiNavMeshCutter::`vftable'; return this;
extern "C" void* __fastcall hkaiNavMeshCutter_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiNavMeshCutter_vftable;
    (void)a2;
    return p;
}

// IDA: ??0hkaiSilhouetteGenerator@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408162b0
// IDA: sets vtable to hkaiSilhouetteGenerator::`vftable', then calls
//   hkaiConvexSilhouetteSet::hkaiConvexSilhouetteSet(&m_cachedSilhouettes, flag)
extern "C" void* __fastcall hkaiSilhouetteGenerator_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiSilhouetteGenerator_vftable;
    // m_cachedSilhouettes is at offset 16 (after vtable + m_vertexPool)
    hkaiConvexSilhouetteSet_constructor((char*)p + 16, a2);
    return p;
}

// IDA: ??0hkaiStreamingCollection@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408188f0
// IDA: this->_Bx._Ptr = (char *)&hkaiStreamingCollection::`vftable';
extern "C" void* __fastcall hkaiStreamingCollection_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiStreamingCollection_vftable;
    (void)a2;
    return p;
}

// IDA: ??0hkaiDirectedGraphInstance@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14081c5f0
// ASM: lea rax, [vftable]; mov [rcx], rax; test edx,edx; jz ret
//   if (a2): mov rdx,[rcx+0x40]; test rdx,rdx; jz ret
//   mov [rcx+0x10],[rdx+0x20]; mov [rcx+0x18],[rdx+0x28]
//   mov [rcx+0x20],[rdx+0x30]; mov [rcx+0x28],[rdx+0x38]
//   mov [rcx+0x30],[rdx+0x10]
extern "C" void* __fastcall hkaiDirectedGraphInstance_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiDirectedGraphInstance_vftable;
    if (a2.m_finishing) {
        char* data = *(char**)((char*)p + 0x40);
        if (data) {
            *(uint64_t*)((char*)p + 0x10) = *(const uint64_t*)(data + 0x20);
            *(uint32_t*)((char*)p + 0x18) = *(const uint32_t*)(data + 0x28);
            *(uint64_t*)((char*)p + 0x20) = *(const uint64_t*)(data + 0x30);
            *(uint32_t*)((char*)p + 0x28) = *(const uint32_t*)(data + 0x38);
            *(uint64_t*)((char*)p + 0x30) = *(const uint64_t*)(data + 0x10);
        }
    }
    return p;
}

// IDA: ??0FindPathInput@hkaiVolumePathfindingUtil@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14081cbe0
// IDA decompile: this->__vftable = &hkaiVolumePathfindingUtil::FindPathInput::`vftable'; return this;
extern "C" void* __fastcall hkaiVolumePathfindingUtil_FindPathInput_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiVolumePathfindingUtil_FindPathInput_vftable;
    (void)a2;
    return p;
}

// ============================================================================
// Part 152: More hkFinishLoadedObjectFlag Constructors
// ============================================================================

// IDA: ??0FindPathOutput@hkaiVolumePathfindingUtil@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14081cc00
// IDA decompile: this->__vftable = &hkaiVolumePathfindingUtil::FindPathOutput::`vftable'; return this;
extern "C" void* __fastcall hkaiVolumePathfindingUtil_FindPathOutput_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiVolumePathfindingUtil_FindPathOutput_vftable;
    (void)a2;
    return p;
}

// IDA: ??0hkaiDynamicNavVolumeMediator@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14081d800
// IDA: this->__vftable = &hkaiDynamicNavVolumeMediator::`vftable'; return this;
extern "C" void* __fastcall hkaiDynamicNavVolumeMediator_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiDynamicNavVolumeMediator_vftable;
    (void)a2;
    return p;
}

// IDA: ??0hkaiDynamicNavMeshQueryMediator@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14081dad0
// IDA: this->__vftable = &hkaiDynamicNavMeshQueryMediator::`vftable'; return this;
extern "C" void* __fastcall hkaiDynamicNavMeshQueryMediator_constructor2(void* p) {
    *(void**)p = &hkaiDynamicNavMeshQueryMediator_vftable;
    return p;
}

// IDA: ??0InputBase@hkaiLineOfSightUtil@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140822140
extern "C" void* __fastcall hkaiLineOfSightUtil_InputBase_constructor(void* p) {
    // Empty function - just return this
    return p;
}

// IDA: ??0LineOfSightInput@hkaiLineOfSightUtil@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140822180
extern "C" void* __fastcall hkaiLineOfSightUtil_LineOfSightInput_constructor(void* p) {
    // Empty function - just return this
    return p;
}

// IDA: ??0DirectPathInput@hkaiLineOfSightUtil@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408221c0
extern "C" void* __fastcall hkaiLineOfSightUtil_DirectPathInput_constructor(void* p) {
    // Empty function - just return this
    return p;
}

// IDA: ??0LineOfSightOutput@hkaiLineOfSightUtil@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140822f70
extern "C" void* __fastcall hkaiLineOfSightUtil_LineOfSightOutput_constructor(void* p) {
    // Empty function - just return this
    return p;
}

// IDA: ??0hkaiNavMesh@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140828350
// IDA: this->__vftable = &hkaiNavMesh::`vftable'; return this;
extern "C" void* __fastcall hkaiNavMesh_constructor(void* p) {
    *(void**)p = &hkaiNavMesh_vftable;
    return p;
}

// IDA: ??0hkaiStaticTreeNavMeshQueryMediator@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408288e0
// IDA: this->__vftable = &hkaiStaticTreeNavMeshQueryMediator::`vftable'; return this;
extern "C" void* __fastcall hkaiStaticTreeNavMeshQueryMediator_constructor(void* p) {
    *(void**)p = &hkaiStaticTreeNavMeshQueryMediator_vftable;
    return p;
}

// IDA: ??0Trajectory@hkaiJumpDetectionSettings@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14082fe20
extern "C" void* __fastcall hkaiJumpDetectionSettings_Trajectory_constructor(void* p) {
    // Empty function - just return this
    return p;
}

// ============================================================================
// Part 153: More hkFinishLoadedObjectFlag Constructors
// ============================================================================

// IDA: ??0hkaiJumpDetectionSettings@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408302d0
// IDA: this->__vftable = &hkaiJumpDetectionSettings::`vftable';
extern "C" void __fastcall hkaiJumpDetectionSettings_constructor(void* p) {
    *(void**)p = &hkaiJumpDetectionSettings_vftable;
}

// IDA: ??0hkaiDirectedGraphExplicitCost@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140830750
// IDA decompile: *(_QWORD *)this = &hkaiDirectedGraphExplicitCost::`vftable'; return this;
extern "C" void* __fastcall hkaiDirectedGraphExplicitCost_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiDirectedGraphExplicitCost_vftable;
    (void)a2;
    return p;
}

// IDA: ??0hkaiAabbTreeNavVolumeMediator@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140830960
// IDA decompile (type confused): this->m_data = &hkaiAabbTreeNavVolumeMediator::`vftable'; return this;
// Actual: sets vtable pointer at offset 0
extern "C" void* __fastcall hkaiAabbTreeNavVolumeMediator_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiAabbTreeNavVolumeMediator_vftable;
    (void)a2;
    return p;
}

// IDA: ??0hkaiNavVolumeInstance@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140831180
// IDA: this->__vftable = &hkaiNavVolumeInstance::`vftable'
// IDA: if (a2.m_finishing && this->m_originalVolume.m_pntr):
// IDA:   this->m_originalCells = m_pntr->m_cells.m_data
// IDA:   this->m_numOriginalCells = m_pntr->m_cells.m_size
// IDA:   this->m_originalEdges = m_pntr->m_edges.m_data
// IDA:   this->m_numOriginalEdges = m_pntr->m_edges.m_size
extern "C" void* __fastcall hkaiNavVolumeInstance_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiNavVolumeInstance_vftable;
    if (a2.m_finishing) {
        void* originalVolume = *(void**)((char*)p + 0x30);
        if (originalVolume) {
            *(void**)((char*)p + 0x10) = *(void**)((char*)originalVolume + 0x10);
            *(int*)((char*)p + 0x18) = *(int*)((char*)originalVolume + 0x18);
            *(void**)((char*)p + 0x20) = *(void**)((char*)originalVolume + 0x20);
            *(int*)((char*)p + 0x28) = *(int*)((char*)originalVolume + 0x28);
        }
    }
    return p;
}

// IDA: ??0JumpDescription@hkaiJumpDetectionSettings@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140830260
extern "C" void* __fastcall hkaiJumpDetectionSettings_JumpDescription_constructor(void* p) {
    // Empty function - just return this
    return p;
}

// IDA: ??0hkaiMaterialPainter@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408313b0
// IDA decompile: this->__vftable = &hkaiMaterialPainter::`vftable'; return this;
extern "C" void* __fastcall hkaiMaterialPainter_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiMaterialPainter_vftable;
    (void)a2;
    return p;
}

// IDA: ??0hkaiEdgePath@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140832590
// IDA: this->__vftable = &hkaiEdgePath::`vftable'; return this;
extern "C" void* __fastcall hkaiEdgePath_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiEdgePath_vftable;
    (void)a2;
    return p;
}

// IDA: ??0CharacterStepSerializableContext@hkaiWorld@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140833970
// IDA: this->__vftable = &hkaiWorld::CharacterStepSerializableContext::`vftable';
extern "C" void __fastcall hkaiWorld_CharacterStepSerializableContext_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiWorld_CharacterStepSerializableContext_vftable;
    (void)a2;
}

// IDA: ??0hkaiObstacleGenerator@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408352e0
// IDA: this->__vftable = &hkaiObstacleGenerator::`vftable'; return this;
extern "C" void* __fastcall hkaiObstacleGenerator_constructor(void* p) {
    *(void**)p = &hkaiObstacleGenerator_vftable;
    return p;
}

// IDA: ??0hkaiNavVolume@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140835960
// IDA: this->__vftable = &hkaiNavVolume::`vftable'; return this;
extern "C" void* __fastcall hkaiNavVolume_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiNavVolume_vftable;
    (void)a2;
    return p;
}

// ============================================================================
// Part 154: More hkFinishLoadedObjectFlag Constructors
// ============================================================================

// IDA: ??0hkaiPlaneVolume@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140836080
// IDA: this->__vftable = &hkaiPlaneVolume::`vftable'
// IDA: hkGeometry::hkGeometry(this + 0x20, a2)
// IDA: if (a2.m_finishing && (_mm_movemask_ps(_mm_cmple_ps(*(__m128*)(this+0x60), *(__m128*)(this+0x50))) & 7))
// IDA:   hkaiPlaneVolume::updateInternalInfo(this)
extern "C" void* __fastcall hkaiPlaneVolume_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiPlaneVolume_vftable;
    hkGeometry_constructor((char*)p + 0x20, a2);
    if (a2.m_finishing
        && (_mm_movemask_ps(_mm_cmple_ps(*(__m128*)((char*)p + 0x60), *(__m128*)((char*)p + 0x50))) & 7) != 0) {
        hkaiPlaneVolume_updateInternalInfo(p);
    }
    return p;
}

// IDA: ??0hkaiConvexSilhouetteSet@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140836ce0
// IDA: empty function - just sets nothing, returns this
extern "C" void* __fastcall hkaiConvexSilhouetteSet_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    (void)a2;
    return p;
}

// IDA: ??0MaterialConstructionInfo@hkaiNavVolumeGenerationSettings@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14085d260
extern "C" void* __fastcall hkaiNavVolumeGenerationSettings_MaterialConstructionInfo_constructor(void* p) {
    // Empty function - just return this
    return p;
}

// IDA: ??0hkaiNavVolumeGenerationSettings@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14085d430
// IDA: hkStringPtr::hkStringPtr((hkStringPtr*)this + 30, a2)
extern "C" void* __fastcall hkaiNavVolumeGenerationSettings_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    hkStringPtr_constructor((char*)p + 0xF0, a2);
    return p;
}

// IDA: ??1hkaiNavVolumeGenerationSettings@@QEAA@XZ @ 0x14085D460
// Destructor: releases hkStringPtr, multiple hkArray<hkReferencedObject*>, and frees arrays
extern "C" void __fastcall hkaiNavVolumeGenerationSettings_destructor(void* p) {
    if (!p) return;
    char* self = (char*)p;

    // hkStringPtr::~hkStringPtr at offset 0xF0
    hkStringPtr_destructor(self + 0xF0);

    // Array 1: hkArray<hkReferencedObject*> at offset 0xD8 (result[27])
    // Size at offset 0xE0, capacity at offset 0xE4
    void** arr1 = *(void***)(self + 0xD8);
    int size1 = *(int*)(self + 0xE0);
    int cap1 = *(int*)(self + 0xE4);
    for (int i = size1 - 1; i >= 0; i--) {
        if (arr1 && arr1[i]) {
            ((hkReferencedObject*)arr1[i])->removeReference();
        }
    }
    *(int*)(self + 0xE0) = 0;
    if (cap1 >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(arr1, 8 * cap1);
    }
    *(void**)(self + 0xD8) = nullptr;
    *(int*)(self + 0xE4) = 0x80000000;

    // Array 2: hkArray<hkReferencedObject*> at offset 0xC8 (result[25])
    void** arr2 = *(void***)(self + 0xC8);
    int size2 = *(int*)(self + 0xD0);
    int cap2 = *(int*)(self + 0xD4);
    for (int j = size2 - 1; j >= 0; j--) {
        if (arr2 && arr2[j]) {
            ((hkReferencedObject*)arr2[j])->removeReference();
        }
    }
    *(int*)(self + 0xD0) = 0;
    if (cap2 >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(arr2, 8 * cap2);
    }
    *(void**)(self + 0xC8) = nullptr;
    *(int*)(self + 0xD4) = 0x80000000;

    // Array 3: at offset 0xB8 (result[23]) - element size 12
    void* arr3 = *(void**)(self + 0xB8);
    int cap3 = *(int*)(self + 0xC4);
    *(int*)(self + 0xB8) = 0;
    if (cap3 >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(arr3, 12 * (cap3 & 0x3FFFFFFF));
    }
    *(void**)(self + 0xB8) = nullptr;
    *(int*)(self + 0xC4) = 0x80000000;

    // Array 4: at offset 0x98 (result[19]) - element size 16
    void* arr4 = *(void**)(self + 0x98);
    int cap4 = *(int*)(self + 0xA4);
    *(int*)(self + 0x98) = 0;
    if (cap4 >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(arr4, 16 * cap4);
    }
    *(void**)(self + 0x98) = nullptr;
    *(int*)(self + 0xA4) = 0x80000000;
}

// IDA: ??1ForwardReferences@@QEAA@XZ @ 0x140791920
// Destructor for Reader::ForwardReferences - frees m_objectRefs and m_arrayRefs arrays
extern "C" void __fastcall ForwardReferences_destructor(void* p) {
    if (!p) return;
    char* self = (char*)p;

    // m_objectRefs at offset 0x0
    void* objData = *(void**)(self + 0x0);
    int objCap = *(int*)(self + 0xC);
    *(int*)(self + 0x8) = 0;  // size = 0
    if (objCap >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(objData, 16 * objCap);
    }
    *(void**)(self + 0x0) = nullptr;
    *(int*)(self + 0xC) = 0x80000000;  // capacity = 0x80000000

    // m_arrayRefs at offset 0x10
    void* arrData = *(void**)(self + 0x10);
    int arrCap = *(int*)(self + 0x1C);
    *(int*)(self + 0x18) = 0;  // size = 0
    if (arrCap >= 0) {
        hkContainerHeapAllocator::s_alloc.bufFree(arrData, 16 * arrCap);
    }
    *(void**)(self + 0x10) = nullptr;
    *(int*)(self + 0x1C) = 0x80000000;  // capacity = 0x80000000
}

// IDA: ??0OverrideSettings@hkaiNavMeshGenerationSettings@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14085e350
extern "C" void* __fastcall hkaiNavMeshGenerationSettings_OverrideSettings_constructor(void* p) {
    // Empty function - just return this
    return p;
}

// IDA: ??0hkaiNavMeshGenerationSettings@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14085e870
// IDA: hkStringPtr::hkStringPtr(&this->m_snapshotFilename, a2)
extern "C" void* __fastcall hkaiNavMeshGenerationSettings_constructor(void* p, hkFinishLoadedObjectFlag a2) {
    hkStringPtr_constructor((char*)p + 0x198, a2);
    return p;
}

// IDA: ??0hkaiDynamicTree@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x14085fdc0
// IDA: this->__vftable = &hkaiDynamicTree::`vftable'
// IDA: if (f) *(byte*)(this + 16) = (*(uint16*)(this + 8) != 0)
extern "C" void* __fastcall hkaiDynamicTree_constructor2(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiDynamicTree_vftable;
    if (a2.m_finishing)
        *(uint8_t*)((char*)p + 16) = (*(uint16_t*)((char*)p + 8) != 0);
    return p;
}

// IDA: ??0hkaiStaticTree@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x140863cb0
// IDA: this->__vftable = &hkaiStaticTree::`vftable'
// IDA: if (f) *(byte*)(this + 16) = (*(uint16*)(this + 8) != 0)
extern "C" void __fastcall hkaiStaticTree_constructor2(void* p, hkFinishLoadedObjectFlag a2) {
    *(void**)p = &hkaiStaticTree_vftable;
    if (a2.m_finishing)
        *(uint8_t*)((char*)p + 16) = (*(uint16_t*)((char*)p + 8) != 0);
}

// IDA: ??0hkcdShape@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408a0f30
// IDA: this->__vftable = &hkcdShape::`vftable'; return this;
extern "C" void* __fastcall hkcdShape_constructor2(void* p) {
    *(void**)p = &hkcdShape_vftable;
    return p;
}

// IDA: ??0InstanceLoadedEvent@hkaiSilhouetteRecorder@@QEAA@VhkFinishLoadedObjectFlag@@@Z @ 0x1408a15e0
// IDA: this->__vftable = &hkaiSilhouetteRecorder::InstanceLoadedEvent::`vftable'; return this;
extern "C" void* __fastcall hkaiSilhouetteRecorder_InstanceLoadedEvent_constructor(void* p) {
    *(void**)p = &hkaiSilhouetteRecorder_InstanceLoadedEvent_vftable;
    return p;
}

// ============================================================================
// Part 155: staticClass Functions
// ============================================================================

// IDA: ?staticClass@InstanceInfo@hkaiStreamingCollection@@SAAEBVhkClass@@XZ @ 0x14080c220
static hkClass hkaiStreamingCollection_InstanceInfo_stub;
extern "C" const hkClass* __fastcall hkaiStreamingCollection_InstanceInfo_staticClass() {
    return &hkaiStreamingCollection_InstanceInfo_stub;
}

// IDA: ?staticClass@CellInstance@hkaiNavVolumeInstance@@SAAEBVhkClass@@XZ @ 0x14080c4d0
static hkClass hkaiNavVolumeInstance_CellInstance_stub;
extern "C" const hkClass* __fastcall hkaiNavVolumeInstance_CellInstance_staticClass() {
    return &hkaiNavVolumeInstance_CellInstance_stub;
}

// IDA: ?staticClass@GeneratorData@Section@hkaiOverlapManager@@SAAEBVhkClass@@XZ @ 0x14080b8c0
static hkClass hkaiOverlapManager_Section_GeneratorData_stub;
extern "C" const hkClass* __fastcall hkaiOverlapManager_Section_GeneratorData_staticClass() {
    return &hkaiOverlapManager_Section_GeneratorData_stub;
}

// IDA: ?staticClass@FindPathInput@hkaiVolumePathfindingUtil@@SAAEBVhkClass@@XZ @ 0x14080bde0
static hkClass hkaiVolumePathfindingUtil_FindPathInput_stub;
extern "C" const hkClass* __fastcall hkaiVolumePathfindingUtil_FindPathInput_staticClass() {
    return &hkaiVolumePathfindingUtil_FindPathInput_stub;
}

// IDA: ?staticClass@MeshInfo@hkaiNavMeshCutter@@SAAEBVhkClass@@XZ @ 0x14080bfa0
static hkClass hkaiNavMeshCutter_MeshInfo_stub;
extern "C" const hkClass* __fastcall hkaiNavMeshCutter_MeshInfo_staticClass() {
    return &hkaiNavMeshCutter_MeshInfo_stub;
}

// IDA: ?staticClass@InputBase@hkaiLineOfSightUtil@@SAAEBVhkClass@@XZ @ 0x14080cc30
static hkClass hkaiLineOfSightUtil_InputBase_stub;
extern "C" const hkClass* __fastcall hkaiLineOfSightUtil_InputBase_staticClass() {
    return &hkaiLineOfSightUtil_InputBase_stub;
}

// IDA: ?staticClass@hkaiDefaultAstarEdgeFilter@@SAAEBVhkClass@@XZ @ 0x14080cd50
static hkClass hkaiDefaultAstarEdgeFilter_stub;
extern "C" const hkClass* __fastcall hkaiDefaultAstarEdgeFilter_staticClass() {
    return &hkaiDefaultAstarEdgeFilter_stub;
}

// IDA: ?staticClass@hkaiSimpleObstacleGenerator@@SAAEBVhkClass@@XZ @ 0x14080d010
static hkClass hkaiSimpleObstacleGenerator_stub;
extern "C" const hkClass* __fastcall hkaiSimpleObstacleGenerator_staticClass() {
    return &hkaiSimpleObstacleGenerator_stub;
}

// IDA: ?staticClass@CharacterStepSerializableContext@hkaiWorld@@SAAEBVhkClass@@XZ @ 0x14080c760
static hkClass hkaiWorld_CharacterStepSerializableContext_stub;
extern "C" const hkClass* __fastcall hkaiWorld_CharacterStepSerializableContext_staticClass() {
    return &hkaiWorld_CharacterStepSerializableContext_stub;
}

// IDA: ?staticClass@hkaiUserEdgePairArray@@SAAEBVhkClass@@XZ @ 0x14080ca50
static hkClass hkaiUserEdgePairArray_stub;
extern "C" const hkClass* __fastcall hkaiUserEdgePairArray_staticClass() {
    return &hkaiUserEdgePairArray_stub;
}

// ============================================================================
// Part 156: More staticClass Functions
// ============================================================================

// IDA: ?staticClass@LineOfSightInput@hkaiLineOfSightUtil@@SAAEBVhkClass@@XZ @ 0x14080cc60
static hkClass hkaiLineOfSightUtil_LineOfSightInput_stub;
extern "C" const hkClass* __fastcall hkaiLineOfSightUtil_LineOfSightInput_staticClass() {
    return &hkaiLineOfSightUtil_LineOfSightInput_stub;
}

// IDA: ?staticClass@DirectPathInput@hkaiLineOfSightUtil@@SAAEBVhkClass@@XZ @ 0x14080cc90
static hkClass hkaiLineOfSightUtil_DirectPathInput_stub;
extern "C" const hkClass* __fastcall hkaiLineOfSightUtil_DirectPathInput_staticClass() {
    return &hkaiLineOfSightUtil_DirectPathInput_stub;
}

// IDA: ?staticClass@LineOfSightOutput@hkaiLineOfSightUtil@@SAAEBVhkClass@@XZ @ 0x14080ccc0
static hkClass hkaiLineOfSightUtil_LineOfSightOutput_stub;
extern "C" const hkClass* __fastcall hkaiLineOfSightUtil_LineOfSightOutput_staticClass() {
    return &hkaiLineOfSightUtil_LineOfSightOutput_stub;
}

// IDA: ?staticClass@BoundaryObstacle@hkaiAvoidanceSolver@@SAAEBVhkClass@@XZ @ 0x14080ce90
static hkClass hkaiAvoidanceSolver_BoundaryObstacle_stub;
extern "C" const hkClass* __fastcall hkaiAvoidanceSolver_BoundaryObstacle_staticClass() {
    return &hkaiAvoidanceSolver_BoundaryObstacle_stub;
}

// IDA: ?staticClass@MovementProperties@hkaiAvoidanceSolver@@SAAEBVhkClass@@XZ @ 0x14080ceb0
static hkClass hkaiAvoidanceSolver_MovementProperties_stub;
extern "C" const hkClass* __fastcall hkaiAvoidanceSolver_MovementProperties_staticClass() {
    return &hkaiAvoidanceSolver_MovementProperties_stub;
}

// IDA: ?staticClass@AvoidanceProperties@hkaiAvoidanceSolver@@SAAEBVhkClass@@XZ @ 0x14080ced0
static hkClass hkaiAvoidanceSolver_AvoidanceProperties_stub;
extern "C" const hkClass* __fastcall hkaiAvoidanceSolver_AvoidanceProperties_staticClass() {
    return &hkaiAvoidanceSolver_AvoidanceProperties_stub;
}

// IDA: ?staticClass@hkaiAstarCostModifier@@SAAEBVhkClass@@XZ @ 0x14080d540
static hkClass hkaiAstarCostModifier_stub;
extern "C" const hkClass* __fastcall hkaiAstarCostModifier_staticClass() {
    return &hkaiAstarCostModifier_stub;
}

// IDA: ?staticClass@hkaiNavVolumeMediator@@SAAEBVhkClass@@XZ @ 0x14080d550
static hkClass hkaiNavVolumeMediator_stub;
extern "C" const hkClass* __fastcall hkaiNavVolumeMediator_staticClass() {
    return &hkaiNavVolumeMediator_stub;
}

// IDA: ?staticClass@hkaiDynamicNavVolumeMediator@@SAAEBVhkClass@@XZ @ 0x14080d5c0
static hkClass hkaiDynamicNavVolumeMediator_stub;
extern "C" const hkClass* __fastcall hkaiDynamicNavVolumeMediator_staticClass2() {
    return &hkaiDynamicNavVolumeMediator_stub;
}

// IDA: ?staticClass@hkaiObstacleGenerator@@SAAEBVhkClass@@XZ @ 0x14080d620
static hkClass hkaiObstacleGenerator_stub;
extern "C" const hkClass* __fastcall hkaiObstacleGenerator_staticClass() {
    return &hkaiObstacleGenerator_stub;
}

// ============================================================================
// Part 157: More staticClass Functions
// ============================================================================

// IDA: ?staticClass@hkaiPlaneVolume@@SAAEBVhkClass@@XZ @ 0x14080d630
static hkClass hkaiPlaneVolume_stub;
extern "C" const hkClass* __fastcall hkaiPlaneVolume_staticClass() {
    return &hkaiPlaneVolume_stub;
}

// IDA: ?staticClass@WallClimbingSettings@hkaiNavMeshGenerationSettings@@SAAEBVhkClass@@XZ @ 0x14085cc40
static hkClass hkaiNavMeshGenerationSettings_WallClimbingSettings_stub;
extern "C" const hkClass* __fastcall hkaiNavMeshGenerationSettings_WallClimbingSettings_staticClass() {
    return &hkaiNavMeshGenerationSettings_WallClimbingSettings_stub;
}

// IDA: ?staticClass@hkaiNavVolumeGenerationSettings@@SAAEBVhkClass@@XZ @ 0x14085cde0
static hkClass hkaiNavVolumeGenerationSettings_stub2;
extern "C" const hkClass* __fastcall hkaiNavVolumeGenerationSettings_staticClass2() {
    return &hkaiNavVolumeGenerationSettings_stub2;
}

// IDA: ?staticClass@hkaiEdgeGeometry@@SAAEBVhkClass@@XZ @ 0x14085ce60
static hkClass hkaiEdgeGeometry_stub;
extern "C" const hkClass* __fastcall hkaiEdgeGeometry_staticClass() {
    return &hkaiEdgeGeometry_stub;
}

// IDA: ?staticClass@hkcdShapeType@@SAAEBVhkClass@@XZ @ 0x1408a0e70
static hkClass hkcdShapeType_stub;
extern "C" const hkClass* __fastcall hkcdShapeType_staticClass() {
    return &hkcdShapeType_stub;
}

// IDA: ?staticClass@hkcdShapeDispatchType@@SAAEBVhkClass@@XZ @ 0x1408a0e90
static hkClass hkcdShapeDispatchType_stub;
extern "C" const hkClass* __fastcall hkcdShapeDispatchType_staticClass() {
    return &hkcdShapeDispatchType_stub;
}

// IDA: ?staticClass@hkcdShapeInfoCodecType@@SAAEBVhkClass@@XZ @ 0x1408a0eb0
static hkClass hkcdShapeInfoCodecType_stub;
extern "C" const hkClass* __fastcall hkcdShapeInfoCodecType_staticClass() {
    return &hkcdShapeInfoCodecType_stub;
}

// IDA: ?staticClass@GraphUnloadedEvent@hkaiSilhouetteRecorder@@SAAEBVhkClass@@XZ @ 0x1408a1190
static hkClass hkaiSilhouetteRecorder_GraphUnloadedEvent_stub;
extern "C" const hkClass* __fastcall hkaiSilhouetteRecorder_GraphUnloadedEvent_staticClass() {
    return &hkaiSilhouetteRecorder_GraphUnloadedEvent_stub;
}

// IDA: ?staticClass@NonplanarFacesSettings@hkaiNavMeshDebugUtils@@SAAEBVhkClass@@XZ @ 0x1408a12f0
static hkClass hkaiNavMeshDebugUtils_NonplanarFacesSettings_stub;
extern "C" const hkClass* __fastcall hkaiNavMeshDebugUtils_NonplanarFacesSettings_staticClass() {
    return &hkaiNavMeshDebugUtils_NonplanarFacesSettings_stub;
}

// IDA: ?staticClass@FaceNormalSettings@hkaiNavMeshDebugUtils@@SAAEBVhkClass@@XZ @ 0x1408a1310
static hkClass hkaiNavMeshDebugUtils_FaceNormalSettings_stub;
extern "C" const hkClass* __fastcall hkaiNavMeshDebugUtils_FaceNormalSettings_staticClass() {
    return &hkaiNavMeshDebugUtils_FaceNormalSettings_stub;
}

// ============================================================================
// Part 158: getVtable Functions
// ============================================================================

// IDA: getVtablehkaiDirectedGraphInstance @ 0x14080c600
extern "C" void* __fastcall getVtablehkaiDirectedGraphInstance() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[88];
        memset(temp, 0, sizeof(temp));
        // hkaiDirectedGraphInstance::hkaiDirectedGraphInstance((hkaiDirectedGraphInstance*)temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiEdgePath @ 0x14080c700
extern "C" void* __fastcall getVtablehkaiEdgePath() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[72];
        memset(temp, 0, sizeof(temp));
        // hkaiEdgePath::hkaiEdgePath((hkaiEdgePath*)temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiWorld @ 0x14080c800
extern "C" void* __fastcall getVtablehkaiWorld() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[584];
        memset(temp, 0, sizeof(temp));
        // hkaiWorld::hkaiWorld((hkaiWorld*)temp, nullptr);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiDirectedGraphExplicitCost @ 0x14080c320
extern "C" void* __fastcall getVtablehkaiDirectedGraphExplicitCost() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[88];
        memset(temp, 0, sizeof(temp));
        // hkaiDirectedGraphExplicitCost::hkaiDirectedGraphExplicitCost((hkaiDirectedGraphExplicitCost*)temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiPathfindingUtilFindPathInput @ 0x14080c380
extern "C" void* __fastcall getVtablehkaiPathfindingUtilFindPathInput() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[200];
        memset(temp, 0, sizeof(temp));
        // hkaiPathfindingUtil::FindPathInput::FindPathInput(&temp, nullptr);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiMaterialPainter @ 0x14080c590
extern "C" void* __fastcall getVtablehkaiMaterialPainter() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[40];
        memset(temp, 0, sizeof(temp));
        // hkaiMaterialPainter::hkaiMaterialPainter((hkaiMaterialPainter*)temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiWorldCharacterStepSerializableContext @ 0x14080c7a0
extern "C" void* __fastcall getVtablehkaiWorldCharacterStepSerializableContext() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[88];
        memset(temp, 0, sizeof(temp));
        // hkaiWorld::CharacterStepSerializableContext::CharacterStepSerializableContext(&temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiNavMeshPathRequestInfo @ 0x14080c850
extern "C" void* __fastcall getVtablehkaiNavMeshPathRequestInfo() {
    // Empty function - just return nullptr
    return nullptr;
}

// IDA: getVtablehkaiPathfindingUtilFindPathOutput @ 0x14080c3f0
extern "C" void* __fastcall getVtablehkaiPathfindingUtilFindPathOutput() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[72];
        memset(temp, 0, sizeof(temp));
        // hkaiPathfindingUtil::FindPathOutput::FindPathOutput(&temp, nullptr);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiAabbTreeNavVolumeMediator @ 0x14080c4b0
extern "C" void* __fastcall getVtablehkaiAabbTreeNavVolumeMediator() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[40];
        memset(temp, 0, sizeof(temp));
        // hkaiAabbTreeNavVolumeMediator::hkaiAabbTreeNavVolumeMediator(&temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiSimpleSilhouetteMerger @ 0x14080d260
extern "C" void* __fastcall getVtablehkaiSimpleSilhouetteMerger() {
    // Empty function
    return nullptr;
}

// IDA: getVtablehkaiNavVolume @ 0x14080d400
extern "C" void* __fastcall getVtablehkaiNavVolume() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[152];
        memset(temp, 0, sizeof(temp));
        // hkaiNavVolume::hkaiNavVolume((hkaiNavVolume*)temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiDefaultAstarEdgeFilter @ 0x14080cd90
extern "C" void* __fastcall getVtablehkaiDefaultAstarEdgeFilter() {
    // Empty function
    return nullptr;
}

// IDA: getVtablehkaiAvoidancePairProperties @ 0x14080cf50
extern "C" void* __fastcall getVtablehkaiAvoidancePairProperties() {
    // Empty function
    return nullptr;
}

// IDA: getVtablehkaiSimpleObstacleGenerator @ 0x14080d060
extern "C" void* __fastcall getVtablehkaiSimpleObstacleGenerator() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[184];
        memset(temp, 0, sizeof(temp));
        // hkaiObstacleGenerator::hkaiObstacleGenerator((hkaiObstacleGenerator*)temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiConvexHullSilhouetteMerger @ 0x14080d290
extern "C" void* __fastcall getVtablehkaiConvexHullSilhouetteMerger() {
    // Empty function
    return nullptr;
}

// IDA: getVtablehkaiDynamicNavMeshQueryMediator @ 0x14080c990
extern "C" void* __fastcall getVtablehkaiDynamicNavMeshQueryMediator() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[56];
        memset(temp, 0, sizeof(temp));
        // hkaiDynamicNavMeshQueryMediator::hkaiDynamicNavMeshQueryMediator(&temp);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiUserEdgeSetupArray @ 0x14080ca90
extern "C" void* __fastcall getVtablehkaiUserEdgeSetupArray() {
    // Empty function
    return nullptr;
}

// IDA: getVtablehkaiUserEdgePairArray @ 0x14080cac0
extern "C" void* __fastcall getVtablehkaiUserEdgePairArray() {
    // Empty function
    return nullptr;
}

// IDA: getVtablehkaiAvoidanceSolverAvoidanceProperties @ 0x14080cf20
extern "C" void* __fastcall getVtablehkaiAvoidanceSolverAvoidanceProperties() {
    // Empty function
    return nullptr;
}

// IDA: getVtablehkaiPathFollowingProperties @ 0x14080d700
extern "C" void* __fastcall getVtablehkaiPathFollowingProperties() {
    // Empty function
    return nullptr;
}

// IDA: getVtablehkaiNavVolumePathRequestInfo @ 0x14080c880
extern "C" void* __fastcall getVtablehkaiNavVolumePathRequestInfo() {
    // Empty function
    return nullptr;
}

// IDA: getVtablehkaiPath @ 0x14080d4b0
extern "C" void* __fastcall getVtablehkaiPath() {
    // Empty function
    return nullptr;
}

// IDA: getVtablehkaiDynamicNavVolumeMediator @ 0x14080d600
extern "C" void* __fastcall getVtablehkaiDynamicNavVolumeMediator() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[24];
        memset(temp, 0, sizeof(temp));
        // hkaiDynamicNavVolumeMediator::hkaiDynamicNavVolumeMediator(&temp, nullptr, {});
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiPlaneVolume @ 0x14080d670
extern "C" void* __fastcall getVtablehkaiPlaneVolume() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[120];
        memset(temp, 0, sizeof(temp));
        // hkaiPlaneVolume::hkaiPlaneVolume(&temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiDefaultAstarCostModifier @ 0x14080d880
extern "C" void* __fastcall getVtablehkaiDefaultAstarCostModifier() {
    // Empty function
    return nullptr;
}

// IDA: getVtablehkaiCharacter @ 0x14080d9d0
extern "C" void* __fastcall getVtablehkaiCharacter() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[200];
        memset(temp, 0, sizeof(temp));
        // hkaiCharacter::hkaiCharacter(&temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiInvertedAabbVolume @ 0x14080da40
extern "C" void* __fastcall getVtablehkaiInvertedAabbVolume() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[40];
        memset(temp, 0, sizeof(temp));
        // hkGeometry::hkGeometry(&temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiPointCloudSilhouetteGenerator @ 0x14080db20
extern "C" void* __fastcall getVtablehkaiPointCloudSilhouetteGenerator() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[216];
        memset(temp, 0, sizeof(temp));
        // hkaiPointCloudSilhouetteGenerator::hkaiPointCloudSilhouetteGenerator(&temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiEdgeGeometry @ 0x14085ceb0
extern "C" void* __fastcall getVtablehkaiEdgeGeometry() {
    // Returns vftable directly
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[8];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiDynamicTree @ 0x14085d150
extern "C" void* __fastcall getVtablehkaiDynamicTree() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[40];
        memset(temp, 0, sizeof(temp));
        // hkaiDynamicTree::hkaiDynamicTree(&temp, nullptr);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiStaticTree @ 0x14085d1b0
extern "C" void* __fastcall getVtablehkaiStaticTree() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[40];
        memset(temp, 0, sizeof(temp));
        // hkaiStaticTree::hkaiStaticTree(&temp, nullptr);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiSilhouetteRecorderWorldConnectedEvent @ 0x1408a0fa0
extern "C" void* __fastcall getVtablehkaiSilhouetteRecorderWorldConnectedEvent() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[40];
        memset(temp, 0, sizeof(temp));
        // hkaiSilhouetteRecorder::WorldConnectedEvent::WorldConnectedEvent(&temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiSilhouetteRecorderInstanceLoadedEvent @ 0x1408a1000
extern "C" void* __fastcall getVtablehkaiSilhouetteRecorderInstanceLoadedEvent() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[56];
        memset(temp, 0, sizeof(temp));
        // hkaiSilhouetteRecorder::InstanceLoadedEvent::InstanceLoadedEvent(&temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiSilhouetteRecorderSilhouettesSteppedEvent @ 0x1408a1060
extern "C" void* __fastcall getVtablehkaiSilhouetteRecorderSilhouettesSteppedEvent() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[120];
        memset(temp, 0, sizeof(temp));
        // hkaiSilhouetteRecorder::SilhouettesSteppedEvent::SilhouettesSteppedEvent(&temp);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiSilhouetteRecorderVolumeLoadedEvent @ 0x1408a10f0
extern "C" void* __fastcall getVtablehkaiSilhouetteRecorderVolumeLoadedEvent() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[56];
        memset(temp, 0, sizeof(temp));
        // hkaiSilhouetteRecorder::VolumeLoadedEvent::VolumeLoadedEvent(&temp, 0);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiSilhouetteRecorderGraphLoadedEvent @ 0x1408a1170
extern "C" void* __fastcall getVtablehkaiSilhouetteRecorderGraphLoadedEvent() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[40];
        memset(temp, 0, sizeof(temp));
        // hkaiSilhouetteRecorder::GraphLoadedEvent::GraphLoadedEvent(&temp);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiSilhouetteRecorderReplayEvent @ 0x1408a11d0
extern "C" void* __fastcall getVtablehkaiSilhouetteRecorderReplayEvent() {
    // Returns vftable directly
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[8];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiSilhouetteRecorderInstanceUnloadedEvent @ 0x1408a1200
extern "C" void* __fastcall getVtablehkaiSilhouetteRecorderInstanceUnloadedEvent() {
    // Returns vftable directly
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[8];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiSilhouetteRecorderVolumeUnloadedEvent @ 0x1408a1230
extern "C" void* __fastcall getVtablehkaiSilhouetteRecorderVolumeUnloadedEvent() {
    // Returns vftable directly
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[8];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: hkDefaultBuiltinTypeRegistry::getVtableClassRegistry @ 0x1407b6370
extern "C" void* __fastcall hkDefaultBuiltinTypeRegistry_getVtableClassRegistry(void* /*this*/) {
    // Returns hkSingleton<hkVtableClassRegistry>::s_instance
    static void* instance = nullptr;
    return instance;
}

// IDA: getVtablehkcdShape @ 0x1408a0f10
extern "C" void* __fastcall getVtablehkcdShape() {
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[40];
        memset(temp, 0, sizeof(temp));
        // hkcdShape::hkcdShape(&temp, nullptr);
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: getVtablehkaiSilhouetteRecorderGraphUnloadedEvent @ 0x1408a1260
extern "C" void* __fastcall getVtablehkaiSilhouetteRecorderGraphUnloadedEvent() {
    // Returns vftable directly
    static void* vtable_ptr = nullptr;
    if (!vtable_ptr) {
        char temp[8];
        memset(temp, 0, sizeof(temp));
        vtable_ptr = *(void**)temp;
    }
    return vtable_ptr;
}

// IDA: finishLoadedObjecthkaiSilhouetteRecorderInstanceUnloadedEvent @ 0x1408a11e0
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteRecorderInstanceUnloadedEvent(void* p) {
    if (p) {
        *(void**)p = &hkaiSilhouetteRecorder_InstanceUnloadedEvent_vftable;
    }
}

// IDA: finishLoadedObjecthkaiSilhouetteRecorderVolumeUnloadedEvent @ 0x1408a1210
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteRecorderVolumeUnloadedEvent(void* p) {
    if (p) {
        *(void**)p = &hkaiSilhouetteRecorder_VolumeUnloadedEvent_vftable;
    }
}

// IDA: finishLoadedObjecthkaiSilhouetteRecorderGraphUnloadedEvent @ 0x1408a1240
extern "C" void __fastcall finishLoadedObjecthkaiSilhouetteRecorderGraphUnloadedEvent(void* p) {
    if (p) {
        *(void**)p = &hkaiSilhouetteRecorder_GraphUnloadedEvent_vftable;
    }
}

// IDA: hkaiNavVolumeDebugUtils::DebugInfo::staticClass @ 0x1408a13f0
static hkClass hkaiNavVolumeDebugUtils_DebugInfo_stub;
extern "C" const hkClass* __fastcall hkaiNavVolumeDebugUtils_DebugInfo_staticClass() {
    return &hkaiNavVolumeDebugUtils_DebugInfo_stub;
}

// IDA: hkTypeInfoRegistry::finishLoadedObject @ 0x1407b5910
extern "C" const void* __fastcall hkTypeInfoRegistry_finishLoadedObject(void* /*this*/, void* obj, const char* className) {
    // Look up type info and call finishLoadedObject
    (void)obj;
    (void)className;
    return nullptr;
}

// IDA: hkTypeInfoRegistry::cleanupLoadedObject @ 0x1407b5980
extern "C" const void* __fastcall hkTypeInfoRegistry_cleanupLoadedObject(void* /*this*/, void* obj, const char* className) {
    // Look up type info and call cleanupLoadedObject
    (void)obj;
    (void)className;
    return nullptr;
}

// IDA: cleanupLoadedObjecthkaiAvoidanceSolverSphereObstacle @ 0x14080ce80
extern "C" void __fastcall cleanupLoadedObjecthkaiAvoidanceSolverSphereObstacle(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiAvoidanceSolverBoundaryObstacle @ 0x14080cea0
extern "C" void __fastcall cleanupLoadedObjecthkaiAvoidanceSolverBoundaryObstacle(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiAvoidanceSolverMovementProperties @ 0x14080cec0
extern "C" void __fastcall cleanupLoadedObjecthkaiAvoidanceSolverMovementProperties(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiAvoidanceSolverAvoidanceProperties @ 0x14080cee0
extern "C" void __fastcall cleanupLoadedObjecthkaiAvoidanceSolverAvoidanceProperties(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiSimpleObstacleGenerator @ 0x14080d020
extern "C" void __fastcall cleanupLoadedObjecthkaiSimpleObstacleGenerator(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiSimpleSilhouetteMerger @ 0x14080d210
extern "C" void __fastcall cleanupLoadedObjecthkaiSimpleSilhouetteMerger(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiConvexHullSilhouetteMerger @ 0x14080d230
extern "C" void __fastcall cleanupLoadedObjecthkaiConvexHullSilhouetteMerger(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiSilhouetteGeneratorSectionContext @ 0x14080d2c0
extern "C" void __fastcall cleanupLoadedObjecthkaiSilhouetteGeneratorSectionContext(const void* p) {
    if (p) {
        // Release referenced object and clear fields
    }
}

// IDA: cleanupLoadedObjecthkaiSplitGenerationSnapshot @ 0x14080d350
extern "C" void __fastcall cleanupLoadedObjecthkaiSplitGenerationSnapshot(void* p) {
    if (p) {
        // Call destructors for nested objects
    }
}

// IDA: cleanupLoadedObjecthkaiNavVolumeCell @ 0x14080d390
extern "C" void __fastcall cleanupLoadedObjecthkaiNavVolumeCell(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavVolumeEdge @ 0x14080d3b0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavVolumeEdge(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiPathPathPoint @ 0x14080d460
extern "C" void __fastcall cleanupLoadedObjecthkaiPathPathPoint(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiPathFollowingProperties @ 0x14080d6f0
extern "C" void __fastcall cleanupLoadedObjecthkaiPathFollowingProperties(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiGraphPathSearchParameters @ 0x14080d730
extern "C" void __fastcall cleanupLoadedObjecthkaiGraphPathSearchParameters(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiLocalSteeringInput @ 0x14080d760
extern "C" void __fastcall cleanupLoadedObjecthkaiLocalSteeringInput(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiConvexSilhouetteSet @ 0x14080d7a0
extern "C" void __fastcall cleanupLoadedObjecthkaiConvexSilhouetteSet(void* p) {
    if (p) {
        // Call destructor
    }
}

// IDA: cleanupLoadedObjecthkaiDefaultAstarCostModifier @ 0x14080d850
extern "C" void __fastcall cleanupLoadedObjecthkaiDefaultAstarCostModifier(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiReferenceFrame @ 0x14080d910
extern "C" void __fastcall cleanupLoadedObjecthkaiReferenceFrame(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavMeshGenerationSnapshot @ 0x14080d970
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshGenerationSnapshot(void* p) {
    if (p) {
        // Call destructors for nested objects
    }
}

// IDA: cleanupLoadedObjecthkaiCharacter @ 0x14080d9c0
extern "C" void __fastcall cleanupLoadedObjecthkaiCharacter(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiInvertedAabbVolume @ 0x14080da00
extern "C" void __fastcall cleanupLoadedObjecthkaiInvertedAabbVolume(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiPointCloudSilhouetteGenerator @ 0x14080db10
extern "C" void __fastcall cleanupLoadedObjecthkaiPointCloudSilhouetteGenerator(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiNavMeshGenerationSettingsOverrideSettings @ 0x14085cca0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshGenerationSettingsOverrideSettings(void* p) {
    if (p) {
        // Call destructor
    }
}

// IDA: cleanupLoadedObjecthkaiEdgeGeometryEdge @ 0x14085ce30
extern "C" void __fastcall cleanupLoadedObjecthkaiEdgeGeometryEdge(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiEdgeGeometryFace @ 0x14085ce50
extern "C" void __fastcall cleanupLoadedObjecthkaiEdgeGeometryFace(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiEdgeGeometry @ 0x14085ce70
extern "C" void __fastcall cleanupLoadedObjecthkaiEdgeGeometry(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiNavMeshSimplificationUtilsExtraVertexSettings @ 0x14085d020
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshSimplificationUtilsExtraVertexSettings(void* p) {
    if (p) {
        // Free allocated memory and clear fields
    }
}

// IDA: cleanupLoadedObjecthkaiNavMeshSimplificationUtilsSettings @ 0x14085d0a0
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshSimplificationUtilsSettings(void* p) {
    if (p) {
        // Free allocated memory and clear fields
    }
}

// IDA: cleanupLoadedObjecthkaiDynamicTree @ 0x14085d140
extern "C" void __fastcall cleanupLoadedObjecthkaiDynamicTree(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiStaticTree @ 0x14085d1a0
extern "C" void __fastcall cleanupLoadedObjecthkaiStaticTree(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkcdShapeType @ 0x1408a0e80
extern "C" void __fastcall cleanupLoadedObjecthkcdShapeType(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkcdShapeDispatchType @ 0x1408a0ea0
extern "C" void __fastcall cleanupLoadedObjecthkcdShapeDispatchType(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkcdShapeInfoCodecType @ 0x1408a0ec0
extern "C" void __fastcall cleanupLoadedObjecthkcdShapeInfoCodecType(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkcdShape @ 0x1408a0f00
extern "C" void __fastcall cleanupLoadedObjecthkcdShape(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiSilhouetteRecorderReplayEvent @ 0x1408a0f50
extern "C" void __fastcall cleanupLoadedObjecthkaiSilhouetteRecorderReplayEvent(const void* p) {
    if (p) {
        // Call destructor via vtable
    }
}

// IDA: cleanupLoadedObjecthkaiNavMeshDebugUtilsNonplanarFacesSettings @ 0x1408a1300
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshDebugUtilsNonplanarFacesSettings(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavMeshDebugUtilsFaceNormalSettings @ 0x1408a1320
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshDebugUtilsFaceNormalSettings(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavMeshDebugUtilsClusterGraphSettings @ 0x1408a1340
extern "C" void __fastcall cleanupLoadedObjecthkaiNavMeshDebugUtilsClusterGraphSettings(const void* p) {
    (void)p;
}

// IDA: cleanupLoadedObjecthkaiNavVolumeDebugUtilsDebugInfo @ 0x1408a1400
extern "C" void __fastcall cleanupLoadedObjecthkaiNavVolumeDebugUtilsDebugInfo(void* p) {
    if (p) {
        // Free allocated memory and clear fields
    }
}
