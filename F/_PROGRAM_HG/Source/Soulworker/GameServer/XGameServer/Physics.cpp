// Physics and Collision System Implementation
// IDA reconstruction from GameServer.exe port 10004
// 
// Physics functions including:
// - Collision detection (CheckCollision, CheckAttackHeight)
// - Raycast (Raycast, RaycastNavMesh)
// - Physics height (GetPhysicsHeight, GetGroundHeight)
// - Force application (ApplyForce, ApplyImpulse, ApplyKnockback)
// - Physics state (SetPhysicsEnabled, SetOnGround)
// - Collision setup (SetupPhysicsAndBound, SetHitCylinder)
// 
// Note: All functions marked as verified=no until build verification
// Chinese comments preserved where applicable

#include "Physics.h"
#include "Mover.h"
#include "MoverEx.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include <cmath>
#include <algorithm>

// ============================================================================
// Global Physics Settings
// ============================================================================

namespace PhysicsSystem {
    static ST_PHYSICS_SETTINGS g_PhysicsSettings = {
        -980.0f,    // Gravity (default: -980.0 units/s^2)
        -2000.0f,   // Terminal velocity
        5.0f,       // Ground tolerance
        true        // Physics enabled
    };
    
    const ST_PHYSICS_SETTINGS& GetSettings() {
        return g_PhysicsSettings;
    }
    
    void SetSettings(const ST_PHYSICS_SETTINGS& settings) {
        g_PhysicsSettings = settings;
    }
    
    void Initialize() {
        // TODO: Initialize Havok physics engine
        // This is a stub - actual implementation requires Havok SDK
    }
    
    void Shutdown() {
        // TODO: Shutdown Havok physics engine
        // This is a stub - actual implementation requires Havok SDK
    }
    
    void Update(float fDeltaTime) {
        // TODO: Update physics simulation
        // This is a stub - actual implementation requires Havok SDK
        (void)fDeltaTime;
    }
}

// ============================================================================
// Collision Detection Functions
// ============================================================================

// IDA 0x1402BE0D0 - CMySkillList::IsInAttackArea
// Check if mover is inside attack area
bool CheckCollision(CMoverEx* pMover, tagATTACK_AREA* pAttackArea, 
                    int nDownAttack, int bPassiveType) {
    if (!pMover) {
        return false;
    }
    
    pAttackArea->byHitPartsIndex = 0;
    
    // Check if mover can be hit
    if (!pMover->IsCanHit(nDownAttack, bPassiveType)) {
        return false;
    }
    
    // Check attack decision (collision detection)
    // TODO: Implement CMover::IsAttackDecision
    // return CMover::IsAttackDecision(pMover, pAttackArea) == 0;
    
    // Stub implementation
    return true;
}

// Check if point is inside attack area
bool CheckCollisionPoint(const hkvVec3& vPoint, const tagATTACK_AREA* pArea) {
    if (!pArea) {
        return false;
    }
    
    // Check height bounds
    if (vPoint.z < pArea->fHeightB || vPoint.z > pArea->fHeightT) {
        return false;
    }
    
    if (pArea->byType == 0) {
        // Sphere collision
        hkvVec3 vDiff = vPoint - pArea->vCenterPos;
        vDiff.z = 0.0f; // 2D distance
        
        float fDistSq = vDiff.x * vDiff.x + vDiff.y * vDiff.y;
        
        // Check if within radius range
        if (fDistSq < pArea->fRadiusStart * pArea->fRadiusStart ||
            fDistSq > pArea->fRadiusEnd * pArea->fRadiusEnd) {
            return false;
        }
        
        // Check angle if specified
        if (pArea->fAngle > 0.0f && pArea->fAngle < 3.14159f) {
            // TODO: Implement angular check
            // Calculate angle between attacker direction and point direction
        }
        
        return true;
    }
    else if (pArea->byType == 1) {
        // Box collision
        hkvVec3 vDiff = vPoint - pArea->vCenterPos;
        vDiff.z = 0.0f;
        
        // TODO: Apply rotation transformation
        
        // Check if inside box
        if (std::abs(vDiff.x) <= pArea->fSizeX * 0.5f &&
            std::abs(vDiff.y) <= pArea->fSizeY * 0.5f) {
            return true;
        }
    }
    
    return false;
}

// Sphere vs sphere collision
bool CheckCollisionSphere(const hkvVec3& vCenter1, float fRadius1,
                          const hkvVec3& vCenter2, float fRadius2) {
    hkvVec3 vDiff = vCenter2 - vCenter1;
    float fDistSq = vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z;
    float fRadiusSum = fRadius1 + fRadius2;
    return fDistSq <= fRadiusSum * fRadiusSum;
}

