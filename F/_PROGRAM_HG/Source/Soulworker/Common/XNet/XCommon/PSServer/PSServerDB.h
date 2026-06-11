#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerItem.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerLogin.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMazeSync.h"

// ============================================================================
// PSServerDB.h - DBAgent 相关结构体及序列化运算符
//
// 此文件包含DBAgent数据库操作的所有数据结构和序列化操作：
// - 金币/BP更新
// - 称号系统
// - 角色位置/无限塔
// - 世界状态/迷宫/区域
// - 成就系统
// - 物品限制
// - 统计数据
// - 每日/每周任务
// - 网吧系统
// - 世界事件
// ============================================================================

// ============================================================================
// DBAgent: 网吧(NetCafe)相关结构
// ============================================================================

// 对齐 IDA: 网吧用户信息 (36 bytes)
struct PS_DB_HAN_NET_CAFE {
    std::uint32_t dwUAID = 0;           // offset 0x00: 账号ID
    char szAccountID[21] = {};          // offset 0x04: 账号名
    std::int32_t nServerID = 0;         // offset 0x1C: 服务器ID
    bool bLogin = false;                // offset 0x20: 登录状态
    std::uint8_t _pad0[3] = {};         // padding
};

static_assert(sizeof(PS_DB_HAN_NET_CAFE) == 36, "PS_DB_HAN_NET_CAFE size must match IDA");

// 对齐 IDA: 网吧用户列表 (40 bytes)
struct PS_DB_HAN_NET_CAFE_LIST {
    std::vector<PS_DB_HAN_NET_CAFE> vecList;  // offset 0x00: 用户列表
    bool bLast = false;                       // offset 0x20: 是否最后
    std::uint8_t _pad0[7] = {};               // padding
};

static_assert(sizeof(PS_DB_HAN_NET_CAFE_LIST) == 40, "PS_DB_HAN_NET_CAFE_LIST size must match IDA");

// ============================================================================
// DBAgent: 世界事件(WorldEvent)相关结构
// ============================================================================

// 对齐 IDA: 世界事件奖励信息 (8 bytes)
struct ST_WORLD_EVENT_REWARD_INFO {
    std::int32_t nRewardIndex = 0;      // offset 0x00: 奖励索引
    std::uint8_t byRewardType = 0;      // offset 0x04: 奖励类型
    std::uint8_t byRewardState = 0;     // offset 0x05: 奖励状态
    std::uint8_t _pad0[2] = {};         // padding
};

static_assert(sizeof(ST_WORLD_EVENT_REWARD_INFO) == 8, "ST_WORLD_EVENT_REWARD_INFO size must match IDA");

// 对齐 IDA: 世界事件信息响应 (48 bytes)
struct PS_WORLD_EVENT_INFO_RES {
    std::int32_t nEventID = 0;              // offset 0x00: 事件ID
    std::int32_t nTotalCount = 0;           // offset 0x04: 总计数
    std::int32_t nMyCount = 0;              // offset 0x08: 我的计数
    std::uint8_t byDailyRewardState = 0;    // offset 0x0C: 每日奖励状态
    std::uint8_t _pad0[3] = {};             // padding
    std::vector<ST_WORLD_EVENT_REWARD_INFO> vecRewardInfo;  // offset 0x10: 奖励列表
};

static_assert(sizeof(PS_WORLD_EVENT_INFO_RES) == 48, "PS_WORLD_EVENT_INFO_RES size must match IDA");

// 对齐 IDA: 世界事件信息请求 (12 bytes)
struct PS_DB_WORLD_EVENT_INFO_REQ {
    std::uint32_t dwUAID = 0;       // offset 0x00: 账号ID
    std::uint32_t dwUCID = 0;       // offset 0x04: 角色ID
    std::int32_t nEventID = 0;      // offset 0x08: 事件ID
};

static_assert(sizeof(PS_DB_WORLD_EVENT_INFO_REQ) == 12, "PS_DB_WORLD_EVENT_INFO_REQ size must match IDA");

// 对齐 IDA: 世界事件信息数据库响应 (64 bytes)
struct PS_DB_WORLD_EVENT_INFO_RES {
    PS_WORLD_EVENT_INFO_RES psInfo{};           // offset 0x00: 事件信息 (48 bytes)
    std::int64_t biLastRegisterDate = 0;        // offset 0x30: 最后注册日期
    std::int64_t biDailyRewardDate = 0;         // offset 0x38: 每日奖励日期
};

static_assert(sizeof(PS_DB_WORLD_EVENT_INFO_RES) == 64, "PS_DB_WORLD_EVENT_INFO_RES size must match IDA");

// 对齐 IDA: 世界事件奖励请求 (12 bytes)
struct PS_WORLD_EVENT_REWARD_REQ {
    std::int32_t nEventID = 0;             // offset 0x00: 事件ID
    std::uint8_t byRewardType = 0;         // offset 0x04: 奖励类型
    std::uint8_t _pad0[3] = {};            // padding
    std::int32_t nRewardIndex = 0;         // offset 0x08: 奖励索引
};

static_assert(sizeof(PS_WORLD_EVENT_REWARD_REQ) == 12, "PS_WORLD_EVENT_REWARD_REQ size must match IDA");

// 对齐 IDA: 世界事件奖励 (120 bytes)
struct PS_DB_WORLD_EVENT_REWARD {
    std::uint32_t dwUAID = 0;                      // offset 0x00: 账号ID
    std::uint32_t dwUCID = 0;                      // offset 0x04: 角色ID
    PS_WORLD_EVENT_REWARD_REQ psReq{};             // offset 0x08: 请求信息 (12 bytes)
    std::uint8_t _pad0[4] = {};                    // padding
    PS_RES_STORAGE_INFO stCreateItem{};            // offset 0x18: 创建物品 (40 bytes)
    PS_RES_STORAGE_INFO stUpdateItem{};            // offset 0x40: 更新物品 (40 bytes)
    std::int32_t nError = 0;                       // offset 0x68: 错误码
    std::uint8_t byItemFlag = 0;                   // offset 0x6C: 物品标记
    std::uint8_t _pad1[3] = {};                    // padding
    std::uint32_t dwRewardItemID = 0;              // offset 0x70: 奖励物品ID
    std::int16_t shRewardCount = 0;                // offset 0x74: 奖励数量
};

static_assert(sizeof(PS_DB_WORLD_EVENT_REWARD) == 120, "PS_DB_WORLD_EVENT_REWARD size must match IDA");

// PS_WORLD_EVENT_REWARD_REQ 序列化运算符
inline void operator>>(XPacket& packet, PS_WORLD_EVENT_REWARD_REQ& value) {
    packet.XParse >> value.nEventID;
    packet.XParse >> value.byRewardType;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.nRewardIndex;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_WORLD_EVENT_REWARD_REQ& value) {
    packet.XParse << value.nEventID;
    packet.XParse << value.byRewardType;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.nRewardIndex;
    return packet;
}

// PS_DB_WORLD_EVENT_REWARD 序列化运算符
inline void operator>>(XPacket& packet, PS_DB_WORLD_EVENT_REWARD& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet >> value.psReq;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet >> value.stCreateItem;
    packet >> value.stUpdateItem;
    packet.XParse >> value.nError;
    packet.XParse >> value.byItemFlag;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad1), sizeof(value._pad1));
    packet.XParse >> value.dwRewardItemID;
    packet.XParse >> value.shRewardCount;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_WORLD_EVENT_REWARD& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet << value.psReq;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet << value.stCreateItem;
    packet << value.stUpdateItem;
    packet.XParse << value.nError;
    packet.XParse << value.byItemFlag;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad1), sizeof(value._pad1));
    packet.XParse << value.dwRewardItemID;
    packet.XParse << value.shRewardCount;
    return packet;
}

// 对齐 IDA: 世界事件注册请求 (56 bytes)
struct PS_DB_WORLD_EVENT_REGISTER_REQ {
    std::uint32_t dwUCID = 0;              // offset 0x00: 角色ID
    std::int32_t nEventID = 0;             // offset 0x04: 事件ID
    std::int32_t nCount = 0;               // offset 0x08: 计数
    std::uint8_t byInvenType = 0;          // offset 0x0C: 背包类型
    std::uint8_t _pad0[3] = {};            // padding
    PS_RES_STORAGE_INFO stUpdateItem{};    // offset 0x10: 物品更新列表 (40 bytes)
};

static_assert(sizeof(PS_DB_WORLD_EVENT_REGISTER_REQ) == 56, "PS_DB_WORLD_EVENT_REGISTER_REQ size must match IDA");

// 对齐 IDA: 世界事件注册响应 (80 bytes)
struct PS_DB_WORLD_EVENT_REGISTER_RES {
    std::int32_t nError = 0;                              // offset 0x00: 错误码
    std::uint8_t _pad0[4] = {};                           // padding
    PS_DB_WORLD_EVENT_REGISTER_REQ psReq{};               // offset 0x08: 请求信息 (56 bytes)
    std::int32_t nTotalCount = 0;                         // offset 0x40: 总计数
    std::int32_t nMyCount = 0;                            // offset 0x44: 我的计数
    std::int64_t biLastRegisterDate = 0;                  // offset 0x48: 最后注册日期
};

static_assert(sizeof(PS_DB_WORLD_EVENT_REGISTER_RES) == 80, "PS_DB_WORLD_EVENT_REGISTER_RES size must match IDA");

// PS_DB_WORLD_EVENT_REGISTER_REQ 序列化运算符
inline void operator>>(XPacket& packet, PS_DB_WORLD_EVENT_REGISTER_REQ& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nEventID;
    packet.XParse >> value.nCount;
    packet.XParse >> value.byInvenType;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet >> value.stUpdateItem;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_WORLD_EVENT_REGISTER_REQ& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nEventID;
    packet.XParse << value.nCount;
    packet.XParse << value.byInvenType;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet << value.stUpdateItem;
    return packet;
}

// PS_DB_WORLD_EVENT_REGISTER_RES 序列化运算符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_WORLD_EVENT_REGISTER_RES& value) {
    packet.XParse << value.nError;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet << value.psReq;
    packet.XParse << value.nTotalCount;
    packet.XParse << value.nMyCount;
    packet.XParse << value.biLastRegisterDate;
    return packet;
}

// PS_HAN_BILLING_ORDER_NO XSendDBPacket 序列化运算符 (结构体在 PSServerLogin.h 中定义)
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_HAN_BILLING_ORDER_NO& value) {
    packet.XParse << value.dwItemID;
    packet.XParse << value.nCount;
    packet.XParse << value.nBanance;
    packet.XParse << value.nOrderNo;
    packet.XParse << value.nResult;
    packet.XParse.SetBytes(value.szCode, 21);
    packet.XParse << value.nShopIndex;
    return packet;
}

// PS_HAN_BILLING_ORDER_NO_VEC XSendDBPacket 序列化运算符 (结构体在 PSServerLogin.h 中定义)
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_HAN_BILLING_ORDER_NO_VEC& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse.SetBytes(value.szBillCode, 21);
    packet.XParse << value.dwRecvUCID;
    packet.XParse << value.dwRecvUAID;
    packet.XParse << value.bGift;
    packet.XParse.SetBytes(value.szRecvAccountID, 21);
    packet.XParse.SetBytes(value.szRecvCharacterName, 21);
    packet.XParse << value.byType;
    packet.XParse << value.bResult;
    std::int16_t shCount = static_cast<std::int16_t>(value.vecOrderInfo.size());
    packet.XParse << shCount;
    for (const auto& item : value.vecOrderInfo) {
        packet << item;
    }
    return packet;
}

// ============================================================================
// DBAgent: 金币/BP更新相关结构
// ============================================================================

// 对齐 IDA 0x14001FDA0: 金币更新请求/响应结构
struct PS_DB_GOLD_UPDATE {
    unsigned int dwActorID = 0;
    std::int64_t nAddGold = 0;
    std::int64_t nTotalGold = 0;
};

// BP信息结构（内部使用）
struct PS_BP_INFO {
    unsigned int dwActorID = 0;
    std::int64_t biBP = 0;
};

// 对齐 IDA 0x140020060: BP更新请求结构
struct PS_DB_BP_UPDATE {
    PS_BP_INFO psBP{};
    std::int64_t nLimitMonsterBP = 0;
    std::int64_t nLimitPVPBP = 0;
};

// ============================================================================
// DBAgent: 称号加载相关结构
// ============================================================================

// 称号信息结构
struct ST_TITLE_INFO_DB {
    unsigned int dwTitleID = 0;
    std::int16_t sType = 0;
    bool bFavorite = false;
};

// 称号加载响应结构
struct PS_TITLE_LOAD {
    ST_TitleInfo stInsideTitle{};
    ST_TitleInfo stOutsideTitle{};
    std::int16_t shFavoritePrefixCount = 0;
    std::int16_t shFavoriteSuffixCount = 0;
    std::vector<ST_TITLE_INFO_DB> vecTitleID;
    std::vector<ST_TITLE_INFO_DB> vecOpenTitleID;
};

// 称号添加请求 - PS_TITLE_ADD
struct PS_TITLE_ADD {
    unsigned int dwTitleID = 0;
    int nLogType = 0;
};

// 称号信息结构（用于选择称号）
struct ST_TITLE_INFO_SELECT {
    unsigned int dwPrefix = 0;
    unsigned int dwSuffix = 0;
};

// 称号选择请求 - PS_REQ_TITLE_UPDATE
struct PS_REQ_TITLE_UPDATE {
    ST_TITLE_INFO_SELECT stInsideTitle{};
    ST_TITLE_INFO_SELECT stOutsideTitle{};
};

// 称号收藏请求 - PS_TITLE_FAVORITE
struct PS_TITLE_FAVORITE {
    unsigned int dwUCID = 0;
    unsigned int dwTitleID = 0;
    bool bFavorite = false;
};

// 称号收藏数据库结构 - PS_DB_TITLE_FAVORITE
struct PS_DB_TITLE_FAVORITE {
    struct {
        unsigned int dwUCID = 0;
        unsigned int dwTitleID = 0;
        bool bFavorite = false;
    } psInfo{};
    int nError = 0;
};

// ============================================================================
// 过场动画相关结构体
// ============================================================================

// 过场动画更新请求 - PS_CUTSCENE_UPDATE
// IDA: referenced by CGocEntity::UpdateCutscene (0x14005FDD0)
struct PS_CUTSCENE_UPDATE {
    bool bOnPlay = false;           // 是否播放中
    char szName[256] = {};          // 过场动画名称
};

// 过场动画更新响应 - PS_CUTSCENE_UPDATE_RES
// IDA: struct size 4, member: nPlayState (int, offset 0)
struct PS_CUTSCENE_UPDATE_RES {
    int nPlayState = 0;             // 播放状态 (0 = 停止)
};

// ============================================================================
// DBAgent 角色位置/状态/FP 相关结构体
// ============================================================================

// 对齐 IDA: 角色位置更新请求结构
struct PS_CHARACTER_UPDATE_POS {
    unsigned int dwActorID = 0;
    UXMapID uxMapID{};
    XVec3 vPos{};
    int nRot = 0;
    int nJumpID = 0;
    unsigned int dwServerID = 0;
};

// 对齐 IDA: 无限塔限制时间更新结构
struct PS_UPDATE_INFINITETOWER_LIMIT_TIME {
    unsigned int dwActorID = 0;
    int nLimitTime = 0;
};

// 对齐 IDA: 无限塔清除章节结构
struct PS_UPDATE_INFINITETOWER_CLEAR_CHAPTER {
    unsigned int dwActorID = 0;
    std::int16_t sClearChapter = 0;
    std::int16_t sClearStage = 0;
};

// ============================================================================
// DBAgent 世界状态/迷宫/区域相关结构体
// ============================================================================

// 对齐 IDA: 迷宫状态结构
struct ST_MAZE_STATE {
    int nMazeID = 0;
    int nClearCount = 0;
    int nSharePoint = 0;
    std::int8_t byRewardStep = 0;
    std::uint8_t _pad0[3] = {};
    int nGroupID = 0;
    int nDistrictID = 0;
};

