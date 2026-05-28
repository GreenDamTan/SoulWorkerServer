// Party.cpp
// CGocParty implementation
// 对齐 IDA GameServer.exe

#include "Soulworker/GameServer/XGameServer/Party.h"

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

// 前置声明 - 避免循环依赖
class CUser;
class CMover;
class CParty;
class CPartyMember;

// ============================================================================
// CGocParty Implementation
// ============================================================================

// IDA: ??0CGocParty@@QEAA@XZ @ 0x14010AFF0
// 对齐 IDA: 调用 GOComponent 构造函数，设置 vtable，初始化 shared_ptr
CGocParty::CGocParty()
    : GOComponent()
    , m_pParty(nullptr)
    , m_biMatchingDate(0) {
}

// IDA: ??1CGocParty@@UEAA@XZ @ 0x14010B080
// 对齐 IDA: 设置 vtable，调用 shared_ptr 析构函数，调用 GOComponent 析构函数
CGocParty::~CGocParty() {
    m_pParty.reset();
}

// IDA: ?Init@CGocParty@@QEAAXXZ @ 0x14010B0D0
// 对齐 IDA: reset shared_ptr，设置 m_biMatchingDate = 0
void CGocParty::Init() {
    m_pParty.reset();
    m_biMatchingDate = 0;
}

// IDA: ?Clear@CGocParty@@QEAAXXZ @ 0x14010B100
// 对齐 IDA: 如果有队伍，获取 ActorID 并调用 CParty::RemoveMember，然后 reset
void CGocParty::Clear() {
    if (IsParty()) {
        // TODO: 需要从 CMover 获取 ActorID 并调用 CParty::RemoveMember
        // IDA 反编译显示需要:
        // 1. 从 this 获取 CMover 对象
        // 2. 获取 ActorID
        // 3. 调用 m_pParty->RemoveMember(dwActorID)
        // 当前简化实现：直接重置
        m_pParty.reset();
    }
    m_biMatchingDate = 0;
}

// IDA: ?IsParty@CGocParty@@QEAA_NXZ @ 0x140091E20
// 对齐 IDA: 检查 shared_ptr operator bool (检查是否 != -1)
bool CGocParty::IsParty() const {
    return m_pParty != nullptr;
}

// IDA: ?GetPartyID@CGocParty@@QEAAHXZ @ 0x14009F760
// 对齐 IDA: 如果没有队伍返回 0，否则调用 CParty::GetPartyID
std::uint32_t CGocParty::GetPartyID() const {
    if (!IsParty()) {
        return 0;
    }
    // IDA: v1 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pParty);
    //      return CParty::GetPartyID(v1);
    // 需要调用 CParty::GetPartyID
    // TODO: 实现 CParty::GetPartyID 调用
    return 0;
}

// IDA: ?IsMember@CGocParty@@QEAA_NK@Z @ 0x1400831B0
// 对齐 IDA: 如果没有队伍返回 false，否则调用 CParty::IsMember
bool CGocParty::IsMember(std::uint32_t dwActorID) const {
    if (!IsParty()) {
        return false;
    }
    // IDA: v2 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pParty);
    //      return CParty::IsMember(v2, dwActorID);
    // TODO: 实现 CParty::IsMember 调用
    return false;
}

// IDA: ?IsFull@CGocParty@@QEAA_NXZ @ 0x14010D370
// 对齐 IDA: 如果有队伍且 GetUserCount() == 4 返回 true
bool CGocParty::IsFull() const {
    if (!IsParty()) {
        return false;
    }
    // IDA: v1 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pParty);
    //      if (CParty::GetUserCount(v1) == 4) return true;
    // TODO: 实现 CParty::GetUserCount 调用
    return false;
}

// IDA: ?IsMaster@CGocParty@@QEAA_NXZ @ 0x1403104B0
// 对齐 IDA: 获取当前 ActorID，与队伍 MasterID 比较
bool CGocParty::IsMaster() const {
    if (!IsParty()) {
        return false;
    }
    // IDA 反编译显示:
    // 1. 从 this 获取 CMover 对象
    // 2. 获取当前 ActorID (QuestID)
    // 3. 调用 CWayPoint::GetCurID (获取 MasterID)
    // 4. 比较 ActorID == MasterID
    // TODO: 需要完整实现
    return false;
}

// IDA: ?GetParty@CGocParty@@QEAA?AV?$shared_ptr@VCParty@@@tr1@std@@XZ @ 0x1402D3AF0
// 对齐 IDA: 返回 m_pParty 的拷贝
std::shared_ptr<CParty> CGocParty::GetParty() const {
    return m_pParty;
}

