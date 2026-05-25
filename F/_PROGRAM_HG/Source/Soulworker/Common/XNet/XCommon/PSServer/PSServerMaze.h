#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerMaze.h - 迷宫进入限制相关结构体及序列化运算符
//
// 此文件包含迷宫进入限制的所有数据结构和序列化操作：
// - PS_UPDATE_MAZE_ENTER_LIMIT_COUNT - 迷宫进入次数更新
// - PS_MAZE_ENTER_LIMIT_COUNT_* 系列 - 迷宫进入限制计数
// - PS_GF_BILLING_RELOAD_RES - GF计费重载响应
//
// 注意: PS_MAZE_UPDATE_INFO 保留在 PSServer.h 中，因为它依赖 ST_MAZE_WAIT_ENTER_USER_INFO
// ============================================================================

// ============================================================================
// 迷宫进入限制计数结构体
// ============================================================================

// 对齐 IDA DBAgent.exe: PS_UPDATE_MAZE_ENTER_LIMIT_COUNT (size 4)
struct PS_UPDATE_MAZE_ENTER_LIMIT_COUNT {
    std::uint16_t wMapID = 0;           // offset 0, size 2
    std::uint8_t byCount = 0;           // offset 2, size 1
    std::uint8_t byPCBangCount = 0;     // offset 3, size 1
};

static_assert(sizeof(PS_UPDATE_MAZE_ENTER_LIMIT_COUNT) == 4, "PS_UPDATE_MAZE_ENTER_LIMIT_COUNT size must match IDA");

// 对齐 IDA DBAgent.exe: PS_DB_MAZE_ENTER_LIMIT_COUNT_GROUP_UPDATE
struct PS_DB_MAZE_ENTER_LIMIT_COUNT_GROUP_UPDATE {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::uint32_t dwGroupID = 0;
    std::uint32_t dwMazeID = 0;
    std::uint32_t dwCount = 0;
    std::uint32_t dwPCBangCount = 0;
    std::int32_t nUpdateTime = 0;
    std::uint8_t byType = 0;  // 0=Character, 1=Account
};

// 对齐 IDA DBAgent.exe: PS_MAZE_ENTER_LIMIT_COUNT_CLEAR
struct PS_MAZE_ENTER_LIMIT_COUNT_CLEAR {
    std::uint32_t dwActorID = 0;
    std::int32_t nClearTime = 0;
};

// 对齐 IDA DBAgent.exe: PS_MAZE_ENTER_LIMIT_COUNT_LIST (size 40)
struct PS_MAZE_ENTER_LIMIT_COUNT_LIST {
    std::uint32_t dwActorID = 0;                                              // offset 0, size 4
    std::uint8_t byResultType = 0;                                            // offset 4, size 1
    std::vector<PS_UPDATE_MAZE_ENTER_LIMIT_COUNT> listEnterMazeCount;         // offset 8, size 32
};

static_assert(sizeof(PS_MAZE_ENTER_LIMIT_COUNT_LIST) == 40, "PS_MAZE_ENTER_LIMIT_COUNT_LIST size must match IDA");

// 对齐 IDA DBAgent.exe: PS_MAZE_ENTER_LIMIT_COUNT_GROUP (size 48)
struct PS_MAZE_ENTER_LIMIT_COUNT_GROUP {
    std::uint16_t wGroupID = 0;                                               // offset 0, size 2
    std::uint8_t byTotalCount = 0;                                            // offset 2, size 1
    std::uint8_t byTotalPCBangCount = 0;                                      // offset 3, size 1
    std::int64_t nLastUpdate = 0;                                             // offset 8, size 8
    std::vector<PS_UPDATE_MAZE_ENTER_LIMIT_COUNT> vecList;                    // offset 16, size 32
};

static_assert(sizeof(PS_MAZE_ENTER_LIMIT_COUNT_GROUP) == 48, "PS_MAZE_ENTER_LIMIT_COUNT_GROUP size must match IDA");

// 对齐 IDA DBAgent.exe: PS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST
// 注意：IDA 显示 mapGroup offset=16, size=32 (老版 MSVC std::map)
// clang-cl/新版 MSVC std::map 可能更大，取消严格大小断言
struct PS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST {
    std::uint32_t dwUAID = 0;                                                 // offset 0, size 4
    std::uint32_t dwUCID = 0;                                                 // offset 4, size 4
    std::uint8_t byResultType = 0;                                            // offset 8, size 1
    // padding 7 bytes (offset 9-15)
    std::map<std::uint16_t, PS_MAZE_ENTER_LIMIT_COUNT_GROUP> mapGroup;        // offset 16
};

// 对齐 IDA DBAgent.exe: PS_MAZE_ENTER_LIMIT_COUNT_GROUP_CLEAR (size 56)
struct PS_MAZE_ENTER_LIMIT_COUNT_GROUP_CLEAR {
    std::uint32_t dwUAID = 0;                                          // offset 0, size 4
    std::uint32_t dwUCID = 0;                                          // offset 4, size 4
    std::uint8_t byType = 0;                                           // offset 8, size 1
    // padding offset 9-15, size 7
    std::int64_t nClearTime = 0;                                       // offset 16, size 8
    std::vector<std::uint16_t> vecDelGroupID;                          // offset 24, size 32
};