// 对齐 IDA: 区域状态结构
struct ST_DISTRICT_STATE {
    int nDistrictID = 0;
    int nDSPoint = 0;
    bool bReward = false;
    std::uint8_t _pad0[3] = {};
};

// 迷宫状态列表
struct ST_MAZE_STATE_LIST {
    std::vector<ST_MAZE_STATE> vecList;
};

// 区域状态列表
struct ST_DISTRICT_STATE_LIST {
    std::vector<ST_DISTRICT_STATE> vecList;
};

// ============================================================================
// DBAgent 成就/社区/职业场景相关结构体
// ============================================================================

// 对齐 IDA: 成就信息结构
struct ST_ACHIEVE_INFO {
    int nIndex = 0;
    std::uint64_t biCount = 0;
};

// 成就列表
struct ST_ACHIEVE_LIST {
    std::vector<ST_ACHIEVE_INFO> vecList;
};

// 对齐 IDA: 成就位结构（128字节位域）
struct ST_ACHIEVE_BIT {
    std::uint8_t szRewardBit[128] = {};
};

// ST_ACHIEVE_UPDATE - 成就更新结构 (size: 32 bytes)
// IDA: 构造函数 0x1400032b0
// 反编译:
//   ST_ACHIEVE_UPDATE *__fastcall ST_ACHIEVE_UPDATE::ST_ACHIEVE_UPDATE(ST_ACHIEVE_UPDATE *this)
//   {
//     PS_EXCHANGE_ITEM_RECALL_RES::PS_EXCHANGE_ITEM_RECALL_RES((PS_REQ_TICKCOUNT *)this);
//     this->nNextIndex = 0;
//     this->byCategory = 0;
//     this->wCount = 0;
//     this->nCurIndex = 0;
//     return this;
//   }
// 注意: 构造函数调用 PS_EXCHANGE_ITEM_RECALL_RES 是编译器对 stUpdateInfo (16 bytes)
//       内存初始化的优化，不是真正的继承关系
struct ST_ACHIEVE_UPDATE {
    ST_ACHIEVE_INFO stUpdateInfo{};  // 成就更新信息 (offset 0, size 16)
    int nNextIndex = 0;              // 下一个索引 (offset 16, size 4)
    std::uint8_t byCategory = 0;     // 分类 (offset 20, size 1)
    std::uint16_t wCount = 0;        // 计数 (offset 22, size 2)
    int nCurIndex = 0;               // 当前索引 (offset 24, size 4)

    // 显式构造函数 (IDA 0x1400032b0)
    ST_ACHIEVE_UPDATE() : stUpdateInfo{}, nNextIndex(0), byCategory(0), wCount(0), nCurIndex(0) {}
};

// 成就更新列表
struct ST_ACHIEVE_UPDATE_LIST {
    std::vector<ST_ACHIEVE_UPDATE> vecList;
};

// ============================================================================
// DBAgent 无限塔/击杀/代表角色相关结构体
// ============================================================================

// 对齐 IDA: 无限塔信息结构（size=24）
struct PS_INFINITE_TOWER_INFO {
    std::int16_t sClearChapter = 0;     // 已通关章节
    std::int16_t sClearStage = 0;      // 已通关阶段
    char _pad0[4] = {};                 // padding (offset 4-7)
    std::int64_t nLimitTime = 0;        // 限制时间 (offset 8)
    std::int16_t sCount = 0;            // 计数 (offset 16)
    char _pad1[6] = {};                 // padding (offset 18-23)
};
static_assert(sizeof(PS_INFINITE_TOWER_INFO) == 24, "PS_INFINITE_TOWER_INFO size mismatch with IDA");

// 对齐 IDA: 击杀用户信息结构（size=8）
struct PS_KILLED_USER_INFO {
    std::uint32_t dwUCID = 0;       // 被击杀角色ID
    std::int32_t nCount = 0;        // 击杀次数
};
static_assert(sizeof(PS_KILLED_USER_INFO) == 8, "PS_KILLED_USER_INFO size mismatch with IDA");

// 对齐 IDA: 击杀用户列表结构（size=40）
struct PS_KILLED_USER_INFOS {
    std::int64_t nInitTime = 0;                     // 初始化时间 (offset 0)
    std::vector<PS_KILLED_USER_INFO> vecKilledUser; // 击杀用户列表 (offset 8)
};
static_assert(sizeof(PS_KILLED_USER_INFOS) == 40, "PS_KILLED_USER_INFOS size mismatch with IDA");

// 对齐 IDA: 代表角色信息结构（size=72）
struct ST_REPRESENTATIVE_INFO {
    std::uint32_t dwUCID = 0;           // 角色ID (offset 0)
    std::uint8_t byClass = 0;           // 职业 (offset 4)
    std::uint8_t byLevel = 0;           // 等级 (offset 5)
    wchar_t strName[21] = {};           // 角色名 (offset 6, size 42)
    // padding at offset 48 (2 bytes? Actually offset 6 + 42 = 48)
    std::uint32_t dwProfilePhotoID = 0; // 头像ID (offset 48)
    wchar_t strLeagueName[10] = {};     // 公会名 (offset 52, size 20)
};
static_assert(sizeof(ST_REPRESENTATIVE_INFO) == 72, "ST_REPRESENTATIVE_INFO size mismatch with IDA");

// 对齐 IDA: 职业场景结构
struct PS_CLASS_SCENE {
    std::uint8_t byClassScene[6] = {};
};

// ============================================================================
// 物品限制相关结构体
// ============================================================================

/**
 * 来自 IDA 0x140057B90: PS_ITEM_LIMIT - 物品限制信息
 * 用于 SP_ITEM_LIMIT_LOAD 存储过程
 */
struct PS_ITEM_LIMIT {
    std::int32_t nItemID = 0;       // +0x00: 物品ID (4 bytes)
    std::int16_t shCount = 0;       // +0x04: 数量 (2 bytes)
    std::uint8_t _pad0[2] = {};     // +0x06: padding (2 bytes)
    std::int64_t biInitDate = 0;    // +0x08: 初始化日期 (8 bytes)
};

static_assert(sizeof(PS_ITEM_LIMIT) == 16, "PS_ITEM_LIMIT size mismatch");

/**
 * 来自 IDA: ST_ITEM_LIMIT_LIST - 物品限制列表
 */
struct ST_ITEM_LIMIT_LIST {
    std::vector<PS_ITEM_LIMIT> vecList;  // +0x00: 限制列表
};

/**
 * 来自 IDA 0x140057DF0: PS_DB_USE_ITEM_APPREARANCE - 外观使用请求数据
 */
struct PS_DB_USE_ITEM_APPREARANCE {
    std::uint32_t dwUCID = 0;
    std::int32_t nErrorCode = 0;
    PS_RES_STORAGE_INFO psUpdateItemList{};
    struct {
        std::uint16_t wAppearanceID = 0;
        std::uint8_t _pad0[2] = {};
        std::int64_t biEndDate = 0;
    } stAppearanceInfo{};
};

/**
 * 来自 IDA 0x14005A4F0: PS_DB_CARD_DECK_OPEN - 卡组打开请求数据
 */
struct PS_DB_CARD_DECK_OPEN {
    std::uint32_t dwUCID = 0;
    std::int32_t nErrorCode = 0;
    PS_RES_STORAGE_INFO psUpdateItemList{};
    PS_QUICKSLOT_CARD psCardDeck{};
};

// PS_DB_CARD_DECK_OPEN 序列化
inline void operator>>(XPacket& packet, PS_DB_CARD_DECK_OPEN& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nErrorCode;
    packet >> value.psUpdateItemList;
    packet >> value.psCardDeck;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_CARD_DECK_OPEN& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nErrorCode;
    packet << value.psUpdateItemList;
    packet << value.psCardDeck;
    return packet;
}

// ============================================================================
// XSQLStatisticsProcess 统计相关结构体
// ============================================================================

/**
 * 对齐 IDA cvdump: ST_STATISTICS_ITEM_EXCHANGE - 物品交换统计
 * Size = 32 bytes
 * 用于 SP_ITEM_EXCHANGE_DATA_SAVE 存储过程
 */
struct ST_STATISTICS_ITEM_EXCHANGE {
    std::uint8_t byFlag = 0;        // +0x00: 标志位
    std::uint8_t _pad0[3] = {};     // +0x01: padding
    std::uint32_t dwUCID = 0;       // +0x04: 角色 ID
    std::uint32_t dwEXC_ID = 0;     // +0x08: 交换 ID
    std::uint8_t _pad1[4] = {};     // +0x0C: padding
    std::int64_t biSerial = 0;      // +0x10: 序列号
    std::int32_t nExpireHour = 0;   // +0x18: 过期小时数
    std::uint8_t _pad2[4] = {};     // +0x1C: padding
};

static_assert(sizeof(ST_STATISTICS_ITEM_EXCHANGE) == 32, "ST_STATISTICS_ITEM_EXCHANGE size mismatch");

/**
 * 对齐 IDA cvdump: ST_STATISTICS_HELPER - 助战统计
 * Size = 12 bytes
 * 用于 SP_HELPER_DATA_SAVE 存储过程
 */
struct ST_STATISTICS_HELPER {
    std::uint8_t byFlag = 0;        // +0x00: 标志位
    std::uint8_t _pad0[3] = {};     // +0x01: padding
    std::uint32_t dwUCID = 0;       // +0x04: 角色 ID
    std::uint32_t dwHelperID = 0;   // +0x08: 助战 ID
};

static_assert(sizeof(ST_STATISTICS_HELPER) == 12, "ST_STATISTICS_HELPER size mismatch");

/**
 * 对齐 IDA cvdump: ST_STATISTICS_MY_ROOM - 我的房间统计
 * Size = 8 bytes
 * 用于 SP_MY_ROOM_DATA_SAVE 存储过程
 */
struct ST_STATISTICS_MY_ROOM {
    std::uint32_t dwUAID = 0;       // +0x00: 账号 ID
    std::uint8_t byLevel = 0;       // +0x04: 等级
    std::uint8_t _pad0[3] = {};     // +0x05: padding
};

static_assert(sizeof(ST_STATISTICS_MY_ROOM) == 8, "ST_STATISTICS_MY_ROOM size mismatch");

/**
 * 对齐 IDA cvdump: ST_STATISTICS_AKASHIC - 阿卡夏统计
 * Size = 12 bytes
 * 用于 SP_AKASHIC_DATA_SAVE 存储过程
 */
struct ST_STATISTICS_AKASHIC {
    std::uint32_t dwUCID = 0;       // +0x00: 角色 ID
    std::uint32_t dwAkashicID = 0;  // +0x04: 阿卡夏 ID
    std::uint8_t byFlag = 0;        // +0x08: 标志位
    std::uint8_t _pad0[3] = {};     // +0x09: padding
};

static_assert(sizeof(ST_STATISTICS_AKASHIC) == 12, "ST_STATISTICS_AKASHIC size mismatch");

/**
 * 对齐 IDA cvdump: ST_STATISTICS_MY_ROOM_ITEM - 我的房间物品统计
 * Size = 32 bytes
 * 用于 SP_MY_ROOM_ITEM_DATA_SAVE 存储过程
 */
struct ST_STATISTICS_MY_ROOM_ITEM {
    std::uint8_t byFlag = 0;        // +0x00: 标志位
    std::uint8_t _pad0[3] = {};     // +0x01: padding
    std::uint32_t dwUAID = 0;       // +0x04: 账号 ID
    std::uint32_t dwUCID = 0;       // +0x08: 角色 ID
    std::uint8_t _pad1[4] = {};     // +0x0C: padding
    std::int64_t biSerial = 0;      // +0x10: 序列号
    std::uint32_t dwItemID = 0;     // +0x18: 物品 ID
    std::uint8_t _pad2[4] = {};     // +0x1C: padding
};

static_assert(sizeof(ST_STATISTICS_MY_ROOM_ITEM) == 32, "ST_STATISTICS_MY_ROOM_ITEM size mismatch");

/**
 * 对齐 IDA cvdump: ST_STATISTICS_QUEST - 任务统计
 * Size = 16 bytes
 * 用于 SP_MISSION_DATA_SAVE 存储过程
 */
struct ST_STATISTICS_QUEST {
    std::uint8_t byFlag = 0;        // +0x00: 标志位
    std::uint8_t _pad0[3] = {};     // +0x01: padding
    std::uint32_t dwUCID = 0;       // +0x04: 角色 ID
    std::uint32_t dwEpisodeID = 0;  // +0x08: 剧集 ID
    std::uint8_t byLevel = 0;       // +0x0C: 等级
    std::uint8_t _pad1[3] = {};     // +0x0D: padding
};

static_assert(sizeof(ST_STATISTICS_QUEST) == 16, "ST_STATISTICS_QUEST size mismatch");

/**
 * 对齐 IDA cvdump: ST_STATISTICS_DAILY_MISSION - 每日任务统计
 * Size = 12 bytes
 * 用于 SP_DAILY_MISSION_DATA_SAVE 存储过程
 */
struct ST_STATISTICS_DAILY_MISSION {
    std::uint32_t dwUAID = 0;       // +0x00: 账号 ID
    std::uint32_t dwUCID = 0;       // +0x04: 角色 ID
    std::uint32_t dwMissionID = 0;  // +0x08: 任务 ID
};

static_assert(sizeof(ST_STATISTICS_DAILY_MISSION) == 12, "ST_STATISTICS_DAILY_MISSION size mismatch");

// ============================================================================
// 每日任务相关结构体 (对齐 IDA XSQLDailyMissionProcess)
// ============================================================================

/**
 * 对齐 IDA: ST_DAILY_MISSION_INFO - 每日任务信息
 * Size = 0x30 (48 bytes)
 */
struct ST_DAILY_MISSION_INFO {
    std::uint32_t dwMissionID = 0;      // +0x00: 任务 ID
    std::uint8_t byState = 0;           // +0x04: 任务状态
    std::uint8_t byAddHelper = 0;       // +0x05: 辅助标记
    std::uint8_t _pad[2] = {};          // +0x06: padding
    std::int16_t shValue = 0;           // +0x08: 任务值
    std::int8_t tAccept[8] = {};        // +0x0A: 接受时间 (CTime)
    std::int8_t _pad2[6] = {};          // +0x12: padding
    std::int8_t tStart[8] = {};         // +0x18: 开始时间 (CTime)
    std::int8_t _pad3[8] = {};          // +0x20: padding
    std::int8_t tEnd[8] = {};           // +0x28: 结束时间 (CTime)
    // Runtime fields (not persisted)
    std::uint32_t dwRemainTime = 0;     // 剩余时间 (秒)
    std::uint32_t dwDurationTime = 0;   // 持续时间 (秒)
};

/**
 * 对齐 IDA: PS_MAP_DAILY_MISSION - 每日任务 MAP 结构
 */
struct PS_MAP_DAILY_MISSION {
    std::map<std::uint32_t, ST_DAILY_MISSION_INFO> mapInfo;
};

/**
 * 对齐 IDA: PS_MAP_DISTRICT_DAILY_MISSION - 按区域分组的每日任务结构
 * Key: 区域ID (District_ID)
 * Value: 该区域的任务列表
 */
struct PS_MAP_DISTRICT_DAILY_MISSION {
    std::map<std::uint32_t, std::vector<ST_DAILY_MISSION_INFO>> mapDistrictMissions;
};

/**
 * 对齐 IDA: PS_DAILY_MISSION_UPDATE - 每日任务更新
 */
struct PS_DAILY_MISSION_UPDATE {
    std::vector<ST_DAILY_MISSION_INFO> vecInfo;
};

// ============================================================================
// 每周任务相关结构体 (对齐 IDA XSQLWeeklyMissionProcess)
// ============================================================================

/**
 * 对齐 IDA: ST_WEEKLY_MISSION_INFO - 每周任务信息
 */
struct ST_WEEKLY_MISSION_INFO {
    std::uint32_t dwMissionID = 0;   // +0x00: 任务 ID
    std::int16_t shValue = 0;       // +0x04: 任务值
    std::uint8_t byState = 0;       // +0x06: 任务状态
    std::uint8_t _pad = 0;
    std::int32_t biDate = 0;        // +0x08: 日期
    std::uint32_t dwUAID = 0;       // +0x0C: 账号 ID
    std::int32_t nDayID = 0;        // +0x10: 天 ID
};

