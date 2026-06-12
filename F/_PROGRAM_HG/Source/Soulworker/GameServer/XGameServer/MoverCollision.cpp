// Mover Collision Detection Methods
// IDA reconstruction from GameServer.exe port 10004
// 
// Collision detection functions for CMover class:
// - IsAttackDecision: Main collision detection for attack areas
// - CollisionShereToLine: Sphere vs line segment collision
// - CollisionCylinderToBox: Cylinder vs box collision
// - FindLineCircleIntersections: Line-circle intersection test
// - IsInRectCircle: Rectangle-circle overlap test
// - GetBoneCurrentWorldSpaceTranslation: Get bone world position
// 
// Note: All functions marked as verified=no until build verification

#include "Mover.h"
#include "Physics.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include <cmath>
#include <algorithm>

// Forward declarations for Vision Engine types
struct VAnimationInfo;

// IDA 0x140368D70 - CMover::IsAttackDecision
// Main collision detection for attack areas
std::uint8_t CMover::IsAttackDecision(tagATTACK_AREA* pAttackArea) {
    if (!pAttackArea) {
        return 1;
    }
    
    // Get mover position
    hkvVec3 vMyPosition = GetPosition();
    
    // Check height bounds
    int bCheckCylinder = 1;
    if (IsAttackHeight(pAttackArea, vMyPosition, bCheckCylinder)) {
        return 1;
    }
    
    // Initialize line segment variables for angular collision
    hkvVec3 vLineLeftS, vLineLeftE, vLineRightS, vLineRightE;
    hkvVec3 vBoxSize(pAttackArea->fSizeX, pAttackArea->fSizeY, 0.0f);
    
    // Calculate line segments for angular attack areas
    if (pAttackArea->byType == 0) {
        // Sphere type - calculate left and right boundary lines
        hkvVec3 vDir(pAttackArea->vAttackerDir.x, pAttackArea->vAttackerDir.y, 0.0f);
        
        // Left boundary line
        hkvMat3 matRotLeft;
        matRotLeft.setFromEulerAngles(0.0f, 0.0f, -pAttackArea->fAngle);
        hkvVec3 vDirLeft = matRotLeft * vDir;
        
        vLineLeftS = pAttackArea->vCenterPos + vDirLeft * pAttackArea->fRadiusStart;
        vLineLeftE = pAttackArea->vCenterPos + vDirLeft * pAttackArea->fRadiusEnd;
        
        // Right boundary line
        hkvMat3 matRotRight;
        matRotRight.setFromEulerAngles(0.0f, 0.0f, pAttackArea->fAngle);
        hkvVec3 vDirRight = matRotRight * vDir;
        
        vLineRightS = pAttackArea->vCenterPos + vDirRight * pAttackArea->fRadiusStart;
        vLineRightE = pAttackArea->vCenterPos + vDirRight * pAttackArea->fRadiusEnd;
    }
    
    std::uint8_t byResult = 1;
    float fMinDistance = 1e9f;
    pAttackArea->byHitPartsIndex = 0;
    
    // Check cylinder collision
    if (bCheckCylinder == 1 && GetHitCylinderRadius() > 0.0f) {
        float fDiffX = vMyPosition.x - pAttackArea->vCenterPos.x;
        float fDiffY = vMyPosition.y - pAttackArea->vCenterPos.y;
        float fDistance = std::sqrt(fDiffX * fDiffX + fDiffY * fDiffY);
        
        if (pAttackArea->byType == 1) {
            // Box collision
            if (CollisionCylinderToBox(&vMyPosition, GetHitCylinderRadius(), 
                                       &pAttackArea->vCenterPos, &vBoxSize, pAttackArea->fAttackerRot)) {
                fMinDistance = fDistance;
                byResult = 0;
            }
        }
        else if (pAttackArea->byType == 2) {
            // Simple sphere collision
            if ((pAttackArea->fRadiusEnd + GetHitCylinderRadius()) >= fDistance) {
                fMinDistance = fDistance;
                byResult = 0;
            }
        }
        else {
            // Sphere collision with angle check
            if (fDistance >= (pAttackArea->fRadiusStart - GetHitCylinderRadius()) &&
                (pAttackArea->fRadiusEnd + GetHitCylinderRadius()) >= fDistance) {
                
                if (pAttackArea->fAngle < 180.0f) {
                    // Check angle
                    D3DXVECTOR2 vDirToTarget(fDiffX / fDistance, fDiffY / fDistance);
                    float fDot = D3DXVec2Dot(&vDirToTarget, &pAttackArea->vAttackerDir);
                    float fBetweenAngle = std::acos(fDot) * 180.0f / 3.1415927f;
                    
                    if (pAttackArea->fAngle < fBetweenAngle && fDot <= 1.0f) {
                        // Check collision with boundary lines
                        if (CollisionShereToLine(&vMyPosition, GetHitCylinderRadius(), &vLineLeftS, &vLineLeftE) ||
                            CollisionShereToLine(&vMyPosition, GetHitCylinderRadius(), &vLineRightS, &vLineRightE)) {
                            fMinDistance = fDistance;
                            byResult = 0;
                        }
                    }
                    else {
                        fMinDistance = fDistance;
                        byResult = 0;
                    }
                }
                else {
                    fMinDistance = fDistance;
                    byResult = 0;
                }
            }
        }
    }
    
    // Check bone collision if available
    if (GetHitCollisionData()) {
        hkvVec3 vBonePos;
        int idx = 0;
        
        for (size_t i = 0; i < GetHitCollisionData()->vHitColisions.size(); ++i) {
            tagHIT_COLLISION* hitCollision = &GetHitCollisionData()->vHitColisions[i];
            
            // Get bone world position
            GetBoneCurrentWorldSpaceTranslation(idx++, &hitCollision->vBonePos, &vBonePos);
            
            float fDiffX = vBonePos.x - pAttackArea->vCenterPos.x;
            float fDiffY = vBonePos.y - pAttackArea->vCenterPos.y;
            float fRadius = hitCollision->fRadius;
            
            if (pAttackArea->byType == 2) {
                // 3D sphere collision
                float fDiffZ = vBonePos.z - pAttackArea->vCenterPos.z;
                float fDist = std::sqrt(fDiffX * fDiffX + fDiffY * fDiffY + fDiffZ * fDiffZ);
                
                if ((pAttackArea->fRadiusEnd + fRadius) >= fDist) {
                    if (fMinDistance > fDist) {
                        pAttackArea->byHitPartsIndex = hitCollision->byHitParts;
                        fMinDistance = fDist;
                    }
                    byResult = 0;
                }
            }
            else if (pAttackArea->byType == 1) {
                // Box collision
                float fDiffZ = vBonePos.z - pAttackArea->vCenterPos.z;
                float fDist = std::sqrt(fDiffX * fDiffX + fDiffY * fDiffY + fDiffZ * fDiffZ);
                
                if (CollisionCylinderToBox(&vBonePos, fRadius, 
                                          &pAttackArea->vCenterPos, &vBoxSize, pAttackArea->fAttackerRot)) {
                    if (fMinDistance > fDist) {
                        pAttackArea->byHitPartsIndex = hitCollision->byHitParts;
                        fMinDistance = fDist;
                    }
                    byResult = 0;
                }
            }
            else {
                // Sphere collision with height check
                float fDiameter = fRadius * 2.0f;
                
                if ((vBonePos.z - fDiameter) <= pAttackArea->fHeightT &&
                    pAttackArea->fHeightB <= (vBonePos.z + fDiameter)) {
                    
                    float fDist = std::sqrt(fDiffX * fDiffX + fDiffY * fDiffY);
                    
                    if (fDist >= (pAttackArea->fRadiusStart - fRadius) &&
                        (pAttackArea->fRadiusEnd + fRadius) >= fDist) {
                        
                        if (pAttackArea->fAngle < 180.0f) {
                            // Check angle
                            D3DXVECTOR2 vDirToTarget(fDiffX / fDist, fDiffY / fDist);
                            float fDot = D3DXVec2Dot(&vDirToTarget, &pAttackArea->vAttackerDir);
                            float fBetweenAngle = std::acos(fDot) * 180.0f / 3.1415927f;
                            
                            if (pAttackArea->fAngle < fBetweenAngle && fDot <= 1.0f) {
                                // Check collision with boundary lines
                                if (CollisionShereToLine(&vBonePos, fRadius, &vLineLeftS, &vLineLeftE) ||
                                    CollisionShereToLine(&vBonePos, fRadius, &vLineRightS, &vLineRightE)) {
                                    if (fMinDistance > fDist) {
                                        pAttackArea->byHitPartsIndex = hitCollision->byHitParts;
                                        fMinDistance = fDist;
                                    }
                                    byResult = 0;
                                }
                            }
                            else {
                                if (fMinDistance > fDist) {
                                    pAttackArea->byHitPartsIndex = hitCollision->byHitParts;
                                    fMinDistance = fDist;
                                }
                                byResult = 0;
                            }
                        }
                        else {
                            if (fMinDistance > fDist) {
                                pAttackArea->byHitPartsIndex = hitCollision->byHitParts;
                                fMinDistance = fDist;
                            }
                            byResult = 0;
                        }
                    }
                }
            }
        }
    }
    
    return byResult;
}

