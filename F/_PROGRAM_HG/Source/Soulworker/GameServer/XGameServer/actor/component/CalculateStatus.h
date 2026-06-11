#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include <functional>

// Forward declarations
class CGocAttribute;

/**
 * @brief CCalculateStatus - 角色状态计算器
 *
 * IDA: CCalculateStatus 单例类，负责计算角色的各种属性值
 * - 构造函数: 0x140038D60
 * - CalculateStatusAll: 0x140038E60
 * - CalculateStatus: 0x140038EB0
 * - Init: 0x1402D3BC0
 * - InitEffect: 0x1402D4CB0
 *
 * 该类维护 77 个状态处理器（索引 0-76），每个处理器包含：
 * - 状态类型 (nStat)
 * - 计算函数指针
 */
class CCalculateStatus {
public:
    // 状态处理器信息结构
    struct STATUS_HANDLER_INFO {
        int nStat;                                      // 状态类型
        float (__fastcall *pFunc)(CCalculateStatus*, CGocAttribute*);  // 计算函数指针
        char szName[16];                                // 状态名称（用于调试）
    };

    // 状态类型枚举（部分）
    enum E_STAT_TYPE {
        STAT_STR = 0,           // 力量
        STAT_AGI = 1,           // 敏捷
        STAT_INT = 2,           // 智力
        STAT_BAL = 3,           // 平衡
        STAT_VIT = 4,           // 体力
        STAT_LUC = 5,           // 幸运
        STAT_HP_MAX = 6,        // 最大HP
        STAT_SG_MAX = 7,        // 最大SG
        STAT_ST_MAX = 8,        // 最大ST
        STAT_ST_REG = 9,        // ST恢复
        STAT_SV_MAX = 10,       // 最大SV
        // ... 其他状态类型 (共 77 个)
        STAT_MAX = 77
    };

    // 构造函数
    // IDA: 0x140038D60
    CCalculateStatus();

    // 析构函数
    ~CCalculateStatus() = default;

    // 初始化
    // IDA: 0x1402D3BC0
    void Init();

    // 初始化效果
    // IDA: 0x1402D4CB0
    void InitEffect();

    // 计算所有状态
    // IDA: 0x140038E60
    void CalculateStatusAll(CGocAttribute* pAttribute);

    // 计算单个状态
    // IDA: 0x140038EB0
    void CalculateStatus(int nStat, CGocAttribute* pAttribute);

    // 从效果获取状态
    // IDA: 0x140038DD0
    void GetStatFromEffect(int nStatType, int& nResultStatType, bool& bAdd);

    // === 状态计算函数（部分示例）===

    // 基础属性计算
    // IDA: 0x1402D6BF0
    float CALCULATE_STAT_STR(CGocAttribute* pAttribute);
    float CALCULATE_STAT_AGI(CGocAttribute* pAttribute);
    float CALCULATE_STAT_INT(CGocAttribute* pAttribute);
    float CALCULATE_STAT_BAL(CGocAttribute* pAttribute);
    float CALCULATE_STAT_VIT(CGocAttribute* pAttribute);
    float CALCULATE_STAT_LUC(CGocAttribute* pAttribute);

    // HP/SG/ST 计算
    // IDA: 0x1402D6DD0
    float CALCULATE_STAT_HP_MAX(CGocAttribute* pAttribute);
    float CALCULATE_STAT_SG_MAX(CGocAttribute* pAttribute);
    float CALCULATE_STAT_ST_MAX(CGocAttribute* pAttribute);
    float CALCULATE_STAT_ST_REG(CGocAttribute* pAttribute);
    float CALCULATE_STAT_SV_MAX(CGocAttribute* pAttribute);

    // ... 其他状态计算函数（共 70+ 个）

private:
    // 状态处理器数组（77 个）
    std::vector<STATUS_HANDLER_INFO> m_vecStatusFunc[STAT_MAX];

    // 状态效果映射
    std::map<int, struct ST_STATUS_EFFECT_INFO> m_mapStatusEffect;
};

// 单例访问
// IDA: 0x140045540
CCalculateStatus* CCalculateStatus_Instance();
