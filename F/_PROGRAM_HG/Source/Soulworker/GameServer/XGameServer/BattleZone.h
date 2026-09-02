#pragma once

#include "Soulworker/GameServer/XCore/XArea/XDistrict.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XGameServer/RespawnManager.h"
#include "Soulworker/GameServer/XGameServer/VaccumManager.h"
#include "Soulworker/GameServer/XGameServer/GameWorldMode.h"
#include <cstdint>
#include <map>
#include <list>
#include <set>
#include <memory>
#include <vector>

// Forward declarations
struct TB_NPC;
struct ST_MONSTER_DAMAGE_INFO;  // Defined in Mover.h

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_StdCompat.h"

// TODO: 推测结果 - 来自 Vision Engine / IDA
// E_SEND_INFO_TYPE - 发送信息类型枚举
enum E_SEND_INFO_TYPE {
    eSendInfoTypeNone = 0,
    eSendInfoTypeSend = 1,      // IDA: send info packet
    eSendInfoTypeNot = 2,       // IDA: do not send info packet
    eSendInfoTypeAll = 3,
    eSendInfoTypeNearby = 4,
    eSendInfoTypeSelf = 5,
    // Legacy names for backward compatibility
    eAll = eSendInfoTypeAll,    // IDA shorthand
    E_SEND_INFO_TYPE_NONE = eSendInfoTypeNone,
    E_SEND_INFO_TYPE_SEND = eSendInfoTypeSend,
    E_SEND_INFO_TYPE_ALL = eSendInfoTypeAll,
    E_SEND_INFO_TYPE_NEARBY = eSendInfoTypeNearby,
    E_SEND_INFO_TYPE_SELF = eSendInfoTypeSelf,
};

// TUXActorID 结构
struct TUXActorID {
    std::uint16_t wType;
    std::uint16_t wID;

    TUXActorID() : wType(0), wID(0) {}
    TUXActorID(std::uint16_t wT, std::uint16_t wI) : wType(wT), wID(wI) {}

    bool operator==(const TUXActorID& other) const {
        return wType == other.wType && wID == other.wID;
    }
};

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
struct VPortalBoxInfo;  // Forward declaration
struct VCommonPositionBoxInfo;
struct STQuestMoveBox;
struct ST_WORLD_MODE_INFO;
struct ST_WORLD_MODE_INFO_VEC;
struct PS_WORLD_MODE_FINISH;
struct PS_WORLD_MODE_COMPLETE;
struct PS_WORLD_MODE_UPDATE;
// ST_MONSTER_DAMAGE_INFO is now defined in Mover.h
// Per IDA 0x1401A28F0 - STInteractionBox - Interaction box runtime state
// Used by CBattleZone::ClickInteractionBox
struct VInterActionBoxInfo;  // Forward declaration
struct STInteractionBox {
    int nBoxIndex = 0;              // Box index
    bool bEnable = true;            // Enable flag
    bool bShow = true;              // Show flag
    std::uint8_t _pad0[2] = {};     // padding
    int nCallCount = 0;             // Call count remaining (decrements on use)
    float fCoolTime = 0.0f;         // Current cooldown time
    std::uint32_t dwActorID = 0;    // Actor ID for interaction object
    VInterActionBoxInfo* pInteractionBox = nullptr;  // Pointer to interaction box info
};

// Per IDA - STMageGateBox - Gate box runtime state
// IDA: Contains bOpen flag
struct STMageGateBox {
    int nBoxIndex;              // Gate box index
    bool bOpen;                 // Is gate open
    std::uint8_t _pad0[3];      // padding for alignment
    void* pGateBoxInfo;         // Pointer to gate box info

    STMageGateBox()
        : nBoxIndex(0)
        , bOpen(false)
        , pGateBoxInfo(nullptr)
    {}
};

// Per IDA - STLuaFunctionBox - Lua function box runtime state
// IDA: Contains bCalled flag
struct STLuaFunctionBox {
    int nBoxIndex;              // Lua function box index
    bool bCalled;               // Has been called
    std::uint8_t _pad0[3];      // padding for alignment
    void* pLuaBoxInfo;          // Pointer to lua box info

    STLuaFunctionBox()
        : nBoxIndex(0)
        , bCalled(false)
        , pLuaBoxInfo(nullptr)
    {}
};

