// Sector.cpp
// CSector implementation - Precisely restored from IDA GameServer.exe

#include "Soulworker/GameServer/XGameServer/Sector.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XCore/XServer/XSendPacket.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <windows.h>

// Forward declarations for external types
class IVScriptInstance;
class GameModeBase;
class OperationMode;

// External function declarations
extern void XPRINT(const char* szFormat, ...);
extern void LogInfo(const char* szCategory, const char* szFormat, ...);
extern void LogDebug(const char* szCategory, const char* szFormat, ...);

// ============================================================================
// Constructor
// IDA: ??0CSector@@QEAA@XZ (0x1406C9F10)
// ============================================================================
CSector::CSector()
    : m_pMaze(nullptr)
    , m_pGameMode(nullptr)
    , m_bCanAI(false)
    , m_bComplete(false)
    , m_bCompleteScriptCall(false)
    , m_bSendQuestMsg(false)
    , m_bOpenPortal(false)
    , m_bInitOpenQuestCondition(false)
    , m_fMonPercent(100.0f)
    , m_nMaxMonCount(0)
    , m_fSectorTime(0.0f)
    , m_nNowStepSpawn(0)
    , m_bTerminateSpawn(false)
    , m_bStopStepSpawn(false)
    , m_bChangeStepState(false)
    , m_bCallQuestScript(false)
    , m_bCallOpenPortalState(false)
    , m_pSectorBox(nullptr)
    , m_pSectorStartBox(nullptr)
    , m_fCheckLastMonsterTime(0.0f)
    , m_nCheckLastMonsterHP(0)
    , m_nRogueKey(0)
    , m_dwRoguelikeLimitTime(0)
    , m_dwRoguelikeLimitTickcount(0)
    , m_nRoguelikeState(0)
{
    // Initialize step spawn arrays
    memset(m_nStepSpawnRate, 0, sizeof(m_nStepSpawnRate));
    memset(m_bStepSpawned, 0, sizeof(m_bStepSpawned));
}

// ============================================================================
// Destructor
// IDA: ??1CSector@@QEAA@XZ (0x1406CA120)
// ============================================================================
CSector::~CSector()
{
    // Clear actor map
    m_mapActor.clear();

    // Clear spawn box ID maps
    m_mapSpawnBoxID.clear();
    m_mapRespawnBoxID.clear();

    // Clear spawn box list
    m_listSpawnBox.clear();

    // Clear lua client sync list
    m_listLuaClientSync.clear();

    // Clear portal box map
    m_mapPotalBox.clear();

    // Clear boss monster map
    m_mapBossMonster.clear();
}

// ============================================================================
// OnUpdate
// IDA: ?OnUpdate@CSector@@QEAA_NM@Z (0x1406CA220)
// ============================================================================
bool CSector::OnUpdate(float fElapsed, float fTime)
{
    // Check last monster time
    CheckLastMonsterTime(fElapsed);

    // Update sector time
    m_fSectorTime += fElapsed;

    // If sector is complete
    if (!m_bComplete) {
        if (m_bChangeStepState) {
            CheckStepCondition();
        }
        CheckRogueLimitTime();
        return false;
    }

    // If complete script already called
    if (m_bCompleteScriptCall) {
        if (m_bOpenPortal) {
            return false;
        }

        // Initialize quest condition if not done
        if (!m_bInitOpenQuestCondition) {
            // TODO: XMaze::InitQuestConditionForSectorClear(m_pMaze);
            m_bInitOpenQuestCondition = true;
        }

        // Check quest condition
        bool bSendMsg = true;
        // TODO: if (XMaze::IsCompleteQuestCondition(m_pMaze, &bSendMsg)) {
        if (bSendMsg) {
            int nSectorBoxID = GetSectorBoxID();
            // TODO: XMaze::SendRoguelikeShopInfo(m_pMaze, nSectorBoxID);
            SetRoguelikeState(2);
            m_bOpenPortal = true;
            SetPortalState();
            // TODO: XMaze::UpdateRandomPortal(m_pMaze, nSectorBoxID);
        } else if (bSendMsg && !m_bSendQuestMsg) {
            m_bSendQuestMsg = true;
            // TODO: Send broadcast packet
        }

        CheckRogueLimitTime();
        return false;
    }

    // Mark complete script call as done
    m_bCompleteScriptCall = true;

    // Log sector clear
    LogInfo("game.contents", "<Clear Sector> ID ( %d )", GetSectorBoxID());

    // Clear game mode reference
    m_pGameMode = nullptr;

    // Execute script
    // TODO: IVScriptInstance* pScriptInst = VGameTrapObject::GetArea(m_pMaze);
    // if (pScriptInst) {
    //     // Execute OnCompleteSector script
    //     pScriptInst->ExecuteFunctionArg("OnCompleteSector", "iTSoulworker:XMaze;", GetSectorBoxID(), m_pMaze);
    //
    //     // Send sector clear packet
    //     // Update sector clear state
    // }

    // TODO: XMaze::UpdateSectorClear(m_pMaze, GetSectorBoxID());
    return true;
}

