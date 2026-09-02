#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerModeMaze.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"

// ============================================================================
// PSServerMapMaze.h - 地图/迷宫创建相关结构体及序列化运算符
//
// 此文件包含地图/迷宫创建的所有数据结构和序列化操作：
// - ST_MAP_INFO - 地图/切服目标信息
// - PS_ENTER_MAP_RES - 进入地图响应
// - PS_CREATE_MAP, PS_CREATE_MAP_RES, PS_CREATE_MAP_LIST - 创建地图
// - ST_DB_CHANNEL_MAP - DB频道地图信息
// - PS_ENTER_MAP_REQ - 进入地图请求
// - ST_ENTER_MAZE_MEMBER_INFO - 迷宫成员信息
// - PS_REQ_CHANGE_SERVER, PS_RES_CHANGE_SERVER - 切服请求/响应
// - ST_CREATE_MAZE, ST_CREATE_MODE_MAZE - 创建迷宫
// - ST_CREATE_MAZE_FOR_RELAY, ST_CREATE_MODE_MAZE_FOR_RELAY - 迷宫创建中继
// - ST_GO_BACK_MAZE - 返回迷宫
// - ST_STATISTICS_MAP_SAVE - 地图切换统计
// ============================================================================

// ============================================================================
// 宏定义
// ============================================================================

// SHIWORD 宏 - 提取有符号高字 (对齐 IDA)
#ifndef SHIWORD
#define SHIWORD(x) ((std::int16_t)(((std::uint32_t)(x) >> 16) & 0xFFFF))
#endif

// SWORD2 宏 - 提取有符号低字 (对齐 IDA)
#ifndef SWORD2
#define SWORD2(x) ((std::int16_t)((std::uint32_t)(x) & 0xFFFF))
#endif

// SBYTE3 宏 - 提取有符号字节3 (对齐 IDA, 用于UXMapID.nMapID)
#ifndef SBYTE3
#define SBYTE3(x) ((std::int8_t)(((std::uint32_t)(x) >> 24) & 0xFF))
#endif

// ============================================================================
// 地图/迷宫基础结构体
// ============================================================================

/**
 * @brief 地图/切服目标信息。
 *
 * 该结构直接按 PDB 中 `ST_MAP_INFO` 的字段顺序补回，用作：
 * - `PS_ENTER_MAP_RES`
 * - `ST_CREATE_MAZE`
 */
struct ST_MAP_INFO {
    unsigned int dwUserID = 0;
    unsigned int dwUAID = 0;
    unsigned int dwServerID = 0;
    int nJumpID = 0;
    int nPortalID = 0;
    UXMapID uxMapID{};
    UXMapID uxParentInstanceID{};
    char szIP[513] = {};
    std::int16_t sPort = 0;
    STPosInfo stPosInfo{};
    std::uint8_t byType = 0;
};

/**
 * @brief 进入地图/切服回包。
 */
struct PS_ENTER_MAP_RES : ST_MAP_INFO {
    std::uint8_t byChangeType = 0;
    bool bChangeServer = false;
    int nResult = 0;
    ST_PARTY_INFO stPartyInfo{};
};

/**
 * @brief 控制层创建地图的单个条目。
 */
struct PS_CREATE_MAP {
    UXMapID uxMapID{};
    int nMaxUserCount = 0;
    int nCurUserCount = 0;
    int nResult = 0;
};

/**
 * @brief 控制层创建地图响应结构。
 */
struct PS_CREATE_MAP_RES {
    UXMapID uxMapID{};
    int nResult = 0;
};

/**
 * @brief DB 频道地图信息结构。
 */
struct ST_DB_CHANNEL_MAP {
    UXMapID uxMapID{};
    __int64 sChannel = 0;
    std::uint16_t wTableID = 0;
    std::uint8_t _pad0[2] = {};
    std::uint32_t dwServerID = 0;
};

/**
 * @brief 控制层创建地图的请求列表。
 */
struct PS_CREATE_MAP_LIST {
    std::vector<PS_CREATE_MAP> vecCreateMap;
};

/**
 * @brief 登录服通知控制/Relay 层进入地图的请求结构。
 */
struct PS_ENTER_MAP_REQ {
    unsigned int dwUAID = 0;
    unsigned int dwActorID = 0;
    ST_PARTY_INFO stPartyInfo{};
    std::uint16_t wMapID = 0;
    std::uint16_t wChannel = 0;
    int nJumpID = 0;
    int nPortalID = 0;
    STPosInfo stEnterDistrictPos{};
    UXMapID uxMapID{};
    std::uint8_t byChangeType = 0;
    XVec3 vNextPos{};
};

