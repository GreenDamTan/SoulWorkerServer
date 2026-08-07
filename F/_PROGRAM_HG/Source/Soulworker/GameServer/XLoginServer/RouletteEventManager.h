// RouletteEventManager.h
// CRouletteEventMgr - 轮盘活动管理器
// 对齐 ControlServer.exe IDA

#pragma once

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <windows.h>
#endif

#include <cstdint>
#include <cwchar>
#include <map>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"

// 前向声明
class XControlServer;
struct PS_DB_ROULETTE_REWARD_INFO;

// PS_GM_ROULETTE_EVENT_ITEM - 轮盘奖励项
// 对齐 IDA ControlServer/GameServer PDB (UDT 0x4c4b / 0x2f4bf, Size 24)
struct ST_GM_ROULETTE_EVENT_ITEM {
    int nRewradIndex = 0;     // 奖励索引 (注意: IDA 拼写错误 "Rewrad")
    int nItemID = 0;          // 物品编号
    int nCount = 0;           // 数量
    int nRate = 0;            // 概率
    int nLimitCount = 0;      // 上限计数
    int nCurCount = 0;        // 当前计数
};

static_assert(sizeof(ST_GM_ROULETTE_EVENT_ITEM) == 24, "ST_GM_ROULETTE_EVENT_ITEM size must match PDB");

// PS_GM_ROULETTE_EVENT_ITEM_LIST - 奖励项列表容器
// 对齐 IDA: PS_GM_ROULETETE_EVENT_ITEM_LIST
struct PS_GM_ROULETETE_EVENT_ITEM_LIST {
    std::vector<ST_GM_ROULETTE_EVENT_ITEM> vecInfo;

    PS_GM_ROULETETE_EVENT_ITEM_LIST() = default;
    ~PS_GM_ROULETETE_EVENT_ITEM_LIST() = default;

    PS_GM_ROULETETE_EVENT_ITEM_LIST& operator=(const PS_GM_ROULETETE_EVENT_ITEM_LIST& other) {
        vecInfo = other.vecInfo;
        return *this;
    }
};

// PS_GM_ROULETTE_EVENT - 轮盘活动信息
// 对齐 IDA ControlServer/GameServer PDB (UDT 0x4c53 / 0xea31, Size 200)
// 布局: nEventID+0, szTitle[60]+4, biStartDate+128, biEndDate+136, byUseType+144,
// nUseCount+148, byCostType+152, nCostID+156, nCostCount+160, nVer+164, psRewardList+168
struct PS_GM_ROULETTE_EVENT {
    int nEventID = 0;                      // 活动ID
    wchar_t szTitle[60] = {};             // 标题 (60 宽字符)
    std::int64_t biStartDate = 0;         // 开始日期
    std::int64_t biEndDate = 0;           // 结束日期
    std::uint8_t byUseType = 0;           // 使用类型
    int nUseCount = 0;                    // 使用次数
    std::uint8_t byCostType = 0;          // 消费类型
    int nCostID = 0;                      // 消费ID
    int nCostCount = 0;                   // 消费数量
    int nVer = 0;                         // 版本号
    PS_GM_ROULETETE_EVENT_ITEM_LIST psRewardList;  // 奖励列表

    PS_GM_ROULETTE_EVENT() = default;
    ~PS_GM_ROULETTE_EVENT() = default;

    PS_GM_ROULETTE_EVENT& operator=(const PS_GM_ROULETTE_EVENT& other) {
        nEventID = other.nEventID;
        std::copy(std::begin(other.szTitle), std::end(other.szTitle), std::begin(szTitle));
        biStartDate = other.biStartDate;
        biEndDate = other.biEndDate;
        byUseType = other.byUseType;
        nUseCount = other.nUseCount;
        byCostType = other.byCostType;
        nCostID = other.nCostID;
        nCostCount = other.nCostCount;
        nVer = other.nVer;
        psRewardList = other.psRewardList;
        return *this;
    }

    // 对齐 IDA 0x14003BDE0: Clear
    void Clear() {
        nEventID = 0;
        std::fill(std::begin(szTitle), std::end(szTitle), L'\0');
        biStartDate = 0;
        biEndDate = 0;
        byUseType = 0;
        nUseCount = 0;
        byCostType = 0;
        nCostID = 0;
        nCostCount = 0;
        nVer = 0;
        psRewardList.vecInfo.clear();
    }
};

static_assert(sizeof(PS_GM_ROULETTE_EVENT) == 200, "PS_GM_ROULETTE_EVENT size must match PDB");

// PS_ROULETTE_EVENT_UPDATE_SERVER - 轮盘更新包
// 对齐 IDA (已在 PSServer.h 中定义)

// PS_DB_ROULETTE_REWARD_INFO 和 PS_ROULETTE_EVENT_UPDATE_SERVER 已在 PSServerDB.h 中定义