// ============================================================================
// AddActor
// IDA: ?AddActor@CSector@@QEAAXKPEAVXActor@@@Z (0x1406CA630)
// ============================================================================
void CSector::AddActor(unsigned int dwID, XActor* pActor)
{
    // Insert into actor map
    auto result = m_mapActor.insert(std::make_pair(dwID, pActor));

    if (result.second) {
        // Increment max monster count
        ++m_nMaxMonCount;

        // Calculate monster percentage
        int nCurCount = static_cast<int>(m_mapActor.size());
        m_fMonPercent = 100.0f - (static_cast<float>(nCurCount) / static_cast<float>(m_nMaxMonCount) * 100.0f);

        // Check if this is a monster
        // TODO: if (XActor::IsMonster(pActor)) {
        //     CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
        //     if (pMonster && pMonster->IsBoss()) {
        //         int nTableID = pMonster->GetTableID();
        //         ++m_mapBossMonster[nTableID];
        //     }
        // }
    }
}

// ============================================================================
// DeleteActor
// IDA: ?DeleteActor@CSector@@QEAAXKAEA_N_N@Z (0x1406CA7A0)
// ============================================================================
void CSector::DeleteActor(unsigned int dwID, bool* bBossDie, bool bCheckMonsterCount)
{
    *bBossDie = false;

    // Find actor in map
    auto it = m_mapActor.find(dwID);
    if (it != m_mapActor.end()) {
        XActor* pActor = it->second;

        // Check if this is a monster
        // TODO: if (XActor::IsMonster(pActor)) {
        //     CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
        //     if (pMonster && pMonster->IsBoss()) {
        //         int nTableID = pMonster->GetTableID();
        //         auto bossIt = m_mapBossMonster.find(nTableID);
        //         if (bossIt != m_mapBossMonster.end()) {
        //             if (--bossIt->second <= 0) {
        //                 *bBossDie = true;
        //             }
        //         }
        //     }
        // }

        // Remove from map
        m_mapActor.erase(it);

        // Check monster count
        if (bCheckMonsterCount) {
            CheckMonsterCount();
        }
    }

    CheckLastMonsterDie();
}

// ============================================================================
// InitClearType
// IDA: ?InitClearType@CSector@@QEAAXXZ (0x1406CA970)
// ============================================================================
void CSector::InitClearType()
{
    // TODO: if (m_pSectorBox->m_eClearType == E_SECTOR_CLEAR_TYPE_SCRIPT) {
    //     m_bComplete = true;
    //     m_bCompleteScriptCall = true;
    //     m_bOpenPortal = true;
    // }
}

// ============================================================================
// SetModeState
// IDA: ?SetModeState@CSector@@QEAAXH@Z (0x1406CA9B0)
// ============================================================================
void CSector::SetModeState(int nState)
{
    if (m_pGameMode) {
        // TODO: m_pGameMode->SetModeState(nState);
    }
}

// ============================================================================
// GetGameModeType
// IDA: ?GetGameModeType@CSector@@QEAA?AW4eGAMEMODE_TYPE@@XZ (0x1406CA9F0)
// ============================================================================
eGAMEMODE_TYPE CSector::GetGameModeType()
{
    if (m_pGameMode) {
        // TODO: return GameModeBase::GetModeType(m_pGameMode);
    }
    return eGAMEMODE_TYPE_NONE;
}

