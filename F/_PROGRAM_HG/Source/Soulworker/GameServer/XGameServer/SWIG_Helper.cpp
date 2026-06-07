// SWIG_Helper.cpp
// SWIG helper functions for Lua bindings
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - SWIG_TypeNameComp (0x1406425b0)
// - SWIG_TypeEquiv (0x1406426c0)
// - SWIG_TypeCheck (0x1406427c0)
// - SWIG_TypeCheckStruct (0x1406428e0)
// - SWIG_TypeCast (0x1406429e0)
// - SWIG_TypeClientData (0x140642a40)
// - SWIG_MangledTypeQueryModule (0x140642ac0)
// - SWIG_TypeQueryModule (0x140642bf0)

#include <cstdint>
#include <cstring>

// SWIG module info structure
struct swig_module_info {
    swig_type_info** types;
    std::size_t size;
    swig_module_info* next;
};

// Forward declaration for Lua state
struct lua_State;

// SWIG type info structure
struct swig_type_info {
    const char* name;
    const char* str;
    swig_cast_info* cast;
    void* clientdata;
    int owndata;
};

// SWIG cast info structure
struct swig_cast_info {
    swig_type_info* type;
    swig_cast_info* next;
    swig_cast_info* prev;
    void* (*converter)(void*, int*);
};

// ============================================================================
// SWIG_TypeNameComp - Compare type names
// IDA @ 0x1406425b0
// Compare two type name segments, skipping spaces
// Returns: 0 if equal, negative if f1<f2, positive if f1>f2
// ============================================================================
extern "C" signed __int64 __fastcall SWIG_TypeNameComp(
    const char* f1, const char* l1,
    const char* f2, const char* l2)
{
    while (true) {
        if (f1 == l1 || f2 == l2) {
            return (l1 - f1) - (l2 - f2);
        }

        // Skip spaces in f1
        while (f1 != l1 && *f1 == ' ') {
            ++f1;
        }

        // Skip spaces in f2
        while (f2 != l2 && *f2 == ' ') {
            ++f2;
        }

        if (*f1 != *f2) {
            break;
        }

        ++f1;
        ++f2;
    }

    if (*f1 <= *f2) {
        return -1;
    } else {
        return 1;
    }
}

// ============================================================================
// SWIG_TypeEquiv - Check if type names are equivalent
// IDA @ 0x1406426c0
// Checks if any '|' separated alternative in nb matches tb
// Returns: true if equivalent, false otherwise
// ============================================================================
extern "C" bool __fastcall SWIG_TypeEquiv(const char* nb, const char* tb)
{
    const char* ne = nb;
    const char* te = tb + strlen(tb);
    bool equiv = false;

    while (!equiv && *ne) {
        const char* nba = ne;

        // Find end of current alternative (delimited by '|')
        while (*ne && *ne != '|') {
            ++ne;
        }

        // Compare this alternative with tb
        equiv = (SWIG_TypeNameComp(nba, ne, tb, te) == 0);

        // Skip the '|' delimiter if present
        if (*ne) {
            ++ne;
        }
    }

    return equiv;
}

// ============================================================================
// SWIG_TypeCheck - Find type in cast list
// IDA @ 0x1406427c0
// Search for a type by name in the cast list and move to front if found
// Returns: pointer to cast info if found, nullptr otherwise
// ============================================================================
extern "C" swig_cast_info* __fastcall SWIG_TypeCheck(const char* c, swig_type_info* ty)
{
    if (!ty) {
        return nullptr;
    }

    for (swig_cast_info* iter = ty->cast; iter; iter = iter->next) {
        const char* name = iter->type->name;

        // Compare names
        int cmp = 0;
        const char* p1 = name;
        const char* p2 = c;

        while (*p1 == *p2) {
            if (!*p1) {
                cmp = 0;
                break;
            }
            ++p1;
            ++p2;
        }

        if (*p1 < *p2) {
            cmp = -1;
        } else if (*p1 > *p2) {
            cmp = 1;
        }

        if (cmp == 0) {
            // Found - move to front for cache efficiency
            if (iter != ty->cast) {
                // Remove from current position
                iter->prev->next = iter->next;
                if (iter->next) {
                    iter->next->prev = iter->prev;
                }

                // Insert at front
                iter->next = ty->cast;
                iter->prev = nullptr;
                if (ty->cast) {
                    ty->cast->prev = iter;
                }
                ty->cast = iter;
            }
            return iter;
        }
    }

    return nullptr;
}

