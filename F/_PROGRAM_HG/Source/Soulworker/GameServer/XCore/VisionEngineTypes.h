#pragma once

// Vision Engine 类型 stub 定义
// 这些是 Vision Engine 和 Havok 物理引擎的类型，需要创建兼容层

#include <cstdint>
#include <cstring>
#include <cmath>
#include <string>
#include <memory>

// 使用 std::tr1 命名空间 (VS2010 兼容) - C++11 中 tr1 已合并到 std
namespace std { namespace tr1 = std; }

// ============================================================================
// Vision Engine 基础类型
// ============================================================================

// VString - Vision Engine 字符串类
struct VString {
    char* m_pBuffer;
    std::uint32_t m_uiLength;
    std::uint32_t m_uiCapacity;

    VString() : m_pBuffer(nullptr), m_uiLength(0), m_uiCapacity(0) {}
    ~VString() { if (m_pBuffer) delete[] m_pBuffer; }

    VString(const char* str) : m_pBuffer(nullptr), m_uiLength(0), m_uiCapacity(0) {
        if (str) {
            m_uiLength = static_cast<std::uint32_t>(std::strlen(str));
            m_uiCapacity = m_uiLength + 1;
            m_pBuffer = new char[m_uiCapacity];
            std::memcpy(m_pBuffer, str, m_uiCapacity);
        }
    }

    VString(const VString& other) : m_pBuffer(nullptr), m_uiLength(0), m_uiCapacity(0) {
        if (other.m_pBuffer && other.m_uiLength > 0) {
            m_uiLength = other.m_uiLength;
            m_uiCapacity = other.m_uiCapacity;
            m_pBuffer = new char[m_uiCapacity];
            std::memcpy(m_pBuffer, other.m_pBuffer, m_uiCapacity);
        }
    }

    VString& operator=(const VString& other) {
        if (this != &other) {
            if (m_pBuffer) delete[] m_pBuffer;
            m_uiLength = other.m_uiLength;
            m_uiCapacity = other.m_uiCapacity;
            if (other.m_pBuffer && m_uiCapacity > 0) {
                m_pBuffer = new char[m_uiCapacity];
                std::memcpy(m_pBuffer, other.m_pBuffer, m_uiCapacity);
            } else {
                m_pBuffer = nullptr;
            }
        }
        return *this;
    }

    const char* AsChar() const { return m_pBuffer ? m_pBuffer : ""; }
    std::uint32_t GetLength() const { return m_uiLength; }
};

// hkvVec3 - Havok 3D 向量
struct hkvVec3 {
    float x, y, z;

    hkvVec3() : x(0.0f), y(0.0f), z(0.0f) {}
    hkvVec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    hkvVec3 operator+(const hkvVec3& other) const { return hkvVec3(x + other.x, y + other.y, z + other.z); }
    hkvVec3 operator-(const hkvVec3& other) const { return hkvVec3(x - other.x, y - other.y, z - other.z); }
    hkvVec3 operator*(float f) const { return hkvVec3(x * f, y * f, z * f); }

    float GetLength() const { return std::sqrt(x * x + y * y + z * z); }
    float GetLengthSquared() const { return x * x + y * y + z * z; }
};

// 注意: XVec3 在 PSCommon.h 中已定义为 struct，不要重复定义

// ============================================================================
// 游戏逻辑类型
// ============================================================================

// SDefenseChangeInfo - 防御变更信息
struct SDefenseChangeInfo {
    std::uint32_t dwSourceID;
    std::uint8_t byDefenseType;
    float fDefenseRate;
    float fDuration;
    std::uint8_t byActive;

    SDefenseChangeInfo() : dwSourceID(0), byDefenseType(0), fDefenseRate(0.0f), fDuration(0.0f), byActive(0) {}
};

// SFilterData - 过滤数据
struct SFilterData {
    std::uint32_t dwFilterID;
    std::uint8_t byFilterType;
    float fFilterValue;

    SFilterData() : dwFilterID(0), byFilterType(0), fFilterValue(0.0f) {}
};

// CActionBuffer - 动作缓冲区 (大型结构 52912 bytes)
class CActionBuffer {
public:
    CActionBuffer() {}
    ~CActionBuffer() {}

private:
    // Stub - 实际大小 52912 bytes，需要从 IDA 还原完整布局
    std::uint8_t m_dummy[52912];
};

// tagBUFF_STATE - Buff 状态
struct tagBUFF_STATE {
    std::uint32_t dwBuffID;
    std::uint32_t dwSourceID;
    float fRemainTime;
    std::uint8_t byBuffType;
    std::uint8_t byActive;

    tagBUFF_STATE() : dwBuffID(0), dwSourceID(0), fRemainTime(0.0f), byBuffType(0), byActive(0) {}
};

// tagMOVE_POS - 移动位置
struct tagMOVE_POS {
    hkvVec3 vPos;
    float fYaw;
    std::uint8_t byMoveType;

    tagMOVE_POS() : vPos(), fYaw(0.0f), byMoveType(0) {}
};

// tagEXTRA_MOVEPOS - 额外移动位置
struct tagEXTRA_MOVEPOS {
    hkvVec3 vPos;
    hkvVec3 vVelocity;
    float fTime;

    tagEXTRA_MOVEPOS() : vPos(), vVelocity(), fTime(0.0f) {}
};

