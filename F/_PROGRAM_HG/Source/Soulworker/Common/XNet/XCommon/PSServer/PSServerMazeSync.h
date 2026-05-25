#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMapMaze.h"

// ============================================================================
// PSServerMazeSync.h - 迷宫同步/更新相关结构体及序列化运算符
//
// 此文件包含迷宫同步和更新的所有数据结构和序列化操作：
// - ST_SERVER_CHECK_ENTER_MAZE - 服务器迷宫进入检查
// - ST_MAZE_WAIT_ENTER_USER_INFO - 迷宫等待进入用户信息
// - ST_MAZE_MEMBER_INFO_SYNC - 迷宫成员信息同步
// - PS_MAZE_INFO_SYNC - 迷宫信息同步
// - PS_MAZE_UPDATE_INFO_SYNC - 迷宫更新信息同步
// - PS_MAZE_UPDATE_INFO - 迷宫更新信息
// - PS_ROULETTE_EVENT_UPDATE_SERVER - 轮盘事件更新
// - PS_DB_INIT_ROULETTE_INFO - 轮盘初始化请求
// - PS_PLAY_TIME_BY_ACCOUNT - 账户游戏时间
// - PS_CHANGE_NAME - 改名请求
// ============================================================================

// ============================================================================
// 迷宫同步结构体
// ============================================================================

/**
 * @brief 服务器迷宫进入检查结构体。
 */
struct ST_SERVER_CHECK_ENTER_MAZE {
    int nMapID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t byState = 0;
};

/**
 * @brief 迷宫等待进入用户信息 (48 bytes, 10 members)。
 */
struct ST_MAZE_WAIT_ENTER_USER_INFO {
    // +0x00 (8 bytes): stMemberInfo
    ST_ENTER_MAZE_MEMBER_INFO stMemberInfo{};
    // +0x08 (8 bytes): dw64ExitTime
    std::uint64_t dw64ExitTime = 0;
    // +0x10 (1 byte): byState
    std::uint8_t byState = 0;
    // +0x14 (4 bytes): nTeam
    int nTeam = 0;
    // +0x18 (1 byte): bEnter
    bool bEnter = false;
    // +0x19 (1 byte): bCheckCondition
    bool bCheckCondition = false;
    // +0x1A (1 byte): bLoadEX
    bool bLoadEX = false;
    // +0x20 (8 bytes): dwCheckSectorPosTick
    std::uint64_t dwCheckSectorPosTick = 0;
    // +0x28 (4 bytes): nReEnterCount
    int nReEnterCount = 0;
    // +0x2C (4 bytes): nRestartState
    int nRestartState = 0;
};

/**
 * @brief 迷宫成员信息同步结构体。
 */
struct ST_MAZE_MEMBER_INFO_SYNC {
    std::uint32_t dwActorID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t byState = 0;
};

/**
 * @brief 迷宫信息同步结构体。
 * 注意: vecMemberInfo 类型根据 IDA CMazeInfo::SyncMazeInfo 使用 ST_MAZE_WAIT_ENTER_USER_INFO
 */
struct PS_MAZE_INFO_SYNC {
    UXMapID uxMapID{};
    int nUserCount = 0;
    int nState = 0;
    std::vector<ST_MAZE_WAIT_ENTER_USER_INFO> vecMemberInfo;
};

/**
 * @brief 迷宫更新信息同步结构体 (包含 bLast 标志和其他字段)。
 */
struct PS_MAZE_UPDATE_INFO_SYNC {
    bool bLast = false;
    ST_PARTY_INFO stPartyInfo{};
    std::uint32_t dwServerID = 0;
    std::int16_t sPort = 0;
    int nJumpID = 0;
    char szIP[513] = {};
    PS_MAZE_INFO_SYNC psMazeInfo{};
};

/**
 * @brief 迷宫更新信息 (ControlServer, size 48)。
 */
#define PS_MAZE_UPDATE_INFO_DEFINED
struct PS_MAZE_UPDATE_INFO {
    UXMapID uxMapID{};
    int nState = 0;
    int nUserCount = 0;
    std::vector<ST_MAZE_WAIT_ENTER_USER_INFO> vecMemberInfo;
};

/**
 * @brief 轮盘事件更新结构体（服务器版）。
 * 注意: RouletteEventManager.h 中有 PS_ROULETTE_EVENT_UPDATE_INFO（字段相同）
 */
