// Animation.cpp - Animation and Motion System Implementation
// Recovered from IDA GameServer.exe port 10004
// 
// This file implements the animation/motion system for GameServer.
// Functions are decompiled from IDA and preserve original logic structure.

#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/Ai.h"
#include "Soulworker/GameServer/XGameServer/ActionResMgr.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// ============================================================================
// CMover Animation Functions
// ============================================================================

// IDA 0x1402AB570 - CMover::ChangeMotion
// Changes the current motion/animation state
void CMover::ChangeMotion(std::int16_t nMotionClass) {
    // IDA: Simple wrapper function
    // Sets the motion class which determines animation type
    
    m_nMotionClass = nMotionClass;
    
    // TODO: IDA shows this may trigger animation callback
    // Need to check if animation event should be triggered
    // if (m_pActionResource && m_pCurMotionEvent) {
    //     // Trigger motion change callback
    // }
    
    // Mark animation as changed
    m_bAnimChanged = 1;
}

// IDA 0x1400154D0 - CMover::SetAnimInfoKey
// Sets animation information using key (integer-based lookup)
void CMover::SetAnimInfoKey(std::uint32_t dwAnimKey) {
    // IDA: Sets animation using key-based lookup
    // Used for fast animation switching
    
    if (m_mapAnimInfoKey == nullptr) {
        // No animation info map available
        return;
    }
    
    // TODO: IDA logic
    // 1. Look up animation key in m_mapAnimInfoKey
    // 2. Get animation index from map
    // 3. Set m_nAnimationIdx
    
    // auto it = m_mapAnimInfoKey->find(dwAnimKey);
    // if (it != m_mapAnimInfoKey->end()) {
    //     m_nAnimationIdx = it->second;
    //     m_bAnimChanged = 1;
    // }
}

// IDA 0x1400154B0 - CMover::SetAnimInfoString
// Sets animation information using string name
void CMover::SetAnimInfoString(const char* szAnimName) {
    // IDA: Sets animation using string-based lookup
    
    if (m_mapAnimInfoString == nullptr || szAnimName == nullptr) {
        return;
    }
    
    // TODO: IDA logic
    // 1. Look up animation name in m_mapAnimInfoString
    // 2. Get animation index from map
    // 3. Set m_nAnimationIdx
    
    // VString strAnimName(szAnimName);
    // auto it = m_mapAnimInfoString->find(strAnimName);
    // if (it != m_mapAnimInfoString->end()) {
    //     m_nAnimationIdx = it->second;
    //     m_bAnimChanged = 1;
    // }
}

// IDA 0x140198E30 - CMover::GetCurMotionEvent
// Gets the current motion event (animation event data)
const VAnimationInfo* CMover::GetCurMotionEvent() const {
    // IDA: Simple getter - returns current motion event pointer
    return m_pCurMotionEvent;
}

// IDA 0x140275370 - CMover::GetAnimationIdx
// Gets the current animation index
std::int32_t CMover::GetAnimationIdx() const {
    // IDA: Simple getter
    return m_nAnimationIdx;
}

// IDA 0x140275270 - CMover::GetMotionClass
// Gets the current motion class
std::int16_t CMover::GetMotionClass() const {
    // IDA: Simple getter
    return m_nMotionClass;
}

// IDA 0x140367CC0 - CMover::SetAnimSpeed
// Sets animation playback speed
void CMover::SetAnimSpeed(float fSpeed) {
    // IDA: Sets animation speed multiplier
    // Used for slow-motion or speed-up effects
    
    m_fAnimSpeed = fSpeed;
    
    // TODO: IDA shows this may need to notify Vision Engine
    // Vision Engine may need to update animation playback rate
    // if (m_pActionResource) {
    //     // Update animation speed in Vision Engine
    // }
}

// IDA 0x1402C6310 - CMover::GetDmgMotionFlag
// Gets damage motion flag
std::int8_t CMover::GetDmgMotionFlag() const {
    // IDA: Simple getter for damage motion flag
    // Used to determine if damage animation should play
    return m_byDmgMontionFlag;
}

