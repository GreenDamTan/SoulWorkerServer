// SWIG_hkvVec3.cpp
// SWIG Lua bindings for hkvVec3 class
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - hkvVec3_normalizeIfNotZero (0x1406446a0)
// - hkvVec3_setLength (0x1406446d0)
// - new_hkvVec3__SWIG_3 (0x140644710)
// - hkvVec3___eq (0x140644940)
// - hkvVec3___add (0x140644980)
// - hkvVec3___sub (0x1406449f0)
// - hkvVec3___mul (0x140644a60)
// - hkvVec3___div (0x140644aa0)
// - hkvVec3___getitem__ (0x140644ae0)
// - hkvVec3___setitem__ (0x140644b10)

#include <cstdint>
#include <cstring>
#include <cmath>

// Forward declarations
struct lua_State;
struct hkvVec3;

// Lua API function declarations
extern "C" {
    void lua_pushnumber(lua_State* L, double n);
    double lua_tonumber(lua_State* L, int idx);
    int lua_type(lua_State* L, int idx);
    void* lua_touserdata(lua_State* L, int idx);
    void lua_pushnil(lua_State* L);
    void lua_pushstring(lua_State* L, const char* s);
    const char* lua_tolstring(lua_State* L, int idx, std::size_t* len);
    int lua_isstring(lua_State* L, int idx);
    int lua_isuserdata(lua_State* L, int idx);
    void luaL_error(lua_State* L, const char* fmt, ...);
}

// SWIG type info forward declaration
struct swig_type_info;
extern swig_type_info* swig_types[];
extern "C" std::int64_t SWIG_Lua_ConvertPtr(lua_State* L, int index, void** ptr, swig_type_info* type, char flags);

// Forward declarations from Vision Engine
extern void* VBaseObject_operator_new(std::size_t size);

// hkvVec3 structure (12 bytes: 3 floats)
struct hkvVec3 {
    float x;
    float y;
    float z;

    // Constructor
    hkvVec3() : x(0.0f), y(0.0f), z(0.0f) {}
    hkvVec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    hkvVec3(float f) : x(f), y(f), z(f) {}
};

// hkvResult enum
enum hkvResult {
    HKV_SUCCESS = 0,
    HKV_FAILURE = 1
};

// ============================================================================
// hkvVec3_normalizeIfNotZero - Normalize if length > epsilon
// IDA @ 0x1406446a0
// Returns true if normalization succeeded
// ============================================================================
extern "C" bool __fastcall hkvVec3_normalizeIfNotZero(hkvVec3* self)
{
    // Calculate length squared
    float lenSq = self->x * self->x + self->y * self->y + self->z * self->z;

    // epsilon squared = 0.000001 * 0.000001 = 1e-12
    if (lenSq > 1e-12f) {
        float len = std::sqrt(lenSq);
        self->x /= len;
        self->y /= len;
        self->z /= len;
        return true;
    }
    return false;
}

// ============================================================================
// hkvVec3_setLength - Set vector length
// IDA @ 0x1406446d0
// Returns true if successful
// ============================================================================
extern "C" bool __fastcall hkvVec3_setLength(hkvVec3* self, float f)
{
    float lenSq = self->x * self->x + self->y * self->y + self->z * self->z;

    if (lenSq > 1e-12f) {
        float currentLen = std::sqrt(lenSq);
        float scale = f / currentLen;
        self->x *= scale;
        self->y *= scale;
        self->z *= scale;
        return true;
    }
    return false;
}

// ============================================================================
// new_hkvVec3__SWIG_3 - Create new hkvVec3
// IDA @ 0x140644710
// Creates hkvVec3, optionally initialized to zero
// ============================================================================
extern "C" hkvVec3* __fastcall new_hkvVec3__SWIG_3(bool bInitialize)
{
    hkvVec3* pVec = (hkvVec3*)VBaseObject_operator_new(sizeof(hkvVec3));

    if (pVec) {
        if (bInitialize) {
            pVec->x = 0.0f;
            pVec->y = 0.0f;
            pVec->z = 0.0f;
        }
        // If !bInitialize, leave uninitialized
    }

    return pVec;
}

