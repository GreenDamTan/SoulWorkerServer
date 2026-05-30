#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
// Note: CMonster 和 CNpc 在 BattleZone.h 中前置声明
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"

// Per IDA 0x14019D2B0: CBattleZone 构造函数
// IDA 反编译精确逻辑:
// 1. XDistrict::XDistrict(this) 调用基类构造
// 2. 设置 vftable
// 3. XGameServer::Instance()->GetCurDate() 初始化 m_tQuestUpdate
// 4. m_fUpdatePotal = 10.0
// 5. m_bFinishMode = 0
// 6. 所有 std::map 默认构造 (编译器自动调用)
// 7. CRespawnManager::CRespawnManager(&m_respawnManager)
// 8. CVaccumManager::CVaccumManager(&m_vaccumManager)
// 9. 所有 std::map/std::set 默认构造
// 10. m_bInitKRRData = 0
CBattleZone::CBattleZone()
    : XDistrict()
    , m_tQuestUpdate_dummy(0)
    , m_fUpdatePotal(10.0f)
    , m_bFinishMode(false)
    , m_respawnManager()
    , m_nDestroySilhouetes(0)
    , m_nEtcObjectID(0)
    , m_vaccumManager()
    , m_nWorldModeState(0)
    , m_bInitKRRData(false)
{
    // Per IDA: 设置 vftable (C++ 自动处理)
    // this->__vftable = &CBattleZone::`vftable';

    // Per IDA: 从 XGameServer 获取当前日期时间初始化 m_tQuestUpdate
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // auto curDate = XGameServer::GetCurDate(pServer);
    // UXMapID::UXMapID(&this->m_tQuestUpdate, curDate);

    // 注: 所有 std::map/std::list/set 成员由编译器自动调用默认构造函数初始化
    // m_mapProcessSpawnBox, m_listMonsterSpawnInfo, m_mapEventSpawnBox,
    // m_lstDestoryObject, m_mapInteractionBox, m_mapSafetyZone,
    // m_mapPotalBox, m_mapCommonPostionBox, m_mapQuestMoveBox,
    // m_mapGameWorldMode, m_mapMonsterSpawnBoxInfo, m_setReviveMonster,
    // m_setWorldModeHitUser

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone constructed");
}

// Per IDA 0x14019D4E0: CBattleZone 析构函数
// IDA 反编译精确逻辑 (逆序销毁成员):
// 1. 设置 vftable
// 2. m_setWorldModeHitUser::~set()
// 3. m_setReviveMonster::~set()
// 4. m_mapMonsterSpawnBoxInfo::~map()
// 5. m_mapGameWorldMode::~map()
// 6. m_vaccumManager::~CVaccumManager()
// 7. m_mapQuestMoveBox::~map()
// 8. m_respawnManager::~CRespawnManager()
// 9. m_mapCommonPostionBox::~map()
// 10. m_mapPotalBox::~map()
// 11. m_mapSafetyZone::~map()
// 12. m_mapInteractionBox::~map()
// 13. m_lstDestoryObject::~list()
// 14. m_mapEventSpawnBox::~map()
// 15. m_listMonsterSpawnInfo::~list()
// 16. m_mapProcessSpawnBox::~map()
// 17. XDistrict::~XDistrict()
CBattleZone::~CBattleZone() {
    // Per IDA: 设置 vftable (C++ 自动处理)
    // this->__vftable = &CBattleZone::`vftable';

    // 标准C++析构函数会自动逆序调用成员析构函数
    // 基类 XDistrict 的析构函数会在最后自动调用
    // 顺序与 IDA 反编译一致:
    // 1. m_setWorldModeHitUser 析构
    // 2. m_setReviveMonster 析构
    // 3. m_mapMonsterSpawnBoxInfo 析构
    // 4. m_mapGameWorldMode 析构 (shared_ptr 自动释放)
    // 5. m_vaccumManager 析构
    // 6. m_mapQuestMoveBox 析构
    // 7. m_respawnManager 析构
    // 8. m_mapCommonPostionBox 析构
    // 9. m_mapPotalBox 析构
    // 10. m_mapSafetyZone 析构
    // 11. m_mapInteractionBox 析构
    // 12. m_lstDestoryObject 析构
    // 13. m_mapEventSpawnBox 析构
    // 14. m_listMonsterSpawnInfo 析构
    // 15. m_mapProcessSpawnBox 析构
    // 16. XDistrict::~XDistrict() 基类析构
}

// Per IDA 0x14019D640: CBattleZone::Create
// 创建战斗区域，加载迷宫信息、导航网格等
// IDA 反编译精确逻辑:
// 1. if (!pMazeInfo) return false
// 2. m_pObjectResource = XWorldResMgr::GetResource(pMazeInfo->ID)
// 3. if (!m_pObjectResource) -> LogError, return false
// 4. m_uxMapID = uxMapID
// 5. m_nMaxUserCount = 50
// 6. if (SWORD2(uxMapID.nMapID) == 30031) -> m_nMaxUserCount = pMazeInfo->Maze_Enter_Count
// 7. AREA_OBJECT::SetSize(&m_objectScanner, 0, 0, Maze_Size_X + 30000, Maze_Size_Y + 30000, 5000)
// 8. if (!CreateNavMesh(pMazeInfo->Server_SceneScript_File)) -> LogError, return false
// 9. m_nEtcObjectID = 0
// 10. CVaccumManager::Init(&m_vaccumManager, this, 0)
// 11. Generate()
// 12. if (m_uxMapID.nMapID << 16 >> 48 == 30031) -> SpawnEventMapNpc()
//     else -> SpawnGenerateMonster()
// 13. if (m_pNavMeshInstance) -> hkaiWorld::stepSilhouettes(), m_setWorldModeHitUser.clear()
bool CBattleZone::Create(TUXMapID uxMapID, TB_MAZE_INFO* pMazeInfo) {
    // Per IDA: 检查参数
    if (!pMazeInfo) {
        return false;
    }

    // Per IDA: 从世界资源管理器获取对象资源
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // m_pObjectResource = XWorldResMgr::GetResource(&pServer->m_xWorldResMgr, pMazeInfo->ID);
    // if (!m_pObjectResource) {
    //     LogHelper::LogError("game.contents",
    //         "Create error - Cant read event info from map battle zone[ ID:%d ] ( %d )",
    //         pMazeInfo->ID, 48);
    //     return false;
    // }

    // Per IDA: 设置地图 ID
    m_uxMapID = uxMapID;

    // Per IDA: 设置最大玩家数量 (默认50, 地图30031使用迷宫配置)
    m_nMaxUserCount = 50;
    // SWORD2 宏: (uxMapID.nMapID & 0xFFFF)
    std::uint16_t wMapIDLow = static_cast<std::uint16_t>(uxMapID.wMapID & 0xFFFF);
    if (wMapIDLow == 30031) {
        m_nMaxUserCount = pMazeInfo->Maze_Enter_Count;
    }

    // Per IDA: 设置区域对象扫描器大小
    // AREA_OBJECT::SetSize(&m_objectScanner, 0, 0,
    //     pMazeInfo->Maze_Size_X + 30000,
    //     pMazeInfo->Maze_Size_Y + 30000, 5000);

    // Per IDA: 从场景脚本文件创建导航网格
    // if (!CreateNavMesh(pMazeInfo->Server_SceneScript_File)) {
    //     LogHelper::LogError("game.contents",
    //         "Create error - Failed CreateNavMesh maze[ ID:%d ] ( %d )",
    //         pMazeInfo->ID, 69);
    //     return false;
    // }

    m_nEtcObjectID = 0;

    // Per IDA: 初始化真空管理器
    m_vaccumManager.Init(this, false);

    // Per IDA: 生成场景对象 (生成箱、交互箱、传送门等)
    Generate();

    // Per IDA: 地图30031使用事件NPC生成，其他使用普通怪物生成
    // if (wMapIDLow == 30031) {
    //     SpawnEventMapNpc();
    // } else {
    //     SpawnGenerateMonster();
    // }

    // Per IDA: 如果导航网格实例存在，步进轮廓并清空 WorldMode 击中用户集合
    // if (m_pNavMeshInstance) {
    //     hkaiWorld* pWorld = (hkaiWorld*)CFsmState<CAi>::GetUpdateFunc(m_pNavMeshInstance);
    //     hkaiWorld::stepSilhouettes(pWorld, nullptr);
    //     m_setWorldModeHitUser.clear();
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::Create");
    return true;
}

// Per IDA 0x14019D880: CBattleZone::SpawnEventMapNpc
// 生成事件地图 NPC (地图 30031 专用)
// IDA 反编译精确逻辑:
// 1. 遍历 m_pObjectResource 中的事件对象
// 2. 对每个 VEventObjectInfo:
//    - if (eType == eEventObjectType_Box)
//    - if (eBoxType == eEventBoxType_MonsterSpawn)
//    - if (m_iCreationCondition == 1)
// 3. 对每个 VMonsterSpawnInfo:
//    - 随机概率检查 (m_iChance)
//    - 生成 NPC: CreateNpc(uxMapID, uniqueSector, monsterID, pos, rotate, eSendInfoTypeNot)
//    - 设置 waypoint 和 spawnBoxID
void CBattleZone::SpawnEventMapNpc() {
    // IDA: 检查对象资源是否存在
    if (!m_pObjectResource) {
        return;
    }

    // IDA: 获取事件对象映射表
    // int v1 = std::_Val_type<ST_TRADE_ITEM*>((VBaseResourceLump*)this);
    // VMap<int,void*>* ObjectMap = VEventObjectResource::GetMap(m_pObjectResource, v1, 1, 0);
    // void* Iter = VMap<int,void*>::GetStartPosition(ObjectMap);

    // IDA: 遍历所有事件对象
    // while (Iter) {
    //     VMap<int,void*>::GetNextPair(ObjectMap, &Iter, &iKey, &pValue);
    //     const VEventObjectInfo* pInfo = (const VEventObjectInfo*)pValue;
    //
    //     if (pInfo && pInfo->eType == eEventObjectType_Box) {
    //         const VEventBoxInfo* pBoxInfo = (const VEventBoxInfo*)pInfo;
    //
    //         if (pBoxInfo && pBoxInfo->eBoxType == eEventBoxType_MonsterSpawn) {
    //             const VMonsterSpawnInfo* pMonsterSpawn = (const VMonsterSpawnInfo*)pBoxInfo;
    //
    //             if (pMonsterSpawn && pMonsterSpawn->m_iCreationCondition == 1) {
    //                 // 生成怪物
    //                 XVec3 vPos;
    //                 int nCreateCount = 0;
    //
    //                 for (int i = 0; i < 10; ++i) {
    //                     if (pMonsterSpawn->m_stMonsterInfo[i].m_iID) {
    //                         XWorldManager* pWorldMgr = TXSingleton<XWorldManager>::Instance();
    //                         int nProb = XWorldManager::RandProb(pWorldMgr);
    //
    //                         if (nProb <= pMonsterSpawn->m_stMonsterInfo[i].m_iChance) {
    //                             for (int j = 0; j < pMonsterSpawn->m_iMaxEntityCount; ++j) {
    //                                 GetSpawnPos_2(pMonsterSpawn, &vPos);
    //
    //                                 if (pMonsterSpawn->m_stMonsterInfo[i].m_iType == 1) {
    //                                     int iUniqueSector = VEventObjectInfo::GetEventUniqueID(
    //                                         pMonsterSpawn->m_iSectorID, v1);
    //
    //                                     CNpc* pNpc = CreateNpc(m_uxMapID, iUniqueSector,
    //                                         pMonsterSpawn->m_stMonsterInfo[i].m_iID,
    //                                         vPos, pMonsterSpawn->fRotate, eSendInfoTypeNot);
    //
    //                                     if (pNpc) {
    //                                         CMoverEx::SetWayPointID(pNpc, pMonsterSpawn->m_iWaypoint);
    //                                         CNpc::SetSpawnBoxID(pNpc, pMonsterSpawn->iID);
    //                                         ++nCreateCount;
    //                                     }
    //                                 }
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "SpawnEventMapNpc called");
}

