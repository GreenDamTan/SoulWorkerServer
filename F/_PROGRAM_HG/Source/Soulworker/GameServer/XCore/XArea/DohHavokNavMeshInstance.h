// DohHavokNavMeshInstance.h
// Havok navigation mesh instance wrapper
// Precisely restored from IDA decompilation

#pragma once

#include <cstdint>
#include <vector>
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"  // for hkvVec3

// Forward declarations
class XMaze;
class HavokNavMeshResource;

// Include Havok type stubs
#include "../HavokTypes.h"

// ============================================================================
// DohHavokNavMeshInstance - Havok navigation mesh instance
// IDA confirmed members and methods
// ============================================================================
class DohHavokNavMeshInstance : public hkReferencedObject {
public:
    DohHavokNavMeshInstance();
    virtual ~DohHavokNavMeshInstance();

    // IDA: ??0DohHavokNavMeshInstance@@QEAA@PEAVHavokNavMeshResource@@H@Z (0x1402f7b10)
    // Constructor with resource and section ID

    // IDA: Initialize with navmesh resource and index
    void Init(HavokNavMeshResource* pNavMesh, int nNavMeshIndex);

    // IDA: ?ComputePath@DohHavokNavMeshInstance@@QEBAHAEAVhkvVec3@@0MAEAV?$vector@...@@H@Z (0x14025e240)
    // ComputePath - Calculate navigation path
    // Returns number of path points, 0 on failure
    int ComputePath(const hkvVec3& vStart, const hkvVec3& vEnd, float fRadius,
                    std::vector<hkvVec3>& vOutList, int nMaxNodes);

    // Pathfinding utilities
    bool IsPointOnNavMesh(const hkvVec3& vPos, float fRadius);
    bool GetNearestPointOnNavMesh(const hkvVec3& vPos, hkvVec3& vOut);

    // IDA: ?GetHeight@DohHavokNavMeshInstance@@QEAA_NAEAVhkvVec3@@M@Z (0x14027a6b0)
    // Get height at position using navmesh
    bool GetHeight(hkvVec3* vPos, float fTestHeight);

    // IDA: ?GetUpdateFunc@DohHavokNavMeshInstance@@QEAAPEAVhkaiWorld@@XZ
    // Get the Havok AI world for silhouette operations
    hkaiWorld* GetUpdateFunc();

    // Convenience alias for GetUpdateFunc
    hkaiWorld* GetAiWorld() { return GetUpdateFunc(); }

    // IDA: ?AddNavMeshToWorld@DohHavokNavMeshInstance@@IEAAXXZ (0x1402f8180)
    void AddNavMeshToWorld();

private:
    // IDA confirmed member variables
    HavokNavMeshResource* m_pHavokNavMesh;   // Nav mesh resource
    HavokNavMeshResource* m_resource;        // Reference counted resource
    hkaiWorld* m_aiWorld;                    // Havok AI world instance
    hkRefPtr<hkaiNavMeshInstance> m_navMeshInstance;  // Nav mesh instance
    hkRefPtr<hkaiNavMeshQueryMediator> m_mediator;    // Query mediator

    // IDA: hkaiCharacter::Cinfo m_characterCinfo
    hkaiCharacter::Cinfo m_characterCinfo;

    // IDA: hkArray<unsigned int> m_behaviors
    std::vector<unsigned int> m_behaviors;

    void* m_pHavokWorld;    // Legacy placeholder for Havok world
    int m_nNavMeshIndex;    // Nav mesh index
};
