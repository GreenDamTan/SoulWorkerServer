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
// 对齐 IDA
struct ST_GM_ROULETTE_EVENT_ITEM {
    int nRewradIndex = 0;     // 奖励索引
    int nCurCount = 0;        // 当前计数
    int nMaxCount = 0;        // 最大计数
    int nItemNo = 0;          // 物品编号
};

// PS_GM_ROULETTE_EVENT - 轮盘活动信息
// 对齐 IDA
struct PS_GM_ROULETTE_EVENT {
    int nVer = 0;             // 版本号
    std::vector<ST_GM_ROULETTE_EVENT_ITEM> psRewardList;

    PS_GM_ROULETTE_EVENT() = default;
    ~PS_GM_ROULETTE_EVENT() = default;

    PS_GM_ROULETTE_EVENT& operator=(const PS_GM_ROULETTE_EVENT& other) {
        nVer = other.nVer;
        psRewardList = other.psRewardList;
        return *this;
    }
};

// PS_ROULETTE_EVENT_UPDATE_SERVER - 轮盘更新包
// 对齐 IDA (已在 PSServer.h 中定义)

// 对齐 IDA CRouletteEventMgr
// 反编译来源:
// - Update @ 0x14003AC10
// - LoadRouletteEvent @ 0x14003A5A0
// - SendRouletteEvent @ 0x14003AD30
// - SendDBRouletteRewardInfo @ 0x14003ACA0
// - UpdateRouletteItemInfo @ 0x14003ADE0
class CRouletteEventMgr {
public:
    CRouletteEventMgr() = default;
    ~CRouletteEventMgr() = default;

    // 对齐 IDA 0x14003A560: Clear
    void Clear() {
        CFAutoSlimWriteLock lock(&m_rwEventRouletteLock);
        m_bLoad = false;
        m_dw64Tick = 0;
        m_psRouletteEventInfo.psRewardList.clear();
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