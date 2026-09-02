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

// ============================================================================
// PSServerCore.h - 核心枚举、基础结构和辅助函数
//
// 此文件包含所有其他 PSServer* 模块共享的基础定义：
// - 核心枚举类型
// - 基础结构体（队伍信息、成员信息等）
// - 物品/镶嵌/镂刻/套装相关结构
// - 辅助函数（GreenDamTan_Bounded*）
// ============================================================================

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

// ============================================================================
// 基础队伍/成员结构（被多个模块引用）
// ============================================================================

/**
 * @brief 队伍/组队进入地图时附带的组信息。
 */
struct ST_PARTY_INFO {
    std::uint8_t byGroupType = 0;
    int nID = 0;
};

/**
 * @brief 队伍成员信息结构。
 */
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

/**
 * @brief 更新队伍成员结构。
 */
struct ST_UPDATE_PARTY_MEMBER {
    std::uint32_t dwPartyID = 0;
    std::uint8_t _pad0[4] = {};
    ST_PARTY_MEMBER stPartyMember{};
};

// ============================================================================
// 物品/镶嵌/镂刻/套装相关结构（被多个模块引用）
// ============================================================================

/**
 * @brief 镶嵌数据结构 - 48 bytes
 */
struct ST_SOCKET_DATA {
    std::int32_t dwSocketID = 0;
    std::uint8_t bySocketPos = 0;
    std::uint8_t _pad1[3] = {};
    ST_EXTEND_OPTION stExtendOption[5] = {};
};

/**
 * @brief 物品镶嵌结构（继承自 ST_SOCKET_DATA）- 56 bytes
 */
struct ST_ITEM_SOCKET : ST_SOCKET_DATA {
    std::int64_t biEquipSerial = 0;
};

/**
 * @brief 物品镶嵌列表 - 32 bytes
 */
struct PS_ITEM_SOCKET_LIST {
    std::vector<ST_ITEM_SOCKET> vecInfo;
};

/**
 * @brief 物品镂刻结构 - 72 bytes
 */
struct ST_ITEM_BROACH {
    std::int64_t biSerial = -1;
    int dwItemID[15];

    ST_ITEM_BROACH() {
        std::fill_n(dwItemID, 15, -1);
    }
};

/**
 * @brief 物品镂刻列表 - 32 bytes
 */
struct PS_ITEM_BROACH_LIST {
    std::vector<ST_ITEM_BROACH> vecInfo;
};

/**
 * @brief Active broach-effect request/response payload - 4 bytes.
 *
 * PDB-backed field layout; zero explicitly clears the current effect.
 */
struct PS_ACTIVE_BROACH_EFFECT {
    std::uint32_t dwActiveBuffID = 0;
};

static_assert(sizeof(PS_ACTIVE_BROACH_EFFECT) == 4,
              "PS_ACTIVE_BROACH_EFFECT size must match PDB");

/**
 * @brief 属性更新项 - 8 bytes
 */
struct ST_UPDATE_STAT {
    float fValue = 0.0f;
    std::uint16_t wStatID = 0;
};

static_assert(sizeof(ST_UPDATE_STAT) == 8, "ST_UPDATE_STAT size must match IDA");

/**
 * @brief 属性向量容器 - 32 bytes
 */
struct ST_STAT_VEC {
    std::vector<ST_UPDATE_STAT> vecUpdateStat;
};

static_assert(sizeof(ST_STAT_VEC) == 32, "ST_STAT_VEC size must match IDA");

/**
 * @brief 特殊选项更新项 - 8 bytes
 */
struct ST_UPDATE_SPECIAL_OPTION {
    std::uint16_t wOptionIndex = 0;
    float fValue = 0.0f;
};

static_assert(sizeof(ST_UPDATE_SPECIAL_OPTION) == 8, "ST_UPDATE_SPECIAL_OPTION size must match IDA");

/**
 * @brief 特殊选项更新列表 - 32 bytes
 */
struct ST_UPDATE_SPECIAL_OPTION_LIST {
    std::vector<ST_UPDATE_SPECIAL_OPTION> vecUpdateOption;
};

