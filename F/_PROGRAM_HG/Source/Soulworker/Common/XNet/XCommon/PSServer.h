#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

/**
 * @brief 队伍/联盟类型枚举。
 *
 * 用于 ST_PARTY_INFO::byGroupType 判断队伍还是联盟。
 */
enum E_PARTY_GROUP_TYPE : std::uint8_t {
    E_PARTY_GROUP_TYPE_PARTY = 1,
    E_PARTY_GROUP_TYPE_FORCE = 2,
};

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

struct ST_PARTY_MEMBER {
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

struct ST_UPDATE_PARTY_MEMBER {
    std::uint32_t dwPartyID = 0;
    std::uint8_t _pad0[4] = {};
    ST_PARTY_MEMBER stPartyMember{};
};

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
// 对齐 IDA: 好友/黑名单相关结构体（DB 格式，带 padding）
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

// 对齐 IDA 0x1400E4220: 创建物品信息
struct ST_CREATE_ITEM {
    std::int32_t nItemID = 0;
    std::int16_t shCount = 0;
    std::uint8_t byUpgrade = 0;
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

// 对齐 IDA 0x1400C9290: 交易所价格历史条目
struct ST_EXCHANGE_PRICE_INFO {
    std::uint32_t dwItemID = 0;
    std::int16_t sCount = 0;
    std::uint8_t _pad0[2] = {};  // 对齐填充
    std::int64_t nPrice_One = 0;
    std::int64_t tRegDate = 0;
    wchar_t strBuyerName[21] = {};
};

// 对齐 IDA 0x1400C9920: 交易所价格历史请求（客户端请求格式）
struct PS_EXCHANGE_PRICE_HISTORY_REQ {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwItemID = 0;
};

// 对齐 IDA 0x140062D80: 交易所价格历史响应
struct PS_EXCHANGE_PRICE_HISTORY_RES {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwItemID = 0;
    std::vector<ST_EXCHANGE_PRICE_INFO> vecHistory;
    std::int64_t n64Price_High = 0;
    std::int64_t n64Price_Low = 0;
    std::int64_t n64Price_Avg = 0;
};

// 对齐 IDA 0x140062E20: DB 返回的交易所价格历史
struct PS_DB_EXCHANGE_PRICE_HISTORY_RES {
    PS_EXCHANGE_PRICE_HISTORY_RES stRes{};
    int nTotalCount = 0;
    std::int64_t n64TotalPrice = 0;
};

// 对齐 IDA: DB 请求的交易所价格历史
struct PS_DB_EXCHANGE_PRICE_HISTORY_REQ {
    PS_EXCHANGE_PRICE_HISTORY_REQ stReq{};
    std::int64_t n64Date = 0;
};

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_EXCHANGE_PRICE_HISTORY_REQ& value) {
    packet.XParse << value.stReq.dwUCID;
    packet.XParse << value.stReq.dwItemID;
    packet.XParse << value.n64Date;
    return packet;
}

// ============================================================================
// Exchange (交易所) 协议结构体 - 对齐 IDA XSQLExchange
// ============================================================================

// 对齐 IDA 0x140038880: 交易所搜索请求
struct PS_EXCHANGE_SEARCH_REQ {
    std::uint32_t dwItemID = 0;
    int nCategoryID = 0;
    int nSubCategoryID = 0;
    int nUseClass = 0;
    int nLevelMin = 0;
    int nLevelMax = 0;
    int nItemGradeMin = 0;
    int nItemGradeMax = 0;
    std::int64_t nPrice = 0;
    int nUpgrade = 0;
    int nPageNum = 0;
};

// 对齐 IDA: 交易所物品镶嵌信息
struct ST_EXCHANGE_BROACH_INFO {
    std::int64_t biSerial = 0;
    std::uint32_t dwItemID[15] = {};
};

// 对齐 IDA: 交易所物品扩展选项
struct ST_EXCHANGE_EXTEND_OPTION {
    std::uint8_t byType = 0;
    int nOption = 0;
};

// 对齐 IDA 0x140038880: 交易所物品信息
struct ST_EXCHANGE_ITEM {
    std::uint32_t dwExchangeID = 0;
    // stItem 嵌入字段（展开 STItem 布局）
    std::uint32_t dwItemID = 0;
    std::int64_t xSerial = 0;
    std::int16_t sCount = 0;
    std::uint8_t byUpgrade = 0;
    ST_EXCHANGE_EXTEND_OPTION stExtendOption[5] = {};
    std::uint8_t bySocketActiveCount = 0;
    std::uint8_t byUpgradeCount = 0;
    std::uint8_t byUpgradeLimit = 0;
    std::uint8_t byEndurance = 0;
    std::uint8_t byRestoreCount = 0;
    std::uint8_t bySealCount = 0;
    std::uint8_t bySealDelCount = 0;
    char szBroachState[16] = {};
    int nAttack = 0;
    int nDefense = 0;
    int nTitleID = 0;
    std::uint8_t byUseCount = 0;
    int nDyeID = 0;
    // 价格
    std::int64_t nPrice_One = 0;
    std::uint8_t byCash_Commission = 0;
    ST_EXCHANGE_BROACH_INFO stBroachInfo{};
    wchar_t strSellerName[21] = {};
    std::int64_t nExpireRemainTime = 0;
};

// 对齐 IDA 0x140038880: 交易所搜索响应
struct PS_EXCHANGE_SEARCH_RES {
    int nPageMax = 0;
    int nPage = 0;
    std::vector<ST_EXCHANGE_ITEM> vecItem;
};

// 对齐 IDA 0x1400399C0: 交易所关注列表请求
struct PS_EXCHANGE_INTEREST_LIST_REQ {
    std::uint32_t dwUCID = 0;
};

// 对齐 IDA 0x1400399C0: 交易所关注列表响应
struct PS_EXCHANGE_INTEREST_LIST_RES {
    std::uint32_t dwUCID = 0;
    std::vector<std::uint32_t> vecItemList;
};

// 对齐 IDA 0x14003B340: 我的交易所物品
struct ST_MY_EXCHANGE_ITEM {
    std::uint32_t dwExchangeID = 0;
    std::uint32_t dwItemID = 0;
    std::int64_t xSerial = 0;
    std::int16_t sCount = 0;
    std::uint8_t byUpgrade = 0;
    ST_EXCHANGE_EXTEND_OPTION stExtendOption[5] = {};
    std::uint8_t bySocketActiveCount = 0;
    std::uint8_t byUpgradeCount = 0;
    std::uint8_t byUpgradeLimit = 0;
    std::uint8_t byEndurance = 0;
    std::uint8_t byRestoreCount = 0;
    std::uint8_t bySealCount = 0;
    std::uint8_t bySealDelCount = 0;
    char szBroachState[16] = {};
    int nAttack = 0;
    int nDefense = 0;
    int nTitleID = 0;
    std::uint8_t byUseCount = 0;
    int nDyeID = 0;
    std::int64_t nPrice_One = 0;
    std::uint8_t byCash_Commission = 0;
    ST_EXCHANGE_BROACH_INFO stBroachInfo{};
    std::int64_t nExpireRemainTime = 0;
};

// 对齐 IDA 0x14003B340: 我的交易所列表请求
struct PS_EXCHANGE_MY_LIST_REQ {
    std::uint32_t dwUCID = 0;
};

// 对齐 IDA 0x14003B340: 我的交易所列表响应
struct PS_EXCHANGE_MY_LIST_RES {
    std::uint8_t bLast = 0;
    std::vector<ST_MY_EXCHANGE_ITEM> vecMyList;
};

// 对齐 IDA 0x14003B5A0: 交易所物品购买检查请求
struct PS_EXCHANGE_ITEM_BUY_REQ {
    std::uint32_t dwExchangeID = 0;
    std::int16_t shCount = 0;
};

// 对齐 IDA 0x14003B5A0: 交易所物品购买检查响应（DB返回）
struct PS_EXCHANGE_ITEM_BUY_RES {
    std::uint32_t dwExchangeID = 0;
    std::int16_t shCount = 0;
    std::int64_t biPrice = 0;
    int nPackageCount = 0;
};

// 对齐 IDA 0x140039BF0: 交易所关注物品请求
struct PS_EXCHANGE_INTEREST_ITEM_REQ {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwItemID = 0;
    std::uint8_t bAdd = 0;
};

// 对齐 IDA 0x140039BF0: 交易所关注物品响应
struct PS_EXCHANGE_INTEREST_ITEM_RES {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwItemID = 0;
    std::uint8_t bAdd = 0;
    int nResult = 0;
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
 * @brief Relay 广播聊天公告。
 *
 * 该结构按 PDB 与 RelayServer.exe 反编译共同恢复：
 * - `byType @ +0x0`
 * - `strMsg[256] @ +0x2`（UTF-16 字节长度 0x200）
 * - `strColor[7] @ +0x202`（UTF-16 字节长度 0x1C）
 * - `nMessageCode @ +0x210`
 */
struct PS_CHAT_NOTICE {
    std::uint8_t byType = 0;
    std::uint8_t _pad0 = 0;
    wchar_t strMsg[256] = {};
    wchar_t strColor[7] = {};
    std::int32_t nMessageCode = 0;
};

/**
 * @brief Relay 广播喇叭消息。
 */
struct PS_CHAT_MEGAPHONE {
    std::uint8_t byInvenType = 0;
    std::uint8_t _pad0 = 0;
    std::int16_t shSlot = 0;
    std::uint32_t dwUCID = 0;
    wchar_t strName[21] = {};
    wchar_t strMsg[256] = {};
};

struct ST_SOCKET_DATA {
    std::int32_t dwSocketID = 0;
    std::uint8_t bySocketPos = 0;
    std::uint8_t _pad1[3] = {};
    ST_EXTEND_OPTION stExtendOption[5] = {};
};

struct ST_ITEM_SOCKET : ST_SOCKET_DATA {
    std::int64_t biEquipSerial = 0;
};

struct PS_ITEM_SOCKET_LIST {
    std::vector<ST_ITEM_SOCKET> vecInfo;
};

struct ST_ITEM_BROACH {
    std::int64_t biSerial = 0;
    int dwItemID[15] = {};
};

// Item broach list - 对齐 IDA PS_ITEM_BROACH_LIST (32 bytes)
struct PS_ITEM_BROACH_LIST {
    std::vector<ST_ITEM_BROACH> vecInfo;
};

/**
 * @brief 属性更新项 - 8 bytes
 * 来自 IDA: ST_UPDATE_STAT
 */
struct ST_UPDATE_STAT {
    float fValue = 0.0f;
    std::uint16_t wStatID = 0;
};

static_assert(sizeof(ST_UPDATE_STAT) == 8, "ST_UPDATE_STAT size must match IDA");

/**
 * @brief 属性向量容器 - 32 bytes
 * 来自 IDA: ST_STAT_VEC
 */
struct ST_STAT_VEC {
    std::vector<ST_UPDATE_STAT> vecUpdateStat;
};

static_assert(sizeof(ST_STAT_VEC) == 32, "ST_STAT_VEC size must match IDA");

/**
 * @brief 特殊选项更新项 - 8 bytes
 * 来自 IDA: ST_UPDATE_SPECIAL_OPTION
 */
struct ST_UPDATE_SPECIAL_OPTION {
    std::uint16_t wOptionIndex = 0;
    float fValue = 0.0f;
};

static_assert(sizeof(ST_UPDATE_SPECIAL_OPTION) == 8, "ST_UPDATE_SPECIAL_OPTION size must match IDA");

/**
 * @brief 特殊选项更新列表 - 32 bytes
 * 来自 IDA: ST_UPDATE_SPECIAL_OPTION_LIST
 */
struct ST_UPDATE_SPECIAL_OPTION_LIST {
    std::vector<ST_UPDATE_SPECIAL_OPTION> vecUpdateOption;
};

static_assert(sizeof(ST_UPDATE_SPECIAL_OPTION_LIST) == 32, "ST_UPDATE_SPECIAL_OPTION_LIST size must match IDA");

/**
 * @brief 快捷栏卡片槽位 - 48 bytes
 * 来自 IDA: PS_QUICKSLOT_CARD
 */
struct PS_QUICKSLOT_CARD {
    std::uint8_t byPage = 0;
    std::uint8_t _pad0 = 0;
    wchar_t szDeckName[13] = {};
    std::uint32_t uniCard[5] = {};  // 匿名联合体的数组表示
};

static_assert(sizeof(PS_QUICKSLOT_CARD) == 48, "PS_QUICKSLOT_CARD size must match IDA");

/**
 * @brief 成就类别统计 - 14 bytes
 * 来自 IDA: ST_ACHIEVE_CATEGORY
 */
struct ST_ACHIEVE_CATEGORY {
    std::uint16_t wCount[7] = {};
};

static_assert(sizeof(ST_ACHIEVE_CATEGORY) == 14, "ST_ACHIEVE_CATEGORY size must match IDA");

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

static_assert(sizeof(ST_OTHER_CHARINFO) == 472, "ST_OTHER_CHARINFO size must match IDA");

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

static_assert(sizeof(PS_DB_CHARACTER_INFO_OTHER_RES) == 576, "PS_DB_CHARACTER_INFO_OTHER_RES size must match IDA");

// ST_STAT_VEC XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_STAT_VEC& value) {
    packet.XParse << static_cast<std::int32_t>(value.vecUpdateStat.size());
    for (const auto& stat : value.vecUpdateStat) {
        packet.XParse << stat.fValue;
        packet.XParse << stat.wStatID;
    }
    return packet;
}

// ST_UPDATE_SPECIAL_OPTION_LIST XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_UPDATE_SPECIAL_OPTION_LIST& value) {
    packet.XParse << static_cast<std::int32_t>(value.vecUpdateOption.size());
    for (const auto& opt : value.vecUpdateOption) {
        packet.XParse << opt.wOptionIndex;
        packet.XParse << opt.fValue;
    }
    return packet;
}

// ST_ACHIEVE_CATEGORY XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_ACHIEVE_CATEGORY& value) {
    for (int i = 0; i < 7; ++i) {
        packet.XParse << value.wCount[i];
    }
    return packet;
}

// ST_SOCKET_DATA XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_SOCKET_DATA& value) {
    packet.XParse << value.dwSocketID;
    packet.XParse << value.bySocketPos;
    for (const ST_EXTEND_OPTION& extendOption : value.stExtendOption) {
        packet << extendOption;
    }
    return packet;
}

// ST_ITEM_SOCKET XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_ITEM_SOCKET& value) {
    packet.XParse << value.biEquipSerial;
    packet.XParse << value.dwSocketID;
    packet.XParse << value.bySocketPos;
    for (const ST_EXTEND_OPTION& extendOption : value.stExtendOption) {
        packet << extendOption;
    }
    return packet;
}

// PS_ITEM_SOCKET_LIST XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_ITEM_SOCKET_LIST& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecInfo.size(), 0xFFFF));
    packet.XParse << count;
    for (std::size_t index = 0; index < count; ++index) {
        packet << value.vecInfo[index];
    }
    return packet;
}

// ST_ITEM_BROACH XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_ITEM_BROACH& value) {
    packet.XParse << value.biSerial;
    for (const int itemID : value.dwItemID) {
        packet.XParse << itemID;
    }
    return packet;
}

// PS_ITEM_BROACH_LIST XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_ITEM_BROACH_LIST& value) {
    std::uint16_t count = static_cast<std::uint16_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// PS_QUICKSLOT_CARD XSendDBPacket 序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_QUICKSLOT_CARD& value) {
    packet.XParse << value.byPage;
    packet.XParse << FixedWideArrayToWString(value.szDeckName);
    for (int i = 0; i < 5; ++i) {
        packet.XParse << value.uniCard[i];
    }
    return packet;
}

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

struct ST_ITEM_PACKAGE_PARTS {
    std::int64_t biSerial = 0;
    int nItemID = 0;
    int nDyeID = 0;
};

struct PS_ITEM_PACKAGE {
    std::int64_t biPackageSerial = 0;
    std::vector<ST_ITEM_PACKAGE_PARTS> vecInfo;
};

// Item package list - 对齐 IDA PS_ITEM_PACKAGE_LIST (32 bytes)
struct PS_ITEM_PACKAGE_LIST {
    std::vector<PS_ITEM_PACKAGE> vecInfo;
};

/**
 * @brief 物品制作限制信息 - 24 bytes
 * 来自 IDA: PS_ITEM_MAKE_LIMIT_INFO
 */
struct PS_ITEM_MAKE_LIMIT_INFO {
    std::int32_t nMakeIndex = 0;    // +0x00: 制作索引
    std::int32_t nItemID = 0;       // +0x04: 物品ID
    std::int16_t shCount = 0;       // +0x08: 数量
    std::uint8_t _pad0[6] = {};     // +0x0A: padding (6 bytes)
    std::int64_t biEndDate = 0;     // +0x10: 结束日期
};

static_assert(sizeof(PS_ITEM_MAKE_LIMIT_INFO) == 24, "PS_ITEM_MAKE_LIMIT_INFO size must match IDA");

/**
 * @brief 物品制作限制列表 - 40 bytes
 * 来自 IDA: PS_ITEM_MAKE_LIMIT_LIST
 */
struct PS_ITEM_MAKE_LIMIT_LIST {
    std::uint8_t byFlag = 0;                                // +0x00: 标志
    std::uint8_t _pad0[7] = {};                             // +0x01: padding (7 bytes)
    std::vector<PS_ITEM_MAKE_LIMIT_INFO> vecInfo;           // +0x08: 列表向量
};

static_assert(sizeof(PS_ITEM_MAKE_LIMIT_LIST) == 40, "PS_ITEM_MAKE_LIMIT_LIST size must match IDA");

/**
 * @brief DB层物品制作限制信息 - 32 bytes
 * 来自 IDA: PS_DB_ITEM_MAKE_LIMIT_INFO
 */
struct PS_DB_ITEM_MAKE_LIMIT_INFO {
    std::uint8_t byLimitType = 0;                           // +0x00: 限制类型
    std::uint8_t _pad0[7] = {};                             // +0x01: padding (7 bytes)
    PS_ITEM_MAKE_LIMIT_INFO psInfo{};                       // +0x08: 制作限制信息
};

static_assert(sizeof(PS_DB_ITEM_MAKE_LIMIT_INFO) == 32, "PS_DB_ITEM_MAKE_LIMIT_INFO size must match IDA");

/**
 * @brief DB层物品制作限制更新请求 - 40 bytes
 * 来自 IDA: PS_DB_ITEM_MAKE_LIMIT_UPDATE
 */
struct PS_DB_ITEM_MAKE_LIMIT_UPDATE {
    std::uint32_t dwUCID = 0;                               // +0x00: 角色ID
    std::uint32_t dwUAID = 0;                               // +0x04: 账户ID
    PS_DB_ITEM_MAKE_LIMIT_INFO psDBInfo{};                  // +0x08: DB限制信息
};

static_assert(sizeof(PS_DB_ITEM_MAKE_LIMIT_UPDATE) == 40, "PS_DB_ITEM_MAKE_LIMIT_UPDATE size must match IDA");

/**
 * @brief DB层物品制作限制初始化请求 - 40 bytes
 * 来自 IDA: PS_DB_ITEM_MAKE_LIMIT_INIT
 */
struct PS_DB_ITEM_MAKE_LIMIT_INIT {
    std::uint32_t dwUAID = 0;                               // +0x00: 账户ID
    std::uint32_t dwUCID = 0;                               // +0x04: 角色ID
    std::vector<PS_DB_ITEM_MAKE_LIMIT_INFO> vecInfo;        // +0x08: 列表向量
};

static_assert(sizeof(PS_DB_ITEM_MAKE_LIMIT_INIT) == 40, "PS_DB_ITEM_MAKE_LIMIT_INIT size must match IDA");

// Packet operators for PS_ITEM_MAKE_LIMIT structures
inline XPacket& operator<<(XPacket& packet, const PS_ITEM_MAKE_LIMIT_INFO& value) {
    packet.XParse << value.nMakeIndex;
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << static_cast<std::int32_t>(0);  // padding
    packet.XParse << value.biEndDate;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_ITEM_MAKE_LIMIT_INFO& value) {
    packet.XParse >> value.nMakeIndex;
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse.GetDWORD();  // skip padding
    packet.XParse >> value.biEndDate;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_MAKE_LIMIT_LIST& value) {
    packet.XParse << value.byFlag;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::int32_t>(0);  // padding
    const std::uint32_t count = static_cast<std::uint32_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_ITEM_MAKE_LIMIT_LIST& value) {
    packet.XParse >> value.byFlag;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetDWORD();  // skip padding
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        PS_ITEM_MAKE_LIMIT_INFO info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_DB_ITEM_MAKE_LIMIT_INFO& value) {
    packet.XParse >> value.byLimitType;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetDWORD();  // skip padding
    packet >> value.psInfo;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_DB_ITEM_MAKE_LIMIT_INIT& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        PS_DB_ITEM_MAKE_LIMIT_INFO info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_MAKE_LIMIT_INIT& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    const std::uint32_t count = static_cast<std::uint32_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const auto& info : value.vecInfo) {
        packet.XParse << info.byLimitType;
        packet.XParse << static_cast<std::uint8_t>(0);
        packet.XParse << static_cast<std::uint8_t>(0);
        packet.XParse << static_cast<std::uint8_t>(0);
        packet.XParse << static_cast<std::int32_t>(0);  // padding
        packet << info.psInfo;
    }
    return packet;
}

struct PS_CHAT_ITEM_LINK {
    std::uint8_t byStart = 0;
    std::uint8_t bySize = 0;
    std::uint8_t byType = 0;
    std::uint8_t _pad2[5] = {};
    std::int64_t i64ID = 0;
    wchar_t szLinkString[64] = {};
    STItem kItem{};
    PS_ITEM_SOCKET_LIST psSocketInfo{};
    ST_ITEM_BROACH psBroachInfo{};
    PS_ITEM_PACKAGE psRePackageCostumeInfo{};
};

struct PS_CHAT_ITEM_LINK_FOR_SERVER {
    std::uint8_t byItemLinkCount = 0;
    std::uint8_t _pad3[7] = {};
    PS_CHAT_ITEM_LINK psItemLinkInfo[3] = {};
};

struct PS_CHAT_WHISPER {
    wchar_t strSender[21] = {};
    wchar_t strReciver[21] = {};
    wchar_t strMsg[256] = {};
    std::int32_t nResult = 0;
    std::uint32_t dwSenderUCID = 0;
};


/**
 * @brief ModeMaze 匹配成员信息。
 */
struct ST_MODE_MAZE_MEMBER_INFO {
    std::uint32_t dwActorID = 0;
    std::uint32_t dwUAID = 0;
    std::uint16_t wMapID = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad0[3] = {};
    std::uint32_t dwProfilePhotoID = 0;
    UXMapID uxMapID{};
    wchar_t strName[21] = {};
    std::int32_t nState = 0;
    std::int32_t nFirstJumpID = 0;
};

/**
 * @brief ModeMaze 匹配进入请求。
 */
struct PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ {
    ST_MODE_MAZE_MEMBER_INFO stMemberInfo{};
    std::uint16_t wModeMazeID = 0;
    std::uint16_t wRank = 0;
};

/**
 * @brief ModeMaze 匹配退出请求。
 */
struct PS_MODE_MAZE_MATCHING_EXIT {
    std::uint32_t dwExitUCID = 0;
    std::uint32_t dwExitUAID = 0;
    std::uint8_t byReason = 0;
    std::uint8_t _pad0[3] = {};
};

/**
 * @brief ModeMaze 匹配等待通知。
 */
struct PS_MODE_MAZE_MATCHING_WAIT {
    std::uint32_t dwMatchingID = 0;
    std::uint16_t wModeMazeID = 0;
    std::uint16_t _pad0 = 0;
    std::uint32_t dwActorID = 0;
    std::vector<ST_MODE_MAZE_MEMBER_INFO> vecMemberInfo;
};

/**
 * @brief ModeMaze 匹配进入响应。
 */
struct PS_MODE_MAZE_MATCHING_ENTER_RES {
    std::uint32_t dwActorID = 0;
    std::uint16_t wModeMazeID = 0;
    std::int32_t nError = 0;
};

/**
 * @brief ModeMaze 匹配事件同步请求。
 */
struct PS_SERVER_MODE_MAZE_MATCHING_EVENT {
    std::int32_t nID = 0;
    std::int32_t nModeMazeID = 0;
    std::vector<unsigned long> vecInfo;
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
 * @brief 控制层创建地图的单个条目。
 *
 * 对齐 IDA 0x1400027A0 (CWorldManager::AddMap):
 * - qmemcpy size = 0x18 (24 bytes)
 * - 字段: uxMapID (8), nMaxUserCount (4), nCurUserCount (4), padding (8)
 */
struct PS_CREATE_MAP {
    UXMapID uxMapID{};
    int nMaxUserCount = 0;
    int nCurUserCount = 0;
    std::uint8_t _pad0[8] = {};
};

static_assert(sizeof(PS_CREATE_MAP) == 0x18, "PS_CREATE_MAP size must match IDA");

/**
 * @brief 控制层创建地图响应结构。
 *
 * 对齐 IDA 0x140002550 (CMapWithChannel::SendServerAddChannelMap):
 * - UXMapID (8 bytes)
 * - nResult (4 bytes, bool as int)
 * - padding (4 bytes)
 */
struct PS_CREATE_MAP_RES {
    UXMapID uxMapID{};
    int nResult = 0;
};

/**
 * @brief DB 频道地图信息结构。
 *
 * 对齐 IDA 0x1400023C0 (CMapWithChannel::SendDBAddChannelMap):
 * - UXMapID (8 bytes)
 * - sChannel (8 bytes, signed)
 * - wTableID (2 bytes)
 * - padding (2 bytes)
 * - dwServerID (4 bytes)
 */
struct ST_DB_CHANNEL_MAP {
    UXMapID uxMapID{};
    __int64 sChannel = 0;
    std::uint16_t wTableID = 0;
    std::uint8_t _pad0[2] = {};
    std::uint32_t dwServerID = 0;
};

// 对齐 IDA: ST_DB_CHANNEL_MAP 反序列化
// 注意: _pad0 是内存对齐填充，不参与序列化
inline void operator>>(XPacket& packet, ST_DB_CHANNEL_MAP& value) {
    packet >> value.uxMapID;
    packet.XParse >> value.sChannel;
    packet.XParse >> value.wTableID;
    // _pad0 跳过 - 仅内存对齐用
    packet.XParse >> value.dwServerID;
}

/**
 * @brief 控制层创建地图的请求列表。
 *
 * 对齐 IDA 0x1400027A0 (CWorldManager::AddMap):
 * - stMapList->vecCreateMap 迭代访问
 */
struct PS_CREATE_MAP_LIST {
    std::vector<PS_CREATE_MAP> vecCreateMap;
};

// 对齐 IDA: PS_CREATE_MAP 反序列化
inline void operator>>(XPacket& packet, PS_CREATE_MAP& value) {
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.nMaxUserCount;
    packet.XParse >> value.nCurUserCount;
}

// 对齐 IDA: PS_CREATE_MAP_LIST 反序列化
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

struct PS_SERVER_FORCE_MATCHING_ENTER {
    ST_CREATE_MAZE stCreateMaze{};
    std::vector<PS_SERVER_FORCE_MATCHING_ENTER_MEMBER> vecMember;
    ST_PARTY_INFO stPartyInfo{};
};

struct PS_SERVER_FORCE_MATCHING_CHECK {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t byCheck = 0;
    std::uint8_t _pad0[3] = {};
    int nError = 0;
};

/**
 * @brief Relay / ModeMaze 层创建匹配迷宫请求。
 *
 * PDB 中该结构独立于 `ST_CREATE_MAZE`，字段顺序与成员类型都不同。
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
 *
 * 用于 m_mapCreateMazeReq，可能包含原始 ST_CREATE_MAZE 的子集。
 * 对齐 IDA: pReqChannelServer, pReqMazeServer, dwTime, stPartyInfo, vecEnterMember
 */
struct ST_CREATE_MAZE_FOR_RELAY {
    class CServer* pReqChannelServer = nullptr;
    class CServer* pReqMazeServer = nullptr;
    unsigned long long dwTime = 0;
    ST_PARTY_INFO stPartyInfo{};
    std::vector<ST_ENTER_MAZE_MEMBER_INFO> vecEnterMember;
};

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

/**
 * @brief ControlServer 模式迷宫创建请求中继结构。
 *
 * 对齐 IDA 0x140013030 (XControlServer::CreateMatchingModeMaze):
 * - pReqChannelServer, pReqMazeServer, dwTime, vecEnterMember
 */
struct ST_CREATE_MODE_MAZE_FOR_RELAY {
    class CServer* pReqChannelServer = nullptr;
    class CServer* pReqMazeServer = nullptr;
    unsigned long long dwTime = 0;
    std::vector<ST_MODE_MAZE_MEMBER_INFO> vecEnterMember;
};

/**
 * @brief 返回迷宫请求结构。
 *
 * 对齐 IDA 0x14000EAC0 (XControlServer::ReqGoBackMaze):
 * 响应包 0xF2, 0x28
 */
struct ST_GO_BACK_MAZE : ST_MAP_INFO {
    bool bResult = false;
    std::uint8_t _pad0[3] = {};
};

// ST_GO_BACK_MAZE 序列化
inline XPacket& operator<<(XPacket& packet, const ST_GO_BACK_MAZE& value) {
    // 基类 ST_MAP_INFO 序列化
    packet.XParse << static_cast<int>(value.dwUserID);
    packet.XParse << static_cast<int>(value.dwUAID);
    packet.XParse << static_cast<int>(value.dwServerID);
    packet.XParse << value.nJumpID;
    packet.XParse << value.nPortalID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.uxParentInstanceID.nMapID;
    packet << value.stPosInfo;
    packet.XParse << static_cast<int>(value.byType);
    // 派生类字段
    packet.XParse << static_cast<int>(value.bResult ? 1 : 0);
    return packet;
}

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
 * @brief 服务器状态更新信息（发送给子服务器）。
 * 注意：此结构体已在 PSCommon.h 中定义，此处仅为文档说明。
 */
// struct SS_UPDATE_SERVER_INFO 在 PSCommon.h 中定义

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
 * @brief ModeMaze 运营时间信息（用于 SendOperationTimeInfo）。
 *
 * 对齐 IDA SendOperationTimeInfo / ModeMazeTime_Cheat。
 */
struct PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO {
    std::uint32_t dwModeMazeID = 0;
    int nHotTime_Start_1st = 0;
    int nHotTime_End_1st = 0;
    int nHotTime_Start_2nd = 0;
    int nHotTime_End_2nd = 0;
    int nHotTime_Start_3rd = 0;
    int nHotTime_End_3rd = 0;
};

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
static_assert(sizeof(PS_CHAT_NOTICE) == 0x214, "PS_CHAT_NOTICE size must match PDB");
static_assert(offsetof(PS_CHAT_NOTICE, strMsg) == 0x2, "PS_CHAT_NOTICE.strMsg offset mismatch");
static_assert(offsetof(PS_CHAT_NOTICE, strColor) == 0x202, "PS_CHAT_NOTICE.strColor offset mismatch");
static_assert(offsetof(PS_CHAT_NOTICE, nMessageCode) == 0x210,
              "PS_CHAT_NOTICE.nMessageCode offset mismatch");
static_assert(sizeof(PS_CHAT_MEGAPHONE) == 0x234, "PS_CHAT_MEGAPHONE size must match PDB");
static_assert(offsetof(PS_CHAT_MEGAPHONE, shSlot) == 0x2, "PS_CHAT_MEGAPHONE.shSlot offset mismatch");
static_assert(offsetof(PS_CHAT_MEGAPHONE, dwUCID) == 0x4, "PS_CHAT_MEGAPHONE.dwUCID offset mismatch");
static_assert(offsetof(PS_CHAT_MEGAPHONE, strName) == 0x8, "PS_CHAT_MEGAPHONE.strName offset mismatch");
static_assert(offsetof(PS_CHAT_MEGAPHONE, strMsg) == 0x32, "PS_CHAT_MEGAPHONE.strMsg offset mismatch");
static_assert(sizeof(ST_SOCKET_DATA) == 0x30, "ST_SOCKET_DATA size must match PDB");
static_assert(sizeof(ST_ITEM_SOCKET) == 0x38, "ST_ITEM_SOCKET size must match PDB");
static_assert(offsetof(ST_ITEM_SOCKET, biEquipSerial) == 0x30, "ST_ITEM_SOCKET.biEquipSerial offset mismatch");
static_assert(sizeof(PS_ITEM_SOCKET_LIST) == 0x20, "PS_ITEM_SOCKET_LIST size must match PDB");
static_assert(sizeof(ST_ITEM_BROACH) == 0x48, "ST_ITEM_BROACH size must match PDB");
static_assert(offsetof(ST_ITEM_BROACH, dwItemID) == 0x8, "ST_ITEM_BROACH.dwItemID offset mismatch");
static_assert(sizeof(ST_ITEM_PACKAGE_PARTS) == 0x10, "ST_ITEM_PACKAGE_PARTS size must match PDB");
static_assert(offsetof(ST_ITEM_PACKAGE_PARTS, nItemID) == 0x8,
              "ST_ITEM_PACKAGE_PARTS.nItemID offset mismatch");
static_assert(offsetof(ST_ITEM_PACKAGE_PARTS, nDyeID) == 0xC,
              "ST_ITEM_PACKAGE_PARTS.nDyeID offset mismatch");
static_assert(sizeof(PS_ITEM_PACKAGE) == 0x28, "PS_ITEM_PACKAGE size must match PDB");
static_assert(offsetof(PS_ITEM_PACKAGE, vecInfo) == 0x8, "PS_ITEM_PACKAGE.vecInfo offset mismatch");
static_assert(sizeof(PS_CHAT_ITEM_LINK) == 0x198, "PS_CHAT_ITEM_LINK size must match PDB");
static_assert(offsetof(PS_CHAT_ITEM_LINK, i64ID) == 0x8, "PS_CHAT_ITEM_LINK.i64ID offset mismatch");
static_assert(offsetof(PS_CHAT_ITEM_LINK, szLinkString) == 0x10,
              "PS_CHAT_ITEM_LINK.szLinkString offset mismatch");
static_assert(offsetof(PS_CHAT_ITEM_LINK, kItem) == 0x90, "PS_CHAT_ITEM_LINK.kItem offset mismatch");
static_assert(offsetof(PS_CHAT_ITEM_LINK, psSocketInfo) == 0x108,
              "PS_CHAT_ITEM_LINK.psSocketInfo offset mismatch");
static_assert(offsetof(PS_CHAT_ITEM_LINK, psBroachInfo) == 0x128,
              "PS_CHAT_ITEM_LINK.psBroachInfo offset mismatch");
static_assert(offsetof(PS_CHAT_ITEM_LINK, psRePackageCostumeInfo) == 0x170,
              "PS_CHAT_ITEM_LINK.psRePackageCostumeInfo offset mismatch");
static_assert(sizeof(PS_CHAT_ITEM_LINK_FOR_SERVER) == 0x4D0,
              "PS_CHAT_ITEM_LINK_FOR_SERVER size must match PDB");
static_assert(offsetof(PS_CHAT_ITEM_LINK_FOR_SERVER, psItemLinkInfo) == 0x8,
              "PS_CHAT_ITEM_LINK_FOR_SERVER.psItemLinkInfo offset mismatch");
static_assert(sizeof(PS_CHAT_WHISPER) == 0x25C, "PS_CHAT_WHISPER size must match PDB");
static_assert(offsetof(PS_CHAT_WHISPER, strReciver) == 0x2A,
              "PS_CHAT_WHISPER.strReciver offset mismatch");
static_assert(offsetof(PS_CHAT_WHISPER, strMsg) == 0x54, "PS_CHAT_WHISPER.strMsg offset mismatch");
static_assert(offsetof(PS_CHAT_WHISPER, nResult) == 0x254, "PS_CHAT_WHISPER.nResult offset mismatch");
static_assert(offsetof(PS_CHAT_WHISPER, dwSenderUCID) == 0x258,
              "PS_CHAT_WHISPER.dwSenderUCID offset mismatch");
static_assert(sizeof(ST_MODE_MAZE_MEMBER_INFO) == 0x58, "ST_MODE_MAZE_MEMBER_INFO size must match PDB");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, wMapID) == 0x8,
              "ST_MODE_MAZE_MEMBER_INFO.wMapID offset mismatch");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, dwProfilePhotoID) == 0x10,
              "ST_MODE_MAZE_MEMBER_INFO.dwProfilePhotoID offset mismatch");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, uxMapID) == 0x18,
              "ST_MODE_MAZE_MEMBER_INFO.uxMapID offset mismatch");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, strName) == 0x20,
              "ST_MODE_MAZE_MEMBER_INFO.strName offset mismatch");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, nState) == 0x4C,
              "ST_MODE_MAZE_MEMBER_INFO.nState offset mismatch");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, nFirstJumpID) == 0x50,
              "ST_MODE_MAZE_MEMBER_INFO.nFirstJumpID offset mismatch");
