#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerUser.h - 用户进入服务器相关结构体及序列化运算符
//
// 此文件包含用户进入服务器的所有数据结构和序列化操作：
// - ST_ENTER_SERVER - 进入服务器基础信息
// - PS_USER_INFO_FOR_RELAY - Relay同步用户信息
// - PS_USERS_INFO - 批量用户信息
// - PS_UPDATE_USER_MAP_INFO - 用户地图变化更新
// - ST_OTHER_CHARINFO - 其他角色信息
// - PS_DB_CHARACTER_INFO_OTHER_RES - 其他角色查询响应
// ============================================================================

// ============================================================================
// 用户进入服务器结构体
// ============================================================================

/**
 * @brief 进入服务器请求结构。
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
 * @brief Relay 同步单个在线角色信息的批量包。
 */
struct PS_USERS_INFO {
    bool bFinish = false;
    std::int32_t nTotalCount = 0;
    std::vector<PS_USER_INFO_FOR_RELAY> vecUserInfo;
};

/**
 * @brief Relay 同步单个在线角色地图变化。
 */
struct PS_UPDATE_USER_MAP_INFO {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwActorID = 0;
    UXMapID uxMapID{};
    ST_PARTY_INFO stPartyInfo{};
    std::int64_t biAuthSessionID = 0;
    bool bLeaveParty = false;
};

/**
 * @brief 其他角色信息结构 - 472 bytes
 * 来自 IDA: ST_OTHER_CHARINFO
 * 用于查询其他角色详细信息时的返回数据
 */
struct ST_OTHER_CHARINFO {
    std::int16_t shLevel = 0;
    std::uint8_t byState = 0;
    std::uint8_t _pad0 = 0;
    wchar_t szComment[51] = {};
    ST_STAT_VEC m_vecBaseStat{};
    ST_STAT_VEC m_vecStat{};
    PS_RES_STORAGE_INFO m_vecEquipItem{};
    std::uint8_t byEchelonLevel = 0;
    std::uint8_t _pad1[3] = {};
    std::int32_t nEchelonExp = 0;
    wchar_t szMemo[31] = {};
    std::int32_t nEquipMemorySlot = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad2[2] = {};
    std::uint32_t dwProfilePhotoID = 0;
    wchar_t szName[21] = {};
    ST_TitleInfo stInsideTitle{};
    ST_TitleInfo stOutsideTitle{};
    PS_ITEM_SOCKET_LIST vecSocketList{};
    PS_ITEM_BROACH_LIST vecBroachList{};
    UAppearanceEx uAppearance{};
    UAppearanceEx uAppearanceEx{};
    ST_UPDATE_SPECIAL_OPTION_LIST psOptionList{};
};

/**
 * @brief DB层角色详情查询响应 - 576 bytes
 * 来自 IDA: PS_DB_CHARACTER_INFO_OTHER_RES
 * 用于 SP_CHARACTER_DETAIL_INFO / SP_CHARACTER_DETAIL_ITEM_BROACH / SP_CHARACTER_DETAIL_ITEM_SOCKET
 */
struct PS_DB_CHARACTER_INFO_OTHER_RES {
    std::int32_t nError = 0;
    bool bCalculateStat = false;
    std::uint8_t _pad0[3] = {};
    ST_OTHER_CHARINFO stTargetInfo{};
    ST_ACHIEVE_CATEGORY stCatrgory{};
    std::uint8_t _pad1[2] = {};
    PS_QUICKSLOT_CARD psCard{};
    std::vector<std::int32_t> vecSkill;
};

// ============================================================================
// Static Assertions - 大小和偏移验证
// ============================================================================

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
static_assert(sizeof(PS_USERS_INFO) == 0x28, "PS_USERS_INFO size must match PDB");
static_assert(offsetof(PS_USERS_INFO, nTotalCount) == 0x4, "PS_USERS_INFO.nTotalCount offset mismatch");
static_assert(offsetof(PS_USERS_INFO, vecUserInfo) == 0x8, "PS_USERS_INFO.vecUserInfo offset mismatch");
static_assert(sizeof(PS_UPDATE_USER_MAP_INFO) == 0x28, "PS_UPDATE_USER_MAP_INFO size must match PDB");
static_assert(offsetof(PS_UPDATE_USER_MAP_INFO, dwActorID) == 0x4,
              "PS_UPDATE_USER_MAP_INFO.dwActorID offset mismatch");
static_assert(offsetof(PS_UPDATE_USER_MAP_INFO, uxMapID) == 0x8,
              "PS_UPDATE_USER_MAP_INFO.uxMapID offset mismatch");
static_assert(offsetof(PS_UPDATE_USER_MAP_INFO, stPartyInfo) == 0x10,
              "PS_UPDATE_USER_MAP_INFO.stPartyInfo offset mismatch");
static_assert(offsetof(PS_UPDATE_USER_MAP_INFO, biAuthSessionID) == 0x18,
              "PS_UPDATE_USER_MAP_INFO.biAuthSessionID offset mismatch");
static_assert(offsetof(PS_UPDATE_USER_MAP_INFO, bLeaveParty) == 0x20,
              "PS_UPDATE_USER_MAP_INFO.bLeaveParty offset mismatch");

// ============================================================================
// 用户进入服务器序列化运算符
// ============================================================================

// ST_ENTER_SERVER 序列化
inline XPacket& operator<<(XPacket& packet, const ST_ENTER_SERVER& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwActorID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << static_cast<int>(value.bFirstConnect ? 1 : 0);
    packet.XParse << value.biAuthSessionID;
    return packet;
}

