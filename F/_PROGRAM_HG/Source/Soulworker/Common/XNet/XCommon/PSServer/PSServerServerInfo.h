#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerServerInfo.h - 服务器信息相关结构体及序列化运算符
//
// 此文件包含服务器信息同步的所有数据结构和序列化操作：
// - SS_SERVER_INFO - 服务器状态同步
// - PS_SERVER_COMMON_INFO - 服务器统计信息
// - ST_SYNC_INFO - 同步信息
// - ST_SERVER_GROUP_INFO - 服务器组信息
// - ST_SERVER_GROUP_INFO_VEC - 服务器组信息列表
// - ST_SERVER_INFO_FOR_USER - 发给客户端的服务器信息
// - PS_SERVERS_INFO_FOR_USER - 服务器信息列表
// ============================================================================

// ============================================================================
// 服务器信息结构体
// ============================================================================

/**
 * @brief 控制服 / Relay 之间同步单台服务器状态的结构。
 *
 * PDB 中命名为 `SS_SERVER_INFO`，由 `XRelaySocket::SendAddServer` 直接发送。
 */
struct SS_SERVER_INFO {
    std::uint32_t dwID = 0;
    int nGroup = 0;
    int nType = 0;
    int nChannel = 0;
    int nState = 0;
    int nCurUser = 0;
    int nMaxUser = 0;
    std::uint16_t sThreadCount = 0;
    std::uint16_t sPort = 0;
    char szName[21] = {};
    char szPrivateIP[513] = {};
    char szPublicIP[513] = {};
    int nMazeCount = 0;
    int nNPCCount = 0;
};

/**
 * @brief 服务器统计信息（用于监控响应）。
 *
 * 对齐 IDA SendServerInfoAll: 包含服务器数和总用户数。
 */
struct PS_SERVER_COMMON_INFO {
    int nServerCount = 0;
    int nTotalUserCount = 0;
};

/**
 * @brief 服务器同步信息结构体（CServer::m_stSyncInfo）。
 *
 * 对齐 IDA RecvMapInfo: 用于控制地图同步状态标志。
 * RecvMapInfo 清除 SyncData 的 bit 1 表示开始接收地图信息。
 */
struct ST_SYNC_INFO {
    std::uint32_t SyncData = 0;  // 同步数据，bit 1 用于地图同步标志
};

/**
 * @brief 发给客户端的服务器组基础信息。
 */
struct ST_SERVER_GROUP_INFO {
    std::uint16_t wID = 0;
    std::int16_t sPort = 0;
    char szName[21] = {};
    char szPublicIP[513] = {};
    int nState = 0;
    int nUserCount = 0;
};

/**
 * @brief 服务器组信息列表（用于 DBAgent 加载服务器组信息）。
 */
struct ST_SERVER_GROUP_INFO_VEC {
    std::vector<ST_SERVER_GROUP_INFO> vecServerInfos;
};

/**
 * @brief 发给客户端的服务器信息（继承自 ST_SERVER_GROUP_INFO）。
 */
struct ST_SERVER_INFO_FOR_USER : ST_SERVER_GROUP_INFO {
    std::uint8_t byCharacterCount = 0;
};

/**
 * @brief 服务器信息列表（发给客户端）。
 */
struct PS_SERVERS_INFO_FOR_USER {
    std::uint8_t byLastServerID = 0;
    std::vector<ST_SERVER_INFO_FOR_USER> vecServerInfo;
};

// ============================================================================
// Static Assertions - 大小和偏移验证
// ============================================================================

static_assert(sizeof(ST_SERVER_GROUP_INFO) == 0x224, "ST_SERVER_GROUP_INFO size must match PDB");
static_assert(offsetof(ST_SERVER_GROUP_INFO, szPublicIP) == 0x19, "ST_SERVER_GROUP_INFO.szPublicIP offset mismatch");
static_assert(offsetof(ST_SERVER_GROUP_INFO, nState) == 0x21C, "ST_SERVER_GROUP_INFO.nState offset mismatch");
static_assert(sizeof(SS_SERVER_INFO) == 0x440, "SS_SERVER_INFO size must match PDB");
static_assert(offsetof(SS_SERVER_INFO, szName) == 0x20, "SS_SERVER_INFO.szName offset mismatch");
static_assert(offsetof(SS_SERVER_INFO, szPrivateIP) == 0x35, "SS_SERVER_INFO.szPrivateIP offset mismatch");
static_assert(offsetof(SS_SERVER_INFO, szPublicIP) == 0x236, "SS_SERVER_INFO.szPublicIP offset mismatch");
static_assert(offsetof(SS_SERVER_INFO, nMazeCount) == 0x438, "SS_SERVER_INFO.nMazeCount offset mismatch");

