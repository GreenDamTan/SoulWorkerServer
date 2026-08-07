// ThreadLocalData.h
// Thread-local data storage for GameServer
// Reconstructed from IDA decompilation

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <memory>
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

// Forward declarations
struct ST_GM_TIME_EVENT_INFO;
class CAi;
class CMonster;
class CUser;
class XArea;
class XDistrict;
class XMaze;
class XMyRoom;
class XModeMaze;
class CBattleZone;
class XPartyManager;
class XForceManager;
class XMonsterMgr;
class XNpcMgr;
class XAkashicObjectMgr;
class ProjectileMgr;
class TrapMgr;
class ChainLightningMgr;
class InteractionObjectMgr;
class VaccumCubeObjectMgr;
class SocialItemObjectMgr;
class GameModeMgr;
class GameScriptManager;
class VScriptResourceManager;
class CVaccumCube;
class VGameProjectileObject;
class VGameTrapObject;
class VChainLightningObject;

// HavokNavMeshResource - forward declaration
class HavokNavMeshResource;

// IVScriptManager - Vision Engine Script Manager Interface
class IVScriptManager {
public:
    virtual ~IVScriptManager() {}
    virtual class IVScriptInstance* CreateScriptInstanceFromFile(const char* szFilePath) = 0;
};

// IVScriptInstance methods that need to be declared
class IVScriptInstanceBase {
public:
    virtual void AssertValid() = 0;
};

// VScriptResourceManager - Vision Script Resource Manager
class VScriptResourceManager {
public:
    VScriptResourceManager() = default;
    ~VScriptResourceManager() = default;

    // Get the IVScriptManager interface
    IVScriptManager* GetIVScriptManager() {
        return &m_scriptManager;
    }

    // Get used resource count
    int GetUsedResourceCount() {
        return static_cast<int>(m_resources.size());
    }

private:
    // Stub script manager implementation
    class StubScriptManager : public IVScriptManager {
    public:
        IVScriptInstance* CreateScriptInstanceFromFile(const char* szFilePath) override {
            // TODO: Implement actual script loading
            (void)szFilePath;
            return nullptr;
        }
    };
    StubScriptManager m_scriptManager;
    std::vector<void*> m_resources;
};

// DohHavokResourceManager - forward declaration (implemented in XCore/XArea)
class DohHavokResourceManager;

// Include UXMapID from PSCommon.h instead of forward declaring
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerLeague.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMyroom.h"

struct XVec3;
struct TB_DISTRICT;
struct TB_MAZE_INFO;
struct ST_CREATE_MAZE;
struct ST_CHANNEL_INFO;
struct ST_LEAGUE_MEMBER_EX;
struct ST_LEAGUE_INFO_UPDATE;
struct ST_LEAGUE_MEMBER_UPDATE;
struct ST_LEAGUE_APPLICANT;
struct ST_LEAGUE_BOARD;
struct ST_LEAGUE_INFO;
struct ST_LEAGUE_AUTH_CHANGE;
struct SS_REPORT_POOL_INFO;
class XSendPacket;
class XClient;

template <typename T>
class TXPool;

// Pool creator types
namespace ThreadLocalDataTypes {
    struct OBJECT_POOL_INFO {
        int nDistrictMaxCount;
        int nMonsterMaxCount;
        int nNPCMaxCount;
        int nAkashicMaxCount;
        int nProjectileMaxCount;
        int nTrapMaxCount;
        int nInteractionMaxCount;
        int nMazeMaxCount;
    };
}

// ============================================================================
// ThreadLocalData - Thread-local storage for game objects
// IDA Constructor @ 0x1406D0A50
// IDA Size: 0x1450 bytes
// ============================================================================
class ThreadLocalData {
public:
    // IDA @ 0x1406D08F0 - Create thread-local instance
    static ThreadLocalData* CreateInstance(int threadCount, int ownerThreadIndex);

    // IDA @ 0x1406D0A50 - Constructor
    ThreadLocalData();

    // IDA @ 0x1406D1870 - Destructor
    ~ThreadLocalData();

    // IDA @ 0x1406D0C70 - Initialize member variables
    void Initialize();

    // IDA @ 0x1406D0D00 - Initialize object pools
    void InitPool();

    // IDA @ 0x1406D1A60 - Get current thread's instance
    static ThreadLocalData* GetInstance();

    // IDA @ 0x1406D1A80 - Get timer
    static class VDefaultTimer* GetTimer();

