// DohHavokNavMeshInstance.h
// Havok navigation mesh instance wrapper
// Reconstructed from IDA decompilation

#pragma once

#include <cstdint>
#include <vector>
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"  // for hkvVec3

// Forward declarations
class XMaze;
class HavokNavMeshResource;

// hkaiWorld - Havok AI World (forward declaration)
class hkaiWorld {
public:
    // IDA: hkaiWorld::stepSilhouettes - step silhouettes for pathfinding
    void stepSilhouettes(void* pGenerator) {
        // TODO: Implement Havok AI silhouette stepping
        // This updates navigation mesh silhouettes for dynamic obstacles
    }
};

// ============================================================================
// DohHavokNavMeshInstance - Havok navigation mesh instance
// ============================================================================
class DohHavokNavMeshInstance {
public:
    DohHavokNavMeshInstance();
    virtual ~DohHavokNavMeshInstance();

    // IDA: Initialize with navmesh resource and index
    void Init(HavokNavMeshResource* pNavMesh, int nNavMeshIndex) {
        m_pHavokNavMesh = pNavMesh;
        m_nNavMeshIndex = nNavMeshIndex;
    }

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

    // IDA: ?GetUpdateFunc@DohHavokNavMeshInstance@@QEAAPEAVhkaiWorld@@XZ
    // Get the Havok AI world for silhouette operations
    hkaiWorld* GetUpdateFunc() {
        return reinterpret_cast<hkaiWorld*>(m_pHavokWorld);
    }

private:
    HavokNavMeshResource* m_pHavokNavMesh;  // Havok nav mesh resource
    void* m_pHavokWorld;    // Placeholder for Havok world
    int m_nNavMeshIndex;    // Nav mesh index
};