// Per IDA - STMagePotalBox (56 bytes)
// Portal box runtime state
struct STMagePotalBox {
    int nBoxIndex;              // offset 0 - Portal box index
    bool bOpen;                 // offset 4 - Is portal open
    std::uint8_t _pad0[3];      // padding for alignment
    VPortalBoxInfo* pPotalBox;  // offset 8 - Pointer to portal box info
    int nOpenRate[5];           // offset 16 - Open rates (20 bytes)
    int nOpenTryCount;          // offset 36 - Number of open attempts
    int nEnterUserCount;        // offset 40 - Number of users entered
    std::int64_t nCloseTime;    // offset 48 - Close timestamp

    STMagePotalBox()
        : nBoxIndex(0)
        , bOpen(false)
        , pPotalBox(nullptr)
        , nOpenTryCount(0)
        , nEnterUserCount(0)
        , nCloseTime(0)
    {
        for (int i = 0; i < 5; ++i) nOpenRate[i] = 0;
    }
};
static_assert(sizeof(STMagePotalBox) >= 56, "STMagePotalBox size check");

struct ST_KRR_MONSTER_INFO;     // Per IDA 0x1401A7FF0
class hkaiPointCloudSilhouetteGenerator;
class DohHavokNavMeshInstance;

// Forward declaration - TB_MONSTER is defined in TB_MONSTER.h
struct TB_MONSTER;

// Per IDA - VMonsterSpawnInfo 结构中的怪物信息
struct VMonsterSpawnInfo_MonsterInfo {
    int m_iID;                           // 怪物/NPC ID
    int m_iType;                         // 类型 (0=Monster, 1=NPC, 2/4=特殊怪物)
    int m_iChance;                       // 生成概率
    // ... 其他字段
};

// Per IDA - VMonsterSpawnInfo 生成箱信息
// IDA 0x1402AD220 GetSpawnPos 分析确认的字段偏移:
//   PosTopLeft at offset 0x14 (20)
//   PosBottomRight at offset 0x20 (32)
//   m_iCreationPositionType at offset 0x11C (284)
struct VMonsterSpawnInfo {
    int iID;                             // 生成箱ID (PDB VEventBoxInfo::iID 继承语义)
    // PDB fieldlist 0x72694: 原始继承 VEventBoxInfo(164B), iUniqueID 是基类成员 +12;
    // 活跃版以平铺替身复现, 此处按 PDB 名补齐
    int iUniqueID;                       // 事件唯一 ID (PDB VEventBoxInfo::iUniqueID 继承语义)
    int m_iSectorID;                     // 区域ID
    // IDA: PosTopLeft at offset 20 (0x14)
    struct { float x; float y; float z; } PosTopLeft;      // 生成区域左上角
    // IDA: PosBottomRight at offset 32 (0x20)
    struct { float x; float y; float z; } PosBottomRight;  // 生成区域右下角
    int m_iMaxEntityCount;               // 最大实体数量
    float fRotate;                       // 旋转角度
    int m_iGroupID;                      // 组ID
    int m_iMoveType;                     // 移动类型
    int m_iWaypoint;                     // 路点ID
    int m_iAggroGroupID;                 // 仇恨组ID
    int m_iAggroDistance;                // 仇恨距离
    int m_iAggroMaxCount;                // 最大仇恨数量
    float m_fTakeTargetRatio;            // 目标比率
    float m_RespawnTime;                 // 重生时间 (PDB +624)
    int m_iCreationCondition;            // 创建条件 (1=立即生成, 2=事件触发)
    char m_ChangeSpawnAction[256];       // 生成动作名称
    VMonsterSpawnInfo_MonsterInfo m_stMonsterInfo[10]; // 怪物信息数组
    int m_iWaitCreationSequenceType;     // 等待创建序列类型
    int m_iWaitCreationMaxWave;          // 等待创建最大波数
    float m_fWaitCreationDelayTime;      // 等待创建延迟时间
    float m_fWaitCreationSequenceTime;   // 等待创建序列时间
    // IDA: SetupScriptTraceHP 使用的字段
    int m_iScriptType;                   // 脚本类型 (1=单次检查, 2=多阶段检查)
    int m_iCheckScirptHP[5];             // 检查脚本HP百分比 (5个阶段)
    // IDA 0x1402AD220: m_iCreationPositionType at offset 0x11C (284)
    // 0 = 中心点, 1/2 = 随机位置
    int m_iCreationPositionType;         // 生成位置类型
    // IDA: m_szObjectKey - 对象键名 (used in AddDieMonsterSpawnBoxID)
    char m_szObjectKey[64];              // 对象键名
    // ---- 批10 补齐: PDB fieldlist 0x72694 尾部字段 (原活跃版缺失) ----
    int m_ProtectionTarget = 0;          // PDB +620
    int m_iStep = 0;                     // PDB +628
    int m_eRespawnType = 0;              // PDB +632 (SetQuestRespawn 判 == 1)
    int m_iRespawnCondition = 0;        // PDB +636 (SetQuestRespawn 传 CGocQuest::FindCondition)
    char m_CreationEffectFile[128] = {}; // PDB +640 (0x4CE1, 128B)
};

