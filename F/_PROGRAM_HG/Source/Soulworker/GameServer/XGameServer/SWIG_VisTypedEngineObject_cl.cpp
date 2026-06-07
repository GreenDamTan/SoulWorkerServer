// SWIG_VisTypedEngineObject_cl.cpp
// SWIG Lua bindings for VisTypedEngineObject_cl class
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - VisTypedEngineObject_cl_AddComponent (0x140648820)
// - VisTypedEngineObject_cl_RemoveComponent (0x140648880)
// - VisTypedEngineObject_cl_RemoveComponentOfType__SWIG_0 (0x1406488e0)
// - VisTypedEngineObject_cl_AddComponentOfType (0x1406489a0)
// - VisTypedEngineObject_cl_AddTriggerTarget (0x140648cd0)
// - VisTypedEngineObject_cl_AddTriggerSource (0x140648d10)
// - VisTypedEngineObject_cl_AddAnimation (0x140648d50)
// - VisTypedEngineObject_cl_AddTransitionStateMachine (0x140648d90)
// - VisTypedEngineObject_cl_GetComponentOfType (0x140648dd0)
// - VisTypedEngineObject_cl_GetComponentOfBaseType (0x140648f70)
// - VisTypedEngineObject_cl_Concat (0x140649110)
// - VisTypedEngineObject_cl_ToString (0x140649370)

#include <cstdint>
#include <cstring>

// Forward declarations
struct lua_State;

// Lua API function declarations
extern "C" {
    void lua_pushstring(lua_State* L, const char* s);
    void lua_pushnil(lua_State* L);
    void lua_pushboolean(lua_State* L, int b);
    void lua_settop(lua_State* L, int idx);
    int lua_type(lua_State* L, int idx);
    int lua_toboolean(lua_State* L, int idx);
    int lua_isuserdata(lua_State* L, int idx);
    int lua_isstring(lua_State* L, int idx);
    const char* lua_tolstring(lua_State* L, int idx, std::size_t* len);
    void luaL_error(lua_State* L, const char* fmt, ...);
}

// SWIG type info forward declaration
struct swig_type_info;
extern swig_type_info* swig_types[];
extern "C" std::int64_t SWIG_Lua_ConvertPtr(lua_State* L, int index, void** ptr, swig_type_info* type, char flags);
extern "C" std::int64_t SWIG_lua_isnilstring(lua_State* L, unsigned int idx);

// Forward declarations from Vision Engine
extern void* VBaseObject_operator_new(std::size_t size);

// IVObjectComponent - base component class
struct IVObjectComponent {
    virtual ~IVObjectComponent() {}
    // ... virtual methods
};

// VisTypedEngineObject_cl - typed engine object
struct VisTypedEngineObject_cl {
    virtual ~VisTypedEngineObject_cl() {}
    virtual int AddComponent(VisTypedEngineObject_cl* self, IVObjectComponent* component) = 0;
    virtual int RemoveComponent(VisTypedEngineObject_cl* self, IVObjectComponent* component) = 0;
    // ... other virtual methods
};

// VObjectComponentCollection - component collection
struct VObjectComponentCollection {
    IVObjectComponent* GetComponentOfType(const char* szTypeName);
    IVObjectComponent* GetComponentOfTypeAndName(const char* szTypeName, const char* szName);
};

// ============================================================================
// VisTypedEngineObject_cl_AddComponent - Add component to object
// IDA @ 0x140648820
// Lua binding to add a component
// ============================================================================
extern "C" bool __fastcall VisTypedEngineObject_cl_AddComponent(VisTypedEngineObject_cl* self, IVObjectComponent* component)
{
    return component && self->AddComponent(self, component) == 1;
}

// ============================================================================
// VisTypedEngineObject_cl_RemoveComponent - Remove component from object
// IDA @ 0x140648880
// Lua binding to remove a component
// ============================================================================
extern "C" bool __fastcall VisTypedEngineObject_cl_RemoveComponent(VisTypedEngineObject_cl* self, IVObjectComponent* component)
{
    return component && self->RemoveComponent(self, component) == 1;
}

// ============================================================================
// VisTypedEngineObject_cl_RemoveComponentOfType__SWIG_0 - Remove component by type
// IDA @ 0x1406488e0
// Lua binding to remove a component by type name
// ============================================================================
extern "C" bool __fastcall VisTypedEngineObject_cl_RemoveComponentOfType__SWIG_0(
    VisTypedEngineObject_cl* self,
    const char* szTypeName,
    const char* szOptionalComponentName)
{
    if (!szTypeName) {
        return false;
    }

    IVObjectComponent* pComponent = nullptr;

    // TODO: Get component collection from self
    // if (szOptionalComponentName) {
    //     pComponent = collection->GetComponentOfTypeAndName(szTypeName, szOptionalComponentName);
    // } else {
    //     pComponent = collection->GetComponentOfType(szTypeName);
    // }

    return pComponent && self->RemoveComponent(self, pComponent) == 1;
}

