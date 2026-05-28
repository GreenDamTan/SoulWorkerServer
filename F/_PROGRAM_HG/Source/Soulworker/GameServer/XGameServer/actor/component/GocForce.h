// GocForce.h
// CGocForce - Game Object Component for Force/Guild system
// 对齐 IDA GameServer.exe (40 bytes)

#pragma once

#include "GocParty.h"
#include <cstdint>
#include <memory>

// 前置声明
class CForce;
class CForceMember;
class CUser;
class XActor;
class XSendPacket;

// ============================================================================
// CGocForce - Force/Guild 组件类
// IDA 确认大小: 40 bytes (继承自 CGocParty)
//
// 功能: 管理 CMover 的 Force/Guild 状态，继承自 CGocParty
// Force 最多可以有 8 个成员
//
// IDA 内存布局 (继承自 CGocParty):
// offset 0:  vtable (8 bytes from GOComponent)
// offset 8:  m_eGOCType (4 bytes from GOComponent)
// offset 16: m_pParty/m_pForce (std::shared_ptr<CParty/CForce>, 16 bytes)
// offset 32: m_biMatchingDate (__int64, 8 bytes)
// offset 40: m_byMatchingState (std::uint8_t, 1 byte)
// ============================================================================
class CGocForce : public CGocParty {
public:
    // === 构造函数 ===
    // IDA: ??0CGocForce@@QEAA@XZ @ 0x140083060
    CGocForce();

    // === 析构函数 ===
    // IDA: ??1CGocForce@@UEAA@XZ @ 0x1400830F0
    virtual ~CGocForce();

    // === Force Management ===

    // Init - 初始化 Force 组件
    // IDA: ?Init@CGocForce@@QEAAXXZ @ 0x140083140
    void Init();

    // Clear - 清除 Force 状态 (使用 CGocParty::Clear)
    // 继承自 CGocParty

    // Leave - 离开 Force
    // IDA: ?Leave@CGocForce@@QEAAXXZ @ 0x140084480
    void Leave();

    // KickOut - 踢出成员
    // IDA: ?KickOut@CGocForce@@QEAA_NKPEAVCUser@@@Z @ 0x1400846F0
    bool KickOut(std::uint32_t dwActorID, CUser* pUser);

    // ChangeMaster - 更改队长
    // IDA: ?ChangeMaster@CGocForce@@QEAAXK@Z @ 0x140084C80
    void ChangeMaster(std::uint32_t dwMaster);

    // === Force Query ===

    // IsForce - 检查是否在 Force 中 (使用 IsParty)
    // IDA: CGocParty::IsParty @ 0x140091E20
    bool IsForce() const;

    // GetForceID - 获取 Force ID (使用 GetPartyID)
    // IDA: CGocParty::GetPartyID @ 0x14009F760
    std::uint32_t GetForceID() const;

    // IsMember - 检查是否为 Force 成员
    // IDA: ?IsMember@CGocForce@@QEAA_NPEAVXActor@@@Z @ 0x14010BBB0
    bool IsMember(class XActor* pActor) const;

    // IsFull - 检查 Force 是否已满 (最多 8 人)
    // IDA: ?IsFull@CGocForce@@QEAA_NXZ @ 0x1400854B0
    bool IsFull() const;

    // IsMaster - 检查是否为队长 (参数版本)
    // IDA: ?IsMaster@CGocForce@@QEAA_NK@Z @ 0x140083160
    bool IsMaster(std::uint32_t dwUCID) const;

    // GetForce - 获取 Force 对象
    std::shared_ptr<CForce> GetForce() const;

    // === Force Sync ===

    // Send - 发送数据包给所有 Force 成员 (使用 CGocParty::Send)
    // 继承自 CGocParty

    // SendForceInfo - 发送 Force 信息
    // IDA: ?SendForceInfo@CGocForce@@QEAAXE@Z @ 0x140084310
    void SendForceInfo(std::uint8_t byUpdateType);

    // ShowMyForceInfo - 显示我的 Force 信息 (使用 CGocParty::ShowMyPartyInfo)
    // 继承自 CGocParty

    // === Force Settings ===

    // SetHP - 设置当前 HP
    // IDA: ?SetHP@CGocForce@@QEAAXH@Z @ 0x140083970
    void SetHP(int nHP);

    // SetMaxHP - 设置最大 HP
    // IDA: ?SetMaxHP@CGocForce@@QEAAXH@Z @ 0x1400838B0
    void SetMaxHP(int nMaxHP);

    // SetLevel - 设置等级
    // IDA: ?SetLevel@CGocForce@@QEAAXH@Z @ 0x140083730
    void SetLevel(int nLevel);

    // SetAwaken - 设置觉醒状态
    // IDA: ?SetAwaken@CGocForce@@QEAAXE@Z @ 0x1400837B0
    void SetAwaken(std::uint8_t byAwaken);

