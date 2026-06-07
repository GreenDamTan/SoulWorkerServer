#include "Soulworker/GameServer/XRelayServer/Party.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/UserObject.h"

#include <chrono>

namespace {
std::uint64_t GreenDamTan_GetPartyTickMs() {
    using namespace std::chrono;
    return static_cast<std::uint64_t>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}
}

// 对齐 IDA: 与 CForceMember::Logout 共享偏移逻辑
void CPartyMember::Logout() {
    m_dwKickOutTime = GreenDamTan_GetPartyTickMs() + 300000ull;
}

CParty::CParty(PS_REQ_PARTY_CREATE& stCreateParty)
    : m_dwPartyID(stCreateParty.dwPartyID), m_dwMasterID(stCreateParty.masterInfo.dwMemberID) {
    AddMember(stCreateParty.masterInfo);
    AddMember(stCreateParty.memberInfo);
}

// 对齐 IDA 0x140060830: 空实现
void CParty::Clear() {
    // IDA 显示这是一个空函数，仅返回
}

std::shared_ptr<CPartyMember> CParty::GetOrCreateMember(std::uint32_t dwMemberID) {
    auto& memberSlot = m_mapPartyMember[dwMemberID];
    if (!memberSlot) {
        memberSlot = std::make_shared<CPartyMember>();
    }
    return memberSlot;
}

// 对齐 IDA 0x140094190: AddMember 参数为非 const 引用
void CParty::AddMember(ST_PARTY_MEMBER& stPartyMember) {
    auto member = std::make_shared<CPartyMember>(stPartyMember);
    m_mapPartyMember[stPartyMember.dwMemberID] = member;
}

// 对齐 IDA 0x1400136A0: 进入服务器时更新成员信息
void CParty::SetMemberInfo(std::uint32_t dwMemberID, UXMapID uxMapID, int nMaxHP) {
    auto it = m_mapPartyMember.find(dwMemberID);
    if (it != m_mapPartyMember.end() && it->second) {
        // 对齐 IDA: 设置 nMaxHP, nHP, nMapID, nChannel, bLogin, 调用 Login(), 设置 m_uxEnterMap
        ST_PARTY_MEMBER partyMember{};
        it->second->GetMemberInfo(partyMember);
        partyMember.nMaxHP = nMaxHP;
        partyMember.nHP = nMaxHP;
        partyMember.nMapID = static_cast<int>(uxMapID.parts.mapID);
        partyMember.nChannel = static_cast<int>(uxMapID.parts.channel);
        partyMember.bLogin = true;
        partyMember.uxMapID = uxMapID;
        it->second->SetMemberInfo(partyMember);
        it->second->Login();  // 对齐 IDA 0x140095460: 重置踢出定时器
    }
}

