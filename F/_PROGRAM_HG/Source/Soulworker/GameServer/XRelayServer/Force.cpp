#include "Soulworker/GameServer/XRelayServer/Force.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/UserObject.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

// 前置声明 - 用于RelayServer的Send函数
class CUser;

// 前置声明 - 来自 XGameServer
class ThreadLocalData;
class XArea;
class CGocForce;
class CGocParty;


// ============================================================================
// CForce::Create
// IDA 0x1401B63E0 - 设置Force ID和Master ID
// ============================================================================
void CForce::Create(std::uint32_t dwPartyID, std::uint32_t dwMasterID) {
    m_dwForceID = dwPartyID;
    m_dwMasterID = dwMasterID;
}

// ============================================================================
// CForceMember::Init
// IDA 0x1401C9FD0 - 0x1401CA096
// 精确还原自 IDA 反编译:
//   1. 检查 nMaxHP < 0 时设置 bLogin = 0
//   2. 如果 bLogin 为 false，清零 HP 和 MaxHP
//   3. 复制 ST_FORCE_MEMBER 到 m_stForceMember
//   4. 复制名称字符串
// ============================================================================
void CForceMember::Init(ST_FORCE_MEMBER& stMember) {
    // IDA: if (stMember->nMaxHP < 0) stMember->bLogin = 0;
    if (stMember.nMaxHP < 0) {
        stMember.bLogin = 0;
    }

    // IDA: if (!stMember->bLogin) { stMember->nHP = 0; stMember->nMaxHP = 0; }
    if (!stMember.bLogin) {
        stMember.nHP = 0;
        stMember.nMaxHP = 0;
    }

    // IDA: ST_PARTY_MEMBER::operator=(&this->m_partyMemberInfo, stMember);
    m_stForceMember = stMember;

    // IDA: 复制名称字符串 (wcscpy 逻辑)
    // 名称已经在 operator= 中复制
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
    // 对齐 IDA 0x1400147A0: 使用 GetTickCount64() + 300000 (5分钟)
    m_dwKickOutTime = GetTickCount64() + 300000ull;
}

bool CForceMember::GetMemberInfo(ST_FORCE_MEMBER& forceMember) const {
    forceMember = m_stForceMember;
    return true;
}

// 对齐 IDA 0x1401CA0A0: CForceMember::Clear
// IDA 反编译精确还原 (GameServer版本):
//   1. 检查 m_pMember 是否存在
//   2. 获取 m_pMember 的 MapInsID
//   3. 检查 ThreadLocalData::FindArea
//   4. 获取 CGocForce 组件并调用 Clear
//   5. 设置 m_pMember = nullptr
//
// RelayServer 版本说明:
//   RelayServer 不包含 ThreadLocalData/CGocForce 等 GameServer 组件
//   因此 RelayServer 版本简化为仅清除成员指针
//   GameServer 版本的完整实现在 XGameServer/Force.cpp 中
void CForceMember::Clear() {
    // IDA: m_pMember = nullptr (最终操作)
    // RelayServer 简化实现: 仅清除成员指针
    m_pMember = nullptr;
}

// ============================================================================
// CForceMember::SetMapID
// IDA 0x1401CA1C0 - 0x1401CA1D6
// 精确还原自 IDA 反编译: 设置 m_stForceMember.nMapID
// ============================================================================
void CForceMember::SetMapID(int nMapID) {
    m_stForceMember.nMapID = nMapID;
}

