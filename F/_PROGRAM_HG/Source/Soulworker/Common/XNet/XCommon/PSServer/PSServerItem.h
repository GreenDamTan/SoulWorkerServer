#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerItem.h - 物品相关结构体及序列化运算符
//
// 此文件包含物品相关的所有数据结构和序列化操作：
// - ST_CREATE_ITEM - 创建物品信息
// - ST_CREATE_ITEMS - 创建物品列表
// - ST_GET_INFO - 物品使用获取信息
// - PS_DB_ITEM_COUNTBOX - 物品数量盒子更新请求
// ============================================================================

// ============================================================================
// 物品相关结构体
// ============================================================================

// PDB UDT 0x34f0: creation reasons are persisted with the DB item mutation.
enum eITEM_CREATE_TYPE {
    E_ITEM_CREATE_TYPE_ITEM_GMCOMMAND = 1,
    E_ITEM_CREATE_TYPE_PICK_UP_MONEY = 2,
    E_ITEM_CREATE_TYPE_PICK_UP_ITEM = 3,
    E_ITEM_CREATE_TYPE_QUEST_ACCEPT = 4,
    E_ITEM_CREATE_TYPE_QUEST_REWARD = 5,
    E_ITEM_CREATE_TYPE_INTERACTION = 6,
    E_ITEM_CREATE_TYPE_SHARE_POINT = 7,
    E_ITEM_CREATE_TYPE_SOUL_METRY = 8,
    E_ITEM_CREATE_TYPE_MAZE_BASE_ITEM = 9,
    E_ITEM_CREATE_TYPE_HIDDEN_EVENT_REWARD = 10,
    E_ITEM_CREATE_TYPE_VACCUM_CUBE = 11,
    E_ITEM_CREATE_TYPE_MYROOM_ITEM = 12,
    E_ITEM_CREATE_TYPE_REWARD_INFINITE_TOWER = 13,
    E_ITEM_CREATE_TYPE_GACHA = 14,
    E_ITEM_CREATE_TYPE_HELPER_SUPPORT = 15,
    E_ITEM_CREATE_TYPE_ACHIEVE = 16,
    E_ITEM_CREATE_TYPE_CONDITION_COMPLETE = 17,
    E_ITEM_CREATE_TYPE_GIVE_UP_QUEST = 18,
    E_ITEM_CREATE_TYPE_SHOP_BUY = 19,
    E_ITEM_CREATE_TYPE_AKASHIC_DISASSEMBLE = 20,
    E_ITEM_CREATE_TYPE_REWARD_ATTENDANCE = 21,
    E_ITEM_CREATE_TYPE_REWARD_PLAYTIME = 22,
    E_ITEM_CREATE_TYPE_USE_RANDOMBOX = 23,
    E_ITEM_CREATE_TYPE_RECEIPT_ACCOUNT_ALL = 24,
    E_ITEM_CREATE_TYPE_RECEIPT_POST_ALL = 25,
    E_ITEM_CREATE_TYPE_SOUL_EVENT = 26,
    E_ITEM_CREATE_TYPE_EXCHANGE = 27,
    E_ITEM_CREATE_TYPE_FAIL_EVOLUTION = 28,
    E_ITEM_CREATE_TYPE_MAKE_AKASHIC = 29,
    E_ITEM_CREATE_TYPE_MAKE_AKASHIC_EX = 30,
    E_ITEM_CREATE_TYPE_DISASSEMBLE = 31,
    E_ITEM_CREATE_TYPE_DISASSEMBLE_EX = 32,
    E_ITEM_CREATE_TYPE_COMPOSE_BROACH = 33,
    E_ITEM_CREATE_TYPE_MAKE = 34,
    E_ITEM_CREATE_TYPE_REPURCHASE = 35,
    E_ITEM_CREATE_TYPE_HARVEST = 36,
    E_ITEM_CREATE_TYPE_PACKAGEBOX = 37,
    E_ITEM_CREATE_TYPE_BUY_FOR_CASH = 38,
    E_ITEM_CREATE_TYPE_RECEIPT_POST = 39,
    E_ITEM_CREATE_TYPE_RECEIPT_ACCOUNT = 40,
    E_ITEM_CREATE_TYPE_BROACH_REMOVE = 41,
    E_ITEM_CREATE_TYPE_WEEKLY_MISSION_REWARD = 42,
    E_ITEM_CREATE_TYPE_WEEKLY_MISSION_REWARD_GROUP = 43,
    E_ITEM_CREATE_TYPE_SOCKET_DETACH = 44,
    E_ITEM_CREATE_TYPE_REWARD_WORLD_EVENT = 45,
    E_ITEM_CREATE_TYPE_ROULETTE_EVENT = 46,
    E_ITEM_CREATE_TYPE_AKASHIC_COMPOSE = 47,
    E_ITEM_CREATE_TYPE_NET_CAFE_COSTUME = 48,
    E_ITEM_CREATE_TYPE_RANKING_REWARD = 49,
    E_ITEM_CREATE_TYPE_BI_REWARD = 50,
};

