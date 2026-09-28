// hkvMat3.h
// hkvMat3 - Havok Vision Engine 3x3 matrix class
// IDA decompilation from GameServer.exe
//
// Functions:
// - hkvMat3::setFromEulerAngles (0x14009f510)
// - operator* (hkvMat3 * scalar) (0x1406c8b90)
// - operator/ (hkvMat3 / scalar) (0x1406c8b40)

#pragma once

#include "hkvMath.h"
#include "hkvVec3.h"
#include <cstddef>
#include <cstring>

// Forward declarations
class hkvEulerUtil;

// ============================================================================
// hkvPlane - Havok Plane (4 floats: normal + distance)
// ============================================================================
struct hkvPlane {
    hkvVec3 m_vNormal;
    float m_fNegDist;

    hkvPlane() : m_vNormal(0.0f, 0.0f, 0.0f), m_fNegDist(0.0f) {}
    hkvPlane(float _x, float _y, float _z, float _d)
        : m_vNormal(_x, _y, _z), m_fNegDist(_d) {}
    hkvPlane(const hkvPlane& rhs) : m_vNormal() {
        operator=(rhs);
    }

    void operator=(const hkvPlane& plane) {
        unsigned char normal[sizeof(hkvVec3)];
        std::memcpy(normal, &plane.m_vNormal, sizeof(normal));
        std::memcpy(&m_vNormal, normal, sizeof(normal));
        m_fNegDist = plane.m_fNegDist;
    }

    float getDistanceTo(const hkvVec3& vPoint) const {
        return m_vNormal.dot(vPoint) + m_fNegDist;
    }

    int GetSide(const hkvVec3& vertex) const {
        return getDistanceTo(vertex) >= 0.0f;
    }

    void setFromPointAndNormal(const hkvVec3& vPointOnPlane, const hkvVec3& vNormal) {
        std::memcpy(&m_vNormal, &vNormal, sizeof(m_vNormal));
        m_fNegDist = -vNormal.dot(vPointOnPlane);
    }
};
static_assert(sizeof(hkvPlane) == 16, "hkvPlane size must match GameServer PDB");
static_assert(offsetof(hkvPlane, m_vNormal) == 0, "hkvPlane normal offset mismatch");
static_assert(offsetof(hkvPlane, m_fNegDist) == 12, "hkvPlane distance offset mismatch");

// ============================================================================
// hkvMat3 - 3x3 rotation matrix (36 bytes: 9 floats)
// Column-major storage for Havok compatibility
// ============================================================================
struct hkvMat3 {
    // Column-major 3x3 matrix (Havok format)
    // m_ElementsCM[0-2] = column 0, m_ElementsCM[3-5] = column 1, m_ElementsCM[6-8] = column 2
    float m_ElementsCM[9];

    // === Constructors ===
    hkvMat3() {
        // Identity matrix (column-major)
        m_ElementsCM[0] = 1.0f; m_ElementsCM[1] = 0.0f; m_ElementsCM[2] = 0.0f;
        m_ElementsCM[3] = 0.0f; m_ElementsCM[4] = 1.0f; m_ElementsCM[5] = 0.0f;
        m_ElementsCM[6] = 0.0f; m_ElementsCM[7] = 0.0f; m_ElementsCM[8] = 1.0f;
    }

    // === Euler Angles ===

    // setFromEulerAngles - Set rotation from Euler angles (degrees)
    // IDA: ?setFromEulerAngles@hkvMat3@@QEAAXMMM@Z @ 0x14009F510
    // TODO: 需人工审查 - 原函数经 ConvertEulerToMat3_Deg 调用外部 ConvertEulerToMat3_Rad；当前手写公式尚未与该外部函数核对。
    void setFromEulerAngles(float fRoll, float fPitch, float fYaw)
    {
        // Convert degrees to radians
        float roll = hkvMath::Deg2Rad(fRoll);
        float pitch = hkvMath::Deg2Rad(fPitch);
        float yaw = hkvMath::Deg2Rad(fYaw);

        // Calculate rotation matrix elements
        float cr = cosf(roll);
        float sr = sinf(roll);
        float cp = cosf(pitch);
        float sp = sinf(pitch);
        float cy = cosf(yaw);
        float sy = sinf(yaw);

        // Combined rotation matrix (XYZ order) - column-major
        // Column 0
        m_ElementsCM[0] = cy * cp;
        m_ElementsCM[1] = sy * cp;
        m_ElementsCM[2] = -sp;
        // Column 1
        m_ElementsCM[3] = cy * sp * sr - sy * cr;
        m_ElementsCM[4] = sy * sp * sr + cy * cr;
        m_ElementsCM[5] = cp * sr;
        // Column 2
        m_ElementsCM[6] = cy * sp * cr + sy * sr;
        m_ElementsCM[7] = sy * sp * cr - cy * sr;
        m_ElementsCM[8] = cp * cr;
    }

    hkvVec3 getAxis(unsigned int iColumn) const {
        return hkvVec3(m_ElementsCM[3 * iColumn],
                       m_ElementsCM[3 * iColumn + 1],
                       m_ElementsCM[3 * iColumn + 2]);
    }

    // === Operators ===

    // operator* - Multiply matrix by scalar
    // IDA: ??D@YA?BVhkvMat3@@AEBV0@M@Z @ 0x1406c8b90
    hkvMat3 operator*(float f) const {
        hkvMat3 result;
        for (int i = 0; i < 9; ++i) {
            result.m_ElementsCM[i] = m_ElementsCM[i] * f;
        }
        return result;
    }

    // operator/ - Divide matrix by scalar
    // IDA: ??K@YA?BVhkvMat3@@AEBV0@M@Z @ 0x1406c8b40
    hkvMat3 operator/(float f) const {
        return operator*(1.0f / f);
    }

    // transformDirection - Transform direction vector
    hkvVec3 transformDirection(const hkvVec3& v) const {
        // Column-major matrix-vector multiplication
        return hkvVec3(
            m_ElementsCM[0] * v.x + m_ElementsCM[3] * v.y + m_ElementsCM[6] * v.z,
            m_ElementsCM[1] * v.x + m_ElementsCM[4] * v.y + m_ElementsCM[7] * v.z,
            m_ElementsCM[2] * v.x + m_ElementsCM[5] * v.y + m_ElementsCM[8] * v.z
        );
    }

    // operator* - Matrix-vector multiplication
    hkvVec3 operator*(const hkvVec3& v) const {
        return transformDirection(v);
    }
};

static_assert(sizeof(hkvMat3) == 36, "hkvMat3 size mismatch - expected 36 bytes");
