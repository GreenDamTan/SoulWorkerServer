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

    // 用于 std::map 的比较运算符
    bool operator<(const VString& other) const {
        const char* a = m_pBuffer ? m_pBuffer : "";
        const char* b = other.m_pBuffer ? other.m_pBuffer : "";
        return std::strcmp(a, b) < 0;
    }

    bool operator==(const VString& other) const {
        const char* a = m_pBuffer ? m_pBuffer : "";
        const char* b = other.m_pBuffer ? other.m_pBuffer : "";
        return std::strcmp(a, b) == 0;
    }

    bool operator!=(const VString& other) const {
        return !(*this == other);
    }
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

    // IDA: 0x1400169B0 - setZero
    void setZero() { x = 0.0f; y = 0.0f; z = 0.0f; }
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

// tagACTION_BUFFER - 动作缓冲区结构 (529 bytes)
// IDA: 从 get_struct_info 获取完整布局
struct tagACTION_BUFFER {
    std::uint8_t byCode;           // offset 0
    std::int16_t nCurrent;          // offset 1 - 当前位置
    std::int16_t nLength;           // offset 3 - 长度
    float fTime;                    // offset 5 - 时间
    char szBuffer[512];             // offset 9 - 缓冲区
    void* pActionTrigger;           // offset 521 - 动作触发器指针

    tagACTION_BUFFER() : byCode(0), nCurrent(0), nLength(0), fTime(0.0f), pActionTrigger(nullptr) {
        std::memset(szBuffer, 0, sizeof(szBuffer));
    }

    // IDA: 0x140016CD0 - Setbool
    void Setbool(bool& in_value) {
        szBuffer[nCurrent++] = in_value ? 1 : 0;
    }
};

