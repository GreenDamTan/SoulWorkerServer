#include "Soulworker/GameServer/XCore/XArea/XMyRoom.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

XMyRoom::XMyRoom()
    : XArea()
    , m_pObjectResource(nullptr)
    , m_pTBMazeInfo(nullptr)
    , m_pTBMyRoomInfo(nullptr)
    , m_bCreate(false)
    , m_byRoomState(0)
    , m_byEnterUser(0)
    , m_stOwnerInfo(nullptr)
    , m_stMyRoomItemList(nullptr)
    , m_stMyRoomUsedUserList(nullptr)
    , m_bSendMyroomInfo(false)
{
    GreenDamTan_log(__FILE__, __FUNCTION__, "XMyRoom constructed");
}

XMyRoom::~XMyRoom() {
    Clear();
    GreenDamTan_log(__FILE__, __FUNCTION__, "XMyRoom destructed");
}

// IDA 0x1402AC560 - GetWorldType
// IDA 反编译精确还原: 返回 3 (表示 MyRoom 类型)
int XMyRoom::GetWorldType() {
    // IDA 精确还原代码:
    // return 3;
    return 3;
}

// IDA 0x1402AC580 - GetMyRoomState
// IDA 反编译精确还原: 返回 m_byRoomState
std::uint8_t XMyRoom::GetMyRoomState() {
    // IDA 精确还原代码:
    // return m_byRoomState;
    return m_byRoomState;
}

// IDA 0x1400FAB90 - GetOwnerUAID
// IDA 反编译精确还原: 返回 m_stOwnerInfo.dwOwnerUAID
std::uint32_t XMyRoom::GetOwnerUAID() {
    // IDA: return this->m_stOwnerInfo.dwOwnerUAID;
    if (m_stOwnerInfo) {
        return m_stOwnerInfo->dwOwnerUAID;
    }
    return 0;
}

// IDA 0x1400FABB0 - GetOwnerInfo
// IDA 反编译精确还原: 复制 m_stOwnerInfo 到输出参数
void XMyRoom::GetOwnerInfo(ST_MYROOM_OWNER_INFO& stOwnerInfo) {
    // IDA: qmemcpy(stOwnerInfo, &this->m_stOwnerInfo, sizeof(ST_MYROOM_OWNER_INFO));
    if (m_stOwnerInfo) {
        stOwnerInfo = *m_stOwnerInfo;
    }
}

// IDA 0x1400FAC00 - IsMyRoomOwner
// IDA 反编译精确还原: 比较 dwUAID 与所有者 UAID
bool XMyRoom::IsMyRoomOwner(std::uint32_t dwUAID) {
    // IDA: return dwUAID == this->m_stOwnerInfo.dwOwnerUAID;
    if (m_stOwnerInfo) {
        return dwUAID == m_stOwnerInfo->dwOwnerUAID;
    }
    return false;
}

// IDA 0x1402AC8A0 - Init
// IDA 反编译精确还原:
// 1. 调用 XIOCPServer::BackSends
// 2. 清空各种列表和映射
// 3. 获取对象资源和表数据
bool XMyRoom::Init() {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // XIOCPServer::BackSends(this);
    // m_listWaitForRecvInfo.clear();
    // m_mpEnterUser.clear();
    // m_mpMyRoomFurniture.clear();
    // m_stMyRoomItemList.clear();
    // m_bCreate = false;
    // m_byRoomState = 0;
    //
    // std::uint16_t tbMapID = GetTBMapID();
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // m_pObjectResource = XWorldResMgr::GetResource(&pServer->m_xWorldResMgr, tbMapID);
    // if (!m_pObjectResource) return false;
    //
    // m_pTBMazeInfo = XResourceMgr::GetTB_MAZE_INFO(&pServer->m_xResourceMgr, tbMapID);
    // if (!m_pTBMazeInfo) return false;
    //
    // std::uint32_t dwIndex = m_stOwnerInfo->shMapIndex;
    // m_pTBMyRoomInfo = XResourceMgr::GetTB_MYROOM_INFO(&pServer->m_xResourceMgr, dwIndex);
    // if (!m_pTBMyRoomInfo) return false;
    //
    // m_byEnterUser = 0;
    // m_mpPollenInfo.clear();
    // m_bSendMyroomInfo = false;
    // return true;

    GreenDamTan_log(__FILE__, __FUNCTION__, "Init stub");
    return true;
}

// IDA 0x1402ACA30 - Clear
// IDA 反编译精确还原: 清理资源
void XMyRoom::Clear() {
    // IDA 精确还原代码:
    // m_pObjectResource = nullptr;
    // m_pTBMazeInfo = nullptr;
    // m_listWaitForRecvInfo.clear();
    // m_objectScanner.mapEtcList.clear();
    // m_objectScanner.mapNPCList.clear();
    // m_objectScanner.clear();
    // m_mpEnterUser.clear();
    // memset(&m_stOwnerInfo, 0, sizeof(m_stOwnerInfo));
    // m_stMyRoomItemList.clear();
    // m_mpMyRoomFurniture.clear();
    // m_bCreate = false;
    // m_byRoomState = 0;
    // ... (遍历 m_mapQuestMoveBox 删除元素)
    // ... (遍历 m_mapActor 删除 NPC)
    // m_byEnterUser = 0;
    // XArea::Clear();
    // m_mpPollenInfo.clear();
    // m_bSendMyroomInfo = false;

    m_pObjectResource = nullptr;
    m_pTBMazeInfo = nullptr;
    m_bCreate = false;
    m_byRoomState = 0;
    m_byEnterUser = 0;
    m_mpEnterUser.clear();
    m_mpMyRoomFurniture.clear();
    m_mpPollenInfo.clear();
    m_bSendMyroomInfo = false;
    GreenDamTan_log(__FILE__, __FUNCTION__, "Clear stub");
}

// IDA 0x1402ACCE0 - ClearState
// IDA 反编译精确还原: 清除房间状态
void XMyRoom::ClearState() {
    // IDA 精确还原代码:
    // m_byRoomState = 0;
    m_byRoomState = 0;
}

// IDA 0x1402ACD00 - CreateRoom
// IDA 反编译精确还原: 创建房间
bool XMyRoom::CreateRoom(void* stEnterUser, void* stOwnerInfo, TUXMapID uxMapID, std::uint32_t dwOwnerUCID) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // m_uxMapID = uxMapID;
    // qmemcpy(&m_stOwnerInfo, stOwnerInfo, sizeof(m_stOwnerInfo));
    // if (!Init()) return false;
    // SpawnGenerate();
    // XSendDBPacket xSendDBPacket(0, 0x25, 3);
    // xSendDBPacket << m_stOwnerInfo.dwOwnerUAID;
    // xSendDBPacket << m_uxMapID.nMapID;
    // xSendDBPacket << stEnterUser;
    // xSendDBPacket << dwOwnerUCID;
    // XGameServer::Instance()->SendDBGame(&xSendDBPacket);
    // return true;

    m_uxMapID = uxMapID;
    (void)stEnterUser;
    (void)stOwnerInfo;
    (void)dwOwnerUCID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CreateRoom stub");
    return true;
}