static_assert(sizeof(ST_UPDATE_SPECIAL_OPTION_LIST) == 32, "ST_UPDATE_SPECIAL_OPTION_LIST size must match IDA");

/**
 * @brief 快捷栏卡片槽位 - 48 bytes
 * 对齐 IDA GameServer.exe: byPage + padding + szDeckName[13] + union{nCard[5] or uniCard[5]}
 * Layout: byPage(1) + _pad0(1) + szDeckName[13](26) + union(20) = 48 bytes
 */
struct PS_QUICKSLOT_CARD {
    std::uint8_t byPage = 0;
    std::uint8_t _pad0 = 0;
    wchar_t szDeckName[13] = {};
    union {
        struct {
            std::uint32_t nCard_1;
            std::uint32_t nCard_2;
            std::uint32_t nCard_3;
            std::uint32_t nCard_4;
            std::uint32_t nCard_5;
        };
        std::uint32_t uniCard[5];
    };
};

static_assert(sizeof(PS_QUICKSLOT_CARD) == 48, "PS_QUICKSLOT_CARD size must match IDA");

/**
 * @brief 快捷栏物品 - 20 bytes
 * 对齐 IDA DBAgent.exe: uxActorID + union{dwItem[4] or uniItem[4]}
 */
struct PS_QUICKSLOT_ITEM {
    UXActorID uxActorID{};
    union {
        struct {
            std::uint32_t dwItem_1;
            std::uint32_t dwItem_2;
            std::uint32_t dwItem_3;
            std::uint32_t dwItem_4;
        };
        std::uint32_t uniItem[4];
    };
};

static_assert(sizeof(PS_QUICKSLOT_ITEM) == 20, "PS_QUICKSLOT_ITEM size must match IDA");

/**
 * @brief 快捷栏物品更新 - 16 bytes
 * 对齐 GameServer.exe PDB: four uint32 item IDs.
 */
struct PS_QUICKSLOT_UPDATE_ITEM {
    union {
        struct {
            std::uint32_t dwItem_1;
            std::uint32_t dwItem_2;
            std::uint32_t dwItem_3;
            std::uint32_t dwItem_4;
        };
        std::uint32_t uniItem[4];
    };
};

static_assert(sizeof(PS_QUICKSLOT_UPDATE_ITEM) == 16,
              "PS_QUICKSLOT_UPDATE_ITEM size must match PDB");

/**
 * @brief 快捷栏卡片向量 - 40 bytes
 * 对齐 IDA DBAgent.exe
 */
struct PS_QUICKSLOT_CARD_VEC {
    std::uint8_t byActivePage = 0;
    std::uint8_t _pad0[7] = {};
    std::vector<PS_QUICKSLOT_CARD> vecInfo;
};

static_assert(sizeof(PS_QUICKSLOT_CARD_VEC) == 40, "PS_QUICKSLOT_CARD_VEC size must match IDA");

/**
 * @brief 网吧状态信息 - 8 bytes
 * PDB LF_FIELDLIST (UDT 0xe400): bool nNetCafe @0, int nEventNo @4, 有默认构造。
 */
struct PS_NETCAFE_INFO {
    bool nNetCafe = false;
    int nEventNo = 0;
};

static_assert(sizeof(PS_NETCAFE_INFO) == 8, "PS_NETCAFE_INFO size must match PDB");

/**
 * @brief 快捷栏更新卡片 - 24 bytes
 * 对齐 IDA DBAgent.exe: byPage + padding + union{nCard[5] or uniCard[5]}
 */
struct PS_QUICKSLOT_UPDATE_CARD {
    std::uint8_t byPage = 0;
    std::uint8_t _pad0[3] = {};
    union {
        struct {
            std::uint32_t nCard_1;
            std::uint32_t nCard_2;
            std::uint32_t nCard_3;
            std::uint32_t nCard_4;
            std::uint32_t nCard_5;
        };
        std::uint32_t uniCard[5];
    };
};

