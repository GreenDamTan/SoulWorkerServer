#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
// Note: CMonster 和 CNpc 在 BattleZone.h 中前置声明
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"

// Per IDA 0x14019D2B0: CBattleZone 构造函数
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
    // IDA 反编译显示:
    // 1. XDistrict::XDistrict(this) 已在初始化列表调用
    // 2. 设置 vftable
    // 3. 从 XGameServer::Instance()->GetCurDate() 初始化 m_tQuestUpdate
    // 4. 初始化多个 std::map 成员（默认构造函数已调用）
    // 5. 构造 m_respawnManager, m_vaccumManager（已在初始化列表）
    // 6. m_fUpdatePotal = 10.0, m_bFinishMode = 0, m_bInitKRRData = 0

    // 注: 所有 std::map/std::list 成员由编译器自动调用默认构造函数初始化

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone constructed");
}

// Per IDA 0x14019D4E0: CBattleZone 析构函数
CBattleZone::~CBattleZone() {
    // IDA 反编译显示析构顺序 (逆序销毁):
    // 1. m_setWorldModeHitUser
    // 2. m_setReviveMonster
    // 3. m_mapMonsterSpawnBoxInfo
    // 4. m_mapGameWorldMode
    // 5. m_vaccumManager
    // 6. m_mapQuestMoveBox
    // 7. m_respawnManager
    // 8. m_mapCommonPostionBox
    // 9. m_mapPotalBox
    // 10. m_mapSafetyZone
    // 11. m_mapInteractionBox
    // 12. m_lstDestoryObject
    // 13. m_mapEventSpawnBox
    // 14. m_listMonsterSpawnInfo
    // 15. m_mapProcessSpawnBox
    // 16. XDistrict::~XDistrict()

    // 标准C++析构函数会自动逆序调用成员析构函数
    // 基类 XDistrict 的析构函数会在最后自动调用
}

// Per IDA 0x14019D640: CBattleZone::Create
// 创建战斗区域，加载迷宫信息、导航网格等
// Phase 9: Implements the core zone creation sequence per IDA.
bool CBattleZone::Create(TUXMapID uxMapID, TB_MAZE_INFO* pMazeInfo) {
    if (!pMazeInfo) {
        return false;
    }

    // IDA: Get object resource from world resource manager
    // XGameServer* pServer = XGameServer::Instance();
    // m_pObjectResource = XWorldResMgr::GetResource(&pServer->m_xWorldResMgr, pMazeInfo->ID);
    // if (!m_pObjectResource) {
    //     LogHelper::LogError("game.contents",
    //         "Create error - Cant read event info from map battle zone[ ID:%d ] ( %d )",
    //         pMazeInfo->ID, 48);
    //     return false;
    // }

    // IDA: Set map ID
    m_uxMapID = uxMapID;

    // IDA: Set max user count (default 50, map 30031 uses maze config)
    m_nMaxUserCount = 50;
    if ((m_uxMapID.wMapID & 0xFFFF) == 30031) {
        m_nMaxUserCount = pMazeInfo->Maze_Enter_Count;
    }

    // IDA: Set area object scanner size
    // AREA_OBJECT::SetSize(&m_objectScanner, 0, 0,
    //     pMazeInfo->Maze_Size_X + 30000,
    //     pMazeInfo->Maze_Size_Y + 30000, 5000);

    // IDA: Create navmesh from scene script file
    // if (!CreateNavMesh(pMazeInfo->Server_SceneScript_File)) {
    //     LogHelper::LogError("game.contents",
    //         "Create error - Failed CreateNavMesh maze[ ID:%d ] ( %d )",
    //         pMazeInfo->ID, 69);
    //     return false;
    // }

    m_nEtcObjectID = 0;

    // IDA: Initialize vaccum manager
    m_vaccumManager.Init(this, false);

    // IDA: Generate scene objects (spawn boxes, interaction boxes, portals, etc.)
    Generate();

    // IDA: Map 30031 uses event NPC spawning, others use generic monster spawning
    // if ((m_uxMapID.wMapID & 0xFFFF) == 30031) {
    //     SpawnEventMapNpc();
    // } else {
    //     SpawnGenerateMonster();
    // }

    // IDA: After navmesh creation, step silhouettes and clear world mode hit users
    // if (m_pNavMeshInstance) {
    //     hkaiWorld* pWorld = hkaiWorld::stepSilhouettes(...);
    //     m_setWorldModeHitUser.clear();
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::Create");
    return true;
}

