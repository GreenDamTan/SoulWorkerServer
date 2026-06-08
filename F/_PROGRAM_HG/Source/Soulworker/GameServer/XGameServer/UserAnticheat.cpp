// UserAnticheat.cpp - Anti-cheat and validation functions for CUser
// IDA Port: 10004 (GameServer.exe)
// 
// This file contains anti-cheat detection and validation functions
// reconstructed from IDA decompilation.
//
// All functions marked as verified=no until build verification complete.

#include "User.h"
#include "Mover.h"
#include "GameServer.h"
#include "Soulworker/GameServer/XCore/XServer/XPacket.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSCommon.h"
#include "Soulworker/GameServer/XCore/XServer/LogHelper.h"
#include <cmath>
#include <cstdlib>
#include <cstdint>

#ifdef _WIN32
#include <Windows.h>
#else
#include <chrono>
#endif

// Helper function to get tick count in milliseconds
static std::uint64_t GetTickCount64Ms() {
#ifdef _WIN32
    return GetTickCount64();
#else
    auto now = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return static_cast<std::uint64_t>(ms.count());
#endif
}

// ============================================================================
// Speed Hack Detection
// ============================================================================

// IDA 0x1406EBA20 - GetSpeedHackAttackLimitTime
// Returns the minimum time in milliseconds between valid attacks (100ms)
std::uint32_t CUser::GetSpeedHackAttackLimitTime() {
    return 100; // 100ms minimum between attacks
}

// IDA 0x1406EBA30 - CheckSpeedHackAttack
// Detects speed hacks by checking attack timing
// Returns true if no speed hack detected (attack is valid)
bool CUser::CheckSpeedHackAttack() {
    // Check if area exists and is valid
    // TODO: Need proper GetArea() implementation from XActor
    // if (GetArea() && GetArea()->IsSomeCheck()) {
    //     return true;
    // }
    
    std::uint64_t dwTickCount = GetTickCount64Ms();
    std::uint32_t dwDeltaAttackTime = static_cast<std::uint32_t>(dwTickCount - m_dwCheckSpeedHackAttack);
    std::uint32_t dwCheckTime = GetSpeedHackAttackLimitTime();
    
    if (m_dwCheckSpeedHackAttack != 0) {
        // Check per-minute cheat count
        if (m_dwCheckSpeedHackAttackForMin <= dwTickCount) {
            m_dwCheckSpeedHackAttackForMin = dwTickCount + 1000; // 1 second window
            
            if (m_nCheatCountForMin > 5) {
                // Log speed hack detection
                LogHelper::LogInfo("game.contents",
                    "[%d]<SPEED HACK - ATTACK FOR MIN> %d, %d < %d (ms)",
                    0, // TODO: GetActorID().dwActorID
                    m_nCurSkillTableIdx,
                    dwDeltaAttackTime,
                    dwCheckTime);
                
                ++m_dwCheatCount[3];
                
                // TODO: Send log to DB
                // ST_LOG_GAME stLog;
                // stLog._nUAID = GetUAID();
                // stLog._nUCID = GetActorID().dwActorID; // simplified
                // stLog._sMainType = 51;
                // stLog._sSubType = 20;
                // stLog.nParam0 = m_nCurSkillTableIdx;
                // stLog.nParam1 = dwDeltaAttackTime;
                // stLog.nParam2 = dwCheckTime;
                // stLog.nParam3 = GetArea() ? GetArea()->GetTBMapID() : 0;
                // stLog.nParam4 = m_nCheatCountForMin;
                // XGameServer::Instance()->SendDBLog(&stLog);
            }
            m_nCheatCountForMin = 0;
        }
        ++m_nCheatCountForMin;
    }
    
    m_dwCheckSpeedHackAttack = dwTickCount;
    return true;
}

// ============================================================================
// Position Validation
// ============================================================================

