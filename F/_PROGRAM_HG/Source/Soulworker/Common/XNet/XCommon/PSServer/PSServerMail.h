#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerMail.h - 邮件系统相关结构体及序列化运算符
//
// 此文件包含邮件系统的所有数据结构和序列化操作：
// - 邮件数据结构
// - 账号邮件/角色邮件
// - 邮件删除/领取
// - GM工具邮件(GMT Post)
// - 头像照片系统
// ============================================================================

// ============================================================
// 邮件基础结构体
// ============================================================

// 对齐 IDA 0x1400E2310: 邮件发送者角色信息
struct ST_POST_CHAR {
    std::uint32_t dwUCID = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad0[2] = {};
    std::uint32_t dwProfilePhotoID = 0;
    wchar_t strName[21] = {};
};

// 对齐 IDA 0x1400E25D0: 邮件数据（含附件物品）
struct ST_POST_DATA {
    std::int64_t biSerial = 0;             // 邮件序列号
    ST_POST_CHAR stCharInfo{};             // 发送者信息
    wchar_t strTitle[41] = {};             // 邮件标题
    wchar_t strMsg[401] = {};              // 邮件内容
    std::int64_t biMoney = 0;              // 附加金钱
    STItem stItemList[5] = {};             // 附加物品列表
    std::int64_t nRegTime = 0;             // 注册时间
    std::uint8_t byFlag = 0;               // 标记
    std::uint8_t byPostType = 0;           // 邮件类型
    std::uint8_t byPostSubType = 0;        // 邮件子类型
    std::uint8_t _pad1[5] = {};
    std::int64_t nRemainTime = 0;          // 剩余时间
    std::int64_t biEventID = 0;            // 活动ID
    PS_ITEM_SOCKET_LIST vecSocketList{};   // 镶嵌列表
    PS_ITEM_BROACH_LIST vecBroachList{};   // 镂刻列表
    PS_ITEM_PACKAGE_LIST vecPackageList{}; // 套装列表
};

// 对齐 IDA 0x1400E2780: 账号邮件数据（用于账号级邮件列表）
// Size: 1672 bytes (0x688)
struct ST_ACCOUNT_POST_DATA {
    std::int64_t biSerial = 0;             // offset 0, 邮件序列号
    std::uint32_t dwUAID = 0;              // offset 8, 账号ID
    std::uint8_t _pad0[4] = {};            // offset 12, padding
    wchar_t szTitle[41] = {};              // offset 16, 邮件标题 (实际 offset 12, size 82)
    wchar_t szMsg[401] = {};               // offset 98, 邮件内容 (实际 offset 94, size 802)
    std::int64_t biMoney = 0;              // offset 896, 附加金钱
    STItem stItemList[5] = {};             // offset 904, 附加物品列表 (5 * 120 = 600 bytes)
    std::int64_t biRegTime = 0;            // offset 1504, 注册时间
    std::int64_t biRemainTime = 0;         // offset 1512, 剩余时间
    std::uint8_t byFlag = 0;               // offset 1520, 标记
    std::uint8_t _pad1 = 0;                // offset 1521, padding
    wchar_t szSendName[21] = {};           // offset 1522, 发送者名字 (42 bytes)
    std::uint8_t byMainType = 0;           // offset 1564, 主类型 (设为4表示账号邮件)
    std::uint8_t bySubType = 0;            // offset 1565, 子类型
    std::uint8_t _pad2[2] = {};            // offset 1566, padding
    std::int64_t biDelDate = 0;            // offset 1568, 删除日期
    PS_ITEM_SOCKET_LIST vecSocketList{};   // offset 1576, 镶嵌列表 (32 bytes)
    PS_ITEM_BROACH_LIST vecBroachList{};   // offset 1608, 镂刻列表 (32 bytes)
    PS_ITEM_PACKAGE_LIST vecPackageList{}; // offset 1640, 套装列表 (32 bytes)
};

// 对齐 IDA 0x1400E8E10: 账号邮件列表
struct PS_ACCOUNT_POST_LIST {
    std::vector<ST_ACCOUNT_POST_DATA> vecAccountPostList;
};

// TODO: 推测结果 - 对齐 IDA 0x1400CD810 ResPostReceipt / 0x1400CDC60 ResAccountPostReceipt 使用
// Size: 16 bytes
struct PS_LOG_ITEM {
    std::int32_t nItemID = 0;
    std::int16_t shCount = 0;
    std::int16_t _pad0 = 0;
    std::int64_t biSerial = 0;
};

struct PS_LOG_ITEM_LIST {
    std::vector<PS_LOG_ITEM> vecLogItem;
};

// ============================================================
// Post Delete 相关结构体
// ============================================================

// 对齐 IDA: 邮件删除信息 (16 bytes)
struct PS_POST_DELETE_INFO {
    std::int64_t biSerial = 0;      // 邮件序列号
    std::int64_t biDeleteDate = 0;  // 删除日期
};

