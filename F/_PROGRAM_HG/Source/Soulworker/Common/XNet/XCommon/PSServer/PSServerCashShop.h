#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerCashShop.h - 现金商店相关结构体及序列化运算符
//
// 此文件包含现金商店的所有数据结构和序列化操作：
// - 现金购买计数
// - 现金套装
// - 商店物品
// ============================================================================

// ============================================================================
// 现金里程结构体
// ============================================================================

// GameServer PDB PSCharacter.h: PS_CASH_MILEAGE is 8 bytes.
enum E_CASH_MILEAGE_TYPE {
    E_CASH_MILEAGE_AKASHIC = 0,
    E_CASH_MILEAGE_BROACH = 1,
    E_CASH_MILEAGE_TAG = 2,
    E_CASH_MILEAGE_MAX = 3,
};

static_assert(sizeof(E_CASH_MILEAGE_TYPE) == 4,
              "E_CASH_MILEAGE_TYPE size must match GameServer PDB");

struct PS_CASH_MILEAGE {
    std::uint8_t byMileageType = 0;
    std::uint8_t _pad[3] = {};
    std::int32_t nCashMileage = 0;
};

static_assert(sizeof(PS_CASH_MILEAGE) == 8,
              "PS_CASH_MILEAGE size must match GameServer PDB");
static_assert(offsetof(PS_CASH_MILEAGE, byMileageType) == 0,
              "PS_CASH_MILEAGE.byMileageType offset mismatch");
static_assert(offsetof(PS_CASH_MILEAGE, nCashMileage) == 4,
              "PS_CASH_MILEAGE.nCashMileage offset mismatch");

struct PS_CASH_MILEAGE_LIST {
    std::vector<PS_CASH_MILEAGE> vecInfo;

    PS_CASH_MILEAGE_LIST() {
        vecInfo.clear();
    }
};

static_assert(sizeof(PS_CASH_MILEAGE_LIST) == 32,
              "PS_CASH_MILEAGE_LIST size must match GameServer PDB");

inline XPacket& operator<<(XPacket& packet, PS_CASH_MILEAGE& value) {
    packet.XParse << value.byMileageType;
    packet.XParse << value.nCashMileage;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, PS_CASH_MILEAGE_LIST& value) {
    const std::uint8_t cCount = static_cast<std::uint8_t>(value.vecInfo.size());
    packet.XParse << cCount;
    for (std::uint8_t c = 0; c < cCount; ++c) {
        packet << value.vecInfo[c];
    }
    return packet;
}

// ============================================================================
// 现金购买计数结构体
// ============================================================================

// 对齐 IDA 0x1400BB750: 现金购买计数
struct PS_CASH_BUY_COUNT {
    std::int32_t nCashShopIndex = 0;
    std::int32_t nBuyCount = 0;
    std::int64_t biEndDate = 0;
    std::uint8_t byBuyType = 0;  // 2/4/6/8 = account, others = character
};

// 对齐 IDA: 现金购买计数列表
struct PS_CASH_BUY_COUNT_LIST {
    std::vector<PS_CASH_BUY_COUNT> vecInfo;
};

// ============================================================================
// 现金套装结构体
// ============================================================================

// 对齐 IDA 0x1400BA500: 现金套装
struct PS_CASH_SET {
    std::uint8_t bySetNo = 0;
    wchar_t szName[11] = {};        // 22 bytes in GetWString
    std::int32_t nIndex[10] = {};
    std::uint32_t dwItemID[10] = {};
};

// 对齐 IDA: 现金套装列表
struct PS_CASH_SET_LIST {
    std::vector<PS_CASH_SET> vecInfo;
};

// ============================================================================
// 商店物品结构体
// ============================================================================

// 对齐 IDA: 商店物品
struct ST_SHOP_ITEM {
    std::uint32_t nItemID = 0;
    std::int16_t shCount = 0;
    std::int32_t nUpdateDate = 0;
    std::int32_t nShopIndex = 0;
};

// 对齐 IDA: 现金商店物品 (用于 m_mapCashshopList)
struct STCashItem {
    std::uint32_t dwIndex = 0;         // 商店索引 (Shop_Index)
    std::uint8_t bySellActive = 0;     // 是否可售 (Sell_Active)
    std::uint32_t dwItemID = 0;        // 售卖物品ID (SellItem_ID)
    std::uint8_t byNeedSlot = 0;       // 需要槽位 (Need_Slot)
    std::int32_t nOrder = 0;           // 排序优先级 (Sell_priority)
    std::int16_t shLevel = 0;          // 售卖等级限制 (Sell_Level)
    std::int32_t nBillingID = 0;       // 计费信息ID (BillingInfo_ID)
    std::int16_t shCashInfo = 0;       // 现金信息标志位 (位运算组合)
    std::int64_t nLimitTime[2] = {};   // 限制时间[开始,结束]
    std::uint8_t bySellCount = 0;      // 售卖数量
};

