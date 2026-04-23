#include "Soulworker/GameServer/XRelayServer/PartyRecruit.h"

#include <algorithm>
#include <ctime>
#include <cwchar>

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/UserPartyInfo.h"

namespace {
std::int64_t GreenDamTan_GetCurDateSec() {
    return static_cast<std::int64_t>(std::time(nullptr));
}
}

void CPartyRecruit::GetRecruitInfo(ST_PARTY_RECRUIT_INFO& stRecruit) const {
    stRecruit.stRecruit = m_stPartyRecruit;
    stRecruit.vecMember.clear();
    stRecruit.vecMember.reserve(m_setMember.size());
    for (const std::uint32_t memberID : m_setMember) {
        stRecruit.vecMember.push_back(memberID);
    }
}

void CPartyRecruit::SetRecruitDate() {
    m_biRecruitDate = GreenDamTan_GetCurDateSec() + 1800;
}

void CPartyRecruit::ClearRecruitDate() {
    m_biRecruitDate = 0;
    for (const std::uint32_t memberID : m_setMember) {
        if (const std::shared_ptr<CUserPartyInfo> userParty = TXSingleton<XRelayServer>::Instance()->GetPartyUser(memberID)) {
            userParty->SetRecruitDate(0);
        }
    }
}

void CPartyRecruit::ApplyMemberClear() {
    // 对齐 IDA 0x1400AE4A0: 保留原版二进制 bug - 条件检查固定使用 stInfo[0]，但清除 stInfo[i]
    // IDA: cmp dword ptr [rax+0A8h], 0 (固定偏移 0xA8 = stInfo[0])
    for (int i = 0; i < 10; ++i) {
        // 条件检查和 GetPartyUser 总是使用 stInfo[0]（原始二进制行为）
        if (m_stApplicantList.stInfo[0].stMember.dwMemberID != 0) {
            const std::shared_ptr<CUserPartyInfo> pPartyUser =
                TXSingleton<XRelayServer>::Instance()->GetPartyUser(m_stApplicantList.stInfo[0].stMember.dwMemberID);
            if (pPartyUser) {
                pPartyUser->DelPartyRecruit(m_stPartyRecruit.dwRecruitID, false);
            }
            // 清除当前索引元素（原始二进制行为）
            m_stApplicantList.stInfo[i] = {};
        }
    }
}

void CPartyRecruit::GetMember(std::vector<std::uint32_t>& vecMember) const {
    vecMember.clear();
    vecMember.reserve(m_setMember.size());
    for (const std::uint32_t memberID : m_setMember) {
        vecMember.push_back(memberID);
    }
}