/**
 * 对齐 IDA: ST_WEEKLY_MISSION_DAY_INFO - 每周任务日信息
 */
struct ST_WEEKLY_MISSION_DAY_INFO {
    std::uint16_t wDayID = 0;
    std::vector<ST_WEEKLY_MISSION_INFO> vecList;
};

/**
 * 对齐 IDA: ST_DB_WEEKLY_MISSION_UPDATE - 每周任务更新结构
 */
struct ST_DB_WEEKLY_MISSION_UPDATE {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::int32_t nDayID = 0;
    std::uint8_t byGroupID = 0;
    std::uint32_t dwMissionID = 0;
    std::int32_t nValue = 0;
    std::uint8_t byState = 0;
    std::uint8_t bAccount = 0;
};

/**
 * 对齐 IDA: PS_DB_WEEKLY_MISSION_UPDATE - 每周任务更新包
 */
struct PS_DB_WEEKLY_MISSION_UPDATE {
    std::vector<ST_DB_WEEKLY_MISSION_UPDATE> vecUpdate;
};

/**
 * 对齐 IDA: PS_DB_WEEKLY_MISSION_GROUP_INFO - 每周任务组信息
 */
struct PS_DB_WEEKLY_MISSION_GROUP_INFO {
    std::uint8_t byGroupID = 0;
    std::uint8_t byRewardState = 0;
    std::map<std::uint16_t, ST_WEEKLY_MISSION_DAY_INFO> mapList;
};

/**
 * 对齐 IDA: PS_DB_WEEKLY_MISSION_GROUP_INFO_REQ - 每周任务组信息请求
 */
struct PS_DB_WEEKLY_MISSION_GROUP_INFO_REQ {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t byGroupID = 0;
    bool bAccount = false;
    bool bLast = false;
};

/**
 * 对齐 IDA: PS_DB_WEEKLY_MISSION_GROUP_INFO_RES - 每周任务组信息响应
 */
struct PS_DB_WEEKLY_MISSION_GROUP_INFO_RES {
    std::uint32_t dwUCID = 0;
    bool bLast = false;
    std::int32_t nPlayCount = 0;
    std::int32_t nError = 0;
    PS_DB_WEEKLY_MISSION_GROUP_INFO stGroupInfo;
};

// ============================================================================
// 其他 DBAgent 相关结构体
// ============================================================================

/**
 * 对齐 IDA: PS_SG_NETCAFE_CHAR_INFO - 网吧角色信息
 * Size = 96 bytes (0x60)
 * 用于 P_CHARACTER_INFO_INSERT 存储过程
 */
struct PS_SG_NETCAFE_CHAR_INFO {
    std::int64_t nStoveID = 0;          // +0x00: 网吧 Stove ID
    char szUCID[21] = {};               // +0x08: 角色 ID (字符串形式)
    wchar_t szName[21] = {};            // +0x1E: 角色名
    std::int32_t nWorldID = 0;          // +0x48: 世界 ID
    std::int32_t nClass = 0;            // +0x4C: 职业
    std::uint8_t byAwaken = 0;          // +0x50: 觉醒等级
    std::uint8_t _pad1[3] = {};         // +0x51: padding
    std::int32_t nLevel = 0;            // +0x54: 等级
    std::int32_t nLogType = 0;          // +0x58: 日志类型
};

static_assert(sizeof(PS_SG_NETCAFE_CHAR_INFO) == 96, "PS_SG_NETCAFE_CHAR_INFO size mismatch");

/**
 * 对齐 IDA: ST_MODE_MAZE_EVENT_REWARD_INFO - 迷宫模式事件奖励信息
 * Size = 8 bytes
 */
struct ST_MODE_MAZE_EVENT_REWARD_INFO {
    unsigned int dwUCID = 0;        // +0x00: 角色 ID
    std::int32_t nPoint = 0;       // +0x04: 积分
};

static_assert(sizeof(ST_MODE_MAZE_EVENT_REWARD_INFO) == 8, "ST_MODE_MAZE_EVENT_REWARD_INFO size mismatch");

/**
 * 对齐 IDA: PS_MODE_MAZE_EVENT_REWARD_INFO - 迷宫模式事件奖励信息包
 * Size = 68 bytes
 * 用于 SP_OPERATION_EVENT_RANK_SETTING 存储过程
 */
struct PS_MODE_MAZE_EVENT_REWARD_INFO {
    std::int32_t nEventID = 0;                                 // +0x00: 事件 ID
    ST_MODE_MAZE_EVENT_REWARD_INFO arrInfo[8] = {};            // +0x04: 8个奖励信息
};

static_assert(sizeof(PS_MODE_MAZE_EVENT_REWARD_INFO) == 68, "PS_MODE_MAZE_EVENT_REWARD_INFO size mismatch");

/**
 * 对齐 IDA: ST_CLASS_EVENT_INFO - 职业事件信息
 * Size = 24 bytes
 */
struct ST_CLASS_EVENT_INFO {
    unsigned int dwUCID = 0;           // +0x00: 角色 ID
    std::uint8_t byType = 0;           // +0x04: 类型
    std::uint8_t _pad1[3] = {};        // +0x05: padding
    std::int32_t nValue = 0;           // +0x08: 数值
    std::int32_t nReward = 0;          // +0x0C: 奖励
    std::int64_t nLastUpdateDate = 0;  // +0x10: 最后更新日期
};

static_assert(sizeof(ST_CLASS_EVENT_INFO) == 24, "ST_CLASS_EVENT_INFO size mismatch");

/**
 * 对齐 IDA: ST_CLASS_EVENT_LIST - 职业事件列表
 * Size = 40 bytes
 */
struct ST_CLASS_EVENT_LIST {
    unsigned int dwUAID = 0;                   // +0x00: 账号 ID
    std::uint8_t _pad1[4] = {};                // +0x04: padding
    std::vector<ST_CLASS_EVENT_INFO> vecList;  // +0x08: 列表
};

static_assert(sizeof(ST_CLASS_EVENT_LIST) == 40, "ST_CLASS_EVENT_LIST size mismatch");

/**
 * 对齐 IDA: PS_PLAY_TIME_FOR_DAY - 每日游戏时间
 * Size = 32 bytes
 */
struct PS_PLAY_TIME_FOR_DAY {
    char szAccountID[21] = {};       // +0x00: 账号 ID
    std::uint8_t _pad1[3] = {};     // +0x15: padding
    unsigned int dwUCID = 0;        // +0x18: 角色 ID
    std::int32_t nSec = 0;          // +0x1C: 游戏秒数
};

static_assert(sizeof(PS_PLAY_TIME_FOR_DAY) == 32, "PS_PLAY_TIME_FOR_DAY size mismatch");

// ============================================================================
// DBAgent 序列化运算符
// ============================================================================

// PS_DB_GOLD_UPDATE 序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_GOLD_UPDATE& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.nAddGold;
    packet.XParse >> value.nTotalGold;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_GOLD_UPDATE& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.nAddGold;
    packet.XParse << value.nTotalGold;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_GOLD_UPDATE& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.nAddGold;
    packet.XParse << value.nTotalGold;
    return packet;
}

// PS_BP_INFO 序列化
inline XPacket& operator>>(XPacket& packet, PS_BP_INFO& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.biBP;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_BP_INFO& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.biBP;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_BP_INFO& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.biBP;
    return packet;
}

// PS_DB_BP_UPDATE 序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_BP_UPDATE& value) {
    packet >> value.psBP;
    packet.XParse >> value.nLimitMonsterBP;
    packet.XParse >> value.nLimitPVPBP;
    return packet;
}

// 称号相关序列化
inline XPacket& operator<<(XPacket& packet, const ST_TITLE_INFO_DB& value) {
    packet.XParse << value.dwTitleID;
    packet.XParse << value.sType;
    packet.XParse << value.bFavorite;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_TITLE_INFO_DB& value) {
    packet.XParse << value.dwTitleID;
    packet.XParse << value.sType;
    packet.XParse << value.bFavorite;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_TITLE_LOAD& value) {
    packet << value.stInsideTitle;
    packet << value.stOutsideTitle;
    packet.XParse << value.shFavoritePrefixCount;
    packet.XParse << value.shFavoriteSuffixCount;
    packet.XParse << static_cast<std::uint32_t>(value.vecTitleID.size());
    for (const auto& item : value.vecTitleID) {
        packet << item;
    }
    packet.XParse << static_cast<std::uint32_t>(value.vecOpenTitleID.size());
    for (const auto& item : value.vecOpenTitleID) {
        packet << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_TITLE_LOAD& value) {
    packet << value.stInsideTitle;
    packet << value.stOutsideTitle;
    packet.XParse << value.shFavoritePrefixCount;
    packet.XParse << value.shFavoriteSuffixCount;
    packet.XParse << static_cast<std::uint32_t>(value.vecTitleID.size());
    for (const auto& item : value.vecTitleID) {
        packet << item;
    }
    packet.XParse << static_cast<std::uint32_t>(value.vecOpenTitleID.size());
    for (const auto& item : value.vecOpenTitleID) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_TITLE_ADD& value) {
    packet.XParse >> value.dwTitleID;
    packet.XParse >> value.nLogType;
}

inline void operator>>(XPacket& packet, ST_TITLE_INFO_SELECT& value) {
    packet.XParse >> value.dwPrefix;
    packet.XParse >> value.dwSuffix;
}

inline XPacket& operator<<(XPacket& packet, const ST_TITLE_INFO_SELECT& value) {
    packet.XParse << value.dwPrefix;
    packet.XParse << value.dwSuffix;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_TITLE_INFO_SELECT& value) {
    packet.XParse << value.dwPrefix;
    packet.XParse << value.dwSuffix;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_TITLE_UPDATE& value) {
    packet >> value.stInsideTitle;
    packet >> value.stOutsideTitle;
}

inline XPacket& operator<<(XPacket& packet, const PS_REQ_TITLE_UPDATE& value) {
    packet << value.stInsideTitle;
    packet << value.stOutsideTitle;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_REQ_TITLE_UPDATE& value) {
    packet << value.stInsideTitle;
    packet << value.stOutsideTitle;
    return packet;
}

inline void operator>>(XPacket& packet, PS_TITLE_FAVORITE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwTitleID;
    packet.XParse >> value.bFavorite;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_TITLE_FAVORITE& value) {
    packet.XParse << value.psInfo.dwUCID;
    packet.XParse << value.psInfo.dwTitleID;
    packet.XParse << value.psInfo.bFavorite;
    packet.XParse << value.nError;
    return packet;
}

// 角色位置更新序列化
inline void operator>>(XPacket& packet, PS_CHARACTER_UPDATE_POS& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.vPos.x;
    packet.XParse >> value.vPos.y;
    packet.XParse >> value.vPos.z;
    packet.XParse >> value.nRot;
    packet.XParse >> value.nJumpID;
    packet.XParse >> value.dwServerID;
}

inline void operator>>(XPacket& packet, PS_UPDATE_INFINITETOWER_LIMIT_TIME& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.nLimitTime;
}

inline void operator>>(XPacket& packet, PS_UPDATE_INFINITETOWER_CLEAR_CHAPTER& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.sClearChapter;
    packet.XParse >> value.sClearStage;
}

// 迷宫/区域状态序列化
inline void operator>>(XPacket& packet, ST_MAZE_STATE& value) {
    packet.XParse >> value.nMazeID;
    packet.XParse >> value.nClearCount;
    packet.XParse >> value.nSharePoint;
    std::int16_t shRewardStep = 0;
    packet.XParse >> shRewardStep;
    value.byRewardStep = static_cast<std::int8_t>(shRewardStep);
    packet.XParse >> value.nGroupID;
    packet.XParse >> value.nDistrictID;
}

inline XPacket& operator<<(XPacket& packet, const ST_MAZE_STATE& value) {
    packet.XParse << value.nMazeID;
    packet.XParse << value.nClearCount;
    packet.XParse << value.nSharePoint;
    packet.XParse << value.byRewardStep;
    packet.XParse << value.nGroupID;
    packet.XParse << value.nDistrictID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_DISTRICT_STATE& value) {
    packet.XParse >> value.nDistrictID;
    packet.XParse >> value.nDSPoint;
    std::int16_t shReward = 0;
    packet.XParse >> shReward;
    value.bReward = (shReward != 0);
}

inline XPacket& operator<<(XPacket& packet, const ST_DISTRICT_STATE& value) {
    packet.XParse << value.nDistrictID;
    packet.XParse << value.nDSPoint;
    packet.XParse << static_cast<std::int16_t>(value.bReward ? 1 : 0);
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_MAZE_STATE_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& item : value.vecList) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_DISTRICT_STATE_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& item : value.vecList) {
        packet << item;
    }
    return packet;
}

// 成就相关序列化
inline void operator>>(XPacket& packet, ST_ACHIEVE_INFO& value) {
    packet.XParse >> value.nIndex;
    packet.XParse >> value.biCount;
}