// IDA 0x1406EBDE0 - CheckInvalidPos
// Validates player position to detect speed/teleport hacks
// Returns true if position is INVALID (cheat detected)
// Parameters:
//   vPos: New position being validated
//   iCallFuncId: 1=move request, other=skill/warp
//   vTargetPos: Target position (for move validation)
//   fMoveSpeed: Client reported move speed
//   byRunBit: Running flag
bool CUser::CheckInvalidPos(XVec3* vPos, int iCallFuncId, XVec3* vTargetPos,
                           float fMoveSpeed, std::uint8_t byRunBit) {
    // Get current position from position info
    // TODO: m_pPosInfo needs proper initialization
    // XVec3 currentPos = m_pPosInfo->vPos;
    
    // Calculate distance difference (placeholder)
    float fDiffX = 0.0f; // fabsf(currentPos.x - vPos->x);
    float fDiffY = 0.0f; // fabsf(currentPos.y - vPos->y);
    float fDiffZ = 0.0f; // fabsf(currentPos.z - vPos->z);
    
    // Check for large position jumps (>3000 units in X or Y)
    if ((fDiffX + fDiffY) > 3000.0f) {
        // Potential teleport hack detected
        LogHelper::LogInfo("game.contents",
            "[%d]<SPEED HACK - POS> %d (%0.2f %0.2f %0.2f) -> (%0.2f %0.2f %0.2f)",
            0, // TODO: ActorID
            iCallFuncId,
            0.0, 0.0, 0.0, // currentPos
            vPos->x, vPos->y, vPos->z);
        
        ++m_dwCheatCount[1];
        return true; // Invalid position
    }
    
    // Check Z-axis (height) anomaly
    if (fDiffZ > 500.0f) {
        // Height difference too large
        // TODO: Check if world type allows flying
        // TODO: Check ground height with CMover::GetHeight
        
        LogHelper::LogInfo("game.contents",
            "[%d]<SPEED HACK - POS> %d Z Pos anomaly (%0.2f)",
            0, iCallFuncId, vPos->z);
        
        ++m_dwCheatCount[1];
        return true;
    }
    
    // Check for move function (iCallFuncId == 1)
    if (iCallFuncId == 1) {
        // Get max move speed from server
        float fSpeedByServer = 0.0f; // GetMaxMoveSpeed(byRunBit);
        
        // Check if client speed exceeds server limit
        if (fMoveSpeed > fSpeedByServer) {
            ++m_nCheckWrongSpeedCount;
            LogHelper::LogInfo("game.system",
                "[%d]<CHECK POS> Over Speed (%0.2f / %0.2f)",
                0, fMoveSpeed, fSpeedByServer);
            
            if (fMoveSpeed > m_fCheckMaxSpeed) {
                m_fCheckMaxSpeed = fMoveSpeed;
            }
        }
        
        // Check target distance
        if (vTargetPos) {
            // float fDistance = XVec3::distance(vPos, vTargetPos);
            float fDistance = 0.0f; // placeholder
            if (fDistance > (fSpeedByServer + 250.0f)) {
                ++m_nCheckWrongTargetCount;
                LogHelper::LogInfo("game.system",
                    "[%d]<CHECK POS> Wrong Target (%0.2f / %0.2f)",
                    0, fDistance, fSpeedByServer);
            }
        }
        
        // Check position movement distance
        // float fPosDistance = XVec3::distance(&currentPos, vPos);
        float fPosDistance = 0.0f; // placeholder
        if (fPosDistance > (fSpeedByServer + 250.0f)) {
            ++m_nCheckWrongPosCount;
            LogHelper::LogInfo("game.system",
                "[%d]<CHECK POS> Wrong Pos (%0.2f / %0.2f)",
                0, fPosDistance, fSpeedByServer);
        }
    }
    
    return false; // Position is valid
}

// ============================================================================
// Bot Detection
// ============================================================================

// IDA 0x1406EAA00 - CheckValidBot
// Checks if player name contains "Bot" to mark as bot
void CUser::CheckValidBot() {
    std::wstring name = GetName();
    if (name.find(L"Bot") != std::wstring::npos) {
        m_bIsBot = true;
    }
}

// ============================================================================
// Logging Functions
// ============================================================================

// IDA 0x1407017A0 - SendCheckSpeedLog
// Logs speed hack statistics to database
void CUser::SendCheckSpeedLog() {
    // TODO: Implement full logging with ST_LOG_GAME
    // ST_LOG_GAME stLog;
    // stLog._nUAID = GetUAID();
    // stLog._nUCID = GetActorID().dwActorID;
    // stLog._sMainType = 52;
    // stLog._sSubType = 3;
    // stLog.nParam0 = m_nCheckWrongSpeedCount;
    // stLog.nParam1 = m_nCheckWrongTargetCount;
    // stLog.nParam2 = m_nCheckWrongPosCount;
    // stLog.nParam3 = *reinterpret_cast<int*>(&m_fCheckMaxSpeed);
    // stLog.nParam4 = m_dwCheatCount[3];
    // stLog.nParam5 = GetArea() ? GetArea()->GetTBMapID() : 0;
    // stLog.nParam6 = GetArea() ? GetArea()->GetInstanceID() : 0;
    // wcscpy_s(stLog.szComment, L"SPEED HACK");
    // XGameServer::Instance()->SendDBLog(&stLog);
    
    // Reset counters
    m_nCheckWrongSpeedCount = 0;
    m_nCheckWrongTargetCount = 0;
    m_nCheckWrongPosCount = 0;
    m_fCheckMaxSpeed = 0.0f;
    m_dwCheatCount[3] = 0;
}

// ============================================================================
// Kick Functions
// ============================================================================

// IDA 0x140701680 - CheckKickoutNow
// Checks if player should be kicked immediately
bool CUser::CheckKickoutNow() {
    // Check if kickout time has passed
    if (m_dwKickoutTime != 0) {
        return GetTickCount64Ms() >= m_dwKickoutTime;
    }
    return false;
}