struct PS_ROULETTE_EVENT_UPDATE_SERVER {
    int nEventID = 0;
    int nRewardID = 0;
    int nRemainCount = 0;
};

/**
 * @brief 轮盘初始化请求结构体。
 */
struct PS_DB_INIT_ROULETTE_INFO {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::int32_t nEventID = 0;
    std::int64_t biInitDate = 0;
};

/**
 * @brief 账户游戏时间信息结构体。
 */
struct PS_PLAY_TIME_BY_ACCOUNT {
    std::uint32_t dwUAID = 0;
    std::int32_t nSec = 0;
    std::int32_t nInitTime = 0;
    std::uint8_t byState = 0;
};

/**
 * @brief 改名请求结构体。
 */
struct PS_CHANGE_NAME {
    std::uint32_t dwActorID = 0;
    wchar_t szChangeName[21] = {};
};

// ============================================================================
// Static Assertions - 大小和偏移验证
// ============================================================================

static_assert(sizeof(ST_MAZE_WAIT_ENTER_USER_INFO) == 48,
              "ST_MAZE_WAIT_ENTER_USER_INFO size mismatch with IDA");

// ============================================================================
// 迷宫同步序列化运算符
// ============================================================================

// ST_SERVER_CHECK_ENTER_MAZE 序列化
inline XPacket& operator<<(XPacket& packet, const ST_SERVER_CHECK_ENTER_MAZE& value) {
    packet.XParse << value.nMapID;
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << value.byState;
    return packet;
}

// ST_SERVER_CHECK_ENTER_MAZE 反序列化
inline void operator>>(XPacket& packet, ST_SERVER_CHECK_ENTER_MAZE& value) {
    packet.XParse >> value.nMapID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byState;
}

// ST_MAZE_WAIT_ENTER_USER_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_MAZE_WAIT_ENTER_USER_INFO& value) {
    packet << value.stMemberInfo;
    packet.XParse << static_cast<std::int64_t>(value.dw64ExitTime);
    packet.XParse << value.byState;
    packet.XParse << value.nTeam;
    packet.XParse << value.bEnter;
    packet.XParse << value.bCheckCondition;
    packet.XParse << value.bLoadEX;
    packet.XParse << static_cast<std::int64_t>(value.dwCheckSectorPosTick);
    packet.XParse << value.nReEnterCount;
    packet.XParse << value.nRestartState;
    return packet;
}

// ST_MAZE_WAIT_ENTER_USER_INFO 反序列化
inline void operator>>(XPacket& packet, ST_MAZE_WAIT_ENTER_USER_INFO& value) {
    packet >> value.stMemberInfo;
    std::int64_t dw64ExitTimeTmp = 0;
    packet.XParse >> dw64ExitTimeTmp;
    value.dw64ExitTime = static_cast<std::uint64_t>(dw64ExitTimeTmp);
    packet.XParse >> value.byState;
    packet.XParse >> value.nTeam;
    packet.XParse >> value.bEnter;
    packet.XParse >> value.bCheckCondition;
    packet.XParse >> value.bLoadEX;
    std::int64_t dwCheckSectorPosTickTmp = 0;
    packet.XParse >> dwCheckSectorPosTickTmp;
    value.dwCheckSectorPosTick = static_cast<std::uint64_t>(dwCheckSectorPosTickTmp);
    packet.XParse >> value.nReEnterCount;
    packet.XParse >> value.nRestartState;
}

// ST_MAZE_MEMBER_INFO_SYNC 序列化
inline XPacket& operator<<(XPacket& packet, const ST_MAZE_MEMBER_INFO_SYNC& value) {
    packet.XParse << static_cast<int>(value.dwActorID);
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << value.byState;
    return packet;
}

// ST_MAZE_MEMBER_INFO_SYNC 反序列化
inline void operator>>(XPacket& packet, ST_MAZE_MEMBER_INFO_SYNC& value) {
    int nActorID = 0;
    int nUCID = 0;
    packet.XParse >> nActorID;
    packet.XParse >> nUCID;
    packet.XParse >> value.byState;
    value.dwActorID = static_cast<std::uint32_t>(nActorID);
    value.dwUCID = static_cast<std::uint32_t>(nUCID);
}