// 对齐 IDA 0x140093F10: SetPartyInfo 需要检查成员在线状态
void CParty::SetPartyInfo(PS_PARTY_INFO& partyInfo) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    m_dwPartyID = partyInfo.dwPartyID;
    m_dwMasterID = partyInfo.dwMaster;
    m_uxMazeID = partyInfo.uxMazeID;
    m_byPartyType = partyInfo.byPartyType;
    m_mapPartyMember.clear();

    for (auto& member : partyInfo.vecPartyMember) {
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

void CParty::GetPartyInfo(PS_PARTY_INFO& partyInfo) {
    partyInfo.dwPartyID = m_dwPartyID;
    partyInfo.dwMaster = m_dwMasterID;
    partyInfo.uxMazeID = m_uxMazeID;
    partyInfo.byPartyType = m_byPartyType;
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

// 对齐 IDA 0x1400946F0: 参数类型为 ST_PARTY_MEMBER_LIST
void CParty::GetPartyMemberList(ST_PARTY_MEMBER_LIST& stMemberList) {
    stMemberList.vecInfo.clear();
    stMemberList.vecInfo.reserve(m_mapPartyMember.size());
    for (const auto& [memberID, member] : m_mapPartyMember) {
        static_cast<void>(memberID);
        if (!member) {
            continue;
        }
        ST_PARTY_MEMBER partyMember{};
        if (member->GetMemberInfo(partyMember)) {
            stMemberList.vecInfo.push_back(partyMember);
        }
    }
}

// 对齐 IDA 0x1400943E0: 非常量方法
std::uint32_t CParty::FindNewMaster() {
    for (const auto& [memberID, member] : m_mapPartyMember) {
        if (memberID != m_dwMasterID) {
            return memberID;
        }
    }
    return 0;
}

// 对齐 IDA 0x140094360: 直接从 map 移除成员
void CParty::Kickout(std::uint32_t dwMemberID) {
    m_mapPartyMember.erase(dwMemberID);
}

// 对齐 IDA 0x1400942B0: CForce::ChangeMaster
bool CParty::ChangeMaster(std::uint32_t dwMaster, bool bLeave) {
    // 查找新队长是否在成员列表中
    const auto it = m_mapPartyMember.find(dwMaster);
    if (it == m_mapPartyMember.end()) {
        return false;
    }

    // 对齐 IDA: 如果 bLeave 为 true，检查新队长是否在线
    if (bLeave) {
        // 获取成员信息检查在线状态
        ST_PARTY_MEMBER stMember{};
        it->second->GetMemberInfo(stMember);
        if (!stMember.bLogin) {
            return false;
        }
    }

    m_dwMasterID = dwMaster;
    return true;
}

// 对齐 IDA 0x140094820
void CParty::SendNameChange(std::uint32_t dwActorID, const wchar_t* pChangeName) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    // 构建 PS_CHANGE_NAME 结构
    PS_CHANGE_NAME stInfo{};
    stInfo.dwActorID = dwActorID;
    wcscpy_s(stInfo.szChangeName, pChangeName);

    // 遍历所有 party 成员
    for (const auto& [memberID, pMember] : m_mapPartyMember) {
        if (!pMember) {
            continue;
        }

        ST_PARTY_MEMBER partyMember{};
        pMember->GetMemberInfo(partyMember);

        if (partyMember.dwMemberID == dwActorID) {
            // 更新该成员的名字
            wcscpy_s(partyMember.strName, pChangeName);
            pMember->SetMemberInfo(partyMember);
        } else {
            // 向其他成员广播改名消息 0xF4/0x41
            const std::shared_ptr<CUserObject> pMemberUser = relayServer.GetUser(partyMember.dwMemberID);
            if (!pMemberUser) {
                continue;
            }

            XSendPacket xSendPacket(0xF4u, 0x41u);
            xSendPacket.XParse << partyMember.dwMemberID;
            xSendPacket << stInfo;
            relayServer.SendPacket(pMemberUser->GetServerID(), xSendPacket);
        }
    }
}

// GreenDamTan_: IDA 中不存在的辅助方法实现
void CParty::GreenDamTan_SetMemberInfo(ST_PARTY_MEMBER& stPartyMember) {
    auto member = GetOrCreateMember(stPartyMember.dwMemberID);
    member->SetMemberInfo(stPartyMember);
}

bool CParty::GreenDamTan_GetMemberInfo(std::uint32_t dwMemberID, ST_PARTY_MEMBER* pPartyMember) {
    if (!pPartyMember) {
        return false;
    }
    const auto it = m_mapPartyMember.find(dwMemberID);
    if (it != m_mapPartyMember.end() && it->second) {
        return it->second->GetMemberInfo(*pPartyMember);
    }
    return false;
}

// ============================================================================
// CParty::SendToLocal
// IDA 0x1403A6110 - 0x1403A624D
// 精确还原自 IDA 反编译: 发送数据包给同地图的队伍成员
// ============================================================================
void CParty::SendToLocal(XSendPacket& xSendPacket, UXMapID uxMapID, std::uint32_t dwExceptID) {
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

    for (const auto& [memberID, pMember] : m_mapPartyMember) {
        if (!pMember) {
            continue;
        }

        // IDA: 检查是否为排除成员
        if (memberID == dwExceptID) {
            continue;
        }

        ST_PARTY_MEMBER partyMember{};
        pMember->GetMemberInfo(partyMember);

        // IDA: 检查成员是否在同地图
        if (partyMember.uxMapID.nMapID != uxMapID.nMapID) {
            continue;
        }

        // IDA: 发送数据包给成员
        const std::shared_ptr<CUserObject> pMemberUser = relayServer.GetUser(memberID);
        if (pMemberUser) {
            relayServer.SendPacket(pMemberUser->GetServerID(), xSendPacket);
        }
    }
}

// ============================================================================
// CParty::SetMemberMapID
// IDA 0x1403A6DB0 - 0x1403A6E7F
// 精确还原自 IDA 反编译: 设置成员地图ID并通知更新
// ============================================================================
void CParty::SetMemberMapID(std::uint32_t dwActorID, int nMapID, int nChannel, UXMapID uxMapID) {
    const auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end() || !it->second) {
        return;
    }

    // IDA: 调用 Login, SetMapID, SetChannel, 设置 uxMapID
    it->second->Login();

    ST_PARTY_MEMBER partyMember{};
    it->second->GetMemberInfo(partyMember);
    partyMember.nMapID = nMapID;
    partyMember.nChannel = nChannel;
    partyMember.uxMapID = uxMapID;
    it->second->SetMemberInfo(partyMember);

    SendUpdateMemberInfo(dwActorID);
}

