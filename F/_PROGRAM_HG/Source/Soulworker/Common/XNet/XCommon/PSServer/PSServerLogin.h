#pragma once

// ============================================================================
// PSServerLogin.h - 登录平台相关结构体及序列化运算符
//
// 此文件包含各平台（TWN/CHN/GF/WM/SG）的登录请求结构体、
// 平台认证信息、账户相关结构以及 HAN 计费相关结构。
// ============================================================================

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// 平台认证信息结构体
// ============================================================================

// 对齐 IDA: WM平台认证信息
// IDA: ??0ST_WM_AUTH_INFO@@QEAA@XZ (0x140068700)
struct ST_WM_AUTH_INFO {
    std::int32_t nUAID = 0;
    char szWMAccountID[256] = {};
    char szWMHID[21] = {};
    char szToken[256] = {};
    std::uint8_t byUserType = 11;        // IDA: 默认值 11
    std::uint8_t byChannelType_WM = 11;  // IDA: 默认值 11
    std::uint8_t byChannelType = 1;      // IDA: 默认值 1
    std::uint8_t byAccessType = 11;      // IDA: 默认值 11
};

inline XPacket& operator>>(XPacket& packet, ST_WM_AUTH_INFO& value) {
    packet.XParse >> value.nUAID;
    short outLen = 0;
    packet.XParse.GetString(value.szWMAccountID, 256, &outLen);
    packet.XParse.GetString(value.szWMHID, 21, &outLen);
    packet.XParse.GetString(value.szToken, 256, &outLen);
    packet.XParse >> value.byUserType;
    packet.XParse >> value.byChannelType_WM;
    packet.XParse >> value.byChannelType;
    packet.XParse >> value.byAccessType;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const ST_WM_AUTH_INFO& value) {
    packet.XParse << value.nUAID;
    packet.XParse << std::string(value.szWMAccountID);
    packet.XParse << std::string(value.szWMHID);
    packet.XParse << std::string(value.szToken);
    packet.XParse << value.byUserType;
    packet.XParse << value.byChannelType_WM;
    packet.XParse << value.byChannelType;
    packet.XParse << value.byAccessType;
    return packet;
}

// ============================================================================
// 平台登录请求结构体
// ============================================================================

// 对齐 IDA 0x14007A610: TWN平台登录请求
struct PS_LOGIN_REQ_FOR_TWN {
    char szAuthCode[1025] = {};
    char szAccountID[21] = {};
    char szMacAddress[18] = {};
    std::int32_t nPacketVersion = 0;
    std::int32_t nIP = 0;
    std::int32_t nSerialNo = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_LOGIN_REQ_FOR_TWN& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szAuthCode, 1025, &outLen);
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse.GetString(value.szMacAddress, 18, &outLen);
    packet.XParse >> value.nPacketVersion;
    packet.XParse >> value.nIP;
    packet.XParse >> value.nSerialNo;
    return packet;
}

// 对齐 IDA 0x14007AF00: CHN平台登录请求
struct PS_LOGIN_REQ_FOR_CHN {
    char szAuthCode[1025] = {};
    char szMacAddress[18] = {};
    char szUserChannel[256] = {};
    char szUserFrom[256] = {};
    char szQ_Mid[256] = {};
    char szQ_M2[256] = {};
    char szQ_MAC[256] = {};
    char UserSubsite[256] = {};
    char szAccountID[21] = {};
    std::int32_t nPacketVersion = 0;
    std::int32_t nIP = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_LOGIN_REQ_FOR_CHN& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szAuthCode, 1025, &outLen);
    packet.XParse.GetString(value.szMacAddress, 18, &outLen);
    packet.XParse.GetString(value.szUserChannel, 256, &outLen);
    packet.XParse.GetString(value.szUserFrom, 256, &outLen);
    packet.XParse.GetString(value.szQ_Mid, 256, &outLen);
    packet.XParse.GetString(value.szQ_M2, 256, &outLen);
    packet.XParse.GetString(value.szQ_MAC, 256, &outLen);
    packet.XParse.GetString(value.UserSubsite, 256, &outLen);
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse >> value.nPacketVersion;
    packet.XParse >> value.nIP;
    return packet;
}

