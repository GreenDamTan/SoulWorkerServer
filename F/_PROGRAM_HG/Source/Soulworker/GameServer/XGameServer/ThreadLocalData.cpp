// ThreadLocalData.cpp
// Thread-local data storage for GameServer
// Reconstructed from IDA decompilation

#include "ThreadLocalData.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#endif

// Forward declarations for stub types
class VScriptResourceManager {};
class GameScriptManager {};
class GameModeMgr {};
class DohHavokResourceManager {};
struct SS_REPORT_POOL_INFO {};

// Static instance for GetInstance() stub
static thread_local ThreadLocalData* s_pInstance = nullptr;
static thread_local VDefaultTimer* s_pTimer = nullptr;

// External global lock (stub)
// CFSRWLock g_VisionLock;

// ============================================================================
// CreateInstance - IDA @ 0x1406D08F0
// ============================================================================
ThreadLocalData* ThreadLocalData::CreateInstance(int threadCount, int ownerThreadIndex)
{
    // IDA: Creates VDefaultTimer and stores in TLS slot 1
    // Then creates ThreadLocalData and stores in TLS slot 3

    ThreadLocalData* pInstance = new ThreadLocalData();
    if (!pInstance) {
        return nullptr;
    }

    // Store in thread-local storage (slot 3 = offset 24)
#ifdef _WIN32
    // Note: Actual TLS implementation requires platform-specific code
    // GetTLSSlot(24)[0] = pInstance;
#endif

    pInstance->m_nThreadCount = threadCount;
    pInstance->m_nOwnerThreadIndex = ownerThreadIndex;

    return pInstance;
}

// ============================================================================
// Constructor - IDA @ 0x1406D0A50
// ============================================================================
ThreadLocalData::ThreadLocalData()
    : m_nMazeCount(0)
    , m_nMonsterCount(0)
    , m_nModeMazeCount(0)
    , m_bInitPool(false)
    , m_bLoadWorld(false)
    , m_bReqSyncWorld(false)
    , m_nLogSynctime(0)
    , m_nReportSynctime(0)
    , m_nThreadCount(0)
    , m_nOwnerThreadIndex(0)
    , m_pDistrictCreator(nullptr)
    , m_pBattleZoneCreator(nullptr)
    , m_pMazeCreator(nullptr)
    , m_pMyRoomCreator(nullptr)
    , m_pModeMazeCreator(nullptr)
{
    // IDA: Initialize all managers using their constructors
    // Note: Using unique_ptr, managers will be created on demand
}

// ============================================================================
// Destructor - IDA @ 0x1406D1870
// ============================================================================
ThreadLocalData::~ThreadLocalData()
{
    // IDA: Destroy all managers and pools in reverse order
    m_DohHavokResourceManager.reset();
    m_xScriptManager.reset();
    m_xLuaScript.reset();
    m_xDistrictPool.reset();
    m_xBattleZonePool.reset();
    m_xGameModeMgr.reset();
    m_xSocialItemObjectMgr.reset();
    m_xVaccumCubeObjectMgr.reset();
    m_xInteractionObjectMgr.reset();
    m_xChainLightningMgr.reset();
    m_xTrapMgr.reset();
    m_xProjectileMgr.reset();
    m_xAkashicMgr.reset();
    m_xMyRoomPool.reset();
    m_xModeMazePool.reset();
    m_xMazePool.reset();
    m_xNpcMgr.reset();
    m_xMonsterMgr.reset();
    m_mapChannelInfo.clear();
    m_mapArea.clear();
    m_mapAi.clear();
    m_xForceMgr.reset();
    m_xPartyMgr.reset();
}

// ============================================================================
// Initialize - IDA @ 0x1406D0C70
// ============================================================================
void ThreadLocalData::Initialize()
{
    m_nMazeCount = 0;
    m_nMonsterCount = 0;
    m_nModeMazeCount = 0;
    m_bInitPool = false;
    m_bLoadWorld = false;
    m_bReqSyncWorld = false;

#ifdef _WIN32
    m_nLogSynctime = GetTickCount64() + 60000;
    m_nReportSynctime = GetTickCount64() + 10000;
#else
    m_nLogSynctime = 0;
    m_nReportSynctime = 0;
#endif
}

// ============================================================================
// InitPool - IDA @ 0x1406D0D00
// ============================================================================
void ThreadLocalData::InitPool()
{
    // Acquire write lock for thread safety
    // CFAutoSlimWriteLock _autolock(&g_VisionLock);

    // Initialize script managers
    // GameScriptManager::InitManager(m_xLuaScript.get(), m_xScriptManager.get());
    // VScriptResourceManager::OneTimeInit(m_xScriptManager.get());

    // Load all scripts
    LoadAllScript();

    // Trigger engine init callbacks
    // VisCallback_cl::TriggerCallbacks(&Vision::Callbacks.OnEngineInit, nullptr);

    // Initialize all pools and managers
    // TODO: Full implementation requires all manager classes

    // Load all navigation meshes
    LoadAllNavmesh();

    m_bInitPool = true;
}

// ============================================================================
// GetInstance - IDA @ 0x1406D1A60
// ============================================================================
ThreadLocalData* ThreadLocalData::GetInstance()
{
#ifdef _WIN32
    // IDA: Returns TLS slot 3 (offset 24)
    // return reinterpret_cast<ThreadLocalData*>(reinterpret_cast<void**>(NtCurrentTeb()->ThreadLocalStoragePointer)[3]);
    // Stub: return static instance
    if (!s_pInstance) {
        s_pInstance = new ThreadLocalData();
    }
    return s_pInstance;
#else
    if (!s_pInstance) {
        s_pInstance = new ThreadLocalData();
    }
    return s_pInstance;
#endif
}

// ============================================================================
// GetTimer - IDA @ 0x1406D1A80
// ============================================================================
VDefaultTimer* ThreadLocalData::GetTimer()
{
    // IDA: Returns TLS slot 1 (offset 8)
    // return reinterpret_cast<VDefaultTimer*>(reinterpret_cast<void**>(NtCurrentTeb()->ThreadLocalStoragePointer)[1]);
    // Stub: return static timer
    if (!s_pTimer) {
        s_pTimer = new VDefaultTimer();
    }
    return s_pTimer;
}

// ============================================================================
// DestroyInstance - IDA @ 0x1406D1780
// ============================================================================
void ThreadLocalData::DestroyInstance()
{
    ThreadLocalData* pInstance = GetInstance();
    if (pInstance) {
        pInstance->Clear();
        delete pInstance;
#ifdef _WIN32
        // reinterpret_cast<void**>(NtCurrentTeb()->ThreadLocalStoragePointer)[3] = nullptr;
#endif
    }
}

// ============================================================================
// Clear - IDA @ 0x1406D14C0
// ============================================================================
void ThreadLocalData::Clear()
{
    // IDA: Acquire write lock for thread safety
    // CFAutoSlimWriteLock _autolock(&g_VisionLock);

    if (m_bInitPool) {
        ClearArea();
        ClearAi();

        // Clear monster manager
        if (m_xMonsterMgr) {
            m_xMonsterMgr->ClearAll();
        }

        // Clear battle zone creator and pool
        if (m_pBattleZoneCreator) {
            delete m_pBattleZoneCreator;
            m_pBattleZoneCreator = nullptr;
        }
        // m_xBattleZonePool->Clear(nullptr);

        // Clear district creator and pool
        if (m_pDistrictCreator) {
            delete m_pDistrictCreator;
            m_pDistrictCreator = nullptr;
        }
        // m_xDistrictPool->Clear(nullptr);

        // Clear maze creator and pool
        if (m_pMazeCreator) {
            delete m_pMazeCreator;
            m_pMazeCreator = nullptr;
        }
        // m_xMazePool->Clear(nullptr);

        // Clear mode maze creator and pool
        if (m_pModeMazeCreator) {
            delete m_pModeMazeCreator;
            m_pModeMazeCreator = nullptr;
        }
        // m_xModeMazePool->Clear(nullptr);

        // Clear my room creator and pool
        if (m_pMyRoomCreator) {
            delete m_pMyRoomCreator;
            m_pMyRoomCreator = nullptr;
        }
        // m_xMyRoomPool->Clear(nullptr);

        // Clear all other managers
        // m_xNpcMgr->ClearAll();
        // m_xAkashicMgr->ClearAll();
        // m_xProjectileMgr->ClearAll();
        // m_xTrapMgr->ClearAll();
        // m_xChainLightningMgr->ClearAll();
        // m_xInteractionObjectMgr->ClearAll();
        // m_xVaccumCubeObjectMgr->ClearAll();
        // m_xSocialItemObjectMgr->ClearAll();

        // Deinitialize script managers
        // GameScriptManager::DeInitManager(m_xLuaScript.get());
        // VScriptResourceManager::OneTimeDeInit(m_xScriptManager.get());
        // DohHavokResourceManager::finish(m_DohHavokResourceManager.get());
    }
}

// ============================================================================
// LoadAllNavmesh - IDA @ 0x1406D1AA0
// ============================================================================
void ThreadLocalData::LoadAllNavmesh()
{
    // TODO: Load all navigation meshes for AI pathfinding
}

// ============================================================================
// LoadAllScript - IDA @ 0x1406D5E80
// ============================================================================
void ThreadLocalData::LoadAllScript()
{
    // IDA: Load all Lua scripts for AI
    // Iterate through monster table and load AI scripts

    // TODO: Get monster table from XGameServer
    // XGameServer* pServer = XGameServer::Instance();
    // auto& mapMonster = pServer->m_mapMonsterTable;

    // for (auto& pair : mapMonster) {
    //     TB_MONSTER* pMonster = &pair.second;
    //
    //     // Load main AI script
    //     if (strlen(pMonster->szAIScript) > 0) {
    //         char szFilePath[256];
    //         sprintf(szFilePath, "\\AI\\%s.lua", pMonster->szAIScript);
    //         VScriptResource* pScript = m_xScriptManager->LoadScriptFile(szFilePath);
    //         if (!pScript || !pScript->IsLoaded()) {
    //             XPRINT(" [ %s ] Missing LuaScript Resource Load fail ", szFilePath);
    //         }
    //     }
    //
    //     // Load additional AI scripts
    //     for (int i = 0; i < 3; ++i) {
    //         if (strlen(pMonster->szAdditionalScript[i]) > 1) {
    //             char szBuffer[256];
    //             sprintf(szBuffer, "\\AI\\%s.lua", pMonster->szAdditionalScript[i]);
    //             VScriptResource* pScript = m_xScriptManager->LoadScriptFile(szBuffer);
    //             if (!pScript || !pScript->IsLoaded()) {
    //                 XPRINT(" [ %s ] Missing LuaScript Resource Load fail ", szBuffer);
    //             }
    //         }
    //     }
    // }
}

// ============================================================================
// AddArea - IDA @ 0x1406D5910
// ============================================================================
void ThreadLocalData::AddArea(XArea* pArea)
{
    if (!pArea) return;

    // IDA: Get instance ID and add to area map
    UXMapID uxMapID = pArea->GetInstanceID();

    // Calculate thread assignment based on map ID
    int nThread = (int)(uxMapID.nMapID % m_nThreadCount);

    // Insert into map
    m_mapArea[uxMapID] = pArea;

    unsigned short TBMapID = pArea->GetTBMapID();
    LogHelper::LogDebug("game.contents", "D Area %d %I64d %d", TBMapID, uxMapID.nMapID, m_nOwnerThreadIndex);
}

// ============================================================================
// FindArea - IDA @ 0x1406D5DA0
// ============================================================================
XArea* ThreadLocalData::FindArea(UXMapID uxMapID)
{
    // IDA: Find area by map ID
    auto it = m_mapArea.find(uxMapID);
    if (it != m_mapArea.end())
    {
        return it->second;
    }
    return nullptr;
}

// ============================================================================
// IsThreadArea - IDA @ 0x1406D5E10
// ============================================================================
bool ThreadLocalData::IsThreadArea(UXMapID uxMapID)
{
    // IDA: Check if area belongs to this thread
    return FindArea(uxMapID) != nullptr;
}

// ============================================================================
// ClearArea - IDA @ 0x1406D39C0
// ============================================================================
void ThreadLocalData::ClearArea()
{
    // IDA: Iterate through all areas, clear them, and return to appropriate pools
    for (auto it = m_mapArea.begin(); it != m_mapArea.end(); )
    {
        XArea* pArea = it->second;
        if (pArea)
        {
            pArea->Clear();

            // Return to appropriate pool based on type
            if (pArea->IsMaze())
            {
                // Check if it's a mode maze
                XModeMaze* pModeMaze = dynamic_cast<XModeMaze*>(pArea);
                if (pModeMaze)
                {
                    // TODO: m_xModeMazePool->Push(pModeMaze);
                    delete pModeMaze;
                }
                else
                {
                    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
                    if (pMaze)
                    {
                        // TODO: m_xMazePool->Push(pMaze);
                        delete pMaze;
                    }
                }
            }
            else
            {
                // Battle zone
                CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
                if (pBattleZone)
                {
                    // TODO: m_xBattleZonePool->Push(pBattleZone);
                    delete pBattleZone;
                }
            }
        }
        it = m_mapArea.erase(it);
    }
}