static_assert(sizeof(PS_QUICKSLOT_UPDATE_CARD) == 24, "PS_QUICKSLOT_UPDATE_CARD size must match IDA");

// 对齐 IDA: 快捷栏更新卡片列表 (32 bytes)
struct PS_QUICKSLOT_UPDATE_CARD_VEC {
    std::vector<PS_QUICKSLOT_UPDATE_CARD> vecInfo;
};

static_assert(sizeof(PS_QUICKSLOT_UPDATE_CARD_VEC) == 32, "PS_QUICKSLOT_UPDATE_CARD_VEC size must match IDA");

// PS_QUICKSLOT_UPDATE_CARD_VEC 反序列化运算符
inline void operator>>(XPacket& packet, PS_QUICKSLOT_UPDATE_CARD_VEC& value) {
    std::int16_t shCount = 0;
    packet.XParse >> shCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(shCount));
    for (std::int16_t i = 0; i < shCount; ++i) {
        PS_QUICKSLOT_UPDATE_CARD item{};
        packet.XParse >> item.byPage;
        packet.XParse.GetBytes(reinterpret_cast<char*>(item._pad0), sizeof(item._pad0));
        for (int j = 0; j < 5; ++j) {
            packet.XParse >> item.uniCard[j];
        }
        value.vecInfo.push_back(item);
    }
}

/**
 * @brief 成就类别统计 - 14 bytes
 */
struct ST_ACHIEVE_CATEGORY {
    std::uint16_t wCount[7] = {};
};

static_assert(sizeof(ST_ACHIEVE_CATEGORY) == 14, "ST_ACHIEVE_CATEGORY size must match IDA");

/**
 * @brief 使用物品信息 - 16 bytes
 * 对齐 IDA DBAgent.exe
 */
struct ST_USE_ITEM_INFO {
    int nItemType = 0;
    std::uint8_t byCount = 0;
    std::uint8_t _pad0[3] = {};
    std::int64_t nUseDate = 0;
};

static_assert(sizeof(ST_USE_ITEM_INFO) == 16, "ST_USE_ITEM_INFO size must match IDA");

/**
 * @brief 使用物品信息列表 - 32 bytes
 */
struct ST_USE_ITEM_INFO_LIST {
    std::vector<ST_USE_ITEM_INFO> vecInfo;
};

static_assert(sizeof(ST_USE_ITEM_INFO_LIST) == 32, "ST_USE_ITEM_INFO_LIST size must match IDA");

// PDB: stEMPTYSLOT is the 4-byte item-inventory empty-slot result.
struct stEMPTYSLOT {
    std::uint8_t byInvenType = 0;
    std::uint8_t _pad0 = 0;
    std::int16_t shSlotPos = 0;
};

static_assert(sizeof(stEMPTYSLOT) == 4,
              "stEMPTYSLOT size must match GameServer PDB");
static_assert(offsetof(stEMPTYSLOT, byInvenType) == 0,
              "stEMPTYSLOT.byInvenType offset mismatch");
static_assert(offsetof(stEMPTYSLOT, shSlotPos) == 2,
              "stEMPTYSLOT.shSlotPos offset mismatch");

// GameServer item-process request payloads recovered from the PDB field lists.
struct PS_REQ_ITEM_MOVE {
    std::uint8_t bySrcInvenType = 0;
    std::uint8_t _pad0[3] = {};
    int nSrcItemID = 0;
    std::int16_t shSrcSlotPos = 0;
    std::uint8_t byDestInvenType = 0;
    std::uint8_t _pad1 = 0;
    int nDestItemID = 0;
    std::int16_t shDestSlotPos = 0;
    std::uint8_t _pad2[2] = {};
};

static_assert(sizeof(PS_REQ_ITEM_MOVE) == 20,
              "PS_REQ_ITEM_MOVE size must match GameServer PDB");
static_assert(offsetof(PS_REQ_ITEM_MOVE, nSrcItemID) == 4,
              "PS_REQ_ITEM_MOVE.nSrcItemID offset mismatch");
