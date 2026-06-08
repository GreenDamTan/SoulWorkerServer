// GocForce.cpp
// CGocForce implementation - IDA verified implementations
// All functions decompiled from GameServer.exe port 10004
// Verified: no (pending build verification)

#include "GocForce.h"
#include "GOComponent.h"
#include "GocNetwork.h"
#include "GocRecode.h"
#include "GocBooster.h"
#include "GocAttribute.h"
#include "User.h"
#include "XCore/XServer/XSendPacket.h"
#include "XCore/XServer/XSendDBPacket.h"
#include "XCore/XServer/XGameServer.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <cstdint>
#include <ctime>

// Forward declarations
class CForce;
class CForceMember;
class CParty;
class CCommunitySocket;

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
bool CGocForce::IsFull() const {
    // IDA: Check m_pForce validity, then CParty::GetUserCount() == 8
    if (!IsParty()) {
        return false;
    }
    auto pForce = GetForce();
    if (!pForce) {
        return false;
    }
    return pForce->GetUserCount() == 8;
}

// IDA: ?IsMaster@CGocForce@@QEAA_NK@Z @ 0x140083160
// Check if given UCID is the Force master
bool CGocForce::IsMaster(std::uint32_t dwUCID) const {
    // IDA: Check m_pForce validity, then CWayPoint::GetCurID() == dwUCID
    if (!IsParty()) {
        return false;
    }
    auto pForce = GetForce();
    if (!pForce) {
        return false;
    }
    return pForce->GetMasterID() == dwUCID;
}

// IDA: ?SendForceInfo@CGocForce@@QEAAXE@Z @ 0x140084310
// Send PS_FORCE_INFO packet to Force owner
void CGocForce::SendForceInfo(std::uint8_t byUpdateType) {
    if (!IsParty()) {
        return;
    }

    // IDA: Build PS_FORCE_INFO from CParty::GetPartyInfo
    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    PS_FORCE_INFO stForceInfo;
    pForce->GetPartyInfo(&stForceInfo);
    stForceInfo.byUpdateType = byUpdateType;
    stForceInfo.byForceType = pForce->GetForceType();

    // IDA: Send packet (main=0x2E, sub=9)
    XSendPacket xSendPacket(0x2E, 9);
    xSendPacket << stForceInfo;

    CUser* pOwnerUser = GetOwnerAsUser();
    if (pOwnerUser) {
        CGocNetwork::Send(pOwnerUser, &xSendPacket);
    }
}

// IDA: ?IsMatchingDate@CGocForce@@QEAA_NXZ @ 0x140085160
// Check if matching date + 180 >= current date
bool CGocForce::IsMatchingDate() const {
    if (m_biMatchingDate == 0) {
        return false;
    }

    // IDA: v3 = m_biMatchingDate + 180
    // IDA: return v3 >= XGameServer::GetCurDate()
    __int64 expiryDate = m_biMatchingDate + 180;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return true;
    }
    return expiryDate >= pServer->GetCurDate();
}