// IDA: ?SetParty@CGocParty@@QEAAXAEAV?$shared_ptr@VCParty@@@tr1@std@@@Z @ 0x140082F30
// 注意: IDA 反编译显示该地址是 ST_POST_DATA 构造函数，需要重新确认
void CGocParty::SetParty(std::shared_ptr<CParty> pParty) {
    m_pParty = pParty;
}

// IDA: ?Send@CGocParty@@QEAAXAEAVXSendPacket@@@Z @ 0x14010B1A0
// 对齐 IDA: 如果有队伍，调用 CForce::Send (实际上是 CParty::Send)
void CGocParty::Send(XSendPacket& sendPacket) {
    if (!IsParty()) {
        return;
    }
    // IDA: v2 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pParty);
    //      CForce::Send(v2, sendPacket, nullptr);
    // TODO: 实现 CForce::Send / CParty::Send 调用
}

// IDA: ?SendPartyInfo@CGocParty@@QEAAXE@Z @ 0x14010B1F0
// 对齐 IDA: 构造 PS_PARTY_INFO 并发送给当前用户
void CGocParty::SendPartyInfo(std::uint8_t byUpdateType) {
    if (!IsParty()) {
        return;
    }
    // IDA 反编译显示:
    // 1. 构造 PS_PARTY_INFO stPartyInfo
    // 2. 调用 CParty::GetPartyInfo 获取队伍信息
    // 3. 设置 stPartyInfo.byUpdateType = byUpdateType
    // 4. 设置 stPartyInfo.byPartyType = CForce::GetForceType
    // 5. 构造 XSendPacket(0x12, 9)
    // 6. 发送数据包
    // TODO: 完整实现
}

// IDA: ?ShowMyPartyInfo@CGocParty@@QEAAXXZ @ 0x14010BB30
// 对齐 IDA: 如果没有队伍则记录日志，否则调用 CParty::ShowPartyInfo
void CGocParty::ShowMyPartyInfo() {
    if (!IsParty()) {
        // IDA: LogHelper::LogDebug("game.party", "<PARTY_INFO> Dont Have And Party ( %d )", ...)
        return;
    }
    // IDA: v1 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pParty);
    //      CParty::ShowPartyInfo(v1);
    // TODO: 实现 CParty::ShowPartyInfo 调用
}

// IDA: ?SetHP@CGocParty@@QEAAXH@Z @ 0x14010B540
// 对齐 IDA: 获取 ActorID 和 MapInsID，调用 CParty::SetMemberHP
void CGocParty::SetHP(int nHP) {
    if (!IsParty()) {
        return;
    }
    // IDA 反编译显示:
    // 1. 从 this 获取 CMover 对象
    // 2. 获取 MapInsID (XActor::GetMapInsID)
    // 3. 获取 ActorID
    // 4. 调用 CParty::SetMemberHP(m_pParty, dwActorID, MapInsID, nHP)
    // TODO: 完整实现
}

// IDA: ?SetMaxHP@CGocParty@@QEAAXH@Z @ 0x14010B600
// 对齐 IDA: 获取 ActorID 和 MapInsID，调用 CParty::SetMemberMaxHP
void CGocParty::SetMaxHP(int nMaxHP) {
    if (!IsParty()) {
        return;
    }
    // IDA 反编译类似 SetHP，调用 CParty::SetMemberMaxHP
    // TODO: 完整实现
}

// IDA: ?SetLevel@CGocParty@@QEAAXH@Z @ 0x14010B6C0
// 对齐 IDA: 获取 ActorID，调用 CParty::SetMemberLevel
void CGocParty::SetLevel(int nLevel) {
    if (!IsParty()) {
        return;
    }
    // IDA 反编译显示:
    // 1. 从 this 获取 CMover 对象
    // 2. 获取 ActorID
    // 3. 调用 CParty::SetMemberLevel(m_pParty, dwActorID, nLevel)
    // TODO: 完整实现
}

// IDA: ?SetAwaken@CGocParty@@QEAAXE@Z @ 0x14010B740
// 对齐 IDA: 获取 ActorID，调用 CParty::SetMemberAwaken
void CGocParty::SetAwaken(std::uint8_t byAwaken) {
    if (!IsParty()) {
        return;
    }
    // IDA 反编译类似 SetLevel，调用 CParty::SetMemberAwaken
    // TODO: 完整实现
}