// IDA 0x1401AB740 - CMoverEx::CheckMovingAttackAnimation
// Checks if moving attack animation is playing
bool CMoverEx::CheckMovingAttackAnimation() const {
    // IDA: Checks if the mover is in a moving attack animation state
    // Returns true if currently performing moving attack
    
    // TODO: IDA logic
    // Check motion class and animation state
    // return (m_nMotionClass == MOTION_ATTACK_MOVING);
    
    return false; // Stub
}

// ============================================================================
// CMoverEx Animation Functions
// ============================================================================

// IDA 0x140275960 - CMoverEx::SetIdleMotionInfo
// Sets idle motion information
void CMoverEx::SetIdleMotionInfo(std::int16_t nMotionClass) {
    // IDA: Sets idle motion info for the mover
    // Used when entity returns to idle state
    
    // Call base ChangeMotion
    this->ChangeMotion(nMotionClass);
    
    // Additional idle-specific logic
    m_bAnimPlay = 1;
    m_fAnimSpeed = 1.0f; // Reset to normal speed for idle
    
    // TODO: IDA shows more logic
    // - Reset animation state
    // - Clear combat flags
    // - Notify AI system
}

// IDA 0x14039D900 - CMoverEx::GetRestoreAnimSpeed
// Gets the animation speed to restore to after special effects
float CMoverEx::GetRestoreAnimSpeed() const {
    // IDA: Returns the saved animation speed for restoration
    // Used after slow-motion or speed-up effects end
    
    return m_fRestoreAnimSpeed;
}

// IDA 0x140398DD0 - CMoverEx::PlayChangeMonsterMotion
// Plays monster transformation/change animation
void CMoverEx::PlayChangeMonsterMotion(std::int16_t nMotionClass, float fBlendTime) {
    // IDA: Plays monster transformation animation
    // Used when monster transforms or changes state
    
    // Save current animation speed
    m_fRestoreAnimSpeed = m_fAnimSpeed;
    
    // Set blend time for smooth transition
    // TODO: IDA shows this uses Vision Engine blend system
    // m_fBlendTime = fBlendTime;
    
    // Change motion
    this->ChangeMotion(nMotionClass);
    
    // TODO: IDA shows network broadcast
    // Broadcast motion change to nearby players
    // SendMotionChangePacket();
}

// IDA 0x140395780 - CMoverEx::PlaySuboAnim
// Plays sub-object animation (for multi-part entities)
void CMoverEx::PlaySuboAnim(std::int16_t nMotionClass, const char* szAnimName, bool bLoop) {
    // IDA: Plays animation for sub-objects
    // Used for complex entities with multiple animated parts
    
    // TODO: IDA logic
    // 1. Find sub-object by name
    // 2. Set animation on sub-object
    // 3. Configure loop settings
    
    // Stub implementation
    this->ChangeMotion(nMotionClass);
}

// IDA 0x140397330 - CMoverEx::PlayCounterAnim
// Plays counter-attack animation
void CMoverEx::PlayCounterAnim(std::int16_t nMotionClass) {
    // IDA: Plays counter-attack animation
    // Used when entity performs counter attack
    
    // Set counter motion
    this->ChangeMotion(nMotionClass);
    
    // TODO: IDA shows additional logic
    // - Set counter attack flags
    // - Configure hit detection
    // - Set animation speed for counter
    m_fAnimSpeed = 1.5f; // Counter animations often faster
}

// ============================================================================
// XActionResMgr Animation Functions
// ============================================================================

