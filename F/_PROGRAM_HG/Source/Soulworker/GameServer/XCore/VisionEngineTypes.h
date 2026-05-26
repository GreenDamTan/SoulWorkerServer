#pragma once

// Vision Engine 类型 stub 定义
// 这些是 Vision Engine 和 Havok 物理引擎的类型，需要创建兼容层

#include <cstdint>
#include <cstring>
#include <cmath>
#include <string>
#include <memory>
#include <vector>
#include <array>
#include <map>
#include <set>

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
    float x;
    float y;

    tagMOVE_POS() : x(0.0f), y(0.0f) {}

    void Clear() {
        // IDA 0x140276450
        x = 0.0f;
        y = 0.0f;
    }
};

// tagEXTRA_MOVEPOS - 额外移动位置
struct tagEXTRA_MOVEPOS {
    float fMovingTime;
    float fRemainTime;
    float y;
    float x;
    std::uint8_t bySettingType;

    tagEXTRA_MOVEPOS() : fMovingTime(0.0f), fRemainTime(0.0f), y(0.0f), x(0.0f), bySettingType(0) {}

    void Clear() {
        // IDA 0x140189340
        fMovingTime = 0.0f;
        fRemainTime = 0.0f;
        y = 0.0f;
        x = 0.0f;
        bySettingType = 0;
    }
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
        : m_pOwner(nullptr)
        , m_nStartID(0)
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
    void Reset() {
        // IDA 0x1401992D0
        m_nStartID = 0;
        m_nCurID = 0;
        m_fWaitTime = 0.0f;
        m_eDirection = E_WAYDIR_FORWARD;
        m_eState = E_WAYSTAT_NONE;
        m_bChangedID = false;
        m_pCurPointInfo = nullptr;
        m_bInited = false;
        m_nRepeatCount = 0;
        m_bReserveDelete = false;
    }
    void Init(void* pOwner) {
        // IDA 0x140199350
        m_pOwner = pOwner;
    }
    bool GetReserveDelete() const { return m_bReserveDelete; }
    void SetReserveDelete(bool bFlag) { m_bReserveDelete = bFlag; }

private:
    void* m_pOwner;  // CMoverEx*
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

// ============================================================================
// Action Resource Manager 类型
// ============================================================================

// VActionResourceLump - Vision Engine 动画资源块
struct VActionResourceLump {
    void* m_pResourceData;
    std::uint32_t m_uiResourceSize;
    VString m_strResourceName;

    VActionResourceLump() : m_pResourceData(nullptr), m_uiResourceSize(0) {}
};

// VAnimationInfo - Vision Engine 动画信息
struct VAnimationInfo {
    std::uint32_t m_uiAnimKey;
    VString m_strAnimName;
    float m_fDuration;
    void* m_pAnimData;

    VAnimationInfo() : m_uiAnimKey(0), m_fDuration(0.0f), m_pAnimData(nullptr) {}
};

// ============================================================================
// Hit Collision 系统 (从 GameServer.exe IDA 反编译还原)
// ============================================================================

// 前向声明
struct tagCOOLTIME;

// tagHIT_COLLISION - Hit Collision 单项数据
// IDA: 构造函数 0x14000bf40 - 初始化 map 和 vBonePos
// 反编译:
//   tagHIT_COLLISION *__fastcall tagHIT_COLLISION::tagHIT_COLLISION(tagHIT_COLLISION *this)
//   {
//     std::_Tree_unchecked_const_iterator<...>::_Tree_unchecked_const_iterator<...>((...)this);
//     hkvVec3::hkvVec3((XVec3 *)&this->vBonePos);
//     return this;
//   }
// TODO: 从 IDA 还原完整字段布局，包含 std::map<int, tagCOOLTIME> 成员
struct tagHIT_COLLISION {
    // TODO: 需要确认是否继承自 std::map<int, tagCOOLTIME>
    // IDA 反编译显示有 map 迭代器构造
    hkvVec3 vBonePos;  // 骨骼位置

    tagHIT_COLLISION() : vBonePos() {}
};

// tagHIT_TRACE_BONE_NAME_DATA - Hit Trace Bone 名称数据
// IDA: 析构函数 0x14000a080 - 销毁 std::vector<VString>
// 反编译:
//   void __fastcall tagHIT_TRACE_BONE_NAME_DATA::~tagHIT_TRACE_BONE_NAME_DATA(tagHIT_TRACE_BONE_NAME_DATA *this)
//   {
//     std::vector<VString>::~vector<VString>(&this->vTraceBoneName);
//   }
// 注意: 原有定义有误，应该是包含 vector 而非单个项目
struct tagHIT_TRACE_BONE_NAME_DATA {
    std::vector<VString> vTraceBoneName;  // 追踪骨骼名称列表

    tagHIT_TRACE_BONE_NAME_DATA() {}
    ~tagHIT_TRACE_BONE_NAME_DATA() {}  // vector 自动析构
};

// tagHIT_COLLISION_DATA - Hit Collision 数据容器
// IDA: 析构函数 0x14000a060 - 销毁 std::vector<tagHIT_COLLISION>
// 反编译:
//   void __fastcall tagHIT_COLLISION_DATA::~tagHIT_COLLISION_DATA(tagHIT_COLLISION_DATA *this)
//   {
//     std::vector<tagHIT_COLLISION>::~vector<tagHIT_COLLISION>(&this->vHitColisions);
//   }
struct tagHIT_COLLISION_DATA {
    std::vector<tagHIT_COLLISION> vHitColisions;  // Hit Collision 列表

    tagHIT_COLLISION_DATA() {}
    virtual ~tagHIT_COLLISION_DATA() {}  // 虚析构函数
};

// VCommonPositionBoxInfo - 通用位置盒信息
struct VCommonPositionBoxInfo {
    std::int32_t m_nID;
    hkvVec3 m_vMin;
    hkvVec3 m_vMax;
    std::uint8_t m_byType;

    VCommonPositionBoxInfo() : m_nID(0), m_vMin(), m_vMax(), m_byType(0) {}
};

// VManagedResource - Vision Engine 托管资源基类
class VManagedResource {
public:
    VManagedResource() : m_bLoaded(false) {}
    virtual ~VManagedResource() {}

    // 检查资源是否已加载
    static bool IsLoaded(VManagedResource* pResource) {
        return pResource && pResource->m_bLoaded;
    }

protected:
    bool m_bLoaded;
};

// VActionResourceManager - Vision Engine 动作资源管理器基类
class VActionResourceManager {
public:
    VActionResourceManager() {}
    virtual ~VActionResourceManager() {}

    // 虚函数: 加载资源
    virtual VManagedResource* Load(const char* szFilePath) { return nullptr; }
    // 虚函数: 移除所有资源块
    virtual void RemoveAllResourceLump() {}
};
