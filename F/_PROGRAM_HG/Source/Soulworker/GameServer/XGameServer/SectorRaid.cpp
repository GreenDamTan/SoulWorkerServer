// SectorRaid.cpp - Sector Raid/Instance Functions Implementation
// Restored from IDA GameServer.exe port 10004
// Functions: Sector AI, spawning, clearing, boss detection

#include "Soulworker/GameServer/XGameServer/Sector.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XGameServer/actor/Mover/Mover.h"
#include "Soulworker/GameServer/XGameServer/User/User.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XCommon/Packet/XSendPacket.h"
#include "Soulworker/GameServer/XGameServer/GameModeBase.h"
#include "Soulworker/GameServer/XGameServer/GameModeMgr.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <map>
#include <list>
#include <algorithm>

// Forward declarations for types used in implementation
class OperationMode;
class IVScriptInstance;
struct PS_CHAT_NOTICE;
struct ST_LOG_GAME;
struct hkvVec3;

// E_SEND_INFO_TYPE constants
enum E_SEND_INFO_TYPE_Constants {
    eSendInfoTypeSend = 1
};

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
    
    // Mark current step as spawned
    m_bStepSpawned[m_nNowStepSpawn] = true;
    
    // Process spawn boxes from list
    if (!m_listSpawnBox.empty()) {
        auto it = m_listSpawnBox.begin();
        while (it != m_listSpawnBox.end()) {
            VMonsterSpawnInfo* pMonsterSpawn = *it;
            if (pMonsterSpawn && m_nNowStepSpawn + 1 == pMonsterSpawn->m_iStep) {
                // Execute spawn box via maze
                if (m_pMaze) {
                    m_pMaze->ExcuteSpawnBox(pMonsterSpawn, eType);
                }
                EraseSpawnBoxID(pMonsterSpawn->iUniqueID);
                it = m_listSpawnBox.erase(it);
            } else {
                ++it;
            }
        }
        LogHelper::LogDebug("game.contents", "<SECTOR> %d Sector Spawned!", m_pSectorBox->iID);
    }
}

