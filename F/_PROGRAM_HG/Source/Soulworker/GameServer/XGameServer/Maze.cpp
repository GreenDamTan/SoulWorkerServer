// Maze.cpp - XMaze 类实现
// 还原自 GameServer.exe IDA 反编译
// 构造函数: 0x140310550
// 析构函数: 0x140310D00

#include "Maze.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/CFSRWLock.h"
#include "Soulworker/GameServer/XCore/XServer/LogHelper.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include <cstdio>
#include <cstring>

// 外部全局变量
extern std::string g_strCurPath_11;

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
    m_strTimeStepFailScript.~basic_string();

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

    // 基类析构会自动调用
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
        xSendPacket.GetParse() << nState;
        SendBroadCast(&xSendPacket, nullptr, eAll);
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

    // 尝试转换为 CMover
    CMover* pMover = dynamic_cast<CMover*>(pActor);
    if (!pMover) {
        return 50001;
    }

    // 设置位置信息
    pMover->SetPosInfo(&vNextPos);
    return 0;
}

// ============================================================================
// CreateNavMesh
// IDA: 0x14031F120
// ============================================================================
bool XMaze::CreateNavMesh(const char* pszFileName) {
    XGameServer* pServer = XGameServer::Instance();
    CFAutoSlimWriteLock autolock(&pServer->m_rwMapLock);

    std::string strPath = g_strCurPath_11 + "/World/Navmesh";
    char szFilePath[272];
    std::sprintf(szFilePath, "%s/%s.hkt", strPath.c_str(), pszFileName);

    // 加载 NavMesh 资源
    // TODO: 需要实现 ThreadLocalData::GetInstance()->m_DohHavokResourceManager.loadNavMesh
    // HavokNavMeshResource* pNavMesh = Instance->m_DohHavokResourceManager.loadNavMesh(szFilePath);

    // 暂时返回 true
    // 完整实现需要 Havok NavMesh 系统
    LogHelper::LogDebug("game.contents", "CreateNavMesh: %s", szFilePath);
    return true;
}

// ============================================================================
// CreateScriptInst
// IDA: 0x14031F2C0
// ============================================================================
bool XMaze::CreateScriptInst(const char* pszFileName) {
    std::string strPath = g_strCurPath_11 + "/Scripts";
    char szFilePath[272];
    std::sprintf(szFilePath, "%s/Server/%s.lua", strPath.c_str(), pszFileName);

    // 创建脚本实例
    // TODO: 需要实现脚本系统
    // ThreadLocalData* Instance = ThreadLocalData::GetInstance();
    // IVScriptManager* v14 = &ThreadLocalData::GetScriptManager(Instance)->IVScriptManager;
    // m_pScriptInstance = v14->CreateScriptInstanceFromFile(szFilePath);

    LogHelper::LogDebug("game.contents", "CreateScriptInst: %s", szFilePath);
    return true;
}

// ============================================================================
// CreateNpc
// IDA: 0x14031A250
// ============================================================================
CNpc* XMaze::CreateNpc(UXMapID uxMazeSerialID, std::uint32_t nSectorID,
                       std::uint32_t nNpcID, XVec3* vPos, float fRot,
                       IXArea::E_SEND_INFO_TYPE eType) {
    XGameServer* pServer = XGameServer::Instance();

    // 检查 NPC 表
    if (!XResourceMgr::GetTB_NPC(&pServer->m_xResourceMgr, nNpcID)) {
        return nullptr;
    }

    // 查找 Sector
    auto it = m_mapSector.find(nSectorID);
    if (it == m_mapSector.end()) {
        return nullptr;
    }

    CSector* pSector = it->second;

    // 创建 NPC
    // TODO: 需要实现 ThreadLocalData::CreateNpc
    // ThreadLocalData* Instance = ThreadLocalData::GetInstance();
    // CNpc* pNpc = Instance->CreateNpc(this, uxMazeSerialID, nSectorID, nNpcID, vPos, fRot, 0);

    CNpc* pNpc = nullptr;  // 临时

    if (pNpc) {
        if (EnterGameObject(pNpc, eType) != 0) {
            DeleteNpc(pNpc);
            return nullptr;
        }

        pNpc->SetSector(pSector);
        pNpc->UpdateSectorID();
        pNpc->SetCollisionEnable(true, false);
    }

    return pNpc;
}

// ============================================================================
// DeleteNpc
// IDA: 0x14031A430
// ============================================================================
void XMaze::DeleteNpc(CNpc* pNpc) {
    if (pNpc) {
        // TODO: 实现 NPC 删除逻辑
        // ExitGameObject(pNpc, eSendInfoTypeNone);
        // ThreadLocalData::DeleteNpc(pNpc);
    }
}

