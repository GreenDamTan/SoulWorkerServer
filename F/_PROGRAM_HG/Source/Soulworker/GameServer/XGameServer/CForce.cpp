// CForce.cpp
// CForce and CForceMember implementation
// 对齐 IDA GameServer.exe

#include "CForce.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"

// Forward declarations
class CUser;
class XSendPacket;

// ============================================================================
// CForceMember Implementation
// ============================================================================

CForceMember::CForceMember()
    : CPartyMember() {
}

CForceMember::~CForceMember() {
}

// IDA: ?SetMapID@CForceMember@@QEAAXH@Z
void CForceMember::SetMapID(int nMapID) {
    // IDA: Set map ID in member info
    // m_partyMemberInfo.uxMapID related
    // TODO: Implement when ST_PARTY_MEMBER fields are available
}

// IDA: ?SetMaxHP@CForceMember@@QEAAXH@Z
void CForceMember::SetMaxHP(int nMaxHP) {
    // IDA: Set max HP in member info
    // TODO: Implement when ST_PARTY_MEMBER fields are available
}

// IDA: ?SetAwaken@CForceMember@@QEAAXE@Z (via CRespawnManager::SetPause)
void CForceMember::SetAwaken(std::uint8_t byAwaken) {
    // IDA: Maps to CRespawnManager::SetPause - awaken/pause state
    // TODO: Implement when awaken system is available
}

// ============================================================================
// CForce Implementation
// ============================================================================

CForce::CForce()
    : CParty()
    , m_byForceType(0) {
}

CForce::~CForce() {
}

// IDA: ?SetMemberMapID@CForce@@QEAAXKHHTUXMapID@@@Z @ 0x1401B7E80
// void __fastcall CForce::SetMemberMapID(CForce *this, unsigned int dwActorID, int nMapID, int nChannel, UXMapID uxMapID)
// {
//   // Find member in map
//   auto iter = m_mapForceMember.find(dwActorID);
//   if (iter != m_mapForceMember.end()) {
//     CForceMember* pMember = iter->second;
//     if (pMember) {
//       CPartyMember::Login(pMember);        // Mark as online
//       CForceMember::SetMapID(pMember, nMapID);
//       CPartyMember::SetChannel(pMember, nChannel);
//       UXMapID::UXMapID(&pMember->uxMapID, uxMapID);
//       CForce::SendUpdateMemberInfo(this, dwActorID);
//     }
//   }
// }
void CForce::SetMemberMapID(std::uint32_t dwActorID, int nMapID, int nChannel, const UXMapID& uxMapID) {
    // IDA verified: Find member, update map ID, channel, map instance, then broadcast
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Call Login to mark as online
        pMember->Login();
        
        // IDA: Set map ID
        pMember->SetMapID(nMapID);
        
        // IDA: Set channel
        pMember->SetChannel(nChannel);
        
        // IDA: Set UXMapID (map instance ID)
        // TODO: pMember->SetMapInsID(uxMapID);
        
        // IDA: Broadcast update to all force members
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?SetMemberHP@CForce@@QEAAXKTUXMapID@@H@Z @ 0x1401B7F50
// IDA verified: Set member HP and broadcast PS_FORCE_MEMEBER_HP to local map
void CForce::SetMemberHP(std::uint32_t dwActorID, const UXMapID& uxMapID, int nHP) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Set HP using GameModeBase::SetModeState
        // TODO: pMember->SetHP(nHP);
        
        // IDA: Construct PS_FORCE_MEMEBER_HP packet
        // stMemeberHP.dwMemberID = dwActorID;
        // stMemeberHP.nMaxHP = pMember->GetMaxHP();
        // stMemeberHP.nHP = nHP;
        // XSendPacket(0x2E, 0x12) << stMemeberHP;
        // CParty::SendToLocal(this, &xSendPacket, uxMapID, dwActorID);
        
        // TODO: Implement packet sending when XSendPacket is available
    }
}

