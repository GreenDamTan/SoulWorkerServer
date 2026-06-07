// SWIG_hkvMat3.cpp
// SWIG Lua bindings for hkvMat3 class
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - hkvMat3_invert (0x140644f10)
// - hkvMat3___eq (0x140645510)
// - hkvMat3___add (0x140645550)
// - hkvMat3___sub (0x1406455d0)
// - hkvMat3___mul__SWIG_0 (0x140645650)
// - hkvMat3___mul__SWIG_1 (0x1406456d0)
// - hkvMat3___div (0x140645710)
// - hkvMat3___getitem__ (0x140645750)
// - hkvMat3___setitem__ (0x140645780)

#include <cstdint>
#include <cstring>
#include <cmath>

// Forward declarations
struct lua_State;
struct hkvMat3;

// Lua API function declarations
extern "C" {
    void lua_pushstring(lua_State* L, const char* s);
    int lua_type(lua_State* L, int idx);
    int lua_isuserdata(lua_State* L, int idx);
    void luaL_error(lua_State* L, const char* fmt, ...);
}

// SWIG type info forward declaration
struct swig_type_info;
extern swig_type_info* swig_types[];
extern "C" std::int64_t SWIG_Lua_ConvertPtr(lua_State* L, int index, void** ptr, swig_type_info* type, char flags);

// hkvMat3 structure (36 bytes: 3x3 floats)
struct hkvMat3 {
    float m[3][3];

    // Default constructor
    hkvMat3() {
        memset(m, 0, sizeof(m));
        m[0][0] = 1.0f;
        m[1][1] = 1.0f;
        m[2][2] = 1.0f;
    }
};

// ============================================================================
// hkvMat3_invert - Invert matrix
// IDA @ 0x140644f10
// Returns true if inversion succeeded
// ============================================================================
extern "C" bool __fastcall hkvMat3_invert(hkvMat3* self)
{
    // Calculate determinant
    float det = self->m[0][0] * (self->m[1][1] * self->m[2][2] - self->m[1][2] * self->m[2][1])
              - self->m[0][1] * (self->m[1][0] * self->m[2][2] - self->m[1][2] * self->m[2][0])
              + self->m[0][2] * (self->m[1][0] * self->m[2][1] - self->m[1][1] * self->m[2][0]);

    if (std::abs(det) < 1e-12f) {
        return false;  // Singular matrix
    }

    float invDet = 1.0f / det;

    // Calculate inverse
    hkvMat3 result;
    result.m[0][0] = (self->m[1][1] * self->m[2][2] - self->m[1][2] * self->m[2][1]) * invDet;
    result.m[0][1] = (self->m[0][2] * self->m[2][1] - self->m[0][1] * self->m[2][2]) * invDet;
    result.m[0][2] = (self->m[0][1] * self->m[1][2] - self->m[0][2] * self->m[1][1]) * invDet;
    result.m[1][0] = (self->m[1][2] * self->m[2][0] - self->m[1][0] * self->m[2][2]) * invDet;
    result.m[1][1] = (self->m[0][0] * self->m[2][2] - self->m[0][2] * self->m[2][0]) * invDet;
    result.m[1][2] = (self->m[0][2] * self->m[1][0] - self->m[0][0] * self->m[1][2]) * invDet;
    result.m[2][0] = (self->m[1][0] * self->m[2][1] - self->m[1][1] * self->m[2][0]) * invDet;
    result.m[2][1] = (self->m[0][1] * self->m[2][0] - self->m[0][0] * self->m[2][1]) * invDet;
    result.m[2][2] = (self->m[0][0] * self->m[1][1] - self->m[0][1] * self->m[1][0]) * invDet;

    *self = result;
    return true;
}

// ============================================================================
// hkvMat3___eq - Equality comparison
// IDA @ 0x140645510
// Lua __eq metamethod for hkvMat3
// ============================================================================
extern "C" bool __fastcall hkvMat3___eq(const hkvMat3* self, hkvMat3* pRhs)
{
    if (!pRhs) {
        return false;
    }

    const float epsilon = 0.00001f;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (std::abs(self->m[i][j] - pRhs->m[i][j]) >= epsilon) {
                return false;
            }
        }
    }
    return true;
}

// ============================================================================
// hkvMat3___add - Addition operator
// IDA @ 0x140645550
// Lua __add metamethod for hkvMat3
// ============================================================================
extern "C" hkvMat3* __fastcall hkvMat3___add(hkvMat3* result, hkvMat3* self, const hkvMat3* pRhs)
{
    if (pRhs) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result->m[i][j] = self->m[i][j] + pRhs->m[i][j];
            }
        }
    } else {
        // Copy self if rhs is null
        *result = *self;
    }

    return result;
}

// ============================================================================
// hkvMat3___sub - Subtraction operator
// IDA @ 0x1406455d0
// Lua __sub metamethod for hkvMat3
// ============================================================================
extern "C" hkvMat3* __fastcall hkvMat3___sub(hkvMat3* result, hkvMat3* self, const hkvMat3* pRhs)
{
    if (pRhs) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result->m[i][j] = self->m[i][j] - pRhs->m[i][j];
            }
        }
    } else {
        // Copy self if rhs is null
        *result = *self;
    }

    return result;
}

// ============================================================================
// hkvMat3___mul__SWIG_0 - Matrix-scalar multiplication
// IDA @ 0x140645650
// Lua __mul metamethod for hkvMat3 (scalar)
// ============================================================================
extern "C" hkvMat3* __fastcall hkvMat3___mul__SWIG_0(hkvMat3* result, hkvMat3* self, float f)
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result->m[i][j] = self->m[i][j] * f;
        }
    }
    return result;
}

