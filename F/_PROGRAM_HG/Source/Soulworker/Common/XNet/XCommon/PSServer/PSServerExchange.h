#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMail.h"

// ============================================================================
// PSServerExchange.h - 交易所 (Exchange) 相关结构体及序列化运算符
//
// 此文件包含交易所系统相关的所有协议结构：
// - 交易所价格历史查询
// - 交易所搜索
// - 交易所关注列表
// - 我的交易所列表
// - 交易所物品购买
// ============================================================================

// ============================================================================
// Exchange (交易所) 协议结构体
// ============================================================================

// 对齐 IDA 0x1400C9290: 交易所价格历史条目
struct ST_EXCHANGE_PRICE_INFO {
    std::uint32_t dwItemID = 0;
    std::int16_t sCount = 0;
    std::uint8_t _pad0[2] = {};  // 对齐填充
    std::int64_t nPrice_One = 0;
    std::int64_t tRegDate = 0;
    wchar_t strBuyerName[21] = {};
};

// 对齐 IDA 0x1400C9920: 交易所价格历史请求（客户端请求格式）
struct PS_EXCHANGE_PRICE_HISTORY_REQ {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwItemID = 0;
};

// 对齐 IDA 0x140062D80: 交易所价格历史响应
struct PS_EXCHANGE_PRICE_HISTORY_RES {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwItemID = 0;
    std::vector<ST_EXCHANGE_PRICE_INFO> vecHistory;
    std::int64_t n64Price_High = 0;
    std::int64_t n64Price_Low = 0;
    std::int64_t n64Price_Avg = 0;
};

// 对齐 IDA 0x140062E20: DB 返回的交易所价格历史
struct PS_DB_EXCHANGE_PRICE_HISTORY_RES {
    PS_EXCHANGE_PRICE_HISTORY_RES stRes{};
    int nTotalCount = 0;
    std::int64_t n64TotalPrice = 0;
};

// 对齐 IDA: DB 请求的交易所价格历史
struct PS_DB_EXCHANGE_PRICE_HISTORY_REQ {
    PS_EXCHANGE_PRICE_HISTORY_REQ stReq{};
    std::int64_t n64Date = 0;
};

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_EXCHANGE_PRICE_HISTORY_REQ& value) {
    packet.XParse << value.stReq.dwUCID;
    packet.XParse << value.stReq.dwItemID;
    packet.XParse << value.n64Date;
    return packet;
}

// ============================================================================
// Exchange (交易所) 协议结构体 - 对齐 IDA XSQLExchange
// ============================================================================

// 对齐 IDA 0x140038880: 交易所搜索请求
struct PS_EXCHANGE_SEARCH_REQ {
    std::uint32_t dwItemID = 0;
    int nCategoryID = 0;
    int nSubCategoryID = 0;
    int nUseClass = 0;
    int nLevelMin = 0;
    int nLevelMax = 0;
    int nItemGradeMin = 0;
    int nItemGradeMax = 0;
    std::int64_t nPrice = 0;
    int nUpgrade = 0;
    int nPageNum = 0;
};

// 对齐 IDA: 交易所物品镶嵌信息
struct ST_EXCHANGE_BROACH_INFO {
    std::int64_t biSerial = 0;
    std::uint32_t dwItemID[15] = {};
};

// 对齐 IDA: 交易所物品扩展选项
struct ST_EXCHANGE_EXTEND_OPTION {
    std::uint8_t byType = 0;
    int nOption = 0;
};

// 对齐 IDA 0x140038880: 交易所物品信息
struct ST_EXCHANGE_ITEM {
    std::uint32_t dwExchangeID = 0;
    std::uint8_t _pad0[4] = {};
    std::int64_t nPrice_One = 0;
    std::int64_t nExpireRemainTime = 0;
    std::uint8_t byCash_Commission = 0;
    std::uint8_t _pad1[7] = {};
    STItem stItem{};
    PS_ITEM_SOCKET_LIST vecSocketList{};
    ST_ITEM_BROACH stBroachInfo{};
    PS_ITEM_PACKAGE psPackageList{};
    wchar_t strSellerName[21] = {};
};

