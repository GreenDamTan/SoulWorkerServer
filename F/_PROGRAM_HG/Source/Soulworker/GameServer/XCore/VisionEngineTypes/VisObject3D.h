// VisObject3D.h
// VisObject3D_cl - Vision Engine 3D object class
// IDA decompilation from GameServer.exe
//
// Functions:
// - VisObject3D_cl::SetPosition (0x140189790)

#pragma once

#include "hkvVec3.h"
#include "VisTypedEngineObject.h"

// ============================================================================
// XVec3 - Extended vector (inherits from hkvVec3)
// ============================================================================
struct XVec3 : public hkvVec3 {
    XVec3() : hkvVec3() {}
    XVec3(float _x, float _y, float _z) : hkvVec3(_x, _y, _z) {}
};

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

        XVec3 vPos(x, y, z);
        SetPosition(static_cast<const hkvVec3*>(&vPos));
    }

    // SetPosition - Set position from hkvVec3
    virtual void SetPosition(const hkvVec3* pPos) {
        // TODO: Implement actual position update
        // This would update internal position and potentially notify rendering system
    }

    // GetPosition - Get current position
    const hkvVec3& GetPosition() const { return m_vPosition; }

protected:
    hkvVec3 m_vPosition;
};
