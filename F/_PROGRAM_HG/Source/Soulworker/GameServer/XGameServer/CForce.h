// CForce.h
// CForce and CForceMember classes for GameServer
// 对齐 IDA GameServer.exe
// 
// Force system: A party-like group system supporting up to 8 members
// Used for: Party content, maze runs, group battles
// Inherits from: CParty

#pragma once

#include "CParty.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <map>
#include <vector>
#include <memory>

// Forward declarations
class CUser;
class CMover;
class XSendPacket;
class XArea;

// ============================================================================
// CForceMember - Force member class (inherits from CPartyMember)
// IDA size: 0x90+ bytes (extends CPartyMember)
//
// Purpose: Manages individual force member state and information
// Extends CPartyMember with force-specific data
// ============================================================================
class CForceMember : public CPartyMember {
public:
    // === Constructor ===
    CForceMember();
    
    // === Destructor ===
    virtual ~CForceMember();

    // === Force-specific Setters ===
    
    // SetMapID - Set map ID
    // IDA: ?SetMapID@CForceMember@@QEAAXH@Z
    void SetMapID(int nMapID);
    
    // SetMaxHP - Set max HP
    // IDA: ?SetMaxHP@CForceMember@@QEAAXH@Z
    void SetMaxHP(int nMaxHP);
    
    // SetAwaken - Set awaken status
    // IDA: ?SetAwaken@CForceMember@@QEAAXE@Z (via CRespawnManager::SetPause)
    void SetAwaken(std::uint8_t byAwaken);

protected:
    // Force-specific member data extends CPartyMember
    // Additional fields TBD based on IDA analysis
};

// ============================================================================
// CForce - Force/Group management class (inherits from CParty)
// IDA size: TBD bytes (extends CParty)
//
// Purpose: Manages force (group) state, membership, and communication
// Force is a specialized party for up to 8 members
// Key features:
// - Member HP/MaxHP/Level/MapID synchronization
// - Local map instance broadcasting
// - Force contribution tracking
// ============================================================================
class CForce : public CParty {
public:
    // === Constructor ===
    // IDA: ??0CForce@@QEAA@XZ
    CForce();
    
    // === Destructor ===
    // IDA: ??1CForce@@UEAA@XZ
    virtual ~CForce();

    // === Member Management ===

    // SetMemberMapID - Set member's current map location
    // IDA: ?SetMemberMapID@CForce@@QEAAXKHHTUXMapID@@@Z @ 0x1401B7E80
    void SetMemberMapID(std::uint32_t dwActorID, int nMapID, int nChannel, const UXMapID& uxMapID);

    // SetMemberHP - Set member's current HP
    // IDA: ?SetMemberHP@CForce@@QEAAXKTUXMapID@@H@Z @ 0x1401B7F50
    void SetMemberHP(std::uint32_t dwActorID, const UXMapID& uxMapID, int nHP);

    // SetMemberMaxHP - Set member's max HP
    // IDA: ?SetMemberMaxHP@CForce@@QEAAXKTUXMapID@@H@Z @ 0x1401B80B0
    void SetMemberMaxHP(std::uint32_t dwActorID, const UXMapID& uxMapID, int nMaxHP);

    // SetMemberLevel - Set member's level
    // IDA: ?SetMemberLevel@CForce@@QEAAXKH@Z @ 0x1401B8210
    void SetMemberLevel(std::uint32_t dwActorID, int nLevel);

    // SetMemberAwaken - Set member's awaken status
    // IDA: ?SetMemberAwaken@CForce@@QEAAXKE@Z @ 0x1401B82B0
    void SetMemberAwaken(std::uint32_t dwActorID, std::uint8_t byAwaken);

    // SetMemberProfilePhoto - Set member's profile photo
    // IDA: ?SetMemberProfilePhoto@CForce@@QEAAXKK@Z @ 0x1401B8350
    void SetMemberProfilePhoto(std::uint32_t dwActorID, std::uint32_t dwPhotoID);

    // === Force Communication ===

    // SendUpdateMemberInfo - Send member info update to all members
    // IDA: ?SendUpdateMemberInfo@CForce@@QEAAXK@Z
    void SendUpdateMemberInfo(std::uint32_t dwActorID);

    // SendToLocal - Send packet to members in same map instance
    // IDA: ?SendToLocal@CParty@@QEAAXAEAVXSendPacket@@TUXMapID@@K@Z (inherited)
    void SendToLocal(XSendPacket& packet, const UXMapID& uxMapID, std::uint32_t dwExcludeActorID);

    // === Force Type ===

    // GetForceType - Get force type (0=normal, etc.)
    // IDA: ?GetForceType@CForce@@QEAAEXZ
    std::uint8_t GetForceType() const;

    // === Force Contribution ===

    // UpdateForceBooster - Update force-wide booster effects
    // IDA: ?UpdateForceBooster@CForce@@QEAAXH@Z
    void UpdateForceBooster(int nType);

    // ApplyReward - Apply rewards to force members
    // IDA: ?ApplyReward@CForce@@QEAAX... (dtor reference)
    void ApplyReward(/* parameters TBD */);

    // === Member Access ===

    // GetMember - Get force member by ActorID
    // IDA: inherited from CParty::GetMember
    CForceMember* GetMember(std::uint32_t dwActorID);

    // GetMembers - Get all force members map
    const std::map<std::uint32_t, CForceMember*>& GetMembers() const { return m_mapForceMember; }

protected:
    // === IDA confirmed member variables ===
    // Inherited from CParty:
    // - m_dwPartyID
    // - m_dwMasterID
    // - m_mapPartyMember (used as m_mapForceMember)
    
    // Force member map (using inherited m_mapPartyMember)
    std::map<std::uint32_t, CForceMember*> m_mapForceMember;
    
    // Force-specific data
    std::uint8_t m_byForceType = 0;
};
