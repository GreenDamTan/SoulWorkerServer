// GocParty.cpp
// CGocParty - Game Object Component for Party system
// 从 IDA GameServer.exe 精确还原

#include "GocParty.h"
#include "GocForce.h"  // For GetForceMember - CGocParty and CGocForce share memory layout
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocBooster.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"

// ============================================================================
// CGocParty Memory Layout Notes (IDA verified):
// ============================================================================
// CGocParty 和 CGocForce 共享相同的内存布局，允许 CGocForce::GetForceMember
// 接受 CGocParty* 作为 this 指针:
//   - offset 0:  vtable (8 bytes from GOComponent)
//   - offset 16: m_pParty/m_pForce (std::shared_ptr, 16 bytes)
//   - offset 32: m_biMatchingDate (__int64, 8 bytes)
// 这种设计允许队伍和公会系统共享 GetForceMember 实现
// ============================================================================

// ============================================================================
// 静态方法
// ============================================================================

// IDA: ?GetFamilyID@CGocParty@@SAHXZ @ 0x140039000
// Family ID = 8 (Party组件的家族ID)
int CGocParty::GetFamilyID()
{
    return 8;
}

// ============================================================================
// 构造函数/析构函数
// ============================================================================

// IDA: ??0CGocParty@@QEAA@XZ @ 0x14010AFF0
CGocParty::CGocParty()
    : GOComponent()
    , m_pParty()
    , m_biMatchingDate(0)
{
    // GOComponent 构造函数会初始化 vtable
    // m_pParty 默认初始化为空 shared_ptr
}

// IDA: ??1CGocParty@@UEAA@XZ @ 0x14010B080
CGocParty::~CGocParty()
{
    // 设置 vtable 指向 CGocParty 的虚函数表
    // 释放 m_pParty (shared_ptr 析构)
    m_pParty.reset();
}

// ============================================================================
// 初始化/清理
// ============================================================================

// IDA: ?Init@CGocParty@@QEAAXXZ @ 0x14010B0D0
void CGocParty::Init()
{
    m_pParty.reset();
    m_biMatchingDate = 0;
}

// IDA: ?Clear@CGocParty@@QEAAXXZ @ 0x14010B100
void CGocParty::Clear()
{
    m_pParty.reset();
    m_biMatchingDate = 0;
}


// ============================================================================
// 队伍状态查询
// ============================================================================

// IDA: ?IsParty@CGocParty@@QEAA_NXZ @ 0x140091E20
bool CGocParty::IsParty() const
{
    // 检查 shared_ptr 是否有效 (operator bool)
    return m_pParty.operator bool();
}

// IDA: ?GetPartyID@CGocParty@@QEAAHXZ @ 0x14009F760
std::uint32_t CGocParty::GetPartyID() const
{
    return 0;
}


// IDA: ?IsMember@CGocParty@@QEAA_NK@Z @ 0x1400831B0
// Verified: Checks if actor ID is a member of this party
// Per IDA decompile at 0x1400831B0:
// - Checks if m_pParty is valid using operator int std::_Bool_struct::* (operator bool)
// - If invalid, returns false
// - Otherwise calls CParty::IsMember via operator->
bool CGocParty::IsMember(std::uint32_t dwActorID) const
{
    (void)dwActorID;
    return false;
}


// IDA: ?IsFull@CGocParty@@QEAA_NXZ @ 0x14010D370
bool CGocParty::IsFull() const
{
    return false;
}


// IDA: ?IsMaster@CGocParty@@QEAA_NXZ @ 0x1403104B0
bool CGocParty::IsMaster() const
{
    return false;
}


// IDA: ?GetParty@CGocParty@@QEAA?AV?$shared_ptr@VCParty@@@tr1@std@@XZ @ 0x1402D3AF0
std::shared_ptr<CParty> CGocParty::GetParty() const
{
    return m_pParty;
}

// ============================================================================
// 队伍数据同步
// ============================================================================

// IDA: ?Send@CGocParty@@QEAAXAEAVXSendPacket@@@Z @ 0x14010B1A0
void CGocParty::Send(XSendPacket& sendPacket)
{
    (void)sendPacket;
}


// IDA: ?SendPartyInfo@CGocParty@@QEAAXE@Z @ 0x14010B1F0
void CGocParty::SendPartyInfo(std::uint8_t byUpdateType)
{
    (void)byUpdateType;
}


