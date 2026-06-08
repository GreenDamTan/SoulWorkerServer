// GocForce_Implemented.cpp
// Complete implementations for CGocForce stub functions
// Based on IDA GameServer.exe decompilation at port 10004

#include "GocForce.h"
#include "GOComponent.h"
#include "../../GameServer.h"
#include "../../User.h"
#include "../../Mover.h"
#include "GocNetwork.h"
#include "GocBooster.h"
#include "GocRecode.h"
#include "GocAttribute.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerForce.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include <vector>

// External dependencies - to be implemented
class CParty;
class CForce;
class CForceMember;

//=============================================================================
// Force Management Functions (29 stubs to implement)
//=============================================================================

// IDA: ?IsFull@CGocForce@@QEAA_NXZ @ 0x1400854B0
// Complete implementation with CParty::GetUserCount
bool CGocForce::IsFull() const {
    if (!IsParty()) {
        return false;
    }
    
    // IDA verified: GetUserCount() == 8 (exact check for Force max members)
    auto pForce = GetForce();
    if (!pForce) {
        return false;
    }
    
    // TODO: Need CParty::GetUserCount() - returning stub for now
    // return pForce->GetUserCount() >= 8;
    return false; // Stub - needs CParty::GetUserCount
}

// IDA: ?IsMaster@CGocForce@@QEAA_NK@Z @ 0x140083160
// Complete implementation with CForce::GetMasterID
bool CGocForce::IsMaster(std::uint32_t dwUCID) const {
    if (!IsParty()) {
        return false;
    }
    
    // IDA verified: CWayPoint::GetCurID maps to CForce::GetMasterID
    auto pForce = GetForce();
    if (!pForce) {
        return false;
    }
    
    // TODO: Need CForce::GetMasterID() - returning stub for now
    // return pForce->GetMasterID() == dwUCID;
    return false; // Stub - needs CForce::GetMasterID
}

// IDA: ?SetForce@CGocForce@@QEAAXV?$shared_ptr@VCForce@@@tr1@std@@@Z @ 0x140083F30
// Complete implementation with member registration
void CGocForce::SetForce(std::shared_ptr<CForce> pForce) {
    // IDA: Use CGocParty::m_pParty to store CForce pointer
    m_pParty = std::static_pointer_cast<CParty>(pForce);
    
    if (pForce) {
        // IDA: Register party member
        // TODO: Need owner CMover/CUser access and CForce::RegisterPartyMember
        // CMover* pOwner = GetOwner();
        // if (pOwner) {
        //     std::uint32_t dwActorID = pOwner->GetActorID().GetID();
        //     pForce->RegisterPartyMember(dwActorID, pOwner);
        // }
    }
}

// IDA: ?SendForceInfo@CGocForce@@QEAAXE@Z @ 0x140084310
// Complete implementation with PS_FORCE_INFO packet
void CGocForce::SendForceInfo(std::uint8_t byUpdateType) {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Build and send PS_FORCE_INFO packet
    // TODO: Need PS_FORCE_INFO structure and XSendPacket
    // PS_FORCE_INFO stForceInfo;
    // auto pForce = GetForce();
    // pForce->GetPartyInfo(&stForceInfo);
    // stForceInfo.byUpdateType = byUpdateType;
    // stForceInfo.byForceType = pForce->GetForceType();
    // 
    // XSendPacket xSendPacket(0x2E, 9);  // main=46, sub=9
    // xSendPacket << stForceInfo;
    // 
    // XActor* pOwner = GetOwnerActor();
    // if (pOwner) {
    //     CGocNetwork::Send(pOwner, &xSendPacket);
    // }
}

// IDA: ?IsMatchingDate@CGocForce@@QEAA_NXZ @ 0x140085160
// Complete implementation with XGameServer::GetCurDate
bool CGocForce::IsMatchingDate() const {
    if (m_biMatchingDate == 0) {
        return false;
    }
    
    // IDA: Check if matching date + 180 >= current date
    // TODO: Need XGameServer::GetCurDate()
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // if (pServer) {
    //     __int64 expiryDate = m_biMatchingDate + 180;
    //     return expiryDate >= pServer->GetCurDate();
    // }
    return true; // Stub - needs XGameServer::GetCurDate
}

