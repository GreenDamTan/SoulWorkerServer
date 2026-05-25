#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerModeMaze.h - ModeMaze 匹配相关结构体及序列化运算符
//
// 此文件包含 ModeMaze 匹配系统的所有数据结构和序列化操作：
// - ST_MODE_MAZE_MEMBER_INFO - ModeMaze 成员信息
// - PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ - 匹配进入请求
// - PS_MODE_MAZE_MATCHING_EXIT - 匹配退出请求
// - PS_MODE_MAZE_MATCHING_WAIT - 匹配等待通知
// - PS_MODE_MAZE_MATCHING_ENTER_RES - 匹配进入响应
// - PS_SERVER_MODE_MAZE_MATCHING_EVENT - 匹配事件同步
// - PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO - 匹配时间信息
// ============================================================================

// ============================================================================
// ModeMaze 匹配结构体
// ============================================================================

/**
 * @brief ModeMaze 匹配成员信息。
 */
struct ST_MODE_MAZE_MEMBER_INFO {
    std::uint32_t dwActorID = 0;
    std::uint32_t dwUAID = 0;
    std::uint16_t wMapID = 0;
    std::uint8_t byClass = 0;
    std::uint8_t byLevel = 0;
    std::uint8_t byAwaken = 0;
    std::uint8_t _pad0[3] = {};
    std::uint32_t dwProfilePhotoID = 0;
    UXMapID uxMapID{};
    wchar_t strName[21] = {};
    std::int32_t nState = 0;
    std::int32_t nFirstJumpID = 0;
};

/**
 * @brief ModeMaze 匹配进入请求。
 */
struct PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ {
    ST_MODE_MAZE_MEMBER_INFO stMemberInfo{};
    std::uint16_t wModeMazeID = 0;
    std::uint16_t wRank = 0;
};

/**
 * @brief ModeMaze 匹配退出请求。
 */
struct PS_MODE_MAZE_MATCHING_EXIT {
    std::uint32_t dwExitUCID = 0;
    std::uint32_t dwExitUAID = 0;
    std::uint8_t byReason = 0;
    std::uint8_t _pad0[3] = {};
};

/**
 * @brief ModeMaze 匹配等待通知。
 */
struct PS_MODE_MAZE_MATCHING_WAIT {
    std::uint32_t dwMatchingID = 0;
    std::uint16_t wModeMazeID = 0;
    std::uint16_t _pad0 = 0;
    std::uint32_t dwActorID = 0;
    std::vector<ST_MODE_MAZE_MEMBER_INFO> vecMemberInfo;
};

/**
 * @brief ModeMaze 匹配进入响应。
 */
struct PS_MODE_MAZE_MATCHING_ENTER_RES {
    std::uint32_t dwActorID = 0;
    std::uint16_t wModeMazeID = 0;
    std::int32_t nError = 0;
};

/**
 * @brief ModeMaze 匹配事件同步请求。
 */
struct PS_SERVER_MODE_MAZE_MATCHING_EVENT {
    std::int32_t nID = 0;
    std::int32_t nModeMazeID = 0;
    std::vector<unsigned long> vecInfo;
};

/**
 * @brief ModeMaze 匹配时间信息。
 * 对齐 IDA: 28 字节，dwModeMazeID 是 unsigned int
 */
struct PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO {
    std::uint32_t dwModeMazeID = 0;     // offset 0x0
    std::int32_t nHotTime_Start_1st = 0; // offset 0x4
    std::int32_t nHotTime_End_1st = 0;   // offset 0x8
    std::int32_t nHotTime_Start_2nd = 0; // offset 0xC
    std::int32_t nHotTime_End_2nd = 0;   // offset 0x10
    std::int32_t nHotTime_Start_3rd = 0; // offset 0x14
    std::int32_t nHotTime_End_3rd = 0;   // offset 0x18
};

// ============================================================================
// Static Assertions - 大小和偏移验证
// ============================================================================

static_assert(sizeof(ST_MODE_MAZE_MEMBER_INFO) == 0x58, "ST_MODE_MAZE_MEMBER_INFO size must match PDB");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, wMapID) == 0x8,
              "ST_MODE_MAZE_MEMBER_INFO.wMapID offset mismatch");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, dwProfilePhotoID) == 0x10,
              "ST_MODE_MAZE_MEMBER_INFO.dwProfilePhotoID offset mismatch");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, uxMapID) == 0x18,
              "ST_MODE_MAZE_MEMBER_INFO.uxMapID offset mismatch");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, strName) == 0x20,
              "ST_MODE_MAZE_MEMBER_INFO.strName offset mismatch");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, nState) == 0x4C,
              "ST_MODE_MAZE_MEMBER_INFO.nState offset mismatch");
static_assert(offsetof(ST_MODE_MAZE_MEMBER_INFO, nFirstJumpID) == 0x50,
              "ST_MODE_MAZE_MEMBER_INFO.nFirstJumpID offset mismatch");

static_assert(sizeof(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ) == 0x60,
              "PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ size must match PDB");
static_assert(offsetof(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ, wModeMazeID) == 0x58,
              "PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ.wModeMazeID offset mismatch");
static_assert(offsetof(PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ, wRank) == 0x5A,
              "PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ.wRank offset mismatch");

