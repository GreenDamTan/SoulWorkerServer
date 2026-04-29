// CPartyManager.cpp
// CPartyManager 队伍管理器实现
// 对齐 IDA ControlServer.exe

#include "CPartyManager.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// 对齐 IDA 0x140030CA0: 构造函数 - m_bLoad 初始化为 true
CPartyManager::CPartyManager()
    : m_bLoad(true)
{
}

CPartyManager::~CPartyManager()
{
    Clear();
}

// 对齐 IDA 0x1400399A0: SetMember 设置成员位置
void CPartyManager::SetMember(int nPartyID, int nActorID, const UXMapID& uxMapID)
{
    auto it = m_mapParty.find(nPartyID);
    if (it != m_mapParty.end()) {
        // Party 已存在，直接设置成员
        std::tr1::shared_ptr<CParty> pParty = it->second;
        if (pParty) {
            pParty->SetMember(nActorID, uxMapID);
        }
    } else {
        // Party 不存在，创建新 Party
        std::tr1::shared_ptr<CParty> pParty(new CParty(nPartyID));
        if (pParty) {
            pParty->SetMember(nActorID, uxMapID);
            m_mapParty[nPartyID] = pParty;
        }
    }
    GreenDamTan_log(__FILE__, __FUNCTION__, "PartyManager::SetMember party=%d actor=%d", nPartyID, nActorID);
}

// 对齐 IDA 0x140039C00: SetMazeID 设置迷宫ID
bool CPartyManager::SetMazeID(int nPartyID, const UXMapID& uxMapID, const UXMapID& uxBeforeMapID)
{
    auto it = m_mapParty.find(nPartyID);
    if (it == m_mapParty.end()) {
        return false;
    }

    std::tr1::shared_ptr<CParty> pParty = it->second;
    if (!pParty) {
        return false;
    }

    // 验证 beforeMapID 匹配
    UXMapID uxCurrentMapID;
    pParty->GetMazeID(&uxCurrentMapID);

    // UXMapID 比较需要使用 nMapID
    if (uxCurrentMapID.nMapID != uxBeforeMapID.nMapID) {
        return false;
    }

    pParty->SetMazeID(uxMapID);
    return true;
}

// 对齐 IDA: RemoveMember 移除成员
void CPartyManager::RemoveMember(int nPartyID, int nActorID)
{
    auto it = m_mapParty.find(nPartyID);
    if (it == m_mapParty.end()) {
        return;
    }

    std::tr1::shared_ptr<CParty> pParty = it->second;
    if (!pParty) {
        return;
    }

    pParty->RemoveMember(nActorID);

    // 如果成员为空，删除整个 Party
    if (pParty->IsEmpty()) {
        m_mapParty.erase(it);
    }
}

// 对齐 IDA 0x140030E80: IsParty 检查队伍是否存在
bool CPartyManager::IsParty(int nPartyID)
{
    return m_mapParty.find(nPartyID) != m_mapParty.end();
}

// 对齐 IDA 0x140030FE0: FindSamePlace 查找同位置成员
// 参数: dwPartyID (PartyID), dwActorID (请求者ActorID), wMapID (地图ID)
UXMapID CPartyManager::FindSamePlace(int nPartyID, unsigned int dwActorID, unsigned int dwMapID)
{
    auto it = m_mapParty.find(nPartyID);
    if (it == m_mapParty.end()) {
        return UXMapID{};
    }

    std::tr1::shared_ptr<CParty> pParty = it->second;
    if (!pParty) {
        return UXMapID{};
    }

    // 调用 CParty::FindSamePlace 查找同位置成员
    return pParty->FindSamePlace(static_cast<unsigned short>(dwMapID), dwActorID);
}
