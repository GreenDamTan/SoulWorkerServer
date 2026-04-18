#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <set>
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

struct ST_ITEM_PACKAGE_PARTS {
    std::int64_t biSerial = 0;
    int nItemID = 0;
    int nDyeID = 0;
};

struct PS_ITEM_PACKAGE {
    std::int64_t biPackageSerial = 0;
    std::vector<ST_ITEM_PACKAGE_PARTS> vecInfo;
};

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