static_assert(sizeof(PS_MAZE_ENTER_LIMIT_COUNT_GROUP_CLEAR) == 56, "PS_MAZE_ENTER_LIMIT_COUNT_GROUP_CLEAR size must match IDA");

// 对齐 IDA DBAgent.exe: PS_GF_BILLING_RELOAD_RES (size 32)
struct PS_GF_BILLING_RELOAD_RES {
    std::vector<std::uint32_t> vecUAID;                                // offset 0, size 32
};

static_assert(sizeof(PS_GF_BILLING_RELOAD_RES) == 32, "PS_GF_BILLING_RELOAD_RES size must match IDA");

// ============================================================================
// 迷宫进入限制计数序列化运算符
// ============================================================================

// PS_UPDATE_MAZE_ENTER_LIMIT_COUNT 反序列化
inline void operator>>(XPacket& packet, PS_UPDATE_MAZE_ENTER_LIMIT_COUNT& value) {
    packet.XParse >> value.wMapID;
    packet.XParse >> value.byCount;
    packet.XParse >> value.byPCBangCount;
}

// PS_MAZE_ENTER_LIMIT_COUNT_LIST 反序列化
inline void operator>>(XPacket& packet, PS_MAZE_ENTER_LIMIT_COUNT_LIST& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.byResultType;
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.listEnterMazeCount.resize(count);
    for (auto& item : value.listEnterMazeCount) {
        packet >> item;
    }
}

// PS_DB_MAZE_ENTER_LIMIT_COUNT_GROUP_UPDATE 反序列化
inline void operator>>(XPacket& packet, PS_DB_MAZE_ENTER_LIMIT_COUNT_GROUP_UPDATE& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwGroupID;
    packet.XParse >> value.dwMazeID;
    packet.XParse >> value.dwCount;
    packet.XParse >> value.dwPCBangCount;
    packet.XParse >> value.nUpdateTime;
    packet.XParse >> value.byType;
}

// PS_MAZE_ENTER_LIMIT_COUNT_CLEAR 反序列化
inline void operator>>(XPacket& packet, PS_MAZE_ENTER_LIMIT_COUNT_CLEAR& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.nClearTime;
}

// PS_MAZE_ENTER_LIMIT_COUNT_GROUP_CLEAR 反序列化
inline void operator>>(XPacket& packet, PS_MAZE_ENTER_LIMIT_COUNT_GROUP_CLEAR& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byType;
    // 跳过 padding (offset 9-15)
    packet.XParse >> value.nClearTime;
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecDelGroupID.clear();
    value.vecDelGroupID.reserve(static_cast<std::size_t>(count));
    for (std::uint16_t index = 0; index < count; ++index) {
        std::uint16_t groupID = 0;
        packet.XParse >> groupID;
        value.vecDelGroupID.push_back(groupID);
    }
}

// PS_GF_BILLING_RELOAD_RES 反序列化
inline void operator>>(XPacket& packet, PS_GF_BILLING_RELOAD_RES& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecUAID.clear();
    value.vecUAID.reserve(static_cast<std::size_t>(count));
    for (std::uint16_t index = 0; index < count; ++index) {
        std::uint32_t uaid = 0;
        packet.XParse >> uaid;
        value.vecUAID.push_back(uaid);
    }
}

// PS_GF_BILLING_RELOAD_RES 序列化
inline XPacket& operator<<(XPacket& packet, const PS_GF_BILLING_RELOAD_RES& value) {
    packet.XParse << static_cast<std::uint16_t>(value.vecUAID.size());
    for (const auto& uaid : value.vecUAID) {
        packet.XParse << uaid;
    }
    return packet;
}

// PS_MAZE_ENTER_LIMIT_COUNT_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MAZE_ENTER_LIMIT_COUNT_LIST& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.byResultType;
    packet.XParse << static_cast<std::uint16_t>(value.listEnterMazeCount.size());
    for (const auto& item : value.listEnterMazeCount) {
        packet.XParse << item.wMapID;
        packet.XParse << item.byCount;
        packet.XParse << item.byPCBangCount;
    }
    return packet;
}

// PS_MAZE_ENTER_LIMIT_COUNT_GROUP 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MAZE_ENTER_LIMIT_COUNT_GROUP& value) {
    packet.XParse << value.wGroupID;
    packet.XParse << value.byTotalCount;
    packet.XParse << value.byTotalPCBangCount;
    packet.XParse << value.nLastUpdate;
    packet.XParse << static_cast<std::uint16_t>(value.vecList.size());
    for (const auto& item : value.vecList) {
        packet.XParse << item.wMapID;
        packet.XParse << item.byCount;
        packet.XParse << item.byPCBangCount;
    }
    return packet;
}

// PS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.byResultType;
    packet.XParse << static_cast<std::uint16_t>(value.mapGroup.size());
    for (const auto& pair : value.mapGroup) {
        packet << pair.second;
    }
    return packet;
}