// 对齐 IDA 0x140038880: 交易所搜索响应
struct PS_EXCHANGE_SEARCH_RES {
    int nPageMax = 0;
    int nPage = 0;
    std::vector<ST_EXCHANGE_ITEM> vecItem;
};

// 对齐 IDA 0x1400399C0: 交易所关注列表请求
struct PS_EXCHANGE_INTEREST_LIST_REQ {
    std::uint32_t dwUCID = 0;
};

// 对齐 IDA 0x1400399C0: 交易所关注列表响应
struct PS_EXCHANGE_INTEREST_LIST_RES {
    std::uint32_t dwUCID = 0;
    std::vector<std::uint32_t> vecItemList;
};

// 对齐 IDA 0x14003B340: 我的交易所物品
struct ST_MY_EXCHANGE_ITEM {
    std::uint32_t dwExchangeID = 0;
    std::uint8_t _pad0[4] = {};
    std::int64_t nExpireDate = 0;
    std::int64_t nOpenDate = 0;
    std::uint8_t byState = 2;
    std::uint8_t _pad1[1] = {};
    std::int16_t sInitCount = 0;
    std::uint8_t _pad2[2] = {};
    std::int64_t nPrice_One = 0;
    STItem stItem{};
    PS_ITEM_SOCKET_LIST vecSocketList{};
    ST_ITEM_BROACH stBroachInfo{};
    PS_ITEM_PACKAGE psPackageList{};
};

// 对齐 IDA 0x14003B340: 我的交易所列表请求
struct PS_EXCHANGE_MY_LIST_REQ {
    std::uint32_t dwUCID = 0;
};

// 对齐 IDA 0x14003B340: 我的交易所列表响应
struct PS_EXCHANGE_MY_LIST_RES {
    std::uint8_t bLast = 0;
    std::vector<ST_MY_EXCHANGE_ITEM> vecMyList;
};

// 对齐 IDA 0x14003B5A0: 交易所物品购买检查请求
// 对齐 IDA: 交易所物品上架请求 (客户端 -> GameServer)
struct PS_EXCHANGE_SELL_REGISTER_REQ {
    std::uint32_t dwItemID = 0;           // offset 0
    std::int16_t shCount = 0;             // offset 4
    std::uint8_t byInvenType = 0;         // offset 6
    std::uint8_t _pad0[1] = {};           // offset 7
    std::int16_t shSlotPos = 0;           // offset 8
    std::uint8_t _pad1[6] = {};           // offset 10
    std::int64_t nPrice_One = 0;          // offset 16
    std::uint8_t byCash_Commission = 0;   // offset 24
    std::uint8_t byCash_Count = 0;        // offset 25
    std::uint8_t byCash_Expire = 0;       // offset 26
    std::uint8_t _pad2[5] = {};           // offset 27
};

// PS_EXCHANGE_SELL_REGISTER_REQ 序列化
inline void operator>>(XPacket& packet, PS_EXCHANGE_SELL_REGISTER_REQ& value) {
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.shCount;
    packet.XParse >> value.byInvenType;
    packet.XParse >> value.shSlotPos;
    packet.XParse >> value.nPrice_One;
    packet.XParse >> value.byCash_Commission;
    packet.XParse >> value.byCash_Count;
    packet.XParse >> value.byCash_Expire;
}
struct PS_EXCHANGE_ITEM_BUY_REQ {
    std::uint32_t dwExchangeID = 0;
    std::int16_t shCount = 0;
};

// 对齐 IDA 0x14003B5A0: 交易所物品购买检查响应（DB返回）
struct PS_EXCHANGE_ITEM_BUY_RES {
    std::uint32_t dwExchangeID = 0;
    std::int16_t shCount = 0;
    std::int64_t biPrice = 0;
    int nPackageCount = 0;
};