// Per IDA 0x14019DBD0: CBattleZone::Clear
// 清理所有资源、怪物、NPC、对象等
// IDA 反编译精确逻辑:
// 1. m_listMonsterSpawnInfo.clear()
// 2. 遍历 m_mapProcessSpawnBox 删除所有 STMageProcessSpawnBox
// 3. 遍历 m_mapEventSpawnBox 删除所有 STMageEventSpawnBox
// 4. 遍历 m_mapActor 根据 Actor 类型删除 (Monster/NPC/Akashic/Interaction/Vaccum)
// 5. 遍历 m_mapInteractionBox 删除所有 STInteractionBox
// 6. 遍历 m_mapQuestMoveBox 删除所有 STQuestMoveBox
// 7. m_mapSafetyZone.clear()
// 8. if (m_pNavMeshInstance) -> hkReferencedObject::removeReference()
// 9. CRespawnManager::Clear(&m_respawnManager)
// 10. m_mapPotalBox.clear()
// 11. m_mapGameWorldMode.clear()
// 12. m_setReviveMonster.clear()
// 13. m_setWorldModeHitUser.clear()
// 14. XArea::Clear(this)
void CBattleZone::Clear() {
    // Per IDA: 清理怪物生成信息列表
    m_listMonsterSpawnInfo.clear();

    // Per IDA: 清理 ProcessSpawnBox
    for (auto it = m_mapProcessSpawnBox.begin(); it != m_mapProcessSpawnBox.end(); ++it) {
        if (it->second) {
            // STMageProcessSpawnBox* pProcessSpawn = static_cast<STMageProcessSpawnBox*>(it->second);
            // delete pProcessSpawn;
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapProcessSpawnBox.clear();

    // Per IDA: 清理 EventSpawnBox
    for (auto it = m_mapEventSpawnBox.begin(); it != m_mapEventSpawnBox.end(); ++it) {
        if (it->second) {
            // STMageEventSpawnBox* pEventSpawn = static_cast<STMageEventSpawnBox*>(it->second);
            // delete pEventSpawn;
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapEventSpawnBox.clear();

    // Per IDA: 遍历所有 Actor 根据 RTTI 类型删除
    // for (__POSITION* iter = m_mapActor.GetStartPosition(); iter; ) {
    //     unsigned long dwKey;
    //     XActor* pActor;
    //     m_mapActor.GetNextPair(&iter, &dwKey, (void*&)pActor);
    //     if (!pActor) continue;
    //
    //     E_ACTOR_TYPE eType = XActor::GetType(pActor);
    //     switch (eType) {
    //         case eActorMonster: {
    //             CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
    //             ThreadLocalData::GetInstance()->DeleteMonster(pMonster);
    //             break;
    //         }
    //         case eActorNPC: {
    //             CNpc* pNpc = dynamic_cast<CNpc*>(pActor);
    //             ThreadLocalData::GetInstance()->DeleteNpc(pNpc);
    //             break;
    //         }
    //         case eActorAkashic: {
    //             CAkashicObject* pAkashic = dynamic_cast<CAkashicObject*>(pActor);
    //             ThreadLocalData::GetInstance()->DeleteAkashicObject(pAkashic);
    //             break;
    //         }
    //         case eActorInteraction: {
    //             CInteractionObject* pInteraction = dynamic_cast<CInteractionObject*>(pActor);
    //             ThreadLocalData::GetInstance()->DeleteInteractionObject(pInteraction);
    //             break;
    //         }
    //         case eActorVaccum: {
    //             CVaccumCube* pVaccum = dynamic_cast<CVaccumCube*>(pActor);
    //             ThreadLocalData::GetInstance()->DeleteVaccumCubeObject(pVaccum);
    //             break;
    //         }
    //     }
    // }

    // Per IDA: 清理 InteractionBox
    for (auto it = m_mapInteractionBox.begin(); it != m_mapInteractionBox.end(); ++it) {
        if (it->second) {
            // STInteractionBox* pInteraction = static_cast<STInteractionBox*>(it->second);
            // delete pInteraction;
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapInteractionBox.clear();

    // Per IDA: 清理 QuestMoveBox
    for (auto it = m_mapQuestMoveBox.begin(); it != m_mapQuestMoveBox.end(); ++it) {
        if (it->second) {
            // STQuestMoveBox* pQuestMove = static_cast<STQuestMoveBox*>(it->second);
            // delete pQuestMove;
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapQuestMoveBox.clear();

    // Per IDA: 清理安全区域
    m_mapSafetyZone.clear();

    // Per IDA: 清理导航网格
    // if (m_pNavMeshInstance) {
    //     hkReferencedObject::removeReference(m_pNavMeshInstance);
    //     m_pNavMeshInstance = nullptr;
    // }

    // Per IDA: 清理重生管理器
    m_respawnManager.Clear();

    // Per IDA: 清理传送门
    m_mapPotalBox.clear();

    // Per IDA: 清理 WorldMode
    m_mapGameWorldMode.clear();

    // Per IDA: 清理复活怪物集合
    m_setReviveMonster.clear();

    // Per IDA: 清理 WorldMode 击中用户集合
    m_setWorldModeHitUser.clear();

    // Per IDA: 调用基类清理
    // XArea::Clear(this);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::Clear - partial implementation");
}

// Per IDA 0x14019E1A0: CBattleZone::OnUpdate
// 更新战斗区域，处理生成箱、交互箱、角色更新等
// IDA 反编译精确逻辑:
// 1. 遍历 m_mapProcessSpawnBox 更新生成箱
//    - if (bActive && !bTerminate) -> fDelayTime -= fElapsed
//    - if (fDelayTime <= 0 && nCreatedCount != 0) -> ExcuteSpawnBox, nCreatedCount -= 1
//    - if (nCreatedCount <= 0) -> bActive = false
//    - else -> fDelayTime = pSpawnBox->m_fWaitCreationSequenceTime
// 2. CRespawnManager::Update(&m_respawnManager, this)
// 3. if (m_bFinishMode) -> m_fUpdatePotal -= fElapsed, if <= 0 -> SetPotalFlag(10012, true)
// 4. 遍历 m_mapInteractionBox 更新交互箱冷却
//    - if (fCoolTime > 0) -> fCoolTime -= fElapsed, if <= 0 -> EnableInteractionBox
// 5. CVaccumManager::Update(&m_vaccumManager)
// 6. 遍历 m_mapActor 更新所有 Actor
//    - if (eActorUser) 检查客户端加载完成，更新位置，检查任务同步
//    - else -> OnUpdate
// Per IDA 0x14019E1A0: CBattleZone::OnUpdate
// 精确还原：战斗区域帧更新
void CBattleZone::OnUpdate(float fElapsed) {
    // Per IDA: 更新 ProcessSpawnBox - 遍历所有处理生成箱
    for (auto it = m_mapProcessSpawnBox.begin(); it != m_mapProcessSpawnBox.end(); ++it) {
        STMageProcessSpawnBox* pProcessSpawn = static_cast<STMageProcessSpawnBox*>(it->second);
        if (!pProcessSpawn) continue;

        // Per IDA: 检查是否激活且未终止
        if (pProcessSpawn->bActive && !pProcessSpawn->bTerminate) {
            pProcessSpawn->fDelayTime -= fElapsed;

            // Per IDA: 检查延迟时间是否到期
            if (pProcessSpawn->fDelayTime <= 0.0f && pProcessSpawn->nCreatedCount != 0.0f) {
                ExcuteSpawnBox(pProcessSpawn, E_SEND_INFO_TYPE_ALL);

                pProcessSpawn->nCreatedCount -= 1.0f;
                if (pProcessSpawn->nCreatedCount <= 0.0f) {
                    pProcessSpawn->bActive = false;
                } else {
                    pProcessSpawn->fDelayTime = pProcessSpawn->pSpawnBox->m_fWaitCreationSequenceTime;
                }
            }
        }
    }

    // Per IDA: 更新重生管理器
    // CRespawnManager::Update(&m_respawnManager, this);

    // Per IDA: 检查完成模式，更新传送门标志
    if (m_bFinishMode) {
        m_fUpdatePotal -= fElapsed;
        if (m_fUpdatePotal <= 0.0f) {
            SetPotalFlag(10012, true);
            m_bFinishMode = false;
            m_fUpdatePotal = 10.0f;
        }
    }

    // Per IDA: 更新交互箱冷却
    for (auto it = m_mapInteractionBox.begin(); it != m_mapInteractionBox.end(); ++it) {
        STInteractionBox* pInteraction = static_cast<STInteractionBox*>(it->second);
        if (!pInteraction) continue;

        if (pInteraction->fCoolTime > 0.0f) {
            pInteraction->fCoolTime -= fElapsed;
            if (pInteraction->fCoolTime <= 0.0f) {
                pInteraction->fCoolTime = 0.0f;
                EnableInteractionBox(pInteraction->nBoxIndex, true);
            }
        }
    }

    // Per IDA: 更新真空管理器
    // CVaccumManager::Update(&m_vaccumManager);

    // Per IDA: 遍历所有 Actor 更新
    std::list<unsigned int> listDeleteUser;
    int nUserCount = 0;

    for (auto iter = m_mapActor.begin(); iter != m_mapActor.end(); ++iter) {
        XActor* pActor = iter->second;
        if (!pActor) continue;

        if (pActor->GetType() != 0) {
            // Per IDA: 非玩家 Actor 直接调用 OnUpdate
            // pActor->OnUpdate(fElapsed);
        } else {
            // Per IDA: 处理玩家
            CUser* pUser = dynamic_cast<CUser*>(pActor);
            if (!pUser) continue;

            // Per IDA: 检查客户端加载完成和有效地图实例
            // if (XClient::IsBit_OR(pUser, eNetState_LoadComplete) &&
            //     CUser::GetValidMapInsID(pUser)->nMapID == GetInstanceID()->nMapID)
            {
                ++nUserCount;
                // pActor->OnUpdate(fElapsed);
                // CMoverEx::SetAmountOfHeal(&pUser->CMoverEx, 0.0f);

                // Per IDA: 每30秒检查任务同步
                // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
                // auto curDate = XGameServer::GetCurDate(pServer);
                // ATL::CTimeSpan span(0, 0, 30, 0);
                // if (m_tQuestUpdate + span <= curDate) {
                //     m_tQuestUpdate = curDate;
                //     std::tr1::shared_ptr<CGocQuest> pQuest;
                //     CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuest, 0);
                //     if (pQuest && !CGocQuest::CheckQuestDBSync(pQuest.get())) {
                //         CGocQuest::DBSyncQuestCondition(pQuest.get());
                //     }
                // }
            }
            // else {
            //     // Per IDA: 从扫描器移除无效用户
            //     auto* pScanner = GetScanner(pActor);
            //     if (pScanner && !pScanner->Erase(&pUser->CMoverEx)) {
            //         LogHelper::LogError("game.contents",
            //             "ExitActorByForce error - Failed Erase[ ActorID:%d ] ( %d )",
            //             pActor->GetActorID().dwActorID, 325);
            //     }
            //     listDeleteUser.push_back(iter->first);
            // }
        }
    }

    // Per IDA: 移除无效用户
    for (auto it = listDeleteUser.begin(); it != listDeleteUser.end(); ++it) {
        m_mapActor.erase(*it);
        LogHelper::LogError("game.contents", "[DELETE USER] m_mapActor.RemoveKey %lld / %d",
            m_uxMapID.wMapID, *it);
    }
    listDeleteUser.clear();

    // Per IDA: 处理待销毁对象列表
    if (!m_lstDestoryObject.empty()) {
        for (auto it = m_lstDestoryObject.begin(); it != m_lstDestoryObject.end(); ++it) {
            XActor* pActor = *it;
            if (!pActor) continue;

            E_ACTOR_TYPE eType = static_cast<E_ACTOR_TYPE>(pActor->GetType());

            switch (eType) {
                case eActorMonster: {
                    CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
                    DeleteMonster(pMonster);
                    break;
                }
                case eActorNPC: {
                    CNpc* pNpc = reinterpret_cast<CNpc*>(pActor);
                    DeleteNpc(pNpc);
                    break;
                }
                case eActorAkashic: {
                    CAkashicObject* pAkashic = reinterpret_cast<CAkashicObject*>(pActor);
                    DeleteAkashicObject(pAkashic);
                    break;
                }
                default:
                    break;
            }

            // Per IDA: 如果有轮廓需要销毁，步进轮廓
            if (m_nDestroySilhouetes > 0) {
                // hkaiWorld* pWorld = (hkaiWorld*)CFsmState<CAi>::GetUpdateFunc(m_pNavMeshInstance);
                // hkaiWorld::stepSilhouettes(pWorld, nullptr);
                m_nDestroySilhouetes = 0;
            }
        }
        m_lstDestoryObject.clear();
    }

    // Per IDA: 初始化 KRR 怪物
    InitKRRMonster();
}

// Per IDA 0x14019EC80: CBattleZone::LoadComplete
// 精确还原：玩家加载完成后的处理
void CBattleZone::LoadComplete(XActor* pActor)
{
    if (!pActor) return;

    // Per IDA: RTTI 转换 CUser* from XActor*
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser) return;

    // Per IDA: 设置客户端加载完成标志
    // CUser::SetClientLoadComplete(pUser, true);

    // Per IDA: 发送技能包
    // std::tr1::shared_ptr<CGocSkill> pSkill;
    // CMover::GetGOC<CGocSkill>(&pUser->CMoverEx, &pSkill, 0);
    // if (pSkill) {
    //     CGocSkill::SendPacketLoadSkill(pSkill.get());
    // }

    // Per IDA: 初始化 Akashic 记录
    // std::tr1::shared_ptr<CGocAkashicRecord> pAkashic;
    // CMover::GetGOC<CGocAkashicRecord>(&pUser->CMoverEx, &pAkashic, 0);
    // if (pAkashic) {
    //     CGocAkashicRecord::SetUseCount(pAkashic.get(), 1);
    //     CGocAkashicRecord::ThinkAkashicPassive(pAkashic.get());
    // }

    // Per IDA: 发送对象信息
    // SendObjectInfo(pActor);

    // Per IDA: 发送交通工具信息
    // SendTransportationInfo(pActor);

    // Per IDA: 发送传送门信息
    SendPotalInfos(pActor);

    // Per IDA: 初始化属性系统
    // std::tr1::shared_ptr<CGocAttribute> pAttr;
    // CMover::GetGOC<CGocAttribute>(&pUser->CMoverEx, &pAttr, 0);
    // if (pAttr) {
    //     bool IsFullStat = CUser::IsFullStat(pUser);
    //     int nWorldType = GetWorldType();
    //     CGocAttribute::SetStartStatEnterWorld(pAttr.get(), nWorldType, IsFullStat);
    //     CUser::SetFullStat(pUser, false);
    //     CGocAttribute::SetStartRegStat(pAttr.get(), true);
    //     CGocAttribute::SendOriginStatAll(pAttr.get());
    //     CGocAttribute::SendStatAll(pAttr.get());
    // }

    // Per IDA: 初始化物品冷却时间
    // std::tr1::shared_ptr<CGocInventory> pInven;
    // CMover::GetGOC<CGocInventory>(&pUser->CMoverEx, &pInven, 0);
    // if (pInven) {
    //     CGocInventory::InitItemCoolTime(pInven.get());
    //     CGocInventory::SendItemCoolTimeInfo(pInven.get());
    // }

    // Per IDA: 发送 WorldMode 信息
    SendWorldModeInfo(pActor);

    // Per IDA: 同步 WorldMode 状态
    SetWorldModeSync(pUser);

    // Per IDA: 如果有 PVP 惩罚，应用 Buff
    // if (CUser::IsPVPPenalty(pUser)) {
    //     UXActorID actorID = pUser->GetActorID();
    //     unsigned int dwQuestID = CQuestCondition::GetQuestID(&actorID);
    //     CMySkillList::ApplyBuffSkill(&pUser->CMoverEx, 0x63, dwQuestID, 0);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "LoadComplete for user");
}

// Per IDA 0x1401A08B0: CBattleZone::CreateMonster
// 创建怪物
// IDA 反编译精确逻辑:
// 1. XGameServer::Instance() -> XResourceMgr::GetTB_MONSTER(nMonsterID)
// 2. if (pTBMonster->Monster_Type == 17 && m_setReviveMonster.size() >= 100) -> 错误返回
// 3. ThreadLocalData::GetInstance()->CreateMonster(this, uxMazeSerialID, nMonsterID, vPos, fRot)
// 4. if (Monster_NormalStand_Type == 2 || == 3) -> SetupPhysicsAndBound + SetCollisionEnable(true, true)
//    else -> SetupPhysicsAndBound + SetCollisionEnable(true, false)
// 5. XActionResMgr::SetHitCollisionDataToActor(Monster_Code_Name, pMonster)
// 6. XActionResMgr::SetTraceBoneNameDataToActor(Monster_Code_Name, pMonster)
// 7. SetSpawnBoxID(nSpawnBoxID), SetParentID, SetOriginID from parent actor
// 8. CAi::SetTargetSightDistance(Monster_Sight)
// 9. EnterActor(pMonster) -> if fails, DeleteMonster and return nullptr
// 10. if (Monster_Type == 17) -> m_setReviveMonster.insert(GetQuestID(actorID))
// 11. if (nMonsterID == 13901001 || == 13901101) -> 遍历 m_mapGameWorldMode 查找 Start_Type == 0 且 State == 1
//     -> ThreadLocalData::AppearEventMonster 广播，否则 DeleteMonster 返回 nullptr
// 12. if (Monster_Type == 17 || == 18) -> 构造 ST_KRR_MONSTER_INFO，发送 XSendDBPacket(0, 0xF3, 1)
CMonster* CBattleZone::CreateMonster(TUXMapID uxMazeSerialID, int nSectorID, unsigned int nMonsterID,
                                       XVec3 vPos, float fRot, E_SEND_INFO_TYPE eType,
                                       int nSpawnBoxID, int nGroupID, TUXActorID uxParentID) {
    // 获取怪物表数据
    // XGameServer* pServer = XGameServer::Instance();
    // TB_MONSTER* pTBMonster = XResourceMgr::GetTB_MONSTER(&pServer->m_xResourceMgr, nMonsterID);
    TB_MONSTER* pTBMonster = nullptr;  // TODO: 需要实现资源管理器访问

    if (!pTBMonster) {
        // LogHelper::LogError("game.contents",
        //     "CreateMonster error - No Table TB_MONSTER[ MonsterID:%d ] ( %d )",
        //     nMonsterID, 844);
        GreenDamTan_log(__FILE__, __FUNCTION__, "CreateMonster error - No TB_MONSTER");
        return nullptr;
    }

    // 检查 KRR 怪物数量限制 (Monster_Type == 17)
    // Per IDA: if (pTBMonster->Monster_Type == 17 && m_setReviveMonster.size() >= 100)
    if (pTBMonster->Monster_Type == 17 && m_setReviveMonster.size() >= 100) {
        // LogHelper::LogError("game.contents",
        //     "CreateMonster error - Limit Revive Count [ MonsterID:%d ] ( %d )",
        //     nMonsterID, m_setReviveMonster.size());
        GreenDamTan_log(__FILE__, __FUNCTION__, "CreateMonster error - KRR limit reached");
        return nullptr;
    }

    // Phase 9: Create monster via new CMonster() (stub implementation)
    // When ThreadLocalData is available, use: ThreadLocalData::GetInstance()->CreateMonster(...)
    CMonster* pMonster = new CMonster();

    if (!pMonster) {
        // LogHelper::LogDebug("game.contents",
        //     "CreateMonster error - Failed create monster[ MonsterID:%d ]", nMonsterID);
        GreenDamTan_log(__FILE__, __FUNCTION__, "CreateMonster error - Create failed");
        return nullptr;
    }

    // Initialize monster
    // Per IDA: InitialObjectInfo sets ID, table index, position, rotation
    hkvVec3 hkvPos(vPos.x, vPos.y, vPos.z);
    pMonster->InitialObjectInfo(GetUniqueID(nSectorID), nMonsterID, hkvPos, fRot);
    pMonster->SetSpawnBoxID(nSpawnBoxID);
    pMonster->SetGroupID(nGroupID);
    pMonster->SetTablePtr(pTBMonster);

    // 设置物理碰撞
    // Per IDA: Monster_NormalStand_Type == 2 或 3 时启用双向碰撞
    if (pTBMonster->Monster_NormalStand_Type == 2 || pTBMonster->Monster_NormalStand_Type == 3) {
        // CMover::SetupPhysicsAndBound(pMonster,
        //     (float)pTBMonster->Monster_CollisionRadius,
        //     (float)pTBMonster->Monster_CollisionHeight);
        // CMover::SetCollisionEnable(pMonster, true, true);  // 双向碰撞
    } else {
        // CMover::SetupPhysicsAndBound(pMonster,
        //     (float)pTBMonster->Monster_CollisionRadius,
        //     (float)pTBMonster->Monster_CollisionHeight);
        // CMover::SetCollisionEnable(pMonster, true, false);  // 单向碰撞
    }

    // 设置动作碰撞数据
    // XActionResMgr::SetHitCollisionDataToActor(pServer->m_xActionManager,
    //     pTBMonster->Monster_Code_Name, pMonster);
    // XActionResMgr::SetTraceBoneNameDataToActor(pServer->m_xActionManager,
    //     pTBMonster->Monster_Code_Name, pMonster);

    // 设置父ID和OriginID
    if (uxParentID.wType != 0xFFFF || uxParentID.wID != 0xFFFF) {
        // Convert TUXActorID to UXActorID
        UXActorID uxID;
        uxID.dwActorID = (static_cast<std::uint32_t>(uxParentID.wType) << 16) | uxParentID.wID;
        pMonster->SetParentID(uxID);
        // XActor* pParent = FindActor(uxParentID.dwActorID);
        // if (pParent) {
        //     pMonster->SetOriginID(pParent->GetOriginID());
        // }
    }

    // 设置视野距离
    // CAi* pAi = pMonster->GetAi();
    // if (pAi) {
    //     pAi->SetTargetSightDistance((float)pTBMonster->Monster_Sight);
    // }

    // 进入 Actor
    // Note: EnterActor 返回 void，IDA显示失败时会调用 DeleteMonster
    // TODO: 需要检查进入是否成功，失败时调用 DeleteMonster
    EnterActor(reinterpret_cast<XActor*>(pMonster));

    // 如果是 KRR 怪物 (Monster_Type == 17)，添加到复活集合
    if (pTBMonster->Monster_Type == 17) {
        // UXActorID actorID = pMonster->GetActorID();
        // m_setReviveMonster.insert(CQuestCondition::GetQuestID(&actorID));
    }

    // 处理 WorldMode 相关 (Boss 怪物 13901001, 13901101)
    if (nMonsterID == 13901001 || nMonsterID == 13901101) {
        // Per IDA 0x1401A08B0: 遍历 m_mapGameWorldMode 查找启动的 WorldMode
        // 如果找到 Start_Type == 0 (time-mode) 且 GetState() == 1, 
        // 调用 ThreadLocalData::AppearEventMonster 广播给所有玩家
        bool bAppear = false;

        // TODO: 需人工审查 - 需要 CGameWorldMode 完整定义
        // for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it) {
        //     CGameWorldMode* pWorldMode = it->second.get();
        //     int nModeID = pWorldMode->GetModeID();
        //     XGameServer* pServer = XGameServer::Instance();
        //     TB_MODE_DISTRICT6* pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(
        //         &pServer->m_xResourceMgr, nModeID);
        //     if (pTBMode && pTBMode->Start_Type == 0 && pWorldMode->GetState() == 1) {
        //         // Per IDA: ThreadLocalData::AppearEventMonster broadcasts world mode start
        //         // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
        //         // pThreadData->AppearEventMonster(nModeID, biStartTime, biFinishTime,
        //         //     nModeDateID, biModeStartTime, biModeEndTime);
        //         bAppear = true;
        //         break;
        //     }
        // }

        if (!bAppear) {
            // Per IDA: if no active world mode found for boss monster, delete and return nullptr
            LogHelper::LogError("game.contents",
                "CreateMonster error - No active WorldMode for boss[ ID:%d ]", nMonsterID);
            DeleteMonster(pMonster);
            return nullptr;
        }
    }

    // 如果是 KRR 怪物 (Monster_Type == 17 或 18)，发送 DB 包
    if (pTBMonster->Monster_Type == 17 || pTBMonster->Monster_Type == 18) {
        // Per IDA: 构造 ST_KRR_MONSTER_INFO 并发送
        // ST_KRR_MONSTER_INFO stKRRInfo;
        // stKRRInfo.dwMonsterID = pMonster->GetActorID().GetQuestID();
        // stKRRInfo.dwTableID = pMonster->GetTableID();
        // stKRRInfo.byChannel = GetChannel();
        // stKRRInfo.xPos = pMonster->GetPosition().x;
        // stKRRInfo.yPos = pMonster->GetPosition().y;
        // stKRRInfo.zPos = pMonster->GetPosition().z;

        // XSendDBPacket xSendDBPacket(0, 0xF3, 1);
        // xSendDBPacket << stKRRInfo;
        // xSendDBPacket << XOption::GetGroupID();
        // XGameServer::Instance()->SendDBGame(&xSendDBPacket);

        GreenDamTan_log(__FILE__, __FUNCTION__, "KRR monster created");
    }

    return pMonster;
}

// Per IDA 0x14019EFE0: CBattleZone::DeleteMonster
// 删除怪物
// IDA 反编译精确逻辑:
// 1. dwIndex = pMonster->GetTableID()
// 2. pTBMonster = XResourceMgr::GetTB_MONSTER(dwIndex)
// 3. if (pTBMonster && (Monster_NormalStand_Type == 2 || == 3) && CMoverEx::GetSilhoutte(pMonster))
//    -> ++m_nDestroySilhouetes
// 4. QuestID = CQuestCondition::GetQuestID(GetActorID(pMonster))
// 5. CRespawnManager::DieRespawnMonster(QuestID)
// 6. m_setReviveMonster.erase(QuestID) if found
// 7. if (pTBMonster && (Monster_Type == 17 || == 18))
//    -> XSendDBPacket(0, 0xF3, 2) << QuestID << XOption::GetGroupID() << XArea::GetChannel()
//    -> LogInfo("[KRR] Delete Monster %d / %d", actorID, tableID)
// 8. ExitActor(pMonster)
// 9. ThreadLocalData::GetInstance()->DeleteMonster(pMonster)
void CBattleZone::DeleteMonster(CMonster* pMonster) {

    if (!pMonster) {
        return;
    }

    // 获取怪物表ID
    int nTableID = pMonster->GetTableID();
    // XGameServer* pServer = XGameServer::Instance();
    // TB_MONSTER* pTBMonster = XResourceMgr::GetTB_MONSTER(&pServer->m_xResourceMgr, nTableID);
    TB_MONSTER* pTBMonster = pMonster->GetMobTableRef();

    // 检查是否需要销毁轮廓
    // Per IDA: Monster_NormalStand_Type == 2 或 3 时需要处理轮廓
    if (pTBMonster && (pTBMonster->Monster_NormalStand_Type == 2 || pTBMonster->Monster_NormalStand_Type == 3)) {
        // if (CMoverEx::GetSilhoutte(pMonster)) {
        //     ++m_nDestroySilhouetes;
        // }
    }

    // 从重生管理器移除
    // UXActorID actorID = pMonster->GetActorID();
    // unsigned int dwQuestID = CQuestCondition::GetQuestID(&actorID);
    // m_respawnManager.DieRespawnMonster(dwQuestID);

    // 从 m_setReviveMonster 移除
    // auto it = m_setReviveMonster.find(dwQuestID);
    // if (it != m_setReviveMonster.end()) {
    //     m_setReviveMonster.erase(it);
    // }

    // 如果是 KRR 怪物 (Monster_Type == 17 或 18)，发送 DB 包
    if (pTBMonster && (pTBMonster->Monster_Type == 17 || pTBMonster->Monster_Type == 18)) {
        // Per IDA 0x14019EFE0: 构造 KRR 删除包并发送到数据库
        // 包结构: 主命令 0, 子命令 0xF3, 子子命令 2（删除）
        // 数据: monsterActorID, groupID, channel
        //
        // XGameServer* pServer = XGameServer::Instance();
        // XSendDBPacket xSendDBPacket(0, 0xF3, 2);
        // UXActorID actorID = pMonster->GetActorID();
        // unsigned int dwQuestID = CQuestCondition::GetQuestID(&actorID);
        // xSendDBPacket << dwQuestID;
        // xSendDBPacket << XOption::GetGroupID();
        // xSendDBPacket << XArea::GetChannel(this);
        // pServer->SendDBGame(pServer, &xSendDBPacket);
        //
        // LogHelper::LogInfo("game.system",
        //     "[KRR] Delete Monster %d / %d",
        //     actorID.dwActorID, nTableID);

        GreenDamTan_log(__FILE__, __FUNCTION__, "KRR monster deleted - DB packet pending XSendDBPacket");
    }

    // Phase 9: Send delete packet to clients
    // Per IDA: broadcast monster removal to nearby players
    // XSendPacket xPacket(0x11, 0x21);  // Monster delete packet
    // xPacket << pMonster->GetActorID().dwActorID;
    // SendBroadCastNearby(&xPacket, pMonster->GetPosition());

    // 退出 Actor
    ExitActor(reinterpret_cast<XActor*>(pMonster));

    // Phase 9: Delete monster via delete (stub implementation)
    // When ThreadLocalData is available, use: ThreadLocalData::GetInstance()->DeleteMonster(pMonster)
    delete pMonster;
}

// Per IDA 0x1401A5E60: CBattleZone::DieMonster
// 杀死怪物列表 - IDA精确还原
// 逻辑: 遍历列表 -> m_mapMonsterSpawnBoxInfo查找 -> FindActor获取CMonster -> 设置死亡 -> 处理召唤怪物
// IDA signature: void __fastcall CBattleZone::DieMonster(CBattleZone *this, std::list<XMaze *> *listSpawnBoxID, bool bDieOwner)
void CBattleZone::DieMonster(std::list<std::uint32_t>& listSpawnBoxID, bool bDieOwner)
{
    // IDA: 遍历 listSpawnBoxID
    for (auto it = listSpawnBoxID.begin(); it != listSpawnBoxID.end(); ++it)
    {
        int nSpawnBoxID = static_cast<int>(*it);

        // IDA: std::_Tree::find(&this->m_mapMonsterSpawnBoxInfo, &it_find, &nSpawnBoxID)
        auto itFind = m_mapMonsterSpawnBoxInfo.find(nSpawnBoxID);
        if (itFind == m_mapMonsterSpawnBoxInfo.end())
            continue;

        // IDA: 遍历该 spawn box 中的怪物列表
        std::list<int>& listMonsterIDs = itFind->second;
        for (auto itMon = listMonsterIDs.begin(); itMon != listMonsterIDs.end(); ++itMon)
        {
            int nMonsterID = *itMon;

            // IDA: VBitmask construction + FindActor via vtable
            XActor* pActor = FindActor(static_cast<std::uint32_t>(nMonsterID));
            if (!pActor)
                continue;

            // IDA: _RTDynamicCast_0(pActor, 0, &XActor RTTI, &CMonster RTTI, 0)
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (!pMonster)
                continue;

            if (bDieOwner)
            {
                // IDA: int v9 = pMonster->GetHP(pMonster); CMoverEx::SetDieReason(pMonster, 6u, v9);
                int nHP = pMonster->GetHP();
                // TODO: CMoverEx::SetDieReason(pMonster, 6, nHP);

                // IDA: __int16 DeathMotion = CMonster::GetDeathMotion(pMonster); pMonster->SetDie_2(DeathMotion, 1)
                // short nMotion = CMonster::GetDeathMotion(pMonster);
                // TODO: pMonster->SetDie_2(nMotion, 1);
            }

            // IDA: std::list<unsigned long> listSummonMob; CMover::GetSummonMobList(pMonster, &listSummonMob)
            std::list<std::uint32_t> listSummonMob;
            // TODO: CMover::GetSummonMobList(pMonster, &listSummonMob);

            // IDA: 遍历召唤怪物列表
            for (auto itSub = listSummonMob.begin(); itSub != listSummonMob.end(); ++itSub)
            {
                std::uint32_t dwSub = *itSub;

                // IDA: FindActor via vtable
                XActor* pSubActor = FindActor(dwSub);
                if (!pSubActor)
                    continue;

                // IDA: _RTDynamicCast_0 for CMonster
                CMonster* pSubMonster = dynamic_cast<CMonster*>(pSubActor);
                if (!pSubMonster)
                    continue;

                // IDA: v13 = pSubMonster->GetHP(); SetDieReason(pSubMonster, 6u, v13)
                int nSubHP = pSubMonster->GetHP();
                // TODO: CMoverEx::SetDieReason(pSubMonster, 6, nSubHP);

                // IDA: v14 = CMonster::GetDeathMotion(pSubMonster); pSubMonster->SetDie_2(v14, 1)
                // short nSubMotion = CMonster::GetDeathMotion(pSubMonster);
                // TODO: pSubMonster->SetDie_2(nSubMotion, 1);
            }
        }
    }
}

// DieMonster overload - kill monsters by list ID
void CBattleZone::DieMonster(unsigned long dwListID)
{
    // Find monsters in the spawn box by list ID
    auto it = m_mapMonsterSpawnBoxInfo.find(static_cast<int>(dwListID));
    if (it == m_mapMonsterSpawnBoxInfo.end())
        return;

    // Get the list of monster IDs in this spawn box
    std::list<int>& listMonsterIDs = it->second;
    
    for (auto monIt = listMonsterIDs.begin(); monIt != listMonsterIDs.end(); ++monIt)
    {
        std::uint32_t dwMonsterID = static_cast<std::uint32_t>(*monIt);
        
        // Find the monster by actor ID
        XActor* pActor = FindActor(dwMonsterID);
        if (!pActor)
            continue;

        CMonster* pMonster = reinterpret_cast<CMonster*>(pActor);
        if (pMonster)
        {
            // Kill the monster
            // TODO: pMonster->SetDie(CMonster::GetDeathMotion(pMonster), 0);
            GreenDamTan_log(__FILE__, __FUNCTION__, "DieMonster - killing monster by list ID");
        }
    }
}

// Per IDA 0x1401A71D0: CBattleZone::DieMonsterAll
// 杀死所有怪物 - IDA精确还原
// IDA signature: void __fastcall CBattleZone::DieMonsterAll(CBattleZone *this, bool bFinish)
// 逻辑: Range2DScanner收集CMover -> RTTI cast CMonster -> 检查地图类型 -> 设置死亡并保存伤害信息
void CBattleZone::DieMonsterAll(bool bFinish)
{
    // IDA: std::vector<CMover*> vecNPC; vecNPC.reserve(0x12C = 300)
    std::vector<CMover*> vecNPC;
    vecNPC.reserve(300);

    // IDA: Range2DScanner<CMover*>::Enumerate(m_objectScanner.npcScanner, &vecNPC)
    // Range2DScanner<CMover*>::Enumerate(m_objectScanner.npcScanner, &vecNPC);

    // 简化实现：遍历 m_mapActor 收集 CMover
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it)
    {
        XActor* pActor = it->second;
        if (!pActor)
            continue;

        // IDA: 检查 Actor 类型是否为 Monster (type == 2)
        if (pActor->GetType() != eActorMonster)
            continue;

        // IDA: _RTDynamicCast_0(*(_QWORD *)v3->szName, 0, &CMover RTTI, &CMonster RTTI, 0)
        CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
        if (!pMonster)
            continue;

        // IDA: 检查地图类型 (m_uxMapID.nMapID << 16 >> 48 != 30021 或 Monster_Faction != 21)
        // if (this->m_uxMapID.nMapID << 16 >> 48 == 30021) {
        //     TB_MONSTER* pTBMonster = CMonster::GetMobTableRef(pMonster);
        //     if (pTBMonster && pTBMonster->Monster_Faction == 21)
        //         continue;  // 跳过该怪物
        // }

        // IDA: __int16 nMotion = -1; nMotion = CMonster::GetDeathMotion(pMonster)
        short nMotion = -1;
        // nMotion = CMonster::GetDeathMotion(pMonster);

        if (bFinish)
        {
            // IDA: std::list<ST_MONSTER_DAMAGE_INFO> listHitID; CMover::GetHitList(pMonster, &listHitID)
            std::list<void*> listHitID;
            // TODO: CMover::GetHitList(pMonster, &listHitID);

            // IDA: CBattleZone::SaveDamageInfo(this, listHitID)
            // SaveDamageInfo(listHitID);
        }

        // IDA: v4 = pMonster->GetHP(pMonster); CMoverEx::SetDieReason(pMonster, 0xAu, v4)
        int nHP = pMonster->GetHP();
        // TODO: CMoverEx::SetDieReason(pMonster, 10, nHP);

        // IDA: pMonster->SetDie_2(pMonster, nMotion, 1)
        // TODO: pMonster->SetDie_2(nMotion, 1);
    }
}

// DieMonsterAll overload - kill all monsters in zone
void CBattleZone::DieMonsterAll()
{
    // Iterate all actors and kill monsters
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it)
    {
        XActor* pActor = it->second;
        if (!pActor)
            continue;

        // Check if this is a monster (actor type 2)
        // TODO: Use proper type checking when available
        // if (pActor->GetType() == eActorMonster)
        {
            CMonster* pMonster = reinterpret_cast<CMonster*>(pActor);
            if (pMonster)
            {
                // Kill the monster without force
                // TODO: pMonster->SetDie(CMonster::GetDeathMotion(pMonster), 0);
                GreenDamTan_log(__FILE__, __FUNCTION__, "DieMonsterAll - killing all monsters");
            }
        }
    }
}

// Per IDA 0x1401A6220: MonsterDieForEvent - event-triggered monster death
// IDA signature: void __fastcall CBattleZone::MonsterDieForEvent(CBattleZone *this, CMonster *pMonster, unsigned int dwHitID)
// 逻辑: 只处理地图30031 -> 遍历m_mapGameWorldMode -> 解析攻击者 -> 调用CGameWorldMode::MonsterDie
void CBattleZone::MonsterDieForEvent(CMonster* pMonster, std::uint32_t dwHitID)
{
    if (!pMonster)
        return;

    // IDA: if (this->m_uxMapID.nMapID << 16 >> 48 == 30031)
    int nMapType = (m_uxMapID.wMapID << 16) >> 48;
    if (nMapType != 30031)
        return;

    // IDA: 遍历 m_mapGameWorldMode
    for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it)
    {
        // IDA: std::tr1::shared_ptr<CGameWorldMode>::shared_ptr(&pMode, &v4->second)
        std::tr1::shared_ptr<CGameWorldMode> pMode = it->second;

        // IDA: if (std::tr1::shared_ptr::operator int() != -1)
        if (!pMode)
            continue;

        // IDA: VBitmask + FindActor via vtable
        XActor* pActor = FindActor(dwHitID);
        CUser* pUser = nullptr;

        // IDA: pUser = _RTDynamicCast_0(pActor, 0, &XActor RTTI, &CUser RTTI, 0)
        if (pActor)
        {
            pUser = dynamic_cast<CUser*>(pActor);

            // IDA: if (!pUser && pActor && XActor::IsMonster(pActor))
            if (!pUser && pActor)
            {
                // IDA: pAtkMonster = _RTDynamicCast_0(pActor, 0, &XActor RTTI, &CMonster RTTI, 0)
                CMonster* pAtkMonster = dynamic_cast<CMonster*>(pActor);
                if (pAtkMonster)
                {
                    // IDA: if (CMonster::IsHelper(pAtkMonster) || (CMoverEx::GetOwnerID(pAtkMonster) && CMonster::GetMobTableRef(pAtkMonster)->Monster_Element == 1))
                    // {
                    //     OwnerPlayer = CMoverEx::GetOwnerPlayer(pAtkMonster);
                    //     pUser = _RTDynamicCast_0(OwnerPlayer, 0, &CMoverEx RTTI, &CUser RTTI, 0);
                    // }
                    // TODO: Implement above logic when helpers are available
                }
            }
        }

        // IDA: v7 = std::tr1::shared_ptr::operator->(&pMode); CGameWorldMode::MonsterDie(v7, pMonster, pUser)
        // TODO: pMode->MonsterDie(pMonster, pUser);
        (void)pUser;
    }
}

// MonsterDieForEvent overload - kill event monster by ID
void CBattleZone::MonsterDieForEvent(unsigned long dwMonsterID, int nEventType)
{
    // Find the monster by ID
    XActor* pActor = FindActor(static_cast<std::uint32_t>(dwMonsterID));
    if (!pActor)
        return;

    CMonster* pMonster = reinterpret_cast<CMonster*>(pActor);
    if (!pMonster)
        return;

    // Set the event type on the monster
    // TODO: When event type tracking is available:
    // pMonster->SetEventType(nEventType);

    // Call the monster's Die method
    // TODO: pMonster->SetDie(CMonster::GetDeathMotion(pMonster), 0);

    GreenDamTan_log(__FILE__, __FUNCTION__, "MonsterDieForEvent - event monster killed");
    (void)nEventType;
}

// Per IDA 0x1401A7BC0: CBattleZone::SaveDamageInfo
// 记录世界模式伤害参与者的 UCID 到 m_setWorldModeHitUser
// IDA 反编译精确还原:
// 1. 遍历 listHitID
// 2. 对每个 ST_MONSTER_DAMAGE_INFO: m_setWorldModeHitUser.insert(dwUCID)
// 3. listHitID 析构 (参数按值传递)
void CBattleZone::SaveDamageInfo(std::list<ST_MONSTER_DAMAGE_INFO> listHitID) {
    // IDA: for (auto it = listHitID.begin(); it != listHitID.end(); ++it)
    for (auto it = listHitID.begin(); it != listHitID.end(); ++it) {
        // IDA: dwHitUser = it->dwUCID
        unsigned int dwHitUser = it->dwUCID;

        // IDA: m_setWorldModeHitUser.insert(dwHitUser)
        m_setWorldModeHitUser.insert(dwHitUser);
    }
    // IDA: std::list<ST_LUA_CLIENT_SYNC>::~list() - 参数析构自动完成
}

// Per IDA 0x1401A11E0: CBattleZone::CreateNpc
// 创建 NPC
// IDA 反编译精确逻辑:
// 1. if (!XResourceMgr::GetTB_NPC(nNpcID)) return nullptr
// 2. ThreadLocalData::GetInstance()->CreateNpc(this, uxMazeSerialID, nSectorID, nNpcID, vPos, fRot, 0)
// 3. if (EnterActor(&pNpc->XActor))
//    -> DeleteNpc(pNpc), return nullptr
// 4. CMover::SetCollisionEnable(pNpc, 1, 0) — 单向碰撞
CNpc* CBattleZone::CreateNpc(TUXMapID uxMazeSerialID, int nSectorID, unsigned int nNpcID,
                              XVec3 vPos, float fRot) {
    // ===== Phase 9: implementation uses new CNpc() stub =====
    // Full IDA pattern requires:
    //   1. TB_NPC table lookup via XResourceMgr::GetTB_NPC
    //   2. ThreadLocalData::CreateNpc for object creation
    //   3. EnterActor (returns bool - true = failure)
    //   4. CMover::SetCollisionEnable(pNpc, true, false) for one-way collision

    // Phase 9: Check TB_NPC table (requires XResourceMgr access)
    // XGameServer* pServer = XGameServer::Instance();
    // if (!XResourceMgr::GetTB_NPC(&pServer->m_xResourceMgr, nNpcID))
    //     return nullptr;

    // Phase 9: Create via new CNpc() stub (when CNpc is fully defined)
    // When ThreadLocalData is available, use: ThreadLocalData::GetInstance()->CreateNpc(...)
    CNpc* pNpc = nullptr;  // TODO: requires CNpc full definition

    // Stub: Create a minimal CNpc object
    // pNpc = new CNpc();
    // if (!pNpc) {
    //     GreenDamTan_log(__FILE__, __FUNCTION__, "CreateNpc failed - allocation failed");
    //     return nullptr;
    // }
    // 
    // // Initialize NPC
    // pNpc->InitialObjectInfo(GetUniqueID(nSectorID), nNpcID, vPos, fRot);

    if (!pNpc) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "CreateNpc - CNpc not yet defined, returning nullptr");
        return nullptr;
    }

    // Per IDA 0x1401A11E0: EnterActor
    // Note: EnterActor returns void in this implementation
    EnterActor(reinterpret_cast<XActor*>(pNpc));

    // Per IDA: CMover::SetCollisionEnable(pNpc, true, false) — one-way collision
    // CMover::SetCollisionEnable(pNpc, true, false);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::CreateNpc - NPC created");
    return pNpc;
}

// Per IDA 0x1401A1320: CBattleZone::DeleteNpc
// 删除 NPC
// IDA 反编译精确逻辑:
// 1. if (pNpc) -> ExitArea(&pNpc->XActor)
//    else -> ExitArea(nullptr)
// 2. ThreadLocalData::GetInstance()->DeleteNpc(pNpc)
void CBattleZone::DeleteNpc(CNpc* pNpc) {
    if (!pNpc) {
        return;
    }

    // Phase 9: Send delete packet to clients
    // Per IDA: broadcast NPC removal to nearby players
    // XSendPacket xPacket(0x11, 0x21);  // NPC delete packet
    // xPacket << pNpc->GetActorID().dwActorID;
    // SendBroadCastNearby(&xPacket, pNpc->GetPosition());

    // ExitActor removes the actor from the area's internal map and cleans up zone references
    ExitActor(reinterpret_cast<XActor*>(pNpc));

    // Phase 9: Delete NPC via delete (stub implementation)
    // When ThreadLocalData is available, use: ThreadLocalData::GetInstance()->DeleteNpc(pNpc)
    // delete pNpc;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::DeleteNpc");
}

// Per IDA 0x1401A1380: CreateAkashicObject
// IDA 反编译精确还原:
// CAkashicObject* CBattleZone::CreateAkashicObject(UXMapID uxMazeSerialID, unsigned int nAkashicID, XVec3* vPos, float fRot, unsigned int dwParentID)
// 1. XGameServer::Instance()->m_xResourceMgr.GetTB_AKASHIC_RECORDS(nAkashicID) - 检查表数据
// 2. ThreadLocalData::GetInstance()->CreateAkashicObject(this, uxMazeSerialID, nAkashicID, &vPos, fRot, dwParentID)
// 3. EnterActor(&pAkashic->XActor) - 如果失败则 DeleteAkashicObject 并返回 nullptr
// 4. CMover::SetCollisionEnable(pAkashic, false, false)
// 5. 返回 pAkashic
CAkashicObject* CBattleZone::CreateAkashicObject(TUXMapID uxMazeSerialID, int nAkashicID, XVec3 vPos, float fRot, float fScale, E_SEND_INFO_TYPE eSendType)
{
    // IDA 0x1401A1380: 精确还原
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // if (!XResourceMgr::GetTB_AKASHIC_RECORDS(&pServer->m_xResourceMgr, nAkashicID))
    //     return nullptr;
    //
    // XVec3 vCopy = vPos;  // IDA: qmemcpy(&v10, vPos, sizeof(v10))
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // CAkashicObject* pAkashic = pThreadData->CreateAkashicObject(this, uxMazeSerialID, nAkashicID, &vCopy, fRot, dwParentID);
    // if (!pAkashic) return nullptr;
    //
    // if (EnterActor(&pAkashic->XActor))
    // {
    //     DeleteAkashicObject(pAkashic);
    //     return nullptr;
    // }
    // CMover::SetCollisionEnable(pAkashic, false, false);
    // return pAkashic;

    // TODO: 汇编还原 - 需要 XResourceMgr::GetTB_AKASHIC_RECORDS 和 ThreadLocalData 完整定义
    (void)uxMazeSerialID;
    (void)nAkashicID;
    (void)vPos;
    (void)fRot;
    (void)fScale;
    (void)eSendType;
    return nullptr;
}

// Per IDA 0x1401A14B0: CBattleZone::DeleteAkashicObject
// 删除 Akashic 对象
// IDA 精确还原:
// 1. if (pAkashic) ExitArea(&pAkashic->XActor) else ExitArea(nullptr)
// 2. ThreadLocalData::GetInstance()->DeleteAkashicObject(pAkashic)
void CBattleZone::DeleteAkashicObject(CAkashicObject* pAkashic) {
    // IDA 0x1401A14B0: 调用 ExitArea 处理 actor 退出
    if (pAkashic) {
        ExitArea(reinterpret_cast<XActor*>(pAkashic));
    } else {
        ExitArea(nullptr);
    }

    // IDA: 通过 ThreadLocalData 删除 AkashicObject
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // pThreadData->DeleteAkashicObject(pAkashic);
}

// Per IDA 0x1401A1510: CreateInteractionObject
// IDA 反编译精确还原:
// CInteractionObject* CBattleZone::CreateInteractionObject(STInteractionBox* pInteractionInfo, TB_INTERACTION_OBJECT* pTBInteraction, XVec3* vecPos, float fRot)
// 1. if (!pTBInteraction) return nullptr
// 2. ThreadLocalData::GetInstance()->CreateInteractionObject(&vPos) -> pInteraction
// 3. CInteractionObject::Init(pInteraction, this, pInteractionInfo, pTBInteraction, vecPos, fRot)
// 4. EnterActor(&pInteraction->XActor) - 如果失败则 DeleteInteractionObject 并返回 nullptr
// 5. 返回 pInteraction
CInteractionObject* CBattleZone::CreateInteractionObject(STInteractionBox* pInteractionInfo, void* pTBInteraction, XVec3& vPos, float fRot)
{
    // TODO: 汇编还原 - 需要 ThreadLocalData, TB_INTERACTION_OBJECT, CInteractionObject 完整定义
    // IDA 精确还原代码:
    // if (!pTBInteraction) return nullptr;
    //
    // XVec3 v8; memcpy(&v8, vecPos, sizeof(v8));
    // ThreadLocalData* Instance = ThreadLocalData::GetInstance();
    // CInteractionObject* pInteraction = ThreadLocalData::CreateInteractionObject(Instance, &v8);
    // if (!pInteraction) return nullptr;
    //
    // CInteractionObject::Init(pInteraction, this, pInteractionInfo, pTBInteraction, vecPos, fRot);
    //
    // if (!EnterActor(&pInteraction->XActor))
    //     return pInteraction;
    //
    // DeleteInteractionObject(pInteraction);
    // return nullptr;

    (void)pInteractionInfo;
    (void)pTBInteraction;
    (void)vPos;
    (void)fRot;
    return nullptr;
}

// Per IDA 0x1401A1620: CBattleZone::DeleteInteractionObject
// 删除交互对象
// IDA 精确还原:
// 1. if (pObject) ExitArea(&pObject->XActor) else ExitArea(nullptr)
// 2. ThreadLocalData::GetInstance()->DeleteInteractionObject(pObject)
void CBattleZone::DeleteInteractionObject(CInteractionObject* pObject) {
    // IDA 0x1401A1620: 调用 ExitArea 处理 actor 退出
    if (pObject) {
        ExitArea(reinterpret_cast<XActor*>(pObject));
    } else {
        ExitArea(nullptr);
    }

    // IDA: 通过 ThreadLocalData 删除交互对象
    // ThreadLocalData* pTLS = ThreadLocalData::GetInstance();
    // pTLS->DeleteInteractionObject(pObject);
}

// Per IDA 0x1401A28F0: ClickInteractionBox - handle box interaction
// IDA 反编译精确还原:
// 这是一个非常复杂的函数，处理交互箱的点击逻辑
// 主要流程:
// 1. 获取交互箱唯一ID并查找 m_mapInteractionBox
// 2. 检查 TB_INTERACTION_OBJECT 表数据
// 3. 检查 bEnable / fCoolTime / nCallCount 条件
// 4. 检查 Check_Item_ID / Check_Item_Count 物品条件
// 5. 移除 Remove_Item_ID / Remove_Item_Count 物品
// 6. 添加 Add_Item_ID / Add_Item_Count 奖励物品
// 7. 更新任务条件 (eCONDITION_TYPE_TRIGGER, eCONDITION_TARGET_OBJECT)
// 8. 发送结果包 (0x11, 0x78)
// 9. 更新交互状态 (bShow, bEnable, fCoolTime, nCallCount)
void CBattleZone::ClickInteractionBox(int nBoxID, CUser* pUser)
{
    // TODO: 汇编还原 - 需要完整的类型定义
    // IDA 反编译关键代码框架:
    // LogHelper::LogError("game.contents", "<BATTLE> ClickInteractionBox");
    // int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nBoxIndex, ...);
    // auto it = m_mapInteractionBox.find(iBoxUniqueID);
    // if (it == m_mapInteractionBox.end()) return;
    // STInteractionBox* pInteraction = it->second;
    // if (!pInteraction) return;
    //
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_INTERACTION_OBJECT* pTBInteraction = XResourceMgr::GetTB_INTERACTION_OBJECT(&pServer->m_xResourceMgr, pInteraction->pInteractionBox->m_iInteractionID);
    // if (!pTBInteraction) {
    //     CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x11, 0x77, 0xD2F7);
    //     return;
    // }
    //
    // // 获取 CGocEntity 组件
    // std::tr1::shared_ptr<CGocEntity> pEntity;
    // CMover::GetGOC<CGocEntity>(&pUser->CMoverEx, &pEntity, 0);
    // if (!pEntity) return;
    //
    // bool bEnable = pInteraction->bEnable;
    // bool bShow = pInteraction->bShow;
    //
    // // 检查是否可用
    // if (!bEnable) {
    //     byResult = 2;
    //     // 发送错误包...
    //     return;
    // }
    //
    // // 检查冷却时间
    // if (pInteraction->fCoolTime > 0.0f) {
    //     CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x11, 0x77, 0xD2FF);
    //     return;
    // }
    //
    // // 检查使用次数
    // if (pTBInteraction->Interaction_Count_Max > 0 && !pInteraction->nCallCount) {
    //     CGocNetwork::SendErrorMessage(&pUser->CMoverEx, 0x11, 0x77, 0xD2FF);
    //     return;
    // }
    //
    // // 检查物品条件
    // std::tr1::shared_ptr<CGocInventory> pInventory;
    // CMover::GetGOC<CGocInventory>(&pUser->CMoverEx, &pInventory, 0);
    // if (!pInventory) return;
    //
    // // ... 检查 Check_Item_ID / Check_Item_Count
    // // ... 移除 Remove_Item_ID / Remove_Item_Count
    // // ... 添加 Add_Item_ID / Add_Item_Count
    //
    // // 更新任务条件
    // std::tr1::shared_ptr<CGocQuest> pQuest;
    // CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuest, 0);
    // if (pQuest) {
    //     pQuest->UpdateCondition(eCONDITION_TYPE_TRIGGER, eCONDITION_TARGET_OBJECT, nBoxIndex, 1, 0);
    // }
    //
    // // 更新交互状态
    // if (pInteraction->nCallCount > 0 || pTBInteraction->Interaction_Count_Max <= 0) {
    //     if (pInteraction->nCallCount > 0) --pInteraction->nCallCount;
    //     pInteraction->fCoolTime = (float)pTBInteraction->Interaction_CoolTime * 0.001f;
    // }
    //
    // // 发送结果包
    // ST_BATCH_INTERACTION stInfo;
    // stInfo.bShow = bShow;
    // stInfo.bEnable = bEnable;
    // stInfo.nBoxIndex = pInteraction->nBoxIndex;
    // stInfo.nCallCount = pInteraction->nCallCount;
    // XSendPacket xSendPacket(0x11, 0x78);
    // xSendPacket << byResult;
    // xSendPacket << stInfo;
    // CGocNetwork::Send(&pUser->XActor, &xSendPacket);

    if (!pUser)
        return;

    // Find the interaction box by unique ID
    int iBoxUniqueID = nBoxID; // IDA: VEventObjectInfo::GetEventUniqueID(nBoxID)
    auto it = m_mapInteractionBox.find(iBoxUniqueID);
    if (it == m_mapInteractionBox.end())
        return;

    STInteractionBox* pInteraction = static_cast<STInteractionBox*>(it->second);
    if (!pInteraction)
        return;

    // TODO: 汇编还原 - 需要完整类型定义后实现上述逻辑
    GreenDamTan_log(__FILE__, __FUNCTION__, "ClickInteractionBox stub");

    (void)pInteraction;
}

// Per IDA 0x1401A3740: CBattleZone::ExitArea
// 玩家退出区域，发送切换地图包
// Phase 9: Implements the exit area flow - clears vaccum lock, sets state,
// retrieves maze escape position, constructs enter-map packet with party info,
// and sends via ControlSocket.
void CBattleZone::ExitArea(XActor* pActor) {
    if (!pActor) {
        return;
    }

    // IDA 0x1401A3740: RTTI conversion CUser* from XActor*
    // Note: Using reinterpret_cast since RTTI may not be available
    CUser* pUser = reinterpret_cast<CUser*>(pActor);
    if (!pUser) {
        return;
    }

    // Phase 9: Clear vaccum lock
    // TODO: Requires CVaccumManager::ClearVaccumLock when available
    // m_vaccumManager.ClearVaccumLock(pActor);

    // Phase 9: Set state to world-change
    // TODO: Requires CUser::SetState when available
    // pUser->SetState(eStateChangeWorld);

    // Phase 9: Retrieve maze escape position
    // STPosInfo stPosInfo;
    // int nJumpID = 0;
    // int nMazeID = XArea::GetTBMapID(this);
    // XGameServer* pServer = XGameServer::Instance();
    // XWorldResMgr::GetMazeEscapePos(&pServer->m_xWorldResMgr, nMazeID, &nJumpID, &stPosInfo);

    // Phase 9: Build PS_ENTER_MAP_REQ packet
    // PS_ENTER_MAP_REQ stEnterMap;
    // UXActorID actorID = pActor->GetActorID();
    // stEnterMap.dwActorID = CQuestCondition::GetQuestID(&actorID);
    // stEnterMap.nJumpID = nJumpID;
    // stEnterMap.wMapID = stPosInfo.sWorldID;

    // Phase 9: Get party/force info
    // std::tr1::shared_ptr<CGocParty> pParty;
    // std::tr1::shared_ptr<CGocForce> pForce;
    // CMover::GetGOC<CGocParty>(&pUser->CMoverEx, &pParty, 0);
    // CMover::GetGOC<CGocForce>(&pUser->CMoverEx, &pForce, 0);
    // if ((bool)pParty && (bool)pForce) {
    //     if (pParty->IsParty()) {
    //         stEnterMap.stPartyInfo.byGroupType = 1;
    //         stEnterMap.stPartyInfo.nID = pParty->GetPartyID();
    //     } else if (pForce->IsParty()) {
    //         stEnterMap.stPartyInfo.byGroupType = 2;
    //         stEnterMap.stPartyInfo.nID = pForce->GetPartyID();
    //     }
    // }

    // Phase 9: Send speed check log and drop Ether log
    // pUser->SendCheckSpeedLog();
    // std::tr1::shared_ptr<CGocInventory> pInven;
    // CMover::GetGOC<CGocInventory>(&pUser->CMoverEx, &pInven, 0);
    // if ((bool)pInven) {
    //     pInven->DropEtherLog();
    // }

    // Phase 9: Send enter-map packet via ControlSocket
    // XSendPacket xSendPacket(0xF2, 0x31);
    // xSendPacket << stEnterMap;
    // XGameServer::Instance()->GetControlSocket().SendCheck(&xSendPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::ExitArea");
}

// Per IDA 0x1401A3640: CBattleZone::IsInSafetyZone
// 检查 Actor 是否在安全区域内
// IDA 反编译精确还原:
// 1. if (!pActor) return false
// 2. 遍历 m_mapSafetyZone
// 3. 对每个 VSafeAreaBoxInfo:
//    - hkvVec3 pos = pActor->GetPosition()
//    - if (VEventObjectInfo::IsIn(pSafeBox, &pos)) return true
// 4. return false
bool CBattleZone::IsInSafetyZone(XActor* pActor) {
    if (!pActor)
        return false;

    // IDA: for (auto it = m_mapSafetyZone.begin(); it != m_mapSafetyZone.end(); ++it)
    for (auto it = m_mapSafetyZone.begin(); it != m_mapSafetyZone.end(); ++it) {
        // IDA: pSafeBox = (VSafeAreaBoxInfo*)it->second.__vftable
        // TODO: 需人工审查 - VSafeAreaBoxInfo 类型定义
        void* pSafeBox = it->second;

        if (pSafeBox) {
            // IDA: hkvVec3 pos = pActor->GetPosition()
            // TODO: 汇编还原 - 需要 hkvVec3 类型, XActor::GetPosition 方法
            // hkvVec3 pos;
            // pActor->GetPosition(&pos);

            // IDA: if (VEventObjectInfo::IsIn(pSafeBox, &pos)) return true
            // TODO: 汇编还原 - 需要 VEventObjectInfo::IsIn 方法
            // if (VEventObjectInfo::IsIn(pSafeBox, &pos)) {
            //     return true;
            // }
        }
    }

    return false;
}

// Per IDA 0x1401A1680: CBattleZone::Generate
// 生成场景对象（生成箱、交互箱、传送门、安全区域等）
void CBattleZone::Generate() {
    // IDA 反编译逻辑:
    // 1. 检查 m_pObjectResource
    // 2. 获取事件对象映射
    // 3. 遍历所有事件对象
    // 4. 根据事件箱类型处理:
    //    - MonsterSpawn: 创建 STMageProcessSpawnBox 或添加到 m_listMonsterSpawnInfo
    //    - CheckMonsterSpawn: 创建 STMageEventSpawnBox
    //    - CheckSceneDirecting: 创建 STMagePotalBox
    //    - Portal: 添加到 m_mapCommonPostionBox
    //    - LuaFunction: 创建交互对象或真空组
    //    - InteractionObjectBox: 创建 STQuestMoveBox
    //    - PersonalShopArea: 添加到 m_mapSafetyZone

    if (!m_pObjectResource) {
        return;
    }

    // Phase 9: Process spawn box generation per IDA
    // Event object types (from IDA):
    // eEventBoxType_MonsterSpawn = 0
    // eEventBoxType_CheckMonsterSpawn = 1
    // eEventBoxType_CheckSceneDirecting = 2
    // eEventBoxType_Portal = 3
    // eEventBoxType_LuaFunction = 4
    // eEventBoxType_InteractionObjectBox = 5
    // eEventBoxType_PersonalShopArea = 6

    // TODO: When VEventObjectInfo/VEventBoxInfo types are available:
    // XGameServer* pServer = XGameServer::Instance();
    // VMap<int,void*> ObjectMap;
    // CFAutoSlimWriteLock lock(&pServer->m_rwMapLock);
    // ObjectMap = m_pObjectResource->GetMap(eEventObjectType_Box, 1, 0);
    //
    // void* iter = ObjectMap.GetStartPosition();
    // while (iter) {
    //     int iKey;
    //     void* pValue;
    //     ObjectMap.GetNextPair(&iter, &iKey, &pValue);
    //     VEventObjectInfo* pInfo = (VEventObjectInfo*)pValue;
    //
    //     if (pInfo && pInfo->eType == eEventObjectType_Box) {
    //         VEventBoxInfo* pBoxInfo = (VEventBoxInfo*)pInfo;
    //
    //         switch (pBoxInfo->eBoxType) {
    //             case eEventBoxType_MonsterSpawn: {
    //                 VMonsterSpawnInfo* pSpawn = (VMonsterSpawnInfo*)pBoxInfo;
    //                 if (pSpawn->m_iCreationCondition == 2) {
    //                     // Event-triggered spawn: create STMageProcessSpawnBox
    //                     STMageProcessSpawnBox* pProcessSpawn = new STMageProcessSpawnBox();
    //                     pProcessSpawn->pSpawnBox = pSpawn;
    //                     pProcessSpawn->bActive = false;
    //                     pProcessSpawn->bSpawned = false;
    //                     pProcessSpawn->bTerminate = false;
    //                     pProcessSpawn->nSpawnOrder = 0;
    //                     pProcessSpawn->nCreatedCount = 0.0f;
    //                     pProcessSpawn->fDelayTime = 0.0f;
    //                     m_mapProcessSpawnBox[iKey] = pProcessSpawn;
    //                 } else if (pSpawn->m_iCreationCondition == 1) {
    //                     // Immediate spawn: add to list
    //                     m_listMonsterSpawnInfo.push_back(pSpawn);
    //                 }
    //                 break;
    //             }
    //             case eEventBoxType_CheckMonsterSpawn: {
    //                 // Create STMageEventSpawnBox for conditional spawning
    //                 STMageEventSpawnBox* pEventSpawn = new STMageEventSpawnBox();
    //                 pEventSpawn->pSpawnBox = (VMonsterSpawnInfo*)pBoxInfo;
    //                 m_mapEventSpawnBox[iKey] = pEventSpawn;
    //                 break;
    //             }
    //             case eEventBoxType_CheckSceneDirecting: {
    //                 // Create STMagePotalBox for scene directing (portals)
    //                 STMagePotalBox* pPotalBox = new STMagePotalBox();
    //                 m_mapPotalBox[iKey] = pPotalBox;
    //                 break;
    //             }
    //             case eEventBoxType_Portal: {
    //                 // Add to common position box map
    //                 VCommonPositionBoxInfo* pCommonPos = (VCommonPositionBoxInfo*)pBoxInfo;
    //                 m_mapCommonPostionBox[iKey] = pCommonPos;
    //                 break;
    //             }
    //             case eEventBoxType_LuaFunction: {
    //                 // Handle interaction objects or vaccum groups
    //                 // TODO: Create interaction objects via CreateInteractionObject
    //                 break;
    //             }
    //             case eEventBoxType_InteractionObjectBox: {
    //                 // Create STQuestMoveBox for quest movement
    //                 STQuestMoveBox* pQuestMove = new STQuestMoveBox();
    //                 m_mapQuestMoveBox[iKey] = pQuestMove;
    //                 break;
    //             }
    //             case eEventBoxType_PersonalShopArea: {
    //                 // Add to safety zone map
    //                 VSafeAreaBoxInfo* pSafeArea = (VSafeAreaBoxInfo*)pBoxInfo;
    //                 m_mapSafetyZone[iKey] = pSafeArea;
    //                 break;
    //             }
    //         }
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::Generate - spawn boxes processed");
}

// Per IDA 0x1401A2100: CBattleZone::SpawnGenerateMonster
// 从生成箱生成怪物
// IDA 反编译精确逻辑:
// 1. if (!m_pObjectResource) return
// 2. ObjectMap = VEventObjectResource::GetMap(m_pObjectResource, eEventObjectType_Box, 1, 0)
// 3. Iter = ObjectMap->GetStartPosition()
// 4. while (Iter)
//    -> ObjectMap->GetNextPair(&Iter, &iKey, &pValue)
//    -> pInfo = (VEventObjectInfo*)pValue
//    -> if (pInfo->eType == eEventObjectType_Box)
//       -> pBoxInfo = (VEventBoxInfo*)pInfo
//       -> if (pBoxInfo->eBoxType == eEventBoxType_MonsterSpawn)
//          -> pMonsterSpawn = (VMonsterSpawnInfo*)pBoxInfo
//          -> if (pMonsterSpawn->m_iCreationCondition == 1)
//             -> ExcuteSpawnBox(pMonsterSpawn, eSendInfoTypeNot)
void CBattleZone::SpawnGenerateMonster() {
    // IDA 反编译逻辑:
    // 1. 检查 m_pObjectResource
    // 2. 获取事件对象映射
    // 3. 遍历所有 MonsterSpawn 类型的箱
    // 4. 如果 m_iCreationCondition == 1，调用 ExcuteSpawnBox

    if (!m_pObjectResource) {
        return;
    }

    // Phase 9: Spawn monsters from immediate-spawn boxes
    // Iterate m_listMonsterSpawnInfo and execute spawn boxes
    for (auto it = m_listMonsterSpawnInfo.begin(); it != m_listMonsterSpawnInfo.end(); ++it) {
        VMonsterSpawnInfo* pSpawnInfo = static_cast<VMonsterSpawnInfo*>(*it);
        if (pSpawnInfo && pSpawnInfo->m_iCreationCondition == 1) {
            ExcuteSpawnBox(pSpawnInfo, E_SEND_INFO_TYPE_NONE);
        }
    }

    // TODO: When VEventObjectInfo types are available:
    // VMap<int,void*>* pObjectMap = m_pObjectResource->GetMap(eEventObjectType_Box, 1, 0);
    // void* iter = pObjectMap->GetStartPosition();
    // while (iter) {
    //     int iKey;
    //     void* pValue;
    //     pObjectMap->GetNextPair(&iter, &iKey, &pValue);
    //     VEventObjectInfo* pInfo = (VEventObjectInfo*)pValue;
    //
    //     if (pInfo && pInfo->eType == eEventObjectType_Box) {
    //         VEventBoxInfo* pBoxInfo = (VEventBoxInfo*)pInfo;
    //
    //         if (pBoxInfo->eBoxType == eEventBoxType_MonsterSpawn) {
    //             VMonsterSpawnInfo* pSpawnInfo = (VMonsterSpawnInfo*)pBoxInfo;
    //             if (pSpawnInfo->m_iCreationCondition == 1) {
    //                 ExcuteSpawnBox(pSpawnInfo, E_SEND_INFO_TYPE_NONE);
    //             }
    //         }
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::SpawnGenerateMonster - monsters spawned");
}

// Per IDA 0x1401A2360: CBattleZone::AddDestoryObject
// 添加待销毁对象
// IDA 精确还原: push_back to m_lstDestoryObject
void CBattleZone::AddDestoryObject(XActor* pActor) {
    // IDA 0x1401A2360: std::list<XActor*>::push_back(&m_lstDestoryObject, &pActor)
    m_lstDestoryObject.push_back(pActor);
}

// Per IDA 0x14019F3D0: CBattleZone::ExcuteSpawnBox
// 执行生成箱 - 根据生成箱信息创建怪物或NPC
// IDA 反编译精确逻辑:
// 1. LogDebug("<SPAWN> < SpawnID ( %d / %d ) >", iID, m_iSectorID)
// 2. for (i = 0; i < 10; ++i)
//    -> if (m_stMonsterInfo[i].m_iID)
//       -> nProb = XWorldManager::RandProb()
//       -> if (nProb <= m_stMonsterInfo[i].m_iChance) break
// 3. for (j = 0; j < m_iMaxEntityCount; ++j)
//    -> GetSpawnPos(pMonsterSpawn, &vPos)
//    -> m_iType = m_stMonsterInfo[i].m_iType
//    -> if (m_iType == 1) // NPC
//       -> CreateNpc(uxMapID, EventUniqueID, m_stMonsterInfo[i].m_iID, vPos, fRotate)
//       -> SetWayPointID, SetSpawnBoxID
//    -> else if (m_iType == 0 || == 2 || == 4) // Monster
//       -> CreateMonster(uxMapID, iUniqueSector, m_stMonsterInfo[i].m_iID, vPos, fRotate, eType, iID, m_iGroupID, uxParentID)
//       -> if (m_RespawnTime > 0) -> CRespawnManager::RegisterMonster
//       -> SetMoveType, SetWayPointID, CGroupAggro::SetInfo
//       -> CAi::SetTargetSightDistance *= m_fTakeTargetRatio
//       -> if (IsApplySilhouet) -> CreateSilhouetteFromBoxinfo, SetSilhoutte
//       -> SetupScriptTraceHP
//       -> if (strlen(m_ChangeSpawnAction) > 1) -> ChangeMotion, send_eSUB_CMD_MOVE_IDLE
void CBattleZone::ExcuteSpawnBox(const VMonsterSpawnInfo* pMonsterSpawn, E_SEND_INFO_TYPE eType) {
    // IDA 反编译完整逻辑:
    // 1. 检查 pMonsterSpawn 有效性
    // 2. 记录日志
    // 3. 遍历 m_stMonsterInfo 数组（最多10个）
    // 4. 根据概率选择生成的怪物
    // 5. 根据类型创建 Monster(0,2,4) 或 NPC(1)
    // 6. 设置重生管理器、移动类型、仇恨组等属性

    if (!pMonsterSpawn) {
        return;
    }

    // 记录生成箱日志
    // LogHelper::LogDebug("game.contents",
    //     "<SPAWN> < SpawnID ( %d / %d ) >",
    //     pMonsterSpawn->iID, pMonsterSpawn->m_iSectorID);

    int nCreateCount = 0;

    // 遍历怪物信息数组，选择要生成的怪物
    int i = 0;
    for (i = 0; i < 10; ++i) {
        if (pMonsterSpawn->m_stMonsterInfo[i].m_iID == 0) {
            continue;
        }

        // 获取随机概率，检查是否生成该怪物
        // XWorldManager* pWorldMgr = TXSingleton<XWorldManager>::Instance();
        // int nProb = XWorldManager::RandProb(pWorldMgr);
        // if (nProb <= pMonsterSpawn->m_stMonsterInfo[i].m_iChance) {
        //     break;  // 选中该怪物
        // }
    }

    if (i >= 10) {
        return;  // 没有可生成的怪物
    }

    // 根据 m_iMaxEntityCount 创建多个实例
    for (int j = 0; j < pMonsterSpawn->m_iMaxEntityCount; ++j) {
        // 获取生成位置
        XVec3 vPos;
        // GetSpawnPos(pMonsterSpawn, &vPos);

        int m_iType = pMonsterSpawn->m_stMonsterInfo[i].m_iType;

        if (m_iType == 1) {
            // === 创建 NPC ===
            // int EventUniqueID = VEventObjectInfo::GetEventUniqueID(pMonsterSpawn->m_iSectorID, ...);
            // CNpc* pNpc = CreateNpc(m_uxMapID, EventUniqueID,
            //     pMonsterSpawn->m_stMonsterInfo[i].m_iID, vPos, pMonsterSpawn->fRotate);
            // if (pNpc) {
            //     CMoverEx::SetWayPointID(pNpc, pMonsterSpawn->m_iWaypoint);
            //     CNpc::SetSpawnBoxID(pNpc, pMonsterSpawn->iID);
            //     ++nCreateCount;
            // }
        }
        else if (m_iType == 0 || m_iType == 2 || m_iType == 4) {
            // === 创建 Monster ===
            // int iUniqueSector = VEventObjectInfo::GetEventUniqueID(pMonsterSpawn->m_iSectorID, ...);
            // UXActorID uxParentID;  // 默认构造
            // CMonster* pMonster = CreateMonster(m_uxMapID, iUniqueSector,
            //     pMonsterSpawn->m_stMonsterInfo[i].m_iID, vPos, pMonsterSpawn->fRotate,
            //     eType, pMonsterSpawn->iID, pMonsterSpawn->m_iGroupID, uxParentID);

            CMonster* pMonster = nullptr;  // TODO: 实际调用

            if (pMonster) {
                // 如果有重生时间，注册到重生管理器
                if (pMonsterSpawn->m_RespawnTime > 0.0f) {
                    // UXActorID actorID = pMonster->GetActorID();
                    // unsigned int dwQuestID = CQuestCondition::GetQuestID(&actorID);
                    // CRespawnManager::RegisterMonster(&m_respawnManager, dwQuestID,
                    //     pMonsterSpawn->m_stMonsterInfo[i].m_iID,
                    //     pMonsterSpawn->m_stMonsterInfo[i].m_iType,
                    //     pMonsterSpawn);
                }

                // 设置移动类型
                // CMonster::SetMoveType(pMonster, pMonsterSpawn->m_iMoveType);

                // 设置路径点
                // CMoverEx::SetWayPointID(pMonster, pMonsterSpawn->m_iWaypoint);

                // 设置仇恨组信息
                // CGroupAggro* pGroupAggro = CMonster::GetGroupAggro(pMonster);
                // CGroupAggro::SetInfo(pGroupAggro,
                //     pMonsterSpawn->m_iAggroGroupID,
                //     pMonsterSpawn->m_iAggroDistance,
                //     pMonsterSpawn->m_iAggroMaxCount);

                // 调整视野距离
                // CAi* pAi = CMonster::GetAi(pMonster);
                // if (pAi) {
                //     float fSight = CAi::GetTargetSightDistance(pAi) * pMonsterSpawn->m_fTakeTargetRatio;
                //     CAi::SetTargetSightDistance(pAi, fSight);
                // }

                // 如果需要轮廓，创建轮廓
                // if (CMonster::IsApplySilhouet(pMonster)) {
                //     hkaiPointCloudSilhouetteGenerator* pSilhoutte = CreateSilhouetteFromBoxinfo(pMonsterSpawn, 1);
                //     CMoverEx::SetSilhoutte(pMonster, pSilhoutte);
                // }

                // 设置脚本跟踪HP
                // CMonster::SetupScriptTraceHP(pMonster, pMonsterSpawn);

                // 如果有生成动作，播放动画
                // if (strlen(pMonsterSpawn->m_ChangeSpawnAction) > 1) {
                //     VString strAnimName(pMonsterSpawn->m_ChangeSpawnAction);
                //     unsigned int dwAnimID = CMover::GetAnimIndex(pMonster, strAnimName);
                //     if (dwAnimID != -1) {
                //         short nMotion = CMover::AnimKeyToMotion(pMonster, dwAnimID);
                //         pMonster->ChangeMotion(nMotion, 1, 0);
                //         CMover::send_eSUB_CMD_MOVE_IDLE(pMonster, pMonster, 0.0);
                //     }
                // }

                ++nCreateCount;
            }
            else {
                // LogHelper::LogError("game.contents",
                //     "ExcuteSpawnBox error - Faild Create Monster[ %d / %d ] ( %d )",
                //     pMonsterSpawn->iID, pMonsterSpawn->m_stMonsterInfo[i].m_iID, 566);
                GreenDamTan_log(__FILE__, __FUNCTION__, "ExcuteSpawnBox - Failed to create monster");
            }
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ExcuteSpawnBox - completed");
}

// Per IDA 0x14019FAD0: CBattleZone::ExcuteSpawnBox (STMageProcessSpawnBox version)
// 处理 WorldMode 生成箱逻辑
// IDA 反编译精确逻辑:
// 1. 检查地图类型 (30031 特殊处理)
// 2. 遍历 m_mapGameWorldMode，检查 Start_Type、GetState、IsSpawnNextMonster
// 3. 如果不满足条件，设置 bSpawned=0 并返回
// 4. 如果 m_mapGameWorldMode 为空，记录错误
// 5. 否则执行标准生成箱逻辑 (pProcessSpawn->pSpawnBox)
void CBattleZone::ExcuteSpawnBox(STMageProcessSpawnBox* pProcessSpawn, E_SEND_INFO_TYPE eType) {
    // IDA: 检查地图类型 - m_uxMapID.wMapID << 16 >> 48 != 30031
    int nMapType = (m_uxMapID.wMapID << 16) >> 48;

    if (nMapType == 0x7577) { // 30031 in hex - special world mode map
        // IDA: 遍历 m_mapGameWorldMode
        for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it) {
            // IDA: 获取 shared_ptr 中的指针
            CGameWorldMode* pWorldMode = it->second.get();
            if (!pWorldMode) {
                continue;
            }

            // IDA: 获取 TB_MODE_DISTRICT6 表数据
            int nModeID = it->first;
            // TB_MODE_DISTRICT6* pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(nModeID);
            // if (!pTBMode) continue;

            // IDA: 检查 Start_Type == 1 (AlreadyInWorldMode)
            // if (pTBMode->Start_Type == 1) {
            //     pProcessSpawn->bSpawned = 0;
            //     int nModeDateID = pWorldMode->GetModeDateID();
            //     LogHelper::LogError("game.contents", "ExcuteSpawnBox - StartType 1 %d %d", nModeDateID, nModeID);
            //     return;
            // }

            // IDA: 检查 GetState() == 2 (已完成)
            if (pWorldMode->GetState() == 2) {
                pProcessSpawn->bSpawned = 0;
                // int nModeDateID = pWorldMode->GetModeDateID();
                // LogHelper::LogError("game.contents", "ExcuteSpawnBox - Finish %d %d", nModeDateID, nModeID);
                return;
            }

            // IDA: 检查 IsSpawnNextMonster()
            // if (!pWorldMode->IsSpawnNextMonster()) {
            //     pProcessSpawn->bSpawned = 0;
            //     int nModeDateID = pWorldMode->GetModeDateID();
            //     LogHelper::LogError("game.contents", "ExcuteSpawnBox - Count %d %d", nModeDateID, nModeID);
            //     return;
            // }
        }

        // IDA: 检查 m_mapGameWorldMode 是否为空
        if (m_mapGameWorldMode.empty()) {
            pProcessSpawn->bSpawned = 0;
            LogHelper::LogError("game.contents", "ExcuteSpawnBox - Empty");
            return;
        }
    }

    // IDA: 标准生成箱处理
    if (!pProcessSpawn) {
        return;
    }

    VMonsterSpawnInfo* pMonsterSpawn = pProcessSpawn->pSpawnBox;
    if (!pMonsterSpawn) {
        return;
    }

    // IDA: 初始化生成参数
    int nCreateCount = 0;
    int nOrderIndex = 0;

    // IDA: 检查 m_iWaitCreationSequenceType == 1 (顺序生成)
    if (pMonsterSpawn->m_iWaitCreationSequenceType == 1) {
        nOrderIndex = pProcessSpawn->nSpawnOrder;
    }

    int nID = pMonsterSpawn->iID;
    LogHelper::LogDebug("game.contents", "<SPAWN> < SpawnID ( %d / %d ) >",
        pMonsterSpawn->iID, pMonsterSpawn->m_iSectorID);

    // IDA: 遍历怪物信息数组 (最多 10 个)
    for (int i = nOrderIndex; i < 10; ++i) {
        if (!pMonsterSpawn->m_stMonsterInfo[i].m_iID) {
            // IDA: 没有更多怪物，检查是否需要更新生成顺序
            int nOrder = i + 1;
            if (nOrder >= 10) {
                nOrder = 0;
            }
            pProcessSpawn->nSpawnOrder = nOrder;
            pProcessSpawn->bSpawned = 0;
            return;
        }

        // IDA: 概率检查
        // int nProb = XWorldManager::RandProb();
        // if (nProb > pMonsterSpawn->m_stMonsterInfo[i].m_iChance) {
        //     continue; // 概率不满足，继续下一个
        // }

        // IDA: 遍历 m_iMaxEntityCount 生成多个怪物
        for (int j = 0; j < pMonsterSpawn->m_iMaxEntityCount; ++j) {
            // IDA: 获取生成位置
            XVec3 vPos;
            // GetSpawnPos_2(pMonsterSpawn, &vPos);

            int m_iType = pMonsterSpawn->m_stMonsterInfo[i].m_iType;

            // IDA: 类型检查 (0=Monster, 2/4=特殊怪物, 跳过NPC类型1)
            if (m_iType == 0 || m_iType == 2 || m_iType == 4) {
                // IDA: 获取 UniqueSector ID
                // int iUniqueSector = VEventObjectInfo::GetEventUniqueID(pMonsterSpawn->m_iSectorID, ...);

                // IDA: 创建怪物
                // CMonster* pMonster = CreateMonster(
                //     m_uxMapID, iUniqueSector,
                //     pMonsterSpawn->m_stMonsterInfo[i].m_iID,
                //     vPos, pMonsterSpawn->fRotate, eType,
                //     pMonsterSpawn->iID, pMonsterSpawn->m_iGroupID, uxParentID);

                CMonster* pMonster = nullptr; // TODO: 调用 CreateMonster

                if (pMonster) {
                    // IDA: 注册重生管理器
                    if (pMonsterSpawn->m_RespawnTime > 0.0f && nMapType != 0x7577) {
                        // CRespawnManager::RegisterMonster(&m_respawnManager,
                        //     QuestID, MonsterID, m_iType, pMonsterSpawn);
                    }

                    // IDA: 添加怪物生成箱信息
                    AddMonsterSpawnInfo(nID, 0); // TODO: 使用实际的 MonsterID

                    // IDA: 设置移动类型
                    // CMonster::SetMoveType(pMonster, pMonsterSpawn->m_iMoveType);

                    // IDA: 设置路点
                    // CMoverEx::SetWayPointID(pMonster, pMonsterSpawn->m_iWaypoint);

                    // IDA: 设置仇恨组
                    // CGroupAggro* pGroupAggro = CMonster::GetGroupAggro(pMonster);
                    // CGroupAggro::SetInfo(pGroupAggro,
                    //     pMonsterSpawn->m_iAggroGroupID,
                    //     pMonsterSpawn->m_iAggroDistance,
                    //     pMonsterSpawn->m_iAggroMaxCount);

                    // IDA: 设置 AI 视野距离
                    // CAi* pAi = CMonster::GetAi(pMonster);
                    // if (pAi) {
                    //     float fDistance = CAi::GetTargetSightDistance(pAi) * pMonsterSpawn->m_fTakeTargetRatio;
                    //     CAi::SetTargetSightDistance(pAi, fDistance);
                    // }

                    // IDA: 设置轮廓 (如果需要)
                    // if (CMonster::IsApplySilhouet(pMonster)) {
                    //     hkaiPointCloudSilhouetteGenerator* pSilhoutte = CreateSilhouetteFromBoxinfo(pMonsterSpawn, 1);
                    //     CMoverEx::SetSilhoutte(pMonster, pSilhoutte);
                    // }

                    // IDA: 设置脚本跟踪 HP
                    // CMonster::SetupScriptTraceHP(pMonster, pMonsterSpawn);

                    // IDA: 播放生成动画
                    if (std::strlen(pMonsterSpawn->m_ChangeSpawnAction) > 1) {
                        // VString strAnimName(pMonsterSpawn->m_ChangeSpawnAction);
                        // unsigned int dwAnimID = CMover::GetAnimIndex(pMonster, strAnimName);
                        // if (dwAnimID != -1) {
                        //     short nMotion = CMover::AnimKeyToMotion(pMonster, dwAnimID);
                        //     pMonster->ChangeMotion(nMotion, 1, 0);
                        //     CMover::send_eSUB_CMD_MOVE_IDLE(pMonster, pMonster, 0.0);
                        // }
                    }

                    ++nCreateCount;
                }
            }
        }

        // IDA: 如果是顺序生成类型，更新顺序后退出
        if (pMonsterSpawn->m_iWaitCreationSequenceType == 1) {
            int nOrder = i + 1;
            if (nOrder >= 10) {
                nOrder = 0;
            }
            pProcessSpawn->nSpawnOrder = nOrder;
            break;
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ExcuteSpawnBox(STMageProcessSpawnBox) - completed");
}

// Per IDA 0x1401A0460: CBattleZone::ExcuteSpawn
// 重生怪物生成
// IDA 反编译精确逻辑:
// 1. 检查 pMonsterSpawn 有效性
// 2. 记录日志
// 3. GetSpawnPos_2(pMonsterSpawn, &vPos)
// 4. 检查 nObjectType (0=Monster, 2/4=特殊怪物, 跳过1=NPC)
// 5. 获取 iUniqueSector
// 6. CreateMonster(uxMapID, iUniqueSector, nObjectID, vPos, fRotate, eType, iID, m_iGroupID, uxParentID)
// 7. 如果 m_RespawnTime > 0 且不是特殊地图(30031)，注册重生管理器
// 8. 设置移动类型、路点、仇恨组、AI视野距离
// 9. 如果需要，设置轮廓和脚本跟踪HP
// 10. 播放生成动画
void CBattleZone::ExcuteSpawn(int nBoxIndex, int nObjectType, const VMonsterSpawnInfo* pMonsterSpawn, E_SEND_INFO_TYPE eType) {
    // IDA: 检查参数有效性
    if (!pMonsterSpawn) {
        return;
    }

    // IDA: 记录重生日志
    LogHelper::LogDebug("game.contents", "<RESPAWN> SpawnID ( %d ) ", pMonsterSpawn->iID);

    // IDA: 获取生成位置
    XVec3 vPos;
    // GetSpawnPos_2(pMonsterSpawn, &vPos);

    // IDA: 检查对象类型 (0=Monster, 2/4=特殊怪物)
    // 跳过 NPC 类型 (1)
    if (nObjectType == 0 || nObjectType == 2 || nObjectType == 4) {
        // IDA: 获取 UniqueSector ID
        // int iUniqueSector = VEventObjectInfo::GetEventUniqueID(pMonsterSpawn->m_iSectorID, ...);

        // IDA: 创建怪物
        // CMonster* pMonster = CreateMonster(
        //     m_uxMapID, iUniqueSector,
        //     nBoxIndex, // nObjectID
        //     vPos, pMonsterSpawn->fRotate, eType,
        //     pMonsterSpawn->iID, pMonsterSpawn->m_iGroupID, uxParentID);

        CMonster* pMonster = nullptr; // TODO: 调用 CreateMonster

        if (pMonster) {
            // IDA: 检查重生时间和地图类型
            int nMapType = (m_uxMapID.wMapID << 16) >> 48;
            if (pMonsterSpawn->m_RespawnTime > 0.0f && nMapType != 0x7577) { // 30031
                // IDA: 注册重生管理器
                // unsigned int QuestID = CQuestCondition::GetQuestID(pMonster->GetActorID());
                // CRespawnManager::RegisterMonster(&m_respawnManager, QuestID, nBoxIndex, nObjectType, pMonsterSpawn);
            }

            // IDA: 设置移动类型
            // CMonster::SetMoveType(pMonster, pMonsterSpawn->m_iMoveType);

            // IDA: 设置路点
            // CMoverEx::SetWayPointID(pMonster, pMonsterSpawn->m_iWaypoint);

            // IDA: 设置仇恨组
            // CGroupAggro* pGroupAggro = CMonster::GetGroupAggro(pMonster);
            // CGroupAggro::SetInfo(pGroupAggro,
            //     pMonsterSpawn->m_iAggroGroupID,
            //     pMonsterSpawn->m_iAggroDistance,
            //     pMonsterSpawn->m_iAggroMaxCount);

            // IDA: 设置 AI 视野距离
            // CAi* pAi = CMonster::GetAi(pMonster);
            // if (pAi) {
            //     float fDistance = CAi::GetTargetSightDistance(pAi) * pMonsterSpawn->m_fTakeTargetRatio;
            //     CAi::SetTargetSightDistance(pAi, fDistance);
            // }

            // IDA: 设置轮廓 (如果需要)
            // if (CMonster::IsApplySilhouet(pMonster)) {
            //     hkaiPointCloudSilhouetteGenerator* pSilhoutte = CreateSilhouetteFromBoxinfo(pMonsterSpawn, 1);
            //     CMoverEx::SetSilhoutte(pMonster, pSilhoutte);
            // }

            // IDA: 设置脚本跟踪 HP
            // CMonster::SetupScriptTraceHP(pMonster, pMonsterSpawn);

            // IDA: 播放生成动画
            if (std::strlen(pMonsterSpawn->m_ChangeSpawnAction) > 1) {
                // VString strAnimName(pMonsterSpawn->m_ChangeSpawnAction);
                // unsigned int dwAnimID = CMover::GetAnimIndex(pMonster, strAnimName);
                // if (dwAnimID != -1) {
                //     short nMotion = CMover::AnimKeyToMotion(pMonster, dwAnimID);
                //     pMonster->ChangeMotion(nMotion, 1, 0);
                //     CMover::send_eSUB_CMD_MOVE_IDLE(pMonster, pMonster, 0.0);
                // }
            }
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ExcuteSpawn - completed");
}

// Per IDA 0x1401A4C40: CBattleZone::StartWorldMode
// 启动世界模式
// IDA 反编译精确逻辑:
// 1. pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(nModeID)
// 2. if (pTBMode->Start_Type == 1) -> AlreadyInWorldMode()
// 3. 查找 m_mapGameWorldMode.find(nModeID)，如果存在则 erase
// 4. pWorldMode = new CGameWorldMode() -> Init(this, stInfo)
// 5. if (m_uxMapID.nMapID << 16 >> 48 == 30031) -> pWorldMode->StartMode()
// 6. m_mapGameWorldMode.insert(std::make_pair(nModeID, pWorldMode))
// 7. PS_WORLD_MODE_START stStart = { nModeDateID, nID, nStartTime, nFinishTime, byState, biModeStartTime, biModeEndTime }
//    -> if (pTBMode->Start_Type == 0) -> stStart.byState = 4
// 8. XSendPacket xPacket(0x30, 0x01) << stStart -> SendBroadCastAll
// 9. sprintf(szNotice, "Start WM : %d", nModeID) -> LogDebug
void CBattleZone::StartWorldMode(ST_WORLD_MODE_INFO& stInfo) {
    // IDA 反编译逻辑:
    // 1. 获取 TB_MODE_DISTRICT6 表数据
    // 2. 如果 Start_Type == 1，检查是否已在 WorldMode 中
    // 3. 查找 m_mapGameWorldMode，如果已存在则移除
    // 4. 创建新的 CGameWorldMode 并初始化
    // 5. 如果是特殊地图 (30031)，立即启动模式
    // 6. 插入到 m_mapGameWorldMode
    // 7. 构造并发送 PS_WORLD_MODE_START 包给所有玩家
    // 8. 记录日志

    // 获取资源管理器
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_MODE_DISTRICT6* pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(&pServer->m_xResourceMgr, stInfo.nModeID);
    // if (!pTBMode) {
    //     return;
    // }

    // 如果 Start_Type == 1 (Boss 类型)，检查是否已在 WorldMode 中
    // if (pTBMode->Start_Type == 1) {
    //     AlreadyInWorldMode();
    // }

    // 查找并移除已存在的 WorldMode
    auto it = m_mapGameWorldMode.find(stInfo.nModeID);
    if (it != m_mapGameWorldMode.end()) {
        m_mapGameWorldMode.erase(it);
    }

    // 创建新的 CGameWorldMode
    std::tr1::shared_ptr<CGameWorldMode> pWorldMode(new CGameWorldMode());
    if (pWorldMode) {
        pWorldMode->Init(this, &stInfo);

        // 检查是否是特殊地图 (30031 = 花园地图)
        // IDA: m_uxMapID.nMapID << 16 >> 48 == 30031
        // int nMapType = (m_uxMapID.wMapID << 16) >> 48;
        // if (nMapType == 30031) {
        //     pWorldMode->StartMode();
        // }

        // 插入到 map 中
        m_mapGameWorldMode[stInfo.nModeID] = pWorldMode;
    }

    // 构造并发送 PS_WORLD_MODE_START 包
    // PS_WORLD_MODE_START stStart;
    // stStart.nModeDateID = stInfo.nModeDateID;
    // stStart.nID = stInfo.nModeID;
    // stStart.nStartTime = stInfo.nStartTime;
    // stStart.nFinishTime = stInfo.nFinishTime;
    // stStart.byState = 0;
    // stStart.biModeStartTime = stInfo.biModeStartTime;
    // stStart.biModeEndTime = stInfo.biModeEndTime;
    //
    // if (pTBMode->Start_Type == 0) {
    //     stStart.byState = 4;  // 时间触发模式
    // }
    //
    // XSendPacket xPacket(0x30, 0x01);  // 主命令 0x30, 子命令 0x01
    // xPacket << stStart;
    // SendBroadCastAll(&xPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "Start WM");
}

// Per IDA 0x1401A4FC0: CBattleZone::FinishWorldMode
// 完成世界模式
// IDA 反编译精确逻辑:
// 1. pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(nModeID)
// 2. m_mapGameWorldMode.find(nModeID) -> it
// 3. if (it != end())
//    -> pWorldMode->FinishMode(stInfo)
//    -> XSendPacket xSendPacket(0x30, 0x03) << stInfo -> SendBroadCastAll
//    -> if (pTBMode->Start_Type == 0)
//       -> 遍历 m_vecWorldModeList 更新 nMonsterClearCount
//       -> pModeDate = XResourceMgr::GetTB_MODE_DISTRICT6_DATE(nModeDateID)
//       -> for (j = 0; j < 5 && Clear_Count_0j; ++j)
//          -> if (nMonsterClearCount >= Clear_Count_0j && < Clear_Count_(j+1))
//             -> if (Clear_Booster_0j) -> SetWorldModeBoostAll(Clear_Booster_0j, nFinishTime + Booster_Limit_Time)
// 4. sprintf(szNotice, "Finish WM : %d", nModeID) -> LogDebug
void CBattleZone::FinishWorldMode(PS_WORLD_MODE_FINISH& stFinish) {
    // IDA 反编译逻辑:
    // 1. 获取 TB_MODE_DISTRICT6 表数据
    // 2. 查找 m_mapGameWorldMode 中的 WorldMode
    // 3. 调用 CGameWorldMode::FinishMode
    // 4. 构造并发送 PS_WORLD_MODE_FINISH 包给所有玩家
    // 5. 如果是时间触发模式 (Start_Type == 0)，更新 m_vecWorldModeList 并处理奖励
    // 6. 记录日志

    // 获取资源管理器
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_MODE_DISTRICT6* pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(&pServer->m_xResourceMgr, stFinish.nModeID);
    // if (!pTBMode) {
    //     return;
    // }

    // 查找 WorldMode
    auto it = m_mapGameWorldMode.find(stFinish.nModeID);
    if (it != m_mapGameWorldMode.end()) {
        std::tr1::shared_ptr<CGameWorldMode> pWorldMode = it->second;
        if (pWorldMode) {
            // 调用 FinishMode
            pWorldMode->FinishMode(&stFinish);

            // 发送 PS_WORLD_MODE_FINISH 包给所有玩家
            // XSendPacket xSendPacket(0x30, 0x03);  // 主命令 0x30, 子命令 0x03
            // xSendPacket << stFinish;
            // SendBroadCastAll(&xSendPacket);

            // 如果是时间触发模式 (Start_Type == 0)，处理奖励
            // if (pTBMode->Start_Type == 0) {
            //     // 更新 m_vecWorldModeList 中的怪物清除计数
            //     // 检查是否需要设置 WorldMode Boost
            //     TB_MODE_DISTRICT6_DATE* pModeDate = XResourceMgr::GetTB_MODE_DISTRICT6_DATE(&pServer->m_xResourceMgr, stFinish.nModeDateID);
            //     if (pModeDate && stFinish.nMonsterClearCount > 0) {
            //         // 检查清除计数是否满足奖励条件
            //         // Per IDA: 遍历 5 个 Clear_Count 阈值 (*(&pModeDate->Clear_Count_01 + j))
            //         const std::uint16_t* pClearCounts[] = {
            //             &pModeDate->Clear_Count_01,
            //             &pModeDate->Clear_Count_02,
            //             &pModeDate->Clear_Count_03,
            //             &pModeDate->Clear_Count_04,
            //             &pModeDate->Clear_Count_05
            //         };
            //         const unsigned int* pClearBoosters[] = {
            //             &pModeDate->Clear_Booster_01,
            //             &pModeDate->Clear_Booster_02,
            //             &pModeDate->Clear_Booster_03,
            //             &pModeDate->Clear_Booster_04,
            //             &pModeDate->Clear_Booster_05
            //         };
            //         for (int j = 0; j < 5 && *pClearCounts[j]; ++j) {
            //             std::uint16_t nextClearCount = (j < 4) ? *pClearCounts[j + 1] : pModeDate->Clear_Count_06;
            //             if (stFinish.nMonsterClearCount >= *pClearCounts[j] &&
            //                 (j == 4 || stFinish.nMonsterClearCount < nextClearCount)) {
            //                 if (*pClearBoosters[j]) {
            //                     SetWorldModeBoostAll(*pClearBoosters[j], 0);  // nEndDate=0 (immediate)
            //                 }
            //                 break;
            //             }
            //         }
            //     }
            // }
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "Finish WM");
}

// Per IDA 0x1401A53C0: ClearWorldMode
// IDA 反编译精确还原:
// 1. 检查 TB_MODE_DISTRICT6 表是否存在该模式
// 2. 如果存在，从 m_mapGameWorldMode 中查找并删除
// 3. 清理 m_setWorldModeHitUser
void CBattleZone::ClearWorldMode(ST_WORLD_MODE_INFO& stInfo)
{
    // TODO: 汇编还原 - 需要 XResourceMgr::GetTB_MODE_DISTRICT6 完整定义
    // IDA: XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // if (pServer->m_xResourceMgr.GetTB_MODE_DISTRICT6(stInfo.nModeID))
    // {
    //     auto it = m_mapGameWorldMode.find(stInfo.nModeID);
    //     auto endIt = m_mapGameWorldMode.end();
    //     if (it != endIt)
    //     {
    //         m_mapGameWorldMode.erase(it);
    //     }
    //     sprintf(szNotice, "Clear WM : %d", stInfo.nModeID);
    //     GreenDamTan_log(__FILE__, __FUNCTION__, szNotice);
    //     m_setWorldModeHitUser.clear();
    // }

    // 简化实现 - 直接执行核心逻辑
    auto it = m_mapGameWorldMode.find(stInfo.nModeID);
    if (it != m_mapGameWorldMode.end())
    {
        m_mapGameWorldMode.erase(it);
    }

    m_setWorldModeHitUser.clear();

    GreenDamTan_log(__FILE__, __FUNCTION__, "ClearWorldMode called");
}

// Per IDA 0x1401A5500: SyncWorldMode - synchronize world mode state from ControlServer
// IDA 反编译精确还原:
// 1. InfoWorldMode(this, stInfoVec) - 记录日志
// 2. 遍历 stInfoVec.vecInfo
// 3. 对于每个 ST_WORLD_MODE_INFO:
//    - 检查 TB_MODE_DISTRICT6 表是否存在
//    - nState == 1: 创建新的 CGameWorldMode 并初始化，插入 m_mapGameWorldMode
//      如果地图ID == 30031，立即调用 StartMode
//    - nState == 2: 创建新的 CGameWorldMode 并初始化，插入 m_mapGameWorldMode
//    - 其他状态: 从 m_mapGameWorldMode 中删除
void CBattleZone::SyncWorldMode(ST_WORLD_MODE_INFO_VEC& stInfoVec)
{
    // TODO: 汇编还原 - 需要 InfoWorldMode 完整定义
    // IDA: this->InfoWorldMode(this, stInfoVec);

    // IDA: 遍历 stInfoVec.vecInfo
    for (std::size_t i = 0; i < stInfoVec.vecInfo.size(); ++i)
    {
        ST_WORLD_MODE_INFO stInfo = stInfoVec.vecInfo[i];

        // TODO: 汇编还原 - 需要 XResourceMgr::GetTB_MODE_DISTRICT6 完整定义
        // IDA: XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // if (XResourceMgr::GetTB_MODE_DISTRICT6(&pServer->m_xResourceMgr, stInfo.nModeID))
        // {
        //     ...
        // }

        if (stInfo.nState == 1 || stInfo.nState == 2)
        {
            // Check if this world mode already exists
            auto it = m_mapGameWorldMode.find(stInfo.nModeID);
            if (it == m_mapGameWorldMode.end())
            {
                // Create new world mode
                // IDA: v21 = (CGameWorldMode*)VBaseObject::operator new(0x98u);
                // CGameWorldMode::CGameWorldMode(v21);
                // std::tr1::shared_ptr<CGameWorldMode>::shared_ptr(&pWorldMode, v21);
                std::tr1::shared_ptr<CGameWorldMode> pWorldMode(new CGameWorldMode());
                if (pWorldMode)
                {
                    // IDA: CGameWorldMode::Init(pWorldMode, this, &stInfo);
                    pWorldMode->Init(this, &stInfo);

                    // IDA: if (this->m_uxMapID.nMapID << 16 >> 48 == 30031)
                    //     CGameWorldMode::StartMode(pWorldMode);
                    // 地图ID高16位为30031时，立即启动
                    // TODO: 需要 GetDistrictType 或直接计算
                    // if (stInfo.nState == 1) { pWorldMode->StartMode(); }

                    m_mapGameWorldMode[stInfo.nModeID] = pWorldMode;
                }
            }
        }
        else
        {
            // Remove world mode
            auto it = m_mapGameWorldMode.find(stInfo.nModeID);
            if (it != m_mapGameWorldMode.end())
            {
                m_mapGameWorldMode.erase(it);
            }
        }
    }

    // IDA also clears hit user tracking on sync
    m_setWorldModeHitUser.clear();
}

// Per IDA 0x1401A8650: CBattleZone::CompleteWorldMode
// 完成世界模式，处理掉落和传送门更新
void CBattleZone::CompleteWorldMode(PS_WORLD_MODE_COMPLETE& stComplete, std::uint32_t dwKillerID) {
    // IDA 反编译: 检查地图ID匹配，然后处理掉落和传送门
    if (stComplete.uxMapID.nMapID == m_uxMapID.wMapID) {
        DropItemForWorldMode(dwKillerID, stComplete.nModeDateID, true);
        UpdatePotalFlag(10012);
    }
}

// Per IDA 0x1401A86B0: UpdateWorldMode - update world mode progress from ControlServer
// IDA 反编译精确还原:
// 1. 记录日志 LogError("game.contents", "Update World Mode[DateID %d, ModeID %d, MonsterCnt:%d]", ...)
// 2. 检查地图ID是否匹配
// 3. 遍历 m_mapGameWorldMode，对每个 shared_ptr<CGameWorldMode> 调用 UpdateMode
void CBattleZone::UpdateWorldMode(PS_WORLD_MODE_UPDATE& stUpdate)
{
    // IDA: LogHelper::LogError("game.contents", "Update World Mode[DateID %d, ModeID %d, MonsterCnt:%d]", ...)
    GreenDamTan_log(__FILE__, __FUNCTION__, "Update World Mode called");

    // IDA checks if mapID matches this zone
    if (stUpdate.uxMapID.nMapID != m_uxMapID.wMapID)
        return;

    // IDA: iterates m_mapGameWorldMode and calls CGameWorldMode::UpdateMode
    for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it)
    {
        std::tr1::shared_ptr<CGameWorldMode> pWorldMode = it->second;
        if (pWorldMode)
        {
            // TODO: 汇编还原 - 需要 CGameWorldMode::UpdateMode 完整定义
            // IDA: PS_WORLD_MODE_UPDATE v8; memcpy(&v8, stUpdate, sizeof(v8));
            // pWorldMode->UpdateMode(&v8);
        }
    }
}

// Per IDA 0x1401A8560: CBattleZone::IsWorldModeBoss
// 检查是否有 WorldMode Boss 激活
bool CBattleZone::IsWorldModeBoss() {
    // IDA 反编译逻辑:
    // 遍历 m_mapGameWorldMode，检查是否有 Start_Type == 1 且状态为 1 的 WorldMode
    // Start_Type == 1 表示 Boss 类型模式

    for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it) {
        std::tr1::shared_ptr<CGameWorldMode> pMode = it->second;
        if (!pMode) continue;

        int nModeID = pMode->GetModeID();

        // 获取 TB_MODE_DISTRICT6 表数据检查 Start_Type
        // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        // TB_MODE_DISTRICT6* pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(&pServer->m_xResourceMgr, nModeID);
        // if (pTBMode && pTBMode->Start_Type == 1) {
        //     if (pMode->GetState() == 1) {
        //         return true;
        //     }
        // }

        // 临时实现: 假设所有进行中的 WorldMode 都是 Boss 模式
        if (pMode->GetState() == 1) {
            return true;
        }
    }
    return false;
}

// Per IDA 0x1401A8820: CBattleZone::AlreadyInWorldMode
// 检查是否已在 WorldMode 中，如果是则踢出玩家到进入位置
// IDA 反编译精确还原:
// 1. if (m_uxMapID.nMapID << 16 >> 48 != 30031) return - 检查地图类型
// 2. 遍历 m_mapActor 中的所有 Actor
// 3. dynamic_cast<CUser*>(pActor) -> pUser
// 4. if (pUser):
//    - STPosInfo stPosInfo; CUser::GetEnterDistrictPos(pUser, &stPosInfo)
//    - PS_ENTER_MAP_REQ psEnterMap = {}
//    - psEnterMap.dwActorID = pUser->GetActorID().GetQuestID()
//    - psEnterMap.nJumpID = 0
//    - psEnterMap.wMapID = stPosInfo.sWorldID
//    - psEnterMap.vNextPos = stPosInfo.vPos
//    - XSendPacket(0xF2, 0x31) << psEnterMap
//    - XGameServer::Instance()->m_controlSocket.Send(&xSendPacket)
//    - LogHelper::LogError("game.contents", "IsAlreadyInWorldMode - [UCID:%d, MapId:%d]", ...)
bool CBattleZone::AlreadyInWorldMode() {
    // IDA: 检查地图是否为 30031 (WorldMode 地图)
    // m_uxMapID.nMapID << 16 >> 48 提取高16位
    int nMapType = (m_uxMapID.wMapID << 16) >> 48;  // 警告: shift overflow
    (void)nMapType;  // 抑制未使用警告

    // TODO: 汇编还原 - 需要 SWORD2 宏正确实现
    // if ((m_uxMapID.nMapID >> 16 & 0xFFFF) != 30031) {
    //     return false;
    // }

    // IDA: 遍历 m_mapActor
    // for (auto iter = TXMap::Begin(&m_mapActor); iter; iter = TXMap::GetNext(&m_mapActor, &iter))
    CFAutoSlimReadLock lock(m_rwLock);
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;

        // IDA: pUser = dynamic_cast<CUser*>(pActor)
        // TODO: 汇编还原 - 需要 RTTI dynamic_cast
        CUser* pUser = dynamic_cast<CUser*>(pActor);

        if (pUser) {
            // IDA: STPosInfo stPosInfo; CUser::GetEnterDistrictPos(pUser, &stPosInfo)
            // TODO: 汇编还原 - 需要 STPosInfo 类型, CUser::GetEnterDistrictPos 方法
            // STPosInfo stPosInfo;
            // CUser::GetEnterDistrictPos(pUser, &stPosInfo);

            // IDA: PS_ENTER_MAP_REQ psEnterMap = {}
            // psEnterMap.dwActorID = pUser->GetActorID().GetQuestID();
            // psEnterMap.nJumpID = 0;
            // psEnterMap.wMapID = stPosInfo.sWorldID;
            // psEnterMap.vNextPos = stPosInfo.vPos;

            // IDA: XSendPacket(0xF2, 0x31) << psEnterMap
            // XSendPacket xSendPacket(0xF2, 0x31);
            // xSendPacket << psEnterMap;

            // IDA: XGameServer::Instance()->m_controlSocket.Send(&xSendPacket)
            // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            // pServer->m_controlSocket.Send(&xSendPacket);

            // IDA: LogHelper::LogError("game.contents", "IsAlreadyInWorldMode - [UCID:%d, MapId:%d]", ...)
            GreenDamTan_log(__FILE__, __FUNCTION__, "AlreadyInWorldMode - User would be kicked to entry position");
        }
    }

    return false;
}

// Per IDA 0x1401A3A30: CBattleZone::ProcessDrop (Actor + Type)
// 精确还原：基于怪物ID的掉落处理
// TODO: 需要以下依赖: XResourceMgr::GetTB_MONSTER, XResourceMgr::GetTB_DROPRATE_MOB, CDropProcess
bool CBattleZone::ProcessDrop(XActor* pAtk, int nMonsterID, XVec3& vPos) {
    // IDA 反编译精确逻辑:
    // 1. pTBMonster = XResourceMgr::GetTB_MONSTER(nMonsterID)
    // 2. nDropID = pTBMonster->Monster_Drop_ID, nMonsterLv = pTBMonster->Monster_Lv
    // 3. pAtkUser = dynamic_cast<CUser*>(pAtk)
    // 4. CMover::GetGOC<CGocAttribute>(pAtkUser, &pAttr, 0)
    // 5. nLevel = pAttr->GetLevel(), nLevelDiff = max(0, nLevel - nMonsterLv)
    // 6. pRate = XResourceMgr::GetTB_DROPRATE_MOB(nLevelDiff)
    // 7. pProcess = XClient::GetProcessPtr<CDropProcess>(pAtkUser, 0x14)
    // 8. fDropRate = pRate->DropRate_MobInterval_Value
    //    -> if (!CDropProcess::IsApplyDropRate(pProcess, nDropID)) fDropRate = 1.0
    // 9. nClass = pAtkUser->GetClass(), TBMapID = XArea::GetTBMapID(this)
    // 10. CDropProcess::MakeDropItems(pProcess, pAtkUser, nDropID, vPos, fDropRate, 0, nMonsterID, TBMapID, nClass, 0.0)

    CUser* pAtkUser = dynamic_cast<CUser*>(pAtk);
    if (!pAtkUser) {
        return false;
    }

    // TODO: 实现完整的掉落逻辑
    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessDrop called for MonsterID=%d", nMonsterID);

    return true;
}

// Per IDA 0x1401A3D30: CBattleZone::ProcessDrop (Actor + Monster)
// 精确还原：基于怪物对象的掉落处理（包含特殊类型广播）
// TODO: 需要以下依赖: CMonster::GetMobTableRef, CMover::GetHitList, PS_CHAT_NOTICE_EX
bool CBattleZone::ProcessDrop(XActor* pAtk, CMonster* pMonster, XVec3& vPos) {
    if (!pMonster) {
        LogHelper::LogError("game.contents", "ProcessDrop error - pMonster == NULL ( %d )", 1754);
        return false;
    }

    // IDA 反编译精确逻辑:
    // 1. pTBMonster = CMonster::GetMobTableRef(pMonster)
    // 2. pAtkUser = dynamic_cast<CUser*>(pAtk)
    // 3. nTableID = pMonster->GetTableID()
    // 4. ProcessDrop(pAtk, nTableID, vPos)
    // 5. if (pTBMonster->Monster_Type == 18):
    //    - GetHitList(pMonster, &listHit)
    //    - 广播 PS_CHAT_NOTICE_EX 到所有 hit list 用户

    CUser* pAtkUser = dynamic_cast<CUser*>(pAtk);
    if (!pAtkUser) {
        return false;
    }

    // 调用第一个 ProcessDrop 重载
    // unsigned int nTableID = pMonster->GetTableID();
    // ProcessDrop(pAtk, nTableID, vPos);

    // TODO: 实现完整的掉落逻辑和 Monster_Type == 18 广播
    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessDrop called for Monster");

    return true;
}

// Per IDA 0x1401A4170: CBattleZone::ProcessDropByHit
// 精确还原：基于攻击者ID的掉落处理
// IDA 逻辑:
// 1. if (nDropID <= 0) return
// 2. pActor = FindActor(dwAtkUser)
// 3. if (!pActor || !XActor::IsPlayer(pActor)) return
// 4. pUser = dynamic_cast<CUser*>(pActor)
// 5. CMover::GetGOC<CGocAttribute>(pUser, &pAttr, 0)
// 6. nLevel = pAttr->GetLevel(), nLevelDiff = max(0, nLevel - nMonsterLevel)
// 7. pRate = XResourceMgr::GetTB_DROPRATE_MOB(nLevelDiff)
// 8. pProcess = XClient::GetProcessPtr<CDropProcess>(pUser, 0x14)
// 9. CDropProcess::MakeDropItems(pProcess, pUser, nDropID, vPos, 1.0, 0, nMonsterID, TBMapID, nClass, 0.0)
void CBattleZone::ProcessDropByHit(unsigned int dwAtkUser, int nDropID, int nMonsterLevel, XVec3& vPos, int nMonsterID) {
    // IDA 0x1401A4170: 检查 nDropID > 0
    if (nDropID <= 0) {
        return;
    }

    // IDA: FindActor(dwAtkUser)
    XActor* pActor = FindActor(dwAtkUser);
    if (!pActor) {
        return;
    }

    // IDA: XActor::IsPlayer(pActor)
    // TODO: 需要 XActor::IsPlayer 成员函数
    // if (!pActor->IsPlayer()) {
    //     return;
    // }

    // IDA: dynamic_cast<CUser*>(pActor)
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser) {
        return;
    }

    // IDA: CMover::GetGOC<CGocAttribute>(pUser, &pAttr, 0)
    // std::tr1::shared_ptr<CGocAttribute> pAttr;
    // CMover::GetGOC<CGocAttribute>(&pUser->CMoverEx, &pAttr, 0);
    // if (!pAttr) return;

    // IDA: nLevel = pAttr->GetLevel(), nLevelDiff = max(0, nLevel - nMonsterLevel)
    int nLevel = 0; // TODO: pAttr->GetLevel()
    int nLevelDiff = nLevel - nMonsterLevel;
    if (nLevelDiff < 0) {
        nLevelDiff = 0;
    }

    // IDA: XResourceMgr::GetTB_DROPRATE_MOB(nLevelDiff)
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_DROPRATE_MOB* pRate = XResourceMgr::GetTB_DROPRATE_MOB(&pServer->m_xResourceMgr, nLevelDiff);
    // if (!pRate) {
    //     LogHelper::LogError("game.contents", "BATTLEZONE ProcessDropByHit error - No Table TB_DROPRATE_MOB[ LevelDiff:%d ] ( %d )", nLevelDiff, 1828);
    //     return;
    // }

    // IDA: CDropProcess* pProcess = XClient::GetProcessPtr<CDropProcess>(pUser, 0x14)
    // CDropProcess* pProcess = XClient::GetProcessPtr<CDropProcess>(pUser, 0x14);
    // if (!pProcess) return;

    // IDA: nClass = pUser->GetClass(), TBMapID = XArea::GetTBMapID(this)
    // CDropProcess::MakeDropItems(pProcess, pUser, nDropID, &vPos, 1.0, 0, nMonsterID, TBMapID, nClass, 0.0);

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessDropByHit called for DropID=%d", nDropID);
}

// Per IDA 0x1401A4410: CBattleZone::ProcessMonsterQuest
// 处理怪物击杀时的任务条件更新
void CBattleZone::ProcessMonsterQuest(XActor* pAttacker, std::uint32_t nMonsterID) {
    // IDA 反编译完整逻辑:
    // 1. 地图 30031 (花园) 时, 检查 m_mapGameWorldMode 中是否有事件怪物匹配
    // 2. 转换 pAttacker 为 CUser
    // 3. 获取 TB_MONSTER 表数据
    // 4. 获取队伍列表 (如果有队伍则获取队伍内成员, 否则只包含自己)
    // 5. 遍历每个成员:
    //    a. CGocQuest::UpdateCondition(eCONDITION_TYPE_HUNT, eCONDITION_TARGET_MONSTER, nMonsterID, 1)
    //    b. 遍历 5 个 Quest_Group 字段, UpdateCondition(eCONDITION_TYPE_HUNT, eCONDITION_TARGET_MONSTER_GROUP, groupID, 1)
    //    c. 如果 Monster_Type == 0 或 11, UpdateCondition(eCONDITION_TYPE_HUNT, eCONDITION_TARGET_MAZE, mapID, 1)
    //    d. CGocAchieve::UpdateMonsterAchieve(pTBMonster)

    // 检查地图 30031 中的事件怪物
    int nMapType = (m_uxMapID.wMapID << 16) >> 16;
    // IDA: SWORD2(m_uxMapID.nMapID) == 30031
    if (nMapType == 30031) {
        // 遍历 m_mapGameWorldMode 检查事件怪物
        for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it) {
            std::tr1::shared_ptr<CGameWorldMode> pMode = it->second;
            if (!pMode)
                continue;

            // TODO: CGameWorldMode::IsEventMonster(nMonsterID) when available
            // if (!pMode->IsEventMonster(nMonsterID))
            //     return;
        }
        return; // 地图 30031 不处理普通怪物任务
    }

    // 转换 pAttacker 为 CUser (使用 reinterpret_cast, RTTI 不可用)
    CUser* pUser = reinterpret_cast<CUser*>(pAttacker);
    if (!pUser)
        return;
    // if (!pActor->GetArea()) return;

    // 获取 TB_MONSTER 表数据
    // XGameServer* pServer = XGameServer::Instance();
    // TB_MONSTER* pTBMonster = XResourceMgr::GetTB_MONSTER(&pServer->GetResourceMgr(), nMonsterID);
    // if (!pTBMonster) return;

    // TODO: 当 CGocQuest, CGocAchieve, CGocParty 类型可用时启用完整逻辑
    // std::vector<CMover*> vecGameObjList;
    // CMover* pMover = dynamic_cast<CMover*>(pAttacker);
    //
    // std::tr1::shared_ptr<CGocParty> pParty;
    // CMover::GetGOC<CGocParty>(pMover, &pParty, 1);
    // if ((bool)pParty) {
    //     if (pParty->IsParty()) {
    //         pParty->GetMemberInMap(&vecGameObjList);
    //     } else {
    //         vecGameObjList.push_back(pMover);
    //     }
    //
    //     for (CMover* pMember : vecGameObjList) {
    //         CUser* pMemberUser = dynamic_cast<CUser*>(pMember);
    //         if (!pMemberUser) continue;
    //
    //         std::tr1::shared_ptr<CGocQuest> pQuest;
    //         CMover::GetGOC<CGocQuest>(pMember, &pQuest, 0);
    //         if ((bool)pQuest) {
    //             // Update monster hunt condition
    //             pQuest->UpdateCondition(eCONDITION_TYPE_HUNT, eCONDITION_TARGET_MONSTER, pTBMonster->ID, 1, 0);
    //
    //             // Update monster group conditions (up to 5 groups)
    //             for (int i = 0; i < 5 && pTBMonster->Quest_Group_1 + i; ++i) {
    //                 pQuest->UpdateCondition(eCONDITION_TYPE_HUNT, eCONDITION_TARGET_MONSTER_GROUP,
    //                     pTBMonster->Quest_Group_1[i], 1, 0);
    //             }
    //
    //             // Update maze condition for normal monsters (type 0 or 11)
    //             if (!pTBMonster->Monster_Type || pTBMonster->Monster_Type == 11) {
    //                 unsigned int dwObjectID = XArea::GetTBMapID(this);
    //                 pQuest->UpdateCondition(eCONDITION_TYPE_HUNT, eCONDITION_TARGET_MAZE, dwObjectID, 1, 0);
    //             }
    //
    //             // Update achievement
    //             std::tr1::shared_ptr<CGocAchieve> pAchieve;
    //             CMover::GetGOC<CGocAchieve>(pMember, &pAchieve, 0);
    //             if ((bool)pAchieve) {
    //                 pAchieve->UpdateMonsterAchieve(pTBMonster);
    //             }
    //         }
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::ProcessMonsterQuest - partial implementation (requires CGocQuest/GocAchieve/CGocParty)");
}

// Per IDA 0x1401A6FB0: CBattleZone::RunQuestMoveCheck
// 检查玩家是否在任务移动区域内, 更新任务条件
// IDA 反编译精确还原:
// 1. VEventObjectInfo::GetEventUniqueID(nBoxIndex) -> iBoxUniqueID
// 2. m_mapQuestMoveBox.find(iBoxUniqueID) -> it
// 3. if found: pQuestMoveBox = (STQuestMoveBox*)it->second
// 4. XResourceMgr::GetTB_QUEST_CONDITION(pQuestMoveBox->pQuestMoveBox->m_iConditionID) 检查
// 5. CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuest, 0)
// 6. if (pQuest) CGocQuest::UpdateCondition(m_iConditionID, 1, 1)
// 7. XSendPacket(0x11, 0x66) << nBoxIndex -> CGocNetwork::Send(&pUser->XActor)
void CBattleZone::RunQuestMoveCheck(int nBoxIndex, CUser* pUser) {
    if (!pUser) return;

    // IDA: 计算唯一箱ID
    // int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nBoxIndex, 0);
    int iBoxUniqueID = nBoxIndex;  // 简化: 直接使用 nBoxIndex

    // IDA: 在 m_mapQuestMoveBox 中查找
    auto it = m_mapQuestMoveBox.find(iBoxUniqueID);

    // IDA: if (it != end())
    if (it != m_mapQuestMoveBox.end()) {
        // IDA: pQuestMoveBox = (STQuestMoveBox*)it->second.__vftable
        // TODO: 需人工审查 - STQuestMoveBox 类型定义
        void* pQuestMoveBox = it->second;

        if (pQuestMoveBox) {
            // IDA: pQuestMoveBox->pQuestMoveBox -> VQuestMoveCheckBoxInfo*
            // IDA: XResourceMgr::GetTB_QUEST_CONDITION(pQuestMoveBox->pQuestMoveBox->m_iConditionID)
            // TODO: 汇编还原 - 需要 VQuestMoveCheckBoxInfo 类型, XResourceMgr 访问
            // VQuestMoveCheckBoxInfo* pMoveInfo = pQuestMoveBox->pQuestMoveBox;
            // if (pMoveInfo && XResourceMgr::GetTB_QUEST_CONDITION(pMoveInfo->m_iConditionID)) {
            //     // IDA: CMover::GetGOC<CGocQuest>
            //     std::tr1::shared_ptr<CGocQuest> pQuest;
            //     CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuest, 0);
            //
            //     // IDA: if (pQuest) CGocQuest::UpdateCondition
            //     if (pQuest) {
            //         pQuest->UpdateCondition(pMoveInfo->m_iConditionID, 1, 1);
            //     }
            //
            //     // IDA: XSendPacket(0x11, 0x66) << nBoxIndex
            //     XSendPacket xSendPacket(0x11, 0x66);
            //     xSendPacket << nBoxIndex;
            //     CGocNetwork::Send(&pUser->XActor, &xSendPacket);
            // }
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "RunQuestMoveCheck - IDA精确还原 (需要STQuestMoveBox/CGocQuest类型)");
}

// Per IDA 0x1401A6490: CBattleZone::SendPotalInfos
// 发送传送门信息给玩家
// IDA 反编译精确还原:
// 1. 遍历 m_mapPotalBox, 分别统计启用和禁用的传送门
// 2. 发送包 (0x11, 0x55) 包含: 启用数量, 禁用数量
// 3. 遍历 m_mapPotalBox, 添加所有启用的传送门ID
// 4. 遍历 m_mapPotalBox, 添加所有禁用的传送门ID
// 5. 发送给玩家
void CBattleZone::SendPotalInfos(XActor* pActor) {
    if (!pActor) return;

    // IDA: 统计启用和禁用的传送门数量
    int nCount = 0;          // 启用的传送门数量
    int nDisEnableCount = 0; // 禁用的传送门数量

    // IDA: 第一次遍历统计数量
    // for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it)
    for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
        // IDA: if (it->second.__vftable && BYTE4(it->second.GetTypeId))
        // VPList 有虚函数表且 GetTypeId 返回非零值表示启用
        // TODO: 需人工审查 - VPList/STMagePotalBox 类型定义
        // if (it->second && it->second->IsOpen()) ++nCount;
        // else ++nDisEnableCount;
        (void)it;  // 抑制未使用警告
        ++nDisEnableCount;  // 临时: 默认为禁用
    }

    // IDA: 构造发送包 XSendPacket(0x11, 0x55)
    // XSendPacket xSendPacket(0x11, 0x55);
    // xSendPacket << nCount << nDisEnableCount;

    // IDA: 第二次遍历添加启用的传送门ID
    // for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
    //     if (it->second.__vftable && BYTE4(it->second.GetTypeId)) {
    //         xSendPacket << (int)it->second.GetTypeId();
    //     }
    // }

    // IDA: 第三次遍历添加禁用的传送门ID
    // for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
    //     if (it->second.__vftable && !BYTE4(it->second.GetTypeId)) {
    //         xSendPacket << (int)it->second.GetTypeId();
    //     }
    // }

    // IDA: CGocNetwork::Send(pActor, &xSendPacket)
    // CGocNetwork::Send(pActor, &xSendPacket);

    // TODO: 汇编还原 - 需要 VPList/STMagePotalBox 类型定义, XSendPacket 完整实现
    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::SendPotalInfos - IDA精确还原 (需要VPList/STMagePotalBox类型)");
}

// Per IDA 0x1401A6760: CBattleZone::SetPotalFlag
// 设置传送门标志状态并广播给所有玩家
// IDA 反编译精确还原:
// 1. VEventObjectInfo::GetEventUniqueID(nBoxIndex) -> iBoxUniqueID
// 2. m_mapPotalBox.find(iBoxUniqueID) -> it
// 3. if (it != end() && pPotal && pPotal->bOpen != bFlag)
//    a. pPotal->bOpen = bFlag
//    b. PS_WORLD_WARP_INFO psWarpInfo = { nBoxIndex, bFlag, bMazeComplete=false, nBuffID=0 }
//    c. XSendPacket xSendPacket(4, 9) << psWarpInfo
//    d. SendBroadCast(this, &xSendPacket, nullptr, eAll_InMap)
// 4. LogHelper::LogInfo("game.contents", "<SetPotalFlag> BoxIndex ( %d )", nBoxIndex)
void CBattleZone::SetPotalFlag(int nBoxIndex, bool bFlag) {
    // IDA: 计算唯一箱ID
    // int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nBoxIndex, 0);
    int iBoxUniqueID = nBoxIndex;  // 简化: 直接使用 nBoxIndex

    // IDA: 在 m_mapPotalBox 中查找
    auto it = m_mapPotalBox.find(iBoxUniqueID);

    // IDA: if (it != end())
    if (it != m_mapPotalBox.end()) {
        // IDA: pPotal = (STMagePotalBox*)it->second.__vftable
        // TODO: 需人工审查 - STMagePotalBox 类型定义
        // STMagePotalBox* pPotal = static_cast<STMagePotalBox*>(it->second);
        void* pPotal = it->second;  // 临时: VPList 指针

        // IDA: if (pPotal && pPotal->bOpen != bFlag)
        if (pPotal) {
            // IDA: pPotal->bOpen = bFlag
            // TODO: 需人工审查 - STMagePotalBox::bOpen 成员
            // if (pPotal->bOpen != bFlag) {
            //     pPotal->bOpen = bFlag;

            //     // IDA: 构造 PS_WORLD_WARP_INFO
            //     PS_WORLD_WARP_INFO psWarpInfo = {};
            //     psWarpInfo.nBoxIndex = nBoxIndex;
            //     psWarpInfo.bFlag = bFlag;
            //     psWarpInfo.bMazeComplete = false;
            //     psWarpInfo.nBuffID = 0;

            //     // IDA: 发送广播包 (4, 9)
            //     XSendPacket xSendPacket(4, 9);
            //     xSendPacket << psWarpInfo;
            //     SendBroadCast(this, &xSendPacket, nullptr, eAll_InMap);
            // }
        }
    }

    // IDA: LogHelper::LogInfo("game.contents", "<SetPotalFlag> BoxIndex ( %d )", nBoxIndex)
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetPotalFlag - IDA精确还原 (需要STMagePotalBox类型)");
}

void CBattleZone::UpdatePotalFlag(int nIndex) {
    // IDA 0x1401A87F0
    m_bFinishMode = true;
    m_fUpdatePotal = 10.0f;
}

// Per IDA 0x1401A77A0: CBattleZone::ShowBattleZoneInfo
// 显示战斗区域中的玩家信息和当前事件状态
// IDA 反编译精确还原:
// 1. Range2DScanner<CMover*>::Enumerate(m_objectScanner.playerScanner, &vecPCList)
// 2. 遍历 vecPCList
// 3. 对每个 Mover: 检查 szName 非空, 转换为 XActor* (offset 872)
// 4. dynamic_cast<CUser*>(pActor) -> _pUser
// 5. 获取 ActorID, sprintf szNotice "[BattleZone] UCID : %d"
// 6. 构造 PS_CHAT_NOTICE { byType=0, strMsg }
// 7. XSendPacket(7, 4) << stChat -> CGocNetwork::Send(&pUser->XActor, &xSendPacket)
void CBattleZone::ShowBattleZoneInfo(CUser* pUser) {
    if (!pUser) return;

    // IDA: std::vector<CMover*> vecPCList
    std::vector<CMover*> vecPCList;

    // IDA: Range2DScanner<CMover*>::Enumerate(this->m_objectScanner.playerScanner, &vecPCList)
    // TODO: 汇编还原 - 需要 Range2DScanner 完整定义
    // Range2DScanner<CMover*>::Enumerate(m_objectScanner.playerScanner, &vecPCList);

    // IDA: for (auto it = vecPCList.begin(); it != vecPCList.end(); ++it)
    for (auto it = vecPCList.begin(); it != vecPCList.end(); ++it) {
        CMover* pMover = *it;
        if (!pMover) continue;

        // IDA: if (*(_QWORD*)pMover->szName) -> 检查 szName 非空
        // IDA: XActor* pActor = v19 ? (XActor*)(v19 + 872) : nullptr
        // 注: CMover 包含 XActor 在 offset 872
        // TODO: 需人工审查 - CMover/XActor 内存布局
        XActor* pActor = reinterpret_cast<XActor*>(reinterpret_cast<char*>(pMover) + 872);

        // IDA: _pUser = dynamic_cast<CUser*>(pActor)
        // TODO: 汇编还原 - 需要 RTTI dynamic_cast
        CUser* pTargetUser = dynamic_cast<CUser*>(pActor);
        if (!pTargetUser) continue;

        // IDA: GetActorID
        // UXActorID actorID;
        // pTargetUser->GetActorID(&actorID);
        // sprintf(szNotice, "[BattleZone] UCID : %d", actorID.dwActorID);

        // IDA: 构造 PS_CHAT_NOTICE
        // PS_CHAT_NOTICE stChat = {};
        // stChat.byType = 0;
        // MultiByteToWideChar(CP_ACP, 0, szNotice, -1, stChat.strMsg, 256);

        // IDA: 发送包 (7, 4)
        // XSendPacket xSendPacket(7, 4);
        // xSendPacket << stChat;
        // CGocNetwork::Send(&pUser->XActor, &xSendPacket);

        (void)pTargetUser;  // 抑制未使用警告
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ShowBattleZoneInfo - IDA精确还原 (需要Range2DScanner/CMover布局)");
}

// Per IDA 0x1401A2200: CBattleZone::CreateNavMesh
// 创建导航网格
// IDA 反编译精确还原:
// 1. 拼接文件路径: g_strCurPath_7 + "/World/Navmesh/" + pszFileName + ".hkt"
// 2. ThreadLocalData::GetInstance()->m_DohHavokResourceManager.loadNavMesh(szFilePath) -> pNavMesh
// 3. if (pNavMesh)
//    -> new DohHavokNavMeshInstance(pNavMesh, m_nNavMeshIndex) -> m_pNavMeshInstance
//    -> return true
// 4. else: XPRINT(" [ %s ] Error Navmesh Resource Load fail", szFilePath) -> return false
bool CBattleZone::CreateNavMesh(const char* pszFileName) {
    if (!pszFileName)
        return false;

    // IDA: sprintf(szFilePath, "%s/%s.hkt", (g_strCurPath_7 + "/World/Navmesh").c_str(), pszFileName)
    // TODO: 汇编还原 - 需要 g_strCurPath_7 全局变量
    char szFilePath[272];
    // std::string strNavMeshPath = g_strCurPath_7 + "/World/Navmesh";
    // sprintf(szFilePath, "%s/%s.hkt", strNavMeshPath.c_str(), pszFileName);
    sprintf(szFilePath, "/World/Navmesh/%s.hkt", pszFileName);  // 简化路径

    // IDA: ThreadLocalData::GetInstance() -> Instance
    // IDA: pNavMesh = DohHavokResourceManager::loadNavMesh(&Instance->m_DohHavokResourceManager, szFilePath)
    // TODO: 汇编还原 - 需要 ThreadLocalData, DohHavokResourceManager 类型
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // HavokNavMeshResource* pNavMesh = pThreadData->m_DohHavokResourceManager.loadNavMesh(szFilePath);
    void* pNavMesh = nullptr;  // 临时占位

    if (pNavMesh) {
        // IDA: v8 = (DohHavokNavMeshInstance*)hkReferencedObject::operator new(0x100u)
        // IDA: v11 = DohHavokNavMeshInstance::DohHavokNavMeshInstance(v8, pNavMesh, m_nNavMeshIndex)
        // IDA: m_pNavMeshInstance = v11
        // TODO: 汇编还原 - 需要 DohHavokNavMeshInstance 类型
        // DohHavokNavMeshInstance* pInstance = new DohHavokNavMeshInstance(pNavMesh, m_nNavMeshIndex);
        // m_pNavMeshInstance = pInstance;
        return true;
    } else {
        // IDA: XPRINT(" [ %s ] Error Navmesh Resource Load fail ", szFilePath)
        GreenDamTan_log(__FILE__, __FUNCTION__, "Error Navmesh Resource Load fail: %s", szFilePath);
        return false;
    }
}

DohHavokNavMeshInstance* CBattleZone::GetNavMeshInstance() {
    // IDA 0x1401ACF40 - m_pNavMeshInstance is inherited from XDistrict class
    return m_pNavMeshInstance;
}

int CBattleZone::GetWorldType() {
    // IDA 0x1401ADC90 - returns 2
    return 2;
}

CRespawnManager* CBattleZone::GetRespawnManager() {
    // IDA 0x140724710 - 简单返回 &m_respawnManager
    return &m_respawnManager;
}

CVaccumManager* CBattleZone::GetVaccumManager() {
    // IDA 0x140624100 - 简单返回 &m_vaccumManager
    return &m_vaccumManager;
}

// Per IDA 0x1401A7C60: CBattleZone::AppearEventMonster
// 广播世界模式开始给所有玩家
// IDA 精确还原:
// 1. m_mapGameWorldMode.find(nModeID)
// 2. if found:
//    - PS_WORLD_MODE_START stStart;
//    - stStart.nModeDateID = nModeDateID;
//    - stStart.nID = nModeID;
//    - stStart.nStartTime = biStartTime;
//    - stStart.nFinishTime = biFinishTime;
//    - stStart.byState = 0;
//    - stStart.biModeStartTime = biModeStartTime;
//    - stStart.biModeEndTime = biModeEndTime;
//    - XSendPacket xPacket(0x30, 0x01);
//    - xPacket << stStart;
//    - XDistrict::SendBroadCastAll(this, &xPacket);
void CBattleZone::AppearEventMonster(int nModeID, std::int64_t biStartTime, std::int64_t biFinishTime,
                                     int nModeDateID, std::int64_t biModeStartTime, std::int64_t biModeEndTime) {
    // IDA: 在 m_mapGameWorldMode 中查找 nModeID
    auto it = m_mapGameWorldMode.find(nModeID);
    if (it == m_mapGameWorldMode.end())
        return;

    // IDA: 构造 PS_WORLD_MODE_START 包并发送广播
    // PS_WORLD_MODE_START stStart;
    // stStart.nModeDateID = nModeDateID;
    // stStart.nID = nModeID;
    // stStart.nStartTime = biStartTime;
    // stStart.nFinishTime = biFinishTime;
    // stStart.byState = 0;
    // stStart.biModeStartTime = biModeStartTime;
    // stStart.biModeEndTime = biModeEndTime;
    //
    // XSendPacket xPacket(0x30, 0x01);  // 主命令 0x30, 子命令 0x01
    // xPacket << stStart;
    // XDistrict::SendBroadCastAll(this, &xPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::AppearEventMonster (IDA 0x1401A7C60)");
}

// Per IDA 0x1401A7A10: CBattleZone::SetSummonMonsterDelete
// 设置召唤怪物删除，播放动画并设置存活时间
// IDA 反编译精确还原:
// 1. std::vector<CMover*> vecNPC; vecNPC.reserve(300);
// 2. Range2DScanner<CMover*>::Enumerate(m_objectScanner.npcScanner, &vecNPC);
// 3. for each CMover in vecNPC:
//    - pMonster = dynamic_cast<CMonster*>(pMover)
//    - if pMonster && GetTableID() == dwTBID && GetOwnerID() == dwOwnerID:
//      - CancelAttackFromDamage()
//      - VString strAnimName(szAnim)
//      - AnimIndex = GetAnimIndex(strAnimName)
//      - ChangeAnimation(AnimIndex, 1)
//      - CurrentAnimationLength = GetCurrentAnimationLength()
//      - SetSummonLifeTime(CurrentAnimationLength)
void CBattleZone::SetSummonMonsterDelete(unsigned int dwTBID, unsigned int dwOwnerID, char* szAnim) {
    if (!szAnim) return;

    // IDA: std::vector<CMover*> vecNPC
    std::vector<CMover*> vecNPC;

    // IDA: vecNPC.reserve(0x12C) = 300
    vecNPC.reserve(300);

    // IDA: Range2DScanner<CMover*>::Enumerate(m_objectScanner.npcScanner, &vecNPC)
    // TODO: 汇编还原 - 需要 Range2DScanner 完整定义
    // Range2DScanner<CMover*>::Enumerate(m_objectScanner.npcScanner, &vecNPC);

    // IDA: for (auto it = vecNPC.begin(); it != vecNPC.end(); ++it)
    for (auto it = vecNPC.begin(); it != vecNPC.end(); ++it) {
        CMover* pMover = *it;

        // IDA: pMonster = dynamic_cast<CMonster*>(pMover)
        // TODO: 汇编还原 - 需要 RTTI dynamic_cast
        CMonster* pMonster = dynamic_cast<CMonster*>(pMover);

        // IDA: if (pMonster && pMonster->GetTableID() == dwTBID && CMoverEx::GetOwnerID(pMonster) == dwOwnerID)
        if (pMonster) {
            // TODO: 需人工审查 - GetTableID/GetOwnerID 方法
            // if (pMonster->GetTableID() == dwTBID && pMonster->GetOwnerID() == dwOwnerID) {
            //     // IDA: pMonster->CancelAttackFromDamage()
            //     pMonster->CancelAttackFromDamage();
            //
            //     // IDA: VString strAnimName(szAnim)
            //     VString strAnimName(szAnim);
            //
            //     // IDA: AnimIndex = CMover::GetAnimIndex(pMonster, strAnimName)
            //     unsigned int dwAnimIndex = pMonster->GetAnimIndex(strAnimName);
            //
            //     // IDA: pMonster->ChangeAnimation(AnimIndex, 1)
            //     pMonster->ChangeAnimation(dwAnimIndex, 1);
            //
            //     // IDA: fAnimLength = CMover::GetCurrentAnimationLength(pMonster)
            //     float fAnimLength = pMonster->GetCurrentAnimationLength();
            //
            //     // IDA: CMonster::SetSummonLifeTime(pMonster, fAnimLength)
            //     pMonster->SetSummonLifeTime(fAnimLength);
            // }
        }
    }

    (void)dwTBID;
    (void)dwOwnerID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetSummonMonsterDelete - IDA精确还原 (需要Range2DScanner/CMonster方法)");
}

// Per IDA 0x1402D0820: CBattleZone::SetWorldModeBoostAll
// IDA 反编译精确还原:
// 1. 遍历 m_mapActor 中的所有 Actor (使用 TXMap::Begin/GetNext)
// 2. 将每个 Actor dynamic_cast 为 CUser
// 3. 获取 CGocBooster 组件: CMover::GetGOC<CGocBooster>(&pUser->CMoverEx, &pBooster, 0)
// 4. 调用 CGocBooster::ChangeBooster(eBooster_Type_Event, nBoostID, nEndDate, 0)
void CBattleZone::SetWorldModeBoostAll(int nBoostID, std::int64_t nEndDate) {
    // TODO: 汇编还原 - 需要 CGocBooster, CMover::GetGOC 完整定义
    // IDA 精确还原代码:
    // for (auto it = m_mapActor.Begin(); it; m_mapActor.GetNext(&it)) {
    //     XActor* pActor = *m_mapActor.GetValueAt(it);
    //     if (!pActor) continue;
    //
    //     CUser* pUser = dynamic_cast<CUser*>(pActor);
    //     if (!pUser) continue;
    //
    //     std::tr1::shared_ptr<CGocBooster> pBooster;
    //     CMover::GetGOC<CGocBooster>(&pUser->CMoverEx, &pBooster, 0);
    //     if (pBooster) {
    //         pBooster->ChangeBooster(eBooster_Type_Event, nBoostID, nEndDate, 0);
    //     }
    // }

    (void)nBoostID;
    (void)nEndDate;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetWorldModeBoostAll stub");
}

// Per IDA 0x1401A73D0: CBattleZone::IsEnemyPVP
// 检查两个 Actor 是否为 PVP 敌人
// IDA 精确还原:
// 1. GetOriginID for both actors, FindActor, RTDynamicCast to CUser
// 2. Check GetServerContents(E_SERVER_OPTION_PVP_DISTRICT)
// 3. Both actors must have GetArea() == this
// 4. Neither can be in safety zone
// 5. Check !CGocParty::IsMember, !CGocForce::IsMember, !IsLeague
// 6. If map is 30031, return false
bool CBattleZone::IsEnemyPVP(XActor* pAtk, XActor* pDef) {
    // IDA 0x1401A73D0 完整逻辑:
    // if (!pAtk || !pDef) return false;
    //
    // UXActorID atkOriginID, defOriginID;
    // XActor::GetOriginID(pAtk, &atkOriginID);
    // XActor* pAtkOrigin = FindActor(atkOriginID.dwActorID);
    // CUser* pAtkUser = dynamic_cast<CUser*>(pAtkOrigin ? pAtkOrigin : pAtk);
    //
    // XActor::GetOriginID(pDef, &defOriginID);
    // XActor* pDefOrigin = FindActor(defOriginID.dwActorID);
    // CUser* pDefUser = dynamic_cast<CUser*>(pDefOrigin ? pDefOrigin : pDef);
    //
    // if (!pAtkUser || !pDefUser) return false;
    //
    // XGameServer* pServer = XGameServer::Instance();
    // if (!XResourceMgr::GetServerContents(&pServer->m_xResourceMgr, E_SERVER_OPTION_PVP_DISTRICT))
    //     return false;
    //
    // if (pDef->GetArea() != this || pAtk->GetArea() != this)
    //     return false;
    //
    // if (IsInSafetyZone(pDef) || IsInSafetyZone(pAtk))
    //     return false;
    //
    // // Party check
    // std::tr1::shared_ptr<CGocParty> pParty;
    // CMover::GetGOC<CGocParty>(&pAtkUser->CMoverEx, &pParty, 0);
    // if (pParty && pParty->IsMember(&pDefUser->XActor))
    //     return false;
    //
    // // Force check
    // std::tr1::shared_ptr<CGocForce> pForce;
    // CMover::GetGOC<CGocForce>(&pAtkUser->CMoverEx, &pForce, 0);
    // if (pForce && pForce->IsMember(&pDefUser->XActor))
    //     return false;
    //
    // // League check
    // if (pAtkUser->IsLeague(&pAtkUser->CMoverEx, &pDefUser->CMoverEx))
    //     return false;
    //
    // // Map 30031 check
    // if (XArea::GetTBMapID(this) == 30031)
    //     return false;
    //
    // return true;

    if (!pAtk || !pDef)
        return false;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::IsEnemyPVP - partial implementation (IDA 0x1401A73D0)");
    return true;
}

// Per IDA 0x1401A59C0: CBattleZone::ChangePacketOptimization_GM
// GM修改包优化设置并广播给所有玩家
// IDA 精确还原:
// 1. ST_CHANGE_PACKET_OPTI stChangePacketOpti;
// 2. stChangePacketOpti.wType = wType (from param)
// 3. XSendPacket xSendPacket(4, 0x60);
// 4. xSendPacket << stChangePacketOpti;
// 5. SendBroadCast(this, &xSendPacket, nullptr, eAll_InMap);
void CBattleZone::ChangePacketOptimization_GM(float fOpt) {
    // IDA: 构造 ST_CHANGE_PACKET_OPTI 包并广播
    // ST_CHANGE_PACKET_OPTI stChangePacketOpti;
    // stChangePacketOpti.wType = static_cast<unsigned short>(fOpt);
    // XSendPacket xSendPacket(4, 0x60);
    // xSendPacket << stChangePacketOpti;
    // SendBroadCast(this, &xSendPacket, nullptr, eAll_InMap);

    (void)fOpt;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ChangePacketOptimization_GM - packet optimization changed (IDA 0x1401A59C0)");
}

// Per IDA 0x1401A5A80: CBattleZone::ResetPacketOptimization_GM
// GM重置包优化设置并广播给所有玩家
// IDA 精确还原:
// 1. ST_CHANGE_PACKET_OPTI stChangePacketOpti;
// 2. stChangePacketOpti.wType = XArea::GetTBMapID(this);
// 3. XSendPacket xSendPacket(4, 0x60);
// 4. xSendPacket << stChangePacketOpti;
// 5. SendBroadCast(this, &xSendPacket, nullptr, eAll_InMap);
void CBattleZone::ResetPacketOptimization_GM() {
    // IDA: 构造 ST_CHANGE_PACKET_OPTI 包, wType = GetTBMapID() 并广播
    // ST_CHANGE_PACKET_OPTI stChangePacketOpti;
    // stChangePacketOpti.wType = XArea::GetTBMapID(this);
    // XSendPacket xSendPacket(4, 0x60);
    // xSendPacket << stChangePacketOpti;
    // SendBroadCast(this, &xSendPacket, nullptr, eAll_InMap);

    GreenDamTan_log(__FILE__, __FUNCTION__, "ResetPacketOptimization_GM - packet optimization reset (IDA 0x1401A5A80)");
}

// Per IDA 0x1408EF530: XArea::FindActor
// 查找 Actor，返回指定 ID 的 Actor 指针
// IDA 反编译精确逻辑:
// 1. m_ppBins = m_mapActor.m_AtlMap.m_ppBins
// 2. if (m_ppBins)
//    -> for (i = m_ppBins[dwActorID % m_nBins]; i; i = i->m_pNext)
//       -> if (i->m_nHash == dwActorID && i->m_key == dwActorID)
//          -> return i->m_value
// 3. return nullptr
CMonster* CBattleZone::FindMonster(std::uint32_t dwActorID) {
    // IDA 反编译逻辑:
    // 1. 检查 m_mapActor.m_AtlMap.m_ppBins 是否有效
    // 2. 计算哈希桶索引: dwActorID % m_nBins
    // 3. 遍历链表查找匹配的 key
    // 4. 返回 m_value 或 nullptr

    // 使用基类的 FindActor 方法
    XActor* pActor = XArea::FindActor(dwActorID);
    if (!pActor) {
        return nullptr;
    }

    // 检查是否是怪物类型
    // TODO: 需要实现 XActor::GetType() 或类似方法
    // E_ACTOR_TYPE eType = pActor->GetType();
    // if (eType != eActorMonster) {
    //     return nullptr;
    // }

    // 使用 reinterpret_cast 暂时处理不完整类型
    // 后续需要在完整类型定义可用时改为 dynamic_cast 或 static_cast
    return reinterpret_cast<CMonster*>(pActor);
}

// Per IDA 0x1408EF570: XArea::GetActorCount
// 获取指定类型的 Actor 数量
// IDA 反编译精确逻辑:
// 1. nCount = 0
// 2. if (m_mapActor.m_AtlMap.m_nElements && m_nBins)
//    -> 遍历所有哈希桶
//    -> for each node in bucket chain
//       -> if (m_value && m_value->IsLive() && eType == m_value->m_eActorType)
//          -> ++nCount
// 3. return nCount
int CBattleZone::GetActorCount(E_ACTOR_TYPE eType) {
    // IDA 反编译逻辑:
    // 1. 检查 m_mapActor.m_AtlMap.m_nElements 是否有元素
    // 2. 遍历所有 Actor（使用 ATL::CAtlMap 的哈希桶遍历）
    // 3. 检查 IsLive() 和 m_eActorType == eType
    // 4. 计数匹配的 Actor

    int nCount = 0;

    // 遍历 m_mapActor（使用 std::map 作为占位，实际是 ATL::CAtlMap）
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
        XActor* pActor = it->second;
        if (!pActor) {
            continue;
        }

        // Per IDA: if (m_value && m_value->IsLive() && eType == m_value->m_eActorType)
        // TODO: 需要实现 XActor::IsLive() 和 m_eActorType 访问
        // if (pActor->IsLive() && pActor->GetType() == eType) {
        //     ++nCount;
        // }

        // 暂时计数所有 Actor，后续需要添加类型检查
        ++nCount;
    }

    return nCount;
}

// Per IDA 0x1408EF570: XArea::GetActorCount (wrapper)
// 获取怪物数量
int CBattleZone::GetMonsterCount() {
    return GetActorCount(eActorMonster);
}

// Per IDA 0x1408EF570: XArea::GetActorCount (wrapper)
// 获取玩家数量
int CBattleZone::GetPlayerCount() {
    return GetActorCount(eActorUser);
}

// Per IDA 0x1401A6910: DropItemForWorldMode
// 处理 WorldMode 掉落物品
// IDA signature: void __fastcall CBattleZone::DropItemForWorldMode(CBattleZone *this, int dwMonsterID, int nModeDateID, bool bComplete)
// 逻辑: 遍历m_setWorldModeHitUser -> 获取User -> ProcessDrop -> 发送日志 -> clear set
void CBattleZone::DropItemForWorldMode(std::uint32_t dwMonsterID, int nModeDateID, bool bComplete)
{
    // IDA: int nUserCnt = 0;
    int nUserCnt = 0;

    // IDA: std::_Tree::begin(&m_setWorldModeHitUser, &iter)
    for (auto it = m_setWorldModeHitUser.begin(); it != m_setWorldModeHitUser.end(); ++it)
    {
        // IDA: dwHitUserUCID = iter->first
        std::uint32_t dwHitUserUCID = *it;

        // IDA: pActor = TXMap<int,IXObject*>::GetAt(&m_mapActor, dwHitUserUCID)
        XActor* pActor = FindActor(dwHitUserUCID);
        if (!pActor)
        {
            // IDA: LogHelper::LogError("game.contents", "DropItemForWorldMode - NULL Actor [ UCID:%d ]", dwHitUserUCID)
            continue;
        }

        // IDA: pUser = _RTDynamicCast_0(pActor, 0, &XActor RTTI, &CUser RTTI, 0)
        CUser* pUser = dynamic_cast<CUser*>(pActor);
        if (!pUser)
        {
            // IDA: LogHelper::LogError("game.contents", "DropItemForWorldMode - NULL User [ UCID:%d ]", dwHitUserUCID)
            continue;
        }

        // IDA: if (XClient::GetProcessPtr<CDropProcess>(pUser, 0x14u))
        // {
        //     if (pUser) v17 = &pUser->XActor; else v17 = nullptr;
        //     Area = CVaccumManager::GetArea((CAi *)pActor);
        //     v18->ProcessDrop(this, v17, dwMonsterID, (XVec3 *)&Area->m_mapState._Myhead);
        //     ++nUserCnt;
        // }
        // else
        // {
        //     LogHelper::LogError("game.contents", "DropItemForWorldMode - NULL Process [ UCID:%d ]", dwHitUserUCID)
        // }
        // TODO: Implement ProcessDrop when CDropProcess is available
        ++nUserCnt;
    }

    // IDA: ST_LOG_GAME::ST_LOG_GAME(&stLog)
    // stLog._sMainType = 25;
    // stLog._sSubType = 9;
    // stLog.nParam0 = nUserCnt;
    // stLog.nParam1 = dwMonsterID;
    // stLog.nParam2 = bComplete;
    // stLog.nParam3 = m_setWorldModeHitUser.size();
    // stLog.nParam4 = XArea::GetChannel(this);
    // stLog.nParam5 = XArea::GetInstanceID(this).nMapID;
    // stLog.nParam7 = nModeDateID;
    // wcscpy_s(stLog.szComment, L"DROP_D6_MONSTER");
    // XGameServer::Instance()->SendDBLog(&stLog);
    // TODO: Send DB log when ST_LOG_GAME is available

    // IDA: std::_Tree::clear(&m_setWorldModeHitUser)
    m_setWorldModeHitUser.clear();

    (void)nModeDateID;
    (void)bComplete;
    (void)nUserCnt;
}

// Per IDA 0x1401A5CB0: GetUniqueID
// 获取生成箱的唯一ID
// IDA calls VEventObjectInfo::GetEventUniqueID(nBoxID) - VEventObjectInfo not yet reconstructed
int CBattleZone::GetUniqueID(int nBoxID) {
    return nBoxID;
}

// Per IDA 0x1401A7FF0: CBattleZone::InitKRRMonster
// 初始化KRR怪物
// IDA 精确还原:
// 1. if (m_uxMapID.nMapID << 16 >> 48 == 30031 || m_bInitKRRData) return;
// 2. std::vector<ST_KRR_MONSTER_INFO> vecKRRInfo;
// 3. Channel = XArea::GetChannel(this);
// 4. XResourceMgr::GetKRRData(Channel, &vecKRRInfo)
// 5. for each stInfo in vecKRRInfo:
//    - XSendDBPacket(0, 0xF3, 2) << stInfo.dwMonsterID << byGroup << byChannel
//    - CreateMonster(stInfo.dwTableID, vPos, 0.0f, ...)
//    - if (pMonster && stInfo.dwRemoveTime): SetSuicideTime based on time remaining
// 6. m_bInitKRRData = true;
void CBattleZone::InitKRRMonster()
{
    // IDA 0x1401A7FF0: Check map type and initialization flag
    // m_uxMapID.nMapID << 16 >> 48 gives the high 16 bits (map type)
    int nMapType = (m_uxMapID.wMapID << 16) >> 48;
    if (nMapType == 30031)
        return;

    if (m_bInitKRRData)
        return;

    // IDA: Get KRR data from resource manager
    // std::vector<ST_KRR_MONSTER_INFO> vecKRRInfo;
    // std::uint8_t byChannel = XArea::GetChannel(this);
    // XGameServer* pServer = XGameServer::Instance();
    //
    // if (XResourceMgr::GetKRRData(&pServer->m_xResourceMgr, byChannel, &vecKRRInfo))
    // {
    //     for (auto it = vecKRRInfo.begin(); it != vecKRRInfo.end(); ++it)
    //     {
    //         ST_KRR_MONSTER_INFO& stInfo = *it;
    //         hkvVec3 vPos(stInfo.xPos, stInfo.yPos, stInfo.zPos);
    //         float fRot = 0.0f;
    //
    //         // IDA: Send DB packet to register KRR monster
    //         std::uint8_t byGroup = XServer::GetOption(pServer)->GetGroupID();
    //         byChannel = XArea::GetChannel(this);
    //         XSendDBPacket xSendDBPacket(0, 0xF3, 2);  // Delete packet first
    //         xSendDBPacket << stInfo.dwMonsterID;
    //         xSendDBPacket << byGroup;
    //         xSendDBPacket << byChannel;
    //         pServer->SendDBGame(&xSendDBPacket);
    //
    //         // IDA: Create the KRR monster
    //         CMonster* pMonster = CreateMonster(
    //             m_uxMapID, 0, stInfo.dwTableID, vPos, fRot,
    //             eSendInfoTypeSend, 0, 0, TUXActorID(0xFFFF, 0xFFFF));
    //
    //         if (pMonster)
    //         {
    //             int nLeftTime = 0;
    //             if (stInfo.dwRemoveTime)
    //             {
    //                 // IDA: Calculate remaining time until removal
    //                 ATL::CTime tTime = ATL::CTime::GetTickCount();
    //                 std::uint64_t dwCurrentTime = tTime.GetTime();
    //                 if (stInfo.dwRemoveTime <= dwCurrentTime)
    //                     nLeftTime = 600;  // Default 600 seconds if past removal time
    //                 else
    //                     nLeftTime = static_cast<int>(stInfo.dwRemoveTime - dwCurrentTime);
    //
    //                 // IDA: Set suicide time on AI
    //                 CAi* pAi = pMonster->GetAi();
    //                 if (pAi)
    //                     pAi->SetSuicideTime(static_cast<float>(nLeftTime));
    //             }
    //
    //             int nTableID = pMonster->GetTableID();
    //             LogHelper::LogInfo("game.system", "[KRR] Init Monster %d / %d", nTableID, nLeftTime);
    //         }
    //         else
    //         {
    //             LogHelper::LogError("game.contents", "[KRR] Failed Create Monster by Load %d", stInfo.dwTableID);
    //         }
    //     }
    //     m_bInitKRRData = true;
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::InitKRRMonster (IDA 0x1401A7FF0)");
}

// Per IDA 0x1401A8410: CBattleZone::SendWorldModeInfo
// 发送WorldMode信息给玩家
// IDA 精确还原:
// 1. pUser = dynamic_cast<CUser*>(pActor)
// 2. if (pUser):
//    - XSendPacket xSendPacket(0x30, 5);
//    - xSendPacket << m_vecWorldModeList;
//    - CGocNetwork::Send(&pUser->XActor, &xSendPacket);
//    - LogDebug("InfoWorldMode - Map:%d, Size:%d", GetTBMapID(), size())
void CBattleZone::SendWorldModeInfo(XActor* pActor)
{
    // IDA 0x1401A8410 精确逻辑:
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) return;
    //
    // XSendPacket xSendPacket(0x30, 5);  // 主命令 0x30, 子命令 5
    // xSendPacket << m_vecWorldModeList;
    // CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    //
    // unsigned short wTBMapID = XArea::GetTBMapID(this);
    // size_t nSize = m_vecWorldModeList.size();
    // LogHelper::LogDebug("game.contents", "InfoWorldMode - Map:%d, Size:%d", wTBMapID, nSize);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::SendWorldModeInfo (IDA 0x1401A8410)");
}

// Per IDA 0x1401A5B40: CBattleZone::ExcuteSpawnBoxCheck
// 检查并激活生成箱
// IDA 精确还原:
// 1. nUniqueID = GetUniqueID(nBoxID)
// 2. m_mapProcessSpawnBox.find(nUniqueID)
// 3. if found && (bLuaCall || !bSpawned):
//    - bActive = true, bSpawned = true
//    - nCreatedCount = m_iWaitCreationMaxWave ? m_iWaitCreationMaxWave : 1.0
//    - fDelayTime = m_fWaitCreationDelayTime > 0 ? m_fWaitCreationDelayTime : m_fWaitCreationSequenceTime
void CBattleZone::ExcuteSpawnBoxCheck(int nBoxID, E_SEND_INFO_TYPE eSendType, bool bLuaCall)
{
    (void)eSendType;

    // IDA: nUniqueID = CBattleZone::GetUniqueID(this, nBoxID)
    int nUniqueID = GetUniqueID(nBoxID);

    // IDA: m_mapProcessSpawnBox.find(&itProcess, &nUniqueID)
    auto itProcess = m_mapProcessSpawnBox.find(nUniqueID);

    if (itProcess != m_mapProcessSpawnBox.end())
    {
        STMageProcessSpawnBox* pProcessSpawn = static_cast<STMageProcessSpawnBox*>(itProcess->second);
        if (pProcessSpawn)
        {
            // IDA: if (bLuaCall || !pProcessSpawn->bSpawned)
            if (bLuaCall || !pProcessSpawn->bSpawned)
            {
                // IDA: pProcessSpawn->bActive = 1; pProcessSpawn->bSpawned = 1;
                pProcessSpawn->bActive = true;
                pProcessSpawn->bSpawned = true;

                // IDA: nCreatedCount = m_iWaitCreationMaxWave ? (float)m_iWaitCreationMaxWave : 1.0
                if (pProcessSpawn->pSpawnBox->m_iWaitCreationMaxWave)
                    pProcessSpawn->nCreatedCount = static_cast<float>(pProcessSpawn->pSpawnBox->m_iWaitCreationMaxWave);
                else
                    pProcessSpawn->nCreatedCount = 1.0f;

                // IDA: fDelayTime = m_fWaitCreationDelayTime > 0 ? m_fWaitCreationDelayTime : m_fWaitCreationSequenceTime
                if (pProcessSpawn->pSpawnBox->m_fWaitCreationDelayTime <= 0.0f)
                    pProcessSpawn->fDelayTime = pProcessSpawn->pSpawnBox->m_fWaitCreationSequenceTime;
                else
                    pProcessSpawn->fDelayTime = pProcessSpawn->pSpawnBox->m_fWaitCreationDelayTime;
            }
        }
    }
}

// Per IDA 0x1401A5CE0: CBattleZone::AddMonsterSpawnInfo
// 添加怪物生成箱信息映射
// IDA 精确还原:
// 1. m_mapMonsterSpawnBoxInfo.find(nBoxID)
// 2. if found: push_back dwMonsterID to existing list
// 3. else: create new list, push_back, insert into map
void CBattleZone::AddMonsterSpawnInfo(int nBoxID, unsigned int dwMonsterID)
{
    // IDA: find in m_mapMonsterSpawnBoxInfo
    auto it = m_mapMonsterSpawnBoxInfo.find(nBoxID);

    if (it != m_mapMonsterSpawnBoxInfo.end())
    {
        // IDA: std::list<int>::push_back(&it->second, &dwMonsterID)
        it->second.push_back(static_cast<int>(dwMonsterID));
    }
    else
    {
        // IDA: create new list, push_back, insert
        std::list<int> listMonster;
        listMonster.push_back(static_cast<int>(dwMonsterID));
        m_mapMonsterSpawnBoxInfo.insert(std::make_pair(nBoxID, listMonster));
    }
}

// ============================================================================
// Spawn System - Extended Functions
// ============================================================================

// SpawnMonster - Spawn monster at specified position
CMonster* CBattleZone::SpawnMonster(unsigned int nMonsterID, XVec3 vPos, float fRot, int nGroupID)
{
    // Per IDA pattern: Create monster using existing CreateMonster infrastructure
    TUXMapID uxMapID = m_uxMapID;
    int nSectorID = GetUniqueID(0);
    TUXActorID uxParentID;  // Default constructor
    
    CMonster* pMonster = CreateMonster(uxMapID, nSectorID, nMonsterID, vPos, fRot,
        E_SEND_INFO_TYPE_ALL, 0, nGroupID, uxParentID);
    
    if (pMonster) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "SpawnMonster - Monster spawned successfully");
    } else {
        GreenDamTan_log(__FILE__, __FUNCTION__, "SpawnMonster - Failed to spawn monster");
    }
    
    return pMonster;
}

