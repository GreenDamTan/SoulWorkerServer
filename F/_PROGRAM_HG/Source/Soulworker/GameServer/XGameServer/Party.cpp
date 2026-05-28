// Party.cpp
// CGocParty implementation
// 对齐 IDA GameServer.exe

#include "Soulworker/GameServer/XGameServer/Party.h"

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

// 前置声明 - 避免循环依赖
class CUser;
class CMover;

// ============================================================================
// CGocParty Implementation
// ============================================================================

// IDA: ??0CGocParty@@QEAA@XZ @ 0x14010AFF0
CGocParty::CGocParty()
    : GOComponent()
    , m_pParty(nullptr)
    , m_biMatchingDate(0) {
}

// IDA: ??1CGocParty@@UEAA@XZ @ 0x14010B080
CGocParty::~CGocParty() {
    m_pParty.reset();
}

// IDA: ?Init@CGocParty@@QEAAXXZ @ 0x14010B0D0
void CGocParty::Init() {
    m_pParty.reset();
    m_biMatchingDate = 0;
}

// IDA: ?Clear@CGocParty@@QEAAXXZ @ 0x14010B100
void CGocParty::Clear() {
    // 对齐 IDA: 如果有队伍，获取当前用户的 ActorID 并从队伍中移除
    if (IsParty()) {
        // TODO: 需要从 CMover 获取 ActorID
        // 当前简化实现：直接重置
        m_pParty.reset();
    }
    m_biMatchingDate = 0;
}

// IDA: ?IsParty@CGocParty@@QEAA_NXZ @ 0x140091E20
bool CGocParty::IsParty() const {
    return m_pParty != nullptr;
}

// IDA: ?GetPartyID@CGocParty@@QEAAHXZ @ 0x14009F760
std::uint32_t CGocParty::GetPartyID() const {
    if (!IsParty()) {
        return 0;
    }
    // TODO: Call CParty::GetPartyID when CParty is fully defined
    return 0;
}

// IDA: ?IsMember@CGocParty@@QEAA_NK@Z @ 0x1400831B0
bool CGocParty::IsMember(std::uint32_t dwActorID) const {
    if (!IsParty()) {
        return false;
    }
    // TODO: Need to check member in CParty
    // Current stub implementation
    return false;
}

// IDA: ?IsFull@CGocParty@@QEAA_NXZ @ 0x14010D370
bool CGocParty::IsFull() const {
    if (!IsParty()) {
        return false;
    }
    // TODO: Call CParty::GetUserCount when CParty is fully defined
    return false;
}

// IDA: ?IsMaster@CGocParty@@QEAA_NXZ @ 0x1403104B0
bool CGocParty::IsMaster() const {
    if (!IsParty()) {
        return false;
    }
    // TODO: 需要从 CMover 获取当前 ActorID 并与 MasterID 比较
    // 当前简化实现
    return false;
}

// IDA: ?GetParty@CGocParty@@QEAA?AV?$shared_ptr@VCParty@@@tr1@std@@XZ @ 0x1402D3AF0
std::shared_ptr<CParty> CGocParty::GetParty() const {
    return m_pParty;
}

// IDA: ?SetParty@CGocParty@@QEAAXAEAV?$shared_ptr@VCParty@@@tr1@std@@@Z @ 0x140082F30
void CGocParty::SetParty(std::shared_ptr<CParty> pParty) {
    m_pParty = pParty;
}

// IDA: ?Send@CGocParty@@QEAAXAEAVXSendPacket@@@Z @ 0x14010B1A0
void CGocParty::Send(XSendPacket& sendPacket) {
    if (!IsParty()) {
        return;
    }
    // TODO: 需要调用 CForce::Send 或类似方法广播给所有成员
    // 当前简化实现
}

// IDA: ?SendPartyInfo@CGocParty@@QEAAXE@Z @ 0x14010B1F0
void CGocParty::SendPartyInfo(std::uint8_t byUpdateType) {
    if (!IsParty()) {
        return;
    }
    // TODO: 构造 PS_PARTY_INFO 并发送给当前用户
    // 当前简化实现
}

// IDA: ?ShowMyPartyInfo@CGocParty@@QEAAXXZ @ 0x14010BB30
void CGocParty::ShowMyPartyInfo() {
    if (!IsParty()) {
        return;
    }
    // TODO: 发送队伍信息给当前用户
    // 当前简化实现
}

// IDA: ?SetHP@CGocParty@@QEAAXH@Z @ 0x14010B540
void CGocParty::SetHP(int nHP) {
    if (!IsParty()) {
        return;
    }
    // TODO: 更新队伍成员的 HP 信息并广播
    // 当前简化实现
}

// IDA: ?SetMaxHP@CGocParty@@QEAAXH@Z @ 0x14010B600
void CGocParty::SetMaxHP(int nMaxHP) {
    if (!IsParty()) {
        return;
    }
    // TODO: 更新队伍成员的最大 HP 信息并广播
    // 当前简化实现
}

