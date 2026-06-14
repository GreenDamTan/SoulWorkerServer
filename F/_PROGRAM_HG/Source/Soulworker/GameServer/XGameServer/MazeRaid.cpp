// MazeRaid.cpp - Raid/Instance Dungeon System Implementation
// Restored from IDA GameServer.exe port 10004
// Functions: Raid creation, management, boss spawn/ai, rewards, instance lifecycle

#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XGameServer/Sector.h"
#include "Soulworker/GameServer/XGameServer/actor/Mover/Mover.h"
#include "Soulworker/GameServer/XGameServer/User/User.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XCommon/Packet/XSendPacket.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMapMaze.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMazeSync.h"
#include "Soulworker/GameServer/XCore/XArea/IXArea.h"
#include <cmath>

using E_BROADCAST_TYPE::eAll;

// ============================================================================
// STCasualRaidTime Structure Implementation
// ============================================================================

// IDA: ?reset@STCasualRaidTime@@QEAAXXZ (0x140354390)
// Clear all timer values
void STCasualRaidTime::reset() {
    nIntValue = 0;
    fFloatValue = 0.0f;
    fWaitSendTime = 0.0f;
}

// IDA: ??0STCasualRaidTime@@QEAA@XZ (0x1403543C0)
// Constructor - initialize with reset
STCasualRaidTime::STCasualRaidTime() {
    reset();
}

// ============================================================================
// XMaze Raid/Instance Functions
// ============================================================================

// IDA: ?UpdateCasualRaidTimer@XMaze@@QEAAXM@Z (0x14031D850)
// Update casual raid timer with elapsed time
void XMaze::UpdateCasualRaidTimer(float fElapsed) {
    // Update main timer
    if (m_stCasualRaidTime.fFloatValue > 0.0f) {
        m_stCasualRaidTime.fFloatValue -= fElapsed;
        if (m_stCasualRaidTime.fFloatValue <= 0.0f) {
            m_stCasualRaidTime.reset();
        }
    }
    
    // Update wait send timer
    if (m_stCasualRaidTime.fWaitSendTime > 0.0f) {
        m_stCasualRaidTime.fWaitSendTime -= fElapsed;
        if (m_stCasualRaidTime.fWaitSendTime <= 0.0f) {
            SendCasualRaidTimer();
            m_stCasualRaidTime.fWaitSendTime = 0.0f;
        }
    }
}

// IDA: ?ShowCasualRaidTimer@XMaze@@QEAAXHMM@Z (0x14032B390)
// Broadcast casual raid timer to all users in maze
void XMaze::ShowCasualRaidTimer(int nValue, float fTime, float fTime2) {
    XSendPacket xPacket(0x11, 0x38);
    
    // Packet structure: type(0) + nValue + fTime + fTime2
    xPacket.XParse << 0;  // iType = 0
    xPacket.XParse << nValue;
    xPacket.XParse << fTime;
    xPacket.XParse << fTime2;
    
    SendBroadCast(&xPacket, nullptr, eAll);
    
    // Store timer values
    m_stCasualRaidTime.nIntValue = nValue;
    m_stCasualRaidTime.fFloatValue = fTime;
    // Note: IDA shows extraPositionData call - likely debug/logging
}

// IDA: ?SetupCasualRaidTimer@XMaze@@QEAAXXZ (0x14032B4B0)
// Setup casual raid timer for sending
void XMaze::SetupCasualRaidTimer() {
    if (m_stCasualRaidTime.fFloatValue > 0.0f) {
        m_stCasualRaidTime.fWaitSendTime = 5.0f;  // 5 second delay
    }
}

// IDA: ?SendCasualRaidTimer@XMaze@@QEAAXXZ (0x14032B4E0)
// Send current casual raid timer to users
void XMaze::SendCasualRaidTimer() {
    if (m_stCasualRaidTime.fFloatValue > 0.0f) {
        ShowCasualRaidTimer(m_stCasualRaidTime.nIntValue, 
                           m_stCasualRaidTime.fFloatValue, 
                           0.0f);
    }
}

