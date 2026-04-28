// DayEventManager.h
// CDayEventMgr - 日常活动管理器
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
struct ST_DAY_EVENT_INFO;

// ST_DAY_EVENT_INFO - 日常活动信息
// 对齐 IDA size=4
struct ST_DAY_EVENT_INFO {
    unsigned short wMazeID = 0;      // 迷宫ID
    unsigned short wBoosterID = 0;   // 增益ID
};

// PS_DAY_EVENT_LIST - 日常活动列表包
// 对齐 IDA
struct PS_DAY_EVENT_LIST {
    std::map<unsigned short, ST_DAY_EVENT_INFO> mapList;

    PS_DAY_EVENT_LIST() = default;
    ~PS_DAY_EVENT_LIST() = default;
};

// XPacket 序列化
inline XPacket& operator<<(XPacket& packet, const ST_DAY_EVENT_INFO& info) {
    packet.XParse << info.wMazeID;
    packet.XParse << info.wBoosterID;
    return packet;
}

inline XPacket& operator<<(XPacket& packet, const PS_DAY_EVENT_LIST& list) {
    packet.XParse << static_cast<unsigned int>(list.mapList.size());
    for (const auto& pair : list.mapList) {
        packet.XParse << pair.first;
        packet << pair.second;
    }
    return packet;
}

// 对齐 IDA CDayEventMgr
// 反编译来源:
// - Update @ 0x14002D9A0
// - SetDayEvent @ 0x14002DAF0
// - SendDayEventAllGameServer @ 0x14002E4D0
// - GetDayEvent @ 0x14002DA90
class CDayEventMgr {
public:
    CDayEventMgr() = default;
    ~CDayEventMgr() = default;

    // 对齐 IDA 0x14002D560: Clear
    void Clear() {
        CFAutoSlimWriteLock lock(&m_rwEventLock);
        m_mapDayEvent.clear();
        m_bLoad = false;
        m_dw64Tick = 0;
    }

    // 对齐 IDA 0x14002D9A0: Update(dw64Tick)
    void Update(unsigned __int64 dw64Tick);

    // 对齐 IDA 0x14002DAF0: SetDayEvent
    void SetDayEvent();

    // 对齐 IDA 0x14002E4D0: SendDayEventAllGameServer
    void SendDayEventAllGameServer();

    // 对齐 IDA 0x14002DA90: GetDayEvent
    void GetDayEvent(PS_DAY_EVENT_LIST& psList);

private:
    bool m_bLoad = false;
    unsigned __int64 m_dw64Tick = 0;
    ATL::CTime m_tLastSetTime;  // 最后设置时间
    CFSRWLock m_rwEventLock;    // 读写锁
    std::map<unsigned short, ST_DAY_EVENT_INFO> m_mapDayEvent;  // 日常活动映射
};
