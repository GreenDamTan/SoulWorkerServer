// GocForce.cpp
// CGocForce implementation
// 对齐 IDA GameServer.exe

#include "GocForce.h"
#include "GOComponent.h"

// 前置声明 - 避免循环依赖
class CUser;
class CForce;
class CForceMember;
class CParty;
class XGameServer;

// ============================================================================
// CGocForce Implementation
// 基于 IDA GameServer.exe 反编译结果
// ============================================================================

// IDA: ??0CGocForce@@QEAA@XZ @ 0x140083060
// CGocForce *__fastcall CGocForce::CGocForce(CGocForce *this)
// {
//   GOComponent::GOComponent(this);
//   this->__vftable = (CGocForce_vtbl *)&CGocForce::`vftable';
//   std::tr1::shared_ptr<CForce>::shared_ptr<CForce>(&this->m_pForce, 0);
//   return this;
// }
CGocForce::CGocForce()
    : CGocParty()
    , m_byMatchingState(0) {
    // IDA: vtable is set after CGocParty construction
    // IDA: m_pParty (inherited) is initialized to nullptr via shared_ptr constructor in CGocParty
    // IDA: m_biMatchingDate (inherited) is initialized to 0 in CGocParty
}

// IDA: ??1CGocForce@@UEAA@XZ @ 0x1400830F0
// void __fastcall CGocForce::~CGocForce(CGocForce *this)
// {
//   this->__vftable = (CGocForce_vtbl *)&CGocForce::`vftable';
//   std::tr1::shared_ptr<CItemAkashic>::~shared_ptr<CItemAkashic>((std::tr1::shared_ptr<CGocNetwork> *)&this->m_pForce);
//   GOComponent::~GOComponent(this);
// }
CGocForce::~CGocForce() {
    // IDA: vtable is set to CGocForce vtable
    // IDA: m_pParty destructor is called in CGocParty destructor
    // IDA: CGocParty destructor is called automatically
}

// IDA: ?Init@CGocForce@@QEAAXXZ @ 0x140083140
// void __fastcall CGocForce::Init(CGocForce *this)
// {
//   this->m_biMatchingDate = 0;
//   this->m_byMatchingState = 0;
// }
void CGocForce::Init() {
    m_biMatchingDate = 0;
    m_byMatchingState = 0;
}

// IsForce - 检查是否在 Force 中
// IDA: 使用 CGocParty::IsParty @ 0x140091E20
bool CGocForce::IsForce() const {
    return IsParty();
}

// GetForceID - 获取 Force ID
// IDA: 使用 CGocParty::GetPartyID @ 0x14009F760
std::uint32_t CGocForce::GetForceID() const {
    return GetPartyID();
}

// IDA: ?IsFull@CGocForce@@QEAA_NXZ @ 0x1400854B0
// bool __fastcall CGocForce::IsFull(CGocForce *this)
// {
//   CParty *v1; // rax
//   bool result; // al
//
//   result = false;
//   if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*((std::tr1::shared_ptr<CItemEquip> *)&this->m_pForce) != -1 )
//   {
//     v1 = (CParty *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&this->m_pForce);
//     if ( (unsigned __int8)CParty::GetUserCount(v1) == 8 )
//       return true;
//   }
//   return result;
// }
bool CGocForce::IsFull() const {
    if (!IsParty()) {
        return false;
    }
    // IDA verified: GetUserCount() == 8 (exact check for Force max members)
    // TODO: 需要访问 CParty::GetUserCount()
    // return m_pParty ? (m_pParty->GetUserCount() == 8) : false;
    return false;
}

// IDA: ?IsMaster@CGocForce@@QEAA_NK@Z @ 0x140083160
// bool __fastcall CGocForce::IsMaster(CGocForce *this, unsigned int dwUCID)
// {
//   VisRenderCollection_cl *v2; // rax
//   bool result; // al
//
//   result = false;
//   if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*((std::tr1::shared_ptr<CItemEquip> *)&this->m_pForce) != -1 )
//   {
//     v2 = (VisRenderCollection_cl *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&this->m_pForce);
//     if ( (unsigned int)CWayPoint::GetCurID(v2) == dwUCID )
//       return true;
//   }
//   return result;
// }
bool CGocForce::IsMaster(std::uint32_t dwUCID) const {
    if (!IsParty()) {
        return false;
    }
    // IDA verified: CWayPoint::GetCurID maps to CForce::GetMasterID
    // TODO: 需要访问 CForce::GetMasterID()
    // return m_pParty ? (m_pParty->GetMasterID() == dwUCID) : false;
    return false;
}