// IDA 0x140369B60 - CMover::CollisionCylinderToBox
// Cylinder vs box collision detection
int CheckCollisionCylinderBox(const hkvVec3& vCylinderCenter, float fRadius,
                              const hkvVec3& vBoxCenter, const hkvVec3& vBoxSize, 
                              float fRotation) {
    // Calculate relative position (ignoring Z)
    hkvVec3 vPos = vCylinderCenter - vBoxCenter;
    vPos.z = 0.0f;
    
    // Apply inverse rotation to get local coordinates
    hkvMat3 matRot;
    matRot.setFromEulerAngles(0.0f, 0.0f, -fRotation);
    
    hkvVec3 vDestPos;
    vDestPos = matRot * vPos;
    
    // Calculate box bounds
    hkvVec3 vLeftTop(-vBoxSize.x * 0.5f, -vBoxSize.y * 0.5f, 0.0f);
    hkvVec3 vRightBottom(vBoxSize.x * 0.5f, vBoxSize.y * 0.5f, 0.0f);
    
    // Check if circle overlaps rectangle
    // TODO: Implement IsInRectCircle
    // This is a stub - actual implementation needed
    
    // Closest point on rectangle to circle center
    float fClosestX = std::clamp(vDestPos.x, vLeftTop.x, vRightBottom.x);
    float fClosestY = std::clamp(vDestPos.y, vLeftTop.y, vRightBottom.y);
    
    // Distance from circle center to closest point
    float fDistX = vDestPos.x - fClosestX;
    float fDistY = vDestPos.y - fClosestY;
    float fDistSq = fDistX * fDistX + fDistY * fDistY;
    
    return fDistSq <= fRadius * fRadius ? 1 : 0;
}

// IDA 0x140368CE0 - CMover::IsAttackHeight
// Check if attack is within valid height range
bool CheckAttackHeight(CMover* pMover, tagATTACK_AREA* pAttackArea,
                       hkvVec3& vPos, int& bCheckCylinder) {
    if (!pMover || !pAttackArea) {
        return true; // No collision data means no restriction
    }
    
    // Check if position is above attack area
    if (vPos.z > pAttackArea->fHeightT) {
        if (!pMover->m_pHitCollisionData) {
            return true;
        }
        bCheckCylinder = 0;
    }
    
    // Check if position is below attack area
    float fMoverHeight = pMover->m_fHitCylinderHeight;
    if (pAttackArea->fHeightB > (vPos.z + fMoverHeight)) {
        if (!pMover->m_pHitCollisionData) {
            return true;
        }
        bCheckCylinder = 0;
    }
    
    return false;
}

// ============================================================================
// Raycast Functions
// ============================================================================

// Raycast - Cast ray and check for intersection
// Note: Actual implementation uses Havok physics engine
bool Raycast(const hkvVec3& vStart, const hkvVec3& vDirection, float fMaxDistance,
             hkvVec3& vHitPoint, hkvVec3& vHitNormal, CMover** ppHitMover) {
    // TODO: Implement using Havok physics raycast
    // IDA references: 0x1402F89A0, 0x14081DCC0
    
    // Stub implementation
    // Real implementation would:
    // 1. Normalize direction
    // 2. Cast ray through Havok physics world
    // 3. Check for collisions with movers and geometry
    // 4. Return hit information
    
    (void)vStart;
    (void)vDirection;
    (void)fMaxDistance;
    (void)vHitPoint;
    (void)vHitNormal;
    (void)ppHitMover;
    
    return false;
}

// IDA 0x14027A6B0 - DohHavokNavMeshInstance::GetHeight
// Raycast against navigation mesh for height
bool RaycastNavMesh(DohHavokNavMeshInstance* pNavMesh, 
                    hkvVec3& vStart, float fTestHeight) {
    if (!pNavMesh) {
        return false;
    }
    
    // TODO: Implement Havok navmesh raycast
    // This queries the navigation mesh for ground height
    
    (void)vStart;
    (void)fTestHeight;
    
    return false;
}

// ============================================================================
// Physics Height Functions
// ============================================================================