// ============================================================================
// hkvVec3___eq - Equality comparison
// IDA @ 0x140644940
// Lua __eq metamethod for hkvVec3
// ============================================================================
extern "C" bool __fastcall hkvVec3___eq(const hkvVec3* self, hkvVec3* pRhs)
{
    if (!pRhs) {
        return false;
    }

    // Compare with epsilon tolerance
    const float epsilon = 0.00001f;
    return (std::abs(self->x - pRhs->x) < epsilon &&
            std::abs(self->y - pRhs->y) < epsilon &&
            std::abs(self->z - pRhs->z) < epsilon);
}

// ============================================================================
// hkvVec3___add - Addition operator
// IDA @ 0x140644980
// Lua __add metamethod for hkvVec3
// ============================================================================
extern "C" hkvVec3* __fastcall hkvVec3___add(hkvVec3* result, hkvVec3* self, const hkvVec3* pRhs)
{
    if (pRhs) {
        result->x = self->x + pRhs->x;
        result->y = self->y + pRhs->y;
        result->z = self->z + pRhs->z;
    } else {
        // Copy self if rhs is null
        result->x = self->x;
        result->y = self->y;
        result->z = self->z;
    }

    return result;
}

// ============================================================================
// hkvVec3___sub - Subtraction operator
// IDA @ 0x1406449f0
// Lua __sub metamethod for hkvVec3
// ============================================================================
extern "C" hkvVec3* __fastcall hkvVec3___sub(hkvVec3* result, hkvVec3* self, const hkvVec3* pRhs)
{
    if (pRhs) {
        result->x = self->x - pRhs->x;
        result->y = self->y - pRhs->y;
        result->z = self->z - pRhs->z;
    } else {
        // Copy self if rhs is null
        result->x = self->x;
        result->y = self->y;
        result->z = self->z;
    }

    return result;
}

// ============================================================================
// hkvVec3___mul - Multiplication operator
// IDA @ 0x140644a60
// Lua __mul metamethod for hkvVec3 (scalar multiplication)
// ============================================================================
extern "C" hkvVec3* __fastcall hkvVec3___mul(hkvVec3* result, hkvVec3* self, float f)
{
    result->x = self->x * f;
    result->y = self->y * f;
    result->z = self->z * f;

    return result;
}

// ============================================================================
// hkvVec3___div - Division operator
// IDA @ 0x140644aa0
// Lua __div metamethod for hkvVec3 (scalar division)
// ============================================================================
extern "C" hkvVec3* __fastcall hkvVec3___div(hkvVec3* result, hkvVec3* self, float f)
{
    if (std::abs(f) > 1e-12f) {
        result->x = self->x / f;
        result->y = self->y / f;
        result->z = self->z / f;
    } else {
        // Avoid division by zero
        result->x = 0.0f;
        result->y = 0.0f;
        result->z = 0.0f;
    }

    return result;
}

// ============================================================================
// hkvVec3___getitem__ - Index operator
// IDA @ 0x140644ae0
// Lua __getitem metamethod (get component by index)
// ============================================================================
extern "C" float __fastcall hkvVec3___getitem__(hkvVec3* self, int index)
{
    if (index >= 0 && index < 3) {
        return (&self->x)[index];
    }
    return 0.0f;
}

// ============================================================================
// hkvVec3___setitem__ - Index assignment operator
// IDA @ 0x140644b10
// Lua __setitem metamethod (set component by index)
// ============================================================================
extern "C" void __fastcall hkvVec3___setitem__(hkvVec3* self, int index, float value)
{
    if (index >= 0 && index < 3) {
        (&self->x)[index] = value;
    }
}

// ============================================================================
// hkvVec3_makeOrthogonalTo - Make orthogonal to normal
// IDA @ 0x1406447c0
// Projects vector to be orthogonal to given normal
// ============================================================================
extern "C" void __fastcall hkvVec3_makeOrthogonalTo(hkvVec3* self, hkvVec3* vNormal)
{
    // Normalize the normal vector
    float lenSq = vNormal->x * vNormal->x + vNormal->y * vNormal->y + vNormal->z * vNormal->z;
    if (lenSq > 1e-12f) {
        float len = std::sqrt(lenSq);
        float nx = vNormal->x / len;
        float ny = vNormal->y / len;
        float nz = vNormal->z / len;

        // Project self onto normal and subtract
        float dot = self->x * nx + self->y * ny + self->z * nz;
        self->x -= dot * nx;
        self->y -= dot * ny;
        self->z -= dot * nz;
    }
}