// ============================================================================
// 服务器信息序列化运算符
// ============================================================================

// SS_SERVER_INFO 反序列化
inline void operator>>(XPacket& packet, SS_SERVER_INFO& value) {
    short outLen = 0;
    packet.XParse >> value.dwID;
    packet.XParse >> value.nGroup;
    packet.XParse >> value.nType;
    packet.XParse >> value.nChannel;
    packet.XParse >> value.nState;
    packet.XParse >> value.nCurUser;
    packet.XParse >> value.nMaxUser;
    packet.XParse >> value.sThreadCount;
    packet.XParse >> value.sPort;
    packet.XParse.GetString(value.szName, static_cast<short>(sizeof(value.szName)), &outLen);
    packet.XParse.GetString(value.szPrivateIP, static_cast<short>(sizeof(value.szPrivateIP)), &outLen);
    packet.XParse.GetString(value.szPublicIP, static_cast<short>(sizeof(value.szPublicIP)), &outLen);
    packet.XParse >> value.nMazeCount;
    packet.XParse >> value.nNPCCount;
}

// SS_SERVER_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const SS_SERVER_INFO& value) {
    packet.XParse << value.dwID;
    packet.XParse << value.nGroup;
    packet.XParse << value.nType;
    packet.XParse << value.nChannel;
    packet.XParse << value.nState;
    packet.XParse << value.nCurUser;
    packet.XParse << value.nMaxUser;
    packet.XParse << value.sThreadCount;
    packet.XParse << value.sPort;
    packet.XParse << std::string(value.szName);
    packet.XParse << std::string(value.szPrivateIP);
    packet.XParse << std::string(value.szPublicIP);
    packet.XParse << value.nMazeCount;
    packet.XParse << value.nNPCCount;
    return packet;
}

// PS_SERVER_COMMON_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_COMMON_INFO& value) {
    packet.XParse << value.nServerCount;
    packet.XParse << value.nTotalUserCount;
    return packet;
}

// ST_SERVER_GROUP_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_SERVER_GROUP_INFO& value) {
    packet.XParse << value.wID;
    packet.XParse << value.sPort;
    packet.XParse << std::string(value.szName);
    packet.XParse << std::string(value.szPublicIP);
    packet.XParse << value.nState;
    packet.XParse << value.nUserCount;
    return packet;
}

// ST_SERVER_GROUP_INFO 反序列化
inline void operator>>(XPacket& packet, ST_SERVER_GROUP_INFO& value) {
    std::string szName;
    std::string szPublicIP;
    packet.XParse >> value.wID;
    packet.XParse >> value.sPort;
    packet.XParse >> szName;
    packet.XParse >> szPublicIP;
    packet.XParse >> value.nState;
    packet.XParse >> value.nUserCount;
    std::strncpy(value.szName, szName.c_str(), sizeof(value.szName) - 1);
    std::strncpy(value.szPublicIP, szPublicIP.c_str(), sizeof(value.szPublicIP) - 1);
}

// ST_SERVER_GROUP_INFO_VEC 序列化
inline XPacket& operator<<(XPacket& packet, const ST_SERVER_GROUP_INFO_VEC& value) {
    packet.XParse << static_cast<std::uint16_t>(value.vecServerInfos.size());
    for (const auto& info : value.vecServerInfos) {
        packet << info;
    }
    return packet;
}

// ST_SERVER_GROUP_INFO_VEC 反序列化
inline void operator>>(XPacket& packet, ST_SERVER_GROUP_INFO_VEC& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecServerInfos.resize(count);
    for (auto& info : value.vecServerInfos) {
        packet >> info;
    }
}

// ST_SERVER_INFO_FOR_USER 序列化
inline XPacket& operator<<(XPacket& packet, const ST_SERVER_INFO_FOR_USER& value) {
    packet << static_cast<const ST_SERVER_GROUP_INFO&>(value);
    return packet;
}

// ST_SERVER_INFO_FOR_USER 反序列化
inline void operator>>(XPacket& packet, ST_SERVER_INFO_FOR_USER& value) {
    packet >> static_cast<ST_SERVER_GROUP_INFO&>(value);
}

// PS_SERVERS_INFO_FOR_USER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVERS_INFO_FOR_USER& value) {
    packet.XParse << static_cast<std::uint16_t>(value.vecServerInfo.size());
    for (const auto& info : value.vecServerInfo) {
        packet << info;
    }
    return packet;
}
