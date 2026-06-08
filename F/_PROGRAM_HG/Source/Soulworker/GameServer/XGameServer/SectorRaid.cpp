// SectorRaid.cpp - Sector Raid/Instance Functions Implementation
// Restored from IDA GameServer.exe port 10004
// Functions: Sector AI, spawning, clearing, boss detection

#include "Soulworker/GameServer/XGameServer/Sector.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XGameServer/actor/Mover/Mover.h"
#include "Soulworker/GameServer/XGameServer/actor/User/User.h"
#include "Soulworker/GameServer/XGameServer/actor/Monster/Monster.h"
#include "Soulworker/GameServer/XGameServer/XGameServer.h"
#include "Soulworker/GameServer/XCore/Resource/XResourceMgr.h"
#include "Soulworker/Common/XNet/XCommon/Packet/XSendPacket.h"
#include <map>

// ============================================================================
// CSector Raid Functions
// ============================================================================

// IDA: ?IsBossSector@CSector@@QEAA_NXZ (0x1406CC270)
// Check if this sector is a boss sector
bool CSector::IsBossSector() {
    // Check if sector type is boss
    return m_eSectorType == E_SECTOR_TYPE_BOSS;
}

// IDA: ?SetComplete@CSector@@QEAAX_N@Z (0x14028D480)
// Set sector completion state
void CSector::SetComplete(bool bComplete) {
    m_bComplete = bComplete;
    
    if (bComplete) {
        // Sector completed - open portals if available
        SetPortalOpen(true);
    }
}

// IDA: ?IsComplete@CSector@@QEAA_NXZ (0x140310280)
// Check if sector is completed
bool CSector::IsComplete() const {
    return m_bComplete;
}

// IDA: ?IsSpawnedAll@CSector@@QEAA_NXZ (0x14028D4A0)
// Check if all monsters have been spawned
bool CSector::IsSpawnedAll() {
    // Check if spawn boxes are empty
    return m_vecSpawnBoxID.empty();
}

// IDA: ?SetPortalOpen@CSector@@QEAAX_N@Z (0x14028D4E0)
// Set portal open state
void CSector::SetPortalOpen(bool bOpen) {
    m_bPortalOpen = bOpen;
}

// IDA: ?SetAI@CSector@@QEAAX_N@Z (0x14028D460)
// Enable or disable sector AI
void CSector::SetAI(bool bEnable) {
    m_bAI = bEnable;
}

// IDA: ?IsTerminateSpawn@CSector@@QEAA_NXZ (0x1403102A0)
// Check if spawn is terminated
bool CSector::IsTerminateSpawn() const {
    return m_bTerminateSpawn;
}

// IDA: ?Reset@CSector@@QEAAXXZ (0x1403102E0)
// Reset sector state
void CSector::Reset() {
    m_bComplete = false;
    m_bAI = false;
    m_bPortalOpen = false;
    m_bTerminateSpawn = false;
    m_vecSpawnBoxID.clear();
    m_vecRespawnBoxID.clear();
}

// IDA: ?SetSectorBox@CSector@@QEAAXPEAUVSectorBoxInfo@@@Z (0x1403102C0)
// Set sector box info
void CSector::SetSectorBox(VSectorBoxInfo* pBox) {
    if (pBox) {
        m_SectorBox = *pBox;
    }
}

// IDA: ?SetStartBox@CSector@@QEAAXPEAUVSectorStartBoxInfo@@@Z (0x140310310)
// Set sector start box info
void CSector::SetStartBox(VSectorStartBoxInfo* pBox) {
    if (pBox) {
        m_StartBox = *pBox;
    }
}

// IDA: ?SetRogueKey@CSector@@QEAAXH@Z (0x140310330)
// Set roguelike key for sector
void CSector::SetRogueKey(int nKey) {
    m_nRogueKey = nKey;
}

// IDA: ?SetRoguelikeLimitTime@CSector@@QEAAXK@Z (0x140310350)
// Set roguelike time limit
void CSector::SetRoguelikeLimitTime(unsigned long dwTime) {
    m_dwRoguelikeLimitTime = dwTime;
}

// IDA: ?GetRoguelikeState@CSector@@QEAAHXZ (0x140310370)
// Get roguelike state
int CSector::GetRoguelikeState() {
    return m_nRoguelikeState;
}

// IDA: ?GetSectorBoxID@CSector@@QEAAHXZ (0x14028D3E0)
// Get sector box ID
int CSector::GetSectorBoxID() const {
    return m_SectorBox.iID;
}

// IDA: ?GetSectorBoxUniqueID@CSector@@QEAAHXZ (0x14028D420)
// Get sector box unique ID
int CSector::GetSectorBoxUniqueID() const {
    return m_SectorBox.iUniqueID;
}