inline XPacket& operator<<(XPacket& packet, const ST_ACHIEVE_INFO& value) {
    packet.XParse << value.nIndex;
    packet.XParse << value.biCount;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_ACHIEVE_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& item : value.vecList) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_ACHIEVE_BIT& value) {
    for (int i = 0; i < 128; ++i) {
        packet.XParse >> value.szRewardBit[i];
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_ACHIEVE_BIT& value) {
    for (int i = 0; i < 128; ++i) {
        packet.XParse << value.szRewardBit[i];
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_ACHIEVE_INFO& value) {
    packet.XParse << value.nIndex;
    packet.XParse << value.biCount;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_ACHIEVE_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& item : value.vecList) {
        packet << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_ACHIEVE_BIT& value) {
    for (int i = 0; i < 128; ++i) {
        packet.XParse << value.szRewardBit[i];
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_ACHIEVE_UPDATE& value) {
    packet >> value.stUpdateInfo;
    packet.XParse >> value.nNextIndex;
    packet.XParse >> value.byCategory;
    packet.XParse >> value.wCount;
}

inline void operator>>(XPacket& packet, ST_ACHIEVE_UPDATE_LIST& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecList.clear();
    value.vecList.reserve(static_cast<std::size_t>(nCount));
    for (int i = 0; i < nCount; ++i) {
        ST_ACHIEVE_UPDATE item;
        packet >> item;
        value.vecList.push_back(item);
    }
}

// 无限塔序列化
inline XPacket& operator<<(XPacket& packet, const PS_INFINITE_TOWER_INFO& value) {
    packet.XParse << value.sClearChapter;
    packet.XParse << value.sClearStage;
    packet.XParse << value.nLimitTime;
    packet.XParse << value.sCount;
    return packet;
}

inline void operator>>(XPacket& packet, PS_INFINITE_TOWER_INFO& value) {
    packet.XParse >> value.sClearChapter;
    packet.XParse >> value.sClearStage;
    packet.XParse >> value.nLimitTime;
    packet.XParse >> value.sCount;
}

// 击杀用户序列化
inline XPacket& operator<<(XPacket& packet, const PS_KILLED_USER_INFO& value) {
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << value.nCount;
    return packet;
}

inline void operator>>(XPacket& packet, PS_KILLED_USER_INFO& value) {
    int nUCID = 0;
    packet.XParse >> nUCID;
    value.dwUCID = static_cast<std::uint32_t>(nUCID);
    packet.XParse >> value.nCount;
}

inline XPacket& operator<<(XPacket& packet, const PS_KILLED_USER_INFOS& value) {
    packet.XParse << value.nInitTime;
    packet.XParse << static_cast<int>(value.vecKilledUser.size());
    for (const auto& item : value.vecKilledUser) {
        packet << item;
    }
    return packet;
}

// 代表角色序列化
inline XPacket& operator<<(XPacket& packet, const ST_REPRESENTATIVE_INFO& value) {
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << static_cast<int>(value.byClass);
    packet.XParse << static_cast<int>(value.byLevel);
    packet.XParse << std::wstring(value.strName);
    packet.XParse << static_cast<int>(value.dwProfilePhotoID);
    packet.XParse << std::wstring(value.strLeagueName);
    return packet;
}

inline void operator>>(XPacket& packet, ST_REPRESENTATIVE_INFO& value) {
    int nUCID = 0, nClass = 0, nLevel = 0, nPhotoID = 0;
    packet.XParse >> nUCID;
    packet.XParse >> nClass;
    packet.XParse >> nLevel;
    value.dwUCID = static_cast<std::uint32_t>(nUCID);
    value.byClass = static_cast<std::uint8_t>(nClass);
    value.byLevel = static_cast<std::uint8_t>(nLevel);
    std::wstring strName;
    packet.XParse >> strName;
    std::wcsncpy(value.strName, strName.c_str(), 20);
    value.strName[20] = L'\0';
    packet.XParse >> nPhotoID;
    value.dwProfilePhotoID = static_cast<std::uint32_t>(nPhotoID);
    std::wstring strLeague;
    packet.XParse >> strLeague;
    std::wcsncpy(value.strLeagueName, strLeague.c_str(), 9);
    value.strLeagueName[9] = L'\0';
}

// 职业场景序列化
inline void operator>>(XPacket& packet, PS_CLASS_SCENE& value) {
    for (int i = 0; i < 6; ++i) {
        packet.XParse >> value.byClassScene[i];
    }
}

// 物品限制序列化
inline XPacket& operator<<(XPacket& packet, const PS_ITEM_LIMIT& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value.biInitDate;
    return packet;
}

inline void operator>>(XPacket& packet, PS_ITEM_LIMIT& value) {
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse >> value.biInitDate;
}

inline XPacket& operator<<(XPacket& packet, const ST_ITEM_LIMIT_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& item : value.vecList) {
        packet << item;
    }
    return packet;
}

// PS_DB_USE_ITEM_APPREARANCE 序列化
inline void operator>>(XPacket& packet, PS_DB_USE_ITEM_APPREARANCE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nErrorCode;
    packet >> value.psUpdateItemList;
    packet.XParse >> value.stAppearanceInfo.wAppearanceID;
    packet.XParse >> value.stAppearanceInfo.biEndDate;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_USE_ITEM_APPREARANCE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nErrorCode;
    packet << value.psUpdateItemList;
    packet.XParse << value.stAppearanceInfo.wAppearanceID;
    packet.XParse << value.stAppearanceInfo.biEndDate;
    return packet;
}

// 统计结构反序列化
inline void operator>>(XPacket& packet, ST_STATISTICS_ITEM_EXCHANGE& value) {
    packet.XParse >> value.byFlag;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwEXC_ID;
    packet.XParse >> value.biSerial;
    packet.XParse >> value.nExpireHour;
}

inline void operator>>(XPacket& packet, ST_STATISTICS_HELPER& value) {
    packet.XParse >> value.byFlag;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwHelperID;
}

inline void operator>>(XPacket& packet, ST_STATISTICS_MY_ROOM& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.byLevel;
}

inline void operator>>(XPacket& packet, ST_STATISTICS_AKASHIC& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwAkashicID;
    packet.XParse >> value.byFlag;
}

inline void operator>>(XPacket& packet, ST_STATISTICS_MY_ROOM_ITEM& value) {
    packet.XParse >> value.byFlag;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.biSerial;
    packet.XParse >> value.dwItemID;
}

inline void operator>>(XPacket& packet, ST_STATISTICS_QUEST& value) {
    packet.XParse >> value.byFlag;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwEpisodeID;
    packet.XParse >> value.byLevel;
}

inline void operator>>(XPacket& packet, ST_STATISTICS_DAILY_MISSION& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwMissionID;
}

inline XPacket& operator<<(XPacket& packet, const ST_STATISTICS_DAILY_MISSION& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwMissionID;
    return packet;
}

// 每日任务序列化
inline void operator>>(XPacket& packet, ST_DAILY_MISSION_INFO& value) {
    packet.XParse >> value.dwMissionID;
    packet.XParse >> value.byState;
    packet.XParse >> value.byAddHelper;
    packet.XParse >> value.shValue;
}

inline XPacket& operator<<(XPacket& packet, const ST_DAILY_MISSION_INFO& value) {
    packet.XParse << value.dwMissionID;
    packet.XParse << value.byState;
    packet.XParse << value.byAddHelper;
    packet.XParse << value.shValue;
    return packet;
}

inline void operator>>(XPacket& packet, PS_MAP_DAILY_MISSION& value) {
    std::uint16_t wCount = 0;
    packet.XParse >> wCount;
    for (std::uint16_t i = 0; i < wCount; ++i) {
        std::uint32_t dwKey = 0;
        packet.XParse >> dwKey;
        ST_DAILY_MISSION_INFO info;
        packet >> info;
        value.mapInfo[dwKey] = info;
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_MAP_DAILY_MISSION& value) {
    packet.XParse << static_cast<std::uint16_t>(value.mapInfo.size());
    for (const auto& pair : value.mapInfo) {
        packet.XParse << pair.first;
        packet << pair.second;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_DAILY_MISSION_UPDATE& value) {
    std::uint16_t wCount = 0;
    packet.XParse >> wCount;
    for (std::uint16_t i = 0; i < wCount; ++i) {
        ST_DAILY_MISSION_INFO info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_DAILY_MISSION_UPDATE& value) {
    packet.XParse << static_cast<std::uint16_t>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

// 每周任务序列化
inline void operator>>(XPacket& packet, ST_WEEKLY_MISSION_INFO& value) {
    packet.XParse >> value.dwMissionID;
    packet.XParse >> value.shValue;
    packet.XParse >> value.byState;
    packet.XParse >> value.biDate;
}

inline XPacket& operator<<(XPacket& packet, const ST_WEEKLY_MISSION_INFO& value) {
    packet.XParse << value.dwMissionID;
    packet.XParse << value.shValue;
    packet.XParse << value.byState;
    packet.XParse << value.biDate;
    return packet;
}

inline void operator>>(XPacket& packet, ST_DB_WEEKLY_MISSION_UPDATE& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nDayID;
    packet.XParse >> value.byGroupID;
    packet.XParse >> value.dwMissionID;
    packet.XParse >> value.nValue;
    packet.XParse >> value.byState;
    packet.XParse >> value.bAccount;
}

inline void operator>>(XPacket& packet, PS_DB_WEEKLY_MISSION_UPDATE& value) {
    std::uint16_t wCount = 0;
    packet.XParse >> wCount;
    for (std::uint16_t i = 0; i < wCount; ++i) {
        ST_DB_WEEKLY_MISSION_UPDATE info;
        packet >> info;
        value.vecUpdate.push_back(info);
    }
}

inline void operator>>(XPacket& packet, PS_DB_WEEKLY_MISSION_GROUP_INFO_REQ& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byGroupID;
    packet.XParse >> value.bAccount;
    packet.XParse >> value.bLast;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_WEEKLY_MISSION_GROUP_INFO_RES& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.bLast;
    packet.XParse << value.nPlayCount;
    packet.XParse << value.nError;
    packet.XParse << value.stGroupInfo.byGroupID;
    packet.XParse << value.stGroupInfo.byRewardState;
    packet.XParse << static_cast<std::uint16_t>(value.stGroupInfo.mapList.size());
    for (const auto& pair : value.stGroupInfo.mapList) {
        packet.XParse << pair.first;
        packet.XParse << static_cast<std::uint16_t>(pair.second.vecList.size());
        for (const auto& info : pair.second.vecList) {
            packet << info;
        }
    }
    return packet;
}

// 网吧角色信息序列化
inline XPacket& operator>>(XPacket& packet, PS_SG_NETCAFE_CHAR_INFO& value) {
    packet.XParse >> reinterpret_cast<std::uint64_t&>(value.nStoveID);
    std::int16_t shLen = 0;
    packet.XParse.GetString(value.szUCID, 21, &shLen);
    packet.XParse.GetWString(value.szName, 21, shLen);
    packet.XParse >> value.nWorldID;
    packet.XParse >> value.nClass;
    packet.XParse >> value.byAwaken;
    packet.XParse >> value.nLevel;
    packet.XParse >> value.nLogType;
    return packet;
}

// 迷宫模式事件奖励序列化
inline XPacket& operator>>(XPacket& packet, PS_MODE_MAZE_EVENT_REWARD_INFO& value) {
    packet.XParse >> value.nEventID;
    for (int i = 0; i < 8; ++i) {
        packet.XParse >> value.arrInfo[i].dwUCID;
        packet.XParse >> value.arrInfo[i].nPoint;
    }
    return packet;
}

// 职业事件序列化
inline XPacket& operator>>(XPacket& packet, ST_CLASS_EVENT_INFO& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byType;
    packet.XParse >> value.nValue;
    packet.XParse >> value.nReward;
    packet.XParse >> value.nLastUpdateDate;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_CLASS_EVENT_LIST& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << static_cast<std::uint16_t>(value.vecList.size());
    for (const auto& info : value.vecList) {
        packet.XParse << info.dwUCID;
        packet.XParse << info.byType;
        packet.XParse << info.nValue;
        packet.XParse << info.nReward;
        packet.XParse << info.nLastUpdateDate;
    }
    return packet;
}

// 每日游戏时间序列化
inline XPacket& operator>>(XPacket& packet, PS_PLAY_TIME_FOR_DAY& value) {
    std::int16_t shLen = 0;
    packet.XParse.GetString(value.szAccountID, 21, &shLen);
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nSec;
    return packet;
}

// ============================================================================
// DBAgent: 轮盘事件更新相关结构
// ============================================================================

/**
 * @brief 轮盘事件DB更新 - 112 bytes
 * 对齐 IDA DBAgent.exe
 */
struct PS_DB_ROULETTE_EVENT_UPDATE {
    PS_RES_STORAGE_INFO psUpdateItemList{};   // +0x00 (40 bytes)
    ST_CREATE_ITEMS stCreateItems{};          // +0x28 (32 bytes)
    std::uint32_t dwUCID = 0;                // +0x48
    std::uint32_t dwUAID = 0;                // +0x4C
    std::uint8_t byUseType = 0;              // +0x50
    std::uint8_t _pad0[3] = {};              // +0x51 (padding to 0x54)
    int nEventID = 0;                        // +0x54
    int nRewardID = 0;                       // +0x58
    int nDayCount = 0;                       // +0x5C
    int nErrorCode = 0;                      // +0x60
    int nRemainCount = 0;                    // +0x64
    std::int64_t biUpdateDate = 0;           // +0x68 (注意: IDA 显示 offset 104 = 0x68)
};

static_assert(sizeof(PS_DB_ROULETTE_EVENT_UPDATE) == 112, "PS_DB_ROULETTE_EVENT_UPDATE size must match IDA");

// PS_DB_ROULETTE_EVENT_UPDATE 反序列化
inline void operator>>(XPacket& packet, PS_DB_ROULETTE_EVENT_UPDATE& value) {
    packet >> value.psUpdateItemList;
    packet >> value.stCreateItems;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.byUseType;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.nEventID;
    packet.XParse >> value.nRewardID;
    packet.XParse >> value.nDayCount;
    packet.XParse >> value.nErrorCode;
    packet.XParse >> value.nRemainCount;
    packet.XParse >> value.biUpdateDate;
}

// PS_DB_ROULETTE_EVENT_UPDATE XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_ROULETTE_EVENT_UPDATE& value) {
    packet << value.psUpdateItemList;
    packet << value.stCreateItems;
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.byUseType;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.nEventID;
    packet.XParse << value.nRewardID;
    packet.XParse << value.nDayCount;
    packet.XParse << value.nErrorCode;
    packet.XParse << value.nRemainCount;
    packet.XParse << value.biUpdateDate;
    return packet;
}

// PS_DB_ROULETTE_EVENT_UPDATE XPacket 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_ROULETTE_EVENT_UPDATE& value) {
    packet << value.psUpdateItemList;
    packet << value.stCreateItems;
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.byUseType;
    packet.XParse << value.nEventID;
    packet.XParse << value.nRewardID;
    packet.XParse << value.nDayCount;
    packet.XParse << value.nErrorCode;
    packet.XParse << value.nRemainCount;
    packet.XParse << value.biUpdateDate;
    return packet;
}

// ============================================================================
// 网吧(NetCafe)序列化运算符
// ============================================================================

// PS_DB_HAN_NET_CAFE 序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_HAN_NET_CAFE& value) {
    packet.XParse >> value.dwUAID;
    std::int16_t shLen = 0;
    packet.XParse.GetString(value.szAccountID, 21, &shLen);
    packet.XParse >> value.nServerID;
    packet.XParse >> value.bLogin;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_HAN_NET_CAFE& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << GreenDamTan_BoundedString(value.szAccountID);
    packet.XParse << value.nServerID;
    packet.XParse << value.bLogin;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    return packet;
}

// PS_DB_HAN_NET_CAFE_LIST 序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_HAN_NET_CAFE_LIST& value) {
    std::int16_t shCount = 0;
    packet.XParse >> shCount;
    value.vecList.clear();
    value.vecList.reserve(static_cast<std::size_t>(shCount));
    for (std::int16_t i = 0; i < shCount; ++i) {
        PS_DB_HAN_NET_CAFE item{};
        packet >> item;
        value.vecList.push_back(item);
    }
    packet.XParse >> value.bLast;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_HAN_NET_CAFE_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& item : value.vecList) {
        packet << item;
    }
    packet.XParse << value.bLast;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    return packet;
}

// ============================================================================
// 世界事件(WorldEvent)序列化运算符
// ============================================================================

// ST_WORLD_EVENT_REWARD_INFO 序列化
inline XPacket& operator>>(XPacket& packet, ST_WORLD_EVENT_REWARD_INFO& value) {
    packet.XParse >> value.nRewardIndex;
    packet.XParse >> value.byRewardType;
    packet.XParse >> value.byRewardState;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_WORLD_EVENT_REWARD_INFO& value) {
    packet.XParse << value.nRewardIndex;
    packet.XParse << value.byRewardType;
    packet.XParse << value.byRewardState;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    return packet;
}

// PS_WORLD_EVENT_INFO_RES 序列化
inline XPacket& operator>>(XPacket& packet, PS_WORLD_EVENT_INFO_RES& value) {
    packet.XParse >> value.nEventID;
    packet.XParse >> value.nTotalCount;
    packet.XParse >> value.nMyCount;
    packet.XParse >> value.byDailyRewardState;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    std::int8_t cCount = 0;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&cCount), 1);
    value.vecRewardInfo.clear();
    value.vecRewardInfo.reserve(static_cast<std::size_t>(cCount));
    for (std::int8_t i = 0; i < cCount; ++i) {
        ST_WORLD_EVENT_REWARD_INFO item{};
        packet >> item;
        value.vecRewardInfo.push_back(item);
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_WORLD_EVENT_INFO_RES& value) {
    packet.XParse << value.nEventID;
    packet.XParse << value.nTotalCount;
    packet.XParse << value.nMyCount;
    packet.XParse << value.byDailyRewardState;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    packet.XParse << static_cast<std::int8_t>(value.vecRewardInfo.size());
    for (const auto& item : value.vecRewardInfo) {
        packet << item;
    }
    return packet;
}

// PS_DB_WORLD_EVENT_INFO_REQ 序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_WORLD_EVENT_INFO_REQ& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nEventID;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_WORLD_EVENT_INFO_REQ& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.nEventID;
    return packet;
}

// PS_DB_WORLD_EVENT_INFO_RES 序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_WORLD_EVENT_INFO_RES& value) {
    packet >> value.psInfo;
    packet.XParse >> value.biLastRegisterDate;
    packet.XParse >> value.biDailyRewardDate;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_WORLD_EVENT_INFO_RES& value) {
    packet << value.psInfo;
    packet.XParse << value.biLastRegisterDate;
    packet.XParse << value.biDailyRewardDate;
    return packet;
}

// ============================================================================
// DBAgent: 物品移动相关结构
// ============================================================================

