// GreenDamTan_MyRoomStructs.h
// MyRoom 相关结构体定义 (共享层)
// 对齐 IDA ControlServer.exe 类型定义

#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <cstdint>

// XVec3 已在 PSCommon.h 中定义，直接使用

// 对齐 IDA: ST_MYROOM_USER (28 bytes, 5 members)
// 用户进入 MyRoom 时携带的信息
struct ST_MYROOM_USER {
    int nServerID = 0;              // +0x00, 4 bytes
    std::uint32_t dwUAID = 0;       // +0x04, 4 bytes
    std::uint32_t dwUCID = 0;       // +0x08, 4 bytes
    std::uint32_t dwBeforeMapID = 0; // +0x0C, 4 bytes
    XVec3 xBeforePos{};             // +0x10, 12 bytes
};
static_assert(sizeof(ST_MYROOM_USER) == 28, "ST_MYROOM_USER size mismatch with IDA");

// 对齐 IDA: ST_MYROOM_OWNER_INFO (68 bytes, 10 members)
// MyRoom 所有者信息
struct ST_MYROOM_OWNER_INFO {
    std::uint32_t dwOwnerUAID = 0;      // +0x00, 4 bytes
    short shMapIndex = 0;               // +0x04, 2 bytes
    short shGridNo = 0;                 // +0x06, 2 bytes
    std::uint32_t dwMapID = 0;          // +0x08, 4 bytes
    std::uint8_t byRoomOpenLevel = 0;   // +0x0C, 1 byte
    // +0x0D padding (1 byte)
    wchar_t szRoomName[20] = {};        // +0x0E, 40 bytes (offset 14 to 54)
    int nRecommendCount = 0;            // +0x38 (56), 4 bytes
    int nFavoriteCount = 0;             // +0x3C (60), 4 bytes
    bool bFavorite = false;             // +0x40 (64), 1 byte
    bool bRecommend = false;            // +0x41 (65), 1 byte
    // +0x42, 2 bytes padding to 68 bytes
};
static_assert(sizeof(ST_MYROOM_OWNER_INFO) == 68, "ST_MYROOM_OWNER_INFO size mismatch with IDA");

// ST_MYROOM_OWNER_INFO 序列化 (对齐 IDA)
inline XPacket& operator<<(XPacket& packet, const ST_MYROOM_OWNER_INFO& value) {
    packet.XParse << static_cast<int>(value.dwOwnerUAID);
    packet.XParse << value.shMapIndex;
    packet.XParse << value.shGridNo;
    packet.XParse << static_cast<int>(value.dwMapID);
    packet.XParse << static_cast<int>(value.byRoomOpenLevel);
    packet.XParse << std::wstring(value.szRoomName);
    packet.XParse << value.nRecommendCount;
    packet.XParse << value.nFavoriteCount;
    packet.XParse << static_cast<int>(value.bFavorite ? 1 : 0);
    packet.XParse << static_cast<int>(value.bRecommend ? 1 : 0);
    return packet;
}

// ST_MYROOM_OWNER_INFO 反序列化 (对齐 IDA)
inline XPacket& operator>>(XPacket& packet, ST_MYROOM_OWNER_INFO& value) {
    int nOwnerUAID = 0;
    int nMapID = 0;
    int nRoomOpenLevel = 0;
    int nFavorite = 0;
    int nRecommend = 0;
    packet.XParse >> nOwnerUAID;
    value.dwOwnerUAID = static_cast<std::uint32_t>(nOwnerUAID);
    packet.XParse >> value.shMapIndex;
    packet.XParse >> value.shGridNo;
    packet.XParse >> nMapID;
    value.dwMapID = static_cast<std::uint32_t>(nMapID);
    packet.XParse >> nRoomOpenLevel;
    value.byRoomOpenLevel = static_cast<std::uint8_t>(nRoomOpenLevel);
    short outLen = 0;
    packet.XParse.GetWString(value.szRoomName, 20, outLen);
    packet.XParse >> value.nRecommendCount;
    packet.XParse >> value.nFavoriteCount;
    packet.XParse >> nFavorite;
    packet.XParse >> nRecommend;
    value.bFavorite = (nFavorite != 0);
    value.bRecommend = (nRecommend != 0);
    return packet;
}