// 对齐 IDA: 商店物品列表
struct ST_SHOP_ITEM_LIST {
    std::vector<ST_SHOP_ITEM> vecInfo;
};

// 对齐 IDA: 数据库商店物品（带UAID）
struct PS_DB_SHOP_ITEM {
    std::uint32_t dwUAID = 0;
    ST_SHOP_ITEM stShopItem;
};

// ============================================================================
// 现金购买计数序列化运算符
// ============================================================================

inline XPacket& operator>>(XPacket& packet, PS_CASH_BUY_COUNT& value) {
    packet.XParse >> value.nCashShopIndex;
    packet.XParse >> value.nBuyCount;
    packet.XParse >> value.biEndDate;
    packet.XParse >> value.byBuyType;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_CASH_BUY_COUNT& value) {
    packet.XParse << value.nCashShopIndex;
    packet.XParse << value.nBuyCount;
    packet.XParse << value.biEndDate;
    packet.XParse << value.byBuyType;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_CASH_BUY_COUNT& value) {
    packet.XParse << value.nCashShopIndex;
    packet.XParse << value.nBuyCount;
    packet.XParse << value.biEndDate;
    packet.XParse << value.byBuyType;
    return packet;
}

// PS_CASH_BUY_COUNT_LIST 序列化
inline XPacket& operator>>(XPacket& packet, PS_CASH_BUY_COUNT_LIST& value) {
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.resize(count);
    for (auto& item : value.vecInfo) {
        packet >> item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_CASH_BUY_COUNT_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_CASH_BUY_COUNT_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// ============================================================================
// 现金套装序列化运算符
// ============================================================================

inline XPacket& operator>>(XPacket& packet, PS_CASH_SET& value) {
    packet.XParse >> value.bySetNo;
    short outLen = 0;
    packet.XParse.GetWString(value.szName, 11, outLen);
    for (int i = 0; i < 10; ++i) {
        packet.XParse >> value.nIndex[i];
    }
    for (int i = 0; i < 10; ++i) {
        packet.XParse >> value.dwItemID[i];
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_CASH_SET& value) {
    packet.XParse << value.bySetNo;
    packet.XParse << std::wstring(value.szName);
    for (int i = 0; i < 10; ++i) {
        packet.XParse << value.nIndex[i];
    }
    for (int i = 0; i < 10; ++i) {
        packet.XParse << value.dwItemID[i];
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_CASH_SET& value) {
    packet.XParse << value.bySetNo;
    packet.XParse << std::wstring(value.szName);
    for (int i = 0; i < 10; ++i) {
        packet.XParse << value.nIndex[i];
    }
    for (int i = 0; i < 10; ++i) {
        packet.XParse << value.dwItemID[i];
    }
    return packet;
}

// PS_CASH_SET_LIST 序列化
inline XPacket& operator>>(XPacket& packet, PS_CASH_SET_LIST& value) {
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.resize(count);
    for (auto& item : value.vecInfo) {
        packet >> item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_CASH_SET_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_CASH_SET_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// ============================================================================
// 商店物品序列化运算符
// ============================================================================

inline XPacket& operator>>(XPacket& packet, ST_SHOP_ITEM& value) {
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse >> value.nUpdateDate;
    packet.XParse >> value.nShopIndex;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_SHOP_ITEM& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value.nUpdateDate;
    packet.XParse << value.nShopIndex;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_SHOP_ITEM& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value.nUpdateDate;
    packet.XParse << value.nShopIndex;
    return packet;
}

// ST_SHOP_ITEM_LIST 序列化
inline XPacket& operator>>(XPacket& packet, ST_SHOP_ITEM_LIST& value) {
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.resize(count);
    for (auto& item : value.vecInfo) {
        packet >> item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_SHOP_ITEM_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_SHOP_ITEM_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// PS_DB_SHOP_ITEM 序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_SHOP_ITEM& value) {
    packet.XParse >> value.dwUAID;
    packet >> value.stShopItem;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_SHOP_ITEM& value) {
    packet.XParse << value.dwUAID;
    packet << value.stShopItem;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_SHOP_ITEM& value) {
    packet.XParse << value.dwUAID;
    packet << value.stShopItem;
    return packet;
}

// ============================================================================
// 商店失败物品列表
// ============================================================================

/**
 * 来自 IDA: PS_SHOP_FAIL_ITEM - 商店购买失败物品列表
 */
struct PS_SHOP_FAIL_ITEM {
    std::vector<std::int32_t> vecItemID;  // 失败物品ID列表
};

// PS_SHOP_FAIL_ITEM 序列化
inline XPacket& operator>>(XPacket& packet, PS_SHOP_FAIL_ITEM& value) {
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecItemID.resize(count);
    for (auto& item : value.vecItemID) {
        packet.XParse >> item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_SHOP_FAIL_ITEM& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecItemID.size());
    for (const auto& item : value.vecItemID) {
        packet.XParse << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_SHOP_FAIL_ITEM& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecItemID.size());
    for (const auto& item : value.vecItemID) {
        packet.XParse << item;
    }
    return packet;
}

// ============================================================================
// 现金礼物信息
// ============================================================================

/**
 * 来自 IDA 0x1400BAE40: ST_CASH_ITEM_GIFT - 现金物品礼物信息
 */
struct ST_CASH_ITEM_GIFT {
    wchar_t szAccountID[21] = {};  // 账号ID (42 bytes)
    wchar_t szName[21] = {};       // 角色名 (42 bytes)
    std::uint8_t _pad0[4] = {};    // padding
};

// ST_CASH_ITEM_GIFT 序列化
inline XPacket& operator>>(XPacket& packet, ST_CASH_ITEM_GIFT& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.szAccountID, 21, outLen);
    packet.XParse.GetWString(value.szName, 21, outLen);
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_CASH_ITEM_GIFT& value) {
    packet.XParse << std::wstring(value.szAccountID);
    packet.XParse << std::wstring(value.szName);
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_CASH_ITEM_GIFT& value) {
    packet.XParse << std::wstring(value.szAccountID);
    packet.XParse << std::wstring(value.szName);
    return packet;
}

// ============================================================================
// 回购物品列表
// ============================================================================

/**
 * 来自 IDA 0x1400B8750: PS_RES_ITEM_REPURCHASER_LIST - 回购物品列表
 * IDA: Contains vecInfo (STItem list), psSocketList, and psBroachList
 */
struct PS_RES_ITEM_REPURCHASER_LIST {
    std::vector<STItem> vecInfo;        // 回购物品列表
    PS_ITEM_SOCKET_LIST psSocketList;   // 镶嵌列表
    PS_ITEM_BROACH_LIST psBroachList;   // 镂刻列表
};

// PS_RES_ITEM_REPURCHASER_LIST 序列化
inline XPacket& operator>>(XPacket& packet, PS_RES_ITEM_REPURCHASER_LIST& value) {
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.resize(count);
    for (auto& item : value.vecInfo) {
        packet >> item;
    }
    packet >> value.psSocketList;
    packet >> value.psBroachList;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_ITEM_REPURCHASER_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    packet << value.psSocketList;
    packet << value.psBroachList;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_RES_ITEM_REPURCHASER_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    packet << value.psSocketList;
    packet << value.psBroachList;
    return packet;
}

// ============================================================================
// GM 现金商店物品信息 (IDA LoadCashShop 使用)
// ============================================================================

/**
 * 来自 IDA: STGMCashItem - GM现金商店物品
 */
struct STGMCashItem {
    std::uint32_t dwID = 0;           // 商店物品ID
    std::uint8_t byActive = 0;        // 是否激活
};

/**
 * 来自 IDA: STGMCashItemList - GM现金商店物品列表
 */
struct STGMCashItemList {
    std::vector<STGMCashItem> vecInfo;
};

/**
 * 来自 IDA: STCashInfo - 现金信息 (用于计费)
 */
struct STCashInfo {
    std::uint32_t dwItemID = 0;       // 物品ID
    std::int16_t shCount = 0;         // 数量
    std::int32_t nBasePrice = 0;      // 基础价格
    std::int32_t nPrice = 0;          // 实际价格
};

/**
 * 来自 IDA: STCashItemDetail - 现金商店物品详细信息
 * 用于 m_mapCashshopList
 */
struct STCashItemDetail {
    std::uint32_t dwIndex = 0;        // 商店索引
    std::uint8_t bySellActive = 0;    // 是否可售
    std::uint32_t dwItemID = 0;       // 售卖物品ID
    std::uint8_t byNeedSlot = 0;      // 需要槽位
    std::int32_t nOrder = 0;          // 排序优先级
    std::int16_t shLevel = 0;         // 售卖等级限制
    std::int32_t nBillingID = 0;      // 计费信息ID
    std::int16_t shCashInfo = 0;      // 现金信息标志位
    std::int64_t nLimitTime[2] = {};  // 限制时间[开始,结束]
    std::uint8_t bySellCount = 0;     // 售卖数量
    std::vector<STCashInfo> vecCashInfo;  // 计费详情列表
};

/**
 * 来自 IDA: STCashItemList - 现金商店物品列表
 */
struct STCashItemList {
    std::vector<STCashItemDetail> vecInfo;
};

/**
 * 来自 IDA: ST_CASH_SHOP_TAB_LIST - 现金商店标签列表
 */
struct ST_CASH_SHOP_TAB_LIST {
    std::vector<std::uint8_t> vecTabInfo;  // 标签信息列表
};
