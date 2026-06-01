// SWIG_CMonster_Bindings.cpp
// SWIG Lua bindings for CMonster class
// Reconstructed from IDA decompilation
//
// These are SWIG-generated wrapper functions that allow Lua scripts to call
// CMonster methods. They follow a consistent pattern:
// 1. Argument count validation
// 2. Type checking (lua_isuserdata, lua_isnumber, etc.)
// 3. SWIG_Lua_ConvertPtr to get the C++ object
// 4. Call the actual method
// 5. Push result to Lua stack
// 6. Error handling

#include <cstdint>

// Forward declarations - these are defined in Lua/SWIG headers
struct lua_State;
struct swig_type_info;

// External SWIG functions
extern "C" {
    int lua_gettop(lua_State* L);
    int lua_type(lua_State* L, int idx);
    int lua_isuserdata(lua_State* L, int idx);
    int lua_isnumber(lua_State* L, int idx);
    double lua_tonumber(lua_State* L, int idx);
    void lua_pushnumber(lua_State* L, double n);
    void lua_pushboolean(lua_State* L, int b);
    void lua_pushfstring(lua_State* L, const char* fmt, ...);
    int lua_error(lua_State* L);
    int SWIG_Lua_ConvertPtr(lua_State* L, int idx, void** ptr, swig_type_info* type, int flags);
    const char* SWIG_Lua_typename(lua_State* L, int idx);
}

// SWIG type info structures
extern swig_type_info* swig_types[];

// Forward declaration
class CMonster;
struct tagACTION_BUFFER;

// ============================================================================
// _wrap_CMonster_Destroy - IDA @ 0x140695E80
// ============================================================================
__int64 __fastcall wrap_CMonster_Destroy(lua_State* L)
{
    int v1; // eax
    const char* v2; // rax
    const char* v3; // rax
    CMonster* arg1; // [rsp+38h] [rbp-20h] BYREF
    const char* str; // [rsp+40h] [rbp-18h]

    arg1 = nullptr;
    if (lua_gettop(L) >= 1 && lua_gettop(L) <= 1) {
        if (lua_isuserdata(L, 1) || !lua_type(L, 1)) {
            if (SWIG_Lua_ConvertPtr(L, 1, (void**)&arg1, swig_types[2], 0) >= 0) {
                // TODO: Call actual method - arg1->Destroy();
                return 0;
            }
            if (swig_types[2] && swig_types[2]->str)
                str = swig_types[2]->str;
            else
                str = "void*";
            v3 = SWIG_Lua_typename(L, 1);
            lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "CMonster_Destroy", 1, str, v3);
        } else {
            v2 = SWIG_Lua_typename(L, 1);
            lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "Destroy", 1, "CMonster *", v2);
        }
    } else {
        v1 = lua_gettop(L);
        lua_pushfstring(L, "Error in %s expected %d..%d args, got %d", "Destroy", 1, 1, v1);
    }
    lua_error(L);
    return 0;
}

// ============================================================================
// _wrap_CMonster_MoveTick - IDA @ 0x140696040
// ============================================================================
__int64 __fastcall wrap_CMonster_MoveTick(lua_State* L)
{
    int v1; // eax
    const char* v2; // rax
    const char* v3; // rax
    CMonster* arg1; // [rsp+38h] [rbp-30h] BYREF
    bool result; // [rsp+40h] [rbp-28h]
    const char* str; // [rsp+48h] [rbp-20h]

    arg1 = nullptr;
    if (lua_gettop(L) >= 1 && lua_gettop(L) <= 1) {
        if (lua_isuserdata(L, 1) || !lua_type(L, 1)) {
            if (SWIG_Lua_ConvertPtr(L, 1, (void**)&arg1, swig_types[2], 0) >= 0) {
                // TODO: Call actual method - result = arg1->MoveTick();
                result = false;
                lua_pushboolean(L, result);
                return 1;
            }
            if (swig_types[2] && swig_types[2]->str)
                str = swig_types[2]->str;
            else
                str = "void*";
            v3 = SWIG_Lua_typename(L, 1);
            lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "CMonster_MoveTick", 1, str, v3);
        } else {
            v2 = SWIG_Lua_typename(L, 1);
            lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "MoveTick", 1, "CMonster *", v2);
        }
    } else {
        v1 = lua_gettop(L);
        lua_pushfstring(L, "Error in %s expected %d..%d args, got %d", "MoveTick", 1, 1, v1);
    }
    lua_error(L);
    return 0;
}

