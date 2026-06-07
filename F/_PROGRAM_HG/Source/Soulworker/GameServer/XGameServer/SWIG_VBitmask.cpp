// SWIG_VBitmask.cpp
// SWIG Lua bindings for VBitmask class
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - SWIG_lua_isnilstring (0x140646de0)
// - VBitmask_Clone (0x140646e40)
// - VBitmask_Concat (0x140646e70)
// - VBitmask_ToString (0x1406470e0)

#include <cstdint>
#include <cstring>

// Forward declarations
struct lua_State;

// Lua API function declarations
extern "C" {
    void lua_pushstring(lua_State* L, const char* s);
    int lua_type(lua_State* L, int idx);
    int lua_isstring(lua_State* L, int idx);
    int lua_isuserdata(lua_State* L, int idx);
    const char* lua_tolstring(lua_State* L, int idx, std::size_t* len);
    void luaL_error(lua_State* L, const char* fmt, ...);
}

// SWIG type info forward declaration
struct swig_type_info;
extern swig_type_info* swig_types[];
extern "C" std::int64_t SWIG_Lua_ConvertPtr(lua_State* L, int index, void** ptr, swig_type_info* type, char flags);

// Forward declarations from Vision Engine
extern void* VBaseObject_operator_new(std::size_t size);

// VBitmask structure - wrapper for bitmask/flags
struct VBitmask {
    unsigned int m_uiValue;

    VBitmask() : m_uiValue(0) {}
    VBitmask(unsigned int value) : m_uiValue(value) {}

    // Get value (used by GetQuestID in decompiled code)
    unsigned int GetValue() const { return m_uiValue; }
};

// Forward declaration - this is actually a simple wrapper
// In the original code, CQuestCondition::GetQuestID is used
// but for SWIG bindings, we just treat it as a bitmask value
namespace {
    int GetBitmaskValue(const VBitmask* self) {
        return self->m_uiValue;
    }
}

// ============================================================================
// SWIG_lua_isnilstring - Check if value is string or nil
// IDA @ 0x140646de0
// Helper function to check for string or nil type
// ============================================================================
extern "C" std::int64_t __fastcall SWIG_lua_isnilstring(lua_State* L, unsigned int idx)
{
    unsigned int ret = lua_isstring(L, idx);
    if (!ret) {
        return (lua_type(L, idx) == 0);  // LUA_TNIL = 0
    }
    return ret;
}

// ============================================================================
// VBitmask_Clone - Clone bitmask
// IDA @ 0x140646e40
// Creates a copy of the bitmask
// ============================================================================
extern "C" VBitmask* __fastcall VBitmask_Clone(VBitmask* result, const VBitmask* self)
{
    result->m_uiValue = self->m_uiValue;
    return result;
}

// ============================================================================
// VBitmask_Concat - String concatenation
// IDA @ 0x140646e70
// Lua .. operator for VBitmask
// ============================================================================
extern "C" std::int64_t __fastcall VBitmask_Concat(lua_State* L)
{
    const char* pszString = nullptr;
    int iIndex = -1;

    // Check if there's a string argument
    if (lua_isstring(L, -1)) {
        pszString = lua_tolstring(L, iIndex--, nullptr);
    }

    // Get self
    if (!lua_isuserdata(L, iIndex) && lua_type(L, iIndex)) {
        luaL_error(L, "Expected %s* as parameter %d", "VBitmask", iIndex);
    }

    VBitmask* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, iIndex, (void**)&self, swig_types[9], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VBitmask");
    }

    // Get string if at -2
    if (--iIndex == -2) {
        pszString = lua_tolstring(L, -2, nullptr);
    }

    // Format bitmask as hex
    std::size_t uiLen = pszString ? strlen(pszString) + 1 : 0;
    char* pszBuffer = new char[uiLen + 16];
    sprintf(pszBuffer, "0x%04x", GetBitmaskValue(self));

    // Concatenate
    if (iIndex == -3) {
        // String + Bitmask
        std::size_t bufferLen = strlen(pszBuffer);
        memcpy(pszBuffer + bufferLen, pszString, uiLen);
    } else if (pszString) {
        // Bitmask + String
        memmove(pszBuffer + uiLen, pszBuffer, strlen(pszBuffer) + 1);
        memcpy(pszBuffer, pszString, uiLen);
    }

    lua_pushstring(L, pszBuffer);
    delete[] pszBuffer;

    return 1;
}

// ============================================================================
// VBitmask_ToString - String conversion
// IDA @ 0x1406470e0
// Lua tostring() for VBitmask
// ============================================================================
extern "C" std::int64_t __fastcall VBitmask_ToString(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, -1) && lua_type(L, -1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VBitmask", -1);
    }

    VBitmask* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, -1, (void**)&self, swig_types[9], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VBitmask");
    }

    char pszBuffer[1024];
    sprintf(pszBuffer, "VBitmask [0x%04x]", GetBitmaskValue(self));
    lua_pushstring(L, pszBuffer);

    return 1;
}
