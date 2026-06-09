#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// ============================================================================
// PSServerChat.h - 聊天系统相关结构体及序列化运算符
//
// 此文件包含聊天系统的所有数据结构和序列化操作：
// - 广播公告
// - 喇叭消息
// - 物品链接
// - 私聊消息
// ============================================================================

// ============================================================================
// 聊天系统结构体
// ============================================================================

/**
 * @brief Relay 广播聊天公告。
 *
 * 该结构按 PDB 与 RelayServer.exe 反编译共同恢复：
 * - `byType @ +0x0`
 * - `strMsg[256] @ +0x2`（UTF-16 字节长度 0x200）
 * - `strColor[7] @ +0x202`（UTF-16 字节长度 0x1C）
 * - `nMessageCode @ +0x210`
 */
struct PS_CHAT_NOTICE {
    std::uint8_t byType = 0;
    std::uint8_t _pad0 = 0;
    wchar_t strMsg[256] = {};
    wchar_t strColor[7] = {};
    std::int32_t nMessageCode = 0;

    // IDA: ??0PS_CHAT_NOTICE@@QEAA@XZ (0x1400386a0)
    PS_CHAT_NOTICE() : byType(0), _pad0(0), nMessageCode(0) {
        strMsg[0] = L'\0';
        wcscpy_s(strColor, L"FF0000");
    }
};

/**
 * @brief Relay 广播喇叭消息。
 */
struct PS_CHAT_MEGAPHONE {
    std::uint8_t byInvenType = 0;
    std::uint8_t _pad0 = 0;
    std::int16_t shSlot = 0;
    std::uint32_t dwUCID = 0;
    wchar_t strName[21] = {};
    wchar_t strMsg[256] = {};
};

/**
 * @brief 聊天物品链接结构。
 */
struct PS_CHAT_ITEM_LINK {
    std::uint8_t byStart = 0;
    std::uint8_t bySize = 0;
    std::uint8_t byType = 0;
    std::uint8_t _pad2[5] = {};
    std::int64_t i64ID = 0;
    wchar_t szLinkString[64] = {};
    STItem kItem{};
    PS_ITEM_SOCKET_LIST psSocketInfo{};
    ST_ITEM_BROACH psBroachInfo{};
    PS_ITEM_PACKAGE psRePackageCostumeInfo{};
};

/**
 * @brief 服务器端聊天物品链接列表。
 */
struct PS_CHAT_ITEM_LINK_FOR_SERVER {
    std::uint8_t byItemLinkCount = 0;
    std::uint8_t _pad3[7] = {};
    PS_CHAT_ITEM_LINK psItemLinkInfo[3] = {};
};

/**
 * @brief 私聊消息结构。
 */
struct PS_CHAT_WHISPER {
    wchar_t strSender[21] = {};
    wchar_t strReciver[21] = {};
    wchar_t strMsg[256] = {};
    std::int32_t nResult = 0;
    std::uint32_t dwSenderUCID = 0;
};

/**
 * @brief Normal chat message request.
 */
struct PS_CHAT_NORMAL {
    wchar_t strMsg[256] = {};
};

/**
 * @brief Trade chat request.
 */
struct PS_REQ_CHAT_TRADE {
    wchar_t strMsg[256] = {};
};

/**
 * @brief Item link request from client.
 */
struct PS_CHAT_ITEM_LINK_REQ {
    std::uint8_t byStart = 0;
    std::uint8_t bySize = 0;
    std::uint8_t byType = 0;
    std::uint8_t _pad[5] = {};
    std::int64_t i64ID = 0;
    wchar_t szLinkString[64] = {};
};

// ============================================================================
// 聊天系统序列化运算符
// ============================================================================

// PS_CHAT_NOTICE 序列化
inline void operator>>(XPacket& packet, PS_CHAT_NOTICE& value) {
    short outLen = 0;
    packet.XParse >> value.byType;
    packet.XParse.GetBYTE();
    packet.XParse.GetWString(value.strMsg, 256, outLen);
    packet.XParse.GetWString(value.strColor, 7, outLen);
    packet.XParse >> value.nMessageCode;
}