// IDA: ?SetLevel@CGocParty@@QEAAXH@Z @ 0x14010B6C0
void CGocParty::SetLevel(int nLevel) {
    if (!IsParty()) {
        return;
    }
    // TODO: 更新队伍成员的等级信息并广播
    // 当前简化实现
}

// IDA: ?SetAwaken@CGocParty@@QEAAXE@Z @ 0x14010B740
void CGocParty::SetAwaken(std::uint8_t byAwaken) {
    if (!IsParty()) {
        return;
    }
    // TODO: 更新队伍成员的觉醒状态并广播
    // 当前简化实现
}

// IDA: ?SetProfilePhoto@CGocParty@@QEAAXK@Z @ 0x14010B7C0
void CGocParty::SetProfilePhoto(std::uint32_t dwPhotoID) {
    if (!IsParty()) {
        return;
    }
    // TODO: 更新队伍成员的头像并广播
    // 当前简化实现
}

// IDA: ?SetMapID@CGocParty@@QEAAXHHTUXMapID@@@Z @ 0x14010BC20
void CGocParty::SetMapID(int nMapID, const UXMapID& uxMapID) {
    if (!IsParty()) {
        return;
    }
    // TODO: 更新队伍成员的地图信息
    // 当前简化实现
}

// IDA: ?IsMatchingDate@CGocParty@@QEAA_NXZ @ 0x14010D3C0
bool CGocParty::IsMatchingDate() const {
    return m_biMatchingDate != 0;
}

// IDA: ?SetMatchingDate@CGocParty@@QEAAX_J@Z @ 0x140085030
void CGocParty::SetMatchingDate(std::int64_t biDate) {
    m_biMatchingDate = biDate;
}

// IDA: ?GetMemberInMap@CGocParty@@QEAAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z @ 0x14010D130
void CGocParty::GetMemberInMap(std::vector<CMover*>& vecMover) {
    vecMover.clear();
    if (!IsParty()) {
        return;
    }
    // TODO: 遍历队伍成员，获取同一地图的 CMover 指针
    // 当前简化实现
}

// IDA: ?ChangePartyMemberName@CGocParty@@QEAAXAEAUPS_CHANGE_NAME@@@Z @ 0x14010D460
void CGocParty::ChangePartyMemberName(PS_CHANGE_NAME& stChangeName) {
    if (!IsParty()) {
        return;
    }
    // TODO: 更新队伍成员名称并广播
    // 当前简化实现
}

// IDA: ?IsExistDeadUser@CGocParty@@QEAA_NXZ @ 0x140083200
bool CGocParty::IsExistDeadUser() const {
    if (!IsParty()) {
        return false;
    }
    // TODO: 检查队伍中是否有死亡成员
    // 当前简化实现
    return false;
}

// IDA: ?Logout@CGocParty@@QEAAXXZ @ 0x14010B840
void CGocParty::Logout() {
    if (!IsParty()) {
        return;
    }
    // TODO: 处理登出逻辑，通知队伍成员
    // 当前简化实现
}

// IDA: ?Leave@CGocParty@@QEAAXXZ @ 0x14010C250
void CGocParty::Leave() {
    if (!IsParty()) {
        return;
    }
    // TODO: 发送离开队伍请求到 RelayServer
    // 当前简化实现
}

// IDA: ?KickOut@CGocParty@@QEAA_NKPEAVCUser@@@Z @ 0x14010BCC0
bool CGocParty::KickOut(std::uint32_t dwActorID, CUser* pUser) {
    if (!IsParty()) {
        return false;
    }
    // TODO: 发送踢出请求到 RelayServer
    // 当前简化实现
    return false;
}

// IDA: ?ChangeMaster@CGocParty@@QEAAXK@Z @ 0x14010C540
void CGocParty::ChangeMaster(std::uint32_t dwMaster) {
    if (!IsParty()) {
        return;
    }
    // TODO: 发送更改队长请求到 RelayServer
    // 当前简化实现
}

// IDA: ?SaveRecode@CGocParty@@QEAAXXZ @ 0x14010B340
void CGocParty::SaveRecode() {
    if (!IsParty()) {
        return;
    }
    // TODO: 保存队伍记录到数据库
    // 当前简化实现
}

// IDA: ?UpdatePartyBooster@CGocParty@@QEAAXXZ @ 0x14010C8F0
void CGocParty::UpdatePartyBooster() {
    if (!IsParty()) {
        return;
    }
    // TODO: 更新队伍增益效果
    // 当前简化实现
}

// IDA: ?GetFamilyID@CGocParty@@SAHXZ @ 0x140039000
int CGocParty::GetFamilyID() {
    // TODO: 返回家族ID
    // 当前简化实现
    return 0;
}
