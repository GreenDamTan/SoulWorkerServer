#pragma once

// 必须在包含 Windows 头文件前定义 NOMINMAX
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

template <std::size_t N>
inline std::wstring FixedWideArrayToWString(const wchar_t (&value)[N]) {
    const wchar_t* begin = value;
    const wchar_t* end = std::find(begin, begin + N, L'\0');
    return std::wstring(begin, end);
}

// 将 std::wstring 复制到固定长度的 wchar_t 数组（反序列化辅助）
template <std::size_t N>
inline void WStringToFixedWideArray(const std::wstring& src, wchar_t (&dest)[N]) {
    std::fill_n(dest, N, L'\0');
    const std::size_t copyLen = (std::min)(src.size(), N - 1);
    std::copy_n(src.data(), copyLen, dest);
}

template <std::size_t N>
inline std::string FixedCharArrayToString(const char (&value)[N]) {
    const char* begin = value;
    const char* end = std::find(begin, begin + N, '\0');
    return std::string(begin, end);
}

// PS_KICK_USER_INFO - 踢出用户信息结构 (size: 1040 bytes = 0x410)
// IDA: 构造函数 0x140001680
// 反编译:
//   PS_KICK_USER_INFO *__fastcall PS_KICK_USER_INFO::PS_KICK_USER_INFO(PS_KICK_USER_INFO *this)
//   {
//     this->byKickType = 0;
//     this->dwUAID = 0;
//     this->strMsg[0] = 0;
//     this->nParam = 0;
//     return this;
//   }
struct PS_KICK_USER_INFO {
    int byKickType = 0;          // 踢出类型 (offset 0, size 4)
    unsigned int dwUAID = 0;     // 用户账号ID (offset 4, size 4)
    int nParam = 0;              // 参数 (offset 8, size 4)
    wchar_t strMsg[513] = {};    // 消息 (offset 12, size 1026)

    // 显式构造函数 (IDA 0x140001680)
    PS_KICK_USER_INFO() : byKickType(0), dwUAID(0), nParam(0), strMsg{} {}
};

static_assert(sizeof(PS_KICK_USER_INFO) == 0x410, "PS_KICK_USER_INFO size must match PDB");
static_assert(offsetof(PS_KICK_USER_INFO, nParam) == 0x8, "PS_KICK_USER_INFO.nParam offset mismatch");
static_assert(offsetof(PS_KICK_USER_INFO, strMsg) == 0xC, "PS_KICK_USER_INFO.strMsg offset mismatch");

struct PS_LOGIN_RES {
    int nUAID = 0;
    bool bClearTutorial = false;
    char szMacAddress[18] = {};
    wchar_t szLoginResultMsg[1025] = {};
    int nErrorCode = 0;
    std::uint8_t byLoginType = 0;
    wchar_t szAuthID[21] = {};
    std::int64_t biAuthSessionID = 0;
    std::uint8_t byGMPower = 0;
    std::uint16_t wBirth_Year = 0;
    std::uint8_t byBirth_Month = 0;
    std::uint8_t byBirth_Day = 0;
};

static_assert(sizeof(PS_LOGIN_RES) == 0x860, "PS_LOGIN_RES size must match PDB");
static_assert(offsetof(PS_LOGIN_RES, szMacAddress) == 0x5, "PS_LOGIN_RES.szMacAddress offset mismatch");
static_assert(offsetof(PS_LOGIN_RES, szLoginResultMsg) == 0x18, "PS_LOGIN_RES.szLoginResultMsg offset mismatch");
static_assert(offsetof(PS_LOGIN_RES, szAuthID) == 0x822, "PS_LOGIN_RES.szAuthID offset mismatch");
static_assert(offsetof(PS_LOGIN_RES, biAuthSessionID) == 0x850, "PS_LOGIN_RES.biAuthSessionID offset mismatch");

/**
 * @brief 客户端提交给二级密码流程的请求结构。
 *
 * PDB 中确认的布局为：
 * - `byCheckType`：本次请求的检查类型
 * - `strPassword[7]`：二级密码明文，原协议固定 6 位并保留结尾 `\0`
 */
struct PS_SECOND_PW_REQ {
    std::uint8_t byCheckType = 0;
    char strPassword[7] = {};
};

/**
 * @brief AccountDB 返回给登录/角色链路的二级密码结果。
 *
 * `nErrorID` 由服务端/数据库决定错误类型，
 * `bySecondPWState` 则表示当前二级密码状态。
 */
struct PS_SECOND_PW_RES {
    int nErrorID = 0;
    std::uint8_t bySecondPWState = 0;
};

/**
 * @brief 交易密码请求结构（5位密码）。
 */
struct PS_TRADE_PW_REQ {
    char strPassword[5] = {};
};

/**
 * @brief 交易密码响应结构。
 */
struct PS_TRADE_PW_RES {
    int nErrorID = 0;
    std::uint8_t byTradePWState = 0;
};

/**
 * @brief Soul Gauge / 第三方认证信息缓存。
 *
 * 该结构来自 PDB 中的 `ST_SG_AUTH_INFO`，当前登录服骨架尚未完整消费，
 * 但先把字段布局保留下来，便于后续继续接通认证链路。
 * IDA: ??0ST_SG_AUTH_INFO@@QEAA@XZ (0x1400687e0)
 */
struct ST_SG_AUTH_INFO {
    unsigned int nUAID = 0;
    std::uint8_t byAuthType = 0;
    char szToken[1025] = {};
    char szRefreshToken[1025] = {};
    std::int64_t nExpireTime = 0;
    std::int64_t nBirth = -1;   // IDA: 默认值 -1
};

static_assert(sizeof(ST_SG_AUTH_INFO) == 0x818, "ST_SG_AUTH_INFO size must match PDB");
static_assert(offsetof(ST_SG_AUTH_INFO, szToken) == 0x5, "ST_SG_AUTH_INFO.szToken offset mismatch");
static_assert(offsetof(ST_SG_AUTH_INFO, szRefreshToken) == 0x406, "ST_SG_AUTH_INFO.szRefreshToken offset mismatch");

/**
 * @brief Gameforge 认证信息缓存。
 *
 * 第 130 轮继续结合 IDA named type `ST_GF_AUTH_INFO` 与 `CUser::m_stGFAuthInfo`
 * 成员偏移交叉确认后补回：
 * - `nUAID`
 * - `szToken / szRefreshToken`
 * - `nExpireTime / nBirth`
 * - `szGFAccountID / szGFClientID / szDisplayName`
 * IDA: ??0ST_GF_AUTH_INFO@@QEAA@XZ (0x140068770)
 */
struct ST_GF_AUTH_INFO {
    unsigned int nUAID = 0;
    char szToken[1025] = {};
    char szRefreshToken[1025] = {};
    std::int64_t nExpireTime = 0;
    std::int64_t nBirth = -1;           // IDA: 默认值 -1
    char szGFAccountID[256] = {};
    char szGFClientID[256] = {};
    char szDisplayName[21] = {};
};

static_assert(sizeof(ST_GF_AUTH_INFO) == 0xA30, "ST_GF_AUTH_INFO size must match PDB");
static_assert(offsetof(ST_GF_AUTH_INFO, szToken) == 0x4, "ST_GF_AUTH_INFO.szToken offset mismatch");
static_assert(offsetof(ST_GF_AUTH_INFO, szRefreshToken) == 0x405,
              "ST_GF_AUTH_INFO.szRefreshToken offset mismatch");
static_assert(offsetof(ST_GF_AUTH_INFO, nExpireTime) == 0x808,
              "ST_GF_AUTH_INFO.nExpireTime offset mismatch");
static_assert(offsetof(ST_GF_AUTH_INFO, nBirth) == 0x810, "ST_GF_AUTH_INFO.nBirth offset mismatch");
static_assert(offsetof(ST_GF_AUTH_INFO, szGFAccountID) == 0x818,
              "ST_GF_AUTH_INFO.szGFAccountID offset mismatch");
static_assert(offsetof(ST_GF_AUTH_INFO, szGFClientID) == 0x918,
              "ST_GF_AUTH_INFO.szGFClientID offset mismatch");
static_assert(offsetof(ST_GF_AUTH_INFO, szDisplayName) == 0xA18,
              "ST_GF_AUTH_INFO.szDisplayName offset mismatch");

/**
 * @brief XignCode 校验更新包。
 */
struct PS_XIGNCODE_UPDATE {
    int session = 0;
    char szBuff[512] = {};
};

static_assert(sizeof(PS_XIGNCODE_UPDATE) == 0x204, "PS_XIGNCODE_UPDATE size must match PDB");

/**
 * @brief XignCode 错误包。
 */
struct PS_XIGNCODE_ERROR {
    int Code = 0;
    int w = 0;
};

static_assert(sizeof(PS_XIGNCODE_ERROR) == 0x8, "PS_XIGNCODE_ERROR size must match PDB");

/**
 * @brief SG Token 更新包。
 */
struct PS_SG_TOKEN_UPDATE {
    wchar_t szAcessToken[1025] = {};
    wchar_t szRefreshToken[1025] = {};
    bool bRenew = false;
};

static_assert(sizeof(PS_SG_TOKEN_UPDATE) == 0x1006, "PS_SG_TOKEN_UPDATE size must match PDB");

/**
 * @brief GameGuard 认证数据包。
 */
struct PS_GAME_GUARD_AUTH {
    unsigned int dwServerNumber = 0;
    unsigned int dwPacketSize = 0;
    std::uint8_t bPacket[4096] = {};
};

static_assert(sizeof(PS_GAME_GUARD_AUTH) == 0x1008, "PS_GAME_GUARD_AUTH size must match PDB");

/**
 * @brief GameGuard 错误回包。
 */
struct PS_GAME_GUARD_ERROR {
    unsigned int dwErrorCode = 0;
};

static_assert(sizeof(PS_GAME_GUARD_ERROR) == 0x4, "PS_GAME_GUARD_ERROR size must match PDB");

/**
 * @brief PDB 中的 29bit 角色 ID + 3bit 类型联合体。
 */
union UXActorID {
    struct ActorParts {
        unsigned int dwID : 29;
        unsigned int byType : 3;
    } parts;
    unsigned int dwActorID;

    constexpr UXActorID() : dwActorID(0) {}
    constexpr UXActorID(unsigned int id) : dwActorID(id) {}
    constexpr operator unsigned long() const { return static_cast<unsigned long>(dwActorID); }
    constexpr unsigned int GetID() const { return parts.dwID; }
    UXActorID& operator=(unsigned int id) { dwActorID = id; return *this; }
    
    // Comparison operators for use in containers
    constexpr bool operator==(const UXActorID& other) const { return dwActorID == other.dwActorID; }
    constexpr bool operator!=(const UXActorID& other) const { return dwActorID != other.dwActorID; }
    constexpr bool operator<(const UXActorID& other) const { return dwActorID < other.dwActorID; }
};

// UXActorID hash support for use in unordered containers
namespace std {
    template<>
    struct hash<UXActorID> {
        size_t operator()(const UXActorID& id) const noexcept {
            return hash<unsigned int>{}(id.dwActorID);
        }
    };
}

/**
 * @brief PDB 中的外观拆分结构。
 */
struct STAppearanceEx {
    std::uint16_t wAppearanceID[4] = {};
};

static_assert(sizeof(STAppearanceEx) == 0x8, "STAppearanceEx size must match PDB");

/**
 * @brief PDB 中的外观联合体，可按分项或按整型整体访问。
 */
union UAppearanceEx {
    STAppearanceEx stAppearance;
    std::uint64_t biAppearance;

    constexpr UAppearanceEx() : biAppearance(0) {}
};

static_assert(sizeof(UAppearanceEx) == 0x8, "UAppearanceEx size must match PDB");

/**
 * @brief PDB 中的地图实例联合体。
 */
union UXMapID {
    struct MapParts {
        std::int64_t seq : 24;
        std::int64_t channel : 8;
        std::int64_t mapID : 16;
        std::int64_t serverID : 16;
    } parts;
    std::int64_t nMapID;

    constexpr UXMapID() : nMapID(0) {}

    // 对齐 IDA: 用于 std::map 键
    constexpr bool operator<(const UXMapID& rhs) const {
        return nMapID < rhs.nMapID;
    }
    constexpr bool operator==(const UXMapID& rhs) const {
        return nMapID == rhs.nMapID;
    }
    constexpr bool operator!=(const UXMapID& rhs) const {
        return nMapID != rhs.nMapID;
    }
};

// 对齐 IDA: UXMapID 包序列化
inline XPacket& operator<<(XPacket& packet, const UXMapID& value) {
    packet.XParse << value.nMapID;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, UXMapID& value) {
    packet.XParse >> value.nMapID;
    return packet;
}

/**
 * @brief 三维向量结构。
 */
struct XVec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

/**
 * @brief 角色基础外观/名称信息。
 */
struct STBaseCharInfo {
    wchar_t strName[21] = {};
    std::uint8_t byClass = 0;
    std::uint8_t byAwaken = 0;
    unsigned int dwProfilePhotoID = 0;
    UAppearanceEx uAppearance{};
    UAppearanceEx uAppearanceEx{};
};

static_assert(sizeof(STBaseCharInfo) == 0x40, "STBaseCharInfo size must match PDB");
static_assert(offsetof(STBaseCharInfo, strName) == 0x0, "STBaseCharInfo.strName offset mismatch");
static_assert(offsetof(STBaseCharInfo, byClass) == 0x2A, "STBaseCharInfo.byClass offset mismatch");
static_assert(offsetof(STBaseCharInfo, byAwaken) == 0x2B, "STBaseCharInfo.byAwaken offset mismatch");
static_assert(offsetof(STBaseCharInfo, dwProfilePhotoID) == 0x2C,
              "STBaseCharInfo.dwProfilePhotoID offset mismatch");
static_assert(offsetof(STBaseCharInfo, uAppearance) == 0x30,
              "STBaseCharInfo.uAppearance offset mismatch");
static_assert(offsetof(STBaseCharInfo, uAppearanceEx) == 0x38,
              "STBaseCharInfo.uAppearanceEx offset mismatch");

/**
 * @brief 能力值结构。
 */
struct STAbility {
    int nCurAbility[5] = {};
    int nMaxAbility[5] = {};
    float fMSR = 0.0f;
    float fASR = 0.0f;
};

static_assert(sizeof(STAbility) == 0x30, "STAbility size must match PDB");

/**
 * @brief 基础装备槽位。
 */
struct STEquipBase {
    STEquipBase() {
        Init();
    }

    void Init() {
        dwItemID = -1;
        byUpgrade = 0;
    }

    int dwItemID = -1;
    std::uint8_t byUpgrade = 0;
};

/**
 * @brief 形象装备条目。
 */
struct ST_EQUIP_ITEM_INFO {
    ST_EQUIP_ITEM_INFO() {
        Init();
    }

    void Init() {
        biSerial = -1;
        nItemID = -1;
        nDyeID = 0;
    }

    std::int64_t biSerial = -1;
    int nItemID = -1;
    int nDyeID = 0;
};

static_assert(sizeof(ST_EQUIP_ITEM_INFO) == 0x10, "ST_EQUIP_ITEM_INFO size must match PDB");

/**
 * @brief 称号信息。
 */
struct ST_TitleInfo {
    unsigned int dwPrefix = 0;
    unsigned int dwSuffix = 0;
};

/**
 * @brief 联盟名片设计联合体。
 */
union STLeagueCardInfo {
    struct CardParts {
        unsigned int dwDesign : 16;
        unsigned int dwBackDesign : 16;
    } parts;
    unsigned int nCard;

    constexpr STLeagueCardInfo() : nCard(0) {}
};

/**
 * @brief 联盟基础信息。
 */
struct STLeagueInfo {
    int nLeagueID = 0;
    wchar_t szLeagueName[10] = {};
    STLeagueCardInfo uCard{};
};

static_assert(sizeof(STLeagueInfo) == 0x1C, "STLeagueInfo size must match PDB");

/**
 * @brief 私人商店展示信息。
 */
struct STPrivateShopInfo {
    std::uint8_t byType = 0;
    wchar_t szTitle[21] = {};
};

static_assert(sizeof(STPrivateShopInfo) == 0x2C, "STPrivateShopInfo size must match PDB");

/**
 * @brief 角色 Buff 显示信息。
 */
struct STBuffInfo {
    int nBuffID = 0;
    float fTime = 0.0f;
    std::uint8_t byCount = 0;
    unsigned int dwOwnerID = 0;
    bool bShow = false;
};

static_assert(sizeof(STBuffInfo) == 0x14, "STBuffInfo size must match PDB");

/**
 * @brief 坐标与朝向信息。
 */
struct STPosInfo {
    STPosInfo() {
        sWorldID = -1;
        uxMapID.nMapID = 0;
        vPos = {};
        fRot = 0.0f;
    }

    void Init() {
        sWorldID = 0;
        uxMapID.nMapID = 0;
        vPos = {};
        fRot = 0.0f;
    }

    std::int16_t sWorldID = -1;
    UXMapID uxMapID{};
    XVec3 vPos{};
    float fRot = 0.0f;
};

/**
 * @brief PDB 中的完整 `STCharInfo` 字段集。
 *
 * 这一轮不再沿用早前裁剪过的最小结构，而是把当前链路已经用到的
 * `STCharInfo` 主体字段完整补回，避免角色选择/角色列表链继续建立在删减版结构之上。
 */
struct STCharInfo {
    STCharInfo() {
        Init();
    }

