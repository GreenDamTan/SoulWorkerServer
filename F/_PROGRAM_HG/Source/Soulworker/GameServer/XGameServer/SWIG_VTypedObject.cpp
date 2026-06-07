// SWIG_VTypedObject.cpp
// SWIG Lua bindings for VTypedObject class
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - VTypedObject_GetType (0x1406471f0)
// - VTypedObject_GetPropertyType (0x140647210)
// - VTypedObject_operator_Se__Se_ (0x140647390)
// - VTypedObject_GetProperties (0x1406473d0)
// - VTypedObject_GetProperty (0x1406476a0)
// - VTypedObject_SetProperty (0x140647dc0)
// - VTypedObject_Concat (0x1406484b0)
// - VTypedObject_ToString (0x140648710)

#include <cstdint>
#include <cstring>

// Forward declarations
struct lua_State;

// Lua API function declarations
extern "C" {
    void lua_pushstring(lua_State* L, const char* s);
    void lua_pushnumber(lua_State* L, double n);
    void lua_pushboolean(lua_State* L, int b);
    void lua_pushnil(lua_State* L);
    void lua_createtable(lua_State* L, int narr, int nrec);
    void lua_settable(lua_State* L, int idx);
    void lua_rawseti(lua_State* L, int idx, int n);
    void lua_settop(lua_State* L, int idx);
    int lua_type(lua_State* L, int idx);
    int lua_isuserdata(lua_State* L, int idx);
    int lua_isstring(lua_State* L, int idx);
    const char* lua_tolstring(lua_State* L, int idx, std::size_t* len);
    void luaL_error(lua_State* L, const char* fmt, ...);
    int luaL_argerror(lua_State* L, int arg, const char* extramsg);
}

// SWIG type info forward declaration
struct swig_type_info;
extern swig_type_info* swig_types[];
extern "C" std::int64_t SWIG_Lua_ConvertPtr(lua_State* L, int index, void** ptr, swig_type_info* type, char flags);
extern "C" void SWIG_Lua_NewPointerObj(lua_State* L, void* ptr, swig_type_info* type, int own);

// Forward declarations from Vision Engine
extern void* VBaseObject_operator_new(std::size_t size);

// Variable types enum (from Vision Engine)
enum VisVariableType {
    VVAR_INT = 0,
    VVAR_FLOAT = 2,
    VVAR_DOUBLE = 3,
    VVAR_INT2 = 4,
    VVAR_BOOL = 5,
    VVAR_VEC3I = 6,
    VVAR_VEC3 = 7,
    VVAR_VEC3D = 8,
    VVAR_STRING = 9,
    VVAR_STRING_OPT = 10,
    VVAR_STRING_WC = 11,
    VVAR_VSTRING = 12,
    VVAR_STRING_RES = 64,
    VVAR_STRING_ENUM = 65,
    VVAR_STRING_CUSTOM = 67,
    VVAR_STRING_HEX = 68,
    VVAR_STRING_WC_STATIC = 69,
    VVAR_STRING_STATIC = 70,
    VVAR_COLOR = 132,
};

// VisVariable_cl structure - property descriptor
struct VisVariable_cl {
    const char* name;
    int type;
    int offset;
    // ... other fields
};

// VTypedObject base class - simplified
struct VTypedObject {
    virtual ~VTypedObject() {}
    virtual const void* GetTypeId() const = 0;
    virtual VisVariable_cl* GetVariable(const char* name) = 0;
    virtual int GetNumVariables() const = 0;
    virtual const char* GetVariableName(int index) const = 0;

    // Helper to get type string
    static const char* GetTypeString(int type) {
        switch (type) {
            case VVAR_INT:
            case VVAR_INT2:
            case VVAR_FLOAT:
            case VVAR_DOUBLE:
                return "number";
            case VVAR_BOOL:
                return "boolean";
            case VVAR_VEC3I:
            case VVAR_VEC3:
            case VVAR_VEC3D:
                return "hkvVec3";
            case VVAR_STRING:
            case VVAR_STRING_OPT:
            case VVAR_STRING_WC:
            case VVAR_VSTRING:
            case VVAR_STRING_RES:
            case VVAR_STRING_ENUM:
            case VVAR_STRING_CUSTOM:
            case VVAR_STRING_HEX:
            case VVAR_STRING_WC_STATIC:
            case VVAR_STRING_STATIC:
                return "string";
            case VVAR_COLOR:
                return "VColorRef";
            default:
                return nullptr;
        }
    }
};

// Forward declaration for LUA_GetValue
extern bool LUA_GetValue(lua_State* L, int idx, void* out);

// ============================================================================
// VTypedObject_GetType - Get type name
// IDA @ 0x1406471f0
// Returns the type name of the object
// ============================================================================
extern "C" const char* __fastcall VTypedObject_GetType(VTypedObject* self)
{
    return *(const char**)self->GetTypeId();
}

// ============================================================================
// VTypedObject_GetPropertyType - Get property type
// IDA @ 0x140647210
// Returns the type name of a property
// ============================================================================
extern "C" const char* __fastcall VTypedObject_GetPropertyType(VTypedObject* self, const char* propName)
{
    if (!propName) {
        return nullptr;
    }

    VisVariable_cl* pVar = self->GetVariable(propName);
    if (pVar) {
        return VTypedObject::GetTypeString(pVar->type);
    }

    return nullptr;
}