// ============================================================================
// SWIG_TypeCheckStruct - Find type by struct pointer
// IDA @ 0x1406428e0
// Search for a type by struct pointer in the cast list
// Returns: pointer to cast info if found, nullptr otherwise
// ============================================================================
extern "C" swig_cast_info* __fastcall SWIG_TypeCheckStruct(swig_type_info* from, swig_type_info* ty)
{
    if (!ty) {
        return nullptr;
    }

    for (swig_cast_info* iter = ty->cast; iter; iter = iter->next) {
        if (iter->type == from) {
            // Found - move to front for cache efficiency
            if (iter != ty->cast) {
                // Remove from current position
                iter->prev->next = iter->next;
                if (iter->next) {
                    iter->next->prev = iter->prev;
                }

                // Insert at front
                iter->next = ty->cast;
                iter->prev = nullptr;
                if (ty->cast) {
                    ty->cast->prev = iter;
                }
                ty->cast = iter;
            }
            return iter;
        }
    }

    return nullptr;
}

// ============================================================================
// SWIG_TypeCast - Cast pointer to target type
// IDA @ 0x1406429e0
// Uses converter function if available
// Returns: converted pointer, or original if no conversion needed
// ============================================================================
extern "C" void* __fastcall SWIG_TypeCast(swig_cast_info* ty, void* ptr, int* newmemory)
{
    if (ty && ty->converter) {
        return ty->converter(ptr, newmemory);
    }
    return ptr;
}

// ============================================================================
// SWIG_TypeClientData - Set client data for type
// IDA @ 0x140642a40
// Sets client data for all equivalents of a type
// ============================================================================
extern "C" void __fastcall SWIG_TypeClientData(swig_type_info* ti, void* clientdata)
{
    if (!ti) {
        return;
    }

    // Set client data for the type
    ti->clientdata = clientdata;

    // Set for all cast equivalents
    for (swig_cast_info* iter = ti->cast; iter; iter = iter->next) {
        if (iter->type && !iter->type->clientdata) {
            iter->type->clientdata = clientdata;
        }
    }
}

// Forward declarations for Lua state
struct lua_State;

// Lua constants
#define LUA_REGISTRYINDEX (-1001000)
#define LUA_TUSERDATA 7
#define LUA_TTABLE 5
#define LUA_TNIL 0

// Lua API function declarations
extern "C" {
    void lua_pushstring(lua_State* L, const char* s);
    void lua_pushlightuserdata(lua_State* L, void* p);
    void lua_rawget(lua_State* L, int idx);
    void lua_rawset(lua_State* L, int idx);
    int lua_type(lua_State* L, int idx);
    void* lua_touserdata(lua_State* L, int idx);
    void* lua_newuserdata(lua_State* L, std::size_t size);
    void lua_settop(lua_State* L, int idx);
    void lua_pushnil(lua_State* L);
    void lua_getmetatable(lua_State* L, int idx);
    int lua_setmetatable(lua_State* L, int idx);
    void lua_getfield(lua_State* L, int idx, const char* k);
    void lua_createtable(lua_State* L, int narr, int nrec);
    void lua_pushcclosure(lua_State* L, int (*fn)(lua_State*), int n);
    void lua_pushvalue(lua_State* L, int idx);
    void lua_remove(lua_State* L, int idx);
    void lua_call(lua_State* L, int nargs, int nresults);
    int lua_pcall(lua_State* L, int nargs, int nresults, int errfunc);
    int lua_iscfunction(lua_State* L, int idx);
    const char* lua_typename(lua_State* L, int tp);
    int lua_isuserdata(lua_State* L, int idx);
    int lua_gettop(lua_State* L);
    int luaL_loadstring(lua_State* L, const char* s);
    const char* lua_tolstring(lua_State* L, int idx, std::size_t* len);
    int lua_pushfstring(lua_State* L, const char* fmt, ...);
}

// SWIG Lua userdata structure
struct swig_lua_userdata {
    void* ptr;
    swig_type_info* type;
    int own;
};

// ============================================================================
// SWIG_MangledTypeQueryModule - Query type by mangled name
// IDA @ 0x140642ac0
// Binary search through module types
// Returns: pointer to type info if found, nullptr otherwise
// ============================================================================
extern "C" swig_type_info* __fastcall SWIG_MangledTypeQueryModule(
    swig_module_info* start,
    swig_module_info* end,
    const char* name)
{
    if (!start || !name) {
        return nullptr;
    }

    do {
        if (start->size > 0) {
            // Binary search through sorted types
            std::size_t l = 0;
            std::size_t r = start->size - 1;

            while (l <= r) {
                std::size_t i = (r + l) >> 1;
                const char* iname = start->types[i]->name;

                if (!iname) {
                    break;
                }

                int cmp = strcmp(name, iname);
                if (cmp == 0) {
                    return start->types[i];
                }

                if (cmp >= 0) {
                    l = i + 1;
                } else {
                    if (i == 0) {
                        break;
                    }
                    r = i - 1;
                }
            }
        }
        start = start->next;
    } while (start != end);

    return nullptr;
}