static_assert(sizeof(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ) == 0x60,
              "PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ size must match PDB");
static_assert(offsetof(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ, wModeMazeID) == 0x58,
              "PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ.wModeMazeID offset mismatch");
static_assert(offsetof(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ, wRank) == 0x5A,
              "PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ.wRank offset mismatch");
static_assert(sizeof(PS_MODE_MAZE_MATCHING_EXIT) == 0x0C,
              "PS_MODE_MAZE_MATCHING_EXIT size must match PDB");
static_assert(offsetof(PS_MODE_MAZE_MATCHING_EXIT, dwExitUAID) == 0x4,
              "PS_MODE_MAZE_MATCHING_EXIT.dwExitUAID offset mismatch");
static_assert(offsetof(PS_MODE_MAZE_MATCHING_EXIT, byReason) == 0x8,
              "PS_MODE_MAZE_MATCHING_EXIT.byReason offset mismatch");
static_assert(sizeof(PS_SERVER_MODE_MAZE_MATCHING_EVENT) == 0x28,
              "PS_SERVER_MODE_MAZE_MATCHING_EVENT size must match PDB");
static_assert(offsetof(PS_SERVER_MODE_MAZE_MATCHING_EVENT, nModeMazeID) == 0x4,
              "PS_SERVER_MODE_MAZE_MATCHING_EVENT.nModeMazeID offset mismatch");
static_assert(offsetof(PS_SERVER_MODE_MAZE_MATCHING_EVENT, vecInfo) == 0x8,
              "PS_SERVER_MODE_MAZE_MATCHING_EVENT.vecInfo offset mismatch");
static_assert(sizeof(ST_SERVER_GROUP_INFO) == 0x224, "ST_SERVER_GROUP_INFO size must match PDB");
static_assert(offsetof(ST_SERVER_GROUP_INFO, szPublicIP) == 0x19, "ST_SERVER_GROUP_INFO.szPublicIP offset mismatch");
static_assert(offsetof(ST_SERVER_GROUP_INFO, nState) == 0x21C, "ST_SERVER_GROUP_INFO.nState offset mismatch");
static_assert(sizeof(SS_SERVER_INFO) == 0x440, "SS_SERVER_INFO size must match PDB");
static_assert(offsetof(SS_SERVER_INFO, szName) == 0x20, "SS_SERVER_INFO.szName offset mismatch");
static_assert(offsetof(SS_SERVER_INFO, szPrivateIP) == 0x35, "SS_SERVER_INFO.szPrivateIP offset mismatch");
static_assert(offsetof(SS_SERVER_INFO, szPublicIP) == 0x236, "SS_SERVER_INFO.szPublicIP offset mismatch");
static_assert(offsetof(SS_SERVER_INFO, nMazeCount) == 0x438, "SS_SERVER_INFO.nMazeCount offset mismatch");
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

// 对齐 IDA: PS_CHAT_FORCE 是 PS_CHAT_PARTY 的别名，用于 Force 聊天消息
using PS_CHAT_FORCE = PS_CHAT_PARTY;
static_assert(sizeof(PS_RES_FORCE_ENTER_SERVER) == 0x98,
              "PS_RES_FORCE_ENTER_SERVER size must match PDB");
static_assert(offsetof(PS_RES_FORCE_ENTER_SERVER, stEnterMember) == 0x8,
              "PS_RES_FORCE_ENTER_SERVER.stEnterMember offset mismatch");
static_assert(offsetof(PS_RES_FORCE_ENTER_SERVER, stForceInfo) == 0x60,
              "PS_RES_FORCE_ENTER_SERVER.stForceInfo offset mismatch");
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

template <std::size_t N>
inline std::wstring GreenDamTan_BoundedWideString(const wchar_t (&value)[N]) {
    constexpr std::size_t maxChars = N > 0 ? N - 1 : 0;
    const wchar_t* end = std::find(value, value + maxChars, L'\0');
    return std::wstring(value, static_cast<std::size_t>(end - value));
}

template <std::size_t N>
inline std::string GreenDamTan_BoundedString(const char (&value)[N]) {
    constexpr std::size_t maxChars = N > 0 ? N - 1 : 0;
    const char* end = std::find(value, value + maxChars, '\0');
    return std::string(value, static_cast<std::size_t>(end - value));
}

inline XPacket& operator<<(XPacket& packet, const ST_PARTY_INFO& value) {
    packet.XParse << value.byGroupType;
    packet.XParse << value.nID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_INFO& value) {
    packet.XParse >> value.byGroupType;
    packet.XParse >> value.nID;
}

inline XPacket& operator<<(XPacket& packet, const ST_PARTY_MEMBER& value) {
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

inline void operator>>(XPacket& packet, ST_PARTY_MEMBER& value) {
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

inline XPacket& operator<<(XPacket& packet, const ST_UPDATE_PARTY_MEMBER& value) {
    packet.XParse << value.dwPartyID;
    packet.XParse << static_cast<std::uint32_t>(0);
    packet << value.stPartyMember;
    return packet;
}

inline void operator>>(XPacket& packet, ST_UPDATE_PARTY_MEMBER& value) {
    packet.XParse >> value.dwPartyID;
    packet.XParse.GetDWORD();
    packet >> value.stPartyMember;
}

// 对齐 IDA: ST_MATCHING_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_MATCHING_INFO& value) {
    packet.XParse << value.dwMatchingID;
    packet.XParse << static_cast<std::uint32_t>(0);  // _pad0
    for (int i = 0; i < 4; ++i) {
        packet << value.stMemberInfo[i];
    }
    packet.XParse << value.nRemainTick;
    return packet;
}

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

inline XPacket& operator<<(XPacket& packet, const PS_PARTY_DELETE& value) {
    packet.XParse << value.dwPartyID;
    packet.XParse << value.dwLeaveMember;
    return packet;
}

inline void operator>>(XPacket& packet, PS_PARTY_DELETE& value) {
    packet.XParse >> value.dwPartyID;
    packet.XParse >> value.dwLeaveMember;
}

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

inline XPacket& operator<<(XPacket& packet, const PS_FORCE_DELETE& value) {
    packet.XParse << value.dwForceID;
    packet.XParse << value.dwLeaveMember;
    return packet;
}

inline void operator>>(XPacket& packet, PS_FORCE_DELETE& value) {
    packet.XParse >> value.dwForceID;
    packet.XParse >> value.dwLeaveMember;
}

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

// 对齐 IDA: ST_PARTY_RECRUIT_UPDATE 序列化
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

inline XPacket& operator<<(XPacket& packet, const PS_RECRUIT_DELETE& value) {
    packet.XParse << value.dwUCID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RECRUIT_DELETE& value) {
    packet.XParse >> value.dwUCID;
}

inline XPacket& operator<<(XPacket& packet, const PS_RECRUIT_STATE& value) {
    packet.XParse << value.bRecruit;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RECRUIT_STATE& value) {
    packet.XParse >> value.bRecruit;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_RECRUIT_DELETE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_RECRUIT_DELETE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nResult;
}

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

inline XPacket& operator<<(XPacket& packet, const PS_SERVER_FORCE_MATCHING_ENTER& value) {
    packet.XParse << value.stCreateMaze.dwUserID;
    packet.XParse << value.stCreateMaze.dwUAID;
    packet.XParse << value.stCreateMaze.dwServerID;
    packet.XParse << value.stCreateMaze.nJumpID;
    packet.XParse << value.stCreateMaze.nPortalID;
    packet.XParse << value.stCreateMaze.uxMapID.nMapID;
    packet.XParse << value.stCreateMaze.uxParentInstanceID.nMapID;
    packet.XParse << std::string(value.stCreateMaze.szIP);
    packet.XParse << value.stCreateMaze.sPort;
    packet << value.stCreateMaze.stPosInfo;
    packet.XParse << value.stCreateMaze.byType;
    packet.XParse << value.stCreateMaze.wReqMapID;
    packet << value.stCreateMaze.stPartyInfo;
    packet << value.stCreateMaze.stEnterDistrictPos;
    packet.XParse << value.stCreateMaze.uxParentMazeID.nMapID;
    packet.XParse << value.stCreateMaze.nResult;
    packet.XParse << value.stCreateMaze.nCreateType;
    const std::uint8_t createMazeMemberCount =
        static_cast<std::uint8_t>(std::min<std::size_t>(value.stCreateMaze.vecEnterMember.size(), 0xFF));
    packet.XParse << createMazeMemberCount;
    for (std::size_t index = 0; index < value.stCreateMaze.vecEnterMember.size() && index < 0xFF; ++index) {
        packet.XParse << value.stCreateMaze.vecEnterMember[index].dwMember;
        packet.XParse << value.stCreateMaze.vecEnterMember[index].nState;
    }
    const std::uint32_t memberCount = static_cast<std::uint32_t>(value.vecMember.size());
    packet.XParse << memberCount;
    for (const auto& member : value.vecMember) {
        packet << member;
    }
    packet << value.stPartyInfo;
    return packet;
}

inline void operator>>(XPacket& packet, PS_SERVER_FORCE_MATCHING_ENTER& value) {
    short outLen = 0;
    packet.XParse >> value.stCreateMaze.dwUserID;
    packet.XParse >> value.stCreateMaze.dwUAID;
    packet.XParse >> value.stCreateMaze.dwServerID;
    packet.XParse >> value.stCreateMaze.nJumpID;
    packet.XParse >> value.stCreateMaze.nPortalID;
    packet.XParse >> value.stCreateMaze.uxMapID.nMapID;
    packet.XParse >> value.stCreateMaze.uxParentInstanceID.nMapID;
    packet.XParse.GetString(value.stCreateMaze.szIP, 513, &outLen);
    packet.XParse >> value.stCreateMaze.sPort;
    packet >> value.stCreateMaze.stPosInfo;
    packet.XParse >> value.stCreateMaze.byType;
    packet.XParse >> value.stCreateMaze.wReqMapID;
    packet >> value.stCreateMaze.stPartyInfo;
    packet >> value.stCreateMaze.stEnterDistrictPos;
    packet.XParse >> value.stCreateMaze.uxParentMazeID.nMapID;
    packet.XParse >> value.stCreateMaze.nResult;
    packet.XParse >> value.stCreateMaze.nCreateType;
    std::uint8_t createMazeMemberCount = 0;
    packet.XParse >> createMazeMemberCount;
    value.stCreateMaze.vecEnterMember.clear();
    value.stCreateMaze.vecEnterMember.reserve(createMazeMemberCount);
    for (std::uint8_t index = 0; index < createMazeMemberCount; ++index) {
        ST_ENTER_MAZE_MEMBER_INFO item{};
        packet.XParse >> item.dwMember;
        packet.XParse >> item.nState;
        value.stCreateMaze.vecEnterMember.push_back(item);
    }
    std::uint32_t memberCount = 0;
    packet.XParse >> memberCount;
    value.vecMember.clear();
    value.vecMember.reserve(memberCount);
    for (std::uint32_t index = 0; index < memberCount; ++index) {
        PS_SERVER_FORCE_MATCHING_ENTER_MEMBER member{};
        packet >> member;
        value.vecMember.push_back(member);
    }
    packet >> value.stPartyInfo;
}

inline XPacket& operator<<(XPacket& packet, const PS_SERVER_FORCE_MATCHING_CHECK& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.byCheck;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.nError;
    return packet;
}

inline void operator>>(XPacket& packet, PS_SERVER_FORCE_MATCHING_CHECK& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byCheck;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse >> value.nError;
}

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

inline void operator>>(XPacket& packet, ST_ENTER_MAZE_MEMBER_INFO& value) {
    packet.XParse >> value.dwMember;
    packet.XParse >> value.nState;
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
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
}

inline XPacket& operator<<(XPacket& packet, const PS_USER_INFO_FOR_RELAY& value) {
    packet << value.stCharInfo;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.dwIP;
    packet.XParse << value.byTradePW;
    std::uint8_t zero = 0;
    packet.XParse << zero;
    packet.XParse << zero;
    packet.XParse << zero;
    packet.XParse << value.biAuthSessionID;
    packet << value.stGameOption;
    packet.XParse << value.byAuthType;
    packet.XParse << zero;
    packet.XParse << zero;
    packet.XParse << zero;
    return packet;
}

inline void operator>>(XPacket& packet, PS_USERS_INFO& value) {
    packet.XParse >> value.bFinish;
    packet.XParse >> value.nTotalCount;
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecUserInfo.clear();
    value.vecUserInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        PS_USER_INFO_FOR_RELAY item{};
        packet >> item;
        value.vecUserInfo.push_back(item);
    }
}

// 对齐 IDA: ST_ENTER_SERVER 序列化
inline void operator>>(XPacket& packet, ST_ENTER_SERVER& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.bFirstConnect;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse >> value.biAuthSessionID;
}

inline XPacket& operator<<(XPacket& packet, const ST_ENTER_SERVER& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwActorID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.bFirstConnect;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.biAuthSessionID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_CHAT_NOTICE& value) {
    short outLen = 0;
    packet.XParse >> value.byType;
    packet.XParse.GetBYTE();
    packet.XParse.GetWString(value.strMsg, 256, outLen);
    packet.XParse.GetWString(value.strColor, 7, outLen);
    packet.XParse >> value.nMessageCode;
}

inline XPacket& operator<<(XPacket& packet, const PS_CHAT_NOTICE& value) {
    packet.XParse << value.byType;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    packet.XParse << GreenDamTan_BoundedWideString(value.strColor);
    packet.XParse << value.nMessageCode;
    return packet;
}

inline void operator>>(XPacket& packet, PS_CHAT_MEGAPHONE& value) {
    short outLen = 0;
    packet.XParse >> value.byInvenType;
    packet.XParse >> value.shSlot;
    packet.XParse >> value.dwUCID;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse.GetWString(value.strMsg, 256, outLen);
}

inline XPacket& operator<<(XPacket& packet, const PS_CHAT_MEGAPHONE& value) {
    packet.XParse << value.byInvenType;
    packet.XParse << value.shSlot;
    packet.XParse << value.dwUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    return packet;
}

inline void operator>>(XPacket& packet, ST_SOCKET_DATA& value) {
    packet.XParse >> value.dwSocketID;
    packet.XParse >> value.bySocketPos;
    for (ST_EXTEND_OPTION& extendOption : value.stExtendOption) {
        packet >> extendOption;
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_SOCKET_DATA& value) {
    packet.XParse << value.dwSocketID;
    packet.XParse << value.bySocketPos;
    for (const ST_EXTEND_OPTION& extendOption : value.stExtendOption) {
        packet << extendOption;
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_ITEM_SOCKET& value) {
    packet.XParse >> value.biEquipSerial;
    packet.XParse >> value.dwSocketID;
    packet.XParse >> value.bySocketPos;
    for (ST_EXTEND_OPTION& extendOption : value.stExtendOption) {
        packet >> extendOption;
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_ITEM_SOCKET& value) {
    packet.XParse << value.biEquipSerial;
    packet.XParse << value.dwSocketID;
    packet.XParse << value.bySocketPos;
    for (const ST_EXTEND_OPTION& extendOption : value.stExtendOption) {
        packet << extendOption;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_ITEM_SOCKET_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint16_t index = 0; index < count; ++index) {
        ST_ITEM_SOCKET item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_SOCKET_LIST& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecInfo.size(), 0xFFFF));
    packet.XParse << count;
    for (std::size_t index = 0; index < count; ++index) {
        packet << value.vecInfo[index];
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_ITEM_BROACH& value) {
    packet.XParse >> value.biSerial;
    for (int& itemID : value.dwItemID) {
        packet.XParse >> itemID;
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_ITEM_BROACH& value) {
    packet.XParse << value.biSerial;
    for (const int itemID : value.dwItemID) {
        packet.XParse << itemID;
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_ITEM_PACKAGE_PARTS& value) {
    packet.XParse >> value.biSerial;
    packet.XParse >> value.nItemID;
    packet.XParse >> value.nDyeID;
}

inline XPacket& operator<<(XPacket& packet, const ST_ITEM_PACKAGE_PARTS& value) {
    packet.XParse << value.biSerial;
    packet.XParse << value.nItemID;
    packet.XParse << value.nDyeID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_ITEM_PACKAGE& value) {
    int count = 0;
    packet.XParse >> value.biPackageSerial;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(std::max(count, 0)));
    for (int index = 0; index < count; ++index) {
        ST_ITEM_PACKAGE_PARTS item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_PACKAGE& value) {
    const int count = static_cast<int>(value.vecInfo.size());
    packet.XParse << value.biPackageSerial;
    packet.XParse << count;
    for (int index = 0; index < count; ++index) {
        packet << value.vecInfo[static_cast<std::size_t>(index)];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_ITEM_BROACH_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint16_t index = 0; index < count; ++index) {
        ST_ITEM_BROACH item{};
        packet >> item;
        value.vecInfo.push_back(std::move(item));
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_BROACH_LIST& value) {
    std::uint16_t count = static_cast<std::uint16_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_ITEM_PACKAGE_LIST& value) {
    // 对齐 IDA 0x1400EB7A0: 使用 int nCount
    std::int32_t nCount = 0;
    packet.XParse >> nCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(nCount));
    for (std::int32_t index = 0; index < nCount; ++index) {
        PS_ITEM_PACKAGE item{};
        packet >> item;
        value.vecInfo.push_back(std::move(item));
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_PACKAGE_LIST& value) {
    // 对齐 IDA 0x1400EB720: 使用 int nCount
    std::int32_t nCount = static_cast<std::int32_t>(value.vecInfo.size());
    packet.XParse << nCount;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_CHAT_ITEM_LINK& value) {
    short outLen = 0;
    packet.XParse >> value.byStart;
    packet.XParse >> value.bySize;
    packet.XParse >> value.byType;
    packet.XParse >> value.i64ID;
    packet.XParse.GetWString(value.szLinkString, 64, outLen);
    packet >> value.kItem;
    packet >> value.psSocketInfo;
    packet >> value.psBroachInfo;
    packet >> value.psRePackageCostumeInfo;
}

inline XPacket& operator<<(XPacket& packet, const PS_CHAT_ITEM_LINK& value) {
    packet.XParse << value.byStart;
    packet.XParse << value.bySize;
    packet.XParse << value.byType;
    packet.XParse << value.i64ID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLinkString);
    packet << value.kItem;
    packet << value.psSocketInfo;
    packet << value.psBroachInfo;
    packet << value.psRePackageCostumeInfo;
    return packet;
}

inline void operator>>(XPacket& packet, PS_CHAT_ITEM_LINK_FOR_SERVER& value) {
    packet.XParse >> value.byItemLinkCount;
    for (PS_CHAT_ITEM_LINK& item : value.psItemLinkInfo) {
        packet >> item;
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_CHAT_ITEM_LINK_FOR_SERVER& value) {
    packet.XParse << value.byItemLinkCount;
    for (const PS_CHAT_ITEM_LINK& item : value.psItemLinkInfo) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_CHAT_WHISPER& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.strSender, 21, outLen);
    packet.XParse.GetWString(value.strReciver, 21, outLen);
    packet.XParse.GetWString(value.strMsg, 256, outLen);
    packet.XParse >> value.nResult;
    packet.XParse >> value.dwSenderUCID;
}

inline XPacket& operator<<(XPacket& packet, const PS_CHAT_WHISPER& value) {
    packet.XParse << GreenDamTan_BoundedWideString(value.strSender);
    packet.XParse << GreenDamTan_BoundedWideString(value.strReciver);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    packet.XParse << value.nResult;
    packet.XParse << value.dwSenderUCID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_UPDATE_USER_MAP_INFO& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.uxMapID.nMapID;
    packet >> value.stPartyInfo;
    packet.XParse >> value.biAuthSessionID;
    packet.XParse >> value.bLeaveParty;
}

inline XPacket& operator<<(XPacket& packet, const ST_MODE_MAZE_MEMBER_INFO& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.wMapID;
    packet.XParse << value.byClass;
    packet.XParse << value.byLevel;
    packet.XParse << value.byAwaken;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << std::wstring(value.strName);
    packet.XParse << value.nState;
    packet.XParse << value.nFirstJumpID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_MODE_MAZE_MEMBER_INFO& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.wMapID;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byLevel;
    packet.XParse >> value.byAwaken;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.uxMapID.nMapID;
    short outLen = 0;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.nState;
    packet.XParse >> value.nFirstJumpID;
}

inline void operator>>(XPacket& packet, PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ& value) {
    packet >> value.stMemberInfo;
    packet.XParse >> value.wModeMazeID;
    packet.XParse >> value.wRank;
}

inline void operator>>(XPacket& packet, PS_MODE_MAZE_MATCHING_EXIT& value) {
    packet.XParse >> value.dwExitUCID;
    packet.XParse >> value.dwExitUAID;
    packet.XParse >> value.byReason;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
}

inline XPacket& operator<<(XPacket& packet, const PS_MODE_MAZE_MATCHING_EXIT& value) {
    packet.XParse << value.dwExitUCID;
    packet.XParse << value.dwExitUAID;
    packet.XParse << value.byReason;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_MODE_MAZE_MATCHING_WAIT& value) {
    packet.XParse << value.dwMatchingID;
    packet.XParse << value.wModeMazeID;
    packet.XParse << value._pad0;
    packet.XParse << value.dwActorID;
    packet.XParse << static_cast<std::int16_t>(static_cast<int>(value.vecMemberInfo.size()));
    for (const auto& member : value.vecMemberInfo) {
        packet << member;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_MODE_MAZE_MATCHING_ENTER_RES& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.wModeMazeID;
    packet.XParse << value.nError;
    return packet;
}

inline void operator>>(XPacket& packet, PS_SERVER_MODE_MAZE_MATCHING_EVENT& value) {
    packet.XParse >> value.nID;
    packet.XParse >> value.nModeMazeID;
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        unsigned int item = 0;
        packet.XParse >> item;
        value.vecInfo.push_back(static_cast<unsigned long>(item));
    }
}

// 对齐 IDA: PS_SERVER_MODE_MAZE_MATCHING_EVENT 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_MODE_MAZE_MATCHING_EVENT& value) {
    packet.XParse << value.nID;
    packet.XParse << value.nModeMazeID;
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet.XParse << static_cast<unsigned int>(item);
    }
    return packet;
}

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

// 对齐 IDA SendServerInfoAll: 服务器统计信息序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_COMMON_INFO& value) {
    packet.XParse << value.nServerCount;
    packet.XParse << value.nTotalUserCount;
    return packet;
}

// 对齐 IDA SendOperationTimeInfo: ModeMaze 运营时间序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO& value) {
    packet.XParse << value.dwModeMazeID;
    packet.XParse << value.nHotTime_Start_1st;
    packet.XParse << value.nHotTime_End_1st;
    packet.XParse << value.nHotTime_Start_2nd;
    packet.XParse << value.nHotTime_End_2nd;
    packet.XParse << value.nHotTime_Start_3rd;
    packet.XParse << value.nHotTime_End_3rd;
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

inline XPacket& operator>>(XPacket& packet, ST_SERVER_GROUP_INFO& value) {
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
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_SERVER_GROUP_INFO_VEC& value) {
    const std::uint8_t serverCount =
        static_cast<std::uint8_t>(std::min<std::size_t>(value.vecServerInfos.size(), 0xFF));
    packet.XParse << serverCount;
    for (std::uint8_t index = 0; index < serverCount; ++index) {
        packet << value.vecServerInfos[index];
    }
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

// 对齐 IDA: ST_CREATE_MAZE 反序列化
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

// PS_PARTY_INFO_ALL serializer - vector of PS_PARTY_INFO for loading all party data
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

// PS_FORCE_INFO_ALL serializer - vector of PS_FORCE_INFO for loading all force data
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
// League Structures
// ============================================================================

// League member base info (32 bytes)
struct ST_LEAGUE_MEMBER {
    std::int32_t nLeagueID = 0;
    std::uint8_t byPosition = 0;
    std::uint8_t _pad0[3] = {};
    std::int64_t biLeagueExp = 0;
    std::int64_t biJoinDate = 0;
    std::int64_t biApplicationDate = 0;
};

// Extended league member with additional info (112 bytes)
struct ST_LEAGUE_MEMBER_EX {
    ST_LEAGUE_MEMBER stMember{};
    bool bLogin = false;
    std::uint8_t _pad0 = 0;
    std::int16_t sWorldID = 0;
    std::uint8_t byChannel = 0;
    std::uint8_t _pad1[3] = {};
    std::uint32_t dwUCID = 0;
    wchar_t szName[21] = {};
    std::int16_t shLevel = 0;
    std::uint8_t _pad2[2] = {};
    std::int64_t biBoardLimitTime = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad3[2] = {};
    std::uint32_t dwProfilePhotoID = 0;
    std::uint8_t _pad4[4] = {};
    std::int64_t biPlayDate = 0;
};

// League invite info (16 bytes)
struct ST_LEAGUE_INVITE_INFO {
    std::int32_t nLeagueID = 0;
    bool bInvite = false;
    std::uint8_t _pad0[3] = {};
    std::uint64_t dwLimitTime = 0;
};

// League create request inner struct (32 bytes)
struct ST_REQ_LEAGUE_CREATE {
    wchar_t szName[10] = {};
    std::uint32_t dwNpcID = 0;
    std::int32_t nLeagueID = 0;
    std::int32_t nErrorCode = 0;
};

// League create packet for server (120 bytes)
struct PS_LEAGUE_CREATE_FOR_SERVER {
    ST_REQ_LEAGUE_CREATE stCreateInfo{};
    std::uint32_t dwActorID = 0;
    wchar_t szMasterName[21] = {};
    std::int64_t nCreateDate = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad0[2] = {};
    std::uint32_t dwProfilePhotoID = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t _pad1 = 0;
    std::int16_t sWorldID = 0;
    std::int32_t nAuth_Elder = 0;
    std::int32_t nAuth_Manager = 0;
    std::int32_t nAuth_SubMaster = 0;
    std::int32_t nServerID = 0;
};

// League invite request (120 bytes)
struct ST_REQ_LEAGUE_INVITE {
    wchar_t szLeagueName[10] = {};
    wchar_t szTargetName[21] = {};
    wchar_t szReqName[21] = {};
    std::uint32_t dwActorID = 0;
    std::uint32_t dwTargetActorID = 0;
    std::int32_t nLeagueID = 0;
    std::int32_t nResult = 0;
};

// League invite accept (60 bytes)
struct ST_REQ_LEAGUE_INVITE_ACCEPT {
    std::uint32_t dwReqUCID = 0;
    std::uint32_t dwTargetUCID = 0;
    wchar_t szTargetName[21] = {};
    std::int32_t nLeagueID = 0;
    std::int32_t nResult = 0;
};

// 联赛邀请拒绝请求
struct ST_REQ_LEAGUE_INVITE_REJECT {
    wchar_t szTargetName[21] = {};
    std::uint32_t dwReqUCID = 0;
    std::uint32_t dwTargetUCID = 0;
    std::int32_t nResult = 0;
};

// 联赛搜索请求
struct ST_REQ_LEAGUE_SEARCH {
    std::uint8_t nState = 0;
    std::uint8_t _pad0 = 0;
    wchar_t szLeagueName[10] = {};
    wchar_t szMasterName[21] = {};
};

// 联赛仓库物品移动请求参数 (48 bytes, 对齐 IDA PS_REQ_ITEM_MOVE_LEAGUE_INVEN)
struct PS_REQ_ITEM_MOVE_LEAGUE_INVEN {
    std::int32_t nLeagueID = 0;         // offset 0x0
    std::uint32_t dwNpcID = 0;          // offset 0x4
    std::int32_t nSrcItemID = 0;        // offset 0x8
    std::int32_t nDestItemID = 0;       // offset 0xc
    std::uint8_t bySrcInvenType = 0;    // offset 0x10
    std::uint8_t byDestInvenType = 0;   // offset 0x11
    std::int16_t shSrcSlotPos = 0;      // offset 0x12
    std::int16_t shDestSlotPos = 0;     // offset 0x14
    std::uint8_t _pad0[2] = {};         // offset 0x16, padding
    std::int64_t biSrcSerial = 0;       // offset 0x18
    std::int64_t biDestcSerial = 0;     // offset 0x20
    std::uint8_t byType = 0;            // offset 0x28
    std::uint8_t _pad1[7] = {};         // offset 0x29, padding to 48 bytes total
};

// 联赛仓库物品移动响应参数 (240 bytes, 对齐 IDA PS_RES_ITEM_MOVE_LEAGUE_INVEN)
struct PS_RES_ITEM_MOVE_LEAGUE_INVEN {
    std::int32_t nLeagueID = 0;         // offset 0x0
    std::int32_t nSrcItemID = 0;        // offset 0x4
    std::int32_t nDestItemID = 0;       // offset 0x8
    std::int16_t shSrcSlotPos = 0;      // offset 0xc
    std::int16_t shDestSlotPos = 0;     // offset 0xe
    std::uint8_t byType = 0;            // offset 0x10
    std::uint8_t _pad0[7] = {};         // padding to 0x18
    STItem stItem{};                    // offset 0x18, size 120
    PS_ITEM_SOCKET_LIST psItemSocketList{};  // offset 0x90, size 32
    PS_ITEM_BROACH_LIST psItemBroachList{};  // offset 0xb0, size 32
    PS_ITEM_PACKAGE_LIST psItemPackageList{}; // offset 0xd0, size 32
};

// 联赛物品移动请求（游戏端）
struct PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME {
    std::int32_t nErrorCode = 0;
    std::uint8_t _pad0[4] = {};
    PS_STORAGE_INFO psStorageInfo{};
    PS_STORAGE_INFO psOutItemInfo{};
    PS_RES_ITEM_MOVE_LEAGUE_INVEN psResItemMoveInfo{};
    std::uint8_t psItemLogList_raw[32] = {};       // PS_LEAGUE_INVENTORY_FOR_LOG_LIST
    std::int32_t nInventorySync = 0;
    PS_REQ_ITEM_MOVE_LEAGUE_INVEN psReqItemMoveInfo{};
};

// 联赛仓库移动请求反序列化
inline void operator>>(XPacket& packet, PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME& value) {
    packet.XParse >> value.nErrorCode;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet >> value.psStorageInfo;
    packet >> value.psOutItemInfo;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&value.psResItemMoveInfo), sizeof(value.psResItemMoveInfo));
    packet.XParse.GetBytes(reinterpret_cast<char*>(value.psItemLogList_raw), sizeof(value.psItemLogList_raw));
    packet.XParse >> value.nInventorySync;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&value.psReqItemMoveInfo), sizeof(value.psReqItemMoveInfo));
}

