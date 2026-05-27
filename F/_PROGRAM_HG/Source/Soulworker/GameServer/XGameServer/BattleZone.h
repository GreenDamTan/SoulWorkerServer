#pragma once

#include "Soulworker/GameServer/XCore/XArea/XDistrict.h"
#include "Soulworker/GameServer/XGameServer/RespawnManager.h"
#include "Soulworker/GameServer/XGameServer/VaccumManager.h"
#include "Soulworker/GameServer/XGameServer/GameWorldMode.h"
#include <cstdint>
#include <map>
#include <list>
#include <set>
#include <memory>

// 使用 std::tr1 命名空间 (VS2010 兼容)
namespace std { namespace tr1 = std; }

// Per IDA: E_ACTOR_TYPE - Actor 类型枚举
enum E_ACTOR_TYPE {
    eActorUser = 0,
    eActorNPC = 1,
    eActorMonster = 2,
    eActorAkashic = 3,
    eActorInteraction = 4,
    eActorVaccum = 5,
};

// TODO: 推测结果 - 来自 Vision Engine / IDA
// E_SEND_INFO_TYPE - 发送信息类型枚举
enum E_SEND_INFO_TYPE {
    E_SEND_INFO_TYPE_NONE = 0,
    E_SEND_INFO_TYPE_ALL = 1,
    E_SEND_INFO_TYPE_NEARBY = 2,
    E_SEND_INFO_TYPE_SELF = 3,
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
struct STMagePotalBox;
struct VCommonPositionBoxInfo;
struct STQuestMoveBox;
struct ST_WORLD_MODE_INFO;
struct ST_WORLD_MODE_INFO_VEC;
struct PS_WORLD_MODE_FINISH;
struct PS_WORLD_MODE_COMPLETE;
struct PS_WORLD_MODE_UPDATE;
struct ST_MONSTER_DAMAGE_INFO;  // Per IDA 0x1401A7BC0
struct ST_KRR_MONSTER_INFO;     // Per IDA 0x1401A7FF0
class hkaiPointCloudSilhouetteGenerator;
class DohHavokNavMeshInstance;

// TODO: 需人工审查 - TB_MONSTER 表结构定义（来自资源表）
struct TB_MONSTER {
    int Monster_Type;                    // 怪物类型 (17=KRR, 18=...)
    int Monster_NormalStand_Type;        // 正常站立类型 (2,3需要轮廓)
    int Monster_CollisionRadius;         // 碰撞半径
    int Monster_CollisionHeight;         // 碰撞高度
    int Monster_Sight;                   // 视野距离
    char* Monster_Code_Name;             // 代码名称
    // ... 其他字段待补充
};

// Per IDA - VMonsterSpawnInfo 结构中的怪物信息
struct VMonsterSpawnInfo_MonsterInfo {
    int m_iID;                           // 怪物/NPC ID
    int m_iType;                         // 类型 (0=Monster, 1=NPC, 2/4=特殊怪物)
    int m_iChance;                       // 生成概率
    // ... 其他字段
};

// Per IDA - VMonsterSpawnInfo 生成箱信息
struct VMonsterSpawnInfo {
    int iID;                             // 生成箱ID
    int m_iSectorID;                     // 区域ID
    int m_iMaxEntityCount;               // 最大实体数量
    float fRotate;                       // 旋转角度
    int m_iGroupID;                      // 组ID
    int m_iMoveType;                     // 移动类型
    int m_iWaypoint;                     // 路点ID
    int m_iAggroGroupID;                 // 仇恨组ID
    int m_iAggroDistance;                // 仇恨距离
    int m_iAggroMaxCount;                // 最大仇恨数量
    float m_fTakeTargetRatio;            // 目标比率
    float m_RespawnTime;                 // 重生时间
    int m_iCreationCondition;            // 创建条件 (1=立即生成, 2=事件触发)
    char m_ChangeSpawnAction[256];       // 生成动作名称
    VMonsterSpawnInfo_MonsterInfo m_stMonsterInfo[10]; // 怪物信息数组
    int m_iWaitCreationSequenceType;     // 等待创建序列类型
    int m_iWaitCreationMaxWave;          // 等待创建最大波数
    float m_fWaitCreationDelayTime;      // 等待创建延迟时间
    float m_fWaitCreationSequenceTime;   // 等待创建序列时间
    // ... 其他字段
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
    
    // Per IDA 0x1401A71D0 - 杀死所有怪物
    void DieMonsterAll(bool bForce);
    
    // Per IDA 0x1401A7BC0 - 保存伤害信息
    void SaveDamageInfo(std::list<struct ST_MONSTER_DAMAGE_INFO>& listHitID);
    
    // Per IDA 0x1401A7FF0 - 初始化KRR怪物
    void InitKRRMonster();
    
    // Per IDA 0x1401A8410 - 发送WorldMode信息
    void SendWorldModeInfo(XActor* pActor);
    
    // Per IDA 0x1401A4410 - 处理怪物任务
    void ProcessMonsterQuest(XActor* pAttacker, std::uint32_t nMonsterID);
    
    // Per IDA 0x1401A6910 - WorldMode掉落物品
    void DropItemForWorldMode(std::uint32_t dwMonsterID, int nModeDateID, bool bComplete);
    
    void MonsterDieForEvent(CMonster* pMonster, std::uint32_t dwKillerID);

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
    void DeleteAkashicObject(CAkashicObject* pObject);

    // 交互对象
    CInteractionObject* CreateInteractionObject(STInteractionBox* pBox, void* pTBInteraction, XVec3& vPos, float fYaw);
    void DeleteInteractionObject(CInteractionObject* pObject);
    void ClickInteractionBox(int nBoxID, CUser* pUser);

    // 区域相关
    void ExitArea(XActor* pActor);
    bool IsInSafetyZone(XActor* pActor);

    // 生成
    void Generate();
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
    void SyncWorldMode(ST_WORLD_MODE_INFO_VEC& stInfoVec);
    void CompleteWorldMode(PS_WORLD_MODE_COMPLETE& stComplete, std::uint32_t dwKillerID);
    void UpdateWorldMode(PS_WORLD_MODE_UPDATE& stUpdate);
    bool IsWorldModeBoss();
    bool AlreadyInWorldMode();

    // Drop
    bool ProcessDrop(XActor* pActor, int nType, XVec3& vPos);
    bool ProcessDrop(XActor* pActor, CMonster* pMonster, XVec3& vPos);
    void ProcessDropByHit(std::uint32_t dwKillerID, int nTableID, int nLevel, XVec3& vPos, int nDropType);

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
    void AppearEventMonster(int nType1, bool bFlag1, int nType2, bool bFlag2, int nType3, bool bFlag3);
    void SetWorldModeBoostAll(int nIndex, bool bFlag);
    void ChangePacketOptimization_GM(float fOpt);
    void ResetPacketOptimization_GM();
    void AddDestoryObject(XActor* pActor);

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
    std::map<int, void*> m_mapEventSpawnBox;  // TODO: 需人工审查

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
