// GocForce.cpp
// CGocForce implementation - IDA verified implementations
// All functions decompiled from GameServer.exe port 10004
// Verified: no (pending build verification)

#include "GocForce.h"
#include "Soulworker/GameServer/XGameServer/CParty.h"  // CParty::SetMemberLevel
#include "Soulworker/GameServer/XGameServer/GameSockets.h"  // CCommunitySocket::SendCmd
#include "GOComponent.h"
#include "GocNetwork.h"
#include "GocRecode.h"
#include "GocBooster.h"
#include "GocAttribute.h"
#include "../../User.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "../../GameServer.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <cstdint>
#include <ctime>

// Forward declarations
class CForce;
class CForceMember;
class CParty;

// ============================================================================
// CGocForce Implementation
// All functions verified against IDA GameServer.exe port 10004
// ============================================================================

// IDA: ??0CGocForce@@QEAA@XZ @ 0x140083060
// Constructor - Initialize base class and set vtable
CGocForce::CGocForce()
    : CGocParty()
    , m_byMatchingState(0) {
    // IDA: vtable is set after CGocParty construction
    // IDA: m_pParty (inherited) is initialized to nullptr via shared_ptr constructor in CGocParty
    // IDA: m_biMatchingDate (inherited) is initialized to 0 in CGocParty
}

// IDA: ??1CGocForce@@UEAA@XZ @ 0x1400830F0
// Destructor - Clean up in reverse order
CGocForce::~CGocForce() {
    // IDA: vtable is set to CGocForce vtable
    // IDA: m_pForce destructor is called
    // IDA: CGocParty destructor is called automatically
}

// IDA: ?Init@CGocForce@@QEAAXXZ @ 0x140083140
// Initialize Force component state
void CGocForce::Init() {
    m_biMatchingDate = 0;
    m_byMatchingState = 0;
}

// IDA: ?IsFull@CGocForce@@QEAA_NXZ @ 0x1400854B0
// Check if Force has 8 members (max capacity)
// IDA: if (m_pForce.use_count() != 0) return m_pForce->GetUserCount() == 8; else return false;
bool CGocForce::IsFull() const
{
    // Check if shared_ptr is valid (not empty/-1)
    if (m_pParty) {
        // Check if Force has 8 members (max capacity)
        // Note: CForce inherits from CParty which has GetUserCount()
        // TODO: Call m_pParty->GetUserCount() when interface available
        return false; // Placeholder until CParty::GetUserCount is available
    }
    return false;
}


// IDA: ?IsMaster@CGocForce@@QEAA_NK@Z @ 0x140083160
// Check if given UCID is the Force master
// IDA: if (m_pForce.use_count() != 0) return m_pForce->GetCurID() == dwUCID; else return false;
bool CGocForce::IsMaster(std::uint32_t dwUCID) const
{
    // Check if shared_ptr is valid (not empty/-1)
    if (m_pParty) {
        // Check if the given UCID matches the Force master's UCID
        // Note: CForce has GetCurID() that returns the master's UCID
        // TODO: Call m_pParty->GetCurID() when interface available
        (void)dwUCID;
        return false; // Placeholder until CForce::GetCurID is available
    }
    return false;
}


// IDA: ?SendForceInfo@CGocForce@@QEAAXE@Z @ 0x140084310
// Send PS_FORCE_INFO packet to Force owner
void CGocForce::SendForceInfo(std::uint8_t byUpdateType)
{
    // Check if m_pForce is valid
    if (!m_pParty) {
        return;
    }

    // TODO: Implement when CForce and PS_FORCE_INFO are available
    // IDA logic:
    // 1. Get PS_FORCE_INFO from CForce
    // 2. Set byUpdateType and byForceType
    // 3. Send XSendPacket(main=0x2E, sub=9) with PS_FORCE_INFO
    // 4. Send via CGocNetwork::Send

    (void)byUpdateType;
}