// ST_ENTER_SERVER 反序列化
inline void operator>>(XPacket& packet, ST_ENTER_SERVER& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.uxMapID.nMapID;
    int nFirstConnect = 0;
    packet.XParse >> nFirstConnect;
    value.bFirstConnect = (nFirstConnect != 0);
    packet.XParse >> value.biAuthSessionID;
}

// PS_USER_INFO_FOR_RELAY 序列化
inline XPacket& operator<<(XPacket& packet, const PS_USER_INFO_FOR_RELAY& value) {
    packet << value.stCharInfo;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.dwIP;
    packet.XParse << value.byTradePW;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.biAuthSessionID;
    packet << value.stGameOption;
    packet.XParse << value.byAuthType;
    return packet;
}

// PS_USER_INFO_FOR_RELAY 反序列化
inline void operator>>(XPacket& packet, PS_USER_INFO_FOR_RELAY& value) {
    packet >> value.stCharInfo;
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.dwIP;
    packet.XParse >> value.byTradePW;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse >> value.biAuthSessionID;
    packet >> value.stGameOption;
    packet.XParse >> value.byAuthType;
}

// PS_USERS_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_USERS_INFO& value) {
    packet.XParse << static_cast<int>(value.bFinish ? 1 : 0);
    packet.XParse << value.nTotalCount;
    packet.XParse << static_cast<std::uint16_t>(value.vecUserInfo.size());
    for (const auto& user : value.vecUserInfo) {
        packet << user;
    }
    return packet;
}

// PS_USERS_INFO 反序列化
inline void operator>>(XPacket& packet, PS_USERS_INFO& value) {
    int nFinish = 0;
    packet.XParse >> nFinish;
    value.bFinish = (nFinish != 0);
    packet.XParse >> value.nTotalCount;
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecUserInfo.clear();
    value.vecUserInfo.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        PS_USER_INFO_FOR_RELAY user{};
        packet >> user;
        value.vecUserInfo.push_back(user);
    }
}

// PS_UPDATE_USER_MAP_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_UPDATE_USER_MAP_INFO& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwActorID;
    packet.XParse << value.uxMapID.nMapID;
    packet << value.stPartyInfo;
    packet.XParse << value.biAuthSessionID;
    packet.XParse << static_cast<int>(value.bLeaveParty ? 1 : 0);
    return packet;
}

// PS_UPDATE_USER_MAP_INFO 反序列化
inline void operator>>(XPacket& packet, PS_UPDATE_USER_MAP_INFO& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.uxMapID.nMapID;
    packet >> value.stPartyInfo;
    packet.XParse >> value.biAuthSessionID;
    int nLeaveParty = 0;
    packet.XParse >> nLeaveParty;
    value.bLeaveParty = (nLeaveParty != 0);
}

// 注意: PS_DB_CHARACTER_INFO_OTHER_REQ 和 PS_DB_CHARACTER_INFO_OTHER_RES 的序列化运算符
// 定义在 PSServerFriend.h 中，因为这两个结构体与好友查询相关

// ============================================================================
// Static Assertions - ST_OTHER_CHARINFO 大小验证
// ============================================================================

static_assert(sizeof(ST_OTHER_CHARINFO) == 472, "ST_OTHER_CHARINFO size must match IDA");
static_assert(sizeof(PS_DB_CHARACTER_INFO_OTHER_RES) == 576, "PS_DB_CHARACTER_INFO_OTHER_RES size must match IDA");

// ============================================================================
// ST_OTHER_CHARINFO XSendDBPacket 序列化运算符
// ============================================================================

// ST_OTHER_CHARINFO XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_OTHER_CHARINFO& value) {
    packet.XParse << value.shLevel;
    packet.XParse << value.byState;
    packet.XParse << FixedWideArrayToWString(value.szComment);
    packet << value.m_vecBaseStat;
    packet << value.m_vecStat;
    packet << value.m_vecEquipItem;
    packet.XParse << value.byEchelonLevel;
    packet.XParse << value.nEchelonExp;
    packet.XParse << FixedWideArrayToWString(value.szMemo);
    packet.XParse << value.nEquipMemorySlot;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << FixedWideArrayToWString(value.szName);
    packet.XParse << value.stInsideTitle.dwPrefix;
    packet.XParse << value.stInsideTitle.dwSuffix;
    packet.XParse << value.stOutsideTitle.dwPrefix;
    packet.XParse << value.stOutsideTitle.dwSuffix;
    packet << value.vecSocketList;
    packet << value.vecBroachList;
    packet.XParse << value.uAppearance.biAppearance;
    packet.XParse << value.uAppearanceEx.biAppearance;
    packet << value.psOptionList;
    return packet;
}

// PS_DB_CHARACTER_INFO_OTHER_RES 序列化操作符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_CHARACTER_INFO_OTHER_RES& value) {
    packet.XParse << value.nError;
    packet.XParse << value.bCalculateStat;
    packet << value.stTargetInfo;
    packet << value.stCatrgory;
    packet << value.psCard;
    packet.XParse << static_cast<std::int32_t>(value.vecSkill.size());
    for (const auto& skill : value.vecSkill) {
        packet.XParse << skill;
    }
    return packet;
}

// ST_CHAR_COMMUNITY XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_CHAR_COMMUNITY& value) {
    packet.XParse << value.byState;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << FixedWideArrayToWString(value.szComment);
    packet.XParse << FixedWideArrayToWString(value.szMemo);
    return packet;
}
// ============================================================================
// static_assert 验证（确保结构体大小与 PDB 匹配）
// ============================================================================

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
