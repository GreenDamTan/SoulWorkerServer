#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerItemMake.h - 物品制作限制相关结构体及序列化运算符
//
// 此文件包含物品制作限制的所有数据结构和序列化操作：
// - PS_ITEM_MAKE_LIMIT_INFO - 物品制作限制信息
// - PS_ITEM_MAKE_LIMIT_LIST - 物品制作限制列表
// - PS_DB_ITEM_MAKE_LIMIT_INFO - DB层物品制作限制信息
// - PS_DB_ITEM_MAKE_LIMIT_UPDATE - DB层物品制作限制更新请求
// - PS_DB_ITEM_MAKE_LIMIT_INIT - DB层物品制作限制初始化请求
// ============================================================================

// ============================================================================
// 物品制作限制结构体
// ============================================================================

/**
 * @brief 物品制作限制信息 - 24 bytes
 * 来自 IDA: PS_ITEM_MAKE_LIMIT_INFO
 */
struct PS_ITEM_MAKE_LIMIT_INFO {
    std::int32_t nMakeIndex = 0;    // +0x00: 制作索引
    std::int32_t nItemID = 0;       // +0x04: 物品ID
    std::int16_t shCount = 0;       // +0x08: 数量
    std::uint8_t _pad0[6] = {};     // +0x0A: padding (6 bytes)
    std::int64_t biEndDate = 0;     // +0x10: 结束日期
};

/**
 * @brief 物品制作限制列表 - 40 bytes
 * 来自 IDA: PS_ITEM_MAKE_LIMIT_LIST
 */
struct PS_ITEM_MAKE_LIMIT_LIST {
    std::uint8_t byFlag = 0;                                // +0x00: 标志
    std::uint8_t _pad0[7] = {};                             // +0x01: padding (7 bytes)
    std::vector<PS_ITEM_MAKE_LIMIT_INFO> vecInfo;           // +0x08: 列表向量
};

/**
 * @brief DB层物品制作限制信息 - 32 bytes
 * 来自 IDA: PS_DB_ITEM_MAKE_LIMIT_INFO
 */
struct PS_DB_ITEM_MAKE_LIMIT_INFO {
    std::uint8_t byLimitType = 0;                           // +0x00: 限制类型
    std::uint8_t _pad0[7] = {};                             // +0x01: padding (7 bytes)
    PS_ITEM_MAKE_LIMIT_INFO psInfo{};                       // +0x08: 制作限制信息
};

/**
 * @brief DB层物品制作限制更新请求 - 40 bytes
 * 来自 IDA: PS_DB_ITEM_MAKE_LIMIT_UPDATE
 */
struct PS_DB_ITEM_MAKE_LIMIT_UPDATE {
    std::uint32_t dwUCID = 0;                               // +0x00: 角色ID
    std::uint32_t dwUAID = 0;                               // +0x04: 账户ID
    PS_DB_ITEM_MAKE_LIMIT_INFO psDBInfo{};                  // +0x08: DB限制信息
};

/**
 * @brief DB层物品制作限制初始化请求 - 40 bytes
 * 来自 IDA: PS_DB_ITEM_MAKE_LIMIT_INIT
 */
struct PS_DB_ITEM_MAKE_LIMIT_INIT {
    std::uint32_t dwUAID = 0;                               // +0x00: 账户ID
    std::uint32_t dwUCID = 0;                               // +0x04: 角色ID
    std::vector<PS_DB_ITEM_MAKE_LIMIT_INFO> vecInfo;        // +0x08: 列表向量
};

// ============================================================================
// Static Assertions - 大小和偏移验证
// ============================================================================

static_assert(sizeof(PS_ITEM_MAKE_LIMIT_INFO) == 24, "PS_ITEM_MAKE_LIMIT_INFO size must match IDA");
static_assert(sizeof(PS_ITEM_MAKE_LIMIT_LIST) == 40, "PS_ITEM_MAKE_LIMIT_LIST size must match IDA");
static_assert(sizeof(PS_DB_ITEM_MAKE_LIMIT_INFO) == 32, "PS_DB_ITEM_MAKE_LIMIT_INFO size must match IDA");
static_assert(sizeof(PS_DB_ITEM_MAKE_LIMIT_UPDATE) == 40, "PS_DB_ITEM_MAKE_LIMIT_UPDATE size must match IDA");
static_assert(sizeof(PS_DB_ITEM_MAKE_LIMIT_INIT) == 40, "PS_DB_ITEM_MAKE_LIMIT_INIT size must match IDA");

// ============================================================================
// 物品制作限制序列化运算符
// ============================================================================

// PS_ITEM_MAKE_LIMIT_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_ITEM_MAKE_LIMIT_INFO& value) {
    packet.XParse << value.nMakeIndex;
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << static_cast<std::int32_t>(0);  // padding
    packet.XParse << value.biEndDate;
    return packet;
}

// PS_ITEM_MAKE_LIMIT_INFO 反序列化
inline XPacket& operator>>(XPacket& packet, PS_ITEM_MAKE_LIMIT_INFO& value) {
    packet.XParse >> value.nMakeIndex;
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse.GetDWORD();  // skip padding
    packet.XParse >> value.biEndDate;
    return packet;
}

// PS_ITEM_MAKE_LIMIT_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_ITEM_MAKE_LIMIT_LIST& value) {
    packet.XParse << value.byFlag;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::int32_t>(0);  // padding
    const std::uint32_t count = static_cast<std::uint32_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

// PS_ITEM_MAKE_LIMIT_LIST 反序列化
inline XPacket& operator>>(XPacket& packet, PS_ITEM_MAKE_LIMIT_LIST& value) {
    packet.XParse >> value.byFlag;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetDWORD();  // skip padding
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        PS_ITEM_MAKE_LIMIT_INFO info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
    return packet;
}

// PS_DB_ITEM_MAKE_LIMIT_INFO 反序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_ITEM_MAKE_LIMIT_INFO& value) {
    packet.XParse >> value.byLimitType;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetDWORD();  // skip padding
    packet >> value.psInfo;
    return packet;
}

// PS_DB_ITEM_MAKE_LIMIT_INIT 反序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_ITEM_MAKE_LIMIT_INIT& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        PS_DB_ITEM_MAKE_LIMIT_INFO info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
    return packet;
}

// PS_DB_ITEM_MAKE_LIMIT_INIT 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_MAKE_LIMIT_INIT& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    const std::uint32_t count = static_cast<std::uint32_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const auto& info : value.vecInfo) {
        packet.XParse << info.byLimitType;
        packet.XParse << static_cast<std::uint8_t>(0);
        packet.XParse << static_cast<std::uint8_t>(0);
        packet.XParse << static_cast<std::uint8_t>(0);
        packet.XParse << static_cast<std::int32_t>(0);  // padding
        packet << info.psInfo;
    }
    return packet;
}