// 联赛仓库移动请求序列化（用于 DB 包）
// 对齐 IDA: 先写 nErrorCode，再写其余字段
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME& value) {
    packet.XParse << value.nErrorCode;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    packet << value.psStorageInfo;
    packet << value.psOutItemInfo;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(&value.psResItemMoveInfo)), sizeof(value.psResItemMoveInfo));
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value.psItemLogList_raw)), sizeof(value.psItemLogList_raw));
    packet.XParse << value.nInventorySync;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(&value.psReqItemMoveInfo)), sizeof(value.psReqItemMoveInfo));
    return packet;
}

// 联赛仓库移动请求序列化（用于 SendPacket）
inline XPacket& operator<<(XPacket& packet, const PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME& value) {
    packet.XParse << value.nErrorCode;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value._pad0)), sizeof(value._pad0));
    packet << value.psStorageInfo;
    packet << value.psOutItemInfo;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(&value.psResItemMoveInfo)), sizeof(value.psResItemMoveInfo));
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(value.psItemLogList_raw)), sizeof(value.psItemLogList_raw));
    packet.XParse << value.nInventorySync;
    packet.XParse.GetBytes(const_cast<char*>(reinterpret_cast<const char*>(&value.psReqItemMoveInfo)), sizeof(value.psReqItemMoveInfo));
    return packet;
}

// League board (192 bytes)
struct ST_LEAGUE_BOARD {
    std::int32_t nSerial = 0;
    std::int32_t nLeagueID = 0;
    wchar_t szCharName[21] = {};
    wchar_t szMsg[61] = {};
    std::int64_t biEnrollDate = 0;
    std::int32_t nResult = 0;
};

// League applicant (80 bytes)
struct ST_LEAGUE_APPLICANT {
    std::int32_t nLeagueID = 0;
    std::uint32_t dwActorID = 0;
    wchar_t szName[21] = {};
    std::int16_t shLevel = 0;
    std::uint8_t _pad0[4] = {};
    std::int64_t biApplicantDate = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad1[2] = {};
    std::uint32_t dwProfilePhotoID = 0;
    std::int32_t nResult = 0;
};

// League applicant accept request (72 bytes)
struct ST_REQ_LEAGUE_APPLICANT_ACCEPT {
    std::uint32_t dwReqActorID = 0;
    wchar_t szReqName[21] = {};
    std::int32_t nLeagueID = 0;
    std::uint8_t _pad0[4] = {};
    std::int64_t biJoinDate = 0;
    std::int32_t nResult = 0;
};

// League applicant reject request (16 bytes)
struct ST_REQ_LEAGUE_APPLICANT_REJECT {
    std::int32_t nLeagueID = 0;
    std::uint32_t dwTargetUCID = 0;
    std::uint32_t dwUCID = 0;
    std::int32_t nResult = 0;
};

// League wealth for server (40 bytes)
struct PS_LEAGUE_WEALTH_FOR_SERVER {
    std::uint32_t dwUCID = 0;
    std::int32_t nLeagueID = 0;
    std::int32_t nTotalExp = 0;
    std::int16_t shExp = 0;
    std::uint8_t _pad0[2] = {};
    std::int32_t nGold = 0;
    std::uint8_t _pad1[4] = {};
    std::int64_t biPrevExp = 0;
    std::int32_t nErrorCode = 0;
};

// Sync league info (16 bytes)
struct PS_SYNC_LEAGUE_INFO {
    std::int32_t nLeagueID = 0;
    std::uint32_t dwUCID = 0;
    std::int32_t nSyncCount = 0;
    bool bSync = false;
};

// League notice (1624 bytes)
struct ST_LEAGUE_NOTICE {
    std::int32_t nLeagueID = 0;
    wchar_t szNotice[801] = {};
    std::int64_t biEnrollDate = 0;
    std::int32_t nResult = 0;
};

// League auth change (76 bytes)
struct ST_LEAGUE_AUTH_CHANGE {
    std::int32_t nAuth[9] = {};
    std::int32_t nLimitGoldOut[9] = {};
    std::int32_t nResult = 0;
};

// League position name change (56 bytes)
struct ST_LEAGUE_POSITION_NAME_CHANGE {
    std::int32_t nPosition = 0;
    wchar_t szLeagueName[11] = {};
    std::int32_t nResult = 0;
    wchar_t szPrevPositionName[11] = {};
};

// League open state (8 bytes)
struct ST_LEAGUE_OPEN {
    std::int32_t nLeagueID = 0;
    bool bOpen = false;
};

// League recruit notice (128 bytes)
struct ST_LEAGUE_RECRUIT_NOTICE {
    std::int32_t nLeagueID = 0;
    wchar_t szNotice[51] = {};
    std::uint8_t _pad0[4] = {};
    std::int64_t biRegDate = 0;
    std::int32_t nResult = 0;
};

// League delegate request (12 bytes)
struct PS_REQ_LEAGUE_DELEGATE {
    std::int32_t nLeagueID = 0;
    std::uint32_t dwNpcID = 0;
    std::uint32_t dwDelegatedUCID = 0;
};

// League card change request (16 bytes) - 对齐 IDA
struct PS_REQ_LEAGUE_CARD {
    std::int32_t nLeagueID = 0;
    std::int16_t shSlot = 0;
    std::uint8_t _pad0[2] = {};  // padding to 0x8
    std::uint32_t dwLeagueCard = 0;  // union ___u2, 同 ST_LEAGUE_INFO 中的字段
    std::int32_t nResult = 0;
};

// League name change for server (80 bytes)
struct PS_LEAGUE_NAME_CHANGE_SERVER {
    std::uint32_t dwUCID = 0;
    std::int32_t nLeagueID = 0;
    PS_RES_STORAGE_INFO psUpdateItemList{};
    wchar_t szLeagueName[10] = {};
    std::uint32_t dwServerID = 0;
    std::int32_t nSysnCount = 0;
    std::int32_t nResult = 0;
};

// League skill learn request (12 bytes)
struct PS_REQ_LEAGUE_SKILL {
    std::int32_t nLeagueID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t bySkillIndex = 0;
    std::uint8_t byType = 0;
};

// League skill enumeration - 对齐 IDA bySkill[8] 索引
enum E_LEAGUE_SKILL {
    E_LEAGUE_SKILL_NONE = 0,
    E_LEAGUE_SKILL_1 = 1,
    E_SKILL_CARD = 2,       // 卡片技能（IDA 确认 CheckLeagueCardChange 传入 edx=2）
    E_LEAGUE_SKILL_3 = 3,
    E_LEAGUE_SKILL_4 = 4,
    E_LEAGUE_SKILL_5 = 5,
    E_LEAGUE_SKILL_6 = 6,
    E_LEAGUE_SKILL_7 = 7,
    E_LEAGUE_SKILL_MAX = 8
};

// League skill learn response (32 bytes) - 对齐 IDA
struct PS_RES_LEAGUE_SKILL {
    std::int32_t nLeagueID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t bySkillIndex = 0;
    std::uint8_t bySkillGroupID = 0;
    std::uint8_t bySkillLevel = 0;
    std::uint8_t bySkillPoint = 0;
    std::int64_t biGold = 0;
    std::int32_t nResult = 0;
};

inline XPacket& operator<<(XPacket& packet, const PS_RES_LEAGUE_SKILL& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.bySkillIndex;
    packet.XParse << value.bySkillGroupID;
    packet.XParse << value.bySkillLevel;
    packet.XParse << value.bySkillPoint;
    packet.XParse << value.biGold;
    packet.XParse << value.nResult;
    return packet;
}

// PS_RES_LEAGUE_SKILL 输入序列化
inline void operator>>(XPacket& packet, PS_RES_LEAGUE_SKILL& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.bySkillIndex;
    packet.XParse >> value.bySkillGroupID;
    packet.XParse >> value.bySkillLevel;
    packet.XParse >> value.bySkillPoint;
    packet.XParse >> value.biGold;
    packet.XParse >> value.nResult;
}

// Auto skill structure - 对齐 IDA PS_AUTO_SKILL (8 bytes)
struct PS_AUTO_SKILL {
    std::uint8_t bySkillInfo[8] = {};  // 8 个技能组等级
};

// PS_AUTO_SKILL 输出序列化
inline XPacket& operator<<(XPacket& packet, const PS_AUTO_SKILL& value) {
    for (int i = 0; i < 8; ++i) {
        packet.XParse << value.bySkillInfo[i];
    }
    return packet;
}

// PS_AUTO_SKILL 输入序列化
inline void operator>>(XPacket& packet, PS_AUTO_SKILL& value) {
    for (int i = 0; i < 8; ++i) {
        packet.XParse >> value.bySkillInfo[i];
    }
}

// League delegate response (24 bytes)
struct PS_RES_LEAGUE_DELEGATE {
    std::int32_t nLeagueID = 0;
    wchar_t szDelegatedName[21] = {};  // 被转让者名称
    wchar_t szDelegateName[21] = {};   // 原会长名称
    std::int32_t nResult = 0;
};

// 联赛成员更新结构（72字节）
struct ST_LEAGUE_MEMBER_UPDATE {
    std::int32_t nLeagueID = 0;
    std::uint32_t dwActorID = 0;
    bool bLogin = false;
    std::uint8_t byLevel = 0;
    std::int16_t sWorld = 0;
    std::uint8_t _pad0[4] = {};
    std::int64_t biPlayDate = 0;
    wchar_t szName[21] = {};
    std::uint8_t byChannel = 0;
    std::uint8_t byAwaken = 0;
    std::uint32_t dwProfilePhotoID = 0;
};

// League info for game client - 对齐 IDA ST_LEAGUE_INFO_FOR_GAME (52 bytes)
struct ST_LEAGUE_INFO_FOR_GAME {
    std::uint32_t dwMasterUCID = 0;      // offset 0x0
    std::uint8_t byLeagueLevel = 0;      // offset 0x4
    std::uint8_t byPosition = 0;         // offset 0x5
    std::uint8_t bySkillInfo[8] = {};    // offset 0x6
    std::int32_t nAuth[9] = {};          // offset 0x10
};

// 联赛信息扩展结构（36字节）
struct ST_LEAGUE_INFO_EX {
    std::uint32_t dwUCID = 0;
    std::int32_t nLeagueID = 0;
    wchar_t szLeagueName[10] = {};
    std::int32_t nMemberCount = 0;
    std::uint32_t dwLeagueCard = 0;  // 对齐 IDA: 联赛卡片ID
};

// League record (112 bytes)
struct ST_LEAGUE_RECORD {
    std::int32_t nLeagueID = 0;
    std::uint8_t byFlag = 0;
    std::uint8_t _pad0[3] = {};
    std::int64_t biRegisterDate = 0;
    wchar_t szValue1[21] = {};
    wchar_t szValue2[21] = {};
    std::int32_t nValue3 = 0;
    std::int32_t nValue4 = 0;
};

// League member position change (12 bytes)
struct ST_LEAGUE_MEMBER_POSITION {
    std::uint32_t dwActorID = 0;
    std::uint8_t byPosition = 0;
    std::uint8_t byState = 0;
    std::int32_t nResult = 0;
};

// League inventory info request (12 bytes)
struct PS_REQ_LEAGUE_INVEN_INFO {
    std::int32_t nLeagueID = 0;
    std::uint32_t dwNpcID = 0;
    std::int16_t shStartPos = 0;
    std::int16_t shEndPos = 0;
};

// Chat league message (524 bytes)
struct PS_CHAT_LEAGUE {
    std::uint32_t dwActorID = 0;
    std::uint32_t dwLeagueID = 0;
    std::uint32_t dwMemberID = 0;
    wchar_t szMsg[256] = {};
};

// DB league load request context
struct PS_DB_LEAGUE_LOAD {
    std::int32_t nLeagueID = 0;
    std::uint32_t dwServerID = 0;
    std::uint32_t dwUCID = 0;
    std::int32_t nLoadType = 0;
    ST_LEAGUE_APPLICANT stApplicant{};
};

// GMT league update list
struct PS_GMT_LEAGUE_UPDATE_LIST {
    std::int32_t nCount = 0;
};

inline void operator>>(XPacket& packet, PS_DB_LEAGUE_LOAD& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwServerID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nLoadType;
}

// 对齐 IDA: AddLeagueUser 发送 DB 包时使用
inline XPacket& operator<<(XPacket& packet, const PS_DB_LEAGUE_LOAD& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwServerID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.nLoadType;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_LEAGUE_LOAD& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwServerID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.nLoadType;
    return packet;
}

inline void operator>>(XPacket& packet, PS_GMT_LEAGUE_UPDATE_LIST& value) {
    packet.XParse >> value.nCount;
}

// Serializers for league structures
inline void operator>>(XPacket& packet, ST_REQ_LEAGUE_CREATE& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.szName, 10, outLen);
    packet.XParse >> value.dwNpcID;
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.nErrorCode;
}

inline void operator>>(XPacket& packet, PS_LEAGUE_CREATE_FOR_SERVER& value) {
    packet >> value.stCreateInfo;
    packet.XParse >> value.dwActorID;
    short outLen = 0;
    packet.XParse.GetWString(value.szMasterName, 21, outLen);
    packet.XParse >> value.nCreateDate;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.byLevel;
    packet.XParse >> value.sWorldID;
    packet.XParse >> value.nAuth_Elder;
    packet.XParse >> value.nAuth_Manager;
    packet.XParse >> value.nAuth_SubMaster;
    packet.XParse >> value.nServerID;
}

// ST_REQ_LEAGUE_CREATE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_CREATE& value) {
    packet.XParse << GreenDamTan_BoundedWideString(value.szName);
    packet.XParse << value.dwNpcID;
    packet.XParse << value.nLeagueID;
    packet.XParse << value.nErrorCode;
    return packet;
}

// 联赛创建包序列化输出
inline XPacket& operator<<(XPacket& packet, const PS_LEAGUE_CREATE_FOR_SERVER& value) {
    packet << value.stCreateInfo;
    packet.XParse << value.dwActorID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szMasterName);
    packet.XParse << value.nCreateDate;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.byLevel;
    packet.XParse << value.sWorldID;
    packet.XParse << value.nAuth_Elder;
    packet.XParse << value.nAuth_Manager;
    packet.XParse << value.nAuth_SubMaster;
    packet.XParse << value.nServerID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_REQ_LEAGUE_INVITE& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.szLeagueName, 10, outLen);
    packet.XParse.GetWString(value.szTargetName, 21, outLen);
    packet.XParse.GetWString(value.szReqName, 21, outLen);
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwTargetActorID;
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.nResult;
}

inline void operator>>(XPacket& packet, ST_REQ_LEAGUE_INVITE_ACCEPT& value) {
    packet.XParse >> value.dwReqUCID;
    packet.XParse >> value.dwTargetUCID;
    short outLen = 0;
    packet.XParse.GetWString(value.szTargetName, 21, outLen);
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.nResult;
}

inline void operator>>(XPacket& packet, ST_REQ_LEAGUE_INVITE_REJECT& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.szTargetName, 21, outLen);
    packet.XParse >> value.dwReqUCID;
    packet.XParse >> value.dwTargetUCID;
    packet.XParse >> value.nResult;
}

inline void operator>>(XPacket& packet, ST_REQ_LEAGUE_SEARCH& value) {
    packet.XParse >> value.nState;
    short outLen = 0;
    packet.XParse.GetWString(value.szLeagueName, 10, outLen);
    packet.XParse.GetWString(value.szMasterName, 21, outLen);
}

inline void operator>>(XPacket& packet, ST_LEAGUE_BOARD& value) {
    packet.XParse >> value.nSerial;
    packet.XParse >> value.nLeagueID;
    short outLen = 0;
    packet.XParse.GetWString(value.szCharName, 21, outLen);
    packet.XParse.GetWString(value.szMsg, 61, outLen);
    packet.XParse >> value.biEnrollDate;
    packet.XParse >> value.nResult;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_APPLICANT& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwActorID;
    short outLen = 0;
    packet.XParse.GetWString(value.szName, 21, outLen);
    packet.XParse >> value.shLevel;
    packet.XParse >> value.biApplicantDate;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.nResult;
}

inline void operator>>(XPacket& packet, ST_REQ_LEAGUE_APPLICANT_ACCEPT& value) {
    packet.XParse >> value.dwReqActorID;
    short outLen = 0;
    packet.XParse.GetWString(value.szReqName, 21, outLen);
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.biJoinDate;
    packet.XParse >> value.nResult;
}

inline void operator>>(XPacket& packet, ST_REQ_LEAGUE_APPLICANT_REJECT& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwTargetUCID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nResult;
}

inline void operator>>(XPacket& packet, PS_LEAGUE_WEALTH_FOR_SERVER& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.nTotalExp;
    packet.XParse >> value.shExp;
    packet.XParse >> value.nGold;
    packet.XParse >> value.biPrevExp;
    packet.XParse >> value.nErrorCode;
}

inline XPacket& operator<<(XPacket& packet, const PS_LEAGUE_WEALTH_FOR_SERVER& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nLeagueID;
    packet.XParse << value.nTotalExp;
    packet.XParse << value.shExp;
    packet.XParse << value.nGold;
    packet.XParse << value.biPrevExp;
    packet.XParse << value.nErrorCode;
    return packet;
}

inline void operator>>(XPacket& packet, PS_SYNC_LEAGUE_INFO& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nSyncCount;
    packet.XParse >> value.bSync;
}

inline XPacket& operator<<(XPacket& packet, const PS_SYNC_LEAGUE_INFO& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.nSyncCount;
    packet.XParse << value.bSync;
    return packet;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_NOTICE& value) {
    packet.XParse >> value.nLeagueID;
    short outLen = 0;
    packet.XParse.GetWString(value.szNotice, 801, outLen);
    packet.XParse >> value.biEnrollDate;
    packet.XParse >> value.nResult;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_AUTH_CHANGE& value) {
    for (int i = 0; i < 9; ++i) {
        packet.XParse >> value.nAuth[i];
    }
    for (int i = 0; i < 9; ++i) {
        packet.XParse >> value.nLimitGoldOut[i];
    }
    packet.XParse >> value.nResult;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_POSITION_NAME_CHANGE& value) {
    packet.XParse >> value.nPosition;
    short outLen = 0;
    packet.XParse.GetWString(value.szLeagueName, 11, outLen);
    packet.XParse >> value.nResult;
    packet.XParse.GetWString(value.szPrevPositionName, 11, outLen);
}

// 对齐 IDA: ST_LEAGUE_AUTH_CHANGE 输出序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_LEAGUE_AUTH_CHANGE& value) {
    for (int i = 0; i < 9; ++i) {
        packet.XParse << value.nAuth[i];
    }
    for (int i = 0; i < 9; ++i) {
        packet.XParse << value.nLimitGoldOut[i];
    }
    packet.XParse << value.nResult;
    return packet;
}

// 对齐 IDA: ST_LEAGUE_POSITION_NAME_CHANGE 输出序列化
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_LEAGUE_POSITION_NAME_CHANGE& value) {
    packet.XParse << value.nPosition;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << value.nResult;
    packet.XParse << GreenDamTan_BoundedWideString(value.szPrevPositionName);
    return packet;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_OPEN& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.bOpen;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_RECRUIT_NOTICE& value) {
    packet.XParse >> value.nLeagueID;
    short outLen = 0;
    packet.XParse.GetWString(value.szNotice, 51, outLen);
    packet.XParse >> value.biRegDate;
    packet.XParse >> value.nResult;
}

inline void operator>>(XPacket& packet, PS_REQ_LEAGUE_DELEGATE& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwNpcID;
    packet.XParse >> value.dwDelegatedUCID;
}

inline void operator>>(XPacket& packet, PS_REQ_LEAGUE_CARD& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwLeagueCard;
    packet.XParse >> value.shSlot;
    packet.XParse >> value.nResult;
}

inline XPacket& operator<<(XPacket& packet, const PS_REQ_LEAGUE_CARD& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwLeagueCard;
    packet.XParse << value.shSlot;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_LEAGUE_NAME_CHANGE_SERVER& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nLeagueID;
    packet >> value.psUpdateItemList;
    short outLen = 0;
    packet.XParse.GetWString(value.szLeagueName, 10, outLen);
    packet.XParse >> value.dwServerID;
    packet.XParse >> value.nSysnCount;
    packet.XParse >> value.nResult;
}

// 对齐 IDA: SendChangeLeagueName operator<< 序列化
inline XPacket& operator<<(XPacket& packet, const PS_LEAGUE_NAME_CHANGE_SERVER& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nLeagueID;
    packet << value.psUpdateItemList;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << value.dwServerID;
    packet.XParse << value.nSysnCount;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_LEAGUE_SKILL& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.bySkillIndex;
    packet.XParse >> value.byType;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_RECORD& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.byFlag;
    packet.XParse >> value.biRegisterDate;
    short outLen = 0;
    packet.XParse.GetWString(value.szValue1, 21, outLen);
    packet.XParse.GetWString(value.szValue2, 21, outLen);
    packet.XParse >> value.nValue3;
    packet.XParse >> value.nValue4;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_MEMBER_POSITION& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.byPosition;
    packet.XParse >> value.byState;
    packet.XParse >> value.nResult;
}

inline void operator>>(XPacket& packet, PS_REQ_LEAGUE_INVEN_INFO& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwNpcID;
    packet.XParse >> value.shStartPos;
    packet.XParse >> value.shEndPos;
}

// 对齐 IDA: ReqLeagueInevntoryInfo 0x140080860 调用 operator<<(XPacket&, PS_REQ_LEAGUE_INVEN_INFO)
inline XPacket& operator<<(XPacket& packet, const PS_REQ_LEAGUE_INVEN_INFO& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwNpcID;
    packet.XParse << value.shStartPos;
    packet.XParse << value.shEndPos;
    return packet;
}

inline void operator>>(XPacket& packet, PS_CHAT_LEAGUE& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwLeagueID;
    packet.XParse >> value.dwMemberID;
    short outLen = 0;
    packet.XParse.GetWString(value.szMsg, 256, outLen);
}

inline void operator>>(XPacket& packet, ST_LEAGUE_MEMBER& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.byPosition;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.biLeagueExp;
    packet.XParse >> value.biJoinDate;
    packet.XParse >> value.biApplicationDate;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_MEMBER_EX& value) {
    packet >> value.stMember;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&value.bLogin), 1);
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad1), sizeof(value._pad1));
    packet.XParse >> value.dwUCID;
    short outLen = 0;
    packet.XParse.GetWString(value.szName, 21, outLen);
    packet.XParse >> value.shLevel;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad2), sizeof(value._pad2));
    packet.XParse >> value.biBoardLimitTime;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad3), sizeof(value._pad3));
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad4), sizeof(value._pad4));
    packet.XParse >> value.biPlayDate;
}

// ============================================================================
// 对齐 IDA: 好友/黑名单结构体序列化（DB 格式）
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

// ST_CREATE_ITEM 对齐 IDA 0x1400EAA00/0x1400EAA70
inline XPacket& operator<<(XPacket& packet, const ST_CREATE_ITEM& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value.byUpgrade;
    return packet;
}

inline void operator>>(XPacket& packet, ST_CREATE_ITEM& value) {
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse >> value.byUpgrade;
}

/**
 * @brief 创建物品列表
 * Per IDA: ST_CREATE_ITEMS - 32 bytes
 */
struct ST_CREATE_ITEMS {
    std::vector<ST_CREATE_ITEM> vecInfo;
};

