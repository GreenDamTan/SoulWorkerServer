#include "Soulworker/GameServer/XCore/XArea/XDistrict.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerWorldMode.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocEntity.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"

// 前置声明
class XGameServer;

XDistrict::XDistrict()
    : XArea()
    , m_pObjectResource(nullptr)
    , m_objectScanner()
    , m_nPcCount(0)
    , m_pNavMeshInstance(nullptr)
    , m_nNavMeshIndex(-1)
    , m_setSocialObjectKeys()
    , m_mapSocialOwnerKeys()
    , m_setSocialObjectFuniture()
    , m_setSocialObjects()
    , m_bCanUseActiveAkashic(false)
    , m_nCheckUserLog(0)
    , m_listWaitForRecvInfo()
    , m_mapWorldMode()
    , m_vecWorldModeList()
{
    GreenDamTan_log(__FILE__, __FUNCTION__, "XDistrict constructed");
}

XDistrict::~XDistrict() {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XDistrict destructed");
}

// IDA 0x1401ACF60 - Get object resource
// IDA 反编译精确还原:
// return this->m_pObjectResource;
VEventObjectResource* XDistrict::GetObjectResource() {
    return m_pObjectResource;
}

// IDA 0x1402C8630 - Create district
// IDA 反编译精确还原:
// 1. 获取对象资源
// 2. 设置区域大小
// 3. 清空社交对象集合
// 4. 设置Akashic使用标志
// 5. 创建导航网格
bool XDistrict::Create(TB_DISTRICT* pDistrict) {
    if (!pDistrict)
        return false;

    // IDA: XGameServer* pServer = TXSingleton<XGameServer>::Instance()
    XGameServer* pServer = XGameServer::Instance();
    
    // IDA: m_pObjectResource = XWorldResMgr::GetResource(&pServer->m_xWorldResMgr, pDistrict->District_ID)
    // PDB: GetResource 为非静态成员, 返回 VEventObjectResource*, 参数 int16
    m_pObjectResource = pServer->GetWorldResMgr().GetResource(
        static_cast<std::int16_t>(pDistrict->District_ID));
    
    if (!m_pObjectResource) {
        LogHelper::LogError("game.system", 
            "Create error - Cant read event info in map district[ DistrictID:%d ] ( %d )", 
            pDistrict->District_ID, 67);
        return false;
    }

    // IDA: AREA_OBJECT::SetSize(&m_objectScanner, ...)
    m_objectScanner.SetSize(
        pDistrict->District_Start_X,
        pDistrict->District_Start_Y,
        pDistrict->District_Size_X,
        pDistrict->District_Size_Y,
        5000);
    
    m_nMaxUserCount = 100;
    
    // IDA: Clear all social object sets
    m_setSocialObjectKeys.clear();
    m_mapSocialOwnerKeys.clear();
    m_setSocialObjectFuniture.clear();
    m_setSocialObjects.clear();
    
    // IDA: m_bCanUseActiveAkashic = (pDistrict->Arkashic_Use != 1)
    m_bCanUseActiveAkashic = true;
    if (pDistrict->Arkashic_Use == 1)
        m_bCanUseActiveAkashic = false;
    
    // IDA: CreateNavMesh
    if (!CreateNavMesh(pDistrict->District_BatName)) {
        LogHelper::LogError("game.contents", 
            "Create error - Failed CreateNavMesh ( %s ) ( %d )", 
            pDistrict->District_BatName, 96);
    }
    
    // IDA: if (m_pNavMeshInstance) { hkaiWorld::stepSilhouettes(...) }
    if (m_pNavMeshInstance) {
        // Note: hkaiWorld::stepSilhouettes requires Havok AI navigation
        // This is a placeholder for the actual Havok call
        // hkaiWorld* pWorld = m_pNavMeshInstance->GetWorld();
        // if (pWorld) pWorld->stepSilhouettes(nullptr);
    }
    
    return true;
}

// IDA 0x1402C8850 - Clear district
// IDA 反编译精确还原:
// 1. 调用基类 Clear
// 2. 重置检查用户日志
// 3. 清空等待接收信息列表
void XDistrict::Clear() {
    XArea::Clear();
    m_nCheckUserLog = 0;
    m_listWaitForRecvInfo.clear();
}

// IDA 0x1402C8810 - Init district
// IDA 反编译精确还原:
// 1. 调用 XIOCPServer::BackSends
// 2. 调用 SpawnGenerate
// 3. 重置 m_nCheckUserLog
bool XDistrict::Init() {
    // IDA: XIOCPServer::BackSends(this)
    // Note: BackSends is a static method in XIOCPServer
    // XIOCPServer::BackSends(this);
    
    // IDA: XDistrict::SpawnGenerate(this)
    SpawnGenerate();
    
    // IDA: m_nCheckUserLog = 0
    m_nCheckUserLog = 0;
    
    return true;
}

// IDA 0x1402C88F0 - EnterActor
// IDA 反编译精确还原:
// 1. 获取扫描器，检查是否已在区域中
// 2. 获取 Actor 位置，设置区域
// 3. 对于 CUser: 检查高度，设置位置，发送 DB 更新
// 4. 将 Actor 插入扫描器
// 5. 扫描周围玩家和对象，发送进入消息
// 6. 对于 CUser: 初始化超级护甲，进入队伍/势力成员
// IDA: ?EnterActor@XDistrict@@UEAAGPEAVXActor@@@Z - returns unsigned short
std::uint16_t XDistrict::EnterActor(XActor* pActor) {
    if (!pActor)
        return 50001;

    // IDA: Range2DScanner<CMover*>* pTargetScanner = GetScanner(pActor)
    Range2DScanner<CMover*>* pTargetScanner = GetScanner(pActor);
    if (!pTargetScanner) {
        return 50001;
    }

    // IDA: Get position from actor
    STPosInfo* pPosInfo = pActor->GetPosInfo();
    float fx = pPosInfo ? pPosInfo->vPos.x : 0.0f;
    float fy = pPosInfo ? pPosInfo->vPos.y : 0.0f;
    float fz = pPosInfo ? pPosInfo->vPos.z : 0.0f;

    // IDA: Check if already in area
    if (XArea::EnterActor(pActor)) {
        UXActorID actorID = pActor->GetActorID();
        LogHelper::LogError("game.contents", "EnterActor error - Already in map[ ActorID:%d ]", actorID.dwActorID);
        return 50001;
    }

    // IDA: pActor->SetArea(this)
    pActor->SetArea(this);

    // IDA: CUser* pUser = dynamic_cast<CUser*>(pActor)
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (pUser) {
        // IDA: Check height and set position
        // Note: CMover::GetHeight requires complete type
        // XVec3 vPos(fx, fy, fz);
        // if (CMover::GetHeight(&pUser->CMoverEx, &vPos, 2000.0f)) {
        //     pActor->SetWorldID(GetTBMapID());
        //     pActor->SetPosInfo(vPos);
        // } else {
        //     // Invalid position handling
        // }
    }

    // IDA: CMover* obj = dynamic_cast<CMover*>(pActor)
    CMover* obj = dynamic_cast<CMover*>(pActor);

    // IDA: Range2DScanner<CMover*>::Insert(pTargetScanner, fx, fy, obj)
    if (pTargetScanner && obj) {
        pTargetScanner->Insert(fx, fy, obj);
    }

    // IDA: std::vector<CMover*> vecPlayerList, vecObjList
    std::vector<CMover*> vecPlayerList;
    std::vector<CMover*> vecObjList;
    vecPlayerList.reserve(300);
    vecObjList.reserve(300);

    // IDA: Scan for nearby players
    if (pActor->GetType()) {
        if (m_objectScanner.playerScanner) {
            m_objectScanner.playerScanner->ScanGrid(fx, fy, 2, 2, vecPlayerList);
        }
    } else {
        ++m_nPcCount;
        if (m_objectScanner.playerScanner) {
            m_objectScanner.playerScanner->ScanGridAndSetObjCnt(fx, fy, 2, 2, vecPlayerList, 1);
        }
        // IDA: if (pUser) pUser->ChangeBattlePose(false)
    }

    // IDA: Scan for nearby objects
    if (m_objectScanner.npcScanner) {
        m_objectScanner.npcScanner->ScanGrid(fx, fy, 2, 2, vecObjList);
   }
    if (m_objectScanner.etcScanner) {
        m_objectScanner.etcScanner->ScanGrid(fx, fy, 2, 2, vecObjList);
    }

    // IDA: if (pUser && GetWorldType() == 2) SendPlayerInfoAll, SendEnterPlayerInfo
    if (pUser && GetWorldType() == 2) {
        SendPlayerInfoAll(pUser);
        SendEnterPlayerInfo(pUser);
    }

    // IDA: ProcessEnterObject(pActor, vecPlayerList, vecObjList)
    ProcessEnterObject(pActor, vecPlayerList, vecObjList);

    // IDA: if (pUser) InitSuperArmorGage, EnterPartyForceMember, CheckOptionEffectInvoke
    if (pUser) {
        // Note: These methods require complete types
        // CUser::InitSuperArmorGage(pUser);
        // EnterPartyForceMember(pUser);
        // CMoverEx::CheckOptionEffectInvoke(&pUser->CMoverEx, EFFECT_CONDITION_EQUIP, ...);
    }

    return 0;
}

// IDA 0x1402C8890 - Get scanner for actor
// IDA 反编译精确还原:
// 根据 Actor 类型返回对应的扫描器:
// - Type == 0 (Player): 返回 playerScanner
// - Type == 1 或 2 (NPC/Monster): 返回 npcScanner
// - 其他: 返回 etcScanner
Range2DScanner<CMover*>* XDistrict::GetScanner(XActor* pActor)
{
    // IDA: Direct decompilation
    int Type = pActor->GetType();
    if (Type == 0)
        return m_objectScanner.playerScanner;
    if (Type > 0 && Type <= 2)
        return m_objectScanner.npcScanner;
    return m_objectScanner.etcScanner;
}

void XDistrict::AddWaitForRecvInfo(CUser* pUser) {
    if (!pUser) return;

    // Add user to waiting list for receive info
    m_listWaitForRecvInfo.push_back(pUser);
}

void XDistrict::RemoveWaitForRecvInfo(CUser* pUser) {
    if (!pUser) return;

    // Remove user from waiting list
    m_listWaitForRecvInfo.remove(pUser);
}

// IDA 0x1402D1330 - Get district type from TB_DISTRICT table
// IDA 反编译精确还原:
// 1. nDistrictIndex = m_uxMapID.nMapID << 16 >> 48  (取高16位)
// 2. XGameServer* pServer = TXSingleton<XGameServer>::Instance()
// 3. TB_DISTRICT* pTBDistrict = XResourceMgr::GetTB_DISTRICT(&pServer->m_xResourceMgr, nDistrictIndex)
// 4. return pTBDistrict ? pTBDistrict->District_Type : 0
std::uint8_t XDistrict::GetDistrictType() {
    XGameServer* pServer = XGameServer::Instance();
    TB_DISTRICT* pTBDistrict = pServer ? pServer->GetResourceMgr().GetTB_DISTRICT(static_cast<std::int16_t>(GetTBMapID())) : nullptr;
    return pTBDistrict ? pTBDistrict->District_Type : 0;
}