// ============================================================================
// ClearAi - IDA @ 0x1406D3B90
// ============================================================================
void ThreadLocalData::ClearAi()
{
    // IDA: Iterate through all AI objects, delete them, and clear the map
    for (auto it = m_mapAi.begin(); it != m_mapAi.end(); )
    {
        CAi* pAi = it->second;
        if (pAi)
        {
            // Delete the AI object
            delete pAi;
        }
        it = m_mapAi.erase(it);
    }
    m_mapAi.clear();
}

// ============================================================================
// CreateDistrict - IDA @ 0x1406D1DA0
// ============================================================================
bool ThreadLocalData::CreateDistrict(UXMapID uxMapID, TB_DISTRICT& tblDistrict)
{
    // IDA: Create district from pool
    XDistrict* pDistrict = nullptr;
    // TODO: m_xDistrictPool->Pop(&pDistrict);

    if (pDistrict)
    {
        if (pDistrict->Create(&tblDistrict))
        {
            pDistrict->Init();
            pDistrict->SetInstanceID(uxMapID);
            pDistrict->SetMaxUserCount(tblDistrict.District_Max_People);

            ThreadLocalData::GetInstance()->AddArea(pDistrict);
            LogHelper::LogInfo("game.system", "Create Map District %d %I64d", tblDistrict.District_ID, uxMapID.nMapID);

            // TODO: XWorldManager::Instance()->AddArea(pDistrict);
            return true;
        }
        else
        {
            LogHelper::LogError("game.system", "CreateDistrict error - Cant create district in pool ( %s ) ( %d )",
                "ThreadLocalData.cpp", 204);
            // TODO: Decrement static map count
            // TODO: m_xDistrictPool->Push(pDistrict);
            return false;
        }
    }
    else
    {
        LogHelper::LogError("game.system", "CreateDistrict error - Failed Create District[ DistrictID:%d ] ( %d )",
            tblDistrict.District_ID, 197);
        // TODO: Decrement static map count
        return false;
    }
}

// ============================================================================
// CreateBattleZone - IDA @ 0x1406D1F20
// ============================================================================
bool ThreadLocalData::CreateBattleZone(UXMapID uxMapID, TB_MAZE_INFO& tblMazeInfo)
{
    // IDA: Create battle zone from pool
    CBattleZone* pBattleZone = nullptr;
    // TODO: m_xBattleZonePool->Pop(&pBattleZone);

    if (pBattleZone)
    {
        if (pBattleZone->Create(uxMapID, &tblMazeInfo))
        {
            int nMaxUserCount = 50;
            // Special handling for map 30031
            if ((short)uxMapID.nMapID == 30031) {
                nMaxUserCount = tblMazeInfo.Maze_Enter_Count;
            }

            ThreadLocalData::GetInstance()->AddArea(pBattleZone);
            pBattleZone->SetMaxUserCount(nMaxUserCount);

            // TODO: XWorldManager::Instance()->AddArea(pBattleZone);
            LogHelper::LogInfo("game.system", "Battle Zone %I64d %d", uxMapID.nMapID, tblMazeInfo.ID);
            return true;
        }
        else
        {
            // TODO: Decrement static map count
            // TODO: m_xBattleZonePool->Push(pBattleZone);
            return false;
        }
    }
    else
    {
        // TODO: Decrement static map count
        LogHelper::LogError("game.contents", "CreateBattleZone error - Failed Create Battle Zone[ ID:%d ] ( %d )",
            tblMazeInfo.ID, 230);
        return false;
    }
}

// ============================================================================
// CreateMaze - IDA @ 0x1406D2080
// ============================================================================
void ThreadLocalData::CreateMaze(ST_CREATE_MAZE& stCreateMaze)
{
    // IDA: Create maze from pool
    XMaze* pMaze = nullptr;
    // TODO: m_xMazePool->Pop(&pMaze);

    if (pMaze)
    {
        ++m_nMazeCount;

        if (pMaze->Create(&stCreateMaze))
        {
            pMaze->SetPartyInfo(stCreateMaze.stPartyInfo);
            pMaze->SetEnterDistrictPos(&stCreateMaze.stEnterDistrictPos);
            AddArea(pMaze);

            size_t memberCount = stCreateMaze.vecEnterMember.size();
            LogHelper::LogDebug("game.contents",
                "< %I64d MAZE > Success Create Maze USER( %d ), MAP ( %d ) , type( %d ), Party( %d ), EnterMember ( %d ) ",
                stCreateMaze.uxMapID.nMapID, stCreateMaze.dwUserID, stCreateMaze.uxMapID.nMapID,
                stCreateMaze.stPartyInfo.byGroupType, stCreateMaze.stPartyInfo.nID, memberCount);
        }
        else
        {
            stCreateMaze.nResult = 50007;
            // TODO: m_xMazePool->Push(pMaze);
        }
    }
    else
    {
        stCreateMaze.nResult = 50006;
        // int poolSize = m_xMazePool->GetCurSize();
        LogHelper::LogError("game.contents",
            "CreateMaze error - Failed create maze no pool[ MapID:%I64d, CurPool:%d] ( %d )",
            stCreateMaze.uxMapID.nMapID, 0, 272);
    }

    LogHelper::LogInfo("game.contents", "<CREATE_MAZE> Create %d Maze ( UCID : %d / %I64d ) - Result : %d",
        (int)(stCreateMaze.uxMapID.nMapID >> 32), stCreateMaze.dwUserID,
        stCreateMaze.uxMapID.nMapID, stCreateMaze.nResult);

    // Log to DB
    ST_LOG_GAME stLog;
    stLog._nUAID = stCreateMaze.dwUAID;
    stLog._nUCID = stCreateMaze.dwUserID;
    stLog._sMainType = 5;
    stLog._sSubType = 1;
    stLog.nParam0 = (int)(stCreateMaze.uxMapID.nMapID >> 32);
    stLog.nParam1 = (int)stCreateMaze.vecEnterMember.size();
    stLog.nParam4 = stCreateMaze.nResult;
    stLog.nParam6 = stCreateMaze.uxMapID.nMapID;
    // wcscpy_s(stLog.szComment, L"Create Maze");
    // TODO: XGameServer::Instance()->SendDBLog(&stLog);

    // TODO: Send to control socket
    // XSendPacket xSendPacket(0xF2, 0x22);
    // xSendPacket << stCreateMaze;
    // XGameServer::Instance()->m_controlSocket.SendCheck(&xSendPacket);
}

// ============================================================================
// DeleteMaze - IDA @ 0x1406D4A00
// ============================================================================
void ThreadLocalData::DeleteMaze(UXMapID uxMapID)
{
    // IDA: Find and delete maze
    auto it = m_mapArea.find(uxMapID);
    if (it == m_mapArea.end())
    {
        LogHelper::LogError("game.contents", "DeleteMaze error - Cant find maze[ MapID:%I64d ] ( %d )",
            uxMapID.nMapID, 868);
        return;
    }

    XArea* pArea = it->second;
    if (!pArea)
    {
        LogHelper::LogError("game.contents", "DeleteMaze error - Invalid Area[ MapID:%I64d ] ( %d )",
            uxMapID.nMapID, 876);
        return;
    }

    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    if (!pMaze)
    {
        LogHelper::LogError("game.contents", "DeleteMaze error - Invalid Maze[ MapID:%I64d ] ( %d )",
            uxMapID.nMapID, 883);
        return;
    }

    // Check if there are still users in maze
    int nUserCount = pMaze->GetCurUserCount();
    if (nUserCount > 0)
    {
        ST_LOG_GAME stLogGame;
        stLogGame._nUAID = 0;
        stLogGame._nUCID = 0;
        stLogGame._sMainType = 5;
        stLogGame._sSubType = 2;
        stLogGame.nParam0 = pArea->GetTBMapID();
        stLogGame.nParam1 = nUserCount;
        stLogGame.nParam6 = uxMapID.nMapID;
        // wcscpy_s(stLogGame.szComment, L"User in Maze");
        // XGameServer::Instance()->SendDBLog(&stLogGame);

        LogHelper::LogError("game.contents",
            "DeleteMaze error - there is user in maze[ MapID:%I64d, UserCount:%d ] ( %d )",
            uxMapID.nMapID, nUserCount, 901);
        return;
    }

    bool bClearMaze = pMaze->IsCompleteClearMaze();
    unsigned short wMapID = pArea->GetTBMapID();
    int nEnterCount = pMaze->GetMazeEnterCount();

    // Clear and remove
    pArea->Clear();
    m_mapArea.erase(it);

    // Return to pool
    // TODO: m_xMazePool->Push(pMaze);
    // int nPoolSize = m_xMazePool->GetCurSize();

    ST_LOG_GAME stLog;
    stLog._nUAID = 0;
    stLog._nUCID = 0;
    stLog._sMainType = 5;
    stLog._sSubType = 2;
    stLog.nParam0 = wMapID;
    stLog.nParam1 = nEnterCount;
    stLog.nParam2 = bClearMaze;
    // stLog.nParam5 = nPoolSize;
    stLog.nParam6 = uxMapID.nMapID;
    // wcscpy_s(stLog.szComment, L"Delete Maze");
    // XGameServer::Instance()->SendDBLog(&stLog);
}

// ============================================================================
// EnterArea - IDA @ 0x1406D3CA0
// ============================================================================
bool ThreadLocalData::EnterArea(CUser* pUser, PS_PARTY_INFO& stPartyInfo)
{
    // IDA: Enter area processing - handles party/force, boosters, and map info updates
    if (!pUser) {
        LogHelper::LogError("game.contents", "EnterArea error - Invalid User ( %d )", 705);
        return false;
    }

    // Get user's map ID from character info
    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();
    UXMapID uxMapID = pCharInfo->stPosInfo.uxMapID;

    // Find the area
    XArea* pArea = FindArea(uxMapID);
    if (!pArea) {
        UXActorID actorID;
        pUser->GetActorID(&actorID);
        LogHelper::LogError("game.contents", "EnterArea error - Cant find area[ ActorID:%d, MapID:%I64d ] ( %d )",
            actorID.dwActorID, uxMapID.nMapID, 847);

        PS_KICK_USER_INFO psKick;
        psKick.dwUAID = pUser->GetUAID();
        psKick.byKickType = 25;
        pUser->Kickout(&psKick, true);
        return false;
    }

    unsigned short wMapID = pArea->GetTBMapID();

    // Check disconnect state for maze
    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    if (pArea->IsMaze() && pMaze) {
        unsigned char byState = 0;
        unsigned char byRealState[15] = {0};
        UXActorID actorID;
        pUser->GetActorID(&actorID);
        byState = pMaze->CheckDisconnecUsertState(actorID.dwActorID, byRealState);

        if (byRealState[0] == 12 || byRealState[0] == 13) {
            LogHelper::LogError("game.contents", "EnterArea error - Failed enter CheckDisconnecUsertState[ MapID:%d ] ( %d )",
                wMapID, 723);
            pUser->SendErrorMessage(0x11, 0x43, 0xD6DA);

            PS_KICK_USER_INFO psKick;
            psKick.dwUAID = pUser->GetUAID();
            psKick.byKickType = 3;
            pUser->Kickout(&psKick, true);
            return false;
        }
    }

    // Enter actor into area
    if (!pArea->EnterActor(&pUser->XActor)) {
        UXActorID actorID;
        pUser->GetActorID(&actorID);
        UXMapID instanceID = pArea->GetInstanceID();
        LogHelper::LogError("game.contents", "EnterArea error - Failed enter Map %I64d / %d ",
            instanceID.nMapID, actorID.dwActorID);
        pUser->SendErrorMessage(0x11, 0x43, 0xD6DA);

        PS_KICK_USER_INFO psKick;
        psKick.dwUAID = pUser->GetUAID();
        psKick.byKickType = 25;
        pUser->Kickout(&psKick, true);
        return false;
    }

    // Log successful entry
    UXActorID actorID;
    pUser->GetActorID(&actorID);
    UXMapID instanceID = pArea->GetInstanceID();
    LogHelper::LogInfo("game.contents", "[ENTER_AREA] %I64d / %d ", instanceID.nMapID, actorID.dwActorID);

    // Check boosters
    std::tr1::shared_ptr<CGocBooster> pBooster = pUser->GetGOC<CGocBooster>();
    if (pBooster) {
        pBooster->CheckTimeEventBooster();
        pBooster->CheckDayEventBooster(wMapID);
    }

    // Set matching state
    std::tr1::shared_ptr<CGocForce> pGocForce = pUser->GetGOC<CGocForce>();
    if (pGocForce) {
        pGocForce->SetMatchingState(0);
    }

    // Handle party info
    if (stPartyInfo.byGroupType == 1 && stPartyInfo.nID > 0) {
        // Party member
        std::tr1::shared_ptr<CGocParty> pGocParty = pUser->GetGOC<CGocParty>();
        std::tr1::shared_ptr<CParty> pParty = m_xPartyMgr ? m_xPartyMgr->GetParty(stPartyInfo.nID) : nullptr;

        if (pParty && pGocParty) {
            UXActorID actorID;
            pUser->GetActorID(&actorID);

            if (pParty->IsMember(actorID.dwActorID)) {
                int nChannel = (int)(uxMapID.nMapID >> 24);
                int nMapID = (int)(uxMapID.nMapID << 16 >> 48);
                pParty->SetMemberMapID(actorID.dwActorID, nMapID, nChannel, uxMapID);

                pGocParty->SetForce(pParty);
                pGocParty->SendPartyInfo(0);

                if (pArea->IsMaze()) {
                    pGocParty->LoadRecode();
                }
            } else {
                LogHelper::LogError("game.contents", "[PARTY] Failed Enter Party %d / %d",
                    stPartyInfo.nID, actorID.dwActorID);
            }
        }
    }
    else if (stPartyInfo.byGroupType == 2 && stPartyInfo.nID > 0) {
        // Force member
        std::tr1::shared_ptr<CForce> pForce = m_xForceMgr ? m_xForceMgr->GetForce(stPartyInfo.nID) : nullptr;

        if (pForce && pGocForce) {
            UXActorID actorID;
            pUser->GetActorID(&actorID);

            if (pForce->IsMember(actorID.dwActorID)) {
                int nChannel = (int)(uxMapID.nMapID >> 24);
                int nMapID = (int)(uxMapID.nMapID << 16 >> 48);
                pForce->SetMemberMapID(actorID.dwActorID, nMapID, nChannel, uxMapID);

                pGocForce->SetForce(pForce);
                pGocForce->SendForceInfo(0);

                if (pArea->IsMaze()) {
                    pGocForce->LoadRecode();
                }
            }
        }
    }

    // Handle league
    if (pUser->GetLeagueID()) {
        AddLeagueMember(pUser);
    }

    // Send map info update to control/community sockets
    PS_UPDATE_USER_MAP_INFO stMapInfo;
    stMapInfo.dwUAID = pUser->GetUAID();
    UXActorID actorID2;
    pUser->GetActorID(&actorID2);
    stMapInfo.dwActorID = actorID2.dwActorID;
    stMapInfo.uxMapID = uxMapID;
    stMapInfo.stPartyInfo = stPartyInfo;
    stMapInfo.biAuthSessionID = pUser->GetAuthSessionID();

    // TODO: Send to control socket and community socket
    // XSendPacket xSendPacket(0xF3, 4);
    // xSendPacket << stMapInfo;
    // XGameServer::Instance()->m_controlSocket.Send(&xSendPacket);

    return true;
}

