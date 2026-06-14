#pragma once

// Vision Engine 类型 stub 定义
// 这些是 Vision Engine 和 Havok 物理引擎的类型，需要创建兼容层

#include <cstdint>
#include <cstring>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cctype>
#include <string>
#include <memory>
#include <vector>
#include <array>
#include <map>
#include <set>

// 使用 std::tr1 命名空间 (VS2010 兼容) - C++11 中 tr1 已合并到 std
namespace std { namespace tr1 = std; }

// 包含 Vision Engine 子类型定义
#include "VisionEngineTypes/hkvVec3.h"
#include "VisionEngineTypes/hkvMat3.h"
#include "VisionEngineTypes/VisTypedEngineObject.h"
#include "VisionEngineTypes/VisObject3D.h"
#include "VisionEngineTypes/VisBaseEntity.h"

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
    int GetLen() const { return static_cast<int>(m_uiLength); }  // IDA compatibility alias

    // Static version for IDA compatibility
    static const char* AsChar(const VString* pStr) {
        return pStr ? (pStr->m_pBuffer ? pStr->m_pBuffer : "") : "";
    }

    // ReplaceAt - replace character at position (IDA compatibility)
    void ReplaceAt(int nPos, int nCount, const char* szReplace) {
        if (!szReplace || nPos < 0 || nPos >= static_cast<int>(m_uiLength)) return;
        // Simple implementation: just replace single character
        if (nCount == 1 && szReplace[0]) {
            m_pBuffer[nPos] = szReplace[0];
        }
    }

    // operator+= for string concatenation
    VString& operator+=(const char* str) {
        if (str && str[0]) {
            size_t addLen = std::strlen(str);
            size_t newLen = m_uiLength + addLen;
            char* newBuffer = new char[newLen + 1];
            if (m_pBuffer) {
                std::memcpy(newBuffer, m_pBuffer, m_uiLength);
            }
            std::memcpy(newBuffer + m_uiLength, str, addLen + 1);
            if (m_pBuffer) delete[] m_pBuffer;
            m_pBuffer = newBuffer;
            m_uiLength = static_cast<std::uint32_t>(newLen);
            m_uiCapacity = m_uiLength + 1;
        }
        return *this;
    }

    // VString methods from IDA
    bool IsEmpty() const { return m_uiLength == 0 || m_pBuffer == nullptr; }

    void Format(const char* fmt, ...) {
        char buffer[256];
        va_list args;
        va_start(args, fmt);
        int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        if (len > 0) {
            if (m_pBuffer) delete[] m_pBuffer;
            m_uiLength = static_cast<std::uint32_t>(len);
            m_uiCapacity = m_uiLength + 1;
            m_pBuffer = new char[m_uiCapacity];
            std::memcpy(m_pBuffer, buffer, m_uiCapacity);
        }
    }

    void Reset() {
        if (m_pBuffer) {
            delete[] m_pBuffer;
            m_pBuffer = nullptr;
        }
        m_uiLength = 0;
        m_uiCapacity = 0;
    }

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

    // ToLower - convert string to lowercase
    void ToLower() {
        if (m_pBuffer) {
            for (std::uint32_t i = 0; i < m_uiLength; ++i) {
                m_pBuffer[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(m_pBuffer[i])));
            }
        }
    }

    // CompareNoCase - case-insensitive comparison
    int CompareNoCase(const char* str) const {
        const char* a = m_pBuffer ? m_pBuffer : "";
        const char* b = str ? str : "";
#ifdef _WIN32
        return _stricmp(a, b);
#else
        return strcasecmp(a, b);
#endif
    }
};

// Note: hkvVec3, hkvPlane, hkvMat3 are now defined in their respective header files
// included above: hkvVec3.h, hkvMat3.h

// ============================================================================
// Additional Vision Engine types
// ============================================================================

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
    SDefenseChangeInfo(std::uint32_t id, std::uint8_t type, float time)
        : dwID(id), byDefenseType(type), fChangeTime(time) {}

    void Clear() {
        dwID = 0;
        byDefenseType = 0;
        fChangeTime = 0.0f;
    }
};

// SFilterData - 过滤数据
// IDA: ?SetFilterData@CMover@@QEAAXHHHH@Z shows struct has 3 DWORD members
struct SFilterData {
    std::uint32_t dwFilterData1;
    std::uint32_t dwFilterData2;
    std::uint32_t dwFilterData3;