// ============================================================================
// SWIG_TypeQueryModule - Query type by human-readable name
// IDA @ 0x140642bf0
// First try mangled name, then search by str field with TypeEquiv
// Returns: pointer to type info if found, nullptr otherwise
// ============================================================================
extern "C" swig_type_info* __fastcall SWIG_TypeQueryModule(
    swig_module_info* start,
    swig_module_info* end,
    const char* name)
{
    // First try exact match on mangled name
    swig_type_info* ret = SWIG_MangledTypeQueryModule(start, end, name);
    if (ret) {
        return ret;
    }

    // Then search by human-readable name (str field)
    swig_module_info* iter = start;
    do {
        for (std::size_t i = 0; i < iter->size; ++i) {
            if (iter->types[i]->str && SWIG_TypeEquiv(iter->types[i]->str, name)) {
                return iter->types[i];
            }
        }
        iter = iter->next;
    } while (iter != end);

    return nullptr;
}

// ============================================================================
// SWIG_Lua_GetModule - Get SWIG module from Lua state
// IDA @ 0x140642ce0
// Retrieve SWIG module info stored in Lua registry
// Returns: pointer to module info, or nullptr if not found
// ============================================================================
extern "C" swig_module_info* __fastcall SWIG_Lua_GetModule(lua_State* L)
{
    swig_module_info* ret = nullptr;

    // Get SWIG runtime data from registry
    lua_pushstring(L, "swig_runtime_data_type_pointer4");
    lua_rawget(L, LUA_REGISTRYINDEX);  // -1001000

    // Check if we got a userdata
    if (lua_type(L, -1) == LUA_TUSERDATA) {  // 7
        ret = (swig_module_info*)lua_touserdata(L, -1);
    }

    // Restore stack
    lua_settop(L, -2);

    return ret;
}

// ============================================================================
// SWIG_Lua_SetModule - Set SWIG module in Lua state
// IDA @ 0x140642d60
// Store SWIG module info in Lua registry
// ============================================================================
extern "C" void __fastcall SWIG_Lua_SetModule(lua_State* L, swig_module_info* module)
{
    lua_pushstring(L, "swig_runtime_data_type_pointer4");
    lua_pushlightuserdata(L, module);
    lua_rawset(L, LUA_REGISTRYINDEX);
}

// Forward declaration
void SWIG_Lua_AddMetatable(lua_State* L, swig_type_info* type);
void SWIG_Lua_get_class_metatable(lua_State* L, const char* name);
const char* lua_typename(lua_State* L, int tp);
int lua_isuserdata(lua_State* L, int idx);
int lua_error(lua_State* L);

// Forward declarations for module get/set
extern "C" std::int64_t SWIG_Lua_module_get(lua_State* L);
extern "C" std::int64_t SWIG_Lua_module_set(lua_State* L);

// ============================================================================
// SWIG_Lua_set_immutable - Error handler for immutable variables
// IDA @ 0x140642db0
// Raises error when trying to modify immutable variable
// ============================================================================
extern "C" std::int64_t __fastcall SWIG_Lua_set_immutable(lua_State* L)
{
    lua_settop(L, -2);
    lua_pushstring(L, "This variable is immutable");
    lua_error(L);
    return 0;
}

// ============================================================================
// SWIG_Lua_AddMetatable - Add metatable to userdata
// IDA @ 0x140643f10
// Sets up metatable for SWIG userdata object
// ============================================================================
extern "C" void __fastcall SWIG_Lua_AddMetatable(lua_State* L, swig_type_info* type)
{
    if (type->clientdata) {
        SWIG_Lua_get_class_metatable(L, *(const char**)type->clientdata);
        if (lua_type(L, -1) == 5) {  // LUA_TTABLE
            lua_setmetatable(L, -2);
        } else {
            lua_settop(L, -2);
        }
    }
}

// ============================================================================
// SWIG_Lua_typename - Get type name of Lua value
// IDA @ 0x140644170
// Returns human-readable type name for userdata or builtin type
// ============================================================================
extern "C" const char* __fastcall SWIG_Lua_typename(lua_State* L, unsigned int tp)
{
    if (lua_isuserdata(L, tp)) {
        swig_lua_userdata* usr = (swig_lua_userdata*)lua_touserdata(L, tp);
        if (usr && usr->type && usr->type->str) {
            return usr->type->str;
        } else {
            return "userdata (unknown type)";
        }
    } else {
        int ltype = lua_type(L, tp);
        return lua_typename(L, ltype);
    }
}