// IDA: ?SetProfilePhoto@CGocParty@@QEAAXK@Z @ 0x14010B7C0
// 对齐 IDA: 获取 ActorID，调用 CParty::SetMemberProfilePhoto
void CGocParty::SetProfilePhoto(std::uint32_t dwPhotoID) {
    if (!IsParty()) {
        return;
    }
    // IDA 反编译类似 SetLevel，调用 CParty::SetMemberProfilePhoto
    // TODO: 完整实现
}

// IDA: ?SetMapID@CGocParty@@QEAAXHHTUXMapID@@@Z @ 0x14010BC20
// 对齐 IDA: 获取 ActorID，调用 CParty::SetMemberMapID
void CGocParty::SetMapID(int nMapID, const UXMapID& uxMapID) {
    if (!IsParty()) {
        return;
    }
    // IDA 反编译显示:
    // 1. 从 this 获取 CMover 对象
    // 2. 获取 ActorID
    // 3. 调用 CParty::SetMemberMapID(m_pParty, dwActorID, nMapID, nChannel, uxMapID)
    // 注意: IDA 显示参数为 (nMapID, nChannel, uxMapID)，nChannel 可能从 uxMapID 提取
    // TODO: 完整实现
}

// IDA: ?IsMatchingDate@CGocParty@@QEAA_NXZ @ 0x14010D3C0
// 对齐 IDA: 检查 m_biMatchingDate 是否非零，且 m_biMatchingDate + 60 >= 当前时间
bool CGocParty::IsMatchingDate() const {
    if (!m_biMatchingDate) {
        return false;
    }
    // IDA: v3 = this->m_biMatchingDate + 60;
    //      v2 = TXSingleton<XGameServer>::Instance();
    //      return v3 >= XGameServer::GetCurDate(v2);
    // 需要获取 XGameServer 的当前时间进行比较
    // TODO: 实现 XGameServer::GetCurDate 调用
    return m_biMatchingDate != 0;
}

// IDA: ?SetMatchingDate@CGocParty@@QEAAX_J@Z @ 0x140085030
// 对齐 IDA: 设置 m_biMatchingDate，根据值设置用户的 MatchingState
void CGocParty::SetMatchingDate(std::int64_t biDate) {
    m_biMatchingDate = biDate;

    // IDA 反编译显示:
    // 如果 biDate <= 0:
    //   1. 获取 CUser 对象 (RTDynamicCast)
    //   2. 调用 CUser::SetMatchingState(user, 0)
    //   3. 获取 CGocInventory 组件
    //   4. 调用 CGocInventory::SetMazeNeedItemID(inven, 0)
    // 否则:
    //   1. 获取 CUser 对象
    //   2. 调用 CUser::SetMatchingState(user, 1)
    // TODO: 完整实现
}

// IDA: ?GetMemberInMap@CGocParty@@QEAAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z @ 0x14010D130
// 对齐 IDA: 遍历队伍成员，获取同一地图的 CMover 指针
void CGocParty::GetMemberInMap(std::vector<CMover*>& vecMover) {
    vecMover.clear();
    if (!IsParty()) {
        return;
    }
    // IDA 反编译显示复杂的遍历逻辑:
    // 1. 遍历 m_pParty->m_mapPartyMember
    // 2. 对每个 CPartyMember，获取 CUser 对象
    // 3. 检查是否在同一地图 (TBMapID 比较)
    // 4. 检查 ThreadLocalData::IsThreadArea
    // 5. 添加到 vecMover
    // TODO: 完整实现
}

// IDA: ?ChangePartyMemberName@CGocParty@@QEAAXAEAUPS_CHANGE_NAME@@@Z @ 0x14010D460
// 对齐 IDA: 调用 CParty::ChangeMemberName
void CGocParty::ChangePartyMemberName(PS_CHANGE_NAME& stChangeName) {
    if (!IsParty()) {
        return;
    }
    // IDA: v2 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pParty);
    //      CParty::ChangeMemberName(v2, psChangeName);
    // TODO: 实现 CParty::ChangeMemberName 调用
}

// IDA: ?IsExistDeadUser@CGocParty@@QEAA_NXZ @ 0x140083200
// 对齐 IDA: 遍历队伍成员，检查是否有死亡成员
bool CGocParty::IsExistDeadUser() const {
    if (!IsParty()) {
        return false;
    }
    // IDA 反编译显示遍历逻辑:
    // 1. 遍历 m_pParty->m_mapPartyMember
    // 2. 对每个 CPartyMember，获取 CUser 对象
    // 3. 检查是否在同一地图
    // 4. 调用 CMover::IsDie 检查是否死亡
    // 5. 如果有死亡成员返回 true
    // TODO: 完整实现
    return false;
}