// ============================================================================
// LeaveArea - IDA @ 0x1406D4E20
// ============================================================================
bool ThreadLocalData::LeaveArea(XClient* pClient)
{
    // IDA: Leave area processing - handles party/force save, boosters, and maze cleanup
    CUser* pUser = dynamic_cast<CUser*>(pClient);
    if (!pUser) {
        return false;
    }

    // Get user's map ID
    UXMapID uxID;
    pUser->XActor.GetMapInsID(&uxID);

    // Find the area
    auto it = m_mapArea.find(uxID);
    if (it == m_mapArea.end()) {
        if (pUser->GetLeagueID()) {
            LeaveLeagueMember(pUser);
        }
        return true;
    }

    XArea* pArea = it->second;
    if (!pArea) {
        if (pUser->GetLeagueID()) {
            LeaveLeagueMember(pUser);
        }
        return true;
    }

    // Exit actor from area
    XActor* pActor = dynamic_cast<XActor*>(pClient);
    if (pArea->ExitActor(pActor)) {
        UXActorID actorID;
        pUser->GetActorID(&actorID);
        UXMapID instanceID = pArea->GetInstanceID();
        LogHelper::LogError("game.contents", "LeaveArea error - Critical Failed exit Actor in ChangeMap ( %I64d / %d )",
            instanceID.nMapID, actorID.dwActorID);
        return false;
    }

    // Log successful leave
    UXActorID actorID;
    pUser->GetActorID(&actorID);
    UXMapID instanceID = pArea->GetInstanceID();
    LogHelper::LogInfo("game.contents", "[LEAVE_AREA] %I64d / %d ", instanceID.nMapID, actorID.dwActorID);

    // Handle maze-specific cleanup
    XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    if (pMaze && pArea->GetWorldType() != 3) {
        if (pMaze->IsDeleteMaze()) {
            // Set maze state to deleting
            pMaze->SetMazeState(5, 0);

            // Send maze update to control
            PS_MAZE_UPDATE_INFO stUpdateMaze;
            stUpdateMaze.uxMapID = pArea->GetInstanceID();
            stUpdateMaze.nState = 3;

            // TODO: Send to control socket
            // XSendPacket xSendPacket(0xF2, 0x26);
            // xSendPacket << stUpdateMaze;
            // XGameServer::Instance()->m_controlSocket.SendCheck(&xSendPacket);

            // Log maze deletion
            ST_LOG_GAME stLog;
            stLog._nUAID = 0;
            stLog._nUCID = 0;
            stLog._sMainType = 5;
            stLog._sSubType = 2;
            stLog.nParam0 = pArea->GetTBMapID();
            stLog.nParam1 = 100;
            // stLog.nParam6 = pMaze->GetInstanceID().nMapID;
            // wcscpy_s(stLog.szComment, L"Maze Deleted");
            // XGameServer::Instance()->SendDBLog(&stLog);
        }
    }
    else if (pArea->GetWorldType() == 3) {
        // MyRoom handling
        XMyRoom* pRoom = dynamic_cast<XMyRoom*>(pArea);
        if (pRoom) {
            if (!pRoom->GetCurUserCount()) {
                pRoom->ClearState();
            }
        }
    }

    // Clear boosters
    std::tr1::shared_ptr<CGocBooster> pBooster = pUser->GetGOC<CGocBooster>();
    if (pBooster) {
        pBooster->ChangeBooster(eBooster_Type_Day_Event, 0, 0, 0);
    }

    // Save and clear party info
    std::tr1::shared_ptr<CGocParty> pGocParty = pUser->GetGOC<CGocParty>();
    if (pGocParty) {
        if (pArea->IsMaze()) {
            pGocParty->SaveRecode();
        }
        pGocParty->Clear();
    }

    // Save and clear force info
    std::tr1::shared_ptr<CGocForce> pGocForce = pUser->GetGOC<CGocForce>();
    if (pGocForce) {
        if (pArea->IsMaze()) {
            pGocForce->SaveRecode();
        }
        pGocForce->Clear();
    }

    // Send exp save
    pUser->SendExpSave();

    // Handle league
    if (pUser->GetLeagueID()) {
        LeaveLeagueMember(pUser);
    }

    return true;
}

// ============================================================================
// ChangeMap - IDA @ 0x1406D5410
// ============================================================================
bool ThreadLocalData::ChangeMap(XClient* pClient, UXMapID uxMapID, XVec3& vPos, float fRot)
{
    // IDA: Change map - leave current area and enter new area via thread job
    CUser* pUser = dynamic_cast<CUser*>(pClient);
    if (!pUser) {
        return false;
    }

    // Get before map ID
    unsigned short nBeforeID = (unsigned short)pUser->XActor.GetArea()->m_pInstance;

    // Get party info
    std::tr1::shared_ptr<CGocParty> pParty = pUser->GetGOC<CGocParty>();
    if (!pParty) {
        return false;
    }

    // Get force info
    std::tr1::shared_ptr<CGocForce> pForce = pUser->GetGOC<CGocForce>();
    if (!pForce) {
        return false;
    }

    // Build party info structure
    ST_PARTY_INFO stPartyInfo;
    memset(&stPartyInfo, 0, sizeof(stPartyInfo));

    unsigned int dwPartyID = pParty->GetPartyID();
    unsigned int dwForceID = pForce->GetPartyID();

    if (dwPartyID) {
        stPartyInfo.byGroupType = 1;
        stPartyInfo.nID = dwPartyID;
    }
    else if (dwForceID) {
        stPartyInfo.byGroupType = 2;
        stPartyInfo.nID = dwForceID;
    }

    // Save auth session ID
    __int64 biAuthSessionID = pUser->GetAuthSessionID();

    // Leave current area
    if (!LeaveArea(pClient)) {
        PS_KICK_USER_INFO psKick;
        psKick.dwUAID = pUser->GetUAID();
        psKick.byKickType = 4;
        psKick.nParam = 100;
        pUser->Kickout(&psKick, true);

        UXActorID actorID;
        pUser->GetActorID(&actorID);
        LogHelper::LogError("game.contents", "ChangeMap error - Failed leave area[ ActorID:%d ] ( %d )",
            actorID.dwActorID, 1164);
        return false;
    }

    // Check if kickout now
    if (pUser->CheckKickoutNow() == 1) {
        UXActorID actorID;
        pUser->GetActorID(&actorID);
        LogHelper::LogError("game.contents", "[Kickout] Now ActorID:%d ( %d )", actorID.dwActorID, 0);
        return false;
    }

    // Update user's map info
    STMyCharInfoEx* pCharInfo = pUser->stMyCharInfoEx();
    pCharInfo->stPosInfo.uxMapID = uxMapID;
    pCharInfo->stPosInfo.vPos = vPos;
    pCharInfo->stPosInfo.fRot = fRot;
    pCharInfo->stPosInfo.sWorldID = (short)uxMapID.nMapID;

    // Set position
    pUser->SetPositionXVec3(vPos);
    pUser->SetAuthSessionID(biAuthSessionID);

    // TODO: Queue enter area job to logic thread manager
    // CLogicThreadManager::Instance()->DoJob(uxMapID.nMapID, [this, pUser, stPartyInfo]() {
    //     EnterArea(pUser, stPartyInfo);
    // });

    (void)nBeforeID; // Suppress unused warning
    return true;
}

// ============================================================================
// CreateMonster - IDA @ 0x1406D88B0
// ============================================================================
CMonster* ThreadLocalData::CreateMonster(XArea* pArea, UXMapID uxMapID, int nMonsterID, XVec3 vPos, float fRot)
{
    // IDA: Create monster using monster manager
    CMonster* pMonster = nullptr;
    if (m_xMonsterMgr) {
        pMonster = m_xMonsterMgr->Create(uxMapID, nMonsterID, &vPos, fRot);
    }

    if (pMonster) {
        // Increment monster count
        ++m_nMonsterCount;

        // Set area for the monster
        // pMonster->SetArea(pArea);

        // Generate event object
        // pMonster->GenerateEventObject();

        // Check height and adjust position
        // hkvVec3 vCheckPos(vPos.x, vPos.y, vPos.z);
        // if (pMonster->GetHeight(&vCheckPos, 200.0f)) {
        //     vPos.z = vCheckPos.z;
        //     pMonster->Move(&vPos);
        // }

        // Set initial yaw
        // pMonster->SetInitYaw(fRot);
    }

    return pMonster;
}

// ============================================================================
// DeleteMonster - IDA @ 0x1406D8A30
// ============================================================================
void ThreadLocalData::DeleteMonster(CMonster* pMonster)
{
    // IDA: Delete monster using monster manager
    if (!pMonster) return;
    if (m_xMonsterMgr) {
        m_xMonsterMgr->Delete(pMonster);
    }
}

// ============================================================================
// AddAi - IDA @ 0x1406D59C0
// ============================================================================
bool ThreadLocalData::AddAi(CAi* pAi, CMonster* pMonster, const char* szScript)
{
    (void)pAi;
    (void)pMonster;
    (void)szScript;
    if (szScript) {
        m_mapAi[szScript] = pAi;
    }
    return true;
}

// ============================================================================
// Update - IDA @ 0x1406D61A0
// ============================================================================
void ThreadLocalData::Update(float fDeltaTime)
{
    // IDA: Update all areas and managers
    if (!m_bInitPool)
        return;

    // Update all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->OnUpdate();
        }
    }

    // Tick game mode manager
    if (m_xGameModeMgr)
    {
        m_xGameModeMgr->Tick(fDeltaTime);
    }

    // Log pool stats periodically (every 30 minutes)
#ifdef _WIN32
    if (m_nLogSynctime < GetTickCount64())
    {
        // Log maze pool stats
        // TODO: Get actual pool sizes
        LogHelper::LogInfo("game.system", "<POOL> Thread%d MAZE\\t( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d MODE MAZE\\t( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d MYROOM\\t( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d MONSTER  ( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d NPC\\t  ( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d AKASHIC\\t ( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d PROJECTILE( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d TRAP\\t ( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d CHAINLIGHT( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d INTERACTION( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d DISTRICT  ( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d BATTLE    ( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);
        LogHelper::LogInfo("game.system", "<POOL> Thread%d SOCIAL    ( %d / %d ) ",
            m_nOwnerThreadIndex, 0, 0);

        m_nLogSynctime = GetTickCount64() + 1800000; // 30 minutes
    }

    // Update report info periodically (every 10 seconds)
    if (m_nReportSynctime < GetTickCount64())
    {
        UpdateReportInfo();
        m_nReportSynctime = GetTickCount64() + 10000;
    }
#endif
}

// ============================================================================
// SendBroadcast - IDA @ 0x1406D6100
// ============================================================================
void ThreadLocalData::SendBroadcast(XSendPacket& xSendPacket)
{
    // IDA: Send broadcast to all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            // Send packet to all clients in area
            pArea->SendBroadcast(&xSendPacket, 0, 2);
        }
    }
}

// ============================================================================
// UpdateChannel - IDA @ 0x1406D6920
// ============================================================================
void ThreadLocalData::UpdateChannel(std::uint16_t wMapID, ST_CHANNEL_INFO& stChannel)
{
    // IDA: Update specific channel info
    auto it = m_mapChannelInfo.find(wMapID);
    if (it != m_mapChannelInfo.end())
    {
        // Find and update the specific channel entry
        // auto& channelVec = it->second;
        // for (size_t i = 0; i < channelVec.size(); ++i) {
        //     if (channelVec[i].wChannel == stChannel.wChannel) {
        //         channelVec[i].byChannelState = stChannel.byChannelState;
        //         break;
        //     }
        // }
    }
}

