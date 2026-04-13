#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

/**
 * @brief 服务器内容开关 ID。
 *
 * PDB 中该枚举名为 `CONTENTS_INFO`。当前仅登录服骨架已经直接用到的枚举值
 * 会参与实际逻辑判断，但其余枚举名也一并保留，避免后续继续还原时再次退回裸数。
 */
enum SERVER_OPTION {
    E_SERVER_OPTION_ATTENDANCE = 0,
    E_SERVER_OPTION_SECOND_PW = 1,
    E_SERVER_OPTION_PVP_DISTRICT = 2,
    E_SERVER_OPTION_RANKING = 3,
    E_SERVER_OPTION_CASHSHOP = 4,
    E_SERVER_OPTION_D6_MODE = 5,
    E_SERVER_OPTION_BROACH_EVENT = 6,
    E_SERVER_OPTION_OVER_INDULGENCE = 7,
    E_SERVER_OPTION_SOUL_WEEKLY_MISSION = 8,
    E_SERVER_OPTION_NETCAFE = 9,
    E_SERVER_OPTION_SOUL_EVENT = 10,
    E_SERVER_OPTION_ITEM_EXCHANGE = 11,
    E_SERVER_OPTION_WAIT_SYSTEM = 12,
    E_SERVER_OPTION_OPERATION_MAZE = 13,
    E_SERVER_OPTION_MAX = 14,
};

/**
 * @brief 切服/进图类型。
 *
 * 名称直接沿用 `rabin2-LoginServer.pdb.txt` 中的 `CHANGE_SERVER_TYPE`，
 * 当前只把已经在登录服骨架里出现到的枚举值落进工程。
 */
enum CHANGE_SERVER_TYPE : std::uint8_t {
    CHANGE_SERVER_TYPE_ENTER_MAP = 0,
    CHANGE_SERVER_TYPE_CHANGE_CHANNEL = 1,
    CHANGE_SERVER_TYPE_EXIT_MAZE = 2,
    CHANGE_SERVER_TYPE_ENTER_MAZE_FROM_MAZE = 3,
    CHANGE_SERVER_TYPE_ENTER_DISTRICT = 4,
    CHANGE_SERVER_TYPE_ENTER_DISTRICT6 = 5,
    CHANGE_SERVER_TYPE_LOGIN = 6,
};

/**
 * @brief 队伍/组队进入地图时附带的组信息。
 */
struct ST_PARTY_INFO {
    std::uint8_t byGroupType = 0;
    int nID = 0;
};

/**
 * @brief 客户端切入 GameServer 时携带的最小进入信息。
 *
 * 这一轮直接按 `GameServer::operator<<(ST_ENTER_SERVER)` /
 * `operator>>(ST_ENTER_SERVER)` 与 PDB 成员偏移补回：
 * - `dwUAID`
 * - `dwActorID`
 * - `uxMapID`
 * - `bFirstConnect`
 * - `biAuthSessionID`
 */
struct ST_ENTER_SERVER {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwActorID = 0;
    UXMapID uxMapID{};
    bool bFirstConnect = false;
    std::uint8_t _pad0[7] = {};
    std::int64_t biAuthSessionID = 0;
};

/**
 * @brief GameServer 批量同步在线角色给控制/Relay 的完整用户信息体。
 *
 * 注意：它与 `LoginServer::ResSelectCharacter` 里真实发送的 `0xF3/1`
 * 五段短包不是同一个 payload。
 */
struct PS_USER_INFO_FOR_RELAY {
    STCharInfo stCharInfo{};
    UXMapID uxMapID{};
    std::uint32_t dwIP = 0;
    std::uint8_t byTradePW = 0;
    std::uint8_t _pad0[3] = {};
    std::int64_t biAuthSessionID = 0;
    ST_GAME_OPTION stGameOption{};
    std::uint8_t byAuthType = 0;
    std::uint8_t _pad1[3] = {};
};

/**
 * @brief 地图/切服目标信息。
 *
 * 该结构直接按 PDB 中 `ST_MAP_INFO` 的字段顺序补回，用作：
 * - `PS_ENTER_MAP_RES`
 * - `ST_CREATE_MAZE`
 *
 * 这样可以避免此前“只保留局部字段”的最小骨架继续漂移。
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
 *
 * PDB 中该结构继承自 `ST_MAP_INFO`，这一轮在骨架里同步恢复该继承关系。
 */
