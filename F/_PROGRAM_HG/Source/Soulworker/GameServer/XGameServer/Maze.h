// Maze.h - XMaze 类声明
// 还原自 GameServer.exe IDA 反编译

#pragma once

#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XCore/XArea/IXArea.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
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
class CCutsceneManager;
class CSector;
class CCellPosMgr;
class GameModeMgr;
class ThreadLocalData;
class LogicTimer;
struct VEventObjectInfo;

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
    std::uint64_t m_dwMazeClearTime;
    std::uint64_t m_dwMazeCreateTime;
    int m_nMazeLevel;
    int m_nBatchLayerLevel;
    bool m_bClearCondition[3];

    ST_MAZE_GAME_STATE()
        : m_nMazeState(0)
        , m_dwMazeStartTime(0)
        , m_dwMazePlayTime(0)
        , m_dwMazeWaitTime(0)
        , m_dwMazeClearTime(0)
        , m_dwMazeCreateTime(0)
        , m_nMazeLevel(0)
        , m_nBatchLayerLevel(0)
    {
        m_bClearCondition[0] = false;
        m_bClearCondition[1] = false;
        m_bClearCondition[2] = false;
    }
};

// ST_ESCORT_MONSTER - Escort monster info
// IDA: Fields from SetEscortMonster/SetEscortCondition function analysis
struct ST_ESCORT_MONSTER {
    int nMonsterID;                              // Monster ID (parsed from string)
    std::uint32_t dwEpisodeID;                   // Episode ID
    char szMonsterDieAnim[128];                  // Die animation name
    int nConditionID;                            // Condition ID (for escort quest)
    std::uint32_t dwActorID;                     // Actor ID (runtime)
    int nEscortID;                               // Escort ID (runtime)
    float fHP;                                   // HP (runtime)
};

// Alias for compatibility
using STEscortMonster = ST_ESCORT_MONSTER;

// ============================================================================
// STCasualRaidTime - Casual Raid Timer Structure
// IDA: ??0STCasualRaidTime@@QEAA@XZ (0x1403543C0)
// ============================================================================
struct STCasualRaidTime {
    int nIntValue;          // Timer value (int)
    float fFloatValue;      // Timer value (float)
    float fWaitSendTime;    // Wait time before sending
    
    STCasualRaidTime();
    void reset();
};

// Forward declaration for CCellPosMgr
class CCellPosMgr;

// ============================================================================
// CWarpPotal - Warp Portal class
// IDA: ?Init@CWarpPotal@@QEAAXPEAVXMaze@@@Z (0x140718730)
// ============================================================================
class CWarpPotal {
public:
    CWarpPotal() : m_bReCheck(false), m_bWarpTimeCheck(false), m_fWarpTime(0.0f),
                   m_nSendTimeSec(0), m_pCurInfo(nullptr), m_pMaze(nullptr), m_nJumpID(0) {}
    ~CWarpPotal() = default;

    // IDA: ?Init@CWarpPotal@@QEAAXPEAVXMaze@@@Z (0x140718730)
    void Init(class XMaze* pMaze);

    // IDA: ?Update@CWarpPotal@@QEAAXM@Z (0x1407191e0)
    void Update(float fElapsedTime);

    // Accessors
    bool IsReCheck() const { return m_bReCheck; }
    bool IsWarpTimeCheck() const { return m_bWarpTimeCheck; }
    float GetWarpTime() const { return m_fWarpTime; }
    int GetSendTimeSec() const { return m_nSendTimeSec; }
    int GetJumpID() const { return m_nJumpID; }
    XMaze* GetMaze() const { return m_pMaze; }

    void SetReCheck(bool bCheck) { m_bReCheck = bCheck; }
    void SetWarpTimeCheck(bool bCheck) { m_bWarpTimeCheck = bCheck; }
    void SetWarpTime(float fTime) { m_fWarpTime = fTime; }
    void SetSendTimeSec(int nSec) { m_nSendTimeSec = nSec; }
    void SetJumpID(int nID) { m_nJumpID = nID; }

private:
    // IDA: ?CheckWarp@CWarpPotal@@QEAAXXZ
    void CheckWarp();

    // IDA: ?ProcessTimeCount@CWarpPotal@@QEAAXM@Z
    void ProcessTimeCount(float fElapsedTime);

    bool m_bReCheck;
    bool m_bWarpTimeCheck;
    float m_fWarpTime;
    int m_nSendTimeSec;
    void* m_pCurInfo;  // TODO: STMagePotalBox* or similar
    class XMaze* m_pMaze;
    int m_nJumpID;
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

    // === Maze Info ===
    // IDA: ?GetMazeType@XMaze@@QEAAEXZ (0x14005ABD0)
    std::uint8_t GetMazeType() const;

    // IDA: ?GetHelperCount@XMaze@@QEAAHXZ (0x140328690)
    int GetHelperCount() const { return static_cast<int>(m_listSummonedHelper.size()); }

    // IDA: ?GetCutSceneMgr@XMaze@@QEAAPEAVCCutsceneManager@@XZ (0x140068310)
    CCutsceneManager* GetCutSceneMgr() { return m_pCutSceneManager; }

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

    // IDA: ?GetMazeGameState@XMaze@@QEAAHXZ (0x1400492B0)
    int GetMazeGameState() const { return m_stMazeGameState.m_nMazeState; }

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
    void SetParty(std::shared_ptr<CParty> pParty);

    // IDA: ?SetForce@XMaze@@QEAAXV?$tr1::shared_ptr@VCForce@@@@@Z (0x140315D50)
    void SetForce(std::shared_ptr<CForce> pForce);

    // === Scanner ===
    std::map<std::uint32_t, CMover*>* GetScanner(XActor* pActor);

    // === User Count ===
    int GetCurUserCount() const;

    // === Roguelike ===
    bool IsRoguelikeMap() const;

    // IDA: ?SendRoguelikePocketBox@XMaze@@QEAA_NH@Z (0x140341980)
    bool SendRoguelikePocketBox(int nSectorID);

    // IDA: ?SendRoguelikeShopInfo@XMaze@@QEAA_NH@Z (0x140342e40)
    bool SendRoguelikeShopInfo(int nSectorID);

    // IDA: ?SendPocketBox@XMaze@@QEAA_NH@Z (0x1403456d0)
    bool SendPocketBox(int nGroup);

    // IDA: ?GetTutorial@XMaze@@QEAA_NXZ (0x140638b50)
    bool GetTutorial() const;

    // IDA: ?GetReturnMapID@XMaze@@UEAAGK@Z (0x140353000)
    virtual unsigned short GetReturnMapID(unsigned long dwUCID);

    // IDA: ?GetMazeLevel@XMaze@@QEAAHXZ (0x140353420)
    int GetMazeLevel() const;

    // IDA: ?GetPartyMemberCount@XMaze@@QEAAHXZ (0x140364a90)
    int GetPartyMemberCount() const;

    // === Unique ID ===
    std::uint32_t GetUniqueID(int nSectorID);

    // === Broadcast ===
    void SendBroadCast(XSendPacket* pPacket, XActor* pExceptActor, E_BROADCAST_TYPE eType);