static_assert(offsetof(PS_REQ_ITEM_MOVE, shSrcSlotPos) == 8,
              "PS_REQ_ITEM_MOVE.shSrcSlotPos offset mismatch");
static_assert(offsetof(PS_REQ_ITEM_MOVE, byDestInvenType) == 10,
              "PS_REQ_ITEM_MOVE.byDestInvenType offset mismatch");
static_assert(offsetof(PS_REQ_ITEM_MOVE, nDestItemID) == 12,
              "PS_REQ_ITEM_MOVE.nDestItemID offset mismatch");
static_assert(offsetof(PS_REQ_ITEM_MOVE, shDestSlotPos) == 16,
              "PS_REQ_ITEM_MOVE.shDestSlotPos offset mismatch");

struct PS_REQ_ITEM_USE {
    std::uint8_t byInvenType = 0;
    std::uint8_t _pad0 = 0;
    std::int16_t shSlotPos = 0;
    std::uint8_t _pad1[4] = {};
    std::int64_t xSerial = 0;
    std::uint8_t byCount = 0;
    std::uint8_t _pad2[7] = {};
};

static_assert(sizeof(PS_REQ_ITEM_USE) == 24,
              "PS_REQ_ITEM_USE size must match GameServer PDB");
static_assert(offsetof(PS_REQ_ITEM_USE, shSlotPos) == 2,
              "PS_REQ_ITEM_USE.shSlotPos offset mismatch");
static_assert(offsetof(PS_REQ_ITEM_USE, xSerial) == 8,
              "PS_REQ_ITEM_USE.xSerial offset mismatch");
static_assert(offsetof(PS_REQ_ITEM_USE, byCount) == 16,
              "PS_REQ_ITEM_USE.byCount offset mismatch");

struct PS_REQ_ITEM_COMBINE : PS_REQ_ITEM_MOVE {
    std::int16_t sCount = 0;
    std::uint8_t _pad0[2] = {};
};

static_assert(sizeof(PS_REQ_ITEM_COMBINE) == 24,
              "PS_REQ_ITEM_COMBINE size must match GameServer PDB");
static_assert(offsetof(PS_REQ_ITEM_COMBINE, sCount) == 20,
              "PS_REQ_ITEM_COMBINE.sCount offset mismatch");

struct PS_REQ_ITEM_DIVIDE {
    std::uint8_t bySrcInvenType = 0;
    std::uint8_t _pad0[3] = {};
    int nSrcItemID = 0;
    std::int16_t shSrcSlotPos = 0;
    std::uint8_t byDestInvenType = 0;
    std::uint8_t _pad1 = 0;
    std::int16_t shDestSlotPos = 0;
    std::int16_t sCount = 0;
};

static_assert(sizeof(PS_REQ_ITEM_DIVIDE) == 16,
              "PS_REQ_ITEM_DIVIDE size must match GameServer PDB");
static_assert(offsetof(PS_REQ_ITEM_DIVIDE, nSrcItemID) == 4,
              "PS_REQ_ITEM_DIVIDE.nSrcItemID offset mismatch");
static_assert(offsetof(PS_REQ_ITEM_DIVIDE, shSrcSlotPos) == 8,
              "PS_REQ_ITEM_DIVIDE.shSrcSlotPos offset mismatch");
static_assert(offsetof(PS_REQ_ITEM_DIVIDE, byDestInvenType) == 10,
              "PS_REQ_ITEM_DIVIDE.byDestInvenType offset mismatch");
static_assert(offsetof(PS_REQ_ITEM_DIVIDE, shDestSlotPos) == 12,
              "PS_REQ_ITEM_DIVIDE.shDestSlotPos offset mismatch");
static_assert(offsetof(PS_REQ_ITEM_DIVIDE, sCount) == 14,
              "PS_REQ_ITEM_DIVIDE.sCount offset mismatch");

struct PS_REQ_TICKCOUNT {
    int nTicknum = 0;
    std::uint8_t _pad0[4] = {};
    std::uint64_t dwTickcount = 0;
};

static_assert(sizeof(PS_REQ_TICKCOUNT) == 16,
              "PS_REQ_TICKCOUNT size must match GameServer PDB");