// ST_MYROOM_USER 序列化 (对齐 IDA)
inline XPacket& operator<<(XPacket& packet, const ST_MYROOM_USER& value) {
    packet.XParse << value.nServerID;
    packet.XParse << static_cast<int>(value.dwUAID);
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << static_cast<int>(value.dwBeforeMapID);
    packet.XParse << value.xBeforePos.x;
    packet.XParse << value.xBeforePos.y;
    packet.XParse << value.xBeforePos.z;
    return packet;
}

// ST_MYROOM_USER 反序列化 (对齐 IDA)
inline XPacket& operator>>(XPacket& packet, ST_MYROOM_USER& value) {
    packet.XParse >> value.nServerID;
    int nUAID = 0;
    int nUCID = 0;
    int nBeforeMapID = 0;
    packet.XParse >> nUAID;
    packet.XParse >> nUCID;
    packet.XParse >> nBeforeMapID;
    value.dwUAID = static_cast<std::uint32_t>(nUAID);
    value.dwUCID = static_cast<std::uint32_t>(nUCID);
    value.dwBeforeMapID = static_cast<std::uint32_t>(nBeforeMapID);
    packet.XParse >> value.xBeforePos.x;
    packet.XParse >> value.xBeforePos.y;
    packet.XParse >> value.xBeforePos.z;
    return packet;
}

// 对齐 IDA DBAgent.exe: ST_MYROOM_ITEM (24 bytes, 4 members)
// MyRoom 物品信息
struct ST_MYROOM_ITEM {
    std::int64_t biSerial = 0;       // +0x00, 8 bytes
    std::uint32_t dwItemID = 0;      // +0x08, 4 bytes
    std::uint8_t byRotation = 0;     // +0x0C, 1 byte
    // +0x0D padding (3 bytes)
    std::uint32_t dwGridIndex = 0;   // +0x10, 4 bytes
    // +0x14 padding (4 bytes)
};
static_assert(sizeof(ST_MYROOM_ITEM) == 24, "ST_MYROOM_ITEM size mismatch with IDA");

// ST_MYROOM_ITEM 序列化
inline XPacket& operator<<(XPacket& packet, const ST_MYROOM_ITEM& value) {
    packet.XParse << value.biSerial;
    packet.XParse << static_cast<int>(value.dwItemID);
    packet.XParse << static_cast<int>(value.byRotation);
    packet.XParse << static_cast<int>(value.dwGridIndex);
    return packet;
}

// ST_MYROOM_ITEM 反序列化
inline XPacket& operator>>(XPacket& packet, ST_MYROOM_ITEM& value) {
    int nItemID = 0;
    int nRotation = 0;
    int nGridIndex = 0;
    packet.XParse >> value.biSerial;
    packet.XParse >> nItemID;
    packet.XParse >> nRotation;
    packet.XParse >> nGridIndex;
    value.dwItemID = static_cast<std::uint32_t>(nItemID);
    value.byRotation = static_cast<std::uint8_t>(nRotation);
    value.dwGridIndex = static_cast<std::uint32_t>(nGridIndex);
    return packet;
}

// ST_MYROOM_ITEM_LIST - 物品列表
struct ST_MYROOM_ITEM_LIST {
    std::vector<ST_MYROOM_ITEM> vecInfo;
};

// ST_MYROOM_ITEM_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const ST_MYROOM_ITEM_LIST& value) {
    packet.XParse << static_cast<int>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// ST_MYROOM_ITEM_LIST 反序列化
inline XPacket& operator>>(XPacket& packet, ST_MYROOM_ITEM_LIST& value) {
    int nCount = 0;
    packet.XParse >> nCount;
    value.vecInfo.clear();
    for (int i = 0; i < nCount; ++i) {
        ST_MYROOM_ITEM item;
        packet >> item;
        value.vecInfo.push_back(item);
    }
    return packet;
}

// ============================================================================
// MyRoom Pollen 结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 花粉互助用户信息（已在 PSServer.h 定义 PS_MYROOM_POLLEN_HELP_USER）