// PS_MAZE_INFO_SYNC 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MAZE_INFO_SYNC& value) {
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.nUserCount;
    packet.XParse << value.nState;
    packet.XParse << static_cast<int>(value.vecMemberInfo.size());
    for (const auto& item : value.vecMemberInfo) {
        packet << item;
    }
    return packet;
}

// PS_MAZE_INFO_SYNC 反序列化
inline void operator>>(XPacket& packet, PS_MAZE_INFO_SYNC& value) {
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.nUserCount;
    packet.XParse >> value.nState;
    int nCount = 0;
    packet.XParse >> nCount;
    value.vecMemberInfo.clear();
    value.vecMemberInfo.reserve(static_cast<std::size_t>(nCount));
    for (int i = 0; i < nCount; ++i) {
        ST_MAZE_WAIT_ENTER_USER_INFO item{};
        packet >> item;
        value.vecMemberInfo.push_back(item);
    }
}

// PS_MAZE_UPDATE_INFO_SYNC 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MAZE_UPDATE_INFO_SYNC& value) {
    packet.XParse << static_cast<int>(value.bLast ? 1 : 0);
    packet << value.stPartyInfo;
    packet.XParse << static_cast<int>(value.dwServerID);
    packet.XParse << value.sPort;
    packet.XParse << value.nJumpID;
    packet.XParse << std::string(value.szIP);
    packet << value.psMazeInfo;
    return packet;
}

// PS_MAZE_UPDATE_INFO_SYNC 反序列化
inline void operator>>(XPacket& packet, PS_MAZE_UPDATE_INFO_SYNC& value) {
    int nLast = 0;
    packet.XParse >> nLast;
    value.bLast = (nLast != 0);
    packet >> value.stPartyInfo;
    int nServerID = 0;
    packet.XParse >> nServerID;
    value.dwServerID = static_cast<std::uint32_t>(nServerID);
    packet.XParse >> value.sPort;
    packet.XParse >> value.nJumpID;
    short sLen = 0;
    packet.XParse.GetString(value.szIP, static_cast<short>(sizeof(value.szIP)), &sLen);
    packet >> value.psMazeInfo;
}

// PS_MAZE_UPDATE_INFO 反序列化
inline void operator>>(XPacket& packet, PS_MAZE_UPDATE_INFO& value) {
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.nState;
    packet.XParse >> value.nUserCount;
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecMemberInfo.clear();
    value.vecMemberInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_MAZE_WAIT_ENTER_USER_INFO item{};
        packet >> item;
        value.vecMemberInfo.push_back(item);
    }
}

// PS_ROULETTE_EVENT_UPDATE_SERVER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_ROULETTE_EVENT_UPDATE_SERVER& value) {
    packet.XParse << value.nEventID;
    packet.XParse << value.nRewardID;
    packet.XParse << value.nRemainCount;
    return packet;
}

// PS_ROULETTE_EVENT_UPDATE_SERVER 反序列化
inline void operator>>(XPacket& packet, PS_ROULETTE_EVENT_UPDATE_SERVER& value) {
    packet.XParse >> value.nEventID;
    packet.XParse >> value.nRewardID;
    packet.XParse >> value.nRemainCount;
}

// PS_DB_INIT_ROULETTE_INFO 反序列化
inline void operator>>(XPacket& packet, PS_DB_INIT_ROULETTE_INFO& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nEventID;
    packet.XParse >> value.biInitDate;
}

// PS_PLAY_TIME_BY_ACCOUNT 反序列化
inline void operator>>(XPacket& packet, PS_PLAY_TIME_BY_ACCOUNT& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.nSec;
    packet.XParse >> value.nInitTime;
    packet.XParse >> value.byState;
}

// PS_PLAY_TIME_BY_ACCOUNT 序列化
inline XPacket& operator<<(XPacket& packet, const PS_PLAY_TIME_BY_ACCOUNT& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.nSec;
    packet.XParse << value.nInitTime;
    packet.XParse << value.byState;
    return packet;
}

// PS_CHANGE_NAME 反序列化
inline void operator>>(XPacket& packet, PS_CHANGE_NAME& value) {
    packet.XParse >> value.dwActorID;
    short outLen = 0;
    packet.XParse.GetWString(value.szChangeName, 21, outLen);
}

// PS_CHANGE_NAME 序列化
inline XPacket& operator<<(XPacket& packet, const PS_CHANGE_NAME& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szChangeName);
    return packet;
}
