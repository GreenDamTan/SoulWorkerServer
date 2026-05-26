#pragma once

#include <cstdint>

// 前置声明
class CMonster;

// ============================================================================
// CGroupAggro - 群体仇恨管理类
// IDA 确认大小: 32 bytes
// ============================================================================
class CGroupAggro {
public:
    CGroupAggro()
        : m_bIsAggro(false)
        , m_pMonster(nullptr)
        , m_nGroupID(0)
        , m_nDistance(0)
        , m_nMaxCount(0)
    {}

    ~CGroupAggro() = default;

    // 成员访问
    bool IsAggro() const { return m_bIsAggro; }
    void SetAggro(bool bAggro) { m_bIsAggro = bAggro; }

    CMonster* GetMonster() const { return m_pMonster; }
    void SetMonster(CMonster* pMonster) { m_pMonster = pMonster; }

    int GetGroupID() const { return m_nGroupID; }
    void SetGroupID(int nGroupID) { m_nGroupID = nGroupID; }

    int GetDistance() const { return m_nDistance; }
    void SetDistance(int nDistance) { m_nDistance = nDistance; }

    int GetMaxCount() const { return m_nMaxCount; }
    void SetMaxCount(int nMaxCount) { m_nMaxCount = nMaxCount; }

private:
    // === IDA 确认的成员变量 ===
    // offset 0: m_bIsAggro (bool)
    bool m_bIsAggro;
    // offset 1-7: padding
    char _pad0[7];

    // offset 8: m_pMonster (CMonster*)
    CMonster* m_pMonster;

    // offset 16: m_nGroupID (int)
    int m_nGroupID;

    // offset 20: m_nDistance (int)
    int m_nDistance;

    // offset 24: m_nMaxCount (int)
    int m_nMaxCount;

    // offset 28-31: padding
    char _pad1[4];

    // Total: 32 bytes
};