// 对齐 IDA 0x140039BF0: 交易所关注物品请求
struct PS_EXCHANGE_INTEREST_ITEM_REQ {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwItemID = 0;
    std::uint8_t bAdd = 0;
};

// 对齐 IDA 0x140039BF0: 交易所关注物品响应
struct PS_EXCHANGE_INTEREST_ITEM_RES {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwItemID = 0;
    std::uint8_t bAdd = 0;
    int nResult = 0;
};

// ============================================================================
// Exchange (交易所) 序列化操作符
// ============================================================================

// ST_EXCHANGE_PRICE_INFO 对齐 IDA 0x1400E9940/0x1400E9A00
inline XPacket& operator<<(XPacket& packet, const ST_EXCHANGE_PRICE_INFO& value) {
    packet.XParse << value.dwItemID;
    packet.XParse << value.sCount;
    packet.XParse << value.nPrice_One;
    packet.XParse << value.tRegDate;
    packet.XParse << GreenDamTan_BoundedWideString(value.strBuyerName);
    return packet;
}

inline void operator>>(XPacket& packet, ST_EXCHANGE_PRICE_INFO& value) {
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.sCount;
    packet.XParse >> value.nPrice_One;
    packet.XParse >> value.tRegDate;
    short sLen = 0;
    packet.XParse.GetWString(value.strBuyerName, 21, sLen);
}

// PS_EXCHANGE_PRICE_HISTORY_REQ 对齐 IDA 0x1400C9920（与 PS_RES_RECRUIT_DELETE 同布局）
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_PRICE_HISTORY_REQ& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwItemID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_EXCHANGE_PRICE_HISTORY_REQ& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwItemID;
}

// PS_EXCHANGE_PRICE_HISTORY_RES 对齐 IDA 0x1400E9AB0/0x1400E9BC0
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_PRICE_HISTORY_RES& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwItemID;
    const std::int16_t count = static_cast<std::int16_t>(std::min<std::size_t>(value.vecHistory.size(), 0x7FFF));
    packet.XParse << count;
    for (std::int16_t i = 0; i < count; ++i) {
        packet << value.vecHistory[static_cast<std::size_t>(i)];
    }
    packet.XParse << value.n64Price_High;
    packet.XParse << value.n64Price_Low;
    packet.XParse << value.n64Price_Avg;
    return packet;
}

inline void operator>>(XPacket& packet, PS_EXCHANGE_PRICE_HISTORY_RES& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwItemID;
    std::int16_t count = 0;
    packet.XParse >> count;
    value.vecHistory.clear();
    value.vecHistory.reserve(static_cast<std::size_t>(count));
    for (std::int16_t i = 0; i < count; ++i) {
        ST_EXCHANGE_PRICE_INFO item{};
        packet >> item;
        value.vecHistory.push_back(item);
    }
    packet.XParse >> value.n64Price_High;
    packet.XParse >> value.n64Price_Low;
    packet.XParse >> value.n64Price_Avg;
}

// PS_DB_EXCHANGE_PRICE_HISTORY_RES 对齐 IDA 0x1400E9D70
inline XPacket& operator<<(XPacket& packet, const PS_DB_EXCHANGE_PRICE_HISTORY_RES& value) {
    packet << value.stRes;
    packet.XParse << value.nTotalCount;
    packet.XParse << value.n64TotalPrice;
    return packet;
}

inline void operator>>(XPacket& packet, PS_DB_EXCHANGE_PRICE_HISTORY_RES& value) {
    packet >> value.stRes;
    packet.XParse >> value.nTotalCount;
    packet.XParse >> value.n64TotalPrice;
}