    // IDA @ 0x1406D1780 - Destroy thread-local instance
    static void DestroyInstance();

    // IDA @ 0x1406D14C0 - Clear all data
    void Clear();

    // IDA @ 0x1406D1AA0 - Load all navigation meshes
    void LoadAllNavmesh();

    // IDA @ 0x1406D5E80 - Load all scripts
    void LoadAllScript();

    // Area management
    // IDA @ 0x1406D5910 - Add area to thread
    void AddArea(XArea* pArea);

    // IDA @ 0x1406D5DA0 - Find area by map ID
    XArea* FindArea(UXMapID uxMapID);

    // IDA @ 0x1406D5E10 - Check if area belongs to this thread
    bool IsThreadArea(UXMapID uxMapID);

    // IDA @ 0x1406D39C0 - Clear all areas
    void ClearArea();

    // IDA @ 0x1406D3B90 - Clear all AI
    void ClearAi();

    // Get Havok resource manager
    DohHavokResourceManager* GetDohHavokResourceManager() { return m_DohHavokResourceManager; }

    // District/BattleZone/Maze creation
    // IDA @ 0x1406D1DA0 - Create district
    bool CreateDistrict(UXMapID uxMapID, TB_DISTRICT& tblDistrict);

    // IDA @ 0x1406D1F20 - Create battle zone
    bool CreateBattleZone(UXMapID uxMapID, TB_MAZE_INFO& tblMazeInfo);

    // IDA @ 0x1406D2080 - Create maze
    void CreateMaze(ST_CREATE_MAZE& stCreateMaze);

    // IDA @ 0x1406D4A00 - Delete maze
    void DeleteMaze(UXMapID uxMapID);

    // IDA @ 0x1406D3CA0 - Enter area
    bool EnterArea(CUser* pUser, PS_PARTY_INFO& stPartyInfo);

    // IDA @ 0x1406D4E20 - Leave area
    bool LeaveArea(XClient* pClient);

    // IDA @ 0x1406D5410 - Change map
    bool ChangeMap(XClient* pClient, UXMapID uxMapID, XVec3& vPos, float fRot);

    // Monster management
    // IDA @ 0x1406D88B0 - Create monster
    CMonster* CreateMonster(XArea* pArea, UXMapID uxMapID, int nMonsterID, XVec3 vPos, float fRot);

    // IDA @ 0x1406D8A30 - Delete monster
    void DeleteMonster(CMonster* pMonster);

    // NPC management
    // IDA @ 0x1406D8E60 - Create NPC
    class CNpc* CreateNpc(XArea* pArea, UXMapID uxMapID, std::uint32_t nSectorID, int nNpcID, XVec3* vPos, float fRot, int nUnused);

    // IDA @ 0x1406D8?? - Delete NPC
    void DeleteNpc(class CNpc* pNpc);

    // IDA @ 0x1406D8FB0 - Create AkashicObject
    // IDA: ?CreateAkashicObject@ThreadLocalData@@QEAAPEAVCAkashicObject@@PEAVXArea@@TUXMapID@@HUXVec3@@MK@Z
    class CAkashicObject* CreateAkashicObject(class XArea* pArea, UXMapID uxMapID, int nAkashicID, XVec3* vPos, float fRot, unsigned int dwParentID);

    // IDA @ 0x1406D?? - Delete AkashicObject
    void DeleteAkashicObject(class CAkashicObject* pAkashic);

    // IDA @ 0x1406D?? - Create InteractionObject
    class CInteractionObject* CreateInteractionObject(XVec3* vPos);

    // IDA @ 0x1406D?? - Delete InteractionObject
    void DeleteInteractionObject(class CInteractionObject* pObject);

    // IDA @ 0x1406D8D60 - Create vaccum cube object
    CVaccumCube* CreateVaccumCubeObject(XVec3 vPos);

    // IDA @ 0x1406D8DB0 - Delete vaccum cube object
    void DeleteVaccumCubeObject(CVaccumCube* pVaccumCube);

    // Projectile/Trap/ChainLightning management
    // Delete projectile object
    void DeleteProjectile(VGameProjectileObject* pProjectile);

    // Delete trap object
    void DeleteTrap(VGameTrapObject* pTrap);

    // Delete chain lightning object
    void DeleteChainLightning(VChainLightningObject* pChainLightning);

    // IDA @ 0x1406D59C0 - Add AI
    bool AddAi(CAi* pAi, CMonster* pMonster, const char* szScript);

    // Update
    // IDA @ 0x1406D61A0 - Update thread
    void Update(float fDeltaTime);