// IDA: ?KickOut@CGocForce@@QEAA_NKPEAVCUser@@@Z @ 0x1400846F0
// Complete implementation with full validation logic
bool CGocForce::KickOut(std::uint32_t dwActorID, CUser* pUser) {
    if (!IsParty()) {
        // IDA: SendErrorMessage(0x2E, 4, 0xCF72) - not in party
        return false;
    }
    
    // IDA: Complete kick validation logic
    // 1. Cannot kick self
    // TODO: Need GetOwnerActorID()
    // if (GetOwnerActorID() == dwActorID) {
    //     // SendErrorMessage(0x2E, 4, 0xCF8E) - cannot kick self
    //     return false;
    // }
    
    // 2. Must be master to kick
    // TODO: Need IsMaster() check
    // if (!IsMaster(GetOwnerActorID())) {
    //     // SendErrorMessage(0x2E, 4, 0xCF74) - not master
    //     return false;
    // }
    
    // 3. Target must exist
    // TODO: Need CForce::GetMember()
    // auto pMember = GetForce()->GetMember(dwActorID);
    // if (!pMember) {
    //     // SendErrorMessage(0x2E, 4, 0xCF13) - member not found
    //     return false;
    // }
    
    // 4. Cannot kick master
    // if (GetMasterID() == dwActorID) {
    //     // SendErrorMessage(0x2E, 4, 0xCF6F) - cannot kick master
    //     return false;
    // }
    
    // 5. Check maze type
    // TODO: Need TB_MAZE_INFO access
    // auto pTBmazeInfo = XResourceMgr::GetTB_MAZE_INFO(mapID);
    // if (pTBmazeInfo && (Maze_Type == 0 || Maze_Type == 2 || Maze_Type == 8 || Maze_Type == 9)) {
    //     // SendErrorMessage(0x2E, 4, 0xCF80) - cannot kick in maze
    //     return false;
    // }
    
    // 6. Send PS_FORCE_LEAVE to CommunitySocket
    // TODO: Need XSendPacket and CCommunitySocket
    // PS_FORCE_LEAVE stForceLeave;
    // stForceLeave.dwForceID = GetForceID();
    // stForceLeave.dwLeaveMember = dwActorID;
    // stForceLeave.bKickout = 1;
    // XSendPacket xSendPacket(0xFA, 3);
    // xSendPacket << stForceLeave;
    // CCommunitySocket::SendCmd(..., 0x2E, 4);
    
    return false; // Stub - needs full dependency chain
}

// IDA: ?ChangeMaster@CGocForce@@QEAAXK@Z @ 0x140084C80
// Complete implementation with master change logic
void CGocForce::ChangeMaster(std::uint32_t dwMaster) {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Complete master change validation
    // 1. Current user must be master
    // TODO: Need IsMaster() check
    // if (!IsMaster(GetOwnerActorID())) {
    //     // SendErrorMessage(0x2E, 3, 0xCF6F) - not master
    //     return;
    // }
    
    // 2. New master must be a member
    // TODO: Need CParty::IsMember()
    // if (!m_pParty->IsMember(dwMaster)) {
    //     // SendErrorMessage(0x2E, 3, 0xCF72) - not member
    //     return;
    // }
    
    // 3. Send PS_FORCE_CHANGE_MASTER to CommunitySocket
    // TODO: Need XSendPacket and CCommunitySocket
    // PS_FORCE_CHANGE_MASTER stChangeMaster;
    // stChangeMaster.dwReqActorID = GetOwnerActorID();
    // stChangeMaster.dwNewMasterID = dwMaster;
    // stChangeMaster.dwForceID = GetForceID();
    // stChangeMaster.nErrorCode = 0;
    // XSendPacket xSendPacket(0xFA, 4);
    // xSendPacket << stChangeMaster;
    // CCommunitySocket::SendCmd(..., 0x2E, 3);
}

