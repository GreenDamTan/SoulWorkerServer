#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerItem.h"

// ============================================================================
// PSServerFriend.h - 好友/黑名单/助战相关结构体及序列化运算符
//
// 此文件包含好友系统、黑名单系统、助战系统相关的结构体定义：
// - 好友请求/响应结构（添加、删除、接受、查找）
// - 黑名单结构（添加、删除）
// - 助战系统结构（支持信息、装备、奖励）
// - 招募系统结构
// - 每日任务好友相关结构
// ============================================================================

// ============================================================================
// 好友/黑名单基础结构体（DB 格式）
// 注意: UserObject.h 定义了客户端格式的结构体（无 padding），这些是 DB 格式
// ============================================================================

struct PS_REQ_FRIEND_DELETE {
    std::uint32_t dwReqID = 0;
    std::uint32_t dwFriendID = 0;
};

// 对齐 IDA 0x140042290: 好友接受请求
struct PS_REQ_FRIEND_ACCEPT {
    std::uint32_t dwReqUCID = 0;
    std::uint32_t dwTargetUCID = 0;
    wchar_t strTargetUserName[21] = {};
    bool bAccept = false;
};

// 对齐 IDA 0x1400DB4F0: 黑名单添加请求
struct PS_REQ_FRIEND_BLOCK_ADD {
    std::uint32_t dwReqUCID = 0;
    wchar_t strTargetName[21] = {};
};

// 对齐 IDA: 黑名单删除请求（与 BLOCK_ADD 布局相同）
struct PS_REQ_FRIEND_BLOCK_DELETE {
    std::uint32_t dwReqUCID = 0;
    wchar_t strTargetName[21] = {};
};

// 对齐 IDA 0x1400E1AD0: 好友查找请求
struct PS_REQ_FRIEND_FIND {
    std::uint32_t dwReqUCID = 0;
    wchar_t strName[21] = {};
};

// DB 格式的黑名单条目（使用 DB_ 前缀避免与 UserObject.h 冲突）
struct DB_BLOCK_INFO {
    std::uint32_t dwUCID = 0;
    wchar_t strName[21] = {};
    std::uint8_t byLevel = 0;
};

struct PS_DB_FRIEND {
    std::uint32_t dwUCID = 0;
    wchar_t strName[21] = {};
    std::uint8_t byLevel = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad0 = 0;
    std::uint32_t dwProfilePhotoID = 0;
    std::uint8_t byState = 0;
    std::uint8_t _pad1 = 0;
    wchar_t strMemo[31] = {};
    std::uint8_t byType = 0;
    std::uint8_t _pad2[7] = {};
    std::int64_t nFriendPoint = 0;
    std::int64_t tLogOut = 0;
    std::int64_t tRemain = 0;
};

struct PS_DB_FRIEND_LIST {
    std::uint32_t dwActorID = 0;
    std::uint8_t _pad0[4] = {};
    std::vector<PS_DB_FRIEND> vecFriend;
};

// DB 格式的黑名单列表（使用 DB_ 前缀）
struct DB_BLOCKLIST_INFO {
    std::vector<DB_BLOCK_INFO> vecBlockList;
};

struct ST_CHAR_COMMUNITY {
    std::uint8_t byState = 0;
    std::uint8_t _pad0 = 0;
    wchar_t szComment[51] = {};
    wchar_t szMemo[31] = {};
};

struct ST_DB_FRIEND_ADD {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t byType = 0;
    std::uint8_t _pad0[7] = {};
    std::int64_t tRemain = 0;
};

// DB 格式的好友信息（使用 DB_ 前缀避免与 UserObject.h 冲突）
struct DB_FRIEND_INFO {
    wchar_t strName[21] = {};
    std::uint8_t _pad0[2] = {};
    std::uint32_t dwID = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad1 = 0;
    std::uint32_t dwProfilePhotoID = 0;
    std::uint8_t byType = 0;
    std::uint8_t byState = 0;
    wchar_t strMemo[31] = {};
    std::uint8_t byChannel = 0;
    std::uint8_t _pad2 = 0;
    std::uint16_t wMapID = 0;
    std::uint8_t _pad3[4] = {};
    std::int64_t nFriendPoint = 0;
    bool bLogin = false;
    std::uint8_t _pad4[7] = {};
    std::int64_t tLogOut = 0;
    std::int64_t tRemain = 0;
};

struct PS_DB_FRIEND_INVITE {
    PS_REQ_FRIEND_DELETE stDeleteReq{};
    PS_REQ_FRIEND_DELETE stDeleteTarget{};
    ST_DB_FRIEND_ADD stReq{};
    ST_DB_FRIEND_ADD stTarget{};
};

struct PS_RES_DB_FRIEND_INVITE {
    PS_DB_FRIEND_INVITE stInvite{};
    DB_FRIEND_INFO stTargetInfo{};
};

struct PS_DB_FRIEND_DELETE {
    int nResult = 0;
    std::uint32_t dwReqUAID = 0;
    std::uint32_t dwReqUCID = 0;
    std::uint32_t dwFriendUAID = 0;
    std::uint32_t dwFriendUCID = 0;
};

struct PS_DB_FRIEND_ACCEPT_RES {
    ST_DB_FRIEND_ADD stReq{};
    ST_DB_FRIEND_ADD stTarget{};
    DB_FRIEND_INFO stTargetInfo{};
    int nResult = 0;
};

// 对齐 IDA: 好友接受请求（发送到 DB）
struct PS_DB_FRIEND_ACCEPT_REQ {
    ST_DB_FRIEND_ADD stReq{};
    ST_DB_FRIEND_ADD stTarget{};
};

// 对齐 IDA: 好友接受/拒绝响应（发送给客户端）
struct PS_RES_FRIEND_ACCEPT {
    int nResult = 0;
    DB_FRIEND_INFO stFriend{};  // 好友信息（使用 DB_FRIEND_INFO 避免交叉依赖）
};

// 对齐 IDA: 好友删除响应（发送给客户端）
struct PS_RES_FRIEND_DELETE {
    std::uint32_t dwReqID = 0;
    std::uint32_t dwFriendID = 0;
    std::uint8_t byUsePopup = 0;
    std::uint8_t _pad0[3] = {};
    int nResult = 0;
};

// 对齐 IDA: 好友操作结果（发送给客户端）
struct PS_FRIEND_RESULT {
    int nResult = 0;
    wchar_t strName[21] = {};
};

// 对齐 IDA: 黑名单添加响应（发送给客户端）
struct PS_RES_BLOCKLIST_ADD {
    std::uint32_t dwReqUAID = 0;
    DB_BLOCK_INFO stBlock{};  // 黑名单信息（使用 DB_BLOCK_INFO 避免交叉依赖）
    PS_FRIEND_RESULT stResult{};
};

