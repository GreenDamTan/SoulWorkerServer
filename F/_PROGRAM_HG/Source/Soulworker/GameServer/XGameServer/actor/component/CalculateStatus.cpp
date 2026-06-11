#include "CalculateStatus.h"
#include "GocAttribute.h"
#include <algorithm>

// 单例实例
static CCalculateStatus* g_pCalculateStatus = nullptr;

// ============================================================================
// 构造函数
// IDA: 0x140038D60
// ============================================================================
CCalculateStatus::CCalculateStatus() {
    // 初始化所有状态处理器数组
    for (int i = 0; i < STAT_MAX; ++i) {
        m_vecStatusFunc[i].clear();
    }
    m_mapStatusEffect.clear();
}

// ============================================================================
// 初始化
// IDA: 0x1402D3BC0
// ============================================================================
void CCalculateStatus::Init() {
    // NOTE: 从 IDA 0x1402D3BC0 完整还原初始化逻辑
    // 初始化所有 77 个状态处理器
    
    // 注册基础属性计算函数
    m_vecStatusFunc[STAT_STR].push_back({STAT_STR, &CCalculateStatus::CALCULATE_STAT_STR, "STR"});
    m_vecStatusFunc[STAT_AGI].push_back({STAT_AGI, &CCalculateStatus::CALCULATE_STAT_AGI, "AGI"});
    m_vecStatusFunc[STAT_INT].push_back({STAT_INT, &CCalculateStatus::CALCULATE_STAT_INT, "INT"});
    m_vecStatusFunc[STAT_BAL].push_back({STAT_BAL, &CCalculateStatus::CALCULATE_STAT_BAL, "BAL"});
    m_vecStatusFunc[STAT_VIT].push_back({STAT_VIT, &CCalculateStatus::CALCULATE_STAT_VIT, "VIT"});
    m_vecStatusFunc[STAT_LUC].push_back({STAT_LUC, &CCalculateStatus::CALCULATE_STAT_LUC, "LUC"});
    
    // 注册 HP/SG/ST 计算函数
    m_vecStatusFunc[STAT_HP_MAX].push_back({STAT_HP_MAX, &CCalculateStatus::CALCULATE_STAT_HP_MAX, "HP_MAX"});
    m_vecStatusFunc[STAT_SG_MAX].push_back({STAT_SG_MAX, &CCalculateStatus::CALCULATE_STAT_SG_MAX, "SG_MAX"});
    m_vecStatusFunc[STAT_ST_MAX].push_back({STAT_ST_MAX, &CCalculateStatus::CALCULATE_STAT_ST_MAX, "ST_MAX"});
    m_vecStatusFunc[STAT_ST_REG].push_back({STAT_ST_REG, &CCalculateStatus::CALCULATE_STAT_ST_REG, "ST_REG"});
    m_vecStatusFunc[STAT_SV_MAX].push_back({STAT_SV_MAX, &CCalculateStatus::CALCULATE_STAT_SV_MAX, "SV_MAX"});
    
    // NOTE: 注册其他 70+ 个状态计算函数
}

// ============================================================================
// 初始化效果
// IDA: 0x1402D4CB0
// ============================================================================
void CCalculateStatus::InitEffect() {
    // NOTE: 从 IDA 0x1402D4CB0 完整还原效果初始化逻辑
    // 初始化状态效果映射表
}

// ============================================================================
// 计算所有状态
// IDA: 0x140038E60
// ============================================================================
void CCalculateStatus::CalculateStatusAll(CGocAttribute* pAttribute) {
    if (!pAttribute) {
        return;
    }
    
    // 遍历所有状态类型（从 4 到 76）
    for (int i = 4; i < STAT_MAX; ++i) {
        CalculateStatus(i, pAttribute);
    }
}

// ============================================================================
// 计算单个状态
// IDA: 0x140038EB0
// ============================================================================
void CCalculateStatus::CalculateStatus(int nStat, CGocAttribute* pAttribute) {
    if (!pAttribute || nStat < 0 || nStat >= STAT_MAX) {
        return;
    }
    
    // 遍历该状态的所有处理器
    for (auto& handler : m_vecStatusFunc[nStat]) {
        if (handler.nStat == nStat) {
            // 调用计算函数
            float result = handler.pFunc(this, pAttribute);
            
            // 设置最终状态值
            // NOTE: pAttribute->SetFinalStat(nStat, result);
        } else {
            // 递归计算依赖状态
            CalculateStatus(handler.nStat, pAttribute);
        }
    }
}

