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

// ST_MYROOM_OWNER_INFO 反序列化 (对齐 IDA)
inline XPacket& operator>>(XPacket& packet, ST_MYROOM_OWNER_INFO& value) {
    int nUAID = 0;
    int nUCID = 0;
    packet.XParse >> nUAID;
    packet.XParse >> nUCID;
    value.dwUAID = static_cast<std::uint32_t>(nUAID);
    value.dwUCID = static_cast<std::uint32_t>(nUCID);
    short outLen = 0;
    packet.XParse.GetWString(value.szName, static_cast<short>(sizeof(value.szName) / sizeof(wchar_t)), outLen);
    return packet;
}

// ST_MYROOM_USER 反序列化 (对齐 IDA)
inline XPacket& operator>>(XPacket& packet, ST_MYROOM_USER& value) {
    packet.XParse >> value.nServerID;
    int nUCID = 0;
    packet.XParse >> nUCID;
    value.dwUCID = static_cast<std::uint32_t>(nUCID);
    return packet;
}