void CPartyRecruit::GetPartyMemberList(ST_PARTY_RECRUIT_APPLY_INFO& stInfo) {
    stInfo.dwMasterUCID = GetMasterID();
    stInfo.byPartyGroupType = GetPartyGroupType();

    if (stInfo.byPartyGroupType == 1) {
        // Party类型：尝试从PartyManager获取
        std::shared_ptr<CParty> pParty = TXSingleton<XRelayServer>::Instance()->GetPartyManager().GetParty(m_stPartyRecruit.dwPartyID);
        if (!pParty) {
            // Party不存在，从m_setMember遍历获取用户信息
            for (const std::uint32_t memberID : m_setMember) {
                const std::shared_ptr<CUserObject> user = TXSingleton<XRelayServer>::Instance()->GetUser(memberID);
                if (!user) {
                    continue;
                }
                ST_PARTY_MEMBER stMember{};
                stMember.bLogin = true;
                stMember.byClass = user->GetClass();
                stMember.byAwaken = user->GetAwaken();
                stMember.dwProfilePhotoID = user->GetProfilePhoto();
                stMember.byLevel = user->GetLevel();
                stMember.dwMemberID = user->GetCID();
                stMember.nChannel = static_cast<std::int16_t>(user->GetChannel());
                stMember.nHP = user->GetHP();
                stMember.nMapID = static_cast<std::int16_t>(user->GetMapID());
                stMember.nMaxHP = user->GetMaxHP();
                const std::wstring name = user->GetName();
                std::wcsncpy(stMember.strName, name.c_str(), 20);
                stMember.strName[20] = L'\0';
                stMember.uxMapID = user->GetMapIns();
                stInfo.stMemberList.vecInfo.push_back(stMember);
            }
        } else {
            // Party存在，使用Party的成员列表
            pParty->GetPartyMemberList(stInfo.stMemberList.vecInfo);
        }
    } else if (stInfo.byPartyGroupType == 2) {
        // Force类型：尝试从ForceManager获取
        std::shared_ptr<CForce> pForce = TXSingleton<XRelayServer>::Instance()->GetForceManager().GetForce(m_stPartyRecruit.dwPartyID);
        if (!pForce) {
            // Force不存在，从m_setMember遍历获取用户信息
            for (const std::uint32_t memberID : m_setMember) {
                const std::shared_ptr<CUserObject> user = TXSingleton<XRelayServer>::Instance()->GetUser(memberID);
                if (!user) {
                    continue;
                }
                ST_PARTY_MEMBER stMember{};
                stMember.bLogin = true;
                stMember.byClass = user->GetClass();
                stMember.byAwaken = user->GetAwaken();
                stMember.dwProfilePhotoID = user->GetProfilePhoto();
                stMember.byLevel = user->GetLevel();
                stMember.dwMemberID = user->GetCID();
                stMember.nChannel = static_cast<std::int16_t>(user->GetChannel());
                stMember.nHP = user->GetHP();
                stMember.nMapID = static_cast<std::int16_t>(user->GetMapID());
                stMember.nMaxHP = user->GetMaxHP();
                const std::wstring name = user->GetName();
                std::wcsncpy(stMember.strName, name.c_str(), 20);
                stMember.strName[20] = L'\0';
                stMember.uxMapID = user->GetMapIns();
                stInfo.stMemberList.vecInfo.push_back(stMember);
            }
        } else {
            // Force存在，使用Force的成员列表（ST_FORCE_MEMBER与ST_PARTY_MEMBER布局相同）
            std::vector<ST_FORCE_MEMBER> vecForceMembers;
            pForce->GetForceMemberList(vecForceMembers);
            for (const auto& member : vecForceMembers) {
                ST_PARTY_MEMBER stMember{};
                stMember.dwMemberID = member.dwMemberID;
                std::wcsncpy(stMember.strName, member.strName, 20);
                stMember.strName[20] = L'\0';
                stMember.byLevel = member.byLevel;
                stMember.byClass = member.byClass;
                stMember.byAwaken = member.byAwaken;
                stMember.dwProfilePhotoID = member.dwProfilePhotoID;
                stMember.nMapID = member.nMapID;
                stMember.nChannel = member.nChannel;
                stMember.nMaxHP = member.nMaxHP;
                stMember.nHP = member.nHP;
                stMember.bLogin = member.bLogin;
                stMember.uxMapID = member.uxMapID;
                stInfo.stMemberList.vecInfo.push_back(stMember);
            }
        }
    } else {
        stInfo.nResult = 0;
    }
}

int CPartyRecruit::GetApplyCount() const {
    return std::count_if(std::begin(m_stApplicantList.stInfo),
                         std::end(m_stApplicantList.stInfo),
                         [](const ST_APPLY_MEMBER& member) { return member.stMember.dwMemberID != 0; });
}

bool CPartyRecruit::IsApplied(std::uint32_t dwActorID) const {
    return std::any_of(std::begin(m_stApplicantList.stInfo),
                       std::end(m_stApplicantList.stInfo),
                       [dwActorID](const ST_APPLY_MEMBER& member) {
                           return member.stMember.dwMemberID == dwActorID;
                       });
}

std::uint8_t CPartyRecruit::RecruitApply(ST_APPLY_MEMBER& stApply) {
    const std::uint32_t masterActorID = GetMasterID();
    const std::shared_ptr<CUserObject> masterUser = TXSingleton<XRelayServer>::Instance()->GetUser(masterActorID);
    if (!masterUser) {
        return 1;
    }

    const std::int64_t currentDate = GreenDamTan_GetCurDateSec();
    if (m_biRecruitDate < currentDate) {
        return 1;
    }

    if (m_stPartyRecruit.shMinLevel > static_cast<std::int16_t>(stApply.stMember.byLevel) ||
        m_stPartyRecruit.shMaxLevel < static_cast<std::int16_t>(stApply.stMember.byLevel)) {
        return 7;
    }

    int emptyIndex = -1;
    for (int index = 0; index < 10; ++index) {
        if (m_stApplicantList.stInfo[index].stMember.dwMemberID == stApply.stMember.dwMemberID) {
            return 2;
        }
        if (m_stApplicantList.stInfo[index].stMember.dwMemberID == 0) {
            emptyIndex = index;
            break;
        }
    }

    if (emptyIndex == -1) {
        return 6;
    }

    m_stApplicantList.stInfo[emptyIndex] = stApply;
    if (m_biRecruitDate <= currentDate) {
        stApply.nRegDate = 0;
    } else {
        stApply.nRegDate = static_cast<int>(m_biRecruitDate - currentDate);
    }

    XSendPacket packet(0xF4u, 0x31u);
    packet.XParse << masterActorID;
    packet << stApply;
    TXSingleton<XRelayServer>::Instance()->SendPacket(masterUser->GetServerID(), packet);
    return 0;
}

