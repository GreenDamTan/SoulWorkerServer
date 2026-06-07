#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerLeague.h - 联赛(League)相关结构体及序列化运算符
//
// 此文件包含联赛系统的所有数据结构和序列化操作：
// - 联赛成员信息
// - 联赛邀请/创建/搜索
// - 联赛仓库物品操作
// - 联赛技能/权限/公告
// ============================================================================

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

// Auto skill structure - 对齐 IDA PS_AUTO_SKILL (8 bytes)
struct PS_AUTO_SKILL {
    std::uint8_t bySkillInfo[8] = {};  // 8 个技能组等级
};

// League delegate response (24 bytes)
struct PS_RES_LEAGUE_DELEGATE {
    std::int32_t nLeagueID = 0;
    wchar_t szDelegatedName[21] = {};  // 被转让者名称
    wchar_t szDelegateName[21] = {};   // 原会长名称
    std::int32_t nResult = 0;
};

// League change name response (50 bytes)
struct PS_RES_LEAGUE_NAME_CHANGE {
    std::int32_t nLeagueID = 0;
    std::uint32_t dwUCID = 0;
    wchar_t szOldName[10] = {};   // 原公会名
    wchar_t szNewName[10] = {};   // 新公会名
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

// League info update - 对齐 IDA ST_LEAGUE_INFO_UPDATE (32 bytes)
// 注意: ST_LEAGUE_INFO_UPDATE 已在 LeagueManager.h 中定义，此声明仅用于引用
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

// GMT league update info (单个公会信息请求)
struct PS_GMT_LEAGUE_UPDATE_INFO {
    std::int32_t nLeagueId = 0;
    std::uint32_t dwUCID = 0;
};

// GMT league update list (GM工具批量查询公会信息)
struct PS_GMT_LEAGUE_UPDATE_LIST {
    std::int32_t nCount = 0;
    std::vector<PS_GMT_LEAGUE_UPDATE_INFO> vecInfo;
};

// ============================================================================
// 联赛列表结构（从 LeagueManager.h 迁移，供序列化运算符使用）
// ============================================================================

// ST_LEAGUE_LIST 定义在 LeagueManager.h 中（包含 ST_LEAGUE_INFO）

// 联赛成员列表
struct ST_LEAGUE_MEMBER_LIST {
    std::int32_t nCount = 0;
    std::vector<ST_LEAGUE_MEMBER_EX> vecInfo;
};

// 联赛公告板列表
struct ST_LEAGUE_BOARD_LIST {
    std::int32_t nCount = 0;
    std::vector<ST_LEAGUE_BOARD> vecInfo;
};

// 联赛申请人列表
struct ST_LEAGUE_APPLICANT_LIST {
    std::int32_t nCount = 0;
    std::vector<ST_LEAGUE_APPLICANT> vecInfo;
};

// 联赛记录列表
struct ST_LEAGUE_RECORD_LIST {
    std::int32_t nCount = 0;
    std::vector<ST_LEAGUE_RECORD> vecInfo;
};

// 联赛申请人检查列表
struct ST_LEAGUE_APPLICANT_CHECK_LIST {
    std::int32_t nCount = 0;
    std::vector<std::uint32_t> vecInfo;
};

// 对齐 IDA 命名约定 - 联赛摘要信息
struct PS_LEAGUE_INFO_SUMMARY {
    std::int32_t nLeagueID = 0;
    std::int32_t nMemberCount = 0;
    std::uint8_t byRating = 0;
    std::uint8_t _pad0[3] = {};
    std::uint32_t dwLeagueCard = 0;     // union ___u3
    wchar_t szLeagueName[10] = {};      // szName (20 bytes)
    wchar_t szMaster[21] = {};          // szMaster (42 bytes)
    wchar_t szSubMaster[21] = {};       // szSubMaster (42 bytes)
    wchar_t szRecruit[51] = {};         // szRecruit (102 bytes)
    // Legacy fields for compatibility
    std::uint32_t dwMasterUCID = 0;
    std::int32_t nLeagueRank = 0;
    std::int32_t nApplicantCount = 0;
    bool bOpen = false;
};

// 联赛摘要列表
struct PS_LEAGUE_SUMMARY_LIST {
    std::int32_t nCount = 0;
    std::vector<PS_LEAGUE_INFO_SUMMARY> vecInfo;
};

// 服务器角色改名信息（包含联赛相关数据）
struct PS_SERVER_CHANGE_CHARACTER_NAME {
    PS_CHANGE_NAME psChangeInfo{};
    ST_PARTY_INFO stPartyInfo{};
    std::int32_t nLeagueID = 0;
    ST_LEAGUE_APPLICANT_CHECK_LIST stApplyList{};
};

// ============================================================================
// League Serialization Operators
// ============================================================================

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

// PS_RES_LEAGUE_SKILL 序列化
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

// PS_LEAGUE_INVENTORY_FOR_LOG 序列化
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

// PS_DB_LEAGUE_LOAD 序列化
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

inline void operator>>(XPacket& packet, PS_GMT_LEAGUE_UPDATE_INFO& value) {
    packet.XParse >> value.nLeagueId;
    packet.XParse >> value.dwUCID;
}

inline void operator>>(XPacket& packet, PS_GMT_LEAGUE_UPDATE_LIST& value) {
    packet.XParse >> value.nCount;
    value.vecInfo.resize(value.nCount);
    for (PS_GMT_LEAGUE_UPDATE_INFO& info : value.vecInfo) {
        packet >> info;
    }
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

// ============================================================================
// 联赛卡片请求序列化运算符（结构体定义在第 291 行）
// ============================================================================

// PS_REQ_LEAGUE_CARD 序列化运算符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_REQ_LEAGUE_CARD& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.shSlot;
    packet.XParse << value.dwLeagueCard;
    packet.XParse << value.nResult;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_REQ_LEAGUE_CARD& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.shSlot;
    packet.XParse << value.dwLeagueCard;
    packet.XParse << value.nResult;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_LEAGUE_CARD& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.shSlot;
    packet.XParse >> value.dwLeagueCard;
    packet.XParse >> value.nResult;
}

// ============================================================================
// 联赛仓库信息请求序列化运算符（结构体定义在第 410 行）
// ============================================================================

// PS_REQ_LEAGUE_INVEN_INFO 序列化运算符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_REQ_LEAGUE_INVEN_INFO& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwNpcID;
    packet.XParse << value.shStartPos;
    packet.XParse << value.shEndPos;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_REQ_LEAGUE_INVEN_INFO& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwNpcID;
    packet.XParse << value.shStartPos;
    packet.XParse << value.shEndPos;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_LEAGUE_INVEN_INFO& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwNpcID;
    packet.XParse >> value.shStartPos;
    packet.XParse >> value.shEndPos;
}