// ============================================================================
// DamageMonster
// IDA: ?DamageMonster@CSector@@QEAAXPEAVCMonster@@@Z (0x1406CAA20)
// ============================================================================
void CSector::DamageMonster(CMonster* pMonster)
{
    // TODO: if (m_pGameMode && GameModeBase::GetModeType(m_pGameMode) == eGAMEMODE_TYPE_OPERATION) {
    //     OperationMode* pOpMode = static_cast<OperationMode*>(m_pGameMode);
    //     if (pOpMode) {
    //         pOpMode->DamageMonster(pMonster);
    //     }
    // }
}

// ============================================================================
// InteractBoxOnMode
// IDA: ?InteractBoxOnMode@CSector@@QEAAXPEAVCUser@@H@Z (0x1406CAA80)
// ============================================================================
void CSector::InteractBoxOnMode(CUser* pUser, int nBoxID)
{
    // TODO: if (m_pGameMode && GameModeBase::GetModeType(m_pGameMode) == eGAMEMODE_TYPE_OPERATION) {
    //     OperationMode* pOpMode = static_cast<OperationMode*>(m_pGameMode);
    //     if (pOpMode) {
    //         pOpMode->OnInteractBox(pUser, nBoxID);
    //     }
    // }
}

// ============================================================================
// DiePlayer
// IDA: ?DiePlayer@CSector@@QEAAXPEAVCUser@@_N@Z (0x1406CAAF0)
// ============================================================================
void CSector::DiePlayer(CUser* pUser, bool bAllDie)
{
    // TODO: if (m_pGameMode && GameModeBase::GetModeType(m_pGameMode) == eGAMEMODE_TYPE_PARTY_QUEST) {
    //     GameModeBase::SendNoticePacket(m_pGameMode, pUser, 32, 1, 0.0f);
    //     if (bAllDie) {
    //         m_pGameMode->SetModeState(3);
    //     }
    // }
}

// ============================================================================
// EnterMode
// IDA: ?EnterMode@CSector@@QEAAXPEAVCUser@@@Z (0x1406CAB80)
// ============================================================================
void CSector::EnterMode(CUser* pUser)
{
    if (m_pGameMode) {
        // TODO: m_pGameMode->Intrusion(pUser);
    }
}

// ============================================================================
// CheckMonsterCount
// IDA: ?CheckMonsterCount@CSector@@QEAAHXZ (0x1406CABC0)
// ============================================================================
int CSector::CheckMonsterCount()
{
    int nCurCount = static_cast<int>(m_mapActor.size());
    m_fMonPercent = 100.0f - (static_cast<float>(nCurCount) / static_cast<float>(m_nMaxMonCount) * 100.0f);

    // TODO: Check clear type
    // if (m_pSectorBox->m_eClearType == E_SECTOR_CLEAR_TYPE_KILL_RATIO) {
    //     if (m_fMonPercent >= static_cast<float>(m_pSectorBox->m_iClearKillTypeRatio)) {
    //         m_bComplete = true;
    //     }
    // } else if (m_pSectorBox->m_eClearType == E_SECTOR_CLEAR_TYPE_SPAWN_BOX) {
    //     if (GetSpawnBoxCount() == 0 && GetRespawnBoxCount() == 0 && m_mapActor.empty()) {
    //         m_bComplete = true;
    //     }
    // }

    CheckStepCondition();
    return nCurCount;
}

// ============================================================================
// GetMonsterCount
// IDA: ?GetMonsterCount@CSector@@QEAAHXZ (0x1406CACC0)
// ============================================================================
int CSector::GetMonsterCount()
{
    int nCount = 0;
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
        // TODO: if (XActor::GetType(it->second) == eActorMonster) {
        //     ++nCount;
        // }
    }
    return nCount;
}

// ============================================================================
// GetNormalMonsterCount
// IDA: ?GetNormalMonsterCount@CSector@@QEAAHXZ (0x1406CAD50)
// ============================================================================
int CSector::GetNormalMonsterCount()
{
    int nCount = 0;
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
        // TODO: if (XActor::GetType(it->second) == eActorMonster) {
        //     CMonster* pMonster = dynamic_cast<CMonster*>(it->second);
        //     if (pMonster && pMonster->IsNormalMonster()) {
        //         ++nCount;
        //     }
        // }
    }
    return nCount;
}