// PS_EXCHANGE_SEARCH_REQ 对齐 IDA 0x140038880
inline void operator>>(XPacket& packet, PS_EXCHANGE_SEARCH_REQ& value) {
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.nCategoryID;
    packet.XParse >> value.nSubCategoryID;
    packet.XParse >> value.nUseClass;
    packet.XParse >> value.nLevelMin;
    packet.XParse >> value.nLevelMax;
    packet.XParse >> value.nItemGradeMin;
    packet.XParse >> value.nItemGradeMax;
    packet.XParse >> value.nPrice;
    packet.XParse >> value.nUpgrade;
    packet.XParse >> value.nPageNum;
}

// PS_EXCHANGE_SEARCH_RES 对齐 IDA 0x140038880
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_SEARCH_RES& value) {
    packet.XParse << value.nPageMax;
    packet.XParse << value.nPage;
    packet.XParse << static_cast<std::uint8_t>(value.vecItem.size());
    for (const ST_EXCHANGE_ITEM& item : value.vecItem) {
        packet.XParse << item.dwExchangeID;
        packet.XParse << item.stItem.nItemID;
        packet.XParse << item.stItem.xSerial;
        packet.XParse << item.stItem.sCount;
        packet.XParse << item.stItem.byUpgrade;
        for (const ST_EXTEND_OPTION& opt : item.stItem.stExtendOption) {
            packet.XParse << opt.byType;
            packet.XParse << opt.nOption;
        }
        packet.XParse << item.stItem.bySocketActiveCount;
        packet.XParse << item.stItem.byUpgradeCount;
        packet.XParse << item.stItem.byUpgradeLimit;
        packet.XParse << item.stItem.byEndurance;
        packet.XParse << item.stItem.byRestoreCount;
        packet.XParse << item.stItem.bySealCount;
        packet.XParse << item.stItem.bySealDelCount;
        packet.XParse << GreenDamTan_BoundedString(item.stItem.szBroachState);
        packet.XParse << item.stItem.nAttack;
        packet.XParse << item.stItem.nDefense;
        packet.XParse << item.stItem.nTitleID;
        packet.XParse << item.stItem.byUseCount;
        packet.XParse << item.stItem.nDyeID;
        packet.XParse << item.nPrice_One;
        packet.XParse << item.byCash_Commission;
        packet.XParse << item.stBroachInfo.biSerial;
        for (std::uint32_t id : item.stBroachInfo.dwItemID) {
            packet.XParse << id;
        }
        packet.XParse << GreenDamTan_BoundedWideString(item.strSellerName);
        packet.XParse << item.nExpireRemainTime;
    }
    return packet;
}

// PS_EXCHANGE_INTEREST_LIST_REQ 对齐 IDA 0x1400399C0
inline void operator>>(XPacket& packet, PS_EXCHANGE_INTEREST_LIST_REQ& value) {
    packet.XParse >> value.dwUCID;
}

// PS_EXCHANGE_INTEREST_LIST_RES 对齐 IDA 0x1400399C0
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_INTEREST_LIST_RES& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << static_cast<std::uint8_t>(value.vecItemList.size());
    for (std::uint32_t id : value.vecItemList) {
        packet.XParse << id;
    }
    return packet;
}

// PS_EXCHANGE_MY_LIST_RES 对齐 IDA 0x14003B340
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_MY_LIST_RES& value) {
    packet.XParse << value.bLast;
    packet.XParse << static_cast<std::uint8_t>(value.vecMyList.size());
    for (const ST_MY_EXCHANGE_ITEM& item : value.vecMyList) {
        packet.XParse << item.dwExchangeID;
        packet.XParse << item.stItem.nItemID;
        packet.XParse << item.stItem.xSerial;
        packet.XParse << item.stItem.sCount;
        packet.XParse << item.stItem.byUpgrade;
        for (const ST_EXTEND_OPTION& opt : item.stItem.stExtendOption) {
            packet.XParse << opt.byType;
            packet.XParse << opt.nOption;
        }
        packet.XParse << item.stItem.bySocketActiveCount;
        packet.XParse << item.stItem.byUpgradeCount;
        packet.XParse << item.stItem.byUpgradeLimit;
        packet.XParse << item.stItem.byEndurance;
        packet.XParse << item.stItem.byRestoreCount;
        packet.XParse << item.stItem.bySealCount;
        packet.XParse << item.stItem.bySealDelCount;
        packet.XParse << GreenDamTan_BoundedString(item.stItem.szBroachState);
        packet.XParse << item.stItem.nAttack;
        packet.XParse << item.stItem.nDefense;
        packet.XParse << item.stItem.nTitleID;
        packet.XParse << item.stItem.byUseCount;
        packet.XParse << item.stItem.nDyeID;
        packet.XParse << item.nPrice_One;
        packet.XParse << item.stBroachInfo.biSerial;
        for (std::uint32_t id : item.stBroachInfo.dwItemID) {
            packet.XParse << id;
        }
        packet.XParse << item.nExpireDate;
    }
    return packet;
}