// IDA 0x1402ACE90 - SpawnGenerate
// IDA 反编译精确还原: 生成对象
void XMyRoom::SpawnGenerate() {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // VMap<int,void*>* ObjectMap = m_pObjectResource->GetColor();
    // void* Iter = ObjectMap->GetStartPosition();
    // while (Iter) {
    //     int iKey; void* pValue;
    //     ObjectMap->GetNextPair(&Iter, &iKey, &pValue);
    //     const VEventObjectInfo* pInfo = (const VEventObjectInfo*)pValue;
    //     if (pValue && pInfo->eType == eEventObjectType_Box) {
    //         const VEventBoxInfo* pBoxInfo = (const VEventBoxInfo*)pInfo;
    //         int v15 = (int)pInfo[1].__vftable;
    //         if (v15 == 1) {
    //             const VMonsterSpawnInfo* pMonsterSpawn = (const VMonsterSpawnInfo*)pBoxInfo;
    //             if (pBoxInfo[1].Plane[4].m_vNormal.y == 1)
    //                 ExcuteSpawnBox(pMonsterSpawn, eSendInfoTypeNot);
    //         } else if (v15 == 13) {
    //             // 任务移动盒处理...
    //         }
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "SpawnGenerate stub");
}

// IDA 0x1402AD380 - GetSpawnPos
// IDA 反编译精确还原: 获取生成位置
void XMyRoom::GetSpawnPos(const void* pEventObjectInfo, XVec3& vPos) {
    // TODO: 汇编还原 - 需要 VEventObjectInfo 完整定义
    // IDA 精确还原代码:
    // const VEventObjectInfo* pInfo = (const VEventObjectInfo*)pEventObjectInfo;
    // vPos.x = (pInfo->PosTopLeft.x + pInfo->PosBottomRight.x) / 2.0f;
    // vPos.y = (pInfo->PosTopLeft.y + pInfo->PosBottomRight.y) / 2.0f;
    // vPos.z = pInfo->PosTopLeft.z;

    (void)pEventObjectInfo;
    vPos.x = 0.0f;
    vPos.y = 0.0f;
    vPos.z = 0.0f;
    GreenDamTan_log(__FILE__, __FUNCTION__, "GetSpawnPos stub");
}

// IDA 0x1402AD3F0 - EnterRoom
// IDA 反编译精确还原: 进入房间
int XMyRoom::EnterRoom(void* stEnterUser) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (!m_bCreate) return 58203;
    // ST_MYROOM_USER* pUser = (ST_MYROOM_USER*)stEnterUser;
    // if (pUser->dwUAID == m_stOwnerInfo.dwOwnerUAID) {
    //     m_mpEnterUser.insert(std::make_pair(pUser->dwUCID, *pUser));
    //     return 0;
    // }
    // if (GetMyRoomState()) return 58205;
    // if (m_byEnterUser < m_pTBMyRoomInfo->My_Room_Max_Access_Member) {
    //     if (m_stOwnerInfo.byRoomOpenLevel) return 58207;
    //     ++m_byEnterUser;
    //     m_mpEnterUser.insert(std::make_pair(pUser->dwUCID, *pUser));
    //     return 0;
    // }
    // return 58206;

    (void)stEnterUser;
    GreenDamTan_log(__FILE__, __FUNCTION__, "EnterRoom stub");
    return 0;
}

// IDA 0x1402AD550 - ExitRoom
// IDA 反编译精确还原: 退出房间
void XMyRoom::ExitRoom(std::uint32_t dwUCID, std::uint32_t& dwBeforeMap) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpEnterUser.find(dwUCID);
    // if (iter != m_mpEnterUser.end()) {
    //     ST_MYROOM_USER stEnterUser = iter->second;
    //     dwBeforeMap = stEnterUser.dwBeforeMapID;
    //     m_mpEnterUser.erase(iter);
    //     if (stEnterUser.dwUAID != m_stOwnerInfo.dwOwnerUAID)
    //         --m_byEnterUser;
    // }
    // DelMyRoomUsedUser(dwUCID);
    // if (m_mpEnterUser.empty()) {
    //     // 发送包到控制socket...
    // }

    (void)dwUCID;
    (void)dwBeforeMap;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ExitRoom stub");
}

// IDA 0x1402AD750 - LoadMyRoomSucc
// IDA 反编译精确还原: 加载房间成功
void XMyRoom::LoadMyRoomSucc(void* stMyRoomItemList) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (m_bCreate) {
    //     // 已创建，直接返回
    //     return;
    // }
    // m_stMyRoomItemList = *stMyRoomItemList;
    // for (auto& item : m_stMyRoomItemList) {
    //     m_mpMyRoomFurniture.insert(std::make_pair(item.biSerial, item));
    // }
    // m_bCreate = true;

    (void)stMyRoomItemList;
    m_bCreate = true;
    GreenDamTan_log(__FILE__, __FUNCTION__, "LoadMyRoomSucc stub");
}

// IDA 0x1402AD8F0 - EnterActor
// IDA: ?EnterActor@XMyRoom@@UEAAGPEAVXActor@@@Z - returns unsigned short
std::uint16_t XMyRoom::EnterActor(XActor* pActor) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // CUser* pClient = dynamic_cast<CUser*>(pActor);
    // if (!pClient) return 50001;
    // if (GetMyRoomState()) {
    //     ExitArea(pActor);
    //     return 0;
    // }
    // std::uint16_t xError = XArea::EnterActor(pActor);
    // if (xError) return xError;
    // // 获取扫描器，设置区域...
    // // 发送房间加载信息...
    // return 0;

    (void)pActor;
    GreenDamTan_log(__FILE__, __FUNCTION__, "EnterActor stub");
    return 0;
}

// IDA 0x1402AE0D0 - ExitActor
// IDA 反编译精确还原: 退出Actor
void XMyRoom::ExitActor(XActor* pActor) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (pUser) CMoverEx::RemoveAllOptionEffect(&pUser->CMoverEx);
    // auto pVecActor = GetScanner(pActor);
    // std::uint32_t dwActorID = pActor->GetActorID().GetQuestID();
    // auto iter = pVecActor->find(dwActorID);
    // if (iter == pVecActor->end()) return 50001;
    // pVecActor->erase(iter);
    // std::uint16_t xError = XArea::ExitActor(pActor);
    // if (xError) return xError;
    // pActor->SetArea(nullptr);
    // CUser::Exit(pUser);
    // // 发送退出信息...
    // std::uint32_t dwBeforeMap = 0;
    // ExitRoom(dwActorID, dwBeforeMap);
    // return 0;

    (void)pActor;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ExitActor stub");
}