// ============================================================================
// CheckLastMonsterDie
// IDA: ?CheckLastMonsterDie@CSector@@QEAAXXZ (0x1406CAE40)
// ============================================================================
void CSector::CheckLastMonsterDie()
{
    if (GetNormalMonsterCount() <= 1 && m_fCheckLastMonsterTime <= 0.0f) {
        m_fCheckLastMonsterTime = 60.0f;
        m_nCheckLastMonsterHP = 0;

        for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
            // TODO: if (XActor::GetType(it->second) == eActorMonster) {
            //     CMonster* pMonster = dynamic_cast<CMonster*>(it->second);
            //     if (pMonster && pMonster->GetHP() > 0) {
            //         m_nCheckLastMonsterHP = pMonster->GetHP();
            //         return;
            //     }
            // }
        }
    }
}

// ============================================================================
// CheckLastMonsterTime
// IDA: ?CheckLastMonsterTime@CSector@@QEAAXM@Z (0x1406CAF80)
// ============================================================================
void CSector::CheckLastMonsterTime(float fTime)
{
    if (!m_bComplete && m_fCheckLastMonsterTime > 0.0f) {
        m_fCheckLastMonsterTime -= fTime;

        if (m_fCheckLastMonsterTime <= 0.0f) {
            m_fCheckLastMonsterTime = 60.0f;

            // Check for stuck monsters
            for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
                // TODO: Check monster position and log if stuck
            }
        }
    }
}

// ============================================================================
// DieMonsters
// IDA: ?DieMonsters@CSector@@QEAAXH_N@Z (0x1406CB6B0)
// ============================================================================
void CSector::DieMonsters(int nMonsterType, bool bSuicide)
{
    // Create a copy of the actor map for safe iteration
    std::map<unsigned int, XActor*> mapTemp = m_mapActor;

    for (auto it = mapTemp.begin(); it != mapTemp.end(); ++it) {
        // TODO: if (XActor::IsMonster(it->second)) {
        //     CMonster* pMonster = dynamic_cast<CMonster*>(it->second);
        //     if (pMonster && CMonster::GetMobTableRef(pMonster)->Monster_Type == nMonsterType) {
        //         short nMotion = -1;
        //         if (bSuicide) {
        //             nMotion = pMonster->GetDeathMotion();
        //         }
        //         int nHP = pMonster->GetHP();
        //         CMoverEx::SetDieReason(pMonster, 0xF, nHP);
        //         pMonster->SetDie(nMotion, bSuicide);
        //     }
        // }
    }
}

// ============================================================================
// DieMonstersAll
// IDA: ?DieMonstersAll@CSector@@QEAAX_N@Z (0x1406CB8D0)
// ============================================================================
void CSector::DieMonstersAll(bool bSuicide)
{
    // Create a copy of the actor map for safe iteration
    std::map<unsigned int, XActor*> mapTemp = m_mapActor;

    for (auto it = mapTemp.begin(); it != mapTemp.end(); ++it) {
        // TODO: if (XActor::IsMonster(it->second)) {
        //     CMonster* pMonster = dynamic_cast<CMonster*>(it->second);
        //     if (pMonster) {
        //         // Skip certain monster types
        //         if (CMonster::GetMobTableRef(pMonster)->Monster_Type == 1 ||
        //             CMonster::GetMobTableRef(pMonster)->Monster_Type == 2) {
        //             continue;
        //         }
        //         short nMotion = -1;
        //         if (bSuicide) {
        //             nMotion = pMonster->GetDeathMotion();
        //         }
        //         int nHP = pMonster->GetHP();
        //         CMoverEx::SetDieReason(pMonster, 0xF, nHP);
        //         pMonster->SetDie(nMotion, bSuicide);
        //     }
        // }
    }
}