// PS_EXCHANGE_ITEM_BUY_REQ 对齐 IDA 0x14003B5A0
inline void operator>>(XPacket& packet, PS_EXCHANGE_ITEM_BUY_REQ& value) {
    packet.XParse >> value.dwExchangeID;
    packet.XParse >> value.shCount;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_EXCHANGE_ITEM_BUY_REQ& value) {
    packet.XParse << value.dwExchangeID;
    packet.XParse << value.shCount;
    return packet;
}

// PS_EXCHANGE_ITEM_BUY_RES 对齐 IDA 0x14003B5A0
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_ITEM_BUY_RES& value) {
    packet.XParse << value.dwExchangeID;
    packet.XParse << value.shCount;
    packet.XParse << value.biPrice;
    packet.XParse << value.nPackageCount;
    return packet;
}

// PS_EXCHANGE_INTEREST_ITEM_REQ 对齐 IDA 0x140039BF0
inline void operator>>(XPacket& packet, PS_EXCHANGE_INTEREST_ITEM_REQ& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.bAdd;
}

// PS_EXCHANGE_INTEREST_ITEM_RES 对齐 IDA 0x140039BF0
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_INTEREST_ITEM_RES& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwItemID;
    packet.XParse << value.bAdd;
    packet.XParse << value.nResult;
    return packet;
}

// ============================================================================
// 交易所价格更新（依赖邮件系统）
// ============================================================================

// 对齐 IDA 0x1400D9710: 交易所价格更新
struct PS_EXCHANGE_PRICE_HISTORY_UPDATE {
    std::uint32_t dwSellerUCID = 0;        // 卖家UCID
    std::uint32_t dwItemID = 0;            // 物品ID
    std::uint32_t dwExchangeID = 0;        // 交易所ID
    std::int16_t sSellCount = 0;           // 售出数量
    std::uint8_t _pad0[6] = {};
    std::int64_t nPrice_One = 0;           // 单价
    std::int64_t tRegDate = 0;             // 注册日期
    ST_POST_DATA stPost{};                 // 邮件数据
    std::uint16_t wSellerRecvPostCount = 0;// 卖家已收邮件数
    std::uint8_t _pad1[6] = {};
    wchar_t strBuyerName[21] = {};         // 买家名字
};

// 对齐 IDA 0x1400E9EF0
inline void operator>>(XPacket& packet, PS_EXCHANGE_PRICE_HISTORY_UPDATE& value) {
    packet.XParse >> value.dwSellerUCID;
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.dwExchangeID;
    packet.XParse >> value.sSellCount;
    packet.XParse >> value.nPrice_One;
    packet.XParse >> value.tRegDate;
    packet >> value.stPost;
    packet.XParse >> value.wSellerRecvPostCount;
    short sLen = 0;
    packet.XParse.GetWString(value.strBuyerName, 21, sLen);
}