// IDA 0x1402AE410 - ExitArea
// IDA 反编译精确还原: 退出区域
void XMyRoom::ExitArea(XActor* pActor) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (pUser) {
    //     pUser->SetState(eStateChangeWorld);
    //     STPosInfo stPosinfo;
    //     std::uint32_t dwActorID = pUser->GetActorID().GetQuestID();
    //     std::uint32_t dwBeforeMap = GetBeforeMap(dwActorID);
    //     ST_LOG_GAME stLog;
    //     stLog._nUAID = pUser->GetUAID();
    //     stLog._nUCID = pUser->GetActorID().GetQuestID();
    //     stLog._sMainType = 19;
    //     stLog._sSubType = 4;
    //     stLog.nParam0 = m_stOwnerInfo.dwMapID;
    //     stLog.nParam1 = m_stOwnerInfo.dwOwnerUAID;
    //     stLog.nParam2 = 2;
    //     stLog.nParam5 = pUser->GetLevel();
    //     XGameServer::Instance()->SendDBLog(&stLog);
    //     // 获取起始传送门ID...
    //     // 发送进入地图请求...
    // }

    (void)pActor;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ExitArea stub");
}

// IDA 0x1402AE6E0 - OnUpdate
// IDA 反编译精确还原: 更新
void XMyRoom::OnUpdate(float fElapsed) {
    // IDA 精确还原代码:
    // for (auto it = m_mapActor.Begin(); it; m_mapActor.GetNext(&it)) {
    //     XActor* pActor = m_mapActor.GetValueAt(it);
    //     if (pActor) pActor->OnUpdate();
    // }

    (void)fElapsed;
    GreenDamTan_log(__FILE__, __FUNCTION__, "OnUpdate stub");
}

// IDA 0x1402AE780 - SendObjectInfo
// IDA 反编译精确还原: 发送对象信息
bool XMyRoom::SendObjectInfo(XActor* pActor) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // XSendPacket xSendPacket(4, 0x11);
    // SendInInfo(&xSendPacket, pActor);
    // XSendPacket xSendPacketUser(4, 0x21);
    // SendOtherInfos(&xSendPacketUser, pActor, eActorUser);
    // XSendPacket xSendPacketNPC(4, 0x22);
    // SendOtherInfos(&xSendPacketNPC, pActor, eActorNPC);
    // return true;

    (void)pActor;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendObjectInfo stub");
    return true;
}

// IDA 0x1402AE8C0 - SendBroadCast
// IDA 反编译精确还原: 发送广播
void XMyRoom::SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
    //     XActor* pTargetActor = it->second;
    //     if (eBroadCastType != eNoneSelf || pTargetActor != pExceptActor) {
    //         pTargetActor->SendPacket(&packet);
    //     }
    // }

    (void)packet;
    (void)pExceptActor;
    (void)eBroadCastType;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendBroadCast stub");
}

// IDA 0x1402AEA20 - LoadComplete
// IDA 反编译精确还原: 加载完成
void XMyRoom::LoadComplete(XActor* pActor) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (pUser) {
    //     pUser->SetClientLoadComplete(true);
    //     SendObjectInfo(pActor);
    //     SendTransportationInfo(pActor);
    //     auto pAttr = pUser->GetGOC<CGocAttribute>();
    //     if (pAttr) pAttr->SetStartRegStat(true);
    //     auto pInven = pUser->GetGOC<CGocInventory>();
    //     if (pInven) {
    //         pInven->InitItemCoolTime();
    //         pInven->SendItemCoolTimeInfo();
    //     }
    // }

    (void)pActor;
    GreenDamTan_log(__FILE__, __FUNCTION__, "LoadComplete stub");
}

// IDA 0x1402AEB50 - SendMyRoomLoad
// IDA 反编译精确还原: 发送房间加载
void XMyRoom::SendMyRoomLoad(class CUser* pUser) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (!pUser) return;
    // ST_MYROOM_OWNER_INFO stOwnerInfo = m_stOwnerInfo;
    // auto pMyroomPtr = pUser->GetGOC<CGocMyroom>();
    // if (!pMyroomPtr) return;
    // if (pMyroomPtr->GetLoadMyroom()) {
    //     pMyroomPtr->GetMyroomBoardInfo(&stOwnerInfo);
    //     XSendPacket packet(0x26, 0x11);
    //     packet << stOwnerInfo;
    //     pUser->Send(&packet);
    //     // ... 发送更多包
    //     SendPollenLoad(pUser);
    //     m_bSendMyroomInfo = false;
    // } else {
    //     m_bSendMyroomInfo = true;
    // }

    (void)pUser;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendMyRoomLoad stub");
}

// IDA 0x1402AEF60 - EditFurniture
// IDA 反编译精确还原: 编辑家具
bool XMyRoom::EditFurniture(class CUser* pUser, void* stEditFurniture, std::uint8_t byState) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto pInvenPtr = pUser->GetGOC<CGocInventory>();
    // auto pInven = pInvenPtr->GetInvenPtr(0xB);
    // for (auto& stInfo : *stEditFurniture) {
    //     if (byState == 1) {
    //         auto iter = m_mpMyRoomFurniture.find(stInfo.biSerial);
    //         if (iter == m_mpMyRoomFurniture.end()) return false;
    //         iter->second.byRotation = stInfo.byRotation;
    //         iter->second.dwGridIndex = stInfo.dwGridIndex;
    //         // 更新 m_stMyRoomItemList...
    //     }
    // }
    // return true;

    (void)pUser;
    (void)stEditFurniture;
    (void)byState;
    GreenDamTan_log(__FILE__, __FUNCTION__, "EditFurniture stub");
    return true;
}

// IDA 0x1402AF210 - AddMyRoomItem
// IDA 反编译精确还原: 添加房间物品
bool XMyRoom::AddMyRoomItem(void* stItem) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // ST_MYROOM_ITEM* pItem = (ST_MYROOM_ITEM*)stItem;
    // auto iter = m_mpMyRoomFurniture.find(pItem->biSerial);
    // if (iter != m_mpMyRoomFurniture.end()) return false;
    // m_mpMyRoomFurniture.insert(std::make_pair(pItem->biSerial, *pItem));
    // m_stMyRoomItemList.push_back(*pItem);
    // return true;

    (void)stItem;
    GreenDamTan_log(__FILE__, __FUNCTION__, "AddMyRoomItem stub");
    return true;
}

// IDA 0x1402AF2F0 - DelMyRoomItem
// IDA 反编译精确还原: 删除房间物品
bool XMyRoom::DelMyRoomItem(std::int64_t biSerial, void* stItem) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpMyRoomFurniture.find(biSerial);
    // if (iter == m_mpMyRoomFurniture.end()) return false;
    // *stItem = iter->second;
    // m_mpMyRoomFurniture.erase(biSerial);
    // // 从 m_stMyRoomItemList 中删除...
    // return true;

    (void)biSerial;
    (void)stItem;
    GreenDamTan_log(__FILE__, __FUNCTION__, "DelMyRoomItem stub");
    return true;
}

