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

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_StdCompat.h"

// VBaseObject (8 bytes: vptr + virtual dtor) is defined in TXSingleton.h
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"

// 包含 Vision Engine 子类型定义
#include "VisionEngineTypes/hkvVec3.h"
#include "VisionEngineTypes/hkvVec4.h"
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

    // IDA: ??0hkvAlignedBBox@@QEAA@AEBVhkvVec3@@0@Z @ 0x140377b20
    hkvAlignedBBox(const hkvVec3& vMin, const hkvVec3& vMax) {
        setWithoutValidityCheck(vMin, vMax);
    }

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

    // IDA: ?setWithoutValidityCheck@hkvAlignedBBox@@QEAAXAEBVhkvVec3@@0@Z @ 0x140377ab0
    void setWithoutValidityCheck(const hkvVec3& vMin, const hkvVec3& vMax) {
        m_vMin = vMin;
        m_vMax = vMax;
    }

    // IDA: ?getCorners@hkvAlignedBBox@@QEBAXPEAVhkvVec3@@@Z @ 0x140377630
    void getCorners(hkvVec3* out_pVertices) const {
        hkvVec3::set(&out_pVertices[0], m_vMin.x, m_vMin.y, m_vMin.z);
        hkvVec3::set(&out_pVertices[1], m_vMin.x, m_vMin.y, m_vMax.z);
        hkvVec3::set(&out_pVertices[2], m_vMin.x, m_vMax.y, m_vMin.z);
        hkvVec3::set(&out_pVertices[3], m_vMin.x, m_vMax.y, m_vMax.z);
        hkvVec3::set(&out_pVertices[4], m_vMax.x, m_vMin.y, m_vMin.z);
        hkvVec3::set(&out_pVertices[5], m_vMax.x, m_vMin.y, m_vMax.z);
        hkvVec3::set(&out_pVertices[6], m_vMax.x, m_vMax.y, m_vMin.z);
        hkvVec3::set(&out_pVertices[7], m_vMax.x, m_vMax.y, m_vMax.z);
    }

    // IDA: ?expandToInclude@hkvAlignedBBox@@QEAAXAEBVhkvVec3@@@Z @ 0x1403777b0
    void expandToInclude(const hkvVec3& v) {
        hkvVec3::setMin(&m_vMin, &v);
        hkvVec3::setMax(&m_vMax, &v);
    }

    // IDA: ?expandToInclude@hkvAlignedBBox@@QEAAXAEBV1@@Z @ 0x140377a70
    void expandToInclude(const hkvAlignedBBox& cc) {
        hkvVec3::setMin(&m_vMin, &cc.m_vMin);
        hkvVec3::setMax(&m_vMax, &cc.m_vMax);
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
    int iItemValue;
    std::uint16_t wItemLevel;
    std::uint8_t byItemRank;
    int iItemValueCritical;

    SItemRateInfo() : iItemValue(0), wItemLevel(0), byItemRank(0), iItemValueCritical(0) {}
    SItemRateInfo(int value, std::uint16_t level, std::uint8_t rank, int critValue)
        : iItemValue(value), wItemLevel(level), byItemRank(rank), iItemValueCritical(critValue) {}

    int GetItemRateResult(float fApplyRate, float fRankRate) {
        const float fItemValue = static_cast<float>(iItemValue);
        return static_cast<int>(fItemValue * fApplyRate * fRankRate - fItemValue);
    }

    void AddValue(int iAddValue, int iValueCritical) {
        iItemValue += iAddValue;
        iItemValueCritical += iValueCritical;
    }
};

static_assert(sizeof(SItemRateInfo) == 0x0C,
              "SItemRateInfo must match the GameServer PDB");
static_assert(offsetof(SItemRateInfo, iItemValueCritical) == 0x08,
              "SItemRateInfo critical value offset must match the GameServer PDB");

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

    // IDA: ?ResetPosition@tagACTION_BUFFER@@QEAAXXZ @ 0x1403A1F10
    void ResetPosition() { nCurrent = 0; }

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

    // Member functions implemented in WayPoint.cpp
    void Update(float fElapsedTime);
    void NextWayPoint();
    void CheckIdleAction();

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
// PDB LF_CLASS 0x4A80B: Size 16 = VBaseObject base (vptr, 8 bytes) + m_iRefCount @ 8 (protected)
// IDA: ?AddRef@VRefCounter@@QEAAKXZ @ 0x140642400
//      ?ReleaseNoDelete@VRefCounter@@QEAAXXZ @ 0x1406424E0
//      ?Release@VRefCounter@@QEAAKXZ @ 0x140642500
//      ?DeleteThis@VRefCounter@@UEAAXXZ @ 0x140188D60
// PDB: ~VRefCounter 与 DeleteThis 为虚 (DeleteThis INTRODUCING VIRTUAL vfptr offset 8)，
//      AddRef/Release/ReleaseNoDelete 为 VANILLA (非虚)
class VRefCounter : public VBaseObject {
public:
    VRefCounter() : m_iRefCount(1) {}
    virtual ~VRefCounter() {}

    // DeleteThis - 调用虚析构链 (dtr_VBaseObject(this, 1))
    virtual void DeleteThis() {
        delete this;
    }

    // AddRef - VAtomic::Increment(m_iRefCount) then return count
    unsigned int AddRef() {
        m_iRefCount++;
        return (unsigned int)m_iRefCount;
    }

    // ReleaseNoDelete - VAtomic::Decrement(m_iRefCount)
    void ReleaseNoDelete() {
        m_iRefCount--;
    }

    // Release - decrement; delete this when count reaches zero
    unsigned int Release() {
        ReleaseNoDelete();
        unsigned int iRefCount = (unsigned int)m_iRefCount;
        if (!iRefCount)
            DeleteThis();
        return iRefCount;
    }

protected:
    int m_iRefCount;   // offset 8
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

// VJumpFrame - 跳跃关键帧 (8 bytes)
// PDB LF_STRUCTURE 0x2C04B: Size 8, fTime float(0) fZPos float(4)
struct VJumpFrame {
    float fTime;   // offset 0
    float fZPos;   // offset 4