struct PS_RES_DB_FRIEND_BLOCK {
    int nResult = 0;
    std::uint32_t dwReqUAID = 0;
    DB_BLOCK_INFO stBlock{};
};

struct PS_RES_BLOCKLIST_DELETE {
    int nResult = 0;
    std::uint32_t dwReqUAID = 0;
    std::uint32_t dwTargetUCID = 0;
    wchar_t strTargetName[21] = {};
};

// 好友邀请响应（邀请检查用）
struct PS_RES_FRIEND_INVITE {
    std::uint32_t dwReqUCID = 0;
    wchar_t strReqUserName[21] = {};
    std::uint8_t _pad0[2] = {};
    std::uint32_t dwTargetUCID = 0;
    wchar_t strTargetUserName[21] = {};
    std::uint8_t byResult = 0;  // 对齐 IDA: 邀请结果码
    std::uint8_t _pad1 = 0;
};

struct ST_FIND_FRIEND {
    std::uint32_t dwUCID = 0;
    wchar_t strName[21] = {};
    std::uint8_t byLevel = 0;
    std::uint8_t byChannel = 0;
    std::uint16_t wMapID = 0;
    bool bLogin = false;
};

struct PS_FIND_FRIEND_LIST {
    std::vector<ST_FIND_FRIEND> vecList;
    bool bLast = false;
    std::uint8_t _pad0[7] = {};
};

struct PS_DB_FRIEND_FIND {
    int nResult = 0;
    std::uint32_t dwReqUCID = 0;
    PS_FIND_FRIEND_LIST psList{};
};

struct ST_RECRUIT_INFO {
    wchar_t strName[21] = {};
    std::uint8_t _pad0[2] = {};
    std::uint32_t dwID = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad1 = 0;
    std::uint32_t dwProfilePhotoID = 0;
    std::uint8_t byState = 0;
    std::uint8_t _pad2 = 0;
    wchar_t strMemo[31] = {};
    std::uint8_t byChannel = 0;
    std::uint8_t _pad3 = 0;
    std::uint16_t wMapID = 0;
    bool bLogin = false;
    std::uint8_t _pad4[3] = {};
    std::int64_t tLogOut = 0;
    std::int64_t tAddTime = 0;
};

struct ST_RECRUIT_LIST {
    std::vector<ST_RECRUIT_INFO> vecRecruit;
};

struct PS_RES_RECRUIT_ADD {
    int nResult = 0;
    std::uint8_t _pad0[4] = {};
    ST_RECRUIT_INFO stAdd{};
};

// 对齐 IDA 0x1400B8480: 招募添加请求
struct PS_RECRUIT_ADD {
    std::uint32_t dwUCID = 0;
};

// 对齐 IDA 0x1400B8E90: 招募列表响应
struct PS_RES_RECRUIT_LIST {
    int nResult = 0;
    ST_RECRUIT_LIST stList{};
};

// 对齐 IDA 0x140044CE0: 好友推荐信息
struct ST_RECOMMAND_FRIEND_INFO {
    wchar_t strName[21] = {};
    std::uint32_t dwID = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint32_t dwProfilePhotoID = 0;
    std::uint16_t wMapID = 0;
    std::uint8_t byChannel = 0;
    bool bLogin = false;
};

// 对齐 IDA 0x1400E2180: 好友推荐响应
struct PS_RES_FRIEND_RECOMMAND {
    std::uint32_t dwReqUCID = 0;
    std::vector<ST_RECOMMAND_FRIEND_INFO> vecFriends;
};

// 对齐 IDA 0x140042250: 招募列表请求（客户端格式，区别于 DB 格式 ST_RECRUIT_LIST）
// 注意: IDA 混合名为 PS_RECRUIT_LIST（不是 PS_RECRUIT_LIST）

// 对齐 IDA: 其他角色信息请求（16 bytes）
struct PS_DB_CHARACTER_INFO_OTHER_REQ {
    std::uint32_t dwUCID = 0;           // +0x00: 查询发起者角色ID
    std::uint32_t dwTargetUCID = 0;     // +0x04: 目标角色ID
    bool bCalculateStat = false;        // +0x08: 是否计算属性
    std::uint8_t _pad0[3] = {};         // padding
    std::int32_t nError = 0;            // +0x0C: 错误码
};

static_assert(sizeof(PS_DB_CHARACTER_INFO_OTHER_REQ) == 16, "PS_DB_CHARACTER_INFO_OTHER_REQ size must match IDA");

inline XPacket& operator>>(XPacket& packet, PS_DB_CHARACTER_INFO_OTHER_REQ& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwTargetUCID;
    packet.XParse >> value.bCalculateStat;
    packet.XParse >> value.nError;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_CHARACTER_INFO_OTHER_REQ& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwTargetUCID;
    packet.XParse << value.bCalculateStat;
    packet.XParse << value.nError;
    return packet;
}

struct PS_RECRUIT_LIST {
    std::uint32_t dwUCID = 0;
    std::uint8_t byLevelMin = 0;
    std::uint8_t byLevelMax = 0;
    std::uint8_t byClass = 0;
};

// 对齐 IDA 0x1400421E0: 每日任务好友请求
struct PS_DAILY_MISSION_FRIEND_REQ {
    std::uint32_t dwReqID = 0;
    std::uint8_t byReqClass = 0;
    std::uint8_t byReqLevel = 0;
    std::uint32_t dwTargetID = 0;
    std::vector<std::uint32_t> vecMission;
};

// 对齐 IDA: 每日任务好友响应条目（与 ST_LEAGUE_OPEN 布局相同）
struct ST_DAILY_MISSION_FRIEND_RES {
    std::uint32_t dwMissionID = 0;
    std::uint8_t byCheckResult = 0;
};

// 对齐 IDA 0x1400E3B90: 每日任务好友响应
struct PS_DAILY_MISSION_FRIEND_RES {
    std::uint32_t dwReqID = 0;
    std::uint32_t dwTargetID = 0;
    std::vector<ST_DAILY_MISSION_FRIEND_RES> vecMission;
};

// 对齐 IDA 0x14002EDD0: 助战支持信息
struct ST_HELPER_SUPPORT_INFO {
    std::uint32_t dwFriendUCID = 0;
    std::uint8_t bySupportType = 0;
    float fVal = 0.0f;
    std::int64_t nDate = 0;
};

// 对齐 IDA: 助战信息响应
struct PS_HELPER_SUPPORT_INFO_RES {
    std::uint8_t bRegister = 0;
    std::uint8_t byRewardType = 0;
    ST_HELPER_SUPPORT_INFO stInfo{};
};