// IDA: ?KickOut@CGocForce@@QEAA_NKPEAVCUser@@@Z @ 0x1400846F0
// Complete kick out logic with validation and error handling
bool CGocForce::KickOut(std::uint32_t dwActorID, CUser* pUser) {
    // IDA: Validate party exists
    if (!IsParty()) {
        CGocNetwork::SendErrorMessage(GetOwnerAsUser(), 0x2E, 4, 0xCF72);
        return false;
    }

    // IDA: Check if kicking self
    UXActorID ownerActorID;
    GetOwnerActorID(ownerActorID);
    if (ownerActorID.GetID() == dwActorID) {
        CGocNetwork::SendErrorMessage(GetOwnerAsUser(), 0x2E, 4, 0xCF8E);
        return false;
    }

    // IDA: Check if kicker is master
    if (!IsMaster(GetOwnerUCID())) {
        CGocNetwork::SendErrorMessage(GetOwnerAsUser(), 0x2E, 4, 0xCF74);
        return false;
    }

    auto pForce = GetForce();
    if (!pForce) {
        CGocNetwork::SendErrorMessage(GetOwnerAsUser(), 0x2E, 4, 0xCF72);
        return false;
    }

    // IDA: Get target member
    CPartyMember* pMember = pForce->GetMember(dwActorID);
    if (!pMember) {
        CGocNetwork::SendErrorMessage(GetOwnerAsUser(), 0x2E, 4, 0xCF13);
        return false;
    }

    // IDA: Check if target is master (can't kick master)
    if (pForce->GetMasterID() == dwActorID) {
        CGocNetwork::SendErrorMessage(GetOwnerAsUser(), 0x2E, 4, 0xCF6F);
        return false;
    }

    // IDA: Check if member exists in party
    if (!pForce->IsMember(dwActorID)) {
        CGocNetwork::SendErrorMessage(GetOwnerAsUser(), 0x2E, 4, 0xCF72);
        return false;
    }

    // IDA: Check maze type restrictions
    TB_MAZE_INFO* pTBmazeInfo = pMember->GetTB_MAZE_INFO();
    if (pTBmazeInfo && (!pTBmazeInfo->Maze_Type || pTBmazeInfo->Maze_Type == 2 || 
        pTBmazeInfo->Maze_Type == 8 || pTBmazeInfo->Maze_Type == 9)) {
        CGocNetwork::SendErrorMessage(GetOwnerAsUser(), 0x2E, 4, 0xCF80);
        return false;
    }

    // IDA: Send PS_FORCE_LEAVE packet to CommunitySocket (main=0xFA, sub=3)
    PS_FORCE_LEAVE stForceLeave;
    stForceLeave.dwForceID = GetForceID();
    stForceLeave.dwLeaveMember = dwActorID;
    stForceLeave.bKickout = 1;

    XSendPacket xSendPacket(0xFA, 3);
    xSendPacket << stForceLeave;
    xSendPacket << ownerActorID.GetID();
    xSendPacket << GetOwnerUAID();
    xSendPacket << GetOwnerLevel();
    xSendPacket << pMember->GetLevel();

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer && pUser) {
        pServer->GetCommunitySocket().SendCmd(&xSendPacket, pUser, 0x2E, 4);
    }

    return true;
}

// IDA: ?ChangeMaster@CGocForce@@QEAAXK@Z @ 0x140084C80
// Change master with validation
void CGocForce::ChangeMaster(std::uint32_t dwMaster) {
    if (!IsParty()) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    // IDA: Check if requester is master
    UXActorID ownerActorID;
    GetOwnerActorID(ownerActorID);
    if (pForce->GetMasterID() != ownerActorID.GetID()) {
        CGocNetwork::SendErrorMessage(GetOwnerAsUser(), 0x2E, 3, 0xCF6F);
        return;
    }

    // IDA: Check if new master is member
    if (!pForce->IsMember(dwMaster)) {
        CGocNetwork::SendErrorMessage(GetOwnerAsUser(), 0x2E, 3, 0xCF72);
        return;
    }

    // IDA: Send PS_FORCE_CHANGE_MASTER to CommunitySocket (main=0xFA, sub=4)
    PS_FORCE_CHANGE_MASTER stChangeMaster;
    stChangeMaster.dwReqActorID = ownerActorID.GetID();
    stChangeMaster.dwNewMasterID = dwMaster;
    stChangeMaster.dwForceID = GetForceID();
    stChangeMaster.nErrorCode = 0;

    XSendPacket xSendPacket(0xFA, 4);
    xSendPacket << stChangeMaster;

    CUser* pOwnerUser = GetOwnerAsUser();
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer && pOwnerUser) {
        pServer->GetCommunitySocket().SendCmd(&xSendPacket, pOwnerUser, 0x2E, 3);
    }
}

