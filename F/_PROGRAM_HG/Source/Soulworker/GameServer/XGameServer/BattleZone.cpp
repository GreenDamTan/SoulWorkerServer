#include "Soulworker/GameServer/XGameServer/BattleZone.h"
// Note: CMonster 和 CNpc 在 BattleZone.h 中前置声明
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

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
bool CBattleZone::Create(TUXMapID uxMapID, TB_MAZE_INFO* pMazeInfo) {
    // IDA 反编译逻辑:
    // 1. 检查 pMazeInfo 是否有效
    // 2. 从 XGameServer 获取资源
    // 3. 设置地图ID和最大用户数
    // 4. 设置区域大小
    // 5. 创建导航网格
    // 6. 初始化真空管理器
    // 7. 生成场景对象
    // 8. 生成怪物

    if (!pMazeInfo) {
        return false;
    }

    // TODO: 实现 - 需要访问 XGameServer 单例获取资源
    // XGameServer* pServer = XGameServer::Instance();
    // m_pObjectResource = pServer->GetResource(pMazeInfo->ID);

    // 设置地图ID
    m_uxMapID = uxMapID;

    // 设置最大用户数 (默认50，特定地图使用迷宫配置)
    m_nMaxUserCount = 50;
    // if (SWORD2(uxMapID.nMapID) == 30031) {
    //     m_nMaxUserCount = pMazeInfo->Maze_Enter_Count;
    // }

    // TODO: 设置区域大小
    // AREA_OBJECT::SetSize(&m_objectScanner, 0, 0,
    //     pMazeInfo->Maze_Size_X + 30000,
    //     pMazeInfo->Maze_Size_Y + 30000, 5000);

    // TODO: 创建导航网格
    // if (!CreateNavMesh(pMazeInfo->Server_SceneScript_File)) {
    //     LogHelper::LogError("game.contents",
    //         "Create error - Failed CreateNavMesh maze[ ID:%d ] ( %d )",
    //         pMazeInfo->ID, 69);
    //     return false;
    // }

    m_nEtcObjectID = 0;

    // 初始化真空管理器
    m_vaccumManager.Init(this, false);

    // 生成场景对象
    Generate();

    // TODO: 特定地图生成事件NPC或怪物
    // if (m_uxMapID.nMapID << 16 >> 48 == 30031) {
    //     SpawnEventMapNpc();
    // } else {
    //     SpawnGenerateMonster();
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::Create - partial implementation");
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
void CBattleZone::LoadComplete(XActor* pActor) {
    // IDA 反编译逻辑:
    // 1. 将 pActor 转换为 CUser
    // 2. 设置客户端加载完成标志
    // 3. 发送技能信息
    // 4. 处理 Akashic 记录
    // 5. 发送对象信息、传送信息、传送门信息
    // 6. 设置属性和状态
    // 7. 初始化物品冷却时间
    // 8. 发送 WorldMode 信息
    // 9. 检查 PVP 惩罚

    if (!pActor) {
        return;
    }

    // TODO: RTTI 转换
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) return;

    // 设置客户端加载完成
    // pUser->SetClientLoadComplete(true);

    // 发送技能信息
    // auto pSkill = pUser->GetGOC<CGocSkill>();
    // if (pSkill) { pSkill->SendPacketLoadSkill(); }

    // 发送对象信息
    // this->SendObjectInfo(pActor);
    // this->SendTransportationInfo(pActor);
    // this->SendPotalInfos(pActor);

    // 设置属性
    // auto pAttr = pUser->GetGOC<CGocAttribute>();
    // if (pAttr) {
    //     bool bFullStat = pUser->IsFullStat();
    //     int nWorldType = GetWorldType();
    //     pAttr->SetStartStatEnterWorld(nWorldType, bFullStat);
    //     pUser->SetFullStat(false);
    //     pAttr->SetStartRegStat(true);
    //     pAttr->SendOriginStatAll();
    //     pAttr->SendStatAll();
    // }

    // 初始化物品冷却时间
    // auto pInven = pUser->GetGOC<CGocInventory>();
    // if (pInven) {
    //     pInven->InitItemCoolTime();
    //     pInven->SendItemCoolTimeInfo();
    // }

    // 发送 WorldMode 信息
    // SendWorldModeInfo(pActor);
    // SetWorldModeSync(pUser);

    // 检查 PVP 惩罚
    // if (pUser->IsPVPPenalty()) {
    //     unsigned int nQuestID = pUser->GetActorID().GetQuestID();
    //     pUser->ApplyBuffSkill(0x63, nQuestID, 0);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::LoadComplete - partial implementation");
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

    // 通过 ThreadLocalData 创建怪物
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // CMonster* pMonster = pThreadData->CreateMonster(this, uxMazeSerialID, nMonsterID, &vPos, fRot);
    CMonster* pMonster = nullptr;  // TODO: 需要实现 ThreadLocalData

    if (!pMonster) {
        // LogHelper::LogDebug("game.contents",
        //     "CreateMonster error - Failed create monster[ MonsterID:%d ]", nMonsterID);
        GreenDamTan_log(__FILE__, __FUNCTION__, "CreateMonster error - Create failed");
        return nullptr;
    }

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

    // 设置生成箱ID
    // CMonster::SetSpawnBoxID(pMonster, nSpawnBoxID);

    // 设置父ID和OriginID
    // XActor* pParent = FindActor(uxParentID.dwActorID);
    // if (pParent) {
    //     pMonster->SetParentID(pParent->GetActorID());
    //     pMonster->SetOriginID(pParent->GetOriginID());
    // }

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
        // Per IDA: 遍历 m_mapGameWorldMode 查找匹配的 WorldMode
        // 如果找到，调用 ThreadLocalData::AppearEventMonster
        bool bAppear = false;

        // TODO: 实现 WorldMode 查找逻辑
        // for (auto it = m_mapGameWorldMode.begin(); it != m_mapGameWorldMode.end(); ++it) {
        //     CGameWorldMode* pWorldMode = it->second.get();
        //     if (pWorldMode && pWorldMode->GetState() == 1) {
        //         // 调用 AppearEventMonster
        //         bAppear = true;
        //         break;
        //     }
        // }

        if (!bAppear) {
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
    // int nTableID = pMonster->GetTableID();
    // XGameServer* pServer = XGameServer::Instance();
    // TB_MONSTER* pTBMonster = XResourceMgr::GetTB_MONSTER(&pServer->m_xResourceMgr, nTableID);
    TB_MONSTER* pTBMonster = nullptr;  // TODO: 需要实现资源管理器访问

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
        // Per IDA: 发送删除包到数据库
        // XSendDBPacket xSendDBPacket(0, 0xF3, 2);  // 主命令0xF3，子命令2
        // xSendDBPacket << dwQuestID;
        // xSendDBPacket << XOption::GetGroupID();
        // xSendDBPacket << GetChannel();
        // XGameServer::Instance()->SendDBGame(&xSendDBPacket);

        // LogHelper::LogInfo("game.system",
        //     "[KRR] Delete Monster %d / %d",
        //     actorID.dwActorID, nTableID);

        GreenDamTan_log(__FILE__, __FUNCTION__, "KRR monster deleted");
    }

    // 退出 Actor
    ExitActor(reinterpret_cast<XActor*>(pMonster));

    // 通过 ThreadLocalData 删除怪物
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // pThreadData->DeleteMonster(pMonster);
}

