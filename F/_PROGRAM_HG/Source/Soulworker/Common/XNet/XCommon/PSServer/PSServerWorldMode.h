#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerWorldMode.h - World Mode 相关序列化运算符
//
// 此文件包含 World Mode 模式的所有序列化运算符：
// - ST_ENTER_WORLD_MODE_INFO
// - ST_WORLD_MODE_INFO, ST_WORLD_MODE_INFO_VEC
// - PS_WORLD_MODE_COMPLETE, PS_WORLD_MODE_UPDATE, PS_WORLD_MODE_FINISH
//
// 注意: World Mode 结构体定义在 PSCommon.h 中
// ============================================================================

// ============================================================================
// World Mode 反序列化运算符 (receive direction)
// ============================================================================

// 对齐 IDA: ST_ENTER_WORLD_MODE_INFO 反序列化
inline void operator>>(XPacket& packet, ST_ENTER_WORLD_MODE_INFO& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_WORLD_MODE item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

// 对齐 IDA: PS_WORLD_MODE_COMPLETE 反序列化
inline void operator>>(XPacket& packet, PS_WORLD_MODE_COMPLETE& value) {
    packet.XParse >> value.nModeID;
    packet.XParse >> value.uxMapID.nMapID;
    short outLen = 0;
    packet.XParse.GetWString(value.strKiller, 21, outLen);
    int nFinish = 0;
    packet.XParse >> nFinish;
    value.bFinish = (nFinish != 0);
    packet.XParse >> value.nModeDateID;
}

// 对齐 IDA: ST_WORLD_MODE_INFO_VEC 反序列化
inline void operator>>(XPacket& packet, ST_WORLD_MODE_INFO_VEC& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_WORLD_MODE_INFO item{};
        packet.XParse >> item.nStartTime;
        packet.XParse >> item.nFinishTime;
        packet.XParse >> item.nModeID;
        packet.XParse >> item.nState;
        int nSuccess = 0;
        packet.XParse >> nSuccess;
        item.bSuccess = (nSuccess != 0);
        packet.XParse >> item.nModeDateID;
        packet.XParse >> item.nMonsterClearCount;
        packet.XParse >> item.biModeStartTime;
        packet.XParse >> item.biModeEndTime;
        value.vecInfo.push_back(item);
    }
}

// 对齐 IDA: PS_WORLD_MODE_UPDATE 反序列化
inline void operator>>(XPacket& packet, PS_WORLD_MODE_UPDATE& value) {
    packet >> value.stInfo;
    packet.XParse >> value.uxMapID.nMapID;
    packet.XParse >> value.nMonsterClearCount;
    int nUpdate = 0;
    packet.XParse >> nUpdate;
    value.bUpdate = (nUpdate != 0);
}

// 对齐 IDA: ST_WORLD_MODE_INFO 反序列化
inline void operator>>(XPacket& packet, ST_WORLD_MODE_INFO& value) {
    int bSuccess = 0;
    packet.XParse >> value.nStartTime;
    packet.XParse >> value.nFinishTime;
    packet.XParse >> value.nModeID;
    packet.XParse >> value.nState;
    packet.XParse >> bSuccess;
    value.bSuccess = (bSuccess != 0);
    packet.XParse >> value.nModeDateID;
    packet.XParse >> value.nMonsterClearCount;
    packet.XParse >> value.biModeStartTime;
    packet.XParse >> value.biModeEndTime;
}

// ============================================================================
// World Mode 序列化运算符 (send direction)
// ============================================================================

// 对齐 IDA: PS_WORLD_MODE_UPDATE 序列化 (用于 0xFB/0x02)
inline XPacket& operator<<(XPacket& packet, const PS_WORLD_MODE_UPDATE& value) {
    packet << value.stInfo;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.nMonsterClearCount;
    packet.XParse << static_cast<int>(value.bUpdate ? 1 : 0);
    return packet;
}

// 对齐 IDA: ST_WORLD_MODE_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_WORLD_MODE_INFO& value) {
    packet.XParse << value.nStartTime;
    packet.XParse << value.nFinishTime;
    packet.XParse << value.nModeID;
    packet.XParse << value.nState;
    packet.XParse << static_cast<int>(value.bSuccess ? 1 : 0);
    packet.XParse << value.nModeDateID;
    packet.XParse << value.nMonsterClearCount;
    packet.XParse << value.biModeStartTime;
    packet.XParse << value.biModeEndTime;
    return packet;
}

// 对齐 IDA: ST_WORLD_MODE_INFO_VEC 序列化
inline XPacket& operator<<(XPacket& packet, const ST_WORLD_MODE_INFO_VEC& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA: PS_WORLD_MODE_COMPLETE 序列化
inline XPacket& operator<<(XPacket& packet, const PS_WORLD_MODE_COMPLETE& value) {
    packet.XParse << value.nModeID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << std::wstring(value.strKiller);
    packet.XParse << static_cast<int>(value.bFinish ? 1 : 0);
    packet.XParse << value.nModeDateID;
    return packet;
}

// 对齐 IDA: ST_ENTER_WORLD_MODE_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_ENTER_WORLD_MODE_INFO& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA: PS_WORLD_MODE_FINISH 序列化 (0xFB/0x04)
inline XPacket& operator<<(XPacket& packet, const PS_WORLD_MODE_FINISH& value) {
    packet.XParse << value.nModeID;
    packet.XParse << value.uxMapID.nMapID;
    packet.XParse << value.nFinishTime;
    packet.XParse << value.nModeDateID;
    packet.XParse << value.nMonsterClearCount;
    packet.XParse << std::wstring(value.strKiller);
    packet.XParse << static_cast<int>(value.bSuccess ? 1 : 0);
    return packet;
}
