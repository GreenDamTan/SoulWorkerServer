#pragma once

// Physics and Collision System for GameServer
// IDA reconstruction - Physics.h

#include <cstdint>
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"

// Forward declarations
class CMover;
class CMoverEx;
class XArea;
class DohHavokNavMeshInstance;
struct TB_DIVERGENCE;

// ============================================================================
// Physics Types and Structures
// ============================================================================

// Attack area structure (IDA 0x1402B75A0)
// Size: 54 bytes
#pragma pack(push, 1)
struct tagATTACK_AREA {
    std::uint8_t byType;           // 0x00 - Type (0=Sphere, 1=Box)
    hkvVec3 vCenterPos;            // 0x01 - Center position (12 bytes)
    D3DXVECTOR2 vAttackerDir;      // 0x0D - Attacker direction (8 bytes)
    float fAttackerRot;            // 0x15 - Attacker rotation
    float fRadiusStart;            // 0x19 - Start radius
    float fRadiusEnd;              // 0x1D - End radius
    float fAngle;                  // 0x21 - Attack angle (half)
    float fHeightT;                // 0x25 - Top height
    float fHeightB;                // 0x29 - Bottom height
    float fSizeX;                  // 0x2D - Box size X
    float fSizeY;                  // 0x31 - Box size Y
    std::uint8_t byHitPartsIndex;  // 0x35 - Hit parts index
};
#pragma pack(pop)

// Attack range structure (IDA 0x1402C1DE0)
// Size: 36 bytes
#pragma pack(push, 1)
struct tagATTACK_RANGE {
    hkvVec3 vCenterPos;    // 0x00 - Center position (12 bytes)
    float fBoxSizeX;       // 0x0C - Box size X
    float fBoxSizeY;       // 0x10 - Box size Y
    float fRadius;         // 0x14 - Radius
    float fAngle;          // 0x18 - Angle
    float fHeight;         // 0x1C - Height
    float fStartPos;       // 0x20 - Start position offset
};
#pragma pack(pop)

// Hit collision structure for bone-based collision (IDA 0x140014F20)
// Size: 29 bytes
#pragma pack(push, 1)
struct tagHIT_COLLISION {
    VString strBoneName;        // 0x00 - Bone name (8 bytes)
    std::int32_t iBoneIndex;    // 0x08 - Bone index
    float fRadius;              // 0x0C - Collision radius
    std::uint8_t byHitParts;    // 0x10 - Hit parts flag
    hkvVec3 vBonePos;           // 0x11 - Bone position (12 bytes)
};
#pragma pack(pop)

// Hit collision data container (IDA 0x140016A50)
// Size: 40 bytes
#pragma pack(push, 1)
struct tagHIT_COLLISION_DATA {
    float fCylinderRadius;                      // 0x00 - Cylinder radius
    float fCylinderHeight;                      // 0x04 - Cylinder height
    std::vector<tagHIT_COLLISION> vHitColisions; // 0x08 - Hit collision list (32 bytes)
};
#pragma pack(pop)

// Ground status structure (IDA 0x14051BDC0)
#pragma pack(push, 1)
struct ST_GROUND_STATUS {
    std::int32_t bOnGround;     // On ground flag
    float fGroundPosZ;          // Ground Z position
    float fVelocityZ;           // Vertical velocity
    float fFallTime;            // Fall time
};
#pragma pack(pop)

// Physics settings structure
struct ST_PHYSICS_SETTINGS {
    float fGravity;             // Gravity acceleration (default: -980.0)
    float fTerminalVelocity;    // Maximum fall speed
    float fGroundTolerance;     // Ground detection tolerance
    bool bPhysicsEnabled;       // Physics simulation enabled
};

// ============================================================================
// Collision Detection Functions
// ============================================================================

// CheckCollision - Check collision between mover and attack area
// IDA 0x1402BE0D0 - CMySkillList::IsInAttackArea
bool CheckCollision(class CMoverEx* pMover, tagATTACK_AREA* pAttackArea, 
                    int nDownAttack, int bPassiveType);

// CheckCollisionPoint - Check if point is inside attack area
bool CheckCollisionPoint(const hkvVec3& vPoint, const tagATTACK_AREA* pArea);

// CheckCollisionSphere - Sphere vs sphere collision
bool CheckCollisionSphere(const hkvVec3& vCenter1, float fRadius1,
                          const hkvVec3& vCenter2, float fRadius2);

// CheckCollisionCylinderBox - Cylinder vs box collision (IDA 0x140369B60)
int CheckCollisionCylinderBox(const hkvVec3& vCylinderCenter, float fRadius,
                              const hkvVec3& vBoxCenter, const hkvVec3& vBoxSize, 
                              float fRotation);

// CheckAttackHeight - Check if attack is within height range (IDA 0x140368CE0)
bool CheckAttackHeight(class CMover* pMover, tagATTACK_AREA* pAttackArea,
                       hkvVec3& vPos, int& bCheckCylinder);

// ============================================================================
// Raycast Functions
// ============================================================================

// Raycast - Cast ray and check for intersection
// Note: Actual raycast implementation uses Havok physics engine
// IDA 0x1402F89A0, 0x14081DCC0 - Havok navmesh raycast
bool Raycast(const hkvVec3& vStart, const hkvVec3& vDirection, float fMaxDistance,
             hkvVec3& vHitPoint, hkvVec3& vHitNormal, class CMover** ppHitMover = nullptr);