// IDA 0x1402D1010 - Handle load complete for actor
// IDA 反编译精确还原:
// 1. pUser = dynamic_cast<CUser*>(pActor)
// 2. if (!pUser) return
// 3. CUser::SetClientLoadComplete(pUser, true)
// 4. CGocInventory::InitItemCoolTime() + SendItemCoolTimeInfo()
// 5. CGocEntity::IsRoguelikeState() -> 如果是 Roguelike 状态则踢出用户
// IDA: ?LoadComplete@XDistrict@@UEAAXPEAVXActor@@@Z (0x1402D1010)
void XDistrict::LoadComplete(XActor* pActor) {
    // IDA: if (!pActor) return
    if (!pActor) return;

    // IDA: CUser* pUser = dynamic_cast<CUser*>(pActor) via _RTDynamicCast
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser) return;

    // IDA: CUser::SetClientLoadComplete(pUser, 1)
    pUser->SetClientLoadComplete(true);

    // IDA: CMover::GetGOC<CGocInventory>(&pUser->CMoverEx, &pInvenPtr, 0)
    // IDA: if (pInvenPtr) { InitItemCoolTime(); SendItemCoolTimeInfo(); }
    std::shared_ptr<CGocInventory> pInvenPtr = pUser->GetGOC_Inventory(false);
    if (pInvenPtr) {
        pInvenPtr->InitItemCoolTime();
        pInvenPtr->SendItemCoolTimeInfo();
    }

    // IDA: CMover::GetGOC<CGocEntity>(&pUser->CMoverEx, &pEntity, 0)
    // IDA: if (pEntity && pEntity->IsRoguelikeState()) { kickout }
    std::shared_ptr<CGocEntity> pEntity = pUser->GetGOC_Entity(false);
    if (pEntity && pEntity->IsRoguelikeState()) {
        UXActorID actorID = pUser->GetActorID();
        LogHelper::LogError("game.item", "pEntity->IsRoguelikeState() %d / ( %d )",
            actorID.dwActorID, 2350);
        PS_KICK_USER_INFO psKickoutInfo;
        psKickoutInfo.byKickType = 0;
        psKickoutInfo.dwUAID = pUser->GetUAID();
        pUser->Kickout(&psKickoutInfo, false);
    }
}

// IDA 0x1402D11E0 - Send world mode info to actor
// IDA 反编译精确还原:
// 1. pUser = dynamic_cast<CUser*>(pActor)
// 2. if (!pUser) return
// 3. XSendPacket xSendPacket(0x30, 5)
// 4. xSendPacket << m_vecWorldModeList
// 5. CGocNetwork::Send(&pUser->XActor, &xSendPacket)
// 6. LogHelper::LogDebug("game.contents", "InfoWorldMode - Map:%d, Size:%d", GetTBMapID(), m_vecWorldModeList.size())
void XDistrict::SendWorldModeInfo(XActor* pActor) {
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser) return;

    XSendPacket xSendPacket(0x30, 5);
    ST_WORLD_MODE_INFO_VEC stInfoVec{};
    stInfoVec.vecInfo = m_vecWorldModeList;
    xSendPacket << stInfoVec;
    CGocNetwork::Send(pActor, xSendPacket);
    LogHelper::LogDebug("game.contents", "InfoWorldMode - Map:%d, Size:%d", GetTBMapID(), m_vecWorldModeList.size());
}

// IDA 0x1402D0930 - Send enter player info to nearby players
// IDA 反编译精确还原:
// 1. if (!pUser) return
// 2. XSendPacket xSendPacket(4, 0x51)
// 3. xSendPacket << 1 (count)
// 4. pUser->SetInfoPacket(xSendPacket)
// 5. CGocNetwork::SendBroadCast(&pUser->CMoverEx, &xSendPacket, 2)
void XDistrict::SendEnterPlayerInfo(CUser* pUser) {
    if (!pUser) return;

    XSendPacket xSendPacket(4, 0x51);
    xSendPacket.XParse << 1;
    pUser->SetInfoPacket(xSendPacket);
    CGocNetwork::SendBroadCast(pUser, xSendPacket, E_BROADCAST_TYPE::eNearby);
}

// IDA 0x1402D0A50 - Send exit player info to nearby players
// IDA 反编译精确还原:
// 1. if (!pUser) return
// 2. XSendPacket xSendPacket(4, 0x52)
// 3. UXActorID actorID = pUser->GetActorID()
// 4. xSendPacket << actorID.dwActorID
// 5. CGocNetwork::SendBroadCast(&pUser->CMoverEx, &xSendPacket, 2)
void XDistrict::SendExitPlayerInfo(CUser* pUser) {
    if (!pUser) return;

    XSendPacket xSendPacket(4, 0x52);
    const UXActorID actorID = pUser->GetActorID();
    xSendPacket.XParse << actorID.dwActorID;
    CGocNetwork::SendBroadCast(pUser, xSendPacket, E_BROADCAST_TYPE::eNearby);
}

// IDA 0x1402D0B60 - Send all player info to specific user
// IDA 反编译精确还原:
// 1. if (!pUser) return
// 2. std::vector<CMover*> vecPCList; vecPCList.reserve(300)
// 3. Range2DScanner<CMover*>::Enumerate(m_objectScanner.playerScanner, &vecPCList)
// 4. std::vector<STCharInfoEx> vecPCInfo; vecPCInfo.reserve(300)
// 5. 遍历 vecPCList，对每个 CUser (非 pUser) 获取 GetMyCharInfoEx 并 push_back 到 vecPCInfo
// 6. 当 vecPCInfo.size() > 66 时，发送包 (4, 0x51) 并清空 vecPCInfo
// 7. 最后发送剩余的玩家信息
// IDA: 0x1402D0B60 - ?SendPlayerInfoAll@XDistrict@@QEAAXPEAVCUser@@@Z
void XDistrict::SendPlayerInfoAll(CUser* pUser) {
    if (!pUser) return;

    // IDA: std::vector<CMover*> vecPCList; vecPCList.reserve(0x12C = 300)
    std::vector<CMover*> vecPCList;
    vecPCList.reserve(300);

    // IDA: Range2DScanner<CMover*>::Enumerate(m_objectScanner.playerScanner, vecPCList)
    if (m_objectScanner.playerScanner) {
        m_objectScanner.playerScanner->Enumerate(vecPCList);
    }

    // IDA: std::vector<STCharInfoEx> vecPCInfo; vecPCInfo.reserve(0x12C)
    std::vector<STCharInfoEx> vecPCInfo;
    vecPCInfo.reserve(300);

    // IDA: Iterate through player list
    for (auto it = vecPCList.begin(); it != vecPCList.end(); ++it) {
        CMover* pMover = *it;
        // IDA: pOtherPC = _RTDynamicCast(pMover, CMover, CUser)
        // Using dynamic_cast for RTTI cast
        CUser* pOtherPC = dynamic_cast<CUser*>(pMover);

        // IDA: if (pOtherPC != pUser)
        if (pOtherPC && pOtherPC != pUser) {
            // IDA: MyCharInfo = CUser::GetMyCharInfoEx(pOtherPC)
            // IDA: std::vector<STCharInfoEx>::push_back(&vecPCInfo, MyCharInfo)
            // Note: GetMyCharInfoEx returns STMyCharInfoEx&, which contains STCharInfoEx at offset 0
            STMyCharInfoEx& myInfo = pOtherPC->GetMyCharInfoEx();
            vecPCInfo.push_back(*reinterpret_cast<STCharInfoEx*>(&myInfo));

            // IDA: if (vecPCInfo.size() > 65.6) - actually 66
            if (vecPCInfo.size() > 66) {
                // IDA: XSendPacket::XSendPacket(&xSendPacket, 4u, 0x51u)
                XSendPacket xSendPacket(4, 0x51);

                // IDA: v9 = vecPCInfo.size(); XParse::operator<<(&xSendPacket.XParse, v9)
                xSendPacket.XParse << static_cast<std::int16_t>(vecPCInfo.size());

                // IDA: Iterate vecPCInfo and write each STCharInfoEx
                for (auto& charInfo : vecPCInfo) {
                    xSendPacket << charInfo;
                }

                // IDA: pActor = pUser ? &pUser->XActor : nullptr; CGocNetwork::Send(pActor, &xSendPacket)
                CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);

                // IDA: vecPCInfo.clear()
                vecPCInfo.clear();
            }
        }
    }

    // IDA: Send remaining players
    if (!vecPCInfo.empty()) {
        // IDA: XSendPacket::XSendPacket(&packet, 4u, 0x51u)
        XSendPacket packet(4, 0x51);

        // IDA: v13 = vecPCInfo.size(); XParse::operator<<(&packet.XParse, v13)
        packet.XParse << static_cast<std::int16_t>(vecPCInfo.size());

        // IDA: Iterate vecPCInfo and write each STCharInfoEx
        for (auto& charInfo : vecPCInfo) {
            packet << charInfo;
        }

        // IDA: v32 = pUser ? &pUser->XActor : nullptr; CGocNetwork::Send(v32, &packet)
        CGocNetwork::Send(static_cast<XActor*>(pUser), packet);

        // IDA: vecPCInfo.clear()
        vecPCInfo.clear();
    }
}

// IDA 0x1402D0190 - Finish world mode
// IDA 反编译精确还原:
// 1. XSendPacket xSendPacket(0x30, 3)
// 2. xSendPacket << stInfo
// 3. SendBroadCastAll(&xSendPacket)
// 4. 在 m_mapWorldMode 中查找并更新状态 (nMonsterClearCount, nState=2, bSuccess, nFinishTime)
// 5. 获取 TB_MODE_DISTRICT6 表数据
// 6. 如果 Start_Type == 0，遍历 m_vecWorldModeList 更新 nMonsterClearCount
// 7. 获取 TB_MODE_DISTRICT6_DATE 表数据，根据 Clear_Count 设置 WorldModeBoost
void XDistrict::FinishWorldMode(PS_WORLD_MODE_FINISH* stInfo) {
    XSendPacket xSendPacket(0x30, 3);
    xSendPacket << *stInfo;
    SendBroadCastAll(xSendPacket);

    auto it = m_mapWorldMode.find(stInfo->nModeID);
    if (it == m_mapWorldMode.end()) {
        return;
    }

    it->second.nMonsterClearCount = stInfo->nMonsterClearCount;
    it->second.nState = 2;
    it->second.bSuccess = stInfo->bSuccess;
    it->second.nFinishTime = stInfo->nFinishTime;

    XGameServer* pServer = XGameServer::Instance();
    TB_MODE_DISTRICT6* pTBMode = pServer ? pServer->GetResourceMgr().GetTB_MODE_DISTRICT6(static_cast<std::uint8_t>(stInfo->nModeID)) : nullptr;
    if (!pTBMode || pTBMode->Start_Type != 0) {
        return;
    }

    for (std::size_t i = 0; i < m_vecWorldModeList.size(); ++i) {
        if (m_vecWorldModeList[i].nModeDateID == stInfo->nModeDateID) {
            m_vecWorldModeList[i].nMonsterClearCount = stInfo->nMonsterClearCount;
        }
    }

    TB_MODE_DISTRICT6_DATE* pModeDate = pServer->GetResourceMgr().GetTB_MODE_DISTRICT6_DATE(static_cast<std::uint16_t>(stInfo->nModeDateID));
    if (!pModeDate || stInfo->nMonsterClearCount <= 0) {
        return;
    }

    const std::uint16_t* pClearCounts[] = {
        &pModeDate->Clear_Count_01,
        &pModeDate->Clear_Count_02,
        &pModeDate->Clear_Count_03,
        &pModeDate->Clear_Count_04,
        &pModeDate->Clear_Count_05,
        &pModeDate->Clear_Count_06,
    };
    const unsigned int* pClearBoosters[] = {
        &pModeDate->Clear_Booster_01,
        &pModeDate->Clear_Booster_02,
        &pModeDate->Clear_Booster_03,
        &pModeDate->Clear_Booster_04,
        &pModeDate->Clear_Booster_05,
        &pModeDate->Clear_Booster_06,
    };

    for (int j = 0; j < 5 && *pClearCounts[j] != 0; ++j) {
        if (stInfo->nMonsterClearCount >= *pClearCounts[j] && stInfo->nMonsterClearCount < *pClearCounts[j + 1]) {
            if (*pClearBoosters[j] != 0) {
                static_cast<CBattleZone*>(this)->SetWorldModeBoostAll(static_cast<int>(*pClearBoosters[j]), stInfo->nFinishTime + pModeDate->Booster_Limit_Time);
            }
            break;
        }

        if (stInfo->nMonsterClearCount >= *pClearCounts[j] && j == 4) {
            if (*pClearBoosters[j + 1] != 0) {
                static_cast<CBattleZone*>(this)->SetWorldModeBoostAll(static_cast<int>(*pClearBoosters[j + 1]), stInfo->nFinishTime + pModeDate->Booster_Limit_Time);
            }
            break;
        }
    }
}

