// SWIG_VisBaseEntity_cl.cpp
// SWIG Lua bindings for VisBaseEntity_cl class
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - VisBaseEntity_cl_SetMesh__SWIG_0 (0x14064a180)
// - VisBaseEntity_cl_SetMesh__SWIG_1 (0x14064a1d0)
// - VisBaseEntity_cl_SetThinkFunctionStatus (0x14064a200)
// - VisBaseEntity_cl_GetThinkFunctionStatus (0x14064a2d0)
// - VisBaseEntity_cl_HasMesh (0x14064a310)
// - VisBaseEntity_cl_SetCastShadows (0x14064a350)
// - VisBaseEntity_cl_GetCastShadows (0x14064a380)
// - VisBaseEntity_cl_SetScaling__SWIG_0 (0x14064a3c0)
// - VisBaseEntity_cl_SetScaling__SWIG_1 (0x14064a400)
// - VisBaseEntity_cl_SetScaling__SWIG_2 (0x14064a470)
// - VisBaseEntity_cl_GetScaling (0x14064a4c0)
// - VisBaseEntity_cl_GetCustomTraceBBox (0x14064a530)
// - VisBaseEntity_cl_SetTraceAccuracy (0x14064a580)
// - VisBaseEntity_cl_WasVisibleInLastFrame (0x14064a5b0)
// - VisBaseEntity_cl_WasVisibleInAnyLastFrame (0x14064a5f0)
// - VisBaseEntity_cl_GetBoundingBox (0x14064a630)
// - VisBaseEntity_cl_GetCollisionBoundingBox (0x14064a680)
// - VisBaseEntity_cl_SetVisible (0x14064a6d0)
// - VisBaseEntity_cl_IsVisible (0x14064a730)
// - VisBaseEntity_cl_SetVisibleBitmask (0x14064a770)
// - VisBaseEntity_cl_GetVisibleBitmask (0x14064a7c0)
// - VisBaseEntity_cl_SetLightInfluenceBitmask (0x14064a800)
// - VisBaseEntity_cl_GetLightInfluenceBitmask (0x14064a840)
// - VisBaseEntity_cl_Concat (0x14064b110)
// - VisBaseEntity_cl_ToString (0x14064b490)

#include <cstdint>
#include <cstring>

// Forward declarations
struct lua_State;
struct hkvVec3;
struct hkvAlignedBBox;

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

// hkvAlignedBBox structure (24 bytes: 2 hkvVec3)
struct hkvAlignedBBox {
    hkvVec3 m_vMin;
    hkvVec3 m_vMax;

    hkvAlignedBBox() {
        m_vMin.x = 1e18f; m_vMin.y = 1e18f; m_vMin.z = 1e18f;
        m_vMax.x = -1e18f; m_vMax.y = -1e18f; m_vMax.z = -1e18f;
    }
};

// VDynamicMesh forward declaration
struct VDynamicMesh;

// VisVisibilityData structure
struct VisVisibilityData {
    unsigned int m_uiVisibleBitmask;
    // ... other fields
};

// VisBaseEntity_cl - base entity class
struct VisBaseEntity_cl {
    VisVisibilityData VVisibilityData;

    virtual ~VisBaseEntity_cl() {}

    // Mesh methods
    virtual int SetMesh_2(const char* szMeshFile) = 0;
    virtual void SetMesh(VDynamicMesh* pMesh) = 0;
    virtual bool HasMesh() const = 0;

    // Shadow methods
    virtual void SetCastShadows(bool bCast) = 0;
    virtual bool GetCastShadows() const = 0;

    // Scaling methods
    virtual bool SetScaling(float fScaling) = 0;
    virtual const hkvVec3* GetScaling() const = 0;

    // Bounding box methods
    virtual const hkvAlignedBBox* GetCustomTraceBBox() const = 0;
    virtual void GetCurrentVisBoundingBox(hkvAlignedBBox* pBox) const = 0;

    // Think function
    virtual void SetThinkFunctionStatus(bool bEnable) = 0;
    virtual bool GetThinkFunctionStatus() const = 0;

    // Visibility
    virtual void SetVisibleBitmask(VisVisibilityData* pData, unsigned int mask) = 0;
    virtual unsigned int GetVisibleBitmask() const = 0;
    virtual void SetLightInfluenceBitmask(unsigned int mask) = 0;
    virtual unsigned int GetLightInfluenceBitmask() const = 0;
};

