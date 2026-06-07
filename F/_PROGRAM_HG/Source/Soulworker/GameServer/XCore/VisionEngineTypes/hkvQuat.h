// hkvQuat.h
// hkvQuat - Havok Vision Engine quaternion class
// IDA decompilation from GameServer.exe
//
// Functions:
// - hkvQuat::hkvQuat (0x140718440)
// - hkvQuat::setIdentity (0x140718400)
// - hkvQuat::setFromEulerAngles (0x1407182b0)
// - hkvQuat::getAsEulerAngles (0x1407183c0)
// - hkvQuat::setFromMat3 (0x140377f90)
// - hkvQuat::getAsMat3 (0x140717fb0)

#pragma once

#include "hkvMath.h"
#include "hkvMat3.h"
#include <cmath>

// Forward declarations
class hkvEulerUtil;

// ============================================================================
// hkvQuat - Quaternion (16 bytes: 4 floats)
// Standard quaternion representation (x, y, z, w)
// ============================================================================
struct hkvQuat {
    float x;
    float y;
    float z;
    float w;

    // === Constructors ===

    // IDA: ??0hkvQuat@@QEAA@XZ @ 0x140718440
    hkvQuat() {
        setIdentity();
    }

    hkvQuat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    // === Identity ===

    // IDA: ?setIdentity@hkvQuat@@QEAAXXZ @ 0x140718400
    void setIdentity() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 1.0f;
    }

    // === Euler Angles ===

    // IDA: ?setFromEulerAngles@hkvQuat@@QEAAXMMM@Z @ 0x1407182b0
    void setFromEulerAngles(float fRollDeg, float fPitchDeg, float fYawDeg) {
        hkvEulerUtil::ConvertEulerToQuaternion_Deg(this, fRollDeg, fPitchDeg, fYawDeg, hkvEulerUtil::Mode::XYZs);
    }

    // IDA: ?getAsEulerAngles@hkvQuat@@QEAAXAEAM00@Z @ 0x1407183c0
    void getAsEulerAngles(float& out_fRollDeg, float& out_fPitchDeg, float& out_fYawDeg) const {
        hkvEulerUtil::ConvertQuaternionToEuler_Deg(this, &out_fRollDeg, &out_fPitchDeg, &out_fYawDeg, hkvEulerUtil::Mode::XYZs);
    }

    // === Matrix Conversion ===

    // IDA: ?setFromMat3@hkvQuat@@QEAAXAEBVhkvMat3@@@Z @ 0x140377f90
    void setFromMat3(const hkvMat3& m) {
        float trace = m.m_ElementsCM[0] + m.m_ElementsCM[4] + m.m_ElementsCM[8];
        float half = 0.5f;

        if (trace <= 0.0f) {
            int next[3] = {1, 2, 0};
            int i = m.m_ElementsCM[4] > m.m_ElementsCM[0] ? 1 : 0;
            if (m.m_ElementsCM[8] > m.m_ElementsCM[3 * i + i]) {
                i = 2;
            }
            int j = next[i];
            int k = next[j];

            float v8 = std::sqrt(m.m_ElementsCM[3 * i + i] - (m.m_ElementsCM[3 * j + j] + m.m_ElementsCM[3 * k + k]) + 1.0f);
            float v11 = half / v8;

            float v[4];
            v[i] = half * v8;
            v[3] = (m.m_ElementsCM[3 * j + k] - m.m_ElementsCM[3 * k + j]) * v11;
            v[j] = (m.m_ElementsCM[3 * i + j] + m.m_ElementsCM[3 * j + i]) * v11;
            v[k] = (m.m_ElementsCM[3 * i + k] + m.m_ElementsCM[3 * k + i]) * v11;

            x = v[0];
            y = v[1];
            z = v[2];
            w = v[3];
        } else {
            float s = std::sqrt(trace + 1.0f);
            float t = half / s;

            x = (m.m_ElementsCM[5] - m.m_ElementsCM[7]) * t;
            y = (m.m_ElementsCM[6] - m.m_ElementsCM[2]) * t;
            z = (m.m_ElementsCM[1] - m.m_ElementsCM[3]) * t;
            w = half * s;
        }
    }

    // IDA: ?getAsMat3@hkvQuat@@QEBA?BVhkvMat3@@XZ @ 0x140717fb0
    hkvMat3 getAsMat3() const {
        hkvMat3 res;

        float xx = x * x * 2.0f;
        float yy = y * y * 2.0f;
        float zz = z * z * 2.0f;
        float xy = x * y * 2.0f;
        float xz = x * z * 2.0f;
        float yz = y * z * 2.0f;
        float wx = w * x * 2.0f;
        float wy = w * y * 2.0f;
        float wz = w * z * 2.0f;

        // Column 0
        res.m_ElementsCM[0] = 1.0f - (yy + zz);
        res.m_ElementsCM[1] = xy + wz;
        res.m_ElementsCM[2] = xz - wy;
        // Column 1
        res.m_ElementsCM[3] = xy - wz;
        res.m_ElementsCM[4] = 1.0f - (xx + zz);
        res.m_ElementsCM[5] = yz + wx;
        // Column 2
        res.m_ElementsCM[6] = xz + wy;
        res.m_ElementsCM[7] = yz - wx;
        res.m_ElementsCM[8] = 1.0f - (xx + yy);

        return res;
    }
};