// IDA: ?Logout@CGocParty@@QEAAXXZ @ 0x14010B840
// 对齐 IDA: 处理登出，发送 ST_UPDATE_PARTY_MEMBER 到 RelayServer
void CGocParty::Logout() {
    if (!IsParty()) {
        // IDA: 即使没有队伍，也需要发送数据包通知
        // 构造 XSendPacket(0xF4, 0x21)，发送 ActorID 和 UAID
        return;
    }
    // IDA 反编译显示:
    // 1. 获取 ActorID
    // 2. 调用 CParty::GetMember 获取 CPartyMember
    // 3. 构造 ST_UPDATE_PARTY_MEMBER
    // 4. 调用 CPartyMember::Logout
    // 5. 构造 XSendPacket(0xF4, 5) 发送
    // 6. 通过 CCommunitySocket::SendCheck 发送
    // 7. 调用 CGocParty::Clear
    // TODO: 完整实现
}

// IDA: ?Leave@CGocParty@@QEAAXXZ @ 0x14010C250
// 对齐 IDA: 发送离开队伍请求到 RelayServer
void CGocParty::Leave() {
    if (!IsParty()) {
        return;
    }
    // IDA 反编译显示:
    // 1. 获取 CUser 对象
    // 2. 检查是否在 Maze 中且 MazeState == 2 (进行中)
    //    如果是则发送错误消息 (0x12, 5, 0xCF25)
    // 3. 构造 PS_PARTY_LEAVE
    // 4. 构造 XSendPacket(0xF4, 3) 发送
    // 5. 通过 CCommunitySocket::SendCmd 发送
    // TODO: 完整实现
}

// IDA: ?KickOut@CGocParty@@QEAA_NKPEAVCUser@@@Z @ 0x14010BCC0
// 对齐 IDA: 发送踢出请求到 RelayServer
bool CGocParty::KickOut(std::uint32_t dwActorID, CUser* pUser) {
    if (!IsParty()) {
        // IDA: 发送错误消息 (0x12, 4, 0xCF0E) - 没有队伍
        return false;
    }
    // IDA 反编译显示复杂的检查逻辑:
    // 1. 检查 dwActorID 是否等于自己的 ActorID (不能踢自己)
    // 2. 检查是否是队长 (IsMaster)
    // 3. 获取 CPartyMember
    // 4. 检查目标成员是否在队伍中
    // 5. 检查 Maze 类型
    // 6. 构造 PS_PARTY_LEAVE 并发送
    // TODO: 完整实现
    return false;
}

// IDA: ?ChangeMaster@CGocParty@@QEAAXK@Z @ 0x14010C540
// 对齐 IDA: 发送更改队长请求到 RelayServer
void CGocParty::ChangeMaster(std::uint32_t dwMaster) {
    if (!IsParty()) {
        return;
    }
    // IDA 反编译显示:
    // 1. 获取当前 ActorID
    // 2. 检查是否是队长 (IsMaster)
    // 3. 检查目标成员是否在队伍中
    // 4. 构造 PS_PARTY_CHANGE_MASTER
    // 5. 构造 XSendPacket(0xF4, 4) 发送
    // 6. 通过 CCommunitySocket::SendCmd 发送
    // TODO: 完整实现
}

// IDA: ?SaveRecode@CGocParty@@QEAAXXZ @ 0x14010B340
// 对齐 IDA: 保存队伍记录到数据库
void CGocParty::SaveRecode() {
    if (!IsParty()) {
        return;
    }
    // IDA 反编译显示:
    // 1. 获取 CGocRecode 组件
    // 2. 获取 MazeRecode 数据
    // 3. 获取 ActorID
    // 4. 调用 CForce::SetMazeRecode
    // TODO: 完整实现
}

// IDA: ?UpdatePartyBooster@CGocParty@@QEAAXXZ @ 0x14010C8F0
// 对齐 IDA: 调用 CParty::UpdatePartyBooster
void CGocParty::UpdatePartyBooster() {
    if (!IsParty()) {
        return;
    }
    // IDA: v1 = std::tr1::shared_ptr<CForce>::operator->(&this->m_pParty);
    //      CParty::UpdatePartyBooster(v1, 0);
    // TODO: 实现 CParty::UpdatePartyBooster 调用
}

// IDA: ?GetFamilyID@CGocParty@@SAHXZ @ 0x140039000
// 对齐 IDA: 返回固定值 8
int CGocParty::GetFamilyID() {
    // IDA: return 8;
    return 8;
}