// 对齐 IDA 0x1400E9DD0
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_PRICE_HISTORY_UPDATE& value) {
    packet.XParse << value.dwSellerUCID;
    packet.XParse << value.dwItemID;
    packet.XParse << value.dwExchangeID;
    packet.XParse << value.sSellCount;
    packet.XParse << value.nPrice_One;
    packet.XParse << value.tRegDate;
    packet << value.stPost;
    packet.XParse << value.wSellerRecvPostCount;
    packet.XParse << GreenDamTan_BoundedWideString(value.strBuyerName);
    return packet;
}

// ============================================================================
// 交易所物品上架注册
// ============================================================================

// 对齐 IDA 0x140039D50: 交易所物品上架请求（DB 处理）
struct PS_DB_EXCHANGE_SELL_REGISTER {
    std::uint32_t dwUCID = 0;
    std::int64_t xSerial = 0;
    std::uint32_t dwItemID = 0;
    std::int64_t nPrice_One = 0;
    int nAddHour = 0;
    std::uint8_t byItemType = 0;
    std::uint8_t byItemSubType = 0;
    std::uint8_t byUseClass = 0;
    std::uint8_t byItemLevel = 0;
    std::uint8_t byItemGrade = 0;
    std::uint8_t _pad0[3] = {};
    std::int64_t xSerial_Commission = 0;
    std::int64_t xSerial_Count = 0;
    std::int64_t xSerial_Expire = 0;
    int nWaitTime = 0;
    std::uint8_t _pad1[4] = {};
    std::int64_t nAddCostMoney = 0;
    std::uint8_t byMaxSellCount = 0;
    std::uint8_t _pad2[3] = {};
    std::uint32_t dwExchangeID = 0;
    int nResult = 0;
    std::uint8_t byInvenType = 0;
    bool bCountItem = false;
    std::uint8_t _pad3[2] = {};
    PS_RES_STORAGE_INFO stUpdateItem{};  // size 40
    PS_STORAGE_INFO stCreateItem{};      // size 128
    int nSaleRate = 0;
    wchar_t strSellerName[21] = {};
};

// PS_DB_EXCHANGE_SELL_REGISTER 对齐 IDA 0x140039D50
inline void operator>>(XPacket& packet, PS_DB_EXCHANGE_SELL_REGISTER& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.xSerial;
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.nPrice_One;
    packet.XParse >> value.nAddHour;
    packet.XParse >> value.byItemType;
    packet.XParse >> value.byItemSubType;
    packet.XParse >> value.byUseClass;
    packet.XParse >> value.byItemLevel;
    packet.XParse >> value.byItemGrade;
    packet.XParse >> value.xSerial_Commission;
    packet.XParse >> value.xSerial_Count;
    packet.XParse >> value.xSerial_Expire;
    packet.XParse >> value.nWaitTime;
    packet.XParse >> value.nAddCostMoney;
    packet.XParse >> value.byMaxSellCount;
    packet.XParse >> value.dwExchangeID;
    packet.XParse >> value.nResult;
    packet.XParse >> value.byInvenType;
    packet.XParse >> value.bCountItem;
    packet >> value.stUpdateItem;
    packet >> value.stCreateItem;
    packet.XParse >> value.nSaleRate;
    short sLen = 0;
    packet.XParse.GetWString(value.strSellerName, 21, sLen);
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_EXCHANGE_SELL_REGISTER& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.xSerial;
    packet.XParse << value.dwItemID;
    packet.XParse << value.nPrice_One;
    packet.XParse << value.nAddHour;
    packet.XParse << value.byItemType;
    packet.XParse << value.byItemSubType;
    packet.XParse << value.byUseClass;
    packet.XParse << value.byItemLevel;
    packet.XParse << value.byItemGrade;
    packet.XParse << value.xSerial_Commission;
    packet.XParse << value.xSerial_Count;
    packet.XParse << value.xSerial_Expire;
    packet.XParse << value.nWaitTime;
    packet.XParse << value.nAddCostMoney;
    packet.XParse << value.byMaxSellCount;
    packet.XParse << value.dwExchangeID;
    packet.XParse << value.nResult;
    packet.XParse << value.byInvenType;
    packet.XParse << value.bCountItem;
    packet << value.stUpdateItem;
    packet << value.stCreateItem;
    packet.XParse << value.nSaleRate;
    packet.XParse << GreenDamTan_BoundedWideString(value.strSellerName);
    return packet;
}