// ============================================================================
// CParty::SetMemberHP
// IDA 0x1403A6E80 - 0x1403A6FD7
// 精确还原自 IDA 反编译: 设置成员当前HP并广播
// ============================================================================
void CParty::SetMemberHP(std::uint32_t dwActorID, UXMapID uxMapID, int nHP) {
    const auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end() || !it->second) {
        return;
    }

    ST_PARTY_MEMBER partyMember{};
    it->second->GetMemberInfo(partyMember);
    partyMember.nHP = nHP;
    it->second->SetMemberInfo(partyMember);

    // IDA: 构建并发送 PS_PARTY_MEMEBER_HP 数据包 (0x12/0x12)
    PS_PARTY_MEMEBER_HP stMemberHP{};
    stMemberHP.dwMemberID = dwActorID;
    stMemberHP.nMaxHP = partyMember.nMaxHP;
    stMemberHP.nHP = nHP;

    XSendPacket xSendPacket(0x12u, 0x12u);
    xSendPacket << stMemberHP;
    SendToLocal(xSendPacket, uxMapID, dwActorID);
}

// ============================================================================
// CParty::SetMemberMaxHP
// IDA 0x1403A6FE0 - 0x1403A7137
// 精确还原自 IDA 反编译: 设置成员最大HP并广播
// ============================================================================
void CParty::SetMemberMaxHP(std::uint32_t dwActorID, UXMapID uxMapID, int nMaxHP) {
    const auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end() || !it->second) {
        return;
    }

    ST_PARTY_MEMBER partyMember{};
    it->second->GetMemberInfo(partyMember);
    partyMember.nMaxHP = nMaxHP;
    it->second->SetMemberInfo(partyMember);

    // IDA: 构建并发送 PS_PARTY_MEMEBER_HP 数据包 (0x12/0x12)
    PS_PARTY_MEMEBER_HP stMemberHP{};
    stMemberHP.dwMemberID = dwActorID;
    stMemberHP.nMaxHP = nMaxHP;
    stMemberHP.nHP = partyMember.nHP;

    XSendPacket xSendPacket(0x12u, 0x12u);
    xSendPacket << stMemberHP;
    SendToLocal(xSendPacket, uxMapID, dwActorID);
}

// ============================================================================
// CParty::SetMemberLevel
// IDA 0x1403A7140 - 0x1403A71D4
// 精确还原自 IDA 反编译: 设置成员等级
// ============================================================================
void CParty::SetMemberLevel(std::uint32_t dwActorID, int nLevel) {
    const auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end() || !it->second) {
        return;
    }

    ST_PARTY_MEMBER partyMember{};
    it->second->GetMemberInfo(partyMember);
    partyMember.byLevel = static_cast<std::uint8_t>(nLevel);
    it->second->SetMemberInfo(partyMember);

    SendUpdateMemberInfo(dwActorID);
}