    // === Boss Sector ===
    // IDA: ?IsBossSector@XMaze@@QEAA_NXZ (0x1402A4C40)
    bool IsBossSector() const;

    // === System Actor ===
    // IDA: ?GetSystemActor@XMaze@@QEAAPEAVCMonster@@XZ (0x1402A67C0)
    CMonster* GetSystemActor() const;

    // === Event Sector ===
    // IDA: ?GetEventSector@XMaze@@QEAAPEAVCSector@@XZ (0x1402A9020)
    CSector* GetEventSector() const;

    // === Clear Force ===
    // IDA: ?ClearForce@XMaze@@QEAAXXZ (0x1401C9E90)
    void ClearForce();

    // === Complete Clear Maze ===
    // IDA: ?IsCompleteClearMaze@XMaze@@QEAA_NXZ (0x1402A5010)
    bool IsCompleteClearMaze() const;

    // === Cell Position Manager ===
    // IDA: ?GetCellPosMgr@XMaze@@QEAAAEAVCCellPosMgr@@XZ (0x140280CA0)
    CCellPosMgr& GetCellPosMgr();

    // === Escort Monster ===
    // IDA: ?GetEscortMonster@XMaze@@QEAAAEAUSTEscortMonster@@XZ (0x140280D00)
    STEscortMonster& GetEscortMonster();

    // === Warp Potal ===
    // IDA: ?GetWarpPotal@XMaze@@QEAAPEAVCWarpPotal@@XZ (0x1402A4BC0)
    class CWarpPotal* GetWarpPotal() const;

    // === Escape Actor ===
    // IDA: ?EscapeActor@XMaze@@UEAA_NPEAVXActor@@@Z (0x1402914C0)
    bool EscapeActor(XActor* pActor);

    // === Send Change Action Spawn ===
    // IDA: ?SendChangeActionSpawn@XMaze@@UEAAXW4E_ACTOR_TYPE@@@Z (0x14028E5C0)
    void SendChangeActionSpawn(int eActorType);

    // === Resource Manager ===
    // IDA: ?GetResourceMgr@XMaze@@UEAAPEAVXResourceMgr@@XZ (0x1401A71B0)
    class XResourceMgr* GetResourceMgr();

    // === NavMesh Instance ===
    // IDA: ?GetNavMeshInstance@XMaze@@UEAAPEAVDohHavokNavMeshInstance@@XZ (0x1402A3D00)
    DohHavokNavMeshInstance* GetNavMeshInstance() const;

    // === Object Resource ===
    // IDA: ?GetObjectResource@XMaze@@UEAAPEAVVEventObjectResource@@XZ (0x1402A3D20)
    class VEventObjectResource* GetObjectResource() const;

    // === Batch Layer Level ===
    // IDA: ?GetBatchLayerLevel@XMaze@@QEAAHXZ (0x1402C7D60)
    int GetBatchLayerLevel() const;

    // === Party Quest ===
    // IDA: ?GetPartyQuest@XMaze@@QEAAAEAUSTPartyQuest@@XZ (0x1401444B0)
    struct STPartyQuest& GetPartyQuest();

    // === Set Event Sector ===
    // IDA: ?SetEventSector@XMaze@@QEAAXPEAVCSector@@@Z (0x1402A70E0)
    void SetEventSector(CSector* pSector);

    // === AI/Sector ===
    void RunSectorAI(int nSectorID, int nState);
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
    // IDA: ?EnterActor@XMaze@@UEAAGPEAVXActor@@@Z (0x140313A60) - returns unsigned short
    std::uint16_t EnterActor(XActor* pActor) override;

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
    void OnUpdate(float fElapsed, float fRealElapsed);

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
    CSector* GetSector(int nSectorID);

    // === IsCallScriptDie ===
    bool IsCallScriptDie(std::uint32_t nMonsterID);

    // === ExcuteSpawn - Respawn Manager Interface ===
    // Per IDA 0x14063EA50 (CRespawnManager::Update):
    //   XMaze::ExcuteSpawn(pMaze, object.nTableID, object.nObjectType, object.pSpawnInfo, eSendInfoTypeSend)
    // This method is called by CRespawnManager::Update to respawn monsters in maze
    void ExcuteSpawn(int nTableID, int nObjectType, const VMonsterSpawnInfo* pSpawnInfo, E_SEND_INFO_TYPE eType);

    // === Random Functions ===
    // IDA: ?RandProb@XMaze@@IEAAHXZ (0x140318E90)
    int RandProb();

    // IDA: ?nRand@XMaze@@IEAAHHH@Z (0x140318EB0)
    int nRand(int nMin, int nMax);

    // IDA: ?fRand@XMaze@@IEAAMMM@Z (0x140318EE0)
    float fRand(float fMin, float fMax);

    // === Notify Functions ===
    // IDA: ?NotifyMonsterDelete@XMaze@@QEAAXPEAVCMonster@@@Z (0x14031A0D0)
    void NotifyMonsterDelete(CMonster* pMonster);

    // === Game Mode Functions ===
    // IDA: ?StartDefenceMode@XMaze@@QEAAXH@Z (0x140329050)
    void StartDefenceMode(int nTabelIndex);

    // IDA: ?StartSurvivalMode@XMaze@@QEAAXH@Z (0x140329090)
    void StartSurvivalMode(int nTabelIndex);

    // IDA: ?StartOperationMode@XMaze@@QEAAXH@Z (0x1403290D0)
    void StartOperationMode(int nTabelIndex);

    // IDA: ?StartUnityMode@XMaze@@QEAAXHH@Z (0x140329110)
    void StartUnityMode(int nTableIndex, int nSectorID);

    // IDA: ?StartWarlordMode@XMaze@@QEAAXHH@Z (0x140329160)
    void StartWarlordMode(int nTableIndex, int nSectorID);

    // IDA: ?AddDSPointByUnity@XMaze@@QEAAXG@Z (0x1403291B0)
    void AddDSPointByUnity(std::uint16_t wDSPoint);

    // === Timer Functions ===
    // IDA: ?AddTimer@XMaze@@QEAAXHM@Z (0x1403292E0)
    void AddTimer(int nID, float fTarget);

    // IDA: ?AddTimerEx@XMaze@@QEAAXPEBDMHHH@Z (0x140329370)
    void AddTimerEx(const char* szLuaFunction, float fTarget, int nParam1, int nParam2, int nParam3);

    // IDA: ?AddEventTimer@XMaze@@QEAAXPEBD00HMHH@Z (0x140329440)
    void AddEventTimer(const char* szLuaFinishFunction, const char* szLuaEventFunction, const char* szLuaReadyFunction, int nType, float fTime, int nParam1, int nParam2);

    // IDA: ?ClearAllTimers@XMaze@@QEAAXXZ (0x140329720)
    void ClearAllTimers();

    // IDA: ?RemoveTimer@XMaze@@QEAAXH@Z (0x140329760)
    void RemoveTimer(int nID);

    // IDA: ?RemoveTimerEx@XMaze@@QEAAMPEBD@Z (0x1403295A0)
    float RemoveTimerEx(const char* szLuaFunction);