// ============================================================================
// SendChannelInfo - IDA @ 0x1406D6A50
// ============================================================================
void ThreadLocalData::SendChannelInfo(CUser* pUser, std::uint16_t wChannelID)
{
    // IDA: Send channel info to user
    auto it = m_mapChannelInfo.find(wChannelID);
    if (it != m_mapChannelInfo.end())
    {
        // XSendPacket xSendPacket(0xF1, 1);
        // xSendPacket << it->second;
        // pUser->Send(&xSendPacket);
    }
}

// ============================================================================
// ReloadMonsterAI - IDA @ 0x1406D6B70
// ============================================================================
void ThreadLocalData::ReloadMonsterAI()
{
    // IDA: Reload all monster AI scripts
    // TODO: Iterate through monster table from XGameServer
    // XGameServer* pServer = XGameServer::Instance();
    // auto& mapMonster = pServer->m_mapMonsterTable;

    // for (auto& pair : mapMonster) {
    //     TB_MONSTER* pMonster = &pair.second;
    //
    //     // Reload main AI script
    //     if (strlen(pMonster->szAIScript) > 0) {
    //         char szFilePath[256];
    //         sprintf(szFilePath, "\\AI\\%s.lua", pMonster->szAIScript);
    //
    //         // Remove old resource if exists
    //         VManagedResource* pResource = m_xScriptManager->GetResourceByName(szFilePath);
    //         if (pResource) {
    //             m_xScriptManager->RemoveResource(pResource);
    //             LogHelper::LogDebug("game.contents", "[RELOAD] Monster Script %s", pMonster->szAIScript);
    //         }
    //
    //         // Load new script
    //         VScriptResource* pScript = m_xScriptManager->LoadScriptFile(szFilePath);
    //         if (!pScript || !pScript->IsLoaded()) {
    //             XPRINT(" [ %s ] Missing LuaScript Resource Load fail ", szFilePath);
    //         }
    //     }
    //
    //     // Reload additional AI scripts
    //     for (int i = 0; i < 3; ++i) {
    //         if (strlen(pMonster->szAdditionalScript[i]) > 1) {
    //             char szBuffer[256];
    //             sprintf(szBuffer, "\\AI\\%s.lua", pMonster->szAdditionalScript[i]);
    //
    //             VManagedResource* pResource = m_xScriptManager->GetResourceByName(szBuffer);
    //             if (pResource) {
    //                 m_xScriptManager->RemoveResource(pResource);
    //                 LogHelper::LogDebug("game.contents", "[RELOAD] Monster Switch Script %s", pMonster->szAdditionalScript[i]);
    //             }
    //
    //             VScriptResource* pScript = m_xScriptManager->LoadScriptFile(szBuffer);
    //             if (!pScript || !pScript->IsLoaded()) {
    //                 XPRINT(" [ %s ] Missing LuaScript Resource Load fail ", szBuffer);
    //             }
    //         }
    //     }
    // }

    // Clear AI objects to force reload
    ClearAi();
}

// ============================================================================
// SendMazeInfo - IDA @ 0x1406D6EF0
// ============================================================================
void ThreadLocalData::SendMazeInfo()
{
    // IDA: Send maze info sync to control socket
    int nMazeCount = 0;

    // Iterate through all areas and sync maze info
    for (auto& pair : m_mapArea)
    {
        XMaze* pMaze = dynamic_cast<XMaze*>(pair.second);
        if (pMaze)
        {
            // pMaze->SendMazeInfoSync();
            ++nMazeCount;
        }
    }

    // Send final sync packet
    // PS_MAZE_UPDATE_INFO_SYNC stUpdateMaze;
    // stUpdateMaze.bLast = 1;
    // XSendPacket xSendPacket(0xF2, 0x23);
    // xSendPacket << stUpdateMaze;
    // XGameServer::Instance()->m_controlSocket.Send(&xSendPacket);

    LogHelper::LogInfo("game.system", "<SYNC> Update Maze Finish [%d] ", nMazeCount);
}

// ============================================================================
// GetScriptManager - IDA @ 0x14025E780
// ============================================================================
VScriptResourceManager& ThreadLocalData::GetScriptManager()
{
    static VScriptResourceManager s_instance;
    return s_instance;
}

// ============================================================================
// GetGameScriptManager - IDA @ 0x14025E7A0
// ============================================================================
GameScriptManager& ThreadLocalData::GetGameScriptManager()
{
    static GameScriptManager s_instance;
    return s_instance;
}

// ============================================================================
// GetGameModeMgr - IDA @ 0x140310490
// ============================================================================
GameModeMgr* ThreadLocalData::GetGameModeMgr()
{
    return m_xGameModeMgr.get();
}

// ============================================================================
// GetReportPoolInfo - IDA @ 0x1403E1E20
// ============================================================================
SS_REPORT_POOL_INFO& ThreadLocalData::GetReportPoolInfo()
{
    static SS_REPORT_POOL_INFO s_info;
    return s_info;
}

// ============================================================================
// AddLeagueMember - IDA @ 0x1406D70A0
// ============================================================================
void ThreadLocalData::AddLeagueMember(CUser* pUser)
{
    // IDA: Add league member to tracking map
    if (!pUser) return;

    int nLeagueID = pUser->GetLeagueID();
    auto it = m_mapLeagueMember.find(nLeagueID);

    if (it != m_mapLeagueMember.end())
    {
        // Add to existing league member list
        // it->second->AddLeagueMember(pUser);
        // Compare sync count
        // unsigned int dwUCID = pUser->GetActorID().dwActorID;
        // it->second->CompareSyncCount(dwUCID);
    }
    else
    {
        // Create new league member list
        // CLeagueMember* pLeagueMember = new CLeagueMember();
        // pLeagueMember->AddLeagueMember(pUser);
        // m_mapLeagueMember[nLeagueID] = pLeagueMember;
        // pLeagueMember->SendSyncLeagueInfo(pUser);
    }
}

// ============================================================================
// LeaveLeagueMember - IDA @ 0x1406D7240
// ============================================================================
void ThreadLocalData::LeaveLeagueMember(CUser* pUser)
{
    // IDA: Remove league member from tracking map
    if (!pUser) return;

    int nLeagueID = pUser->GetLeagueID();
    auto it = m_mapLeagueMember.find(nLeagueID);

    if (it != m_mapLeagueMember.end())
    {
        // unsigned int dwUCID = pUser->GetActorID().dwActorID & 0x1FFFFFFF;
        // it->second->DeleteLeagueMember(dwUCID);
    }
}

// ============================================================================
// CreateMyRoom - IDA @ 0x1406D7D90
// ============================================================================
void ThreadLocalData::CreateMyRoom(ST_MYROOM_USER& stEnterUser, ST_MYROOM_OWNER_INFO& stOwnerInfo, UXMapID uxMapID, unsigned int dwOwnerUCID)
{
    // IDA: Create my room from pool
    int nResult = 0;
    XMyRoom* pMyRoom = nullptr;
    // TODO: m_xMyRoomPool->Pop(&pMyRoom);

    if (pMyRoom)
    {
        if (pMyRoom->CreateRoom(&stEnterUser, &stOwnerInfo, uxMapID, dwOwnerUCID))
        {
            AddArea(pMyRoom);
            LogHelper::LogError("game.contents", "Create MyRoom Success UCID: %d", stEnterUser.nPoint);
        }
        else
        {
            nResult = 1;
            // m_xMyRoomPool->Push(pMyRoom);
            LogHelper::LogError("game.contents", "Create MyRoom Fail UCID: %d", stEnterUser.nPoint);
        }
    }
    else
    {
        nResult = 1;
        LogHelper::LogError("game.contents", "Create MyRoom No Pool UCID: %d", stEnterUser.nPoint);
    }

    if (nResult == 1)
    {
        // Send error response to control socket
        // XSendPacket xSendPacket(0xF2, 0x52);
        // xSendPacket << 1;
        // xSendPacket << stOwnerInfo.dwOwnerUAID;
        // xSendPacket << stEnterUser;
        // xSendPacket << dwOwnerUCID;
        // XGameServer::Instance()->m_controlSocket.SendCheck(&xSendPacket);
    }
}

// ============================================================================
// EnterMyRoom - IDA @ 0x1406D7FB0
// ============================================================================
void ThreadLocalData::EnterMyRoom(ST_MYROOM_USER& stEnterUser, UXMapID uxMapID, unsigned int dwOwnerUCID)
{
    // IDA: Enter existing my room
    int nErrorCode = 0;
    ST_MYROOM_OWNER_INFO stOwnerInfo;

    XArea* pArea = FindArea(uxMapID);
    if (pArea)
    {
        XMyRoom* pMyRoom = dynamic_cast<XMyRoom*>(pArea);
        if (pMyRoom)
        {
            nErrorCode = pMyRoom->EnterRoom(&stEnterUser);
            pMyRoom->GetOwnerInfo(&stOwnerInfo);
        }
    }
    else
    {
        nErrorCode = 58204; // Error code for room not found
    }

    // Send response to control socket
    // XSendPacket xSendPacket(0xF2, 0x51);
    // xSendPacket << nErrorCode;
    // xSendPacket << stEnterUser;
    // xSendPacket << uxMapID;
    // xSendPacket << stOwnerInfo;
    // xSendPacket << dwOwnerUCID;
    // XGameServer::Instance()->m_controlSocket.SendCheck(&xSendPacket);
}

// ============================================================================
// DeleteMyRoomReq - IDA @ 0x1406D8190
// ============================================================================
void ThreadLocalData::DeleteMyRoomReq(unsigned int dwOwnerUAID, UXMapID uxMapID)
{
    // IDA: Request delete my room
    // XSendPacket xSendPacket(0xF2, 0x59);
    // xSendPacket << dwOwnerUAID;
    // xSendPacket << uxMapID;
    // XGameServer::Instance()->m_controlSocket.SendCheck(&xSendPacket);
}

// ============================================================================
// DeleteMyRoomRes - IDA @ 0x1406D8250
// ============================================================================
void ThreadLocalData::DeleteMyRoomRes(UXMapID uxMapID)
{
    // IDA: Delete my room response - actually delete the room
    auto it = m_mapArea.find(uxMapID);
    if (it != m_mapArea.end())
    {
        XArea* pArea = it->second;
        if (pArea)
        {
            XMyRoom* pMyRoom = dynamic_cast<XMyRoom*>(pArea);
            if (pMyRoom)
            {
                pMyRoom->Clear();
                m_mapArea.erase(it);
                // m_xMyRoomPool->Push(pMyRoom);
            }
            // int nPoolSize = m_xMyRoomPool->GetCurSize();
            LogHelper::LogError("game.contents", "Delete MyRoom PoolSize: %d", 0);
        }
    }
}

// ============================================================================
// SendCashShopBanner - IDA @ 0x1406D86E0
// ============================================================================
void ThreadLocalData::SendCashShopBanner()
{
    // IDA: Send cash shop banner to all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            // pArea->SendCashShopBanner();
        }
    }
}

// ============================================================================
// CreateNpc - IDA @ 0x1406D8E60
// ============================================================================
CNpc* ThreadLocalData::CreateNpc(XArea* pArea, UXMapID uxMapID, int nSectorID, int nNpcID, XVec3 vPos, float fRot, bool bIsEvent)
{
    // IDA: Create NPC using NPC manager
    CNpc* pNpc = nullptr;
    if (m_xNpcMgr) {
        // pNpc = m_xNpcMgr->Create(uxMapID, nNpcID, &vPos, fRot);
    }

    if (pNpc)
    {
        pNpc->SetArea(pArea);
        pNpc->GenerateEventObject();

        // Check height and adjust position
        // hkvVec3 vCheckPos(vPos.x, vPos.y, vPos.z);
        // if (pNpc->GetHeight(&vCheckPos, 200.0f)) {
        //     vPos.z = vCheckPos.z;
        //     pNpc->Move(&vPos);
        // }
    }

    return pNpc;
}

// ============================================================================
// CreateAkashicObject - IDA @ 0x1406D8FB0
// IDA: ?CreateAkashicObject@ThreadLocalData@@QEAAPEAVCAkashicObject@@PEAVXArea@@TUXMapID@@HUXVec3@@MK@Z
// ============================================================================
CAkashicObject* ThreadLocalData::CreateAkashicObject(XArea* pArea, UXMapID uxMapID, int nAkashicID, XVec3* vPos, float fRot, unsigned int dwParentID)
{
    // IDA 0x1406D8FB0: 精确还原
    // qmemcpy(&v9, vPos, sizeof(v9))
    XVec3 vCopy = *vPos;

    // IDA: pAkashic = XAkashicObjectMgr::Create(&this->m_xAkashicMgr, uxMapID, nAkashicID, &v9, fRot, dwParentID)
    CAkashicObject* pAkashic = nullptr;
    if (m_xAkashicMgr) {
        pAkashic = m_xAkashicMgr->Create(uxMapID, nAkashicID, &vCopy, fRot, dwParentID);
    }

    // IDA: if (pAkashic) { v10 = pAkashic->CMoverEx::CMover::XActor::IXObject::__vftable; v10->SetArea(&pAkashic->XActor, pArea); }
    if (pAkashic)
    {
        pAkashic->SetArea(pArea);
    }

    return pAkashic;
}

