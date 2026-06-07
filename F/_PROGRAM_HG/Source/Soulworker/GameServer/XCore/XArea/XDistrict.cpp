#include "Soulworker/GameServer/XCore/XArea/XDistrict.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

// 前置声明
class XGameServer;

// Note: CUser is forward declared, we can only use pointers to it
// Full definition requires including User.h which creates circular dependencies

XDistrict::XDistrict()
    : XArea()
    , m_pObjectResource(nullptr)
    , m_objectScanner_dummy{}
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

    // TODO: 汇编还原 - 需要 XGameServer 和 XWorldResMgr 完整定义
    // IDA 精确还原代码:
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // m_pObjectResource = XWorldResMgr::GetResource(&pServer->m_xWorldResMgr, pDistrict->District_ID);
    // if (!m_pObjectResource) {
    //     LogHelper::LogError("game.system", "Create error - Cant read event info in map district[ DistrictID:%d ]", pDistrict->District_ID);
    //     return false;
    // }
    //
    // AREA_OBJECT::SetSize(&m_objectScanner, pDistrict->District_Start_X, pDistrict->District_Start_Y,
    //                      pDistrict->District_Size_X, pDistrict->District_Size_Y, 5000);
    // m_nMaxUserCount = 100;
    // m_setSocialObjectKeys.clear();
    // m_mapSocialOwnerKeys.clear();
    // m_setSocialObjectFuniture.clear();
    // m_setSocialObjects.clear();
    // m_bCanUseActiveAkashic = (pDistrict->Arkashic_Use != 1);
    //
    // if (!CreateNavMesh(pDistrict->District_BatName)) {
    //     LogHelper::LogError("game.contents", "Create error - Failed CreateNavMesh ( %s )", pDistrict->District_BatName);
    // }
    // if (m_pNavMeshInstance) {
    //     hkaiWorld* pWorld = m_pNavMeshInstance->GetWorld();
    //     if (pWorld) pWorld->stepSilhouettes(nullptr);
    // }
    // return true;

    GreenDamTan_log(__FILE__, __FUNCTION__, "Create stub");
    return true;
}

// IDA 0x1402C8850 - Clear district
// IDA 反编译精确还原:
// 1. 调用基类 Clear
// 2. 重置检查用户日志
// 3. 清空等待接收信息列表
void XDistrict::Clear() {
    // TODO: 汇编还原 - 需要基类完整定义
    // IDA 精确还原代码:
    // XArea::Clear();
    // m_nCheckUserLog = 0;
    // m_listWaitForRecvInfo.clear();

    GreenDamTan_log(__FILE__, __FUNCTION__, "Clear stub");
}

