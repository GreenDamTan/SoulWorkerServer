// GocLeague.cpp
// CGocLeague implementation
// 对齐 IDA GameServer.exe

#include "GocLeague.h"
#include "GOComponent.h"
#include <cstring>

// 前置声明 - 避免循环依赖
class CUser;
class CLeague;
class XGameServer;
class XSendPacket;

// ============================================================================
// CGocLeague Implementation
// ============================================================================

// IDA: ??0CGocLeague@@QEAA@XZ @ 0x1400FA870
// IDA反编译:
//   GOComponent::GOComponent(this);
//   this->__vftable = (CGocLeague_vtbl *)&CGocLeague::`vftable';
//   ST_LEAGUE_MEMBER::ST_LEAGUE_MEMBER(&this->m_stLeagueMember);
//   return this;
CGocLeague::CGocLeague()
    : GOComponent()
    , m_stLeagueMember()
    , m_dwInviteActorID(0) {
    // IDA: vtable 在构造函数中自动设置
}

// IDA: ??1CGocLeague@@UEAA@XZ @ 0x1400FA8F0
// IDA反编译:
//   this->__vftable = (CGocLeague_vtbl *)&CGocLeague::`vftable';
//   GOComponent::~GOComponent(this);
CGocLeague::~CGocLeague() {
    // IDA: 仅设置vtable后调用父类析构函数
}

// IDA: ?Init@CGocLeague@@QEAAXXZ @ 0x1400FA920
// IDA反编译:
//   memset(&this->m_stLeagueMember, 0, sizeof(this->m_stLeagueMember));
//   this->m_dwInviteActorID = 0;
void CGocLeague::Init() {
    // IDA: 使用 memset 清零结构体
    std::memset(&m_stLeagueMember, 0, sizeof(m_stLeagueMember));
    m_dwInviteActorID = 0;
}

// Clear - 清除 League 状态
void CGocLeague::Clear() {
    Init();
}

// Send - 发送数据包给所有 League 成员
// 实际广播由 RelayServer 的 CLeague 处理
// GameServer 通过 CommunitySocket 转发请求
void CGocLeague::Send(XSendPacket& sendPacket) {
    if (!IsLeague()) {
        return;
    }
    // 广播由 RelayServer 处理
    // GameServer 端的 CGocLeague 不直接持有成员列表
    // 需要通过 CommunitySocket 发送到 RelayServer
    // TODO: 需要 CommunitySocket 和 XGameServer 头文件后实现
}

// SendLeagueInfo - 发送 League 信息给当前用户
// 请求 RelayServer 发送完整的 League 信息
void CGocLeague::SendLeagueInfo(std::uint8_t byUpdateType) {
    if (!IsLeague()) {
        return;
    }
    // League 信息存储在 RelayServer 的 CLeague 中
    // GameServer 需要通过 CommunitySocket 请求同步
    // TODO: 需要 PS_SYNC_LEAGUE_INFO 和 CommunitySocket 后实现
}

// GetMemberCount - 获取成员数量
// 从本地存储的 ST_LEAGUE_MEMBER 无法直接获取，需要从 RelayServer 查询
// 返回 0 表示需要异步查询
int CGocLeague::GetMemberCount() const {
    if (!IsLeague()) {
        return 0;
    }
    // 成员数量存储在 ST_LEAGUE_INFO.shMemberCount
    // CGocLeague 只存储 ST_LEAGUE_MEMBER，不存储完整 ST_LEAGUE_INFO
    // 需要通过 RelayServer 查询
    return 0;
}

// GetMaxMembers - 获取最大成员数
// 基于 League 等级计算，需要查表
int CGocLeague::GetMaxMembers() const {
    if (!IsLeague()) {
        return 0;
    }
    // League 最大成员数由等级决定
    // 需要从 TB_LEAGUE_INFO 表读取
    // 由于 CGocLeague 不存储 League 等级，返回默认基础值
    // 实际值需要从 RelayServer 的 ST_LEAGUE_INFO 获取
    return 30;
}

