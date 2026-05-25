#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerParty.h - 组队(Party)和公会(Force)相关结构及序列化运算符
//
// 此文件包含组队和公会系统相关的所有结构体定义及其网络序列化运算符：
// - 组队进入/创建/离开/删除
// - 组队招募系统
// - 公会进入/创建/离开/删除
// - 公会匹配系统
// - 组队/公会聊天
//
// 依赖关系：PSServerParty.h -> PSServerCore.h -> PSCommon.h
// ============================================================================

// ============================================================================
// 组队(Party)相关结构体
// ============================================================================

struct PS_REQ_PARTY_ENTER_SERVER {
    std::uint32_t dwMemberID = 0;
    std::uint32_t dwPartyID = 0;
    UXMapID uxMapID{};
    int nMaxHP = 0;
    bool bReqPartyInfo = false;
    std::uint8_t _pad0[3] = {};
};

struct PS_PARTY_INFO {
    std::uint32_t dwPartyID = 0;
    std::uint32_t dwMaster = 0;
    UXMapID uxMazeID{};
    std::uint8_t byUpdateType = 0;
    std::uint8_t byPartyType = 0;
    std::uint8_t _pad0[6] = {};
    std::vector<ST_PARTY_MEMBER> vecPartyMember;
};

struct PS_RES_PARTY_ENTER_SERVER {
    bool bLoadParty = false;
    std::uint8_t _pad0[7] = {};
    ST_PARTY_MEMBER stEnterMember{};
    PS_PARTY_INFO stPartyInfo{};
};

struct ST_APPLY_MEMBER {
    ST_PARTY_MEMBER stMember{};
    int nRegDate = 0;
    std::uint8_t _pad0[4] = {};
};

struct ST_APPLY_MEMBER_LIST {
    ST_APPLY_MEMBER stInfo[10] = {};
};

struct ST_PARTY_RECRUIT {
    std::uint32_t dwPartyID = 0;
    wchar_t szMsg[21] = {};
    std::int16_t shMinLevel = 0;
    std::int16_t shMaxLevel = 0;
    std::uint8_t byPurpose = 0;
    std::uint8_t _pad0 = 0;
    wchar_t szLeaderName[21] = {};
    std::uint8_t byUserCount = 0;
    std::uint8_t _pad1 = 0;
    int nRemainTime = 0;
    std::uint32_t dwPurposeMapID = 0;
    std::uint32_t dwRecruitID = 0;
    std::uint32_t dwMasterUCID = 0;
    std::uint8_t byPartyGroupType = 0;
    std::uint8_t _pad2[3] = {};
};

struct ST_PARTY_RECRUIT_LIST {
    std::vector<ST_PARTY_RECRUIT> vecInfo;
    bool bLast = false;
    std::uint8_t _pad0[7] = {};
};

struct ST_PARTY_RECRUIT_INFO {
    ST_PARTY_RECRUIT stRecruit{};
    std::vector<unsigned long> vecMember;
};

struct ST_PARTY_RECRUIT_INFO_LIST {
    std::vector<ST_PARTY_RECRUIT_INFO> vecInfo;
};

struct ST_PARTY_MEMBER_LIST {
    std::vector<ST_PARTY_MEMBER> vecInfo;
};

struct ST_PARTY_RECRUIT_APPLY_INFO {
    int nResult = 0;
    std::uint32_t dwActorID = 0;
    std::uint32_t dwRecruitID = 0;
    std::uint32_t dwMasterUCID = 0;
    std::uint8_t byPartyGroupType = 0;
    std::uint8_t _pad0[7] = {};
    ST_PARTY_MEMBER_LIST stMemberList{};
};

struct ST_PARTY_RECRUIT_APPLY {
    std::uint32_t dwRecruitID = 0;
    std::uint8_t byPartyGroupType = 0;
    std::uint8_t _pad0[3] = {};
};

// 对齐 IDA: 匹配信息结构（368 字节）
struct ST_MATCHING_INFO {
    std::uint32_t dwMatchingID = 0;
    std::uint8_t _pad0[4] = {};
    ST_PARTY_MEMBER stMemberInfo[4]{};
    int nRemainTick = 0;
};

struct PS_REQ_PARTY_CREATE {
    std::uint32_t dwPartyID = 0;
    std::uint32_t dwReqServerID = 0;
    ST_PARTY_MEMBER masterInfo{};
    ST_PARTY_MEMBER memberInfo{};
    int nErrorCode = 0;
    std::uint32_t dwRecruitID = 0;
    std::uint32_t dwMasterUAID = 0;
};

struct PS_PARTY_ADDMEMBER {
    ST_PARTY_MEMBER stMember{};
    std::uint32_t dwPartyID = 0;
    int nErrorCode = 0;
};

struct PS_PARTY_LEAVE {
    std::uint32_t dwPartyID = 0;
    std::uint32_t dwLeaveMember = 0;
    bool bKickout = false;
    std::uint8_t _pad0[3] = {};
};

struct PS_PARTY_DELETE {
    std::uint32_t dwPartyID = 0;
    std::uint32_t dwLeaveMember = 0;
};

struct PS_PARTY_CHANGE_MASTER {
    std::uint32_t dwReqActorID = 0;
    std::uint32_t dwNewMasterID = 0;
    std::uint32_t dwPartyID = 0;
    int nErrorCode = 0;
};

/**
 * @brief 组队登录成员请求结构 (DBAgent)
 * 来自 IDA: PS_REQ_PARTY_LOGIN_MEMBER - 32 bytes
 */
struct PS_REQ_PARTY_LOGIN_MEMBER {
    std::uint32_t dwMemberID = 0;     // +0x00: 成员ID (4 bytes)
    std::uint32_t dwPartyID = 0;      // +0x04: 组队ID (4 bytes)
    int nMapID = 0;                   // +0x08: 地图ID (4 bytes)
    std::uint8_t _pad0[4] = {};       // +0x0C: padding (4 bytes)
    UXMapID uxMapID{};                // +0x10: 地图ID (8 bytes)
    int nMaxHP = 0;                   // +0x18: 最大HP (4 bytes)
    // +0x1C-0x1F: padding (4 bytes)
};

static_assert(sizeof(PS_REQ_PARTY_LOGIN_MEMBER) == 32, "PS_REQ_PARTY_LOGIN_MEMBER size must match IDA");
static_assert(offsetof(PS_REQ_PARTY_LOGIN_MEMBER, dwPartyID) == 4, "PS_REQ_PARTY_LOGIN_MEMBER.dwPartyID offset mismatch");
static_assert(offsetof(PS_REQ_PARTY_LOGIN_MEMBER, nMapID) == 8, "PS_REQ_PARTY_LOGIN_MEMBER.nMapID offset mismatch");
static_assert(offsetof(PS_REQ_PARTY_LOGIN_MEMBER, uxMapID) == 16, "PS_REQ_PARTY_LOGIN_MEMBER.uxMapID offset mismatch");
static_assert(offsetof(PS_REQ_PARTY_LOGIN_MEMBER, nMaxHP) == 24, "PS_REQ_PARTY_LOGIN_MEMBER.nMaxHP offset mismatch");

// PS_REQ_PARTY_LOGIN_MEMBER 序列化运算符
inline void operator>>(XPacket& packet, PS_REQ_PARTY_LOGIN_MEMBER& value) {
    packet.XParse >> value.dwMemberID;
    packet.XParse >> value.dwPartyID;
    packet.XParse >> value.nMapID;
    packet >> value.uxMapID;
    packet.XParse >> value.nMaxHP;
}