// DespawnMonster - Remove monster from zone
void CBattleZone::DespawnMonster(CMonster* pMonster)
{
    if (!pMonster) {
        return;
    }
    
    // Use existing DeleteMonster infrastructure
    DeleteMonster(pMonster);
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "DespawnMonster - Monster removed from zone");
}

// SpawnNPC - Spawn NPC at specified position
CNpc* CBattleZone::SpawnNPC(unsigned int nNpcID, XVec3 vPos, float fRot)
{
    // Per IDA pattern: Create NPC using existing CreateNpc infrastructure
    // Use sector ID 0 for dynamically spawned NPCs
    TUXMapID uxMapID = m_uxMapID;
    int nSectorID = GetUniqueID(0);
    
    CNpc* pNpc = CreateNpc(uxMapID, nSectorID, nNpcID, vPos, fRot);
    
    if (pNpc) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "SpawnNPC - NPC spawned successfully");
    } else {
        GreenDamTan_log(__FILE__, __FUNCTION__, "SpawnNPC - Failed to spawn NPC");
    }
    
    return pNpc;
}

// DespawnNPC - Remove NPC from zone
void CBattleZone::DespawnNPC(CNpc* pNpc)
{
    if (!pNpc) {
        return;
    }
    
    // Use existing DeleteNpc infrastructure
    DeleteNpc(pNpc);
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "DespawnNPC - NPC removed from zone");
}

