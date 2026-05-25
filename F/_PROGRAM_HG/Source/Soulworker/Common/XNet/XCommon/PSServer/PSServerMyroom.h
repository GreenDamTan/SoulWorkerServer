#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerMyroom.h - Myroom/花粉互助相关结构体及序列化运算符
//
// 此文件包含 Myroom 相关的所有数据结构和序列化操作：
// - PS_MYROOM_POLLEN_HELP_USER - 花粉互助用户信息
// - PS_REQ_CLEAR_USER_STATE - 清除用户状态请求
// ============================================================================

// ============================================================================
// Myroom 相关结构体
// ============================================================================

/**
 * @brief 花粉互助用户信息。
 * 对齐 IDA 0x1400D94A0。
 */
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

// ============================================================================
// Myroom 序列化运算符
// ============================================================================

// PS_MYROOM_POLLEN_HELP_USER 反序列化
// 对齐 IDA 0x1400DED80
inline void operator>>(XPacket& packet, PS_MYROOM_POLLEN_HELP_USER& value) {
    packet.XParse >> value.byClass;
    packet.XParse >> value.byAwaken;
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.dwUCID;
    short sLen = 0;
    packet.XParse.GetWString(value.szName, 21, sLen);
}

// PS_MYROOM_POLLEN_HELP_USER 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MYROOM_POLLEN_HELP_USER& value) {
    packet.XParse << value.byClass;
    packet.XParse << value.byAwaken;
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.dwUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szName);
    return packet;
}

// PS_REQ_CLEAR_USER_STATE 序列化
// 对齐 IDA 0x1400B3160
inline XPacket& operator<<(XPacket& packet, const PS_REQ_CLEAR_USER_STATE& value) {
    const std::uint16_t count = static_cast<std::uint16_t>(std::min<std::size_t>(value.vecUserID.size(), 0xFFFF));
    packet.XParse << count;
    for (std::uint16_t i = 0; i < count; ++i) {
        packet.XParse << value.vecUserID[i];
    }
    return packet;
}

// PS_REQ_CLEAR_USER_STATE 反序列化
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