// IDA 0x1402AF490 - AddMyRoomUsedUser
// IDA 反编译精确还原: 添加房间使用用户
std::uint8_t XMyRoom::AddMyRoomUsedUser(std::uint32_t dwActorID, std::int64_t i64Serial, std::uint8_t byAniIndex) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // std::uint8_t byIndex = GetEmptyIndex(i64Serial);
    // if (byIndex == 255) return -1;
    // ST_MYROOM_USED_USER stUsedUser;
    // stUsedUser.dwActorID = dwActorID;
    // stUsedUser.biSerial = i64Serial;
    // stUsedUser.byIndex = byIndex;
    // stUsedUser.byAniIndex = byAniIndex;
    // m_stMyRoomUsedUserList.push_back(stUsedUser);
    // return byIndex;

    (void)dwActorID;
    (void)i64Serial;
    (void)byAniIndex;
    GreenDamTan_log(__FILE__, __FUNCTION__, "AddMyRoomUsedUser stub");
    return 0;
}

// IDA 0x1402AF530 - DelMyRoomUsedUser
// IDA 反编译精确还原: 删除房间使用用户
bool XMyRoom::DelMyRoomUsedUser(std::uint32_t dwActorID) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // for (auto veciter = m_stMyRoomUsedUserList.begin(); veciter != m_stMyRoomUsedUserList.end(); ) {
    //     if (dwActorID == veciter->dwActorID) {
    //         veciter = m_stMyRoomUsedUserList.erase(veciter);
    //     } else {
    //         ++veciter;
    //     }
    // }
    // return true;

    (void)dwActorID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "DelMyRoomUsedUser stub");
    return true;
}

// IDA 0x1402AF640 - GetEmptyIndex
// IDA 反编译精确还原: 获取空索引
std::uint8_t XMyRoom::GetEmptyIndex(std::int64_t i64Serial) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpMyRoomFurniture.find(i64Serial);
    // if (iter == m_mpMyRoomFurniture.end()) return -1;
    // ST_MYROOM_ITEM stMyroomItemInfo = iter->second;
    // auto pItemTable = XResourceMgr::GetTB_ITEM(stMyroomItemInfo.dwItemID);
    // if (!pItemTable) return -1;
    // auto pMyroomItemRef = XResourceMgr::GetTB_MYROOM_FURNITURE(pItemTable->Furniture_ID);
    // if (!pMyroomItemRef) return -1;
    // int iMaxUsedUser = pMyroomItemRef->Furniture_Item_Special_Use;
    // if (iMaxUsedUser > 4) iMaxUsedUser = 4;
    // bool bUsedSlot[4] = {false};
    // int iCount = 0;
    // for (auto& stItem : m_stMyRoomUsedUserList) {
    //     if (i64Serial == stItem.biSerial) {
    //         bUsedSlot[stItem.byIndex] = true;
    //         ++iCount;
    //     }
    // }
    // int iEmptySlotMaxCount = iMaxUsedUser - iCount;
    // if (iEmptySlotMaxCount <= 0) return -1;
    // int iRandomIndex = rand() % iEmptySlotMaxCount;
    // int iEmptySlotCount = 0;
    // for (int i = 0; i < 4; ++i) {
    //     if (!bUsedSlot[i]) {
    //         if (iRandomIndex == iEmptySlotCount) return i;
    //         ++iEmptySlotCount;
    //     }
    // }
    // return -1;

    (void)i64Serial;
    GreenDamTan_log(__FILE__, __FUNCTION__, "GetEmptyIndex stub");
    return 0;
}

// IDA 0x1402AF900 - IsCanChangeDoorState
// IDA 反编译精确还原: 检查是否可以更改门状态
bool XMyRoom::IsCanChangeDoorState(char cDoorIndex) {
    // IDA 精确还原代码:
    // if (cDoorIndex >= 10) return false;
    // float fChangeTime = m_fDoorStateChangedTime[cDoorIndex] + 1.0f;
    // auto Timer = ThreadLocalData::GetTimer();
    // return IVTimer::GetTime(Timer) > fChangeTime;

    (void)cDoorIndex;
    GreenDamTan_log(__FILE__, __FUNCTION__, "IsCanChangeDoorState stub");
    return true;
}

// IDA 0x1402AF970 - SetDoorState
// IDA 反编译精确还原: 设置门状态
void XMyRoom::SetDoorState(char cDoorIndex, bool bOpen) {
    // IDA 精确还原代码:
    // m_bDoorOpen[cDoorIndex] = bOpen;
    // auto Timer = ThreadLocalData::GetTimer();
    // m_fDoorStateChangedTime[cDoorIndex] = IVTimer::GetTime(Timer);

    (void)cDoorIndex;
    (void)bOpen;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetDoorState stub");
}

// IDA 0x1402AF9C0 - AllUserOut
// IDA 反编译精确还原: 所有用户退出
void XMyRoom::AllUserOut(int nReason) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // for (auto it = m_objectScanner.begin(); it != m_objectScanner.end(); ++it) {
    //     CMover* pUser = it->second;
    //     CUser* pCUser = dynamic_cast<CUser*>(pUser);
    //     if (pCUser->GetUAID() != m_stOwnerInfo.dwOwnerUAID) {
    //         XSendPacket xSendPacket(0x26, 0x25);
    //         xSendPacket << nReason;
    //         pUser->Send(&xSendPacket);
    //         ExitArea(pUser);
    //     }
    // }

    (void)nReason;
    GreenDamTan_log(__FILE__, __FUNCTION__, "AllUserOut stub");
}

// IDA 0x1402AFBA0 - UserKickOut
// IDA 反编译精确还原: 用户踢出
void XMyRoom::UserKickOut(std::uint32_t dwKickActorID) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_objectScanner.find(dwKickActorID);
    // if (iter != m_objectScanner.end()) {
    //     CMover* pUser = iter->second;
    //     XSendPacket xSendPacket(0x26, 0x25);
    //     xSendPacket << 58211;
    //     pUser->Send(&xSendPacket);
    //     ExitArea(pUser);
    // }

    (void)dwKickActorID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "UserKickOut stub");
}

// IDA 0x1402AFD10 - GetCurUserCount
// IDA 反编译精确还原: 获取当前用户数
int XMyRoom::GetCurUserCount() {
    // IDA 精确还原代码:
    // return m_objectScanner.size();

    GreenDamTan_log(__FILE__, __FUNCTION__, "GetCurUserCount stub");
    return static_cast<int>(m_mpEnterUser.size());
}

