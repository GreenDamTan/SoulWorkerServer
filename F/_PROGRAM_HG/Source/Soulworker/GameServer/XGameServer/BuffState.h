#pragma once

#include <cstdint>

/**
 * @brief tagBUFF_STATE - Buff状态结构
 *
 * 表示单个Buff实例的运行时状态。
 * IDA 反编译来源: GameServer.exe
 *
 * 结构体大小: 70 bytes (IDA验证)
 * Clear 函数地址: 0x140353870
 * 构造函数地址: 0x1403539A0
 * 数组大小: 50 个元素 (CMover::m_stBuffState[50])
 *
 * 注意: 此结构体使用紧凑对齐 (无填充)，与原二进制布局完全匹配
 */

#pragma pack(push, 1)
struct tagBUFF_STATE {
    // ============================================================================
    // 成员变量 (来自 IDA get_struct_info, 大小70字节)
    // ============================================================================

    // offset 0, size 2
    std::uint16_t nBuffIndex;       // Buff索引 (Buff表中定义)

    // offset 2, size 2
    std::uint16_t nGroupID;         // Buff组ID

    // offset 4, size 4
    std::uint32_t dwID;             // 施加者ID (攻击者/施法者)

    // offset 8, size 4
    float fLifeTime;                // 剩余存活时间 (秒)

    // offset 12, size 4
    float fGapTime;                 // 间隔时间 (用于DoT/HoT)

    // offset 16, size 4
    float fGapTimeCheck;            // 间隔时间检查计数器

    // offset 20, size 1
    std::uint8_t byGapApplyCount;  // 间隔应用次数

    // offset 21, size 1
    std::uint8_t byEffectType;     // 效果类型 (10 = 特殊类型需要匹配dwID)

    // offset 22, size 3
    std::uint8_t byStatType[3];    // 属性类型数组 (最多3个)

    // offset 25, size 12 (3 * 4)
    float fSkillVal[3];            // 技能数值数组 (最多3个)

    // offset 37, size 2 - 注意: IDA显示为__int16而非int32
    std::int16_t nRandomVal;       // 随机值

    // offset 39, size 1
    std::uint8_t byCount;          // 计数 (默认1)

    // offset 40, size 1
    std::uint8_t byDotCount;       // DoT计数

    // offset 41, size 1
    std::uint8_t byBuffLV;         // Buff等级 (默认1)

    // offset 42, size 1
    std::uint8_t byBuffType;       // Buff类型

    // offset 43, size 12 (3 * 4) - 注意: IDA显示为unsigned int[3]
    std::uint32_t nCreatedEffectID[3]; // 创建的效果ID数组 (最多3个，默认-1)

    // offset 55, size 8
    void* pEffectSound;            // 效果声音指针 (VisObject3D_cl*)

    // offset 63, size 1
    std::uint8_t bySendType;       // 发送类型

    // offset 64, size 4
    std::int32_t iSkillVal;        // 整数技能值

    // offset 68, size 1
    bool bShow;                    // 是否显示 (默认true)

    // offset 69, size 1
    std::uint8_t bySystemType;     // 系统类型

    // 总大小: 70 bytes

    // ============================================================================
    // 方法
    // ============================================================================

    /**
     * @brief Clear - 清空Buff状态
     * IDA 0x140353870: ?Clear@tagBUFF_STATE@@QEAAXXZ
     * 验证: 与IDA反编译结果完全匹配
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
        nCreatedEffectID[0] = static_cast<std::uint32_t>(-1);
        nCreatedEffectID[1] = static_cast<std::uint32_t>(-1);
        nCreatedEffectID[2] = static_cast<std::uint32_t>(-1);
        pEffectSound = nullptr;
        bySendType = 0;
        iSkillVal = 0;
        bShow = true;
        bySystemType = 0;
    }

    /**
     * @brief IsLife - 检查是否存活
     * @return 如果nBuffIndex > 0则返回true
     * 注意: 此函数在IDA中未发现独立实现，推测为内联函数
     */
    bool IsLife() const {
        return nBuffIndex > 0;
    }

    /**
     * @brief 默认构造函数
     * IDA 0x1403539A0: ??0tagBUFF_STATE@@QEAA@XZ
     * 验证: 与IDA反编译结果完全匹配 (调用Clear)
     */
    tagBUFF_STATE() {
        Clear();
    }
};
#pragma pack(pop)

// 编译时检查大小 (确保布局正确)
// IDA验证: sizeof(tagBUFF_STATE) == 70
static_assert(sizeof(tagBUFF_STATE) == 70, "tagBUFF_STATE size mismatch - expected 70 bytes from IDA");
