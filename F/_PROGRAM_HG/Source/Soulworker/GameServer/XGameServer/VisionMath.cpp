// VisionMath.cpp
// Vision Engine math functions
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - hkvVec3::getLengthSquared (0x140085870)
// - hkvVec3::getLength (0x1400858c0)
// - hkvVec3::getDistanceTo (0x1400859e0)
// - hkvVec3::isZero (0x140276580)
// - hkvVec3::normalize (0x1402767a0)
// - hkvVec3::getLengthInverse (0x140276780)
// - hkvMat3::setIdentity (0x14009f5d0)
// - hkvMat3::hkvMat3 (0x14009f660)
// - hkvMat3::transformDirection (0x14009f200)

#include <cmath>

// hkvVec3 structure (12 bytes: 3 floats)
struct hkvVec3 {
    float x;
    float y;
    float z;

    hkvVec3() : x(0.0f), y(0.0f), z(0.0f) {}
    hkvVec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    hkvVec3(float f) : x(f), y(f), z(f) {}

    // Get squared length
    float getLengthSquared() const {
        return (x * x) + (y * y) + (z * z);
    }

    // Get length
    float getLength() const {
        return std::sqrt(getLengthSquared());
    }

    // Get inverse length (1/length)
    float getLengthInverse() const {
        float lenSq = getLengthSquared();
        if (lenSq > 0.0f) {
            return 1.0f / std::sqrt(lenSq);
        }
        return 0.0f;
    }

    // Check if zero with epsilon
    bool isZero(float fEpsilon = 0.0f) const {
        if (fEpsilon == 0.0f) {
            return x == 0.0f && y == 0.0f && z == 0.0f;
        } else {
            float negEpsilon = -fEpsilon;
            return x >= negEpsilon && x <= fEpsilon &&
                   y >= negEpsilon && y <= fEpsilon &&
                   z >= negEpsilon && z <= fEpsilon;
        }
    }

    // Normalize in place
    void normalize() {
        float invLen = getLengthInverse();
        x *= invLen;
        y *= invLen;
        z *= invLen;
    }

