// TraceHPState.cpp - HP追踪状态实现
// IDA 反编译确认

#include "Soulworker/GameServer/XGameServer/TraceHPState.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"

// ============================================================================
// Init IDA 0x140198e60
// 初始化HP追踪状态
// ============================================================================
void CTraceHPState::Init(CMonster* pMonster) {
    // IDA 反编译确认:
    // m_pMonster = _pMonster;
    // m_vecCheckPercent.clear();
    m_pMonster = pMonster;
    m_vecCheckPercent.clear();
}

// ============================================================================
// OnUpdate IDA 0x140197ea0
// 更新HP追踪状态
// ============================================================================
void CTraceHPState::OnUpdate(int nParam1, int nParam2) {
    // IDA 反编译确认: 空函数体
    // 原始逻辑可能已被优化或未实现
}

// ============================================================================
// Update - Update HP trace state (wrapper for OnUpdate)
// ============================================================================
void CTraceHPState::Update() {
    OnUpdate(0, 0);
}

// ============================================================================
// Check - Check HP conditions
// ============================================================================
bool CTraceHPState::Check(float fPercent) {
    if (!m_pMonster) {
        return false;
    }

    // 检查是否在检查百分比列表中
    for (size_t i = 0; i < m_vecCheckPercent.size(); ++i) {
        if (m_vecCheckPercent[i] <= fPercent) {
            return true;
        }
    }

    return false;
}

// ============================================================================
// Reset - Reset state
// ============================================================================
void CTraceHPState::Reset() {
    m_pMonster = nullptr;
    m_vecCheckPercent.clear();
}

// ============================================================================
// IsValid - Check if valid
// ============================================================================
bool CTraceHPState::IsValid() const {
    return m_pMonster != nullptr;
}

// ============================================================================
// GetHP - Get current HP
// ============================================================================
int CTraceHPState::GetHP() const {
    if (!m_pMonster) {
        return 0;
    }
    return m_pMonster->GetHP();
}

// ============================================================================
// GetMaxHP - Get max HP
// ============================================================================
int CTraceHPState::GetMaxHP() const {
    if (!m_pMonster) {
        return 0;
    }
    // TODO: CMonster 需要实现 GetMaxHP 方法
    // 暂时返回当前HP作为最大HP
    return m_pMonster->GetHP();
}

// ============================================================================
// GetPercent - Get HP percent
// ============================================================================
float CTraceHPState::GetPercent() const {
    if (!m_pMonster) {
        return 0.0f;
    }

    int nMaxHP = GetMaxHP();
    if (nMaxHP <= 0) {
        return 0.0f;
    }

    return static_cast<float>(GetHP()) / static_cast<float>(nMaxHP) * 100.0f;
}

// ============================================================================
// SetTarget - Set target HP percent
// ============================================================================
void CTraceHPState::SetTarget(float fPercent) {
    m_vecCheckPercent.push_back(fPercent);
}

// ============================================================================
// IsComplete - Check if complete
// ============================================================================
bool CTraceHPState::IsComplete() const {
    if (!m_pMonster) {
        return false;
    }

    // 如果没有设置检查点，则认为未完成
    if (m_vecCheckPercent.empty()) {
        return false;
    }

    // 检查是否所有检查点都已通过
    float fCurrentPercent = GetPercent();
    for (size_t i = 0; i < m_vecCheckPercent.size(); ++i) {
        if (fCurrentPercent > m_vecCheckPercent[i]) {
            return false;
        }
    }

    return true;
}

// ============================================================================
// GetProgress - Get progress
// ============================================================================
float CTraceHPState::GetProgress() const {
    return GetPercent();
}
