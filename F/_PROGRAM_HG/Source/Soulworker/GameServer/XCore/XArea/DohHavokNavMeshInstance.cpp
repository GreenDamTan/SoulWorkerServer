// DohHavokNavMeshInstance.cpp
// Havok navigation mesh instance wrapper
// Precisely restored from IDA GameServer.exe decompilation

#include "DohHavokNavMeshInstance.h"
#include "../HavokTypes.h"
#include <cstring>

// Static constants from IDA
static const float HAVOK_SCALE_FACTOR = 0.01f;  // IDA: 0.0099999998
static const float HAVOK_SCALE_INV = 100.0f;    // IDA: 100.0

// ============================================================================
// DohHavokNavMeshInstance Constructor
// IDA: ??0DohHavokNavMeshInstance@@QEAA@PEAVHavokNavMeshResource@@H@Z (0x1402f7b10)
// ============================================================================
DohHavokNavMeshInstance::DohHavokNavMeshInstance()
    : m_pHavokNavMesh(nullptr)
    , m_pHavokWorld(nullptr)
    , m_nNavMeshIndex(0)
    , m_aiWorld(nullptr)
    , m_resource(nullptr)
{
    // Initialize member arrays
    // IDA: hkArray<unsigned int>::hkArray
    // IDA: m_navMeshInstance, m_mediator initialization
}

DohHavokNavMeshInstance::~DohHavokNavMeshInstance() {
    // IDA: ??1DohHavokNavMeshInstance@@UEAA@XZ (0x1402f8050)
    if (m_resource) {
        hkReferencedObject::removeReference(reinterpret_cast<hkReferencedObject*>(m_resource));
    }
    if (m_aiWorld) {
        delete m_aiWorld;
        m_aiWorld = nullptr;
    }
}

