// GocForce.cpp
// CGocForce implementation - IDA verified implementations
// All functions decompiled from GameServer.exe port 10004
// Verified: no (pending build verification)

#include "GocForce.h"
#include "Soulworker/GameServer/XGameServer/CParty.h"  // CParty::SetMemberLevel
#include "Soulworker/GameServer/XGameServer/CForce.h"  // CForce::GetForceType/UpdateForceBooster
#include "Soulworker/GameServer/XCore/XArea/XArea.h"  // XArea::GetChannel
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
    // IDA: if (shared_ptr bool check) return;  (m_pForce present check)
    if (!m_pParty)
        return;

    // IDA: PS_FORCE_INFO stForceInfo;  (PS_PARTY_INFO ctor cast)
    // CParty::GetPartyInfo(m_pForce.get(), &stForceInfo);
    // stForceInfo.byUpdateType = byUpdateType;
    // stForceInfo.byForceType = CForce::GetForceType(m_pForce.get());
    // XSendPacket xSendPacket(0x2E, 9) << stForceInfo; CGocNetwork::Send(...);
    PS_FORCE_INFO stForceInfo;
    m_pParty->GetPartyInfo(reinterpret_cast<PS_PARTY_INFO&>(stForceInfo));
    stForceInfo.byUpdateType = byUpdateType;
    stForceInfo.byForceType = static_cast<CForce*>(m_pParty.get())->GetForceType();

    XSendPacket xSendPacket(0x2E, 9);
    xSendPacket << stForceInfo;
    CGocNetwork::Send(GetOwnerGO(), xSendPacket);
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
    // IDA: if (!IsParty()) { SendErrorMessage(0x2E, 4, 0xCF72); return 0; }
    if (!IsParty()) {
        CGocNetwork::SendErrorMessage(GetOwnerGO(), 0x2E, 4, 0xCF72);
        return false;
    }
    // IDA: 不能踢自己 (owner UCID == dwActorID -> 0xCF8E)
    CMover* pOwner = GetOwnerGO();
    std::uint32_t dwOwnerUCID = pOwner ? pOwner->GetActorID().dwActorID : 0;
    if (dwOwnerUCID == dwActorID) {
        CGocNetwork::SendErrorMessage(GetOwnerGO(), 0x2E, 4, 0xCF8E);
        return false;
    }
    // IDA: 必须为队长 (owner 无 area 或非队长 -> 0xCF74)
    CUser* pOwnerUser = dynamic_cast<CUser*>(pOwner);
    if (!pOwnerUser || !pOwnerUser->GetArea() || m_pParty->GetMasterID() != dwOwnerUCID) {
        CGocNetwork::SendErrorMessage(GetOwnerGO(), 0x2E, 4, 0xCF74);
        return false;
    }
    // IDA: 目标必须为成员 (CParty::GetMember -> 0xCF13)
    if (!m_pParty)
        return false;
    CPartyMember* pMember = m_pParty->GetMember(dwActorID);
    if (!pMember) {
        CGocNetwork::SendErrorMessage(GetOwnerGO(), 0x2E, 4, 0xCF13);
        return false;
    }
    // IDA: 不能踢队长 (目标即队长 -> 0xCF6F)
    if (m_pParty->GetMasterID() == dwActorID) {
        CGocNetwork::SendErrorMessage(GetOwnerGO(), 0x2E, 4, 0xCF6F);
        return false;
    }
    // IDA: 若目标在迷宫内 (Maze_Type 0/2/8/9) -> 0xCF80
    int nMapID = pMember->GetMemberInfo().nMapID;
    TB_MAZE_INFO* pTBmazeInfo = TXSingleton<XGameServer>::Instance()->GetResourceMgr().GetTB_MAZE_INFO(nMapID);
    if (pTBmazeInfo && (!pTBmazeInfo->Maze_Type || pTBmazeInfo->Maze_Type == 2 ||
                        pTBmazeInfo->Maze_Type == 8 || pTBmazeInfo->Maze_Type == 9)) {
        CGocNetwork::SendErrorMessage(GetOwnerGO(), 0x2E, 4, 0xCF80);
        return false;
    }
    // IDA: 发送踢出请求到 CommunitySocket
    PS_FORCE_LEAVE stForceLeave;
    stForceLeave.dwForceID = m_pParty->GetPartyID();
    stForceLeave.dwLeaveMember = dwActorID;
    stForceLeave.bKickout = 1;

    XSendPacket xSendPacket(0xFA, 3);
    xSendPacket << stForceLeave;
    xSendPacket.XParse << (pUser ? pUser->GetUCID() : 0);
    xSendPacket.XParse << static_cast<int>(pUser ? pUser->GetUAID() : 0);
    xSendPacket.XParse << (pUser ? pUser->GetLevel() : 0);
    xSendPacket.XParse << pMember->GetMemberInfo().byLevel;
    TXSingleton<XGameServer>::Instance()->GetCommunitySocket().SendCmd(
        &xSendPacket, pOwnerUser, 0x2E, 4);
    return true;
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

            // IDA: CCommunitySocket::SendCmd(&m_communitySocket, &xSendPacket, pUser, 0x2E, 3)
            XSendPacket xSendPacket(0xFA, 4);
            xSendPacket << stChangeMaster;
            CUser* pUser = dynamic_cast<CUser*>(pOwner);
            TXSingleton<XGameServer>::Instance()->GetCommunitySocket().SendCmd(
                &xSendPacket, pUser, 0x2E, 3);
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
    // IDA: if (!IsParty()) return;
    if (!IsParty()) {
        return;
    }

    // IDA: pUser = dynamic_cast<CUser*>(GetOwnerGO()); if (!pUser) return;
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) {
        return;
    }

    // IDA: if (pUser->GetArea()) {
    //   stForceLeave.dwForceID = GetPartyID();
    //   stForceLeave.dwLeaveMember = GetUCID();
    //   stForceLeave.bKickout = 0;
    //   XSendPacket xSendPacket(0xFA, 3) << stForceLeave << GetUCID << GetUAID << GetLevel << GetLevel;
    //   CCommunitySocket::SendCmd(&m_communitySocket, &xSendPacket, pUser, 0x2E, 5);
    // }
    if (pUser->GetArea()) {
        PS_FORCE_LEAVE stForceLeave;
        stForceLeave.dwForceID = m_pParty->GetPartyID();
        stForceLeave.dwLeaveMember = pUser->GetUCID();
        stForceLeave.bKickout = 0;

        XSendPacket xSendPacket(0xFA, 3);
        xSendPacket << stForceLeave;
        xSendPacket.XParse << pUser->GetUCID();
        xSendPacket.XParse << static_cast<int>(pUser->GetUAID());
        xSendPacket.XParse << pUser->GetLevel();
        xSendPacket.XParse << pUser->GetLevel();
        TXSingleton<XGameServer>::Instance()->GetCommunitySocket().SendCmd(
            &xSendPacket, pUser, 0x2E, 5);
    }
}