// Per IDA - STMageProcessSpawnBox 处理生成箱
struct STMageProcessSpawnBox {
    VMonsterSpawnInfo* pSpawnBox;        // 生成箱信息
    bool bActive;                        // 是否激活
    bool bSpawned;                       // 是否已生成
    bool bTerminate;                     // 是否终止
    int nSpawnOrder;                     // 生成顺序
    float nCreatedCount;                 // 已创建计数
    float fDelayTime;                    // 延迟时间
    // ... 其他字段
};

// Per IDA - STMageEventSpawnBox 事件生成箱
// IDA: Used in ExcuteEventSpawn
struct STMageEventSpawnBox {
    int nLoopCount;                      // 循环计数
    void* pEventBox;                     // 事件箱指针 (contains m_iCheckBox[10])
    // ... 其他字段

    STMageEventSpawnBox() : nLoopCount(0), pEventBox(nullptr) {}
};

// TODO: 需人工审查 - CMonster 最小定义（继承自 MoverEx.h）
// Note: XActor 是前置声明，实际定义在 MoverEx.h 中
class CMonster;

// TODO: 需人工审查 - CNpc 最小定义（继承自 MoverEx.h）
class CNpc;

// TODO: 推测结果 - 来自 IDA struct CBattleZone (1104 bytes)
class CBattleZone : public XDistrict {
public:
    // 构造/析构
    CBattleZone();
    virtual ~CBattleZone();

    // 核心生命周期
    bool Create(TUXMapID uxMapID, TB_MAZE_INFO* pMazeInfo);
    void Clear();
    void OnUpdate(float fDelta);
    void LoadComplete(XActor* pActor);

    // Per IDA 0x1401A08B0 - 创建怪物
    CMonster* CreateMonster(TUXMapID uxMazeSerialID, int nSectorID, unsigned int nMonsterID,
                            XVec3 vPos, float fRot, E_SEND_INFO_TYPE eType,
                            int nSpawnBoxID, int nGroupID, TUXActorID uxParentID);

    // Per IDA 0x14019EFE0 - 删除怪物
    void DeleteMonster(CMonster* pMonster);

    // Per IDA 0x1401A5E60 - 批量杀死怪物
    void DieMonster(std::list<std::uint32_t>& listMonsterID, bool bForce);
    void DieMonster(unsigned long dwListID);  // Kill monsters by list ID

    // Per IDA 0x1401A71D0 - 杀死所有怪物
    void DieMonsterAll(bool bForce);
    void DieMonsterAll();  // Kill all monsters in zone

    // Per IDA 0x1401A7BC0 - 保存伤害信息
    void SaveDamageInfo(std::list<ST_MONSTER_DAMAGE_INFO> listHitID);

    // Per IDA 0x1401A7FF0 - 初始化KRR怪物
    void InitKRRMonster();

    // Per IDA 0x1401A8410 - 发送WorldMode信息
    void SendWorldModeInfo(XActor* pActor);

    // Per IDA 0x1401A4410 - 处理怪物任务
    void ProcessMonsterQuest(XActor* pAttacker, std::uint32_t nMonsterID);

    // Per IDA 0x1401A6910 - WorldMode掉落物品
    void DropItemForWorldMode(std::uint32_t dwMonsterID, int nModeDateID, bool bComplete);
    void DropItemForWorldMode(CMonster* pMonster, int nDropType);  // Drop items for world mode

    void MonsterDieForEvent(CMonster* pMonster, std::uint32_t dwKillerID);
    void MonsterDieForEvent(unsigned long dwMonsterID, int nEventType);  // Kill event monster