// IDA 0x14036D130 - CMover::GetHeight
// Get ground height at position
bool GetPhysicsHeight(CMover* pMover, hkvVec3& vPos, float fTestHeight) {
    if (!pMover) {
        return false;
    }
    
    // Get area from mover
    XArea* pArea = pMover->GetArea();
    if (!pArea) {
        return false;
    }
    
    // Get navmesh instance from area
    DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    if (!pNavMesh) {
        return false;
    }
    
    // Query navmesh for height
    return RaycastNavMesh(pNavMesh, vPos, fTestHeight);
}

// Get ground height using navmesh
float GetGroundHeight(XArea* pArea, const hkvVec3& vPos) {
    if (!pArea) {
        return 0.0f;
    }
    
    // TODO: Implement using Havok navmesh
    // IDA reference: 0x14027A6B0
    
    (void)vPos;
    return 0.0f;
}

// IDA 0x1403A2890 - VJumpInfo::GetCurHeight
// Calculate current jump height
float GetJumpHeight(float fInitialVelocity, float fTime) {
    // Physics formula: h = v0 * t - 0.5 * g * t^2
    float fGravity = PhysicsSystem::GetSettings().fGravity;
    return fInitialVelocity * fTime - 0.5f * fGravity * fTime * fTime;
}

// IDA 0x1403A2BC0 - CMover::GetHavokCapsuleHeight
float GetHavokCapsuleHeight(CMover* pMover) {
    if (!pMover) {
        return 0.0f;
    }
    return pMover->m_fCapsuleHeight;
}

// IDA 0x140276870 - CMover::GetHavokCapsuleRadius
float GetHavokCapsuleRadius(CMover* pMover) {
    if (!pMover) {
        return 0.0f;
    }
    return pMover->m_fCapsuleRadius;
}

// ============================================================================
// Physics Force Functions
// ============================================================================

// Apply physics force to mover
void ApplyForce(CMover* pMover, const hkvVec3& vForce, float fDuration) {
    if (!pMover) {
        return;
    }
    
    // TODO: Implement force application
    // This should integrate with the extra movement system
    // IDA reference: 0x14036C210 (CMover::AddExtraMoving)
    
    if (fDuration > 0.0f) {
        pMover->AddExtraMoving(vForce.x, vForce.y, fDuration);
    }
    
    (void)vForce;
}

// Apply instantaneous impulse
void ApplyImpulse(CMover* pMover, const hkvVec3& vImpulse) {
    if (!pMover) {
        return;
    }
    
    // TODO: Implement impulse application
    // Impulse is force applied over very short duration
    
    (void)vImpulse;
}

// Apply knockback force (used in combat)
void ApplyKnockback(CMover* pTarget, CMover* pAttacker, 
                    float fForce, float fDuration) {
    if (!pTarget || !pAttacker) {
        return;
    }
    
    // Calculate knockback direction from attacker to target
    hkvVec3 vTargetPos = pTarget->GetPosition();
    hkvVec3 vAttackerPos = pAttacker->GetPosition();
    
    hkvVec3 vDirection = vTargetPos - vAttackerPos;
    vDirection.z = 0.0f;
    
    // Normalize
    float fLength = std::sqrt(vDirection.x * vDirection.x + 
                              vDirection.y * vDirection.y);
    if (fLength > 0.0f) {
        vDirection.x /= fLength;
        vDirection.y /= fLength;
    }
    
    // Apply force
    hkvVec3 vForce(vDirection.x * fForce, vDirection.y * fForce, 0.0f);
    ApplyForce(pTarget, vForce, fDuration);
}

// Apply gravity to mover
void ApplyGravity(CMover* pMover, float fDeltaTime) {
    if (!pMover || !PhysicsSystem::GetSettings().bPhysicsEnabled) {
        return;
    }
    
    // TODO: Implement gravity application
    // This should modify the mover's vertical velocity
    
    (void)fDeltaTime;
}

// Calculate velocity for ballistic trajectory
hkvVec3 CalculateLaunchVelocity(const hkvVec3& vStart, const hkvVec3& vTarget, 
                                 float fGravity, float fTimeToTarget) {
    hkvVec3 vDiff = vTarget - vStart;
    
    // Calculate horizontal velocity
    float fHorizontalSpeed = std::sqrt(vDiff.x * vDiff.x + vDiff.y * vDiff.y) / fTimeToTarget;
    
    // Calculate vertical velocity
    float fVerticalSpeed = (vDiff.z - 0.5f * fGravity * fTimeToTarget * fTimeToTarget) / fTimeToTarget;
    
    // Normalize horizontal direction
    float fHorizontalDist = std::sqrt(vDiff.x * vDiff.x + vDiff.y * vDiff.y);
    hkvVec3 vResult;
    
    if (fHorizontalDist > 0.0f) {
        vResult.x = (vDiff.x / fHorizontalDist) * fHorizontalSpeed;
        vResult.y = (vDiff.y / fHorizontalDist) * fHorizontalSpeed;
    } else {
        vResult.x = 0.0f;
        vResult.y = 0.0f;
    }
    
    vResult.z = fVerticalSpeed;
    
    return vResult;
}