    VJumpFrame() : fTime(0.0f), fZPos(0.0f) {}
};

// VJumpInfo - 跳跃信息 (152 bytes)
// PDB LF_FIELDLIST 0x77C03: Size 152, szName char[128](0) arTranslationFrames VArray<VJumpFrame>(128)
// IDA ctor: ??0VJumpInfo@@QEAA@XZ @ 0x14072ECA0
//   VArray<VJumpFrame> ctor then memset(this, 0, 0x80) then VArray::Reset(arTranslationFrames);
//   net effect = 128-byte zeroed header + default-constructed VArray (null/count/capacity 0)
struct VJumpInfo {
    char szName[128];                     // offset 0
    VArray<VJumpFrame> arTranslationFrames; // offset 128 (24 bytes)

    VJumpInfo() : arTranslationFrames() {
        std::memset(szName, 0, sizeof(szName));
    }

    float GetCurHeight(float fTime) const {
        // IDA 0x1403A2890 - VJumpInfo::GetCurHeight
        (void)fTime;
        return 0.0f;
    }
};

// VChunkFile - Vision Engine 分块文件读取类 (240 bytes)
// PDB LF_CLASS 0x49820: Size 240. Read 为外部引擎导入，当前重建提供最小兼容接口。
class VChunkFile {
public:
    // IDA: ?ReadShort@VChunkFile@@QEAAHAEAF@Z @ 0x140640210
    // return Read(this, iVal, 2, "s") == 2
    virtual int ReadShort(short* iVal) {
        (void)iVal;
        // TODO: 依赖外部引擎 VChunkFile::Read，当前重建无法精确读取分块文件
        return 0;
    }

    // IDA: ?ReadInt@VChunkFile@@QEAAHAEAH@Z @ 0x140640260
    // return ReadDWord(this, iVal) == 4
    virtual int ReadInt(int* iVal) {
        (void)iVal;
        return 0;
    }

    // IDA: ?Readbool@VChunkFile@@QEAAHAEA_N@Z @ 0x1406402B0
    // return Read(this, cVal, 1) == 1
    virtual int Readbool(char* cVal) {
        (void)cVal;
        return 0;
    }

    virtual ~VChunkFile() {}
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
    std::vector<VJumpInfo> m_arJumpInfos;               // 跳跃信息 (offset 168, 32 bytes)
    std::map<int, ActionTrigger*> m_mapAttackTrigger;   // 攻击触发器映射 (offset 200, 32 bytes)

    VActionResourceLump() : base() {}

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

    // CreateTrigger - IDA: ?CreateTrigger@VActionResourceLump@@SAPEAVActionTrigger@@PEAVVChunkFile@@@Z @ 0x14072F350
    // 从 VChunkFile 读取触发器类型并创建对应触发器实例
    static ActionTrigger* CreateTrigger(VChunkFile* infile);
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

// Forward declaration
struct VEventBoxInfo;

// VCommonPositionBoxInfo - 通用位置盒信息
// PDB UDT 0x77D8F (176B, 继承 VEventBoxInfo) 的完整精确布局
// 已收敛到 InteractionObject.h 的 pack(4) 精确定义，
// 此处仅保留前向声明，删除旧的简化版 (m_nID/m_vMin/m_vMax 臆造字段已废弃)。
struct VCommonPositionBoxInfo;

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
// PDB LF_FIELDLIST 0x78CDA: Size 184, fAnimSpeed float(168) fMaxMultiple float(172) fDuration float(176)
// IDA ctor: ??0ChargingInputTrigger@@QEAA@XZ @ 0x14072DD70 (Size 184 = 0xB8), all three floats 0.0f
class ChargingInputTrigger : public ActionTrigger {
public:
    float fAnimSpeed;     // offset 168
    float fMaxMultiple;   // offset 172
    float fDuration;      // offset 176

    ChargingInputTrigger() : ActionTrigger(), fAnimSpeed(0.0f), fMaxMultiple(0.0f), fDuration(0.0f) {}
};

// UserDataTrigger - 用户数据触发器 (TypeOfTrigger = 5)
// PDB LF_FIELDLIST 0x6D759: Size 184, DataType int(168) DataValue int(172) DataValue2 int(176)
// IDA ctor: ??0UserDataTrigger@@QEAA@XZ @ 0x14072BFD0 (Size 184 = 0xB8), all three ints 0
class UserDataTrigger : public ActionTrigger {
public:
    std::int32_t DataType;    // offset 168
    std::int32_t DataValue;   // offset 172
    std::int32_t DataValue2;  // offset 176

    UserDataTrigger() : ActionTrigger(), DataType(0), DataValue(0), DataValue2(0) {}
};

// SRangeInfo - 攻击范围信息 (36 bytes)
// PDB LF_FIELDLIST 0x26B5A: vCenterPos(0) fBoxSizeX(12) fBoxSizeY(16) fRadius(20) fAngle(24) fHeight(28) fStartPos(32)
// MovingInputTrigger::sAttackRange 使用 (type 0x26B4B)
struct SRangeInfo {
    hkvVec3 vCenterPos;  // offset 0
    float fBoxSizeX;     // offset 12
    float fBoxSizeY;     // offset 16
    float fRadius;       // offset 20
    float fAngle;        // offset 24
    float fHeight;       // offset 28
    float fStartPos;     // offset 32

    SRangeInfo() : vCenterPos(), fBoxSizeX(0.0f), fBoxSizeY(0.0f), fRadius(0.0f),
        fAngle(0.0f), fHeight(0.0f), fStartPos(0.0f) {}
};

// MovingInputTrigger - 移动输入触发器 (TypeOfTrigger = 8)
// PDB LF_FIELDLIST 0x69F88: Size 240, Duration(168) MaxSpeed(172) OffsetDistance(176) MovingType(180)
//   MonsterRank(184) MonsterWeightRank(188) sAttackRange SRangeInfo(192) vPullPoint hkvVec3(228)
// IDA ctor: ??0MovingInputTrigger@@QEAA@XZ @ 0x14072B640 (Size 240 = 0xF0)
class MovingInputTrigger : public ActionTrigger {
public:
    float Duration;            // offset 168
    float MaxSpeed;            // offset 172
    float OffsetDistance;      // offset 176
    std::int32_t MovingType;   // offset 180
    std::int32_t MonsterRank;  // offset 184
    std::int32_t MonsterWeightRank; // offset 188
    SRangeInfo sAttackRange;   // offset 192
    hkvVec3 vPullPoint;        // offset 228