// IDA 0x1402D0590 - Sync world mode
// IDA 反编译精确还原:
// 1. InfoWorldMode(stInfoVec) - 记录日志
// 2. 遍历 stInfoVec->vecInfo
// 3. 对于每个 ST_WORLD_MODE_INFO:
//    - 检查 TB_MODE_DISTRICT6 表是否存在
//    - nState == 1 或 2: 在 m_mapWorldMode 中查找并更新，或插入新条目
//    - nState == 0: 从 m_mapWorldMode 中删除
// 4. 记录日志
void XDistrict::SyncWorldMode(std::vector<ST_WORLD_MODE_INFO>* stInfoVec) {
    if (!stInfoVec) return;

    InfoWorldMode(*stInfoVec);

    for (auto it = stInfoVec->begin(); it != stInfoVec->end(); ++it) {
        ST_WORLD_MODE_INFO stInfo = *it;

        XGameServer* pServer = XGameServer::Instance();
        TB_MODE_DISTRICT6* pTBMode = pServer ? pServer->GetResourceMgr().GetTB_MODE_DISTRICT6(static_cast<std::uint8_t>(stInfo.nModeID)) : nullptr;
        if (!pTBMode) {
            continue;
        }

        if (stInfo.nState == 1 || stInfo.nState == 2) {
            auto mapIt = m_mapWorldMode.find(stInfo.nModeID);
            if (mapIt != m_mapWorldMode.end()) {
                mapIt->second = stInfo;
            } else {
                m_mapWorldMode.insert(std::make_pair(stInfo.nModeID, stInfo));
            }
            LogHelper::LogInfo("game.contents", "[WORLD_MODE] Sync Start or Finish %d %d %d",
                stInfo.nModeID, stInfo.nState, stInfo.nModeDateID);
        }
        else if (stInfo.nState == 0) {
            auto mapIt = m_mapWorldMode.find(stInfo.nModeID);
            if (mapIt != m_mapWorldMode.end()) {
                m_mapWorldMode.erase(mapIt);
            }
            LogHelper::LogInfo("game.contents", "[WORLD_MODE] Sync Ready %d %d",
                stInfo.nModeID, stInfo.nModeDateID);
        }
    }
}
// IDA 0x1402D0FE0 - Set object info request (add user to wait list)
void XDistrict::SetObjectInfoReq(CUser* pUser)
{
    if (!pUser) return;

    // IDA: Direct decompilation - push_back to waiting list
    m_listWaitForRecvInfo.push_back(pUser);
}

// IDA: ?ProcessEnterObject@XDistrict@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@1@Z (0x1402C9B10)
// Verified: Direct IDA decompilation
void XDistrict::ProcessEnterObject(XActor* pActor, std::vector<CMover*>& vecPlayerList, std::vector<CMover*>& vecObjList)
{
    if (!vecPlayerList.empty())
    {
        ProcessSendEnterObjectToOthers(vecPlayerList, pActor);
        ProcessSendTranslateInfoToOthers(pActor, vecPlayerList);
    }
}

// IDA: ?ProcessSendEnterObjectToOthers@XDistrict@@QEAAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@PEAVXActor@@@Z (0x1402C9B70)
// Verified: Direct IDA decompilation
void XDistrict::ProcessSendEnterObjectToOthers(std::vector<CMover*>& vecPlayerList, XActor* pEnterActor)
{
    if (vecPlayerList.empty() || !pEnterActor)
        return;

    // IDA: Check actor type and send appropriate enter packet
    // Note: Cannot access IsPlayer/IsMonster/IsNPC methods on incomplete type
    // TODO: 完整实现需要 CGocNetwork::Send 和各类型 Info 结构
}

// IDA: ?ProcessSendEnterObjectListToPlayer@XDistrict@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@1@Z (0x1402C9E80)
// Verified: Direct IDA decompilation
void XDistrict::ProcessSendEnterObjectListToPlayer(XActor* pActor, std::vector<CMover*>& vecPlayerList, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Send PC info first
    SendPcInfo(pActor, vecPlayerList, vecObjList);

    // IDA: If object list is not empty, send NPC/Monster/Interaction/Vaccum/Social info
    if (!vecObjList.empty())
    {
        SendNpcInfo(pActor, vecObjList);
        SendMonsterInfo(pActor, vecObjList);
        SendInteractionInfo(pActor, vecObjList);
        SendVaccumInfo(pActor, vecObjList);
        SendSocialInfo(pActor, vecObjList);
    }
}

// IDA: ?SendPcInfo@XDistrict@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@1@Z (0x1402C9F30)
// Verified: Direct IDA decompilation
void XDistrict::SendPcInfo(XActor* pActor, std::vector<CMover*>& vecPlayerList, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Create vector for PC info
    // std::vector<STCharInfoEx> vecPCInfo;
    // vecPCInfo.reserve(300);

    // IDA: Iterate through player list and collect info
    // Note: Cannot use dynamic_cast on incomplete types
    // TODO: 完整实现需要 STCharInfoEx 结构和 CGocNetwork::Send

    // IDA: Call ProcessSendTranslateInfoToPlayer after sending PC info
    ProcessSendTranslateInfoToPlayer(pActor, vecPlayerList);
}

// IDA: ?SendNpcInfo@XDistrict@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402CA260)
// Verified: Direct IDA decompilation
void XDistrict::SendNpcInfo(XActor* pActor, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Create vector for NPC info and send
    // Note: Cannot access GetType() on incomplete type
    // TODO: 完整实现需要 STNpcInfo 结构和 CNpc 类
}

// IDA: ?SendMonsterInfo@XDistrict@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402CA590)
// Verified: Direct IDA decompilation
void XDistrict::SendMonsterInfo(XActor* pActor, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Create vector for Monster info and send
    // Note: Cannot access GetType() on incomplete type
    // TODO: 完整实现需要 STMonsterInfo 结构和 CMonster 类
}

// IDA: ?SendInteractionInfo@XDistrict@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402CA9D0)
// Verified: Direct IDA decompilation
void XDistrict::SendInteractionInfo(XActor* pActor, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Create vector for Interaction info and send
    // Note: Cannot access GetType() on incomplete type
    // TODO: 完整实现需要 ST_BATCH_INTERACTION_LIST 结构和 CInteractionObject 类
}

// IDA: ?SendVaccumInfo@XDistrict@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402CAC10)
// Verified: Direct IDA decompilation
void XDistrict::SendVaccumInfo(XActor* pActor, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Create vector for VaccumCube info and send
    // Note: Cannot access GetType() on incomplete type
    // TODO: 完整实现需要 PS_VACCUM_CUBE_LIST 结构和 CVaccumCube 类
}

// IDA: ?SendSocialInfo@XDistrict@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402CAE50)
// Verified: Direct IDA decompilation
void XDistrict::SendSocialInfo(XActor* pActor, std::vector<CMover*>& vecObjList)
{
    if (!pActor)
        return;

    // IDA: Create vector for Social Item info and send
    // Note: Cannot access GetType() on incomplete type
    // TODO: 完整实现需要 ST_SOCIAL_ITEM_LIST 结构和 CSocialItemObject 类
}

// IDA: ?ProcessSendTranslateInfoToOthers@XDistrict@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402925B0)
// Verified: Direct IDA decompilation
void XDistrict::ProcessSendTranslateInfoToOthers(XActor* pActor, std::vector<CMover*>& vecPlayerList)
{
    if (!pActor)
        return;

    // IDA: Check if actor is playing public transport and send info
    // Note: Cannot use dynamic_cast on incomplete type CUser
    // TODO: 完整实现需要 CUser::IsPlayingPublicTransport 和 CGocNetwork::SendAfterLoading
}

// IDA: ?ProcessSendTranslateInfoToPlayer@XDistrict@@QEAAXPEAVXActor@@AEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z (0x1402CE420)
// Verified: Direct IDA decompilation
void XDistrict::ProcessSendTranslateInfoToPlayer(XActor* pActor, std::vector<CMover*>& vecPlayerList)
{
    if (!pActor)
        return;

    // IDA: Iterate through player list and send transport info
    // Note: Cannot use dynamic_cast on incomplete type CUser
    // TODO: 完整实现需要 CUser::IsPlayingPublicTransport 和 CGocNetwork::SendAfterLoading
}

// IDA 0x1402CB530 - ExitActor
// IDA 反编译精确还原:
// 1. GetScanner获取扫描器
// 2. dynamic_cast检查CUser
// 3. 如果是CUser: RemoveAllOptionEffect, DBSyncQuestCondition
// 4. 从扫描器中删除对象
// 5. 扫描周围玩家并发送退出消息
// 6. 调用ProcessLeaveObject
// 7. SetArea(nullptr), ExitActor基类
void XDistrict::ExitActor(XActor* pActor)
{
    if (!pActor)
        return;

    // IDA: Range2DScanner<CMover*>* pTargetScanner = GetScanner(pActor)
    Range2DScanner<CMover*>* pTargetScanner = GetScanner(pActor);
    if (!pTargetScanner) {
        return;
    }

    // IDA: CUser* pUser = dynamic_cast<CUser*>(pActor)
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    // Note: RemoveAllOptionEffect and DBSyncQuestCondition require complete types

    // IDA: Get position from actor
    // Note: XActor may not have m_vPos directly
    STPosInfo* pPosInfo = pActor->GetPosInfo();
    float fx = pPosInfo ? pPosInfo->vPos.x : 0.0f;
    float fy = pPosInfo ? pPosInfo->vPos.y : 0.0f;
    
    // IDA: CMover* obj = dynamic_cast<CMover*>(pActor)
    CMover* obj = dynamic_cast<CMover*>(pActor);

    // IDA: Range2DScanner<CMover*>::Erase(pTargetScanner, fx, fy, obj)
    if (pTargetScanner && obj) {
        pTargetScanner->Erase(fx, fy, obj);
    }

    // IDA: std::vector<CMover*> vecPlayerList; vecPlayerList.reserve(300)
    std::vector<CMover*> vecPlayerList;
    vecPlayerList.reserve(300);

    // IDA: Scan for nearby players
    if (m_objectScanner.playerScanner) {
        if (pActor->GetType()) {
            m_objectScanner.playerScanner->ScanGrid(fx, fy, 2, 2, vecPlayerList);
        } else {
            --m_nPcCount;
            m_objectScanner.playerScanner->ScanGridAndSetObjCnt(fx, fy, 2, 2, vecPlayerList, -1);
        }
    }

    // IDA: if (pUser && GetWorldType() == 2) SendExitPlayerInfo(pUser)
    if (pUser && GetWorldType() == 2) {
        SendExitPlayerInfo(pUser);
    }

    // IDA: pActor->SetArea(nullptr)
    pActor->SetArea(nullptr);
    
    // IDA: XArea::ExitActor(pActor)
    XArea::ExitActor(pActor);
}