// ============================================================================
// VisBaseEntity_cl_SetMesh__SWIG_0 - Set mesh from file
// IDA @ 0x14064a180
// ============================================================================
extern "C" bool __fastcall VisBaseEntity_cl_SetMesh__SWIG_0(VisBaseEntity_cl* self, const char* szMeshFile)
{
    return self->SetMesh_2(szMeshFile) == 1;
}

// ============================================================================
// VisBaseEntity_cl_SetMesh__SWIG_1 - Set mesh from VDynamicMesh
// IDA @ 0x14064a1d0
// ============================================================================
extern "C" char __fastcall VisBaseEntity_cl_SetMesh__SWIG_1(VisBaseEntity_cl* self, VDynamicMesh* pMesh)
{
    self->SetMesh(pMesh);
    return 1;
}

// ============================================================================
// VisBaseEntity_cl_SetThinkFunctionStatus - Set think function status
// IDA @ 0x14064a200
// ============================================================================
extern "C" void __fastcall VisBaseEntity_cl_SetThinkFunctionStatus(VisBaseEntity_cl* self, bool bEnable)
{
    // Iterate through components and update VScriptComponent think status
    // TODO: Implement component iteration
    self->SetThinkFunctionStatus(bEnable);
}

// ============================================================================
// VisBaseEntity_cl_GetThinkFunctionStatus - Get think function status
// IDA @ 0x14064a2d0
// ============================================================================
extern "C" bool __fastcall VisBaseEntity_cl_GetThinkFunctionStatus(VisBaseEntity_cl* self)
{
    return self->GetThinkFunctionStatus() == 1;
}

// ============================================================================
// VisBaseEntity_cl_HasMesh - Check if entity has mesh
// IDA @ 0x14064a310
// ============================================================================
extern "C" bool __fastcall VisBaseEntity_cl_HasMesh(VisBaseEntity_cl* self)
{
    return self->HasMesh() == 1;
}

// ============================================================================
// VisBaseEntity_cl_SetCastShadows - Set cast shadows
// IDA @ 0x14064a350
// ============================================================================
extern "C" void __fastcall VisBaseEntity_cl_SetCastShadows(VisBaseEntity_cl* self, bool bValue)
{
    self->SetCastShadows(bValue);
}

// ============================================================================
// VisBaseEntity_cl_GetCastShadows - Get cast shadows
// IDA @ 0x14064a380
// ============================================================================
extern "C" bool __fastcall VisBaseEntity_cl_GetCastShadows(VisBaseEntity_cl* self)
{
    return self->GetCastShadows() == 1;
}

// ============================================================================
// VisBaseEntity_cl_SetScaling__SWIG_0 - Set uniform scaling
// IDA @ 0x14064a3c0
// ============================================================================
extern "C" bool __fastcall VisBaseEntity_cl_SetScaling__SWIG_0(VisBaseEntity_cl* self, float fScaling)
{
    return self->SetScaling(fScaling) == 1;
}

// ============================================================================
// VisBaseEntity_cl_SetScaling__SWIG_1 - Set scaling from hkvVec3
// IDA @ 0x14064a400
// ============================================================================
extern "C" bool __fastcall VisBaseEntity_cl_SetScaling__SWIG_1(VisBaseEntity_cl* self, const hkvVec3* vScaling)
{
    // TODO: Implement non-uniform scaling
    return false;
}

// ============================================================================
// VisBaseEntity_cl_SetScaling__SWIG_2 - Set scaling from floats
// IDA @ 0x14064a470
// ============================================================================
extern "C" bool __fastcall VisBaseEntity_cl_SetScaling__SWIG_2(VisBaseEntity_cl* self, float x, float y, float z)
{
    // TODO: Implement non-uniform scaling from floats
    return false;
}

// ============================================================================
// VisBaseEntity_cl_GetScaling - Get scaling
// IDA @ 0x14064a4c0
// ============================================================================
extern "C" hkvVec3* __fastcall VisBaseEntity_cl_GetScaling(hkvVec3* result, VisBaseEntity_cl* self)
{
    const hkvVec3* scaling = self->GetScaling();
    if (scaling) {
        *result = *scaling;
    } else {
        result->x = 1.0f;
        result->y = 1.0f;
        result->z = 1.0f;
    }
    return result;
}