    // === Sector Monster Functions ===
    // IDA: ?AllDestroySectorMonster@XMaze@@QEAAXH@Z (0x140329A60)
    void AllDestroySectorMonster(int nSectorID);

    // === Position Functions ===
    // IDA: ?GetCommonPositionBox@XMaze@@QEAA?AVhkvVec3@@H@Z (0x140329880)
    struct hkvVec3 GetCommonPositionBox(int nID);

    // IDA: ?GetCommonPositionBoxInfo@XMaze@@QEAAPEBUVCommonPositionBoxInfo@@H@Z (0x140329940)
    const VCommonPositionBoxInfo* GetCommonPositionBoxInfo(int nID);

    // === Sector Functions ===
    // IDA: ?SectorClear@XMaze@@QEAAXH@Z (0x14032A420)
    void SectorClear(int nSectorID);

    // IDA: ?DieEventSectorMonster@XMaze@@QEAAXH_N@Z (0x14032A4F0)
    void DieEventSectorMonster(int nMonsterType, bool bSuicide);

    // IDA: ?LuaClientSync@XMaze@@QEAAXHHHM@Z (0x14032A160)
    void LuaClientSync(int nType, int nValue, int nSectorID, float fLimitTime);

    // === Mode Check Functions ===
    // IDA: ?IsUnityMode@XMaze@@QEAA_NXZ (0x14032B920)
    bool IsUnityMode();

    // IDA: ?IsTerminateSpawn@XMaze@@QEAA_NH@Z (0x14032B960)
    bool IsTerminateSpawn(int nSectorID);

    // === Episode/Condition Functions ===
    // IDA: ?IsCompleteEpisode@XMaze@@QEAA_NKH@Z (0x14032B9F0)
    bool IsCompleteEpisode(std::uint32_t dwUserID, std::uint32_t nQuestID);

    // IDA: ?IsCompleteCondition@XMaze@@QEAA_NKH@Z (0x14032BAE0)
    bool IsCompleteCondition(std::uint32_t dwUserID, int nConditionID);

    // IDA: ?IsProcessEpisode@XMaze@@QEAA_NKH@Z (0x14032BBF0)
    bool IsProcessEpisode(std::uint32_t dwUserID, std::uint32_t nQuestID);

    // IDA: ?IsProcessCondition@XMaze@@QEAA_NKH@Z (0x14032BD00)
    bool IsProcessCondition(std::uint32_t dwUserID, std::uint32_t nConditionID);

    // === Quest Functions ===
    // IDA: ?RemoveQuestAll@XMaze@@QEAA_NXZ (0x14032BE10)
    bool RemoveQuestAll();

    // IDA: ?SetEscortCondition@XMaze@@QEAAXH@Z (0x14032BEE0)
    void SetEscortCondition(int nConditionID);

    // IDA: ?IsHaveQuest@XMaze@@QEAA_NH@Z (0x14032BFF0)
    bool IsHaveQuest(std::uint32_t nQuestID);

    // IDA: ?AcceptQuest@XMaze@@QEAAXH@Z (0x14032C0F0)
    void AcceptQuest(std::uint32_t nQuestID);

    // IDA: ?CompleteQuest@XMaze@@QEAAXH@Z (0x14032C1D0)
    void CompleteQuest(std::uint32_t nQuestID);

    // IDA: ?CompleteCondition@XMaze@@QEAAXH@Z (0x14032C2D0)
    void CompleteCondition(std::uint32_t nConditionID);

    // === User Search Functions ===
    // IDA: ?FindUserByLowLevel@XMaze@@QEAAKXZ (0x14032C3D0)
    std::uint32_t FindUserByLowLevel();

    // IDA: ?FindUserByQuest@XMaze@@QEAAPEAVCMover@@H@Z (0x14032C4E0)
    CMover* FindUserByQuest(std::uint32_t nQuestID);

    // IDA: ?GetUserByIndex@XMaze@@QEAAPEAVCMover@@H@Z (0x14032C5E0)
    CMover* GetUserByIndex(int iIndex);

    // IDA: ?IsHaveCondition@XMaze@@QEAA_NH@Z (0x14032C790)
    bool IsHaveCondition(std::uint32_t nConditionID);

    // === Interaction Functions ===
    // IDA: ?IsEnableInteraction@XMaze@@QEAA_NH@Z (0x14032C890)
    bool IsEnableInteraction(int nID);

    // === Monster Check Functions ===
    // IDA: ?IsExistedMonster@XMaze@@QEAA_NH@Z (0x14032C920)
    bool IsExistedMonster(int nMonsterID);

    // IDA: ?SetInvincibleMonster@XMaze@@QEAAXH_N@Z (0x14032CA20)
    void SetInvincibleMonster(int nMonsterID, bool bEnable);

    // === Lua Value Functions (int index overload) ===
    // IDA: ?SetLuaValue@XMaze@@QEAAXHH@Z (0x14032E310)
    void SetLuaValue(int nIndex, std::uint32_t nValue);

    // IDA: ?AddLuaValue@XMaze@@QEAAXHH@Z (0x14032E350)
    void AddLuaValue(int nIndex, int nValue);

    // === Spawn Control Functions ===
    // IDA: ?TerminateSpawnBox@XMaze@@QEAAXH@Z (0x14032CB30)
    void TerminateSpawnBox(int nSpawnBoxID);

    // IDA: ?TerminateSpawn@XMaze@@QEAAXH@Z (0x14032CCF0)
    void TerminateSpawn(int nSectorID);

    // IDA: ?UpdateSectorClear@XMaze@@QEAAXH@Z (0x14032CDC0)
    void UpdateSectorClear(unsigned int nSectorID);

    // === Destroy Object Functions ===
    // IDA: ?AddDestoryObject@XMaze@@UEAAXPEAVXActor@@@Z (0x14032CF40)
    void AddDestoryObject(XActor* pActor);

    // === Script Callback Functions ===
    // IDA: ?CallScriptPreDieMonster@XMaze@@QEAAXHHH@Z (0x14032D050)
    void CallScriptPreDieMonster(unsigned int nMonsterID, int nSpawnBoxID, unsigned int nGroupID);

    // IDA: ?CallScriptPostDieMonster@XMaze@@QEAAXHHH@Z (0x14032D240)
    void CallScriptPostDieMonster(unsigned int nMonsterID, int nSpawnBoxID, unsigned int nGroupID);

    // === Game Rule Functions ===
    // IDA: ?AddMazeGameRule@XMaze@@QEAAXPEAD00000@Z (0x14032D3F0)
    void AddMazeGameRule(char* szParam1, char* szParam2, char* szParam3, char* szParam4, char* szParam5, char* szParam6);

    // IDA: ?SetMonsterForceActionSkill@XMaze@@QEAAXPEAD0@Z (0x14032D5C0)
    void SetMonsterForceActionSkill(char* szMonsterID, char* szSkillID);

    // IDA: ?SetMonsterCommonActionAi@XMaze@@QEAAXPEAD0@Z (0x14032D7A0)
    void SetMonsterCommonActionAi(char* szMonsterID, char* szCommonActionIndex);