    void Init() {
        uxActorID.dwActorID = 0xFFFFFFFFu;
        std::fill(std::begin(GreenDamTan_gap_0C), std::end(GreenDamTan_gap_0C), 0);
        stBaseInfo = {};
        dwUAID = 0;
        byLevel = 1;
        byFaction = 0;
        std::fill(std::begin(GreenDamTan_gap_56), std::end(GreenDamTan_gap_56), 0);
        stAbility = {};
        stSoulWeapon.Init();
        stSubWeapon.Init();
        for (ST_EQUIP_ITEM_INFO& itemInfo : stShapeEquipItemInfo) {
            itemInfo.Init();
        }
        for (ST_EQUIP_ITEM_INFO& itemInfo : stLookEquipIemInfo) {
            itemInfo.Init();
        }
        dwActiveBroachEffect = 0;
        byGMPower = 0;
        std::fill(std::begin(GreenDamTan_gap_25D), std::end(GreenDamTan_gap_25D), 0);
        dwPvPKillCount = 0;
        stTitleInfo = {};
        stLeagueInfo = {};
        stShopInfo = {};
        shFP = 0;
        shBonusFP = 0;
        shPCBangFP = 0;
        byEchelonLevel = 0;
        GreenDamTan_gap_2BB[0] = 0;
        nEchelonExp = 0;
        bBattlePose = false;
        std::fill(std::begin(GreenDamTan_gap_2C1), std::end(GreenDamTan_gap_2C1), 0);
        dwStatus = 0;
        vecBuffInfo.clear();
        byCharSlotPos = 0;
        std::fill(std::begin(GreenDamTan_gap_2E9), std::end(GreenDamTan_gap_2E9), 0);
        nEqualizerID = 0;
    }

    void* __vftable = nullptr;
    UXActorID uxActorID{};
    std::uint8_t GreenDamTan_gap_0C[4] = {};
    STBaseCharInfo stBaseInfo{};
    unsigned int dwUAID = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t byFaction = 0;
    std::uint8_t GreenDamTan_gap_56[2] = {};
    STAbility stAbility{};
    STEquipBase stSoulWeapon{};
    STEquipBase stSubWeapon{};
    ST_EQUIP_ITEM_INFO stShapeEquipItemInfo[14] = {};
    ST_EQUIP_ITEM_INFO stLookEquipIemInfo[14] = {};
    unsigned int dwActiveBroachEffect = 0;
    std::uint8_t byGMPower = 0;
    std::uint8_t GreenDamTan_gap_25D[3] = {};
    unsigned int dwPvPKillCount = 0;
    ST_TitleInfo stTitleInfo{};
    STLeagueInfo stLeagueInfo{};
    STPrivateShopInfo stShopInfo{};
    std::int16_t shFP = 0;
    std::int16_t shBonusFP = 0;
    std::int16_t shPCBangFP = 0;
    std::uint8_t byEchelonLevel = 0;
    std::uint8_t GreenDamTan_gap_2BB[1] = {};
    int nEchelonExp = 0;
    bool bBattlePose = false;
    std::uint8_t GreenDamTan_gap_2C1[3] = {};
    unsigned int dwStatus = 0;
    std::vector<STBuffInfo> vecBuffInfo;
    std::uint8_t byCharSlotPos = 0;
    std::uint8_t GreenDamTan_gap_2E9[3] = {};
    int nEqualizerID = 0;
};

static_assert(sizeof(STCharInfo) == 0x2F0, "STCharInfo size must match PDB");
static_assert(offsetof(STCharInfo, __vftable) == 0x0, "STCharInfo.__vftable offset mismatch");
static_assert(offsetof(STCharInfo, uxActorID) == 0x8, "STCharInfo.uxActorID offset mismatch");
static_assert(offsetof(STCharInfo, stBaseInfo) == 0x10, "STCharInfo.stBaseInfo offset mismatch");
static_assert(offsetof(STCharInfo, dwUAID) == 0x50, "STCharInfo.dwUAID offset mismatch");
static_assert(offsetof(STCharInfo, byLevel) == 0x54, "STCharInfo.byLevel offset mismatch");
static_assert(offsetof(STCharInfo, byFaction) == 0x55, "STCharInfo.byFaction offset mismatch");
static_assert(offsetof(STCharInfo, stAbility) == 0x58, "STCharInfo.stAbility offset mismatch");
static_assert(offsetof(STCharInfo, stSoulWeapon) == 0x88, "STCharInfo.stSoulWeapon offset mismatch");
static_assert(offsetof(STCharInfo, stSubWeapon) == 0x90, "STCharInfo.stSubWeapon offset mismatch");
static_assert(offsetof(STCharInfo, stShapeEquipItemInfo) == 0x98,
              "STCharInfo.stShapeEquipItemInfo offset mismatch");
static_assert(offsetof(STCharInfo, stLookEquipIemInfo) == 0x178,
              "STCharInfo.stLookEquipIemInfo offset mismatch");
static_assert(offsetof(STCharInfo, dwActiveBroachEffect) == 0x258,
              "STCharInfo.dwActiveBroachEffect offset mismatch");
static_assert(offsetof(STCharInfo, byGMPower) == 0x25C, "STCharInfo.byGMPower offset mismatch");
static_assert(offsetof(STCharInfo, dwPvPKillCount) == 0x260,
              "STCharInfo.dwPvPKillCount offset mismatch");
static_assert(offsetof(STCharInfo, stTitleInfo) == 0x264,
              "STCharInfo.stTitleInfo offset mismatch");
static_assert(offsetof(STCharInfo, stLeagueInfo) == 0x26C,
              "STCharInfo.stLeagueInfo offset mismatch");
static_assert(offsetof(STCharInfo, stShopInfo) == 0x288, "STCharInfo.stShopInfo offset mismatch");
static_assert(offsetof(STCharInfo, shFP) == 0x2B4, "STCharInfo.shFP offset mismatch");
static_assert(offsetof(STCharInfo, shBonusFP) == 0x2B6, "STCharInfo.shBonusFP offset mismatch");
static_assert(offsetof(STCharInfo, shPCBangFP) == 0x2B8, "STCharInfo.shPCBangFP offset mismatch");
static_assert(offsetof(STCharInfo, byEchelonLevel) == 0x2BA,
              "STCharInfo.byEchelonLevel offset mismatch");
static_assert(offsetof(STCharInfo, nEchelonExp) == 0x2BC,
              "STCharInfo.nEchelonExp offset mismatch");
static_assert(offsetof(STCharInfo, bBattlePose) == 0x2C0,
              "STCharInfo.bBattlePose offset mismatch");
static_assert(offsetof(STCharInfo, dwStatus) == 0x2C4, "STCharInfo.dwStatus offset mismatch");
static_assert(offsetof(STCharInfo, vecBuffInfo) == 0x2C8,
              "STCharInfo.vecBuffInfo offset mismatch");
static_assert(offsetof(STCharInfo, byCharSlotPos) == 0x2E8,
              "STCharInfo.byCharSlotPos offset mismatch");
static_assert(offsetof(STCharInfo, nEqualizerID) == 0x2EC,
              "STCharInfo.nEqualizerID offset mismatch");

/**
 * @brief PDB 中的角色列表扩展信息。
 */
struct STCharInfoEx : STCharInfo {
    STCharInfoEx() {
        Init();
    }

    void Init() {
        STCharInfo::Init();
        stPosInfo.Init();
        fCurSuperArmorGage = 0.0f;
        fMaxSuperArmorGage = 0.0f;
    }

    STPosInfo stPosInfo{};
    float fCurSuperArmorGage = 0.0f;
    float fMaxSuperArmorGage = 0.0f;
};

static_assert(sizeof(STCharInfoEx) == 0x318, "STCharInfoEx size must match PDB");
static_assert(offsetof(STCharInfoEx, stPosInfo) == 0x2F0, "STCharInfoEx.stPosInfo offset mismatch");
static_assert(offsetof(STCharInfoEx, fCurSuperArmorGage) == 0x310,
              "STCharInfoEx.fCurSuperArmorGage offset mismatch");
static_assert(offsetof(STCharInfoEx, fMaxSuperArmorGage) == 0x314,
              "STCharInfoEx.fMaxSuperArmorGage offset mismatch");

struct STMyCharInfoEx_UserDBBits {
    unsigned int bLoadCharacter : 1;
    unsigned int bLoadInven : 1;
    unsigned int bLoadQuest : 1;
    unsigned int bLoadSoulmetry : 1;
    unsigned int bLoadWorld : 1;
    unsigned int bLoadQuickSlot : 1;
    unsigned int bLoadBlockList : 1;
    unsigned int bLoadBank : 1;
    unsigned int bLoadFriendList : 1;
    unsigned int bLoadFriendServer : 1;
    unsigned int bLoadItemExchange : 1;
    unsigned int bLoadGesture : 1;
    unsigned int bLoadTitle : 1;
    unsigned int bLoadFurniture : 1;
    unsigned int bLoadParty : 1;
    unsigned int bLoadForce : 1;
    unsigned int bLoadAchieve : 1;
    unsigned int bLoadAkashicRecord : 1;
    unsigned int bSocketInfo : 1;
    unsigned int bBroachInfo : 1;
    unsigned int bLoadUseInfo : 1;
    unsigned int bLoadNpcCredit : 1;
    unsigned int bLoadShopItem : 1;
    unsigned int bLoadCashCount : 1;
    unsigned int bLoadCashSet : 1;
    unsigned int bLoadAppearance : 1;
    unsigned int bLoadCharCommunity : 1;
    unsigned int bLoad_ALL : 1;
    unsigned int bLoadPostInfo : 1;
    unsigned int bLoadDailyMission : 1;
    unsigned int bLoadMyRoom : 1;
    unsigned int bLoadLevelUpEvent : 1;
    unsigned int bLoadNetCafe : 1;
    unsigned int bLoadSGAuth : 1;
    unsigned int bLoadGFAuth : 1;
    unsigned int bLoadWMAuth : 1;
    unsigned int bPackageInfo : 1;
};

union STMyCharInfoEx_UserDB {
    STMyCharInfoEx_UserDBBits UserDB;
    int nUserDB;

    constexpr STMyCharInfoEx_UserDB() : nUserDB(0) {}
};

struct STMyCharInfoEx_SyncUserBits {
    unsigned int bSyncCharacter : 1;
    unsigned int bSyncInven : 1;
    unsigned int bSyncQuest : 1;
    unsigned int bSyncSoulmetry : 1;
    unsigned int bSyncWorld : 1;
    unsigned int bSyncQuickSlot : 1;
    unsigned int bSyncBlockList : 1;
    unsigned int bSyncBank : 1;
    unsigned int bSyncAchieve : 1;
    unsigned int bSyncFriendList : 1;
    unsigned int bSyncItemExchange : 1;
    unsigned int bSyncGesture : 1;
    unsigned int bSyncTitle : 1;
    unsigned int bSyncFurniture : 1;
    unsigned int bSyncCashInfo : 1;
    unsigned int bSyncParty : 1;
    unsigned int bSyncAppearance : 1;
    unsigned int bSyncAkashicRecord : 1;
    unsigned int bSyncUseInfo : 1;
    unsigned int bSyncNpcCredit : 1;
    unsigned int bSyncShopItem : 1;
    unsigned int bSyncCharCommunity : 1;
    unsigned int bSyncPostRecvInfo : 1;
    unsigned int bSyncPostSendInfo : 1;
    unsigned int bSyncPostSaveInfo : 1;
    unsigned int bSyncPostAccountInfo : 1;
    unsigned int bSyncPollenInfo : 1;
    unsigned int bSyncMyRoom : 1;
    unsigned int bSyncNetCafe : 1;
    unsigned int bSyncSGAuth : 1;
};

union STMyCharInfoEx_SyncUser {
    STMyCharInfoEx_SyncUserBits SyncUser;
    int nSyncUser;

    constexpr STMyCharInfoEx_SyncUser() : nSyncUser(0) {}
};

/**
 * @brief PDB 中的完整 `STMyCharInfoEx` 字段集。
 */
struct STMyCharInfoEx : STCharInfoEx {
    STMyCharInfoEx() {
        Init();
    }

    void Init() {
        STCharInfoEx::Init();
        nExp = 0;
        biMoney = 0;
        biBP = 0;
        biEther = 0;
        biFriendPoint = 0;
        biRecycle = 0;
        byCommonStep = 0;
        byConsumeStep = 0;
        byCostumeStep = 0;
        byCardStep = 0;
        bNetCafe = false;
        nDyePoint = 0;
        nRenovatePoint = 0;
        nRefinePoint = 0;
        std::fill(std::begin(szAccountID), std::end(szAccountID), '\0');
        userDBBits.nUserDB = 0;
        syncUserBits.nSyncUser = 0;
    }

    std::int64_t nExp = 0;
    std::int64_t biMoney = 0;
    std::int64_t biBP = 0;
    std::int64_t biEther = 0;
    std::int64_t biFriendPoint = 0;
    std::int64_t biRecycle = 0;
    std::uint8_t byCommonStep = 0;
    std::uint8_t byConsumeStep = 0;
    std::uint8_t byCostumeStep = 0;
    std::uint8_t byCardStep = 0;
    bool bNetCafe = false;
    int nDyePoint = 0;
    int nRenovatePoint = 0;
    int nRefinePoint = 0;
    char szAccountID[21] = {};
    STMyCharInfoEx_UserDB userDBBits{};
    STMyCharInfoEx_SyncUser syncUserBits{};
};

static_assert(sizeof(STMyCharInfoEx) == 0x380, "STMyCharInfoEx size must match PDB");
static_assert(offsetof(STMyCharInfoEx, nExp) == 0x318, "STMyCharInfoEx.nExp offset mismatch");
static_assert(offsetof(STMyCharInfoEx, biMoney) == 0x320,
              "STMyCharInfoEx.biMoney offset mismatch");
static_assert(offsetof(STMyCharInfoEx, biBP) == 0x328, "STMyCharInfoEx.biBP offset mismatch");
static_assert(offsetof(STMyCharInfoEx, biEther) == 0x330,
              "STMyCharInfoEx.biEther offset mismatch");
static_assert(offsetof(STMyCharInfoEx, biFriendPoint) == 0x338,
              "STMyCharInfoEx.biFriendPoint offset mismatch");
static_assert(offsetof(STMyCharInfoEx, biRecycle) == 0x340,
              "STMyCharInfoEx.biRecycle offset mismatch");
static_assert(offsetof(STMyCharInfoEx, byCommonStep) == 0x348,
              "STMyCharInfoEx.byCommonStep offset mismatch");
static_assert(offsetof(STMyCharInfoEx, byConsumeStep) == 0x349,
              "STMyCharInfoEx.byConsumeStep offset mismatch");
static_assert(offsetof(STMyCharInfoEx, byCostumeStep) == 0x34A,
              "STMyCharInfoEx.byCostumeStep offset mismatch");
static_assert(offsetof(STMyCharInfoEx, byCardStep) == 0x34B,
              "STMyCharInfoEx.byCardStep offset mismatch");
static_assert(offsetof(STMyCharInfoEx, bNetCafe) == 0x34C,
              "STMyCharInfoEx.bNetCafe offset mismatch");
static_assert(offsetof(STMyCharInfoEx, nDyePoint) == 0x350,
              "STMyCharInfoEx.nDyePoint offset mismatch");
static_assert(offsetof(STMyCharInfoEx, nRenovatePoint) == 0x354,
              "STMyCharInfoEx.nRenovatePoint offset mismatch");
static_assert(offsetof(STMyCharInfoEx, nRefinePoint) == 0x358,
              "STMyCharInfoEx.nRefinePoint offset mismatch");
static_assert(offsetof(STMyCharInfoEx, szAccountID) == 0x35C,
              "STMyCharInfoEx.szAccountID offset mismatch");
static_assert(offsetof(STMyCharInfoEx, userDBBits) == 0x374,
              "STMyCharInfoEx.userDBBits offset mismatch");
static_assert(offsetof(STMyCharInfoEx, syncUserBits) == 0x37C,
              "STMyCharInfoEx.syncUserBits offset mismatch");

/**
 * @brief 登录服中缓存的账号角色列表容器。
 */
struct STUserInfo {
    int nUAID = 0;
    std::uint16_t wLastServerGroupID = 0;
    std::vector<STMyCharInfoEx> vSTCharInfo;
};

static_assert(sizeof(STUserInfo) == 0x28, "STUserInfo size must match PDB");

/**
 * @brief 角色所处地图/复活点映射信息。
 */
struct ST_CHARACTER_MAP_INFO {
    int nUCID = 0;
    int nMapID = 0;
    int nRevivePoint = 0;
    int nPrevMapID = 0;
    int nPrevRevivePoint = 0;
};

static_assert(sizeof(ST_CHARACTER_MAP_INFO) == 0x14, "ST_CHARACTER_MAP_INFO size must match PDB");

/**
 * @brief 账号下所有角色的地图映射列表。
 */
struct PS_CHARACTER_MAP_LIST {
    std::vector<ST_CHARACTER_MAP_INFO> vecInfo;
};

static_assert(sizeof(PS_CHARACTER_MAP_LIST) == 0x20, "PS_CHARACTER_MAP_LIST size must match PDB");

/**
 * @brief 单个角色的布罗奇外观槽信息。
 */
struct PS_BROACH_SHAPE {
    int dwItemID[15] = {};
};

static_assert(sizeof(PS_BROACH_SHAPE) == 0x3C, "PS_BROACH_SHAPE size must match PDB");

/**
 * @brief 布罗奇外观列表。
 */
struct PS_BROACH_SHAPE_LIST {
    std::vector<PS_BROACH_SHAPE> vecInfo;
};

