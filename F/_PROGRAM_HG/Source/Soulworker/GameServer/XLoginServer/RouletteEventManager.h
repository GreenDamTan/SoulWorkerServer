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
#include <map>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"

// 前向声明
class XControlServer;
struct PS_DB_ROULETTE_REWARD_INFO;

// PS_GM_ROULETTE_EVENT_ITEM - 轮盘奖励项
// 对齐 IDA ControlServer.exe
struct ST_GM_ROULETTE_EVENT_ITEM {
    int nRewradIndex = 0;     // 奖励索引 (注意: IDA 拼写错误 "Rewrad")
    int nCurCount = 0;        // 当前计数
    int nMaxCount = 0;        // 最大计数
    int nItemNo = 0;          // 物品编号
};

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
// 对齐 IDA ControlServer.exe 0x14003A720
// 字段顺序从 IDA 复制构造函数提取:
// nEventID, szTitle, biStartDate, biEndDate, byUseType, nUseCount,
// byCostType, nCostID, nCostCount, nVer, psRewardList
struct PS_GM_ROULETTE_EVENT {
    int nEventID = 0;                      // 活动ID
    wchar_t szTitle[256] = {};            // 标题
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

// PS_ROULETTE_EVENT_UPDATE_SERVER - 轮盘更新包
// 对齐 IDA (已在 PSServer.h 中定义)

// PS_DB_ROULETTE_REWARD_INFO - DB返回的轮盘奖励信息
// 对齐 IDA ControlServer.exe 0x14003AEB0
struct PS_ROULETTE_EVENT_UPDATE_INFO {
    int nEventID = 0;
    int nRewardID = 0;
    int nRemainCount = 0;
};

struct PS_DB_ROULETTE_REWARD_INFO {
    std::vector<PS_ROULETTE_EVENT_UPDATE_INFO> vecInfo;
};

// PS_GM_ROULETTE_EVENT 序列化操作符
// 对齐 IDA SendRouletteEvent (0xF2, 0x74)
inline XPacket& operator<<(XPacket& packet, const PS_GM_ROULETTE_EVENT& value) {
    packet.XParse << value.nEventID;
    // 标题 (固定长度)
    for (int i = 0; i < 256; ++i) {
        packet.XParse << value.szTitle[i];
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
        packet.XParse << item.nCurCount;
        packet.XParse << item.nMaxCount;
        packet.XParse << item.nItemNo;
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