struct PS_ENTER_MAP_RES : ST_MAP_INFO {
    std::uint8_t byChangeType = 0;
    bool bChangeServer = false;
    int nResult = 0;
    ST_PARTY_INFO stPartyInfo{};
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
 *
 * PDB 中该结构继承自 `ST_MAP_INFO`，这一轮同步恢复父子关系和字段顺序。
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
 * @brief 地图切换统计写盘结构。
 *
 * `RecvCreateMazeRes / RecvEnterServer` 都会把它发往 StatisticsDB `main=0xF0, sub=0x12`。
 */
struct ST_STATISTICS_MAP_SAVE {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwMapID = 0;
    std::uint32_t dwServerID = 0;
};

/**
 * @brief 控制服返回的“继续切服”结果。
 *
 * PDB 中尺寸为 `0x210`，当前按 `dwActorID / dwUAID / szIP / sPort / byType / bResult`
 * 的已证实字段顺序恢复。
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

static_assert(sizeof(ST_SERVER_GROUP_INFO) == 0x224, "ST_SERVER_GROUP_INFO size must match PDB");
static_assert(offsetof(ST_SERVER_GROUP_INFO, szPublicIP) == 0x19, "ST_SERVER_GROUP_INFO.szPublicIP offset mismatch");
static_assert(offsetof(ST_SERVER_GROUP_INFO, nState) == 0x21C, "ST_SERVER_GROUP_INFO.nState offset mismatch");
static_assert(sizeof(SS_SERVER_INFO) == 0x440, "SS_SERVER_INFO size must match PDB");
static_assert(offsetof(SS_SERVER_INFO, szName) == 0x20, "SS_SERVER_INFO.szName offset mismatch");
static_assert(offsetof(SS_SERVER_INFO, szPrivateIP) == 0x35, "SS_SERVER_INFO.szPrivateIP offset mismatch");
static_assert(offsetof(SS_SERVER_INFO, szPublicIP) == 0x236, "SS_SERVER_INFO.szPublicIP offset mismatch");
static_assert(offsetof(SS_SERVER_INFO, nMazeCount) == 0x438, "SS_SERVER_INFO.nMazeCount offset mismatch");
static_assert(sizeof(ST_PARTY_INFO) == 0x08, "ST_PARTY_INFO size must match PDB");
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

/**
 * @brief 发给客户端的服务器信息，包含该组角色数。
 */
struct ST_SERVER_INFO_FOR_USER : ST_SERVER_GROUP_INFO {
    std::uint8_t byCharacterCount = 0;
};

static_assert(sizeof(ST_SERVER_INFO_FOR_USER) == 0x228, "ST_SERVER_INFO_FOR_USER size must match PDB");
static_assert(offsetof(ST_SERVER_INFO_FOR_USER, byCharacterCount) == 0x224,
              "ST_SERVER_INFO_FOR_USER.byCharacterCount offset mismatch");
static_assert(sizeof(ST_ENTER_SERVER) == 0x20, "ST_ENTER_SERVER size must match PDB");
static_assert(offsetof(ST_ENTER_SERVER, bFirstConnect) == 0x10,
              "ST_ENTER_SERVER.bFirstConnect offset mismatch");
static_assert(offsetof(ST_ENTER_SERVER, biAuthSessionID) == 0x18,
              "ST_ENTER_SERVER.biAuthSessionID offset mismatch");
static_assert(sizeof(PS_USER_INFO_FOR_RELAY) == 0x318, "PS_USER_INFO_FOR_RELAY size must match PDB");
static_assert(offsetof(PS_USER_INFO_FOR_RELAY, uxMapID) == 0x2F0,
              "PS_USER_INFO_FOR_RELAY.uxMapID offset mismatch");
static_assert(offsetof(PS_USER_INFO_FOR_RELAY, biAuthSessionID) == 0x300,
              "PS_USER_INFO_FOR_RELAY.biAuthSessionID offset mismatch");
static_assert(offsetof(PS_USER_INFO_FOR_RELAY, stGameOption) == 0x308,
              "PS_USER_INFO_FOR_RELAY.stGameOption offset mismatch");
static_assert(offsetof(PS_USER_INFO_FOR_RELAY, byAuthType) == 0x314,
              "PS_USER_INFO_FOR_RELAY.byAuthType offset mismatch");
static_assert(sizeof(PS_REQ_CHANGE_SERVER) == 0x0C, "PS_REQ_CHANGE_SERVER size must match PDB");
static_assert(offsetof(PS_REQ_CHANGE_SERVER, dwUAID) == 0x4,
              "PS_REQ_CHANGE_SERVER.dwUAID offset mismatch");
static_assert(offsetof(PS_REQ_CHANGE_SERVER, byType) == 0x8,
              "PS_REQ_CHANGE_SERVER.byType offset mismatch");
#ifdef _WIN32
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
#endif
static_assert(sizeof(ST_USER_CHARACTER_COUNT_FOR_SERVER) == 0x08,
              "ST_USER_CHARACTER_COUNT_FOR_SERVER size must match PDB");
static_assert(offsetof(ST_USER_CHARACTER_COUNT_FOR_SERVER, wCount) == 0x4,
              "ST_USER_CHARACTER_COUNT_FOR_SERVER.wCount offset mismatch");
static_assert(sizeof(ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC) == 0x28,
              "ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC size must match PDB");
static_assert(offsetof(ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC, vecInfo) == 0x8,
              "ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC.vecInfo offset mismatch");

/**
 * @brief 发给客户端的服务器列表包体。
 */
struct PS_SERVERS_INFO_FOR_USER {
    std::uint8_t byLastServerID = 0;
    std::vector<ST_SERVER_INFO_FOR_USER> vecServerInfo;
};

static_assert(sizeof(PS_SERVERS_INFO_FOR_USER) == 0x28, "PS_SERVERS_INFO_FOR_USER size must match PDB");
static_assert(offsetof(PS_SERVERS_INFO_FOR_USER, vecServerInfo) == 0x8,
              "PS_SERVERS_INFO_FOR_USER.vecServerInfo offset mismatch");

inline XPacket& operator<<(XPacket& packet, const ST_PARTY_INFO& value) {
    packet.XParse << value.byGroupType;
    packet.XParse << value.nID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_INFO& value) {
    packet.XParse >> value.byGroupType;
    packet.XParse >> value.nID;
}

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

inline XPacket& operator<<(XPacket& packet, const PS_ENTER_MAP_RES& value) {
    packet << static_cast<const ST_MAP_INFO&>(value);
    packet.XParse << value.bChangeServer;
    packet.XParse << value.byChangeType;
    packet.XParse << value.nResult;
    packet << value.stPartyInfo;
    return packet;
}

inline void operator>>(XPacket& packet, PS_ENTER_MAP_RES& value) {
    packet >> static_cast<ST_MAP_INFO&>(value);
    packet.XParse >> value.bChangeServer;
    packet.XParse >> value.byChangeType;
    packet.XParse >> value.nResult;
    packet >> value.stPartyInfo;
}

inline XPacket& operator<<(XPacket& packet, const ST_ENTER_MAZE_MEMBER_INFO& value) {
    packet.XParse << value.dwMember;
    packet.XParse << value.nState;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_STATISTICS_MAP_SAVE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwMapID;
    packet.XParse << value.dwServerID;
    return packet;
}

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

inline XPacket& operator<<(XPacket& packet, const PS_REQ_CHANGE_SERVER& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.byType;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_CHANGE_SERVER& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.byType;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_CHANGE_SERVER& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwUAID;
    packet.XParse << std::string(value.szIP);
    packet.XParse << value.sPort;
    packet.XParse << value.byType;
    packet.XParse << value.bResult;
    return packet;
}

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

inline void operator>>(XPacket& packet, ST_USER_CHARACTER_COUNT_FOR_SERVER& value) {
    packet.XParse >> value.nServerGroupID;
    packet.XParse >> value.wCount;
}

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

inline XPacket& operator<<(XPacket& packet, const ST_SERVER_GROUP_INFO& value) {
    packet.XParse << value.wID;
    packet.XParse << value.sPort;
    packet.XParse << std::string(value.szName);
    packet.XParse << std::string(value.szPublicIP);
    packet.XParse << value.nState;
    packet.XParse << value.nUserCount;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_SERVER_INFO_FOR_USER& value) {
    packet << static_cast<const ST_SERVER_GROUP_INFO&>(value);
    packet.XParse << value.byCharacterCount;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_SERVERS_INFO_FOR_USER& value) {
    const std::uint8_t serverCount =
        static_cast<std::uint8_t>(std::min<std::size_t>(value.vecServerInfo.size(), 0xFF));
    packet.XParse << value.byLastServerID;
    packet.XParse << serverCount;
    for (std::size_t index = 0; index < serverCount; ++index) {
        packet << value.vecServerInfo[index];
    }
    return packet;
}

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
