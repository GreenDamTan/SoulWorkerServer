#pragma once

#include <cstdint>

/**
 * @brief tagBUFF_STATE - Buff状态结构
 *
 * 表示单个Buff实例的运行时状态。
 * IDA 反编译来源: GameServer.exe
 *
 * Clear 函数地址: 0x140353870
 * 数组大小: 50 个元素 (CMover::m_stBuffState[50])
 */
struct tagBUFF_STATE {
    // ============================================================================
    // 成员变量 (来自 IDA 反编译)
    // ============================================================================

    // nBuffIndex - Buff索引 (Buff表中定义)
    std::uint16_t nBuffIndex;

    // nGroupID - Buff组ID
    std::uint16_t nGroupID;

    // dwID - 施加者ID (攻击者/施法者)
    std::uint32_t dwID;

    // fLifeTime - 剩余存活时间 (秒)
    float fLifeTime;

    // fGapTime - 间隔时间 (用于DoT/HoT)
    float fGapTime;

    // fGapTimeCheck - 间隔时间检查计数器
    float fGapTimeCheck;

    // byGapApplyCount - 间隔应用次数
    std::uint8_t byGapApplyCount;

    // byEffectType - 效果类型 (10 = 特殊类型需要匹配dwID)
    std::uint8_t byEffectType;

    // byStatType - 属性类型数组 (最多3个)
    std::uint8_t byStatType[3];

    // fSkillVal - 技能数值数组 (最多3个)
    float fSkillVal[3];

    // nRandomVal - 随机值
    std::int32_t nRandomVal;

    // byCount - 计数 (默认1)
    std::uint8_t byCount;

    // byDotCount - DoT计数
    std::uint8_t byDotCount;

    // byBuffLV - Buff等级 (默认1)
    std::uint8_t byBuffLV;

    // byBuffType - Buff类型
    std::uint8_t byBuffType;

    // nCreatedEffectID - 创建的效果ID数组 (最多3个，默认-1)
    std::int32_t nCreatedEffectID[3];

    // pEffectSound - 效果声音指针
    void* pEffectSound;

    // bySendType - 发送类型
    std::uint8_t bySendType;

    // iSkillVal - 整数技能值
    std::int32_t iSkillVal;

    // bShow - 是否显示 (默认true)
    bool bShow;

    // bySystemType - 系统类型
    std::uint8_t bySystemType;

    // ============================================================================
    // 方法
    // ============================================================================

    /**
     * @brief Clear - 清空Buff状态
     * IDA 0x140353870: ?Clear@tagBUFF_STATE@@QEAAXXZ
     */
    void Clear() {
        nBuffIndex = 0;
        nGroupID = 0;
        dwID = static_cast<std::uint32_t>(-1);
        fLifeTime = 0.0f;
        fGapTime = 0.0f;
        fGapTimeCheck = 0.0f;
        byGapApplyCount = 0;
        byEffectType = 0;
        byStatType[0] = 0;
        byStatType[1] = 0;
        byStatType[2] = 0;
        fSkillVal[0] = 0.0f;
        fSkillVal[1] = 0.0f;
        fSkillVal[2] = 0.0f;
        nRandomVal = 0;
        byCount = 1;
        byDotCount = 0;
        byBuffLV = 1;
        byBuffType = 0;
        nCreatedEffectID[0] = -1;
        nCreatedEffectID[1] = -1;
        nCreatedEffectID[2] = -1;
        pEffectSound = nullptr;
        bySendType = 0;
        iSkillVal = 0;
        bShow = true;
        bySystemType = 0;
    }

    /**
     * @brief IsLife - 检查是否存活
     * @return 如果nBuffIndex > 0则返回true
     */
    bool IsLife() const {
        return nBuffIndex > 0;
    }

    /**
     * @brief 默认构造函数
     */
    tagBUFF_STATE() {
        Clear();
    }
};

// 编译时检查大小 (确保布局正确)
// 实际大小需要从 IDA 确认
// static_assert(sizeof(tagBUFF_STATE) == XX, "tagBUFF_STATE size mismatch");