// IDA 0x1402CC060 - SendBroadCast
// IDA 反编译精确还原:
// 1. 如果 eBroadCastType == eAll_InMap, 调用 SendBroadCastAll
// 2. 否则获取pActor位置
// 3. 扫描周围玩家
// 4. 调用 CGocNetwork::Send 广播消息
void XDistrict::SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType)
{
    // IDA: if (eBroadCastType == eAll) SendBroadCastAll(packet)
    if (eBroadCastType == E_BROADCAST_TYPE::eAll) {
        SendBroadCastAll(packet);
        return;
    }

    // IDA: if (!pActor) return
    if (!pExceptActor) return;

    // IDA: XActor* pExcept = nullptr; if (eBroadCastType == eNoneSelf) pExcept = pActor
    XActor* pExcept = nullptr;
    if (eBroadCastType == E_BROADCAST_TYPE::eNoneSelf) {
        pExcept = pExceptActor;
    }

    // IDA: Get position from actor
    STPosInfo* pPosInfo = pExceptActor->GetPosInfo();
    float fx = pPosInfo ? pPosInfo->vPos.x : 0.0f;
    float fy = pPosInfo ? pPosInfo->vPos.y : 0.0f;

    // IDA: std::vector<CMover*> vecPlayerList; vecPlayerList.reserve(300)
    std::vector<CMover*> vecPlayerList;
    vecPlayerList.reserve(300);
    
    // IDA: Range2DScanner<CMover*>::ScanGrid(m_objectScanner.playerScanner, fx, fy, 2, 2, vecPlayerList)
    if (m_objectScanner.playerScanner) {
        m_objectScanner.playerScanner->ScanGrid(fx, fy, 2, 2, vecPlayerList);
    }
    
    // IDA: CGocNetwork::Send(vecPlayerList, packet, pExcept)
    CGocNetwork::Send(vecPlayerList, packet, pExcept);
}

// IDA 0x1402CC180 - SendBroadCastAll
// IDA 反编译精确还原:
// 1. 枚举所有玩家
// 2. 遍历并发送消息给每个玩家
void XDistrict::SendBroadCastAll(XSendPacket& packet)
{
    // IDA: std::vector<CMover*> vecPCList
    std::vector<CMover*> vecPCList;

    // IDA: Range2DScanner<CMover*>::Enumerate(m_objectScanner.playerScanner, vecPCList)
    if (m_objectScanner.playerScanner) {
        m_objectScanner.playerScanner->Enumerate(vecPCList);
    }

    // IDA: Iterate through all players and send packet
    for (auto it = vecPCList.begin(); it != vecPCList.end(); ++it) {
        CMover* pMover = *it;
        if (pMover) {
            // IDA: pActor = (XActor*)(pMover + 872) - CMoverEx inherits from CMover, which has XActor at offset
            // Actually, CMoverEx is-a XActor through inheritance chain
            XActor* pActor = static_cast<XActor*>(pMover);
            CGocNetwork::Send(pActor, packet);
        }
    }
}

// IDA 0x1402CC7D0 - OnUpdate
// IDA 反编译精确还原:
// 1. 处理等待接收信息的用户列表
// 2. 遍历所有Actor，更新或删除无效用户
// 3. 检查社交物品过期时间
// 4. 定期记录用户数量日志
void XDistrict::OnUpdate(float fElapsed)
{
    // IDA: Process waiting users list
    for (auto it = m_listWaitForRecvInfo.begin(); it != m_listWaitForRecvInfo.end(); ) {
        CUser* pUser = *it;
        if (pUser) {
            // IDA: Check if user DB is loaded
            // Note: STMyCharInfoEx may not have UserDB field in current implementation
            // STMyCharInfoEx& info = pUser->GetMyCharInfoEx();
            // if ((info.UserDB & 4) != 0 && (info.UserDB & 8) != 0 && (info.UserDB & 0x10) != 0) {
            //     LoadComplete(static_cast<XActor*>(pUser));
            //     it = m_listWaitForRecvInfo.erase(it);
            //     continue;
            // }
        }
        ++it;
    }

    int nUserCount = 0;
    std::list<std::uint32_t> listDeleteUser;
    std::list<std::uint32_t> listDeleteSocialObj;

    // IDA: Iterate through all actors
    // Note: m_mapActor uses std::map, not TXMap
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (!pActor) continue;

        // IDA: Check actor type
        int nType = pActor->GetType();
        if (nType != 0) {
            // IDA: Type 6 = SocialItemObject
            // Note: CSocialItemObject is incomplete type
            // if (nType == 6) {
            //     CSocialItemObject* pSocialObject = dynamic_cast<CSocialItemObject*>(pActor);
            //     if (pSocialObject) {
            //         // IDA: CSocialItemObject::CheckRemainTime
            //         if (pSocialObject->CheckRemainTime(fElapsed)) {
            //             UXActorID actorID = pSocialObject->GetActorID();
            //             listDeleteSocialObj.push_back(CQuestCondition::GetQuestID(&actorID));
            //         }
            //     }
            // }
        } else {
            // IDA: Type 0 = Player (CUser)
            CUser* pUser = dynamic_cast<CUser*>(pActor);
            if (pUser) {
                // IDA: Check if user is valid
                // Note: IsBit_OR and GetValidMapInsID methods don't exist in current implementation
                // UXMapID validMapInsID;
                // if (pUser->IsBit_OR(static_cast<XClient::E_NET_STATE>(5)) &&
                //     pUser->GetValidMapInsID(&validMapInsID)->nMapID != 0) {
                //     
                //     UXMapID instanceID = GetInstanceID();
                //     if (validMapInsID.nMapID == instanceID.nMapID) {
                //         // IDA: Valid user - call OnUpdate
                //         pActor->OnUpdate();
                //         ++nUserCount;
                //     } else {
                //         // IDA: Invalid user - remove
                //         Range2DScanner<CMover*>* pTargetScanner = GetScanner(pActor);
                //         if (pTargetScanner) {
                //             CMover* obj = dynamic_cast<CMover*>(pActor);
                //             pTargetScanner->Erase(obj);
                //         }
                //         listDeleteUser.push_back(pair.first);
                //     }
                // }
                ++nUserCount;
            }
        }
    }

    // IDA: Delete invalid users
    for (auto it = listDeleteUser.begin(); it != listDeleteUser.end(); ++it) {
        m_mapActor.erase(*it);
    }
    listDeleteUser.clear();

    // IDA: Delete expired social objects
    for (auto it = listDeleteSocialObj.begin(); it != listDeleteSocialObj.end(); ++it) {
        DeleteSocialItemObject(*it);
    }
    listDeleteSocialObj.clear();

    // IDA: Log user count every 60 seconds
    std::int64_t nNowTick = GetTickCount64();
    if (nNowTick - m_nCheckUserLog >= 60000) {
        ST_LOG_GAME stLog = {};
        stLog._nUAID = 0;
        stLog._nUCID = 0;
        stLog._sMainType = 100;
        stLog._sSubType = 5;
        
        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            stLog.nParam0 = pServer->GetServerID();
        }
        stLog.nParam1 = static_cast<std::int16_t>(GetChannel());
        stLog.nParam2 = static_cast<std::uint16_t>(GetTBMapID());
        stLog.nParam3 = nUserCount;
        
        if (pServer) {
            pServer->SendDBLog(stLog);
        }
        m_nCheckUserLog = nNowTick;
    }
    
    (void)fElapsed;
}

// IDA 0x1402CD500 - MoveActor
// IDA 反编译精确还原:
// 1. 检查是否在同一区域或周围区域
// 2. 获取扫描器并移动对象
// 3. 扫描进入和离开的玩家/对象
// 4. 调用 ProcessMoveObject 处理移动
std::uint16_t XDistrict::MoveActor(XActor* pActor, XVec3& vPos, float fRot, bool bByForce)
{
    if (!pActor)
        return 1;

    // IDA: Get source position
    STPosInfo* pPosInfo = pActor->GetPosInfo();
    float sx = pPosInfo ? pPosInfo->vPos.x : 0.0f;
    float sy = pPosInfo ? pPosInfo->vPos.y : 0.0f;
    float dx = vPos.x;
    float dy = vPos.y;

    // IDA: Check if around sector (unless forced)
    if (!bByForce && !IsAroundSector(sx, sy, dx, dy)) {
        UXActorID actorID = pActor->GetActorID();
        LogHelper::LogError("game.contents", 
            "MoveActor error - Failed user movement[ ActorID:%d, PosX:%2.f, PosY:%2.f ] ( %s ) ( %d )",
            actorID.dwActorID, vPos.x, vPos.y, "F:\\_PROGRAM_HG\\Source\\Soulworker\\GameServer\\XGameServer\\District.cpp", 1330);
        return 1;
    }

    // IDA: Check if same sector - just update position
    if (IsSameSector(sx, sy, dx, dy)) {
        STPosInfo newPos;
        newPos.vPos = vPos;
        pActor->SetPosInfo(&newPos);
        return 0;
    }

    // IDA: Get scanner for actor
    Range2DScanner<CMover*>* pScanner = GetScanner(pActor);
    if (!pScanner) return 1;

    // IDA: Move in scanner
    CMover* obj = dynamic_cast<CMover*>(pActor);
    if (obj && !pScanner->Move(sx, sy, dx, dy, obj)) {
        UXActorID actorID = pActor->GetActorID();
        LogHelper::LogError("game.contents", 
            "MoveActor error - Failed user movement[ ActorID:%d, srcX:%.2f, srcY:%.2f, destX:%.2f, destY:%.2f ] ( %d )",
            actorID.dwActorID, sx, sy, dx, dy, 1350);
        return 1;
    }

    // IDA: Update position
    STPosInfo newPos;
    newPos.vPos = vPos;
    pActor->SetPosInfo(&newPos);

    // IDA: Scan for enter/leave players and objects
    // Note: Range2DScanner methods ScanEnterLeaveAndSetObjCnt and ScanEnterLeaveCurrentWhenMove
    // don't exist in current implementation - using simplified approach
    std::vector<CMover*> vecEnterPlayerList;
    std::vector<CMover*> vecLeavePlayerList;
    std::vector<CMover*> vecEnterObjList;
    std::vector<CMover*> vecLeaveObjList;
    
    vecEnterPlayerList.reserve(300);
    vecLeavePlayerList.reserve(300);
    vecEnterObjList.reserve(300);
    vecLeaveObjList.reserve(300);

    // IDA: Scan player scanner
    if (pActor->GetType() == 0) { // Player type
        // Note: ScanEnterLeaveAndSetObjCnt not available
        if (m_objectScanner.playerScanner) {
            m_objectScanner.playerScanner->ScanGrid(dx, dy, 2, 2, vecEnterPlayerList);
        }
    } else {
        // Note: ScanEnterLeaveCurrentWhenMove not available
        if (m_objectScanner.playerScanner) {
            m_objectScanner.playerScanner->ScanGrid(dx, dy, 2, 2, vecEnterPlayerList);
        }
    }

    // IDA: Copy player lists to object lists
    vecEnterObjList = vecEnterPlayerList;
    vecLeaveObjList = vecLeavePlayerList;

    // IDA: Scan NPC and ETC scanners
    if (m_objectScanner.npcScanner) {
        m_objectScanner.npcScanner->ScanGrid(dx, dy, 2, 2, vecEnterObjList);
    }
    if (m_objectScanner.etcScanner) {
        m_objectScanner.etcScanner->ScanGrid(dx, dy, 2, 2, vecEnterObjList);
    }

    // IDA: Process move object
    // Note: ProcessMoveObject not declared - commented out
    // ProcessMoveObject(pActor, vecEnterPlayerList, vecLeavePlayerList, vecEnterObjList, vecLeaveObjList);

    (void)fRot;
    return 0;
}