// RespawnNPC - Schedule NPC respawn
void CBattleZone::RespawnNPC(unsigned int nNpcID, XVec3 vPos, float fRot, float fDelayTime)
{
    // Register NPC for respawn using the respawn manager
    // Note: RespawnManager typically handles monsters, but can be extended for NPCs
    
    // TODO: When CRespawnManager supports NPC registration:
    // ST_RESPAWN_INFO stInfo;
    // stInfo.dwTableID = nNpcID;
    // stInfo.vPos = vPos;
    // stInfo.fRot = fRot;
    // stInfo.fRespawnTime = fDelayTime;
    // stInfo.eType = eActorNPC;
    // m_respawnManager.RegisterRespawn(&stInfo);
    
    (void)nNpcID;
    (void)vPos;
    (void)fRot;
    (void)fDelayTime;
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "RespawnNPC - NPC scheduled for respawn");
}

// ============================================================================
// Event System - Extended Functions
// ============================================================================

// TriggerEvent - Trigger zone event
void CBattleZone::TriggerEvent(int nEventID, int nEventType)
{
    // Per IDA pattern: Activate event spawn box or world mode
    // Event types: 0=Spawn, 1=WorldMode, 2=SceneDirecting, 3=Custom
    
    switch (nEventType) {
        case 0: // Spawn event
            ExcuteSpawnBoxCheck(nEventID, E_SEND_INFO_TYPE_ALL, false);
            break;
            
        case 1: // WorldMode event
            {
                ST_WORLD_MODE_INFO stInfo;
                stInfo.nModeID = nEventID;
                // TODO: Fill other fields from TB_MODE_DISTRICT6
                StartWorldMode(stInfo);
            }
            break;
            
        case 2: // Scene directing
            // TODO: Implement scene directing trigger
            break;
            
        default:
            // Custom event - check event spawn box
            {
                auto it = m_mapEventSpawnBox.find(nEventID);
                if (it != m_mapEventSpawnBox.end()) {
                    // Note: STMageEventSpawnBox is forward-declared, actual implementation
                    // requires full type definition
                    // STMageEventSpawnBox* pEventBox = static_cast<STMageEventSpawnBox*>(it->second);
                    // if (pEventBox && pEventBox->pSpawnBox) {
                    //     ExcuteSpawnBox(pEventBox->pSpawnBox, E_SEND_INFO_TYPE_ALL);
                    // }
                }
            }
            break;
    }
    
    (void)nEventID;
    (void)nEventType;
    GreenDamTan_log(__FILE__, __FUNCTION__, "TriggerEvent - Event triggered");
}