// IDA 0x14000C360 - XActionResMgr::SetAnimInfoToActor
// Sets animation information to an actor
bool XActionResMgr::SetAnimInfoToActor(std::uint32_t dwTableID, CMover* pMover) {
    // IDA: Sets animation information from action resource to actor
    // This is a key function that loads and applies animation data
    
    if (pMover == nullptr) {
        return false;
    }
    
    // Get action resource for the table ID
    VActionResourceLump* pActionRes = this->GetAction(dwTableID);
    if (pActionRes == nullptr) {
        // Try to load it
        pActionRes = this->LoadAction(dwTableID);
        if (pActionRes == nullptr) {
            return false;
        }
    }
    
    // TODO: IDA logic
    // 1. Get animation info from action resource
    // 2. Apply to actor (CMover)
    // 3. Set animation maps (key and string versions)
    // 4. Configure animation events
    
    // Stub implementation
    pMover->m_pActionResource = pActionRes;
    
    return true;
}

// IDA 0x140367A30 - XActionResMgr::GetAnimIndex
// Gets animation index by motion class and sub-class
std::int32_t XActionResMgr::GetAnimIndex(std::int16_t nMotionClass, std::int16_t nSubClass, bool bBattlePose) {
    // IDA: Calculates animation index from motion parameters
    // Uses formula: (MotionClass * MaxSubClass) + SubClass
    
    // TODO: IDA shows this uses lookup tables
    // For now, use simple calculation
    
    const std::int32_t MAX_SUBCLASS = 100;
    std::int32_t nIndex = (nMotionClass * MAX_SUBCLASS) + nSubClass;
    
    // Adjust for battle pose
    if (bBattlePose) {
        nIndex += 10000; // Offset for battle animations
    }
    
    return nIndex;
}

// IDA 0x14000B250 - XActionResMgr::RegisterAnimInfo
// Registers animation information into the system
bool XActionResMgr::RegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, 
                                      const VString& strAnimName, bool bBattlePose) {
    // IDA: Registers animation info for later lookup
    // This is called during resource loading
    
    // Calculate animation index
    std::int32_t nAnimIndex = this->GetAnimIndex(nMotionClass, nSubClass, bBattlePose);
    
    // Add to string map (AnimIndex -> AnimName)
    if (m_mapAnimInfoString.find(m_dwTableID) == m_mapAnimInfoString.end()) {
        m_mapAnimInfoString[m_dwTableID] = new std::map<unsigned long, VString>();
    }
    (*m_mapAnimInfoString[m_dwTableID])[nAnimIndex] = strAnimName;
    
    // Add to key map (AnimName -> AnimIndex)
    if (m_mapAnimInfoKey.find(m_dwTableID) == m_mapAnimInfoKey.end()) {
        m_mapAnimInfoKey[m_dwTableID] = new std::map<VString, unsigned long>();
    }
    (*m_mapAnimInfoKey[m_dwTableID])[strAnimName] = nAnimIndex;
    
    // TODO: IDA shows additional logic
    // - Parse animation events from action resource
    // - Register triggers and callbacks
    // - Set up hit collision data
    
    return true;
}

// IDA 0x140009230 - XActionResMgr::ChangeMotionCallback
// Callback for motion changes
void XActionResMgr::ChangeMotionCallback(CMover* pMover, const VAnimationInfo* pInfo) {
    // IDA: Called when a motion changes
    // Handles animation events and triggers
    
    if (pMover == nullptr || pInfo == nullptr) {
        return;
    }
    
    // TODO: IDA logic
    // 1. Get animation events from info
    // 2. Process triggers (damage frames, sound effects, etc.)
    // 3. Apply hit collision data
    // 4. Notify combat system
    
    // Stub implementation
    pMover->m_pCurMotionEvent = pInfo;
}

// ============================================================================
// CUser Animation Functions
// ============================================================================

// IDA 0x1405291F0 - CUser::SetPlayLoopMotion
// Sets whether to play loop motion
void CUser::SetPlayLoopMotion(bool bIsPlayLoopMotion) {
    // IDA: Sets loop motion flag for player
    // Used for continuous animations (running, swimming, etc.)
    
    // TODO: IDA shows this sets a member variable
    // m_bPlayLoopMotion = bIsPlayLoopMotion;
    
    // Additional player-specific logic
    // - Update animation state
    // - Notify client
}