// GetForce - 获取 Force 对象
std::shared_ptr<CForce> CGocForce::GetForce() const {
    // IDA: m_pParty is used as m_pForce (继承自 CGocParty)
    return std::static_pointer_cast<CForce>(m_pParty);
}

// IDA: ?SetForce@CGocForce@@QEAAXV?$shared_ptr@VCForce@@@tr1@std@@@Z @ 0x140083F30
// void __fastcall CGocForce::SetForce(CGocParty *this, std::tr1::shared_ptr<CDropItemGroup> *pParty)
// {
//   std::tr1::shared_ptr<CForce>::operator=((std::tr1::shared_ptr<CDropItemGroup> *)&this->m_pParty, pParty);
//   if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*((std::tr1::shared_ptr<CItemEquip> *)&this->m_pParty) != -1 )
//   {
//     // Get owner user and register party member
//     CParty::RegisterPartyMember(v3, dwActorID, pMember);
//   }
// }
void CGocForce::SetForce(std::shared_ptr<CForce> pForce) {
    // IDA: 使用 CGocParty::m_pParty 存储 CForce 指针
    m_pParty = std::static_pointer_cast<CParty>(pForce);
    // TODO: 如果 pForce 有效，需要注册成员
    // if (pForce) {
    //     // Get owner CMover/CUser and register as member
    //     // pForce->RegisterPartyMember(dwActorID, pMember);
    // }
}