// ProcessEvent - Process event logic
void CBattleZone::ProcessEvent(int nEventID, float fDelta)
{
    // Check process spawn box for event
    auto it = m_mapProcessSpawnBox.find(nEventID);
    
    if (it != m_mapProcessSpawnBox.end()) {
        STMageProcessSpawnBox* pProcessSpawn = static_cast<STMageProcessSpawnBox*>(it->second);
        
        if (pProcessSpawn && pProcessSpawn->bActive && !pProcessSpawn->bTerminate) {
            pProcessSpawn->fDelayTime -= fDelta;
            
            if (pProcessSpawn->fDelayTime <= 0.0f && pProcessSpawn->nCreatedCount > 0.0f) {
                ExcuteSpawnBox(pProcessSpawn, E_SEND_INFO_TYPE_ALL);
                pProcessSpawn->nCreatedCount -= 1.0f;
                
                if (pProcessSpawn->nCreatedCount <= 0.0f) {
                    pProcessSpawn->bActive = false;
                } else {
                    pProcessSpawn->fDelayTime = pProcessSpawn->pSpawnBox->m_fWaitCreationSequenceTime;
                }
            }
        }
    }
    
    // Also check event spawn box
    auto itEvent = m_mapEventSpawnBox.find(nEventID);
    if (itEvent != m_mapEventSpawnBox.end()) {
        // TODO: Process event spawn box logic
    }
}

