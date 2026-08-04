// HavokNavMeshResource.h
// Havok Navigation Mesh Resource wrapper
// Precisely restored from IDA decompilation

#pragma once

#include <cstdint>

// Forward declarations
class hkaiNavMesh;
class hkaiNavMeshQueryMediator;

// Include Havok types
#include "../HavokTypes.h"

// ============================================================================
// HavokNavMeshResource - Holds Havok navigation mesh and query mediator
// IDA: ??0HavokNavMeshResource@@QEAA@PEAVhkaiNavMesh@@PEAVhkaiNavMeshQueryMediator@@@Z (0x1402f8eb0)
// ============================================================================
class HavokNavMeshResource : public hkReferencedObject {
public:
    HavokNavMeshResource(hkaiNavMesh* navMesh, hkaiNavMeshQueryMediator* mediator);
    virtual ~HavokNavMeshResource();

private:
    // IDA: m_navMesh - Navigation mesh instance
    hkRefPtr<hkaiNavMesh> m_navMesh;

    // IDA: m_mediator - Query mediator
    hkRefPtr<hkaiNavMeshQueryMediator> m_mediator;

    // Friend classes
    friend class DohHavokNavMeshInstance;
    friend class DohHavokResourceManager;
};
