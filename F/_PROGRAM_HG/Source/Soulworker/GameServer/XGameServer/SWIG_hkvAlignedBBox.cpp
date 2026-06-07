// SWIG_hkvAlignedBBox.cpp
// SWIG Lua bindings for hkvAlignedBBox class
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - new_hkvAlignedBBox__SWIG_1 (0x140645d20)
// - new_hkvAlignedBBox__SWIG_3 (0x140645da0)
// - hkvAlignedBBox_set__SWIG_1 (0x140645e60)
// - hkvAlignedBBox_clone (0x140645f00)
// - hkvAlignedBBox___eq (0x140645f40)
// - hkvAlignedBBox_Concat (0x140645f70)
// - hkvAlignedBBox_ToString (0x140646250)

#include <cstdint>
#include <cstring>
#include <cmath>

// Forward declarations
struct lua_State;
struct hkvVec3;
struct hkvAlignedBBox;

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

// hkvVec3 structure (12 bytes: 3 floats)
struct hkvVec3 {
    float x;
    float y;
    float z;

    hkvVec3() : x(0.0f), y(0.0f), z(0.0f) {}
    hkvVec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

// hkvNoInitialization tag
struct hkvNoInitialization_3_t {};
static const hkvNoInitialization_3_t hkvNoInitialization_3 = {};

// hkvAlignedBBox structure (24 bytes: 2 hkvVec3)
struct hkvAlignedBBox {
    hkvVec3 m_vMin;
    hkvVec3 m_vMax;

    // Default constructor
    hkvAlignedBBox() {
        setInvalid();
    }

    // Constructor with no initialization
    hkvAlignedBBox(hkvNoInitialization_3_t) {
        // No initialization
    }

    // Constructor with min/max
    hkvAlignedBBox(const hkvVec3& vMin, const hkvVec3& vMax) {
        setWithoutValidityCheck(vMin, vMax);
    }

    // Set to invalid state
    void setInvalid() {
        m_vMin.x = 1e18f;
        m_vMin.y = 1e18f;
        m_vMin.z = 1e18f;
        m_vMax.x = -1e18f;
        m_vMax.y = -1e18f;
        m_vMax.z = -1e18f;
    }

    // Set without validity check
    void setWithoutValidityCheck(const hkvVec3& vMin, const hkvVec3& vMax) {
        m_vMin = vMin;
        m_vMax = vMax;
    }

    // Set from min/max
    void set(const hkvVec3& vMin, const hkvVec3& vMax) {
        setWithoutValidityCheck(vMin, vMax);
    }

    // Check equality with epsilon
    bool isEqual(const hkvAlignedBBox& other, float epsilon = 0.00001f) const {
        return (std::abs(m_vMin.x - other.m_vMin.x) < epsilon &&
                std::abs(m_vMin.y - other.m_vMin.y) < epsilon &&
                std::abs(m_vMin.z - other.m_vMin.z) < epsilon &&
                std::abs(m_vMax.x - other.m_vMax.x) < epsilon &&
                std::abs(m_vMax.y - other.m_vMax.y) < epsilon &&
                std::abs(m_vMax.z - other.m_vMax.z) < epsilon);
    }

    // Expand to include point
    void expandToInclude(const hkvVec3& v) {
        if (v.x < m_vMin.x) m_vMin.x = v.x;
        if (v.y < m_vMin.y) m_vMin.y = v.y;
        if (v.z < m_vMin.z) m_vMin.z = v.z;
        if (v.x > m_vMax.x) m_vMax.x = v.x;
        if (v.y > m_vMax.y) m_vMax.y = v.y;
        if (v.z > m_vMax.z) m_vMax.z = v.z;
    }

    // Expand to include another bbox
    void expandToInclude(const hkvAlignedBBox& other) {
        expandToInclude(other.m_vMin);
        expandToInclude(other.m_vMax);
    }