    // Per IDA XArea::FindActor (0x1408EF530) - 查找怪物
    CMonster* FindMonster(std::uint32_t dwActorID);

    // Per IDA XArea::GetActorCount (0x1408EF570) - 获取怪物数量
    int GetMonsterCount();

    // Per IDA XArea::GetActorCount (0x1408EF570) - 获取玩家数量
    int GetPlayerCount();

    // Per IDA XArea::GetActorCount (0x1408EF570) - 获取指定类型 Actor 数量
    int GetActorCount(E_ACTOR_TYPE eType);

    // Per IDA 0x1401A11E0 - 创建NPC
    CNpc* CreateNpc(TUXMapID uxMazeSerialID, int nSectorID, unsigned int nNpcID,
                    XVec3 vPos, float fRot);

    // Per IDA 0x1401A1320 - 删除NPC
    void DeleteNpc(CNpc* pNpc);

    // Akashic Object
    CAkashicObject* CreateAkashicObject(TUXMapID uxMapID, int nTableID, XVec3 vPos, float fYaw, float fScale, E_SEND_INFO_TYPE eSendType);
    CMonster* CreateAkashicObject(int nObjectType, int nObjectID);  // Create akashic object
    void DeleteAkashicObject(CAkashicObject* pObject);

    // 交互对象
    CInteractionObject* CreateInteractionObject(STInteractionBox* pBox, void* pTBInteraction, XVec3& vPos, float fYaw);
    CMonster* CreateInteractionObject(int nNpcID, TB_NPC* pNpcData, const XVec3& vPos);  // Create interaction object
    void DeleteInteractionObject(CInteractionObject* pObject);
    void ClickInteractionBox(int nBoxID, CUser* pUser);
    void ClickInteractionBox(CUser* pUser, unsigned long dwObjectID, int nClickType);  // Handle interaction box click

    // 区域相关
    void ExitArea(XActor* pActor);
    bool IsInSafetyZone(XActor* pActor);

    // 生成
    void Generate();
    void SpawnEventMapNpc();  // Per IDA 0x14019D880: 生成事件地图 NPC
    void SpawnGenerateMonster();
    void ExcuteSpawnBox(const VMonsterSpawnInfo* pSpawnInfo, E_SEND_INFO_TYPE eSendType);
    void ExcuteSpawnBox(STMageProcessSpawnBox* pSpawnBox, E_SEND_INFO_TYPE eSendType);
    void ExcuteSpawn(int nBoxIndex, int nSpawnIndex, const VMonsterSpawnInfo* pSpawnInfo, E_SEND_INFO_TYPE eSendType);

    // Per IDA 0x1401A5B40 - 检查并激活生成箱
    void ExcuteSpawnBoxCheck(int nBoxID, E_SEND_INFO_TYPE eSendType, bool bLuaCall);

    // Per IDA 0x1401A5CE0 - 添加怪物生成箱信息映射
    void AddMonsterSpawnInfo(int nBoxID, unsigned int dwMonsterID);

    // Per IDA - 辅助函数
    int GetUniqueID(int nBoxID);
    void GetSpawnPos(const VMonsterSpawnInfo* pMonsterSpawn, XVec3* pPos);

    // World Mode
    void StartWorldMode(ST_WORLD_MODE_INFO& stInfo);
    void FinishWorldMode(PS_WORLD_MODE_FINISH& stFinish);
    void ClearWorldMode(ST_WORLD_MODE_INFO& stInfo);
    void ClearWorldMode();  // Clear world mode state
    void SyncWorldMode(ST_WORLD_MODE_INFO_VEC& stInfoVec);
    void SyncWorldMode();  // Sync world mode to clients
    void CompleteWorldMode(PS_WORLD_MODE_COMPLETE& stComplete, std::uint32_t dwKillerID);
    void UpdateWorldMode(PS_WORLD_MODE_UPDATE& stUpdate);
    void UpdateWorldMode(float fElapsedTime);  // Update world mode (tick)
    bool IsWorldModeBoss();
    bool AlreadyInWorldMode();

    // Drop
    bool ProcessDrop(XActor* pAtk, int nMonsterID, XVec3& vPos);
    bool ProcessDrop(XActor* pAtk, CMonster* pMonster, XVec3& vPos);
    void ProcessDropByHit(unsigned int dwAtkUser, int nDropID, int nMonsterLevel, XVec3& vPos, int nMonsterID);