// ============================================================================
// 联赛财富/同步/改名请求序列化运算符
// ============================================================================

// PS_LEAGUE_WEALTH_FOR_SERVER 序列化运算符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_LEAGUE_WEALTH_FOR_SERVER& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nLeagueID;
    packet.XParse << value.nTotalExp;
    packet.XParse << value.shExp;
    packet.XParse << value.nGold;
    packet.XParse << value.biPrevExp;
    packet.XParse << value.nErrorCode;
    return packet;
}

// PS_LEAGUE_WEALTH_FOR_SERVER XPacket 序列化运算符
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

// PS_LEAGUE_WEALTH_FOR_SERVER 反序列化运算符
inline void operator>>(XPacket& packet, PS_LEAGUE_WEALTH_FOR_SERVER& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.nTotalExp;
    packet.XParse >> value.shExp;
    packet.XParse >> value.nGold;
    packet.XParse >> value.biPrevExp;
    packet.XParse >> value.nErrorCode;
}

// PS_SYNC_LEAGUE_INFO 序列化运算符
inline XSendPacket& operator<<(XSendPacket& packet, const PS_SYNC_LEAGUE_INFO& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.nSyncCount;
    packet.XParse << value.bSync;
    return packet;
}

// PS_SYNC_LEAGUE_INFO XPacket 序列化运算符
inline XPacket& operator<<(XPacket& packet, const PS_SYNC_LEAGUE_INFO& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.nSyncCount;
    packet.XParse << value.bSync;
    return packet;
}

// PS_SYNC_LEAGUE_INFO 反序列化运算符
inline void operator>>(XPacket& packet, PS_SYNC_LEAGUE_INFO& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nSyncCount;
    packet.XParse >> value.bSync;
}

// ============================================================================
// DBAgent 需要的联赛反序列化运算符（输出运算符定义在 LeagueManager.h 中）
// ============================================================================