// IDA 0x1406F0B30 - CUser::ChangeMotion (version 1)
// Changes player motion state
void CUser::ChangeMotion(std::int16_t nMotionClass) {
    // IDA: Changes player animation state
    // Player-specific motion changes with network sync
    
    // Call base class
    CMover::ChangeMotion(nMotionClass);
    
    // TODO: IDA shows player-specific logic
    // 1. Broadcast motion change to nearby players
    // 2. Update player state machine
    // 3. Cancel current skills if needed
    // 4. Update movement speed based on motion
}

// IDA 0x1406F0AA0 - CUser::ChangeMotion (version 2)
// Changes player motion with additional parameters
void CUser::ChangeMotion(std::int16_t nMotionClass, std::int16_t nSubClass, bool bForce) {
    // IDA: Extended version with sub-class and force flag
    
    if (bForce) {
        // Force change - cancel current animation immediately
        m_bAnimChanged = 1;
        m_nAnimationIdx = -1;
    }
    
    // Calculate animation index
    // std::int32_t nAnimIdx = XActionResMgr::GetAnimIndex(nMotionClass, nSubClass, true);
    
    // Call simpler version
    this->ChangeMotion(nMotionClass);
}

// IDA 0x1406F2200 - CUser::GetNextMotion
// Gets the next motion in a sequence
std::int16_t CUser::GetNextMotion(std::int16_t nCurrentMotion) {
    // IDA: Gets next motion in animation sequence
    // Used for combo systems
    
    // TODO: IDA logic
    // Look up next motion in combo table
    // For now, return current + 1
    
    return nCurrentMotion + 1;
}

// ============================================================================
// CMonster Animation Functions
// ============================================================================

// IDA 0x14035C2C0 - CMonster::CheckSuperArmorMotion
// Checks if monster should use super armor motion
bool CMonster::CheckSuperArmorMotion() const {
    // IDA: Checks super armor state for animation
    // Super armor = monster doesn't flinch from damage
    
    // TODO: IDA logic
    // Check super armor gage and flags
    // return (m_fCurSuperArmorGage > 0.0f);
    
    return false; // Stub
}

// IDA 0x140356AB0 - CMonster::GetReservedMotion
// Gets reserved motion for monster
std::int16_t CMonster::GetReservedMotion() const {
    // IDA: Gets reserved motion (planned animation)
    // Used for AI-driven animation sequences
    
    // TODO: IDA shows this returns a reserved motion value
    // return m_nReservedMotion;
    
    return 0; // Stub
}

// IDA 0x140356A60 - CMonster::SetReservedMotion
// Sets reserved motion for monster
void CMonster::SetReservedMotion(std::int16_t nMotion) {
    // IDA: Sets reserved motion for later playback
    
    // TODO: IDA shows this sets a member variable
    // m_nReservedMotion = nMotion;
}

// IDA 0x14035E380 - CMonster::GetDeathMotion
// Gets death animation motion class
std::int16_t CMonster::GetDeathMotion() const {
    // IDA: Gets appropriate death animation based on damage type
    
    // TODO: IDA logic
    // - Check last damage type
    // - Return appropriate death motion
    // - Random variation for different death animations
    
    return 1; // Default death motion
}

// IDA 0x14035C430 - CMonster::ChangeMotion (version 1)
// Changes monster motion (simple version)
void CMonster::ChangeMotion(std::int16_t nMotionClass) {
    // IDA: Simple monster motion change
    
    // Call base class
    CMover::ChangeMotion(nMotionClass);
    
    // Monster-specific: notify AI
    // if (m_pAI) {
    //     m_pAI->OnMotionChanged(nMotionClass);
    // }
}

// IDA 0x14035C350 - CMonster::ChangeMotion (version 2)
// Changes monster motion with additional parameters
void CMonster::ChangeMotion(std::int16_t nMotionClass, bool bForce, float fBlendTime) {
    // IDA: Extended version with force and blend time
    
    if (bForce) {
        // Force immediate change
        m_bAnimChanged = 1;
    }
    
    // Set blend time for smooth transition
    // m_fBlendTime = fBlendTime;
    
    // Call simpler version
    this->ChangeMotion(nMotionClass);
}