inline void operator>>(XPacket& packet, ST_CREATE_ITEMS& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        ST_CREATE_ITEM info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_CREATE_ITEMS& value) {
    packet.XParse << static_cast<std::uint16_t>(value.vecInfo.size());
    for (const ST_CREATE_ITEM& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

/**
 * @brief 物品使用获取信息
 * Per IDA 0x140052710: ST_GET_INFO - 72 bytes
 */
struct ST_GET_INFO {
    std::int32_t nExp = 0;                      // +0x00: 经验值 (4 bytes)
    std::int32_t nMoney = 0;                   // +0x04: 金币 (4 bytes)
    std::int32_t nBP = 0;                      // +0x08: BP (4 bytes)
    std::int32_t nEther = 0;                   // +0x0C: Ether (4 bytes)
    std::int32_t nTitleID[2] = {0, 0};         // +0x10: 称号ID数组 (8 bytes)
    std::uint32_t dwHelperID = 0;              // +0x18: HelperID (4 bytes)
    ST_CREATE_ITEMS stGetItem;                 // +0x20: 获取物品列表 (32 bytes)
    std::int32_t nRenovatePoint = 0;           // +0x40: 翻新点数 (4 bytes)
    std::int32_t nRenovatePointTotal = 0;      // +0x44: 翻新总点数 (4 bytes)
};
static_assert(sizeof(ST_GET_INFO) == 72, "ST_GET_INFO size must match IDA");

inline void operator>>(XPacket& packet, ST_GET_INFO& value) {
    packet.XParse >> value.nExp;
    packet.XParse >> value.nMoney;
    packet.XParse >> value.nBP;
    packet.XParse >> value.nEther;
    packet.XParse >> value.nTitleID[0];
    packet.XParse >> value.nTitleID[1];
    packet.XParse >> value.dwHelperID;
    packet >> value.stGetItem;
    packet.XParse >> value.nRenovatePoint;
    packet.XParse >> value.nRenovatePointTotal;
}

inline XPacket& operator<<(XPacket& packet, const ST_GET_INFO& value) {
    packet.XParse << value.nExp;
    packet.XParse << value.nMoney;
    packet.XParse << value.nBP;
    packet.XParse << value.nEther;
    packet.XParse << value.nTitleID[0];
    packet.XParse << value.nTitleID[1];
    packet.XParse << value.dwHelperID;
    packet << value.stGetItem;
    packet.XParse << value.nRenovatePoint;
    packet.XParse << value.nRenovatePointTotal;
    return packet;
}

/**
 * @brief 物品数量盒子更新请求
 * Per IDA 0x1400592C0: PS_DB_ITEM_COUNTBOX - 224 bytes
 */
struct PS_DB_ITEM_COUNTBOX {
    std::uint32_t dwUCID = 0;                  // +0x00: UCID (4 bytes)
    std::uint8_t _pad0[4] = {};                // +0x04: padding (4 bytes)
    PS_RES_STORAGE_INFO psUpdateItemList;     // +0x08: 更新物品列表 (40 bytes)
    PS_RES_STORAGE_INFO psCreateItemList;     // +0x30: 创建物品列表 (40 bytes)
    PS_RES_STORAGE_INFO psCountboxItem;       // +0x58: Countbox物品 (40 bytes)
    ST_GET_INFO stGetInfo;                    // +0x80: 获取信息 (72 bytes)
    std::uint32_t dwUseItemID = 0;            // +0xC8: 使用物品ID (4 bytes)
    std::int64_t biUseItemSerial = 0;         // +0xD0: 使用物品序列号 (8 bytes)
    std::int32_t nErrorCode = 0;              // +0xD8: 错误码 (4 bytes)
};
static_assert(sizeof(PS_DB_ITEM_COUNTBOX) == 224, "PS_DB_ITEM_COUNTBOX size must match IDA");

inline void operator>>(XPacket& packet, PS_DB_ITEM_COUNTBOX& value) {
    packet.XParse >> value.dwUCID;
    packet >> value.psUpdateItemList;
    packet >> value.psCreateItemList;
    packet >> value.psCountboxItem;
    packet >> value.stGetInfo;
    packet.XParse >> value.dwUseItemID;
    packet.XParse >> value.biUseItemSerial;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_COUNTBOX& value) {
    packet.XParse << value.dwUCID;
    packet << value.psUpdateItemList;
    packet << value.psCreateItemList;
    packet << value.psCountboxItem;
    packet << value.stGetInfo;
    packet.XParse << value.dwUseItemID;
    packet.XParse << value.biUseItemSerial;
    packet.XParse << value.nErrorCode;
    return packet;
}

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

// ST_EXCHANGE_PRICE_INFO 对齐 IDA 0x1400E9940/0x1400E9A00
inline XPacket& operator<<(XPacket& packet, const ST_EXCHANGE_PRICE_INFO& value) {
    packet.XParse << value.dwItemID;
    packet.XParse << value.sCount;
    packet.XParse << value.nPrice_One;
    packet.XParse << value.tRegDate;
    packet.XParse << GreenDamTan_BoundedWideString(value.strBuyerName);
    return packet;
}

inline void operator>>(XPacket& packet, ST_EXCHANGE_PRICE_INFO& value) {
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.sCount;
    packet.XParse >> value.nPrice_One;
    packet.XParse >> value.tRegDate;
    short sLen = 0;
    packet.XParse.GetWString(value.strBuyerName, 21, sLen);
}

// PS_EXCHANGE_PRICE_HISTORY_REQ 对齐 IDA 0x1400C9920（与 PS_RES_RECRUIT_DELETE 同布局）
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_PRICE_HISTORY_REQ& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwItemID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_EXCHANGE_PRICE_HISTORY_REQ& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwItemID;
}

// PS_EXCHANGE_PRICE_HISTORY_RES 对齐 IDA 0x1400E9AB0/0x1400E9BC0
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_PRICE_HISTORY_RES& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwItemID;
    const std::int16_t count = static_cast<std::int16_t>(std::min<std::size_t>(value.vecHistory.size(), 0x7FFF));
    packet.XParse << count;
    for (std::int16_t i = 0; i < count; ++i) {
        packet << value.vecHistory[static_cast<std::size_t>(i)];
    }
    packet.XParse << value.n64Price_High;
    packet.XParse << value.n64Price_Low;
    packet.XParse << value.n64Price_Avg;
    return packet;
}

inline void operator>>(XPacket& packet, PS_EXCHANGE_PRICE_HISTORY_RES& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwItemID;
    std::int16_t count = 0;
    packet.XParse >> count;
    value.vecHistory.clear();
    value.vecHistory.reserve(static_cast<std::size_t>(count));
    for (std::int16_t i = 0; i < count; ++i) {
        ST_EXCHANGE_PRICE_INFO item{};
        packet >> item;
        value.vecHistory.push_back(item);
    }
    packet.XParse >> value.n64Price_High;
    packet.XParse >> value.n64Price_Low;
    packet.XParse >> value.n64Price_Avg;
}

// PS_DB_EXCHANGE_PRICE_HISTORY_RES 对齐 IDA 0x1400E9D70
inline XPacket& operator<<(XPacket& packet, const PS_DB_EXCHANGE_PRICE_HISTORY_RES& value) {
    packet << value.stRes;
    packet.XParse << value.nTotalCount;
    packet.XParse << value.n64TotalPrice;
    return packet;
}

inline void operator>>(XPacket& packet, PS_DB_EXCHANGE_PRICE_HISTORY_RES& value) {
    packet >> value.stRes;
    packet.XParse >> value.nTotalCount;
    packet.XParse >> value.n64TotalPrice;
}

// ============================================================================
// Exchange (交易所) 序列化操作符
// ============================================================================

// PS_EXCHANGE_SEARCH_REQ 对齐 IDA 0x140038880
inline void operator>>(XPacket& packet, PS_EXCHANGE_SEARCH_REQ& value) {
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.nCategoryID;
    packet.XParse >> value.nSubCategoryID;
    packet.XParse >> value.nUseClass;
    packet.XParse >> value.nLevelMin;
    packet.XParse >> value.nLevelMax;
    packet.XParse >> value.nItemGradeMin;
    packet.XParse >> value.nItemGradeMax;
    packet.XParse >> value.nPrice;
    packet.XParse >> value.nUpgrade;
    packet.XParse >> value.nPageNum;
}

// PS_EXCHANGE_SEARCH_RES 对齐 IDA 0x140038880
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_SEARCH_RES& value) {
    packet.XParse << value.nPageMax;
    packet.XParse << value.nPage;
    packet.XParse << static_cast<std::uint8_t>(value.vecItem.size());
    for (const ST_EXCHANGE_ITEM& item : value.vecItem) {
        packet.XParse << item.dwExchangeID;
        packet.XParse << item.dwItemID;
        packet.XParse << item.xSerial;
        packet.XParse << item.sCount;
        packet.XParse << item.byUpgrade;
        for (const ST_EXCHANGE_EXTEND_OPTION& opt : item.stExtendOption) {
            packet.XParse << opt.byType;
            packet.XParse << opt.nOption;
        }
        packet.XParse << item.bySocketActiveCount;
        packet.XParse << item.byUpgradeCount;
        packet.XParse << item.byUpgradeLimit;
        packet.XParse << item.byEndurance;
        packet.XParse << item.byRestoreCount;
        packet.XParse << item.bySealCount;
        packet.XParse << item.bySealDelCount;
        packet.XParse << GreenDamTan_BoundedString(item.szBroachState);
        packet.XParse << item.nAttack;
        packet.XParse << item.nDefense;
        packet.XParse << item.nTitleID;
        packet.XParse << item.byUseCount;
        packet.XParse << item.nDyeID;
        packet.XParse << item.nPrice_One;
        packet.XParse << item.byCash_Commission;
        packet.XParse << item.stBroachInfo.biSerial;
        for (std::uint32_t id : item.stBroachInfo.dwItemID) {
            packet.XParse << id;
        }
        packet.XParse << GreenDamTan_BoundedWideString(item.strSellerName);
        packet.XParse << item.nExpireRemainTime;
    }
    return packet;
}

// PS_EXCHANGE_INTEREST_LIST_REQ 对齐 IDA 0x1400399C0
inline void operator>>(XPacket& packet, PS_EXCHANGE_INTEREST_LIST_REQ& value) {
    packet.XParse >> value.dwUCID;
}

// PS_EXCHANGE_INTEREST_LIST_RES 对齐 IDA 0x1400399C0
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_INTEREST_LIST_RES& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << static_cast<std::uint8_t>(value.vecItemList.size());
    for (std::uint32_t id : value.vecItemList) {
        packet.XParse << id;
    }
    return packet;
}

// PS_EXCHANGE_MY_LIST_RES 对齐 IDA 0x14003B340
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_MY_LIST_RES& value) {
    packet.XParse << value.bLast;
    packet.XParse << static_cast<std::uint8_t>(value.vecMyList.size());
    for (const ST_MY_EXCHANGE_ITEM& item : value.vecMyList) {
        packet.XParse << item.dwExchangeID;
        packet.XParse << item.dwItemID;
        packet.XParse << item.xSerial;
        packet.XParse << item.sCount;
        packet.XParse << item.byUpgrade;
        for (const ST_EXCHANGE_EXTEND_OPTION& opt : item.stExtendOption) {
            packet.XParse << opt.byType;
            packet.XParse << opt.nOption;
        }
        packet.XParse << item.bySocketActiveCount;
        packet.XParse << item.byUpgradeCount;
        packet.XParse << item.byUpgradeLimit;
        packet.XParse << item.byEndurance;
        packet.XParse << item.byRestoreCount;
        packet.XParse << item.bySealCount;
        packet.XParse << item.bySealDelCount;
        packet.XParse << GreenDamTan_BoundedString(item.szBroachState);
        packet.XParse << item.nAttack;
        packet.XParse << item.nDefense;
        packet.XParse << item.nTitleID;
        packet.XParse << item.byUseCount;
        packet.XParse << item.nDyeID;
        packet.XParse << item.nPrice_One;
        packet.XParse << item.byCash_Commission;
        packet.XParse << item.stBroachInfo.biSerial;
        for (std::uint32_t id : item.stBroachInfo.dwItemID) {
            packet.XParse << id;
        }
        packet.XParse << item.nExpireRemainTime;
    }
    return packet;
}

// PS_EXCHANGE_ITEM_BUY_REQ 对齐 IDA 0x14003B5A0
inline void operator>>(XPacket& packet, PS_EXCHANGE_ITEM_BUY_REQ& value) {
    packet.XParse >> value.dwExchangeID;
    packet.XParse >> value.shCount;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_EXCHANGE_ITEM_BUY_REQ& value) {
    packet.XParse << value.dwExchangeID;
    packet.XParse << value.shCount;
    return packet;
}

// PS_EXCHANGE_ITEM_BUY_RES 对齐 IDA 0x14003B5A0
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_ITEM_BUY_RES& value) {
    packet.XParse << value.dwExchangeID;
    packet.XParse << value.shCount;
    packet.XParse << value.biPrice;
    packet.XParse << value.nPackageCount;
    return packet;
}

// PS_EXCHANGE_INTEREST_ITEM_REQ 对齐 IDA 0x140039BF0
inline void operator>>(XPacket& packet, PS_EXCHANGE_INTEREST_ITEM_REQ& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.bAdd;
}

// PS_EXCHANGE_INTEREST_ITEM_RES 对齐 IDA 0x140039BF0
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_INTEREST_ITEM_RES& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwItemID;
    packet.XParse << value.bAdd;
    packet.XParse << value.nResult;
    return packet;
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

// ============================================================
// 对齐 IDA: 交易所价格更新 / 邮件系统 / 花粉互助 结构体
// ============================================================

// 对齐 IDA 0x1400EA170: 联赛仓库日志条目
struct PS_LEAGUE_INVENTORY_FOR_LOG {
    std::int32_t nItemID = 0;
    std::int16_t shItemCount = 0;
    std::int16_t shPos = 0;
    std::int64_t biSerial = 0;
    std::int32_t nAttack = 0;
    std::int32_t nDefense = 0;
    std::int32_t nItemTitleID = 0;
};

// 对齐 IDA 0x1400EB480: 联赛仓库日志列表
struct PS_LEAGUE_INVENTORY_FOR_LOG_LIST {
    std::vector<PS_LEAGUE_INVENTORY_FOR_LOG> vecInfo;
};

// 对齐 IDA 0x1400E2310: 邮件发送者角色信息
struct ST_POST_CHAR {
    std::uint32_t dwUCID = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad0[2] = {};
    std::uint32_t dwProfilePhotoID = 0;
    wchar_t strName[21] = {};
};

// 对齐 IDA 0x1400E25D0: 邮件数据（含附件物品）
struct ST_POST_DATA {
    std::int64_t biSerial = 0;             // 邮件序列号
    ST_POST_CHAR stCharInfo{};             // 发送者信息
    wchar_t strTitle[41] = {};             // 邮件标题
    wchar_t strMsg[401] = {};              // 邮件内容
    std::int64_t biMoney = 0;              // 附加金钱
    STItem stItemList[5] = {};             // 附加物品列表
    std::int64_t nRegTime = 0;             // 注册时间
    std::uint8_t byFlag = 0;               // 标记
    std::uint8_t byPostType = 0;           // 邮件类型
    std::uint8_t byPostSubType = 0;        // 邮件子类型
    std::uint8_t _pad1[5] = {};
    std::int64_t nRemainTime = 0;          // 剩余时间
    std::int64_t biEventID = 0;            // 活动ID
    PS_ITEM_SOCKET_LIST vecSocketList{};   // 镶嵌列表
    PS_ITEM_BROACH_LIST vecBroachList{};   // 镂刻列表
    PS_ITEM_PACKAGE_LIST vecPackageList{}; // 套装列表
};

// 对齐 IDA 0x1400E2780: 账号邮件数据（用于账号级邮件列表）
// Size: 1672 bytes (0x688)
struct ST_ACCOUNT_POST_DATA {
    std::int64_t biSerial = 0;             // offset 0, 邮件序列号
    std::uint32_t dwUAID = 0;              // offset 8, 账号ID
    std::uint8_t _pad0[4] = {};            // offset 12, padding
    wchar_t szTitle[41] = {};              // offset 16, 邮件标题 (实际 offset 12, size 82)
    wchar_t szMsg[401] = {};               // offset 98, 邮件内容 (实际 offset 94, size 802)
    std::int64_t biMoney = 0;              // offset 896, 附加金钱
    STItem stItemList[5] = {};             // offset 904, 附加物品列表 (5 * 120 = 600 bytes)
    std::int64_t biRegTime = 0;            // offset 1504, 注册时间
    std::int64_t biRemainTime = 0;         // offset 1512, 剩余时间
    std::uint8_t byFlag = 0;               // offset 1520, 标记
    std::uint8_t _pad1 = 0;                // offset 1521, padding
    wchar_t szSendName[21] = {};           // offset 1522, 发送者名字 (42 bytes)
    std::uint8_t byMainType = 0;           // offset 1564, 主类型 (设为4表示账号邮件)
    std::uint8_t bySubType = 0;            // offset 1565, 子类型
    std::uint8_t _pad2[2] = {};            // offset 1566, padding
    std::int64_t biDelDate = 0;            // offset 1568, 删除日期
    PS_ITEM_SOCKET_LIST vecSocketList{};   // offset 1576, 镶嵌列表 (32 bytes)
    PS_ITEM_BROACH_LIST vecBroachList{};   // offset 1608, 镂刻列表 (32 bytes)
    PS_ITEM_PACKAGE_LIST vecPackageList{}; // offset 1640, 套装列表 (32 bytes)
};

// 对齐 IDA 0x1400E8E10: 账号邮件列表
struct PS_ACCOUNT_POST_LIST {
    std::vector<ST_ACCOUNT_POST_DATA> vecAccountPostList;
};

// TODO: 推测结果 - 对齐 IDA 0x1400CD810 ResPostReceipt / 0x1400CDC60 ResAccountPostReceipt 使用
// Size: 16 bytes
struct PS_LOG_ITEM {
    std::int32_t nItemID = 0;
    std::int16_t shCount = 0;
    std::int16_t _pad0 = 0;
    std::int64_t biSerial = 0;
};

struct PS_LOG_ITEM_LIST {
    std::vector<PS_LOG_ITEM> vecLogItem;
};

inline void operator>>(XPacket& packet, PS_LOG_ITEM& value) {
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse >> value._pad0;
    packet.XParse >> value.biSerial;
}

inline XPacket& operator<<(XPacket& packet, const PS_LOG_ITEM& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value._pad0;
    packet.XParse << value.biSerial;
    return packet;
}

inline void operator>>(XPacket& packet, PS_LOG_ITEM_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecLogItem.clear();
    value.vecLogItem.reserve(count);
    for (std::uint8_t i = 0; i < count; ++i) {
        PS_LOG_ITEM item{};
        packet >> item;
        value.vecLogItem.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_LOG_ITEM_LIST& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecLogItem.size());
    for (const PS_LOG_ITEM& item : value.vecLogItem) {
        packet << item;
    }
    return packet;
}

// ============================================================
// Post Delete 相关结构体
// ============================================================

// 对齐 IDA: 邮件删除信息 (16 bytes)
struct PS_POST_DELETE_INFO {
    std::int64_t biSerial = 0;      // 邮件序列号
    std::int64_t biDeleteDate = 0;  // 删除日期
};

// 对齐 IDA: 邮件删除列表 (32 bytes)
struct PS_POST_DELETE_LIST {
    std::vector<PS_POST_DELETE_INFO> vecInfo;
};

// 对齐 IDA 0x1400E9BF0: 批量删除邮件请求 (88 bytes)
struct PS_POST_DELETE_ALL_SERVER {
    std::uint32_t dwUAID = 0;               // 账号ID
    std::uint32_t dwUCID = 0;               // 角色ID
    std::uint16_t wPostCount = 0;           // 邮件计数
    std::uint16_t _pad0 = 0;
    std::int32_t nErrorCode = 0;            // 错误码
    std::uint8_t byPostType = 0;            // 邮件类型: 0=收件箱, 1=发件箱, 2=已读, 3=账号邮件
    std::uint8_t _pad1[7] = {};
    PS_POST_DELETE_LIST psDeleteList;       // 删除列表
    PS_POST_DELETE_LIST psFailedList;       // 失败列表
};

// ============================================================
// Post Level Up Event 相关结构体
// ============================================================

// 对齐 IDA: 升级活动事件信息 (12 bytes)
struct ST_POST_LEVEL_UP_EVENT_INFO {
    std::int32_t nGroup = 0;    // 分组ID
    std::int32_t nLv = 0;       // 等级
    std::int32_t nUCID = 0;     // 角色ID
};

// 对齐 IDA: 升级活动事件列表 (32 bytes)
struct PS_POST_LEVEL_UP_EVENT_INFO_VEC {
    std::vector<ST_POST_LEVEL_UP_EVENT_INFO> vecInfo;
};

// 对齐 IDA: 升级活动事件更新 (16 bytes)
struct PS_POST_LEVEL_UP_EVENT_UPDATE {
    std::int32_t nUAID = 0;     // 账号ID
    std::int32_t nGroup = 0;    // 分组ID
    std::int32_t nLv = 0;       // 等级
    std::int32_t nUCID = 0;     // 角色ID
};

// ============================================================
// Post Receipt All 相关结构体
// ============================================================

// 对齐 IDA: 头像照片信息 (16 bytes)
struct ST_PROFILE_PHOTO_INFO {
    std::uint32_t dwPhotoID = 0;      // 头像ID
    std::uint8_t byPeriodType = 0;    // 周期类型
    std::uint8_t byFavorite = 0;      // 是否收藏
    std::uint8_t byState = 0;         // 状态
    std::uint8_t _pad0 = 0;           // 对齐填充
    std::int64_t nEndDate = 0;        // 结束日期
};

// 对齐 IDA 0x140028640: 头像照片加载请求/响应结构
struct PS_PROFILE_PHOTO_LOAD {
    std::uint32_t dwUCID = 0;              // 角色ID
    std::vector<ST_PROFILE_PHOTO_INFO> vecList;  // 照片列表
};

// 对齐 IDA 0x140028B70: 头像照片更新请求/响应结构
struct PS_DB_PROFILE_PHOTO_UPDATE {
    std::uint32_t dwUCID = 0;              // 角色ID
    ST_PROFILE_PHOTO_INFO stInfo;          // 照片信息
    std::int32_t nError = 0;               // 错误码
};

// 对齐 IDA 0x140028C70: 头像照片更换请求/响应结构
struct PS_DB_PROFILE_PHOTO_CHANGE {
    std::uint32_t dwUCID = 0;              // 角色ID
    ST_PROFILE_PHOTO_INFO stNewPhotoInfo;  // 新照片信息
    std::int32_t nError = 0;               // 错误码
};

// 对齐 IDA 0x1400288D0: 头像照片添加请求/响应结构
struct PS_DB_PROFILE_PHOTO_ADD {
    std::uint32_t dwUCID = 0;              // 角色ID
    ST_PROFILE_PHOTO_INFO stInfo;          // 照片信息
    std::vector<PS_STORAGE_INFO> psUpdateItemList;  // 物品更新列表
    std::int32_t nError = 0;               // 错误码
};

// 对齐 IDA: 批量领取邮件请求
struct PS_POST_RECEIPT_ALL_SERVER {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::int64_t biSerial = 0;
    std::uint8_t byFlag = 0;
    std::uint8_t byPostType = 0;
    std::uint8_t bDecrease = 0;
    std::uint8_t _pad0 = 0;
    std::int64_t biRemainTime = 0;
    std::uint16_t wPostCount = 0;
    std::int32_t nErrorCode = 0;
    PS_RES_STORAGE_INFO psCreateItem;
    PS_RES_STORAGE_INFO psUpdateItem;
    PS_RES_STORAGE_INFO psUpdateSerial;
    std::vector<ST_PROFILE_PHOTO_INFO> stAppearanceList;
};

// 对齐 IDA 0x1400D9710: 交易所价格更新
struct PS_EXCHANGE_PRICE_HISTORY_UPDATE {
    std::uint32_t dwSellerUCID = 0;        // 卖家UCID
    std::uint32_t dwItemID = 0;            // 物品ID
    std::uint32_t dwExchangeID = 0;        // 交易所ID
    std::int16_t sSellCount = 0;           // 售出数量
    std::uint8_t _pad0[6] = {};
    std::int64_t nPrice_One = 0;           // 单价
    std::int64_t tRegDate = 0;             // 注册日期
    ST_POST_DATA stPost{};                 // 邮件数据
    std::uint16_t wSellerRecvPostCount = 0;// 卖家已收邮件数
    std::uint8_t _pad1[6] = {};
    wchar_t strBuyerName[21] = {};         // 买家名字
};

// 对齐 IDA 0x1400D94A0: 花粉互助用户信息
struct PS_MYROOM_POLLEN_HELP_USER {
    std::uint8_t byClass = 0;              // 职业类型
    std::uint8_t byAwaken = 0;             // 觉醒等级
    std::uint8_t _pad0[2] = {};
    std::uint32_t dwProfilePhotoID = 0;    // 头像ID
    std::uint32_t dwUCID = 0;              // 角色ID
    wchar_t szName[21] = {};               // 角色名
};

/**
 * @brief 清除用户状态请求（发给 AccountDB）。
 *
 * 对齐 IDA 0x1400B3160 XRelayServer::ClearUserState。
 * 当游戏服务器断开时，收集该服务器上所有用户的 UAID 列表发送给 AccountDB。
 */
struct PS_REQ_CLEAR_USER_STATE {
    std::vector<std::uint32_t> vecUserID;  // UAID 列表
};

// 对齐 IDA 0x1400B3160: 序列化 PS_REQ_CLEAR_USER_STATE
inline XPacket& operator<<(XPacket& packet, const PS_REQ_CLEAR_USER_STATE& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecUserID.size(), 0xFFFF));
    packet.XParse << count;
    for (std::uint16_t i = 0; i < count; ++i) {
        packet.XParse << value.vecUserID[i];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_CLEAR_USER_STATE& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecUserID.clear();
    value.vecUserID.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        std::uint32_t id = 0;
        packet.XParse >> id;
        value.vecUserID.push_back(id);
    }
}

// ============================================================
// 反序列化器: PS_LEAGUE_INVENTORY_FOR_LOG / LIST
// ============================================================

inline void operator>>(XPacket& packet, PS_LEAGUE_INVENTORY_FOR_LOG& value) {
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shItemCount;
    packet.XParse >> value.shPos;
    packet.XParse >> value.biSerial;
    packet.XParse >> value.nAttack;
    packet.XParse >> value.nDefense;
    packet.XParse >> value.nItemTitleID;
}

inline XPacket& operator<<(XPacket& packet, const PS_LEAGUE_INVENTORY_FOR_LOG& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shItemCount;
    packet.XParse << value.shPos;
    packet.XParse << value.biSerial;
    packet.XParse << value.nAttack;
    packet.XParse << value.nDefense;
    packet.XParse << value.nItemTitleID;
    return packet;
}

// 对齐 IDA 0x1400EB480: 读取计数 + 循环反序列化
inline void operator>>(XPacket& packet, PS_LEAGUE_INVENTORY_FOR_LOG_LIST& value) {
    std::int8_t cCount = 0;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&cCount), 1);
    for (std::int8_t i = 0; i < cCount; ++i) {
        PS_LEAGUE_INVENTORY_FOR_LOG info{};
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_LEAGUE_INVENTORY_FOR_LOG_LIST& value) {
    auto cCount = static_cast<std::int8_t>(value.vecInfo.size());
    packet.XParse << cCount;
    for (const auto& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

// ============================================================
// 反序列化器: ST_POST_CHAR / ST_POST_DATA
// ============================================================

// 对齐 IDA 0x1400E2310
inline void operator>>(XPacket& packet, ST_POST_CHAR& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse >> value.dwProfilePhotoID;
    short sLen = 0;
    packet.XParse.GetWString(value.strName, 21, sLen);
}

inline XPacket& operator<<(XPacket& packet, const ST_POST_CHAR& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    return packet;
}

// 对齐 IDA 0x1400E25D0
inline void operator>>(XPacket& packet, ST_POST_DATA& value) {
    packet.XParse >> value.biSerial;
    packet >> value.stCharInfo;
    short sLen = 0;
    packet.XParse.GetWString(value.strTitle, 41, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.strMsg, 401, sLen);
    packet.XParse >> value.biMoney;
    for (int i = 0; i < 5; ++i) {
        packet >> value.stItemList[i];
    }
    packet.XParse >> value.nRegTime;
    packet.XParse >> value.byFlag;
    packet.XParse >> value.byPostType;
    packet.XParse >> value.byPostSubType;
    packet.XParse >> value.nRemainTime;
    packet.XParse >> value.biEventID;
    packet >> value.vecSocketList;
    packet >> value.vecBroachList;
    packet >> value.vecPackageList;
}

inline XPacket& operator<<(XPacket& packet, const ST_POST_DATA& value) {
    packet.XParse << value.biSerial;
    packet << value.stCharInfo;
    packet.XParse << GreenDamTan_BoundedWideString(value.strTitle);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    packet.XParse << value.biMoney;
    for (int i = 0; i < 5; ++i) {
        packet << value.stItemList[i];
    }
    packet.XParse << value.nRegTime;
    packet.XParse << value.byFlag;
    packet.XParse << value.byPostType;
    packet.XParse << value.byPostSubType;
    packet.XParse << value.nRemainTime;
    packet.XParse << value.biEventID;
    packet << value.vecSocketList;
    packet << value.vecBroachList;
    packet << value.vecPackageList;
    return packet;
}

// ============================================================
// 序列化器: ST_ACCOUNT_POST_DATA / PS_ACCOUNT_POST_LIST
// ============================================================

// 对齐 IDA 0x1400E2780: 账号邮件数据序列化
inline XPacket& operator<<(XPacket& packet, const ST_ACCOUNT_POST_DATA& value) {
    packet.XParse << value.biSerial;
    packet.XParse << value.dwUAID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szTitle);
    packet.XParse << GreenDamTan_BoundedWideString(value.szMsg);
    packet.XParse << value.biMoney;
    for (int i = 0; i < 5; ++i) {
        packet << value.stItemList[i];
    }
    packet.XParse << value.biRegTime;
    packet.XParse << value.biRemainTime;
    packet.XParse << value.byFlag;
    packet.XParse << GreenDamTan_BoundedWideString(value.szSendName);
    packet.XParse << value.byMainType;
    packet.XParse << value.bySubType;
    packet.XParse << value.biDelDate;
    packet << value.vecSocketList;
    packet << value.vecBroachList;
    packet << value.vecPackageList;
    return packet;
}

// 对齐 IDA 0x1400E8E10: 账号邮件列表序列化
inline XPacket& operator<<(XPacket& packet, const PS_ACCOUNT_POST_LIST& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecAccountPostList.size(), 0xFFFF));
    packet.XParse << count;
    for (const auto& item : value.vecAccountPostList) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA: 账号邮件列表反序列化
inline void operator>>(XPacket& packet, PS_ACCOUNT_POST_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecAccountPostList.clear();
    value.vecAccountPostList.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        ST_ACCOUNT_POST_DATA item{};
        packet.XParse >> item.biSerial;
        packet.XParse >> item.dwUAID;
        short sLen = 0;
        packet.XParse.GetWString(item.szTitle, 41, sLen);
        sLen = 0;
        packet.XParse.GetWString(item.szMsg, 401, sLen);
        packet.XParse >> item.biMoney;
        for (int j = 0; j < 5; ++j) {
            packet >> item.stItemList[j];
        }
        packet.XParse >> item.biRegTime;
        packet.XParse >> item.biRemainTime;
        packet.XParse >> item.byFlag;
        sLen = 0;
        packet.XParse.GetWString(item.szSendName, 21, sLen);
        packet.XParse >> item.byMainType;
        packet.XParse >> item.bySubType;
        packet.XParse >> item.biDelDate;
        packet >> item.vecSocketList;
        packet >> item.vecBroachList;
        packet >> item.vecPackageList;
        value.vecAccountPostList.push_back(item);
    }
}