    // Broadcast
    // IDA @ 0x1406D6100 - Send broadcast
    void SendBroadcast(XSendPacket& xSendPacket);

    // IDA @ 0x1406D85A0 - Send time event to all areas
    void SendTimeEvent(ST_GM_TIME_EVENT_INFO& stInfo);

    // Channel management
    // IDA @ 0x1406D67D0 - Update all channels
    void UpdateChannelAll(PS_CHANNEL_INFO& stChannelInfo);

    // IDA @ 0x1406D6920 - Update a single channel entry
    void UpdateChannel(std::uint16_t wMapID, ST_CHANNEL_INFO& stChannel);

    // IDA @ 0x1406D6A50 - Send channel info
    void SendChannelInfo(CUser* pUser, std::uint16_t wChannelID);

    // IDA @ 0x1406D6EF0 - Send maze info
    void SendMazeInfo();

    // Script managers
    // IDA @ 0x14025E780 - Get script manager
    VScriptResourceManager& GetScriptManager();

    // IDA @ 0x14025E7A0 - Get game script manager
    GameScriptManager& GetGameScriptManager();

    // IDA @ 0x140310490 - Get game mode manager
    GameModeMgr* GetGameModeMgr();

    // IDA @ 0x1403E1E20 - Get report pool info
    SS_REPORT_POOL_INFO& GetReportPoolInfo();

    // League functions
    // IDA @ 0x1406D70A0 - Add league member
    void AddLeagueMember(CUser* pUser);

    // IDA @ 0x1406D7240 - Leave league member
    void LeaveLeagueMember(CUser* pUser);

    // IDA @ 0x1406D2960 - Create matching maze with force info
    void CreateMatchingMaze(ST_CREATE_MAZE& stCreateMaze, PS_FORCE_INFO& stForceInfo, unsigned long dwMatchingID);

    // IDA @ 0x1406D3340 - Create matching mode maze
    void CreateMatchingModeMaze(ST_CREATE_MODE_MAZE& stCreateMaze);

    // IDA @ 0x1406DA1B0 - Send league delegate
    void SendLeagueDelegate(PS_RES_LEAGUE_DELEGATE& psDelegateRes, unsigned int dwDelegatedUCID, int nSyncCount);

    // IDA @ 0x1406DA280 - Send league change name
    void SendLeagueChangeName(PS_RES_LEAGUE_NAME_CHANGE& psResChangeInfo, int nSyncCount);

    // IDA @ 0x1406DA340 - Update sync league load
    void UpdateSyncLeagueLoad(int nSyncCount);

    // IDA @ 0x1406DA3C0 - Send league inventory move
    void SendLeagueInventoryMove(unsigned int dwReqUCID, PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME& psItemMoveForServer);

    // IDA @ 0x1406DA650 - Send day event
    void SendDayEvent(void* psDayEvent);  // PS_DAY_EVENT_LIST* - forward declaration placeholder

    // IDA @ 0x1406DA6F0 - Send operation time
    void SendOperationTime();

    // IDA @ 0x1406DA780 - Send world event booster
    void SendWorldEventBooster(unsigned long dwBuff_ID, __int64 biEndDate);

    // IDA @ 0x1406DA820 - Compare league inventory count
    bool CompareLeagueInventoryCount(unsigned int nLeagueID, unsigned int dwUCID);

    // IDA @ 0x1406DA940 - Start world mode
    void StartWorldMode(ST_WORLD_MODE_INFO& stInfo);

    // IDA @ 0x1406DA9E0 - Finish world mode
    void FinishWorldMode(PS_WORLD_MODE_FINISH& stInfo);

    // IDA @ 0x1406DAA80 - Clear world mode
    void ClearWorldMode(ST_WORLD_MODE_INFO& stInfo);

    // IDA @ 0x1406DAB20 - Sync world mode
    void SyncWorldMode(ST_WORLD_MODE_INFO_VEC& stInfoVec);

    // IDA @ 0x1406DACA0 - Complete world mode
    void CompleteWorldMode(PS_WORLD_MODE_COMPLETE& stComplete, unsigned int dwMonsterID);

    // IDA @ 0x1406D9C20 - MyRoom pollen load
    void MyRoomPollenLoad(UXMapID uxMapID, void* psPollenList);  // PS_MYROOM_POLLEN_LIST* - forward declaration placeholder

    // IDA @ 0x1406D9D00 - MyRoom pollen add
    void MyRoomPollenAdd(int nErrorCode, UXMapID uxMapID, int nPollenIndex);