// ============================================================================
// CreateAkashicObject
// IDA: 0x14031A4A0
// ============================================================================
CAkashicObject* XMaze::CreateAkashicObject(UXMapID uxMazeSerialID, std::uint32_t nAkashicID,
                                           XVec3* vPos, float fRot, std::uint32_t dwParentID,
                                           IXArea::E_SEND_INFO_TYPE eType) {
    XGameServer* pServer = XGameServer::Instance();

    // 检查 Akashic 表
    if (!XResourceMgr::GetTB_AKASHIC_RECORDS(&pServer->m_xResourceMgr, nAkashicID)) {
        return nullptr;
    }

    // 创建 Akashic Object
    // TODO: 需要实现 ThreadLocalData::CreateAkashicObject
    // ThreadLocalData* Instance = ThreadLocalData::GetInstance();
    // CAkashicObject* pAkashic = Instance->CreateAkashicObject(this, uxMazeSerialID, nAkashicID, vPos, fRot, dwParentID);

    CAkashicObject* pAkashic = nullptr;  // 临时

    if (pAkashic) {
        if (EnterGameObject(pAkashic, eType) != 0) {
            DeleteAkashicObject(pAkashic);
            return nullptr;
        }

        pAkashic->SetCollisionEnable(false, false);
    }

    return pAkashic;
}

// ============================================================================
// DeleteAkashicObject
// IDA: 0x14031A5E0
// ============================================================================
void XMaze::DeleteAkashicObject(CAkashicObject* pAkashic) {
    if (pAkashic) {
        // TODO: 实现 Akashic Object 删除逻辑
        // ExitGameObject(pAkashic, eSendInfoTypeNone);
        // ThreadLocalData::DeleteAkashicObject(pAkashic);
    }
}

// ============================================================================
// SetParty
// IDA: 0x140315C40
// ============================================================================
void XMaze::SetParty(std::tr1::shared_ptr<CParty> pParty) {
    if (!m_pParty) {
        m_pParty = pParty;

        if (m_pParty) {
            m_pParty->SetMazeID(m_uxMapID);
            m_nPartyUserCount = m_pParty->GetUserCount();
            m_stPartyInfo.byGroupType = 1;  // Party

            int nPartyID = m_pParty->GetPartyID();
            LogHelper::LogDebug("game.contents",
                "<MAZE> SetParty ( PID : %d / Count : %d )",
                nPartyID, static_cast<int>(m_nPartyUserCount));
        }
    }
}

// ============================================================================
// SetForce
// IDA: 0x140315D50
// ============================================================================
void XMaze::SetForce(std::tr1::shared_ptr<CForce> pForce) {
    if (!m_pForce) {
        m_pForce = pForce;

        if (m_pForce) {
            m_pForce->SetMazeID(m_uxMapID);
            m_nPartyUserCount = m_pForce->GetUserCount();
            m_stPartyInfo.byGroupType = 2;  // Force

            int nForceID = m_pForce->GetPartyID();
            LogHelper::LogDebug("game.contents",
                "<MAZE> SetForce ( PID : %d / Count : %d )",
                nForceID, static_cast<int>(m_nPartyUserCount));
        }
    }
}

// ============================================================================
// GetScanner
// 获取扫描器对象中的玩家列表
// ============================================================================
std::map<std::uint32_t, CMover*>* XMaze::GetScanner(XActor* pActor) {
    if (!pActor) {
        return nullptr;
    }

    // TODO: 实现扫描器逻辑
    // 返回 m_objectScanner.mapPlayerList 的适当条目
    return nullptr;
}

// ============================================================================
// GetCurUserCount
// 获取当前用户数量
// ============================================================================
int XMaze::GetCurUserCount() const {
    return static_cast<int>(m_mapWaitEnterMazeUser.size());
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
    // TODO: 实现唯一ID生成逻辑
    return static_cast<std::uint32_t>(nSectorID);
}

// ============================================================================
// SendBroadCast
// 广播消息
// ============================================================================
void XMaze::SendBroadCast(XSendPacket* pPacket, XActor* pExceptActor, E_BROADCAST_TYPE eType) {
    if (!pPacket) {
        return;
    }

    // TODO: 实现广播逻辑
    // 遍历 m_objectScanner 中的所有玩家发送消息
}

// ============================================================================
// 辅助方法存根
// ============================================================================

void XMaze::RunSectorAI(int nSectorID, int nState) {
    // TODO: 需要实现
}

void XMaze::PauseAlltimer(bool bPause) {
    // TODO: 需要实现
}

void XMaze::UpdateClearMazeCondition(int nType, int nValue) {
    // TODO: 需要实现
}

void XMaze::LoadComplete(CUser* pUser) {
    // TODO: 需要实现
}

void XMaze::ChangeMonster(std::uint32_t dwMobID) {
    // TODO: 需要实现
}

void XMaze::CheckFollowMonster() {
    // TODO: 需要实现
}

void XMaze::RefreshUserCountInSector(std::uint32_t dwActorID) {
    // TODO: 需要实现
}

void XMaze::RestartResetState(bool bState1, bool bState2) {
    // TODO: 需要实现
}

int XMaze::FindInvisibleActorCnt() {
    // TODO: 需要实现
    return 0;
}

bool XMaze::CheckSpawnBoxGroupCount(float fElapsed) {
    // TODO: 需要实现
    return false;
}