// 对齐 IDA: 邮件删除列表 (32 bytes)
struct PS_POST_DELETE_LIST {
    std::vector<PS_POST_DELETE_INFO> vecInfo;
};

// 对齐 IDA 0x1400E9BF0: 批量删除邮件请求 (88 bytes)
struct PS_POST_DELETE_ALL_SERVER {
    std::uint32_t dwUAID = 0;               // 账号ID
    std::uint32_t dwUCID = 0;               // 角色ID
    std::uint16_t wPostCount = 0;           // 邮件计数
    std::uint16_t _pad0 = 0;
    std::int32_t nErrorCode = 0;            // 错误码
    std::uint8_t byPostType = 0;            // 邮件类型: 0=收件箱, 1=发件箱, 2=已读, 3=账号邮件
    std::uint8_t _pad1[7] = {};
    PS_POST_DELETE_LIST psDeleteList;       // 删除列表
    PS_POST_DELETE_LIST psFailedList;       // 失败列表
};

// ============================================================
// Post Level Up Event 相关结构体
// ============================================================

// 对齐 IDA: 升级活动事件信息 (12 bytes)
struct ST_POST_LEVEL_UP_EVENT_INFO {
    std::int32_t nGroup = 0;    // 分组ID
    std::int32_t nLv = 0;       // 等级
    std::int32_t nUCID = 0;     // 角色ID
};

// 对齐 IDA: 升级活动事件列表 (32 bytes)
struct PS_POST_LEVEL_UP_EVENT_INFO_VEC {
    std::vector<ST_POST_LEVEL_UP_EVENT_INFO> vecInfo;
};

// 对齐 IDA: 升级活动事件更新 (16 bytes)
struct PS_POST_LEVEL_UP_EVENT_UPDATE {
    std::int32_t nUAID = 0;     // 账号ID
    std::int32_t nGroup = 0;    // 分组ID
    std::int32_t nLv = 0;       // 等级
    std::int32_t nUCID = 0;     // 角色ID
};

// PS_POST_LEVEL_UP_EVENT_UPDATE 反序列化
inline void operator>>(XPacket& packet, PS_POST_LEVEL_UP_EVENT_UPDATE& value) {
    packet.XParse >> value.nUAID;
    packet.XParse >> value.nGroup;
    packet.XParse >> value.nLv;
    packet.XParse >> value.nUCID;
}

// ============================================================
// Post Receipt All / 头像照片相关结构体
// ============================================================

// 对齐 IDA: 头像照片信息 (16 bytes)
struct ST_PROFILE_PHOTO_INFO {
    std::uint32_t dwPhotoID = 0;      // 头像ID
    std::uint8_t byPeriodType = 0;    // 周期类型
    std::uint8_t byFavorite = 0;      // 是否收藏
    std::uint8_t byState = 0;         // 状态
    std::uint8_t _pad0 = 0;           // 对齐填充
    std::int64_t nEndDate = 0;        // 结束日期
};

// ST_PROFILE_PHOTO_INFO 序列化运算符
inline void operator>>(XPacket& packet, ST_PROFILE_PHOTO_INFO& value) {
    packet.XParse >> value.dwPhotoID;
    packet.XParse >> value.byPeriodType;
    packet.XParse >> value.byFavorite;
    packet.XParse >> value.byState;
    packet.XParse >> value._pad0;
    packet.XParse >> value.nEndDate;
}

inline XPacket& operator<<(XPacket& packet, const ST_PROFILE_PHOTO_INFO& value) {
    packet.XParse << value.dwPhotoID;
    packet.XParse << value.byPeriodType;
    packet.XParse << value.byFavorite;
    packet.XParse << value.byState;
    packet.XParse << value._pad0;
    packet.XParse << value.nEndDate;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_PROFILE_PHOTO_INFO& value) {
    packet.XParse << value.dwPhotoID;
    packet.XParse << value.byPeriodType;
    packet.XParse << value.byFavorite;
    packet.XParse << value.byState;
    packet.XParse << value._pad0;
    packet.XParse << value.nEndDate;
    return packet;
}

// 对齐 IDA 0x140028640: 头像照片加载请求/响应结构
struct PS_PROFILE_PHOTO_LOAD {
    std::uint32_t dwUCID = 0;              // 角色ID
    std::vector<ST_PROFILE_PHOTO_INFO> vecList;  // 照片列表
};

// 对齐 IDA 0x140028B70: 头像照片更新请求/响应结构
struct PS_DB_PROFILE_PHOTO_UPDATE {
    std::uint32_t dwUCID = 0;              // 角色ID
    ST_PROFILE_PHOTO_INFO stInfo;          // 照片信息
    std::int32_t nError = 0;               // 错误码
};