    // Get 8 corner vertices
    void getCorners(hkvVec3* out_pVertices) const {
        // Corner 0: min, min, min
        out_pVertices[0].x = m_vMin.x; out_pVertices[0].y = m_vMin.y; out_pVertices[0].z = m_vMin.z;
        // Corner 1: min, min, max
        out_pVertices[1].x = m_vMin.x; out_pVertices[1].y = m_vMin.y; out_pVertices[1].z = m_vMax.z;
        // Corner 2: min, max, min
        out_pVertices[2].x = m_vMin.x; out_pVertices[2].y = m_vMax.y; out_pVertices[2].z = m_vMin.z;
        // Corner 3: min, max, max
        out_pVertices[3].x = m_vMin.x; out_pVertices[3].y = m_vMax.y; out_pVertices[3].z = m_vMax.z;
        // Corner 4: max, min, min
        out_pVertices[4].x = m_vMax.x; out_pVertices[4].y = m_vMin.y; out_pVertices[4].z = m_vMin.z;
        // Corner 5: max, min, max
        out_pVertices[5].x = m_vMax.x; out_pVertices[5].y = m_vMin.y; out_pVertices[5].z = m_vMax.z;
        // Corner 6: max, max, min
        out_pVertices[6].x = m_vMax.x; out_pVertices[6].y = m_vMax.y; out_pVertices[6].z = m_vMin.z;
        // Corner 7: max, max, max
        out_pVertices[7].x = m_vMax.x; out_pVertices[7].y = m_vMax.y; out_pVertices[7].z = m_vMax.z;
    }
};

// ============================================================================
// new_hkvAlignedBBox__SWIG_1 - Create new hkvAlignedBBox
// IDA @ 0x140645d20
// Creates hkvAlignedBBox, optionally initialized to invalid state
// ============================================================================
extern "C" hkvAlignedBBox* __fastcall new_hkvAlignedBBox__SWIG_1(bool bInitialize)
{
    hkvAlignedBBox* pBox = (hkvAlignedBBox*)VBaseObject_operator_new(sizeof(hkvAlignedBBox));

    if (pBox) {
        new (pBox) hkvAlignedBBox(hkvNoInitialization_3);
        if (bInitialize) {
            pBox->setInvalid();
        }
    }

    return pBox;
}

// ============================================================================
// new_hkvAlignedBBox__SWIG_3 - Create new hkvAlignedBBox with min/max
// IDA @ 0x140645da0
// Creates hkvAlignedBBox with specified min and max coordinates
// ============================================================================
extern "C" hkvAlignedBBox* __fastcall new_hkvAlignedBBox__SWIG_3(float x1, float y1, float z1, float x2, float y2, float z2)
{
    hkvAlignedBBox* pBox = (hkvAlignedBBox*)VBaseObject_operator_new(sizeof(hkvAlignedBBox));

    if (pBox) {
        hkvVec3 vMin(x1, y1, z1);
        hkvVec3 vMax(x2, y2, z2);
        new (pBox) hkvAlignedBBox(vMin, vMax);
    }

    return pBox;
}

// ============================================================================
// hkvAlignedBBox_set__SWIG_1 - Set min/max from floats
// IDA @ 0x140645e60
// Sets the bounding box from 6 float values
// ============================================================================
extern "C" void __fastcall hkvAlignedBBox_set__SWIG_1(hkvAlignedBBox* self, float x1, float y1, float z1, float x2, float y2, float z2)
{
    hkvVec3 vMin(x1, y1, z1);
    hkvVec3 vMax(x2, y2, z2);
    self->set(vMin, vMax);
}

// ============================================================================
// hkvAlignedBBox_clone - Clone bounding box
// IDA @ 0x140645f00
// Creates a copy of the bounding box
// ============================================================================
extern "C" hkvAlignedBBox* __fastcall hkvAlignedBBox_clone(hkvAlignedBBox* result, const hkvAlignedBBox* self)
{
    *result = *self;
    return result;
}

// ============================================================================
// hkvAlignedBBox___eq - Equality comparison
// IDA @ 0x140645f40
// Lua __eq metamethod for hkvAlignedBBox
// ============================================================================
extern "C" bool __fastcall hkvAlignedBBox___eq(const hkvAlignedBBox* self, const hkvAlignedBBox* pRhs)
{
    if (!pRhs) {
        return false;
    }

    return pRhs->isEqual(*self, 0.0000099999997f);
}

// ============================================================================
// hkvAlignedBBox_Concat - String concatenation
// IDA @ 0x140645f70
// Lua .. operator for hkvAlignedBBox
// ============================================================================
extern "C" std::int64_t __fastcall hkvAlignedBBox_Concat(lua_State* L)
{
    const char* pszString = nullptr;
    int iIndex = -1;

    // Check if there's a string argument
    if (lua_isstring(L, -1)) {
        pszString = lua_tolstring(L, iIndex--, nullptr);
    }

    // Get self
    if (!lua_isuserdata(L, iIndex) && lua_type(L, iIndex)) {
        luaL_error(L, "Expected %s* as parameter %d", "hkvAlignedBBox", iIndex);
    }

    hkvAlignedBBox* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, iIndex, (void**)&self, swig_types[28], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "hkvAlignedBBox");
    }

    // Get string if at -2
    if (--iIndex == -2) {
        pszString = lua_tolstring(L, -2, nullptr);
    }

    // Format bounding box
    std::size_t uiLen = pszString ? strlen(pszString) + 1 : 0;
    char* pszBuffer = new char[uiLen + 64];
    sprintf(pszBuffer, "[%1.2f,%1.2f,%1.2f][%1.2f,%1.2f,%1.2f]",
            self->m_vMin.x, self->m_vMin.y, self->m_vMin.z,
            self->m_vMax.x, self->m_vMax.y, self->m_vMax.z);

    // Concatenate
    if (iIndex == -3) {
        // String + BBox
        std::size_t bufferLen = strlen(pszBuffer);
        memcpy(pszBuffer + bufferLen, pszString, uiLen);
    } else if (pszString) {
        // BBox + String
        memmove(pszBuffer + uiLen, pszBuffer, strlen(pszBuffer) + 1);
        memcpy(pszBuffer, pszString, uiLen);
    }

    lua_pushstring(L, pszBuffer);
    delete[] pszBuffer;

    return 1;
}

// ============================================================================
// hkvAlignedBBox_ToString - String conversion
// IDA @ 0x140646250
// Lua tostring() for hkvAlignedBBox
// ============================================================================
extern "C" std::int64_t __fastcall hkvAlignedBBox_ToString(lua_State* L)
{
    // Get self
    if (!lua_isuserdata(L, -1) && lua_type(L, -1)) {
        luaL_error(L, "Expected %s* as parameter %d", "hkvAlignedBBox", -1);
    }

    hkvAlignedBBox* self = nullptr;
    if (SWIG_Lua_ConvertPtr(L, -1, (void**)&self, swig_types[28], 0) < 0) {
        luaL_error(L, "Unable to convert self to %s*", "hkvAlignedBBox");
    }

    char pszBuffer[1024];
    sprintf(pszBuffer, "hkvAlignedBBox: Min[%1.2f,%1.2f,%1.2f] - Max[%1.2f,%1.2f,%1.2f]",
            self->m_vMin.x, self->m_vMin.y, self->m_vMin.z,
            self->m_vMax.x, self->m_vMax.y, self->m_vMax.z);
    lua_pushstring(L, pszBuffer);

    return 1;
}