/**
 * @brief 区域传送请求 (ReqWorldDistrictTransport 上行结构)。
 * PDB UDT 0x481b4, Size 104:
 *   stEnterMap (PS_ENTER_MAP_REQ, +0) / dwNpcID (ulong, +88) /
 *   wTransportID (ushort, +92) / dwTransportItemID (ulong, +96)
 * 反序列化算子定义于本文件尾部 (PS_ENTER_MAP_REQ operator>> 之后)。
 */
struct PS_DISTRICT_TRANSPORT_REQ {
    PS_ENTER_MAP_REQ stEnterMap{};
    std::uint32_t dwNpcID = 0;
    std::uint16_t wTransportID = 0;
    std::uint32_t dwTransportItemID = 0;
    std::uint8_t _pad0[4] = {};
};

/**
 * @brief 迷宫创建请求中的单个进入成员。
 */
struct ST_ENTER_MAZE_MEMBER_INFO {
    unsigned int dwMember = 0;
    int nState = 0;
};

/**
 * @brief 客户端请求登录服继续切服的上行结构。
 */
struct PS_REQ_CHANGE_SERVER {
    unsigned int dwActorID = 0;
    unsigned int dwUAID = 0;
    std::uint8_t byType = 0;
};

/**
 * @brief 控制/Relay 层创建迷宫请求。
 */
struct ST_CREATE_MAZE : ST_MAP_INFO {
    std::uint16_t wReqMapID = 0;
    ST_PARTY_INFO stPartyInfo{};
    UXMapID uxParentMazeID{};
    STPosInfo stEnterDistrictPos{};
    std::vector<ST_ENTER_MAZE_MEMBER_INFO> vecEnterMember;
    int nResult = 0;
    int nCreateType = 0;
};

/**
 * @brief 强制匹配进入请求。
 */
struct PS_SERVER_FORCE_MATCHING_ENTER {
    ST_CREATE_MAZE stCreateMaze{};
    std::vector<PS_SERVER_FORCE_MATCHING_ENTER_MEMBER> vecMember;
    ST_PARTY_INFO stPartyInfo{};
};

/**
 * @brief 强制匹配检查请求。
 */
struct PS_SERVER_FORCE_MATCHING_CHECK {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t byCheck = 0;
    std::uint8_t _pad0[3] = {};
    int nError = 0;
};

/**
 * @brief Relay / ModeMaze 层创建匹配迷宫请求。
 */
struct ST_CREATE_MODE_MAZE : ST_MAP_INFO {
    std::uint16_t wReqMapID = 0;
    std::uint8_t _pad0[6] = {};
    UXMapID uxParentMazeID{};
    std::uint16_t wEnterDistrictID = 0;
    std::uint8_t _pad1[2] = {};
    std::uint32_t dwMatchingID = 0;
    int nModeType = 0;
    int nResult = 0;
    bool bHotTime = false;
    std::uint8_t _pad2[3] = {};
    std::uint32_t dwMasterServerID = 0;
    std::uint32_t dwEventRoomID = 0;
    std::uint8_t _pad3[4] = {};
    std::vector<ST_MODE_MAZE_MEMBER_INFO> vecEnterMember;
};

/**
 * @brief ControlServer 迷宫创建请求中继结构。
 */
struct ST_CREATE_MAZE_FOR_RELAY {
    class CServer* pReqChannelServer = nullptr;
    class CServer* pReqMazeServer = nullptr;
    unsigned long long dwTime = 0;
    ST_PARTY_INFO stPartyInfo{};
    std::vector<ST_ENTER_MAZE_MEMBER_INFO> vecEnterMember;
};

/**
 * @brief ControlServer 模式迷宫创建请求中继结构。
 */
struct ST_CREATE_MODE_MAZE_FOR_RELAY {
    class CServer* pReqChannelServer = nullptr;
    class CServer* pReqMazeServer = nullptr;
    unsigned long long dwTime = 0;
    std::vector<ST_MODE_MAZE_MEMBER_INFO> vecEnterMember;
};

/**
 * @brief 返回迷宫请求结构。
 */
struct ST_GO_BACK_MAZE : ST_MAP_INFO {
    bool bResult = false;
    std::uint8_t _pad0[3] = {};
};

/**
 * @brief 地图切换统计写盘结构。
 */
struct ST_STATISTICS_MAP_SAVE {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwMapID = 0;
    std::uint32_t dwServerID = 0;
};