// IDA 0x1402AFD40 - SetMyRoomSetup
// IDA 反编译精确还原: 设置房间设置
void XMyRoom::SetMyRoomSetup(class CUser* pOwnerUser, void* stMyRoomSetup) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // PS_MYROOM_SETUP* pSetup = (PS_MYROOM_SETUP*)stMyRoomSetup;
    // m_stOwnerInfo.byRoomOpenLevel = pSetup->byOpenLevel;
    // wcscpy(m_stOwnerInfo.szRoomName, pSetup->szMyRoomName);
    // if (m_stOwnerInfo.byRoomOpenLevel == 1) {
    //     AllUserOut(58207);
    // }

    (void)pOwnerUser;
    (void)stMyRoomSetup;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetMyRoomSetup stub");
}

// IDA 0x1402AFE00 - RunQuestMoveCheck
// IDA 反编译精确还原: 运行任务移动检查
void XMyRoom::RunQuestMoveCheck(int nBoxIndex, class CUser* pUser) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto it = m_mapQuestMoveBox.find(nBoxIndex);
    // if (it != m_mapQuestMoveBox.end()) {
    //     STQuestMoveBox* pQuestMoveBox = it->second;
    //     auto pTBQuestCondition = XResourceMgr::GetTB_QUEST_CONDITION(pQuestMoveBox->pQuestMoveBox->m_iConditionID);
    //     if (pTBQuestCondition) {
    //         auto pQuest = pUser->GetGOC<CGocQuest>();
    //         pQuest->UpdateCondition(pQuestMoveBox->pQuestMoveBox->m_iConditionID, 1, 1);
    //         XSendPacket xSendPacket(0x11, 0x66);
    //         xSendPacket << nBoxIndex;
    //         pUser->Send(&xSendPacket);
    //     }
    // }

    (void)nBoxIndex;
    (void)pUser;
    GreenDamTan_log(__FILE__, __FUNCTION__, "RunQuestMoveCheck stub");
}

// IDA 0x1402AFFE0 - EscapeActor
// IDA 反编译精确还原: 逃离Actor
bool XMyRoom::EscapeActor(XActor* pActor) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // CUser* pUser = dynamic_cast<CUser*>(pActor);
    // if (!pUser) return false;
    // int nMapID = GetTBMapID();
    // STPosInfo stMovePos;
    // if (!XWorldResMgr::GetStartPortalID(nMapID, &nJumpID, &stMovePos)) return false;
    // MoveActor(pUser, &stMovePos.vPos);
    // pUser->MoveingValueClear();
    // pUser->ChangeMotion(1, 1, 0);
    // pUser->SendResWarp(0, &stMovePos.vPos, stMovePos.fRot);
    // return true;

    (void)pActor;
    GreenDamTan_log(__FILE__, __FUNCTION__, "EscapeActor stub");
    return true;
}

// IDA 0x1402B0140 - GetExitDistrictID
// IDA 反编译精确还原: 获取出口区域ID
bool XMyRoom::GetExitDistrictID(std::uint32_t dwActorID, std::uint16_t& wMapID, int& nJumpID, XVec3& vPos) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpEnterUser.find(dwActorID);
    // if (iter == m_mpEnterUser.end()) return false;
    // ST_MYROOM_USER stEnterUser = iter->second;
    // wMapID = stEnterUser.dwBeforeMapID;
    // STPosInfo stPosinfo;
    // auto pServer = XGameServer::Instance();
    // return XWorldResMgr::GetStartPortalID(&pServer->m_xWorldResMgr, wMapID, &nJumpID, &stPosinfo);

    (void)dwActorID;
    (void)wMapID;
    (void)nJumpID;
    (void)vPos;
    GreenDamTan_log(__FILE__, __FUNCTION__, "GetExitDistrictID stub");
    return false;
}

// IDA 0x1402B0300 - PollenLoad
// IDA 反编译精确还原: 花粉加载
void XMyRoom::PollenLoad(void* psPollenList) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // m_mpPollenInfo.clear();
    // for (auto& psInfo : *psPollenList) {
    //     ST_POLLEN_INFO stInfo;
    //     stInfo.stPollenInfo = psInfo;
    //     stInfo.bySubCmd = 0;
    //     SetPollenInfo(&stInfo.stPollenInfo);
    // }

    (void)psPollenList;
    GreenDamTan_log(__FILE__, __FUNCTION__, "PollenLoad stub");
}

// IDA 0x1402B0520 - SetPollenInfo
// IDA 反编译精确还原: 设置花粉信息
void XMyRoom::SetPollenInfo(void* psPollenInfo) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // ST_POLLEN_INFO stInfo;
    // stInfo.stPollenInfo = *psPollenInfo;
    // stInfo.bySubCmd = 0;
    // m_mpPollenInfo.insert(std::make_pair(stInfo.stPollenInfo.nPollenIndex, stInfo));

    (void)psPollenInfo;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetPollenInfo stub");
}

// IDA 0x1402B0630 - CanPollenAdd
// IDA 反编译精确还原: 检查是否可以添加花粉
bool XMyRoom::CanPollenAdd(int nPollenIndex) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (!nPollenIndex) return false;
    // auto iter = m_mpPollenInfo.find(nPollenIndex);
    // if (iter != m_mpPollenInfo.end()) return false;
    // if (!m_pTBMyRoomInfo) return false;
    // for (int i = 0; i < 10; ++i) {
    //     if (m_pTBMyRoomInfo->Nomal_Pollen_Index_01[i] == nPollenIndex) return true;
    // }
    // return false;

    (void)nPollenIndex;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CanPollenAdd stub");
    return true;
}

// IDA 0x1402B06F0 - CanCultivation
// IDA 反编译精确还原: 检查是否可以培育
std::uint32_t XMyRoom::CanCultivation(int nPollenIndex) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpPollenInfo.find(nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) return 58213;
    // if (iter->second.byState) return 58286;
    // if (iter->second.byCultivating) return 58287;
    // return 0;

    (void)nPollenIndex;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CanCultivation stub");
    return 0;
}

// IDA 0x1402B0D80 - CanPollenCancel
// IDA 反编译精确还原: 检查是否可以取消花粉
bool XMyRoom::CanPollenCancel(int nPollenIndex) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpPollenInfo.find(nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) return false;
    // if (iter->second.byCultivating) return false;
    // return iter->second.byState != 0;

    (void)nPollenIndex;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CanPollenCancel stub");
    return true;
}