// IDA: ?IsBossSector@XMaze@@QEAA_NXZ (0x1402A4C40)
// Check if current sector is a boss sector
bool XMaze::IsBossSector() const {
    return m_bBossSector;
}

// IDA: ?SetBossSector@XMaze@@QEAAX_N@Z (0x14032FFF0)
// Set boss sector flag and notify control server
void XMaze::SetBossSector(bool bFlag) {
    // Only update if maze is not finished and flag changed
    if (m_stMazeGameState.m_nMazeState != 4 && m_bBossSector != bFlag) {
        m_bBossSector = bFlag;
        
        if (m_bBossSector) {
            // Notify control server about boss sector
            PS_MAZE_UPDATE_INFO stUpdateMaze;
            stUpdateMaze.uxMapID = GetInstanceID();
            stUpdateMaze.nState = 2;  // Boss state
            stUpdateMaze.nUserCount = static_cast<int>(m_objectScanner.size());
            
            // Add all wait-enter users
            for (auto& pair : m_mapWaitEnterMazeUser) {
                stUpdateMaze.vecMemberInfo.push_back(pair.second);
            }
            
            // Send to control server
            XSendPacket xSendPacket(0xF2, 0x26);
            xSendPacket << stUpdateMaze;
            
            auto pServer = XGameServer::Instance();
            if (pServer) {
                pServer->GetControlSocket().Send(&xSendPacket);
            }
            
            LogHelper::LogDebug("game.contents", 
                "<< eSUB_CMD_SERVER_UPDATE_MAZE 2 >> 2 ");
        }
    }
}

// IDA: ?GoRoguelikeBoss@XMaze@@QEAAXH@Z (0x140344CB0)
// Go to roguelike boss sector
void XMaze::GoRoguelikeBoss(int nState) {
    // Get first user in maze
    auto it_user = m_objectScanner.begin();
    if (it_user == m_objectScanner.end()) {
        return;
    }
    
    // Cast to CUser
    CUser* pUser = dynamic_cast<CUser*>(it_user->second);
    if (!pUser) {
        return;
    }
    
    // Update roguelike step
    auto pEntity = pUser->GetGOC<CGocEntity>();
    if (pEntity) {
        pEntity->UpdateRoguelikeStep();
    }
    
    // Spawn boss sector monsters
    int nSectorID = m_pRogueBossSector->GetSectorBoxID();
    SpawnSectorMonsterForOpt(nSectorID);
    
    // Set next sector to boss sector
    m_pRogueNextSector = m_pRogueBossSector;
    
    // Send sector load resource packet
    PS_MAZE_SECTOR_LOAD_RESOURCE stSector;
    stSector.nSectorID = m_pRogueNextSector->GetSectorBoxID();
    
    XSendPacket xSendPacket(0x11, 0x04);
    xSendPacket << stSector;
    SendBroadCast(&xSendPacket, nullptr, eAll);
    
    // Warp all users to boss sector
    if (m_pRogueNextSector) {
        STPosInfo posInfo;
        m_pRogueNextSector->GetStartPos(&posInfo.vPos, &posInfo.fRot);
        
        if (!AllUserWarp(&posInfo.vPos, posInfo.fRot, 0)) {
            LogHelper::LogError("game.contents", 
                "SendWarpPotal Error - Cant find Portal Pos ( %d )", 13827);
            return;
        }
        
        // Send roguelike current info
        auto pEntity2 = pUser->GetGOC<CGocEntity>();
        if (pEntity2) {
            pEntity2->SendRoguelikeCurrentInfo();
        }
        
        // Send warp packet to all users
        STWarp warpInfo;
        warpInfo.byResult = 0;
        warpInfo.xPos = posInfo.vPos;
        warpInfo.fRot = posInfo.fRot;
        warpInfo.nSectorID = m_pRogueNextSector->GetSectorBoxID();
        
        XSendPacket v26(0x04, 0x0E);
        v26.XParse << 0;  // nPortalID
        v26 << warpInfo;
        SendBroadCast(&v26, nullptr, eAll);
        
        // Send boss sector message if configured
        int LastSectorID = GetLastSectorID();
        int nMazeID = GetTBMapID();
        auto pServer = XGameServer::Instance();
        if (pServer) {
            auto pInfo = pServer->GetResourceMgr().FindRoguelikeData(nMazeID, LastSectorID);
            if (pInfo && pInfo->BI_Sector_Start_Message > 0) {
                PS_MODE_MAZE_NOTICE stNotice;
                stNotice.nType = pInfo->BI_Sector_Start_Message_Type;
                stNotice.nValue = pInfo->BI_Sector_Start_Message;
                
                XSendPacket packet(0x33, 0x30);
                packet << stNotice;
                CGocNetwork::Send(&pUser->GetXActor(), &packet);
            }
        }
    }
}