/**
 * @brief 控制服返回的"继续切服"结果。
 */
struct PS_RES_CHANGE_SERVER {
    std::uint32_t dwActorID = 0;
    std::uint32_t dwUAID = 0;
    char szIP[513] = {};
    char _pad0 = '\0';
    std::int16_t sPort = 0;
    std::uint8_t byType = 0;
    bool bResult = false;
};

/**
 * @brief 单个服务器组上的角色计数信息。
 */
struct ST_USER_CHARACTER_COUNT_FOR_SERVER {
    int nServerGroupID = 0;
    std::uint16_t wCount = 0;
};

/**
 * @brief 服务器组角色计数列表。
 */
struct ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC {
    int nLastServerID = 0;
    std::vector<ST_USER_CHARACTER_COUNT_FOR_SERVER> vecInfo;
};

// ============================================================================
// Static Assertions - 大小和偏移验证
// ============================================================================

static_assert(sizeof(PS_CREATE_MAP) == 0x18, "PS_CREATE_MAP size must match IDA");

// ============================================================================
// 地图/迷宫序列化运算符
// ============================================================================

// ST_DB_CHANNEL_MAP 反序列化
inline void operator>>(XPacket& packet, ST_DB_CHANNEL_MAP& value) {
    packet >> value.uxMapID;
    packet.XParse >> value.sChannel;
    packet.XParse >> value.wTableID;
    // _pad0 跳过 - 仅内存对齐用
    packet.XParse >> value.dwServerID;
}

// PS_CREATE_MAP 反序列化
inline void operator>>(XPacket& packet, PS_CREATE_MAP& value) {
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.nMaxUserCount;
    packet.XParse >> value.nCurUserCount;
    packet.XParse >> value.nResult;
}

// PS_CREATE_MAP_LIST 反序列化
inline void operator>>(XPacket& packet, PS_CREATE_MAP_LIST& value) {
    int nCount = 0;
    packet.XParse >> nCount;
    value.vecCreateMap.clear();
    value.vecCreateMap.reserve(nCount);
    for (int i = 0; i < nCount; ++i) {
        PS_CREATE_MAP map{};
        packet >> map;
        value.vecCreateMap.push_back(map);
    }
}

// ST_ENTER_MAZE_MEMBER_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_ENTER_MAZE_MEMBER_INFO& value) {
    packet.XParse << value.dwMember;
    packet.XParse << value.nState;
    return packet;
}

// ST_ENTER_MAZE_MEMBER_INFO 反序列化
inline void operator>>(XPacket& packet, ST_ENTER_MAZE_MEMBER_INFO& value) {
    packet.XParse >> value.dwMember;
    packet.XParse >> value.nState;
}

// ST_STATISTICS_MAP_SAVE 序列化
inline void operator>>(XPacket& packet, ST_STATISTICS_MAP_SAVE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwMapID;
    packet.XParse >> value.dwServerID;
}

inline XPacket& operator<<(XPacket& packet, const ST_STATISTICS_MAP_SAVE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwMapID;
    packet.XParse << value.dwServerID;
    return packet;
}

// ST_GO_BACK_MAZE 序列化
inline XPacket& operator<<(XPacket& packet, const ST_GO_BACK_MAZE& value) {
    packet.XParse << static_cast<int>(value.dwUserID);
    packet.XParse << static_cast<int>(value.dwUAID);
    packet.XParse << static_cast<int>(value.dwServerID);
    packet.XParse << value.nJumpID;
    packet.XParse << value.nPortalID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.uxParentInstanceID.nMapID;
    packet << value.stPosInfo;
    packet.XParse << static_cast<int>(value.byType);
    packet.XParse << static_cast<int>(value.bResult ? 1 : 0);
    return packet;
}

// ============================================================================
// 地图/迷宫扩展序列化运算符
// ============================================================================

// ST_MAP_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_MAP_INFO& value) {
    packet.XParse << value.dwUserID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwServerID;
    packet.XParse << value.nJumpID;
    packet.XParse << value.nPortalID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.uxParentInstanceID.nMapID;
    packet.XParse << std::string(value.szIP);
    packet.XParse << value.sPort;
    packet << value.stPosInfo;
    packet.XParse << value.byType;
    return packet;
}