// ============================================================================
// EraseSpawnBoxID
// IDA: ?EraseSpawnBoxID@CSector@@QEAAXH@Z (0x1406CBAC0)
// ============================================================================
void CSector::EraseSpawnBoxID(int nUniqueBoxID)
{
    auto it = m_mapSpawnBoxID.find(nUniqueBoxID);
    if (it != m_mapSpawnBoxID.end()) {
        m_mapSpawnBoxID.erase(it);
        LogDebug("game.contents", "<EraseSpawnBoxID> ID ( %d / %d )",
                 nUniqueBoxID, static_cast<int>(m_mapSpawnBoxID.size()));
    }
}

// ============================================================================
// EraseRespawnBoxID
// IDA: ?EraseRespawnBoxID@CSector@@QEAAXH@Z (0x1406CBB70)
// ============================================================================
void CSector::EraseRespawnBoxID(int nUniqueBoxID)
{
    bool bChange = false;
    auto it = m_mapRespawnBoxID.find(nUniqueBoxID);
    if (it != m_mapRespawnBoxID.end()) {
        bChange = true;
        m_mapRespawnBoxID.erase(it);
        LogDebug("game.contents", "<EraseRespawnBoxID> ID ( %d / %d )",
                 nUniqueBoxID, static_cast<int>(m_mapRespawnBoxID.size()));
    }

    if (bChange) {
        CheckMonsterCount();
    }
}

// ============================================================================
// SectorClear
// IDA: ?SectorClear@CSector@@QEAAXXZ (0x1406CBC40)
// ============================================================================
void CSector::SectorClear()
{
    // TODO: if (m_pSectorBox->m_eClearType == E_SECTOR_CLEAR_TYPE_SCRIPT_CALL) {
    //     m_bComplete = true;
    // }
}

// ============================================================================
// SetPortalState
// IDA: ?SetPortalState@CSector@@QEAAXXZ (0x1406CBC70)
// ============================================================================
void CSector::SetPortalState()
{
    // TODO: if (m_pSectorBox && m_pSectorBox->m_iSectorExitID && !m_bCallOpenPortalState) {
    //     m_bCallOpenPortalState = true;
    //
    //     if (XMaze::IsRoguelikeMap(m_pMaze)) {
    //         int nSectorID = XMaze::RunRogueNextSector(m_pMaze);
    //         // Set rogue portal flags
    //     } else {
    //         // Set normal portal/gate flags
    //     }
    // }
}

// ============================================================================
// AddSpawnBox
// IDA: ?AddSpawnBox@CSector@@QEAAXPEAUVMonsterSpawnInfo@@@Z (0x1406CBDD0)
// ============================================================================
void CSector::AddSpawnBox(VMonsterSpawnInfo* pSpawnBox)
{
    m_listSpawnBox.push_back(pSpawnBox);
}

// ============================================================================
// AddSpawnBoxID
// IDA: ?AddSpawnBoxID@CSector@@QEAAXH@Z (0x1406CBE00)
// ============================================================================
void CSector::AddSpawnBoxID(int nBoxUniqueID)
{
    m_mapSpawnBoxID.insert(std::make_pair(nBoxUniqueID, nBoxUniqueID));
}

// ============================================================================
// AddRespawnBoxID
// IDA: ?AddRespawnBoxID@CSector@@QEAAXH@Z (0x1406CBE40)
// ============================================================================
void CSector::AddRespawnBoxID(int nUniqueBoxID)
{
    m_mapRespawnBoxID.insert(std::make_pair(nUniqueBoxID, nUniqueBoxID));
}

// ============================================================================
// TerminateSpawn
// IDA: ?TerminateSpawn@CSector@@QEAAXXZ (0x1406CBE80)
// ============================================================================
void CSector::TerminateSpawn()
{
    m_bTerminateSpawn = true;
    m_bStopStepSpawn = true;
    m_mapSpawnBoxID.clear();
    m_mapRespawnBoxID.clear();
    m_listSpawnBox.clear();
    CheckMonsterCount();
}

