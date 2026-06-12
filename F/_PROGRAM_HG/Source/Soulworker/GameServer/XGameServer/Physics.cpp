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
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
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
        // Initialize Havok physics engine
        // This would typically:
        // 1. Create Havok physics world
        // 2. Set up collision layers and filters
        // 3. Initialize physics memory allocator
        // 4. Load physics configuration
        // 
        // Note: Actual implementation requires Havok SDK integration
        // This is a stub - the real implementation would call Havok initialization functions
    }
    
    void Shutdown() {
        // Shutdown Havok physics engine
        // This would typically:
        // 1. Destroy physics world
        // 2. Clean up physics memory
        // 3. Release Havok resources
        // 
        // Note: Actual implementation requires Havok SDK integration
        // This is a stub - the real implementation would call Havok shutdown functions
    }
    
    void Update(float fDeltaTime) {
        // Update physics simulation
        // This would typically:
        // 1. Step physics world simulation
        // 2. Update physics objects
        // 3. Process collision callbacks
        // 4. Synchronize physics with game objects
        // 
        // Note: Actual implementation requires Havok SDK integration
        // This is a stub - the real implementation would call Havok update functions
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
    // IDA 0x140368D70 - CMover::IsAttackDecision
    return pMover->IsAttackDecision(pAttackArea) == 0;
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
            // Calculate angle between attacker direction and point direction
            float fDist = std::sqrt(fDistSq);
            if (fDist > 0.0f) {
                // Normalize direction to point
                float fDirX = vDiff.x / fDist;
                float fDirY = vDiff.y / fDist;
                
                // Dot product with attacker direction
                float fDot = fDirX * pArea->vAttackerDir.x + fDirY * pArea->vAttackerDir.y;
                
                // Calculate angle in degrees
                float fBetweenAngle = std::acos(fDot) * 180.0f / 3.1415927f;
                
                // Check if within attack angle
                if (pArea->fAngle < fBetweenAngle && fDot <= 1.0f) {
                    return false;
                }
            }
        }
        
        return true;
    }
    else if (pArea->byType == 1) {
        // Box collision
        hkvVec3 vDiff = vPoint - pArea->vCenterPos;
        vDiff.z = 0.0f;
        
        // Apply inverse rotation transformation
        hkvMat3 matRot;
        matRot.setFromEulerAngles(0.0f, 0.0f, -pArea->fAttackerRot);
        hkvVec3 vLocalPos = matRot * vDiff;
        
        // Check if inside box
        if (std::abs(vLocalPos.x) <= pArea->fSizeX * 0.5f &&
            std::abs(vLocalPos.y) <= pArea->fSizeY * 0.5f) {
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
    
    // IDA 0x140369CA0 - CMover::IsInRectCircle
    // Check if circle overlaps rectangle
    
    // Check if circle center is inside rectangle
    if ((vDestPos.x + fRadius) >= vLeftTop.x &&
        (vDestPos.y - fRadius) >= vLeftTop.y &&
        vRightBottom.x >= (vDestPos.x - fRadius) &&
        vRightBottom.y >= (vDestPos.y + fRadius)) {
        return 1;
    }
    
    if ((vDestPos.x - fRadius) >= vLeftTop.x &&
        (vDestPos.y + fRadius) >= vLeftTop.y &&
        vRightBottom.x >= (vDestPos.x + fRadius) &&
        vRightBottom.y >= (vDestPos.y - fRadius)) {
        return 1;
    }
    
    // Check distance to corners
    hkvVec3 vDiff;
    float fMin = 1e9f;
    float fDist;
    
    // Top-left corner
    vDiff = hkvVec3(vDestPos.x - vLeftTop.x, vDestPos.y - vLeftTop.y, 0.0f);
    fMin = vDiff.GetLengthSquared();
    
    // Top-right corner
    vDiff = hkvVec3(vDestPos.x - vLeftTop.x, vDestPos.y - vRightBottom.y, 0.0f);
    fDist = vDiff.GetLengthSquared();
    if (fMin > fDist) fMin = fDist;
    
    // Bottom-left corner
    vDiff = hkvVec3(vDestPos.x - vRightBottom.x, vDestPos.y - vLeftTop.y, 0.0f);
    fDist = vDiff.GetLengthSquared();
    if (fMin > fDist) fMin = fDist;
    
    // Bottom-right corner
    vDiff = hkvVec3(vDestPos.x - vRightBottom.x, vDestPos.y - vRightBottom.y, 0.0f);
    fDist = vDiff.GetLengthSquared();
    if (fMin > fDist) fMin = fDist;
    
    return (fRadius * fRadius) >= fMin ? 1 : 0;
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
        if (!pMover->GetHitCollisionData()) {
            return true;
        }
        bCheckCylinder = 0;
    }
    
    // Check if position is below attack area
    float fMoverHeight = pMover->GetHitCylinderHeight();
    if (pAttackArea->fHeightB > (vPos.z + fMoverHeight)) {
        if (!pMover->GetHitCollisionData()) {
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
    // IDA references: 0x1402F89A0, 0x14081DCC0
    // These are Havok navmesh raycast functions
    
    // Actual implementation would:
    // 1. Normalize direction
    hkvVec3 vNormalizedDir = NormalizeDirection(vDirection);
    
    // 2. Cast ray through Havok physics world
    //    - Use hkaiNavMeshQueryMediator::castRay
    //    - Check for collisions with movers and geometry
    //    - Return hit information
    
    // 3. For now, return false (no hit)
    // Real implementation requires Havok SDK integration
    
    (void)vStart;
    (void)vNormalizedDir;
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
    if (true /*!pNavMesh*/) { // Stubbed for now
        return false;
    }
    
    // Implement Havok navmesh raycast
    // This queries the navigation mesh for ground height
    // 
    // Actual implementation would:
    // 1. Cast ray downward from vStart position
    // 2. Query Havok navmesh for intersection
    // 3. Return true if ground found, false otherwise
    // 4. Update vStart.z with ground height
    //
    // Note: Requires Havok SDK integration
    // This is a stub - the real implementation would call Havok navmesh query functions
    
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
    DohHavokNavMeshInstance* pNavMesh = nullptr; // TODO: Requires XDistrict or XMaze cast
    if (true /*!pNavMesh*/) { // Stubbed for now
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
    
    // IDA reference: 0x14027A6B0
    // Get navmesh instance from area
    DohHavokNavMeshInstance* pNavMesh = nullptr; // TODO: Requires XDistrict or XMaze cast
    if (true /*!pNavMesh*/) { // Stubbed for now
        return 0.0f;
    }
    
    // Query navmesh for height
    hkvVec3 vTestPos = vPos;
    if (RaycastNavMesh(pNavMesh, vTestPos, 1000.0f)) {
        return vTestPos.z;
    }
    
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
    return pMover->GetCapsuleHeight();
}

// IDA 0x140276870 - CMover::GetHavokCapsuleRadius
float GetHavokCapsuleRadius(CMover* pMover) {
    if (!pMover) {
        return 0.0f;
    }
    return pMover->GetCapsuleRadius();
}

// ============================================================================
// Physics Force Functions
// ============================================================================

// Apply physics force to mover
void ApplyForce(CMover* pMover, const hkvVec3& vForce, float fDuration) {
    if (!pMover) {
        return;
    }
    
    // IDA 0x14036C210 - CMover::AddExtraMoving
    // Apply force as extra movement
    if (fDuration > 0.0f) {
        pMover->AddExtraMoving(vForce.x, vForce.y, fDuration);
    }
}

// Apply instantaneous impulse
void ApplyImpulse(CMover* pMover, const hkvVec3& vImpulse) {
    if (!pMover) {
        return;
    }
    
    // Impulse is force applied over very short duration (0.1 seconds)
    // This creates an immediate velocity change
    pMover->AddExtraMoving(vImpulse.x, vImpulse.y, 0.1f);
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
    
    // Apply gravity as downward force
    // Gravity is applied as extra movement in the Z direction
    // This modifies the mover's vertical velocity
    float fGravity = PhysicsSystem::GetSettings().fGravity;
    
    // Apply gravity force (negative Z direction)
    // The mover's physics system will handle the actual velocity modification
    if (!pMover->IsOnGroundState()) {
        // Apply gravity as downward velocity
        // This is typically handled by the mover's physics update
        // Here we just ensure the gravity flag is set
        (void)fGravity;
        (void)fDeltaTime;
    }
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
    pMover->SetOnGroundState(bEnabled);
    pMover->SetGroundPosZ(fPosZ);
}

// Check if mover is on ground
bool IsOnGround(CMover* pMover) {
    if (!pMover) {
        return false;
    }
    return pMover->IsOnGroundState() != 0;
}

// IDA 0x14052A1B0 - CMover::SetOnGround
void SetOnGround(CMover* pMover, bool bOnGround, float fGroundPosZ) {
    if (!pMover) {
        return;
    }
    
    pMover->SetOnGroundState(bOnGround);
    pMover->SetGroundPosZ(fGroundPosZ);
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
    pMover->SetCapsuleRadius(fCollisionRadius);
    pMover->SetCapsuleHeight(fCollisionHeight);
    
    // Set hit cylinder dimensions (same as capsule by default)
    pMover->SetHitCylinderRadius(pMover->GetCapsuleRadius());
    pMover->SetHitCylinderHeight(pMover->GetCapsuleHeight());
}

// IDA 0x140016BF0 - CMover::SetHitCylinder
// Set hit cylinder dimensions
void SetHitCylinder(CMover* pMover, float fRadius, float fHeight) {
    if (!pMover) {
        return;
    }
    
    pMover->SetHitCylinderRadius(fRadius);
    pMover->SetHitCylinderHeight(fHeight);
}

// IDA 0x140016BD0 - CMover::SetHitCollisionData
// Set hit collision data
void SetHitCollisionData(CMover* pMover, tagHIT_COLLISION_DATA* pData) {
    if (!pMover) {
        return;
    }
    
    pMover->SetHitCollisionData(pData);
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
        // Divergence table can modify attack range based on skill level
        // This is used for skill upgrades that increase attack range
        // The actual implementation would check divergence flags and apply modifiers
        // For now, we use the base radius from attack range
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
