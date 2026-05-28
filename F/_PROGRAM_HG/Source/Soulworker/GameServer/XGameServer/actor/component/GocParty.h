// GocParty.h
// CGocParty - Game Object Component for Party system
// 对齐 IDA GameServer.exe (40 bytes)

#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <memory>
#include <vector>

// 前置声明
class CParty;
class CPartyMember;
class CMover;
class CUser;
class XSendPacket;
class XActor;
struct PS_CHANGE_NAME;

// ============================================================================
// CGocParty - 队伍组件类
// IDA 确认大小: 40 bytes
//
// 功能: 管理 CMover 的队伍状态，作为 GOComponent 的子类附加到玩家对象上
// 队伍最多可以有 4 个成员
//
// IDA 内存布局:
// offset 0:  vtable (8 bytes from GOComponent)
// offset 8:  m_pOwner (8 bytes from GOComponent) - but may vary
// offset 16: m_pParty (std::shared_ptr<CParty>, 16 bytes)
// offset 32: m_biMatchingDate (__int64, 8 bytes)
// ============================================================================
class CGocParty : public GOComponent {
public:
    // === 构造函数 ===
    // IDA: ??0CGocParty@@QEAA@XZ @ 0x14010AFF0
    CGocParty();

    // === 析构函数 ===
    // IDA: ??1CGocParty@@UEAA@XZ @ 0x14010B080
    virtual ~CGocParty();

    // === Party Management ===

    // Init - 初始化队伍组件
    // IDA: ?Init@CGocParty@@QEAAXXZ @ 0x14010B0D0
    void Init();

    // Clear - 清除队伍状态
    // IDA: ?Clear@CGocParty@@QEAAXXZ @ 0x14010B100
    void Clear();

    // Leave - 离开队伍
    // IDA: ?Leave@CGocParty@@QEAAXXZ @ 0x14010C250
    void Leave();

    // KickOut - 踢出成员
    // IDA: ?KickOut@CGocParty@@QEAA_NKPEAVCUser@@@Z @ 0x14010BCC0
    bool KickOut(std::uint32_t dwActorID, CUser* pUser);

    // ChangeMaster - 更改队长
    // IDA: ?ChangeMaster@CGocParty@@QEAAXK@Z @ 0x14010C540
    void ChangeMaster(std::uint32_t dwMaster);

    // === Party Query ===

    // IsParty - 检查是否在队伍中
    // IDA: ?IsParty@CGocParty@@QEAA_NXZ @ 0x140091E20
    bool IsParty() const;

    // GetPartyID - 获取队伍ID
    // IDA: ?GetPartyID@CGocParty@@QEAAHXZ @ 0x14009F760
    std::uint32_t GetPartyID() const;

    // IsMember - 检查是否为队伍成员
    // IDA: ?IsMember@CGocParty@@QEAA_NK@Z @ 0x1400831B0
    bool IsMember(std::uint32_t dwActorID) const;

    // IsFull - 检查队伍是否已满 (最多 4 人)
    // IDA: ?IsFull@CGocParty@@QEAA_NXZ @ 0x14010D370
    bool IsFull() const;

    // IsMaster - 检查是否为队长
    // IDA: ?IsMaster@CGocParty@@QEAA_NXZ @ 0x1403104B0
    bool IsMaster() const;

    // GetParty - 获取队伍对象
    // IDA: ?GetParty@CGocParty@@QEAA?AV?$shared_ptr@VCParty@@@tr1@std@@XZ @ 0x1402D3AF0
    std::shared_ptr<CParty> GetParty() const;

    // === Party Sync ===

    // Send - 发送数据包给所有队伍成员
    // IDA: ?Send@CGocParty@@QEAAXAEAVXSendPacket@@@Z @ 0x14010B1A0
    void Send(XSendPacket& sendPacket);

    // SendPartyInfo - 发送队伍信息
    // IDA: ?SendPartyInfo@CGocParty@@QEAAXE@Z @ 0x14010B1F0
    void SendPartyInfo(std::uint8_t byUpdateType);