    SFilterData() : dwFilterData1(0), dwFilterData2(0), dwFilterData3(0) {}
};

// SItemRateInfo - Item rate info for calculating item rates
// Used by GetItemRateResultWeapon, GetItemRateResultGear
struct SItemRateInfo {
    int iItemValue;           // Item value
    std::uint16_t wItemLevel; // Item level
    std::uint8_t byItemRank;  // Item rank
    int iItemValueCritical;   // Critical item value

    SItemRateInfo() : iItemValue(0), wItemLevel(0), byItemRank(0), iItemValueCritical(0) {}
    SItemRateInfo(int value, std::uint16_t level, std::uint8_t rank, int critValue)
        : iItemValue(value), wItemLevel(level), byItemRank(rank), iItemValueCritical(critValue) {}
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
    void SetString(const char* value) {
        if (value) {
            std::size_t len = std::strlen(value) + 1;
            std::memcpy(&szBuffer[nCurrent], value, len);
            nCurrent += static_cast<std::int16_t>(len);
        }
    }

    // IDA: operator<< for tagACTION_BUFFER
    tagACTION_BUFFER& operator<<(float value) { SetFLOAT(value); return *this; }
    tagACTION_BUFFER& operator<<(int value) { SetINT(value); return *this; }
    tagACTION_BUFFER& operator<<(std::int16_t value) { SetSHORT(value); return *this; }
    tagACTION_BUFFER& operator<<(std::uint32_t value) { SetINT(static_cast<int>(value)); return *this; }
    tagACTION_BUFFER& operator<<(const char* value) { SetString(value); return *this; }
    tagACTION_BUFFER& operator<<(const std::string& value) { SetString(value.c_str()); return *this; }
    tagACTION_BUFFER& operator<<(std::uint8_t value) { SetINT(static_cast<int>(value)); return *this; }
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

    // IDA: ?FindCodeData@CActionBuffer@@QEAAPEAUtagACTION_BUFFER@@EPEAE@Z
    // Find action buffer by code, returns first match and sets byOutPos
    tagACTION_BUFFER* FindCodeData(std::uint8_t byCode, std::uint8_t* pOutPos) {
        for (int i = m_bCurPos; i < m_bActionCnt + m_bCurPos; ++i) {
            std::uint8_t bPos = static_cast<std::uint8_t>(i % 100);
            if (m_arAction[bPos].byCode == byCode) {
                if (pOutPos) {
                    *pOutPos = bPos;
                }
                return &m_arAction[bPos];
            }
        }
        return nullptr;
    }

    // IDA: ?Delete@CActionBuffer@@QEAAXE@Z
    // Delete action at specified position
    void Delete(std::uint8_t byPos) {
        if (m_bActionCnt == 0) return;
        // Shift elements to fill the gap
        for (int i = byPos; i < m_bActionCnt + m_bCurPos - 1; ++i) {
            std::uint8_t bCurPos = static_cast<std::uint8_t>(i % 100);
            std::uint8_t bNextPos = static_cast<std::uint8_t>((i + 1) % 100);
            m_arAction[bCurPos] = m_arAction[bNextPos];
        }
        m_bActionCnt--;
    }

    // IDA: ?DeleteCodeData@CActionBuffer@@QEAAXE@Z @ 0x140735550
    // Delete all actions with specified code
    void DeleteCodeData(std::uint8_t byCode) {
        bool bFound = true;
        while (bFound) {
            bFound = false;
            for (int i = m_bCurPos; i < m_bActionCnt + m_bCurPos; ++i) {
                std::uint8_t bPos = static_cast<std::uint8_t>(i % 100);
                if (m_arAction[bPos].byCode == byCode) {
                    Delete(bPos);
                    bFound = true;
                    break;
                }
            }
        }
    }

    // CheckTime - check if any action buffer time has elapsed
    bool CheckTime() {
        for (int i = m_bCurPos; i < m_bActionCnt + m_bCurPos; ++i) {
            if (m_arAction[static_cast<unsigned __int8>(i % 100)].fTime <= 0.0f) {
                return true;
            }
        }
        return false;
    }