inline XPacket& operator<<(XPacket& packet, const PS_REQ_PARTY_LOGIN_MEMBER& value) {
    packet.XParse << value.dwMemberID;
    packet.XParse << value.dwPartyID;
    packet.XParse << value.nMapID;
    packet << value.uxMapID;
    packet.XParse << value.nMaxHP;
    return packet;
}

struct PS_FORCE_CHANGE_MASTER {
    std::uint32_t dwReqActorID = 0;
    std::uint32_t dwNewMasterID = 0;
    std::uint32_t dwForceID = 0;
    int nErrorCode = 0;
};

struct ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT {
    std::uint32_t dwMasterUCID = 0;
    std::uint32_t dwTargetUCID = 0;
    std::uint8_t byPartyGroupType = 0;
    std::uint8_t _pad0[3] = {};
};

struct PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK {
    std::uint32_t dwServerID = 0;
    int nErrorCode = 0;
    std::uint32_t dwUAID = 0;
    std::uint32_t dwRecruitID = 0;
    std::uint8_t byPartyGroupType = 0;
    std::uint8_t _pad0[7] = {};
    PS_PARTY_ADDMEMBER stMember{};
};

struct ST_PARTY_RECRUIT_DEL {
    std::uint32_t dwPartyID = 0;
    std::uint32_t dwMasterID = 0;
    std::uint32_t dwRecruitID = 0;
};

struct ST_PARTY_RECRUIT_DEL_LIST {
    std::vector<ST_PARTY_RECRUIT_DEL> vecInfo;
};

// 对齐 IDA 0x1400AF0B0/0x1400AF270: 招募成员信息更新结构 (16 字节)
// 用于 ApplyMemberLevelUp / ApplyMemberMapMove 通知
struct ST_PARTY_RECRUIT_UPDATE {
    std::uint32_t dwActorID = 0;    // 申请者 UCID
    std::int16_t shLevel = 0;       // 等级更新 (ApplyMemberLevelUp)
    std::uint16_t _pad0 = 0;
    std::uint32_t dwMapID = 0;      // 地图更新 (ApplyMemberMapMove)
    std::uint32_t _pad1 = 0;
};

struct PS_RECRUIT_DELETE {
    std::uint32_t dwUCID = 0;
};

struct PS_RECRUIT_STATE {
    bool bRecruit = false;
};

struct PS_RES_RECRUIT_DELETE {
    std::uint32_t dwUCID = 0;
    int nResult = 0;
};

struct PS_SERVER_PARTY_RECRUIT_ADD_REQ {
    ST_PARTY_RECRUIT stRecruit{};
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t _pad0[3] = {};
};

struct PS_SERVER_PARTY_RECRUIT_ADD_RES {
    ST_PARTY_RECRUIT_INFO stRecruitInfo{};
    std::uint32_t dwUCID = 0;
    std::uint32_t dwRecruitID = 0;
    int nResult = 0;
    int nRemainSec = 0;
};

// ============================================================================
// 公会(Force)相关结构体
// ============================================================================

struct ST_CREATE_FORCE {
    std::uint32_t dwMatchingID = 0;
    std::uint32_t dwLeaderUCID = 0;
    std::uint32_t dwMemberUCID[8] = {};
};

struct PS_DB_FORCE_MATCHING_CREATE {
    ST_CREATE_FORCE stCreateForce{};
    std::set<std::uint32_t> setDeleteParty;
    std::set<std::uint32_t> setDeleteForce;
};

struct PS_SERVER_FORCE_MATCHING_ENTER_MEMBER {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad0 = 0;
    std::uint32_t dwProfilePhotoID = 0;
    wchar_t strName[21] = {};
    UXMapID uxMapID{};
    std::int64_t nExp = 0;
    int nState = 0;
};

struct ST_FORCE_MEMBER {
    std::uint32_t dwMemberID = 0;
    wchar_t strName[21] = {};
    std::uint8_t byLevel = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad0[3] = {};
    std::uint32_t dwProfilePhotoID = 0;
    int nMapID = 0;
    int nChannel = 0;
    int nMaxHP = 0;
    int nHP = 0;
    bool bLogin = false;
    std::uint8_t _pad1[7] = {};
    UXMapID uxMapID{};
};

struct ST_FORCE_MATCHING_INFO {
    std::uint32_t dwMatchingID = 0;
    std::uint8_t _pad0[4] = {};
    ST_FORCE_MEMBER stMemberInfo[8] = {};
    int nRemainTick = 0;
    std::uint8_t _pad1[4] = {};
};

struct ST_UPDATE_FORCE_MEMBER {
    std::uint32_t dwForceID = 0;
    std::uint8_t _pad0[4] = {};
    ST_FORCE_MEMBER stForceMember{};
};

struct PS_FORCE_ADDMEMBER {
    ST_FORCE_MEMBER stMember{};
    std::uint32_t dwForceID = 0;
    int nErrorCode = 0;
};

struct PS_FORCE_LEAVE {
    std::uint32_t dwForceID = 0;
    std::uint32_t dwLeaveMember = 0;
    bool bKickout = false;
    std::uint8_t _pad0[3] = {};
};

struct PS_FORCE_DELETE {
    std::uint32_t dwForceID = 0;
    std::uint32_t dwLeaveMember = 0;
};

struct PS_REQ_FORCE_CREATE {
    std::uint32_t dwForceID = 0;
    std::uint32_t dwReqServerID = 0;
    ST_FORCE_MEMBER masterInfo{};
    ST_FORCE_MEMBER memberInfo{};
    int nErrorCode = 0;
    std::uint32_t dwRecruitID = 0;
    std::uint32_t dwMasterUAID = 0;
};

struct PS_REQ_FORCE_ENTER_SERVER {
    std::uint32_t dwMemberID = 0;
    std::uint32_t dwForceID = 0;
    UXMapID uxMapID{};
    int nMaxHP = 0;
    bool bReqForceInfo = false;
    std::uint8_t _pad0[3] = {};
};

struct PS_FORCE_INFO {
    std::uint32_t dwForceID = 0;
    std::uint32_t dwMaster = 0;
    UXMapID uxMazeID{};
    std::uint8_t byUpdateType = 0;
    std::uint8_t byForceType = 0;
    std::uint8_t _pad0[6] = {};
    std::vector<ST_FORCE_MEMBER> vecForceMember;
};

struct PS_PARTY_INFO_ALL {
    std::vector<PS_PARTY_INFO> vecPartyInfo;
};

struct PS_FORCE_INFO_ALL {
    std::vector<PS_FORCE_INFO> vecForceInfo;
};

struct PS_CHAT_PARTY {
    std::uint32_t dwActorID = 0;
    std::uint32_t dwPartyID = 0;
    wchar_t szMsg[256] = {};
};

struct PS_RES_FORCE_ENTER_SERVER {
    bool bLoadForce = false;
    std::uint8_t _pad0[7] = {};
    ST_FORCE_MEMBER stEnterMember{};
    PS_FORCE_INFO stForceInfo{};
};

// ============================================================================
// 序列化运算符
// ============================================================================

// ST_MATCHING_INFO 序列化 - 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const ST_MATCHING_INFO& value) {
    packet.XParse << value.dwMatchingID;
    packet.XParse << static_cast<std::uint32_t>(0);  // _pad0
    for (int i = 0; i < 4; ++i) {
        packet << value.stMemberInfo[i];
    }
    packet.XParse << value.nRemainTick;
    return packet;
}