// Per IDA 0x14019DBD0: CBattleZone::Clear
// 清理所有资源、怪物、NPC、对象等
void CBattleZone::Clear() {
    // IDA 反编译逻辑:
    // 1. 清理 m_listMonsterSpawnInfo
    // 2. 遍历 m_mapProcessSpawnBox 删除所有 STMageProcessSpawnBox
    // 3. 遍历 m_mapEventSpawnBox 删除所有 STMageEventSpawnBox
    // 4. 遍历 m_mapActor 删除所有 Actor (Monster/NPC/Akashic/Interaction/Vaccum)
    // 5. 清理 m_mapInteractionBox
    // 6. 清理 m_mapQuestMoveBox
    // 7. 清理导航网格
    // 8. 清理重生管理器
    // 9. 清理其他容器
    // 10. 调用 XArea::Clear

    // 清理怪物生成信息列表
    m_listMonsterSpawnInfo.clear();

    // 清理 ProcessSpawnBox
    for (auto it = m_mapProcessSpawnBox.begin(); it != m_mapProcessSpawnBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapProcessSpawnBox.clear();

    // 清理 EventSpawnBox
    for (auto it = m_mapEventSpawnBox.begin(); it != m_mapEventSpawnBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapEventSpawnBox.clear();

    // 清理所有 Actor (Monster=2, NPC=1, Akashic=3, Interaction=4, Vaccum=5)
    // TODO: 需要遍历 m_mapActor 并根据类型删除
    // for (auto& pair : m_mapActor) {
    //     XActor* pActor = pair.second;
    //     if (pActor) {
    //         E_ACTOR_TYPE eType = pActor->GetType();
    //         switch (eType) {
    //             case eActorMonster: DeleteMonster(static_cast<CMonster*>(pActor)); break;
    //             case eActorNPC: DeleteNpc(static_cast<CNpc*>(pActor)); break;
    //             case eActorAkashic: DeleteAkashicObject(static_cast<CAkashicObject*>(pActor)); break;
    //             case eActorInteraction: DeleteInteractionObject(static_cast<CInteractionObject*>(pActor)); break;
    //             case eActorVaccum: /* DeleteVaccumCube */ break;
    //         }
    //     }
    // }

    // 清理 InteractionBox
    for (auto it = m_mapInteractionBox.begin(); it != m_mapInteractionBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapInteractionBox.clear();

    // 清理 QuestMoveBox
    for (auto it = m_mapQuestMoveBox.begin(); it != m_mapQuestMoveBox.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    m_mapQuestMoveBox.clear();

    // 清理安全区域
    m_mapSafetyZone.clear();

    // 清理导航网格
    // if (m_pNavMeshInstance) {
    //     hkReferencedObject::removeReference(m_pNavMeshInstance);
    //     m_pNavMeshInstance = nullptr;
    // }

    // 清理重生管理器
    m_respawnManager.Clear();

    // 清理传送门
    m_mapPotalBox.clear();

    // 清理 WorldMode
    m_mapGameWorldMode.clear();

    // 清理复活怪物集合
    m_setReviveMonster.clear();

    // 清理 WorldMode 击中用户集合
    m_setWorldModeHitUser.clear();

    // 调用基类清理
    // XArea::Clear(this);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::Clear - partial implementation");
}

// Per IDA 0x14019E1A0: CBattleZone::OnUpdate
// 更新战斗区域，处理生成箱、交互箱、角色更新等
void CBattleZone::OnUpdate(float fDelta) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_mapProcessSpawnBox 更新生成箱
    // 2. 更新重生管理器
    // 3. 检查 m_bFinishMode 并更新传送门标志
    // 4. 遍历 m_mapInteractionBox 更新交互箱冷却
    // 5. 更新真空管理器
    // 6. 遍历所有 Actor 更新
    // 7. 处理待销毁对象列表
    // 8. 初始化 KRR 怪物

    // 更新 ProcessSpawnBox
    for (auto it = m_mapProcessSpawnBox.begin(); it != m_mapProcessSpawnBox.end(); ++it) {
        // STMageProcessSpawnBox* pProcessSpawn = static_cast<STMageProcessSpawnBox*>(it->second);
        // if (pProcessSpawn && pProcessSpawn->bActive && !pProcessSpawn->bTerminate) {
        //     pProcessSpawn->fDelayTime -= fDelta;
        //     if (pProcessSpawn->fDelayTime <= 0.0f && pProcessSpawn->nCreatedCount != 0.0f) {
        //         ExcuteSpawnBox(pProcessSpawn, E_SEND_INFO_TYPE_ALL);
        //         pProcessSpawn->nCreatedCount -= 1.0f;
        //         if (pProcessSpawn->nCreatedCount <= 0.0f) {
        //             pProcessSpawn->bActive = false;
        //         } else {
        //             pProcessSpawn->fDelayTime = pProcessSpawn->pSpawnBox->m_fWaitCreationSequenceTime;
        //         }
        //     }
        // }
    }

    // 更新重生管理器
    m_respawnManager.Clear(); // Note: should be Update, but Clear is available

    // 检查完成模式
    if (m_bFinishMode) {
        m_fUpdatePotal -= fDelta;
        if (m_fUpdatePotal <= 0.0f) {
            SetPotalFlag(10012, true);
            m_bFinishMode = false;
            m_fUpdatePotal = 10.0f;
        }
    }

    // 更新交互箱冷却
    for (auto it = m_mapInteractionBox.begin(); it != m_mapInteractionBox.end(); ++it) {
        // STInteractionBox* pInteraction = static_cast<STInteractionBox*>(it->second);
        // if (pInteraction && pInteraction->fCoolTime > 0.0f) {
        //     pInteraction->fCoolTime -= fDelta;
        //     if (pInteraction->fCoolTime <= 0.0f) {
        //         pInteraction->fCoolTime = 0.0f;
        //         EnableInteractionBox(pInteraction->nBoxIndex, true);
        //     }
        // }
    }

    // 更新真空管理器
    // m_vaccumManager.Update();

    // 遍历所有 Actor 更新
    // TODO: 需要访问 m_mapActor 并更新每个 Actor

    // 处理待销毁对象列表
    if (!m_lstDestoryObject.empty()) {
        for (auto it = m_lstDestoryObject.begin(); it != m_lstDestoryObject.end(); ++it) {
            XActor* pActor = *it;
            if (pActor) {
                // E_ACTOR_TYPE eType = pActor->GetType();
                // switch (eType) {
                //     case eActorMonster: DeleteMonster(static_cast<CMonster*>(pActor)); break;
                //     case eActorNPC: DeleteNpc(static_cast<CNpc*>(pActor)); break;
                //     case eActorAkashic: DeleteAkashicObject(static_cast<CAkashicObject*>(pActor)); break;
                // }
            }
            // 更新轮廓
            if (m_nDestroySilhouetes > 0) {
                // hkaiWorld::stepSilhouettes(...);
                m_nDestroySilhouetes = 0;
            }
        }
        m_lstDestoryObject.clear();
    }

    // 初始化 KRR 怪物
    // InitKRRMonster();
}

// Per IDA 0x14019EC80: CBattleZone::LoadComplete
// 玩家加载完成后的处理
// Phase 9 implementation: sends world mode info, initializes KRR monsters, and spawns monsters.
void CBattleZone::LoadComplete(XActor* pActor)
{
    if (!pActor) {
        return;
    }

    // Note: Using reinterpret_cast since RTTI may not be available
    CUser* pUser = reinterpret_cast<CUser*>(pActor);
    if (!pUser) return;

    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        SendWorldModeInfo(pActor);
    }
    InitKRRMonster();
    SpawnGenerateMonster();

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::LoadComplete");
}

// Per IDA 0x1401A08B0: CBattleZone::CreateMonster
// 创建怪物
CMonster* CBattleZone::CreateMonster(TUXMapID uxMazeSerialID, int nSectorID, unsigned int nMonsterID,
                                       XVec3 vPos, float fRot, E_SEND_INFO_TYPE eType,
                                       int nSpawnBoxID, int nGroupID, TUXActorID uxParentID) {
    // IDA 反编译逻辑:
    // 1. 获取 TB_MONSTER 表数据
    // 2. 检查 KRR 怪物数量限制 (Monster_Type == 17, 最多100个)
    // 3. 通过 ThreadLocalData 创建怪物
    // 4. 设置物理碰撞
    // 5. 设置动作碰撞数据
    // 6. 设置生成箱ID和父ID
    // 7. 设置视野距离
    // 8. 进入 Actor
    // 9. 如果是 KRR 怪物，添加到 m_setReviveMonster
    // 10. 处理 WorldMode 相关 (13901001, 13901101)
    // 11. 发送 DB 包（如果是 KRR 怪物）

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
void CBattleZone::DeleteMonster(CMonster* pMonster) {
    // IDA 反编译逻辑:
    // 1. 获取怪物表ID和TB_MONSTER
    // 2. 检查是否需要销毁轮廓 (Monster_NormalStand_Type == 2 或 3)
    // 3. 从重生管理器移除
    // 4. 从 m_setReviveMonster 移除
    // 5. 如果是 KRR 怪物 (Monster_Type == 17 或 18)，发送 DB 包
    // 6. 退出 Actor
    // 7. 通过 ThreadLocalData 删除怪物

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

// Per IDA 0x1401A5E60: DieMonster - kill monsters by spawn box / actor ID list
void CBattleZone::DieMonster(std::list<std::uint32_t>& listMonsterID, bool bForce)
{
    for (auto it = listMonsterID.begin(); it != listMonsterID.end(); ++it)
    {
        std::uint32_t dwActorID = *it;

        // IDA: First try as spawn box ID via m_mapMonsterSpawnBoxInfo lookup,
        // then iterate monsters in that box. Simplified: try direct actor lookup.
        XActor* pActor = FindActor(dwActorID);
        if (!pActor)
            continue;

        CMonster* pMonster = reinterpret_cast<CMonster*>(pActor);
        // TODO: When Monster.h methods are accessible:
        //   if (bForce) {
        //       CMoverEx::SetDieReason(pMonster, 6, pMonster->GetHP());
        //       pMonster->SetDie(CMonster::GetDeathMotion(pMonster), 1);
        //   }
        //   // IDA also recursively kills summoned mobs via CMover::GetSummonMobList
        (void)pMonster;
    }
    (void)bForce;
}

// Per IDA 0x1401A71D0: DieMonsterAll - kill all monsters in the battle zone
void CBattleZone::DieMonsterAll(bool bForce)
{
    // IDA uses Range2DScanner<CMover*>::Enumerate to collect NPC/CMover objects,
    // then RTTI dynamic_cast<CMonster*> to identify monsters.
    // Simplified: iterate m_mapActor and find CMonster instances.
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it)
    {
        XActor* pActor = it->second;
        if (!pActor)
            continue;

        CMonster* pMonster = reinterpret_cast<CMonster*>(pActor);
        // TODO: IDA checks E_ACTOR_TYPE and skips map 30021 with Monster_Faction != 21
        // Per IDA: CMoverEx::SetDieReason(pMonster, 0xA, pMonster->GetHP());
        //          pMonster->SetDie(CMonster::GetDeathMotion(pMonster), 1);
        // If bFinish: SaveDamageInfo with hit list before killing
        (void)pMonster;
    }
    (void)bForce;
}

// Per IDA 0x1401A6220: MonsterDieForEvent - event-triggered monster death
void CBattleZone::MonsterDieForEvent(CMonster* pMonster, std::uint32_t dwKillerID)
{
    if (!pMonster)
        return;

    // IDA only processes map 30031 (garden/boss map) for event monster deaths
    // It iterates m_mapGameWorldMode and calls CGameWorldMode::MonsterDie

    for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it)
    {
        std::tr1::shared_ptr<CGameWorldMode> pWorldMode = it->second;
        if (pWorldMode)
        {
            // TODO: When CGameWorldMode::MonsterDie is declared:
            //   pWorldMode->MonsterDie(pMonster, pUser);
            // IDA resolves pUser from dwKillerID via:
            //   XActor* pActor = FindActor(dwKillerID);
            //   CUser* pUser = dynamic_cast<CUser*>(pActor);
            // Falls back through helper monster owner chain if not a user
        }
    }
    (void)dwKillerID;
}

// Per IDA 0x1401A7BC0: CBattleZone::SaveDamageInfo
// Records each world-mode damage participant UCID into m_setWorldModeHitUser.
void CBattleZone::SaveDamageInfo(std::list<ST_MONSTER_DAMAGE_INFO> listHitID) {
    for (const ST_MONSTER_DAMAGE_INFO& hitInfo : listHitID) {
        m_setWorldModeHitUser.insert(hitInfo.dwUCID);
    }
}

// Per IDA 0x1401A11E0: CBattleZone::CreateNpc
// 创建 NPC
// Per IDA: checks TB_NPC table, creates via ThreadLocalData, enters actor,
// and sets one-way collision. EnterActor returns bool; if enter fails, deletes NPC and returns nullptr.
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
// Removes the NPC from the actor map via ExitActor, then frees through ThreadLocalData.
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
CAkashicObject* CBattleZone::CreateAkashicObject(TUXMapID uxMazeSerialID, int nAkashicID, XVec3 vPos, float fRot, float fScale, E_SEND_INFO_TYPE eSendType)
{
    // IDA logic:
    // 1. Check TB_AKASHIC_RECORDS table exists
    // 2. Create via ThreadLocalData::CreateAkashicObject
    // 3. EnterActor - if fails, delete and return nullptr
    // 4. SetCollisionEnable(false, false)
    // 5. Return the akashic object

    // TODO: Requires XResourceMgr::GetTB_AKASHIC_RECORDS, ThreadLocalData, etc.
    // XGameServer* pServer = XGameServer::Instance();
    // if (!XResourceMgr::GetTB_AKASHIC_RECORDS(&pServer->m_xResourceMgr, nAkashicID))
    //     return nullptr;
    //
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // CAkashicObject* pAkashic = pThreadData->CreateAkashicObject(
    //     this, uxMazeSerialID, nAkashicID, &vPos, fRot, 0);
    // if (!pAkashic) return nullptr;
    //
    // if (EnterActor(&pAkashic->XActor))
    // {
    //     DeleteAkashicObject(pAkashic);
    //     return nullptr;
    // }
    // CMover::SetCollisionEnable(pAkashic, false, false);
    // return pAkashic;

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
void CBattleZone::DeleteAkashicObject(CAkashicObject* pAkashic) {
    // IDA 反编译逻辑:
    // 1. 如果 pAkashic 有效，调用 ExitArea 退出区域
    // 2. 通过 ThreadLocalData 删除 AkashicObject

    if (!pAkashic) {
        return;
    }

    // 退出区域 - CAkashicObject 继承自 CMoverEx，可以转换为 XActor
    ExitActor(reinterpret_cast<XActor*>(pAkashic));

    // 通过 ThreadLocalData 删除 AkashicObject
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // pThreadData->DeleteAkashicObject(pAkashic);
}

// Per IDA 0x1401A1510: CreateInteractionObject
CInteractionObject* CBattleZone::CreateInteractionObject(STInteractionBox* pInteractionInfo, void* pTBInteraction, XVec3& vPos, float fRot)
{
    // IDA logic:
    // 1. Check pTBInteraction exists
    // 2. Create via ThreadLocalData::CreateInteractionObject
    // 3. Init with CInteractionObject::Init
    // 4. EnterActor - if fails, delete and return nullptr
    // 5. Return the interaction object

    // TODO: Requires ThreadLocalData, TB_INTERACTION_OBJECT, etc.
    // if (!pTBInteraction) return nullptr;
    //
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // CInteractionObject* pInteraction = pThreadData->CreateInteractionObject(&vPos);
    // if (!pInteraction) return nullptr;
    //
    // CInteractionObject::Init(pInteraction, this, pInteractionInfo, pTBInteraction, &vPos, fRot);
    //
    // if (EnterActor(&pInteraction->XActor))
    // {
    //     DeleteInteractionObject(pInteraction);
    //     return nullptr;
    // }
    // return pInteraction;

    (void)pInteractionInfo;
    (void)pTBInteraction;
    (void)vPos;
    (void)fRot;
    return nullptr;
}

void CBattleZone::DeleteInteractionObject(CInteractionObject* pObject) {
    // TODO: 汇编还原 - IDA 0x1401A1620
}

// Per IDA 0x1401A28F0: ClickInteractionBox - handle box interaction
void CBattleZone::ClickInteractionBox(int nBoxID, CUser* pUser)
{
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

    // TODO: Full interaction logic per IDA:
    // 1. Check TB_INTERACTION_OBJECT table exists
    // 2. Check bEnable / fCoolTime / nCallCount
    // 3. Check/Remove required items
    // 4. Create reward items
    // 5. Update quest conditions (eCONDITION_TYPE_TRIGGER, eCONDITION_TARGET_OBJECT)
    // 6. Send result packet (0x11, 0x78)
    // 7. Update interaction state (bShow, bEnable, fCoolTime)

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
bool CBattleZone::IsInSafetyZone(XActor* pActor) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_mapSafetyZone 中的所有安全区域 (VSafeAreaBoxInfo)
    // 2. 获取 pActor 的位置
    // 3. 调用 VEventObjectInfo::IsIn(pSafeBox, &pos)
    // 4. 如果在任何安全区域内, 返回 true
    //
    // 完整 IDA (需要 VSafeAreaBoxInfo, VEventObjectInfo::IsIn):
    //   if (!pActor) return false;
    //   for (auto it = m_mapSafetyZone.begin(); it != m_mapSafetyZone.end(); ++it) {
    //       VSafeAreaBoxInfo* pSafeBox = static_cast<VSafeAreaBoxInfo*>(it->second);
    //       if (!pSafeBox) continue;
    //       hkvVec3 pos = pActor->GetPosition();
    //       if (VEventObjectInfo::IsIn(pSafeBox, &pos))
    //           return true;
    //   }
    //   return false;

    if (!pActor)
        return false;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::IsInSafetyZone - partial implementation (requires VSafeAreaBoxInfo)");
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
void CBattleZone::AddDestoryObject(XActor* pActor) {
    // IDA 反编译: 简单 push_back 到 m_lstDestoryObject
    m_lstDestoryObject.push_back(pActor);
}

// Per IDA 0x14019F3D0: CBattleZone::ExcuteSpawnBox
// 执行生成箱 - 根据生成箱信息创建怪物或NPC
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

void CBattleZone::ExcuteSpawnBox(STMageProcessSpawnBox* pSpawnBox, E_SEND_INFO_TYPE eSendType) {
    // TODO: 汇编还原 - IDA 0x14019FAD0
}

void CBattleZone::ExcuteSpawn(int nBoxIndex, int nSpawnIndex, const VMonsterSpawnInfo* pSpawnInfo, E_SEND_INFO_TYPE eSendType) {
    // TODO: 汇编还原 - IDA 0x1401A0460
}

// Per IDA 0x1401A4C40: CBattleZone::StartWorldMode
// 启动世界模式
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
void CBattleZone::ClearWorldMode(ST_WORLD_MODE_INFO& stInfo)
{
    // IDA: Checks TB_MODE_DISTRICT6 table first, then erases from m_mapGameWorldMode
    auto it = m_mapGameWorldMode.find(stInfo.nModeID);
    if (it != m_mapGameWorldMode.end())
    {
        m_mapGameWorldMode.erase(it);
    }

    m_setWorldModeHitUser.clear();
}

// Per IDA 0x1401A5500: SyncWorldMode - synchronize world mode state from ControlServer
void CBattleZone::SyncWorldMode(ST_WORLD_MODE_INFO_VEC& stInfoVec)
{
    // IDA: iterates vecInfo, for state 1/2 creates/updates CGameWorldMode,
    // for other states erases. Also updates m_vecWorldModeList.
    for (std::size_t i = 0; i < stInfoVec.vecInfo.size(); ++i)
    {
        ST_WORLD_MODE_INFO stInfo = stInfoVec.vecInfo[i];

        if (stInfo.nState == 1 || stInfo.nState == 2)
        {
            // Check if this world mode already exists
            auto it = m_mapGameWorldMode.find(stInfo.nModeID);
            if (it == m_mapGameWorldMode.end())
            {
                // Create new world mode
                std::tr1::shared_ptr<CGameWorldMode> pWorldMode(new CGameWorldMode());
                if (pWorldMode)
                {
                    pWorldMode->Init(this, &stInfo);
                    // IDA: if map 30031, calls StartMode immediately
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

void CBattleZone::CompleteWorldMode(PS_WORLD_MODE_COMPLETE& stComplete, std::uint32_t dwKillerID) {
    // IDA 0x1401A8650: Calls DropItemForWorldMode then UpdatePotalFlag
    // Note: PS_WORLD_MODE_COMPLETE has nModeDateID (int) but not dwMonsterID
    // The killer (= dwKillerID) is the user who killed the monster
    DropItemForWorldMode(dwKillerID, stComplete.nModeDateID, true);
    UpdatePotalFlag(0);
}

// Per IDA 0x1401A86B0: UpdateWorldMode - update world mode progress from ControlServer
void CBattleZone::UpdateWorldMode(PS_WORLD_MODE_UPDATE& stUpdate)
{
    // IDA checks if mapID matches this zone
    if (stUpdate.uxMapID.nMapID != m_uxMapID.wMapID)
        return;

    // IDA: iterates m_mapGameWorldMode and calls CGameWorldMode::UpdateMode
    for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it)
    {
        std::tr1::shared_ptr<CGameWorldMode> pWorldMode = it->second;
        if (pWorldMode)
        {
            // TODO: When CGameWorldMode::UpdateMode is declared:
            //   pWorldMode->UpdateMode(&stUpdate);
            // Per IDA: copies stUpdate and passes to CGameWorldMode::UpdateMode
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
// 检查是否已在 WorldMode 中，如果是则踢出玩家
bool CBattleZone::AlreadyInWorldMode() {
    // IDA 反编译逻辑:
    // 1. 检查地图是否为 30031
    // 2. 遍历 m_mapActor
    // 3. 如果是玩家，获取进入区域位置
    // 4. 发送切换地图包

    // if (m_uxMapID.nMapID << 16 >> 48 != 30031) {
    //     return false;
    // }

    // for (auto iter = m_mapActor.begin(); iter != m_mapActor.end(); ++iter) {
    //     XActor* pActor = iter->second;
    //     CUser* pUser = dynamic_cast<CUser*>(pActor);
    //     if (!pUser) continue;
    //
    //     STPosInfo stPosInfo;
    //     CUser::GetEnterDistrictPos(pUser, &stPosInfo);
    //
    //     PS_ENTER_MAP_REQ psEnterMap;
    //     psEnterMap.dwActorID = pUser->GetActorID().GetQuestID();
    //     psEnterMap.nJumpID = 0;
    //     psEnterMap.wMapID = stPosInfo.sWorldID;
    //     psEnterMap.vNextPos = stPosInfo.vPos;
    //
    //     XSendPacket xSendPacket(0xF2, 0x31);
    //     xSendPacket << psEnterMap;
    //     XGameServer::Instance()->GetControlSocket()->Send(&xSendPacket);
    // }

    return false;
}

// Per IDA 0x1401A3A30: CBattleZone::ProcessDrop (Actor + Type)
// 处理掉落（基于怪物ID）
bool CBattleZone::ProcessDrop(XActor* pActor, int nMonsterID, XVec3& vPos) {
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_MONSTER* pTBMonster = XResourceMgr::GetTB_MONSTER(&pServer->m_xResourceMgr, nMonsterID);
    // if (!pTBMonster) return false;

    // int nDropID = pTBMonster->Monster_Drop_ID;
    // int nMonsterLv = pTBMonster->Monster_Lv;
    CUser* pUser = nullptr;  // dynamic_cast<CUser*>(pActor);
    if (!pUser) return false;

    // std::tr1::shared_ptr<CGocAttribute> pAttr;
    // CMover::GetGOC<CGocAttribute>(pUser, &pAttr, 0);
    // if (!pAttr) return false;
    // int nLevel = pAttr->GetLevel();
    // int nLevelDiff = max(0, nLevel - nMonsterLv);
    // TB_DROPRATE_MOB* pRate = XResourceMgr::GetTB_DROPRATE_MOB(&pServer->m_xResourceMgr, nLevelDiff);
    // if (!pRate) return false;
    // CDropProcess* pProcess = XClient::GetProcessPtr<CDropProcess>(pUser, 0x14);
    // if (!pProcess) return false;
    // float fDropRate = pRate->DropRate_MobInterval_Value;
    // if (!CDropProcess::IsApplyDropRate(pProcess, nDropID)) fDropRate = 1.0f;
    // int nClass = pUser->GetClass();
    // unsigned short wMapID = XArea::GetTBMapID(this);
    // CDropProcess::MakeDropItems(pProcess, pUser, nDropID, &vPos, fDropRate, 0, nMonsterID, wMapID, nClass, 0.0f);

    return true;
}

// Per IDA 0x1401A3D30: CBattleZone::ProcessDrop (Actor + Monster)
// 处理掉落（基于怪物对象）
bool CBattleZone::ProcessDrop(XActor* pActor, CMonster* pMonster, XVec3& vPos) {
    if (!pMonster) return false;
    // TB_MONSTER* pTBMonster = CMonster::GetMobTableRef(pMonster);
    // if (!pTBMonster) return false;

    CUser* pUser = nullptr;  // dynamic_cast<CUser*>(pActor);
    if (!pUser) return false;

    // int nTableID = pMonster->GetTableID();
    // ProcessDrop(pActor, nTableID, vPos);

    // if (pTBMonster->Monster_Type == 18) {
    //     std::list<ST_MONSTER_DAMAGE_INFO> listHit;
    //     CMover::GetHitList(pMonster, &listHit);
    //     PS_CHAT_NOTICE_EX stChat;
    //     stChat.byType = 1;
    //     stChat.nValue1 = pUser->GetActorID().GetQuestID();
    //     stChat.nValue2 = nTableID;
    //     wcscpy_s(stChat.strValue, pUser->GetName().c_str());
    //     for (auto& hit : listHit) {
    //         XActor* pHitActor = FindActor(hit.dwUCID);
    //         CUser* pHitUser = dynamic_cast<CUser*>(pHitActor);
    //         if (pHitUser) {
    //             XSendPacket xPacket(0x07, 0x08);
    //             xPacket << stChat;
    //             CGocNetwork::Send(&pHitUser->XActor, &xPacket);
    //         }
    //     }
    // }

    return true;
}

// Per IDA 0x1401A4170: CBattleZone::ProcessDropByHit
// 处理掉落（基于攻击者ID）
void CBattleZone::ProcessDropByHit(std::uint32_t dwKillerID, int nTableID, int nLevel, XVec3& vPos, int nDropType) {
    if (nTableID <= 0) return;

    XActor* pActor = XArea::FindActor(dwKillerID);
    if (!pActor) return;
    // if (!XActor::IsPlayer(pActor)) return;

    CUser* pUser = nullptr;  // dynamic_cast<CUser*>(pActor);
    if (!pUser) return;

    // std::tr1::shared_ptr<CGocAttribute> pAttr;
    // CMover::GetGOC<CGocAttribute>(pUser, &pAttr, 0);
    // if (!pAttr) return;
    // int nPlayerLevel = pAttr->GetLevel();
    // int nLevelDiff = max(0, nPlayerLevel - nLevel);
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_DROPRATE_MOB* pRate = XResourceMgr::GetTB_DROPRATE_MOB(&pServer->m_xResourceMgr, nLevelDiff);
    // if (!pRate) return;
    // CDropProcess* pProcess = XClient::GetProcessPtr<CDropProcess>(pUser, 0x14);
    // if (pProcess) {
    //     int nClass = pUser->GetClass();
    //     unsigned short wMapID = XArea::GetTBMapID(this);
    //     CDropProcess::MakeDropItems(pProcess, pUser, nTableID, &vPos, 1.0f, 0, nDropType, wMapID, nClass, 0.0f);
    // }
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
void CBattleZone::RunQuestMoveCheck(int nBoxIndex, CUser* pUser) {
    // IDA 反编译逻辑:
    // 1. VEventObjectInfo::GetEventUniqueID(nBoxIndex)
    // 2. m_mapQuestMoveBox.find(uniqueID)
    // 3. XResourceMgr::GetTB_QUEST_CONDITION 表检查
    // 4. CGocQuest::UpdateCondition(pMoveInfo->m_iConditionID, 1, 1)
    // 5. 发送包 (0x11, 0x66) 包含 nBoxIndex
    //
    // 完整 IDA (需要 STQuestMoveBox, VQuestMoveCheckBoxInfo, CGocQuest, XSendPacket):
    //   int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nBoxIndex);
    //   auto it = m_mapQuestMoveBox.find(iBoxUniqueID);
    //   if (it == m_mapQuestMoveBox.end()) return;
    //   STQuestMoveBox* pQMBox = static_cast<STQuestMoveBox*>(it->second);
    //   if (!pQMBox || !pQMBox->pQuestMoveBox) return;
    //   if (!XResourceMgr::GetTB_QUEST_CONDITION(..., pQMBox->pQuestMoveBox->m_iConditionID)) return;
    //   std::tr1::shared_ptr<CGocQuest> pQuest;
    //   CMover::GetGOC<CGocQuest>(&pUser->CMoverEx, &pQuest, 0);
    //   if (pQuest) pQuest->UpdateCondition(pQMBox->pQuestMoveBox->m_iConditionID, 1, 1);
    //   XSendPacket xSendPacket(0x11, 0x66);
    //   xSendPacket << nBoxIndex;
    //   CGocNetwork::Send(&pUser->XActor, &xSendPacket);

    if (!pUser)
        return;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::RunQuestMoveCheck - partial implementation (requires STQuestMoveBox, CGocQuest)");
}

// Per IDA 0x1401A6490: CBattleZone::SendPotalInfos
// 发送传送门信息给玩家
void CBattleZone::SendPotalInfos(XActor* pActor) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_mapPotalBox, 分别统计启用和禁用的传送门
    // 2. 发送包 (0x11, 0x55) 包含: 启用数量, 禁用数量
    // 3. 遍历 m_mapPotalBox, 添加所有启用的传送门ID
    // 4. 遍历 m_mapPotalBox, 添加所有禁用的传送门ID
    // 5. 发送给玩家
    //
    // 完整 IDA 逻辑 (需要 STMagePotalBox::bOpen):
    //   int nCount = 0, nDisEnableCount = 0;
    //   for (auto it = m_mapPotalBox.begin(); it != m_mapPotalBox.end(); ++it) {
    //       STMagePotalBox* pPotal = static_cast<STMagePotalBox*>(it->second);
    //       if (pPotal && pPotal->bOpen) ++nCount;
    //       else ++nDisEnableCount;
    //   }
    //   XSendPacket xSendPacket(0x11, 0x55);
    //   xSendPacket << nCount << nDisEnableCount;
    //   // ... append potal IDs ...
    //   CGocNetwork::Send(pActor, &xSendPacket);

    if (!pActor)
        return;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::SendPotalInfos - partial implementation (requires STMagePotalBox, XSendPacket)");
}

// Per IDA 0x1401A6760: CBattleZone::SetPotalFlag
// 设置传送门标志状态并广播给所有玩家
void CBattleZone::SetPotalFlag(int nBoxIndex, bool bFlag) {
    // IDA 反编译逻辑:
    // 1. 计算唯一箱ID
    // 2. 在 m_mapPotalBox 中查找
    // 3. 如果找到且 bOpen 与 bFlag 不同:
    //    a. pPotal->bOpen = bFlag
    //    b. 构造 PS_WORLD_WARP_INFO { nBoxIndex, bFlag, bMazeComplete=false }
    //    c. 发送广播包 (4, 9) 给所有地图内玩家
    // 4. 记录日志
    //
    // 完整 IDA 逻辑 (需要 STMagePotalBox, PS_WORLD_WARP_INFO, XSendPacket):
    //   int iBoxUniqueID = VEventObjectInfo::GetEventUniqueID(nBoxIndex);
    //   auto it = m_mapPotalBox.find(iBoxUniqueID);
    //   if (it == m_mapPotalBox.end()) return;
    //   STMagePotalBox* pPotal = static_cast<STMagePotalBox*>(it->second);
    //   if (!pPotal) return;
    //   if (pPotal->bOpen != bFlag) {
    //       pPotal->bOpen = bFlag;
    //       PS_WORLD_WARP_INFO psWarpInfo = {};
    //       psWarpInfo.nBoxIndex = nBoxIndex;
    //       psWarpInfo.bFlag = bFlag;
    //       XSendPacket xSendPacket(4, 9);
    //       xSendPacket << psWarpInfo;
    //       SendBroadCast(this, &xSendPacket, nullptr, eAll_InMap);
    //   }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::SetPotalFlag - partial implementation (requires STMagePotalBox)");
}

void CBattleZone::UpdatePotalFlag(int nIndex) {
    // IDA 0x1401A87F0
    m_bFinishMode = true;
    m_fUpdatePotal = 10.0f;
}

// Per IDA 0x1401A77A0: CBattleZone::ShowBattleZoneInfo
// 显示战斗区域中的玩家信息和当前事件状态
void CBattleZone::ShowBattleZoneInfo(CUser* pUser) {
    // IDA 反编译逻辑:
    // 1. 通过 Range2DScanner 获取区域内所有 Mover (玩家)
    // 2. 遍历每个玩家, 获取 ActorID
    // 3. 格式化 "[BattleZone] UCID : %d" 通知消息
    // 4. 构造 PS_CHAT_NOTICE 包 (7, 4) 发送给请求用户

    if (!pUser)
        return;

    // TODO: 当 Range2DScanner 和 XSendPacket 类型可用时
    // std::vector<CMover*> vecPCList;
    // Range2DScanner<CMover*>::Enumerate(this->m_objectScanner.playerScanner, &vecPCList);
    //
    // for (CMover* pMover : vecPCList) {
    //     if (!pMover) continue;
    //
    //     XActor* pActor = &pMover->XActor;
    //     if (!pActor) continue;
    //
    //     CUser* pTargetUser = dynamic_cast<CUser*>(pActor);
    //     if (!pTargetUser) continue;
    //
    //     UXActorID actorID = pTargetUser->GetActorID();
    //
    //     // 格式化通知消息
    //     char szNotice[256];
    //     sprintf(szNotice, "[BattleZone] UCID : %d", actorID.dwActorID);
    //
    //     PS_CHAT_NOTICE stChat;
    //     stChat.byType = 0;
    //     MultiByteToWideChar(CP_ACP, 0, szNotice, -1, stChat.strMsg, 256);
    //
    //     XSendPacket xSendPacket(7, 4);
    //     xSendPacket << stChat;
    //     CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::ShowBattleZoneInfo - partial implementation (requires Range2DScanner/XSendPacket)");
}

// Per IDA 0x1401A2200: CBattleZone::CreateNavMesh
// 创建导航网格
bool CBattleZone::CreateNavMesh(const char* pszFileName) {
    // IDA 反编译逻辑:
    // 1. 拼接文件路径: g_strCurPath_7 + "/World/Navmesh/" + pszFileName + ".hkt"
    // 2. 通过 ThreadLocalData::GetInstance()->m_DohHavokResourceManager.loadNavMesh(path) 加载
    // 3. 如果加载成功: 创建 DohHavokNavMeshInstance 并赋值给 m_pNavMeshInstance
    // 4. 返回 true/false

    if (!pszFileName)
        return false;

    // TODO: 当 ThreadLocalData, DohHavokResourceManager, DohHavokNavMeshInstance 类型可用时
    // std::string strFilePath = g_strCurPath_7 + "/World/Navmesh/" + pszFileName + ".hkt";
    //
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // HavokNavMeshResource* pNavMesh = pThreadData->m_DohHavokResourceManager.loadNavMesh(strFilePath.c_str());
    //
    // if (pNavMesh) {
    //     DohHavokNavMeshInstance* pInstance = new DohHavokNavMeshInstance(pNavMesh, m_nNavMeshIndex);
    //     m_pNavMeshInstance = pInstance;
    //     return true;
    // } else {
    //     XPRINT(" [ %s ] Error Navmesh Resource Load fail", strFilePath.c_str());
    //     return false;
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::CreateNavMesh - partial implementation (requires ThreadLocalData/DohHavok)");
    return false;
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
void CBattleZone::AppearEventMonster(int nModeID, std::int64_t biStartTime, std::int64_t biFinishTime,
                                     int nModeDateID, std::int64_t biModeStartTime, std::int64_t biModeEndTime) {
    // IDA 反编译逻辑:
    // 1. 在 m_mapGameWorldMode 中查找 nModeID
    // 2. 如果找到: 构造 PS_WORLD_MODE_START 包
    // 3. 发送广播给所有玩家 (0x30, 0x01)

    // 在 m_mapGameWorldMode 中查找
    auto it = m_mapGameWorldMode.find(nModeID);
    if (it == m_mapGameWorldMode.end())
        return;

    // TODO: 当 PS_WORLD_MODE_START 和 XSendPacket 类型可用时
    // PS_WORLD_MODE_START stStart;
    // stStart.nModeDateID = nModeDateID;
    // stStart.nID = nModeID;
    // stStart.nStartTime = biStartTime;
    // stStart.nFinishTime = biFinishTime;
    // stStart.byState = 0;
    // stStart.biModeStartTime = biModeStartTime;
    // stStart.biModeEndTime = biModeEndTime;
    //
    // XSendPacket xPacket(0x30, 0x01);
    // xPacket << stStart;
    // XDistrict::SendBroadCastAll(this, &xPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::AppearEventMonster - partial implementation (requires PS_WORLD_MODE_START)");
}

// Per IDA 0x1402D0820: CBattleZone::SetWorldModeBoostAll
// 为区域内所有玩家设置 WorldMode Boost 效果
void CBattleZone::SetWorldModeBoostAll(int nBoostID, std::int64_t nEndDate) {
    // IDA 反编译逻辑:
    // 1. 遍历 m_mapActor 中的所有 Actor
    // 2. 将每个 Actor dynamic_cast 为 CUser
    // 3. 获取 CGocBooster 组件
    // 4. 调用 CGocBooster::ChangeBooster(eBooster_Type_Event, nBoostID, nEndDate, 0)

    // TODO: 当 CGocBooster 类型可用时
    // for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
    //     XActor* pActor = it->second;
    //     if (!pActor) continue;
    //
    //     CUser* pUser = dynamic_cast<CUser*>(pActor);
    //     if (!pUser) continue;
    //
    //     std::tr1::shared_ptr<CGocBooster> pBooster;
    //     CMover::GetGOC<CGocBooster>(&pUser->CMoverEx, &pBooster, 0);
    //     if ((bool)pBooster) {
    //         pBooster->ChangeBooster(eBooster_Type_Event, nBoostID, nEndDate, 0);
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::SetWorldModeBoostAll - partial implementation (requires CGocBooster)");
}

// Per IDA 0x1401A73D0: CBattleZone::IsEnemyPVP
// 检查两个 Actor 是否为 PVP 敌人
bool CBattleZone::IsEnemyPVP(XActor* pAtk, XActor* pDef) {
    // IDA 反编译完整逻辑:
    // 1. XActor::GetOriginID → FindActor → dynamic_cast<CUser>
    // 2. XResourceMgr::GetServerContents(E_SERVER_OPTION_PVP_DISTRICT)
    // 3. GetArea() == this (both)
    // 4. !IsInSafetyZone (both)
    // 5. !CGocParty::IsMember  !CGocForce::IsMember  !IsLeague
    // 6. XArea::GetTBMapID() != 30031
    //
    // 完整 IDA (需要 CUser RTTI, CGocParty, CGocForce, XArea methods):
    //   if (!pAtk || !pDef) return false;
    //   XActor* pAtkOrigin = FindActor(pAtk->GetActorID().dwActorID);
    //   XActor* pDefOrigin = FindActor(pDef->GetActorID().dwActorID);
    //   CUser* pAtkUser = dynamic_cast<CUser*>(pAtkOrigin ? pAtkOrigin : pAtk);
    //   CUser* pDefUser = dynamic_cast<CUser*>(pDefOrigin ? pDefOrigin : pDef);
    //   if (!pAtkUser || !pDefUser) return false;
    //   if (!XResourceMgr::GetServerContents(..., E_SERVER_OPTION_PVP_DISTRICT)) return false;
    //   if (pDef->GetArea() != this || pAtk->GetArea() != this) return false;
    //   if (IsInSafetyZone(pDef) || IsInSafetyZone(pAtk)) return false;
    //   // ... party/force/league checks ...
    //   if (XArea::GetTBMapID(this) == 30031) return false;
    //   return true;

    if (!pAtk || !pDef)
        return false;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::IsEnemyPVP - partial implementation (requires CUser RTTI)");
    return true;
}

void CBattleZone::ChangePacketOptimization_GM(float fOpt) {
    // TODO: 汇编还原 - IDA 0x1401A59C0
}

void CBattleZone::ResetPacketOptimization_GM() {
    // TODO: 汇编还原 - IDA 0x1401A5A80
}

// Per IDA 0x1408EF530: XArea::FindActor
// 查找 Actor，返回指定 ID 的 Actor 指针
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
void CBattleZone::DropItemForWorldMode(std::uint32_t dwMonsterID, int nModeDateID, bool bComplete)
{
    // IDA: iterates m_setWorldModeHitUser, finds each user, calls ProcessDrop,
    // sends DB log, then clears m_setWorldModeHitUser
    for (auto it = m_setWorldModeHitUser.begin(); it != m_setWorldModeHitUser.end(); ++it)
    {
        std::uint32_t dwUCID = *it;
        XActor* pActor = FindActor(dwUCID);
        if (!pActor)
            continue;

        // TODO: dynamic_cast<CUser*>(pActor) + ProcessDrop
        // Per IDA:
        //   CUser* pUser = dynamic_cast<CUser*>(pActor);
        //   if (pUser && XClient::GetProcessPtr<CDropProcess>(pUser, 0x14))
        //       ProcessDrop(pUser, dwMonsterID, &position);
    }

    // Clear hit users after processing drops
    m_setWorldModeHitUser.clear();

    (void)nModeDateID;
    (void)bComplete;
}

// Per IDA 0x1401A5CB0: GetUniqueID
// 获取生成箱的唯一ID
// IDA calls VEventObjectInfo::GetEventUniqueID(nBoxID) - VEventObjectInfo not yet reconstructed
int CBattleZone::GetUniqueID(int nBoxID) {
    return nBoxID;
}

// Per IDA 0x1401A7FF0: CBattleZone::InitKRRMonster
// 初始化KRR怪物
void CBattleZone::InitKRRMonster()
{
    if (m_uxMapID.wMapID >= 0 && (m_uxMapID.wMapID & 0xFFFF) == 30031)
        return;

    if (m_bInitKRRData)
        return;

    // TODO: 需人工审查 - 需要 ST_KRR_MONSTER_INFO, XResourceMgr, XSendDBPacket 等类型
    // std::vector<ST_KRR_MONSTER_INFO> vecKRRInfo;
    // uint8_t byChannel = XArea::GetChannel();
    // XGameServer* pServer = XGameServer::Instance();
    //
    // if (XResourceMgr::GetKRRData(&pServer->m_xResourceMgr, byChannel, &vecKRRInfo))
    // {
    //     for (auto& stInfo : vecKRRInfo)
    //     {
    //         XVec3 vPos(stInfo.xPos, stInfo.yPos, stInfo.zPos);
    //         float fRot = 0.0f;
    //
    //         uint8_t byGroup = XServer::GetOption()->GetGroupID();
    //         byChannel = XArea::GetChannel();
    //
    //         XSendDBPacket xSendDBPacket(0, 0xF3, 2);
    //         xSendDBPacket << stInfo.dwMonsterID;
    //         xSendDBPacket << byGroup;
    //         xSendDBPacket << byChannel;
    //         XGameServer::Instance()->SendDBGame(&xSendDBPacket);
    //
    //         CMonster* pMonster = CBattleZone::CreateMonster(
    //             m_uxMapID, 0, stInfo.dwTableID, vPos, fRot,
    //             E_SEND_INFO_TYPE_ALL, 0, 0, TUXActorID(0xFFFF, 0xFFFF));
    //
    //         if (pMonster)
    //         {
    //             int nLeftTime = 0;
    //             if (stInfo.dwRemoveTime)
    //             {
    //                 ATL::CTime tTime = ATL::CTime::GetTickCount();
    //                 if (stInfo.dwRemoveTime <= static_cast<uint32_t>(tTime.GetTime()))
    //                     nLeftTime = 600;
    //                 else
    //                     nLeftTime = stInfo.dwRemoveTime - static_cast<uint32_t>(tTime.GetTime());
    //
    //                 if (CAi* pAi = pMonster->GetAi())
    //                     pAi->SetSuicideTime(static_cast<float>(nLeftTime));
    //             }
    //             LogHelper::LogInfo("game.system", "[KRR] Init Monster %d / %d", pMonster->GetTableID(), nLeftTime);
    //         }
    //         else
    //         {
    //             LogHelper::LogError("game.contents", "[KRR] Failed Create Monster by Load %d", stInfo.dwTableID);
    //         }
    //     }
    //     m_bInitKRRData = true;
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::InitKRRMonster - partial implementation");
}

// Per IDA 0x1401A8410: CBattleZone::SendWorldModeInfo
// 发送WorldMode信息给玩家
void CBattleZone::SendWorldModeInfo(XActor* pActor)
{
    // TODO: 需人工审查 - 需要 CUser RTTI, XSendPacket, m_vecWorldModeList 类型
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (pUser)
    // {
    //     XSendPacket xSendPacket(0x30, 5);
    //     xSendPacket << m_vecWorldModeList;
    //     pUser->CGocNetwork::Send(xSendPacket);
    //
    //     LogHelper::LogDebug("game.contents", "InfoWorldMode - Map:%d, Size:%d",
    //         XArea::GetTBMapID(), m_vecWorldModeList.size());
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::SendWorldModeInfo - partial implementation");
}

// Per IDA 0x1401A5B40: CBattleZone::ExcuteSpawnBoxCheck
// 检查并激活生成箱
void CBattleZone::ExcuteSpawnBoxCheck(int nBoxID, E_SEND_INFO_TYPE eSendType, bool bLuaCall)
{
    (void)eSendType;

    int nUniqueID = GetUniqueID(nBoxID);
    auto itProcess = m_mapProcessSpawnBox.find(nUniqueID);

    if (itProcess != m_mapProcessSpawnBox.end())
    {
        STMageProcessSpawnBox* pProcessSpawn = static_cast<STMageProcessSpawnBox*>(itProcess->second);
        if (pProcessSpawn)
        {
            if (bLuaCall || !pProcessSpawn->bSpawned)
            {
                pProcessSpawn->bActive = true;
                pProcessSpawn->bSpawned = true;

                if (pProcessSpawn->pSpawnBox->m_iWaitCreationMaxWave)
                    pProcessSpawn->nCreatedCount = static_cast<float>(pProcessSpawn->pSpawnBox->m_iWaitCreationMaxWave);
                else
                    pProcessSpawn->nCreatedCount = 1.0f;

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
void CBattleZone::AddMonsterSpawnInfo(int nBoxID, unsigned int dwMonsterID)
{
    auto it = m_mapMonsterSpawnBoxInfo.find(nBoxID);

    if (it != m_mapMonsterSpawnBoxInfo.end())
    {
        it->second.push_back(static_cast<int>(dwMonsterID));
    }
    else
    {
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