// IDA: ?SendForceInfo@CGocForce@@QEAAXE@Z @ 0x140084310
// void __fastcall CGocForce::SendForceInfo(CGocForce *this, unsigned __int8 byUpdateType, float a3)
// IDA verified: 发送 PS_FORCE_INFO 给当前用户
void CGocForce::SendForceInfo(std::uint8_t byUpdateType) {
    // IDA: 检查 m_pParty 有效性
    if (!IsParty()) {
        return;
    }

    // TODO: 需要 PS_FORCE_INFO 结构和相关依赖
    // IDA 反编译的核心逻辑:
    // PS_FORCE_INFO stForceInfo;
    // m_pParty->GetPartyInfo(&stForceInfo);
    // stForceInfo.byUpdateType = byUpdateType;
    // stForceInfo.byForceType = CForce::GetForceType(m_pParty.get());
    // XSendPacket xSendPacket(0x2E, 9);  // main=46, sub=9
    // xSendPacket << stForceInfo;
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// IDA: ?IsMatchingDate@CGocForce@@QEAA_NXZ @ 0x140085160
// bool __fastcall CGocForce::IsMatchingDate(CGocForce *this)
// {
//   XGameServer *v2; // rax
//   __int64 v3; // [rsp+20h] [rbp-18h]
//
//   if ( !this->m_biMatchingDate )
//     return 0;
//   v3 = this->m_biMatchingDate + 180;
//   v2 = TXSingleton<XGameServer>::Instance();
//   return v3 >= (__int64)XGameServer::GetCurDate(v2);
// }
bool CGocForce::IsMatchingDate() const {
    if (m_biMatchingDate == 0) {
        return false;
    }
    // TODO: Get current date from XGameServer
    // __int64 expiryDate = m_biMatchingDate + 180;
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // return expiryDate >= pServer->GetCurDate();
    return true;
}

// IDA: ?AddMatchingDate@CGocForce@@QEAAXH@Z @ 0x140085130
// void __fastcall CGocForce::AddMatchingDate(CGocForce *this, int nAddTime)
// {
//   if ( this->m_biMatchingDate > 0 )
//     this->m_biMatchingDate += nAddTime;
// }
void CGocForce::AddMatchingDate(int nAddTime) {
    if (m_biMatchingDate > 0) {
        m_biMatchingDate += nAddTime;
    }
}

// IDA: ?SetMatchingState@CGocForce@@QEAAXE@Z @ 0x1401F3600
// void __fastcall CGocForce::SetMatchingState(CGocForce *this, unsigned __int8 byState)
// {
//   this->m_byMatchingState = byState;
// }
void CGocForce::SetMatchingState(std::uint8_t byState) {
    m_byMatchingState = byState;
}

// IDA: ?GetMatchingState@CGocForce@@QEAAEXZ @ 0x1403B0280
// __int64 __fastcall CGocForce::GetMatchingState(CGocForce *this)
// {
//   return this->m_byMatchingState;
// }
std::uint8_t CGocForce::GetMatchingState() const {
    return m_byMatchingState;
}

// IDA: ?KickOut@CGocForce@@QEAA_NKPEAVCUser@@@Z @ 0x1400846F0
// 复杂的踢出验证逻辑，包含大量错误检查
bool CGocForce::KickOut(std::uint32_t dwActorID, CUser* pUser) {
    // IDA: 1. 检查是否在 Force 中
    if (!IsParty()) {
        // IDA: SendErrorMessage(0x2E, 4, 0xCF72) - not in party
        return false;
    }

    // IDA verified: 完整的踢出验证逻辑
    // TODO: 需要完整的错误码和发送机制

    // 2. 检查不能踢自己
    // IDA: UXActorID::operator==(ownerActorID, &dwActorID)
    // if (GetOwnerActorID() == dwActorID) {
    //     // SendErrorMessage(0x2E, 4, 0xCF8E) - cannot kick self
    //     return false;
    // }

    // 3. 检查是否是队长
    // IDA: v41->vtable[5]() 调用 IsMaster 检查
    // if (!IsMaster()) {
    //     // SendErrorMessage(0x2E, 4, 0xCF74) - not master
    //     return false;
    // }

    // 4. 检查目标成员是否存在
    // auto pMember = m_pParty->GetMember(dwActorID);
    // if (!pMember) {
    //     // SendErrorMessage(0x2E, 4, 0xCF13) - member not found
    //     return false;
    // }

    // 5. 检查目标是否是队长（不能踢队长）
    // IDA: CurID == QuestID
    // if (GetMasterID() == dwActorID) {
    //     // SendErrorMessage(0x2E, 4, 0xCF6F) - cannot kick master
    //     return false;
    // }

    // 6. 检查迷宫类型
    // IDA: 检查 TB_MAZE_INFO 的 Maze_Type
    // auto pTBmazeInfo = XResourceMgr::GetTB_MAZE_INFO(mapID);
    // if (pTBmazeInfo && (Maze_Type == 0 || Maze_Type == 2 || Maze_Type == 8 || Maze_Type == 9)) {
    //     // SendErrorMessage(0x2E, 4, 0xCF80) - cannot kick in maze
    //     return false;
    // }

    // 7. 发送 PS_FORCE_LEAVE 到 CommunitySocket
    // stForceLeave.dwForceID = GetForceID();
    // stForceLeave.dwLeaveMember = dwActorID;
    // stForceLeave.bKickout = 1;
    // XSendPacket(main=0xFA, sub=3)
    // CCommunitySocket::SendCmd(..., 0x2E, 4)

    return false;
}

// IDA: ?ChangeMaster@CGocForce@@QEAAXK@Z @ 0x140084C80
// 更改队长
void CGocForce::ChangeMaster(std::uint32_t dwMaster) {
    if (!IsParty()) {
        return;
    }

    // IDA verified: 更改队长逻辑
    // TODO: 需要完整的错误码和发送机制

    // 1. 检查当前是否是队长
    // IDA: CurID == QuestID (owner is master check)
    // if (!IsMaster()) {
    //     // SendErrorMessage(0x2E, 3, 0xCF6F) - not master
    //     return;
    // }

    // 2. 检查新队长是否是成员
    // IDA: CParty::IsMember(m_pParty, dwMaster)
    // if (!m_pParty->IsMember(dwMaster)) {
    //     // SendErrorMessage(0x2E, 3, 0xCF72) - not member
    //     return;
    // }

    // 3. 发送 PS_FORCE_CHANGE_MASTER 到 CommunitySocket
    // stChangeMaster.dwReqActorID = GetOwnerActorID();
    // stChangeMaster.dwNewMasterID = dwMaster;
    // stChangeMaster.dwForceID = GetForceID();
    // stChangeMaster.nErrorCode = 0;
    // XSendPacket(main=0xFA, sub=4)
    // CCommunitySocket::SendCmd(..., 0x2E, 3)
}

// IDA: ?Leave@CGocForce@@QEAAXXZ @ 0x140084480
// 发送 PS_FORCE_LEAVE 到 CommunitySocket
void CGocForce::Leave() {
    if (!IsParty()) {
        return;
    }

    // IDA verified: 发送离开 Force 请求到 RelayServer
    // TODO: 需要访问 owner CUser 和相关结构
    // 1. 获取 owner CUser via RTTI dynamic_cast
    // 2. 检查 GetArea() 是否有效
    // 3. 构造 PS_FORCE_LEAVE
    //    dwForceID = GetForceID()
    //    dwLeaveMember = owner ActorID
    //    bKickout = 0 (自愿离开)
    // 4. 构造 XSendPacket(main=0xFA, sub=3)
    // 5. 写入 ActorID, UAID, Level (两次)
    // 6. 发送到 CommunitySocket (main=0x2E, sub=5)
}

// IDA: ?Logout@CGocForce@@QEAAXXZ @ 0x140084010
// 处理登出逻辑
void CGocForce::Logout() {
    if (IsParty()) {
        // IDA: Force 登出处理
        // TODO: 需要 CPartyMember::Logout 和 ST_UPDATE_FORCE_MEMBER 结构
        // 1. 获取 owner ActorID
        // 2. 调用 m_pParty->GetMember(dwActorID) 获取成员信息
        // 3. 构造 ST_UPDATE_FORCE_MEMBER
        // 4. 调用 CPartyMember::Logout
        // 5. 构造 XSendPacket(main=0xFA, sub=5)
        // 6. 发送到 CommunitySocket
    } else {
        // IDA: 非Force情况，发送简单的登出消息
        // TODO: 需要访问 owner CUser
        // 1. 获取 owner CUser via RTTI dynamic_cast
        // 2. 构造 XSendPacket(main=0xFA, sub=0x14)
        // 3. 写入 ActorID, logout type=2, UAID, Level
        // 4. 发送到 CommunitySocket
    }

    // IDA: 最后调用 CGocParty::Clear
    Clear();
}

// IDA: ?SetHP@CGocForce@@QEAAXH@Z @ 0x140083970
// void __fastcall CGocForce::SetHP(CGocForce *this, int nHP)
void CGocForce::SetHP(int nHP) {
    if (!IsParty()) {
        return;
    }
    // IDA verified: Get owner ActorID and MapInsID, call CForce::SetMemberHP
    // TODO: 需要访问 owner CMover 获取 ActorID 和 MapInsID
    // 1. Get owner ActorID from m_pOwner
    // 2. Get MapInsID from owner XActor
    // 3. Call m_pParty->SetMemberHP(dwActorID, uxMapID, nHP)
}

// IDA: ?SetMaxHP@CGocForce@@QEAAXH@Z @ 0x1400838B0
// void __fastcall CGocForce::SetMaxHP(CGocForce *this, int nHP)
void CGocForce::SetMaxHP(int nMaxHP) {
    if (!IsParty()) {
        return;
    }
    // IDA verified: Same pattern as SetHP, call CForce::SetMemberMaxHP
    // TODO: 需要访问 owner CMover 获取 ActorID 和 MapInsID
}

// IDA: ?SetLevel@CGocForce@@QEAAXH@Z @ 0x140083730
// void __fastcall CGocForce::SetLevel(CGocForce *this, int nLevel)
void CGocForce::SetLevel(int nLevel) {
    if (!IsParty()) {
        return;
    }
    // IDA verified: Get owner ActorID, call CForce::SetMemberLevel
    // TODO: 需要访问 owner CMover 获取 ActorID
}

// IDA: ?SetAwaken@CGocForce@@QEAAXE@Z @ 0x1400837B0
// void __fastcall CGocForce::SetAwaken(CGocForce *this, unsigned __int8 byGrade)
void CGocForce::SetAwaken(std::uint8_t byAwaken) {
    if (!IsParty()) {
        return;
    }
    // IDA verified: Same pattern as SetLevel, call CForce::SetMemberAwaken
    // TODO: 需要访问 owner CMover 获取 ActorID
}

// IDA: ?SetProfilePhoto@CGocForce@@QEAAXK@Z @ 0x140083830
// void __fastcall CGocForce::SetProfilePhoto(CGocForce *this, unsigned int dwPhotoID)
void CGocForce::SetProfilePhoto(std::uint32_t dwPhotoID) {
    if (!IsParty()) {
        return;
    }
    // IDA verified: Same pattern as SetLevel, call CForce::SetMemberProfilePhoto
    // TODO: 需要访问 owner CMover 获取 ActorID
}

// IDA: ?SetMapID@CGocForce@@QEAAXHHTUXMapID@@@Z @ 0x140083690
// void __fastcall CGocForce::SetMapID(CGocForce *this, int nMapID, int nChannel, UXMapID uxMapID)
void CGocForce::SetMapID(int nMapID, int nChannel, const UXMapID& uxMapID) {
    if (!IsParty()) {
        return;
    }
    // IDA verified: Get owner ActorID, call CForce::SetMemberMapID
    // TODO: 需要访问 owner CMover 获取 ActorID
}

// IDA: ?UpdatePartyBooster@CGocForce@@QEAAXXZ @ 0x140084EE0
// void __fastcall CGocForce::UpdatePartyBooster(CGocForce *this)
// {
//   CForce *v1; // rax
//
//   if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*((std::tr1::shared_ptr<CItemEquip> *)&this->m_pForce) != -1 )
//   {
//     v1 = (CForce *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&this->m_pForce);
//     CForce::UpdateForceBooster(v1, 0);
//   }
// }
void CGocForce::UpdatePartyBooster() {
    if (!IsParty()) {
        return;
    }
    // IDA verified: 直接调用 CForce::UpdateForceBooster(0)
    // TODO: 需要 CForce::UpdateForceBooster 方法
    // m_pParty->UpdateForceBooster(0);
}

// IDA: ?UpdatePartyBoosterByCount@CGocForce@@QEAAXHH@Z @ 0x140084F30
// 根据成员数量更新增益
void CGocForce::UpdatePartyBoosterByCount(int nPartyCount, int nFriendCount) {
    // IDA: 获取 owner CUser via RTTI dynamic_cast
    // TODO: 需要 XResourceMgr::GetTB_PARTYEXP_MEMBER 表访问
    // IDA 反编译的核心逻辑:
    // CUser* pOwnerUser = GetOwnerAsUser();
    // if (!pOwnerUser) return;
    //
    // auto iterPartyExp = XResourceMgr::GetTB_PARTYEXP_MEMBER()->begin();
    // for (; iterPartyExp != end; ++iterPartyExp) {
    //     if (iterPartyExp->PartyCount == nPartyCount && iterPartyExp->FriendCount == nFriendCount) {
    //         pOwnerUser->ChangeBooster(eBooster_Type_Party, iterPartyExp->BoosterID);
    //         return;
    //     }
    // }
    // pOwnerUser->ChangeBooster(eBooster_Type_Party, 0);  // 没找到就清除
}

// IDA: ?ReserveReviveAll@CGocForce@@QEAAXKK@Z @ 0x140083350
// IDA verified: 预约复活所有成员
void CGocForce::ReserveReviveAll(std::uint32_t dwActorID, std::uint32_t dwID) {
    // IDA: pOwnerUser = (CUser *)_RTDynamicCast_0(...)
    // TODO: 需要 owner CUser 访问和 RTTI cast

    if (!IsParty()) {
        return;
    }

    // TODO: 需要实现完整逻辑:
    // CUser* pOwnerUser = GetOwnerAsUser();
    // if (!pOwnerUser) return;
    //
    // XSendPacket xSendPacket(3, 0x48);  // main=3, sub=72
    // xSendPacket << dwActorID << dwID;
    //
    // // 如果 owner 死亡或 HP <= 0，设置预约复活
    // if (pOwnerUser->IsDie() || pOwnerUser->GetHP() <= 0) {
    //     pOwnerUser->SetReserveRevive(1);
    //     pOwnerUser->DoReserverRevive();
    // }
    //
    // // 遍历所有 Force 成员
    // for (auto& it : m_pParty->GetMembers()) {
    //     CPartyMember* pMemberInfo = it.second;
    //     // 检查是否在同一地图 (TBMapID)
    //     if (pMemberInfo->GetTBMapID() != pOwnerUser->GetTBMapID()) continue;
    //
    //     CUser* pMember = pMemberInfo->GetUser();
    //     if (!pMember) continue;
    //
    //     UXMapID uxMapID = pMember->GetMapInsID();
    //     if (!ThreadLocalData::IsThreadArea(uxMapID)) continue;
    //
    //     // 如果成员死亡，设置预约复活并发送复活请求
    //     if (pMember->IsDie()) {
    //         pMember->SetReserveRevive(1);
    //         pMember->BridgeSend(&xSendPacket);
    //     }
    // }
}

// IDA: ?LoadRecode@CGocForce@@QEAAXXZ @ 0x14010B430
// 从 CForce 加载迷宫记录到 CGocRecode
void CGocForce::LoadRecode() {
    if (!IsParty()) {
        return;
    }
    // IDA: 1. Get CGocRecode component from owner
    // IDA: 2. Call m_pParty->GetMazeRecode(dwActorID, nRecode)
    // IDA: 3. Call CGocRecode::SetFullRecode(nRecode)
    // TODO: 需要 CGocRecode 组件和 CForce::GetMazeRecode 方法
}

// IDA: ?NeedReviveBuffUser@CGocForce@@QEAA_NXZ @ 0x14010C7A0
// IDA verified: 检查是否有成员需要复活 Buff
bool CGocForce::NeedReviveBuffUser() {
    if (!IsParty()) {
        return false;
    }

    // TODO: 需要 CForce 成员迭代器和 CMover::FindBuffByEffectType
    // IDA 反编译的核心逻辑:
    // UXMapID ownerMapID = GetOwner()->GetMapInsID();
    // for (auto& it : m_pParty->GetMembers()) {
    //     CPartyMember* pMemberInfo = it.second;
    //     CUser* pMember = pMemberInfo->GetUser();
    //     if (!pMember) continue;
    //
    //     // 检查 MapInsID 是否与 owner 相同
    //     UXMapID memberMapID = pMember->GetMapInsID();
    //     if (memberMapID != ownerMapID) continue;
    //
    //     // 检查成员是否没有复活 Buff (effect type 1)
    //     if (pMember->FindBuffByEffectType(1, 0) == -1) {
    //         return true;  // 找到需要复活的成员
    //     }
    // }
    return false;
}

// IDA: ?DeletePartyBoost@CGocForce@@QEAAXXZ @ 0x14010C940
// void __fastcall CGocForce::DeletePartyBoost(CGocParty *this)
// {
//   CMover *v1; // rax
//   CGocBooster *v2; // rax
//   std::tr1::shared_ptr<CGocBooster> pBooster; // [rsp+30h] [rbp-28h] BYREF
//
//   v1 = (CMover *)std::list<CBattleZone *>::size((VChunkLocker *)this);  // get owner
//   CMover::GetGOC<CGocBooster>(v1, &pBooster, 0);  // get CGocBooster component
//   if ( std::tr1::shared_ptr::operator int std::_Bool_struct::*(pBooster) != -1 )  // if valid
//   {
//     v2 = std::tr1::shared_ptr<CGocBooster>::operator->(&pBooster);
//     CGocBooster::ChangeBooster(v2, eBooster_Type_Party, 0, 0, 0);  // remove party booster
//   }
//   std::tr1::shared_ptr::~shared_ptr(&pBooster);
// }
void CGocForce::DeletePartyBoost() {
    // IDA verified: 获取 CGocBooster 组件并移除队伍增益
    // TODO: 需要访问 owner CMover 和 CGocBooster 组件

    // 伪代码逻辑:
    // CMover* pOwner = GetOwner();  // 从 GOComponent 获取 owner
    // if (!pOwner) return;
    //
    // auto pBooster = pOwner->GetGOC<CGocBooster>();
    // if (!pBooster) return;
    //
    // pBooster->ChangeBooster(eBooster_Type_Party, 0, 0, 0);  // 移除队伍增益
}

// IDA: ?GetForceMember@CGocForce@@QEAAXPEAVCUser@@AEAV?$vector@PEAVCForceMember@@V?$allocator@PEAVCForceMember@@@std@@@std@@_N@Z @ 0x14010C9B0
// IDA verified: 获取 Force 成员列表
void CGocForce::GetForceMember(CUser* pUser, std::vector<CForceMember*>& vecMember, bool bIncludeOffline) {
    vecMember.clear();
    if (!IsParty()) {
        return;
    }

    // TODO: 需要 CForce 成员迭代器和相关依赖
    // IDA 反编译的核心逻辑:
    // for (auto& it : m_pParty->GetMembers()) {
    //     CPartyMember* pMemberInfo = it.second;
    //     if (bIncludeOffline) {
    //         vecMember.push_back((CForceMember*)pMemberInfo);
    //         continue;
    //     }
    //
    //     CUser* pMemberUser = pMemberInfo->GetUser();
    //     if (!pMemberUser) continue;
    //
    //     // 检查成员未死亡
    //     if (CMover::IsDie(pMemberUser)) continue;
    //
    //     // 检查是否在同一地图
    //     UXMapID memberMapID = pMemberUser->GetMapInsID();
    //     UXMapID killerMapID = pUser->GetMapInsID();
    //     if (memberMapID != killerMapID) continue;
    //
    //     // 检查是否是同一成员或在迷宫中
    //     if (pMemberInfo->GetActorID() == pUser->GetActorID() || pUser->IsMaze()) {
    //         vecMember.push_back((CForceMember*)pMemberInfo);
    //         continue;
    //     }
    //
    //     // 检查距离 < 5000
    //     hkvVec3* pos1 = pMemberUser->GetPosition();
    //     hkvVec3* pos2 = pUser->GetPosition();
    //     float distance = hkvVec3::Dist(pos1, pos2);
    //     if (distance < 5000.0f) {
    //         vecMember.push_back((CForceMember*)pMemberInfo);
    //     }
    // }
}

// IDA: ?GetForceUserCount@CGocForce@@QEAAEXZ @ 0x14010D330
// unsigned __int8 __fastcall CGocForce::GetForceUserCount(CGocParty *this)
// {
//   CParty *v1; // rax
//
//   if ( !CGocParty::IsParty(this) )
//     return 0;
//   v1 = (CParty *)std::tr1::shared_ptr<CForce>::operator->(&this->m_pParty);
//   return CParty::GetUserCount(v1);
// }
std::uint8_t CGocForce::GetForceUserCount() const {
    if (!IsParty()) {
        return 0;
    }
    // IDA verified: CParty::GetUserCount()
    // TODO: 需要 CParty::GetUserCount 方法
    // return m_pParty ? m_pParty->GetUserCount() : 0;
    return 0;
}

// IDA: ?GetMasterID@CGocForce@@QEAAKXZ @ 0x14010D410
// __int64 __fastcall CGocForce::GetMasterID(CGocParty *this)
// {
//   VisRenderCollection_cl *v1; // rax
//
//   if ( std::tr1::shared_ptr::operator int std::_Bool_struct::*(m_pParty) == -1 )
//     return 0;
//   v1 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pParty);
//   return CWayPoint::GetCurID(v1);  // 返回当前队长 ID
// }
std::uint32_t CGocForce::GetMasterID() const {
    if (!IsParty()) {
        return 0;
    }
    // IDA verified: CWayPoint::GetCurID maps to CForce::GetMasterID
    // TODO: 需要 CForce::GetMasterID 方法
    // return m_pParty ? m_pParty->GetMasterID() : 0;
    return 0;
}

// IDA: ?SetExp@CGocForce@@QEAAXPEAVCUser@@MH@Z @ 0x140083A30
// 经验分配逻辑
void CGocForce::SetExp(CUser* pUser, float fExp, int nExpType) {
    if (!IsParty()) {
        return;
    }

    // IDA verified: 复杂的经验分配逻辑
    // TODO: 需要 XResourceMgr、TB_PARTYEXP_LEVEL、TB_PARTYEXP_MOB 表

    // 伪代码逻辑:
    // 1. 计算等级差距 (LevelGap)
    // auto pForce = m_pParty.get();
    // int nLevelGap = pForce->GetLevelGap();

    // 2. 从 TB_PARTYEXP_LEVEL 获取等级间隔加成
    // auto pTBPartyExpLevel = XResourceMgr::GetTB_PARTYEXP_LEVEL(nLevelGap);
    // if (!pTBPartyExpLevel) return;
    // float fTotalExp = fExp * pTBPartyExpLevel->LevelInterval_Value;

    // 3. 计算怪物等级间隔
    // int cMonsterInterval = pForce->GetCommonThreadIndex() - nExpType;

    // 4. 从 TB_PARTYEXP_MOB 获取怪物等级加成
    // auto pTBPartyExpMob = XResourceMgr::GetTB_PARTYEXP_MOB(cMonsterInterval);
    // if (!pTBPartyExpMob) return;
    // fTotalExp *= pTBPartyExpMob->Mob_IntervalValue;

    // 5. 获取所有成员列表
    // std::vector<CForceMember*> vecExpMember;
    // GetForceMember(pUser, vecExpMember, false);

    // 6. 对每个成员计算最终经验
    // for (auto pMember : vecExpMember) {
    //     CUser* pMemberUser = pMember->GetUser();
    //     if (!pMemberUser) continue;
    //
    //     float fForceExp = fTotalExp;
    //
    //     // 特殊效果加成
    //     auto pAttr = pMemberUser->GetGOC<CGocAttribute>();
    //     float fRate = pAttr->GetSpecialEffect(EFFECT_SPECIAL_EXP_ADD_RAT) * 0.01f;
    //
    //     // Booster 加成
    //     auto pBooster = pMemberUser->GetGOC<CGocBooster>();
    //     fRate += pBooster->GetTotalRate(eBooster_Effect_IncExp);
    //
    //     // League 技能加成
    //     float fLeagueRate = 0.0f;
    //     if (pMemberUser->IsLeagueSkill(4)) {
    //         fLeagueRate = pMemberUser->GetLeagueSkillEffectValue(4) * 0.01f;
    //     }
    //
    //     float fBonus = fForceExp * (fRate + fLeagueRate);
    //     fForceExp += fBonus;
    //
    //     // 设置经验
    //     pAttr->SetExp(fForceExp, fBonus, 1);
    // }
}

// IDA: ?IsMember@CGocForce@@QEAA_NPEAVXActor@@@Z @ 0x14010BBB0
// IDA verified: 检查是否为 Force 成员
// bool __fastcall CGocForce::IsMember(CGocParty *this, XActor *pMember)
// {
//   if ( m_pParty invalid ) return 0;
//   v2 = pMember->GetActorID(pMember, v5);
//   dwActor = CQuestCondition::GetQuestID(v2);  // extract actor ID
//   v3 = m_pParty.operator->();
//   return CParty::IsMember(v3, dwActor);
// }
bool CGocForce::IsMember(XActor* pActor) const {
    if (!IsParty() || !pActor) {
        return false;
    }
    // IDA verified: 获取 ActorID 并检查是否是 Force 成员
    // TODO: 需要 XActor::GetActorID 方法和 CParty::IsMember
    // std::uint32_t dwActorID = pActor->GetActorID();
    // return m_pParty->IsMember(dwActorID);
    return false;
}

// IDA: ?GetFamilyID@CGocForce@@SAHXZ @ 0x140039030
// __int64 __fastcall CGocForce::GetFamilyID()
// {
//   return 22;
// }
int CGocForce::GetFamilyID() {
    // IDA verified: 直接返回常量 22
    return 22;
}