static_assert(sizeof(hkvQuat) == 16, "hkvQuat size mismatch - expected 16 bytes");

// ============================================================================
// hkvEulerUtil - Euler angle utility class
// ============================================================================
struct hkvEulerUtil {
    enum class Mode {
        XYZs,
        XZYs,
        YXZs,
        YZXs,
        ZXYs,
        ZYXs,
        XYZr,
        XZYr,
        YXZr,
        YZXr,
        ZXYr,
        ZYXr
    };

    // IDA: ?ConvertEulerToQuaternion_Deg@hkvEulerUtil@@SA?AVhkvQuat@@MMMW4Enum@Mode@1@@Z @ 0x140718230
    static hkvQuat ConvertEulerToQuaternion_Deg(float fRotation1, float fRotation2, float fRotation3, Mode EulerMode) {
        hkvQuat result;
        float rad1 = hkvMath::Deg2Rad(fRotation1);
        float rad2 = hkvMath::Deg2Rad(fRotation2);
        float rad3 = hkvMath::Deg2Rad(fRotation3);
        ConvertEulerToQuaternion_Rad(&result, rad1, rad2, rad3, EulerMode);
        return result;
    }

    // IDA: ?ConvertQuaternionToEuler_Deg@hkvEulerUtil@@SAXAEBVhkvQuat@@AEAM11W4Enum@Mode@1@@Z @ 0x140718330
    static void ConvertQuaternionToEuler_Deg(const hkvQuat* q, float* out_fRollDeg, float* out_fPitchDeg, float* out_fYawDeg, Mode EulerMode) {
        // TODO: Implement quaternion to euler conversion
        // Placeholder implementation
        *out_fRollDeg = 0.0f;
        *out_fPitchDeg = 0.0f;
        *out_fYawDeg = 0.0f;
    }

    // Convert Euler angles to quaternion (radians)
    static void ConvertEulerToQuaternion_Rad(hkvQuat* result, float rad1, float rad2, float rad3, Mode EulerMode) {
        // TODO: Implement based on Euler mode
        // For XYZs mode (most common):
        float cr = std::cos(rad1 * 0.5f);
        float sr = std::sin(rad1 * 0.5f);
        float cp = std::cos(rad2 * 0.5f);
        float sp = std::sin(rad2 * 0.5f);
        float cy = std::cos(rad3 * 0.5f);
        float sy = std::sin(rad3 * 0.5f);

        result->w = cr * cp * cy + sr * sp * sy;
        result->x = sr * cp * cy - cr * sp * sy;
        result->y = cr * sp * cy + sr * cp * sy;
        result->z = cr * cp * sy - sr * sp * cy;
    }
};