// ============================================================================
// VTypedObject_operator_Se__Se_ - Equality comparison
// IDA @ 0x140647390
// Lua __eq metamethod for VTypedObject
// ============================================================================
extern "C" bool __fastcall VTypedObject_operator_Se__Se_(VTypedObject* self, const VTypedObject* other)
{
    return self == other;
}

// ============================================================================
// VTypedObject_GetProperties - Get all properties
// IDA @ 0x1406473d0
// Returns a table of all properties
// ============================================================================
extern "C" std::int64_t __fastcall VTypedObject_GetProperties(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, 1) && lua_type(L, 1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VTypedObject", 1);
    }

    VTypedObject* pTypedObject = nullptr;
    if (SWIG_Lua_ConvertPtr(L, 1, (void**)&pTypedObject, swig_types[16], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VTypedObject");
    }

    lua_createtable(L, 0, 0);

    int iCount = pTypedObject->GetNumVariables();
    for (int i = 0; i < iCount; ++i) {
        const char* szName = pTypedObject->GetVariableName(i);

        lua_createtable(L, 0, 0);

        // Name
        lua_pushstring(L, "Name");
        lua_pushstring(L, szName);
        lua_settable(L, -3);

        // Type
        lua_pushstring(L, "Type");
        VisVariable_cl* pVar = pTypedObject->GetVariable(szName);
        const char* typeStr = pVar ? VTypedObject::GetTypeString(pVar->type) : "unknown";
        lua_pushstring(L, typeStr ? typeStr : "unknown");
        lua_settable(L, -3);

        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}

// ============================================================================
// VTypedObject_GetProperty - Get property value
// IDA @ 0x1406476a0
// Lua getter for VTypedObject properties
// ============================================================================
extern "C" std::int64_t __fastcall VTypedObject_GetProperty(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, 1) && lua_type(L, 1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VTypedObject", 1);
    }

    VTypedObject* pTypedObject = nullptr;
    if (SWIG_Lua_ConvertPtr(L, 1, (void**)&pTypedObject, swig_types[16], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VTypedObject");
    }

    // Get property name
    const char* pszName = nullptr;
    if (!LUA_GetValue(L, 2, &pszName)) {
        luaL_argerror(L, 3, "Expected const char *");
        return 0;
    }

    lua_settop(L, -2);

    VisVariable_cl* pVar = pTypedObject->GetVariable(pszName);
    if (!pVar) {
        luaL_error(L, "Called getter of unknown property '%s'", pszName);
        lua_pushnil(L);
        return 1;
    }

    // TODO: Implement value retrieval based on type
    // This requires access to VisVariable_cl::GetValueDirect
    lua_pushnil(L);
    return 1;
}

// ============================================================================
// VTypedObject_SetProperty - Set property value
// IDA @ 0x140647dc0
// Lua setter for VTypedObject properties
// ============================================================================
extern "C" std::int64_t __fastcall VTypedObject_SetProperty(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, 1) && lua_type(L, 1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VTypedObject", 1);
    }

    VTypedObject* pTypedObject = nullptr;
    if (SWIG_Lua_ConvertPtr(L, 1, (void**)&pTypedObject, swig_types[16], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VTypedObject");
    }

    // Get property name
    const char* pszName = nullptr;
    if (!LUA_GetValue(L, 2, &pszName)) {
        luaL_argerror(L, 3, "Expected const char *");
        return 0;
    }

    VisVariable_cl* pVar = pTypedObject->GetVariable(pszName);
    if (!pVar) {
        luaL_error(L, "Called setter of unknown property '%s'", pszName);
        lua_pushnil(L);
        return 1;
    }

    // TODO: Implement value setting based on type
    // This requires access to VisVariable_cl::SetValueDirect
    lua_settop(L, 0);
    return 0;
}

// ============================================================================
// VTypedObject_Concat - String concatenation
// IDA @ 0x1406484b0
// Lua .. operator for VTypedObject
// ============================================================================
extern "C" std::int64_t __fastcall VTypedObject_Concat(lua_State* L)
{
    const char* pszString = nullptr;
    int iIndex = -1;

    // Check if there's a string argument
    if (lua_isstring(L, -1)) {
        pszString = lua_tolstring(L, iIndex--, nullptr);
    }

    // Get self
    if (!lua_isuserdata(L, iIndex) && lua_type(L, iIndex)) {
        luaL_error(L, "Expected %s* as parameter %d", "VTypedObject", iIndex);
    }

    VTypedObject* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, iIndex, (void**)&self, swig_types[16], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VTypedObject");
    }

    // Get string if at -2
    if (--iIndex == -2) {
        pszString = lua_tolstring(L, -2, nullptr);
    }

    // Format object type
    std::size_t uiLen = pszString ? strlen(pszString) + 1 : 0;
    char* pszBuffer = new char[uiLen + 128];
    sprintf(pszBuffer, "%s", VTypedObject_GetType(self));

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
// VTypedObject_ToString - String conversion
// IDA @ 0x140648710
// Lua tostring() for VTypedObject
// ============================================================================
extern "C" std::int64_t __fastcall VTypedObject_ToString(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, -1) && lua_type(L, -1)) {
        luaL_error(L, "Expected %s* as parameter %d", "VTypedObject", -1);
    }

    VTypedObject* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, -1, (void**)&self, swig_types[16], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "VTypedObject");
    }

    char pszBuffer[1024];
    sprintf(pszBuffer, "%s: 0x%p", VTypedObject_GetType(self), self);
    lua_pushstring(L, pszBuffer);

    return 1;
}