    // SetProfilePhoto - 设置头像
    // IDA: ?SetProfilePhoto@CGocForce@@QEAAXK@Z @ 0x140083830
    void SetProfilePhoto(std::uint32_t dwPhotoID);

    // SetMapID - 设置地图 ID
    // IDA: ?SetMapID@CGocForce@@QEAAXHHTUXMapID@@@Z @ 0x140083690
    void SetMapID(int nMapID, int nChannel, const UXMapID& uxMapID);

    // SetForce - 设置 Force 对象
    // IDA: ?SetForce@CGocForce@@QEAAXV?$shared_ptr@VCForce@@@tr1@std@@@Z @ 0x140083F30
    void SetForce(std::shared_ptr<CForce> pForce);

    // === Matching ===

    // IsMatchingDate - 检查是否在匹配中
    // IDA: ?IsMatchingDate@CGocForce@@QEAA_NXZ @ 0x140085160
    bool IsMatchingDate() const;

    // AddMatchingDate - 增加匹配时间
    // IDA: ?AddMatchingDate@CGocForce@@QEAAXH@Z @ 0x140085130
    void AddMatchingDate(int nAddTime);

    // SetMatchingState - 设置匹配状态
    // IDA: ?SetMatchingState@CGocForce@@QEAAXE@Z @ 0x1401F3600
    void SetMatchingState(std::uint8_t byState);

    // GetMatchingState - 获取匹配状态
    // IDA: ?GetMatchingState@CGocForce@@QEAAEXZ @ 0x1403B0280
    std::uint8_t GetMatchingState() const;

    // === Member Operations ===

    // GetForceMember - 获取 Force 成员列表
    // IDA: ?GetForceMember@CGocForce@@QEAAXPEAVCUser@@AEAV?$vector@PEAVCForceMember@@V?$allocator@PEAVCForceMember@@@std@@@std@@_N@Z @ 0x14010C9B0
    void GetForceMember(CUser* pUser, std::vector<class CForceMember*>& vecMember, bool bIncludeOffline);

    // GetForceUserCount - 获取 Force 成员数量
    // IDA: ?GetForceUserCount@CGocForce@@QEAAEXZ @ 0x14010D330
    std::uint8_t GetForceUserCount() const;

    // GetMasterID - 获取队长 ID
    // IDA: ?GetMasterID@CGocForce@@QEAAKXZ @ 0x14010D410
    std::uint32_t GetMasterID() const;

    // === Utility ===

    // Logout - 登出处理
    // IDA: ?Logout@CGocForce@@QEAAXXZ @ 0x140084010
    void Logout();

    // LoadRecode - 加载记录
    // IDA: ?LoadRecode@CGocForce@@QEAAXXZ @ 0x14010B430
    void LoadRecode();

    // UpdatePartyBooster - 更新队伍增益
    // IDA: ?UpdatePartyBooster@CGocForce@@QEAAXXZ @ 0x140084EE0
    void UpdatePartyBooster();

    // UpdatePartyBoosterByCount - 根据成员数量更新增益
    // IDA: ?UpdatePartyBoosterByCount@CGocForce@@QEAAXHH@Z @ 0x140084F30
    void UpdatePartyBoosterByCount(int nPartyCount, int nFriendCount);

    // ReserveReviveAll - 预约复活所有成员
    // IDA: ?ReserveReviveAll@CGocForce@@QEAAXKK@Z @ 0x140083350
    void ReserveReviveAll(std::uint32_t dwActorID, std::uint32_t dwID);

    // NeedReviveBuffUser - 检查是否需要复活 Buff
    // IDA: ?NeedReviveBuffUser@CGocForce@@QEAA_NXZ @ 0x14010C7A0
    bool NeedReviveBuffUser();

    // DeletePartyBoost - 删除队伍增益
    // IDA: ?DeletePartyBoost@CGocForce@@QEAAXXZ @ 0x14010C940
    void DeletePartyBoost();

    // SetExp - 设置经验值
    // IDA: ?SetExp@CGocForce@@QEAAXPEAVCUser@@MH@Z @ 0x140083A30
    void SetExp(CUser* pUser, float fExp, int nExpType);

    // GetFamilyID - 获取家族 ID (静态方法)
    // IDA: ?GetFamilyID@CGocForce@@SAHXZ @ 0x140039030
    static int GetFamilyID();

protected:
    // === IDA 确认的成员变量 ===
    // 继承自 CGocParty:
    // offset 16-31: m_pParty (std::shared_ptr<CParty>, 16 bytes) - 作为 m_pForce 使用
    // offset 32-39: m_biMatchingDate (__int64, 8 bytes)

    // offset 40: m_byMatchingState (std::uint8_t, 1 byte)
    std::uint8_t m_byMatchingState = 0;

    // Total: 41+ bytes (with padding)
};