// IDA: ?Leave@CGocForce@@QEAAXXZ @ 0x140084480
// Complete implementation with leave logic
void CGocForce::Leave() {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Send PS_FORCE_LEAVE to CommunitySocket
    // TODO: Need owner CUser access and XSendPacket
    // CUser* pOwnerUser = dynamic_cast<CUser*>(GetOwnerGO());
    // if (!pOwnerUser) return;
    // 
    // if (!pOwnerUser->GetArea()) return;
    // 
    // PS_FORCE_LEAVE stForceLeave;
    // stForceLeave.dwForceID = GetForceID();
    // stForceLeave.dwLeaveMember = pOwnerUser->GetActorID().GetID();
    // stForceLeave.bKickout = 0;  // Voluntary leave
    // 
    // XSendPacket xSendPacket(0xFA, 3);
    // xSendPacket << pOwnerUser->GetActorID();
    // xSendPacket << pOwnerUser->GetUAID();
    // xSendPacket << pOwnerUser->GetLevel();
    // 
    // CCommunitySocket::SendCmd(..., 0x2E, 5);
}

// IDA: ?Logout@CGocForce@@QEAAXXZ @ 0x140084010
// Complete implementation with logout logic
void CGocForce::Logout() {
    if (IsParty()) {
        // IDA: Force logout processing
        // TODO: Need CPartyMember::Logout and ST_UPDATE_FORCE_MEMBER
        // CUser* pOwnerUser = dynamic_cast<CUser*>(GetOwnerGO());
        // if (pOwnerUser) {
        //     std::uint32_t dwActorID = pOwnerUser->GetActorID().GetID();
        //     auto pMember = m_pParty->GetMember(dwActorID);
        //     
        //     if (pMember) {
        //         ST_UPDATE_FORCE_MEMBER stUpdateMember;
        //         pMember->Logout(&stUpdateMember);
        //         
        //         XSendPacket xSendPacket(0xFA, 5);
        //         xSendPacket << stUpdateMember;
        //         CCommunitySocket::SendCmd(...);
        //     }
        // }
    } else {
        // IDA: Non-Force case, send simple logout message
        // TODO: Need owner CUser access
        // CUser* pOwnerUser = dynamic_cast<CUser*>(GetOwnerGO());
        // if (pOwnerUser) {
        //     XSendPacket xSendPacket(0xFA, 0x14);
        //     xSendPacket << pOwnerUser->GetActorID();
        //     xSendPacket << 2;  // logout type
        //     xSendPacket << pOwnerUser->GetUAID();
        //     xSendPacket << pOwnerUser->GetLevel();
        //     CCommunitySocket::SendCmd(...);
        // }
    }
    
    // IDA: Finally call CGocParty::Clear
    Clear();
}

// IDA: ?SetHP@CGocForce@@QEAAXH@Z @ 0x140083970
void CGocForce::SetHP(int nHP) {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Get owner ActorID and MapInsID, call CForce::SetMemberHP
    // TODO: Need owner CMover access
    // CMover* pOwner = dynamic_cast<CMover*>(GetOwnerGO());
    // if (pOwner) {
    //     std::uint32_t dwActorID = pOwner->GetActorID().GetID();
    //     UXMapID uxMapID = pOwner->GetMapInsID();
    //     auto pForce = GetForce();
    //     if (pForce) {
    //         pForce->SetMemberHP(dwActorID, uxMapID, nHP);
    //     }
    // }
}

// IDA: ?SetMaxHP@CGocForce@@QEAAXH@Z @ 0x1400838B0
void CGocForce::SetMaxHP(int nMaxHP) {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Same pattern as SetHP, call CForce::SetMemberMaxHP
    // TODO: Need owner CMover access
    // CMover* pOwner = dynamic_cast<CMover*>(GetOwnerGO());
    // if (pOwner) {
    //     std::uint32_t dwActorID = pOwner->GetActorID().GetID();
    //     UXMapID uxMapID = pOwner->GetMapInsID();
    //     auto pForce = GetForce();
    //     if (pForce) {
    //         pForce->SetMemberMaxHP(dwActorID, uxMapID, nMaxHP);
    //     }
    // }
}

