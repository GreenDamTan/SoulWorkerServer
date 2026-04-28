// RouletteEventManager.cpp
// CRouletteEventMgr 轮盘活动管理器实现
// 对齐 ControlServer.exe IDA

#include "RouletteEventManager.h"
#include "Soulworker/GameServer/XControlServer/ControlServer.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"

#ifdef _WIN32
#include <intrin.h>
#endif

// ============================================================================
// Update - 对齐 IDA 0x14003AC10
// ============================================================================
void CRouletteEventMgr::Update(unsigned __int64 dw64Tick, int nGroupID)
{
    // 对齐 IDA: CRouletteEventMgr::Update
    // 检查是否到达更新时间
    if (m_dw64Tick <= dw64Tick) {
        if (m_dw64Tick) {
            // 已初始化，检查版本变化
            if (LoadRouletteEvent(nGroupID)) {
                SendRouletteEvent(true);
            }
        } else {
            // 首次初始化
            LoadRouletteEvent(nGroupID);
            SendDBRouletteRewardInfo();
        }
        // 设置下次更新时间 (60秒后)
        m_dw64Tick = dw64Tick + 60000;
    }
}

// ============================================================================
// LoadRouletteEvent - 对齐 IDA 0x14003A5A0
// ============================================================================
bool CRouletteEventMgr::LoadRouletteEvent(int nGroupID)
{
    // 对齐 IDA: CRouletteEventMgr::LoadRouletteEvent
    XControlServer* pServer = XControlServer::Instance();
    if (!pServer) return false;

    // 加载轮盘活动数据
    // XResourceMgr::LoadRouletteEvent(nGroupID);

    PS_GM_ROULETTE_EVENT psRouletteEventInfo;
    // XResourceMgr::GetRouletteEvent(psRouletteEventInfo);

    if (m_bLoad) {
        // 已加载，检查版本
        if (m_psRouletteEventInfo.nVer == psRouletteEventInfo.nVer) {
            return false;  // 版本相同，无需更新
        } else {
            // 版本变化，检查日期计数
            CheckItemDayCount(psRouletteEventInfo);
            return true;
        }
    } else {
        // 首次加载
        m_psRouletteEventInfo = psRouletteEventInfo;
        m_bLoad = true;
        return true;
    }
}

// ============================================================================
// CheckItemDayCount - 对齐 IDA 0x14003A9E0
// ============================================================================
void CRouletteEventMgr::CheckItemDayCount(PS_GM_ROULETTE_EVENT& psInfo)
{
    // 对齐 IDA: CRouletteEventMgr::CheckItemDayCount
    // 检查并更新每日计数
    m_psRouletteEventInfo.nVer = psInfo.nVer;
    // Stub: 实现需要完整的计数检查逻辑
}

// ============================================================================
// SendRouletteEvent - 对齐 IDA 0x14003AD30
// ============================================================================
void CRouletteEventMgr::SendRouletteEvent(bool bSendGameServer)
{
    // 对齐 IDA: CRouletteEventMgr::SendRouletteEvent
    XControlServer* pServer = XControlServer::Instance();
    if (!pServer) return;

    CFAutoSlimReadLock lock(&m_rwEventRouletteLock);

    XSendPacket xSendPacket(0xF2, 0x71);
    xSendPacket.XParse << m_psRouletteEventInfo.nVer;
    xSendPacket.XParse << static_cast<unsigned int>(m_psRouletteEventInfo.psRewardList.size());
    for (const auto& item : m_psRouletteEventInfo.psRewardList) {
        xSendPacket.XParse << item.nRewradIndex;
        xSendPacket.XParse << item.nCurCount;
        xSendPacket.XParse << item.nMaxCount;
        xSendPacket.XParse << item.nItemNo;
    }

    if (bSendGameServer) {
        pServer->SendPacketAll(xSendPacket, 0);
    }
}

// ============================================================================
// SendDBRouletteRewardInfo - 对齐 IDA 0x14003ACA0
// ============================================================================
void CRouletteEventMgr::SendDBRouletteRewardInfo()
{
    // 对齐 IDA: CRouletteEventMgr::SendDBRouletteRewardInfo
    XControlServer* pServer = XControlServer::Instance();
    if (!pServer) return;

    // 发送轮盘奖励信息请求到 DB
    XSendDBPacket dbPacket(0, 0x42, 0x50);
    pServer->SendDBLog(dbPacket);
}

// ============================================================================
// UpdateRouletteItemInfo - 对齐 IDA 0x14003ADE0
// ============================================================================
void CRouletteEventMgr::UpdateRouletteItemInfo(PS_ROULETTE_EVENT_UPDATE_SERVER& stUpdateInfo)
{
    // 对齐 IDA: CRouletteEventMgr::UpdateRouletteItemInfo
    CFAutoSlimWriteLock lock(&m_rwEventRouletteLock);

    // 查找并更新奖励计数
    for (auto& item : m_psRouletteEventInfo.psRewardList) {
        if (item.nRewradIndex == stUpdateInfo.nIndex) {
            item.nCurCount = stUpdateInfo.nValue;
            break;
        }
    }

    SendRouletteEvent(false);
}

// ============================================================================
// SetRouletteRewardInfo - 对齐 IDA 0x14003AEB0
// ============================================================================
void CRouletteEventMgr::SetRouletteRewardInfo(PS_DB_ROULETTE_REWARD_INFO& stRewardInfo)
{
    // 对齐 IDA: CRouletteEventMgr::SetRouletteRewardInfo
    CFAutoSlimWriteLock lock(&m_rwEventRouletteLock);

    // 更新奖励信息
    // Stub: 实现需要完整的数据结构支持
}