// ============================================================================
// ReloadMazeResource - IDA @ 0x1406D90D0
// ============================================================================
bool ThreadLocalData::ReloadMazeResource(int nMazeID)
{
    // IDA: Reload maze resource and script
    // XGameServer* pServer = XGameServer::Instance();
    // TB_MAZE_INFO* pMaze = pServer->m_xResourceMgr.GetTB_MAZE_INFO(nMazeID);
    // if (!pMaze) return false;

    // // Build script path
    // std::string strPath = g_strCurPath + "/Scripts";
    // char szFilePath[256];
    // sprintf(szFilePath, "%s/Server/%s.lua", strPath.c_str(), pMaze->Server_SceneScript_File);

    // // Remove old resource if exists
    // VManagedResource* pResource = m_xScriptManager->GetResourceByName(szFilePath);
    // if (pResource) {
    //     m_xScriptManager->RemoveResource(pResource);
    //     LogHelper::LogInfo("game.system", "[RELOAD] Maze Script %s", pMaze->Server_SceneScript_File);
    // }

    // // Reload world resource
    // return XWorldResMgr::Reload(nMazeID);
    return true;
}

// ============================================================================
// UpdateChannel - IDA @ 0x1406D6920
// ============================================================================
void ThreadLocalData::UpdateChannel(std::uint16_t wMapID, ST_CHANNEL_INFO& stChannel)
{
    // IDA: Update specific channel info
    auto it = m_mapChannelInfo.find(wMapID);
    if (it != m_mapChannelInfo.end())
    {
        // Find and update the specific channel entry
        // auto& channelVec = it->second;
        // for (size_t i = 0; i < channelVec.size(); ++i) {
        //     if (channelVec[i].wChannel == stChannel.wChannel) {
        //         channelVec[i].byChannelState = stChannel.byChannelState;
        //         break;
        //     }
        // }
    }
}

// ============================================================================
// SendLeagueJoinUser_Apply - IDA @ 0x1406D7300
// ============================================================================
void ThreadLocalData::SendLeagueJoinUser_Apply(ST_LEAGUE_MEMBER_EX& stMemberEx, ST_LEAGUE_INFO_UPDATE& stInfoUpdate, int nSyncCount)
{
    // IDA: Find league member and call JoinLeagueUser
    auto it = m_mapLeagueMember.find(stMemberEx.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->JoinLeagueUser(stMemberEx, stInfoUpdate, true);
        }
    }
}

// ============================================================================
// SendLoginLeagueMember - IDA @ 0x1406D73F0
// ============================================================================
void ThreadLocalData::SendLoginLeagueMember(ST_LEAGUE_MEMBER_UPDATE& stUpdate)
{
    // IDA: Find league member and call LoginLeagueMember
    auto it = m_mapLeagueMember.find(stUpdate.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->LoginLeagueMember(stUpdate);
        }
    }
}

// ============================================================================
// DeleteLeague - IDA @ 0x1406D74A0
// ============================================================================
void ThreadLocalData::DeleteLeague(unsigned int nLeagueID, unsigned int dwUCID)
{
    // IDA: Find and delete league member
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->DeleteLeagueMember(dwUCID);
        }
        m_mapLeagueMember.erase(it);
    }
}

// ============================================================================
// SendLeagueApply - IDA @ 0x1406D7540
// ============================================================================
void ThreadLocalData::SendLeagueApply(ST_LEAGUE_APPLICANT& stApplicant)
{
    // IDA: Find league member and send apply notification
    auto it = m_mapLeagueMember.find(stApplicant.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->SendLeagueApply(stApplicant);
        }
    }
}

// ============================================================================
// SendDeleteLeagueApplicant - IDA @ 0x1406D75F0
// ============================================================================
void ThreadLocalData::SendDeleteLeagueApplicant(unsigned int nLeagueID, unsigned int dwUCID, int nSyncCount)
{
    // IDA: Find league member and delete applicant
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->LeagueApplicantDelete(dwUCID);
        }
    }
}

// ============================================================================
// SendLeagueBoard - IDA @ 0x1406D7690
// ============================================================================
void ThreadLocalData::SendLeagueBoard(ST_LEAGUE_BOARD& stBoard, unsigned int dwUCID, unsigned int nLeagueID)
{
    // IDA: Find league member and send board
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->SendLeagueBoard(stBoard, dwUCID);
        }
    }
}

// ============================================================================
// LeagueInfoChange - IDA @ 0x1406D7750
// ============================================================================
void ThreadLocalData::LeagueInfoChange(ST_LEAGUE_INFO& stInfo)
{
    // IDA: Find league member and send info change
    auto it = m_mapLeagueMember.find(stInfo.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->SendLeagueInfo(stInfo);
        }
    }
}

// ============================================================================
// ChangeLeagueAuth - IDA @ 0x1406D7800
// ============================================================================
void ThreadLocalData::ChangeLeagueAuth(ST_LEAGUE_AUTH_CHANGE& stChange, unsigned int nLeagueID, int nSyncCount)
{
    // IDA: Find league member and change auth
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->ChangeLeagueAuth(stChange);
        }
    }
}

// ============================================================================
// ChangePositionName - IDA @ 0x1406D78D0
// ============================================================================
void ThreadLocalData::ChangePositionName(ST_LEAGUE_POSITION_NAME_CHANGE& stChange, unsigned int nLeagueID)
{
    // IDA: Find league member and change position name
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->ChangeLeaguePositionName(stChange);
        }
    }
}

// ============================================================================
// UpdateMemberPosition - IDA @ 0x1406D7980
// ============================================================================
void ThreadLocalData::UpdateMemberPosition(ST_LEAGUE_MEMBER_POSITION& stPosition, unsigned int nLeagueID, unsigned int dwActorID, int nSyncCount)
{
    // IDA: Find league member and update position
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->ChangeLeagueMemberPosition(stPosition, dwActorID);
        }
    }
}

// ============================================================================
// SendLeagueWithdraw - IDA @ 0x1406D7A40
// ============================================================================
void ThreadLocalData::SendLeagueWithdraw(unsigned int nLeagueID, unsigned int dwReqUCID, ST_LEAGUE_INFO_UPDATE& stInfoUpdate, int nSyncCount)
{
    // IDA: Find league member and process withdraw
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->WithDrawLeagueMember(dwReqUCID, stInfoUpdate);
        }
    }
}

// ============================================================================
// UpdateLeagueMember - IDA @ 0x1406D7B10
// ============================================================================
void ThreadLocalData::UpdateLeagueMember(ST_LEAGUE_MEMBER_UPDATE& stUpdate)
{
    // IDA: Find league member and update
    auto it = m_mapLeagueMember.find(stUpdate.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateLeagueMember(stUpdate);
        }
    }
}

// ============================================================================
// SendLeagueMsg - IDA @ 0x1406D7BC0
// ============================================================================
void ThreadLocalData::SendLeagueMsg(PS_CHAT_LEAGUE& stChat, PS_CHAT_ITEM_LINK_FOR_SERVER& stItemLink)
{
    // IDA: Find league member and send chat message
    auto it = m_mapLeagueMember.find(stChat.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->SendLeagueMsg(stChat, stItemLink);
        }
    }
}

// ============================================================================
// LeagueApplicantUpdate - IDA @ 0x1406D7CC0
// ============================================================================
void ThreadLocalData::LeagueApplicantUpdate(const ST_LEAGUE_APPLICANT_CHECK_LIST& stApplicant, unsigned int dwOwnerUCID)
{
    // IDA: Find league member and update applicant
    auto it = m_mapLeagueMember.find(stApplicant.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->LeagueApplicantUpdate(stApplicant, dwOwnerUCID);
        }
    }
}

// ============================================================================
// SendTimeEvent - IDA @ 0x1406D85A0
// ============================================================================
void ThreadLocalData::SendTimeEvent(ST_GM_TIME_EVENT_INFO& stInfo)
{
    // IDA: Send time event to all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->SendTimeEvent(stInfo);
        }
    }
}

// ============================================================================
// SendServerOption - IDA @ 0x1406D8640
// ============================================================================
void ThreadLocalData::SendServerOption(PS_CONTENTS_INFO& stServerOptionInfo)
{
    // IDA: Send server option to all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->SendServerOption(stServerOptionInfo);
        }
    }
}

// ============================================================================
// SendCashShopItem - IDA @ 0x1406D8770
// ============================================================================
void ThreadLocalData::SendCashShopItem(STCashItemList& stCashshopList)
{
    // IDA: Send cash shop item to all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->SendCashShopItem(stCashshopList);
        }
    }
}

// ============================================================================
// SendCashShopTab - IDA @ 0x1406D8810
// ============================================================================
void ThreadLocalData::SendCashShopTab(ST_CASH_SHOP_TAB_LIST& stList)
{
    // IDA: Send cash shop tab to all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->SendCashShopTab(stList);
        }
    }
}

// ============================================================================
// UpdateReportInfo - IDA @ 0x1406D9230
// ============================================================================
void ThreadLocalData::UpdateReportInfo()
{
    // IDA: Update report pool info for monitoring
    SS_REPORT_POOL_INFO& info = m_ReportPoolInfo;

    // Get pool sizes from all managers
    if (m_xMonsterMgr) {
        info.nMonsterCurSize = m_xMonsterMgr->GetCurMaxQueue();
        info.nMonsterFullSize = m_xMonsterMgr->GetMaxQueueSize();
    }
    if (m_xNpcMgr) {
        info.nNpcCurSize = m_xNpcMgr->GetCurMaxQueue();
        info.nNpcFullSize = m_xNpcMgr->GetMaxQueueSize();
    }
    if (m_xAkashicMgr) {
        info.nAkashicCurSize = m_xAkashicMgr->GetCurMaxQueue();
        info.nAkashicFullSize = m_xAkashicMgr->GetMaxQueueSize();
    }
    if (m_xProjectileMgr) {
        info.nProjectileCurSize = m_xProjectileMgr->GetCurMaxQueue();
        info.nProjectileFullSize = m_xProjectileMgr->GetMaxQueueSize();
    }
    if (m_xTrapMgr) {
        info.nTrapCurSize = m_xTrapMgr->GetCurMaxQueue();
        info.nTrapFullSize = m_xTrapMgr->GetMaxQueueSize();
    }
    if (m_xChainLightningMgr) {
        info.nChainLightningCurSize = m_xChainLightningMgr->GetCurMaxQueue();
        info.nChainLightningFullSize = m_xChainLightningMgr->GetMaxQueueSize();
    }
    if (m_xInteractionObjectMgr) {
        info.nInteractionCurSize = m_xInteractionObjectMgr->GetCurMaxQueue();
        info.nInteractionFullSize = m_xInteractionObjectMgr->GetMaxQueueSize();
    }
    if (m_xVaccumCubeObjectMgr) {
        info.nVaccumCubeCurSize = m_xVaccumCubeObjectMgr->GetCurMaxQueue();
        info.nVaccumCubeFullSize = m_xVaccumCubeObjectMgr->GetMaxQueueSize();
    }
    if (m_xSocialItemObjectMgr) {
        info.nSocialCurSize = m_xSocialItemObjectMgr->GetCurMaxQueue();
        info.nSocialFullSize = m_xSocialItemObjectMgr->GetMaxQueueSize();
    }

    // Get pool sizes from maze and myroom pools
    if (m_xMazePool) {
        info.nMazeCurSize = m_xMazePool->GetCurMaxSize();
        info.nMazeFullSize = m_xMazePool->GetFullSize();
    }
    if (m_xMyRoomPool) {
        info.nMyRoomCurSize = m_xMyRoomPool->GetCurMaxSize();
        info.nMyRoomFullSize = m_xMyRoomPool->GetFullSize();
    }

    // TODO: Send to observe socket
    // XGameServer* pServer = XGameServer::Instance();
    // SS_REPORT_POOL_INFO* pPoolInfo = pServer->m_scObserveSocket.GetPoolInfo();
    // *pPoolInfo = info;
}

// ============================================================================
// SendLeagueNoticeChangeToMember - IDA @ 0x1406D9540
// ============================================================================
void ThreadLocalData::SendLeagueNoticeChangeToMember(ST_LEAGUE_NOTICE& stNotice, unsigned int dwReqUCID)
{
    // IDA: Find league member and send notice
    auto it = m_mapLeagueMember.find(stNotice.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->SendLeagueNotice(stNotice, dwReqUCID);
        }
    }
}

// ============================================================================
// SendLeagueRecruitNoticeToMember - IDA @ 0x1406D9600
// ============================================================================
void ThreadLocalData::SendLeagueRecruitNoticeToMember(ST_LEAGUE_RECRUIT_NOTICE& stRecruitNoticeInfo, __int64 biRemainTime)
{
    // IDA: Find league member and send recruit notice
    auto it = m_mapLeagueMember.find(stRecruitNoticeInfo.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->SendLeagueRecruitNotice(stRecruitNoticeInfo, biRemainTime);
        }
    }
}

// ============================================================================
// SendLeagueJoinUser_Invite - IDA @ 0x1406D96C0
// ============================================================================
void ThreadLocalData::SendLeagueJoinUser_Invite(ST_LEAGUE_MEMBER_EX& stMemberEx, ST_LEAGUE_INFO_UPDATE& stInfoUpdate, unsigned char byApplyState, int nSyncCount)
{
    // IDA: Find league member and process invite
    auto it = m_mapLeagueMember.find(stMemberEx.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->JoinLeagueUser(stMemberEx, stInfoUpdate, byApplyState);
        }
    }
}

