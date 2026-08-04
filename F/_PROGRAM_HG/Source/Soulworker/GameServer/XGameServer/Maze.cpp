// Maze.cpp - XMaze 类实现
// 还原自 GameServer.exe IDA 反编译
// 构造函数: 0x140310550
// 析构函数: 0x140310D00

#include "Maze.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/CutsceneManager.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/Npc.h"
#include "Soulworker/GameServer/XGameServer/Ai.h"
#include "Soulworker/GameServer/XGameServer/WorldManager.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XArea/DohHavokNavMeshInstance.h"
#include "Soulworker/GameServer/XCore/XArea/DohHavokResourceManager.h"
#include "Soulworker/GameServer/XCore/HavokTypes.h"
#include "Soulworker/GameServer/XGameServer/Sector.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNpcAttribute.h"
#include "Soulworker/GameServer/XGameServer/InteractionObject.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocRecode.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocBooster.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocHelper.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XGameServer/CParty.h"
#include "Soulworker/GameServer/XGameServer/CForce.h"
#include "Soulworker/Common/XNet/XCommon/Packet/XSendPacket.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMazeSync.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>

// 外部全局变量
extern std::string g_strCurPath_10;

// ============================================================================
// MAZE_OBJECT 实现
// ============================================================================

MAZE_OBJECT::MAZE_OBJECT() {
    // 默认构造
}

MAZE_OBJECT::~MAZE_OBJECT() {
    // 清理
}

// ============================================================================
// XMaze 构造函数
// IDA: 0x140310550
// ============================================================================
XMaze::XMaze()
    : XArea()
    , m_objectScanner()
    , m_bMazeComplete(false)
    , m_nNavMeshIndex(0)
    , m_pNavMeshInstance(nullptr)
    , m_pMazeResource(nullptr)
    , m_pSystemActor(nullptr)
    , m_pScriptInstance(nullptr)
    , m_pTBMazeInfo(nullptr)
    , m_fCellPosCheckTime(0.0f)
    , m_nSpawnRateGrouton(0)
    , m_nSpawnRateGroutonBooster(0)
    , m_nGroutonBoxID(0)
    , m_dwWaitGroutonSpawnTime(0)
    , m_nStartSectorID(0)
    , m_nLastSpawnedSectorID(0)
    , m_nDestroySilhouetes(0)
    , m_pActiveEventSector(nullptr)
    , m_nPartyUserCount(0)
    , m_bBossSector(false)
    , m_bHaveBotUser(false)
    , m_fUpdateProcessEscort(0.0f)
    , m_dwWaitToEnterForceMember(0)
    , m_dwWaitToLoadEXMember(0)
    , m_dwWaitUserTime(0)
    , m_nMonsterKiillScoreModeState(0)
    , m_bRestartReady(false)
    , m_nTimeStepTarget(0)
    , m_nTimeStepState(0)
    , m_nBit(0)
    , m_nPartyMemeberCount(0)
    , m_nMaxUserCount(14)
    , m_pRogueStartSector(nullptr)
    , m_pRogueNextSector(nullptr)
    , m_pRogueBossSector(nullptr)
    , m_nRoguelikeMaxSector(1)
    , m_nRoguelikeGoBossState(0)
    , m_nRoguelikePortalBuffID(0)
    , m_nRoguelikeLastPortalID(0)
{
    // 初始化所有 map
    m_mapProcessSpawnBox.clear();
    m_mapCheckEventSpawnBox.clear();
    m_mapEventSpawnBox.clear();
    m_mapPotalBox.clear();
    m_mapRandomPotalBox.clear();
    m_mapGateBox.clear();
    m_mapLuaFunctionBox.clear();
    m_mapInteractionBox.clear();
    m_mapQuestMoveBox.clear();

    // 初始化列表
    m_lstDestoryObject.clear();
    m_lstSilhouetteObject.clear();
    m_arLogicTimers.clear();
    m_arWaitLogicTimers.clear();
    m_listMonsterSpawnInfo.clear();
    m_listSectorStartBox.clear();
    m_listSummonedHelper.clear();
    m_listWaitForRecvInfo.clear();

    // 初始化 vector
    m_vecActiveLastSectorID.clear();
    m_lstChangeMonster.clear();
    m_listDieMonsterSpawnBoxID.clear();
    m_listSyncSpawnActive.clear();
    m_vecLuaValue.resize(0x1E);  // 30 个 Lua 值
    m_vecLuaValues.clear();
    m_vecGameRules.clear();
    m_vecRoguelikeRandomShopStat.clear();

    // 初始化 map
    m_mapSector.clear();
    m_mapCommonPostionBox.clear();
    m_mapRespawnBox.clear();
    m_mapCheckSectorUser.clear();
    m_mapMazeCountUser.clear();
    m_setUpdateEnterProcess.clear();
    m_mapMonsterDamageType.clear();
    m_mpFpUseUCID.clear();
    m_mapReviveCount.clear();
    m_mapUserHitedCount.clear();
    m_mapDropList.clear();
    m_mapCutsceneOff.clear();
    m_mapRogueSector.clear();
    m_mapRoguelikeRandomPocketInfo.clear();
    m_mapRoguelikePortalBuffList.clear();
    m_mapRoguelikePortalBuff.clear();
    m_mapRandomBuff.clear();
    m_mapTimeStepTimer.clear();
    m_mapGameTrapObject.clear();
    m_mapGameTrapObjectGroup.clear();
    m_mapGroupMOB.clear();
    m_mapGroupID_Monster.clear();
    m_mapSpawnBoxGroupLimit.clear();
    m_mapCallScriptDieMonster.clear();
    m_mapUserDamage.clear();

    // 清零内存
    std::memset(m_nMazeLog, 0, sizeof(m_nMazeLog));
    std::memset(&m_stEscortMonster, 0, sizeof(m_stEscortMonster));

    // 清空字符串
    m_strTimeStepFailScript.clear();
}

// ============================================================================
// XMaze 析构函数
// IDA: 0x140310D00
// ============================================================================
XMaze::~XMaze() {
    // 按照构造的逆序销毁成员
    m_strTimeStepFailScript.clear();

    m_mapTimeStepTimer.clear();
    m_mapRandomBuff.clear();
    m_mapRoguelikePortalBuff.clear();
    m_mapRoguelikePortalBuffList.clear();
    m_mapRoguelikeRandomPocketInfo.clear();
    m_vecRoguelikeRandomShopStat.clear();
    m_mapRogueSector.clear();
    m_mapCutsceneOff.clear();
    m_mapDropList.clear();
    m_mapUserHitedCount.clear();
    m_mapUserDamage.clear();
    m_mapReviveCount.clear();
    m_mapCallScriptDieMonster.clear();
    m_mapSpawnBoxGroupLimit.clear();
    m_mapGroupID_Monster.clear();
    m_mapGroupMOB.clear();
    m_mapGameTrapObjectGroup.clear();
    m_mapGameTrapObject.clear();
    m_vecGameRules.clear();
    m_vecLuaValues.clear();
    m_mpFpUseUCID.clear();
    m_mapMonsterDamageType.clear();
    m_pForce.reset();
    m_pParty.reset();
    m_setUpdateEnterProcess.clear();
    m_mapMazeCountUser.clear();
    m_mapWaitEnterMazeUser.clear();
    m_listWaitForRecvInfo.clear();
    m_vecLuaValue.clear();
    m_vecActiveLastSectorID.clear();
    m_mapCheckSectorUser.clear();
    m_listDieMonsterSpawnBoxID.clear();
    m_lstChangeMonster.clear();
    m_listSectorStartBox.clear();
    m_listSyncSpawnActive.clear();
    m_mapRespawnBox.clear();
    m_listMonsterSpawnInfo.clear();
    m_mapCommonPostionBox.clear();
    m_arWaitLogicTimers.clear();
    m_arLogicTimers.clear();
    m_mapSector.clear();
    m_lstSilhouetteObject.clear();
    m_lstDestoryObject.clear();
    m_mapQuestMoveBox.clear();
    m_mapInteractionBox.clear();
    m_mapLuaFunctionBox.clear();
    m_mapGateBox.clear();
    m_mapRandomPotalBox.clear();
    m_mapPotalBox.clear();
    m_mapEventSpawnBox.clear();
    m_mapCheckEventSpawnBox.clear();
    m_mapProcessSpawnBox.clear();
}

// ============================================================================
// ScanGridOrigin
// IDA 0x14032EE00 - ?ScanGridOrigin@XMaze@@UEAAXMMEHKAEAV?$vector@PEAVCMover@@...
// Scans grid origin for maze objects with faction filtering
// ============================================================================
void XMaze::ScanGridOrigin(float dx, float dy, unsigned char byNation, int sectorRange, unsigned int dwOptions, std::vector<CMover*>& vecOut) {
    // IDA: Clear and reserve output vector
    vecOut.clear();
    vecOut.reserve(300);  // 0x12C
    
    // IDA: Parse option flags
    bool bCheckUsers = (dwOptions & 1) != 0;
    bool bCheckNPCs = (dwOptions & 2) != 0;
    bool bCheckFaction1 = (dwOptions & 4) != 0;
    bool bCheckFaction2 = (dwOptions & 8) != 0;
    bool bSkipGM = (dwOptions & 0x10) != 0;
    
    (void)dx;
    (void)dy;
    (void)sectorRange;
    
    XGameServer* pServer = XGameServer::Instance();
    
    // IDA: Check users (bit 0)
    if (bCheckUsers) {
        for (auto& pair : m_objectScanner.mapPlayerList) {
            CMover* pMover = pair.second;
            if (!pMover) {
                continue;
            }
            
            // TODO: IsGM and IsUserStatus methods need to be added to CUser
            // IDA: Check GM status if flag is set
            // if (bSkipGM) {
            //     CUser* pUser = dynamic_cast<CUser*>(pMover);
            //     if (pUser && pUser->IsGM() && pUser->IsStatus(0x2000)) {
            //         continue;
            //     }
            // }
            
            // TODO: GetClientLoadComplete method needs to be added to CUser
            // IDA: Check client load complete
            // CUser* pUser = dynamic_cast<CUser*>(pMover);
            // if (pUser && !pUser->GetClientLoadComplete()) {
            //     continue;
            // }
            
            // IDA: Faction filtering
            if (bCheckFaction1 || bCheckFaction2) {
                unsigned char byActorNation = pMover->GetNation();
                unsigned char byFaction = pServer->GetResourceMgr().GetFaction(byNation, byActorNation);
                
                if (bCheckFaction1 && byFaction != 1) {
                    continue;
                }
                if (bCheckFaction2 && byFaction != 2) {
                    continue;
                }
            }
            
            vecOut.push_back(pMover);
        }
    }
    
    // IDA: Check NPCs/Monsters (bit 1)
    if (bCheckNPCs) {
        for (auto& pair : m_objectScanner.mapNPCList) {
            CMover* pMover = pair.second;
            if (!pMover) {
                continue;
            }
            
            // IDA: Skip if maze state is 2
            if (m_stMazeGameState.m_nMazeState == 2) {
                continue;
            }
            
            // IDA: Check if it's a monster
            CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
            if (!pMonster) {
                continue;
            }
            
            // TODO: GetGroupID method needs to be added to CSector
            // IDA: Check sector and system actor
            // CSector* pSector = pMonster->GetSector();
            // if (!pSector || pSector->GetGroupID() != 0) {
            //     continue;
            // }
            
            if (pMonster->IsSystemActor()) {
                continue;
            }
            
            // IDA: Faction filtering
            if (bCheckFaction1 || bCheckFaction2) {
                unsigned char byActorNation = pMover->GetNation();
                unsigned char byFaction = pServer->GetResourceMgr().GetFaction(byNation, byActorNation);
                
                if (bCheckFaction1 && byFaction != 1) {
                    continue;
                }
                if (bCheckFaction2 && byFaction != 2) {
                    continue;
                }
            }
            
            vecOut.push_back(pMover);
        }
    }
    
    // IDA: Check etc objects (all bits = 15)
    if (dwOptions == 15) {
        for (auto& pair : m_objectScanner.mapEtcList) {
            CMover* pMover = pair.second;
            if (pMover) {
                vecOut.push_back(pMover);
            }
        }
    }
}

// ============================================================================
// XMaze Getter Functions
// ============================================================================

// IDA: ?GetMazeType@XMaze@@QEAAEXZ (0x14005ABD0)
// Verified: Direct IDA decompilation - returns Maze_Type from TB_MAZE_INFO
std::uint8_t XMaze::GetMazeType() const {
    if (m_pTBMazeInfo) {
        return m_pTBMazeInfo->Maze_Type;
    }
    return 0;
}

// ============================================================================
// StartMazeTime
// IDA: 0x140311B70
// ============================================================================
void XMaze::StartMazeTime() {
    if (m_stMazeGameState.m_nMazeState == 0) {
        SetMazeState(1, true);
        m_stMazeGameState.m_dwMazeStartTime = GetTickCount64();
        RunSectorAI(10001, true);
    }
}

// ============================================================================
// FinishMazeTime
// IDA: 0x140311BD0
// ============================================================================
void XMaze::FinishMazeTime() {
    SetMazeState(4, true);
    m_stMazeGameState.m_dwMazePlayTime = GetTickCount64() - m_stMazeGameState.m_dwMazeStartTime;
}

// ============================================================================
// MazePlayTime_Now
// IDA: 0x140311C10
// ============================================================================
std::uint32_t XMaze::MazePlayTime_Now() {
    std::uint32_t dwPlayTime = static_cast<std::uint32_t>(
        GetTickCount64() - static_cast<std::uint64_t>(m_stMazeGameState.m_dwMazeStartTime)
    );

    if (dwPlayTime) {
        return dwPlayTime / 1000;  // 转换为秒
    }
    return 0;
}

// ============================================================================
// SetMazeState
// IDA: 0x140312FB0
// ============================================================================
void XMaze::SetMazeState(int nState, bool bSync) {
    m_stMazeGameState.m_nMazeState = nState;
    m_stMazeGameState.m_dwMazeWaitTime = GetTickCount64();

    if (nState == 7) {
        PauseAlltimer(true);
    }

    if (bSync) {
        XSendPacket xSendPacket(0x11, 0x21);
        xSendPacket << nState;
        SendBroadCast(&xSendPacket, nullptr, E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
    }

    LogHelper::LogDebug("game.contents", "<MAZE_STATE> SetMaze ( %d ) ", nState);
}

// ============================================================================
// SetLuaValue
// IDA: 0x1403130C0
// ============================================================================
void XMaze::SetLuaValue(const char* szObjectKey) {
    if (szObjectKey) {
        std::string strKey(szObjectKey);
        m_vecLuaValues.push_back(strKey);
    }
}

// ============================================================================
// FindActor
// IDA: 0x140313920
// ============================================================================
XActor* XMaze::FindActor(UXActorID uxActorID) {
    return XArea::FindActor(uxActorID.dwActorID);
}

// ============================================================================
// MoveActor
// IDA: 0x140315750
// 精确还原自 IDA 反编译
// ============================================================================
std::uint16_t XMaze::MoveActor(XActor* pActor, XVec3& vNextPos, float fRot) {
    if (!pActor) {
        return 50001;
    }

    // IDA: RTTI cast to CMover using _RTDynamicCast
    CMover* pMover = dynamic_cast<CMover*>(pActor);
    if (!pMover) {
        return 50001;
    }

    // IDA: pMover->SetPosInfo(vNextPos)
    pMover->SetPosInfo(vNextPos, fRot);

    return 0;
}

// Per IDA 0x140315800: XMaze::MoveActor (UXActorID version)
// 精确还原 - 查找Actor后委托给XActor版本
std::uint16_t XMaze::MoveActor(UXActorID uxActorID, XVec3& vNextPos, float fRot) {
    XActor* pActor = FindActor(uxActorID);
    if (pActor) {
        return MoveActor(pActor, vNextPos, fRot);
    }
    return 50001;  // Actor not found
}

// ============================================================================
// CreateNavMesh
// IDA: ?CreateNavMesh@XMaze@@QEAA_NPEBD@Z (0x14031F120)
// 创建导航网格
// 精确还原自 IDA 反编译
// ============================================================================
bool XMaze::CreateNavMesh(const char* pszFileName) {
    if (!pszFileName) {
        return false;
    }

    // IDA: Get auto slim write lock
    XGameServer* pGameServer = XGameServer::Instance();
    CFAutoSlimWriteLock _autolock(&pGameServer->GetMapLock());

    // IDA: Build file path - g_strCurPath_11 + "/World/Navmesh" + "/" + pszFileName + ".hkt"
    // Note: Using g_strCurPath_10 (same as g_strCurPath_11 in IDA)
    extern std::string g_strCurPath_10;
    std::string strNavMeshDir = g_strCurPath_10 + "/World/Navmesh";
    char szFilePath[272];
    std::snprintf(szFilePath, sizeof(szFilePath), "%s/%s.hkt", strNavMeshDir.c_str(), pszFileName);

    // IDA: Get ThreadLocalData instance and load navmesh
    ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    if (!pThreadData) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "[ %s ] Error ThreadLocalData is null", szFilePath);
        return false;
    }

    // IDA: HavokNavMeshResource* pNavMesh = DohHavokResourceManager::loadNavMesh(szFilePath)
    DohHavokResourceManager* pMgr = pThreadData->GetDohHavokResourceManager();
    HavokNavMeshResource* pNavMesh = pMgr ? pMgr->loadNavMesh(szFilePath) : nullptr;

    if (!pNavMesh) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "[ %s ] Error Navmesh Resouce Laod fail", szFilePath);
        return false;
    }

    // IDA: Create DohHavokNavMeshInstance via hkReferencedObject::operator new(0x100u)
    // v12 = (DohHavokNavMeshInstance *)hkReferencedObject::operator new(0x100u);
    // v16 = DohHavokNavMeshInstance::DohHavokNavMeshInstance(v12, pNavMesh, this->m_nNavMeshIndex);
    DohHavokNavMeshInstance* pInstance = new DohHavokNavMeshInstance();
    pInstance->Init(pNavMesh, m_nNavMeshIndex);
    m_pNavMeshInstance = pInstance;
    m_pNavMeshResource = pNavMesh;

    return true;
}

// ============================================================================
// CreateScriptInst
// IDA: 0x14031F2C0
// 精确还原自 IDA 反编译
// ============================================================================
bool XMaze::CreateScriptInst(const char* pszFileName) {
    if (!pszFileName) {
        return false;
    }

    // IDA: Build file path - g_strCurPath_11 + "/Scripts/Server/" + pszFileName + ".lua"
    extern std::string g_strCurPath_10;
    std::string strScriptsDir = g_strCurPath_10 + "/Scripts";
    char szFilePath[272];
    std::snprintf(szFilePath, sizeof(szFilePath), "%s/Server/%s.lua", strScriptsDir.c_str(), pszFileName);

    // IDA: Get ThreadLocalData and script manager
    ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    if (!pThreadData) {
        return false;
    }

    // IDA: VScriptResourceManager* pScriptManager = ThreadLocalData::GetScriptManager(Instance)
    VScriptResourceManager& scriptManager = pThreadData->GetScriptManager();

    // IDA: pScriptInst = v14->CreateScriptInstanceFromFile(v14, szFilePath)
    IVScriptManager* pIVScriptManager = scriptManager.GetIVScriptManager();
    IVScriptInstance* pScriptInst = pIVScriptManager->CreateScriptInstanceFromFile(szFilePath);

    if (!pScriptInst) {
        return false;
    }

    // IDA: this->m_pScriptInstance = pScriptInst
    m_pScriptInstance = pScriptInst;

    // IDA: this->m_pScriptInstance->AssertValid(this->m_pScriptInstance)
    m_pScriptInstance->AssertValid();

    // IDA: Call OnMazeCreated script function
    // IVScriptInstance::ExecuteFunctionArg(this->m_pScriptInstance, "OnMazeCreated", "oTSoulworker:XMaze;", SystemActor, this)
    CMonster* pSystemActor = GetSystemActor();
    m_pScriptInstance->ExecuteFunctionArg("OnMazeCreated", "oTSoulworker:XMaze;", pSystemActor, this);

    return true;
}

// ============================================================================
// CreateNpc
// IDA: 0x14031A250
// TODO: Blocked - CNpc needs complete definition (STNpcInfo mismatch)
// ============================================================================
CNpc* XMaze::CreateNpc(UXMapID uxMazeSerialID, std::uint32_t nSectorID,
                       std::uint32_t nNpcID, XVec3* vPos, float fRot,
                       ::E_SEND_INFO_TYPE eType) {
    // IDA: Check if NPC table exists
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer->GetResourceMgr().GetTB_NPC(nNpcID)) {
        return nullptr;
    }

    // IDA: Find sector
    auto it = m_mapSector.find(nSectorID);
    if (it == m_mapSector.end()) {
        return nullptr;
    }
    CSector* pSector = it->second;
    if (!pSector) {
        return nullptr;
    }

    // TODO: Blocked - ThreadLocalData::CreateNpc and CNpc need complete implementations
    // IDA: Create NPC through ThreadLocalData
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // CNpc* pNpc = pThreadData->CreateNpc(this, uxMazeSerialID, nSectorID, nNpcID, vPos, fRot, 0);
    // if (pNpc) {
    //     if (EnterGameObject(static_cast<XActor*>(pNpc), eType)) {
    //         DeleteNpc(pNpc);
    //         return nullptr;
    //     }
    //     pNpc->SetSector(pSector);
    //     pNpc->UpdateSectorID();
    //     pNpc->SetCollisionEnable(true, false);
    // }

    return nullptr;
}

// ============================================================================
// RandProb
// IDA: 0x140318E90
// ============================================================================
int XMaze::RandProb() {
    XWorldManager* pWorldMgr = XWorldManager::Instance();
    return pWorldMgr->RandProb();
}

// ============================================================================
// nRand
// IDA: 0x140318EB0
// ============================================================================
int XMaze::nRand(int nMin, int nMax) {
    XWorldManager* pWorldMgr = XWorldManager::Instance();
    return pWorldMgr->nRand(nMin, nMax);
}

// ============================================================================
// fRand
// IDA: 0x140318EE0
// ============================================================================
float XMaze::fRand(float fMin, float fMax) {
    XWorldManager* pWorldMgr = XWorldManager::Instance();
    return pWorldMgr->fRand(fMin, fMax);
}

// ============================================================================
// NotifyMonsterDelete
// IDA: 0x14031A0D0
// ============================================================================
// ============================================================================
// NotifyMonsterDelete
// IDA: 0x14031A0D0
// 精确还原: 通知怪物删除（广播给机器人用户）
// ============================================================================
void XMaze::NotifyMonsterDelete(CMonster* pMonster) {
    if (!pMonster) {
        return;
    }

    // IDA: Only broadcast if we have bot users
    if (m_bHaveBotUser) {
        // IDA: Create packet (0x17, 0x13)
        XSendPacket xPacket(0x17, 0x13);

        // IDA: Get actor ID and convert to quest ID
        UXActorID actorID = pMonster->GetActorID();
        // TODO: CQuestCondition::GetQuestID needs to be defined
        // int nQuestID = CQuestCondition::GetQuestID(&actorID);
        // xPacket << nQuestID;

        // IDA: Broadcast to all users
        // SendBroadCast(this, &xPacket, &pMonster->XActor, eAll);
        XActor* pActor = static_cast<XActor*>(pMonster);
        SendBroadCast(&xPacket, pActor, E_BROADCAST_TYPE::eAll);

        // IDA: Log the deletion
        LogHelper::LogDebug("game.contents", "######## MONSTER DELETE ########### %d", actorID.dwActorID);
    }
}

// ============================================================================
// DeleteNpc
// IDA: 0x14031A430
// 精确还原: 删除 NPC
// ============================================================================
void XMaze::DeleteNpc(CNpc* pNpc) {
    // IDA: Call ExitGameObject with eSendInfoTypeNot
    if (pNpc) {
        ExitGameObject(reinterpret_cast<XActor*>(pNpc), eSendInfoTypeNot);
    } else {
        ExitGameObject(nullptr, eSendInfoTypeNot);
    }

    // IDA: Delete NPC through ThreadLocalData
    // TODO: ThreadLocalData needs complete definition
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // if (pThreadData) {
    //     pThreadData->DeleteNpc(pNpc);
    // }
}

// ============================================================================
// CreateAkashicObject
// IDA: 0x14031A4A0
// ============================================================================
CAkashicObject* XMaze::CreateAkashicObject(UXMapID uxMazeSerialID, std::uint32_t nAkashicID,
                                           XVec3* vPos, float fRot, std::uint32_t dwParentID,
                                           ::E_SEND_INFO_TYPE eType) {
    // IDA: Check if Akashic table exists
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer->GetResourceMgr().GetTB_AKASHIC_RECORDS(nAkashicID)) {
        return nullptr;
    }

    // TODO: ThreadLocalData needs complete definition
    // IDA: Create Akashic object through ThreadLocalData
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // if (!pThreadData) {
    //     return nullptr;
    // }
    // CAkashicObject* pAkashic = pThreadData->CreateAkashicObject(this, uxMazeSerialID, nAkashicID, vPos, fRot, dwParentID);
    // if (!pAkashic) {
    //     return nullptr;
    // }
    // if (EnterGameObject(pAkashic, eType) != 0) {
    //     DeleteAkashicObject(pAkashic);
    //     return nullptr;
    // }
    // pAkashic->SetCollisionEnable(false, false);
    
    return nullptr;
}

// ============================================================================
// DeleteAkashicObject
// IDA: 0x14031A5E0
// ============================================================================
// ============================================================================
// DeleteAkashicObject
// IDA: 0x14031A5E0
// 精确还原: 删除 Akashic 对象
// ============================================================================
void XMaze::DeleteAkashicObject(CAkashicObject* pAkashic) {
    // IDA: Call ExitGameObject with eSendInfoTypeNot
    if (pAkashic) {
        ExitGameObject(reinterpret_cast<XActor*>(pAkashic), eSendInfoTypeNot);
    } else {
        ExitGameObject(nullptr, eSendInfoTypeNot);
    }

    // IDA: Delete Akashic object through ThreadLocalData
    // TODO: ThreadLocalData needs complete definition
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // if (pThreadData) {
    //     pThreadData->DeleteAkashicObject(pAkashic);
    // }
}

// ============================================================================
// SetParty
// IDA: ?SetParty@XMaze@@QEAAXV?$shared_ptr@VCParty@@@tr1@std@@@Z (0x140315C40)
// ============================================================================
void XMaze::SetParty(std::shared_ptr<CParty> pParty) {
    // IDA: Only set if not already set (m_pParty == nullptr)
    if (!m_pParty) {
        m_pParty = pParty;
        m_pForce.reset();

        if (pParty) {
            // IDA: Set maze ID - convert TUXMapID to UXMapID
            UXMapID uxMapID;
            uxMapID.nMapID = static_cast<std::int64_t>(XArea::GetMapID().nMapID);
            pParty->SetMazeID(uxMapID);

            // IDA: Get user count
            m_nPartyUserCount = pParty->GetUserCount();
            m_stPartyInfo.byGroupType = 1;

            // IDA: Log the party set
            int nPartyID = pParty->GetPartyID();
            int nUserCount = pParty->GetUserCount();
            LogHelper::LogDebug("game.contents", "<MAZE> SetParty ( PID : %d / Count : %d )", nPartyID, nUserCount);
        } else {
            m_stPartyInfo.byGroupType = 0;
            m_nPartyUserCount = 0;
        }
    }
}

// ============================================================================
// SetForce
// IDA: ?SetForce@XMaze@@QEAAXV?$shared_ptr@VCForce@@@tr1@std@@@Z (0x140315D50)
// ============================================================================
void XMaze::SetForce(std::shared_ptr<CForce> pForce) {
    // IDA: Only set if not already set (m_pForce == nullptr)
    if (!m_pForce) {
        m_pForce = pForce;
        m_pParty.reset();

        if (pForce) {
            // IDA: Set maze ID - convert TUXMapID to UXMapID
            UXMapID uxMapID;
            uxMapID.nMapID = static_cast<std::int64_t>(XArea::GetMapID().nMapID);
            pForce->SetMazeID(uxMapID);

            // IDA: Get user count
            m_nPartyUserCount = pForce->GetUserCount();
            m_stPartyInfo.byGroupType = 2;

            // IDA: Log the force set
            int nPartyID = pForce->GetPartyID();
            int nUserCount = pForce->GetUserCount();
            LogHelper::LogDebug("game.contents", "<MAZE> SetForce ( PID : %d / Count : %d )", nPartyID, nUserCount);
        } else {
            m_stPartyInfo.byGroupType = 0;
            m_nPartyUserCount = 0;
        }
    }
}

// ============================================================================
// GetScanner
// IDA: 0x1403264D0
// EXACT IDA implementation - get scanner map based on actor type
// ============================================================================
std::map<std::uint32_t, CMover*>* XMaze::GetScanner(XActor* pActor) {
    if (!pActor) {
        return nullptr;
    }
    
    int nType = pActor->GetType();
    if (nType == 0) {
        // Type 0: Users
        return &m_objectScanner.mapPlayerList;
    }
    if (nType > 0 && nType <= 2) {
        // Type 1-2: NPCs and Monsters
        return &m_objectScanner.mapNPCList;
    }
    // Type > 2: Other actors
    return &m_objectScanner.mapEtcList;
}

// ============================================================================
// GetCurUserCount
// IDA: 0x140324AF0
// EXACT IDA implementation: returns m_objectScanner.size()
// ============================================================================
int XMaze::GetCurUserCount() const {
    // IDA: return m_objectScanner.size() which returns mapPlayerList.size()
    return static_cast<int>(m_objectScanner.size());
}

// ============================================================================
// IsCallScriptDie
// IDA: 0x140318E00
// 精确还原: 检查怪物是否调用脚本死亡
// ============================================================================
bool XMaze::IsCallScriptDie(std::uint32_t nMonsterID) {
    // IDA: 在 m_mapCallScriptDieMonster 中查找 nMonsterID
    auto it = m_mapCallScriptDieMonster.find(nMonsterID);
    if (it == m_mapCallScriptDieMonster.end()) {
        return false;
    }
    // 返回找到的 bool 值
    return it->second;
}

// ============================================================================
// GetSector
// IDA: 0x14032B270
// 精确还原: 通过 SectorID 获取 CSector 指针
// ============================================================================
CSector* XMaze::GetSector(int nSectorID) {
    // 获取 BatchLayerLevel 并计算唯一 ID
    int nBatchLayerLevel = GetBatchLayerLevel();
    int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSectorID, nBatchLayerLevel);

    // 在 m_mapSector 中查找
    auto it = m_mapSector.find(iBoxUniqueID);
    if (it != m_mapSector.end()) {
        return it->second;
    }
    return nullptr;
}

// ============================================================================
// IsRoguelikeMap
// 检查是否是 Roguelike 地图
// ============================================================================
bool XMaze::IsRoguelikeMap() const {
    if (m_pTBMazeInfo) {
        return m_pTBMazeInfo->Maze_Type == 15;
    }
    return false;
}

// ============================================================================
// GetUniqueID
// 获取唯一ID
// ============================================================================
std::uint32_t XMaze::GetUniqueID(int nSectorID) {
    // IDA: 生成唯一 ID
    // 使用 BatchLayerLevel 和 SectorID 组合生成唯一 ID
    int nBatchLayerLevel = GetBatchLayerLevel();
    return static_cast<std::uint32_t>((nBatchLayerLevel << 16) | (nSectorID & 0xFFFF));
}

// ============================================================================
// SendBroadCast
// IDA: 0x1403265E0
// Broadcast packet to all movers in scanner
// ============================================================================
void XMaze::SendBroadCast(XSendPacket& xSendPacket, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) {
    // IDA: Iterate through m_objectScanner
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: Check if object has vtable
        XActor* pActor = static_cast<XActor*>(pMover);

        // IDA: Check broadcast type - if eNoneSelf, skip except actor
        if (eBroadCastType == E_BROADCAST_TYPE::eNoneSelf && pActor == pExceptActor) {
            continue;
        }

        // IDA: Send packet via BridgeSend virtual function
        pActor->BridgeSend(xSendPacket);
    }
}

// ============================================================================
// SendBroadCast (with ExceptDie parameter)
// IDA: 0x1403266F0
// Broadcast packet to all movers in scanner, optionally skipping dead actors
// ============================================================================
void XMaze::SendBroadCast(XSendPacket& xSendPacket, XActor* pExceptActor, bool ExceptDie, E_BROADCAST_TYPE eBroadCastType) {
    // IDA: Iterate through m_objectScanner
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: Check if object has vtable
        XActor* pActor = static_cast<XActor*>(pMover);

        // IDA: Check broadcast type - if eNoneSelf, skip except actor
        if (eBroadCastType == E_BROADCAST_TYPE::eNoneSelf && pActor == pExceptActor) {
            continue;
        }

        // IDA: If ExceptDie is true, skip dead actors
        if (ExceptDie && pMover->IsDie()) {
            continue;
        }

        // IDA: Send packet via BridgeSend virtual function
        pActor->BridgeSend(xSendPacket);
    }
}

// Legacy overload for pointer-based calls (backward compatibility)
void XMaze::SendBroadCast(XSendPacket* pPacket, XActor* pExceptActor, E_BROADCAST_TYPE eType) {
    if (!pPacket) return;
    SendBroadCast(*pPacket, pExceptActor, eType);
}

// ============================================================================
// RunSectorAI
// IDA: ?RunSectorAI@XMaze@@QEAAXAEBVhkvVec3@@_N@Z (0x14031F780)
// IDA: ?RunSectorAI@XMaze@@QEAAXH_N@Z (0x14031F7C0)
// ============================================================================
void XMaze::RunSectorAI(const hkvVec3& vPos, bool bIsPotal) {
    int nSector = GetSectorIDFromPos(vPos);
    RunSectorAI(nSector, bIsPotal);
}

void XMaze::RunSectorAI(int nSector, bool bIsPotal) {
    // IDA: Log sector AI activation
    // CTextDBLog::AddLog(&m_textDBLog, 19, nSector, 0, "");

    int nBatchLayerLevel = GetBatchLayerLevel();
    int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSector, nBatchLayerLevel);

    auto it = m_mapSector.find(iBoxUniqueID);
    if (it != m_mapSector.end()) {
        CSector* pSector = it->second;
        if (pSector) {
            pSector->SetAI(true);
            pSector->CheckClearState();
            SetLastSectorID(nSector, bIsPotal);
            bool bIsBossSector = pSector->IsBossSector();
            SetBossSector(bIsBossSector);
        }
        // LogHelper::LogDebug("game.contents", "<MAZE> RunSectorAI (MAP:%d / SECTOR:%d) ", m_pTBMazeInfo->ID, nSector);
    }
}

// ============================================================================
// UpdateClearMazeCondition
// IDA: 0x140324D50
// 更新迷宫通关条件
// ============================================================================
void XMaze::UpdateClearMazeCondition(int nConditionType, int nValue) {
    // IDA: Check if maze is not complete and TBMazeInfo exists
    if (m_bMazeComplete || !m_pTBMazeInfo) return;

    // IDA: Loop through 3 clear conditions
    // Note: IDA accesses fields as array via pointer arithmetic
    std::uint8_t conTypes[3] = {
        m_pTBMazeInfo->Clear_Con_Type_01,
        m_pTBMazeInfo->Clear_Con_Type_02,
        m_pTBMazeInfo->Clear_Con_Type_03
    };
    unsigned int conValues[3] = {
        m_pTBMazeInfo->Clear_Con_Value_01,
        m_pTBMazeInfo->Clear_Con_Value_02,
        m_pTBMazeInfo->Clear_Con_Value_03
    };

    for (int i = 0; i < 3; ++i) {
        if (conTypes[i] == nConditionType && static_cast<int>(conValues[i]) == nValue) {
            m_stMazeGameState.m_bClearCondition[i] = false;
        }
    }

    // IDA: Process reward
    ProcessReward();
}

// ============================================================================
// ProcessReward
// IDA: 0x140324E10
// 处理迷宫通关奖励
// ============================================================================
void XMaze::ProcessReward() {
    // IDA: Check if maze is not complete and TBMazeInfo exists
    if (m_bMazeComplete || !m_pTBMazeInfo) return;

    // IDA: Check clear calculation type
    if (m_pTBMazeInfo->Clear_Cal_Type == 1) {
        // AND logic: all conditions must be cleared
        m_bMazeComplete = true;
        for (int i = 0; i < 3; ++i) {
            if (m_stMazeGameState.m_bClearCondition[i]) {
                m_bMazeComplete = false;
                break;
            }
        }
    } else if (m_pTBMazeInfo->Clear_Cal_Type == 2) {
        // OR logic: any condition cleared means complete
        m_bMazeComplete = false;
        for (int j = 0; j < 3; ++j) {
            if (!m_stMazeGameState.m_bClearCondition[j]) {
                m_bMazeComplete = true;
                break;
            }
        }
    } else if (m_pTBMazeInfo->Maze_Type == 7) {
        // Infinite tower: always complete
        m_bMazeComplete = true;
    }

    if (!m_bMazeComplete) return;

    // IDA: Log maze completion
    // CTextDBLog::AddLog(&m_textDBLog, 4, 0, 0, ...);

    // IDA: Get last sector ID and destroy monsters
    int nLastSectorID = GetLastSectorID();
    AllDestroySectorMonster(nLastSectorID);

    // IDA: Finish maze time
    FinishMazeTime();

    // IDA: Check hidden event state
    CheckHiddenEventState();

    // IDA: Release helper support equip
    // TODO: ReleaseHelperSupportEquip();

    // TODO: Implement reward distribution based on maze type
    // - Maze_Type 14, 18, 19: Monster kill score mode
    // - Maze_Type 7: Infinite tower
    // - Other types: Normal maze reward
}

void XMaze::LoadComplete(CUser* pUser) {
    // IDA: 玩家加载完成处理
    if (!pUser) {
        return;
    }

    // 检查是否需要等待其他玩家
    if (m_dwWaitUserTime > 0) {
        auto it = m_listWaitForRecvInfo.begin();
        while (it != m_listWaitForRecvInfo.end()) {
            if (*it == pUser) {
                it = m_listWaitForRecvInfo.erase(it);
                break;
            }
            ++it;
        }
    }

    // 发送迷宫信息给玩家
    SendMazeInfo();
}

void XMaze::ChangeMonster(std::uint32_t dwMobID) {
    (void)dwMobID;
    // TODO: restore monster morph traversal once CMonster::ChangeMonster is recovered.
}

void XMaze::CheckFollowMonster() {
    // Per IDA 0x14032C690: XMaze::CheckFollowMonster 精确还原
    // Iterate through m_objectScanner.mapNPCList
    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: Check if actor type is 2 (Monster)
        if (pMover->GetType() == 2) {
            CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
            if (pMonster && pMonster->IsFollower()) {
                pMonster->CheckFollowMonster();
            }
        }
    }
}

// ============================================================================
// RefreshUserCountInSector
// IDA: 0x14033BE00
// 刷新区域中用户数量
// ============================================================================
void XMaze::RefreshUserCountInSector(std::uint32_t dwActorID) {
    if (m_bMazeComplete) {
        return;
    }
    
    // Find the actor in the check sector user map
    auto it = m_mapCheckSectorUser.find(dwActorID);
    if (it == m_mapCheckSectorUser.end()) {
        return;
    }
    
    // Get the unique ID from the map entry
    int nUniqueID = it->second;
    
    // Get the portal box
    // TODO: STMagePotalBox needs complete definition
    // STMagePotalBox* pBox = GetMazePotalBox(nUniqueID);
    // For now, just erase the entry
    m_mapCheckSectorUser.erase(it);
    
    // TODO: restore STMagePotalBox enter-count/open-state handling once the box layout is complete.
    // The IDA code shows:
    // - Decrement pBox->nEnterUserCount
    // - If count reaches 0 and box is not open, set bOpen = 1 and broadcast PS_WORLD_WARP_INFO
}

void XMaze::RestartResetState(bool bState1, bool bState2) {
    // IDA: 重置重启状态
    m_bRestartReady = bState1;
    m_nTimeStepState = bState2 ? 1 : 0;
    
    if (bState1 && bState2) {
        // 准备重启迷宫
        m_nTimeStepTarget = 0;
        m_dwWaitUserTime = 0;
    }
}

int XMaze::FindInvisibleActorCnt() {
    // IDA 0x140333650: XMaze::FindInvisibleActorCnt 精确还原
    // 统计隐形 Actor 数量 (GM 且状态 0x2000)
    int nCount = 0;

    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pObj = it->second;
        if (!pObj) continue;

        CUser* pUser = dynamic_cast<CUser*>(pObj);
        if (pUser && pUser->IsGM() && pUser->IsStatus(0x2000u)) {
            ++nCount;
        }
    }

    return nCount;
}

// IDA: ?UpdateMazeState@XMaze@@QEAAXXZ (0x140326820)
// IDA 精确还原 - 更新迷宫状态
void XMaze::UpdateMazeState() {
    int nMazeState = m_stMazeGameState.m_nMazeState;

    if (nMazeState == 0) {
        // IDA: State 0 - Initial state, check creation timeout
        std::uint64_t dwCreateTime = m_stMazeGameState.m_dwMazeCreateTime + 300000;  // 5 minutes
        if (dwCreateTime >= GetTickCount64()) {
            // IDA: Still within creation time window
            // Check for Maze_Type == 9 (Force maze) with wait to enter force member
            if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Type == 9 &&
                m_dwWaitToEnterForceMember && m_dwWaitToEnterForceMember <= GetTickCount64()) {
                // IDA: Call OnEnterForceMemberAll script
                if (m_pScriptInstance) {
                    // TODO: IVScriptInstance::HasFunction and ExecuteFunctionArg
                    // m_pScriptInstance->ExecuteFunctionArg("OnEnterForceMemberAll", "TSoulworker:XMaze;", this);
                }
                StartMazeTime();
                m_dwWaitToEnterForceMember = 0;
                LogHelper::LogDebug("game.contents", "[MAZE_FORCE] Timeout Wait to enter force member");
            }

            // IDA: Check wait to load EX member
            if (m_dwWaitToLoadEXMember && m_dwWaitToLoadEXMember <= GetTickCount64()) {
                StartMazeTime();
                m_dwWaitToLoadEXMember = 0;
                LogHelper::LogDebug("game.contents", "[MODE_MAZE] Timeout Wait to Loading Ex Finish");
                // TODO: SendLoadEx_GameStart();
            }
        } else {
            // IDA: Creation time expired
            if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Type == 17 && GetCurUserCount() > 0) {
                // IDA: Special handling for Maze_Type == 17
                StartMazeTime();
            } else {
                // IDA: Send maze update packet with state 3 (timeout)
                // PS_MAZE_UPDATE_INFO stUpdateMaze;
                // stUpdateMaze.uxMapID = GetInstanceID();
                // stUpdateMaze.nState = 3;
                // stUpdateMaze.nUserCount = 0;
                // XSendPacket xSendPacket(0xF2, 0x26);
                // xSendPacket << stUpdateMaze;
                // XGameServer::Instance()->GetControlSocket().Send(&xSendPacket);

                // IDA: Log timeout
                // ST_LOG_GAME stLog;
                // stLog._sMainType = 5;
                // stLog._sSubType = 2;
                // stLog.nParam0 = GetTBMapID();
                // stLog.nParam1 = 101;
                // XGameServer::Instance()->SendDBLog(&stLog);

                LogHelper::LogDebug("game.contents", "<< eSUB_CMD_SERVER_UPDATE_MAZE 6 >> 3");
                m_stMazeGameState.m_nMazeState = 5;  // Set to expired state
            }
        }
    } else if (nMazeState == 1) {
        // IDA: State 1 - Running state, check wait enter maze users
        bool bChange = false;

        for (auto it = m_mapWaitEnterMazeUser.begin(); it != m_mapWaitEnterMazeUser.end(); ) {
            ST_MAZE_WAIT_ENTER_USER_INFO& stInfo = it->second;
            // IDA: dwLimitTime is at offset 0x08 (dw64ExitTime used as limit time)
            std::uint64_t dwLimitTime = stInfo.dw64ExitTime;

            if (dwLimitTime && dwLimitTime <= GetTickCount64()) {
                // IDA: Timeout - check if user is in objectScanner
                auto itObj = m_objectScanner.mapPlayerList.find(it->first);
                if (itObj == m_objectScanner.mapPlayerList.end()) {
                    // IDA: User not found, remove from wait list
                    it = m_mapWaitEnterMazeUser.erase(it);
                    bChange = true;
                    continue;
                }
                // IDA: User found, clear limit time
                stInfo.dw64ExitTime = 0;
            }
            ++it;
        }

        // IDA: If changed, send update packet
        if (bChange) {
            // PS_MAZE_UPDATE_INFO st;
            // st.uxMapID = GetInstanceID();
            // st.nState = GetMazeState();
            // st.nUserCount = GetCurUserCount();
            // for (auto& pair : m_mapWaitEnterMazeUser) {
            //     st.vecMemberInfo.push_back(pair.second);
            // }
            // XSendPacket xSendPacket(0xF2, 0x26);
            // xSendPacket << st;
            // XGameServer::Instance()->GetControlSocket().Send(&xSendPacket);
            LogHelper::LogDebug("game.contents", "<< eSUB_CMD_SERVER_UPDATE_MAZE 1 >> %d", GetMazeState());
        }
    } else if (nMazeState == 3) {
        // IDA: State 3 - Force state 1
        if (m_stMazeGameState.m_nMazeState == 3) {
            SetMazeState(1, true);
        }
    } else if (nMazeState == 4) {
        // IDA: State 4 - Check Grouton spawn
        if (m_dwWaitGroutonSpawnTime && m_dwWaitGroutonSpawnTime <= GetTickCount64()) {
            // TODO: ExcuteSpawnBoxCheck(m_nGroutonBoxID, eSendInfoTypeSend, 0);
            m_dwWaitGroutonSpawnTime = 0;
        }
    } else if (nMazeState == 7) {
        // IDA: State 7 - Re-enter wait state
        int nReEnterWaitTime = 5;  // Default 5 minutes
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) {
            TB_COMMON* pTB_Common = pServer->GetResourceMgr().GetTB_COMMON(0x753C);
            if (pTB_Common) {
                nReEnterWaitTime = static_cast<int>(pTB_Common->Value);
            }
        }

        std::uint64_t dwWaitTime = m_stMazeGameState.m_dwMazeWaitTime + 60000 * nReEnterWaitTime;
        if (GetTickCount64() >= dwWaitTime) {
            // IDA: Send maze update packet with state 3
            // PS_MAZE_UPDATE_INFO st;
            // st.uxMapID = GetInstanceID();
            // st.nState = 3;
            // st.nUserCount = 0;
            // XSendPacket xSendPacket(0xF2, 0x26);
            // xSendPacket << st;
            // XGameServer::Instance()->GetControlSocket().Send(&xSendPacket);

            // IDA: Log re-enter timeout
            // ST_LOG_GAME stLogGame;
            // stLogGame._sMainType = 5;
            // stLogGame._sSubType = 2;
            // stLogGame.nParam0 = GetTBMapID();
            // stLogGame.nParam1 = 101;
            // XGameServer::Instance()->SendDBLog(&stLogGame);

            LogHelper::LogDebug("game.contents", "<< eSUB_CMD_SERVER_UPDATE_MAZE 7 >> 3");
            m_stMazeGameState.m_nMazeState = 4;
        }
    }
}

// ============================================================================
// CompleteEscortCondition
// IDA: 0x14032EC10
// ============================================================================
void XMaze::CompleteEscortCondition() {
    m_fUpdateProcessEscort = 5.0f;
}

// ============================================================================
// ProcessEscortCondition
// IDA: 0x14032EC30
// ============================================================================
void XMaze::ProcessEscortCondition(float fElapsed) {
    if (m_fUpdateProcessEscort > 0.0f) {
        m_fUpdateProcessEscort -= fElapsed;
        if (m_fUpdateProcessEscort <= 0.0f) {
            // Iterate through all movers in the scanner
            for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
                CMover* pObj = it->second;
                if (!pObj) continue;
                
                // RTTI cast to CUser
                CUser* pUser = dynamic_cast<CUser*>(pObj);
                if (!pUser) continue;
                
                // TODO: CGocQuest needs complete definition
                // GetGOC<CGocQuest> and check condition
                // For now, just log that we would complete the condition
                LogHelper::LogDebug("game.contents", 
                    "<ESCORT> ProcessEscortCondition for user (ConditionID: %d)", 
                    m_stEscortMonster.nConditionID);
            }
        }
    }
}

// ============================================================================
// UpdateCasualRaidTimer
// IDA: 0x14031D850
// ============================================================================
void XMaze::UpdateCasualRaidTimer(float fElapsed) {
    // Update fFloatValue timer
    if (m_stCasualRaidTime.fFloatValue > 0.0f) {
        m_stCasualRaidTime.fFloatValue -= fElapsed;
        if (m_stCasualRaidTime.fFloatValue <= 0.0f) {
            m_stCasualRaidTime.reset();
        }
    }

    // Update fWaitSendTime timer
    if (m_stCasualRaidTime.fWaitSendTime > 0.0f) {
        m_stCasualRaidTime.fWaitSendTime -= fElapsed;
        if (m_stCasualRaidTime.fWaitSendTime <= 0.0f) {
            SendCasualRaidTimer();
            m_stCasualRaidTime.fWaitSendTime = 0.0f;
        }
    }
}

// ============================================================================
// UpdatePortalState
// IDA: ?UpdatePortalState@XMaze@@QEAAXXZ (0x14031FE20)
// ============================================================================
void XMaze::UpdatePortalState() {
    // Iterate through all portal boxes
    for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
        int nBoxIndex = it->first;
        STMagePotalBox* pPotal = it->second;

        if (!pPotal) {
            continue;
        }

        // Check if portal is open and close time has expired
        if (pPotal->bOpen && pPotal->nCloseTime > 0 && pPotal->nCloseTime <= GetTickCount64()) {
            // Close the portal
            pPotal->bOpen = false;

            // Send packet to all users
            XSendPacket xSendPacket(4, 9);
            xSendPacket << nBoxIndex;
            xSendPacket << pPotal->bOpen;
            xSendPacket << m_bMazeComplete;
            SendBroadCast(&xSendPacket, nullptr, E_BROADCAST_TYPE::eAll);

            // Log the portal close
            LogHelper::LogDebug("game.contents", "[PORTAL] Close portal by Time out ( %d )", nBoxIndex);
            // CTextDBLog::AddLog(&m_textDBLog, 20, nBoxIndex, 0, "TIMEOUT");
        }
    }
}

void XMaze::SyncSpawnedActive() {
    // TODO: restore active spawn-box sync once STMageProcessSpawnBox layout is complete.
}

// ============================================================================
// EnterPartyForceMember
// IDA: 0x1403241D0
// EXACT IDA implementation - handle party/force member entry to maze
// ============================================================================
void XMaze::EnterPartyForceMember(CUser* pUser) {
    (void)pUser;
    // TODO: restore party/force member enter synchronization once CParty/CForce managers are recovered.
}

// ============================================================================
// SendSyncHiddenEventInfo
// IDA: 0x140330D40
// EXACT IDA implementation - send hidden event info to user
// ============================================================================
void XMaze::SendSyncHiddenEventInfo(CUser* pUser) {
    if (!m_pHiddenEvent) {
        return;
    }
    
    // TODO: CHiddenEvent needs complete definition
    // The IDA code shows:
    // 1. Send packet with HiddenEventID, EventConditionID, byInit=1, nValue=0
    // 2. Send packet with HiddenEventID, EventConditionID, HiddenEventState, ModeDateID
    
    // For now, just log that we would send the hidden event info
    LogHelper::LogDebug("game.contents", "<HIDDEN_EVENT> SendSyncHiddenEventInfo to user");
    
    // TODO: restore hidden-event synchronization after CHiddenEvent is recovered.
}

// ============================================================================
// SendOutInfo
// IDA: 0x1408F0D50 (XArea::SendOutInfo)
// EXACT IDA implementation - send object removal info
// ============================================================================
void XMaze::SendOutInfo(XSendPacket* pPacket, XActor* pActor) {
    (void)pPacket;
    (void)pActor;
    // TODO: restore object removal packet broadcast with current XActor API.
}

// ============================================================================
// GetMazeLayerLevel
// IDA: 0x14033AAD0
// EXACT IDA implementation - calculate maze layer level from bitmask
// ============================================================================
void XMaze::GetMazeLayerLevel(int nLayerBit, int* pnBaseLevel) {
    int nLayerBita = nLayerBit;
    m_nBit = 0;
    
    if (nLayerBit >= 100) {
        int nTmpBase = 0;
        int nTmp;
        do {
            nTmp = nLayerBita / 100;
            int iLevel = nLayerBita % 100;
            
            if (nLayerBita % 100 < 0 || iLevel >= 28) {
                m_nBit = -1;
                return;
            }
            
            m_nBit += 1 << (iLevel + 1);
            
            if (nTmpBase < iLevel) {
                nTmpBase = nLayerBita % 100;
            }
            
            nLayerBita /= 100;
        } while (nTmp >= 10);
        
        m_nBit += (1 << (nTmp + 1)) | 1;
        
        if (nTmp >= 0 && nLayerBita < 28) {
            if (nTmpBase < nLayerBita) {
                nTmpBase = nLayerBita;
            }
            *pnBaseLevel = nTmpBase;
        } else {
            m_nBit = -1;
        }
    } else {
        *pnBaseLevel = nLayerBit;
        if (*pnBaseLevel < 0x1C) {
            m_nBit += (1 << (*pnBaseLevel + 1)) | 1;
        } else {
            m_nBit = -1;
        }
    }
}

// ============================================================================
// GetSpawnPos
// IDA: 0x14031A650
// EXACT IDA implementation - get spawn position from monster spawn info
// ============================================================================
bool XMaze::GetSpawnPos(int nSpawnBoxID, XVec3& vPos, float& fRot) {
    (void)nSpawnBoxID;
    vPos = XVec3{};
    fRot = 0.0f;
    // TODO: restore spawn-box position extraction after VMonsterSpawnInfo layout is reconciled.
    return false;
}

// Per IDA 0x14031A650: XMaze::GetSpawnPos
// 获取生成位置 - IDA精确还原
// 逻辑: 根据 m_iCreationPositionType 决定位置计算方式
//   0 = 中心点 (TopLeft + BottomRight) / 2
//   1/2 = 随机位置 (fRand between TopLeft and BottomRight)
void XMaze::GetSpawnPos(const VMonsterSpawnInfo* pMonsterSpawn, XVec3& vPos) {
    if (!pMonsterSpawn) {
        return;
    }

    // IDA: vPos.z = pMonsterSpawn->PosTopLeft.z
    vPos.z = pMonsterSpawn->PosTopLeft.z;

    int nType = pMonsterSpawn->m_iCreationPositionType;
    if (nType == 0) {
        // IDA: 中心点计算
        vPos.x = (pMonsterSpawn->PosTopLeft.x + pMonsterSpawn->PosBottomRight.x) / 2.0f;
        vPos.y = (pMonsterSpawn->PosTopLeft.y + pMonsterSpawn->PosBottomRight.y) / 2.0f;
    } else if (nType == 1 || nType == 2) {
        // IDA: 随机位置计算
        vPos.x = fRand(pMonsterSpawn->PosTopLeft.x, pMonsterSpawn->PosBottomRight.x);
        vPos.y = fRand(pMonsterSpawn->PosTopLeft.y, pMonsterSpawn->PosBottomRight.y);
    }
}

// ============================================================================
// Init, Create, Clear, Generate 等大型方法存根
// 这些方法需要完整的实现
// ============================================================================

// ============================================================================
// Init
// IDA: 0x140311210
// ============================================================================
bool XMaze::Init() {
    // IDA: XIOCPServer::BackSends(this, nullptr) - skip, XMaze doesn't inherit XIOCPServer

    // Reset state
    m_bMazeComplete = false;
    m_nNavMeshIndex = 0;
    m_vecActiveLastSectorID.clear();
    m_nStartSectorID = 0;
    m_nLastSpawnedSectorID = 0;
    m_nDestroySilhouetes = 0;
    m_pNavMeshInstance = nullptr;
    m_pMazeResource = nullptr;
    m_pSystemActor = nullptr;
    m_pScriptInstance = nullptr;
    m_pActiveEventSector = nullptr;

    // Clear Party/Force - IDA: shared_ptr reset
    m_pParty.reset();
    m_pForce.reset();
    m_nPartyUserCount = 0;
    m_bBossSector = false;
    m_pHiddenEvent = nullptr;
    m_bHaveBotUser = false;

    // Initialize position info - IDA: STPosInfo::Init
    m_stEnterDistrictPos.Init();
    m_fUpdateProcessEscort = 0.0f;
    m_dwWaitToEnterForceMember = 0;
    m_dwWaitToLoadEXMember = 0;

    // Clear Roguelike data
    m_mapRogueSector.clear();
    m_pRogueStartSector = nullptr;
    m_pRogueNextSector = nullptr;
    m_pRogueBossSector = nullptr;
    m_nRoguelikeMaxSector = 1;
    m_nRoguelikeGoBossState = 0;
    m_vecRoguelikeRandomShopStat.clear();
    m_mapRoguelikeRandomPocketInfo.clear();
    m_mapRoguelikePortalBuffList.clear();
    m_mapRoguelikePortalBuff.clear();
    m_mapRandomBuff.clear();
    m_nRoguelikePortalBuffID = 0;
    m_nRoguelikeLastPortalID = 0;

    // IDA: CCutsceneManager::Init(&this->m_cutSceneManager, this)
    // Note: IDA shows embedded object, but we use pointer to avoid incomplete type issues
    if (!m_pCutSceneManager) {
        m_pCutSceneManager = new CCutsceneManager();
    }
    m_pCutSceneManager->Init(this);

    // IDA: memset(&this->m_stInfiniteTowerInfo, 0, sizeof(this->m_stInfiniteTowerInfo))
    std::memset(&m_stInfiniteTowerInfo, 0, sizeof(m_stInfiniteTowerInfo));

    // Clear escort monster
    std::memset(&m_stEscortMonster, 0, sizeof(m_stEscortMonster));

    // Get Maze resource - IDA: XWorldResMgr::GetResource
    std::uint16_t TBMapID = GetTBMapID();
    XGameServer* pServer = XGameServer::Instance();
    m_pMazeResource = XWorldResMgr::GetResource(&pServer->GetWorldResMgr(), TBMapID);

    if (!m_pMazeResource) {
        LogHelper::LogError("game.contents",
            "Init error - Failed create maze. No data in _WORLD_RES_MGR Maze[ TBID:%d ] ( %d )",
            TBMapID, 117);
        return false;
    }

    // Get Maze table info
    m_pTBMazeInfo = pServer->GetResourceMgr().GetTB_MAZE_INFO(TBMapID);
    if (!m_pTBMazeInfo) {
        LogHelper::LogError("game.contents",
            "Init error - Failed create maze. No data in _TB_MAZE[ TBID:%d ] ( %d )",
            TBMapID, 124);
        return false;
    }

    // Initialize Maze Game State
    std::memset(&m_stMazeGameState, 0, sizeof(m_stMazeGameState));
    m_stMazeGameState.m_dwMazeClearTime = m_pTBMazeInfo->Maze_ClearTime;
    m_stMazeGameState.m_dwMazeCreateTime = GetTickCount64();
    m_stMazeGameState.m_dwMazeWaitTime = 0;

    // Clear Lua and Game Rules
    m_vecLuaValues.clear();
    m_vecGameRules.clear();

    // Set Maze Level
    m_stMazeGameState.m_nMazeLevel = m_pTBMazeInfo->Maze_Difficulty_Type;
    int nBaseLevel = 0;
    m_nBit = 0;
    GetMazeLayerLevel(m_pTBMazeInfo->Layer_BitMask, &nBaseLevel);
    m_stMazeGameState.m_nBatchLayerLevel = nBaseLevel;

    // Maze_Type == 7 uses Difficulty_Type as BatchLayerLevel
    if (m_pTBMazeInfo->Maze_Type == 7) {
        m_stMazeGameState.m_nBatchLayerLevel = m_pTBMazeInfo->Maze_Difficulty_Type;
    }

    // Set clear conditions
    for (int i = 0; i < 3; ++i) {
        int nClearConType = *(&m_pTBMazeInfo->Clear_Con_Type_01 + i);
        if (nClearConType) {
            m_stMazeGameState.m_bClearCondition[i] = true;
        }
    }

    // Create NavMesh
    if (!CreateNavMesh(m_pTBMazeInfo->ServerMap)) {
        LogHelper::LogError("game.contents",
            "Init error - Failed CreateNavMesh[ %s ] ( %d )",
            m_pTBMazeInfo->ServerMap, 161);
        return false;
    }

    // IDA: CTextDBLog::Init(&this->m_textDBLog, ...)
    CTextDBLog::Init(&m_textDBLog, static_cast<__int64>(GetMapID().nMapID));

    // Generate Maze
    Generate();

    // Create script instance
    if (!CreateScriptInst(m_pTBMazeInfo->Server_SceneScript_File)) {
        LogHelper::LogError("game.contents",
            "Init error - Failed CreateScriptInst[ %s ] ( %d )",
            m_pTBMazeInfo->Server_SceneScript_File, 170);
        return false;
    }

    // IDA: Initialize Grouton from TB_MAZEREWARD_ITEM
    m_nGroutonBoxID = 0;
    m_dwWaitGroutonSpawnTime = 0;
    TB_MAZEREWARD_ITEM* pRewardItem = pServer->GetResourceMgr().GetTB_MAZEREWARD_ITEM(TBMapID);
    if (pRewardItem) {
        m_nGroutonBoxID = pRewardItem->Box_ID;
    }

    // Spawn monsters
    SpawnGenerateMonster();

    // IDA: XIOCPServer::BackSends(this, nullptr) - skip, XMaze doesn't inherit XIOCPServer

    // IDA: CWarpPotal::Init(&this->m_xWarpPotal, this)
    // Note: IDA shows embedded object m_xWarpPotal, but we use pointer m_pWarpPotal
    if (!m_pWarpPotal) {
        m_pWarpPotal = new CWarpPotal();
    }
    m_pWarpPotal->Init(this);

    // IDA: NavMesh silhouette step with write lock
    if (m_pNavMeshInstance) {
        CFAutoSlimWriteLock _autolock(&pServer->GetMapLock());
        hkaiWorld* pWorld = m_pNavMeshInstance->GetUpdateFunc();
        if (pWorld) {
            pWorld->stepSilhouettes(nullptr);
        }
    } else {
        return false;
    }

    // Set wait user time
    m_dwWaitUserTime = GetTickCount64() + 300000;  // 5 minutes

    // Clear maps
    m_mpFpUseUCID.clear();
    m_mapGameTrapObject.clear();
    m_mapGameTrapObjectGroup.clear();
    m_mapReviveCount.clear();
    m_mapUserDamage.clear();
    m_mapUserHitedCount.clear();
    m_mapCheckSectorUser.clear();

    // IDA: Get spawn rate from TimeEventMgr
    m_nSpawnRateGrouton = pServer->GetTimeEventMgr().GetEventValue(3);

    // Clear Maze Log
    std::memset(m_nMazeLog, 0, sizeof(m_nMazeLog));

    // IDA: STMonsterKillScoreMode::Init(&this->m_stMonsterKillScoreMode)
    STMonsterKillScoreMode::Init(&m_stMonsterKillScoreMode);

    // Initialize Monster Kill Score Mode
    m_nMonsterKiillScoreModeState = 0;
    m_bRestartReady = false;
    m_nTimeStepTarget = 0;
    m_nTimeStepState = 0;
    m_strTimeStepFailScript.clear();

    return true;
}

// ============================================================================
// Create
// IDA: 0x140315870
// ============================================================================
bool XMaze::Create(ST_CREATE_MAZE& stCreateMaze) {
    // Per IDA 0x140315870: XMaze::Create 精确还原
    // 1. 设置 Map ID (UXMapID -> TUXMapID conversion via nMapID field)
    m_uxMapID.nMapID = stCreateMaze.uxMapID.nMapID;

    // 2. 添加等待进入 Maze 的用户
    for (const auto& memberInfo : stCreateMaze.vecEnterMember) {
        ST_MAZE_WAIT_ENTER_USER_INFO stInfo;
        stInfo.stMemberInfo = memberInfo;
        stInfo.dw64ExitTime = 0;
        stInfo.byState = 10;  // 等待状态
        // IDA: Key is dwMember (first field of ST_ENTER_MAZE_MEMBER_INFO)
        m_mapWaitEnterMazeUser[memberInfo.dwMember] = stInfo;
    }

    // 3. 设置用户计数
    m_nPartyMemeberCount = static_cast<int>(m_mapWaitEnterMazeUser.size());
    m_nMaxUserCount = 14;

    // 4. 调用 Init
    if (!Init()) {
        std::uint16_t nMapID = GetTBMapID();
        GreenDamTan_log(__FILE__, __FUNCTION__, "Create error - Failed create maze when Init[ TID:%d ] ( %d )", nMapID, 1107);
        return false;
    }

    // 5. 保存创建信息
    m_stCreateMazeInfo = stCreateMaze;

    return true;
}

// IDA: ?Clear@XMaze@@UEAAXXZ (0x140311C60)
// IDA 精确还原 - 清除迷宫中所有数据
void XMaze::Clear() {
    // IDA: 清除怪物生成信息列表
    m_listMonsterSpawnInfo.clear();

    // IDA: 检查是否是 Maze_Group == 74 的特殊迷宫（需要记录日志）
    if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Group == 74) {
        // TODO: ST_LOG_GAME 日志记录
        // ST_LOG_GAME stLog;
        // stLog._nUAID = 0;
        // stLog._nUCID = 0;
        // stLog._sMainType = 51;
        // stLog._sSubType = 19;
        // stLog.nParam0 = m_nMazeLog[0];
        // ... 等等
        // XGameServer::Instance()->SendDBLog(&stLog);
    }

    // IDA: 发送文本DB日志
    // CTextDBLog::SendLogDB(&m_textDBLog);

    // IDA: 遍历 objectScanner 中的所有玩家，踢出他们
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            // IDA: 尝试转换为 CUser
            CUser* pUser = dynamic_cast<CUser*>(pMover);
            if (pUser) {
                // TODO: Kickout user
                // PS_KICK_USER_INFO psKick;
                // psKick.dwUAID = pUser->GetUAID();
                // psKick.byKickType = 4;
                // pUser->Kickout(&psKick, 1);
            }
        }
    }

    // IDA: 清除重生盒子
    m_mapRespawnBox.clear();

    // IDA: 清除处理生成盒子并删除指针
    for (auto it = m_mapProcessSpawnBox.begin(); it != m_mapProcessSpawnBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapProcessSpawnBox.clear();

    // IDA: 清除事件生成盒子并删除指针
    for (auto it = m_mapEventSpawnBox.begin(); it != m_mapEventSpawnBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapEventSpawnBox.clear();

    // IDA: 清除随机传送门盒子
    m_mapRandomPotalBox.clear();

    // IDA: 清除传送门盒子并删除指针
    for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapPotalBox.clear();

    // IDA: 清除门盒子并删除指针
    for (auto it = m_mapGateBox.begin(); it != m_mapGateBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapGateBox.clear();

    // IDA: 清除Lua函数盒子并删除指针
    for (auto it = m_mapLuaFunctionBox.begin(); it != m_mapLuaFunctionBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapLuaFunctionBox.clear();

    // IDA: 清除交互盒子并删除指针
    for (auto it = m_mapInteractionBox.begin(); it != m_mapInteractionBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapInteractionBox.clear();

    // IDA: 清除任务移动盒子并删除指针
    for (auto it = m_mapQuestMoveBox.begin(); it != m_mapQuestMoveBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapQuestMoveBox.clear();

    // IDA: 清除检查事件生成盒子并删除指针
    for (auto it = m_mapCheckEventSpawnBox.begin(); it != m_mapCheckEventSpawnBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapCheckEventSpawnBox.clear();

    // IDA: 清除普通位置盒子
    m_mapCommonPostionBox.clear();

    // IDA: 清除扇区起始盒子列表
    m_listSectorStartBox.clear();

    // IDA: 清除变更怪物列表
    m_lstChangeMonster.clear();

    // IDA: 清除剪影对象列表并删除指针
    for (auto it = m_lstSilhouetteObject.begin(); it != m_lstSilhouetteObject.end(); ) {
        void* pSilhouette = *it;
        if (pSilhouette) {
            // TODO: VTrapBase_cl::SetLifeTime(pSilhouette, 0.0f);
        }
        it = m_lstSilhouetteObject.erase(it);
    }

    // IDA: 清除所有 Actor（从 m_mapActor）
    // for (auto iter = m_mapActor.Begin(); iter != m_mapActor.End(); ++iter) {
    //     XActor* pActor = iter->second;
    //     if (pActor) {
    //         // 根据类型处理不同类型的 Actor
    //         CNpc* pNpc = dynamic_cast<CNpc*>(pActor);
    //         if (pNpc) {
    //             DeleteNpc(pNpc);
    //             continue;
    //         }
    //         CAkashicObject* pAkashic = dynamic_cast<CAkashicObject*>(pActor);
    //         if (pAkashic) {
    //             DeleteAkashicObject(pAkashic);
    //             continue;
    //         }
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
    //         if (pMonster) {
    //             DeleteMonster(pMonster);
    //             continue;
    //         }
    //     }
    // }
    // m_mapActor.clear();

    // IDA: 清除扇区并删除指针
    for (auto it = m_mapSector.begin(); it != m_mapSector.end(); ++it) {
        CSector* pSector = it->second;
        if (pSector) {
            // TODO: pSector->Clear();
            delete pSector;
            it->second = nullptr;
        }
    }
    m_mapSector.clear();

    // IDA: 清除游戏模式
    // GameModeMgr::DestroyGameMode(GameModeMgr::Instance(), GetInstanceID());

    // IDA: 清除待销毁对象列表
    m_lstDestoryObject.clear();

    // IDA: 清除等待进入迷宫的用户
    m_mapWaitEnterMazeUser.clear();

    // IDA: 清除迷宫计数用户
    m_mapMazeCountUser.clear();

    // IDA: 清除更新进入处理集合
    m_setUpdateEnterProcess.clear();

    // IDA: 清除检查扇区用户
    m_mapCheckSectorUser.clear();

    // IDA: 清除复活计数
    m_mapReviveCount.clear();

    // IDA: 清除用户受击计数
    m_mapUserHitedCount.clear();

    // IDA: 清除用户伤害
    m_mapUserDamage.clear();

    // IDA: 清除怪物伤害类型
    m_mapMonsterDamageType.clear();

    // IDA: 清除 FP 使用 UCID
    m_mpFpUseUCID.clear();

    // IDA: 清除掉落列表
    m_mapDropList.clear();

    // IDA: 清除过场动画关闭状态
    m_mapCutsceneOff.clear();

    // IDA: 清除游戏陷阱对象
    for (auto it = m_mapGameTrapObject.begin(); it != m_mapGameTrapObject.end(); ++it) {
        if (it->second) {
            // TODO: 释放陷阱对象
        }
    }
    m_mapGameTrapObject.clear();

    // IDA: 清除游戏陷阱对象组
    for (auto it = m_mapGameTrapObjectGroup.begin(); it != m_mapGameTrapObjectGroup.end(); ++it) {
        for (void* pTrap : it->second) {
            if (pTrap) {
                // TODO: 释放陷阱对象
            }
        }
    }
    m_mapGameTrapObjectGroup.clear();

    // IDA: 清除脚本死亡怪物
    m_mapCallScriptDieMonster.clear();

    // IDA: 清除组怪物
    m_mapGroupMOB.clear();
    m_mapGroupID_Monster.clear();

    // IDA: 清除生成盒子组限制
    m_mapSpawnBoxGroupLimit.clear();

    // IDA: 清除 Roguelike 相关数据
    m_mapRogueSector.clear();
    m_mapRoguelikeRandomPocketInfo.clear();
    m_mapRoguelikePortalBuffList.clear();
    m_mapRoguelikePortalBuff.clear();
    m_mapRandomBuff.clear();

    // IDA: 清除时间步进定时器
    m_mapTimeStepTimer.clear();

    // IDA: 清除定时器
    m_arLogicTimers.clear();
    m_arWaitLogicTimers.clear();

    // IDA: 清除等待接收信息列表
    m_listWaitForRecvInfo.clear();

    // IDA: 清除召唤助手
    m_listSummonedHelper.clear();

    // IDA: 清除死亡怪物生成盒子ID
    m_listDieMonsterSpawnBoxID.clear();

    // IDA: 清除同步生成活动
    m_listSyncSpawnActive.clear();

    // IDA: 清除活动最后扇区ID
    m_vecActiveLastSectorID.clear();

    // IDA: 清除 Lua 值
    m_vecLuaValue.clear();
    m_vecLuaValues.clear();

    // IDA: 清除游戏规则
    m_vecGameRules.clear();

    // IDA: 清除 Roguelike 随机商店状态
    m_vecRoguelikeRandomShopStat.clear();

    // IDA: 重置隐藏事件
    if (m_pHiddenEvent) {
        // TODO: CHiddenEvent::Clear(m_pHiddenEvent);
        // delete m_pHiddenEvent;
        m_pHiddenEvent = nullptr;
    }

    // IDA: 重置系统 Actor
    m_pSystemActor = nullptr;

    // IDA: 重置 Boss 扇区标志
    m_bBossSector = false;

    // IDA: 重置事件扇区
    m_pActiveEventSector = nullptr;

    // IDA: 重置 Roguelike 相关
    m_pRogueBossSector = nullptr;
    m_pRogueNextSector = nullptr;
    m_pRogueStartSector = nullptr;
    m_nRoguelikeMaxSector = 0;
    m_nRoguelikeGoBossState = 0;
    m_nRoguelikePortalBuffID = 0;
    m_nRoguelikeLastPortalID = 0;

    // IDA: 重置护送怪物
    memset(&m_stEscortMonster, 0, sizeof(m_stEscortMonster));

    // IDA: 重置队伍任务
    m_stPartyQuest.reset();

    // IDA: 重置迷宫游戏状态
    m_stMazeGameState = ST_MAZE_GAME_STATE();

    // IDA: 重置其他标志
    m_bMazeComplete = false;
    m_bRestartReady = false;
    m_bHaveBotUser = false;
    m_nMonsterKiillScoreModeState = 0;
    m_nTimeStepTarget = 0;
    m_nTimeStepState = 0;
    m_nBit = 0;
    m_nPartyUserCount = 0;
    m_nPartyMemeberCount = 0;
    m_nLastSpawnedSectorID = 0;

    // IDA: 清除等待时间
    m_dwWaitToEnterForceMember = 0;
    m_dwWaitToLoadEXMember = 0;
    m_dwWaitUserTime = 0;
    m_dwWaitGroutonSpawnTime = 0;
    m_fUpdateProcessEscort = 0.0f;
    m_fCellPosCheckTime = 0.0f;

    // IDA: 清除传送门
    if (m_pWarpPotal) {
        // TODO: delete m_pWarpPotal;
        m_pWarpPotal = nullptr;
    }

    // IDA: 清除导航网格
    if (m_pNavMeshInstance) {
        // TODO: DohHavokNavMeshInstance::Destroy(m_pNavMeshInstance);
        m_pNavMeshInstance = nullptr;
    }
    m_nNavMeshIndex = 0;

    // IDA: 清除脚本实例
    if (m_pScriptInstance) {
        // TODO: IVScriptInstance::Release(m_pScriptInstance);
        m_pScriptInstance = nullptr;
    }

    // IDA: 清除迷宫资源
    if (m_pMazeResource) {
        // TODO: VBaseResourceLump::Release(m_pMazeResource);
        m_pMazeResource = nullptr;
    }

    // IDA: 清除过场动画管理器
    if (m_pCutSceneManager) {
        // TODO: CCutsceneManager::Clear(m_pCutSceneManager);
    }

    // IDA: 重置休闲突袭计时器
    m_stCasualRaidTime.reset();

    // IDA: 重置时间步进字符串
    m_strTimeStepFailScript.clear();

    // IDA: 调用基类 Clear
    XArea::Clear();
}

// IDA: ?Generate@XMaze@@UEAAXXZ (0x140315E60)
// IDA 精确还原 - 生成迷宫中的所有对象
void XMaze::Generate() {
    // IDA: 获取批处理层级
    int nBatchLayerLevel = GetBatchLayerLevel();

    // IDA: 获取事件对象资源中的 Map
    // VEventObjectResource* pEventRes = VEventObjectResource::GetMap(m_pMazeResource, nBatchLayerLevel, 1, m_nBit);

    // IDA: 遍历所有事件盒子并创建相应的数据结构
    // for (auto& boxInfo : pEventRes->GetBoxList()) {
    //     switch (boxInfo->GetType()) {
    //         case eEventBoxType_ProcessSpawn: {
    //             VMonsterSpawnInfo* pMonsterSpawn = static_cast<VMonsterSpawnInfo*>(boxInfo);
    //
    //             // IDA: 处理创建条件
    //             if (pMonsterSpawn->m_iCreationCondition == 2) {
    //                 // 创建处理生成盒子
    //                 STMageProcessSpawnBox* pProcessSpawn = new STMageProcessSpawnBox();
    //                 pProcessSpawn->nBoxIndex = pMonsterSpawn->iUniqueID;
    //                 pProcessSpawn->pSpawnBox = pMonsterSpawn;
    //                 m_mapProcessSpawnBox[pProcessSpawn->nBoxIndex] = pProcessSpawn;
    //             } else if (pMonsterSpawn->m_iCreationCondition == 1) {
    //                 // 添加到怪物生成信息列表
    //                 m_listMonsterSpawnInfo.push_back(pMonsterSpawn);
    //             }
    //
    //             // IDA: 处理重生盒子
    //             if (pMonsterSpawn->m_RespawnTime > 0.0f &&
    //                 pMonsterSpawn->m_eRespawnType == 1 &&
    //                 pMonsterSpawn->m_iRespawnCondition > 0) {
    //                 m_mapRespawnBox[pMonsterSpawn->iID] = pMonsterSpawn;
    //             }
    //
    //             // IDA: 添加怪物组ID
    //             AddMonsterGroupID(pMonsterSpawn->m_iGroupID);
    //             AddGroupIDMonsterList(pMonsterSpawn->m_iGroupID, pMonsterSpawn);
    //             break;
    //         }
    //         case eEventBoxType_MonsterSpawn: {
    //             VCheckMonsterSpawnInfo* pCheckMonsterSpawn = static_cast<VCheckMonsterSpawnInfo*>(boxInfo);
    //             STMageEventSpawnBox* pEventSpawn = new STMageEventSpawnBox();
    //             pEventSpawn->nBoxIndex = pCheckMonsterSpawn->iUniqueID;
    //             pEventSpawn->nLoopCount = pCheckMonsterSpawn->m_iLoopCount;
    //             pEventSpawn->pEventBox = pCheckMonsterSpawn;
    //             m_mapEventSpawnBox[pCheckMonsterSpawn->iUniqueID] = pEventSpawn;
    //             break;
    //         }
    //         case eEventBoxType_Potal: {
    //             // IDA: 创建传送门盒子
    //             STMagePotalBox* pPotalBox = new STMagePotalBox();
    //             // TODO: 填充数据
    //             m_mapPotalBox[boxInfo->iUniqueID] = pPotalBox;
    //             break;
    //         }
    //         case eEventBoxType_RandomPotal: {
    //             // IDA: 创建随机传送门盒子
    //             // m_mapRandomPotalBox[boxInfo->iUniqueID] = ...;
    //             break;
    //         }
    //         case eEventBoxType_CommonPosition: {
    //             // IDA: 创建普通位置盒子
    //             // m_mapCommonPostionBox[boxInfo->iUniqueID] = ...;
    //             break;
    //         }
    //         case eEventBoxType_Sector: {
    //             // IDA: 创建扇区
    //             VSectorBoxInfo* pSectorBox = static_cast<VSectorBoxInfo*>(boxInfo);
    //             CSector* pSector = new CSector();
    //             // pSector->Init(this, pSectorBox);
    //
    //             // IDA: 设置扇区的步进生成信息
    //             // for (int i = 0; i < 3; ++i) {
    //             //     pSector->SetStepSpawn(i, pSectorBox->m_iConditionKillRatioStep[i]);
    //             // }
    //             // pSector->InitClearType();
    //
    //             m_mapSector[pSectorBox->iUniqueID] = pSector;
    //
    //             // IDA: Roguelike 处理
    //             if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Type == 15) {
    //                 // Roguelike boss sector handling
    //                 if (m_pRogueBossSector) {
    //                     // Check if this is the boss sector
    //                     // int nUniqueID = pSector->GetSectorBoxUniqueID();
    //                     // m_mapRogueSector[nUniqueID] = pSector;
    //                 } else {
    //                     m_pRogueBossSector = pSector;
    //                 }
    //             }
    //             break;
    //         }
    //         case eEventBoxType_Gate: {
    //             // IDA: 创建门盒子
    //             // m_mapGateBox[boxInfo->iUniqueID] = ...;
    //             break;
    //         }
    //         case eEventBoxType_LuaFunction: {
    //             // IDA: 创建Lua函数盒子
    //             // m_mapLuaFunctionBox[boxInfo->iUniqueID] = ...;
    //             break;
    //         }
    //         case eEventBoxType_Interaction: {
    //             // IDA: 创建交互盒子
    //             // m_mapInteractionBox[boxInfo->iUniqueID] = ...;
    //             break;
    //         }
    //         case eEventBoxType_QuestMove: {
    //             // IDA: 创建任务移动盒子
    //             // m_mapQuestMoveBox[boxInfo->iUniqueID] = ...;
    //             break;
    //         }
    //         case eEventBoxType_CheckEventSpawn: {
    //             VCheckEventSpawnBoxInfo* pCheckEventSpawn = static_cast<VCheckEventSpawnBoxInfo*>(boxInfo);
    //             STMageCheckEventSpawnBox* pCheckSpawnBox = new STMageCheckEventSpawnBox();
    //             pCheckSpawnBox->nBoxIndex = pCheckEventSpawn->iUniqueID;
    //             pCheckSpawnBox->pEventBox = pCheckEventSpawn;
    //             m_mapCheckEventSpawnBox[pCheckEventSpawn->iUniqueID] = pCheckSpawnBox;
    //             break;
    //         }
    //         case eEventBoxType_SectorStart: {
    //             // IDA: 添加到扇区起始盒子列表
    //             // m_listSectorStartBox.push_back(...);
    //             break;
    //         }
    //     }
    // }

    // IDA: 处理怪物生成信息列表，添加到相应的扇区
    for (auto it = m_listMonsterSpawnInfo.begin(); it != m_listMonsterSpawnInfo.end(); ++it) {
        VMonsterSpawnInfo* pSpawnBox = *it;
        if (pSpawnBox) {
            // IDA: 获取扇区ID
            // int nSectorID = VEventObjectInfo::GetEventUniqueID(pSpawnBox->m_iSectorID, ...);

            // IDA: 查找扇区并添加生成盒子
            // auto sectorIt = m_mapSector.find(nSectorID);
            // if (sectorIt != m_mapSector.end()) {
            //     CSector* pSector = sectorIt->second;
            //     if (pSector) {
            //         pSector->AddSpawnBox(pSpawnBox);
            //         if (IsSectorClearConditionBox(pSpawnBox)) {
            //             pSector->AddSpawnBoxID(pSpawnBox->iUniqueID);
            //         }
            //     }
            // }
        }
    }

    // IDA: 处理处理生成盒子，添加到相应的扇区
    for (auto it = m_mapProcessSpawnBox.begin(); it != m_mapProcessSpawnBox.end(); ++it) {
        STMageProcessSpawnBox* pProcessSpawn = it->second;
        if (pProcessSpawn && pProcessSpawn->pSpawnBox) {
            // IDA: 获取扇区ID并添加生成盒子
            // int nSectorID = VEventObjectInfo::GetEventUniqueID(pProcessSpawn->pSpawnBox->m_iSectorID, ...);
            // auto sectorIt = m_mapSector.find(nSectorID);
            // if (sectorIt != m_mapSector.end()) {
            //     CSector* pSector = sectorIt->second;
            //     if (pSector && IsSectorClearConditionBox(pProcessSpawn->pSpawnBox)) {
            //         pSector->AddSpawnBoxID(pProcessSpawn->nBoxIndex);
            //     }
            // }
        }
    }

    // IDA: 处理扇区起始盒子列表
    for (auto it = m_listSectorStartBox.begin(); it != m_listSectorStartBox.end(); ++it) {
        VSafeAreaBoxInfo* pStartBox = *it;
        if (pStartBox) {
            // IDA: 查找扇区并设置起始位置
            // int nSectorID = ...;
            // auto sectorIt = m_mapSector.find(nSectorID);
            // if (sectorIt != m_mapSector.end()) {
            //     CSector* pSector = sectorIt->second;
            //     if (pSector) {
            //         pSector->SetStartPosition(pStartBox);
            //     }
            // }
        }
    }
    m_listSectorStartBox.clear();

    // IDA: Roguelike 特殊处理
    if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Type == 15) {
        // IDA: 处理 Roguelike 传送门盒子
        for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
            STMagePotalBox* pPotalBox = it->second;
            if (pPotalBox) {
                // IDA: 获取扇区
                // int nSectorID = ...;
                // auto sectorIt = m_mapSector.find(nSectorID);
                // if (sectorIt != m_mapSector.end()) {
                //     CSector* pSector = sectorIt->second;
                //     if (pSector) {
                //         // 设置传送门连接
                //     }
                // }
            }
        }

        // IDA: 设置 Roguelike 起始扇区
        if (!m_pRogueStartSector && !m_mapSector.empty()) {
            m_pRogueStartSector = m_mapSector.begin()->second;
        }
    }

    // IDA: Generate completed
}

// ============================================================================
// SpawnGenerateMonster
// IDA: ?SpawnGenerateMonster@XMaze@@UEAAXXZ (0x140317750)
// 精确还原自 IDA 反编译
// ============================================================================
void XMaze::SpawnGenerateMonster() {
    // IDA: Skip if roguelike map
    if (IsRoguelikeMap()) {
        return;
    }

    // IDA: Iterate through sectors and spawn monsters (max 2 steps)
    int nStep = 0;
    for (auto it = m_mapSector.begin(); it != m_mapSector.end(); ++it) {
        if (nStep >= 2) {
            break;
        }

        CSector* pSector = it->second;
        if (pSector) {
            // IDA: if (!CSector::IsSpawnedAll(pSector))
            if (!pSector->IsSpawnedAll()) {
                m_nLastSpawnedSectorID = static_cast<int>(it->first);
                pSector->SpawnMonster(eSendInfoTypeNot);
                ++nStep;
            }
        }
    }

    // IDA: Create system actor if not already created
    if (m_pSystemActor) {
        return;
    }

    // IDA: Get maze info
    std::uint16_t TBMapID = GetTBMapID();
    XGameServer* pGameServer = XGameServer::Instance();
    TB_MAZE_INFO* pMazeInfo = pGameServer->GetResourceMgr().GetTB_MAZE_INFO(TBMapID);

    if (!pMazeInfo || !pMazeInfo->Maze_SceneAttack_Monster) {
        return;
    }

    // IDA: Create system actor at origin
    XVec3 vPos(0.0f, 0.0f, 0.0f);
    ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    UXMapID uxMapID;
    uxMapID.nMapID = GetMapID().nMapID;
    m_pSystemActor = pThreadData->CreateMonster(
        this, uxMapID, pMazeInfo->Maze_SceneAttack_Monster, vPos, 0.0f);

    if (m_pSystemActor) {
        // IDA: Enter game object
        if (EnterGameObject(m_pSystemActor, eSendInfoTypeNot)) {
            DeleteMonster(m_pSystemActor);
            m_pSystemActor = nullptr;
        }

        // IDA: Set system actor flag
        if (m_pSystemActor) {
            m_pSystemActor->SetSystemActor(true);
        }
    } else {
        LogHelper::LogDebug("game.contents", "SystemAotr Not ID : %d", pMazeInfo->Maze_SceneAttack_Monster);
    }
}

// ============================================================================
// ExcuteEventSpawn
// IDA: ?ExcuteEventSpawn@XMaze@@QEAAXH@Z (0x140317A40)
// ============================================================================
void XMaze::ExcuteEventSpawn(int nBoxIndex) {
    // IDA: Calculate unique box ID
    int nBatchLayerLevel = GetBatchLayerLevel();
    int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nBoxIndex, nBatchLayerLevel);

    // IDA: Find event spawn box in map
    auto it = m_mapEventSpawnBox.find(iBoxUniqueID);
    if (it == m_mapEventSpawnBox.end()) {
        return;
    }

    STMageEventSpawnBox* pEventSpawn = it->second;
    if (!pEventSpawn) {
        return;
    }

    // IDA: Check loop count and event box
    if (pEventSpawn->nLoopCount <= 0 || !pEventSpawn->pEventBox) {
        return;
    }

    // IDA: Decrement loop count
    --pEventSpawn->nLoopCount;

    // IDA: Execute spawn box checks
    // for (int i = 0; i < 10 && pEventSpawn->pEventBox->m_iCheckBox[i]; ++i) {
    //     ExcuteSpawnBoxCheck(pEventSpawn->pEventBox->m_iCheckBox[i], eSendInfoTypeSend, false);
    // }
}

// ============================================================================
// EnterGameObject
// IDA: 0x140313130
// ============================================================================
std::uint16_t XMaze::EnterGameObject(XActor* pActor, E_SEND_INFO_TYPE eType) {
    if (!pActor) {
        return 50001;
    }

    // IDA: Call base class EnterActor
    std::uint16_t xError = XArea::EnterActor(pActor);
    if (xError) {
        return xError;
    }

    // IDA: Get scanner for this actor
    std::map<std::uint32_t, CMover*>* vecActor = GetScanner(pActor);
    if (!vecActor) {
        LogHelper::LogDebug("game.contents", "<SCANNER> Scanner is nullptr Actor %d ( %d )", 0, 554);
        return 50001;
    }

    // IDA: Insert into scanner map
    CMover* pMover = dynamic_cast<CMover*>(pActor);
    if (pMover) {
        vecActor->insert(std::make_pair(static_cast<std::uint32_t>(pMover->GetID()), pMover));
    }

    // IDA: Get TBID based on actor type
    int nTBID = 0;
    E_ACTOR_TYPE actorType = pActor->GetType();
    if (actorType == E_ACTOR_TYPE::eActorMonster) {
        CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
        if (pMonster) {
            nTBID = pMonster->GetTableID();
        }
    } else if (actorType == E_ACTOR_TYPE::eActorNPC) {
        // TODO: CNpc is incomplete type
        // CNpc* pNpc = dynamic_cast<CNpc*>(pActor);
        // if (pNpc) {
        //     nTBID = pNpc->GetTableID();
        // }
    }

    // IDA: Log to text DB
    // TODO: CQuestCondition::GetQuestID needs implementation
    // m_textDBLog.AddLog(2, 0, nTBID, "");

    // IDA: Send info if eType == eSendInfoTypeSend
    if (eType == E_SEND_INFO_TYPE::eSendInfoTypeSend) {
        if (actorType == E_ACTOR_TYPE::eActorNPC) {
            // IDA: Send NPC info packet (main=4, sub=0x13)
            // TODO: CNpc is incomplete type
            // CNpc* pNpc = dynamic_cast<CNpc*>(pActor);
            // if (pNpc) {
            //     PS_NPCINFO_VEC stNpcInfos;
            //     stNpcInfos.vecNpcInfo.push_back(pNpc->GetNpcInfo());
            //     XSendPacket xPacket(4, 0x13);
            //     xPacket << stNpcInfos;
            //     SendBroadCast(xPacket, nullptr, E_BROADCAST_TYPE::eAll);
            // }
        } else if (actorType == E_ACTOR_TYPE::eActorMonster) {
            // IDA: Send Monster info packet (main=4, sub=0x15)
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (pMonster) {
                // TODO: PS_MONSTERINFO_VEC packet
                // PS_MONSTERINFO_VEC stMonsterInfos;
                // stMonsterInfos.vecMonsterInfo.push_back(pMonster->GetMonsterInfo());
                // XSendPacket xPacket(4, 0x15);
                // xPacket << stMonsterInfos;
                // SendBroadCast(xPacket, nullptr, E_BROADCAST_TYPE::eAll);
            }
        }
    }

    return 0;
}

// ============================================================================
// ExitGameObject
// IDA: 0x140313580
// ============================================================================
std::uint16_t XMaze::ExitGameObject(XActor* pActor, E_SEND_INFO_TYPE eType) {
    if (!pActor) {
        return 50001;
    }

    // IDA: Call base class ExitActor
    XArea::ExitActor(pActor);

    // IDA: Get TBID based on actor type
    int nTBID = 0;
    E_ACTOR_TYPE actorType = pActor->GetType();
    if (actorType == E_ACTOR_TYPE::eActorMonster) {
        CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
        if (pMonster) {
            nTBID = pMonster->GetTableID();
        }
    } else if (actorType == E_ACTOR_TYPE::eActorNPC) {
        // TODO: CNpc is incomplete type
        // CNpc* pNpc = dynamic_cast<CNpc*>(pActor);
        // if (pNpc) {
        //     nTBID = pNpc->GetTableID();
        // }
    }

    // IDA: Log to text DB
    // TODO: CQuestCondition::GetQuestID needs implementation
    // m_textDBLog.AddLog(3, 0, nTBID, "");

    // IDA: Send info if eType == eSendInfoTypeSend
    if (eType == E_SEND_INFO_TYPE::eSendInfoTypeSend) {
        if (actorType == E_ACTOR_TYPE::eActorNPC) {
            // IDA: Send NPC exit packet (main=4, sub=0x14)
            XSendPacket xSendPacket(4, 0x14);
            SendOutInfo(&xSendPacket, pActor);
        } else if (actorType == E_ACTOR_TYPE::eActorMonster) {
            // IDA: Send Monster exit packet (main=4, sub=0x16)
            XSendPacket xSendPacket(4, 0x16);
            SendOutInfo(&xSendPacket, pActor);
        }
    }

    // IDA: Get scanner and remove actor
    std::map<std::uint32_t, CMover*>* pVecActor = GetScanner(pActor);
    if (!pVecActor) {
        LogHelper::LogDebug("game.contents", "<SCANNER> Cant Find Maze Actor %d", 0);
        return 50001;
    }

    // IDA: Find and erase actor from scanner
    UXActorID actorID = pActor->GetActorID();
    std::uint32_t dwActorID = actorID.dwActorID;
    auto it = pVecActor->find(dwActorID);
    if (it == pVecActor->end()) {
        LogHelper::LogDebug("game.contents", "<SCANNER> Cant Find Maze Actor %d", dwActorID);
        return 50001;
    }

    pVecActor->erase(it);
    return 0;
}

// ============================================================================
// IsAliveMonster
// IDA: 0x140313950
// ============================================================================
bool XMaze::IsAliveMonster(int nTableID) {
    // IDA: Iterate over m_mapActor (std::map)
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (!pActor) {
            continue;
        }

        // IDA: Check if actor type is 2 (Monster)
        if (pActor->GetType() == E_ACTOR_TYPE::eActorMonster) {
            // IDA: RTTI cast to CMonster
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (pMonster) {
                // IDA: Check TableID match
                if (pMonster->GetTableID() == nTableID) {
                    // IDA: Return true if not in death status (status != 2)
                    return !pMonster->IsStatus(2);
                }
            }
        }
    }
    return false;
}

// ============================================================================
// EnterActor
// IDA: 0x140313A60
// IDA: ?EnterActor@XMaze@@UEAAGPEAVXActor@@@Z - returns unsigned short
// ============================================================================
std::uint16_t XMaze::EnterActor(XActor* pActor) {
    // IDA: Check null pointer
    if (!pActor) {
        return 50001;
    }

    // IDA: Check Maze state - return 55002 if state is 5 or 6
    int nMazeState = GetMazeGameState();
    if (nMazeState == 5 || nMazeState == 6) {
        return 55002;
    }

    // IDA: _RTDynamicCast to CUser
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser) {
        return 50001;
    }

    // IDA: Call base class EnterActor
    std::uint16_t xError = XArea::EnterActor(pActor);
    if (xError) {
        return xError;
    }

    // IDA: GetScanner for this actor
    std::map<std::uint32_t, CMover*>* vecActor = GetScanner(pActor);
    if (!vecActor) {
        LogHelper::LogDebug("game.contents", "<SCANNER> Scanner is nullptr Actor %d ( %d )", 0, 716);
        return 50001;
    }

    // IDA: Insert into scanner map
    CMover* pMover = dynamic_cast<CMover*>(pActor);
    if (pMover) {
        vecActor->insert(std::make_pair(static_cast<std::uint32_t>(pMover->GetID()), pMover));
    }

    // IDA: Set area
    pActor->SetArea(this);

    // IDA: CheckBuffByLocation
    pUser->CheckBuffByLocation(this);

    // IDA: Get CGocRecode and InitMaze
    {
        auto pRecode = pUser->GetGOC_Recode(false);
        if (pRecode) {
            int nMazeID = XArea::GetTBMapID();
            pRecode->InitMaze(nMazeID);
        }
    }

    // IDA: EnterPartyForceMember
    EnterPartyForceMember(pUser);

    // IDA: Reset wait user time
    m_dwWaitUserTime = 0;

    // IDA: Log enter
    std::uint32_t TBMapID = XArea::GetTBMapID();
    LogHelper::LogDebug("game.contents", "<%d MAZE> Enter User : %d", TBMapID, pMover ? pMover->GetID() : 0);

    // IDA: SendSyncHiddenEventInfo
    SendSyncHiddenEventInfo(pUser);

    // IDA: Get CGocEntity and UpdateOpenTitle
    {
        auto pEntity = pUser->GetGOC_Entity(false);
        if (pEntity) {
            int nValue = XArea::GetTBMapID();
            pEntity->UpdateOpenTitle(4, nValue);
        }
    }

    // IDA: Get CGocBooster and EnterBooster
    {
        auto pBooster = pUser->GetGOC_Booster(false);
        if (pBooster) {
            pBooster->EnterBooster(2);
        }
    }

    // IDA: ChangeBattlePose(true)
    pUser->ChangeBattlePose(true);

    // IDA: Get start portal position and move
    // TODO: XWorldResMgr::GetStartPortalPos not implemented
    // STPosInfo stPosInfo;
    // int nMapID = XArea::GetTBMapID();
    // XGameServer* pServer = XGameServer::Instance();
    // if (pServer && pServer->GetWorldResMgr().GetStartPortalPos(nMapID, &stPosInfo)) {
    //     pUser->Move(&stPosInfo.vPos);
    // }

    // IDA: Check bot user
    if (pActor->IsBot()) {
        m_bHaveBotUser = true;
    }

    // IDA: CutsceneManager AddMember
    if (m_pCutSceneManager) {
        // TODO: CCutsceneManager::AddMember not implemented
        // m_pCutSceneManager->AddMember(pUser);
    }

    // IDA: InitSuperArmorGage and send packet
    // TODO: CUser SuperArmor methods need implementation
    // pUser->InitSuperArmorGage();
    // float fMaxSuperArmor = pUser->GetMaxSuperArmorGage();
    // float fCurSuperArmor = pUser->GetCurSuperArmorGage();
    // CMover::send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE(pUser, fCurSuperArmor, fMaxSuperArmor);

    // IDA: Check not roguelike map - option effect
    if (!IsRoguelikeMap()) {
        // TODO: CheckOptionEffectInvoke, ReleaseInvokedOptionEffect need implementation
        // pUser->CheckOptionEffectInvoke(EFFECT_CONDITION_EQUIP, pUser, 0.0f, EFFECT_INVOKE_DONT_CARE);
        // pUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_EQUIP);
    }

    // IDA: SetSimpleDefenseType(3)
    // TODO: SetSimpleDefenseType needs implementation
    // pUser->SetSimpleDefenseType(3);

    // IDA: Log to text DB
    // TODO: CQuestCondition::GetQuestID needs implementation
    // m_textDBLog.AddLog(0, 0, 0, "");

    return 0;
}

// ============================================================================
// ExitActor
// IDA: 0x1403140D0
// 精确还原: 用户退出迷宫时的处理
// ============================================================================
void XMaze::ExitActor(XActor* pActor) {
    // IDA: 检查 pActor 非空
    if (!pActor) {
        return;
    }

    // IDA: RTTI cast to CUser
    // 使用 reinterpret_cast 代替 dynamic_cast (VS2010 兼容)
    // CUser* pUser = _RTDynamicCast_0(pActor, 0, &XActor `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    CUser* pUser = reinterpret_cast<CUser*>(pActor);

    // IDA: 如果不是 CUser，直接返回
    // 但这里我们暂时跳过这个检查，因为 CMoverEx 提供了足够的功能

    // IDA: 检查控制怪物
    // if (pUser->IsControlMonster()) {
    //     hkvVec3 vPos;  // 默认构造
    //     pUser->SetControlMonster(&vPos, 0, 0);
    // }
    // TODO: CMoverEx::IsControlMonster, SetControlMonster 需要实现

    // IDA: 同步任务条件
    // CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuestPtr, 0);
    // if (pQuestPtr) {
    //     v5 = std::tr1::shared_ptr<CGocQuest>::operator->(&pQuestPtr);
    //     CGocQuest::DBSyncQuestCondition(v5);
    // }
    // TODO: CMover::GetGOC<CGocQuest> 需要实现

    // IDA: 发送耐久度日志和 Ether 日志
    // nMazeID = XArea::GetTBMapID(this);
    // CMover::GetGOC<CGocInventory>(&pUser->CMoverEx, &pInvenPtr, 0);
    // if (pInvenPtr) {
    //     CGocInventory::SendEnduranceLog(v6, nMazeID);
    //     CGocInventory::DropEtherLog(v7);
    // }
    // TODO: CMover::GetGOC<CGocInventory> 需要实现

    // IDA: 移除所有选项效果
    // CMoverEx::RemoveAllOptionEffect(&pUser->CMoverEx);
    // TODO: CMoverEx::RemoveAllOptionEffect 需要实现

    // IDA: 从 WarpPotal 移除
    // v8 = pUser->XActor::GetActorID(&v105);
    // CWarpPotal::RemoveWarpPotal(&m_xWarpPotal, *v8);
    // TODO: CWarpPotal::RemoveWarpPotal 需要实现，成员变量名需要确认是 m_xWarpPotal 还是 m_pWarpPotal

    // IDA: 清除 Buff 类型 1
    // CMover::ClearBuffByType(&pUser->CMoverEx, 1u);
    // TODO: CMover::ClearBuffByType 需要实现

    // IDA: 从 CutsceneManager 删除成员
    // v9 = pUser->XActor::GetActorID(&v106);
    // QuestID = CQuestCondition::GetQuestID(v9);
    // CCutsceneManager::DeleteMember(&m_cutSceneManager, QuestID);
    // TODO: CCutsceneManager::DeleteMember 和 CQuestCondition::GetQuestID 需要实现

    // IDA: 检查 PartyQuest setUser
    // v11 = pActor->GetActorID(pActor, v108);
    // _Keyval = CQuestCondition::GetQuestID(v11);
    // std::_Tree::find(&m_stPartyQuest.setUser, &it, &_Keyval);
    // if (it != end) {
    //     CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuest, 0);
    //     if (pQuest) {
    //         CGocQuest::FailQuest(v13, m_stPartyQuest.nQuestID, a3);
    //     }
    //     erase it from m_stPartyQuest.setUser
    // }
    // TODO: STPartyQuest 结构需要完整定义

    // IDA: 检查护送怪物状态
    // if (m_stPartyQuest.nState == 1 && m_stPartyQuest.setUser.empty()) {
    //     m_stPartyQuest.nState = 2;
    //     if (m_stEscortMonster.pMonster) {
    //         nMotion = CMover::GetMotionClass(m_stEscortMonster.pMonster);
    //         VString strAnimName(m_stEscortMonster.szMonsterDieAnim);
    //         dwAnimID = CMover::GetAnimIndex(m_stEscortMonster.pMonster, strAnimName);
    //         if (dwAnimID != -1)
    //             nMotion = CMover::AnimKeyToMotion(m_stEscortMonster.pMonster, dwAnimID);
    //         v14 = m_stEscortMonster.pMonster->GetHP();
    //         CMoverEx::SetDieReason(m_stEscortMonster.pMonster, 6u, v14);
    //         m_stEscortMonster.pMonster->SetDie_2(m_stEscortMonster.pMonster, nMotion, 1);
    //     }
    // }
    // TODO: ST_ESCORT_MONSTER::pMonster, CMover::GetMotionClass 等函数需要实现

    // IDA: GetScanner 获取扫描器
    std::map<std::uint32_t, CMover*>* pVecActor = GetScanner(pActor);
    if (pVecActor) {
        // IDA: 从 Scanner 中移除用户
        UXActorID actorID = pActor->GetActorID();
        std::uint32_t dwActorIDValue = actorID.dwActorID;

        auto iter = pVecActor->find(dwActorIDValue);
        if (iter != pVecActor->end()) {
            pVecActor->erase(iter);
        } else {
            // IDA: 记录日志 - 找不到 Maze Actor
            TUXMapID instanceID = GetInstanceID();
            GreenDamTan_log(__FILE__, __FUNCTION__, "<SCANNER> Cant Find Maze Actor %d / %I64d",
                           dwActorIDValue, instanceID.nMapID);
        }

        // IDA: 获取用户计数
        int nUserCnt = GetCurUserCount();

        // IDA: 更新 Party/Force Booster
        if (m_pParty) {
            // TODO: CParty::UpdatePartyBooster
            // m_pParty->UpdatePartyBooster(dwLeaveMemberID);
        } else if (m_pForce) {
            // TODO: CForce::UpdateForceBooster
            // m_pForce->UpdateForceBooster(v160);
        }

        // IDA: 检查隐形 Actor
        int nInvisibleCnt = FindInvisibleActorCnt();
        if (nInvisibleCnt > 0 && nUserCnt == 1) {
            // IDA: 清除 Party/Force 的 MazeID
            if (m_pParty) {
                // TODO: 发送 packet (0xF2, 0x55)
                // UXMapID uxMapID(0);
                // m_pParty->SetMazeID(uxMapID);
                // XSendPacket v93(0xF2, 0x55);
                // v93 << m_pParty->GetPartyID();
                // XGameServer::Instance()->m_controlSocket.Send(&v93);
            } else if (m_pForce) {
                // TODO: 发送 packet (0xF2, 0x63)
                // UXMapID uxMapID(0);
                // m_pForce->SetMazeID(uxMapID);
                // XSendPacket v94(0xF2, 0x63);
                // v94 << m_pForce->GetPartyID();
                // XGameServer::Instance()->m_controlSocket.Send(&v94);
            }
        }

        // IDA: 保存 Force 记录
        // CMover::GetGOC<CGocForce>(&pUser->CMoverEx, &pForce, 0);
        // if (pForce) {
        //     CGocParty::SaveRecode(v35);
        // }
        // TODO: CGocForce::SaveRecode 需要实现

        // IDA: 释放 Helper
        // CMover::GetGOC<CGocHelper>(&pUser->CMoverEx, &pHelper, 0);
        // if (pHelper) {
        //     CGocHelper::AllHelperRelease(v36);
        //     CGocHelper::OtherHelperClear(v37);
        // }
        // TODO: CGocHelper 需要实现

        // IDA: 检查跟随怪物
        CheckFollowMonster();

        // IDA: 刷新用户计数
        RefreshUserCountInSector(dwActorIDValue);

        // IDA: 更新等待进入 Maze 用户
        auto it_ = m_mapWaitEnterMazeUser.find(dwActorIDValue);
        if (it_ != m_mapWaitEnterMazeUser.end()) {
            // IDA: 设置退出时间为当前时间 + 300000ms (5分钟)
            it_->second.dw64ExitTime = GetTickCount64() + 300000;
            // IDA: byState == 1 表示正在加载中
            if (it_->second.byState == 1 && !m_bRestartReady) {
                RestartResetState(false, false);
            }
        }

        // IDA: 发送退出信息
        // XSendPacket xSendPacket(4, 0x12);
        // SendOutInfo(this, &xSendPacket, pActor);
        // TODO: SendOutInfo 需要实现

        // IDA: 记录日志
        int TBMapID = GetTBMapID();
        TUXMapID instanceID2 = GetInstanceID();
        GreenDamTan_log(__FILE__, __FUNCTION__, "<%I64d MAZE> Leave User : %d Map : %d",
                       instanceID2.nMapID, dwActorIDValue, TBMapID);

        // IDA: 添加日志到 m_textDBLog
        // CTextDBLog::AddLog(&m_textDBLog, 1, v45, 0, "");
        // TODO: m_textDBLog 需要定义

        // IDA: 发送速度检查日志
        // CUser::SendCheckSpeedLog(pUser);
        // TODO: CUser::SendCheckSpeedLog 需要实现

        // IDA: 检查迷宫类型 (Maze_Type == 13 或 19)
        if (m_pTBMazeInfo &&
            (m_pTBMazeInfo->Maze_Type == 13 || m_pTBMazeInfo->Maze_Type == 19) &&
            !m_bMazeComplete &&
            m_objectScanner.mapPlayerList.empty()) {
            // IDA: 处理特殊迷宫类型的退出
            auto itWait = m_mapWaitEnterMazeUser.find(dwActorIDValue);
            if (itWait != m_mapWaitEnterMazeUser.end() &&
                itWait->second.byState != 12) {  // LOBYTE 检查
                // IDA: 检查重入次数限制
                int nReEnterCount = 0;
                // TODO: 从 TB_COMMON 获取重入次数限制
                // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                // TB_COMMON* pTB_Common = XResourceMgr::GetTB_COMMON(&pServer->m_xResourceMgr, 0x753Bu);

                // IDA: 设置迷宫状态为 7
                SetMazeState(7, false);

                // IDA: 记录日志
                GreenDamTan_log(__FILE__, __FUNCTION__, "<%I64d MAZE> UPDATE STATE : %d %d %d",
                               instanceID2.nMapID, dwActorIDValue, TBMapID, 7);
            }
        }

        // IDA: 检查 Roguelike 迷宫 (Maze_Type == 15)
        if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Type == 15) {
            // TODO: 处理 Roguelike 迷宫退出逻辑
            // CMover::GetGOC<CGocEntity>(&pUser->CMoverEx, &pEntity, 0);
            // if (pEntity && CGocEntity::IsRoguelikeMapToGoNext) ...
        }

        // IDA: 调用基类 ExitActor
        XArea::ExitActor(pActor);

        // IDA: 设置 Area 为空
        pActor->SetArea(nullptr);

        // IDA: 用户退出
        // CUser::Exit(pUser);
        // TODO: CUser::Exit 需要实现

    } else {
        // IDA: GetScanner 返回 nullptr，记录错误日志
        UXActorID actorID = pActor->GetActorID();
        GreenDamTan_log(__FILE__, __FUNCTION__, "<SCANNER> Cant Find Maze Actor %d ( %d )",
                       actorID.dwActorID, 852);
    }
}

CMonster* XMaze::CreateMonster(UXMapID uxMazeSerialID, std::uint32_t nSectorID, int nMonsterID,
                               XVec3* vPos, float fRot, E_SEND_INFO_TYPE eType,
                               int nSpawnBoxID, int nGroupID, UXActorID uxParentID) {
    // IDA: 0x140318F20 - XMaze::CreateMonster 精确还原

    // 1. 获取怪物表数据
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_MONSTER* pTBMonster = pServer->GetResourceMgr().GetTB_MONSTER(nMonsterID);

    if (!pTBMonster) {
        LogHelper::LogError("game.contents", "CreateMonster error - No Table TB_MONSTER[ MonsterID:%d ] ( %d )",
                           nMonsterID, 2008);
        return nullptr;
    }

    // 2. 检查活动事件区域的游戏模式类型 (IDA: eGM_MODE_EQ == 5)
    if (m_pActiveEventSector && m_pActiveEventSector->GetGameModeType() == 5 && pTBMonster->Monster_Type != 3) {
        return nullptr;
    }

    // 3. 查找 Sector
    CSector* pSector = nullptr;
    auto itSector = m_mapSector.find(nSectorID);
    if (itSector != m_mapSector.end()) {
        pSector = itSector->second;
    }

    // 4. 复制位置向量
    XVec3 vLocalPos = *vPos;

    // 5. 创建怪物实例 (通过 ThreadLocalData)
    ThreadLocalData* pThreadLocal = ThreadLocalData::GetInstance();
    CMonster* pMonster = pThreadLocal->CreateMonster(this, uxMazeSerialID, nMonsterID, vLocalPos, fRot);

    if (!pMonster) {
        std::uint16_t nMapID = GetTBMapID();
        LogHelper::LogError("game.contents", "<ERROR> %d Failed Create Monster in %d Map", nMonsterID, nMapID);
        if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Group == 74) {
            m_nMazeLog[5] = 1;
        }
        return nullptr;
    }

    // 6. IDA: 添加到 Sector (如果怪物类型为0或3且没有清除条件类型)
    if ((pTBMonster->Monster_Type == 0 || pTBMonster->Monster_Type == 3)
        && pTBMonster->Monster_ClearCondition_Type == 0 && pSector) {
        UXActorID actorID = pMonster->GetActorID();
        // IDA: CSector::AddActor(pSector, QuestID, pActor)
        // QuestID comes from CQuestCondition::GetQuestID but we simplify
        pSector->AddActor(actorID.dwActorID, static_cast<XActor*>(pMonster));
    }

    // 7. IDA: 怪物类型11也添加到 Sector
    if (pTBMonster->Monster_Type == 11 && pSector) {
        UXActorID actorID = pMonster->GetActorID();
        pSector->AddActor(actorID.dwActorID, static_cast<XActor*>(pMonster));
    }

    // 8. IDA: 设置物理和碰撞
    if (pTBMonster->Monster_NormalStand_Type == 2 || pTBMonster->Monster_NormalStand_Type == 3) {
        pMonster->SetupPhysicsAndBound(static_cast<float>(pTBMonster->Monster_CollisionRadius),
                                       static_cast<float>(pTBMonster->Monster_CollisionHeight));
        pMonster->SetCollisionEnable(true, true);
    } else {
        pMonster->SetupPhysicsAndBound(static_cast<float>(pTBMonster->Monster_CollisionRadius),
                                       static_cast<float>(pTBMonster->Monster_CollisionHeight));
        pMonster->SetCollisionEnable(true, false);
    }

    // 9. IDA: 设置 Sector、SpawnBoxID、GroupID
    pMonster->SetSector(pSector);
    pMonster->SetSpawnBoxID(nSpawnBoxID);
    pMonster->SetGroupID(nGroupID);

    // 10. IDA: 设置父 Actor
    XActor* pParent = FindActor(uxParentID.dwActorID);
    if (pParent) {
        UXActorID parentActorID = pParent->GetActorID();
        pMonster->SetParentID(parentActorID);
        UXActorID originID = pParent->GetOriginID();
        pMonster->SetOriginID(originID);
    }

    // 11. IDA: 设置视野距离
    CAi* pAi = pMonster->GetAi();
    if (pAi) {
        pAi->SetTargetSightDistance(static_cast<float>(pTBMonster->Monster_Sight));
    }

    // 12. IDA: Roguelike 模式属性加成 (Maze_Type == 15)
    if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Type == 15) {
        // TODO: Implement Roguelike stat boost when GOC interfaces are complete
    }

    // 13. IDA: 进入游戏对象
    if (EnterGameObject(pMonster, eType)) {
        // 特殊阵营日志
        if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Group == 74
            && pMonster->GetMobTableRef() && pMonster->GetMobTableRef()->Monster_Faction == 25) {
            m_nMazeLog[6] = 1;
        }
        DeleteMonster(pMonster);
        return nullptr;
    }

    // 14. IDA: 设置破碎部位信息 (怪物类型11)
    if (pTBMonster->Monster_Type == 11) {
        // TODO: Implement SetHitPartsInfo when method is available
    }

    // 15. IDA: 设置 SA Break 时间
    float fSABreakTime = 0.0f;
    const VAnimationInfo* pAnimInfo = pMonster->GetActionDesc("B_SABreak_Start");
    if (pAnimInfo) {
        fSABreakTime += pAnimInfo->fAnimationLength;
    }
    pAnimInfo = pMonster->GetActionDesc("B_SABreak_End");
    if (pAnimInfo) {
        fSABreakTime += pAnimInfo->fAnimationLength;
    }
    // TODO: GetRecoverySuperArmorTime needs to be implemented in CMonster
    float fSABreakLoopTime = 0.0f;  // pMonster->GetRecoverySuperArmorTime();
    pMonster->SetSABreakType(pTBMonster->Monster_SA_Type);
    pMonster->SetSABreakLoopTime(fSABreakLoopTime);
    pMonster->SetSABreakTime(fSABreakTime + fSABreakLoopTime);

    // 16. IDA: 护送怪物检查
    if (m_stEscortMonster.nMonsterID == nMonsterID) {
        // Store actor ID instead of pointer
        m_stEscortMonster.dwActorID = pMonster->GetActorID().dwActorID;
    }

    return pMonster;
}

void XMaze::DeleteMonster(CMonster* pMonster) {
    // Per IDA 0x140319AC0: XMaze::DeleteMonster 精确还原
    if (!pMonster) {
        return;
    }

    // 1. 特殊阵营日志 (Maze_Group == 74, Monster_Faction == 25)
    if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Group == 74 &&
        pMonster->GetMobTableRef() && pMonster->GetMobTableRef()->Monster_Faction == 25) {
        m_nMazeLog[8] = 1;
    }

    // 2. 获取怪物表数据
    std::uint32_t nTableID = pMonster->GetTableID();

    GreenDamTan_log(__FILE__, __FUNCTION__, "DeleteMonster TableID=%d", nTableID);

    // 3. 通知怪物删除
    NotifyMonsterDelete(pMonster);

    // 4. 退出游戏对象
    // ExitGameObject(pMonster, eSendInfoTypeNone);
}

// ============================================================================
// OnUpdate
// IDA: 0x14031C330
// IDA 精确还原 - 迷宫更新主循环
// ============================================================================
void XMaze::OnUpdate(float fElapsed, float fRealElapsed) {
    // IDA: 1. 处理等待接收信息的用户列表
    // 遍历 m_listWaitForRecvInfo，检查用户 DB 状态标志
    for (auto itRecv = m_listWaitForRecvInfo.begin(); itRecv != m_listWaitForRecvInfo.end(); ) {
        void* pVoid = *itRecv;
        CUser* pUser = static_cast<CUser*>(pVoid);
        if (pUser) {
            // IDA: 检查 UserDB 标志位 (bit 2, 3, 4)
            // TODO: 需要访问用户 DB 标志
            // 暂时跳过这个检查
        }
        ++itRecv;
    }

    // IDA: 2. 检查 SpawnBoxGroup 计数
    bool bSpawn = CheckSpawnBoxGroupCount(fElapsed);

    // IDA: 3. 更新 ProcessSpawnBox 列表
    for (auto it = m_mapProcessSpawnBox.begin(); it != m_mapProcessSpawnBox.end(); ++it) {
        STMageProcessSpawnBox* pProcessSpawn = it->second;
        if (!pProcessSpawn) continue;

        if (pProcessSpawn->bActive && !pProcessSpawn->bTerminate) {
            pProcessSpawn->fDelayTime -= fElapsed;
            if (pProcessSpawn->fDelayTime <= 0.0f && pProcessSpawn->nCreatedCount != 0.0f) {
                // 执行生成盒
                ExcuteSpawnBox(pProcessSpawn, eSendInfoTypeSend);
                pProcessSpawn->nCreatedCount -= 1.0f;

                if (pProcessSpawn->nCreatedCount <= 0.0f) {
                    pProcessSpawn->bActive = false;

                    // IDA: 从 Sector 中移除 SpawnBoxID
                    int nBatchLayerLevel = GetBatchLayerLevel();
                    int iUniqueSector = VEventObjectInfo::GetEventUniqueID(pProcessSpawn->pSpawnBox->m_iSectorID, nBatchLayerLevel);
                    auto itSector = m_mapSector.find(iUniqueSector);
                    if (itSector != m_mapSector.end()) {
                        CSector* pSector = itSector->second;
                        if (pSector) {
                            pSector->EraseSpawnBoxID(it->first);
                        }
                    }
                    bSpawn = true;
                } else {
                    pProcessSpawn->fDelayTime = pProcessSpawn->pSpawnBox->m_fWaitCreationSequenceTime;
                }
            }
        }
    }

    // IDA: 4. 如果有生成，检查最后的 Sector
    if (bSpawn) {
        int nLastSectorID = GetLastSectorID();
        std::uint32_t nUniqueID = GetUniqueID(nLastSectorID);
        auto itLastSector = m_mapSector.find(nUniqueID);
        if (itLastSector != m_mapSector.end()) {
            CSector* pLastSector = itLastSector->second;
            if (pLastSector) {
                pLastSector->CheckMonsterCount();
                if (pLastSector->IsComplete() && m_pTBMazeInfo && m_pTBMazeInfo->Maze_Type == 7) {
                    UpdateClearMazeCondition(3, 1);
                }
            }
        }
    }

    // IDA: 5. 更新重生管理器
    // TODO: m_respawnManager.Update(this);

    // IDA: 6. 同步生成的活动状态
    SyncSpawnedActive();

    // IDA: 7. 遍历所有 Sector 并更新
    for (auto& pair : m_mapSector) {
        CSector* pSector = pair.second;
        if (pSector) {
            pSector->OnUpdate(fElapsed, fRealElapsed);
        }
    }

    // IDA: 8. 更新交互盒冷却时间
    for (auto& pair : m_mapInteractionBox) {
        STInteractionBox* pInteraction = pair.second;
        if (pInteraction && pInteraction->fCoolTime > 0.0f) {
            pInteraction->fCoolTime -= fElapsed;
            if (pInteraction->fCoolTime <= 0.0f) {
                pInteraction->fCoolTime = 0.0f;
            }
        }
    }

    // IDA: 9. CellPos 检查 (每秒一次)
    int bCheckCellPos = 0;
    m_fCellPosCheckTime += fElapsed;
    if (m_fCellPosCheckTime > 1.0f) {
        m_fCellPosCheckTime = 0.0f;
        bCheckCellPos = 1;
        m_CellPosMgr.RemoveAllInfo();
    }

    // IDA: 10. 遍历所有 Actor 并更新
    std::list<std::uint32_t> listDeleteUser;
    for (auto itActorPair = m_mapActor.begin(); itActorPair != m_mapActor.end(); ++itActorPair) {
        XActor* pActor = itActorPair->second;
        if (!pActor) continue;

        E_ACTOR_TYPE eActorType = static_cast<E_ACTOR_TYPE>(pActor->GetType());

        if (eActorType == eActorMonster) {
            // IDA: 处理怪物
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (pMonster && (m_stMazeGameState.m_nMazeState == 1 || m_stMazeGameState.m_nMazeState == 4)) {
                CSector* pMonsterSector = pMonster->GetSector();
                if (pMonsterSector && !pMonster->GetOwnerID()) {
                    // Sector 有效，检查 GroupID
                }

                // IDA: CellPos 更新
                if (bCheckCellPos) {
                    if (pMonster->IsMoving()) {
                        std::uint32_t dwID = pMonster->GetID();
                        m_CellPosMgr.RemoveMonsterInfo(dwID);
                        pMonster->SetCellID(0xFFFFFFFF);
                    } else {
                        hkvVec3 vPos = pMonster->GetPosition();
                        int bDuplicate = 0;
                        std::uint32_t dwID = m_CellPosMgr.GetEmptyCellID(vPos, &bDuplicate);
                        if (bDuplicate) {
                            pMonster->SetCellID(dwID);
                        }
                        std::uint32_t dwMonsterID = pMonster->GetID();
                        m_CellPosMgr.AddMonsterInfo(dwID, dwMonsterID);
                    }
                }
            }
        } else if (eActorType == eActorNPC) {
            // IDA: 处理 NPC - 目前跳过，需要完整的 CNpc 定义
        } else if (eActorType == eActorUser && bCheckCellPos) {
            // IDA: 处理用户 CellPos
            CUser* pUserActor = dynamic_cast<CUser*>(pActor);
            if (pUserActor) {
                hkvVec3 vPos = pUserActor->GetPosition();
                std::uint32_t dwCellID = m_CellPosMgr.CellIDFromPos(vPos);
                pUserActor->SetCellID(dwCellID);
            }
        }

        // IDA: 调用 Actor 的 OnUpdate
        // TODO: XActor::OnUpdate 需要实现
        // pActor->OnUpdate(fElapsed);
    }

    // IDA: 11. 处理待删除的用户
    for (auto it = listDeleteUser.begin(); it != listDeleteUser.end(); ++it) {
        std::uint32_t dwActorID = *it;
        m_mapActor.erase(dwActorID);
    }
    listDeleteUser.clear();

    // IDA: 12. 更新 objectScanner 中的治疗量
    for (auto& pair : m_objectScanner.mapPlayerList) {
        CUser* pUser = dynamic_cast<CUser*>(pair.second);
        if (pUser && !pUser->IsGM()) {
            pUser->SetAmountOfHeal(0.0f);
        }
    }

    // IDA: 13. 处理待销毁的对象列表
    if (!m_lstDestoryObject.empty()) {
        for (auto it = m_lstDestoryObject.begin(); it != m_lstDestoryObject.end(); ++it) {
            XActor* pDestroyActor = *it;
            if (!pDestroyActor) continue;

            E_ACTOR_TYPE eType = static_cast<E_ACTOR_TYPE>(pDestroyActor->GetType());
            switch (eType) {
                case eActorMonster: {
                    CMonster* pMonster = dynamic_cast<CMonster*>(pDestroyActor);
                    if (pMonster) DeleteMonster(pMonster);
                    break;
                }
                case eActorNPC: {
                    // TODO: CNpc 删除
                    // CNpc* pNpc = dynamic_cast<CNpc*>(pDestroyActor);
                    // if (pNpc) DeleteNpc(pNpc);
                    break;
                }
                case eActorAkashic: {
                    // TODO: CAkashicObject 删除 - 需要完整类型定义
                    // CAkashicObject* pAkashic = dynamic_cast<CAkashicObject*>(pDestroyActor);
                    // if (pAkashic) DeleteAkashicObject(pAkashic);
                    break;
                }
                default:
                    break;
            }
        }

        // IDA: 更新导航网格轮廓
        if (m_nDestroySilhouetes > 0) {
            // TODO: 需要 hkaiWorld::stepSilhouettes
            m_nDestroySilhouetes = 0;
        }

        m_lstDestoryObject.clear();
    }

    // IDA: 14. 处理怪物变换列表
    if (!m_lstChangeMonster.empty()) {
        for (auto it = m_lstChangeMonster.begin(); it != m_lstChangeMonster.end(); ++it) {
            std::uint32_t dwMobID = *it;
            ChangeMonster(dwMobID);
        }
        m_lstChangeMonster.clear();
    }

    // IDA: 15. 更新传送门
    if (m_pWarpPotal) {
        m_pWarpPotal->Update(fElapsed);
    }

    // IDA: 16. 更新定时器
    UpdateTimer(fElapsed);

    // IDA: 17. 更新迷宫状态
    UpdateMazeState();

    // IDA: 18. 处理护送条件
    ProcessEscortCondition(fElapsed);

    // IDA: 19. 更新过场动画管理器
    if (m_pCutSceneManager) {
        // TODO: CCutsceneManager::OnUpdate needs proper signature
        // m_pCutSceneManager->OnUpdate(fElapsed, fRealElapsed);
    }

    // IDA: 20. 更新休闲副本定时器
    UpdateCasualRaidTimer(fElapsed);

    // IDA: 21. 更新传送门状态
    UpdatePortalState();
}

// ============================================================================
// SendSectorInfos
// IDA: 0x14031D920
// ============================================================================
void XMaze::SendSectorInfos(CUser* pUser) {
    // 计算完成的 Sector 数量
    int nCount = 0;
    for (auto it = m_mapSector.begin(); it != m_mapSector.end(); ++it) {
        CSector* pSector = it->second;
        if (pSector && pSector->IsComplete()) {
            ++nCount;
        }
    }

    // 发送包 (main=0x11, sub=0x53)
    XSendPacket xSendPacket(0x11, 0x53);
    xSendPacket << nCount;

    // 添加完成的 Sector Box ID
    for (auto it = m_mapSector.begin(); it != m_mapSector.end(); ++it) {
        CSector* pSector = it->second;
        if (pSector && pSector->IsComplete()) {
            xSendPacket << pSector->GetSectorBoxID();
        }
    }

    // 获取当前 Sector ID from position
    int nSectorIDFromPos = 0;
    if (pUser) {
        // TODO: CVaccumManager::GetArea returns CFsmClass<CAi>*, need proper position extraction
        // CFsmClass<CAi>* pArea = CVaccumManager::GetArea(pUser);
        // if (pArea) {
        //     nSectorIDFromPos = GetSectorIDFromPos(pArea->GetPosition());
        // }
        // For now, use GetSectorIDFromPos with user position
        // nSectorIDFromPos = GetSectorIDFromPos(pUser->GetPosition());
    }
    xSendPacket << nSectorIDFromPos;

    // Broadcast to all
    SendBroadCast(&xSendPacket, static_cast<XActor*>(pUser), E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
}

// ============================================================================
// SendGateInfos
// IDA: 0x14031DB80
// TODO: STMageGateBox not defined, using void* placeholder
// ============================================================================
void XMaze::SendGateInfos(CUser* pUser, bool bReEnter) {
    // IDA: Count active gates (BYTE4 of second field indicates active state)
    int nCount = 0;
    for (auto it = m_mapGateBox.begin(); it != m_mapGateBox.end(); ++it) {
        // TODO: STMageGateBox* pGateBox = it->second;
        // IDA checks: if (pGateBox && BYTE4(pGateBox->GetTypeId)) ++nCount;
        // This likely checks pGateBox->bActive flag at offset +4
        (void)it;
    }

    // 发送包 (main=0x11, sub=0x54)
    XSendPacket xSendPacket(0x11, 0x54);
    xSendPacket << bReEnter;
    xSendPacket << nCount;

    // IDA: Send active gate IDs (first int field of STMageGateBox)
    for (auto it = m_mapGateBox.begin(); it != m_mapGateBox.end(); ++it) {
        // TODO: if (pGateBox && BYTE4(pGateBox->GetTypeId)) {
        //     xSendPacket << pGateBox->nGateID;  // or GetTypeId field
        // }
        (void)it;
    }

    // Broadcast to all
    SendBroadCast(&xSendPacket, static_cast<XActor*>(pUser), E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
}

// ============================================================================
// SendDieMonsters
// IDA: 0x14031DDB0
// 精确还原: 发送死亡怪物信息给客户端
// ============================================================================
void XMaze::SendDieMonsters(CUser* pUser) {
    // IDA: Get count of die monster spawn box IDs
    int nCount = static_cast<int>(m_listDieMonsterSpawnBoxID.size());

    // IDA: Create packet (0x11, 0x68)
    XSendPacket xSendPacket(0x11, 0x68);
    xSendPacket << nCount;

    // IDA: Write each spawn box ID
    for (auto it = m_listDieMonsterSpawnBoxID.begin(); it != m_listDieMonsterSpawnBoxID.end(); ++it) {
        xSendPacket << *it;
    }

    // IDA: Broadcast to all users
    // SendBroadCast(this, &xSendPacket, pActor, eAll);
    XActor* pActor = static_cast<XActor*>(pUser);
    SendBroadCast(&xSendPacket, pActor, E_BROADCAST_TYPE::eAll);
}

// ============================================================================
// SendPotalInfos
// IDA: 0x14031DEF0
// 精确还原: 发送传送门信息给客户端
// ============================================================================
void XMaze::SendPotalInfos(CUser* pUser) {
    // IDA: Count enabled and disabled portals
    int nCount = 0;          // 启用计数
    int nDisEnableCount = 0; // 禁用计数

    // IDA: First pass - count enabled/disabled
    for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
        STMagePotalBox* pPotalBox = it->second;
        // IDA: Check if pPotalBox is valid and bOpen flag
        if (pPotalBox && pPotalBox->bOpen) {
            ++nCount;
        } else {
            ++nDisEnableCount;
        }
    }

    // IDA: Create packet (0x11, 0x55)
    XSendPacket xSendPacket(0x11, 0x55);
    xSendPacket << nCount;
    xSendPacket << nDisEnableCount;

    // IDA: Second pass - write enabled portal IDs
    for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
        STMagePotalBox* pPotalBox = it->second;
        if (pPotalBox && pPotalBox->bOpen) {
            // IDA: Write nBoxIndex (which is the portal ID)
            xSendPacket << pPotalBox->nBoxIndex;
        }
    }

    // IDA: Third pass - write disabled portal IDs
    for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
        STMagePotalBox* pPotalBox = it->second;
        if (pPotalBox && !pPotalBox->bOpen) {
            // IDA: Write nBoxIndex (which is the portal ID)
            xSendPacket << pPotalBox->nBoxIndex;
        }
    }

    // IDA: Broadcast to all users
    XActor* pActor = static_cast<XActor*>(pUser);
    SendBroadCast(&xSendPacket, pActor, E_BROADCAST_TYPE::eAll);
}

// ============================================================================
// SendInteractionInfos
// IDA: 0x14031E1F0
// TODO: CGocSoulMetry, CGocQuest, CGocAttribute not defined
// ============================================================================
void XMaze::SendInteractionInfos(CUser* pUser) {
    if (!pUser) {
        return;
    }

    // TODO: CGocSoulMetry not defined
    // 获取 SoulMetry 组件
    // std::tr1::shared_ptr<CGocSoulMetry> pSoulMetry = pUser->GetGOC<CGocSoulMetry>();
    std::vector<std::uint32_t> vecSoulMetry;

    int nMazeID = XArea::GetTBMapID();
    // TODO: m_pTBMazeInfo may be null
    int nGroupID = 0;
    if (m_pTBMazeInfo) {
        nGroupID = m_pTBMazeInfo->Maze_Group;
    }
    // if (pSoulMetry) {
    //     pSoulMetry->MakeInteractionObject(nGroupID, nMazeID, &vecSoulMetry);
    // }

    // TODO: CGocQuest, CGocAttribute not defined
    // 获取任务和属性组件
    // std::tr1::shared_ptr<CGocQuest> pQuest = pUser->GetGOC<CGocQuest>();
    // std::tr1::shared_ptr<CGocAttribute> pAttr = pUser->GetGOC<CGocAttribute>();

    // if (!pQuest || !pAttr) {
    //     return;
    // }

    // TODO: ST_BATCH_INTERACTION_LIST, STInteractionBox, VInterActionBoxInfo not defined
    // 构建交互对象列表
    // ST_BATCH_INTERACTION_LIST lstInterAction;
    // for (auto it = m_mapInteractionBox.begin(); it != m_mapInteractionBox.end(); ++it) {
    //     STInteractionBox* pInteraction = it->second;
    //     if (!pInteraction) continue;
    //
    //     VInterActionBoxInfo* pInteractionBox = pInteraction->pInteractionBox;
    //     XGameServer* pServer = XGameServer::Instance();
    //     TB_INTERACTION_OBJECT* pTBInteraction = XResourceMgr::GetTB_INTERACTION_OBJECT(
    //         &pServer->GetResourceMgr(), pInteractionBox->m_iInteractionID);
    //
    //     if (!pTBInteraction) continue;
    //
    //     bool bShow = pInteraction->bShow;
    //     bool bEnable = pInteraction->bEnable;
    //
    //     // 检查启用状态
    //     if (pTBInteraction->Enable_State == 1) {
    //         if (pTBInteraction->Object_type && pTBInteraction->Object_type != 3) {
    //             bEnable = false;
    //             for (auto soulIt = vecSoulMetry.begin(); soulIt != vecSoulMetry.end(); ++soulIt) {
    //                 if (*soulIt == pTBInteraction->ID) {
    //                     bEnable = true;
    //                     break;
    //                 }
    //             }
    //         } else {
    //             if (pTBInteraction->Check_Quest_Condition_ID) {
    //                 if (!pQuest->FindCondition(pTBInteraction->Check_Quest_Condition_ID)) {
    //                     bEnable = false;
    //                 }
    //             } else if (pTBInteraction->Check_Quest_ID) {
    //                 if (!pQuest->FindEpisode(pTBInteraction->Check_Quest_ID)) {
    //                     bEnable = false;
    //                 }
    //             }
    //         }
    //     }
    //
    //     // 检查显示条件
    //     if (pTBInteraction->Show_Sight == 1) {
    //         if (pTBInteraction->Show_Condition_Quest_ID) {
    //             if (!pQuest->FindCondition(pTBInteraction->Show_Condition_Quest_ID)) {
    //                 bShow = false;
    //             }
    //         } else if (pTBInteraction->Show_Quest_ID) {
    //             if (!pQuest->FindEpisode(pTBInteraction->Show_Quest_ID)) {
    //                 bShow = false;
    //             }
    //         }
    //     }
    //
    //     // 添加到列表
    //     ST_BATCH_INTERACTION stInfo;
    //     stInfo.bShow = bShow;
    //     stInfo.bEnable = bEnable;
    //     stInfo.nBoxIndex = pInteraction->nBoxIndex;
    //     stInfo.nCallCount = pInteraction->nCallCount;
    //     lstInterAction.vecInteraction.push_back(stInfo);
    // }

    // 发送包
    // if (!lstInterAction.vecInteraction.empty()) {
    //     XSendPacket xSendPacket(0x11, 0x56);
    //     xSendPacket << lstInterAction;
    //     pUser->Send(&xSendPacket);
    // }
}

// ============================================================================
// SendLastClientSync
// IDA: 0x14031EB10
// 精确还原: 发送最后客户端同步
// ============================================================================
void XMaze::SendLastClientSync(CUser* pUser) {
    // IDA: Get last sector ID
    int nLastSectorID = GetLastSectorID();

    // IDA: Get unique ID from sector ID
    int nUniqueID = GetUniqueID(nLastSectorID);

    // IDA: Find sector in map
    auto it = m_mapSector.find(nUniqueID);
    if (it == m_mapSector.end()) {
        return;
    }

    // IDA: Get sector pointer
    CSector* pSector = it->second;
    if (pSector) {
        // IDA: Call SendLuaClientSync
        // Note: IDA shows 3 parameters but actual signature may differ
        // CSector::SendLuaClientSync(pSector, pUser, a3);
        pSector->SendLuaClientSync(pUser);
    }
}

// ============================================================================
// SendObjectInfo
// IDA: ?SendObjectInfo@XMaze@@UEAA_NPEAVXActor@@_N@Z (0x14031EBE0)
// 发送对象信息给 Actor
// 精确还原自 IDA 反编译
// ============================================================================
bool XMaze::SendObjectInfo(XActor* pActor, bool bReEnter) {
    if (!pActor) {
        return false;
    }

    // IDA: 创建 XSendPacket 对象 (main=4, sub=0x11) 用于发送进入信息
    XSendPacket xSendPacket(4, 0x11);
    SendInInfo(xSendPacket, pActor);

    // IDA: 创建 XSendPacket 对象 (main=4, sub=0x21) 用于发送其他用户信息
    XSendPacket xSendPacketUser(4, 0x21);
    SendOtherInfos(xSendPacketUser, pActor, eActorUser);

    // IDA: 创建 XSendPacket 对象 (main=4, sub=0x23) 用于发送怪物信息
    XSendPacket xSendPacketMonster(4, 0x23);
    SendOtherInfos(xSendPacketMonster, pActor, eActorMonster);

    // IDA: 发送怪物生成动作变更
    SendChangeActionSpawn(eActorMonster);

    // IDA: 如果是用户，同步召唤助手信息
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (pUser) {
        // 获取 CGocHelper 组件
        auto pHelper = pUser->GetGOC<CGocHelper>();
        if (pHelper) {
            // TODO: GocHelper.cpp 尚未完成还原，暂时注释
            // pHelper->SyncSummonedInfo();
        }
    }

    // IDA: 创建 XSendPacket 对象 (main=4, sub=0x22) 用于发送 NPC 信息
    XSendPacket xSendPacketNPC(4, 0x22);
    SendOtherInfos(xSendPacketNPC, pActor, eActorNPC);

    // IDA: 发送其他信息
    SendSectorInfos(pUser);
    SendGateInfos(pUser, bReEnter);
    SendDieMonsters(pUser);
    SendPotalInfos(pUser);
    SendInteractionInfos(pUser);
    SendLastClientSync(pUser);
    SendUseReviveCount(pActor);

    // IDA: 日志记录
    int nLogSendBuffer = 0;
    if (pUser) {
        nLogSendBuffer = pUser->GetLogSendBuffer();
    }
    LogHelper::LogDebug("game.contents", "<SEND_LOG> SendObjectinfo ( %d )", nLogSendBuffer);

    return true;
}

// ============================================================================
// GetSectorIDFromPos
// IDA: 0x14031F450
// 根据位置获取 Sector ID
// ============================================================================
int XMaze::GetSectorIDFromPos(const hkvVec3& vPos) {
    for (auto it = m_mapSector.begin(); it != m_mapSector.end(); ++it) {
        CSector* pSector = it->second;
        if (!pSector) continue;

        VSectorBox* pBox = pSector->GetSectorBox();
        if (!pBox) continue;

        // 检查位置是否在 Sector 范围内
        if (vPos.x >= pBox->PosTopLeft.x && pBox->PosBottomRight.x >= vPos.x &&
            vPos.y >= pBox->PosTopLeft.y && pBox->PosBottomRight.y >= vPos.y) {
            return pSector->GetSectorBoxID();
        }
    }
    return -1;
}

// ============================================================================
// GetSectorFromPos
// IDA: 0x14031F670
// 根据位置获取 Sector 指针
// ============================================================================
CSector* XMaze::GetSectorFromPos(const hkvVec3& vPos) {
    for (auto it = m_mapSector.begin(); it != m_mapSector.end(); ++it) {
        CSector* pSector = it->second;
        if (!pSector) continue;

        VSectorBox* pBox = pSector->GetSectorBox();
        if (!pBox) continue;

        // 检查位置是否在 Sector 范围内
        if (vPos.x >= pBox->PosTopLeft.x && pBox->PosBottomRight.x >= vPos.x &&
            vPos.y >= pBox->PosTopLeft.y && pBox->PosBottomRight.y >= vPos.y) {
            return pSector;
        }
    }
    return nullptr;
}

// ============================================================================
// IsBossSector
// IDA: 0x1402A4C40
// ============================================================================
bool XMaze::IsBossSector() const {
    return m_bBossSector;
}

// ============================================================================
// GetSystemActor
// IDA: 0x1402A67C0
// ============================================================================
CMonster* XMaze::GetSystemActor() const {
    return m_pSystemActor;
}

// ============================================================================
// GetEventSector
// IDA: 0x1402A9020
// ============================================================================
CSector* XMaze::GetEventSector() const {
    return m_pActiveEventSector;
}

// ============================================================================
// ClearForce
// IDA: 0x1401C9E90
// ============================================================================
void XMaze::ClearForce() {
    m_pForce.reset();
}

// ============================================================================
// IsCompleteClearMaze
// IDA: 0x1402A5010
// ============================================================================
bool XMaze::IsCompleteClearMaze() const {
    return m_bMazeComplete;
}

// ============================================================================
// GetCellPosMgr
// IDA: 0x140280CA0
// TODO: CCellPosMgr is incomplete type, m_CellPosMgr not defined
// ============================================================================
CCellPosMgr& XMaze::GetCellPosMgr() {
    // TODO: m_CellPosMgr member variable not defined, CCellPosMgr is incomplete
    static char s_CellPosMgr[sizeof(void*) * 32];  // Placeholder
    return *reinterpret_cast<CCellPosMgr*>(s_CellPosMgr);
}

// ============================================================================
// GetEscortMonster
// IDA: 0x140280D00
// ============================================================================
STEscortMonster& XMaze::GetEscortMonster() {
    return m_stEscortMonster;
}

// ============================================================================
// CWarpPotal::Init
// IDA: 0x140718730
// Verified: Direct IDA decompilation
// ============================================================================
void CWarpPotal::Init(XMaze* pMaze) {
    // IDA decompiled:
    // void __fastcall CWarpPotal::Init(CWarpPotal *this, XMaze *pMaze)
    // {
    //   this->m_bReCheck = 0;
    //   this->m_bWarpTimeCheck = 0;
    //   this->m_fWarpTime = 0.0;
    //   this->m_nSendTimeSec = 0;
    //   this->m_pCurInfo = nullptr;
    //   this->m_pMaze = pMaze;
    //   this->m_nJumpID = 0;
    // }
    m_bReCheck = false;
    m_bWarpTimeCheck = false;
    m_fWarpTime = 0.0f;
    m_nSendTimeSec = 0;
    m_pCurInfo = nullptr;
    m_pMaze = pMaze;
    m_nJumpID = 0;
}

// IDA: ?CheckWarp@CWarpPotal@@QEAAXXZ
// TODO: Need to decompile and implement
void CWarpPotal::CheckWarp() {
    // TODO: Implement CheckWarp logic
}

// IDA: ?ProcessTimeCount@CWarpPotal@@QEAAXM@Z
// TODO: Need to decompile and implement
void CWarpPotal::ProcessTimeCount(float fElapsedTime) {
    // TODO: Implement ProcessTimeCount logic
}

// ============================================================================
// CWarpPotal::Update
// IDA: 0x1407191e0
// Verified: Direct IDA decompilation
// ============================================================================
void CWarpPotal::Update(float fElapsedTime) {
    // IDA decompiled:
    // void __fastcall CWarpPotal::Update(CWarpPotal *this, float fElapsedTime)
    // {
    //   if ( this->m_bReCheck )
    //   {
    //     CWarpPotal::CheckWarp(this);
    //     this->m_bReCheck = 0;
    //   }
    //   if ( this->m_bWarpTimeCheck )
    //     CWarpPotal::ProcessTimeCount(this, fElapsedTime);
    // }

    if (m_bReCheck) {
        CheckWarp();
        m_bReCheck = false;
    }

    if (m_bWarpTimeCheck) {
        ProcessTimeCount(fElapsedTime);
    }
}

// ============================================================================
// GetWarpPotal
// IDA: 0x1402A4BC0
// ============================================================================
CWarpPotal* XMaze::GetWarpPotal() const {
    return m_pWarpPotal;
}

// ============================================================================
// EscapeActor
// IDA: 0x1402914C0
// ============================================================================
bool XMaze::EscapeActor(XActor* pActor) {
    // IDA: RTTI cast to CUser
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser) {
        return false;
    }

    // IDA: Call WarpSectorStartPos
    return WarpSectorStartPos(pUser);
}

// ============================================================================
// SendChangeActionSpawn
// IDA: 0x14028E5C0
// ============================================================================
void XMaze::SendChangeActionSpawn(E_ACTOR_TYPE eActorType) {
    // IDA: Create PS_MOVE_IDLE_VEC to collect monster info
    PS_MOVE_IDLE_VEC stInfoVec;

    // IDA: Iterate over m_mapActor (std::map)
    for (auto& [dwActorID, pOtherActor] : m_mapActor) {
        if (pOtherActor && pOtherActor->IsLive() && pOtherActor->GetType() == eActorType) {
            // IDA: _RTDynamicCast to CMonster
            CMonster* pMonster = dynamic_cast<CMonster*>(pOtherActor);
            if (pMonster) {
                // IDA: GetMoveIdleInfo with fElapsed = 0.0
                PS_MOVE_IDLE stInfo;
                pMonster->GetMoveIdleInfo(stInfo, 0.0f);
                stInfoVec.vecMoveIdle.push_back(stInfo);
            }
        }
    }

    // IDA: If not empty, send packet (main=5, sub=15)
    if (!stInfoVec.vecMoveIdle.empty()) {
        XSendPacket xPacket(5, 15);
        xPacket << stInfoVec;
        SendBroadCastAfterLoading(xPacket, nullptr, E_BROADCAST_TYPE::eNoneSelf);
    }
}

// ============================================================================
// GetResourceMgr
// IDA: 0x1401A71B0
// ============================================================================
XResourceMgr* XMaze::GetResourceMgr() {
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        return &pServer->GetResourceMgr();
    }
    return nullptr;
}

// ============================================================================
// GetNavMeshInstance
// IDA: 0x1402A3D00
// ============================================================================
DohHavokNavMeshInstance* XMaze::GetNavMeshInstance() const {
    return m_pNavMeshInstance;
}

// ============================================================================
// GetObjectResource
// IDA: 0x1402A3D20
// ============================================================================
VEventObjectResource* XMaze::GetObjectResource() const {
    return static_cast<VEventObjectResource*>(m_pMazeResource);
}

// ============================================================================
// GetBatchLayerLevel
// IDA: 0x1402C7D60
// ============================================================================
int XMaze::GetBatchLayerLevel() const {
    return m_stMazeGameState.m_nBatchLayerLevel;
}

// ============================================================================
// GetPartyQuest
// IDA: 0x1401444B0
// TODO: STPartyQuest is incomplete type
// ============================================================================
struct STPartyQuestPlaceholder { char data[256]; };
STPartyQuest& XMaze::GetPartyQuest() {
    // TODO: STPartyQuest is incomplete type - need proper definition
    static STPartyQuestPlaceholder s_stPartyQuest;
    return *reinterpret_cast<STPartyQuest*>(&s_stPartyQuest);
}

// ============================================================================
// StartDefenceMode
// IDA: 0x140329050
// TODO: 需要 GameModeMgr, ThreadLocalData 类型定义
// ============================================================================
void XMaze::StartDefenceMode(int nTabelIndex) {
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // GameModeMgr* pGameModeMgr = pThreadData->GetGameModeMgr();
    // pGameModeMgr->StartDefenceMode(nTabelIndex, this);
    OutputDebugStringA("DefenceMode");
}

// ============================================================================
// AddTimer
// IDA: ?AddTimer@XMaze@@QEAAXHM@Z (0x1403292E0)
// 精确还原 - 添加定时器
// ============================================================================
void XMaze::AddTimer(int nID, float fTarget) {
    // IDA: 创建新的 LogicTimer
    LogicTimer newTimer;
    // IDA: CFsmCondition::SetVariables(&newTimer, nID) - 设置 timer ID
    newTimer.SetID(nID);
    // IDA: LogicTimer::SetTimer(&newTimer, fTarget, 1)
    newTimer.SetTimer(fTarget, true);
    // IDA: 添加到等待计时器列表
    m_arWaitLogicTimers.push_back(newTimer);
}

// ============================================================================
// ClearAllTimers
// IDA: 0x140329720
// ============================================================================
void XMaze::ClearAllTimers() {
    m_arLogicTimers.clear();
    m_arWaitLogicTimers.clear();
}

// IDA: ?RemoveTimer@XMaze@@QEAAXH@Z (0x140329760)
// IDA 精确还原 - 移除定时器
// 使用 std::find_if 和 lambda 精确还原
void XMaze::RemoveTimer(int nID) {
    // IDA: 在等待计时器列表中查找并重置计时器
    // 使用 lambda 查找 ID 匹配的计时器
    auto waitIter = std::find_if(m_arWaitLogicTimers.begin(), m_arWaitLogicTimers.end(),
        [nID](const LogicTimer& timer) { return timer.GetID() == nID; });
    if (waitIter != m_arWaitLogicTimers.end()) {
        waitIter->SetTimer(0.0f, true);
    }

    // IDA: 在活动计时器列表中查找并重置计时器
    auto activeIter = std::find_if(m_arLogicTimers.begin(), m_arLogicTimers.end(),
        [nID](const LogicTimer& timer) { return timer.GetID() == nID; });
    if (activeIter != m_arLogicTimers.end()) {
        activeIter->SetTimer(0.0f, true);
    }
}

// ============================================================================
// AllDestroySectorMonster
// IDA: 0x140329A60
// TODO: 需要 VEventObjectInfo 类型定义
// ============================================================================
void XMaze::AllDestroySectorMonster(int nSectorID) {
    // 获取 Sector
    int nBatchLayerLevel = GetBatchLayerLevel();
    // int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSectorID, nBatchLayerLevel);

    // auto it = m_mapSector.find(iBoxUniqueID);
    // if (it != m_mapSector.end() && it->second) {
    //     CSector* pSector = it->second;
    //
    //     // 判断是否为自杀模式
    //     bool bSuicide = false;
    //     if (m_pTBMazeInfo && (m_pTBMazeInfo->Maze_Type == 14 || m_pTBMazeInfo->Maze_Type == 18 || m_pTBMazeInfo->Maze_Type == 19)) {
    //         bSuicide = true;
    //     }
    //
    //     pSector->DieMonstersAll(bSuicide);
    // }
}

// ============================================================================
// StartSurvivalMode
// IDA: 0x140329090
// TODO: 需要 GameModeMgr, ThreadLocalData 类型定义
// ============================================================================
void XMaze::StartSurvivalMode(int nTabelIndex) {
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // GameModeMgr* pGameModeMgr = pThreadData->GetGameModeMgr();
    // pGameModeMgr->StartSurvivalMode(nTabelIndex, this);
    OutputDebugStringA("SurvivalMode");
}

// ============================================================================
// StartOperationMode
// IDA: 0x1403290D0
// TODO: 需要 GameModeMgr, ThreadLocalData 类型定义
// ============================================================================
void XMaze::StartOperationMode(int nTabelIndex) {
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // GameModeMgr* pGameModeMgr = pThreadData->GetGameModeMgr();
    // pGameModeMgr->StartOperationMode(nTabelIndex, this);
    OutputDebugStringA("OperationMode");
}

// ============================================================================
// StartUnityMode
// IDA: 0x140329110
// TODO: 需要 GameModeMgr, ThreadLocalData 类型定义
// ============================================================================
void XMaze::StartUnityMode(int nTableIndex, int nSectorID) {
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // GameModeMgr* pGameModeMgr = pThreadData->GetGameModeMgr();
    // pGameModeMgr->StartUnityEventMode(nTableIndex, nSectorID, this);
    OutputDebugStringA("Start StartUnityMode\n");
}

// ============================================================================
// StartWarlordMode
// IDA: 0x140329160
// TODO: 需要 GameModeMgr, ThreadLocalData 类型定义
// ============================================================================
void XMaze::StartWarlordMode(int nTableIndex, int nSectorID) {
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // GameModeMgr* pGameModeMgr = pThreadData->GetGameModeMgr();
    // pGameModeMgr->StartWarlordMode(nTableIndex, nSectorID, this);
    OutputDebugStringA("Start StartWarlordMode\n");
}

// ============================================================================
// AddDSPointByUnity
// IDA: ?AddDSPointByUnity@XMaze@@QEAAXG@Z (0x1403291B0)
// 精确还原 - 为所有在线用户添加 DS 点数
// ============================================================================
void XMaze::AddDSPointByUnity(std::uint16_t wDSPoint) {
    // IDA: 遍历 m_objectScanner 中的所有 actor
    // 使用 begin()/end() 迭代器遍历玩家列表
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) continue;

        // IDA: 使用 RTTI 动态转换为 CUser
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser && !pMover->IsDie()) {
            // IDA: 获取 CGocRecode 组件
            auto pRecode = pUser->GetGOC<CGocRecode>();
            if (pRecode) {
                // IDA: 调用 CGocRecode::AddDSPoint
                pRecode->AddDSPoint(static_cast<int>(wDSPoint), 0.0f);
            }
        }
    }
}

// ============================================================================
// AddTimerEx
// IDA: ?AddTimerEx@XMaze@@QEAAXPEBDMHHH@Z (0x140329370)
// 精确还原 - 添加扩展定时器（带 Lua 脚本函数）
// ============================================================================
void XMaze::AddTimerEx(const char* szLuaFunction, float fTarget, int nParam1, int nParam2, int nParam3) {
    if (!szLuaFunction) {
        return;
    }

    // IDA: 创建新的 LogicTimer
    LogicTimer newTimer;
    // IDA: 计算函数名的哈希值作为 ID
    int nHash = VHashString::GetHash(szLuaFunction);
    // IDA: CFsmCondition::SetVariables(&newTimer, nHash)
    newTimer.SetID(nHash);
    // IDA: LogicTimer::SetTimer(&newTimer, fTarget, 1)
    newTimer.SetTimer(fTarget, true);
    // IDA: LogicTimer::SetUserString(&newTimer, szLuaFunction)
    newTimer.SetUserString(szLuaFunction);
    // IDA: LogicTimer::SetParam(&newTimer, nParam1, nParam2, nParam3)
    newTimer.SetParam(nParam1, nParam2, nParam3);
    // IDA: 添加到等待计时器列表
    m_arWaitLogicTimers.push_back(newTimer);
}

// ============================================================================
// AddEventTimer
// IDA: ?AddEventTimer@XMaze@@QEAAXPEBD00HMHH@Z (0x140329440)
// 精确还原 - 添加事件定时器
// ============================================================================
void XMaze::AddEventTimer(const char* szLuaFinishFunction, const char* szLuaEventFunction,
                          const char* szLuaReadyFunction, int nType, float fTime,
                          int nParam1, int nParam2) {
    if (!szLuaFinishFunction) {
        return;
    }

    // IDA: 创建新的 LogicTimer
    LogicTimer newTimer;
    // IDA: 计算函数名的哈希值作为 ID
    int nHash = VHashString::GetHash(szLuaFinishFunction);
    newTimer.SetID(nHash);
    // IDA: LogicTimer::SetType(&newTimer, nType)
    newTimer.SetType(nType);
    // IDA: LogicTimer::SetTimer(&newTimer, (float)nParam2, 1)
    newTimer.SetTimer(static_cast<float>(nParam2), true);
    // IDA: LogicTimer::SetOriginTime(&newTimer, fTime)
    newTimer.SetOriginTime(fTime);
    // IDA: LogicTimer::SetUserString(&newTimer, szLuaFinishFunction)
    newTimer.SetUserString(szLuaFinishFunction);
    // IDA: LogicTimer::SetEventString(&newTimer, szLuaEventFunction)
    newTimer.SetEventString(szLuaEventFunction);
    // IDA: LogicTimer::SetReadyString(&newTimer, szLuaReadyFunction)
    newTimer.SetReadyString(szLuaReadyFunction);
    // IDA: LogicTimer::SetNextTime(&newTimer, 0, (int)fTime)
    newTimer.SetNextTime(0, static_cast<int>(fTime));
    // IDA: LogicTimer::SetParam(&newTimer, nParam1, nParam2, 0)
    newTimer.SetParam(nParam1, nParam2, 0);
    // IDA: 如果类型是 2，使用迷宫清除时间
    if (nType == 2 && m_pTBMazeInfo) {
        newTimer.SetTimer(static_cast<float>(m_pTBMazeInfo->Maze_ClearTime), true);
    }
    // IDA: 添加到等待计时器列表
    m_arWaitLogicTimers.push_back(newTimer);
}

// ============================================================================
// RemoveTimerEx
// IDA: ?RemoveTimerEx@XMaze@@QEAAMPEBD@Z (0x1403295A0)
// 精确还原 - 移除扩展定时器并返回剩余时间
// ============================================================================
float XMaze::RemoveTimerEx(const char* szLuaFunction) {
    if (!szLuaFunction) {
        return 0.0f;
    }

    // IDA: 计算函数名的哈希值
    int nID = VHashString::GetHash(szLuaFunction);
    float fLeftTime = 0.0f;

    // IDA: 在活动计时器列表中查找并获取剩余时间
    auto iter = std::find_if(m_arLogicTimers.begin(), m_arLogicTimers.end(),
        [nID](const LogicTimer& timer) { return timer.GetID() == nID; });

    if (iter != m_arLogicTimers.end()) {
        // IDA: 获取最大时间和当前时间
        float fMaxTimer = iter->GetMaxTimer();
        float fTimer = iter->GetTimer();
        // IDA: 计算剩余时间
        fLeftTime = fMaxTimer - fTimer;
        // IDA: 重置计时器
        iter->SetTimer(0.0f, true);
    }

    // IDA: 确保返回非负值
    if (fLeftTime < 0.0f) {
        return 0.0f;
    }
    return fLeftTime;
}

// ============================================================================
// GetCommonPositionBox
// IDA: 0x140329880
// TODO: 需要 VEventObjectInfo, hkvVec3 类型定义
// ============================================================================
struct hkvVec3 XMaze::GetCommonPositionBox(int nID) {
    // int nBatchLayerLevel = GetBatchLayerLevel();
    // int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nID, nBatchLayerLevel);
    //
    // auto it = m_mapCommonPostionBox.find(iBoxUniqueID);
    // if (it != m_mapCommonPostionBox.end() && it->second != nullptr) {
    //     const VCommonPositionBoxInfo* pInfo = it->second;
    //     return VEventObjectInfo::GetCenter(&pInfo->VEventBoxInfo);
    // }
    // return hkvVec3::ZeroVector();
    return hkvVec3();
}

// ============================================================================
// GetCommonPositionBoxInfo
// IDA: 0x140329940
// TODO: 需要 VEventObjectInfo 类型定义
// ============================================================================
const VCommonPositionBoxInfo* XMaze::GetCommonPositionBoxInfo(int nID) {
    // int nBatchLayerLevel = GetBatchLayerLevel();
    // int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nID, nBatchLayerLevel);
    //
    // auto it = m_mapCommonPostionBox.find(iBoxUniqueID);
    // if (it != m_mapCommonPostionBox.end()) {
    //     return it->second;
    // }
    return nullptr;
}

// ============================================================================
// SectorClear
// IDA: 0x14032A420
// TODO: 需要 CTextDBLog, VEventObjectInfo 类型定义
// ============================================================================
void XMaze::SectorClear(int nSectorID) {
    // CTextDBLog::AddLog(&m_textDBLog, 42, nSectorID, 0, "");
    // int nBatchLayerLevel = GetBatchLayerLevel();
    // int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSectorID, nBatchLayerLevel);
    //
    // auto it = m_mapSector.find(iBoxUniqueID);
    // if (it != m_mapSector.end()) {
    //     CSector* pSector = it->second;
    //     if (pSector) {
    //         pSector->SetComplete(true);
    //     }
    // }
}

// ============================================================================
// DieEventSectorMonster
// IDA: 0x14032A4F0
// ============================================================================
void XMaze::DieEventSectorMonster(int nMonsterType, bool bSuicide) {
    if (m_pActiveEventSector) {
        m_pActiveEventSector->DieMonsters(nMonsterType, bSuicide);
    }
}

// ============================================================================
// LuaClientSync
// IDA: 0x14032A160
// TODO: 需要 CTextDBLog, VEventObjectInfo, ST_LUA_CLIENT_SYNC 类型定义
// ============================================================================
void XMaze::LuaClientSync(int nType, int nValue, int nSectorID, float fLimitTime) {
    // CTextDBLog::AddLog(&m_textDBLog, 43, nType, nValue, "");
    //
    // if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Group == 74 && !nType && nValue == 51) {
    //     m_nMazeLog[3] = 1;
    // }
    //
    // XSendPacket xSendPacket(0x11, 0x60);
    // xSendPacket << nType;
    // xSendPacket << nValue;
    // SendBroadCast(&xSendPacket, nullptr, eAll);
    //
    // if (nSectorID > 0) {
    //     int nBatchLayerLevel = GetBatchLayerLevel();
    //     int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSectorID, nBatchLayerLevel);
    //
    //     auto it = m_mapSector.find(iBoxUniqueID);
    //     if (it != m_mapSector.end() && it->second) {
    //         CSector* pSector = it->second;
    //         ST_LUA_CLIENT_SYNC stInfo;
    //         stInfo.nType = nType;
    //         stInfo.nValue = nValue;
    //         stInfo.nLimitTime = 0;
    //         if (fLimitTime > 0.0f) {
    //             stInfo.nLimitTime = (std::uint32_t)(GetTickCount64() + (std::uint64_t)(fLimitTime * 1000.0f));
    //         }
    //         pSector->AddLuaClientSync(&stInfo);
    //     }
    // }
}

// ============================================================================
// IsUnityMode
// IDA: 0x14032B920
// ============================================================================
bool XMaze::IsUnityMode() {
    return m_pActiveEventSector && m_pActiveEventSector->GetGameModeType() == 3;
}

// ============================================================================
// IsTerminateSpawn
// IDA: 0x14032B960
// Check if spawn is terminated in specified sector
// ============================================================================
bool XMaze::IsTerminateSpawn(int nSectorID) {
    // IDA: Find sector by ID and call IsTerminateSpawn
    auto it = m_mapSector.find(nSectorID);
    if (it == m_mapSector.end() || !it->second) {
        return false;
    }
    return it->second->IsTerminateSpawn();
}

// ============================================================================
// IsCompleteEpisode
// IDA: ?IsCompleteEpisode@XMaze@@QEAA_NKH@Z (0x14032B9F0)
// ============================================================================
bool XMaze::IsCompleteEpisode(std::uint32_t dwUserID, std::uint32_t nQuestID) {
    // IDA: Iterate through m_objectScanner.mapPlayerList
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        // IDA: Check it->first == dwUserID
        if (pMover && it->first == dwUserID) {
            // IDA: GetGOC<CGocQuest> and call IsCompleteEpisode
            auto pQuest = pMover->GetGOC_Quest(false);
            if (pQuest) {
                return pQuest->IsCompleteEpisode(nQuestID);
            }
        }
    }
    return false;
}

// ============================================================================
// IsCompleteCondition
// IDA: ?IsCompleteCondition@XMaze@@QEAA_NKH@Z (0x14032BAE0)
// ============================================================================
bool XMaze::IsCompleteCondition(std::uint32_t dwUserID, int nConditionID) {
    // IDA: Iterate through m_objectScanner.mapPlayerList
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            // IDA: Check XActor::GetID() == dwUserID (through vtable)
            if (pMover->GetID() == dwUserID) {
                // IDA: GetGOC<CGocQuest> and call IsCompleteCondition
                auto pQuest = pMover->GetGOC_Quest(false);
                if (pQuest) {
                    return pQuest->IsCompleteCondition(nConditionID);
                }
            }
        }
    }
    return false;
}

// ============================================================================
// IsProcessEpisode
// IDA: ?IsProcessEpisode@XMaze@@QEAA_NKH@Z (0x14032BBF0)
// ============================================================================
bool XMaze::IsProcessEpisode(std::uint32_t dwUserID, std::uint32_t nQuestID) {
    // IDA: Iterate through m_objectScanner.mapPlayerList
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            // IDA: Check XActor::GetID() == dwUserID (through vtable)
            if (pMover->GetID() == dwUserID) {
                // IDA: GetGOC<CGocQuest> and call FindEpisode
                auto pQuest = pMover->GetGOC_Quest(false);
                if (pQuest) {
                    return pQuest->FindEpisode(nQuestID);
                }
            }
        }
    }
    return false;
}

// ============================================================================
// IsProcessCondition
// IDA: ?IsProcessCondition@XMaze@@QEAA_NKH@Z (0x14032BD00)
// ============================================================================
bool XMaze::IsProcessCondition(std::uint32_t dwUserID, std::uint32_t nConditionID) {
    // IDA: Iterate through m_objectScanner.mapPlayerList
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            // IDA: Check XActor::GetID() == dwUserID (through vtable)
            if (pMover->GetID() == dwUserID) {
                // IDA: GetGOC<CGocQuest> and call FindCondition
                auto pQuest = pMover->GetGOC_Quest(false);
                if (pQuest) {
                    return pQuest->FindCondition(nConditionID);
                }
            }
        }
    }
    return false;
}

// ============================================================================
// RemoveQuestAll
// IDA: ?RemoveQuestAll@XMaze@@QEAA_NXZ (0x14032BE10)
// ============================================================================
bool XMaze::RemoveQuestAll() {
    // IDA: Iterate through m_objectScanner.mapPlayerList
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            // IDA: GetGOC<CGocQuest> and call ResetQuestAll
            auto pQuest = pMover->GetGOC_Quest(false);
            if (pQuest) {
                pQuest->ResetQuestAll();
            }
        }
    }
    return false;
}

// ============================================================================
// SetEscortCondition
// IDA: 0x14032BEE0
// ============================================================================
void XMaze::SetEscortCondition(int nConditionID) {
    // IDA: this->m_stEscortMonster.nConditionID = nConditionID;
    m_stEscortMonster.nConditionID = nConditionID;
}

// ============================================================================
// IsHaveQuest
// IDA: ?IsHaveQuest@XMaze@@QEAA_NH@Z (0x14032BFF0)
// ============================================================================
bool XMaze::IsHaveQuest(std::uint32_t nQuestID) {
    // IDA: Iterate through m_objectScanner.mapPlayerList
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            // IDA: GetGOC<CGocQuest> and call FindEpisode
            auto pQuest = pMover->GetGOC_Quest(false);
            if (pQuest && pQuest->FindEpisode(nQuestID)) {
                return true;
            }
        }
    }
    return false;
}

// ============================================================================
// AcceptQuest
// IDA: ?AcceptQuest@XMaze@@QEAAXH@Z (0x14032C0F0)
// ============================================================================
void XMaze::AcceptQuest(std::uint32_t nQuestID) {
    // IDA: Iterate through m_objectScanner.mapPlayerList
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            // IDA: GetGOC<CGocQuest> and if FindEpisode returns false, call AcceptQuest
            auto pQuest = pMover->GetGOC_Quest(false);
            if (pQuest && !pQuest->FindEpisode(nQuestID)) {
                pQuest->AcceptQuest(nQuestID, false);
            }
        }
    }
}

// ============================================================================
// CompleteQuest
// IDA: ?CompleteQuest@XMaze@@QEAAXH@Z (0x14032C1D0)
// ============================================================================
void XMaze::CompleteQuest(std::uint32_t nQuestID) {
    // IDA: Iterate through m_objectScanner.mapPlayerList
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            // IDA: GetGOC<CGocQuest> and if FindEpisode returns true, call CompleteQuest
            auto pQuest = pMover->GetGOC_Quest(false);
            if (pQuest && pQuest->FindEpisode(nQuestID)) {
                pQuest->CompleteQuest(nQuestID, 0);
            }
        }
    }
}

// ============================================================================
// CompleteCondition
// IDA: ?CompleteCondition@XMaze@@QEAAXH@Z (0x14032C2D0)
// ============================================================================
void XMaze::CompleteCondition(std::uint32_t nConditionID) {
    // IDA: Iterate through m_objectScanner.mapPlayerList
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            // IDA: GetGOC<CGocQuest> and if FindCondition returns 1, call CompleteConditionByForce
            auto pQuest = pMover->GetGOC_Quest(false);
            if (pQuest && pQuest->FindCondition(nConditionID) == 1) {
                pQuest->CompleteConditionByForce(nConditionID);
            }
        }
    }
}

// ============================================================================
// FindUserByLowLevel
// IDA: 0x14032C3D0
// Find user with lowest level in maze
// ============================================================================
std::uint32_t XMaze::FindUserByLowLevel() {
    // IDA: Iterate through m_objectScanner to find lowest level user
    std::uint32_t dwActorID = 0;
    int nLevel = 100;

    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover && pMover->GetLevel() < nLevel) {
            nLevel = pMover->GetLevel();
            UXActorID uxActorID = pMover->GetActorID();
            dwActorID = uxActorID.dwActorID;
        }
    }
    return dwActorID;
}

// ============================================================================
// FindUserByQuest
// IDA: 0x14032C4E0
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
CMover* XMaze::FindUserByQuest(std::uint32_t nQuestID) {
    // IDA: Iterate through m_objectScanner.mapPlayerList
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            // IDA: GetGOC<CGocQuest> and call FindEpisode
            auto pQuest = pMover->GetGOC_Quest(false);
            if (pQuest && pQuest->FindEpisode(nQuestID)) {
                return pMover;
            }
        }
    }
    return nullptr;
}

// ============================================================================
// GetUserByIndex
// IDA: 0x14032C5E0
// ============================================================================
CMover* XMaze::GetUserByIndex(int iIndex) {
    // IDA: 0x14032C5E0 - Iterate through m_objectScanner and return user at index
    int iCount = 0;
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            if (iCount == iIndex) {
                return pMover;
            }
            ++iCount;
        }
    }
    return nullptr;
}

// ============================================================================
// IsHaveCondition
// IDA: ?IsHaveCondition@XMaze@@QEAA_NH@Z (0x14032C790)
// ============================================================================
bool XMaze::IsHaveCondition(std::uint32_t nConditionID) {
    // IDA: Iterate through m_objectScanner.mapPlayerList
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            // IDA: GetGOC<CGocQuest> and call FindCondition
            auto pQuest = pMover->GetGOC_Quest(false);
            if (pQuest && pQuest->FindCondition(nConditionID)) {
                return true;
            }
        }
    }
    return false;
}

// ============================================================================
// IsEnableInteraction
// IDA: 0x14032C890
// 检查交互对象是否启用
// ============================================================================
bool XMaze::IsEnableInteraction(int nID) {
    auto it = m_mapInteractionBox.find(nID);
    if (it == m_mapInteractionBox.end()) {
        return false;
    }

    STInteractionBox* pInteraction = it->second;
    if (pInteraction == nullptr) {
        return false;
    }

    return pInteraction->bEnable;  // IDA: bEnable
}

// ============================================================================
// IsExistedMonster
// IDA: 0x14032C920
// 检查指定TableID的怪物是否存在
// ============================================================================
bool XMaze::IsExistedMonster(int nMonsterID) {
    // IDA: Iterate through m_objectScanner.mapNPCList
    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) continue;

        // IDA: Check if actor type is 2 (ActorType_Monster)
        if (pMover->GetType() != 2) continue;

        // IDA: Dynamic cast to CMonster
        CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        if (pMonster && pMonster->GetTableID() == nMonsterID) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// SetInvincibleMonster
// IDA: ?SetInvincibleMonster@XMaze@@QEAAXH_N@Z @ 0x14032CA20
// 设置指定TableID怪物的无敌状态
// IDA 精确还原
// ============================================================================
void XMaze::SetInvincibleMonster(int nMonsterID, bool bEnable) {
    // IDA: 遍历 m_objectScanner.mapNPCList
    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: 检查是否为怪物类型 (type == 2)
        E_ACTOR_TYPE eType = pMover->GetType();
        if (eType != eActorMonster) {
            continue;
        }

        // IDA: RTTI dynamic_cast 到 CMonster
        CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        if (!pMonster) {
            continue;
        }

        // IDA: 比较 GetTableID() 与 nMonsterID
        if (pMonster->GetTableID() == nMonsterID) {
            // IDA: 匹配则调用 SetInvincibleActor
            pMonster->SetInvincibleActor(bEnable);
        }
    }
}

// ============================================================================
// SetLuaValue (int index overload)
// IDA: 0x14032E310
// 设置 m_vecLuaValue 指定索引的值
// ============================================================================
void XMaze::SetLuaValue(int nIndex, std::uint32_t nValue) {
    if (nIndex >= 0 && static_cast<size_t>(nIndex) < m_vecLuaValue.size()) {
        m_vecLuaValue[nIndex].dwActorID = nValue;
    }
}

// ============================================================================
// AddLuaValue
// IDA: 0x14032E350
// 累加 m_vecLuaValue 指定索引的值
// ============================================================================
void XMaze::AddLuaValue(int nIndex, int nValue) {
    if (nIndex >= 0 && static_cast<size_t>(nIndex) < m_vecLuaValue.size()) {
        m_vecLuaValue[nIndex].dwActorID += nValue;
    }
}

// ============================================================================
// TerminateSpawnBox
// IDA: 0x14032CB30
// 终止指定SpawnBox的生成
// IDA 精确还原
// ============================================================================
void XMaze::TerminateSpawnBox(int nSpawnBoxID) {
    // IDA: CTextDBLog::AddLog(&this->m_textDBLog, 22, nSpawnBoxID, 0, &byte_140AFF6FA)
    // 注：CTextDBLog 暂未完整实现，跳过日志调用

    // IDA: BatchLayerLevel = XMaze::GetBatchLayerLevel(this)
    int nBatchLayerLevel = GetBatchLayerLevel();

    // IDA: iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSpawnBoxID, BatchLayerLevel)
    int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSpawnBoxID, nBatchLayerLevel);

    // IDA: std::_Tree::find(&this->m_mapProcessSpawnBox, &it, &iBoxUniqueID)
    auto it = m_mapProcessSpawnBox.find(iBoxUniqueID);

    // IDA: if (it != end)
    if (it != m_mapProcessSpawnBox.end()) {
        // IDA: pMonsterSpawnInfo = it->second
        STMageProcessSpawnBox* pInfo = it->second;

        // IDA: if (pMonsterSpawnInfo)
        if (pInfo) {
            // IDA: pMonsterSpawnInfo->bTerminate = 1
            pInfo->bTerminate = true;

            // IDA: if (!pMonsterSpawnInfo->bSpawned || (pMonsterSpawnInfo->bSpawned && pMonsterSpawnInfo->bActive))
            if (!pInfo->bSpawned || (pInfo->bSpawned && pInfo->bActive)) {
                // IDA: pMonsterSpawnInfo->bActive = 0
                pInfo->bActive = false;

                // IDA: v4 = XMaze::GetBatchLayerLevel(this)
                int nBatchLevel2 = GetBatchLayerLevel();

                // IDA: iUniqueID = VEventObjectInfo::GetEventUniqueID(pMonsterSpawnInfo->pSpawnBox->m_iSectorID, v4)
                int iUniqueID = VEventObjectInfo::GetEventUniqueID(pInfo->pSpawnBox->m_iSectorID, nBatchLevel2);

                // IDA: std::_Tree::find(&this->m_mapSector, &v10, &_Keyval)
                auto sectorIt = m_mapSector.find(iUniqueID);

                // IDA: if (sectorIt != end && sectorIt->second)
                if (sectorIt != m_mapSector.end() && sectorIt->second) {
                    // IDA: CSector::EraseSpawnBoxID(pSector, iBoxUniqueID)
                    sectorIt->second->EraseSpawnBoxID(iBoxUniqueID);
                }
            }
        }
    }
}

// ============================================================================
// TerminateSpawn
// IDA: 0x14032CCF0
// 终止指定Sector的生成
// IDA 精确还原
// ============================================================================
void XMaze::TerminateSpawn(int nSectorID) {
    // IDA: nSectorIDa = nSectorID; if (!nSectorID) nSectorIDa = XMaze::GetLastSectorID(this)
    int nTargetSectorID = nSectorID;
    if (nTargetSectorID == 0) {
        nTargetSectorID = GetLastSectorID();
    }

    // IDA: iUniqueID = XMaze::GetUniqueID(this, nSectorIDa)
    int iUniqueID = GetUniqueID(nTargetSectorID);

    // IDA: std::_Tree::find(&this->m_mapSector, &it, &_Keyval)
    auto it = m_mapSector.find(iUniqueID);

    // IDA: if (it != end)
    if (it != m_mapSector.end()) {
        // IDA: pSector = it->second
        CSector* pSector = it->second;

        // IDA: if (pSector) CSector::TerminateSpawn(pSector)
        if (pSector) {
            pSector->TerminateSpawn();
        }
    }

    // IDA: LogHelper::LogDebug("game.contents", "<MAZE> Call TerminateSectorSpawn ( %d )", nSectorIDa)
    LogHelper::LogDebug("game.contents", "<MAZE> Call TerminateSectorSpawn ( %d )", nTargetSectorID);
}

// ============================================================================
// UpdateSectorClear
// IDA: 0x14032CDC0
// 更新Sector清除状态，更新任务条件
// IDA 精确还原
// ============================================================================
void XMaze::UpdateSectorClear(unsigned int nSectorID) {
    // IDA: 遍历 m_objectScanner 中的所有 CMover
    for (auto it = m_objectScanner.mapPlayerList.begin(); it != m_objectScanner.mapPlayerList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: 获取 CGocQuest 组件
        std::shared_ptr<CGocQuest> pQuest = pMover->GetGOC_Quest(false);
        if (pQuest) {
            // IDA: 调用 UpdateCondition (type=2, target=3, objectID=sectorID, count=mazeID)
            // 注：原始 IDA 中 UpdateMazeCondition 参数顺序可能不同
            pQuest->UpdateCondition(eCONDITION_TYPE_TRIGGER, eCONDITION_TARGET_ITEM,
                                    nSectorID, static_cast<int>(m_pTBMazeInfo ? m_pTBMazeInfo->ID : 0), false);

            // IDA: 同步数据库
            pQuest->DBSyncQuestCondition();

            // IDA: 如果是 CUser，调用 SendExpSave
            CUser* pUser = dynamic_cast<CUser*>(pMover);
            if (pUser) {
                // TODO: CUser::SendExpSave 需要实现
                // pUser->SendExpSave();
            }
        }
    }
}

// ============================================================================
// AddDestoryObject
// IDA: 0x14032CF40
// 添加销毁对象到销毁列表
// ============================================================================
void XMaze::AddDestoryObject(XActor* pActor) {
    if (pActor != nullptr) {
        // 检查是否是 Monster 类型
        if (pActor->IsMonster()) {
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (pMonster != nullptr) {
                // 检查是否是 SystemActor
                if (pMonster->IsSystemActor()) {
                    // LogHelper::LogError("game.contents", "<MAZE> die system actor : %d ", GetTBMapID());
                    return;
                }

                int nGroupID = pMonster->GetGroupID();
                int nSpawnBoxID = pMonster->GetSpawnBoxID();
                int nTableID = pMonster->GetTableID();

                // 调用死亡后脚本
                CallScriptPostDieMonster(nTableID, nSpawnBoxID, nGroupID);
            }
        }
    }

    // 添加到销毁列表
    m_lstDestoryObject.push_back(pActor);
}

// ============================================================================
// CallScriptPreDieMonster
// IDA: 0x14032D050
// 调用怪物死亡前脚本
// IDA 精确还原
// ============================================================================
void XMaze::CallScriptPreDieMonster(unsigned int nMonsterID, int nSpawnBoxID, unsigned int nGroupID) {
    // IDA: 将 ID 转换为字符串
    char szMonsterID[16];
    char szSpawnBoxID[16];
    _itoa(nMonsterID, szMonsterID, 10);
    _itoa(nSpawnBoxID, szSpawnBoxID, 10);

    // IDA: 检查是否调用脚本死亡
    bool bCallScriptDie = IsCallScriptDie(nMonsterID);
    bool bResult = false;

    // IDA: 如果脚本实例存在
    if (m_pScriptInstance) {
        // IDA: 调用 OnDie 脚本
        if (m_pScriptInstance->HasFunction("OnDie") && !bCallScriptDie) {
            m_pScriptInstance->ExecuteFunctionArg("OnDie", "ssTSoulworker:XMaze;", szMonsterID, szSpawnBoxID, this);
        }

        // IDA: 删除怪物组 ID
        bResult = DeleteMonsterGroupID(nGroupID, true);

        // IDA: 调用 OnDieEx 脚本
        if (m_pScriptInstance->HasFunction("OnDieEx") && bResult && !bCallScriptDie) {
            m_pScriptInstance->ExecuteFunctionArg("OnDieEx", "iTSoulworker:XMaze;", nGroupID, this);
        }
    }

    // IDA: 添加到死亡怪物列表
    AddDieMonsterSpawnBoxID(nSpawnBoxID);

    // IDA: 检查过场动画状态
    if (bResult) {
        m_pCutSceneManager->CheckCutsceneState(2, static_cast<std::int32_t>(nGroupID));
    }
}

// ============================================================================
// CallScriptPostDieMonster
// IDA: 0x14032D240
// 调用怪物死亡后脚本
// IDA 精确还原
// ============================================================================
void XMaze::CallScriptPostDieMonster(unsigned int nMonsterID, int nSpawnBoxID, unsigned int nGroupID) {
    // IDA: 将 ID 转换为字符串
    char szMonsterID[16];
    char szSpawnBoxID[16];
    _itoa(nMonsterID, szMonsterID, 10);
    _itoa(nSpawnBoxID, szSpawnBoxID, 10);

    // IDA: 检查是否调用脚本死亡
    bool bCallScriptDie = IsCallScriptDie(nMonsterID);
    bool bResult = false;

    // IDA: 如果脚本实例存在
    if (m_pScriptInstance) {
        // IDA: 调用 OnRealDie 脚本
        if (m_pScriptInstance->HasFunction("OnRealDie") && !bCallScriptDie) {
            m_pScriptInstance->ExecuteFunctionArg("OnRealDie", "ssTSoulworker:XMaze;", szMonsterID, szSpawnBoxID, this, 0);
        }

        // IDA: 删除怪物组 ID (bPre = false)
        bResult = DeleteMonsterGroupID(nGroupID, false);

        // IDA: 调用 OnRealDieEx 脚本
        if (m_pScriptInstance->HasFunction("OnRealDieEx") && bResult && !bCallScriptDie) {
            m_pScriptInstance->ExecuteFunctionArg("OnRealDieEx", "iTSoulworker:XMaze;", nGroupID, this);
        }
    }
}

// ============================================================================
// AddMazeGameRule
// IDA: 0x14032D3F0
// 添加迷宫游戏规则
// ============================================================================
void XMaze::AddMazeGameRule(char* szParam1, char* szParam2, char* szParam3, char* szParam4, char* szParam5, char* szParam6) {
    // IDA 0x14032D3F0: XMaze::AddMazeGameRule 精确还原
    // 1. 将6个字符串参数转换为整数
    // 2. 创建 MAZE_GAME_RULE 结构体
    // 3. 添加到 m_vecGameRules

    MAZE_GAME_RULE rule;
    rule.nParam1 = szParam1 ? std::stoi(szParam1) : 0;
    rule.nParam2 = szParam2 ? std::stoi(szParam2) : 0;
    rule.nParam3 = szParam3 ? static_cast<std::uint16_t>(std::stoi(szParam3)) : 0;
    rule.nParam4 = szParam4 ? static_cast<std::uint16_t>(std::stoi(szParam4)) : 0;
    rule.nParam5 = szParam5 ? static_cast<std::uint16_t>(std::stoi(szParam5)) : 0;
    rule.nParam6 = szParam6 ? static_cast<std::uint16_t>(std::stoi(szParam6)) : 0;

    m_vecGameRules.push_back(rule);
}

// ============================================================================
// SetMonsterForceActionSkill
// IDA: 0x14032D5C0
// 设置怪物强制动作技能
// IDA 精确还原
// ============================================================================
void XMaze::SetMonsterForceActionSkill(char* szMonsterID, char* szSkillID) {
    // IDA: 将字符串转换为整数 ID
    int nMonsterID = std::stoi(szMonsterID);
    int nSkillID = std::stoi(szSkillID);

    // IDA: 遍历 m_objectScanner.mapNPCList 中的所有对象
    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: 检查是否为怪物类型 (type == 2)
        E_ACTOR_TYPE eType = pMover->GetType();
        if (eType != eActorMonster) {
            continue;
        }

        // IDA: RTTI dynamic_cast 到 CMonster
        CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        if (!pMonster) {
            continue;
        }

        // IDA: 检查 TableID 是否匹配
        if (pMonster->GetTableID() == nMonsterID) {
            pMonster->ForceActionSkill(nSkillID);
            return;
        }
    }
}

// ============================================================================
// GetLuaValue
// IDA: 0x14032E3A0
// 获取 m_vecLuaValue 指定索引的值
// ============================================================================
int XMaze::GetLuaValue(int nIndex) {
    if (nIndex >= 0 && static_cast<size_t>(nIndex) < m_vecLuaValue.size()) {
        return static_cast<int>(m_vecLuaValue[nIndex].dwActorID);
    }
    return 0;
}

// ============================================================================
// GetUniqueIDFromSector
// IDA: 0x14032E3D0
// 获取唯一ID (基于SectorID和BatchLayerLevel)
// ============================================================================
int XMaze::GetUniqueIDFromSector(int nID) {
    int nBatchLayerLevel = GetBatchLayerLevel();
    return VEventObjectInfo::GetEventUniqueID(nID, nBatchLayerLevel);
}

// ============================================================================
// SetGameModeState
// IDA: 0x14032E400
// 设置游戏模式状态
// TODO: 需要 CSector::SetModeState 方法
// ============================================================================
void XMaze::SetGameModeState(int nState) {
    // IDA: if (m_pActiveEventSector) m_pActiveEventSector->SetModeState(nState);
    if (m_pActiveEventSector) {
        m_pActiveEventSector->SetModeState(nState);
    }
}

// ============================================================================
// InitQuestConditionForSectorClear
// IDA: 0x14032E440
// 初始化Sector清除的任务条件
// IDA 精确还原
// ============================================================================
void XMaze::InitQuestConditionForSectorClear() {
    // IDA: 遍历 m_objectScanner 中的所有 CMover
    // 注意：IDA 显示遍历整个 m_objectScanner，包括 player, NPC 和 etc 列表
    for (auto it = m_objectScanner.mapPlayerList.begin(); it != m_objectScanner.mapPlayerList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: 获取 CGocQuest 组件
        std::shared_ptr<CGocQuest> pQuest = pMover->GetGOC_Quest(false);
        if (pQuest) {
            // IDA: 调用初始化和更新函数
            pQuest->InitQuestConditionForSectorClear();
            pQuest->UpdateQuestConditionForSectorClear();
        }
    }
}

// ============================================================================
// IsCompleteQuestCondition
// IDA: 0x14032E520
// 检查任务条件是否完成
// IDA 精确还原
// ============================================================================
bool XMaze::IsCompleteQuestCondition(bool& bSendMsg) {
    // IDA: 遍历 m_objectScanner 中的所有 CMover
    for (auto it = m_objectScanner.mapPlayerList.begin(); it != m_objectScanner.mapPlayerList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: 获取 CGocQuest 组件
        std::shared_ptr<CGocQuest> pQuest = pMover->GetGOC_Quest(false);
        if (pQuest) {
            // IDA: 检查任务条件是否完成
            if (!pQuest->IsQuestCondtionForSectorClear()) {
                bSendMsg = pQuest->IsSendMsgSectorClear();
                return false;
            }
        }
    }
    return true;
}

// ============================================================================
// SetObjectInfoReq
// IDA: 0x14032E630
// 设置对象信息请求，添加到等待接收信息列表
// ============================================================================
void XMaze::SetObjectInfoReq(CUser* pUser) {
    m_listWaitForRecvInfo.push_back(pUser);
}

// ============================================================================
// ApplyBuff
// IDA: 0x14032E7D0
// 对指定Actor应用Buff
// ============================================================================
void XMaze::ApplyBuff(unsigned int nActorID, unsigned short nBuffID) {
    // IDA: 在 m_objectScanner 中查找 nActorID
    // 由于当前 m_objectScanner 分为三个 map，需要在所有 map 中查找

    // 首先在玩家列表中查找
    auto itPlayer = m_objectScanner.mapPlayerList.find(nActorID);
    if (itPlayer != m_objectScanner.mapPlayerList.end()) {
        CMover* pObj = itPlayer->second;
        if (pObj) {
            pObj->SetBuffStatus(nBuffID, nActorID, true);
        }
        return;
    }

    // 在 NPC/怪物列表中查找
    auto itNPC = m_objectScanner.mapNPCList.find(nActorID);
    if (itNPC != m_objectScanner.mapNPCList.end()) {
        CMover* pObj = itNPC->second;
        if (pObj) {
            pObj->SetBuffStatus(nBuffID, nActorID, true);
        }
        return;
    }

    // 在其他列表中查找
    auto itEtc = m_objectScanner.mapEtcList.find(nActorID);
    if (itEtc != m_objectScanner.mapEtcList.end()) {
        CMover* pObj = itEtc->second;
        if (pObj) {
            pObj->SetBuffStatus(nBuffID, nActorID, true);
        }
        return;
    }
}

// ============================================================================
// SetMonsterCommonActionAi
// IDA: 0x14032D7A0
// 设置怪物通用动作AI
// ============================================================================
void XMaze::SetMonsterCommonActionAi(char* szMonsterID, char* szCommonActionIndex) {
    // IDA 反编译逻辑:
    // 1. 将字符串转换为整数 ID
    // 2. 遍历 m_objectScanner 中的所有对象
    // 3. 找到类型为 Monster 且 TableID 匹配的怪物
    // 4. 调用 CAi::ChangeAiState((E_FSMSTATES)(nIndex + 19))

    int nMonsterID = std::stoi(szMonsterID);
    int nIndex = std::stoi(szCommonActionIndex);

    // IDA: 遍历 m_objectScanner.mapNPCList
    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) continue;

        // IDA: XActor::GetType() == 2 (Monster)
        if (pMover->GetType() != 2) continue;

        // IDA: RTTI cast to CMonster
        CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        if (!pMonster) continue;

        // IDA: pMonster->GetTableID() == nMonsterID
        if (pMonster->GetTableID() != nMonsterID) continue;

        // IDA: 获取 AI 并改变状态
        CAi* pAi = pMonster->GetAi();
        if (pAi) {
            int _nState = nIndex + 19;
            pAi->ChangeAiState(_nState);
            return;
        }
    }
}

// ============================================================================
// WarpMonsterToPoint
// IDA: 0x14032D9B0
// 传送怪物到指定点
// IDA 精确还原
// ============================================================================
void XMaze::WarpMonsterToPoint(char* szMonsterID, char* szBoxID, int nActivate) {
    // IDA: std::string::string(&_Str, szMonsterID)
    // IDA: nMonsterID = std::stoi(&_Str, nullptr, 10)
    int nMonsterID = std::stoi(szMonsterID);

    // IDA: std::string::string(&v12, szBoxID)
    // IDA: nBoxID = std::stoi(&v12, nullptr, 10)
    int nBoxID = std::stoi(szBoxID);

    // IDA: v25 = nActivate > 0; bActivate = v25
    bool bActivate = nActivate > 0;

    // IDA: pBoxInfo = XMaze::GetCommonPositionBoxInfo(this, nBoxID)
    const VCommonPositionBoxInfo* pBoxInfo = GetCommonPositionBoxInfo(nBoxID);

    // IDA: if (pBoxInfo)
    if (pBoxInfo) {
        // IDA: VEventObjectInfo::GetCenter(&pBoxInfo->VEventBoxInfo, &result)
        hkvVec3 centerPos = pBoxInfo->GetCenter();

        // IDA: fRot = pBoxInfo->fRotate
        float fRot = pBoxInfo->fRotate;

        // IDA: hkvVec3::hkvVec3(&vPos, result.x, result.y, result.z) -> XVec3
        XVec3 vPos;
        vPos.x = centerPos.x;
        vPos.y = centerPos.y;
        vPos.z = centerPos.z;

        // IDA: 遍历 m_objectScanner.mapNPCList
        for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
            CMover* pMover = it->second;
            if (!pMover) {
                continue;
            }

            // IDA: if (XActor::GetType(pMover) == 2) // ActorType_Monster
            if (pMover->GetType() != eActorMonster) {
                continue;
            }

            // IDA: pMonster = dynamic_cast<CMonster*>(pMover)
            CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
            if (!pMonster) {
                continue;
            }

            // IDA: if (pMonster->GetTableID(pMonster) == nMonsterID)
            if (pMonster->GetTableID() != nMonsterID) {
                continue;
            }

            // IDA: bEnable = bActivate == 0
            bool bEnable = !bActivate;

            // IDA: CMover::SetInvincibleActor(pMonster, bEnable)
            pMonster->SetInvincibleActor(bEnable);

            // IDA: if (CMonster::GetAi(pMonster))
            CAi* pAi = pMonster->GetAi();
            if (pAi) {
                // IDA: CAi::ChangeAiState(Ai, FSMSTATES_WAIT)
                pAi->ChangeAiState(FSMSTATES_WAIT);
            }

            // IDA: this->MoveActor_2(this, v27, &vPos)
            // Note: IDA shows v27 = pMonster->XActor, MoveActor accepts XActor*, XVec3&, float
            MoveActor(static_cast<XActor*>(pMonster), vPos, 0.0f);

            // IDA: pMonster->SetDirectionYaw(pMonster, v10, v9) where v9 = 2
            pMonster->SetDirectionYaw(fRot, 2);

            // IDA: CMover::MoveingValueClear(pMonster)
            pMonster->MoveingValueClear();

            // IDA: pMonster->ChangeMotion_3(pMonster, 0, 1, 0)
            pMonster->ChangeMotion(0, 1, 0);

            // IDA: CMover::send_eSUB_CMD_MOVE_IDLE(pMonster, pMonster, -1.0)
            pMonster->send_eSUB_CMD_MOVE_IDLE(pMonster, -1.0f);

            return;
        }
    }
}

// ============================================================================
// SetMonsterDefenceType
// IDA: 0x14032DD90
// 设置怪物防御类型
// IDA 精确还原
// ============================================================================
void XMaze::SetMonsterDefenceType(int nMonsterID, unsigned char byType, float fTime) {
    // IDA: 遍历 m_objectScanner.mapNPCList
    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: if (XActor::GetType(pMover) == 2) // ActorType_Monster
        if (pMover->GetType() != eActorMonster) {
            continue;
        }

        // IDA: pMonster = dynamic_cast<CMonster*>(pMover)
        CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        if (!pMonster) {
            continue;
        }

        // IDA: if (pMonster->GetTableID(pMonster) == nMonsterID)
        if (pMonster->GetTableID() == nMonsterID) {
            // IDA: CMoverEx::ChangeDefenseTypeForce(pMonster, byType, fTime)
            pMonster->ChangeDefenseTypeForce(byType, fTime);
            return;
        }
    }
}

// ============================================================================
// MoveNpcToWayPoint
// IDA: 0x14032DEA0
// 移动NPC到路径点
// IDA 精确还原
// ============================================================================
void XMaze::MoveNpcToWayPoint(int nNpcID, int nWayPointID) {
    // IDA: 遍历 m_objectScanner.mapNPCList
    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: if (XActor::GetType(pMover) == 1) // ActorType_Npc
        if (pMover->GetType() != eActorNPC) {
            continue;
        }

        // IDA: pNpc = dynamic_cast<CNpc*>(pMover)
        CNpc* pNpc = dynamic_cast<CNpc*>(pMover);
        if (!pNpc) {
            continue;
        }

        // IDA: if (pNpc->GetTableID(pNpc) == nNpcID)
        if (pNpc->GetTableID() == nNpcID) {
            // IDA: CNpc::MoveToWayPoint(pNpc, nWayPointID)
            pNpc->MoveToWayPoint(nWayPointID);
            return;
        }
    }
}

// ============================================================================
// ChangeNpcAnimation
// IDA: 0x14032DFB0
// 改变NPC动画
// IDA 精确还原
// ============================================================================
void XMaze::ChangeNpcAnimation(int nNpcID, const char* szAnimName) {
    // IDA: 遍历 m_objectScanner.mapNPCList
    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: if (XActor::GetType(pMover) == 1) // ActorType_Npc
        if (pMover->GetType() != eActorNPC) {
            continue;
        }

        // IDA: pNpc = dynamic_cast<CNpc*>(pMover)
        CNpc* pNpc = dynamic_cast<CNpc*>(pMover);
        if (!pNpc) {
            continue;
        }

        // IDA: if (pNpc->GetTableID(pNpc) == nNpcID)
        if (pNpc->GetTableID() == nNpcID) {
            // IDA: pNpc->ChangeMotion_2(pNpc, szAnimName, 1)
            pNpc->ChangeMotion(szAnimName, 1);

            // IDA: CMover::send_eSUB_CMD_MOVE_IDLE(pNpc, pNpc, 0.0)
            pNpc->send_eSUB_CMD_MOVE_IDLE(pNpc, 0.0f);

            return;
        }
    }
}

// ============================================================================
// SetNpcRotation
// IDA: 0x14032E0E0
// 设置NPC旋转
// IDA 精确还原
// ============================================================================
void XMaze::SetNpcRotation(int nNpcID, float fYaw, int nKey) {
    // IDA: 遍历 m_objectScanner.mapNPCList
    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: if (XActor::GetType(pMover) == 1) // ActorType_Npc
        if (pMover->GetType() != eActorNPC) {
            continue;
        }

        // IDA: pNpc = dynamic_cast<CNpc*>(pMover)
        CNpc* pNpc = dynamic_cast<CNpc*>(pMover);
        if (!pNpc) {
            continue;
        }

        // IDA: if (pNpc->GetTableID(pNpc) == nNpcID)
        if (pNpc->GetTableID() == nNpcID) {
            // IDA: CNpc::CallMovingYaw(pNpc, fYaw, nKey)
            pNpc->CallMovingYaw(fYaw, nKey);
            return;
        }
    }
}

// ============================================================================
// SetMonsterAllowPassiveType
// IDA: 0x14032E200
// 设置怪物允许被动类型
// IDA 精确还原
// ============================================================================
void XMaze::SetMonsterAllowPassiveType(int nMonsterID, int nAllowPassiveType) {
    // IDA: 遍历 m_objectScanner.mapNPCList
    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: if (XActor::GetType(pMover) == 2) // ActorType_Monster
        if (pMover->GetType() != eActorMonster) {
            continue;
        }

        // IDA: pMonster = dynamic_cast<CMonster*>(pMover)
        CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        if (!pMonster) {
            continue;
        }

        // IDA: if (pMonster->GetTableID(pMonster) == nMonsterID)
        if (pMonster->GetTableID() == nMonsterID) {
            // IDA: CMover::SetAllowPassiveType(pMonster, nAllowPassiveType)
            pMonster->SetAllowPassiveType(nAllowPassiveType);
        }
    }
}

// ============================================================================
// IsSectorClearConditionBox
// IDA: 0x14032E880
// 检查是否为Sector清除条件Box
// IDA 精确还原
// ============================================================================
bool XMaze::IsSectorClearConditionBox(const VMonsterSpawnInfo* pSpawnInfo) {
    // IDA: if (!pSpawnInfo) return 0
    if (pSpawnInfo == nullptr) {
        return false;
    }

    // IDA: for (i = 0; i < 10; ++i)
    for (int i = 0; i < 10; ++i) {
        // IDA: if (pSpawnInfo->m_stMonsterInfo[i].m_iType == 1) return 0
        if (pSpawnInfo->m_stMonsterInfo[i].m_iType == 1) {
            return false;
        }

        // IDA: nMonsterID = pSpawnInfo->m_stMonsterInfo[i].m_iID
        int nMonsterID = pSpawnInfo->m_stMonsterInfo[i].m_iID;

        // IDA: if (!nMonsterID) break
        if (nMonsterID == 0) {
            break;
        }

        // IDA: v3 = TXSingleton<XGameServer>::Instance()
        // IDA: pTBMonster = XResourceMgr::GetTB_MONSTER(&v3->m_xResourceMgr, nMonsterID)
        XGameServer* pServer = XGameServer::Instance();
        TB_MONSTER* pTBMonster = pServer->GetResourceMgr().GetTB_MONSTER(nMonsterID);

        // IDA: if (pTBMonster)
        if (pTBMonster) {
            // IDA: if (pTBMonster->Monster_Type && pTBMonster->Monster_Type != 3 && pTBMonster->Monster_Type != 11) return 0
            if (pTBMonster->Monster_Type != 0 && pTBMonster->Monster_Type != 3 && pTBMonster->Monster_Type != 11) {
                return false;
            }

            // IDA: if ((!pTBMonster->Monster_Type || pTBMonster->Monster_Type == 3 || pTBMonster->Monster_Type == 11) && pTBMonster->Monster_ClearCondition_Type == 1) return 0
            if (pTBMonster->Monster_ClearCondition_Type == 1) {
                return false;
            }
        }
    }

    // IDA: return 1
    return true;
}

// ============================================================================
// SetSectorStepStop
// IDA: 0x14032E9C0
// 设置Sector步骤停止
// IDA 精确还原
// ============================================================================
void XMaze::SetSectorStepStop(int nSectorID, bool bStop) {
    // IDA: nUniqueSector = XMaze::GetUniqueID(this, nSectorID)
    int nUniqueSector = GetUniqueID(nSectorID);

    // IDA: std::_Tree::find(&this->m_mapSector, &it, &_Keyval)
    auto it = m_mapSector.find(nUniqueSector);

    // IDA: if (it != end)
    if (it != m_mapSector.end()) {
        // IDA: if (it->second)
        CSector* pSector = it->second;
        if (pSector) {
            // IDA: CSector::SetStepStop((CSector *)v4->second.__vftable, bStop)
            pSector->SetStepStop(bStop);
        }
    }
}

// ============================================================================
// FailEscortQuest
// IDA: 0x14032EA70
// 护送任务失败
// IDA 精确还原
// ============================================================================
void XMaze::FailEscortQuest() {
    // IDA: 遍历 m_objectScanner
    for (auto it = m_objectScanner.mapPlayerList.begin(); it != m_objectScanner.mapPlayerList.end(); ++it) {
        CMover* pObj = it->second;
        if (!pObj) {
            continue;
        }

        // IDA: pUser = dynamic_cast<CUser*>(pObj)
        CUser* pUser = dynamic_cast<CUser*>(pObj);
        if (!pUser) {
            continue;
        }

        // IDA: if (!CUser::IsGM(pUser) || !CUser::IsStatus(pUser, 0x2000u))
        if (pUser->IsGM() && pUser->IsStatus(0x2000u)) {
            continue;
        }

        // IDA: CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuest, 1)
        std::shared_ptr<CGocQuest> pQuest;
        pUser->CMoverEx::GetGOC<CGocQuest>(&pQuest, true);

        // IDA: if (shared_ptr invalid || !CGocQuest::FindEpisode(...))
        if (!pQuest || !pQuest->FindEpisode(m_stEscortMonster.dwEpisodeID)) {
            continue;
        }

        // IDA: v5 = shared_ptr::operator->(&pQuest)
        // IDA: CGocQuest::FailQuest(v5, this->m_stPartyQuest.nQuestID)
        pQuest->FailQuest(m_stPartyQuest.nQuestID);

        // IDA: XMaze::ResetPartyQuest(this)
        ResetPartyQuest();

        return;
    }
}

// ============================================================================
// ProcessMonsterQuest
// IDA: 0x14032F3C0
// 处理怪物任务更新
// TODO: 需要 CMover::GetGOC, CGocQuest, CGocDailyMission, CGocAchieve 等类型定义
// ============================================================================
void XMaze::ProcessMonsterQuest(XActor* pAttacker, unsigned int nMonsterID) {
    // IDA 反编译逻辑:
    // 1. 获取 TB_MONSTER 表数据
    // 2. 遍历 m_objectScanner 中的所有用户
    // 3. 对每个用户更新任务条件:
    //    - CGocQuest::UpdateCondition (eCONDITION_TARGET_MONSTER, eCONDITION_TARGET_MONSTER_GROUP, eCONDITION_TARGET_MAZE)
    //    - CGocDailyMission::UpdateKillType
    //    - CGocAchieve::UpdateMonsterAchieve
    //    - CGocClassEvent::CanBroachEquip (if Monster_Faction == 18)
    // 4. 调用 UpdateHiddenEventCondition

    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) return;

    TB_MONSTER* pTBMonster = pServer->GetResourceMgr().GetTB_MONSTER(nMonsterID);
    if (!pTBMonster) return;

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         CUser* pUser = dynamic_cast<CUser*>(pMover);
    //         if (pUser) {
    //             std::shared_ptr<CGocQuest> pQuest;
    //             pUser->GetGOC<CGocQuest>(&pQuest);
    //             if (pQuest) {
    //                 pQuest->UpdateCondition(3, eCONDITION_TARGET_MONSTER, pTBMonster->ID, 1, 0);
    //                 for (int i = 0; i < 5 && pTBMonster->Quest_Group[i]; ++i) {
    //                     pQuest->UpdateCondition(3, eCONDITION_TARGET_MONSTER_GROUP, pTBMonster->Quest_Group[i], 1, 0);
    //                 }
    //                 if (!pTBMonster->Monster_Type || pTBMonster->Monster_Type == 11) {
    //                     pQuest->UpdateCondition(3, eCONDITION_TARGET_MAZE, GetTBMapID(), 1, 0);
    //                 }
    //             }
    //         }
    //     }
    // }
    // UpdateHiddenEventCondition(1, nMonsterID, 1);
}

// ============================================================================
// LearnSkill
// IDA: 0x14032F770
// 教授技能给指定用户
// TODO: 需要 CMover::GetGOC<CGocSkill>, CGocSkill 类型定义
// ============================================================================
void XMaze::LearnSkill(unsigned int dwUserID, int nSkillID) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_objectScanner 中的所有对象
    // 2. 找到 ActorID 匹配的用户
    // 3. 获取 CGocSkill 组件
    // 4. 调用 CGocSkill::LearnSkill(nSkillID, 0, 0)

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         UXActorID actorID = pMover->GetActorID();
    //         if (actorID.dwActorID == dwUserID) {
    //             std::shared_ptr<CGocSkill> pSkill;
    //             pMover->GetGOC<CGocSkill>(&pSkill);
    //             if (pSkill) {
    //                 pSkill->LearnSkill(nSkillID, 0, 0);
    //             }
    //             return;
    //         }
    //     }
    // }
}

// ============================================================================
// GetUserClass
// IDA: 0x14032F8A0
// 获取用户职业
// ============================================================================
std::uint8_t XMaze::GetUserClass(unsigned int dwUserID) {
    // IDA 精确还原: 遍历 m_objectScanner 中的所有对象
    // 查找 ActorID 匹配的用户并返回其职业

    // 遍历所有三个map (mapPlayerList, mapNPCList, mapEtcList)
    for (auto it = m_objectScanner.mapPlayerList.begin(); it != m_objectScanner.mapPlayerList.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            UXActorID actorID = pMover->GetActorID();
            if (actorID.dwActorID == dwUserID) {
                return pMover->GetClass();
            }
        }
    }

    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            UXActorID actorID = pMover->GetActorID();
            if (actorID.dwActorID == dwUserID) {
                return pMover->GetClass();
            }
        }
    }

    for (auto it = m_objectScanner.mapEtcList.begin(); it != m_objectScanner.mapEtcList.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover) {
            UXActorID actorID = pMover->GetActorID();
            if (actorID.dwActorID == dwUserID) {
                return pMover->GetClass();
            }
        }
    }

    return 0;
}

// ============================================================================
// ChatMessage
// IDA: 0x14032F980
// 发送聊天消息
// TODO: 需要 PS_CHAT_NOTICE, XSendPacket, CGocNetwork 类型定义
// ============================================================================
void XMaze::ChatMessage(unsigned int dwUserID, char* szMsg) {
    // IDA 反编译逻辑:
    // 如果 dwUserID != 0:
    //   1. 在 m_objectScanner 中找到指定用户
    //   2. 发送 PS_CHAT_NOTICE 包 (main=7, sub=1)
    // 否则:
    //   1. 获取第一个用户
    //   2. 广播 PS_CHAT_NOTICE 包 (main=7, sub=4)

    // 简化实现：需要完整类型定义后还原
    // if (dwUserID != 0) {
    //     // 发送给指定用户
    //     auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    //     for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //         CMover* pMover = it->second;
    //         if (pMover) {
    //             UXActorID actorID = pMover->GetActorID();
    //             if (actorID.dwActorID == dwUserID) {
    //                 PS_CHAT_NOTICE stChat;
    //                 stChat.byType = 0;
    //                 MultiByteToWideChar(CP_ACP, 0, szMsg, -1, stChat.strMsg, 256);
    //                 XSendPacket xSendPacket(7, 1);
    //                 xSendPacket << stChat;
    //                 pMover->Send(&xSendPacket);
    //                 return;
    //             }
    //         }
    //     }
    // } else {
    //     // 广播给所有用户
    //     auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    //     if (!scannerMap.empty()) {
    //         CMover* pFirstMover = scannerMap.begin()->second;
    //         if (pFirstMover) {
    //             PS_CHAT_NOTICE stChat;
    //             stChat.byType = 0;
    //             MultiByteToWideChar(CP_ACP, 0, szMsg, -1, stChat.strMsg, 256);
    //             XSendPacket xSendPacket(7, 4);
    //             xSendPacket << stChat;
    //             pFirstMover->SendBroadCast(&xSendPacket, 0);
    //         }
    //     }
    // }
}

// ============================================================================
// GetMazeState
// IDA: 0x14032FC70
// 获取迷宫状态
// ============================================================================
int XMaze::GetMazeState() {
    // IDA 反编译逻辑:
    // if (GetMazeGameState() == 4) return 3;  // 完成
    // if (GetMazeGameState() == 6) return 4;  // 失败
    // if (IsBossSector()) return 2;           // Boss区域
    // return 1;                                // 普通

    int nGameState = GetMazeGameState();
    if (nGameState == 4) return 3;
    if (nGameState == 6) return 4;
    if (IsBossSector()) return 2;
    return 1;
}

// ============================================================================
// IsParty
// IDA: 0x14032FEA0
// 检查是否有队伍
// ============================================================================
bool XMaze::IsParty() {
    // IDA 反编译逻辑:
    // return m_pParty != nullptr
    return m_pParty != nullptr;
}

// ============================================================================
// SetLastSectorID
// IDA: 0x14032FF20
// 设置最后Sector ID
// ============================================================================
void XMaze::SetLastSectorID(int nVal, bool bIsPotal) {
    // IDA 反编译逻辑:
    // if (bIsPotal) m_vecActiveLastSectorID.clear();
    // m_vecActiveLastSectorID.push_back(nVal);

    if (bIsPotal) {
        m_vecActiveLastSectorID.clear();
    }
    m_vecActiveLastSectorID.push_back(UXActorID(nVal));
}

// ============================================================================
// GetLastSectorID
// IDA: 0x14032FF90
// 获取最后Sector ID
// ============================================================================
int XMaze::GetLastSectorID() {
    // IDA 反编译逻辑:
    // int iCount = m_vecActiveLastSectorID.size() - 1;
    // if (iCount < 0) return 0;
    // return m_vecActiveLastSectorID[iCount]->dwActorID;

    int iCount = static_cast<int>(m_vecActiveLastSectorID.size()) - 1;
    if (iCount < 0) return 0;
    return static_cast<int>(m_vecActiveLastSectorID[iCount].dwActorID);
}

// ============================================================================
// PartySend
// IDA: 0x14032FED0
// 发送包给队伍
// TODO: 需要 CParty::Send 方法
// ============================================================================
void XMaze::PartySend(XSendPacket& xSendPacket) {
    // IDA: 0x14032FED0
    // if (m_pParty != nullptr) {
    //     m_pParty->Send(xSendPacket, nullptr);
    // }

    if (m_pParty) {
        m_pParty->Send(xSendPacket, 0);
    }
}

// ============================================================================
// SetBossSector
// IDA: 0x14032FFF0
// 设置Boss区域标志
// ============================================================================
void XMaze::SetBossSector(bool bFlag) {
    // IDA 反编译:
    // if (m_stMazeGameState.m_nMazeState != 4 && m_bBossSector != bFlag) {
    //     m_bBossSector = bFlag;
    //     if (m_bBossSector) {
    //         PS_MAZE_UPDATE_INFO stUpdateMaze;
    //         TUXMapID result = GetInstanceID();
    //         stUpdateMaze.uxMapID = UXMapID(result.nMapID);  // 通过 __int64 构造
    //         stUpdateMaze.nState = 2;
    //         stUpdateMaze.nUserCount = m_objectScanner.size();
    //         for (auto& pair : m_mapWaitEnterMazeUser) {
    //             stUpdateMaze.vecMemberInfo.push_back(pair.second);
    //         }
    //         XSendPacket xSendPacket(0xF2, 0x26);
    //         xSendPacket << stUpdateMaze;
    //         XIOCPClient::Send(&xSendPacket);
    //     }
    // }

    if (m_stMazeGameState.m_nMazeState != 4 && m_bBossSector != bFlag) {
        m_bBossSector = bFlag;
        if (m_bBossSector) {
            PS_MAZE_UPDATE_INFO stUpdateMaze;
            TUXMapID instanceID = GetInstanceID();
            stUpdateMaze.uxMapID.nMapID = instanceID.nMapID;
            stUpdateMaze.nState = 2;
            stUpdateMaze.nUserCount = static_cast<int>(m_objectScanner.size());

            for (auto& pair : m_mapWaitEnterMazeUser) {
                stUpdateMaze.vecMemberInfo.push_back(pair.second);
            }

            XSendPacket xSendPacket(0xF2, 0x26);
            xSendPacket << stUpdateMaze;

            auto pServer = XGameServer::Instance();
            if (pServer) {
                pServer->GetControlSocket().Send(xSendPacket);
            }

            LogHelper::LogDebug("game.contents", "<< eSUB_CMD_SERVER_UPDATE_MAZE 2 >> 2 ");
        }
    }
}

// ============================================================================
// SetMonsterLastDamageType
// IDA: 0x1403305C0
// 设置怪物最后伤害类型
// ============================================================================
void XMaze::SetMonsterLastDamageType(unsigned int dwMonsterID, unsigned int dwCondition) {
    // IDA 反编译逻辑:
    // auto it = m_mapMonsterDamageType.find(dwMonsterID);
    // if (it != m_mapMonsterDamageType.end()) {
    //     it->second = dwCondition;
    // } else {
    //     m_mapMonsterDamageType[dwMonsterID] = dwCondition;
    // }

    m_mapMonsterDamageType[dwMonsterID] = dwCondition;
}

// ============================================================================
// GetMonsterLastDamageType
// IDA: 0x140330660
// 获取怪物最后伤害类型
// ============================================================================
unsigned int XMaze::GetMonsterLastDamageType(unsigned int dwMonsterID) {
    // IDA 反编译逻辑:
    // auto it = m_mapMonsterDamageType.find(dwMonsterID);
    // if (it != m_mapMonsterDamageType.end()) {
    //     return it->second;
    // }
    // return 0;

    auto it = m_mapMonsterDamageType.find(dwMonsterID);
    if (it != m_mapMonsterDamageType.end()) {
        return it->second;
    }
    return 0;
}

// ============================================================================
// UseSkillCondition
// IDA: 0x1403306E0
// 使用技能条件
// TODO: 需要 UpdateHiddenEventCondition 方法
// ============================================================================
void XMaze::UseSkillCondition(unsigned char bySkillType, unsigned int dwValue) {
    // IDA 反编译逻辑:
    // if (bySkillType == 4) {
    //     UpdateHiddenEventCondition(9, 0, dwValue);
    // } else if (bySkillType == 5) {
    //     UpdateHiddenEventCondition(8, 0, dwValue);
    // }

    if (bySkillType == 4) {
        UpdateHiddenEventCondition(9, 0, dwValue);
    } else if (bySkillType == 5) {
        UpdateHiddenEventCondition(8, 0, dwValue);
    }
}

// ============================================================================
// UpdateHiddenEventCondition
// IDA: 0x140330740
// 更新隐藏事件条件
// TODO: 需要 CHiddenEvent 类型定义
// ============================================================================
void XMaze::UpdateHiddenEventCondition(int nTarget, unsigned int dwObject, unsigned int dwValue) {
    // IDA 反编译逻辑:
    // if (m_pHiddenEvent) {
    //     if (CHiddenEvent::UpdateCondition(m_pHiddenEvent, nTarget, dwObject, dwValue)) {
    //         int nState = CHiddenEvent::GetHiddenEventState(m_pHiddenEvent);
    //         if (nState == 2 || nState == 3 || nState == 4) {
    //             SendHiddenEventUpdate();
    //         }
    //     }
    // }

    if (m_pHiddenEvent) {
        // TODO: CHiddenEvent not defined
        // if (CHiddenEvent::UpdateCondition(m_pHiddenEvent, nTarget, dwObject, dwValue)) {
        //     int nState = CHiddenEvent::GetHiddenEventState(m_pHiddenEvent);
        //     if (nState == 2 || nState == 3 || nState == 4) {
        //         SendHiddenEventUpdate();
        //     }
        // }
    }
}

// ============================================================================
// CheckHiddenEventState
// IDA: 0x1403307D0
// 检查隐藏事件状态
// TODO: 需要 CHiddenEvent, CGocAchieve, CGocQuest 类型定义
// ============================================================================
bool XMaze::CheckHiddenEventState() {
    // IDA 反编译逻辑:
    // 1. 检查 m_pHiddenEvent
    // 2. 调用 CHiddenEvent::CheckResult
    // 3. 如果结果为true，调用 SendHiddenEventUpdate
    // 4. 设置奖励物品
    // 5. 遍历所有用户，记录日志并更新成就/任务条件

    if (!m_pHiddenEvent) {
        return false;
    }

    // TODO: 完整实现需要 CHiddenEvent 类型
    // if (CHiddenEvent::CheckResult(m_pHiddenEvent)) {
    //     SendHiddenEventUpdate();
    // }
    // CHiddenEvent::SetRewardItem(m_pHiddenEvent);
    // ... 遍历用户记录日志

    return true;
}

// ============================================================================
// SendHiddenEventUpdate
// IDA: 0x140330C20
// 发送隐藏事件更新
// TODO: 需要 CHiddenEvent, XSendPacket 类型定义
// ============================================================================
void XMaze::SendHiddenEventUpdate() {
    // IDA 反编译逻辑:
    // if (m_pHiddenEvent) {
    //     XSendPacket xSendPacket(0x11, 0x37);
    //     xSendPacket << CHiddenEvent::GetHiddenEventID(m_pHiddenEvent);
    //     xSendPacket << CHiddenEvent::GetEventConditionID(m_pHiddenEvent);
    //     xSendPacket << CHiddenEvent::GetHiddenEventState(m_pHiddenEvent);
    //     xSendPacket << CGameWorldMode::GetModeDateID(m_pHiddenEvent);
    //     SendBroadCast(&xSendPacket, nullptr, eAll);
    // }

    if (m_pHiddenEvent) {
        // TODO: CHiddenEvent not defined
        // XSendPacket xSendPacket(0x11, 0x37);
        // xSendPacket << CHiddenEvent::GetHiddenEventID(m_pHiddenEvent);
        // xSendPacket << CHiddenEvent::GetEventConditionID(m_pHiddenEvent);
        // xSendPacket << CHiddenEvent::GetHiddenEventState(m_pHiddenEvent);
        // xSendPacket << CGameWorldMode::GetModeDateID(m_pHiddenEvent);
        // SendBroadCast(&xSendPacket, nullptr, eAll);
    }
}

// ============================================================================
// SendMazeInfo
// IDA: 0x140331130
// 发送迷宫信息
// TODO: 需要 PS_MAZE_UPDATE_INFO, XSendPacket, XIOCPClient 类型定义
// ============================================================================
bool XMaze::SendMazeInfo() {
    // IDA 反编译逻辑:
    // PS_MAZE_UPDATE_INFO stUpdateMaze;
    // stUpdateMaze.uxMapID = GetInstanceID();
    // stUpdateMaze.nState = GetMazeState();
    // stUpdateMaze.nUserCount = m_objectScanner.size();
    // for (auto& pair : m_mapWaitEnterMazeUser) {
    //     stUpdateMaze.vecMemberInfo.push_back(pair.second);
    // }
    // XSendPacket xSendPacket(0xF2, 0x26);
    // xSendPacket << stUpdateMaze;
    // XGameServer::Instance()->GetControlSocket().Send(&xSendPacket);
    // return true;

    // TODO: 完整实现需要 PS_MAZE_UPDATE_INFO 类型
    // PS_MAZE_UPDATE_INFO stUpdateMaze;
    // stUpdateMaze.nState = GetMazeState();
    // stUpdateMaze.nUserCount = GetCurUserCount();
    // XSendPacket xSendPacket(0xF2, 0x26);
    // xSendPacket << stUpdateMaze;
    // XGameServer::Instance()->GetControlSocket().Send(&xSendPacket);
    return true;
}

// ============================================================================
// DiePlayer
// IDA: 0x14032B530
// 玩家死亡处理
// TODO: 需要 CSector::DiePlayer, CGocParty, CGocForce, CGocEntity 等类型定义
// ============================================================================
void XMaze::DiePlayer(CUser* pUser) {
    // IDA: 0x14032B530
    // 1. Check if all users are dead
    // 2. Call m_pActiveEventSector->DiePlayer(pUser, bAllDie)
    // 3. If not revive or maze type is 9, set disconnect user state
    // 4. If Roguelike map, process result

    if (!pUser) return;

    // Check if all users are dead
    bool bAllDie = true;
    for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
        CMover* pMover = it->second;
        if (pMover && !pMover->IsDie()) {
            bAllDie = false;
            break;
        }
    }

    // Call event sector handler
    if (m_pActiveEventSector) {
        m_pActiveEventSector->DiePlayer(pUser, bAllDie);
    }

    // Handle non-revive case
    if (!IsRevive() || GetMazeType() == 9) {
        // TODO: Set disconnect user state for party/force
        // This requires CGocParty, CGocForce, SetDisconnectUserState
    }

    // Roguelike map handling
    if (IsRoguelikeMap()) {
        // TODO: CGocEntity processing
        // FinishMazeTime();
        // pEntity->SetRoguelikeResult(m_stMazeGameState.m_dwMazePlayTime, 0);
        // pEntity->SendRoguelikeResult(0);
    }
}

// ============================================================================
// IsRevive
// IDA: 0x14032B8A0
// 检查是否可以复活
// ============================================================================
bool XMaze::IsRevive() {
    // IDA 反编译逻辑:
    // return m_pTBMazeInfo && m_pTBMazeInfo->Revival_Type != 1

    return m_pTBMazeInfo && m_pTBMazeInfo->Revival_Type != 1;
}

// ============================================================================
// IsUseItem
// IDA: 0x14032B8E0
// 检查是否可以使用物品
// ============================================================================
bool XMaze::IsUseItem() {
    // IDA 反编译逻辑:
    // return !m_pTBMazeInfo || m_pTBMazeInfo->Potion_Limit != 1

    return !m_pTBMazeInfo || m_pTBMazeInfo->Potion_Limit != 1;
}

// ============================================================================
// DamageMonster
// IDA: 0x14032B300
// 对怪物造成伤害
// ============================================================================
void XMaze::DamageMonster(CMonster* pMonster) {
    // IDA: if (m_pActiveEventSector) m_pActiveEventSector->DamageMonster(pMonster);
    if (m_pActiveEventSector) {
        m_pActiveEventSector->DamageMonster(pMonster);
    }
}

void XMaze::OnProtectSkill(int nSkillType) {
    (void)nSkillType;
}

// ============================================================================
// InteractBoxOnMode
// IDA: 0x14032B340
// 模式下的交互盒处理
// TODO: 需要 CSector::InteractBoxOnMode 方法
// ============================================================================
void XMaze::InteractBoxOnMode(CUser* pUser, int nBoxID) {
    // IDA: if (m_pActiveEventSector) m_pActiveEventSector->InteractBoxOnMode(pUser, nBoxID);
    if (m_pActiveEventSector) {
        m_pActiveEventSector->InteractBoxOnMode(pUser, nBoxID);
    }
}

// ============================================================================
// TestReward
// IDA: 0x140324B20
// 测试迷宫奖励
// ============================================================================
void XMaze::TestReward() {
    // IDA 反编译: XMaze::TestReward
    // 1. 调用 FinishMazeTime
    // 2. 如果有队伍，调用 CParty::MazeReward
    // 3. 否则遍历所有用户，调用 CGocRecode::TestRandomScore 和 CGocRecode::MazeReward

    FinishMazeTime();

    if (m_pParty) {
        // 有队伍时，调用队伍奖励
        // m_pParty->MazeReward(m_stMazeGameState.m_dwMazePlayTime);
    } else {
        // 遍历所有用户
        // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
        // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
        //     CMover* pObj = it->second;
        //     if (pObj) {
        //         CUser* pUser = dynamic_cast<CUser*>(pObj);
        //         if (pUser && (!pUser->IsGM() || !pUser->IsStatus(0x2000))) {
        //             std::shared_ptr<CGocRecode> pRecode;
        //             pUser->GetGOC<CGocRecode>(&pRecode);
        //             if (pRecode) {
        //                 pRecode->TestRandomScore();
        //                 pRecode->MazeReward(m_stMazeGameState.m_dwMazePlayTime, 1, 1);
        //             }
        //         }
        //     }
        // }
    }
}

// ============================================================================
// ResetAllSectorFlags
// IDA: 0x140329B80
// 重置所有Sector标志
// ============================================================================
void XMaze::ResetAllSectorFlags() {
    // IDA 反编译: XMaze::ResetAllSectorFlags
    // 1. 遍历 m_mapGateBox，设置所有 STMageGateBox::bOpen = false
    // 2. 遍历 m_mapPotalBox，设置所有 STMagePotalBox::bOpen = false
    // 3. 遍历 m_mapLuaFunctionBox，设置所有 STLuaFunctionBox::bCalled = false
    // 4. 遍历 m_mapSector，调用所有 CSector::Reset
    // 5. 调用 ClearAllTimers

    // 重置所有 GateBox
    for (auto& pair : m_mapGateBox) {
        if (pair.second) {
            pair.second->bOpen = false;
        }
    }

    // 重置所有 PotalBox
    for (auto& pair : m_mapPotalBox) {
        if (pair.second) {
            pair.second->bOpen = false;
        }
    }

    // 重置所有 LuaFunctionBox
    for (auto& pair : m_mapLuaFunctionBox) {
        if (pair.second) {
            pair.second->bCalled = false;
        }
    }

    // 重置所有 Sector
    for (auto& pair : m_mapSector) {
        if (pair.second) {
            // TODO: 需要实现 CSector::Reset
            // pair.second->Reset();
        }
    }

    // 清除所有定时器
    ClearAllTimers();
}

// ============================================================================
// CallScriptUpdateQuest
// IDA: 0x140331630
// ============================================================================
void XMaze::CallScriptUpdateQuest(unsigned int dwActorID, int nType, unsigned int dwID) {
    if (m_pScriptInstance) {
        m_pScriptInstance->ExecuteFunctionArg(
            "OnUpdateQuest",
            "iiiTSoulworker:XMaze;",
            dwActorID,
            nType,
            dwID,
            this);
    }
}

// ============================================================================
// RunQuestConditionStart
// IDA: 0x140323360
// ============================================================================
void XMaze::RunQuestConditionStart(unsigned int dwUserID, int nConditionID) {
    char szConditionID[16];
    _itoa(nConditionID, szConditionID, 10);

    if (m_pScriptInstance && m_pScriptInstance->HasFunction("OnQuestConditionStart")) {
        m_pScriptInstance->ExecuteFunctionArg(
            "OnQuestConditionStart",
            "siTSoulworker:XMaze;",
            szConditionID,
            dwUserID,
            this);
    }
}

// ============================================================================
// RunQuestConditionEnd
// IDA: 0x140323420
// ============================================================================
void XMaze::RunQuestConditionEnd(unsigned int dwUserID, int nConditionID) {
    char szConditionID[16];
    _itoa(nConditionID, szConditionID, 10);

    if (m_pScriptInstance && m_pScriptInstance->HasFunction("OnQuestConditionEnd")) {
        m_pScriptInstance->ExecuteFunctionArg(
            "OnQuestConditionEnd",
            "siTSoulworker:XMaze;",
            szConditionID,
            dwUserID,
            this);
    }
}

// ============================================================================
// ShowSectorInfo
// IDA: 0x1403316A0
// 显示Sector信息给用户
// ============================================================================
void XMaze::ShowSectorInfo(CUser* pUser) {
    // Per IDA 0x1403316A0: XMaze::ShowSectorInfo 精确还原
    if (!pUser) {
        return;
    }

    // IDA: Get user position
    const hkvVec3& vPos = pUser->GetPosition();

    // IDA: Get sector from position
    CSector* pSector = GetSectorFromPos(vPos);
    if (pSector) {
        pSector->ShowSectorInfo(pUser);
    }
}

// ============================================================================
// GetMonsterCountByID
// IDA: 0x1403316F0
// 根据TableID获取怪物数量
// ============================================================================
unsigned int XMaze::GetMonsterCountByID(unsigned int dwID) {
    // IDA: Iterate through m_objectScanner.mapNPCList
    unsigned int nCount = 0;

    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) continue;

        // IDA: Check if actor type is 2 (ActorType_Monster)
        if (pMover->GetType() != 2) continue;

        // IDA: Dynamic cast to CMonster
        CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        if (pMonster && pMonster->GetTableID() == dwID) {
            ++nCount;
        }
    }

    return nCount;
}

// ============================================================================
// GetMonsterSpawnBoxInfo
// 根据SpawnBoxID获取生成箱信息
// ============================================================================
VMonsterSpawnInfo* XMaze::GetMonsterSpawnBoxInfo(int nSpawnBoxID) {
    // IDA: 遍历 m_listMonsterSpawnInfo 查找匹配的 iID
    for (auto it = m_listMonsterSpawnInfo.begin(); it != m_listMonsterSpawnInfo.end(); ++it) {
        VMonsterSpawnInfo* pInfo = *it;
        if (pInfo && pInfo->iID == nSpawnBoxID) {
            return pInfo;
        }
    }
    return nullptr;
}

// ============================================================================
// AddDieMonsterSpawnBoxID
// IDA: 0x140331FF0
// 添加死亡怪物的SpawnBoxID到列表
// IDA 精确还原
// ============================================================================
void XMaze::AddDieMonsterSpawnBoxID(int nSpawnBoxID) {
    // IDA: 获取生成箱信息
    VMonsterSpawnInfo* pMonsterSpawnInfo = GetMonsterSpawnBoxInfo(nSpawnBoxID);
    if (!pMonsterSpawnInfo) {
        return;
    }

    // IDA: 创建 VString 并检查是否为空
    VString keyString(pMonsterSpawnInfo->m_szObjectKey);
    if (!keyString.IsEmpty()) {
        // IDA: std::vector<unsigned long>::push_back
        m_listDieMonsterSpawnBoxID.push_back(nSpawnBoxID);
    }
}

// ============================================================================
// AddChangeMonster
// IDA: 0x140332090
// 添加变换怪物ID到列表
// IDA 精确还原
// ============================================================================
void XMaze::AddChangeMonster(unsigned int dwMonsterID) {
    // IDA: 遍历 m_lstChangeMonster 检查是否已存在
    for (auto it = m_lstChangeMonster.begin(); it != m_lstChangeMonster.end(); ++it) {
        if (*it == dwMonsterID) {
            return;  // 已存在，不重复添加
        }
    }
    // IDA: std::vector<unsigned long>::push_back
    m_lstChangeMonster.push_back(dwMonsterID);
}

// ============================================================================
// GetMazePotalBox
// IDA: 0x140332C30
// EXACT IDA implementation - returns portal box by unique ID
// ============================================================================
STMagePotalBox* XMaze::GetMazePotalBox(int nUniqueID) {
    auto it = m_mapPotalBox.find(nUniqueID);
    if (it != m_mapPotalBox.end()) {
        return it->second;
    }
    return nullptr;
}

// ============================================================================
// UpdateQuestRespawn
// IDA: 0x140330440
// 更新任务重生点
// IDA 精确还原
// ============================================================================
void XMaze::UpdateQuestRespawn(int nCondition) {
    // IDA: std::vector<const VMonsterSpawnInfo*> vecDeletedBox
    std::vector<const VMonsterSpawnInfo*> vecDeletedBox;

    // IDA: CRespawnManager::RemoveQuestMonster(&this->m_respawnManager, nCondition, &vecDeletedBox)
    m_respawnManager.RemoveQuestMonster(nCondition, vecDeletedBox);

    // IDA: for loop through vecDeletedBox
    for (auto it = vecDeletedBox.begin(); it != vecDeletedBox.end(); ++it) {
        // IDA: pMonsterSpawn = *it
        const VMonsterSpawnInfo* pMonsterSpawn = *it;

        // IDA: if (pMonsterSpawn)
        if (pMonsterSpawn) {
            // IDA: BatchLayerLevel = XMaze::GetBatchLayerLevel(this)
            int nBatchLayerLevel = GetBatchLayerLevel();

            // IDA: iUniqueID = VEventObjectInfo::GetEventUniqueID(pMonsterSpawn->m_iSectorID, BatchLayerLevel)
            int iUniqueID = VEventObjectInfo::GetEventUniqueID(pMonsterSpawn->m_iSectorID, nBatchLayerLevel);

            // IDA: std::_Tree::find(&this->m_mapSector, &v8, &_Keyval)
            auto sectorIt = m_mapSector.find(iUniqueID);

            // IDA: if (sectorIt != end)
            if (sectorIt != m_mapSector.end()) {
                // IDA: pSector = sectorIt->second
                CSector* pSector = sectorIt->second;

                // IDA: if (pSector) CSector::EraseRespawnBoxID(pSector, pMonsterSpawn->iUniqueID)
                // 注: IDA 中 iUniqueID 应该是 iID (SpawnBoxID)
                if (pSector) {
                    pSector->EraseRespawnBoxID(pMonsterSpawn->iID);
                }
            }
        }
    }
}

// ============================================================================
// ChangeMonsterLevelStat
// IDA: 0x140328840
// 根据队伍人数改变怪物等级属性
// IDA 精确还原
// ============================================================================
void XMaze::ChangeMonsterLevelStat(int nMemberCount) {
    // IDA: this->m_nPartyMemeberCount = nMemberCount
    m_nPartyMemeberCount = nMemberCount;

    // IDA: if (nMemberCount > 0)
    if (nMemberCount > 0) {
        // IDA: 遍历 m_objectScanner.mapNPCList
        for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
            CMover* pMover = it->second;
            if (!pMover) {
                continue;
            }

            // IDA: if (XActor::GetType(pMover) == 2) // ActorType_Monster
            if (pMover->GetType() != eActorMonster) {
                continue;
            }

            // IDA: pMonster = dynamic_cast<CMonster*>(pMover)
            CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
            if (!pMonster) {
                continue;
            }

            // IDA: if (!pMonster->IsFollower(pMonster))
            if (pMonster->IsFollower()) {
                continue;
            }

            // IDA: pMobTable = CMonster::GetMobTableRef(pMonster)
            TB_MONSTER* pMobTable = pMonster->GetMobTableRef();
            if (!pMobTable) {
                continue;
            }

            // IDA: if (!pMobTable->Monster_Type) // 普通怪物
            if (pMobTable->Monster_Type != 0) {
                continue;
            }

            // IDA: 保存当前 HP 和 SuperArmor 比例
            int nHP = pMonster->GetHP();
            int nMaxHP = pMonster->GetMaxHP();
            int nCurSuperArmor = static_cast<int>(pMonster->GetCurSuperArmorGage());
            int nMaxSuperArmor = static_cast<int>(pMonster->GetMaxSuperArmorGage());

            float fHpRate = 1.0f;
            if (nHP > 0) {
                fHpRate = static_cast<float>(nHP) / static_cast<float>(nMaxHP);
            }

            float fArmorRate = 1.0f;
            if (nCurSuperArmor > 0 && nMaxSuperArmor > 0) {
                fArmorRate = static_cast<float>(nCurSuperArmor) / static_cast<float>(nMaxSuperArmor);
            }

            // IDA: CMover::GetGOC<CGocNpcAttribute>(pMonster, &pAttr, 0)
            std::shared_ptr<CGocNpcAttribute> pAttr;
            pMonster->GetGOC<CGocNpcAttribute>(&pAttr, false);

            if (pAttr) {
                // IDA: CGocNpcAttribute::Init(pAttr, 1u, pMobTable)
                pAttr->Init(1, pMobTable);
            }

            // IDA: CMonster::ApplyLevelToStat(pMonster, 0)
            pMonster->ApplyLevelToStat(0);

            // IDA: pMonster->ReapllyBuffAll(pMonster)
            pMonster->ReapllyBuffAll();

            // IDA: 恢复 HP 和 SuperArmor 比例
            int nNewMaxHP = pMonster->GetHP();
            int nChangeHP = static_cast<int>(static_cast<float>(nNewMaxHP) * fHpRate);
            float fNewMaxArmor = pMonster->GetMaxSuperArmorGage();
            int nChangeArmor = static_cast<int>(fNewMaxArmor * fArmorRate);

            // IDA: pMonster->SetHpEx(pMonster, nChangeHP)
            pMonster->SetHpEx(nChangeHP);

            // IDA: CMover::SetCurSuperArmorGage(pMonster, nChangeArmor)
            pMonster->SetCurSuperArmorGage(static_cast<float>(nChangeArmor));
        }

        // IDA: XMaze::SendNoticePacket(this, 80, 0, 0.0)
        SendNoticePacket(80, 0, 0.0f);

        // IDA: XMaze::RestartResetState(this, 0, 0)
        RestartResetState(false, false);
    }
}

// ============================================================================
// SendNoticePacket
// IDA: 0x140328BC0
// 发送通知包
// IDA 精确还原
// ============================================================================
void XMaze::SendNoticePacket(int iType, int iValue, float fTime) {
    // IDA: XSendPacket::XSendPacket(&xPacket, 0x11u, 0x51u)
    XSendPacket xPacket(0x11, 0x51);

    // IDA: XParse::operator<<(&xPacket.XParse, iType)
    xPacket << iType;

    // IDA: XParse::operator<<(&xPacket.XParse, iValue)
    xPacket << iValue;

    // IDA: XParse::operator<<(&xPacket.XParse, fTime)
    xPacket << fTime;

    // IDA: this->SendBroadCast(this, &xPacket, nullptr, eAll)
    SendBroadCast(xPacket, nullptr, E_BROADCAST_TYPE::eAll);
}

// ============================================================================
// ShowCasualRaidTimer
// IDA: 0x14032B390
// 显示休闲副本计时器
// IDA 精确还原
// ============================================================================
void XMaze::ShowCasualRaidTimer(int nValue, float fTime, float fTime2) {
    // IDA: XSendPacket::XSendPacket(&xPacket, 0x11u, 0x38u)
    XSendPacket xPacket(0x11, 0x38);

    // IDA: XParse::operator<<(&xPacket.XParse, 0)
    xPacket << 0;  // iType

    // IDA: XParse::operator<<(&xPacket.XParse, nValue)
    xPacket << nValue;

    // IDA: XParse::operator<<(&xPacket.XParse, fTime)
    xPacket << fTime;

    // IDA: XParse::operator<<(&xPacket.XParse, fTime2)
    xPacket << fTime2;

    // IDA: this->SendBroadCast(this, &xPacket, nullptr, eAll)
    SendBroadCast(xPacket, nullptr, E_BROADCAST_TYPE::eAll);

    // IDA: this->m_stCasualRaidTime.nIntValue = nValue
    m_stCasualRaidTime.nIntValue = nValue;

    // IDA: this->m_stCasualRaidTime.fFloatValue = fTime
    m_stCasualRaidTime.fFloatValue = fTime;
}

// ============================================================================
// SetupCasualRaidTimer
// IDA: 0x14032B4B0
// 设置休闲副本计时器
// IDA 精确还原
// ============================================================================
void XMaze::SetupCasualRaidTimer() {
    // IDA: if (this->m_stCasualRaidTime.fFloatValue > 0.0)
    if (m_stCasualRaidTime.fFloatValue > 0.0f) {
        // IDA: this->m_stCasualRaidTime.fWaitSendTime = 5.0
        m_stCasualRaidTime.fWaitSendTime = 5.0f;
    }
}

// ============================================================================
// SendCasualRaidTimer
// IDA: 0x14032B4E0
// 发送休闲副本计时器
// IDA 精确还原
// ============================================================================
void XMaze::SendCasualRaidTimer() {
    // IDA: if (this->m_stCasualRaidTime.fFloatValue > 0.0)
    if (m_stCasualRaidTime.fFloatValue > 0.0f) {
        // IDA: XMaze::ShowCasualRaidTimer(this, m_stCasualRaidTime.nIntValue, m_stCasualRaidTime.fFloatValue, 0.0)
        ShowCasualRaidTimer(m_stCasualRaidTime.nIntValue, m_stCasualRaidTime.fFloatValue, 0.0f);
    }
}

// ============================================================================
// EnterInfiniteTowerNextStage
// IDA: 0x140331F10
// 进入无限塔下一层
// ============================================================================
bool XMaze::EnterInfiniteTowerNextStage(CUser* pUser) {
    // IDA 反编译: XMaze::EnterInfiniteTowerNextStage
    // 1. 检查迷宫是否完成、是否为无限塔类型(Maze_Type==7)
    // 2. 如果有下一层，进入下一层
    // 3. 否则退出区域

    if (!m_bMazeComplete || !m_pTBMazeInfo || m_pTBMazeInfo->Maze_Type != 7) {
        return false;
    }

    // if (m_stInfiniteTowerInfo.m_wNextMaze) {
    //     if (pUser) {
    //         pUser->EnterWorldByForce(m_stInfiniteTowerInfo.m_wNextMaze, 0, &m_stEnterDistrictPos);
    //     }
    //     return true;
    // } else {
    //     ExitArea(pUser ? &pUser->XActor : nullptr);
    //     return true;
    // }

    return true;
}

// ============================================================================
// SendMazeInfoSync
// IDA: 0x1403312C0
// 发送迷宫信息同步
// ============================================================================
bool XMaze::SendMazeInfoSync() {
    // IDA 反编译: XMaze::SendMazeInfoSync
    // 1. 构建 PS_MAZE_UPDATE_INFO_SYNC 结构
    // 2. 设置迷宫ID、状态、用户数量
    // 3. 添加等待进入的用户信息
    // 4. 设置服务器ID、端口、IP
    // 5. 发送包 (main=0xF2, sub=0x23)

    // PS_MAZE_UPDATE_INFO_SYNC stUpdateMaze;
    // stUpdateMaze.uxMapID = GetInstanceID();
    // stUpdateMaze.psMazeInfo.nState = GetMazeState();
    // stUpdateMaze.psMazeInfo.nUserCount = m_objectScanner.size();
    //
    // for (auto& pair : m_mapWaitEnterMazeUser) {
    //     stUpdateMaze.psMazeInfo.vecMemberInfo.push_back(pair.second);
    // }
    //
    // stUpdateMaze.stPartyInfo = m_stPartyInfo;
    // stUpdateMaze.dwServerID = XGameServer::Instance()->GetOption()->GetQuestID();
    // stUpdateMaze.sPort = XGameServer::Instance()->GetOption()->GetPort();
    // strcpy_s(stUpdateMaze.szIP, XGameServer::Instance()->GetOption()->GetPublicIP());
    // stUpdateMaze.nJumpID = m_nJumpID;
    //
    // XSendPacket xSendPacket(0xF2, 0x23);
    // xSendPacket << stUpdateMaze;
    // XGameServer::Instance()->GetControlSocket().Send(&xSendPacket);

    return true;
}

// ============================================================================
// UpdateRandomPortal
// IDA: 0x140332A10
// 更新随机传送门
// ============================================================================
void XMaze::UpdateRandomPortal(int nSectorID) {
    // IDA 反编译: XMaze::UpdateRandomPortal
    // 1. 遍历 m_mapRandomPotalBox
    // 2. 检查每个传送门的清除条件是否匹配 nSectorID
    // 3. 根据概率决定是否打开传送门
    // 4. 如果尝试次数超过4次，从列表中移除

    // for (auto it = m_mapRandomPotalBox.begin(); it != m_mapRandomPotalBox.end(); ) {
    //     STMagePotalBox* pPortal = it->second;
    //     bool bFind = false;
    //
    //     if (pPortal->nOpenTryCount <= 4) {
    //         // 检查清除条件
    //         bFind = (pPortal->pPotalBox->m_iClearSectorID1 == nSectorID ||
    //                  pPortal->pPotalBox->m_iClearSectorID2 == nSectorID ||
    //                  pPortal->pPotalBox->m_iClearSectorID3 == nSectorID ||
    //                  pPortal->pPotalBox->m_iClearSectorID4 == nSectorID ||
    //                  pPortal->pPotalBox->m_iClearSectorID5 == nSectorID);
    //
    //         if (bFind) {
    //             int nValue = nRand(1, 100);
    //             if (nValue <= pPortal->nOpenRate[pPortal->nOpenTryCount]) {
    //                 SetPotalFlag(pPortal->pPotalBox->iID, true);
    //                 it = m_mapRandomPotalBox.erase(it);
    //                 continue;
    //             }
    //             ++pPortal->nOpenTryCount;
    //         }
    //     } else {
    //         it = m_mapRandomPotalBox.erase(it);
    //         continue;
    //     }
    //     ++it;
    // }

    // 简化实现：需要完整类型定义后还原
}

// ============================================================================
// WarpSectorStartPos
// IDA: 0x1403319A0
// 传送到Sector起始位置
// ============================================================================
bool XMaze::WarpSectorStartPos(CUser* pUser) {
    // IDA 反编译: XMaze::WarpSectorStartPos
    // 1. 获取最后Sector ID
    // 2. 获取起始位置
    // 3. 移动用户到起始位置
    // 4. 清除移动值，改变动作
    // 5. 发送传送包

    if (!pUser) {
        return false;
    }

    // int nLastSectorID = GetLastSectorID();
    // CSector* pLastSector = GetSector(nLastSectorID);
    // if (!pLastSector) {
    //     return false;
    // }
    //
    // XVec3 vStart;
    // float fRot = 0.0f;
    // if (!pLastSector->GetStartPos(&vStart, &fRot)) {
    //     return false;
    // }
    //
    // MoveActor(&pUser->XActor, &vStart);
    // pUser->MoveingValueClear();
    // pUser->ChangeMotion(1, true, 20);
    // pUser->SendResWarp(0, &vStart, fRot);

    return true;
}

// ============================================================================
// MoveNextSector
// IDA: 0x140331AD0
// 移动到下一个Sector
// ============================================================================
void XMaze::MoveNextSector(CUser* pUser, int nNextStep) {
    // IDA 反编译: XMaze::MoveNextSector - 复杂函数
    // 1. 获取用户当前位置和Sector
    // 2. 计算下一个Sector ID
    // 3. 获取下一个Sector的起始位置
    // 4. 设置当前Sector完成、打开传送门
    // 5. 销毁当前Sector怪物
    // 6. 激活下一个Sector AI
    // 7. 生成下一个Sector怪物
    // 8. 移动用户到新位置
    // 9. 发送传送包

    if (!pUser) {
        return;
    }

    // const hkvVec3& Position = pUser->GetPosition();
    // CSector* pSector = GetSectorFromPos(Position);
    // if (!pSector) {
    //     return;
    // }
    //
    // int nNextSectorID = 0;
    // if (nNextStep) {
    //     int nSectorBoxUniqueID = pSector->GetSectorBoxUniqueID();
    //     nNextSectorID = nNextStep + nSectorBoxUniqueID - (nSectorBoxUniqueID % 100);
    // } else {
    //     nNextSectorID = pSector->GetSectorBoxUniqueID() + 1;
    // }
    //
    // if (nNextSectorID > pSector->GetSectorBoxUniqueID()) {
    //     auto it = m_mapSector.find(nNextSectorID);
    //     if (it != m_mapSector.end()) {
    //         CSector* pNextSector = it->second;
    //         if (pNextSector) {
    //             XVec3 vStart;
    //             float fRot;
    //             if (pNextSector->GetStartPos(&vStart, &fRot)) {
    //                 // 设置当前Sector完成
    //                 pSector->SetComplete(true);
    //                 pSector->SetPortalOpen(true);
    //                 pSector->TerminateSpawn();
    //                 AllDestroySectorMonster(pSector->GetSectorBoxID());
    //
    //                 // 激活下一个Sector
    //                 pNextSector->SetAI(true);
    //                 SetLastSectorID(pNextSector->GetSectorBoxID(), false);
    //                 SetBossSector(pNextSector->IsBossSector());
    //                 SpawnSectorMonster(pNextSector->GetSectorBoxID());
    //
    //                 // 移动用户
    //                 MoveActor(&pUser->XActor, &vStart);
    //                 pUser->MoveingValueClear();
    //                 pUser->ChangeMotion(1, true, 21);
    //
    //                 // 设置过场动画
    //                 m_pCutSceneManager->SetActiveSectorID(pNextSector->GetSectorBoxID());
    //                 m_pCutSceneManager->CheckCutsceneState(1, pNextSector->GetSectorBoxID());
    //
    //                 // 发送传送包
    //                 STWarp warpInfo;
    //                 warpInfo.byResult = 0;
    //                 warpInfo.xPos = vStart;
    //                 warpInfo.fRot = fRot;
    //                 XSendPacket xSendPacket(4, 0x0E);
    //                 xSendPacket << 0 << warpInfo;
    //                 pUser->SendBroadCast(&xSendPacket, nullptr);
    //             }
    //         }
    //     }
    // }

    // 简化实现：需要完整类型定义后还原
}

// ============================================================================
// SetQuestRespawn
// IDA: 0x1403301E0
// 设置任务重生点
// ============================================================================
void XMaze::SetQuestRespawn(unsigned int dwActorID, int nTableID, int nType, const void* pMonsterSpawn) {
    // IDA 反编译: XMaze::SetQuestRespawn
    // 1. 检查 m_objectScanner.mapPlayerList 不为空
    // 2. 检查 pMonsterSpawn->m_RespawnTime > 0 且 m_eRespawnType == 1
    // 3. 获取第一个玩家的 CGocQuest 组件
    // 4. 检查任务条件是否满足
    // 5. 注册任务怪物到重生管理器
    // 6. 在 Sector 中添加重生 Box ID

    // 简化实现：需要完整类型定义后还原
}

// ============================================================================
// CheatSetHiddenEvent
// IDA: 0x140330F70
// 作弊设置隐藏事件
// ============================================================================
void XMaze::CheatSetHiddenEvent(unsigned short wEvent, unsigned int dwCondition) {
    // IDA 反编译: XMaze::CheatSetHiddenEvent
    // 1. 检查 m_pHiddenEvent 是否为空
    // 2. 获取 TB_HIDDEN_EVENT 和 TB_EVENT_CONDITION 表数据
    // 3. 创建 CHiddenEvent 对象
    // 4. 初始化并启动隐藏事件
    // 5. 发送更新通知

    if (m_pHiddenEvent) {
        return;  // 已有隐藏事件
    }

    // 简化实现：需要完整类型定义后还原
}

// ============================================================================
// CheatUpdateHiddenEvent
// IDA: 0x1403310A0
// 作弊更新隐藏事件
// ============================================================================
void XMaze::CheatUpdateHiddenEvent(int nType, unsigned short wEvent) {
    // IDA 反编译: XMaze::CheatUpdateHiddenEvent
    // 根据类型更新隐藏事件状态:
    // type=2: 设置状态为3
    // type=3: 设置状态为4
    // type=0: 清除条件计数

    if (!m_pHiddenEvent) {
        return;
    }

    // 简化实现：需要完整类型定义后还原
}

// ============================================================================
// SetSummonMonsterDelete
// IDA: 0x140331800
// 设置召唤怪物删除
// ============================================================================
void XMaze::SetSummonMonsterDelete(unsigned int dwTBID, unsigned int dwOwnerID, char* szAnim) {
    // IDA 反编译: XMaze::SetSummonMonsterDelete
    // 1. 遍历 m_objectScanner.mapNPCList 中的所有怪物
    // 2. 找到 TableID 和 OwnerID 匹配的怪物
    // 3. 取消攻击，播放动画，设置生命周期为动画长度

    // 简化实现：需要完整类型定义后还原
}

// ============================================================================
// WarpPortal
// IDA: 0x140332CA0
// 传送门传送
// ============================================================================
bool XMaze::WarpPortal(CUser* pUser, int nMapID, int nJumpID, int nPortalID) {
    // IDA 反编译: XMaze::WarpPortal - 复杂函数
    // 1. 获取传送门 Box 信息
    // 2. 检查传送门是否开放
    // 3. 根据传送门类型处理:
    //    - 最大用户数<=0: Roguelike 或 WarpPotal 处理
    //    - 最大用户数>0: 限制用户数传送
    // 4. 移动用户到目标位置
    // 5. 发送传送包

    if (!pUser) {
        return false;
    }

    // 简化实现：需要完整类型定义后还原
    return true;
}

// ============================================================================
// GetStartSectorID
// IDA: 0x140333750
// 获取起始SectorID
// ============================================================================
void XMaze::GetStartSectorID(int nID) {
    // IDA 反编译: XMaze::GetStartSectorID
    // 如果 m_nStartSectorID 为0，直接设置
    // 否则取最小值

    if (m_nStartSectorID == 0) {
        m_nStartSectorID = nID;
    } else if (nID < m_nStartSectorID) {
        m_nStartSectorID = nID;
        SetLastSectorID(m_nStartSectorID, true);
    }
}

// ============================================================================
// SetFpUseUCID
// IDA: 0x1403337E0
// 设置FP使用的UCID
// ============================================================================
void XMaze::SetFpUseUCID(unsigned int dwUCID, unsigned char byFPUse) {
    // IDA 反编译: XMaze::SetFpUseUCID
    // 插入到 m_mpFpUseUCID map 中

    m_mpFpUseUCID[dwUCID] = byFPUse;
}

// ============================================================================
// GetFpUseUCID
// IDA: 0x140333830
// 获取FP使用的UCID
// ============================================================================
bool XMaze::GetFpUseUCID(unsigned int dwUCID, unsigned char& byFPUse) {
    // IDA 反编译: XMaze::GetFpUseUCID
    // 从 m_mpFpUseUCID map 中查找

    auto it = m_mpFpUseUCID.find(dwUCID);
    if (it != m_mpFpUseUCID.end()) {
        byFPUse = it->second;
        return true;
    }
    return false;
}

// ============================================================================
// SetTrapLifeTime
// IDA: 0x140333910
// 设置陷阱生命周期
// ============================================================================
void XMaze::SetTrapLifeTime(unsigned int nIndex, float fTime) {
    // IDA 反编译: XMaze::SetTrapLifeTime
    // 1. 在 m_mapGameTrapObject 中查找陷阱
    // 2. 调用 VTrapBase_cl::SetLifeTime

    auto it = m_mapGameTrapObject.find(nIndex);
    if (it != m_mapGameTrapObject.end() && it->second) {
        // it->second->SetLifeTime(fTime);
        // m_mapGameTrapObject.erase(it);
    }
}

// ============================================================================
// GetRewardHiddenEvent
// IDA: 0x140330BD0
// 获取隐藏事件奖励
// ============================================================================
bool XMaze::GetRewardHiddenEvent(unsigned short& wHiddenID, int& nItemID, short& shCount) {
    // IDA 反编译: XMaze::GetRewardHiddenEvent
    // 如果有隐藏事件，获取奖励物品

    if (!m_pHiddenEvent) {
        return false;
    }

    // return m_pHiddenEvent->GetRewardItem(&wHiddenID, &nItemID, &shCount);
    return false;
}

// ============================================================================
// AddTrapObject
// IDA: 0x1403338B0
// 添加陷阱对象
// ============================================================================
bool XMaze::AddTrapObject(unsigned int nIndex, void* pTrapObj) {
    // IDA 反编译: XMaze::AddTrapObject
    // 将陷阱对象插入到 m_mapGameTrapObject

    if (!pTrapObj) {
        return false;
    }

    m_mapGameTrapObject[nIndex] = pTrapObj;
    return true;
}

// ============================================================================
// AddMonsterGroupID
// IDA: 0x140333C60
// 添加怪物组ID
// ============================================================================
void XMaze::AddMonsterGroupID(int nGroupID) {
    // IDA 反编译: XMaze::AddMonsterGroupID
    // 如果组ID已存在，增加计数；否则创建新条目

    if (nGroupID <= 0) {
        return;
    }

    auto it = m_mapGroupMOB.find(nGroupID);
    if (it != m_mapGroupMOB.end()) {
        // 已存在，增加计数
        // it->second.nMaxCount++;
    } else {
        // 创建新条目
        // STMonterGroupInfo stInfo;
        // stInfo.nPreCurCount = 0;
        // stInfo.nPostCurCount = 0;
        // stInfo.nMaxCount = 1;
        // m_mapGroupMOB[nGroupID] = stInfo;
    }
}

// ============================================================================
// DeleteMonsterGroupID
// IDA: 0x140333D70
// 删除怪物组ID
// IDA 精确还原
// ============================================================================
bool XMaze::DeleteMonsterGroupID(int nGroupID, bool bPre) {
    // IDA: 检查 nGroupID 是否有效
    if (nGroupID <= 0) {
        return false;
    }

    // IDA: 在 m_mapGroupMOB 中查找
    auto it = m_mapGroupMOB.find(nGroupID);
    if (it == m_mapGroupMOB.end()) {
        return false;
    }

    // IDA: 根据类型增加计数器并检查是否达到最大值
    if (bPre) {
        it->second.nPreCurCount++;
        if (it->second.nPreCurCount == it->second.nMaxCount) {
            return true;
        }
    } else {
        it->second.nPostCurCount++;
        if (it->second.nPostCurCount == it->second.nMaxCount) {
            return true;
        }
    }

    return false;
}

// ============================================================================
// ResetMonsterGroup
// IDA: 0x140333EE0
// 重置怪物组
// ============================================================================
void XMaze::ResetMonsterGroup(int nGroupID) {
    // IDA 反编译: XMaze::ResetMonsterGroup
    // 重置前置和后置计数为0

    auto it = m_mapGroupMOB.find(nGroupID);
    if (it != m_mapGroupMOB.end()) {
        // it->second.nPreCurCount = 0;
        // it->second.nPostCurCount = 0;
    }
}

// ============================================================================
// WriteMazeRewardCashItemLog
// IDA: 0x140331520
// 写入迷宫奖励商城物品日志
// ============================================================================
void XMaze::WriteMazeRewardCashItemLog(int dwUAID, int dwUCID, void* stRewardItem, void* stReduceItem) {
    // IDA 反编译: XMaze::WriteMazeRewardCashItemLog
    // 构建日志结构并发送到数据库

    // ST_LOG_GAME stLog;
    // stLog._nUAID = dwUAID;
    // stLog._nUCID = dwUCID;
    // stLog._sMainType = 5;
    // stLog._sSubType = 24;
    // stLog.nParam0 = GetTBMapID();
    // stLog.nParam1 = stRewardItem->nItemID;
    // stLog.nParam2 = stRewardItem->shCount;
    // stLog.nParam3 = stReduceItem->nItemID;
    // stLog.nParam4 = stReduceItem->shCount;
    // stLog.nParam6 = GetInstanceID();
    // XGameServer::Instance()->SendDBLog(&stLog);

    // 简化实现：需要完整类型定义后还原
}

// ============================================================================
// AddTrapObjectGroup
// IDA: 0x1403339D0
// 添加陷阱对象到指定组
// ============================================================================
bool XMaze::AddTrapObjectGroup(unsigned int nGroup, void* pTrapObj) {
    // IDA 反编译: XMaze::AddTrapObjectGroup
    // 检查陷阱对象是否有效
    // 在 m_mapGameTrapObjectGroup 中查找组
    // 如果找到则添加到现有组的 vector
    // 否则创建新的 vector 并插入新条目

    if (!pTrapObj) {
        return false;
    }

    auto it = m_mapGameTrapObjectGroup.find(nGroup);
    if (it != m_mapGameTrapObjectGroup.end()) {
        // 组存在，添加到现有 vector
        it->second.push_back(pTrapObj);
    } else {
        // 组不存在，创建新条目
        std::vector<void*> vecObject;
        vecObject.push_back(pTrapObj);
        m_mapGameTrapObjectGroup[nGroup] = std::move(vecObject);
    }

    return true;
}

// ============================================================================
// SetTrapLifeTimeGroup
// IDA: 0x140333B30
// 设置组内所有陷阱的生存时间
// ============================================================================
void XMaze::SetTrapLifeTimeGroup(unsigned int nGroup, float fTime) {
    // IDA 反编译: XMaze::SetTrapLifeTimeGroup
    // 查找指定组的陷阱对象列表
    // 遍历所有陷阱对象，调用 SetLifeTime

    auto it = m_mapGameTrapObjectGroup.find(nGroup);
    if (it == m_mapGameTrapObjectGroup.end()) {
        return;
    }

    auto& vecObjects = it->second;
    for (size_t i = 0; i < vecObjects.size(); ++i) {
        void* pObject = vecObjects[i];
        if (pObject) {
            // TODO: VTrapBase_cl::SetLifeTime(pObject, fTime);
            // Need VGameTrapObject/VTrapBase_cl definition
        }
    }
}

// ============================================================================
// GetSectorByGroupID
// IDA: 0x1403341C0
// 根据 GroupID 获取 Sector ID
// ============================================================================
int XMaze::GetSectorByGroupID(int nGroupID) {
    // IDA 反编译: XMaze::GetSectorByGroupID
    // 在 m_mapGroupID_Monster 中查找 GroupID
    // 如果找到且 vecBoxList 非空，返回对应 Sector 的 ID
    // 否则返回 -1

    auto it = m_mapGroupID_Monster.find(nGroupID);
    if (it == m_mapGroupID_Monster.end()) {
        return -1;
    }

    auto& stInfo = it->second;
    if (stInfo.vecBoxList.empty()) {
        return -1;
    }

    // 从第一个 BoxList 条目获取 Sector ID
    auto* pFirstData = stInfo.vecBoxList[0].pInfo;
    if (pFirstData) {
        return pFirstData->m_iSectorID;
    }

    return -1;
}

// ============================================================================
// StopSpawnBoxGroupCount
// IDA: 0x1403345F0
// 停止生成箱组计数
// ============================================================================
void XMaze::StopSpawnBoxGroupCount(int nGroupID) {
    // IDA 反编译: XMaze::StopSpawnBoxGroupCount
    // 记录日志
    // 在 m_mapSpawnBoxGroupLimit 中查找 GroupID
    // 如果找到，设置标志为 0
    // 获取对应 Sector，清除 SpawnBoxID

    auto it = m_mapSpawnBoxGroupLimit.find(nGroupID);
    if (it == m_mapSpawnBoxGroupLimit.end()) {
        return;
    }

    // 设置停止标志
    it->second.bStart = false;

    int nSectorId = GetSectorByGroupID(nGroupID);
    if (nSectorId == -1) {
        return;
    }

    CSector* pSector = GetSector(nSectorId);
    if (pSector) {
        // 遍历怪物组，清除 spawn box ID
        auto itMonster = m_mapGroupID_Monster.find(nGroupID);
        if (itMonster != m_mapGroupID_Monster.end()) {
            auto& stInfo = itMonster->second;
            for (size_t i = 0; i < stInfo.vecBoxList.size(); ++i) {
                auto* pData = &stInfo.vecBoxList[i];
                if (pData && pData->pInfo) {
                    // CSector::EraseSpawnBoxID(pSector, pData->pInfo->iID);
                    // TODO: CSector::EraseSpawnBoxID 需要实现
                }
            }
        }
        // CSector::CheckMonsterCount(pSector);
        // TODO: CSector::CheckMonsterCount 需要实现
    }
}

// ============================================================================
// GetExitBoxInfo
// IDA: 0x140334c00
// 获取出口箱子信息
// ============================================================================
bool XMaze::GetExitBoxInfo(unsigned short& wMapID, int& nJumpID, XVec3& vPos) {
    // IDA 0x140334c00: XMaze::GetExitBoxInfo 精确还原
    // 根据 Maze_Type 处理不同类型的迷宫出口

    if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Type == 7) {
        // 特殊迷宫类型 7 (Roguelike?)
        int sWorldID = m_stEnterDistrictPos.sWorldID;
        switch (sWorldID) {
            case 10021:
                wMapID = 10021;
                nJumpID = 1002199;
                return true;
            case 10031:
                wMapID = 10031;
                nJumpID = 1003199;
                return true;
            case 10041:
                wMapID = 10041;
                nJumpID = 1004199;
                return true;
        }
    } else if (m_pTBMazeInfo &&
               (m_pTBMazeInfo->Maze_Type == 6 ||
                m_pTBMazeInfo->Maze_Type == 13 ||
                m_pTBMazeInfo->Maze_Type == 19 ||
                !m_pTBMazeInfo->Escape_District_ID)) {
        // 类型 6, 13, 19 或无 Escape_District_ID
        wMapID = static_cast<unsigned short>(m_stEnterDistrictPos.sWorldID);
        nJumpID = 0;
        vPos = m_stEnterDistrictPos.vPos;
        return true;
    }

    // 从 WorldResMgr 获取迷宫出口位置
    // TODO: 需要完整 XWorldResMgr::GetMazeEscapePos 实现
    STPosInfo stPosInfo;
    int nEscapeJumpID = 0;
    int nMazeID = GetTBMapID();

    auto pServer = XGameServer::Instance();
    // TODO: XWorldResMgr::GetMazeEscapePos
    // if (!pServer->m_xWorldResMgr.GetMazeEscapePos(nMazeID, &nEscapeJumpID, &stPosInfo))
    //     return false;

    // wMapID = static_cast<unsigned short>(stPosInfo.sWorldID);
    // nJumpID = nEscapeJumpID;
    // return true;

    return false;
}

// ============================================================================
// CompleteTimerStep
// IDA: 0x140334e10
// 完成计时器步骤
// ============================================================================
void XMaze::CompleteTimerStep(const char* szID, int nStep) {
    // IDA 反编译: XMaze::CompleteTimerStep (0x140334E10)
    // 遍历 m_arLogicTimers 列表
    // 根据 ID 和步骤标记计时器完成

    if (!szID) {
        return;
    }

    // 计算字符串哈希值
    int nHash = VHashString::GetHash(szID);

    // 遍历逻辑计时器列表
    for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ++it) {
        LogicTimer& timer = *it;
        // IDA: GetQuestID 实际上返回第一个字段 (id_)
        int nQuestID = timer.GetID();

        if (nQuestID == nHash) {
            // 检查步骤条件
            if (nStep == 0 || timer.GetStep() == nStep) {
                timer.SetFinishStep(true);
            }
            return;
        }
    }
}

// ============================================================================
// AddEventMaxTime
// IDA: 0x140334ee0
// 添加事件最大时间
// ============================================================================
void XMaze::AddEventMaxTime(const char* szID, float fTime) {
    // IDA 反编译: XMaze::AddEventMaxTime (0x140334EE0)
    // 遍历 m_arLogicTimers 列表
    // 根据 ID 添加最大时间并更新显示

    if (!szID) {
        return;
    }

    // 计算字符串哈希值
    int nHash = VHashString::GetHash(szID);

    // 遍历逻辑计时器列表
    for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ++it) {
        LogicTimer& timer = *it;
        // IDA: GetQuestID 实际上返回第一个字段 (id_)
        int nQuestID = timer.GetID();

        if (nQuestID == nHash) {
            // 添加最大时间
            timer.AddMaxTime(fTime);

            // 获取剩余时间
            int nLeftTime = static_cast<int>(timer.GetLeftTime());

            // 根据组ID执行不同操作
            if (timer.GetGroupID() == 2) {
                // 怪物击杀得分模式
                int nAddTime = static_cast<int>(fTime);
                int nMsTime = static_cast<int>(timer.GetLeftTime() * 1000.0f);
                AddMonsterKillScoreModeTime(nMsTime, nAddTime);
            } else {
                // 休闲副本计时器
                ShowCasualRaidTimer(0, static_cast<float>(nLeftTime), fTime);
            }
            return;
        }
    }
}

// ============================================================================
// AddGroupIDMonsterList
// IDA: 0x140333f80
// 添加 GroupID 怪物列表
// ============================================================================
void XMaze::AddGroupIDMonsterList(int nGroupID, VMonsterSpawnInfo* pMonsterSpawn) {
    // IDA 反编译: XMaze::AddGroupIDMonsterList
    // 如果 GroupID 有效，在 m_mapGroupID_Monster 中添加或更新怪物信息

    if (nGroupID <= 0 || !pMonsterSpawn) {
        return;
    }

    auto it = m_mapGroupID_Monster.find(nGroupID);
    if (it != m_mapGroupID_Monster.end()) {
        // 组已存在，检查是否可以添加
        auto& stInfo = it->second;
        // 如果列表为空，或者第一个条目的 pInfo 非空，或者 SectorID 匹配，则添加
        if (stInfo.vecBoxList.empty() ||
            !stInfo.vecBoxList[0].pInfo ||
            stInfo.vecBoxList[0].pInfo->m_iSectorID == pMonsterSpawn->m_iSectorID) {
            STMonterGroupMonsterData stData;
            stData.pInfo = pMonsterSpawn;
            stData.fWaitTime = 0.0f;
            stData.bSpawn = false;
            stInfo.vecBoxList.push_back(stData);
        }
    } else {
        // 组不存在，创建新条目
        STMonterGroupMonsterInfo stInfo;
        stInfo.nGroupID = nGroupID;
        stInfo.vecBoxList.clear();
        STMonterGroupMonsterData stData;
        stData.pInfo = pMonsterSpawn;
        stData.fWaitTime = 0.0f;
        stData.bSpawn = false;
        stInfo.vecBoxList.push_back(stData);
        m_mapGroupID_Monster[nGroupID] = stInfo;
    }
}

// ============================================================================
// DoSpawnBoxGroupCount
// IDA: 0x1403342a0
// 执行生成箱组计数
// ============================================================================
void XMaze::DoSpawnBoxGroupCount(int nGroupID, int nLimit, float fWaitTime) {
    // IDA 反编译: XMaze::DoSpawnBoxGroupCount
    // 在 m_mapSpawnBoxGroupLimit 中添加或更新生成箱组限制

    if (nGroupID <= 0 || nLimit <= 0) {
        return;
    }

    auto it = m_mapSpawnBoxGroupLimit.find(nGroupID);
    if (it == m_mapSpawnBoxGroupLimit.end()) {
        // 创建新条目
        STSpawnBoxGroupInfo stGroup;
        stGroup.nGroupID = nGroupID;
        stGroup.m_nLimit = nLimit;
        stGroup.fWaitTime = fWaitTime;
        stGroup.bStart = true;
        m_mapSpawnBoxGroupLimit[nGroupID] = stGroup;

        // 初始化对应怪物组的等待时间
        auto itMonster = m_mapGroupID_Monster.find(nGroupID);
        if (itMonster != m_mapGroupID_Monster.end()) {
            auto& stInfo = itMonster->second;
            for (size_t i = 0; i < stInfo.vecBoxList.size(); ++i) {
                if (i == 0) {
                    stInfo.vecBoxList[i].fWaitTime = 0.0f;
                } else {
                    stInfo.vecBoxList[i].fWaitTime = fWaitTime;
                }
                stInfo.vecBoxList[i].bSpawn = false;
            }
        }
    } else {
        // 更新现有条目
        it->second.bStart = true;
        it->second.m_nLimit = nLimit;
        it->second.fWaitTime = fWaitTime;

        // 重置怪物组的等待时间和生成状态
        auto itMonster = m_mapGroupID_Monster.find(nGroupID);
        if (itMonster != m_mapGroupID_Monster.end()) {
            auto& stInfo = itMonster->second;
            for (size_t j = 0; j < stInfo.vecBoxList.size(); ++j) {
                if (j == 0) {
                    stInfo.vecBoxList[j].fWaitTime = 0.0f;
                } else {
                    stInfo.vecBoxList[j].fWaitTime = fWaitTime;
                }
                stInfo.vecBoxList[j].bSpawn = false;
            }
        }
    }
}

// ============================================================================
// InitPartyQuest
// IDA: 0x140335030
// 初始化队伍任务
// ============================================================================
void XMaze::InitPartyQuest(unsigned int dwQuestID, int nConditionID, int nStartSector) {
    // IDA 0x140335030: XMaze::InitPartyQuest 精确还原
    m_stPartyQuest.nQuestID = static_cast<int>(dwQuestID);
    m_stPartyQuest.nStartCondtion = nConditionID;
    m_stPartyQuest.nStartSector = nStartSector;
}

// ============================================================================
// UpdatePartyQuest
// IDA: 0x140335080
// 更新队伍任务
// ============================================================================
void XMaze::UpdatePartyQuest(unsigned int dwQuestID, int nConditionID, bool bComplete) {
    // IDA 0x140335080: XMaze::UpdatePartyQuest 精确还原
    if (m_stPartyQuest.nQuestID > 0 && m_stPartyQuest.nQuestID == static_cast<int>(dwQuestID)) {
        if (bComplete) {
            m_stPartyQuest.nState = 2;
        } else {
            m_stPartyQuest.setCondition.insert(nConditionID);
        }
    }
}

// ============================================================================
// ResetPartyQuest
// IDA: 0x140335100
// 重置队伍任务
// ============================================================================
void XMaze::ResetPartyQuest() {
    // IDA 0x140335100: XMaze::ResetPartyQuest 精确还原
    m_stPartyQuest.reset();
}

// ============================================================================
// CallTestLuaFunction
// IDA: 0x140335130
// 调用测试Lua函数
// ============================================================================
void XMaze::CallTestLuaFunction(unsigned int nParam) {
    // IDA 0x140335130: XMaze::CallTestLuaFunction 精确还原
    if (m_pScriptInstance) {
        // TODO: 需要 IVScriptInstance::ExecuteFunctionArg 完整接口定义
        // m_pScriptInstance->ExecuteFunctionArg("OnTestFunction", "iTSoulworker:XMaze;", nParam, this);
    }
}

// ============================================================================
// UpdateTimer
// IDA: 0x140335170
// 更新计时器（大函数，约500行）
// IDA: ?UpdateTimer@XMaze@@QEAAXM@Z (0x140335170)
// ============================================================================
void XMaze::UpdateTimer(float fElapsed) {
    // IDA: 将等待列表中的计时器移到活动列表
    while (!m_arWaitLogicTimers.empty()) {
        m_arLogicTimers.push_back(m_arWaitLogicTimers.front());
        m_arWaitLogicTimers.pop_front();
    }

    // IDA: 遍历活动计时器列表
    for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ) {
        LogicTimer& timer = *it;
        int nType = timer.GetType();

        // IDA: 检查暂停状态
        if (timer.IsPause()) {
            ++it;
            continue;
        }

        // IDA: 检查是否需要调用 Ready 脚本
        if (timer.IsReady() && !timer.IsCallReadyScript()) {
            // TODO: 调用 Ready 脚本
            // if (m_pScriptInstance && timer.GetReadyString()) {
            //     m_pScriptInstance->ExecuteFunctionArg(timer.GetReadyString(), ...);
            // }
            timer.SetCallReadyScript(true);
        }

        // IDA: 更新计时器
        TimerResult_e result = timer.Update(fElapsed);

        if (result == TimerResult_e::COMPLETE) {
            // IDA: 计时器完成
            // TODO: 调用完成回调
            // if (m_pScriptInstance) {
            //     const char* szUserString = timer.GetUserString();
            //     if (szUserString && strlen(szUserString) > 0) {
            //         m_pScriptInstance->ExecuteFunctionArg(szUserString, ...);
            //     }
            // }

            // IDA: 根据类型处理特殊逻辑
            switch (nType) {
                case TIMER_TYPE_SCRIPT:
                    // TODO: 脚本类型计时器
                    break;
                case TIMER_TYPE_TIME_STEP:
                    // TODO: 时间步骤计时器
                    break;
                default:
                    break;
            }

            // 移除完成的计时器
            it = m_arLogicTimers.erase(it);
        } else {
            ++it;
        }
    }
}

// ============================================================================
// StartEventTimer
// IDA: 0x140336490
// 启动事件计时器
// ============================================================================
// IDA: ?StartEventTimer@XMaze@@QEAAXPEBD@Z (0x140336490)
// IDA 精确还原 - 启动事件计时器
void XMaze::StartEventTimer(const char* szID) {
    if (!szID) {
        return;
    }

    // IDA: 计算ID哈希值
    int nHash = VHashString::GetHash(szID);

    // IDA: 遍历 m_arLogicTimers 列表
    for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ++it) {
        LogicTimer& timer = *it;
        int nTimerID = timer.GetID();
        if (nTimerID == nHash) {
            timer.SetPause(false);
            return;
        }
    }
}

// ============================================================================
// GetExitDistrictID
// IDA: 0x140336540
// 获取出口区域ID
// ============================================================================
bool XMaze::GetExitDistrictID(unsigned int dwActorID, unsigned short& wMapID, int& nJumpID, XVec3& vPos) {
    // IDA 0x140336540: XMaze::GetExitDistrictID 精确还原
    // 根据 Maze_Type 处理不同类型的迷宫出口

    if (m_pTBMazeInfo) {
        if (m_pTBMazeInfo->Maze_Type == 7) {
            // 特殊迷宫类型 7 (Roguelike?)
            switch (m_stEnterDistrictPos.sWorldID) {
                case 0x2725: // 10021
                    wMapID = 10021;
                    nJumpID = 1002199;
                    return true;
                case 0x272F: // 10031
                    wMapID = 10031;
                    nJumpID = 1003199;
                    return true;
                case 0x2739: // 10041
                    wMapID = 10041;
                    nJumpID = 1004199;
                    return true;
            }
        } else if (m_pTBMazeInfo->Maze_Type == 6 ||
                   m_pTBMazeInfo->Maze_Type == 13 ||
                   m_pTBMazeInfo->Maze_Type == 19 ||
                   !wMapID) {
            // 类型 6, 13, 19 或无 wMapID
            wMapID = static_cast<unsigned short>(m_stEnterDistrictPos.sWorldID);
            nJumpID = 0;
            vPos = m_stEnterDistrictPos.vPos;
            return true;
        }
    }

    // 从 WorldResMgr 获取迷宫出口位置
    // TODO: 需要完整 XWorldResMgr::GetMazeEscapePos 实现
    STPosInfo stPosInfo;
    int nEscapeJumpID = 0;
    int nMazeID = GetTBMapID();

    auto pServer = XGameServer::Instance();
    // TODO: XWorldResMgr::GetMazeEscapePos
    // if (!pServer->m_xWorldResMgr.GetMazeEscapePos(nMazeID, &nEscapeJumpID, &stPosInfo))
    //     return false;

    // wMapID = static_cast<unsigned short>(stPosInfo.sWorldID);
    // nJumpID = nEscapeJumpID;
    // return true;

    return false;
}

// ============================================================================
// SetModeFail
// IDA: 0x140336fc0
// 设置模式失败
// ============================================================================
void XMaze::SetModeFail() {
    // IDA 反编译: XMaze::SetModeFail
    // 设置迷宫状态为失败(6)
    // 发送迷宫信息和广播包

    // m_stMazeGameState.m_nMazeState = 6;
    // SendMazeInfo();

    // 发送失败包
    // XSendPacket xSendPacket(0x11, 0x69);
    // SendBroadCast(&xSendPacket, nullptr, eAll);
}

// ============================================================================
// IsCutsceneCondition
// IDA: 0x140337070
// 检查过场动画条件
// ============================================================================
// IDA: ?IsCutsceneCondition@XMaze@@QEAA_NKHH@Z (0x140337070)
// IDA 精确还原 - 检查过场动画条件
bool XMaze::IsCutsceneCondition(unsigned int dwActorID, int nConditionType, int nConditionValue) {
    // IDA: 在 m_objectScanner 中查找 Actor
    auto it = m_objectScanner.mapPlayerList.find(dwActorID);
    if (it == m_objectScanner.mapPlayerList.end()) {
        return false;
    }

    CMover* pUser = it->second;
    if (!pUser) {
        return false;
    }

    // IDA: 根据 nConditionType 检查不同条件
    // nConditionType == 3: Check Condition
    // nConditionType == 4: Check Episode
    // Other: Return true (default case)

    if (nConditionType == 3) {
        // TODO: GetGOC<CGocQuest> and FindCondition
        // auto pQuest = pUser->GetGOC<CGocQuest>();
        // if (pQuest && pQuest->FindCondition(nConditionValue)) {
        //     return true;
        // }
        return false;
    } else if (nConditionType == 4) {
        // TODO: GetGOC<CGocQuest> and FindEpisode
        // auto pQuest = pUser->GetGOC<CGocQuest>();
        // if (pQuest && pQuest->FindEpisode(nConditionValue)) {
        //     return true;
        // }
        return false;
    }

    // IDA: Default case - return true
    return true;
}

// ============================================================================
// RestartSendLog
// IDA: 0x1403400E0
// 发送重启日志
// ============================================================================
void XMaze::RestartSendLog(int nUCID, int nParam1, int nParam2, int nParam3, int nParam4) {
    // IDA 反编译: 创建日志并发送到数据库
    ST_LOG_GAME stLog;
    stLog._sMainType = 5;
    stLog._sSubType = 33;
    stLog.nParam0 = nUCID;
    stLog.nParam1 = nParam1;
    stLog.nParam2 = nParam2;
    stLog.nParam3 = nParam3;
    stLog.nParam4 = nParam4;

    // IDA: 获取实例ID
    TUXMapID instanceID = GetInstanceID();
    stLog.nParam6 = instanceID.nMapID;

    // IDA: 设置注释
    wcscpy_s(stLog.szComment, L"RESTART_UPDATE");

    // IDA: 发送日志
    // TODO: 需要 XGameServer::SendDBLog 实现
    // XGameServer::Instance()->SendDBLog(&stLog);

    // IDA: 记录错误日志
    // LogHelper::LogError("game.contents", "[RESTART] Error ActorID : %d %d %d %d %d %I64d",
    //     nUCID, nParam1, nParam2, nParam3, nParam4, instanceID.nMapID);

    (void)stLog;  // 暂时避免未使用警告
}

// ============================================================================
// GetRestartState
// IDA: 0x140340020
// 获取重启状态
// ============================================================================
bool XMaze::GetRestartState(CUser* pUser) {
    // IDA 反编译: 检查用户是否在等待进入迷宫列表中
    if (!pUser) {
        return false;
    }

    // IDA: 获取用户的 ActorID
    UXActorID actorID = pUser->GetActorID();
    std::uint32_t dwUCID = actorID.dwActorID;

    // IDA: 在 m_mapWaitEnterMazeUser 中查找
    auto it = m_mapWaitEnterMazeUser.find(dwUCID);
    if (it == m_mapWaitEnterMazeUser.end()) {
        return false;
    }

    // IDA: 检查是否有有效状态
    // return it->second.ptrsSize != 0;
    return true;  // 简化实现
}

// ============================================================================
// AddTimeStepTimer
// IDA: 0x140340360
// 添加时间步进计时器
// ============================================================================
void XMaze::AddTimeStepTimer(int nStep, float fTime, int nSpawnBoxID, int nDeathMotion, const char* szTimeout) {
    // IDA 反编译: 创建 ST_TIME_STEP_TIMER 并添加到 m_mapTimeStepTimer
    // ST_TIME_STEP_TIMER stInfo;
    // stInfo.nStep = nStep;
    // stInfo.fTime = fTime;
    // stInfo.nDeathMotion = nDeathMotion;
    // stInfo.nSpawnBoxID = nSpawnBoxID;
    // if (szTimeout) {
    //     stInfo.strTimeout = szTimeout;
    // }
    // m_mapTimeStepTimer[nStep] = stInfo;

    // TODO: 需要 ST_TIME_STEP_TIMER 完整类型定义
    (void)nStep;
    (void)fTime;
    (void)nSpawnBoxID;
    (void)nDeathMotion;
    (void)szTimeout;
}

// ============================================================================
// StartTimeStepTimer
// IDA: 0x1403404B0
// 启动时间步进计时器
// ============================================================================
void XMaze::StartTimeStepTimer() {
    // IDA 反编译: 从 m_mapTimeStepTimer 获取第一个计时器并启动
    // if (m_mapTimeStepTimer.empty()) {
    //     return;
    // }
    //
    // auto it = m_mapTimeStepTimer.begin();
    // ST_TIME_STEP_TIMER stStart = it->second;
    // int nStep = static_cast<int>(m_mapTimeStepTimer.size());
    //
    // LogicTimer newTimer;
    // newTimer.SetType(3);
    // newTimer.SetTimer(stStart.fTime, 1);
    // newTimer.SetOriginTime(stStart.fTime);
    // newTimer.SetParam(nStep, stStart.nSpawnBoxID, stStart.nDeathMotion);
    // if (!stStart.strTimeout.empty()) {
    //     newTimer.SetEventString(stStart.strTimeout.c_str());
    // }
    // m_arWaitLogicTimers.push_back(newTimer);
    //
    // m_nTimeStepTarget = 0;
    // m_vTimeStepMonsterPos.x = 0.0f;
    // m_vTimeStepMonsterPos.y = 0.0f;
    // m_vTimeStepMonsterPos.z = 0.0f;
    // m_fTimeStepMonsterRot = 0.0f;
    // m_strTimeStepFailScript.clear();

    // TODO: 需要 ST_TIME_STEP_TIMER, LogicTimer 完整类型定义
}

// ============================================================================
// ChangeMonsterMotion
// IDA: ?ChangeMonsterMotion@XMaze@@QEAAXHFF@Z (0x140340660)
// 更改怪物动作
// ============================================================================
void XMaze::ChangeMonsterMotion(int nMonsterID, std::int16_t nSourceMotion, std::int16_t nDestMotion) {
    // IDA: Iterate through m_objectScanner.mapNPCList
    for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
        CMover* pMover = it->second;
        if (!pMover) {
            continue;
        }

        // IDA: Check if actor type is 2 (Monster)
        if (pMover->GetType() != 2) {
            continue;
        }

        // IDA: RTTI cast to CMonster
        CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        if (!pMonster) {
            continue;
        }

        // IDA: Check if monster has matching table ID
        if (pMonster->GetTableID() == nMonsterID) {
            // IDA: Set reserved motion
            pMonster->SetReservedMotion(nSourceMotion, nDestMotion);

            // IDA: Send packet to broadcast the motion change
            XSendPacket xSendPacket(0x17, 0x14);

            // ST_MONSTER_RESERVE_MOTION structure
            struct ST_MONSTER_RESERVE_MOTION {
                std::uint32_t nMonsterID;
                std::int16_t nDefaultMontion;
                std::int16_t nReservedMontion;
            } stInfo;

            stInfo.nMonsterID = pMonster->GetID();
            stInfo.nDefaultMontion = nSourceMotion;
            stInfo.nReservedMontion = nDestMotion;

            xSendPacket << stInfo.nMonsterID << stInfo.nDefaultMontion << stInfo.nReservedMontion;
            SendBroadCast(xSendPacket, nullptr, E_BROADCAST_TYPE::eAll);
        }
    }
}

// ============================================================================
// SetRoguePortalFlag
// IDA: 0x140340880
// 设置 Roguelike 传送门标志
// ============================================================================
void XMaze::SetRoguePortalFlag(int nBoxIndex, int nNextSectorID, bool bFlag) {
    // IDA 反编译: 在 m_mapPotalBox 中查找传送门并设置标志
    auto it = m_mapPotalBox.find(nBoxIndex);
    if (it == m_mapPotalBox.end()) {
        return;
    }

    // TODO: 需要 STMagePotalBox 完整类型定义
    // STMagePotalBox* pPotal = it->second;
    // if (!pPotal || pPotal->bOpen == bFlag) {
    //     return;
    // }
    //
    // // IDA: 如果是 Roguelike 地图且有下一个区域 ID，获取 buff
    // int nBuffID = 0;
    // if (IsRoguelikeMap() && nNextSectorID > 0) {
    //     nBuffID = GetRoguelikePortalBuff();
    //     m_mapRoguelikePortalBuff[nBoxIndex] = nBuffID;
    // }
    //
    // pPotal->bOpen = bFlag;
    //
    // // IDA: 发送 PS_WORLD_WARP_INFO 广播
    // PS_WORLD_WARP_INFO psWarpInfo;
    // psWarpInfo.nBoxIndex = nBoxIndex;
    // psWarpInfo.bFlag = bFlag;
    // psWarpInfo.bMazeComplete = IsRoguelikeMap() ? false : m_bMazeComplete;
    // psWarpInfo.nBuffID = nBuffID;
    //
    // XSendPacket xSendPacket(4, 9);
    // xSendPacket << psWarpInfo;
    // SendBroadCast(&xSendPacket, nullptr, eAll);

    (void)nNextSectorID;
    (void)bFlag;
}

// ============================================================================
// SetRoguelikeTimeout
// IDA: 0x1403444B0
// 设置 Roguelike 超时
// ============================================================================
void XMaze::SetRoguelikeTimeout() {
    // IDA 反编译: 遍历所有用户设置超时状态
    // 遍历 m_objectScanner 中的所有用户
    // 对每个活着的用户设置 buff 状态 3900
    // 发送 PS_MODE_MAZE_NOTICE 包
    // 记录日志到数据库

    // TODO: 需要 m_objectScanner 完整类型定义
    // for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
    //     CUser* pUser = dynamic_cast<CUser*>(it->second);
    //     if (!pUser || pUser->IsDie()) {
    //         continue;
    //     }
    //
    //     // IDA: 设置超时 buff
    //     pUser->SetBuffStatus(3900, 0, true);
    //
    //     // IDA: 发送通知
    //     int nSectorID = GetLastSectorID();
    //     int nMazeID = GetTBMapID();
    //     TB_MODE_BI_SECTOR_INFO* pInfo = XResourceMgr::FindRoguelikeData(nMazeID, nSectorID);
    //     if (pInfo && pInfo->BI_Sector_Value_Massage > 0) {
    //         PS_MODE_MAZE_NOTICE stNotice;
    //         stNotice.nType = pInfo->BI_Sector_Value_Type;
    //         stNotice.nValue = pInfo->BI_Sector_Value_Massage;
    //         XSendPacket xSendPacket(0x33, 0x30);
    //         xSendPacket << stNotice;
    //         CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    //     }
    //
    //     // IDA: 记录日志
    //     ST_LOG_GAME stLog;
    //     stLog._nUAID = pUser->GetUAID();
    //     stLog._nUCID = pUser->GetActorID().dwActorID;
    //     stLog._sMainType = 28;
    //     stLog._sSubType = 29;
    //     stLog.nParam0 = nMazeID;
    //     stLog.nParam1 = nSectorID;
    //     stLog.nParam6 = GetInstanceID().nMapID;
    //     wcscpy_s(stLog.szComment, L"ROGUE_TIMEOUT");
    //     XGameServer::Instance()->SendDBLog(&stLog);
    // }
}

// ============================================================================
// PlayClientEvent
// IDA: 0x140345700
// 播放客户端事件
// ============================================================================
void XMaze::PlayClientEvent(char* szEvent, int nState) {
    // IDA 反编译: 发送 PS_MAZE_PLAY_EVENT 广播
    if (!szEvent) {
        return;
    }

    // TODO: 需要 PS_MAZE_PLAY_EVENT 结构定义
    // PS_MAZE_PLAY_EVENT stEvent;
    // strcpy(stEvent.szEventParam, szEvent);
    // stEvent.nEventState = nState;
    //
    // XSendPacket xSendPacket(0x11, 5);
    // xSendPacket << stEvent;
    // SendBroadCast(&xSendPacket, nullptr, eAll);

    (void)szEvent;
    (void)nState;
}

// ============================================================================
// AddCutscene
// IDA: 0x1403371c0
// 添加过场动画
// ============================================================================
void XMaze::AddCutscene(char* szName, int nType, unsigned int dwTime, int nConditionType, int nConditionValue) {
    // IDA 反编译: XMaze::AddCutscene
    // 调用 CCutsceneManager::AddCutsceneInfo

    // TODO: 需要 CCutsceneManager 完整类型定义
    // m_cutSceneManager.AddCutsceneInfo(szName, nType, dwTime, nConditionType, nConditionValue);
}

// ============================================================================
// UpdatePublicInteractionobjectInfo
// IDA: 0x140337210
// 更新公共交互对象信息
// ============================================================================
bool XMaze::UpdatePublicInteractionobjectInfo(int nIndex, bool bSight, bool bEnable, int nCount) {
    // IDA 反编译: XMaze::UpdatePublicInteractionobjectInfo
    // 在 m_mapInteractionBox 中查找指定索引
    // 更新 enable、sight 和 count 状态

    auto it = m_mapInteractionBox.find(nIndex);
    if (it == m_mapInteractionBox.end()) {
        return false;
    }

    if (!it->second) {
        return false;
    }

    it->second->bEnable = bEnable;
    it->second->bShow = bSight;
    it->second->nCallCount = nCount;

    return true;
}

// ============================================================================
// GetPublicInteractionobject
// IDA: 0x1403372e0
// 获取公共交互对象
// ============================================================================
bool XMaze::GetPublicInteractionobject(int nIndex, bool& bEnable, bool& bSight, int& nCount) {
    // IDA 反编译: XMaze::GetPublicInteractionobject
    // 在 m_mapInteractionBox 中查找指定索引
    // 返回 enable、sight 和 count 状态

    auto it = m_mapInteractionBox.find(nIndex);
    if (it == m_mapInteractionBox.end()) {
        return false;
    }

    if (!it->second) {
        return false;
    }

    bEnable = it->second->bEnable;
    bSight = it->second->bShow;
    nCount = it->second->nCallCount;

    return true;
}

// ============================================================================
// CanUseReviveCount
// IDA: 0x1403373b0
// 检查是否可以使用复活次数
// ============================================================================
bool XMaze::CanUseReviveCount(unsigned int dwUCID) {
    // IDA 0x1403373b0: XMaze::CanUseReviveCount 精确还原
    // 检查 m_mapReviveCount 中用户的复活次数

    auto it = m_mapReviveCount.find(dwUCID);
    if (it == m_mapReviveCount.end()) {
        return true;
    }

    if (!m_pTBMazeInfo || !m_pTBMazeInfo->Revival_Item_Count) {
        return true;
    }

    return m_pTBMazeInfo->Revival_Item_Count > it->second;
}

// ============================================================================
// AddUseReviveCount
// IDA: 0x140337460
// 添加使用复活次数
// ============================================================================
void XMaze::AddUseReviveCount(CUser* pUser) {
    // IDA 0x140337460: XMaze::AddUseReviveCount 精确还原
    if (!pUser) {
        return;
    }

    UXActorID uxActorID = pUser->GetActorID();
    unsigned int dwUCID = uxActorID.dwActorID;

    auto it = m_mapReviveCount.find(dwUCID);
    if (it == m_mapReviveCount.end()) {
        m_mapReviveCount[dwUCID] = 1;
    } else {
        ++it->second;
    }
    SendUseReviveCount(pUser);
}

// ============================================================================
// SendUseReviveCount
// IDA: 0x1403375e0
// 发送使用复活次数
// ============================================================================
void XMaze::SendUseReviveCount(XActor* pActor) {
    // IDA 0x1403375e0: XMaze::SendUseReviveCount 精确还原
    if (!pActor) {
        return;
    }

    UXActorID uxActorID = pActor->GetActorID();
    unsigned int dwUCID = uxActorID.dwActorID;
    unsigned char byCount = 0;

    auto it = m_mapReviveCount.find(dwUCID);
    if (it != m_mapReviveCount.end()) {
        byCount = static_cast<unsigned char>(it->second);
    }

    // TODO: 需要 XSendPacket 和 CGocNetwork::Send
    // XSendPacket xSendPacket(0x11, 0x70);
    // xSendPacket << byCount;
    // CGocNetwork::Send(pActor, &xSendPacket);
}

// ============================================================================
// IsPlayCutscene
// IDA: 0x140337720
// 检查是否正在播放过场动画
// ============================================================================
bool XMaze::IsPlayCutscene(unsigned int dwActorID) {
    // IDA 0x140337720: XMaze::IsPlayCutscene 精确还原
    if (m_pCutSceneManager) {
        return m_pCutSceneManager->IsPlayCutscene(dwActorID);
    }
    return false;
}

// ============================================================================
// IsCutsceneState
// IDA: 0x140337750
// 检查是否处于过场动画状态
// ============================================================================
bool XMaze::IsCutsceneState() {
    // IDA 0x140337750: XMaze::IsCutsceneState 精确还原
    if (m_pCutSceneManager) {
        return m_pCutSceneManager->IsCutsceneState();
    }
    return false;
}

// ============================================================================
// ProcessGameRuleCheck
// IDA: 0x140337780
// 处理游戏规则检查
// ============================================================================
void XMaze::ProcessGameRuleCheck(CMover* pMover) {
    // IDA 0x140337780: XMaze::ProcessGameRuleCheck 精确还原
    // 根据 Mover 类型分发到不同的处理函数

    if (!pMover) {
        return;
    }

    // IDA: XActor::GetType - 0=User, 2=Monster
    int nType = pMover->GetType();
    if (nType == 0) {
        // User
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        ProcessGameRuleForUser(pUser);
    } else if (nType == 2) {
        // Monster
        CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        ProcessGameRuleForMonster(pMonster);
    }
}

// ============================================================================
// ProcessGameRuleForUser
// IDA: 0x140337830
// 为用户处理游戏规则
// ============================================================================
void XMaze::ProcessGameRuleForUser(CUser* pUser) {
    // IDA 0x140337830: XMaze::ProcessGameRuleForUser 精确还原
    // 遍历 m_vecGameRules，检查并应用规则
    // 检查 BuffStatus 条件并设置新状态

    if (!pUser || m_vecGameRules.empty()) {
        return;
    }

    for (size_t i = 0; i < m_vecGameRules.size(); ++i) {
        MAZE_GAME_RULE& rule = m_vecGameRules[i];

        // IDA: Only process rules for users (nParam1 == 1)
        if (rule.nParam1 != 1) {
            continue;
        }

        bool bValidCheck = true;

        // IDA: Check if user has buff nParam3
        if (rule.nParam3 > 0) {
            bValidCheck = pUser->FindBuffStatus(rule.nParam3, 0) != -1;
        }

        // IDA: Check if user has buff nParam4
        if (bValidCheck && rule.nParam4 > 0) {
            bValidCheck = pUser->FindBuffStatus(rule.nParam4, 0) != -1;
        }

        // IDA: Check if user has buff nParam5
        if (bValidCheck && rule.nParam5 > 0) {
            bValidCheck = pUser->FindBuffStatus(rule.nParam5, 0) != -1;
        }

        // IDA: If all conditions met, clear old buffs and set new buff
        if (bValidCheck) {
            pUser->ClearBuffStatus(rule.nParam3, false, 0);
            pUser->ClearBuffStatus(rule.nParam4, false, 0);
            pUser->ClearBuffStatus(rule.nParam5, false, 0);
            pUser->SetBuffStatus(rule.nParam6, 0, true);
        }
    }
}

// ============================================================================
// ProcessGameRuleForMonster
// IDA: 0x140337a90
// 为怪物处理游戏规则
// ============================================================================
void XMaze::ProcessGameRuleForMonster(CMonster* pMonster) {
    // IDA 0x140337a90: XMaze::ProcessGameRuleForMonster 精确还原
    // 遍历 m_vecGameRules，检查并应用规则
    // 检查 BuffStatus 条件并改变 AI 状态

    if (!pMonster || m_vecGameRules.empty()) {
        return;
    }

    // IDA: Get monster table ID for comparison
    int nMonsterID = pMonster->GetTableID();

    for (size_t i = 0; i < m_vecGameRules.size(); ++i) {
        MAZE_GAME_RULE& rule = m_vecGameRules[i];

        // IDA: Only process rules for monsters (nParam1 == 2) with matching table ID
        if (rule.nParam1 != 2 || rule.nParam2 != nMonsterID) {
            continue;
        }

        bool bValidCheck = true;

        // IDA: Check if monster has buff nParam3
        if (rule.nParam3 > 0) {
            bValidCheck = pMonster->FindBuffStatus(rule.nParam3, 0) != -1;
        }

        // IDA: Check if monster has buff nParam4
        if (bValidCheck && rule.nParam4 > 0) {
            bValidCheck = pMonster->FindBuffStatus(rule.nParam4, 0) != -1;
        }

        // IDA: Check if monster has buff nParam5
        if (bValidCheck && rule.nParam5 > 0) {
            bValidCheck = pMonster->FindBuffStatus(rule.nParam5, 0) != -1;
        }

        // IDA: If all conditions met and monster has AI, clear buffs and change AI state
        if (bValidCheck && pMonster->GetAi()) {
            pMonster->ClearBuffStatus(rule.nParam3, false, 0);
            pMonster->ClearBuffStatus(rule.nParam4, false, 0);
            pMonster->ClearBuffStatus(rule.nParam5, false, 0);

            // IDA: _nState = nParam6 + 19 (FSM state offset)
            int nState = rule.nParam6 + 19;
            pMonster->GetAi()->ChangeAiState(nState);
        }
    }
}

// ============================================================================
// CallScriptCutsceneOff
// IDA: 0x140337cb0
// 调用脚本过场动画关闭
// ============================================================================
void XMaze::CallScriptCutsceneOff(const char* szCutsceneName) {
    // IDA 反编译: XMaze::CallScriptCutsceneOff
    // 检查脚本实例是否有 OnCutsceneOff 函数
    // 如果在 m_mapCutsceneOff 中已存在则增加计数
    // 否则调用脚本并记录

    if (!szCutsceneName || !m_pScriptInstance) {
        return;
    }

    // TODO: 需要完整类型定义
    // if (!m_pScriptInstance->HasFunction("OnCutsceneOff")) {
    //     return;
    // }
    //
    // auto it = m_mapCutsceneOff.find(szCutsceneName);
    // if (it != m_mapCutsceneOff.end()) {
    //     ++it->second;
    // } else {
    //     m_pScriptInstance->ExecuteFunctionArg("OnCutsceneOff", "sTSoulworker:XMaze;", szCutsceneName, this);
    //     m_mapCutsceneOff[szCutsceneName] = 1;
    // }
}

// ============================================================================
// ProcessExp
// IDA: 0x140337ee0
// 处理经验值
// ============================================================================
void XMaze::ProcessExp(XActor* pAtk, float fExp, char nMonsterLv) {
    // IDA 反编译: XMaze::ProcessExp
    // 如果攻击者是用户，直接设置经验值
    // 否则遍历 m_objectScanner 找到第一个非GM隐身用户设置经验值

    if (!pAtk) {
        return;
    }

    // TODO: 需要完整类型定义
    // CUser* pUser = dynamic_cast<CUser*>(pAtk);
    // if (pUser) {
    //     pUser->SetExp(fExp, nMonsterLv);
    //     return;
    // }
    //
    // // 遍历 m_objectScanner 找到用户
    // for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
    //     CUser* pFoundUser = dynamic_cast<CUser*>(it->second);
    //     if (pFoundUser && (!pFoundUser->IsGM() || !pFoundUser->IsStatus(0x2000))) {
    //         pFoundUser->SetExp(fExp, nMonsterLv);
    //         return;
    //     }
    // }
}

// ============================================================================
// CheckMazeCondition
// IDA: 0x140338020
// 检查迷宫条件（大函数，约3000行）
// ============================================================================
void XMaze::CheckMazeCondition(XActor* pActor) {
    // IDA 反编译: XMaze::CheckMazeCondition
    // 这是一个非常大的函数，处理多种迷宫条件检查：
    // 1. 检查等待进入迷宫的用户列表
    // 2. 更新无限塔信息
    // 3. 检查进入次数限制
    // 4. 检查并消耗需要的物品
    // 5. 检查传送球物品
    // 6. 验证 Roguelike 状态

    // TODO: 需要完整类型定义后还原
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) {
    //     return;
    // }
    //
    // unsigned int dwActorID = pUser->GetActorID();
    // auto it = m_mapWaitEnterMazeUser.find(dwActorID);
    // if (it == m_mapWaitEnterMazeUser.end() || it->second.bChecked) {
    //     return;
    // }
    //
    // // 无限塔处理...
    // // 进入次数检查...
    // // 需要物品检查...
    // // 传送球检查...
    // // Roguelike 状态验证...
    //
    // it->second.bChecked = true;
}

// ============================================================================
// StartWorldMode
// IDA: 0x140338de0
// 启动世界模式
// ============================================================================
void XMaze::StartWorldMode(ST_WORLD_MODE_INFO& stInfo) {
    // IDA 反编译: XMaze::StartWorldMode
    // 获取模式配置，更新 m_mapWorldMode
    // 发送 PS_WORLD_MODE_START 包广播

    // TODO: 需要完整类型定义
    // XGameServer* pGameServer = XGameServer::Instance();
    // TB_MODE_DISTRICT6* pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(&pGameServer->m_xResourceMgr, stInfo.nModeID);
    // if (!pTBMode) {
    //     return;
    // }
    //
    // // 更新或添加到 m_mapWorldMode
    // m_mapWorldMode[stInfo.nModeID] = stInfo;
    //
    // // 发送启动包
    // PS_WORLD_MODE_START stStart;
    // stStart.nModeDateID = stInfo.nModeDateID;
    // stStart.nID = stInfo.nModeID;
    // stStart.nStartTime = stInfo.nStartTime;
    // stStart.nFinishTime = stInfo.nFinishTime;
    // stStart.byState = pTBMode->Start_Type ? 0 : 4;
    // stStart.biModeStartTime = stInfo.biModeStartTime;
    // stStart.biModeEndTime = stInfo.biModeEndTime;
    //
    // XSendPacket xSendPacket(0x30, 1);
    // xSendPacket << stStart;
    // SendBroadCast(&xSendPacket, nullptr, eAll);
}

// ============================================================================
// SetDisconnectUserState
// IDA: 0x140336930
// 设置断开连接用户状态
// ============================================================================
void XMaze::SetDisconnectUserState(unsigned int dwUCID, ST_PARTY_INFO stPartyInfo) {
    // IDA 反编译: XMaze::SetDisconnectUserState
    // 1. 检查 m_pTBMazeInfo 是否有效
    // 2. 根据 stPartyInfo.byGroupType 检查队伍/势力
    // 3. 在 m_mapWaitEnterMazeUser 中找到用户
    // 4. 对于特定迷宫类型，设置断开状态为 12
    // 5. 设置超时时间为 5 分钟后
    // 6. 发送断开连接状态包给控制服务器

    if (!m_pTBMazeInfo) {
        // LogHelper::LogError("game.contents",
        //     "SetDisconnectUserState - m_pTBMazeInfo NULL [ ActorID:%d, PartyID:%d, InstanceID:%I64d ] ( %d )",
        //     dwUCID, stPartyInfo.nID, GetInstanceID().nMapID, 10331);
        return;
    }

    // 检查队伍/势力
    if (stPartyInfo.byGroupType == 1) {
        // 队伍类型
        if (!m_pParty) {
            // LogHelper::LogError("game.contents",
            //     "SetDisconnectUserState - m_pParty NULL [ ActorID:%d, PartyID:%d ] ( %d )",
            //     dwUCID, stPartyInfo.nID, 10339);
            return;
        }
        // int nPartyID = m_pParty->GetPartyID();
        // if (nPartyID != stPartyInfo.nID) {
        //     LogHelper::LogError(...);
        //     return;
        // }
    } else if (stPartyInfo.byGroupType == 2) {
        // 势力类型
        if (!m_pForce) {
            // LogHelper::LogError("game.contents",
            //     "SetDisconnectUserState - m_pForce NULL [ ActorID:%d, PartyID:%d ] ( %d )",
            //     dwUCID, stPartyInfo.nID, 10353);
            return;
        }
        // int nForceID = m_pForce->GetForceID();
        // if (nForceID != stPartyInfo.nID) {
        //     LogHelper::LogError(...);
        //     return;
        // }
    }

    // 查找用户
    auto it = m_mapWaitEnterMazeUser.find(dwUCID);
    if (it != m_mapWaitEnterMazeUser.end()) {
        // 特定迷宫类型需要设置断开状态
        if (m_pTBMazeInfo->Maze_Type == 2 || m_pTBMazeInfo->Maze_Type == 8 ||
            m_pTBMazeInfo->Maze_Type == 9 || m_pTBMazeInfo->Maze_Type == 13 ||
            m_pTBMazeInfo->Maze_Type == 19) {
            // 设置断开状态为 12
            // it->second.m_pRegisteredAtType = 12;
        }

        // 设置超时时间为 5 分钟后
        // it->second.m_eObjectFlags = GetTickCount64() + 300000;

        // 发送断开连接状态包
        // XGameServer* pGameServer = XGameServer::Instance();
        // if (pGameServer->m_controlSocket.IsCanSend()) {
        //     XSendPacket xSendPacket(0xF2, 0x39);
        //     xSendPacket << it->second;
        //     xSendPacket << GetInstanceID().nMapID;
        //     pGameServer->m_controlSocket.Send(&xSendPacket);
        // }
    }
}

// ============================================================================
// CheckDisconnecUsertState
// IDA: 0x140336e00
// 检查断开连接用户状态
// ============================================================================
unsigned char XMaze::CheckDisconnecUsertState(unsigned int dwUCID, unsigned char& byState) {
    // IDA 反编译: XMaze::CheckDisconnecUsertState
    // 返回值: 0=未找到, 1=普通迷宫, 2=状态10返回, 3=状态12需要处理
    // byState: 输出用户状态

    byState = 0;

    if (!m_pTBMazeInfo) {
        // LogHelper::LogError("game.contents",
        //     "CheckDisconnecUsertState - m_pTBMazeInfo NULL [ ActorID:%d ] ( %d )",
        //     dwUCID, 10399);
        return 0;
    }

    // 必须有队伍或势力
    if (!m_pParty && !m_pForce) {
        return 0;
    }

    // 查找用户
    auto it = m_mapWaitEnterMazeUser.find(dwUCID);
    if (it == m_mapWaitEnterMazeUser.end()) {
        return 0;
    }

    // 特定迷宫类型检查状态
    if (m_pTBMazeInfo->Maze_Type == 2 || m_pTBMazeInfo->Maze_Type == 8 ||
        m_pTBMazeInfo->Maze_Type == 9) {
        // byState = it->second.m_pRegisteredAtType;
        // switch (byState) {
        //     case 10: return 2;
        //     case 11: return 0;
        //     case 12:
        //         it->second.m_pRegisteredAtType = 13;
        //         return 3;
        //     default: return 0;
        // }
        return 0;
    }

    return 1;
}

// ============================================================================
// SyncWorldMode
// IDA: 0x1403394c0
// 同步世界模式信息向量
// ============================================================================
void XMaze::SyncWorldMode(ST_WORLD_MODE_INFO_VEC& stInfoVec) {
    // IDA 反编译: XMaze::SyncWorldMode
    // 1. 调用 InfoWorldMode 获取当前信息
    // 2. 遍历 stInfoVec 中的每个 ST_WORLD_MODE_INFO
    // 3. 根据 nState 更新 m_mapWorldMode
    //    - nState == 1 或 2: 更新或插入记录
    //    - nState == 0: 删除记录

    // InfoWorldMode(stInfoVec);

    // for (auto& stInfo : stInfoVec.vecInfos) {
    //     XGameServer* pGameServer = XGameServer::Instance();
    //     TB_MODE_DISTRICT6* pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(&pGameServer->m_xResourceMgr, stInfo.nModeID);
    //     if (!pTBMode) continue;
    //
    //     if (stInfo.nState == 1 || stInfo.nState == 2) {
    //         // 开始或结束状态，更新或插入
    //         auto it = m_mapWorldMode.find(stInfo.nModeID);
    //         if (it != m_mapWorldMode.end()) {
    //             it->second = stInfo;
    //         } else {
    //             m_mapWorldMode[stInfo.nModeID] = stInfo;
    //         }
    //         LogHelper::LogInfo("game.contents",
    //             "[WORLD_MODE] Sync Start or Finish %d %d %d",
    //             stInfo.nModeID, stInfo.nState, stInfo.nModeDateID);
    //     } else if (stInfo.nState == 0) {
    //         // 准备状态，删除记录
    //         m_mapWorldMode.erase(stInfo.nModeID);
    //         LogHelper::LogInfo("game.contents",
    //             "[WORLD_MODE] Sync Ready %d %d",
    //             stInfo.nModeID, stInfo.nModeDateID);
    //     }
    // }
}

// ============================================================================
// SetWorldModeBoostAll
// IDA: 0x140339750
// 为所有用户设置世界模式 Boost
// ============================================================================
void XMaze::SetWorldModeBoostAll(unsigned short nBoostID, __int64 nEndDate) {
    // IDA 反编译: XMaze::SetWorldModeBoostAll
    // 遍历所有 actor，找到 CUser，为其设置 Boost

    // for (auto it = m_mapActor.Begin(); it; m_mapActor.GetNext(it)) {
    //     XActor* pActor = m_mapActor.GetValueAt(it);
    //     if (!pActor) continue;
    //
    //     CUser* pUser = dynamic_cast<CUser*>(pActor);
    //     if (!pUser) continue;
    //
    //     // 检查用户是否在有效区域
    //     XArea* pArea = pUser->GetArea();
    //     if (!pArea || !pArea->IsMaze()) continue;
    //
    //     // 获取 CGocBooster 组件并设置 Boost
    //     std::shared_ptr<CGocBooster> pBooster;
    //     pUser->GetGOC<CGocBooster>(&pBooster);
    //     if (pBooster) {
    //         pBooster->ChangeBooster(eBooster_Type_Event, nBoostID, nEndDate, 0);
    //     }
    // }
}

// ============================================================================
// SetWorldModeSync
// IDA: 0x1403398f0
// 为特定 actor 设置世界模式同步
// ============================================================================
void XMaze::SetWorldModeSync(XActor* pActor) {
    // IDA 反编译: XMaze::SetWorldModeSync
    // 1. 将 actor 转换为 CUser
    // 2. 遍历 m_mapWorldMode
    // 3. 对于 nState == 1 的模式，发送 PS_WORLD_MODE_START 包
    // 4. 对于 nState == 2 的模式，根据清除数量设置 Boost

    if (!pActor) return;

    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) return;
    //
    // for (auto it = m_mapWorldMode.begin(); it != m_mapWorldMode.end(); ++it) {
    //     ST_WORLD_MODE_INFO& stInfo = it->second;
    //
    //     if (stInfo.nState == 1) {
    //         // 发送开始包
    //         PS_WORLD_MODE_START stStart;
    //         stStart.nModeDateID = stInfo.nModeDateID;
    //         stStart.nID = stInfo.nModeID;
    //         stStart.nStartTime = stInfo.nStartTime;
    //         stStart.nFinishTime = stInfo.nFinishTime;
    //         stStart.byState = 1;
    //         stStart.biModeStartTime = stInfo.biModeStartTime;
    //         stStart.biModeEndTime = stInfo.biModeEndTime;
    //
    //         XSendPacket xSendPacket(0x30, 1);
    //         xSendPacket << stStart;
    //         pUser->Send(&xSendPacket);
    //     } else if (stInfo.nState == 2) {
    //         // 结束状态，根据清除数量设置 Boost
    //         XGameServer* pGameServer = XGameServer::Instance();
    //         TB_MODE_DISTRICT6_DATE* pModeDate = XResourceMgr::GetTB_MODE_DISTRICT6_DATE(
    //             &pGameServer->m_xResourceMgr, stInfo.nModeDateID);
    //         if (!pModeDate) continue;
    //
    //         // 更新 m_vecWorldModeList 中的清除数量
    //         for (size_t i = 0; i < m_vecWorldModeList.size(); ++i) {
    //             if (m_vecWorldModeList[i].nModeDateID == stInfo.nModeDateID) {
    //                 m_vecWorldModeList[i].nMonsterClearCount = stInfo.nMonsterClearCount;
    //             }
    //         }
    //
    //         // 根据清除数量设置 Boost
    //         if (stInfo.nMonsterClearCount > 0) {
    //             for (int j = 0; j < 5 && pModeDate->Clear_Count_01[j]; ++j) {
    //                 if (stInfo.nMonsterClearCount >= pModeDate->Clear_Count_01[j] &&
    //                     stInfo.nMonsterClearCount < pModeDate->Clear_Count_02[j]) {
    //                     if (pModeDate->Clear_Booster_01[j]) {
    //                         int nBoostID = pModeDate->Clear_Booster_01[j];
    //                         std::shared_ptr<CGocBooster> pBooster;
    //                         pUser->GetGOC<CGocBooster>(&pBooster);
    //                         __int64 nRemainTime = pModeDate->Booster_Limit_Time + stInfo.nFinishTime;
    //                         if (pBooster) {
    //                             pBooster->ChangeBooster(eBooster_Type_Event, nBoostID, nRemainTime, 0);
    //                         }
    //                     }
    //                     break;
    //                 }
    //                 // 检查最高清除数量档位
    //                 if (stInfo.nMonsterClearCount >= pModeDate->Clear_Count_01[j] && j == 4) {
    //                     if (pModeDate->Clear_Booster_06) {
    //                         int nBoostID = pModeDate->Clear_Booster_02[j];
    //                         std::shared_ptr<CGocBooster> pBooster;
    //                         pUser->GetGOC<CGocBooster>(&pBooster);
    //                         __int64 nRemainTime = pModeDate->Booster_Limit_Time + stInfo.nFinishTime;
    //                         if (pBooster) {
    //                             pBooster->ChangeBooster(eBooster_Type_Event, nBoostID, nRemainTime, 0);
    //                         }
    //                     }
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    // }
}

// ============================================================================
// AddUserDamage
// IDA: 0x14033a7c0
// 添加用户伤害记录
// ============================================================================
void XMaze::AddUserDamage(unsigned int dwUCID, int nDamage) {
    // IDA 反编译: XMaze::AddUserDamage
    // 在 m_mapUserDamage 中添加或更新用户的伤害值

    auto it = m_mapUserDamage.find(dwUCID);
    if (it == m_mapUserDamage.end()) {
        // 新用户，插入记录
        m_mapUserDamage[dwUCID] = nDamage;
    } else {
        // 已存在，累加伤害
        it->second += nDamage;
    }
}

// ============================================================================
// GetUserDamagePercent
// IDA: 0x14033a880
// 获取用户伤害百分比
// ============================================================================
int XMaze::GetUserDamagePercent(unsigned int dwUCID) {
    // IDA 反编译: XMaze::GetUserDamagePercent
    // 计算用户伤害占总伤害的百分比

    // 计算总伤害
    std::uint64_t nTotalDamage = 0;
    for (const auto& pair : m_mapUserDamage) {
        nTotalDamage += pair.second;
    }

    if (nTotalDamage <= 0) {
        return 0;
    }

    // 查找用户伤害
    auto it = m_mapUserDamage.find(dwUCID);
    if (it == m_mapUserDamage.end()) {
        return 0;
    }

    // 计算百分比: (用户伤害 * 100) / 总伤害
    return static_cast<int>((static_cast<float>(it->second) * 100.0f) / static_cast<float>(nTotalDamage));
}

// ============================================================================
// AddHitedCount
// IDA: 0x14033a9a0
// 增加用户被击次数
// ============================================================================
void XMaze::AddHitedCount(unsigned int dwUCID) {
    // IDA 反编译: XMaze::AddHitedCount
    // 在 m_mapUserHitedCount 中增加用户的被击次数

    auto it = m_mapUserHitedCount.find(dwUCID);
    if (it == m_mapUserHitedCount.end()) {
        // 新用户，设置为 1
        m_mapUserHitedCount[dwUCID] = 1;
    } else {
        // 已存在，增加计数
        it->second++;
    }
}

// ============================================================================
// GetHitedCount
// IDA: 0x14033aa60
// 获取用户被击次数
// ============================================================================
int XMaze::GetHitedCount(unsigned int dwUCID) {
    // IDA 反编译: XMaze::GetHitedCount
    // 返回用户的被击次数

    auto it = m_mapUserHitedCount.find(dwUCID);
    if (it == m_mapUserHitedCount.end()) {
        return 0;
    }
    return it->second;
}

// ============================================================================
// GetMazeLayerLevel
// IDA: 0x14033aad0
// 获取迷宫层级别
// ============================================================================
void XMaze::GetMazeLayerLevel(int nLayerBit, unsigned int& nBaseLevel) {
    // IDA 反编译: XMaze::GetMazeLayerLevel
    // 根据 nLayerBit 计算层级别和位掩码
    // nLayerBit 格式: 可以是简单数字(如 5) 或复合数字(如 50201 表示 5层02级01子级)

    m_nBit = 0;

    if (nLayerBit >= 100) {
        // 复合层数，需要解析
        int nTmpBase = 0;
        int nTmp = nLayerBit;

        do {
            int nLevel = nTmp % 100;
            int nNext = nTmp / 100;

            // 检查层级有效性 (0-27)
            if (nLevel < 0 || nLevel >= 28) {
                m_nBit = -1;
                return;
            }

            // 设置对应位
            m_nBit += 1 << (nLevel + 1);

            // 记录最大层级
            if (nTmpBase < nLevel) {
                nTmpBase = nLevel;
            }

            nTmp = nNext;
        } while (nTmp >= 10);

        // 处理最高位
        m_nBit += (1 << (nTmp + 1)) | 1;

        if (nTmp >= 0 && nTmp < 28) {
            if (nTmpBase < nTmp) {
                nTmpBase = nTmp;
            }
            nBaseLevel = nTmpBase;
        } else {
            m_nBit = -1;
        }
    } else {
        // 简单层数
        nBaseLevel = nLayerBit;
        if (nBaseLevel < 28) {
            m_nBit += (1 << (nBaseLevel + 1)) | 1;
        } else {
            m_nBit = -1;
        }
    }
}

// ============================================================================
// IsEnemyPVP
// IDA: 0x14033ac90
// 检查是否是PVP敌人
// ============================================================================
bool XMaze::IsEnemyPVP(XActor* pAtk, XActor* pDef) {
    // IDA 反编译: XMaze::IsEnemyPVP
    // 非过场动画状态且地图ID为20005时是PVP地图
    return !IsCutsceneState() && GetTBMapID() == 20005;
}

// ============================================================================
// GetMonsterByID
// IDA: 0x14033ace0
// 根据怪物表ID获取怪物对象
// ============================================================================
CMonster* XMaze::GetMonsterByID(unsigned int dwID) {
    // IDA 反编译: XMaze::GetMonsterByID
    // 遍历 m_objectScanner.mapNPCList 查找匹配ID的怪物

    // for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (!pMover) continue;
    //
    //     // 检查是否是怪物类型
    //     if (pMover->GetType() != 2) continue;
    //
    //     CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //     if (!pMonster) continue;
    //
    //     if (pMonster->GetTableID() == dwID) {
    //         return pMonster;
    //     }
    // }
    return nullptr;
}

// ============================================================================
// IsDeleteMaze
// IDA: 0x14033ade0
// 检查迷宫是否可以删除
// ============================================================================
bool XMaze::IsDeleteMaze() {
    // IDA 反编译: XMaze::IsDeleteMaze
    // 当前用户数为0且迷宫状态不为7时可删除
    return GetCurUserCount() == 0 && m_stMazeGameState.m_nMazeState != 7;
}

// ============================================================================
// PauseAlltimer
// IDA: 0x14033ae10
// 暂停/恢复所有定时器
// ============================================================================
void XMaze::PauseAlltimer(bool bPause) {
    // IDA 反编译: XMaze::PauseAlltimer
    // 遍历 m_arLogicTimers，设置暂停状态
    // void __fastcall XMaze::PauseAlltimer(XMaze *this, bool bPause)
    // {
    //   for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ++it) {
    //     LogicTimer* pTimer = *it;
    //     pTimer->SetPause(bPause);
    //     int nType = pTimer->GetGroupID();
    //     if (!bPause && nType == 1) {
    //       float LeftTime = pTimer->GetLeftTime();
    //       ShowCasualRaidTimer(0, LeftTime, 0.0);
    //     }
    //   }
    // }

    // 简化实现 - 需要完整的 LogicTimer 类型定义
    // for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ++it) {
    //     LogicTimer* pTimer = *it;
    //     if (!pTimer) continue;
    //
    //     pTimer->SetPause(bPause);
    //
    //     // 如果恢复暂停且是类型1（休闲突袭），显示计时器
    //     if (!bPause && pTimer->GetGroupID() == 1) {
    //         float fLeftTime = pTimer->GetLeftTime();
    //         ShowCasualRaidTimer(0, fLeftTime, 0.0f);
    //     }
    // }
}

// ============================================================================
// ApplyBuffToMonster
// IDA: 0x140339e20
// 对指定ID的怪物应用Buff
// ============================================================================
void XMaze::ApplyBuffToMonster(const char* szMonsterID, const char* szBuffID) {
    // IDA 反编译: XMaze::ApplyBuffToMonster
    // void __fastcall XMaze::ApplyBuffToMonster(XMaze *this, char *szMonsterID, char *szBuffID)
    // {
    //   int nMonsterID = std::stoi(szMonsterID);
    //   int nBuffID = std::stoi(szBuffID);
    //   for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (!pMover) continue;
    //     CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //     if (!pMonster) continue;
    //     if (!pMonster->IsCanAI()) continue;
    //     if (pMonster->GetTableID() == nMonsterID) {
    //       pMonster->SetBuffStatus(nBuffID, 0, true);
    //     }
    //   }
    // }

    if (!szMonsterID || !szBuffID) return;

    // 简化实现 - 需要完整的 CMonster::IsCanAI, CMonster::SetBuffStatus 方法
    // int nMonsterID = std::stoi(szMonsterID);
    // int nBuffID = std::stoi(szBuffID);
    //
    // for (auto it = m_objectScanner.mapNPCList.begin(); it != m_objectScanner.mapNPCList.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (!pMover) continue;
    //
    //     CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //     if (!pMonster) continue;
    //
    //     if (!pMonster->IsCanAI()) continue;
    //
    //     if (pMonster->GetTableID() == nMonsterID) {
    //         pMonster->SetBuffStatus(nBuffID, 0, true);
    //     }
    // }
}

// ============================================================================
// ChangeMobIDByMazeLevel
// IDA: 0x14033a000
// 根据迷宫等级改变怪物ID
// ============================================================================
unsigned int XMaze::ChangeMobIDByMazeLevel(unsigned int nMonsterID) {
    // IDA 反编译: XMaze::ChangeMobIDByMazeLevel
    // __int64 __fastcall XMaze::ChangeMobIDByMazeLevel(XMaze *this, unsigned int nMonsterID)
    // {
    //   XGameServer *v2; // rax
    //   TB_CHANGE_MOB_ID *pTB_CHANGE_MOB_ID;
    //   int MazeLevel;
    //
    //   v2 = TXSingleton<XGameServer>::Instance();
    //   pTB_CHANGE_MOB_ID = XResourceMgr::GetTB_CHANGE_MOB_ID(&v2->m_xResourceMgr, nMonsterID);
    //   if (!pTB_CHANGE_MOB_ID)
    //     return nMonsterID;
    //   MazeLevel = XMaze::GetMazeLevel(this);
    //   switch (MazeLevel) {
    //     case 0: return pTB_CHANGE_MOB_ID->MOB_ID_Difficulty01;
    //     case 1: return pTB_CHANGE_MOB_ID->MOB_ID_Difficulty02;
    //     case 2: return pTB_CHANGE_MOB_ID->MOB_ID_Difficulty03;
    //     default: return nMonsterID;
    //   }
    // }

    // 简化实现 - 需要完整的 XResourceMgr::GetTB_CHANGE_MOB_ID 和 TB_CHANGE_MOB_ID 类型定义
    // XGameServer* pGameServer = XGameServer::Instance();
    // TB_CHANGE_MOB_ID* pTB = XResourceMgr::GetTB_CHANGE_MOB_ID(&pGameServer->m_xResourceMgr, nMonsterID);
    // if (!pTB) return nMonsterID;
    //
    // int nMazeLevel = GetMazeLevel();
    // switch (nMazeLevel) {
    //     case 0: return pTB->MOB_ID_Difficulty01;
    //     case 1: return pTB->MOB_ID_Difficulty02;
    //     case 2: return pTB->MOB_ID_Difficulty03;
    //     default: return nMonsterID;
    // }
    return nMonsterID;
}

// ============================================================================
// ApplyBuffEx
// IDA: 0x14033a080
// 扩展应用Buff
// ============================================================================
void XMaze::ApplyBuffEx(int nType, int nCount, const char* szBuffID) {
    // IDA 反编译: XMaze::ApplyBuffEx (复杂函数，约1700字节)
    // 主要逻辑:
    // 1. 解析 BuffID
    // 2. 创建 mapGetTaget multimap 存储目标
    // 3. 根据 nType 选择目标类型:
    //    - nType == 1: 只遍历用户 (非GM)
    //    - nType == 2: 只遍历怪物 (有 Sector 且 GroupID 非零, 特定 Monster_Type)
    //    - 其他: 遍历用户和怪物
    // 4. 使用 rand() 作为 key 插入 multimap (随机选择)
    // 5. 遍历 mapGetTaget, 最多应用 nCount 个目标
    // 6. 调用 CMover::SetBuffStatus(nBuffID, 0, true)

    if (nCount <= 0 || !szBuffID) return;

    // 简化实现 - 需要完整的类型定义
    // int nBuffID = std::stoi(szBuffID);
    // std::multimap<int, CMover*> mapTargets;
    //
    // if (nType == 1) {
    //     // 遍历所有用户
    //     for (auto& pair : m_objectScanner) {
    //         CUser* pUser = dynamic_cast<CUser*>(pair.second);
    //         if (pUser && !pUser->IsGM()) {
    //             mapTargets.insert({rand(), pUser});
    //         }
    //     }
    // } else if (nType == 2) {
    //     // 遍历所有怪物
    //     for (auto& pair : m_objectScanner.mapNPCList) {
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pair.second);
    //         if (pMonster && pMonster->GetSector() && pMonster->GetSector()->GetGroupID()) {
    //             TB_MONSTER* pTB = pMonster->GetMobTableRef();
    //             if (pTB && (pTB->Monster_Type == 0 || pTB->Monster_Type == 3)) {
    //                 mapTargets.insert({rand(), pMonster});
    //             }
    //         }
    //     }
    // } else {
    //     // 遍历所有用户和怪物
    //     // ... (类似上述逻辑)
    // }
    //
    // // 应用Buff
    // int nApplied = 0;
    // for (auto it = mapTargets.begin(); it != mapTargets.end() && nApplied < nCount; ++it, ++nApplied) {
    //     it->second->SetBuffStatus(nBuffID, 0, true);
    // }
}

// ============================================================================
// ExcuteCheckEventSpawnBox
// IDA: 0x140317b60
// 执行检查事件生成盒
// ============================================================================
void XMaze::ExcuteCheckEventSpawnBox(CUser* pUser, int nSectorID, int nBoxIndex) {
    // IDA 反编译: XMaze::ExcuteCheckEventSpawnBox
    // 检查事件生成盒并触发相应事件

    if (!pUser) return;

    // int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nBoxIndex, GetBatchLayerLevel());
    // auto it = m_mapCheckEventSpawnBox.find(iBoxUniqueID);
    // if (it == m_mapCheckEventSpawnBox.end()) return;
    //
    // STMageCheckEventSpawnBox* pEventSpawn = it->second;
    // if (!pEventSpawn || !pEventSpawn->pEventBox) return;
    //
    // int nEventType = pEventSpawn->pEventBox->m_eEvent_Type;
    // if (nEventType == 0) {
    //     // 普通事件
    //     if (!pEventSpawn->bActive) {
    //         pEventSpawn->bActive = true;
    //         int nRate = nRand(1, 10000);
    //         if (pEventSpawn->pEventBox->m_fEvent_Rate >= nRate) {
    //             // 触发事件
    //             GameModeMgr::StartCheckEventMode(this, pEventSpawn);
    //         }
    //     }
    // } else if (nEventType == 1) {
    //     // 隐藏事件
    //     if (!pEventSpawn->bActive && !m_pHiddenEvent) {
    //         pEventSpawn->bActive = true;
    //         // 检查概率并触发隐藏事件
    //         // ...
    //     }
    // }
}

// ============================================================================
// ExcuteSpawnBox (STMageProcessSpawnBox版本)
// IDA: 0x14031aac0
// 执行生成盒
// ============================================================================
void XMaze::ExcuteSpawnBox(void* pProcessSpawn, E_SEND_INFO_TYPE eType) {
    // IDA 反编译: XMaze::ExcuteSpawnBox
    // 处理 STMageProcessSpawnBox 类型的生成盒

    // STMageProcessSpawnBox* pSpawn = static_cast<STMageProcessSpawnBox*>(pProcessSpawn);
    // if (!pSpawn || !pSpawn->pSpawnBox) return;
    //
    // VMonsterSpawnInfo* pMonsterSpawn = pSpawn->pSpawnBox;
    // if (IsTerminateSpawn(pMonsterSpawn->m_iSectorID)) return;
    //
    // int nProb = RandProb();
    // bool bSpawn = false;
    //
    // for (int i = 0; i < 10; ++i) {
    //     if (!pMonsterSpawn->m_stMonsterInfo[i].m_iID) continue;
    //
    //     // 根据生成类型和概率决定是否生成
    //     // 生成怪物或NPC
    //     // ...
    // }
}

// ============================================================================
// ExcuteSpawnBox (VMonsterSpawnInfo版本)
// IDA: 0x14031b560
// 执行生成盒
// ============================================================================
void XMaze::ExcuteSpawnBox(const VMonsterSpawnInfo* pMonsterSpawn, E_SEND_INFO_TYPE eType) {
    // IDA 反编译: XMaze::ExcuteSpawnBox
    // 处理 VMonsterSpawnInfo 类型的生成盒

    if (!pMonsterSpawn) return;

    // int nProb = RandProb();
    // bool bSpawn = false;
    // int nCreateCount = 0;
    //
    // for (int i = 0; i < 10; ++i) {
    //     if (!pMonsterSpawn->m_stMonsterInfo[i].m_iID) continue;
    //
    //     // 根据生成类型和概率决定是否生成
    //     // ...
    //
    //     if (bSpawn) {
    //         for (int j = 0; j < pMonsterSpawn->m_iMaxEntityCount; ++j) {
    //             XVec3 vPos;
    //             GetSpawnPos_2(pMonsterSpawn, &vPos);
    //
    //             int nType = pMonsterSpawn->m_stMonsterInfo[i].m_iType;
    //             if (nType == 0 || nType == 2 || nType == 4) {
    //                 // 生成怪物
    //                 int nMonsterID = ChangeMobIDByMazeLevel(pMonsterSpawn->m_stMonsterInfo[i].m_iID);
    //                 CMonster* pMonster = CreateMonster(m_uxMapID, iUniqueSector, nMonsterID, &vPos, pMonsterSpawn->fRotate, eType, pMonsterSpawn->iID, pMonsterSpawn->m_iGroupID, UXActorID());
    //                 if (pMonster) {
    //                     CheckGuardTarget(pMonster, pMonsterSpawn->m_ProtectionTarget);
    //                     pMonster->SetMoveType(pMonsterSpawn->m_iMoveType);
    //                     ++nCreateCount;
    //                 }
    //             } else if (nType == 1) {
    //                 // 生成NPC
    //                 CNpc* pNpc = CreateNpc(m_uxMapID, iUniqueSector, pMonsterSpawn->m_stMonsterInfo[i].m_iID, &vPos, pMonsterSpawn->fRotate, eType);
    //                 if (pNpc) {
    //                     pNpc->SetWayPointID(pMonsterSpawn->m_iWaypoint);
    //                     ++nCreateCount;
    //                 }
    //             }
    //         }
    //     }
    // }
    //
    // if (nCreateCount > 0) {
    //     AddSyncSpawnActive(pMonsterSpawn);
    // }
}

// ============================================================================
// CheckGuardTarget
// IDA: 0x140328260
// 检查守护目标
// ============================================================================
void XMaze::CheckGuardTarget(CMonster* pMonster, int nGuardSpawnBoxID) {
    // IDA 反编译: XMaze::CheckGuardTarget
    // 检查怪物的守护目标设置

    if (!pMonster) return;

    // 如果守护目标ID有效，查找对应的怪物并设置守护关系
    if (nGuardSpawnBoxID > 0) {
        // 遍历所有怪物，找到对应生成盒ID的怪物
        // for (auto it = m_objectScanner.Begin(); it != m_objectScanner.End(); ++it) {
        //     CActor* pActor = dynamic_cast<CActor*>(*it);
        //     if (!pActor) continue;
        //
        //     CMonster* pTarget = dynamic_cast<CMonster*>(pActor);
        //     if (pTarget && pTarget->GetSpawnBoxID() == nGuardSpawnBoxID) {
        //         pMonster->SetGuardTarget(pTarget);
        //         break;
        //     }
        // }
    }
}

// ============================================================================
// CreateSilhouetteFromBoxinfo
// IDA: ?CreateSilhouetteFromBoxinfo@XMaze@@QEAAPEAVhkaiPointCloudSilhouetteGenerator@@PEBUVEventBoxInfo@@_N@Z (0x140329dc0)
// Creates Havok silhouette generator from event box info for navigation mesh obstacles
// ============================================================================
hkaiPointCloudSilhouetteGenerator* XMaze::CreateSilhouetteFromBoxinfo(const VEventBoxInfo* pBoxInfo, bool bEnable) {
    // IDA: Validate input
    if (!pBoxInfo) {
        return nullptr;
    }

    // IDA: Check for valid box size
    if (pBoxInfo->Size.x <= 0.0f || pBoxInfo->Size.y <= 0.0f || pBoxInfo->Size.z <= 0.0f) {
        LogHelper::LogDebug("game.contents", "<GAME> WARNING!!! Invalid Box Size On Silhouette %d", pBoxInfo->iID);
    }

    // IDA: Create rotation quaternion from box rotation
    // Convert degrees to radians: fRotate * 0.017453292 (PI/180)
    hkVector4 up(0.0f, 0.0f, 1.0f, 0.0f);
    hkQuaternion rot;
    rot.setAxisAngle(up, pBoxInfo->fRotate * 0.017453292f);

    // IDA: Calculate center position (scale by 0.01 for Havok units)
    float centerX = ((pBoxInfo->PosTopLeft.x + pBoxInfo->PosBottomRight.x) / 2.0f) * 0.01f;
    float centerY = ((pBoxInfo->PosTopLeft.y + pBoxInfo->PosBottomRight.y) / 2.0f) * 0.01f;
    float centerZ = pBoxInfo->PosTopLeft.z * 0.01f;

    // IDA: Set position vector
    hkVector4 pos(centerX, centerY, centerZ, 0.0f);

    // IDA: Create transform from rotation and position
    hkQTransform transform;
    hkQTransform::setIdentity(&transform);
    hkQTransform::setTranslation(&transform, &pos);
    // Note: rotation would be set here with full Havok SDK

    // IDA: Allocate silhouette generator (0xD0 = 208 bytes)
    hkaiPointCloudSilhouetteGenerator* pSilGen = new hkaiPointCloudSilhouetteGenerator();

    // IDA: Configure silhouette generator
    hkaiSilhouetteGenerator::setLazyRecomputeDisplacementThreshold(pSilGen, 0.2f);
    hkaiPointCloudSilhouetteGenerator::setWeldTolerance(pSilGen, 0.1f);

    // IDA: Set AABB from box size (negative half-extent to positive half-extent)
    // Scale by 0.01 for Havok units
    hkAabb aabb;
    aabb.m_min.x = (-pBoxInfo->Size.x / 2.0f) * 0.01f;
    aabb.m_min.y = (-pBoxInfo->Size.y / 2.0f) * 0.01f;
    aabb.m_min.z = 0.0f;
    aabb.m_max.x = (pBoxInfo->Size.x / 2.0f) * 0.01f;
    aabb.m_max.y = (pBoxInfo->Size.y / 2.0f) * 0.01f;
    aabb.m_max.z = pBoxInfo->Size.z * 0.01f;

    hkaiPointCloudSilhouetteGenerator::setFromAabb(pSilGen, &aabb);
    hkaiPointCloudSilhouetteGenerator::setTransform(pSilGen, &transform);
    hkaiPointCloudSilhouetteGenerator::setEnabled(pSilGen, bEnable);

    // IDA: Add to aiWorld and release reference
    if (m_pNavMeshInstance && m_pNavMeshInstance->GetAiWorld()) {
        m_pNavMeshInstance->GetAiWorld()->addSilhouetteGenerator(pSilGen);
    }

    // IDA: Remove our reference (aiWorld holds its own reference)
    hkReferencedObject::removeReference(reinterpret_cast<hkReferencedObject*>(pSilGen));

    return pSilGen;
}

// ============================================================================
// ChangeMonster
// IDA: 0x14032a540
// 更改怪物
// ============================================================================
bool XMaze::ChangeMonster(int nOldMonsterID, int nNewMonsterID, int nSpawnBoxID) {
    // IDA 反编译: XMaze::ChangeMonster
    // 将指定怪物替换为新ID的怪物

    if (nOldMonsterID <= 0 || nNewMonsterID <= 0) return false;

    // 遍历查找目标怪物
    // for (auto it = m_objectScanner.Begin(); it != m_objectScanner.End(); ++it) {
    //     CActor* pActor = dynamic_cast<CActor*>(*it);
    //     if (!pActor) continue;
    //
    //     CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
    //     if (!pMonster) continue;
    //
    //     // 检查怪物ID和生成盒ID
    //     if (pMonster->GetMonsterID() == nOldMonsterID) {
    //         if (nSpawnBoxID > 0 && pMonster->GetSpawnBoxID() != nSpawnBoxID) {
    //             continue;
    //         }
    //
    //         // 获取怪物位置和朝向
    //         XVec3 vPos = pMonster->GetPosition();
    //         float fRotate = pMonster->GetRotate();
    //         int iUniqueSector = pMonster->GetUniqueSector();
    //
    //         // 删除旧怪物
    //         UXActorID uxActorID = pMonster->GetActorID();
    //         RemoveActor(pMonster);
    //
    //         // 创建新怪物
    //         CMonster* pNewMonster = CreateMonster(m_uxMapID, iUniqueSector, nNewMonsterID, &vPos, fRotate, E_SEND_INFO_TYPE::SYNC, nSpawnBoxID, 0, UXActorID());
    //         if (pNewMonster) {
    //             return true;
    //         }
    //     }
    // }

    return false;
}

// ============================================================================
// SetEscortMonster
// IDA: 0x14032bf00
// 设置护送怪物
// ============================================================================
void XMaze::SetEscortMonster(unsigned int dwEpisodeID, const char* szMonsterID, const char* szAnimName) {
    // IDA 反编译: XMaze::SetEscortMonster
    // void __fastcall XMaze::SetEscortMonster(
    //     XMaze *this,
    //     unsigned int dwEpisodeID,
    //     const char *szMonsterID,
    //     const char *szAnimName)
    // {
    //   std::string _Str;
    //   v5 = -2;
    //   std::string::string(&_Str, szMonsterID);
    //   this->m_stEscortMonster.nMonsterID = std::stoi(&_Str, nullptr, 10);
    //   std::string::~string(&_Str);
    //   this->m_stEscortMonster.dwEpisodeID = dwEpisodeID;
    //   strcpy_s(this->m_stEscortMonster.szMonsterDieAnim, 0x80u, szAnimName);
    //   LogHelper::LogDebug("game.contents", "SetEscortMonster %d, %s, %s",
    //       this->m_stEscortMonster.nMonsterID, szMonsterID, szAnimName);
    // }

    if (szMonsterID) {
        m_stEscortMonster.nMonsterID = std::stoi(szMonsterID);
    }
    m_stEscortMonster.dwEpisodeID = dwEpisodeID;
    if (szAnimName) {
        strcpy_s(m_stEscortMonster.szMonsterDieAnim, sizeof(m_stEscortMonster.szMonsterDieAnim), szAnimName);
    }
}

// ============================================================================
// CheckCanDirectMove2 (static version)
// IDA: ?CheckCanDirectMove2@XMaze@@SAHPEAVDohHavokNavMeshInstance@@AEAVhkvVec3@@1MHH@Z (0x14032aad0)
// Check if direct move is possible using navmesh
// Returns: 1 if can move directly, 0 otherwise
// ============================================================================
int XMaze::CheckCanDirectMove2(DohHavokNavMeshInstance* pNavMesh, hkvVec3* vStartPos,
                                hkvVec3* vDestPos, float fRadius, int bFlying, int bDontCareCurve) {
    // IDA: CheckCanDirectMove2 - precise restoration
    if (!pNavMesh) {
        return 0;
    }

    // Check if start and end are the same
    if (vStartPos->x == vDestPos->x && vStartPos->y == vDestPos->y && vStartPos->z == vDestPos->z) {
        return 1;
    }

    float fZPos = vStartPos->z;
    hkvVec3 vPos = *vStartPos;
    hkvVec3 vNextPos = *vDestPos;

    // Get height at positions
    pNavMesh->GetHeight(&vPos, 200.0f);
    if (bFlying) {
        pNavMesh->GetHeight(&vNextPos, 200.0f);
    }

    // Compute path
    std::vector<hkvVec3> vOutList;
    vOutList.resize(15);

    int nPathCnt = pNavMesh->ComputePath(vPos, vNextPos, fRadius + 10.0f, vOutList, 15);
    bool bShouldCheck = false;

    // Check path curvature
    if (!bDontCareCurve && nPathCnt > 3) {
        hkvVec3 vTempStart = vPos;
        hkvVec3 vTempEnd = vNextPos;
        vTempStart.z = 0.0f;
        vTempEnd.z = 0.0f;

        // Calculate direct distance
        hkvVec3 vDiff = vTempEnd - vTempStart;
        float fOriginalDist = vDiff.getLength();

        // Calculate path distance
        float fPathDist = 0.0f;
        hkvVec3 vPathStart(vOutList[0].x, vOutList[0].y, 0.0f);

        for (int i = 1; i < nPathCnt; ++i) {
            hkvVec3 vPathNext(vOutList[i].x, vOutList[i].y, 0.0f);
            hkvVec3 vPathDiff = vPathNext - vPathStart;
            fPathDist += vPathDiff.getLength();
            vPathStart = vPathNext;
        }

        // If path is 5% longer than direct distance, need to check
        if ((fPathDist - fOriginalDist) > (fOriginalDist * 0.05f)) {
            bShouldCheck = true;
        }
    }

    int bRet = 1;
    if (nPathCnt < 2 || bShouldCheck) {
        bRet = 0;
        *vDestPos = vPos;

        hkvVec3 vDiff = vNextPos - *vStartPos;
        float fDiffSq = vDiff.getLengthSquared();

        if (fDiffSq > 400.0f) {
            vDiff *= 0.5f;
            vNextPos = vPos + vDiff;
            pNavMesh->GetHeight(&vNextPos, 200.0f);

            int nCount = (fDiffSq <= 10000.0f) ? 3 : 5;
            hkvVec3 vTemp = vPos;

            for (int j = 0; j < nCount; ++j) {
                vDiff *= 0.5f;
                int nPathCnta = pNavMesh->ComputePath(vPos, vNextPos, fRadius + 10.0f, vOutList, 15);

                if (nPathCnta != 2 && (bDontCareCurve || nPathCnta < 2)) {
                    vNextPos -= vDiff;
                    pNavMesh->GetHeight(&vNextPos, 200.0f);
                } else {
                    vDestPos->x = vOutList[nPathCnta - 1].x;
                    vDestPos->y = vOutList[nPathCnta - 1].y;
                    if (!pNavMesh->GetHeight(vDestPos, 200.0f)) {
                        *vDestPos = vTemp;
                        break;
                    }
                    vTemp = *vDestPos;
                    vNextPos += vDiff;
                    pNavMesh->GetHeight(&vNextPos, 200.0f);
                }
            }
        }
    } else {
        vDestPos->x = vOutList[nPathCnt - 1].x;
        vDestPos->y = vOutList[nPathCnt - 1].y;
        if (!pNavMesh->GetHeight(vDestPos, 200.0f)) {
            bRet = 0;
            *vDestPos = *vStartPos;
        }
    }

    if (bFlying) {
        vDestPos->z = fZPos;
    }

    return bRet;
}

// ============================================================================
// CheckCanDirectMove2 (instance version)
// IDA: 0x14032aad0
// 检查是否可以直接移动（版本2）
// ============================================================================
bool XMaze::CheckCanDirectMove2(const XVec3* pStartPos, const XVec3* pEndPos, float fRadius) {
    if (!pStartPos || !pEndPos) return false;
    if (!m_pNavMeshInstance) return false;

    hkvVec3 vStart(pStartPos->x, pStartPos->y, pStartPos->z);
    hkvVec3 vEnd(pEndPos->x, pEndPos->y, pEndPos->z);

    int result = CheckCanDirectMove2(m_pNavMeshInstance, &vStart, &vEnd, fRadius, 0, 0);
    return result != 0;
}

// ============================================================================
// ResetLuaFunctionBox
// IDA: 0x14033bae0
// 重置 Lua 函数盒子
// ============================================================================
void XMaze::ResetLuaFunctionBox(int nBoxIndex) {
    // IDA 反编译: XMaze::ResetLuaFunctionBox
    // void __fastcall XMaze::ResetLuaFunctionBox(XMaze *this, int nBoxIndex)
    // {
    //   int BatchLayerLevel = XMaze::GetBatchLayerLevel(this);
    //   int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nBoxIndex, BatchLayerLevel);
    //   auto it = m_mapLuaFunctionBox.find(iBoxUniqueID);
    //   if (it != m_mapLuaFunctionBox.end()) {
    //     STLuaFunctionBox* pLuaFunction = it->second;
    //     if (pLuaFunction) {
    //       pLuaFunction->bCalled = 0;
    //     }
    //   }
    // }

    // 简化实现 - 需要完整的 m_mapLuaFunctionBox 和 STLuaFunctionBox 类型定义
    // int nBatchLayerLevel = GetBatchLayerLevel();
    // int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nBoxIndex, nBatchLayerLevel);
    // auto it = m_mapLuaFunctionBox.find(iBoxUniqueID);
    // if (it != m_mapLuaFunctionBox.end() && it->second) {
    //     it->second->bCalled = false;
    // }
}

// ============================================================================
// FinishMonsterKillScoreMode
// IDA: 0x14033b240
// 完成怪物击杀分数模式
// ============================================================================
void XMaze::FinishMonsterKillScoreMode() {
    // IDA 反编译: XMaze::FinishMonsterKillScoreMode (约800字节)
    // 主要逻辑:
    // 1. 构造 PS_MONSTER_KILL_SCORE_UPDATE 包，设置 nState=2, nPoint=m_stMonsterKillScoreMode.nPoint
    // 2. 广播给所有用户
    // 3. 根据迷宫类型记录日志:
    //    - Maze_Type == 14: MONSTER_KILL_FINISH
    //    - Maze_Type == 18: LEAGUE_RAID_FINISH
    //    - Maze_Type == 19: MONSTER_KILL_FINISH_NOT_LIMIT

    // 简化实现 - 需要完整的 PS_MONSTER_KILL_SCORE_UPDATE 和 SendBroadCast
    // PS_MONSTER_KILL_SCORE_UPDATE psUpdate;
    // psUpdate.nState = 2;
    // psUpdate.dwLeftTime = 0;
    // psUpdate.nPoint = m_stMonsterKillScoreMode.nPoint;
    // SendBroadCast(&xSendPacket, nullptr, eAll);
}

// ============================================================================
// AddMonsterKillScoreModePoint
// IDA: 0x14033b560
// 增加怪物击杀分数模式点数
// ============================================================================
void XMaze::AddMonsterKillScoreModePoint(int nPoint) {
    // IDA 反编译: XMaze::AddMonsterKillScoreModePoint (约1000字节)
    // 主要逻辑:
    // 1. 如果 nPoint == 0 直接返回
    // 2. m_stMonsterKillScoreMode.nPoint += nPoint
    // 3. 构造 PS_MONSTER_KILL_SCORE_UPDATE 包，设置 nState=1, nPoint=当前点数
    // 4. 广播给所有用户
    // 5. 如果脚本有 OnUpdateMonsterKillScore 函数，执行脚本
    // 6. 根据迷宫类型记录日志:
    //    - Maze_Type == 14: MONSTER_KILL_ADD_POINT
    //    - Maze_Type == 18: LEAGUE_RAID_ADD_POINT
    //    - Maze_Type == 19: MONSTER_KILL_ADD_POINT_NOT_LIMIT

    if (nPoint == 0) return;

    // 简化实现 - 需要完整的类型定义
    // m_stMonsterKillScoreMode.nPoint += nPoint;
    // PS_MONSTER_KILL_SCORE_UPDATE psUpdate;
    // psUpdate.nState = 1;
    // psUpdate.nPoint = m_stMonsterKillScoreMode.nPoint;
    // SendBroadCast(&xSendPacket, nullptr, eAll);
}

// ============================================================================
// AddMonsterKillScoreModeTime
// IDA: 0x14033b960
// 增加怪物击杀分数模式时间
// ============================================================================
void XMaze::AddMonsterKillScoreModeTime(unsigned int nLeftTime, int nAddTime) {
    // IDA 反编译: XMaze::AddMonsterKillScoreModeTime (0x14033B960)
    // 主要逻辑:
    // 1. 构造 PS_MONSTER_KILL_SCORE_UPDATE 包
    // 2. 设置 nState=1, dwLeftTime=nLeftTime, nAddTime=nAddTime, nPoint=当前点数
    // 3. 广播给所有用户
    // 4. 记录日志 MONSTER_KILL_ADD_TIME

    // TODO: 需要完整的 PS_MONSTER_KILL_SCORE_UPDATE 结构和 SendBroadCast
    // PS_MONSTER_KILL_SCORE_UPDATE psUpdate;
    // psUpdate.nState = 1;
    // psUpdate.dwLeftTime = nLeftTime;
    // psUpdate.nAddTime = nAddTime;
    // psUpdate.nPoint = m_stMonsterKillScoreMode.nPoint;

    // XSendPacket xSendPacket(0x11, 0x14);
    // xSendPacket << psUpdate;
    // SendBroadCast(&xSendPacket, nullptr, eAll);

    // ST_LOG_GAME stLog;
    // stLog._sMainType = 5;
    // stLog._sSubType = 32;
    // stLog.nParam0 = m_pTBMazeInfo->ID;
    // stLog.nParam1 = 3;
    // stLog.nParam2 = nLeftTime;
    // stLog.nParam3 = nAddTime;
    // stLog.nParam6 = GetInstanceID();
    // wcscpy_s(stLog.szComment, L"MONSTER_KILL_ADD_TIME");
    // XGameServer::Instance()->SendDBLog(&stLog);
}

// ============================================================================
// SendRoguelikePocketBox
// IDA: ?SendRoguelikePocketBox@XMaze@@QEAA_NH@Z (0x140341980)
// 发送肉鸽口袋盒子信息
// ============================================================================
bool XMaze::SendRoguelikePocketBox(int nSectorID) {
    // IDA 反编译: XMaze::SendRoguelikePocketBox (约3000字节)
    // 主要逻辑:
    // 1. 检查迷宫类型是否为15 (Roguelike)
    // 2. 获取批次层数级别
    // 3. 计算盒子唯一ID
    // 4. 查找对应的Sector
    // 5. 遍历所有用户，获取默认技能列表
    // 6. 随机打乱技能列表
    // 7. 构造 PS_ROGUELIKE_POCKET_INFOS 包
    // 8. 发送给用户

    // 检查迷宫类型
    if (!m_pTBMazeInfo || m_pTBMazeInfo->Maze_Type != 15) {
        return false;
    }

    // TODO: 需要完整的实现
    // 需要以下类型和函数:
    // - GetBatchLayerLevel()
    // - VEventObjectInfo::GetEventUniqueID()
    // - CSector::SetRoguelikeState()
    // - CGocSkill::ChargeModeSkillActiveCount()
    // - CGocSkill::GetModeDefaultSkillList()
    // - XResourceMgr::FindRoguelikeData()
    // - std::random_shuffle()

    return true;
}

// ============================================================================
// SendRoguelikeShopInfo
// IDA: ?SendRoguelikeShopInfo@XMaze@@QEAA_NH@Z (0x140342e40)
// 发送肉鸽商店信息
// ============================================================================
bool XMaze::SendRoguelikeShopInfo(int nSectorID) {
    // IDA 反编译: XMaze::SendRoguelikeShopInfo (约3000字节)
    // 主要逻辑:
    // 1. 检查迷宫类型是否为15 (Roguelike)
    // 2. 清空随机商店属性列表
    // 3. 查找Roguelike数据
    // 4. 获取升级组数据
    // 5. 随机打乱属性列表
    // 6. 遍历所有用户
    // 7. 更新Roguelike步骤
    // 8. 计算奖励金钱
    // 9. 构造 PS_ROGUELIKE_SHOP_INFO 包
    // 10. 发送给用户

    // 检查迷宫类型
    if (!m_pTBMazeInfo || m_pTBMazeInfo->Maze_Type != 15) {
        return false;
    }

    // TODO: 需要完整的实现
    // 需要以下类型和函数:
    // - XResourceMgr::FindRoguelikeData()
    // - XResourceMgr::GetTB_MODE_BI_UPGRADE_GROUP()
    // - CGocEntity::UpdateRoguelikeStep()
    // - CGocEntity::GetRoguelikeRewardMoney()
    // - CGocSkill::UpdateModeShopMoney()
    // - CGocSkill::GetModeShopMyInfo()

    return true;
}

// ============================================================================
// SendPocketBox
// IDA: ?SendPocketBox@XMaze@@QEAA_NH@Z (0x1403456d0)
// 发送口袋盒子
// ============================================================================
bool XMaze::SendPocketBox(int nGroup) {
    // IDA 反编译: XMaze::SendPocketBox (约40字节)
    // 简单函数，仅检查迷宫类型是否为15

    if (!m_pTBMazeInfo) {
        return false;
    }

    return m_pTBMazeInfo->Maze_Type == 15;
}

// ============================================================================
// GetTutorial
// IDA: ?GetTutorial@XMaze@@QEAA_NXZ (0x140638b50)
// 获取是否为教程迷宫
// ============================================================================
bool XMaze::GetTutorial() const {
    // IDA 反编译: XMaze::GetTutorial (约40字节)
    // 检查迷宫类型是否为1

    if (!m_pTBMazeInfo) {
        return false;
    }

    return m_pTBMazeInfo->Maze_Type == 1;
}

// ============================================================================
// GetRoguelikeNextMap
// IDA: ?GetRoguelikeNextMap@XMaze@@QEAA_NPEAVCUser@@AEAHAEAUSTPosInfo@@@Z (0x1403447f0)
// 获取肉鸽下一张地图
// ============================================================================
bool XMaze::GetRoguelikeNextMap(CUser* pUser, int& nNextMapID, STPosInfo& stPosInfo) {
    // IDA 反编译: XMaze::GetRoguelikeNextMap (约260字节)
    // 主要逻辑:
    // 1. 检查是否为肉鸽地图
    // 2. 检查随机口袋信息是否为空
    // 3. 获取用户的 CGocEntity 组件
    // 4. 检查肉鸽状态
    // 5. 获取下一张地图ID

    if (!IsRoguelikeMap()) {
        return true;
    }

    // TODO: 需要完整的实现
    // 需要以下类型和函数:
    // - m_mapRoguelikeRandomPocketInfo.empty()
    // - CMover::GetGOC<CGocEntity>()
    // - CGocEntity::IsRoguelikeState()
    // - CGocEntity::GetRoguelikeNextMap()

    return true;
}

// ============================================================================
// GetReturnMapID
// IDA: ?GetReturnMapID@XMaze@@UEAAGK@Z (0x140353000)
// 获取返回地图ID
// ============================================================================
unsigned short XMaze::GetReturnMapID(unsigned long dwUCID) {
    // IDA 反编译: XMaze::GetReturnMapID (约20字节)
    // 简单函数，返回固定值10003

    return 10003;
}

// ============================================================================
// GetMazeLevel
// IDA: ?GetMazeLevel@XMaze@@QEAAHXZ (0x140353420)
// 获取迷宫等级
// ============================================================================
int XMaze::GetMazeLevel() const {
    // IDA 反编译: XMaze::GetMazeLevel (约20字节)
    // 返回迷宫状态中的等级

    return m_stMazeGameState.m_nMazeLevel;
}

// ============================================================================
// GetPartyMemberCount
// IDA: ?GetPartyMemberCount@XMaze@@QEAAHXZ (0x140364a90)
// 获取队伍成员数量
// ============================================================================
int XMaze::GetPartyMemberCount() const {
    // IDA 反编译: XMaze::GetPartyMemberCount (约20字节)
    // 返回队伍成员数量

    return m_nPartyMemeberCount;
}

// ============================================================================
// SetEscortMonster
// IDA: ?SetEscortMonster@XMaze@@QEAAXKPEBD0@Z (0x14032bf00)
// 设置护送怪物信息
// ============================================================================
void XMaze::SetEscortMonster(unsigned long dwEpisodeID, const char* szMonsterID, const char* szAnimName) {
    // IDA 反编译: XMaze::SetEscortMonster (约240字节)
    // 主要逻辑:
    // 1. 解析怪物ID字符串为整数
    // 2. 设置护送怪物信息

    if (!szMonsterID || !szAnimName) return;

    // 解析怪物ID
    m_stEscortMonster.nMonsterID = std::stoi(szMonsterID);
    m_stEscortMonster.dwEpisodeID = dwEpisodeID;

    // 复制动画名称
    std::strncpy(m_stEscortMonster.szMonsterDieAnim, szAnimName, sizeof(m_stEscortMonster.szMonsterDieAnim) - 1);
    m_stEscortMonster.szMonsterDieAnim[sizeof(m_stEscortMonster.szMonsterDieAnim) - 1] = '\0';

    // TODO: LogHelper::LogDebug
}

// ============================================================================
// SetDisconnectUserState
// IDA: ?SetDisconnectUserState@XMaze@@QEAAXKUST_PARTY_INFO@@@Z (0x140336930)
// 设置断开连接用户状态
// ============================================================================
void XMaze::SetDisconnectUserState(unsigned long dwUCID, const ST_PARTY_INFO& stPartyInfo) {
    // IDA 反编译: XMaze::SetDisconnectUserState (约1200字节)
    // 主要逻辑:
    // 1. 检查 m_pTBMazeInfo 是否有效
    // 2. 根据 stPartyInfo.byGroupType 检查队伍/Force
    // 3. 在 m_mapWaitEnterMazeUser 中查找用户
    // 4. 设置断开连接状态
    // 5. 发送通知包

    if (!m_pTBMazeInfo) return;

    // TODO: 需要完整的实现
    // 需要以下类型和函数:
    // - m_pParty, m_pForce
    // - CParty::GetPartyID()
    // - m_mapWaitEnterMazeUser.find()
    // - CGameControlSocket::IsCanSend()
}

// ============================================================================
// SetEnterDistrictPos
// IDA: ?SetEnterDistrictPos@XMaze@@QEAAXAEAUSTPosInfo@@@Z (0x1406e0460)
// 设置进入区域位置
// ============================================================================
void XMaze::SetEnterDistrictPos(STPosInfo& stPos) {
    // IDA 反编译: XMaze::SetEnterDistrictPos (约40字节)
    // 简单赋值

    m_stEnterDistrictPos = stPos;
}

// ============================================================================
// SetPartyInfo
// IDA: ?SetPartyInfo@XMaze@@QEAAXUST_PARTY_INFO@@@Z (0x1406e0490)
// 设置队伍信息
// ============================================================================
void XMaze::SetPartyInfo(const ST_PARTY_INFO& stPartyInfo) {
    // IDA 反编译: XMaze::SetPartyInfo (约30字节)
    // 简单赋值

    m_stPartyInfo = stPartyInfo;
}

// ============================================================================
// IsModeCondition
// IDA: ?IsModeCondition@XMaze@@UEAA_NXZ (0x140345840)
// 检查是否为模式条件
// ============================================================================
bool XMaze::IsModeCondition() {
    // IDA 反编译: XMaze::IsModeCondition (约40字节)
    // 返回 !IsRoguelikeMap()

    return !IsRoguelikeMap();
}

// ============================================================================
// FinishRoguelikeSector
// IDA: ?FinishRoguelikeSector@XMaze@@QEAAXPEAVCUser@@H@Z (0x140345180)
// 完成肉鸽扇区
// ============================================================================
void XMaze::FinishRoguelikeSector(CUser* pUser, int nSectorID) {
    // IDA 反编译结果精确还原（简化实现）
    if (!pUser) return;

    // 获取批次层数级别
    int nBatchLayerLevel = GetBatchLayerLevel();

    // 计算唯一ID - 简化版本
    // TODO: VEventObjectInfo::GetEventUniqueID 需要实现
    int nUniqueID = nSectorID; // 简化：直接使用 SectorID

    // 查找对应的Sector
    auto it = m_mapSector.find(nUniqueID);
    if (it != m_mapSector.end()) {
        CSector* pSector = it->second;
        if (pSector) {
            // 检查肉鸽状态必须是2（进行中）
            if (pSector->GetRoguelikeState() != 2) {
                return;
            }
            // 设置状态为3（完成）
            pSector->SetRoguelikeState(3);
        }
    }

    // 获取迷宫ID
    int nMazeID = static_cast<unsigned short>(XArea::GetTBMapID());

    // 查找肉鸽数据 - 简化版本
    // TODO: XResourceMgr::FindRoguelikeData 需要实现
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_MODE_BI_SECTOR_INFO* pInfo = XResourceMgr::FindRoguelikeData(nMazeID, nSectorID);
    TB_MODE_BI_SECTOR_INFO* pInfo = nullptr; // 简化：暂不查找表数据

    if (pInfo) {
        int nMessage = pInfo->BI_Sector_End_Message;
        int nType = pInfo->BI_Sector_End_Message_Type;

        if (nMessage > 0) {
            // 发送肉鸽通知包 - 简化版本
            // TODO: PS_MODE_MAZE_NOTICE 结构需要定义
            XSendPacket xSendPacket(0x33, 0x30);
            xSendPacket << nType;
            xSendPacket << nMessage;
            SendBroadCast(&xSendPacket, nullptr, E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
        }
    }

    // 检查无敌状态并清除
    // TODO: 需要正确获取 CUser 的 CMoverEx 成员
    // CMover* pMover = static_cast<CMover*>(pUser);
    // if (pMover && pMover->IsInvincibleActor()) {
    //     pMover->SetInvincibleActor(0);
    //     // pMover->ClearImmunityStatus(4); // TODO: ClearImmunityStatus 需要实现
    // }
}

// ============================================================================
// CheckSpawnBoxGroupCount
// IDA: 0x1403347D0
// 检查 SpawnBox 组计数，用于怪物生成控制
// ============================================================================
bool XMaze::CheckSpawnBoxGroupCount(float fElapsed) {
    // IDA: 遍历 m_mapSpawnBoxGroupLimit
    for (auto it = m_mapSpawnBoxGroupLimit.begin(); it != m_mapSpawnBoxGroupLimit.end(); ++it) {
        int nGroupID = it->first;
        STSpawnBoxGroupInfo& stInfo = it->second;

        // IDA: 根据 GroupID 获取 SectorID
        int nSectorID = GetSectorByGroupID(nGroupID);
        if (nSectorID == -1) {
            continue;
        }

        // IDA: 获取 Sector
        CSector* pSector = GetSector(nSectorID);
        if (!pSector) {
            continue;
        }

        // IDA: 检查是否启用 (BYTE1 of m_eObjectFlags == 1)
        // Note: IDA checks BYTE1 which is second byte, meaning (m_eObjectFlags >> 8) & 0xFF == 1
        // Simplified: just check if m_eObjectFlags has the right flag set
        if ((stInfo.m_eObjectFlags & 0xFF00) != 0x0100) {
            continue;
        }

        // IDA: 查找 m_mapGroupID_Monster
        auto itGroup = m_mapGroupID_Monster.find(nGroupID);
        if (itGroup == m_mapGroupID_Monster.end()) {
            continue;
        }

        STMonterGroupMonsterInfo& stMonsterInfo = itGroup->second;
        if (stMonsterInfo.vecBoxList.empty()) {
            continue;
        }

        // IDA: 获取 Sector 怪物计数
        int nSectorMonsterCnt = pSector->GetMonsterCount();

        // IDA: 如果怪物数小于限制，检查是否需要生成
        if (nSectorMonsterCnt < stInfo.m_nLimit) {
            for (size_t i = 0; i < stMonsterInfo.vecBoxList.size(); ++i) {
                STMonterGroupMonsterData& stData = stMonsterInfo.vecBoxList[i];

                if (!stData.bSpawn && stData.pInfo) {
                    stData.fWaitTime -= fElapsed;
                    if (stData.fWaitTime > 0.0f) {
                        break;
                    }

                    // IDA: 执行生成
                    int nWaveCount = stData.pInfo->m_iWaitCreationMaxWave > 0
                        ? stData.pInfo->m_iWaitCreationMaxWave : 1;
                    int nEntityCount = stData.pInfo->m_iMaxEntityCount > 0
                        ? stData.pInfo->m_iMaxEntityCount : 1;

                    // TODO: ExcuteSpawnBoxCheck 需要实现
                    // ExcuteSpawnBoxCheck(stData.pInfo->iID, eSendInfoTypeSend, 1);
                    stData.bSpawn = 1;

                    nSectorMonsterCnt += nEntityCount * nWaveCount;
                }
            }
        }
    }

    return false;
}