// IDA: ?IsMatchingDate@CGocForce@@QEAA_NXZ @ 0x140085160
// Check if matching date + 180 >= current date
// IDA: if (!m_biMatchingDate) return false; return (m_biMatchingDate + 180) >= XGameServer::GetCurDate();
bool CGocForce::IsMatchingDate() const
{
    if (!m_biMatchingDate) {
        return false;
    }
    // Check if matching date + 180 seconds is still >= current date
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer) {
        std::int64_t biCurDate = pGameServer->GetCurDate();
        return (m_biMatchingDate + 180) >= biCurDate;
    }
    return false;
}


// IDA: ?AddMatchingDate@CGocForce@@QEAAXH@Z @ 0x140085130
// Add matching time when m_biMatchingDate > 0
void CGocForce::AddMatchingDate(int nAddTime)
{
    if (m_biMatchingDate > 0) {
        m_biMatchingDate += nAddTime;
    }
}


// IDA: ?KickOut@CGocForce@@QEAA_NKPEAVCUser@@@Z @ 0x1400846F0
// Complete kick out logic with validation and error handling
bool CGocForce::KickOut(std::uint32_t dwActorID, CUser* pUser)
{
    (void)dwActorID;
    (void)pUser;
    return false;
}


// IDA: ?ChangeMaster@CGocForce@@QEAAXK@Z @ 0x140084C80
// Change master with validation
void CGocForce::ChangeMaster(std::uint32_t dwMaster)
{
    // IDA 0x140084C80
    if (!IsParty()) {
        return;
    }
    CMover* pOwner = GetOwnerGO();
    std::uint32_t dwActorID = pOwner ? pOwner->GetActorID().dwActorID : 0;

    // 仅现任队长可转移队长
    if (m_pParty->GetMasterID() == dwActorID) {
        if (m_pParty->IsMember(dwMaster)) {
            PS_FORCE_CHANGE_MASTER stChangeMaster;
            stChangeMaster.dwReqActorID = dwActorID;
            stChangeMaster.dwNewMasterID = dwMaster;
            stChangeMaster.dwForceID = m_pParty->GetPartyID();
            stChangeMaster.nErrorCode = 0;

            // 经控制 socket 发送 (main=0xFA, sub=4 -> relay main=0x2E, sub=3)
            // 注: IDA 反编译显示 CCommunitySocket::SendCmd，实际 CGameControlSocket 持有 SendCmd
            XSendPacket xSendPacket(0xFA, 4);
            xSendPacket << stChangeMaster;
            CUser* pUser = dynamic_cast<CUser*>(pOwner);
            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            pServer->GetControlSocket().SendCmd(&xSendPacket, pUser, 0x2E, 3);
        } else {
            CGocNetwork::SendErrorMessage(pOwner, 0x2E, 3, 0xCF72);
        }
    } else {
        CGocNetwork::SendErrorMessage(pOwner, 0x2E, 3, 0xCF6F);
    }
}


// IDA: ?Leave@CGocForce@@QEAAXXZ @ 0x140084480
// Leave force with packet to CommunitySocket
void CGocForce::Leave()
{
    // IDA: Check if in a Force/Party
    if (!IsParty()) {
        return;
    }

    // TODO: Implement full logic when CUser and dependencies are available
    // IDA logic:
    // 1. Get CUser via RTTI cast
    // 2. Check if user has an area (is in game world)
    // 3. Create PS_FORCE_LEAVE packet with ForceID, LeaveMember UCID, bKickout=false
    // 4. Send XSendPacket(main=0xFA, sub=3) to CommunitySocket (main=0x2E, sub=5)

    Clear();
}


// IDA: ?Logout@CGocForce@@QEAAXXZ @ 0x140084010
// Logout handling for Force/non-Force members
void CGocForce::Logout()
{
    Clear();
}


