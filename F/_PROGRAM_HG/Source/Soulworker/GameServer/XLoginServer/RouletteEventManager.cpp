// RouletteEventManager.cpp
// CRouletteEventMgr 轮盘活动管理器实现
// 对齐 ControlServer.exe IDA

#include "RouletteEventManager.h"
#include "Soulworker/GameServer/XControlServer/ControlServer.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

#ifdef _WIN32
#include <intrin.h>
#endif

// ============================================================================
// Update - 对齐 IDA 0x14003AC10
// IDA: 检查 m_dw64Tick <= dw64Tick, 根据是否首次加载执行不同路径
// ============================================================================
void CRouletteEventMgr::Update(unsigned __int64 dw64Tick, int nGroupID)
{
    // 对齐 IDA: if (m_dw64Tick <= dw64Tick)
    if (m_dw64Tick <= dw64Tick) {
        if (m_dw64Tick) {
            // 对齐 IDA: 非首次更新，检查版本变化
            if (LoadRouletteEvent(nGroupID)) {
                SendRouletteEvent(true);
            }
        } else {
            // 对齐 IDA: 首次加载
            LoadRouletteEvent(nGroupID);
            SendDBRouletteRewardInfo();
        }
        // 对齐 IDA: 设置下次更新时间 (60秒后)
        m_dw64Tick = dw64Tick + 60000;
    }
}

// ============================================================================
// LoadRouletteEvent - 对齐 IDA 0x14003A5A0
// IDA: 加载轮盘活动数据，检查版本号
// ============================================================================
bool CRouletteEventMgr::LoadRouletteEvent(int nGroupID)
{
    // 对齐 IDA: XControlServer::Instance()
    XControlServer* pServer = XControlServer::Instance();
    if (!pServer) return false;

    // TODO: XResourceMgr::LoadRouletteEvent(nGroupID)
    // TODO: XResourceMgr::GetRouletteEvent(psRouletteEventInfo)

    PS_GM_ROULETTE_EVENT psRouletteEventInfo;
    // 对齐 IDA: 从 XResourceMgr 获取轮盘活动数据
    // XResourceMgr::GetRouletteEvent(&pServer->m_xResourceMgr, &psRouletteEventInfo);

    if (m_bLoad) {
        // 对齐 IDA: 已加载，检查版本
        if (m_psRouletteEventInfo.nVer == psRouletteEventInfo.nVer) {
            // 对齐 IDA: 版本相同，无需更新
            return false;
        } else {
            // 对齐 IDA: 版本变化，检查日期计数
            CheckItemDayCount(psRouletteEventInfo);
            return true;
        }
    } else {
        // 对齐 IDA: 首次加载，直接赋值
        m_psRouletteEventInfo = psRouletteEventInfo;
        m_bLoad = true;
        return true;
    }
}

// ============================================================================
// CheckItemDayCount - 对齐 IDA 0x14003A9E0
// IDA: 使用写锁，更新奖励项的每日计数
// ============================================================================
void CRouletteEventMgr::CheckItemDayCount(PS_GM_ROULETTE_EVENT& psUpdateInfo)
{
    // 对齐 IDA: CFAutoSlimWriteLock::CFAutoSlimWriteLock
    CFAutoSlimWriteLock lock(&m_rwEventRouletteLock);

    LogHelper::LogDebug("game.contents",
        "CheckItemDayCount - Update Roulette Version[Prev:%d, New:%d]",
        m_psRouletteEventInfo.nVer,
        psUpdateInfo.nVer);

    // 对齐 IDA: if (psUpdateInfo->nEventID)
    if (psUpdateInfo.nEventID) {
        // 对齐 IDA: 保留原有的 nCurCount，更新其他字段
        for (size_t j = 0; j < m_psRouletteEventInfo.psRewardList.vecInfo.size(); ++j) {
            if (j < psUpdateInfo.psRewardList.vecInfo.size()) {
                psUpdateInfo.psRewardList.vecInfo[j].nCurCount =
                    m_psRouletteEventInfo.psRewardList.vecInfo[j].nCurCount;
            }
        }
        // 对齐 IDA: PS_GM_ROULETTE_EVENT::operator=
        m_psRouletteEventInfo = psUpdateInfo;
    } else {
        // 对齐 IDA: 事件ID为0，清空数据但保留 nCurCount
        PS_GM_ROULETETE_EVENT_ITEM_LIST psRewardItemList;
        for (size_t i = 0; i < m_psRouletteEventInfo.psRewardList.vecInfo.size(); ++i) {
            ST_GM_ROULETTE_EVENT_ITEM stRewardItem;
            stRewardItem.nCurCount = m_psRouletteEventInfo.psRewardList.vecInfo[i].nCurCount;
            psRewardItemList.vecInfo.push_back(stRewardItem);
        }
        m_psRouletteEventInfo.Clear();
        m_psRouletteEventInfo.psRewardList = psRewardItemList;
    }
}

