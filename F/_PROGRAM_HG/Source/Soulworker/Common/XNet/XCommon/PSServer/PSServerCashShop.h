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
 */
struct PS_RES_ITEM_REPURCHASER_LIST {
    std::vector<STItem> vecInfo;  // 回购物品列表
};

// PS_RES_ITEM_REPURCHASER_LIST 序列化
inline XPacket& operator>>(XPacket& packet, PS_RES_ITEM_REPURCHASER_LIST& value) {
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.resize(count);
    for (auto& item : value.vecInfo) {
        packet >> item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_ITEM_REPURCHASER_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_RES_ITEM_REPURCHASER_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}