    // IDA: ?WarpMonsterToPoint@XMaze@@QEAAXPEAD0H@Z (0x14032D9B0)
    void WarpMonsterToPoint(char* szMonsterID, char* szBoxID, int nActivate);

    // IDA: ?SetMonsterDefenceType@XMaze@@QEAAXHEM@Z (0x14032DD90)
    void SetMonsterDefenceType(int nMonsterID, unsigned char byType, float fTime);

    // IDA: ?MoveNpcToWayPoint@XMaze@@QEAAXHH@Z (0x14032DEA0)
    void MoveNpcToWayPoint(int nNpcID, int nWayPointID);

    // IDA: ?ChangeNpcAnimation@XMaze@@QEAAXHPEBD@Z (0x14032DFB0)
    void ChangeNpcAnimation(int nNpcID, const char* szAnimName);

    // IDA: ?SetNpcRotation@XMaze@@QEAAXHMH@Z (0x14032E0E0)
    void SetNpcRotation(int nNpcID, float fYaw, int nKey);

    // IDA: ?SetMonsterAllowPassiveType@XMaze@@QEAAXHH@Z (0x14032E200)
    void SetMonsterAllowPassiveType(int nMonsterID, int nAllowPassiveType);

    // === Lua Value Functions ===
    // IDA: ?GetLuaValue@XMaze@@QEAAHH@Z (0x14032E3A0)
    int GetLuaValue(int nIndex);

    // IDA: ?GetUniqueID@XMaze@@QEAAHH@Z (0x14032E3D0)
    int GetUniqueIDFromSector(int nID);

    // === Game Mode Functions ===
    // IDA: ?SetGameModeState@XMaze@@QEAAXH@Z (0x14032E400)
    void SetGameModeState(int nState);

    // IDA: ?InitQuestConditionForSectorClear@XMaze@@QEAAXXZ (0x14032E440)
    void InitQuestConditionForSectorClear();

    // IDA: ?IsCompleteQuestCondition@XMaze@@QEAA_NAEA_N@Z (0x14032E520)
    bool IsCompleteQuestCondition(bool& bSendMsg);

    // === Object Info Functions ===
    // IDA: ?SetObjectInfoReq@XMaze@@QEAAXPEAVCUser@@@Z (0x14032E630)
    void SetObjectInfoReq(CUser* pUser);

    // IDA: ?ApplyBuff@XMaze@@QEAAXHH@Z (0x14032E7D0)
    void ApplyBuff(unsigned int nActorID, unsigned short nBuffID);

    // IDA: ?IsSectorClearConditionBox@XMaze@@QEAA_NPEBUVMonsterSpawnInfo@@@Z (0x14032E880)
    bool IsSectorClearConditionBox(const VMonsterSpawnInfo* pSpawnInfo);

    // IDA: ?SetSectorStepStop@XMaze@@QEAAXH_N@Z (0x14032E9C0)
    void SetSectorStepStop(int nSectorID, bool bStop);

    // IDA: ?FailEscortQuest@XMaze@@QEAAXXZ (0x14032EA70)
    void FailEscortQuest();

    // === Escort Condition Functions ===
    // IDA: ?CompleteEscortCondition@XMaze@@QEAAXXZ (0x14032EC10)
    void CompleteEscortCondition();

    // === Monster Quest Functions ===
    // IDA: ?ProcessMonsterQuest@XMaze@@UEAAXPEAVXActor@@H@Z (0x14032F3C0)
    void ProcessMonsterQuest(XActor* pAttacker, unsigned int nMonsterID);

    // === Skill Functions ===
    // IDA: ?LearnSkill@XMaze@@QEAAXKH@Z (0x14032F770)
    void LearnSkill(unsigned int dwUserID, int nSkillID);

    // === User Search Functions ===
    // IDA: ?GetUserClass@XMaze@@QEAAEK@Z (0x14032F8A0)
    std::uint8_t GetUserClass(unsigned int dwUserID);

    // === Chat Functions ===
    // IDA: ?ChatMessage@XMaze@@QEAAXKPEAD@Z (0x14032F980)
    void ChatMessage(unsigned int dwUserID, char* szMsg);

    // === Maze State Functions ===
    // IDA: ?GetMazeState@XMaze@@QEAAHXZ (0x14032FC70)
    int GetMazeState();

    // === Party Functions ===
    // IDA: ?IsParty@XMaze@@QEAA_NXZ (0x14032FEA0)
    bool IsParty();

    // === Sector ID Functions ===
    // IDA: ?SetLastSectorID@XMaze@@QEAAXH_N@Z (0x14032FF20)
    void SetLastSectorID(int nVal, bool bIsPotal);

    // IDA: ?GetLastSectorID@XMaze@@QEAAHXZ (0x14032FF90)
    int GetLastSectorID();

    // === Party Send Functions ===
    // IDA: ?PartySend@XMaze@@QEAAXAEAVXSendPacket@@@Z (0x14032FED0)
    void PartySend(XSendPacket& xSendPacket);

    // === Boss Sector Functions ===
    // IDA: ?SetBossSector@XMaze@@QEAAX_N@Z (0x14032FFF0)
    void SetBossSector(bool bFlag);

    // === Monster Damage Type Functions ===
    // IDA: ?SetMonsterLastDamageType@XMaze@@QEAAXKK@Z (0x1403305C0)
    void SetMonsterLastDamageType(unsigned int dwMonsterID, unsigned int dwCondition);

    // IDA: ?GetMonsterLastDamageType@XMaze@@QEAAKK@Z (0x140330660)
    unsigned int GetMonsterLastDamageType(unsigned int dwMonsterID);

    // === Skill Condition Functions ===
    // IDA: ?UseSkillCondition@XMaze@@QEAAXEK@Z (0x1403306E0)
    void UseSkillCondition(unsigned char bySkillType, unsigned int dwValue);

    // === Hidden Event Functions ===
    // IDA: ?UpdateHiddenEventCondition@XMaze@@QEAAXHKK@Z (0x140330740)
    void UpdateHiddenEventCondition(int nTarget, unsigned int dwObject, unsigned int dwValue);

    // IDA: ?CheckHiddenEventState@XMaze@@QEAA_NXZ (0x1403307D0)
    bool CheckHiddenEventState();

    // IDA: ?SendHiddenEventUpdate@XMaze@@QEAAXXZ (0x140330C20)
    void SendHiddenEventUpdate();

    // === Maze Info Functions ===
    // IDA: ?SendMazeInfo@XMaze@@QEAA_NXZ (0x140331130)
    bool SendMazeInfo();

    // === Player Death Functions ===
    // IDA: ?DiePlayer@XMaze@@UEAAXPEAVCUser@@@Z (0x14032B530)
    void DiePlayer(CUser* pUser);

    // IDA: ?IsRevive@XMaze@@UEAA_NXZ (0x14032B8A0)
    bool IsRevive();

    // IDA: ?IsUseItem@XMaze@@UEAA_NXZ (0x14032B8E0)
    bool IsUseItem();