// IDA 0x1402C8810 - Init district
// IDA 反编译精确还原:
// 1. 调用 XIOCPServer::BackSends
// 2. 调用 SpawnGenerate
// 3. 重置 m_nCheckUserLog
bool XDistrict::Init() {
    // TODO: 汇编还原 - 需要 XIOCPServer 完整定义
    // IDA 精确还原代码:
    // XIOCPServer::BackSends(this);
    // XDistrict::SpawnGenerate(this);
    // m_nCheckUserLog = 0;
    // return true;

    GreenDamTan_log(__FILE__, __FUNCTION__, "Init stub");
    SpawnGenerate();
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
        return 0;

    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码（非常复杂，约200行）:
    // Range2DScanner<CMover*>* pTargetScanner = GetScanner(pActor);
    // if (!pTargetScanner) return 50001;
    //
    // float fx = pActor->m_vPos.x;
    // float fy = pActor->m_vPos.y;
    // float fz = pActor->m_vPos.z;
    //
    // if (XArea::EnterActor(this, pActor)) {
    //     LogHelper::LogError("game.contents", "EnterActor error - Already in map...");
    //     return 50001;
    // }
    //
    // pActor->SetArea(this);
    //
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (pUser) {
    //     XVec3 vPos(fx, fy, fz);
    //     if (CMover::GetHeight(&pUser->CMoverEx, &vPos, 2000.0f)) {
    //         XActor::SetWorldID(pActor, GetTBMapID());
    //         pActor->SetPosInfo(vPos);
    //     } else {
    //         // Invalid position - get start portal pos
    //         STPosInfo stStartPos;
    //         XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //         if (!XWorldResMgr::GetStartPortalPos(&pServer->m_xWorldResMgr, GetTBMapID(), &stStartPos)) {
    //             // Failed - use default position
    //             XActor::SetWorldID(pActor, 10003);
    //             XVec3 defaultPos(10228.0f, 10058.0f, 90.0f);
    //             pActor->SetPosInfo(defaultPos);
    //             // Send DB update packet...
    //             pActor->SetArea(nullptr);
    //             XArea::ExitActor(this, pActor);
    //             return 50001;
    //         }
    //         XActor::SetWorldID(pActor, GetTBMapID());
    //         pActor->SetPosInfo(stStartPos.vPos);
    //     }
    // }
    //
    // CMover* obj = dynamic_cast<CMover*>(pActor);
    // if (!Range2DScanner<CMover*>::Insert(pTargetScanner, fx, fy, &obj)) {
    //     LogHelper::LogError("game.contents", "EnterActor error - Actor is line over...");
    //     XArea::ExitActor(this, pActor);
    //     return 50001;
    // }
    //
    // std::vector<CMover*> vecPlayerList, vecObjList;
    // vecPlayerList.reserve(300);
    // vecObjList.reserve(300);
    //
    // if (XActor::GetType(pActor)) {
    //     Range2DScanner<CMover*>::ScanGrid(m_objectScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList);
    // } else {
    //     ++m_nPcCount;
    //     Range2DScanner<CMover*>::ScanGridAndSetObjCnt(m_objectScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList, 1);
    //     if (pUser) {
    //         pUser->ChangeBattlePose(false);
    //         pUser->CheckBuffByLocation(this);
    //     }
    // }
    //
    // Range2DScanner<CMover*>::ScanGrid(m_objectScanner.npcScanner, fx, fy, 2, 2, &vecObjList);
    // Range2DScanner<CMover*>::ScanGrid(m_objectScanner.etcScanner, fx, fy, 2, 2, &vecObjList);
    //
    // if (pUser && GetWorldType() == 2) {
    //     SendPlayerInfoAll(pUser);
    //     SendEnterPlayerInfo(pUser);
    // }
    //
    // ProcessEnterObject(pActor, vecPlayerList, vecObjList);
    //
    // if (pUser) {
    //     // Record district state, init super armor, enter party/force member, check option effects
    //     CUser::InitSuperArmorGage(pUser);
    //     EnterPartyForceMember(pUser);
    //     CMoverEx::CheckOptionEffectInvoke(&pUser->CMoverEx, EFFECT_CONDITION_EQUIP, &pUser->CMoverEx, 0.0f, EFFECT_INVOKE_DONT_CARE);
    // }
    // return 0;

    GreenDamTan_log(__FILE__, __FUNCTION__, "EnterActor stub");
}

// IDA 0x1402C8890 - Get scanner for actor
// IDA 反编译精确还原:
// 根据 Actor 类型返回对应的扫描器:
// - Type == 0 (Player): 返回 playerScanner
// - Type == 1 或 2 (NPC/Monster): 返回 npcScanner
// - 其他: 返回 etcScanner
Range2DScanner<CMover*>* XDistrict::GetScanner(XActor* pActor) {
    // TODO: 汇编还原 - 需要 XActor::GetType 和 AREA_OBJECT 完整定义
    // IDA 精确还原代码:
    // int Type = XActor::GetType(pActor);
    // if (Type == 0) return m_objectScanner.playerScanner;
    // if (Type > 0 && Type <= 2) return m_objectScanner.npcScanner;
    // return m_objectScanner.etcScanner;

    (void)pActor;
    GreenDamTan_log(__FILE__, __FUNCTION__, "GetScanner stub");
    return nullptr;
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
    // IDA: m_uxMapID.nMapID << 16 >> 48 to get district index
    std::int64_t nDistrictIndex = static_cast<std::int64_t>(m_uxMapID.nMapID) << 16 >> 48;

    // TODO: 汇编还原 - 需要 XGameServer 完整定义
    // IDA 精确还原代码:
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_DISTRICT* pTBDistrict = XResourceMgr::GetTB_DISTRICT(&pServer->m_xResourceMgr, nDistrictIndex);
    // return pTBDistrict ? pTBDistrict->District_Type : 0;

    (void)nDistrictIndex;
    return 0;
}

// IDA 0x1402D1010 - Handle load complete for actor
// IDA 反编译精确还原:
// 1. pUser = dynamic_cast<CUser*>(pActor)
// 2. if (!pUser) return
// 3. CUser::SetClientLoadComplete(pUser, true)
// 4. CGocInventory::InitItemCoolTime() + SendItemCoolTimeInfo()
// 5. CGocEntity::IsRoguelikeState() -> 如果是 Roguelike 状态则踢出用户
void XDistrict::LoadComplete(XActor* pActor) {
    if (!pActor) return;

    // TODO: 汇编还原 - 需要 RTTI dynamic_cast 和 CUser 完整定义
    // IDA 精确还原代码:
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) return;
    //
    // CUser::SetClientLoadComplete(pUser, true);
    //
    // // 初始化物品冷却时间
    // std::tr1::shared_ptr<CGocInventory> pInvenPtr;
    // CMover::GetGOC<CGocInventory>(&pUser->CMoverEx, &pInvenPtr, 0);
    // if (pInvenPtr) {
    //     pInvenPtr->InitItemCoolTime();
    //     pInvenPtr->SendItemCoolTimeInfo();
    // }
    //
    // // 检查 Roguelike 状态
    // std::tr1::shared_ptr<CGocEntity> pEntity;
    // CMover::GetGOC<CGocEntity>(&pUser->CMoverEx, &pEntity, 0);
    // if (pEntity && pEntity->IsRoguelikeState()) {
    //     UXActorID actorID;
    //     pUser->GetActorID(&actorID);
    //     LogHelper::LogError("game.item", "pEntity->IsRoguelikeState() %d / ( %d )", actorID.dwActorID, 2350);
    //
    //     PS_KICK_USER_INFO psKickoutInfo;
    //     psKickoutInfo.byKickType = 0;
    //     psKickoutInfo.dwUAID = pUser->GetUAID();
    //     pUser->Kickout(&psKickoutInfo, 0);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "LoadComplete stub");
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
    if (!pActor) return;

    // TODO: 汇编还原 - 需要 CUser RTTI 和 CGocNetwork::Send 完整定义
    // IDA 精确还原代码:
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) return;
    //
    // XSendPacket xSendPacket(0x30, 5);
    // xSendPacket << m_vecWorldModeList;
    // CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    //
    // unsigned __int64 nSize = m_vecWorldModeList.size();
    // unsigned __int16 wMapID = GetTBMapID();
    // LogHelper::LogDebug("game.contents", "InfoWorldMode - Map:%d, Size:%d", wMapID, nSize);

    GreenDamTan_log(__FILE__, __FUNCTION__, "SendWorldModeInfo stub");
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

    // TODO: 汇编还原 - 需要 CUser::SetInfoPacket 和 CGocNetwork::SendBroadCast 完整定义
    // IDA 精确还原代码:
    // XSendPacket xSendPacket(4, 0x51);
    // xSendPacket << 1;  // count = 1
    // pUser->SetInfoPacket(xSendPacket);
    // CGocNetwork::SendBroadCast(&pUser->CMoverEx, &xSendPacket, 2);

    GreenDamTan_log(__FILE__, __FUNCTION__, "SendEnterPlayerInfo stub");
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

    // TODO: 汇编还原 - 需要 CGocNetwork::SendBroadCast 完整定义
    // IDA 精确还原代码:
    // XSendPacket xSendPacket(4, 0x52);
    // UXActorID actorID;
    // pUser->GetActorID(&actorID);
    // xSendPacket << actorID.dwActorID;
    // CGocNetwork::SendBroadCast(&pUser->CMoverEx, &xSendPacket, 2);

    GreenDamTan_log(__FILE__, __FUNCTION__, "SendExitPlayerInfo stub");
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
void XDistrict::SendPlayerInfoAll(CUser* pUser) {
    if (!pUser) return;

    // TODO: 汇编还原 - 需要 Range2DScanner, CUser::GetMyCharInfoEx, CGocNetwork::Send 完整定义
    // IDA 精确还原代码:
    // std::vector<CMover*> vecPCList;
    // vecPCList.reserve(300);
    // Range2DScanner<CMover*>::Enumerate(m_objectScanner.playerScanner, &vecPCList);
    //
    // std::vector<STCharInfoEx> vecPCInfo;
    // vecPCInfo.reserve(300);
    //
    // for (auto it = vecPCList.begin(); it != vecPCList.end(); ++it) {
    //     CUser* pOtherPC = dynamic_cast<CUser*>(*it);
    //     if (pOtherPC && pOtherPC != pUser) {
    //         STCharInfoEx* pInfo = pOtherPC->GetMyCharInfoEx();
    //         vecPCInfo.push_back(*pInfo);
    //
    //         // 每66个玩家发送一次
    //         if (vecPCInfo.size() > 66) {
    //             XSendPacket xSendPacket(4, 0x51);
    //             xSendPacket << (short)vecPCInfo.size();
    //             for (auto& info : vecPCInfo) {
    //                 xSendPacket << info;
    //             }
    //             CGocNetwork::Send(&pUser->XActor, &xSendPacket);
    //             vecPCInfo.clear();
    //         }
    //     }
    // }
    //
    // // 发送剩余玩家
    // XSendPacket packet(4, 0x51);
    // packet << (short)vecPCInfo.size();
    // for (auto& info : vecPCInfo) {
    //     packet << info;
    // }
    // CGocNetwork::Send(&pUser->XActor, &packet);

    GreenDamTan_log(__FILE__, __FUNCTION__, "SendPlayerInfoAll stub");
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
    if (!stInfo) return;

    // TODO: 汇编还原 - 需要 SendBroadCastAll, GetTB_MODE_DISTRICT6, GetTB_MODE_DISTRICT6_DATE 完整定义
    // IDA 精确还原代码:
    // XSendPacket xSendPacket(0x30, 3);
    // xSendPacket << *stInfo;
    // SendBroadCastAll(&xSendPacket);
    //
    // auto it = m_mapWorldMode.find(stInfo->nModeID);
    // if (it != m_mapWorldMode.end()) {
    //     it->second.nMonsterClearCount = stInfo->nMonsterClearCount;
    //     it->second.nState = 2;  // Finish
    //     it->second.bSuccess = stInfo->bSuccess;
    //     it->second.nFinishTime = stInfo->nFinishTime;
    //
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     TB_MODE_DISTRICT6* pTBMode = XResourceMgr::GetTB_MODE_DISTRICT6(&pServer->m_xResourceMgr, stInfo->nModeID);
    //     if (pTBMode && pTBMode->Start_Type == 0) {
    //         // 更新 m_vecWorldModeList
    //         for (size_t i = 0; i < m_vecWorldModeList.size(); ++i) {
    //             if (m_vecWorldModeList[i].nModeDateID == stInfo->nModeDateID) {
    //                 m_vecWorldModeList[i].nMonsterClearCount = stInfo->nMonsterClearCount;
    //             }
    //         }
    //
    //         // 根据 Clear_Count 设置 WorldModeBoost
    //         TB_MODE_DISTRICT6_DATE* pModeDate = XResourceMgr::GetTB_MODE_DISTRICT6_DATE(&pServer->m_xResourceMgr, stInfo->nModeDateID);
    //         if (pModeDate && stInfo->nMonsterClearCount > 0) {
    //             for (int j = 0; j < 5 && pModeDate->Clear_Count[j]; ++j) {
    //                 if (stInfo->nMonsterClearCount >= pModeDate->Clear_Count[j] &&
    //                     (j == 4 || stInfo->nMonsterClearCount < pModeDate->Clear_Count[j + 1])) {
    //                     if (pModeDate->Clear_Booster[j]) {
    //                         SetWorldModeBoostAll(pModeDate->Clear_Booster[j], stInfo->nFinishTime + pModeDate->Booster_Limit_Time);
    //                     }
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "FinishWorldMode stub");
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

    // TODO: 汇编还原 - 需要 InfoWorldMode 完整定义
    // IDA 精确还原代码:
    // InfoWorldMode(stInfoVec);
    //
    // for (auto it = stInfoVec->begin(); it != stInfoVec->end(); ++it) {
    //     ST_WORLD_MODE_INFO stInfo = *it;
    //
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     if (!XResourceMgr::GetTB_MODE_DISTRICT6(&pServer->m_xResourceMgr, stInfo.nModeID))
    //         continue;
    //
    //     if (stInfo.nState == 1 || stInfo.nState == 2) {
    //         // Start or Finish
    //         auto mapIt = m_mapWorldMode.find(stInfo.nModeID);
    //         if (mapIt != m_mapWorldMode.end()) {
    //             mapIt->second = stInfo;
    //         } else {
    //             m_mapWorldMode.insert(std::make_pair(stInfo.nModeID, stInfo));
    //         }
    //         LogHelper::LogInfo("game.contents", "[WORLD_MODE] Sync Start or Finish %d %d %d",
    //             stInfo.nModeID, stInfo.nState, stInfo.nModeDateID);
    //     }
    //     else if (stInfo.nState == 0) {
    //         // Ready - remove from map
    //         auto mapIt = m_mapWorldMode.find(stInfo.nModeID);
    //         if (mapIt != m_mapWorldMode.end()) {
    //             m_mapWorldMode.erase(mapIt);
    //         }
    //         LogHelper::LogInfo("game.contents", "[WORLD_MODE] Sync Ready %d %d",
    //             stInfo.nModeID, stInfo.nModeDateID);
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "SyncWorldMode stub");
}

