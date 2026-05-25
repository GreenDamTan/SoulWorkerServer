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