void CBattleZone::DieMonster(std::list<std::uint32_t>& listMonsterID, bool bForce) {
    // TODO: 汇编还原 - IDA 0x1401A5E60
}

void CBattleZone::DieMonsterAll(bool bForce) {
    // TODO: 汇编还原 - IDA 0x1401A71D0
}

void CBattleZone::MonsterDieForEvent(CMonster* pMonster, std::uint32_t dwKillerID) {
    // TODO: 汇编还原 - IDA 0x1401A6220
}

// Per IDA 0x1401A11E0: CBattleZone::CreateNpc
// 创建 NPC
CNpc* CBattleZone::CreateNpc(TUXMapID uxMazeSerialID, int nSectorID, unsigned int nNpcID,
                              XVec3 vPos, float fRot) {
    // IDA 反编译逻辑:
    // 1. 获取 TB_NPC 表数据，检查是否存在
    // 2. 通过 ThreadLocalData 创建 NPC
    // 3. 进入 Actor
    // 4. 设置碰撞（单向碰撞）

    // 获取 NPC 表数据
    // XGameServer* pServer = XGameServer::Instance();
    // TB_NPC* pTBNpc = XResourceMgr::GetTB_NPC(&pServer->m_xResourceMgr, nNpcID);
    // if (!pTBNpc) {
    //     return nullptr;
    // }

    // 通过 ThreadLocalData 创建 NPC
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // CNpc* pNpc = pThreadData->CreateNpc(this, uxMazeSerialID, nSectorID, nNpcID, &vPos, fRot, 0);
    CNpc* pNpc = nullptr;  // TODO: 需要实现 ThreadLocalData

    if (!pNpc) {
        return nullptr;
    }

    // 进入 Actor
    // Note: EnterActor 返回 void
    // TODO: 需要检查进入是否成功，失败时调用 DeleteNpc
    EnterActor(reinterpret_cast<XActor*>(pNpc));

    // 设置碰撞 - Per IDA: CMover::SetCollisionEnable(pNpc, 1, 0)
    // CMover::SetCollisionEnable(pNpc, true, false);  // 单向碰撞

    return pNpc;
}

