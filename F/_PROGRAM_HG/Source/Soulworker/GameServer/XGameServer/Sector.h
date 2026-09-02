// Sector.h
// CSector - Sector management class for maze/area system
// Precisely restored from IDA GameServer.exe

#pragma once

#include <cstdint>
#include <map>
#include <list>
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"

// Forward declarations
class XMaze;
class XGameMode;
class CUser;
class CMonster;
class XActor;
struct VSectorStartBoxInfo;
struct VMonsterSpawnInfo;
struct STMagePotalBox;
struct XVec3;

// E_SECTOR_CLEAR_TYPE - Sector clear type enumeration
enum E_SECTOR_CLEAR_TYPE {
    E_SECTOR_CLEAR_TYPE_KILL_RATIO = 0,    // Kill monster ratio
    E_SECTOR_CLEAR_TYPE_SCRIPT = 1,         // Script controlled
    E_SECTOR_CLEAR_TYPE_QUEST = 2,          // Quest controlled
    E_SECTOR_CLEAR_TYPE_SPAWN_BOX = 3,      // Spawn box controlled
    E_SECTOR_CLEAR_TYPE_SCRIPT_CALL = 4,    // Script call controlled
};

// E_SECTOR_TYPE - Sector type enumeration
enum E_SECTOR_TYPE {
    E_SECTOR_TYPE_NORMAL = 0,
    E_SECTOR_TYPE_BOSS = 1,
};

// ============================================================================
// VSectorBoxInfo - Sector box info structure with position bounds
// IDA: Contains iID, iUniqueID, and position boundaries
// ============================================================================
struct VSectorBoxInfo {
    hkvVec3 PosTopLeft;        // Top-left corner position
    hkvVec3 PosBottomRight;    // Bottom-right corner position
    int iID;                   // Sector ID
    int iUniqueID;             // Unique ID

    int GetID() const { return iID; }
    int GetUniqueID() const { return iUniqueID; }
};

// VSectorBox - Alias for backwards compatibility
// IDA: Some code uses VSectorBox, others use VSectorBoxInfo
struct VSectorBox : public VSectorBoxInfo {
    // Per IDA CSector::InitClearType: m_eClearType at offset
    E_SECTOR_CLEAR_TYPE m_eClearType;   // Clear type
    E_SECTOR_TYPE m_eType;              // Sector type (normal/boss)
    char m_szClearScript[256];          // Clear script name

    VSectorBox() : m_eClearType(E_SECTOR_CLEAR_TYPE_KILL_RATIO), m_eType(E_SECTOR_TYPE_NORMAL) {
        m_szClearScript[0] = '\0';
    }
};

// ST_LUA_CLIENT_SYNC - Lua client sync structure
// Per IDA analysis: used for synchronizing Lua script values to client
struct ST_LUA_CLIENT_SYNC {
    std::uint64_t nLimitTime;  // Tick count deadline
    int nType;                  // Sync type
    int nValue;                 // Sync value
};

// Include E_SEND_INFO_TYPE enum definition
// Note: BattleZone.h defines this enum, we use it for SpawnMonster signature
#include "Soulworker/GameServer/XGameServer/BattleZone.h"

// eGAMEMODE_TYPE - Game mode type enumeration
enum eGAMEMODE_TYPE {
    eGAMEMODE_TYPE_NONE = 0,
    eGAMEMODE_TYPE_NORMAL = 1,
    eGAMEMODE_TYPE_PVP = 2,
    eGAMEMODE_TYPE_PARTY_QUEST = 3,
    eGAMEMODE_TYPE_SURVIVAL = 4,
    eGAMEMODE_TYPE_BOSS = 5,
    eGAMEMODE_TYPE_OPERATION = 6,
};

// ============================================================================
// CSector - Sector class for managing maze sectors
// IDA confirmed size: TBD
//
// Function: Manages individual sectors within a maze, including:
// - Actor (monster/NPC) management
// - Spawn box management
// - Portal management
// - Sector clear conditions
// - Roguelike mode support
// - Step spawn system
// ============================================================================
class CSector {
public:
    // === Constructor/Destructor ===
    // IDA: ??0CSector@@QEAA@XZ (0x1406C9F10)
    CSector();

    // IDA: ??1CSector@@QEAA@XZ (0x1406CA120)
    ~CSector();