// ============================================================================
// Physics State Functions
// ============================================================================

// IDA 0x14052A1B0 - CMover::SetOnGround
// Set ground state
void SetPhysicsEnabled(CMover* pMover, bool bEnabled, float fPosZ) {
    if (!pMover) {
        return;
    }
    
    // Set on ground flag and ground position
    pMover->m_bOnGround = bEnabled ? 1 : 0;
    pMover->m_fGroundPosZ = fPosZ;
}

// Check if mover is on ground
bool IsOnGround(CMover* pMover) {
    if (!pMover) {
        return false;
    }
    return pMover->m_bOnGround != 0;
}

// IDA 0x14052A1B0 - CMover::SetOnGround
void SetOnGround(CMover* pMover, bool bOnGround, float fGroundPosZ) {
    if (!pMover) {
        return;
    }
    
    pMover->m_bOnGround = bOnGround ? 1 : 0;
    pMover->m_fGroundPosZ = fGroundPosZ;
}

// ============================================================================
// Collision Setup Functions
// ============================================================================

// IDA 0x140367910 - CMover::SetupPhysicsAndBound
// Setup physics bounds (capsule)
void SetupPhysicsAndBound(CMover* pMover, float fCollisionRadius, float fCollisionHeight) {
    if (!pMover) {
        return;
    }
    
    // Set capsule dimensions
    pMover->m_fCapsuleRadius = fCollisionRadius;
    pMover->m_fCapsuleHeight = fCollisionHeight;
    
    // Set hit cylinder dimensions (same as capsule by default)
    pMover->m_fHitCylinderRadius = pMover->m_fCapsuleRadius;
    pMover->m_fHitCylinderHeight = pMover->m_fCapsuleHeight;
}

// IDA 0x140016BF0 - CMover::SetHitCylinder
// Set hit cylinder dimensions
void SetHitCylinder(CMover* pMover, float fRadius, float fHeight) {
    if (!pMover) {
        return;
    }
    
    pMover->m_fHitCylinderRadius = fRadius;
    pMover->m_fHitCylinderHeight = fHeight;
}

// IDA 0x140016BD0 - CMover::SetHitCollisionData
// Set hit collision data
void SetHitCollisionData(CMover* pMover, tagHIT_COLLISION_DATA* pData) {
    if (!pMover) {
        return;
    }
    
    pMover->m_pHitCollisionData = pData;
}

// IDA 0x1402C1DE0 - CMySkillList::GetAttackArea
// Calculate attack area from attack range definition
void GetAttackArea(CMoverEx* pMoverEx, const hkvVec3& vPos, float fYaw,
                   std::int16_t shAttackRangeType, tagATTACK_RANGE* pAttackRange,
                   tagATTACK_AREA* pAttackArea) {
    if (!pMoverEx || !pAttackRange || !pAttackArea) {
        return;
    }
    
    float fRadius = pAttackRange->fRadius;
    
    // Get divergence table for optional radius modification
    TB_DIVERGENCE* pDivergenceTable = pMoverEx->GetCurDivergenceTable();
    if (pDivergenceTable) {
        // Apply divergence options if applicable
        // TODO: Implement divergence option logic
        (void)pDivergenceTable;
    }
    
    // Create rotation matrix from yaw
    hkvMat3 matRot;
    matRot.setFromEulerAngles(0.0f, 0.0f, fYaw);
    
    if (shAttackRangeType == 0) {
        // Sphere type
        pAttackArea->byType = 0;
        
        // Rotate center position
        hkvVec3 vRotatedCenter = matRot * pAttackRange->vCenterPos;
        pAttackArea->vCenterPos = vRotatedCenter + vPos;
        
        // Set attack direction
        hkvVec3 vForwardDir(0.0f, -1.0f, 0.0f);
        hkvVec3 vAttackerDir = matRot * vForwardDir;
        pAttackArea->vAttackerDir = D3DXVECTOR2(vAttackerDir.x, vAttackerDir.y);
    }
    else if (shAttackRangeType == 1) {
        // Box type
        pAttackArea->byType = 1;
        
        // Rotate center position
        hkvVec3 vRotatedCenter = matRot * pAttackRange->vCenterPos;
        pAttackArea->vCenterPos = vRotatedCenter + vPos;
        
        // Set attack direction
        hkvVec3 vForwardDir(0.0f, -1.0f, 0.0f);
        hkvVec3 vAttackerDir = matRot * vForwardDir;
        pAttackArea->vAttackerDir = D3DXVECTOR2(vAttackerDir.x, vAttackerDir.y);
        
        // Set box dimensions
        pAttackArea->fSizeX = pAttackRange->fBoxSizeX;
        pAttackArea->fSizeY = pAttackRange->fBoxSizeY;
    }
    
    // Set common attack area properties
    pAttackArea->fAttackerRot = fYaw;
    pAttackArea->fRadiusStart = pAttackRange->fStartPos;
    pAttackArea->fRadiusEnd = fRadius;
    pAttackArea->fAngle = pAttackRange->fAngle * 0.5f;
    
    // Set height bounds
    pAttackArea->fHeightB = vPos.z + pAttackRange->vCenterPos.z;
    pAttackArea->fHeightT = pAttackArea->fHeightB + pAttackRange->fHeight;
    
    pAttackArea->byHitPartsIndex = 0;
}

