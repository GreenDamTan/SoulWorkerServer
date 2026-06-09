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

// VType - Vision Engine 类型基类 (前向声明)
class VType;

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
    hkvVec3 operator*(const hkvVec3& other) const { return hkvVec3(x * other.x, y * other.y, z * other.z); }

    float GetLength() const { return std::sqrt(x * x + y * y + z * z); }
    float GetLengthSquared() const { return x * x + y * y + z * z; }

    // IDA: 0x1400169B0 - setZero
    void setZero() { x = 0.0f; y = 0.0f; z = 0.0f; }

    // isZero - 检查向量是否为零向量
    bool isZero(float fEpsilon = 0.0001f) const {
        return (std::abs(x) < fEpsilon && std::abs(y) < fEpsilon && std::abs(z) < fEpsilon);
    }

    // normalizeIfNotZero - 如果不是零向量则归一化
    void normalizeIfNotZero() {
        float len = GetLength();
        if (len > 0.0001f) {
            x /= len; y /= len; z /= len;
        }
    }

    // dot - 点积
    float dot(const hkvVec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // IDA: 0x1403A20C0 - Normalize
    // Normalize - 归一化向量，返回是否成功
    bool Normalize() {
        float len = GetLength();
        if (len < 0.000001f) {
            return false;  // 失败：向量长度太小
        }
        x /= len;
        y /= len;
        z /= len;
        return true;  // 成功
    }
};

// hkvPlane - Havok 平面 (4 floats: normal + distance)
struct hkvPlane {
    float x, y, z, d;  // normal (x,y,z) and distance (d)

    hkvPlane() : x(0.0f), y(0.0f), z(0.0f), d(0.0f) {}
    hkvPlane(float _x, float _y, float _z, float _d) : x(_x), y(_y), z(_z), d(_d) {}
};

// hkvMat3 - Havok 3x3 旋转矩阵
struct hkvMat3 {
    float m[3][3];

    hkvMat3() {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }

    // setFromEulerAngles - 从欧拉角设置旋转矩阵
    void setFromEulerAngles(float fRoll, float fPitch, float fYaw) {
        // 简化实现 - 使用 Yaw 旋转 (绕 Z 轴)
        float cosYaw = std::cos(fYaw);
        float sinYaw = std::sin(fYaw);
        m[0][0] = cosYaw; m[0][1] = -sinYaw; m[0][2] = 0.0f;
        m[1][0] = sinYaw; m[1][1] = cosYaw;  m[1][2] = 0.0f;
        m[2][0] = 0.0f;   m[2][1] = 0.0f;    m[2][2] = 1.0f;
    }

    // transformDirection - 变换方向向量
    hkvVec3 transformDirection(const hkvVec3& v) const {
        return hkvVec3(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
        );
    }

    // operator* - 矩阵与向量乘法
    hkvVec3 operator*(const hkvVec3& v) const {
        return transformDirection(v);
    }
};

// hkvAlignedBBox - Havok Aligned Bounding Box
// IDA: 0x1403788B0 - setInvalid
struct hkvAlignedBBox {
    hkvVec3 m_vMin;
    hkvVec3 m_vMax;

    hkvAlignedBBox() : m_vMin(), m_vMax() {}

    // IDA: 0x1403788B0 - setInvalid
    void setInvalid() {
        // Set min to max float, max to min float (inverted for invalidation)
        m_vMin.x = 3.40282e38f;
        m_vMin.y = 3.40282e38f;
        m_vMin.z = 3.40282e38f;
        m_vMax.x = -3.40282e38f;
        m_vMax.y = -3.40282e38f;
        m_vMax.z = -3.40282e38f;
    }
};

// D3DXVECTOR2 - DirectX 2D 向量 (用于攻击判定)
struct D3DXVECTOR2 {
    float x, y;

    D3DXVECTOR2() : x(0.0f), y(0.0f) {}
    D3DXVECTOR2(float _x, float _y) : x(_x), y(_y) {}
};

// D3DXVec2Dot - 2D 向量点积
inline float D3DXVec2Dot(const D3DXVECTOR2* pV1, const D3DXVECTOR2* pV2) {
    return pV1->x * pV2->x + pV1->y * pV2->y;
}