// ============================================================================
// SpawnMonster
// IDA: ?SpawnMonster@CSector@@QEAAXW4E_SEND_INFO_TYPE@IXArea@@@Z (0x1406CBEF0)
// ============================================================================
void CSector::SpawnMonster(E_SEND_INFO_TYPE eType)
{
    m_bStepSpawned[m_nNowStepSpawn] = true;

    if (!m_listSpawnBox.empty()) {
        for (auto it = m_listSpawnBox.begin(); it != m_listSpawnBox.end(); ) {
            VMonsterSpawnInfo* pMonsterSpawn = *it;
            // TODO: if (m_nNowStepSpawn + 1 == pMonsterSpawn->m_iStep) {
            //     XMaze::ExcuteSpawnBox(m_pMaze, pMonsterSpawn, eType);
            //     EraseSpawnBoxID(pMonsterSpawn->iUniqueID);
            //     it = m_listSpawnBox.erase(it);
            // } else {
            //     ++it;
            // }
            ++it;
        }
        LogDebug("game.contents", "<SECTOR> %d Sector Spawned!", GetSectorBoxID());
    }
}

// ============================================================================
// SetStepSpawn
// IDA: ?SetStepSpawn@CSector@@QEAAXHH@Z (0x1406CC050)
// ============================================================================
void CSector::SetStepSpawn(int nIndex, int nRate)
{
    m_nStepSpawnRate[nIndex] = nRate;
}

// ============================================================================
// SetStepStop
// IDA: ?SetStepStop@CSector@@QEAAX_N@Z (0x1406CC080)
// ============================================================================
void CSector::SetStepStop(bool bStop)
{
    if (bStop != m_bStopStepSpawn) {
        m_bChangeStepState = true;
    }
    m_bStopStepSpawn = bStop;
    LogDebug("game.contents", "<STEP> SetStop ( %d / %d )", GetSectorBoxID(), bStop);
}

// ============================================================================
// CheckStepCondition
// IDA: ?CheckStepCondition@CSector@@QEAAXXZ (0x1406CC100)
// ============================================================================
void CSector::CheckStepCondition()
{
    m_bChangeStepState = false;

    if (!m_bStopStepSpawn) {
        if (m_bStepSpawned[m_nNowStepSpawn]) {
            // Check if we should advance to next step
            if (m_nStepSpawnRate[m_nNowStepSpawn] != 0 &&
                m_fMonPercent >= static_cast<float>(m_nStepSpawnRate[m_nNowStepSpawn])) {
                ++m_nNowStepSpawn;

                // Execute script callback
                // TODO: IVScriptInstance* pScriptInst = VGameTrapObject::GetArea(m_pMaze);
                // if (pScriptInst) {
                //     pScriptInst->ExecuteFunctionArg("OnCompleteSpawnStepCondition",
                //                                     "iiTSoulworker:XMaze;",
                //                                     GetSectorBoxID(), m_nNowStepSpawn, m_pMaze);
                // }

                if (!m_bStopStepSpawn) {
                    SpawnMonster(eSendInfoTypeSend);
                }
                LogDebug("game.contents", "<STEP> Complete Step ( %d )", m_nNowStepSpawn);
            }
        } else {
            SpawnMonster(eSendInfoTypeSend);
        }
    }
}

// ============================================================================
// IsBossSector
// IDA: ?IsBossSector@CSector@@QEAA_NXZ (0x1406CC270)
// ============================================================================
bool CSector::IsBossSector()
{
    // TODO: return m_pSectorBox && m_pSectorBox->m_eType == E_SECTOR_TYPE_BOSS;
    return false;
}

// ============================================================================
// ShowSectorInfo
// IDA: ?ShowSectorInfo@CSector@@QEAAXPEAVCUser@@@Z (0x1406CC2A0)
// ============================================================================
void CSector::ShowSectorInfo(CUser* pUser)
{
    wchar_t szBuff[256];
    swprintf(szBuff, L"--- < Sector : %d > ---", GetSectorBoxID());
    SendChatMessage(pUser, szBuff);
    SendChatMessage(pUser, L"");

    // Show actor info
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
        // TODO: Show monster info
    }

    // Show spawn box info
    SendChatMessage(pUser, L"");
    for (auto it = m_mapSpawnBoxID.begin(); it != m_mapSpawnBoxID.end(); ++it) {
        wchar_t szMsg[256];
        swprintf(szMsg, L"SpawnBox: %d", it->first);
        SendChatMessage(pUser, szMsg);
    }

    // Show respawn box info
    SendChatMessage(pUser, L"");
    for (auto it = m_mapRespawnBoxID.begin(); it != m_mapRespawnBoxID.end(); ++it) {
        wchar_t szMsg[256];
        swprintf(szMsg, L"SpawnBox: %d", it->first);
        SendChatMessage(pUser, szMsg);
    }
}