// tagBUFF_STATE 定义移至 Soulworker/GameServer/XGameServer/BuffState.h
// 该头文件包含完整的 IDA 还原定义

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

    bool IsZero() const {
        // IDA 0x1402762D0
        return x == 0.0f && y == 0.0f;
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

    bool IsZero() const {
        // 检查是否为零值
        return fMovingTime == 0.0f && fRemainTime == 0.0f && y == 0.0f && x == 0.0f && bySettingType == 0;
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
// IDA: 0x140026B90 - Init function
struct PS_TICKCOUNT_INFO {
    int nTicknum;
    std::uint8_t byType;
    std::uint8_t padding_5;
    std::uint8_t padding_6;
    std::uint8_t padding_7;
    std::uint32_t dwReqTickcount;
    std::uint32_t dwResTickcount;
    std::uint32_t dwGetTickcount;
    std::uint64_t dw64ReqTickcount;
    std::uint64_t dw64ResTickcount;
    std::uint64_t dw64GetTickcount;
    int nFps;

    PS_TICKCOUNT_INFO() { Init(); }
    void Init() {
        nTicknum = 0;
        byType = 0;
        dwReqTickcount = 0;
        dwResTickcount = 0;
        dwGetTickcount = 0;
        dw64ReqTickcount = 0;
        dw64ResTickcount = 0;
        dw64GetTickcount = 0;
        nFps = 0;
    }
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

// 前置声明
class ActionTrigger;

// VArray - Vision Engine 动态数组模板
template<typename T>
class VArray {
public:
    T* m_pElements;
    int m_nCount;
    int m_nCapacity;

    VArray() : m_pElements(nullptr), m_nCount(0), m_nCapacity(0) {}
    ~VArray() { if (m_pElements) delete[] m_pElements; }

    int GetLength() const { return m_nCount; }
    T& operator[](int idx) { return m_pElements[idx]; }
    const T& operator[](int idx) const { return m_pElements[idx]; }
    T* ElementAt(int idx) { return &m_pElements[idx]; }
    const T* ElementAt(int idx) const { return &m_pElements[idx]; }
};

// VRefCounter - Vision Engine 引用计数基类 (16 bytes)
class VRefCounter {
public:
    void* __vftable;
    int m_iRefCount;

    VRefCounter() : __vftable(nullptr), m_iRefCount(1) {}
    virtual ~VRefCounter() {}
};

// TypeOfActionBufferBehavior - 动作缓冲行为类型枚举
enum TypeOfActionBufferBehavior {
    ACTION_BUFFER_BEHAVIOR_NONE = 0,
    ACTION_BUFFER_BEHAVIOR_RESETBEFORE = 1,
    ACTION_BUFFER_BEHAVIOR_RESETAFTER = 2,
    ACTION_BUFFER_BEHAVIOR_RESETBOTH = 3
};

// TypeOfMoving - 移动类型枚举
enum TypeOfMoving {
    MOVING_NONE = 0,
    MOVING_NORMAL = 1,
    MOVING_SPECIAL = 2
};

// TypeOfDefense - 防御类型枚举
enum TypeOfDefense {
    DEFENSE_NONE = 0,
    DEFENSE_NORMAL = 1,
    DEFENSE_SPECIAL = 2
};

// TypeOfHUD - HUD类型枚举
enum TypeOfHUD {
    HUD_NONE = 0,
    HUD_NORMAL = 1,
    HUD_SPECIAL = 2
};

// TypeOfAnimationBehavior - 动画行为类型枚举
enum TypeOfAnimationBehavior {
    ANIM_BEHAVIOR_NONE = 0,
    ANIM_BEHAVIOR_LOOP = 1,
    ANIM_BEHAVIOR_ONCE = 2
};

// EndOfAnimationType - 动画结束类型枚举
enum EndOfAnimationType {
    END_OF_ANIM_NONE = 0,
    END_OF_ANIM_IDLE = 1,
    END_OF_ANIM_STAND = 2
};

// ActionTrigger - 动作触发器结构 (168 bytes)
// IDA: 从 get_struct_info 获取完整布局
class ActionTrigger : public VRefCounter {
public:
    std::uint8_t TypeOfTrigger;     // 触发器类型 (offset 16)
    std::uint8_t padding_17;        // padding (offset 17)
    std::int16_t EventID;           // 事件ID (offset 18)
    char TriggerName[128];          // 触发器名称 (offset 20)
    float StartTime;                // 开始时间 (offset 148)
    float EndTime;                  // 结束时间 (offset 152)
    std::int32_t dwFilterInfo1;     // 过滤信息1 (offset 156)
    std::int32_t dwFilterInfo2;     // 过滤信息2 (offset 160)
    std::int32_t dwFilterInfo3;     // 过滤信息3 (offset 164)

    ActionTrigger()
        : VRefCounter()
        , TypeOfTrigger(0)
        , padding_17(0)
        , EventID(0)
        , StartTime(0.0f)
        , EndTime(0.0f)
        , dwFilterInfo1(0)
        , dwFilterInfo2(0)
        , dwFilterInfo3(0)
    {
        std::memset(TriggerName, 0, sizeof(TriggerName));
    }

    virtual ~ActionTrigger() {}

    // IDA: 0x140016F20 - IsFiltering static function
    static bool IsFiltering(int nCurrData1, int nCurrData2, int nCurrData3,
                            int nTriggerData1, int nTriggerData2, int nTriggerData3) {
        if (!nTriggerData1 && !nTriggerData2 && !nTriggerData3)
            return false;
        if (!nCurrData1 && !nCurrData2 && !nCurrData3)
            return false;

        // Check WORD filter (low 16 bits)
        if ((nTriggerData1 & 0xFFFF) != 0 &&
            (nTriggerData1 & 0xFFFF) != 0xFFFF &&
            ((nCurrData1 & 0xFFFF) & (nTriggerData1 & 0xFFFF)) == 0) {
            return true;
        }

        // Check high byte filter (bits 24-31)
        unsigned int nMask = nTriggerData1 & 0xFF000000;
        unsigned int nMask2 = nCurrData1 & 0xFF000000;
        if (nMask != 0 && nMask2 != 0 && nMask != 0xFF000000 && (nMask2 & nMask) == 0)
            return true;

        // Check second byte filter (bits 16-23)
        int nMaska = nTriggerData1 & 0xFF0000;
        int nMask2a = nCurrData1 & 0xFF0000;
        if (nMaska != 0 && nMask2a != 0 && nMaska != 0xFF0000 && (nMask2a & nMaska) == 0)
            return true;

        // Check Data2 filters
        if (nTriggerData2 != 0 && (nCurrData2 & 0xFFFF) != 0xFFFF) {
            if ((nTriggerData2 & 0x40000000) != 0 && nCurrData2 != 0)
                return true;
            if ((nTriggerData2 & 0xFFFF) != 0 && ((nCurrData2 & 0xFFFF) & (nTriggerData2 & 0xFFFF)) == 0)
                return true;
            if ((nTriggerData2 & 0x3FFF0000) != 0 && (nCurrData2 & nTriggerData2 & 0x3FFF0000) == 0)
                return true;
        }

        // Check Data3 filters
        if (nTriggerData3 == 0)
            return false;

        if ((nTriggerData3 & 0xFF) != 0 && (nTriggerData3 & 0xFF) != 255)
            return ((nCurrData3 & nTriggerData3) & 0xFF) == 0;

        int nMaskb = nTriggerData3 & 0xFF00;
        return (nMaskb != 0 && nMaskb != 0xFF00 && (nCurrData3 & 0xFF00 & nMaskb) == 0);
    }
};

// VAnimationInfo - Vision Engine 动画信息 (312 bytes)
// IDA: 从 get_struct_info 获取完整布局
struct VAnimationInfo {
    char szName[128];                                   // 动画名称 (offset 0)
    TypeOfMoving eCanMoving;                            // 移动类型 (offset 128)
    std::int16_t iAnimGroup;                            // 动画组 (offset 132)
    std::int16_t padding_134;                           // padding (offset 134)
    float fUpperRotRate;                                // 上身旋转速率 (offset 136)
    TypeOfDefense eDefenseType;                         // 防御类型 (offset 140)
    TypeOfHUD eHUDType;                                 // HUD类型 (offset 144)
    TypeOfAnimationBehavior eAnimationBehavior;         // 动画行为 (offset 148)
    TypeOfActionBufferBehavior eActionBufferBehavior;   // 动作缓冲行为 (offset 152)
    EndOfAnimationType eEndofAnimation;                 // 动画结束类型 (offset 156)
    std::int16_t iAnimBlendingType;                     // 动画混合类型 (offset 160)
    std::int16_t padding_162;                           // padding (offset 162)
    std::uint8_t padding_164[4];                        // padding (offset 164)
    VArray<ActionTrigger*> arTriggers;                  // 触发器数组 (offset 168, 24 bytes)
    std::uint8_t arTriggerTypeCounter[42];              // 触发器类型计数器 (offset 192)
    std::uint8_t padding_234[2];                        // padding (offset 234)
    float fAnimationLength;                             // 动画长度 (offset 236)
    VArray<void*> arOffsetDeltaFrames;                  // 偏移增量帧 (offset 240, 24 bytes)
    VArray<void*> arTranslationFrames;                  // 平移帧 (offset 264, 24 bytes)
    VArray<void*> arRotationFrames;                     // 旋转帧 (offset 288, 24 bytes)

    VAnimationInfo()
        : eCanMoving(MOVING_NONE)
        , iAnimGroup(0)
        , padding_134(0)
        , fUpperRotRate(0.0f)
        , eDefenseType(DEFENSE_NONE)
        , eHUDType(HUD_NONE)
        , eAnimationBehavior(ANIM_BEHAVIOR_NONE)
        , eActionBufferBehavior(ACTION_BUFFER_BEHAVIOR_NONE)
        , eEndofAnimation(END_OF_ANIM_NONE)
        , iAnimBlendingType(0)
        , padding_162(0)
        , fAnimationLength(0.0f)
    {
        std::memset(szName, 0, sizeof(szName));
        std::memset(padding_164, 0, sizeof(padding_164));
        std::memset(arTriggerTypeCounter, 0, sizeof(arTriggerTypeCounter));
        std::memset(padding_234, 0, sizeof(padding_234));
    }
};

// VBaseResourceLump - Vision Engine 基础资源块 (104 bytes)
struct VBaseResourceLump {
    void* __vftable;
    void* m_pResourceData;
    std::uint32_t m_uiResourceSize;
    char m_szResourceName[260];
    std::uint32_t m_uiFlags;

    VBaseResourceLump()
        : __vftable(nullptr)
        , m_pResourceData(nullptr)
        , m_uiResourceSize(0)
        , m_uiFlags(0)
    {
        std::memset(m_szResourceName, 0, sizeof(m_szResourceName));
    }
};

// VActionResourceLump - Vision Engine 动画资源块 (232 bytes)
// IDA: 从 get_struct_info 获取完整布局
struct VActionResourceLump {
    VBaseResourceLump base;                             // 基类 (offset 0, 104 bytes)
    std::vector<VAnimationInfo> m_arAnimationContainer; // 动画容器 (offset 104, 32 bytes)
    std::vector<float> m_arHitBoneRadius;               // 命中骨骼半径 (offset 136, 32 bytes)
    std::vector<void*> m_arJumpInfos;                   // 跳跃信息 (offset 168, 32 bytes)
    std::map<int, ActionTrigger*> m_mapAttackTrigger;   // 攻击触发器映射 (offset 200, 32 bytes)

    VActionResourceLump() {}

    // 获取动画列表
    VAnimationInfo* GetActionList() {
        return m_arAnimationContainer.empty() ? nullptr : &m_arAnimationContainer[0];
    }
    const VAnimationInfo* GetActionList() const {
        return m_arAnimationContainer.empty() ? nullptr : &m_arAnimationContainer[0];
    }

    // 获取动画数量
    std::size_t GetActionLength() const {
        return m_arAnimationContainer.size();
    }

    // 查找动画信息
    const VAnimationInfo* FindAnimationInfo(const char* pszName) const {
        if (!pszName) return nullptr;
        for (std::size_t i = 0; i < m_arAnimationContainer.size(); ++i) {
            if (std::strcmp(m_arAnimationContainer[i].szName, pszName) == 0) {
                return &m_arAnimationContainer[i];
            }
        }
        return nullptr;
    }
    VAnimationInfo* FindAnimationInfo(const char* pszName) {
        if (!pszName) return nullptr;
        for (std::size_t i = 0; i < m_arAnimationContainer.size(); ++i) {
            if (std::strcmp(m_arAnimationContainer[i].szName, pszName) == 0) {
                return &m_arAnimationContainer[i];
            }
        }
        return nullptr;
    }
};

// ============================================================================
// Hit Collision 系统 (从 GameServer.exe IDA 反编译还原)
// ============================================================================

// tagCOOLTIME - 冷却时间结构 (size: 16 bytes)
// IDA: get_struct_info 完整布局
struct tagCOOLTIME {
    std::uint32_t dwTotalTime;   // 总时间 (offset 0)
    float fStartTime;            // 开始时间 (offset 4)
    float fEndTime;              // 结束时间 (offset 8)
    std::uint8_t byType;         // 类型 (offset 12)

    tagCOOLTIME() : dwTotalTime(0), fStartTime(0.0f), fEndTime(0.0f), byType(0) {}
};

// tagHIT_COLLISION - Hit Collision 单项数据 (size: 29 bytes)
// IDA: 默认构造函数 0x14000bf40, 拷贝构造函数 0x140014f20
// 反编译 (拷贝构造):
//   tagHIT_COLLISION *__fastcall tagHIT_COLLISION::tagHIT_COLLISION(tagHIT_COLLISION *this, const tagHIT_COLLISION *__that)
//   {
//     VString::VString(&this->strBoneName, &__that->strBoneName);
//     this->iBoneIndex = __that->iBoneIndex;
//     this->fRadius = __that->fRadius;
//     this->byHitParts = __that->byHitParts;
//     qmemcpy(&this->vBonePos, &__that->vBonePos, sizeof(this->vBonePos));
//     return this;
//   }
struct tagHIT_COLLISION {
    VString strBoneName;         // 骨骼名称 (offset 0, size 8)
    int iBoneIndex;              // 骨骼索引 (offset 8, size 4)
    float fRadius;               // 碰撞半径 (offset 12, size 4)
    std::uint8_t byHitParts;     // 受击部位 (offset 16, size 1)
    hkvVec3 vBonePos;            // 骨骼位置 (offset 17, size 12)

    // 默认构造函数 (IDA 0x14000bf40)
    tagHIT_COLLISION() : strBoneName(), iBoneIndex(-1), fRadius(0.0f), byHitParts(0), vBonePos() {}

    // 拷贝构造函数 (IDA 0x140014f20)
    tagHIT_COLLISION(const tagHIT_COLLISION& other)
        : strBoneName(other.strBoneName)
        , iBoneIndex(other.iBoneIndex)
        , fRadius(other.fRadius)
        , byHitParts(other.byHitParts)
        , vBonePos(other.vBonePos) {}

    // 拷贝赋值运算符
    tagHIT_COLLISION& operator=(const tagHIT_COLLISION& other) {
        if (this != &other) {
            strBoneName = other.strBoneName;
            iBoneIndex = other.iBoneIndex;
            fRadius = other.fRadius;
            byHitParts = other.byHitParts;
            vBonePos = other.vBonePos;
        }
        return *this;
    }
};

// tagHIT_TRACE_BONE_NAME_DATA - Hit Trace Bone 名称数据 (size: 32 bytes)
// IDA: 析构函数 0x14000a080, get_struct_info 完整布局
// 反编译:
//   void __fastcall tagHIT_TRACE_BONE_NAME_DATA::~tagHIT_TRACE_BONE_NAME_DATA(tagHIT_TRACE_BONE_NAME_DATA *this)
//   {
//     std::vector<VString>::~vector<VString>(&this->vTraceBoneName);
//   }
struct tagHIT_TRACE_BONE_NAME_DATA {
    std::vector<VString> vTraceBoneName;  // 追踪骨骼名称列表 (offset 0, size 32)

    tagHIT_TRACE_BONE_NAME_DATA() {}
    ~tagHIT_TRACE_BONE_NAME_DATA() {}  // vector 自动析构
};

// tagHIT_COLLISION_DATA - Hit Collision 数据容器 (size: 40 bytes)
// IDA: 析构函数 0x14000a060, get_struct_info 完整布局
// 反编译:
//   void __fastcall tagHIT_COLLISION_DATA::~tagHIT_COLLISION_DATA(tagHIT_COLLISION_DATA *this)
//   {
//     std::vector<tagHIT_COLLISION>::~vector<tagHIT_COLLISION>(&this->vHitColisions);
//   }
struct tagHIT_COLLISION_DATA {
    float fCylinderRadius;                       // 圆柱碰撞体半径 (offset 0)
    float fCylinderHeight;                       // 圆柱碰撞体高度 (offset 4)
    std::vector<tagHIT_COLLISION> vHitColisions; // Hit Collision 列表 (offset 8, size 32)

    tagHIT_COLLISION_DATA() : fCylinderRadius(0.0f), fCylinderHeight(0.0f) {}
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
    std::uint16_t m_iResourceFlag;  // IDA: resource flag field

    VManagedResource() : m_iResourceFlag(0) {}
    virtual ~VManagedResource() {}

    // IDA: 0x140018750 - IsResourceFlagSet
    bool IsResourceFlagSet(int iMask) const {
        return (static_cast<std::uint16_t>(iMask) & m_iResourceFlag) == iMask;
    }

    // IDA: 0x140018790 - IsLoaded
    bool IsLoaded() const {
        return IsResourceFlagSet(1);
    }
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

// ============================================================================
// Timer System Types (IDA verified)
// ============================================================================

// IVTimer - Vision Engine Timer Interface
// IDA: ?GetTime@IVTimer@@QEBAMXZ (0x140276890)
class IVTimer {
public:
    float m_fTime;  // IDA confirmed: returns this->m_fTime

    IVTimer() : m_fTime(0.0f) {}
    virtual ~IVTimer() {}

    // IDA 0x140276890: return this->m_fTime
    float GetTime() const { return m_fTime; }
    
    // IDA 0x140049010: return m_fTimeDifference
    float GetTimeDifference() const { return m_fTimeDifference; }
    
    // IDA 0x1406D08D0: m_fTimeDifference = diff
    void SetTimeDifference(float diff) { m_fTimeDifference = diff; }

protected:
    float m_fTimeDifference = 0.0f;
};

// VDefaultTimer - Default Timer Implementation
// Inherits from IVTimer
class VDefaultTimer : public IVTimer {
public:
    VDefaultTimer(bool bInit = true) : IVTimer() {
        if (bInit) {
            Init();
        }
    }
    
    virtual ~VDefaultTimer() {}
    
    virtual void Init() {
        m_fTime = 0.0f;
        m_fTimeDifference = 0.0f;
    }
    
    virtual void Update() {
        // Timer update logic would go here
        // This is a stub - actual implementation would update m_fTime
    }
    
    virtual void DeleteThis() {
        delete this;
    }
};

// ThreadLocalData - Thread-local storage for game data
// IDA: ?GetTimer@ThreadLocalData@@SAPEAVVDefaultTimer@@XZ (0x1406D1A80)
// Returns TLS slot 1 pointer as VDefaultTimer*
class ThreadLocalData {
public:
    // IDA 0x1406D1A80: returns *(VDefaultTimer**)(TLS[1])
    static VDefaultTimer* GetTimer() {
        // Windows TLS: TEB->ThreadLocalStoragePointer + slot*8
        // IDA shows it reads TLS slot 1 (offset 8 from TLS pointer)
        // This is a stub - actual implementation needs platform-specific TLS access
        static VDefaultTimer s_DefaultTimer;
        return &s_DefaultTimer;
    }
    
    // IDA 0x1406D1A60: returns TLS instance pointer
    static ThreadLocalData* GetInstance() {
        static ThreadLocalData s_Instance;
        return &s_Instance;
    }
};