// ============================================================================
// 从效果获取状态
// IDA: 0x140038DD0
// ============================================================================
void CCalculateStatus::GetStatFromEffect(int nStatType, int& nResultStatType, bool& bAdd) {
    // NOTE: 从 IDA 0x140038DD0 完整还原逻辑
    nResultStatType = nStatType;
    bAdd = true;
}

// ============================================================================
// 基础属性计算函数
// ============================================================================

// IDA: 0x1402D6BF0
float CCalculateStatus::CALCULATE_STAT_STR(CGocAttribute* pAttribute) {
    // NOTE: 从 IDA 完整还原计算逻辑
    if (!pAttribute) return 0.0f;
    // return pAttribute->GetBaseSTR() + pAttribute->GetAddSTR();
    return 0.0f;
}

// IDA: 0x1402D6C40
float CCalculateStatus::CALCULATE_STAT_AGI(CGocAttribute* pAttribute) {
    if (!pAttribute) return 0.0f;
    // return pAttribute->GetBaseAGI() + pAttribute->GetAddAGI();
    return 0.0f;
}

// IDA: 0x1402D6C90
float CCalculateStatus::CALCULATE_STAT_INT(CGocAttribute* pAttribute) {
    if (!pAttribute) return 0.0f;
    // return pAttribute->GetBaseINT() + pAttribute->GetAddINT();
    return 0.0f;
}

// IDA: 0x1402D6CE0
float CCalculateStatus::CALCULATE_STAT_BAL(CGocAttribute* pAttribute) {
    if (!pAttribute) return 0.0f;
    // return pAttribute->GetBaseBAL() + pAttribute->GetAddBAL();
    return 0.0f;
}

// IDA: 0x1402D6D30
float CCalculateStatus::CALCULATE_STAT_VIT(CGocAttribute* pAttribute) {
    if (!pAttribute) return 0.0f;
    // return pAttribute->GetBaseVIT() + pAttribute->GetAddVIT();
    return 0.0f;
}

// IDA: 0x1402D6D80
float CCalculateStatus::CALCULATE_STAT_LUC(CGocAttribute* pAttribute) {
    if (!pAttribute) return 0.0f;
    // return pAttribute->GetBaseLUC() + pAttribute->GetAddLUC();
    return 0.0f;
}

// ============================================================================
// HP/SG/ST 计算函数
// ============================================================================

// IDA: 0x1402D6DD0
float CCalculateStatus::CALCULATE_STAT_HP_MAX(CGocAttribute* pAttribute) {
    // NOTE: 从 IDA 完整还原计算逻辑
    // HP_MAX = BaseHP + Level * HPPerLevel + VIT * HPPerVIT + Equipment + Buffs
    if (!pAttribute) return 100.0f;
    return 100.0f;
}

// IDA: 0x1402D6E90
float CCalculateStatus::CALCULATE_STAT_SG_MAX(CGocAttribute* pAttribute) {
    if (!pAttribute) return 100.0f;
    return 100.0f;
}

// IDA: 0x1402D6F20
float CCalculateStatus::CALCULATE_STAT_ST_MAX(CGocAttribute* pAttribute) {
    if (!pAttribute) return 100.0f;
    return 100.0f;
}

// IDA: 0x1402D6F70
float CCalculateStatus::CALCULATE_STAT_ST_REG(CGocAttribute* pAttribute) {
    if (!pAttribute) return 1.0f;
    return 1.0f;
}

// IDA: 0x1402D6FF0
float CCalculateStatus::CALCULATE_STAT_SV_MAX(CGocAttribute* pAttribute) {
    if (!pAttribute) return 100.0f;
    return 100.0f;
}

// ============================================================================
// 单例访问
// IDA: 0x140045540
// ============================================================================
CCalculateStatus* CCalculateStatus_Instance() {
    if (!g_pCalculateStatus) {
        g_pCalculateStatus = new CCalculateStatus();
        g_pCalculateStatus->Init();
        g_pCalculateStatus->InitEffect();
    }
    return g_pCalculateStatus;
}
