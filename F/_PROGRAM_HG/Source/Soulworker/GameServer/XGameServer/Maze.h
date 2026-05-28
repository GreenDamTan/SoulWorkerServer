// Maze.h - XMaze 类声明
// 还原自 GameServer.exe IDA 反编译

#pragma once

#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XCore/XArea/IXArea.h"
#include "Soulworker/GameServer/XGameServer/Sector.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMapMaze.h"
#include <cstdint>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <string>

// 前置声明
class CUser;
class CMonster;
class CNpc;
class CAkashicObject;
class CInteractionObject;
class XActor;
class CMover;
struct TB_MAZE_INFO;
struct VMonsterSpawnInfo;
struct STMageProcessSpawnBox;
struct STMageEventSpawnBox;
struct STInteractionBox;
struct VSafeAreaBoxInfo;
struct STMagePotalBox;
struct VCommonPositionBoxInfo;
struct STQuestMoveBox;
struct ST_WORLD_MODE_INFO;
struct ST_WORLD_MODE_INFO_VEC;
struct PS_WORLD_MODE_FINISH;
struct PS_WORLD_MODE_COMPLETE;
struct PS_WORLD_MODE_UPDATE;
class hkaiPointCloudSilhouetteGenerator;
class DohHavokNavMeshInstance;
class CParty;
class CForce;
struct XVec3;
class CGameWorldMode;

// MAZE_OBJECT - Maze object entry
struct MAZE_OBJECT {
    std::uint32_t dwActorID;
    int nObjectType;
    int nObjectID;

    MAZE_OBJECT();
    ~MAZE_OBJECT();
};

// ST_MAZE_GAME_STATE - Maze game state
struct ST_MAZE_GAME_STATE {
    int m_nMazeState;
    std::uint64_t m_dwMazeStartTime;
    std::uint64_t m_dwMazePlayTime;
    std::uint64_t m_dwMazeWaitTime;

    ST_MAZE_GAME_STATE()
        : m_nMazeState(0)
        , m_dwMazeStartTime(0)
        , m_dwMazePlayTime(0)
        , m_dwMazeWaitTime(0)
    {}
};

// ST_ESCORT_MONSTER - Escort monster info
struct ST_ESCORT_MONSTER {
    std::uint32_t dwActorID;
    int nEscortID;
    float fHP;
    // ... other fields
};

// ============================================================================
// XMaze - Maze class for managing maze instances
// IDA confirmed size: TBD
//
// Function: Manages maze instances including:
// - Sector management
// - Monster spawning
// - Quest progression
// - Event handling
// - Roguelike mode
// ============================================================================
class XMaze : public XArea {
public:
    // === Constructor/Destructor ===
    // IDA: ??0XMaze@@QEAA@XZ (0x140310550)
    XMaze();

    // IDA: ??1XMaze@@QEAA@XZ (0x140310D00)
    virtual ~XMaze();

    // === IXArea interface ===
    int GetWorldType() override { return 1; }  // 1 = Maze

    // === Initialization ===
    // IDA: ?Init@XMaze@@QEAA_NXZ (0x140311210)
    bool Init();

    // IDA: ?Create@XMaze@@QEAA_NAEAUST_CREATE_MAZE@@@Z (0x140315870)
    bool Create(ST_CREATE_MAZE& stCreateMaze);

    // IDA: ?Clear@XMaze@@QEAAXXZ (0x140311C60)
    void Clear();

    // IDA: ?Generate@XMaze@@QEAAXXZ (0x140315E60)
    void Generate();

    // === Maze Time ===
    // IDA: ?StartMazeTime@XMaze@@QEAAXXZ (0x140311B70)
    void StartMazeTime();

    // IDA: ?FinishMazeTime@XMaze@@QEAAXXZ (0x140311BD0)
    void FinishMazeTime();

    // IDA: ?MazePlayTime_Now@XMaze@@QEAAIXZ (0x140311C10)
    std::uint32_t MazePlayTime_Now();