// 对齐 IDA: PS_MYROOM_POLLEN_INFO - 花粉信息
struct PS_MYROOM_POLLEN_INFO {
    int nPollenIndex = 0;               // +0x00
    std::uint32_t dwCultivationItem = 0; // +0x04
    std::uint32_t dwNutritionItem = 0;   // +0x08
    std::uint32_t dwWiltBlockItem = 0;  // +0x0C
    std::int64_t biStartDate = 0;       // +0x10
    std::int64_t biHarvestDate = 0;     // +0x18
    std::int64_t biWiltDate = 0;        // +0x20
    std::int64_t biRotDate = 0;         // +0x28
    std::vector<PS_MYROOM_POLLEN_HELP_USER> vecHelpUser; // 5个互助用户
    std::uint8_t byLookCount = 0;       // +0x??
};

// 对齐 IDA: PS_MYROOM_POLLEN_LIST - 花粉列表
struct PS_MYROOM_POLLEN_LIST {
    std::vector<PS_MYROOM_POLLEN_INFO> vecInfo;
};

// PS_MYROOM_POLLEN_INFO 反序列化
inline XPacket& operator>>(XPacket& packet, PS_MYROOM_POLLEN_INFO& value) {
    packet.XParse >> value.nPollenIndex;
    packet.XParse >> value.dwCultivationItem;
    packet.XParse >> value.dwNutritionItem;
    packet.XParse >> value.dwWiltBlockItem;
    packet.XParse >> value.biStartDate;
    packet.XParse >> value.biHarvestDate;
    packet.XParse >> value.biWiltDate;
    packet.XParse >> value.biRotDate;
    int nHelpCount = 0;
    packet.XParse >> nHelpCount;
    value.vecHelpUser.clear();
    for (int i = 0; i < nHelpCount && i < 5; ++i) {
        PS_MYROOM_POLLEN_HELP_USER user;
        packet >> user;
        value.vecHelpUser.push_back(user);
    }
    packet.XParse >> value.byLookCount;
    return packet;
}

// PS_MYROOM_POLLEN_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MYROOM_POLLEN_INFO& value) {
    packet.XParse << value.nPollenIndex;
    packet.XParse << value.dwCultivationItem;
    packet.XParse << value.dwNutritionItem;
    packet.XParse << value.dwWiltBlockItem;
    packet.XParse << value.biStartDate;
    packet.XParse << value.biHarvestDate;
    packet.XParse << value.biWiltDate;
    packet.XParse << value.biRotDate;
    packet.XParse << static_cast<int>(value.vecHelpUser.size());
    for (const auto& user : value.vecHelpUser) {
        packet << user;
    }
    packet.XParse << value.byLookCount;
    return packet;
}

// ============================================================================
// MyRoom Favorite 结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: ST_MYROOM_FAVORITE_INFO - 收藏信息
struct ST_MYROOM_FAVORITE_INFO {
    std::uint32_t dwOwnerUAID = 0;      // +0x00
    bool bFavorite = false;            // +0x04
    int nRecommendCount = 0;           // +0x08
    wchar_t szRoomName[20] = {};      // +0x0C, 40 bytes
};

// ST_MYROOM_FAVORITE_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_MYROOM_FAVORITE_INFO& value) {
    packet.XParse << static_cast<int>(value.dwOwnerUAID);
    packet.XParse << static_cast<int>(value.bFavorite ? 1 : 0);
    packet.XParse << value.nRecommendCount;
    packet.XParse << std::wstring(value.szRoomName);
    return packet;
}

// ST_MYROOM_FAVORITE_INFO 反序列化
inline XPacket& operator>>(XPacket& packet, ST_MYROOM_FAVORITE_INFO& value) {
    int nOwnerUAID = 0;
    int nFavorite = 0;
    packet.XParse >> nOwnerUAID;
    packet.XParse >> nFavorite;
    packet.XParse >> value.nRecommendCount;
    value.dwOwnerUAID = static_cast<std::uint32_t>(nOwnerUAID);
    value.bFavorite = (nFavorite != 0);
    short outLen = 0;
    packet.XParse.GetWString(value.szRoomName, 20, outLen);
    return packet;
}