// 对齐 IDA: 单个账号邮件数据反序列化
inline void operator>>(XPacket& packet, ST_ACCOUNT_POST_DATA& value) {
    packet.XParse >> value.biSerial;
    packet.XParse >> value.dwUAID;
    short sLen = 0;
    packet.XParse.GetWString(value.szTitle, 41, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.szMsg, 401, sLen);
    packet.XParse >> value.biMoney;
    for (int j = 0; j < 5; ++j) {
        packet >> value.stItemList[j];
    }
    packet.XParse >> value.biRegTime;
    packet.XParse >> value.biRemainTime;
    packet.XParse >> value.byFlag;
    sLen = 0;
    packet.XParse.GetWString(value.szSendName, 21, sLen);
    packet.XParse >> value.byMainType;
    packet.XParse >> value.bySubType;
    packet.XParse >> value.biDelDate;
    packet >> value.vecSocketList;
    packet >> value.vecBroachList;
    packet >> value.vecPackageList;
}

// ============================================================
// 序列化器: Post Delete 相关结构体
// ============================================================

inline XPacket& operator<<(XPacket& packet, const PS_POST_DELETE_INFO& value) {
    packet.XParse << value.biSerial;
    packet.XParse << value.biDeleteDate;
    return packet;
}

inline void operator>>(XPacket& packet, PS_POST_DELETE_INFO& value) {
    packet.XParse >> value.biSerial;
    packet.XParse >> value.biDeleteDate;
}

inline XPacket& operator<<(XPacket& packet, const PS_POST_DELETE_LIST& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecInfo.size(), 0xFFFF));
    packet.XParse << count;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_POST_DELETE_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        PS_POST_DELETE_INFO item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_POST_DELETE_ALL_SERVER& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.wPostCount;
    packet.XParse << value.nErrorCode;
    packet.XParse << value.byPostType;
    packet << value.psDeleteList;
    packet << value.psFailedList;
    return packet;
}

inline void operator>>(XPacket& packet, PS_POST_DELETE_ALL_SERVER& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.wPostCount;
    packet.XParse >> value.nErrorCode;
    packet.XParse >> value.byPostType;
    packet >> value.psDeleteList;
    packet >> value.psFailedList;
}

// ============================================================
// 序列化器: Post Level Up Event 相关结构体
// ============================================================

inline XPacket& operator<<(XPacket& packet, const ST_POST_LEVEL_UP_EVENT_INFO& value) {
    packet.XParse << value.nGroup;
    packet.XParse << value.nLv;
    packet.XParse << value.nUCID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_POST_LEVEL_UP_EVENT_INFO& value) {
    packet.XParse >> value.nGroup;
    packet.XParse >> value.nLv;
    packet.XParse >> value.nUCID;
}

inline XPacket& operator<<(XPacket& packet, const PS_POST_LEVEL_UP_EVENT_INFO_VEC& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecInfo.size(), 0xFFFF));
    packet.XParse << count;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_POST_LEVEL_UP_EVENT_INFO_VEC& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        ST_POST_LEVEL_UP_EVENT_INFO item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_POST_LEVEL_UP_EVENT_UPDATE& value) {
    packet.XParse << value.nUAID;
    packet.XParse << value.nGroup;
    packet.XParse << value.nLv;
    packet.XParse << value.nUCID;
    return packet;
}

inline void operator>>(XPacket& packet, PS_POST_LEVEL_UP_EVENT_UPDATE& value) {
    packet.XParse >> value.nUAID;
    packet.XParse >> value.nGroup;
    packet.XParse >> value.nLv;
    packet.XParse >> value.nUCID;
}

// ============================================================
// 反序列化器: PS_EXCHANGE_PRICE_HISTORY_UPDATE
// ============================================================

// 对齐 IDA 0x1400E9EF0
inline void operator>>(XPacket& packet, PS_EXCHANGE_PRICE_HISTORY_UPDATE& value) {
    packet.XParse >> value.dwSellerUCID;
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.dwExchangeID;
    packet.XParse >> value.sSellCount;
    packet.XParse >> value.nPrice_One;
    packet.XParse >> value.tRegDate;
    packet >> value.stPost;
    packet.XParse >> value.wSellerRecvPostCount;
    short sLen = 0;
    packet.XParse.GetWString(value.strBuyerName, 21, sLen);
}

// 对齐 IDA 0x1400E9DD0
inline XPacket& operator<<(XPacket& packet, const PS_EXCHANGE_PRICE_HISTORY_UPDATE& value) {
    packet.XParse << value.dwSellerUCID;
    packet.XParse << value.dwItemID;
    packet.XParse << value.dwExchangeID;
    packet.XParse << value.sSellCount;
    packet.XParse << value.nPrice_One;
    packet.XParse << value.tRegDate;
    packet << value.stPost;
    packet.XParse << value.wSellerRecvPostCount;
    packet.XParse << GreenDamTan_BoundedWideString(value.strBuyerName);
    return packet;
}

// ============================================================
// 反序列化器: PS_MYROOM_POLLEN_HELP_USER
// ============================================================

// 对齐 IDA 0x1400DED80
inline void operator>>(XPacket& packet, PS_MYROOM_POLLEN_HELP_USER& value) {
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.dwUCID;
    short sLen = 0;
    packet.XParse.GetWString(value.szName, 21, sLen);
}

inline XPacket& operator<<(XPacket& packet, const PS_MYROOM_POLLEN_HELP_USER& value) {
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.dwUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szName);
    return packet;
}

// ============================================================
// 服务器迷宫进入检查相关结构体 (对齐 IDA)
// ============================================================

// 对齐 IDA: ST_SERVER_CHECK_ENTER_MAZE
struct ST_SERVER_CHECK_ENTER_MAZE {
    int nMapID = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t byState = 0;
};

// ST_SERVER_CHECK_ENTER_MAZE 序列化
inline XPacket& operator<<(XPacket& packet, const ST_SERVER_CHECK_ENTER_MAZE& value) {
    packet.XParse << value.nMapID;
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << value.byState;
    return packet;
}

inline void operator>>(XPacket& packet, ST_SERVER_CHECK_ENTER_MAZE& value) {
    packet.XParse >> value.nMapID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byState;
}

// 对齐 IDA: ST_MAZE_WAIT_ENTER_USER_INFO (48 bytes, 10 members)
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
static_assert(sizeof(ST_MAZE_WAIT_ENTER_USER_INFO) == 48, "ST_MAZE_WAIT_ENTER_USER_INFO size mismatch with IDA");

// ST_MAZE_WAIT_ENTER_USER_INFO 序列化 (wire format uses stMemberInfo.dwMember and stMemberInfo.nState)
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

// ============================================================
// 轮盘事件更新相关结构体 (对齐 IDA)
// ============================================================

// 对齐 IDA: PS_ROULETTE_EVENT_UPDATE_SERVER - 轮盘奖励信息
struct PS_ROULETTE_EVENT_UPDATE_SERVER {
    int nEventID = 0;
    int nRewardID = 0;
    int nRemainCount = 0;
};

inline XPacket& operator<<(XPacket& packet, const PS_ROULETTE_EVENT_UPDATE_SERVER& value) {
    packet.XParse << value.nEventID;
    packet.XParse << value.nRewardID;
    packet.XParse << value.nRemainCount;
    return packet;
}

inline void operator>>(XPacket& packet, PS_ROULETTE_EVENT_UPDATE_SERVER& value) {
    packet.XParse >> value.nEventID;
    packet.XParse >> value.nRewardID;
    packet.XParse >> value.nRemainCount;
}

// 对齐 IDA: PS_DB_ROULETTE_REWARD_INFO - 轮盘奖励信息列表
struct PS_DB_ROULETTE_REWARD_INFO {
    std::vector<PS_ROULETTE_EVENT_UPDATE_SERVER> vecRewardInfo;
};

inline XPacket& operator<<(XPacket& packet, const PS_DB_ROULETTE_REWARD_INFO& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecRewardInfo.size());
    for (const auto& info : value.vecRewardInfo) {
        packet << info;
    }
    return packet;
}

// 对齐 IDA: PS_DB_INIT_ROULETTE_INFO - 轮盘初始化请求
struct PS_DB_INIT_ROULETTE_INFO {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::int32_t nEventID = 0;
    std::int64_t biInitDate = 0;
};

inline void operator>>(XPacket& packet, PS_DB_INIT_ROULETTE_INFO& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nEventID;
    packet.XParse >> value.biInitDate;
}

// 对齐 IDA: PS_PLAY_TIME_BY_ACCOUNT - 账户游戏时间信息
struct PS_PLAY_TIME_BY_ACCOUNT {
    std::uint32_t dwUAID = 0;
    std::int32_t nSec = 0;
    std::int32_t nInitTime = 0;
    std::uint8_t byState = 0;
};

inline void operator>>(XPacket& packet, PS_PLAY_TIME_BY_ACCOUNT& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.nSec;
    packet.XParse >> value.nInitTime;
    packet.XParse >> value.byState;
}

inline XPacket& operator<<(XPacket& packet, const PS_PLAY_TIME_BY_ACCOUNT& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.nSec;
    packet.XParse << value.nInitTime;
    packet.XParse << value.byState;
    return packet;
}

// ============================================================
// 签到系统相关结构体 (对齐 IDA)
// ============================================================

// 对齐 IDA: PS_ATTENDANCE_INFO - 签到信息
struct PS_ATTENDANCE_INFO {
    std::uint32_t dwType = 0;
    std::uint32_t dwAttendanceID = 0;
    std::uint8_t byApplyAttendance = 0;
    std::uint8_t byAttendanceCount = 0;  // 签到计数
    std::int32_t nAttendance[14] = {};  // 签到状态数组
};

inline void operator>>(XPacket& packet, PS_ATTENDANCE_INFO& value) {
    packet.XParse >> value.dwType;
    packet.XParse >> value.dwAttendanceID;
    packet.XParse >> value.byApplyAttendance;
    for (int i = 0; i < 14; ++i) {
        packet.XParse >> value.nAttendance[i];
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ATTENDANCE_INFO& value) {
    packet.XParse << value.dwType;
    packet.XParse << value.dwAttendanceID;
    packet.XParse << value.byApplyAttendance;
    for (int i = 0; i < 14; ++i) {
        packet.XParse << value.nAttendance[i];
    }
    return packet;
}

// 对齐 IDA: PS_ATTENDANCE_CONTINUE - 连续签到信息
struct PS_ATTENDANCE_CONTINUE {
    std::uint32_t dwType = 0;
    std::uint8_t byAttendanceCount = 0;  // 连续签到天数
    std::int64_t nLastAttendanceDate = 0;  // 上次签到日期
    std::uint8_t byApplyAttendance = 0;
    std::int32_t nAttendance[7] = {};  // 连续签到状态数组
};

inline void operator>>(XPacket& packet, PS_ATTENDANCE_CONTINUE& value) {
    packet.XParse >> value.dwType;
    packet.XParse >> value.byAttendanceCount;
    packet.XParse >> value.nLastAttendanceDate;
    packet.XParse >> value.byApplyAttendance;
    for (int i = 0; i < 7; ++i) {
        packet.XParse >> value.nAttendance[i];
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ATTENDANCE_CONTINUE& value) {
    packet.XParse << value.dwType;
    packet.XParse << value.byAttendanceCount;
    packet.XParse << value.nLastAttendanceDate;
    packet.XParse << value.byApplyAttendance;
    for (int i = 0; i < 7; ++i) {
        packet.XParse << value.nAttendance[i];
    }
    return packet;
}

// 对齐 IDA: PS_ATTENDANCE_PLAY_TIME - 签到游戏时间
struct PS_ATTENDANCE_PLAY_TIME {
    std::uint32_t dwType = 0;
    std::int64_t nPlaySec = 0;  // 游戏时间秒数
    std::uint8_t byCurPos = 0;  // 当前位置
    std::int64_t nUpdateDate = 0;  // 更新日期
    std::uint8_t byApplyAttendance = 0;
};

inline void operator>>(XPacket& packet, PS_ATTENDANCE_PLAY_TIME& value) {
    packet.XParse >> value.dwType;
    packet.XParse >> value.nPlaySec;
    packet.XParse >> value.byCurPos;
    packet.XParse >> value.nUpdateDate;
    packet.XParse >> value.byApplyAttendance;
}

inline XPacket& operator<<(XPacket& packet, const PS_ATTENDANCE_PLAY_TIME& value) {
    packet.XParse << value.dwType;
    packet.XParse << value.nPlaySec;
    packet.XParse << value.byCurPos;
    packet.XParse << value.nUpdateDate;
    packet.XParse << value.byApplyAttendance;
    return packet;
}

// ============================================================
// 网吧任务相关结构体 (对齐 IDA)
// ============================================================

// 对齐 IDA: ST_NETCAFE_MISSION_INFO - 网吧任务信息
struct ST_NETCAFE_MISSION_INFO {
    std::uint32_t dwID = 0;
    std::int32_t nStartTime = 0;
    std::int32_t nEndTime = 0;
    std::uint32_t dwValue = 0;
    std::int32_t nUpdateTime = 0;
};

inline void operator>>(XPacket& packet, ST_NETCAFE_MISSION_INFO& value) {
    packet.XParse >> value.dwID;
    packet.XParse >> value.nStartTime;
    packet.XParse >> value.nEndTime;
    packet.XParse >> value.dwValue;
    packet.XParse >> value.nUpdateTime;
}

inline XPacket& operator<<(XPacket& packet, const ST_NETCAFE_MISSION_INFO& value) {
    packet.XParse << value.dwID;
    packet.XParse << value.nStartTime;
    packet.XParse << value.nEndTime;
    packet.XParse << value.dwValue;
    packet.XParse << value.nUpdateTime;
    return packet;
}

// 对齐 IDA: PS_NETCAFE_MISSION_LIST - 网吧任务列表
struct PS_NETCAFE_MISSION_LIST {
    std::uint32_t dwUAID = 0;
    std::vector<ST_NETCAFE_MISSION_INFO> vecList;
};

inline void operator>>(XPacket& packet, PS_NETCAFE_MISSION_LIST& value) {
    packet.XParse >> value.dwUAID;
    std::uint16_t wCount = 0;
    packet.XParse >> wCount;
    for (std::uint16_t i = 0; i < wCount; ++i) {
        ST_NETCAFE_MISSION_INFO info;
        packet >> info;
        value.vecList.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_NETCAFE_MISSION_LIST& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << static_cast<std::uint16_t>(value.vecList.size());
    for (const ST_NETCAFE_MISSION_INFO& info : value.vecList) {
        packet << info;
    }
    return packet;
}

// 对齐 IDA: PS_NETCAFE_MISSION_UPDATE - 网吧任务更新
struct PS_NETCAFE_MISSION_UPDATE {
    std::uint32_t dwUAID = 0;
    ST_NETCAFE_MISSION_INFO stMission;
};

inline void operator>>(XPacket& packet, PS_NETCAFE_MISSION_UPDATE& value) {
    packet.XParse >> value.dwUAID;
    packet >> value.stMission;
}

inline XPacket& operator<<(XPacket& packet, const PS_NETCAFE_MISSION_UPDATE& value) {
    packet.XParse << value.dwUAID;
    packet << value.stMission;
    return packet;
}

// ============================================================
// 迷宫更新信息同步相关结构体 (对齐 IDA)
// ============================================================

// 对齐 IDA: ST_MAZE_MEMBER_INFO_SYNC
struct ST_MAZE_MEMBER_INFO_SYNC {
    std::uint32_t dwActorID = 0;   // 对齐 IDA: ActorID
    std::uint32_t dwUCID = 0;      // 对齐 IDA: UCID
    std::uint8_t byState = 0;
};

inline XPacket& operator<<(XPacket& packet, const ST_MAZE_MEMBER_INFO_SYNC& value) {
    packet.XParse << static_cast<int>(value.dwActorID);
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << value.byState;
    return packet;
}

inline void operator>>(XPacket& packet, ST_MAZE_MEMBER_INFO_SYNC& value) {
    int nActorID = 0;
    int nUCID = 0;
    packet.XParse >> nActorID;
    packet.XParse >> nUCID;
    packet.XParse >> value.byState;
    value.dwActorID = static_cast<std::uint32_t>(nActorID);
    value.dwUCID = static_cast<std::uint32_t>(nUCID);
}

// 对齐 IDA: PS_MAZE_INFO_SYNC
// 注意: vecMemberInfo 类型根据 IDA CMazeInfo::SyncMazeInfo 使用 ST_MAZE_WAIT_ENTER_USER_INFO
struct PS_MAZE_INFO_SYNC {
    UXMapID uxMapID{};
    int nUserCount = 0;     // 对齐 IDA: 用户数
    int nState = 0;         // 对齐 IDA: 迷宫状态
    std::vector<ST_MAZE_WAIT_ENTER_USER_INFO> vecMemberInfo;  // IDA: 使用 ST_MAZE_WAIT_ENTER_USER_INFO
};

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

// 对齐 IDA: PS_MAZE_UPDATE_INFO_SYNC (包含 bLast 标志和其他字段)
struct PS_MAZE_UPDATE_INFO_SYNC {
    bool bLast = false;                    // 对齐 IDA: 是否为最后一个同步包
    ST_PARTY_INFO stPartyInfo{};           // 对齐 IDA: 队伍信息
    std::uint32_t dwServerID = 0;          // 对齐 IDA: 服务器ID
    std::int16_t sPort = 0;                // 对齐 IDA: 端口
    int nJumpID = 0;                       // 对齐 IDA: 跳转ID
    char szIP[513] = {};                   // 对齐 IDA: IP地址
    PS_MAZE_INFO_SYNC psMazeInfo{};
};

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

// 对齐 IDA: 改名请求结构
struct PS_CHANGE_NAME {
    std::uint32_t dwActorID = 0;
    wchar_t szChangeName[21] = {};
};

inline void operator>>(XPacket& packet, PS_CHANGE_NAME& value) {
    packet.XParse >> value.dwActorID;
    short outLen = 0;
    packet.XParse.GetWString(value.szChangeName, 21, outLen);
}

// 对齐 IDA: PS_CHANGE_NAME 序列化
inline XPacket& operator<<(XPacket& packet, const PS_CHANGE_NAME& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szChangeName);
    return packet;
}

// 对齐 IDA: 认证类型更新请求结构
struct PS_USER_UPDATE_AUTH_TYPE {
    std::uint32_t dwUAID = 0;
    std::uint8_t byAuthType = 0;
};

inline void operator>>(XPacket& packet, PS_USER_UPDATE_AUTH_TYPE& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.byAuthType;
}

// ============================================================================
// World Mode Serialization Operators (receive direction)
// Note: World mode structs are defined in PSCommon.h
// ============================================================================

// 对齐 IDA: ST_ENTER_WORLD_MODE_INFO 反序列化
inline void operator>>(XPacket& packet, ST_ENTER_WORLD_MODE_INFO& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_WORLD_MODE item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

// 对齐 IDA: PS_WORLD_MODE_COMPLETE 反序列化
inline void operator>>(XPacket& packet, PS_WORLD_MODE_COMPLETE& value) {
    packet.XParse >> value.nModeID;
    packet.XParse >> value.uxMapID.nMapID;
    short outLen = 0;
    packet.XParse.GetWString(value.strKiller, 21, outLen);
    int nFinish = 0;
    packet.XParse >> nFinish;
    value.bFinish = (nFinish != 0);
    packet.XParse >> value.nModeDateID;
}

// 对齐 IDA: ST_WORLD_MODE_INFO_VEC 反序列化
inline void operator>>(XPacket& packet, ST_WORLD_MODE_INFO_VEC& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_WORLD_MODE_INFO item{};
        packet.XParse >> item.nStartTime;
        packet.XParse >> item.nFinishTime;
        packet.XParse >> item.nModeID;
        packet.XParse >> item.nState;
        int nSuccess = 0;
        packet.XParse >> nSuccess;
        item.bSuccess = (nSuccess != 0);
        packet.XParse >> item.nModeDateID;
        packet.XParse >> item.nMonsterClearCount;
        packet.XParse >> item.biModeStartTime;
        packet.XParse >> item.biModeEndTime;
        value.vecInfo.push_back(item);
    }
}

// 对齐 IDA: PS_WORLD_MODE_UPDATE 反序列化
inline void operator>>(XPacket& packet, PS_WORLD_MODE_UPDATE& value) {
    packet >> value.stInfo;
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.nMonsterClearCount;
    int nUpdate = 0;
    packet.XParse >> nUpdate;
    value.bUpdate = (nUpdate != 0);
}

// 对齐 IDA: PS_WORLD_MODE_UPDATE 序列化 (用于 0xFB/0x02)
inline XPacket& operator<<(XPacket& packet, const PS_WORLD_MODE_UPDATE& value) {
    packet << value.stInfo;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.nMonsterClearCount;
    packet.XParse << static_cast<int>(value.bUpdate ? 1 : 0);
    return packet;
}

// ============================================================================
// ControlServer GM Tool Structures
// ============================================================================

// 对齐 IDA: GM用户踢出信息结构
struct ST_GM_USER_KICK_INFO {
    std::uint32_t dwNo = 0;
    std::uint32_t dwUAID = 0;
    std::int64_t nDate = 0;
    std::uint8_t byteUse = 0;
    std::uint8_t _pad0[1] = {};
    wchar_t wszMsg[513] = {};
};

static_assert(sizeof(ST_GM_USER_KICK_INFO) == 1048, "ST_GM_USER_KICK_INFO size must match IDA");

// 对齐 IDA: GM公告信息结构
struct ST_GM_NOTICE_INFO {
    std::uint32_t dwNo = 0;
    std::int16_t shType = 0;
    std::int16_t shViewType = 0;
    wchar_t strMsg[256] = {};
    wchar_t strColor[7] = {};
    std::int64_t nDate = 0;
    std::int64_t nEndTime = 0;
    std::uint8_t byTerm = 0;
    std::uint8_t byteUse = 0;
    std::uint8_t byteDel = 0;
};

static_assert(sizeof(ST_GM_NOTICE_INFO) == 560, "ST_GM_NOTICE_INFO size must match IDA");

// 对齐 IDA: GM时间事件信息结构
struct ST_GM_TIME_EVENT_INFO {
    std::uint32_t dwNo = 0;
    std::uint32_t dwServerIDX = 0;
    std::uint32_t dwBuff_ID = 0;
    std::int32_t dwBuff_Val = 0;
    std::int64_t nStartDate = 0;
    std::int64_t nEndDate = 0;
    std::uint8_t byteFlag = 0;
    std::uint8_t byteUse = 0;
    std::uint8_t byteClass = 0;
};

static_assert(sizeof(ST_GM_TIME_EVENT_INFO) == 40, "ST_GM_TIME_EVENT_INFO size must match IDA");

// 对齐 IDA: GM数值事件信息结构
struct ST_GM_VALUE_EVENT_INFO {
    std::uint32_t dwNo = 0;
    std::uint32_t dwServerIDX = 0;
    std::uint8_t byEventType = 0;
    std::uint8_t _pad0[3] = {};
    std::uint32_t dwEvent_Val = 0;
    std::int64_t nStartDate = 0;
    std::int64_t nEndDate = 0;
    std::uint8_t byteUse = 0;
};

static_assert(sizeof(ST_GM_VALUE_EVENT_INFO) == 40, "ST_GM_VALUE_EVENT_INFO size must match IDA");

// 对齐 IDA: GM数值事件列表
struct PS_GM_VALUE_EVENT_LIST {
    std::vector<ST_GM_VALUE_EVENT_INFO> vecEvent;
};

// 对齐 IDA: Banner信息结构
struct ST_BANNER_INFO {
    char szUrl[500] = {};
    std::int32_t nTime = 0;
    std::int32_t nNo = 0;
    std::int32_t nMain = 0;
    std::int32_t nSub = 0;
};

static_assert(sizeof(ST_BANNER_INFO) == 516, "ST_BANNER_INFO size must match IDA");

// 对齐 IDA: Banner列表
struct ST_BANNER_LIST {
    std::vector<ST_BANNER_INFO> vecInfo;
};

// ============================================================================
// ControlServer GM Tool Serialization Operators
// ============================================================================

// 对齐 IDA: ST_GM_USER_KICK_INFO 反序列化
inline void operator>>(XPacket& packet, ST_GM_USER_KICK_INFO& value) {
    packet.XParse >> value.dwNo;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.nDate;
    packet.XParse >> value.byteUse;
    short outLen = 0;
    packet.XParse.GetWString(value.wszMsg, 513, outLen);
}

// 对齐 IDA: ST_GM_NOTICE_INFO 反序列化
inline void operator>>(XPacket& packet, ST_GM_NOTICE_INFO& value) {
    packet.XParse >> value.dwNo;
    packet.XParse >> value.shType;
    packet.XParse >> value.shViewType;
    short outLen = 0;
    packet.XParse.GetWString(value.strMsg, 256, outLen);
    packet.XParse.GetWString(value.strColor, 7, outLen);
    packet.XParse >> value.nDate;
    packet.XParse >> value.nEndTime;
    packet.XParse >> value.byTerm;
    packet.XParse >> value.byteUse;
    packet.XParse >> value.byteDel;
}

// 对齐 IDA: ST_GM_TIME_EVENT_INFO 反序列化
inline void operator>>(XPacket& packet, ST_GM_TIME_EVENT_INFO& value) {
    packet.XParse >> value.dwNo;
    packet.XParse >> value.dwServerIDX;
    packet.XParse >> value.dwBuff_ID;
    packet.XParse >> value.dwBuff_Val;
    packet.XParse >> value.nStartDate;
    packet.XParse >> value.nEndDate;
    packet.XParse >> value.byteFlag;
    packet.XParse >> value.byteUse;
    packet.XParse >> value.byteClass;
}

// 对齐 IDA: ST_GM_VALUE_EVENT_INFO 反序列化
inline void operator>>(XPacket& packet, ST_GM_VALUE_EVENT_INFO& value) {
    packet.XParse >> value.dwNo;
    packet.XParse >> value.dwServerIDX;
    packet.XParse >> value.byEventType;
    packet.XParse >> value.dwEvent_Val;
    packet.XParse >> value.nStartDate;
    packet.XParse >> value.nEndDate;
    packet.XParse >> value.byteUse;
}

// 对齐 IDA: PS_GM_VALUE_EVENT_LIST 反序列化
inline void operator>>(XPacket& packet, PS_GM_VALUE_EVENT_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecEvent.clear();
    value.vecEvent.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_GM_VALUE_EVENT_INFO item{};
        packet >> item;
        value.vecEvent.push_back(item);
    }
}

// 对齐 IDA: ST_BANNER_INFO 反序列化
inline void operator>>(XPacket& packet, ST_BANNER_INFO& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szUrl, 500, &outLen);
    packet.XParse >> value.nTime;
    packet.XParse >> value.nNo;
    packet.XParse >> value.nMain;
    packet.XParse >> value.nSub;
}

// 对齐 IDA: ST_BANNER_LIST 反序列化
inline void operator>>(XPacket& packet, ST_BANNER_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_BANNER_INFO item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

// ============================================================================
// ControlServer GM Tool Serialization Operators (send direction)
// ============================================================================

// 对齐 IDA: ST_GM_TIME_EVENT_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_GM_TIME_EVENT_INFO& value) {
    packet.XParse << value.dwNo;
    packet.XParse << value.dwServerIDX;
    packet.XParse << value.dwBuff_ID;
    packet.XParse << value.dwBuff_Val;
    packet.XParse << value.nStartDate;
    packet.XParse << value.nEndDate;
    packet.XParse << value.byteFlag;
    packet.XParse << value.byteUse;
    packet.XParse << value.byteClass;
    return packet;
}

// 对齐 IDA: ST_GM_VALUE_EVENT_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_GM_VALUE_EVENT_INFO& value) {
    packet.XParse << value.dwNo;
    packet.XParse << value.dwServerIDX;
    packet.XParse << value.byEventType;
    packet.XParse << value.dwEvent_Val;
    packet.XParse << value.nStartDate;
    packet.XParse << value.nEndDate;
    packet.XParse << value.byteUse;
    return packet;
}

// 对齐 IDA: PS_GM_VALUE_EVENT_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_GM_VALUE_EVENT_LIST& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecEvent.size());
    for (const auto& item : value.vecEvent) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA: ST_BANNER_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_BANNER_INFO& value) {
    packet.XParse << std::string(value.szUrl);
    packet.XParse << value.nTime;
    packet.XParse << value.nNo;
    packet.XParse << value.nMain;
    packet.XParse << value.nSub;
    return packet;
}

// 对齐 IDA: ST_BANNER_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const ST_BANNER_LIST& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// ============================================================================
// World Mode Serialization Operators (send direction)
// Note: operator<< for ST_WORLD_MODE is in PSCommon.h
// ============================================================================

// 对齐 IDA: ST_WORLD_MODE_INFO 反序列化
inline void operator>>(XPacket& packet, ST_WORLD_MODE_INFO& value) {
    int bSuccess = 0;
    packet.XParse >> value.nStartTime;
    packet.XParse >> value.nFinishTime;
    packet.XParse >> value.nModeID;
    packet.XParse >> value.nState;
    packet.XParse >> bSuccess;
    value.bSuccess = (bSuccess != 0);
    packet.XParse >> value.nModeDateID;
    packet.XParse >> value.nMonsterClearCount;
    packet.XParse >> value.biModeStartTime;
    packet.XParse >> value.biModeEndTime;
}

// 对齐 IDA: ST_WORLD_MODE_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_WORLD_MODE_INFO& value) {
    packet.XParse << value.nStartTime;
    packet.XParse << value.nFinishTime;
    packet.XParse << value.nModeID;
    packet.XParse << value.nState;
    packet.XParse << static_cast<int>(value.bSuccess ? 1 : 0);
    packet.XParse << value.nModeDateID;
    packet.XParse << value.nMonsterClearCount;
    packet.XParse << value.biModeStartTime;
    packet.XParse << value.biModeEndTime;
    return packet;
}

// 对齐 IDA: ST_WORLD_MODE_INFO_VEC 序列化
inline XPacket& operator<<(XPacket& packet, const ST_WORLD_MODE_INFO_VEC& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA: PS_WORLD_MODE_COMPLETE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_WORLD_MODE_COMPLETE& value) {
    packet.XParse << value.nModeID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << std::wstring(value.strKiller);
    packet.XParse << static_cast<int>(value.bFinish ? 1 : 0);
    packet.XParse << value.nModeDateID;
    return packet;
}

// 对齐 IDA: ST_ENTER_WORLD_MODE_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_ENTER_WORLD_MODE_INFO& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA: PS_WORLD_MODE_FINISH 序列化 (0xFB/0x04)
inline XPacket& operator<<(XPacket& packet, const PS_WORLD_MODE_FINISH& value) {
    packet.XParse << value.nModeID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.nFinishTime;
    packet.XParse << value.nModeDateID;
    packet.XParse << value.nMonsterClearCount;
    packet.XParse << std::wstring(value.strKiller);
    packet.XParse << static_cast<int>(value.bSuccess ? 1 : 0);
    return packet;
}