    // Multiply by scalar in place
    hkvVec3& operator*=(float f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    // Subtract operator
    hkvVec3 operator-(const hkvVec3& rhs) const {
        return hkvVec3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    // Add operator
    hkvVec3 operator+(const hkvVec3& rhs) const {
        return hkvVec3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    // Get distance to another point
    float getDistanceTo(const hkvVec3& rhs) const {
        hkvVec3 diff = *this - rhs;
        return diff.getLength();
    }
};

// hkvMat3 structure (36 bytes: 3x3 floats in column-major order)
struct hkvMat3 {
    // Column-major storage: m_ElementsCM[column][row] or m_ElementsCM[column*3 + row]
    float m_ElementsCM[9];

    // Default constructor - creates identity matrix
    hkvMat3() {
        setIdentity();
    }

    // Set to identity matrix
    void setIdentity() {
        m_ElementsCM[0] = 1.0f;
        m_ElementsCM[1] = 0.0f;
        m_ElementsCM[2] = 0.0f;
        m_ElementsCM[3] = 0.0f;
        m_ElementsCM[4] = 1.0f;
        m_ElementsCM[5] = 0.0f;
        m_ElementsCM[6] = 0.0f;
        m_ElementsCM[7] = 0.0f;
        m_ElementsCM[8] = 1.0f;
    }

    // Transform a direction vector (rotation only, no translation)
    hkvVec3 transformDirection(const hkvVec3& v) const {
        return hkvVec3(
            m_ElementsCM[0] * v.x + m_ElementsCM[3] * v.y + m_ElementsCM[6] * v.z,
            m_ElementsCM[1] * v.x + m_ElementsCM[4] * v.y + m_ElementsCM[7] * v.z,
            m_ElementsCM[2] * v.x + m_ElementsCM[5] * v.y + m_ElementsCM[8] * v.z
        );
    }

    // Access element by row and column
    float& operator()(int row, int col) {
        return m_ElementsCM[col * 3 + row];
    }

    const float& operator()(int row, int col) const {
        return m_ElementsCM[col * 3 + row];
    }
};

// ============================================================================
// hkvVec3::getLengthSquared - Get squared length
// IDA @ 0x140085870
// ============================================================================
float __fastcall hkvVec3_getLengthSquared(const hkvVec3* self)
{
    return (self->x * self->x) + (self->y * self->y) + (self->z * self->z);
}

// ============================================================================
// hkvVec3::getLength - Get length
// IDA @ 0x1400858c0
// ============================================================================
float __fastcall hkvVec3_getLength(const hkvVec3* self)
{
    return std::sqrt(self->getLengthSquared());
}

// ============================================================================
// hkvVec3::getDistanceTo - Get distance to another point
// IDA @ 0x1400859e0
// ============================================================================
float __fastcall hkvVec3_getDistanceTo(const hkvVec3* self, const hkvVec3* rhs)
{
    hkvVec3 diff = *self - *rhs;
    return diff.getLength();
}

// ============================================================================
// hkvVec3::isZero - Check if zero with epsilon
// IDA @ 0x140276580
// ============================================================================
bool __fastcall hkvVec3_isZero(const hkvVec3* self, float fEpsilon)
{
    if (fEpsilon == 0.0f) {
        return self->x == 0.0f && self->y == 0.0f && self->z == 0.0f;
    } else {
        float negEpsilon = -fEpsilon;
        return self->x >= negEpsilon && self->x <= fEpsilon &&
               self->y >= negEpsilon && self->y <= fEpsilon &&
               self->z >= negEpsilon && self->z <= fEpsilon;
    }
}

// ============================================================================
// hkvVec3::getLengthInverse - Get inverse length
// IDA @ 0x140276780
// ============================================================================
float __fastcall hkvVec3_getLengthInverse(const hkvVec3* self)
{
    float lenSq = self->getLengthSquared();
    if (lenSq > 0.0f) {
        return 1.0f / std::sqrt(lenSq);
    }
    return 0.0f;
}

// ============================================================================
// hkvVec3::normalize - Normalize in place
// IDA @ 0x1402767a0
// ============================================================================
void __fastcall hkvVec3_normalize(hkvVec3* self)
{
    float invLen = self->getLengthInverse();
    self->x *= invLen;
    self->y *= invLen;
    self->z *= invLen;
}

// ============================================================================
// hkvMat3::setIdentity - Set to identity matrix
// IDA @ 0x14009f5d0
// ============================================================================
void __fastcall hkvMat3_setIdentity(hkvMat3* self)
{
    self->m_ElementsCM[0] = 1.0f;
    self->m_ElementsCM[3] = 0.0f;
    self->m_ElementsCM[6] = 0.0f;
    self->m_ElementsCM[1] = 0.0f;
    self->m_ElementsCM[4] = 1.0f;
    self->m_ElementsCM[7] = 0.0f;
    self->m_ElementsCM[2] = 0.0f;
    self->m_ElementsCM[5] = 0.0f;
    self->m_ElementsCM[8] = 1.0f;
}

// ============================================================================
// hkvMat3::hkvMat3 - Default constructor
// IDA @ 0x14009f660
// ============================================================================
hkvMat3* __fastcall hkvMat3_constructor(hkvMat3* self)
{
    self->setIdentity();
    return self;
}

// ============================================================================
// hkvMat3::transformDirection - Transform direction vector
// IDA @ 0x14009f200
// ============================================================================
hkvVec3* __fastcall hkvMat3_transformDirection(hkvVec3* result, const hkvMat3* self, const hkvVec3* v)
{
    result->x = self->m_ElementsCM[0] * v->x + self->m_ElementsCM[3] * v->y + self->m_ElementsCM[6] * v->z;
    result->y = self->m_ElementsCM[1] * v->x + self->m_ElementsCM[4] * v->y + self->m_ElementsCM[7] * v->z;
    result->z = self->m_ElementsCM[2] * v->x + self->m_ElementsCM[5] * v->y + self->m_ElementsCM[8] * v->z;
    return result;
}
