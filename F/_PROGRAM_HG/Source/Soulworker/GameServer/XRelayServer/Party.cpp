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