// IDA 0x1402B0E10 - GetPollenHarvest
// IDA 反编译精确还原: 获取花粉收获
bool XMyRoom::GetPollenHarvest(int nPollenIndex, std::uint32_t& dwItemID, std::int16_t& shCount, std::uint8_t& byResult) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpPollenInfo.find(nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) return false;
    // PS_MYROOM_POLLEN_INFO psInfo = iter->second.stPollenInfo;
    // auto pTBCultivation = XResourceMgr::GetTB_CULTIVATION(psInfo.dwCultivationItem);
    // if (!pTBCultivation) return false;
    // auto biCurdate = XGameServer::GetCurDate();
    // if (psInfo.biRotDate >= biCurdate || psInfo.dwWiltBlockItem) {
    //     // 腐烂状态
    //     *byResult = 3;
    //     *dwItemID = pTBCultivation->Decay_Item_ID;
    //     *shCount = pTBCultivation->Decay_Item_ID_Num;
    //     return true;
    // }
    // if (psInfo.biWiltDate >= biCurdate || psInfo.dwWiltBlockItem) {
    //     // 枯萎状态
    //     *byResult = 2;
    //     *dwItemID = pTBCultivation->Wilt_Item_ID;
    //     *shCount = pTBCultivation->Wilt_Item_ID_Num;
    //     return true;
    // }
    // if (psInfo.biHarvestDate >= biCurdate) {
    //     return false; // 还没到收获时间
    // }
    // // 收获状态
    // *byResult = 0;
    // *dwItemID = pTBCultivation->Crops_Item_ID;
    // *shCount = pTBCultivation->Crops_Item_Num;
    // // 检查是否有奖励
    // if (pTBCultivation->Cultivation_Bonus_Rate && pTBCultivation->Crops_Bonus_Item_Num) {
    //     int nRate = XItemFactory::nRand(1, 10000);
    //     if (nRate <= pTBCultivation->Cultivation_Bonus_Rate) {
    //         *byResult = 1;
    //         *shCount += pTBCultivation->Crops_Bonus_Item_Num;
    //     }
    // }
    // return true;

    (void)nPollenIndex;
    (void)dwItemID;
    (void)shCount;
    (void)byResult;
    GreenDamTan_log(__FILE__, __FUNCTION__, "GetPollenHarvest stub");
    return false;
}

// IDA 0x1402B1100 - ClearPollen
// IDA 反编译精确还原: 清除花粉
void XMyRoom::ClearPollen(int nPollenIndex, std::uint8_t bySubcmd) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpPollenInfo.find(nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) return;
    // if (!bySubcmd || iter->second.bySubCmd == bySubcmd) {
    //     iter->second.vecHelpUser.clear();
    //     iter->second.stPollenInfo = {};
    //     iter->second.bySubCmd = 0;
    //     iter->second.byCultivating = 0;
    //     iter->second.byState = 0;
    // }

    (void)nPollenIndex;
    (void)bySubcmd;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ClearPollen stub");
}

// IDA 0x1402B1460 - GetBeforeMap
// IDA 反编译精确还原: 获取之前地图
std::uint32_t XMyRoom::GetBeforeMap(std::uint32_t dwActorID) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpEnterUser.find(dwActorID);
    // if (iter != m_mpEnterUser.end()) {
    //     ST_MYROOM_USER stEnterUser = iter->second;
    //     if (stEnterUser.dwBeforeMapID) {
    //         return stEnterUser.dwBeforeMapID;
    //     }
    //     LogHelper::LogError("GetBeforeMap AAA UCID:%d", dwActorID);
    //     return 10003;
    // }
    // LogHelper::LogError("GetBeforeMap BBB UCID:%d", dwActorID);
    // return 10003;

    (void)dwActorID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "GetBeforeMap stub");
    return 10003;
}

// IDA 0x1402B1550 - SendPollenAdd
// IDA 反编译精确还原: 发送花粉添加
void XMyRoom::SendPollenAdd(int nPollenIndex) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // PS_MYROOM_POLLEN_INFO stInfo;
    // stInfo.nPollenIndex = nPollenIndex;
    // SetPollenInfo(&stInfo);
    // XSendPacket xSendPacket(0x26, 0x31);
    // xSendPacket << stInfo;
    // SendBroadCast(xSendPacket, nullptr, eAll);

    (void)nPollenIndex;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendPollenAdd stub");
}

// IDA 0x1402B1660 - SendPollenCultivation
// IDA 反编译精确还原: 发送花粉培育
void XMyRoom::SendPollenCultivation(void* psInfo) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // PS_DB_MYROOM_POLLEN_CULTIVATION* pDBInfo = (PS_DB_MYROOM_POLLEN_CULTIVATION*)psInfo;
    // auto iter = m_mpPollenInfo.find(pDBInfo->stInfo.nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) return;
    // if (iter->second.bySubCmd != 50) {
    //     LogHelper::LogError("SendPollenCultivation (nPollenIndex:%d)", pDBInfo->stInfo.nPollenIndex);
    //     return;
    // }
    // iter->second.stPollenInfo.biStartDate = pDBInfo->stInfo.biStartDate;
    // iter->second.stPollenInfo.biHarvestDate = pDBInfo->stInfo.biHarvestDate;
    // iter->second.stPollenInfo.biWiltDate = pDBInfo->stInfo.biWiltDate;
    // iter->second.stPollenInfo.biRotDate = pDBInfo->stInfo.biRotDate;
    // iter->second.stPollenInfo.dwCultivationItem = pDBInfo->stInfo.dwCultivationItem;
    // iter->second.byCultivating = 0;
    // iter->second.stPollenInfo.dwNutritionItem = pDBInfo->dwNutritionItem;
    // iter->second.stPollenInfo.dwWiltBlockItem = pDBInfo->dwWiltBlockItem;
    // XSendPacket xSendPacket(0x26, 0x32);
    // xSendPacket << pDBInfo->stInfo;
    // SendBroadCast(xSendPacket, nullptr, eAll);

    (void)psInfo;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendPollenCultivation stub");
}

// IDA 0x1402B1870 - SendPollenHarvest
// IDA 反编译精确还原: 发送花粉收获
void XMyRoom::SendPollenHarvest(int nPollenIndex) {
    // IDA 精确还原代码:
    // ClearPollen(nPollenIndex, 0x33);
    // XSendPacket xSendPacket(0x26, 0x33);
    // xSendPacket << nPollenIndex;
    // SendBroadCast(xSendPacket, nullptr, eAll);

    (void)nPollenIndex;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendPollenHarvest stub");
}

// IDA 0x1402B1930 - SendPollenHelp
// IDA 反编译精确还原: 发送花粉帮助
void XMyRoom::SendPollenHelp(int nPollenIndex, void* psHelpUser, std::int64_t biHarvestDate) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // PS_MYROOM_POLLEN_INFO psPollenInfo;
    // AddPollenHelpUser(nPollenIndex, psHelpUser, biHarvestDate, &psPollenInfo);
    // XSendPacket xSendPacket(0x26, 0x34);
    // xSendPacket << nPollenIndex;
    // xSendPacket << psPollenInfo;
    // SendBroadCast(xSendPacket, nullptr, eAll);

    (void)nPollenIndex;
    (void)psHelpUser;
    (void)biHarvestDate;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendPollenHelp stub");
}