// PS_GM_ROULETTE_EVENT 序列化操作符
// 对齐 IDA SendRouletteEvent (0xF2, 0x74)
// 布局: nEventID, 标题(WORD 字节长度 + wchar 字节), biStartDate, biEndDate, byUseType,
// nUseCount, byCostType, nCostID, nCostCount, nVer, 奖励列表(int 计数 + 6 字段 item)
inline XPacket& operator<<(XPacket& packet, const PS_GM_ROULETTE_EVENT& value) {
    packet.XParse << value.nEventID;
    // 标题 - 写入 WORD 字节长度和宽字符字节 (XParse::GetWString 对称)
    const std::uint16_t titleByteLen =
        static_cast<std::uint16_t>(std::wcslen(value.szTitle) * 2);
    packet.XParse << titleByteLen;
    for (int i = 0; i < static_cast<int>(titleByteLen / 2); ++i) {
        const std::uint16_t ch = static_cast<std::uint16_t>(value.szTitle[i]);
        packet.XParse << ch;
    }
    packet.XParse << value.biStartDate;
    packet.XParse << value.biEndDate;
    packet.XParse << value.byUseType;
    packet.XParse << value.nUseCount;
    packet.XParse << value.byCostType;
    packet.XParse << value.nCostID;
    packet.XParse << value.nCostCount;
    packet.XParse << value.nVer;
    // 奖励列表
    packet.XParse << static_cast<int>(value.psRewardList.vecInfo.size());
    for (const auto& item : value.psRewardList.vecInfo) {
        packet.XParse << item.nRewradIndex;
        packet.XParse << item.nItemID;
        packet.XParse << item.nCount;
        packet.XParse << item.nRate;
        packet.XParse << item.nLimitCount;
        packet.XParse << item.nCurCount;
    }
    return packet;
}

// PS_GM_ROULETTE_EVENT 反序列化操作符
// 对齐 IDA @ 0x140750A20
inline void operator>>(XPacket& packet, PS_GM_ROULETTE_EVENT& value) {
    packet.XParse >> value.nEventID;
    // 标题 - XParse::GetWString(60) 读取 WORD 字节长度 + 宽字符字节
    short outLen = 0;
    packet.XParse.GetWString(value.szTitle, 60, outLen);
    packet.XParse >> value.biStartDate;
    packet.XParse >> value.biEndDate;
    packet.XParse >> value.byUseType;
    packet.XParse >> value.nUseCount;
    packet.XParse >> value.byCostType;
    packet.XParse >> value.nCostID;
    packet.XParse >> value.nCostCount;
    packet.XParse >> value.nVer;
    // 奖励列表
    int nCount = 0;
    packet.XParse >> nCount;
    value.psRewardList.vecInfo.clear();
    value.psRewardList.vecInfo.reserve(nCount);
    for (int i = 0; i < nCount; ++i) {
        ST_GM_ROULETTE_EVENT_ITEM item{};
        packet.XParse >> item.nRewradIndex;
        packet.XParse >> item.nItemID;
        packet.XParse >> item.nCount;
        packet.XParse >> item.nRate;
        packet.XParse >> item.nLimitCount;
        packet.XParse >> item.nCurCount;
        value.psRewardList.vecInfo.push_back(item);
    }
}

// ============================================================================
// 客户端轮盘事件包类型 (GameServer -> Client)
// PS_ROULETTE_EVENT: PDB UDT 0x1ced9, Size 200
// ST_ROULETTE_EVENT_ITEM: PDB UDT 0x1cedb, Size 12
// ============================================================================

// ST_ROULETTE_EVENT_ITEM - 客户端轮盘奖励项
struct ST_ROULETTE_EVENT_ITEM {
    int nRewardID = 0;
    int nItemID = 0;
    int nCount = 0;
};

static_assert(sizeof(ST_ROULETTE_EVENT_ITEM) == 12, "ST_ROULETTE_EVENT_ITEM size must match PDB");

// PS_ROULETTE_EVENT_ITEM_LIST - 客户端奖励项列表
struct PS_ROULETTE_EVENT_ITEM_LIST {
    std::vector<ST_ROULETTE_EVENT_ITEM> vecInfo;
};

static_assert(sizeof(PS_ROULETTE_EVENT_ITEM_LIST) == 32, "PS_ROULETTE_EVENT_ITEM_LIST size must match PDB");