    // Quest (ProcessMonsterQuest 声明已移到上方主要函数区)
    void RunQuestMoveCheck(int nQuestID, CUser* pUser);

    // Portal
    void SendPotalInfos(XActor* pActor);
    void SetPotalFlag(int nIndex, bool bFlag);
    void UpdatePotalFlag(int nIndex);

    // 其他
    void ShowBattleZoneInfo(CUser* pUser);
    bool CreateNavMesh(const char* szPath);
    DohHavokNavMeshInstance* GetNavMeshInstance();
    int GetWorldType();
    CRespawnManager* GetRespawnManager();
    CVaccumManager* GetVaccumManager();
    // Per IDA 0x1401A7C60: AppearEventMonster - Broadcast world mode start to all players
    void AppearEventMonster(int nModeID, std::int64_t biStartTime, std::int64_t biFinishTime, int nModeDateID, std::int64_t biModeStartTime, std::int64_t biModeEndTime);

    // Per IDA 0x1402D0820: SetWorldModeBoostAll - Apply booster to all users in zone
    void SetWorldModeBoostAll(int nBoostID, std::int64_t nEndDate);

    // === Spawn System ===
    // Spawn monster at specified position
    CMonster* SpawnMonster(unsigned int nMonsterID, XVec3 vPos, float fRot, int nGroupID = 0);
    // Despawn monster from zone
    void DespawnMonster(CMonster* pMonster);
    // Spawn NPC at specified position
    CNpc* SpawnNPC(unsigned int nNpcID, XVec3 vPos, float fRot);
    // Remove NPC from zone
    void DespawnNPC(CNpc* pNpc);
    // Schedule NPC respawn
    void RespawnNPC(unsigned int nNpcID, XVec3 vPos, float fRot, float fDelayTime);

    // === Event System ===
    // Trigger zone event
    void TriggerEvent(int nEventID, int nEventType);
    // Process event logic
    void ProcessEvent(int nEventID, float fDelta);
    // End current event
    void EndEvent(int nEventID);
    // Check event conditions
    bool CheckEvent(int nEventID);

    // === Portal System ===
    // Enable portal
    void ActivatePortal(int nPortalID);
    // Disable portal
    void DeactivatePortal(int nPortalID);
    // Check portal conditions
    bool CheckPortal(int nPortalID, CUser* pUser);
    // Get portal list
    std::vector<int> GetPortalList();

    // === Quest System ===
    // Start zone quest
    void StartQuest(int nQuestID, CUser* pUser);
    // Complete quest
    void EndQuest(int nQuestID, CUser* pUser, bool bSuccess);
    // Check quest progress
    bool CheckQuest(int nQuestID, CUser* pUser);
    // Get quest list for user
    std::vector<int> GetQuestList(CUser* pUser);

    // === Maze System ===
    // Player enters maze
    void EnterMaze(CUser* pUser, int nMazeID);
    // Player exits maze
    void ExitMaze(CUser* pUser);
    // Maze logic update
    void ProcessMaze(float fDelta);
    // User enter zone
    void EnterUser(CUser* pUser);
    // User exit zone
    void ExitUser(CUser* pUser);
    // Get all users in zone
    std::vector<CUser*> GetUserList();
    // Get all monsters in zone
    std::vector<CMonster*> GetMonsterList();

    // Per IDA 0x1401A73D0: IsEnemyPVP - Check if two actors are PVP enemies
    bool IsEnemyPVP(XActor* pAtk, XActor* pDef);

    // Per IDA 0x1401A59C0: ChangePacketOptimization_GM - GM command to change packet optimization
    void ChangePacketOptimization_GM(float fOpt);

    // Per IDA 0x1401A5A80: ResetPacketOptimization_GM - GM command to reset packet optimization
    void ResetPacketOptimization_GM();

    // Per IDA 0x1401A2360: AddDestoryObject - Add actor to pending destroy list
    void AddDestoryObject(XActor* pActor);

    // Per IDA 0x1401A2740: EnableInteractionBox - Enable/disable interaction box
    void EnableInteractionBox(int nBoxIndex, bool bEnable);

    // Per IDA 0x1401A7A10: SetSummonMonsterDelete - Set summon monster for deletion with animation
    void SetSummonMonsterDelete(unsigned int dwTBID, unsigned int dwOwnerID, char* szAnim);