    // IDA: ?SetMazeState@XMaze@@QEAAXH_N@Z (0x140312FB0)
    void SetMazeState(int nState, bool bSync);

    // === Lua Interface ===
    // IDA: ?SetLuaValue@XMaze@@QEAAXPEBD@Z (0x1403130C0)
    void SetLuaValue(const char* szObjectKey);

    // === Actor Management ===
    // IDA: ?FindActor@XMaze@@QEAAPEAVXActor@@UXActorID@@@Z (0x140313920)
    XActor* FindActor(UXActorID uxActorID);

    // IDA: ?MoveActor@XMaze@@QEAA?AV?$TResult@V?$optional@VXVec3@@@@@@@@PEAVXActor@@AEAUXVec3@@M@Z (0x140315750)
    std::uint16_t MoveActor(XActor* pActor, XVec3& vNextPos, float fRot);

    // === Navigation ===
    // IDA: ?CreateNavMesh@XMaze@@QEAA_NPEBD@Z (0x14031F120)
    bool CreateNavMesh(const char* pszFileName);

    // IDA: ?CreateScriptInst@XMaze@@QEAA_NPEBD@Z (0x14031F2C0)
    bool CreateScriptInst(const char* pszFileName);

    // === NPC Management ===
    // IDA: ?CreateNpc@XMaze@@QEAAPEAVCNpc@@UXMapID@@IIPEAUXVec3@@MW4E_SEND_INFO_TYPE@IXArea@@@Z (0x14031A250)
    CNpc* CreateNpc(UXMapID uxMazeSerialID, std::uint32_t nSectorID,
                    std::uint32_t nNpcID, XVec3* vPos, float fRot,
                    E_SEND_INFO_TYPE eType);

    // IDA: ?DeleteNpc@XMaze@@QEAAXPEAVCNpc@@@Z (0x14031A430)
    void DeleteNpc(CNpc* pNpc);

    // === Akashic Object ===
    // IDA: ?CreateAkashicObject@XMaze@@QEAAPEAVCAkashicObject@@UXMapID@@IPEAUXVec3@@MKW4E_SEND_INFO_TYPE@IXArea@@@Z (0x14031A4A0)
    CAkashicObject* CreateAkashicObject(UXMapID uxMazeSerialID, std::uint32_t nAkashicID,
                                        XVec3* vPos, float fRot, std::uint32_t dwParentID,
                                        E_SEND_INFO_TYPE eType);

    // IDA: ?DeleteAkashicObject@XMaze@@QEAAXPEAVCAkashicObject@@@Z (0x14031A5E0)
    void DeleteAkashicObject(CAkashicObject* pAkashic);

    // === Party/Force ===
    // IDA: ?SetParty@XMaze@@QEAAXV?$tr1::shared_ptr@VCParty@@@@@Z (0x140315C40)
    void SetParty(std::tr1::shared_ptr<CParty> pParty);

    // IDA: ?SetForce@XMaze@@QEAAXV?$tr1::shared_ptr@VCForce@@@@@Z (0x140315D50)
    void SetForce(std::tr1::shared_ptr<CForce> pForce);

    // === Scanner ===
    std::map<std::uint32_t, CMover*>* GetScanner(XActor* pActor);

    // === User Count ===
    int GetCurUserCount() const;

    // === Roguelike ===
    bool IsRoguelikeMap() const;

    // === Unique ID ===
    std::uint32_t GetUniqueID(int nSectorID);

    // === Broadcast ===
    void SendBroadCast(XSendPacket* pPacket, XActor* pExceptActor, E_BROADCAST_TYPE eType);

    // === AI/Sector ===
    void RunSectorAI(int nSectorID, int nState);
    void PauseAlltimer(bool bPause);
    void UpdateClearMazeCondition(int nType, int nValue);

    // === User Events ===
    void LoadComplete(CUser* pUser);
    void ChangeMonster(std::uint32_t dwMobID);
    void CheckFollowMonster();
    void RefreshUserCountInSector(std::uint32_t dwActorID);
    void RestartResetState(bool bState1, bool bState2);
    int FindInvisibleActorCnt();