    // ShowMyPartyInfo - 显示我的队伍信息
    // IDA: ?ShowMyPartyInfo@CGocParty@@QEAAXXZ @ 0x14010BB30
    void ShowMyPartyInfo();

    // === Party Settings ===

    // SetHP - 设置当前HP
    // IDA: ?SetHP@CGocParty@@QEAAXH@Z @ 0x14010B540
    void SetHP(int nHP);

    // SetMaxHP - 设置最大HP
    // IDA: ?SetMaxHP@CGocParty@@QEAAXH@Z @ 0x14010B600
    void SetMaxHP(int nMaxHP);

    // SetLevel - 设置等级
    // IDA: ?SetLevel@CGocParty@@QEAAXH@Z @ 0x14010B6C0
    void SetLevel(int nLevel);

    // SetAwaken - 设置觉醒状态
    // IDA: ?SetAwaken@CGocParty@@QEAAXE@Z @ 0x14010B740
    void SetAwaken(std::uint8_t byAwaken);

    // SetProfilePhoto - 设置头像
    // IDA: ?SetProfilePhoto@CGocParty@@QEAAXK@Z @ 0x14010B7C0
    void SetProfilePhoto(std::uint32_t dwPhotoID);

    // SetMapID - 设置地图ID
    // IDA: ?SetMapID@CGocParty@@QEAAXHHTUXMapID@@@Z @ 0x14010BC20
    void SetMapID(int nMapID, int nChannel, const UXMapID& uxMapID);

    // === Matching ===

    // IsMatchingDate - 检查是否在匹配中
    // IDA: ?IsMatchingDate@CGocParty@@QEAA_NXZ @ 0x14010D3C0
    bool IsMatchingDate() const;

    // SetMatchingDate - 设置匹配时间
    // IDA: ?SetMatchingDate@CGocParty@@QEAAX_J@Z @ 0x140085030
    void SetMatchingDate(std::int64_t biDate);

    // === Member Operations ===

    // GetMemberInMap - 获取地图中的队伍成员
    // IDA: ?GetMemberInMap@CGocParty@@QEAAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z @ 0x14010D130
    void GetMemberInMap(std::vector<CMover*>& vecMover);

    // ChangePartyMemberName - 更改队伍成员名称
    // IDA: ?ChangePartyMemberName@CGocParty@@QEAAXAEAUPS_CHANGE_NAME@@@Z @ 0x14010D460
    void ChangePartyMemberName(PS_CHANGE_NAME& stChangeName);

    // === Utility ===

    // IsExistDeadUser - 检查是否存在死亡成员
    // IDA: ?IsExistDeadUser@CGocParty@@QEAA_NXZ @ 0x140083200
    bool IsExistDeadUser() const;

    // Logout - 登出处理
    // IDA: ?Logout@CGocParty@@QEAAXXZ @ 0x14010B840
    void Logout();

    // SaveRecode - 保存记录
    // IDA: ?SaveRecode@CGocParty@@QEAAXXZ @ 0x14010B340
    void SaveRecode();

    // UpdatePartyBooster - 更新队伍增益
    // IDA: ?UpdatePartyBooster@CGocParty@@QEAAXXZ @ 0x14010C8F0
    void UpdatePartyBooster();

    // SetExp - 设置经验值
    // IDA: ?SetExp@CGocParty@@QEAAXPEAVCUser@@MH@Z @ 0x14010CC20
    void SetExp(CUser* pKiller, float fExp, int nMonsterLv);

    // GetFamilyID - 获取家族ID (静态方法)
    // IDA: ?GetFamilyID@CGocParty@@SAHXZ @ 0x140039000
    static int GetFamilyID();

protected:
    // === IDA 确认的成员变量 ===
    // offset 16-31: m_pParty (std::shared_ptr<CParty>, 16 bytes)
    std::shared_ptr<CParty> m_pParty;

    // offset 32-39: m_biMatchingDate (__int64, 8 bytes)
    std::int64_t m_biMatchingDate = 0;

    // Total: 40+ bytes (with padding)
};
// Note: Actual size may vary due to alignment - verify with IDA