static_assert(sizeof(PS_BROACH_SHAPE_LIST) == 0x20, "PS_BROACH_SHAPE_LIST size must match PDB");

/**
 * @brief 代表角色变更结果结构。
 *
 * 该结构由角色/数据库链路返回，包含：
 * - 账号/角色 ID
 * - 职业、等级、名称、头像
 * - 代表角色生效时间
 * - 错误码
 */
struct PS_CHARACTER_REPRESENTATIVE_CHANGE {
    unsigned int dwUAID = 0;
    unsigned int dwUCID = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byLevel = 0;
    wchar_t strName[21] = {};
    unsigned int dwProfilePhotoID = 0;
    std::int64_t nRepresentativeDate = 0;
    int nError = 0;
};

static_assert(sizeof(PS_CHARACTER_REPRESENTATIVE_CHANGE) == 0x48,
              "PS_CHARACTER_REPRESENTATIVE_CHANGE size must match PDB");

/**
 * @brief 客户端角色删角冷却剩余时间回包。
 */
struct PS_DELETE_CHARACTER_RES {
    int nRemainTime = 0;
};

static_assert(sizeof(PS_DELETE_CHARACTER_RES) == 0x4, "PS_DELETE_CHARACTER_RES size must match PDB");

/**
 * @brief 角色换槽请求/回包结构。
 */
struct PS_CHARACTER_CHANGE_SLOT {
    int nUAID = 0;
    int nSrcUCID = 0;
    int nDestUCID = 0;
    int nErrorCode = 0;
    std::uint8_t bySrcSlot = 0;
    std::uint8_t byDestSlot = 0;
    std::uint8_t byReserved0 = 0;
    std::uint8_t byReserved1 = 0;
};

static_assert(sizeof(PS_CHARACTER_CHANGE_SLOT) == 0x14,
              "PS_CHARACTER_CHANGE_SLOT size must match PDB");

/**
 * @brief 客户端角色名检测请求。
 */
struct PS_REQ_CHECK_NAME {
    wchar_t strName[21] = {};
};

static_assert(sizeof(PS_REQ_CHECK_NAME) == 0x2A, "PS_REQ_CHECK_NAME size must match PDB");

/**
 * @brief GameDB 返回的角色名检测结果。
 */
struct PS_RES_CHECK_NAME {
    wchar_t strName[21] = {};
    std::uint8_t byResult = 0;
    std::uint8_t byReserved = 0;
};

static_assert(sizeof(PS_RES_CHECK_NAME) == 0x2C, "PS_RES_CHECK_NAME size must match PDB");

/**
 * @brief 客户端发起的角色选择请求。
 *
 * PDB 已确认该结构包含：
 * - 目标角色 `dwActorID`
 * - 账号 `dwUAID`
 * - `bClearTutorial`
 * - 上一次地图与复活点
 *
 * 其中 `dwUAID`、`nPrevMapID`、`nPrevRevivePoint` 会在登录服侧进一步回填/修正。
 */
struct PS_CHARACTER_SELECT {
    unsigned int dwActorID = 0;
    unsigned int dwUAID = 0;
    bool bClearTutorial = false;
    int nPrevMapID = 0;
    int nPrevRevivePoint = 0;
};

struct ST_EXTEND_OPTION {
    std::int16_t byType = 0;
    int nOption = 0;
};

struct STItem {
    STItem() {
        Init();
    }

    void Init() {
        nItemID = -1;
        xSerial = -1;
        sCount = 0;
        bBindType = 0;
        byEndurance = 0;
        for (ST_EXTEND_OPTION& option : stExtendOption) {
            option.byType = 0;
            option.nOption = 0;
        }
        byUpgrade = 0;
        eFlag = 0;
        bySocketActiveCount = 0;
        nCashDate = 0;
        byUpgradeCount = 0;
        byUpgradeLimit = 0;
        nExp = 0;
        std::fill(std::begin(szBroachState), std::end(szBroachState), '\0');
        constexpr char kDefaultBroachState[] = "000000000000000";
        std::copy_n(kDefaultBroachState, sizeof(kDefaultBroachState) - 1, szBroachState);
        byRestoreCount = 0;
        bySealCount = 0;
        bySealDelCount = 0;
        nAttack = 0;
        nDefense = 0;
        nTitleID = 0;
        byUseCount = 0;
        nDyeID = 0;
    }

    int nItemID = -1;
    std::int64_t xSerial = -1;
    std::int16_t sCount = 0;
    std::uint8_t bBindType = 0;
    std::uint8_t byEndurance = 0;
    ST_EXTEND_OPTION stExtendOption[5] = {};
    std::uint8_t byUpgrade = 0;
    std::uint8_t eFlag = 0;
    std::uint8_t bySocketActiveCount = 0;
    std::int64_t nCashDate = 0;
    std::uint8_t byUpgradeCount = 0;
    std::uint8_t byUpgradeLimit = 0;
    int nExp = 0;
    char szBroachState[16] = {};
    std::uint8_t byRestoreCount = 0;
    std::uint8_t bySealCount = 0;
    std::uint8_t bySealDelCount = 0;
    int nAttack = 0;
    int nDefense = 0;
    int nTitleID = 0;
    std::uint8_t byUseCount = 0;
    int nDyeID = 0;
};

struct PS_DEFAULT_INVEN_ITEM {
    STItem stItem{};
    std::uint8_t byInvenType = 0;
    std::int16_t sPos = 0;
};

struct PS_DEFAULT_INVEN_ITEMS {
    std::vector<PS_DEFAULT_INVEN_ITEM> vecItems;
};

/**
 * @brief 物品栏单槽位索引。
 *
 * 该结构在 PDB 中被大量物品协议复用，
 * 当前先按 `PS_ITEM_SLOT_INFO` 的真实字段名与偏移恢复。
 */
struct PS_ITEM_SLOT_INFO {
    std::uint8_t byInvenType = 0;
    std::uint16_t shSlotPos = 0;
};

/**
 * @brief 槽位索引列表。
 *
 * PDB 中该结构本体是 `std::vector<PS_ITEM_SLOT_INFO>` 包装。
 * 由于 Linux 下 STL ABI 与 Windows/MSVC 不同，尺寸只在 Windows 侧强校验。
 */
struct PS_ITEM_SLOT_INFOS {
    std::vector<PS_ITEM_SLOT_INFO> vecInfo;
};

/**
 * @brief 物品存储项。
 *
 * `PS_STORAGE_INFO` 会把槽位信息与 `STItem` 实体打包在一起，
 * 是大量 ItemDB 回包复用的基础结构。
 */
struct PS_STORAGE_INFO {
    std::uint8_t byInvenType = 0;
    std::uint16_t shSlotPos = 0;
    STItem stItem{};
};

// TODO: 推测结果 - 对齐 IDA 0x1400CCB40 ResTradeResult 使用
struct ST_TRADE_ITEM {
    PS_STORAGE_INFO stInfo{};
};

// TODO: 推测结果 - 对齐 IDA 0x1400CCB40 ResTradeResult 使用
struct ST_TRADE_ITEM_LIST {
    std::vector<ST_TRADE_ITEM> vecTradeItem;
};

/**
 * @brief 物品存储结果列表。
 *
 * PDB 中 `byType` 位于 `std::vector<PS_STORAGE_INFO>` 之后，
 * 当前先保留真实字段顺序，便于后续继续接 ItemDB 回包链。
 */
struct PS_RES_STORAGE_INFO {
    std::vector<PS_STORAGE_INFO> vecItem;
    std::uint8_t byType = 0;
};

/**
 * @brief 客户端称号更换请求。
 *
 * 该结构已经在 PDB 中确认，但本轮重新核实 `LoginServer.exe` 后，
 * 仍未发现对应的上行处理函数或分发表 case。
 */
struct PS_REQ_ITEM_TITLE_CHANGE {
    std::uint32_t dwNpcID = 0;
    PS_ITEM_SLOT_INFO psSelectItem{};
    PS_ITEM_SLOT_INFO psTitleItem{};
    PS_ITEM_SLOT_INFOS psNeedItems{};
};

/**
 * @brief ItemDB 返回的称号更换结果。
 *
 * 当前仅确认了共享协议结构与字段布局，
 * `LoginServer.exe` 内尚未核实到对应的 DB 分发分支。
 */
struct PS_DB_ITEM_TITLE_CHANGE {
    std::uint32_t dwUCID = 0;
    PS_STORAGE_INFO psUpdateItemInfo{};
    PS_RES_STORAGE_INFO psUpdateItemList{};
    bool bSuccess = false;
    int nResult = 0;
};

/**
 * @brief 发给客户端的称号更换结果。
 *
 * 当前登录服二进制中尚未发现对应下行处理链，
 * 这里只先把 PDB 已确认的协议布局补回工程。
 */
struct PS_RES_ITEM_TITLE_CHANGE {
    std::int64_t biItemSerial = 0;
    int nTitleID = 0;
    bool bSuccess = false;
};

struct PS_DB_CHARACTER_CREATE {
    int nUAID = 0;
    STMyCharInfoEx stCharInfo{};
    std::uint16_t wDistrictID = 0;
    float fPosX = 0.0f;
    float fPosY = 0.0f;
    float fPosZ = 0.0f;
    int nDefulatSkill[20] = {};
    int nSkillDeck[6][4] = {};
    int nDefulatGesture[6] = {};
    int nDefaultConsume[2] = {};
};

// 以下结构对应建角成功后发往 StatisticsDB 的统计包。
// 当前字段集与序列化顺序只保留已经由 PDB / IDA 证实的部分。
struct ST_STATISTICS_CHARACTER_CREATE {
    unsigned int dwUAID = 0;
    unsigned int dwUCID = 0;
    wchar_t strName[21] = {};
    std::uint8_t byClass = 0;
    std::uint8_t byCount = 0;
    unsigned int dwItem1 = 0;
    unsigned int dwItem2 = 0;
};

struct ST_STATISTICS_SKILL {
    std::uint8_t byFlag = 0;
    unsigned int dwUCID = 0;
    unsigned int dwSkill_New = 0;
    unsigned int dwSkill_Old = 0;
    unsigned int dwDivergenceID = 0;
};

struct ST_STATISTICS_ITEM {
    std::uint8_t byFlag = 0;
    std::int64_t biSerial = 0;
    unsigned int dwUCID = 0;
    unsigned int dwItemID = 0;
    std::uint8_t byUpgrade = 0;
    std::uint8_t byUpgradeLimit = 0;
};

struct ST_STATISTICS_CHARACTER_SAVE {
    std::uint8_t byFlag = 0;
    unsigned int dwUCID = 0;
    std::uint8_t byLevel = 0;
    std::int64_t biMoney = 0;
    unsigned int dwPreFix = 0;
    unsigned int dwSufFix = 0;
    std::int64_t biBattlePoint = 0;
    std::int64_t biEther = 0;
    std::uint16_t shFP = 0;
    std::uint16_t shBonusFP = 0;
    int nQuickSlotItem[4] = {};
    std::int64_t biQuickSlotCard[5] = {};
    std::uint16_t shClearChapter = 0;
    std::uint16_t shClearStage = 0;
};

/**
 * @brief LogDB 细日志结构。
 *
 * 注意：
 * - `rabin2-LoginServer.pdb.txt` 中的 `wchar_t[202] / [102]` 是按字节数显示；
 * - 结合偏移与 `wcscpy_s(..., 0x33u, ...)` 可确认真实字段应恢复为
 *   `wchar_t[101] / [51] / [51]`。
 */
struct ST_LOG_GAME {
    int _nUAID = 0;
    int _nUCID = 0;
    std::uint16_t _sMainType = 0;
    std::uint16_t _sSubType = 0;
    int nParam0 = 0;
    int nParam1 = 0;
    int nParam2 = 0;
    int nParam3 = 0;
    int nParam4 = 0;
    std::int64_t nParam5 = 0;
    std::int64_t nParam6 = 0;
    int nParam7 = 0;
    int nParam8 = 0;
    std::int64_t nParam9 = 0;
    wchar_t szParam10[101] = {};
    wchar_t szComment[51] = {};
    wchar_t szComment2[51] = {};
    int nParam11 = 0;
    int nParam12 = 0;
    int nWorld_Idx = 0;
};

// 对齐 IDA: 聊天日志结构（DB main=0x42, sub=9）
struct ST_CHAT_LOG_GAME {
    int nUAID = 0;
    int nUCID = 0;
    std::int16_t sType = 0;
    std::uint16_t _pad0 = 0;
    int nParam0 = 0;
    int nParam1 = 0;
    int nParam2 = 0;
    int nParam3 = 0;
    int nParam4 = 0;
    std::int64_t nParam5 = 0;
    std::int64_t nParam6 = 0;
    wchar_t szComment[257] = {};
};

// 对齐 IDA: 统计日志结构（DB main=0x42, sub=3）
struct ST_STAT_LOG_GAME {
    int nUAID = 0;
    int nUCID = 0;
    float fParam0 = 0.0f;
    float fParam1 = 0.0f;
    float fParam2 = 0.0f;
    float fParam3 = 0.0f;
    float fParam4 = 0.0f;
    float fParam5 = 0.0f;
    float fParam6 = 0.0f;
    float fParam7 = 0.0f;
    float fParam8 = 0.0f;
};

// 对齐 IDA: KRR 怪物信息结构（DB main=0x01, sub=1/2）
struct ST_KRR_MONSTER_INFO {
    std::uint32_t dwMonsterID = 0;
    std::uint32_t dwTableID = 0;
    std::uint8_t byChannel = 0;
    std::uint8_t _pad0[3] = {};
    float xPos = 0.0f;
    float yPos = 0.0f;
    float zPos = 0.0f;
    std::uint64_t dwRemoveTime = 0;
};

// 对齐 IDA: 文本日志结构（DB main=0x42, sub=4）
struct ST_LOG_TEXT {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    std::int16_t shMainType = 0;
    std::int16_t shSubType = 0;
    int nMapID = 0;
    std::int64_t nInstanceID = 0;
    char szMsg[8000] = {};
};

// 对齐 IDA: 金币日志结构（DB main=0x42, sub=6）
struct ST_LOG_MONEY {
    std::uint32_t dwUCID = 0;
    std::int64_t biIncMoney = 0;
    std::int64_t biDescMoney = 0;
};

// 对齐 IDA: SG角色日志结构（DB main=0x42, sub=8）
struct ST_LOG_SG_CHAR {
    char szMsg[1024] = {};
};

// 对齐 IDA: 现金日志结构（DB main=0x42, sub=0x22）
struct PS_LOG_CASH {
    std::int32_t nUAID = 0;
    std::int32_t nUCID = 0;
    std::int64_t nOrderNo = 0;
    std::int32_t nParam1 = 0;
    std::int32_t nParam2 = 0;
    std::int32_t nParam3 = 0;
    std::int32_t nParam4 = 0;
    std::int32_t nParam5 = 0;
    std::int32_t nParam6 = 0;
    char szBillCode[21] = {};
};

// 对齐 IDA: 角色连接服务器日志结构（DB main=0x42, sub=0x16）
struct PS_LOG_CHARACTER_CONNECT_SERVER {
    std::uint32_t dwUCID = 0;
    std::int32_t nServerID = 0;
    std::uint8_t byType = 0;
};

// 对齐 IDA: 客户端日志结构（DB main=0x42, sub=0x1B）
struct ST_CLIENT_LOG {
    std::uint8_t byType = 0;
    char szLog[128] = {};
};

// ST_SG_AUTH_INFO 已在文件前面定义（约第116行）

static_assert(sizeof(STItem) == 0x78, "STItem size must match PDB");
static_assert(sizeof(PS_DEFAULT_INVEN_ITEM) == 0x80,
              "PS_DEFAULT_INVEN_ITEM size must match PDB");
static_assert(sizeof(PS_DEFAULT_INVEN_ITEMS) == 0x20,
              "PS_DEFAULT_INVEN_ITEMS size must match PDB");
static_assert(sizeof(PS_ITEM_SLOT_INFO) == 0x4,
              "PS_ITEM_SLOT_INFO size must match PDB");
static_assert(sizeof(PS_STORAGE_INFO) == 0x80,
              "PS_STORAGE_INFO size must match PDB");
static_assert(offsetof(PS_STORAGE_INFO, stItem) == 0x8,
              "PS_STORAGE_INFO.stItem offset mismatch");
static_assert(sizeof(PS_RES_ITEM_TITLE_CHANGE) == 0x10,
              "PS_RES_ITEM_TITLE_CHANGE size must match PDB");
static_assert(sizeof(ST_STATISTICS_ITEM) == 0x20,
              "ST_STATISTICS_ITEM size must match PDB");
static_assert(sizeof(ST_STATISTICS_CHARACTER_SAVE) == 0x78,
              "ST_STATISTICS_CHARACTER_SAVE size must match PDB");
#ifdef _WIN32
static_assert(sizeof(PS_ITEM_SLOT_INFOS) == 0x20,
              "PS_ITEM_SLOT_INFOS size must match PDB");
static_assert(sizeof(PS_RES_STORAGE_INFO) == 0x28,
              "PS_RES_STORAGE_INFO size must match PDB");
static_assert(offsetof(PS_RES_STORAGE_INFO, byType) == 0x20,
              "PS_RES_STORAGE_INFO.byType offset mismatch");
static_assert(sizeof(PS_REQ_ITEM_TITLE_CHANGE) == 0x30,
              "PS_REQ_ITEM_TITLE_CHANGE size must match PDB");