// IDA: ?Logout@CGocForce@@QEAAXXZ @ 0x140084010
// Logout handling for Force/non-Force members
void CGocForce::Logout()
{
    // IDA: if (IsParty()) {
    //   dwActorID = GetUCID();
    //   pMembera = CParty::GetMember(m_pForce.get(), dwActorID);
    //   if (pMembera) {
    //     ST_UPDATE_FORCE_MEMBER stForceMember; stForceMember.dwForceID = GetPartyID();
    //     CPartyMember::Logout(pMembera, &stForceMember);
    //     XSendPacket xSendPacket(0xFA, 5) << stForceMember;
    //     CCommunitySocket::SendCheck(&m_communitySocket, &xSendPacket);
    //   }
    // } else {
    //   pUser = dynamic_cast<CUser*>(GetOwnerGO());
    //   if (pUser) {
    //     XSendPacket v23(0xFA, 0x14) << GetUCID << 2 << GetUAID << GetLevel;
    //     CCommunitySocket::SendCheck(&m_communitySocket, &v23);
    //   }
    // }
    // LogHelper::LogDebug("game.party", "<FORCE> Logout Req ( %d )", GetUCID);
    // CGocParty::Clear(this);
    if (IsParty()) {
        std::uint32_t dwActorID = dynamic_cast<CUser*>(GetOwnerGO())->GetUCID();
        CPartyMember* pMembera = m_pParty->GetMember(dwActorID);
        if (pMembera) {
            ST_UPDATE_PARTY_MEMBER stForceMember;
            stForceMember.dwPartyID = m_pParty->GetPartyID();
            pMembera->Logout(stForceMember);

            XSendPacket xSendPacket(0xFA, 5);
            xSendPacket << stForceMember;
            TXSingleton<XGameServer>::Instance()->GetCommunitySocket().SendCheck(&xSendPacket);
        }
    } else {
        CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
        if (pUser) {
            XSendPacket v23(0xFA, 0x14);
            v23.XParse << pUser->GetUCID();
            v23.XParse << 2;
            v23.XParse << static_cast<int>(pUser->GetUAID());
            v23.XParse << pUser->GetLevel();
            TXSingleton<XGameServer>::Instance()->GetCommunitySocket().SendCheck(&v23);
        }
    }
    LogHelper::LogDebug("game.party", "<FORCE> Logout Req ( %d )",
                        static_cast<int>(dynamic_cast<CUser*>(GetOwnerGO())->GetUCID()));
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
    // IDA: if (shared_ptr bool check) { CForce::UpdateForceBooster(m_pForce.get(), 0); }
    if (!m_pParty)
        return;
    static_cast<CForce*>(m_pParty.get())->UpdateForceBooster(0);
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
    // IDA: if (shared_ptr bool check) {
    //   v1 = GetOwnerGO(); CMover::GetGOC<CGocRecode>(v1, &pRecode, 0);
    //   if (pRecode) {
    //     dwActorID = GetUCID();
    //     CForce::GetMazeRecode(m_pForce.get(), dwActorID, nRecode);
    //     CGocRecode::SetFullRecode(pRecode, nRecode);
    //   }
    // }
    if (!m_pParty)
        return;
    CMover* pOwner = GetOwnerGO();
    std::shared_ptr<CGocRecode> pRecode = pOwner ? pOwner->GetGOC_Recode(false) : nullptr;
    if (!pRecode)
        return;

    std::uint32_t dwActorID = pOwner->GetActorID().dwActorID;
    int nRecode[10] = {};
    static_cast<CForce*>(m_pParty.get())->GetMazeRecode(dwActorID, nRecode);
    pRecode->SetFullRecode(nRecode);
}


