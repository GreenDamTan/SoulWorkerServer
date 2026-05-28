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
std::uint32_t CGocForce::GetForceID() const {
    if (!IsForce()) {
        return 0;
    }
    // TODO: Call CForce::GetForceID when CForce is fully defined
    return 0;
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
    // Force 最多 8 个成员
    // TODO: Call CForce::GetUserCount when CForce is fully defined
    // return m_pForce->GetUserCount() == 8;
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
    if (!IsForce()) {
        return false;
    }
    // IDA: Compare CForce::GetCurID() with dwUCID
    // TODO: return m_pForce->GetCurID() == dwUCID;
    return false;
}

// IsMaster - 检查当前玩家是否为队长
bool CGocForce::IsMaster() const {
    if (!IsForce()) {
        return false;
    }
    // TODO: 需要从 CMover 获取当前 ActorID 并与 MasterID 比较
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
void CGocForce::SendForceInfo(std::uint8_t byUpdateType) {
    if (!IsForce()) {
        return;
    }
    // TODO: 构造 PS_FORCE_INFO 并发送给当前用户
    // IDA: PS_PARTY_INFO::PS_PARTY_INFO(&stForceInfo);
    // IDA: CParty::GetPartyInfo(m_pForce.get(), &stForceInfo);
    // IDA: stForceInfo.byUpdateType = byUpdateType;
    // IDA: stForceInfo.byForceType = CForce::GetForceType(m_pForce.get());
    // IDA: Send to owner via CGocNetwork::Send
}

// ShowMyForceInfo - 显示我的 Force 信息
void CGocForce::ShowMyForceInfo() {
    if (!IsForce()) {
        return;
    }
    // TODO: 发送 Force 信息给当前用户
}

// IDA: ?SetHP@CGocForce@@QEAAXH@Z @ 0x140083970
// 获取 owner actor ID 和 MapInsID，调用 CForce::SetMemberHP
void CGocForce::SetHP(int nHP) {
    if (!IsForce()) {
        return;
    }
    // TODO:
    // 1. Get owner actor ID
    // 2. Get MapInsID
    // 3. Call m_pForce->SetMemberHP(dwActorID, uxMapID, nHP)
}

// IDA: ?SetMaxHP@CGocForce@@QEAAXH@Z @ 0x1400838B0
void CGocForce::SetMaxHP(int nMaxHP) {
    if (!IsForce()) {
        return;
    }
    // TODO: 类似 SetHP
}

// IDA: ?SetLevel@CGocForce@@QEAAXH@Z @ 0x140083730
void CGocForce::SetLevel(int nLevel) {
    if (!IsForce()) {
        return;
    }
    // TODO: Get owner actor ID, call m_pForce->SetMemberLevel(dwActorID, nLevel)
}

// IDA: ?SetAwaken@CGocForce@@QEAAXE@Z @ 0x1400837B0
void CGocForce::SetAwaken(std::uint8_t byAwaken) {
    if (!IsForce()) {
        return;
    }
    // TODO: Get owner actor ID, call m_pForce->SetMemberAwaken(dwActorID, byAwaken)
}

// IDA: ?SetProfilePhoto@CGocForce@@QEAAXK@Z @ 0x140083830
void CGocForce::SetProfilePhoto(std::uint32_t dwPhotoID) {
    if (!IsForce()) {
        return;
    }
    // TODO: Get owner actor ID, call m_pForce->SetMemberProfilePhoto(dwActorID, dwPhotoID)
}

// IDA: ?SetMapID@CGocForce@@QEAAXHHTUXMapID@@@Z @ 0x140083690
void CGocForce::SetMapID(int nMapID, int nChannel, const UXMapID& uxMapID) {
    if (!IsForce()) {
        return;
    }
    // TODO: Get owner actor ID, call m_pForce->SetMemberMapID(dwActorID, nMapID, nChannel, uxMapID)
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
void CGocForce::Logout() {
    if (!IsForce()) {
        return;
    }
    // TODO: 处理登出逻辑
    // IDA: 如果是 Party，发送 ST_UPDATE_FORCE_MEMBER 到 CommunitySocket
    // IDA: 否则发送简单的登出消息
    // IDA: 最后调用 Clear()
    Clear();
}

// IDA: ?Leave@CGocForce@@QEAAXXZ @ 0x140084480
// 发送 PS_FORCE_LEAVE 到 CommunitySocket
void CGocForce::Leave() {
    if (!IsForce()) {
        return;
    }
    // TODO: 发送离开 Force 请求到 RelayServer
    // IDA: 构造 PS_FORCE_LEAVE
    // IDA: 发送到 CommunitySocket via SendCmd
}

// IDA: ?KickOut@CGocForce@@QEAA_NKPEAVCUser@@@Z @ 0x1400846F0
// 复杂的踢出验证逻辑
bool CGocForce::KickOut(std::uint32_t dwActorID, CUser* pUser) {
    if (!IsForce()) {
        // IDA: SendErrorMessage(0x2E, 4, 0xCF72)
        return false;
    }
    // TODO: 完整的踢出验证逻辑
    // IDA: 1. 检查不能踢自己
    // IDA: 2. 检查是否是队长
    // IDA: 3. 检查目标成员是否存在
    // IDA: 4. 检查迷宫类型
    // IDA: 5. 发送 PS_FORCE_LEAVE 到 CommunitySocket
    return false;
}

// IDA: ?ChangeMaster@CGocForce@@QEAAXK@Z @ 0x140084C80
// 更改队长
void CGocForce::ChangeMaster(std::uint32_t dwMaster) {
    if (!IsForce()) {
        return;
    }
    // TODO: 发送更改队长请求到 RelayServer
    // IDA: 1. 检查当前是否是队长
    // IDA: 2. 检查新队长是否是成员
    // IDA: 3. 发送 PS_FORCE_CHANGE_MASTER 到 CommunitySocket
}

// IDA: ?LoadRecode@CGocForce@@QEAAXXZ @ 0x14010B430
// 从 CForce 加载迷宫记录到 CGocRecode
void CGocForce::LoadRecode() {
    if (!IsForce()) {
        return;
    }
    // TODO:
    // 1. Get CGocRecode component from owner
    // 2. Call m_pForce->GetMazeRecode(dwActorID, nRecode)
    // 3. Call CGocRecode::SetFullRecode(nRecode)
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
    if (!IsForce()) {
        return;
    }
    // TODO: Call m_pForce->UpdateForceBooster(0)
}

// IDA: ?UpdatePartyBoosterByCount@CGocForce@@QEAAXHH@Z @ 0x140084F30
// 根据成员数量更新增益
void CGocForce::UpdatePartyBoosterByCount(int nPartyCount, int nFriendCount) {
    // TODO:
    // 1. Get owner CUser
    // 2. Iterate TB_PARTYEXP_MEMBER table
    // 3. Find matching PartyCount and FriendCount
    // 4. Call CUser::ChangeBooster(eBooster_Type_Party, wBoosterID)
}

// IDA: ?ReserveReviveAll@CGocForce@@QEAAXKK@Z @ 0x140083350
// 预约复活所有成员
void CGocForce::ReserveReviveAll(std::uint32_t dwActorID, std::uint32_t dwID) {
    if (!IsForce()) {
        return;
    }
    // TODO: 复杂的复活逻辑
    // IDA: 1. Get owner CUser
    // IDA: 2. 构造复活数据包
    // IDA: 3. 如果 owner 死亡或 HP <= 0，设置预约复活
    // IDA: 4. 遍历所有 Force 成员，检查是否在同一地图
    // IDA: 5. 对符合条件的成员发送复活请求
}

// IDA: ?NeedReviveBuffUser@CGocForce@@QEAA_NXZ @ 0x14010C7A0
// 检查是否有成员需要复活 Buff
bool CGocForce::NeedReviveBuffUser() {
    if (!IsForce()) {
        return false;
    }
    // TODO: 遍历所有成员检查是否需要复活 Buff
    // IDA: 1. 遍历 m_pForce 成员
    // IDA: 2. 检查成员 MapInsID 是否与 owner 相同
    // IDA: 3. 检查成员是否没有复活 Buff (FindBuffByEffectType(1) == -1)
    // IDA: 4. 如果找到需要复活的成员返回 true
    return false;
}

// IDA: ?DeletePartyBoost@CGocForce@@QEAAXXZ @ 0x14010C940
// void __fastcall CGocForce::DeletePartyBoost(CGocParty *this)
// {
//   CMover *v1; // rax
//   CGocBooster *v2; // rax
//   std::tr1::shared_ptr<CGocBooster> pBooster; // [rsp+30h] [rbp-28h] BYREF
//
//   v1 = (CMover *)std::list<CBattleZone *>::size((VChunkLocker *)this);
//   CMover::GetGOC<CGocBooster>(v1, &pBooster, 0);
//   if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*((std::tr1::shared_ptr<CItemEquip> *)&pBooster) != -1 )
//   {
//     v2 = (CGocBooster *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&pBooster);
//     CGocBooster::ChangeBooster(v2, eBooster_Type_Party, 0, 0, 0);
//   }
// }
void CGocForce::DeletePartyBoost() {
    // TODO:
    // 1. Get CGocBooster component from owner
    // 2. Call CGocBooster::ChangeBooster(eBooster_Type_Party, 0, 0, 0)
}

// IDA: ?GetForceMember@CGocForce@@QEAAXPEAVCUser@@AEAV?$vector@PEAVCForceMember@@V?$allocator@PEAVCForceMember@@@std@@@std@@_N@Z @ 0x14010C9B0
// 获取 Force 成员列表
void CGocForce::GetForceMember(CUser* pUser, std::vector<CForceMember*>& vecMember, bool bIncludeOffline) {
    vecMember.clear();
    if (!IsForce()) {
        return;
    }
    // TODO: 遍历 m_pForce 成员
    // IDA: 1. 如果 bIncludeOffline，添加所有成员
    // IDA: 2. 否则只添加：
    //       - 未死亡的成员
    //       - 与 pUser 在同一地图的成员
    //       - 距离 < 5000 的成员（如果不是在迷宫中）
}

// IDA: ?GetForceUserCount@CGocForce@@QEAAEXZ @ 0x14010D330
// unsigned __int8 __fastcall CGocForce::GetForceUserCount(CGocParty *this)
// {
//   CParty *v1; // rax
//
//   if ( !CGocParty::IsParty(this) )
//     return 0;
//   v1 = (CParty *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&this->m_pParty);
//   return CParty::GetUserCount(v1);
// }
std::uint8_t CGocForce::GetForceUserCount() const {
    if (!IsForce()) {
        return 0;
    }
    // TODO: return m_pForce->GetUserCount();
    return 0;
}

// IDA: ?GetMasterID@CGocForce@@QEAAKXZ @ 0x14010D410
// __int64 __fastcall CGocForce::GetMasterID(CGocParty *this)
// {
//   VisRenderCollection_cl *v1; // rax
//
//   if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*((std::tr1::shared_ptr<CItemEquip> *)&this->m_pParty) == -1 )
//     return 0;
//   v1 = (VisRenderCollection_cl *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&this->m_pParty);
//   return CWayPoint::GetCurID(v1);
// }
std::uint32_t CGocForce::GetMasterID() const {
    if (!IsForce()) {
        return 0;
    }
    // TODO: return m_pForce->GetCurID();
    return 0;
}

// IDA: ?SetExp@CGocForce@@QEAAXPEAVCUser@@MH@Z @ 0x140083A30
// 经验分配逻辑
void CGocForce::SetExp(CUser* pUser, float fExp, int nExpType) {
    if (!IsForce()) {
        return;
    }
    // TODO: 复杂的经验分配逻辑
    // IDA: 1. 计算 LevelGap
    // IDA: 2. 从 TB_PARTYEXP_LEVEL 获取等级间隔加成
    // IDA: 3. 从 TB_PARTYEXP_MOB 获取怪物等级加成
    // IDA: 4. 获取所有成员列表
    // IDA: 5. 对每个成员计算最终经验：
    //       - 获取特殊效果加成
    //       - 获取 Booster 加成
    //       - 获取 League 技能加成
    //       - 设置经验到成员
}

// IDA: ?IsMember@CGocForce@@QEAA_NPEAVXActor@@@Z @ 0x14010BBB0
// bool __fastcall CGocForce::IsMember(CGocParty *this, XActor *pMember)
// {
//   VBitmask *v2; // rax
//   CParty *v3; // rax
//   _BYTE v5[4]; // [rsp+20h] [rbp-18h] BYREF
//   unsigned int dwActor; // [rsp+24h] [rbp-14h]
//
//   if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*((std::tr1::shared_ptr<CItemEquip> *)&this->m_pParty) == -1 )
//     return 0;
//   v2 = (VBitmask *)pMember->GetActorID(pMember, v5);
//   dwActor = CQuestCondition::GetQuestID(v2);
//   v3 = (CParty *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&this->m_pParty);
//   return CParty::IsMember(v3, dwActor);
// }
bool CGocForce::IsMember(XActor* pActor) const {
    if (!IsForce() || !pActor) {
        return false;
    }
    // TODO:
    // 1. Get ActorID from pActor
    // 2. Call m_pForce->IsMember(dwActorID)
    return false;
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
