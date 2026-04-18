#pragma once

#include <algorithm>
#include <cstddef>
#include <string>

#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

/**
 * @brief 客户端选项位集合。
 *
 * 该结构来自 `common/xnet/xcommon/psoption.h`，当前登录服链路里主要用于：
 * 1. 登录后把客户端选项位同步给 AccountDB。
 * 2. 等待队列取消请求中作为附带载荷被读取，但当前分支不继续使用该值。
 */
struct ST_OPTION_BIT {
    char szOption[64] = {};
};

static_assert(sizeof(ST_OPTION_BIT) == 0x40, "ST_OPTION_BIT size must match PDB");

enum E_OPTION_INDEX : std::int32_t {
    eOption_WhisperMsg = 0x15,
    eOption_Register_Friend = 0x19,
    eOption_OtherInfo = 0x1A,
};

enum E_OPTION_STATE : std::int32_t {
    eGAME_OPTION_ALLOW_ALL = 0x0,
    eGAME_OPTION_ALLOW_PARTIAL = 0x1,
    eGAME_OPTION_REFUSE_ALL = 0x2,
};

/**
 * @brief GameServer 侧已经解析后的三枚常用选项位。
 *
 * 结合 `PS_USER_INFO_FOR_RELAY` 的 PDB 布局与 `CUser::SetGameOption`
 * 当前只确认：
 * - `nOption_WhisperMsg`
 * - `nOption_Register_Friend`
 * - `nOption_OtherInfo`
 *
 * 三项为稳定字段。
 */
struct ST_GAME_OPTION {
    int nOption_WhisperMsg = 0;
    int nOption_Register_Friend = 0;
    int nOption_OtherInfo = 0;
};

static_assert(sizeof(ST_GAME_OPTION) == 0x0C, "ST_GAME_OPTION size must match PDB");

/**
 * @brief 用户热键/按键配置。
 */
struct ST_USER_KEY_OPTION {
    unsigned int dwUAID = 0;
    char szKeyOption[2048] = {};
};

static_assert(sizeof(ST_USER_KEY_OPTION) == 0x804, "ST_USER_KEY_OPTION size must match PDB");
static_assert(offsetof(ST_USER_KEY_OPTION, szKeyOption) == 0x4, "ST_USER_KEY_OPTION.szKeyOption offset mismatch");

/**
 * @brief 控制服下发的内容开关集合。
 *
 * `CLoginControlSocket::RecvServerOptionUpdate` 会读取该结构，
 * 然后交给 `XLoginServer::SendServerOption_SecondPW` 更新全局内容开关。
 */
struct PS_CONTENTS_INFO {
    bool bContents[14] = {};
};

static_assert(sizeof(PS_CONTENTS_INFO) == 0xE, "PS_CONTENTS_INFO size must match PDB");

inline void operator>>(XPacket& packet, ST_OPTION_BIT& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szOption, 64, &outLen);
}

inline XPacket& operator<<(XPacket& packet, const ST_OPTION_BIT& value) {
    packet.XParse << std::string(value.szOption);
    return packet;
}

inline void operator>>(XPacket& packet, ST_GAME_OPTION& value) {
    packet.XParse >> value.nOption_WhisperMsg;
    packet.XParse >> value.nOption_Register_Friend;
    packet.XParse >> value.nOption_OtherInfo;
}

inline XPacket& operator<<(XPacket& packet, const ST_GAME_OPTION& value) {
    packet.XParse << value.nOption_WhisperMsg;
    packet.XParse << value.nOption_Register_Friend;
    packet.XParse << value.nOption_OtherInfo;
    return packet;
}

inline void operator>>(XPacket& packet, ST_USER_KEY_OPTION& value) {
    short outLen = 0;
    packet.XParse >> value.dwUAID;
    packet.XParse.GetString(value.szKeyOption, 2048, &outLen);
}

inline XPacket& operator<<(XPacket& packet, const ST_USER_KEY_OPTION& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << std::string(value.szKeyOption);
    return packet;
}

inline void operator>>(XPacket& packet, PS_CONTENTS_INFO& value) {
    for (bool& item : value.bContents) {
        packet.XParse >> item;
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_CONTENTS_INFO& value) {
    for (bool item : value.bContents) {
        packet.XParse << item;
    }
    return packet;
}