// 对齐 IDA 0x140079CC0: GF平台登录请求
struct PS_LOGIN_REQ_FOR_GF {
    wchar_t szAuthCode[1025] = {};
    char szMacAddress[18] = {};
    std::int32_t nPacketVersion = 0;
    std::int32_t nIP = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_LOGIN_REQ_FOR_GF& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.szAuthCode, 1025, outLen);
    packet.XParse.GetString(value.szMacAddress, 18, &outLen);
    packet.XParse >> value.nPacketVersion;
    packet.XParse >> value.nIP;
    return packet;
}

// 注意：ST_GF_AUTH_INFO 已在 PSCommon.h 中定义，使用该定义

// 对齐 IDA: WM平台登录请求
struct PS_LOGIN_REQ_FOR_WM {
    char szHID[21] = {};
    char szToken[256] = {};
    char szMacAddress[18] = {};
    std::int32_t nPacketVersion = 0;
    std::int32_t nIP = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_LOGIN_REQ_FOR_WM& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szHID, 21, &outLen);
    packet.XParse.GetString(value.szToken, 256, &outLen);
    packet.XParse.GetString(value.szMacAddress, 18, &outLen);
    packet.XParse >> value.nPacketVersion;
    packet.XParse >> value.nIP;
    return packet;
}

// 对齐 IDA 0x1400793E0: SG平台登录请求
struct PS_LOGIN_REQ_FOR_SG {
    char szAccountID[21] = {};
    char szMacAddress[18] = {};
    std::int32_t nIP = 0;
    std::uint8_t byAuthType = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_LOGIN_REQ_FOR_SG& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse.GetString(value.szMacAddress, 18, &outLen);
    packet.XParse >> value.nIP;
    packet.XParse >> value.byAuthType;
    return packet;
}

// ============================================================================
// 账户相关结构体
// ============================================================================

// 对齐 IDA: 账户事件列表
struct PS_ACCOUNT_EVENT_LIST {
    char szAccountID[21] = {};
    std::uint32_t dwUCID = 0;
    std::vector<std::uint32_t> vecEventID;
};

inline XPacket& operator>>(XPacket& packet, PS_ACCOUNT_EVENT_LIST& value) {
    short outLen = 0;
    packet.XParse.GetString(value.szAccountID, 21, &outLen);
    packet.XParse >> value.dwUCID;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_ACCOUNT_EVENT_LIST& value) {
    packet.XParse << std::string(value.szAccountID);
    packet.XParse << value.dwUCID;
    packet.XParse << static_cast<std::uint32_t>(value.vecEventID.size());
    for (const auto& eventId : value.vecEventID) {
        packet.XParse << eventId;
    }
    return packet;
}

// 对齐 IDA: 账户登录日期响应
struct PS_DB_ACCOUNT_DATE_RES {
    std::int64_t dwComebackTime = 0;
    std::int64_t tCreateDate = 0;  // CTime 作为 time64_t
};

inline XPacket& operator<<(XPacket& packet, const PS_DB_ACCOUNT_DATE_RES& value) {
    packet.XParse << value.dwComebackTime;
    packet.XParse << value.tCreateDate;
    return packet;
}

// 对齐 IDA: 账户现金里程信息
struct PS_DB_CASH_MILEAGE_INFO {
    std::uint8_t byType = 0;
    std::int32_t nCashMileage[3] = {0, 0, 0};
    std::int32_t nErrorCode = 0;
};