// IDA 0x14036A080 - CMover::CollisionShereToLine
// Check if sphere collides with line segment
bool CMover::CollisionShereToLine(hkvVec3* vSphereCenter, float fRadius, 
                                   hkvVec3* vLineStart, hkvVec3* vLineEnd) {
    if (!vSphereCenter || !vLineStart || !vLineEnd) {
        return false;
    }
    
    return FindLineCircleIntersections(
        vSphereCenter->x, vSphereCenter->y, fRadius,
        vLineStart->x, vLineStart->y,
        vLineEnd->x, vLineEnd->y) > 0;
}

// IDA 0x140369B60 - CMover::CollisionCylinderToBox
// Check if cylinder collides with rotated box
int CMover::CollisionCylinderToBox(hkvVec3* vCylinderCenter, float fRadius,
                                    hkvVec3* vBoxCenter, hkvVec3* vBoxSize, 
                                    float fRotation) {
    if (!vCylinderCenter || !vBoxCenter || !vBoxSize) {
        return 0;
    }
    
    // Calculate relative position (ignoring Z)
    hkvVec3 vPos = *vCylinderCenter - *vBoxCenter;
    vPos.z = 0.0f;
    
    // Apply inverse rotation to get local coordinates
    hkvMat3 matRot;
    matRot.setFromEulerAngles(0.0f, 0.0f, -fRotation);
    
    hkvVec3 vDestPos = matRot * vPos;
    
    // Calculate box bounds
    hkvVec3 vLeftTop(-vBoxSize->x * 0.5f, -vBoxSize->y * 0.5f, 0.0f);
    hkvVec3 vRightBottom(vBoxSize->x * 0.5f, vBoxSize->y * 0.5f, 0.0f);
    
    return IsInRectCircle(&vLeftTop, &vRightBottom, &vDestPos, fRadius);
}