// ST_LEAGUE_BOARD 反序列化运算符 (用于 DBAgent)
inline void operator>>(XPacket& packet, ST_LEAGUE_BOARD& value) {
    packet.XParse >> value.nSerial;
    packet.XParse >> value.nLeagueID;
    short outLen = 0;
    packet.XParse.GetWString(value.szCharName, 21, outLen);
    packet.XParse.GetWString(value.szMsg, 61, outLen);
    packet.XParse >> value.biEnrollDate;
    packet.XParse >> value.nResult;
}

// ST_LEAGUE_APPLICANT 反序列化运算符 (用于 DBAgent)
inline void operator>>(XPacket& packet, ST_LEAGUE_APPLICANT& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwActorID;
    short outLen = 0;
    packet.XParse.GetWString(value.szName, 21, outLen);
    packet.XParse >> value.shLevel;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.biApplicantDate;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad1), sizeof(value._pad1));
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.nResult;
}

// ============================================================================
// 联赛结构体输入反序列化运算符（从 LeagueManager.h 迁移，供 DBAgent/RelayServer 使用）
// ============================================================================

// ST_LEAGUE_MEMBER 输入反序列化
inline void operator>>(XPacket& packet, ST_LEAGUE_MEMBER& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.byPosition;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.biLeagueExp;
    packet.XParse >> value.biJoinDate;
    packet.XParse >> value.biApplicationDate;
}

// ST_LEAGUE_MEMBER_EX 输入反序列化
inline void operator>>(XPacket& packet, ST_LEAGUE_MEMBER_EX& value) {
    packet >> value.stMember;
    packet.XParse >> value.bLogin;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&value._pad1), sizeof(value._pad1));
    packet.XParse >> value.dwUCID;
    short outLen = 0;
    packet.XParse.GetWString(value.szName, 21, outLen);
    packet.XParse >> value.shLevel;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&value._pad2), sizeof(value._pad2));
    packet.XParse >> value.biBoardLimitTime;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&value._pad3), sizeof(value._pad3));
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&value._pad4), sizeof(value._pad4));
    packet.XParse >> value.biPlayDate;
}

// ST_REQ_LEAGUE_APPLICANT_ACCEPT 输入反序列化
inline void operator>>(XPacket& packet, ST_REQ_LEAGUE_APPLICANT_ACCEPT& value) {
    packet.XParse >> value.dwReqActorID;
    short outLen = 0;
    packet.XParse.GetWString(value.szReqName, 21, outLen);
    packet.XParse >> value.nLeagueID;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&value._pad0), sizeof(value._pad0));
    packet.XParse >> value.biJoinDate;
    packet.XParse >> value.nResult;
}

// ST_REQ_LEAGUE_APPLICANT_REJECT 输入反序列化
inline void operator>>(XPacket& packet, ST_REQ_LEAGUE_APPLICANT_REJECT& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwTargetUCID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nResult;
}

// ST_LEAGUE_NOTICE 输入反序列化
inline void operator>>(XPacket& packet, ST_LEAGUE_NOTICE& value) {
    packet.XParse >> value.nLeagueID;
    short outLen = 0;
    packet.XParse.GetWString(value.szNotice, 801, outLen);
    packet.XParse >> value.biEnrollDate;
    packet.XParse >> value.nResult;
}

// PS_LEAGUE_NAME_CHANGE_SERVER 输入反序列化
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

// ST_LEAGUE_POSITION_NAME_CHANGE 输入反序列化
inline void operator>>(XPacket& packet, ST_LEAGUE_POSITION_NAME_CHANGE& value) {
    packet.XParse >> value.nPosition;
    short outLen = 0;
    packet.XParse.GetWString(value.szLeagueName, 11, outLen);
    packet.XParse >> value.nResult;
    packet.XParse.GetWString(value.szPrevPositionName, 11, outLen);
}

// ST_LEAGUE_AUTH_CHANGE 输入反序列化
inline void operator>>(XPacket& packet, ST_LEAGUE_AUTH_CHANGE& value) {
    for (int i = 0; i < 9; ++i) packet.XParse >> value.nAuth[i];
    for (int i = 0; i < 9; ++i) packet.XParse >> value.nLimitGoldOut[i];
    packet.XParse >> value.nResult;
}

// ST_LEAGUE_MEMBER_POSITION 输入反序列化
inline void operator>>(XPacket& packet, ST_LEAGUE_MEMBER_POSITION& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.byPosition;
    packet.XParse >> value.byState;
    packet.XParse >> value.nResult;
}

// ST_LEAGUE_OPEN 输入反序列化
inline void operator>>(XPacket& packet, ST_LEAGUE_OPEN& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.bOpen;
}