// 对齐 IDA PS_DB_PROFILE_PHOTO_CHANGE (48 bytes)
struct PS_DB_PROFILE_PHOTO_CHANGE {
    std::uint32_t dwUCID = 0;              // offset 0x00: 角色ID
    std::uint8_t _pad0[4] = {};            // offset 0x04: padding
    ST_PROFILE_PHOTO_INFO stNewPhotoInfo{}; // offset 0x08: 新照片信息 (16 bytes)
    ST_PROFILE_PHOTO_INFO stOldPhotoInfo{}; // offset 0x18: 旧照片信息 (16 bytes)
    std::int32_t nError = 0;               // offset 0x28: 错误码
};
static_assert(sizeof(PS_DB_PROFILE_PHOTO_CHANGE) == 48, "PS_DB_PROFILE_PHOTO_CHANGE size must match IDA");

// PS_DB_PROFILE_PHOTO_CHANGE 反序列化
inline void operator>>(XPacket& packet, PS_DB_PROFILE_PHOTO_CHANGE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet >> value.stNewPhotoInfo;
    packet >> value.stOldPhotoInfo;
    packet.XParse >> value.nError;
}

// 对齐 IDA 0x1400288D0: 头像照片添加请求/响应结构 (88 bytes)
struct PS_DB_PROFILE_PHOTO_ADD {
    std::uint32_t dwUCID = 0;              // offset 0x00: 角色ID (4 bytes)
    std::uint8_t _pad0[4] = {};            // offset 0x04: padding (4 bytes)
    PS_RES_STORAGE_INFO psUpdateItemList{}; // offset 0x08: 物品更新列表 (40 bytes)
    ST_PROFILE_PHOTO_INFO stInfo{};        // offset 0x30: 照片信息 (16 bytes)
    std::uint32_t dwItemID = 0;            // offset 0x40: 物品ID (4 bytes)
    std::uint8_t _pad1[4] = {};            // offset 0x44: padding (4 bytes)
    std::int64_t biSerial = 0;             // offset 0x48: 序列号 (8 bytes)
    std::int32_t nError = 0;               // offset 0x50: 错误码 (4 bytes)
};

static_assert(sizeof(PS_DB_PROFILE_PHOTO_ADD) == 88, "PS_DB_PROFILE_PHOTO_ADD size must match IDA");

// 对齐 IDA: 批量领取邮件请求
struct PS_POST_RECEIPT_ALL_SERVER {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::int64_t biSerial = 0;
    std::uint8_t byFlag = 0;
    std::uint8_t byPostType = 0;
    std::uint8_t bDecrease = 0;
    std::uint8_t _pad0 = 0;
    std::int64_t biRemainTime = 0;
    std::uint16_t wPostCount = 0;
    std::int32_t nErrorCode = 0;
    PS_RES_STORAGE_INFO psCreateItem;
    PS_RES_STORAGE_INFO psUpdateItem;
    PS_RES_STORAGE_INFO psUpdateSerial;
    std::vector<ST_PROFILE_PHOTO_INFO> stAppearanceList;
};

// PS_POST_RECEIPT_ALL_SERVER 反序列化
inline void operator>>(XPacket& packet, PS_POST_RECEIPT_ALL_SERVER& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.biSerial;
    packet.XParse >> value.byFlag;
    packet.XParse >> value.byPostType;
    packet.XParse >> value.bDecrease;
    packet.XParse >> value.biRemainTime;
    packet.XParse >> value.wPostCount;
    packet.XParse >> value.nErrorCode;
    packet >> value.psCreateItem;
    packet >> value.psUpdateItem;
    packet >> value.psUpdateSerial;
}

// ============================================================
// GMT Post 相关结构体 (GM工具邮件系统)
// ============================================================

/**
 * @brief 系统邮件物品项 - 24 bytes
 * 来自 IDA: ST_SYSTEM_POST_ITEM
 */
struct ST_SYSTEM_POST_ITEM {
    std::int32_t nItemID = 0;      // +0x00: 物品ID (4 bytes)
    std::int16_t shCount = 0;     // +0x04: 数量 (2 bytes)
    std::int16_t _pad0 = 0;       // +0x06: padding (2 bytes)
    std::int32_t nAttack = 0;     // +0x08: 攻击力 (4 bytes)
    std::int32_t nDefense = 0;    // +0x0C: 防御力 (4 bytes)
    std::int32_t nTitleID = 0;    // +0x10: 称号ID (4 bytes)
    std::int32_t nDyeID = 0;      // +0x14: 染色ID (4 bytes)
};

static_assert(sizeof(ST_SYSTEM_POST_ITEM) == 24, "ST_SYSTEM_POST_ITEM size must match IDA");

/**
 * @brief 系统邮件结构 - 1064 bytes
 * 来自 IDA: ST_SYSTEM_POST
 */