// PS_PARTY_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_PARTY_INFO& value) {
    packet.XParse << value.dwPartyID;
    packet.XParse << value.dwMaster;
    packet.XParse << value.uxMazeID.nMapID;
    packet.XParse << value.byUpdateType;
    packet.XParse << value.byPartyType;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    const std::uint8_t memberCount =
        static_cast<std::uint8_t>(std::min<std::size_t>(value.vecPartyMember.size(), 0xFF));
    packet.XParse << memberCount;
    for (std::size_t index = 0; index < memberCount; ++index) {
        packet << value.vecPartyMember[index];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_PARTY_INFO& value) {
    packet.XParse >> value.dwPartyID;
    packet.XParse >> value.dwMaster;
    packet.XParse >> value.uxMazeID.nMapID;
    packet.XParse >> value.byUpdateType;
    packet.XParse >> value.byPartyType;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    value.vecPartyMember.clear();
    std::uint8_t memberCount = 0;
    packet.XParse >> memberCount;
    value.vecPartyMember.reserve(memberCount);
    for (std::uint8_t index = 0; index < memberCount; ++index) {
        ST_PARTY_MEMBER member{};
        packet >> member;
        value.vecPartyMember.push_back(member);
    }
}

// PS_REQ_PARTY_ENTER_SERVER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_REQ_PARTY_ENTER_SERVER& value) {
    packet.XParse << value.dwMemberID;
    packet.XParse << value.dwPartyID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.nMaxHP;
    packet.XParse << value.bReqPartyInfo;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_PARTY_ENTER_SERVER& value) {
    packet.XParse >> value.dwMemberID;
    packet.XParse >> value.dwPartyID;
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.nMaxHP;
    packet.XParse >> value.bReqPartyInfo;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
}

// PS_RES_PARTY_ENTER_SERVER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_RES_PARTY_ENTER_SERVER& value) {
    packet.XParse << value.bLoadParty;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet << value.stEnterMember;
    packet << value.stPartyInfo;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_PARTY_ENTER_SERVER& value) {
    packet.XParse >> value.bLoadParty;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet >> value.stEnterMember;
    packet >> value.stPartyInfo;
}

// ST_FORCE_MEMBER 序列化
inline XPacket& operator<<(XPacket& packet, const ST_FORCE_MEMBER& value) {
    packet.XParse << value.dwMemberID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.byLevel;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.nMapID;
    packet.XParse << value.nChannel;
    packet.XParse << value.nMaxHP;
    packet.XParse << value.nHP;
    packet.XParse << value.bLogin;
    packet.XParse << value.uxMapID.nMapID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_FORCE_MEMBER& value) {
    packet.XParse >> value.dwMemberID;
    short outLen = 0;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.byLevel;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.nMapID;
    packet.XParse >> value.nChannel;
    packet.XParse >> value.nMaxHP;
    packet.XParse >> value.nHP;
    packet.XParse >> value.bLogin;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse >> value.uxMapID.nMapID;
}

// ST_APPLY_MEMBER 序列化
inline XPacket& operator<<(XPacket& packet, const ST_APPLY_MEMBER& value) {
    packet << value.stMember;
    packet.XParse << value.nRegDate;
    packet.XParse << static_cast<std::uint32_t>(0);
    return packet;
}

inline void operator>>(XPacket& packet, ST_APPLY_MEMBER& value) {
    packet >> value.stMember;
    packet.XParse >> value.nRegDate;
    packet.XParse.GetDWORD();
}

// ST_APPLY_MEMBER_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const ST_APPLY_MEMBER_LIST& value) {
    for (const ST_APPLY_MEMBER& member : value.stInfo) {
        packet << member;
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_APPLY_MEMBER_LIST& value) {
    for (ST_APPLY_MEMBER& member : value.stInfo) {
        packet >> member;
    }
}

// ST_PARTY_RECRUIT 序列化
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_RECRUIT& value) {
    packet.XParse << value.dwPartyID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szMsg);
    packet.XParse << value.shMinLevel;
    packet.XParse << value.shMaxLevel;
    packet.XParse << value.byPurpose;
    packet.XParse << FixedWideArrayToWString(value.szLeaderName);
    packet.XParse << value.byUserCount;
    packet.XParse << value.nRemainTime;
    packet.XParse << value.dwPurposeMapID;
    packet.XParse << value.dwRecruitID;
    packet.XParse << value.dwMasterUCID;
    packet.XParse << value.byPartyGroupType;
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_RECRUIT& value) {
    packet.XParse >> value.dwPartyID;
    short outLen = 0;
    packet.XParse.GetWString(value.szMsg, 21, outLen);
    packet.XParse >> value.shMinLevel;
    packet.XParse >> value.shMaxLevel;
    packet.XParse >> value.byPurpose;
    packet.XParse.GetBYTE();
    packet.XParse.GetWString(value.szLeaderName, 21, outLen);
    packet.XParse >> value.byUserCount;
    packet.XParse.GetBYTE();
    packet.XParse >> value.nRemainTime;
    packet.XParse >> value.dwPurposeMapID;
    packet.XParse >> value.dwRecruitID;
    packet.XParse >> value.dwMasterUCID;
    packet.XParse >> value.byPartyGroupType;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
}

// ST_PARTY_RECRUIT_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_RECRUIT_LIST& value) {
    const std::uint32_t count = static_cast<std::uint32_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const ST_PARTY_RECRUIT& recruit : value.vecInfo) {
        packet << recruit;
    }
    packet.XParse << value.bLast;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint32_t>(0);
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_RECRUIT_LIST& value) {
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint32_t index = 0; index < count; ++index) {
        ST_PARTY_RECRUIT recruit{};
        packet >> recruit;
        value.vecInfo.push_back(std::move(recruit));
    }
    packet.XParse >> value.bLast;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetDWORD();
}

// ST_PARTY_MEMBER_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_MEMBER_LIST& value) {
    const std::uint32_t count = static_cast<std::uint32_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const ST_PARTY_MEMBER& member : value.vecInfo) {
        packet << member;
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_MEMBER_LIST& value) {
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint32_t index = 0; index < count; ++index) {
        ST_PARTY_MEMBER member{};
        packet >> member;
        value.vecInfo.push_back(std::move(member));
    }
}

// ST_PARTY_RECRUIT_APPLY_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_RECRUIT_APPLY_INFO& value) {
    packet.XParse << value.nResult;
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwRecruitID;
    packet.XParse << value.dwMasterUCID;
    packet.XParse << value.byPartyGroupType;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet << value.stMemberList;
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_RECRUIT_APPLY_INFO& value) {
    packet.XParse >> value.nResult;
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwRecruitID;
    packet.XParse >> value.dwMasterUCID;
    packet.XParse >> value.byPartyGroupType;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet >> value.stMemberList;
}

// ST_PARTY_RECRUIT_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_RECRUIT_INFO& value) {
    packet << value.stRecruit;
    const std::uint32_t memberCount = static_cast<std::uint32_t>(value.vecMember.size());
    packet.XParse << memberCount;
    for (const unsigned long memberID : value.vecMember) {
        packet.XParse << static_cast<std::uint32_t>(memberID);
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_RECRUIT_INFO& value) {
    packet >> value.stRecruit;
    std::uint32_t memberCount = 0;
    packet.XParse >> memberCount;
    value.vecMember.clear();
    value.vecMember.reserve(memberCount);
    for (std::uint32_t index = 0; index < memberCount; ++index) {
        std::uint32_t memberID = 0;
        packet.XParse >> memberID;
        value.vecMember.push_back(memberID);
    }
}

// ST_PARTY_RECRUIT_INFO_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_RECRUIT_INFO_LIST& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecInfo.size(), 0xFFFF));
    packet.XParse << count;
    for (std::size_t index = 0; index < count; ++index) {
        packet << value.vecInfo[index];
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_RECRUIT_INFO_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint16_t index = 0; index < count; ++index) {
        ST_PARTY_RECRUIT_INFO item{};
        packet >> item;
        value.vecInfo.push_back(std::move(item));
    }
}