// IDA 0x1402D0FE0 - Set object info request (add user to wait list)
void XDistrict::SetObjectInfoReq(CUser* pUser) {
    if (!pUser) return;

    // IDA反编译结果：将用户添加到等待接收信息列表
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

    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // Range2DScanner<CMover*>* pTargetScanner = GetScanner(pActor);
    // if (!pTargetScanner) return 50001;
    //
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (pUser) {
    //     CMoverEx::RemoveAllOptionEffect(&pUser->CMoverEx);
    //     // ... quest sync
    // }
    //
    // float fx = pActor->m_vPos.x;
    // float fy = pActor->m_vPos.y;
    // CMover* obj = dynamic_cast<CMover*>(pActor);
    //
    // if (Range2DScanner<CMover*>::Erase(pTargetScanner, fx, fy, &obj)) {
    //     std::vector<CMover*> vecPlayerList;
    //     vecPlayerList.reserve(300);
    //
    //     if (XActor::GetType(pActor)) {
    //         Range2DScanner<CMover*>::ScanGrid(m_objectScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList);
    //     } else {
    //         --m_nPcCount;
    //         Range2DScanner<CMover*>::ScanGridAndSetObjCnt(m_objectScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList, -1);
    //     }
    //
    //     if (pUser && GetWorldType() == 2) {
    //         SendExitPlayerInfo(pUser);
    //     }
    //
    //     ProcessLeaveObject(pActor, vecPlayerList, true);
    //     pActor->SetArea(nullptr);
    //     if (pUser) CUser::Exit(pUser);
    //     return XArea::ExitActor(pActor);
    // }
    // return 50001;

    GreenDamTan_log(__FILE__, __FUNCTION__, "ExitActor stub");
    // Note: Base class XArea::ExitActor returns void
}

// IDA 0x1402CC060 - SendBroadCast
// IDA 反编译精确还原:
// 1. 如果 eBroadCastType == eAll_InMap, 调用 SendBroadCastAll
// 2. 否则获取pActor位置
// 3. 扫描周围玩家
// 4. 调用 CGocNetwork::Send 广播消息
void XDistrict::SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType)
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (eBroadCastType == eAll_InMap) {
    //     SendBroadCastAll(packet);
    //     return;
    // }
    //
    // if (!pActor) return;
    //
    // XActor* pExcept = nullptr;
    // if (eBroadCastType == eNoneSelf) {
    //     pExcept = pActor;
    // }
    //
    // float fx = pActor->m_vPos.x;
    // float fy = pActor->m_vPos.y;
    //
    // std::vector<CMover*> vecPlayerList;
    // vecPlayerList.reserve(300);
    // Range2DScanner<CMover*>::ScanGrid(m_objectScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList);
    // CGocNetwork::Send(vecPlayerList, packet, pExcept);

    GreenDamTan_log(__FILE__, __FUNCTION__, "SendBroadCast stub");
}