    // === Update ===
    // IDA: ?OnUpdate@CSector@@QEAA_NM@Z (0x1406CA220)
    bool OnUpdate(float fElapsed, float fTime);

    // === Actor Management ===
    // IDA: ?AddActor@CSector@@QEAAXKPEAVXActor@@@Z (0x1406CA630)
    void AddActor(unsigned int dwID, XActor* pActor);

    // IDA: ?DeleteActor@CSector@@QEAAXKAEA_N_N@Z (0x1406CA7A0)
    void DeleteActor(unsigned int dwID, bool* bBossDie, bool bCheckMonsterCount);

    // === Initialization ===
    // IDA: ?InitClearType@CSector@@QEAAXXZ (0x1406CA970)
    void InitClearType();

    // IDA: ?SetAI@CSector@@QEAAX_N@Z (0x14028D460)
    void SetAI(bool bEnable);

    // === Game Mode ===
    // IDA: ?SetModeState@CSector@@QEAAXH@Z (0x1406CA9B0)
    void SetModeState(int nState);

    // IDA: ?GetGameModeType@CSector@@QEAA?AW4eGAMEMODE_TYPE@@XZ (0x1406CA9F0)
    eGAMEMODE_TYPE GetGameModeType();

    // IDA: ?IsTerminateSpawn@CSector@@QEAA_NXZ (0x1406CA9C0)
    bool IsTerminateSpawn();

    // IDA: ?DamageMonster@CSector@@QEAAXPEAVCMonster@@@Z (0x1406CAA20)
    void DamageMonster(CMonster* pMonster);

    // IDA: ?InteractBoxOnMode@CSector@@QEAAXPEAVCUser@@H@Z (0x1406CAA80)
    void InteractBoxOnMode(CUser* pUser, int nBoxID);

    // IDA: ?DiePlayer@CSector@@QEAAXPEAVCUser@@_N@Z (0x1406CAAF0)
    void DiePlayer(CUser* pUser, bool bAllDie);

    // IDA: ?EnterMode@CSector@@QEAAXPEAVCUser@@@Z (0x1406CAB80)
    void EnterMode(CUser* pUser);

    // === Monster Count ===
    // IDA: ?CheckMonsterCount@CSector@@QEAAHXZ (0x1406CABC0)
    int CheckMonsterCount();

    // IDA: ?GetMonsterCount@CSector@@QEAAHXZ (0x1406CACC0)
    int GetMonsterCount();

    // IDA: ?GetNormalMonsterCount@CSector@@QEAAHXZ (0x1406CAD50)
    int GetNormalMonsterCount();

    // IDA: ?CheckLastMonsterDie@CSector@@QEAAXXZ (0x1406CAE40)
    void CheckLastMonsterDie();

    // IDA: ?CheckLastMonsterTime@CSector@@QEAAXM@Z (0x1406CAF80)
    void CheckLastMonsterTime(float fTime);

    // === Monster Death ===
    // IDA: ?DieMonsters@CSector@@QEAAXH_N@Z (0x1406CB6B0)
    void DieMonsters(int nMonsterType, bool bSuicide);

    // IDA: ?DieMonstersAll@CSector@@QEAAX_N@Z (0x1406CB8D0)
    void DieMonstersAll(bool bSuicide);

    // === Spawn Box Management ===
    // IDA: ?EraseSpawnBoxID@CSector@@QEAAXH@Z (0x1406CBAC0)
    void EraseSpawnBoxID(int nUniqueBoxID);

    // IDA: ?EraseRespawnBoxID@CSector@@QEAAXH@Z (0x1406CBB70)
    void EraseRespawnBoxID(int nUniqueBoxID);

    // IDA: ?SectorClear@CSector@@QEAAXXZ (0x1406CBC40)
    void SectorClear();

    // === Portal ===
    // IDA: ?SetPortalState@CSector@@QEAAXXZ (0x1406CBC70)
    void SetPortalState();

    // IDA: ?AddSpawnBox@CSector@@QEAAXPEAUVMonsterSpawnInfo@@@Z (0x1406CBDD0)
    void AddSpawnBox(VMonsterSpawnInfo* pSpawnBox);

    // IDA: ?AddSpawnBoxID@CSector@@QEAAXH@Z (0x1406CBE00)
    void AddSpawnBoxID(int nBoxUniqueID);

    // IDA: ?AddRespawnBoxID@CSector@@QEAAXH@Z (0x1406CBE40)
    void AddRespawnBoxID(int nUniqueBoxID);