struct ST_SYSTEM_POST {
    std::uint8_t byPostType = 0;              // +0x00: 邮件类型 (1 byte)
    std::uint8_t byPostSubType = 0;           // +0x01: 邮件子类型 (1 byte)
    wchar_t strTitle[41] = {};                // +0x02: 邮件标题 (82 bytes)
    wchar_t strMsg[401] = {};                 // +0x54: 邮件内容 (802 bytes)
    wchar_t strName[21] = {};                 // +0x376: 发送者名字 (42 bytes)
    ST_SYSTEM_POST_ITEM stSysItem[5] = {};    // +0x3A0: 物品列表 (5 * 24 = 120 bytes)
    std::int64_t biMoney = 0;                 // +0x418: 金钱 (8 bytes)
    std::int64_t _pad0 = 0;                   // +0x420: padding (8 bytes)
};

static_assert(sizeof(ST_SYSTEM_POST) == 1064, "ST_SYSTEM_POST size must match IDA");

/**
 * @brief GMT邮件条件 - 12 bytes
 * 来自 IDA: ST_GMT_POST_CONDITION
 */
struct ST_GMT_POST_CONDITION {
    std::uint8_t byConditionType = 0;   // +0x00: 条件类型 (1 byte)
    std::uint8_t _pad0[3] = {};         // +0x01: padding (3 bytes)
    std::int32_t nMin = 0;              // +0x04: 最小值 (4 bytes)
    std::int32_t nMax = 0;              // +0x08: 最大值 (4 bytes)
};

static_assert(sizeof(ST_GMT_POST_CONDITION) == 12, "ST_GMT_POST_CONDITION size must match IDA");

/**
 * @brief GMT邮件信息 - 1784 bytes
 * 来自 IDA: ST_GMT_POST_INFO
 */
struct ST_GMT_POST_INFO {
    std::int64_t biNo = 0;                        // +0x00: 编号 (8 bytes)
    std::uint32_t dwUCID = 0;                     // +0x08: 角色ID (4 bytes)
    std::uint8_t bySystemMailSubType = 0;        // +0x0C: 系统邮件子类型 (1 byte)
    std::uint8_t byPostSubType = 0;               // +0x0D: 邮件子类型 (1 byte)
    wchar_t strTitle[41] = {};                    // +0x0E: 邮件标题 (82 bytes)
    wchar_t strMsg[401] = {};                     // +0x60: 邮件内容 (802 bytes)
    ST_SYSTEM_POST_ITEM stSysItem[5] = {};       // +0x384: 系统物品列表 (120 bytes)
    std::int64_t biItemSerial[5] = {};           // +0x3FC: 物品序列号 (40 bytes)
    bool bSendPost = false;                       // +0x424: 是否发送 (1 byte)
    std::uint8_t _pad0[7] = {};                   // +0x425: padding (7 bytes)
    std::int64_t biPostSerial = 0;                // +0x42C: 邮件序列号 (8 bytes)
    std::int64_t biGold = 0;                      // +0x434: 金钱 (8 bytes)
    wchar_t strName[21] = {};                     // +0x43C: 发送者名字 (42 bytes)
    std::int64_t biDelDate = 0;                   // +0x466: 删除日期 (8 bytes)
    STItem stItem[5] = {};                        // +0x46E: 物品列表 (600 bytes)
    ST_GMT_POST_CONDITION stCondition[3] = {};   // +0x6C4: 条件列表 (36 bytes)
};

static_assert(sizeof(ST_GMT_POST_INFO) == 1784, "ST_GMT_POST_INFO size must match IDA");

/**
 * @brief GMT邮件发送数据 - 1848 bytes
 * 来自 IDA: ST_GMT_POST_SEND
 */
struct ST_GMT_POST_SEND {
    ST_GMT_POST_INFO stPost{};                    // +0x000: 邮件信息 (1784 bytes)
    std::int64_t biNo = 0;                        // +0x6F8: 编号 (8 bytes)
    std::int64_t biRegTime = 0;                   // +0x700: 注册时间 (8 bytes)
    std::uint16_t wPostCount = 0;                 // +0x708: 邮件计数 (2 bytes)
    std::uint8_t _pad0[6] = {};                   // +0x70A: padding (6 bytes)
    std::int64_t biPostSerial = 0;                // +0x710: 邮件序列号 (8 bytes)
    std::vector<ST_POST_DATA> vecPostData;        // +0x718: 邮件数据列表 (32 bytes)
};

static_assert(sizeof(ST_GMT_POST_SEND) == 1848, "ST_GMT_POST_SEND size must match IDA");

/**
 * @brief GMT邮件列表请求/响应
 * 来自 IDA: PS_GMT_POST_LIST
 */
struct PS_GMT_POST_LIST {
    std::vector<ST_GMT_POST_INFO> vecPostList;    // +0x00: 邮件列表 (32 bytes)
    std::int32_t nRefreshPostType = 0;            // +0x20: 刷新类型 (4 bytes)
    bool bLast = false;                           // +0x24: 是否最后 (1 byte)
};