// ============================================================================
// SWIG_Lua_type - Push type name to Lua stack
// IDA @ 0x140644200
// Lua callable function returning type name
// ============================================================================
extern "C" std::int64_t __fastcall SWIG_Lua_type(lua_State* L)
{
    const char* name = SWIG_Lua_typename(L, 1);
    lua_pushstring(L, name);
    return 1;
}

// ============================================================================
// SWIG_Lua_equal - Compare two userdata pointers
// IDA @ 0x140644230
// Returns true if both userdata point to same object
// ============================================================================
extern "C" std::int64_t __fastcall SWIG_Lua_equal(lua_State* L)
{
    if (!lua_isuserdata(L, 1) || !lua_isuserdata(L, 2)) {
        return 0;
    }

    swig_lua_userdata* usr1 = (swig_lua_userdata*)lua_touserdata(L, 1);
    swig_lua_userdata* usr2 = (swig_lua_userdata*)lua_touserdata(L, 2);

    bool result = (usr1->ptr == usr2->ptr);
    lua_pushboolean(L, result);
    return 1;
}

// ============================================================================
// SWIG_Lua_module_get - Get property from module table
// IDA @ 0x140642df0
// Lua __index metamethod for module tables
// ============================================================================
extern "C" std::int64_t __fastcall SWIG_Lua_module_get(lua_State* L)
{
    // Get .get table from metatable
    lua_getmetatable(L, 1);
    lua_pushstring(L, ".get");
    lua_rawget(L, -2);
    lua_remove(L, -2);  // Remove metatable

    if (lua_type(L, -1) == LUA_TTABLE) {
        lua_pushvalue(L, 2);  // Key
        lua_rawget(L, -2);
        lua_remove(L, -2);  // Remove .get table

        if (lua_iscfunction(L, -1)) {
            lua_call(L, 0, 1);
            return 1;
        }
        lua_settop(L, -2);  // Pop result
    }

    lua_settop(L, -2);  // Pop .get table or nil
    lua_pushnil(L);
    return 1;
}

// ============================================================================
// SWIG_Lua_module_set - Set property on module table
// IDA @ 0x140642ef0
// Lua __newindex metamethod for module tables
// ============================================================================
extern "C" std::int64_t __fastcall SWIG_Lua_module_set(lua_State* L)
{
    // Get .set table from metatable
    lua_getmetatable(L, 1);
    lua_pushstring(L, ".set");
    lua_rawget(L, -2);
    lua_remove(L, -2);  // Remove metatable from stack (index becomes -1)

    if (lua_type(L, -1) == LUA_TTABLE) {
        lua_pushvalue(L, 2);  // Key
        lua_rawget(L, -2);
        lua_remove(L, -2);  // Remove .set table

        if (lua_iscfunction(L, -1)) {
            lua_pushvalue(L, 3);  // Value
            lua_call(L, 1, 0);
            return 0;
        }
    }

    // Fallback: direct set on table
    lua_settop(L, 3);
    lua_rawset(L, 1);
    return 0;
}

// ============================================================================
// SWIG_Lua_module_begin - Begin module registration
// IDA @ 0x140642ff0
// Creates module table with metatable and get/set subtables
// ============================================================================
extern "C" void __fastcall SWIG_Lua_module_begin(lua_State* L, const char* name)
{
    // Create module table and register it
    lua_pushstring(L, name);
    lua_createtable(L, 0, 0);

    // Create metatable with __index and __newindex
    lua_createtable(L, 0, 0);
    lua_pushstring(L, "__index");
    lua_pushcclosure(L, (int (*)(lua_State*))SWIG_Lua_module_get, 0);
    lua_rawset(L, -3);
    lua_pushstring(L, "__newindex");
    lua_pushcclosure(L, (int (*)(lua_State*))SWIG_Lua_module_set, 0);
    lua_rawset(L, -3);

    // Create .get and .set subtables
    lua_pushstring(L, ".get");
    lua_createtable(L, 0, 0);
    lua_rawset(L, -3);
    lua_pushstring(L, ".set");
    lua_createtable(L, 0, 0);
    lua_rawset(L, -3);

    // Set metatable on module table
    lua_setmetatable(L, -2);

    // Register module in global table
    lua_rawset(L, -3);

    // Leave module table on stack
    lua_pushstring(L, name);
    lua_rawget(L, -2);
}