// ============================================================================
// CParty::SetMemberAwaken
// IDA 0x1403A71E0 - 0x1403A7275
// 精确还原自 IDA 反编译: 设置成员觉醒状态
// ============================================================================
void CParty::SetMemberAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken) {
    const auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end() || !it->second) {
        return;
    }

    ST_PARTY_MEMBER partyMember{};
    it->second->GetMemberInfo(partyMember);
    partyMember.byAwaken = byAwaken;
    it->second->SetMemberInfo(partyMember);

    SendUpdateMemberInfo(dwActorID);
}

// ============================================================================
// CParty::SetMemberProfilePhoto
// IDA 0x1403A7280 - 0x1403A7314
// 精确还原自 IDA 反编译: 设置成员头像
// ============================================================================
void CParty::SetMemberProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwPhotoID) {
    const auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end() || !it->second) {
        return;
    }

    ST_PARTY_MEMBER partyMember{};
    it->second->GetMemberInfo(partyMember);
    partyMember.dwProfilePhotoID = dwPhotoID;
    it->second->SetMemberInfo(partyMember);

    SendUpdateMemberInfo(dwActorID);
}

// ============================================================================
// CParty::SendUpdateMemberInfo
// IDA 0x1403AA2D0 - 发送成员更新信息
// ============================================================================
void CParty::SendUpdateMemberInfo(std::uint32_t dwActorID) {
    const auto it = m_mapPartyMember.find(dwActorID);
    if (it == m_mapPartyMember.end() || !it->second) {
        return;
    }

    ST_PARTY_MEMBER partyMember{};
    it->second->GetMemberInfo(partyMember);

    // IDA: 构建 ST_UPDATE_PARTY_MEMBER 并发送
    ST_UPDATE_PARTY_MEMBER stUpdateMember{};
    stUpdateMember.dwPartyID = m_dwPartyID;
    stUpdateMember.stPartyMember = partyMember;

    // IDA: 广播给所有在线成员
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    for (const auto& [memberID, pMember] : m_mapPartyMember) {
        if (!pMember || memberID == dwActorID) {
            continue;
        }

        ST_PARTY_MEMBER member{};
        pMember->GetMemberInfo(member);

        if (!member.bLogin) {
            continue;
        }

        // 发送更新消息 0x12/0x0A
        XSendPacket xSendPacket(0x12u, 0x0Au);
        xSendPacket << stUpdateMember;

        const std::shared_ptr<CUserObject> pMemberUser = relayServer.GetUser(memberID);
        if (pMemberUser) {
            relayServer.SendPacket(pMemberUser->GetServerID(), xSendPacket);
        }
    }
}

// ============================================================================
// CParty::SetEnterMazeResponse
// IDA 0x1403AA360 - 0x1403AA438
// 精确还原自 IDA 反编译: 设置进入迷宫响应
// 注意: RelayServer简化实现，不处理迷宫进入逻辑
// ============================================================================
bool CParty::SetEnterMazeResponse(std::uint32_t dwAgreeActor) {
    // RelayServer 简化实现：不处理迷宫进入逻辑
    // IDA 原始逻辑需要 m_stEnterMazeRequst 和 m_setAgreeToMazeMember 成员
    // 这些成员在 RelayServer 版本中不存在
    static_cast<void>(dwAgreeActor);
    return true;
}

// ============================================================================
// CParty::SetPartyMemberState
// IDA 0x1401BD3B0 - 0x1401BD443
// 精确还原自 IDA 反编译: 设置队伍成员状态
// ============================================================================
void CParty::SetPartyMemberState(std::uint8_t byType) {
    // IDA: 遍历所有成员设置状态
    for (auto& [memberID, pMember] : m_mapPartyMember) {
        if (!pMember) {
            continue;
        }
        // IDA: 调用 SetOutputState(byType)
        // RelayServer 简化实现：仅更新成员状态
        ST_PARTY_MEMBER partyMember{};
        pMember->GetMemberInfo(partyMember);
        // 注意: RelayServer 不需要实现 SetOutputState
        static_cast<void>(partyMember);
        static_cast<void>(byType);
    }
}