// IDA: ?Leave@CGocForce@@QEAAXXZ @ 0x140084480
// Leave force with packet to CommunitySocket
void CGocForce::Leave() {
    if (!IsParty()) {
        return;
    }

    CUser* pOwnerUser = GetOwnerAsUser();
    if (!pOwnerUser || !pOwnerUser->GetArea()) {
        return;
    }

    // IDA: Build PS_FORCE_LEAVE packet
    PS_FORCE_LEAVE stForceLeave;
    stForceLeave.dwForceID = GetForceID();
    UXActorID ownerActorID;
    GetOwnerActorID(ownerActorID);
    stForceLeave.dwLeaveMember = ownerActorID.GetID();
    stForceLeave.bKickout = 0;

    // IDA: Send to CommunitySocket (main=0xFA, sub=3)
    XSendPacket xSendPacket(0xFA, 3);
    xSendPacket << stForceLeave;
    xSendPacket << ownerActorID.GetID();
    xSendPacket << GetOwnerUAID();
    xSendPacket << GetOwnerLevel();
    xSendPacket << GetOwnerLevel();

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        pServer->GetCommunitySocket().SendCmd(&xSendPacket, pOwnerUser, 0x2E, 5);
    }
}

// IDA: ?Logout@CGocForce@@QEAAXXZ @ 0x140084010
// Logout handling for Force/non-Force members
void CGocForce::Logout() {
    if (IsParty()) {
        // IDA: Force member logout
        UXActorID ownerActorID;
        GetOwnerActorID(ownerActorID);

        auto pForce = GetForce();
        if (pForce) {
            CPartyMember* pMember = pForce->GetMember(ownerActorID.GetID());
            if (pMember) {
                ST_UPDATE_FORCE_MEMBER stForceMember;
                stForceMember.dwForceID = GetForceID();
                pMember->Logout(&stForceMember);

                XSendPacket xSendPacket(0xFA, 5);
                xSendPacket << stForceMember;

                XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                if (pServer) {
                    pServer->GetCommunitySocket().SendCheck(&xSendPacket);
                }
            }
        }
    } else {
        // IDA: Non-Force logout (main=0xFA, sub=0x14)
        CUser* pOwnerUser = GetOwnerAsUser();
        if (!pOwnerUser) {
            return;
        }

        UXActorID ownerActorID;
        GetOwnerActorID(ownerActorID);

        XSendPacket xSendPacket(0xFA, 0x14);
        xSendPacket << ownerActorID.GetID();
        xSendPacket << 2;
        xSendPacket << GetOwnerUAID();
        xSendPacket << GetOwnerLevel();

        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) {
            pServer->GetCommunitySocket().SendCheck(&xSendPacket);
        }
    }

    // IDA: LogHelper::LogDebug("game.party", "<FORCE> Logout Req ( %d )", dwActorID)
    Clear();
}

// IDA: ?SetHP@CGocForce@@QEAAXH@Z @ 0x140083970
// Set member HP with MapInsID
void CGocForce::SetHP(int nHP) {
    if (!IsParty()) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    UXActorID ownerActorID;
    GetOwnerActorID(ownerActorID);
    UXMapID uxMapID;
    GetOwnerMapInsID(uxMapID);

    pForce->SetMemberHP(ownerActorID.GetID(), uxMapID, nHP);
}

// IDA: ?SetMaxHP@CGocForce@@QEAAXH@Z @ 0x1400838B0
// Set member max HP with MapInsID
void CGocForce::SetMaxHP(int nMaxHP) {
    if (!IsParty()) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    UXActorID ownerActorID;
    GetOwnerActorID(ownerActorID);
    UXMapID uxMapID;
    GetOwnerMapInsID(uxMapID);

    pForce->SetMemberMaxHP(ownerActorID.GetID(), uxMapID, nMaxHP);
}

// IDA: ?SetLevel@CGocForce@@QEAAXH@Z @ 0x140083730
// Set member level
void CGocForce::SetLevel(int nLevel) {
    if (!IsParty()) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    UXActorID ownerActorID;
    GetOwnerActorID(ownerActorID);
    pForce->SetMemberLevel(ownerActorID.GetID(), nLevel);
}