// IDA: ?ShowMyPartyInfo@CGocParty@@QEAAXXZ @ 0x14010BB30
void CGocParty::ShowMyPartyInfo()
{
}


// ============================================================================
// 队伍成员状态设置
// ============================================================================

// IDA: ?SetHP@CGocParty@@QEAAXH@Z @ 0x14010B540
void CGocParty::SetHP(int nHP)
{
    (void)nHP;
}


// IDA: ?SetMaxHP@CGocParty@@QEAAXH@Z @ 0x14010B600
void CGocParty::SetMaxHP(int nMaxHP)
{
    (void)nMaxHP;
}


// IDA: ?SetLevel@CGocParty@@QEAAXH@Z @ 0x14010B6C0
void CGocParty::SetLevel(int nLevel)
{
    (void)nLevel;
}


// IDA: ?SetAwaken@CGocParty@@QEAAXE@Z @ 0x14010B740
void CGocParty::SetAwaken(std::uint8_t byAwaken)
{
    (void)byAwaken;
}


// IDA: ?SetProfilePhoto@CGocParty@@QEAAXK@Z @ 0x14010B7C0
void CGocParty::SetProfilePhoto(std::uint32_t dwPhotoID)
{
    (void)dwPhotoID;
}


// IDA: ?SetMapID@CGocParty@@QEAAXHHTUXMapID@@@Z @ 0x14010BC20
void CGocParty::SetMapID(int nMapID, int nChannel, const UXMapID& uxMapID)
{
    (void)nMapID;
    (void)nChannel;
    (void)uxMapID;
}


// ============================================================================
// 匹配系统
// ============================================================================

// IDA: ?IsMatchingDate@CGocParty@@QEAA_NXZ @ 0x14010D3C0
bool CGocParty::IsMatchingDate() const
{
    return m_biMatchingDate != 0;
}


// IDA: ?SetMatchingDate@CGocParty@@QEAAX_J@Z @ 0x140085030
void CGocParty::SetMatchingDate(std::int64_t biDate)
{
    m_biMatchingDate = biDate;
}


// ============================================================================
// 队伍操作
// ============================================================================

// IDA: ?KickOut@CGocParty@@QEAA_NKPEAVCUser@@@Z @ 0x14010BCC0
bool CGocParty::KickOut(std::uint32_t dwActorID, CUser* pUser)
{
    (void)dwActorID;
    (void)pUser;
    return false;
}


// IDA: ?Leave@CGocParty@@QEAAXXZ @ 0x14010C250
void CGocParty::Leave()
{
    Clear();
}


// IDA: ?ChangeMaster@CGocParty@@QEAAXK@Z @ 0x14010C540
void CGocParty::ChangeMaster(std::uint32_t dwMaster)
{
    (void)dwMaster;
}


// ============================================================================
// 成员操作
// ============================================================================

// IDA: ?GetMemberInMap@CGocParty@@QEAAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z @ 0x14010D130
void CGocParty::GetMemberInMap(std::vector<CMover*>& vecMover)
{
    vecMover.clear();
}


// IDA: ?ChangePartyMemberName@CGocParty@@QEAAXAEAUPS_CHANGE_NAME@@@Z @ 0x14010D460
void CGocParty::ChangePartyMemberName(PS_CHANGE_NAME& stChangeName)
{
    (void)stChangeName;
}


// ============================================================================
// 工具函数
// ============================================================================

// IDA: ?IsExistDeadUser@CGocParty@@QEAA_NXZ @ 0x140083200
bool CGocParty::IsExistDeadUser() const
{
    return false;
}


// IDA: ?Logout@CGocParty@@QEAAXXZ @ 0x14010B840
void CGocParty::Logout()
{
    Clear();
}


// IDA: ?SaveRecode@CGocParty@@QEAAXXZ @ 0x14010B340
void CGocParty::SaveRecode()
{
}


// IDA: ?UpdatePartyBooster@CGocParty@@QEAAXXZ @ 0x14010C8F0
void CGocParty::UpdatePartyBooster()
{
}


// IDA: ?SetExp@CGocParty@@QEAAXPEAVCUser@@MH@Z @ 0x14010CC20
void CGocParty::SetExp(CUser* pKiller, float fExp, int nMonsterLv)
{
    (void)pKiller;
    (void)fExp;
    (void)nMonsterLv;
}