static_assert(sizeof(PS_GMT_POST_LIST) == 40, "PS_GMT_POST_LIST size must match IDA");

/**
 * @brief GMT邮件发送列表
 * 来自 IDA: PS_GMT_POST_SEND_LIST
 */
struct PS_GMT_POST_SEND_LIST {
    std::uint32_t dwUCID = 0;                     // +0x00: 角色ID (4 bytes)
    std::uint8_t _pad0[4] = {};                   // +0x04: padding (4 bytes)
    std::vector<ST_GMT_POST_SEND> vecPostList;    // +0x08: 邮件发送列表 (32 bytes)
    std::int32_t nRefreshPostType = 0;            // +0x28: 刷新类型 (4 bytes)
    bool bLast = false;                           // +0x2C: 是否最后 (1 byte)
};

static_assert(sizeof(PS_GMT_POST_SEND_LIST) == 48, "PS_GMT_POST_SEND_LIST size must match IDA");

/**
 * @brief 邮件列表
 * 来自 IDA: ST_POST_LIST
 */
struct ST_POST_LIST {
    std::vector<ST_POST_DATA> vecData;
};

static_assert(sizeof(ST_POST_LIST) == 32, "ST_POST_LIST size must match IDA");

// ============================================================================
// 邮件系统序列化运算符
// ============================================================================

// PS_LOG_ITEM 序列化
inline void operator>>(XPacket& packet, PS_LOG_ITEM& value) {
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse >> value._pad0;
    packet.XParse >> value.biSerial;
}

inline XPacket& operator<<(XPacket& packet, const PS_LOG_ITEM& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value._pad0;
    packet.XParse << value.biSerial;
    return packet;
}

inline void operator>>(XPacket& packet, PS_LOG_ITEM_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecLogItem.clear();
    value.vecLogItem.reserve(count);
    for (std::uint8_t i = 0; i < count; ++i) {
        PS_LOG_ITEM item{};
        packet >> item;
        value.vecLogItem.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_LOG_ITEM_LIST& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecLogItem.size());
    for (const PS_LOG_ITEM& item : value.vecLogItem) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA 0x1400E2310
inline void operator>>(XPacket& packet, ST_POST_CHAR& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse >> value.dwProfilePhotoID;
    short sLen = 0;
    packet.XParse.GetWString(value.strName, 21, sLen);
}

inline XPacket& operator<<(XPacket& packet, const ST_POST_CHAR& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    return packet;
}

// 对齐 IDA 0x1400E25D0
inline void operator>>(XPacket& packet, ST_POST_DATA& value) {
    packet.XParse >> value.biSerial;
    packet >> value.stCharInfo;
    short sLen = 0;
    packet.XParse.GetWString(value.strTitle, 41, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.strMsg, 401, sLen);
    packet.XParse >> value.biMoney;
    for (int i = 0; i < 5; ++i) {
        packet >> value.stItemList[i];
    }
    packet.XParse >> value.nRegTime;
    packet.XParse >> value.byFlag;
    packet.XParse >> value.byPostType;
    packet.XParse >> value.byPostSubType;
    packet.XParse >> value.nRemainTime;
    packet.XParse >> value.biEventID;
    packet >> value.vecSocketList;
    packet >> value.vecBroachList;
    packet >> value.vecPackageList;
}

inline XPacket& operator<<(XPacket& packet, const ST_POST_DATA& value) {
    packet.XParse << value.biSerial;
    packet << value.stCharInfo;
    packet.XParse << GreenDamTan_BoundedWideString(value.strTitle);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    packet.XParse << value.biMoney;
    for (int i = 0; i < 5; ++i) {
        packet << value.stItemList[i];
    }
    packet.XParse << value.nRegTime;
    packet.XParse << value.byFlag;
    packet.XParse << value.byPostType;
    packet.XParse << value.byPostSubType;
    packet.XParse << value.nRemainTime;
    packet.XParse << value.biEventID;
    packet << value.vecSocketList;
    packet << value.vecBroachList;
    packet << value.vecPackageList;
    return packet;
}

// 对齐 IDA 0x1400E2780: 账号邮件数据序列化
inline XPacket& operator<<(XPacket& packet, const ST_ACCOUNT_POST_DATA& value) {
    packet.XParse << value.biSerial;
    packet.XParse << value.dwUAID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szTitle);
    packet.XParse << GreenDamTan_BoundedWideString(value.szMsg);
    packet.XParse << value.biMoney;
    for (int i = 0; i < 5; ++i) {
        packet << value.stItemList[i];
    }
    packet.XParse << value.biRegTime;
    packet.XParse << value.biRemainTime;
    packet.XParse << value.byFlag;
    packet.XParse << GreenDamTan_BoundedWideString(value.szSendName);
    packet.XParse << value.byMainType;
    packet.XParse << value.bySubType;
    packet.XParse << value.biDelDate;
    packet << value.vecSocketList;
    packet << value.vecBroachList;
    packet << value.vecPackageList;
    return packet;
}

