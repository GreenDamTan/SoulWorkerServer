// HavokNavMeshResource.cpp
// Havok Navigation Mesh Resource wrapper
// Precisely restored from IDA decompilation

#include "HavokNavMeshResource.h"
#include "../HavokTypes.h"

// ============================================================================
// HavokNavMeshResource::HavokNavMeshResource
// IDA: ??0HavokNavMeshResource@@QEAA@PEAVhkaiNavMesh@@PEAVhkaiNavMeshQueryMediator@@@Z (0x1402f8eb0)
// ============================================================================
HavokNavMeshResource::HavokNavMeshResource(hkaiNavMesh* navMesh, hkaiNavMeshQueryMediator* mediator) {
    // IDA: Initialize reference counted object
    // hkReferencedObject::hkReferencedObject(this);

    // IDA: Store navMesh and mediator refs
    m_navMesh = navMesh;
    m_mediator = mediator;
}

// ============================================================================
// HavokNavMeshResource::~HavokNavMeshResource
// IDA: ??1HavokNavMeshResource@@UEAA@XZ (0x1402f8f20)
// ============================================================================
HavokNavMeshResource::~HavokNavMeshResource() {
    // IDA: hkRefPtr destructors clean up references automatically
    m_navMesh = nullptr;
    m_mediator = nullptr;
}