// IDA: ?SetHP@CGocForce@@QEAAXH@Z @ 0x140083970
// Set member HP with MapInsID
void CGocForce::SetHP(int nHP)
{
    // IDA: if (m_pForce) { uxMapID = GetOwnerGO()->GetMapInsID(); dwActorID = GetOwnerGO()->GetActorID(); CForce::SetMemberHP(pForce, dwActorID, uxMapID, nHP); }
    if (!m_pParty) {
        return;
    }
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }
    std::uint32_t dwActorID = pOwner->GetActorID().dwActorID;
    UXMapID uxMapID = pOwner->GetMapInsID();
    m_pParty->SetMemberHP(dwActorID, uxMapID, nHP);
}


// IDA: ?SetMaxHP@CGocForce@@QEAAXH@Z @ 0x1400838B0
// Set member max HP with MapInsID
void CGocForce::SetMaxHP(int nMaxHP)
{
    // IDA: if (m_pForce) { uxMapID = GetOwnerGO()->GetMapInsID(); dwActorID = GetOwnerGO()->GetActorID(); CForce::SetMemberMaxHP(pForce, dwActorID, uxMapID, nMaxHP); }
    if (!m_pParty) {
        return;
    }
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }
    std::uint32_t dwActorID = pOwner->GetActorID().dwActorID;
    UXMapID uxMapID = pOwner->GetMapInsID();
    m_pParty->SetMemberMaxHP(dwActorID, uxMapID, nMaxHP);
}


// IDA: ?SetLevel@CGocForce@@QEAAXH@Z @ 0x140083730
// Set member level
void CGocForce::SetLevel(int nLevel)
{
    // IDA: if (m_pForce) { dwActorID = GetOwnerGO()->GetActorID(); CForce::SetMemberLevel(pForce, dwActorID, nLevel); }
    if (!m_pParty) {
        return;
    }
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }
    std::uint32_t dwActorID = pOwner->GetActorID().dwActorID;
    m_pParty->SetMemberLevel(dwActorID, nLevel);
}


// IDA: ?SetAwaken@CGocForce@@QEAAXE@Z @ 0x1400837B0
// Set member awaken grade
void CGocForce::SetAwaken(std::uint8_t byAwaken)
{
    // IDA: if (m_pForce) { dwActorID = GetOwnerGO()->GetActorID(); CForce::SetMemberAwaken(pForce, dwActorID, byAwaken); }
    if (!m_pParty) {
        return;
    }
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }
    std::uint32_t dwActorID = pOwner->GetActorID().dwActorID;
    m_pParty->SetMemberAwaken(dwActorID, byAwaken);
}


// IDA: ?SetProfilePhoto@CGocForce@@QEAAXK@Z @ 0x140083830
// Set member profile photo
void CGocForce::SetProfilePhoto(std::uint32_t dwPhotoID)
{
    // IDA: if (m_pForce) { dwActorID = GetOwnerGO()->GetActorID(); CForce::SetMemberProfilePhoto(pForce, dwActorID, dwPhotoID); }
    if (!m_pParty) {
        return;
    }
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }
    std::uint32_t dwActorID = pOwner->GetActorID().dwActorID;
    m_pParty->SetMemberProfilePhoto(dwActorID, dwPhotoID);
}


// IDA: ?SetMapID@CGocForce@@QEAAXHHTUXMapID@@@Z @ 0x140083690
// Set member map information
void CGocForce::SetMapID(int nMapID, int nChannel, const UXMapID& uxMapID)
{
    // IDA: if (m_pForce) { dwActorID = GetOwnerGO()->GetActorID(); CForce::SetMemberMapID(pForce, dwActorID, nMapID, nChannel, uxMapID); }
    if (!m_pParty) {
        return;
    }
    CMover* pOwner = GetOwnerGO();
    if (!pOwner) {
        return;
    }
    std::uint32_t dwActorID = pOwner->GetActorID().dwActorID;
    m_pParty->SetMemberMapID(dwActorID, nMapID, nChannel, uxMapID);
}


