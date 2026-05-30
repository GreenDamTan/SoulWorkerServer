#include "Soulworker/GameServer/XRelayServer/Force.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/UserObject.h"


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
    // 对齐 IDA 0x1400147A0: 使用 GetTickCount64() + 300000 (5分钟)
    m_dwKickOutTime = GetTickCount64() + 300000ull;
}

bool CForceMember::GetMemberInfo(ST_FORCE_MEMBER& forceMember) const {
    forceMember = m_stForceMember;
    return true;
}

// 对齐 IDA 0x1401CA0A0: CForceMember::Clear
// 注意: IDA 反编译显示此函数涉及 m_pMember (CUser 指针) 和 CGocParty/CGocForce 组件
// 当前简化实现：仅重置基本字段
void CForceMember::Clear() {
    // IDA: 如果 m_pMember 存在，检查区域并调用 CGocParty::Clear
    // 当前简化：直接清除成员信息
    m_stForceMember = ST_FORCE_MEMBER{};
    m_dwKickOutTime = 0;
    m_uxEnterMap = UXMapID{};
    // TODO: 需要实现完整的 IDA 逻辑，涉及 CGocForce 组件清理
}

CForce::CForce(PS_REQ_FORCE_CREATE& stCreateForce)
    : m_dwForceID(stCreateForce.dwForceID), m_dwMasterID(stCreateForce.masterInfo.dwMemberID) {
    AddMember(stCreateForce.masterInfo);
    AddMember(stCreateForce.memberInfo);
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

void CForce::AddMember(ST_FORCE_MEMBER& stForceMember) {
    auto member = std::make_shared<CForceMember>(stForceMember);
    m_mapForceMember[stForceMember.dwMemberID] = member;
}

// 对齐 IDA 0x1400135A0: SetMemberInfo 不调用 Login，Logout 仅设 kickout timer
void CForce::SetMemberInfo(ST_FORCE_MEMBER& forceMember) {
    const auto it = m_mapForceMember.find(forceMember.dwMemberID);
    if (it == m_mapForceMember.end() || !it->second) {
        return;  // 对齐 IDA: 成员不存在则直接返回
    }

    const std::shared_ptr<CForceMember> member = it->second;
    ST_FORCE_MEMBER normalized = forceMember;
    if (normalized.bLogin) {
        normalized.nHP = normalized.nMaxHP;
    } else {
        normalized.nHP = 0;
        normalized.nMaxHP = 0;
        member->Logout();  // IDA: 仅设 kickout timer
        // 对齐 IDA: 如果是队长下线，删除招募
        if (normalized.dwMemberID == m_dwMasterID) {
            XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
            relayServer.GetPartyMatchingMgr().ReqPartyRecruitDel(m_dwMasterID);
        }
    }
    member->SetMemberInfo(normalized);
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

// 对齐 IDA 0x140094650
void CForce::SetMemberEnterMap(std::uint32_t dwMemberID, UXMapID uxMapID) {
    const auto it = m_mapForceMember.find(dwMemberID);
    if (it != m_mapForceMember.end() && it->second) {
        it->second->SetEnterMap(uxMapID);
    }
}

// 对齐 IDA 0x140013320: SetForceInfo 需要检查成员在线状态
void CForce::SetForceInfo(PS_FORCE_INFO& forceInfo) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    m_dwForceID = forceInfo.dwForceID;
    m_dwMasterID = forceInfo.dwMaster;
    m_uxMazeID = forceInfo.uxMazeID;
    m_byForceType = forceInfo.byForceType;
    m_mapForceMember.clear();

    for (auto& member : forceInfo.vecForceMember) {
        // 对齐 IDA: 检查成员是否在线
        std::shared_ptr<CUserObject> pMemberUser = relayServer.GetUser(member.dwMemberID);
        if (!pMemberUser) {
            // 对齐 IDA: 如果用户不在线，设置 bLogin = false
            member.bLogin = false;
        }
        // 对齐 IDA: 如果 bLogin 为 false（无论原因），清除 HP
        if (!member.bLogin) {
            member.nHP = 0;
            member.nMaxHP = 0;
        }
        AddMember(member);
    }
}

bool CForce::GetMemberInfo(std::uint32_t dwMemberID, ST_FORCE_MEMBER& forceMember) {
    const auto it = m_mapForceMember.find(dwMemberID);
    if (it == m_mapForceMember.end() || !it->second) {
        return false;
    }
    return it->second->GetMemberInfo(forceMember);
}

void CForce::GetForceInfo(PS_FORCE_INFO& forceInfo) {
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

bool CForce::ChangeMaster(std::uint32_t dwNewMasterID, bool bLeave) {
    const auto it = m_mapForceMember.find(dwNewMasterID);
    if (it == m_mapForceMember.end() || !it->second) {
        return false;
    }
    if (bLeave) {
        // When called from leave flow, new master must be logged in
        ST_FORCE_MEMBER memberInfo{};
        it->second->GetMemberInfo(memberInfo);
        if (!memberInfo.bLogin) {
            return false;
        }
    }
    m_dwMasterID = dwNewMasterID;
    return true;
}

std::uint32_t CForce::FindNewMaster() const {
    for (const auto& [memberID, member] : m_mapForceMember) {
        if (member && memberID != m_dwMasterID) {
            return memberID;
        }
    }
    return 0;
}

void CForce::RemoveMember(std::uint32_t dwMemberID) {
    m_mapForceMember.erase(dwMemberID);
}

void CForce::Kickout(std::uint32_t dwMemberID) {
    m_mapForceMember.erase(dwMemberID);
}

void CForce::GetForceMemberList(ST_PARTY_MEMBER_LIST& stMemberList) {
    stMemberList.vecInfo.clear();
    stMemberList.vecInfo.reserve(m_mapForceMember.size());
    for (const auto& [memberID, member] : m_mapForceMember) {
        static_cast<void>(memberID);
        if (!member) {
            continue;
        }
        ST_FORCE_MEMBER forceMember{};
        if (member->GetMemberInfo(forceMember)) {
            // 对齐 IDA 0x140013950: 将 ST_FORCE_MEMBER 转为 ST_PARTY_MEMBER
            ST_PARTY_MEMBER partyMember{};
            partyMember.dwMemberID = forceMember.dwMemberID;
            partyMember.byLevel = forceMember.byLevel;
            partyMember.byClass = forceMember.byClass;
            partyMember.byAwaken = forceMember.byAwaken;
            partyMember.dwProfilePhotoID = forceMember.dwProfilePhotoID;
            partyMember.nMapID = forceMember.nMapID;
            partyMember.nChannel = forceMember.nChannel;
            partyMember.nMaxHP = forceMember.nMaxHP;
            partyMember.nHP = forceMember.nHP;
            partyMember.bLogin = forceMember.bLogin;
            partyMember.uxMapID = forceMember.uxMapID;
            wcscpy_s(partyMember.strName, forceMember.strName);
            stMemberList.vecInfo.push_back(partyMember);
        }
    }
}

// 对齐 IDA 0x140013BA0
void CForce::SendNameChange(std::uint32_t dwActorID, const wchar_t* pChangeName) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 构建 PS_CHANGE_NAME 结构
    PS_CHANGE_NAME stInfo{};
    stInfo.dwActorID = dwActorID;
    wcscpy_s(stInfo.szChangeName, pChangeName);

    // 遍历所有 force 成员
    for (const auto& [memberID, pMember] : m_mapForceMember) {
        if (!pMember) {
            continue;
        }

        ST_FORCE_MEMBER forceMember{};
        pMember->GetMemberInfo(forceMember);

        if (forceMember.dwMemberID == dwActorID) {
            // 更新该成员的名字
            wcscpy_s(forceMember.strName, pChangeName);
            pMember->SetMemberInfo(forceMember);
        } else {
            // 向其他成员广播改名消息 0xFA/0x20
            const std::shared_ptr<CUserObject> pMemberUser = relayServer.GetUser(forceMember.dwMemberID);
            if (!pMemberUser) {
                continue;
            }

            XSendPacket xSendPacket(0xFAu, 0x20u);
            xSendPacket.XParse << forceMember.dwMemberID;
            xSendPacket << stInfo;
            relayServer.SendPacket(pMemberUser->GetServerID(), xSendPacket);
        }
    }
}

// 对齐 IDA 0x1401B6460: CForce::Clear
// 遍历所有成员，调用 Clear() 并删除
void CForce::Clear() {
    // 遍历所有 force 成员
    for (auto it = m_mapForceMember.begin(); it != m_mapForceMember.end(); ) {
        auto& pMember = it->second;
        if (pMember) {
            // IDA: 调用 CForceMember::Clear
            pMember->Clear();
        }
        // IDA: 删除成员并推进迭代器
        it = m_mapForceMember.erase(it);
    }

    // IDA: 重置基本字段
    m_dwForceID = 0;
    m_dwMasterID = 0;
    m_byForceType = 0;
    m_uxMazeID = UXMapID{};
}