    // IDA @ 0x1406D9DA0 - MyRoom pollen cultivation
    void MyRoomPollenCultivation(int nErrorCode, UXMapID uxMapID, void* psInfo);  // PS_DB_MYROOM_POLLEN_CULTIVATION* - forward declaration placeholder

    // IDA @ 0x1406D9E40 - MyRoom pollen harvest
    void MyRoomPollenHarvest(int nErrorCode, UXMapID uxMapID, int nPollenIndex);

    // IDA @ 0x1406D9EE0 - MyRoom pollen help
    void MyRoomPollenHelp(int nErrorCode, UXMapID uxMapID, int nPollenIndex,
        PS_MYROOM_POLLEN_HELP_USER* psHelpUser, unsigned __int64 biHarvestDate, unsigned int dwOwnerUAID);

    // IDA @ 0x1406DA090 - MyRoom pollen item use
    void MyRoomPollenItemUse(int nErrorCode, UXMapID uxMapID, void* psPollenInfo);  // PS_MYROOM_POLLEN_INFO* - forward declaration placeholder

    // IDA @ 0x1406DA8A0 - MyRoom pollen cancel
    void MyRoomPollenCancel(int nErrorCode, UXMapID uxMapID, int nPollenIndex);

    // IDA @ 0x1406DABC0 - Appear event monster
    void AppearEventMonster(int nModeID, __int64 biStartTime, __int64 biFinishTime,
        int nModeDateID, __int64 biModeStartTime, __int64 biModeEndTime);

    // IDA @ 0x1406DAD50 - Update world mode
    void UpdateWorldMode(PS_WORLD_MODE_UPDATE& stUpdate);

private:
    // Member variables (layout from IDA)
    // Offset 0x00: vtable
    // Offset 0x08-0x??: various managers and pools

    // Counters
    int m_nMazeCount;           // Offset ~0x180
    int m_nMonsterCount;        // Offset ~0x184
    int m_nModeMazeCount;       // Offset ~0x188
    bool m_bInitPool;           // Offset ~0x18C
    bool m_bLoadWorld;          // Offset ~0x18D
    bool m_bReqSyncWorld;       // Offset ~0x18E
    std::uint64_t m_nLogSynctime;     // Offset ~0x190
    std::uint64_t m_nReportSynctime;  // Offset ~0x198

    // Thread info
    int m_nThreadCount;         // Offset 0x180 (384 decimal from IDA)
    int m_nOwnerThreadIndex;    // Offset 0x184 (388 decimal from IDA)

    // Managers - using raw pointers to avoid incomplete type issues in stub
    XPartyManager* m_xPartyMgr;
    XForceManager* m_xForceMgr;
    XMonsterMgr* m_xMonsterMgr;
    XNpcMgr* m_xNpcMgr;
    XAkashicObjectMgr* m_xAkashicMgr;
    ProjectileMgr* m_xProjectileMgr;
    TrapMgr* m_xTrapMgr;
    ChainLightningMgr* m_xChainLightningMgr;
    InteractionObjectMgr* m_xInteractionObjectMgr;
    VaccumCubeObjectMgr* m_xVaccumCubeObjectMgr;
    SocialItemObjectMgr* m_xSocialItemObjectMgr;
    GameModeMgr* m_xGameModeMgr;
    GameScriptManager* m_xLuaScript;
    VScriptResourceManager* m_xScriptManager;
    DohHavokResourceManager* m_DohHavokResourceManager;

    // Pools - using raw pointers
    TXPool<XDistrict>* m_xDistrictPool;
    TXPool<CBattleZone>* m_xBattleZonePool;
    TXPool<XMaze>* m_xMazePool;
    TXPool<XMyRoom>* m_xMyRoomPool;
    TXPool<XModeMaze>* m_xModeMazePool;

    // Creators
    void* m_pDistrictCreator;
    void* m_pBattleZoneCreator;
    void* m_pMazeCreator;
    void* m_pMyRoomCreator;
    void* m_pModeMazeCreator;

    // Maps
    std::map<std::string, CAi*> m_mapAi;
    std::map<int, XArea*> m_mapArea;
    std::map<std::uint16_t, PS_CHANNEL_INFO> m_mapChannelInfo;
    std::map<unsigned int, void*> m_mapLeagueMember;  // CLeagueMember* - forward declaration placeholder

    // Report pool info
    SS_REPORT_POOL_INFO m_ReportPoolInfo;
};