static_assert(offsetof(PS_REQ_TICKCOUNT, dwTickcount) == 8,
              "PS_REQ_TICKCOUNT.dwTickcount offset mismatch");

inline XPacket& operator<<(XPacket& packet, const PS_REQ_ITEM_MOVE& value) {
    packet.XParse << value.bySrcInvenType;
    packet.XParse << value.nSrcItemID;
    packet.XParse << value.shSrcSlotPos;
    packet.XParse << value.byDestInvenType;
    packet.XParse << value.nDestItemID;
    packet.XParse << value.shDestSlotPos;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_REQ_ITEM_MOVE& value) {
    packet.XParse >> value.bySrcInvenType;
    packet.XParse >> value.nSrcItemID;
    packet.XParse >> value.shSrcSlotPos;
    packet.XParse >> value.byDestInvenType;
    packet.XParse >> value.nDestItemID;
    packet.XParse >> value.shDestSlotPos;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_REQ_ITEM_USE& value) {
    packet.XParse << value.byInvenType;
    packet.XParse << value.shSlotPos;
    packet.XParse << value.xSerial;
    packet.XParse << value.byCount;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_REQ_ITEM_USE& value) {
    packet.XParse >> value.byInvenType;
    packet.XParse >> value.shSlotPos;
    packet.XParse >> value.xSerial;
    packet.XParse >> value.byCount;
    return packet;
}

inline XPacket& operator<<(XPacket& packet,
                           const PS_REQ_ITEM_COMBINE& value) {
    packet << static_cast<const PS_REQ_ITEM_MOVE&>(value);
    packet.XParse << value.sCount;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_REQ_ITEM_COMBINE& value) {
    packet >> static_cast<PS_REQ_ITEM_MOVE&>(value);
    packet.XParse >> value.sCount;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_REQ_ITEM_DIVIDE& value) {
    packet.XParse << value.bySrcInvenType;
    packet.XParse << value.nSrcItemID;
    packet.XParse << value.shSrcSlotPos;
    packet.XParse << value.byDestInvenType;
    packet.XParse << value.shDestSlotPos;
    packet.XParse << value.sCount;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_REQ_ITEM_DIVIDE& value) {
    packet.XParse >> value.bySrcInvenType;
    packet.XParse >> value.nSrcItemID;
    packet.XParse >> value.shSrcSlotPos;
    packet.XParse >> value.byDestInvenType;
    packet.XParse >> value.shDestSlotPos;
    packet.XParse >> value.sCount;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_REQ_TICKCOUNT& value) {
    packet.XParse << value.nTicknum;
    packet.XParse << value.dwTickcount;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, PS_REQ_TICKCOUNT& value) {
    packet.XParse >> value.nTicknum;
    packet.XParse >> value.dwTickcount;
    return packet;
}

/**
 * @brief 物品套装部件 - 16 bytes
 */
struct ST_ITEM_PACKAGE_PARTS {
    std::int64_t biSerial = 0;
    int nItemID = 0;
    int nDyeID = 0;
};

/**
 * @brief 物品套装 - 40 bytes
 */
struct PS_ITEM_PACKAGE {
    std::int64_t biPackageSerial = 0;
    std::vector<ST_ITEM_PACKAGE_PARTS> vecInfo;
};

/**
 * @brief 物品套装列表 - 32 bytes
 */
struct PS_ITEM_PACKAGE_LIST {
    std::vector<PS_ITEM_PACKAGE> vecInfo;
};

// ============================================================================
// 辅助函数
// ============================================================================

/**
 * @brief 将固定大小宽字符数组转换为 std::wstring（自动截断）
 */
template <std::size_t N>
inline std::wstring GreenDamTan_BoundedWideString(const wchar_t (&value)[N]) {
    constexpr std::size_t maxChars = N > 0 ? N - 1 : 0;
    const wchar_t* end = std::find(value, value + maxChars, L'\0');
    return std::wstring(value, static_cast<std::size_t>(end - value));
}