// tagATTACK_AREA - 攻击区域判定结构 (用于 IsAttackDecision)
// IDA: CMover::IsAttackDecision @ 0x140368D70
// 从反编译推断的结构布局
struct tagATTACK_AREA {
    std::uint8_t byType;              // 攻击类型 (0=扇形, 1=盒子, 2=球体)
    std::uint8_t byHitPartsIndex;     // 输出: 命中部位索引
    std::uint8_t padding_2[2];        // padding
    hkvVec3 vCenterPos;               // 攻击中心位置
    float fRadiusStart;               // 起始半径 (扇形攻击内半径)
    float fRadiusEnd;                 // 结束半径 (扇形攻击外半径)
    float fAngle;                     // 扇形角度 (度)
    D3DXVECTOR2 vAttackerDir;         // 攻击者方向向量
    float fAttackerRot;               // 攻击者旋转角度
    float fSizeX;                     // 盒子攻击 X 尺寸
    float fSizeY;                     // 盒子攻击 Y 尺寸
    float fHeightT;                   // 高度上限
    float fHeightB;                   // 高度下限
    float padding[8];                 // 其他未用字段

    tagATTACK_AREA() {
        std::memset(this, 0, sizeof(tagATTACK_AREA));
    }
};

// 注意: XVec3 在 PSCommon.h 中已定义为 struct，不要重复定义

// ============================================================================
// 游戏逻辑类型
// ============================================================================

// SDefenseChangeInfo - 防御变更信息
// IDA: ??0SDefenseChangeInfo@@QEAA@XZ (0x1403789a0), ?Clear@SDefenseChangeInfo@@QEAAXXZ (0x140378970)
struct SDefenseChangeInfo {
    std::uint32_t dwID;           // IDA: dwID
    std::uint8_t byDefenseType;   // IDA: byDefenseType
    float fChangeTime;            // IDA: fChangeTime

    SDefenseChangeInfo() { Clear(); }

    void Clear() {
        dwID = 0;
        byDefenseType = 0;
        fChangeTime = 0.0f;
    }
};

// SFilterData - 过滤数据
struct SFilterData {
    std::uint32_t dwFilterID;
    std::uint8_t byFilterType;
    float fFilterValue;

    SFilterData() : dwFilterID(0), byFilterType(0), fFilterValue(0.0f) {}
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

    tagACTION_BUFFER(std::uint8_t byVal = 0, float fVal = 0.0f)
        : byCode(byVal), nCurrent(0), nLength(512), fTime(fVal), pActionTrigger(nullptr)
    {
        std::memset(szBuffer, 0, sizeof(szBuffer));
    }

    // IDA: SetFLOAT @ 0x140016C60, SetINT @ 0x140016D10, SetSHORT @ 0x140016D80
    void SetFLOAT(float value) { *(float*)(&szBuffer[nCurrent]) = value; nCurrent += 4; }
    void SetINT(int value) { *(int*)(&szBuffer[nCurrent]) = value; nCurrent += 4; }
    void SetSHORT(std::int16_t value) { *(std::int16_t*)(&szBuffer[nCurrent]) = value; nCurrent += 2; }
};

// CActionBuffer - 动作缓冲区管理类 (大型结构 52912 bytes)
// IDA: Process @ 0x1407353B0, Push @ 0x140735120
class CActionBuffer {
public:
    CActionBuffer() : m_bCurPos(0), m_bActionCnt(0) {}
    ~CActionBuffer() {}

    // IDA: ?Push@CActionBuffer@@QEAAHAEAUtagACTION_BUFFER@@@Z @ 0x140735120
    int Push(tagACTION_BUFFER* xAction) {
        if (m_bActionCnt >= 100) return 0;
        unsigned __int8 bPos = 0;
        int i = m_bCurPos;
        for (; i < m_bActionCnt + m_bCurPos; ++i) {
            bPos = static_cast<unsigned __int8>(i % 100);
            if (m_arAction[bPos].fTime > xAction->fTime) {
                for (int j = m_bActionCnt + m_bCurPos; j > i; --j) {
                    m_arAction[static_cast<unsigned __int8>(j % 100)] =
                        m_arAction[static_cast<unsigned __int8>((j - 1) % 100)];
                }
                break;
            }
        }
        if (i == m_bActionCnt + m_bCurPos) {
            bPos = static_cast<unsigned __int8>((m_bActionCnt + m_bCurPos) % 100);
        }
        m_arAction[bPos] = *xAction;
        ++m_bActionCnt;
        return 1;
    }