// ============================================================================
// hkvMat3___mul__SWIG_1 - Matrix-matrix multiplication
// IDA @ 0x1406456d0
// Lua __mul metamethod for hkvMat3 (matrix)
// ============================================================================
extern "C" hkvMat3* __fastcall hkvMat3___mul__SWIG_1(hkvMat3* result, hkvMat3* self, const hkvMat3* pRhs)
{
    if (!pRhs) {
        *result = *self;
        return result;
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result->m[i][j] = 0;
            for (int k = 0; k < 3; ++k) {
                result->m[i][j] += self->m[i][k] * pRhs->m[k][j];
            }
        }
    }

    return result;
}

// ============================================================================
// hkvMat3___div - Division operator (scalar)
// IDA @ 0x140645710
// Lua __div metamethod for hkvMat3
// ============================================================================
extern "C" hkvMat3* __fastcall hkvMat3___div(hkvMat3* result, hkvMat3* self, float f)
{
    if (std::abs(f) > 1e-12f) {
        float invF = 1.0f / f;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result->m[i][j] = self->m[i][j] * invF;
            }
        }
    } else {
        // Avoid division by zero
        memset(result, 0, sizeof(hkvMat3));
    }

    return result;
}

// ============================================================================
// hkvMat3___getitem__ - Index operator (get element)
// IDA @ 0x140645750
// Lua __getitem metamethod for hkvMat3
// ============================================================================
extern "C" float __fastcall hkvMat3___getitem__(hkvMat3* self, int index)
{
    if (index >= 0 && index < 9) {
        int row = index / 3;
        int col = index % 3;
        return self->m[row][col];
    }
    return 0.0f;
}

// ============================================================================
// hkvMat3___setitem__ - Index assignment operator
// IDA @ 0x140645780
// Lua __setitem metamethod for hkvMat3
// ============================================================================
extern "C" void __fastcall hkvMat3___setitem__(hkvMat3* self, int index, float value)
{
    if (index >= 0 && index < 9) {
        int row = index / 3;
        int col = index % 3;
        self->m[row][col] = value;
    }
}

// ============================================================================
// hkvMat3_clone - Clone matrix
// IDA @ 0x1406454c0
// Creates a copy of the matrix
// ============================================================================
extern "C" hkvMat3* __fastcall hkvMat3_clone(hkvMat3* result, const hkvMat3* self)
{
    *result = *self;
    return result;
}

// ============================================================================
// hkvMat3_ToString - String conversion
// IDA @ 0x140645b40
// Lua tostring() for hkvMat3
// ============================================================================
extern "C" std::int64_t __fastcall hkvMat3_ToString(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, -1) && lua_type(L, -1)) {
        luaL_error(L, "Expected %s* as parameter %d", "hkvMat3", -1);
    }

    hkvMat3* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, -1, (void**)&self, swig_types[29], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "hkvMat3");
    }

    char pszBuffer[1024];
    sprintf(pszBuffer, "hkvMat3 CM: c0[%1.2f,%1.2f,%1.2f]  c1[%1.2f,%1.2f,%1.2f] c2[%1.2f,%1.2f,%1.2f]",
            self->m[0][0], self->m[0][1], self->m[0][2],
            self->m[1][0], self->m[1][1], self->m[1][2],
            self->m[2][0], self->m[2][1], self->m[2][2]);
    lua_pushstring(L, pszBuffer);

    return 1;
}

// ============================================================================
// hkvMat3_Concat - String concatenation
// IDA @ 0x1406457c0
// Lua .. operator for hkvMat3
// ============================================================================
extern "C" std::int64_t __fastcall hkvMat3_Concat(lua_State* L)
{
    const char* pszString = nullptr;
    int iIndex = -1;

    // Check if there's a string argument
    if (lua_isstring(L, -1)) {
        pszString = lua_tolstring(L, iIndex--, nullptr);
    }

    // Get self
    if (!lua_isuserdata(L, iIndex) && lua_type(L, iIndex)) {
        luaL_error(L, "Expected %s* as parameter %d", "hkvMat3", iIndex);
    }

    hkvMat3* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, iIndex, (void**)&self, swig_types[29], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "hkvMat3");
    }

    // Get string if at -2
    if (--iIndex == -2) {
        pszString = lua_tolstring(L, -2, nullptr);
    }

    // Format matrix
    std::size_t uiLen = pszString ? strlen(pszString) + 1 : 0;
    char* pszBuffer = new char[uiLen + 256];
    sprintf(pszBuffer, "[%1.2f,%1.2f,%1.2f][%1.2f,%1.2f,%1.2f][%1.2f,%1.2f,%1.2f]",
            self->m[0][0], self->m[0][1], self->m[0][2],
            self->m[1][0], self->m[1][1], self->m[1][2],
            self->m[2][0], self->m[2][1], self->m[2][2]);

    // Concatenate
    if (iIndex == -3) {
        // String + Matrix
        std::size_t bufferLen = strlen(pszBuffer);
        memcpy(pszBuffer + bufferLen, pszString, uiLen);
    } else if (pszString) {
        // Matrix + String
        memmove(pszBuffer + uiLen, pszBuffer, strlen(pszBuffer) + 1);
        memcpy(pszBuffer, pszString, uiLen);
    }

    lua_pushstring(L, pszBuffer);
    delete[] pszBuffer;

    return 1;
}