// 对齐 IDA: 助战列表条目
struct ST_HELPER_SUPPORT_ENTRY {
    ST_HELPER_SUPPORT_INFO stInfo{};
    std::uint8_t byRewardState = 0;
    wchar_t strName[21] = {};
    std::uint8_t byLevel = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
};

// 对齐 IDA: 助战列表响应
struct PS_HELPER_SUPPORT_LIST_RES {
    std::uint8_t byCount = 0;
    ST_HELPER_SUPPORT_ENTRY stEntries[3]{};
};

// 对齐 IDA: 助战装备响应
struct PS_HELPER_SUPPORT_EQUIP_RES {
    int nResult = 0;
    ST_HELPER_SUPPORT_INFO stInfo{};
    std::uint16_t wFriendPointReward = 0;
};

// 对齐 IDA 0x1400421B0: 助战注册请求
struct PS_SERVER_HELPER_SUPPORT_REGISTER {
    ST_HELPER_SUPPORT_INFO stInfo{};
    int nResult = 0;
};

// Note: ST_CREATE_ITEM is now defined in PSServerItem.h

// ============================================================================
// Additional packet structures for CGocFriend implementation
// ============================================================================

// 对齐 IDA 0x140087C80: 好友邀请请求
struct PS_REQ_FRIEND_INVITE {
    wchar_t strName[21] = {};
};

// 对齐 IDA 0x1400882F0: 好友删除请求
struct PS_FRIEND_DELETE {
    std::uint32_t dwFriendID = 0;
    bool bOnMsg = false;
};

// 对齐 IDA 0x140088460: 黑名单添加请求
struct PS_FRIEND_BLOCK_ADD {
    wchar_t strName[21] = {};
};

// 对齐 IDA 0x1400886B0: 黑名单删除请求
struct PS_FRIEND_BLOCK_DELETE {
    wchar_t strName[21] = {};
};

// 对齐 IDA 0x140042160: 助战奖励请求
struct PS_SERVER_HELPER_SUPPORT_REWARD {
    std::uint32_t dwUCID = 0;
    std::uint16_t wFriendPoint = 0;
    std::vector<ST_CREATE_ITEM> vecItems;
    int nResult = 0;
};

// 对齐 IDA: 助战装备请求（共享 PS_REQ_LEAGUE_DELEGATE 的 operator>>）
struct PS_HELPER_SUPPORT_EQUIP_REQ {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwFriendUCID = 0;
    std::uint32_t dwReserved = 0;
};

// 对齐 IDA 0x140062CF0: DB 返回的助战装备结果
struct PS_DB_HELPER_SUPPORT_EQUIP {
    // 基类字段（与 PS_HELPER_SUPPORT_EQUIP_REQ 布局相同: 3 个 uint32）
    std::uint32_t dwUCID = 0;
    std::uint32_t dwFriendUCID = 0;
    std::uint32_t dwReserved = 0;
    // DB 返回的助战信息
    ST_HELPER_SUPPORT_INFO stSupport{};
    // DB 返回的友情点奖励
    std::uint16_t wFriendPointReward = 0;
    // DB 返回结果
    int nResult = 0;
};

// ============================================================================
// 好友/黑名单结构体序列化运算符（DB 格式）
// ============================================================================

inline XPacket& operator<<(XPacket& packet, const DB_BLOCK_INFO& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.byLevel;
    return packet;
}

inline void operator>>(XPacket& packet, DB_BLOCK_INFO& value) {
    short outLen = 0;
    packet.XParse >> value.dwUCID;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.byLevel;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_FRIEND& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.byLevel;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.byState;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMemo);
    packet.XParse << value.byType;
    packet.XParse << static_cast<std::uint64_t>(0);
    packet.XParse << value.nFriendPoint;
    packet.XParse << value.tLogOut;
    packet.XParse << value.tRemain;
    return packet;
}

inline void operator>>(XPacket& packet, PS_DB_FRIEND& value) {
    short outLen = 0;
    packet.XParse >> value.dwUCID;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.byLevel;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse.GetBYTE();
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.byState;
    packet.XParse.GetBYTE();
    packet.XParse.GetWString(value.strMemo, 31, outLen);
    packet.XParse >> value.byType;
    packet.XParse.GetDWORD();  // _pad2 alignment
    packet.XParse >> value.nFriendPoint;
    packet.XParse >> value.tLogOut;
    packet.XParse >> value.tRemain;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_FRIEND_LIST& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << static_cast<std::uint32_t>(0);
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecFriend.size(), 0xFFFF));
    packet.XParse << count;
    for (std::size_t i = 0; i < count; ++i) {
        packet << value.vecFriend[i];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_DB_FRIEND_LIST& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse.GetDWORD();
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecFriend.clear();
    value.vecFriend.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        PS_DB_FRIEND item{};
        packet >> item;
        value.vecFriend.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const DB_BLOCKLIST_INFO& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecBlockList.size(), 0xFFFF));
    packet.XParse << count;
    for (std::size_t i = 0; i < count; ++i) {
        packet << value.vecBlockList[i];
    }
    return packet;
}

inline void operator>>(XPacket& packet, DB_BLOCKLIST_INFO& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecBlockList.clear();
    value.vecBlockList.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        DB_BLOCK_INFO item{};
        packet >> item;
        value.vecBlockList.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_CHAR_COMMUNITY& value) {
    packet.XParse << value.byState;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << GreenDamTan_BoundedWideString(value.szComment);
    packet.XParse << GreenDamTan_BoundedWideString(value.szMemo);
    return packet;
}

inline void operator>>(XPacket& packet, ST_CHAR_COMMUNITY& value) {
    short outLen = 0;
    packet.XParse >> value.byState;
    packet.XParse.GetBYTE();
    packet.XParse.GetWString(value.szComment, 51, outLen);
    packet.XParse.GetWString(value.szMemo, 31, outLen);
}

inline XPacket& operator<<(XPacket& packet, const ST_DB_FRIEND_ADD& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.byType;
    packet.XParse << static_cast<std::uint64_t>(0);
    packet.XParse << value.tRemain;
    return packet;
}

inline void operator>>(XPacket& packet, ST_DB_FRIEND_ADD& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byType;
    packet.XParse.GetQWORD();
    packet.XParse >> value.tRemain;
}