// ============================================================================
// SendLeagueKickout - IDA @ 0x1406D97C0
// ============================================================================
void ThreadLocalData::SendLeagueKickout(unsigned int nLeagueID, unsigned int dwReqUCID, unsigned int dwKickoutUCID, ST_LEAGUE_INFO_UPDATE& stUpdateInfo, int nSyncCount)
{
    // IDA: Find league member and process kickout
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->KickoutLeagueMember(dwReqUCID, dwKickoutUCID, stUpdateInfo);
        }
    }
}

// ============================================================================
// SendLeagueRecordUpdate - IDA @ 0x1406D9890
// ============================================================================
void ThreadLocalData::SendLeagueRecordUpdate(ST_LEAGUE_RECORD& stRecordInfo)
{
    // IDA: Find league member and update record
    auto it = m_mapLeagueMember.find(stRecordInfo.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->Record(stRecordInfo);
        }
    }
}

// ============================================================================
// SendLeagueCardChange - IDA @ 0x1406D9940
// ============================================================================
void ThreadLocalData::SendLeagueCardChange(PS_REQ_LEAGUE_CARD& psCardInfo, int nSyncCount)
{
    // IDA: Find league member and change card
    auto it = m_mapLeagueMember.find(psCardInfo.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->CardChange(psCardInfo);
        }
    }
}

// ============================================================================
// SendLeagueWealth - IDA @ 0x1406D99F0
// ============================================================================
void ThreadLocalData::SendLeagueWealth(ST_LEAGUE_INFO_UPDATE& stUpdateInfo)
{
    // IDA: Find league member and update wealth
    auto it = m_mapLeagueMember.find(stUpdateInfo.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->Wealth(stUpdateInfo);
        }
    }
}

// ============================================================================
// SendLeagueLevelUp - IDA @ 0x1406D9AA0
// ============================================================================
void ThreadLocalData::SendLeagueLevelUp(unsigned int nLeagueID, unsigned char byLevel, unsigned char bySkillPoint, PS_AUTO_SKILL& psAutoSkillInfo, int nSyncCount)
{
    // IDA: Find league member and send level up
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->Levelup(nLeagueID, byLevel, bySkillPoint, psAutoSkillInfo);
        }
    }
}

// ============================================================================
// SendLeagueSkillLearn - IDA @ 0x1406D9B60
// ============================================================================
void ThreadLocalData::SendLeagueSkillLearn(PS_RES_LEAGUE_SKILL& psSkillInfo, int nSyncCount)
{
    // IDA: Find league member and send skill learn
    auto it = m_mapLeagueMember.find(psSkillInfo.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->SkillLearn(psSkillInfo);
        }
    }
}

// ============================================================================
// SendChannelInfo - IDA @ 0x1406D6A50
// ============================================================================
void ThreadLocalData::SendChannelInfo(CUser* pUser, std::uint16_t wMapID)
{
    // IDA: Find channel info and send to user
    auto it = m_mapChannelInfo.find(wMapID);
    if (it != m_mapChannelInfo.end())
    {
        // XSendPacket xSendPacket(0xF1, 1);
        // xSendPacket << it->second;
        // pUser->XActor.Send(&xSendPacket);
    }
}

// ============================================================================
// MyRoomLoad - IDA @ 0x1406D8390
// ============================================================================
void ThreadLocalData::MyRoomLoad(int nResult, unsigned int dwOwnerUAID, UXMapID uxMapID, ST_MYROOM_USER& stCreateUser, ST_MYROOM_ITEM_LIST& stMyRoomItemList, unsigned int dwOwnerUCID)
{
    // IDA: Load my room with items
    XArea* pArea = FindArea(uxMapID);
    if (pArea)
    {
        if (!nResult)
        {
            XMyRoom* pMyRoom = dynamic_cast<XMyRoom*>(pArea);
            if (pMyRoom)
            {
                pMyRoom->LoadMyRoomSucc(stMyRoomItemList);
                pMyRoom->EnterRoom(stCreateUser);
            }
        }

        // Send response to control socket
        // XSendPacket xSendPacket(0xF2, 0x52);
        // xSendPacket << nResult;
        // xSendPacket << dwOwnerUAID;
        // xSendPacket << stCreateUser;
        // xSendPacket << dwOwnerUCID;
        // XGameServer::Instance()->m_controlSocket.SendCheck(&xSendPacket);
    }
    else
    {
        LogHelper::LogError("game.contents", "MyRoomLoad error - Cant find area UCID: %d", stCreateUser.nPoint);
    }
}

// ============================================================================
// CreateModeMaze - IDA @ 0x1406D2F70
// ============================================================================
void ThreadLocalData::CreateModeMaze(ST_CREATE_MAZE& stCreateMaze)
{
    // IDA: Create mode maze from pool
    XModeMaze* pModeMaze = nullptr;
    // TODO: m_xModeMazePool->Pop(&pModeMaze);

    if (pModeMaze)
    {
        ++m_nModeMazeCount;

        if (pModeMaze->Create(&stCreateMaze))
        {
            pModeMaze->SetPartyInfo(stCreateMaze.stPartyInfo);
            pModeMaze->SetEnterDistrictPos(&stCreateMaze.stEnterDistrictPos);
            AddArea(pModeMaze);

            size_t memberCount = stCreateMaze.vecEnterMember.size();
            LogHelper::LogDebug("game.contents",
                "< %I64d MODEMAZE > Success Create ModeMaze USER( %d ), MAP ( %d ) , type( %d ), Party( %d ), EnterMember ( %d ) ",
                stCreateMaze.uxMapID.nMapID, stCreateMaze.dwUserID, stCreateMaze.uxMapID.nMapID,
                stCreateMaze.stPartyInfo.byGroupType, stCreateMaze.stPartyInfo.nID, memberCount);
        }
        else
        {
            stCreateMaze.nResult = 50007;
            // m_xModeMazePool->Push(pModeMaze);
        }
    }
    else
    {
        stCreateMaze.nResult = 50006;
        // int curSize = m_xModeMazePool->GetCurSize();
        LogHelper::LogError("game.contents",
            "CreateModeMaze error - Failed create maze no pool[ MapID:%I64d, CurPool:%d ] ( %d )",
            stCreateMaze.uxMapID.nMapID, 0, 493);
    }

    LogHelper::LogInfo("game.contents", "<CREATE_MODE_MAZE> Create %d Maze ( UCID : %d / %I64d ) - Result : %d",
        (int)(stCreateMaze.uxMapID.nMapID >> 32), stCreateMaze.dwUserID,
        stCreateMaze.uxMapID.nMapID, stCreateMaze.nResult);

    // Log to DB
    ST_LOG_GAME stLog;
    stLog._nUAID = stCreateMaze.dwUAID;
    stLog._nUCID = stCreateMaze.dwUserID;
    stLog._sMainType = 5;
    stLog._sSubType = 1;
    stLog.nParam0 = (int)(stCreateMaze.uxMapID.nMapID >> 32);
    stLog.nParam1 = (int)stCreateMaze.vecEnterMember.size();
    stLog.nParam4 = stCreateMaze.nResult;
    stLog.nParam6 = stCreateMaze.uxMapID.nMapID;
    // wcscpy_s(stLog.szComment, L"Create Mode Maze");
    // XGameServer::Instance()->SendDBLog(&stLog);

    // Send to control socket
    // XSendPacket xSendPacket(0xF2, 0x22);
    // xSendPacket << stCreateMaze;
    // XGameServer::Instance()->m_controlSocket.SendCheck(&xSendPacket);
}

// ============================================================================
// DeleteModeMaze - IDA @ 0x1406D35E0
// ============================================================================
void ThreadLocalData::DeleteModeMaze(UXMapID uxMapID)
{
    // IDA: Find and delete mode maze
    auto it = m_mapArea.find(uxMapID);
    if (it == m_mapArea.end())
    {
        LogHelper::LogError("game.contents", "DeleteModeMaze error - Cant find ModeMaze[ MapID:%I64d ] ( %d )",
            uxMapID.nMapID, 595);
        return;
    }

    XArea* pArea = it->second;
    if (!pArea)
    {
        LogHelper::LogError("game.contents", "DeleteModeMaze error - Invalid Area[ MapID:%I64d ] ( %d )",
            uxMapID.nMapID, 603);
        return;
    }

    XModeMaze* pModeMaze = dynamic_cast<XModeMaze*>(pArea);
    if (!pModeMaze)
    {
        LogHelper::LogError("game.contents", "DeleteModeMaze error - Invalid Maze[ MapID:%I64d ] ( %d )",
            uxMapID.nMapID, 610);
        return;
    }

    // Check if there are still users in maze
    int nUserCount = pModeMaze->GetCurUserCount();
    if (nUserCount > 0)
    {
        ST_LOG_GAME stLogGame;
        stLogGame._nUAID = 0;
        stLogGame._nUCID = 0;
        stLogGame._sMainType = 5;
        stLogGame._sSubType = 2;
        stLogGame.nParam0 = pArea->GetTBMapID();
        stLogGame.nParam1 = nUserCount;
        stLogGame.nParam6 = uxMapID.nMapID;
        // wcscpy_s(stLogGame.szComment, L"User in ModeMaze");
        // XGameServer::Instance()->SendDBLog(&stLogGame);

        LogHelper::LogError("game.contents",
            "DeleteModeMaze error - there is user in maze[ MapID:%I64d, UserCount:%d ] ( %d )",
            uxMapID.nMapID, nUserCount, 628);
        return;
    }

    bool bClearMaze = pModeMaze->IsCompleteClearMaze();
    unsigned short wMapID = pArea->GetTBMapID();
    int nEnterCount = pModeMaze->GetMazeEnterCount();

    // Clear and remove
    pArea->Clear();
    m_mapArea.erase(it);

    // Return to pool
    // m_xModeMazePool->Push(pModeMaze);
    // int nPoolSize = m_xModeMazePool->GetCurSize();

    ST_LOG_GAME stLog;
    stLog._nUAID = 0;
    stLog._nUCID = 0;
    stLog._sMainType = 5;
    stLog._sSubType = 2;
    stLog.nParam0 = wMapID;
    stLog.nParam1 = nEnterCount;
    stLog.nParam2 = bClearMaze;
    // stLog.nParam5 = nPoolSize;
    stLog.nParam6 = uxMapID.nMapID;
    // wcscpy_s(stLog.szComment, L"Delete Mode Maze");
    // XGameServer::Instance()->SendDBLog(&stLog);
}

// ============================================================================
// LoadAllScript - IDA @ 0x1406D5E80
// ============================================================================
void ThreadLocalData::LoadAllScript()
{
    // IDA: Load all Lua scripts for AI
    // TODO: Get monster table from XGameServer
    // XGameServer* pServer = XGameServer::Instance();
    // auto& mapMonster = pServer->m_mapMonsterTable;

    // for (auto& pair : mapMonster) {
    //     TB_MONSTER* pMonster = &pair.second;
    //
    //     // Load main AI script
    //     if (strlen(pMonster->szAIScript) > 0) {
    //         char szFilePath[256];
    //         sprintf(szFilePath, "\\AI\\%s.lua", pMonster->szAIScript);
    //         VScriptResource* pScript = m_xScriptManager->LoadScriptFile(szFilePath);
    //         if (!pScript || !pScript->IsLoaded()) {
    //             XPRINT(" [ %s ] Missing LuaScript Resource Load fail ", szFilePath);
    //         }
    //     }
    //
    //     // Load additional AI scripts
    //     for (int i = 0; i < 3; ++i) {
    //         if (strlen(pMonster->szAdditionalScript[i]) > 1) {
    //             char szBuffer[256];
    //             sprintf(szBuffer, "\\AI\\%s.lua", pMonster->szAdditionalScript[i]);
    //             VScriptResource* pScript = m_xScriptManager->LoadScriptFile(szBuffer);
    //             if (!pScript || !pScript->IsLoaded()) {
    //                 XPRINT(" [ %s ] Missing LuaScript Resource Load fail ", szBuffer);
    //             }
    //         }
    //     }
    // }
}