// IDA 0x14036A120 - CMover::FindLineCircleIntersections
// Find intersection points between line segment and circle
int CMover::FindLineCircleIntersections(float cx, float cy, float radius,
                                         float x1, float y1, float x2, float y2) {
    // Calculate line direction
    float dx = x2 - x1;
    float dy = y2 - y1;
    
    // Quadratic equation coefficients
    float A = dx * dx + dy * dy;
    float b = 2.0f * (dx * (x1 - cx) + dy * (y1 - cy));
    float c = (x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy) - radius * radius;
    
    float det = b * b - 4.0f * A * c;
    
    // No intersection
    if (A <= 0.0000001f || det < 0.0f) {
        return 0;
    }
    
    // One intersection (tangent)
    if (det == 0.0f) {
        float t = -b / (2.0f * A);
        return (t > 0.0f && t < 1.0f) ? 1 : 0;
    }
    
    // Two intersections
    float sqrtDet = std::sqrt(det);
    float ta = (-b - sqrtDet) / (2.0f * A);
    
    if (ta <= 0.0f || ta >= 1.0f) {
        float tb = (sqrtDet - b) / (2.0f * A);
        if (tb <= 0.0f || tb >= 1.0f) {
            return 0;
        }
        return 2;
    }
    
    return 1;
}