static_assert(offsetof(PS_REQ_ITEM_TITLE_CHANGE, psNeedItems) == 0x10,
              "PS_REQ_ITEM_TITLE_CHANGE.psNeedItems offset mismatch");
static_assert(sizeof(PS_DB_ITEM_TITLE_CHANGE) == 0xB8,
              "PS_DB_ITEM_TITLE_CHANGE size must match PDB");
static_assert(sizeof(ST_LOG_GAME) == 0x1E8, "ST_LOG_GAME size must match PDB");
static_assert(offsetof(ST_LOG_GAME, szParam10) == 0x40, "ST_LOG_GAME.szParam10 offset mismatch");
static_assert(offsetof(ST_LOG_GAME, szComment) == 0x10A, "ST_LOG_GAME.szComment offset mismatch");
static_assert(offsetof(ST_LOG_GAME, szComment2) == 0x170, "ST_LOG_GAME.szComment2 offset mismatch");
static_assert(offsetof(ST_LOG_GAME, nParam11) == 0x1D8, "ST_LOG_GAME.nParam11 offset mismatch");
static_assert(offsetof(ST_LOG_GAME, nWorld_Idx) == 0x1E0, "ST_LOG_GAME.nWorld_Idx offset mismatch");
#endif

struct SS_UPDATE_SERVER_INFO {
    std::uint32_t dwID = 0;
    std::uint16_t nState = 0;
    std::uint16_t nCurUser = 0;
};

enum CHANNEL_STATE : std::int32_t {
    CHANNEL_STATE_LOW = 0,
    CHANNEL_STATE_NORMAL = 1,
    CHANNEL_STATE_HIGH = 2,
    CHANNEL_STATE_FULL = 3,
};

struct ST_CHANNEL_INFO {
    std::uint16_t wChannel = 0;
    std::uint8_t byChannelState = 0;
    std::uint8_t _pad0 = 0;
};

struct PS_CHANNEL_INFO {
    std::uint16_t wMapID = 0;
    std::uint8_t _pad0[6] = {};
    std::vector<ST_CHANNEL_INFO> vecChannel;
};

// 对齐 IDA: ST_CHANNEL_INFO 包序列化
inline XPacket& operator<<(XPacket& packet, const ST_CHANNEL_INFO& value) {
    packet.XParse << value.wChannel;
    packet.XParse << value.byChannelState;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_CHANNEL_INFO& value) {
    packet.XParse >> value.wChannel;
    packet.XParse >> value.byChannelState;
    return packet;
}

// 对齐 IDA: PS_CHANNEL_INFO 包序列化
inline XPacket& operator>>(XPacket& packet, PS_CHANNEL_INFO& value) {
    packet.XParse >> value.wMapID;
    // 跳过 padding
    std::uint8_t pad[6] = {};
    for (int i = 0; i < 6; ++i) {
        packet.XParse >> pad[i];
    }
    // 读取频道列表
    std::uint16_t wCount = 0;
    packet.XParse >> wCount;
    value.vecChannel.resize(wCount);
    for (std::uint16_t i = 0; i < wCount; ++i) {
        packet >> value.vecChannel[i];
    }
    return packet;
}

struct SS_CHANGE_CHANNEL_REQ {
    std::uint16_t sChannel = 0;
};

struct STChannelInfo {
    char szIP[513] = {};
    char _pad0 = '\0';
    std::uint16_t sPort = 0;
};

struct SS_REPORT_POOL_INFO {
    int nMonsterCurSize = 0;
    int nMonsterFullSize = 0;
    int nNpcCurSize = 0;
    int nNpcFullSize = 0;
    int nAkashicCurSize = 0;
    int nAkashicFullSize = 0;
    int nProjectileCurSize = 0;
    int nProjectileFullSize = 0;
    int nTrapCurSize = 0;
    int nTrapFullSize = 0;
    int nChainLightningCurSize = 0;
    int nChainLightningFullSize = 0;
    int nInteractionCurSize = 0;
    int nInteractionFullSize = 0;
    int nVaccumCubeCurSize = 0;
    int nVaccumCubeFullSize = 0;
    int nMazeCurSize = 0;
    int nMazeFullSize = 0;
    int nMyRoomCurSize = 0;
    int nMyRoomFullSize = 0;
    int nSocialCurSize = 0;
    int nSocialFullSize = 0;
};

struct SS_REPORT_CONNECT_INFO {
    bool bControlConnect = false;
    bool bCommunityConnect = false;
    wchar_t szLogicThread[33] = {};
};

struct SS_REPORT_SERVER_STATUS {
    int nServerType = 0;
    std::uint32_t dwServerID = 0;
    std::uint32_t dwProcessID = 0;
    int nUserCount = 0;
    char szIP[513] = {};
    int nPort = 0;
    bool bNetCafe = false;
    char _pad0[3] = {};
    SS_REPORT_POOL_INFO poolInfo{};
    SS_REPORT_CONNECT_INFO connectInfo{};
};

#ifdef _WIN32
static_assert(sizeof(SS_UPDATE_SERVER_INFO) == 0x8, "SS_UPDATE_SERVER_INFO size must match PDB");
static_assert(sizeof(CHANNEL_STATE) == 0x4, "CHANNEL_STATE size must match PDB");
static_assert(sizeof(ST_CHANNEL_INFO) == 0x4, "ST_CHANNEL_INFO size must match PDB");
static_assert(offsetof(ST_CHANNEL_INFO, byChannelState) == 0x2,
              "ST_CHANNEL_INFO.byChannelState offset mismatch");
static_assert(sizeof(PS_CHANNEL_INFO) == 0x28, "PS_CHANNEL_INFO size must match PDB");
static_assert(offsetof(PS_CHANNEL_INFO, vecChannel) == 0x8,
              "PS_CHANNEL_INFO.vecChannel offset mismatch");
static_assert(sizeof(SS_CHANGE_CHANNEL_REQ) == 0x2, "SS_CHANGE_CHANNEL_REQ size must match PDB");
static_assert(sizeof(STChannelInfo) == 0x204, "STChannelInfo size must match PDB");
static_assert(offsetof(STChannelInfo, sPort) == 0x202, "STChannelInfo.sPort offset mismatch");
static_assert(sizeof(SS_REPORT_POOL_INFO) == 0x58, "SS_REPORT_POOL_INFO size must match PDB");
static_assert(sizeof(SS_REPORT_CONNECT_INFO) == 0x44, "SS_REPORT_CONNECT_INFO size must match PDB");
static_assert(sizeof(SS_REPORT_SERVER_STATUS) == 0x2B8, "SS_REPORT_SERVER_STATUS size must match PDB");
static_assert(offsetof(SS_REPORT_SERVER_STATUS, szIP) == 0x10, "SS_REPORT_SERVER_STATUS.szIP offset mismatch");
static_assert(offsetof(SS_REPORT_SERVER_STATUS, nPort) == 0x214, "SS_REPORT_SERVER_STATUS.nPort offset mismatch");
static_assert(offsetof(SS_REPORT_SERVER_STATUS, poolInfo) == 0x21C, "SS_REPORT_SERVER_STATUS.poolInfo offset mismatch");
static_assert(offsetof(SS_REPORT_SERVER_STATUS, connectInfo) == 0x274, "SS_REPORT_SERVER_STATUS.connectInfo offset mismatch");
#endif

inline void operator>>(XPacket& packet, PS_SECOND_PW_REQ& value) {
    packet.XParse >> value.byCheckType;
    short outLen = 0;
    packet.XParse.GetString(value.strPassword, 7, &outLen);
}

inline XPacket& operator<<(XPacket& packet, const PS_SECOND_PW_REQ& value) {
    packet.XParse << value.byCheckType;
    packet.XParse << std::string(value.strPassword);
    return packet;
}

inline void operator>>(XPacket& packet, PS_KICK_USER_INFO& value) {
    short outLen = 0;
    packet.XParse >> value.byKickType;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.nParam;
    packet.XParse.GetWString(value.strMsg, 513, outLen);
}

inline XPacket& operator<<(XPacket& packet, const PS_KICK_USER_INFO& value) {
    packet.XParse << value.byKickType;
    packet.XParse << value.dwUAID;
    packet.XParse << value.nParam;
    packet.XParse << FixedWideArrayToWString(value.strMsg);
    return packet;
}

inline void operator>>(XPacket& packet, PS_LOGIN_RES& value) {
    short outLen = 0;
    packet.XParse >> value.nUAID;
    packet.XParse >> value.bClearTutorial;
    packet.XParse.GetBytes(value.szMacAddress, sizeof(value.szMacAddress));
    packet.XParse.GetWString(value.szLoginResultMsg, 1025, outLen);
    packet.XParse >> value.nErrorCode;
    packet.XParse >> value.byLoginType;
    packet.XParse.GetWString(value.szAuthID, 21, outLen);
    packet.XParse >> value.biAuthSessionID;
    packet.XParse >> value.byGMPower;
    packet.XParse >> value.wBirth_Year;
    packet.XParse >> value.byBirth_Month;
    packet.XParse >> value.byBirth_Day;
}

inline XPacket& operator<<(XPacket& packet, const PS_LOGIN_RES& value) {
    packet.XParse << value.nUAID;
    packet.XParse << value.bClearTutorial;
    packet.XParse.SetBytes(value.szMacAddress, sizeof(value.szMacAddress));
    packet.XParse << FixedWideArrayToWString(value.szLoginResultMsg);
    packet.XParse << value.nErrorCode;
    packet.XParse << value.byLoginType;
    packet.XParse << FixedWideArrayToWString(value.szAuthID);
    packet.XParse << value.biAuthSessionID;
    packet.XParse << value.byGMPower;
    packet.XParse << value.wBirth_Year;
    packet.XParse << value.byBirth_Month;
    packet.XParse << value.byBirth_Day;
    return packet;
}

inline void operator>>(XPacket& packet, PS_CHARACTER_SELECT& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.bClearTutorial;
    packet.XParse >> value.nPrevMapID;
    packet.XParse >> value.nPrevRevivePoint;
}

inline XPacket& operator<<(XPacket& packet, const PS_CHARACTER_SELECT& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.bClearTutorial;
    packet.XParse << value.nPrevMapID;
    packet.XParse << value.nPrevRevivePoint;
    return packet;
}

inline void operator>>(XPacket& packet, ST_EXTEND_OPTION& value) {
    packet.XParse >> value.byType;
    packet.XParse >> value.nOption;
}

inline XPacket& operator<<(XPacket& packet, const ST_EXTEND_OPTION& value) {
    packet.XParse << value.byType;
    packet.XParse << value.nOption;
    return packet;
}

inline void operator>>(XPacket& packet, STItem& value) {
    short outLen = 0;
    packet.XParse >> value.nItemID;
    packet.XParse >> value.xSerial;
    packet.XParse >> value.sCount;
    packet.XParse >> value.bBindType;
    for (ST_EXTEND_OPTION& extendOption : value.stExtendOption) {
        packet >> extendOption;
    }
    packet.XParse >> value.byUpgrade;
    packet.XParse >> value.byEndurance;
    packet.XParse >> value.bySocketActiveCount;
    packet.XParse >> value.nCashDate;
    packet.XParse >> value.byUpgradeCount;
    packet.XParse >> value.byUpgradeLimit;
    packet.XParse >> value.eFlag;
    packet.XParse >> value.nExp;
    packet.XParse.GetString(value.szBroachState, 16, &outLen);
    packet.XParse >> value.byRestoreCount;
    packet.XParse >> value.bySealCount;
    packet.XParse >> value.bySealDelCount;
    packet.XParse >> value.nAttack;
    packet.XParse >> value.nDefense;
    packet.XParse >> value.nTitleID;
    packet.XParse >> value.byUseCount;
    packet.XParse >> value.nDyeID;
}

inline void operator>>(XPacket& packet, PS_STORAGE_INFO& value) {
    packet.XParse >> value.byInvenType;
    packet.XParse >> value.shSlotPos;
    packet >> value.stItem;
}

inline void operator>>(XPacket& packet, PS_RES_STORAGE_INFO& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecItem.clear();
    value.vecItem.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        PS_STORAGE_INFO item{};
        packet >> item;
        value.vecItem.push_back(std::move(item));
    }
    packet.XParse >> value.byType;
}

inline XPacket& operator<<(XPacket& packet, const STItem& value) {
    packet.XParse << value.nItemID;
    packet.XParse << value.xSerial;
    packet.XParse << value.sCount;
    packet.XParse << value.bBindType;
    for (const ST_EXTEND_OPTION& extendOption : value.stExtendOption) {
        packet << extendOption;
    }
    packet.XParse << value.byUpgrade;
    packet.XParse << value.byEndurance;
    packet.XParse << value.bySocketActiveCount;
    packet.XParse << value.nCashDate;
    packet.XParse << value.byUpgradeCount;
    packet.XParse << value.byUpgradeLimit;
    packet.XParse << value.eFlag;
    packet.XParse << value.nExp;
    packet.XParse << std::string(value.szBroachState);
    packet.XParse << value.byRestoreCount;
    packet.XParse << value.bySealCount;
    packet.XParse << value.bySealDelCount;
    packet.XParse << value.nAttack;
    packet.XParse << value.nDefense;
    packet.XParse << value.nTitleID;
    packet.XParse << value.byUseCount;
    packet.XParse << value.nDyeID;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_STORAGE_INFO& value) {
    packet.XParse << value.byInvenType;
    packet.XParse << value.shSlotPos;
    packet << value.stItem;
    return packet;
}

// TODO: 推测结果 - 对齐 IDA 0x1400CCB40 ResTradeResult 序列化
inline void operator>>(XPacket& packet, ST_TRADE_ITEM& value) {
    packet >> value.stInfo;
}

inline XPacket& operator<<(XPacket& packet, const ST_TRADE_ITEM& value) {
    packet << value.stInfo;
    return packet;
}

inline void operator>>(XPacket& packet, ST_TRADE_ITEM_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecTradeItem.resize(count);
    for (std::uint8_t i = 0; i < count; ++i) {
        packet >> value.vecTradeItem[i];
    }
}

inline XPacket& operator<<(XPacket& packet, const ST_TRADE_ITEM_LIST& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecTradeItem.size());
    for (const ST_TRADE_ITEM& item : value.vecTradeItem) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_STORAGE_INFO& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecItem.size());
    for (const PS_STORAGE_INFO& item : value.vecItem) {
        packet << item;
    }
    packet.XParse << value.byType;
    return packet;
}

// 对齐 IDA: XSendDBPacket 输出操作符
inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_RES_STORAGE_INFO& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecItem.size());
    for (const PS_STORAGE_INFO& item : value.vecItem) {
        packet << item;
    }
    packet.XParse << value.byType;
    return packet;
}

inline void operator>>(XPacket& packet, PS_DEFAULT_INVEN_ITEM& value) {
    packet >> value.stItem;
    packet.XParse >> value.byInvenType;
    packet.XParse >> value.sPos;
}

inline XPacket& operator<<(XPacket& packet, const PS_DEFAULT_INVEN_ITEM& value) {
    packet << value.stItem;
    packet.XParse << value.byInvenType;
    packet.XParse << value.sPos;
    return packet;
}