// IDA: ?SectorClear@XMaze@@QEAAXH@Z (0x14032A420)
// Mark sector as cleared
void XMaze::SectorClear(int nSectorID) {
    m_textDBLog.AddLog(42, nSectorID, 0, "");
    
    // Get sector unique ID
    int BatchLayerLevel = GetBatchLayerLevel();
    int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSectorID, BatchLayerLevel);
    
    // Find sector
    auto it = m_mapSector.find(iBoxUniqueID);
    if (it != m_mapSector.end() && it->second) {
        CSector* pSector = it->second;
        if (pSector) {
            pSector->SetComplete(true);
        }
    }
}

// IDA: ?SpawnSectorMonster@XMaze@@QEAAXH@Z (0x14031F900)
// Spawn monsters in specified sector
void XMaze::SpawnSectorMonster(int nSector) {
    int BatchLayerLevel = GetBatchLayerLevel();
    int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSector, BatchLayerLevel);
    
    auto it = m_mapSector.find(iBoxUniqueID);
    if (it != m_mapSector.end() && it->second) {
        CSector* pSector = it->second;
        if (pSector) {
            pSector->SpawnMonster(eSendInfoTypeSend);
        }
    }
}

// IDA: ?SpawnSectorMonsterForOpt@XMaze@@QEAAXH@Z (0x14031F9B0)
// Spawn monsters in sector with optimization
void XMaze::SpawnSectorMonsterForOpt(int nSector) {
    // TODO: Implementation differs from SpawnSectorMonster - needs IDA analysis
    // Currently using simplified version
    SpawnSectorMonster(nSector);
}

// IDA: ?AllDestroySectorMonster@XMaze@@QEAAXH@Z (0x140329A60)
// Destroy all monsters in sector
void XMaze::AllDestroySectorMonster(int nSectorID) {
    m_textDBLog.AddLog(16, nSectorID, 0, "");
    
    int BatchLayerLevel = GetBatchLayerLevel();
    int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSectorID, BatchLayerLevel);
    
    auto it = m_mapSector.find(iBoxUniqueID);
    if (it != m_mapSector.end() && it->second) {
        CSector* pSector = it->second;
        if (pSector) {
            bool bSuicide = false;
            
            // Check if maze type requires suicide (instant destroy)
            if (m_pTBMazeInfo) {
                auto MazeType = m_pTBMazeInfo->Maze_Type;
                if (MazeType == 14 || MazeType == 18 || MazeType == 19) {
                    bSuicide = true;
                }
            }
            
            pSector->DieMonstersAll(bSuicide);
        }
    }
}