// ============================================================================
// 交易所物品购买
// ============================================================================

// 对齐 IDA 0x14003A820: 交易所物品购买请求（DB 处理）
struct PS_DB_EXCHANGE_ITEM_BUY {
    std::uint32_t dwExchangeID = 0;
    std::int16_t shCount = 0;
    std::uint8_t _pad0[2] = {};
    std::uint32_t dwUCID = 0;
    std::uint8_t byItemType = 0;
    std::uint8_t _pad1[3] = {};
    int nResult = 0;
    ST_POST_DATA stPost[2] = {};      // 两个邮件数据（买家和卖家）
    int nRecvCount[2] = {};            // 接收计数
    std::uint32_t dwSellerUCID = 0;
    std::uint8_t _pad2[4] = {};
    std::int64_t nSellPrice = 0;
    std::uint32_t dwSellerUAID = 0;
};

// PS_DB_EXCHANGE_ITEM_BUY 对齐 IDA 0x14003A820
inline void operator>>(XPacket& packet, PS_DB_EXCHANGE_ITEM_BUY& value) {
    packet.XParse >> value.dwExchangeID;
    packet.XParse >> value.shCount;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byItemType;
    packet.XParse >> value.nResult;
    packet >> value.stPost[0];
    packet >> value.stPost[1];
    packet.XParse >> value.nRecvCount[0];
    packet.XParse >> value.nRecvCount[1];
    packet.XParse >> value.dwSellerUCID;
    packet.XParse >> value.nSellPrice;
    packet.XParse >> value.dwSellerUAID;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_EXCHANGE_ITEM_BUY& value) {
    packet.XParse << value.dwExchangeID;
    packet.XParse << value.shCount;
    packet.XParse << value.dwUCID;
    packet.XParse << value.byItemType;
    packet.XParse << value.nResult;
    packet << value.stPost[0];
    packet << value.stPost[1];
    packet.XParse << value.nRecvCount[0];
    packet.XParse << value.nRecvCount[1];
    packet.XParse << value.dwSellerUCID;
    packet.XParse << value.nSellPrice;
    packet.XParse << value.dwSellerUAID;
    return packet;
}

// ============================================================================
// 交易所物品召回
// ============================================================================

// 对齐 IDA 0x14003B0F0: 交易所物品召回请求（DB 处理）
struct PS_DB_EXCHANGE_ITEM_RECALL_REQ {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwExchangeID = 0;
    std::int64_t xSerial = 0;
    std::uint8_t byType = 0;
    std::uint8_t _pad0[7] = {};
    std::int64_t nPostSerial = 0;
    std::uint8_t byPostSubType = 0;
};

// 对齐 IDA 0x14003B0F0: 交易所物品召回响应（DB 处理）
struct PS_DB_EXCHANGE_ITEM_RECALL_RES {
    int nResult = 0;
    std::uint8_t _pad0[4] = {};
    ST_POST_DATA stPost{};
    std::uint16_t wPostCount = 0;
};

// PS_DB_EXCHANGE_ITEM_RECALL_REQ 序列化
inline void operator>>(XPacket& packet, PS_DB_EXCHANGE_ITEM_RECALL_REQ& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwExchangeID;
    packet.XParse >> value.xSerial;
    packet.XParse >> value.byType;
    packet.XParse >> value.nPostSerial;
    packet.XParse >> value.byPostSubType;
}

// PS_DB_EXCHANGE_ITEM_RECALL_RES 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_EXCHANGE_ITEM_RECALL_RES& value) {
    packet.XParse << value.nResult;
    packet << value.stPost;
    packet.XParse << value.wPostCount;
    return packet;
}