// ============================================================================
// SendDBRouletteRewardInfo - 对齐 IDA 0x14003ACA0
// IDA: 发送轮盘奖励信息请求到 DB (包类型 0x49/0x2D)
// ============================================================================
void CRouletteEventMgr::SendDBRouletteRewardInfo()
{
    XControlServer* pServer = XControlServer::Instance();
    if (!pServer) return;

    // 对齐 IDA: XSendDBPacket(0, 0x49, 0x2D)
    XSendDBPacket xSendDBPacket(0, 0x49, 0x2D);
    xSendDBPacket.XParse << m_psRouletteEventInfo.nEventID;

    pServer->SendDBGame(xSendDBPacket);
}

// ============================================================================
// SendRouletteEvent - 对齐 IDA 0x14003AD30
// IDA: 发送轮盘活动信息到所有 GameServer (包类型 0xF2/0x74)
// ============================================================================
void CRouletteEventMgr::SendRouletteEvent(bool bSendGameServer)
{
    XControlServer* pServer = XControlServer::Instance();
    if (!pServer) return;

    // 对齐 IDA: XSendPacket(0xF2, 0x74)
    XSendPacket xSendPacket(0xF2, 0x74);

    // 对齐 IDA: operator<<(&xSendPacket, &m_psRouletteEventInfo)
    xSendPacket << m_psRouletteEventInfo;

    // 对齐 IDA: XParse << bSend
    xSendPacket.XParse << bSendGameServer;

    // 对齐 IDA: SendPacketAll
    pServer->SendPacketAll(xSendPacket, 0);
}

// ============================================================================
// UpdateRouletteItemInfo - 对齐 IDA 0x14003ADE0
// IDA: 更新轮盘奖励项计数
// ============================================================================
void CRouletteEventMgr::UpdateRouletteItemInfo(PS_ROULETTE_EVENT_UPDATE_SERVER& stUpdateInfo)
{
    // 对齐 IDA: CFAutoSlimWriteLock::CFAutoSlimWriteLock
    CFAutoSlimWriteLock lock(&m_rwEventRouletteLock);

    // 对齐 IDA: 遍历 psRewardList 查找匹配的 nRewradIndex
    for (size_t i = 0; i < m_psRouletteEventInfo.psRewardList.vecInfo.size(); ++i) {
        auto& item = m_psRouletteEventInfo.psRewardList.vecInfo[i];
        // 对齐 IDA: if (item->nRewradIndex == stUpdateInfo->nRewardID)
        if (item.nRewradIndex == stUpdateInfo.nIndex) {
            item.nCurCount = stUpdateInfo.nValue;
            break;
        }
    }

    // 对齐 IDA: SendRouletteEvent(0)
    SendRouletteEvent(false);
}

// ============================================================================
// SetRouletteRewardInfo - 对齐 IDA 0x14003AEB0
// IDA: 设置轮盘奖励信息（从 DB 返回）
// ============================================================================
void CRouletteEventMgr::SetRouletteRewardInfo(PS_DB_ROULETTE_REWARD_INFO& stRewardInfo)
{
    // 对齐 IDA: CFAutoSlimWriteLock::CFAutoSlimWriteLock
    CFAutoSlimWriteLock lock(&m_rwEventRouletteLock);

    // 对齐 IDA: 双重循环匹配 nEventID 和 nRewradIndex
    for (size_t i = 0; i < m_psRouletteEventInfo.psRewardList.vecInfo.size(); ++i) {
        auto& rewardItem = m_psRouletteEventInfo.psRewardList.vecInfo[i];

        for (size_t k = 0; k < stRewardInfo.vecInfo.size(); ++k) {
            auto& infoItem = stRewardInfo.vecInfo[k];

            // 对齐 IDA: 检查 nEventID 匹配
            if (m_psRouletteEventInfo.nEventID == infoItem.nEventID) {
                // 对齐 IDA: 检查 nRewradIndex 匹配
                if (rewardItem.nRewradIndex == infoItem.nRewardID) {
                    rewardItem.nCurCount = infoItem.nRemainCount;
                }
            }
        }
    }

    m_bLoad = true;

    LogHelper::LogDebug("game.contents",
        "SetRouletteRewardInfo - Complete Load Roulette Info[EventID:%d]",
        m_psRouletteEventInfo.nEventID);

    // 对齐 IDA: SendRouletteEvent(1)
    SendRouletteEvent(true);
}
