// GreenDamTan_MyRoomStructs.h
// MyRoom 相关结构体定义 (共享层)
// 对齐 IDA ControlServer.exe 类型定义

#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <cstdint>

// XVec3 已在 PSCommon.h 中定义，直接使用

// 对齐 IDA: ST_MYROOM_USER (28 bytes, 5 members)
// 用户进入 MyRoom 时携带的信息
struct ST_MYROOM_USER {
    int nServerID = 0;              // +0x00, 4 bytes
    std::uint32_t dwUAID = 0;       // +0x04, 4 bytes
    std::uint32_t dwUCID = 0;       // +0x08, 4 bytes
    std::uint32_t dwBeforeMapID = 0; // +0x0C, 4 bytes
    XVec3 xBeforePos{};             // +0x10, 12 bytes
};
static_assert(sizeof(ST_MYROOM_USER) == 28, "ST_MYROOM_USER size mismatch with IDA");

// 对齐 IDA: ST_MYROOM_OWNER_INFO (68 bytes, 10 members)
// MyRoom 所有者信息
struct ST_MYROOM_OWNER_INFO {
    std::uint32_t dwOwnerUAID = 0;      // +0x00, 4 bytes
    short shMapIndex = 0;               // +0x04, 2 bytes
    short shGridNo = 0;                 // +0x06, 2 bytes
    std::uint32_t dwMapID = 0;          // +0x08, 4 bytes
    std::uint8_t byRoomOpenLevel = 0;   // +0x0C, 1 byte
    // +0x0D padding (1 byte)
    wchar_t szRoomName[20] = {};        // +0x0E, 40 bytes (offset 14 to 54)
    int nRecommendCount = 0;            // +0x38 (56), 4 bytes
    int nFavoriteCount = 0;             // +0x3C (60), 4 bytes
    bool bFavorite = false;             // +0x40 (64), 1 byte
    bool bRecommend = false;            // +0x41 (65), 1 byte
    // +0x42, 2 bytes padding to 68 bytes
};
static_assert(sizeof(ST_MYROOM_OWNER_INFO) == 68, "ST_MYROOM_OWNER_INFO size mismatch with IDA");

// ST_MYROOM_OWNER_INFO 序列化 (对齐 IDA)
inline XPacket& operator<<(XPacket& packet, const ST_MYROOM_OWNER_INFO& value) {
    packet.XParse << static_cast<int>(value.dwOwnerUAID);
    packet.XParse << value.shMapIndex;
    packet.XParse << value.shGridNo;
    packet.XParse << static_cast<int>(value.dwMapID);
    packet.XParse << static_cast<int>(value.byRoomOpenLevel);
    packet.XParse << std::wstring(value.szRoomName);
    packet.XParse << value.nRecommendCount;
    packet.XParse << value.nFavoriteCount;
    packet.XParse << static_cast<int>(value.bFavorite ? 1 : 0);
    packet.XParse << static_cast<int>(value.bRecommend ? 1 : 0);
    return packet;
}

// ST_MYROOM_OWNER_INFO 反序列化 (对齐 IDA)
inline XPacket& operator>>(XPacket& packet, ST_MYROOM_OWNER_INFO& value) {
    int nOwnerUAID = 0;
    int nMapID = 0;
    int nRoomOpenLevel = 0;
    int nFavorite = 0;
    int nRecommend = 0;
    packet.XParse >> nOwnerUAID;
    value.dwOwnerUAID = static_cast<std::uint32_t>(nOwnerUAID);
    packet.XParse >> value.shMapIndex;
    packet.XParse >> value.shGridNo;
    packet.XParse >> nMapID;
    value.dwMapID = static_cast<std::uint32_t>(nMapID);
    packet.XParse >> nRoomOpenLevel;
    value.byRoomOpenLevel = static_cast<std::uint8_t>(nRoomOpenLevel);
    short outLen = 0;
    packet.XParse.GetWString(value.szRoomName, 20, outLen);
    packet.XParse >> value.nRecommendCount;
    packet.XParse >> value.nFavoriteCount;
    packet.XParse >> nFavorite;
    packet.XParse >> nRecommend;
    value.bFavorite = (nFavorite != 0);
    value.bRecommend = (nRecommend != 0);
    return packet;
}

// ST_MYROOM_USER 序列化 (对齐 IDA)
inline XPacket& operator<<(XPacket& packet, const ST_MYROOM_USER& value) {
    packet.XParse << value.nServerID;
    packet.XParse << static_cast<int>(value.dwUAID);
    packet.XParse << static_cast<int>(value.dwUCID);
    packet.XParse << static_cast<int>(value.dwBeforeMapID);
    packet.XParse << value.xBeforePos.x;
    packet.XParse << value.xBeforePos.y;
    packet.XParse << value.xBeforePos.z;
    return packet;
}

// ST_MYROOM_USER 反序列化 (对齐 IDA)
inline XPacket& operator>>(XPacket& packet, ST_MYROOM_USER& value) {
    packet.XParse >> value.nServerID;
    int nUAID = 0;
    int nUCID = 0;
    int nBeforeMapID = 0;
    packet.XParse >> nUAID;
    packet.XParse >> nUCID;
    packet.XParse >> nBeforeMapID;
    value.dwUAID = static_cast<std::uint32_t>(nUAID);
    value.dwUCID = static_cast<std::uint32_t>(nUCID);
    value.dwBeforeMapID = static_cast<std::uint32_t>(nBeforeMapID);
    packet.XParse >> value.xBeforePos.x;
    packet.XParse >> value.xBeforePos.y;
    packet.XParse >> value.xBeforePos.z;
    return packet;
}