    MovingInputTrigger() : ActionTrigger(), Duration(0.0f), MaxSpeed(0.0f), OffsetDistance(0.0f),
        MovingType(0), MonsterRank(0), MonsterWeightRank(0), sAttackRange(), vPullPoint() {}
};

// ExtraInputTrigger - 额外输入触发器 (CreateTrigger factory case 2)
// PDB LF_FIELDLIST 0x6F422: Size 176, SkipTime float(168) ReplayTime float(172)
// IDA ctor: ??0ExtraInputTrigger@@QEAA@XZ @ 0x14072B370 (Size 176 = 0xB0), both floats 0.0f
// Note: factory switch case 2 creates this class; the old header comment "TypeOfTrigger = 9" was wrong.
class ExtraInputTrigger : public ActionTrigger {
public:
    float SkipTime;     // offset 168
    float ReplayTime;   // offset 172

    ExtraInputTrigger() : ActionTrigger(), SkipTime(0.0f), ReplayTime(0.0f) {}
};

// JumpAttackTrigger - 跳跃攻击触发器 (TypeOfTrigger = 10)
// PDB LF_FIELDLIST 0x6D640: base ActionTrigger offset 0, AddVelocityZ(168) GravityScale(172) JumpDelayTime(176) JumpEndDelayTime(180)
// IDA ctor: ??0JumpAttackTrigger@@QEAA@XZ @ 0x14072BB20 (Size 184 = 0xB8)
class JumpAttackTrigger : public ActionTrigger {
public:
    float AddVelocityZ;      // offset 168
    float GravityScale;      // offset 172
    float JumpDelayTime;     // offset 176
    float JumpEndDelayTime;  // offset 180

    JumpAttackTrigger() : ActionTrigger(), AddVelocityZ(0.0f), GravityScale(0.0f),
        JumpDelayTime(0.0f), JumpEndDelayTime(0.0f) {}
};

// DeathTrigger - 死亡触发器 (TypeOfTrigger = 13)
// PDB LF_FIELDLIST 0x7682F: Size 304, szDeathAnim(168) IsLocalClient(296) bMustExcute(297)
// IDA ctor: ??0DeathTrigger@@QEAA@XZ @ 0x14072C3D0 (Size 304 = 0x130)
class DeathTrigger : public ActionTrigger {
public:
    char szDeathAnim[128];  // offset 168
    bool IsLocalClient;     // offset 296
    bool bMustExcute;       // offset 297

    DeathTrigger() : ActionTrigger(), IsLocalClient(false), bMustExcute(false) {
        std::memset(szDeathAnim, 0, sizeof(szDeathAnim));
    }
};

// InvisibleTrigger - 隐身触发器 (TypeOfTrigger = 14)
// PDB LF_FIELDLIST 0x79A64: Size 192, CollisionOn(168) EffectOn(169) MoveOn(170) AIOn(171)
//   ConditionType(172) ConditionValue(176) ConditionExtValue1(180) ConditionExtValue2(184) ConditionExtValue3(188)
// IDB ctor: InvisibleTrigger::InvisibleTrigger(0x14072C4F0) (Size 192 = 0xC0)
class InvisibleTrigger : public ActionTrigger {
public:
    bool CollisionOn;       // offset 168
    bool EffectOn;          // offset 169
    bool MoveOn;            // offset 170
    bool AIOn;              // offset 171
    std::int32_t ConditionType;      // offset 172
    std::int32_t ConditionValue;     // offset 176
    std::int32_t ConditionExtValue1;// offset 180
    std::int32_t ConditionExtValue2;// offset 184
    std::int32_t ConditionExtValue3;// offset 188

    InvisibleTrigger() : ActionTrigger(), CollisionOn(false), EffectOn(false), MoveOn(false), AIOn(false),
        ConditionType(0), ConditionValue(0), ConditionExtValue1(0), ConditionExtValue2(0), ConditionExtValue3(0) {}
};

// WarpToPointTrigger - 传送到点触发器 (TypeOfTrigger = 15)
// PDB LF_FIELDLIST 0x70C00: Size 184, WarpPattern(168) WarpPoint(172) WarpYaw(176)
// IDB ctor: IDA 0x14072C6E0 WarpYaw = -1.0f, others 0 (Size 184 = 0xB8)
class WarpToPointTrigger : public ActionTrigger {
public:
    std::int32_t WarpPattern;  // offset 168
    std::int32_t WarpPoint;    // offset 172
    float WarpYaw;             // offset 176

    WarpToPointTrigger() : ActionTrigger(), WarpPattern(0), WarpPoint(0), WarpYaw(-1.0f) {}
};

// SummonMonsterTrigger - 召唤怪物触发器 (TypeOfTrigger = 16)
// IDA: serializer SummonMonsterTrigger::Serialize, vftable type 0x1117
// PDB LF_CLASS 0x76669: total size 528 (= ActionTrigger 168 + SummonMonsterTrigger 360)
// PDB LF_FIELDLIST 0x76668, method list 0x76662 with 2 ctors
// IDA ctor: ??0SummonMonsterTrigger@@QEAA@XZ @ 0x14072C8B0
class SummonMonsterTrigger : public ActionTrigger {
public:
    float AlphaValue;                // offset 168
    float BlendingTime;              // offset 172
    std::int32_t MonsterID;          // offset 176
    char szSummonAnim[128];          // offset 180
    hkvVec3 SummonPos;               // offset 308
    bool ApplyRotation;              // offset 320
    std::int32_t SummonType;         // offset 324
    std::int32_t SummonChance;       // offset 328 (0-10000)
    std::int32_t SummonID;           // offset 332
    bool AlwaysOnGround;             // offset 336
    bool FallowMonster;              // offset 337
    bool CopyMotion;                 // offset 338
    std::int32_t iEventBoxID;        // offset 340
    std::int32_t iChargeLevel;       // offset 344
    std::int32_t iSkillLevel;        // offset 348
    std::int16_t sSkillCondition;    // offset 352
    std::int32_t iCombatType;        // offset 356
    char szDivergenceValue[128];     // offset 360
    float fDelayTime;                // offset 488
    std::int32_t iSummonCount;       // offset 492
    float RandomRadius;              // offset 496
    float Rotation;                  // offset 500
    bool IsLocalClient;              // offset 504
    bool bDeleteWhenMotionChange;    // offset 505
    float fSummonLifeTime;           // offset 508
    float fSummonDisappearTime;      // offset 512
    std::int32_t nSkillID;           // offset 516
    bool bSuicidePossible;           // offset 520