// ============================================================================
// SWIG_Lua_module_add_variable - Add variable getter/setter to module
// IDA @ 0x140643150
// Registers a variable with optional getter and setter functions
// ============================================================================
extern "C" void __fastcall SWIG_Lua_module_add_variable(
    lua_State* L,
    const char* name,
    int (*getFn)(lua_State*),
    int (*setFn)(lua_State*))
{
    // Add to .get table
    lua_getmetatable(L, -1);
    lua_pushstring(L, ".get");
    lua_rawget(L, -2);
    lua_pushstring(L, name);
    lua_pushcclosure(L, getFn, 0);
    lua_rawset(L, -3);
    lua_settop(L, -2);

    // Add to .set table if setter provided
    if (setFn) {
        lua_pushstring(L, ".set");
        lua_rawget(L, -2);
        lua_pushstring(L, name);
        lua_pushcclosure(L, setFn, 0);
        lua_rawset(L, -3);
        lua_settop(L, -2);
    }

    lua_settop(L, -2);
}

// ============================================================================
// SWIG_Lua_module_add_function - Add function to module
// IDA @ 0x140643260
// Registers a function in the module table
// ============================================================================
extern "C" void __fastcall SWIG_Lua_module_add_function(
    lua_State* L,
    const char* name,
    int (*fn)(lua_State*))
{
    lua_pushstring(L, name);
    lua_pushcclosure(L, fn, 0);
    lua_rawset(L, -3);
}

// ============================================================================
// SWIG_Lua_get_class_registry - Get/create SWIG class registry
// IDA @ 0x1406437a0
// Returns SWIG class registry table from Lua registry
// ============================================================================
extern "C" void __fastcall SWIG_Lua_get_class_registry(lua_State* L)
{
    lua_pushstring(L, "SWIG");
    lua_rawget(L, LUA_REGISTRYINDEX);

    if (lua_type(L, -1) != LUA_TTABLE) {
        // Create registry if not exists
        lua_settop(L, -2);
        lua_pushstring(L, "SWIG");
        lua_createtable(L, 0, 0);
        lua_rawset(L, LUA_REGISTRYINDEX);
        lua_pushstring(L, "SWIG");
        lua_rawget(L, LUA_REGISTRYINDEX);
    }
}

// ============================================================================
// SWIG_Lua_get_class_metatable - Get class metatable by name
// IDA @ 0x140643850
// Retrieves class metatable from SWIG registry
// ============================================================================
extern "C" void __fastcall SWIG_Lua_get_class_metatable(lua_State* L, const char* cname)
{
    SWIG_Lua_get_class_registry(L);
    lua_pushstring(L, cname);
    lua_rawget(L, -2);
    lua_remove(L, -2);  // Remove registry
}

// Forward declaration for Vision engine class handlers
extern "C" bool VisionLuaClassGet(lua_State* L);
extern "C" void VisionLuaClassSet(lua_State* L);

// Additional Lua API declarations
const void* lua_topointer(lua_State* L, int idx);
const char* lua_tolstring(lua_State* L, int idx, std::size_t* len);
void lua_gettable(lua_State* L, int idx);
void lua_settable(lua_State* L, int idx);

// ============================================================================
// VisionLuaClassGet - Vision Engine property get handler
// IDA @ 0x1406432b0
// Handles property access for Vision Engine objects
// ============================================================================
extern "C" void __fastcall VisionLuaClassGet(lua_State* L)
{
    const char* pKey = lua_tolstring(L, 2, nullptr);
    const void* iPtr = lua_topointer(L, 1);
    lua_pushfstring(L, "$node-%p-%s$", iPtr, pKey);
    lua_gettable(L, -2);
}

// ============================================================================
// VisionLuaClassSet - Vision Engine property set handler
// IDA @ 0x140643530
// Handles property assignment for Vision Engine objects
// ============================================================================
extern "C" void __fastcall VisionLuaClassSet(lua_State* L)
{
    const char* pKey = lua_tolstring(L, 2, nullptr);
    const void* iPtr = lua_topointer(L, 1);
    lua_pushfstring(L, "$node-%p-%s$", iPtr, pKey);
    lua_pushvalue(L, 3);
    lua_settable(L, -2);
}

