// GocLeague.h
// CGocLeague - Game Object Component for League/Guild system
// 对齐 IDA GameServer.exe

#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <memory>

// 前置声明
class CLeague;
class CUser;
class XSendPacket;
struct ST_LEAGUE_MEMBER;

// ============================================================================
// CGocLeague - League/Guild 组件类
// IDA 确认大小: 包含 ST_LEAGUE_MEMBER 成员
//
// 功能: 管理 CMover 的 League/Guild 状态，作为 GOComponent 的子类附加到玩家对象上
// League 是大型公会系统，支持更多成员和更复杂的功能
// ============================================================================
class CGocLeague : public GOComponent {
public:
    // === 构造函数 ===
    // IDA: ??0CGocLeague@@QEAA@XZ @ 0x1400FA870
    CGocLeague();

    // === 析构函数 ===
    // IDA: ??1CGocLeague@@UEAA@XZ @ 0x1400FA8F0
    virtual ~CGocLeague();

    // === League Management ===

    // Init - 初始化 League 组件
    // IDA: ?Init@CGocLeague@@QEAAXXZ @ 0x1400FA920
    void Init();

    // Clear - 清除 League 状态
    void Clear();

    // === League Query ===

    // GetFamilyID - 获取 Family ID (静态函数)
    // IDA: ?GetFamilyID@CGocLeague@@SAHXZ @ 0x1402AC5A0
    // Returns: 13 (常量)
    static int GetFamilyID();

    // IsLeague - 检查是否在 League 中
    // 内联函数 - 简单 getter
    bool IsLeague() const { return m_stLeagueMember.nLeagueID != 0; }

    // GetLeagueID - 获取 League ID
    // 内联函数 - 简单 getter
    int GetLeagueID() const { return m_stLeagueMember.nLeagueID; }

    // IsLeagueLeader - 检查是否为 League 领袖
    // 内联函数 - position 0 为领袖
    bool IsLeagueLeader() const { return m_stLeagueMember.byPosition == 0; }

    // GetLeagueRank - 获取 League 等级/排名
    // TODO: 需要根据 exp 计算实际等级，当前返回 exp 值
    int GetLeagueRank() const {
        // TODO: 需要根据 exp 计算实际等级
        return static_cast<int>(m_stLeagueMember.biLeagueExp);
    }

    // GetLeague - 获取 League 对象
    std::shared_ptr<CLeague> GetLeague() const;

    // === League Info ===

    // GetInviteActorID - 获取邀请者 ActorID
    std::uint32_t GetInviteActorID() const { return m_dwInviteActorID; }

    // SetInviteActorID - 设置邀请者 ActorID
    void SetInviteActorID(std::uint32_t dwActorID) { m_dwInviteActorID = dwActorID; }

    // GetLeagueMember - 获取 League 成员信息
    const ST_LEAGUE_MEMBER* GetLeagueMember() const { return &m_stLeagueMember; }

    // === League Sync ===

    // Send - 发送数据包给所有 League 成员
    void Send(XSendPacket& sendPacket);

    // SendLeagueInfo - 发送 League 信息
    void SendLeagueInfo(std::uint8_t byUpdateType);

    // === Member Operations ===

    // GetMemberCount - 获取成员数量
    int GetMemberCount() const;

    // GetMaxMembers - 获取最大成员数
    int GetMaxMembers() const;

    // KickMember - 踢出成员
    bool KickMember(int nPlayerId);

    // === Utility ===

    // Logout - 登出处理
    void Logout();

    // SaveRecode - 保存记录
    void SaveRecode();

    // === League Permission Checks ===

    // HasPermission - 检查是否有特定权限
    bool HasPermission(int nPermissionIndex) const;

    // CanKickMember - 检查是否可以踢出成员
    bool CanKickMember() const;

    // CanInviteMember - 检查是否可以邀请成员
    bool CanInviteMember() const;

    // CanChangeNotice - 检查是否可以修改公告
    bool CanChangeNotice() const;

    // CanUseLeagueWarehouse - 检查是否可以使用 League 仓库
    bool CanUseLeagueWarehouse() const;

private:
    // === IDA 确认的成员变量 ===
    // offset 0-15: GOComponent base (vtable pointer + padding = 16 bytes)

    // ST_LEAGUE_MEMBER 结构体
    // IDA: ??0ST_LEAGUE_MEMBER@@QEAA@XZ @ 0x1400FAB20
    ST_LEAGUE_MEMBER m_stLeagueMember;

    // 邀请者 ActorID
    std::uint32_t m_dwInviteActorID = 0;
};

// ============================================================================
// ST_LEAGUE_MEMBER - League 成员信息结构体
// IDA: ??0ST_LEAGUE_MEMBER@@QEAA@XZ @ 0x1400FAB20
// IDA size: 32 bytes
// ============================================================================
struct ST_LEAGUE_MEMBER {
    int nLeagueID = 0;                    // offset 0, size 4
    std::uint8_t byPosition = 0;          // offset 4, size 1
    // padding: offset 5-7 (3 bytes)
    std::int64_t biLeagueExp = 0;         // offset 8, size 8
    std::int64_t biJoinDate = 0;          // offset 16, size 8
    std::int64_t biApplicationDate = 0;   // offset 24, size 8

    ST_LEAGUE_MEMBER() {
        // IDA 初始化逻辑: 默认初始化所有成员为0
        nLeagueID = 0;
        byPosition = 0;
        biLeagueExp = 0;
        biJoinDate = 0;
        biApplicationDate = 0;
    }
};