inline XPacket& operator<<(XPacket& packet, const DB_FRIEND_INFO& value) {
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << static_cast<std::uint16_t>(0);
    packet.XParse << value.dwID;
    packet.XParse << value.byLevel;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.byType;
    packet.XParse << value.byState;
    packet.XParse << GreenDamTan_BoundedWideString(value.strMemo);
    packet.XParse << value.byChannel;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.wMapID;
    packet.XParse << static_cast<std::uint32_t>(0);
    packet.XParse << value.nFriendPoint;
    packet.XParse << value.bLogin;
    packet.XParse << static_cast<std::uint64_t>(0);
    packet.XParse << value.tLogOut;
    packet.XParse << value.tRemain;
    return packet;
}

inline void operator>>(XPacket& packet, DB_FRIEND_INFO& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse.GetWORD();
    packet.XParse >> value.dwID;
    packet.XParse >> value.byLevel;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse.GetBYTE();
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.byType;
    packet.XParse >> value.byState;
    packet.XParse.GetWString(value.strMemo, 31, outLen);
    packet.XParse >> value.byChannel;
    packet.XParse.GetBYTE();
    packet.XParse >> value.wMapID;
    packet.XParse.GetDWORD();
    packet.XParse >> value.nFriendPoint;
    packet.XParse >> value.bLogin;
    packet.XParse.GetQWORD();
    packet.XParse >> value.tLogOut;
    packet.XParse >> value.tRemain;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_FRIEND_INVITE& value) {
    packet.XParse << value.dwReqUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strReqUserName);
    packet.XParse << static_cast<std::uint16_t>(0);
    packet.XParse << value.dwTargetUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strTargetUserName);
    packet.XParse << value._pad1;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_FRIEND_INVITE& value) {
    short outLen = 0;
    packet.XParse >> value.dwReqUCID;
    packet.XParse.GetWString(value.strReqUserName, 21, outLen);
    packet.XParse.GetWORD();
    packet.XParse >> value.dwTargetUCID;
    packet.XParse.GetWString(value.strTargetUserName, 21, outLen);
    packet.XParse >> value._pad1;
}

// PS_REQ_FRIEND_DELETE operators need to be defined before PS_DB_FRIEND_INVITE uses them
inline XPacket& operator<<(XPacket& packet, const PS_REQ_FRIEND_DELETE& value) {
    packet.XParse << value.dwReqID;
    packet.XParse << value.dwFriendID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_FRIEND_DELETE& value) {
    packet.XParse >> value.dwReqID;
    packet.XParse >> value.dwFriendID;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_FRIEND_INVITE& value) {
    packet << value.stDeleteReq;
    packet << value.stDeleteTarget;
    packet << value.stReq;
    packet << value.stTarget;
    return packet;
}

inline void operator>>(XPacket& packet, PS_DB_FRIEND_INVITE& value) {
    packet >> value.stDeleteReq;
    packet >> value.stDeleteTarget;
    packet >> value.stReq;
    packet >> value.stTarget;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_DB_FRIEND_INVITE& value) {
    packet << value.stInvite;
    packet << value.stTargetInfo;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_DB_FRIEND_INVITE& value) {
    packet >> value.stInvite;
    packet >> value.stTargetInfo;
}

// PS_DB_FRIEND_ACCEPT_REQ 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const PS_DB_FRIEND_ACCEPT_REQ& value) {
    packet << value.stReq;
    packet << value.stTarget;
    return packet;
}

inline void operator>>(XPacket& packet, PS_DB_FRIEND_ACCEPT_REQ& value) {
    packet >> value.stReq;
    packet >> value.stTarget;
}

// PS_RES_FRIEND_ACCEPT 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const PS_RES_FRIEND_ACCEPT& value) {
    packet.XParse << value.nResult;
    packet << value.stFriend;
    return packet;
}

// PS_FRIEND_RESULT 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const PS_FRIEND_RESULT& value) {
    packet.XParse << value.nResult;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    return packet;
}

// PS_RES_BLOCKLIST_ADD 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const PS_RES_BLOCKLIST_ADD& value) {
    packet.XParse << value.dwReqUAID;
    packet << value.stBlock;
    packet << value.stResult;
    return packet;
}

// PS_RES_FRIEND_DELETE 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const PS_RES_FRIEND_DELETE& value) {
    packet.XParse << value.dwReqID;
    packet.XParse << value.dwFriendID;
    packet.XParse << value.byUsePopup;
    packet.XParse << value.nResult;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_FRIEND_DELETE& value) {
    packet.XParse << value.nResult;
    packet.XParse << value.dwReqUAID;
    packet.XParse << value.dwReqUCID;
    packet.XParse << value.dwFriendUAID;
    packet.XParse << value.dwFriendUCID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_DB_FRIEND_DELETE& value) {
    packet.XParse >> value.nResult;
    packet.XParse >> value.dwReqUAID;
    packet.XParse >> value.dwReqUCID;
    packet.XParse >> value.dwFriendUAID;
    packet.XParse >> value.dwFriendUCID;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_FRIEND_ACCEPT_RES& value) {
    packet << value.stReq;
    packet << value.stTarget;
    packet << value.stTargetInfo;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_DB_FRIEND_ACCEPT_RES& value) {
    packet >> value.stReq;
    packet >> value.stTarget;
    packet >> value.stTargetInfo;
    packet.XParse >> value.nResult;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_DB_FRIEND_BLOCK& value) {
    packet.XParse << value.nResult;
    packet.XParse << value.dwReqUAID;
    packet << value.stBlock;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_DB_FRIEND_BLOCK& value) {
    packet.XParse >> value.nResult;
    packet.XParse >> value.dwReqUAID;
    packet >> value.stBlock;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_BLOCKLIST_DELETE& value) {
    packet.XParse << value.nResult;
    packet.XParse << value.dwReqUAID;
    packet.XParse << value.dwTargetUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strTargetName);
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_BLOCKLIST_DELETE& value) {
    short outLen = 0;
    packet.XParse >> value.nResult;
    packet.XParse >> value.dwReqUAID;
    packet.XParse >> value.dwTargetUCID;
    packet.XParse.GetWString(value.strTargetName, 21, outLen);
}

inline XPacket& operator<<(XPacket& packet, const ST_FIND_FRIEND& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.byLevel;
    packet.XParse << value.byChannel;
    packet.XParse << value.wMapID;
    packet.XParse << value.bLogin;
    return packet;
}

inline void operator>>(XPacket& packet, ST_FIND_FRIEND& value) {
    short outLen = 0;
    packet.XParse >> value.dwUCID;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.byLevel;
    packet.XParse >> value.byChannel;
    packet.XParse >> value.wMapID;
    packet.XParse >> value.bLogin;
}

inline XPacket& operator<<(XPacket& packet, const PS_FIND_FRIEND_LIST& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecList.size(), 0xFFFF));
    packet.XParse << count;
    for (std::size_t i = 0; i < count; ++i) {
        packet << value.vecList[i];
    }
    packet.XParse << value.bLast;
    return packet;
}