    // === Spawn Box ===
    bool CheckSpawnBoxGroupCount(float fElapsed);
    void ExcuteSpawnBox(void* pSpawnBox, E_SEND_INFO_TYPE eType);

    // === Timer ===
    void UpdateTimer(float fElapsed);
    void UpdateMazeState();

    // === Escort ===
    void ProcessEscortCondition(float fElapsed);

    // === Casual Raid ===
    void UpdateCasualRaidTimer(float fElapsed);

    // === Portal ===
    void UpdatePortalState();

    // === Sync ===
    void SyncSpawnedActive();

    // === Party/Force Member ===
    void EnterPartyForceMember(CUser* pUser);
    void SendSyncHiddenEventInfo(CUser* pUser);
    void SendOutInfo(XSendPacket* pPacket, XActor* pActor);

    // === Layer Level ===
    void GetMazeLayerLevel(int nBitMask, int* pnBaseLevel);

    // === Spawn Position ===
    bool GetSpawnPos(int nSpawnBoxID, XVec3& vPos, float& fRot);

    // === Monster Management ===
    // IDA: ?SpawnGenerateMonster@XMaze@@UEAAXXZ (0x140317750)
    void SpawnGenerateMonster();

    // IDA: ?ExcuteEventSpawn@XMaze@@UEAAXXZ (0x140317A40)
    void ExcuteEventSpawn();

    // === Game Object Entry/Exit ===
    // IDA: ?EnterGameObject@XMaze@@QEAA?AV?$TResult@V?$optional@VXVec3@@@@@@@@PEAVXActor@@W4E_SEND_INFO_TYPE@IXArea@@@Z (0x140313130)
    std::uint16_t EnterGameObject(XActor* pActor, E_SEND_INFO_TYPE eType);

    // IDA: ?ExitGameObject@XMaze@@QEAA?AV?$TResult@V?$optional@VXVec3@@@@@@@@PEAVXActor@@W4E_SEND_INFO_TYPE@IXArea@@@Z (0x140313580)
    std::uint16_t ExitGameObject(XActor* pActor, E_SEND_INFO_TYPE eType);

    // === Monster Check ===
    // IDA: ?IsAliveMonster@XMaze@@QEAA_NH@Z (0x140313950)
    bool IsAliveMonster(int nTableID);

    // === Actor Entry/Exit ===
    // IDA: ?EnterActor@XMaze@@QEAA?AV?$TResult@V?$optional@VXVec3@@@@@@@@PEAVXActor@@@Z (0x140313A60)
    void EnterActor(XActor* pActor) override;

    // IDA: ?ExitActor@XMaze@@QEAA?AV?$TResult@V?$optional@VXVec3@@@@@@@@PEAVXActor@@@Z (0x1403140D0)
    void ExitActor(XActor* pActor) override;

    // === Monster Creation ===
    // IDA: ?CreateMonster@XMaze@@QEAAPEAVCMonster@@UXMapID@@IHPEAUXVec3@@MW4E_SEND_INFO_TYPE@IXArea@@HUXActorID@@@Z (0x140318F20)
    CMonster* CreateMonster(UXMapID uxMazeSerialID, std::uint32_t nSectorID, int nMonsterID,
                           XVec3* vPos, float fRot, E_SEND_INFO_TYPE eType,
                           int nSpawnBoxID, int nGroupID, UXActorID uxParentID);

    // IDA: ?DeleteMonster@XMaze@@QEAAXPEAVCMonster@@@Z (0x140319AC0)
    void DeleteMonster(CMonster* pMonster);

    // === Update ===
    // IDA: ?OnUpdate@XMaze@@UEAAXMM@Z (0x14031C330)
    void OnUpdate(float fElapsed, float fRealElapsed) override;