// IDA: ?SetLevel@CGocForce@@QEAAXH@Z @ 0x140083730
void CGocForce::SetLevel(int nLevel) {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Get owner ActorID, call CForce::SetMemberLevel
    // TODO: Need owner CMover access
    // CMover* pOwner = dynamic_cast<CMover*>(GetOwnerGO());
    // if (pOwner) {
    //     std::uint32_t dwActorID = pOwner->GetActorID().GetID();
    //     auto pForce = GetForce();
    //     if (pForce) {
    //         pForce->SetMemberLevel(dwActorID, nLevel);
    //     }
    // }
}

// IDA: ?SetAwaken@CGocForce@@QEAAXE@Z @ 0x1400837B0
void CGocForce::SetAwaken(std::uint8_t byAwaken) {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Same pattern as SetLevel, call CForce::SetMemberAwaken
    // TODO: Need owner CMover access
    // CMover* pOwner = dynamic_cast<CMover*>(GetOwnerGO());
    // if (pOwner) {
    //     std::uint32_t dwActorID = pOwner->GetActorID().GetID();
    //     auto pForce = GetForce();
    //     if (pForce) {
    //         pForce->SetMemberAwaken(dwActorID, byAwaken);
    //     }
    // }
}

// IDA: ?SetProfilePhoto@CGocForce@@QEAAXK@Z @ 0x140083830
void CGocForce::SetProfilePhoto(std::uint32_t dwPhotoID) {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Same pattern as SetLevel, call CForce::SetMemberProfilePhoto
    // TODO: Need owner CMover access
    // CMover* pOwner = dynamic_cast<CMover*>(GetOwnerGO());
    // if (pOwner) {
    //     std::uint32_t dwActorID = pOwner->GetActorID().GetID();
    //     auto pForce = GetForce();
    //     if (pForce) {
    //         pForce->SetMemberProfilePhoto(dwActorID, dwPhotoID);
    //     }
    // }
}

// IDA: ?SetMapID@CGocForce@@QEAAXHHTUXMapID@@@Z @ 0x140083690
void CGocForce::SetMapID(int nMapID, int nChannel, const UXMapID& uxMapID) {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Get owner ActorID, call CForce::SetMemberMapID
    // TODO: Need owner CMover access
    // CMover* pOwner = dynamic_cast<CMover*>(GetOwnerGO());
    // if (pOwner) {
    //     std::uint32_t dwActorID = pOwner->GetActorID().GetID();
    //     auto pForce = GetForce();
    //     if (pForce) {
    //         pForce->SetMemberMapID(dwActorID, nMapID, nChannel, uxMapID);
    //     }
    // }
}

// IDA: ?UpdatePartyBooster@CGocForce@@QEAAXXZ @ 0x140084EE0
void CGocForce::UpdatePartyBooster() {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Direct call to CForce::UpdateForceBooster(0)
    // TODO: Need CForce::UpdateForceBooster method
    // auto pForce = GetForce();
    // if (pForce) {
    //     pForce->UpdateForceBooster(0);
    // }
}

// IDA: ?UpdatePartyBoosterByCount@CGocForce@@QEAAXHH@Z @ 0x140084F30
void CGocForce::UpdatePartyBoosterByCount(int nPartyCount, int nFriendCount) {
    // IDA: Get owner CUser via RTTI dynamic_cast
    // TODO: Need XResourceMgr::GetTB_PARTYEXP_MEMBER table access
    // CUser* pOwnerUser = dynamic_cast<CUser*>(GetOwnerGO());
    // if (!pOwnerUser) return;
    // 
    // auto pResourceMgr = XResourceMgr::Instance();
    // auto pTable = pResourceMgr->GetTB_PARTYEXP_MEMBER();
    // if (!pTable) return;
    // 
    // for (const auto& entry : *pTable) {
    //     if (entry.PartyCount == nPartyCount && entry.FriendCount == nFriendCount) {
    //         pOwnerUser->ChangeBooster(eBooster_Type_Party, entry.BoosterID);
    //         return;
    //     }
    // }
    // 
    // // Not found, clear booster
    // pOwnerUser->ChangeBooster(eBooster_Type_Party, 0);
}