// ST_PARTY_RECRUIT_APPLY 序列化
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_RECRUIT_APPLY& value) {
    packet.XParse << value.dwRecruitID;
    packet.XParse << value.byPartyGroupType;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_RECRUIT_APPLY& value) {
    packet.XParse >> value.dwRecruitID;
    packet.XParse >> value.byPartyGroupType;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
}

// PS_REQ_PARTY_CREATE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_REQ_PARTY_CREATE& value) {
    packet.XParse << value.dwPartyID;
    packet.XParse << value.dwReqServerID;
    packet << value.masterInfo;
    packet << value.memberInfo;
    packet.XParse << value.nErrorCode;
    packet.XParse << value.dwRecruitID;
    packet.XParse << value.dwMasterUAID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_PARTY_CREATE& value) {
    packet.XParse >> value.dwPartyID;
    packet.XParse >> value.dwReqServerID;
    packet >> value.masterInfo;
    packet >> value.memberInfo;
    packet.XParse >> value.nErrorCode;
    packet.XParse >> value.dwRecruitID;
    packet.XParse >> value.dwMasterUAID;
}

// PS_PARTY_ADDMEMBER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_PARTY_ADDMEMBER& value) {
    packet << value.stMember;
    packet.XParse << value.dwPartyID;
    packet.XParse << value.nErrorCode;
    return packet;
}

inline void operator>>(XPacket& packet, PS_PARTY_ADDMEMBER& value) {
    packet >> value.stMember;
    packet.XParse >> value.dwPartyID;
    packet.XParse >> value.nErrorCode;
}

// PS_FORCE_ADDMEMBER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_FORCE_ADDMEMBER& value) {
    packet << value.stMember;
    packet.XParse << value.dwForceID;
    packet.XParse << value.nErrorCode;
    return packet;
}

inline void operator>>(XPacket& packet, PS_FORCE_ADDMEMBER& value) {
    packet >> value.stMember;
    packet.XParse >> value.dwForceID;
    packet.XParse >> value.nErrorCode;
}

// PS_PARTY_LEAVE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_PARTY_LEAVE& value) {
    packet.XParse << value.dwPartyID;
    packet.XParse << value.dwLeaveMember;
    packet.XParse << value.bKickout;
    return packet;
}

inline void operator>>(XPacket& packet, PS_PARTY_LEAVE& value) {
    packet.XParse >> value.dwPartyID;
    packet.XParse >> value.dwLeaveMember;
    packet.XParse >> value.bKickout;
}

// PS_PARTY_DELETE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_PARTY_DELETE& value) {
    packet.XParse << value.dwPartyID;
    packet.XParse << value.dwLeaveMember;
    return packet;
}

inline void operator>>(XPacket& packet, PS_PARTY_DELETE& value) {
    packet.XParse >> value.dwPartyID;
    packet.XParse >> value.dwLeaveMember;
}

// PS_FORCE_LEAVE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_FORCE_LEAVE& value) {
    packet.XParse << value.dwForceID;
    packet.XParse << value.dwLeaveMember;
    packet.XParse << value.bKickout;
    return packet;
}

inline void operator>>(XPacket& packet, PS_FORCE_LEAVE& value) {
    packet.XParse >> value.dwForceID;
    packet.XParse >> value.dwLeaveMember;
    packet.XParse >> value.bKickout;
}

// PS_FORCE_DELETE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_FORCE_DELETE& value) {
    packet.XParse << value.dwForceID;
    packet.XParse << value.dwLeaveMember;
    return packet;
}

inline void operator>>(XPacket& packet, PS_FORCE_DELETE& value) {
    packet.XParse >> value.dwForceID;
    packet.XParse >> value.dwLeaveMember;
}

// PS_PARTY_CHANGE_MASTER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_PARTY_CHANGE_MASTER& value) {
    packet.XParse << value.dwReqActorID;
    packet.XParse << value.dwNewMasterID;
    packet.XParse << value.dwPartyID;
    packet.XParse << value.nErrorCode;
    return packet;
}

inline void operator>>(XPacket& packet, PS_PARTY_CHANGE_MASTER& value) {
    packet.XParse >> value.dwReqActorID;
    packet.XParse >> value.dwNewMasterID;
    packet.XParse >> value.dwPartyID;
    packet.XParse >> value.nErrorCode;
}

// PS_FORCE_CHANGE_MASTER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_FORCE_CHANGE_MASTER& value) {
    packet.XParse << value.dwReqActorID;
    packet.XParse << value.dwNewMasterID;
    packet.XParse << value.dwForceID;
    packet.XParse << value.nErrorCode;
    return packet;
}

inline void operator>>(XPacket& packet, PS_FORCE_CHANGE_MASTER& value) {
    packet.XParse >> value.dwReqActorID;
    packet.XParse >> value.dwNewMasterID;
    packet.XParse >> value.dwForceID;
    packet.XParse >> value.nErrorCode;
}

// PS_REQ_FORCE_CREATE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_REQ_FORCE_CREATE& value) {
    packet.XParse << value.dwForceID;
    packet.XParse << value.dwReqServerID;
    packet << value.masterInfo;
    packet << value.memberInfo;
    packet.XParse << value.nErrorCode;
    packet.XParse << value.dwRecruitID;
    packet.XParse << value.dwMasterUAID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_FORCE_CREATE& value) {
    packet.XParse >> value.dwForceID;
    packet.XParse >> value.dwReqServerID;
    packet >> value.masterInfo;
    packet >> value.memberInfo;
    packet.XParse >> value.nErrorCode;
    packet.XParse >> value.dwRecruitID;
    packet.XParse >> value.dwMasterUAID;
}

// ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT 序列化
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT& value) {
    packet.XParse << value.dwMasterUCID;
    packet.XParse << value.dwTargetUCID;
    packet.XParse << value.byPartyGroupType;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_RECRUIT_APPLY_ACCEPT_REJECT& value) {
    packet.XParse >> value.dwMasterUCID;
    packet.XParse >> value.dwTargetUCID;
    packet.XParse >> value.byPartyGroupType;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
}

// PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK& value) {
    packet.XParse << value.dwServerID;
    packet.XParse << value.nErrorCode;
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwRecruitID;
    packet.XParse << value.byPartyGroupType;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint32_t>(0);
    packet << value.stMember;
    return packet;
}

inline void operator>>(XPacket& packet, PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK& value) {
    packet.XParse >> value.dwServerID;
    packet.XParse >> value.nErrorCode;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwRecruitID;
    packet.XParse >> value.byPartyGroupType;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetDWORD();
    packet >> value.stMember;
}

// ST_PARTY_RECRUIT_DEL 序列化
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_RECRUIT_DEL& value) {
    packet.XParse << value.dwPartyID;
    packet.XParse << value.dwMasterID;
    packet.XParse << value.dwRecruitID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_RECRUIT_DEL& value) {
    packet.XParse >> value.dwPartyID;
    packet.XParse >> value.dwMasterID;
    packet.XParse >> value.dwRecruitID;
}

// ST_PARTY_RECRUIT_DEL_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_RECRUIT_DEL_LIST& value) {
    const std::uint8_t count = static_cast<std::uint8_t>(std::min<std::size_t>(value.vecInfo.size(), 0xFF));
    packet.XParse << count;
    for (std::size_t index = 0; index < value.vecInfo.size() && index < static_cast<std::size_t>(count); ++index) {
        packet << value.vecInfo[index];
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_RECRUIT_DEL_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_PARTY_RECRUIT_DEL item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

// ST_PARTY_RECRUIT_UPDATE 序列化 - 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_RECRUIT_UPDATE& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.shLevel;
    packet.XParse << value.dwMapID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_RECRUIT_UPDATE& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.shLevel;
    packet.XParse >> value.dwMapID;
}