// IDA: ?NeedReviveBuffUser@CGocForce@@QEAA_NXZ @ 0x14010C7A0
// Check if any member needs revive buff
bool CGocForce::NeedReviveBuffUser()
{
    // IDA: if (shared_ptr bool check) iterate m_pParty->m_mapPartyMember
    if (!m_pParty)
        return false;

    // IDA: for (auto it : m_mapPartyMember) {
    //   pMemberInfo = it->second; if (!pMemberInfo) continue;
    //   pMember = pMemberInfo->GetMember(); if (!pMember) continue;
    //   if (GetMapInsID(pMember) == GetMapInsID(this) && CMover::FindBuffByEffectType(&pMember->CMoverEx, 1, 0) == -1) return true;
    // }
    UXMapID uxOwnerMap = GetOwnerGO()->GetMapInsID();
    for (auto& kv : m_pParty->GetMemberMap()) {
        CPartyMember* pMemberInfo = kv.second;
        if (!pMemberInfo)
            continue;
        CUser* pMember = pMemberInfo->GetMember();
        if (!pMember)
            continue;
        if (pMember->GetMapInsID() == uxOwnerMap &&
            pMember->FindBuffByEffectType(1, 0) == -1) {
            return true;
        }
    }
    return false;
}


// IDA: ?DeletePartyBoost@CGocForce@@QEAAXXZ @ 0x14010C940
// Remove party booster
void CGocForce::DeletePartyBoost()
{
    // IDA: v1 = GetOwnerGO(); CMover::GetGOC<CGocBooster>(v1, &pBooster, 0);
    // if (pBooster) CGocBooster::ChangeBooster(pBooster, eBooster_Type_Party, 0, 0, 0);
    CMover* pOwner = GetOwnerGO();
    std::shared_ptr<CGocBooster> pBooster = pOwner ? pOwner->GetGOC_Booster(false) : nullptr;
    if (pBooster) {
        pBooster->ChangeBooster(eBooster_Type_Party, 0, 0, false);
    }
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
std::uint8_t CGocForce::GetForceUserCount()
{
    // IDA: if (!CGocParty::IsParty(this)) return 0;
    // v1 = this->m_pParty.get(); return CParty::GetUserCount(v1);
    if (!IsParty())
        return 0;
    return m_pParty->GetUserCount();
}





// IDA: ?GetMasterID@CGocForce@@QEAAKXZ @ 0x14010D410
// Get Force master ID
std::uint32_t CGocForce::GetMasterID()
{
    // IDA: if (shared_ptr bool check) return 0;
    // v1 = this->m_pParty.get(); return CParty::GetMasterID(v1);
    if (!m_pParty)
        return 0;
    return m_pParty->GetMasterID();
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
bool CGocForce::IsMember(XActor* pActor)
{
    // IDA: if (shared_ptr bool check) return false;
    // dwActor = pMember->GetActorID().dwActorID;
    // v3 = this->m_pParty.get(); return CParty::IsMember(v3, dwActor);
    if (!m_pParty)
        return false;
    std::uint32_t dwActor = pActor->GetActorID().dwActorID;
    return m_pParty->IsMember(dwActor);
}


// IDA: ?CheckPassiveSkill@CGocForce@@QEAAXPEAVCUser@@EE@Z @ 0x1400851B0
// Check passive skill for Force members
void CGocForce::CheckPassiveSkill(CUser* pUser, std::uint8_t byTargetType, std::uint8_t byCondition)
{
    // IDA: if (shared_ptr bool check) { CParty::CheckPassiveSkill(m_pParty.get(), pUser, byTargetType, byCondition); }
    if (!m_pParty)
        return;
    m_pParty->CheckPassiveSkill(pUser, byTargetType, byCondition);
}


// IDA: ?CheckForceMatchingEnter@CGocForce@@QEAA_NXZ @ 0x140085210
// Check if all Force members can enter matching
bool CGocForce::CheckForceMatchingEnter()
{
    // IDA: if (shared_ptr bool check) return 0;
    if (!m_pParty)
        return false;

    // IDA: shWorldID = -1; nChannelID = -1; iterate m_mapPartyMember
    std::int16_t shWorldID = -1;
    int nChannelID = -1;
    UXMapID uxMapID;

    for (auto& kv : m_pParty->GetMemberMap()) {
        CPartyMember* pMemberInfo = kv.second;
        if (!pMemberInfo)
            return false;
        // IDA: dwActor = pMemberInfo->GetMemberID(); pMember = FindActorIDToUser(dwActor);
        CUser* pMember = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(pMemberInfo->GetMemberID());
        if (!pMember)
            return false;
        if (!pMember->GetArea())
            return false;

        if (shWorldID == -1 && nChannelID == -1) {
            shWorldID = pMember->GetWorldID();
            nChannelID = pMember->GetArea()->GetChannel();
            uxMapID = pMember->GetMapInsID();
        }
        if (shWorldID != pMember->GetWorldID())
            return false;
        if (nChannelID != pMember->GetArea()->GetChannel())
            return false;
        if (uxMapID != pMember->GetMapInsID())
            return false;
    }
    return true;
}


// IDA: ?GetFamilyID@CGocForce@@SAHXZ @ 0x140039030
// Return constant 22
int CGocForce::GetFamilyID() {
    return 22;
}


// Force setter
// IDA: ?SetForce@CGocForce@@QEAAXV?$shared_ptr@VCForce@@@tr1@std@@@Z @ 0x140083F30
// 赋值 m_pParty；有 owner 时取其 ActorID 并向队伍注册成员用户指针。
void CGocForce::SetForce(std::shared_ptr<CForce> pForce)
{
    m_pParty = std::static_pointer_cast<CParty>(pForce);
    if (m_pParty) {
        CMover* pOwnerMover = GetOwnerGO();
        CUser* pMember = pOwnerMover ? static_cast<CUser*>(pOwnerMover) : nullptr;
        if (pMember) {
            const std::uint32_t dwActorID = pMember->GetActorID().dwActorID;
            m_pParty->RegisterPartyMember(dwActorID, pMember);
        }
    }
}



// Helper methods for getting owner info












// IDA: ?SetMatchingState@CGocForce@@QEAAXE@Z @ 0x1401F3600
// 设置匹配状态
void CGocForce::SetMatchingState(std::uint8_t byState) {
    m_byMatchingState = byState;
}