    // IDA: ?Process@CActionBuffer@@QEAAXM@Z @ 0x1407353B0
    void Process(float fElapsedTime) {
        for (int i = m_bCurPos; i < m_bActionCnt + m_bCurPos; ++i) {
            m_arAction[static_cast<unsigned __int8>(i % 100)].fTime -= fElapsedTime;
        }
    }

    // Clear - 清空动作缓冲区
    void Clear() {
        m_bCurPos = 0;
        m_bActionCnt = 0;
    }

    std::uint8_t GetActionCount() const { return m_bActionCnt; }

    void DeleteCodeData(std::uint8_t byCode) {
        (void)byCode;
    }

private:
    std::uint8_t m_bCurPos;
    std::uint8_t m_bActionCnt;
    tagACTION_BUFFER m_arAction[100];
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
// tagTIME_SLOW - 时间减慢效果
// IDA: ?Clear@tagTIME_SLOW@@QEAAXXZ (0x1403789c0)
struct tagTIME_SLOW {
    float fTime;       // IDA: fTime
    float fSpeed;      // IDA: fSpeed (default 1.0)

    tagTIME_SLOW() { Clear(); }

    void Clear() {
        fTime = 0.0f;
        fSpeed = 1.0f;
    }
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

    // IDA: ?Find@VPList@@QEBAHPEAX@Z - Find pointer in list, returns index or -1
    int Find(void* ptr) const {
        for (int i = 0; i < nPtrs; ++i) {
            if (ptrs[i] == ptr) return i;
        }
        return -1;
    }

    // IDA: ?Append@VPList@@QEAAXPEAX@Z - Append pointer to list
    void Append(void* ptr) {
        if (nPtrs >= ptrsSize) {
            // Grow array
            int newSize = ptrsSize == 0 ? 8 : ptrsSize * 2;
            void** newPtrs = new void*[newSize];
            if (ptrs) {
                for (int i = 0; i < nPtrs; ++i) newPtrs[i] = ptrs[i];
                delete[] ptrs;
            }
            ptrs = newPtrs;
            ptrsSize = newSize;
        }
        ptrs[nPtrs++] = ptr;
    }

    // IDA: ?AddUnique@VPList@@QEAA_NPEAX@Z (0x1403A1F30) - Add pointer if not already present
    bool AddUnique(void* ptr) {
        if (Find(ptr) >= 0) return false;
        Append(ptr);
        return true;
    }
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
// Forward declarations
class VResourceManager;

// VManagedResource - Vision Engine Managed Resource Base Class
// IDA verified: multiple functions at 0x140018xxx, 0x14072ADxx, 0x140734Dxx, 0x140772xxx
class VManagedResource {
public:
    std::uint16_t m_iResourceFlag;  // IDA: resource flag field
    float m_fLastTimeUsed = 0.0f;   // IDA: last usage timestamp
    char* m_szFilename = nullptr;   // IDA: resource filename

    VManagedResource();
    virtual ~VManagedResource();

    // IDA: 0x140018750 - IsResourceFlagSet
    bool IsResourceFlagSet(int iMask) const;

    // IDA: 0x140018790 - IsLoaded
    bool IsLoaded() const;

    // IDA: 0x14072AD90 - EnsureLoaded
    void EnsureLoaded();

    // IDA: 0x14072AD10 - EnsureUnloaded
    void EnsureUnloaded();

    // IDA: 0x14072AD80 - GetGlobalTime (static)
    static float GetGlobalTime();

    // IDA: 0x140734DB0 - GetFilename
    const char* GetFilename() const;

    // IDA: 0x1407727A0 - SetResourceFlag
    void SetResourceFlag(int iMask);

    // Virtual functions for derived classes
    virtual void DoReload();
    virtual void DoUnload();
    virtual const char* GetDebugName() const { return "VManagedResource"; }

protected:
    static float g_fGlobalTime;  // IDA: global time for resource management
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
// Full implementation in XGameServer/ThreadLocalData.h
// IDA: ?GetTimer@ThreadLocalData@@SAPEAVVDefaultTimer@@XZ (0x1406D1A80)
// Forward declaration only - actual class is in ThreadLocalData.h
class ThreadLocalData;