// IDA 0x1402CC180 - SendBroadCastAll
// IDA 反编译精确还原:
// 1. 枚举所有玩家
// 2. 遍历并发送消息给每个玩家
void XDistrict::SendBroadCastAll(XSendPacket& packet)
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // std::vector<CMover*> vecPCList;
    // Range2DScanner<CMover*>::Enumerate(m_objectScanner.playerScanner, &vecPCList);
    //
    // for (auto it = vecPCList.begin(); it != vecPCList.end(); ++it) {
    //     CMover* pMover = *it;
    //     if (pMover && pMover->m_pGOC) {
    //         XActor* pActor = (XActor*)((char*)pMover + 872);
    //         CGocNetwork::Send(pActor, packet);
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "SendBroadCastAll stub");
}

// IDA 0x1402CC7D0 - OnUpdate
// IDA 反编译精确还原:
// 1. 处理等待接收信息的用户列表
// 2. 遍历所有Actor，更新或删除无效用户
// 3. 检查社交物品过期时间
// 4. 定期记录用户数量日志
void XDistrict::OnUpdate(float fElapsed)
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码非常复杂，包含:
    // - 遍历 m_listWaitForRecvInfo，检查用户DB加载状态
    // - 遍历 m_mapActor，更新有效用户，删除无效用户
    // - 检查社交物品 CSocialItemObject::CheckRemainTime
    // - 每分钟记录一次用户数量日志

    (void)fElapsed;
    GreenDamTan_log(__FILE__, __FUNCTION__, "OnUpdate stub");
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

    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // float sx = pActor->m_vPos.x;
    // float sy = pActor->m_vPos.y;
    // float dx = vPos.x;
    // float dy = vPos.y;
    //
    // if (!bByForce && !IsAroundSector(sx, sy, dx, dy)) {
    //     LogHelper::LogError("game.contents", "MoveActor error - Failed user movement...");
    //     return 1;
    // }
    //
    // if (IsSameSector(sx, sy, dx, dy)) {
    //     pActor->SetPosInfo(vPos);
    //     return 0;
    // }
    //
    // Range2DScanner<CMover*>* pScanner = GetScanner(pActor);
    // if (!pScanner) return 1;
    //
    // CMover* obj = dynamic_cast<CMover*>(pActor);
    // if (!Range2DScanner<CMover*>::Move(pScanner, sx, sy, dx, dy, obj)) {
    //     LogHelper::LogError("game.contents", "MoveActor error - Failed user movement...");
    //     return 1;
    // }
    //
    // pActor->SetPosInfo(vPos);
    //
    // std::vector<CMover*> vecEnterPlayerList, vecLeavePlayerList;
    // std::vector<CMover*> vecEnterObjList, vecLeaveObjList;
    // // ... scan and process move
    // ProcessMoveObject(pActor, vecEnterPlayerList, vecEnterObjList, vecLeavePlayerList, vecLeaveObjList);
    // return 0;

    (void)vPos;
    (void)fRot;
    (void)bByForce;
    GreenDamTan_log(__FILE__, __FUNCTION__, "MoveActor stub");
    return 0;
}