    // IDA: ?TerminateSpawn@CSector@@QEAAXXZ (0x1406CBE80)
    void TerminateSpawn();

    // IDA: ?SpawnMonster@CSector@@QEAAXW4E_SEND_INFO_TYPE@IXArea@@@Z (0x1406CBEF0)
    void SpawnMonster(E_SEND_INFO_TYPE eType);

    // IDA: ?IsSpawnedAll@CSector@@QEAA_NXZ
    bool IsSpawnedAll() { return m_bSpawnedAll; }

    // === Step Spawn System ===
    // IDA: ?SetStepSpawn@CSector@@QEAAXHH@Z (0x1406CC050)
    void SetStepSpawn(int nIndex, int nRate);

    // IDA: ?SetStepStop@CSector@@QEAAX_N@Z (0x1406CC080)
    void SetStepStop(bool bStop);

    // IDA: ?CheckStepCondition@CSector@@QEAAXXZ (0x1406CC100)
    void CheckStepCondition();

    // === Sector Info ===
    // IDA: ?IsBossSector@CSector@@QEAA_NXZ (0x1406CC270)
    bool IsBossSector();

    // IDA: ?ShowSectorInfo@CSector@@QEAAXPEAVCUser@@@Z (0x1406CC2A0)
    void ShowSectorInfo(CUser* pUser);

    // IDA: ?SendChatMessage@CSector@@QEAAXPEAVCUser@@PEA_W@Z (0x1406CC5F0)
    void SendChatMessage(CUser* pUser, wchar_t* szMeg);

    // === Position ===
    // IDA: ?GetStartPos@CSector@@QEAA_NAEAUXVec3@@AEAM@Z (0x1406CC7E0)
    bool GetStartPos(XVec3& vStartPos, float& fRot);

    // === Portal ===
    // IDA: ?AddPortal@CSector@@QEAAXPEAUSTMagePotalBox@@@Z (0x1406CC8D0)
    void AddPortal(STMagePotalBox* pPortal);

    // === Roguelike ===
    // IDA: ?StartRogueSector@CSector@@QEAAXXZ (0x1406CC910)
    void StartRogueSector();

    // IDA: ?CheckRogueLimitTime@CSector@@QEAAXXZ (0x1406CC970)
    void CheckRogueLimitTime();

    // IDA: ?SetRoguelikeState@CSector@@QEAAXH@Z (0x1406CC9D0)
    void SetRoguelikeState(int nState);

    // === Quest/Script ===
    // IDA: ?CallPartyQuestScript@CSector@@QEAAXXZ (0x1406CCA80)
    void CallPartyQuestScript();

    // IDA: ?CheckClearState@CSector@@QEAAXXZ (0x1406CCB00)
    void CheckClearState();

    // === Lua Client Sync ===
    // IDA: ?AddLuaClientSync@CSector@@QEAAXAEAUST_LUA_CLIENT_SYNC@@@Z (0x1406CCBC0)
    void AddLuaClientSync(ST_LUA_CLIENT_SYNC& stSync);

    // IDA: ?SendLuaClientSync@CSector@@QEAAXPEAVCUser@@@Z (0x1406CCBF0)
    void SendLuaClientSync(CUser* pUser);

    // === Debug ===
    // IDA: ?ShowSectorMonsterInfo@CSector@@QEAAXPEAVCUser@@@Z (0x1406CCDD0)
    void ShowSectorMonsterInfo(CUser* pUser);

    // === Helper Functions ===
    // GetSectorBoxID - Returns sector box ID
    int GetSectorBoxID() const;

    // GetSectorBoxUniqueID - Returns sector box unique ID
    int GetSectorBoxUniqueID() const;

    // GetSpawnBoxCount - Returns spawn box count
    int GetSpawnBoxCount() const;

    // GetRespawnBoxCount - Returns respawn box count
    int GetRespawnBoxCount() const;

    // === Setters ===
    void SetMaze(XMaze* pMaze) { m_pMaze = pMaze; }
    void SetGameMode(XGameMode* pGameMode) { m_pGameMode = pGameMode; }
    void SetSectorBox(VSectorBox* pBox) { m_pSectorBox = pBox; }
    void SetSectorStartBox(VSectorStartBoxInfo* pBox) { m_pSectorStartBox = pBox; }
    void SetRogueKey(int nKey) { m_nRogueKey = nKey; }
    void SetRoguelikeLimitTime(unsigned int dwTime) { m_dwRoguelikeLimitTime = dwTime; }