// ============================================================================
// _wrap_CMonster_ActionBufferProcess - IDA @ 0x140696230
// ============================================================================
__int64 __fastcall wrap_CMonster_ActionBufferProcess(lua_State* L)
{
    int v1; // eax
    const char* v2; // rax
    const char* v3; // rax
    const char* v4; // rax
    const char* v5; // rax
    CMonster* arg1; // [rsp+40h] [rbp-28h] BYREF
    tagACTION_BUFFER** argp2; // [rsp+48h] [rbp-20h] BYREF
    const char* v9; // [rsp+50h] [rbp-18h]
    const char* str; // [rsp+58h] [rbp-10h]

    arg1 = nullptr;
    if (lua_gettop(L) >= 2 && lua_gettop(L) <= 2) {
        if (lua_isuserdata(L, 1) || !lua_type(L, 1)) {
            if (lua_isuserdata(L, 2)) {
                if (SWIG_Lua_ConvertPtr(L, 1, (void**)&arg1, swig_types[2], 0) >= 0) {
                    if (SWIG_Lua_ConvertPtr(L, 2, (void**)&argp2, swig_types[7], 0) >= 0) {
                        // TODO: Call actual method - arg1->ActionBufferProcess(*argp2);
                        return 0;
                    }
                    if (swig_types[7] && swig_types[7]->str)
                        str = swig_types[7]->str;
                    else
                        str = "void*";
                    v5 = SWIG_Lua_typename(L, 2);
                    lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "CMonster_ActionBufferProcess", 2, str, v5);
                } else {
                    if (swig_types[2] && swig_types[2]->str)
                        v9 = swig_types[2]->str;
                    else
                        v9 = "void*";
                    v4 = SWIG_Lua_typename(L, 1);
                    lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "CMonster_ActionBufferProcess", 1, v9, v4);
                }
            } else {
                v3 = SWIG_Lua_typename(L, 2);
                lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "ActionBufferProcess", 2, "LPACTION_BUFFER", v3);
            }
        } else {
            v2 = SWIG_Lua_typename(L, 1);
            lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "ActionBufferProcess", 1, "CMonster *", v2);
        }
    } else {
        v1 = lua_gettop(L);
        lua_pushfstring(L, "Error in %s expected %d..%d args, got %d", "ActionBufferProcess", 2, 2, v1);
    }
    lua_error(L);
    return 0;
}

// ============================================================================
// _wrap_CMonster_CalcAbility - IDA @ 0x140696500
// ============================================================================
__int64 __fastcall wrap_CMonster_CalcAbility(lua_State* L)
{
    int v1; // eax
    const char* v2; // rax
    const char* v3; // rax
    CMonster* arg1; // [rsp+38h] [rbp-20h] BYREF
    const char* str; // [rsp+40h] [rbp-18h]

    arg1 = nullptr;
    if (lua_gettop(L) >= 1 && lua_gettop(L) <= 1) {
        if (lua_isuserdata(L, 1) || !lua_type(L, 1)) {
            if (SWIG_Lua_ConvertPtr(L, 1, (void**)&arg1, swig_types[2], 0) >= 0) {
                // TODO: Call actual method - arg1->CalcAbility();
                return 0;
            }
            if (swig_types[2] && swig_types[2]->str)
                str = swig_types[2]->str;
            else
                str = "void*";
            v3 = SWIG_Lua_typename(L, 1);
            lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "CMonster_CalcAbility", 1, str, v3);
        } else {
            v2 = SWIG_Lua_typename(L, 1);
            lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "CalcAbility", 1, "CMonster *", v2);
        }
    } else {
        v1 = lua_gettop(L);
        lua_pushfstring(L, "Error in %s expected %d..%d args, got %d", "CalcAbility", 1, 1, v1);
    }
    lua_error(L);
    return 0;
}

// ============================================================================
// _wrap_CMonster_IsCanHit - IDA @ 0x1406966C0
// ============================================================================
__int64 __fastcall wrap_CMonster_IsCanHit(lua_State* L)
{
    int v1; // eax
    const char* v2; // rax
    const char* v3; // rax
    const char* v4; // rax
    const char* v5; // rax
    int arg3; // [rsp+30h] [rbp-38h]
    int arg2; // [rsp+34h] [rbp-34h]
    CMonster* arg1; // [rsp+40h] [rbp-28h] BYREF
    int result; // [rsp+48h] [rbp-20h]
    const char* str; // [rsp+50h] [rbp-18h]

    arg1 = nullptr;
    if (lua_gettop(L) >= 3 && lua_gettop(L) <= 3) {
        if (lua_isuserdata(L, 1) || !lua_type(L, 1)) {
            if (lua_isnumber(L, 2)) {
                if (lua_isnumber(L, 3)) {
                    if (SWIG_Lua_ConvertPtr(L, 1, (void**)&arg1, swig_types[2], 0) >= 0) {
                        arg2 = (int)lua_tonumber(L, 2);
                        arg3 = (int)lua_tonumber(L, 3);
                        // TODO: Call actual method - result = arg1->IsCanHit(arg2, arg3);
                        result = 0;
                        lua_pushnumber(L, result);
                        return 1;
                    }
                    if (swig_types[2] && swig_types[2]->str)
                        str = swig_types[2]->str;
                    else
                        str = "void*";
                    v5 = SWIG_Lua_typename(L, 1);
                    lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "CMonster_IsCanHit", 1, str, v5);
                } else {
                    v4 = SWIG_Lua_typename(L, 3);
                    lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "IsCanHit", 3, "INT", v4);
                }
            } else {
                v3 = SWIG_Lua_typename(L, 2);
                lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "IsCanHit", 2, "INT", v3);
            }
        } else {
            v2 = SWIG_Lua_typename(L, 1);
            lua_pushfstring(L, "Error in %s (arg %d), expected '%s' got '%s'", "IsCanHit", 1, "CMonster *", v2);
        }
    } else {
        v1 = lua_gettop(L);
        lua_pushfstring(L, "Error in %s expected %d..%d args, got %d", "IsCanHit", 3, 3, v1);
    }
    lua_error(L);
    return 0;
}