// IDA: ?SetAwaken@CGocForce@@QEAAXE@Z @ 0x1400837B0
// Set member awaken grade
void CGocForce::SetAwaken(std::uint8_t byAwaken) {
    if (!IsParty()) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    UXActorID ownerActorID;
    GetOwnerActorID(ownerActorID);
    pForce->SetMemberAwaken(ownerActorID.GetID(), byAwaken);
}

// IDA: ?SetProfilePhoto@CGocForce@@QEAAXK@Z @ 0x140083830
// Set member profile photo
void CGocForce::SetProfilePhoto(std::uint32_t dwPhotoID) {
    if (!IsParty()) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    UXActorID ownerActorID;
    GetOwnerActorID(ownerActorID);
    pForce->SetMemberProfilePhoto(ownerActorID.GetID(), dwPhotoID);
}

// IDA: ?SetMapID@CGocForce@@QEAAXHHTUXMapID@@@Z @ 0x140083690
// Set member map information
void CGocForce::SetMapID(int nMapID, int nChannel, const UXMapID& uxMapID) {
    if (!IsParty()) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    UXActorID ownerActorID;
    GetOwnerActorID(ownerActorID);
    pForce->SetMemberMapID(ownerActorID.GetID(), nMapID, nChannel, uxMapID);
}

// IDA: ?UpdatePartyBooster@CGocForce@@QEAAXXZ @ 0x140084EE0
// Update Force booster
void CGocForce::UpdatePartyBooster() {
    if (!IsParty()) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    // IDA: Direct call to CForce::UpdateForceBooster(0)
    pForce->UpdateForceBooster(0);
}

// IDA: ?UpdatePartyBoosterByCount@CGocForce@@QEAAXHH@Z @ 0x140084F30
// Update booster based on party/friend count
void CGocForce::UpdatePartyBoosterByCount(int nPartyCount, int nFriendCount) {
    CUser* pOwnerUser = GetOwnerAsUser();
    if (!pOwnerUser) {
        return;
    }

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    // IDA: Iterate TB_PARTYEXP_MEMBER table
    for (auto& iter : pServer->GetResourceMgr().GetTB_PARTYEXP_MEMBER_Map()) {
        if (iter.second.PartyCount == nPartyCount && iter.second.FriendCount == nFriendCount) {
            pOwnerUser->ChangeBooster(eBooster_Type_Party, iter.second.BoosterID);
            return;
        }
    }
    pOwnerUser->ChangeBooster(eBooster_Type_Party, 0);
}

// IDA: ?ReserveReviveAll@CGocForce@@QEAAXKK@Z @ 0x140083350
// Reserve revive for all members
void CGocForce::ReserveReviveAll(std::uint32_t dwActorID, std::uint32_t dwID) {
    CUser* pOwnerUser = GetOwnerAsUser();
    if (!pOwnerUser) {
        return;
    }

    // IDA: Create revive packet (main=3, sub=0x48)
    XSendPacket xSendPacket(3, 0x48);
    xSendPacket << dwActorID;
    xSendPacket << dwID;

    // IDA: If owner dead or HP <= 0, set reserve revive
    if (pOwnerUser->IsDie() || pOwnerUser->GetHP() <= 0) {
        pOwnerUser->SetReserveRevive(1);
        pOwnerUser->DoReserveRevive();
    }

    if (!IsParty()) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    // IDA: Get owner's TBMapID and MapInsID
    std::uint16_t ownerTBMapID = pOwnerUser->GetTBMapID();
    UXMapID ownerMapInsID;
    pOwnerUser->GetMapInsID(ownerMapInsID);

    // IDA: Iterate Force members
    for (auto& it : pForce->GetMembers()) {
        CUser* pMember = it.second->GetUser();
        if (!pMember) {
            continue;
        }

        // IDA: Check same TBMapID
        if (pMember->GetTBMapID() != ownerTBMapID) {
            continue;
        }

        // IDA: Check same MapInsID via ThreadLocalData::IsThreadArea
        UXMapID memberMapInsID;
        pMember->GetMapInsID(memberMapInsID);
        if (!ThreadLocalData::IsThreadArea(memberMapInsID)) {
            continue;
        }

        // IDA: If member dead, set reserve and BridgeSend packet
        if (pMember->IsDie()) {
            pMember->SetReserveRevive(1);
            pMember->BridgeSend(&xSendPacket);
        }
    }
}