// ============================================================================
// Physics Utility Functions
// ============================================================================

// IDA 0x14036DCB0 - CMover::IsValidPos(XVec3*)
// Validate position bounds
bool IsValidPosition(const hkvVec3& vPos) {
    // Valid range: -10,000,000 to +10,000,000
    const float fMinBound = -10000000.0f;
    const float fMaxBound = 10000000.0f;
    
    return (vPos.x >= fMinBound && vPos.x <= fMaxBound &&
            vPos.y >= fMinBound && vPos.y <= fMaxBound &&
            vPos.z >= fMinBound && vPos.z <= fMaxBound);
}

// IDA 0x14036DD40 - CMover::IsValidPos(float, float)
// Validate X/Y coordinates
bool IsValidPosition(float fX, float fY) {
    const float fMinBound = -10000000.0f;
    const float fMaxBound = 10000000.0f;
    
    return (fX >= fMinBound && fX <= fMaxBound &&
            fY >= fMinBound && fY <= fMaxBound);
}

// IDA 0x14036DDA0 - CMover::IsValidRot
// Validate rotation value
bool IsValidRotation(float fRot) {
    // Valid range: 0 to 2*PI
    return (fRot >= 0.0f && fRot <= 6.283185f);
}

// Calculate 2D distance (ignoring Z)
float CalculateDistance2D(const hkvVec3& vPos1, const hkvVec3& vPos2) {
    float fDiffX = vPos2.x - vPos1.x;
    float fDiffY = vPos2.y - vPos1.y;
    return std::sqrt(fDiffX * fDiffX + fDiffY * fDiffY);
}

// Calculate 3D distance
float CalculateDistance3D(const hkvVec3& vPos1, const hkvVec3& vPos2) {
    float fDiffX = vPos2.x - vPos1.x;
    float fDiffY = vPos2.y - vPos1.y;
    float fDiffZ = vPos2.z - vPos1.z;
    return std::sqrt(fDiffX * fDiffX + fDiffY * fDiffY + fDiffZ * fDiffZ);
}

// Normalize direction vector
hkvVec3 NormalizeDirection(const hkvVec3& vDirection) {
    float fLength = std::sqrt(vDirection.x * vDirection.x + 
                              vDirection.y * vDirection.y + 
                              vDirection.z * vDirection.z);
    
    if (fLength > 0.0f) {
        return hkvVec3(vDirection.x / fLength, 
                       vDirection.y / fLength, 
                       vDirection.z / fLength);
    }
    
    return hkvVec3(0.0f, 0.0f, 0.0f);
}

// Calculate angle between two positions
float CalculateAngle(const hkvVec3& vFrom, const hkvVec3& vTo) {
    float fDiffX = vTo.x - vFrom.x;
    float fDiffY = vTo.y - vFrom.y;
    return std::atan2(fDiffY, fDiffX);
}

// Linear interpolation between positions
hkvVec3 InterpolatePosition(const hkvVec3& vStart, const hkvVec3& vEnd, float fT) {
    fT = std::clamp(fT, 0.0f, 1.0f);
    return hkvVec3(
        vStart.x + (vEnd.x - vStart.x) * fT,
        vStart.y + (vEnd.y - vStart.y) * fT,
        vStart.z + (vEnd.z - vStart.z) * fT
    );
}