inline void operator>>(XPacket& packet, PS_FIND_FRIEND_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecList.clear();
    value.vecList.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        ST_FIND_FRIEND item{};
        packet >> item;
        value.vecList.push_back(item);
    }
    packet.XParse >> value.bLast;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_FRIEND_FIND& value) {
    packet.XParse << value.nResult;
    packet.XParse << value.dwReqUCID;
    packet << value.psList;
    return packet;
}

inline void operator>>(XPacket& packet, PS_DB_FRIEND_FIND& value) {
    packet.XParse >> value.nResult;
    packet.XParse >> value.dwReqUCID;
    packet >> value.psList;
}

inline XPacket& operator<<(XPacket& packet, const ST_RECRUIT_INFO& value) {
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << static_cast<std::uint16_t>(0);
    packet.XParse << value.dwID;
    packet.XParse << value.byLevel;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.byState;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMemo);
    packet.XParse << value.byChannel;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.wMapID;
    packet.XParse << value.bLogin;
    packet.XParse << static_cast<std::uint64_t>(0);
    packet.XParse << value.tLogOut;
    packet.XParse << value.tAddTime;
    return packet;
}

inline void operator>>(XPacket& packet, ST_RECRUIT_INFO& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse.GetWORD();
    packet.XParse >> value.dwID;
    packet.XParse >> value.byLevel;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse.GetBYTE();
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.byState;
    packet.XParse.GetBYTE();
    packet.XParse.GetWString(value.strMemo, 31, outLen);
    packet.XParse >> value.byChannel;
    packet.XParse.GetBYTE();
    packet.XParse >> value.wMapID;
    packet.XParse >> value.bLogin;
    packet.XParse.GetQWORD();
    packet.XParse >> value.tLogOut;
    packet.XParse >> value.tAddTime;
}

inline XPacket& operator<<(XPacket& packet, const ST_RECRUIT_LIST& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecRecruit.size(), 0xFFFF));
    packet.XParse << count;
    for (std::size_t i = 0; i < count; ++i) {
        packet << value.vecRecruit[i];
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_RECRUIT_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecRecruit.clear();
    value.vecRecruit.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        ST_RECRUIT_INFO item{};
        packet >> item;
        value.vecRecruit.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_RECRUIT_ADD& value) {
    packet.XParse << value.nResult;
    packet.XParse << static_cast<std::uint32_t>(0);
    packet << value.stAdd;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_RECRUIT_ADD& value) {
    packet.XParse >> value.nResult;
    packet.XParse.GetDWORD();
    packet >> value.stAdd;
}

// 对齐 IDA 0x1400B8480: PS_RECRUIT_ADD 序列化
inline XPacket& operator<<(XPacket& packet, const PS_RECRUIT_ADD& value) {
    packet.XParse << value.dwUCID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RECRUIT_ADD& value) {
    packet.XParse >> value.dwUCID;
}

// 对齐 IDA 0x1400B8E90: PS_RES_RECRUIT_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_RES_RECRUIT_LIST& value) {
    packet.XParse << value.nResult;
    packet << value.stList;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_RECRUIT_LIST& value) {
    packet.XParse >> value.nResult;
    packet >> value.stList;
}

// ============================================================================
// 对齐 IDA: 好友相关结构体的序列化运算符
// ============================================================================

// PS_REQ_FRIEND_ACCEPT 对齐 IDA 0x1400E1140
inline void operator>>(XPacket& packet, PS_REQ_FRIEND_ACCEPT& value) {
    short outLen = 0;
    packet.XParse >> value.dwReqUCID;
    packet.XParse >> value.dwTargetUCID;
    packet.XParse.GetWString(value.strTargetUserName, 21, outLen);
    packet.XParse >> value.bAccept;
}

// PS_REQ_FRIEND_BLOCK_ADD 对齐 IDA 0x1400DB4F0
inline XPacket& operator<<(XPacket& packet, const PS_REQ_FRIEND_BLOCK_ADD& value) {
    packet.XParse << value.dwReqUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strTargetName);
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_FRIEND_BLOCK_ADD& value) {
    short outLen = 0;
    packet.XParse >> value.dwReqUCID;
    packet.XParse.GetWString(value.strTargetName, 21, outLen);
}

// PS_REQ_FRIEND_BLOCK_DELETE 与 BLOCK_ADD 布局相同
inline XPacket& operator<<(XPacket& packet, const PS_REQ_FRIEND_BLOCK_DELETE& value) {
    packet.XParse << value.dwReqUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strTargetName);
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_FRIEND_BLOCK_DELETE& value) {
    short outLen = 0;
    packet.XParse >> value.dwReqUCID;
    packet.XParse.GetWString(value.strTargetName, 21, outLen);
}

// PS_REQ_FRIEND_FIND 对齐 IDA 0x1400E1AD0
inline XPacket& operator<<(XPacket& packet, const PS_REQ_FRIEND_FIND& value) {
    packet.XParse << value.dwReqUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_FRIEND_FIND& value) {
    short outLen = 0;
    packet.XParse >> value.dwReqUCID;
    packet.XParse.GetWString(value.strName, 21, outLen);
}

// ST_RECOMMAND_FRIEND_INFO 对齐 IDA 0x1400E1EA0/0x1400E1FC0
inline XPacket& operator<<(XPacket& packet, const ST_RECOMMAND_FRIEND_INFO& value) {
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.dwID;
    packet.XParse << value.byLevel;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.wMapID;
    packet.XParse << value.byChannel;
    packet.XParse << value.bLogin;
    return packet;
}

inline void operator>>(XPacket& packet, ST_RECOMMAND_FRIEND_INFO& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.dwID;
    packet.XParse >> value.byLevel;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.wMapID;
    packet.XParse >> value.byChannel;
    packet.XParse >> value.bLogin;
}

