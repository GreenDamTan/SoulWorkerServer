#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerAttendance.h - 签到系统相关结构体及序列化运算符
//
// 此文件包含签到系统的所有数据结构和序列化操作：
// - 签到信息
// - 连续签到
// - 签到游戏时间
// ============================================================================

// ============================================================================
// 签到系统结构体 (IDA-verified from GameServer.exe)
// ============================================================================

// IDA-verified: PS_ATTENDANCE_INFO - 签到信息 (128 bytes)
// Members ordered by IDA offset
struct PS_ATTENDANCE_INFO {
    std::uint32_t dwAttendanceID = 0;        // offset 0, size 4
    // padding 4 bytes at offset 4
    std::int64_t nAttendance[14] = {};       // offset 8, size 112 (int64 array!)
    std::uint8_t byAttendanceCount = 0;      // offset 120, size 1
    std::uint8_t byApplyAttendance = 0;      // offset 121, size 1
    // padding 2 bytes at offset 122
    std::uint32_t dwType = 0;                // offset 124, size 4
};

// IDA-verified: PS_ATTENDANCE_CONTINUE - 连续签到信息 (24 bytes)
// Members ordered by IDA offset
struct PS_ATTENDANCE_CONTINUE {
    std::uint8_t byAttendanceCount = 0;      // offset 0, size 1
    // padding 7 bytes at offset 1
    std::int64_t nLastAttendanceDate = 0;    // offset 8, size 8
    std::uint8_t byApplyAttendance = 0;      // offset 16, size 1
    // padding 3 bytes at offset 17
    std::uint32_t dwType = 0;                // offset 20, size 4
};

// IDA-verified: PS_ATTENDANCE_PLAY_TIME - 签到游戏时间 (32 bytes)
// Members ordered by IDA offset
struct PS_ATTENDANCE_PLAY_TIME {
    std::uint8_t byCurPos = 0;               // offset 0, size 1
    // padding 7 bytes at offset 1
    std::int64_t nPlaySec = 0;               // offset 8, size 8
    std::int64_t nUpdateDate = 0;            // offset 16, size 8
    std::uint8_t byApplyAttendance = 0;      // offset 24, size 1
    // padding 3 bytes at offset 25
    std::uint32_t dwType = 0;                // offset 28, size 4
};

// ============================================================================
// 签到系统序列化运算符 (IDA-verified)
// Note: 序列化顺序与结构体内存布局一致，padding 字节会被自动处理
// ============================================================================

// PS_ATTENDANCE_INFO 序列化 (IDA-verified order)
inline void operator>>(XPacket& packet, PS_ATTENDANCE_INFO& value) {
    packet.XParse >> value.dwAttendanceID;
    // Skip padding 4 bytes - read as raw
    std::uint32_t padding1;
    packet.XParse >> padding1;
    for (int i = 0; i < 14; ++i) {
        packet.XParse >> value.nAttendance[i];
    }
    packet.XParse >> value.byAttendanceCount;
    packet.XParse >> value.byApplyAttendance;
    // Skip padding 2 bytes - read as raw
    std::uint16_t padding2;
    packet.XParse >> padding2;
    packet.XParse >> value.dwType;
}

inline XPacket& operator<<(XPacket& packet, const PS_ATTENDANCE_INFO& value) {
    packet.XParse << value.dwAttendanceID;
    // Write padding 4 bytes as zero
    packet.XParse << std::uint32_t(0);
    for (int i = 0; i < 14; ++i) {
        packet.XParse << value.nAttendance[i];
    }
    packet.XParse << value.byAttendanceCount;
    packet.XParse << value.byApplyAttendance;
    // Write padding 2 bytes as zero
    packet.XParse << std::uint16_t(0);
    packet.XParse << value.dwType;
    return packet;
}

// PS_ATTENDANCE_CONTINUE 序列化 (IDA-verified order)
inline void operator>>(XPacket& packet, PS_ATTENDANCE_CONTINUE& value) {
    packet.XParse >> value.byAttendanceCount;
    // Skip padding 7 bytes - read as raw
    std::uint8_t padding7[7];
    for (int i = 0; i < 7; ++i) {
        packet.XParse >> padding7[i];
    }
    packet.XParse >> value.nLastAttendanceDate;
    packet.XParse >> value.byApplyAttendance;
    // Skip padding 3 bytes - read as raw
    std::uint8_t padding3[3];
    for (int i = 0; i < 3; ++i) {
        packet.XParse >> padding3[i];
    }
    packet.XParse >> value.dwType;
}

inline XPacket& operator<<(XPacket& packet, const PS_ATTENDANCE_CONTINUE& value) {
    packet.XParse << value.byAttendanceCount;
    // Write padding 7 bytes as zero
    for (int i = 0; i < 7; ++i) {
        packet.XParse << std::uint8_t(0);
    }
    packet.XParse << value.nLastAttendanceDate;
    packet.XParse << value.byApplyAttendance;
    // Write padding 3 bytes as zero
    for (int i = 0; i < 3; ++i) {
        packet.XParse << std::uint8_t(0);
    }
    packet.XParse << value.dwType;
    return packet;
}

// PS_ATTENDANCE_PLAY_TIME 序列化 (IDA-verified order)
inline void operator>>(XPacket& packet, PS_ATTENDANCE_PLAY_TIME& value) {
    packet.XParse >> value.byCurPos;
    // Skip padding 7 bytes - read as raw
    std::uint8_t padding7[7];
    for (int i = 0; i < 7; ++i) {
        packet.XParse >> padding7[i];
    }
    packet.XParse >> value.nPlaySec;
    packet.XParse >> value.nUpdateDate;
    packet.XParse >> value.byApplyAttendance;
    // Skip padding 3 bytes - read as raw
    std::uint8_t padding3[3];
    for (int i = 0; i < 3; ++i) {
        packet.XParse >> padding3[i];
    }
    packet.XParse >> value.dwType;
}

inline XPacket& operator<<(XPacket& packet, const PS_ATTENDANCE_PLAY_TIME& value) {
    packet.XParse << value.byCurPos;
    // Write padding 7 bytes as zero
    for (int i = 0; i < 7; ++i) {
        packet.XParse << std::uint8_t(0);
    }
    packet.XParse << value.nPlaySec;
    packet.XParse << value.nUpdateDate;
    packet.XParse << value.byApplyAttendance;
    // Write padding 3 bytes as zero
    for (int i = 0; i < 3; ++i) {
        packet.XParse << std::uint8_t(0);
    }
    packet.XParse << value.dwType;
    return packet;
}
