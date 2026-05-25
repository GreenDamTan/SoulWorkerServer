#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerMisc.h - 杂项结构体及序列化运算符
//
// 此文件包含防沉迷、优惠券、自动封禁等杂项数据结构和序列化操作：
// - PS_INDULGENCE_INFO - 防沉迷信息
// - PS_DB_USE_COUPON_REQ/RES - 优惠券使用请求/响应
// - PS_DB_CHECK_AUTO_BLOCK_INFO - 自动封禁检查信息
// - PS_USER_UPDATE_AUTH_TYPE - 用户认证类型更新
// ============================================================================

// ============================================================================
// 用户认证类型更新
// ============================================================================

struct PS_USER_UPDATE_AUTH_TYPE {
    std::uint32_t dwUAID = 0;
    std::uint8_t byAuthType = 0;
};

inline void operator>>(XPacket& packet, PS_USER_UPDATE_AUTH_TYPE& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.byAuthType;
}

// ============================================================================
// 防沉迷信息
// ============================================================================

// 对齐 IDA: 防沉迷信息
struct PS_INDULGENCE_INFO {
    std::uint32_t dwUAID = 0;
    bool bLogin = false;
    std::int32_t nIndulgenceConnectTerm = 0;
    std::int32_t nIndulgenceDisconnectTerm = 0;
    std::int32_t nLastDisConnectTime = 0;
    std::int32_t nLeftAlertTime = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_INDULGENCE_INFO& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.bLogin;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_INDULGENCE_INFO& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.bLogin;
    packet.XParse << value.nIndulgenceConnectTerm;
    packet.XParse << value.nIndulgenceDisconnectTerm;
    packet.XParse << value.nLastDisConnectTime;
    packet.XParse << value.nLeftAlertTime;
    return packet;
}

// ============================================================================
// 优惠券使用
// ============================================================================

// 对齐 IDA: 优惠券使用请求
struct PS_DB_USE_COUPON_REQ {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    char szCouponCode[21] = {};
};

inline XPacket& operator>>(XPacket& packet, PS_DB_USE_COUPON_REQ& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    short outLen = 0;
    packet.XParse.GetString(value.szCouponCode, 21, &outLen);
    return packet;
}

// 对齐 IDA: 优惠券使用响应
struct PS_DB_USE_COUPON_RES {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    char szCouponCode[21] = {};
    std::int32_t nError = 0;
    std::int32_t nItem = 0;
    std::int32_t nItemCount = 0;
    std::int32_t nEventID = 0;
    std::uint8_t byCouponType = 0;
};

inline XPacket& operator<<(XPacket& packet, const PS_DB_USE_COUPON_RES& value) {
    packet.XParse << value.dwUAID;
    packet.XParse << value.dwUCID;
    packet.XParse << std::string(value.szCouponCode);
    packet.XParse << value.nError;
    packet.XParse << value.nItem;
    packet.XParse << value.nItemCount;
    packet.XParse << value.nEventID;
    packet.XParse << value.byCouponType;
    return packet;
}

// ============================================================================
// 自动封禁检查
// ============================================================================

// 对齐 IDA: 自动封禁检查信息
struct PS_DB_CHECK_AUTO_BLOCK_INFO {
    char szAccountID[21] = {};
    std::uint8_t byTypeDB = 0;
    char szEndDate[24] = {};
    wchar_t strComment[101] = {};
    wchar_t strUserComment[101] = {};
};

inline XPacket& operator>>(XPacket& packet, PS_DB_CHECK_AUTO_BLOCK_INFO& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse >> value.byTypeDB;
    packet.XParse.GetString(value.szEndDate, 24, &outLen);
    packet.XParse.GetWString(value.strComment, 101, outLen);
    packet.XParse.GetWString(value.strUserComment, 101, outLen);
    return packet;
}

// ============================================================================
// NPC 信用度 (NPC Credit)
// ============================================================================

// 对齐 IDA: NPC 信用度信息 (24 bytes)
struct PS_NPC_CREDIT {
    std::int32_t nGroupID = 0;          // offset 0x00: 分组ID
    std::uint8_t byGrade = 0;           // offset 0x04: 等级
    std::uint8_t _pad0[3] = {};         // padding
    std::int32_t nPoint = 0;            // offset 0x08: 点数
    std::int16_t shCurPoint = 0;        // offset 0x0C: 当前点数
    std::uint8_t _pad1[2] = {};         // padding
    std::int64_t nUpdateDate = 0;       // offset 0x10: 更新日期
};

static_assert(sizeof(PS_NPC_CREDIT) == 24, "PS_NPC_CREDIT size must match IDA");

// 对齐 IDA: NPC 信用度列表 (32 bytes)
struct PS_NPC_CREDIT_LIST {
    std::vector<PS_NPC_CREDIT> vecInfo; // offset 0x00: 信用度列表
};

static_assert(sizeof(PS_NPC_CREDIT_LIST) == 32, "PS_NPC_CREDIT_LIST size must match IDA");

// PS_NPC_CREDIT 序列化运算符
inline void operator>>(XPacket& packet, PS_NPC_CREDIT& value) {
    packet.XParse >> value.nGroupID;
    packet.XParse >> value.byGrade;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad0), sizeof(value._pad0));
    packet.XParse >> value.nPoint;
    packet.XParse >> value.shCurPoint;
    packet.XParse.GetBytes(reinterpret_cast<char*>(value._pad1), sizeof(value._pad1));
    packet.XParse >> value.nUpdateDate;
}

inline XPacket& operator<<(XPacket& packet, const PS_NPC_CREDIT& value) {
    packet.XParse << value.nGroupID;
    packet.XParse << value.byGrade;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.nPoint;
    packet.XParse << value.shCurPoint;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad1), sizeof(value._pad1));
    packet.XParse << value.nUpdateDate;
    return packet;
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_NPC_CREDIT& value) {
    packet.XParse << value.nGroupID;
    packet.XParse << value.byGrade;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad0), sizeof(value._pad0));
    packet.XParse << value.nPoint;
    packet.XParse << value.shCurPoint;
    packet.XParse.SetBytes(reinterpret_cast<const char*>(value._pad1), sizeof(value._pad1));
    packet.XParse << value.nUpdateDate;
    return packet;
}

// PS_NPC_CREDIT_LIST 序列化运算符
inline void operator>>(XPacket& packet, PS_NPC_CREDIT_LIST& value) {
    std::int16_t shCount = 0;
    packet.XParse >> shCount;
    value.vecInfo.clear();
    value.vecInfo.reserve(static_cast<std::size_t>(shCount));
    for (std::int16_t i = 0; i < shCount; ++i) {
        PS_NPC_CREDIT item{};
        packet >> item;
        value.vecInfo.push_back(item);
    }
}

inline XSendDBPacket& operator<<(XSendDBPacket& packet, const PS_NPC_CREDIT_LIST& value) {
    std::int16_t shCount = static_cast<std::int16_t>(value.vecInfo.size());
    packet.XParse << shCount;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_NPC_CREDIT_LIST& value) {
    std::int16_t shCount = static_cast<std::int16_t>(value.vecInfo.size());
    packet.XParse << shCount;
    for (const auto& item : value.vecInfo) {
        packet << item;
    }
    return packet;
}