// IDA: ?RunSectorAI@XMaze@@QEAAXH_N@Z (0x14031F7C0)
// Run AI for specified sector
void XMaze::RunSectorAI(int nSector, bool bIsPotal) {
    m_textDBLog.AddLog(19, nSector, 0, "");
    
    int BatchLayerLevel = GetBatchLayerLevel();
    int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSector, BatchLayerLevel);
    
    auto it = m_mapSector.find(iBoxUniqueID);
    if (it != m_mapSector.end()) {
        CSector* pSector = it->second;
        if (pSector) {
            pSector->SetAI(true);
            pSector->CheckClearState();
            SetLastSectorID(nSector, bIsPotal);
            
            bool IsBossSector = pSector->IsBossSector();
            SetBossSector(IsBossSector);
        }
        
        LogHelper::LogDebug("game.contents", 
            "<MAZE> RunSectorAI (MAP:%d / SECTOR:%d) ", 
            m_pTBMazeInfo ? m_pTBMazeInfo->ID : 0, nSector);
    }
}

// IDA: ?GetSectorFromPos@XMaze@@QEAAPEAVCSector@@AEBVhkvVec3@@@Z (0x14031F670)
// Get sector from world position
CSector* XMaze::GetSectorFromPos(const hkvVec3& vPos) {
    for (auto& pair : m_mapSector) {
        CSector* pSector = pair.second;
        if (pSector) {
            VSectorBoxInfo* pBox = reinterpret_cast<VSectorBoxInfo*>(
                pSector->GetModeEndTime());
            
            if (pBox) {
                // Check if position is within sector bounds
                if (vPos.x >= pBox->PosTopLeft.x && 
                    vPos.x <= pBox->PosBottomRight.x &&
                    vPos.y >= pBox->PosTopLeft.y && 
                    vPos.y <= pBox->PosBottomRight.y) {
                    return pSector;
                }
            }
        }
    }
    
    return nullptr;
}

// IDA: ?GetSector@XMaze@@QEAAPEAVCSector@@H@Z (0x14032B270)
// Get sector by ID
CSector* XMaze::GetSector(int nSectorID) {
    int BatchLayerLevel = GetBatchLayerLevel();
    int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSectorID, BatchLayerLevel);
    
    auto it = m_mapSector.find(iBoxUniqueID);
    if (it != m_mapSector.end()) {
        return it->second;
    }
    
    return nullptr;
}

// IDA: ?GetSectorIDFromPos@XMaze@@QEAAHAEBVhkvVec3@@@Z (0x14031F450)
// Get sector ID from world position
int XMaze::GetSectorIDFromPos(const hkvVec3& vPos) {
    CSector* pSector = GetSectorFromPos(vPos);
    if (pSector) {
        return pSector->GetSectorBoxID();
    }
    return 0;
}

// IDA: ?GetSectorUniqueIDFromPos@XMaze@@QEAAHAEBVhkvVec3@@@Z (0x14031F560)
// Get sector unique ID from world position
int XMaze::GetSectorUniqueIDFromPos(const hkvVec3& vPos) {
    CSector* pSector = GetSectorFromPos(vPos);
    if (pSector) {
        return pSector->GetSectorBoxUniqueID();
    }
    return 0;
}

// IDA: ?DieEventSectorMonster@XMaze@@QEAAXH_N@Z (0x14032A4F0)
// Handle monster death event in sector
void XMaze::DieEventSectorMonster(int nSectorID, bool bEvent) {
    // TODO: Implementation - needs IDA analysis
    // Stub for compilation
}

// IDA: ?ResetAllSectorFlags@XMaze@@QEAAXXZ (0x140329B80)
// Reset all sector flags
void XMaze::ResetAllSectorFlags() {
    // TODO: Implementation - needs IDA analysis
    // Stub for compilation
}

// IDA: ?AllUserWarpInSector@XMaze@@QEAAXVhkvVec3@@M@Z (0x140323EF0)
// Warp all users in sector to position
void XMaze::AllUserWarpInSector(hkvVec3 vPos, float fRot) {
    // TODO: Implementation - needs IDA analysis
    // Stub for compilation
}

