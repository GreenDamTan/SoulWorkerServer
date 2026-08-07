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
// PDB UDT 0x15047, Size = 1688
struct ST_POST_DATA {
    std::int64_t biSerial = 0;             // offset 0, 邮件序列号
    ST_POST_CHAR stCharInfo{};             // offset 8, 发送者信息
    wchar_t strTitle[41] = {};             // offset 64, 邮件标题
    wchar_t strMsg[401] = {};              // offset 146, 邮件内容
    std::int64_t biMoney = 0;              // offset 952, 附加金钱
    STItem stItemList[5] = {};             // offset 960, 附加物品列表
    std::int64_t nRegTime = 0;             // offset 1560, 注册时间
    std::uint8_t byFlag = 0;               // offset 1568, 标记
    std::uint8_t byPostType = 0;           // offset 1569, 邮件类型
    std::uint8_t byPostSubType = 0;        // offset 1570, 邮件子类型
    std::uint8_t _pad1[5] = {};
    std::int64_t nRemainTime = 0;          // offset 1576, 剩余时间
    PS_ITEM_SOCKET_LIST vecSocketList{};   // offset 1584, 镶嵌列表
    PS_ITEM_BROACH_LIST vecBroachList{};   // offset 1616, 镂刻列表
    PS_ITEM_PACKAGE_LIST vecPackageList{}; // offset 1648, 套装列表
    // offset 1680: union { dwID1 / dwID2 / biEventID }
    std::int64_t biEventID = 0;            // 活动ID
};
static_assert(sizeof(ST_POST_DATA) == 1688, "ST_POST_DATA size must match PDB (UDT 0x15047, 1688)");

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

// 对齐 IDA: 物品日志项 (16 bytes)
// 来自 IDA: PS_LOG_ITEM
struct PS_LOG_ITEM {
    std::int64_t biSerial = 0;   // +0x00: 物品序列号
    std::int32_t nItemID = 0;    // +0x08: 物品ID
    std::int16_t shCount = 0;    // +0x0C: 数量
    std::int16_t _pad0 = 0;      // +0x0E: padding
};

struct PS_LOG_ITEM_LIST {
    std::vector<PS_LOG_ITEM> vecLogItem;
};

// 对齐 IDA: 物品分解结果项 (16 bytes)
// 来自 IDA: PS_ITEM_DISASSEMBLE
struct PS_ITEM_DISASSEMBLE {
    std::int32_t nReqItemID = 0;      // +0x00: 请求物品ID
    std::int32_t nResultID = 0;       // +0x04: 结果物品ID
    std::int16_t shResultCount = 0;   // +0x08: 结果数量
    std::int32_t nResultEther = 0;    // +0x0C: 结果以太
};

// 对齐 IDA: 物品分解结果列表 (32 bytes)
// 来自 IDA: PS_ITEM_DISASSEMBLE_RESULT
struct PS_ITEM_DISASSEMBLE_RESULT {
    std::vector<PS_ITEM_DISASSEMBLE> vecInfo;
};

// 对齐 IDA: 物品插槽更新项 (136 bytes)
// 来自 IDA: ST_ITEM_SOCKET_UPDATE
struct ST_ITEM_SOCKET_UPDATE {
    PS_STORAGE_INFO stInfo{};         // +0x00: 存储信息 (128 bytes)
    std::uint8_t bySocketPos = 0;     // +0x80: 插槽位置
    std::uint8_t _pad0[7] = {};       // padding
};

// 对齐 IDA: 物品插槽更新列表 (32 bytes)
// 来自 IDA: ST_ITEM_SOCKET_UPDATE_LIST
struct ST_ITEM_SOCKET_UPDATE_LIST {
    std::vector<ST_ITEM_SOCKET_UPDATE> vecSocket;
};