// 对齐 IDA: PS_MYROOM_FAVORITE_LIST - 收藏列表
struct PS_MYROOM_FAVORITE_LIST {
    std::vector<ST_MYROOM_FAVORITE_INFO> vecInfo;
};

// PS_MYROOM_FAVORITE_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MYROOM_FAVORITE_LIST& value) {
    packet.XParse << static_cast<int>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

// ============================================================================
// MyRoom Rank 结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: PS_MYROOM_RANK_INFO - 排名信息
struct PS_MYROOM_RANK_INFO {
    std::uint8_t byCurrentRank = 0;    // 当前排名
    std::uint8_t byPastRank = 0;       // 上期排名
    bool bReward = false;             // 是否已领取奖励
    char _pad[1] = {};
    int nRecommendCount = 0;           // 推荐数
    int nFavoriteCount = 0;            // 收藏数
    wchar_t szName[20] = {};           // 房间名
    std::uint32_t dwUAID = 0;          // 所有者UAID
};

// PS_MYROOM_RANK_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MYROOM_RANK_INFO& value) {
    packet.XParse << static_cast<int>(value.byCurrentRank);
    packet.XParse << static_cast<int>(value.byPastRank);
    packet.XParse << value.nRecommendCount;
    packet.XParse << value.nFavoriteCount;
    packet.XParse << std::wstring(value.szName);
    packet.XParse << static_cast<int>(value.dwUAID);
    return packet;
}

// 对齐 IDA: PS_MYROOM_RANK_LIST - 排名列表
struct PS_MYROOM_RANK_LIST {
    std::vector<PS_MYROOM_RANK_INFO> vecInfo;
};

// PS_MYROOM_RANK_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MYROOM_RANK_LIST& value) {
    packet.XParse << static_cast<int>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

// ============================================================================
// MyRoom Recommend 结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: PS_MYROOM_RECOMMEND_LIST - 推荐 UAID 列表
struct PS_MYROOM_RECOMMEND_LIST {
    std::vector<std::uint32_t> vecUAID;
};

// PS_MYROOM_RECOMMEND_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MYROOM_RECOMMEND_LIST& value) {
    packet.XParse << static_cast<int>(value.vecUAID.size());
    for (const auto& uaid : value.vecUAID) {
        packet.XParse << static_cast<int>(uaid);
    }
    return packet;
}

// ============================================================================
// MyRoom Furniture 结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: PS_MYROOM_FUNITURE_LIST - 家具列表（IDA 校正: std::vector<int>）
struct PS_MYROOM_FUNITURE_LIST {
    std::vector<std::int32_t> vecInfo;
};

// ============================================================================
// MyRoom Setup 结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: PS_MYROOM_SETUP - 房间设置
struct PS_MYROOM_SETUP {
    std::uint8_t byOpenLevel = 0;      // 开放等级
    wchar_t szMyRoomName[20] = {};     // 房间名 (40 bytes)
};

// PS_MYROOM_SETUP 反序列化 (对齐 IDA)
inline XPacket& operator>>(XPacket& packet, PS_MYROOM_SETUP& value) {
    int nOpenLevel = 0;
    packet.XParse >> nOpenLevel;
    value.byOpenLevel = static_cast<std::uint8_t>(nOpenLevel);
    short outLen = 0;
    packet.XParse.GetWString(value.szMyRoomName, 20, outLen);
    return packet;
}

// PS_MYROOM_SETUP 序列化 (对齐 IDA)
inline XPacket& operator<<(XPacket& packet, const PS_MYROOM_SETUP& value) {
    packet.XParse << static_cast<int>(value.byOpenLevel);
    packet.XParse << std::wstring(value.szMyRoomName);
    return packet;
}

// ============================================================================
// MyRoom Pollen Cultivation 结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: PS_DB_MYROOM_POLLEN_CULTIVATION - 花粉培养请求
struct PS_DB_MYROOM_POLLEN_CULTIVATION {
    PS_MYROOM_POLLEN_INFO stInfo{};     // 花粉信息
    std::uint32_t dwNutritionItem = 0;  // 营养物品ID
    std::uint32_t dwWiltBlockItem = 0;  // 枯萎阻止物品ID
};