// ============================================================================
// VisTypedEngineObject_cl_AddComponentOfType - Add component by type
// IDA @ 0x1406489a0
// Lua binding to create and add a component by type name
// ============================================================================
extern "C" std::int64_t __fastcall VisTypedEngineObject_cl_AddComponentOfType(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, 1) && lua_type(L, 1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VisTypedEngineObject_cl", 1);
    }

    VisTypedEngineObject_cl* pSelf = nullptr;
    if (SWIG_Lua_ConvertPtr(L, 1, (void**)&pSelf, swig_types[21], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VisTypedEngineObject_cl");
    }

    // Get component type
    if (!SWIG_lua_isnilstring(L, 2)) {
        luaL_error(L, "Expected a string value as parameter 2 for VisTypedEngineObject_AddComponentOfType");
    }

    const char* szComponentType = lua_tolstring(L, 2, nullptr);
    const char* szComponentName = nullptr;

    if (lua_isstring(L, 3)) {
        szComponentName = lua_tolstring(L, 3, nullptr);
    }

    bool bCreateDynamicProperty = true;
    if (lua_type(L, 4) == 1) {  // LUA_TBOOLEAN
        bCreateDynamicProperty = lua_toboolean(L, 4) == 1;
    }

    // TODO: Implement component creation through VTypeManager
    // This requires access to Vision::GetTypeManager() and VType::CreateInstance

    lua_settop(L, 0);
    lua_pushnil(L);
    return 1;
}

// ============================================================================
// VisTypedEngineObject_cl_AddTriggerTarget - Add trigger target
// IDA @ 0x140648cd0
// Lua binding to add a trigger target
// ============================================================================
extern "C" void __fastcall VisTypedEngineObject_cl_AddTriggerTarget(VisTypedEngineObject_cl* self, void* target)
{
    // TODO: Implement trigger target addition
}

// ============================================================================
// VisTypedEngineObject_cl_AddTriggerSource - Add trigger source
// IDA @ 0x140648d10
// Lua binding to add a trigger source
// ============================================================================
extern "C" void __fastcall VisTypedEngineObject_cl_AddTriggerSource(VisTypedEngineObject_cl* self, void* source)
{
    // TODO: Implement trigger source addition
}

// ============================================================================
// VisTypedEngineObject_cl_AddAnimation - Add animation
// IDA @ 0x140648d50
// Lua binding to add an animation
// ============================================================================
extern "C" void __fastcall VisTypedEngineObject_cl_AddAnimation(VisTypedEngineObject_cl* self, void* animation)
{
    // TODO: Implement animation addition
}

// ============================================================================
// VisTypedEngineObject_cl_AddTransitionStateMachine - Add transition state machine
// IDA @ 0x140648d90
// Lua binding to add a transition state machine
// ============================================================================
extern "C" void __fastcall VisTypedEngineObject_cl_AddTransitionStateMachine(VisTypedEngineObject_cl* self, void* stateMachine)
{
    // TODO: Implement state machine addition
}

// ============================================================================
// VisTypedEngineObject_cl_GetComponentOfType - Get component by type
// IDA @ 0x140648dd0
// Lua binding to get a component by type name
// ============================================================================
extern "C" std::int64_t __fastcall VisTypedEngineObject_cl_GetComponentOfType(lua_State* L)
{
    // TODO: Implement component retrieval
    lua_pushnil(L);
    return 1;
}

// ============================================================================
// VisTypedEngineObject_cl_GetComponentOfBaseType - Get component by base type
// IDA @ 0x140648f70
// Lua binding to get a component by base type name
// ============================================================================
extern "C" std::int64_t __fastcall VisTypedEngineObject_cl_GetComponentOfBaseType(lua_State* L)
{
    // TODO: Implement component retrieval by base type
    lua_pushnil(L);
    return 1;
}

// ============================================================================
// VisTypedEngineObject_cl_Concat - String concatenation
// IDA @ 0x140649110
// Lua .. operator for VisTypedEngineObject_cl
// ============================================================================
extern "C" std::int64_t __fastcall VisTypedEngineObject_cl_Concat(lua_State* L)
{
    const char* pszString = nullptr;
    int iIndex = -1;

    // Check if there's a string argument
    if (lua_isstring(L, -1)) {
        pszString = lua_tolstring(L, iIndex--, nullptr);
    }

    // Get self
    if (!lua_isuserdata(L, iIndex) && lua_type(L, iIndex)) {
        luaL_error(L, "Expected %s* as parameter %d", "VisTypedEngineObject_cl", iIndex);
    }

    VisTypedEngineObject_cl* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, iIndex, (void**)&self, swig_types[21], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VisTypedEngineObject_cl");
    }

    // Get string if at -2
    if (--iIndex == -2) {
        pszString = lua_tolstring(L, -2, nullptr);
    }

    // Format object
    std::size_t uiLen = pszString ? strlen(pszString) + 1 : 0;
    char* pszBuffer = new char[uiLen + 128];
    sprintf(pszBuffer, "VisTypedEngineObject_cl: 0x%p", self);

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
// VisTypedEngineObject_cl_ToString - String conversion
// IDA @ 0x140649370
// Lua tostring() for VisTypedEngineObject_cl
// ============================================================================
extern "C" std::int64_t __fastcall VisTypedEngineObject_cl_ToString(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, -1) && lua_type(L, -1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VisTypedEngineObject_cl", -1);
    }

    VisTypedEngineObject_cl* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, -1, (void**)&self, swig_types[21], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VisTypedEngineObject_cl");
    }

    char pszBuffer[1024];
    sprintf(pszBuffer, "VisTypedEngineObject_cl: 0x%p", self);
    lua_pushstring(L, pszBuffer);

    return 1;
}