// PS_RES_FRIEND_RECOMMAND 对齐 IDA 0x1400E2180
inline XPacket& operator<<(XPacket& packet, const PS_RES_FRIEND_RECOMMAND& value) {
    packet.XParse << value.dwReqUCID;
    const std::uint8_t count = static_cast<std::uint8_t>(std::min<std::size_t>(value.vecFriends.size(), 255));
    packet.XParse << count;
    for (std::size_t i = 0; i < count; ++i) {
        packet << value.vecFriends[i];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_FRIEND_RECOMMAND& value) {
    std::uint8_t count = 0;
    packet.XParse >> value.dwReqUCID;
    packet.XParse >> count;
    value.vecFriends.clear();
    value.vecFriends.reserve(count);
    for (std::uint8_t i = 0; i < count; ++i) {
        ST_RECOMMAND_FRIEND_INFO item{};
        packet >> item;
        value.vecFriends.push_back(item);
    }
}

// PS_RECRUIT_LIST 对齐 IDA 0x1400E1930（客户端请求格式）
inline XPacket& operator<<(XPacket& packet, const PS_RECRUIT_LIST& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.byLevelMin;
    packet.XParse << value.byLevelMax;
    packet.XParse << value.byClass;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RECRUIT_LIST& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byLevelMin;
    packet.XParse >> value.byLevelMax;
    packet.XParse >> value.byClass;
}

// PS_DAILY_MISSION_FRIEND_REQ 对齐 IDA 0x1400E39A0/0x1400E3AA0
inline XPacket& operator<<(XPacket& packet, const PS_DAILY_MISSION_FRIEND_REQ& value) {
    packet.XParse << value.dwReqID;
    packet.XParse << value.byReqClass;
    packet.XParse << value.byReqLevel;
    packet.XParse << value.dwTargetID;
    const std::int16_t count = static_cast<std::int16_t>(std::min<std::size_t>(value.vecMission.size(), 0x7FFF));
    packet.XParse << count;
    for (std::int16_t i = 0; i < count; ++i) {
        packet.XParse << value.vecMission[static_cast<std::size_t>(i)];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_DAILY_MISSION_FRIEND_REQ& value) {
    std::int16_t count = 0;
    packet.XParse >> value.dwReqID;
    packet.XParse >> value.byReqClass;
    packet.XParse >> value.byReqLevel;
    packet.XParse >> value.dwTargetID;
    packet.XParse >> count;
    value.vecMission.clear();
    value.vecMission.reserve(static_cast<std::size_t>(count));
    for (std::int16_t i = 0; i < count; ++i) {
        std::uint32_t missionID = 0;
        packet.XParse >> missionID;
        value.vecMission.push_back(missionID);
    }
}

// ST_DAILY_MISSION_FRIEND_RES 对齐 IDA 0x1400E3980
inline XPacket& operator<<(XPacket& packet, const ST_DAILY_MISSION_FRIEND_RES& value) {
    packet.XParse << value.dwMissionID;
    packet.XParse << value.byCheckResult;
    return packet;
}

inline void operator>>(XPacket& packet, ST_DAILY_MISSION_FRIEND_RES& value) {
    packet.XParse >> value.dwMissionID;
    packet.XParse >> value.byCheckResult;
}

// PS_DAILY_MISSION_FRIEND_RES 对齐 IDA 0x1400E3B90/0x1400E3C50
inline XPacket& operator<<(XPacket& packet, const PS_DAILY_MISSION_FRIEND_RES& value) {
    packet.XParse << value.dwReqID;
    packet.XParse << value.dwTargetID;
    const std::int16_t count = static_cast<std::int16_t>(std::min<std::size_t>(value.vecMission.size(), 0x7FFF));
    packet.XParse << count;
    for (std::int16_t i = 0; i < count; ++i) {
        packet << value.vecMission[static_cast<std::size_t>(i)];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_DAILY_MISSION_FRIEND_RES& value) {
    std::int16_t count = 0;
    packet.XParse >> value.dwReqID;
    packet.XParse >> value.dwTargetID;
    packet.XParse >> count;
    value.vecMission.clear();
    value.vecMission.reserve(static_cast<std::size_t>(count));
    for (std::int16_t i = 0; i < count; ++i) {
        ST_DAILY_MISSION_FRIEND_RES item{};
        packet >> item;
        value.vecMission.push_back(item);
    }
}

// ST_HELPER_SUPPORT_INFO 对齐 IDA 0x1400E4250/0x1400E42D0
inline XPacket& operator<<(XPacket& packet, const ST_HELPER_SUPPORT_INFO& value) {
    packet.XParse << value.dwFriendUCID;
    packet.XParse << value.bySupportType;
    packet.XParse << value.fVal;
    packet.XParse << value.nDate;
    return packet;
}

inline void operator>>(XPacket& packet, ST_HELPER_SUPPORT_INFO& value) {
    packet.XParse >> value.dwFriendUCID;
    packet.XParse >> value.bySupportType;
    packet.XParse >> value.fVal;
    packet.XParse >> value.nDate;
}

// PS_HELPER_SUPPORT_INFO_RES 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const PS_HELPER_SUPPORT_INFO_RES& value) {
    packet.XParse << value.bRegister;
    packet.XParse << value.byRewardType;
    packet << value.stInfo;
    return packet;
}

// ST_HELPER_SUPPORT_ENTRY 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const ST_HELPER_SUPPORT_ENTRY& value) {
    packet << value.stInfo;
    packet.XParse << value.byRewardState;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.byLevel;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    return packet;
}

// PS_HELPER_SUPPORT_LIST_RES 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const PS_HELPER_SUPPORT_LIST_RES& value) {
    packet.XParse << value.byCount;
    for (int i = 0; i < value.byCount && i < 3; ++i) {
        packet << value.stEntries[i];
    }
    return packet;
}

// PS_HELPER_SUPPORT_EQUIP_RES 对齐 IDA
inline XPacket& operator<<(XPacket& packet, const PS_HELPER_SUPPORT_EQUIP_RES& value) {
    packet.XParse << value.nResult;
    packet << value.stInfo;
    packet.XParse << value.wFriendPointReward;
    return packet;
}

// PS_SERVER_HELPER_SUPPORT_REGISTER 对齐 IDA 0x1400E4660/0x1400E46A0
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_HELPER_SUPPORT_REGISTER& value) {
    packet << value.stInfo;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_SERVER_HELPER_SUPPORT_REGISTER& value) {
    packet >> value.stInfo;
    packet.XParse >> value.nResult;
}

// Note: ST_CREATE_ITEM serialization operators are now in PSServerItem.h

// PS_SERVER_HELPER_SUPPORT_REWARD 对齐 IDA 0x1400E46F0/0x1400E47D0
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_HELPER_SUPPORT_REWARD& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.wFriendPoint;
    const std::int16_t count = static_cast<std::int16_t>(std::min<std::size_t>(value.vecItems.size(), 0x7FFF));
    packet.XParse << count;
    for (std::int16_t i = 0; i < count; ++i) {
        packet << value.vecItems[static_cast<std::size_t>(i)];
    }
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_SERVER_HELPER_SUPPORT_REWARD& value) {
    std::int16_t count = 0;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.wFriendPoint;
    packet.XParse >> count;
    value.vecItems.clear();
    value.vecItems.reserve(static_cast<std::size_t>(count));
    for (std::int16_t i = 0; i < count; ++i) {
        ST_CREATE_ITEM item{};
        packet >> item;
        value.vecItems.push_back(item);
    }
    packet.XParse >> value.nResult;
}