// IDA: ?LoadRecode@CGocForce@@QEAAXXZ @ 0x14010B430
// Load maze recode from Force to CGocRecode
void CGocForce::LoadRecode() {
    if (!IsParty()) {
        return;
    }

    CUser* pOwnerUser = GetOwnerAsUser();
    if (!pOwnerUser) {
        return;
    }

    auto pRecode = pOwnerUser->GetGOC<CGocRecode>();
    if (!pRecode) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    UXActorID ownerActorID;
    GetOwnerActorID(ownerActorID);

    int nRecode[10];
    pForce->GetMazeRecode(ownerActorID.GetID(), nRecode);
    pRecode->SetFullRecode(nRecode);
}

// IDA: ?NeedReviveBuffUser@CGocForce@@QEAA_NXZ @ 0x14010C7A0
// Check if any member needs revive buff
bool CGocForce::NeedReviveBuffUser() {
    if (!IsParty()) {
        return false;
    }

    CUser* pOwnerUser = GetOwnerAsUser();
    if (!pOwnerUser) {
        return false;
    }

    UXMapID ownerMapInsID;
    pOwnerUser->GetMapInsID(ownerMapInsID);

    auto pForce = GetForce();
    if (!pForce) {
        return false;
    }

    // IDA: Iterate Force members
    for (auto& it : pForce->GetMembers()) {
        CUser* pMember = it.second->GetUser();
        if (!pMember) {
            continue;
        }

        // IDA: Check same MapInsID
        UXMapID memberMapInsID;
        pMember->GetMapInsID(memberMapInsID);
        if (memberMapInsID != ownerMapInsID) {
            continue;
        }

        // IDA: Check CMover::FindBuffByEffectType(1, 0) == -1
        if (pMember->FindBuffByEffectType(1, 0) == -1) {
            return true;
        }
    }

    return false;
}

// IDA: ?DeletePartyBoost@CGocForce@@QEAAXXZ @ 0x14010C940
// Remove party booster
void CGocForce::DeletePartyBoost() {
    CUser* pOwnerUser = GetOwnerAsUser();
    if (!pOwnerUser) {
        return;
    }

    auto pBooster = pOwnerUser->GetGOC<CGocBooster>();
    if (!pBooster) {
        return;
    }

    pBooster->ChangeBooster(eBooster_Type_Party, 0, 0, 0);
}

// IDA: ?GetForceMember@CGocForce@@QEAAXPEAVCUser@@AEAV?$vector@PEAVCForceMember@@V?$allocator@PEAVCForceMember@@@std@@@std@@_N@Z @ 0x14010C9B0
// Get Force member list with filtering
void CGocForce::GetForceMember(CUser* pUser, std::vector<CForceMember*>& vecMember, bool bIncludeOffline) {
    vecMember.clear();

    if (!IsParty()) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    UXActorID ownerActorID;
    GetOwnerActorID(ownerActorID);
    UXMapID ownerMapInsID;
    if (pUser) {
        pUser->GetMapInsID(ownerMapInsID);
    }

    // IDA: Iterate Force members
    for (auto& it : pForce->GetMembers()) {
        CForceMember* pMember = it.second;
        if (!pMember) {
            continue;
        }

        if (bIncludeOffline) {
            vecMember.push_back(pMember);
            continue;
        }

        // IDA: Filter by: not dead, same MapInsID, same actor or maze or distance < 5000
        CUser* pMemberUser = pMember->GetUser();
        if (!pMemberUser || pMemberUser->IsDie()) {
            continue;
        }

        UXMapID memberMapInsID;
        pMemberUser->GetMapInsID(memberMapInsID);
        if (memberMapInsID != ownerMapInsID) {
            continue;
        }

        // IDA: Check if same actor or maze or within distance
        if (pMember->GetActorID() == ownerActorID.GetID() || pUser->IsMaze()) {
            vecMember.push_back(pMember);
            continue;
        }

        // IDA: Check distance < 5000
        hkvVec3 ownerPos = pUser->GetPosition();
        hkvVec3 memberPos = pMemberUser->GetPosition();
        float fDistance = ownerPos.Dist(memberPos);
        if (fDistance < 5000.0f) {
            vecMember.push_back(pMember);
        }
    }
}

