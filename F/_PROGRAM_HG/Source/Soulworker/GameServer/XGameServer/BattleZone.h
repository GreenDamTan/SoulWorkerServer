#pragma once

#include "Soulworker/GameServer/XCore/XArea/XDistrict.h"
#include "Soulworker/GameServer/XGameServer/RespawnManager.h"
#include "Soulworker/GameServer/XGameServer/VaccumManager.h"
#include <cstdint>
#include <map>
#include <list>
#include <set>
#include <memory>

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

    // 怪物管理
    CMonster* CreateMonster(TUXMapID uxMapID, int nTableID, int nLevel, XVec3 vPos, float fYaw,
                            E_SEND_INFO_TYPE eSendType, int nGroupID, int nSpawnType, TUXActorID uxActorID);
    void DeleteMonster(CMonster* pMonster);
    void DieMonster(std::list<std::uint32_t>& listMonsterID, bool bForce);
    void DieMonsterAll(bool bForce);
    void MonsterDieForEvent(CMonster* pMonster, std::uint32_t dwKillerID);

    // NPC管理
    CNpc* CreateNpc(TUXMapID uxMapID, int nTableID, int nLevel, XVec3 vPos, float fYaw, E_SEND_INFO_TYPE eSendType);
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

    // Quest
    void ProcessMonsterQuest(XActor* pActor, int nQuestID);
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
    std::map<int, std::tr1::shared_ptr<void>> m_mapGameWorldMode;  // TODO: 需人工审查

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