    // === Monster Damage Functions ===
    // IDA: ?DamageMonster@XMaze@@QEAAXPEAVCMonster@@@Z (0x14032B300)
    void DamageMonster(CMonster* pMonster);

    // IDA: ?InteractBoxOnMode@XMaze@@QEAAXPEAVCUser@@H@Z (0x14032B340)
    void InteractBoxOnMode(CUser* pUser, int nBoxID);

    // === Sector Reset Functions ===
    // IDA: ?ResetAllSectorFlags@XMaze@@QEAAXXZ (0x140329B80)
    void ResetAllSectorFlags();

    // === Test Functions ===
    // IDA: ?TestReward@XMaze@@QEAAXXZ (0x140324B20)
    void TestReward();

    // === Script Quest Functions ===
    // IDA: ?CallScriptUpdateQuest@XMaze@@QEAAXKHK@Z (0x140331630)
    void CallScriptUpdateQuest(unsigned int dwActorID, int nType, unsigned int dwID);

    // === Sector Info Functions ===
    // IDA: ?ShowSectorInfo@XMaze@@QEAAXPEAVCUser@@@Z (0x1403316A0)
    void ShowSectorInfo(CUser* pUser);

    // === Monster Count Functions ===
    // IDA: ?GetMonsterCountByID@XMaze@@QEAAHK@Z (0x1403316F0)
    unsigned int GetMonsterCountByID(unsigned int dwID);

    // === Die Monster SpawnBox Functions ===
    // IDA: ?AddDieMonsterSpawnBoxID@XMaze@@QEAAXH@Z (0x140331FF0)
    void AddDieMonsterSpawnBoxID(int nSpawnBoxID);

    // === Change Monster Functions ===
    // IDA: ?AddChangeMonster@XMaze@@QEAAXK@Z (0x140332090)
    void AddChangeMonster(unsigned int dwMonsterID);

    // === Portal Functions ===
    // IDA: ?GetMazePotalBox@XMaze@@QEAAPEAUSTMagePotalBox@@H@Z (0x140332C30)
    STMagePotalBox* GetMazePotalBox(int nUniqueID);

    // === Quest Respawn Functions ===
    // IDA: ?UpdateQuestRespawn@XMaze@@QEAAXH@Z (0x140330440)
    void UpdateQuestRespawn(int nCondition);

    // === Monster Level Stat Functions ===
    // IDA: ?ChangeMonsterLevelStat@XMaze@@QEAAXH@Z (0x140328840)
    void ChangeMonsterLevelStat(int nMemberCount);

    // === Casual Raid Timer Functions ===
    // IDA: ?ShowCasualRaidTimer@XMaze@@QEAAXHMM@Z (0x14032B390)
    void ShowCasualRaidTimer(int nValue, float fTime, float fTime2);

    // IDA: ?SetupCasualRaidTimer@XMaze@@QEAAXXZ (0x14032B4B0)
    void SetupCasualRaidTimer();

    // IDA: ?SendCasualRaidTimer@XMaze@@QEAAXXZ (0x14032B4E0)
    void SendCasualRaidTimer();

    // === Infinite Tower Functions ===
    // IDA: ?EnterInfiniteTowerNextStage@XMaze@@QEAA_NPEAVCUser@@@Z (0x140331F10)
    bool EnterInfiniteTowerNextStage(CUser* pUser);

    // === Maze Info Sync Functions ===
    // IDA: ?SendMazeInfoSync@XMaze@@QEAA_NXZ (0x1403312C0)
    bool SendMazeInfoSync();

    // === Random Portal Functions ===
    // IDA: ?UpdateRandomPortal@XMaze@@QEAAXH@Z (0x140332A10)
    void UpdateRandomPortal(int nSectorID);

    // === Sector Warp Functions ===
    // IDA: ?WarpSectorStartPos@XMaze@@UEAA_NPEAVCUser@@@Z (0x1403319A0)
    bool WarpSectorStartPos(CUser* pUser);

    // IDA: ?MoveNextSector@XMaze@@UEAAXPEAVCUser@@H@Z (0x140331AD0)
    void MoveNextSector(CUser* pUser, int nNextStep);

    // === Quest Respawn Functions ===
    // IDA: ?SetQuestRespawn@XMaze@@QEAAXKHHPEBUVMonsterSpawnInfo@@@Z (0x1403301E0)
    void SetQuestRespawn(unsigned int dwActorID, int nTableID, int nType, const void* pMonsterSpawn);

    // === Hidden Event Cheat Functions ===
    // IDA: ?CheatSetHiddenEvent@XMaze@@QEAAXGK@Z (0x140330F70)
    void CheatSetHiddenEvent(unsigned short wEvent, unsigned int dwCondition);

    // IDA: ?CheatUpdateHiddenEvent@XMaze@@QEAAXHG@Z (0x1403310A0)
    void CheatUpdateHiddenEvent(int nType, unsigned short wEvent);

    // === Summon Monster Functions ===
    // IDA: ?SetSummonMonsterDelete@XMaze@@QEAAXKKPEAD@Z (0x140331800)
    void SetSummonMonsterDelete(unsigned int dwTBID, unsigned int dwOwnerID, char* szAnim);

    // === Portal Warp Functions ===
    // IDA: ?WarpPortal@XMaze@@UEAA_NPEAVCUser@@HHH@Z (0x140332CA0)
    bool WarpPortal(CUser* pUser, int nMapID, int nJumpID, int nPortalID);

    // === Start Sector Functions ===
    // IDA: ?GetStartSectorID@XMaze@@QEAAXH@Z (0x140333750)
    void GetStartSectorID(int nID);

    // === FP Use UCID Functions ===
    // IDA: ?SetFpUseUCID@XMaze@@QEAAXKE@Z (0x1403337E0)
    void SetFpUseUCID(unsigned int dwUCID, unsigned char byFPUse);

    // IDA: ?GetFpUseUCID@XMaze@@QEAA_NKAEAE@Z (0x140333830)
    bool GetFpUseUCID(unsigned int dwUCID, unsigned char& byFPUse);

    // === Trap Functions ===
    // IDA: ?SetTrapLifeTime@XMaze@@QEAAXHM@Z (0x140333910)
    void SetTrapLifeTime(unsigned int nIndex, float fTime);

    // === Hidden Event Reward Functions ===
    // IDA: ?GetRewardHiddenEvent@XMaze@@QEAA_NAEAGAEAHAEAF@Z (0x140330BD0)
    bool GetRewardHiddenEvent(unsigned short& wHiddenID, int& nItemID, short& shCount);

    // === Trap Object Functions ===
    // IDA: ?AddTrapObject@XMaze@@QEAA_NHPEAVVGameTrapObject@@@Z (0x1403338B0)
    bool AddTrapObject(unsigned int nIndex, void* pTrapObj);

    // === Monster Group Functions ===
    // IDA: ?AddMonsterGroupID@XMaze@@QEAAXH@Z (0x140333C60)
    void AddMonsterGroupID(int nGroupID);

    // IDA: ?DeleteMonsterGroupID@XMaze@@QEAA_NH_N@Z (0x140333D70)
    bool DeleteMonsterGroupID(int nGroupID, bool bPre);