// IDA: ?SendSectorCompleteState@XMaze@@QEAAXHH@Z (0x14031FAF0)
// Send sector complete state to users
void XMaze::SendSectorCompleteState(int nSectorID, int nState) {
    // TODO: Implementation - needs IDA analysis
    // Stub for compilation
}

// ============================================================================
// CMonster Raid Functions
// ============================================================================

// IDA: ?IsBoss_Named_Raid@CMonster@@UEAAHXZ (0x1403585C0)
// Check if monster is a raid boss (rank 3, 4, or 5)
bool CMonster::IsBoss_Named_Raid() {
    if (!m_pMobTableRef) {
        return false;
    }
    
    // Monster_Rank: 3=Named, 4=Boss, 5=Raid Boss
    return m_pMobTableRef->Monster_Rank == 4 ||
           m_pMobTableRef->Monster_Rank == 5 ||
           m_pMobTableRef->Monster_Rank == 3;
}

// IDA: ?IsBoss@CMonster@@UEAAHXZ (0x140358570)
// Check if monster is a boss
bool CMonster::IsBoss() {
    if (!m_pMobTableRef) {
        return false;
    }
    
    // Check for boss rank (4 or 5)
    return m_pMobTableRef->Monster_Rank == 4 ||
           m_pMobTableRef->Monster_Rank == 5;
}

// IDA: ?IsRemainBossMonster@CMonster@@QEAA_NXZ (0x14035A950)
// Check if monster is a remaining boss (should stay alive)
bool CMonster::IsRemainBossMonster() {
    if (!m_pMobTableRef) {
        return false;
    }
    
    // Check various conditions for remain boss
    // Boss monsters with specific types should remain
    if (IsBoss_Named_Raid()) {
        // Additional checks from IDA
        // TODO: Full logic restoration from IDA
        return true;
    }
    
    return false;
}

// ============================================================================
// CBattleZone Raid Functions
// ============================================================================