    SummonMonsterTrigger()
        : ActionTrigger()
        , AlphaValue(1.0f)
        , BlendingTime(1.0f)
        , MonsterID(0)
        , SummonPos()
        , ApplyRotation(true)
        , SummonType(0)
        , SummonChance(10000)
        , SummonID(0)
        , AlwaysOnGround(false)
        , FallowMonster(false)
        , CopyMotion(false)
        , iEventBoxID(0)
        , iChargeLevel(-1)
        , iSkillLevel(-1)
        , sSkillCondition(0)
        , iCombatType(-1)
        , fDelayTime(0.0f)
        , iSummonCount(1)
        , RandomRadius(0.0f)
        , Rotation(0.0f)
        , IsLocalClient(false)
        , bDeleteWhenMotionChange(false)
        , fSummonLifeTime(-1.0f)
        , fSummonDisappearTime(0.0f)
        , nSkillID(0)
        , bSuicidePossible(false)
    {
        std::memset(szSummonAnim, 0, sizeof(szSummonAnim));
        std::memset(szDivergenceValue, 0, sizeof(szDivergenceValue));
    }
};

// LuaFunctionCallTrigger - Lua函数调用触发器 (TypeOfTrigger = 17)
// PDB LF_FIELDLIST 0x6D4D4: Size 560, LuaFilename char[260](168) LuaFunction char[128](428)
// IDA: ??0LuaFunctionCallTrigger@@QEAA@XZ @ 0x14072C7D0, memset both arrays to 0 (Size 560 = 0x230)
class LuaFunctionCallTrigger : public ActionTrigger {
public:
    char LuaFilename[260];  // offset 168
    char LuaFunction[128];  // offset 428

    LuaFunctionCallTrigger() : ActionTrigger() {
        std::memset(LuaFilename, 0, sizeof(LuaFilename));
        std::memset(LuaFunction, 0, sizeof(LuaFunction));
    }
};

// AkashicTrigger - Akashic触发器 (TypeOfTrigger = 18)
// PDB LF_FIELDLIST 0x69293: Size 192, AlphaValue float(168) BlendingTime float(172)
//   SummonPos hkvVec3(176) ApplyRotation bool(188)
// IDA ctor: ??0AkashicTrigger@@QEAA@XZ @ 0x14072D220 (Size 192 = 0xC0),
//   SummonPos default-zero, AlphaValue = BlendingTime = 1.0f, ApplyRotation = true
class AkashicTrigger : public ActionTrigger {
public:
    float AlphaValue;     // offset 168
    float BlendingTime;   // offset 172
    hkvVec3 SummonPos;    // offset 176
    bool ApplyRotation;   // offset 188

    AkashicTrigger() : ActionTrigger(), AlphaValue(1.0f), BlendingTime(1.0f),
        SummonPos(), ApplyRotation(true) {}
};

// SubordinationComboTrigger - 从属连击触发器 (TypeOfTrigger = 21)
// PDB tagSUBO_COMBO_DESC: Size 132 = szAniName char[128](0) + iUseableLevel int(128)
// sSuboComboDesc[20] 数组共 2640B (offset 168..2808)
struct SuboComboDesc {
    char szAniName[128];     // offset 0
    std::int32_t iUseableLevel; // offset 128

    SuboComboDesc() : iUseableLevel(0) {
        std::memset(szAniName, 0, sizeof(szAniName));
    }
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
        bCancelRClick(0), bPlayOnce(0), bSkipEndMotion(0), bEnableChanageDir(0) {}
};

// AttachToAttackerTrigger - 附加到攻击者触发器 (TypeOfTrigger = 22)
// PDB LF_FIELDLIST 0x7601F: Size 200, fLifeTime float(168) fAttachDist float(172) fAngle float(176)
//   vOffset hkvVec3(180) iApplyWeightRAnk int(192)
// IDA ctor: ??0AttachToAttackerTrigger@@QEAA@XZ @ 0x14072D930 (Size 200 = 0xC8),
//   vOffset zero, fLifeTime/fAttachDist/fAngle 0.0f, iApplyWeightRAnk 0
class AttachToAttackerTrigger : public ActionTrigger {
public:
    float fLifeTime;            // offset 168
    float fAttachDist;          // offset 172
    float fAngle;               // offset 176
    hkvVec3 vOffset;            // offset 180
    std::int32_t iApplyWeightRAnk; // offset 192

    AttachToAttackerTrigger() : ActionTrigger(), fLifeTime(0.0f), fAttachDist(0.0f),
        fAngle(0.0f), vOffset(), iApplyWeightRAnk(0) {}
};

// AnimSpeedTrigger - 动画速度触发器 (TypeOfTrigger = 23)
// PDB LF_FIELDLIST 0x69464: Size 176, fDuration float(168) fSpeed float(172)
// IDA ctor: ??0AnimSpeedTrigger@@QEAA@XZ @ 0x14072DAF0 (Size 176 = 0xB0), fDuration 0.0f, fSpeed 1.0f
class AnimSpeedTrigger : public ActionTrigger {
public:
    float fDuration;   // offset 168
    float fSpeed;      // offset 172

    AnimSpeedTrigger() : ActionTrigger(), fDuration(0.0f), fSpeed(1.0f) {}
};

// CounterAttackTrigger - 反击触发器 (TypeOfTrigger = 24)
// PDB LF_FIELDLIST 0x72B38: Size 320, fMinRange float(168) fMaxRange float(172) fAngle float(176)
//   szAniName char[128](180) iProbability int(308) fDuration float(312)
// IDA ctor: ??0CounterAttackTrigger@@QEAA@XZ @ 0x14072DBD0 (Size 320 = 0x140),
//   floats 0.0f, szAniName memset, iProbability 0, fDuration 0.0f
class CounterAttackTrigger : public ActionTrigger {
public:
    float fMinRange;       // offset 168
    float fMaxRange;       // offset 172
    float fAngle;          // offset 176
    char szAniName[128];   // offset 180
    std::int32_t iProbability; // offset 308
    float fDuration;       // offset 312

    CounterAttackTrigger() : ActionTrigger(), fMinRange(0.0f), fMaxRange(0.0f), fAngle(0.0f),
        iProbability(0), fDuration(0.0f) {
        std::memset(szAniName, 0, sizeof(szAniName));
    }
};

// DefenseTypeTrigger - 防御类型触发器 (TypeOfTrigger = 25)
// PDB LF_FIELDLIST 0x6B82D: Size 176, sDefenseType short(168) bImmunity bool(170) nHitEffect int(172)
// IDA ctor: ??0DefenseTypeTrigger@@QEAA@XZ @ 0x14072DE70 (Size 176 = 0xB0), all zero
class DefenseTypeTrigger : public ActionTrigger {
public:
    std::int16_t sDefenseType;  // offset 168
    bool bImmunity;             // offset 170
    std::int32_t nHitEffect;    // offset 172