// ============================================================================
// SWIG_Lua_class_get - __index metamethod for class objects
// IDA @ 0x140643320
// Handles property/method access on class userdata
// ============================================================================
extern "C" bool __fastcall SWIG_Lua_class_get(lua_State* L)
{
    // Get .get table from metatable
    lua_getmetatable(L, -1);
    lua_pushstring(L, ".get");
    lua_rawget(L, -2);
    lua_pushvalue(L, 2);  // Key
    lua_rawget(L, -2);
    lua_remove(L, -2);  // Remove .get table

    if (lua_iscfunction(L, -1)) {
        lua_pushvalue(L, 1);  // Self
        lua_call(L, 1, 1);
        lua_remove(L, -2);  // Remove metatable
        return true;
    }

    // Try .fn table for methods
    lua_settop(L, -2);
    lua_pushstring(L, ".fn");
    lua_rawget(L, -2);
    lua_pushvalue(L, 2);  // Key
    lua_rawget(L, -2);
    lua_remove(L, -2);  // Remove .fn table

    if (lua_type(L, -1) == 6) {  // LUA_TFUNCTION
        lua_remove(L, -2);  // Remove metatable
        return true;
    }

    // Try __getitem
    lua_settop(L, -2);
    lua_pushstring(L, "__getitem");
    lua_rawget(L, -2);

    if (lua_iscfunction(L, -1)) {
        lua_pushvalue(L, 1);  // Self
        lua_pushvalue(L, 2);  // Key
        lua_call(L, 2, 1);
        lua_remove(L, -2);  // Remove metatable
        return true;
    }

    // Fallback to Vision engine handler
    VisionLuaClassGet(L);
    return lua_type(L, -1) != LUA_TNIL;
}

// ============================================================================
// SWIG_Lua_class_set - __newindex metamethod for class objects
// IDA @ 0x1406435b0
// Handles property assignment on class userdata
// ============================================================================
extern "C" std::int64_t __fastcall SWIG_Lua_class_set(lua_State* L)
{
    // Get .set table from metatable
    lua_getmetatable(L, 1);
    lua_pushstring(L, ".set");
    lua_rawget(L, -2);

    if (lua_type(L, -1) == LUA_TTABLE) {
        lua_pushvalue(L, 2);  // Key
        lua_rawget(L, -2);

        if (lua_iscfunction(L, -1)) {
            lua_pushvalue(L, 1);  // Self
            lua_pushvalue(L, 3);  // Value
            lua_call(L, 2, 0);
            return 0;
        }
        lua_settop(L, -2);
    }

    lua_settop(L, -2);

    // Try __setitem
    lua_pushstring(L, "__setitem");
    lua_rawget(L, -2);

    if (lua_iscfunction(L, -1)) {
        lua_pushvalue(L, 1);  // Self
        lua_pushvalue(L, 2);  // Key
        lua_pushvalue(L, 3);  // Value
        lua_call(L, 3, 0);
        lua_remove(L, -2);  // Remove metatable
        return 1;
    }

    // Fallback to Vision engine handler
    VisionLuaClassSet(L);
    return 0;
}

// ============================================================================
// SWIG_Lua_class_destruct - __gc metamethod for class userdata
// IDA @ 0x140643730
// Called when userdata is garbage collected
// ============================================================================
extern "C" std::int64_t __fastcall SWIG_Lua_class_destruct(lua_State* L)
{
    swig_lua_userdata* usr = (swig_lua_userdata*)lua_touserdata(L, -1);

    if (usr->own) {
        swig_lua_class* clss = (swig_lua_class*)usr->type->clientdata;
        if (clss && clss->destructor) {
            clss->destructor(usr->ptr);
        }
    }

    return 0;
}

// SWIG Lua class structure
struct swig_lua_class {
    const char* name;
    swig_type_info* type;
    swig_lua_class** bases;
    struct {
        const char* name;
        int (*getmethod)(lua_State*);
        int (*setmethod)(lua_State*);
    }* attributes;
    struct {
        const char* name;
        int (*method)(lua_State*);
    }* methods;
    void (*destructor)(void*);
};

// ============================================================================
// SWIG_Lua_add_class_variable - Add variable to class metatable
// IDA @ 0x1406438a0
// Registers getter/setter for class property
// ============================================================================
extern "C" void __fastcall SWIG_Lua_add_class_variable(
    lua_State* L,
    const char* name,
    int (*getFn)(lua_State*),
    int (*setFn)(lua_State*))
{
    // Add to .get table
    lua_pushstring(L, ".get");
    lua_rawget(L, -2);
    lua_pushstring(L, name);
    lua_pushcclosure(L, getFn, 0);
    lua_rawset(L, -3);
    lua_settop(L, -2);

    // Add to .set table if setter provided
    if (setFn) {
        lua_pushstring(L, ".set");
        lua_rawget(L, -2);
        lua_pushstring(L, name);
        lua_pushcclosure(L, setFn, 0);
        lua_rawset(L, -3);
        lua_settop(L, -2);
    }
}

// Forward declaration
void SWIG_Lua_add_class_details(lua_State* L, swig_lua_class* clss);

