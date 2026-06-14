// DohHavokNavMeshInstance.h
// Havok navigation mesh instance wrapper
// Reconstructed from IDA decompilation

#pragma once

#include <cstdint>
#include <vector>
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"  // for hkvVec3

// Forward declarations
class XMaze;

// ============================================================================
// DohHavokNavMeshInstance - Havok navigation mesh instance
// ============================================================================
class DohHavokNavMeshInstance {
public:
    DohHavokNavMeshInstance();
    virtual ~DohHavokNavMeshInstance();

    // ComputePath - Calculate navigation path
    // Returns number of path points, 0 on failure
    int ComputePath(const hkvVec3& vStart, const hkvVec3& vEnd, float fRadius,
                    std::vector<hkvVec3>& vOutList, int nMaxNodes);

    // Pathfinding utilities
    bool IsPointOnNavMesh(const hkvVec3& vPos, float fRadius);
    bool GetNearestPointOnNavMesh(const hkvVec3& vPos, hkvVec3& vOut);

    // IDA: ?GetHeight@DohHavokNavMeshInstance@@QEAA_NAEAVhkvVec3@@M@Z
    // Get height at position using navmesh
    bool GetHeight(hkvVec3* vPos, float fTestHeight);

private:
    // TODO: Add Havok-specific members when implementing
    void* m_pHavokNavMesh;  // Placeholder for actual Havok nav mesh
    void* m_pHavokWorld;    // Placeholder for Havok world
};