// ST_MAP_INFO 反序列化
inline void operator>>(XPacket& packet, ST_MAP_INFO& value) {
    short outLen = 0;
    packet.XParse >> value.dwUserID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwServerID;
    packet.XParse >> value.nJumpID;
    packet.XParse >> value.nPortalID;
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.uxParentInstanceID.nMapID;
    packet.XParse.GetString(value.szIP, 513, &outLen);
    packet.XParse >> value.sPort;
    packet >> value.stPosInfo;
    packet.XParse >> value.byType;
}

// PS_ENTER_MAP_RES 序列化
inline XPacket& operator<<(XPacket& packet, const PS_ENTER_MAP_RES& value) {
    packet << static_cast<const ST_MAP_INFO&>(value);
    packet.XParse << value.bChangeServer;
    packet.XParse << value.byChangeType;
    packet.XParse << value.nResult;
    packet << value.stPartyInfo;
    return packet;
}

// PS_ENTER_MAP_RES 反序列化
inline void operator>>(XPacket& packet, PS_ENTER_MAP_RES& value) {
    packet >> static_cast<ST_MAP_INFO&>(value);
    packet.XParse >> value.bChangeServer;
    packet.XParse >> value.byChangeType;
    packet.XParse >> value.nResult;
    packet >> value.stPartyInfo;
}

// PS_ENTER_MAP_REQ 序列化
inline XPacket& operator<<(XPacket& packet, const PS_ENTER_MAP_REQ& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwActorID;
    packet << value.stPartyInfo;
    packet.XParse << value.wMapID;
    packet.XParse << value.wChannel;
    packet.XParse << value.nJumpID;
    packet.XParse << value.nPortalID;
    packet.XParse << value.uxMapID.nMapID;
    packet << value.stEnterDistrictPos;
    packet.XParse << value.byChangeType;
    packet << value.vNextPos;
    return packet;
}

// PS_ENTER_MAP_REQ 反序列化
inline void operator>>(XPacket& packet, PS_ENTER_MAP_REQ& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwActorID;
    packet >> value.stPartyInfo;
    packet.XParse >> value.wMapID;
    packet.XParse >> value.wChannel;
    packet.XParse >> value.nJumpID;
    packet.XParse >> value.nPortalID;
    packet.XParse >> value.uxMapID.nMapID;
    packet >> value.stEnterDistrictPos;
    packet.XParse >> value.byChangeType;
    packet >> value.vNextPos;
}

// PS_DISTRICT_TRANSPORT_REQ 反序列化 (Per IDA operator>> @ 0x14074C8C0)
inline void operator>>(XPacket& packet, PS_DISTRICT_TRANSPORT_REQ& value) {
    packet >> value.stEnterMap;
    packet.XParse >> value.dwNpcID;
    packet.XParse >> value.wTransportID;
    packet.XParse >> value.dwTransportItemID;
}

// PS_REQ_CHANGE_SERVER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_REQ_CHANGE_SERVER& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.byType;
    return packet;
}

// PS_REQ_CHANGE_SERVER 反序列化
inline void operator>>(XPacket& packet, PS_REQ_CHANGE_SERVER& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.byType;
}

// PS_RES_CHANGE_SERVER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_RES_CHANGE_SERVER& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwUAID;
    packet.XParse << std::string(value.szIP);
    packet.XParse << value.sPort;
    packet.XParse << value.byType;
    packet.XParse << value.bResult;
    return packet;
}

// PS_RES_CHANGE_SERVER 反序列化
inline void operator>>(XPacket& packet, PS_RES_CHANGE_SERVER& value) {
    short outLen = 0;
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwUAID;
    std::memset(value.szIP, 0, sizeof(value.szIP));
    packet.XParse.GetString(value.szIP, sizeof(value.szIP), &outLen);
    packet.XParse >> value.sPort;
    packet.XParse >> value.byType;
    packet.XParse >> value.bResult;
}

// ST_USER_CHARACTER_COUNT_FOR_SERVER 反序列化
inline void operator>>(XPacket& packet, ST_USER_CHARACTER_COUNT_FOR_SERVER& value) {
    packet.XParse >> value.nServerGroupID;
    packet.XParse >> value.wCount;
}

// ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC 反序列化
inline void operator>>(XPacket& packet, ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC& value) {
    std::uint8_t count = 0;
    packet.XParse >> value.nLastServerID;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_USER_CHARACTER_COUNT_FOR_SERVER item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

// ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC 序列化
inline XPacket& operator<<(XPacket& packet, const ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC& value) {
    packet.XParse << value.nLastServerID;
    const std::uint8_t count = static_cast<std::uint8_t>(std::min<std::size_t>(value.vecInfo.size(), 0xFF));
    packet.XParse << count;
    for (std::uint8_t index = 0; index < count; ++index) {
        packet.XParse << value.vecInfo[index].nServerGroupID;
        packet.XParse << value.vecInfo[index].wCount;
    }
    return packet;
}

// ST_CREATE_MAZE 序列化
inline XPacket& operator<<(XPacket& packet, const ST_CREATE_MAZE& value) {
    packet << static_cast<const ST_MAP_INFO&>(value);
    packet.XParse << value.wReqMapID;
    packet << value.stPartyInfo;
    packet << value.stEnterDistrictPos;
    packet.XParse << value.uxParentMazeID.nMapID;
    packet.XParse << value.nResult;
    packet.XParse << value.nCreateType;
    const std::uint8_t memberCount =
        static_cast<std::uint8_t>(std::min<std::size_t>(value.vecEnterMember.size(), 0xFF));
    packet.XParse << memberCount;
    for (std::size_t index = 0; index < value.vecEnterMember.size() && index < 0xFF; ++index) {
        packet << value.vecEnterMember[index];
    }
    return packet;
}

// ST_CREATE_MAZE 反序列化
inline void operator>>(XPacket& packet, ST_CREATE_MAZE& value) {
    packet >> static_cast<ST_MAP_INFO&>(value);
    packet.XParse >> value.wReqMapID;
    packet >> value.stPartyInfo;
    packet >> value.stEnterDistrictPos;
    packet.XParse >> value.uxParentMazeID.nMapID;
    packet.XParse >> value.nResult;
    packet.XParse >> value.nCreateType;
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecEnterMember.clear();
    value.vecEnterMember.reserve(count);
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_ENTER_MAZE_MEMBER_INFO item{};
        packet.XParse >> item.dwMember;
        packet.XParse >> item.nState;
        value.vecEnterMember.push_back(item);
    }
}

// ST_CREATE_MODE_MAZE 序列化
inline XPacket& operator<<(XPacket& packet, const ST_CREATE_MODE_MAZE& value) {
    packet << static_cast<const ST_MAP_INFO&>(value);
    packet.XParse << value.wReqMapID;
    packet.XParse << value.uxParentMazeID.nMapID;
    packet.XParse << value.wEnterDistrictID;
    packet.XParse << value.dwMatchingID;
    packet.XParse << value.nModeType;
    packet.XParse << value.nResult;
    packet.XParse << value.bHotTime;
    packet.XParse << value.dwMasterServerID;
    packet.XParse << value.dwEventRoomID;
    const std::uint8_t memberCount =
        static_cast<std::uint8_t>(std::min<std::size_t>(value.vecEnterMember.size(), 0xFF));
    packet.XParse << memberCount;
    for (std::size_t index = 0; index < value.vecEnterMember.size() && index < 0xFF; ++index) {
        packet << value.vecEnterMember[index];
    }
    return packet;
}

// ST_CREATE_MODE_MAZE 反序列化
inline void operator>>(XPacket& packet, ST_CREATE_MODE_MAZE& value) {
    packet >> static_cast<ST_MAP_INFO&>(value);
    packet.XParse >> value.wReqMapID;
    packet.XParse >> value.uxParentMazeID.nMapID;
    packet.XParse >> value.wEnterDistrictID;
    packet.XParse >> value.dwMatchingID;
    packet.XParse >> value.nModeType;
    packet.XParse >> value.nResult;
    packet.XParse >> value.bHotTime;
    packet.XParse >> value.dwMasterServerID;
    packet.XParse >> value.dwEventRoomID;
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecEnterMember.clear();
    value.vecEnterMember.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_MODE_MAZE_MEMBER_INFO item{};
        packet >> item;
        value.vecEnterMember.push_back(item);
    }
}

// ============================================================================
// 强制匹配序列化运算符
// ============================================================================

// PS_SERVER_FORCE_MATCHING_ENTER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_FORCE_MATCHING_ENTER& value) {
    packet << value.stCreateMaze;
    const std::uint8_t memberCount =
        static_cast<std::uint8_t>(std::min<std::size_t>(value.vecMember.size(), 0xFF));
    packet.XParse << memberCount;
    for (std::size_t index = 0; index < value.vecMember.size() && index < 0xFF; ++index) {
        packet << value.vecMember[index];
    }
    packet << value.stPartyInfo;
    return packet;
}