// ============================================================================
// SWIG_Lua_add_class_details - Add class details recursively
// IDA @ 0x140643990
// Adds attributes and methods from class and base classes
// ============================================================================
extern "C" void __fastcall SWIG_Lua_add_class_details(lua_State* L, swig_lua_class* clss)
{
    // Process base classes first
    for (int i = 0; clss->bases[i]; ++i) {
        SWIG_Lua_add_class_details(L, clss->bases[i]);
    }

    // Add attributes
    for (int i = 0; clss->attributes[i].name; ++i) {
        SWIG_Lua_add_class_variable(
            L,
            clss->attributes[i].name,
            clss->attributes[i].getmethod,
            clss->attributes[i].setmethod);
    }

    // Add methods to .fn table
    lua_pushstring(L, ".fn");
    lua_rawget(L, -2);
    for (int i = 0; clss->methods[i].name; ++i) {
        lua_pushstring(L, clss->methods[i].name);
        lua_pushcclosure(L, clss->methods[i].method, 0);
        lua_rawset(L, -3);
    }
    lua_settop(L, -2);

    // Add __ methods directly to metatable
    for (int i = 0; clss->methods[i].name; ++i) {
        if (clss->methods[i].name[0] == '_' && clss->methods[i].name[1] == '_') {
            lua_pushstring(L, clss->methods[i].name);
            lua_pushcclosure(L, clss->methods[i].method, 0);
            lua_rawset(L, -3);
        }
    }
}

// ============================================================================
// SWIG_Lua_init_base_class - Initialize base class pointers
// IDA @ 0x140643c00
// Resolves base class names to actual class pointers
// ============================================================================
extern "C" void __fastcall SWIG_Lua_init_base_class(lua_State* L, swig_lua_class* clss)
{
    swig_module_info* module = SWIG_Lua_GetModule(L);

    for (int i = 0; clss->base_names[i]; ++i) {
        if (!clss->bases[i]) {
            swig_type_info* info = SWIG_TypeQueryModule(module, module, clss->base_names[i]);
            if (info) {
                clss->bases[i] = (swig_lua_class*)info->clientdata;
            }
        }
    }
}