// PS_DB_MYROOM_POLLEN_CULTIVATION 反序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_MYROOM_POLLEN_CULTIVATION& value) {
    packet >> value.stInfo;
    packet.XParse >> value.dwNutritionItem;
    packet.XParse >> value.dwWiltBlockItem;
    return packet;
}

// PS_DB_MYROOM_POLLEN_CULTIVATION 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_MYROOM_POLLEN_CULTIVATION& value) {
    packet << value.stInfo;
    packet.XParse << value.dwNutritionItem;
    packet.XParse << value.dwWiltBlockItem;
    return packet;
}

// ============================================================================
// MyRoom Recommend DB 结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: PS_DB_MYROOM_RECOMMEND - 推荐更新请求
struct PS_DB_MYROOM_RECOMMEND {
    std::uint32_t dwUAID = 0;           // 推荐者 UAID
    std::uint32_t dwOwnerUAID = 0;      // 被推荐的 MyRoom 所有者 UAID
    int nErrorCode = 0;                 // 错误码（输出）
};

// PS_DB_MYROOM_RECOMMEND 反序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_MYROOM_RECOMMEND& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwOwnerUAID;
    return packet;
}

// ============================================================================
// MyRoom Favorite DB 结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: PS_REQ_MYROOM_FAVORITE - 收藏请求
struct PS_REQ_MYROOM_FAVORITE {
    std::uint32_t dwOwnerUAID = 0;      // 被收藏的 MyRoom 所有者 UAID
    bool bFavorite = false;            // true=添加收藏, false=删除收藏
};

// PS_REQ_MYROOM_FAVORITE 反序列化
inline XPacket& operator>>(XPacket& packet, PS_REQ_MYROOM_FAVORITE& value) {
    packet.XParse >> value.dwOwnerUAID;
    packet.XParse >> value.bFavorite;
    return packet;
}

// ============================================================================
// MyRoom Board 结构体 (对齐 IDA DBAgent.exe - 2026-05-04 校正)
// ============================================================================

// 对齐 IDA: PS_MYROOM_BOARD_POT - 花盆信息（IDA size=16）
struct PS_MYROOM_BOARD_POT {
    std::int32_t nItemID = 0;          // 物品 ID
    std::int64_t biHarvestDate = 0;    // 收获日期
};

// 对齐 IDA: ST_MYROOM_BOARD_INFO - MyRoom 房间信息（IDA size=280）
struct ST_MYROOM_BOARD_INFO {
    std::uint32_t dwOwnerUAID = 0;     // MyRoom 所有者 UAID
    std::uint32_t dwUCID = 0;          // 角色 ID
    std::int32_t nRecommendCount = 0;  // 推荐数
    std::int32_t nFavoriteCount = 0;   // 收藏数
    bool bFavorite = false;            // 是否收藏
    char _pad0[1] = {};                // padding (offset 17)
    wchar_t szContents[31] = {};       // 留言内容 (offset 18, size 62)
    wchar_t szName[20] = {};           // 留言者名字 (offset 80, size 40)
    std::int32_t nFunitureID[12] = {}; // 家具 ID 数组 (offset 120, size 48)
    PS_MYROOM_BOARD_POT stPotInfo[6] = {}; // 花盆信息数组 (offset 168, size 96)
    std::int64_t biRegDate = 0;        // 注册时间 (offset 264, size 8)
    std::int32_t nFunitureCount = 0;   // 家具数量 (offset 272, size 4)
};

// 对齐 IDA: PS_REQ_MYROOM_BOARD_INFO - 留言板请求（IDA size=8）
struct PS_REQ_MYROOM_BOARD_INFO {
    std::uint32_t dwUAID = 0;          // 请求者 UAID
    std::uint8_t byPage = 0;           // 页码
};

// PS_REQ_MYROOM_BOARD_INFO 反序列化
inline XPacket& operator>>(XPacket& packet, PS_REQ_MYROOM_BOARD_INFO& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.byPage;
    return packet;
}