    DefenseTypeTrigger() : ActionTrigger(), sDefenseType(0), bImmunity(false), nHitEffect(0) {}
};

// DetachTrigger - 分离触发器 (TypeOfTrigger = 30)
// PDB LF_FIELDLIST 0x760D2: Size 312, DropTime float(168) szAniName char[128](172) SkillID int(300) RefEventID int(304)
// IDA ctor: ??0DetachTrigger@@QEAA@XZ @ 0x14072B4C0 (Size 312 = 0x138), SkillID = RefEventID = -1
class DetachTrigger : public ActionTrigger {
public:
    float DropTime;          // offset 168
    char szAniName[128];     // offset 172
    std::int32_t SkillID;    // offset 300
    std::int32_t RefEventID; // offset 304

    DetachTrigger() : ActionTrigger(), DropTime(0.0f), SkillID(-1), RefEventID(-1) {
        std::memset(szAniName, 0, sizeof(szAniName));
    }
};

// CollisionChangeTrigger - 碰撞变更触发器 (TypeOfTrigger = 33)
// PDB LF_FIELDLIST 0x6C7D0: Size 176, nCollisionType int(168)
// IDA ctor: ??0CollisionChangeTrigger@@QEAA@XZ @ 0x14072E5B0 (Size 176 = 0xB0), nCollisionType 0
class CollisionChangeTrigger : public ActionTrigger {
public:
    std::int32_t nCollisionType;  // offset 168

    CollisionChangeTrigger() : ActionTrigger(), nCollisionType(0) {}
};

// AutoRotationTrigger - 自动旋转触发器 (TypeOfTrigger = 35)
// PDB LF_FIELDLIST 0x6F47F: Size 176, fDuration float(168) fSpeed float(172)
// IDA ctor: ??0AutoRotationTrigger@@QEAA@XZ @ 0x14072E650 (Size 176 = 0xB0), fDuration 0.0f, fSpeed 1.0f
class AutoRotationTrigger : public ActionTrigger {
public:
    float fDuration;   // offset 168
    float fSpeed;      // offset 172

    AutoRotationTrigger() : ActionTrigger(), fDuration(0.0f), fSpeed(1.0f) {}
};

// RandomSummonTrigger - 随机召唤触发器 (TypeOfTrigger = 36)
// PDB LF_FIELDLIST 0x6D45D: Size 352, szSummonAnim char[128](168) SummonPos hkvVec3(296)
//   RandomMinRadius float(308) RandomMaxRadius float(312) iSummonCount int(316) iMonsterID1(320) iMonsterID2(324) iMonsterID3(328)
//   iMonsterRate1(332) iMonsterRate2(336) iMonsterRate3(340) bSuicidePossible bool(344)
// IDA ctor: ??0RandomSummonTrigger@@QEAA@XZ @ 0x14072CF20 (Size 352 = 0x160)
class RandomSummonTrigger : public ActionTrigger {
public:
    char szSummonAnim[128];   // offset 168
    hkvVec3 SummonPos;        // offset 296
    float RandomMinRadius;    // offset 308
    float RandomMaxRadius;    // offset 312
    std::int32_t iSummonCount;   // offset 316
    std::int32_t iMonsterID1;    // offset 320
    std::int32_t iMonsterID2;    // offset 324
    std::int32_t iMonsterID3;    // offset 328
    std::int32_t iMonsterRate1;  // offset 332
    std::int32_t iMonsterRate2;  // offset 336
    std::int32_t iMonsterRate3;  // offset 340
    bool bSuicidePossible;       // offset 344

    RandomSummonTrigger() : ActionTrigger(), SummonPos(), RandomMinRadius(0.0f), RandomMaxRadius(0.0f),
        iSummonCount(0), iMonsterID1(0), iMonsterID2(0), iMonsterID3(0),
        iMonsterRate1(0), iMonsterRate2(0), iMonsterRate3(0), bSuicidePossible(false) {
        std::memset(szSummonAnim, 0, sizeof(szSummonAnim));
    }
};

// LinkSkillTrigger - 链接技能触发器 (TypeOfTrigger = 37)
// PDB LF_FIELDLIST 0x7872D: Size 184, nLinkType int(168) nSkillID int(172) nProbability int(176) fDuration float(180)
// IDA ctor: ??0LinkSkillTrigger@@QEAA@XZ @ 0x14072E720 (Size 184 = 0xB8), ints 0, fDuration 0.0f
class LinkSkillTrigger : public ActionTrigger {
public:
    std::int32_t nLinkType;     // offset 168
    std::int32_t nSkillID;      // offset 172
    std::int32_t nProbability;  // offset 176
    float fDuration;            // offset 180

    LinkSkillTrigger() : ActionTrigger(), nLinkType(0), nSkillID(0), nProbability(0), fDuration(0.0f) {}
};

// CheckAttackSkillTrigger - 检查攻击技能触发器 (TypeOfTrigger = 38)
// PDB LF_FIELDLIST 0x729C5: Size 192, nAngle(168) nMinRange(172) nMaxRange(176) nSkillID(180) nProbability(184) fDuration float(188)
// IDA ctor: ??0CheckAttackSkillTrigger@@QEAA@XZ @ 0x14072E860 (Size 192 = 0xC0)
class CheckAttackSkillTrigger : public ActionTrigger {
public:
    std::int32_t nAngle;       // offset 168
    std::int32_t nMinRange;    // offset 172
    std::int32_t nMaxRange;    // offset 176
    std::int32_t nSkillID;     // offset 180
    std::int32_t nProbability; // offset 184
    float fDuration;           // offset 188

    CheckAttackSkillTrigger() : ActionTrigger(), nAngle(0), nMinRange(0), nMaxRange(0),
        nSkillID(0), nProbability(0), fDuration(0.0f) {}
};

// DelSummonMonsterTrigger - 删除召唤怪物触发器 (TypeOfTrigger = 39)
// PDB LF_FIELDLIST 0x6BB09: Size 304, MonsterID int(168) szSummonAnim char[128](172)
// IDA ctor: ??0DelSummonMonsterTrigger@@QEAA@XZ @ 0x14072E9F0 (Size 304 = 0x130)
class DelSummonMonsterTrigger : public ActionTrigger {
public:
    std::int32_t MonsterID;    // offset 168
    char szSummonAnim[128];    // offset 172