// PS_HELPER_SUPPORT_EQUIP_REQ 使用与 PS_REQ_LEAGUE_DELEGATE 相同的 operator>>
// 对齐 IDA 0x1400E7950
inline void operator>>(XPacket& packet, PS_HELPER_SUPPORT_EQUIP_REQ& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwFriendUCID;
    packet.XParse >> value.dwReserved;
}

// PS_DB_HELPER_SUPPORT_EQUIP 对齐 IDA 0x1400E4AA0
inline XPacket& operator<<(XPacket& packet, const PS_DB_HELPER_SUPPORT_EQUIP& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwFriendUCID;
    packet.XParse << value.dwReserved;
    packet << value.stSupport;
    packet.XParse << value.wFriendPointReward;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_DB_HELPER_SUPPORT_EQUIP& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwFriendUCID;
    packet.XParse >> value.dwReserved;
    packet >> value.stSupport;
    packet.XParse >> value.wFriendPointReward;
    packet.XParse >> value.nResult;
}

// ============================================================================
// Helper (助手) 系统结构体 - 用于 XSQLHelperProcess
// ============================================================================

/**
 * 对齐 IDA: StatInfo 结构 (8 bytes)
 * 用于 vecOrigin 和 vecAddditional
 */
struct ST_HELPER_STAT_INFO {
    std::uint8_t byIndex = 0;       // +0x00
    std::uint8_t _pad0[3] = {};     // padding
    float fStatValue = 0.0f;        // +0x04
};

/**
 * 对齐 IDA STItem (120 bytes)
 * 助手物品信息结构
 */
struct ST_ITEM_HELPER {
    std::int32_t nItemID = 0;           // +0x00
    std::uint8_t _pad0[4] = {};         // +0x04 padding
    std::int64_t xSerial = 0;           // +0x08
    std::int16_t sCount = 0;            // +0x10
    std::uint8_t bBindType = 0;         // +0x12
    std::uint8_t byEndurance = 0;       // +0x13
    struct ST_EXTEND_OPTION {           // +0x14 (5 * 8 = 40 bytes)
        std::uint8_t byType = 0;        // +0x00
        std::uint8_t _pad0[3] = {};     // padding
        std::int32_t nOption = 0;       // +0x04
    } stExtendOption[5];
    std::uint8_t byUpgrade = 0;         // +0x3C (60)
    std::uint8_t eFlag = 0;             // +0x3D (61)
    std::uint8_t bySocketActiveCount = 0; // +0x3E (62)
    std::uint8_t _pad1 = {};            // +0x3F padding
    std::int64_t nCashDate = 0;         // +0x40 (64)
    std::uint8_t byUpgradeCount = 0;    // +0x48 (72)
    std::uint8_t byUpgradeLimit = 0;    // +0x49 (73)
    std::uint8_t _pad2[2] = {};         // padding
    std::int32_t nExp = 0;              // +0x4C (76)
    char szBroachState[16] = {};        // +0x50 (80)
    std::uint8_t byRestoreCount = 0;    // +0x60 (96)
    std::uint8_t bySealCount = 0;       // +0x61 (97)
    std::uint8_t bySealDelCount = 0;    // +0x62 (98)
    std::uint8_t _pad3 = {};            // padding
    std::int32_t nAttack = 0;           // +0x64 (100)
    std::int32_t nDefense = 0;          // +0x68 (104)
    std::int32_t nTitleID = 0;          // +0x6C (108)
    std::uint8_t byUseCount = 0;        // +0x70 (112)
    std::uint8_t _pad4[3] = {};         // padding
    std::int32_t nDyeID = 0;            // +0x74 (116)
};
static_assert(sizeof(ST_ITEM_HELPER) == 120, "ST_ITEM_HELPER size mismatch");

/**
 * 对齐 IDA ST_HELPER_INFO (472 bytes)
 * 助手信息
 */
struct ST_HELPER_INFO {
    std::uint32_t dwHelperID = 0;       // +0x00
    std::uint8_t _pad0[4] = {};         // +0x04 padding
    ST_ITEM_HELPER stItem[3]{};         // +0x08 (3 * 120 = 360 bytes)
    ST_HELPER_SUPPORT_INFO stFriendSupport{}; // +0x178 (368)
    bool bSummon = false;               // +0x190 (392)
    std::uint8_t _pad1[7] = {};         // padding
    std::vector<ST_HELPER_STAT_INFO> vecOrigin; // +0x198 (400)
    std::vector<ST_HELPER_STAT_INFO> vecAddditional; // +0x1B8 (432)
    std::uint8_t byOrder = 0;           // +0x1D0 (464)
    std::uint8_t _pad2[7] = {};         // padding
};
static_assert(sizeof(ST_HELPER_INFO) == 472, "ST_HELPER_INFO size mismatch");

/**
 * 对齐 IDA: StatInfoList - 助手属性列表
 * 用于 PS_HELPER_STAT_UPDATE
 */
using StatInfoList = std::vector<ST_HELPER_STAT_INFO>;

/**
 * 对齐 IDA 0x140095170: 助手属性更新
 */
struct PS_HELPER_STAT_UPDATE {
    std::map<std::uint32_t, StatInfoList> mapHelperStats; // key: HelperID, value: StatInfoList
};

/**
 * 对齐 IDA 0x14004B3E0: 助手列表响应
 */
struct PS_HELPER_LIST_RES {
    std::uint32_t dwUCID = 0;           // +0x00
    std::uint8_t byAutoSummon = 0;      // +0x04
    std::uint8_t _pad0[3] = {};         // padding
    std::vector<ST_HELPER_INFO> vecHelper; // +0x08
};

/**
 * 对齐 IDA 0x14004B7C0: 添加助手请求
 */
struct PS_HELPER_ADD_REQ {
    std::uint32_t dwUCID = 0;           // +0x00
    std::uint32_t dwHelperID = 0;       // +0x04
    std::uint8_t byOrder = 0;           // +0x08
    std::uint8_t _pad0[3] = {};         // padding
};

/**
 * 对齐 IDA 0x14004B7C0: 添加助手响应
 */
struct PS_HELPER_ADD_RES {
    std::uint32_t dwUCID = 0;           // +0x00
    std::uint32_t dwHelperID = 0;       // +0x04
    std::uint8_t byOrder = 0;           // +0x08
    std::uint8_t _pad0[3] = {};         // padding
    std::int32_t nError = 0;            // +0x0C
};

/**
 * 对齐 IDA 0x14004C5B0: 更改助手顺序请求/响应
 */