// ============================================================================
// VisBaseEntity_cl_GetCustomTraceBBox - Get custom trace bounding box
// IDA @ 0x14064a530
// ============================================================================
extern "C" hkvAlignedBBox* __fastcall VisBaseEntity_cl_GetCustomTraceBBox(hkvAlignedBBox* result, VisBaseEntity_cl* self)
{
    const hkvAlignedBBox* bbox = self->GetCustomTraceBBox();
    if (bbox) {
        *result = *bbox;
    }
    return result;
}

// ============================================================================
// VisBaseEntity_cl_SetTraceAccuracy - Set trace accuracy
// IDA @ 0x14064a580
// ============================================================================
extern "C" void __fastcall VisBaseEntity_cl_SetTraceAccuracy(VisBaseEntity_cl* self, int iAccuracy)
{
    // TODO: Implement SetTraceAccuracy
}

// ============================================================================
// VisBaseEntity_cl_WasVisibleInLastFrame - Check if was visible in last frame
// IDA @ 0x14064a5b0
// ============================================================================
extern "C" bool __fastcall VisBaseEntity_cl_WasVisibleInLastFrame(VisBaseEntity_cl* self)
{
    // TODO: Implement WasVisibleInLastFrame
    return false;
}

// ============================================================================
// VisBaseEntity_cl_WasVisibleInAnyLastFrame - Check if was visible in any last frame
// IDA @ 0x14064a5f0
// ============================================================================
extern "C" bool __fastcall VisBaseEntity_cl_WasVisibleInAnyLastFrame(VisBaseEntity_cl* self)
{
    // TODO: Implement WasVisibleInAnyLastFrame
    return false;
}

// ============================================================================
// VisBaseEntity_cl_GetBoundingBox - Get bounding box
// IDA @ 0x14064a630
// ============================================================================
extern "C" hkvAlignedBBox* __fastcall VisBaseEntity_cl_GetBoundingBox(hkvAlignedBBox* result, VisBaseEntity_cl* self)
{
    hkvAlignedBBox newBox;
    self->GetCurrentVisBoundingBox(&newBox);
    *result = newBox;
    return result;
}

// ============================================================================
// VisBaseEntity_cl_GetCollisionBoundingBox - Get collision bounding box
// IDA @ 0x14064a680
// ============================================================================
extern "C" hkvAlignedBBox* __fastcall VisBaseEntity_cl_GetCollisionBoundingBox(hkvAlignedBBox* result, VisBaseEntity_cl* self)
{
    // TODO: Implement GetCollisionBoundingBox
    return result;
}

// ============================================================================
// VisBaseEntity_cl_SetVisible - Set visibility
// IDA @ 0x14064a6d0
// ============================================================================
extern "C" void __fastcall VisBaseEntity_cl_SetVisible(VisBaseEntity_cl* self, bool bVisible)
{
    unsigned int mask = bVisible ? 0xFFFFFFFF : 0;
    self->SetVisibleBitmask(&self->VVisibilityData, mask);
}

// ============================================================================
// VisBaseEntity_cl_IsVisible - Check if visible
// IDA @ 0x14064a730
// ============================================================================
extern "C" bool __fastcall VisBaseEntity_cl_IsVisible(VisBaseEntity_cl* self)
{
    return self->GetVisibleBitmask() != 0;
}

// ============================================================================
// VisBaseEntity_cl_SetVisibleBitmask - Set visibility bitmask
// IDA @ 0x14064a770
// ============================================================================
extern "C" void __fastcall VisBaseEntity_cl_SetVisibleBitmask(VisBaseEntity_cl* self, unsigned int mask)
{
    self->SetVisibleBitmask(&self->VVisibilityData, mask);
}

// ============================================================================
// VisBaseEntity_cl_GetVisibleBitmask - Get visibility bitmask
// IDA @ 0x14064a7c0
// ============================================================================
extern "C" unsigned int __fastcall VisBaseEntity_cl_GetVisibleBitmask(VisBaseEntity_cl* self)
{
    return self->GetVisibleBitmask();
}

// ============================================================================
// VisBaseEntity_cl_SetLightInfluenceBitmask - Set light influence bitmask
// IDA @ 0x14064a800
// ============================================================================
extern "C" void __fastcall VisBaseEntity_cl_SetLightInfluenceBitmask(VisBaseEntity_cl* self, unsigned int mask)
{
    self->SetLightInfluenceBitmask(mask);
}