// 对齐 IDA: 物品移动数据库结构 (280 bytes)
struct PS_DB_ITEM_MOVE {
    std::uint8_t bySyncType = 0;        // offset 0x00
    std::uint8_t _pad0[3] = {};         // padding
    std::uint32_t dwSrcActorID = 0;     // offset 0x04
    std::uint8_t bySrcInvenType = 0;    // offset 0x08
    std::uint8_t _pad1 = 0;             // padding
    std::int16_t shSrcSlotPos = 0;      // offset 0x0A
    std::uint8_t _pad2[4] = {};         // padding
    STItem stSrcItem{};                 // offset 0x10 (120 bytes)
    std::uint8_t bySrcStoreType = 0;    // offset 0x88
    std::uint8_t _pad3[3] = {};         // padding
    std::uint32_t dwDestActorID = 0;    // offset 0x8C
    std::uint8_t byDestInvenType = 0;   // offset 0x90
    std::uint8_t _pad4 = 0;             // padding
    std::int16_t shDestSlotPos = 0;     // offset 0x92
    std::uint8_t _pad5[4] = {};         // padding
    STItem stDestItem{};                // offset 0x98 (120 bytes)
    std::uint8_t byDestStoreType = 0;   // offset 0x110
    bool bSyncToClient = false;         // offset 0x111
    std::uint8_t _pad6[6] = {};         // padding to 280 bytes
};

static_assert(sizeof(PS_DB_ITEM_MOVE) == 280, "PS_DB_ITEM_MOVE size must match IDA");

// 对齐 IDA: 物品移动列表 (32 bytes)
struct PS_DB_ITEM_MOVE_VEC {
    std::vector<PS_DB_ITEM_MOVE> vecItem;
};

// ============================================================================
// DBAgent: 位置检查相关结构
// ============================================================================

// 对齐 IDA PS_DB_CHECK_LOCATION (112 bytes)
// IDA 偏移: dwUAID=0, dwUCID=4, uxMapID=8, wMapID=16, nServerID=20, szAccountID=24, strName=46, szIP=88
// 注意: strName(wchar_t[21]) 在 offset 46，非对齐位置，需要 pack(1)
#pragma pack(push, 1)
struct PS_DB_CHECK_LOCATION {
    std::uint32_t dwUAID = 0;           // offset 0x00 (4 bytes)
    std::uint32_t dwUCID = 0;           // offset 0x04 (4 bytes)
    std::int64_t uxMapID = 0;           // offset 0x08 (8 bytes)
    std::uint16_t wMapID = 0;           // offset 0x10 (2 bytes)
    std::uint8_t _pad0[2] = {};         // offset 0x12 (2 bytes padding)
    std::int32_t nServerID = 0;         // offset 0x14 (4 bytes)
    char szAccountID[21] = {};          // offset 0x18 (21 bytes), ends at 0x2D
    std::uint8_t _pad1[1] = {};         // offset 0x2D (1 byte padding)
    wchar_t szName[21] = {};            // offset 0x2E (42 bytes), ends at 0x58
    char szIP[21] = {};                 // offset 0x58 (21 bytes), ends at 0x6D
    std::uint8_t _pad2[3] = {};         // offset 0x6D (3 bytes padding to 112)
};
#pragma pack(pop)

static_assert(sizeof(PS_DB_CHECK_LOCATION) == 112, "PS_DB_CHECK_LOCATION size must match IDA");

// PS_DB_CHECK_LOCATION 反序列化
inline void operator>>(XPacket& packet, PS_DB_CHECK_LOCATION& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.uxMapID;
    packet.XParse >> value.wMapID;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.nServerID;
    packet.XParse.GetBytes(value.szAccountID, 21);
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad1), sizeof(value._pad1));
    short outLen = 0;
    packet.XParse.GetWString(value.szName, 21, outLen);
    packet.XParse.GetBytes(value.szIP, 21);
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad2), sizeof(value._pad2));
}

// PS_DB_CHECK_LOCATION 序列化（DBAgent 发回）
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_CHECK_LOCATION& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.uxMapID;
    packet.XParse << value.wMapID;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.nServerID;
    packet.XParse.SetBytes(value.szAccountID, 21);
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad1), sizeof(value._pad1));
    packet.XParse << GreenDamTan_BoundedWideString(value.szName);
    packet.XParse.SetBytes(value.szIP, 21);
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad2), sizeof(value._pad2));
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_CHECK_LOCATION& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.uxMapID;
    packet.XParse << value.wMapID;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.nServerID;
    packet.XParse.SetBytes(value.szAccountID, 21);
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad1), sizeof(value._pad1));
    packet.XParse << GreenDamTan_BoundedWideString(value.szName);
    packet.XParse.SetBytes(value.szIP, 21);
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad2), sizeof(value._pad2));
    return packet;
}

// PS_DB_ITEM_MOVE 序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_ITEM_MOVE& value) {
    packet.XParse >> value.bySyncType;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.dwSrcActorID;
    packet.XParse >> value.bySrcInvenType;
    packet.XParse >> value._pad1;
    packet.XParse >> value.shSrcSlotPos;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad2), sizeof(value._pad2));
    packet >> value.stSrcItem;
    packet.XParse >> value.bySrcStoreType;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad3), sizeof(value._pad3));
    packet.XParse >> value.dwDestActorID;
    packet.XParse >> value.byDestInvenType;
    packet.XParse >> value._pad4;
    packet.XParse >> value.shDestSlotPos;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad5), sizeof(value._pad5));
    packet >> value.stDestItem;
    packet.XParse >> value.byDestStoreType;
    packet.XParse >> value.bSyncToClient;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad6), sizeof(value._pad6));
    return packet;
}

// PS_DB_ITEM_MOVE_VEC 序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_ITEM_MOVE_VEC& value) {
    std::int16_t shCount = 0;
    packet.XParse >> shCount;
    value.vecItem.clear();
    value.vecItem.reserve(static_cast<std::size_t>(shCount));
    for (std::int16_t i = 0; i < shCount; ++i) {
        PS_DB_ITEM_MOVE item{};
        packet >> item;
        value.vecItem.push_back(item);
    }
    return packet;
}

// PS_DB_ITEM_MOVE XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_ITEM_MOVE& value) {
    packet.XParse << value.bySyncType;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.dwSrcActorID;
    packet.XParse << value.bySrcInvenType;
    packet.XParse << value._pad1;
    packet.XParse << value.shSrcSlotPos;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad2), sizeof(value._pad2));
    packet << value.stSrcItem;
    packet.XParse << value.bySrcStoreType;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad3), sizeof(value._pad3));
    packet.XParse << value.dwDestActorID;
    packet.XParse << value.byDestInvenType;
    packet.XParse << value._pad4;
    packet.XParse << value.shDestSlotPos;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad5), sizeof(value._pad5));
    packet << value.stDestItem;
    packet.XParse << value.byDestStoreType;
    packet.XParse << value.bSyncToClient;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad6), sizeof(value._pad6));
    return packet;
}

// PS_DB_ITEM_MOVE_VEC XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_ITEM_MOVE_VEC& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecItem.size());
    for (const auto& item : value.vecItem) {
        packet << item;
    }
    return packet;
}

// ============================================================================
// DBAgent: 阿卡夏记录相关结构
// ============================================================================

// 对齐 IDA ST_AKASHIC_RECORD (12 bytes)
struct ST_AKASHIC_RECORD {
    std::uint32_t dwAkashicID = 0;      // offset 0x00: 阿卡夏ID
    std::int32_t nPosition = 0;         // offset 0x04: 位置
    std::int32_t nAkashicExp = 0;       // offset 0x08: 经验值
};
static_assert(sizeof(ST_AKASHIC_RECORD) == 12, "ST_AKASHIC_RECORD size must match IDA");

// 对齐 IDA ST_AKASHIC_LIST (32 bytes)
struct ST_AKASHIC_LIST {
    std::vector<ST_AKASHIC_RECORD> vecInfo;
};
static_assert(sizeof(ST_AKASHIC_LIST) == 32, "ST_AKASHIC_LIST size must match IDA");

// ST_AKASHIC_RECORD 序列化运算符
inline void operator>>(XPacket& packet, ST_AKASHIC_RECORD& value) {
    packet.XParse >> value.dwAkashicID;
    packet.XParse >> value.nPosition;
    packet.XParse >> value.nAkashicExp;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_AKASHIC_RECORD& value) {
    packet.XParse << value.dwAkashicID;
    packet.XParse << value.nPosition;
    packet.XParse << value.nAkashicExp;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_AKASHIC_RECORD& value) {
    packet.XParse << value.dwAkashicID;
    packet.XParse << value.nPosition;
    packet.XParse << value.nAkashicExp;
    return packet;
}