// 对齐 IDA 0x1400E8E10: 账号邮件列表序列化
inline XPacket& operator<<(XPacket& packet, const PS_ACCOUNT_POST_LIST& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecAccountPostList.size(), 0xFFFF));
    packet.XParse << count;
    for (const auto& item : value.vecAccountPostList) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA: 账号邮件列表反序列化
inline void operator>>(XPacket& packet, PS_ACCOUNT_POST_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecAccountPostList.clear();
    value.vecAccountPostList.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        ST_ACCOUNT_POST_DATA item{};
        packet.XParse >> item.biSerial;
        packet.XParse >> item.dwUAID;
        short sLen = 0;
        packet.XParse.GetWString(item.szTitle, 41, sLen);
        sLen = 0;
        packet.XParse.GetWString(item.szMsg, 401, sLen);
        packet.XParse >> item.biMoney;
        for (int j = 0; j < 5; ++j) {
            packet >> item.stItemList[j];
        }
        packet.XParse >> item.biRegTime;
        packet.XParse >> item.biRemainTime;
        packet.XParse >> item.byFlag;
        sLen = 0;
        packet.XParse.GetWString(item.szSendName, 21, sLen);
        packet.XParse >> item.byMainType;
        packet.XParse >> item.bySubType;
        packet.XParse >> item.biDelDate;
        packet >> item.vecSocketList;
        packet >> item.vecBroachList;
        packet >> item.vecPackageList;
        value.vecAccountPostList.push_back(item);
    }
}

// 对齐 IDA: 单个账号邮件数据反序列化
inline void operator>>(XPacket& packet, ST_ACCOUNT_POST_DATA& value) {
    packet.XParse >> value.biSerial;
    packet.XParse >> value.dwUAID;
    short sLen = 0;
    packet.XParse.GetWString(value.szTitle, 41, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.szMsg, 401, sLen);
    packet.XParse >> value.biMoney;
    for (int j = 0; j < 5; ++j) {
        packet >> value.stItemList[j];
    }
    packet.XParse >> value.biRegTime;
    packet.XParse >> value.biRemainTime;
    packet.XParse >> value.byFlag;
    sLen = 0;
    packet.XParse.GetWString(value.szSendName, 21, sLen);
    packet.XParse >> value.byMainType;
    packet.XParse >> value.bySubType;
    packet.XParse >> value.biDelDate;
    packet >> value.vecSocketList;
    packet >> value.vecBroachList;
    packet >> value.vecPackageList;
}

// Post Delete 序列化
inline XPacket& operator<<(XPacket& packet, const PS_POST_DELETE_INFO& value) {
    packet.XParse << value.biSerial;
    packet.XParse << value.biDeleteDate;
    return packet;
}

inline void operator>>(XPacket& packet, PS_POST_DELETE_INFO& value) {
    packet.XParse >> value.biSerial;
    packet.XParse >> value.biDeleteDate;
}

inline XPacket& operator<<(XPacket& packet, const PS_POST_DELETE_LIST& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecInfo.size(), 0xFFFF));
    packet.XParse << count;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_POST_DELETE_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        PS_POST_DELETE_INFO item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_POST_DELETE_ALL_SERVER& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.wPostCount;
    packet.XParse << value.nErrorCode;
    packet.XParse << value.byPostType;
    packet << value.psDeleteList;
    packet << value.psFailedList;
    return packet;
}

inline void operator>>(XPacket& packet, PS_POST_DELETE_ALL_SERVER& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.wPostCount;
    packet.XParse >> value.nErrorCode;
    packet.XParse >> value.byPostType;
    packet >> value.psDeleteList;
    packet >> value.psFailedList;
}

// Post Level Up Event 序列化
inline XPacket& operator<<(XPacket& packet, const ST_POST_LEVEL_UP_EVENT_INFO& value) {
    packet.XParse << value.nGroup;
    packet.XParse << value.nLv;
    packet.XParse << value.nUCID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_POST_LEVEL_UP_EVENT_INFO& value) {
    packet.XParse >> value.nGroup;
    packet.XParse >> value.nLv;
    packet.XParse >> value.nUCID;
}

inline XPacket& operator<<(XPacket& packet, const PS_POST_LEVEL_UP_EVENT_INFO_VEC& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecInfo.size(), 0xFFFF));
    packet.XParse << count;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_POST_LEVEL_UP_EVENT_INFO_VEC& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        ST_POST_LEVEL_UP_EVENT_INFO item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

// PS_PROFILE_PHOTO_LOAD 序列化操作符
inline XPacket& operator>>(XPacket& packet, PS_PROFILE_PHOTO_LOAD& value) {
    packet.XParse >> value.dwUCID;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_PROFILE_PHOTO_LOAD& value) {
    // 向量序列化
    std::int16_t nCount = static_cast<std::int16_t>(value.vecList.size());
    packet.XParse << nCount;
    for (const auto& item : value.vecList) {
        packet << item;
    }
    return packet;
}