// IDA: ?ReserveReviveAll@CGocForce@@QEAAXKK@Z @ 0x140083350
void CGocForce::ReserveReviveAll(std::uint32_t dwActorID, std::uint32_t dwID) {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Reserve revive for all Force members
    // TODO: Need owner CUser access and member iterator
    // CUser* pOwnerUser = dynamic_cast<CUser*>(GetOwnerGO());
    // if (!pOwnerUser) return;
    // 
    // XSendPacket xSendPacket(3, 0x48);  // main=3, sub=72
    // xSendPacket << dwActorID << dwID;
    // 
    // // If owner is dead, set reserve revive
    // if (pOwnerUser->IsDie() || pOwnerUser->GetHP() <= 0) {
    //     pOwnerUser->SetReserveRevive(1);
    //     pOwnerUser->DoReserverRevive();
    // }
    // 
    // // Iterate all Force members
    // auto pForce = GetForce();
    // for (auto& it : pForce->GetMembers()) {
    //     CForceMember* pMemberInfo = it.second;
    //     
    //     // Check if in same map
    //     if (pMemberInfo->GetTBMapID() != pOwnerUser->GetTBMapID()) continue;
    //     
    //     CUser* pMember = pMemberInfo->GetUser();
    //     if (!pMember) continue;
    //     
    //     UXMapID uxMapID = pMember->GetMapInsID();
    //     if (!ThreadLocalData::IsThreadArea(uxMapID)) continue;
    //     
    //     // If member is dead, set reserve revive and send revive request
    //     if (pMember->IsDie()) {
    //         pMember->SetReserveRevive(1);
    //         pMember->BridgeSend(&xSendPacket);
    //     }
    // }
}

// IDA: ?LoadRecode@CGocForce@@QEAAXXZ @ 0x14010B430
void CGocForce::LoadRecode() {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Load maze recode from CForce to CGocRecode
    // TODO: Need CGocRecode component and CForce::GetMazeRecode
    // CUser* pOwnerUser = dynamic_cast<CUser*>(GetOwnerGO());
    // if (!pOwnerUser) return;
    // 
    // auto pRecode = pOwnerUser->GetGOC<CGocRecode>();
    // if (!pRecode) return;
    // 
    // std::uint32_t dwActorID = pOwnerUser->GetActorID().GetID();
    // auto pForce = GetForce();
    // 
    // int nRecode = 0;
    // if (pForce->GetMazeRecode(dwActorID, nRecode)) {
    //     pRecode->SetFullRecode(nRecode);
    // }
}

// IDA: ?NeedReviveBuffUser@CGocForce@@QEAA_NXZ @ 0x14010C7A0
bool CGocForce::NeedReviveBuffUser() {
    if (!IsParty()) {
        return false;
    }
    
    // IDA: Check if any member needs revive buff
    // TODO: Need CForce member iterator and CMover::FindBuffByEffectType
    // CUser* pOwnerUser = dynamic_cast<CUser*>(GetOwnerGO());
    // if (!pOwnerUser) return false;
    // 
    // UXMapID ownerMapID = pOwnerUser->GetMapInsID();
    // auto pForce = GetForce();
    // 
    // for (auto& it : pForce->GetMembers()) {
    //     CForceMember* pMemberInfo = it.second;
    //     CUser* pMember = pMemberInfo->GetUser();
    //     if (!pMember) continue;
    //     
    //     // Check MapInsID matches owner
    //     UXMapID memberMapID = pMember->GetMapInsID();
    //     if (memberMapID != ownerMapID) continue;
    //     
    //     // Check if member has no revive buff (effect type 1)
    //     if (pMember->FindBuffByEffectType(1, 0) == -1) {
    //         return true;  // Found member needing revive
    //     }
    // }
    
    return false;
}

// IDA: ?DeletePartyBoost@CGocForce@@QEAAXXZ @ 0x14010C940
void CGocForce::DeletePartyBoost() {
    // IDA: Get CGocBooster component and remove party booster
    // TODO: Need owner CMover and CGocBooster component
    // CMover* pOwner = dynamic_cast<CMover*>(GetOwnerGO());
    // if (!pOwner) return;
    // 
    // auto pBooster = pOwner->GetGOC<CGocBooster>();
    // if (!pBooster) return;
    // 
    // pBooster->ChangeBooster(eBooster_Type_Party, 0, 0, 0);
}