/**
 * @brief 将固定大小字符数组转换为 std::string（自动截断）
 */
template <std::size_t N>
inline std::string GreenDamTan_BoundedString(const char (&value)[N]) {
    constexpr std::size_t maxChars = N > 0 ? N - 1 : 0;
    const char* end = std::find(value, value + maxChars, '\0');
    return std::string(value, static_cast<std::size_t>(end - value));
}

// ============================================================================
// 基础结构序列化运算符
// ============================================================================

// ST_PARTY_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_PARTY_INFO& value) {
    packet.XParse << value.byGroupType;
    packet.XParse << value.nID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_INFO& value) {
    packet.XParse >> value.byGroupType;
    packet.XParse >> value.nID;
}

// ST_PARTY_MEMBER 序列化
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
    packet << value.uxMapID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_PARTY_MEMBER& value) {
    short outLen = 0;
    packet.XParse >> value.dwMemberID;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.byLevel;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.nMapID;
    packet.XParse >> value.nChannel;
    packet.XParse >> value.nMaxHP;
    packet.XParse >> value.nHP;
    packet.XParse >> value.bLogin;
    packet >> value.uxMapID;
}

// ST_UPDATE_PARTY_MEMBER 序列化
inline XPacket& operator<<(XPacket& packet, const ST_UPDATE_PARTY_MEMBER& value) {
    packet.XParse << value.dwPartyID;
    packet << value.stPartyMember;
    return packet;
}

inline void operator>>(XPacket& packet, ST_UPDATE_PARTY_MEMBER& value) {
    packet.XParse >> value.dwPartyID;
    packet >> value.stPartyMember;
}

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

