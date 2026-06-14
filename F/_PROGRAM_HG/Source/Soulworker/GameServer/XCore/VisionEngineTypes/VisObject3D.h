// VisObject3D.h
// VisObject3D_cl - Vision Engine 3D object class
// IDA decompilation from GameServer.exe
//
// Functions:
// - VisObject3D_cl::SetPosition (0x140189790)

#pragma once

#include "hkvVec3.h"
#include "hkvMat3.h"
#include "VisTypedEngineObject.h"

// Note: XVec3 is defined in PSCommon.h, do not redefine here

// ============================================================================
// VisObject3D_cl - Vision Engine 3D object class
// Base class for all 3D visible objects
// ============================================================================
class VisObject3D_cl : public VisTypedEngineObject_cl {
public:
    // SetPosition - Set position from x, y, z coordinates
    // IDA: ?SetPosition@VisObject3D_cl@@QEAAXMMM@Z @ 0x140189790
    void SetPosition(float x, float y, float z) {
        // IDA code:
        // void __fastcall VisObject3D_cl::SetPosition(VisObject3D_cl *this, float x, float y, float z)
        // {
        //   const struct hkvVec3 *v4; // rax
        //   XVec3 v5; // [rsp+20h] [rbp-18h] BYREF
        //
        //   v4 = (const struct hkvVec3 *)hkvVec3::hkvVec3(&v5, x, y, z);
        //   VisObject3D_cl::SetPosition(this, v4);
        // }

        hkvVec3 vPos(x, y, z);
        SetPosition(&vPos);
    }

    // SetPosition - Set position from hkvVec3 pointer
    virtual void SetPosition(const hkvVec3* pPos) {
        // TODO: Implement actual position update
        // This would update internal position and potentially notify rendering system
        if (pPos) {
            m_vPosition = *pPos;
        }
    }

    // GetPosition - Get current position (returns const reference)
    // IDA: Returns pointer to internal position (this + offset)
    const hkvVec3& GetPosition() const { return m_vPosition; }

    // GetPosition - Get position into output parameter (for compatibility)
    // IDA pattern: VisObject3D_cl::GetPosition(this) returns hkvVec3*
    const hkvVec3* GetPosition(hkvVec3* pOut) const {
        if (pOut) {
            *pOut = m_vPosition;
        }
        return &m_vPosition;
    }

    // SetOrientation - Set orientation from yaw, pitch, roll
    virtual void SetOrientation(float fYaw, float fPitch, float fRoll) {
        m_vOrientation.x = fYaw;
        m_vOrientation.y = fPitch;
        m_vOrientation.z = fRoll;
    }

    // SetOrientation - Set orientation from hkvVec3 pointer
    virtual void SetOrientation(const hkvVec3* pOrientation) {
        if (pOrientation) {
            m_vOrientation = *pOrientation;
        }
    }

    // GetOrientation - Get current orientation
    const hkvVec3& GetOrientation() const { return m_vOrientation; }

    // GetUseEulerAngles - Check if using Euler angles
    static bool GetUseEulerAngles() { return true; }

    // SetRotationMatrix - Set rotation from matrix
    virtual void SetRotationMatrix(const hkvMat3* pMat) {
        // TODO: Implement
    }

protected:
    hkvVec3 m_vPosition;
    hkvVec3 m_vOrientation;
};