// tagTIME_SLOW - 时间减速
struct tagTIME_SLOW {
    float fSlowRate;
    float fDuration;
    std::uint32_t dwSourceID;

    tagTIME_SLOW() : fSlowRate(1.0f), fDuration(0.0f), dwSourceID(0) {}
};

// SDelayedProjectile - 延迟投射物
struct SDelayedProjectile {
    std::uint32_t dwProjectileID;
    hkvVec3 vStartPos;
    hkvVec3 vTargetPos;
    float fDelayTime;
    std::uint8_t byActive;

    SDelayedProjectile() : dwProjectileID(0), vStartPos(), vTargetPos(), fDelayTime(0.0f), byActive(0) {}
};

// ============================================================================
// Vision Engine 基础对象类型
// ============================================================================

// VTypedObject - Vision Engine 类型对象基类 (32 bytes)
struct VTypedObject {
    void* __vftable;            // vtable pointer
    int m_eObjectFlags;         // object flags
    void* m_pRegisteredAtType;  // VType*
    void* m_pDeserializationArchive;  // VArchive*

    VTypedObject() : __vftable(nullptr), m_eObjectFlags(0), m_pRegisteredAtType(nullptr), m_pDeserializationArchive(nullptr) {}
    virtual ~VTypedObject() {}
};

// VPList - Vision Engine 指针列表 (48 bytes)
class VPList : public VTypedObject {
public:
    void** ptrs;      // pointer array
    int nPtrs;        // current count
    int ptrsSize;     // allocated size

    VPList() : VTypedObject(), ptrs(nullptr), nPtrs(0), ptrsSize(0) {}
    virtual ~VPList() { if (ptrs) delete[] ptrs; }

    int GetLength() const { return nPtrs; }
    void Reset() { if (ptrs) { delete[] ptrs; ptrs = nullptr; } nPtrs = 0; ptrsSize = 0; }
    void*& operator[](int idx) { return ptrs[idx]; }
    const void*& operator[](int idx) const { return const_cast<const void*&>(ptrs[idx]); }
};

// ============================================================================
// Waypoint 系统
// ============================================================================

// E_WAYDIR - Waypoint 方向枚举
enum E_WAYDIR {
    E_WAYDIR_FORWARD = 0,
    E_WAYDIR_BACKWARD = 1,
    E_WAYDIR_RANDOM = 2
};

// E_WAYSTAT - Waypoint 状态枚举
enum E_WAYSTAT {
    E_WAYSTAT_NONE = 0,
    E_WAYSTAT_WAITING = 1,
    E_WAYSTAT_MOVING = 2
};

// VWayPointInfo - 前置声明
struct VWayPointInfo;

// CWayPoint - Waypoint 类 (从 IDA 构造函数还原)
class CWayPoint {
public:
    CWayPoint()
        : m_nStartID(0)
        , m_nCurID(0)
        , m_fWaitTime(0.0f)
        , m_eDirection(E_WAYDIR_FORWARD)
        , m_eState(E_WAYSTAT_NONE)
        , m_bChangedID(false)
        , m_pCurPointInfo(nullptr)
        , m_bInited(false)
        , m_nRepeatCount(0)
        , m_bReserveDelete(false)
    {}

    int GetCurID() const { return m_nCurID; }
    void SetCurID(int nID, bool bFlag = false) { m_nCurID = nID; m_bChangedID = true; }
    void Reset() { m_nCurID = m_nStartID; m_fWaitTime = 0.0f; m_eState = E_WAYSTAT_NONE; }
    bool GetReserveDelete() const { return m_bReserveDelete; }
    void SetReserveDelete(bool bFlag) { m_bReserveDelete = bFlag; }

private:
    int m_nStartID;
    int m_nCurID;
    float m_fWaitTime;
    E_WAYDIR m_eDirection;
    E_WAYSTAT m_eState;
    bool m_bChangedID;
    VWayPointInfo* m_pCurPointInfo;
    bool m_bInited;
    int m_nRepeatCount;
    bool m_bReserveDelete;
};

// ============================================================================
// 技能系统
// ============================================================================

// CMySkillList - 前置声明 (完整定义在 MySkillList.h)
class CMySkillList;

// ============================================================================
// 用户相关结构
// ============================================================================

// PS_TICKCOUNT_INFO - Tick 计数信息
struct PS_TICKCOUNT_INFO {
    std::uint32_t dwTickCount;
    std::uint32_t dwLastTick;
    float fInterval;

    PS_TICKCOUNT_INFO() : dwTickCount(0), dwLastTick(0), fInterval(0.0f) {}
};

// ST_CHECK_POS - 位置检查结构
struct ST_CHECK_POS {
    hkvVec3 vPos;
    float fYaw;
    std::uint32_t dwCheckTime;
    std::uint8_t byCheckType;

    ST_CHECK_POS() : vPos(), fYaw(0.0f), dwCheckTime(0), byCheckType(0) {}
};

// ============================================================================
// 用户相关结构
// 注意: ST_CHAR_COMMUNITY, ST_LEAGUE_INFO_FOR_GAME, STPosInfo, CFSRWLock
// 已在其他头文件 (PSServerFriend.h, PSServerLeague.h, PSCommon.h, CFSRWLock.h) 中定义
// 这里只放前向声明
// ============================================================================

struct ST_CHAR_COMMUNITY;
struct STMyCharInfoEx;