// IDA: ?DieMonsters@CSector@@QEAAXH_N@Z (0x1406CB6B0)
// Kill monsters of specific type
void CSector::DieMonsters(int nMonsterType, bool bSuicide) {
    // Create a copy of the actor map to avoid iterator invalidation
    std::map<unsigned int, XActor*> mapTemp;
    mapTemp.insert(m_mapActor.begin(), m_mapActor.end());
    
    for (auto& pair : mapTemp) {
        XActor* pActor = pair.second;
        if (pActor && XActor::IsMonster(pActor)) {
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (pMonster) {
                // Check monster type matches
                auto pMobTable = pMonster->GetMobTableRef();
                if (pMobTable && pMobTable->Monster_Type == nMonsterType) {
                    short nMotion = -1;
                    if (bSuicide) {
                        nMotion = pMonster->GetDeathMotion();
                    }
                    int nHP = pMonster->GetHP();
                    pMonster->SetDieReason(0xF, nHP);
                    pMonster->SetDie(nMotion, bSuicide);
                    
                    // Clear random summon skills
                    auto pSkillMgr = pMonster->GetSkillMgr();
                    if (pSkillMgr) {
                        pSkillMgr->ClearRandomSummon();
                    }
                }
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
    if (m_bComplete || m_fCheckLastMonsterTime <= 0.0f) {
        return;
    }
    
    m_fCheckLastMonsterTime -= fTime;
    if (m_fCheckLastMonsterTime > 0.0f) {
        return;
    }
    
    // Reset timer
    m_fCheckLastMonsterTime = 60.0f;
    
    // Check for stuck monsters
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (pActor && XActor::GetType(pActor) == 2) {  // Monster type
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (pMonster && pMonster->GetHP() > 0) {
                auto standType = pMonster->GetStandType();
                if (standType != 2 && standType != 3) {  // Not dead/dying
                    // Check if monster can reach ground
                    hkvVec3 vPos = pMonster->GetPosition();
                    hkvVec3 vDestPos = vPos;
                    
                    if (pMonster->GetHeight(&vDestPos, 200.0f)) {
                        // Monster is on ground - log if boss
                        if (pMonster->IsBoss()) {
                            ST_LOG_GAME stLogGame;
                            stLogGame._nUAID = 0;
                            stLogGame._nUCID = 0;
                            stLogGame._sMainType = 51;
                            stLogGame._sSubType = 17;
                            stLogGame.nParam0 = pMonster->GetTableID();
                            stLogGame.nParam1 = (int)vPos.x;
                            stLogGame.nParam2 = (int)vPos.y;
                            stLogGame.nParam3 = (int)vPos.z;
                            stLogGame.nParam4 = pMonster->GetMotionClass();
                            stLogGame.nParam5 = pMonster->GetAIState();
                            stLogGame.nParam6 = pMonster->GetRouletteDayCount();
                            stLogGame.nParam7 = GetSectorBoxID();
                            stLogGame.nParam8 = m_pMaze ? m_pMaze->GetSectorIDFromPos(&vPos) : 0;
                            stLogGame.nParam9 = m_pMaze ? m_pMaze->GetTBMapID() : 0;
                            
                            int nCurHP = pMonster->GetHP();
                            wsprintfW(stLogGame.szParam10, L"%d -> %d", m_nCheckLastMonsterHP, nCurHP);
                            
                            if (m_nCheckLastMonsterHP == nCurHP) {
                                wcscpy_s(stLogGame.szComment, L"LastMonsterInSector : Boss(Check)");
                            } else {
                                wcscpy_s(stLogGame.szComment, L"LastMonsterInSector : Boss");
                            }
                            
                            auto pServer = TXSingleton<XGameServer>::Instance();
                            if (pServer) {
                                pServer->SendDBLog(&stLogGame);
                            }
                        }
                    } else {
                        // Monster is stuck in air - log
                        ST_LOG_GAME stLog;
                        stLog._nUAID = 0;
                        stLog._nUCID = 0;
                        stLog._sMainType = 51;
                        stLog._sSubType = 10;
                        stLog.nParam0 = pMonster->GetTableID();
                        stLog.nParam1 = (int)vPos.x;
                        stLog.nParam2 = (int)vPos.y;
                        stLog.nParam3 = (int)vPos.z;
                        stLog.nParam4 = pMonster->GetMotionClass();
                        stLog.nParam5 = pMonster->GetAIState();
                        stLog.nParam6 = pMonster->GetRouletteDayCount();
                        stLog.nParam7 = GetSectorBoxID();
                        stLog.nParam8 = m_pMaze ? m_pMaze->GetSectorIDFromPos(&vPos) : 0;
                        stLog.nParam9 = m_pMaze ? m_pMaze->GetTBMapID() : 0;
                        wcscpy_s(stLog.szComment, L"LastMonsterInSector");
                        
                        auto pServer = TXSingleton<XGameServer>::Instance();
                        if (pServer) {
                            pServer->SendDBLog(&stLog);
                        }
                    }
                    
                    m_nCheckLastMonsterHP = pMonster->GetHP();
                    return;
                }
            }
        }
    }
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
        m_listSpawnBox.push_back(pSpawnBox);
    }
}

// IDA: ?InitClearType@CSector@@QEAAXXZ (0x1406CA970)
// Initialize sector clear type
void CSector::InitClearType() {
    if (m_pSectorBox && m_pSectorBox->m_eClearType == E_SECTOR_CLEAR_TYPE_SCRIPT) {
        m_bComplete = true;
        m_bCompleteScriptCall = true;
        m_bOpenPortal = true;
    }
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
    
    // Check if game mode is Operation mode (type 6)
    if (m_pGameMode && m_pGameMode->GetModeType() == eGAMEMODE_TYPE_OPERATION) {
        OperationMode* pOperationMode = static_cast<OperationMode*>(m_pGameMode);
        if (pOperationMode) {
            pOperationMode->DamageMonster(pMonster);
        }
    }
}

// IDA: ?InteractBoxOnMode@CSector@@QEAAXPEAVCUser@@H@Z (0x1406CAA80)
// Handle mode-specific box interaction
void CSector::InteractBoxOnMode(CUser* pUser, int nBoxID) {
    if (!pUser) {
        return;
    }
    
    // Check if game mode is Operation mode (type 6)
    if (m_pGameMode && m_pGameMode->GetModeType() == eGAMEMODE_TYPE_OPERATION) {
        OperationMode* pOperationMode = static_cast<OperationMode*>(m_pGameMode);
        if (pOperationMode) {
            pOperationMode->OnInteractBox(pUser, nBoxID);
        }
    }
}

// IDA: ?DiePlayer@CSector@@QEAAXPEAVCUser@@_N@Z (0x1406CAAF0)
// Handle player death in sector
void CSector::DiePlayer(CUser* pUser, bool bAllDie) {
    if (!pUser) {
        return;
    }
    
    // Check if game mode is Party Quest mode (type 3)
    if (m_pGameMode && m_pGameMode->GetModeType() == eGAMEMODE_TYPE_PARTY_QUEST) {
        m_pGameMode->SendNoticePacket(pUser, 32, 1, 0.0f);
        if (bAllDie) {
            m_pGameMode->SetModeState(3);
        }
    }
}

// IDA: ?EnterMode@CSector@@QEAAXPEAVCUser@@@Z (0x1406CAB80)
// Handle player entering game mode in sector
void CSector::EnterMode(CUser* pUser) {
    if (!pUser) {
        return;
    }
    
    if (m_pGameMode) {
        m_pGameMode->Intrusion(pUser);
    }
}

// IDA: ?SetStepSpawn@CSector@@QEAAXHH@Z (0x1406CC050)
// Set step spawn configuration
void CSector::SetStepSpawn(int nIndex, int nRate) {
    if (nIndex >= 0 && nIndex < 10) {
        m_nStepSpawnRate[nIndex] = nRate;
    }
}

// IDA: ?SetStepStop@CSector@@QEAAX_N@Z (0x1406CC080)
// Set step spawn stop flag
void CSector::SetStepStop(bool bStop) {
    m_bStopStepSpawn = bStop;
}

// IDA: ?CheckStepCondition@CSector@@QEAAXXZ (0x1406CC100)
// Check step spawn conditions
void CSector::CheckStepCondition() {
    m_bChangeStepState = false;
    
    if (m_bStopStepSpawn) {
        return;
    }
    
    if (m_bStepSpawned[m_nNowStepSpawn]) {
        // Current step spawned, check if we can advance
        if (m_nStepSpawnRate[m_nNowStepSpawn] > 0 && 
            m_fMonPercent >= (float)m_nStepSpawnRate[m_nNowStepSpawn]) {
            // Advance to next step
            m_nNowStepSpawn++;
            
            // Execute script callback
            if (m_pMaze) {
                IVScriptInstance* pScriptInst = m_pMaze->GetArea();
                if (pScriptInst) {
                    pScriptInst->ExecuteFunctionArg(
                        "OnCompleteSpawnStepCondition",
                        "iiTSoulworker:XMaze;",
                        GetSectorBoxID(),
                        m_nNowStepSpawn,
                        m_pMaze);
                }
            }
            
            // Spawn next wave if not stopped
            if (!m_bStopStepSpawn) {
                SpawnMonster(eSendInfoTypeSend);
            }
            
            LogHelper::LogDebug("game.contents", "<STEP> Complete Step ( %d )", m_nNowStepSpawn);
        }
    } else {
        // Current step not spawned yet, spawn it
        SpawnMonster(eSendInfoTypeSend);
    }
}

// IDA: ?ShowSectorInfo@CSector@@QEAAXPEAVCUser@@@Z (0x1406CC2A0)
// Show sector info to user
void CSector::ShowSectorInfo(CUser* pUser) {
    if (!pUser) {
        return;
    }
    
    wchar_t szBuff[256];
    
    // Send sector header
    swprintf(szBuff, L"--- < Sector : %d > ---", GetSectorBoxID());
    SendChatMessage(pUser, szBuff);
    SendChatMessage(pUser, L"");
    
    // List all actors
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
        if (pMonster) {
            swprintf(szBuff, L"ActorID : %d", pMonster->GetTableID());
            SendChatMessage(pUser, szBuff);
        }
    }
    
    // List spawn boxes
    SendChatMessage(pUser, L"");
    for (auto& pair : m_mapSpawnBoxID) {
        swprintf(szBuff, L"SpawnBox: %d", pair.first);
        SendChatMessage(pUser, szBuff);
    }
    
    // List respawn boxes
    SendChatMessage(pUser, L"");
    for (auto& pair : m_mapRespawnBoxID) {
        swprintf(szBuff, L"SpawnBox: %d", pair.first);
        SendChatMessage(pUser, szBuff);
    }
}

// IDA: ?SendChatMessage@CSector@@QEAAXPEAVCUser@@PEA_W@Z (0x1406CC5F0)
// Send chat message in sector
void CSector::SendChatMessage(CUser* pUser, wchar_t* szMsg) {
    if (!szMsg) {
        return;
    }
    
    PS_CHAT_NOTICE stChat;
    stChat.byType = 0;
    
    // Copy message
    wcscpy_s(stChat.strMsg, szMsg);
    
    // Set color (red)
    wcscpy_s(stChat.strColor, L"FF0000");
    
    // Create and send packet
    XSendPacket xSendPacket(7, 4);
    xSendPacket << stChat;
    
    // Send to user if specified
    XActor* pActor = pUser ? &pUser->XActor : nullptr;
    CGocNetwork::Send(pActor, &xSendPacket);
}