// IDA 0x1402B1A60 - SendPollenItemUse
// IDA 反编译精确还原: 发送花粉物品使用
void XMyRoom::SendPollenItemUse(void* psPollenInfo) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // PollenItemUse(psPollenInfo);
    // XSendPacket xSendPacket(0x26, 0x35);
    // xSendPacket << *psPollenInfo;
    // SendBroadCast(xSendPacket, nullptr, eAll);

    (void)psPollenInfo;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendPollenItemUse stub");
}

// IDA 0x1402B1B50 - SendPollenLoad
// IDA 反编译精确还原: 发送花粉加载
void XMyRoom::SendPollenLoad(class CUser* pUser) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (!pUser) return;
    // PS_MYROOM_POLLEN_LIST psPollenList;
    // for (auto& [key, stInfo] : m_mpPollenInfo) {
    //     psPollenList.vecInfo.push_back(stInfo.stPollenInfo);
    // }
    // XSendPacket xSendPacket(0x26, 0x30);
    // xSendPacket << psPollenList;
    // pUser->Send(&xSendPacket);

    (void)pUser;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendPollenLoad stub");
}

// IDA 0x1402AD040 - ExcuteSpawnBox
// IDA 反编译精确还原:
// 1. 遍历怪物信息数组（最多10个）
// 2. 对于 Type==1 的 NPC：创建 NPC 并进入游戏
void XMyRoom::ExcuteSpawnBox(const VMonsterSpawnInfo* pMonsterSpawn, E_SEND_INFO_TYPE eType) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // XVec3 vPos;
    // hkvVec3::hkvVec3(&vPos);
    // for (int i = 0; i < 10; ++i) {
    //     if (pMonsterSpawn->m_stMonsterInfo[i].m_iID) {
    //         if (pMonsterSpawn->m_stMonsterInfo[i].m_iType == 1) {
    //             GetSpawnPos(pMonsterSpawn, &vPos);
    //             ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();
    //             CNpc* pNpc = ThreadLocalData::CreateNpc(pThreadData, this, m_uxMapID, 0,
    //                 pMonsterSpawn->m_stMonsterInfo[i].m_iID, &vPos, pMonsterSpawn->fRotate);
    //             if (pNpc) {
    //                 if (EnterGameObject(&pNpc->XActor, eSendInfoTypeNot)) {
    //                     ThreadLocalData::DeleteNpc(ThreadLocalData::GetInstance(), pNpc);
    //                 } else {
    //                     CMoverEx::SetWayPointID(pNpc, pMonsterSpawn->m_iWaypoint);
    //                     CNpc::SetSpawnBoxID(pNpc, pMonsterSpawn->iID);
    //                 }
    //             }
    //         }
    //     }
    // }

    (void)pMonsterSpawn;
    (void)eType;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ExcuteSpawnBox stub");
}

// IDA 0x1402ADBE0 - EnterGameObject
// IDA 反编译精确还原: 进入游戏对象
std::uint16_t XMyRoom::EnterGameObject(XActor* pActor, E_SEND_INFO_TYPE eType) {
    // TODO: 汇编还原 - 需要完整类型定义
    (void)pActor;
    (void)eType;
    GreenDamTan_log(__FILE__, __FUNCTION__, "EnterGameObject stub");
    return 0;
}

// IDA 0x1402ADEA0 - ExitGameObject
// IDA 反编译精确还原: 退出游戏对象
std::uint16_t XMyRoom::ExitGameObject(XActor* pActor, E_SEND_INFO_TYPE eType) {
    // TODO: 汇编还原 - 需要完整类型定义
    (void)pActor;
    (void)eType;
    GreenDamTan_log(__FILE__, __FUNCTION__, "ExitGameObject stub");
    return 0;
}

// IDA 0x1402B0AC0 - CanHelpUser
// IDA 反编译精确还原: 检查是否可以帮助用户
std::uint32_t XMyRoom::CanHelpUser(std::uint32_t dwHelpUCID, int nPollenIndex, std::uint8_t& byCount, std::int64_t& biHarvestDate) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpPollenInfo.find(nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) return 58213;
    // PS_MYROOM_POLLEN_INFO psInfo = iter->second.stPollenInfo;
    // if (psInfo.byCultivating) return 58287;
    // byCount = (std::uint8_t)psInfo.vecHelpUser.size();
    // if (byCount >= 5) return 58214;
    // for (auto& helpUser : psInfo.vecHelpUser) {
    //     if (dwHelpUCID == helpUser.dwUCID) return 58215;
    // }
    // auto pTBCultivation = XResourceMgr::GetTB_CULTIVATION(psInfo.dwCultivationItem);
    // if (!pTBCultivation) return 58290;
    // biHarvestDate = psInfo.biHarvestDate - (int)(60 * pTBCultivation->Cultivation_Time * g_fPollenHelp[byCount]);
    // return 0;

    (void)dwHelpUCID;
    (void)nPollenIndex;
    (void)byCount;
    (void)biHarvestDate;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CanHelpUser stub");
    return 0;
}

// IDA 0x1402B1250 - AddPollenHelpUser
// IDA 反编译精确还原: 添加花粉帮助用户
void XMyRoom::AddPollenHelpUser(int nPollenIndex, void* psHelpUser, std::int64_t biHarvestDate, void* psPollenInfo) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpPollenInfo.find(nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) return;
    // if (iter->second.bySubCmd == 52) {
    //     iter->second.stPollenInfo.biHarvestDate = biHarvestDate;
    //     iter->second.stPollenInfo.vecHelpUser.push_back(*psHelpUser);
    //     iter->second.bySubCmd = 0;
    //     *psPollenInfo = iter->second.stPollenInfo;
    // } else {
    //     LogHelper::LogError("game.contents", "AddPollenHelpUser (nPollenIndex:%d)", nPollenIndex);
    // }

    (void)nPollenIndex;
    (void)psHelpUser;
    (void)biHarvestDate;
    (void)psPollenInfo;
    GreenDamTan_log(__FILE__, __FUNCTION__, "AddPollenHelpUser stub");
}

// IDA 0x1402B1340 - PollenItemUse
// IDA 反编译精确还原: 花粉物品使用
void XMyRoom::PollenItemUse(void* psPollenInfo) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpPollenInfo.find(psPollenInfo->nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) return;
    // if (iter->second.bySubCmd == 53) {
    //     iter->second.stPollenInfo.biHarvestDate = psPollenInfo->biHarvestDate;
    //     iter->second.stPollenInfo.dwNutritionItem = psPollenInfo->dwNutritionItem;
    //     iter->second.stPollenInfo.dwWiltBlockItem = psPollenInfo->dwWiltBlockItem;
    //     iter->second.bySubCmd = 0;
    // } else {
    //     LogHelper::LogError("game.contents", "PollenItemUse (nPollenIndex:%d)", psPollenInfo->nPollenIndex);
    // }

    (void)psPollenInfo;
    GreenDamTan_log(__FILE__, __FUNCTION__, "PollenItemUse stub");
}

