#include "Soulworker/GameServer/XRelayServer/Force.h"

#include <chrono>

namespace {
std::uint64_t GreenDamTan_GetForceTickMs() {
    using namespace std::chrono;
    return static_cast<std::uint64_t>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}
}

void CForceMember::SetMemberInfo(const ST_FORCE_MEMBER& forceMember) {
    m_stForceMember = forceMember;
    m_uxEnterMap = forceMember.uxMapID;
    if (m_stForceMember.bLogin) {
        if (m_stForceMember.nHP == 0) {
            m_stForceMember.nHP = m_stForceMember.nMaxHP;
        }
        m_dwKickOutTime = 0;
    }
}

void CForceMember::SetEnterMap(UXMapID uxMapID) {
    m_uxEnterMap = uxMapID;
    m_stForceMember.uxMapID = uxMapID;
    m_stForceMember.nMapID = static_cast<std::int32_t>(uxMapID.parts.mapID);
    m_stForceMember.nChannel = static_cast<std::int32_t>(uxMapID.parts.channel);
}

void CForceMember::Login() {
    m_stForceMember.bLogin = true;
    m_stForceMember.nHP = m_stForceMember.nMaxHP;
    m_dwKickOutTime = 0;
}

void CForceMember::Logout() {
    m_stForceMember.bLogin = false;
    m_stForceMember.nHP = 0;
    m_stForceMember.nMaxHP = 0;
    m_dwKickOutTime = GreenDamTan_GetForceTickMs() + 300000ull;
}

bool CForceMember::GetMemberInfo(ST_FORCE_MEMBER& forceMember) const {
    forceMember = m_stForceMember;
    return true;
}

std::shared_ptr<CForceMember> CForce::GetOrCreateMember(std::uint32_t dwMemberID) {
    auto& memberSlot = m_mapForceMember[dwMemberID];
    if (!memberSlot) {
        ST_FORCE_MEMBER forceMember{};
        forceMember.dwMemberID = dwMemberID;
        memberSlot = std::make_shared<CForceMember>(forceMember);
    }
    return memberSlot;
}

void CForce::SetMemberInfo(const ST_FORCE_MEMBER& forceMember) {
    const std::shared_ptr<CForceMember> member = GetOrCreateMember(forceMember.dwMemberID);
    ST_FORCE_MEMBER normalized = forceMember;
    if (normalized.bLogin) {
        normalized.nHP = normalized.nMaxHP;
    } else {
        normalized.nHP = 0;
        normalized.nMaxHP = 0;
        member->Logout();
    }
    member->SetMemberInfo(normalized);
    if (normalized.bLogin) {
        member->Login();
    }
    if (m_dwMasterID == 0) {
        m_dwMasterID = normalized.dwMemberID;
    }
    m_uxMazeID = normalized.uxMapID;
}

void CForce::SetMemberInfo(std::uint32_t dwMemberID, UXMapID uxMapID, int nMaxHP) {
    const std::shared_ptr<CForceMember> member = GetOrCreateMember(dwMemberID);
    ST_FORCE_MEMBER forceMember{};
    member->GetMemberInfo(forceMember);
    forceMember.dwMemberID = dwMemberID;
    forceMember.uxMapID = uxMapID;
    forceMember.nMapID = static_cast<std::int32_t>(uxMapID.parts.mapID);
    forceMember.nChannel = static_cast<std::int32_t>(uxMapID.parts.channel);
    forceMember.nMaxHP = nMaxHP;
    forceMember.nHP = nMaxHP;
    forceMember.bLogin = true;
    member->SetMemberInfo(forceMember);
    member->SetEnterMap(uxMapID);
    member->Login();
    if (m_dwMasterID == 0) {
        m_dwMasterID = dwMemberID;
    }
    m_uxMazeID = uxMapID;
}

bool CForce::GetMemberInfo(std::uint32_t dwMemberID, ST_FORCE_MEMBER& forceMember) const {
    const auto it = m_mapForceMember.find(dwMemberID);
    if (it == m_mapForceMember.end() || !it->second) {
        return false;
    }
    return it->second->GetMemberInfo(forceMember);
}

void CForce::GetForceInfo(PS_FORCE_INFO& forceInfo) const {
    forceInfo.dwForceID = m_dwForceID;
    forceInfo.dwMaster = m_dwMasterID;
    forceInfo.uxMazeID = m_uxMazeID;
    forceInfo.byForceType = m_byForceType;
    forceInfo.vecForceMember.clear();
    forceInfo.vecForceMember.reserve(m_mapForceMember.size());
    for (const auto& [memberID, member] : m_mapForceMember) {
        static_cast<void>(memberID);
        if (!member) {
            continue;
        }
        ST_FORCE_MEMBER forceMember{};
        if (member->GetMemberInfo(forceMember)) {
            forceInfo.vecForceMember.push_back(forceMember);
        }
    }
}