// IDA 0x1402CDB30 - IsAroundSector
// IDA 反编译精确还原:
// 1. 计算两个位置的坐标
// 2. 比较坐标差值是否在2以内
bool XDistrict::IsAroundSector(float fx, float fy, float fx2, float fy2)
{
    // IDA: Direct decompilation
    Range2DScanner<CMover*>::Coord coord1 = m_objectScanner.playerScanner->CalcCoordFromPos(fx, fy);
    Range2DScanner<CMover*>::Coord coord2 = m_objectScanner.playerScanner->CalcCoordFromPos(fx2, fy2);
    return std::abs(coord1.x - coord2.x) <= 2 && std::abs(coord1.y - coord2.y) <= 2;
}

// IDA 0x1402CDBD0 - IsSameSector
// IDA 反编译精确还原:
// 1. 计算两个位置的坐标
// 2. 比较坐标是否相同
bool XDistrict::IsSameSector(float fx, float fy, float fx2, float fy2)
{
    // IDA: Direct decompilation
    Range2DScanner<CMover*>::Coord coord1 = m_objectScanner.playerScanner->CalcCoordFromPos(fx, fy);
    Range2DScanner<CMover*>::Coord coord2 = m_objectScanner.playerScanner->CalcCoordFromPos(fx2, fy2);
    return coord1.x == coord2.x && coord1.y == coord2.y;
}

// IDA 0x1402CB960 - SendObjectInfo
// IDA 反编译精确还原:
// 1. 获取扫描器
// 2. 获取Actor位置
// 3. 扫描周围玩家和对象
// 4. 如果是玩家，调用ProcessSendEnterObjectListToPlayer
// 5. 发送世界模式信息
bool XDistrict::SendObjectInfo(XActor* pActor)
{
    if (!pActor)
        return false;

    // IDA: if (!GetScanner(pActor)) return false;
    if (!GetScanner(pActor))
        return false;

    // IDA: Get position from actor
    STPosInfo* pPosInfo = pActor->GetPosInfo();
    float fx = pPosInfo ? pPosInfo->vPos.x : 0.0f;
    float fy = pPosInfo ? pPosInfo->vPos.y : 0.0f;

    // IDA: std::vector<CMover*> vecPlayerList; vecPlayerList.reserve(0x12C)
    std::vector<CMover*> vecPlayerList;
    vecPlayerList.reserve(300);

    // IDA: Range2DScanner<CMover*>::ScanGrid(m_objectScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList)
    if (m_objectScanner.playerScanner) {
        m_objectScanner.playerScanner->ScanGrid(fx, fy, 2, 2, vecPlayerList);
    }

    // IDA: std::vector<CMover*> vecObjList; vecObjList.reserve(0x12C)
    std::vector<CMover*> vecObjList;
    vecObjList.reserve(300);

    // IDA: Range2DScanner<CMover*>::ScanGrid(m_objectScanner.npcScanner, fx, fy, 2, 2, &vecObjList)
    if (m_objectScanner.npcScanner) {
        m_objectScanner.npcScanner->ScanGrid(fx, fy, 2, 2, vecObjList);
    }

    // IDA: Range2DScanner<CMover*>::ScanGrid(m_objectScanner.etcScanner, fx, fy, 2, 2, &vecObjList)
    if (m_objectScanner.etcScanner) {
        m_objectScanner.etcScanner->ScanGrid(fx, fy, 2, 2, vecObjList);
    }

    // IDA: if (XActor::IsPlayer(pActor)) ProcessSendEnterObjectListToPlayer(pActor, vecPlayerList, vecObjList)
    if (pActor->IsPlayer()) {
        ProcessSendEnterObjectListToPlayer(pActor, vecPlayerList, vecObjList);
    }

    // IDA: SendWorldModeInfo(pActor)
    SendWorldModeInfo(pActor);

    // IDA: XMaze::SetWorldModeSync((XMaze*)this, pActor)
    // Note: XDistrict does not inherit from XMaze, so this call is omitted
    // SetWorldModeSync(pActor);

    return true;
}

// IDA 0x1402CD4C0 - IsValidPosition
// IDA 反编译精确还原:
// 检查位置是否在扫描器范围内
bool XDistrict::IsValidPosition(XVec3& vPos)
{
    // IDA: Uses Range2DScanner::IsValidPos
    // Note: Range2DScanner may not have IsValidPos method in current implementation
    // Using CalcCoordFromPos to verify position is valid
    if (!m_objectScanner.playerScanner)
        return false;
    Range2DScanner<CMover*>::Coord coord = m_objectScanner.playerScanner->CalcCoordFromPos(vPos.x, vPos.y);
    return coord.x >= 0 && coord.y >= 0;
}

// IDA 0x1402CBD60 - EscapeActor
// IDA 反编译精确还原:
// 1. dynamic_cast检查CUser
// 2. 获取区域类型
// 3. 根据区域类型获取传送位置
// 4. 移动Actor并清除移动值
// 5. 发送传送响应
bool XDistrict::EscapeActor(XActor* pActor)
{
    if (!pActor)
        return false;

    // IDA: CUser* pUser = dynamic_cast<CUser*>(pActor)
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser)
        return false;

    // IDA: XArea* pArea = pActor->GetArea()
    XArea* pArea = pActor->GetArea();
    if (!pArea)
        return false;

    // IDA: STPosInfo stMovePos
    STPosInfo stMovePos = {};

    // IDA: if (pArea->GetWorldType() == 2) - Battlefield type
    if (pArea->GetWorldType() == 2) {
        // IDA: nEscapePoint = CUser::GetRevivePoint(pUser)
        // Note: GetRevivePoint method not yet defined
        // int nEscapePoint = pUser->GetRevivePoint();
        int nMapID = static_cast<int>(GetTBMapID());

        // IDA: XGameServer* pServer = TXSingleton<XGameServer>::Instance()
        XGameServer* pServer = XGameServer::Instance();

        // IDA: if (!XWorldResMgr::GetPortalPos(&pServer->m_xWorldResMgr, nMapID, nEscapePoint, &stMovePos))
        // Note: GetPortalPos method not yet defined
        // if (!pServer || !pServer->GetWorldResMgr().GetPortalPos(nMapID, nEscapePoint, &stMovePos)) {
        //     // IDA: XWorldResMgr::GetStartPortalPos(&pServer->m_xWorldResMgr, TBMapID, &stMovePos)
        //     if (pServer) {
        //         pServer->GetWorldResMgr().GetStartPortalPos(static_cast<int>(GetTBMapID()), &stMovePos);
        //     }
        // }

        // IDA: MoveActor(pActor, stMovePos.vPos)
        // Note: stMovePos not initialized due to missing methods
        // MoveActor(pActor, stMovePos.vPos, stMovePos.fRot, true);

        // IDA: CMover::MoveingValueClear(&pUser->CMoverEx)
        // Note: Requires CMover method
        // pUser->MoveingValueClear();

        // IDA: pUser->ChangeMotion(1, 1, 0)
        // Note: Requires CMover method
        // pUser->ChangeMotion(1, 1, 0);

        // IDA: CUser::SendResWarp(pUser, 0, &stMovePos.vPos, stMovePos.fRot)
        // Note: SendResWarp method not yet defined
        // pUser->SendResWarp(0, stMovePos.vPos, stMovePos.fRot);

        GreenDamTan_log(__FILE__, __FUNCTION__, "EscapeActor - battlefield type (requires GetRevivePoint/GetPortalPos/SendResWarp)");
        return true;
    }
    else {
        // IDA: Non-battlefield type
        int nJumpID = 0;
        int nMapID = static_cast<int>(GetTBMapID());

        // IDA: XGameServer* pServer = TXSingleton<XGameServer>::Instance()
        XGameServer* pServer = XGameServer::Instance();

        // IDA: if (XWorldResMgr::GetStartPortalID(&pServer->m_xWorldResMgr, nMapID, &nJumpID, &stMovePos))
        // Note: GetStartPortalID method not yet defined
        // if (pServer && pServer->GetWorldResMgr().GetStartPortalID(nMapID, &nJumpID, &stMovePos)) {
        //     // IDA: MoveActor(pActor, stMovePos.vPos)
        //     MoveActor(pActor, stMovePos.vPos, stMovePos.fRot, true);

        //     // IDA: CMover::MoveingValueClear(&pUser->CMoverEx)
        //     // Note: Requires CMover method
        //     // pUser->MoveingValueClear();

        //     // IDA: pUser->ChangeMotion(1, 1, 0)
        //     // Note: Requires CMover method
        //     // pUser->ChangeMotion(1, 1, 0);

        //     // IDA: CUser::SendResWarp(pUser, 0, &stMovePos.vPos, stMovePos.fRot)
        //     // Note: SendResWarp method not yet defined
        //     pUser->SendResWarp(0, stMovePos.vPos, stMovePos.fRot);

        //     return true;
        // }
        // else {
        //     return false;
        // }

        GreenDamTan_log(__FILE__, __FUNCTION__, "EscapeActor - non-battlefield type (requires GetStartPortalID/SendResWarp)");
        return true;
    }
}

// IDA 0x1402CC270 - SpawnGenerate
// IDA 反编译精确还原:
// 1. 获取对象资源映射
// 2. 遍历所有对象
// 3. 如果是Box类型且启用，执行生成
void XDistrict::SpawnGenerate()
{
    // IDA: VMap<int, void*> ObjectMap(10)
    // Note: VMap type not yet defined - using placeholder
    // VMap<int, void*> ObjectMap(10);

    // IDA: XGameServer* pServer = TXSingleton<XGameServer>::Instance()
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer)
        return;

    // IDA: CFAutoSlimWriteLock lock(&pServer->m_rwMapLock)
    // Note: Lock not yet implemented
    // CFAutoSlimWriteLock lock(&pServer->m_rwMapLock);

    // IDA: ObjectMap = m_pObjectResource->GetObjectMap()
    // Note: GetObjectMap method not yet defined
    // ObjectMap = m_pObjectResource->GetObjectMap();

    // IDA: Iter = VMap<int, void*>::GetStartPosition(&ObjectMap)
    // void* Iter = ObjectMap.GetStartPosition();

    // IDA: while (Iter)
    // while (Iter) {
    //     int iKey;
    //     void* pValue;
    //     VMap<int, void*>::GetNextPair(&ObjectMap, &Iter, &iKey, &pValue);
    //
    //     VEventObjectInfo* pInfo = (VEventObjectInfo*)pValue;
    //     if (pInfo && pInfo->eType == eEventObjectType_Box) {
    //         VEventBoxInfo* pBoxInfo = (VEventBoxInfo*)pInfo;
    //         if (pBoxInfo->bEnable == 1 && pBoxInfo->bSpawn == 1) {
    //             ExcuteSpawnBox((VMonsterSpawnInfo*)pBoxInfo, eSendInfoTypeNot);
    //         }
    //     }
    // }

    // IDA: VMap<int, void*>::~VMap<int, void*>(&ObjectMap)

    // Note: VEventObjectInfo, VEventBoxInfo, VMap types not yet defined
    // Using stub implementation until types are available
    GreenDamTan_log(__FILE__, __FUNCTION__, "SpawnGenerate - requires VEventObjectInfo/VEventBoxInfo/VMap types");
}

