// GreenDamTan_MyRoomStructs.h
// MyRoom 相关结构体定义 (共享层)

#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <cstdint>

// MyRoom 相关结构体定义
struct ST_MYROOM_USER {
    int nServerID = 0;
    std::uint32_t dwUCID = 0;
};

struct ST_MYROOM_OWNER_INFO {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    wchar_t szName[21] = {};
};

// ST_MYROOM_OWNER_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_MYROOM_OWNER_INFO& value) {
    packet.XParse << static_cast<int>(value.dwUAID);
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << std::wstring(value.szName);
    return packet;
}

// ST_MYROOM_USER 序列化
inline XPacket& operator<<(XPacket& packet, const ST_MYROOM_USER& value) {
    packet.XParse << value.nServerID;
    packet.XParse << static_cast<int>(value.dwUCID);
    return packet;
}