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
// __int64 __fastcall CGocForce::CGocForce(CGocForce *this)
// {
//   GOComponent::GOComponent(this);
//   this->__vftable = (CGocForce_vtbl *)&CGocForce::`vftable';
//   std::tr1::shared_ptr<CForce>::shared_ptr<CForce>(&this->m_pForce, 0);
//   return this;
// }
CGocForce::CGocForce()
    : GOComponent()
    , m_pForce(nullptr)
    , m_biMatchingDate(0)
    , m_byMatchingState(0) {
    // IDA: vtable is set after GOComponent construction
    // IDA: m_pForce is initialized to nullptr via shared_ptr constructor
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
    // IDA: m_pForce destructor is called
    m_pForce.reset();
    // IDA: GOComponent destructor is called
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

// Clear - 清除 Force 状态
void CGocForce::Clear() {
    if (IsForce()) {
        m_pForce.reset();
    }
    m_biMatchingDate = 0;
    m_byMatchingState = 0;
}

// IsForce - 检查是否在 Force 中 (继承自 CGocParty)
bool CGocForce::IsForce() const {
    return m_pForce != nullptr;
}

// GetForceID - 获取 Force ID (继承自 CGocParty)
// IDA: 调用 CForce::GetForceID 或类似方法
std::uint32_t CGocForce::GetForceID() const {
    if (!IsForce()) {
        return 0;
    }
    // IDA verified: Call CForce::GetForceID()
    return static_cast<std::uint32_t>(m_pForce->GetForceID());
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
    if (!IsForce()) {
        return false;
    }
    // IDA verified: Force 最多 8 个成员
    return m_pForce->GetMemberCount() >= 8;
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
    if (!IsForce()) {
        return false;
    }
    // IDA verified: Compare CForce master ID (GetCurID) with dwUCID
    return GetMasterID() == dwUCID;
}

// IsMaster - 检查当前玩家是否为队长
// IDA: 获取 owner ActorID 并与 MasterID 比较
bool CGocForce::IsMaster() const {
    if (!IsForce()) {
        return false;
    }
    // TODO: 需要从 CMover 获取当前 ActorID 并与 MasterID 比较
    // 需要访问 owner CMover 的 ActorID
    // IDA: pOwner->GetActorID() == m_pForce->GetCurID()
    return false;
}

// GetForce - 获取 Force 对象
std::shared_ptr<CForce> CGocForce::GetForce() const {
    return m_pForce;
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
    m_pForce = pForce;
    // TODO: Register party member if pForce is valid
    // if (pForce) {
    //     // Get owner CMover/CUser and register as member
    // }
}

// Send - 发送数据包给所有 Force 成员
void CGocForce::Send(XSendPacket& sendPacket) {
    if (!IsForce()) {
        return;
    }
    // TODO: 需要调用 CForce::Send 广播给所有成员
}

// IDA: ?SendForceInfo@CGocForce@@QEAAXE@Z @ 0x140084310
// 发送 PS_FORCE_INFO 给当前用户
// void __fastcall CGocForce::SendForceInfo(CGocForce *this, unsigned __int8 byUpdateType, float a3)
// {
//   ...
//   if ( m_pForce valid )
//   {
//     PS_PARTY_INFO::PS_PARTY_INFO(&stForceInfo);
//     CParty::GetPartyInfo(m_pForce.get(), &stForceInfo);
//     stForceInfo.byUpdateType = byUpdateType;
//     stForceInfo.byForceType = CForce::GetForceType(m_pForce.get());
//     XSendPacket::XSendPacket(&xSendPacket, 0x2E, 9);  // main=46, sub=9
//     operator<<(&xSendPacket, &stForceInfo);
//     CGocNetwork::Send(pActor, &xSendPacket);
//   }
// }
void CGocForce::SendForceInfo(std::uint8_t byUpdateType) {
    if (!IsForce()) {
        return;
    }
    // IDA verified: 构造 PS_FORCE_INFO 并发送给当前用户
    // TODO: 需要 PS_FORCE_INFO 结构和发送机制
    // 1. 构造 PS_FORCE_INFO
    // 2. 调用 CForce::GetPartyInfo 填充信息
    // 3. 设置 byUpdateType 和 byForceType
    // 4. 发送 main=0x2E(46), sub=9 的数据包
}

// UpdatePartyBoosterByCount - 根据成员数量更新增益
// IDA: ?UpdatePartyBoosterByCount@CGocForce@@QEAAXHH@Z @ 0x140084F30
void CGocForce::UpdatePartyBoosterByCount(int nPartyCount, int nFriendCount) {
    // IDA: 遍历 TB_PARTYEXP_MEMBER 表，找到匹配的 PartyCount 和 FriendCount
    // 然后调用 CUser::ChangeBooster(eBooster_Type_Party, wBoosterID)
    // TODO: 需要 XResourceMgr::GetTB_PARTYEXP_MEMBER 表访问
    // 伪代码:
    // auto pOwner = GetOwnerAsUser();
    // if (!pOwner) return;
    //
    // auto pTable = XResourceMgr::GetTB_PARTYEXP_MEMBER();
    // for (auto& row : pTable) {
    //     if (row.PartyCount == nPartyCount && row.FriendCount == nFriendCount) {
    //         pOwner->ChangeBooster(eBooster_Type_Party, row.BoosterID);
    //         break;
    //     }
    // }
}

// LoadRecode - 从 CForce 加载迷宫记录到 CGocRecode
// IDA: ?LoadRecode@CGocForce@@QEAAXXZ @ 0x14010B430
void CGocForce::LoadRecode() {
    if (!IsForce()) {
        return;
    }
    // IDA: 1. Get CGocRecode component from owner
    // IDA: 2. Call m_pForce->GetMazeRecode(dwActorID, nRecode)
    // IDA: 3. Call CGocRecode::SetFullRecode(nRecode)
    // TODO: 需要 CGocRecode 组件和 CForce::GetMazeRecode 方法
}

// ShowMyForceInfo - 显示我的 Force 信息
void CGocForce::ShowMyForceInfo() {
    if (!IsForce()) {
        return;
    }
    // 发送 Force 信息给当前用户
    // TODO: 调用 SendForceInfo 或类似方法
}

// Send - 发送数据包给所有 Force 成员
void CGocForce::Send(XSendPacket& sendPacket) {
    if (!IsForce()) {
        return;
    }
    // IDA: 调用 CForce::Send 广播给所有成员
    // TODO: 需要 CForce::Send 方法
    // m_pForce->Send(sendPacket);
}

// IDA: ?SetHP@CGocForce@@QEAAXH@Z @ 0x140083970
// void __fastcall CGocForce::SetHP(CGocForce *this, int nHP)
// {
//   VChunkFile *v2; // rax
//   VBitmask *v3; // rax
//   CForce *v4; // rax
//   UXMapID result; // [rsp+20h] [rbp-38h] BYREF
//   char v6[8]; // [rsp+28h] [rbp-30h] BYREF
//   DynArray_cl<int> *p_m_ChunkSizeTempMemOfs; // [rsp+30h] [rbp-28h]
//   UXMapID *MapInsID; // [rsp+38h] [rbp-20h]
//   DynArray_cl<int>_vtbl *v9; // [rsp+40h] [rbp-18h]
//   unsigned int dwActorID; // [rsp+48h] [rbp-10h]
//
//   if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*((std::tr1::shared_ptr<CItemEquip> *)&this->m_pForce) != -1 )
//   {
//     p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone *>::size((VChunkLocker *)this)[3].m_ChunkSizeTempMemOfs;
//     v2 = std::list<CBattleZone *>::size((VChunkLocker *)this);
//     MapInsID = XActor::GetMapInsID((XActor *)&v2[3].m_ChunkSizeTempMemOfs, &result);
//     v9 = p_m_ChunkSizeTempMemOfs->__vftable;
//     v3 = (VBitmask *)((__int64 (__fastcall *)(DynArray_cl<int> *, char *))v9[7].dtr_DynArray_cl<int>)(p_m_ChunkSizeTempMemOfs, v6);
//     dwActorID = CQuestCondition::GetQuestID(v3);
//     v4 = (CForce *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&this->m_pForce);
//     CForce::SetMemberHP(v4, dwActorID, (UXMapID)MapInsID->__s0, nHP);
//   }
// }
void CGocForce::SetHP(int nHP) {
    if (!IsForce()) {
        return;
    }
    // IDA verified: Get owner ActorID and MapInsID, call CForce::SetMemberHP
    // TODO: 需要访问 owner CMover 获取 ActorID 和 MapInsID
    // 1. Get owner ActorID from m_pOwner
    // 2. Get MapInsID from owner XActor
    // 3. Call m_pForce->SetMemberHP(dwActorID, uxMapID, nHP)
}

// IDA: ?SetMaxHP@CGocForce@@QEAAXH@Z @ 0x1400838B0
// void __fastcall CGocForce::SetMaxHP(CGocForce *this, int nHP)
// 同 SetHP 结构，调用 CForce::SetMemberMaxHP
void CGocForce::SetMaxHP(int nMaxHP) {
    if (!IsForce()) {
        return;
    }
    // IDA verified: Same pattern as SetHP, call CForce::SetMemberMaxHP
    // TODO: 需要访问 owner CMover 获取 ActorID 和 MapInsID
}

// IDA: ?SetLevel@CGocForce@@QEAAXH@Z @ 0x140083730
// void __fastcall CGocForce::SetLevel(CGocForce *this, int nLevel)
// {
//   VBitmask *v2; // rax
//   CForce *v3; // rax
//   _BYTE v4[8]; // [rsp+20h] [rbp-28h] BYREF
//   DynArray_cl<int> *p_m_ChunkSizeTempMemOfs; // [rsp+28h] [rbp-20h]
//   unsigned int dwActorID; // [rsp+30h] [rbp-18h]
//
//   if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*((std::tr1::shared_ptr<CItemEquip> *)&this->m_pForce) != -1 )
//   {
//     p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone *>::size((VChunkLocker *)this)[3].m_ChunkSizeTempMemOfs;
//     v2 = (VBitmask *)((__int64 (__fastcall *)(DynArray_cl<int> *, _BYTE *))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(p_m_ChunkSizeTempMemOfs, v4);
//     dwActorID = CQuestCondition::GetQuestID(v2);
//     v3 = (CForce *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&this->m_pForce);
//     CForce::SetMemberLevel(v3, dwActorID, nLevel);
//   }
// }
void CGocForce::SetLevel(int nLevel) {
    if (!IsForce()) {
        return;
    }
    // IDA verified: Get owner ActorID, call CForce::SetMemberLevel
    // TODO: 需要访问 owner CMover 获取 ActorID
}

// IDA: ?SetAwaken@CGocForce@@QEAAXE@Z @ 0x1400837B0
// void __fastcall CGocForce::SetAwaken(CGocForce *this, unsigned __int8 byGrade)
// 同 SetLevel 结构，调用 CForce::SetMemberAwaken
void CGocForce::SetAwaken(std::uint8_t byAwaken) {
    if (!IsForce()) {
        return;
    }
    // IDA verified: Same pattern as SetLevel, call CForce::SetMemberAwaken
    // TODO: 需要访问 owner CMover 获取 ActorID
}

// IDA: ?SetProfilePhoto@CGocForce@@QEAAXK@Z @ 0x140083830
// void __fastcall CGocForce::SetProfilePhoto(CGocForce *this, unsigned int dwPhotoID)
// 同 SetLevel 结构，调用 CForce::SetMemberProfilePhoto
void CGocForce::SetProfilePhoto(std::uint32_t dwPhotoID) {
    if (!IsForce()) {
        return;
    }
    // IDA verified: Same pattern as SetLevel, call CForce::SetMemberProfilePhoto
    // TODO: 需要访问 owner CMover 获取 ActorID
}

// IDA: ?SetMapID@CGocForce@@QEAAXHHTUXMapID@@@Z @ 0x140083690
// void __fastcall CGocForce::SetMapID(CGocForce *this, int nMapID, int nChannel, UXMapID uxMapID)
// {
//   VBitmask *v4; // rax
//   CForce *v5; // rax
//   _BYTE v6[8]; // [rsp+30h] [rbp-28h] BYREF
//   DynArray_cl<int> *p_m_ChunkSizeTempMemOfs; // [rsp+38h] [rbp-20h]
//   unsigned int dwActorID; // [rsp+40h] [rbp-18h]
//
//   if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*((std::tr1::shared_ptr<CItemEquip> *)&this->m_pForce) != -1 )
//   {
//     p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone *>::size((VChunkLocker *)this)[3].m_ChunkSizeTempMemOfs;
//     v4 = (VBitmask *)((__int64 (__fastcall *)(DynArray_cl<int> *, _BYTE *))p_m_ChunkSizeTempMemOfs->__vftable[7].dtr_DynArray_cl<int>)(p_m_ChunkSizeTempMemOfs, v6);
//     dwActorID = CQuestCondition::GetQuestID(v4);
//     v5 = (CForce *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&this->m_pForce);
//     CForce::SetMemberMapID(v5, dwActorID, nMapID, nChannel, uxMapID);
//   }
// }
void CGocForce::SetMapID(int nMapID, int nChannel, const UXMapID& uxMapID) {
    if (!IsForce()) {
        return;
    }
    // IDA verified: Get owner ActorID, call CForce::SetMemberMapID
    // TODO: 需要访问 owner CMover 获取 ActorID
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

// IDA: ?Logout@CGocForce@@QEAAXXZ @ 0x140084010
// 处理登出逻辑，发送 PS_FORCE_LEAVE 到 CommunitySocket
// void __fastcall CGocForce::Logout(CGocForce *this, float a2, float a3)
// {
//   if ( CGocParty::IsParty((CGocParty *)this) )
//   {
//     dwActorID = CQuestCondition::GetQuestID(v3);
//     v4 = (CParty *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&this->m_pForce);
//     pMembera = CParty::GetMember(v4, dwActorID);
//     if ( pMembera )
//     {
//       ST_UPDATE_PARTY_MEMBER::ST_UPDATE_PARTY_MEMBER(&stForceMember);
//       stForceMember.dwForceID = CGocParty::GetPartyID((CGocParty *)this);
//       CPartyMember::Logout(pMembera, &stForceMember);
//       XSendPacket::XSendPacket(&xSendPacket, 0xFA, 5);  // main=250, sub=5
//       operator<<(&xSendPacket, &stForceMember);
//       CCommunitySocket::SendCheck(&communitySocket, &xSendPacket);
//     }
//   }
//   else
//   {
//     // 发送非 Force 登出消息
//     pUser = _RTDynamicCast_0(...);
//     if ( pUser )
//     {
//       XSendPacket::XSendPacket(&v23, 0xFA, 0x14);  // main=250, sub=20
//       XParse::operator<<(&v23.XParse, dwActorID);
//       XParse::operator<<(&v23.XParse, 2);  // logout type
//       XParse::operator<<(&v23.XParse, dwUAID);
//       XParse::operator<<(&v23.XParse, byLevel);
//       CCommunitySocket::SendCheck(&communitySocket, &v23);
//     }
//   }
//   CGocParty::Clear((CGocParty *)this);
// }
void CGocForce::Logout() {
    if (!IsForce()) {
        // IDA: 非Force情况，发送简单的登出消息
        // TODO: 需要访问 owner CUser
        // 1. 构造 XSendPacket(main=0xFA, sub=0x14)
        // 2. 写入 ActorID, logout type=2, UAID, Level
        // 3. 发送到 CommunitySocket
        Clear();
        return;
    }

    // IDA verified: Force 登出处理
    // TODO: 需要 CPartyMember::Logout 和 ST_UPDATE_FORCE_MEMBER 结构
    // 1. 获取 owner ActorID
    // 2. 调用 m_pForce->GetMember(dwActorID) 获取成员信息
    // 3. 构造 ST_UPDATE_FORCE_MEMBER
    // 4. 调用 CPartyMember::Logout
    // 5. 构造 XSendPacket(main=0xFA, sub=5)
    // 6. 发送到 CommunitySocket
    Clear();
}

// IDA: ?Leave@CGocForce@@QEAAXXZ @ 0x140084480
// 发送 PS_FORCE_LEAVE 到 CommunitySocket
// void __fastcall CGocForce::Leave(CGocForce *this, float a2, float a3)
// {
//   if ( CGocParty::IsParty((CGocParty *)this) )
//   {
//     pUser = (CUser *)_RTDynamicCast_0(...);
//     if ( pUser )
//     {
//       if ( v21->GetArea(&pUser->XActor) )
//       {
//         stForceLeave.dwForceID = CGocParty::GetPartyID((CGocParty *)this);
//         stForceLeave.dwLeaveMember = CQuestCondition::GetQuestID(v4);  // owner ActorID
//         stForceLeave.bKickout = 0;  // leave, not kick
//         XSendPacket::XSendPacket(&xSendPacket, 0xFA, 3);  // main=250, sub=3
//         operator<<(&xSendPacket, &stForceLeave);
//         XParse::operator<<(&xSendPacket.XParse, dwActorID);
//         XParse::operator<<(&xSendPacket.XParse, dwUAID);
//         XParse::operator<<(&xSendPacket.XParse, byLevel);
//         XParse::operator<<(&xSendPacket.XParse, byLevel);  // twice?
//         CCommunitySocket::SendCmd(&communitySocket, &xSendPacket, pUser, 0x2E, 5);
//       }
//     }
//   }
// }
void CGocForce::Leave() {
    if (!IsForce()) {
        return;
    }
    // IDA verified: 发送离开 Force 请求到 RelayServer
    // TODO: 需要访问 owner CUser 和相关结构
    // 1. 构造 PS_FORCE_LEAVE
    // 2. 设置 dwForceID = GetForceID()
    // 3. 设置 dwLeaveMember = owner ActorID
    // 4. 设置 bKickout = 0 (自愿离开)
    // 5. 构造 XSendPacket(main=0xFA, sub=3)
    // 6. 写入 ActorID, UAID, Level
    // 7. 发送到 CommunitySocket (main=0x2E, sub=5)
}

// IDA: ?KickOut@CGocForce@@QEAA_NKPEAVCUser@@@Z @ 0x1400846F0
// 复杂的踢出验证逻辑
// char __fastcall CGocForce::KickOut(CGocForce *this, unsigned int dwActorID, CUser *pUser)
// {
//   if ( !CGocParty::IsParty((CGocParty *)this) )
//   {
//     CGocNetwork::SendErrorMessage(v5, 0x2E, 4, 0xCF72);  // not in party
//     return 0;
//   }
//   // 检查不能踢自己
//   if ( UXActorID::operator==(v7, &dwActorIDa) )
//   {
//     CGocNetwork::SendErrorMessage(v8, 0x2E, 4, 0xCF8E);  // cannot kick self
//     return 0;
//   }
//   // 检查是否是队长
//   if ( !IsMaster() )
//   {
//     CGocNetwork::SendErrorMessage(v9, 0x2E, 4, 0xCF74);  // not master
//     return 0;
//   }
//   // 检查成员是否存在
//   pMember = CParty::GetMember(m_pForce, dwActorID);
//   if ( !pMember )
//   {
//     CGocNetwork::SendErrorMessage(v12, 0x2E, 4, 0xCF13);  // member not found
//     return 0;
//   }
//   // 检查目标是否是队长
//   if ( CurID == QuestID )  // 目标是队长
//   {
//     CGocNetwork::SendErrorMessage(v16, 0x2E, 4, 0xCF6F);  // cannot kick master
//     return 0;
//   }
//   // 检查是否是成员
//   if ( !CParty::IsMember(m_pForce, dwActorID) )
//   {
//     CGocNetwork::SendErrorMessage(v10, 0x2E, 4, 0xCF72);
//     return 0;
//   }
//   // 检查迷宫类型
//   pTBmazeInfo = XResourceMgr::GetTB_MAZE_INFO(...);
//   if ( pTBmazeInfo && (!Maze_Type || Maze_Type == 2 || Maze_Type == 8 || Maze_Type == 9) )
//   {
//     CGocNetwork::SendErrorMessage(v19, 0x2E, 4, 0xCF80);  // cannot kick in maze
//     return 0;
//   }
//   // 发送踢出请求
//   stForceLeave.dwForceID = GetPartyID();
//   stForceLeave.dwLeaveMember = dwActorID;
//   stForceLeave.bKickout = 1;
//   XSendPacket::XSendPacket(&xSendPacket, 0xFA, 3);  // main=250, sub=3
//   operator<<(&xSendPacket, &stForceLeave);
//   XParse::operator<<(&xSendPacket.XParse, pUser->GetActorID());
//   XParse::operator<<(&xSendPacket.XParse, pUser->GetUAID());
//   XParse::operator<<(&xSendPacket.XParse, pUser->GetLevel());
//   XParse::operator<<(&xSendPacket.XParse, pMember->GetAwaken());
//   CCommunitySocket::SendCmd(&communitySocket, &xSendPacket, pUser, 0x2E, 4);
//   return 1;
// }
bool CGocForce::KickOut(std::uint32_t dwActorID, CUser* pUser) {
    if (!IsForce()) {
        // IDA: SendErrorMessage(0x2E, 4, 0xCF72) - not in party
        return false;
    }

    // IDA verified: 完整的踢出验证逻辑
    // TODO: 需要完整的错误码和发送机制

    // 1. 检查不能踢自己
    // if (GetOwnerActorID() == dwActorID) {
    //     // SendErrorMessage(0x2E, 4, 0xCF8E)
    //     return false;
    // }

    // 2. 检查是否是队长
    // if (!IsMaster()) {
    //     // SendErrorMessage(0x2E, 4, 0xCF74)
    //     return false;
    // }

    // 3. 检查目标成员是否存在
    // auto pMember = m_pForce->GetMember(dwActorID);
    // if (!pMember) {
    //     // SendErrorMessage(0x2E, 4, 0xCF13)
    //     return false;
    // }

    // 4. 检查目标是否是队长（不能踢队长）
    // if (GetMasterID() == dwActorID) {
    //     // SendErrorMessage(0x2E, 4, 0xCF6F)
    //     return false;
    // }

    // 5. 检查迷宫类型
    // auto pTBmazeInfo = XResourceMgr::GetTB_MAZE_INFO(mapID);
    // if (pTBmazeInfo && (Maze_Type == 0 || Maze_Type == 2 || Maze_Type == 8 || Maze_Type == 9)) {
    //     // SendErrorMessage(0x2E, 4, 0xCF80)
    //     return false;
    // }

    // 6. 发送 PS_FORCE_LEAVE 到 CommunitySocket
    // stForceLeave.dwForceID = GetForceID();
    // stForceLeave.dwLeaveMember = dwActorID;
    // stForceLeave.bKickout = 1;
    // XSendPacket(main=0xFA, sub=3)
    // CCommunitySocket::SendCmd(..., 0x2E, 4)

    return false;
}

// IDA: ?ChangeMaster@CGocForce@@QEAAXK@Z @ 0x140084C80
// 更改队长
// void __fastcall CGocForce::ChangeMaster(CGocForce *this, unsigned int dwMaster, float a3)
// {
//   if ( CGocParty::IsParty((CGocParty *)this) )
//   {
//     QuestID = CQuestCondition::GetQuestID(v4);  // owner ActorID
//     CurID = CWayPoint::GetCurID(v5);  // current master ID
//     if ( CurID == QuestID )  // owner is master
//     {
//       if ( CParty::IsMember(v8, dwMaster) )  // new master is member
//       {
//         stChangeMaster.dwReqActorID = CQuestCondition::GetQuestID(v10);  // owner ActorID
//         stChangeMaster.dwNewMasterID = dwMaster;
//         stChangeMaster.dwForceID = CParty::GetPartyID(v11);
//         stChangeMaster.nErrorCode = 0;
//         XSendPacket::XSendPacket(&xSendPacket, 0xFA, 4);  // main=250, sub=4
//         operator<<(&xSendPacket, &stChangeMaster);
//         CCommunitySocket::SendCmd(&communitySocket, &xSendPacket, pUser, 0x2E, 3);
//       }
//       else
//       {
//         CGocNetwork::SendErrorMessage(v9, 0x2E, 3, 0xCF72);  // not member
//       }
//     }
//     else
//     {
//       CGocNetwork::SendErrorMessage(v7, 0x2E, 3, 0xCF6F);  // not master
//     }
//   }
// }
void CGocForce::ChangeMaster(std::uint32_t dwMaster) {
    if (!IsForce()) {
        return;
    }

    // IDA verified: 更改队长逻辑
    // TODO: 需要完整的错误码和发送机制

    // 1. 检查当前是否是队长
    // if (!IsMaster()) {
    //     // SendErrorMessage(0x2E, 3, 0xCF6F)
    //     return;
    // }

    // 2. 检查新队长是否是成员
    // if (!m_pForce->IsMember(dwMaster)) {
    //     // SendErrorMessage(0x2E, 3, 0xCF72)
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

// IDA: ?LoadRecode@CGocForce@@QEAAXXZ @ 0x14010B430
// 从 CForce 加载迷宫记录到 CGocRecode
void CGocForce::LoadRecode() {
    if (!IsForce()) {
        return;
    }
    // IDA: 1. Get CGocRecode component from owner
    // IDA: 2. Call m_pForce->GetMazeRecode(dwActorID, nRecode)
    // IDA: 3. Call CGocRecode::SetFullRecode(nRecode)
    // TODO: 需要 CGocRecode 组件和 CForce::GetMazeRecode 方法
}

// IDA: ?UpdatePartyBooster@CGocForce@@QEAAXXZ @ 0x140084EE0
// void __fastcall CGocForce::UpdatePartyBooster(CGocForce *this)
// {
//   CForce *v1; // rax
//
//   if ( std::tr1::shared_ptr::operator int std::_Bool_struct::*(m_pForce) != -1 )
//   {
//     v1 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pForce);
//     CForce::UpdateForceBooster(v1, 0);
//   }
// }
void CGocForce::UpdatePartyBooster() {
    if (!IsForce()) {
        return;
    }
    // IDA verified: 调用 CForce::UpdateForceBooster(0)
    // TODO: 需要 CForce::UpdateForceBooster 方法
    // m_pForce->UpdateForceBooster(0);
}

// IDA: ?UpdatePartyBoosterByCount@CGocForce@@QEAAXHH@Z @ 0x140084F30
// 根据成员数量更新增益
void CGocForce::UpdatePartyBoosterByCount(int nPartyCount, int nFriendCount) {
    // IDA: 遍历 TB_PARTYEXP_MEMBER 表，找到匹配的 PartyCount 和 FriendCount
    // 然后调用 CUser::ChangeBooster(eBooster_Type_Party, wBoosterID)
    // TODO: 需要 XResourceMgr::GetTB_PARTYEXP_MEMBER 表访问
    // 伪代码:
    // auto pOwner = GetOwnerAsUser();
    // if (!pOwner) return;
    //
    // auto pTable = XResourceMgr::GetTB_PARTYEXP_MEMBER();
    // for (auto& row : pTable) {
    //     if (row.PartyCount == nPartyCount && row.FriendCount == nFriendCount) {
    //         pOwner->ChangeBooster(eBooster_Type_Party, row.BoosterID);
    //         break;
    //     }
    // }
}

// IDA: ?ReserveReviveAll@CGocForce@@QEAAXKK@Z @ 0x140083350
// 预约复活所有成员
// void __fastcall CGocForce::ReserveReviveAll(CGocParty *this, int dwActorID, int dwID)
// {
//   pOwnerUser = (CUser *)_RTDynamicCast_0(...);  // 获取 owner CUser
//   if ( pOwnerUser )
//   {
//     XSendPacket::XSendPacket(&xSendPacket, 3, 0x48);  // main=3, sub=72
//     XParse::operator<<(&xSendPacket.XParse, dwActorID);
//     XParse::operator<<(&xSendPacket.XParse, dwID);
//
//     // 如果 owner 死亡或 HP <= 0，设置预约复活
//     if ( CMover::IsDie(&pOwnerUser->CMoverEx) || GetHP(&pOwnerUser->CMoverEx) <= 0 )
//     {
//       CUser::SetReserveRevive(pOwnerUser, 1);
//       CUser::DoReserverRevive(pOwnerUser);
//     }
//
//     // 遍历所有 Force 成员
//     if ( m_pParty valid )
//     {
//       for ( auto& it : m_pParty->GetMembers() )
//       {
//         pMemberInfo = std::_Tree_iterator::operator->(&it)->second.__vftable;
//         p_m_ChunkSizeTempMemOfs = &std::list<CBattleZone *>::size(this)[3].m_ChunkSizeTempMemOfs;
//         TBMapID = XArea::GetTBMapID(v12);  // owner map ID
//         if ( pMemberInfo->MapID == TBMapID )  // same map
//         {
//           pMember = std::tr1::_Ptr_base::_Get(pMemberInfo);
//           if ( pMember )
//           {
//             XActor::GetMapInsID(&pMember->XActor, &uxMapID);
//             Instance = ThreadLocalData::GetInstance();
//             if ( ThreadLocalData::IsThreadArea(Instance, uxMapID) && CMover::IsDie(&pMember->CMoverEx) )
//             {
//               CUser::SetReserveRevive(pMember, 1);
//               v30->BridgeSend(&pMember->XActor, &xSendPacket);  // 发送复活请求
//             }
//           }
//         }
//       }
//     }
//   }
// }
void CGocForce::ReserveReviveAll(std::uint32_t dwActorID, std::uint32_t dwID) {
    if (!IsForce()) {
        return;
    }

    // IDA verified: 复杂的复活逻辑
    // TODO: 需要访问 owner CUser 和 CForce 成员迭代器

    // 伪代码逻辑:
    // CUser* pOwnerUser = GetOwnerAsUser();
    // if (!pOwnerUser) return;
    //
    // // 构造复活数据包
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
    // std::uint16_t ownerTBMapID = pOwnerUser->GetTBMapID();
    //
    // for (auto& member : m_pForce->GetMembers()) {
    //     // 检查是否在同一地图
    //     if (member->GetMapID() != ownerTBMapID) continue;
    //
    //     CUser* pMember = member->GetUser();
    //     if (!pMember) continue;
    //
    //     UXMapID uxMapID = pMember->GetMapInsID();
    //     if (!ThreadLocalData::IsThreadArea(uxMapID)) continue;
    //
    //     // 如果成员死亡，设置预约复活并发送复活请求
    //     if (pMember->IsDie()) {
    //         pMember->SetReserveRevive(1);
    //         pMember->Send(xSendPacket);  // 发送复活请求
    //     }
    // }
}

// IDA: ?NeedReviveBuffUser@CGocForce@@QEAA_NXZ @ 0x14010C7A0
// 检查是否有成员需要复活 Buff
// char __fastcall CGocForce::NeedReviveBuffUser(CGocParty *this)
// {
//   if ( m_pForce valid )
//   {
//     for ( auto& it : m_pForce->GetMembers() )
//     {
//       pMemberInfo = std::_Tree_iterator::operator->(&it)->second.__vftable;
//       if ( pMemberInfo )
//       {
//         pMember = std::tr1::_Ptr_base::_Get(pMemberInfo);
//         if ( pMember )
//         {
//           _Right = XActor::GetMapInsID(&pMember->XActor, &v14);  // member MapInsID
//           v6 = std::list<CBattleZone *>::size(this);
//           MapInsID = XActor::GetMapInsID((XActor *)&v6[3].m_ChunkSizeTempMemOfs, &v13);  // owner MapInsID
//           if ( std::_Tree_const_iterator::operator==(MapInsID, _Right)  // same map
//             && CMover::FindBuffByEffectType(&pMember->CMoverEx, 1, 0) == -1 )  // no revive buff
//           {
//             return 1;  // found member needing revive
//           }
//         }
//       }
//     }
//   }
//   return 0;
// }
bool CGocForce::NeedReviveBuffUser() {
    if (!IsForce()) {
        return false;
    }

    // IDA verified: 遍历所有成员检查是否需要复活 Buff
    // TODO: 需要 CForce 成员迭代器和 CMover::FindBuffByEffectType

    // 伪代码逻辑:
    // UXMapID ownerMapID = GetOwner()->GetMapInsID();
    //
    // for (auto& member : m_pForce->GetMembers()) {
    //     CUser* pMember = member->GetUser();
    //     if (!pMember) continue;
    //
    //     // 检查 MapInsID 是否与 owner 相同
    //     UXMapID memberMapID = pMember->GetMapInsID();
    //     if (memberMapID != ownerMapID) continue;
    //
    //     // 检查成员是否没有复活 Buff (effect type 1)
    //     if (pMember->FindBuffByEffectType(1) == -1) {
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
//   __int64 v4; // [rsp+40h] [rbp-18h]
//
//   v4 = -2;
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
// 获取 Force 成员列表
// void __fastcall CGocForce::GetForceMember(CGocParty *this, CUser *pKiller, std::vector<CForceMember*> *vecExpMember, bool bAllMemeber)
// {
//   if ( m_pForce valid )
//   {
//     std::_Tree::begin(&it);
//     while ( !std::_Tree_const_iterator::operator!=(&it, &end) )
//     {
//       pMember = std::_Tree_iterator::operator->(&it)->second.__vftable;
//       if ( bAllMemeber )
//       {
//         std::vector::push_back(vecExpMember, &pMember);
//         goto next;
//       }
//       pUser = std::tr1::_Ptr_base::_Get(pMember);
//       if ( pUser )
//       {
//         if ( !CMover::IsDie(&pUser->CMoverEx) )
//         {
//           rhs = XActor::GetMapInsID(&pUser->XActor, &v19);
//           MapInsID = XActor::GetMapInsID(&pKiller->XActor, &v18);
//           if ( !UXMapID::operator!=(MapInsID, rhs) )  // same map
//           {
//             v22 = VObjectComponentCollection::Count(pMember);  // member ActorID
//             QuestID = CQuestCondition::GetQuestID(v10);  // killer ActorID
//             if ( v22 == QuestID || CUser::IsMaze(pKiller) )
//               break;  // add member
//             v2 = VisObject3D_cl::GetPosition(&pUser->CMoverEx);
//             Position = VisObject3D_cl::GetPosition(&pKiller->CMoverEx);
//             fDistance = hkvVec3::Dist(Position, v2);
//             if ( fDistance < 5000.0 )  // within 5000 units
//               std::vector::push_back(vecExpMember, &pMember);
//           }
//         }
//       }
// next:
//       std::_Tree_iterator::operator++(&it);
//     }
//   }
// }
void CGocForce::GetForceMember(CUser* pUser, std::vector<CForceMember*>& vecMember, bool bIncludeOffline) {
    vecMember.clear();
    if (!IsForce()) {
        return;
    }

    // IDA verified: 遍历 m_pForce 成员
    // TODO: 需要 CForce 成员迭代器和 CForceMember 结构

    // 伪代码逻辑:
    // for (auto& member : m_pForce->GetMembers()) {
    //     if (bIncludeOffline) {
    //         vecMember.push_back(member);
    //         continue;
    //     }
    //
    //     CUser* pMemberUser = member->GetUser();
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
    //     if (member->GetActorID() == pUser->GetActorID() || pUser->IsMaze()) {
    //         vecMember.push_back(member);
    //         continue;
    //     }
    //
    //     // 检查距离 < 5000
    //     float distance = hkvVec3::Dist(pMemberUser->GetPosition(), pUser->GetPosition());
    //     if (distance < 5000.0f) {
    //         vecMember.push_back(member);
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
    if (!IsForce()) {
        return 0;
    }
    // IDA verified: 返回 Force 成员数量
    return static_cast<std::uint8_t>(m_pForce->GetMemberCount());
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
    if (!IsForce()) {
        return 0;
    }
    // IDA verified: 返回 Force 队长 ID (GetCurID)
    // TODO: 需要确认 CForce 是否有 GetCurID 或类似方法
    // 可能是第一个成员或者有专门的 master ID 存储
    // 暂时返回 0，需要进一步确认 CForce 结构
    return 0;
}

// IDA: ?SetExp@CGocForce@@QEAAXPEAVCUser@@MH@Z @ 0x140083A30
// 经验分配逻辑
// void __fastcall CGocForce::SetExp(CGocForce *this, CUser *pKiller, float fExp, char nMonsterLv)
// {
//   if ( m_pForce valid )
//   {
//     v4 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pForce);
//     nLevelGap = CParty::GetLavelGap(v4);  // 等级差距
//     pTBPartyExpLevel = XResourceMgr::GetTB_PARTYEXP_LEVEL(&v5->m_xResourceMgr, nLevelGap);
//     if ( pTBPartyExpLevel )
//     {
//       fTotalExp = fExp * pTBPartyExpLevel->LevelInterval_Value;  // 等级间隔加成
//       v6 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pForce);
//       cMonsterInterval = CLogicThreadManager::GetCommonThreadIndex(v6) - nMonsterLv;
//       pTBPartyExpMob = XResourceMgr::GetTB_PARTYEXP_MOB(&v7->m_xResourceMgr, cMonsterInterval);
//       if ( pTBPartyExpMob )
//       {
//         fTotalExpa = fTotalExp * pTBPartyExpMob->Mob_IntervalValue;  // 怪物等级加成
//         std::vector::vector(&vecExpMember);
//         CGocForce::GetForceMember(this, pKiller, &vecExpMember, 0);  // 获取成员列表
//         for ( auto& it : vecExpMember )
//         {
//           pMember = *it;
//           pUser = std::tr1::_Ptr_base::_Get(pMember);
//           if ( pUser )
//           {
//             fForceExp = fTotalExpa;
//             CMover::GetGOC<CGocAttribute>(&pUser->CMoverEx, &pAttr, 0);
//             fRate = CGocAttribute::GetSpecialEffect(pAttr, EFFECT_SPECIAL_EXP_ADD_RAT) * 0.01;  // 特殊效果加成
//             CMover::GetGOC<CGocBooster>(&pUser->CMoverEx, &v33, 0);
//             TotalRate = CGocBooster::GetTotalRate(v33, eBooster_Effect_IncExp);  // Booster 加成
//             fRate = fRate + TotalRate;
//             fLeagueRate = 0.0;
//             if ( CUser::IsLeagueSkill(pUser, 4) )  // League 技能加成
//               fLeagueRate = CUser::GetLeagueSkillEffectValue(pUser, 4) * 0.01;
//             fBonus = fForceExp * (fRate + fLeagueRate);
//             fForceExp = fForceExp + fBonus;  // 最终经验
//             CGocAttribute::SetExp(v14, fForceExp, fBonus, 1);  // 设置经验
//           }
//         }
//         std::vector::~vector(&vecExpMember);
//       }
//     }
//   }
// }
void CGocForce::SetExp(CUser* pUser, float fExp, int nExpType) {
    if (!IsForce()) {
        return;
    }

    // IDA verified: 复杂的经验分配逻辑
    // TODO: 需要 XResourceMgr、TB_PARTYEXP_LEVEL、TB_PARTYEXP_MOB 表

    // 伪代码逻辑:
    // 1. 计算等级差距 (LevelGap)
    // auto pForce = m_pForce.get();
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
// bool __fastcall CGocForce::IsMember(CGocParty *this, XActor *pMember)
// {
//   VBitmask *v2; // rax
//   CParty *v3; // rax
//   _BYTE v5[4]; // [rsp+20h] [rbp-18h] BYREF
//   unsigned int dwActor; // [rsp+24h] [rbp-14h]
//
//   if ( std::tr1::shared_ptr::operator int std::_Bool_struct::*(m_pParty) == -1 )
//     return 0;
//   v2 = (VBitmask *)pMember->GetActorID(pMember, v5);  // get actor ID
//   dwActor = CQuestCondition::GetQuestID(v2);  // extract actor ID
//   v3 = (CParty *)std::tr1::shared_ptr<CForce>::operator->(&this->m_pParty);
//   return CParty::IsMember(v3, dwActor);  // check if member
// }
bool CGocForce::IsMember(XActor* pActor) const {
    if (!IsForce() || !pActor) {
        return false;
    }
    // IDA verified: 获取 ActorID 并检查是否是 Force 成员
    // TODO: 需要 XActor::GetActorID 方法
    // 伪代码:
    // std::uint32_t dwActorID = pActor->GetActorID();
    // return m_pForce->IsMember(dwActorID);
    return false;
}

// IDA: ?UpdatePartyBooster@CGocForce@@QEAAXXZ @ 0x140084EE0
// void __fastcall CGocForce::UpdatePartyBooster(CGocForce *this)
// {
//   CForce *v1; // rax
//
//   if ( std::tr1::shared_ptr::operator int std::_Bool_struct::*(m_pForce) != -1 )
//   {
//     v1 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pForce);
//     CForce::UpdateForceBooster(v1, 0);  // 更新 Force 增益
//   }
// }
void CGocForce::UpdatePartyBooster() {
    if (!IsForce()) {
        return;
    }
    // IDA verified: 调用 CForce::UpdateForceBooster(0)
    // TODO: 需要 CForce::UpdateForceBooster 方法
    // m_pForce->UpdateForceBooster(0);
}

// IDA: ?GetFamilyID@CGocForce@@SAHXZ @ 0x140039030
// __int64 __fastcall CGocForce::GetFamilyID()
// {
//   return 22;
// }
int CGocForce::GetFamilyID() {
    // IDA: 直接返回常量 22
    return 22;
}