// ============================================================================
// DohHavokNavMeshInstance::ComputePath
// IDA: ?ComputePath@DohHavokNavMeshInstance@@QEBAHAEAVhkvVec3@@0MAEAV?$vector@UtagPATHFIND_POINT@@V?$allocator@UtagPATHFIND_POINT@@@std@@@std@@H@Z (0x14025e240)
// Returns: number of path points, 0 on failure
// ============================================================================
int DohHavokNavMeshInstance::ComputePath(const hkvVec3& vStart, const hkvVec3& vEnd,
                                          float fRadius, std::vector<hkvVec3>& vOutList,
                                          int nMaxNodes) {
    // IDA: Variables
    hkVector4 startPoint, endPoint;
    hkVector4 startClosestPoint, endClosestPoint;
    unsigned int startFaceKey = static_cast<unsigned int>(-1);
    unsigned int endFaceKey = static_cast<unsigned int>(-1);
    hkaiPathfindingUtil::FindPathInput input;
    hkaiPathfindingUtil::FindPathOutput output;
    bool foundPath = false;
    int nPathCount = 0;
    int nSkipCount = 0;
    hkvVec3 vPrePos;
    hkvVec3 vPos;

    // IDA: Scale radius by 0.01
    float radius = fRadius * HAVOK_SCALE_FACTOR;

    // IDA: Load start point into hkVector4 and scale
    hkVector4::load<3, 1>(&startPoint, &vStart.x);
    hkVector4::zeroComponent<3>(&startPoint);
    hkSimdReal scale;
    hkSimdReal::fromFloat(&scale, const_cast<float*>(&HAVOK_SCALE_FACTOR));
    hkVector4::mul(&startPoint, &scale);

    // IDA: Load end point into hkVector4 and scale
    hkVector4::load<3, 1>(&endPoint, &vEnd.x);
    hkVector4::zeroComponent<3>(&endPoint);
    hkSimdReal scale2;
    hkSimdReal::fromFloat(&scale2, const_cast<float*>(&HAVOK_SCALE_FACTOR));
    hkVector4::mul(&endPoint, &scale2);

    // IDA: Get closest points on navmesh
    if (!m_aiWorld) {
        return 0;
    }

    hkaiNavMeshQueryMediator* pMediator = m_aiWorld->getDynamicQueryMediator();
    if (!pMediator) {
        return 0;
    }

    // IDA: Find start face key
    startFaceKey = pMediator->getClosestPoint(&startPoint, radius, &startClosestPoint);
    if (startFaceKey == static_cast<unsigned int>(-1)) {
        return 0;
    }

    // IDA: Find end face key
    endFaceKey = pMediator->getClosestPoint(&endPoint, radius, &endClosestPoint);
    if (endFaceKey == static_cast<unsigned int>(-1)) {
        return 0;
    }

    // IDA: Initialize pathfinding input/output
    hkaiPathfindingUtil::FindPathInput::Init(&input, 1);
    hkaiPathfindingUtil::FindPathOutput::Init(&output);

    // IDA: Set start point and face key
    hkVector4::setAll(&input.m_startPoint, reinterpret_cast<hkSimdReal*>(&startClosestPoint));
    input.m_startFaceKey = startFaceKey;

    // IDA: Set goal point and face key
    input.m_goalPoints.resize(1);
    input.m_goalFaceKeys.resize(1);
    hkVector4::setAll(&input.m_goalPoints[0], reinterpret_cast<hkSimdReal*>(&endClosestPoint));
    input.m_goalFaceKeys[0] = endFaceKey;

    // IDA: Set agent diameter
    input.m_agentInfo.m_diameter = radius * 2.0f;

    // IDA: Set up vector (Z-up)
    input.m_searchParameters.m_up.set(0.0f, 0.0f, 1.0f, 0.0f);

    // IDA: Execute pathfinding
    void* pStreamingCollection = m_aiWorld->getStreamingCollection();
    hkaiPathfindingUtil::findPath(pStreamingCollection);

    // IDA: Check if path was found
    foundPath = (output.m_outputParameters.m_status == hkaiPathfindingUtil::SEARCH_SUCCEEDED);

    // IDA: Initialize previous position
    vPrePos.setZero();
    nPathCount = 0;
    nSkipCount = 0;

    if (foundPath && !output.m_pathOut.empty()) {
        nPathCount = static_cast<int>(output.m_pathOut.size());
        if (nPathCount >= nMaxNodes) {
            nPathCount = nMaxNodes;
        }

        // IDA: Convert Havok path points back to game coordinates
        for (int i = 0; i < nPathCount; ++i) {
            hkVector4 p;
            hkSimdReal invScale;
            hkSimdReal::fromFloat(&invScale, const_cast<float*>(&HAVOK_SCALE_INV));
            hkaiPath::PathPoint* pPathPoint = &output.m_pathOut[i];
            hkVector4::setMul(&p, &pPathPoint->m_position, &invScale);
            hkVector4::store<3, 1>(&p, &vPos.x);

            // IDA: Skip duplicate points
            if (vPos.x == vPrePos.x && vPos.y == vPrePos.y && vPos.z == vPrePos.z) {
                ++nSkipCount;
            } else {
                vOutList.push_back(vPos);
                vPrePos = vPos;
            }
        }
    }

    // IDA: Cleanup
    hkaiPathfindingUtil::FindPathOutput::Destroy(&output);

    return nPathCount - nSkipCount;
}

// ============================================================================
// DohHavokNavMeshInstance::IsPointOnNavMesh
// ============================================================================
bool DohHavokNavMeshInstance::IsPointOnNavMesh(const hkvVec3& vPos, float fRadius) {
    if (!m_aiWorld) return false;

    hkVector4 point;
    hkVector4 closestPoint;

    hkVector4::load<3, 1>(&point, &vPos.x);
    hkVector4::zeroComponent<3>(&point);
    hkSimdReal scale;
    hkSimdReal::fromFloat(&scale, const_cast<float*>(&HAVOK_SCALE_FACTOR));
    hkVector4::mul(&point, &scale);

    hkaiNavMeshQueryMediator* pMediator = m_aiWorld->getDynamicQueryMediator();
    if (!pMediator) return false;

    float scaledRadius = fRadius * HAVOK_SCALE_FACTOR;
    unsigned int faceKey = pMediator->getClosestPoint(&point, scaledRadius, &closestPoint);

    return faceKey != static_cast<unsigned int>(-1);
}

