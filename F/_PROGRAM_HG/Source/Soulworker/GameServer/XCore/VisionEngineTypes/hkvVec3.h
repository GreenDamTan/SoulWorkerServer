// hkvVec3.h
// hkvVec3 - 3D vector class (Havok Vision Engine)
// IDA decompilation from GameServer.exe
//
// Functions:
// - hkvVec3::ZeroVector (0x140189100)
// - hkvVec3::hkvVec3 (constructor)
// - hkvVec3::DistSqr (0x140364320)
// - hkvVec3::isEqual (0x1403643f0)

#pragma once

#include <cstdint>
#include <cmath>
#include "hkvMath.h"

// ============================================================================
// hkvResult - Havok result type
// ============================================================================
struct hkvResult {
    int result;  // 0 = success, non-zero = failure
};

// ============================================================================
// hkvVec3 - 3D vector (12 bytes: 3 floats)
// Basic vector class from Havok Vision Engine
// ============================================================================
struct hkvVec3 {
    float x;
    float y;
    float z;

    // === Constructor ===
    hkvVec3() : x(0.0f), y(0.0f), z(0.0f) {}

    hkvVec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    // Constructor with single value (all components same)
    // IDA shows: hkvVec3::hkvVec3(result, 0.0)
    explicit hkvVec3(float val) : x(val), y(val), z(val) {}

    // === Static Factory ===

    // ZeroVector - Returns a zero vector (0, 0, 0)
    // IDA: ?ZeroVector@hkvVec3@@SA?BV1@XZ @ 0x140189100
    static hkvVec3 ZeroVector() {
        return hkvVec3(0.0f, 0.0f, 0.0f);
    }

    // === Length methods ===
    float GetLength() const { return std::sqrt(x * x + y * y + z * z); }
    float GetLengthSquared() const { return x * x + y * y + z * z; }

    // Alias for Havok naming convention
    float getLength() const { return GetLength(); }
    float getLengthSquared() const { return GetLengthSquared(); }

    // IDA: 0x1400169B0 - setZero
    void setZero() { x = 0.0f; y = 0.0f; z = 0.0f; }

    // IDA: ?set@hkvVec3@@QEAAXM@Z @ 0x140378870
    // set - Set all components to the same value
    void set(float xyz) { x = xyz; y = xyz; z = xyz; }

    // isZero - 检查向量是否为零向量
    bool isZero(float fEpsilon = 0.0001f) const {
        return (std::abs(x) < fEpsilon && std::abs(y) < fEpsilon && std::abs(z) < fEpsilon);
    }

    // getNormalized - 返回归一化后的向量副本
    hkvVec3 getNormalized() const {
        float len = GetLength();
        if (len > 0.0001f) {
            return hkvVec3(x / len, y / len, z / len);
        }
        return hkvVec3(0.0f, 0.0f, 0.0f);
    }

    // normalizeIfNotZero - 如果不是零向量则归一化
    void normalizeIfNotZero() {
        float len = GetLength();
        if (len > 0.0001f) {
            x /= len; y /= len; z /= len;
        }
    }

    // IDA: hkvVec3::normalizeIfNotZero(&vDirection, &v13, 0.000001)
    // Normalize with result output - Havok style
    hkvResult normalizeIfNotZero(float fEpsilon) {
        hkvResult res;
        float len = GetLength();
        if (len > fEpsilon) {
            x /= len; y /= len; z /= len;
            res.result = 0;  // success
        } else {
            res.result = 1;  // failure - vector too small
        }
        return res;
    }

    // dot - 点积
    float dot(const hkvVec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // getDistanceTo - 计算到另一个向量的距离
    float getDistanceTo(const hkvVec3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    // getDistanceToSquared - 计算到另一个向量的距离平方
    // IDA: ?getDistanceToSquared@hkvVec3@@QEBAMV1@@Z
    float getDistanceToSquared(const hkvVec3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return dx * dx + dy * dy + dz * dz;
    }

    // DistSqr - Distance squared to another vector (alias)
    // IDA: ?DistSqr@hkvVec3@@QEBAMAEBV1@@Z @ 0x140364320
    float DistSqr(const hkvVec3& v2) const {
        return getDistanceToSquared(v2);
    }

    // isEqual - Compare vectors with epsilon tolerance
    // IDA: ?isEqual@hkvVec3@@QEBA_NAEBV1@M@Z @ 0x1403643f0
    bool isEqual(const hkvVec3& rhs, float fEpsilon) const {
        return hkvMath::isFloatEqual(x, rhs.x, fEpsilon)
            && hkvMath::isFloatEqual(y, rhs.y, fEpsilon)
            && hkvMath::isFloatEqual(z, rhs.z, fEpsilon);
    }

    // IDA: 0x1403A20C0 - Normalize
    // Normalize - 归一化向量，返回是否成功
    bool Normalize() {
        float len = GetLength();
        if (len < 0.000001f) {
            return false;  // 失败：向量长度太小
        }
        x /= len;
        y /= len;
        z /= len;
        return true;  // 成功
    }

    // === Operators ===
    hkvVec3 operator+(const hkvVec3& other) const {
        return hkvVec3(x + other.x, y + other.y, z + other.z);
    }

    // operator+= - Add and assign
    // IDA: ??YhkvVec3@@QEAAXAEBV0@@Z @ 0x14009f370
    hkvVec3& operator+=(const hkvVec3& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    hkvVec3 operator-(const hkvVec3& other) const {
        return hkvVec3(x - other.x, y - other.y, z - other.z);
    }

    // operator-= - Subtract and assign
    // IDA: ??ZhkvVec3@@QEAAXAEBV0@@Z
    hkvVec3& operator-=(const hkvVec3& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    hkvVec3 operator*(float scalar) const {
        return hkvVec3(x * scalar, y * scalar, z * scalar);
    }

    // Component-wise multiplication
    hkvVec3 operator*(const hkvVec3& other) const {
        return hkvVec3(x * other.x, y * other.y, z * other.z);
    }

    // operator*= - Multiply by scalar and assign
    // IDA: ??XhkvVec3@@QEAAXM@Z
    hkvVec3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // operator/ - Divide by scalar
    // IDA: ??K@YA?BVhkvVec3@@AEBV0@M@Z @ 0x1406c99f0
    hkvVec3 operator/(float scalar) const {
        return hkvVec3(x / scalar, y / scalar, z / scalar);
    }

    // operator/= - Divide by scalar and assign
    // IDA: ??_0hkvVec3@@QEAAXM@Z
    hkvVec3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    bool operator==(const hkvVec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const hkvVec3& other) const {
        return !(*this == other);
    }

    // === Static utility methods ===

    // set - Set vector components
    static void set(hkvVec3* v, float _x, float _y, float _z) {
        v->x = _x;
        v->y = _y;
        v->z = _z;
    }

    // setMin - Set to component-wise minimum
    static void setMin(hkvVec3* v, const hkvVec3* rhs) {
        if (rhs->x < v->x) v->x = rhs->x;
        if (rhs->y < v->y) v->y = rhs->y;
        if (rhs->z < v->z) v->z = rhs->z;
    }

    // setMax - Set to component-wise maximum
    static void setMax(hkvVec3* v, const hkvVec3* rhs) {
        if (rhs->x > v->x) v->x = rhs->x;
        if (rhs->y > v->y) v->y = rhs->y;
        if (rhs->z > v->z) v->z = rhs->z;
    }
};

static_assert(sizeof(hkvVec3) == 12, "hkvVec3 size mismatch - expected 12 bytes");