// PS_SERVER_FORCE_MATCHING_ENTER 反序列化
inline void operator>>(XPacket& packet, PS_SERVER_FORCE_MATCHING_ENTER& value) {
    packet >> value.stCreateMaze;
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecMember.clear();
    value.vecMember.reserve(count);
    for (std::uint8_t index = 0; index < count; ++index) {
        PS_SERVER_FORCE_MATCHING_ENTER_MEMBER item{};
        packet >> item;
        value.vecMember.push_back(item);
    }
    packet >> value.stPartyInfo;
}

// PS_SERVER_FORCE_MATCHING_CHECK 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_FORCE_MATCHING_CHECK& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.byCheck;
    packet.XParse << value.nError;
    return packet;
}

// PS_SERVER_FORCE_MATCHING_CHECK 反序列化
inline void operator>>(XPacket& packet, PS_SERVER_FORCE_MATCHING_CHECK& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byCheck;
    packet.XParse >> value.nError;
}
// ============================================================================
// static_assert 验证（确保结构体大小与 PDB 匹配）
// ============================================================================

static_assert(sizeof(PS_SERVER_FORCE_MATCHING_ENTER) == 0x2E0,
              "PS_SERVER_FORCE_MATCHING_ENTER size must match PDB");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_ENTER, vecMember) == 0x2B8,
              "PS_SERVER_FORCE_MATCHING_ENTER.vecMember offset mismatch");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_ENTER, stPartyInfo) == 0x2D8,
              "PS_SERVER_FORCE_MATCHING_ENTER.stPartyInfo offset mismatch");
static_assert(sizeof(PS_SERVER_FORCE_MATCHING_CHECK) == 0x10,
              "PS_SERVER_FORCE_MATCHING_CHECK size must match PDB");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_CHECK, byCheck) == 0x8,
              "PS_SERVER_FORCE_MATCHING_CHECK.byCheck offset mismatch");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_CHECK, nError) == 0xC,
              "PS_SERVER_FORCE_MATCHING_CHECK.nError offset mismatch");
static_assert(sizeof(ST_MAP_INFO) == 0x258, "ST_MAP_INFO size must match PDB");
static_assert(offsetof(ST_MAP_INFO, uxMapID) == 0x18, "ST_MAP_INFO.uxMapID offset mismatch");
static_assert(offsetof(ST_MAP_INFO, uxParentInstanceID) == 0x20,
              "ST_MAP_INFO.uxParentInstanceID offset mismatch");
static_assert(offsetof(ST_MAP_INFO, sPort) == 0x22A, "ST_MAP_INFO.sPort offset mismatch");
static_assert(offsetof(ST_MAP_INFO, stPosInfo) == 0x230, "ST_MAP_INFO.stPosInfo offset mismatch");
static_assert(offsetof(ST_MAP_INFO, byType) == 0x250, "ST_MAP_INFO.byType offset mismatch");
static_assert(sizeof(PS_ENTER_MAP_RES) == 0x268, "PS_ENTER_MAP_RES size must match PDB");
static_assert(offsetof(PS_ENTER_MAP_RES, byChangeType) == 0x258,
              "PS_ENTER_MAP_RES.byChangeType offset mismatch");
static_assert(offsetof(PS_ENTER_MAP_RES, bChangeServer) == 0x259,
              "PS_ENTER_MAP_RES.bChangeServer offset mismatch");
static_assert(offsetof(PS_ENTER_MAP_RES, nResult) == 0x25C,
              "PS_ENTER_MAP_RES.nResult offset mismatch");
static_assert(offsetof(PS_ENTER_MAP_RES, stPartyInfo) == 0x260,
              "PS_ENTER_MAP_RES.stPartyInfo offset mismatch");
static_assert(sizeof(PS_ENTER_MAP_REQ) == 0x58, "PS_ENTER_MAP_REQ size must match PDB");
static_assert(offsetof(PS_ENTER_MAP_REQ, stEnterDistrictPos) == 0x20,
              "PS_ENTER_MAP_REQ.stEnterDistrictPos offset mismatch");
static_assert(offsetof(PS_ENTER_MAP_REQ, uxMapID) == 0x40,
              "PS_ENTER_MAP_REQ.uxMapID offset mismatch");
static_assert(offsetof(PS_ENTER_MAP_REQ, byChangeType) == 0x48,
              "PS_ENTER_MAP_REQ.byChangeType offset mismatch");
static_assert(sizeof(PS_DISTRICT_TRANSPORT_REQ) == 104,
              "PS_DISTRICT_TRANSPORT_REQ size must match PDB UDT 0x481b4");