    DelSummonMonsterTrigger() : ActionTrigger(), MonsterID(0) {
        std::memset(szSummonAnim, 0, sizeof(szSummonAnim));
    }
};

// ApplyPassiveSkillTrigger - 应用被动技能触发器 (TypeOfTrigger = 40)
// PDB LF_FIELDLIST 0x6DA23: Size 176, iSkillGroupID int(168)
// IDA ctor: ??0ApplyPassiveSkillTrigger@@QEAA@XZ @ 0x14072EAD0 (Size 176 = 0xB0), iSkillGroupID 0
class ApplyPassiveSkillTrigger : public ActionTrigger {
public:
    std::int32_t iSkillGroupID;  // offset 168

    ApplyPassiveSkillTrigger() : ActionTrigger(), iSkillGroupID(0) {}
};

// MyBuffControlTrigger - 我的Buff控制触发器 (TypeOfTrigger = 41)
// PDB LF_FIELDLIST 0x6E06E: Size 192, cMyBuffControlType char(168) cBuffType char(169)
//   shApplyCount short(170) arBuffID int[5](172)
// IDA ctor: ??0MyBuffControlTrigger@@QEAA@XZ @ 0x14072EB60 (Size 192 = 0xC0),
//   cMyBuffControlType = 1, cBuffType 0, shApplyCount 0, arBuffID memset
class MyBuffControlTrigger : public ActionTrigger {
public:
    char cMyBuffControlType;  // offset 168
    char cBuffType;           // offset 169
    std::int16_t shApplyCount;// offset 170
    std::int32_t arBuffID[5]; // offset 172

    MyBuffControlTrigger() : ActionTrigger(), cMyBuffControlType(1), cBuffType(0), shApplyCount(0) {
        std::memset(arBuffID, 0, sizeof(arBuffID));
    }
};

// CameraShakingTrigger - 镜头震动触发器 (CreateTrigger factory case 0)
// PDB LF_FIELDLIST 0x6D913: Size 192, Frequence float(168) Intencity float(172) RangeMin float(176)
//   RangeMax float(180) iType int(184)
// IDA ctor: ??0CameraShakingTrigger@@QEAA@XZ @ 0x14072AE20 (Size 192 = 0xC0), all zero
class CameraShakingTrigger : public ActionTrigger {
public:
    float Frequence;   // offset 168
    float Intencity;   // offset 172
    float RangeMin;    // offset 176
    float RangeMax;    // offset 180
    std::int32_t iType;// offset 184

    CameraShakingTrigger() : ActionTrigger(), Frequence(0.0f), Intencity(0.0f),
        RangeMin(0.0f), RangeMax(0.0f), iType(0) {}
};

// CameraZoomTrigger - 镜头缩放触发器 (CreateTrigger factory case 0x14)
// PDB LF_FIELDLIST 0x753F7: Size 184, ZoomDepth float(168) ZoomHeight float(172) ZoomEndType int(176)
// IDA ctor: ??0CameraZoomTrigger@@QEAA@XZ @ 0x14072AF70 (Size 184 = 0xB8), all zero
class CameraZoomTrigger : public ActionTrigger {
public:
    float ZoomDepth;    // offset 168
    float ZoomHeight;   // offset 172
    std::int32_t ZoomEndType; // offset 176

    CameraZoomTrigger() : ActionTrigger(), ZoomDepth(0.0f), ZoomHeight(0.0f), ZoomEndType(0) {}
};

// SoundPlayTrigger - 音效播放触发器 (CreateTrigger factory case 1)
// PDB LF_FIELDLIST 0x7899D: Size 856, SoundID int(168) szEventName[128](172) szSoundEventName[128](300)
//   szAttachBone[128](428) fFadeOutTime float(556) ChargeLevel int(560) iSkillLevel int(564)
//   sSkillCondition short(568) CombatType int(572) nLinkTriggerType int(576) bOnlyPlayOne bool(580)
//   szLinkAnimation[128](581) szDivergenceValue[128](709) pszProjectName char*(840)
//   nPromotionLevelLimit int(848) bDontStopWhenHide bool(852)
// IDA ctor: ??0SoundPlayTrigger@@QEAA@XZ @ 0x14072AFD0 (Size 856 = 0x358),
//   SoundID 0, fFadeOutTime 0.0f, ChargeLevel/iSkillLevel/CombatType = -1, rest zero, char arrays memset
class SoundPlayTrigger : public ActionTrigger {
public:
    std::int32_t SoundID;         // offset 168
    char szEventName[128];        // offset 172
    char szSoundEventName[128];   // offset 300
    char szAttachBone[128];       // offset 428
    float fFadeOutTime;           // offset 556
    std::int32_t ChargeLevel;     // offset 560
    std::int32_t iSkillLevel;     // offset 564
    std::int16_t sSkillCondition; // offset 568
    std::int32_t CombatType;      // offset 572
    std::int32_t nLinkTriggerType;// offset 576
    bool bOnlyPlayOne;            // offset 580
    char szLinkAnimation[128];    // offset 581 (packed after bool)
    char szDivergenceValue[128];  // offset 709 (packed after array)
    char* pszProjectName;         // offset 840
    std::int32_t nPromotionLevelLimit; // offset 848
    bool bDontStopWhenHide;       // offset 852

    SoundPlayTrigger() : ActionTrigger(), SoundID(0), fFadeOutTime(0.0f),
        ChargeLevel(-1), iSkillLevel(-1), sSkillCondition(0), CombatType(-1),
        nLinkTriggerType(0), bOnlyPlayOne(false), pszProjectName(nullptr),
        nPromotionLevelLimit(0), bDontStopWhenHide(false) {
        std::memset(szEventName, 0, sizeof(szEventName));
        std::memset(szSoundEventName, 0, sizeof(szSoundEventName));
        std::memset(szAttachBone, 0, sizeof(szAttachBone));
        std::memset(szLinkAnimation, 0, sizeof(szLinkAnimation));
        std::memset(szDivergenceValue, 0, sizeof(szDivergenceValue));
    }
};

// TrajectoryTrigger - 弹道触发器 (CreateTrigger factory case 6)
// PDB LF_FIELDLIST 0x6EC8B: Size 176, LeftHand bool(168)
// IDA ctor: ??0TrajectoryTrigger@@QEAA@XZ @ 0x14072C0E0 (Size 176 = 0xB0), LeftHand 0
class TrajectoryTrigger : public ActionTrigger {
public:
    bool LeftHand;   // offset 168