// ============================================================================
// ControlServer Maze Update Structures
// ============================================================================

// 注意: ST_MAZE_WAIT_ENTER_USER_INFO 已在上方定义 (line 5885)

// 对齐 IDA ControlServer.exe: PS_MAZE_UPDATE_INFO (size 48)
#define PS_MAZE_UPDATE_INFO_DEFINED
struct PS_MAZE_UPDATE_INFO {
    UXMapID uxMapID{};                                      // offset 0, size 8
    int nState = 0;                                         // offset 8, size 4
    int nUserCount = 0;                                     // offset 12, size 4
    std::vector<ST_MAZE_WAIT_ENTER_USER_INFO> vecMemberInfo; // offset 16, size 32
};

// 对齐 IDA: PS_MAZE_UPDATE_INFO 反序列化
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

// 对齐 IDA DBAgent.exe: PS_GF_BILLING_RELOAD_RES (size 32)
struct PS_GF_BILLING_RELOAD_RES {
    std::vector<std::uint32_t> vecUAID;                                // offset 0, size 32
};

static_assert(sizeof(PS_GF_BILLING_RELOAD_RES) == 32, "PS_GF_BILLING_RELOAD_RES size must match IDA");

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

// 对齐 IDA: 防沉迷信息
struct PS_INDULGENCE_INFO {
    std::uint32_t dwUAID = 0;
    bool bLogin = false;
    std::int32_t nIndulgenceConnectTerm = 0;
    std::int32_t nIndulgenceDisconnectTerm = 0;
    std::int32_t nLastDisConnectTime = 0;
    std::int32_t nLeftAlertTime = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_INDULGENCE_INFO& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.bLogin;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_INDULGENCE_INFO& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.bLogin;
    packet.XParse << value.nIndulgenceConnectTerm;
    packet.XParse << value.nIndulgenceDisconnectTerm;
    packet.XParse << value.nLastDisConnectTime;
    packet.XParse << value.nLeftAlertTime;
    return packet;
}

// 对齐 IDA: 优惠券使用请求
struct PS_DB_USE_COUPON_REQ {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    char szCouponCode[21] = {};
};

inline XPacket& operator>>(XPacket& packet, PS_DB_USE_COUPON_REQ& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    short outLen = 0;
    packet.XParse.GetString(value.szCouponCode, 21, &outLen);
    return packet;
}

// 对齐 IDA: 优惠券使用响应
struct PS_DB_USE_COUPON_RES {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    char szCouponCode[21] = {};
    std::int32_t nError = 0;
    std::int32_t nItem = 0;
    std::int32_t nItemCount = 0;
    std::int32_t nEventID = 0;
    std::uint8_t byCouponType = 0;
};

inline XPacket& operator<<(XPacket& packet, const PS_DB_USE_COUPON_RES& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << std::string(value.szCouponCode);
    packet.XParse << value.nError;
    packet.XParse << value.nItem;
    packet.XParse << value.nItemCount;
    packet.XParse << value.nEventID;
    packet.XParse << value.byCouponType;
    return packet;
}

// 对齐 IDA: 自动封禁检查信息
struct PS_DB_CHECK_AUTO_BLOCK_INFO {
    char szAccountID[21] = {};
    std::uint8_t byTypeDB = 0;
    char szEndDate[24] = {};
    wchar_t strComment[101] = {};
    wchar_t strUserComment[101] = {};
};

inline XPacket& operator>>(XPacket& packet, PS_DB_CHECK_AUTO_BLOCK_INFO& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse >> value.byTypeDB;
    packet.XParse.GetString(value.szEndDate, 24, &outLen);
    packet.XParse.GetWString(value.strComment, 101, outLen);
    packet.XParse.GetWString(value.strUserComment, 101, outLen);
    return packet;
}

// 对齐 IDA: WM平台认证信息
struct ST_WM_AUTH_INFO {
    std::int32_t nUAID = 0;
    char szWMAccountID[256] = {};
    char szWMHID[21] = {};
    char szToken[256] = {};
    std::uint8_t byUserType = 0;
    std::uint8_t byChannelType_WM = 0;
    std::uint8_t byChannelType = 0;
    std::uint8_t byAccessType = 0;
};

inline XPacket& operator>>(XPacket& packet, ST_WM_AUTH_INFO& value) {
    packet.XParse >> value.nUAID;
    short outLen = 0;
    packet.XParse.GetString(value.szWMAccountID, 256, &outLen);
    packet.XParse.GetString(value.szWMHID, 21, &outLen);
    packet.XParse.GetString(value.szToken, 256, &outLen);
    packet.XParse >> value.byUserType;
    packet.XParse >> value.byChannelType_WM;
    packet.XParse >> value.byChannelType;
    packet.XParse >> value.byAccessType;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_WM_AUTH_INFO& value) {
    packet.XParse << value.nUAID;
    packet.XParse << std::string(value.szWMAccountID);
    packet.XParse << std::string(value.szWMHID);
    packet.XParse << std::string(value.szToken);
    packet.XParse << value.byUserType;
    packet.XParse << value.byChannelType_WM;
    packet.XParse << value.byChannelType;
    packet.XParse << value.byAccessType;
    return packet;
}

// 对齐 IDA: WM平台登录请求
struct PS_LOGIN_REQ_FOR_WM {
    char szHID[21] = {};
    char szToken[256] = {};
    char szMacAddress[18] = {};
    std::int32_t nPacketVersion = 0;
    std::int32_t nIP = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_LOGIN_REQ_FOR_WM& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szHID, 21, &outLen);
    packet.XParse.GetString(value.szToken, 256, &outLen);
    packet.XParse.GetString(value.szMacAddress, 18, &outLen);
    packet.XParse >> value.nPacketVersion;
    packet.XParse >> value.nIP;
    return packet;
}

// 对齐 IDA: 账户事件列表
struct PS_ACCOUNT_EVENT_LIST {
    char szAccountID[21] = {};
    std::uint32_t dwUCID = 0;
    std::vector<std::uint32_t> vecEventID;
};

inline XPacket& operator>>(XPacket& packet, PS_ACCOUNT_EVENT_LIST& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse >> value.dwUCID;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_ACCOUNT_EVENT_LIST& value) {
    packet.XParse << std::string(value.szAccountID);
    packet.XParse << value.dwUCID;
    packet.XParse << static_cast<std::uint32_t>(value.vecEventID.size());
    for (const auto& eventId : value.vecEventID) {
        packet.XParse << eventId;
    }
    return packet;
}

// 对齐 IDA: 账户登录日期响应
struct PS_DB_ACCOUNT_DATE_RES {
    std::int64_t dwComebackTime = 0;
    std::int64_t tCreateDate = 0;  // CTime 作为 time64_t
};

inline XPacket& operator<<(XPacket& packet, const PS_DB_ACCOUNT_DATE_RES& value) {
    packet.XParse << value.dwComebackTime;
    packet.XParse << value.tCreateDate;
    return packet;
}

// 对齐 IDA: 账户现金里程信息
struct PS_DB_CASH_MILEAGE_INFO {
    std::uint8_t byType = 0;
    std::int32_t nCashMileage[3] = {0, 0, 0};
    std::int32_t nErrorCode = 0;
};

inline XPacket& operator<<(XPacket& packet, const PS_DB_CASH_MILEAGE_INFO& value) {
    packet.XParse << value.byType;
    packet.XParse << value.nCashMileage[0];
    packet.XParse << value.nCashMileage[1];
    packet.XParse << value.nCashMileage[2];
    packet.XParse << value.nErrorCode;
    return packet;
}

// 对齐 IDA 0x1400793E0: SG平台登录请求
struct PS_LOGIN_REQ_FOR_SG {
    char szAccountID[21] = {};
    char szMacAddress[18] = {};
    std::int32_t nIP = 0;
    std::uint8_t byAuthType = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_LOGIN_REQ_FOR_SG& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse.GetString(value.szMacAddress, 18, &outLen);
    packet.XParse >> value.nIP;
    packet.XParse >> value.byAuthType;
    return packet;
}

// 对齐 IDA: HAN计费订单信息
struct PS_HAN_BILLING_ORDER_NO {
    std::uint32_t dwItemID = 0;
    std::int32_t nCount = 0;
    std::int32_t nBanance = 0;
    std::int64_t nOrderNo = 0;
    std::int32_t nResult = 0;
    char szCode[21] = {};
    std::int32_t nShopIndex = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_HAN_BILLING_ORDER_NO& value) {
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.nCount;
    packet.XParse >> value.nBanance;
    packet.XParse >> value.nOrderNo;
    packet.XParse >> value.nResult;
    short outLen = 0;
    packet.XParse.GetString(value.szCode, 21, &outLen);
    packet.XParse >> value.nShopIndex;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_HAN_BILLING_ORDER_NO& value) {
    packet.XParse << value.dwItemID;
    packet.XParse << value.nCount;
    packet.XParse << value.nBanance;
    packet.XParse << value.nOrderNo;
    packet.XParse << value.nResult;
    packet.XParse << std::string(value.szCode);
    packet.XParse << value.nShopIndex;
    return packet;
}

// 对齐 IDA: HAN计费订单列表
struct PS_HAN_BILLING_ORDER_NO_VEC {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    char szBillCode[21] = {};
    std::uint32_t dwRecvUCID = 0;
    std::uint32_t dwRecvUAID = 0;
    bool bGift = false;
    char szRecvAccountID[21] = {};
    char szRecvCharacterName[21] = {};
    std::uint8_t byType = 0;
    bool bResult = false;
    std::vector<PS_HAN_BILLING_ORDER_NO> vecOrderInfo;
};

inline XPacket& operator>>(XPacket& packet, PS_HAN_BILLING_ORDER_NO_VEC& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    short outLen = 0;
    packet.XParse.GetString(value.szBillCode, 21, &outLen);
    packet.XParse >> value.dwRecvUCID;
    packet.XParse >> value.dwRecvUAID;
    packet.XParse >> value.bGift;
    packet.XParse.GetString(value.szRecvAccountID, 21, &outLen);
    packet.XParse.GetString(value.szRecvCharacterName, 21, &outLen);
    packet.XParse >> value.byType;
    packet.XParse >> value.bResult;
    // vecOrderInfo 需要单独解析
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_HAN_BILLING_ORDER_NO_VEC& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << std::string(value.szBillCode);
    packet.XParse << value.dwRecvUCID;
    packet.XParse << value.dwRecvUAID;
    packet.XParse << value.bGift;
    packet.XParse << std::string(value.szRecvAccountID);
    packet.XParse << std::string(value.szRecvCharacterName);
    packet.XParse << value.byType;
    packet.XParse << value.bResult;
    packet.XParse << static_cast<std::uint32_t>(value.vecOrderInfo.size());
    for (const auto& item : value.vecOrderInfo) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA 0x1400BB750: 现金购买计数
struct PS_CASH_BUY_COUNT {
    std::int32_t nCashShopIndex = 0;
    std::int32_t nBuyCount = 0;
    std::int64_t biEndDate = 0;
    std::uint8_t byBuyType = 0;  // 2/4/6/8 = account, others = character
};

inline XPacket& operator>>(XPacket& packet, PS_CASH_BUY_COUNT& value) {
    packet.XParse >> value.nCashShopIndex;
    packet.XParse >> value.nBuyCount;
    packet.XParse >> value.biEndDate;
    packet.XParse >> value.byBuyType;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_CASH_BUY_COUNT& value) {
    packet.XParse << value.nCashShopIndex;
    packet.XParse << value.nBuyCount;
    packet.XParse << value.biEndDate;
    packet.XParse << value.byBuyType;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_CASH_BUY_COUNT& value) {
    packet.XParse << value.nCashShopIndex;
    packet.XParse << value.nBuyCount;
    packet.XParse << value.biEndDate;
    packet.XParse << value.byBuyType;
    return packet;
}

// 对齐 IDA: 现金购买计数列表
struct PS_CASH_BUY_COUNT_LIST {
    std::vector<PS_CASH_BUY_COUNT> vecInfo;
};

inline XPacket& operator>>(XPacket& packet, PS_CASH_BUY_COUNT_LIST& value) {
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.resize(count);
    for (auto& item : value.vecInfo) {
        packet >> item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_CASH_BUY_COUNT_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_CASH_BUY_COUNT_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA 0x1400BA500: 现金套装
struct PS_CASH_SET {
    std::uint8_t bySetNo = 0;
    wchar_t szName[11] = {};  // 22 bytes in GetWString
    std::int32_t nIndex[10] = {};
    std::uint32_t dwItemID[10] = {};
};

inline XPacket& operator>>(XPacket& packet, PS_CASH_SET& value) {
    packet.XParse >> value.bySetNo;
    short outLen = 0;
    packet.XParse.GetWString(value.szName, 11, outLen);
    for (int i = 0; i < 10; ++i) {
        packet.XParse >> value.nIndex[i];
    }
    for (int i = 0; i < 10; ++i) {
        packet.XParse >> value.dwItemID[i];
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_CASH_SET& value) {
    packet.XParse << value.bySetNo;
    packet.XParse << std::wstring(value.szName);
    for (int i = 0; i < 10; ++i) {
        packet.XParse << value.nIndex[i];
    }
    for (int i = 0; i < 10; ++i) {
        packet.XParse << value.dwItemID[i];
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_CASH_SET& value) {
    packet.XParse << value.bySetNo;
    packet.XParse << std::wstring(value.szName);
    for (int i = 0; i < 10; ++i) {
        packet.XParse << value.nIndex[i];
    }
    for (int i = 0; i < 10; ++i) {
        packet.XParse << value.dwItemID[i];
    }
    return packet;
}

// 对齐 IDA: 现金套装列表
struct PS_CASH_SET_LIST {
    std::vector<PS_CASH_SET> vecInfo;
};

inline XPacket& operator>>(XPacket& packet, PS_CASH_SET_LIST& value) {
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.resize(count);
    for (auto& item : value.vecInfo) {
        packet >> item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_CASH_SET_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_CASH_SET_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA: 商店物品
struct ST_SHOP_ITEM {
    std::uint32_t nItemID = 0;
    std::int16_t shCount = 0;
    std::int32_t nUpdateDate = 0;
    std::int32_t nShopIndex = 0;
};

inline XPacket& operator>>(XPacket& packet, ST_SHOP_ITEM& value) {
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse >> value.nUpdateDate;
    packet.XParse >> value.nShopIndex;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_SHOP_ITEM& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value.nUpdateDate;
    packet.XParse << value.nShopIndex;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_SHOP_ITEM& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value.nUpdateDate;
    packet.XParse << value.nShopIndex;
    return packet;
}

// 对齐 IDA: 商店物品列表
struct ST_SHOP_ITEM_LIST {
    std::vector<ST_SHOP_ITEM> vecInfo;
};

inline XPacket& operator>>(XPacket& packet, ST_SHOP_ITEM_LIST& value) {
    std::uint32_t count = 0;
    packet.XParse >> count;
    value.vecInfo.resize(count);
    for (auto& item : value.vecInfo) {
        packet >> item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_SHOP_ITEM_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_SHOP_ITEM_LIST& value) {
    packet.XParse << static_cast<std::uint32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA: 数据库商店物品（带UAID）
struct PS_DB_SHOP_ITEM {
    std::uint32_t dwUAID = 0;
    ST_SHOP_ITEM stShopItem;
};

inline XPacket& operator>>(XPacket& packet, PS_DB_SHOP_ITEM& value) {
    packet.XParse >> value.dwUAID;
    packet >> value.stShopItem;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_SHOP_ITEM& value) {
    packet.XParse << value.dwUAID;
    packet << value.stShopItem;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_SHOP_ITEM& value) {
    packet.XParse << value.dwUAID;
    packet << value.stShopItem;
    return packet;
}

// 对齐 IDA 0x14007A610: TWN平台登录请求
struct PS_LOGIN_REQ_FOR_TWN {
    char szAuthCode[1025] = {};
    char szAccountID[21] = {};
    char szMacAddress[18] = {};
    std::int32_t nPacketVersion = 0;
    std::int32_t nIP = 0;
    std::int32_t nSerialNo = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_LOGIN_REQ_FOR_TWN& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szAuthCode, 1025, &outLen);
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse.GetString(value.szMacAddress, 18, &outLen);
    packet.XParse >> value.nPacketVersion;
    packet.XParse >> value.nIP;
    packet.XParse >> value.nSerialNo;
    return packet;
}

// 对齐 IDA 0x14007AF00: CHN平台登录请求
struct PS_LOGIN_REQ_FOR_CHN {
    char szAuthCode[1025] = {};
    char szMacAddress[18] = {};
    char szUserChannel[256] = {};
    char szUserFrom[256] = {};
    char szQ_Mid[256] = {};
    char szQ_M2[256] = {};
    char szQ_MAC[256] = {};
    char UserSubsite[256] = {};
    char szAccountID[21] = {};
    std::int32_t nPacketVersion = 0;
    std::int32_t nIP = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_LOGIN_REQ_FOR_CHN& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szAuthCode, 1025, &outLen);
    packet.XParse.GetString(value.szMacAddress, 18, &outLen);
    packet.XParse.GetString(value.szUserChannel, 256, &outLen);
    packet.XParse.GetString(value.szUserFrom, 256, &outLen);
    packet.XParse.GetString(value.szQ_Mid, 256, &outLen);
    packet.XParse.GetString(value.szQ_M2, 256, &outLen);
    packet.XParse.GetString(value.szQ_MAC, 256, &outLen);
    packet.XParse.GetString(value.UserSubsite, 256, &outLen);
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse >> value.nPacketVersion;
    packet.XParse >> value.nIP;
    return packet;
}

// 对齐 IDA 0x140079CC0: GF平台登录请求
struct PS_LOGIN_REQ_FOR_GF {
    wchar_t szAuthCode[1025] = {};
    char szMacAddress[18] = {};
    std::int32_t nPacketVersion = 0;
    std::int32_t nIP = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_LOGIN_REQ_FOR_GF& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.szAuthCode, 1025, outLen);
    packet.XParse.GetString(value.szMacAddress, 18, &outLen);
    packet.XParse >> value.nPacketVersion;
    packet.XParse >> value.nIP;
    return packet;
}

// 注意：ST_GF_AUTH_INFO 已在 PSCommon.h 中定义，使用该定义

// ============================================================================
// DBAgent: 金币/BP更新相关结构
// ============================================================================

// 对齐 IDA 0x14001FDA0: 金币更新请求/响应结构
struct PS_DB_GOLD_UPDATE {
    unsigned int dwActorID = 0;
    std::int64_t nAddGold = 0;
    std::int64_t nTotalGold = 0;
};

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

// BP信息结构（内部使用）
struct PS_BP_INFO {
    unsigned int dwActorID = 0;
    std::int64_t biBP = 0;
};

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

// 对齐 IDA 0x140020060: BP更新请求结构
struct PS_DB_BP_UPDATE {
    PS_BP_INFO psBP{};
    std::int64_t nLimitMonsterBP = 0;
    std::int64_t nLimitPVPBP = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_DB_BP_UPDATE& value) {
    packet >> value.psBP;
    packet.XParse >> value.nLimitMonsterBP;
    packet.XParse >> value.nLimitPVPBP;
    return packet;
}

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
    std::vector<ST_TITLE_INFO_DB> vecTitleID;
    std::vector<ST_TITLE_INFO_DB> vecOpenTitleID;
};

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

// ============================================================================
// DBAgent: 称号操作相关结构
// ============================================================================

// 称号添加请求 - PS_TITLE_ADD
struct PS_TITLE_ADD {
    unsigned int dwTitleID = 0;
    int nLogType = 0;
};

inline void operator>>(XPacket& packet, PS_TITLE_ADD& value) {
    packet.XParse >> value.dwTitleID;
    packet.XParse >> value.nLogType;
}

// 称号信息结构（用于选择称号）
struct ST_TITLE_INFO_SELECT {
    unsigned int dwTitleID = 0;
    unsigned int dwSuffix = 0;
};

inline void operator>>(XPacket& packet, ST_TITLE_INFO_SELECT& value) {
    packet.XParse >> value.dwTitleID;
    packet.XParse >> value.dwSuffix;
}

inline XPacket& operator<<(XPacket& packet, const ST_TITLE_INFO_SELECT& value) {
    packet.XParse << value.dwTitleID;
    packet.XParse << value.dwSuffix;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const ST_TITLE_INFO_SELECT& value) {
    packet.XParse << value.dwTitleID;
    packet.XParse << value.dwSuffix;
    return packet;
}

// 称号选择请求 - PS_REQ_TITLE_UPDATE
struct PS_REQ_TITLE_UPDATE {
    ST_TITLE_INFO_SELECT stInsideTitle{};
    ST_TITLE_INFO_SELECT stOutsideTitle{};
};

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

// 称号收藏请求 - PS_TITLE_FAVORITE
struct PS_TITLE_FAVORITE {
    unsigned int dwUCID = 0;
    unsigned int dwTitleID = 0;
    bool bFavorite = false;
};

inline void operator>>(XPacket& packet, PS_TITLE_FAVORITE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwTitleID;
    packet.XParse >> value.bFavorite;
}

// 称号收藏数据库结构 - PS_DB_TITLE_FAVORITE
struct PS_DB_TITLE_FAVORITE {
    struct {
        unsigned int dwUCID = 0;
        unsigned int dwTitleID = 0;
        bool bFavorite = false;
    } psInfo{};
    int nError = 0;
};

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_TITLE_FAVORITE& value) {
    packet.XParse << value.psInfo.dwUCID;
    packet.XParse << value.psInfo.dwTitleID;
    packet.XParse << value.psInfo.bFavorite;
    packet.XParse << value.nError;
    return packet;
}

// ==================== DBAgent 角色位置/状态/FP 相关结构体 ====================

// 对齐 IDA: 角色位置更新请求结构
struct PS_CHARACTER_UPDATE_POS {
    unsigned int dwActorID = 0;
    UXMapID uxMapID{};
    XVec3 vPos{};
    int nRot = 0;
    int nJumpID = 0;
    unsigned int dwServerID = 0;
};

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

// 对齐 IDA: 无限塔限制时间更新结构
struct PS_UPDATE_INFINITETOWER_LIMIT_TIME {
    unsigned int dwActorID = 0;
    int nLimitTime = 0;
};

inline void operator>>(XPacket& packet, PS_UPDATE_INFINITETOWER_LIMIT_TIME& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.nLimitTime;
}

// 对齐 IDA: 无限塔清除章节结构
struct PS_UPDATE_INFINITETOWER_CLEAR_CHAPTER {
    unsigned int dwActorID = 0;
    std::int16_t sClearChapter = 0;
    std::int16_t sClearStage = 0;
};

inline void operator>>(XPacket& packet, PS_UPDATE_INFINITETOWER_CLEAR_CHAPTER& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.sClearChapter;
    packet.XParse >> value.sClearStage;
}

// ==================== DBAgent 世界状态/迷宫/区域相关结构体 ====================

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

// 对齐 IDA: 区域状态结构
struct ST_DISTRICT_STATE {
    int nDistrictID = 0;
    int nDSPoint = 0;
    bool bReward = false;
    std::uint8_t _pad0[3] = {};
};

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

// 迷宫状态列表
struct ST_MAZE_STATE_LIST {
    std::vector<ST_MAZE_STATE> vecList;
};

inline XPacket& operator<<(XPacket& packet, const ST_MAZE_STATE_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& item : value.vecList) {
        packet << item;
    }
    return packet;
}

// 区域状态列表
struct ST_DISTRICT_STATE_LIST {
    std::vector<ST_DISTRICT_STATE> vecList;
};

inline XPacket& operator<<(XPacket& packet, const ST_DISTRICT_STATE_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& item : value.vecList) {
        packet << item;
    }
    return packet;
}

// ==================== DBAgent 成就/社区/职业场景相关结构体 ====================

// 对齐 IDA: 成就信息结构
struct ST_ACHIEVE_INFO {
    int nIndex = 0;
    std::uint64_t biCount = 0;
};

inline void operator>>(XPacket& packet, ST_ACHIEVE_INFO& value) {
    packet.XParse >> value.nIndex;
    packet.XParse >> value.biCount;
}

inline XPacket& operator<<(XPacket& packet, const ST_ACHIEVE_INFO& value) {
    packet.XParse << value.nIndex;
    packet.XParse << value.biCount;
    return packet;
}

// 成就列表
struct ST_ACHIEVE_LIST {
    std::vector<ST_ACHIEVE_INFO> vecList;
};

inline XPacket& operator<<(XPacket& packet, const ST_ACHIEVE_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& item : value.vecList) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA: 成就位结构（128字节位域）
struct ST_ACHIEVE_BIT {
    std::uint8_t szRewardBit[128] = {};
};

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

// 成就类别计数序列化函数（结构体已在前面定义）
inline void operator>>(XPacket& packet, ST_ACHIEVE_CATEGORY& value) {
    for (int i = 0; i < 7; ++i) {
        packet.XParse >> value.wCount[i];
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_ACHIEVE_CATEGORY& value) {
    for (int i = 0; i < 7; ++i) {
        packet.XParse << value.wCount[i];
    }
    return packet;
}

// XSendDBPacket 操作符 - 成就相关结构
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

// PS_INFINITE_TOWER_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_INFINITE_TOWER_INFO& value) {
    packet.XParse << value.sClearChapter;
    packet.XParse << value.sClearStage;
    packet.XParse << value.nLimitTime;
    packet.XParse << value.sCount;
    return packet;
}

// PS_INFINITE_TOWER_INFO 反序列化
inline void operator>>(XPacket& packet, PS_INFINITE_TOWER_INFO& value) {
    packet.XParse >> value.sClearChapter;
    packet.XParse >> value.sClearStage;
    packet.XParse >> value.nLimitTime;
    packet.XParse >> value.sCount;
}

// 对齐 IDA: 击杀用户信息结构（size=8）
struct PS_KILLED_USER_INFO {
    std::uint32_t dwUCID = 0;       // 被击杀角色ID
    std::int32_t nCount = 0;        // 击杀次数
};
static_assert(sizeof(PS_KILLED_USER_INFO) == 8, "PS_KILLED_USER_INFO size mismatch with IDA");

// PS_KILLED_USER_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_KILLED_USER_INFO& value) {
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << value.nCount;
    return packet;
}

// PS_KILLED_USER_INFO 反序列化
inline void operator>>(XPacket& packet, PS_KILLED_USER_INFO& value) {
    int nUCID = 0;
    packet.XParse >> nUCID;
    value.dwUCID = static_cast<std::uint32_t>(nUCID);
    packet.XParse >> value.nCount;
}

// 对齐 IDA: 击杀用户列表结构（size=40）
struct PS_KILLED_USER_INFOS {
    std::int64_t nInitTime = 0;                     // 初始化时间 (offset 0)
    std::vector<PS_KILLED_USER_INFO> vecKilledUser; // 击杀用户列表 (offset 8)
};
static_assert(sizeof(PS_KILLED_USER_INFOS) == 40, "PS_KILLED_USER_INFOS size mismatch with IDA");

// PS_KILLED_USER_INFOS 序列化
inline XPacket& operator<<(XPacket& packet, const PS_KILLED_USER_INFOS& value) {
    packet.XParse << value.nInitTime;
    packet.XParse << static_cast<int>(value.vecKilledUser.size());
    for (const auto& item : value.vecKilledUser) {
        packet << item;
    }
    return packet;
}

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

// ST_REPRESENTATIVE_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_REPRESENTATIVE_INFO& value) {
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << static_cast<int>(value.byClass);
    packet.XParse << static_cast<int>(value.byLevel);
    packet.XParse << std::wstring(value.strName);
    packet.XParse << static_cast<int>(value.dwProfilePhotoID);
    packet.XParse << std::wstring(value.strLeagueName);
    return packet;
}

// ST_REPRESENTATIVE_INFO 反序列化
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

// 对齐 IDA: 成就更新结构
struct ST_ACHIEVE_UPDATE {
    ST_ACHIEVE_INFO stUpdateInfo{};
    int nNextIndex = 0;
    std::uint8_t byCategory = 0;
    std::uint16_t wCount = 0;
};

inline void operator>>(XPacket& packet, ST_ACHIEVE_UPDATE& value) {
    packet >> value.stUpdateInfo;
    packet.XParse >> value.nNextIndex;
    packet.XParse >> value.byCategory;
    packet.XParse >> value.wCount;
}

// 成就更新列表
struct ST_ACHIEVE_UPDATE_LIST {
    std::vector<ST_ACHIEVE_UPDATE> vecList;
};

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

// 对齐 IDA: 职业场景结构
struct PS_CLASS_SCENE {
    std::uint8_t byClassScene[6] = {};
};

inline void operator>>(XPacket& packet, PS_CLASS_SCENE& value) {
    for (int i = 0; i < 6; ++i) {
        packet.XParse >> value.byClassScene[i];
    }
}

// PT_ECHELON_INFO 定义在 PSCommon.h 中

// ============================================================
// ST_PROFILE_PHOTO_INFO 序列化操作符
// ============================================================

inline XPacket& operator>>(XPacket& packet, ST_PROFILE_PHOTO_INFO& value) {
    packet.XParse >> value.dwPhotoID;
    packet.XParse >> value.byPeriodType;
    packet.XParse >> value.byFavorite;
    packet.XParse >> value.byState;
    packet.XParse >> value.nEndDate;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_PROFILE_PHOTO_INFO& value) {
    packet.XParse << value.dwPhotoID;
    packet.XParse << value.byPeriodType;
    packet.XParse << value.byFavorite;
    packet.XParse << value.byState;
    packet.XParse << value.nEndDate;
    return packet;
}

// PS_PROFILE_PHOTO_LOAD 序列化操作符
inline XPacket& operator>>(XPacket& packet, PS_PROFILE_PHOTO_LOAD& value) {
    packet.XParse >> value.dwUCID;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_PROFILE_PHOTO_LOAD& value) {
    // 向量序列化
    std::int16_t nCount = static_cast<std::int16_t>(value.vecList.size());
    packet.XParse << nCount;
    for (const auto& item : value.vecList) {
        packet << item;
    }
    return packet;
}

// PS_DB_PROFILE_PHOTO_UPDATE 序列化操作符
inline XPacket& operator>>(XPacket& packet, PS_DB_PROFILE_PHOTO_UPDATE& value) {
    packet.XParse >> value.dwUCID;
    packet >> value.stInfo;
    packet.XParse >> value.nError;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_PROFILE_PHOTO_UPDATE& value) {
    packet.XParse << value.dwUCID;
    packet << value.stInfo;
    packet.XParse << value.nError;
    return packet;
}

// PS_DB_PROFILE_PHOTO_CHANGE 序列化操作符
inline XPacket& operator>>(XPacket& packet, PS_DB_PROFILE_PHOTO_CHANGE& value) {
    packet.XParse >> value.dwUCID;
    packet >> value.stNewPhotoInfo;
    packet.XParse >> value.nError;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_PROFILE_PHOTO_CHANGE& value) {
    packet.XParse << value.dwUCID;
    packet << value.stNewPhotoInfo;
    packet.XParse << value.nError;
    return packet;
}