// Per IDA 0x1401A1320: CBattleZone::DeleteNpc
// 删除 NPC
void CBattleZone::DeleteNpc(CNpc* pNpc) {
    // IDA 反编译逻辑:
    // 1. 如果 pNpc 有效，调用 ExitArea 退出区域
    // 2. 通过 ThreadLocalData 删除 NPC

    if (!pNpc) {
        return;
    }

    // 退出区域 - CNpc 继承自 CMoverEx，可以转换为 XActor
    ExitActor(reinterpret_cast<XActor*>(pNpc));

    // 通过 ThreadLocalData 删除 NPC
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // pThreadData->DeleteNpc(pNpc);
}

CAkashicObject* CBattleZone::CreateAkashicObject(TUXMapID uxMapID, int nTableID, XVec3 vPos, float fYaw, float fScale, E_SEND_INFO_TYPE eSendType) {
    // TODO: 汇编还原 - IDA 0x1401A1380
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

CInteractionObject* CBattleZone::CreateInteractionObject(STInteractionBox* pBox, void* pTBInteraction, XVec3& vPos, float fYaw) {
    // TODO: 汇编还原 - IDA 0x1401A1510
    return nullptr;
}

void CBattleZone::DeleteInteractionObject(CInteractionObject* pObject) {
    // TODO: 汇编还原 - IDA 0x1401A1620
}

void CBattleZone::ClickInteractionBox(int nBoxID, CUser* pUser) {
    // TODO: 汇编还原 - IDA 0x1401A28F0
}

// Per IDA 0x1401A3740: CBattleZone::ExitArea
// 玩家退出区域，发送切换地图包
void CBattleZone::ExitArea(XActor* pActor) {
    // IDA 反编译逻辑:
    // 1. RTTI 转换为 CUser
    // 2. 清除真空锁
    // 3. 设置状态为切换世界
    // 4. 获取逃跑位置
    // 5. 构造 PS_ENTER_MAP_REQ 包
    // 6. 获取队伍/公会信息
    // 7. 发送切换地图包

    if (!pActor) {
        return;
    }

    // RTTI 转换为 CUser
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) {
    //     return;
    // }

    // 清除真空锁
    // m_vaccumManager.ClearVaccumLock(pActor);

    // 设置状态为切换世界
    // pUser->SetState(eStateChangeWorld);

    // 获取逃跑位置
    // STPosInfo stPosInfo;
    // int nJumpID = 0;
    // int nMazeID = GetTBMapID();
    // XGameServer::Instance()->GetWorldResMgr()->GetMazeEscapePos(nMazeID, &nJumpID, &stPosInfo);

    // 构造切换地图请求包
    // PS_ENTER_MAP_REQ stEnterMap;
    // stEnterMap.dwActorID = pActor->GetActorID().GetQuestID();
    // stEnterMap.nJumpID = nJumpID;
    // stEnterMap.wMapID = stPosInfo.sWorldID;

    // 获取队伍/公会信息
    // auto pParty = pUser->GetGOC<CGocParty>();
    // auto pForce = pUser->GetGOC<CGocForce>();
    // if (pParty && pForce) {
    //     if (pParty->IsParty()) {
    //         stEnterMap.stPartyInfo.byGroupType = 1;
    //         stEnterMap.stPartyInfo.nID = pParty->GetPartyID();
    //     } else if (pForce->IsParty()) {
    //         stEnterMap.stPartyInfo.byGroupType = 2;
    //         stEnterMap.stPartyInfo.nID = pForce->GetPartyID();
    //     }
    // }

    // 发送速度检查日志
    // pUser->SendCheckSpeedLog();

    // 掉落 Ether 日志
    // auto pInven = pUser->GetGOC<CGocInventory>();
    // if (pInven) {
    //     pInven->DropEtherLog();
    // }

    // 发送切换地图包
    // XSendPacket xSendPacket(0xF2, 0x31);
    // xSendPacket << stEnterMap;
    // XGameServer::Instance()->GetControlSocket()->SendCheck(&xSendPacket);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::ExitArea - partial implementation");
}

bool CBattleZone::IsInSafetyZone(XActor* pActor) {
    // TODO: 汇编还原 - IDA 0x1401A3640
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

    // TODO: 获取事件对象映射
    // VMap<int,void*> ObjectMap;
    // XGameServer* pServer = XGameServer::Instance();
    // CFAutoSlimWriteLock lock(&pServer->m_rwMapLock);
    // ObjectMap = m_pObjectResource->GetMap(eEventObjectType_Box, 1, 0);

    // 遍历所有事件对象
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
    //             case eEventBoxType_MonsterSpawn:
    //                 // 处理怪物生成箱
    //                 // VMonsterSpawnInfo* pSpawn = (VMonsterSpawnInfo*)pBoxInfo;
    //                 // if (pSpawn->m_iCreationCondition == 2) {
    //                 //     // 创建 STMageProcessSpawnBox
    //                 // } else if (pSpawn->m_iCreationCondition == 1) {
    //                 //     m_listMonsterSpawnInfo.push_back(pSpawn);
    //                 // }
    //                 break;
    //             case eEventBoxType_CheckMonsterSpawn:
    //                 // 创建 STMageEventSpawnBox
    //                 break;
    //             case eEventBoxType_CheckSceneDirecting:
    //                 // 创建 STMagePotalBox
    //                 break;
    //             case eEventBoxType_Portal:
    //                 // 添加到 m_mapCommonPostionBox
    //                 break;
    //             case eEventBoxType_LuaFunction:
    //                 // 处理交互对象或真空组
    //                 break;
    //             case eEventBoxType_InteractionObjectBox:
    //                 // 创建 STQuestMoveBox
    //                 break;
    //             case eEventBoxType_PersonalShopArea:
    //                 // 添加到 m_mapSafetyZone
    //                 break;
    //         }
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::Generate - partial implementation");
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

    // TODO: 获取事件对象映射
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

    GreenDamTan_log(__FILE__, __FUNCTION__, "CBattleZone::SpawnGenerateMonster - partial implementation");
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
            //                     SetWorldModeBoostAll(*pClearBoosters[j], false);
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

void CBattleZone::ClearWorldMode(ST_WORLD_MODE_INFO& stInfo) {
    // TODO: 汇编还原 - IDA 0x1401A53C0
}

void CBattleZone::SyncWorldMode(ST_WORLD_MODE_INFO_VEC& stInfoVec) {
    // TODO: 汇编还原 - IDA 0x1401A5500
}

void CBattleZone::CompleteWorldMode(PS_WORLD_MODE_COMPLETE& stComplete, std::uint32_t dwKillerID) {
    // TODO: 汇编还原 - IDA 0x1401A8650
}

void CBattleZone::UpdateWorldMode(PS_WORLD_MODE_UPDATE& stUpdate) {
    // TODO: 汇编还原 - IDA 0x1401A86B0
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

void CBattleZone::ProcessMonsterQuest(XActor* pAttacker, std::uint32_t nMonsterID) {
    // TODO: 汇编还原 - IDA 0x1401A4410
}

void CBattleZone::RunQuestMoveCheck(int nQuestID, CUser* pUser) {
    // TODO: 汇编还原 - IDA 0x1401A6FB0
}

void CBattleZone::SendPotalInfos(XActor* pActor) {
    // TODO: 汇编还原 - IDA 0x1401A6490
}

void CBattleZone::SetPotalFlag(int nIndex, bool bFlag) {
    // TODO: 汇编还原 - IDA 0x1401A6760
}

void CBattleZone::UpdatePotalFlag(int nIndex) {
    // TODO: 汇编还原 - IDA 0x1401A87F0
}

void CBattleZone::ShowBattleZoneInfo(CUser* pUser) {
    // TODO: 汇编还原 - IDA 0x1401A77A0
}

bool CBattleZone::CreateNavMesh(const char* szPath) {
    // TODO: 汇编还原 - IDA 0x1401A2200
    return false;
}

DohHavokNavMeshInstance* CBattleZone::GetNavMeshInstance() {
    // IDA 0x1401ACF40 - 简单返回成员
    // TODO: 需人工审查 - 需要确认 XDistrict::m_pNavMeshInstance 偏移
    return nullptr;
}

int CBattleZone::GetWorldType() {
    // IDA 0x1401ADC90 - 简单返回成员
    // TODO: 需人工审查 - 需要确认实际的 world type 成员
    return 0;
}

CRespawnManager* CBattleZone::GetRespawnManager() {
    // IDA 0x140724710 - 简单返回 &m_respawnManager
    return &m_respawnManager;
}

CVaccumManager* CBattleZone::GetVaccumManager() {
    // IDA 0x140624100 - 简单返回 &m_vaccumManager
    return &m_vaccumManager;
}

void CBattleZone::AppearEventMonster(int nType1, bool bFlag1, int nType2, bool bFlag2, int nType3, bool bFlag3) {
    // TODO: 汇编还原 - IDA 0x1401A7C60
}

void CBattleZone::SetWorldModeBoostAll(int nIndex, bool bFlag) {
    // TODO: 汇编还原 - IDA 0x1402D0820
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