bool CPartyRecruit::RecruitAccept(CServer* pServer, std::uint32_t dwAcceptID) {
    if (!pServer) {
        return false;
    }

    const std::int64_t currentDate = GreenDamTan_GetCurDateSec();
    if (m_biRecruitDate < currentDate) {
        return false;
    }

    int applyIndex = -1;
    for (int index = 0; index < 10; ++index) {
        if (m_stApplicantList.stInfo[index].stMember.dwMemberID == dwAcceptID) {
            applyIndex = index;
            break;
        }
    }
    if (applyIndex == -1) {
        return false;
    }

    const std::shared_ptr<CUserPartyInfo> partyUser = TXSingleton<XRelayServer>::Instance()->GetPartyUser(dwAcceptID);
    const std::shared_ptr<CUserObject> user = TXSingleton<XRelayServer>::Instance()->GetUser(dwAcceptID);
    if (!partyUser || !user) {
        DelApplyMember(dwAcceptID, false);
        return false;
    }

    const std::uint32_t serverID = user->GetServerID();
    CServer* memberServer = TXSingleton<XRelayServer>::Instance()->GetServer(serverID);
    if (!memberServer) {
        DelApplyMember(dwAcceptID, false);
        return false;
    }

    if (m_stPartyRecruit.byPartyGroupType == 1 && static_cast<int>(m_setMember.size()) >= 4) {
        partyUser->DelPartyRecruit(m_stPartyRecruit.dwRecruitID, false);
        DelApplyMember(dwAcceptID, false);
        return false;
    }
    if (m_stPartyRecruit.byPartyGroupType == 2 && static_cast<int>(m_setMember.size()) >= 8) {
        partyUser->DelPartyRecruit(m_stPartyRecruit.dwRecruitID, false);
        DelApplyMember(dwAcceptID, false);
        return false;
    }

    PS_SERVER_PARTY_RECRUIT_APPLY_ACCEPT_CHECK checkInfo{};
    checkInfo.dwRecruitID = m_stPartyRecruit.dwRecruitID;
    checkInfo.byPartyGroupType = m_stPartyRecruit.byPartyGroupType;
    checkInfo.dwServerID = pServer->GetServerID();
    checkInfo.stMember.stMember.dwMemberID = user->GetCID();
    const std::wstring name = user->GetName();
    std::wcsncpy(checkInfo.stMember.stMember.strName, name.c_str(), 20);
    checkInfo.stMember.stMember.strName[20] = L'\0';
    checkInfo.stMember.stMember.byLevel = user->GetLevel();
    checkInfo.stMember.stMember.byClass = m_stApplicantList.stInfo[applyIndex].stMember.byClass;
    checkInfo.stMember.stMember.byAwaken = m_stApplicantList.stInfo[applyIndex].stMember.byAwaken;
    checkInfo.stMember.stMember.dwProfilePhotoID = m_stApplicantList.stInfo[applyIndex].stMember.dwProfilePhotoID;
    checkInfo.stMember.stMember.nMapID = user->GetMapID();
    checkInfo.stMember.stMember.nChannel = user->GetChannel();
    checkInfo.stMember.stMember.nMaxHP = 1;
    checkInfo.stMember.stMember.nHP = 1;
    checkInfo.stMember.stMember.bLogin = true;
    checkInfo.stMember.stMember.uxMapID = user->GetMapIns();
    checkInfo.stMember.dwPartyID = m_stPartyRecruit.dwPartyID;

    XSendPacket packet(0xF4u, 0x32u);
    packet << checkInfo;
    memberServer->SendEx(packet);
    return true;
}

