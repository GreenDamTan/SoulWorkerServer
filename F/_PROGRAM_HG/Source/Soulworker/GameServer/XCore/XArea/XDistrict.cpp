#include "Soulworker/GameServer/XCore/XArea/XDistrict.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

// 前置声明
class XGameServer;
class CUser;
class XActor;

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