// ============================================================================
// VisBaseEntity_cl_GetLightInfluenceBitmask - Get light influence bitmask
// IDA @ 0x14064a840
// ============================================================================
extern "C" unsigned int __fastcall VisBaseEntity_cl_GetLightInfluenceBitmask(VisBaseEntity_cl* self)
{
    return self->GetLightInfluenceBitmask();
}

// ============================================================================
// VisBaseEntity_cl_SetTechnique__SWIG_0 - Set technique
// IDA @ 0x14064a890
// ============================================================================
extern "C" void __fastcall VisBaseEntity_cl_SetTechnique__SWIG_0(VisBaseEntity_cl* self, const char* szTechnique)
{
    // TODO: Implement SetTechnique
}

// ============================================================================
// VisBaseEntity_cl_SetEffect__SWIG_0 - Set effect
// IDA @ 0x14064ab90
// ============================================================================
extern "C" void __fastcall VisBaseEntity_cl_SetEffect__SWIG_0(VisBaseEntity_cl* self, const char* szEffect)
{
    // TODO: Implement SetEffect
}

// ============================================================================
// VisBaseEntity_cl_ClearShaderSet - Clear shader set
// IDA @ 0x14064b0a0
// ============================================================================
extern "C" void __fastcall VisBaseEntity_cl_ClearShaderSet(VisBaseEntity_cl* self)
{
    // TODO: Implement ClearShaderSet
}

// ============================================================================
// VisBaseEntity_cl_SetPrimarySortingKey - Set primary sorting key
// IDA @ 0x14064b0c0
// ============================================================================
extern "C" void __fastcall VisBaseEntity_cl_SetPrimarySortingKey(VisBaseEntity_cl* self, int iKey)
{
    // TODO: Implement SetPrimarySortingKey
}

// ============================================================================
// VisBaseEntity_cl_GetPrimarySortingKey - Get primary sorting key
// IDA @ 0x14064b0f0
// ============================================================================
extern "C" int __fastcall VisBaseEntity_cl_GetPrimarySortingKey(VisBaseEntity_cl* self)
{
    // TODO: Implement GetPrimarySortingKey
    return 0;
}

// ============================================================================
// VisBaseEntity_cl_Concat - String concatenation
// IDA @ 0x14064b110
// Lua .. operator for VisBaseEntity_cl
// ============================================================================
extern "C" std::int64_t __fastcall VisBaseEntity_cl_Concat(lua_State* L)
{
    const char* pszString = nullptr;
    int iIndex = -1;

    // Check if there's a string argument
    if (lua_isstring(L, -1)) {
        pszString = lua_tolstring(L, iIndex--, nullptr);
    }

    // Get self
    if (!lua_isuserdata(L, iIndex) && lua_type(L, iIndex)) {
        luaL_error(L, "Expected %s* as parameter %d", "VisBaseEntity_cl", iIndex);
    }

    VisBaseEntity_cl* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, iIndex, (void**)&self, swig_types[19], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VisBaseEntity_cl");
    }

    // Get string if at -2
    if (--iIndex == -2) {
        pszString = lua_tolstring(L, -2, nullptr);
    }

    // Format object
    std::size_t uiLen = pszString ? strlen(pszString) + 1 : 0;
    char* pszBuffer = new char[uiLen + 128];
    sprintf(pszBuffer, "VisBaseEntity_cl: 0x%p", self);

    // Concatenate
    if (iIndex == -3) {
        // String + Entity
        std::size_t bufferLen = strlen(pszBuffer);
        memcpy(pszBuffer + bufferLen, pszString, uiLen);
    } else if (pszString) {
        // Entity + String
        memmove(pszBuffer + uiLen, pszBuffer, strlen(pszBuffer) + 1);
        memcpy(pszBuffer, pszString, uiLen);
    }

    lua_pushstring(L, pszBuffer);
    delete[] pszBuffer;

    return 1;
}

// ============================================================================
// VisBaseEntity_cl_ToString - String conversion
// IDA @ 0x14064b490
// Lua tostring() for VisBaseEntity_cl
// ============================================================================
extern "C" std::int64_t __fastcall VisBaseEntity_cl_ToString(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, -1) && lua_type(L, -1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VisBaseEntity_cl", -1);
    }

    VisBaseEntity_cl* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, -1, (void**)&self, swig_types[19], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VisBaseEntity_cl");
    }

    char pszBuffer[1024];
    sprintf(pszBuffer, "VisBaseEntity_cl: 0x%p", self);
    lua_pushstring(L, pszBuffer);

    return 1;
}