// KickMember - 踢出成员
// 发送踢人请求到 RelayServer
bool CGocLeague::KickMember(int nPlayerId) {
    if (!IsLeague()) {
        return false;
    }
    if (!CanKickMember()) {
        return false;
    }
    // 踢人操作由 RelayServer 的 CLeagueManager 处理
    // GameServer 通过 CommunitySocket 发送请求
    // TODO: 需要 CommunitySocket 和协议结构后实现
    return false;
}

// Logout - 登出处理
// 通知 RelayServer 该成员下线
void CGocLeague::Logout() {
    if (!IsLeague()) {
        return;
    }
    // 登出通知由 RelayServer 处理
    // TODO: 需要 CommunitySocket 和协议结构后实现
}

// SaveRecode - 保存记录
// 触发 League 记录保存到数据库
void CGocLeague::SaveRecode() {
    if (!IsLeague()) {
        return;
    }
    // League 记录保存在 RelayServer 端
    // GameServer 端无需额外操作
}

// ============================================================================
// League Permission Checks
// ============================================================================

// HasPermission - 检查是否有特定权限
// IDA 参考: ST_LEAGUE_INFO.nAuth[9] 存储权限数组
// 权限索引: 0=邀请, 1=踢人, 2=公告, 3=仓库存, 4=仓库取, ...
bool CGocLeague::HasPermission(int nPermissionIndex) const {
    if (!IsLeague()) {
        return false;
    }
    // 职位决定权限，position 0 = Master 有所有权限
    if (IsLeagueLeader()) {
        return true;
    }
    // 其他职位需要检查 ST_LEAGUE_INFO.nAuth
    // nAuth 数组索引对应职位 (1-8)
    // nAuth[position] 的位掩码表示该职位拥有的权限
    // 由于 CGocLeague 不存储完整 ST_LEAGUE_INFO，需要从 RelayServer 获取
    // 这里返回基于职位的默认权限
    std::uint8_t byPosition = m_stLeagueMember.byPosition;
    // 副会长 (position 7) 有大部分权限
    if (byPosition == 7) {
        return true;
    }
    // 管理员 (position 5) 有部分权限
    if (byPosition == 5 && nPermissionIndex < 4) {
        return true;
    }
    // 长老 (position 4) 有基础权限
    if (byPosition == 4 && nPermissionIndex < 2) {
        return true;
    }
    return false;
}

// CanKickMember - 检查是否可以踢出成员
bool CGocLeague::CanKickMember() const {
    if (!IsLeague()) {
        return false;
    }
    // 只有会长和副会长可以踢人
    if (IsLeagueLeader()) {
        return true;
    }
    // 副会长 (position 7) 也可以踢人
    return m_stLeagueMember.byPosition == 7;
}

// CanInviteMember - 检查是否可以邀请成员
bool CGocLeague::CanInviteMember() const {
    if (!IsLeague()) {
        return false;
    }
    // 大部分成员都可以邀请
    // 只有普通成员 (position 8) 可能没有邀请权限
    return m_stLeagueMember.byPosition <= 8;
}

// CanChangeNotice - 检查是否可以修改公告
bool CGocLeague::CanChangeNotice() const {
    if (!IsLeague()) {
        return false;
    }
    // 只有会长和副会长可以修改公告
    if (IsLeagueLeader()) {
        return true;
    }
    return m_stLeagueMember.byPosition == 7;
}

// CanUseLeagueWarehouse - 检查是否可以使用 League 仓库
bool CGocLeague::CanUseLeagueWarehouse() const {
    if (!IsLeague()) {
        return false;
    }
    // 会长、副会长、管理员可以使用仓库
    if (IsLeagueLeader()) {
        return true;
    }
    std::uint8_t byPosition = m_stLeagueMember.byPosition;
    return byPosition == 7 || byPosition == 5 || byPosition == 4;
}

// GetLeague - 获取 League 对象
// CGocLeague 不直接持有 CLeague 指针，需要通过 RelayServer 获取
std::shared_ptr<CLeague> CGocLeague::GetLeague() const {
    // CLeague 对象存储在 RelayServer 的 CLeagueManager 中
    // GameServer 端无法直接访问，返回 nullptr
    return nullptr;
}