// ST_LEAGUE_RECRUIT_NOTICE 输入反序列化
inline void operator>>(XPacket& packet, ST_LEAGUE_RECRUIT_NOTICE& value) {
    packet.XParse >> value.nLeagueID;
    short outLen = 0;
    packet.XParse.GetWString(value.szNotice, 51, outLen);
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.biRegDate;
    packet.XParse >> value.nResult;
}

// PS_REQ_LEAGUE_DELEGATE 输入反序列化
inline void operator>>(XPacket& packet, PS_REQ_LEAGUE_DELEGATE& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwNpcID;
    packet.XParse >> value.dwDelegatedUCID;
}

// ST_REQ_LEAGUE_INVITE_REJECT 输入反序列化
inline void operator>>(XPacket& packet, ST_REQ_LEAGUE_INVITE_REJECT& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.szTargetName, 21, outLen);
    packet.XParse >> value.dwReqUCID;
    packet.XParse >> value.dwTargetUCID;
    packet.XParse >> value.nResult;
}

// ST_REQ_LEAGUE_SEARCH 输入反序列化
inline void operator>>(XPacket& packet, ST_REQ_LEAGUE_SEARCH& value) {
    packet.XParse >> value.nState;
    short outLen = 0;
    packet.XParse.GetWString(value.szLeagueName, 10, outLen);
    packet.XParse.GetWString(value.szMasterName, 21, outLen);
}

// PS_CHAT_LEAGUE 输入反序列化
inline void operator>>(XPacket& packet, PS_CHAT_LEAGUE& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwLeagueID;
    packet.XParse >> value.dwMemberID;
    short outLen = 0;
    packet.XParse.GetWString(value.szMsg, 256, outLen);
}

// ST_LEAGUE_RECORD 输入反序列化
inline void operator>>(XPacket& packet, ST_LEAGUE_RECORD& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.byFlag;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&value._pad0), sizeof(value._pad0));
    packet.XParse >> value.biRegisterDate;
    short outLen = 0;
    packet.XParse.GetWString(value.szValue1, 21, outLen);
    packet.XParse.GetWString(value.szValue2, 21, outLen);
    packet.XParse >> value.nValue3;
    packet.XParse >> value.nValue4;
}

// PS_REQ_LEAGUE_SKILL 输入反序列化
inline void operator>>(XPacket& packet, PS_REQ_LEAGUE_SKILL& value) {
    packet.XParse >> value.nLeagueID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.bySkillIndex;
    packet.XParse >> value.byType;
}

// PS_LEAGUE_NAME_CHANGE_SERVER XPacket 序列化运算符 (用于 DBAgent)
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

// ============================================================================
// 联赛输出序列化运算符（从 LeagueManager.h 迁移，确保通过 PSServer.h 可见）
// ============================================================================

// ST_LEAGUE_MEMBER 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_MEMBER& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.byPosition;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.biLeagueExp;
    packet.XParse << value.biJoinDate;
    packet.XParse << value.biApplicationDate;
    return packet;
}

// ST_LEAGUE_MEMBER_EX 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_MEMBER_EX& value) {
    packet << value.stMember;
    packet.XParse << value.bLogin;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad1), sizeof(value._pad1));
    packet.XParse << value.dwUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szName);
    packet.XParse << value.shLevel;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad2), sizeof(value._pad2));
    packet.XParse << value.biBoardLimitTime;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad3), sizeof(value._pad3));
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad4), sizeof(value._pad4));
    packet.XParse << value.biPlayDate;
    return packet;
}

// ST_LEAGUE_BOARD 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_BOARD& value) {
    packet.XParse << value.nSerial;
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szCharName);
    packet.XParse << GreenDamTan_BoundedWideString(value.szMsg);
    packet.XParse << value.biEnrollDate;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_APPLICANT 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_APPLICANT& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwActorID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szName);
    packet.XParse << value.shLevel;
    packet.XParse << value.biApplicantDate;
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_RECORD 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_RECORD& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.byFlag;
    packet.XParse << value.biRegisterDate;
    packet.XParse << GreenDamTan_BoundedWideString(value.szValue1);
    packet.XParse << GreenDamTan_BoundedWideString(value.szValue2);
    packet.XParse << value.nValue3;
    packet.XParse << value.nValue4;
    return packet;
}