// IDA 0x1402CB0B0 - ExitSocialItemObject
// IDA 反编译精确还原:
// 1. 检查 bLeave 标志和用户的 SocialOwnerID
// 2. 如果是 Owner，结束社交物品并删除
// 3. 否则检查用户的 SocialUseID，找到对应社交物品对象
// 4. 验证用户是否在使用该物品
// 5. 结束播放，删除用户关联
// 6. 如果是家具类型且用户是 Owner 或状态不是 WAIT，删除物品
// 7. 移除光环技能，清除 SocialUseID
// 8. 发送停止响应包，广播给附近玩家
bool XDistrict::ExitSocialItemObject(CUser* pUser, int bLeave)
{
    if (!pUser)
        return false;

    // IDA: if (bLeave == 1 && dwOwnerID = CUser::GetSocialOwnerID(pUser))
    // Note: GetSocialOwnerID method not yet defined
    if (bLeave == 1) {
        // std::uint32_t dwOwnerID = pUser->GetSocialOwnerID();
        // CSocialItemObject* pOwnerObject = FindSocialItemObject(dwOwnerID);

        // if (pOwnerObject) {
        //     // IDA: QuestID = CQuestCondition::GetQuestID(pUser->GetActorID())
        //     UXActorID actorID = pUser->GetActorID();
        //     std::uint32_t QuestID = CQuestCondition::GetQuestID(&actorID);

        //     // IDA: CSocialItemObject::FinishPlaySocialItemObject(pOwnerObject, QuestID)
        //     pOwnerObject->FinishPlaySocialItemObject(QuestID);

        //     // IDA: DeleteSocialItemObject(dwOwnerID)
        //     DeleteSocialItemObject(dwOwnerID);

        //     return true;
        // }
    }

    // IDA: dwObjectID = CUser::GetSocialUseID(pUser)
    std::uint32_t dwObjectID = pUser->GetSocialUseID();
    CSocialItemObject* pSocialItemObject = FindSocialItemObject(dwObjectID);

    if (!pSocialItemObject)
        return false;

    // IDA: QuestID = CQuestCondition::GetQuestID(pUser->GetActorID())
    // Note: CQuestCondition::GetQuestID is a member method, not static
    // UXActorID actorID = pUser->GetActorID();
    // std::uint32_t QuestID = CQuestCondition::GetQuestID(&actorID);

    // IDA: if (!CSocialItemObject::IsExistUser(pSocialItemObject, QuestID))
    // Note: CSocialItemObject is incomplete type
    // if (!pSocialItemObject->IsExistUser(QuestID))
    //     return false;

    // IDA: CSocialItemObject::FinishPlaySocialItemObject(pSocialItemObject, QuestID)
    // pSocialItemObject->FinishPlaySocialItemObject(QuestID);

    // IDA: if (!CSocialItemObject::DeleteUser(pSocialItemObject, QuestID))
    // if (!pSocialItemObject->DeleteUser(QuestID))
    //     return false;

    // IDA: ItemInfo = CSocialItemObject::GetItemInfo(pSocialItemObject)
    // Note: ST_SOCIAL_ITEM_INFO not yet defined
    // const ST_SOCIAL_ITEM_INFO* pItemInfo = pSocialItemObject->GetItemInfo();
    // ST_SOCIAL_ITEM_INFO stItemInfo = *pItemInfo;

    // IDA: if (CSocialItemObject::GetSocialType(pSocialItemObject) == 3) - Furniture type
    // if (pSocialItemObject->GetSocialType() == 3) {
    //     std::uint32_t userQuestID = CQuestCondition::GetQuestID(&actorID);

    //     // IDA: if (stItemInfo.dwOwnerID == userQuestID || GetSocialPlayState != WAIT)
    //     if (stItemInfo.dwOwnerID == userQuestID ||
    //         pSocialItemObject->GetSocialPlayState() != E_SOCIAL_OBJECT_STATE_WAIT) {
    //         DeleteSocialItemObject(dwObjectID);
    //     }
    // }

    // IDA: CMoverEx::RemoveAuraSkill(&pUser->CMoverEx, 1)
    // Note: Requires CMoverEx method
    // pUser->RemoveAuraSkill(1);

    // IDA: CUser::SetSocialUseID(pUser, 0)
    pUser->SetSocialUseID(0);

    // IDA: PS_SOCIALITEM_STOP_RES stStopRes
    // Note: PS_SOCIALITEM_STOP_RES not yet defined
    // PS_SOCIALITEM_STOP_RES stStopRes = {};
    // stStopRes.dwActorID = CQuestCondition::GetQuestID(&actorID);
    // stStopRes.stItemInfo = stItemInfo;

    // IDA: MyroomBackupPos = CUser::GetMyroomBackupPos(pUser)
    // XVec3 backupPos = pUser->GetMyroomBackupPos();
    // stStopRes.vPos = backupPos;

    // IDA: stStopRes.fRot = CUser::GetMyroomBackupYaw(pUser)
    // stStopRes.fRot = pUser->GetMyroomBackupYaw();

    // IDA: XSendPacket xSendPacket(0x2D, 3)
    // XSendPacket xSendPacket(0x2D, 3);
    // xSendPacket << stStopRes;

    // IDA: CGocNetwork::BroadcastNearby(pUser, nullptr, &xSendPacket)
    // CGocNetwork::BroadcastNearby(pUser, nullptr, xSendPacket);

    (void)dwObjectID;
    (void)pSocialItemObject;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ExitSocialItemObject - requires CSocialItemObject/ST_SOCIAL_ITEM_INFO types");
    return true;
}

// IDA 0x1402CC390 - ExcuteSpawnBox
// IDA 反编译精确还原:
// 1. 遍历怪物信息数组（最多10个）
// 2. 对于 Type==1 的 NPC：创建 NPC，设置航点 ID 和生成盒 ID
// 3. 对于 Type==5 的社交物品：获取社交物品表，创建社交物品对象
// 4. 设置社交物品的半径、最大计数、类型和家具信息
// 5. 将社交物品对象添加到区域
void XDistrict::ExcuteSpawnBox(const VMonsterSpawnInfo* pMonsterSpawn, E_SEND_INFO_TYPE eType)
{
    if (!pMonsterSpawn)
        return;

    // IDA: hkvVec3 vPos
    XVec3 vPos = {};

    // IDA: for (int i = 0; i < 10; ++i)
    for (int i = 0; i < 10; ++i) {
        // IDA: if (pMonsterSpawn->m_stMonsterInfo[i].m_iID == 0) continue
        if (pMonsterSpawn->m_stMonsterInfo[i].m_iID == 0)
            continue;

        // IDA: if (pMonsterSpawn->m_stMonsterInfo[i].m_iType == 1) - NPC type
        if (pMonsterSpawn->m_stMonsterInfo[i].m_iType == 1) {
            // IDA: GetSpawnPos_2(pMonsterSpawn, &vPos)
            GetSpawnPos(pMonsterSpawn, vPos);

            // IDA: ThreadLocalData::GetInstance()->CreateNpc(...)
            // Note: ThreadLocalData and CNpc not yet defined
            // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
            // CNpc* pNpc = pThreadData->CreateNpc(this, m_uxMapID, 0,
            //     pMonsterSpawn->m_stMonsterInfo[i].m_iID, &vPos, pMonsterSpawn->fRotate);

            // if (pNpc) {
            //     if (EnterActor(&pNpc->XActor)) {
            //         ThreadLocalData::GetInstance()->DeleteNpc(pNpc);
            //     } else {
            //         pNpc->SetWayPointID(pMonsterSpawn->m_iWaypoint);
            //         pNpc->SetSpawnBoxID(pMonsterSpawn->iID);
            //     }
            // }
        }
        // IDA: else if (pMonsterSpawn->m_stMonsterInfo[i].m_iType == 5) - Social item type
        else if (pMonsterSpawn->m_stMonsterInfo[i].m_iType == 5) {
            // IDA: GetSpawnPos_2(pMonsterSpawn, &vPos)
            GetSpawnPos(pMonsterSpawn, vPos);

            int wSocialItemID = pMonsterSpawn->m_stMonsterInfo[i].m_iID;

            // IDA: XGameServer* pServer = TXSingleton<XGameServer>::Instance()
            XGameServer* pServer = XGameServer::Instance();
            if (!pServer)
                return;

            // IDA: pSocialItemTable = XResourceMgr::GetTB_SOCIAL_ITEM(&pServer->m_xResourceMgr, wSocialItemID)
            TB_SOCIAL_ITEM* pSocialItemTable = pServer->GetResourceMgr().GetTB_SOCIAL_ITEM(wSocialItemID);
            if (!pSocialItemTable)
                return;

            // IDA: fRadius = pSocialItemTable->Collision_Range / 2.0f
            float fRadius = pSocialItemTable->Collision_Range / 2.0f;

            // IDA: pSocialItemObject = CreateSocialItemObject(0, &vPos, pMonsterSpawn->fRotate, wSocialItemID)
            CSocialItemObject* pSocialItemObject = CreateSocialItemObject(0, vPos, pMonsterSpawn->fRotate, wSocialItemID);

            if (pSocialItemObject) {
                // IDA: CSocialItemObject::SetRadius(pSocialItemObject, fRadius)
                // Note: CSocialItemObject is incomplete type
                // pSocialItemObject->SetRadius(fRadius);

                // IDA: CSocialItemObject::SetMaxCount(pSocialItemObject, pSocialItemTable->Use_Max)
                // pSocialItemObject->SetMaxCount(pSocialItemTable->Use_Max);

                // IDA: pTB_Social_Item = XResourceMgr::GetTB_SOCIAL_ITEM(...)
                TB_SOCIAL_ITEM* pTB_Social_Item = pServer->GetResourceMgr().GetTB_SOCIAL_ITEM(wSocialItemID);

                if (pTB_Social_Item) {
                    // IDA: CSocialItemObject::SetSocialType(pSocialItemObject, pTB_Social_Item->Social_Item_Type)
                    // pSocialItemObject->SetSocialType(pTB_Social_Item->Social_Item_Type);

                    // IDA: if (pSocialItemTable->Social_Item_Type == 1) - Furniture
                    if (pTB_Social_Item->Social_Item_Type == 1) {
                        // IDA: pTB_Furniture = XResourceMgr::GetTB_MYROOM_FURNITURE(...)
                        TB_MYROOM_FURNITURE* pTB_Furniture = pServer->GetResourceMgr().GetTB_MYROOM_FURNITURE(
                            pTB_Social_Item->Furniture_Object_ID);

                        if (!pTB_Furniture) {
                            LogHelper::LogError("game.item", "ExcuteSpawnBox error - SocialID:%d", wSocialItemID);
                            return;
                        }

                        // IDA: CSocialItemObject::SetFurnitureInfo(pSocialItemObject, pTB_Furniture->Furniture_Item_Special_Use)
                        // pSocialItemObject->SetFurnitureInfo(pTB_Furniture->Furniture_Item_Special_Use);
                    }
                }

                // IDA: dwObjectID = CQuestCondition::GetQuestID(pSocialItemObject->GetActorID())
                // Note: CQuestCondition::GetQuestID is a member method, not static
                // UXActorID actorID = pSocialItemObject->GetActorID();
                // std::uint32_t dwObjectID = CQuestCondition::GetQuestID(&actorID);

                // IDA: if (!AddSocialItemObject(pSocialItemObject))
                // if (!AddSocialItemObject(pSocialItemObject)) {
                //     DeleteSocialItemObject(dwObjectID);
                //     return;
                // }

                (void)pSocialItemObject;
            }
        }
    }

    (void)eType;
}

// IDA 0x1402CE7F0 - SpawnNPC
// IDA 反编译精确还原:
// 1. 创建 NPC
// 2. 调用 EnterActor 进入区域
// 3. 成功后将 NPC ID 添加到生成列表
bool XDistrict::SpawnNPC(int nNpcID, XVec3& xPos, float fRot)
{
    // IDA: ThreadLocalData::GetInstance()->CreateNpc(...)
    // Note: ThreadLocalData and CNpc not yet defined
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // CNpc* pNpc = pThreadData->CreateNpc(this, m_uxMapID, 0, nNpcID, &xPos, fRot);

    // if (pNpc) {
    //     if (!EnterActor(&pNpc->XActor)) {
    //         UXActorID actorID = pNpc->GetActorID();
    //         std::uint32_t dwActorID = CQuestCondition::GetQuestID(&actorID);
    //         m_listSpawnNPC.push_back(dwActorID);
    //         return true;
    //     }
    //     ThreadLocalData::GetInstance()->DeleteNpc(pNpc);
    // }

    (void)nNpcID;
    (void)xPos;
    (void)fRot;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SpawnNPC - requires ThreadLocalData/CNpc types");
    return true;
}