// IDA 0x140369CA0 - CMover::IsInRectCircle
// Check if circle overlaps with rectangle
bool CMover::IsInRectCircle(hkvVec3* vLeftTop, hkvVec3* vRightBottom,
                             hkvVec3* vCircleCenter, float fRadius) {
    if (!vLeftTop || !vRightBottom || !vCircleCenter) {
        return false;
    }
    
    // Check if circle center is inside rectangle (with radius margin)
    if ((vCircleCenter->x + fRadius) >= vLeftTop->x &&
        (vCircleCenter->y - fRadius) >= vLeftTop->y &&
        vRightBottom->x >= (vCircleCenter->x - fRadius) &&
        vRightBottom->y >= (vCircleCenter->y + fRadius)) {
        return true;
    }
    
    if ((vCircleCenter->x - fRadius) >= vLeftTop->x &&
        (vCircleCenter->y + fRadius) >= vLeftTop->y &&
        vRightBottom->x >= (vCircleCenter->x + fRadius) &&
        vRightBottom->y >= (vCircleCenter->y - fRadius)) {
        return true;
    }
    
    // Check distance to corners
    float fMin = 1e9f;
    float fDist;
    
    // Top-left corner
    hkvVec3 vDiff(vCircleCenter->x - vLeftTop->x, vCircleCenter->y - vLeftTop->y, 0.0f);
    fMin = vDiff.GetLengthSquared();
    
    // Top-right corner
    vDiff = hkvVec3(vCircleCenter->x - vLeftTop->x, vCircleCenter->y - vRightBottom->y, 0.0f);
    fDist = vDiff.GetLengthSquared();
    if (fMin > fDist) fMin = fDist;
    
    // Bottom-left corner
    vDiff = hkvVec3(vCircleCenter->x - vRightBottom->x, vCircleCenter->y - vLeftTop->y, 0.0f);
    fDist = vDiff.GetLengthSquared();
    if (fMin > fDist) fMin = fDist;
    
    // Bottom-right corner
    vDiff = hkvVec3(vCircleCenter->x - vRightBottom->x, vCircleCenter->y - vRightBottom->y, 0.0f);
    fDist = vDiff.GetLengthSquared();
    if (fMin > fDist) fMin = fDist;
    
    return (fRadius * fRadius) >= fMin;
}

// IDA 0x140368690 - CMover::GetBoneCurrentWorldSpaceTranslation
// Get bone world position from animation
std::uint64_t CMover::GetBoneCurrentWorldSpaceTranslation(int idx, hkvVec3* vBoneCenterPos, hkvVec3* vPos) {
    if (!vBoneCenterPos || !vPos) {
        return 0;
    }
    
    // Check if we have current motion event
    if (!m_pCurMotionEvent) {
        return 0;
    }
    
    // Get bone translation from animation
    hkvVec3 boneTranslation;
    // Note: VAnimationInfo::GetBoneTranslation would be called here
    // This requires the Vision Engine animation system
    // For now, use the bone center position directly
    boneTranslation = *vBoneCenterPos;
    
    // Apply scaling (simplified - would need m_vScaling from base class)
    // boneTranslation.x *= m_vScaling.x;
    // boneTranslation.y *= m_vScaling.y;
    // boneTranslation.z *= m_vScaling.z;
    
    // Get entity rotation (simplified - would need GetRotationMatrix from base class)
    // const hkvMat3* pRotationMatrix = GetRotationMatrix();
    // hkvQuat entityRotation;
    // hkvQuat::FromMatrix(&entityRotation, pRotationMatrix);
    
    // Transform bone position by entity rotation (simplified)
    // hkvVec3 vTransformedBone;
    // hkvQuat::PreTransformVector(&entityRotation, &vTransformedBone, &boneTranslation);
    // boneTranslation = vTransformedBone;
    
    // Check if bone translation is zero
    if (boneTranslation.isZero(0.00001f)) {
        // Use bone center position
        *vPos = GetPosition() + *vBoneCenterPos;
    }
    else {
        // Use transformed bone position
        *vPos = GetPosition() + boneTranslation;
    }
    
    return 1;
}