// ST_LEAGUE_INFO_FOR_GAME 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_INFO_FOR_GAME& value) {
    packet.XParse << value.dwMasterUCID;
    packet.XParse << value.byLeagueLevel;
    packet.XParse << value.byPosition;
    for (int i = 0; i < 8; ++i) packet.XParse << value.bySkillInfo[i];
    for (int i = 0; i < 9; ++i) packet.XParse << value.nAuth[i];
    return packet;
}

// ST_LEAGUE_MEMBER_UPDATE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_MEMBER_UPDATE& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwActorID;
    packet.XParse << value.bLogin;
    packet.XParse << value.byLevel;
    packet.XParse << value.sWorld;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.biPlayDate;
    packet.XParse << GreenDamTan_BoundedWideString(value.szName);
    packet.XParse << value.byChannel;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    return packet;
}

// ST_LEAGUE_INFO_EX 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_INFO_EX& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << value.nMemberCount;
    return packet;
}

// ST_REQ_LEAGUE_INVITE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_INVITE& value) {
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << GreenDamTan_BoundedWideString(value.szTargetName);
    packet.XParse << GreenDamTan_BoundedWideString(value.szReqName);
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwTargetActorID;
    packet.XParse << value.nLeagueID;
    packet.XParse << value.nResult;
    return packet;
}

// ST_REQ_LEAGUE_INVITE_ACCEPT 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_INVITE_ACCEPT& value) {
    packet.XParse << value.dwReqUCID;
    packet.XParse << value.dwTargetUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szTargetName);
    packet.XParse << value.nLeagueID;
    packet.XParse << value.nResult;
    return packet;
}

// ST_REQ_LEAGUE_INVITE_REJECT 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_INVITE_REJECT& value) {
    packet.XParse << GreenDamTan_BoundedWideString(value.szTargetName);
    packet.XParse << value.dwReqUCID;
    packet.XParse << value.dwTargetUCID;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_NOTICE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_NOTICE& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szNotice);
    packet.XParse << value.biEnrollDate;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_AUTH_CHANGE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_AUTH_CHANGE& value) {
    for (int i = 0; i < 9; ++i) packet.XParse << value.nAuth[i];
    for (int i = 0; i < 9; ++i) packet.XParse << value.nLimitGoldOut[i];
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_OPEN 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_OPEN& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.bOpen;
    return packet;
}

// ST_LEAGUE_RECRUIT_NOTICE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_RECRUIT_NOTICE& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szNotice);
    packet.XParse << value.biRegDate;
    packet.XParse << value.nResult;
    return packet;
}

// ST_REQ_LEAGUE_APPLICANT_ACCEPT 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_APPLICANT_ACCEPT& value) {
    packet.XParse << value.dwReqActorID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szReqName);
    packet.XParse << value.nLeagueID;
    packet.XParse << value.biJoinDate;
    packet.XParse << value.nResult;
    return packet;
}

// ST_REQ_LEAGUE_APPLICANT_REJECT 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_APPLICANT_REJECT& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwTargetUCID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_MEMBER_POSITION 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_MEMBER_POSITION& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.byPosition;
    packet.XParse << value.byState;
    packet.XParse << value.nResult;
    return packet;
}

// ST_LEAGUE_POSITION_NAME_CHANGE 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_POSITION_NAME_CHANGE& value) {
    packet.XParse << value.nPosition;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << value.nResult;
    packet.XParse << GreenDamTan_BoundedWideString(value.szPrevPositionName);
    return packet;
}

// PS_REQ_LEAGUE_DELEGATE 输出序列化
inline XPacket& operator<<(XPacket& packet, const PS_REQ_LEAGUE_DELEGATE& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << value.dwNpcID;
    packet.XParse << value.dwDelegatedUCID;
    return packet;
}

// PS_RES_LEAGUE_DELEGATE 输出序列化
inline XPacket& operator<<(XPacket& packet, const PS_RES_LEAGUE_DELEGATE& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szDelegatedName);
    packet.XParse << GreenDamTan_BoundedWideString(value.szDelegateName);
    packet.XParse << value.nResult;
    return packet;
}

// PS_CHAT_LEAGUE 输出序列化
inline XPacket& operator<<(XPacket& packet, const PS_CHAT_LEAGUE& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwLeagueID;
    packet.XParse << value.dwMemberID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szMsg);
    return packet;
}

// ST_REQ_LEAGUE_SEARCH 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_REQ_LEAGUE_SEARCH& value) {
    packet.XParse << value.nState;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << GreenDamTan_BoundedWideString(value.szMasterName);
    return packet;
}

// ============================================================================
// 联赛列表结构输出序列化运算符（从 LeagueManager.h 迁移）
// ============================================================================