// ============================================================================
// SWIG_Lua_class_register - Register class with Lua
// IDA @ 0x140643cc0
// Creates metatable and registers constructor, methods, and properties
// ============================================================================
extern "C" void __fastcall SWIG_Lua_class_register(lua_State* L, swig_lua_class* clss)
{
    // Register constructor if present
    if (clss->constructor) {
        lua_pushstring(L, clss->name);
        lua_pushcclosure(L, clss->constructor, 0);
        lua_rawset(L, -3);
    }

    // Get class registry
    SWIG_Lua_get_class_registry(L);

    // Create class metatable
    lua_pushstring(L, clss->name);
    lua_createtable(L, 0, 0);

    // Set .type
    lua_pushstring(L, ".type");
    lua_pushstring(L, clss->name);
    lua_rawset(L, -3);

    // Create .get, .set, .fn tables
    lua_pushstring(L, ".get");
    lua_createtable(L, 0, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, ".set");
    lua_createtable(L, 0, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, ".fn");
    lua_createtable(L, 0, 0);
    lua_rawset(L, -3);

    // Set metamethods
    lua_pushstring(L, "__index");
    lua_pushcclosure(L, (int (*)(lua_State*))SWIG_Lua_class_get, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__newindex");
    lua_pushcclosure(L, (int (*)(lua_State*))SWIG_Lua_class_set, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__gc");
    lua_pushcclosure(L, (int (*)(lua_State*))SWIG_Lua_class_destruct, 0);
    lua_rawset(L, -3);

    // Register in class registry
    lua_rawset(L, -3);
    lua_settop(L, -2);

    // Add class details
    SWIG_Lua_get_class_metatable(L, clss->name);
    SWIG_Lua_add_class_details(L, clss);
    lua_settop(L, -2);
}

// SWIG constant info structure
struct swig_lua_const_info {
    int type;
    const char* name;
    long lvalue;
    double dvalue;
    const char* pvalue;
    swig_type_info** ptype;
};

// Forward declaration
void SWIG_Lua_NewPackedObj(lua_State* L, void* ptr, std::size_t size, swig_type_info* type);

// ============================================================================
// SWIG_Lua_InstallConstants - Install constants into module table
// IDA @ 0x1406442e0
// Registers constants (int, float, string, pointer, etc.)
// ============================================================================
extern "C" void __fastcall SWIG_Lua_InstallConstants(lua_State* L, swig_lua_const_info* constants)
{
    for (int i = 0; constants[i].type; ++i) {
        switch (constants[i].type) {
            case 1:  // INT
                lua_pushstring(L, constants[i].name);
                lua_pushnumber(L, (double)constants[i].lvalue);
                lua_rawset(L, -3);
                break;

            case 2:  // FLOAT/DOUBLE
                lua_pushstring(L, constants[i].name);
                lua_pushnumber(L, constants[i].dvalue);
                lua_rawset(L, -3);
                break;

            case 3:  // STRING
                lua_pushstring(L, constants[i].name);
                lua_pushstring(L, constants[i].pvalue);
                lua_rawset(L, -3);
                break;

            case 4:  // POINTER
                lua_pushstring(L, constants[i].name);
                SWIG_Lua_NewPointerObj(L, constants[i].pvalue, *constants[i].ptype, 0);
                lua_rawset(L, -3);
                break;

            case 5:  // PACKED
                lua_pushstring(L, constants[i].name);
                SWIG_Lua_NewPackedObj(L, constants[i].pvalue, constants[i].lvalue, *constants[i].ptype);
                lua_rawset(L, -3);
                break;

            case 6:  // CHAR
                lua_pushstring(L, constants[i].name);
                lua_pushfstring(L, "%c", (int)(char)constants[i].lvalue);
                lua_rawset(L, -3);
                break;

            default:
                continue;
        }
    }
}

// ============================================================================
// SWIG_Lua_NewPackedObj - Create packed userdata
// IDA @ 0x1406440f0
// Creates userdata with packed binary data
// ============================================================================
extern "C" void __fastcall SWIG_Lua_NewPackedObj(lua_State* L, void* ptr, std::size_t size, swig_type_info* type)
{
    if (ptr) {
        void* usr = lua_newuserdata(L, size + sizeof(swig_lua_userdata));
        if (usr) {
            // Copy data
            memcpy((char*)usr + sizeof(swig_lua_userdata), ptr, size);
            ((swig_lua_userdata*)usr)->ptr = (char*)usr + sizeof(swig_lua_userdata);
            ((swig_lua_userdata*)usr)->type = type;
            ((swig_lua_userdata*)usr)->own = 0;
            SWIG_Lua_AddMetatable(L, type);
        }
    } else {
        lua_pushnil(L);
    }
}

// ============================================================================
// SWIG_Lua_dostring - Execute Lua string
// IDA @ 0x1406445c0
// Compiles and executes a Lua string, returns true on error
// ============================================================================
extern "C" bool __fastcall SWIG_Lua_dostring(lua_State* L, const char* str)
{
    if (!str || !*str) {
        return false;
    }

    int top = lua_gettop(L);
    bool error = luaL_loadstring(L, str) || lua_pcall(L, 0, -1, 0);

    if (error) {
        const char* errMsg = lua_tolstring(L, -1, nullptr);
        fprintf(stderr, "%s\n", errMsg);
    }

    lua_settop(L, top);
    return error;
}

// ============================================================================
// SWIG_Lua_NewPointerObj - Create new userdata for pointer
// IDA @ 0x140643f80
// Creates a Lua userdata object wrapping a C++ pointer
// ============================================================================
extern "C" void __fastcall SWIG_Lua_NewPointerObj(lua_State* L, void* ptr, swig_type_info* type, int own)
{
    if (ptr) {
        // Create userdata
        swig_lua_userdata* usr = (swig_lua_userdata*)lua_newuserdata(L, sizeof(swig_lua_userdata));
        usr->ptr = ptr;
        usr->type = type;
        usr->own = own;

        // Add metatable for type
        SWIG_Lua_AddMetatable(L, type);
    } else {
        lua_pushnil(L);
    }
}

// ============================================================================
// SWIG_Lua_ConvertPtr - Convert Lua value to C++ pointer
// IDA @ 0x140644000
// Extracts and type-checks a C++ pointer from Lua userdata
// Returns: 0 on success, -1 on failure
// ============================================================================
extern "C" std::int64_t __fastcall SWIG_Lua_ConvertPtr(
    lua_State* L,
    unsigned int index,
    void** ptr,
    swig_type_info* type,
    char flags)
{
    // Check for nil/none
    if (lua_type(L, index) == 0) {  // LUA_TNIL
        *ptr = nullptr;
        return 0;
    }

    // Get userdata
    swig_lua_userdata* usr = (swig_lua_userdata*)lua_touserdata(L, index);
    if (!usr) {
        return -1;
    }

    // Clear own flag if requested
    if ((flags & 1) != 0) {
        usr->own = 0;
    }

    // If no type checking needed
    if (!type) {
        *ptr = usr->ptr;
        return 0;
    }

    // Check type compatibility
    swig_cast_info* cast = SWIG_TypeCheckStruct(usr->type, type);
    if (!cast) {
        return -1;
    }

    // Perform type conversion
    int newmemory[6] = {0};
    *ptr = SWIG_TypeCast(cast, usr->ptr, newmemory);

    return 0;
}