inline void operator>>(XPacket& packet, PS_DEFAULT_INVEN_ITEMS& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecItems.clear();
    value.vecItems.reserve(count);
    for (std::uint8_t index = 0; index < count; ++index) {
        PS_DEFAULT_INVEN_ITEM item{};
        packet >> item;
        value.vecItems.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_DEFAULT_INVEN_ITEMS& value) {
    const std::uint8_t count = static_cast<std::uint8_t>(
        std::min<std::size_t>(value.vecItems.size(), 0xFF));
    packet.XParse << count;
    for (std::size_t index = 0; index < count; ++index) {
        packet << value.vecItems[index];
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_SECOND_PW_RES& value) {
    packet.XParse >> value.nErrorID;
    packet.XParse >> value.bySecondPWState;
}

inline XPacket& operator<<(XPacket& packet, const PS_SECOND_PW_RES& value) {
    packet.XParse << value.nErrorID;
    packet.XParse << value.bySecondPWState;
    return packet;
}

inline void operator>>(XPacket& packet, PS_TRADE_PW_REQ& value) {
    packet.XParse.GetString(value.strPassword, 5, nullptr);
}

inline XPacket& operator<<(XPacket& packet, const PS_TRADE_PW_REQ& value) {
    packet.XParse << std::string(value.strPassword);
    return packet;
}

inline void operator>>(XPacket& packet, PS_TRADE_PW_RES& value) {
    packet.XParse >> value.nErrorID;
    packet.XParse >> value.byTradePWState;
}

inline XPacket& operator<<(XPacket& packet, const PS_TRADE_PW_RES& value) {
    packet.XParse << value.nErrorID;
    packet.XParse << value.byTradePWState;
    return packet;
}

inline void operator>>(XPacket& packet, ST_SG_AUTH_INFO& value) {
    short outLen = 0;
    packet.XParse >> value.nUAID;
    packet.XParse >> value.byAuthType;
    packet.XParse.GetString(value.szToken, 1025, &outLen);
    packet.XParse.GetString(value.szRefreshToken, 1025, &outLen);
    packet.XParse >> value.nExpireTime;
    packet.XParse >> value.nBirth;
}

inline XPacket& operator<<(XPacket& packet, const ST_SG_AUTH_INFO& value) {
    packet.XParse << value.nUAID;
    packet.XParse << value.byAuthType;
    packet.XParse << std::string(value.szToken);
    packet.XParse << std::string(value.szRefreshToken);
    packet.XParse << value.nExpireTime;
    packet.XParse << value.nBirth;
    return packet;
}

inline void operator>>(XPacket& packet, ST_GF_AUTH_INFO& value) {
    short outLen = 0;
    packet.XParse >> value.nUAID;
    packet.XParse.GetString(value.szToken, 1025, &outLen);
    packet.XParse.GetString(value.szRefreshToken, 1025, &outLen);
    packet.XParse >> value.nExpireTime;
    packet.XParse >> value.nBirth;
    packet.XParse.GetString(value.szGFAccountID, 256, &outLen);
    packet.XParse.GetString(value.szGFClientID, 256, &outLen);
    packet.XParse.GetString(value.szDisplayName, 21, &outLen);
}

inline XPacket& operator<<(XPacket& packet, const ST_GF_AUTH_INFO& value) {
    packet.XParse << value.nUAID;
    packet.XParse << std::string(value.szToken);
    packet.XParse << std::string(value.szRefreshToken);
    packet.XParse << value.nExpireTime;
    packet.XParse << value.nBirth;
    packet.XParse << std::string(value.szGFAccountID);
    packet.XParse << std::string(value.szGFClientID);
    packet.XParse << std::string(value.szDisplayName);
    return packet;
}

inline void operator>>(XPacket& packet, PS_XIGNCODE_UPDATE& value) {
    packet.XParse >> value.session;
    packet.XParse.GetBytes(value.szBuff, sizeof(value.szBuff));
}

inline XPacket& operator<<(XPacket& packet, const PS_XIGNCODE_UPDATE& value) {
    packet.XParse << value.session;
    packet.XParse.SetBytes(value.szBuff, sizeof(value.szBuff));
    return packet;
}

inline void operator>>(XPacket& packet, PS_XIGNCODE_ERROR& value) {
    packet.XParse >> value.Code;
    packet.XParse >> value.w;
}

inline void operator>>(XPacket& packet, PS_SG_TOKEN_UPDATE& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.szAcessToken, 1025, outLen);
    packet.XParse.GetWString(value.szRefreshToken, 1025, outLen);
    packet.XParse >> value.bRenew;
}

inline void operator>>(XPacket& packet, PS_GAME_GUARD_AUTH& value) {
    packet.XParse.GetBytes(reinterpret_cast<char*>(value.bPacket), sizeof(value.bPacket));
    packet.XParse >> value.dwServerNumber;
    packet.XParse >> value.dwPacketSize;
}

inline void operator>>(XPacket& packet, PS_GAME_GUARD_ERROR& value) {
    packet.XParse >> value.dwErrorCode;
}

inline void operator>>(XPacket& packet, UXActorID& value) {
    packet.XParse >> value.dwActorID;
}

inline XPacket& operator<<(XPacket& packet, const UXActorID& value) {
    packet.XParse << value.dwActorID;
    return packet;
}

inline void operator>>(XPacket& packet, UAppearanceEx& value) {
    packet.XParse >> value.biAppearance;
}

inline XPacket& operator<<(XPacket& packet, const UAppearanceEx& value) {
    packet.XParse << value.biAppearance;
    return packet;
}

inline void operator>>(XPacket& packet, XVec3& value) {
    packet.XParse >> value.x;
    packet.XParse >> value.y;
    packet.XParse >> value.z;
}

inline XPacket& operator<<(XPacket& packet, const XVec3& value) {
    packet.XParse << value.x;
    packet.XParse << value.y;
    packet.XParse << value.z;
    return packet;
}

inline void operator>>(XPacket& packet, STBaseCharInfo& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.uAppearance.biAppearance;
    packet.XParse >> value.uAppearanceEx.biAppearance;
}

inline XPacket& operator<<(XPacket& packet, const STBaseCharInfo& value) {
    packet.XParse << FixedWideArrayToWString(value.strName);
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.uAppearance.biAppearance;
    packet.XParse << value.uAppearanceEx.biAppearance;
    return packet;
}

inline void operator>>(XPacket& packet, STAbility& value) {
    for (int index = 0; index < 5; ++index) {
        packet.XParse >> value.nCurAbility[index];
        packet.XParse >> value.nMaxAbility[index];
    }
    packet.XParse >> value.fMSR;
    packet.XParse >> value.fASR;
}

inline XPacket& operator<<(XPacket& packet, const STAbility& value) {
    for (int index = 0; index < 5; ++index) {
        packet.XParse << value.nCurAbility[index];
        packet.XParse << value.nMaxAbility[index];
    }
    packet.XParse << value.fMSR;
    packet.XParse << value.fASR;
    return packet;
}

inline void operator>>(XPacket& packet, STEquipBase& value) {
    packet.XParse >> value.byUpgrade;
    packet.XParse >> value.dwItemID;
}

inline XPacket& operator<<(XPacket& packet, const STEquipBase& value) {
    packet.XParse << value.byUpgrade;
    packet.XParse << value.dwItemID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_EQUIP_ITEM_INFO& value) {
    packet.XParse >> value.biSerial;
    packet.XParse >> value.nItemID;
    packet.XParse >> value.nDyeID;
}

inline XPacket& operator<<(XPacket& packet, const ST_EQUIP_ITEM_INFO& value) {
    packet.XParse << value.biSerial;
    packet.XParse << value.nItemID;
    packet.XParse << value.nDyeID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_TitleInfo& value) {
    packet.XParse >> value.dwPrefix;
    packet.XParse >> value.dwSuffix;
}

inline XPacket& operator<<(XPacket& packet, const ST_TitleInfo& value) {
    packet.XParse << value.dwPrefix;
    packet.XParse << value.dwSuffix;
    return packet;
}

inline void operator>>(XPacket& packet, STLeagueInfo& value) {
    short outLen = 0;
    packet.XParse >> value.nLeagueID;
    packet.XParse.GetWString(value.szLeagueName, 10, outLen);
    packet.XParse >> value.uCard.nCard;
}

inline XPacket& operator<<(XPacket& packet, const STLeagueInfo& value) {
    packet.XParse << value.nLeagueID;
    packet.XParse << FixedWideArrayToWString(value.szLeagueName);
    packet.XParse << value.uCard.nCard;
    return packet;
}

inline void operator>>(XPacket& packet, STPrivateShopInfo& value) {
    short outLen = 0;
    packet.XParse >> value.byType;
    packet.XParse.GetWString(value.szTitle, 21, outLen);
}

inline XPacket& operator<<(XPacket& packet, const STPrivateShopInfo& value) {
    packet.XParse << value.byType;
    packet.XParse << FixedWideArrayToWString(value.szTitle);
    return packet;
}

inline void operator>>(XPacket& packet, STBuffInfo& value) {
    packet.XParse >> value.nBuffID;
    packet.XParse >> value.fTime;
    packet.XParse >> value.byCount;
    packet.XParse >> value.dwOwnerID;
    packet.XParse >> value.bShow;
}

inline XPacket& operator<<(XPacket& packet, const STBuffInfo& value) {
    packet.XParse << value.nBuffID;
    packet.XParse << value.fTime;
    packet.XParse << value.byCount;
    packet.XParse << value.dwOwnerID;
    packet.XParse << value.bShow;
    return packet;
}

inline void operator>>(XPacket& packet, STPosInfo& value) {
    packet.XParse >> value.sWorldID;
    packet.XParse >> value.uxMapID.nMapID;
    packet >> value.vPos;
    packet.XParse >> value.fRot;
}

inline XPacket& operator<<(XPacket& packet, const STPosInfo& value) {
    packet.XParse << value.sWorldID;
    packet.XParse << value.uxMapID.nMapID;
    packet << value.vPos;
    packet.XParse << value.fRot;
    return packet;
}

inline void operator>>(XPacket& packet, STCharInfo& value) {
    packet >> value.uxActorID;
    packet >> value.stBaseInfo;
    packet.XParse >> value.byLevel;
    packet.XParse >> value.byFaction;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.byGMPower;
    packet.XParse >> value.dwPvPKillCount;
    packet >> value.stSoulWeapon;
    packet >> value.stSubWeapon;
    for (int index = 0; index < 14; ++index) {
        packet >> value.stShapeEquipItemInfo[index];
        packet >> value.stLookEquipIemInfo[index];
    }
    packet.XParse >> value.dwActiveBroachEffect;
    packet >> value.stTitleInfo;
    packet >> value.stLeagueInfo;
    packet >> value.stAbility;
    packet >> value.stShopInfo;
    packet.XParse >> value.shFP;
    packet.XParse >> value.shBonusFP;
    packet.XParse >> value.shPCBangFP;
    packet.XParse >> value.byEchelonLevel;
    packet.XParse >> value.nEchelonExp;
    packet.XParse >> value.bBattlePose;
    packet.XParse >> value.dwStatus;
    value.vecBuffInfo.clear();
    std::uint8_t buffCount = 0;
    packet.XParse >> buffCount;
    value.vecBuffInfo.reserve(static_cast<std::size_t>(buffCount));
    for (std::uint8_t index = 0; index < buffCount; ++index) {
        STBuffInfo buffInfo{};
        packet >> buffInfo;
        value.vecBuffInfo.push_back(buffInfo);
    }
    packet.XParse >> value.byCharSlotPos;
    packet.XParse >> value.nEqualizerID;
}

inline XPacket& operator<<(XPacket& packet, const STCharInfo& value) {
    packet << value.uxActorID;
    packet << value.stBaseInfo;
    packet.XParse << value.byLevel;
    packet.XParse << value.byFaction;
    packet.XParse << value.dwUAID;
    packet.XParse << value.byGMPower;
    packet.XParse << value.dwPvPKillCount;
    packet << value.stSoulWeapon;
    packet << value.stSubWeapon;
    for (int index = 0; index < 14; ++index) {
        packet << value.stShapeEquipItemInfo[index];
        packet << value.stLookEquipIemInfo[index];
    }
    packet.XParse << value.dwActiveBroachEffect;
    packet << value.stTitleInfo;
    packet << value.stLeagueInfo;
    packet << value.stAbility;
    packet << value.stShopInfo;
    packet.XParse << value.shFP;
    packet.XParse << value.shBonusFP;
    packet.XParse << value.shPCBangFP;
    packet.XParse << value.byEchelonLevel;
    packet.XParse << value.nEchelonExp;
    packet.XParse << value.bBattlePose;
    packet.XParse << value.dwStatus;
    packet.XParse << static_cast<std::uint8_t>(std::min<std::size_t>(value.vecBuffInfo.size(), 0xFF));
    for (std::size_t index = 0; index < value.vecBuffInfo.size() && index < 0xFF; ++index) {
        packet << value.vecBuffInfo[index];
    }
    packet.XParse << value.byCharSlotPos;
    packet.XParse << value.nEqualizerID;
    return packet;
}

inline void operator>>(XPacket& packet, STCharInfoEx& value) {
    packet >> static_cast<STCharInfo&>(value);
    packet >> value.stPosInfo;
    packet.XParse >> value.fCurSuperArmorGage;
    packet.XParse >> value.fMaxSuperArmorGage;
}

inline XPacket& operator<<(XPacket& packet, const STCharInfoEx& value) {
    packet << static_cast<const STCharInfo&>(value);
    packet << value.stPosInfo;
    packet.XParse << value.fCurSuperArmorGage;
    packet.XParse << value.fMaxSuperArmorGage;
    return packet;
}

inline void operator>>(XPacket& packet, STMyCharInfoEx& value) {
    packet >> static_cast<STCharInfoEx&>(value);
    packet.XParse >> value.nExp;
    packet.XParse >> value.biMoney;
    packet.XParse >> value.byCommonStep;
    packet.XParse >> value.byConsumeStep;
    packet.XParse >> value.byCostumeStep;
    packet.XParse >> value.byCardStep;
    packet.XParse >> value.userDBBits.nUserDB;
    packet.XParse >> value.syncUserBits.nSyncUser;
    packet.XParse >> value.biBP;
    packet.XParse >> value.biEther;
    packet.XParse >> value.biFriendPoint;
    short outLen = 0;
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse >> value.bNetCafe;
    packet.XParse >> value.biRecycle;
    packet.XParse >> value.nDyePoint;
    packet.XParse >> value.nRenovatePoint;
    packet.XParse >> value.nRefinePoint;
}

// PS_DB_CHARACTER_CREATE 反序列化操作符 - 必须在 STMyCharInfoEx 反序列化操作符之后
inline void operator>>(XPacket& packet, PS_DB_CHARACTER_CREATE& value) {
    packet.XParse >> value.nUAID;
    packet >> value.stCharInfo;
    packet.XParse >> value.wDistrictID;
    packet.XParse >> value.fPosX;
    packet.XParse >> value.fPosY;
    packet.XParse >> value.fPosZ;
    for (int i = 0; i < 20; ++i) {
        packet.XParse >> value.nDefulatSkill[i];
    }
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            packet.XParse >> value.nSkillDeck[i][j];
        }
    }
    for (int i = 0; i < 6; ++i) {
        packet.XParse >> value.nDefulatGesture[i];
    }
    for (int i = 0; i < 2; ++i) {
        packet.XParse >> value.nDefaultConsume[i];
    }
}