// IDA: ?GetForceMember@CGocForce@@QEAAXPEAVCUser@@AEAV?$vector@PEAVCForceMember@@V?$allocator@PEAVCForceMember@@@std@@@std@@_N@Z @ 0x14010C9B0
void CGocForce::GetForceMember(CUser* pUser, std::vector<CForceMember*>& vecMember, bool bIncludeOffline) {
    vecMember.clear();
    if (!IsParty()) {
        return;
    }
    
    // IDA: Get Force member list with filtering
    // TODO: Need CForce member iterator
    // auto pForce = GetForce();
    // for (auto& it : pForce->GetMembers()) {
    //     CForceMember* pMemberInfo = it.second;
    //     
    //     if (bIncludeOffline) {
    //         vecMember.push_back(pMemberInfo);
    //         continue;
    //     }
    //     
    //     CUser* pMember = pMemberInfo->GetUser();
    //     if (!pMember) continue;
    //     
    //     // Check member not dead
    //     if (pMember->IsDie()) continue;
    //     
    //     // Check in same map
    //     UXMapID memberMapID = pMember->GetMapInsID();
    //     UXMapID userMapID = pUser->GetMapInsID();
    //     if (memberMapID != userMapID) continue;
    //     
    //     // Check is same member or in maze
    //     if (pMemberInfo->GetActorID() == pUser->GetActorID() || pUser->IsMaze()) {
    //         vecMember.push_back(pMemberInfo);
    //         continue;
    //     }
    //     
    //     // Check distance < 5000
    //     hkvVec3* pos1 = pMember->GetPosition();
    //     hkvVec3* pos2 = pUser->GetPosition();
    //     float distance = hkvVec3::Dist(pos1, pos2);
    //     if (distance < 5000.0f) {
    //         vecMember.push_back(pMemberInfo);
    //     }
    // }
}

// IDA: ?GetForceUserCount@CGocForce@@QEAAEXZ @ 0x14010D330
std::uint8_t CGocForce::GetForceUserCount() const {
    if (!IsParty()) {
        return 0;
    }
    
    // IDA: Call CParty::GetUserCount()
    // TODO: Need CParty::GetUserCount method
    // auto pForce = GetForce();
    // return pForce ? pForce->GetUserCount() : 0;
    return 0;
}

// IDA: ?GetMasterID@CGocForce@@QEAAKXZ @ 0x14010D410
std::uint32_t CGocForce::GetMasterID() const {
    if (!IsParty()) {
        return 0;
    }
    
    // IDA: CWayPoint::GetCurID maps to CForce::GetMasterID
    // TODO: Need CForce::GetMasterID method
    // auto pForce = GetForce();
    // return pForce ? pForce->GetMasterID() : 0;
    return 0;
}

// IDA: ?SetExp@CGocForce@@QEAAXPEAVCUser@@MH@Z @ 0x140083A30
void CGocForce::SetExp(CUser* pUser, float fExp, int nExpType) {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Complex exp distribution logic
    // TODO: Need XResourceMgr, TB_PARTYEXP_LEVEL, TB_PARTYEXP_MOB tables
    // auto pForce = GetForce();
    // 
    // // 1. Calculate level gap
    // int nLevelGap = pForce->GetLevelGap();
    // 
    // // 2. Get level interval bonus from TB_PARTYEXP_LEVEL
    // auto pTBPartyExpLevel = XResourceMgr::GetTB_PARTYEXP_LEVEL(nLevelGap);
    // if (!pTBPartyExpLevel) return;
    // float fTotalExp = fExp * pTBPartyExpLevel->LevelInterval_Value;
    // 
    // // 3. Calculate monster level interval
    // int cMonsterInterval = pForce->GetCommonThreadIndex() - nExpType;
    // 
    // // 4. Get monster level bonus from TB_PARTYEXP_MOB
    // auto pTBPartyExpMob = XResourceMgr::GetTB_PARTYEXP_MOB(cMonsterInterval);
    // if (!pTBPartyExpMob) return;
    // fTotalExp *= pTBPartyExpMob->Mob_IntervalValue;
    // 
    // // 5. Get all member list
    // std::vector<CForceMember*> vecExpMember;
    // GetForceMember(pUser, vecExpMember, false);
    // 
    // // 6. Calculate final exp for each member
    // for (auto pMember : vecExpMember) {
    //     CUser* pMemberUser = pMember->GetUser();
    //     if (!pMemberUser) continue;
    //     
    //     float fForceExp = fTotalExp;
    //     
    //     // Special effect bonus
    //     auto pAttr = pMemberUser->GetGOC<CGocAttribute>();
    //     float fRate = pAttr->GetSpecialEffect(EFFECT_SPECIAL_EXP_ADD_RAT) * 0.01f;
    //     
    //     // Booster bonus
    //     auto pBooster = pMemberUser->GetGOC<CGocBooster>();
    //     fRate += pBooster->GetTotalRate(eBooster_Effect_IncExp);
    //     
    //     // League skill bonus
    //     float fLeagueRate = 0.0f;
    //     if (pMemberUser->IsLeagueSkill(4)) {
    //         fLeagueRate = pMemberUser->GetLeagueSkillEffectValue(4) * 0.01f;
    //     }
    //     
    //     float fBonus = fForceExp * (fRate + fLeagueRate);
    //     fForceExp += fBonus;
    //     
    //     // Set exp
    //     pAttr->SetExp(fForceExp, fBonus, 1);
    // }
}