// PS_DB_PROFILE_PHOTO_UPDATE 序列化操作符
inline XPacket& operator>>(XPacket& packet, PS_DB_PROFILE_PHOTO_UPDATE& value) {
    packet.XParse >> value.dwUCID;
    packet >> value.stInfo;
    packet.XParse >> value.nError;
    return packet;
}

// PS_DB_PROFILE_PHOTO_ADD 序列化操作符 (对齐 IDA 88 bytes)
inline XPacket& operator>>(XPacket& packet, PS_DB_PROFILE_PHOTO_ADD& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet >> value.psUpdateItemList;
    packet >> value.stInfo;
    packet.XParse >> value.dwItemID;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad1), sizeof(value._pad1));
    packet.XParse >> value.biSerial;
    packet.XParse >> value.nError;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_PROFILE_PHOTO_ADD& value) {
    packet.XParse << value.dwUCID;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    packet << value.psUpdateItemList;
    packet << value.stInfo;
    packet.XParse << value.dwItemID;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad1)), sizeof(value._pad1));
    packet.XParse << value.biSerial;
    packet.XParse << value.nError;
    return packet;
}

// GMT Post 序列化操作符
inline XPacket& operator<<(XPacket& packet, const ST_SYSTEM_POST_ITEM& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value.nAttack;
    packet.XParse << value.nDefense;
    packet.XParse << value.nTitleID;
    packet.XParse << value.nDyeID;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_SYSTEM_POST_ITEM& value) {
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse >> value.nAttack;
    packet.XParse >> value.nDefense;
    packet.XParse >> value.nTitleID;
    packet.XParse >> value.nDyeID;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_SYSTEM_POST& value) {
    packet.XParse << value.byPostType;
    packet.XParse << value.byPostSubType;
    packet.XParse << GreenDamTan_BoundedWideString(value.strTitle);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    for (int i = 0; i < 5; ++i) packet << value.stSysItem[i];
    packet.XParse << value.biMoney;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_SYSTEM_POST& value) {
    packet.XParse >> value.byPostType;
    packet.XParse >> value.byPostSubType;
    std::int16_t sLen = 0;
    packet.XParse.GetWString(value.strTitle, 41, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.strMsg, 401, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.strName, 21, sLen);
    for (int i = 0; i < 5; ++i) packet >> value.stSysItem[i];
    packet.XParse >> value.biMoney;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_GMT_POST_CONDITION& value) {
    packet.XParse << value.byConditionType;
    packet.XParse << value.nMin;
    packet.XParse << value.nMax;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_GMT_POST_CONDITION& value) {
    packet.XParse >> value.byConditionType;
    packet.XParse >> value.nMin;
    packet.XParse >> value.nMax;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_GMT_POST_INFO& value) {
    packet.XParse << value.biNo;
    packet.XParse << value.dwUCID;
    packet.XParse << value.bySystemMailSubType;
    packet.XParse << value.byPostSubType;
    packet.XParse << GreenDamTan_BoundedWideString(value.strTitle);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    for (int i = 0; i < 5; ++i) packet << value.stSysItem[i];
    for (int i = 0; i < 5; ++i) packet.XParse << value.biItemSerial[i];
    packet.XParse << static_cast<std::uint8_t>(value.bSendPost ? 1 : 0);
    packet.XParse << value.biPostSerial;
    packet.XParse << value.biGold;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.biDelDate;
    for (int i = 0; i < 5; ++i) packet << value.stItem[i];
    for (int i = 0; i < 3; ++i) packet << value.stCondition[i];
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_GMT_POST_INFO& value) {
    packet.XParse >> value.biNo;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.bySystemMailSubType;
    packet.XParse >> value.byPostSubType;
    std::int16_t sLen = 0;
    packet.XParse.GetWString(value.strTitle, 41, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.strMsg, 401, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.strName, 21, sLen);
    for (int i = 0; i < 5; ++i) packet >> value.stSysItem[i];
    for (int i = 0; i < 5; ++i) packet.XParse >> value.biItemSerial[i];
    std::uint8_t bSendPost = 0;
    packet.XParse >> bSendPost;
    value.bSendPost = (bSendPost != 0);
    packet.XParse >> value.biPostSerial;
    packet.XParse >> value.biGold;
    packet.XParse >> value.biDelDate;
    for (int i = 0; i < 5; ++i) packet >> value.stItem[i];
    for (int i = 0; i < 3; ++i) packet >> value.stCondition[i];
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_GMT_POST_SEND& value) {
    packet << value.stPost;
    packet.XParse << value.biNo;
    packet.XParse << value.biRegTime;
    packet.XParse << value.wPostCount;
    packet.XParse << value.biPostSerial;
    // vecPostData 序列化
    packet.XParse << static_cast<std::int16_t>(value.vecPostData.size());
    for (const auto& data : value.vecPostData) {
        packet << data;
    }
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_GMT_POST_SEND& value) {
    packet >> value.stPost;
    packet.XParse >> value.biNo;
    packet.XParse >> value.biRegTime;
    packet.XParse >> value.wPostCount;
    packet.XParse >> value.biPostSerial;
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecPostData.clear();
    value.vecPostData.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_POST_DATA data;
        packet >> data;
        value.vecPostData.push_back(data);
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_GMT_POST_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecPostList.size());
    for (const auto& info : value.vecPostList) {
        packet << info;
    }
    packet.XParse << value.nRefreshPostType;
    packet.XParse << static_cast<std::uint8_t>(value.bLast ? 1 : 0);
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_GMT_POST_LIST& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecPostList.clear();
    value.vecPostList.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_GMT_POST_INFO info;
        packet >> info;
        value.vecPostList.push_back(info);
    }
    packet.XParse >> value.nRefreshPostType;
    std::uint8_t bLast = 0;
    packet.XParse >> bLast;
    value.bLast = (bLast != 0);
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_GMT_POST_SEND_LIST& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << static_cast<std::int16_t>(value.vecPostList.size());
    for (const auto& send : value.vecPostList) {
        packet << send;
    }
    packet.XParse << value.nRefreshPostType;
    packet.XParse << static_cast<std::uint8_t>(value.bLast ? 1 : 0);
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_GMT_POST_SEND_LIST& value) {
    packet.XParse >> value.dwUCID;
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecPostList.clear();
    value.vecPostList.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_GMT_POST_SEND send;
        packet >> send;
        value.vecPostList.push_back(send);
    }
    packet.XParse >> value.nRefreshPostType;
    std::uint8_t bLast = 0;
    packet.XParse >> bLast;
    value.bLast = (bLast != 0);
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_POST_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecData.size());
    for (const auto& data : value.vecData) {
        packet << data;
    }
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_POST_LIST& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecData.clear();
    value.vecData.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_POST_DATA data;
        packet >> data;
        value.vecData.push_back(data);
    }
    return packet;
}