// IDA 0x1402CDB30 - IsAroundSector
// IDA 反编译精确还原:
// 1. 计算两个位置的坐标
// 2. 比较坐标差值是否在2以内
bool XDistrict::IsAroundSector(float fx, float fy, float fx2, float fy2)
{
    // TODO: 汇编还原 - 需要 Range2DScanner 完整定义
    // IDA 精确还原代码:
    // Range2DScanner<CMover*>::Coord coord1, coord2;
    // Range2DScanner<CMover*>::CalcCoordFromPos(m_objectScanner.playerScanner, &coord1, fx, fy);
    // Range2DScanner<CMover*>::CalcCoordFromPos(m_objectScanner.playerScanner, &coord2, fx2, fy2);
    // return abs(coord1.x - coord2.x) <= 2 && abs(coord1.y - coord2.y) <= 2;

    (void)fx; (void)fy; (void)fx2; (void)fy2;
    return true; // 简化实现
}

// IDA 0x1402CDBD0 - IsSameSector
// IDA 反编译精确还原:
// 1. 计算两个位置的坐标
// 2. 比较坐标是否相同
bool XDistrict::IsSameSector(float fx, float fy, float fx2, float fy2)
{
    // TODO: 汇编还原 - 需要 Range2DScanner 完整定义
    // IDA 精确还原代码:
    // Range2DScanner<CMover*>::Coord coord1, coord2;
    // Range2DScanner<CMover*>::CalcCoordFromPos(m_objectScanner.playerScanner, &coord1, fx, fy);
    // Range2DScanner<CMover*>::CalcCoordFromPos(m_objectScanner.playerScanner, &coord2, fx2, fy2);
    // return coord1 == coord2;

    (void)fx; (void)fy; (void)fx2; (void)fy2;
    return true; // 简化实现
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

    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (!GetScanner(pActor)) return false;
    //
    // float fx = pActor->m_vPos.x;
    // float fy = pActor->m_vPos.y;
    //
    // std::vector<CMover*> vecPlayerList;
    // vecPlayerList.reserve(300);
    // Range2DScanner<CMover*>::ScanGrid(m_objectScanner.playerScanner, fx, fy, 2, 2, &vecPlayerList);
    //
    // std::vector<CMover*> vecObjList;
    // vecObjList.reserve(300);
    // Range2DScanner<CMover*>::ScanGrid(m_objectScanner.npcScanner, fx, fy, 2, 2, &vecObjList);
    // Range2DScanner<CMover*>::ScanGrid(m_objectScanner.etcScanner, fx, fy, 2, 2, &vecObjList);
    //
    // if (XActor::IsPlayer(pActor)) {
    //     ProcessSendEnterObjectListToPlayer(pActor, vecPlayerList, vecObjList);
    // }
    // SendWorldModeInfo(pActor);
    // SetWorldModeSync(pActor);
    // return true;

    GreenDamTan_log(__FILE__, __FUNCTION__, "SendObjectInfo stub");
    return true;
}

