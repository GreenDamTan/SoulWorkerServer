// DohHavokNavMeshInstance.h
// Havok navigation mesh instance wrapper
// Reconstructed from IDA decompilation

#pragma once

#include <cstdint>
#include <vector>
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"  // for VString, VVector3

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
    int ComputePath(const VVector3& vStart, const VVector3& vEnd, float fRadius, 
                    std::vector<VVector3>& vOutList, int nMaxNodes);

    // Pathfinding utilities
    bool IsPointOnNavMesh(const VVector3& vPos, float fRadius);
    bool GetNearestPointOnNavMesh(const VVector3& vPos, VVector3& vOut);

private:
    // TODO: Add Havok-specific members when implementing
    void* m_pHavokNavMesh;  // Placeholder for actual Havok nav mesh
    void* m_pHavokWorld;    // Placeholder for Havok world
};
