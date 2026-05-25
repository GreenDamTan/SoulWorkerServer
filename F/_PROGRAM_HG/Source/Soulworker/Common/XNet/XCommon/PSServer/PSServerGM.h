#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerGM.h - GM 工具相关结构体及序列化运算符
//
// 此文件包含GM工具系统的所有数据结构和序列化操作：
// - GM用户踢出信息
// - GM公告信息
// - GM时间/数值事件
// - Banner信息
// ============================================================================

// ============================================================================
// ControlServer GM Tool Structures
// ============================================================================

// 对齐 IDA: GM用户踢出信息结构
struct ST_GM_USER_KICK_INFO {
    std::uint32_t dwNo = 0;
    std::uint32_t dwUAID = 0;
    std::int64_t nDate = 0;
    std::uint8_t byteUse = 0;
    std::uint8_t _pad0[1] = {};
    wchar_t wszMsg[513] = {};
};

static_assert(sizeof(ST_GM_USER_KICK_INFO) == 1048, "ST_GM_USER_KICK_INFO size must match IDA");

// 对齐 IDA: GM公告信息结构
struct ST_GM_NOTICE_INFO {
    std::uint32_t dwNo = 0;
    std::int16_t shType = 0;
    std::int16_t shViewType = 0;
    wchar_t strMsg[256] = {};
    wchar_t strColor[7] = {};
    std::int64_t nDate = 0;
    std::int64_t nEndTime = 0;
    std::uint8_t byTerm = 0;
    std::uint8_t byteUse = 0;
    std::uint8_t byteDel = 0;
};

static_assert(sizeof(ST_GM_NOTICE_INFO) == 560, "ST_GM_NOTICE_INFO size must match IDA");

// 对齐 IDA: GM时间事件信息结构
struct ST_GM_TIME_EVENT_INFO {
    std::uint32_t dwNo = 0;
    std::uint32_t dwServerIDX = 0;
    std::uint32_t dwBuff_ID = 0;
    std::int32_t dwBuff_Val = 0;
    std::int64_t nStartDate = 0;
    std::int64_t nEndDate = 0;
    std::uint8_t byteFlag = 0;
    std::uint8_t byteUse = 0;
    std::uint8_t byteClass = 0;
};

static_assert(sizeof(ST_GM_TIME_EVENT_INFO) == 40, "ST_GM_TIME_EVENT_INFO size must match IDA");

// 对齐 IDA: GM数值事件信息结构
struct ST_GM_VALUE_EVENT_INFO {
    std::uint32_t dwNo = 0;
    std::uint32_t dwServerIDX = 0;
    std::uint8_t byEventType = 0;
    std::uint8_t _pad0[3] = {};
    std::uint32_t dwEvent_Val = 0;
    std::int64_t nStartDate = 0;
    std::int64_t nEndDate = 0;
    std::uint8_t byteUse = 0;
};

static_assert(sizeof(ST_GM_VALUE_EVENT_INFO) == 40, "ST_GM_VALUE_EVENT_INFO size must match IDA");

// 对齐 IDA: GM数值事件列表
struct PS_GM_VALUE_EVENT_LIST {
    std::vector<ST_GM_VALUE_EVENT_INFO> vecEvent;
};

// 对齐 IDA: Banner信息结构
struct ST_BANNER_INFO {
    char szUrl[500] = {};
    std::int32_t nTime = 0;
    std::int32_t nNo = 0;
    std::int32_t nMain = 0;
    std::int32_t nSub = 0;
};

static_assert(sizeof(ST_BANNER_INFO) == 516, "ST_BANNER_INFO size must match IDA");

// 对齐 IDA: Banner列表
struct ST_BANNER_LIST {
    std::vector<ST_BANNER_INFO> vecInfo;
};

// ============================================================================
// ControlServer GM Tool Serialization Operators
// ============================================================================

// 对齐 IDA: ST_GM_USER_KICK_INFO 反序列化
inline void operator>>(XPacket& packet, ST_GM_USER_KICK_INFO& value) {
    packet.XParse >> value.dwNo;
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.nDate;
    packet.XParse >> value.byteUse;
    short outLen = 0;
    packet.XParse.GetWString(value.wszMsg, 513, outLen);
}