// IDA: ?GetForceUserCount@CGocForce@@QEAAEXZ @ 0x14010D330
// Get online member count
std::uint8_t CGocForce::GetForceUserCount() const {
    if (!IsParty()) {
        return 0;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return 0;
    }

    return static_cast<std::uint8_t>(pForce->GetUserCount());
}

// IDA: ?GetMasterID@CGocForce@@QEAAKXZ @ 0x14010D410
// Get Force master ID
std::uint32_t CGocForce::GetMasterID() const {
    if (!IsParty()) {
        return 0;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return 0;
    }

    return pForce->GetMasterID();
}

// IDA: ?SetExp@CGocForce@@QEAAXPEAVCUser@@MH@Z @ 0x140083A30
// Complex experience distribution logic
void CGocForce::SetExp(CUser* pUser, float fExp, int nExpType) {
    if (!IsParty() || !pUser) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    // IDA: Calculate level gap
    std::uint8_t nLevelGap = pForce->GetLevelGap();

    // IDA: Get TB_PARTYEXP_LEVEL multiplier
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) {
        return;
    }

    TB_PARTYEXP_LEVEL* pTBPartyExpLevel = pServer->GetResourceMgr().GetTB_PARTYEXP_LEVEL(nLevelGap);
    if (!pTBPartyExpLevel) {
        return;
    }

    float fTotalExp = fExp * pTBPartyExpLevel->LevelInterval_Value;

    // IDA: Calculate monster level interval
    char cMonsterInterval = pForce->GetCommonThreadIndex() - nExpType;

    TB_PARTYEXP_MOB* pTBPartyExpMob = pServer->GetResourceMgr().GetTB_PARTYEXP_MOB(cMonsterInterval);
    if (!pTBPartyExpMob) {
        return;
    }

    fTotalExp *= pTBPartyExpMob->Mob_IntervalValue;

    // IDA: Get Force member list
    std::vector<CForceMember*> vecExpMember;
    GetForceMember(pUser, vecExpMember, false);

    // IDA: For each member, calculate and set exp
    for (auto* pMember : vecExpMember) {
        CUser* pMemberUser = pMember->GetUser();
        if (!pMemberUser) {
            continue;
        }

        float fForceExp = fTotalExp;

        // IDA: Get special effect EXP_ADD_RAT
        auto pAttr = pMemberUser->GetGOC<CGocAttribute>();
        if (pAttr) {
            float fRate = pAttr->GetSpecialEffect(EFFECT_SPECIAL_EXP_ADD_RAT) * 0.01f;

            // IDA: Get booster EXP rate
            auto pBooster = pMemberUser->GetGOC<CGocBooster>();
            if (pBooster) {
                fRate += pBooster->GetTotalRate(eBooster_Effect_IncExp);
            }

            // IDA: Get league skill effect
            float fLeagueRate = 0.0f;
            if (pMemberUser->IsLeagueSkill(4)) {
                fLeagueRate = pMemberUser->GetLeagueSkillEffectValue(4) * 0.01f;
            }

            float fBonus = fForceExp * (fRate + fLeagueRate);
            fForceExp += fBonus;

            pAttr->SetExp(fForceExp, fBonus, 1);
        }
    }
}