inline XPacket& operator<<(XPacket& packet, const PS_CHAT_NOTICE& value) {
    packet.XParse << value.byType;
    packet.XParse << static_cast<std::uint8_t>(0);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    packet.XParse << GreenDamTan_BoundedWideString(value.strColor);
    packet.XParse << value.nMessageCode;
    return packet;
}

// PS_CHAT_MEGAPHONE 序列化
inline void operator>>(XPacket& packet, PS_CHAT_MEGAPHONE& value) {
    short outLen = 0;
    packet.XParse >> value.byInvenType;
    packet.XParse >> value.shSlot;
    packet.XParse >> value.dwUCID;
    packet.XParse.GetWString(value.strName, 21, outLen);
    packet.XParse.GetWString(value.strMsg, 256, outLen);
}

inline XPacket& operator<<(XPacket& packet, const PS_CHAT_MEGAPHONE& value) {
    packet.XParse << value.byInvenType;
    packet.XParse << value.shSlot;
    packet.XParse << value.dwUCID;
    packet.XParse << GreenDamTan_BoundedWideString(value.strName);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    return packet;
}

// PS_CHAT_ITEM_LINK 序列化
inline void operator>>(XPacket& packet, PS_CHAT_ITEM_LINK& value) {
    short outLen = 0;
    packet.XParse >> value.byStart;
    packet.XParse >> value.bySize;
    packet.XParse >> value.byType;
    packet.XParse >> value.i64ID;
    packet.XParse.GetWString(value.szLinkString, 64, outLen);
    packet >> value.kItem;
    packet >> value.psSocketInfo;
    packet >> value.psBroachInfo;
    packet >> value.psRePackageCostumeInfo;
}

inline XPacket& operator<<(XPacket& packet, const PS_CHAT_ITEM_LINK& value) {
    packet.XParse << value.byStart;
    packet.XParse << value.bySize;
    packet.XParse << value.byType;
    packet.XParse << value.i64ID;
    packet.XParse << GreenDamTan_BoundedWideString(value.szLinkString);
    packet << value.kItem;
    packet << value.psSocketInfo;
    packet << value.psBroachInfo;
    packet << value.psRePackageCostumeInfo;
    return packet;
}

// PS_CHAT_ITEM_LINK_FOR_SERVER 序列化
inline void operator>>(XPacket& packet, PS_CHAT_ITEM_LINK_FOR_SERVER& value) {
    packet.XParse >> value.byItemLinkCount;
    for (PS_CHAT_ITEM_LINK& item : value.psItemLinkInfo) {
        packet >> item;
    }
}

inline XPacket& operator<<(XPacket& packet, const PS_CHAT_ITEM_LINK_FOR_SERVER& value) {
    packet.XParse << value.byItemLinkCount;
    for (const PS_CHAT_ITEM_LINK& item : value.psItemLinkInfo) {
        packet << item;
    }
    return packet;
}

// PS_CHAT_WHISPER 序列化
inline void operator>>(XPacket& packet, PS_CHAT_WHISPER& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.strSender, 21, outLen);
    packet.XParse.GetWString(value.strReciver, 21, outLen);
    packet.XParse.GetWString(value.strMsg, 256, outLen);
    packet.XParse >> value.nResult;
    packet.XParse >> value.dwSenderUCID;
}

inline XPacket& operator<<(XPacket& packet, const PS_CHAT_WHISPER& value) {
    packet.XParse << GreenDamTan_BoundedWideString(value.strSender);
    packet.XParse << GreenDamTan_BoundedWideString(value.strReciver);
    packet.XParse << GreenDamTan_BoundedWideString(value.strMsg);
    packet.XParse << value.nResult;
    packet.XParse << value.dwSenderUCID;
    return packet;
}
// ============================================================================
// static_assert 验证（确保结构体大小与 PDB 匹配）
// ============================================================================

static_assert(sizeof(PS_CHAT_NOTICE) == 0x214, "PS_CHAT_NOTICE size must match PDB");
static_assert(offsetof(PS_CHAT_NOTICE, strMsg) == 0x2, "PS_CHAT_NOTICE.strMsg offset mismatch");
static_assert(offsetof(PS_CHAT_NOTICE, strColor) == 0x202, "PS_CHAT_NOTICE.strColor offset mismatch");
static_assert(offsetof(PS_CHAT_NOTICE, nMessageCode) == 0x210,
              "PS_CHAT_NOTICE.nMessageCode offset mismatch");
