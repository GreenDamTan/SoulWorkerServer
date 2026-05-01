// ControlServer Simple Party Manager Implementation
// 对齐 ControlServer.exe IDA 反编译

#include "Soulworker/GameServer/XLoginServer/PartyManager.h"

// ============================================================================
// CPartyManager Implementation
// ============================================================================

std::shared_ptr<CParty> CPartyManager::GetOrCreateParty(std::uint32_t dwPartyID) {
    auto& partySlot = m_mapParty[dwPartyID];
    if (!partySlot) {
        partySlot = std::make_shared<CParty>(static_cast<int>(dwPartyID));
    }
    return partySlot;
}

// 对齐 IDA 0x1400399A0: SetMember
void CPartyManager::SetMember(int nPartyID, int nActorID, UXMapID uxMapID) {
    // 对齐 IDA: 在 m_mapParty 中查找或创建 Party，调用 SetMember
    auto it = m_mapParty.find(nPartyID);
    if (it != m_mapParty.end() && it->second) {
        // 队伍存在，调用 CForce::SetMember (对齐 IDA)
        it->second->SetMember(nActorID, uxMapID);
    } else {
        // 队伍不存在，创建新队伍并添加成员 (对齐 IDA: 使用 CForce 构造)
        auto pParty = GetOrCreateParty(static_cast<std::uint32_t>(nPartyID));
        pParty->SetMember(nActorID, uxMapID);
    }
    // 注意: IDA 中没有 m_mapPartyUser 的更新，已移除
}

// 对齐 IDA 0x140039B30: RemoveMember
void CPartyManager::RemoveMember(int nPartyID, int nActorID) {
    // 查找队伍
    auto it = m_mapParty.find(static_cast<std::uint32_t>(nPartyID));
    if (it == m_mapParty.end() || !it->second) {
        return;
    }

    // 移除成员
    it->second->RemoveMember(nActorID);

    // 注意: IDA 中没有 m_mapPartyUser 的清除，已移除

    // 如果队伍为空，删除队伍
    if (it->second->IsEmpty()) {
        m_mapParty.erase(it);
    }
}

// 对齐 IDA 0x140030F60: SetMazeID (2参数版本 - ControlServer使用)
void CPartyManager::SetMazeID(int nPartyID, UXMapID uxMapID) {
    auto it = m_mapParty.find(static_cast<std::uint32_t>(nPartyID));
    if (it != m_mapParty.end() && it->second) {
        it->second->SetMazeID(uxMapID);
    }
}

// 对齐 IDA 0x140039C00: SetMazeID (3参数版本)
bool CPartyManager::SetMazeID(int nPartyID, UXMapID uxMapID, UXMapID uxBeforeMapID) {
    // 查找队伍
    auto it = m_mapParty.find(static_cast<std::uint32_t>(nPartyID));
    if (it == m_mapParty.end() || !it->second) {
        return false;
    }

    // 验证 beforeMapID 匹配当前 MazeID
    UXMapID currentMazeID = it->second->GetMazeID();
    if (currentMazeID.nMapID != uxBeforeMapID.nMapID) {
        return false;
    }

    // 设置新的 MazeID
    it->second->SetMazeID(uxMapID);
    return true;
}

// 对齐 IDA CForceManager::GetMazeID 0x140039D00
bool CPartyManager::GetMazeID(int nPartyID, int nActorID, UXMapID* puxMapID) {
    if (!puxMapID) {
        return false;
    }

    // 查找队伍
    auto it = m_mapParty.find(static_cast<std::uint32_t>(nPartyID));
    if (it == m_mapParty.end() || !it->second) {
        return false;
    }

    // 获取 MazeID
    *puxMapID = it->second->GetMazeID();
    return true;
}

// 对齐 IDA 0x140030FE0: FindSamePlace
UXMapID CPartyManager::FindSamePlace(std::uint32_t dwPartyID, std::uint32_t dwActorID, bool bRaid) {
    // 查找队伍
    auto it = m_mapParty.find(dwPartyID);
    if (it == m_mapParty.end() || !it->second) {
        return UXMapID();  // 返回默认构造
    }

    // bRaid 决定使用 HIWORD 还是完整 mapID 比较
    // 对齐 IDA: 参数为 wMapID (从 dwActorID 或外部传入)
    // 这里简化处理，使用 actorID 的低16位作为 wMapID
    std::uint16_t wMapID = static_cast<std::uint16_t>(dwActorID & 0xFFFF);

    return it->second->FindSamePlace(wMapID, dwActorID);
}