    // === Sector start box accessor ===
    // IDA 0x14031F9B0 (XMaze::SpawnSectorMonsterForOpt): 经 CSector+0x30
    // m_pSectorStartBox 读 +444 m_iRelativeSectorID 修正 nEnd。
    // PDB 修正: CSector+0x30 实际类型为 VSectorBoxInfo* (UDT 0x7426B, 468B)，
    // 早前落地为 VSectorStartBoxInfo* (UDT 0x4AA64, 168B) 属臆造；
    // 完整 VSectorBoxInfo 定义待 CSector 布局对齐批次统一还原。
    VSectorStartBoxInfo* GetSectorStartBox() const { return m_pSectorStartBox; }

    // === Getters ===
    XMaze* GetMaze() const { return m_pMaze; }
    XGameMode* GetGameMode() const { return m_pGameMode; }
    VSectorBox* GetSectorBox() const { return m_pSectorBox; }
    bool IsComplete() const { return m_bComplete; }
    bool IsCanAI() const { return m_bCanAI; }
    int GetRogueKey() const { return m_nRogueKey; }
    int GetRoguelikeState() const { return m_nRoguelikeState; }

    // IDA: ?GetActor@CSector@@QEAAPEAV?$map@KPEAVXActor@@...@@Z (0x14002F250)
    // Returns pointer to actor map
    std::map<unsigned int, XActor*>* GetActor() { return &m_mapActor; }

protected:
    // === IDA confirmed member variables ===

    // Maze reference
    XMaze* m_pMaze;

    // Game mode reference
    XGameMode* m_pGameMode;

    // Sector flags
    bool m_bAI;                  // AI enabled
    bool m_bCanAI;               // Can enable AI
    bool m_bComplete;            // Sector completed
    bool m_bCompleteScriptCall;
    bool m_bSendQuestMsg;
    bool m_bPortalOpen;          // Portal is open
    bool m_bOpenPortal;
    bool m_bInitOpenQuestCondition;
    bool m_bTerminateSpawn;      // Spawn terminated
    bool m_bStopStepSpawn;       // Step spawn stopped
    bool m_bSpawnedAll;          // All monsters spawned

    // Sector type
    E_SECTOR_TYPE m_eSectorType;

    // Game mode
    eGAMEMODE_TYPE m_eGameModeType;
    int m_nModeState;

    // Monster percentage
    float m_fMonPercent;

    // Max monster count
    int m_nMaxMonCount;

    // Sector time
    float m_fSectorTime;

    // Actor map (key: actor ID, value: XActor*)
    std::map<unsigned int, XActor*> m_mapActor;

    // Boss monster map (key: monster table ID, value: count)
    std::map<int, int> m_mapBossMonster;

    // Spawn box list
    std::list<VMonsterSpawnInfo*> m_listSpawnBox;

    // Spawn box ID vector
    std::vector<int> m_vecSpawnBoxID;

    // Respawn box ID vector
    std::vector<int> m_vecRespawnBoxID;

    // Spawn box ID map
    std::map<int, int> m_mapSpawnBoxID;

    // Respawn box ID map
    std::map<int, int> m_mapRespawnBoxID;

    // Step spawn variables
    int m_nNowStepSpawn;
    bool m_bChangeStepState;
    bool m_bCallQuestScript;
    bool m_bCallOpenPortalState;

    // Step spawn rate array (10 steps max)
    int m_nStepSpawnRate[10];
    bool m_bStepSpawned[10];

    // Lua client sync list
    std::list<ST_LUA_CLIENT_SYNC> m_listLuaClientSync;

    // Portal box map
    std::map<int, STMagePotalBox*> m_mapPotalBox;

    // Sector box
    VSectorBox m_SectorBox;

    // Sector box reference
    VSectorBox* m_pSectorBox;

    // Sector start box reference
    VSectorStartBoxInfo* m_StartBox;
    VSectorStartBoxInfo* m_pSectorStartBox;

    // Check last monster time
    float m_fCheckLastMonsterTime;
    int m_nCheckLastMonsterHP;

    // Roguelike variables
    int m_nRogueKey;
    unsigned long m_dwRoguelikeLimitTime;
    std::uint64_t m_dwRoguelikeLimitTickcount;
    int m_nRoguelikeState;
};