    TrajectoryTrigger() : ActionTrigger(), LeftHand(false) {}
};

// ScreenBlurTrigger - 屏幕模糊触发器 (CreateTrigger factory case 7)
// PDB LF_FIELDLIST 0x683FB: Size 176, BlurType char(168) Speed float(172)
// IDA ctor: ??0ScreenBlurTrigger@@QEAA@XZ @ 0x14072C170 (Size 176 = 0xB0), BlurType 0, Speed 1.0f
class ScreenBlurTrigger : public ActionTrigger {
public:
    char BlurType;   // offset 168
    float Speed;     // offset 172

    ScreenBlurTrigger() : ActionTrigger(), BlurType(0), Speed(1.0f) {}
};

// AlphaBlendingTrigger - 阿尔法混合触发器 (CreateTrigger factory case 9)
// PDB LF_FIELDLIST 0x69F97: Size 184, StartAlphaValue float(168) EndAlphaValue float(172)
//   OnlyWeapon bool(176) OnlySubWeapon bool(177)
// IDA ctor: ??0AlphaBlendingTrigger@@QEAA@XZ @ 0x14072B980 (Size 184 = 0xB8), all zero
class AlphaBlendingTrigger : public ActionTrigger {
public:
    float StartAlphaValue;   // offset 168
    float EndAlphaValue;     // offset 172
    bool OnlyWeapon;         // offset 176
    bool OnlySubWeapon;      // offset 177

    AlphaBlendingTrigger() : ActionTrigger(), StartAlphaValue(0.0f), EndAlphaValue(0.0f),
        OnlyWeapon(false), OnlySubWeapon(false) {}
};

// CreateEffectTrigger - 创建特效触发器 (CreateTrigger factory case 0xB)
// PDB LF_FIELDLIST 0x76821: Size 864, szCreateParticleFile[128](168) szDestroyParticleFile[128](296)
//   szLinkAnimation[128](424) szAttachableBone[128](552) ParticleOffsetPos hkvVec3(680)
//   ParticleOffsetRot hkvVec3(692) ParentDirection bool(704) ChargeLevel int(708)
//   iSkillLevel int(712) sSkillCondition short(716) CombatType int(720) MinEndTime float(724)
//   bEffectShowAlways bool(728) szDivergenceValue[128](729)
// IDA ctor: ??0CreateEffectTrigger@@QEAA@XZ @ 0x14072BC40 (Size 864 = 0x360),
//   vecs zero, ints 0, MinEndTime 0.0f, bools 0, char arrays memset
class CreateEffectTrigger : public ActionTrigger {
public:
    char szCreateParticleFile[128];   // offset 168
    char szDestroyParticleFile[128];  // offset 296
    char szLinkAnimation[128];        // offset 424
    char szAttachableBone[128];       // offset 552
    hkvVec3 ParticleOffsetPos;        // offset 680
    hkvVec3 ParticleOffsetRot;        // offset 692
    bool ParentDirection;             // offset 704
    std::int32_t ChargeLevel;         // offset 708
    std::int32_t iSkillLevel;         // offset 712
    std::int16_t sSkillCondition;     // offset 716
    std::int32_t CombatType;          // offset 720
    float MinEndTime;                 // offset 724
    bool bEffectShowAlways;           // offset 728
    char szDivergenceValue[128];      // offset 729 (packed after bool)

    CreateEffectTrigger() : ActionTrigger(), ParticleOffsetPos(), ParticleOffsetRot(),
        ParentDirection(false), ChargeLevel(0), iSkillLevel(0), sSkillCondition(0),
        CombatType(0), MinEndTime(0.0f), bEffectShowAlways(false) {
        std::memset(szCreateParticleFile, 0, sizeof(szCreateParticleFile));
        std::memset(szDestroyParticleFile, 0, sizeof(szDestroyParticleFile));
        std::memset(szLinkAnimation, 0, sizeof(szLinkAnimation));
        std::memset(szAttachableBone, 0, sizeof(szAttachableBone));
        std::memset(szDivergenceValue, 0, sizeof(szDivergenceValue));
    }
};

// ShaderChangeTrigger - 着色器变更触发器 (CreateTrigger factory case 0xC)
// PDB LF_FIELDLIST 0x77134: Size 568, szShaderFile[128](168) szTechnicque[128](296)
//   szParamName[128](424) vecParamValue hkvVec4(552)
// IDA ctor: ??0ShaderChangeTrigger@@QEAA@XZ @ 0x14072C250 (Size 568 = 0x238),
//   vecParamValue ZeroVector, char arrays memset
class ShaderChangeTrigger : public ActionTrigger {
public:
    char szShaderFile[128];    // offset 168
    char szTechnicque[128];    // offset 296
    char szParamName[128];     // offset 424
    hkvVec4 vecParamValue;     // offset 552

    ShaderChangeTrigger() : ActionTrigger(), vecParamValue() {
        std::memset(szShaderFile, 0, sizeof(szShaderFile));
        std::memset(szTechnicque, 0, sizeof(szTechnicque));
        std::memset(szParamName, 0, sizeof(szParamName));
    }
};

// MeshAttachmentTrigger - 网格附着触发器 (CreateTrigger factory case 0x13)
// PDB LF_FIELDLIST 0x73E35: Size 600, AttachPos hkvVec3(168) AttachRot hkvVec3(180)
//   AttachScale hkvVec3(192) szBoneName[128](204) szModelFileName[128](332) szAnimName[128](460)
//   AttachToBone bool(588) UseSubWeapon bool(589) ModelID int(592)
// IDA ctor: ??0MeshAttachmentTrigger@@QEAA@XZ @ 0x14072D390 (Size 600 = 0x258),
//   vecs zero, AttachToBone = 1, UseSubWeapon 0, ModelID 0, char arrays memset
class MeshAttachmentTrigger : public ActionTrigger {
public:
    hkvVec3 AttachPos;            // offset 168
    hkvVec3 AttachRot;            // offset 180
    hkvVec3 AttachScale;          // offset 192
    char szBoneName[128];         // offset 204
    char szModelFileName[128];    // offset 332
    char szAnimName[128];         // offset 460
    bool AttachToBone;            // offset 588
    bool UseSubWeapon;            // offset 589
    std::int32_t ModelID;         // offset 592