// IDA 0x1402CD4C0 - IsValidPosition
// IDA 反编译精确还原:
// 调用 Range2DScanner::IsValidPos 检查位置是否有效
bool XDistrict::IsValidPosition(XVec3& vPos)
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // return Range2DScanner<CMover*>::IsValidPos(m_objectScanner.playerScanner, vPos.x, vPos.y);

    (void)vPos;
    GreenDamTan_log(__FILE__, __FUNCTION__, "IsValidPosition stub");
    return true;
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

    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) return false;
    //
    // XArea* pArea = pActor->GetArea();
    // if (pArea->GetWorldType() == 2) {
    //     // 战场类型
    //     int nEscapePoint = pUser->GetRevivePoint();
    //     int nMapID = GetTBMapID();
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     if (!XWorldResMgr::GetPortalPos(&pServer->m_xWorldResMgr, nMapID, nEscapePoint, &stMovePos)) {
    //         XWorldResMgr::GetStartPortalPos(&pServer->m_xWorldResMgr, nMapID, &stMovePos);
    //     }
    // } else {
    //     int nMapID = GetTBMapID();
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     if (!XWorldResMgr::GetStartPortalID(&pServer->m_xWorldResMgr, nMapID, &nJumpID, &stMovePos)) {
    //         return false;
    //     }
    // }
    //
    // MoveActor(pActor, stMovePos.vPos);
    // CMover::MoveingValueClear(&pUser->CMoverEx);
    // pUser->ChangeMotion(1, 1, 0);
    // pUser->SendResWarp(0, stMovePos.vPos, stMovePos.fRot);
    // return true;

    GreenDamTan_log(__FILE__, __FUNCTION__, "EscapeActor stub");
    return true;
}

// IDA 0x1402CC270 - SpawnGenerate
// IDA 反编译精确还原:
// 1. 获取对象资源映射
// 2. 遍历所有对象
// 3. 如果是Box类型且启用，执行生成
void XDistrict::SpawnGenerate()
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // VMap<int, void*> ObjectMap(10);
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // CFAutoSlimWriteLock lock(&pServer->m_rwMapLock);
    // ObjectMap = m_pObjectResource->GetObjectMap();
    //
    // void* Iter = ObjectMap.GetStartPosition();
    // while (Iter) {
    //     int iKey;
    //     void* pValue;
    //     ObjectMap.GetNextPair(&Iter, &iKey, &pValue);
    //
    //     VEventObjectInfo* pInfo = (VEventObjectInfo*)pValue;
    //     if (pInfo && pInfo->eType == eEventObjectType_Box) {
    //         VEventBoxInfo* pBoxInfo = (VEventBoxInfo*)pInfo;
    //         if (pBoxInfo->bEnable == 1 && pBoxInfo->bSpawn == 1) {
    //             ExcuteSpawnBox((VMonsterSpawnInfo*)pBoxInfo, eSendInfoTypeNot);
    //         }
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "SpawnGenerate stub");
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

    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (bLeave == 1) {
    //     unsigned int dwOwnerID = CUser::GetSocialOwnerID(pUser);
    //     CSocialItemObject* pOwnerObject = XDistrict::FindSocialItemObject(this, dwOwnerID);
    //     if (pOwnerObject) {
    //         unsigned int QuestID = CQuestCondition::GetQuestID(pUser->GetActorID());
    //         CSocialItemObject::FinishPlaySocialItemObject(pOwnerObject, QuestID);
    //         this->DeleteSocialItemObject(this, dwOwnerID);
    //         return true;
    //     }
    // }
    //
    // unsigned int dwObjectID = CUser::GetSocialUseID(pUser);
    // CSocialItemObject* pSocialItemObject = XDistrict::FindSocialItemObject(this, dwObjectID);
    // if (!pSocialItemObject)
    //     return false;
    //
    // unsigned int QuestID = CQuestCondition::GetQuestID(pUser->GetActorID());
    // if (!CSocialItemObject::IsExistUser(pSocialItemObject, QuestID))
    //     return false;
    //
    // CSocialItemObject::FinishPlaySocialItemObject(pSocialItemObject, QuestID);
    // if (!CSocialItemObject::DeleteUser(pSocialItemObject, QuestID))
    //     return false;
    //
    // const ST_SOCIAL_ITEM_INFO* pItemInfo = CSocialItemObject::GetItemInfo(pSocialItemObject);
    // ST_SOCIAL_ITEM_INFO stItemInfo(pItemInfo);
    //
    // if (CSocialItemObject::GetSocialType(pSocialItemObject) == 3) { // Furniture type
    //     unsigned int userQuestID = CQuestCondition::GetQuestID(pUser->GetActorID());
    //     if (stItemInfo.dwOwnerID == userQuestID ||
    //         CSocialItemObject::GetSocialPlayState(pSocialItemObject) != E_SOCIAL_OBJECT_STATE_WAIT) {
    //         this->DeleteSocialItemObject(this, dwObjectID);
    //     }
    // }
    //
    // CMoverEx::RemoveAuraSkill(&pUser->CMoverEx, 1);
    // CUser::SetSocialUseID(pUser, 0);
    //
    // PS_SOCIALITEM_STOP_RES stStopRes;
    // stStopRes.dwActorID = CQuestCondition::GetQuestID(pUser->GetActorID());
    // stStopRes.stItemInfo = stItemInfo;
    // stStopRes.vPos = CUser::GetMyroomBackupPos(pUser);
    // stStopRes.fRot = CUser::GetMyroomBackupYaw(pUser);
    //
    // XSendPacket xSendPacket(0x2D, 3);
    // xSendPacket << stStopRes;
    // CGocNetwork::BroadcastNearby(pUser, nullptr, &xSendPacket);
    //
    // ST_SOCIAL_ITEM_INFO::~ST_SOCIAL_ITEM_INFO(&stItemInfo);
    // PS_SOCIALITEM_STOP_RES::~PS_SOCIALITEM_STOP_RES(&stStopRes);
    // return true;

    (void)bLeave;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ExitSocialItemObject stub");
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

    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // XVec3 vPos;
    // hkvVec3::hkvVec3(&vPos);
    //
    // for (int i = 0; i < 10; ++i) {
    //     if (pMonsterSpawn->m_stMonsterInfo[i].m_iID == 0)
    //         continue;
    //
    //     if (pMonsterSpawn->m_stMonsterInfo[i].m_iType == 1) {
    //         // NPC type
    //         this->GetSpawnPos_2(this, pMonsterSpawn, &vPos);
    //         ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    //         CNpc* pNpc = ThreadLocalData::CreateNpc(
    //             pThreadData, this, this->m_uxMapID, 0,
    //             pMonsterSpawn->m_stMonsterInfo[i].m_iID, &vPos, pMonsterSpawn->fRotate);
    //
    //         if (pNpc) {
    //             if (this->EnterActor(this, &pNpc->XActor)) {
    //                 ThreadLocalData::DeleteNpc(ThreadLocalData::GetInstance(), pNpc);
    //             } else {
    //                 CMoverEx::SetWayPointID(pNpc, pMonsterSpawn->m_iWaypoint);
    //                 CNpc::SetSpawnBoxID(pNpc, pMonsterSpawn->iID);
    //             }
    //         }
    //     }
    //     else if (pMonsterSpawn->m_stMonsterInfo[i].m_iType == 5) {
    //         // Social item type
    //         this->GetSpawnPos_2(this, pMonsterSpawn, &vPos);
    //         int wSocialItemID = pMonsterSpawn->m_stMonsterInfo[i].m_iID;
    //
    //         XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //         TB_SOCIAL_ITEM* pSocialItemTable = XResourceMgr::GetTB_SOCIAL_ITEM(&pServer->m_xResourceMgr, wSocialItemID);
    //         if (!pSocialItemTable)
    //             return;
    //
    //         float fRadius = pSocialItemTable->Collision_Range / 2.0f;
    //         CSocialItemObject* pSocialItemObject = XDistrict::CreateSocialItemObject(
    //             this, 0, &vPos, pMonsterSpawn->fRotate, wSocialItemID);
    //
    //         if (pSocialItemObject) {
    //             CSocialItemObject::SetRadius(pSocialItemObject, fRadius);
    //             CSocialItemObject::SetMaxCount(pSocialItemObject, pSocialItemTable->Use_Max);
    //
    //             TB_SOCIAL_ITEM* pTB_Social_Item = XResourceMgr::GetTB_SOCIAL_ITEM(
    //                 &pServer->m_xResourceMgr, wSocialItemID);
    //             if (pTB_Social_Item) {
    //                 CSocialItemObject::SetSocialType(pSocialItemObject, pTB_Social_Item->Social_Item_Type);
    //
    //                 if (pTB_Social_Item->Social_Item_Type == 1) { // Furniture
    //                     TB_MYROOM_FURNITURE* pTB_Furniture = XResourceMgr::GetTB_MYROOM_FURNITURE(
    //                         &pServer->m_xResourceMgr, pTB_Social_Item->Furniture_Object_ID);
    //                     if (!pTB_Furniture) {
    //                         LogHelper::LogError("game.item", "ExcuteSpawnBox error - SocialID:%d", wSocialItemID);
    //                         return;
    //                     }
    //                     CSocialItemObject::SetFurnitureInfo(pSocialItemObject, pTB_Furniture->Furniture_Item_Special_Use);
    //                 }
    //             }
    //
    //             unsigned int dwObjectID = CQuestCondition::GetQuestID(pSocialItemObject->GetActorID());
    //             if (!XDistrict::AddSocialItemObject(this, pSocialItemObject)) {
    //                 this->DeleteSocialItemObject(this, dwObjectID);
    //                 return;
    //             }
    //         }
    //     }
    // }

    (void)eType;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ExcuteSpawnBox stub");
}