// IDA: ?SetMemberMaxHP@CForce@@QEAAXKTUXMapID@@H@Z @ 0x1401B80B0
// IDA verified: Set member max HP and broadcast PS_FORCE_MEMEBER_HP to local map
void CForce::SetMemberMaxHP(std::uint32_t dwActorID, const UXMapID& uxMapID, int nMaxHP) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Set max HP using CForceMember::SetMaxHP
        pMember->SetMaxHP(nMaxHP);
        
        // IDA: Construct PS_FORCE_MEMEBER_HP packet
        // stMemeberHP.dwMemberID = dwActorID;
        // stMemeberHP.nMaxHP = nMaxHP;
        // stMemeberHP.nHP = pMember->GetHP();
        // XSendPacket(0x2E, 0x12) << stMemeberHP;
        // CParty::SendToLocal(this, &xSendPacket, uxMapID, dwActorID);
        
        // TODO: Implement packet sending when XSendPacket is available
    }
}

// IDA: ?SetMemberLevel@CForce@@QEAAXKH@Z @ 0x1401B8210
// IDA verified: Set member level and broadcast update
void CForce::SetMemberLevel(std::uint32_t dwActorID, int nLevel) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Set level using CPartyMember::SetLevel
        pMember->SetLevel(static_cast<std::uint8_t>(nLevel));
        
        // IDA: Broadcast update to all force members
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?SetMemberAwaken@CForce@@QEAAXKE@Z @ 0x1401B82B0
// IDA verified: Set member awaken status (maps to CRespawnManager::SetPause) and broadcast
void CForce::SetMemberAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Set awaken using CRespawnManager::SetPause
        pMember->SetAwaken(byAwaken);
        
        // IDA: Broadcast update to all force members
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?SetMemberProfilePhoto@CForce@@QEAAXKK@Z @ 0x1401B8350
// IDA verified: Set member profile photo and broadcast update
void CForce::SetMemberProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwPhotoID) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end() && iter->second) {
        CForceMember* pMember = iter->second;
        
        // IDA: Set profile photo using CPartyMember::SetProfilePhoto
        pMember->SetProfilePhoto(dwPhotoID);
        
        // IDA: Broadcast update to all force members
        SendUpdateMemberInfo(dwActorID);
    }
}

// IDA: ?SendUpdateMemberInfo@CForce@@QEAAXK@Z
// Broadcast member info update to all force members
void CForce::SendUpdateMemberInfo(std::uint32_t dwActorID) {
    // IDA: Construct PS_FORCE_MEMBER_INFO packet
    // Send to all force members
    // TODO: Implement when packet structures are available
}

// IDA: ?SendToLocal@CParty@@QEAAXAEAVXSendPacket@@TUXMapID@@K@Z
// Send packet to members in same map instance
void CForce::SendToLocal(XSendPacket& packet, const UXMapID& uxMapID, std::uint32_t dwExcludeActorID) {
    // IDA: Iterate through all force members
    // Check if member's map instance matches uxMapID
    // Send packet to matching members (excluding dwExcludeActorID)
    // TODO: Implement when member iteration and packet sending are available
}

// IDA: ?GetForceType@CForce@@QEAAEXZ
std::uint8_t CForce::GetForceType() const {
    return m_byForceType;
}

// IDA: ?UpdateForceBooster@CForce@@QEAAXH@Z
void CForce::UpdateForceBooster(int nType) {
    // IDA: Update booster effects for all force members
    // nType parameter controls which booster to update
    // TODO: Implement when booster system is available
}

// IDA: ?GetMazeRecode@CForce@@QEAAXKPEAH@Z @ 0x1403A6D30
void CForce::GetMazeRecode(std::uint32_t dwActorID, int* pMazeRecode) {
    // IDA: iter = m_mapPartyMember.find(dwActorID);
    // if (iter != end) CPartyMember::GetRecode((CPartyMember*)iter->second, pMazeRecode);
    auto iter = m_mapPartyMember.find(dwActorID);
    if (iter != m_mapPartyMember.end()) {
        iter->second->GetRecode(pMazeRecode);
    }
}

// IDA: ?ApplyReward@CForce@@QEAAX... (dtor reference)
void CForce::ApplyReward(/* parameters TBD */) {
    // IDA: Apply rewards to force members
    // Called after completing content (maze, raid, etc.)
    // TODO: Implement when reward system is available
}

// IDA: GetMember - Get force member by ActorID
CForceMember* CForce::GetMember(std::uint32_t dwActorID) {
    auto iter = m_mapForceMember.find(dwActorID);
    if (iter != m_mapForceMember.end()) {
        return iter->second;
    }
    return nullptr;
}