    // IDA: ?ResetMonsterGroup@XMaze@@QEAAXH@Z (0x140333EE0)
    void ResetMonsterGroup(int nGroupID);

    // === Log Functions ===
    // IDA: ?WriteMazeRewardCashItemLog@XMaze@@QEAAXKKAEAUST_CREATE_ITEM@@0@Z (0x140331520)
    void WriteMazeRewardCashItemLog(int dwUAID, int dwUCID, void* stRewardItem, void* stReduceItem);

    // === Trap Object Group Functions ===
    // IDA: ?AddTrapObjectGroup@XMaze@@QEAA_NHPEAVVGameTrapObject@@@Z (0x1403339D0)
    bool AddTrapObjectGroup(unsigned int nGroup, void* pTrapObj);

    // IDA: ?SetTrapLifeTimeGroup@XMaze@@QEAAXHM@Z (0x140333B30)
    void SetTrapLifeTimeGroup(unsigned int nGroup, float fTime);

    // === Monster Group ID Functions ===
    // IDA: ?GetSectorByGroupID@XMaze@@QEAAHH@Z (0x1403341C0)
    int GetSectorByGroupID(int nGroupID);

    // IDA: ?StopSpawnBoxGroupCount@XMaze@@QEAAXH@Z (0x1403345F0)
    void StopSpawnBoxGroupCount(int nGroupID);

    // IDA: ?GetExitBoxInfo@XMaze@@UEAA_NAEAGAEAHAEAUXVec3@@@Z (0x140334c00)
    bool GetExitBoxInfo(unsigned short& wMapID, int& nJumpID, XVec3& vPos);

    // IDA: ?CompleteTimerStep@XMaze@@QEAAXPEBDH@Z (0x140334e10)
    void CompleteTimerStep(const char* szID, int nStep);

    // IDA: ?AddEventMaxTime@XMaze@@QEAAXPEBDM@Z (0x140334ee0)
    void AddEventMaxTime(const char* szID, float fTime);

    // IDA: ?AddGroupIDMonsterList@XMaze@@QEAAXHPEAUVMonsterSpawnInfo@@@Z (0x140333f80)
    void AddGroupIDMonsterList(int nGroupID, VMonsterSpawnInfo* pMonsterSpawn);

    // IDA: ?DoSpawnBoxGroupCount@XMaze@@QEAAXHHM@Z (0x1403342a0)
    void DoSpawnBoxGroupCount(int nGroupID, int nLimit, float fWaitTime);

    // IDA: ?InitPartyQuest@XMaze@@QEAAXKHH@Z (0x140335030)
    void InitPartyQuest(unsigned int dwQuestID, int nConditionID, int nStartSector);

    // IDA: ?UpdatePartyQuest@XMaze@@QEAAXKH_N@Z (0x140335080)
    void UpdatePartyQuest(unsigned int dwQuestID, int nConditionID, bool bComplete);

    // IDA: ?ResetPartyQuest@XMaze@@QEAAXXZ (0x140335100)
    void ResetPartyQuest();

    // IDA: ?CallTestLuaFunction@XMaze@@QEAAXH@Z (0x140335130)
    void CallTestLuaFunction(unsigned int nParam);

    // IDA: ?StartEventTimer@XMaze@@QEAAXPEBD@Z (0x140336490)
    void StartEventTimer(const char* szID);

    // IDA: ?GetExitDistrictID@XMaze@@UEAA_NKAEAGAEAHAEAUXVec3@@@Z (0x140336540)
    bool GetExitDistrictID(unsigned int dwActorID, unsigned short& wMapID, int& nJumpID, XVec3& vPos);

    // IDA: ?SetModeFail@XMaze@@QEAAXXZ (0x140336fc0)
    void SetModeFail();

    // IDA: ?IsCutsceneCondition@XMaze@@QEAA_NKHH@Z (0x140337070)
    bool IsCutsceneCondition(unsigned int dwActorID, int nConditionType, int nConditionValue);

    // IDA: ?RestartSendLog@XMaze@@QEAAXHHHHH@Z (0x1403400e0)
    void RestartSendLog(int nUCID, int nParam1, int nParam2, int nParam3, int nParam4);

    // IDA: ?GetRestartState@XMaze@@QEAA_NPEAVCUser@@@Z (0x140340020)
    bool GetRestartState(CUser* pUser);

    // IDA: ?AddTimeStepTimer@XMaze@@QEAAXHMHHPEBD@Z (0x140340360)
    void AddTimeStepTimer(int nStep, float fTime, int nSpawnBoxID, int nDeathMotion, const char* szTimeout);

    // IDA: ?StartTimeStepTimer@XMaze@@QEAAXXZ (0x1403404b0)
    void StartTimeStepTimer();

    // IDA: ?ChangeMonsterMotion@XMaze@@QEAAXHFF@Z (0x140340660)
    void ChangeMonsterMotion(int nMonsterID, std::int16_t nSourceMotion, std::int16_t nDestMotion);

    // IDA: ?SetRoguePortalFlag@XMaze@@QEAAXHH_N@Z (0x140340880)
    void SetRoguePortalFlag(int nBoxIndex, int nNextSectorID, bool bFlag);

    // IDA: ?SetRoguelikeTimeout@XMaze@@QEAAXXZ (0x1403444b0)
    void SetRoguelikeTimeout();

    // IDA: ?PlayClientEvent@XMaze@@QEAAXPEADH@Z (0x140345700)
    void PlayClientEvent(char* szEvent, int nState);

    // IDA: ?AddCutscene@XMaze@@QEAAXPEADHKHH@Z (0x1403371c0)
    void AddCutscene(char* szName, int nType, unsigned int dwTime, int nConditionType, int nConditionValue);

    // IDA: ?UpdatePublicInteractionobjectInfo@XMaze@@QEAA_NH_N0H@Z (0x140337210)
    bool UpdatePublicInteractionobjectInfo(int nIndex, bool bSight, bool bEnable, int nCount);

    // IDA: ?GetPublicInteractionobject@XMaze@@QEAA_NHAEA_N0AEAH@Z (0x1403372e0)
    bool GetPublicInteractionobject(int nIndex, bool& bEnable, bool& bSight, int& nCount);

    // IDA: ?CanUseReviveCount@XMaze@@QEAA_NK@Z (0x1403373b0)
    bool CanUseReviveCount(unsigned int dwUCID);

    // IDA: ?AddUseReviveCount@XMaze@@QEAAXPEAVCUser@@@Z (0x140337460)
    void AddUseReviveCount(CUser* pUser);

    // IDA: ?SendUseReviveCount@XMaze@@QEAAXPEAVXActor@@@Z (0x1403375e0)
    void SendUseReviveCount(XActor* pActor);

    // IDA: ?IsPlayCutscene@XMaze@@QEAA_NK@Z (0x140337720)
    bool IsPlayCutscene(unsigned int dwActorID);

    // IDA: ?IsCutsceneState@XMaze@@QEAA_NXZ (0x140337750)
    bool IsCutsceneState();