    // Per IDA 0x1401A4940: ClickVaccumCube - Handle vaccum cube click interaction
    bool ClickVaccumCube(int nInteractionID, CUser* pUser);

    // Per IDA 0x1401A6CA0: SetWorldModeSync - Sync world mode state to user
    void SetWorldModeSync(CUser* pUser);

    // IDA 确认的成员变量 (offset from struct start)
private:
    // === 来自 XDistrict 基类 (0-415) ===

    // === CBattleZone 本身成员 (416+) ===
    // offset 416: m_tQuestUpdate (ATL::CTime, 8 bytes)
    // TODO: 推测目录归属 - 需要确认 ATL::CTime 类型定义
    std::uint64_t m_tQuestUpdate_dummy;  // TODO: 需人工审查 - 替换为 ATL::CTime

    // offset 424: m_fUpdatePotal (float)
    float m_fUpdatePotal;

    // offset 428: m_bFinishMode (bool)
    bool m_bFinishMode;

    // offset 432: m_mapProcessSpawnBox (std::map<int, STMageProcessSpawnBox*>, 32 bytes)
    std::map<int, void*> m_mapProcessSpawnBox;  // TODO: 需人工审查 - 类型待确认

    // offset 464: m_listMonsterSpawnInfo (std::list<VMonsterSpawnInfo*>, 24 bytes)
    std::list<void*> m_listMonsterSpawnInfo;  // TODO: 需人工审查 - 类型待确认

    // offset 488: m_mapEventSpawnBox (std::map<int, STMageEventSpawnBox*>, 32 bytes)
    std::map<int, STMageEventSpawnBox*> m_mapEventSpawnBox;

    // offset 520: m_lstDestoryObject (std::list<XActor*>, 24 bytes)
    std::list<XActor*> m_lstDestoryObject;

    // offset 544: m_mapInteractionBox (std::map<int, STInteractionBox*>, 32 bytes)
    std::map<int, void*> m_mapInteractionBox;  // TODO: 需人工审查

    // offset 576: m_mapSafetyZone (std::map<int, VSafeAreaBoxInfo*>, 32 bytes)
    std::map<int, void*> m_mapSafetyZone;  // TODO: 需人工审查

    // offset 608: m_mapPotalBox (std::map<int, STMagePotalBox*>, 32 bytes)
    std::map<int, void*> m_mapPotalBox;  // TODO: 需人工审查

    // offset 640: m_mapCommonPostionBox (std::map<int, VCommonPositionBoxInfo*>, 32 bytes)
    std::map<int, void*> m_mapCommonPostionBox;  // TODO: 需人工审查

    // offset 672: m_respawnManager (CRespawnManager, 72 bytes)
    CRespawnManager m_respawnManager;

    // offset 744: m_mapQuestMoveBox (std::map<int, STQuestMoveBox*>, 32 bytes)
    std::map<int, void*> m_mapQuestMoveBox;  // TODO: 需人工审查

    // offset 776: m_nDestroySilhouetes (int)
    int m_nDestroySilhouetes;

    // offset 780: m_nEtcObjectID (int)
    int m_nEtcObjectID;

    // offset 784: m_vaccumManager (CVaccumManager, 176 bytes)
    CVaccumManager m_vaccumManager;

    // offset 960: m_mapGameWorldMode (std::map<int, shared_ptr<CGameWorldMode>>, 32 bytes)
    std::map<int, std::tr1::shared_ptr<CGameWorldMode>> m_mapGameWorldMode;

    // offset 992: m_mapMonsterSpawnBoxInfo (std::map<int, std::list<int>>, 32 bytes)
    std::map<int, std::list<int>> m_mapMonsterSpawnBoxInfo;

    // offset 1024: m_setReviveMonster (std::set<unsigned long>, 32 bytes)
    std::set<std::uint32_t> m_setReviveMonster;

    // offset 1056: m_nWorldModeState (int)
    int m_nWorldModeState;

    // offset 1064: m_setWorldModeHitUser (std::set<unsigned long>, 32 bytes)
    std::set<std::uint32_t> m_setWorldModeHitUser;

    // offset 1096: m_bInitKRRData (bool)
    bool m_bInitKRRData;

    // Total size: 1104 bytes (verified from IDA)
};