// IDA: ?IsWorldModeBoss@CBattleZone@@QEAA_NXZ (0x1401A8560)
// Check if battle zone has active world mode boss
bool CBattleZone::IsWorldModeBoss() {
    // Iterate through all game world modes
    for (auto it = m_mapGameWorldMode.begin(); 
         it != m_mapGameWorldMode.end(); ++it) {
        
        auto& pWorldMode = it->second;
        if (!pWorldMode) {
            continue;
        }
        
        int nModeID = pWorldMode->GetModeID();
        
        // Get mode info from resource manager
        auto pServer = XGameServer::Instance();
        if (!pServer) {
            continue;
        }
        
        TB_MODE_DISTRICT6* pTBMode = pServer->GetResourceMgr().GetTB_MODE_DISTRICT6(nModeID);
        if (pTBMode) {
            // Check if this is a boss world mode (Start_Type == 1)
            if (pTBMode->Start_Type == 1) {
                // Check if boss is active (state == 1)
                if (pWorldMode->GetState() == 1) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

// ============================================================================
// CGocInventory Raid Functions
// ============================================================================

// IDA: ?UseCasualRaidDecreaseEnterLimit@CGocInventory@@QEAA_NV?$shared_ptr@VCItem@@@tr1@std@@F@Z (0x1400C4F40)
// Use item to decrease casual raid enter limit count
bool CGocInventory::UseCasualRaidDecreaseEnterLimit(
    std::tr1::shared_ptr<CItem> pItem, short shSlot) {
    
    // Validate item
    if (!pItem) {
        return false;
    }
    
    TB_ITEM* pTB_Item = pItem->GetItemTable();
    if (!pTB_Item) {
        return false;
    }
    
    // Get owner mover
    CMover* pMover = GetOwnerMover();
    if (!pMover) {
        return false;
    }
    
    // Get recode component
    auto pRecode = pMover->GetGOC<CGocRecode>();
    if (pRecode) {
        pRecode->DecreaseEnterCasualMazeLimiteCount();
    }
    
    // Get item serial
    __int64 biItemSerial = pItem->GetSerial();
    
    // Prepare item structures
    ST_GET_INFO stGetInfo;
    PS_RES_STORAGE_INFO psCreateItem;
    PS_RES_STORAGE_INFO psUpdateItem;
    
    // Reduce item
    unsigned char InvenType = pItem->GetInvenType();
    if (!ReduceItem3(InvenType, shSlot, 1, 0x2C, &psUpdateItem)) {
        return false;
    }
    
    // Log game event
    ST_LOG_GAME stLog;
    stLog._sSubType = 22;
    
    CUser* pUser = dynamic_cast<CUser*>(pMover);
    if (pUser) {
        UXMapID mapID = pUser->GetMapInsID();
        stLog.nParam3 = static_cast<int>(mapID.nMapID >> 16);
    }
    
    // Update item end
    if (!UpdateItemEnd(0x2C, &psUpdateItem, &stLog)) {
        return false;
    }
    
    // Send DB packet
    PS_GACHA_LIST stGachaList;
    unsigned char byFlag = 0;
    
    IXObject* pObject = pUser ? &pUser->GetXActor() : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 0x21, 0x12);
    
    // Get quest ID
    int QuestID = 0;  // TODO: Get from quest condition
    xSendDBPacket.XParse << QuestID;
    xSendDBPacket << psUpdateItem;
    xSendDBPacket << psCreateItem;
    xSendDBPacket << stGetInfo;
    
    int CurID = pItem->GetCurID();
    xSendDBPacket.XParse << CurID;
    xSendDBPacket.XParse << 0;
    xSendDBPacket << stGachaList;
    xSendDBPacket.XParse << byFlag;
    xSendDBPacket.XParse << biItemSerial;
    
    auto pServer = XGameServer::Instance();
    if (pServer) {
        pServer->SendDBGame(&xSendDBPacket);
    }
    
    return true;
}

// ============================================================================
// CGocRecode Maze Functions
// ============================================================================

// IDA: ?InitMaze@CGocRecode@@QEAAXH@Z (0x140144F00)
// Initialize maze record for new maze run
void CGocRecode::InitMaze(int nMazeID) {
    // Reset all maze stats
    m_nMazeClearTime = 0;
    m_nMazeEpisodeNo = 0;
    m_nMazeDifficulty = 0;
    m_nMazeLevel = 0;
    m_nMazeExp = 0;
    m_nMazeMoney = 0;
    m_nMazeGroup = 0;
    m_nPlayPointOrder = 1;
    m_fPlayPoint = 0.0f;
    m_nRank = 0;
    m_fExpValue = 1.0f;
    m_fMoneyValue = 1.0f;
    m_fPartyExpValue = 0.0f;
    m_fPartyMoneyValue = 0.0f;
    m_nClearPoint = 0;
    m_fFinalRewardExp = 0.0f;
    m_nFinalRewardMoney = 0;
    m_dwPlayTime = 0;
    m_nComboBPByUnity = 0;
    m_nDSPointByUnity = 0;
    
    m_nMazeID = nMazeID;
    
    // Get maze info from resource
    auto pServer = XGameServer::Instance();
    if (pServer) {
        TB_MAZE_INFO* pMaze = pServer->GetResourceMgr().GetTB_MAZE_INFO(m_nMazeID);
        if (pMaze) {
            m_nMazeClearTime = 1000 * pMaze->Maze_ClearTime;
            m_nMazeEpisodeNo = pMaze->Maze_Episode_No;
            m_nMazeDifficulty = pMaze->Maze_Difficulty_Type;
            m_nMazeLevel = pMaze->Req_Min_Lv;
            m_nMazeExp = pMaze->Maze_Reward_EXP;
            m_nMazeMoney = pMaze->Maze_Reward_Money;
            m_nMazeGroup = pMaze->Maze_Group;
        }
    }
    
    // Clear record arrays
    memset(m_nMazeRecode, 0, sizeof(m_nMazeRecode));
    memset(&m_stBaseRewardItem, 0, sizeof(m_stBaseRewardItem));
    memset(&m_stCashRewardItem, 0, sizeof(m_stCashRewardItem));
    memset(&m_stCashRewardItemEx, 0, sizeof(m_stCashRewardItemEx));
    memset(m_stCashRewardItemListEX, 0, sizeof(m_stCashRewardItemListEX));
    memset(&m_stHiddenEventRewardItem, 0, sizeof(m_stHiddenEventRewardItem));
    
    m_byHiddenEventSendPost = 0;
    m_nPartyMemberCount = 0;
    m_bShowCutscene = false;
}

// IDA: ?MazeReward@CGocRecode@@QEAAHK_NH@Z (0x140145930)
// Process maze rewards after completion
int CGocRecode::MazeReward(unsigned int dwPlayTime, bool bApplyReward, int nPartyMemberCount) {
    m_dwPlayTime = dwPlayTime;
    m_nMazeClearTime -= dwPlayTime;
    m_nMazeClearTime /= 1000;
    
    unsigned int dwPlayTime_sec = dwPlayTime / 1000;
    unsigned int dwRankingPlayTime = dwPlayTime / 100;
    m_nPartyMemberCount = nPartyMemberCount;
    
    // Calculate play points
    float fPlay_Point = 0.0f;
    CalculateFirst(&fPlay_Point, nPartyMemberCount);
    CalculateSecond(&fPlay_Point);
    CalculateThird(&fPlay_Point);
    CalculateFourth(&fPlay_Point);
    
    // Determine rank
    GetRank(fPlay_Point);
    
    // Apply rewards if requested
    if (bApplyReward) {
        ApplyReward();
    }
    
    // Update clear info
    UpdateClearInfo(dwRankingPlayTime);
    
    // Update quest conditions
    CMover* pMover = GetOwnerMover();
    if (pMover) {
        // Update quest
        auto pQuest = pMover->GetGOC<CGocQuest>();
        if (pQuest) {
            pQuest->UpdateCondition(10, eCONDITION_TARGET_MAZE, m_nMazeID, 1, 0);
        }
        
        // Update daily mission
        auto pMission = pMover->GetGOC<CGocDailyMission>();
        if (pMission) {
            bool bPartyWith = nPartyMemberCount > 1;
            pMission->UpdateMazeClearType(
                eDAILY_MISSION_TARGET_MAZE,
                m_nMazeID,
                m_nRank,
                dwPlayTime_sec,
                bPartyWith);
        }
        
        // Update class event
        auto pClassEvent = pMover->GetGOC<CGocClassEvent>();
        if (pClassEvent) {
            pClassEvent->CanBroachEquip(0x0A, 0);
        }
        
        // Update weekly mission
        auto pWeeklyMission = pMover->GetGOC<CGocWeeklyMission>();
        if (pWeeklyMission) {
            pWeeklyMission->CheckWeeklyMissionUpdate(2, m_nMazeID, 1);
        }
        
        // Update achievement
        unsigned char byMazeType = 0;
        auto pServer = XGameServer::Instance();
        if (pServer) {
            TB_MAZE_INFO* pTB_MazeInfo = pServer->GetResourceMgr().GetTB_MAZE_INFO(m_nMazeID);
            if (pTB_MazeInfo) {
                byMazeType = pTB_MazeInfo->Maze_Type;
            }
        }
        
        auto pAchieve = pMover->GetGOC<CGocAchieve>();
        if (pAchieve) {
            pAchieve->UpdatemMazeClearAchieve(m_nRank, dwPlayTime_sec, m_nMazeID, byMazeType);
        }
        
        // Update entity title
        auto pEntity = pMover->GetGOC<CGocEntity>();
        if (pEntity) {
            pEntity->UpdateOpenTitle(5, m_nMazeID);
        }
    }
    
    return m_nRank;
}