// PS_RECRUIT_DELETE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_RECRUIT_DELETE& value) {
    packet.XParse << value.dwUCID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RECRUIT_DELETE& value) {
    packet.XParse >> value.dwUCID;
}

// PS_RECRUIT_STATE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_RECRUIT_STATE& value) {
    packet.XParse << value.bRecruit;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RECRUIT_STATE& value) {
    packet.XParse >> value.bRecruit;
}

// PS_RES_RECRUIT_DELETE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_RES_RECRUIT_DELETE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_RECRUIT_DELETE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nResult;
}

// PS_SERVER_PARTY_RECRUIT_ADD_REQ 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_PARTY_RECRUIT_ADD_REQ& value) {
    packet << value.stRecruit;
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.byLevel;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    return packet;
}

inline void operator>>(XPacket& packet, PS_SERVER_PARTY_RECRUIT_ADD_REQ& value) {
    packet >> value.stRecruit;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byLevel;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
}

// PS_SERVER_PARTY_RECRUIT_ADD_RES 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_PARTY_RECRUIT_ADD_RES& value) {
    packet << value.stRecruitInfo;
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwRecruitID;
    packet.XParse << value.nResult;
    packet.XParse << value.nRemainSec;
    return packet;
}

inline void operator>>(XPacket& packet, PS_SERVER_PARTY_RECRUIT_ADD_RES& value) {
    packet >> value.stRecruitInfo;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwRecruitID;
    packet.XParse >> value.nResult;
    packet.XParse >> value.nRemainSec;
}

// PS_SERVER_FORCE_MATCHING_ENTER_MEMBER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_FORCE_MATCHING_ENTER_MEMBER& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.byClass;
    packet.XParse << value.byLevel;
    packet.XParse << value.byAwaken;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.nExp;
    packet.XParse << value.nState;
    return packet;
}

inline void operator>>(XPacket& packet, PS_SERVER_FORCE_MATCHING_ENTER_MEMBER& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byLevel;
    packet.XParse >> value.byAwaken;
    packet.XParse.GetBYTE();
    packet.XParse >> value.dwProfilePhotoID;
    short outLen = 0;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.nExp;
    packet.XParse >> value.nState;
}

// ST_FORCE_MATCHING_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_FORCE_MATCHING_INFO& value) {
    packet.XParse << value.dwMatchingID;
    packet.XParse << static_cast<std::uint32_t>(0);
    for (const ST_FORCE_MEMBER& member : value.stMemberInfo) {
        packet << member;
    }
    packet.XParse << value.nRemainTick;
    packet.XParse << static_cast<std::uint32_t>(0);
    return packet;
}

inline void operator>>(XPacket& packet, ST_FORCE_MATCHING_INFO& value) {
    packet.XParse >> value.dwMatchingID;
    packet.XParse.GetDWORD();
    for (ST_FORCE_MEMBER& member : value.stMemberInfo) {
        packet >> member;
    }
    packet.XParse >> value.nRemainTick;
    packet.XParse.GetDWORD();
}

// ST_CREATE_FORCE 序列化
inline XPacket& operator<<(XPacket& packet, const ST_CREATE_FORCE& value) {
    packet.XParse << value.dwMatchingID;
    packet.XParse << value.dwLeaderUCID;
    for (std::uint32_t memberUCID : value.dwMemberUCID) {
        packet.XParse << memberUCID;
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_CREATE_FORCE& value) {
    packet.XParse >> value.dwMatchingID;
    packet.XParse >> value.dwLeaderUCID;
    for (std::uint32_t& memberUCID : value.dwMemberUCID) {
        packet.XParse >> memberUCID;
    }
}

// PS_DB_FORCE_MATCHING_CREATE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_FORCE_MATCHING_CREATE& value) {
    packet << value.stCreateForce;
    const std::uint16_t partyCount = static_cast<std::uint16_t>(value.setDeleteParty.size());
    packet.XParse << partyCount;
    for (std::uint32_t partyID : value.setDeleteParty) {
        packet.XParse << partyID;
    }
    const std::uint16_t forceCount = static_cast<std::uint16_t>(value.setDeleteForce.size());
    packet.XParse << forceCount;
    for (std::uint32_t forceID : value.setDeleteForce) {
        packet.XParse << forceID;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_DB_FORCE_MATCHING_CREATE& value) {
    packet >> value.stCreateForce;
    std::uint16_t partyCount = 0;
    packet.XParse >> partyCount;
    value.setDeleteParty.clear();
    for (std::uint16_t index = 0; index < partyCount; ++index) {
        std::uint32_t partyID = 0;
        packet.XParse >> partyID;
        value.setDeleteParty.insert(partyID);
    }
    std::uint16_t forceCount = 0;
    packet.XParse >> forceCount;
    value.setDeleteForce.clear();
    for (std::uint16_t index = 0; index < forceCount; ++index) {
        std::uint32_t forceID = 0;
        packet.XParse >> forceID;
        value.setDeleteForce.insert(forceID);
    }
}

// ST_UPDATE_FORCE_MEMBER 序列化
inline XPacket& operator<<(XPacket& packet, const ST_UPDATE_FORCE_MEMBER& value) {
    packet.XParse << value.dwForceID;
    packet.XParse << static_cast<std::uint32_t>(0);
    packet << value.stForceMember;
    return packet;
}

inline void operator>>(XPacket& packet, ST_UPDATE_FORCE_MEMBER& value) {
    packet.XParse >> value.dwForceID;
    packet.XParse.GetDWORD();
    packet >> value.stForceMember;
}

// PS_FORCE_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_FORCE_INFO& value) {
    packet.XParse << value.dwForceID;
    packet.XParse << value.dwMaster;
    packet.XParse << value.uxMazeID.nMapID;
    packet.XParse << value.byUpdateType;
    packet.XParse << value.byForceType;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    const std::uint8_t memberCount =
        static_cast<std::uint8_t>(std::min<std::size_t>(value.vecForceMember.size(), 0xFF));
    packet.XParse << memberCount;
    for (std::size_t index = 0; index < memberCount; ++index) {
        packet << value.vecForceMember[index];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_FORCE_INFO& value) {
    packet.XParse >> value.dwForceID;
    packet.XParse >> value.dwMaster;
    packet.XParse >> value.uxMazeID.nMapID;
    packet.XParse >> value.byUpdateType;
    packet.XParse >> value.byForceType;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    std::uint8_t memberCount = 0;
    packet.XParse >> memberCount;
    value.vecForceMember.clear();
    value.vecForceMember.reserve(memberCount);
    for (std::uint8_t index = 0; index < memberCount; ++index) {
        ST_FORCE_MEMBER member{};
        packet >> member;
        value.vecForceMember.push_back(member);
    }
}

// PS_REQ_FORCE_ENTER_SERVER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_REQ_FORCE_ENTER_SERVER& value) {
    packet.XParse << value.dwMemberID;
    packet.XParse << value.dwForceID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.nMaxHP;
    packet.XParse << value.bReqForceInfo;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_FORCE_ENTER_SERVER& value) {
    packet.XParse >> value.dwMemberID;
    packet.XParse >> value.dwForceID;
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.nMaxHP;
    packet.XParse >> value.bReqForceInfo;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
}