// ST_AKASHIC_LIST 序列化运算符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_AKASHIC_LIST& value) {
    std::int16_t shCount = static_cast<std::int16_t>(value.vecInfo.size());
    packet.XParse << shCount;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_AKASHIC_LIST& value) {
    std::int16_t shCount = static_cast<std::int16_t>(value.vecInfo.size());
    packet.XParse << shCount;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_AKASHIC_LIST& value) {
    std::int16_t shCount = 0;
    packet.XParse >> shCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(shCount));
    for (std::int16_t i = 0; i < shCount; ++i) {
        ST_AKASHIC_RECORD item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

// ============================================================================
// DBAgent: 世界事件每日奖励相关结构
// ============================================================================

// 对齐 IDA: 世界事件每日奖励 (104 bytes)
struct PS_DB_WORLD_EVENT_DAILY_REWARD {
    std::uint32_t dwUCID = 0;                      // offset 0x00: 角色ID
    std::int32_t nEventID = 0;                     // offset 0x04: 事件ID
    std::int64_t biDailyRewardDate = 0;            // offset 0x08: 每日奖励日期
    PS_RES_STORAGE_INFO stCreateItem{};            // offset 0x10: 创建物品 (40 bytes)
    PS_RES_STORAGE_INFO stUpdateItem{};            // offset 0x38: 更新物品 (40 bytes)
    std::int32_t nError = 0;                       // offset 0x60: 错误码
    std::uint8_t byItemFlag = 0;                   // offset 0x64: 物品标记
};

static_assert(sizeof(PS_DB_WORLD_EVENT_DAILY_REWARD) == 104, "PS_DB_WORLD_EVENT_DAILY_REWARD size must match IDA");

// PS_DB_WORLD_EVENT_DAILY_REWARD 序列化运算符
inline void operator>>(XPacket& packet, PS_DB_WORLD_EVENT_DAILY_REWARD& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nEventID;
    packet.XParse >> value.biDailyRewardDate;
    packet >> value.stCreateItem;
    packet >> value.stUpdateItem;
    packet.XParse >> value.nError;
    packet.XParse >> value.byItemFlag;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_WORLD_EVENT_DAILY_REWARD& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nEventID;
    packet.XParse << value.biDailyRewardDate;
    packet << value.stCreateItem;
    packet << value.stUpdateItem;
    packet.XParse << value.nError;
    packet.XParse << value.byItemFlag;
    return packet;
}

// ============================================================================
// DBAgent: 轮盘事件相关结构
// ============================================================================

// 对齐 IDA: 轮盘信息 (16 bytes)
struct PS_ROULETTE_INFO {
    std::int32_t nDayCount = 0;                    // offset 0x00: 日计数
    std::uint8_t _pad0[4] = {};                    // padding
    std::int64_t biRegDate = 0;                    // offset 0x08: 注册日期
};

static_assert(sizeof(PS_ROULETTE_INFO) == 16, "PS_ROULETTE_INFO size must match IDA");

// 对齐 IDA: 轮盘事件信息 (32 bytes)
struct PS_DB_ROULETTE_EVENT_INFO {
    std::uint32_t dwUCID = 0;                      // offset 0x00: 角色ID
    std::uint32_t dwUAID = 0;                      // offset 0x04: 账号ID
    std::int32_t nEventID = 0;                     // offset 0x08: 事件ID
    std::int32_t nErrorCode = 0;                   // offset 0x0C: 错误码
    PS_ROULETTE_INFO psRoulettInfo{};              // offset 0x10: 轮盘信息 (16 bytes)
};

static_assert(sizeof(PS_DB_ROULETTE_EVENT_INFO) == 32, "PS_DB_ROULETTE_EVENT_INFO size must match IDA");

// 对齐 IDA: 轮盘奖励信息 (32 bytes)
struct PS_DB_ROULETTE_REWARD_INFO {
    std::vector<PS_ROULETTE_EVENT_UPDATE_SERVER> vecInfo;
};

static_assert(sizeof(PS_DB_ROULETTE_REWARD_INFO) == 32, "PS_DB_ROULETTE_REWARD_INFO size must match IDA");

// PS_ROULETTE_INFO 序列化运算符
inline void operator>>(XPacket& packet, PS_ROULETTE_INFO& value) {
    packet.XParse >> value.nDayCount;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.biRegDate;
}

inline XPacket& operator<<(XPacket& packet, const PS_ROULETTE_INFO& value) {
    packet.XParse << value.nDayCount;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.biRegDate;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_ROULETTE_INFO& value) {
    packet.XParse << value.nDayCount;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.biRegDate;
    return packet;
}

// PS_DB_ROULETTE_EVENT_INFO 序列化运算符
inline void operator>>(XPacket& packet, PS_DB_ROULETTE_EVENT_INFO& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.nEventID;
    packet.XParse >> value.nErrorCode;
    packet >> value.psRoulettInfo;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_ROULETTE_EVENT_INFO& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.nEventID;
    packet.XParse << value.nErrorCode;
    packet << value.psRoulettInfo;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_ROULETTE_EVENT_INFO& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.nEventID;
    packet.XParse << value.nErrorCode;
    packet << value.psRoulettInfo;
    return packet;
}

// PS_DB_ROULETTE_REWARD_INFO 序列化运算符
inline void operator>>(XPacket& packet, PS_DB_ROULETTE_REWARD_INFO& value) {
    std::int16_t shCount = 0;
    packet.XParse >> shCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(shCount));
    for (std::int16_t i = 0; i < shCount; ++i) {
        PS_ROULETTE_EVENT_UPDATE_SERVER item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_ROULETTE_REWARD_INFO& value) {
    std::int16_t shCount = static_cast<std::int16_t>(value.vecInfo.size());
    packet.XParse << shCount;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// ============================================================================
// DBAgent: 手势(Gesture)相关结构
// ============================================================================

/**
 * 来自 IDA 0x14004A9F0: PS_GESTURE_SLOT - 手势槽位数据
 * 包含6个手势ID，每个槽位一个
 */
struct PS_GESTURE_SLOT {
    std::int32_t nGestureID[6] = {};  // 6个手势槽位
};

static_assert(sizeof(PS_GESTURE_SLOT) == 24, "PS_GESTURE_SLOT size must be 24 bytes");

// PS_GESTURE_SLOT 序列化运算符
inline XPacket& operator>>(XPacket& packet, PS_GESTURE_SLOT& value) {
    for (int i = 0; i < 6; ++i) {
        packet.XParse >> value.nGestureID[i];
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_GESTURE_SLOT& value) {
    for (int i = 0; i < 6; ++i) {
        packet.XParse << value.nGestureID[i];
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_GESTURE_SLOT& value) {
    for (int i = 0; i < 6; ++i) {
        packet.XParse << value.nGestureID[i];
    }
    return packet;
}

// ============================================================================
// DBAgent: 技能(Skill)相关结构
// ============================================================================

/**
 * 来自 IDA: ST_SKILL_INFO - 技能信息 (8 bytes)
 */
struct ST_SKILL_INFO {
    std::int32_t nID = 0;              // 技能ID
    std::int32_t nDivergenceID = 0;    // 分歧ID
};

static_assert(sizeof(ST_SKILL_INFO) == 8, "ST_SKILL_INFO size must be 8 bytes");

/**
 * 来自 IDA: PS_SKILL_DECK - 技能卡组 (20 bytes)
 * 对齐 IDA 反编译: wPos + 2 bytes padding + union { nSkill_1/2/3/4 或 uniSkill[4] }
 */
struct PS_SKILL_DECK {
    std::uint16_t wPos = 0;            // 槽位位置
    std::uint16_t _pad0 = 0;           // padding
    union {
        struct {
            std::int32_t nSkill_1 = 0;     // 技能1
            std::int32_t nSkill_2 = 0;     // 技能2
            std::int32_t nSkill_3 = 0;     // 技能3
            std::int32_t nSkill_4 = 0;     // 技能4
        };
        std::int32_t nSkill[4];             // 数组访问方式
    };
};

static_assert(sizeof(PS_SKILL_DECK) == 20, "PS_SKILL_DECK size must be 20 bytes");

// PS_SKILL_DECK 反序列化操作符
inline void operator>>(XPacket& packet, PS_SKILL_DECK& value) {
    packet.XParse >> value.wPos;
    packet.XParse >> value.nSkill_1;
    packet.XParse >> value.nSkill_2;
    packet.XParse >> value.nSkill_3;
    packet.XParse >> value.nSkill_4;
}

// PS_SKILL_DECK 序列化操作符
inline XPacket& operator<<(XPacket& packet, const PS_SKILL_DECK& value) {
    packet.XParse << value.wPos;
    packet.XParse << value.nSkill_1;
    packet.XParse << value.nSkill_2;
    packet.XParse << value.nSkill_3;
    packet.XParse << value.nSkill_4;
    return packet;
}

/**
 * 来自 IDA: PS_SKILL_DECK_PAGE - 技能卡组页
 * 对齐 IDA 反编译: byDeckPage + padding + wDeckBonus[4] + szDeckName
 */
struct PS_SKILL_DECK_PAGE {
    std::uint8_t byDeckPage = 0;       // 页码
    std::uint8_t _pad0[3] = {};        // padding
    std::uint16_t wDeckBonus[4] = {};  // 卡组加成 (8 bytes)
    wchar_t szDeckName[21] = {};       // 卡组名称 (42 bytes)
};

static_assert(sizeof(PS_SKILL_DECK_PAGE) == 54, "PS_SKILL_DECK_PAGE size must be 54 bytes");

// PS_SKILL_DECK_PAGE 反序列化操作符
inline void operator>>(XPacket& packet, PS_SKILL_DECK_PAGE& value) {
    packet.XParse >> value.byDeckPage;
    for (int i = 0; i < 4; ++i) {
        packet.XParse >> value.wDeckBonus[i];
    }
    // 读取字符串 (21个wchar_t) - 使用XParse.GetWString
    short outLen = 0;
    packet.XParse.GetWString(value.szDeckName, 21, outLen);
}

// PS_SKILL_DECK_PAGE 序列化操作符
inline XPacket& operator<<(XPacket& packet, const PS_SKILL_DECK_PAGE& value) {
    packet.XParse << value.byDeckPage;
    for (int i = 0; i < 4; ++i) {
        packet.XParse << value.wDeckBonus[i];
    }
    packet.XParse << GreenDamTan_BoundedWideString(value.szDeckName);
    return packet;
}

/**
 * 来自 IDA: PS_SKILL_PAGE - 技能页列表
 * 注意: std::vector 大小在不同平台可能不同，这里移除static_assert
 */
struct PS_SKILL_PAGE {
    std::uint8_t byActivePage = 0;     // 激活页码
    std::uint8_t _pad0[7] = {};        // padding
    std::vector<PS_SKILL_DECK_PAGE> vecInfo;  // 卡组页列表
};

// PS_SKILL_PAGE 反序列化操作符
inline void operator>>(XPacket& packet, PS_SKILL_PAGE& value) {
    packet.XParse >> value.byActivePage;
    std::uint8_t nCount = 0;
    packet.XParse >> nCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(nCount);
    for (std::uint8_t i = 0; i < nCount; ++i) {
        PS_SKILL_DECK_PAGE page{};
        packet >> page;
        value.vecInfo.push_back(page);
    }
}

// PS_SKILL_PAGE 序列化操作符
inline XPacket& operator<<(XPacket& packet, const PS_SKILL_PAGE& value) {
    packet.XParse << value.byActivePage;
    packet.XParse << static_cast<std::int8_t>(value.vecInfo.size());
    for (const auto& page : value.vecInfo) {
        packet << page;
    }
    return packet;
}

/**
 * 来自 IDA: PS_SKILL_LOAD - 技能加载数据
 */
struct PS_SKILL_LOAD {
    UXActorID uxActorID{};
    std::uint16_t wTotalSkillPoint = 0;      // 总技能点
    std::uint16_t wSkillPoint = 0;           // 剩余技能点
    std::uint16_t wDeckSlotCount = 0;        // 卡组槽位数量
    std::uint8_t _pad0[2] = {};              // padding
    std::vector<ST_SKILL_INFO> vecInfo;      // 技能列表
    std::vector<PS_SKILL_DECK> stSkillDeck;  // 技能卡组
    PS_SKILL_PAGE psSkillPage{};             // 技能页
};

// PS_SKILL_LOAD 反序列化操作符
inline void operator>>(XPacket& packet, PS_SKILL_LOAD& value) {
    packet.XParse >> value.uxActorID.dwActorID;
    packet.XParse >> value.wTotalSkillPoint;
    packet.XParse >> value.wSkillPoint;
    packet.XParse >> value.wDeckSlotCount;
    // vecInfo
    std::int16_t nSkillCount = 0;
    packet.XParse >> nSkillCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(nSkillCount);
    for (std::int16_t i = 0; i < nSkillCount; ++i) {
        ST_SKILL_INFO info{};
        packet.XParse >> info.nID;
        packet.XParse >> info.nDivergenceID;
        value.vecInfo.push_back(info);
    }
    // stSkillDeck
    std::int16_t nDeckCount = 0;
    packet.XParse >> nDeckCount;
    value.stSkillDeck.clear();
    value.stSkillDeck.reserve(nDeckCount);
    for (std::int16_t i = 0; i < nDeckCount; ++i) {
        PS_SKILL_DECK deck{};
        packet >> deck;
        value.stSkillDeck.push_back(deck);
    }
    // psSkillPage
    packet >> value.psSkillPage;
}

// PS_SKILL_LOAD 序列化操作符
inline XPacket& operator<<(XPacket& packet, const PS_SKILL_LOAD& value) {
    packet.XParse << value.uxActorID.dwActorID;
    packet.XParse << value.wTotalSkillPoint;
    packet.XParse << value.wSkillPoint;
    packet.XParse << value.wDeckSlotCount;
    // vecInfo
    packet.XParse << static_cast<std::int16_t>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet.XParse << info.nID;
        packet.XParse << info.nDivergenceID;
    }
    // stSkillDeck
    packet.XParse << static_cast<std::int16_t>(value.stSkillDeck.size());
    for (const auto& deck : value.stSkillDeck) {
        packet << deck;
    }
    // psSkillPage
    packet << value.psSkillPage;
    return packet;
}

// PS_CLASS_SCENE 序列化操作符
inline XPacket& operator<<(XPacket& packet, const PS_CLASS_SCENE& value) {
    for (int i = 0; i < 6; ++i) {
        packet.XParse << value.byClassScene[i];
    }
    return packet;
}

/**
 * 来自 IDA: PS_DB_SKILL_LEARN - 技能学习请求数据
 */
struct PS_DB_SKILL_LEARN {
    UXActorID uxActorID{};
    std::int32_t nOldSkillID = 0;
    std::int32_t nNewSkillID = 0;
    std::uint8_t byResult = 0;
    std::uint8_t _pad0[3] = {};
    std::int32_t nDivergenceID = 0;
    std::int32_t nUseSkillPoint = 0;
};

/**
 * 来自 IDA: PS_DB_SKILL_UPDATE_POINT - 技能点更新数据
 */
struct PS_DB_SKILL_UPDATE_POINT {
    UXActorID uxActorID{};
    std::uint16_t wTotalSkillPoint = 0;
    std::uint16_t wSkillPoint = 0;
};

/**
 * 来自 IDA: PS_SKILL_POINT - 技能点数据 (发送给客户端)
 * 用于 main=6, sub=0x73 包
 */
struct PS_SKILL_POINT {
    std::uint16_t wTotalSkillPoint = 0;
    std::uint16_t wSkillPoint = 0;
};

// PS_SKILL_POINT 序列化操作符
inline XPacket& operator<<(XPacket& packet, const PS_SKILL_POINT& value) {
    packet.XParse << value.wTotalSkillPoint;
    packet.XParse << value.wSkillPoint;
    return packet;
}

// PS_DB_SKILL_LEARN 序列化操作符
inline void operator>>(XPacket& packet, PS_DB_SKILL_LEARN& value) {
    packet.XParse >> value.uxActorID.dwActorID;
    packet.XParse >> value.nOldSkillID;
    packet.XParse >> value.nNewSkillID;
    packet.XParse >> value.nDivergenceID;
    packet.XParse >> value.nUseSkillPoint;
}

// PS_DB_SKILL_LEARN XSendDBPacket 序列化操作符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_SKILL_LEARN& value) {
    packet.XParse << value.uxActorID.dwActorID;
    packet.XParse << value.nOldSkillID;
    packet.XParse << value.nNewSkillID;
    packet.XParse << value.byResult;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.nDivergenceID;
    packet.XParse << value.nUseSkillPoint;
    return packet;
}

// PS_DB_SKILL_UPDATE_POINT 序列化操作符
inline void operator>>(XPacket& packet, PS_DB_SKILL_UPDATE_POINT& value) {
    packet.XParse >> value.uxActorID.dwActorID;
    packet.XParse >> value.wTotalSkillPoint;
    packet.XParse >> value.wSkillPoint;
}

// PS_DB_SKILL_UPDATE_POINT XSendDBPacket 序列化操作符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_SKILL_UPDATE_POINT& value) {
    packet.XParse << value.uxActorID.dwActorID;
    packet.XParse << value.wTotalSkillPoint;
    packet.XParse << value.wSkillPoint;
    return packet;
}

/**
 * 来自 IDA 0x1400BDB30: PS_SKILL_CHANGE - 技能变更数据
 */
struct PS_SKILL_CHANGE {
    std::uint32_t dwBeforeSkillID = 0;
    std::uint32_t dwBeforeDivergence = 0;
    std::uint32_t dwNewSkillID = 0;
    std::uint32_t dwNewDivergence = 0;
    std::int32_t nReturnSkillPoint = 0;
};

inline void operator>>(XPacket& packet, PS_SKILL_CHANGE& value) {
    packet.XParse >> value.dwBeforeSkillID;
    packet.XParse >> value.dwBeforeDivergence;
    packet.XParse >> value.dwNewSkillID;
    packet.XParse >> value.dwNewDivergence;
    packet.XParse >> value.nReturnSkillPoint;
}

/**
 * 来自 IDA 0x1400BE570: PS_DECK_BONUS - 卡组加成数据 (10 bytes)
 */
struct PS_DECK_BONUS {
    std::uint16_t wDeckBonus[4] = {};  // 卡组加成数组
    std::uint8_t byDeckPage = 0;       // 页码
    std::uint8_t _pad0 = 0;            // padding
};

inline void operator>>(XPacket& packet, PS_DECK_BONUS& value) {
    for (int i = 0; i < 4; ++i) {
        packet.XParse >> value.wDeckBonus[i];
    }
    packet.XParse >> value.byDeckPage;
}

inline XPacket& operator<<(XPacket& packet, const PS_DECK_BONUS& value) {
    for (int i = 0; i < 4; ++i) {
        packet.XParse << value.wDeckBonus[i];
    }
    packet.XParse << value.byDeckPage;
    return packet;
}

inline XSendPacket& operator<<(XSendPacket& packet, const PS_DECK_BONUS& value) {
    for (int i = 0; i < 4; ++i) {
        packet.XParse << value.wDeckBonus[i];
    }
    packet.XParse << value.byDeckPage;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DECK_BONUS& value) {
    for (int i = 0; i < 4; ++i) {
        packet.XParse << value.wDeckBonus[i];
    }
    packet.XParse << value.byDeckPage;
    return packet;
}

/**
 * 来自 IDA 0x1400BE570: PS_UPDATE_DECK_BONUS_VEC - 卡组加成批量更新数据
 */
struct PS_UPDATE_DECK_BONUS_VEC {
    std::vector<PS_DECK_BONUS> vecInfo;
};

inline void operator>>(XPacket& packet, PS_UPDATE_DECK_BONUS_VEC& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(nCount);
    for (std::int16_t i = 0; i < nCount; ++i) {
        PS_DECK_BONUS info{};
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_UPDATE_DECK_BONUS_VEC& value) {
    std::int16_t nCount = static_cast<std::int16_t>(value.vecInfo.size());
    packet.XParse << nCount;
    for (std::int16_t i = 0; i < nCount; ++i) {
        packet << value.vecInfo[i];
    }
    return packet;
}

inline XSendPacket& operator<<(XSendPacket& packet, const PS_UPDATE_DECK_BONUS_VEC& value) {
    std::int16_t nCount = static_cast<std::int16_t>(value.vecInfo.size());
    packet.XParse << nCount;
    for (std::int16_t i = 0; i < nCount; ++i) {
        packet << value.vecInfo[i];
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_UPDATE_DECK_BONUS_VEC& value) {
    std::int16_t nCount = static_cast<std::int16_t>(value.vecInfo.size());
    packet.XParse << nCount;
    for (std::int16_t i = 0; i < nCount; ++i) {
        packet << value.vecInfo[i];
    }
    return packet;
}

/**
 * 来自 IDA 0x1400BF330: PS_DECK_ACTIVE - 卡组激活数据
 */
struct PS_DECK_ACTIVE {
    std::uint8_t byType = 0;         // 类型: 0=技能卡组, 1=阿卡什卡组
    std::uint8_t byActivePage = 0;   // 激活页码
};

inline void operator>>(XPacket& packet, PS_DECK_ACTIVE& value) {
    packet.XParse >> value.byType;
    packet.XParse >> value.byActivePage;
}

/**
 * 来自 IDA 0x1400BE2E0: PS_SKILL_DECK_VEC - 技能卡组批量数据
 */
struct PS_SKILL_DECK_VEC {
    std::vector<PS_SKILL_DECK> vecInfo;
};

inline void operator>>(XPacket& packet, PS_SKILL_DECK_VEC& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(nCount);
    for (std::int16_t i = 0; i < nCount; ++i) {
        PS_SKILL_DECK deck{};
        packet >> deck;
        value.vecInfo.push_back(deck);
    }
}

// PS_SKILL_DECK_VEC 序列化操作符
inline XPacket& operator<<(XPacket& packet, const PS_SKILL_DECK_VEC& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecInfo.size());
    for (const auto& deck : value.vecInfo) {
        packet << deck;
    }
    return packet;
}

// PS_SKILL_DECK_VEC 数据库包序列化操作符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_SKILL_DECK_VEC& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecInfo.size());
    for (const auto& deck : value.vecInfo) {
        packet << deck;
    }
    return packet;
}

/**
 * 来自 IDA 0x1400BF490: PS_DECK_NAME - 卡组名称数据
 */
struct PS_DECK_NAME {
    std::uint8_t byDeckPage = 0;
    std::uint8_t _pad0 = 0;
    wchar_t szDeckName[13] = {};  // 卡组名称
};

inline void operator>>(XPacket& packet, PS_DECK_NAME& value) {
    packet.XParse >> value.byDeckPage;
    short outLen = 0;
    packet.XParse.GetWString(value.szDeckName, 13, outLen);
}

/**
 * 来自 IDA 0x1400BF490: PS_DECK_NAME_VEC - 卡组名称批量数据
 */
struct PS_DECK_NAME_VEC {
    std::uint8_t byType = 0;  // 类型: 0=技能卡组, 1=阿卡什卡组
    std::vector<PS_DECK_NAME> vecInfo;
};

inline void operator>>(XPacket& packet, PS_DECK_NAME_VEC& value) {
    packet.XParse >> value.byType;
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(nCount);
    for (std::int16_t i = 0; i < nCount; ++i) {
        PS_DECK_NAME name{};
        packet >> name;
        value.vecInfo.push_back(name);
    }
}

/**
 * 来自 IDA 0x1400BECE0: ST_BOOSTER_INFO - 增益道具信息
 * IDA verified from CGocBooster member access patterns:
 * - wBoosterID: offset 0
 * - bAccount: offset 2 (account-level flag)
 * - byTimeType: offset 3 (time decrease condition)
 * - bConsumeTime: offset 4 (active consumption flag)
 * - lRemainTime: offset 8 (remaining time in ms or timestamp)
 */
struct ST_BOOSTER_INFO {
    std::uint16_t wBoosterID = 0;
    std::uint8_t bAccount = 0;       // 是否账号级
    std::uint8_t byTimeType = 0;     // Time decrease condition (from TB_BOOSTER::Decrease_Condition)
    std::uint8_t bConsumeTime = 0;   // Active time consumption flag
    std::uint8_t _pad0[3] = {};      // Padding for alignment
    std::int64_t lRemainTime = 0;    // Remaining time (ms or absolute timestamp)
};

inline XPacket& operator<<(XPacket& packet, const ST_BOOSTER_INFO& value) {
    packet.XParse << value.wBoosterID;
    packet.XParse << value.bAccount;
    packet.XParse << value.lRemainTime;
    return packet;
}

/**
 * 来自 IDA 0x1400BECE0: PS_BOOSTER_LIST_RES - 增益道具列表响应
 */
struct PS_BOOSTER_LIST_RES {
    std::vector<ST_BOOSTER_INFO> vecInfo;
};

inline XPacket& operator<<(XPacket& packet, const PS_BOOSTER_LIST_RES& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

/**
 * 来自 IDA 0x1400BF7E0: PS_DB_SKILL_DECK_OPEN - 技能卡组页解锁请求数据
 */
struct PS_DB_SKILL_DECK_OPEN {
    std::uint32_t dwUCID = 0;
    PS_RES_STORAGE_INFO psUpdateItemList{};
    struct {
        std::int32_t nResult = 0;
        std::uint8_t byDeckPage = 0;
        std::uint8_t _pad0[3] = {};
        std::uint16_t wDeckBonus[4] = {};
    } psResOpen{};
};

inline void operator>>(XPacket& packet, PS_DB_SKILL_DECK_OPEN& value) {
    packet.XParse >> value.dwUCID;
    packet >> value.psUpdateItemList;
    packet.XParse >> value.psResOpen.nResult;
    packet.XParse >> value.psResOpen.byDeckPage;
    for (int i = 0; i < 4; ++i) {
        packet.XParse >> value.psResOpen.wDeckBonus[i];
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_SKILL_DECK_OPEN& value) {
    packet.XParse << value.dwUCID;
    packet << value.psUpdateItemList;
    packet.XParse << value.psResOpen.nResult;
    packet.XParse << value.psResOpen.byDeckPage;
    for (int i = 0; i < 4; ++i) {
        packet.XParse << value.psResOpen.wDeckBonus[i];
    }
    return packet;
}

// ============================================================================
// Quest 相关结构定义 - 用于 XSQLQuestProcess
// ============================================================================

/**
 * 来自 IDA: ST_QUEST_EPISODE_CONDITION - 任务章节条件
 */
struct ST_QUEST_EPISODE_CONDITION {
    std::int32_t nCondition = 0;
    std::uint8_t byValue = 0;
    std::uint8_t _pad0[3] = {};
};

/**
 * 来自 IDA: ST_QUEST_EPISODE - 任务章节信息 (0x60 bytes)
 */
struct ST_QUEST_EPISODE {
    std::uint8_t byAddHelper = 0;
    std::uint8_t _pad0[1] = {};
    std::int16_t shCompleteBit = 0;
    bool bFailed = false;
    std::uint8_t _pad1[3] = {};
    ST_QUEST_EPISODE_CONDITION stCondition[10] = {};
};

/**
 * 来自 IDA: PS_QUEST_EPISODE - 单个任务章节数据包
 */
struct PS_QUEST_EPISODE {
    std::uint32_t dwEpisodeID = 0;
    ST_QUEST_EPISODE stEpisode{};
};

/**
 * 来自 IDA: ST_QUEST_REPEAT_INFO - 可重复任务信息 (0x18 bytes)
 */
struct ST_QUEST_REPEAT_INFO {
    std::uint32_t dwQuestID = 0;
    std::uint8_t byCount = 0;
    std::uint8_t _pad0[7] = {};
    std::int64_t biLastInitDate = 0;
};

/**
 * 来自 IDA: PS_QUEST_EPISODE_MAP - 任务章节映射
 * 使用 std::map<EpisodeID, ST_QUEST_EPISODE>
 */
struct PS_QUEST_EPISODE_MAP {
    std::map<std::uint32_t, ST_QUEST_EPISODE> mapInfo;
};

/**
 * 来自 IDA: PS_QUEST_COMPLETE_EPISODE - 完成的章节信息
 */
struct PS_QUEST_COMPLETE_EPISODE {
    std::int32_t nEpisodeID = 0;
    std::int32_t nCompleteCount = 0;
};

/**
 * 来自 IDA: PS_REPEAT_QUEST_MAP - 可重复任务映射
 * 使用 std::map<QuestID, ST_QUEST_REPEAT_INFO>
 */
struct PS_REPEAT_QUEST_MAP {
    std::map<std::uint32_t, ST_QUEST_REPEAT_INFO> mapInfo;
};

/**
 * 来自 IDA: PS_REPEAT_QUEST_INFO - 可重复任务添加请求数据
 */
struct PS_REPEAT_QUEST_INFO {
    std::uint32_t dwUCID = 0;
    ST_QUEST_REPEAT_INFO stInfo{};
};

/**
 * 来自 IDA: PS_REPEAT_QUEST_UPDATE - 可重复任务批量更新数据
 */
struct PS_REPEAT_QUEST_UPDATE {
    std::uint32_t dwUCID = 0;
    std::vector<ST_QUEST_REPEAT_INFO> vecUpdate;
};

/**
 * 来自 IDA: PS_QUEST_FIRST_DROP_ITEM - 首次掉落物品信息
 */
struct PS_QUEST_FIRST_DROP_ITEM {
    std::int32_t nQuestID = 0;
    std::int32_t nItemID = 0;
};

/**
 * 来自 IDA: ST_QUEST_FIRST_DROP_ITEM - 首次掉落物品更新请求数据
 */
struct ST_QUEST_FIRST_DROP_ITEM {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwQuestID = 0;
    std::uint32_t dwItemID = 0;
    std::int32_t nCount = 0;
};

/**
 * 来自 IDA: PS_QUEST_COMPLETE_ADD_LIST - 任务完成附加列表
 */
struct PS_QUEST_COMPLETE_ADD_LIST {
    std::vector<std::int32_t> vecQuestID;  // 任务ID列表
};

// ============================================================================
// Quest 相关结构序列化操作符
// ============================================================================

// ST_QUEST_EPISODE_CONDITION 序列化
inline XPacket& operator<<(XPacket& packet, const ST_QUEST_EPISODE_CONDITION& value) {
    packet.XParse << value.nCondition;
    packet.XParse << value.byValue;
    return packet;
}

// ST_QUEST_EPISODE 序列化
inline XPacket& operator<<(XPacket& packet, const ST_QUEST_EPISODE& value) {
    packet.XParse << value.byAddHelper;
    packet.XParse << value.shCompleteBit;
    packet.XParse << static_cast<std::uint8_t>(value.bFailed ? 1 : 0);
    for (int i = 0; i < 10; ++i) {
        packet << value.stCondition[i];
    }
    return packet;
}

// ST_QUEST_EPISODE 反序列化
inline void operator>>(XPacket& packet, ST_QUEST_EPISODE& value) {
    packet.XParse >> value.byAddHelper;
    packet.XParse >> value.shCompleteBit;
    std::uint8_t byFailed = 0;
    packet.XParse >> byFailed;
    value.bFailed = (byFailed != 0);
    for (int i = 0; i < 10; ++i) {
        packet.XParse >> value.stCondition[i].nCondition;
        packet.XParse >> value.stCondition[i].byValue;
    }
}

// PS_QUEST_EPISODE 反序列化
inline void operator>>(XPacket& packet, PS_QUEST_EPISODE& value) {
    packet.XParse >> value.dwEpisodeID;
    packet >> value.stEpisode;
}

// PS_QUEST_EPISODE_MAP 反序列化
inline void operator>>(XPacket& packet, PS_QUEST_EPISODE_MAP& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.mapInfo.clear();
    for (std::int16_t i = 0; i < nCount; ++i) {
        std::uint32_t dwEpisodeID = 0;
        ST_QUEST_EPISODE stEpisode{};
        packet.XParse >> dwEpisodeID;
        packet >> stEpisode;
        value.mapInfo[dwEpisodeID] = stEpisode;
    }
}

// PS_QUEST_EPISODE_MAP 序列化
inline XPacket& operator<<(XPacket& packet, const PS_QUEST_EPISODE_MAP& value) {
    packet.XParse << static_cast<std::int16_t>(value.mapInfo.size());
    for (const auto& pair : value.mapInfo) {
        packet.XParse << pair.first;  // EpisodeID
        packet << pair.second;        // ST_QUEST_EPISODE
    }
    return packet;
}

// PS_QUEST_COMPLETE_EPISODE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_QUEST_COMPLETE_EPISODE& value) {
    packet.XParse << value.nEpisodeID;
    packet.XParse << value.nCompleteCount;
    return packet;
}

// ST_QUEST_REPEAT_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_QUEST_REPEAT_INFO& value) {
    packet.XParse << value.dwQuestID;
    packet.XParse << value.byCount;
    packet.XParse << value.biLastInitDate;
    return packet;
}

// PS_REPEAT_QUEST_MAP 序列化
inline XPacket& operator<<(XPacket& packet, const PS_REPEAT_QUEST_MAP& value) {
    packet.XParse << static_cast<std::int16_t>(value.mapInfo.size());
    for (const auto& pair : value.mapInfo) {
        packet << pair.second;  // ST_QUEST_REPEAT_INFO (contains QuestID)
    }
    return packet;
}

// PS_REPEAT_QUEST_INFO 反序列化
inline void operator>>(XPacket& packet, PS_REPEAT_QUEST_INFO& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.stInfo.dwQuestID;
    packet.XParse >> value.stInfo.byCount;
    packet.XParse >> value.stInfo.biLastInitDate;
}

// PS_REPEAT_QUEST_UPDATE 反序列化
inline void operator>>(XPacket& packet, PS_REPEAT_QUEST_UPDATE& value) {
    packet.XParse >> value.dwUCID;
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecUpdate.clear();
    value.vecUpdate.reserve(nCount);
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_QUEST_REPEAT_INFO info{};
        packet.XParse >> info.dwQuestID;
        packet.XParse >> info.byCount;
        packet.XParse >> info.biLastInitDate;
        value.vecUpdate.push_back(info);
    }
}

