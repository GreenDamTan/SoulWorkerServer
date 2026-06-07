// SWIG_VisObject3D_cl.cpp
// SWIG Lua bindings for VisObject3D_cl class
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - VisObject3D_cl_GetPosition (0x1406494d0)
// - VisObject3D_cl_SetPosition__SWIG_1 (0x140649540)
// - VisObject3D_cl_IncPosition__SWIG_1 (0x140649590)
// - VisObject3D_cl_SetOrientation__SWIG_0 (0x1406495e0)
// - VisObject3D_cl_IncOrientation__SWIG_0 (0x140649630)
// - VisObject3D_cl_SetUseEulerAngles (0x140649680)
// - VisObject3D_cl_GetUseEulerAngles (0x1406496b0)
// - VisObject3D_cl_GetMotionDelta (0x1406496f0)
// - VisObject3D_cl_GetMotionDeltaUser (0x140649760)
// - VisObject3D_cl_GetMotionDeltaAnim (0x1406497d0)
// - VisObject3D_cl_HasMotionDelta (0x140649840)
// - VisObject3D_cl_HasMotionDeltaUser (0x140649880)
// - VisObject3D_cl_HasMotionDeltaAnim (0x1406498c0)
// - VisObject3D_cl_HasRotationDelta (0x140649900)
// - VisObject3D_cl_GetRotationDelta (0x140649940)
// - VisObject3D_cl_GetParent (0x1406499b0)
// - VisObject3D_cl_GetChild (0x140649a80)
// - VisObject3D_cl_Concat (0x140649d00)
// - VisObject3D_cl_ToString (0x140649fd0)

#include <cstdint>
#include <cstring>

// Forward declarations
struct lua_State;
struct hkvVec3;