struct PS_HELPER_CHANGE_ORDER {
    std::uint32_t dwUCID = 0;           // +0x00
    std::uint32_t dwHelperID_1 = 0;     // +0x04
    std::uint8_t byOrder_1 = 0;         // +0x08
    std::uint8_t _pad0[3] = {};         // padding
    std::uint32_t dwHelperID_2 = 0;     // +0x0C
    std::uint8_t byOrder_2 = 0;         // +0x10
    std::uint8_t _pad1[3] = {};         // padding
    std::int32_t nError = 0;            // +0x14
};

/**
 * 对齐 IDA 0x14004C810: 更改自动召唤标志
 */
struct PS_HELPER_CHANGE_AUTO_SUMMON {
    std::uint32_t dwUCID = 0;           // +0x00
    std::uint8_t byFlag = 0;            // +0x04
    std::uint8_t _pad0[3] = {};         // padding
};

/**
 * 对齐 IDA 0x14004BF30: 助手装备请求
 */
struct PS_DB_HELPER_EQUIP_REQ {
    std::uint32_t dwUCID = 0;           // +0x00
    struct {
        std::uint32_t dwHelperID = 0;   // +0x04
        std::int16_t shHelperSlotPos = 0; // +0x08
        std::uint8_t _pad0[2] = {};
        std::int64_t xInvenSerial = 0;  // +0x0C
        std::int64_t xHelperSerial = 0; // +0x14
        std::int16_t shInvenSlotPos = 0; // +0x1C
        std::uint8_t byInvenType = 0;   // +0x1E
        std::uint8_t _pad1[1] = {};
    } psEquip;
};

/**
 * 对齐 IDA 0x14004BF30: 助手装备响应
 */
struct PS_DB_HELPER_EQUIP_RES {
    std::uint32_t dwHelperID = 0;       // +0x00
    std::int16_t shHelperSlotPos = 0;   // +0x04
    std::uint8_t _pad0[2] = {};
    std::int64_t xInvenSerial = 0;      // +0x08
    std::int64_t xHelperSerial = 0;     // +0x10
    std::int16_t shInvenSlotPos = 0;    // +0x18
    std::uint8_t byInvenType = 0;       // +0x1A
    std::uint8_t _pad1[1] = {};
    std::int32_t nError = 0;            // +0x1C
};

/**
 * 对齐 IDA 0x14004BC80: 助手支援释放请求
 */
struct PS_DB_HELPER_SUPPORT_RELEASE {
    std::uint32_t dwUCID = 0;           // +0x00
    std::uint32_t dwHelperID = 0;       // +0x04
    std::int32_t nResult = 0;           // +0x08
};

// ============================================================================
// Helper 系统结构体序列化运算符
// ============================================================================

// ST_HELPER_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_HELPER_INFO& value) {
    packet.XParse << value.dwHelperID;
    packet << value.stFriendSupport;
    packet.XParse << value.byOrder;
    return packet;
}

inline void operator>>(XPacket& packet, ST_HELPER_INFO& value) {
    packet.XParse >> value.dwHelperID;
    packet >> value.stFriendSupport;
    packet.XParse >> value.byOrder;
}

// PS_HELPER_LIST_RES 序列化
inline XPacket& operator<<(XPacket& packet, const PS_HELPER_LIST_RES& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.byAutoSummon;
    const std::int16_t count = static_cast<std::int16_t>(std::min<std::size_t>(value.vecHelper.size(), 0x7FFF));
    packet.XParse << count;
    for (std::int16_t i = 0; i < count; ++i) {
        packet << value.vecHelper[static_cast<std::size_t>(i)];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_HELPER_LIST_RES& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byAutoSummon;
    std::int16_t count = 0;
    packet.XParse >> count;
    value.vecHelper.clear();
    value.vecHelper.reserve(static_cast<std::size_t>(count));
    for (std::int16_t i = 0; i < count; ++i) {
        ST_HELPER_INFO item{};
        packet >> item;
        value.vecHelper.push_back(item);
    }
}

// PS_HELPER_ADD_REQ 序列化
inline void operator>>(XPacket& packet, PS_HELPER_ADD_REQ& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwHelperID;
    packet.XParse >> value.byOrder;
}

// PS_HELPER_ADD_RES 序列化
inline XPacket& operator<<(XPacket& packet, const PS_HELPER_ADD_RES& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwHelperID;
    packet.XParse << value.byOrder;
    packet.XParse << value.nError;
    return packet;
}

// PS_HELPER_CHANGE_ORDER 序列化
inline void operator>>(XPacket& packet, PS_HELPER_CHANGE_ORDER& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwHelperID_1;
    packet.XParse >> value.byOrder_1;
    packet.XParse >> value.dwHelperID_2;
    packet.XParse >> value.byOrder_2;
}

inline XPacket& operator<<(XPacket& packet, const PS_HELPER_CHANGE_ORDER& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwHelperID_1;
    packet.XParse << value.byOrder_1;
    packet.XParse << value.dwHelperID_2;
    packet.XParse << value.byOrder_2;
    packet.XParse << value.nError;
    return packet;
}

// PS_HELPER_CHANGE_AUTO_SUMMON 序列化
inline void operator>>(XPacket& packet, PS_HELPER_CHANGE_AUTO_SUMMON& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byFlag;
}

inline XPacket& operator<<(XPacket& packet, const PS_HELPER_CHANGE_AUTO_SUMMON& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.byFlag;
    return packet;
}

// PS_DB_HELPER_EQUIP_REQ 序列化
inline void operator>>(XPacket& packet, PS_DB_HELPER_EQUIP_REQ& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.psEquip.dwHelperID;
    packet.XParse >> value.psEquip.shHelperSlotPos;
    packet.XParse >> value.psEquip.xInvenSerial;
    packet.XParse >> value.psEquip.xHelperSerial;
    packet.XParse >> value.psEquip.shInvenSlotPos;
    packet.XParse >> value.psEquip.byInvenType;
}

// PS_DB_HELPER_EQUIP_RES 序列化
inline XPacket& operator<<(XPacket& packet, const PS_DB_HELPER_EQUIP_RES& value) {
    packet.XParse << value.dwHelperID;
    packet.XParse << value.shHelperSlotPos;
    packet.XParse << value.xInvenSerial;
    packet.XParse << value.xHelperSerial;
    packet.XParse << value.shInvenSlotPos;
    packet.XParse << value.byInvenType;
    packet.XParse << value.nError;
    return packet;
}

// PS_DB_HELPER_SUPPORT_RELEASE 序列化
inline void operator>>(XPacket& packet, PS_DB_HELPER_SUPPORT_RELEASE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwHelperID;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_HELPER_SUPPORT_RELEASE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwHelperID;
    packet.XParse << value.nResult;
    return packet;
}
