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
    CTraceHPState() : m_pMonster(nullptr) {}

    // === 析构函数 ===
    ~CTraceHPState() = default;

    // === IDA 反编译确认的方法 ===

    // Init - 初始化HP追踪状态，绑定所属怪物
    // IDA: ?Init@CTraceHPState@@QEAAXPEAVCMonster@@@Z @ 0x140198E60
    void Init(CMonster* pMonster);

    // Clear - 清除追踪状态
    void Clear() {
        m_pMonster = nullptr;
        m_vecCheckPercent.clear();
    }

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
    std::vector<float> m_vecCheckPercent;

    // Total: 约56 bytes (包含vector内部结构)
};