static_assert(sizeof(eITEM_CREATE_TYPE) == 4,
              "eITEM_CREATE_TYPE must match the GameServer PDB");

/**
 * @brief 创建物品信息
 * Per IDA: ST_CREATE_ITEM - 8 bytes
 */
struct ST_CREATE_ITEM {
    std::int32_t nItemID = 0;
    std::int16_t shCount = 0;
    std::uint8_t byUpgrade = 0;
};

/**
 * @brief 创建物品列表
 * Per IDA: ST_CREATE_ITEMS - 32 bytes
 */
struct ST_CREATE_ITEMS {
    std::vector<ST_CREATE_ITEM> vecInfo;
};

// TODO: 推测目录归属
// GameServer PDB UDT layouts used by the CGocInventory tool-result state.
struct PS_TOOL_ITEM {
    std::int32_t nItemID = 0;
    std::int32_t nCount = 0;
    std::uint8_t byComplete = 0;
    std::int32_t nCurIndex = 0;
};

struct PS_TOOL_SOULSTONE_INFO {
    PS_TOOL_ITEM stItemInfo;
    std::uint8_t byRate = 0;
    char szItemName[100] = {};
};

struct PS_TOOL_ITEM_INFO {
    std::int32_t nItemID = 0;
    std::int32_t nCount = 0;
    std::int32_t nValue1 = 0;
    std::int32_t nValue2 = 0;
    std::int32_t nValue3 = 0;
    char szItemName[100] = {};
};

struct PS_RES_TOOL_DROP_INFO {
    std::vector<PS_TOOL_ITEM_INFO> vecInfo;
};

struct PS_RES_TOOL_SOULSTONE {
    std::uint8_t byType = 0;
    std::vector<PS_TOOL_SOULSTONE_INFO> vecInfo;
};

static_assert(sizeof(PS_TOOL_ITEM) == 16,
              "PS_TOOL_ITEM size must match GameServer PDB");
static_assert(sizeof(PS_TOOL_SOULSTONE_INFO) == 120,
              "PS_TOOL_SOULSTONE_INFO size must match GameServer PDB");
static_assert(sizeof(PS_TOOL_ITEM_INFO) == 120,
              "PS_TOOL_ITEM_INFO size must match GameServer PDB");
static_assert(sizeof(PS_RES_TOOL_DROP_INFO) == 32,
              "PS_RES_TOOL_DROP_INFO size must match GameServer PDB");
static_assert(sizeof(PS_RES_TOOL_SOULSTONE) == 40,
              "PS_RES_TOOL_SOULSTONE size must match GameServer PDB");

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

// ST_CREATE_ITEM 反序列化
inline void operator>>(XPacket& packet, ST_CREATE_ITEM& value) {
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse >> value.byUpgrade;
}

// ST_CREATE_ITEM 序列化
inline XPacket& operator<<(XPacket& packet, const ST_CREATE_ITEM& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value.byUpgrade;
    return packet;
}

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