    // === Send Infos ===
    void SendSectorInfos(CUser* pUser);
    void SendGateInfos(CUser* pUser);
    void SendDieMonsters(CUser* pUser);
    void SendPotalInfos(CUser* pUser);
    void SendInteractionInfos(CUser* pUser);
    void SendLastClientSync(CUser* pUser);
    void SendObjectInfo(CUser* pUser, XActor* pActor);

    // === Sector ===
    int GetSectorIDFromPos(XVec3& vPos);
    CSector* GetSectorFromPos(XVec3& vPos);

    // === ExcuteSpawn - Respawn Manager Interface ===
    // Per IDA 0x14063EA50 (CRespawnManager::Update):
    //   XMaze::ExcuteSpawn(pMaze, object.nTableID, object.nObjectType, object.pSpawnInfo, eSendInfoTypeSend)
    // This method is called by CRespawnManager::Update to respawn monsters in maze
    void ExcuteSpawn(int nTableID, int nObjectType, const VMonsterSpawnInfo* pSpawnInfo, E_SEND_INFO_TYPE eType);

protected:
    // === IDA confirmed member variables ===

    // Object Scanner
    // TODO: Define MAZE_OBJECT_SCANNER type
    char m_objectScanner[256];  // Placeholder for scanner object

    // Maze flags
    bool m_bMazeComplete;

    // NavMesh
    int m_nNavMeshIndex;
    DohHavokNavMeshInstance* m_pNavMeshInstance;
    void* m_pNavMeshResource;  // TODO: HavokNavMeshResource*

    // Maze Resource
    void* m_pMazeResource;  // TODO: VBaseResourceLump*

    // System Actor (for scene attacks)
    CMonster* m_pSystemActor;

    // Script Instance
    void* m_pScriptInstance;  // TODO: IVScriptInstance*

    // Maze Info Table
    TB_MAZE_INFO* m_pTBMazeInfo;

    // Position check time
    float m_fCellPosCheckTime;

    // Grouton spawn
    int m_nSpawnRateGrouton;
    int m_nSpawnRateGroutonBooster;
    int m_nGroutonBoxID;
    std::uint64_t m_dwWaitGroutonSpawnTime;

    // Sector
    int m_nStartSectorID;
    int m_nLastSpawnedSectorID;
    int m_nDestroySilhouetes;
    CSector* m_pActiveEventSector;

    // Party/Force
    std::tr1::shared_ptr<CParty> m_pParty;
    std::tr1::shared_ptr<CForce> m_pForce;
    int m_nPartyUserCount;
    ST_PARTY_INFO m_stPartyInfo;

    // Boss
    bool m_bBossSector;

    // Bot
    bool m_bHaveBotUser;

    // Escort
    float m_fUpdateProcessEscort;
    ST_ESCORT_MONSTER m_stEscortMonster;

    // Wait times
    std::uint64_t m_dwWaitToEnterForceMember;
    std::uint64_t m_dwWaitToLoadEXMember;
    std::uint64_t m_dwWaitUserTime;

    // Monster kill score
    int m_nMonsterKiillScoreModeState;

    // Restart
    bool m_bRestartReady;

    // Time step
    int m_nTimeStepTarget;
    int m_nTimeStepState;
    std::string m_strTimeStepFailScript;

    // Bit flags
    int m_nBit;

    // Party member count
    int m_nPartyMemeberCount;
    int m_nMaxUserCount;

    // Maze Game State
    ST_MAZE_GAME_STATE m_stMazeGameState;

    // Maze Log
    int m_nMazeLog[10];