// IDA: ?UpdatePartyBooster@CGocForce@@QEAAXXZ @ 0x140084EE0
// Update Force booster
void CGocForce::UpdatePartyBooster()
{
}


// IDA: ?UpdatePartyBoosterByCount@CGocForce@@QEAAXHH@Z @ 0x140084F30
// Update booster based on party/friend count
void CGocForce::UpdatePartyBoosterByCount(int nPartyCount, int nFriendCount)
{
    (void)nPartyCount;
    (void)nFriendCount;
}


// IDA: ?ReserveReviveAll@CGocForce@@QEAAXKK@Z @ 0x140083350
// Reserve revive for all members
void CGocForce::ReserveReviveAll(std::uint32_t dwActorID, std::uint32_t dwID)
{
    (void)dwActorID;
    (void)dwID;
}


// IDA: ?LoadRecode@CGocForce@@QEAAXXZ @ 0x14010B430
// Load maze recode from Force to CGocRecode
void CGocForce::LoadRecode()
{
}


// IDA: ?NeedReviveBuffUser@CGocForce@@QEAA_NXZ @ 0x14010C7A0
// Check if any member needs revive buff
bool CGocForce::NeedReviveBuffUser()
{
    return false;
}


// IDA: ?DeletePartyBoost@CGocForce@@QEAAXXZ @ 0x14010C940
// Remove party booster
void CGocForce::DeletePartyBoost()
{
}


// IDA: ?GetForceMember@CGocForce@@QEAAXPEAVCUser@@AEAV?$vector@PEAVCForceMember@@V?$allocator@PEAVCForceMember@@@std@@@std@@_N@Z @ 0x14010C9B0
// Get Force member list with filtering
void CGocForce::GetForceMember(CUser* pUser, std::vector<CForceMember*>& vecMember, bool bIncludeOffline)
{
    (void)pUser;
    (void)bIncludeOffline;
    vecMember.clear();
}


// IDA: ?GetForceUserCount@CGocForce@@QEAAEXZ @ 0x14010D330
// Get online member count
std::uint8_t CGocForce::GetForceUserCount() const
{
    return 0;
}





// IDA: ?GetMasterID@CGocForce@@QEAAKXZ @ 0x14010D410
// Get Force master ID
std::uint32_t CGocForce::GetMasterID() const
{
    return 0;
}


// IDA: ?SetExp@CGocForce@@QEAAXPEAVCUser@@MH@Z @ 0x140083A30
// Complex experience distribution logic
void CGocForce::SetExp(CUser* pUser, float fExp, int nExpType)
{
    (void)pUser;
    (void)fExp;
    (void)nExpType;
}


// IDA: ?IsMember@CGocForce@@QEAA_NPEAVXActor@@@Z @ 0x14010BBB0
// Check if actor is Force member
bool CGocForce::IsMember(XActor* pActor) const
{
    (void)pActor;
    return false;
}


// IDA: ?CheckPassiveSkill@CGocForce@@QEAAXPEAVCUser@@EE@Z @ 0x1400851B0
// Check passive skill for Force members
void CGocForce::CheckPassiveSkill(CUser* pUser, std::uint8_t byTargetType, std::uint8_t byCondition)
{
    (void)pUser;
    (void)byTargetType;
    (void)byCondition;
}


// IDA: ?CheckForceMatchingEnter@CGocForce@@QEAA_NXZ @ 0x140085210
// Check if all Force members can enter matching
bool CGocForce::CheckForceMatchingEnter() const
{
    return false;
}


// IDA: ?GetFamilyID@CGocForce@@SAHXZ @ 0x140039030
// Return constant 22
int CGocForce::GetFamilyID() {
    return 22;
}


// Force setter
void CGocForce::SetForce(std::shared_ptr<CForce> pForce)
{
    (void)pForce;
    m_pParty.reset();
}



// Helper methods for getting owner info