// ============================================================================
// CAi Animation Functions
// ============================================================================

// IDA 0x1402603C0 - CAi::SetIdleMotionInfo
// Sets idle motion info for AI-controlled entity
void CAi::SetIdleMotionInfo(std::int16_t nMotionClass) {
    // IDA: Sets idle motion for AI entity
    // Called when AI returns to idle state
    
    // Get the monster this AI controls
    CMonster* pMonster = this->GetMonster();
    if (pMonster) {
        pMonster->SetIdleMotionInfo(nMotionClass);
    }
}

// IDA 0x140260C30 - CAi::GetDeathActionMotion
// Gets death action motion for AI entity
std::int16_t CAi::GetDeathActionMotion() {
    // IDA: Determines appropriate death animation
    // Based on how the monster died
    
    std::int16_t nMotionClass = 1; // Default death
    std::uint32_t dwAnimID = 0;
    
    // TODO: IDA logic
    // - Check death cause
    // - Check for special death animations
    // - Random variation
    
    // Get the monster
    CMonster* pMonster = this->GetMonster();
    if (pMonster) {
        nMotionClass = pMonster->GetDeathMotion();
    }
    
    return nMotionClass;
}

// ============================================================================
// CAkashicObject Animation Functions
// ============================================================================

// IDA 0x14019A540 - CAkashicObject::ClearMotion
// Clears motion state for Akashic object
void CAkashicObject::ClearMotion() {
    // IDA: Clears motion state
    // Used when Akashic object is deactivated
    
    // TODO: IDA logic
    // - Reset motion class
    // - Clear animation state
    // - Stop current animation
    
    // Stub - needs CMoverEx base class access
    // CMoverEx::ClearMotion();
}

// ============================================================================
// Additional Animation Helper Functions
// ============================================================================

// Helper: Blend between two animations
void BlendAnimations(CMover* pMover, std::int16_t nFromMotion, std::int16_t nToMotion, float fBlendDuration) {
    // Helper function for smooth animation blending
    // Not directly found in IDA, but logic exists in multiple places
    
    if (pMover == nullptr) {
        return;
    }
    
    // Save blend parameters
    // pMover->m_fBlendTime = fBlendDuration;
    // pMover->m_nFromMotion = nFromMotion;
    
    // Start new motion
    pMover->ChangeMotion(nToMotion);
}

// Helper: Get animation state
std::int32_t GetAnimationState(CMover* pMover) {
    // Helper to query animation state
    // Returns combined state information
    
    if (pMover == nullptr) {
        return -1;
    }
    
    std::int32_t nState = 0;
    
    // Pack motion class and animation index
    nState = (pMover->GetMotionClass() << 16) | (pMover->GetAnimationIdx() & 0xFFFF);
    
    return nState;
}

// Helper: Stop animation
void StopAnimation(CMover* pMover) {
    // Helper to stop current animation
    // Sets entity to frozen state
    
    if (pMover == nullptr) {
        return;
    }
    
    // Pause animation
    pMover->m_bAnimPlay = 0;
    pMover->m_fAnimSpeed = 0.0f;
}

// Helper: Resume animation
void ResumeAnimation(CMover* pMover) {
    // Helper to resume paused animation
    
    if (pMover == nullptr) {
        return;
    }
    
    // Resume animation
    pMover->m_bAnimPlay = 1;
    pMover->m_fAnimSpeed = pMover->m_fRestoreAnimSpeed;
}

// Helper: Query if animation is playing
bool IsAnimationPlaying(CMover* pMover) {
    // Helper to check if animation is active
    
    if (pMover == nullptr) {
        return false;
    }
    
    return (pMover->m_bAnimPlay != 0 && pMover->m_fAnimSpeed > 0.0f);
}