void XMaze::ExcuteSpawnBox(void* pSpawnBox, E_SEND_INFO_TYPE eType) {
    // TODO: 需要实现
}

void XMaze::UpdateTimer(float fElapsed) {
    // TODO: 需要实现
}

void XMaze::UpdateMazeState() {
    // TODO: 需要实现
}

void XMaze::ProcessEscortCondition(float fElapsed) {
    // TODO: 需要实现
}

void XMaze::UpdateCasualRaidTimer(float fElapsed) {
    // TODO: 需要实现
}

void XMaze::UpdatePortalState() {
    // TODO: 需要实现
}

void XMaze::SyncSpawnedActive() {
    // TODO: 需要实现
}

void XMaze::EnterPartyForceMember(CUser* pUser) {
    // TODO: 需要实现
}

void XMaze::SendSyncHiddenEventInfo(CUser* pUser) {
    // TODO: 需要实现
}

void XMaze::SendOutInfo(XSendPacket* pPacket, XActor* pActor) {
    // TODO: 需要实现
}

void XMaze::GetMazeLayerLevel(int nBitMask, int* pnBaseLevel) {
    // TODO: 需要实现
    if (pnBaseLevel) {
        *pnBaseLevel = 0;
    }
}

bool XMaze::GetSpawnPos(int nSpawnBoxID, XVec3& vPos, float& fRot) {
    // TODO: 需要实现
    return false;
}

// ============================================================================
// Init, Create, Clear, Generate 等大型方法存根
// 这些方法需要完整的实现
// ============================================================================

bool XMaze::Init() {
    // TODO: 需要完整实现 - IDA 0x140311210
    // 这是一个非常大的函数，需要分步骤还原
    return false;
}

bool XMaze::Create(ST_CREATE_MAZE& stCreateMaze) {
    // TODO: 需要完整实现 - IDA 0x140315870
    return false;
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
    // TODO: 需要完整实现 - IDA 0x140317750
}

void XMaze::ExcuteEventSpawn() {
    // TODO: 需要完整实现 - IDA 0x140317A40
}

std::uint16_t XMaze::EnterGameObject(XActor* pActor, IXArea::E_SEND_INFO_TYPE eType) {
    // TODO: 需要完整实现 - IDA 0x140313130
    return 50001;
}

std::uint16_t XMaze::ExitGameObject(XActor* pActor, IXArea::E_SEND_INFO_TYPE eType) {
    // TODO: 需要完整实现 - IDA 0x140313580
    return 50001;
}

bool XMaze::IsAliveMonster(int nTableID) {
    // TODO: 需要完整实现 - IDA 0x140313950
    return false;
}

std::uint16_t XMaze::EnterActor(XActor* pActor) {
    // TODO: 需要完整实现 - IDA 0x140313A60
    return 50001;
}

std::uint16_t XMaze::ExitActor(XActor* pActor) {
    // TODO: 需要完整实现 - IDA 0x1403140D0
    return 50001;
}

CMonster* XMaze::CreateMonster(UXMapID uxMazeSerialID, std::uint32_t nSectorID, int nMonsterID,
                               XVec3* vPos, float fRot, IXArea::E_SEND_INFO_TYPE eType,
                               int nSpawnBoxID, int nGroupID, UXActorID uxParentID) {
    // TODO: 需要完整实现 - IDA 0x140318F20
    return nullptr;
}

void XMaze::DeleteMonster(CMonster* pMonster) {
    // TODO: 需要完整实现 - IDA 0x140319AC0
}

void XMaze::OnUpdate(float fElapsed, float fRealElapsed) {
    // TODO: 需要完整实现 - IDA 0x14031C330
    // 这是一个非常大的函数，需要分步骤还原
}

void XMaze::SendSectorInfos(CUser* pUser) {
    // TODO: 需要完整实现 - IDA 0x14031D920
}

void XMaze::SendGateInfos(CUser* pUser) {
    // TODO: 需要完整实现 - IDA 0x14031DB80
}

void XMaze::SendDieMonsters(CUser* pUser) {
    // TODO: 需要完整实现 - IDA 0x14031DDB0
}

void XMaze::SendPotalInfos(CUser* pUser) {
    // TODO: 需要完整实现 - IDA 0x14031DEF0
}

void XMaze::SendInteractionInfos(CUser* pUser) {
    // TODO: 需要完整实现 - IDA 0x14031E1F0
}

void XMaze::SendLastClientSync(CUser* pUser) {
    // TODO: 需要完整实现 - IDA 0x14031EB10
}

void XMaze::SendObjectInfo(CUser* pUser, XActor* pActor) {
    // TODO: 需要完整实现 - IDA 0x14031EBE0
}

int XMaze::GetSectorIDFromPos(XVec3& vPos) {
    // TODO: 需要完整实现 - IDA 0x14031F450
    return 0;
}

CSector* XMaze::GetSectorFromPos(XVec3& vPos) {
    // TODO: 需要完整实现 - IDA 0x14031F670
    return nullptr;
}