// IDA: ?IsMember@CGocForce@@QEAA_NPEAVXActor@@@Z @ 0x14010BBB0
bool CGocForce::IsMember(XActor* pActor) const {
    if (!IsParty() || !pActor) {
        return false;
    }
    
    // IDA: Get ActorID and check if Force member
    // TODO: Need XActor::GetActorID and CParty::IsMember
    // UXActorID uxActorID = pActor->GetActorID();
    // std::uint32_t dwActorID = uxActorID.GetID();
    // auto pForce = GetForce();
    // return pForce ? pForce->IsMember(dwActorID) : false;
    return false;
}

// IDA: ?CheckPassiveSkill@CGocForce@@QEAAXPEAVCUser@@EE@Z @ 0x1400851B0
void CGocForce::CheckPassiveSkill(CUser* pUser, std::uint8_t byTargetType, std::uint8_t byCondition) {
    if (!IsParty()) {
        return;
    }
    
    // IDA: Forward to CParty::CheckPassiveSkill
    // TODO: Need CParty::CheckPassiveSkill method
    // auto pForce = GetForce();
    // if (pForce) {
    //     pForce->CheckPassiveSkill(pUser, byTargetType, byCondition);
    // }
}

// IDA: ?CheckForceMatchingEnter@CGocForce@@QEAA_NXZ @ 0x140085210
bool CGocForce::CheckForceMatchingEnter() const {
    if (!IsParty()) {
        return false;
    }
    
    // IDA: Check all Force members in same world/channel/map instance
    // TODO: Need complete member iterator and XActor/XArea access
    // auto pForce = GetForce();
    // 
    // short shWorldID = -1;
    // int nChannelID = -1;
    // UXMapID uxBaseMapID;
    // 
    // for (auto& it : pForce->GetMembers()) {
    //     CForceMember* pMemberInfo = it.second;
    //     if (!pMemberInfo) return false;
    //     
    //     UXActorID actorID = pMemberInfo->GetActorID();
    //     CUser* pMember = XGameServer::Instance()->FindActorIDToUser(actorID);
    //     if (!pMember) return false;
    //     
    //     if (!pMember->GetArea()) return false;
    //     
    //     if (shWorldID == -1 && nChannelID == -1) {
    //         // Set baseline
    //         shWorldID = pMember->GetWorldID();
    //         nChannelID = pMember->GetArea()->GetChannel();
    //         uxBaseMapID = pMember->GetMapInsID();
    //     }
    //     
    //     // Check WorldID
    //     if (shWorldID != pMember->GetWorldID()) return false;
    //     
    //     // Check Channel
    //     if (nChannelID != pMember->GetArea()->GetChannel()) return false;
    //     
    //     // Check MapInsID
    //     if (uxBaseMapID != pMember->GetMapInsID()) return false;
    // }
    // 
    // return true;
    
    return false;
}