// ============================================================================
// CreateMatchingMaze - IDA @ 0x1406D2450
// ============================================================================
void ThreadLocalData::CreateMatchingMaze(ST_CREATE_MAZE& stCreateMaze, PS_PARTY_INFO& stPartyInfo, unsigned int dwMatchingID)
{
    // IDA: Create matching maze with party info
    XMaze* pMaze = nullptr;
    // TODO: m_xMazePool->Pop(&pMaze);

    if (pMaze)
    {
        if (pMaze->Create(&stCreateMaze))
        {
            ST_PARTY_INFO stPartyGroupInfo;
            memset(&stPartyGroupInfo, 0, sizeof(stPartyGroupInfo));
            stPartyGroupInfo.byGroupType = 0;
            stPartyGroupInfo.nID = 0;
            pMaze->SetPartyInfo(stPartyGroupInfo);
            AddArea(pMaze);

            // Add party
            ThreadLocalData* pInstance = GetInstance();
            // std::tr1::shared_ptr<CParty> pParty = pInstance->m_xPartyMgr->AddParty(stPartyInfo);

            // if (pParty) {
            //     stPartyGroupInfo.byGroupType = 1;
            //     stPartyGroupInfo.nID = pParty->GetPartyID();
            //     stCreateMaze.stPartyInfo = stPartyGroupInfo;
            //     pParty->SetMazeID(stCreateMaze.uxMapID);
            //     pMaze->SetPartyInfo(stPartyGroupInfo);
            //     pMaze->SetParty(pParty);
            // }

            // TODO: Queue job to logic thread manager
            // CLogicThreadManager::Instance()->DoJobAllThread([&]() {
            //     // Process matching maze creation
            // });
        }
        else
        {
            stCreateMaze.nResult = 50007;
            // m_xMazePool->Push(pMaze);
        }
    }
    else
    {
        stCreateMaze.nResult = 50006;
        // int curSize = m_xMazePool->GetCurSize();
        LogHelper::LogError("game.contents",
            "CreateMatchingMaze error - Failed create maze no pool[ MapID:%I64d, CurPool:%d ] ( %d )",
            stCreateMaze.uxMapID.nMapID, 0, 334);
    }

    LogHelper::LogInfo("game.contents", "<CREATE_MAZE> Create %d Maze ( UCID : %d / %I64d ) - Result : %d",
        (int)(stCreateMaze.uxMapID.nMapID >> 32), stCreateMaze.dwUserID,
        stCreateMaze.uxMapID.nMapID, stCreateMaze.nResult);

    // Log to DB
    ST_LOG_GAME stLog;
    stLog._nUAID = stCreateMaze.dwUAID;
    stLog._nUCID = stCreateMaze.dwUserID;
    stLog._sMainType = 5;
    stLog._sSubType = 1;
    stLog.nParam0 = (int)(stCreateMaze.uxMapID.nMapID >> 32);
    stLog.nParam1 = (int)stCreateMaze.vecEnterMember.size();
    stLog.nParam4 = stCreateMaze.nResult;
    stLog.nParam6 = stCreateMaze.uxMapID.nMapID;
    // wcscpy_s(stLog.szComment, L"Create Matching Maze");
    // XGameServer::Instance()->SendDBLog(&stLog);

    // Send to control socket
    // XSendPacket xSendPacket(0xF2, 0x41);
    // xSendPacket << dwMatchingID;
    // xSendPacket << stCreateMaze;
    // xSendPacket << stPartyInfo;
    // XGameServer::Instance()->m_controlSocket.SendCheck(&xSendPacket);
}

// ============================================================================
// CreateMonster - IDA @ 0x1406D88B0
// ============================================================================
CMonster* ThreadLocalData::CreateMonster(XArea* pArea, UXMapID uxMapID, int nMonsterID, XVec3 vPos, float fRot)
{
    // IDA: Create monster using monster manager
    CMonster* pMonster = nullptr;
    if (m_xMonsterMgr) {
        pMonster = m_xMonsterMgr->Create(uxMapID, nMonsterID, &vPos, fRot);
    }

    if (pMonster)
    {
        ++m_nMonsterCount;
        pMonster->SetArea(pArea);
        pMonster->GenerateEventObject();

        // Check height and adjust position
        // hkvVec3 vCheckPos(vPos.x, vPos.y, vPos.z);
        // if (pMonster->GetHeight(&vCheckPos, 200.0f)) {
        //     vPos.z = vCheckPos.z;
        //     pMonster->Move(&vPos);
        // }

        pMonster->SetInitYaw(fRot);
    }

    return pMonster;
}

// ============================================================================
// DeleteMonster - IDA @ 0x1406D8A30
// ============================================================================
void ThreadLocalData::DeleteMonster(CMonster* pMonster)
{
    // IDA: Delete monster using monster manager
    if (!pMonster) return;
    if (m_xMonsterMgr) {
        m_xMonsterMgr->Delete(pMonster);
    }
}

// ============================================================================
// CreateProjectile - IDA @ 0x1406D8A60
// ============================================================================
VGameProjectileObject* ThreadLocalData::CreateProjectile(XArea* pArea, UXMapID uxMapID, XVec3 vPos)
{
    // IDA: Create projectile using projectile manager
    VGameProjectileObject* pProjectile = nullptr;
    if (m_xProjectileMgr) {
        pProjectile = m_xProjectileMgr->Create(&vPos);
    }

    if (pProjectile)
    {
        pProjectile->Init();
        pProjectile->SetArea(pArea);
    }

    return pProjectile;
}

// ============================================================================
// DeleteProjectile - IDA @ 0x1406D8AF0
// ============================================================================
void ThreadLocalData::DeleteProjectile(VGameProjectileObject* pProjectile)
{
    // IDA: Delete projectile using projectile manager
    if (!pProjectile) return;
    if (m_xProjectileMgr) {
        m_xProjectileMgr->Delete(pProjectile);
    }
}

// ============================================================================
// CreateTrap - IDA @ 0x1406D8B20
// ============================================================================
VGameTrapObject* ThreadLocalData::CreateTrap(XArea* pArea, UXMapID uxMapID, XVec3 vPos)
{
    // IDA: Create trap using trap manager
    VGameTrapObject* pTrap = nullptr;
    if (m_xTrapMgr) {
        pTrap = m_xTrapMgr->Create(&vPos);
    }

    if (pTrap)
    {
        pTrap->Init();
        pTrap->SetArea(pArea);

        if (pArea)
        {
            ThreadLocalData* pInstance = GetInstance();
            if (pInstance)
            {
                unsigned short wMapID = pArea->GetTBMapID();
                pTrap->SetLogData(wMapID, pInstance->m_nOwnerThreadIndex);
            }
        }
    }

    return pTrap;
}

// ============================================================================
// DeleteTrap - IDA @ 0x1406D8BF0
// ============================================================================
void ThreadLocalData::DeleteTrap(VGameTrapObject* pTrap)
{
    // IDA: Delete trap using trap manager
    if (!pTrap) return;
    if (m_xTrapMgr) {
        m_xTrapMgr->Delete(pTrap);
    }
}

// ============================================================================
// CreateChainLightning - IDA @ 0x1406D8C20
// ============================================================================
VChainLightningObject* ThreadLocalData::CreateChainLightning(XArea* pArea, UXMapID uxMapID, XVec3 vPos)
{
    // IDA: Create chain lightning using chain lightning manager
    VChainLightningObject* pChainLightning = nullptr;
    if (m_xChainLightningMgr) {
        pChainLightning = m_xChainLightningMgr->Create(&vPos);
    }

    if (pChainLightning)
    {
        pChainLightning->Init();
        pChainLightning->SetArea(pArea);
    }

    return pChainLightning;
}

// ============================================================================
// DeleteChainLightning - IDA @ 0x1406D8CB0
// ============================================================================
void ThreadLocalData::DeleteChainLightning(VChainLightningObject* pChainLightning)
{
    // IDA: Delete chain lightning using chain lightning manager
    if (!pChainLightning) return;
    if (m_xChainLightningMgr) {
        m_xChainLightningMgr->Delete(pChainLightning);
    }
}

// ============================================================================
// CreateInteractionObject - IDA @ 0x1406D8CE0
// ============================================================================
CInteractionObject* ThreadLocalData::CreateInteractionObject(XVec3 vPos)
{
    // IDA: Create interaction object using interaction object manager
    if (m_xInteractionObjectMgr) {
        return m_xInteractionObjectMgr->Create(&vPos);
    }
    return nullptr;
}

// ============================================================================
// DeleteInteractionObject - IDA @ 0x1406D8D30
// ============================================================================
void ThreadLocalData::DeleteInteractionObject(CInteractionObject* pInteractionObject)
{
    // IDA: Delete interaction object using interaction object manager
    if (!pInteractionObject) return;
    if (m_xInteractionObjectMgr) {
        m_xInteractionObjectMgr->Delete(pInteractionObject);
    }
}

// ============================================================================
// CreateVaccumCubeObject - IDA @ 0x1406D8D60
// ============================================================================
CVaccumCube* ThreadLocalData::CreateVaccumCubeObject(XVec3 vPos)
{
    // IDA: Create vaccum cube using vaccum cube object manager
    if (m_xVaccumCubeObjectMgr) {
        return m_xVaccumCubeObjectMgr->Create(&vPos);
    }
    return nullptr;
}

// ============================================================================
// DeleteVaccumCubeObject - IDA @ 0x1406D8DB0
// ============================================================================
void ThreadLocalData::DeleteVaccumCubeObject(CVaccumCube* pVaccumCube)
{
    // IDA: Delete vaccum cube using vaccum cube object manager
    if (!pVaccumCube) return;
    if (m_xVaccumCubeObjectMgr) {
        m_xVaccumCubeObjectMgr->Delete(pVaccumCube);
    }
}

// ============================================================================
// CreateSocialItemObject - IDA @ 0x1406D8DE0
// ============================================================================
CSocialItemObject* ThreadLocalData::CreateSocialItemObject(XVec3 vPos)
{
    // IDA: Create social item object using social item object manager
    if (m_xSocialItemObjectMgr) {
        return m_xSocialItemObjectMgr->Create(&vPos);
    }
    return nullptr;
}

// ============================================================================
// DeleteSocialItemObject - IDA @ 0x1406D8E30
// ============================================================================
void ThreadLocalData::DeleteSocialItemObject(CSocialItemObject* pSocialItemObject)
{
    // IDA: Delete social item object using social item object manager
    if (!pSocialItemObject) return;
    if (m_xSocialItemObjectMgr) {
        m_xSocialItemObjectMgr->Delete(pSocialItemObject);
    }
}

// ============================================================================
// DeleteAkashicObject - IDA @ 0x1406D9070
// ============================================================================
void ThreadLocalData::DeleteAkashicObject(CAkashicObject* pAkashic)
{
    // IDA: Delete akashic object using akashic manager
    if (!pAkashic) return;
    if (m_xAkashicMgr) {
        m_xAkashicMgr->Delete(pAkashic);
    }
}

// ============================================================================
// DeleteNpc - IDA @ 0x1406D90A0
// ============================================================================
void ThreadLocalData::DeleteNpc(CNpc* pNpc)
{
    // IDA: Delete NPC using NPC manager
    if (!pNpc) return;
    if (m_xNpcMgr) {
        m_xNpcMgr->Delete(pNpc);
    }
}

// ============================================================================
// CreateMatchingMaze - IDA @ 0x1406D2960
// ============================================================================
void ThreadLocalData::CreateMatchingMaze(ST_CREATE_MAZE& stCreateMaze, PS_FORCE_INFO& stForceInfo, unsigned long dwMatchingID)
{
    // IDA: Create matching maze with force info
    XMaze* pMaze = m_xMazePool ? m_xMazePool->Pop() : nullptr;

    if (pMaze)
    {
        if (pMaze->Create(&stCreateMaze))
        {
            ST_PARTY_INFO stPartyGroupInfo = {};
            stPartyGroupInfo.byGroupType = 0;
            stPartyGroupInfo.nID = 0;
            pMaze->SetPartyInfo(stPartyGroupInfo);
            AddArea(pMaze);
            stCreateMaze.stPartyInfo = stPartyGroupInfo;

            // Add force
            std::shared_ptr<CForce> pForce;
            if (m_xForceMgr)
            {
                m_xForceMgr->AddForce(pForce, &stForceInfo);
            }

            if (pForce)
            {
                stPartyGroupInfo.byGroupType = 2;
                stPartyGroupInfo.nID = pForce->GetPartyID();
                stCreateMaze.stPartyInfo = stPartyGroupInfo;
                pForce->SetMazeID(stCreateMaze.uxMapID);
                pMaze->SetPartyInfo(stPartyGroupInfo);
                pMaze->SetForce(pForce);
            }

            // TODO: Call DoJobAllThread for force notification
            // CLogicThreadManager::DoJobAllThread(...);
        }
        else
        {
            stCreateMaze.nResult = 50007;
            if (m_xMazePool) m_xMazePool->Push(pMaze);
        }
    }
    else
    {
        stCreateMaze.nResult = 50006;
        // TODO: Log error - "CreateMatchingMaze error - Failed create maze no pool"
    }

    // TODO: Send response packet via control socket
}

// ============================================================================
// CreateMatchingModeMaze - IDA @ 0x1406D3340
// ============================================================================
void ThreadLocalData::CreateMatchingModeMaze(ST_CREATE_MODE_MAZE& stCreateMaze)
{
    // IDA: Create matching mode maze
    XModeMaze* pModeMaze = m_xModeMazePool ? m_xModeMazePool->Pop() : nullptr;

    if (pModeMaze)
    {
        ++m_nModeMazeCount;
        if (pModeMaze->Create(&stCreateMaze))
        {
            AddArea(pModeMaze);
            // Log success
        }
        else
        {
            stCreateMaze.nResult = 50007;
            if (m_xModeMazePool) m_xModeMazePool->Push(pModeMaze);
        }
    }
    else
    {
        stCreateMaze.nResult = 50006;
        // TODO: Log error - "CreateModeMaze error - Failed create mode maze no pool"
    }

    // TODO: Send response packet via control socket
}

// ============================================================================
// SendLeagueDelegate - IDA @ 0x1406DA1B0
// ============================================================================
void ThreadLocalData::SendLeagueDelegate(PS_RES_LEAGUE_DELEGATE& psDelegateRes, unsigned int dwDelegatedUCID, int nSyncCount)
{
    // IDA: Find league member and delegate
    auto it = m_mapLeagueMember.find(psDelegateRes.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->Delegate(&psDelegateRes, dwDelegatedUCID);
        }
    }
}

// ============================================================================
// SendLeagueChangeName - IDA @ 0x1406DA280
// ============================================================================
void ThreadLocalData::SendLeagueChangeName(PS_RES_LEAGUE_NAME_CHANGE& psResChangeInfo, int nSyncCount)
{
    // IDA: Find league member and change name
    auto it = m_mapLeagueMember.find(psResChangeInfo.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
            pMember->ChangeName(&psResChangeInfo);
        }
    }
}