static_assert(sizeof(PS_MODE_MAZE_MATCHING_EXIT) == 0x0C,
              "PS_MODE_MAZE_MATCHING_EXIT size must match PDB");
static_assert(offsetof(PS_MODE_MAZE_MATCHING_EXIT, dwExitUAID) == 0x4,
              "PS_MODE_MAZE_MATCHING_EXIT.dwExitUAID offset mismatch");
static_assert(offsetof(PS_MODE_MAZE_MATCHING_EXIT, byReason) == 0x8,
              "PS_MODE_MAZE_MATCHING_EXIT.byReason offset mismatch");

static_assert(sizeof(PS_SERVER_MODE_MAZE_MATCHING_EVENT) == 0x28,
              "PS_SERVER_MODE_MAZE_MATCHING_EVENT size must match PDB");
static_assert(offsetof(PS_SERVER_MODE_MAZE_MATCHING_EVENT, nModeMazeID) == 0x4,
              "PS_SERVER_MODE_MAZE_MATCHING_EVENT.nModeMazeID offset mismatch");
static_assert(offsetof(PS_SERVER_MODE_MAZE_MATCHING_EVENT, vecInfo) == 0x8,
              "PS_SERVER_MODE_MAZE_MATCHING_EVENT.vecInfo offset mismatch");

// ============================================================================
// ModeMaze 匹配序列化运算符
// ============================================================================

// ST_MODE_MAZE_MEMBER_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_MODE_MAZE_MEMBER_INFO& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.dwUAID;
    packet.XParse << value.wMapID;
    packet.XParse << value.byClass;
    packet.XParse << value.byLevel;
    packet.XParse << value.byAwaken;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << value.dwProfilePhotoID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << std::wstring(value.strName);
    packet.XParse << value.nState;
    packet.XParse << value.nFirstJumpID;
    return packet;
}

inline void operator>>(XPacket& packet, ST_MODE_MAZE_MEMBER_INFO& value) {
    packet.XParse >> value.dwActorID;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.wMapID;
    packet.XParse >> value.byClass;
    packet.XParse >> value.byLevel;
    packet.XParse >> value.byAwaken;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse >> value.dwProfilePhotoID;
    packet.XParse >> value.uxMapID.nMapID;
    short outLen = 0;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse >> value.nState;
    packet.XParse >> value.nFirstJumpID;
}

// PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ 反序列化
inline void operator>>(XPacket& packet, PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ& value) {
    packet >> value.stMemberInfo;
    packet.XParse >> value.wModeMazeID;
    packet.XParse >> value.wRank;
}

// PS_MODE_MAZE_MATCHING_EXIT 反序列化
inline void operator>>(XPacket& packet, PS_MODE_MAZE_MATCHING_EXIT& value) {
    packet.XParse >> value.dwExitUCID;
    packet.XParse >> value.dwExitUAID;
    packet.XParse >> value.byReason;
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
    packet.XParse.GetBYTE();
}

// PS_MODE_MAZE_MATCHING_EXIT 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MODE_MAZE_MATCHING_EXIT& value) {
    packet.XParse << value.dwExitUCID;
    packet.XParse << value.dwExitUAID;
    packet.XParse << value.byReason;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << static_cast<std::uint8_t>(0);
    return packet;
}

// PS_MODE_MAZE_MATCHING_WAIT 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MODE_MAZE_MATCHING_WAIT& value) {
    packet.XParse << value.dwMatchingID;
    packet.XParse << value.wModeMazeID;
    packet.XParse << value._pad0;
    packet.XParse << value.dwActorID;
    packet.XParse << static_cast<std::int16_t>(static_cast<int>(value.vecMemberInfo.size()));
    for (const auto& member : value.vecMemberInfo) {
        packet << member;
    }
    return packet;
}

// PS_MODE_MAZE_MATCHING_ENTER_RES 序列化
inline XPacket& operator<<(XPacket& packet, const PS_MODE_MAZE_MATCHING_ENTER_RES& value) {
    packet.XParse << value.dwActorID;
    packet.XParse << value.wModeMazeID;
    packet.XParse << value.nError;
    return packet;
}

// PS_SERVER_MODE_MAZE_MATCHING_EVENT 反序列化
inline void operator>>(XPacket& packet, PS_SERVER_MODE_MAZE_MATCHING_EVENT& value) {
    packet.XParse >> value.nID;
    packet.XParse >> value.nModeMazeID;
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        unsigned int item = 0;
        packet.XParse >> item;
        value.vecInfo.push_back(static_cast<unsigned long>(item));
    }
}

// PS_SERVER_MODE_MAZE_MATCHING_EVENT 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_MODE_MAZE_MATCHING_EVENT& value) {
    packet.XParse << value.nID;
    packet.XParse << value.nModeMazeID;
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet.XParse << static_cast<unsigned int>(item);
    }
    return packet;
}

// PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const PS_SERVER_MODE_MAZE_MATCHING_TIME_INFO& value) {
    packet.XParse << value.dwModeMazeID;
    packet.XParse << value.nHotTime_Start_1st;
    packet.XParse << value.nHotTime_End_1st;
    packet.XParse << value.nHotTime_Start_2nd;
    packet.XParse << value.nHotTime_End_2nd;
    packet.XParse << value.nHotTime_Start_3rd;
    packet.XParse << value.nHotTime_End_3rd;
    return packet;
}