// ============================================================================
// CForceMember::SetMaxHP
// IDA 0x1401CA1E0 - 0x1401CA1F6
// 精确还原自 IDA 反编译: 设置 m_stForceMember.nMaxHP
// ============================================================================
void CForceMember::SetMaxHP(int nHP) {
    m_stForceMember.nMaxHP = nHP;
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

// ============================================================================
// CForce::SetForceMemberInfo
// IDA 0x1401B7D80 - 0x1401B7DF4
// 精确还原自 IDA 反编译: 设置指定成员的Force信息
// ============================================================================
void CForce::SetForceMemberInfo(std::uint32_t dwActorID, ST_FORCE_MEMBER& stMemberInfo) {
    const auto it = m_mapForceMember.find(dwActorID);
    if (it != m_mapForceMember.end() && it->second) {
        it->second->Init(stMemberInfo);
    }
}

// ============================================================================
// CForce::SetMemberMapID
// IDA 0x1401B7E80 - 0x1401B7F4F
// 精确还原自 IDA 反编译: 设置成员地图ID、频道并通知更新
// ============================================================================
void CForce::SetMemberMapID(std::uint32_t dwActorID, int nMapID, int nChannel, UXMapID uxMapID) {
    const auto it = m_mapForceMember.find(dwActorID);
    if (it != m_mapForceMember.end() && it->second) {
        it->second->Login();
        it->second->SetMapID(nMapID);
        // IDA: 设置频道和UXMapID
        ST_FORCE_MEMBER forceMember{};
        it->second->GetMemberInfo(forceMember);
        forceMember.nChannel = nChannel;
        forceMember.uxMapID = uxMapID;
        it->second->SetMemberInfo(forceMember);
        SendUpdateMemberInfo(dwActorID);
    }
}

// ============================================================================
// CForce::SetMemberHP
// IDA 0x1401B7F50 - 0x1401B80A7
// 精确还原自 IDA 反编译: 设置成员当前HP并广播
// ============================================================================
void CForce::SetMemberHP(std::uint32_t dwActorID, UXMapID uxMapID, int nHP) {
    const auto it = m_mapForceMember.find(dwActorID);
    if (it == m_mapForceMember.end() || !it->second) {
        return;
    }

    const std::shared_ptr<CForceMember> member = it->second;
    ST_FORCE_MEMBER forceMember{};
    member->GetMemberInfo(forceMember);

    // IDA: 设置HP并保留MaxHP
    forceMember.nHP = nHP;
    member->SetMemberInfo(forceMember);

    // IDA: 构建并发送 PS_FORCE_MEMEBER_HP 数据包 (0x2E/0x12)
    PS_FORCE_MEMEBER_HP stMemberHP{};
    stMemberHP.dwMemberID = dwActorID;
    stMemberHP.nMaxHP = forceMember.nMaxHP;
    stMemberHP.nHP = nHP;

    XSendPacket xSendPacket(0x2Eu, 0x12u);
    xSendPacket << stMemberHP;
    SendToLocal(xSendPacket, uxMapID, dwActorID);
}

// ============================================================================
// CForce::SetMemberMaxHP
// IDA 0x1401B80B0 - 0x1401B8207
// 精确还原自 IDA 反编译: 设置成员最大HP并广播
// ============================================================================
void CForce::SetMemberMaxHP(std::uint32_t dwActorID, UXMapID uxMapID, int nMaxHP) {
    const auto it = m_mapForceMember.find(dwActorID);
    if (it == m_mapForceMember.end() || !it->second) {
        return;
    }

    const std::shared_ptr<CForceMember> member = it->second;
    ST_FORCE_MEMBER forceMember{};
    member->GetMemberInfo(forceMember);

    // IDA: 设置MaxHP
    forceMember.nMaxHP = nMaxHP;
    member->SetMemberInfo(forceMember);

    // IDA: 构建并发送 PS_FORCE_MEMEBER_HP 数据包 (0x2E/0x12)
    PS_FORCE_MEMEBER_HP stMemberHP{};
    stMemberHP.dwMemberID = dwActorID;
    stMemberHP.nMaxHP = nMaxHP;
    stMemberHP.nHP = forceMember.nHP;

    XSendPacket xSendPacket(0x2Eu, 0x12u);
    xSendPacket << stMemberHP;
    SendToLocal(xSendPacket, uxMapID, dwActorID);
}

// ============================================================================
// CForce::SetMemberAwaken
// IDA 0x1401B82B0 - 0x1401B8345
// 精确还原自 IDA 反编译: 设置成员觉醒状态并通知更新
// ============================================================================
void CForce::SetMemberAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken) {
    const auto it = m_mapForceMember.find(dwActorID);
    if (it == m_mapForceMember.end() || !it->second) {
        return;
    }

    const std::shared_ptr<CForceMember> member = it->second;
    ST_FORCE_MEMBER forceMember{};
    member->GetMemberInfo(forceMember);
    forceMember.byAwaken = byAwaken;
    member->SetMemberInfo(forceMember);

    // IDA: 调用 SendUpdateMemberInfo
    SendUpdateMemberInfo(dwActorID);
}