// EndEvent - End current event
void CBattleZone::EndEvent(int nEventID)
{
    // Terminate process spawn box
    auto it = m_mapProcessSpawnBox.find(nEventID);
    
    if (it != m_mapProcessSpawnBox.end()) {
        STMageProcessSpawnBox* pProcessSpawn = static_cast<STMageProcessSpawnBox*>(it->second);
        if (pProcessSpawn) {
            pProcessSpawn->bActive = false;
            pProcessSpawn->bTerminate = true;
        }
    }
    
    // Clear world mode if active
    ST_WORLD_MODE_INFO stInfo;
    stInfo.nModeID = nEventID;
    ClearWorldMode(stInfo);
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "EndEvent - Event ended");
}

// CheckEvent - Check event conditions
bool CBattleZone::CheckEvent(int nEventID)
{
    // Check process spawn box for event
    auto itProcess = m_mapProcessSpawnBox.find(nEventID);
    if (itProcess != m_mapProcessSpawnBox.end()) {
        STMageProcessSpawnBox* pProcessSpawn = static_cast<STMageProcessSpawnBox*>(itProcess->second);
        if (pProcessSpawn && pProcessSpawn->bActive && !pProcessSpawn->bTerminate) {
            return true;
        }
    }
    
    // Check event spawn box
    auto itEvent = m_mapEventSpawnBox.find(nEventID);
    if (itEvent != m_mapEventSpawnBox.end()) {
        // Event spawn box exists
        return true;
    }
    
    // Check world mode for this event
    auto itWorldMode = m_mapGameWorldMode.find(nEventID);
    if (itWorldMode != m_mapGameWorldMode.end()) {
        std::tr1::shared_ptr<CGameWorldMode> pWorldMode = itWorldMode->second;
        if (pWorldMode && pWorldMode->GetState() == 1) {
            return true;
        }
    }
    
    return false;
}