// ============================================================================
// SendChatMessage
// IDA: ?SendChatMessage@CSector@@QEAAXPEAVCUser@@PEA_W@Z (0x1406CC5F0)
// ============================================================================
void CSector::SendChatMessage(CUser* pUser, wchar_t* szMeg)
{
    // TODO: PS_CHAT_NOTICE stChat;
    // stChat.byType = 0;
    // wcscpy_s(stChat.strMsg, szMeg);
    // wcscpy_s(stChat.strColor, L"FF0000");
    //
    // XSendPacket xSendPacket(0x07, 0x04);
    // xSendPacket << stChat;
    //
    // if (pUser) {
    //     CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    // }
}

// ============================================================================
// GetStartPos
// IDA: ?GetStartPos@CSector@@QEAA_NAEAUXVec3@@AEAM@Z (0x1406CC7E0)
// ============================================================================
bool CSector::GetStartPos(XVec3& vStartPos, float& fRot)
{
    if (!m_pSectorStartBox) {
        return false;
    }

    // TODO: XWorldManager* pWorldMgr = TXSingleton<XWorldManager>::Instance();
    // vStartPos.x = pWorldMgr->fRand(m_pSectorStartBox->PosTopLeft.x, m_pSectorStartBox->PosBottomRight.x);
    // vStartPos.y = pWorldMgr->fRand(m_pSectorStartBox->PosTopLeft.y, m_pSectorStartBox->PosBottomRight.y);
    // vStartPos.z = m_pSectorStartBox->PosTopLeft.z;
    // fRot = m_pSectorStartBox->fRotate;

    return true;
}

// ============================================================================
// AddPortal
// IDA: ?AddPortal@CSector@@QEAAXPEAUSTMagePotalBox@@@Z (0x1406CC8D0)
// ============================================================================
void CSector::AddPortal(STMagePotalBox* pPortal)
{
    if (pPortal) {
        // TODO: m_mapPotalBox.insert(std::make_pair(pPortal->nBoxIndex, pPortal));
    }
}

// ============================================================================
// StartRogueSector
// IDA: ?StartRogueSector@CSector@@QEAAXXZ (0x1406CC910)
// ============================================================================
void CSector::StartRogueSector()
{
    if (m_dwRoguelikeLimitTime) {
        std::int64_t nTimeMs = 1000LL * m_dwRoguelikeLimitTime;
        m_dwRoguelikeLimitTickcount = GetTickCount64() + nTimeMs;
    }
}

// ============================================================================
// CheckRogueLimitTime
// IDA: ?CheckRogueLimitTime@CSector@@QEAAXXZ (0x1406CC970)
// ============================================================================
void CSector::CheckRogueLimitTime()
{
    if (!m_bComplete && m_dwRoguelikeLimitTickcount != 0) {
        if (m_dwRoguelikeLimitTickcount <= GetTickCount64()) {
            // TODO: XMaze::SetRoguelikeTimeout(m_pMaze);
            m_dwRoguelikeLimitTickcount = 0;
        }
    }
}

// ============================================================================
// SetRoguelikeState
// IDA: ?SetRoguelikeState@CSector@@QEAAXH@Z (0x1406CC9D0)
// ============================================================================
void CSector::SetRoguelikeState(int nState)
{
    if (m_nRoguelikeState != nState) {
        m_nRoguelikeState = nState;

        if (nState == 3) {
            // TODO: IVScriptInstance* pScriptInst = VGameTrapObject::GetArea(m_pMaze);
            // if (pScriptInst && pScriptInst->HasFunction("OnCompleteRoguelikeSector")) {
            //     pScriptInst->ExecuteFunctionArg("OnCompleteRoguelikeSector",
            //                                     "iTSoulworker:XMaze;",
            //                                     GetSectorBoxID(), m_pMaze);
            // }
        }
    }
}

