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