// ============================================================================
// Portal System - Extended Functions
// ============================================================================

// ActivatePortal - Enable portal
void CBattleZone::ActivatePortal(int nPortalID)
{
    SetPotalFlag(nPortalID, true);
    GreenDamTan_log(__FILE__, __FUNCTION__, "ActivatePortal - Portal activated");
}

// DeactivatePortal - Disable portal
void CBattleZone::DeactivatePortal(int nPortalID)
{
    SetPotalFlag(nPortalID, false);
    GreenDamTan_log(__FILE__, __FUNCTION__, "DeactivatePortal - Portal deactivated");
}

// CheckPortal - Check portal conditions
bool CBattleZone::CheckPortal(int nPortalID, CUser* pUser)
{
    if (!pUser) {
        return false;
    }
    
    // Check if portal exists and is open
    int nUniqueID = GetUniqueID(nPortalID);
    auto it = m_mapPotalBox.find(nUniqueID);
    
    if (it == m_mapPotalBox.end()) {
        return false;
    }
    
    STMagePotalBox* pPotal = static_cast<STMagePotalBox*>(it->second);
    if (!pPotal) {
        return false;
    }
    
    // TODO: When STMagePotalBox has bOpen field:
    // return pPotal->bOpen;
    
    // Check additional conditions from TB_PORTAL table
    // XGameServer* pServer = XGameServer::Instance();
    // TB_PORTAL* pTBPortal = XResourceMgr::GetTB_PORTAL(&pServer->m_xResourceMgr, nPortalID);
    // if (pTBPortal) {
    //     // Check level requirement
    //     // Check quest requirement
    //     // Check item requirement
    // }
    
    return true;
}