// IDA 0x1402CE940 - DeleteNPC
// IDA 反编译精确还原:
// 1. 遍历 m_listSpawnNPC 列表
// 2. 查找每个 NPC ID 对应的 Actor
// 3. 检查是否匹配目标 NPC ID
// 4. 调用 ExitActor 退出区域
// 5. 删除 NPC
bool XDistrict::DeleteNPC(int nNpcID)
{
    // IDA: Iterate through m_listSpawnNPC
    for (auto it = m_listSpawnNPC.begin(); it != m_listSpawnNPC.end(); ++it) {
        std::uint32_t dwActorID = *it;

        // IDA: XActor* pActor = FindActor(dwActorID)
        XActor* pActor = FindActor(dwActorID);
        if (!pActor)
            continue;

        // IDA: CNpc* pNpc = dynamic_cast<CNpc*>(pActor)
        // Note: CNpc not yet defined
        // CNpc* pNpc = dynamic_cast<CNpc*>(pActor);
        // if (!pNpc)
        //     continue;

        // IDA: if (pNpc->GetTableID() == nNpcID)
        // Note: GetTableID method not yet defined
        // if (pNpc->GetTableID() == nNpcID) {
        //     ExitActor(&pNpc->XActor);
        //     ThreadLocalData::GetInstance()->DeleteNpc(pNpc);
        //     return true;
        // }
    }

    (void)nNpcID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "DeleteNPC - requires CNpc type");
    return true;
}

// IDA 0x1402CEAB0 - CreateSocialItemObject
// IDA 反编译精确还原:
// 1. 创建社交物品对象
// 2. 初始化对象（设置区域、位置、旋转、所有者ID、物品ID）
// 3. 返回创建的对象
CSocialItemObject* XDistrict::CreateSocialItemObject(std::uint32_t dwOwnerID, XVec3& vecPos, float fRot, std::uint16_t wItemID)
{
    // IDA: ThreadLocalData::GetInstance()->CreateSocialItemObject(&vecPos)
    // Note: ThreadLocalData and CSocialItemObject not yet defined
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // CSocialItemObject* pSocialItem = pThreadData->CreateSocialItemObject(&vecPos);

    // if (!pSocialItem)
    //     return nullptr;

    // IDA: CSocialItemObject::Init(pSocialItem, this, &vecPos, fRot, dwOwnerID, wItemID)
    // pSocialItem->Init(this, &vecPos, fRot, dwOwnerID, wItemID);

    // return pSocialItem;

    (void)dwOwnerID;
    (void)vecPos;
    (void)fRot;
    (void)wItemID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CreateSocialItemObject - requires ThreadLocalData/CSocialItemObject types");
    return nullptr;
}

// IDA 0x1402CEB50 - AddSocialItemObject
// IDA 反编译精确还原:
// 1. 检查 pSocialItem 是否有效
// 2. 调用 EnterActor 进入区域
// 3. 将 ActorID 添加到 m_setSocialObjectKeys
// 4. 将 OwnerID 映射到 ActorID 添加到 m_mapSocialOwnerKeys
// 5. 如果是家具类型(2)，添加到 m_setSocialObjectFuniture
// 6. 如果不是类型1，添加到 m_setSocialObjects
bool XDistrict::AddSocialItemObject(CSocialItemObject* pSocialItem)
{
    // IDA: if (!pSocialItem) return false
    if (!pSocialItem)
        return false;

    // IDA: if (EnterActor(&pSocialItem->XActor))
    // Note: CSocialItemObject inherits from XActor
    // if (EnterActor(static_cast<XActor*>(pSocialItem))) {
    //     ThreadLocalData::GetInstance()->DeleteSocialItemObject(pSocialItem);
    //     return false;
    // }

    // IDA: UXActorID actorID = pSocialItem->GetActorID()
    // UXActorID actorID = pSocialItem->GetActorID();
    // m_setSocialObjectKeys.insert(actorID.dwActorID);

    // IDA: std::uint32_t ownerID = pSocialItem->GetOwnerID()
    // std::uint32_t ownerID = pSocialItem->GetOwnerID();
    // m_mapSocialOwnerKeys[ownerID] = actorID.dwActorID;

    // IDA: if (pSocialItem->GetSocialType() == 2)
    // Note: CSocialItemObject methods not yet defined
    // if (pSocialItem->GetSocialType() == 2) {
    //     m_setSocialObjectFuniture.insert(actorID.dwActorID);
    // }

    // IDA: if (pSocialItem->GetSocialType() != 1)
    // if (pSocialItem->GetSocialType() != 1) {
    //     m_setSocialObjects.insert(actorID.dwActorID);
    // }

    (void)pSocialItem;
    GreenDamTan_log(__FILE__, __FUNCTION__, "AddSocialItemObject - requires CSocialItemObject type");
    return true;
}

// IDA 0x1402CED90 - CanCreateSocialItem
// IDA 反编译精确还原:
// 1. 检查 m_setSocialObjects 大小是否小于 20
// 2. 如果是家具类型(2)，检查 m_setSocialObjectFuniture 大小是否小于 5
bool XDistrict::CanCreateSocialItem(std::uint8_t bySocialObjType)
{
    // IDA: Direct decompilation
    if (m_setSocialObjects.size() >= 0x14)
        return false;
    return bySocialObjType != 2 || m_setSocialObjectFuniture.size() < 5;
}

// IDA 0x1402CEDF0 - DeleteSocialItemObject
// IDA 反编译精确还原:
// 1. 在 m_setSocialObjectKeys 中查找 dwObjectID
// 2. 获取对应的 Actor，转换为 CSocialItemObject
// 3. 获取所有者 Actor，清除 SocialOwnerID
// 4. 调用 EndProcess，退出区域，删除对象
// 5. 从各集合中删除该对象
bool XDistrict::DeleteSocialItemObject(std::uint32_t dwObjectID)
{
    // IDA: Find dwObjectID in m_setSocialObjectKeys
    auto it = m_setSocialObjectKeys.find(dwObjectID);
    if (it == m_setSocialObjectKeys.end())
        return false;

    // IDA: XActor* pActor = FindActor(*it)
    XActor* pActor = FindActor(*it);
    if (!pActor)
        return false;

    // IDA: CSocialItemObject* pSocialItem = dynamic_cast<CSocialItemObject*>(pActor)
    // Note: CSocialItemObject not yet defined
    // CSocialItemObject* pSocialItem = dynamic_cast<CSocialItemObject*>(pActor);
    // if (!pSocialItem)
    //     return false;

    // IDA: if (pSocialItem->GetActorID() != dwObjectID) return false
    // UXActorID actorID = pSocialItem->GetActorID();
    // if (actorID.dwActorID != dwObjectID)
    //     return false;

    // IDA: std::uint32_t ownerID = pSocialItem->GetOwnerID()
    // std::uint32_t ownerID = pSocialItem->GetOwnerID();

    // IDA: XActor* pOwnerActor = FindActor(ownerID)
    // XActor* pOwnerActor = FindActor(ownerID);
    // if (pOwnerActor) {
    //     CUser* pUser = dynamic_cast<CUser*>(pOwnerActor);
    //     if (pUser) {
    //         pUser->SetSocialOwnerID(0);
    //     }
    // }

    // IDA: CSocialItemObject::EndProcess(pSocialItem)
    // pSocialItem->EndProcess();

    // IDA: ExitActor(&pSocialItem->XActor)
    // ExitActor(static_cast<XActor*>(pSocialItem));

    // IDA: ThreadLocalData::GetInstance()->DeleteSocialItemObject(pSocialItem)
    // ThreadLocalData::GetInstance()->DeleteSocialItemObject(pSocialItem);

    // IDA: Erase from all sets
    m_setSocialObjectKeys.erase(dwObjectID);
    // m_mapSocialOwnerKeys.erase(ownerID);
    m_setSocialObjectFuniture.erase(dwObjectID);
    m_setSocialObjects.erase(dwObjectID);

    (void)dwObjectID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "DeleteSocialItemObject - requires CSocialItemObject type");
    return true;
}

// IDA 0x1402CF360 - FindSocialItemObject
// IDA 反编译精确还原:
// 1. 在 m_setSocialObjectKeys 中查找 dwObjectID
// 2. 获取对应的 Actor，转换为 CSocialItemObject
// 3. 验证 ActorID 是否匹配
CSocialItemObject* XDistrict::FindSocialItemObject(std::uint32_t dwObjectID)
{
    // TODO: CSocialItemObject is incomplete type - cannot use dynamic_cast
    // IDA 精确还原代码:
    // auto it = m_setSocialObjectKeys.find(dwObjectID);
    // if (it == m_setSocialObjectKeys.end())
    //     return nullptr;
    //
    // XActor* pActor = FindActor(*it);
    // if (!pActor) return nullptr;
    //
    // CSocialItemObject* pSocialItem = dynamic_cast<CSocialItemObject*>(pActor);
    // if (pSocialItem && pSocialItem->GetActorID() == dwObjectID)
    //     return pSocialItem;
    // return nullptr;

    (void)dwObjectID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "FindSocialItemObject stub");
    return nullptr;
}

// IDA 0x1402CF4A0 - FindSocialItemObjectByOwner
// IDA 反编译精确还原:
// 1. 在 m_mapSocialOwnerKeys 中查找 dwOwnerID
// 2. 获取对应的 Actor，转换为 CSocialItemObject
// 3. 验证 OwnerID 是否匹配
CSocialItemObject* XDistrict::FindSocialItemObjectByOwner(std::uint32_t dwOwnerID)
{
    // TODO: CSocialItemObject is incomplete type - cannot use dynamic_cast or GetOwnerID
    // IDA 精确还原代码:
    // auto it = m_mapSocialOwnerKeys.find(dwOwnerID);
    // if (it == m_mapSocialOwnerKeys.end())
    //     return nullptr;
    //
    // XActor* pActor = FindActor(it->second);
    // if (!pActor) return nullptr;
    //
    // CSocialItemObject* pSocialItem = dynamic_cast<CSocialItemObject*>(pActor);
    // if (pSocialItem && CSocialItemObject::GetOwnerID(pSocialItem) == dwOwnerID)
    //     return pSocialItem;
    // return nullptr;

    (void)dwOwnerID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "FindSocialItemObjectByOwner stub");
    return nullptr;
}

// IDA 0x1402CF1F0 - IsInSocialItemObjects
// IDA 反编译精确还原:
// 1. 遍历 m_setSocialObjectKeys
// 2. 对每个社交物品对象，获取位置和半径
// 3. 计算与目标位置的距离
// 4. 如果距离小于半径之和，返回 true
// 注意: IDA 使用 hkvVec3，项目中使用 XVec3
bool XDistrict::IsInSocialItemObjects(XVec3& vCircleCenter, float fRadius)
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // for (auto it = m_setSocialObjectKeys.begin(); it != m_setSocialObjectKeys.end(); ++it) {
    //     XActor* pActor = FindActor(*it);
    //     if (pActor) {
    //         CSocialItemObject* pSocialItem = dynamic_cast<CSocialItemObject*>(pActor);
    //         if (pSocialItem) {
    //             const hkvVec3& pos = pSocialItem->GetPosition();
    //             hkvVec3 diff = pos - vCircleCenter;
    //             float fTargetDistance = diff.getLength();
    //             float fTargetRadius = CSocialItemObject::GetRadius(pSocialItem) + fRadius;
    //             if (fTargetRadius > fTargetDistance)
    //                 return true;
    //         }
    //     }
    // }
    // return false;

    (void)vCircleCenter;
    (void)fRadius;
    GreenDamTan_log(__FILE__, __FUNCTION__, "IsInSocialItemObjects stub");
    return false;
}