// 对齐 IDA: 插槽拆卸请求结构体 (112 bytes)
// 来自 IDA: PS_DB_SOCKET_DETACH
struct PS_DB_SOCKET_DETACH {
    std::uint32_t dwUCID = 0;                       // +0x00: 角色ID
    std::uint8_t _pad0[4] = {};                     // +0x04: padding
    PS_RES_STORAGE_INFO psUpdateItemList;           // +0x08: 更新物品列表 (40 bytes)
    PS_RES_STORAGE_INFO psCreateItemList;           // +0x30: 创建物品列表 (40 bytes)
    std::int64_t biEquipedSerial = 0;               // +0x58: 装备序列号
    PS_ITEM_SLOT_INFO psEquipItemInfo{};            // +0x60: 装备物品槽位信息 (4 bytes)
    std::uint8_t byDetachPos = 0;                   // +0x64: 拆卸位置
    std::uint8_t _pad1[3] = {};                     // padding
    std::int32_t nResult = 0;                       // +0x68: 结果码
};

// 对齐 IDA: 耐久度信息 (16 bytes)
// 来自 IDA: ST_ENDURANCE_INFO
struct ST_ENDURANCE_INFO {
    std::int64_t biSerial = 0;          // +0x00: 物品序列号
    std::uint8_t byCurEndurance = 0;    // +0x08: 当前耐久度
    std::uint8_t _pad0[7] = {};         // padding
};