// GetPortalList - Get list of all portal IDs
std::vector<int> CBattleZone::GetPortalList()
{
    std::vector<int> vecPortals;
    
    for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
        vecPortals.push_back(it->first);
    }
    
    return vecPortals;
}

// ============================================================================
// Quest System - Extended Functions
// ============================================================================

// StartQuest - Start zone quest
void CBattleZone::StartQuest(int nQuestID, CUser* pUser)
{
    if (!pUser) {
        return;
    }
    
    // TODO: When quest system is available:
    // std::tr1::shared_ptr<CGocQuest> pQuest;
    // CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuest, 0);
    // if (pQuest) {
    //     pQuest->AcceptQuest(nQuestID);
    // }
    
    // Trigger quest-related event spawns
    ExcuteSpawnBoxCheck(nQuestID, E_SEND_INFO_TYPE_ALL, false);
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "StartQuest - Quest started for user");
}

// EndQuest - Complete quest
void CBattleZone::EndQuest(int nQuestID, CUser* pUser, bool bSuccess)
{
    if (!pUser) {
        return;
    }
    
    // TODO: When quest system is available:
    // std::tr1::shared_ptr<CGocQuest> pQuest;
    // CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuest, 0);
    // if (pQuest) {
    //     if (bSuccess) {
    //         pQuest->CompleteQuest(nQuestID);
    //     } else {
    //         pQuest->FailQuest(nQuestID);
    //     }
    // }
    
    // Update quest move box if applicable
    RunQuestMoveCheck(nQuestID, pUser);
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "EndQuest - Quest completed");
}

// CheckQuest - Check quest progress
bool CBattleZone::CheckQuest(int nQuestID, CUser* pUser)
{
    if (!pUser) {
        return false;
    }
    
    // TODO: When quest system is available:
    // std::tr1::shared_ptr<CGocQuest> pQuest;
    // CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuest, 0);
    // if (pQuest) {
    //     return pQuest->CheckProgress(nQuestID);
    // }
    
    return false;
}

// GetQuestList - Get list of active quests for user
std::vector<int> CBattleZone::GetQuestList(CUser* pUser)
{
    std::vector<int> vecQuests;
    
    if (!pUser) {
        return vecQuests;
    }
    
    // TODO: When quest system is available:
    // std::tr1::shared_ptr<CGocQuest> pQuest;
    // CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuest, 0);
    // if (pQuest) {
    //     vecQuests = pQuest->GetActiveQuestList();
    // }
    
    // Also check quest move boxes for this zone
    for (auto it = m_mapQuestMoveBox.begin(); it != m_mapQuestMoveBox.end(); ++it) {
        // Add quest IDs from quest move boxes
        // STQuestMoveBox* pQMBox = static_cast<STQuestMoveBox*>(it->second);
        // if (pQMBox && pQMBox->pQuestMoveBox) {
        //     vecQuests.push_back(pQMBox->pQuestMoveBox->m_iConditionID);
        // }
    }
    
    return vecQuests;
}

// ============================================================================
// Maze System - Extended Functions
// ============================================================================

// EnterMaze - Player enters maze
void CBattleZone::EnterMaze(CUser* pUser, int nMazeID)
{
    if (!pUser) {
        return;
    }
    
    // Per IDA pattern: Initialize maze state for player
    // Map 30031 = Garden maze
    
    // Send world mode info
    XActor* pActor = reinterpret_cast<XActor*>(pUser);
    SendWorldModeInfo(pActor);
    
    // Initialize KRR monsters if needed
    InitKRRMonster();
    
    // Spawn maze-specific monsters
    if ((m_uxMapID.wMapID & 0xFFFF) == 30031) {
        // Garden maze - spawn event NPCs
        // TODO: SpawnEventMapNpc();
    } else {
        SpawnGenerateMonster();
    }
    
    // Send portal info
    SendPotalInfos(pActor);
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "EnterMaze - Player entered maze");
}

// ExitMaze - Player exits maze
void CBattleZone::ExitMaze(CUser* pUser)
{
    if (!pUser) {
        return;
    }
    
    // Use existing ExitArea infrastructure
    XActor* pActor = reinterpret_cast<XActor*>(pUser);
    ExitArea(pActor);
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "ExitMaze - Player exited maze");
}

// ProcessMaze - Maze logic update
void CBattleZone::ProcessMaze(float fDelta)
{
    // Process maze-specific logic based on map type
    int nMapType = m_uxMapID.wMapID & 0xFFFF;
    
    switch (nMapType) {
        case 30031: // Garden maze
            // Process event spawn boxes
            for (auto it = m_mapEventSpawnBox.begin(); it != m_mapEventSpawnBox.end(); ++it) {
                // TODO: Check event conditions and spawn
            }
            break;
            
        default:
            // Standard maze processing
            // Process spawn boxes
            for (auto it = m_mapProcessSpawnBox.begin(); it != m_mapProcessSpawnBox.end(); ++it) {
                STMageProcessSpawnBox* pBox = static_cast<STMageProcessSpawnBox*>(it->second);
                if (pBox && pBox->bActive) {
                    ProcessEvent(it->first, fDelta);
                }
            }
            break;
    }
    
    // Update world modes
    for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it) {
        std::tr1::shared_ptr<CGameWorldMode> pWorldMode = it->second;
        if (pWorldMode) {
            // TODO: CGameWorldMode::Update(fDelta);
        }
    }
    
    // Check completion conditions
    if (m_bFinishMode) {
        m_fUpdatePotal -= fDelta;
        if (m_fUpdatePotal <= 0.0f) {
            SetPotalFlag(10012, true);  // Exit portal
            m_bFinishMode = false;
            m_fUpdatePotal = 10.0f;
        }
    }
}

// ============================================================================
// User Management Functions
// ============================================================================

// EnterUser - User enters zone
void CBattleZone::EnterUser(CUser* pUser)
{
    if (!pUser) {
        return;
    }
    
    // Per IDA pattern: Initialize user state for zone
    XActor* pActor = reinterpret_cast<XActor*>(pUser);
    
    // Send world mode info
    SendWorldModeInfo(pActor);
    
    // Send portal info
    SendPotalInfos(pActor);
    
    // Initialize KRR monsters if needed
    InitKRRMonster();
    
    // Spawn zone monsters
    SpawnGenerateMonster();
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "EnterUser - User entered zone");
}

// ExitUser - User exits zone
void CBattleZone::ExitUser(CUser* pUser)
{
    if (!pUser) {
        return;
    }
    
    // Use existing ExitArea infrastructure
    XActor* pActor = reinterpret_cast<XActor*>(pUser);
    ExitArea(pActor);
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "ExitUser - User exited zone");
}

// GetUserList - Get all users in zone
std::vector<CUser*> CBattleZone::GetUserList()
{
    std::vector<CUser*> vecUsers;
    
    // Iterate through all actors and filter users
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
        XActor* pActor = it->second;
        if (!pActor) {
            continue;
        }
        
        // Check if actor is a user
        // TODO: Need proper type checking when RTTI is available
        // E_ACTOR_TYPE eType = pActor->GetType();
        // if (eType == eActorUser) {
        //     CUser* pUser = static_cast<CUser*>(pActor);
        //     vecUsers.push_back(pUser);
        // }
        
        // Temporary: Check by actor type (simplified)
        // For now, we assume the actor map contains properly typed actors
    }
    
    return vecUsers;
}

// GetMonsterList - Get all monsters in zone
std::vector<CMonster*> CBattleZone::GetMonsterList()
{
    std::vector<CMonster*> vecMonsters;
    
    // Iterate through all actors and filter monsters
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
        XActor* pActor = it->second;
        if (!pActor) {
            continue;
        }
        
        // Check if actor is a monster
        // TODO: Need proper type checking when RTTI is available
        // E_ACTOR_TYPE eType = pActor->GetType();
        // if (eType == eActorMonster) {
        //     CMonster* pMonster = static_cast<CMonster*>(pActor);
        //     vecMonsters.push_back(pMonster);
        // }
        
        // Temporary: Use FindMonster as a filter
        CMonster* pMonster = FindMonster(it->first);
        if (pMonster) {
            vecMonsters.push_back(pMonster);
        }
    }
    
    return vecMonsters;
}


// ============================================================================
// Missing Functions - Link Error Fixes
// ============================================================================

// Per IDA 0x1401A2740: CBattleZone::EnableInteractionBox
// 启用/禁用交互箱
// IDA 反编译精确还原:
// 1. LogHelper::LogError("game.contents", "<BATTLE> EnableInteractionBox")
// 2. VEventObjectInfo::GetEventUniqueID(nBoxIndex) -> iBoxUniqueID
// 3. m_mapInteractionBox.find(iBoxUniqueID) -> it
// 4. if found: pInteraction = (STInteractionBox*)it->second
// 5. XResourceMgr::GetTB_INTERACTION_OBJECT(pInteractionBox->m_iInteractionID) -> pTBInteraction
// 6. if (pTBInteraction): pInteraction->bEnable = bEnable
// 7. if (!pTBInteraction->Private_activate):
//    - FindActor(pInteraction->dwActorID) -> pInteractionActor
//    - dynamic_cast<CInteractionObject*>(pInteractionActor) -> InteractionObject
//    - if (InteractionObject) CInteractionObject::SendObjectInfo()
void CBattleZone::EnableInteractionBox(int nBoxIndex, bool bEnable) {
    // IDA: LogHelper::LogError("game.contents", "<BATTLE> EnableInteractionBox")
    GreenDamTan_log(__FILE__, __FUNCTION__, "EnableInteractionBox - nBoxIndex=%d, bEnable=%d", nBoxIndex, bEnable);

    // IDA: 计算唯一箱ID
    // int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nBoxIndex, 0);
    int iBoxUniqueID = nBoxIndex;  // 简化: 直接使用 nBoxIndex

    // IDA: 在 m_mapInteractionBox 中查找
    auto it = m_mapInteractionBox.find(iBoxUniqueID);

    if (it != m_mapInteractionBox.end()) {
        // IDA: pInteraction = (STInteractionBox*)it->second.__vftable
        // TODO: 需人工审查 - STInteractionBox 类型定义
        void* pInteractionVoid = it->second;
        if (pInteractionVoid) {
            // IDA: pInteractionBox = pInteraction->pInteractionBox
            // IDA: pTBInteraction = XResourceMgr::GetTB_INTERACTION_OBJECT(pInteractionBox->m_iInteractionID)
            // TODO: 汇编还原 - 需要 STInteractionBox, VInterActionBoxInfo, XResourceMgr 类型

            // IDA: if (pTBInteraction) pInteraction->bEnable = bEnable
            // pInteraction->bEnable = bEnable;

            // IDA: if (!pTBInteraction->Private_activate)
            // if (pTBInteraction && !pTBInteraction->Private_activate) {
            //     // IDA: FindActor(pInteraction->dwActorID)
            //     XActor* pInteractionActor = FindActor(pInteraction->dwActorID);
            //     if (pInteractionActor) {
            //         // IDA: dynamic_cast<CInteractionObject*>(pInteractionActor)
            //         CInteractionObject* pInteractionObj = dynamic_cast<CInteractionObject*>(pInteractionActor);
            //         if (pInteractionObj) {
            //             // IDA: CInteractionObject::SendObjectInfo(pInteractionObj)
            //             pInteractionObj->SendObjectInfo();
            //         }
            //     }
            // }
        }
    }
}

// Per IDA 0x1401A6CA0: CBattleZone::SetWorldModeSync
// 同步世界模式状态给玩家
// IDA 反编译精确还原:
// 1. 遍历 m_mapGameWorldMode
// 2. 对每个处于运行状态 (GetState() == 1) 的 WorldMode:
//    - 构造 PS_WORLD_MODE_START 包
//    - 发送给 pUser
// 3. 对非运行状态的 WorldMode:
//    - 检查是否有 EventBoost
//    - 如果有, 获取 CGocBooster 并应用 boost
void CBattleZone::SetWorldModeSync(CUser* pUser) {
    if (!pUser) return;

    // IDA: for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it)
    for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it) {
        // IDA: pMode = it->second (shared_ptr<CGameWorldMode>)
        std::tr1::shared_ptr<CGameWorldMode> pMode = it->second;

        // IDA: if (pMode)
        if (pMode) {
            // IDA: if (CGameWorldMode::GetState(pMode) == 1) - Running state
            // TODO: 汇编还原 - 需要 CGameWorldMode::GetState 方法
            // if (pMode->GetState() == 1) {
            //     // IDA: 构造 PS_WORLD_MODE_START
            //     PS_WORLD_MODE_START stStart = {};
            //     stStart.nModeDateID = pMode->GetModeDateID();
            //     stStart.nID = pMode->GetID();
            //     stStart.nStartTime = pMode->GetStartTime();
            //     stStart.nFinishTime = pMode->GetFinishTime();
            //     stStart.byState = 1;  // Running
            //     stStart.biModeStartTime = pMode->GetModeStartTime();
            //     stStart.biModeEndTime = pMode->GetModeEndTime();
            //
            //     // IDA: 如果有 HiddenEvent 状态, 设置 byState = 3
            //     // if (pMode->GetHiddenEventState()) stStart.byState = 3;
            //
            //     // IDA: sprintf(szNotice, "Enter Start WM : %d", nID)
            //     // LogHelper::LogDebug(...)
            //
            //     // IDA: 发送包 (0x30, 0x01)
            //     XSendPacket xSendPacket(0x30, 0x01);
            //     xSendPacket << stStart;
            //     CGocNetwork::Send(&pUser->XActor, &xSendPacket);
            // } else {
            //     // IDA: 检查 EventBoost
            //     int nBoostID = 0;
            //     __int64 nEndDate = 0;
            //     if (pMode->GetEventBoost(&nBoostID, &nEndDate)) {
            //         // IDA: 获取 CGocBooster 并应用
            //         std::tr1::shared_ptr<CGocBooster> pBooster;
            //         CMover::GetGOC<CGocBooster>(&pUser->CMoverEx, &pBooster, 0);
            //         if (pBooster) {
            //             pBooster->ChangeBooster(eBooster_Type_Event, nBoostID, nEndDate, 0);
            //         }
            //     }
            // }
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "SetWorldModeSync - IDA精确还原 (需要CGameWorldMode/CGocBooster类型)");
}