inline XPacket& operator<<(XPacket& packet, const STMyCharInfoEx& value) {
    packet << static_cast<const STCharInfoEx&>(value);
    packet.XParse << value.nExp;
    packet.XParse << value.biMoney;
    packet.XParse << value.byCommonStep;
    packet.XParse << value.byConsumeStep;
    packet.XParse << value.byCostumeStep;
    packet.XParse << value.byCardStep;
    packet.XParse << value.userDBBits.nUserDB;
    packet.XParse << value.syncUserBits.nSyncUser;
    packet.XParse << value.biBP;
    packet.XParse << value.biEther;
    packet.XParse << value.biFriendPoint;
    packet.XParse << std::string(value.szAccountID);
    packet.XParse << value.bNetCafe;
    packet.XParse << value.biRecycle;
    packet.XParse << value.nDyePoint;
    packet.XParse << value.nRenovatePoint;
    packet.XParse << value.nRefinePoint;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_CHARACTER_CREATE& value) {
    packet.XParse << value.nUAID;
    packet << value.stCharInfo;
    packet.XParse << value.wDistrictID;
    packet.XParse << value.fPosX;
    packet.XParse << value.fPosY;
    packet.XParse << value.fPosZ;
    for (int skillId : value.nDefulatSkill) {
        packet.XParse << skillId;
    }
    for (const auto& skillDeckRow : value.nSkillDeck) {
        for (int skillId : skillDeckRow) {
            packet.XParse << skillId;
        }
    }
    for (int gestureId : value.nDefulatGesture) {
        packet.XParse << gestureId;
    }
    for (int consumeId : value.nDefaultConsume) {
        packet.XParse << consumeId;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_STATISTICS_CHARACTER_CREATE& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << FixedWideArrayToWString(value.strName);
    packet.XParse << value.byClass;
    packet.XParse << value.byCount;
    packet.XParse << value.dwItem1;
    packet.XParse << value.dwItem2;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_STATISTICS_SKILL& value) {
    packet.XParse >> value.byFlag;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwSkill_New;
    packet.XParse >> value.dwSkill_Old;
    packet.XParse >> value.dwDivergenceID;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_STATISTICS_SKILL& value) {
    packet.XParse << value.byFlag;
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwSkill_New;
    packet.XParse << value.dwSkill_Old;
    packet.XParse << value.dwDivergenceID;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_STATISTICS_ITEM& value) {
    packet.XParse >> value.byFlag;
    packet.XParse >> value.biSerial;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.byUpgrade;
    packet.XParse >> value.byUpgradeLimit;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_STATISTICS_ITEM& value) {
    packet.XParse << value.byFlag;
    packet.XParse << value.biSerial;
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwItemID;
    packet.XParse << value.byUpgrade;
    packet.XParse << value.byUpgradeLimit;
    return packet;
}

inline XPacket& operator>>(XPacket& packet, ST_STATISTICS_CHARACTER_CREATE& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    std::int16_t shLen = 0;
    packet.XParse.GetWString(value.strName, 21, shLen);
    packet.XParse >> value.byClass;
    packet.XParse >> value.byCount;
    packet.XParse >> value.dwItem1;
    packet.XParse >> value.dwItem2;
    return packet;
}

// operator<< for ST_STATISTICS_CHARACTER_CREATE already defined above

inline XPacket& operator>>(XPacket& packet, ST_STATISTICS_CHARACTER_SAVE& value) {
    packet.XParse >> value.byFlag;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byLevel;
    packet.XParse >> value.biMoney;
    packet.XParse >> value.dwPreFix;
    packet.XParse >> value.dwSufFix;
    packet.XParse >> value.biBattlePoint;
    packet.XParse >> value.biEther;
    packet.XParse >> value.shFP;
    packet.XParse >> value.shBonusFP;
    for (int i = 0; i < 4; ++i) {
        packet.XParse >> value.nQuickSlotItem[i];
    }
    for (int i = 0; i < 5; ++i) {
        packet.XParse >> value.biQuickSlotCard[i];
    }
    packet.XParse >> value.shClearChapter;
    packet.XParse >> value.shClearStage;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_STATISTICS_CHARACTER_SAVE& value) {
    packet.XParse << value.byFlag;
    packet.XParse << value.dwUCID;
    packet.XParse << value.byLevel;
    packet.XParse << value.biMoney;
    packet.XParse << value.dwPreFix;
    packet.XParse << value.dwSufFix;
    packet.XParse << value.biBattlePoint;
    packet.XParse << value.biEther;
    packet.XParse << value.shFP;
    packet.XParse << value.shBonusFP;
    for (int quickSlotItem : value.nQuickSlotItem) {
        packet.XParse << quickSlotItem;
    }
    for (std::int64_t quickSlotCard : value.biQuickSlotCard) {
        packet.XParse << quickSlotCard;
    }
    packet.XParse << value.shClearChapter;
    packet.XParse << value.shClearStage;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_LOG_GAME& value) {
    packet.XParse << value._nUAID;
    packet.XParse << value._nUCID;
    packet.XParse << value._sMainType;
    packet.XParse << value._sSubType;
    packet.XParse << value.nParam0;
    packet.XParse << value.nParam1;
    packet.XParse << value.nParam2;
    packet.XParse << value.nParam3;
    packet.XParse << value.nParam4;
    packet.XParse << value.nParam5;
    packet.XParse << value.nParam6;
    packet.XParse << value.nParam7;
    packet.XParse << value.nParam8;
    packet.XParse << value.nParam9;
    packet.XParse << FixedWideArrayToWString(value.szParam10);
    packet.XParse << FixedWideArrayToWString(value.szComment);
    packet.XParse << FixedWideArrayToWString(value.szComment2);
    packet.XParse << value.nWorld_Idx;
    packet.XParse << value.nParam11;
    packet.XParse << value.nParam12;
    return packet;
}

// 对齐 IDA: ST_LOG_GAME 反序列化（WriteAuthLog 需要）
inline void operator>>(XPacket& packet, ST_LOG_GAME& value) {
    std::wstring tempStr;
    packet.XParse >> value._nUAID;
    packet.XParse >> value._nUCID;
    packet.XParse >> value._sMainType;
    packet.XParse >> value._sSubType;
    packet.XParse >> value.nParam0;
    packet.XParse >> value.nParam1;
    packet.XParse >> value.nParam2;
    packet.XParse >> value.nParam3;
    packet.XParse >> value.nParam4;
    packet.XParse >> value.nParam5;
    packet.XParse >> value.nParam6;
    packet.XParse >> value.nParam7;
    packet.XParse >> value.nParam8;
    packet.XParse >> value.nParam9;
    packet.XParse >> tempStr;
    WStringToFixedWideArray(tempStr, value.szParam10);
    packet.XParse >> tempStr;
    WStringToFixedWideArray(tempStr, value.szComment);
    packet.XParse >> tempStr;
    WStringToFixedWideArray(tempStr, value.szComment2);
    packet.XParse >> value.nWorld_Idx;
    packet.XParse >> value.nParam11;
    packet.XParse >> value.nParam12;
}

// 对齐 IDA: ST_CHAT_LOG_GAME 序列化
inline XPacket& operator<<(XPacket& packet, const ST_CHAT_LOG_GAME& value) {
    packet.XParse << value.nUAID;
    packet.XParse << value.nUCID;
    packet.XParse << value.sType;
    packet.XParse << value.nParam0;
    packet.XParse << value.nParam1;
    packet.XParse << value.nParam2;
    packet.XParse << value.nParam3;
    packet.XParse << value.nParam4;
    packet.XParse << value.nParam5;
    packet.XParse << value.nParam6;
    packet.XParse << FixedWideArrayToWString(value.szComment);
    return packet;
}

// 对齐 IDA: ST_CHAT_LOG_GAME 反序列化（WriteChatLog 需要）
inline void operator>>(XPacket& packet, ST_CHAT_LOG_GAME& value) {
    std::wstring tempStr;
    packet.XParse >> value.nUAID;
    packet.XParse >> value.nUCID;
    packet.XParse >> value.sType;
    packet.XParse >> value.nParam0;
    packet.XParse >> value.nParam1;
    packet.XParse >> value.nParam2;
    packet.XParse >> value.nParam3;
    packet.XParse >> value.nParam4;
    packet.XParse >> value.nParam5;
    packet.XParse >> value.nParam6;
    packet.XParse >> tempStr;
    WStringToFixedWideArray(tempStr, value.szComment);
}

// 对齐 IDA: ST_STAT_LOG_GAME 序列化
inline XPacket& operator<<(XPacket& packet, const ST_STAT_LOG_GAME& value) {
    packet.XParse << value.nUAID;
    packet.XParse << value.nUCID;
    packet.XParse << value.fParam0;
    packet.XParse << value.fParam1;
    packet.XParse << value.fParam2;
    packet.XParse << value.fParam3;
    packet.XParse << value.fParam4;
    packet.XParse << value.fParam5;
    packet.XParse << value.fParam6;
    packet.XParse << value.fParam7;
    packet.XParse << value.fParam8;
    return packet;
}

// 对齐 IDA: ST_STAT_LOG_GAME 反序列化（WriteStatLog 需要）
inline void operator>>(XPacket& packet, ST_STAT_LOG_GAME& value) {
    packet.XParse >> value.nUAID;
    packet.XParse >> value.nUCID;
    packet.XParse >> value.fParam0;
    packet.XParse >> value.fParam1;
    packet.XParse >> value.fParam2;
    packet.XParse >> value.fParam3;
    packet.XParse >> value.fParam4;
    packet.XParse >> value.fParam5;
    packet.XParse >> value.fParam6;
    packet.XParse >> value.fParam7;
    packet.XParse >> value.fParam8;
}

// 对齐 IDA: ST_KRR_MONSTER_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_KRR_MONSTER_INFO& value) {
    packet.XParse << value.dwMonsterID;
    packet.XParse << value.dwTableID;
    packet.XParse << value.byChannel;
    packet.XParse << value.xPos;
    packet.XParse << value.yPos;
    packet.XParse << value.zPos;
    packet.XParse << value.dwRemoveTime;
    return packet;
}

// 对齐 IDA: ST_KRR_MONSTER_INFO 反序列化
inline void operator>>(XPacket& packet, ST_KRR_MONSTER_INFO& value) {
    packet.XParse >> value.dwMonsterID;
    packet.XParse >> value.dwTableID;
    packet.XParse >> value.byChannel;
    packet.XParse >> value.xPos;
    packet.XParse >> value.yPos;
    packet.XParse >> value.zPos;
    packet.XParse >> value.dwRemoveTime;
}

// 对齐 IDA: ST_LOG_TEXT 序列化
inline XPacket& operator<<(XPacket& packet, const ST_LOG_TEXT& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.shMainType;
    packet.XParse << value.shSubType;
    packet.XParse << value.nMapID;
    packet.XParse << value.nInstanceID;
    packet.XParse << FixedCharArrayToString(value.szMsg);
    return packet;
}

// 对齐 IDA: ST_LOG_TEXT 反序列化（WriteLogText 需要）
inline void operator>>(XPacket& packet, ST_LOG_TEXT& value) {
    std::string tempStr;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.shMainType;
    packet.XParse >> value.shSubType;
    packet.XParse >> value.nMapID;
    packet.XParse >> value.nInstanceID;
    packet.XParse >> tempStr;
    std::fill_n(value.szMsg, 8000, '\0');
    const std::size_t copyLen = (std::min)(tempStr.size(), static_cast<std::size_t>(7999));
    std::copy_n(tempStr.data(), copyLen, value.szMsg);
}

// 对齐 IDA: ST_LOG_MONEY 序列化
inline XPacket& operator<<(XPacket& packet, const ST_LOG_MONEY& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.biIncMoney;
    packet.XParse << value.biDescMoney;
    return packet;
}

// 对齐 IDA: ST_LOG_MONEY 反序列化
inline void operator>>(XPacket& packet, ST_LOG_MONEY& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.biIncMoney;
    packet.XParse >> value.biDescMoney;
}

// 对齐 IDA: ST_LOG_SG_CHAR 序列化
inline XPacket& operator<<(XPacket& packet, const ST_LOG_SG_CHAR& value) {
    packet.XParse << FixedCharArrayToString(value.szMsg);
    return packet;
}

// 对齐 IDA: ST_LOG_SG_CHAR 反序列化
inline void operator>>(XPacket& packet, ST_LOG_SG_CHAR& value) {
    std::string tempStr;
    packet.XParse >> tempStr;
    std::fill_n(value.szMsg, 1024, '\0');
    const std::size_t copyLen = (std::min)(tempStr.size(), static_cast<std::size_t>(1023));
    std::copy_n(tempStr.data(), copyLen, value.szMsg);
}

// 对齐 IDA: PS_LOG_CASH 序列化
inline XPacket& operator<<(XPacket& packet, const PS_LOG_CASH& value) {
    packet.XParse << value.nUAID;
    packet.XParse << value.nUCID;
    packet.XParse << value.nOrderNo;
    packet.XParse << value.nParam1;
    packet.XParse << value.nParam2;
    packet.XParse << value.nParam3;
    packet.XParse << value.nParam4;
    packet.XParse << value.nParam5;
    packet.XParse << value.nParam6;
    packet.XParse << FixedCharArrayToString(value.szBillCode);
    return packet;
}

// 对齐 IDA: PS_LOG_CASH 反序列化
inline void operator>>(XPacket& packet, PS_LOG_CASH& value) {
    std::string tempBillCode;
    packet.XParse >> value.nUAID;
    packet.XParse >> value.nUCID;
    packet.XParse >> value.nOrderNo;
    packet.XParse >> value.nParam1;
    packet.XParse >> value.nParam2;
    packet.XParse >> value.nParam3;
    packet.XParse >> value.nParam4;
    packet.XParse >> value.nParam5;
    packet.XParse >> value.nParam6;
    packet.XParse >> tempBillCode;
    std::fill_n(value.szBillCode, 21, '\0');
    const std::size_t copyLen = (std::min)(tempBillCode.size(), static_cast<std::size_t>(20));
    std::copy_n(tempBillCode.data(), copyLen, value.szBillCode);
}

// 对齐 IDA: PS_LOG_CHARACTER_CONNECT_SERVER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_LOG_CHARACTER_CONNECT_SERVER& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nServerID;
    packet.XParse << value.byType;
    return packet;
}

// 对齐 IDA: PS_LOG_CHARACTER_CONNECT_SERVER 反序列化
inline void operator>>(XPacket& packet, PS_LOG_CHARACTER_CONNECT_SERVER& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nServerID;
    packet.XParse >> value.byType;
}

// 对齐 IDA: ST_CLIENT_LOG 序列化
inline XPacket& operator<<(XPacket& packet, const ST_CLIENT_LOG& value) {
    packet.XParse << value.byType;
    packet.XParse << FixedCharArrayToString(value.szLog);
    return packet;
}

// 对齐 IDA: ST_CLIENT_LOG 反序列化
inline void operator>>(XPacket& packet, ST_CLIENT_LOG& value) {
    std::string tempLog;
    packet.XParse >> value.byType;
    packet.XParse >> tempLog;
    std::fill_n(value.szLog, 128, '\0');
    const std::size_t copyLen = (std::min)(tempLog.size(), static_cast<std::size_t>(127));
    std::copy_n(tempLog.data(), copyLen, value.szLog);
}

// ST_SG_AUTH_INFO 序列化已在文件前面定义（约第1624行）

inline void operator>>(XPacket& packet, SS_UPDATE_SERVER_INFO& value) {
    packet.XParse >> value.dwID;
    packet.XParse >> value.nState;
    packet.XParse >> value.nCurUser;
}

inline XPacket& operator<<(XPacket& packet, const SS_UPDATE_SERVER_INFO& value) {
    packet.XParse << value.dwID;
    packet.XParse << value.nState;
    packet.XParse << value.nCurUser;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const SS_REPORT_POOL_INFO& value) {
    packet.XParse << value.nMonsterCurSize;
    packet.XParse << value.nMonsterFullSize;
    packet.XParse << value.nNpcCurSize;
    packet.XParse << value.nNpcFullSize;
    packet.XParse << value.nAkashicCurSize;
    packet.XParse << value.nAkashicFullSize;
    packet.XParse << value.nProjectileCurSize;
    packet.XParse << value.nProjectileFullSize;
    packet.XParse << value.nTrapCurSize;
    packet.XParse << value.nTrapFullSize;
    packet.XParse << value.nChainLightningCurSize;
    packet.XParse << value.nChainLightningFullSize;
    packet.XParse << value.nInteractionCurSize;
    packet.XParse << value.nInteractionFullSize;
    packet.XParse << value.nVaccumCubeCurSize;
    packet.XParse << value.nVaccumCubeFullSize;
    packet.XParse << value.nMazeCurSize;
    packet.XParse << value.nMazeFullSize;
    packet.XParse << value.nMyRoomCurSize;
    packet.XParse << value.nMyRoomFullSize;
    packet.XParse << value.nSocialCurSize;
    packet.XParse << value.nSocialFullSize;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const SS_REPORT_CONNECT_INFO& value) {
    packet.XParse << value.bControlConnect;
    packet.XParse << value.bCommunityConnect;
    packet.XParse << FixedWideArrayToWString(value.szLogicThread);
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const SS_REPORT_SERVER_STATUS& value) {
    packet.XParse << value.nServerType;
    packet.XParse << value.dwServerID;
    packet.XParse << value.dwProcessID;
    packet.XParse << value.nUserCount;
    packet.XParse << FixedCharArrayToString(value.szIP);
    packet.XParse << value.nPort;
    packet.XParse << value.bNetCafe;
    packet << value.poolInfo;
    packet << value.connectInfo;
    return packet;
}

inline void operator>>(XPacket& packet, ST_CHARACTER_MAP_INFO& value) {
    packet.XParse >> value.nUCID;
    packet.XParse >> value.nMapID;
    packet.XParse >> value.nRevivePoint;
    packet.XParse >> value.nPrevMapID;
    packet.XParse >> value.nPrevRevivePoint;
}

inline XPacket& operator<<(XPacket& packet, const ST_CHARACTER_MAP_INFO& value) {
    packet.XParse << value.nUCID;
    packet.XParse << value.nMapID;
    packet.XParse << value.nRevivePoint;
    packet.XParse << value.nPrevMapID;
    packet.XParse << value.nPrevRevivePoint;
    return packet;
}

