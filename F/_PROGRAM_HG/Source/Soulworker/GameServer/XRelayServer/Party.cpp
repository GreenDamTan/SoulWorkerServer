#include "Soulworker/GameServer/XRelayServer/Party.h"

CParty::CParty(PS_REQ_PARTY_CREATE& stCreateParty)
    : m_dwPartyID(stCreateParty.dwPartyID), m_dwMasterID(stCreateParty.masterInfo.dwMemberID) {
    AddMember(stCreateParty.masterInfo);
    AddMember(stCreateParty.memberInfo);
}

std::shared_ptr<CPartyMember> CParty::GetOrCreateMember(std::uint32_t dwMemberID) {
    auto& memberSlot = m_mapPartyMember[dwMemberID];
    if (!memberSlot) {
        memberSlot = std::make_shared<CPartyMember>();
    }
    return memberSlot;
}

void CParty::AddMember(const ST_PARTY_MEMBER& stPartyMember) {
    auto member = std::make_shared<CPartyMember>(stPartyMember);
    m_mapPartyMember[stPartyMember.dwMemberID] = member;
}

void CParty::SetMemberInfo(const ST_PARTY_MEMBER& partyMember) {
    auto member = GetOrCreateMember(partyMember.dwMemberID);
    member->SetMemberInfo(partyMember);
}

void CParty::SetMemberInfo(std::uint32_t dwMemberID, UXMapID uxMapID, int nMaxHP) {
    // 对齐 IDA 0x1400136A0: 进入服务器时更新成员信息
    auto it = m_mapPartyMember.find(dwMemberID);
    if (it != m_mapPartyMember.end() && it->second) {
        it->second->SetEnterInfo(uxMapID, nMaxHP);
        it->second->Login();
    }
}

void CParty::SetPartyInfo(const PS_PARTY_INFO& partyInfo) {
    m_dwPartyID = partyInfo.dwPartyID;
    m_dwMasterID = partyInfo.dwMaster;
    m_mapPartyMember.clear();

    for (const auto& member : partyInfo.vecPartyMember) {
        AddMember(member);
    }
}

bool CParty::GetMemberInfo(std::uint32_t dwMemberID, ST_PARTY_MEMBER& partyMember) const {
    const auto it = m_mapPartyMember.find(dwMemberID);
    if (it == m_mapPartyMember.end()) {
        return false;
    }
    return it->second->GetMemberInfo(partyMember);
}

void CParty::GetMemberInfo(std::uint32_t dwMemberID, ST_PARTY_MEMBER* pPartyMember) const {
    if (!pPartyMember) {
        return;
    }
    const auto it = m_mapPartyMember.find(dwMemberID);
    if (it != m_mapPartyMember.end() && it->second) {
        it->second->GetMemberInfo(*pPartyMember);
    }
}

void CParty::GetPartyInfo(PS_PARTY_INFO& partyInfo) const {
    partyInfo.dwPartyID = m_dwPartyID;
    partyInfo.dwMaster = m_dwMasterID;
    partyInfo.vecPartyMember.clear();
    partyInfo.vecPartyMember.reserve(m_mapPartyMember.size());
    for (const auto& [memberID, member] : m_mapPartyMember) {
        static_cast<void>(memberID);
        if (!member) {
            continue;
        }
        ST_PARTY_MEMBER partyMember{};
        if (member->GetMemberInfo(partyMember)) {
            partyInfo.vecPartyMember.push_back(partyMember);
        }
    }
}

void CParty::GetPartyMemberList(std::vector<ST_PARTY_MEMBER>& vecMember) const {
    vecMember.clear();
    vecMember.reserve(m_mapPartyMember.size());
    for (const auto& [memberID, member] : m_mapPartyMember) {
        static_cast<void>(memberID);
        if (!member) {
            continue;
        }
        ST_PARTY_MEMBER partyMember{};
        if (member->GetMemberInfo(partyMember)) {
            vecMember.push_back(partyMember);
        }
    }
}

void CParty::RemoveMember(std::uint32_t dwMemberID) {
    // 对齐 IDA: 从队伍移除成员
    m_mapPartyMember.erase(dwMemberID);
}

bool CParty::ChangeMaster(std::uint32_t dwNewMasterID, bool bIsDBRequest) {
    // 对齐 IDA: 变更队长
    static_cast<void>(bIsDBRequest);  // 未使用的参数

    // 检查新队长是否在队伍中
    if (m_mapPartyMember.find(dwNewMasterID) == m_mapPartyMember.end()) {
        return false;  // 新队长不存在于队伍中
    }

    m_dwMasterID = dwNewMasterID;
    return true;
}

std::uint32_t CParty::FindNewMaster() const {
    // 对齐 IDA: 查找新队长（返回第一个非队长的成员ID）
    for (const auto& [memberID, member] : m_mapPartyMember) {
        if (memberID != m_dwMasterID) {
            return memberID;
        }
    }
    return 0;  // 没有其他成员
}

void CParty::Kickout(std::uint32_t dwMemberID) {
    // 对齐 IDA: 踢出成员（与 RemoveMember 相同）
    RemoveMember(dwMemberID);
}