    // Pop - remove and return the first expired action
    tagACTION_BUFFER* Pop() {
        if (m_bActionCnt == 0) return nullptr;
        unsigned __int8 bPos = static_cast<unsigned __int8>(m_bCurPos % 100);
        m_bCurPos++;
        m_bActionCnt--;
        return &m_arAction[bPos];
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

    // IsNoneZero - alias for !IsZero() (IDA naming)
    bool IsNoneZero() const {
        return x != 0.0f || y != 0.0f;
    }

    // Static version for IDA compatibility (tagMOVE_POS::IsNoneZero(&pos))
    static bool IsNoneZero(const tagMOVE_POS* pPos) {
        return pPos && (pPos->x != 0.0f || pPos->y != 0.0f);
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
// IDA: Complete struct from CheckDelayedProjectile
struct AttackJudgmentTrigger;  // forward declaration

struct SDelayedProjectile {
    std::uint32_t dwProjectileID;
    hkvVec3 vStartPos;
    hkvVec3 vTargetPos;
    float fDelayTime;
    float fCurTime;              // 当前时间
    float fCreateDelayTime;      // 创建延迟时间
    std::uint8_t byActive;
    std::uint8_t bUsed;          // 是否已使用
    std::uint8_t padding[2];
    AttackJudgmentTrigger* pTrigger;  // 攻击触发器
    std::uint32_t nSkillID;      // 技能ID

    SDelayedProjectile() : dwProjectileID(0), vStartPos(), vTargetPos(), fDelayTime(0.0f),
                           fCurTime(0.0f), fCreateDelayTime(0.0f), byActive(0), bUsed(0),
                           pTrigger(nullptr), nSkillID(0) {}
};

// ============================================================================
// Vision Engine 基础对象类型
// ============================================================================

// Note: VTypedObject is now defined in VTypedObject.h

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
// Note: PS_TICKCOUNT_INFO is defined in PSServerDB.h to avoid duplicate definition

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
    int GetCurID() const { return m_nCount; }  // IDA compatibility alias
    T& operator[](int idx) { return m_pElements[idx]; }
    const T& operator[](int idx) const { return m_pElements[idx]; }
    T* ElementAt(int idx) { return &m_pElements[idx]; }
    const T* ElementAt(int idx) const { return &m_pElements[idx]; }

    // IDA compatibility methods
    void SetSize(int nSize, int nGrowBy = -1, bool bPreserve = false) {
        (void)nGrowBy;
        (void)bPreserve;
        if (m_pElements) delete[] m_pElements;
        m_pElements = new T[nSize];
        m_nCount = nSize;
        m_nCapacity = nSize;
    }

    void SetAt(int idx, T* pValue) {
        if (idx >= 0 && idx < m_nCount) {
            m_pElements[idx] = *pValue;
        }
    }

    // RemoveAll - clear all elements (IDA compatibility)
    void RemoveAll() {
        if (m_pElements) {
            delete[] m_pElements;
            m_pElements = nullptr;
        }
        m_nCount = 0;
        m_nCapacity = 0;
    }
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

// Convenience alias for legacy code
constexpr TypeOfActionBufferBehavior RESETAFTER = ACTION_BUFFER_BEHAVIOR_RESETAFTER;

// TypeOfMoving - 移动类型枚举
enum TypeOfMoving {
    MOVING_NONE = 0,
    MOVING_NORMAL = 1,
    MOVING_SPECIAL = 2,
    MOVE_UPPER_ANIM = 1  // Alias for upper body animation (same as MOVING_NORMAL)
};

// TypeOfDefense - 防御类型枚举
enum TypeOfDefense {
    DEFENSE_NONE = 0,
    DEFENSE_NORMAL = 1,
    SUPER_ARMOR = 2,
    INVINCIBLE = 3
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

// Convenience alias for legacy code - LOOP means continue looping (no end action)
constexpr EndOfAnimationType LOOP = END_OF_ANIM_NONE;

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

    // IDA: 0x140016E80 - SetFiltering static function
    static void SetFiltering(int iSkillLevel, int iChargeLevel, int iCombatType, int iDivergence,
                            int iActionCondition, int iPvpCondition,
                            int* pFilterData1, int* pFilterData2, int* pFilterData3) {
        if (pFilterData1) {
            *pFilterData1 = (iSkillLevel & 0xFF) | ((iChargeLevel & 0xFF) << 8) |
                           ((iCombatType & 0xFF) << 16) | ((iDivergence & 0xFF) << 24);
        }
        if (pFilterData2) {
            *pFilterData2 = (iActionCondition & 0xFFFF) | ((iPvpCondition & 0x3FFF) << 16);
        }
        if (pFilterData3) {
            *pFilterData3 = 0;
        }
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

    // IDA: ?GetBoneRotaion@VAnimationInfo@@SAMAEBU1@HM@Z (note: typo in original name)
    // Get bone rotation at specified time
    static float GetBoneRotaion(const VAnimationInfo* pInfo, int idx, float fTime) {
        // TODO: 需要还原 - 目前返回0
        // 需要从 arRotationFrames 中获取骨骼旋转数据
        (void)pInfo;
        (void)idx;
        (void)fTime;
        return 0.0f;
    }

    // IDA: ?CopyData@VAnimationInfo@@QEAAXAEAV1@@Z (0x1403766C0)
    // Copy animation info data from another instance
    void CopyData(VAnimationInfo& other) {
        std::strcpy(szName, other.szName);
        eCanMoving = other.eCanMoving;
        iAnimGroup = other.iAnimGroup;
        fUpperRotRate = other.fUpperRotRate;
        eDefenseType = other.eDefenseType;
        eHUDType = other.eHUDType;
        eAnimationBehavior = other.eAnimationBehavior;
        eActionBufferBehavior = other.eActionBufferBehavior;
        eEndofAnimation = other.eEndofAnimation;
        iAnimBlendingType = other.iAnimBlendingType;
        std::memcpy(arTriggerTypeCounter, other.arTriggerTypeCounter, sizeof(arTriggerTypeCounter));
        fAnimationLength = other.fAnimationLength;
        // Note: VArray assignment would need operator= to be implemented
        // arOffsetDeltaFrames = other.arOffsetDeltaFrames;
        // arTranslationFrames = other.arTranslationFrames;
        // arRotationFrames = other.arRotationFrames;
    }

    // IDA: ?GetOffsetDelta@VAnimationInfo@@SAXAEBU1@AEAVhkvVec3@@MM@Z
    // Get animation offset delta between two time points
    static void GetOffsetDelta(const VAnimationInfo* pInfo, hkvVec3* pOffset, float fPrevTime, float fCurTime) {
        // TODO: 需要还原 - 目前返回零向量
        // 需要从动画帧数据中计算偏移
        (void)pInfo;
        (void)fPrevTime;
        (void)fCurTime;
        if (pOffset) {
            pOffset->x = 0.0f;
            pOffset->y = 0.0f;
            pOffset->z = 0.0f;
        }
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

// Forward declaration
struct VJumpInfo;

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

    // GetJumpInfo - stub for now
    VJumpInfo* GetJumpInfo(const char* pszName) const {
        // TODO: Implement from IDA
        (void)pszName;
        return nullptr;
    }

    // IsLoaded - check if resource data is loaded
    bool IsLoaded() const {
        return base.m_pResourceData != nullptr;
    }

    // AddJumpInfo - stub for now (IDA: loads jump data from .jdf file)
    void AddJumpInfo(const char* pszFilePath) {
        // TODO: Implement from IDA
        (void)pszFilePath;
    }

    // GetAttackEvent - IDA: ?GetAttackEvent@VActionResourceLump@@QEAAPEAVActionTrigger@@H@Z (0x1407302A0)
    // 获取攻击事件触发器
    ActionTrigger* GetAttackEvent(int nEventID) {
        auto it = m_mapAttackTrigger.find(nEventID);
        if (it != m_mapAttackTrigger.end()) {
            return it->second;
        }
        return nullptr;
    }
    const ActionTrigger* GetAttackEvent(int nEventID) const {
        auto it = m_mapAttackTrigger.find(nEventID);
        if (it != m_mapAttackTrigger.end()) {
            return it->second;
        }
        return nullptr;
    }
};

// VJumpInfo - Jump info structure (stub)
struct VJumpInfo {
    float GetCurHeight(float fTime) const {
        // TODO: Implement from IDA
        (void)fTime;
        return 0.0f;
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

// ============================================================================
// Action Trigger Types (from IDA ActionDestToEntity @ 0x14000a280)
// ============================================================================

// ChargingInputTrigger - 充力输入触发器 (TypeOfTrigger = 4)
// IDA: offset 148 = StartTime (inherited from ActionTrigger)
class ChargingInputTrigger : public ActionTrigger {
public:
    // Inherits StartTime at offset 148
    ChargingInputTrigger() : ActionTrigger() { TypeOfTrigger = 4; }
};

// UserDataTrigger - 用户数据触发器 (TypeOfTrigger = 5)
class UserDataTrigger : public ActionTrigger {
public:
    UserDataTrigger() : ActionTrigger() { TypeOfTrigger = 5; }
};

// MovingInputTrigger - 移动输入触发器 (TypeOfTrigger = 8)
class MovingInputTrigger : public ActionTrigger {
public:
    MovingInputTrigger() : ActionTrigger() { TypeOfTrigger = 8; }
};

// ExtraInputTrigger - 额外输入触发器 (TypeOfTrigger = 9)
// IDA: used in GetAttackInputEvent, has ReplayTime and SkipTime fields
class ExtraInputTrigger : public ActionTrigger {
public:
    float ReplayTime;   // Time to replay animation
    float SkipTime;     // Time to skip
    float StartTime;    // Start time

    ExtraInputTrigger() : ActionTrigger(), ReplayTime(0.0f), SkipTime(0.0f), StartTime(0.0f) {
        TypeOfTrigger = 9;
    }
};

// JumpAttackTrigger - 跳跃攻击触发器 (TypeOfTrigger = 10)
class JumpAttackTrigger : public ActionTrigger {
public:
    JumpAttackTrigger() : ActionTrigger() { TypeOfTrigger = 10; }
};

// DeathTrigger - 死亡触发器 (TypeOfTrigger = 13)
class DeathTrigger : public ActionTrigger {
public:
    DeathTrigger() : ActionTrigger() { TypeOfTrigger = 13; }
};

// InvisibleTrigger - 隐身触发器 (TypeOfTrigger = 14)
class InvisibleTrigger : public ActionTrigger {
public:
    InvisibleTrigger() : ActionTrigger() { TypeOfTrigger = 14; }
};

// WarpToPointTrigger - 传送到点触发器 (TypeOfTrigger = 15)
class WarpToPointTrigger : public ActionTrigger {
public:
    WarpToPointTrigger() : ActionTrigger() { TypeOfTrigger = 15; }
};

// SummonMonsterTrigger - 召唤怪物触发器 (TypeOfTrigger = 16)
// IDA: offset 168+ = SummonType, SummonID, SummonChance
class SummonMonsterTrigger : public ActionTrigger {
public:
    std::int32_t SummonType;      // offset 168
    std::int32_t SummonID;        // offset 172
    std::int32_t SummonChance;    // offset 176 (0-10000)

    SummonMonsterTrigger() : ActionTrigger(), SummonType(0), SummonID(0), SummonChance(0) {
        TypeOfTrigger = 16;
    }
};

// LuaFunctionCallTrigger - Lua函数调用触发器 (TypeOfTrigger = 17)
class LuaFunctionCallTrigger : public ActionTrigger {
public:
    LuaFunctionCallTrigger() : ActionTrigger() { TypeOfTrigger = 17; }
};

// AkashicTrigger - Akashic触发器 (TypeOfTrigger = 18)
class AkashicTrigger : public ActionTrigger {
public:
    AkashicTrigger() : ActionTrigger() { TypeOfTrigger = 18; }
};

// SubordinationComboTrigger - 从属连击触发器 (TypeOfTrigger = 21)
// IDA: sSuboComboDesc[20] array with szAniName
struct SuboComboDesc {
    char szAniName[128];
    SuboComboDesc() { std::memset(szAniName, 0, sizeof(szAniName)); }
};

class SubordinationComboTrigger : public ActionTrigger {
public:
    SuboComboDesc sSuboComboDesc[20];
    // IDA: SubordinationComboTrigger constructor @ 0x14072D6C0
    float fMaxWaitTime;         // offset after sSuboComboDesc
    int iMaxCount;              // max count
    std::uint8_t bCancelRClick; // cancel right click
    std::uint8_t bPlayOnce;     // play once
    std::uint8_t bSkipEndMotion; // skip end motion
    std::uint8_t bEnableChanageDir; // enable change direction (typo in original)

    SubordinationComboTrigger() : ActionTrigger(), fMaxWaitTime(0.0f), iMaxCount(0),
        bCancelRClick(0), bPlayOnce(0), bSkipEndMotion(0), bEnableChanageDir(0) {
        TypeOfTrigger = 21;
    }
};

// AttachToAttackerTrigger - 附加到攻击者触发器 (TypeOfTrigger = 22)
class AttachToAttackerTrigger : public ActionTrigger {
public:
    AttachToAttackerTrigger() : ActionTrigger() { TypeOfTrigger = 22; }
};

// AnimSpeedTrigger - 动画速度触发器 (TypeOfTrigger = 23)
class AnimSpeedTrigger : public ActionTrigger {
public:
    AnimSpeedTrigger() : ActionTrigger() { TypeOfTrigger = 23; }
};

// CounterAttackTrigger - 反击触发器 (TypeOfTrigger = 24)
class CounterAttackTrigger : public ActionTrigger {
public:
    CounterAttackTrigger() : ActionTrigger() { TypeOfTrigger = 24; }
};

// DefenseTypeTrigger - 防御类型触发器 (TypeOfTrigger = 25)
class DefenseTypeTrigger : public ActionTrigger {
public:
    DefenseTypeTrigger() : ActionTrigger() { TypeOfTrigger = 25; }
};

// DetachTrigger - 分离触发器 (TypeOfTrigger = 30)
class DetachTrigger : public ActionTrigger {
public:
    DetachTrigger() : ActionTrigger() { TypeOfTrigger = 30; }
};

// CollisionChangeTrigger - 碰撞变更触发器 (TypeOfTrigger = 33)
class CollisionChangeTrigger : public ActionTrigger {
public:
    CollisionChangeTrigger() : ActionTrigger() { TypeOfTrigger = 33; }
};

// AutoRotationTrigger - 自动旋转触发器 (TypeOfTrigger = 35)
class AutoRotationTrigger : public ActionTrigger {
public:
    AutoRotationTrigger() : ActionTrigger() { TypeOfTrigger = 35; }
};

// RandomSummonTrigger - 随机召唤触发器 (TypeOfTrigger = 36)
class RandomSummonTrigger : public ActionTrigger {
public:
    RandomSummonTrigger() : ActionTrigger() { TypeOfTrigger = 36; }
};

// LinkSkillTrigger - 链接技能触发器 (TypeOfTrigger = 37)
class LinkSkillTrigger : public ActionTrigger {
public:
    LinkSkillTrigger() : ActionTrigger() { TypeOfTrigger = 37; }
};

// CheckAttackSkillTrigger - 检查攻击技能触发器 (TypeOfTrigger = 38)
class CheckAttackSkillTrigger : public ActionTrigger {
public:
    // IDA: Inferred from CheckAttackSkillEnable (0x14039E370)
    std::int32_t nMinRange;     // Minimum attack range
    std::int32_t nMaxRange;     // Maximum attack range
    std::int32_t nAngle;        // Attack angle in degrees
    float fReplayTime;          // Replay time for animation

    CheckAttackSkillTrigger() : ActionTrigger(), nMinRange(0), nMaxRange(0), nAngle(0), fReplayTime(0.0f) { TypeOfTrigger = 38; }
};

// DelSummonMonsterTrigger - 删除召唤怪物触发器 (TypeOfTrigger = 39)
// IDA: MonsterID field
class DelSummonMonsterTrigger : public ActionTrigger {
public:
    std::int32_t MonsterID;  // offset 168

    DelSummonMonsterTrigger() : ActionTrigger(), MonsterID(0) { TypeOfTrigger = 39; }
};

// ApplyPassiveSkillTrigger - 应用被动技能触发器 (TypeOfTrigger = 40)
// IDA: iSkillGroupID field
class ApplyPassiveSkillTrigger : public ActionTrigger {
public:
    std::int32_t iSkillGroupID;  // offset 168

    ApplyPassiveSkillTrigger() : ActionTrigger(), iSkillGroupID(0) { TypeOfTrigger = 40; }
};

// MyBuffControlTrigger - 我的Buff控制触发器 (TypeOfTrigger = 41)
class MyBuffControlTrigger : public ActionTrigger {
public:
    MyBuffControlTrigger() : ActionTrigger() { TypeOfTrigger = 41; }
};