// IDA 0x1402CE7F0 - SpawnNPC
// IDA 反编译精确还原:
// 1. 创建 NPC
// 2. 调用 EnterActor 进入区域
// 3. 成功后将 NPC ID 添加到生成列表
bool XDistrict::SpawnNPC(int nNpcID, XVec3& xPos, float fRot)
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // CNpc* pNpc = ThreadLocalData::CreateNpc(pThreadData, this, this->m_uxMapID, 0, nNpcID, &xPos, fRot);
    // if (pNpc) {
    //     if (!EnterActor(&pNpc->XActor)) {
    //         unsigned int dwActorID = CQuestCondition::GetQuestID(pNpc->GetActorID());
    //         m_listSpawnNPC.push_back(dwActorID);
    //         return true;
    //     }
    //     ThreadLocalData::DeleteNpc(ThreadLocalData::GetInstance(), pNpc);
    // }
    // return false;

    (void)nNpcID;
    (void)xPos;
    (void)fRot;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SpawnNPC stub");
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
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // for (auto it = m_listSpawnNPC.begin(); it != m_listSpawnNPC.end(); ++it) {
    //     unsigned int dwActorID = *it;
    //     XActor* pActor = FindActor(dwActorID);
    //     if (pActor) {
    //         CNpc* pNpc = dynamic_cast<CNpc*>(pActor);
    //         if (pNpc && pNpc->GetTableID() == nNpcID) {
    //             ExitActor(&pNpc->XActor);
    //             ThreadLocalData::DeleteNpc(ThreadLocalData::GetInstance(), pNpc);
    //             return true;
    //         }
    //     }
    // }
    // return false;

    (void)nNpcID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "DeleteNPC stub");
    return true;
}

