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
// 签到系统结构体
// ============================================================================

// 对齐 IDA: PS_ATTENDANCE_INFO - 签到信息
struct PS_ATTENDANCE_INFO {
    std::uint32_t dwType = 0;
    std::uint32_t dwAttendanceID = 0;
    std::uint8_t byApplyAttendance = 0;
    std::uint8_t byAttendanceCount = 0;  // 签到计数
    std::int32_t nAttendance[14] = {};   // 签到状态数组
};

// 对齐 IDA: PS_ATTENDANCE_CONTINUE - 连续签到信息
struct PS_ATTENDANCE_CONTINUE {
    std::uint32_t dwType = 0;
    std::uint8_t byAttendanceCount = 0;    // 连续签到天数
    std::int64_t nLastAttendanceDate = 0;  // 上次签到日期
    std::uint8_t byApplyAttendance = 0;
    std::int32_t nAttendance[7] = {};      // 连续签到状态数组
};

// 对齐 IDA: PS_ATTENDANCE_PLAY_TIME - 签到游戏时间
struct PS_ATTENDANCE_PLAY_TIME {
    std::uint32_t dwType = 0;
    std::int64_t nPlaySec = 0;       // 游戏时间秒数
    std::uint8_t byCurPos = 0;       // 当前位置
    std::int64_t nUpdateDate = 0;    // 更新日期
    std::uint8_t byApplyAttendance = 0;
};

// ============================================================================
// 签到系统序列化运算符
// ============================================================================

// PS_ATTENDANCE_INFO 序列化
inline void operator>>(XPacket& packet, PS_ATTENDANCE_INFO& value) {
    packet.XParse >> value.dwType;
    packet.XParse >> value.dwAttendanceID;
    packet.XParse >> value.byApplyAttendance;
    for (int i = 0; i < 14; ++i) {
        packet.XParse >> value.nAttendance[i];
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ATTENDANCE_INFO& value) {
    packet.XParse << value.dwType;
    packet.XParse << value.dwAttendanceID;
    packet.XParse << value.byApplyAttendance;
    for (int i = 0; i < 14; ++i) {
        packet.XParse << value.nAttendance[i];
    }
    return packet;
}

// PS_ATTENDANCE_CONTINUE 序列化
inline void operator>>(XPacket& packet, PS_ATTENDANCE_CONTINUE& value) {
    packet.XParse >> value.dwType;
    packet.XParse >> value.byAttendanceCount;
    packet.XParse >> value.nLastAttendanceDate;
    packet.XParse >> value.byApplyAttendance;
    for (int i = 0; i < 7; ++i) {
        packet.XParse >> value.nAttendance[i];
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_ATTENDANCE_CONTINUE& value) {
    packet.XParse << value.dwType;
    packet.XParse << value.byAttendanceCount;
    packet.XParse << value.nLastAttendanceDate;
    packet.XParse << value.byApplyAttendance;
    for (int i = 0; i < 7; ++i) {
        packet.XParse << value.nAttendance[i];
    }
    return packet;
}

// PS_ATTENDANCE_PLAY_TIME 序列化
inline void operator>>(XPacket& packet, PS_ATTENDANCE_PLAY_TIME& value) {
    packet.XParse >> value.dwType;
    packet.XParse >> value.nPlaySec;
    packet.XParse >> value.byCurPos;
    packet.XParse >> value.nUpdateDate;
    packet.XParse >> value.byApplyAttendance;
}

inline XPacket& operator<<(XPacket& packet, const PS_ATTENDANCE_PLAY_TIME& value) {
    packet.XParse << value.dwType;
    packet.XParse << value.nPlaySec;
    packet.XParse << value.byCurPos;
    packet.XParse << value.nUpdateDate;
    packet.XParse << value.byApplyAttendance;
    return packet;
}