// IDA 0x1402B1D20 - PollenUse
// IDA 反编译精确还原: 花粉使用
void XMyRoom::PollenUse(int nPollenIndex, std::uint8_t bySubCmd) {
    // IDA 精确还原代码:
    // auto iter = m_mpPollenInfo.find(nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) return;
    // if (!iter->second.bySubCmd) {
    //     iter->second.bySubCmd = bySubCmd;
    // }

    (void)nPollenIndex;
    (void)bySubCmd;
    GreenDamTan_log(__FILE__, __FUNCTION__, "PollenUse stub");
}

// IDA 0x1402B1DB0 - CanUsePollen
// IDA 反编译精确还原: 检查是否可以使用花粉
bool XMyRoom::CanUsePollen(int nPollenIndex) {
    // IDA 精确还原代码:
    // auto iter = m_mpPollenInfo.find(nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) {
    //     LogHelper::LogError("game.contents", "[POLLEN] XMyRoom::IsUsePollen - iter == m_mpPollenInfo.end() [ UAID:%d ]", m_stOwnerInfo.dwOwnerUAID);
    //     return false;
    // }
    // if (iter->second.bySubCmd) {
    //     LogHelper::LogError("game.contents", "[POLLEN] XMyRoom::IsUsePollen - iter->second.bySubCmd [ UAID:%d / %d ]", m_stOwnerInfo.dwOwnerUAID, iter->second.bySubCmd);
    //     return false;
    // }
    // return true;

    (void)nPollenIndex;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CanUsePollen stub");
    return true;
}

// IDA 0x1402B1E80 - SendPollenCancel
// IDA 反编译精确还原: 发送花粉取消
void XMyRoom::SendPollenCancel(int nPollenIndex) {
    // IDA 精确还原代码:
    // ClearPollen(nPollenIndex, 0x37);
    // XSendPacket xSendPacket(0x26, 0x37);
    // xSendPacket << nPollenIndex;
    // SendBroadCast(xSendPacket, nullptr, eAll);

    (void)nPollenIndex;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendPollenCancel stub");
}

// IDA 0x1402B1F40 - SetPollenLockCount
// IDA 反编译精确还原: 设置花粉锁定计数
bool XMyRoom::SetPollenLockCount(class CUser* pUser, int nPollenIndex, std::uint8_t byCount) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // auto iter = m_mpPollenInfo.find(nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) return false;
    // if (iter->second.stPollenInfo.byLockCount >= byCount) return false;
    // if (iter->second.stPollenInfo.vecHelpUser.size() < byCount) return false;
    // auto pMyroom = pUser->GetGOC<CGocMyroom>();
    // if (!pMyroom) return false;
    // if (pMyroom->SetPollenLockCount(nPollenIndex, byCount)) {
    //     iter->second.stPollenInfo.byLockCount = byCount;
    //     return true;
    // }
    // return false;

    (void)pUser;
    (void)nPollenIndex;
    (void)byCount;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetPollenLockCount stub");
    return false;
}

// IDA 0x1402B20A0 - CheatPollen
// IDA 反编译精确还原: 作弊花粉
void XMyRoom::CheatPollen(class CUser* pUser, int nPollenIndex, int nDecSec) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (nDecSec < 0) return;
    // auto dwUAID = pUser->GetUAID();
    // if (!IsMyRoomOwner(dwUAID)) return;
    // auto iter = m_mpPollenInfo.find(nPollenIndex);
    // if (iter == m_mpPollenInfo.end()) return;
    // if (iter->second.bySubCmd) return;
    // if (!iter->second.stPollenInfo.biHarvestDate) return;
    // auto nCurDate = XGameServer::GetCurDate();
    // if (iter->second.stPollenInfo.biStartDate < nCurDate) return;
    // int nChangeDate = nDecSec + nCurDate;
    // if (nChangeDate < nCurDate) return;
    // // 根据时间调整花粉状态...
    // iter->second.bySubCmd = 53;
    // PS_MYROOM_POLLEN_INFO psInfo = iter->second.stPollenInfo;
    // XSendDBPacket xSendDBPacket(pUser, 0x25, 0x23);
    // xSendDBPacket << pUser->GetMapInsID();
    // xSendDBPacket << pUser->GetActorID();
    // xSendDBPacket << pUser->GetUAID();
    // xSendDBPacket << psInfo;
    // XGameServer::Instance()->SendDBGame(&xSendDBPacket);

    (void)pUser;
    (void)nPollenIndex;
    (void)nDecSec;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CheatPollen stub");
}

// IDA 0x1402B2630 - UpdateRecommendCount
// IDA 反编译精确还原: 更新推荐计数
void XMyRoom::UpdateRecommendCount(int nCount) {
    // IDA 精确还原代码:
    // m_stOwnerInfo.nRecommendCount = nCount;
    (void)nCount;
    GreenDamTan_log(__FILE__, __FUNCTION__, "UpdateRecommendCount stub");
}

// IDA 0x1402CBB40 - SendTransportationInfo
// IDA 反编译精确还原: 发送运输信息
bool XMyRoom::SendTransportationInfo(XActor* pActor) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA 精确还原代码:
    // if (!pActor) return false;
    // if (XActor::GetType(pActor) != 0) return false;
    // for (auto& [dwActorID, pOtherActor] : m_mapActor) {
    //     if (pOtherActor && pOtherActor->IsLive() && pOtherActor != pActor) {
    //         if (XActor::GetType(pOtherActor) == 0) {
    //             CUser* pUser = dynamic_cast<CUser*>(pOtherActor);
    //             if (CUser::IsPlayingPublicTransport(pUser)) {
    //                 ST_MOVE_TRANSPORT_TAKE stMoveTake;
    //                 stMoveTake.dwActorID = pUser->GetActorID();
    //                 stMoveTake.wTransportTableIdx = CUser::GetPublicTransportIndex(pUser);
    //                 stMoveTake.fStartTime = CUser::GetPublicTransportTime(pUser);
    //                 XSendPacket xPacket(5, 0x16);
    //                 xPacket << stMoveTake;
    //                 pActor->BridgeSend_AfterLoading(&xPacket);
    //             }
    //         }
    //     }
    // }
    // return true;

    (void)pActor;
    GreenDamTan_log(__FILE__, __FUNCTION__, "SendTransportationInfo stub");
    return true;
}

// IDA 0x14052A680 - SetMyRoomState
// IDA 反编译精确还原: 设置房间状态
void XMyRoom::SetMyRoomState(std::uint8_t byState) {
    // IDA 精确还原代码:
    // m_byRoomState = byState;
    m_byRoomState = byState;
}

// IDA 0x14052A6A0 - GetSendRoomInfo
// IDA 反编译精确还原: 获取发送房间信息
bool XMyRoom::GetSendRoomInfo() {
    // IDA 精确还原代码:
    // return m_bSendMyroomInfo;
    return m_bSendMyroomInfo;
}