inline XPacket& operator<<(XPacket& packet, const PS_DB_CASH_MILEAGE_INFO& value) {
    packet.XParse << value.byType;
    packet.XParse << value.nCashMileage[0];
    packet.XParse << value.nCashMileage[1];
    packet.XParse << value.nCashMileage[2];
    packet.XParse << value.nErrorCode;
    return packet;
}

// 对齐 IDA: 现金里程更新信息
struct PS_CASH_MILEAGE_UPDATE {
    std::int32_t nShopIndex = 0;
    std::int32_t nItemID = 0;
    std::int32_t nCount = 0;
    std::int64_t biSerial = 0;
    std::int32_t nMileageType1 = 0;
    std::int32_t nMileageType2 = 0;
    std::int32_t nMileageType3 = 0;
};

// 对齐 IDA: 现金里程更新请求
struct PS_CASH_MILEAGE_UPDATE_REQ {
    std::uint8_t byMileageType = 0;
    std::int32_t nCashMileage = 0;
};

// 对齐 IDA: 现金里程列表（用于更新/使用）
struct PS_DB_CASH_MILEAGE_LIST {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwSendUAID = 0;
    std::vector<PS_CASH_MILEAGE_UPDATE> vecInfo;
    PS_CASH_MILEAGE_UPDATE_REQ psUpdateMileage{};
    std::int32_t nResultMileage[3] = {0, 0, 0};
    std::int32_t nErrorCode = 0;
};

// ============================================================================
// HAN 计费相关结构体
// ============================================================================

// 对齐 IDA: HAN计费订单信息
struct PS_HAN_BILLING_ORDER_NO {
    std::uint32_t dwItemID = 0;
    std::int32_t nCount = 0;
    std::int32_t nBanance = 0;
    std::int64_t nOrderNo = 0;
    std::int32_t nResult = 0;
    char szCode[21] = {};
    std::int32_t nShopIndex = 0;
};

inline XPacket& operator>>(XPacket& packet, PS_HAN_BILLING_ORDER_NO& value) {
    packet.XParse >> value.dwItemID;
    packet.XParse >> value.nCount;
    packet.XParse >> value.nBanance;
    packet.XParse >> value.nOrderNo;
    packet.XParse >> value.nResult;
    short outLen = 0;
    packet.XParse.GetString(value.szCode, 21, &outLen);
    packet.XParse >> value.nShopIndex;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_HAN_BILLING_ORDER_NO& value) {
    packet.XParse << value.dwItemID;
    packet.XParse << value.nCount;
    packet.XParse << value.nBanance;
    packet.XParse << value.nOrderNo;
    packet.XParse << value.nResult;
    packet.XParse << std::string(value.szCode);
    packet.XParse << value.nShopIndex;
    return packet;
}

// 对齐 IDA: HAN计费订单列表
struct PS_HAN_BILLING_ORDER_NO_VEC {
    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;
    char szBillCode[21] = {};
    std::uint32_t dwRecvUCID = 0;
    std::uint32_t dwRecvUAID = 0;
    bool bGift = false;
    char szRecvAccountID[21] = {};
    char szRecvCharacterName[21] = {};
    std::uint8_t byType = 0;
    bool bResult = false;
    std::vector<PS_HAN_BILLING_ORDER_NO> vecOrderInfo;
};

inline XPacket& operator>>(XPacket& packet, PS_HAN_BILLING_ORDER_NO_VEC& value) {
    packet.XParse >> value.dwUAID;
    packet.XParse >> value.dwUCID;
    short outLen = 0;
    packet.XParse.GetString(value.szBillCode, 21, &outLen);
    packet.XParse >> value.dwRecvUCID;
    packet.XParse >> value.dwRecvUAID;
    packet.XParse >> value.bGift;
    packet.XParse.GetString(value.szRecvAccountID, 21, &outLen);
    packet.XParse.GetString(value.szRecvCharacterName, 21, &outLen);
    packet.XParse >> value.byType;
    packet.XParse >> value.bResult;
    // vecOrderInfo 需要单独解析
    return packet;
}