// PS_CHAT_PARTY 序列化
inline XPacket& operator<<(XPacket& packet, const PS_CHAT_PARTY& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwPartyID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szMsg);
    return packet;
}

inline void operator>>(XPacket& packet, PS_CHAT_PARTY& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwPartyID;
    short outLen = 0;
    packet.XParse.GetWString(value.szMsg, 256, outLen);
}

// PS_RES_FORCE_ENTER_SERVER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_RES_FORCE_ENTER_SERVER& value) {
    packet.XParse << value.bLoadForce;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet << value.stEnterMember;
    packet << value.stForceInfo;
    return packet;
}

// PS_PARTY_INFO_ALL 序列化
inline void operator>>(XPacket& packet, PS_PARTY_INFO_ALL& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecPartyInfo.clear();
    value.vecPartyInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint16_t index = 0; index < count; ++index) {
        PS_PARTY_INFO info{};
        packet >> info;
        value.vecPartyInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_PARTY_INFO_ALL& value) {
    const std::uint16_t count =
        static_cast<std::uint16_t>(std::min<std::size_t>(value.vecPartyInfo.size(), 0xFFFF));
    packet.XParse << count;
    for (std::uint16_t index = 0; index < count; ++index) {
        packet << value.vecPartyInfo[index];
    }
    return packet;
}

// PS_FORCE_INFO_ALL 序列化
inline void operator>>(XPacket& packet, PS_FORCE_INFO_ALL& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecForceInfo.clear();
    value.vecForceInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint16_t index = 0; index < count; ++index) {
        PS_FORCE_INFO info{};
        packet >> info;
        value.vecForceInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_FORCE_INFO_ALL& value) {
    const std::uint16_t count =
        static_cast<std::uint16_t>(std::min<std::size_t>(value.vecForceInfo.size(), 0xFFFF));
    packet.XParse << count;
    for (std::uint16_t index = 0; index < count; ++index) {
        packet << value.vecForceInfo[index];
    }
    return packet;
}

// ============================================================================
// 组队邀请/拒绝相关结构体
// ============================================================================

// Invite structure for party invite flow
struct ST_INVITE_INFO {
    std::uint32_t dwMasterID = 0;
    std::uint64_t dwLimitTime = 0;
};

struct PS_REQ_PARTY_INVITE {
    std::uint32_t dwReqActorID = 0;
    std::uint32_t dwInviteActorID = 0;
    wchar_t strReqName[21] = {};
    wchar_t strName[21] = {};
    std::uint32_t dwReqServerID = 0;
    std::int32_t nResult = 0;
};

struct PS_REQ_FORCE_INVITE {
    std::uint32_t dwReqActorID = 0;
    std::uint32_t dwInviteActorID = 0;
    wchar_t strReqName[21] = {};
    wchar_t strName[21] = {};
    std::uint32_t dwReqServerID = 0;
    std::int32_t nResult = 0;
};

// 对齐 IDA: 队伍邀请接受响应（12 字节）
struct PS_RES_PARTY_INVITE {
    std::uint32_t dwMasterID = 0;
    std::uint32_t dwAcceptID = 0;
    std::int32_t nResult = 0;
};

// Party accept response (8 bytes) - sent to client on accept error
struct PS_RES_PARTY_ACCEPT {
    std::uint32_t dwAcceptID = 0;
    std::int32_t nResult = 0;
};

// Force invite response (12 bytes) - sent from GameServer to RelayServer
struct PS_RES_FORCE_INVITE {
    std::uint32_t dwMasterID = 0;
    std::uint32_t dwAcceptID = 0;
    std::int32_t nResult = 0;
};

// Force accept response (8 bytes) - sent to client
struct PS_RES_FORCE_ACCEPT {
    std::uint32_t dwAcceptID = 0;
    std::int32_t nResult = 0;
};

// Party/Force reject packet (56 bytes) - used for both party and force reject
struct PS_PARTY_REJECT {
    std::uint32_t dwReqActor = 0;
    std::uint32_t dwRejectID = 0;
    wchar_t strRejectName[21] = {};
    std::uint32_t dwErrorID = 0;
};

// 对齐 IDA: PS_FORCE_REJECT 是 PS_PARTY_REJECT 的别名
using PS_FORCE_REJECT = PS_PARTY_REJECT;

// ============================================================================
// 组队邀请/拒绝序列化运算符
// ============================================================================

// 对齐 IDA: PS_RES_PARTY_INVITE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_RES_PARTY_INVITE& value) {
    packet.XParse << value.dwMasterID;
    packet.XParse << value.dwAcceptID;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_PARTY_INVITE& value) {
    packet.XParse >> value.dwMasterID;
    packet.XParse >> value.dwAcceptID;
    packet.XParse >> value.nResult;
}

// 对齐 IDA: PS_RES_PARTY_ACCEPT 序列化 (0x140098D20)
inline XPacket& operator<<(XPacket& packet, const PS_RES_PARTY_ACCEPT& value) {
    packet.XParse << value.dwAcceptID;
    packet.XParse << value.nResult;
    return packet;
}

// Serializers for invite structures
inline XPacket& operator<<(XPacket& packet, const PS_REQ_PARTY_INVITE& value) {
    packet.XParse << value.dwReqActorID;
    packet.XParse << value.dwInviteActorID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strReqName);
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.dwReqServerID;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_PARTY_INVITE& value) {
    packet.XParse >> value.dwReqActorID;
    packet.XParse >> value.dwInviteActorID;
    short outLen = 0;
    packet.XParse.GetWString(value.strReqName, 21, outLen);
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.dwReqServerID;
    packet.XParse >> value.nResult;
}

inline XPacket& operator<<(XPacket& packet, const PS_REQ_FORCE_INVITE& value) {
    packet.XParse << value.dwReqActorID;
    packet.XParse << value.dwInviteActorID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strReqName);
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.dwReqServerID;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_FORCE_INVITE& value) {
    packet.XParse >> value.dwReqActorID;
    packet.XParse >> value.dwInviteActorID;
    short outLen = 0;
    packet.XParse.GetWString(value.strReqName, 21, outLen);
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.dwReqServerID;
    packet.XParse >> value.nResult;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_FORCE_INVITE& value) {
    packet.XParse << value.dwMasterID;
    packet.XParse << value.dwAcceptID;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_FORCE_INVITE& value) {
    packet.XParse >> value.dwMasterID;
    packet.XParse >> value.dwAcceptID;
    packet.XParse >> value.nResult;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_FORCE_ACCEPT& value) {
    packet.XParse << value.dwAcceptID;
    packet.XParse << value.nResult;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_PARTY_REJECT& value) {
    packet.XParse << value.dwReqActor;
    packet.XParse << value.dwRejectID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strRejectName);
    packet.XParse << value.dwErrorID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_PARTY_REJECT& value) {
    packet.XParse >> value.dwReqActor;
    packet.XParse >> value.dwRejectID;
    short outLen = 0;
    packet.XParse.GetWString(value.strRejectName, 21, outLen);
    packet.XParse >> value.dwErrorID;
}
// ============================================================================
// static_assert 验证（确保结构体大小与 PDB 匹配）
// ============================================================================

static_assert(sizeof(ST_PARTY_INFO) == 0x08, "ST_PARTY_INFO size must match PDB");
static_assert(sizeof(ST_PARTY_MEMBER) == 0x58, "ST_PARTY_MEMBER size must match PDB");
static_assert(offsetof(ST_PARTY_MEMBER, strName) == 0x4, "ST_PARTY_MEMBER.strName offset mismatch");
static_assert(offsetof(ST_PARTY_MEMBER, byLevel) == 0x2E, "ST_PARTY_MEMBER.byLevel offset mismatch");
static_assert(offsetof(ST_PARTY_MEMBER, byAwaken) == 0x30, "ST_PARTY_MEMBER.byAwaken offset mismatch");
static_assert(offsetof(ST_PARTY_MEMBER, dwProfilePhotoID) == 0x34,
              "ST_PARTY_MEMBER.dwProfilePhotoID offset mismatch");