// PS_DB_PROFILE_PHOTO_ADD 序列化操作符
inline XPacket& operator>>(XPacket& packet, PS_DB_PROFILE_PHOTO_ADD& value) {
    packet.XParse >> value.dwUCID;
    packet >> value.stInfo;
    // 向量反序列化
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.psUpdateItemList.clear();
    value.psUpdateItemList.reserve(static_cast<std::size_t>(nCount));
    for (int i = 0; i < nCount; ++i) {
        PS_STORAGE_INFO item;
        packet >> item;
        value.psUpdateItemList.push_back(item);
    }
    packet.XParse >> value.nError;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_DB_PROFILE_PHOTO_ADD& value) {
    packet.XParse << value.dwUCID;
    packet << value.stInfo;
    // 向量序列化
    std::int16_t nCount = static_cast<std::int16_t>(value.psUpdateItemList.size());
    packet.XParse << nCount;
    for (const auto& item : value.psUpdateItemList) {
        packet << item;
    }
    packet.XParse << value.nError;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_POST_RECEIPT_ALL_SERVER& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.biSerial;
    packet.XParse >> value.byFlag;
    packet.XParse >> value.byPostType;
    packet.XParse >> value.bDecrease;
    packet.XParse >> value._pad0;
    packet.XParse >> value.biRemainTime;
    packet.XParse >> value.wPostCount;
    packet.XParse >> value.nErrorCode;
    packet >> value.psCreateItem;
    packet >> value.psUpdateItem;
    packet >> value.psUpdateSerial;
    // 向量反序列化
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.stAppearanceList.clear();
    value.stAppearanceList.reserve(static_cast<std::size_t>(nCount));
    for (int i = 0; i < nCount; ++i) {
        ST_PROFILE_PHOTO_INFO item;
        packet >> item;
        value.stAppearanceList.push_back(item);
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_POST_RECEIPT_ALL_SERVER& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.biSerial;
    packet.XParse << value.byFlag;
    packet.XParse << value.byPostType;
    packet.XParse << value.bDecrease;
    packet.XParse << value._pad0;
    packet.XParse << value.biRemainTime;
    packet.XParse << value.wPostCount;
    packet.XParse << value.nErrorCode;
    packet << value.psCreateItem;
    packet << value.psUpdateItem;
    packet << value.psUpdateSerial;
    // 向量序列化
    packet.XParse << static_cast<std::int16_t>(value.stAppearanceList.size());
    for (const auto& item : value.stAppearanceList) {
        packet << item;
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_POST_RECEIPT_ALL_SERVER& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.biSerial;
    packet.XParse << value.byFlag;
    packet.XParse << value.byPostType;
    packet.XParse << value.bDecrease;
    packet.XParse << value._pad0;
    packet.XParse << value.biRemainTime;
    packet.XParse << value.wPostCount;
    packet.XParse << value.nErrorCode;
    packet << value.psCreateItem;
    packet << value.psUpdateItem;
    packet << value.psUpdateSerial;
    // 向量序列化
    packet.XParse << static_cast<std::int16_t>(value.stAppearanceList.size());
    for (const auto& item : value.stAppearanceList) {
        packet << item;
    }
    return packet;
}

// ============================================================
// PS_DB_ITEM_MOVE - 物品移动/合并请求结构
// 来自 IDA: 280 bytes, 14 fields
// ============================================================

/**
 * @brief 物品移动/合并请求数据库结构。
 *
 * 用于物品移动、合并、分割等操作的请求数据包。
 * 字段布局来自 IDA 反编译 operator<< / operator>>。
 * 总大小: 280 bytes
 */
struct PS_DB_ITEM_MOVE {
    std::uint32_t dwSrcActorID = 0;       // +0x00: 源角色ID (4 bytes)
    std::uint8_t bySrcInvenType = 0;      // +0x04: 源背包类型 (1 byte)
    std::uint8_t _pad0[1] = {};           // +0x05: padding (1 byte)
    std::int16_t shSrcSlotPos = 0;        // +0x06: 源槽位位置 (2 bytes)
    STItem stSrcItem{};                   // +0x08: 源物品信息 (120 bytes)
    std::uint8_t bySrcStoreType = 0;      // +0x80: 源存储类型 (1 byte)
    std::uint8_t _pad1[3] = {};           // +0x81: padding (3 bytes)
    std::uint32_t dwDestActorID = 0;      // +0x84: 目标角色ID (4 bytes)
    std::uint8_t byDestInvenType = 0;     // +0x88: 目标背包类型 (1 byte)
    std::uint8_t _pad2[3] = {};           // +0x89: padding (3 bytes)
    std::int16_t shDestSlotPos = 0;       // +0x8C: 目标槽位位置 (2 bytes)
    std::uint8_t _pad3[2] = {};           // +0x8E: padding (2 bytes)
    STItem stDestItem{};                  // +0x90: 目标物品信息 (120 bytes)
    std::uint8_t byDestStoreType = 0;     // +0x108: 目标存储类型 (1 byte)
    std::uint8_t _pad4[3] = {};           // +0x109: padding (3 bytes)
    std::uint8_t bSyncToClient = 0;       // +0x10C: 是否同步到客户端 (1 byte)
    std::uint8_t _pad5[7] = {};           // +0x10D: padding to 280 bytes
};

static_assert(sizeof(PS_DB_ITEM_MOVE) == 280, "PS_DB_ITEM_MOVE size must match PDB");
static_assert(offsetof(PS_DB_ITEM_MOVE, bySrcInvenType) == 4, "PS_DB_ITEM_MOVE.bySrcInvenType offset mismatch");
static_assert(offsetof(PS_DB_ITEM_MOVE, shSrcSlotPos) == 6, "PS_DB_ITEM_MOVE.shSrcSlotPos offset mismatch");
static_assert(offsetof(PS_DB_ITEM_MOVE, stSrcItem) == 8, "PS_DB_ITEM_MOVE.stSrcItem offset mismatch");
static_assert(offsetof(PS_DB_ITEM_MOVE, bySrcStoreType) == 128, "PS_DB_ITEM_MOVE.bySrcStoreType offset mismatch");
static_assert(offsetof(PS_DB_ITEM_MOVE, dwDestActorID) == 132, "PS_DB_ITEM_MOVE.dwDestActorID offset mismatch");
static_assert(offsetof(PS_DB_ITEM_MOVE, stDestItem) == 144, "PS_DB_ITEM_MOVE.stDestItem offset mismatch");
static_assert(offsetof(PS_DB_ITEM_MOVE, bSyncToClient) == 268, "PS_DB_ITEM_MOVE.bSyncToClient offset mismatch");

inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_MOVE& value) {
    packet.XParse << value.dwSrcActorID;
    packet.XParse << value.bySrcInvenType;
    packet.XParse << value.shSrcSlotPos;
    packet << value.stSrcItem;
    packet.XParse << value.bySrcStoreType;
    packet.XParse << value.dwDestActorID;
    packet.XParse << value.byDestInvenType;
    packet.XParse << value.shDestSlotPos;
    packet << value.stDestItem;
    packet.XParse << value.byDestStoreType;
    packet.XParse << value.bSyncToClient;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_DB_ITEM_MOVE& value) {
    packet.XParse >> value.dwSrcActorID;
    packet.XParse >> value.bySrcInvenType;
    packet.XParse >> value.shSrcSlotPos;
    packet >> value.stSrcItem;
    packet.XParse >> value.bySrcStoreType;
    packet.XParse >> value.dwDestActorID;
    packet.XParse >> value.byDestInvenType;
    packet.XParse >> value.shDestSlotPos;
    packet >> value.stDestItem;
    packet.XParse >> value.byDestStoreType;
    packet.XParse >> value.bSyncToClient;
    return packet;
}

/**
 * @brief 物品移动请求列表结构。
 *
 * 用于批量物品移动操作。
 * 总大小: 32 bytes
 */
struct PS_DB_ITEM_MOVE_VEC {
    std::uint32_t dwActorID = 0;          // +0x00: 角色ID (4 bytes)
    std::uint8_t byInvenType = 0;         // +0x04: 背包类型 (1 byte)
    std::uint8_t _pad0[1] = {};           // +0x05: padding (1 byte)
    std::int16_t shSlotPos = 0;           // +0x06: 槽位位置 (2 bytes)
    std::uint8_t _pad1[4] = {};           // +0x08: padding (4 bytes)
    std::vector<PS_DB_ITEM_MOVE> vecItems; // +0x10: 物品列表向量 (24 bytes on 64-bit)
};

// Note: PS_DB_ITEM_MOVE_VEC size depends on platform (24 bytes vector header on 64-bit)
// PDB reports 32 bytes, but that's for 32-bit builds. We skip static_assert for cross-platform compatibility.

inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_MOVE_VEC& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.byInvenType;
    packet.XParse << value.shSlotPos;
    packet.XParse << static_cast<std::int16_t>(value.vecItems.size());
    for (const auto& item : value.vecItems) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_DB_ITEM_MOVE_VEC& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.byInvenType;
    packet.XParse >> value.shSlotPos;
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecItems.clear();
    value.vecItems.reserve(static_cast<std::size_t>(nCount));
    for (int i = 0; i < nCount; ++i) {
        PS_DB_ITEM_MOVE item;
        packet >> item;
        value.vecItems.push_back(item);
    }
    return packet;
}

// PS_QUICKSLOT_CARD 序列化操作符 - 结构体已在前面定义
inline XPacket& operator<<(XPacket& packet, const PS_QUICKSLOT_CARD& value) {
    packet.XParse << value.byPage;
    packet.XParse << GreenDamTan_BoundedWideString(value.szDeckName);
    for (int i = 0; i < 5; ++i) {
        packet.XParse << value.uniCard[i];
    }
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_QUICKSLOT_CARD& value) {
    packet.XParse >> value.byPage;
    short outLen = 0;
    packet.XParse.GetWString(value.szDeckName, 13, outLen);
    for (int i = 0; i < 5; ++i) {
        packet.XParse >> value.uniCard[i];
    }
    return packet;
}

/**
 * @brief 快捷栏物品信息 - 20 bytes
 * 来自 IDA: PS_QUICKSLOT_ITEM
 */
struct PS_QUICKSLOT_ITEM {
    std::uint32_t uxActorID = 0;        // +0x00: 角色ID (4 bytes)
    std::uint32_t dwItem_1 = 0;         // +0x04: 物品槽位1 (4 bytes)
    std::uint32_t dwItem_2 = 0;         // +0x08: 物品槽位2 (4 bytes)
    std::uint32_t dwItem_3 = 0;         // +0x0C: 物品槽位3 (4 bytes)
    std::uint32_t dwItem_4 = 0;         // +0x10: 物品槽位4 (4 bytes)
};

static_assert(sizeof(PS_QUICKSLOT_ITEM) == 20, "PS_QUICKSLOT_ITEM size must match IDA");

inline XPacket& operator<<(XPacket& packet, const PS_QUICKSLOT_ITEM& value) {
    packet.XParse << value.uxActorID;
    packet.XParse << value.dwItem_1;
    packet.XParse << value.dwItem_2;
    packet.XParse << value.dwItem_3;
    packet.XParse << value.dwItem_4;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_QUICKSLOT_ITEM& value) {
    packet.XParse >> value.uxActorID;
    packet.XParse >> value.dwItem_1;
    packet.XParse >> value.dwItem_2;
    packet.XParse >> value.dwItem_3;
    packet.XParse >> value.dwItem_4;
    return packet;
}

// PS_QUICKSLOT_CARD 结构体及序列化函数已在前面定义

/**
 * @brief 快捷栏卡片列表 - 40 bytes (64-bit)
 * 来自 IDA: PS_QUICKSLOT_CARD_VEC
 */
struct PS_QUICKSLOT_CARD_VEC {
    std::uint8_t byActivePage = 0;          // +0x00: 活动页面索引 (1 byte)
    std::uint8_t _pad0[7] = {};             // +0x01: padding (7 bytes)
    std::vector<PS_QUICKSLOT_CARD> vecInfo; // +0x08: 卡片列表向量 (32 bytes on 64-bit)
};

inline XPacket& operator<<(XPacket& packet, const PS_QUICKSLOT_CARD_VEC& value) {
    packet.XParse << value.byActivePage;
    packet.XParse << static_cast<std::int16_t>(value.vecInfo.size());
    for (const auto& card : value.vecInfo) {
        packet << card;
    }
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_QUICKSLOT_CARD_VEC& value) {
    packet.XParse >> value.byActivePage;
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(nCount));
    for (int i = 0; i < nCount; ++i) {
        PS_QUICKSLOT_CARD card;
        packet >> card;
        value.vecInfo.push_back(card);
    }
    return packet;
}

/**
 * @brief 快捷栏卡片更新信息 - 24 bytes
 * 来自 IDA: PS_QUICKSLOT_UPDATE_CARD
 */
struct PS_QUICKSLOT_UPDATE_CARD {
    std::uint8_t byPage = 0;        // +0x00: 页面索引
    std::uint8_t _pad0[3] = {};     // +0x01: padding (3 bytes)
    std::int32_t nCard_1 = 0;       // +0x04: 卡片槽位1
    std::int32_t nCard_2 = 0;       // +0x08: 卡片槽位2
    std::int32_t nCard_3 = 0;       // +0x0C: 卡片槽位3
    std::int32_t nCard_4 = 0;       // +0x10: 卡片槽位4
    std::int32_t nCard_5 = 0;       // +0x14: 卡片槽位5
};

static_assert(sizeof(PS_QUICKSLOT_UPDATE_CARD) == 24, "PS_QUICKSLOT_UPDATE_CARD size must match IDA");

/**
 * @brief 快捷栏卡片更新列表
 * 来自 IDA: PS_QUICKSLOT_UPDATE_CARD_VEC
 */
struct PS_QUICKSLOT_UPDATE_CARD_VEC {
    std::vector<PS_QUICKSLOT_UPDATE_CARD> vecInfo;
};

inline XPacket& operator>>(XPacket& packet, PS_QUICKSLOT_UPDATE_CARD& value) {
    packet.XParse >> value.byPage;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse >> value.nCard_1;
    packet.XParse >> value.nCard_2;
    packet.XParse >> value.nCard_3;
    packet.XParse >> value.nCard_4;
    packet.XParse >> value.nCard_5;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_QUICKSLOT_UPDATE_CARD_VEC& value) {
    std::uint32_t nCount = 0;
    packet.XParse >> nCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(nCount);
    for (std::uint32_t i = 0; i < nCount; ++i) {
        PS_QUICKSLOT_UPDATE_CARD card;
        packet >> card;
        value.vecInfo.push_back(card);
    }
    return packet;
}

/**
 * @brief 阿卡夏记录信息
 * Per IDA 0x1400566B0: ST_AKASHIC_RECORD
 */
struct ST_AKASHIC_RECORD {
    std::uint32_t dwAkashicID = 0;
    std::uint8_t byState = 0;
    std::int32_t nAkashicExp = 0;
};

/**
 * @brief 阿卡夏记录列表
 */
struct ST_AKASHIC_LIST {
    std::vector<ST_AKASHIC_RECORD> vecInfo;
};

inline XPacket& operator<<(XPacket& packet, const ST_AKASHIC_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet.XParse << info.dwAkashicID;
        packet.XParse << info.byState;
        packet.XParse << info.nAkashicExp;
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_AKASHIC_LIST& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_AKASHIC_RECORD info;
        packet.XParse >> info.dwAkashicID;
        packet.XParse >> info.byState;
        packet.XParse >> info.nAkashicExp;
        value.vecInfo.push_back(info);
    }
}

/**
 * @brief 物品使用信息 - 16 bytes
 * 来自 IDA: ST_USE_ITEM_INFO
 */
struct ST_USE_ITEM_INFO {
    std::int32_t nItemType = 0;     // +0x00: 物品类型ID (4 bytes)
    std::uint8_t byCount = 0;       // +0x04: 使用次数 (1 byte)
    std::uint8_t _pad0[3] = {};     // +0x05: padding (3 bytes)
    std::int64_t nUseDate = 0;      // +0x08: 使用日期时间 (8 bytes)
};

static_assert(sizeof(ST_USE_ITEM_INFO) == 16, "ST_USE_ITEM_INFO size must match IDA");

/**
 * @brief 物品使用信息列表
 * 来自 IDA: ST_USE_ITEM_INFO_LIST
 */
struct ST_USE_ITEM_INFO_LIST {
    std::vector<ST_USE_ITEM_INFO> vecInfo;
};

inline XPacket& operator<<(XPacket& packet, const ST_USE_ITEM_INFO_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecInfo.size());
    for (const auto& info : value.vecInfo) {
        packet.XParse << info.nItemType;
        packet.XParse << info.byCount;
        packet.XParse << info.nUseDate;
    }
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_USE_ITEM_INFO_LIST& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_USE_ITEM_INFO info;
        packet.XParse >> info.nItemType;
        packet.XParse >> info.byCount;
        packet.XParse >> info.nUseDate;
        value.vecInfo.push_back(info);
    }
    return packet;
}

// ============================================================
// GMT Post 相关结构体 (GM工具邮件系统)
// ============================================================

/**
 * @brief 系统邮件物品项 - 24 bytes
 * 来自 IDA: ST_SYSTEM_POST_ITEM
 */
struct ST_SYSTEM_POST_ITEM {
    std::int32_t nItemID = 0;      // +0x00: 物品ID (4 bytes)
    std::int16_t shCount = 0;     // +0x04: 数量 (2 bytes)
    std::int16_t _pad0 = 0;       // +0x06: padding (2 bytes)
    std::int32_t nAttack = 0;     // +0x08: 攻击力 (4 bytes)
    std::int32_t nDefense = 0;    // +0x0C: 防御力 (4 bytes)
    std::int32_t nTitleID = 0;    // +0x10: 称号ID (4 bytes)
    std::int32_t nDyeID = 0;      // +0x14: 染色ID (4 bytes)
};

static_assert(sizeof(ST_SYSTEM_POST_ITEM) == 24, "ST_SYSTEM_POST_ITEM size must match IDA");

/**
 * @brief 系统邮件结构 - 1064 bytes
 * 来自 IDA: ST_SYSTEM_POST
 */
struct ST_SYSTEM_POST {
    std::uint8_t byPostType = 0;              // +0x00: 邮件类型 (1 byte)
    std::uint8_t byPostSubType = 0;           // +0x01: 邮件子类型 (1 byte)
    wchar_t strTitle[41] = {};                // +0x02: 邮件标题 (82 bytes)
    wchar_t strMsg[401] = {};                 // +0x54: 邮件内容 (802 bytes)
    wchar_t strName[21] = {};                 // +0x376: 发送者名字 (42 bytes)
    ST_SYSTEM_POST_ITEM stSysItem[5] = {};    // +0x3A0: 物品列表 (5 * 24 = 120 bytes)
    std::int64_t biMoney = 0;                 // +0x418: 金钱 (8 bytes)
    std::int64_t _pad0 = 0;                   // +0x420: padding (8 bytes)
};

static_assert(sizeof(ST_SYSTEM_POST) == 1064, "ST_SYSTEM_POST size must match IDA");

/**
 * @brief GMT邮件条件 - 12 bytes
 * 来自 IDA: ST_GMT_POST_CONDITION
 */
struct ST_GMT_POST_CONDITION {
    std::uint8_t byConditionType = 0;   // +0x00: 条件类型 (1 byte)
    std::uint8_t _pad0[3] = {};         // +0x01: padding (3 bytes)
    std::int32_t nMin = 0;              // +0x04: 最小值 (4 bytes)
    std::int32_t nMax = 0;              // +0x08: 最大值 (4 bytes)
};

static_assert(sizeof(ST_GMT_POST_CONDITION) == 12, "ST_GMT_POST_CONDITION size must match IDA");

/**
 * @brief GMT邮件信息 - 1784 bytes
 * 来自 IDA: ST_GMT_POST_INFO
 */
struct ST_GMT_POST_INFO {
    std::int64_t biNo = 0;                        // +0x00: 编号 (8 bytes)
    std::uint32_t dwUCID = 0;                     // +0x08: 角色ID (4 bytes)
    std::uint8_t bySystemMailSubType = 0;        // +0x0C: 系统邮件子类型 (1 byte)
    std::uint8_t byPostSubType = 0;               // +0x0D: 邮件子类型 (1 byte)
    wchar_t strTitle[41] = {};                    // +0x0E: 邮件标题 (82 bytes)
    wchar_t strMsg[401] = {};                     // +0x60: 邮件内容 (802 bytes)
    ST_SYSTEM_POST_ITEM stSysItem[5] = {};       // +0x384: 系统物品列表 (120 bytes)
    std::int64_t biItemSerial[5] = {};           // +0x3FC: 物品序列号 (40 bytes)
    bool bSendPost = false;                       // +0x424: 是否发送 (1 byte)
    std::uint8_t _pad0[7] = {};                   // +0x425: padding (7 bytes)
    std::int64_t biPostSerial = 0;                // +0x42C: 邮件序列号 (8 bytes)
    std::int64_t biGold = 0;                      // +0x434: 金钱 (8 bytes)
    wchar_t strName[21] = {};                     // +0x43C: 发送者名字 (42 bytes)
    std::int64_t biDelDate = 0;                   // +0x466: 删除日期 (8 bytes)
    STItem stItem[5] = {};                        // +0x46E: 物品列表 (600 bytes)
    ST_GMT_POST_CONDITION stCondition[3] = {};   // +0x6C4: 条件列表 (36 bytes)
};

static_assert(sizeof(ST_GMT_POST_INFO) == 1784, "ST_GMT_POST_INFO size must match IDA");

/**
 * @brief GMT邮件发送数据 - 1848 bytes
 * 来自 IDA: ST_GMT_POST_SEND
 */
struct ST_GMT_POST_SEND {
    ST_GMT_POST_INFO stPost{};                    // +0x000: 邮件信息 (1784 bytes)
    std::int64_t biNo = 0;                        // +0x6F8: 编号 (8 bytes)
    std::int64_t biRegTime = 0;                   // +0x700: 注册时间 (8 bytes)
    std::uint16_t wPostCount = 0;                 // +0x708: 邮件计数 (2 bytes)
    std::uint8_t _pad0[6] = {};                   // +0x70A: padding (6 bytes)
    std::int64_t biPostSerial = 0;                // +0x710: 邮件序列号 (8 bytes)
    std::vector<ST_POST_DATA> vecPostData;        // +0x718: 邮件数据列表 (32 bytes)
};

static_assert(sizeof(ST_GMT_POST_SEND) == 1848, "ST_GMT_POST_SEND size must match IDA");

/**
 * @brief GMT邮件列表请求/响应
 * 来自 IDA: PS_GMT_POST_LIST
 */
struct PS_GMT_POST_LIST {
    std::vector<ST_GMT_POST_INFO> vecPostList;    // +0x00: 邮件列表 (32 bytes)
    std::int32_t nRefreshPostType = 0;            // +0x20: 刷新类型 (4 bytes)
    bool bLast = false;                           // +0x24: 是否最后 (1 byte)
};

static_assert(sizeof(PS_GMT_POST_LIST) == 40, "PS_GMT_POST_LIST size must match IDA");

/**
 * @brief GMT邮件发送列表
 * 来自 IDA: PS_GMT_POST_SEND_LIST
 */
struct PS_GMT_POST_SEND_LIST {
    std::uint32_t dwUCID = 0;                     // +0x00: 角色ID (4 bytes)
    std::uint8_t _pad0[4] = {};                   // +0x04: padding (4 bytes)
    std::vector<ST_GMT_POST_SEND> vecPostList;    // +0x08: 邮件发送列表 (32 bytes)
    std::int32_t nRefreshPostType = 0;            // +0x28: 刷新类型 (4 bytes)
    bool bLast = false;                           // +0x2C: 是否最后 (1 byte)
};

static_assert(sizeof(PS_GMT_POST_SEND_LIST) == 48, "PS_GMT_POST_SEND_LIST size must match IDA");

/**
 * @brief 邮件列表
 * 来自 IDA: ST_POST_LIST
 */
struct ST_POST_LIST {
    std::vector<ST_POST_DATA> vecData;
};

static_assert(sizeof(ST_POST_LIST) == 32, "ST_POST_LIST size must match IDA");

// GMT Post 序列化操作符
inline XPacket& operator<<(XPacket& packet, const ST_SYSTEM_POST_ITEM& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.shCount;
    packet.XParse << value.nAttack;
    packet.XParse << value.nDefense;
    packet.XParse << value.nTitleID;
    packet.XParse << value.nDyeID;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_SYSTEM_POST_ITEM& value) {
    packet.XParse >> value.nItemID;
    packet.XParse >> value.shCount;
    packet.XParse >> value.nAttack;
    packet.XParse >> value.nDefense;
    packet.XParse >> value.nTitleID;
    packet.XParse >> value.nDyeID;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_SYSTEM_POST& value) {
    packet.XParse << value.byPostType;
    packet.XParse << value.byPostSubType;
    packet.XParse << GreenDamTan_BoundedWideString(value.strTitle);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    for (int i = 0; i < 5; ++i) packet << value.stSysItem[i];
    packet.XParse << value.biMoney;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_SYSTEM_POST& value) {
    packet.XParse >> value.byPostType;
    packet.XParse >> value.byPostSubType;
    std::int16_t sLen = 0;
    packet.XParse.GetWString(value.strTitle, 41, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.strMsg, 401, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.strName, 21, sLen);
    for (int i = 0; i < 5; ++i) packet >> value.stSysItem[i];
    packet.XParse >> value.biMoney;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_GMT_POST_CONDITION& value) {
    packet.XParse << value.byConditionType;
    packet.XParse << value.nMin;
    packet.XParse << value.nMax;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_GMT_POST_CONDITION& value) {
    packet.XParse >> value.byConditionType;
    packet.XParse >> value.nMin;
    packet.XParse >> value.nMax;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_GMT_POST_INFO& value) {
    packet.XParse << value.biNo;
    packet.XParse << value.dwUCID;
    packet.XParse << value.bySystemMailSubType;
    packet.XParse << value.byPostSubType;
    packet.XParse << GreenDamTan_BoundedWideString(value.strTitle);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    for (int i = 0; i < 5; ++i) packet << value.stSysItem[i];
    for (int i = 0; i < 5; ++i) packet.XParse << value.biItemSerial[i];
    packet.XParse << static_cast<std::uint8_t>(value.bSendPost ? 1 : 0);
    packet.XParse << value.biPostSerial;
    packet.XParse << value.biGold;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << value.biDelDate;
    for (int i = 0; i < 5; ++i) packet << value.stItem[i];
    for (int i = 0; i < 3; ++i) packet << value.stCondition[i];
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_GMT_POST_INFO& value) {
    packet.XParse >> value.biNo;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.bySystemMailSubType;
    packet.XParse >> value.byPostSubType;
    std::int16_t sLen = 0;
    packet.XParse.GetWString(value.strTitle, 41, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.strMsg, 401, sLen);
    sLen = 0;
    packet.XParse.GetWString(value.strName, 21, sLen);
    for (int i = 0; i < 5; ++i) packet >> value.stSysItem[i];
    for (int i = 0; i < 5; ++i) packet.XParse >> value.biItemSerial[i];
    std::uint8_t bSendPost = 0;
    packet.XParse >> bSendPost;
    value.bSendPost = (bSendPost != 0);
    packet.XParse >> value.biPostSerial;
    packet.XParse >> value.biGold;
    packet.XParse >> value.biDelDate;
    for (int i = 0; i < 5; ++i) packet >> value.stItem[i];
    for (int i = 0; i < 3; ++i) packet >> value.stCondition[i];
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_GMT_POST_SEND& value) {
    packet << value.stPost;
    packet.XParse << value.biNo;
    packet.XParse << value.biRegTime;
    packet.XParse << value.wPostCount;
    packet.XParse << value.biPostSerial;
    // vecPostData 序列化
    packet.XParse << static_cast<std::int16_t>(value.vecPostData.size());
    for (const auto& data : value.vecPostData) {
        packet << data;
    }
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_GMT_POST_SEND& value) {
    packet >> value.stPost;
    packet.XParse >> value.biNo;
    packet.XParse >> value.biRegTime;
    packet.XParse >> value.wPostCount;
    packet.XParse >> value.biPostSerial;
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecPostData.clear();
    value.vecPostData.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_POST_DATA data;
        packet >> data;
        value.vecPostData.push_back(data);
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_GMT_POST_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecPostList.size());
    for (const auto& info : value.vecPostList) {
        packet << info;
    }
    packet.XParse << value.nRefreshPostType;
    packet.XParse << static_cast<std::uint8_t>(value.bLast ? 1 : 0);
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_GMT_POST_LIST& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecPostList.clear();
    value.vecPostList.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_GMT_POST_INFO info;
        packet >> info;
        value.vecPostList.push_back(info);
    }
    packet.XParse >> value.nRefreshPostType;
    std::uint8_t bLast = 0;
    packet.XParse >> bLast;
    value.bLast = (bLast != 0);
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_GMT_POST_SEND_LIST& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << static_cast<std::int16_t>(value.vecPostList.size());
    for (const auto& send : value.vecPostList) {
        packet << send;
    }
    packet.XParse << value.nRefreshPostType;
    packet.XParse << static_cast<std::uint8_t>(value.bLast ? 1 : 0);
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_GMT_POST_SEND_LIST& value) {
    packet.XParse >> value.dwUCID;
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecPostList.clear();
    value.vecPostList.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_GMT_POST_SEND send;
        packet >> send;
        value.vecPostList.push_back(send);
    }
    packet.XParse >> value.nRefreshPostType;
    std::uint8_t bLast = 0;
    packet.XParse >> bLast;
    value.bLast = (bLast != 0);
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_POST_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecData.size());
    for (const auto& data : value.vecData) {
        packet << data;
    }
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_POST_LIST& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecData.clear();
    value.vecData.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_POST_DATA data;
        packet >> data;
        value.vecData.push_back(data);
    }
    return packet;
}

// ============================================================
// Han Net Cafe (网吧) 相关结构体
// ============================================================

/**
 * @brief 网吧连接用户数据 - 36 bytes
 * 来自 IDA: PS_DB_HAN_NET_CAFE
 */
struct PS_DB_HAN_NET_CAFE {
    std::uint32_t dwUAID = 0;           // +0x00: 账号ID (4 bytes)
    char szAccountID[21] = {};          // +0x04: 账号ID字符串 (21 bytes)
    std::int32_t nServerID = 0;         // +0x1C: 服务器ID (4 bytes)
    bool bLogin = false;                // +0x20: 是否登录 (1 byte)
};