// PS_QUEST_FIRST_DROP_ITEM 序列化
inline XPacket& operator<<(XPacket& packet, const PS_QUEST_FIRST_DROP_ITEM& value) {
    packet.XParse << value.nQuestID;
    packet.XParse << value.nItemID;
    return packet;
}

// ST_QUEST_FIRST_DROP_ITEM 反序列化
inline void operator>>(XPacket& packet, ST_QUEST_FIRST_DROP_ITEM& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwQuestID;
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.nCount;
}

// PS_QUEST_COMPLETE_ADD_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_QUEST_COMPLETE_ADD_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecQuestID.size());
    for (const auto& questID : value.vecQuestID) {
        packet.XParse << questID;
    }
    return packet;
}

// 注意: PS_DEFAULT_INVEN_ITEM 和 PS_DEFAULT_INVEN_ITEMS 已在 PSCommon.h 中定义

// ============================================================================
// DBAgent: 排名(Ranking)相关结构
// ============================================================================

// 对齐 IDA: 排名信息 (20 bytes) - 与 DBLoadTable.h 中的 ST_RANKING_INFO 相同
// 用于 DBAgent 协议，避免循环依赖
struct PS_DB_RANKING_INFO {
    std::uint16_t wRankInfoIndex = 0;    // offset 0: 排名索引
    std::uint8_t _pad0[2] = {};          // padding
    std::int32_t nTopRank = 0;           // offset 4: 顶部排名
    std::uint8_t byType = 0;             // offset 8: 类型 (1=Total, 2=Time, 3=ClearCount, 5=MonsterKillScore)
    std::uint8_t byClassType = 0;        // offset 9: 职业类型
    std::uint8_t byRewradType = 0;       // offset 10: 奖励类型
    std::uint8_t _pad1 = {};             // padding
    std::uint32_t dwMazeID = 0;          // offset 12: 迷宫ID
    std::uint8_t byClass = 0;            // offset 16: 职业
    std::uint8_t _pad2[3] = {};          // padding
};

static_assert(sizeof(PS_DB_RANKING_INFO) == 20, "PS_DB_RANKING_INFO size must match IDA");

// PS_DB_RANKING_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_RANKING_INFO& value) {
    packet.XParse << value.wRankInfoIndex;
    packet.XParse << value.nTopRank;
    packet.XParse << value.byType;
    packet.XParse << value.byClassType;
    packet.XParse << value.byRewradType;
    packet.XParse << value.dwMazeID;
    packet.XParse << value.byClass;
    return packet;
}

// PS_DB_RANKING_INFO 反序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_RANKING_INFO& value) {
    packet.XParse >> value.wRankInfoIndex;
    packet.XParse >> value.nTopRank;
    packet.XParse >> value.byType;
    packet.XParse >> value.byClassType;
    packet.XParse >> value.byRewradType;
    packet.XParse >> value.dwMazeID;
    packet.XParse >> value.byClass;
    return packet;
}

// 对齐 IDA: 用户排名信息 (108 bytes)
struct ST_USER_RANKING_INFO {
    std::uint32_t dwUAID = 0;            // offset 0: 账号ID
    std::uint32_t dwUCID = 0;            // offset 4: 角色ID
    std::uint16_t wRankInfoIndex = 0;    // offset 8: 排名索引
    std::uint8_t _pad0[2] = {};          // offset 10: padding (for nRank alignment)
    std::int32_t nRank = 0;              // offset 12: 排名
    std::int32_t nMaxRank = 0;           // offset 16: 最大排名
    std::uint8_t byClass = 0;            // offset 20: 职业
    std::uint8_t byLevel = 0;            // offset 21: 等级
    std::uint8_t _pad1[2] = {};          // offset 22: padding (for nPlayCount alignment)
    std::int32_t nPlayCount = 0;         // offset 24: 游玩次数
    std::int32_t nScore = 0;             // offset 28: 分数
    std::uint8_t byLastReward = 0;       // offset 32: 上次奖励
    std::uint8_t _pad2[3] = {};          // offset 33: padding (for dwProfilePhotoID alignment)
    std::uint32_t dwProfilePhotoID = 0;  // offset 36: 头像ID
    wchar_t strName[21] = {};            // offset 40: 角色名 (42 bytes)
    wchar_t strLeagueName[10] = {};      // offset 82: 联盟名 (20 bytes)
    std::int32_t nMyBestClearTime = 0;   // offset 104: 我的最快通关时间
};

static_assert(sizeof(ST_USER_RANKING_INFO) == 108, "ST_USER_RANKING_INFO size must match IDA");

// 对齐 IDA: 排名列表请求基础结构 (12 bytes)
struct PS_RANKING_LIST_REQ {
    std::uint32_t dwUAID = 0;            // offset 0x00: 账号ID
    std::uint32_t dwUCID = 0;            // offset 0x04: 角色ID
    std::uint16_t wRankInfoIndex = 0;    // offset 0x08: 排名索引
    bool bLastRanking = false;           // offset 0x0A: 是否上周排名
    std::uint8_t byClass = 0;            // offset 0x0B: 职业
};

static_assert(sizeof(PS_RANKING_LIST_REQ) == 12, "PS_RANKING_LIST_REQ size must match IDA");