static_assert(offsetof(ST_PARTY_MEMBER, nMapID) == 0x38, "ST_PARTY_MEMBER.nMapID offset mismatch");
static_assert(offsetof(ST_PARTY_MEMBER, nChannel) == 0x3C,
              "ST_PARTY_MEMBER.nChannel offset mismatch");
static_assert(offsetof(ST_PARTY_MEMBER, nMaxHP) == 0x40, "ST_PARTY_MEMBER.nMaxHP offset mismatch");
static_assert(offsetof(ST_PARTY_MEMBER, nHP) == 0x44, "ST_PARTY_MEMBER.nHP offset mismatch");
static_assert(offsetof(ST_PARTY_MEMBER, bLogin) == 0x48, "ST_PARTY_MEMBER.bLogin offset mismatch");
static_assert(offsetof(ST_PARTY_MEMBER, uxMapID) == 0x50,
              "ST_PARTY_MEMBER.uxMapID offset mismatch");
static_assert(sizeof(ST_UPDATE_PARTY_MEMBER) == 0x60,
              "ST_UPDATE_PARTY_MEMBER size must match PDB");
static_assert(offsetof(ST_UPDATE_PARTY_MEMBER, stPartyMember) == 0x8,
              "ST_UPDATE_PARTY_MEMBER.stPartyMember offset mismatch");
static_assert(sizeof(PS_REQ_PARTY_ENTER_SERVER) == 0x18,
              "PS_REQ_PARTY_ENTER_SERVER size must match PDB");
static_assert(offsetof(PS_REQ_PARTY_ENTER_SERVER, uxMapID) == 0x8,
              "PS_REQ_PARTY_ENTER_SERVER.uxMapID offset mismatch");
static_assert(offsetof(PS_REQ_PARTY_ENTER_SERVER, nMaxHP) == 0x10,
              "PS_REQ_PARTY_ENTER_SERVER.nMaxHP offset mismatch");
static_assert(offsetof(PS_REQ_PARTY_ENTER_SERVER, bReqPartyInfo) == 0x14,
              "PS_REQ_PARTY_ENTER_SERVER.bReqPartyInfo offset mismatch");
static_assert(sizeof(PS_PARTY_INFO) == 0x38, "PS_PARTY_INFO size must match PDB");
static_assert(offsetof(PS_PARTY_INFO, uxMazeID) == 0x8, "PS_PARTY_INFO.uxMazeID offset mismatch");
static_assert(offsetof(PS_PARTY_INFO, byUpdateType) == 0x10,
              "PS_PARTY_INFO.byUpdateType offset mismatch");
static_assert(offsetof(PS_PARTY_INFO, vecPartyMember) == 0x18,
              "PS_PARTY_INFO.vecPartyMember offset mismatch");
static_assert(sizeof(PS_RES_PARTY_ENTER_SERVER) == 0x98,
              "PS_RES_PARTY_ENTER_SERVER size must match PDB");
static_assert(offsetof(PS_RES_PARTY_ENTER_SERVER, stEnterMember) == 0x8,
              "PS_RES_PARTY_ENTER_SERVER.stEnterMember offset mismatch");
static_assert(offsetof(PS_RES_PARTY_ENTER_SERVER, stPartyInfo) == 0x60,
              "PS_RES_PARTY_ENTER_SERVER.stPartyInfo offset mismatch");
static_assert(sizeof(ST_APPLY_MEMBER) == 0x60, "ST_APPLY_MEMBER size must match PDB");
static_assert(offsetof(ST_APPLY_MEMBER, stMember) == 0x0,
              "ST_APPLY_MEMBER.stMember offset mismatch");
static_assert(offsetof(ST_APPLY_MEMBER, nRegDate) == 0x58,
              "ST_APPLY_MEMBER.nRegDate offset mismatch");
static_assert(sizeof(ST_APPLY_MEMBER_LIST) == 0x3C0, "ST_APPLY_MEMBER_LIST size must match PDB");
static_assert(sizeof(ST_PARTY_RECRUIT) == 0x74, "ST_PARTY_RECRUIT size must match PDB");
static_assert(offsetof(ST_PARTY_RECRUIT, szMsg) == 0x4,
              "ST_PARTY_RECRUIT.szMsg offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT, shMinLevel) == 0x2E,
              "ST_PARTY_RECRUIT.shMinLevel offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT, shMaxLevel) == 0x30,
              "ST_PARTY_RECRUIT.shMaxLevel offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT, byPurpose) == 0x32,
              "ST_PARTY_RECRUIT.byPurpose offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT, szLeaderName) == 0x34,
              "ST_PARTY_RECRUIT.szLeaderName offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT, byUserCount) == 0x5E,
              "ST_PARTY_RECRUIT.byUserCount offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT, nRemainTime) == 0x60,
              "ST_PARTY_RECRUIT.nRemainTime offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT, dwPurposeMapID) == 0x64,
              "ST_PARTY_RECRUIT.dwPurposeMapID offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT, dwRecruitID) == 0x68,
              "ST_PARTY_RECRUIT.dwRecruitID offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT, dwMasterUCID) == 0x6C,
              "ST_PARTY_RECRUIT.dwMasterUCID offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT, byPartyGroupType) == 0x70,
              "ST_PARTY_RECRUIT.byPartyGroupType offset mismatch");
static_assert(sizeof(ST_PARTY_RECRUIT_INFO) == 0x98, "ST_PARTY_RECRUIT_INFO size must match PDB");
static_assert(offsetof(ST_PARTY_RECRUIT_INFO, stRecruit) == 0x0,
              "ST_PARTY_RECRUIT_INFO.stRecruit offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT_INFO, vecMember) == 0x78,
              "ST_PARTY_RECRUIT_INFO.vecMember offset mismatch");
static_assert(sizeof(ST_PARTY_RECRUIT_DEL) == 0xC, "ST_PARTY_RECRUIT_DEL size must match bounded wire layout");
static_assert(offsetof(ST_PARTY_RECRUIT_DEL, dwMasterID) == 0x4,
              "ST_PARTY_RECRUIT_DEL.dwMasterID offset mismatch");
static_assert(offsetof(ST_PARTY_RECRUIT_DEL, dwRecruitID) == 0x8,
              "ST_PARTY_RECRUIT_DEL.dwRecruitID offset mismatch");
static_assert(sizeof(ST_PARTY_RECRUIT_DEL_LIST) == 0x20,
              "ST_PARTY_RECRUIT_DEL_LIST size must match std::vector layout");
static_assert(sizeof(PS_RECRUIT_DELETE) == 0x4, "PS_RECRUIT_DELETE size must match bounded wire layout");
static_assert(sizeof(PS_RECRUIT_STATE) == 0x1, "PS_RECRUIT_STATE size must match bounded wire layout");
static_assert(sizeof(PS_RES_RECRUIT_DELETE) == 0x8,
              "PS_RES_RECRUIT_DELETE size must match bounded wire layout");
static_assert(offsetof(PS_RES_RECRUIT_DELETE, nResult) == 0x4,
              "PS_RES_RECRUIT_DELETE.nResult offset mismatch");