    // IDA: ?ProcessGameRuleCheck@XMaze@@QEAAXPEAVCMover@@@Z (0x140337780)
    void ProcessGameRuleCheck(CMover* pMover);

    // IDA: ?ProcessGameRuleForUser@XMaze@@QEAAXPEAVCUser@@@Z (0x140337830)
    void ProcessGameRuleForUser(CUser* pUser);

    // IDA: ?ProcessGameRuleForMonster@XMaze@@QEAAXPEAVCMonster@@@Z (0x140337a90)
    void ProcessGameRuleForMonster(CMonster* pMonster);

    // IDA: ?CallScriptCutsceneOff@XMaze@@QEAAXPEAD@Z (0x140337cb0)
    void CallScriptCutsceneOff(const char* szCutsceneName);

    // IDA: ?ProcessExp@XMaze@@UEAAXPEAVXActor@@MH@Z (0x140337ee0)
    void ProcessExp(XActor* pAtk, float fExp, char nMonsterLv);

    // IDA: ?CheckMazeCondition@XMaze@@QEAAXPEAVXActor@@@Z (0x140338020)
    void CheckMazeCondition(XActor* pActor);

    // IDA: ?StartWorldMode@XMaze@@UEAAXAEAUST_WORLD_MODE_INFO@@@Z (0x140338de0)
    void StartWorldMode(ST_WORLD_MODE_INFO& stInfo);

    // IDA: ?FinishWorldMode@XMaze@@UEAAXAEAUPS_WORLD_MODE_FINISH@@@Z (0x140339020)
    void FinishWorldMode(PS_WORLD_MODE_FINISH& stInfo);

    // IDA: ?SetDisconnectUserState@XMaze@@QEAAXKUST_PARTY_INFO@@@Z (0x140336930)
    void SetDisconnectUserState(unsigned int dwUCID, ST_PARTY_INFO stPartyInfo);

    // IDA: ?CheckDisconnecUsertState@XMaze@@QEAAEKAEAE@Z (0x140336e00)
    unsigned char CheckDisconnecUsertState(unsigned int dwUCID, unsigned char& byState);

    // IDA: ?SyncWorldMode@XMaze@@UEAAXAEAUST_WORLD_MODE_INFO_VEC@@@Z (0x1403394c0)
    void SyncWorldMode(ST_WORLD_MODE_INFO_VEC& stInfoVec);

    // IDA: ?SetWorldModeBoostAll@XMaze@@QEAAXH_J@Z (0x140339750)
    void SetWorldModeBoostAll(unsigned short nBoostID, __int64 nEndDate);

    // IDA: ?SetWorldModeSync@XMaze@@QEAAXPEAVXActor@@@Z (0x1403398f0)
    void SetWorldModeSync(XActor* pActor);

    // IDA: ?AddUserDamage@XMaze@@QEAAXKH@Z (0x14033a7c0)
    void AddUserDamage(unsigned int dwUCID, int nDamage);

    // IDA: ?GetUserDamagePercent@XMaze@@QEAAHK@Z (0x14033a880)
    int GetUserDamagePercent(unsigned int dwUCID);

    // IDA: ?AddHitedCount@XMaze@@QEAAXK@Z (0x14033a9a0)
    void AddHitedCount(unsigned int dwUCID);

    // IDA: ?GetHitedCount@XMaze@@QEAAHK@Z (0x14033aa60)
    int GetHitedCount(unsigned int dwUCID);

    // IDA: ?GetMazeLayerLevel@XMaze@@QEAAXHAEAH@Z (0x14033aad0)
    void GetMazeLayerLevel(int nLayerBit, unsigned int& nBaseLevel);

    // IDA: ?IsEnemyPVP@XMaze@@UEAA_NPEAVXActor@@0@Z (0x14033ac90)
    bool IsEnemyPVP(XActor* pAtk, XActor* pDef);

    // IDA: ?GetMonsterByID@XMaze@@QEAAPEAVCMonster@@K@Z (0x14033ace0)
    CMonster* GetMonsterByID(unsigned int dwID);

    // IDA: ?IsDeleteMaze@XMaze@@QEAA_NXZ (0x14033ade0)
    bool IsDeleteMaze();

    // IDA: ?PauseAlltimer@XMaze@@QEAAX_N@Z (0x14033ae10)
    void PauseAlltimer(bool bPause);

    // IDA: ?ApplyBuffToMonster@XMaze@@QEAAXPEAD0@Z (0x140339e20)
    void ApplyBuffToMonster(const char* szMonsterID, const char* szBuffID);

    // IDA: ?ChangeMobIDByMazeLevel@XMaze@@QEAAHH@Z (0x14033a000)
    unsigned int ChangeMobIDByMazeLevel(unsigned int nMonsterID);

    // IDA: ?ApplyBuffEx@XMaze@@QEAAXHHPEAD@Z (0x14033a080)
    void ApplyBuffEx(int nType, int nCount, const char* szBuffID);

    // IDA: ?ExcuteCheckEventSpawnBox@XMaze@@QEAAXPEAVCUser@@HH@Z (0x140317b60)
    void ExcuteCheckEventSpawnBox(CUser* pUser, int nSectorID, int nBoxIndex);

    // IDA: ?ExcuteSpawnBox@XMaze@@QEAAXPEAUSTMageProcessSpawnBox@@W4E_SEND_INFO_TYPE@IXArea@@@Z (0x14031aac0)
    void ExcuteSpawnBox(void* pProcessSpawn, E_SEND_INFO_TYPE eType);

    // IDA: ?ExcuteSpawnBox@XMaze@@QEAAXPEBUVMonsterSpawnInfo@@W4E_SEND_INFO_TYPE@IXArea@@@Z (0x14031b560)
    void ExcuteSpawnBox(const VMonsterSpawnInfo* pMonsterSpawn, E_SEND_INFO_TYPE eType);

    // IDA: ?CheckGuardTarget@XMaze@@QEAAXPEAVCMonster@@H@Z (0x140328260)
    void CheckGuardTarget(CMonster* pMonster, int nGuardSpawnBoxID);

    // IDA: ?CreateSilhouetteFromBoxinfo@XMaze@@QEAA_NPEBUVMonsterSpawnInfo@@PEAPEAX@Z (0x140329dc0)
    bool CreateSilhouetteFromBoxinfo(const VMonsterSpawnInfo* pBoxInfo, void** ppSilhouette);

    // IDA: ?ChangeMonster@XMaze@@QEAA_NHHH@Z (0x14032a540)
    bool ChangeMonster(int nOldMonsterID, int nNewMonsterID, int nSpawnBoxID);

    // IDA: ?SetEscortMonster@XMaze@@QEAAXKPEBD0@Z (0x14032bf00)
    void SetEscortMonster(unsigned int dwEpisodeID, const char* szMonsterID, const char* szAnimName);

    // IDA: ?CheckCanDirectMove2@XMaze@@QEAA_NPEBUXVec3@@0M@Z (0x14032aad0)
    bool CheckCanDirectMove2(const XVec3* pStartPos, const XVec3* pEndPos, float fRadius);