static_assert(sizeof(PS_DB_HAN_NET_CAFE) == 36, "PS_DB_HAN_NET_CAFE size must match IDA");

/**
 * @brief 网吧连接用户列表 - 40 bytes
 * 来自 IDA: PS_DB_HAN_NET_CAFE_LIST
 */
struct PS_DB_HAN_NET_CAFE_LIST {
    std::vector<PS_DB_HAN_NET_CAFE> vecList;  // +0x00: 用户列表 (32 bytes)
    bool bLast = false;                       // +0x20: 是否最后 (1 byte)
};

static_assert(sizeof(PS_DB_HAN_NET_CAFE_LIST) == 40, "PS_DB_HAN_NET_CAFE_LIST size must match IDA");

// 序列化操作符
inline XPacket& operator<<(XPacket& packet, const PS_DB_HAN_NET_CAFE& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << std::string(value.szAccountID);
    packet.XParse << value.nServerID;
    packet.XParse << static_cast<std::uint8_t>(value.bLogin ? 1 : 0);
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_DB_HAN_NET_CAFE& value) {
    std::string szAccountID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> szAccountID;
    packet.XParse >> value.nServerID;
    std::uint8_t bLogin = 0;
    packet.XParse >> bLogin;
    value.bLogin = (bLogin != 0);
    std::strncpy(value.szAccountID, szAccountID.c_str(), sizeof(value.szAccountID) - 1);
    value.szAccountID[sizeof(value.szAccountID) - 1] = '\0';
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_HAN_NET_CAFE_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& cafe : value.vecList) {
        packet << cafe;
    }
    packet.XParse << static_cast<std::uint8_t>(value.bLast ? 1 : 0);
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_DB_HAN_NET_CAFE_LIST& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecList.clear();
    value.vecList.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        PS_DB_HAN_NET_CAFE cafe;
        packet >> cafe;
        value.vecList.push_back(cafe);
    }
    std::uint8_t bLast = 0;
    packet.XParse >> bLast;
    value.bLast = (bLast != 0);
    return packet;
}

// ============================================================
// World Event (世界事件) 相关结构体
// ============================================================

/**
 * @brief 世界事件奖励信息 - 8 bytes
 * 来自 IDA: ST_WORLD_EVENT_REWARD_INFO
 */
struct ST_WORLD_EVENT_REWARD_INFO {
    std::int32_t nRewardIndex = 0;      // +0x00: 奖励索引 (4 bytes)
    std::uint8_t byRewardType = 0;      // +0x04: 奖励类型 (1 byte)
    std::uint8_t byRewardState = 0;     // +0x05: 奖励状态 (1 byte)
};

static_assert(sizeof(ST_WORLD_EVENT_REWARD_INFO) == 8, "ST_WORLD_EVENT_REWARD_INFO size must match IDA");

/**
 * @brief 世界事件信息请求 - 12 bytes
 * 来自 IDA: PS_DB_WORLD_EVENT_INFO_REQ
 */
struct PS_DB_WORLD_EVENT_INFO_REQ {
    std::uint32_t dwUAID = 0;           // +0x00: 账号ID (4 bytes)
    std::uint32_t dwUCID = 0;           // +0x04: 角色ID (4 bytes)
    std::int32_t nEventID = 0;          // +0x08: 事件ID (4 bytes)
};

static_assert(sizeof(PS_DB_WORLD_EVENT_INFO_REQ) == 12, "PS_DB_WORLD_EVENT_INFO_REQ size must match IDA");

/**
 * @brief 世界事件信息响应 - 48 bytes
 * 来自 IDA: PS_WORLD_EVENT_INFO_RES
 */
struct PS_WORLD_EVENT_INFO_RES {
    std::int32_t nEventID = 0;                      // +0x00: 事件ID (4 bytes)
    std::int32_t nTotalCount = 0;                   // +0x04: 总计数 (4 bytes)
    std::int32_t nMyCount = 0;                      // +0x08: 我的计数 (4 bytes)
    std::uint8_t byDailyRewardState = 0;           // +0x0C: 每日奖励状态 (1 byte)
    std::uint8_t _pad0[3] = {};                    // +0x0D: padding (3 bytes)
    std::vector<ST_WORLD_EVENT_REWARD_INFO> vecRewardInfo;  // +0x10: 奖励列表 (32 bytes)
};

static_assert(sizeof(PS_WORLD_EVENT_INFO_RES) == 48, "PS_WORLD_EVENT_INFO_RES size must match IDA");

/**
 * @brief 世界事件信息完整响应 - 64 bytes
 * 来自 IDA: PS_DB_WORLD_EVENT_INFO_RES
 */
struct PS_DB_WORLD_EVENT_INFO_RES {
    PS_WORLD_EVENT_INFO_RES psInfo{};              // +0x00: 事件信息 (48 bytes)
    std::int64_t biLastRegisterDate = 0;            // +0x30: 最后注册日期 (8 bytes)
    std::int64_t biDailyRewardDate = 0;             // +0x38: 每日奖励日期 (8 bytes)
};

static_assert(sizeof(PS_DB_WORLD_EVENT_INFO_RES) == 64, "PS_DB_WORLD_EVENT_INFO_RES size must match IDA");

// 序列化操作符
inline XPacket& operator<<(XPacket& packet, const ST_WORLD_EVENT_REWARD_INFO& value) {
    packet.XParse << value.nRewardIndex;
    packet.XParse << value.byRewardType;
    packet.XParse << value.byRewardState;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_WORLD_EVENT_REWARD_INFO& value) {
    packet.XParse >> value.nRewardIndex;
    packet.XParse >> value.byRewardType;
    packet.XParse >> value.byRewardState;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_WORLD_EVENT_INFO_REQ& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.nEventID;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_DB_WORLD_EVENT_INFO_REQ& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nEventID;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_WORLD_EVENT_INFO_RES& value) {
    packet.XParse << value.nEventID;
    packet.XParse << value.nTotalCount;
    packet.XParse << value.nMyCount;
    packet.XParse << value.byDailyRewardState;
    packet.XParse << static_cast<std::int16_t>(value.vecRewardInfo.size());
    for (const auto& reward : value.vecRewardInfo) {
        packet << reward;
    }
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_WORLD_EVENT_INFO_RES& value) {
    packet.XParse >> value.nEventID;
    packet.XParse >> value.nTotalCount;
    packet.XParse >> value.nMyCount;
    packet.XParse >> value.byDailyRewardState;
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecRewardInfo.clear();
    value.vecRewardInfo.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_WORLD_EVENT_REWARD_INFO reward;
        packet >> reward;
        value.vecRewardInfo.push_back(reward);
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_WORLD_EVENT_INFO_RES& value) {
    packet << value.psInfo;
    packet.XParse << value.biLastRegisterDate;
    packet.XParse << value.biDailyRewardDate;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_DB_WORLD_EVENT_INFO_RES& value) {
    packet >> value.psInfo;
    packet.XParse >> value.biLastRegisterDate;
    packet.XParse >> value.biDailyRewardDate;
    return packet;
}

// ============================================================
// World Event Register (世界事件注册) 相关结构体
// ============================================================

/**
 * @brief 世界事件注册请求 - 56 bytes
 * 来自 IDA: PS_DB_WORLD_EVENT_REGISTER_REQ
 */
struct PS_DB_WORLD_EVENT_REGISTER_REQ {
    std::uint32_t dwUCID = 0;                   // +0x00: 角色ID (4 bytes)
    std::int32_t nEventID = 0;                  // +0x04: 事件ID (4 bytes)
    std::int32_t nCount = 0;                    // +0x08: 计数 (4 bytes)
    std::uint8_t byInvenType = 0;               // +0x0C: 物品类型 (1 byte)
    std::uint8_t _pad0[3] = {};                 // +0x0D: padding (3 bytes)
    PS_RES_STORAGE_INFO stUpdateItem;           // +0x10: 更新物品列表 (40 bytes)
};

static_assert(sizeof(PS_DB_WORLD_EVENT_REGISTER_REQ) == 56, "PS_DB_WORLD_EVENT_REGISTER_REQ size must match IDA");

/**
 * @brief 世界事件注册响应 - 80 bytes
 * 来自 IDA: PS_DB_WORLD_EVENT_REGISTER_RES
 */
struct PS_DB_WORLD_EVENT_REGISTER_RES {
    std::int32_t nError = 0;                    // +0x00: 错误码 (4 bytes)
    std::uint8_t _pad0[4] = {};                 // +0x04: padding (4 bytes)
    PS_DB_WORLD_EVENT_REGISTER_REQ psReq;       // +0x08: 请求参数 (56 bytes)
    std::int32_t nTotalCount = 0;               // +0x40: 总计数 (4 bytes)
    std::int32_t nMyCount = 0;                  // +0x44: 我的计数 (4 bytes)
    std::int64_t biLastRegisterDate = 0;        // +0x48: 最后注册日期 (8 bytes)
};

static_assert(sizeof(PS_DB_WORLD_EVENT_REGISTER_RES) == 80, "PS_DB_WORLD_EVENT_REGISTER_RES size must match IDA");

// operator>> for PS_DB_WORLD_EVENT_REGISTER_REQ
inline XPacket& operator>>(XPacket& packet, PS_DB_WORLD_EVENT_REGISTER_REQ& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nEventID;
    packet.XParse >> value.nCount;
    packet.XParse >> value.byInvenType;
    // stUpdateItem: PS_RES_STORAGE_INFO
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.stUpdateItem.vecItem.clear();
    value.stUpdateItem.vecItem.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        PS_STORAGE_INFO info;
        packet.XParse >> info.byInvenType;
        packet.XParse >> info.shSlotPos;
        packet >> info.stItem;
        value.stUpdateItem.vecItem.push_back(info);
    }
    packet.XParse >> value.stUpdateItem.byType;
    return packet;
}

// operator<< for PS_DB_WORLD_EVENT_REGISTER_RES
inline XPacket& operator<<(XPacket& packet, const PS_DB_WORLD_EVENT_REGISTER_RES& value) {
    packet.XParse << value.nError;
    packet.XParse << value.psReq.dwUCID;
    packet.XParse << value.psReq.nEventID;
    packet.XParse << value.psReq.nCount;
    packet.XParse << value.psReq.byInvenType;
    packet.XParse << static_cast<std::int16_t>(value.psReq.stUpdateItem.vecItem.size());
    for (const auto& info : value.psReq.stUpdateItem.vecItem) {
        packet.XParse << info.byInvenType;
        packet.XParse << info.shSlotPos;
        packet << info.stItem;
    }
    packet.XParse << value.psReq.stUpdateItem.byType;
    packet.XParse << value.nTotalCount;
    packet.XParse << value.nMyCount;
    packet.XParse << value.biLastRegisterDate;
    return packet;
}

// ============================================================
// World Event Reward (世界事件奖励) 相关结构体
// ============================================================

/**
 * @brief 世界事件奖励请求 - 12 bytes
 * 来自 IDA: PS_WORLD_EVENT_REWARD_REQ
 */
struct PS_WORLD_EVENT_REWARD_REQ {
    std::int32_t nEventID = 0;          // +0x00: 事件ID (4 bytes)
    std::uint8_t byRewardType = 0;     // +0x04: 奖励类型 (1 byte)
    std::uint8_t _pad0[3] = {};        // +0x05: padding (3 bytes)
    std::int32_t nRewardIndex = 0;     // +0x08: 奖励索引 (4 bytes)
};

static_assert(sizeof(PS_WORLD_EVENT_REWARD_REQ) == 12, "PS_WORLD_EVENT_REWARD_REQ size must match IDA");

/**
 * @brief 世界事件奖励数据库请求 - 120 bytes
 * 来自 IDA: PS_DB_WORLD_EVENT_REWARD
 */
struct PS_DB_WORLD_EVENT_REWARD {
    std::uint32_t dwUAID = 0;                  // +0x00: 账号ID (4 bytes)
    std::uint32_t dwUCID = 0;                  // +0x04: 角色ID (4 bytes)
    PS_WORLD_EVENT_REWARD_REQ psReq;           // +0x08: 奖励请求参数 (12 bytes)
    PS_RES_STORAGE_INFO stCreateItem;          // +0x18: 创建物品列表 (40 bytes)
    PS_RES_STORAGE_INFO stUpdateItem;          // +0x40: 更新物品列表 (40 bytes)
    std::int32_t nError = 0;                   // +0x68: 错误码 (4 bytes)
    std::uint8_t byItemFlag = 0;               // +0x6C: 物品标志 (1 byte)
    std::uint8_t _pad0[3] = {};                // +0x6D: padding (3 bytes)
    std::uint32_t dwRewardItemID = 0;          // +0x70: 奖励物品ID (4 bytes)
    std::int16_t shRewardCount = 0;            // +0x74: 奖励数量 (2 bytes)
};

static_assert(sizeof(PS_DB_WORLD_EVENT_REWARD) == 120, "PS_DB_WORLD_EVENT_REWARD size must match IDA");

// operator>> for PS_DB_WORLD_EVENT_REWARD
inline XPacket& operator>>(XPacket& packet, PS_DB_WORLD_EVENT_REWARD& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.psReq.nEventID;
    packet.XParse >> value.psReq.byRewardType;
    packet.XParse >> value.psReq.nRewardIndex;
    // stCreateItem
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.stCreateItem.vecItem.clear();
    value.stCreateItem.vecItem.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        PS_STORAGE_INFO info;
        packet.XParse >> info.byInvenType;
        packet.XParse >> info.shSlotPos;
        packet >> info.stItem;
        value.stCreateItem.vecItem.push_back(info);
    }
    packet.XParse >> value.stCreateItem.byType;
    // stUpdateItem
    nCount = 0;
    packet.XParse >> nCount;
    value.stUpdateItem.vecItem.clear();
    value.stUpdateItem.vecItem.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        PS_STORAGE_INFO info;
        packet.XParse >> info.byInvenType;
        packet.XParse >> info.shSlotPos;
        packet >> info.stItem;
        value.stUpdateItem.vecItem.push_back(info);
    }
    packet.XParse >> value.stUpdateItem.byType;
    packet.XParse >> value.byItemFlag;
    packet.XParse >> value.dwRewardItemID;
    packet.XParse >> value.shRewardCount;
    return packet;
}

// operator<< for PS_DB_WORLD_EVENT_REWARD
inline XPacket& operator<<(XPacket& packet, const PS_DB_WORLD_EVENT_REWARD& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.psReq.nEventID;
    packet.XParse << value.psReq.byRewardType;
    packet.XParse << value.psReq.nRewardIndex;
    // stCreateItem
    packet.XParse << static_cast<std::int16_t>(value.stCreateItem.vecItem.size());
    for (const auto& info : value.stCreateItem.vecItem) {
        packet.XParse << info.byInvenType;
        packet.XParse << info.shSlotPos;
        packet << info.stItem;
    }
    packet.XParse << value.stCreateItem.byType;
    // stUpdateItem
    packet.XParse << static_cast<std::int16_t>(value.stUpdateItem.vecItem.size());
    for (const auto& info : value.stUpdateItem.vecItem) {
        packet.XParse << info.byInvenType;
        packet.XParse << info.shSlotPos;
        packet << info.stItem;
    }
    packet.XParse << value.stUpdateItem.byType;
    packet.XParse << value.nError;
    packet.XParse << value.byItemFlag;
    packet.XParse << value.dwRewardItemID;
    packet.XParse << value.shRewardCount;
    return packet;
}

// ============================================================
// World Event Daily Reward (世界事件每日奖励) 相关结构体
// ============================================================

/**
 * @brief 世界事件每日奖励数据库请求 - 104 bytes
 * 来自 IDA: PS_DB_WORLD_EVENT_DAILY_REWARD
 */
struct PS_DB_WORLD_EVENT_DAILY_REWARD {
    std::uint32_t dwUCID = 0;                  // +0x00: 角色ID (4 bytes)
    std::int32_t nEventID = 0;                 // +0x04: 事件ID (4 bytes)
    std::int64_t biDailyRewardDate = 0;        // +0x08: 每日奖励日期 (8 bytes)
    PS_RES_STORAGE_INFO stCreateItem;          // +0x10: 创建物品列表 (40 bytes)
    PS_RES_STORAGE_INFO stUpdateItem;          // +0x38: 更新物品列表 (40 bytes)
    std::int32_t nError = 0;                   // +0x60: 错误码 (4 bytes)
    std::uint8_t byItemFlag = 0;               // +0x64: 物品标志 (1 byte)
};

static_assert(sizeof(PS_DB_WORLD_EVENT_DAILY_REWARD) == 104, "PS_DB_WORLD_EVENT_DAILY_REWARD size must match IDA");

// operator>> for PS_DB_WORLD_EVENT_DAILY_REWARD
inline XPacket& operator>>(XPacket& packet, PS_DB_WORLD_EVENT_DAILY_REWARD& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nEventID;
    packet.XParse >> value.biDailyRewardDate;
    // stCreateItem
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.stCreateItem.vecItem.clear();
    value.stCreateItem.vecItem.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        PS_STORAGE_INFO info;
        packet.XParse >> info.byInvenType;
        packet.XParse >> info.shSlotPos;
        packet >> info.stItem;
        value.stCreateItem.vecItem.push_back(info);
    }
    packet.XParse >> value.stCreateItem.byType;
    // stUpdateItem
    nCount = 0;
    packet.XParse >> nCount;
    value.stUpdateItem.vecItem.clear();
    value.stUpdateItem.vecItem.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        PS_STORAGE_INFO info;
        packet.XParse >> info.byInvenType;
        packet.XParse >> info.shSlotPos;
        packet >> info.stItem;
        value.stUpdateItem.vecItem.push_back(info);
    }
    packet.XParse >> value.stUpdateItem.byType;
    packet.XParse >> value.byItemFlag;
    return packet;
}

// operator<< for PS_DB_WORLD_EVENT_DAILY_REWARD
inline XPacket& operator<<(XPacket& packet, const PS_DB_WORLD_EVENT_DAILY_REWARD& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nEventID;
    packet.XParse << value.biDailyRewardDate;
    // stCreateItem
    packet.XParse << static_cast<std::int16_t>(value.stCreateItem.vecItem.size());
    for (const auto& info : value.stCreateItem.vecItem) {
        packet.XParse << info.byInvenType;
        packet.XParse << info.shSlotPos;
        packet << info.stItem;
    }
    packet.XParse << value.stCreateItem.byType;
    // stUpdateItem
    packet.XParse << static_cast<std::int16_t>(value.stUpdateItem.vecItem.size());
    for (const auto& info : value.stUpdateItem.vecItem) {
        packet.XParse << info.byInvenType;
        packet.XParse << info.shSlotPos;
        packet << info.stItem;
    }
    packet.XParse << value.stUpdateItem.byType;
    packet.XParse << value.nError;
    packet.XParse << value.byItemFlag;
    return packet;
}

// ============================================================
// Roulette Event (轮盘事件) 相关结构体
// ============================================================

/**
 * @brief 轮盘信息 - 16 bytes
 * 来自 IDA: PS_ROULETTE_INFO
 */
struct PS_ROULETTE_INFO {
    std::int32_t nDayCount = 0;          // +0x00: 天数计数 (4 bytes)
    std::uint8_t _pad0[4] = {};          // +0x04: padding (4 bytes)
    std::int64_t biRegDate = 0;          // +0x08: 注册日期 (8 bytes)
};

static_assert(sizeof(PS_ROULETTE_INFO) == 16, "PS_ROULETTE_INFO size must match IDA");

/**
 * @brief 轮盘事件信息数据库请求 - 32 bytes
 * 来自 IDA: PS_DB_ROULETTE_EVENT_INFO
 */
struct PS_DB_ROULETTE_EVENT_INFO {
    std::uint32_t dwUCID = 0;            // +0x00: 角色ID (4 bytes)
    std::uint32_t dwUAID = 0;            // +0x04: 账号ID (4 bytes)
    std::int32_t nEventID = 0;           // +0x08: 事件ID (4 bytes)
    std::int32_t nErrorCode = 0;         // +0x0C: 错误码 (4 bytes)
    PS_ROULETTE_INFO psRoulettInfo;      // +0x10: 轮盘信息 (16 bytes)
};

static_assert(sizeof(PS_DB_ROULETTE_EVENT_INFO) == 32, "PS_DB_ROULETTE_EVENT_INFO size must match IDA");

// operator>> for PS_DB_ROULETTE_EVENT_INFO
inline XPacket& operator>>(XPacket& packet, PS_DB_ROULETTE_EVENT_INFO& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.nEventID;
    return packet;
}

// operator<< for PS_DB_ROULETTE_EVENT_INFO
inline XPacket& operator<<(XPacket& packet, const PS_DB_ROULETTE_EVENT_INFO& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.nEventID;
    packet.XParse << value.nErrorCode;
    packet.XParse << value.psRoulettInfo.nDayCount;
    packet.XParse << value.psRoulettInfo.biRegDate;
    return packet;
}

/**
 * @brief 轮盘事件更新请求 - 112 bytes
 * 来自 IDA: PS_DB_ROULETTE_EVENT_UPDATE
 */
struct PS_DB_ROULETTE_EVENT_UPDATE {
    PS_RES_STORAGE_INFO psUpdateItemList;  // +0x00: 更新物品列表 (40 bytes)
    ST_CREATE_ITEMS stCreateItems;         // +0x28: 创建物品列表 (32 bytes)
    std::uint32_t dwUCID = 0;              // +0x48: 角色ID (4 bytes)
    std::uint32_t dwUAID = 0;              // +0x4C: 账号ID (4 bytes)
    std::uint8_t byUseType = 0;            // +0x50: 使用类型 (1 byte)
    std::uint8_t _pad0[3] = {};            // +0x51: padding (3 bytes)
    std::int32_t nEventID = 0;             // +0x54: 事件ID (4 bytes)
    std::int32_t nRewardID = 0;            // +0x58: 奖励ID (4 bytes)
    std::int32_t nDayCount = 0;            // +0x5C: 天数计数 (4 bytes)
    std::int32_t nErrorCode = 0;           // +0x60: 错误码 (4 bytes)
    std::int32_t nRemainCount = 0;         // +0x64: 剩余计数 (4 bytes)
    std::int64_t biUpdateDate = 0;         // +0x68: 更新日期 (8 bytes)
};

static_assert(sizeof(PS_DB_ROULETTE_EVENT_UPDATE) == 112, "PS_DB_ROULETTE_EVENT_UPDATE size must match IDA");

// operator>> for PS_DB_ROULETTE_EVENT_UPDATE
inline XPacket& operator>>(XPacket& packet, PS_DB_ROULETTE_EVENT_UPDATE& value) {
    // psUpdateItemList
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.psUpdateItemList.vecItem.clear();
    value.psUpdateItemList.vecItem.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        PS_STORAGE_INFO info;
        packet.XParse >> info.byInvenType;
        packet.XParse >> info.shSlotPos;
        packet >> info.stItem;
        value.psUpdateItemList.vecItem.push_back(info);
    }
    packet.XParse >> value.psUpdateItemList.byType;
    // stCreateItems
    nCount = 0;
    packet.XParse >> nCount;
    value.stCreateItems.vecInfo.clear();
    value.stCreateItems.vecInfo.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        ST_CREATE_ITEM item;
        packet >> item;
        value.stCreateItems.vecInfo.push_back(item);
    }
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.byUseType;
    packet.XParse >> value.nEventID;
    packet.XParse >> value.nRewardID;
    packet.XParse >> value.nDayCount;
    return packet;
}

// operator<< for PS_DB_ROULETTE_EVENT_UPDATE
inline XPacket& operator<<(XPacket& packet, const PS_DB_ROULETTE_EVENT_UPDATE& value) {
    // psUpdateItemList
    packet.XParse << static_cast<std::int16_t>(value.psUpdateItemList.vecItem.size());
    for (const auto& info : value.psUpdateItemList.vecItem) {
        packet.XParse << info.byInvenType;
        packet.XParse << info.shSlotPos;
        packet << info.stItem;
    }
    packet.XParse << value.psUpdateItemList.byType;
    // stCreateItems
    packet.XParse << static_cast<std::int16_t>(value.stCreateItems.vecInfo.size());
    for (const auto& item : value.stCreateItems.vecInfo) {
        packet << item;
    }
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

/**
 * 来自 IDA 0x140027630: PS_DB_CHECK_LOCATION - 角色位置检查
 * 用于 SP_CHARACTER_LOCATION_LOAD 存储过程
 * 总大小约 112 bytes (0x70)
 */
struct PS_DB_CHECK_LOCATION {
    std::uint32_t dwUAID = 0;               // +0x00: 账号ID (4 bytes)
    std::uint32_t dwUCID = 0;               // +0x04: 角色ID (4 bytes)
    char szAccountID[21] = {};              // +0x08: 账号字符串 (21 bytes)
    wchar_t szName[21] = {};                // +0x20: 角色名 (42 bytes)
    char szIP[21] = {};                     // +0x4A: IP地址 (21 bytes) - 注意: 0x20 + 42/2*2 = 0x4A左右
    // 输出字段
    std::uint32_t uxMapID = 0;              // +0x60: 地图ID (4 bytes)
    std::int32_t nServerID = 0;             // +0x64: 服务器ID (4 bytes)
    std::uint8_t byChannelID[16] = {};      // +0x68: 频道ID (16 bytes)
    std::uint16_t wMapID = 0;               // +0x78: 地图ID (2 bytes)
};

inline void operator>>(XPacket& packet, PS_DB_CHECK_LOCATION& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    short outLen = 0;
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse.GetWString(value.szName, 21, outLen);
    packet.XParse.GetString(value.szIP, 21, &outLen);
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_CHECK_LOCATION& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << std::string(value.szAccountID);
    packet.XParse << std::wstring(value.szName);
    packet.XParse << std::string(value.szIP);
    packet.XParse << value.uxMapID;
    packet.XParse << value.nServerID;
    packet.XParse.GetBytes(reinterpret_cast<char*>(const_cast<std::uint8_t*>(value.byChannelID)), 16);
    packet.XParse << value.wMapID;
    return packet;
}

/**
 * 来自 IDA 0x1400B8BF0: PS_NPC_CREDIT - NPC信用度信息
 * 用于 SP_NPC_CREDIT_SELECT 存储过程
 */
struct PS_NPC_CREDIT {
    std::int32_t nGroupID = 0;      // +0x00: 组ID (4 bytes)
    std::uint8_t byGrade = 0;       // +0x04: 等级 (1 byte)
    std::uint8_t _pad0[3] = {};     // +0x05: padding (3 bytes)
    std::int32_t nPoint = 0;        // +0x08: 点数 (4 bytes)
    std::int16_t shCurPoint = 0;    // +0x0C: 当前点数 (2 bytes)
    std::uint8_t _pad1[2] = {};     // +0x0E: padding (2 bytes)
    std::int32_t nUpdateDate = 0;   // +0x10: 更新日期 (4 bytes)
};

static_assert(sizeof(PS_NPC_CREDIT) == 20, "PS_NPC_CREDIT size mismatch");

/**
 * 来自 IDA: PS_NPC_CREDIT_LIST - NPC信用度列表
 */
struct PS_NPC_CREDIT_LIST {
    std::vector<PS_NPC_CREDIT> vecList;  // +0x00: 信用度列表
};

inline XPacket& operator<<(XPacket& packet, const PS_NPC_CREDIT& value) {
    packet.XParse << value.nGroupID;
    packet.XParse << value.byGrade;
    packet.XParse << value.nPoint;
    packet.XParse << value.shCurPoint;
    packet.XParse << value.nUpdateDate;
    return packet;
}

inline void operator>>(XPacket& packet, PS_NPC_CREDIT& value) {
    packet.XParse >> value.nGroupID;
    packet.XParse >> value.byGrade;
    packet.XParse >> value.nPoint;
    packet.XParse >> value.shCurPoint;
    packet.XParse >> value.nUpdateDate;
}

inline XPacket& operator<<(XPacket& packet, const PS_NPC_CREDIT_LIST& value) {
    packet.XParse << static_cast<std::int16_t>(value.vecList.size());
    for (const auto& item : value.vecList) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_NPC_CREDIT_LIST& value) {
    std::int16_t nCount = 0;
    packet.XParse >> nCount;
    value.vecList.clear();
    value.vecList.reserve(static_cast<std::size_t>(nCount));
    for (std::int16_t i = 0; i < nCount; ++i) {
        PS_NPC_CREDIT item;
        packet >> item;
        value.vecList.push_back(item);
    }
}

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

// operator>> 反序列化
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

// operator>> 反序列化
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

// ============================================================
// 每日任务相关结构体 (对齐 IDA XSQLDailyMissionProcess)
// ============================================================

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
};

/**
 * 对齐 IDA: PS_MAP_DAILY_MISSION - 每日任务 MAP 结构
 */
struct PS_MAP_DAILY_MISSION {
    std::map<std::uint32_t, ST_DAILY_MISSION_INFO> mapInfo;
};

/**
 * 对齐 IDA: PS_DAILY_MISSION_UPDATE - 每日任务更新
 */
struct PS_DAILY_MISSION_UPDATE {
    std::vector<ST_DAILY_MISSION_INFO> vecInfo;
};

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

// ============================================================
// 每周任务相关结构体 (对齐 IDA XSQLWeeklyMissionProcess)
// ============================================================

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

// 注意：ST_STATISTICS_ITEM 在 PSCommon.h 第 1109 行已定义，此处仅添加 operator>>
// 注意：ST_STATISTICS_MAP_SAVE 在本文件第 1841 行已定义，此处仅添加 operator>>

// operator>> 反序列化
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

// ST_STATISTICS_ITEM operator>> moved to PSCommon.h (struct defined there)

inline void operator>>(XPacket& packet, ST_STATISTICS_MAP_SAVE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwMapID;
    packet.XParse >> value.dwServerID;
}

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

// operator>> for ST_CLASS_EVENT_INFO
inline XPacket& operator>>(XPacket& packet, ST_CLASS_EVENT_INFO& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byType;
    packet.XParse >> value.nValue;
    packet.XParse >> value.nReward;
    packet.XParse >> value.nLastUpdateDate;
    return packet;
}

// operator>> for PS_MODE_MAZE_EVENT_REWARD_INFO
inline XPacket& operator>>(XPacket& packet, PS_MODE_MAZE_EVENT_REWARD_INFO& value) {
    packet.XParse >> value.nEventID;
    for (int i = 0; i < 8; ++i) {
        packet.XParse >> value.arrInfo[i].dwUCID;
        packet.XParse >> value.arrInfo[i].nPoint;
    }
    return packet;
}

// operator<< for ST_CLASS_EVENT_LIST
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

// operator>> for PS_PLAY_TIME_FOR_DAY
inline XPacket& operator>>(XPacket& packet, PS_PLAY_TIME_FOR_DAY& value) {
    std::int16_t shLen = 0;
    packet.XParse.GetString(value.szAccountID, 21, &shLen);
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nSec;
    return packet;
}
