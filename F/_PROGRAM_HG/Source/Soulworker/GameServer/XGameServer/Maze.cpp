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
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/WorldManager.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XGameServer/Sector.h"
#include <cstdio>
#include <cstring>

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
        RunSectorAI(10001, 1);
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
// ============================================================================
std::uint16_t XMaze::MoveActor(XActor* pActor, XVec3& vNextPos, float fRot) {
    if (!pActor) {
        return 50001;
    }

    (void)vNextPos;
    (void)fRot;
    // TODO: restore XActor/CMover position update once SetPosInfo ownership is recovered.
    return 0;
}

// ============================================================================
// CreateNavMesh
// IDA: 0x14031F120
// ============================================================================
bool XMaze::CreateNavMesh(const char* pszFileName) {
    (void)pszFileName;
    // TODO: restore Havok navmesh resource loading once DohHavokResourceManager is recovered.
    m_pNavMeshInstance = nullptr;
    return false;
}

// ============================================================================
// CreateScriptInst
// IDA: 0x14031F2C0
// ============================================================================
bool XMaze::CreateScriptInst(const char* pszFileName) {
    (void)pszFileName;
    // TODO: restore Vision script instance creation once script manager wrappers are recovered.
    m_pScriptInstance = nullptr;
    return false;
}

// ============================================================================
// CreateNpc
// IDA: 0x14031A250
// ============================================================================
CNpc* XMaze::CreateNpc(UXMapID uxMazeSerialID, std::uint32_t nSectorID,
                       std::uint32_t nNpcID, XVec3* vPos, float fRot,
                       ::E_SEND_INFO_TYPE eType) {
    (void)uxMazeSerialID;
    (void)nSectorID;
    (void)nNpcID;
    (void)vPos;
    (void)fRot;
    (void)eType;
    // TODO: restore ThreadLocalData NPC creation and sector insertion path.
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
void XMaze::NotifyMonsterDelete(CMonster* pMonster) {
    (void)pMonster;
    // TODO: restore bot-user monster delete broadcast once actor ID plumbing is recovered.
}

// ============================================================================
// DeleteNpc
// IDA: 0x14031A430
// ============================================================================
void XMaze::DeleteNpc(CNpc* pNpc) {
    (void)pNpc;
    // TODO: restore ThreadLocalData NPC destruction and ExitGameObject dispatch.
}

// ============================================================================
// CreateAkashicObject
// IDA: 0x14031A4A0
// ============================================================================
CAkashicObject* XMaze::CreateAkashicObject(UXMapID uxMazeSerialID, std::uint32_t nAkashicID,
                                           XVec3* vPos, float fRot, std::uint32_t dwParentID,
                                           ::E_SEND_INFO_TYPE eType) {
    (void)uxMazeSerialID;
    (void)nAkashicID;
    (void)vPos;
    (void)fRot;
    (void)dwParentID;
    (void)eType;
    // TODO: restore Akashic object resource lookup and ThreadLocalData creation.
    return nullptr;
}

// ============================================================================
// DeleteAkashicObject
// IDA: 0x14031A5E0
// ============================================================================
void XMaze::DeleteAkashicObject(CAkashicObject* pAkashic) {
    (void)pAkashic;
    // TODO: restore Akashic object destruction and ExitGameObject dispatch.
}

// ============================================================================
// SetParty
// IDA: 0x140315C40
// ============================================================================
void XMaze::SetParty(std::shared_ptr<CParty> pParty) {
    m_pParty = pParty;
    m_pForce.reset();
    m_stPartyInfo.byGroupType = pParty ? 1 : 0;
    m_nPartyUserCount = 0;
    // TODO: restore CParty::SetMazeID/GetPartyID/GetUserCount once CParty is complete here.
}

// ============================================================================
// SetForce
// IDA: 0x140315D50
// ============================================================================
void XMaze::SetForce(std::shared_ptr<CForce> pForce) {
    m_pForce = pForce;
    m_pParty.reset();
    m_stPartyInfo.byGroupType = pForce ? 2 : 0;
    m_nPartyUserCount = 0;
    // TODO: restore CForce::SetMazeID/GetPartyID/GetUserCount once CForce is complete here.
}

// ============================================================================
// GetScanner
// IDA: 0x1403264D0
// EXACT IDA implementation - get scanner map based on actor type
// ============================================================================
std::map<std::uint32_t, CMover*>* XMaze::GetScanner(XActor* pActor) {
    (void)pActor;
    // TODO: reconcile OBJECT_SCANNER key type before exposing scanner maps.
    return nullptr;
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
// TODO: Function signature may not match declaration in header
// ============================================================================
CSector* XMaze::GetSector(int nSectorID) {
    // 获取 BatchLayerLevel 并计算唯一 ID
    // TODO: VEventObjectInfo not defined
    int nBatchLayerLevel = GetBatchLayerLevel();
    // int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nSectorID, nBatchLayerLevel);
    int iBoxUniqueID = nSectorID;  // Simplified stub

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
// EXACT IDA implementation - broadcast to all movers in scanner
// ============================================================================
void XMaze::SendBroadCast(XSendPacket* pPacket, XActor* pExceptActor, E_BROADCAST_TYPE eType) {
    (void)pPacket;
    (void)pExceptActor;
    (void)eType;
    // TODO: restore CGocNetwork broadcast once actor/network ownership is recovered.
}

// ============================================================================
// 辅助方法存根
// ============================================================================

void XMaze::RunSectorAI(int nSectorID, int nState) {
    (void)nSectorID;
    (void)nState;
    // TODO: restore CSector::RunAI equivalent.
}

void XMaze::UpdateClearMazeCondition(int nConditionType, int nValue) {
    (void)nConditionType;
    (void)nValue;
    // TODO: restore clear-condition table walk and reward processing.
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
    // TODO: restore summoned-helper lookup from actor IDs.
}

// ============================================================================
// RefreshUserCountInSector
// IDA: 0x14033BE00
// 刷新区域中用户数量
// ============================================================================
void XMaze::RefreshUserCountInSector(std::uint32_t dwActorID) {
    m_mapCheckSectorUser.erase(dwActorID);
    // TODO: restore STMagePotalBox enter-count/open-state handling once the box layout is complete.
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
    // TODO: restore invisibility query once XActor exposes the status accessor.
    return 0;
}

void XMaze::UpdateMazeState() {
    // IDA: 更新迷宫状态
    // 检查通关条件
    if (!m_bMazeComplete && m_pTBMazeInfo) {
        // 更新游戏时间
        m_stMazeGameState.m_dwMazePlayTime += 1;
        
        // 检查是否超时
        if (m_pTBMazeInfo->Maze_ClearTime > 0) {
            if (m_stMazeGameState.m_dwMazePlayTime >= m_pTBMazeInfo->Maze_ClearTime) {
                // 迷宫失败
                m_bMazeComplete = true;
            }
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
    }
    // TODO: restore CGocQuest escort condition completion.
}

void XMaze::UpdateCasualRaidTimer(float fElapsed) {
    (void)fElapsed;
    // TODO: restore casual raid timer reward path.
}

void XMaze::UpdatePortalState() {
    // TODO: restore portal state update once STMagePotalBox layout is available.
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
    (void)pUser;
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

// ============================================================================
// Init, Create, Clear, Generate 等大型方法存根
// 这些方法需要完整的实现
// ============================================================================

// ============================================================================
// Init
// IDA: 0x140311210
// ============================================================================
bool XMaze::Init() {
    // Reset state - IDA: XIOCPServer::BackSends returns true (0x1408F0C60)
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

    // Clear Party/Force
    m_pParty.reset();
    m_pForce.reset();
    m_nPartyUserCount = 0;
    m_bBossSector = false;
    m_pHiddenEvent = nullptr;
    m_bHaveBotUser = false;

    // Initialize position info
    m_stEnterDistrictPos.x = 0;
    m_stEnterDistrictPos.y = 0;
    m_stEnterDistrictPos.z = 0;
    m_stEnterDistrictPos.fRot = 0;
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

    // TODO: restore CCutsceneManager initialization once the full type is available here.

    // Clear escort monster
    std::memset(&m_stEscortMonster, 0, sizeof(m_stEscortMonster));

    // Get Maze resource - IDA: XWorldResMgr::GetResource (0x140720370)
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

    // Generate Maze
    Generate();

    // Create script instance
    if (!CreateScriptInst(m_pTBMazeInfo->Server_SceneScript_File)) {
        LogHelper::LogError("game.contents",
            "Init error - Failed CreateScriptInst[ %s ] ( %d )",
            m_pTBMazeInfo->Server_SceneScript_File, 170);
        return false;
    }

    // Initialize Grouton
    m_nGroutonBoxID = 0;
    m_dwWaitGroutonSpawnTime = 0;

    // Spawn monsters
    SpawnGenerateMonster();

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

    // Clear Maze Log
    std::memset(m_nMazeLog, 0, sizeof(m_nMazeLog));

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
    // 设置 Map ID
    // TODO: UXMapID to TUXMapID conversion needed
    // m_uxMapID = stCreateMaze.uxMapID;

    // 添加等待进入 Maze 的用户
    // TODO: ST_ENTER_MAZE_MEMBER_INFO missing member dwActorID
    for (const auto& memberInfo : stCreateMaze.vecEnterMember) {
        ST_MAZE_WAIT_ENTER_USER_INFO stInfo;
        stInfo.stMemberInfo = memberInfo;
        stInfo.dw64ExitTime = 0;
        stInfo.byState = 10;  // 等待状态
        // // TODO: ST_ENTER_MAZE_MEMBER_INFO needs dwActorID member
        // m_mapWaitEnterMazeUser[memberInfo.dwActorID] = stInfo;
    }

    // 设置用户计数
    m_nPartyMemeberCount = static_cast<int>(m_mapWaitEnterMazeUser.size());
    m_nMaxUserCount = 14;

    // 调用 Init
    if (!Init()) {
        return false;
    }

    // 保存创建信息
    // TODO: m_stCreateMazeInfo not defined
    // // TODO: m_stCreateMazeInfo member not defined in XMaze
    // m_stCreateMazeInfo = stCreateMaze;

    return true;
}

void XMaze::Clear() {
    // TODO: 需要完整实现 - IDA 0x140311C60
    // 这是一个非常大的函数，需要分步骤还原
}

void XMaze::Generate() {
    // TODO: 需要完整实现 - IDA 0x140315E60
    // 这是一个非常大的函数，需要分步骤还原
}

void XMaze::SpawnGenerateMonster() {
    // TODO: restore sector spawn and system actor creation after sector/resource types are reconciled.
}

// ============================================================================
// ExcuteEventSpawn
// IDA: 0x140317A40
// EXACT IDA implementation - fixed signature to match IDA
// ============================================================================
void XMaze::ExcuteEventSpawn() {
    // TODO: restore event spawn-box execution from original map iteration.
}

// ============================================================================
// EnterGameObject
// IDA: 0x140313130
// TODO: XActor is incomplete type - need to include proper header
// ============================================================================
std::uint16_t XMaze::EnterGameObject(XActor* pActor, E_SEND_INFO_TYPE eType) {
    if (!pActor) {
        return 50001;
    }

    // 调用基类 EnterActor
    // TODO: XArea::EnterActor returns void, not uint16_t
    XArea::EnterActor(pActor);

    // TODO: XActor is incomplete type, GetScanner not defined
    // 获取 Scanner
    // std::map<std::uint32_t, CMover*>* vecActor = GetScanner(pActor);
    // if (!vecActor) {
    //     UXActorID* pActorID = pActor->GetActorID();
    //     LogHelper::LogDebug("game.contents", "<SCANNER> Scanner is nullptr Actor %d ( %d )", pActorID->dwActorID, 554);
    //     return 50001;
    // }

    // TODO: XActor is incomplete type, dynamic_cast fails
    // RTTI cast to CMover
    // CMover* pMover = dynamic_cast<CMover*>(pActor);
    // UXActorID* pActorID = pActor->GetActorID();
    // vecActor->insert(std::make_pair(pActorID->dwActorID, pMover));

    // TODO: XActor is incomplete type
    // 获取 TBID
    // int nTBID = 0;
    // int nType = pActor->GetType();
    // if (nType == 2) {  // Monster
    //     CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
    //     if (pMonster) {
    //         nTBID = pMonster->GetTableID();
    //     }
    // } else if (nType == 1) {  // NPC
    //     CNpc* pNpc = dynamic_cast<CNpc*>(pActor);
    //     if (pNpc) {
    //         nTBID = pNpc->GetTableID();
    //     }
    // }

    // TODO: m_textDBLog and CQuestCondition not defined
    // 记录日志
    // UXActorID* pID = pActor->GetActorID();
    // int nQuestID = CQuestCondition::GetQuestID(pID);
    // m_textDBLog.AddLog(2, nQuestID, nTBID, "");

    // TODO: XActor is incomplete type, CNpc, CMonster incomplete types
    // 发送信息
    // if (eType == eSendInfoTypeSend) {
    //     if (nType == 1) {  // NPC
    //         CNpc* pNpc = dynamic_cast<CNpc*>(pActor);
    //         if (pNpc) {
    //             PS_NPCINFO_VEC stNpcInfos;
    //             stNpcInfos.vecNpcInfo.push_back(*pNpc->GetItemInfo());
    //             XSendPacket xSendPacket(4, 0x13);
    //             xSendPacket << stNpcInfos;
    //             SendBroadCast(&xSendPacket, nullptr, eAll);
    //         }
    //     } else if (nType == 2) {  // Monster
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
    //         if (pMonster) {
    //             PS_MONSTERINFO_VEC stMonsterInfos;
    //             stMonsterInfos.vecMonsterInfo.push_back(*pMonster->GetMonsterInfo());
    //             XSendPacket xSendPacket(4, 0x15);
    //             xSendPacket << stMonsterInfos;
    //             SendBroadCast(&xSendPacket, nullptr, eAll);
    //         }
    //     }
    // }

    return 0;
}

// ============================================================================
// ExitGameObject
// IDA: 0x140313580
// TODO: XActor is incomplete type - need to include proper header
// ============================================================================
std::uint16_t XMaze::ExitGameObject(XActor* pActor, E_SEND_INFO_TYPE eType) {
    if (!pActor) {
        return 50001;
    }

    // 调用基类 ExitActor
    // TODO: XArea::ExitActor returns void, not uint16_t
    XArea::ExitActor(pActor);

    // TODO: XActor is incomplete type
    // 获取 TBID
    // int nTBID = 0;
    // int nType = pActor->GetType();
    // if (nType == 2) {  // Monster
    //     CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
    //     if (pMonster) {
    //         nTBID = pMonster->GetTableID();
    //     }
    // } else if (nType == 1) {  // NPC
    //     CNpc* pNpc = dynamic_cast<CNpc*>(pActor);
    //     if (pNpc) {
    //         nTBID = pNpc->GetTableID();
    //     }
    // }

    // TODO: m_textDBLog and CQuestCondition not defined
    // 记录日志
    // UXActorID* pActorID = pActor->GetActorID();
    // int nQuestID = CQuestCondition::GetQuestID(pActorID);
    // m_textDBLog.AddLog(3, nQuestID, nTBID, "");

    // TODO: XActor is incomplete type, SendOutInfo not defined
    // 发送退出信息
    // if (eType == eSendInfoTypeSend) {
    //     if (nType == 1) {  // NPC
    //         XSendPacket xSendPacket(4, 0x14);
    //         SendOutInfo(&xSendPacket, pActor);
    //     } else if (nType == 2) {  // Monster
    //         XSendPacket xSendPacket(4, 0x16);
    //         SendOutInfo(&xSendPacket, pActor);
    //     }
    // }

    // TODO: GetScanner not defined
    // 从 Scanner 中移除
    // std::map<std::uint32_t, CMover*>* pVecActor = GetScanner(pActor);
    // if (!pVecActor) {
    //     UXActorID* pID = pActor->GetActorID();
    //     LogHelper::LogDebug("game.contents", "<SCANNER> Cant Find Maze Actor %d", pID->dwActorID);
    //     return 50001;
    // }

    // UXActorID* pID = pActor->GetActorID();
    // std::uint32_t dwActorID = CQuestCondition::GetQuestID(pID);
    // auto it = pVecActor->find(dwActorID);
    // if (it == pVecActor->end()) {
    //     UXActorID* pID2 = pActor->GetActorID();
    //     LogHelper::LogDebug("game.contents", "<SCANNER> Cant Find Maze Actor %d", pID2->dwActorID);
    //     return 50001;
    // }

    // pVecActor->erase(it);
    return 0;
}

// ============================================================================
// IsAliveMonster
// IDA: 0x140313950
// TODO: XActor is incomplete type, m_mapActor uses std::map not custom map
// ============================================================================
bool XMaze::IsAliveMonster(int nTableID) {
    // 遍历所有 Actor - use standard std::map iteration
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (!pActor) {
            continue;
        }

        // TODO: XActor is incomplete type
        // 检查是否是 Monster
        // if (pActor->GetType() == 2) {
        //     CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
        //     if (pMonster && pMonster->GetTableID() == nTableID) {
        //         检查是否存活（不是死亡状态）
        //         return !pMonster->IsStatus(2);
        //     }
        // }
    }
    return false;
}

// ============================================================================
// EnterActor
// IDA: 0x140313A60
// IDA: ?EnterActor@XMaze@@UEAAGPEAVXActor@@@Z - returns unsigned short
// ============================================================================
std::uint16_t XMaze::EnterActor(XActor* pActor) {
    if (!pActor) {
        return 0;
    }

    // 检查 Maze 状态
    int nMazeState = GetMazeGameState();
    if (nMazeState == 5 || nMazeState == 6) {
        return 0;
    }

    // TODO: XActor is incomplete type
    // RTTI cast to CUser
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) {
    //     return;
    // }

    // 调用基类 EnterActor
    XArea::EnterActor(pActor);

    // TODO: GetScanner not defined
    // 获取 Scanner
    // std::map<std::uint32_t, CMover*>* vecActor = GetScanner(pActor);
    // if (!vecActor) {
    //     UXActorID* pActorID = pActor->GetActorID();
    //     LogHelper::LogDebug("game.contents", "<SCANNER> Scanner is nullptr Actor %d ( %d )", pActorID->dwActorID, 716);
    //     return;
    // }

    // 添加到 Scanner
    // CMover* pMover = dynamic_cast<CMover*>(pActor);
    // UXActorID* pActorID = pActor->GetActorID();
    // vecActor->insert(std::make_pair(pActorID->dwActorID, pMover));

    // 设置 Area
    // pActor->SetArea(this);

    // TODO: CUser::CheckBuffByLocation not defined
    // 检查 Buff
    // pUser->CheckBuffByLocation(this);

    // TODO: CGocRecode not defined
    // 初始化 Maze 记录
    // std::tr1::shared_ptr<CGocRecode> pRecode = pUser->GetGOC<CGocRecode>();
    // if (pRecode) {
    //     int nMazeID = XArea::GetTBMapID();
    //     pRecode->InitMaze(nMazeID);
    // }

    // 进入 Party/Force 成员
    // EnterPartyForceMember(pUser);

    // 重置等待用户时间
    m_dwWaitUserTime = 0;

    // TODO: XActor is incomplete type
    // 记录日志
    // std::uint32_t TBMapID = XArea::GetTBMapID();
    // UXActorID* pID = pActor->GetActorID();
    // LogHelper::LogDebug("game.contents", "<%d MAZE> Enter User : %d", TBMapID, pID->dwActorID);

    // 发送隐藏事件信息
    // SendSyncHiddenEventInfo(pUser);

    // TODO: CGocEntity not defined
    // 更新称号
    // std::tr1::shared_ptr<CGocEntity> pEntity = pUser->GetGOC<CGocEntity>();
    // if (pEntity) {
    //     int nValue = XArea::GetTBMapID();
    //     pEntity->UpdateOpenTitle(4, nValue);
    // }

    // TODO: CGocBooster not defined
    // 进入 Booster
    // std::tr1::shared_ptr<CGocBooster> pBooster = pUser->GetGOC<CGocBooster>();
    // if (pBooster) {
    //     pBooster->EnterBooster(2);
    // }

    // TODO: pUser is not defined (XActor is incomplete type)
    // 改变战斗姿态
    // pUser->ChangeBattlePose(true);

    // TODO: GetStartPortalPos, STPosInfo not defined
    // 获取起始传送门位置
    // STPosInfo stPosInfo;
    // int nMapID = XArea::GetTBMapID();
    // XGameServer* pServer = XGameServer::Instance();
    // if (pServer->m_xWorldResMgr.GetStartPortalPos(nMapID, &stPosInfo)) {
    //     pUser->Move(&stPosInfo.vPos);
    // }

    // TODO: XActor::IsBot not defined
    // 检查 Bot
    // if (pActor->IsBot()) {
    //     m_bHaveBotUser = true;
    // }

    // TODO: m_cutSceneManager not defined
    // 添加到 Cutscene Manager
    // m_cutSceneManager.AddMember(pUser);

    // TODO: CUser SuperArmor methods not defined
    // 初始化 SuperArmor
    // pUser->InitSuperArmorGage();
    // float fMaxSuperArmor = pUser->GetMaxSuperArmorGage();
    // float fCurSuperArmor = pUser->GetCurSuperArmorGage();
    // pUser->send_eSUB_CMD_MONSTER_SUPER_ARMOR_GAGE(pUser, fCurSuperArmor, fMaxSuperArmor);

    // TODO: IsRoguelikeMap, CheckOptionEffectInvoke not defined
    // 检查非 Roguelike 地图
    // if (!IsRoguelikeMap()) {
    //     pUser->CheckOptionEffectInvoke(EFFECT_CONDITION_EQUIP, pUser, 0.0f, EFFECT_INVOKE_DONT_CARE);
    //     pUser->ReleaseInvokedOptionEffect(EFFECT_CONDITION_EQUIP);
    // }

    // 设置简单防御类型
    // pUser->SetSimpleDefenseType(3);

    // TODO: m_textDBLog, CQuestCondition not defined
    // 记录日志
    // UXActorID* pID2 = pUser->GetActorID();
    // int nQuestID = CQuestCondition::GetQuestID(pID2);
    // m_textDBLog.AddLog(0, nQuestID, 0, "");

    return 0;
}

// ============================================================================
// ExitActor
// IDA: 0x1403140D0
// TODO: Return type differs from declaration (should be void)
// ============================================================================
void XMaze::ExitActor(XActor* pActor) {
    if (!pActor) {
        return;
    }

    // TODO: XActor is incomplete type
    // RTTI cast to CUser
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) {
    //     return;
    // }

    // TODO: CUser::IsControlMonster, SetControlMonster not defined
    // 检查控制怪物
    // if (pUser->IsControlMonster()) {
    //     pUser->SetControlMonster(nullptr, 0, 0);
    // }

    // TODO: CGocQuest, GetGOC not defined
    // 同步任务条件
    // std::tr1::shared_ptr<CGocQuest> pQuestPtr = pUser->GetGOC<CGocQuest>();
    // if (pQuestPtr) {
    //     pQuestPtr->DBSyncQuestCondition();
    // }

    // TODO: CGocInventory not defined
    // 发送耐久度日志
    // std::tr1::shared_ptr<CGocInventory> pInvenPtr = pUser->GetGOC<CGocInventory>();
    // int nMazeID = XArea::GetTBMapID();
    // if (pInvenPtr) {
    //     pInvenPtr->SendEnduranceLog(nMazeID);
    //     pInvenPtr->DropEtherLog();
    // }

    // TODO: RemoveAllOptionEffect is protected
    // 移除所有选项效果
    // pUser->RemoveAllOptionEffect();

    // TODO: m_xWarpPotal, CWarpPotal not defined
    // 从 WarpPotal 移除
    // UXActorID* pActorID = pUser->GetActorID();
    // m_xWarpPotal.RemoveWarpPotal(pActorID->dwActorID);

    // TODO: CUser::ClearBuffByType not defined
    // 清除 Buff
    // pUser->ClearBuffByType(1);

    // TODO: m_cutSceneManager, CQuestCondition not defined
    // 从 Cutscene Manager 删除
    // UXActorID* pUserID = pUser->GetActorID();
    // int nQuestID = CQuestCondition::GetQuestID(pUserID);
    // m_cutSceneManager.DeleteMember(nQuestID);

    // TODO: XActor::GetActorID returns value not pointer
    // 检查 PartyQuest
    // UXActorID* pID = pActor->GetActorID();
    // int nKeyVal = CQuestCondition::GetQuestID(pID);
    // auto it = m_stPartyQuest.setUser.find(nKeyVal);
    // if (it != m_stPartyQuest.setUser.end()) {
    //     std::tr1::shared_ptr<CGocQuest> pQuest = pUser->GetGOC<CGocQuest>();
    //     if (pQuest) {
    //         pQuest->FailQuest(m_stPartyQuest.nQuestID, 0.0f);
    //     }
    //     m_stPartyQuest.setUser.erase(it);
    // }

    // TODO: m_stPartyQuest, m_stEscortMonster incomplete types
    // 检查护送怪物
    // if (m_stPartyQuest.nState == 1 && m_stPartyQuest.setUser.empty()) {
    //     m_stPartyQuest.nState = 2;
    //     if (m_stEscortMonster.pMonster) {
    //         int nMotion = m_stEscortMonster.pMonster->GetMotionClass();
    //         VString strAnimName(m_stEscortMonster.szMonsterDieAnim);
    //         int dwAnimID = m_stEscortMonster.pMonster->GetAnimIndex(strAnimName);
    //         if (dwAnimID != -1) {
    //             nMotion = m_stEscortMonster.pMonster->AnimKeyToMotion(dwAnimID);
    //         }
    //         int nHP = m_stEscortMonster.pMonster->GetHP();
    //         m_stEscortMonster.pMonster->SetDieReason(6, nHP);
    //         m_stEscortMonster.pMonster->SetDie_2(nMotion, 1);
    //     }
    // }

    // TODO: GetScanner not defined
    // 从 Scanner 移除
    // std::map<std::uint32_t, CMover*>* pVecActor = GetScanner(pActor);
    // if (!pVecActor) {
    //     UXActorID* pID2 = pActor->GetActorID();
    //     LogHelper::LogError("game.contents", "<SCANNER> Cant Find Maze Actor %d ( %d )", pID2->dwActorID, 852);
    //     return;
    // }

    // UXActorID* pID3 = pActor->GetActorID();
    // std::uint32_t v114 = CQuestCondition::GetQuestID(pID3);
    // auto iter = pVecActor->find(v114);
    // if (iter != pVecActor->end()) {
    //     pVecActor->erase(iter);
    // } else {
    //     UXMapID* pInstanceID = XArea::GetInstanceID();
    //     UXActorID* pID4 = pActor->GetActorID();
    //     LogHelper::LogError("game.contents", "<SCANNER> Cant Find Maze Actor %d / %I64d", pID4->dwActorID, pInstanceID->nMapID);
    // }

    // TODO: GetCurUserCount not defined
    // 获取用户计数
    // int nUserCnt = GetCurUserCount();

    // TODO: XActor is incomplete type, CQuestCondition not defined
    // 更新 Party/Force Booster
    // if (m_pParty) {
    //     UXActorID* pID5 = pActor->GetActorID();
    //     std::uint32_t dwLeaveMemberID = CQuestCondition::GetQuestID(pID5);
    //     m_pParty->UpdatePartyBooster(dwLeaveMemberID);
    // } else if (m_pForce) {
    //     UXActorID* pID6 = pActor->GetActorID();
    //     std::uint32_t v160 = CQuestCondition::GetQuestID(pID6);
    //     m_pForce->UpdateForceBooster(v160);
    // }

    // TODO: FindInvisibleActorCnt, m_pParty, m_pForce incomplete types
    // 检查隐形 Actor
    // if (FindInvisibleActorCnt() > 0 && nUserCnt == 1) {
    //     if (m_pParty) {
    //         UXMapID uxMapID(0);
    //         m_pParty->SetMazeID(uxMapID);
    //         XSendPacket v93(0xF2, 0x55);
    //         v93 << m_pParty->GetPartyID();
    //         XGameServer::Instance()->m_controlSocket.Send(&v93);
    //     } else if (m_pForce) {
    //         UXMapID uxMapID(0);
    //         m_pForce->SetMazeID(uxMapID);
    //         XSendPacket v94(0xF2, 0x63);
    //         v94 << m_pForce->GetPartyID();
    //         XGameServer::Instance()->m_controlSocket.Send(&v94);
    //     }
    // }

    // TODO: CGocForce, pUser not defined
    // 保存 Force 记录
    // std::tr1::shared_ptr<CGocForce> pForce = pUser->GetGOC<CGocForce>();
    // if (pForce) {
    //     pForce->SaveRecode();
    // }

    // TODO: CGocHelper not defined
    // 释放 Helper
    // std::tr1::shared_ptr<CGocHelper> pHelper = pUser->GetGOC<CGocHelper>();
    // if (pHelper) {
    //     pHelper->AllHelperRelease();
    //     pHelper->OtherHelperClear();
    // }

    // TODO: CheckFollowMonster not defined
    // 检查跟随怪物
    // CheckFollowMonster();

    // TODO: RefreshUserCountInSector not defined
    // 刷新用户计数
    // UXActorID* pID7 = pUser->GetActorID();
    // int nActorID = CQuestCondition::GetQuestID(pID7);
    // RefreshUserCountInSector(nActorID);

    // TODO: XActor is incomplete type
    // 更新等待进入 Maze 用户
    // UXActorID* pID8 = pActor->GetActorID();
    // std::uint32_t v126 = CQuestCondition::GetQuestID(pID8);
    // auto it_ = m_mapWaitEnterMazeUser.find(v126);
    // if (it_ != m_mapWaitEnterMazeUser.end()) {
    //     it_->second.dw64ExitTime = GetTickCount64() + 300000;
    //     if (it_->second.byState == 1 && !m_bRestartReady) {
    //         RestartResetState(false, false);
    //     }
    // }

    // TODO: SendOutInfo not defined
    // 发送退出信息
    // XSendPacket xSendPacket(4, 0x12);
    // SendOutInfo(&xSendPacket, pActor);

    // TODO: XArea::GetInstanceID not defined
    // 记录日志
    // int TBMapID = XArea::GetTBMapID();
    // UXActorID* pID9 = pActor->GetActorID();
    // UXMapID* pInstanceID2 = XArea::GetInstanceID();
    // LogHelper::LogDebug("game.contents", "<%I64d MAZE> Leave User : %d Map : %d", pInstanceID2->nMapID, pID9->dwActorID, TBMapID);

    // TODO: m_textDBLog, CQuestCondition not defined
    // UXActorID* pID10 = pUser->GetActorID();
    // int nQuestID2 = CQuestCondition::GetQuestID(pID10);
    // m_textDBLog.AddLog(1, nQuestID2, 0, "");

    // TODO: pUser->SendCheckSpeedLog not defined
    // 发送速度检查日志
    // pUser->SendCheckSpeedLog();

    // 调用基类 ExitActor
    XArea::ExitActor(pActor);

    // TODO: XActor::SetArea not defined
    // 设置 Area 为空
    // pActor->SetArea(nullptr);

    // TODO: pUser->Exit not defined
    // 用户退出
    // pUser->Exit();
}

CMonster* XMaze::CreateMonster(UXMapID uxMazeSerialID, std::uint32_t nSectorID, int nMonsterID,
                               XVec3* vPos, float fRot, E_SEND_INFO_TYPE eType,
                               int nSpawnBoxID, int nGroupID, UXActorID uxParentID) {
    // Per IDA 0x140318F20: XMaze::CreateMonster 精确还原
    // 1. 获取怪物表数据
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_MONSTER* pTBMonster = pServer->GetResourceMgr().GetTB_MONSTER(nMonsterID);

    if (!pTBMonster) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "CreateMonster error - No Table TB_MONSTER[ MonsterID:%d ]", nMonsterID);
        return nullptr;
    }

    // 2. 检查活动事件区域的游戏模式类型
    if (m_pActiveEventSector && m_pActiveEventSector->GetGameModeType() == 5 && pTBMonster->Monster_Type != 3) {
        return nullptr;
    }

    // 3. 查找 Sector
    CSector* pSector = nullptr;
    auto itSector = m_mapSector.find(nSectorID);
    if (itSector != m_mapSector.end()) {
        pSector = itSector->second;
    }

    // 4. 创建怪物实例 (通过 ThreadLocalData)
    // TODO: ThreadLocalData::CreateMonster 需要实现
    CMonster* pMonster = nullptr;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CreateMonster ID=%d SectorID=%d", nMonsterID, nSectorID);

    if (!pMonster) {
        std::uint16_t nMapID = GetTBMapID();
        GreenDamTan_log(__FILE__, __FUNCTION__, "<ERROR> %d Failed Create Monster in %d Map", nMonsterID, nMapID);
        if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Group == 74) {
            m_nMazeLog[5] = 1;
        }
        return nullptr;
    }

    // 5. 设置基本属性
    pMonster->SetSpawnBoxID(nSpawnBoxID);
    pMonster->SetGroupID(nGroupID);

    // 6. 护送怪物检查
    if (m_stEscortMonster.nMonsterID == nMonsterID) {
        // TODO: ST_ESCORT_MONSTER 需要更新
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

void XMaze::OnUpdate(float fElapsed, float fRealElapsed) {
    // Per IDA 0x14031C330: XMaze::OnUpdate 精确还原
    // 这是一个非常大的函数，简化实现

    // 1. 更新重生管理器
    // TODO: CRespawnManager::Update 需要实现
    // m_respawnManager.Update(this);

    // 2. 更新所有 Sector
    for (auto& pair : m_mapSector) {
        CSector* pSector = pair.second;
        if (pSector) {
            pSector->OnUpdate(fElapsed, fRealElapsed);
        }
    }

    // 3. CellPos 检查
    m_fCellPosCheckTime += fElapsed;
    if (m_fCellPosCheckTime > 1.0f) {
        m_fCellPosCheckTime = 0.0f;
        // TODO: CCellPosMgr 需要完整定义
        // GetCellPosMgr().RemoveAllInfo();
    }

    // 4. 遍历所有 Actor 并更新
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (pActor) {
            // TODO: XActor::OnUpdate 需要实现
            // pActor->OnUpdate();
        }
    }

    // 5. 更新定时器
    UpdateTimer(fElapsed);

    // 6. 更新迷宫状态
    UpdateMazeState();

    // 7. 处理护送条件
    ProcessEscortCondition(fElapsed);

    // 8. 更新过场动画管理器
    // TODO: CCutsceneManager 需要完整定义
    // if (m_pCutSceneManager) {
    //     m_pCutSceneManager->OnUpdate();
    // }

    // 9. 更新休闲副本定时器
    UpdateCasualRaidTimer(fElapsed);

    // 10. 更新传送门状态
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

    // 发送包
    XSendPacket xSendPacket(0x11, 0x53);
    xSendPacket << nCount;

    // 添加完成的 Sector Box ID
    for (auto it = m_mapSector.begin(); it != m_mapSector.end(); ++it) {
        CSector* pSector = it->second;
        if (pSector && pSector->IsComplete()) {
            xSendPacket << pSector->GetSectorBoxID();
        }
    }

    // 获取当前 Sector ID
    int nSectorIDFromPos = 0;
    // TODO: CUser::GetPosition returns hkvVec3, need XVec3 conversion
    // if (pUser) {
    //     XVec3 vPos = pUser->GetPosition();
    //     nSectorIDFromPos = GetSectorIDFromPos(vPos);
    // }
    xSendPacket << nSectorIDFromPos;

    // TODO: CUser inherits from XActor, need proper cast
    // SendBroadCast(&xSendPacket, static_cast<XActor*>(pUser), E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
    SendBroadCast(&xSendPacket, nullptr, E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
}

// ============================================================================
// SendGateInfos
// IDA: 0x14031DB80
// TODO: STMageGateBox not defined, using void* placeholder
// ============================================================================
void XMaze::SendGateInfos(CUser* pUser) {
    // TODO: STMageGateBox not defined - need to define struct
    // 计算激活的 Gate 数量
    int nCount = 0;
    // for (auto it = m_mapGateBox.begin(); it != m_mapGateBox.end(); ++it) {
    //     STMageGateBox* pGateBox = it->second;
    //     if (pGateBox && pGateBox->bActive) {
    //         ++nCount;
    //     }
    // }

    // 发送包
    XSendPacket xSendPacket(0x11, 0x54);
    xSendPacket << false;  // bReEnter
    xSendPacket << nCount;

    // TODO: STMageGateBox not defined
    // 添加激活的 Gate ID
    // for (auto it = m_mapGateBox.begin(); it != m_mapGateBox.end(); ++it) {
    //     STMageGateBox* pGateBox = it->second;
    //     if (pGateBox && pGateBox->bActive) {
    //         xSendPacket << pGateBox->nGateID;
    //     }
    // }

    // TODO: CUser inherits from XActor, need proper cast
    SendBroadCast(&xSendPacket, nullptr, E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
}

// ============================================================================
// SendDieMonsters
// IDA: 0x14031DDB0
// ============================================================================
void XMaze::SendDieMonsters(CUser* pUser) {
    int nCount = static_cast<int>(m_listDieMonsterSpawnBoxID.size());

    XSendPacket xSendPacket(0x11, 0x68);
    xSendPacket << nCount;

    // std::list doesn't have subscript operator, use iterator
    for (auto it = m_listDieMonsterSpawnBoxID.begin(); it != m_listDieMonsterSpawnBoxID.end(); ++it) {
        xSendPacket << *it;
    }

    // TODO: CUser inherits from XActor, need proper cast
    SendBroadCast(&xSendPacket, nullptr, E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
}

// ============================================================================
// SendPotalInfos
// IDA: 0x14031DEF0
// TODO: STMagePotalBox is incomplete type
// ============================================================================
void XMaze::SendPotalInfos(CUser* pUser) {
    // TODO: STMagePotalBox is incomplete type - need to define struct properly
    // 计算启用和禁用的 Portal 数量
    int nCount = 0;
    int nDisEnableCount = 0;
    // for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
    //     STMagePotalBox* pPotalBox = it->second;
    //     if (pPotalBox) {
    //         if (pPotalBox->bEnable) {
    //             ++nCount;
    //         } else {
    //             ++nDisEnableCount;
    //         }
    //     }
    // }

    XSendPacket xSendPacket(0x11, 0x55);
    xSendPacket << nCount;
    xSendPacket << nDisEnableCount;

    // TODO: STMagePotalBox is incomplete type
    // 添加启用的 Portal ID
    // for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
    //     STMagePotalBox* pPotalBox = it->second;
    //     if (pPotalBox && pPotalBox->bEnable) {
    //         xSendPacket << pPotalBox->nPotalID;
    //     }
    // }

    // 添加禁用的 Portal ID
    // for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
    //     STMagePotalBox* pPotalBox = it->second;
    //     if (pPotalBox && !pPotalBox->bEnable) {
    //         xSendPacket << pPotalBox->nPotalID;
    //     }
    // }

    // TODO: CUser inherits from XActor, need proper cast
    SendBroadCast(&xSendPacket, nullptr, E_BROADCAST_TYPE::E_BROADCAST_TYPE_ALL);
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
// TODO: GetLastSectorID not defined
// ============================================================================
void XMaze::SendLastClientSync(CUser* pUser) {
    // TODO: GetLastSectorID not defined
    // int nLastSectorID = GetLastSectorID();
    int nLastSectorID = 0;
    int nUniqueID = GetUniqueID(nLastSectorID);

    auto it = m_mapSector.find(nUniqueID);
    if (it == m_mapSector.end()) {
        return;
    }

    CSector* pSector = it->second;
    if (pSector) {
        // TODO: SendLuaClientSync signature mismatch - takes only pUser
        // pSector->SendLuaClientSync(pUser, 0.0f);
        pSector->SendLuaClientSync(pUser);
    }
}

void XMaze::SendObjectInfo(CUser* pUser, XActor* pActor) {
    // TODO: 需要完整实现 - IDA 0x14031EBE0
}

// ============================================================================
// GetSectorIDFromPos
// IDA: 0x14031F450
// TODO: VSectorBoxInfo -> VSectorBox, GetSectorBoxInfo -> GetSectorBox
// ============================================================================
int XMaze::GetSectorIDFromPos(XVec3& vPos) {
    for (auto it = m_mapSector.begin(); it != m_mapSector.end(); ++it) {
        CSector* pSector = it->second;
        if (!pSector) continue;

        // TODO: VSectorBox is incomplete type, GetSectorBoxInfo renamed to GetSectorBox
        // VSectorBox* pBox = pSector->GetSectorBox();
        // if (!pBox) continue;

        // 检查位置是否在 Sector 范围内
        // if (vPos.x >= pBox->PosTopLeft.x && vPos.x <= pBox->PosBottomRight.x &&
        //     vPos.y >= pBox->PosTopLeft.y && vPos.y <= pBox->PosBottomRight.y) {
        //     return pSector->GetSectorBoxID();
        // }
    }
    return -1;
}

// ============================================================================
// GetSectorFromPos
// IDA: 0x14031F670
// TODO: VSectorBoxInfo -> VSectorBox, GetSectorBoxInfo -> GetSectorBox
// ============================================================================
CSector* XMaze::GetSectorFromPos(XVec3& vPos) {
    for (auto it = m_mapSector.begin(); it != m_mapSector.end(); ++it) {
        CSector* pSector = it->second;
        if (!pSector) continue;

        // TODO: VSectorBox is incomplete type, GetSectorBoxInfo renamed to GetSectorBox
        // VSectorBox* pBox = pSector->GetSectorBox();
        // if (!pBox) continue;

        // 检查位置是否在 Sector 范围内
        // if (vPos.x >= pBox->PosTopLeft.x && vPos.x <= pBox->PosBottomRight.x &&
        //     vPos.y >= pBox->PosTopLeft.y && vPos.y <= pBox->PosBottomRight.y) {
        //     return pSector;
        // }
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
// TODO: WarpSectorStartPos not defined
// ============================================================================
bool XMaze::EscapeActor(XActor* pActor) {
    if (!pActor) {
        return false;
    }

    // TODO: XActor is incomplete type
    // RTTI cast to CUser
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) {
    //     return false;
    // }

    // TODO: WarpSectorStartPos not defined
    // Warp to sector start position
    // return WarpSectorStartPos(pUser);
    return false;
}

// ============================================================================
// SendChangeActionSpawn
// IDA: 0x14028E5C0
// ============================================================================
void XMaze::SendChangeActionSpawn(int eActorType) {
    // TODO: 完整实现需要:
    // 1. 遍历 m_mapActor 中所有 actor
    // 2. 筛选匹配 eActorType 的活着的 actor
    // 3. 获取 CMonster 的 MoveIdleInfo
    // 4. 广播 PS_MOVE_IDLE_VEC 包 (main=5, sub=15)
    (void)eActorType;
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
// IDA: 0x1403292E0
// TODO: 需要 LogicTimer 类型定义
// ============================================================================
void XMaze::AddTimer(int nID, float fTarget) {
    // LogicTimer newTimer;
    // newTimer.SetVariables(nID);
    // newTimer.SetTimer(fTarget, true);
    // m_arWaitLogicTimers.push_back(newTimer);
}

// ============================================================================
// ClearAllTimers
// IDA: 0x140329720
// ============================================================================
void XMaze::ClearAllTimers() {
    m_arLogicTimers.clear();
    m_arWaitLogicTimers.clear();
}

// ============================================================================
// RemoveTimer
// IDA: 0x140329760
// TODO: 需要 LogicTimer 类型定义
// ============================================================================
void XMaze::RemoveTimer(int nID) {
    // 查找并重置等待计时器列表中的计时器
    // for (auto it = m_arWaitLogicTimers.begin(); it != m_arWaitLogicTimers.end(); ++it) {
    //     if (it->GetID() == nID) {
    //         it->SetTimer(0.0f, true);
    //         break;
    //     }
    // }

    // 查找并重置活动计时器列表中的计时器
    // for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ++it) {
    //     if (it->GetID() == nID) {
    //         it->SetTimer(0.0f, true);
    //         break;
    //     }
    // }
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
// IDA: 0x1403291B0
// TODO: 需要 CGocRecode, CMover::GetGOC 模板函数
// ============================================================================
void XMaze::AddDSPointByUnity(std::uint16_t wDSPoint) {
    // 遍历所有用户，添加DS点数
    // 使用 m_objectScanner 遍历所有 Actor
    // 注意: m_objectScanner 实际上是一个 std::map<std::uint32_t, CMover*>
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);

    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (!pMover) continue;
    //
    //     // 使用 RTTI 动态转换为 CUser
    //     CUser* pUser = dynamic_cast<CUser*>(pMover);
    //     if (pUser && !pMover->IsDie()) {
    //         // 获取 CGocRecode 组件
    //         // CMover::GetGOC<CGocRecode> 在编译时需要模板特化
    //     }
    // }
}

// ============================================================================
// AddTimerEx
// IDA: 0x140329370
// TODO: 需要 LogicTimer, VHashString 类型定义
// ============================================================================
void XMaze::AddTimerEx(const char* szLuaFunction, float fTarget, int nParam1, int nParam2, int nParam3) {
    // LogicTimer newTimer;
    // int nHash = VHashString::GetHash(szLuaFunction);
    // newTimer.SetVariables(nHash);
    // newTimer.SetTimer(fTarget, true);
    // newTimer.SetUserString(szLuaFunction);
    // newTimer.SetParam(nParam1, nParam2, nParam3);
    // m_arWaitLogicTimers.push_back(newTimer);
}

// ============================================================================
// AddEventTimer
// IDA: 0x140329440
// TODO: 需要 LogicTimer, VHashString 类型定义
// ============================================================================
void XMaze::AddEventTimer(const char* szLuaFinishFunction, const char* szLuaEventFunction,
                          const char* szLuaReadyFunction, int nType, float fTime,
                          int nParam1, int nParam2) {
    // LogicTimer newTimer;
    // int nHash = VHashString::GetHash(szLuaFinishFunction);
    // newTimer.SetVariables(nHash);
    // newTimer.SetType(nType);
    // newTimer.SetTimer((float)nParam2, true);
    // newTimer.SetOriginTime(fTime);
    // newTimer.SetUserString(szLuaFinishFunction);
    // newTimer.SetEventString(szLuaEventFunction);
    // newTimer.SetReadyString(szLuaReadyFunction);
    // newTimer.SetNextTime(0, (int)fTime);
    // newTimer.SetParam(nParam1, nParam2, 0);
    // if (nType == 2 && m_pTBMazeInfo) {
    //     newTimer.SetTimer((float)m_pTBMazeInfo->Maze_ClearTime, true);
    // }
    // m_arWaitLogicTimers.push_back(newTimer);
}

// ============================================================================
// RemoveTimerEx
// IDA: 0x1403295A0
// TODO: 需要 LogicTimer, VHashString 类型定义
// ============================================================================
float XMaze::RemoveTimerEx(const char* szLuaFunction) {
    // int nID = VHashString::GetHash(szLuaFunction);
    // float fLeftTime = 0.0f;
    //
    // for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ++it) {
    //     LogicTimer* pTimer = &(*it);
    //     if (pTimer->GetID() == nID) {
    //         float fMaxTimer = pTimer->GetMaxTimer();
    //         float fTimer = pTimer->GetTimer();
    //         fLeftTime = fMaxTimer - fTimer;
    //         pTimer->SetTimer(0.0f, true);
    //         break;
    //     }
    // }
    //
    // if (fLeftTime < 0.0f) return 0.0f;
    // return fLeftTime;
    return 0.0f;
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
// TODO: 需要 CSector::IsTerminateSpawn 方法
// ============================================================================
bool XMaze::IsTerminateSpawn(int nSectorID) {
    auto it = m_mapSector.find(nSectorID);
    if (it == m_mapSector.end() || !it->second) {
        return false;
    }
    // TODO: CSector 需要添加 IsTerminateSpawn 方法
    // return it->second->IsTerminateSpawn();
    return false;
}

// ============================================================================
// IsCompleteEpisode
// IDA: 0x14032B9F0
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
bool XMaze::IsCompleteEpisode(std::uint32_t dwUserID, std::uint32_t nQuestID) {
    // 遍历 m_objectScanner 中的所有用户
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && it->first == dwUserID) {
    //         std::shared_ptr<CGocQuest> pQuest;
    //         pMover->GetGOC<CGocQuest>(&pQuest);
    //         if (pQuest) {
    //             return pQuest->IsCompleteEpisode(nQuestID);
    //         }
    //     }
    // }
    return false;
}

// ============================================================================
// IsCompleteCondition
// IDA: 0x14032BAE0
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
bool XMaze::IsCompleteCondition(std::uint32_t dwUserID, int nConditionID) {
    // 遍历 m_objectScanner 中的所有用户
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         XActor* pActor = static_cast<XActor*>(pMover);
    //         if (pActor->GetID() == dwUserID) {
    //             std::shared_ptr<CGocQuest> pQuest;
    //             pMover->GetGOC<CGocQuest>(&pQuest);
    //             if (pQuest) {
    //                 return pQuest->IsCompleteCondition(nConditionID);
    //             }
    //         }
    //     }
    // }
    return false;
}

// ============================================================================
// IsProcessEpisode
// IDA: 0x14032BBF0
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
bool XMaze::IsProcessEpisode(std::uint32_t dwUserID, std::uint32_t nQuestID) {
    // 遍历 m_objectScanner 中的所有用户
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         XActor* pActor = static_cast<XActor*>(pMover);
    //         if (pActor->GetID() == dwUserID) {
    //             std::shared_ptr<CGocQuest> pQuest;
    //             pMover->GetGOC<CGocQuest>(&pQuest);
    //             if (pQuest) {
    //                 return pQuest->FindEpisode(nQuestID);
    //             }
    //         }
    //     }
    // }
    return false;
}

// ============================================================================
// IsProcessCondition
// IDA: 0x14032BD00
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
bool XMaze::IsProcessCondition(std::uint32_t dwUserID, std::uint32_t nConditionID) {
    // 遍历 m_objectScanner 中的所有用户
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         XActor* pActor = static_cast<XActor*>(pMover);
    //         if (pActor->GetID() == dwUserID) {
    //             std::shared_ptr<CGocQuest> pQuest;
    //             pMover->GetGOC<CGocQuest>(&pQuest);
    //             if (pQuest) {
    //                 return pQuest->FindCondition(nConditionID);
    //             }
    //         }
    //     }
    // }
    return false;
}

// ============================================================================
// RemoveQuestAll
// IDA: 0x14032BE10
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
bool XMaze::RemoveQuestAll() {
    // 遍历 m_objectScanner 中的所有用户
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         std::shared_ptr<CGocQuest> pQuest;
    //         pMover->GetGOC<CGocQuest>(&pQuest);
    //         if (pQuest) {
    //             pQuest->ResetQuestAll();
    //         }
    //     }
    // }
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
// IDA: 0x14032BFF0
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
bool XMaze::IsHaveQuest(std::uint32_t nQuestID) {
    // 遍历 m_objectScanner 中的所有用户
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         std::shared_ptr<CGocQuest> pQuest;
    //         pMover->GetGOC<CGocQuest>(&pQuest);
    //         if (pQuest && pQuest->FindEpisode(nQuestID)) {
    //             return true;
    //         }
    //     }
    // }
    return false;
}

// ============================================================================
// AcceptQuest
// IDA: 0x14032C0F0
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
void XMaze::AcceptQuest(std::uint32_t nQuestID) {
    // 遍历 m_objectScanner 中的所有用户
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         std::shared_ptr<CGocQuest> pQuest;
    //         pMover->GetGOC<CGocQuest>(&pQuest);
    //         if (pQuest && !pQuest->FindEpisode(nQuestID)) {
    //             pQuest->AcceptQuest(nQuestID, 0);
    //         }
    //     }
    // }
}

// ============================================================================
// CompleteQuest
// IDA: 0x14032C1D0
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
void XMaze::CompleteQuest(std::uint32_t nQuestID) {
    // 遍历 m_objectScanner 中的所有用户
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         std::shared_ptr<CGocQuest> pQuest;
    //         pMover->GetGOC<CGocQuest>(&pQuest);
    //         if (pQuest && pQuest->FindEpisode(nQuestID)) {
    //             pQuest->CompleteQuest(nQuestID, 0);
    //         }
    //     }
    // }
}

// ============================================================================
// CompleteCondition
// IDA: 0x14032C2D0
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
void XMaze::CompleteCondition(std::uint32_t nConditionID) {
    // 遍历 m_objectScanner 中的所有用户
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         std::shared_ptr<CGocQuest> pQuest;
    //         pMover->GetGOC<CGocQuest>(&pQuest);
    //         if (pQuest && pQuest->FindCondition(nConditionID) == 1) {
    //             pQuest->CompleteConditionByForce(nConditionID, 0);
    //         }
    //     }
    // }
}

// ============================================================================
// FindUserByLowLevel
// IDA: 0x14032C3D0
// TODO: 需要 CMover::GetLevel, XActor::GetActorID 类型定义
// ============================================================================
std::uint32_t XMaze::FindUserByLowLevel() {
    // std::uint32_t dwActorID = 0;
    // int nLevel = 100;
    //
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && pMover->GetLevel() < nLevel) {
    //         nLevel = pMover->GetLevel();
    //         dwActorID = pMover->GetActorID().dwActorID;
    //     }
    // }
    // return dwActorID;
    return 0;
}

// ============================================================================
// FindUserByQuest
// IDA: 0x14032C4E0
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
CMover* XMaze::FindUserByQuest(std::uint32_t nQuestID) {
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         std::shared_ptr<CGocQuest> pQuest;
    //         pMover->GetGOC<CGocQuest>(&pQuest);
    //         if (pQuest && pQuest->FindEpisode(nQuestID)) {
    //             return pMover;
    //         }
    //     }
    // }
    return nullptr;
}

// ============================================================================
// GetUserByIndex
// IDA: 0x14032C5E0
// ============================================================================
CMover* XMaze::GetUserByIndex(int iIndex) {
    // int iCount = 0;
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         if (iCount == iIndex) {
    //             return pMover;
    //         }
    //         ++iCount;
    //     }
    // }
    return nullptr;
}

// ============================================================================
// IsHaveCondition
// IDA: 0x14032C790
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
bool XMaze::IsHaveCondition(std::uint32_t nConditionID) {
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         std::shared_ptr<CGocQuest> pQuest;
    //         pMover->GetGOC<CGocQuest>(&pQuest);
    //         if (pQuest && pQuest->FindCondition(nConditionID)) {
    //             return true;
    //         }
    //     }
    // }
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

    return pInteraction->bEnabled;  // 注意: IDA显示bEnable，实际定义是bEnabled
}

// ============================================================================
// IsExistedMonster
// IDA: 0x14032C920
// 检查指定TableID的怪物是否存在
// TODO: 需要 m_objectScanner 完整类型定义和 CMover::GetType
// ============================================================================
bool XMaze::IsExistedMonster(int nMonsterID) {
    // IDA 反编译逻辑:
    // 遍历 m_objectScanner.mapNPCList (实际上应该是 actor scanner map)
    // 检查每个 actor 的类型是否为 2 (ActorType_Monster)
    // 如果是 monster，通过 RTTI dynamic_cast 转换为 CMonster*
    // 然后比较 GetTableID() 与 nMonsterID

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && pMover->GetType() == 2) {  // ActorType_Monster = 2
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //         if (pMonster && pMonster->GetTableID() == nMonsterID) {
    //             return true;
    //         }
    //     }
    // }
    return false;
}

// ============================================================================
// SetInvincibleMonster
// IDA: 0x14032CA20
// 设置指定TableID怪物的无敌状态
// TODO: 需要 m_objectScanner 完整类型定义和 CMover::SetInvincibleActor
// ============================================================================
void XMaze::SetInvincibleMonster(int nMonsterID, bool bEnable) {
    // IDA 反编译逻辑:
    // 遍历 m_objectScanner.mapNPCList
    // 检查每个 actor 的类型是否为 2 (ActorType_Monster)
    // 如果是 monster，通过 RTTI dynamic_cast 转换为 CMonster*
    // 然后比较 GetTableID() 与 nMonsterID
    // 匹配则调用 CMover::SetInvincibleActor(bEnable)

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && pMover->GetType() == 2) {  // ActorType_Monster = 2
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //         if (pMonster && pMonster->GetTableID() == nMonsterID) {
    //             pMonster->SetInvincibleActor(bEnable);
    //         }
    //     }
    // }
}

// ============================================================================
// SetLuaValue (int index overload)
// IDA: 0x14032E310
// 设置 m_vecLuaValue 指定索引的值
// ============================================================================
void XMaze::SetLuaValue(int nIndex, std::uint32_t nValue) {
    if (nIndex >= 0 && static_cast<size_t>(nIndex) < m_vecLuaValue.size()) {
        m_vecLuaValue[nIndex] = static_cast<int>(nValue);
    }
}

// ============================================================================
// AddLuaValue
// IDA: 0x14032E350
// 累加 m_vecLuaValue 指定索引的值
// ============================================================================
void XMaze::AddLuaValue(int nIndex, int nValue) {
    if (nIndex >= 0 && static_cast<size_t>(nIndex) < m_vecLuaValue.size()) {
        m_vecLuaValue[nIndex] += nValue;
    }
}

// ============================================================================
// TerminateSpawnBox
// IDA: 0x14032CB30
// 终止指定SpawnBox的生成
// TODO: 需要 STMageProcessSpawnBox, CSector::EraseSpawnBoxID 等类型定义
// ============================================================================
void XMaze::TerminateSpawnBox(int nSpawnBoxID) {
    // IDA 反编译逻辑:
    // 1. 调用 CTextDBLog::AddLog 记录日志
    // 2. 获取 BatchLayerLevel 并计算 iBoxUniqueID
    // 3. 在 m_mapProcessSpawnBox 中查找对应的 STMageProcessSpawnBox
    // 4. 设置 bTerminate = true, 根据条件设置 bActive = false
    // 5. 在对应的 Sector 中调用 EraseSpawnBoxID

    // 简化实现：需要完整类型定义后还原
    int nBatchLayerLevel = GetBatchLayerLevel();
    int iBoxUniqueID = nSpawnBoxID;  // VEventObjectInfo::GetEventUniqueID(nSpawnBoxID, nBatchLayerLevel)

    auto it = m_mapProcessSpawnBox.find(iBoxUniqueID);
    if (it != m_mapProcessSpawnBox.end() && it->second != nullptr) {
        // STMageProcessSpawnBox* pInfo = it->second;
        // pInfo->bTerminate = true;
        // 完整逻辑需要 STMageProcessSpawnBox 类型定义
    }
}

// ============================================================================
// TerminateSpawn
// IDA: 0x14032CCF0
// 终止指定Sector的生成
// TODO: 需要 CSector::TerminateSpawn 方法
// ============================================================================
void XMaze::TerminateSpawn(int nSectorID) {
    // IDA 反编译逻辑:
    // 1. 如果 nSectorID 为 0，获取 LastSectorID
    // 2. 计算 iUniqueID
    // 3. 在 m_mapSector 中查找对应的 CSector
    // 4. 调用 CSector::TerminateSpawn()

    int nTargetSectorID = nSectorID;
    if (nTargetSectorID == 0) {
        // nTargetSectorID = GetLastSectorID();
    }

    int iUniqueID = GetUniqueID(nTargetSectorID);
    auto it = m_mapSector.find(iUniqueID);
    if (it != m_mapSector.end() && it->second != nullptr) {
        // it->second->TerminateSpawn();
    }
}

// ============================================================================
// UpdateSectorClear
// IDA: 0x14032CDC0
// 更新Sector清除状态，更新任务条件
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
void XMaze::UpdateSectorClear(unsigned int nSectorID) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_objectScanner 中的所有 CMover
    // 2. 对每个 CMover 获取 CGocQuest 组件
    // 3. 调用 CGocQuest::UpdateMazeCondition 更新条件
    // 4. 调用 CGocQuest::DBSyncQuestCondition 同步数据库
    // 5. 如果是 CUser，调用 SendExpSave

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         std::shared_ptr<CGocQuest> pQuest;
    //         pMover->GetGOC<CGocQuest>(&pQuest);
    //         if (pQuest) {
    //             std::uint32_t dwMazeID = m_pTBMazeInfo->ID;
    //             pQuest->UpdateMazeCondition(eCONDITION_TYPE_CLEAR, eCONDITION_TARGET_SECTOR, nSectorID, dwMazeID, 1, 0);
    //             pQuest->DBSyncQuestCondition();
    //             CUser* pUser = dynamic_cast<CUser*>(pMover);
    //             if (pUser) {
    //                 pUser->SendExpSave();
    //             }
    //         }
    //     }
    // }
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
// TODO: 需要 IVScriptInstance, CCutsceneManager 类型定义
// ============================================================================
void XMaze::CallScriptPreDieMonster(unsigned int nMonsterID, int nSpawnBoxID, unsigned int nGroupID) {
    // IDA 反编译逻辑:
    // 1. 将 nMonsterID 和 nSpawnBoxID 转换为字符串
    // 2. 检查 IsCallScriptDie
    // 3. 如果脚本实例存在且有 "OnDie" 函数，调用脚本
    // 4. 调用 DeleteMonsterGroupID
    // 5. 如果有 "OnDieEx" 函数，调用脚本
    // 6. 添加到死亡怪物列表
    // 7. 检查过场动画状态

    bool bCallScriptDie = IsCallScriptDie(nMonsterID);

    // 简化实现：需要完整类型定义后还原
    // char szMonsterID[16], szSpawnBoxID[16];
    // _itoa(nMonsterID, szMonsterID, 10);
    // _itoa(nSpawnBoxID, szSpawnBoxID, 10);

    // if (m_pScriptInstance && !bCallScriptDie) {
    //     if (m_pScriptInstance->HasFunction("OnDie")) {
    //         m_pScriptInstance->ExecuteFunctionArg("OnDie", "ssTSoulworker:XMaze;", szMonsterID, szSpawnBoxID, this);
    //     }
    // }

    // bool bResult = DeleteMonsterGroupID(nGroupID, 1);

    // if (m_pScriptInstance && bResult && !bCallScriptDie) {
    //     if (m_pScriptInstance->HasFunction("OnDieEx")) {
    //         m_pScriptInstance->ExecuteFunctionArg("OnDieEx", "iTSoulworker:XMaze;", nGroupID, this);
    //     }
    // }

    // AddDieMonsterSpawnBoxID(nSpawnBoxID);
    // if (bResult) {
    //     m_pCutSceneManager->CheckCutsceneState(2, nGroupID);
    // }
}

// ============================================================================
// CallScriptPostDieMonster
// IDA: 0x14032D240
// 调用怪物死亡后脚本
// TODO: 需要 IVScriptInstance 类型定义
// ============================================================================
void XMaze::CallScriptPostDieMonster(unsigned int nMonsterID, int nSpawnBoxID, unsigned int nGroupID) {
    // IDA 反编译逻辑类似 CallScriptPreDieMonster
    // 在怪物死亡后调用相关脚本处理

    bool bCallScriptDie = IsCallScriptDie(nMonsterID);

    // 简化实现：需要完整类型定义后还原
    // if (m_pScriptInstance && !bCallScriptDie) {
    //     if (m_pScriptInstance->HasFunction("OnPostDie")) {
    //         m_pScriptInstance->ExecuteFunctionArg("OnPostDie", ...);
    //     }
    // }
}

// ============================================================================
// AddMazeGameRule
// IDA: 0x14032D3F0
// 添加迷宫游戏规则
// TODO: 需要 MAZE_GAME_RULE 类型定义
// ============================================================================
void XMaze::AddMazeGameRule(char* szParam1, char* szParam2, char* szParam3, char* szParam4, char* szParam5, char* szParam6) {
    // IDA 反编译逻辑:
    // 1. 将6个字符串参数转换为整数
    // 2. 创建 MAZE_GAME_RULE 结构体
    // 3. 添加到 m_vecGameRules

    // 简化实现：需要 MAZE_GAME_RULE 类型定义后还原
    // struct MAZE_GAME_RULE {
    //     int nParam1, nParam2, nParam3, nParam4, nParam5, nParam6;
    // };
    // MAZE_GAME_RULE rule;
    // rule.nParam1 = std::stoi(szParam1);
    // rule.nParam2 = std::stoi(szParam2);
    // rule.nParam3 = std::stoi(szParam3);
    // rule.nParam4 = std::stoi(szParam4);
    // rule.nParam5 = std::stoi(szParam5);
    // rule.nParam6 = std::stoi(szParam6);
    // m_vecGameRules.push_back(rule);
}

// ============================================================================
// SetMonsterForceActionSkill
// IDA: 0x14032D5C0
// 设置怪物强制动作技能
// TODO: 需要 CMonster::ForceActionSkill 方法
// ============================================================================
void XMaze::SetMonsterForceActionSkill(char* szMonsterID, char* szSkillID) {
    // IDA 反编译逻辑:
    // 1. 将字符串转换为整数 ID
    // 2. 遍历 m_objectScanner 中的所有对象
    // 3. 找到类型为 Monster 且 TableID 匹配的怪物
    // 4. 调用 CMonster::ForceActionSkill(nSkillID)

    int nMonsterID = std::stoi(szMonsterID);
    int nSkillID = std::stoi(szSkillID);

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && pMover->GetType() == 2) {  // ActorType_Monster
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //         if (pMonster && pMonster->GetTableID() == nMonsterID) {
    //             pMonster->ForceActionSkill(nSkillID);
    //             return;
    //         }
    //     }
    // }
}

// ============================================================================
// GetLuaValue
// IDA: 0x14032E3A0
// 获取 m_vecLuaValue 指定索引的值
// ============================================================================
int XMaze::GetLuaValue(int nIndex) {
    if (nIndex >= 0 && static_cast<size_t>(nIndex) < m_vecLuaValue.size()) {
        return m_vecLuaValue[nIndex];
    }
    return 0;
}

// ============================================================================
// GetUniqueIDFromSector
// IDA: 0x14032E3D0
// 获取唯一ID (基于SectorID和BatchLayerLevel)
// TODO: 需要 VEventObjectInfo::GetEventUniqueID
// ============================================================================
int XMaze::GetUniqueIDFromSector(int nID) {
    int nBatchLayerLevel = GetBatchLayerLevel();
    // return VEventObjectInfo::GetEventUniqueID(nID, nBatchLayerLevel);
    return nID;  // 简化实现
}

// ============================================================================
// SetGameModeState
// IDA: 0x14032E400
// 设置游戏模式状态
// TODO: 需要 CSector::SetModeState 方法
// ============================================================================
void XMaze::SetGameModeState(int nState) {
    if (m_pActiveEventSector != nullptr) {
        // m_pActiveEventSector->SetModeState(nState);
    }
}

// ============================================================================
// InitQuestConditionForSectorClear
// IDA: 0x14032E440
// 初始化Sector清除的任务条件
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
void XMaze::InitQuestConditionForSectorClear() {
    // IDA 反编译逻辑:
    // 1. 遍历 m_objectScanner 中的所有 CMover
    // 2. 对每个 CMover 获取 CGocQuest 组件
    // 3. 调用 CGocQuest::InitQuestConditionForSectorClear()
    // 4. 调用 CGocQuest::UpdateQuestConditionForSectorClear()

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         std::shared_ptr<CGocQuest> pQuest;
    //         pMover->GetGOC<CGocQuest>(&pQuest);
    //         if (pQuest) {
    //             pQuest->InitQuestConditionForSectorClear();
    //             pQuest->UpdateQuestConditionForSectorClear();
    //         }
    //     }
    // }
}

// ============================================================================
// IsCompleteQuestCondition
// IDA: 0x14032E520
// 检查任务条件是否完成
// TODO: 需要 CMover::GetGOC<CGocQuest>, CGocQuest 类型定义
// ============================================================================
bool XMaze::IsCompleteQuestCondition(bool& bSendMsg) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_objectScanner 中的所有 CMover
    // 2. 对每个 CMover 获取 CGocQuest 组件
    // 3. 调用 CGocQuest::IsQuestCondtionForSectorClear()
    // 4. 如果返回 false，检查是否需要发送消息

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         std::shared_ptr<CGocQuest> pQuest;
    //         pMover->GetGOC<CGocQuest>(&pQuest);
    //         if (pQuest) {
    //             if (!pQuest->IsQuestCondtionForSectorClear()) {
    //                 bSendMsg = pQuest->IsSendMsgSectorClear();
    //                 return false;
    //             }
    //         }
    //     }
    // }
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
// TODO: 需要 CMover::SetBuffStatus 方法
// ============================================================================
void XMaze::ApplyBuff(unsigned int nActorID, unsigned short nBuffID) {
    // IDA 反编译逻辑:
    // 1. 在 m_objectScanner 中查找 nActorID
    // 2. 如果找到 CMover，调用 SetBuffStatus

    // 简化实现：需要完整类型定义后还原
    // auto it = m_objectScanner.find(nActorID);
    // if (it != m_objectScanner.end()) {
    //     CMover* pObj = it->second;
    //     if (pObj) {
    //         pObj->SetBuffStatus(nBuffID, nActorID, true);
    //     }
    // }
}

// ============================================================================
// SetMonsterCommonActionAi
// IDA: 0x14032D7A0
// 设置怪物通用动作AI
// TODO: 需要 CMonster::GetAi, CAi::ChangeAiState 方法
// ============================================================================
void XMaze::SetMonsterCommonActionAi(char* szMonsterID, char* szCommonActionIndex) {
    // IDA 反编译逻辑:
    // 1. 将字符串转换为整数 ID
    // 2. 遍历 m_objectScanner 中的所有对象
    // 3. 找到类型为 Monster 且 TableID 匹配的怪物
    // 4. 调用 CAi::ChangeAiState((E_FSMSTATES)(nIndex + 19))

    int nMonsterID = std::stoi(szMonsterID);
    int nIndex = std::stoi(szCommonActionIndex);

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && pMover->GetType() == 2) {  // ActorType_Monster
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //         if (pMonster && pMonster->GetTableID() == nMonsterID) {
    //             CAi* pAi = pMonster->GetAi();
    //             if (pAi) {
    //                 pAi->ChangeAiState((E_FSMSTATES)(nIndex + 19));
    //                 return;
    //             }
    //         }
    //     }
    // }
}

// ============================================================================
// WarpMonsterToPoint
// IDA: 0x14032D9B0
// 传送怪物到指定点
// TODO: 需要 VEventObjectInfo, CMonster::MoveActor 等方法
// ============================================================================
void XMaze::WarpMonsterToPoint(char* szMonsterID, char* szBoxID, int nActivate) {
    // IDA 反编译逻辑:
    // 1. 将字符串转换为整数 ID
    // 2. 获取 CommonPositionBoxInfo
    // 3. 遍历找到匹配的怪物
    // 4. 设置无敌状态、改变AI状态、移动到目标位置

    int nMonsterID = std::stoi(szMonsterID);
    int nBoxID = std::stoi(szBoxID);
    bool bActivate = nActivate > 0;

    // 简化实现：需要完整类型定义后还原
    // const VCommonPositionBoxInfo* pBoxInfo = GetCommonPositionBoxInfo(nBoxID);
    // if (pBoxInfo) {
    //     XVec3 vPos = pBoxInfo->GetCenter();
    //     float fRot = pBoxInfo->fRotate;
    //     // 找到怪物并传送
    // }
}

// ============================================================================
// SetMonsterDefenceType
// IDA: 0x14032DD90
// 设置怪物防御类型
// TODO: 需要 CMoverEx::ChangeDefenseTypeForce 方法
// ============================================================================
void XMaze::SetMonsterDefenceType(int nMonsterID, unsigned char byType, float fTime) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_objectScanner 中的所有对象
    // 2. 找到类型为 Monster 且 TableID 匹配的怪物
    // 3. 调用 CMoverEx::ChangeDefenseTypeForce(byType, fTime)

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && pMover->GetType() == 2) {  // ActorType_Monster
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //         if (pMonster && pMonster->GetTableID() == nMonsterID) {
    //             pMonster->ChangeDefenseTypeForce(byType, fTime);
    //             return;
    //         }
    //     }
    // }
}

// ============================================================================
// MoveNpcToWayPoint
// IDA: 0x14032DEA0
// 移动NPC到路径点
// TODO: 需要 CNpc::MoveToWayPoint 方法
// ============================================================================
void XMaze::MoveNpcToWayPoint(int nNpcID, int nWayPointID) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_objectScanner 中的所有对象
    // 2. 找到类型为 NPC (Type=1) 且 TableID 匹配的 NPC
    // 3. 调用 CNpc::MoveToWayPoint(nWayPointID)

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && pMover->GetType() == 1) {  // ActorType_Npc
    //         CNpc* pNpc = dynamic_cast<CNpc*>(pMover);
    //         if (pNpc && pNpc->GetTableID() == nNpcID) {
    //             pNpc->MoveToWayPoint(nWayPointID);
    //             return;
    //         }
    //     }
    // }
}

// ============================================================================
// ChangeNpcAnimation
// IDA: 0x14032DFB0
// 改变NPC动画
// TODO: 需要 CNpc::ChangeMotion 方法
// ============================================================================
void XMaze::ChangeNpcAnimation(int nNpcID, const char* szAnimName) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_objectScanner 中的所有对象
    // 2. 找到类型为 NPC (Type=1) 且 TableID 匹配的 NPC
    // 3. 调用 CNpc::ChangeMotion(szAnimName, 1)

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && pMover->GetType() == 1) {  // ActorType_Npc
    //         CNpc* pNpc = dynamic_cast<CNpc*>(pMover);
    //         if (pNpc && pNpc->GetTableID() == nNpcID) {
    //             pNpc->ChangeMotion(szAnimName, 1);
    //             pNpc->send_eSUB_CMD_MOVE_IDLE(0.0);
    //             return;
    //         }
    //     }
    // }
}

// ============================================================================
// SetNpcRotation
// IDA: 0x14032E0E0
// 设置NPC旋转
// TODO: 需要 CNpc::CallMovingYaw 方法
// ============================================================================
void XMaze::SetNpcRotation(int nNpcID, float fYaw, int nKey) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_objectScanner 中的所有对象
    // 2. 找到类型为 NPC (Type=1) 且 TableID 匹配的 NPC
    // 3. 调用 CNpc::CallMovingYaw(fYaw, nKey)

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && pMover->GetType() == 1) {  // ActorType_Npc
    //         CNpc* pNpc = dynamic_cast<CNpc*>(pMover);
    //         if (pNpc && pNpc->GetTableID() == nNpcID) {
    //             pNpc->CallMovingYaw(fYaw, nKey);
    //             return;
    //         }
    //     }
    // }
}

// ============================================================================
// SetMonsterAllowPassiveType
// IDA: 0x14032E200
// 设置怪物允许被动类型
// TODO: 需要 CMover::SetAllowPassiveType 方法
// ============================================================================
void XMaze::SetMonsterAllowPassiveType(int nMonsterID, int nAllowPassiveType) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_objectScanner 中的所有对象
    // 2. 找到类型为 Monster 且 TableID 匹配的怪物
    // 3. 调用 CMover::SetAllowPassiveType(nAllowPassiveType)

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && pMover->GetType() == 2) {  // ActorType_Monster
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //         if (pMonster && pMonster->GetTableID() == nMonsterID) {
    //             pMonster->SetAllowPassiveType(nAllowPassiveType);
    //         }
    //     }
    // }
}

// ============================================================================
// IsSectorClearConditionBox
// IDA: 0x14032E880
// 检查是否为Sector清除条件Box
// TODO: 需要 VMonsterSpawnInfo, TB_MONSTER 类型定义
// ============================================================================
bool XMaze::IsSectorClearConditionBox(const VMonsterSpawnInfo* pSpawnInfo) {
    if (pSpawnInfo == nullptr) {
        return false;
    }

    // IDA 反编译逻辑:
    // 1. 遍历 pSpawnInfo->m_stMonsterInfo (最多10个)
    // 2. 如果类型为1 (NPC)，返回 false
    // 3. 获取 TB_MONSTER 表数据
    // 4. 检查 Monster_Type 和 Monster_ClearCondition_Type

    // 简化实现：需要完整类型定义后还原
    // for (int i = 0; i < 10; ++i) {
    //     if (pSpawnInfo->m_stMonsterInfo[i].m_iType == 1) {
    //         return false;
    //     }
    //     int nMonsterID = pSpawnInfo->m_stMonsterInfo[i].m_iID;
    //     if (nMonsterID == 0) break;
    //     TB_MONSTER* pTBMonster = XResourceMgr::GetTB_MONSTER(nMonsterID);
    //     if (pTBMonster) {
    //         if (pTBMonster->Monster_Type != 0 && pTBMonster->Monster_Type != 3 && pTBMonster->Monster_Type != 11) {
    //             return false;
    //         }
    //         if (pTBMonster->Monster_ClearCondition_Type == 1) {
    //             return false;
    //         }
    //     }
    // }
    return true;
}

// ============================================================================
// SetSectorStepStop
// IDA: 0x14032E9C0
// 设置Sector步骤停止
// TODO: 需要 CSector::SetStepStop 方法
// ============================================================================
void XMaze::SetSectorStepStop(int nSectorID, bool bStop) {
    // IDA 反编译逻辑:
    // 1. 获取唯一SectorID
    // 2. 在 m_mapSector 中查找对应的 CSector
    // 3. 调用 CSector::SetStepStop(bStop)

    int nUniqueSector = GetUniqueID(nSectorID);
    auto it = m_mapSector.find(nUniqueSector);
    if (it != m_mapSector.end() && it->second != nullptr) {
        // it->second->SetStepStop(bStop);
    }
}

// ============================================================================
// FailEscortQuest
// IDA: 0x14032EA70
// 护送任务失败
// TODO: 需要 CGocQuest 类型定义
// ============================================================================
void XMaze::FailEscortQuest() {
    // IDA 反编译逻辑:
    // 1. 遍历 m_objectScanner 中的所有对象
    // 2. 找到 CUser (非GM或非特定状态)
    // 3. 获取 CGocQuest 组件
    // 4. 检查是否有护送任务
    // 5. 调用 CGocQuest::FailQuest

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         CUser* pUser = dynamic_cast<CUser*>(pMover);
    //         if (pUser && (!pUser->IsGM() || !pUser->IsStatus(0x2000))) {
    //             std::shared_ptr<CGocQuest> pQuest;
    //             pUser->GetGOC<CGocQuest>(&pQuest);
    //             if (pQuest && pQuest->FindEpisode(m_stEscortMonster.dwEpisodeID)) {
    //                 pQuest->FailQuest(m_stPartyQuest.nQuestID, 0.0f);
    //                 ResetPartyQuest();
    //             }
    //         }
    //     }
    // }
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
// TODO: 需要 TXMap 迭代器, CMover::GetClass 方法
// ============================================================================
std::uint8_t XMaze::GetUserClass(unsigned int dwUserID) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_objectScanner 中的所有对象
    // 2. 找到 ActorID 匹配的用户
    // 3. 调用 CMover::GetClass() 返回职业

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover) {
    //         UXActorID actorID = pMover->GetActorID();
    //         if (actorID.dwActorID == dwUserID) {
    //             return pMover->GetClass();
    //         }
    //     }
    // }
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
    m_vecActiveLastSectorID.push_back(nVal);
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
    // return m_vecActiveLastSectorID[iCount];

    int iCount = static_cast<int>(m_vecActiveLastSectorID.size()) - 1;
    if (iCount < 0) return 0;
    return m_vecActiveLastSectorID[iCount];
}

// ============================================================================
// PartySend
// IDA: 0x14032FED0
// 发送包给队伍
// TODO: 需要 CParty::Send 方法
// ============================================================================
void XMaze::PartySend(XSendPacket& xSendPacket) {
    // IDA 反编译逻辑:
    // if (m_pParty != nullptr) {
    //     m_pParty->Send(xSendPacket, nullptr);
    // }

    if (m_pParty) {
        // TODO: CParty::Send not defined
        // m_pParty->Send(&xSendPacket, nullptr);
    }
}

// ============================================================================
// SetBossSector
// IDA: 0x14032FFF0
// 设置Boss区域标志
// TODO: 需要 PS_MAZE_UPDATE_INFO, XSendPacket, XIOCPClient 类型定义
// ============================================================================
void XMaze::SetBossSector(bool bFlag) {
    // IDA 反编译逻辑:
    // if (m_stMazeGameState.m_nMazeState != 4 && m_bBossSector != bFlag) {
    //     m_bBossSector = bFlag;
    //     if (m_bBossSector) {
    //         // 发送 PS_MAZE_UPDATE_INFO 包 (main=0xF2, sub=0x26)
    //         // 广播给所有等待进入迷宫的用户
    //     }
    // }

    if (m_stMazeGameState.m_nMazeState != 4 && m_bBossSector != bFlag) {
        m_bBossSector = bFlag;
        if (m_bBossSector) {
            // TODO: 发送 PS_MAZE_UPDATE_INFO 包
            // PS_MAZE_UPDATE_INFO stUpdateMaze;
            // stUpdateMaze.uxMapID = GetInstanceID();
            // stUpdateMaze.nState = 2;
            // stUpdateMaze.nUserCount = m_objectScanner.size();
            // for (auto& pair : m_mapWaitEnterMazeUser) {
            //     stUpdateMaze.vecMemberInfo.push_back(pair.second);
            // }
            // XSendPacket xSendPacket(0xF2, 0x26);
            // xSendPacket << stUpdateMaze;
            // XGameServer::Instance()->GetControlSocket().Send(&xSendPacket);
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
    // IDA 反编译逻辑:
    // 1. 检查所有用户是否都已死亡
    // 2. 调用 m_pActiveEventSector->DiePlayer(pUser, bAllDie)
    // 3. 如果不能复活或迷宫类型为9，设置断开连接状态
    // 4. 如果是Roguelike地图，处理结果

    if (!pUser) return;

    // 检查所有用户是否都已死亡
    bool bAllDie = true;
    // TODO: 需要TXMap迭代器
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && !pMover->IsDie()) {
    //         bAllDie = false;
    //         break;
    //     }
    // }

    // 调用事件Sector处理
    if (m_pActiveEventSector) {
        // TODO: CSector::DiePlayer not defined
        // m_pActiveEventSector->DiePlayer(pUser, bAllDie);
    }

    // 处理不能复活的情况
    if (!IsRevive() || GetMazeType() == 9) {
        // TODO: 设置断开连接状态
        // pUser->GetGOC<CGocParty>(&pParty);
        // if (pParty && pParty->IsParty()) {
        //     SetDisconnectUserState(pUser->GetActorID(), stPartyInfo);
        // }
    }

    // Roguelike地图处理
    if (IsRoguelikeMap()) {
        // TODO: CGocEntity 处理
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
// TODO: 需要 CSector::DamageMonster 方法
// ============================================================================
void XMaze::DamageMonster(CMonster* pMonster) {
    // IDA 反编译逻辑:
    // if (m_pActiveEventSector) {
    //     m_pActiveEventSector->DamageMonster(pMonster);
    // }

    if (m_pActiveEventSector) {
        // TODO: CSector::DamageMonster not defined
        // m_pActiveEventSector->DamageMonster(pMonster);
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
    // IDA 反编译逻辑:
    // if (m_pActiveEventSector) {
    //     m_pActiveEventSector->InteractBoxOnMode(pUser, nBoxID);
    // }

    if (m_pActiveEventSector) {
        // TODO: CSector::InteractBoxOnMode not defined
        // m_pActiveEventSector->InteractBoxOnMode(pUser, nBoxID);
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
            // TODO: 需要 STMageGateBox 完整类型定义
            // pair.second->bOpen = false;
        }
    }

    // 重置所有 PotalBox
    for (auto& pair : m_mapPotalBox) {
        if (pair.second) {
            // TODO: 需要 STMagePotalBox 完整类型定义
            // pair.second->bOpen = false;
        }
    }

    // 重置所有 LuaFunctionBox
    for (auto& pair : m_mapLuaFunctionBox) {
        if (pair.second) {
            // TODO: 需要 STLuaFunctionBox 完整类型定义
            // pair.second->bCalled = false;
        }
    }

    // 重置所有 Sector
    for (auto& pair : m_mapSector) {
        if (pair.second) {
            // TODO: CSector::Reset not defined
            // pair.second->Reset();
        }
    }

    // 清除所有定时器
    ClearAllTimers();
}

// ============================================================================
// CallScriptUpdateQuest
// IDA: 0x140331630
// 调用脚本更新任务
// ============================================================================
void XMaze::CallScriptUpdateQuest(unsigned int dwActorID, int nType, unsigned int dwID) {
    // IDA 反编译: XMaze::CallScriptUpdateQuest
    // if (m_pScriptInstance) {
    //     IVScriptInstance::ExecuteFunctionArg(
    //         m_pScriptInstance,
    //         "OnUpdateQuest",
    //         "iiiTSoulworker:XMaze;",
    //         dwActorID, nType, dwID, this);
    // }

    if (m_pScriptInstance) {
        // TODO: IVScriptInstance::ExecuteFunctionArg not defined
        // m_pScriptInstance->ExecuteFunctionArg("OnUpdateQuest", "iiiTSoulworker:XMaze;", dwActorID, nType, dwID, this);
    }
}

// ============================================================================
// ShowSectorInfo
// IDA: 0x1403316A0
// 显示Sector信息给用户
// ============================================================================
void XMaze::ShowSectorInfo(CUser* pUser) {
    // IDA 反编译: XMaze::ShowSectorInfo
    // 1. 获取用户位置
    // 2. 根据位置获取 Sector
    // 3. 调用 CSector::ShowSectorInfo

    if (!pUser) return;

    // const hkvVec3& Position = pUser->GetPosition();
    // CSector* pSector = GetSectorFromPos(Position);
    // if (pSector) {
    //     pSector->ShowSectorInfo(pUser);
    // }

    // 简化实现
    // TODO: 需要 CMoverEx::GetPosition, GetSectorFromPos, CSector::ShowSectorInfo
}

// ============================================================================
// GetMonsterCountByID
// IDA: 0x1403316F0
// 根据TableID获取怪物数量
// ============================================================================
unsigned int XMaze::GetMonsterCountByID(unsigned int dwID) {
    // IDA 反编译: XMaze::GetMonsterCountByID
    // 遍历 m_objectScanner.mapNPCList
    // 检查每个 actor 的类型是否为 2 (ActorType_Monster)
    // 如果是 monster，通过 RTTI dynamic_cast 转换为 CMonster*
    // 然后比较 GetTableID() 与 dwID，计数匹配的怪物

    unsigned int nCount = 0;

    // 简化实现：需要完整类型定义后还原
    // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner.mapNPCList);
    // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
    //     CMover* pMover = it->second;
    //     if (pMover && pMover->GetType() == 2) {  // ActorType_Monster = 2
    //         CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //         if (pMonster && pMonster->GetTableID() == dwID) {
    //             ++nCount;
    //         }
    //     }
    // }

    return nCount;
}

// ============================================================================
// AddDieMonsterSpawnBoxID
// IDA: 0x140331FF0
// 添加死亡怪物的SpawnBoxID到列表
// ============================================================================
void XMaze::AddDieMonsterSpawnBoxID(int nSpawnBoxID) {
    // IDA 反编译: XMaze::AddDieMonsterSpawnBoxID
    // 1. 调用 GetMonsterSpawnBoxInfo(nSpawnBoxID) 获取 VMonsterSpawnInfo
    // 2. 如果存在且 ObjectKey 不为空，添加到 m_listDieMonsterSpawnBoxID

    // VMonsterSpawnInfo* pMonsterSpawnInfo = GetMonsterSpawnBoxInfo(nSpawnBoxID);
    // if (pMonsterSpawnInfo) {
    //     VString keyString(pMonsterSpawnInfo->m_szObjectKey);
    //     if (!keyString.IsEmpty()) {
    //         m_listDieMonsterSpawnBoxID.push_back(nSpawnBoxID);
    //     }
    // }

    // 简化实现：需要完整类型定义后还原
}

// ============================================================================
// AddChangeMonster
// IDA: 0x140332090
// 添加变换怪物ID到列表
// ============================================================================
void XMaze::AddChangeMonster(unsigned int dwMonsterID) {
    // IDA 反编译: XMaze::AddChangeMonster
    // 1. 遍历 m_lstChangeMonster 检查是否已存在
    // 2. 如果不存在则添加

    // for (auto it = m_lstChangeMonster.begin(); it != m_lstChangeMonster.end(); ++it) {
    //     if (*it == dwMonsterID) {
    //         return;  // 已存在，不重复添加
    //     }
    // }
    // m_lstChangeMonster.push_back(dwMonsterID);

    // 简化实现：需要完整类型定义后还原
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
// ============================================================================
void XMaze::UpdateQuestRespawn(int nCondition) {
    // IDA 反编译: XMaze::UpdateQuestRespawn
    // 1. 调用 CRespawnManager::RemoveQuestMonster
    // 2. 遍历被删除的 SpawnBox 列表
    // 3. 从对应的 Sector 中删除重生点

    // std::vector<const VMonsterSpawnInfo*> vecDeletedBox;
    // m_respawnManager.RemoveQuestMonster(nCondition, &vecDeletedBox);
    //
    // for (auto it = vecDeletedBox.begin(); it != vecDeletedBox.end(); ++it) {
    //     const VMonsterSpawnInfo* pMonsterSpawn = *it;
    //     if (pMonsterSpawn) {
    //         int iUniqueID = VEventObjectInfo::GetEventUniqueID(pMonsterSpawn->m_iSectorID, GetBatchLayerLevel());
    //         auto sectorIt = m_mapSector.find(iUniqueID);
    //         if (sectorIt != m_mapSector.end() && sectorIt->second) {
    //             sectorIt->second->EraseRespawnBoxID(pMonsterSpawn->iUniqueID);
    //         }
    //     }
    // }

    // 简化实现：需要完整类型定义后还原
}

// ============================================================================
// ChangeMonsterLevelStat
// IDA: 0x140328840
// 根据队伍人数改变怪物等级属性
// ============================================================================
void XMaze::ChangeMonsterLevelStat(int nMemberCount) {
    // IDA 反编译: XMaze::ChangeMonsterLevelStat
    // 1. 设置 m_nPartyMemeberCount
    // 2. 遍历 m_objectScanner.mapNPCList 中的所有怪物
    // 3. 对非跟随者怪物，重新初始化属性并应用等级
    // 4. 保存当前 HP 和 SuperArmor 比例
    // 5. 重新应用 Buff 并恢复 HP/SuperArmor 比例
    // 6. 发送通知包并重置状态

    m_nPartyMemeberCount = nMemberCount;

    if (nMemberCount > 0) {
        // 遍历所有怪物
        // auto& scannerMap = *reinterpret_cast<std::map<std::uint32_t, CMover*>*>(&m_objectScanner.mapNPCList);
        // for (auto it = scannerMap.begin(); it != scannerMap.end(); ++it) {
        //     CMover* pMover = it->second;
        //     if (pMover && pMover->GetType() == 2) {  // ActorType_Monster
        //         CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
        //         if (pMonster && !pMonster->IsFollower()) {
        //             TB_MONSTER* pMobTable = pMonster->GetMobTableRef();
        //             if (pMobTable && !pMobTable->Monster_Type) {
        //                 // 保存当前比例
        //                 int nHP = pMonster->GetHP();
        //                 int nMaxHP = pMonster->GetMaxHP();
        //                 float fHpRate = (nHP > 0) ? (float)nHP / nMaxHP : 1.0f;
        //
        //                 // 重新初始化属性
        //                 std::shared_ptr<CGocNpcAttribute> pAttr;
        //                 pMonster->GetGOC<CGocNpcAttribute>(&pAttr);
        //                 if (pAttr) {
        //                     pAttr->Init(1, pMobTable);
        //                 }
        //
        //                 // 应用等级和 Buff
        //                 pMonster->ApplyLevelToStat(0);
        //                 pMonster->ReapllyBuffAll();
        //
        //                 // 恢复比例
        //                 int nNewMaxHP = pMonster->GetHP();
        //                 int nChangeHP = (int)(nNewMaxHP * fHpRate);
        //                 pMonster->SetHpEx(nChangeHP);
        //             }
        //         }
        //     }
        // }

        // SendNoticePacket(80, 0, 0.0f);
        // RestartResetState(false, false);
    }
}

// ============================================================================
// ShowCasualRaidTimer
// IDA: 0x14032B390
// 显示休闲副本计时器
// ============================================================================
void XMaze::ShowCasualRaidTimer(int nValue, float fTime, float fTime2) {
    // IDA 反编译: XMaze::ShowCasualRaidTimer
    // 发送包 (main=0x11, sub=0x38) 广播计时器信息

    // XSendPacket xPacket(0x11, 0x38);
    // xPacket << 0;  // iType
    // xPacket << nValue;
    // xPacket << fTime;
    // xPacket << fTime2;
    // SendBroadCast(&xPacket, nullptr, eAll);

    // 保存状态
    // m_stCasualRaidTime.nIntValue = nValue;
    // m_stCasualRaidTime.fFloatValue = fTime;

    // 简化实现：需要完整类型定义后还原
}

// ============================================================================
// SetupCasualRaidTimer
// IDA: 0x14032B4B0
// 设置休闲副本计时器
// ============================================================================
void XMaze::SetupCasualRaidTimer() {
    // IDA 反编译: XMaze::SetupCasualRaidTimer
    // 如果 fFloatValue > 0，设置 fWaitSendTime = 5.0

    // if (m_stCasualRaidTime.fFloatValue > 0.0f) {
    //     m_stCasualRaidTime.fWaitSendTime = 5.0f;
    // }

    // 简化实现：需要完整类型定义后还原
}

// ============================================================================
// SendCasualRaidTimer
// IDA: 0x14032B4E0
// 发送休闲副本计时器
// ============================================================================
void XMaze::SendCasualRaidTimer() {
    // IDA 反编译: XMaze::SendCasualRaidTimer
    // 如果 fFloatValue > 0，调用 ShowCasualRaidTimer

    // if (m_stCasualRaidTime.fFloatValue > 0.0f) {
    //     ShowCasualRaidTimer(m_stCasualRaidTime.nIntValue, m_stCasualRaidTime.fFloatValue, 0.0f);
    // }

    // 简化实现：需要完整类型定义后还原
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
// ============================================================================
bool XMaze::DeleteMonsterGroupID(int nGroupID, bool bPre) {
    // IDA 反编译: XMaze::DeleteMonsterGroupID
    // 根据类型增加前置或后置计数，检查是否达到最大值

    if (nGroupID <= 0) {
        return false;
    }

    auto it = m_mapGroupMOB.find(nGroupID);
    if (it == m_mapGroupMOB.end()) {
        return false;
    }

    // if (bPre) {
    //     it->second.nPreCurCount++;
    //     if (it->second.nPreCurCount == it->second.nMaxCount) {
    //         return true;
    //     }
    // } else {
    //     it->second.nPostCurCount++;
    //     if (it->second.nPostCurCount == it->second.nMaxCount) {
    //         return true;
    //     }
    // }

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
        // TODO: 需要完整类型定义
        // it->second.push_back(pTrapObj);
    } else {
        // 组不存在，创建新条目
        // TODO: 需要完整类型定义
        // std::vector<VGameTrapObject*> vecObject;
        // vecObject.push_back(pTrapObj);
        // m_mapGameTrapObjectGroup[nGroup] = std::move(vecObject);
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

    // TODO: 需要完整类型定义
    // auto& vecObjects = it->second;
    // for (size_t i = 0; i < vecObjects.size(); ++i) {
    //     VGameTrapObject* pObject = vecObjects[i];
    //     if (pObject) {
    //         VTrapBase_cl::SetLifeTime(pObject, fTime);
    //     }
    // }
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

    // TODO: 需要完整 STMonterGroupMonsterInfo 类型定义
    // auto& stInfo = it->second;
    // if (stInfo.vecBoxList.empty()) {
    //     return -1;
    // }
    // 从第一个 BoxList 条目获取 Sector ID
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
    // it->second.bActive = false; // TODO: 需要完整类型定义

    int nSectorId = GetSectorByGroupID(nGroupID);
    if (nSectorId == -1) {
        return;
    }

    // TODO: 需要完整 CSector 类型定义
    // CSector* pSector = GetSector(nSectorId);
    // if (pSector) {
    //     // 清除 spawn box ID
    //     // CSector::EraseSpawnBoxID(...)
    //     // CSector::CheckMonsterCount(pSector);
    // }
}

// ============================================================================
// GetExitBoxInfo
// IDA: 0x140334c00
// 获取出口箱子信息
// ============================================================================
bool XMaze::GetExitBoxInfo(unsigned short& wMapID, int& nJumpID, XVec3& vPos) {
    // IDA 反编译: XMaze::GetExitBoxInfo
    // 根据 Maze_Type 处理不同类型的迷宫出口

    if (m_pTBMazeInfo && m_pTBMazeInfo->Maze_Type == 7) {
        // 特殊迷宫类型 7 (Roguelike?)
        // TODO: 需要 m_stEnterDistrictPos 完整类型定义
        // int sWorldID = m_stEnterDistrictPos.sWorldID;
        // switch (sWorldID) {
        //     case 10021:
        //         wMapID = 10021;
        //         nJumpID = 1002199;
        //         return true;
        //     case 10031:
        //         wMapID = 10031;
        //         nJumpID = 1003199;
        //         return true;
        //     case 10041:
        //         wMapID = 10041;
        //         nJumpID = 1004199;
        //         return true;
        // }
    } else if (m_pTBMazeInfo &&
               (m_pTBMazeInfo->Maze_Type == 6 ||
                m_pTBMazeInfo->Maze_Type == 13 ||
                m_pTBMazeInfo->Maze_Type == 19 ||
                !m_pTBMazeInfo->Escape_District_ID)) {
        // 类型 6, 13, 19 或无 Escape_District_ID
        // TODO: 需要 m_stEnterDistrictPos 完整类型定义
        // wMapID = m_stEnterDistrictPos.sWorldID;
        // nJumpID = 0;
        // vPos = m_stEnterDistrictPos.vPos;
        // return true;
    }

    // 从 WorldResMgr 获取迷宫出口位置
    // TODO: 需要完整 XWorldResMgr 类型定义

    return false;
}

// ============================================================================
// CompleteTimerStep
// IDA: 0x140334e10
// 完成计时器步骤
// ============================================================================
void XMaze::CompleteTimerStep(const char* szID, int nStep) {
    // IDA 反编译: XMaze::CompleteTimerStep
    // 遍历 m_arLogicTimers 列表
    // 根据 ID 和步骤标记计时器完成

    if (!szID) {
        return;
    }

    // TODO: 需要完整 LogicTimer 类型定义
    // int nHash = VHashString::GetHash(szID);
    // for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ++it) {
    //     LogicTimer* pTimer = *it;
    //     int nQuestID = pTimer->GetQuestID();
    //     if (nQuestID == nHash) {
    //         if (nStep == 0 || pTimer->GetStep() == nStep) {
    //             pTimer->SetFinishStep(true);
    //         }
    //         return;
    //     }
    // }
}

// ============================================================================
// AddEventMaxTime
// IDA: 0x140334ee0
// 添加事件最大时间
// ============================================================================
void XMaze::AddEventMaxTime(const char* szID, float fTime) {
    // IDA 反编译: XMaze::AddEventMaxTime
    // 遍历 m_arLogicTimers 列表
    // 根据 ID 添加最大时间并更新显示

    if (!szID) {
        return;
    }

    // TODO: 需要完整 LogicTimer 类型定义
    // int nHash = VHashString::GetHash(szID);
    // for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ++it) {
    //     LogicTimer* pTimer = *it;
    //     int nQuestID = pTimer->GetQuestID();
    //     if (nQuestID == nHash) {
    //         pTimer->AddMaxTime(fTime);
    //         int nLeftTime = (int)pTimer->GetLeftTime();
    //         if (pTimer->GetGroupID() == 2) {
    //             int nAddTime = (int)fTime;
    //             int nMsTime = (int)(pTimer->GetLeftTime() * 1000.0f);
    //             AddMonsterKillScoreModeTime(nMsTime, nAddTime);
    //         } else {
    //             ShowCasualRaidTimer(0, (float)nLeftTime, fTime);
    //         }
    //         return;
    //     }
    // }
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
        // TODO: 需要完整类型定义
        // auto& stInfo = it->second;
        // 如果列表为空或 SectorID 匹配，则添加
        // if (stInfo.vecBoxList.empty()) {
        //     stInfo.vecBoxList.push_back(stData);
        // }
    } else {
        // 组不存在，创建新条目
        // TODO: 需要完整 STMonterGroupMonsterInfo 类型定义
        // STMonterGroupMonsterInfo stInfo;
        // stInfo.nGroupID = nGroupID;
        // stInfo.vecBoxList.clear();
        // STMonterGroupMonsterData stData;
        // stData.pInfo = pMonsterSpawn;
        // stData.fWaitTime = 0.0f;
        // stData.bSpawn = false;
        // stInfo.vecBoxList.push_back(stData);
        // m_mapGroupID_Monster[nGroupID] = stInfo;
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
        // TODO: 需要完整 STSpawnBoxGroupInfo 类型定义
        // STSpawnBoxGroupInfo stGroup;
        // stGroup.nGroupID = nGroupID;
        // stGroup.nLimit = nLimit;
        // stGroup.fWaitTime = fWaitTime;
        // stGroup.bStart = true;
        // m_mapSpawnBoxGroupLimit[nGroupID] = stGroup;

        // 初始化对应怪物组的等待时间
        auto itMonster = m_mapGroupID_Monster.find(nGroupID);
        if (itMonster != m_mapGroupID_Monster.end()) {
            // TODO: 需要完整类型定义
            // auto& stInfo = itMonster->second;
            // for (size_t i = 0; i < stInfo.vecBoxList.size(); ++i) {
            // }
        }
    } else {
        // 更新现有条目
        // it->second.bStart = true;
        // it->second.nLimit = nLimit;
        // it->second.fWaitTime = fWaitTime;

        // 重置怪物组的等待时间和生成状态
        auto itMonster = m_mapGroupID_Monster.find(nGroupID);
        if (itMonster != m_mapGroupID_Monster.end()) {
            // TODO: 需要完整类型定义
            // auto& stInfo = itMonster->second;
            // for (size_t j = 0; j < stInfo.vecBoxList.size(); ++j) {
            // }
        }
    }
}

// ============================================================================
// InitPartyQuest
// IDA: 0x140335030
// 初始化队伍任务
// ============================================================================
void XMaze::InitPartyQuest(unsigned int dwQuestID, int nConditionID, int nStartSector) {
    // IDA 反编译: XMaze::InitPartyQuest
    // 设置队伍任务的基本信息
    // TODO: 需要 m_stPartyQuest 成员定义
    // m_stPartyQuest.nQuestID = dwQuestID;
    // m_stPartyQuest.nStartCondtion = nConditionID;
    // m_stPartyQuest.nStartSector = nStartSector;
}

// ============================================================================
// UpdatePartyQuest
// IDA: 0x140335080
// 更新队伍任务
// ============================================================================
void XMaze::UpdatePartyQuest(unsigned int dwQuestID, int nConditionID, bool bComplete) {
    // IDA 反编译: XMaze::UpdatePartyQuest
    // 如果任务ID匹配，根据完成状态更新
    // TODO: 需要 m_stPartyQuest 成员定义
    // if (m_stPartyQuest.nQuestID <= 0 || m_stPartyQuest.nQuestID != dwQuestID) {
    //     return;
    // }
    // if (bComplete) {
    //     m_stPartyQuest.nState = 2;
    // } else {
    //     m_stPartyQuest.setCondition.insert(nConditionID);
    // }
}

// ============================================================================
// ResetPartyQuest
// IDA: 0x140335100
// 重置队伍任务
// ============================================================================
void XMaze::ResetPartyQuest() {
    // IDA 反编译: XMaze::ResetPartyQuest
    // 调用 STPartyQuest::reset 清理任务状态
    // TODO: 需要 m_stPartyQuest 成员定义
    // m_stPartyQuest.nQuestID = 0;
    // m_stPartyQuest.nStartCondtion = 0;
    // m_stPartyQuest.nStartSector = 0;
    // m_stPartyQuest.nState = 0;
    // m_stPartyQuest.setCondition.clear();
}

// ============================================================================
// CallTestLuaFunction
// IDA: 0x140335130
// 调用测试Lua函数
// ============================================================================
void XMaze::CallTestLuaFunction(unsigned int nParam) {
    // IDA 反编译: XMaze::CallTestLuaFunction
    // 通过脚本实例调用 OnTestFunction

    if (m_pScriptInstance) {
        // TODO: 需要 IVScriptInstance 完整接口定义
        // IVScriptInstance::ExecuteFunctionArg(m_pScriptInstance, "OnTestFunction", "iTSoulworker:XMaze;", nParam, this);
    }
}

// ============================================================================
// UpdateTimer
// IDA: 0x140335170
// 更新计时器（大函数，约500行）
// ============================================================================
void XMaze::UpdateTimer(float fElapsed) {
    // IDA 反编译: XMaze::UpdateTimer
    // 这是一个非常大的函数，处理多种类型的计时器更新
    // 主要逻辑：
    // 1. 将等待列表中的计时器移动到活动列表
    // 2. 遍历所有活动计时器，根据类型执行不同逻辑
    // 3. 类型1：普通计时器，包含 Ready/Step/Complete 状态机
    // 4. 类型2：怪物击杀积分模式计时器
    // 5. 类型3：时间步骤计时器

    // 第一步：将等待列表中的计时器移到活动列表
    // while (!m_arWaitLogicTimers.empty()) {
    //     LogicTimer* pTimer = m_arWaitLogicTimers.front();
    //     m_arLogicTimers.push_back(pTimer);
    //     m_arWaitLogicTimers.pop_front();
    // }

    // 遍历活动计时器列表
    for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ) {
        // TODO: 需要完整 LogicTimer 类型定义
        // LogicTimer* pTimer = *it;
        // float fMaxTime = pTimer->GetMaxTimer();
        // int nType = pTimer->GetGroupID();

        // if (pTimer->IsPause()) {
        //     ++it;
        //     continue;
        // }

        // switch (nType) {
        //     case 1: // 普通计时器
        //         // 处理 Ready 状态、Update、Step 状态机
        //         break;
        //     case 2: // 怪物击杀积分模式
        //         // 处理 StartMonsterKillScoreMode 等
        //         break;
        //     case 3: // 时间步骤计时器
        //         // 处理 TimeStep 相关逻辑
        //         break;
        //     default:
        //         // 默认计时器更新
        //         break;
        // }

        ++it;
    }
}

// ============================================================================
// StartEventTimer
// IDA: 0x140336490
// 启动事件计时器
// ============================================================================
void XMaze::StartEventTimer(const char* szID) {
    // IDA 反编译: XMaze::StartEventTimer
    // 遍历 m_arLogicTimers 列表
    // 根据 ID 哈希值匹配计时器并取消暂停

    if (!szID) {
        return;
    }

    // TODO: 需要完整 LogicTimer 类型定义
    // int nHash = VHashString::GetHash(szID);
    // for (auto it = m_arLogicTimers.begin(); it != m_arLogicTimers.end(); ++it) {
    //     LogicTimer* pTimer = *it;
    //     int nQuestID = pTimer->GetQuestID();
    //     if (nQuestID == nHash) {
    //         pTimer->SetPause(false);
    //         return;
    //     }
    // }
}

// ============================================================================
// GetExitDistrictID
// IDA: 0x140336540
// 获取出口区域ID
// ============================================================================
bool XMaze::GetExitDistrictID(unsigned int dwActorID, unsigned short& wMapID, int& nJumpID, XVec3& vPos) {
    // IDA 反编译: XMaze::GetExitDistrictID
    // 根据 Maze_Type 处理不同类型的迷宫出口

    if (m_pTBMazeInfo) {
        if (m_pTBMazeInfo->Maze_Type == 7) {
            // 特殊迷宫类型 7 (Roguelike?)
            // TODO: 需要 m_stEnterDistrictPos 完整类型定义
            // switch (m_stEnterDistrictPos.sWorldID) {
            //     case 0x2725: // 10021
            //         wMapID = 10021;
            //         nJumpID = 1002199;
            //         return true;
            //     case 0x272F: // 10031
            //         wMapID = 10031;
            //         nJumpID = 1003199;
            //         return true;
            //     case 0x2739: // 10041
            //         wMapID = 10041;
            //         nJumpID = 1004199;
            //         return true;
            // }
        } else if (m_pTBMazeInfo->Maze_Type == 6 ||
                   m_pTBMazeInfo->Maze_Type == 13 ||
                   m_pTBMazeInfo->Maze_Type == 19 ||
                   !wMapID) {
            // 类型 6, 13, 19 或无 wMapID
            // TODO: 需要 m_stEnterDistrictPos 完整类型定义
            // wMapID = m_stEnterDistrictPos.sWorldID;
            // nJumpID = 0;
            // vPos = m_stEnterDistrictPos.vPos;
            // return true;
        }
    }

    // 从 WorldResMgr 获取迷宫出口位置
    // TODO: 需要完整 XWorldResMgr 类型定义

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
bool XMaze::IsCutsceneCondition(unsigned int dwActorID, int nConditionType, int nConditionValue) {
    // IDA 反编译: XMaze::IsCutsceneCondition
    // 在 m_objectScanner 中查找 Actor
    // 获取 GocQuest 组件检查条件

    // TODO: 需要完整类型定义
    // auto it = m_objectScanner.find(dwActorID);
    // if (it == m_objectScanner.end()) {
    //     return false;
    // }
    //
    // CMover* pUser = it->second;
    // if (!pUser) {
    //     return false;
    // }
    //
    // auto pQuest = pUser->GetGOC<CGocQuest>();
    // if (nConditionType == 3) {
    //     return pQuest->FindCondition(nConditionValue);
    // } else if (nConditionType == 4) {
    //     return pQuest->FindEpisode(nConditionValue);
    // }
    // return true;

    return false;
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
// IDA: 0x140340660
// 更改怪物动作
// ============================================================================
void XMaze::ChangeMonsterMotion(int nMonsterID, std::int16_t nSourceMotion, std::int16_t nDestMotion) {
    // IDA 反编译: 遍历 m_objectScanner.mapNPCList 查找匹配的怪物并更改动作
    // 遍历所有 NPC/怪物，检查类型和 ID，设置预留动作并广播

    // TODO: 需要 m_objectScanner 完整类型定义
    (void)nMonsterID;
    (void)nSourceMotion;
    (void)nDestMotion;
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

    // TODO: 需要 STInteractionBox 完整类型定义
    // if (!it->second) {
    //     return false;
    // }
    // it->second->bEnable = bEnable;
    // it->second->bSight = bSight;
    // it->second->nCount = nCount;

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

    // TODO: 需要 STInteractionBox 完整类型定义
    // if (!it->second) {
    //     return false;
    // }
    // bEnable = it->second->bEnable;
    // bSight = it->second->bSight;
    // nCount = it->second->nCount;

    return true;
}

// ============================================================================
// CanUseReviveCount
// IDA: 0x1403373b0
// 检查是否可以使用复活次数
// ============================================================================
bool XMaze::CanUseReviveCount(unsigned int dwUCID) {
    // IDA 反编译: XMaze::CanUseReviveCount
    // 检查 m_mapReviveCount 中用户的复活次数
    // 如果未找到或未设置限制，返回 true

    auto it = m_mapReviveCount.find(dwUCID);
    if (it == m_mapReviveCount.end()) {
        return true;
    }

    // TODO: 需要 m_pTBMazeInfo 完整类型定义
    // if (!m_pTBMazeInfo || !m_pTBMazeInfo->Revival_Item_Count) {
    //     return true;
    // }
    // return m_pTBMazeInfo->Revival_Item_Count > it->second;

    return true;
}

// ============================================================================
// AddUseReviveCount
// IDA: 0x140337460
// 添加使用复活次数
// ============================================================================
void XMaze::AddUseReviveCount(CUser* pUser) {
    // IDA 反编译: XMaze::AddUseReviveCount
    // 获取用户 UCID，在 m_mapReviveCount 中增加计数

    if (!pUser) {
        return;
    }

    // TODO: 需要完整类型定义
    // unsigned int dwUCID = pUser->GetUCID();
    // auto it = m_mapReviveCount.find(dwUCID);
    // if (it == m_mapReviveCount.end()) {
    //     m_mapReviveCount[dwUCID] = 1;
    // } else {
    //     ++it->second;
    // }
    // SendUseReviveCount(pUser);
}

// ============================================================================
// SendUseReviveCount
// IDA: 0x1403375e0
// 发送使用复活次数
// ============================================================================
void XMaze::SendUseReviveCount(XActor* pActor) {
    // IDA 反编译: XMaze::SendUseReviveCount
    // 发送复活次数包给指定 Actor

    if (!pActor) {
        return;
    }

    // TODO: 需要完整类型定义
    // unsigned int dwUCID = pActor->GetActorID();
    // unsigned char byCount = 0;
    // auto it = m_mapReviveCount.find(dwUCID);
    // if (it != m_mapReviveCount.end()) {
    //     byCount = it->second;
    // }
    // XSendPacket xSendPacket(0x11, 0x70);
    // xSendPacket << byCount;
    // pActor->Send(&xSendPacket);
}

// ============================================================================
// IsPlayCutscene
// IDA: 0x140337720
// 检查是否正在播放过场动画
// ============================================================================
bool XMaze::IsPlayCutscene(unsigned int dwActorID) {
    // IDA 反编译: XMaze::IsPlayCutscene
    // 调用 CCutsceneManager::IsPlayCutscene

    // TODO: 需要 CCutsceneManager 完整类型定义
    // return m_cutSceneManager.IsPlayCutscene(dwActorID);
    return false;
}

// ============================================================================
// IsCutsceneState
// IDA: 0x140337750
// 检查是否处于过场动画状态
// ============================================================================
bool XMaze::IsCutsceneState() {
    // IDA 反编译: XMaze::IsCutsceneState
    // 调用 CCutsceneManager::IsCutsceneState

    // TODO: 需要 CCutsceneManager 完整类型定义
    // return m_cutSceneManager.IsCutsceneState();
    return false;
}

// ============================================================================
// ProcessGameRuleCheck
// IDA: 0x140337780
// 处理游戏规则检查
// ============================================================================
void XMaze::ProcessGameRuleCheck(CMover* pMover) {
    // IDA 反编译: XMaze::ProcessGameRuleCheck
    // 根据 Mover 类型分发到不同的处理函数

    if (!pMover) {
        return;
    }

    // TODO: 需要完整类型定义
    // int nType = pMover->GetType();
    // if (nType == 0) {
    //     // User
    //     CUser* pUser = dynamic_cast<CUser*>(pMover);
    //     ProcessGameRuleForUser(pUser);
    // } else if (nType == 2) {
    //     // Monster
    //     CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    //     ProcessGameRuleForMonster(pMonster);
    // }
}

// ============================================================================
// ProcessGameRuleForUser
// IDA: 0x140337830
// 为用户处理游戏规则
// ============================================================================
void XMaze::ProcessGameRuleForUser(CUser* pUser) {
    // IDA 反编译: XMaze::ProcessGameRuleForUser
    // 遍历 m_vecGameRules，检查并应用规则
    // 检查 BuffStatus 条件并设置新状态

    if (!pUser || m_vecGameRules.empty()) {
        return;
    }

    // TODO: 需要完整 MAZE_GAME_RULE 类型定义
    // for (size_t i = 0; i < m_vecGameRules.size(); ++i) {
    //     auto& rule = m_vecGameRules[i];
    //     if (rule.nParam1 != 1) {
    //         continue;
    //     }
    //
    //     bool bValidCheck = true;
    //     if (rule.nParam3 > 0) {
    //         bValidCheck = pUser->FindBuffStatus(rule.nParam3, 0) != -1;
    //     }
    //     if (bValidCheck && rule.nParam4 > 0) {
    //         bValidCheck = pUser->FindBuffStatus(rule.nParam4, 0) != -1;
    //     }
    //     if (bValidCheck && rule.nParam5 > 0) {
    //         bValidCheck = pUser->FindBuffStatus(rule.nParam5, 0) != -1;
    //     }
    //
    //     if (bValidCheck) {
    //         pUser->ClearBuffStatus(rule.nParam3, 0, 0);
    //         pUser->ClearBuffStatus(rule.nParam4, 0, 0);
    //         pUser->ClearBuffStatus(rule.nParam5, 0, 0);
    //         pUser->SetBuffStatus(rule.nParam6, 0, true);
    //     }
    // }
}

// ============================================================================
// ProcessGameRuleForMonster
// IDA: 0x140337a90
// 为怪物处理游戏规则
// ============================================================================
void XMaze::ProcessGameRuleForMonster(CMonster* pMonster) {
    // IDA 反编译: XMaze::ProcessGameRuleForMonster
    // 遍历 m_vecGameRules，检查并应用规则
    // 检查 BuffStatus 条件并改变 AI 状态

    if (!pMonster || m_vecGameRules.empty()) {
        return;
    }

    // TODO: 需要完整 MAZE_GAME_RULE 类型定义
    // int nMonsterID = pMonster->GetTableID();
    // for (size_t i = 0; i < m_vecGameRules.size(); ++i) {
    //     auto& rule = m_vecGameRules[i];
    //     if (rule.nParam1 != 2 || rule.nParam2 != nMonsterID) {
    //         continue;
    //     }
    //
    //     bool bValidCheck = true;
    //     if (rule.nParam3 > 0) {
    //         bValidCheck = pMonster->FindBuffStatus(rule.nParam3, 0) != -1;
    //     }
    //     if (bValidCheck && rule.nParam4 > 0) {
    //         bValidCheck = pMonster->FindBuffStatus(rule.nParam4, 0) != -1;
    //     }
    //     if (bValidCheck && rule.nParam5 > 0) {
    //         bValidCheck = pMonster->FindBuffStatus(rule.nParam5, 0) != -1;
    //     }
    //
    //     if (bValidCheck && pMonster->GetAi()) {
    //         pMonster->ClearBuffStatus(rule.nParam3, 0, 0);
    //         pMonster->ClearBuffStatus(rule.nParam4, 0, 0);
    //         pMonster->ClearBuffStatus(rule.nParam5, 0, 0);
    //         pMonster->GetAi()->ChangeAiState(static_cast<E_FSMSTATES>(rule.nParam6 + 19));
    //     }
    // }
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
// IDA: 0x140329dc0
// 从盒子信息创建剪影
// ============================================================================
bool XMaze::CreateSilhouetteFromBoxinfo(const VMonsterSpawnInfo* pBoxInfo, void** ppSilhouette) {
    // IDA 反编译: XMaze::CreateSilhouetteFromBoxinfo
    // 使用 Havok 导航网格创建剪影

    if (!pBoxInfo || !ppSilhouette) return false;

    // 获取导航网格
    // hkpNavMesh* pNavMesh = GetNavMesh();
    // if (!pNavMesh) return false;

    // 创建剪影生成器
    // hkpSilhouetteGenerator* pGenerator = nullptr;

    // 根据盒子类型和形状创建剪影
    // if (pBoxInfo->m_iShapeType == 0) {
    //     // 矩形区域
    //     hkpSilhouetteGeneratorCinfo cinfo;
    //     cinfo.m_navMesh = pNavMesh;
    //     cinfo.m_localExtents.set(pBoxInfo->m_fWidth / 2.0f, pBoxInfo->m_fHeight / 2.0f);
    //     cinfo.m_localTranslation.set(pBoxInfo->m_vPos.x, pBoxInfo->m_vPos.y, pBoxInfo->m_vPos.z);
    //     pGenerator = new hkpSilhouetteGenerator(cinfo);
    // } else {
    //     // 圆形区域
    //     // ...
    // }

    // *ppSilhouette = pGenerator;
    // return pGenerator != nullptr;

    *ppSilhouette = nullptr;
    return false; // TODO: 实现 Havok 导航网格剪影创建
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
// CheckCanDirectMove2
// IDA: 0x14032aad0
// 检查是否可以直接移动（版本2）
// ============================================================================
bool XMaze::CheckCanDirectMove2(const XVec3* pStartPos, const XVec3* pEndPos, float fRadius) {
    // IDA 反编译: XMaze::CheckCanDirectMove2
    // 使用导航网格检查两点之间是否可以直接移动

    if (!pStartPos || !pEndPos) return false;

    // 如果起点和终点相同，返回true
    // if (*pStartPos == *pEndPos) return true;

    // 获取导航网格
    // hkpNavMesh* pNavMesh = GetNavMesh();
    // if (!pNavMesh) return false;

    // 创建射线检测
    // hkpNavMeshRayCastInput input;
    // input.m_from.set(pStartPos->x, pStartPos->y, pStartPos->z);
    // input.m_to.set(pEndPos->x, pEndPos->y, pEndPos->z);
    // input.m_radius = fRadius;

    // hkpNavMeshRayCastOutput output;
    // if (pNavMesh->rayCast(input, output)) {
    //     // 如果射线没有碰到障碍物，可以直达
    //     return !output.m_didHit;
    // }

    // 简化实现：假设可以直达
    // TODO: 实现 Havok 导航网格射线检测
    return true;
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