// 对齐 IDA: ST_GM_NOTICE_INFO 反序列化
inline void operator>>(XPacket& packet, ST_GM_NOTICE_INFO& value) {
    packet.XParse >> value.dwNo;
    packet.XParse >> value.shType;
    packet.XParse >> value.shViewType;
    short outLen = 0;
    packet.XParse.GetWString(value.strMsg, 256, outLen);
    packet.XParse.GetWString(value.strColor, 7, outLen);
    packet.XParse >> value.nDate;
    packet.XParse >> value.nEndTime;
    packet.XParse >> value.byTerm;
    packet.XParse >> value.byteUse;
    packet.XParse >> value.byteDel;
}

// 对齐 IDA: ST_GM_TIME_EVENT_INFO 反序列化
inline void operator>>(XPacket& packet, ST_GM_TIME_EVENT_INFO& value) {
    packet.XParse >> value.dwNo;
    packet.XParse >> value.dwServerIDX;
    packet.XParse >> value.dwBuff_ID;
    packet.XParse >> value.dwBuff_Val;
    packet.XParse >> value.nStartDate;
    packet.XParse >> value.nEndDate;
    packet.XParse >> value.byteFlag;
    packet.XParse >> value.byteUse;
    packet.XParse >> value.byteClass;
}

// 对齐 IDA: ST_GM_VALUE_EVENT_INFO 反序列化
inline void operator>>(XPacket& packet, ST_GM_VALUE_EVENT_INFO& value) {
    packet.XParse >> value.dwNo;
    packet.XParse >> value.dwServerIDX;
    packet.XParse >> value.byEventType;
    packet.XParse >> value.dwEvent_Val;
    packet.XParse >> value.nStartDate;
    packet.XParse >> value.nEndDate;
    packet.XParse >> value.byteUse;
}

// 对齐 IDA: PS_GM_VALUE_EVENT_LIST 反序列化
inline void operator>>(XPacket& packet, PS_GM_VALUE_EVENT_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecEvent.clear();
    value.vecEvent.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_GM_VALUE_EVENT_INFO item{};
        packet >> item;
        value.vecEvent.push_back(item);
    }
}

// 对齐 IDA: ST_BANNER_INFO 反序列化
inline void operator>>(XPacket& packet, ST_BANNER_INFO& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szUrl, 500, &outLen);
    packet.XParse >> value.nTime;
    packet.XParse >> value.nNo;
    packet.XParse >> value.nMain;
    packet.XParse >> value.nSub;
}

// 对齐 IDA: ST_BANNER_LIST 反序列化
inline void operator>>(XPacket& packet, ST_BANNER_LIST& value) {
    std::uint8_t count = 0;
    packet.XParse >> count;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(count));
    for (std::uint8_t index = 0; index < count; ++index) {
        ST_BANNER_INFO item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

// ============================================================================
// ControlServer GM Tool Serialization Operators (send direction)
// ============================================================================

// 对齐 IDA: ST_GM_TIME_EVENT_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_GM_TIME_EVENT_INFO& value) {
    packet.XParse << value.dwNo;
    packet.XParse << value.dwServerIDX;
    packet.XParse << value.dwBuff_ID;
    packet.XParse << value.dwBuff_Val;
    packet.XParse << value.nStartDate;
    packet.XParse << value.nEndDate;
    packet.XParse << value.byteFlag;
    packet.XParse << value.byteUse;
    packet.XParse << value.byteClass;
    return packet;
}

// 对齐 IDA: ST_GM_VALUE_EVENT_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_GM_VALUE_EVENT_INFO& value) {
    packet.XParse << value.dwNo;
    packet.XParse << value.dwServerIDX;
    packet.XParse << value.byEventType;
    packet.XParse << value.dwEvent_Val;
    packet.XParse << value.nStartDate;
    packet.XParse << value.nEndDate;
    packet.XParse << value.byteUse;
    return packet;
}

// 对齐 IDA: PS_GM_VALUE_EVENT_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const PS_GM_VALUE_EVENT_LIST& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecEvent.size());
    for (const auto& item : value.vecEvent) {
        packet << item;
    }
    return packet;
}

// 对齐 IDA: ST_BANNER_INFO 序列化
inline XPacket& operator<<(XPacket& packet, const ST_BANNER_INFO& value) {
    packet.XParse << std::string(value.szUrl);
    packet.XParse << value.nTime;
    packet.XParse << value.nNo;
    packet.XParse << value.nMain;
    packet.XParse << value.nSub;
    return packet;
}

// 对齐 IDA: ST_BANNER_LIST 序列化
inline XPacket& operator<<(XPacket& packet, const ST_BANNER_LIST& value) {
    packet.XParse << static_cast<std::uint8_t>(value.vecInfo.size());
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}