// PS_ROULETTE_EVENT - 客户端轮盘活动信息
// 布局: szTitle[60]+0, nEventID+120, byUseType+124, nDayLimit+128, biStartDate+136,
// biEndDate+144, byCostType+152, nCostID+156, nCostCount+160, psRewardItemList+168
struct PS_ROULETTE_EVENT {
    wchar_t szTitle[60] = {};
    int nEventID = 0;
    std::uint8_t byUseType = 0;
    int nDayLimit = 0;
    std::int64_t biStartDate = 0;
    std::int64_t biEndDate = 0;
    std::uint8_t byCostType = 0;
    int nCostID = 0;
    int nCostCount = 0;
    PS_ROULETTE_EVENT_ITEM_LIST psRewardItemList;
};

static_assert(sizeof(PS_ROULETTE_EVENT) == 200, "PS_ROULETTE_EVENT size must match PDB");

// PS_ROULETTE_EVENT 序列化 (对齐 IDA 0x140751060, 客户端包 0x2A/0x27)
inline XPacket& operator<<(XPacket& packet, PS_ROULETTE_EVENT& value) {
    // 标题 - WORD 字节长度 + 宽字符字节
    const std::uint16_t titleByteLen =
        static_cast<std::uint16_t>(std::wcslen(value.szTitle) * 2);
    packet.XParse << titleByteLen;
    for (int i = 0; i < static_cast<int>(titleByteLen / 2); ++i) {
        packet.XParse << static_cast<std::uint16_t>(value.szTitle[i]);
    }
    packet.XParse << value.nEventID;
    packet.XParse << value.byUseType;
    packet.XParse << value.nDayLimit;
    packet.XParse << value.biStartDate;
    packet.XParse << value.biEndDate;
    packet.XParse << value.byCostType;
    packet.XParse << value.nCostID;
    packet.XParse << value.nCostCount;
    // 奖励列表
    packet.XParse << static_cast<int>(value.psRewardItemList.vecInfo.size());
    for (const auto& item : value.psRewardItemList.vecInfo) {
        packet.XParse << item.nRewardID;
        packet.XParse << item.nItemID;
        packet.XParse << item.nCount;
    }
    return packet;
}

// 对齐 IDA CRouletteEventMgr
// 反编译来源:
// - Constructor @ 0x14003A4C0
// - Destructor @ 0x14003A530
// - Clear @ 0x14003A560
// - Update @ 0x14003AC10
// - LoadRouletteEvent @ 0x14003A5A0
// - SendRouletteEvent @ 0x14003AD30
// - SendDBRouletteRewardInfo @ 0x14003ACA0
// - UpdateRouletteItemInfo @ 0x14003ADE0
// - SetRouletteRewardInfo @ 0x14003AEB0
class CRouletteEventMgr {
public:
    // 对齐 IDA 0x14003A4C0: 构造函数
    // IDA: CFSRWLock::CFSRWLock + PS_GM_ROULETTE_EVENT::PS_GM_ROULETTE_EVENT + Clear
    CRouletteEventMgr() {
        // CFSRWLock 默认构造
        // PS_GM_ROULETTE_EVENT 默认构造
        Clear();
    }

    // 对齐 IDA 0x14003A530: 析构函数
    // IDA: Clear + PS_GM_ROULETTE_EVENT::~PS_GM_ROULETTE_EVENT
    ~CRouletteEventMgr() {
        Clear();
    }

    // 对齐 IDA 0x14003A560: Clear
    // IDA: m_dw64Tick = 0; m_bLoad = 0; PS_GM_ROULETTE_EVENT::Clear
    void Clear() {
        m_dw64Tick = 0;
        m_bLoad = false;
        m_psRouletteEventInfo.Clear();
    }

    // 对齐 IDA 0x14003AC10: Update(dw64Tick, nGroupID)
    void Update(unsigned __int64 dw64Tick, int nGroupID);

    // 对齐 IDA 0x14003A5A0: LoadRouletteEvent
    bool LoadRouletteEvent(int nGroupID);

    // 对齐 IDA 0x14003A9E0: CheckItemDayCount
    void CheckItemDayCount(PS_GM_ROULETTE_EVENT& psInfo);

    // 对齐 IDA 0x14003AD30: SendRouletteEvent
    void SendRouletteEvent(bool bSendGameServer);

    // 对齐 IDA 0x14003ACA0: SendDBRouletteRewardInfo
    void SendDBRouletteRewardInfo();

    // 对齐 IDA 0x14003ADE0: UpdateRouletteItemInfo
    void UpdateRouletteItemInfo(PS_ROULETTE_EVENT_UPDATE_SERVER& stUpdateInfo);

    // 对齐 IDA 0x14003AEB0: SetRouletteRewardInfo
    void SetRouletteRewardInfo(PS_DB_ROULETTE_REWARD_INFO& stRewardInfo);

private:
    bool m_bLoad = false;
    unsigned __int64 m_dw64Tick = 0;
    CFSRWLock m_rwEventRouletteLock;        // 读写锁
    PS_GM_ROULETTE_EVENT m_psRouletteEventInfo;  // 轮盘活动信息
};