// IDA 0x14070AD80 - SetKick_AlreadyLogin
// Marks player for kick due to duplicate login
void CUser::SetKick_AlreadyLogin() {
    m_bKick_AlreadyLogin = true;
}

// IDA 0x14070AFA0 - IsKick_AlreadyLogin
// Checks if player is marked for duplicate login kick
bool CUser::IsKick_AlreadyLogin() {
    return m_bKick_AlreadyLogin;
}

// ============================================================================
// Additional Validation Functions (Placeholder implementations)
// ============================================================================

// ValidatePacket - Validate incoming packet structure and data
// Returns true if packet is valid
bool ValidatePacket(void* pPacket, std::uint16_t wPacketSize) {
    if (pPacket == nullptr || wPacketSize < sizeof(std::uint16_t)) {
        return false;
    }
    
    // TODO: Implement packet validation based on packet type
    // 1. Check minimum packet size
    // 2. Validate packet header
    // 3. Check for buffer overflows
    // 4. Validate packet sequence number
    
    return true;
}

// CheckItemDuplication - Detect item duplication attempts
// Returns true if duplication detected
bool CheckItemDuplication(CUser* pUser, std::uint64_t ullItemSerial, 
                          std::uint32_t dwItemID, int nCount) {
    if (pUser == nullptr) {
        return false;
    }
    
    // TODO: Implement item duplication check
    // 1. Check if item serial already exists in inventory
    // 2. Check for duplicate item serial in trade
    // 3. Verify item count matches expected values
    // 4. Check transaction history for duplicates
    
    return false;
}

// ValidateAction - Validate player action request
// Returns true if action is valid
bool ValidateAction(CUser* pUser, int nActionType, int nActionParam) {
    if (pUser == nullptr) {
        return false;
    }
    
    // TODO: Implement action validation
    // 1. Check if action type is valid
    // 2. Verify action is allowed in current state
    // 3. Check cooldown timers
    // 4. Validate action parameters
    
    return true;
}

// LogSuspiciousActivity - Log potential exploit attempts
void LogSuspiciousActivity(CUser* pUser, int nActivityType, const char* szDescription,
                          int nParam0, int nParam1, int nParam2) {
    if (pUser == nullptr) {
        return;
    }
    
    // TODO: Implement full logging with ST_LOG_GAME
    LogHelper::LogInfo("game.security",
        "[SUSPICIOUS] UAID:%d Type:%d %s (p0=%d, p1=%d, p2=%d)",
        pUser->GetUAID(),
        nActivityType,
        szDescription ? szDescription : "",
        nParam0, nParam1, nParam2);
}

// BanPlayer - Ban player account
// Returns true if ban successful
bool BanPlayer(std::uint32_t dwUAID, int nBanType, int nBanDuration,
               const char* szReason) {
    // TODO: Implement ban logic
    // 1. Add to ban list
    // 2. Update database
    // 3. Kick player if online
    // 4. Log ban action
    
    LogHelper::LogInfo("game.security",
        "[BAN] UAID:%d Type:%d Duration:%d Reason:%s",
        dwUAID, nBanType, nBanDuration,
        szReason ? szReason : "");
    
    return true;
}

// KickPlayer - Kick player from server
// Returns true if kick successful
bool KickPlayer(CUser* pUser, std::uint8_t byKickType, int nParam) {
    if (pUser == nullptr) {
        return false;
    }
    
    // Create kick info structure
    PS_KICK_USER_INFO stKickInfo = {};
    stKickInfo.byKickType = byKickType;
    stKickInfo.nParam = nParam;
    
    // Call kickout function
    pUser->Kickout(&stKickInfo, false);
    
    return true;
}

// ============================================================================
// CMover Static Validation Functions
// ============================================================================

// IDA 0x14036DCB0 - CMover::IsValidPos
// Validates if position coordinates are within valid range
bool CMover::IsValidPos(XVec3* vPos) {
    if (vPos == nullptr) {
        return false;
    }
    
    // Check X coordinate bounds (-10000000 to 10000000)
    if (vPos->x < -10000000.0f || vPos->x > 10000000.0f) {
        return false;
    }
    
    // Check Y coordinate bounds
    if (vPos->y < -10000000.0f || vPos->y > 10000000.0f) {
        return false;
    }
    
    // Check Z coordinate bounds
    if (vPos->z < -10000000.0f || vPos->z > 10000000.0f) {
        return false;
    }
    
    return true;
}

// IDA 0x14036DD40 - CMover::IsValidPos (overload with separate coordinates)
bool CMover::IsValidPos(float fX, float fY) {
    return (fX >= -10000000.0f && fX <= 10000000.0f &&
            fY >= -10000000.0f && fY <= 10000000.0f);
}

// IDA 0x14036DDA0 - CMover::IsValidRot
// Validates rotation value
bool CMover::IsValidRot(float fRot) {
    // Rotation should be between 0 and 2*PI (6.283185)
    return (fRot >= 0.0f && fRot <= 6.283185f);
}