// IDA: ?GetActor@CSector@@QEAAPEAV?$map@KPEAVXActor@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKPEAVXActor@@@std@@@3@@std@@XZ (0x14002F250)
// Get actor map
std::map<unsigned int, XActor*>* CSector::GetActor() {
    return &m_mapActor;
}

// IDA: ?SpawnMonster@CSector@@QEAAXW4E_SEND_INFO_TYPE@IXArea@@@Z (0x1406CBEF0)
// Spawn monsters in sector
void CSector::SpawnMonster(E_SEND_INFO_TYPE eType) {
    if (m_bTerminateSpawn) {
        return;
    }
    
    // Process spawn boxes
    for (int nBoxUniqueID : m_vecSpawnBoxID) {
        // TODO: Call maze spawn function for each box
        // Needs XMaze reference
    }
}

// IDA: ?DieMonsters@CSector@@QEAAXH_N@Z (0x1406CB6B0)
// Kill monsters of specific type
void CSector::DieMonsters(int nMonsterType, bool bSuicide) {
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (pActor) {
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (pMonster) {
                // TODO: Check monster type and kill
                // Needs monster type field access
            }
        }
    }
}

// IDA: ?DieMonstersAll@CSector@@QEAAX_N@Z (0x1406CB8D0)
// Kill all monsters in sector
void CSector::DieMonstersAll(bool bSuicide) {
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (pActor) {
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (pMonster) {
                if (bSuicide) {
                    pMonster->Suicide();
                } else {
                    pMonster->Die();
                }
            }
        }
    }
}

// IDA: ?CheckMonsterCount@CSector@@QEAAHXZ (0x1406CABC0)
// Check and return monster count
int CSector::CheckMonsterCount() {
    int nCount = 0;
    
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (pActor) {
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (pMonster && pMonster->IsAlive()) {
                nCount++;
            }
        }
    }
    
    return nCount;
}

// IDA: ?GetMonsterCount@CSector@@QEAAHXZ (0x1406CACC0)
// Get total monster count
int CSector::GetMonsterCount() {
    return CheckMonsterCount();
}

// IDA: ?GetNormalMonsterCount@CSector@@QEAAHXZ (0x1406CAD50)
// Get normal (non-boss) monster count
int CSector::GetNormalMonsterCount() {
    int nCount = 0;
    
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (pActor) {
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (pMonster && pMonster->IsAlive() && !pMonster->IsBoss()) {
                nCount++;
            }
        }
    }
    
    return nCount;
}

// IDA: ?CheckLastMonsterDie@CSector@@QEAAXXZ (0x1406CAE40)
// Check if last monster died - trigger sector completion
void CSector::CheckLastMonsterDie() {
    int nMonsterCount = CheckMonsterCount();
    
    if (nMonsterCount == 0) {
        // All monsters dead - sector complete
        SetComplete(true);
    }
}

// IDA: ?CheckLastMonsterTime@CSector@@QEAAXM@Z (0x1406CAF80)
// Check last monster spawn time
void CSector::CheckLastMonsterTime(float fTime) {
    // TODO: Implementation - needs time tracking fields
    // Stub for compilation
}

// IDA: ?SectorClear@CSector@@QEAAXXZ (0x1406CBC40)
// Clear sector state
void CSector::SectorClear() {
    m_bComplete = false;
    m_bAI = false;
    
    // Kill all monsters
    DieMonstersAll(true);
    
    // Clear actor map
    m_mapActor.clear();
    
    // Clear spawn boxes
    m_vecSpawnBoxID.clear();
    m_vecRespawnBoxID.clear();
}

// IDA: ?SetPortalState@CSector@@QEAAXXZ (0x1406CBC70)
// Update portal state based on sector completion
void CSector::SetPortalState() {
    SetPortalOpen(m_bComplete);
}

// IDA: ?EraseSpawnBoxID@CSector@@QEAAXH@Z (0x1406CBAC0)
// Remove spawn box ID
void CSector::EraseSpawnBoxID(int nUniqueBoxID) {
    auto it = std::find(m_vecSpawnBoxID.begin(), m_vecSpawnBoxID.end(), nUniqueBoxID);
    if (it != m_vecSpawnBoxID.end()) {
        m_vecSpawnBoxID.erase(it);
    }
}

// IDA: ?EraseRespawnBoxID@CSector@@QEAAXH@Z (0x1406CBB70)
// Remove respawn box ID
void CSector::EraseRespawnBoxID(int nUniqueBoxID) {
    auto it = std::find(m_vecRespawnBoxID.begin(), m_vecRespawnBoxID.end(), nUniqueBoxID);
    if (it != m_vecRespawnBoxID.end()) {
        m_vecRespawnBoxID.erase(it);
    }
}