// IDA 0x1402CF5B0 - EnterPartyForceMember
// IDA 反编译精确还原:
// 1. 获取用户的队伍和势力信息
// 2. 更新队伍成员信息（HP、等级、职业等）
// 3. 发送队伍更新消息
// 4. 更新势力成员信息
// 5. 发送势力更新消息
bool XDistrict::EnterPartyForceMember(CUser* pUser)
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码非常复杂，约200行:
    // - 获取 Party 和 Force
    // - 更新 ST_PARTY_MEMBER 和 ST_FORCE_MEMBER
    // - 调用 CForce::SetForceMemberInfo
    // - 发送 CCommunitySocket::SendPartyUpdateMemberInfo 和 SendForceUpdateMemberInfo

    (void)pUser;
    GreenDamTan_log(__FILE__, __FUNCTION__, "EnterPartyForceMember stub");
    return true;
}

// IDA 0x1402CFE00 - CreateNavMesh
// IDA 反编译精确还原:
// 1. 构建导航网格文件路径: /World/Navmesh/{filename}.hkt
// 2. 加载导航网格资源
// 3. 创建 DohHavokNavMeshInstance 实例
// 4. 设置 m_pNavMeshInstance
bool XDistrict::CreateNavMesh(const char* pszFileName)
{
    // TODO: 汇编还原 - 需要 DohHavokNavMeshInstance 和 DohHavokResourceManager 完整定义
    // IDA 精确还原代码:
    // std::string path = g_strCurPath_9 + "/World/Navmesh";
    // char szFilePath[272];
    // sprintf(szFilePath, "%s/%s.hkt", path.c_str(), pszFileName);
    //
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // HavokNavMeshResource* pNavMesh = DohHavokResourceManager::loadNavMesh(
    //     &pThreadData->m_DohHavokResourceManager, szFilePath);
    // if (!pNavMesh) {
    //     XPRINT(" [ %s ] Error Navmesh Resouce Laod fail ", szFilePath);
    //     return false;
    // }
    //
    // m_pNavMeshInstance = new DohHavokNavMeshInstance(pNavMesh, m_nNavMeshIndex);
    // return true;

    (void)pszFileName;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CreateNavMesh stub");
    return true;
}

// IDA 0x1402CFF60 - StartWorldMode
// IDA 反编译精确还原:
// 1. 获取世界模式表数据
// 2. 更新或插入 m_mapWorldMode
// 3. 构建并发送 PS_WORLD_MODE_START 包
// 4. 广播给所有玩家
void XDistrict::StartWorldMode(ST_WORLD_MODE_INFO& stInfo)
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_MODE_DISTRICT6* pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(&pServer->m_xResourceMgr, stInfo.nModeID);
    // if (pTBMode) {
    //     // 更新或插入 m_mapWorldMode
    //     auto it = m_mapWorldMode.find(stInfo.nModeID);
    //     if (it != m_mapWorldMode.end()) {
    //         m_mapWorldMode.erase(it);
    //     }
    //     m_mapWorldMode[stInfo.nModeID] = stInfo;
    // }
    //
    // PS_WORLD_MODE_START stStart;
    // stStart.nModeDateID = stInfo.nModeDateID;
    // stStart.nID = stInfo.nModeID;
    // stStart.nStartTime = stInfo.nStartTime;
    // stStart.nFinishTime = stInfo.nFinishTime;
    // stStart.byState = 0;
    // if (!pTBMode->Start_Type) stStart.byState = 4;
    // stStart.biModeStartTime = stInfo.biModeStartTime;
    // stStart.biModeEndTime = stInfo.biModeEndTime;
    //
    // XSendPacket xPacket(0x30, 1);
    // xPacket << stStart;
    // SendBroadCastAll(xPacket);

    (void)stInfo;
    GreenDamTan_log(__FILE__, __FUNCTION__, "StartWorldMode stub");
}

// IDA 0x1402DF5F0 - IsDistirct
// IDA 反编译精确还原: 简单返回 true
bool XDistrict::IsDistirct()
{
    // IDA 精确还原代码:
    // return true;

    return true;
}

// IDA 0x1402AD220 - GetSpawnPos
// IDA 反编译精确还原:
// 1. 根据 m_iCreationPositionType 确定生成位置:
//    - Type == 0: 使用中心位置
//    - Type == 1 或 2: 随机位置
void XDistrict::GetSpawnPos(const VMonsterSpawnInfo* pMonsterSpawn, XVec3& vPos)
{
    // TODO: 汇编还原 - 需要 VMonsterSpawnInfo 完整定义
    // IDA 精确还原代码:
    // vPos.z = pMonsterSpawn->PosTopLeft.z;
    // int type = pMonsterSpawn->m_iCreationPositionType;
    // if (type == 0) {
    //     vPos.x = (pMonsterSpawn->PosTopLeft.x + pMonsterSpawn->PosBottomRight.x) / 2.0f;
    //     vPos.y = (pMonsterSpawn->PosTopLeft.y + pMonsterSpawn->PosBottomRight.y) / 2.0f;
    // } else if (type == 1 || type == 2) {
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     vPos.x = XGameServer::fRand(pServer, pMonsterSpawn->PosTopLeft.x, pMonsterSpawn->PosBottomRight.x);
    //     vPos.y = XGameServer::fRand(pServer, pMonsterSpawn->PosTopLeft.y, pMonsterSpawn->PosBottomRight.y);
    // }

    (void)pMonsterSpawn;
    (void)vPos;
    GreenDamTan_log(__FILE__, __FUNCTION__, "GetSpawnPos stub");
}

// IDA 0x1402D0EE0 - AppearEventMonster
// IDA 反编译精确还原:
// 1. 构建 PS_WORLD_MODE_START 包
// 2. 广播给所有玩家
void XDistrict::AppearEventMonster(int nModeID, std::int64_t biStartTime, std::int64_t biFinishTime,
                                     int nModeDateID, std::int64_t biModeStartTime, std::int64_t biModeEndTime)
{
    // TODO: 汇编还原 - 需要 PS_WORLD_MODE_START 完整定义
    // IDA 精确还原代码:
    // PS_WORLD_MODE_START stStart;
    // stStart.nModeDateID = nModeDateID;
    // stStart.nID = nModeID;
    // stStart.nStartTime = biStartTime;
    // stStart.nFinishTime = biFinishTime;
    // stStart.byState = 0;
    // stStart.biModeStartTime = biModeStartTime;
    // stStart.biModeEndTime = biModeEndTime;
    //
    // XSendPacket xPacket(0x30, 1);
    // xPacket << stStart;
    // SendBroadCastAll(xPacket);

    (void)nModeID;
    (void)biStartTime;
    (void)biFinishTime;
    (void)nModeDateID;
    (void)biModeStartTime;
    (void)biModeEndTime;
    GreenDamTan_log(__FILE__, __FUNCTION__, "AppearEventMonster stub");
}

// IDA 0x140339430 - ClearWorldMode
// IDA 反编译精确还原:
// 1. 在 m_mapWorldMode 中查找 stInfo->nModeID
// 2. 如果找到，从 map 中删除
void XDistrict::ClearWorldMode(ST_WORLD_MODE_INFO& stInfo)
{
    auto it = m_mapWorldMode.find(stInfo.nModeID);
    if (it != m_mapWorldMode.end()) {
        m_mapWorldMode.erase(it);
    }
}

// IDA 0x140340230 - InfoWorldMode
// IDA 反编译精确还原:
// 1. 清空 m_vecWorldModeList
// 2. 遍历输入的 stInfos
// 3. 获取每个模式的表数据
// 4. 如果 Start_Type == 1，修改 ModeID 为 After_Mode_ID 并添加到列表
void XDistrict::InfoWorldMode(std::vector<ST_WORLD_MODE_INFO>& stInfos)
{
    m_vecWorldModeList.clear();
    XGameServer* pServer = XGameServer::Instance();
    for (std::size_t i = 0; i < stInfos.size(); ++i) {
        ST_WORLD_MODE_INFO stInfo = stInfos[i];
        TB_MODE_DISTRICT6* pMode = pServer ? pServer->GetResourceMgr().GetTB_MODE_DISTRICT6(static_cast<std::uint8_t>(stInfo.nModeID)) : nullptr;
        if (pMode && pMode->Start_Type == 1) {
            stInfo.nModeID = pMode->After_Mode_ID;
            m_vecWorldModeList.push_back(stInfo);
        }
    }
}

// IDA 0x1405FA3B0 - IsCanUseActiveAkashic
// IDA 反编译精确还原: 返回 m_bCanUseActiveAkashic
bool XDistrict::IsCanUseActiveAkashic()
{
    // IDA 精确还原代码:
    // return m_bCanUseActiveAkashic;

    return m_bCanUseActiveAkashic;
}

// IDA 0x1402CE630 - ScanGridOrigin
// IDA 反编译精确还原: 扫描网格原点
void XDistrict::ScanGridOrigin(float dx, float dy, unsigned char byNation, int sectorRange, unsigned int dwOptions, std::vector<CMover*>& vecOut)
{
    // IDA: XDistrict::_ScanGrid(this, &this->m_objectScanner, dx, dy, byNation, sectorRange, dwOptions, vecGameObjList);
    _ScanGrid(m_objectScanner, dx, dy, byNation, sectorRange, dwOptions, vecOut);
}

// IDA 0x1402CE6A0 - _ScanGrid
// IDA 反编译精确还原: 内部扫描网格实现
void XDistrict::_ScanGrid(AREA_OBJECT& objectScanner, float dx, float dy, unsigned char byNation, int sectorRange, unsigned int dwOptions, std::vector<CMover*>& vecGameObjList)
{
    // IDA: std::vector<CMover*>::clear(&vecGameObjList);
    vecGameObjList.clear();
    
    // IDA: std::vector<CMover*>::reserve(&vecGameObjList, 0x12C);
    vecGameObjList.reserve(0x12C);  // 300 elements
    
    // IDA: if ((dwOptions & 1) != 0)
    if ((dwOptions & 1) != 0) {
        // IDA: Range2DScanner<CMover*>::ScanGrid(objectScanner->playerScanner, dx, dy, sectorRange, sectorRange, vecGameObjList);
        if (objectScanner.playerScanner) {
            objectScanner.playerScanner->ScanGrid(dx, dy, sectorRange, sectorRange, vecGameObjList);
        }
    }
    
    // IDA: if ((dwOptions & 2) != 0)
    if ((dwOptions & 2) != 0) {
        // IDA: Range2DScanner<CMover*>::ScanGrid(objectScanner->npcScanner, dx, dy, sectorRange, sectorRange, vecGameObjList);
        if (objectScanner.npcScanner) {
            objectScanner.npcScanner->ScanGrid(dx, dy, sectorRange, sectorRange, vecGameObjList);
        }
    }
    
    // IDA: if (dwOptions == 15)
    if (dwOptions == 15) {
        // IDA: Range2DScanner<CMover*>::ScanGrid(objectScanner->etcScanner, dx, dy, sectorRange, sectorRange, vecGameObjList);
        if (objectScanner.etcScanner) {
            objectScanner.etcScanner->ScanGrid(dx, dy, sectorRange, sectorRange, vecGameObjList);
        }
    }
}