static_assert(sizeof(PS_SERVER_PARTY_RECRUIT_ADD_RES) == 0xA8,
              "PS_SERVER_PARTY_RECRUIT_ADD_RES size must match PDB");
static_assert(offsetof(PS_SERVER_PARTY_RECRUIT_ADD_RES, stRecruitInfo) == 0x0,
              "PS_SERVER_PARTY_RECRUIT_ADD_RES.stRecruitInfo offset mismatch");
static_assert(offsetof(PS_SERVER_PARTY_RECRUIT_ADD_RES, dwUCID) == 0x98,
              "PS_SERVER_PARTY_RECRUIT_ADD_RES.dwUCID offset mismatch");
static_assert(offsetof(PS_SERVER_PARTY_RECRUIT_ADD_RES, dwRecruitID) == 0x9C,
              "PS_SERVER_PARTY_RECRUIT_ADD_RES.dwRecruitID offset mismatch");
static_assert(offsetof(PS_SERVER_PARTY_RECRUIT_ADD_RES, nResult) == 0xA0,
              "PS_SERVER_PARTY_RECRUIT_ADD_RES.nResult offset mismatch");
static_assert(offsetof(PS_SERVER_PARTY_RECRUIT_ADD_RES, nRemainSec) == 0xA4,
              "PS_SERVER_PARTY_RECRUIT_ADD_RES.nRemainSec offset mismatch");
static_assert(sizeof(ST_FORCE_MEMBER) == 0x58, "ST_FORCE_MEMBER size must match PDB");
static_assert(offsetof(ST_FORCE_MEMBER, strName) == 0x4, "ST_FORCE_MEMBER.strName offset mismatch");
static_assert(offsetof(ST_FORCE_MEMBER, byLevel) == 0x2E, "ST_FORCE_MEMBER.byLevel offset mismatch");
static_assert(offsetof(ST_FORCE_MEMBER, byAwaken) == 0x30, "ST_FORCE_MEMBER.byAwaken offset mismatch");
static_assert(offsetof(ST_FORCE_MEMBER, dwProfilePhotoID) == 0x34,
              "ST_FORCE_MEMBER.dwProfilePhotoID offset mismatch");
static_assert(offsetof(ST_FORCE_MEMBER, nMapID) == 0x38, "ST_FORCE_MEMBER.nMapID offset mismatch");
static_assert(offsetof(ST_FORCE_MEMBER, nChannel) == 0x3C,
              "ST_FORCE_MEMBER.nChannel offset mismatch");
static_assert(offsetof(ST_FORCE_MEMBER, nMaxHP) == 0x40, "ST_FORCE_MEMBER.nMaxHP offset mismatch");
static_assert(offsetof(ST_FORCE_MEMBER, nHP) == 0x44, "ST_FORCE_MEMBER.nHP offset mismatch");
static_assert(offsetof(ST_FORCE_MEMBER, bLogin) == 0x48, "ST_FORCE_MEMBER.bLogin offset mismatch");
static_assert(offsetof(ST_FORCE_MEMBER, uxMapID) == 0x50,
              "ST_FORCE_MEMBER.uxMapID offset mismatch");
static_assert(sizeof(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER) == 0x58,
              "PS_SERVER_FORCE_MATCHING_ENTER_MEMBER size must match PDB");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER, strName) == 0x10,
              "PS_SERVER_FORCE_MATCHING_ENTER_MEMBER.strName offset mismatch");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER, uxMapID) == 0x40,
              "PS_SERVER_FORCE_MATCHING_ENTER_MEMBER.uxMapID offset mismatch");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER, nExp) == 0x48,
              "PS_SERVER_FORCE_MATCHING_ENTER_MEMBER.nExp offset mismatch");
static_assert(offsetof(PS_SERVER_FORCE_MATCHING_ENTER_MEMBER, nState) == 0x50,
              "PS_SERVER_FORCE_MATCHING_ENTER_MEMBER.nState offset mismatch");
static_assert(sizeof(ST_FORCE_MATCHING_INFO) == 0x2D0, "ST_FORCE_MATCHING_INFO size must match PDB");
static_assert(offsetof(ST_FORCE_MATCHING_INFO, stMemberInfo) == 0x8,
              "ST_FORCE_MATCHING_INFO.stMemberInfo offset mismatch");
static_assert(offsetof(ST_FORCE_MATCHING_INFO, nRemainTick) == 0x2C8,
              "ST_FORCE_MATCHING_INFO.nRemainTick offset mismatch");
static_assert(sizeof(ST_CREATE_FORCE) == 0x28, "ST_CREATE_FORCE size must match PDB");
static_assert(offsetof(ST_CREATE_FORCE, dwLeaderUCID) == 0x4,
              "ST_CREATE_FORCE.dwLeaderUCID offset mismatch");
static_assert(offsetof(ST_CREATE_FORCE, dwMemberUCID) == 0x8,
              "ST_CREATE_FORCE.dwMemberUCID offset mismatch");
static_assert(sizeof(ST_UPDATE_FORCE_MEMBER) == 0x60,
              "ST_UPDATE_FORCE_MEMBER size must match PDB");
static_assert(offsetof(ST_UPDATE_FORCE_MEMBER, stForceMember) == 0x8,
              "ST_UPDATE_FORCE_MEMBER.stForceMember offset mismatch");
static_assert(sizeof(PS_REQ_FORCE_ENTER_SERVER) == 0x18,
              "PS_REQ_FORCE_ENTER_SERVER size must match PDB");
static_assert(offsetof(PS_REQ_FORCE_ENTER_SERVER, uxMapID) == 0x8,
              "PS_REQ_FORCE_ENTER_SERVER.uxMapID offset mismatch");
static_assert(offsetof(PS_REQ_FORCE_ENTER_SERVER, nMaxHP) == 0x10,
              "PS_REQ_FORCE_ENTER_SERVER.nMaxHP offset mismatch");
static_assert(offsetof(PS_REQ_FORCE_ENTER_SERVER, bReqForceInfo) == 0x14,
              "PS_REQ_FORCE_ENTER_SERVER.bReqForceInfo offset mismatch");
static_assert(sizeof(PS_FORCE_INFO) == 0x38, "PS_FORCE_INFO size must match PDB");
static_assert(offsetof(PS_FORCE_INFO, uxMazeID) == 0x8, "PS_FORCE_INFO.uxMazeID offset mismatch");
static_assert(offsetof(PS_FORCE_INFO, byUpdateType) == 0x10,
              "PS_FORCE_INFO.byUpdateType offset mismatch");
static_assert(offsetof(PS_FORCE_INFO, vecForceMember) == 0x18,
              "PS_FORCE_INFO.vecForceMember offset mismatch");
static_assert(sizeof(PS_CHAT_PARTY) == 0x208, "PS_CHAT_PARTY size must match PDB");
static_assert(offsetof(PS_CHAT_PARTY, dwPartyID) == 0x4, "PS_CHAT_PARTY.dwPartyID offset mismatch");
static_assert(offsetof(PS_CHAT_PARTY, szMsg) == 0x8, "PS_CHAT_PARTY.szMsg offset mismatch");
static_assert(sizeof(PS_RES_FORCE_ENTER_SERVER) == 0x98,
              "PS_RES_FORCE_ENTER_SERVER size must match PDB");
static_assert(offsetof(PS_RES_FORCE_ENTER_SERVER, stEnterMember) == 0x8,
              "PS_RES_FORCE_ENTER_SERVER.stEnterMember offset mismatch");
static_assert(offsetof(PS_RES_FORCE_ENTER_SERVER, stForceInfo) == 0x60,
              "PS_RES_FORCE_ENTER_SERVER.stForceInfo offset mismatch");