// ============================================================================
// CForceManager Implementation
// 注意: XLoginServer 继承设计 - CForceManager 继承 CPartyManager
// IDA 显示 CForceManager 和 CPartyManager 布局相同 (40 bytes)
// 使用 m_mapParty 并 static_cast 为 CForce* (对齐 IDA 行为)
// ============================================================================

// 对齐 IDA: GetOrCreateForce - 重用父类的 m_mapParty
std::shared_ptr<CForce> CForceManager::GetOrCreateForce(std::uint32_t dwForceID) {
    auto it = m_mapParty.find(dwForceID);
    if (it != m_mapParty.end() && it->second) {
        return std::static_pointer_cast<CForce>(it->second);
    }
    // 创建新 Force
    auto pForce = std::make_shared<CForce>(static_cast<int>(dwForceID));
    m_mapParty[dwForceID] = pForce;
    return pForce;
}

// 对齐 IDA 0x140030CF0: SetMember
void CForceManager::SetMember(int nForceID, int nActorID, UXMapID uxMapID) {
    // 对齐 IDA: 在 map 中查找 Force，不存在则创建
    auto it = m_mapParty.find(static_cast<std::uint32_t>(nForceID));
    if (it != m_mapParty.end() && it->second) {
        // Force 存在，调用 CForce::SetMember (实际是 CParty::SetMember)
        it->second->SetMember(nActorID, uxMapID);
    } else {
        // Force 不存在，创建新 Force 并添加成员 (对齐 IDA: new CForce + insert)
        auto pForce = std::make_shared<CForce>(nForceID);
        pForce->SetMember(nActorID, uxMapID);
        m_mapParty[static_cast<std::uint32_t>(nForceID)] = pForce;
    }
}

// 对齐 IDA 0x140039B30: RemoveMember
void CForceManager::RemoveMember(int nForceID, int nActorID) {
    // 查找 force
    auto it = m_mapParty.find(static_cast<std::uint32_t>(nForceID));
    if (it == m_mapParty.end() || !it->second) {
        return;
    }

    // 移除成员
    it->second->RemoveMember(nActorID);

    // 如果 force 为空，删除 force
    if (it->second->IsEmpty()) {
        m_mapParty.erase(it);
    }
}

// 对齐 IDA 0x140030F60: SetMazeID
void CForceManager::SetMazeID(int nForceID, UXMapID uxMapID) {
    auto it = m_mapParty.find(static_cast<std::uint32_t>(nForceID));
    if (it != m_mapParty.end() && it->second) {
        it->second->SetMazeID(uxMapID);
    }
}

// 对齐 IDA: SetMazeID (3参数版本)
bool CForceManager::SetMazeID(int nForceID, UXMapID uxMapID, UXMapID uxBeforeMapID) {
    auto it = m_mapParty.find(static_cast<std::uint32_t>(nForceID));
    if (it == m_mapParty.end() || !it->second) {
        return false;
    }

    // 验证 beforeMapID 匹配当前 MazeID
    UXMapID currentMazeID = it->second->GetMazeID();
    if (currentMazeID.nMapID != uxBeforeMapID.nMapID) {
        return false;
    }

    // 设置新的 MazeID
    it->second->SetMazeID(uxMapID);
    return true;
}

// 对齐 IDA 0x140039D00: GetMazeID
bool CForceManager::GetMazeID(int nForceID, int nActorID, UXMapID* puxMapID) {
    if (!puxMapID) {
        return false;
    }

    // 查找 force
    auto it = m_mapParty.find(static_cast<std::uint32_t>(nForceID));
    if (it == m_mapParty.end() || !it->second) {
        return false;
    }

    // 获取 MazeID (对齐 IDA: nActorID 参数存在但未使用)
    *puxMapID = it->second->GetMazeID();
    return true;
}

// 对齐 IDA 0x140030EE0: IsFull
bool CForceManager::IsFull(int nForceID) {
    auto it = m_mapParty.find(static_cast<std::uint32_t>(nForceID));
    if (it == m_mapParty.end() || !it->second) {
        return false;  // 对齐 IDA: 未找到返回 false
    }
    return it->second->IsFull();
}