    // IDA: ?ResetLuaFunctionBox@XMaze@@QEAAXH@Z (0x14033bae0)
    void ResetLuaFunctionBox(int nBoxIndex);

    // IDA: ?FinishMonsterKillScoreMode@XMaze@@QEAAXXZ (0x14033b240)
    void FinishMonsterKillScoreMode();

    // IDA: ?AddMonsterKillScoreModePoint@XMaze@@QEAAXH@Z (0x14033b560)
    void AddMonsterKillScoreModePoint(int nPoint);

    // === Raid/Instance Dungeon Functions ===
    // IDA: ?UpdateCasualRaidTimer@XMaze@@QEAAXM@Z (0x14031D850)
    void UpdateCasualRaidTimer(float fElapsed);

    // IDA: ?SetBossSector@XMaze@@QEAAX_N@Z (0x14032FFF0)
    void SetBossSector(bool bFlag);

    // IDA: ?GoRoguelikeBoss@XMaze@@QEAAXH@Z (0x140344CB0)
    void GoRoguelikeBoss(int nState);

    // IDA: ?SectorClear@XMaze@@QEAAXH@Z (0x14032A420)
    void SectorClear(int nSectorID);

    // IDA: ?SpawnSectorMonster@XMaze@@QEAAXH@Z (0x14031F900)
    void SpawnSectorMonster(int nSector);

    // IDA: ?SpawnSectorMonsterForOpt@XMaze@@QEAAXH@Z (0x14031F9B0)
    void SpawnSectorMonsterForOpt(int nSector);

    // IDA: ?AllDestroySectorMonster@XMaze@@QEAAXH@Z (0x140329A60)
    void AllDestroySectorMonster(int nSectorID);

    // IDA: ?RunSectorAI@XMaze@@QEAAXH_N@Z (0x14031F7C0)
    void RunSectorAI(int nSector, bool bIsPotal);

    // IDA: ?GetSectorFromPos@XMaze@@QEAAPEAVCSector@@AEBVhkvVec3@@@Z (0x14031F670)
    CSector* GetSectorFromPos(const hkvVec3& vPos);

    // IDA: ?GetSector@XMaze@@QEAAPEAVCSector@@H@Z (0x14032B270)
    CSector* GetSector(int nSectorID);

    // IDA: ?GetSectorIDFromPos@XMaze@@QEAAHAEBVhkvVec3@@@Z (0x14031F450)
    int GetSectorIDFromPos(const hkvVec3& vPos);

    // IDA: ?GetSectorUniqueIDFromPos@XMaze@@QEAAHAEBVhkvVec3@@@Z (0x14031F560)
    int GetSectorUniqueIDFromPos(const hkvVec3& vPos);

    // IDA: ?DieEventSectorMonster@XMaze@@QEAAXH_N@Z (0x14032A4F0)
    void DieEventSectorMonster(int nSectorID, bool bEvent);

    // IDA: ?ResetAllSectorFlags@XMaze@@QEAAXXZ (0x140329B80)
    void ResetAllSectorFlags();

    // IDA: ?AllUserWarpInSector@XMaze@@QEAAXVhkvVec3@@M@Z (0x140323EF0)
    void AllUserWarpInSector(hkvVec3 vPos, float fRot);

    // IDA: ?SendSectorCompleteState@XMaze@@QEAAXHH@Z (0x14031FAF0)
    void SendSectorCompleteState(int nSectorID, int nState);

    // === Helper Functions ===
    int GetBatchLayerLevel() const;
    void SetLastSectorID(int nSectorID, bool bIsPotal);
    int GetLastSectorID() const;
    bool AllUserWarp(XVec3* vPos, float fRot, int nType);

protected:
    // === IDA confirmed member variables ===

    // Cutscene Manager - 使用指针避免不完整类型问题
    // IDA: m_cutSceneManager at fixed offset in XMaze
    CCutsceneManager* m_pCutSceneManager;

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
    std::shared_ptr<CParty> m_pParty;
    std::shared_ptr<CForce> m_pForce;
    int m_nPartyUserCount;
    ST_PARTY_INFO m_stPartyInfo;

    // Boss
    bool m_bBossSector;

    // Casual Raid Timer
    STCasualRaidTime m_stCasualRaidTime;

    // Sector Map
    std::map<unsigned int, CSector*> m_mapSector;

    // Roguelike Boss Sector
    CSector* m_pRogueBossSector;
    CSector* m_pRogueNextSector;

    // Wait Enter Maze User
    std::map<unsigned long, ST_MAZE_WAIT_ENTER_USER_INFO> m_mapWaitEnterMazeUser;

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

    // Hidden Event - IDA confirmed
    void* m_pHiddenEvent;  // TODO: proper type for hidden event

    // Enter District Position - IDA confirmed
    struct STPosInfo {
        float x, y, z;
        float fRot;
        STPosInfo() : x(0), y(0), z(0), fRot(0) {}
        static void Init(STPosInfo* pInfo) {
            if (pInfo) {
                pInfo->x = 0;
                pInfo->y = 0;
                pInfo->z = 0;
                pInfo->fRot = 0;
            }
        }
    };
    STPosInfo m_stEnterDistrictPos;

    // IDA: ?GetRoguelikeNextMap@XMaze@@QEAA_NPEAVCUser@@AEAHAEAUSTPosInfo@@@Z (0x1403447f0)
    bool GetRoguelikeNextMap(CUser* pUser, int& nNextMapID, STPosInfo& stPosInfo);

    // IDA: ?SetEscortMonster@XMaze@@QEAAXKPEBD0@Z (0x14032bf00)
    void SetEscortMonster(unsigned long dwEpisodeID, const char* szMonsterID, const char* szAnimName);

    // IDA: ?SetDisconnectUserState@XMaze@@QEAAXKUST_PARTY_INFO@@@Z (0x140336930)
    void SetDisconnectUserState(unsigned long dwUCID, const struct ST_PARTY_INFO& stPartyInfo);

    // IDA: ?SetEnterDistrictPos@XMaze@@QEAAXAEAUSTPosInfo@@@Z (0x1406e0460)
    void SetEnterDistrictPos(STPosInfo& stPos);

    // IDA: ?SetPartyInfo@XMaze@@QEAAXUST_PARTY_INFO@@@Z (0x1406e0490)
    void SetPartyInfo(const struct ST_PARTY_INFO& stPartyInfo);

    // IDA: ?IsModeCondition@XMaze@@UEAA_NXZ (0x140345840)
    virtual bool IsModeCondition();

    // IDA: ?FinishRoguelikeSector@XMaze@@QEAAXPEAVCUser@@H@Z (0x140345180)
    void FinishRoguelikeSector(CUser* pUser, int nSectorID);

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

    // Logic Timers (IDA confirmed)
    std::list<void*> m_arLogicTimers;      // TODO: LogicTimer* proper type
    std::list<void*> m_arWaitLogicTimers;  // TODO: LogicTimer* proper type

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

    // Cell Position Manager
    // TODO: CCellPosMgr m_CellPosMgr;  // Need proper type definition

    // Warp Portal
    class CWarpPotal* m_pWarpPotal;
};
