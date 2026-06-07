#pragma once

#include <cstdint>
#include <vector>

// 前置声明
class CMonster;

// ============================================================================
// CTraceHPState - HP追踪状态类
// IDA 确认大小: 56 bytes (Monster.h中m_xTraceHPState_dummy[7])
//
// 功能: 追踪怪物HP变化，用于Boss血条显示、阶段转换等
// ============================================================================
class CTraceHPState {
public:
    // === 构造函数 ===
    // IDA: ??0CTraceHPState@@QEAA@XZ @ 0x140198E00
    CTraceHPState() : m_byType(0), m_nPreHP(-1), m_pMonster(nullptr) {}

    // === 析构函数 ===
    // IDA: ??1CTraceHPState@@QEAA@XZ @ 0x140198E40
    ~CTraceHPState() = default;

    // === IDA 反编译确认的方法 ===

    // Init - 初始化HP追踪状态，绑定所属怪物
    // IDA: ?Init@CTraceHPState@@QEAAXPEAVCMonster@@@Z @ 0x140198E60
    void Init(CMonster* pMonster);

    // OnUpdate - 更新HP追踪状态
    // IDA: ?OnUpdate@CTraceHPState@@QEAAXHH@Z @ 0x140197EA0
    void OnUpdate(int nParam1, int nParam2);

    // Clear - 清除追踪状态
    void Clear() {
        m_pMonster = nullptr;
        m_vecCheckPercent.clear();
    }

    // === 辅助函数 (Round 6 Phase 5) ===

    // Update - Update HP trace state
    void Update();

    // Check - Check HP conditions
    bool Check(float fPercent);

    // Reset - Reset state
    void Reset();

    // IsValid - Check if valid
    bool IsValid() const;

    // GetHP - Get current HP
    int GetHP() const;

    // GetMaxHP - Get max HP
    int GetMaxHP() const;

    // GetPercent - Get HP percent
    float GetPercent() const;

    // SetTarget - Set target HP percent
    void SetTarget(float fPercent);

    // IsComplete - Check if complete
    bool IsComplete() const;

    // GetProgress - Get progress
    float GetProgress() const;

    // IDA: ?SetCheckPercent@CTraceHPState@@QEAAXH@Z (0x140364730)
    void SetCheckPercent(int nPercent) { m_vecCheckPercent.push_back(static_cast<float>(nPercent)); }

    // IDA: ?SetType@CTraceHPState@@QEAAXE@Z (0x140364780)
    void SetType(std::uint8_t byType) { m_byType = byType; }

    // === 成员访问器 ===
    CMonster* GetMonster() const { return m_pMonster; }
    void SetMonster(CMonster* pMonster) { m_pMonster = pMonster; }

    std::vector<float>& GetCheckPercent() { return m_vecCheckPercent; }
    const std::vector<float>& GetCheckPercent() const { return m_vecCheckPercent; }

private:
    // === IDA 确认的成员变量 ===
    // offset 0: m_pMonster (CMonster*)
    CMonster* m_pMonster;

    // offset 8: m_vecCheckPercent (std::vector<float>)
    // IDA: std::vector<int> m_vecCheckPercent
    std::vector<float> m_vecCheckPercent;

    // offset 24: m_byType (std::uint8_t) - 追踪类型 (1=HP百分比追踪)
    std::uint8_t m_byType = 0;

    // offset 28: m_nPreHP (int) - 上一次HP值
    int m_nPreHP = 0;

    // Total: 约56 bytes (包含vector内部结构)
};