// 对齐 IDA: 耐久度列表 (32 bytes)
// 来自 IDA: ST_ENDURANCE_LIST
struct ST_ENDURANCE_LIST {
    std::vector<ST_ENDURANCE_INFO> vecItem;
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

// 对齐 IDA 0x1400643D0: 头像照片收藏设置请求
struct PS_PROFILE_PHOTO_FAVORITE {
    std::uint32_t dwPhotoID = 0;           // 照片ID
    std::uint8_t byFavorite = 0;           // 收藏标记 (0=取消, 1=收藏)
};

// PDB LF_FIELDLIST (UDT 0xc554): ST_PROFILE_PHOTO_INFO stPhoto @0, bool bDelete @16, size 24
struct PS_PROFILE_PHOTO_UPDATE {
    ST_PROFILE_PHOTO_INFO stPhoto{};       // offset 0x00: 照片信息 (16 bytes)
    bool bDelete = false;                  // offset 0x10: 删除标记
};
static_assert(sizeof(PS_PROFILE_PHOTO_UPDATE) == 24, "PS_PROFILE_PHOTO_UPDATE size must match PDB");

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

// 对齐 IDA: 邮件领取结果
struct PS_RES_POST_RECEIPT {
    std::int64_t biSerial = 0;
    std::uint8_t byPostFlag = 0;
    std::uint8_t _pad0[7] = {};
    std::int64_t biRemainTime = 0;
    std::uint16_t wPostCount = 0;
    std::uint8_t _pad1[6] = {};
};

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
    union {                                    // +0x420: event/identifier payload
        struct {
            std::uint32_t dwID1;
            std::uint32_t dwID2;
        };
        std::int64_t dwEventID = 0;
    };
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

// PS_LOG_ITEM 序列化 (对齐 IDA: biSerial, nItemID, shCount)
inline void operator>>(XPacket& packet, PS_LOG_ITEM& value) {
    packet.XParse >> value.biSerial;
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse >> value._pad0;
}

inline XPacket& operator<<(XPacket& packet, const PS_LOG_ITEM& value) {
    packet.XParse << value.biSerial;
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value._pad0;
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

// PS_ITEM_DISASSEMBLE 序列化
inline void operator>>(XPacket& packet, PS_ITEM_DISASSEMBLE& value) {
    packet.XParse >> value.nReqItemID;
    packet.XParse >> value.nResultID;
    packet.XParse >> value.shResultCount;
    packet.XParse >> value.nResultEther;
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_DISASSEMBLE& value) {
    packet.XParse << value.nReqItemID;
    packet.XParse << value.nResultID;
    packet.XParse << value.shResultCount;
    packet.XParse << value.nResultEther;
    return packet;
}

// PS_ITEM_DISASSEMBLE_RESULT 序列化
inline void operator>>(XPacket& packet, PS_ITEM_DISASSEMBLE_RESULT& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint8_t i = 0; i < count; ++i) {
        PS_ITEM_DISASSEMBLE info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_DISASSEMBLE_RESULT& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

// ST_ITEM_SOCKET_UPDATE 序列化
inline void operator>>(XPacket& packet, ST_ITEM_SOCKET_UPDATE& value) {
    packet >> value.stInfo;
    packet.XParse >> value.bySocketPos;
}

inline XPacket& operator<<(XPacket& packet, const ST_ITEM_SOCKET_UPDATE& value) {
    packet << value.stInfo;
    packet.XParse << value.bySocketPos;
    return packet;
}

// ST_ITEM_SOCKET_UPDATE_LIST 序列化
inline void operator>>(XPacket& packet, ST_ITEM_SOCKET_UPDATE_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecSocket.clear();
    value.vecSocket.reserve(count);
    for (std::uint8_t i = 0; i < count; ++i) {
        ST_ITEM_SOCKET_UPDATE info;
        packet >> info;
        value.vecSocket.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_ITEM_SOCKET_UPDATE_LIST& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecSocket.size());
    for (const auto& info : value.vecSocket) {
        packet << info;
    }
    return packet;
}

// PS_DB_SOCKET_DETACH 序列化
inline void operator>>(XPacket& packet, PS_DB_SOCKET_DETACH& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse.GetDWORD();  // skip padding
    packet >> value.psUpdateItemList;
    packet >> value.psCreateItemList;
    packet.XParse >> value.biEquipedSerial;
    packet.XParse >> value.psEquipItemInfo.byInvenType;
    packet.XParse.GetBYTE();
    packet.XParse >> value.psEquipItemInfo.shSlotPos;
    packet.XParse >> value.byDetachPos;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();  // skip padding
    packet.XParse >> value.nResult;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_SOCKET_DETACH& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << static_cast<std::int32_t>(0);  // padding
    packet << value.psUpdateItemList;
    packet << value.psCreateItemList;
    packet.XParse << value.biEquipedSerial;
    packet.XParse << value.psEquipItemInfo.byInvenType;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.psEquipItemInfo.shSlotPos;
    packet.XParse << value.byDetachPos;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.nResult;
    return packet;
}

// ST_ENDURANCE_INFO 序列化
inline void operator>>(XPacket& packet, ST_ENDURANCE_INFO& value) {
    packet.XParse >> value.biSerial;
    packet.XParse >> value.byCurEndurance;
    packet.XParse.GetQWORD();  // skip padding
}

inline XPacket& operator<<(XPacket& packet, const ST_ENDURANCE_INFO& value) {
    packet.XParse << value.biSerial;
    packet.XParse << value.byCurEndurance;
    packet.XParse << static_cast<std::int64_t>(0);  // padding
    return packet;
}

// ST_ENDURANCE_LIST 序列化
inline void operator>>(XPacket& packet, ST_ENDURANCE_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecItem.clear();
    value.vecItem.reserve(count);
    for (std::uint8_t i = 0; i < count; ++i) {
        ST_ENDURANCE_INFO info;
        packet >> info;
        value.vecItem.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_ENDURANCE_LIST& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecItem.size());
    for (const auto& info : value.vecItem) {
        packet << info;
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
    packet >> value.vecSocketList;
    packet >> value.vecBroachList;
    packet >> value.vecPackageList;
    packet.XParse >> value.biEventID;
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
    packet << value.vecSocketList;
    packet << value.vecBroachList;
    packet << value.vecPackageList;
    packet.XParse << value.biEventID;
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

inline XPacket& operator<<(XPacket& packet, const PS_PROFILE_PHOTO_LOAD& value) {
    packet.XParse << value.dwUCID;
    std::int16_t nCount = static_cast<std::int16_t>(value.vecList.size());
    packet.XParse << nCount;
    for (const auto& item : value.vecList) {
        packet << item;
    }
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

// PS_PROFILE_PHOTO_UPDATE 序列化操作符
inline XPacket& operator<<(XPacket& packet, const PS_PROFILE_PHOTO_UPDATE& value) {
    packet << value.stPhoto;
    packet.XParse << value.bDelete;
    return packet;
}

inline void operator>>(XPacket& packet, PS_PROFILE_PHOTO_UPDATE& value) {
    packet >> value.stPhoto;
    packet.XParse >> value.bDelete;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_PROFILE_PHOTO_UPDATE& value) {
    packet << value.stPhoto;
    packet.XParse << value.bDelete;
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
    packet.XParse << value.dwEventID;
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
    packet.XParse >> value.dwEventID;
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

// ============================================================================
// XSQLItemSetupProcess 需要的额外结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: 物品镂刻状态更新 (24 bytes)
// 来自 IDA ReqItemBroachActive: PS_ITEM_BROACH_STATE_UPDATE
struct PS_ITEM_BROACH_STATE_UPDATE {
    std::int64_t biSerial = 0;              // +0x00: 物品序列号
    wchar_t szBroachState[8] = {};          // +0x08: 镂刻状态字符串 (16 bytes)
};

// PS_ITEM_BROACH_STATE_UPDATE 序列化
inline void operator>>(XPacket& packet, PS_ITEM_BROACH_STATE_UPDATE& value) {
    packet.XParse >> value.biSerial;
    short sLen = 0;
    packet.XParse.GetWString(value.szBroachState, 8, sLen);
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_BROACH_STATE_UPDATE& value) {
    packet.XParse << value.biSerial;
    packet.XParse << GreenDamTan_BoundedWideString(value.szBroachState);
    return packet;
}

// 对齐 IDA: 物品绑定类型更新 (16 bytes)
// 来自 IDA ReqItemBroachEquip: ST_ITEM_BIND_TYPE_UPDATE
struct ST_ITEM_BIND_TYPE_UPDATE {
    std::int64_t biCostumeSerial = 0;       // +0x00: 时装序列号
    std::uint8_t byBindType = 0;            // +0x08: 绑定类型
    std::uint8_t _pad0[7] = {};             // padding
};

// ST_ITEM_BIND_TYPE_UPDATE 序列化
inline void operator>>(XPacket& packet, ST_ITEM_BIND_TYPE_UPDATE& value) {
    packet.XParse >> value.biCostumeSerial;
    packet.XParse >> value.byBindType;
    packet.XParse.GetQWORD();  // skip padding
}

inline XPacket& operator<<(XPacket& packet, const ST_ITEM_BIND_TYPE_UPDATE& value) {
    packet.XParse << value.biCostumeSerial;
    packet.XParse << value.byBindType;
    packet.XParse << static_cast<std::int64_t>(0);  // padding
    return packet;
}

// 对齐 IDA: 物品绑定类型更新列表
struct ST_ITEM_BIND_TYPE_UPDATE_LIST {
    std::vector<ST_ITEM_BIND_TYPE_UPDATE> vecInfo;
};

// ST_ITEM_BIND_TYPE_UPDATE_LIST 序列化
inline void operator>>(XPacket& packet, ST_ITEM_BIND_TYPE_UPDATE_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint8_t i = 0; i < count; ++i) {
        ST_ITEM_BIND_TYPE_UPDATE info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_ITEM_BIND_TYPE_UPDATE_LIST& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

// ============================================================================
// 物品镂刻系统相关结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: 镂刻信息结构 (64 bytes)
// Per IDA ReqItemBroachEquip: ST_BROACH_INFO
struct ST_BROACH_INFO {
    std::int64_t biSerial = 0;              // +0x00: 物品序列号
    std::uint32_t dwItemID[15] = {};        // +0x08: 镂刻物品ID数组 (60 bytes)
    std::uint8_t _pad0[4] = {};             // padding
};

// ST_BROACH_INFO 序列化
inline void operator>>(XPacket& packet, ST_BROACH_INFO& value) {
    packet.XParse >> value.biSerial;
    for (int i = 0; i < 15; ++i) {
        packet.XParse >> value.dwItemID[i];
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_BROACH_INFO& value) {
    packet.XParse << value.biSerial;
    for (int i = 0; i < 15; ++i) {
        packet.XParse << value.dwItemID[i];
    }
    return packet;
}

// 对齐 IDA: 镂刻装备请求结构 (360+ bytes)
// Per IDA ReqItemBroachEquip: PS_DB_BROACH_EQUIP
struct PS_DB_BROACH_EQUIP {
    std::uint32_t dwUCID = 0;               // +0x00: 角色ID
    std::uint8_t _pad0[4] = {};             // padding
    ST_BROACH_INFO stBroachInfo{};          // +0x08: 镂刻信息 (64 bytes)
    PS_RES_STORAGE_INFO psUpdateItemList{}; // +0x48: 更新物品列表
    std::int32_t nDBErrorCode = 0;          // DB错误码
};

// PS_DB_BROACH_EQUIP 序列化
inline void operator>>(XPacket& packet, PS_DB_BROACH_EQUIP& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet >> value.stBroachInfo;
    packet >> value.psUpdateItemList;
    packet.XParse >> value.nDBErrorCode;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_BROACH_EQUIP& value) {
    packet.XParse << value.dwUCID;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    packet << value.stBroachInfo;
    packet << value.psUpdateItemList;
    packet.XParse << value.nDBErrorCode;
    return packet;
}

// 对齐 IDA: 阿卡夏分解请求结构
// Per IDA ReqAkashicDisassemble: PS_DB_AKASHIC_DISASSEMBLE
struct PS_DB_AKASHIC_DISASSEMBLE {
    std::uint32_t dwUCID = 0;
    std::uint8_t byState = 0;
    std::uint8_t byFlag = 0;
    std::int32_t nDBErrorCode = 0;
    PS_RES_STORAGE_INFO psCreateItemList{};
    PS_RES_STORAGE_INFO psUpdateItemList{};
    std::vector<std::uint32_t> psList;  // 阿卡夏ID列表
};

// PS_DB_AKASHIC_DISASSEMBLE 序列化
inline void operator>>(XPacket& packet, PS_DB_AKASHIC_DISASSEMBLE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byState;
    packet.XParse >> value.byFlag;
    packet.XParse >> value.nDBErrorCode;
    packet >> value.psCreateItemList;
    packet >> value.psUpdateItemList;
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.psList.clear();
    value.psList.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        std::uint32_t id = 0;
        packet.XParse >> id;
        value.psList.push_back(id);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_AKASHIC_DISASSEMBLE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.byState;
    packet.XParse << value.byFlag;
    packet.XParse << value.nDBErrorCode;
    packet << value.psCreateItemList;
    packet << value.psUpdateItemList;
    packet.XParse << static_cast<std::uint16_t>(value.psList.size());
    for (const auto& id : value.psList) {
        packet.XParse << id;
    }
    return packet;
}

// 对齐 IDA: 创建物品结构 (对齐 ST_CREATE_ITEM from PSServerFriend.h)
// 确保与 PSServerFriend.h 中的定义一致
// ST_CREATE_ITEM 已在 PSServerFriend.h 中定义 (16 bytes)

// ============================================================================
// 阿卡夏合成/获取信息相关结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: 阿卡夏获取信息项 (4 bytes)
// Per IDA ReqItemAkashicGetInfoLoad: PS_AKASHIC_GETINFO
struct PS_AKASHIC_GETINFO {
    std::uint32_t dwAkashicGroupID = 0;   // 阿卡夏组ID
};

// PS_AKASHIC_GETINFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_AKASHIC_GETINFO& value) {
    packet.XParse << value.dwAkashicGroupID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_AKASHIC_GETINFO& value) {
    packet.XParse >> value.dwAkashicGroupID;
}

// 对齐 IDA: 阿卡夏获取信息列表
struct PS_AKASHIC_GETINFO_LIST {
    std::vector<PS_AKASHIC_GETINFO> vecInfo;
};

// PS_AKASHIC_GETINFO_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_AKASHIC_GETINFO_LIST& value) {
    packet.XParse << static_cast<std::uint16_t>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_AKASHIC_GETINFO_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        PS_AKASHIC_GETINFO info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

// 对齐 IDA: 阿卡夏合成请求结构
// Per IDA ReqItemAkashicComposeEx: PS_DB_AKASHIC_COMPOSE
struct PS_DB_AKASHIC_COMPOSE {
    std::uint32_t dwUCID = 0;
    std::uint8_t byFlag = 0;
    std::uint8_t _pad0[3] = {};
    std::int32_t nErrorCode = 0;
    PS_RES_STORAGE_INFO psUpdateItemList{};
    PS_RES_STORAGE_INFO psCreateItemList{};
};

// PS_DB_AKASHIC_COMPOSE 序列化
inline void operator>>(XPacket& packet, PS_DB_AKASHIC_COMPOSE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byFlag;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.nErrorCode;
    packet >> value.psUpdateItemList;
    packet >> value.psCreateItemList;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_AKASHIC_COMPOSE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.byFlag;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    packet.XParse << value.nErrorCode;
    packet << value.psUpdateItemList;
    packet << value.psCreateItemList;
    return packet;
}

// 对齐 IDA: 阿卡夏获取信息添加请求
// Per IDA ReqItemAkashicGetInfoAdd: PS_DB_AKASHIC_GETINFO
struct PS_DB_AKASHIC_GETINFO {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwAkashicGroupID = 0;
};

// PS_DB_AKASHIC_GETINFO 序列化
inline void operator>>(XPacket& packet, PS_DB_AKASHIC_GETINFO& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwAkashicGroupID;
}

// PS_DB_AKASHIC_GETINFO 数据库包序列化操作符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_AKASHIC_GETINFO& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwAkashicGroupID;
    return packet;
}

// ============================================================================
// 物品染色/称号变更相关结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: 染色信息 (8 bytes)
struct PS_DYE_INFO {
    std::int32_t nDyePoint = 0;          // 染色点数
    std::int32_t _pad0 = 0;
};

// 对齐 IDA: 物品染色请求结构
// Per IDA ReqItemDye: PS_DB_ITEM_DYE
struct PS_DB_ITEM_DYE {
    std::uint32_t dwUCID = 0;
    std::uint8_t _pad0[4] = {};
    PS_DYE_INFO psResDyeInfo{};
    std::int32_t nErrorCode = 0;
    PS_RES_STORAGE_INFO psUpdateItemList{};
    PS_RES_STORAGE_INFO psCostumeItemList{};
};

// PS_DB_ITEM_DYE 序列化
inline void operator>>(XPacket& packet, PS_DB_ITEM_DYE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.psResDyeInfo.nDyePoint;
    packet.XParse >> value.nErrorCode;
    packet >> value.psUpdateItemList;
    packet >> value.psCostumeItemList;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_DYE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    packet.XParse << value.psResDyeInfo.nDyePoint;
    packet.XParse << value.nErrorCode;
    packet << value.psUpdateItemList;
    packet << value.psCostumeItemList;
    return packet;
}

// PS_DB_ITEM_TITLE_CHANGE 已在 PSCommon.h 中定义
// 这里仅补充序列化运算符

// PS_DB_ITEM_TITLE_CHANGE 序列化（对齐 IDA ReqItemTitleChange）
inline void operator>>(XPacket& packet, PS_DB_ITEM_TITLE_CHANGE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nResult;
    packet >> value.psUpdateItemInfo;
    packet >> value.psUpdateItemList;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_TITLE_CHANGE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nResult;
    packet << value.psUpdateItemInfo;
    packet << value.psUpdateItemList;
    return packet;
}

// ============================================================================
// 物品翻新系统相关结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: 扩展选项更新项 (8 bytes)
struct ST_EXTEND_OPTION_UPDATE {
    std::int16_t shOptionID = 0;
    std::int16_t _pad0 = 0;
    std::int32_t nOption = 0;
};

// 对齐 IDA: 物品选项信息 (52 bytes)
struct ST_ITEM_OPTION_INFO {
    std::int64_t xSerial = 0;
    ST_EXTEND_OPTION_UPDATE stExtendOption[5] = {};
};

// 对齐 IDA: 物品翻新请求结构
// Per IDA ReqItemRenovate: PS_DB_ITEM_RENOVATE
struct PS_DB_ITEM_RENOVATE {
    std::uint32_t dwUCID = 0;
    std::int32_t nRenovatePoint = 0;
    ST_ITEM_OPTION_INFO stItemInfo{};
    PS_RES_STORAGE_INFO psUpdateItem{};
};

// PS_DB_ITEM_RENOVATE 序列化
inline void operator>>(XPacket& packet, PS_DB_ITEM_RENOVATE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nRenovatePoint;
    packet.XParse >> value.stItemInfo.xSerial;
    for (int i = 0; i < 5; ++i) {
        packet.XParse >> value.stItemInfo.stExtendOption[i].shOptionID;
        packet.XParse >> value.stItemInfo.stExtendOption[i].nOption;
    }
    packet >> value.psUpdateItem;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_RENOVATE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nRenovatePoint;
    packet.XParse << value.stItemInfo.xSerial;
    for (int i = 0; i < 5; ++i) {
        packet.XParse << value.stItemInfo.stExtendOption[i].shOptionID;
        packet.XParse << value.stItemInfo.stExtendOption[i].nOption;
    }
    packet << value.psUpdateItem;
    return packet;
}

// ============================================================================
// 物品镂刻移除系统相关结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: 物品镂刻移除请求结构
// Per IDA ReqItemBroachRemove: PS_DB_BROACH_REMOVE
struct PS_DB_BROACH_REMOVE {
    std::uint32_t dwUCID = 0;
    std::uint8_t byRemoveType = 0;
    bool bClear = false;
    std::uint8_t byCreateFlag = 0;
    std::int32_t nErrorCode = 0;
    std::int32_t nBroachID[15] = {};
    std::vector<std::int64_t> biSerialList;
    PS_RES_STORAGE_INFO psReduceItem{};
    PS_RES_STORAGE_INFO psCreateItem{};
};

// PS_DB_BROACH_REMOVE 序列化
inline void operator>>(XPacket& packet, PS_DB_BROACH_REMOVE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byRemoveType;
    packet.XParse >> value.bClear;
    packet.XParse >> value.byCreateFlag;
    packet.XParse >> value.nErrorCode;
    for (int i = 0; i < 15; ++i) {
        packet.XParse >> value.nBroachID[i];
    }
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.biSerialList.clear();
    value.biSerialList.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        std::int64_t serial = 0;
        packet.XParse >> serial;
        value.biSerialList.push_back(serial);
    }
    packet >> value.psReduceItem;
    packet >> value.psCreateItem;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_BROACH_REMOVE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.byRemoveType;
    packet.XParse << value.bClear;
    packet.XParse << value.byCreateFlag;
    packet.XParse << value.nErrorCode;
    for (int i = 0; i < 15; ++i) {
        packet.XParse << value.nBroachID[i];
    }
    packet.XParse << static_cast<std::uint16_t>(value.biSerialList.size());
    for (const auto& serial : value.biSerialList) {
        packet.XParse << serial;
    }
    packet << value.psReduceItem;
    packet << value.psCreateItem;
    return packet;
}

// ============================================================================
// 物品精炼系统相关结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: 物品精炼请求结构
// Per IDA ReqItemRefine: PS_DB_ITEM_REFINE
struct PS_DB_ITEM_REFINE {
    std::uint32_t dwUCID = 0;
    std::int64_t biPoint = 0;
    PS_STORAGE_INFO psRefineItem{};
    PS_RES_STORAGE_INFO psUpdateItemList{};
    std::int32_t nResult = 0;
};

// PS_DB_ITEM_REFINE 序列化
inline void operator>>(XPacket& packet, PS_DB_ITEM_REFINE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.biPoint;
    packet >> value.psRefineItem;
    packet >> value.psUpdateItemList;
    packet.XParse >> value.nResult;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_REFINE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.biPoint;
    packet << value.psRefineItem;
    packet << value.psUpdateItemList;
    packet.XParse << value.nResult;
    return packet;
}

// ============================================================================
// 物品插槽交换系统相关结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: 物品插槽交换请求结构
// Per IDA ReqItemSocketExchange: PS_DB_SOCKET_EXCHANGE
struct PS_DB_SOCKET_EXCHANGE {
    std::uint32_t dwUCID = 0;
    PS_RES_STORAGE_INFO psCreateItemList{};
    PS_RES_STORAGE_INFO psUpdateItemList{};
    std::int32_t nResult = 0;
};

// PS_DB_SOCKET_EXCHANGE 序列化
inline void operator>>(XPacket& packet, PS_DB_SOCKET_EXCHANGE& value) {
    packet.XParse >> value.dwUCID;
    packet >> value.psCreateItemList;
    packet >> value.psUpdateItemList;
    packet.XParse >> value.nResult;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_SOCKET_EXCHANGE& value) {
    packet.XParse << value.dwUCID;
    packet << value.psCreateItemList;
    packet << value.psUpdateItemList;
    packet.XParse << value.nResult;
    return packet;
}

// ============================================================================
// 物品插槽升级系统相关结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: 物品插槽升级请求结构
// Per IDA ReqItemSocketUpgrade: PS_DB_SOCKET_UPGRADE
struct PS_DB_SOCKET_UPGRADE {
    std::uint32_t dwUCID = 0;
    std::uint8_t byUpgradeType = 0;
    std::uint8_t byFlag = 0;
    std::uint8_t _pad0[2] = {};
    ST_ITEM_SOCKET stSocketData{};  // 使用 ST_ITEM_SOCKET 包含 biEquipSerial
    PS_RES_STORAGE_INFO psUpdateItemList{};
    PS_RES_STORAGE_INFO psCreateItemList{};
    std::int32_t nResult = 0;
};

// PS_DB_SOCKET_UPGRADE 序列化
inline void operator>>(XPacket& packet, PS_DB_SOCKET_UPGRADE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byUpgradeType;
    packet.XParse >> value.byFlag;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet >> value.stSocketData;
    packet >> value.psUpdateItemList;
    packet >> value.psCreateItemList;
    packet.XParse >> value.nResult;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_SOCKET_UPGRADE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.byUpgradeType;
    packet.XParse << value.byFlag;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    packet << value.stSocketData;
    packet << value.psUpdateItemList;
    packet << value.psCreateItemList;
    packet.XParse << value.nResult;
    return packet;
}

// ============================================================================
// 物品插槽提取系统相关结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: 物品插槽提取请求结构
// Per IDA ReqItemSocketExtract: PS_DB_SOCKET_EXTRACT
struct PS_DB_SOCKET_EXTRACT {
    std::uint32_t dwUCID = 0;
    std::uint8_t byExtratType = 0;
    std::uint8_t bySocketIndex = 0;
    std::uint8_t _pad0[2] = {};
    std::int64_t biSerial = 0;
    PS_RES_STORAGE_INFO psUpdateItemList{};
    PS_RES_STORAGE_INFO psCreateItemList{};
    std::int32_t nResult = 0;
};

// PS_DB_SOCKET_EXTRACT 序列化
inline void operator>>(XPacket& packet, PS_DB_SOCKET_EXTRACT& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byExtratType;
    packet.XParse >> value.bySocketIndex;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.biSerial;
    packet >> value.psUpdateItemList;
    packet >> value.psCreateItemList;
    packet.XParse >> value.nResult;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_SOCKET_EXTRACT& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.byExtratType;
    packet.XParse << value.bySocketIndex;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    packet.XParse << value.biSerial;
    packet << value.psUpdateItemList;
    packet << value.psCreateItemList;
    packet.XParse << value.nResult;
    return packet;
}