// ============================================================================
// DBAgent 需要的邮件序列化运算符
// ============================================================================

// PS_POST_RECEIPT_ALL_SERVER 序列化运算符
inline XPacket& operator<<(XPacket& packet, const PS_POST_RECEIPT_ALL_SERVER& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.biSerial;
    packet.XParse << value.byFlag;
    packet.XParse << value.byPostType;
    packet.XParse << value.bDecrease;
    packet.XParse << value.biRemainTime;
    packet.XParse << value.wPostCount;
    packet.XParse << value.nErrorCode;
    packet << value.psCreateItem;
    packet << value.psUpdateItem;
    packet << value.psUpdateSerial;
    packet.XParse << static_cast<std::int16_t>(value.stAppearanceList.size());
    for (const auto& item : value.stAppearanceList) {
        packet << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_POST_RECEIPT_ALL_SERVER& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.biSerial;
    packet.XParse << value.byFlag;
    packet.XParse << value.byPostType;
    packet.XParse << value.bDecrease;
    packet.XParse << value.biRemainTime;
    packet.XParse << value.wPostCount;
    packet.XParse << value.nErrorCode;
    packet << value.psCreateItem;
    packet << value.psUpdateItem;
    packet << value.psUpdateSerial;
    packet.XParse << static_cast<std::int16_t>(value.stAppearanceList.size());
    for (const auto& item : value.stAppearanceList) {
        packet << item;
    }
    return packet;
}

// PS_POST_LEVEL_UP_EVENT_UPDATE 序列化运算符
inline XPacket& operator<<(XPacket& packet, const PS_POST_LEVEL_UP_EVENT_UPDATE& value) {
    packet.XParse << value.nUAID;
    packet.XParse << value.nGroup;
    packet.XParse << value.nLv;
    packet.XParse << value.nUCID;
    return packet;
}

// PS_DB_PROFILE_PHOTO_UPDATE XSendDBPacket 序列化运算符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_PROFILE_PHOTO_UPDATE& value) {
    packet.XParse << value.dwUCID;
    packet << value.stInfo;
    packet.XParse << value.nError;
    return packet;
}

// PS_DB_PROFILE_PHOTO_CHANGE 序列化运算符
inline XPacket& operator<<(XPacket& packet, const PS_DB_PROFILE_PHOTO_CHANGE& value) {
    packet.XParse << value.dwUCID;
    packet << value.stNewPhotoInfo;
    packet.XParse << value.nError;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_PROFILE_PHOTO_CHANGE& value) {
    packet.XParse << value.dwUCID;
    packet << value.stNewPhotoInfo;
    packet.XParse << value.nError;
    return packet;
}
