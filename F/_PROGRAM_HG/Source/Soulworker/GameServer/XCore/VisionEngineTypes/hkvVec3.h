// hkvVec3.h
// hkvVec3 - 3D vector class (Havok Vision Engine)
// IDA decompilation from GameServer.exe
//
// Functions:
// - hkvVec3::ZeroVector (0x140189100)
// - hkvVec3::hkvVec3 (constructor)

#pragma once

#include <cstdint>

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

    hkvVec3 operator*(float scalar) const {
        return hkvVec3(x * scalar, y * scalar, z * scalar);
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
};

static_assert(sizeof(hkvVec3) == 12, "hkvVec3 size mismatch - expected 12 bytes");
