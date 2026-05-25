#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerItem.h - 物品相关结构体及序列化运算符
//
// 此文件包含物品相关的所有数据结构和序列化操作：
// - ST_CREATE_ITEMS - 创建物品列表
// - ST_GET_INFO - 物品使用获取信息
// - PS_DB_ITEM_COUNTBOX - 物品数量盒子更新请求
// ============================================================================

// ============================================================================
// 物品相关结构体
// ============================================================================

/**
 * @brief 创建物品列表
 * Per IDA: ST_CREATE_ITEMS - 32 bytes
 */
struct ST_CREATE_ITEMS {
    std::vector<ST_CREATE_ITEM> vecInfo;
};

/**
 * @brief 物品使用获取信息
 * Per IDA 0x140052710: ST_GET_INFO - 72 bytes
 */
struct ST_GET_INFO {
    std::int32_t nExp = 0;                      // +0x00: 经验值 (4 bytes)
    std::int32_t nMoney = 0;                   // +0x04: 金币 (4 bytes)
    std::int32_t nBP = 0;                      // +0x08: BP (4 bytes)
    std::int32_t nEther = 0;                   // +0x0C: Ether (4 bytes)
    std::int32_t nTitleID[2] = {0, 0};         // +0x10: 称号ID数组 (8 bytes)
    std::uint32_t dwHelperID = 0;              // +0x18: HelperID (4 bytes)
    ST_CREATE_ITEMS stGetItem;                 // +0x20: 获取物品列表 (32 bytes)
    std::int32_t nRenovatePoint = 0;           // +0x40: 翻新点数 (4 bytes)
    std::int32_t nRenovatePointTotal = 0;      // +0x44: 翻新总点数 (4 bytes)
};

/**
 * @brief 物品数量盒子更新请求
 * Per IDA 0x1400592C0: PS_DB_ITEM_COUNTBOX - 224 bytes
 */
struct PS_DB_ITEM_COUNTBOX {
    std::uint32_t dwUCID = 0;                  // +0x00: UCID (4 bytes)
    std::uint8_t _pad0[4] = {};                // +0x04: padding (4 bytes)
    PS_RES_STORAGE_INFO psUpdateItemList;     // +0x08: 更新物品列表 (40 bytes)
    PS_RES_STORAGE_INFO psCreateItemList;     // +0x30: 创建物品列表 (40 bytes)
    PS_RES_STORAGE_INFO psCountboxItem;       // +0x58: Countbox物品 (40 bytes)
    ST_GET_INFO stGetInfo;                    // +0x80: 获取信息 (72 bytes)
    std::uint32_t dwUseItemID = 0;            // +0xC8: 使用物品ID (4 bytes)
    std::int64_t biUseItemSerial = 0;         // +0xD0: 使用物品序列号 (8 bytes)
    std::int32_t nErrorCode = 0;              // +0xD8: 错误码 (4 bytes)
};

// ============================================================================
// Static Assertions - 大小和偏移验证
// ============================================================================

static_assert(sizeof(ST_GET_INFO) == 72, "ST_GET_INFO size must match IDA");
static_assert(sizeof(PS_DB_ITEM_COUNTBOX) == 224, "PS_DB_ITEM_COUNTBOX size must match IDA");

// ============================================================================
// 物品序列化运算符
// ============================================================================

// ST_CREATE_ITEMS 反序列化
inline void operator>>(XPacket& packet, ST_CREATE_ITEMS& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        ST_CREATE_ITEM info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

// ST_CREATE_ITEMS 序列化
inline XPacket& operator<<(XPacket& packet, const ST_CREATE_ITEMS& value) {
    packet.XParse << static_cast<std::uint16_t>(value.vecInfo.size());
    for (const ST_CREATE_ITEM& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

// ST_GET_INFO 反序列化
inline void operator>>(XPacket& packet, ST_GET_INFO& value) {
    packet.XParse >> value.nExp;
    packet.XParse >> value.nMoney;
    packet.XParse >> value.nBP;
    packet.XParse >> value.nEther;
    packet.XParse >> value.nTitleID[0];
    packet.XParse >> value.nTitleID[1];
    packet.XParse >> value.dwHelperID;
    packet >> value.stGetItem;
    packet.XParse >> value.nRenovatePoint;
    packet.XParse >> value.nRenovatePointTotal;
}

// ST_GET_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_GET_INFO& value) {
    packet.XParse << value.nExp;
    packet.XParse << value.nMoney;
    packet.XParse << value.nBP;
    packet.XParse << value.nEther;
    packet.XParse << value.nTitleID[0];
    packet.XParse << value.nTitleID[1];
    packet.XParse << value.dwHelperID;
    packet << value.stGetItem;
    packet.XParse << value.nRenovatePoint;
    packet.XParse << value.nRenovatePointTotal;
    return packet;
}

// PS_DB_ITEM_COUNTBOX 反序列化
inline void operator>>(XPacket& packet, PS_DB_ITEM_COUNTBOX& value) {
    packet.XParse >> value.dwUCID;
    packet >> value.psUpdateItemList;
    packet >> value.psCreateItemList;
    packet >> value.psCountboxItem;
    packet >> value.stGetInfo;
    packet.XParse >> value.dwUseItemID;
    packet.XParse >> value.biUseItemSerial;
}

// PS_DB_ITEM_COUNTBOX 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_COUNTBOX& value) {
    packet.XParse << value.dwUCID;
    packet << value.psUpdateItemList;
    packet << value.psCreateItemList;
    packet << value.psCountboxItem;
    packet << value.stGetInfo;
    packet.XParse << value.dwUseItemID;
    packet.XParse << value.biUseItemSerial;
    packet.XParse << value.nErrorCode;
    return packet;
}