// 对齐 IDA: 排名列表请求 (32 bytes)
struct PS_DB_RANKING_LIST_REQ {
    std::uint64_t dw64SetCount = 0;          // offset 0x00: 设置计数
    PS_DB_RANKING_INFO stRankingInfo{};      // offset 0x08: 排名信息 (20 bytes)
    bool bLastRanking = false;               // offset 0x1C: 是否上周排名
    std::uint8_t byRankingCategory = 0;      // offset 0x1D: 排名类别 (1=个人, 3=组队)
    std::uint8_t _pad0[2] = {};              // padding
};

static_assert(sizeof(PS_DB_RANKING_LIST_REQ) == 32, "PS_DB_RANKING_LIST_REQ size must match IDA");

// 对齐 IDA: 排名列表响应 (72 bytes)
struct PS_DB_RANKING_LIST_RES {
    PS_DB_RANKING_LIST_REQ stReq{};                      // offset 0x00: 请求信息 (32 bytes)
    std::vector<ST_USER_RANKING_INFO> vecList;           // offset 0x20: 排名列表
    bool bLast = false;                                  // offset 0x40: 是否最后
    std::uint8_t _pad0[7] = {};                          // padding
};

static_assert(sizeof(PS_DB_RANKING_LIST_RES) == 72, "PS_DB_RANKING_LIST_RES size must match IDA");

// 对齐 IDA: 我的排名信息请求 (36 bytes)
struct PS_DB_MY_RANKING_INFO_REQ {
    PS_RANKING_LIST_REQ stMyReq{};            // offset 0x00: 我的请求信息 (12 bytes)
    PS_DB_RANKING_INFO stRankingInfo{};       // offset 0x0C: 排名信息 (20 bytes)
    std::uint8_t byRankingCategory = 0;       // offset 0x20: 排名类别
    std::uint8_t _pad0[3] = {};               // padding
};

static_assert(sizeof(PS_DB_MY_RANKING_INFO_REQ) == 36, "PS_DB_MY_RANKING_INFO_REQ size must match IDA");

// 对齐 IDA: 我的排名信息响应 (264 bytes)
struct PS_DB_MY_RANKING_INFO_RES {
    PS_RANKING_LIST_REQ stMyReq{};                         // offset 0x00: 我的请求信息 (12 bytes)
    PS_DB_RANKING_INFO stRankingInfo{};                    // offset 0x0C: 排名信息 (20 bytes)
    ST_USER_RANKING_INFO stMyInfo{};                       // offset 0x20: 我的排名信息 (108 bytes)
    ST_USER_RANKING_INFO stMySeasonInfo{};                 // offset 0x8C: 我的赛季排名信息 (108 bytes)
    std::uint64_t dw64SeasonSetCount = 0;                  // offset 0xF8: 赛季设置计数
    std::uint8_t byRankingCategory = 0;                    // offset 0x100: 排名类别
    std::uint8_t _pad0[7] = {};                            // padding
};

static_assert(sizeof(PS_DB_MY_RANKING_INFO_RES) == 264, "PS_DB_MY_RANKING_INFO_RES size must match IDA");

// 对齐 IDA: 排名积分更新 (132 bytes)
struct PS_DB_RANKING_POINT_UPDATE {
    ST_USER_RANKING_INFO stUser{};            // offset 0x00: 用户排名信息 (108 bytes)
    PS_DB_RANKING_INFO stRankingInfo{};       // offset 0x6C: 排名信息 (20 bytes)
    std::uint8_t byRankingCategory = 0;       // offset 0x80: 排名类别
    std::uint8_t _pad0[3] = {};               // padding
};

static_assert(sizeof(PS_DB_RANKING_POINT_UPDATE) == 132, "PS_DB_RANKING_POINT_UPDATE size must match IDA");

// ============================================================================
// Ranking 序列化运算符
// ============================================================================

// ST_USER_RANKING_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_USER_RANKING_INFO& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.wRankInfoIndex;
    packet.XParse << value.nRank;
    packet.XParse << value.nMaxRank;
    packet.XParse << value.byClass;
    packet.XParse << value.byLevel;
    packet.XParse << value.nPlayCount;
    packet.XParse << value.nScore;
    packet.XParse << value.byLastReward;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << std::wstring(value.strName);
    packet.XParse << std::wstring(value.strLeagueName);
    packet.XParse << value.nMyBestClearTime;
    return packet;
}

// ST_USER_RANKING_INFO 反序列化
inline XPacket& operator>>(XPacket& packet, ST_USER_RANKING_INFO& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.wRankInfoIndex;
    packet.XParse >> value.nRank;
    packet.XParse >> value.nMaxRank;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byLevel;
    packet.XParse >> value.nPlayCount;
    packet.XParse >> value.nScore;
    packet.XParse >> value.byLastReward;
    packet.XParse >> value.dwProfilePhotoID;
    std::wstring strName;
    packet.XParse >> strName;
    std::wcsncpy(value.strName, strName.c_str(), 20);
    value.strName[20] = L'\0';
    std::wstring strLeagueName;
    packet.XParse >> strLeagueName;
    std::wcsncpy(value.strLeagueName, strLeagueName.c_str(), 9);
    value.strLeagueName[9] = L'\0';
    packet.XParse >> value.nMyBestClearTime;
    return packet;
}

// PS_RANKING_LIST_REQ 序列化
inline XPacket& operator<<(XPacket& packet, const PS_RANKING_LIST_REQ& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.wRankInfoIndex;
    packet.XParse << value.bLastRanking;
    packet.XParse << value.byClass;
    return packet;
}

// PS_RANKING_LIST_REQ 反序列化
inline XPacket& operator>>(XPacket& packet, PS_RANKING_LIST_REQ& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.wRankInfoIndex;
    packet.XParse >> value.bLastRanking;
    packet.XParse >> value.byClass;
    return packet;
}

// PS_DB_RANKING_LIST_REQ 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_RANKING_LIST_REQ& value) {
    packet.XParse << value.dw64SetCount;
    packet << value.stRankingInfo;
    packet.XParse << value.bLastRanking;
    packet.XParse << value.byRankingCategory;
    return packet;
}

// PS_DB_RANKING_LIST_REQ 反序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_RANKING_LIST_REQ& value) {
    packet.XParse >> value.dw64SetCount;
    packet >> value.stRankingInfo;
    packet.XParse >> value.bLastRanking;
    packet.XParse >> value.byRankingCategory;
    return packet;
}

// PS_DB_RANKING_LIST_RES 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_RANKING_LIST_RES& value) {
    packet << value.stReq;
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& info : value.vecList) {
        packet << info;
    }
    packet.XParse << value.bLast;
    return packet;
}

// PS_DB_MY_RANKING_INFO_REQ 反序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_MY_RANKING_INFO_REQ& value) {
    packet >> value.stMyReq;
    packet >> value.stRankingInfo;
    packet.XParse >> value.byRankingCategory;
    return packet;
}

// PS_DB_MY_RANKING_INFO_RES 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_MY_RANKING_INFO_RES& value) {
    packet << value.stMyReq;
    packet << value.stRankingInfo;
    packet << value.stMyInfo;
    packet << value.stMySeasonInfo;
    packet.XParse << value.dw64SeasonSetCount;
    packet.XParse << value.byRankingCategory;
    return packet;
}

// PS_DB_RANKING_POINT_UPDATE 反序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_RANKING_POINT_UPDATE& value) {
    packet >> value.stUser;
    packet >> value.stRankingInfo;
    packet.XParse >> value.byRankingCategory;
    return packet;
}

/**
 * 来自 IDA 0x1400AD190: PS_DB_RANKING_REWARD - 排名奖励数据
 */
struct PS_DB_RANKING_REWARD {
    PS_RANKING_LIST_REQ stReq{};
    std::uint64_t dw64SeasonSetCount = 0;
    std::int32_t nError = 0;
    std::int32_t nRank = 0;
    std::uint8_t byRankingCategory = 0;
    std::uint8_t byFlag = 0;
    std::uint8_t _pad0[2] = {};
    std::vector<PS_STORAGE_INFO> stUpdateItemList;
    std::vector<PS_STORAGE_INFO> stCreateItemList;
};

/**
 * 来自 IDA 0x1400B5DB0: PS_MODE_MAZE_RANKING_POINT_UPDATE - 模式迷宫排名积分更新
 */
struct PS_MODE_MAZE_RANKING_POINT_UPDATE {
    std::uint32_t dwUCID = 0;
    wchar_t strName[21] = {};
    std::uint8_t byClass = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t _pad0[2] = {};
    std::int32_t nPoint = 0;
    std::int32_t nPlayCount = 0;
    std::uint32_t dwProfilePhotoID = 0;
    wchar_t strLeagueName[10] = {};
    std::int32_t nTotalPoint = 0;
};

/**
 * 来自 IDA 0x1400B6030: PS_DB_OPERATION_RANKING_LIST_RES - 操作排名列表响应
 */
struct PS_DB_OPERATION_RANKING_LIST_RES {
    struct {
        std::uint16_t wRankInfoIndex = 0;
        std::uint8_t bLastRanking = 0;
        std::uint8_t _pad0 = {};
        ST_USER_RANKING_INFO stMyInfo{};
        std::vector<ST_USER_RANKING_INFO> vecList;
        std::uint8_t bLast = 0;
        std::uint8_t _pad1[3] = {};
    } stRes;
    std::int32_t nMyLastRank = 0;
};

/**
 * PS_MODE_MAZE_RANKING_FOR_MATCHING - 匹配排名数据
 */
struct PS_MODE_MAZE_RANKING_FOR_MATCHING {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwModeMazeID = 0;
    std::int32_t nRank = 0;
};

// PS_DB_RANKING_REWARD 反序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_RANKING_REWARD& value) {
    packet >> value.stReq;
    packet.XParse >> value.dw64SeasonSetCount;
    packet.XParse >> value.nError;
    packet.XParse >> value.nRank;
    packet.XParse >> value.byRankingCategory;
    packet.XParse >> value.byFlag;
    std::int16_t nUpdateCount = 0;
    packet.XParse >> nUpdateCount;
    value.stUpdateItemList.resize(nUpdateCount);
    for (auto& info : value.stUpdateItemList) {
        packet >> info;
    }
    std::int16_t nCreateCount = 0;
    packet.XParse >> nCreateCount;
    value.stCreateItemList.resize(nCreateCount);
    for (auto& info : value.stCreateItemList) {
        packet >> info;
    }
    return packet;
}

// PS_DB_RANKING_REWARD 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_RANKING_REWARD& value) {
    packet << value.stReq;
    packet.XParse << value.dw64SeasonSetCount;
    packet.XParse << value.nError;
    packet.XParse << value.nRank;
    packet.XParse << value.byRankingCategory;
    packet.XParse << value.byFlag;
    packet.XParse << static_cast<std::int16_t>(value.stUpdateItemList.size());
    for (const auto& info : value.stUpdateItemList) {
        packet << info;
    }
    packet.XParse << static_cast<std::int16_t>(value.stCreateItemList.size());
    for (const auto& info : value.stCreateItemList) {
        packet << info;
    }
    return packet;
}

// PS_MODE_MAZE_RANKING_POINT_UPDATE 反序列化
inline XPacket& operator>>(XPacket& packet, PS_MODE_MAZE_RANKING_POINT_UPDATE& value) {
    packet.XParse >> value.dwUCID;
    std::wstring strName;
    packet.XParse >> strName;
    std::wcsncpy(value.strName, strName.c_str(), 20);
    value.strName[20] = L'\0';
    packet.XParse >> value.byClass;
    packet.XParse >> value.byLevel;
    packet.XParse >> value.nPoint;
    packet.XParse >> value.nPlayCount;
    packet.XParse >> value.dwProfilePhotoID;
    std::wstring strLeagueName;
    packet.XParse >> strLeagueName;
    std::wcsncpy(value.strLeagueName, strLeagueName.c_str(), 9);
    value.strLeagueName[9] = L'\0';
    packet.XParse >> value.nTotalPoint;
    return packet;
}

// PS_DB_OPERATION_RANKING_LIST_RES 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_OPERATION_RANKING_LIST_RES& value) {
    packet.XParse << value.stRes.wRankInfoIndex;
    packet.XParse << value.stRes.bLastRanking;
    packet << value.stRes.stMyInfo;
    packet.XParse << static_cast<std::int16_t>(value.stRes.vecList.size());
    for (const auto& info : value.stRes.vecList) {
        packet << info;
    }
    packet.XParse << value.stRes.bLast;
    packet.XParse << value.nMyLastRank;
    return packet;
}

// PS_MODE_MAZE_RANKING_FOR_MATCHING 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MODE_MAZE_RANKING_FOR_MATCHING& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwModeMazeID;
    packet.XParse << value.nRank;
    return packet;
}

// ============================================================================
// DBAgent: SoulMetry(灵魂熔炉)相关结构
// ============================================================================

/**
 * 来自 IDA 0x1400C1620: PS_SOULMETRY_INFO - 灵魂熔炉信息
 */
struct PS_SOULMETRY_INFO {
    std::int32_t dwSoulMetryID = 0;
    std::int16_t shValue = 0;
    std::uint8_t _pad0[2] = {};
};

/**
 * 来自 IDA 0x1400C1620: PS_SOULMETRY_LIST - 灵魂熔炉列表
 */
struct PS_SOULMETRY_LIST {
    std::vector<PS_SOULMETRY_INFO> vecInfo;
};

/**
 * 来自 IDA 0x1400C1620: PS_SOULMETRY_COMPLETE - 灵魂熔炉完成信息
 */
struct PS_SOULMETRY_COMPLETE {
    std::int32_t dwSoulMetryID = 0;
    std::int16_t shValue = 0;
    std::uint8_t _pad0[2] = {};
};

// PS_SOULMETRY_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SOULMETRY_INFO& value) {
    packet.XParse << value.dwSoulMetryID;
    packet.XParse << value.shValue;
    return packet;
}

// PS_SOULMETRY_INFO 反序列化
inline XPacket& operator>>(XPacket& packet, PS_SOULMETRY_INFO& value) {
    packet.XParse >> value.dwSoulMetryID;
    packet.XParse >> value.shValue;
    return packet;
}

// PS_SOULMETRY_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SOULMETRY_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

// PS_SOULMETRY_LIST 反序列化
inline XPacket& operator>>(XPacket& packet, PS_SOULMETRY_LIST& value) {
    std::int16_t nSize = 0;
    packet.XParse >> nSize;
    value.vecInfo.resize(nSize);
    for (auto& info : value.vecInfo) {
        packet >> info;
    }
    return packet;
}

// PS_SOULMETRY_COMPLETE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SOULMETRY_COMPLETE& value) {
    packet.XParse << value.dwSoulMetryID;
    packet.XParse << value.shValue;
    return packet;
}

// PS_SOULMETRY_COMPLETE 反序列化
inline XPacket& operator>>(XPacket& packet, PS_SOULMETRY_COMPLETE& value) {
    packet.XParse >> value.dwSoulMetryID;
    packet.XParse >> value.shValue;
    return packet;
}

// PS_CUTSCENE_UPDATE_RES 序列化
// IDA: used by CCutsceneManager::SkipCutscene (0x1401B2280)
inline XSendPacket& operator<<(XSendPacket& packet, const PS_CUTSCENE_UPDATE_RES& value) {
    packet.XParse << value.nPlayState;
    return packet;
}

// ============================================================================
// Daily Mission District Map Serialization
// ============================================================================

// PS_MAP_DISTRICT_DAILY_MISSION 序列化
// IDA: used by CGocDailyMission::SendDailyMissionList (0x140055780)
inline XSendPacket& operator<<(XSendPacket& packet, const PS_MAP_DISTRICT_DAILY_MISSION& value) {
    packet.XParse << static_cast<std::int16_t>(value.mapDistrictMissions.size());
    for (const auto& pair : value.mapDistrictMissions) {
        packet.XParse << pair.first; // District_ID
        packet.XParse << static_cast<std::int16_t>(pair.second.size());
        for (const auto& mission : pair.second) {
            packet << mission;
        }
    }
    return packet;
}

// PS_DAILY_MISSION_UPDATE 序列化
inline XSendPacket& operator<<(XSendPacket& packet, const PS_DAILY_MISSION_UPDATE& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}