// IDA 0x1402CEAB0 - CreateSocialItemObject
// IDA 反编译精确还原:
// 1. 创建社交物品对象
// 2. 初始化对象（设置区域、位置、旋转、所有者ID、物品ID）
// 3. 返回创建的对象
CSocialItemObject* XDistrict::CreateSocialItemObject(std::uint32_t dwOwnerID, XVec3& vecPos, float fRot, std::uint16_t wItemID)
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    // CSocialItemObject* pSocialItem = ThreadLocalData::CreateSocialItemObject(pThreadData, &vecPos);
    // if (!pSocialItem)
    //     return nullptr;
    // CSocialItemObject::Init(pSocialItem, this, &vecPos, fRot, dwOwnerID, wItemID);
    // return pSocialItem;

    (void)dwOwnerID;
    (void)vecPos;
    (void)fRot;
    (void)wItemID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CreateSocialItemObject stub");
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
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (!pSocialItem) return false;
    // if (EnterActor(&pSocialItem->XActor)) {
    //     ThreadLocalData::DeleteSocialItemObject(ThreadLocalData::GetInstance(), pSocialItem);
    //     return false;
    // }
    // UXActorID actorID = pSocialItem->GetActorID();
    // m_setSocialObjectKeys.insert(actorID);
    //
    // unsigned int ownerID = CSocialItemObject::GetOwnerID(pSocialItem);
    // m_mapSocialOwnerKeys[ownerID] = actorID;
    //
    // if (CSocialItemObject::GetSocialType(pSocialItem) == 2) {
    //     m_setSocialObjectFuniture.insert(actorID);
    // }
    // if (CSocialItemObject::GetSocialType(pSocialItem) != 1) {
    //     m_setSocialObjects.insert(actorID);
    // }
    // return true;

    (void)pSocialItem;
    GreenDamTan_log(__FILE__, __FUNCTION__, "AddSocialItemObject stub");
    return true;
}

// IDA 0x1402CED90 - CanCreateSocialItem
// IDA 反编译精确还原:
// 1. 检查 m_setSocialObjects 大小是否小于 20
// 2. 如果是家具类型(2)，检查 m_setSocialObjectFuniture 大小是否小于 5
bool XDistrict::CanCreateSocialItem(std::uint8_t bySocialObjType)
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (m_setSocialObjects.size() >= 20)
    //     return false;
    // if (bySocialObjType != 2)
    //     return true;
    // return m_setSocialObjectFuniture.size() < 5;

    (void)bySocialObjType;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CanCreateSocialItem stub");
    return true;
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
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto it = m_setSocialObjectKeys.find(dwObjectID);
    // if (it == m_setSocialObjectKeys.end())
    //     return false;
    //
    // XActor* pActor = FindActor(*it);
    // if (!pActor) return false;
    //
    // CSocialItemObject* pSocialItem = dynamic_cast<CSocialItemObject*>(pActor);
    // if (!pSocialItem) return false;
    // if (pSocialItem->GetActorID() != dwObjectID) return false;
    //
    // unsigned int ownerID = CSocialItemObject::GetOwnerID(pSocialItem);
    // XActor* pOwnerActor = FindActor(ownerID);
    // if (pOwnerActor) {
    //     CUser* pUser = dynamic_cast<CUser*>(pOwnerActor);
    //     if (pUser) CUser::SetSocialOwnerID(pUser, 0);
    // }
    //
    // CSocialItemObject::EndProcess(pSocialItem);
    // ExitActor(&pSocialItem->XActor);
    // ThreadLocalData::DeleteSocialItemObject(ThreadLocalData::GetInstance(), pSocialItem);
    //
    // m_setSocialObjectKeys.erase(dwObjectID);
    // m_mapSocialOwnerKeys.erase(ownerID);
    // m_setSocialObjectFuniture.erase(dwObjectID);
    // m_setSocialObjects.erase(dwObjectID);
    // return true;

    (void)dwObjectID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "DeleteSocialItemObject stub");
    return true;
}

// IDA 0x1402CF360 - FindSocialItemObject
// IDA 反编译精确还原:
// 1. 在 m_setSocialObjectKeys 中查找 dwObjectID
// 2. 获取对应的 Actor，转换为 CSocialItemObject
// 3. 验证 ActorID 是否匹配
CSocialItemObject* XDistrict::FindSocialItemObject(std::uint32_t dwObjectID)
{
    // TODO: 汇编还原 - 需要完整类型定义
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
    // TODO: 汇编还原 - 需要完整类型定义
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
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto it = m_mapWorldMode.find(stInfo.nModeID);
    // if (it != m_mapWorldMode.end()) {
    //     m_mapWorldMode.erase(it);
    // }

    (void)stInfo;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ClearWorldMode stub");
}

// IDA 0x140340230 - InfoWorldMode
// IDA 反编译精确还原:
// 1. 清空 m_vecWorldModeList
// 2. 遍历输入的 stInfos
// 3. 获取每个模式的表数据
// 4. 如果 Start_Type == 1，修改 ModeID 为 After_Mode_ID 并添加到列表
void XDistrict::InfoWorldMode(std::vector<ST_WORLD_MODE_INFO>& stInfos)
{
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // m_vecWorldModeList.vecInfo.clear();
    // for (size_t i = 0; i < stInfos.size(); ++i) {
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     TB_MODE_DISTRICT6* pMode = XResourceMgr::GetTB_MODE_DISTRICT6(&pServer->m_xResourceMgr, stInfos[i].nModeID);
    //     if (pMode->Start_Type == 1) {
    //         ST_WORLD_MODE_INFO stInfo = stInfos[i];
    //         stInfo.nModeID = pMode->After_Mode_ID;
    //         m_vecWorldModeList.push_back(stInfo);
    //     }
    // }

    (void)stInfos;
    GreenDamTan_log(__FILE__, __FUNCTION__, "InfoWorldMode stub");
}

// IDA 0x1405FA3B0 - IsCanUseActiveAkashic
// IDA 反编译精确还原: 返回 m_bCanUseActiveAkashic
bool XDistrict::IsCanUseActiveAkashic()
{
    // IDA 精确还原代码:
    // return m_bCanUseActiveAkashic;

    return m_bCanUseActiveAkashic;
}