// Lua API function declarations
extern "C" {
    void lua_pushstring(lua_State* L, const char* s);
    void lua_pushnumber(lua_State* L, double n);
    void lua_pushboolean(lua_State* L, int b);
    void lua_pushnil(lua_State* L);
    void lua_settop(lua_State* L, int idx);
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
extern "C" void SWIG_Lua_NewPointerObj(lua_State* L, void* ptr, swig_type_info* type, int own);

// Forward declarations from Vision Engine
extern void* VBaseObject_operator_new(std::size_t size);

// hkvVec3 structure (12 bytes: 3 floats)
struct hkvVec3 {
    float x;
    float y;
    float z;

    hkvVec3() : x(0.0f), y(0.0f), z(0.0f) {}
    hkvVec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

// VisObject3D_cl - 3D object base class
struct VisObject3D_cl {
    virtual ~VisObject3D_cl() {}

    // Position methods
    virtual const hkvVec3* GetPosition() const = 0;
    virtual void SetPosition(const hkvVec3* pos) = 0;
    virtual void IncPosition(const hkvVec3* delta) = 0;

    // Orientation methods
    virtual void SetOrientation(const hkvVec3* orient) = 0;
    virtual void IncOrientation(const hkvVec3* delta) = 0;

    // Parent/child methods
    virtual VisObject3D_cl* GetParent() const = 0;
    virtual VisObject3D_cl* GetChild(int index) const = 0;

    // Motion delta methods
    virtual const hkvVec3* GetMotionDelta() const = 0;
    virtual const hkvVec3* GetMotionDeltaUser() const = 0;
    virtual const hkvVec3* GetMotionDeltaAnim() const = 0;
    virtual bool HasMotionDelta() const = 0;
    virtual bool HasMotionDeltaUser() const = 0;
    virtual bool HasMotionDeltaAnim() const = 0;
    virtual bool HasRotationDelta() const = 0;
    virtual const hkvVec3* GetRotationDelta() const = 0;

    // Euler angles
    virtual void SetUseEulerAngles(bool use) = 0;
    virtual bool GetUseEulerAngles() const = 0;
};

// Forward declaration for LUA_PushObjectProxy
extern void LUA_PushObjectProxy(lua_State* L, void* obj, void* parent);

// ============================================================================
// VisObject3D_cl_GetPosition - Get object position
// IDA @ 0x1406494d0
// Returns the position as hkvVec3
// ============================================================================
extern "C" hkvVec3* __fastcall VisObject3D_cl_GetPosition(hkvVec3* result, VisObject3D_cl* self)
{
    const hkvVec3* pos = self->GetPosition();
    if (pos) {
        *result = *pos;
    } else {
        result->x = 0.0f;
        result->y = 0.0f;
        result->z = 0.0f;
    }
    return result;
}

// ============================================================================
// VisObject3D_cl_SetPosition__SWIG_1 - Set position from floats
// IDA @ 0x140649540
// Sets position from x, y, z components
// ============================================================================
extern "C" void __fastcall VisObject3D_cl_SetPosition__SWIG_1(VisObject3D_cl* self, float x, float y, float z)
{
    hkvVec3 pos(x, y, z);
    self->SetPosition(&pos);
}

// ============================================================================
// VisObject3D_cl_IncPosition__SWIG_1 - Increment position from floats
// IDA @ 0x140649590
// Increments position by x, y, z components
// ============================================================================
extern "C" void __fastcall VisObject3D_cl_IncPosition__SWIG_1(VisObject3D_cl* self, float x, float y, float z)
{
    hkvVec3 delta(x, y, z);
    self->IncPosition(&delta);
}

// ============================================================================
// VisObject3D_cl_SetOrientation__SWIG_0 - Set orientation from floats
// IDA @ 0x1406495e0
// Sets orientation from x, y, z components
// ============================================================================
extern "C" void __fastcall VisObject3D_cl_SetOrientation__SWIG_0(VisObject3D_cl* self, float x, float y, float z)
{
    hkvVec3 orient(x, y, z);
    self->SetOrientation(&orient);
}

// ============================================================================
// VisObject3D_cl_IncOrientation__SWIG_0 - Increment orientation from floats
// IDA @ 0x140649630
// Increments orientation by x, y, z components
// ============================================================================
extern "C" void __fastcall VisObject3D_cl_IncOrientation__SWIG_0(VisObject3D_cl* self, float x, float y, float z)
{
    hkvVec3 delta(x, y, z);
    self->IncOrientation(&delta);
}

// ============================================================================
// VisObject3D_cl_SetUseEulerAngles - Set use euler angles flag
// IDA @ 0x140649680
// ============================================================================
extern "C" void __fastcall VisObject3D_cl_SetUseEulerAngles(VisObject3D_cl* self, bool use)
{
    self->SetUseEulerAngles(use);
}

// ============================================================================
// VisObject3D_cl_GetUseEulerAngles - Get use euler angles flag
// IDA @ 0x1406496b0
// ============================================================================
extern "C" bool __fastcall VisObject3D_cl_GetUseEulerAngles(VisObject3D_cl* self)
{
    return self->GetUseEulerAngles();
}

// ============================================================================
// VisObject3D_cl_GetMotionDelta - Get motion delta
// IDA @ 0x1406496f0
// Returns motion delta as hkvVec3
// ============================================================================
extern "C" hkvVec3* __fastcall VisObject3D_cl_GetMotionDelta(hkvVec3* result, VisObject3D_cl* self)
{
    const hkvVec3* delta = self->GetMotionDelta();
    if (delta) {
        *result = *delta;
    } else {
        result->x = 0.0f;
        result->y = 0.0f;
        result->z = 0.0f;
    }
    return result;
}

// ============================================================================
// VisObject3D_cl_GetMotionDeltaUser - Get user motion delta
// IDA @ 0x140649760
// ============================================================================
extern "C" hkvVec3* __fastcall VisObject3D_cl_GetMotionDeltaUser(hkvVec3* result, VisObject3D_cl* self)
{
    const hkvVec3* delta = self->GetMotionDeltaUser();
    if (delta) {
        *result = *delta;
    } else {
        result->x = 0.0f;
        result->y = 0.0f;
        result->z = 0.0f;
    }
    return result;
}

// ============================================================================
// VisObject3D_cl_GetMotionDeltaAnim - Get animation motion delta
// IDA @ 0x1406497d0
// ============================================================================
extern "C" hkvVec3* __fastcall VisObject3D_cl_GetMotionDeltaAnim(hkvVec3* result, VisObject3D_cl* self)
{
    const hkvVec3* delta = self->GetMotionDeltaAnim();
    if (delta) {
        *result = *delta;
    } else {
        result->x = 0.0f;
        result->y = 0.0f;
        result->z = 0.0f;
    }
    return result;
}

// ============================================================================
// VisObject3D_cl_HasMotionDelta - Check if has motion delta
// IDA @ 0x140649840
// ============================================================================
extern "C" bool __fastcall VisObject3D_cl_HasMotionDelta(VisObject3D_cl* self)
{
    return self->HasMotionDelta();
}

// ============================================================================
// VisObject3D_cl_HasMotionDeltaUser - Check if has user motion delta
// IDA @ 0x140649880
// ============================================================================
extern "C" bool __fastcall VisObject3D_cl_HasMotionDeltaUser(VisObject3D_cl* self)
{
    return self->HasMotionDeltaUser();
}

// ============================================================================
// VisObject3D_cl_HasMotionDeltaAnim - Check if has animation motion delta
// IDA @ 0x1406498c0
// ============================================================================
extern "C" bool __fastcall VisObject3D_cl_HasMotionDeltaAnim(VisObject3D_cl* self)
{
    return self->HasMotionDeltaAnim();
}

// ============================================================================
// VisObject3D_cl_HasRotationDelta - Check if has rotation delta
// IDA @ 0x140649900
// ============================================================================
extern "C" bool __fastcall VisObject3D_cl_HasRotationDelta(VisObject3D_cl* self)
{
    return self->HasRotationDelta();
}

// ============================================================================
// VisObject3D_cl_GetRotationDelta - Get rotation delta
// IDA @ 0x140649940
// ============================================================================
extern "C" hkvVec3* __fastcall VisObject3D_cl_GetRotationDelta(hkvVec3* result, VisObject3D_cl* self)
{
    const hkvVec3* delta = self->GetRotationDelta();
    if (delta) {
        *result = *delta;
    } else {
        result->x = 0.0f;
        result->y = 0.0f;
        result->z = 0.0f;
    }
    return result;
}

// ============================================================================
// VisObject3D_cl_GetParent - Get parent object (Lua binding)
// IDA @ 0x1406499b0
// Returns parent as Lua userdata
// ============================================================================
extern "C" std::int64_t __fastcall VisObject3D_cl_GetParent(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, 1) && lua_type(L, 1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VisObject3D_cl", 1);
    }

    VisObject3D_cl* pSelf = nullptr;
    if (SWIG_Lua_ConvertPtr(L, 1, (void**)&pSelf, swig_types[18], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VisObject3D_cl");
    }

    lua_settop(L, 0);
    VisObject3D_cl* parent = pSelf->GetParent();
    LUA_PushObjectProxy(L, parent, nullptr);

    return 1;
}

// ============================================================================
// VisObject3D_cl_GetChild - Get child object by index
// IDA @ 0x140649a80
// Returns child as Lua userdata
// ============================================================================
extern "C" std::int64_t __fastcall VisObject3D_cl_GetChild(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, 1) && lua_type(L, 1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VisObject3D_cl", 1);
    }

    VisObject3D_cl* pSelf = nullptr;
    if (SWIG_Lua_ConvertPtr(L, 1, (void**)&pSelf, swig_types[18], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VisObject3D_cl");
    }

    // Get child index
    int index = 0;
    // TODO: Get index from Lua parameter

    VisObject3D_cl* child = pSelf->GetChild(index);
    LUA_PushObjectProxy(L, child, pSelf);

    return 1;
}

// ============================================================================
// VisObject3D_cl_Concat - String concatenation
// IDA @ 0x140649d00
// Lua .. operator for VisObject3D_cl
// ============================================================================
extern "C" std::int64_t __fastcall VisObject3D_cl_Concat(lua_State* L)
{
    const char* pszString = nullptr;
    int iIndex = -1;

    // Check if there's a string argument
    if (lua_isstring(L, -1)) {
        pszString = lua_tolstring(L, iIndex--, nullptr);
    }

    // Get self
    if (!lua_isuserdata(L, iIndex) && lua_type(L, iIndex)) {
        luaL_error(L, "Expected %s* as parameter %d", "VisObject3D_cl", iIndex);
    }

    VisObject3D_cl* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, iIndex, (void**)&self, swig_types[18], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VisObject3D_cl");
    }

    // Get string if at -2
    if (--iIndex == -2) {
        pszString = lua_tolstring(L, -2, nullptr);
    }

    // Format object
    std::size_t uiLen = pszString ? strlen(pszString) + 1 : 0;
    char* pszBuffer = new char[uiLen + 128];
    sprintf(pszBuffer, "VisObject3D_cl: 0x%p", self);

    // Concatenate
    if (iIndex == -3) {
        // String + Object
        std::size_t bufferLen = strlen(pszBuffer);
        memcpy(pszBuffer + bufferLen, pszString, uiLen);
    } else if (pszString) {
        // Object + String
        memmove(pszBuffer + uiLen, pszBuffer, strlen(pszBuffer) + 1);
        memcpy(pszBuffer, pszString, uiLen);
    }

    lua_pushstring(L, pszBuffer);
    delete[] pszBuffer;

    return 1;
}

// ============================================================================
// VisObject3D_cl_ToString - String conversion
// IDA @ 0x140649fd0
// Lua tostring() for VisObject3D_cl
// ============================================================================
extern "C" std::int64_t __fastcall VisObject3D_cl_ToString(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, -1) && lua_type(L, -1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VisObject3D_cl", -1);
    }

    VisObject3D_cl* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, -1, (void**)&self, swig_types[18], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VisObject3D_cl");
    }

    char pszBuffer[1024];
    sprintf(pszBuffer, "VisObject3D_cl: 0x%p", self);
    lua_pushstring(L, pszBuffer);

    return 1;
}
