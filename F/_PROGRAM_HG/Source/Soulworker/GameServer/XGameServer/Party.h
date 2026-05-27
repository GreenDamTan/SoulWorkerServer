// Party.h
// CGocParty - Game Object Component Party
// 对齐 IDA GameServer.exe (40 bytes)

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

// 包含完整定义而非前置声明
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

// 前置声明
class CMover;
class CUser;
class CParty;
class CPartyMember;
class XSendPacket;
struct PS_PARTY_INFO;
struct PS_CHANGE_NAME;

// ============================================================================
// GOComponent - 游戏对象组件基类
// 对齐 IDA: GOComponent (16 bytes - vtable pointer + padding)
// ============================================================================
class GOComponent {
public:
    GOComponent();
    virtual ~GOComponent();
    
private:
    // Padding to match IDA size of 16 bytes
    char _pad0[8] = {};
};

// ============================================================================
// CGocParty - 队伍组件类
// IDA 确认大小: 40 bytes
//
// 功能: 管理 CMover 的队伍状态，作为 GOComponent 的子类附加到玩家对象上
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

    // IsFull - 检查队伍是否已满
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
    void SetMapID(int nMapID, const UXMapID& uxMapID);

    // SetParty - 设置队伍对象
    // IDA: ?SetParty@CGocParty@@QEAAXAEAV?$shared_ptr@VCParty@@@tr1@std@@@Z @ 0x140082F30
    void SetParty(std::shared_ptr<CParty> pParty);

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

    // GetFamilyID - 获取家族ID (静态方法)
    // IDA: ?GetFamilyID@CGocParty@@SAHXZ @ 0x140039000
    static int GetFamilyID();

private:
    // === IDA 确认的成员变量 ===
    // offset 0-15: GOComponent base (vtable pointer + padding = 16 bytes)
    
    // offset 16-31: m_pParty (std::shared_ptr<CParty>, 16 bytes)
    std::shared_ptr<CParty> m_pParty;

    // offset 32-39: m_biMatchingDate (__int64, 8 bytes)
    std::int64_t m_biMatchingDate = 0;

    // Total: 40 bytes
};
static_assert(sizeof(CGocParty) == 40, "CGocParty size must be 40 bytes per IDA");