void CPartyRecruit::SetRecruitInfo(std::uint32_t dwRecruitID, std::uint32_t dwMasterID, const ST_PARTY_RECRUIT& stRecruit) {
    m_setMember.clear();
    m_stPartyRecruit = stRecruit;
    m_stPartyRecruit.dwRecruitID = dwRecruitID;
    m_stPartyRecruit.dwMasterUCID = dwMasterID;
    SetRecruitDate();
}

void CPartyRecruit::AddMember(std::uint32_t dwActorID) {
    if (dwActorID != 0) {
        m_setMember.insert(dwActorID);
        m_stPartyRecruit.byUserCount = static_cast<std::uint8_t>(m_setMember.size());
    }
}

void CPartyRecruit::RemoveMember(std::uint32_t dwActorID) {
    m_setMember.erase(dwActorID);
    m_stPartyRecruit.byUserCount = static_cast<std::uint8_t>(m_setMember.size());
}

void CPartyRecruit::DelApplyMember(std::uint32_t dwActorID, bool bSend) {
    for (ST_APPLY_MEMBER& member : m_stApplicantList.stInfo) {
        if (member.stMember.dwMemberID != dwActorID) {
            continue;
        }

        if (const std::shared_ptr<CUserObject> masterUser = TXSingleton<XRelayServer>::Instance()->GetUser(GetMasterID())) {
            XSendPacket packet(0xF4u, 0x2Fu);
            packet.XParse << GetMasterID();
            packet.XParse << dwActorID;
            packet.XParse << m_stPartyRecruit.dwRecruitID;
            TXSingleton<XRelayServer>::Instance()->SendPacket(masterUser->GetServerID(), packet);
        }

        if (bSend) {
            const std::shared_ptr<CUserObject> user = TXSingleton<XRelayServer>::Instance()->GetUser(dwActorID);
            const std::shared_ptr<CUserPartyInfo> partyUser = TXSingleton<XRelayServer>::Instance()->GetPartyUser(dwActorID);
            if (user && partyUser) {
                partyUser->ClearApplyParty(m_stPartyRecruit.dwRecruitID);

                XSendPacket packet(0xF4u, 0x2Fu);
                packet.XParse << dwActorID;
                packet.XParse << dwActorID;
                packet.XParse << m_stPartyRecruit.dwRecruitID;
                TXSingleton<XRelayServer>::Instance()->SendPacket(user->GetServerID(), packet);
            }
        }

        member = {};
        return;
    }
}

void CUserPartyInfo::DelPartyRecruit(std::uint32_t dwRecruitID, bool bPartySend) {
    for (std::size_t index = 0; index < std::size(m_dwApplyRecruitID); ++index) {
        if (m_dwApplyRecruitID[index] != dwRecruitID) {
            continue;
        }

        if (bPartySend) {
            if (const std::shared_ptr<CPartyRecruit> recruit =
                    TXSingleton<XRelayServer>::Instance()->GetPartyMatchingMgr().FindRecruitPtr(m_dwApplyRecruitID[index])) {
                recruit->DelApplyMember(m_dwActorID, false);
            }
        }

        m_dwApplyRecruitID[index] = 0;
        break;
    }

    // 无论是否找到匹配项，都发送包
    XSendPacket packet(0xF4u, 0x2Fu);
    packet.XParse << m_dwActorID;
    packet.XParse << m_dwActorID;
    packet.XParse << dwRecruitID;
    TXSingleton<XRelayServer>::Instance()->SendPacket(m_dwServerID, packet);
}

void CPartyRecruit::SendApplyUserList(CServer* pServer, std::uint32_t dwActorID) const {
    if (!pServer || GetMasterID() != dwActorID) {
        return;
    }

    ST_APPLY_MEMBER_LIST sendInfo = m_stApplicantList;
    const std::uint32_t currentTime = static_cast<std::uint32_t>(GreenDamTan_GetCurDateSec());
    for (ST_APPLY_MEMBER& member : sendInfo.stInfo) {
        if (member.nRegDate != 0) {
            member.nRegDate -= static_cast<int>(currentTime);
        }
    }

    XSendPacket sendPacket(0xF4u, 0x2Du);
    sendPacket.XParse << dwActorID;
    sendPacket << sendInfo;
    pServer->SendEx(sendPacket);
}
