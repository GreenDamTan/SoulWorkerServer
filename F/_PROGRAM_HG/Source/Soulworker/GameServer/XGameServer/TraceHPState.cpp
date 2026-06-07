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
// OnUpdate IDA 0x140198EA0
// 更新HP追踪状态 - 检查百分比并触发脚本事件
// ============================================================================
void CTraceHPState::OnUpdate(int nCurHP, int nMaxHP) {
    // IDA 反编译精确还原:
    // void __fastcall CTraceHPState::OnUpdate(CTraceHPState *this, int nCurHP, int nMaxHP)
    // {
    //   if ( std::vector<PS_ITEM_SLOT_INFO>::size(&this->m_vecCheckPercent)
    //     && this->m_byType == 1
    //     && this->m_nPreHP != nCurHP )
    //   {
    //     this->m_nPreHP = nCurHP;
    //     nPercent = (int)((float)nCurHP / (float)nMaxHP * 100.0);
    //     // 遍历检查百分比列表
    //     for (auto it = m_vecCheckPercent.begin(); it != m_vecCheckPercent.end(); ++it) {
    //       nCheckPercent = *it;
    //       if (nPercent >= nCheckPercent) {
    //         ++it;  // 跳过已达到的百分比
    //         continue;
    //       }
    //       // 获取 Maze 区域
    //       pMaze = dynamic_cast<XMaze*>(m_pMonster->GetArea());
    //       if (!pMaze) {
    //         // 日志输出
    //         return;
    //       }
    //       // 执行脚本函数 OnMonsterProcessHP
    //       pScriptInst = pMaze->GetScriptInstance();
    //       if (!pScriptInst) return;
    //       // 构建怪物ID字符串
    //       // 执行脚本
    //       // 发送广播包 (main=0x11, sub=0x58)
    //       XSendPacket xSendPacket(0x11, 0x58);
    //       xSendPacket << m_pMonster->GetTableID();
    //       xSendPacket << nPercent;
    //       pMaze->SendBroadCast(&xSendPacket, nullptr, eAll);
    //       // 从列表中移除该百分比
    //       m_vecCheckPercent.erase(it);
    //     }
    //   }
    // }

    if (m_vecCheckPercent.empty() || m_byType != 1 || m_nPreHP == nCurHP)
        return;

    m_nPreHP = nCurHP;
    int nPercent = static_cast<int>((static_cast<float>(nCurHP) / static_cast<float>(nMaxHP)) * 100.0f);

    // TODO: 需要完整的 XMaze 和脚本系统支持
    // 遍历检查百分比列表
    for (auto it = m_vecCheckPercent.begin(); it != m_vecCheckPercent.end(); ) {
        int nCheckPercent = *it;
        if (nPercent >= nCheckPercent) {
            ++it;
            continue;
        }

        // 获取 Maze 区域并执行脚本
        // XArea* pArea = m_pMonster->GetArea();
        // XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
        // if (!pMaze) continue;

        // 发送广播包通知 HP 百分比变化
        // XSendPacket xSendPacket(0x11, 0x58);
        // xSendPacket << m_pMonster->GetTableID();
        // xSendPacket << nPercent;
        // pMaze->SendBroadCast(&xSendPacket, nullptr, eAll);

        // 从列表中移除
        it = m_vecCheckPercent.erase(it);
    }
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

    // TODO: TB_MONSTER 需要完整定义才能访问字段
    // Per IDA: Get max HP from monster table
    // TB_MONSTER* pMobTable = m_pMonster->GetMobTableRef();
    // if (pMobTable) {
    //     return pMobTable->HP_Max_INT_Value;
    // }

    // Fallback: return current HP if table not available
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