inline void operator>>(XPacket& packet, PS_CHARACTER_MAP_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (int index = 0; index < static_cast<int>(count); ++index) {
        ST_CHARACTER_MAP_INFO info{};
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_CHARACTER_MAP_LIST& value) {
    std::int8_t count = static_cast<std::int8_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const ST_CHARACTER_MAP_INFO& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_BROACH_SHAPE& value) {
    for (int& itemId : value.dwItemID) {
        packet.XParse >> itemId;
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_BROACH_SHAPE& value) {
    for (int itemId : value.dwItemID) {
        packet.XParse << itemId;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_BROACH_SHAPE_LIST& value) {
    int count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve((std::max)(count, 0));
    for (int index = 0; index < count; ++index) {
        PS_BROACH_SHAPE item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_BROACH_SHAPE_LIST& value) {
    std::int32_t count = static_cast<std::int32_t>(value.vecInfo.size());
    packet.XParse << count;
    for (const PS_BROACH_SHAPE& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline void operator>>(XPacket& packet, PS_CHARACTER_REPRESENTATIVE_CHANGE& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byLevel;
    short outLen = 0;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.nRepresentativeDate;
    packet.XParse >> value.nError;
}

inline XPacket& operator<<(XPacket& packet, const PS_CHARACTER_REPRESENTATIVE_CHANGE& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << value.byClass;
    packet.XParse << value.byLevel;
    packet.XParse << FixedWideArrayToWString(value.strName);
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.nRepresentativeDate;
    packet.XParse << value.nError;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DELETE_CHARACTER_RES& value) {
    packet.XParse << value.nRemainTime;
    return packet;
}

inline void operator>>(XPacket& packet, PS_CHARACTER_CHANGE_SLOT& value) {
    packet.XParse >> value.nUAID;
    packet.XParse >> value.nSrcUCID;
    packet.XParse >> value.nDestUCID;
    packet.XParse >> value.nErrorCode;
    packet.XParse >> value.bySrcSlot;
    packet.XParse >> value.byDestSlot;
}

inline XPacket& operator<<(XPacket& packet, const PS_CHARACTER_CHANGE_SLOT& value) {
    packet.XParse << value.nUAID;
    packet.XParse << value.nSrcUCID;
    packet.XParse << value.nDestUCID;
    packet.XParse << value.nErrorCode;
    packet.XParse << value.bySrcSlot;
    packet.XParse << value.byDestSlot;
    return packet;
}

inline void operator>>(XPacket& packet, PS_REQ_CHECK_NAME& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.strName, 21, outLen);
}

inline XPacket& operator<<(XPacket& packet, const PS_REQ_CHECK_NAME& value) {
    packet.XParse << FixedWideArrayToWString(value.strName);
    return packet;
}

inline void operator>>(XPacket& packet, PS_RES_CHECK_NAME& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.byResult;
}

inline XPacket& operator<<(XPacket& packet, const PS_RES_CHECK_NAME& value) {
    packet.XParse << FixedWideArrayToWString(value.strName);
    packet.XParse << value.byResult;
    return packet;
}

/**
 * @brief 世界模式信息 (ControlServer)
 * 来自 IDA: ST_WORLD_MODE - 16 bytes
 */
struct ST_WORLD_MODE {
    int nModeDateID = 0;      // +0x00: 模式日期ID
    int nModeID = 0;          // +0x04: 模式ID
    __int64 biEnterDate = 0;  // +0x08: 进入日期
};

static_assert(sizeof(ST_WORLD_MODE) == 16, "ST_WORLD_MODE size must match IDA");
static_assert(offsetof(ST_WORLD_MODE, nModeDateID) == 0, "ST_WORLD_MODE.nModeDateID offset mismatch");
static_assert(offsetof(ST_WORLD_MODE, nModeID) == 4, "ST_WORLD_MODE.nModeID offset mismatch");
static_assert(offsetof(ST_WORLD_MODE, biEnterDate) == 8, "ST_WORLD_MODE.biEnterDate offset mismatch");

inline XPacket& operator<<(XPacket& packet, const ST_WORLD_MODE& value) {
    packet.XParse << value.nModeDateID;
    packet.XParse << value.nModeID;
    packet.XParse << value.biEnterDate;
    return packet;
}

inline void operator>>(XPacket& packet, ST_WORLD_MODE& value) {
    packet.XParse >> value.nModeDateID;
    packet.XParse >> value.nModeID;
    packet.XParse >> value.biEnterDate;
}

/**
 * @brief 进入世界模式信息 (ControlServer)
 * 来自 IDA: ST_ENTER_WORLD_MODE_INFO - 32 bytes
 */
struct ST_ENTER_WORLD_MODE_INFO {
    std::vector<ST_WORLD_MODE> vecInfo;  // +0x00: 世界模式列表 (32 bytes)
};

static_assert(sizeof(ST_ENTER_WORLD_MODE_INFO) == 32, "ST_ENTER_WORLD_MODE_INFO size must match IDA");

/**
 * @brief 世界模式完成请求结构 (ControlServer)
 * 来自 IDA: PS_WORLD_MODE_COMPLETE - 64 bytes
 */
struct PS_WORLD_MODE_COMPLETE {
    int nModeID = 0;                    // +0x00: 模式ID
    // +0x04-0x07: padding (4 bytes)
    UXMapID uxMapID{};                  // +0x08: 地图ID (8 bytes)
    wchar_t strKiller[21] = {};         // +0x10: 击杀者名称 (42 bytes)
    bool bFinish = false;               // +0x3A: 是否完成
    // +0x3B-0x3F: padding (5 bytes)
    int nModeDateID = 0;                // +0x3C: 模式日期ID (应该是 0x3C = 60)
};

static_assert(sizeof(PS_WORLD_MODE_COMPLETE) == 64, "PS_WORLD_MODE_COMPLETE size must match IDA");

/**
 * @brief 世界模式信息结构 (ControlServer)
 * 来自 IDA: ST_WORLD_MODE_INFO - 56 bytes
 */
struct ST_WORLD_MODE_INFO {
    __int64 nStartTime = 0;             // +0x00: 开始时间 (8 bytes)
    __int64 nFinishTime = 0;            // +0x08: 结束时间 (8 bytes)
    int nModeID = 0;                    // +0x10: 模式ID (4 bytes)
    int nState = 0;                     // +0x14: 状态 (4 bytes)
    bool bSuccess = false;              // +0x18: 是否成功 (1 byte)
    // +0x19-0x1B: padding (3 bytes)
    int nModeDateID = 0;                // +0x1C: 模式日期ID (4 bytes)
    int nMonsterClearCount = 0;         // +0x20: 怪物清除计数 (4 bytes)
    // +0x24-0x27: padding (4 bytes)
    __int64 biModeStartTime = 0;       // +0x28: 模式开始时间 (8 bytes)
    __int64 biModeEndTime = 0;         // +0x30: 模式结束时间 (8 bytes)
};

static_assert(sizeof(ST_WORLD_MODE_INFO) == 56, "ST_WORLD_MODE_INFO size must match IDA");

/**
 * @brief 世界模式信息向量 (ControlServer)
 * 来自 IDA: ST_WORLD_MODE_INFO_VEC - 32 bytes
 */
struct ST_WORLD_MODE_INFO_VEC {
    std::vector<ST_WORLD_MODE_INFO> vecInfo;  // +0x00: 世界模式信息列表 (32 bytes)
};

static_assert(sizeof(ST_WORLD_MODE_INFO_VEC) == 32, "ST_WORLD_MODE_INFO_VEC size must match IDA");

/**
 * @brief 世界模式更新结构 (ControlServer)
 * 来自 IDA: PS_WORLD_MODE_UPDATE - 32 bytes
 */
struct PS_WORLD_MODE_UPDATE {
    ST_WORLD_MODE stInfo{};             // +0x00: 世界模式信息 (16 bytes)
    UXMapID uxMapID{};                  // +0x10: 地图ID (8 bytes)
    int nMonsterClearCount = 0;         // +0x18: 怪物清除计数 (4 bytes)
    bool bUpdate = false;               // +0x1C: 是否更新 (1 byte)
    // +0x1D-0x1F: padding (3 bytes)
};

static_assert(sizeof(PS_WORLD_MODE_UPDATE) == 32, "PS_WORLD_MODE_UPDATE size must match IDA");

/**
 * @brief 世界模式启动结构 (GameServer)
 * 来自 IDA: PS_WORLD_MODE_START - 用于 0x30/0x01 包
 * 用于通知客户端世界模式启动
 */
struct PS_WORLD_MODE_START {
    int nModeDateID = 0;                // +0x00: 模式日期ID (4 bytes)
    int nID = 0;                        // +0x04: 模式ID (4 bytes)
    std::int64_t nStartTime = 0;        // +0x08: 开始时间 (8 bytes)
    std::int64_t nFinishTime = 0;       // +0x10: 结束时间 (8 bytes)
    std::uint8_t byState = 0;           // +0x18: 状态 (1 byte)
    // +0x19-0x1F: padding (7 bytes)
    std::int64_t biModeStartTime = 0;   // +0x20: 模式开始时间 (8 bytes)
    std::int64_t biModeEndTime = 0;     // +0x28: 模式结束时间 (8 bytes)
};

static_assert(sizeof(PS_WORLD_MODE_START) == 0x30, "PS_WORLD_MODE_START size must match IDA");

inline XPacket& operator<<(XPacket& packet, const PS_WORLD_MODE_START& value) {
    packet.XParse << value.nModeDateID;
    packet.XParse << value.nID;
    packet.XParse << value.nStartTime;
    packet.XParse << value.nFinishTime;
    packet.XParse << value.byState;
    packet.XParse << value.biModeStartTime;
    packet.XParse << value.biModeEndTime;
    return packet;
}

/**
 * @brief 世界模式完成结构 (ControlServer)
 * 来自 IDA: PS_WORLD_MODE_FINISH - 用于 0xFB/0x04 包
 * 布局: nModeID(4) + padding(4) + uxMapID(8) + nFinishTime(8) + nModeDateID(4) + nMonsterClearCount(4) + strKiller(42) + bSuccess(1) + padding(5) = 80 bytes
 */
struct PS_WORLD_MODE_FINISH {
    int nModeID = 0;                    // +0x00: 模式ID (4 bytes)
    // +0x04-0x07: padding for 8-byte alignment
    UXMapID uxMapID{};                  // +0x08: 地图ID (8 bytes)
    std::int64_t nFinishTime = 0;       // +0x10: 完成时间 (8 bytes)
    int nModeDateID = 0;                // +0x18: 模式日期ID (4 bytes)
    int nMonsterClearCount = 0;         // +0x1C: 怪物清除计数 (4 bytes)
    wchar_t strKiller[21] = {};         // +0x20: 击杀者名字 (42 bytes, Unicode)
    bool bSuccess = false;              // +0x4A: 是否成功 (1 byte)
    // +0x4B-0x4F: padding (5 bytes)
};

static_assert(sizeof(PS_WORLD_MODE_FINISH) == 80, "PS_WORLD_MODE_FINISH size must match IDA");

/**
 * @brief 梯队信息结构 (DBAgent)
 * 来自 IDA: PT_ECHELON_INFO - 20 bytes
 * 用于 SP_ECHELON_INFO_UPDATE 存储过程
 */
struct PT_ECHELON_INFO {
    std::uint8_t byEchelonLevel = 0;    // +0x00: 梯队等级 (1 byte)
    // +0x01-0x03: padding (3 bytes)
    int nEchelonExp = 0;                // +0x04: 梯队经验 (4 bytes)
    int nTotalExp = 0;                  // +0x08: 总经验 (4 bytes)
    int nBounsExp = 0;                  // +0x0C: 奖励经验 (4 bytes)
    bool bLevelUp = false;              // +0x10: 是否升级 (1 byte)
    // +0x11-0x13: padding (3 bytes)
};

static_assert(sizeof(PT_ECHELON_INFO) == 20, "PT_ECHELON_INFO size must match IDA");
static_assert(offsetof(PT_ECHELON_INFO, nEchelonExp) == 4, "PT_ECHELON_INFO.nEchelonExp offset mismatch");
static_assert(offsetof(PT_ECHELON_INFO, nTotalExp) == 8, "PT_ECHELON_INFO.nTotalExp offset mismatch");
static_assert(offsetof(PT_ECHELON_INFO, nBounsExp) == 12, "PT_ECHELON_INFO.nBounsExp offset mismatch");
static_assert(offsetof(PT_ECHELON_INFO, bLevelUp) == 16, "PT_ECHELON_INFO.bLevelUp offset mismatch");

inline void operator>>(XPacket& packet, PT_ECHELON_INFO& value) {
    packet.XParse >> value.byEchelonLevel;
    packet.XParse >> value.nEchelonExp;
    packet.XParse >> value.nTotalExp;
    packet.XParse >> value.nBounsExp;
    packet.XParse >> value.bLevelUp;
}

inline XPacket& operator<<(XPacket& packet, const PT_ECHELON_INFO& value) {
    packet.XParse << value.byEchelonLevel;
    packet.XParse << value.nEchelonExp;
    packet.XParse << value.nTotalExp;
    packet.XParse << value.nBounsExp;
    packet.XParse << value.bLevelUp;
    return packet;
}

/**
 * @brief 回收更新请求结构 (DBAgent)
 * 来自 IDA: PS_DB_RECYCLE_UPDATE - 32 bytes
 * 用于 SP_ADD_RECYCLE 存储过程
 */
struct PS_DB_RECYCLE_UPDATE {
    unsigned int dwUCID = 0;            // +0x00: 角色ID (4 bytes)
    // +0x04-0x07: padding (4 bytes)
    std::int64_t biRecycle = 0;         // +0x08: 回收数量 (8 bytes)
    std::int64_t biTotalRecycle = 0;    // +0x10: 总回收数量 (8 bytes)
    int nErrorCode = 0;                 // +0x18: 错误码 (4 bytes)
    // +0x1C-0x1F: padding (4 bytes)
};

static_assert(sizeof(PS_DB_RECYCLE_UPDATE) == 32, "PS_DB_RECYCLE_UPDATE size must match IDA");
static_assert(offsetof(PS_DB_RECYCLE_UPDATE, biRecycle) == 8, "PS_DB_RECYCLE_UPDATE.biRecycle offset mismatch");
static_assert(offsetof(PS_DB_RECYCLE_UPDATE, biTotalRecycle) == 16, "PS_DB_RECYCLE_UPDATE.biTotalRecycle offset mismatch");
static_assert(offsetof(PS_DB_RECYCLE_UPDATE, nErrorCode) == 24, "PS_DB_RECYCLE_UPDATE.nErrorCode offset mismatch");

inline void operator>>(XPacket& packet, PS_DB_RECYCLE_UPDATE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.biRecycle;
    packet.XParse >> value.biTotalRecycle;
    packet.XParse >> value.nErrorCode;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_RECYCLE_UPDATE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.biRecycle;
    packet.XParse << value.biTotalRecycle;
    packet.XParse << value.nErrorCode;
    return packet;
}

/**
 * @brief 角色觉醒更新结构 (DBAgent)
 * 来自 IDA: PS_CHAR_UPDATE_AWAKEN - 16 bytes
 * 用于 SP_CHARACTER_AWAKEN_UPDATE 存储过程
 */
struct PS_CHAR_UPDATE_AWAKEN {
    unsigned int dwUCID = 0;            // +0x00: 角色ID (4 bytes)
    std::uint8_t byGrade = 0;           // +0x04: 觉醒等级 (1 byte)
    // +0x05-0x07: padding (3 bytes)
    unsigned int dwProfilePhotoID = 0;  // +0x08: 头像ID (4 bytes)
    bool bEffect = false;               // +0x0C: 是否有效 (1 byte)
    // +0x0D-0x0F: padding (3 bytes)
};

static_assert(sizeof(PS_CHAR_UPDATE_AWAKEN) == 16, "PS_CHAR_UPDATE_AWAKEN size must match IDA");
static_assert(offsetof(PS_CHAR_UPDATE_AWAKEN, byGrade) == 4, "PS_CHAR_UPDATE_AWAKEN.byGrade offset mismatch");
static_assert(offsetof(PS_CHAR_UPDATE_AWAKEN, dwProfilePhotoID) == 8, "PS_CHAR_UPDATE_AWAKEN.dwProfilePhotoID offset mismatch");
static_assert(offsetof(PS_CHAR_UPDATE_AWAKEN, bEffect) == 12, "PS_CHAR_UPDATE_AWAKEN.bEffect offset mismatch");

inline void operator>>(XPacket& packet, PS_CHAR_UPDATE_AWAKEN& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.byGrade;
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.bEffect;
}

inline XPacket& operator<<(XPacket& packet, const PS_CHAR_UPDATE_AWAKEN& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.byGrade;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.bEffect;
    return packet;
}

/**
 * @brief 角色免费复活计数结构 (DBAgent)
 * 来自 IDA: PS_CHARACTER_FREE_REVIVE - 12 bytes
 * 用于 SP_CHARACTER_FREE_REVIVAL_UPDATE 存储过程
 */
struct PS_CHARACTER_FREE_REVIVE {
    unsigned int dwUCID = 0;            // +0x00: 角色ID (4 bytes)
    int nFreeReviveCount = 0;           // +0x04: 免费复活计数 (4 bytes)
    int nMaxFreeReviveCount = 0;        // +0x08: 最大免费复活计数 (4 bytes)
};

static_assert(sizeof(PS_CHARACTER_FREE_REVIVE) == 12, "PS_CHARACTER_FREE_REVIVE size must match IDA");
static_assert(offsetof(PS_CHARACTER_FREE_REVIVE, nFreeReviveCount) == 4, "PS_CHARACTER_FREE_REVIVE.nFreeReviveCount offset mismatch");
static_assert(offsetof(PS_CHARACTER_FREE_REVIVE, nMaxFreeReviveCount) == 8, "PS_CHARACTER_FREE_REVIVE.nMaxFreeReviveCount offset mismatch");

inline void operator>>(XPacket& packet, PS_CHARACTER_FREE_REVIVE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nFreeReviveCount;
    packet.XParse >> value.nMaxFreeReviveCount;
}

inline XPacket& operator<<(XPacket& packet, const PS_CHARACTER_FREE_REVIVE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nFreeReviveCount;
    packet.XParse << value.nMaxFreeReviveCount;
    return packet;
}

/**
 * @brief 平衡器更新结构 (DBAgent)
 * 来自 IDA: PS_DB_EQUALIZER_UPDATE - 8 bytes
 * 用于 SP_CHARACTER_EQUALIZER_UPDATE 存储过程
 */
struct PS_DB_EQUALIZER_UPDATE {
    unsigned int dwUCID = 0;            // +0x00: 角色ID (4 bytes)
    int nEqualizerID = 0;               // +0x04: 平衡器ID (4 bytes)
};

static_assert(sizeof(PS_DB_EQUALIZER_UPDATE) == 8, "PS_DB_EQUALIZER_UPDATE size must match IDA");
static_assert(offsetof(PS_DB_EQUALIZER_UPDATE, nEqualizerID) == 4, "PS_DB_EQUALIZER_UPDATE.nEqualizerID offset mismatch");

inline void operator>>(XPacket& packet, PS_DB_EQUALIZER_UPDATE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.nEqualizerID;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_EQUALIZER_UPDATE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.nEqualizerID;
    return packet;
}

/**
 * @brief 称号添加结构 (DBAgent)
 * 来自 IDA: PS_TITLE_ADD - 8 bytes
 * 用于 SP_TITLE_ADD 存储过程
 */
struct PS_TITLE_ADD_INFO {
    unsigned int dwTitleID = 0;         // +0x00: 称号ID (4 bytes)
    int nLogType = 0;                   // +0x04: 日志类型 (4 bytes)
};

static_assert(sizeof(PS_TITLE_ADD_INFO) == 8, "PS_TITLE_ADD_INFO size must match IDA");
static_assert(offsetof(PS_TITLE_ADD_INFO, nLogType) == 4, "PS_TITLE_ADD_INFO.nLogType offset mismatch");

inline void operator>>(XPacket& packet, PS_TITLE_ADD_INFO& value) {
    packet.XParse >> value.dwTitleID;
    packet.XParse >> value.nLogType;
}

inline XPacket& operator<<(XPacket& packet, const PS_TITLE_ADD_INFO& value) {
    packet.XParse << value.dwTitleID;
    packet.XParse << value.nLogType;
    return packet;
}

/**
 * @brief 称号添加列表结构 (DBAgent)
 * 来自 IDA: PS_TITLE_ADD_LIST - 40 bytes
 * 用于批量添加称号
 */
struct PS_TITLE_ADD_LIST {
    unsigned int dwUCID = 0;            // +0x00: 角色ID (4 bytes)
    // +0x04-0x07: padding (4 bytes)
    std::vector<PS_TITLE_ADD_INFO> vecInfo;  // +0x08: 称号列表 (32 bytes)
};

static_assert(sizeof(PS_TITLE_ADD_LIST) == 40, "PS_TITLE_ADD_LIST size must match IDA");
static_assert(offsetof(PS_TITLE_ADD_LIST, vecInfo) == 8, "PS_TITLE_ADD_LIST.vecInfo offset mismatch");

inline void operator>>(XPacket& packet, PS_TITLE_ADD_LIST& value) {
    packet.XParse >> value.dwUCID;
    // 手动读取 vector 大小和元素
    std::uint16_t nSize = 0;
    packet.XParse >> nSize;
    value.vecInfo.clear();
    value.vecInfo.reserve(nSize);
    for (std::uint16_t i = 0; i < nSize; ++i) {
        PS_TITLE_ADD_INFO info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_TITLE_ADD_LIST& value) {
    packet.XParse << value.dwUCID;
    // 手动写入 vector 大小和元素
    std::uint16_t nSize = static_cast<std::uint16_t>(value.vecInfo.size());
    packet.XParse << nSize;
    for (const PS_TITLE_ADD_INFO& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

/**
 * @brief 邮件恢复物品信息结构 (DBAgent)
 * 来自 IDA: PS_ITEM_RESTORE_INFO - 136 bytes
 * 用于恢复邮件中的物品数据
 */
struct PS_ITEM_RESTORE_INFO {
    std::uint8_t byInvenType = 0;       // +0x00: 物品栏类型
    std::uint8_t _pad0[1] = {};        // +0x01: padding
    std::int16_t shSlotPos = 0;        // +0x02: 槽位位置
    std::uint8_t _pad1[4] = {};        // +0x04-0x07: padding
    STItem stItem{};                   // +0x08: 物品信息 (120 bytes)
    std::int32_t nPostNumber = -1;     // +0x80: 邮件编号
};

static_assert(sizeof(PS_ITEM_RESTORE_INFO) == 136, "PS_ITEM_RESTORE_INFO size must match IDA");
static_assert(offsetof(PS_ITEM_RESTORE_INFO, shSlotPos) == 2, "PS_ITEM_RESTORE_INFO.shSlotPos offset mismatch");
static_assert(offsetof(PS_ITEM_RESTORE_INFO, stItem) == 8, "PS_ITEM_RESTORE_INFO.stItem offset mismatch");
static_assert(offsetof(PS_ITEM_RESTORE_INFO, nPostNumber) == 128, "PS_ITEM_RESTORE_INFO.nPostNumber offset mismatch");

inline void operator>>(XPacket& packet, PS_ITEM_RESTORE_INFO& value) {
    packet.XParse >> value.byInvenType;
    packet.XParse >> value.shSlotPos;
    packet >> value.stItem;
    packet.XParse >> value.nPostNumber;
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_RESTORE_INFO& value) {
    packet.XParse << value.byInvenType;
    packet.XParse << value.shSlotPos;
    packet << value.stItem;
    packet.XParse << value.nPostNumber;
    return packet;
}

/**
 * @brief 邮件恢复物品列表结构 (DBAgent)
 * 来自 IDA: PS_ITEM_RESTORE_LIST - 32 bytes
 * 用于批量恢复邮件物品
 */
struct PS_ITEM_RESTORE_LIST {
    std::vector<PS_ITEM_RESTORE_INFO> vecInfo;
};

inline void operator>>(XPacket& packet, PS_ITEM_RESTORE_LIST& value) {
    std::int16_t nSize = 0;
    packet.XParse >> nSize;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(nSize));
    for (std::int16_t i = 0; i < nSize; ++i) {
        PS_ITEM_RESTORE_INFO info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_RESTORE_LIST& value) {
    std::int16_t nSize = static_cast<std::int16_t>(value.vecInfo.size());
    packet.XParse << nSize;
    for (const PS_ITEM_RESTORE_INFO& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

// ============================================================================
// ItemDB 相关结构体定义
// ============================================================================

/**
 * @brief 金币移动信息结构
 * Per IDA 0x140057810: PS_MOVE_MONEY_INFO
 */
struct PS_MOVE_MONEY_INFO {
    std::int64_t biMoney = 0;
    std::uint8_t byTargetType = 0;  // 0: 转到背包, 1: 转到银行
};

/**
 * @brief 金币移动请求数据库包
 * Per IDA 0x140057810: PS_DB_MOVE_MONEY
 */
struct PS_DB_MOVE_MONEY {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwUAID = 0;
    PS_MOVE_MONEY_INFO psMoveMoneyInfo{};
};

inline void operator>>(XPacket& packet, PS_DB_MOVE_MONEY& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.psMoveMoneyInfo.biMoney;
    packet.XParse >> value.psMoveMoneyInfo.byTargetType;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_MOVE_MONEY& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.psMoveMoneyInfo.biMoney;
    packet.XParse << value.psMoveMoneyInfo.byTargetType;
    return packet;
}

/**
 * @brief 金币移动结果
 * Per IDA 0x140057810: PS_RES_MOVE_MONEY
 */
struct PS_RES_MOVE_MONEY {
    std::int32_t nInvenMoney = 0;
    std::int32_t nBankMoney = 0;
    std::int32_t nResult = 0;
};

inline XPacket& operator<<(XPacket& packet, const PS_RES_MOVE_MONEY& value) {
    packet.XParse << value.nInvenMoney;
    packet.XParse << value.nBankMoney;
    packet.XParse << value.nResult;
    return packet;
}

/**
 * @brief 物品冷却时间信息
 * Per IDA 0x1400584E0: PS_ITEM_COOLTIME_INFO - 24 bytes
 */
struct PS_ITEM_COOLTIME_INFO {
    std::uint16_t byCooltimeGroupID = 0;    // +0x00: 冷却组ID (2 bytes)
    std::uint8_t _pad0[6] = {};             // +0x02: padding (6 bytes)
    std::int64_t biRemainDate = 0;          // +0x08: 剩余日期时间 (8 bytes)
    std::int64_t biCooltimeValue = 0;       // +0x10: 冷却值 (8 bytes)
};
static_assert(sizeof(PS_ITEM_COOLTIME_INFO) == 24, "PS_ITEM_COOLTIME_INFO size must match IDA");

inline void operator>>(XPacket& packet, PS_ITEM_COOLTIME_INFO& value) {
    packet.XParse >> value.byCooltimeGroupID;
    packet.XParse >> value.biRemainDate;
    packet.XParse >> value.biCooltimeValue;
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_COOLTIME_INFO& value) {
    packet.XParse << value.byCooltimeGroupID;
    packet.XParse << value.biRemainDate;
    packet.XParse << value.biCooltimeValue;
    return packet;
}

/**
 * @brief 物品冷却时间列表
 * Per IDA 0x1400584E0: PS_ITEM_COOMTIME_LIST (注意原始拼写错误 COOMTIME)
 */
struct PS_ITEM_COOMTIME_LIST {
    std::vector<PS_ITEM_COOLTIME_INFO> vecInfo;
};

inline void operator>>(XPacket& packet, PS_ITEM_COOMTIME_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        PS_ITEM_COOLTIME_INFO info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_COOMTIME_LIST& value) {
    packet.XParse << static_cast<std::uint16_t>(value.vecInfo.size());
    for (const PS_ITEM_COOLTIME_INFO& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

/**
 * @brief 物品冷却时间更新请求
 * Per IDA 0x140058BA0: PS_DB_ITEM_COOLTIME_UPDATE - 32 bytes
 */
struct PS_DB_ITEM_COOLTIME_UPDATE {
    std::uint32_t dwUCID = 0;               // +0x00: UCID (4 bytes)
    std::uint8_t _pad0[4] = {};             // +0x04: padding (4 bytes)
    PS_ITEM_COOLTIME_INFO psInfo;           // +0x08: 冷却信息 (24 bytes)
};
static_assert(sizeof(PS_DB_ITEM_COOLTIME_UPDATE) == 32, "PS_DB_ITEM_COOLTIME_UPDATE size must match IDA");

inline void operator>>(XPacket& packet, PS_DB_ITEM_COOLTIME_UPDATE& value) {
    packet.XParse >> value.dwUCID;
    packet >> value.psInfo;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_COOLTIME_UPDATE& value) {
    packet.XParse << value.dwUCID;
    packet << value.psInfo;
    return packet;
}

/**
 * @brief 物品减少请求数据库包
 * Per IDA 0x14004F490: PS_DB_ITEM_REDUCE
 */
struct PS_DB_ITEM_REDUCE {
    std::uint32_t dwUCID = 0;
    std::int64_t xSerial = 0;
    std::int16_t nReduceCount = 0;
};

inline void operator>>(XPacket& packet, PS_DB_ITEM_REDUCE& value) {
    packet.XParse >> value.dwUCID;
    packet.XParse >> value.xSerial;
    packet.XParse >> value.nReduceCount;
}

inline XPacket& operator<<(XPacket& packet, const PS_DB_ITEM_REDUCE& value) {
    packet.XParse << value.dwUCID;
    packet.XParse << value.xSerial;
    packet.XParse << value.nReduceCount;
    return packet;
}

/**
 * @brief 物品整理单项数据
 * Per IDA 0x140051160: PS_ITEM_LINE_UP
 */
struct PS_ITEM_LINE_UP {
    std::int64_t xItemID = 0;
    std::uint8_t byInvenType = 0;
    std::int16_t shSlotPos = 0;
    std::int16_t shCount = 0;
};

/**
 * @brief 物品整理列表
 * Per IDA 0x140051160: PS_ITEM_LINE_UP_VEC
 */
struct PS_ITEM_LINE_UP_VEC {
    std::vector<PS_ITEM_LINE_UP> vecItem;
};

inline void operator>>(XPacket& packet, PS_ITEM_LINE_UP_VEC& value) {
    std::int16_t nSize = 0;
    packet.XParse >> nSize;
    value.vecItem.clear();
    value.vecItem.reserve(static_cast<std::size_t>(nSize));
    for (std::int16_t i = 0; i < nSize; ++i) {
        PS_ITEM_LINE_UP item;
        packet.XParse >> item.xItemID;
        packet.XParse >> item.byInvenType;
        packet.XParse >> item.shSlotPos;
        packet.XParse >> item.shCount;
        value.vecItem.push_back(item);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ITEM_LINE_UP_VEC& value) {
    std::int16_t nSize = static_cast<std::int16_t>(value.vecItem.size());
    packet.XParse << nSize;
    for (const PS_ITEM_LINE_UP& item : value.vecItem) {
        packet.XParse << item.xItemID;
        packet.XParse << item.byInvenType;
        packet.XParse << item.shSlotPos;
        packet.XParse << item.shCount;
    }
    return packet;
}

/**
 * @brief 网吧物品购买请求
 * Per IDA 0x1400587A0: PS_EVENT_NETCAFE_ITEM_BUY - 32 bytes
 */
struct PS_EVENT_NETCAFE_ITEM_BUY {
    std::vector<std::uint32_t> vecItemIDList;
};

inline void operator>>(XPacket& packet, PS_EVENT_NETCAFE_ITEM_BUY& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecItemIDList.clear();
    value.vecItemIDList.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        std::uint32_t itemID = 0;
        packet.XParse >> itemID;
        value.vecItemIDList.push_back(itemID);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_EVENT_NETCAFE_ITEM_BUY& value) {
    packet.XParse << static_cast<std::uint16_t>(value.vecItemIDList.size());
    for (const std::uint32_t itemID : value.vecItemIDList) {
        packet.XParse << itemID;
    }
    return packet;
}

// PS_DB_ITEM_COUNTBOX 定义移到 PSServer.h（依赖 ST_GET_INFO）

/**
 * @brief 抽卡列表单项数据
 * Per IDA 0x140052710: PS_GACHA_INFO
 */
struct PS_GACHA_INFO {
    std::uint8_t byType = 0;
    std::int32_t nItemID = 0;
    std::int32_t nCount = 0;
};

/**
 * @brief 抽卡结果列表
 * Per IDA 0x140052710: PS_GACHA_LIST
 */
struct PS_GACHA_LIST {
    std::vector<PS_GACHA_INFO> vecInfo;
};

inline void operator>>(XPacket& packet, PS_GACHA_LIST& value) {
    std::int16_t nSize = 0;
    packet.XParse >> nSize;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(nSize));
    for (std::int16_t i = 0; i < nSize; ++i) {
        PS_GACHA_INFO info;
        packet.XParse >> info.byType;
        packet.XParse >> info.nItemID;
        packet.XParse >> info.nCount;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_GACHA_LIST& value) {
    std::int16_t nSize = static_cast<std::int16_t>(value.vecInfo.size());
    packet.XParse << nSize;
    for (const PS_GACHA_INFO& info : value.vecInfo) {
        packet.XParse << info.byType;
        packet.XParse << info.nItemID;
        packet.XParse << info.nCount;
    }
    return packet;
}


/**
 * @brief 外观信息
 * Per IDA 0x1400554F0: ST_APPEARANCE_INFO
 */
struct ST_APPEARANCE_INFO {
    std::uint16_t wAppearanceID = 0;
    std::int64_t biEndDate = 0;
};

/**
 * @brief 外观列表
 * Per IDA 0x1400554F0: ST_APPEARANCE_LIST
 */
struct ST_APPEARANCE_LIST {
    std::vector<ST_APPEARANCE_INFO> vecInfo;
};

inline XPacket& operator>>(XPacket& packet, ST_APPEARANCE_LIST& value) {
    std::int16_t nSize = 0;
    packet.XParse >> nSize;
    value.vecInfo.resize(nSize);
    for (ST_APPEARANCE_INFO& info : value.vecInfo) {
        packet.XParse >> info.wAppearanceID;
        packet.XParse >> info.biEndDate;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_APPEARANCE_LIST& value) {
    std::int16_t nSize = static_cast<std::int16_t>(value.vecInfo.size());
    packet.XParse << nSize;
    for (const ST_APPEARANCE_INFO& info : value.vecInfo) {
        packet.XParse << info.wAppearanceID;
        packet.XParse << info.biEndDate;
    }
    return packet;
}

// ST_ITEM_SOCKET 和 PS_ITEM_SOCKET_LIST 已在 PSServer.h 中定义

/**
 * @brief 删除预约物品信息
 * Per IDA 0x1400581F0: PS_DELETE_RESERVE_ITEM
 */
struct PS_DELETE_RESERVE_ITEM {
    std::uint32_t dwItemID = 0;
    std::int32_t nCount = 0;
    std::int32_t nMapID = 0;
};

inline void operator>>(XPacket& packet, PS_DELETE_RESERVE_ITEM& value) {
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.nCount;
    packet.XParse >> value.nMapID;
}

inline XPacket& operator<<(XPacket& packet, const PS_DELETE_RESERVE_ITEM& value) {
    packet.XParse << value.dwItemID;
    packet.XParse << value.nCount;
    packet.XParse << value.nMapID;
    return packet;
}

/**
 * @brief 物品删除预约列表
 * Per IDA 0x140058CE0: PS_DELETE_RESERVE_ITEM_LIST
 */
struct PS_DELETE_RESERVE_ITEM_LIST {
    std::vector<PS_DELETE_RESERVE_ITEM> vecInfo;
};

inline void operator>>(XPacket& packet, PS_DELETE_RESERVE_ITEM_LIST& value) {
    std::uint16_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        PS_DELETE_RESERVE_ITEM info;
        packet >> info;
        value.vecInfo.push_back(info);
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_DELETE_RESERVE_ITEM_LIST& value) {
    packet.XParse << static_cast<std::uint16_t>(value.vecInfo.size());
    for (const PS_DELETE_RESERVE_ITEM& info : value.vecInfo) {
        packet << info;
    }
    return packet;
}

/**
 * @brief 系统日志结构体
 * Per IDA 0x1400CE040: ST_LOG_SYSTEM (464 bytes)
 */
struct ST_LOG_SYSTEM {
    std::int32_t nUAID = 0;
    std::int32_t nUCID = 0;
    std::int16_t sType = 0;
    std::int32_t nParam0 = 0;
    std::int32_t nParam1 = 0;
    std::int32_t nParam2 = 0;
    std::int32_t nParam3 = 0;
    std::int32_t nParam4 = 0;
    std::int32_t nParam5 = 0;
    std::int32_t nParam6 = 0;
    std::int32_t nParam7 = 0;
    std::int64_t nParam8 = 0;
    std::int64_t nParam9 = 0;
    wchar_t szParam10[200] = {};
};

inline void operator>>(XPacket& packet, ST_LOG_SYSTEM& value) {
    packet.XParse >> value.nUAID;
    packet.XParse >> value.nUCID;
    packet.XParse >> value.sType;
    packet.XParse >> value.nParam0;
    packet.XParse >> value.nParam1;
    packet.XParse >> value.nParam2;
    packet.XParse >> value.nParam3;
    packet.XParse >> value.nParam4;
    packet.XParse >> value.nParam5;
    packet.XParse >> value.nParam6;
    packet.XParse >> value.nParam7;
    packet.XParse >> value.nParam8;
    packet.XParse >> value.nParam9;
    // szParam10 是固定 wchar_t 数组
    short outLen = 0;
    packet.XParse.GetWString(value.szParam10, 200, outLen);
}

inline XPacket& operator<<(XPacket& packet, const ST_LOG_SYSTEM& value) {
    packet.XParse << value.nUAID;
    packet.XParse << value.nUCID;
    packet.XParse << value.sType;
    packet.XParse << value.nParam0;
    packet.XParse << value.nParam1;
    packet.XParse << value.nParam2;
    packet.XParse << value.nParam3;
    packet.XParse << value.nParam4;
    packet.XParse << value.nParam5;
    packet.XParse << value.nParam6;
    packet.XParse << value.nParam7;
    packet.XParse << value.nParam8;
    packet.XParse << value.nParam9;
    packet.XParse << FixedWideArrayToWString(value.szParam10);
    return packet;
}