// RaycastNavMesh - Raycast against navigation mesh (IDA 0x14027A6B0)
bool RaycastNavMesh(class DohHavokNavMeshInstance* pNavMesh, 
                    hkvVec3& vStart, float fTestHeight);

// ============================================================================
// Physics Height Functions
// ============================================================================

// GetPhysicsHeight - Get ground height at position (IDA 0x14036D130)
bool GetPhysicsHeight(class CMover* pMover, hkvVec3& vPos, float fTestHeight);

// GetGroundHeight - Get ground height at position using navmesh
float GetGroundHeight(class XArea* pArea, const hkvVec3& vPos);

// GetJumpHeight - Calculate jump height based on initial velocity (IDA 0x1403A2890)
float GetJumpHeight(float fInitialVelocity, float fTime);

// GetHavokCapsuleHeight - Get Havok physics capsule height (IDA 0x1403A2BC0)
float GetHavokCapsuleHeight(class CMover* pMover);

// GetHavokCapsuleRadius - Get Havok physics capsule radius (IDA 0x140276870)
float GetHavokCapsuleRadius(class CMover* pMover);

// ============================================================================
// Physics Force Functions
// ============================================================================

// ApplyForce - Apply physics force to mover
void ApplyForce(class CMover* pMover, const hkvVec3& vForce, float fDuration = 0.0f);

// ApplyImpulse - Apply instantaneous impulse
void ApplyImpulse(class CMover* pMover, const hkvVec3& vImpulse);

// ApplyKnockback - Apply knockback force (used in combat)
void ApplyKnockback(class CMover* pTarget, class CMover* pAttacker, 
                    float fForce, float fDuration);

// ApplyGravity - Apply gravity to mover
void ApplyGravity(class CMover* pMover, float fDeltaTime);

// CalculateLaunchVelocity - Calculate velocity for ballistic trajectory
hkvVec3 CalculateLaunchVelocity(const hkvVec3& vStart, const hkvVec3& vTarget, 
                                 float fGravity, float fTimeToTarget);

// ============================================================================
// Physics State Functions
// ============================================================================

// SetPhysicsEnabled - Enable/disable physics for mover (IDA 0x14052A1B0)
void SetPhysicsEnabled(class CMover* pMover, bool bEnabled, float fPosZ = 0.0f);

// IsOnGround - Check if mover is on ground
bool IsOnGround(class CMover* pMover);

// SetOnGround - Set ground state (IDA 0x14052A1B0)
void SetOnGround(class CMover* pMover, bool bOnGround, float fGroundPosZ);

// ============================================================================
// Collision Setup Functions
// ============================================================================

// SetupPhysicsAndBound - Setup physics bounds (IDA 0x140367910)
void SetupPhysicsAndBound(class CMover* pMover, float fCollisionRadius, float fCollisionHeight);

// SetHitCylinder - Set hit cylinder dimensions (IDA 0x140016BF0)
void SetHitCylinder(class CMover* pMover, float fRadius, float fHeight);

// SetHitCollisionData - Set hit collision data (IDA 0x140016BD0)
void SetHitCollisionData(class CMover* pMover, tagHIT_COLLISION_DATA* pData);

// GetAttackArea - Calculate attack area from range (IDA 0x1402C1DE0)
void GetAttackArea(class CMoverEx* pMoverEx, const hkvVec3& vPos, float fYaw,
                   std::int16_t shAttackRangeType, tagATTACK_RANGE* pAttackRange,
                   tagATTACK_AREA* pAttackArea);

// ============================================================================
// Physics Utility Functions
// ============================================================================

// IsValidPosition - Validate position bounds (IDA 0x14036DCB0)
bool IsValidPosition(const hkvVec3& vPos);
bool IsValidPosition(float fX, float fY);

// IsValidRotation - Validate rotation value (IDA 0x14036DDA0)
bool IsValidRotation(float fRot);

// CalculateDistance2D - Calculate 2D distance
float CalculateDistance2D(const hkvVec3& vPos1, const hkvVec3& vPos2);

// CalculateDistance3D - Calculate 3D distance
float CalculateDistance3D(const hkvVec3& vPos1, const hkvVec3& vPos2);

// NormalizeDirection - Normalize direction vector
hkvVec3 NormalizeDirection(const hkvVec3& vDirection);

// CalculateAngle - Calculate angle between two positions
float CalculateAngle(const hkvVec3& vFrom, const hkvVec3& vTo);

// InterpolatePosition - Linear interpolation between positions
hkvVec3 InterpolatePosition(const hkvVec3& vStart, const hkvVec3& vEnd, float fT);

// ============================================================================
// Global Physics Settings
// ============================================================================

namespace PhysicsSystem {
    // Get physics settings
    const ST_PHYSICS_SETTINGS& GetSettings();
    
    // Set physics settings
    void SetSettings(const ST_PHYSICS_SETTINGS& settings);
    
    // Initialize physics system
    void Initialize();
    
    // Shutdown physics system
    void Shutdown();
    
    // Update physics simulation
    void Update(float fDeltaTime);
}