static_assert(offsetof(PS_DISTRICT_TRANSPORT_REQ, dwNpcID) == 88,
              "PS_DISTRICT_TRANSPORT_REQ.dwNpcID offset mismatch");
static_assert(offsetof(PS_DISTRICT_TRANSPORT_REQ, wTransportID) == 92,
              "PS_DISTRICT_TRANSPORT_REQ.wTransportID offset mismatch");
static_assert(offsetof(PS_DISTRICT_TRANSPORT_REQ, dwTransportItemID) == 96,
              "PS_DISTRICT_TRANSPORT_REQ.dwTransportItemID offset mismatch");
static_assert(sizeof(ST_CREATE_MAZE) == 0x2B8, "ST_CREATE_MAZE size must match PDB");
static_assert(offsetof(ST_CREATE_MAZE, stPartyInfo) == 0x25C,
              "ST_CREATE_MAZE.stPartyInfo offset mismatch");
static_assert(offsetof(ST_CREATE_MAZE, uxParentMazeID) == 0x268,
              "ST_CREATE_MAZE.uxParentMazeID offset mismatch");
static_assert(offsetof(ST_CREATE_MAZE, stEnterDistrictPos) == 0x270,
              "ST_CREATE_MAZE.stEnterDistrictPos offset mismatch");
static_assert(offsetof(ST_CREATE_MAZE, vecEnterMember) == 0x290,
              "ST_CREATE_MAZE.vecEnterMember offset mismatch");
static_assert(offsetof(ST_CREATE_MAZE, nResult) == 0x2B0,
              "ST_CREATE_MAZE.nResult offset mismatch");
static_assert(offsetof(ST_CREATE_MAZE, nCreateType) == 0x2B4,
              "ST_CREATE_MAZE.nCreateType offset mismatch");
static_assert(sizeof(PS_SERVER_FORCE_MATCHING_ENTER) == 0x2E0,
              "PS_SERVER_FORCE_MATCHING_ENTER size must match PDB");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_ENTER, vecMember) == 0x2B8,
              "PS_SERVER_FORCE_MATCHING_ENTER.vecMember offset mismatch");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_ENTER, stPartyInfo) == 0x2D8,
              "PS_SERVER_FORCE_MATCHING_ENTER.stPartyInfo offset mismatch");
static_assert(sizeof(PS_SERVER_FORCE_MATCHING_CHECK) == 0x10,
              "PS_SERVER_FORCE_MATCHING_CHECK size must match PDB");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_CHECK, byCheck) == 0x8,
              "PS_SERVER_FORCE_MATCHING_CHECK.byCheck offset mismatch");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_CHECK, nError) == 0xC,
              "PS_SERVER_FORCE_MATCHING_CHECK.nError offset mismatch");
static_assert(sizeof(ST_CREATE_MODE_MAZE) == 0x2A8, "ST_CREATE_MODE_MAZE size must match PDB");
static_assert(offsetof(ST_CREATE_MODE_MAZE, wReqMapID) == 0x258,
              "ST_CREATE_MODE_MAZE.wReqMapID offset mismatch");
static_assert(offsetof(ST_CREATE_MODE_MAZE, uxParentMazeID) == 0x260,
              "ST_CREATE_MODE_MAZE.uxParentMazeID offset mismatch");
static_assert(offsetof(ST_CREATE_MODE_MAZE, wEnterDistrictID) == 0x268,
              "ST_CREATE_MODE_MAZE.wEnterDistrictID offset mismatch");
static_assert(offsetof(ST_CREATE_MODE_MAZE, dwMatchingID) == 0x26C,
              "ST_CREATE_MODE_MAZE.dwMatchingID offset mismatch");
static_assert(offsetof(ST_CREATE_MODE_MAZE, nModeType) == 0x270,
              "ST_CREATE_MODE_MAZE.nModeType offset mismatch");
static_assert(offsetof(ST_CREATE_MODE_MAZE, nResult) == 0x274,
              "ST_CREATE_MODE_MAZE.nResult offset mismatch");
static_assert(offsetof(ST_CREATE_MODE_MAZE, bHotTime) == 0x278,
              "ST_CREATE_MODE_MAZE.bHotTime offset mismatch");
static_assert(offsetof(ST_CREATE_MODE_MAZE, dwMasterServerID) == 0x27C,
              "ST_CREATE_MODE_MAZE.dwMasterServerID offset mismatch");
static_assert(offsetof(ST_CREATE_MODE_MAZE, dwEventRoomID) == 0x280,
              "ST_CREATE_MODE_MAZE.dwEventRoomID offset mismatch");