// IDA: ?IsMember@CGocForce@@QEAA_NPEAVXActor@@@Z @ 0x14010BBB0
// Check if actor is Force member
bool CGocForce::IsMember(XActor* pActor) const {
    if (!IsParty() || !pActor) {
        return false;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return false;
    }

    UXActorID actorID;
    pActor->GetActorID(actorID);
    return pForce->IsMember(actorID.GetID());
}

// IDA: ?CheckPassiveSkill@CGocForce@@QEAAXPEAVCUser@@EE@Z @ 0x1400851B0
// Check passive skill for Force members
void CGocForce::CheckPassiveSkill(CUser* pUser, std::uint8_t byTargetType, std::uint8_t byCondition) {
    if (!IsParty()) {
        return;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return;
    }

    // IDA: Direct call to CParty::CheckPassiveSkill
    pForce->CheckPassiveSkill(pUser, byTargetType, byCondition);
}

// IDA: ?CheckForceMatchingEnter@CGocForce@@QEAA_NXZ @ 0x140085210
// Check if all Force members can enter matching
bool CGocForce::CheckForceMatchingEnter() const {
    if (!IsParty()) {
        return false;
    }

    auto pForce = GetForce();
    if (!pForce) {
        return false;
    }

    __int16 shWorldID = -1;
    int nChannelID = -1;
    UXMapID uxBaseMapID;

    // IDA: Check if all members are in same WorldID, ChannelID, MapInsID
    for (auto& it : pForce->GetMembers()) {
        CUser* pMember = it.second->GetUser();
        if (!pMember || !pMember->GetArea()) {
            return false;
        }

        if (shWorldID == -1 && nChannelID == -1) {
            shWorldID = pMember->GetWorldID();
            nChannelID = pMember->GetArea()->GetChannel();
            pMember->GetMapInsID(uxBaseMapID);
        }

        if (shWorldID != pMember->GetWorldID()) {
            return false;
        }
        if (nChannelID != pMember->GetArea()->GetChannel()) {
            return false;
        }

        UXMapID memberMapInsID;
        pMember->GetMapInsID(memberMapInsID);
        if (uxBaseMapID != memberMapInsID) {
            return false;
        }
    }

    return true;
}

// IDA: ?GetFamilyID@CGocForce@@SAHXZ @ 0x140039030
// Return constant 22
int CGocForce::GetFamilyID() {
    return 22;
}

// Force getter
std::shared_ptr<CForce> CGocForce::GetForce() const {
    return std::static_pointer_cast<CForce>(m_pParty);
}

// Force setter
void CGocForce::SetForce(std::shared_ptr<CForce> pForce) {
    m_pParty = std::static_pointer_cast<CParty>(pForce);
}

// Helper methods for getting owner info
CUser* CGocForce::GetOwnerAsUser() const {
    XActor* pActor = GetOwnerActor();
    if (!pActor) {
        return nullptr;
    }
    return dynamic_cast<CUser*>(pActor);
}

void CGocForce::GetOwnerActorID(UXActorID& actorID) const {
    XActor* pActor = GetOwnerActor();
    if (pActor) {
        pActor->GetActorID(actorID);
    }
}

std::uint32_t CGocForce::GetOwnerUCID() const {
    CUser* pUser = GetOwnerAsUser();
    return pUser ? pUser->GetUCID() : 0;
}

std::uint32_t CGocForce::GetOwnerUAID() const {
    CUser* pUser = GetOwnerAsUser();
    return pUser ? pUser->GetUAID() : 0;
}

std::uint8_t CGocForce::GetOwnerLevel() const {
    CUser* pUser = GetOwnerAsUser();
    return pUser ? pUser->GetLevel() : 0;
}

void CGocForce::GetOwnerMapInsID(UXMapID& mapID) const {
    XActor* pActor = GetOwnerActor();
    if (pActor) {
        pActor->GetMapInsID(mapID);
    }
}