// ============================================================================
// DohHavokNavMeshInstance::GetNearestPointOnNavMesh
// ============================================================================
bool DohHavokNavMeshInstance::GetNearestPointOnNavMesh(const hkvVec3& vPos, hkvVec3& vOut) {
    if (!m_aiWorld) {
        vOut = vPos;
        return false;
    }

    hkVector4 point;
    hkVector4 closestPoint;

    hkVector4::load<3, 1>(&point, &vPos.x);
    hkVector4::zeroComponent<3>(&point);
    hkSimdReal scale;
    hkSimdReal::fromFloat(&scale, const_cast<float*>(&HAVOK_SCALE_FACTOR));
    hkVector4::mul(&point, &scale);

    hkaiNavMeshQueryMediator* pMediator = m_aiWorld->getDynamicQueryMediator();
    if (!pMediator) {
        vOut = vPos;
        return false;
    }

    unsigned int faceKey = pMediator->getClosestPoint(&point, 1.0f * HAVOK_SCALE_FACTOR, &closestPoint);

    if (faceKey == static_cast<unsigned int>(-1)) {
        vOut = vPos;
        return false;
    }

    // Scale back to game coordinates
    hkSimdReal invScale;
    hkSimdReal::fromFloat(&invScale, const_cast<float*>(&HAVOK_SCALE_INV));
    hkVector4 result;
    hkVector4::setMul(&result, &closestPoint, &invScale);
    hkVector4::store<3, 1>(&result, &vOut.x);

    return true;
}

// ============================================================================
// DohHavokNavMeshInstance::GetHeight
// IDA: ?GetHeight@DohHavokNavMeshInstance@@QEAA_NAEAVhkvVec3@@M@Z (0x14027A6B0)
// ============================================================================
bool DohHavokNavMeshInstance::GetHeight(hkvVec3* vPos, float fTestHeight) {
    if (!vPos || !m_aiWorld) {
        return false;
    }

    // IDA: Get height from navmesh at position
    hkVector4 point;
    hkVector4 closestPoint;

    hkVector4::load<3, 1>(&point, &vPos->x);
    hkVector4::zeroComponent<3>(&point);
    hkSimdReal scale;
    hkSimdReal::fromFloat(&scale, const_cast<float*>(&HAVOK_SCALE_FACTOR));
    hkVector4::mul(&point, &scale);

    hkaiNavMeshQueryMediator* pMediator = m_aiWorld->getDynamicQueryMediator();
    if (!pMediator) {
        return false;
    }

    float scaledRadius = fTestHeight * HAVOK_SCALE_FACTOR;
    unsigned int faceKey = pMediator->getClosestPoint(&point, scaledRadius, &closestPoint);

    if (faceKey == static_cast<unsigned int>(-1)) {
        return false;
    }

    // IDA: Update position Z from navmesh
    hkSimdReal invScale;
    hkSimdReal::fromFloat(&invScale, const_cast<float*>(&HAVOK_SCALE_INV));
    hkVector4 result;
    hkVector4::setMul(&result, &closestPoint, &invScale);
    vPos->z = result.z;

    return true;
}

// ============================================================================
// DohHavokNavMeshInstance::AddNavMeshToWorld
// IDA: ?AddNavMeshToWorld@DohHavokNavMeshInstance@@IEAAXXZ (0x1402f8180)
// ============================================================================
void DohHavokNavMeshInstance::AddNavMeshToWorld() {
    // IDA: Add navmesh instance to Havok world
    // Stub - requires full Havok integration
}

// ============================================================================
// DohHavokNavMeshInstance::GetUpdateFunc
// IDA: ?GetUpdateFunc@DohHavokNavMeshInstance@@QEAAPEAVhkaiWorld@@XZ
// ============================================================================
hkaiWorld* DohHavokNavMeshInstance::GetUpdateFunc() {
    return m_aiWorld;
}

// ============================================================================
// DohHavokNavMeshInstance::Init
// ============================================================================
void DohHavokNavMeshInstance::Init(HavokNavMeshResource* pNavMesh, int nNavMeshIndex) {
    m_pHavokNavMesh = pNavMesh;
    m_nNavMeshIndex = nNavMeshIndex;
    m_resource = pNavMesh;

    if (m_resource) {
        hkReferencedObject::addReference(reinterpret_cast<hkReferencedObject*>(m_resource));
    }
}