// vector 输出序列化
inline XPacket& operator<<(XPacket& packet, const std::vector<ST_LEAGUE_MEMBER_EX>& vec) {
    packet.XParse << static_cast<std::int32_t>(vec.size());
    for (const auto& item : vec) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const std::vector<ST_LEAGUE_BOARD>& vec) {
    packet.XParse << static_cast<std::int32_t>(vec.size());
    for (const auto& item : vec) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const std::vector<ST_LEAGUE_APPLICANT>& vec) {
    packet.XParse << static_cast<std::int32_t>(vec.size());
    for (const auto& item : vec) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const std::vector<ST_LEAGUE_RECORD>& vec) {
    packet.XParse << static_cast<std::int32_t>(vec.size());
    for (const auto& item : vec) {
        packet << item;
    }
    return packet;
}

// PS_LEAGUE_INFO_SUMMARY 输出序列化
inline XPacket& operator<<(XPacket& packet, const PS_LEAGUE_INFO_SUMMARY& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLeagueName);
    packet.XParse << value.dwMasterUCID;
    packet.XParse << value.nLeagueRank;
    packet.XParse << value.nMemberCount;
    packet.XParse << value.nApplicantCount;
    packet.XParse << value.bOpen;
    return packet;
}

// PS_LEAGUE_SUMMARY_LIST 输出序列化
inline XPacket& operator<<(XPacket& packet, const PS_LEAGUE_SUMMARY_LIST& value) {
    packet.XParse << static_cast<std::int32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// ST_LEAGUE_APPLICANT_CHECK_LIST 输出序列化
inline XPacket& operator<<(XPacket& packet, const ST_LEAGUE_APPLICANT_CHECK_LIST& value) {
    packet.XParse << static_cast<std::int32_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet.XParse << item;
    }
    return packet;
}

// ============================================================================
// 联赛列表结构反序列化运算符（从 LeagueManager.h 迁移）
// ============================================================================

inline void operator>>(XPacket& packet, ST_LEAGUE_MEMBER_LIST& value) {
    packet.XParse >> value.nCount;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_BOARD_LIST& value) {
    packet.XParse >> value.nCount;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_APPLICANT_LIST& value) {
    packet.XParse >> value.nCount;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_RECORD_LIST& value) {
    packet.XParse >> value.nCount;
}

inline void operator>>(XPacket& packet, ST_LEAGUE_APPLICANT_CHECK_LIST& value) {
    packet.XParse >> value.nCount;
}

// PS_LEAGUE_INFO_SUMMARY 反序列化
inline void operator>>(XPacket& packet, PS_LEAGUE_INFO_SUMMARY& value) {
    packet.XParse >> value.nLeagueID;
    short outLen = 0;
    packet.XParse.GetWString(value.szLeagueName, 10, outLen);
    packet.XParse >> value.dwMasterUCID;
    packet.XParse >> value.nLeagueRank;
    packet.XParse >> value.nMemberCount;
    packet.XParse >> value.nApplicantCount;
    packet.XParse.GetBytes(reinterpret_cast<char*>(&value.bOpen), 1);
}

// PS_LEAGUE_SUMMARY_LIST 反序列化
inline void operator>>(XPacket& packet, PS_LEAGUE_SUMMARY_LIST& value) {
    packet.XParse >> value.nCount;
    value.vecInfo.clear();
    for (std::int32_t i = 0; i < value.nCount && i < 100; ++i) {
        PS_LEAGUE_INFO_SUMMARY item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

// PS_SERVER_CHANGE_CHARACTER_NAME 反序列化
inline void operator>>(XPacket& packet, PS_SERVER_CHANGE_CHARACTER_NAME& value) {
    packet >> value.psChangeInfo;
    packet >> value.stPartyInfo;
    packet.XParse >> value.nLeagueID;
    packet >> value.stApplyList;
}

// PS_SERVER_CHANGE_CHARACTER_NAME 输出序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_CHANGE_CHARACTER_NAME& value) {
    packet << value.psChangeInfo;
    packet << value.stPartyInfo;
    packet.XParse << value.nLeagueID;
    packet << value.stApplyList;
    return packet;
}

// ============================================================================
// ST_LEAGUE_LIST 和 ST_LEAGUE_MEMBER_LIST 输出序列化在 LeagueManager.h 中定义
// （因为 ST_LEAGUE_INFO 和 ST_LEAGUE_MEMBER_LIST 定义在此）
// ============================================================================