static_assert(offsetof(ST_CREATE_MODE_MAZE, vecEnterMember) == 0x288,
              "ST_CREATE_MODE_MAZE.vecEnterMember offset mismatch");
static_assert(sizeof(PS_REQ_CHANGE_SERVER) == 0x0C, "PS_REQ_CHANGE_SERVER size must match PDB");
static_assert(offsetof(PS_REQ_CHANGE_SERVER, dwUAID) == 0x4,
              "PS_REQ_CHANGE_SERVER.dwUAID offset mismatch");
static_assert(offsetof(PS_REQ_CHANGE_SERVER, byType) == 0x8,
              "PS_REQ_CHANGE_SERVER.byType offset mismatch");
static_assert(sizeof(ST_STATISTICS_MAP_SAVE) == 0x0C, "ST_STATISTICS_MAP_SAVE size must match PDB");
static_assert(sizeof(PS_RES_CHANGE_SERVER) == 0x210, "PS_RES_CHANGE_SERVER size must match PDB");
static_assert(offsetof(PS_RES_CHANGE_SERVER, dwUAID) == 0x4,
              "PS_RES_CHANGE_SERVER.dwUAID offset mismatch");
static_assert(offsetof(PS_RES_CHANGE_SERVER, szIP) == 0x8,
              "PS_RES_CHANGE_SERVER.szIP offset mismatch");
static_assert(offsetof(PS_RES_CHANGE_SERVER, sPort) == 0x20A,
              "PS_RES_CHANGE_SERVER.sPort offset mismatch");
static_assert(offsetof(PS_RES_CHANGE_SERVER, byType) == 0x20C,
              "PS_RES_CHANGE_SERVER.byType offset mismatch");
static_assert(offsetof(PS_RES_CHANGE_SERVER, bResult) == 0x20D,
              "PS_RES_CHANGE_SERVER.bResult offset mismatch");
static_assert(sizeof(ST_USER_CHARACTER_COUNT_FOR_SERVER) == 0x08,
              "ST_USER_CHARACTER_COUNT_FOR_SERVER size must match PDB");
static_assert(offsetof(ST_USER_CHARACTER_COUNT_FOR_SERVER, wCount) == 0x4,
              "ST_USER_CHARACTER_COUNT_FOR_SERVER.wCount offset mismatch");
static_assert(sizeof(ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC) == 0x28,
              "ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC size must match PDB");
static_assert(offsetof(ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC, vecInfo) == 0x8,
              "ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC.vecInfo offset mismatch");

// ============================================================================
// PS_RES_REVIVE - 复活响应广播 (main=3, sub=0x44)
// PDB: UDT 0x6e248, size 28 bytes
// ============================================================================
struct PS_RES_REVIVE {
    std::uint32_t dwActorID = 0;   // offset 0: 复活者 ActorID
    std::uint32_t dwOwnerID = 0;   // offset 4: 复活来源 OwnerID
    std::uint8_t byType = 0;       // offset 8: 复活类型
    XVec3 vPos{};                  // offset 12: 复活位置
    float fRot = 0.0f;             // offset 24: 朝向
};

static_assert(sizeof(PS_RES_REVIVE) == 0x1C, "PS_RES_REVIVE size must match PDB");
static_assert(offsetof(PS_RES_REVIVE, dwActorID) == 0x0, "PS_RES_REVIVE.dwActorID offset mismatch");
static_assert(offsetof(PS_RES_REVIVE, dwOwnerID) == 0x4, "PS_RES_REVIVE.dwOwnerID offset mismatch");
static_assert(offsetof(PS_RES_REVIVE, byType) == 0x8, "PS_RES_REVIVE.byType offset mismatch");
static_assert(offsetof(PS_RES_REVIVE, vPos) == 0xC, "PS_RES_REVIVE.vPos offset mismatch");
static_assert(offsetof(PS_RES_REVIVE, fRot) == 0x18, "PS_RES_REVIVE.fRot offset mismatch");

inline XPacket& operator<<(XPacket& packet, const PS_RES_REVIVE& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwOwnerID;
    packet.XParse << value.byType;
    packet << value.vPos;
    packet.XParse << value.fRot;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_RES_REVIVE& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwOwnerID;
    packet.XParse >> value.byType;
    packet >> value.vPos;
    packet.XParse >> value.fRot;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_RES_REVIVE& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwOwnerID;
    packet.XParse << value.byType;
    packet << value.vPos;
    packet.XParse << value.fRot;
    return packet;
}
