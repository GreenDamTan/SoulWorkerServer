// SWIG_VColorRef.cpp
// SWIG Lua bindings for VColorRef class
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - new_VColorRef__SWIG_3 (0x140646950)
// - VColorRef_Clone (0x1406469d0)
// - VColorRef_Concat (0x140646a20)
// - VColorRef_ToString (0x140646cb0)

#include <cstdint>
#include <cstring>

// Forward declarations
struct lua_State;

// Lua API function declarations
extern "C" {
    void lua_pushstring(lua_State* L, const char* s);
    int lua_type(lua_State* L, int idx);
    int lua_isuserdata(lua_State* L, int idx);
    int lua_isstring(lua_State* L, int idx);
    const char* lua_tolstring(lua_State* L, int idx, std::size_t* len);
    void luaL_error(lua_State* L, const char* fmt, ...);
}

// SWIG type info forward declaration
struct swig_type_info;
extern swig_type_info* swig_types[];
extern "C" std::int64_t SWIG_Lua_ConvertPtr(lua_State* L, int index, void** ptr, swig_type_info* type, char flags);

// Forward declarations from Vision Engine
extern void* VBaseObject_operator_new(std::size_t size);

// VColorRef structure (4 bytes: RGBA)
struct VColorRef {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    // Default constructor
    VColorRef() : r(0), g(0), b(0), a(255) {}

    // Constructor with RGBA values
    VColorRef(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255)
        : r(_r), g(_g), b(_b), a(_a) {}
};

// ============================================================================
// new_VColorRef__SWIG_3 - Create new VColorRef from existing
// IDA @ 0x140646950
// Creates a copy of the color
// ============================================================================
extern "C" VColorRef* __fastcall new_VColorRef__SWIG_3(const VColorRef* other)
{
    VColorRef* pColor = (VColorRef*)VBaseObject_operator_new(sizeof(VColorRef));

    if (pColor) {
        new (pColor) VColorRef(other->r, other->g, other->b, other->a);
    }

    return pColor;
}

// ============================================================================
// VColorRef_Clone - Clone color
// IDA @ 0x1406469d0
// Creates a copy of the color
// ============================================================================
extern "C" VColorRef* __fastcall VColorRef_Clone(VColorRef* result, VColorRef* self)
{
    new (result) VColorRef(self->r, self->g, self->b, self->a);
    return result;
}

// ============================================================================
// VColorRef_Concat - String concatenation
// IDA @ 0x140646a20
// Lua .. operator for VColorRef
// ============================================================================
extern "C" std::int64_t __fastcall VColorRef_Concat(lua_State* L)
{
    const char* pszString = nullptr;
    int iIndex = -1;

    // Check if there's a string argument
    if (lua_isstring(L, -1)) {
        pszString = lua_tolstring(L, iIndex--, nullptr);
    }

    // Get self
    if (!lua_isuserdata(L, iIndex) && lua_type(L, iIndex)) {
        luaL_error(L, "Expected %s* as parameter %d", "VColorRef", iIndex);
    }

    VColorRef* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, iIndex, (void**)&self, swig_types[10], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VColorRef");
    }

    // Get string if at -2
    if (--iIndex == -2) {
        pszString = lua_tolstring(L, -2, nullptr);
    }

    // Format color
    std::size_t uiLen = pszString ? strlen(pszString) + 1 : 0;
    char* pszBuffer = new char[uiLen + 32];
    sprintf(pszBuffer, "[%d,%d,%d,%d]", self->r, self->g, self->b, self->a);

    // Concatenate
    if (iIndex == -3) {
        // String + Color
        std::size_t bufferLen = strlen(pszBuffer);
        memcpy(pszBuffer + bufferLen, pszString, uiLen);
    } else if (pszString) {
        // Color + String
        memmove(pszBuffer + uiLen, pszBuffer, strlen(pszBuffer) + 1);
        memcpy(pszBuffer, pszString, uiLen);
    }

    lua_pushstring(L, pszBuffer);
    delete[] pszBuffer;

    return 1;
}

// ============================================================================
// VColorRef_ToString - String conversion
// IDA @ 0x140646cb0
// Lua tostring() for VColorRef
// ============================================================================
extern "C" std::int64_t __fastcall VColorRef_ToString(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, -1) && lua_type(L, -1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VColorRef", -1);
    }

    VColorRef* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, -1, (void**)&self, swig_types[10], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VColorRef");
    }

    char pszBuffer[1024];
    sprintf(pszBuffer, "VColorRef: R %d, G %d, B %d, A %d", self->r, self->g, self->b, self->a);
    lua_pushstring(L, pszBuffer);

    return 1;
}