static_assert(sizeof(PS_CHAT_MEGAPHONE) == 0x234, "PS_CHAT_MEGAPHONE size must match PDB");
static_assert(offsetof(PS_CHAT_MEGAPHONE, shSlot) == 0x2, "PS_CHAT_MEGAPHONE.shSlot offset mismatch");
static_assert(offsetof(PS_CHAT_MEGAPHONE, dwUCID) == 0x4, "PS_CHAT_MEGAPHONE.dwUCID offset mismatch");
static_assert(offsetof(PS_CHAT_MEGAPHONE, strName) == 0x8, "PS_CHAT_MEGAPHONE.strName offset mismatch");
static_assert(offsetof(PS_CHAT_MEGAPHONE, strMsg) == 0x32, "PS_CHAT_MEGAPHONE.strMsg offset mismatch");
static_assert(sizeof(PS_CHAT_ITEM_LINK) == 0x198, "PS_CHAT_ITEM_LINK size must match PDB");
static_assert(offsetof(PS_CHAT_ITEM_LINK, i64ID) == 0x8, "PS_CHAT_ITEM_LINK.i64ID offset mismatch");
static_assert(offsetof(PS_CHAT_ITEM_LINK, szLinkString) == 0x10,
              "PS_CHAT_ITEM_LINK.szLinkString offset mismatch");
static_assert(offsetof(PS_CHAT_ITEM_LINK, kItem) == 0x90, "PS_CHAT_ITEM_LINK.kItem offset mismatch");
static_assert(offsetof(PS_CHAT_ITEM_LINK, psSocketInfo) == 0x108,
              "PS_CHAT_ITEM_LINK.psSocketInfo offset mismatch");
static_assert(offsetof(PS_CHAT_ITEM_LINK, psBroachInfo) == 0x128,
              "PS_CHAT_ITEM_LINK.psBroachInfo offset mismatch");
static_assert(offsetof(PS_CHAT_ITEM_LINK, psRePackageCostumeInfo) == 0x170,
              "PS_CHAT_ITEM_LINK.psRePackageCostumeInfo offset mismatch");
static_assert(sizeof(PS_CHAT_ITEM_LINK_FOR_SERVER) == 0x4D0,
              "PS_CHAT_ITEM_LINK_FOR_SERVER size must match PDB");
static_assert(offsetof(PS_CHAT_ITEM_LINK_FOR_SERVER, psItemLinkInfo) == 0x8,
              "PS_CHAT_ITEM_LINK_FOR_SERVER.psItemLinkInfo offset mismatch");
static_assert(sizeof(PS_CHAT_WHISPER) == 0x25C, "PS_CHAT_WHISPER size must match PDB");
static_assert(offsetof(PS_CHAT_WHISPER, strReciver) == 0x2A,
              "PS_CHAT_WHISPER.strReciver offset mismatch");
static_assert(offsetof(PS_CHAT_WHISPER, strMsg) == 0x54, "PS_CHAT_WHISPER.strMsg offset mismatch");
static_assert(offsetof(PS_CHAT_WHISPER, nResult) == 0x254, "PS_CHAT_WHISPER.nResult offset mismatch");
static_assert(offsetof(PS_CHAT_WHISPER, dwSenderUCID) == 0x258,
              "PS_CHAT_WHISPER.dwSenderUCID offset mismatch");

// ============================================================================
// Additional chat packet structures - serialization operators
// ============================================================================

// PS_CHAT_NORMAL serialization
inline void operator>>(XPacket& packet, PS_CHAT_NORMAL& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.strMsg, 256, outLen);
}

// PS_REQ_CHAT_TRADE serialization
inline void operator>>(XPacket& packet, PS_REQ_CHAT_TRADE& value) {
    short outLen = 0;
    packet.XParse.GetWString(value.strMsg, 256, outLen);
}

// PS_CHAT_ITEM_LINK_REQ serialization
inline void operator>>(XPacket& packet, PS_CHAT_ITEM_LINK_REQ& value) {
    short outLen = 0;
    packet.XParse >> value.byStart;
    packet.XParse >> value.bySize;
    packet.XParse >> value.byType;
    packet.XParse >> value.i64ID;
    packet.XParse.GetWString(value.szLinkString, 64, outLen);
}