    MeshAttachmentTrigger() : ActionTrigger(), AttachPos(), AttachRot(), AttachScale(),
        AttachToBone(true), UseSubWeapon(false), ModelID(0) {
        std::memset(szBoneName, 0, sizeof(szBoneName));
        std::memset(szModelFileName, 0, sizeof(szModelFileName));
        std::memset(szAnimName, 0, sizeof(szAnimName));
    }
};

// CharacterCameraLockTrigger - 角色镜头锁定触发器 (CreateTrigger factory case 0x1D)
// PDB LF_FIELDLIST 0x76CE0: Size 168, no own members (base only)
// IDA ctor: ??0CharacterCameraLockTrigger@@QEAA@XZ @ 0x14072B460 (Size 168 = 0xA8)
class CharacterCameraLockTrigger : public ActionTrigger {
public:
    CharacterCameraLockTrigger() : ActionTrigger() {}
};

// AttackJunctionTrigger - 攻击连接触发器 (CreateTrigger factory case 0x1A)
// PDB LF_FIELDLIST 0x4A946: Size 192, nJunctionType int(168) fCheckTime float(172)
//   nNextSkillID int(176) fCheckTime2 float(180) nNextSkillID2 int(184)
// IDA ctor: ??0AttackJunctionTrigger@@QEAA@XZ @ 0x14072DF80 (Size 192 = 0xC0), all zero
class AttackJunctionTrigger : public ActionTrigger {
public:
    std::int32_t nJunctionType;   // offset 168
    float fCheckTime;             // offset 172
    std::int32_t nNextSkillID;    // offset 176
    float fCheckTime2;            // offset 180
    std::int32_t nNextSkillID2;   // offset 184

    AttackJunctionTrigger() : ActionTrigger(), nJunctionType(0), fCheckTime(0.0f),
        nNextSkillID(0), fCheckTime2(0.0f), nNextSkillID2(0) {}
};

// InputFlagTrigger - 输入标记触发器 (CreateTrigger factory case 0x1B)
// PDB LF_FIELDLIST 0x73560: Size 176, nInputFlag int(168) nSkillLevel int(172)
// IDA ctor: ??0InputFlagTrigger@@QEAA@XZ @ 0x14072E0F0 (Size 176 = 0xB0), all zero
class InputFlagTrigger : public ActionTrigger {
public:
    std::int32_t nInputFlag;   // offset 168
    std::int32_t nSkillLevel;  // offset 172

    InputFlagTrigger() : ActionTrigger(), nInputFlag(0), nSkillLevel(0) {}
};

// DeathShaderTrigger - 死亡着色器触发器 (CreateTrigger factory case 0x1C)
// PDB LF_FIELDLIST 0x77BF4: Size 296, szShaderFileName[128](168)
// IDA ctor: ??0DeathShaderTrigger@@QEAA@XZ @ 0x14072E1D0 (Size 296 = 0x128), memset
class DeathShaderTrigger : public ActionTrigger {
public:
    char szShaderFileName[128];   // offset 168

    DeathShaderTrigger() : ActionTrigger() {
        std::memset(szShaderFileName, 0, sizeof(szShaderFileName));
    }
};

// ShaderEffectTrigger - 着色器特效触发器 (CreateTrigger factory case 0x1F)
// PDB LF_FIELDLIST 0x7673E: Size 216, ShaderEffectType short(168) LifeTime float(172)
//   CaptureTime float(176) FrameTime float(180) Strength float(184) Speed float(188)
//   ColorR int(192) ColorG int(196) ColorB int(200) ColorA int(204) SkillLevel int(208)
//   OnlyWeapon bool(212)
// IDA ctor: ??0ShaderEffectTrigger@@QEAA@XZ @ 0x14072E280 (Size 216 = 0xD8),
//   ShaderEffectType 0, floats 0, colors 0, SkillLevel -1, OnlyWeapon 0
class ShaderEffectTrigger : public ActionTrigger {
public:
    std::int16_t ShaderEffectType; // offset 168
    float LifeTime;                // offset 172
    float CaptureTime;             // offset 176
    float FrameTime;               // offset 180
    float Strength;                // offset 184
    float Speed;                   // offset 188
    std::int32_t ColorR;           // offset 192
    std::int32_t ColorG;           // offset 196
    std::int32_t ColorB;           // offset 200
    std::int32_t ColorA;           // offset 204
    std::int32_t SkillLevel;       // offset 208
    bool OnlyWeapon;               // offset 212

    ShaderEffectTrigger() : ActionTrigger(), ShaderEffectType(0), LifeTime(0.0f),
        CaptureTime(0.0f), FrameTime(0.0f), Strength(0.0f), Speed(0.0f),
        ColorR(0), ColorG(0), ColorB(0), ColorA(0), SkillLevel(-1), OnlyWeapon(false) {}
};

// CameraAnimTrigger - 镜头动画触发器 (CreateTrigger factory case 0x20)
// PDB LF_FIELDLIST 0x6D01B: Size 304, szAnimName[128](168) bIsSkill bool(296) nCameraFlag int(300)
// IDA ctor: ??0CameraAnimTrigger@@QEAA@XZ @ 0x14072E4A0 (Size 304 = 0x130),
//   szAnimName memset, bIsSkill 0, nCameraFlag 0
class CameraAnimTrigger : public ActionTrigger {
public:
    char szAnimName[128];   // offset 168
    bool bIsSkill;          // offset 296
    std::int32_t nCameraFlag; // offset 300

    CameraAnimTrigger() : ActionTrigger(), bIsSkill(false), nCameraFlag(0) {
        std::memset(szAnimName, 0, sizeof(szAnimName));
    }
};

// CameraFOVTrigger - 镜头视野触发器 (CreateTrigger factory case 0x22)
// PDB LF_FIELDLIST 0x76EEB: Size 176, BlendingTime float(168) FovValue float(172)
// IDA ctor: ??0CameraFOVTrigger@@QEAA@XZ @ 0x14072BAD0 (Size 176 = 0xB0), all zero
class CameraFOVTrigger : public ActionTrigger {
public:
    float BlendingTime;   // offset 168
    float FovValue;       // offset 172

    CameraFOVTrigger() : ActionTrigger(), BlendingTime(0.0f), FovValue(0.0f) {}
};