// ============================================================================
// CForce::SetMemberProfilePhoto
// IDA 0x1401B8350 - 0x1401B83E4
// 精确还原自 IDA 反编译: 设置成员头像
// ============================================================================
void CForce::SetMemberProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwPhotoID) {
    const auto it = m_mapForceMember.find(dwActorID);
    if (it == m_mapForceMember.end() || !it->second) {
        return;
    }

    const std::shared_ptr<CForceMember> member = it->second;
    ST_FORCE_MEMBER forceMember{};
    member->GetMemberInfo(forceMember);
    forceMember.dwProfilePhotoID = dwPhotoID;
    member->SetMemberInfo(forceMember);

    SendUpdateMemberInfo(dwActorID);
}

// ============================================================================
// CForce::SetForceType
// IDA 0x1403AABF0 - 0x1403AAC93
// 精确还原自 IDA 反编译: 设置Force类型
// ============================================================================
void CForce::SetForceType(std::uint8_t byType) {
    // IDA: 如果 byType == 1，遍历成员设置状态
    if (byType == 1) {
        for (auto& [memberID, pMember] : m_mapForceMember) {
            if (!pMember) {
                continue;
            }
            // IDA: 调用 SetOutputState(2)
            ST_FORCE_MEMBER forceMember{};
            pMember->GetMemberInfo(forceMember);
            // 注意: RelayServer 简化实现，不调用 SetOutputState
            static_cast<void>(forceMember);
        }
    }
    m_byForceType = byType;
}

// ============================================================================
// CForce::SetEnterMazeResponse
// IDA 0x1401BB340 - 0x1401BB418
// 精确还原自 IDA 反编译: 设置进入迷宫响应
// 注意: RelayServer简化实现，不处理迷宫进入逻辑
// ============================================================================
bool CForce::SetEnterMazeResponse(std::uint32_t dwAgreeActor) {
    // RelayServer 简化实现：不处理迷宫进入逻辑
    // IDA 原始逻辑需要 m_stEnterMazeRequst 和 m_setAgreeToMazeMember 成员
    // 这些成员在 RelayServer 版本中不存在
    static_cast<void>(dwAgreeActor);
    return true;
}

// ============================================================================
// CForce::SetMazeRecode
// IDA 0x1403A6CB0 - 0x1403A6D24
// 精确还原自 IDA 反编译: 设置迷宫记录
// ============================================================================
void CForce::SetMazeRecode(std::uint32_t dwActorID, int* pMazeRecode) {
    const auto it = m_mapForceMember.find(dwActorID);
    if (it == m_mapForceMember.end() || !it->second) {
        return;
    }
    // IDA: 调用 CPartyMember::SetRecode
    // RelayServer 简化实现：不存储迷宫记录
    static_cast<void>(pMazeRecode);
}

// ============================================================================
// CForce::GetMazeRecode
// IDA 0x1403A6D30 - 0x1403A6DA4
// 精确还原自 IDA 反编译: 获取迷宫记录
// ============================================================================
void CForce::GetMazeRecode(std::uint32_t dwActorID, int* pMazeRecode) {
    const auto it = m_mapForceMember.find(dwActorID);
    if (it == m_mapForceMember.end() || !it->second) {
        return;
    }
    // IDA: 调用 CPartyMember::GetRecode
    // RelayServer 简化实现：不存储迷宫记录
    static_cast<void>(pMazeRecode);
}

// ============================================================================
// CForce::SendUpdateMemberInfo
// IDA 0x1401B83F0 - 0x1401B8527
// 精确还原自 IDA 反编译: 发送成员更新信息
// ============================================================================
void CForce::SendUpdateMemberInfo(std::uint32_t dwActorID) {
    const auto it = m_mapForceMember.find(dwActorID);
    if (it == m_mapForceMember.end() || !it->second) {
        return;
    }

    const std::shared_ptr<CForceMember> member = it->second;
    ST_FORCE_MEMBER stForceMember{};
    member->GetMemberInfo(stForceMember);

    // IDA: 构建 ST_UPDATE_FORCE_MEMBER 并发送
    ST_UPDATE_FORCE_MEMBER stUpdateForceMember{};
    stUpdateForceMember.dwForceID = m_dwForceID;
    stUpdateForceMember.stForceMember = stForceMember;

    // IDA: 通过 XGameServer::m_communitySocket 发送
    // RelayServer 简化实现：广播给所有在线成员
    XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
    for (const auto& [memberID, pMember] : m_mapForceMember) {
        if (!pMember || memberID == dwActorID) {
            continue;
        }

        ST_FORCE_MEMBER forceMember{};
        pMember->GetMemberInfo(forceMember);

        if (!forceMember.bLogin) {
            continue;
        }

        // 发送更新消息 0x2E/0x0A
        XSendPacket xSendPacket(0x2Eu, 0x0Au);
        xSendPacket << stUpdateForceMember;

        const std::shared_ptr<CUserObject> pMemberUser = relayServer.GetUser(memberID);
        if (pMemberUser) {
            relayServer.SendPacket(pMemberUser->GetServerID(), xSendPacket);
        }
    }
}