    // === Maps ===
    std::map<int, STMageProcessSpawnBox*> m_mapProcessSpawnBox;
    std::map<int, STMageEventSpawnBox*> m_mapCheckEventSpawnBox;
    std::map<int, STMageEventSpawnBox*> m_mapEventSpawnBox;
    std::map<int, STMagePotalBox*> m_mapPotalBox;
    std::map<int, STMagePotalBox*> m_mapRandomPotalBox;
    std::map<int, void*> m_mapGateBox;  // TODO: STMageGateBox*
    std::map<int, void*> m_mapLuaFunctionBox;  // TODO: STLuaFunctionBox*
    std::map<int, STInteractionBox*> m_mapInteractionBox;
    std::map<int, VCommonPositionBoxInfo*> m_mapCommonPostionBox;
    std::map<int, STQuestMoveBox*> m_mapQuestMoveBox;
    std::map<int, VSafeAreaBoxInfo*> m_mapRespawnBox;

    // Sector Map
    std::map<int, CSector*> m_mapSector;

    // Wait Enter Maze Users
    std::map<std::uint32_t, CUser*> m_mapWaitEnterMazeUser;

    // Lists
    std::list<XActor*> m_lstDestoryObject;
    std::list<void*> m_lstSilhouetteObject;  // TODO: hkaiPointCloudSilhouetteGenerator*
    std::list<VMonsterSpawnInfo*> m_listMonsterSpawnInfo;
    std::list<VSafeAreaBoxInfo*> m_listSectorStartBox;
    std::list<std::uint32_t> m_listSummonedHelper;
    std::list<void*> m_listWaitForRecvInfo;  // TODO: proper type

    // Vectors
    std::vector<int> m_vecActiveLastSectorID;
    std::list<CMonster*> m_lstChangeMonster;
    std::list<int> m_listDieMonsterSpawnBoxID;
    std::list<void*> m_listSyncSpawnActive;  // TODO: proper type
    std::vector<int> m_vecLuaValue;
    std::vector<std::string> m_vecLuaValues;
    std::vector<CGameWorldMode*> m_vecGameRules;
    std::vector<void*> m_vecRoguelikeRandomShopStat;  // TODO: proper type

    // Additional Maps
    std::map<std::uint32_t, int> m_mapCheckSectorUser;
    std::map<int, int> m_mapMazeCountUser;
    std::set<int> m_setUpdateEnterProcess;
    std::map<int, int> m_mapMonsterDamageType;
    std::map<std::uint32_t, std::uint32_t> m_mpFpUseUCID;
    std::map<std::uint32_t, int> m_mapReviveCount;
    std::map<std::uint32_t, int> m_mapUserHitedCount;
    std::map<std::uint32_t, std::vector<void*>> m_mapDropList;  // TODO: proper type
    std::map<std::uint32_t, bool> m_mapCutsceneOff;

    // Roguelike
    std::map<int, CSector*> m_mapRogueSector;
    std::map<int, void*> m_mapRoguelikeRandomPocketInfo;  // TODO: proper type
    std::map<int, std::vector<int>> m_mapRoguelikePortalBuffList;
    std::map<int, int> m_mapRoguelikePortalBuff;
    std::map<int, void*> m_mapRandomBuff;  // TODO: proper type
    std::map<int, void*> m_mapTimeStepTimer;  // TODO: proper type

    // Game Trap
    std::map<std::uint32_t, void*> m_mapGameTrapObject;  // TODO: proper type
    std::map<int, void*> m_mapGameTrapObjectGroup;  // TODO: proper type

    // Group Aggro
    std::map<int, std::vector<CMonster*>> m_mapGroupMOB;
    std::map<int, std::vector<CMonster*>> m_mapGroupID_Monster;

    // Spawn Box Group Limit
    std::map<int, int> m_mapSpawnBoxGroupLimit;

    // Script Die Monster
    std::map<std::uint32_t, void*> m_mapCallScriptDieMonster;  // TODO: proper type

    // User Damage
    std::map<std::uint32_t, std::uint64_t> m_mapUserDamage;

    // Roguelike Sectors
    CSector* m_pRogueStartSector;
    CSector* m_pRogueNextSector;
    CSector* m_pRogueBossSector;
    int m_nRoguelikeMaxSector;
    int m_nRoguelikeGoBossState;
    int m_nRoguelikePortalBuffID;
    int m_nRoguelikeLastPortalID;
};