// ============================================================================
// UpdateSyncLeagueLoad - IDA @ 0x1406DA340
// ============================================================================
void ThreadLocalData::UpdateSyncLeagueLoad(int nSyncCount)
{
    // IDA: Update sync count for all league members
    for (auto& pair : m_mapLeagueMember)
    {
        CLeagueMember* pMember = pair.second;
        if (pMember)
        {
            pMember->UpdateSyncCount(nSyncCount);
        }
    }
}

// ============================================================================
// SendLeagueInventoryMove - IDA @ 0x1406DA3C0
// ============================================================================
void ThreadLocalData::SendLeagueInventoryMove(unsigned int dwReqUCID, PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME& psItemMoveForServer)
{
    // IDA: Send league inventory move notification
    PS_RES_ITEM_MOVE_LEAGUE_INVEN psItemMoveInfo = psItemMoveForServer.psResItemMoveInfo;
    psItemMoveInfo.stItem = psItemMoveForServer.psStorageInfo.stItem;

    auto it = m_mapLeagueMember.find(psItemMoveForServer.psReqItemMoveInfo.nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            pMember->UpdateInventorySyncCount(psItemMoveForServer.nInventorySync);
            pMember->InventoryMove(dwReqUCID, &psItemMoveInfo);
        }
    }
}

// ============================================================================
// SendDayEvent - IDA @ 0x1406DA650
// ============================================================================
void ThreadLocalData::SendDayEvent(PS_DAY_EVENT_LIST& psDayEvent)
{
    // IDA: Send day event to all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->SendDayEvent(&psDayEvent);
        }
    }
}

// ============================================================================
// SendOperationTime - IDA @ 0x1406DA6F0
// ============================================================================
void ThreadLocalData::SendOperationTime()
{
    // IDA: Send operation time to all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->SendOperationTime();
        }
    }
}

// ============================================================================
// SendWorldEventBooster - IDA @ 0x1406DA780
// ============================================================================
void ThreadLocalData::SendWorldEventBooster(unsigned long dwBuff_ID, __int64 biEndDate)
{
    // IDA: Send world event booster to all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->SendWorldEventBooster(dwBuff_ID, biEndDate);
        }
    }
}

// ============================================================================
// CompareLeagueInventoryCount - IDA @ 0x1406DA820
// ============================================================================
bool ThreadLocalData::CompareLeagueInventoryCount(unsigned int nLeagueID, unsigned int dwUCID)
{
    // IDA: Compare league inventory count
    auto it = m_mapLeagueMember.find(nLeagueID);
    if (it != m_mapLeagueMember.end())
    {
        CLeagueMember* pMember = it->second;
        if (pMember)
        {
            return pMember->CompareInventorySyncCount(dwUCID);
        }
    }
    return false;
}

// ============================================================================
// StartWorldMode - IDA @ 0x1406DA940
// ============================================================================
void ThreadLocalData::StartWorldMode(ST_WORLD_MODE_INFO& stInfo)
{
    // IDA: Start world mode for all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->StartWorldMode(&stInfo);
        }
    }
}

// ============================================================================
// FinishWorldMode - IDA @ 0x1406DA9E0
// ============================================================================
void ThreadLocalData::FinishWorldMode(PS_WORLD_MODE_FINISH& stInfo)
{
    // IDA: Finish world mode for all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->FinishWorldMode(&stInfo);
        }
    }
}

// ============================================================================
// ClearWorldMode - IDA @ 0x1406DAA80
// ============================================================================
void ThreadLocalData::ClearWorldMode(ST_WORLD_MODE_INFO& stInfo)
{
    // IDA: Clear world mode for all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->ClearWorldMode(&stInfo);
        }
    }
}

// ============================================================================
// SyncWorldMode - IDA @ 0x1406DAB20
// ============================================================================
void ThreadLocalData::SyncWorldMode(ST_WORLD_MODE_INFO_VEC& stInfoVec)
{
    // IDA: Sync world mode for all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->SyncWorldMode(&stInfoVec);
        }
    }
}

// ============================================================================
// CompleteWorldMode - IDA @ 0x1406DACA0
// ============================================================================
void ThreadLocalData::CompleteWorldMode(PS_WORLD_MODE_COMPLETE& stComplete, unsigned int dwMonsterID)
{
    // IDA: Complete world mode for all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->CompleteWorldMode(&stComplete, dwMonsterID);
        }
    }
}

// ============================================================================
// MyRoomPollenLoad - IDA @ 0x1406D9C20
// ============================================================================
void ThreadLocalData::MyRoomPollenLoad(UXMapID uxMapID, PS_MYROOM_POLLEN_LIST* psPollenList)
{
    // IDA: Find area and load pollen data to MyRoom
    ThreadLocalData* pInstance = GetInstance();
    XArea* pArea = pInstance->FindArea(uxMapID);

    if (pArea)
    {
        XMyRoom* pMyRoom = dynamic_cast<XMyRoom*>(pArea);
        if (pMyRoom)
        {
            pMyRoom->PollenLoad(*psPollenList);
        }
    }
}

// ============================================================================
// MyRoomPollenAdd - IDA @ 0x1406D9D00
// ============================================================================
void ThreadLocalData::MyRoomPollenAdd(int nErrorCode, UXMapID uxMapID, int nPollenIndex)
{
    // IDA: Find area and add pollen to MyRoom
    ThreadLocalData* pInstance = GetInstance();
    XArea* pArea = pInstance->FindArea(uxMapID);

    if (pArea)
    {
        XMyRoom* pMyRoom = dynamic_cast<XMyRoom*>(pArea);
        if (pMyRoom)
        {
            if (nErrorCode)
            {
                // TODO: LogHelper::LogError("game.contents", "MyRoomPollenAdd ERROR(%d) ", nErrorCode);
            }
            else
            {
                pMyRoom->SendPollenAdd(nPollenIndex);
            }
        }
    }
}

// ============================================================================
// MyRoomPollenCultivation - IDA @ 0x1406D9DA0
// ============================================================================
void ThreadLocalData::MyRoomPollenCultivation(int nErrorCode, UXMapID uxMapID, PS_DB_MYROOM_POLLEN_CULTIVATION& psInfo)
{
    // IDA: Find area and send pollen cultivation to MyRoom
    ThreadLocalData* pInstance = GetInstance();
    XArea* pArea = pInstance->FindArea(uxMapID);

    if (pArea)
    {
        XMyRoom* pMyRoom = dynamic_cast<XMyRoom*>(pArea);
        if (pMyRoom)
        {
            if (nErrorCode)
            {
                // TODO: LogHelper::LogError("game.contents", "MyRoomPollenCultivation ERROR(%d) ", nErrorCode);
            }
            else
            {
                pMyRoom->SendPollenCultivation(&psInfo);
            }
        }
    }
}

// ============================================================================
// MyRoomPollenHarvest - IDA @ 0x1406D9E40
// ============================================================================
void ThreadLocalData::MyRoomPollenHarvest(int nErrorCode, UXMapID uxMapID, int nPollenIndex)
{
    // IDA: Find area and harvest pollen from MyRoom
    ThreadLocalData* pInstance = GetInstance();
    XArea* pArea = pInstance->FindArea(uxMapID);

    if (pArea)
    {
        XMyRoom* pMyRoom = dynamic_cast<XMyRoom*>(pArea);
        if (pMyRoom)
        {
            if (nErrorCode)
            {
                // TODO: LogHelper::LogError("game.contents", "MyRoomPollenCultivation ERROR(%d) ", nErrorCode);
            }
            else
            {
                pMyRoom->SendPollenHarvest(nPollenIndex);
            }
        }
    }
}

// ============================================================================
// MyRoomPollenHelp - IDA @ 0x1406D9EE0
// ============================================================================
void ThreadLocalData::MyRoomPollenHelp(int nErrorCode, UXMapID uxMapID, int nPollenIndex,
    PS_MYROOM_POLLEN_HELP_USER* psHelpUser, unsigned __int64 biHarvestDate, unsigned int dwOwnerUAID)
{
    // IDA: Find area and send pollen help notification
    if (!nErrorCode)
    {
        // TODO: Send packet to community socket
    }

    ThreadLocalData* pInstance = GetInstance();
    XArea* pArea = pInstance->FindArea(uxMapID);

    if (pArea)
    {
        XMyRoom* pMyRoom = dynamic_cast<XMyRoom*>(pArea);
        if (pMyRoom)
        {
            if (nErrorCode)
            {
                // TODO: LogHelper::LogError("game.contents", "MyRoomPollenHelp ERROR(%d) ", nErrorCode);
            }
            else
            {
                pMyRoom->SendPollenHelp(nPollenIndex, psHelpUser, biHarvestDate);
            }
        }
    }
}

// ============================================================================
// MyRoomPollenItemUse - IDA @ 0x1406DA090
// ============================================================================
void ThreadLocalData::MyRoomPollenItemUse(int nErrorCode, UXMapID uxMapID, PS_MYROOM_POLLEN_INFO* psPollenInfo)
{
    // IDA: Find area and use pollen item in MyRoom
    ThreadLocalData* pInstance = GetInstance();
    XArea* pArea = pInstance->FindArea(uxMapID);

    if (pArea)
    {
        XMyRoom* pMyRoom = dynamic_cast<XMyRoom*>(pArea);
        if (pMyRoom)
        {
            if (nErrorCode)
            {
                // TODO: LogHelper::LogError("game.contents", "MyRoomPollenItemUse ERROR(%d) ", nErrorCode);
            }
            else
            {
                pMyRoom->SendPollenItemUse(*psPollenInfo);
            }
        }
    }
}

// ============================================================================
// MyRoomPollenCancel - IDA @ 0x1406DA8A0
// ============================================================================
void ThreadLocalData::MyRoomPollenCancel(int nErrorCode, UXMapID uxMapID, int nPollenIndex)
{
    // IDA: Find area and cancel pollen in MyRoom
    ThreadLocalData* pInstance = GetInstance();
    XArea* pArea = pInstance->FindArea(uxMapID);

    if (pArea)
    {
        XMyRoom* pMyRoom = dynamic_cast<XMyRoom*>(pArea);
        if (pMyRoom)
        {
            if (nErrorCode)
            {
                // TODO: LogHelper::LogError("game.contents", "MyRoomPollenCancel ERROR(%d) ", nErrorCode);
            }
            else
            {
                pMyRoom->SendPollenCancel(nPollenIndex);
            }
        }
    }
}

// ============================================================================
// AppearEventMonster - IDA @ 0x1406DABC0
// ============================================================================
void ThreadLocalData::AppearEventMonster(int nModeID, __int64 biStartTime, __int64 biFinishTime,
    int nModeDateID, __int64 biModeStartTime, __int64 biModeEndTime)
{
    // IDA: Appear event monster in all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->AppearEventMonster(nModeID, biStartTime, biFinishTime, nModeDateID, biModeStartTime, biModeEndTime);
        }
    }
}

// ============================================================================
// UpdateWorldMode - IDA @ 0x1406DAD50
// ============================================================================
void ThreadLocalData::UpdateWorldMode(PS_WORLD_MODE_UPDATE& stUpdate)
{
    // IDA: Update world mode for all areas
    for (auto& pair : m_mapArea)
    {
        XArea* pArea = pair.second;
        if (pArea)
        {
            pArea->UpdateWorldMode(&stUpdate);
        }
    }
}

// ============================================================================
// GetScriptManager - IDA @ 0x14025E780
// ============================================================================
VScriptResourceManager& ThreadLocalData::GetScriptManager()
{
    // IDA: Returns reference to script manager
    return *m_xScriptManager;
}

// ============================================================================
// GetGameScriptManager - IDA @ 0x14025E7A0
// ============================================================================
GameScriptManager& ThreadLocalData::GetGameScriptManager()
{
    // IDA: Returns reference to game script manager (Lua)
    return *m_xLuaScript;
}

// ============================================================================
// GetGameModeMgr - IDA @ 0x140310490
// ============================================================================
GameModeMgr* ThreadLocalData::GetGameModeMgr()
{
    // IDA: Returns pointer to game mode manager
    return m_xGameModeMgr.get();
}

// ============================================================================
// GetReportPoolInfo - IDA @ 0x1403E1E20
// ============================================================================
SS_REPORT_POOL_INFO& ThreadLocalData::GetReportPoolInfo()
{
    // IDA: Returns reference to report pool info
    return m_ReportPoolInfo;
}

// ============================================================================
// DeleteProjectile - Delete projectile object
// ============================================================================
void ThreadLocalData::DeleteProjectile(VGameProjectileObject* pProjectile)
{
    if (pProjectile)
    {
        // IDA: Delete the projectile object
        delete pProjectile;
    }
}

// ============================================================================
// DeleteTrap - Delete trap object
// ============================================================================
void ThreadLocalData::DeleteTrap(VGameTrapObject* pTrap)
{
    if (pTrap)
    {
        // IDA: Delete the trap object
        delete pTrap;
    }
}

// ============================================================================
// DeleteChainLightning - Delete chain lightning object
// ============================================================================
void ThreadLocalData::DeleteChainLightning(VChainLightningObject* pChainLightning)
{
    if (pChainLightning)
    {
        // IDA: Delete the chain lightning object
        delete pChainLightning;
    }
}