// ============================================================================
// CForce::OrderPlayPoint
// IDA 0x1401B72B0 - 0x1401B7656
// 精确还原自 IDA 反编译: 排序游戏点数并设置排名
// 注意: 这是GameServer版本的函数，RelayServer版本需要不同实现
// ============================================================================
void CForce::OrderPlayPoint() {
    // IDA 反编译分析:
    // 1. 遍历 m_mapForceMember，获取每个成员的 CGocRecode 组件
    // 2. 调用 CGocRecode::GetPlayPoint 获取游戏点数
    // 3. 将 <CGocRecode*, float> 对存入 map 并排序
    // 4. 按排序结果调用 CGocRecode::SetPlayPointOrder 设置排名
    //
    // RelayServer 简化实现: 不直接访问 CGocRecode
    // 此函数在 RelayServer 中不应被调用
    // TODO: GameServer 版本需要实现完整逻辑
}

// ============================================================================
// CForce::MazeReward
// IDA 0x1401B76A0 - 0x1401B7A2A
// 精确还原自 IDA 反编译: 迷宫奖励发放
// 注意: 这是GameServer版本的函数，RelayServer版本需要不同实现
// ============================================================================
int CForce::MazeReward(std::uint32_t dwPlayTime) {
    // IDA 反编译分析:
    // 1. 第一次遍历: 计算在同一迷宫的 Force 成员数量 (nForceCount)
    //    - 检查 ThreadLocalData::IsThreadArea
    //    - 检查 XArea::GetTBMapID 是否匹配 m_uxMazeID
    // 2. 第二次遍历: 调用 CGocRecode::MazeReward 发放奖励
    //    - 返回最高排名 nHighRank
    // 3. 调用 OrderPlayPoint() 排序游戏点数
    // 4. 调用 ApplyReward() 应用奖励
    // 5. 调用 SendMazeClear(0, a3) 发送迷宫清除通知
    //
    // RelayServer 简化实现: 不直接访问 CGocRecode 和 ThreadLocalData
    // 此函数在 RelayServer 中不应被调用
    // TODO: GameServer 版本需要实现完整逻辑
    OrderPlayPoint();
    ApplyReward();
    // SendMazeClear(0);  // CParty 方法
    return 0;
}

// ============================================================================
// CForce::ApplyReward
// IDA 0x1401B7A30 - 0x1401B7D5C
// 精确还原自 IDA 反编译: 应用奖励到所有成员
// 注意: 这是GameServer版本的函数，RelayServer版本需要不同实现
// ============================================================================
void CForce::ApplyReward() {
    // IDA 反编译分析:
    // 1. 遍历 m_mapForceMember
    // 2. 检查成员是否在同一迷宫 (UXMapID::operator!=)
    // 3. 获取 CGocRecode 组件，调用 CGocRecode::ApplyReward(stReward)
    // 4. 将 stReward 添加到 PS_REWARD_PARTY_SCORE_LIST
    // 5. 构建并发送 XSendPacket(0x11, 0x73) 数据包
    // 6. 第二次遍历: 发送数据包给同迷宫的成员
    //
    // RelayServer 简化实现: 不直接访问 CGocRecode
    // 此函数在 RelayServer 中不应被调用
    // TODO: GameServer 版本需要实现完整逻辑

    // 简化实现: 不发送数据包
    // PS_REWARD_PARTY_SCORE_LIST psForceReward;
    // XSendPacket xSendPacket(0x11u, 0x73u);
    // xSendPacket << psForceReward;
    // SendToLocal(xSendPacket, m_uxMazeID, 0);
}