// PS_QUICKSLOT_CARD XPacket 序列化
inline XPacket& operator<<(XPacket& packet, const PS_QUICKSLOT_CARD& value) {
    packet.XParse << value.byPage;
    packet.XParse << GreenDamTan_BoundedWideString(value.szDeckName);
    for (int i = 0; i < 5; ++i) {
        packet.XParse << value.uniCard[i];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_QUICKSLOT_CARD& value) {
    packet.XParse >> value.byPage;
    short outLen = 0;
    packet.XParse.GetWString(value.szDeckName, 13, outLen);
    for (int i = 0; i < 5; ++i) {
        packet.XParse >> value.uniCard[i];
    }
}

// PS_QUICKSLOT_UPDATE_ITEM 序列化运算符
inline XSendDBPacket& operator<<(XSendDBPacket& packet,
                                 const PS_QUICKSLOT_UPDATE_ITEM& value) {
    for (int i = 0; i < 4; ++i) {
        packet.XParse << value.uniItem[i];
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet,
                           const PS_QUICKSLOT_UPDATE_ITEM& value) {
    for (int i = 0; i < 4; ++i) {
        packet.XParse << value.uniItem[i];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_QUICKSLOT_UPDATE_ITEM& value) {
    for (int i = 0; i < 4; ++i) {
        packet.XParse >> value.uniItem[i];
    }
}

// PS_QUICKSLOT_ITEM 序列化运算符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_QUICKSLOT_ITEM& value) {
    packet << value.uxActorID;
    for (int i = 0; i < 4; ++i) {
        packet.XParse << value.uniItem[i];
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_QUICKSLOT_ITEM& value) {
    packet << value.uxActorID;
    for (int i = 0; i < 4; ++i) {
        packet.XParse << value.uniItem[i];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_QUICKSLOT_ITEM& value) {
    packet >> value.uxActorID;
    for (int i = 0; i < 4; ++i) {
        packet.XParse >> value.uniItem[i];
    }
}

// PS_NETCAFE_INFO 序列化运算符
inline XPacket& operator<<(XPacket& packet, const PS_NETCAFE_INFO& value) {
    packet.XParse << value.nNetCafe;
    packet.XParse << value.nEventNo;
    return packet;
}

inline void operator>>(XPacket& packet, PS_NETCAFE_INFO& value) {
    packet.XParse >> value.nNetCafe;
    packet.XParse >> value.nEventNo;
}

// PS_QUICKSLOT_CARD_VEC 序列化运算符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_QUICKSLOT_CARD_VEC& value) {
    packet.XParse << value.byActivePage;
    std::uint16_t count = static_cast<std::uint16_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const auto& card : value.vecInfo) {
        packet << card;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_QUICKSLOT_CARD_VEC& value) {
    packet.XParse << value.byActivePage;
    auto count = static_cast<std::uint8_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const auto& card : value.vecInfo) {
        packet << card;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_QUICKSLOT_CARD_VEC& value) {
    packet.XParse >> value.byActivePage;
    std::uint8_t count = 0;
    packet.XParse >> count;
    for (int i = 0; i < count; ++i) {
        PS_QUICKSLOT_CARD card{};
        packet >> card;
        value.vecInfo.push_back(card);
    }
}

// PS_QUICKSLOT_UPDATE_CARD 序列化运算符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_QUICKSLOT_UPDATE_CARD& value) {
    packet.XParse << value.byPage;
    for (int i = 0; i < 5; ++i) {
        packet.XParse << value.uniCard[i];
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_QUICKSLOT_UPDATE_CARD& value) {
    packet.XParse << value.byPage;
    for (int i = 0; i < 5; ++i) {
        packet.XParse << value.uniCard[i];
    }
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_QUICKSLOT_UPDATE_CARD_VEC& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_QUICKSLOT_UPDATE_CARD_VEC& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_QUICKSLOT_UPDATE_CARD& value) {
    packet.XParse >> value.byPage;
    for (int i = 0; i < 5; ++i) {
        packet.XParse >> value.uniCard[i];
    }
}

// ST_USE_ITEM_INFO 序列化运算符
inline XPacket& operator<<(XPacket& packet, ST_USE_ITEM_INFO& value) {
    packet.XParse << value.nItemType;
    packet.XParse << value.byCount;
    packet.XParse << value.nUseDate;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_USE_ITEM_INFO& value) {
    packet.XParse >> value.nItemType;
    packet.XParse >> value.byCount;
    packet.XParse >> value.nUseDate;
    return packet;
}

// ST_USE_ITEM_INFO_LIST 序列化运算符
inline XPacket& operator<<(XPacket& packet, ST_USE_ITEM_INFO_LIST& value) {
    char cCount = static_cast<char>(value.vecInfo.size());
    packet.XParse << cCount;

    for (char c = 0; c < cCount; ++c) {
        packet << value.vecInfo[static_cast<std::size_t>(c)];
    }

    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_USE_ITEM_INFO_LIST& value) {
    char cCount = 0;
    packet.XParse >> cCount;

    for (char c = 0; c < cCount; ++c) {
        ST_USE_ITEM_INFO info{};
        packet >> info;
        value.vecInfo.push_back(info);
    }

    return packet;
}

// ST_ACHIEVE_CATEGORY XPacket 序列化
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

// ============================================================================
// XPacket 序列化运算符（用于客户端通信）
// ============================================================================

// ST_SOCKET_DATA XPacket 序列化
inline XPacket& operator<<(XPacket& packet, const ST_SOCKET_DATA& value) {
    packet.XParse << value.dwSocketID;
    packet.XParse << value.bySocketPos;
    for (const ST_EXTEND_OPTION& extendOption : value.stExtendOption) {
        packet << extendOption;
    }
    return packet;
}

inline void operator>>(XPacket& packet, ST_SOCKET_DATA& value) {
    packet.XParse >> value.dwSocketID;
    packet.XParse >> value.bySocketPos;
    for (ST_EXTEND_OPTION& extendOption : value.stExtendOption) {
        packet >> extendOption;
    }
}

// ST_ITEM_SOCKET XPacket 序列化
inline XPacket& operator<<(XPacket& packet, const ST_ITEM_SOCKET& value) {
    packet.XParse << value.biEquipSerial;
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

// PS_ITEM_SOCKET_LIST XPacket 序列化
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

// ST_ITEM_BROACH XPacket 序列化
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

// ST_ITEM_PACKAGE_PARTS XPacket 序列化
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

// PS_ITEM_PACKAGE XPacket 序列化
inline void operator>>(XPacket& packet, PS_ITEM_PACKAGE& value) {
    int count = 0;
    packet.XParse >> value.biPackageSerial;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>((count > 0) ? count : 0));
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

// PS_ITEM_BROACH_LIST XPacket 序列化
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

// PS_ACTIVE_BROACH_EFFECT XPacket serialization
inline XPacket& operator>>(XPacket& packet, PS_ACTIVE_BROACH_EFFECT& value) {
    packet.XParse >> value.dwActiveBuffID;
    return packet;
}

inline XPacket& operator<<(XPacket& packet,
                           const PS_ACTIVE_BROACH_EFFECT& value) {
    packet.XParse << value.dwActiveBuffID;
    return packet;
}

// PS_ITEM_PACKAGE_LIST XPacket 序列化
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

// 镂刻序列号列表 - PDB UDT 0x13b6e, 32 bytes: vecInfo = vector<int64>
struct PS_BROACH_SERIAL_LIST {
    std::vector<std::int64_t> vecInfo;
};

// PS_BROACH_SERIAL_LIST XPacket 序列化
inline XPacket& operator<<(XPacket& packet, const PS_BROACH_SERIAL_LIST& value) {
    // 对齐 IDA SendBroachUpdate (0x1400C0910): 使用 int nCount
    std::int32_t nCount = static_cast<std::int32_t>(value.vecInfo.size());
    packet.XParse << nCount;
    for (const auto& biSerial : value.vecInfo) {
        packet.XParse << biSerial;
    }
    return packet;
}
// ============================================================================
// static_assert 验证（确保结构体大小与 PDB 匹配）
// ============================================================================

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

// ============================================================================
// PS_OBJECT_REMOVE - Object Remove Structure
// ============================================================================

/**
 * @brief PS_OBJECT_REMOVE - Object removal notification structure.
 * IDA: struct PS_OBJECT_REMOVE (32 bytes)
 * Used by XArea::SendOutInfo to notify clients about removed objects.
 */
struct PS_OBJECT_REMOVE {
    std::vector<UXActorID> vecObjectID;
};

static_assert(sizeof(PS_OBJECT_REMOVE) == 32, "PS_OBJECT_REMOVE size must match IDA");

// PS_OBJECT_REMOVE XPacket serialization
inline XPacket& operator<<(XPacket& packet, const PS_OBJECT_REMOVE& value) {
    std::uint16_t count = static_cast<std::uint16_t>(value.vecObjectID.size());
    packet.XParse << count;
    for (const auto& actorID : value.vecObjectID) {
        packet << actorID;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_OBJECT_REMOVE& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecObjectID.clear();
    value.vecObjectID.reserve(static_cast<std::size_t>(count));
    for (std::uint16_t i = 0; i < count; ++i) {
        UXActorID actorID{};
        packet >> actorID;
        value.vecObjectID.push_back(actorID);
    }
}

// ============================================================================
// PS_REQ_MOVE_MONEY - Money Move Request Structure
// ============================================================================

/**
 * @brief PS_REQ_MOVE_MONEY - Request to move money between inventory and bank.
 * IDA: struct PS_REQ_MOVE_MONEY
 * Used by CGocInventory::IsValidMoveMoney.
 * byTargetType 0: Bank to Inventory
 * byTargetType 1: Inventory to Bank
 */
struct PS_REQ_MOVE_MONEY {
    std::uint8_t byTargetType = 0;  // 0 = Bank->Inventory, 1 = Inventory->Bank
    std::int64_t biMoney = 0;       // Amount to move
};

// PS_REQ_MOVE_MONEY XPacket serialization
inline XPacket& operator<<(XPacket& packet, const PS_REQ_MOVE_MONEY& value) {
    packet.XParse << value.byTargetType;
    packet.XParse << value.biMoney;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_MOVE_MONEY& value) {
    packet.XParse >> value.byTargetType;
    packet.XParse >> value.biMoney;
}