// ============================================================================
// hkvVec3_clone - Clone vector
// IDA @ 0x1406447f0
// Creates a copy of the vector
// ============================================================================
extern "C" hkvVec3* __fastcall hkvVec3_clone(hkvVec3* result, const hkvVec3* self)
{
    result->x = self->x;
    result->y = self->y;
    result->z = self->z;
    return result;
}

// ============================================================================
// hkvVec3_compMul__SWIG_1 - Component-wise multiplication
// IDA @ 0x140644840
// Multiplies each component by corresponding values
// ============================================================================
extern "C" hkvVec3* __fastcall hkvVec3_compMul__SWIG_1(hkvVec3* result, hkvVec3* self, float x, float y, float z)
{
    result->x = self->x * x;
    result->y = self->y * y;
    result->z = self->z * z;
    return result;
}

// ============================================================================
// hkvVec3_compDiv__SWIG_1 - Component-wise division
// IDA @ 0x1406448a0
// Divides each component by corresponding values
// ============================================================================
extern "C" hkvVec3* __fastcall hkvVec3_compDiv__SWIG_1(hkvVec3* result, hkvVec3* self, float x, float y, float z)
{
    if (std::abs(x) > 1e-12f) result->x = self->x / x;
    else result->x = 0.0f;

    if (std::abs(y) > 1e-12f) result->y = self->y / y;
    else result->y = 0.0f;

    if (std::abs(z) > 1e-12f) result->z = self->z / z;
    else result->z = 0.0f;

    return result;
}

// ============================================================================
// hkvVec3___len - Length operator (returns 3)
// IDA @ 0x140644900
// Lua __len metamethod for hkvVec3
// ============================================================================
extern "C" int __fastcall hkvVec3___len(hkvVec3* self)
{
    return 3;
}

// ============================================================================
// hkvVec3_Concat - String concatenation
// IDA @ 0x140644b50
// Lua .. operator for hkvVec3
// ============================================================================
extern "C" std::int64_t __fastcall hkvVec3_Concat(lua_State* L)
{
    const char* pszString = nullptr;
    int iIndex = -1;

    // Check if there's a string argument
    if (lua_isstring(L, -1)) {
        pszString = lua_tolstring(L, iIndex--, nullptr);
    }

    // Get self
    if (!lua_isuserdata(L, iIndex) && lua_type(L, iIndex)) {
        luaL_error(L, "Expected %s* as parameter %d", "hkvVec3", iIndex);
    }

    hkvVec3* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, iIndex, (void**)&self, swig_types[30], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "hkvVec3");
    }

    // Get string if at -2
    if (--iIndex == -2) {
        pszString = lua_tolstring(L, -2, nullptr);
    }

    // Format vector
    char* pszBuffer = new char[128 + (pszString ? strlen(pszString) + 1 : 0)];
    sprintf(pszBuffer, "[%1.2f,%1.2f,%1.2f]", self->x, self->y, self->z);

    // Concatenate
    if (iIndex == -3 && pszString) {
        // String + Vector
        std::size_t uiLen = strlen(pszString);
        std::size_t vecLen = strlen(pszBuffer);
        memmove(pszBuffer + uiLen, pszBuffer, vecLen + 1);
        memcpy(pszBuffer, pszString, uiLen);
    } else if (pszString) {
        // Vector + String
        strcat(pszBuffer, pszString);
    }

    lua_pushstring(L, pszBuffer);
    delete[] pszBuffer;

    return 1;
}

// ============================================================================
// hkvVec3_ToString - String conversion
// IDA @ 0x140644de0
// Lua tostring() for hkvVec3
// ============================================================================
extern "C" std::int64_t __fastcall hkvVec3_ToString(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, -1) && lua_type(L, -1)) {
        luaL_error(L, "Expected %s* as parameter %d", "hkvVec3", -1);
    }

    hkvVec3* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, -1, (void**)&self, swig_types[30], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "hkvVec3");
    }

    char pszBuffer[1024];
    sprintf(pszBuffer, "hkvVec3: [%1.2f,%1.2f,%1.2f]", self->x, self->y, self->z);
    lua_pushstring(L, pszBuffer);

    return 1;
}