// ============================================================================
// CallPartyQuestScript
// IDA: ?CallPartyQuestScript@CSector@@QEAAXXZ (0x1406CCA80)
// ============================================================================
void CSector::CallPartyQuestScript()
{
    if (!m_bCallQuestScript) {
        // TODO: IVScriptInstance* pScriptInst = VGameTrapObject::GetArea(m_pMaze);
        // if (pScriptInst) {
        //     pScriptInst->ExecuteFunctionArg("OnPartyQuestEvent",
        //                                     "iTSoulworker:XMaze;",
        //                                     GetSectorBoxID(), m_pMaze);
        //     m_bCallQuestScript = true;
        // }
    }
}

// ============================================================================
// CheckClearState
// IDA: ?CheckClearState@CSector@@QEAAXXZ (0x1406CCB00)
// ============================================================================
void CSector::CheckClearState()
{
    // TODO: if (m_pSectorBox->m_eClearType == E_SECTOR_CLEAR_TYPE_QUEST) {
    //     int nID = m_pSectorBox->iID % 10000 + 100 * (10 * (XArea::GetTBMapID(m_pMaze) / 10) + 1);
    //     TB_SECTORQUEST* pTBSectorQuest = XResourceMgr::GetTB_SECTORQUEST(nID);
    //     if (pTBSectorQuest) {
    //         if (pTBSectorQuest->Before_Check_Quest_ID_01 || pTBSectorQuest->Final_Check_Quest_ID_01) {
    //             m_bComplete = true;
    //         }
    //     }
    // }
}

// ============================================================================
// AddLuaClientSync
// IDA: ?AddLuaClientSync@CSector@@QEAAXAEAUST_LUA_CLIENT_SYNC@@@Z (0x1406CCBC0)
// ============================================================================
void CSector::AddLuaClientSync(ST_LUA_CLIENT_SYNC& stSync)
{
    m_listLuaClientSync.push_back(stSync);
}

// ============================================================================
// SendLuaClientSync
// IDA: ?SendLuaClientSync@CSector@@QEAAXPEAVCUser@@@Z (0x1406CCBF0)
// ============================================================================
void CSector::SendLuaClientSync(CUser* pUser)
{
    if (!pUser) {
        return;
    }

    for (auto it = m_listLuaClientSync.begin(); it != m_listLuaClientSync.end(); ++it) {
        // TODO: ST_LUA_CLIENT_SYNC& stInfo = *it;
        // if (!stInfo.nLimitTime || stInfo.nLimitTime > GetTickCount64()) {
        //     XSendPacket xSendPacket(0x11, 0x60);
        //     xSendPacket << stInfo.nType;
        //     xSendPacket << stInfo.nValue;
        //     CGocNetwork::Send(&pUser->XActor, &xSendPacket);
        // }
    }
}

// ============================================================================
// ShowSectorMonsterInfo
// IDA: ?ShowSectorMonsterInfo@CSector@@QEAAXPEAVCUser@@@Z (0x1406CCDD0)
// ============================================================================
void CSector::ShowSectorMonsterInfo(CUser* pUser)
{
    wchar_t szBuff[256];
    swprintf(szBuff, L"--- < Sector : %d > ---", GetSectorBoxID());
    SendChatMessage(pUser, szBuff);
    SendChatMessage(pUser, L"< Spawned Monster Info >");

    int nNum = 1;
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
        // TODO: Show detailed monster info
        wchar_t szMsg[256];
        // swprintf with monster details
        SendChatMessage(pUser, szMsg);
        ++nNum;
    }
}

// ============================================================================
// Helper Functions
// ============================================================================

int CSector::GetSectorBoxID() const
{
    // TODO: return m_pSectorBox ? m_pSectorBox->iID : 0;
    return 0;
}

int CSector::GetSectorBoxUniqueID() const
{
    // TODO: return m_pSectorBox ? m_pSectorBox->iUniqueID : 0;
    return 0;
}

int CSector::GetSpawnBoxCount() const
{
    return static_cast<int>(m_mapSpawnBoxID.size());
}

int CSector::GetRespawnBoxCount() const
{
    return static_cast<int>(m_mapRespawnBoxID.size());
}