// IDA: ?AddSpawnBoxID@CSector@@QEAAXH@Z (0x1406CBE00)
// Add spawn box ID
void CSector::AddSpawnBoxID(int nBoxUniqueID) {
    m_vecSpawnBoxID.push_back(nBoxUniqueID);
}

// IDA: ?AddRespawnBoxID@CSector@@QEAAXH@Z (0x1406CBE40)
// Add respawn box ID
void CSector::AddRespawnBoxID(int nUniqueBoxID) {
    m_vecRespawnBoxID.push_back(nUniqueBoxID);
}

// IDA: ?TerminateSpawn@CSector@@QEAAXXZ (0x1406CBE80)
// Terminate monster spawning
void CSector::TerminateSpawn() {
    m_bTerminateSpawn = true;
    m_vecSpawnBoxID.clear();
}

// IDA: ?AddSpawnBox@CSector@@QEAAXPEAUVMonsterSpawnInfo@@@Z (0x1406CBDD0)
// Add spawn box info
void CSector::AddSpawnBox(VMonsterSpawnInfo* pSpawnBox) {
    if (pSpawnBox) {
        // TODO: Process spawn box info
        // Needs VMonsterSpawnInfo structure definition
    }
}

// IDA: ?InitClearType@CSector@@QEAAXXZ (0x1406CA970)
// Initialize sector clear type
void CSector::InitClearType() {
    // TODO: Implementation - needs clear type field
    // Stub for compilation
}

// IDA: ?SetModeState@CSector@@QEAAXH@Z (0x1406CA9B0)
// Set game mode state
void CSector::SetModeState(int nState) {
    m_nModeState = nState;
}

// IDA: ?GetGameModeType@CSector@@QEAA?AW4eGAMEMODE_TYPE@@XZ (0x1406CA9F0)
// Get game mode type
eGAMEMODE_TYPE CSector::GetGameModeType() {
    return m_eGameModeType;
}

// IDA: ?DamageMonster@CSector@@QEAAXPEAVCMonster@@@Z (0x1406CAA20)
// Handle monster damage in sector
void CSector::DamageMonster(CMonster* pMonster) {
    if (!pMonster) {
        return;
    }
    
    // TODO: Implementation - needs damage tracking
    // Stub for compilation
}

// IDA: ?InteractBoxOnMode@CSector@@QEAAXPEAVCUser@@H@Z (0x1406CAA80)
// Handle mode-specific box interaction
void CSector::InteractBoxOnMode(CUser* pUser, int nBoxID) {
    if (!pUser) {
        return;
    }
    
    // TODO: Implementation - needs mode-specific logic
    // Stub for compilation
}

// IDA: ?DiePlayer@CSector@@QEAAXPEAVCUser@@_N@Z (0x1406CAAF0)
// Handle player death in sector
void CSector::DiePlayer(CUser* pUser, bool bAllDie) {
    if (!pUser) {
        return;
    }
    
    // TODO: Implementation - needs death handling
    // Stub for compilation
}

// IDA: ?EnterMode@CSector@@QEAAXPEAVCUser@@@Z (0x1406CAB80)
// Handle player entering game mode in sector
void CSector::EnterMode(CUser* pUser) {
    if (!pUser) {
        return;
    }
    
    // TODO: Implementation - needs mode enter logic
    // Stub for compilation
}

// IDA: ?SetStepSpawn@CSector@@QEAAXHH@Z (0x1406CC050)
// Set step spawn configuration
void CSector::SetStepSpawn(int nIndex, int nRate) {
    // TODO: Implementation - needs step spawn system
    // Stub for compilation
}

// IDA: ?SetStepStop@CSector@@QEAAX_N@Z (0x1406CC080)
// Set step spawn stop flag
void CSector::SetStepStop(bool bStop) {
    m_bStepStop = bStop;
}

// IDA: ?CheckStepCondition@CSector@@QEAAXXZ (0x1406CC100)
// Check step spawn conditions
void CSector::CheckStepCondition() {
    if (m_bStepStop) {
        return;
    }
    
    // TODO: Implementation - needs step condition logic
    // Stub for compilation
}

// IDA: ?ShowSectorInfo@CSector@@QEAAXPEAVCUser@@@Z (0x1406CC2A0)
// Show sector info to user
void CSector::ShowSectorInfo(CUser* pUser) {
    if (!pUser) {
        return;
    }
    
    // TODO: Implementation - needs sector info packet
    // Stub for compilation
}

// IDA: ?SendChatMessage@CSector@@QEAAXPEAVCUser@@PEA_W@Z (0x1406CC5F0)
// Send chat message in sector
void CSector::SendChatMessage(CUser* pUser, wchar_t* szMsg) {
    if (!pUser || !szMsg) {
        return;
    }
    
    // TODO: Implementation - needs chat packet
    // Stub for compilation
}