// ST_MYROOM_BOARD_INFO 反序列化
inline XPacket& operator>>(XPacket& packet, ST_MYROOM_BOARD_INFO& value) {
    packet.XParse >> value.dwOwnerUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nRecommendCount;
    packet.XParse >> value.nFavoriteCount;
    int nFavorite = 0;
    packet.XParse >> nFavorite;
    value.bFavorite = (nFavorite != 0);
    std::wstring strContents;
    packet.XParse >> strContents;
    std::wcsncpy(value.szContents, strContents.c_str(), 30);
    value.szContents[30] = L'\0';
    std::wstring strName;
    packet.XParse >> strName;
    std::wcsncpy(value.szName, strName.c_str(), 19);
    value.szName[19] = L'\0';
    for (int i = 0; i < 12; ++i) {
        packet.XParse >> value.nFunitureID[i];
    }
    for (int i = 0; i < 6; ++i) {
        packet.XParse >> value.stPotInfo[i].nItemID;
        packet.XParse >> value.stPotInfo[i].biHarvestDate;
    }
    packet.XParse >> value.biRegDate;
    packet.XParse >> value.nFunitureCount;
    return packet;
}

// 对齐 IDA: PS_MYROOM_BOARD_LIST - 留言板列表（IDA size=40）
struct PS_MYROOM_BOARD_LIST {
    std::int32_t nMaxPage = 0;         // 最大页数
    char _pad0[4] = {};                // padding (offset 4)
    std::vector<ST_MYROOM_BOARD_INFO> vecInfo; // 留言列表 (offset 8)
};

// ST_MYROOM_BOARD_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_MYROOM_BOARD_INFO& value) {
    packet.XParse << static_cast<int>(value.dwOwnerUAID);
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << value.nRecommendCount;
    packet.XParse << value.nFavoriteCount;
    packet.XParse << static_cast<int>(value.bFavorite ? 1 : 0);
    packet.XParse << std::wstring(value.szContents);
    packet.XParse << std::wstring(value.szName);
    for (int i = 0; i < 12; ++i) {
        packet.XParse << value.nFunitureID[i];
    }
    for (int i = 0; i < 6; ++i) {
        packet.XParse << value.stPotInfo[i].nItemID;
        packet.XParse << value.stPotInfo[i].biHarvestDate;
    }
    packet.XParse << value.biRegDate;
    packet.XParse << value.nFunitureCount;
    return packet;
}

// PS_MYROOM_BOARD_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MYROOM_BOARD_LIST& value) {
    packet.XParse << value.nMaxPage;
    packet.XParse << static_cast<int>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// ============================================================================
// MyRoom Rank Reward 结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: PS_DB_MYROOM_RANK_REWARD - 排名奖励请求
struct PS_DB_MYROOM_RANK_REWARD {
    std::uint32_t dwUAID = 0;          // 请求者 UAID
    int nErrorCode = 0;                 // 错误码（输出）
};

// PS_DB_MYROOM_RANK_REWARD 反序列化
inline XPacket& operator>>(XPacket& packet, PS_DB_MYROOM_RANK_REWARD& value) {
    packet.XParse >> value.dwUAID;
    return packet;
}

// PS_DB_MYROOM_RANK_REWARD 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_MYROOM_RANK_REWARD& value) {
    packet.XParse << static_cast<int>(value.dwUAID);
    packet.XParse << value.nErrorCode;
    return packet;
}

// ============================================================================
// MyRoom Community 结构体 (对齐 IDA DBAgent.exe)
// ============================================================================

// 对齐 IDA: PS_MYROOM_COMMUNITY_INFO - 社区信息
struct PS_MYROOM_COMMUNITY_INFO {
    int nRecommendCount = 0;           // 推荐数
    int nFavoriteCount = 0;            // 收藏数
};

// PS_MYROOM_COMMUNITY_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MYROOM_COMMUNITY_INFO& value) {
    packet.XParse << value.nRecommendCount;
    packet.XParse << value.nFavoriteCount;
    return packet;
}
