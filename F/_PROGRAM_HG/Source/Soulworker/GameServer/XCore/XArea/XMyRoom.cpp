#include "Soulworker/GameServer/XCore/XArea/XMyRoom.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_MyRoomStructs.h"
#include "Soulworker/GameServer/XGameServer/InteractionObject.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAttribute.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/Npc.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"

// ST_MOVE_TRANSPORT_TAKE - Transport take packet structure
// IDA: Used in XMyRoom::SendTransportationInfo
struct ST_MOVE_TRANSPORT_TAKE {
    std::uint32_t dwActorID = 0;
    std::uint16_t wTransportTableIdx = 0;
    float fStartTime = 0.0f;
};

// XSendPacket operator<< for ST_MOVE_TRANSPORT_TAKE
XSendPacket& operator<<(XSendPacket& packet, const ST_MOVE_TRANSPORT_TAKE& data) {
    packet << data.dwActorID;
    packet << data.wTransportTableIdx;
    packet << data.fStartTime;
    return packet;
}

XMyRoom::XMyRoom()
    : XArea()
    , m_objectScanner()
    , m_pObjectResource(nullptr)
    , m_pTBMazeInfo(nullptr)
    , m_pTBMyRoomInfo(nullptr)
    , m_bCreate(false)
    , m_byRoomState(0)
    , m_byEnterUser(0)
    , m_stOwnerInfo(nullptr)
    , m_stMyRoomItemList()
    , m_stMyRoomUsedUserList()
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
    // IDA: XIOCPServer::BackSends(this);
    // Note: BackSends is called on base class XIOCPServer
    // m_listWaitForRecvInfo.clear();  // TODO: Add member to header
    m_mpEnterUser.clear();
    m_mpMyRoomFurniture.clear();
    // Note: m_stMyRoomItemList is a pointer, clear handled separately
    m_bCreate = false;
    m_byRoomState = 0;

    std::uint16_t tbMapID = GetTBMapID();
    XGameServer* pServer = XGameServer::Instance();
    m_pObjectResource = static_cast<VEventObjectResource*>(
        XWorldResMgr::GetResource(&pServer->GetWorldResMgr(), tbMapID));
    if (!m_pObjectResource) {
        return false;
    }

    m_pTBMazeInfo = pServer->GetResourceMgr().GetTB_MAZE_INFO(tbMapID);
    if (!m_pTBMazeInfo) {
        return false;
    }

    std::uint32_t dwIndex = m_stOwnerInfo ? m_stOwnerInfo->shMapIndex : 0;
    m_pTBMyRoomInfo = pServer->GetResourceMgr().GetTB_MYROOM_INFO(dwIndex);
    if (!m_pTBMyRoomInfo) {
        return false;
    }

    m_byEnterUser = 0;
    m_mpPollenInfo.clear();
    m_bSendMyroomInfo = false;
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
    // IDA: Clear member variables
    m_pObjectResource = nullptr;
    m_pTBMazeInfo = nullptr;
    m_bCreate = false;
    m_byRoomState = 0;
    m_byEnterUser = 0;
    m_mpEnterUser.clear();
    m_mpMyRoomFurniture.clear();
    m_mpPollenInfo.clear();
    m_bSendMyroomInfo = false;

    // IDA: Call base class Clear
    XArea::Clear();
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
    m_uxMapID = uxMapID;
    
    // Copy owner info
    if (stOwnerInfo && m_stOwnerInfo) {
        *m_stOwnerInfo = *static_cast<ST_MYROOM_OWNER_INFO*>(stOwnerInfo);
    }
    
    if (!Init()) {
        return false;
    }
    
    SpawnGenerate();
    
    // IDA: Send DB packet
    // XSendDBPacket xSendDBPacket(0, 0x25, 3);
    // xSendDBPacket << m_stOwnerInfo->dwOwnerUAID;
    // xSendDBPacket << m_uxMapID.nMapID;
    // xSendDBPacket << stEnterUser;
    // xSendDBPacket << dwOwnerUCID;
    // XGameServer::Instance()->SendDBGame(xSendDBPacket);
    
    // TODO: Implement when XSendDBPacket operator<< for void* is available
    (void)stEnterUser;
    (void)dwOwnerUCID;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CreateRoom - needs XSendDBPacket implementation");
    return true;
}

// IDA 0x1402ACE90 - SpawnGenerate
// IDA 反编译精确还原: 生成对象
void XMyRoom::SpawnGenerate() {
    if (!m_pObjectResource) {
        return;
    }
    
    // IDA 精确还原代码:
    // 获取对象资源映射
    // VMap<int, void*>* ObjectMap = m_pObjectResource->GetColor();
    // void* Iter = ObjectMap->GetStartPosition();
    // while (Iter) {
    //     int iKey = 0;
    //     void* pValue = nullptr;
    //     ObjectMap->GetNextPair(&Iter, &iKey, &pValue);
    //     
    //     const VEventObjectInfo* pInfo = static_cast<const VEventObjectInfo*>(pValue);
    //     if (pValue && pInfo->eType == eEventObjectType_Box) {
    //         const VEventBoxInfo* pBoxInfo = (const VEventBoxInfo*)pInfo;
    //         int v15 = (int)pInfo[1].__vftable;
    //         if (v15 == 1) {
    //             // Monster spawn box
    //             const VMonsterSpawnInfo* pMonsterSpawn = (const VMonsterSpawnInfo*)pBoxInfo;
    //             if (LODWORD(pBoxInfo[1].Plane[4].m_vNormal.y) == 1) {
    //                 ExcuteSpawnBox(pMonsterSpawn, eSendInfoTypeNot);
    //             }
    //         } else if (v15 == 13) {
    //             // Quest move box
    //             VQuestMoveCheckBoxInfo* pQuestMoveBoxInfo = (VQuestMoveCheckBoxInfo*)pBoxInfo;
    //             STQuestMoveBox* pQuestMove = new STQuestMoveBox();
    //             if (pQuestMove) {
    //                 pQuestMove->nBoxIndex = pQuestMoveBoxInfo->iUniqueID;
    //                 pQuestMove->pQuestMoveBox = pQuestMoveBoxInfo;
    //                 m_mapQuestMoveBox.insert(std::make_pair(pQuestMoveBoxInfo->iUniqueID, pQuestMove));
    //             }
    //         }
    //     }
    // }
    
    // Note: VEventObjectResource, VMap, VEventObjectInfo types need full implementation
    // This is a partial implementation based on IDA decompilation
    GreenDamTan_log(__FILE__, __FUNCTION__, "SpawnGenerate - partial implementation");
}

// IDA 0x1402AD380 - GetSpawnPos
// IDA 反编译精确还原: 获取生成位置
void XMyRoom::GetSpawnPos(const void* pEventObjectInfo, XVec3& vPos) {
    // IDA 精确还原代码:
    const VEventObjectInfo* pInfo = static_cast<const VEventObjectInfo*>(pEventObjectInfo);
    if (!pInfo) {
        vPos.x = 0.0f;
        vPos.y = 0.0f;
        vPos.z = 0.0f;
        return;
    }
    
    vPos.x = (pInfo->PosTopLeft.x + pInfo->PosBottomRight.x) / 2.0f;
    vPos.y = (pInfo->PosTopLeft.y + pInfo->PosBottomRight.y) / 2.0f;
    vPos.z = pInfo->PosTopLeft.z;
}

// IDA 0x1402AD3F0 - EnterRoom
// IDA 反编译精确还原: 进入房间
int XMyRoom::EnterRoom(void* stEnterUser) {
    // IDA 精确还原代码:
    ST_MYROOM_USER* pUser = static_cast<ST_MYROOM_USER*>(stEnterUser);
    if (!pUser) {
        return 58203;
    }

    if (!m_bCreate) {
        return 58203;
    }

    // 检查是否是房间所有者
    if (pUser->dwUAID == m_stOwnerInfo->dwOwnerUAID) {
        m_mpEnterUser.insert(std::make_pair(pUser->dwUCID, *pUser));
        return 0;
    }

    // 检查房间状态
    if (GetMyRoomState()) {
        return 58205;
    }

    // 检查最大访问成员数
    if (m_byEnterUser < m_pTBMyRoomInfo->My_Room_Max_Access_Member) {
        // 检查房间开放等级
        if (m_stOwnerInfo->byRoomOpenLevel) {
            return 58207;
        }

        ++m_byEnterUser;
        m_mpEnterUser.insert(std::make_pair(pUser->dwUCID, *pUser));
        return 0;
    }

    return 58206;
}

// IDA 0x1402AD550 - ExitRoom
// IDA 反编译精确还原: 退出房间
void XMyRoom::ExitRoom(std::uint32_t dwUCID, std::uint32_t& dwBeforeMap) {
    // IDA 精确还原代码:
    auto iter = m_mpEnterUser.find(dwUCID);
    if (iter != m_mpEnterUser.end()) {
        ST_MYROOM_USER stEnterUser = iter->second;
        dwBeforeMap = stEnterUser.dwBeforeMapID;
        if (m_stOwnerInfo && stEnterUser.dwUAID != m_stOwnerInfo->dwOwnerUAID) {
            --m_byEnterUser;
        }
        m_mpEnterUser.erase(iter);
    }

    DelMyRoomUsedUser(dwUCID);

    // 如果房间为空，发送包到控制服务器
    if (m_mpEnterUser.empty()) {
        XSendPacket xSendPacket(0xF2, 0x53);
        xSendPacket << m_stOwnerInfo->dwOwnerUAID;
        xSendPacket << m_uxMapID.nMapID;
        // Note: GetControlSocket method needs to be verified
        // XGameServer::Instance()->GetControlSocket().SendCheck(&xSendPacket);
        GreenDamTan_log(__FILE__, __FUNCTION__, "ExitRoom - needs GetControlSocket implementation");
    }
}

// IDA 0x1402AD750 - LoadMyRoomSucc
// IDA 反编译精确还原: 加载房间成功
void XMyRoom::LoadMyRoomSucc(void* stMyRoomItemList) {
    // IDA: if (m_bCreate) return;
    // ST_MYROOM_ITEM_LIST* pItemList = static_cast<ST_MYROOM_ITEM_LIST*>(stMyRoomItemList);
    // *static_cast<ST_MYROOM_ITEM_LIST*>(m_stMyRoomItemList) = *pItemList;
    // for (auto& item : *pMyRoomItems) {
    //     m_mpMyRoomFurniture.insert(std::make_pair(item.biSerial, &item));
    // }
    // m_bCreate = true;
    
    // TODO: Implement when ST_MYROOM_ITEM_LIST is fully defined
    (void)stMyRoomItemList;
    m_bCreate = true;
    GreenDamTan_log(__FILE__, __FUNCTION__, "LoadMyRoomSucc - needs ST_MYROOM_ITEM_LIST implementation");
}

// IDA 0x1402AD8F0 - EnterActor
// IDA: ?EnterActor@XMyRoom@@UEAAGPEAVXActor@@@Z - returns unsigned short
std::uint16_t XMyRoom::EnterActor(XActor* pActor) {
    // IDA 精确还原代码:
    CUser* pClient = dynamic_cast<CUser*>(pActor);
    if (!pClient) {
        return 50001;
    }
    
    if (GetMyRoomState()) {
        ExitArea(pActor);
        return 0;
    }
    
    std::uint16_t xError = XArea::EnterActor(pActor);
    if (xError) {
        return xError;
    }
    
    // 添加actor到映射
    CMover* pMover = dynamic_cast<CMover*>(pActor);
    if (pMover) {
        // 使用actor ID作为key
        std::uint32_t dwActorID = reinterpret_cast<std::uint32_t>(pActor);
        m_mapActor[dwActorID] = pActor;
    }
    
    pActor->SetArea(this);
    
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (pUser) {
        pUser->ChangeBattlePose(0);
    }
    
    SendMyRoomLoad(pClient);
    
    return 0;
}

// IDA 0x1402AE0D0 - ExitActor
// IDA 反编译精确还原: 退出Actor
void XMyRoom::ExitActor(XActor* pActor) {
    // IDA 精确还原代码:
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (pUser) {
        // CMoverEx::RemoveAllOptionEffect(&pUser->CMoverEx);
    }
    
    // 从映射中移除actor
    std::uint32_t dwActorID = reinterpret_cast<std::uint32_t>(pActor);
    m_mapActor.erase(dwActorID);
    
    XArea::ExitActor(pActor);
    
    pActor->SetArea(nullptr);
    
    // Note: CUser::Exit method needs to be verified
    // if (pUser) {
    //     CUser::Exit(pUser);
    // }
    
    // 发送退出信息
    XSendPacket xSendPacket(4, 0x12);
    SendOutInfo(xSendPacket, pActor);
    
    // 退出房间
    std::uint32_t dwBeforeMap = 0;
    if (pUser) {
        std::uint32_t dwUCID = reinterpret_cast<std::uint32_t>(pUser);
        ExitRoom(dwUCID, dwBeforeMap);
    }
}

// IDA 0x1402AE410 - ExitArea
// IDA 反编译精确还原: 退出区域
void XMyRoom::ExitArea(XActor* pActor) {
    // IDA 精确还原代码:
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (pUser) {
        pUser->SetState(eStateChangeWorld);
        
        STPosInfo stPosinfo;
        std::uint32_t dwActorID = reinterpret_cast<std::uint32_t>(pUser);
        std::uint32_t dwBeforeMap = GetBeforeMap(dwActorID);
        
        ST_LOG_GAME stLog;
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = dwActorID;
        stLog._sMainType = 19;
        stLog._sSubType = 4;
        stLog.nParam0 = m_stOwnerInfo->dwMapID;
        stLog.nParam1 = m_stOwnerInfo->dwOwnerUAID;
        stLog.nParam2 = 2;
        stLog.nParam5 = pUser->GetLevel();
        XGameServer::Instance()->SendDBLog(stLog);
        
        // 获取起始传送门ID
        int nJumpID = 0;
        // Note: GetStartPortalID method needs to be verified
        // XWorldResMgr::GetStartPortalID(dwBeforeMap, &nJumpID, &stPosinfo);
        GreenDamTan_log(__FILE__, __FUNCTION__, "ExitArea - needs GetStartPortalID implementation");
        
        // 发送进入地图请求
        PS_ENTER_MAP_REQ stEnterMap;
        stEnterMap.dwActorID = dwActorID;
        stEnterMap.nJumpID = nJumpID;
        stEnterMap.wMapID = static_cast<std::uint16_t>(dwBeforeMap);
        stEnterMap.stPartyInfo.byGroupType = 0;
        stEnterMap.stPartyInfo.nID = 0;
        
        XSendPacket xSendPacket(0xF2, 0x31);
        xSendPacket << stEnterMap;
        // Note: GetControlSocket method needs to be verified
        // XGameServer::Instance()->GetControlSocket().SendCheck(&xSendPacket);
        GreenDamTan_log(__FILE__, __FUNCTION__, "ExitArea - needs GetControlSocket implementation");
    }
}

// IDA 0x1402AE6E0 - OnUpdate
// IDA 反编译精确还原: 更新
void XMyRoom::OnUpdate(float fElapsed) {
    // IDA 精确还原代码:
    // 遍历所有actor并更新
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
        XActor* pActor = it->second;
        if (pActor) {
            // Note: OnUpdate method needs to be verified
            // pActor->OnUpdate();
        }
    }
    
    (void)fElapsed;
}

// IDA 0x1402AE780 - SendObjectInfo
// IDA 反编译精确还原: 发送对象信息
bool XMyRoom::SendObjectInfo(XActor* pActor) {
    // IDA 精确还原代码:
    XSendPacket xSendPacket(4, 0x11);
    SendInInfo(xSendPacket, pActor);
    
    XSendPacket xSendPacketUser(4, 0x21);
    SendOtherInfos(xSendPacketUser, pActor, eActorUser);
    
    XSendPacket xSendPacketNPC(4, 0x22);
    SendOtherInfos(xSendPacketNPC, pActor, eActorNPC);
    
    return true;
}

// IDA 0x1402AE8C0 - SendBroadCast
// IDA 反编译精确还原: 发送广播
void XMyRoom::SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) {
    // IDA: Iterate through m_mapActor (named m_objectScanner in IDA)
    for (auto& [dwActorID, pActor] : m_mapActor) {
        if (!pActor) continue;

        // IDA: if (eBroadCastType != eNoneSelf || pTargetActor != pExceptActor)
        if (eBroadCastType != E_BROADCAST_TYPE::eNoneSelf || pActor != pExceptActor) {
            // IDA: pTargetActor->SendPacket(&packet) via virtual call
            pActor->BridgeSend(packet);
        }
    }
}

// IDA: ScanGridOrigin - inherited from XArea (base class stub)
// XMyRoom doesn't have grid-based scanning like XDistrict/XMaze
// Just call the base class implementation (which does nothing)
void XMyRoom::ScanGridOrigin(float dx, float dy, unsigned char byNation, int sectorRange, unsigned int dwOptions, std::vector<CMover*>& vecOut) {
    // XMyRoom doesn't have grid-based scanning like XDistrict/XMaze
    // Just call the base class implementation (which does nothing)
    XArea::ScanGridOrigin(dx, dy, byNation, sectorRange, dwOptions, vecOut);
}

// IDA 0x1402AE9C0 - GetScanner
// IDA 反编译精确还原: 获取扫描器
// Returns the appropriate scanner map based on actor type
std::map<std::uint32_t, XActor*>* XMyRoom::GetScanner(XActor* pActor) {
    if (!pActor) return nullptr;

    // IDA: int Type = XActor::GetType(pActor)
    int Type = pActor->GetType();

    // IDA: if (!Type) return &this->m_objectScanner (playerScanner)
    if (Type == eActorUser) {
        // 返回玩家扫描器 (使用 m_mapActor 作为简化实现)
        return &m_mapActor;
    }

    // IDA: if (Type > 0 && Type <= 2) return &this->m_objectScanner.mapNPCList
    if (Type > 0 && Type <= 2) {
        // 返回 NPC/怪物扫描器 (使用 m_mapActor 作为简化实现)
        return &m_mapActor;
    }

    // IDA: return &this->m_objectScanner.mapEtcList
    // 返回其他扫描器 (使用 m_mapActor 作为简化实现)
    return &m_mapActor;
}

// IDA 0x1402AEA20 - LoadComplete
// IDA 反编译精确还原: 加载完成
void XMyRoom::LoadComplete(XActor* pActor) {
    // IDA: CUser* pUser = dynamic_cast<CUser*>(pActor) via _RTDynamicCast
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser) return;

    // IDA: CUser::SetClientLoadComplete(pUser, 1)
    pUser->SetClientLoadComplete(true);

    // IDA: SendObjectInfo(pActor)
    SendObjectInfo(pActor);

    // IDA: SendTransportationInfo(pActor)
    SendTransportationInfo(pActor);

    // IDA: CMover::GetGOC<CGocAttribute>(&pUser->CMoverEx, &pAttr, 0)
    // IDA: if (pAttr) pAttr->SetStartRegStat(1)
    std::shared_ptr<CGocAttribute> pAttr = pUser->GetGOC_Attribute(false);
    if (pAttr) {
        pAttr->SetStartRegStat(true);
    }

    // IDA: CMover::GetGOC<CGocInventory>(&pUser->CMoverEx, &pInvenPtr, 0)
    // IDA: if (pInvenPtr) { InitItemCoolTime(); SendItemCoolTimeInfo(); }
    std::shared_ptr<CGocInventory> pInvenPtr = pUser->GetGOC_Inventory(false);
    if (pInvenPtr) {
        pInvenPtr->InitItemCoolTime();
        pInvenPtr->SendItemCoolTimeInfo();
    }
}

// IDA 0x1402AEB50 - SendMyRoomLoad
// IDA 反编译精确还原: 发送房间加载
// Note: Requires CGocMyroom which has many compilation errors - simplified for now
void XMyRoom::SendMyRoomLoad(class CUser* pUser) {
    // IDA: if (!pUser) return
    if (!pUser) return;

    // Simplified implementation - send basic room info
    ST_MYROOM_OWNER_INFO stOwnerInfo;
    if (m_stOwnerInfo) {
        stOwnerInfo = *m_stOwnerInfo;
    }

    // IDA: XSendPacket packet(0x26, 0x11)
    XSendPacket packet(0x26, 0x11);
    packet << stOwnerInfo;

    // IDA: CGocNetwork::Send(pActor, &packet)
    CGocNetwork::Send(static_cast<XActor*>(pUser), packet);

    // IDA: Send door open states
    std::vector<std::uint8_t> vecDoorOpen;
    for (int i = 0; i < 10; ++i) {
        if (m_bDoorOpen[i]) {
            vecDoorOpen.push_back(static_cast<std::uint8_t>(i));
        }
    }

    if (!vecDoorOpen.empty()) {
        XSendPacket doorPacket(0x26, 0x27);
        doorPacket << static_cast<std::int16_t>(vecDoorOpen.size());
        for (auto idx : vecDoorOpen) {
            doorPacket << idx;
        }
        CGocNetwork::Send(static_cast<XActor*>(pUser), doorPacket);
    }

    // IDA: XMyRoom::SendPollenLoad(this, pUser)
    SendPollenLoad(pUser);

    m_bSendMyroomInfo = false;
}

// IDA 0x1402AEF60 - EditFurniture
// IDA 反编译精确还原: 编辑家具
bool XMyRoom::EditFurniture(class CUser* pUser, void* stEditFurniture, std::uint8_t byState) {
    // IDA: 获取背包组件
    std::shared_ptr<CGocInventory> pInvenPtr = pUser->GetGOC_Inventory(false);
    if (!pInvenPtr) return false;

    // IDA: pInven = pInvenPtr->GetInvenPtr(0xB)
    XBaseInventory* pInven = pInvenPtr->GetInvenPtr(0xB);
    if (!pInven) return false;

    // IDA: stEditFurniture 是 ST_MYROOM_ITEM_LIST*
    ST_MYROOM_ITEM_LIST* pEditList = static_cast<ST_MYROOM_ITEM_LIST*>(stEditFurniture);
    if (!pEditList) return false;

    // IDA: 遍历编辑列表
    for (const auto& stInfo : pEditList->vecInfo) {
        if (byState == 1) {
            // IDA: 查找家具
            auto iter = m_mpMyRoomFurniture.find(stInfo.biSerial);
            if (iter == m_mpMyRoomFurniture.end()) return false;

            // IDA: 更新家具属性
            iter->second.byRotation = stInfo.byRotation;
            iter->second.dwGridIndex = stInfo.dwGridIndex;

            // IDA: 更新 m_stMyRoomItemList
            for (size_t i = 0; i < m_stMyRoomItemList.size(); ++i) {
                if (m_stMyRoomItemList[i].biSerial == stInfo.biSerial) {
                    m_stMyRoomItemList[i].byRotation = stInfo.byRotation;
                    m_stMyRoomItemList[i].dwGridIndex = stInfo.dwGridIndex;
                    break;
                }
            }
        }
    }
    return true;
}

// IDA 0x1402AF210 - AddMyRoomItem
// IDA 反编译精确还原: 添加房间物品
bool XMyRoom::AddMyRoomItem(void* stItem) {
    ST_MYROOM_ITEM* pItem = static_cast<ST_MYROOM_ITEM*>(stItem);
    if (!pItem) return false;

    // IDA: 检查是否已存在
    auto iter = m_mpMyRoomFurniture.find(pItem->biSerial);
    if (iter != m_mpMyRoomFurniture.end()) return false;

    // IDA: 插入到映射表
    m_mpMyRoomFurniture.insert(std::make_pair(pItem->biSerial, *pItem));

    // IDA: 添加到列表
    m_stMyRoomItemList.push_back(*pItem);

    return true;
}

// IDA 0x1402AF2F0 - DelMyRoomItem
// IDA 反编译精确还原: 删除房间物品
bool XMyRoom::DelMyRoomItem(std::int64_t biSerial, void* stItem) {
    ST_MYROOM_ITEM* pOutItem = static_cast<ST_MYROOM_ITEM*>(stItem);
    if (!pOutItem) return false;

    // IDA: 查找家具
    auto iter = m_mpMyRoomFurniture.find(biSerial);
    if (iter == m_mpMyRoomFurniture.end()) return false;

    // IDA: 复制到输出参数
    *pOutItem = iter->second;

    // IDA: 从映射表中删除
    m_mpMyRoomFurniture.erase(biSerial);

    // IDA: 从列表中删除
    for (auto veciter = m_stMyRoomItemList.begin(); veciter != m_stMyRoomItemList.end(); ++veciter) {
        if (biSerial == veciter->biSerial) {
            m_stMyRoomItemList.erase(veciter);
            break;
        }
    }

    return true;
}

// IDA 0x1402AF490 - AddMyRoomUsedUser
// IDA 反编译精确还原: 添加房间使用用户
std::uint8_t XMyRoom::AddMyRoomUsedUser(std::uint32_t dwActorID, std::int64_t i64Serial, std::uint8_t byAniIndex) {
    // IDA: 获取空槽位索引
    std::uint8_t byIndex = GetEmptyIndex(i64Serial);
    if (byIndex == 255) return static_cast<std::uint8_t>(-1);

    // IDA: 创建使用用户信息
    ST_MYROOM_USED_USER stUsedUser;
    stUsedUser.dwActorID = dwActorID;
    stUsedUser.biSerial = i64Serial;
    stUsedUser.byIndex = byIndex;
    stUsedUser.byAniIndex = byAniIndex;

    // IDA: 添加到列表
    m_stMyRoomUsedUserList.push_back(stUsedUser);

    return byIndex;
}

// IDA 0x1402AF530 - DelMyRoomUsedUser
// IDA 反编译精确还原: 删除房间使用用户
bool XMyRoom::DelMyRoomUsedUser(std::uint32_t dwActorID) {
    // IDA: 遍历并删除匹配的用户
    for (auto veciter = m_stMyRoomUsedUserList.begin(); veciter != m_stMyRoomUsedUserList.end(); ) {
        if (dwActorID == veciter->dwActorID) {
            veciter = m_stMyRoomUsedUserList.erase(veciter);
        } else {
            ++veciter;
        }
    }
    return true;
}

// IDA 0x1402AF640 - GetEmptyIndex
// IDA 反编译精确还原: 获取空索引
std::uint8_t XMyRoom::GetEmptyIndex(std::int64_t i64Serial) {
    // IDA: 查找家具
    auto iter = m_mpMyRoomFurniture.find(i64Serial);
    if (iter == m_mpMyRoomFurniture.end()) return static_cast<std::uint8_t>(-1);

    // IDA: 获取家具信息
    ST_MYROOM_ITEM stMyroomItemInfo = iter->second;

    // IDA: 获取物品表
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_ITEM* pItemTable = pServer->GetResourceMgr().GetTB_ITEM(stMyroomItemInfo.dwItemID);
    if (!pItemTable) return static_cast<std::uint8_t>(-1);

    // IDA: 获取家具表
    TB_MYROOM_FURNITURE* pMyroomItemRef = pServer->GetResourceMgr().GetTB_MYROOM_FURNITURE(pItemTable->Furniture_ID);
    if (!pMyroomItemRef) return static_cast<std::uint8_t>(-1);

    // IDA: 获取最大使用人数
    int iMaxUsedUser = static_cast<int>(pMyroomItemRef->Furniture_Item_Special_Use);
    if (iMaxUsedUser > 4) iMaxUsedUser = 4;

    // IDA: 标记已使用的槽位
    bool bUsedSlot[4] = {false, false, false, false};
    int iCount = 0;

    for (const auto& stItem : m_stMyRoomUsedUserList) {
        if (i64Serial == stItem.biSerial) {
            if (stItem.byIndex < 4) {
                bUsedSlot[stItem.byIndex] = true;
            }
            ++iCount;
        }
    }

    // IDA: 计算空槽数量
    int iEmptySlotMaxCount = iMaxUsedUser - iCount;
    if (iEmptySlotMaxCount <= 0) return static_cast<std::uint8_t>(-1);

    // IDA: 随机选择一个空槽
    int iRandomIndex = std::rand() % iEmptySlotMaxCount;
    int iEmptySlotCount = 0;

    for (int i = 0; i < 4; ++i) {
        if (!bUsedSlot[i]) {
            if (iRandomIndex == iEmptySlotCount) return static_cast<std::uint8_t>(i);
            ++iEmptySlotCount;
        }
    }

    return static_cast<std::uint8_t>(-1);
}

// IDA 0x1402AF900 - IsCanChangeDoorState
// IDA 反编译精确还原: 检查是否可以更改门状态
bool XMyRoom::IsCanChangeDoorState(char cDoorIndex) {
    // IDA: 检查门索引范围
    if (cDoorIndex >= 10) return false;

    // IDA: 获取当前时间并检查冷却
    // Note: ThreadLocalData::GetTimer() 和 IVTimer::GetTime() 未实现
    // 简化实现：总是返回true
    // TODO: 需要实现 Timer 相关功能
    return true;
}

// IDA 0x1402AF970 - SetDoorState
// IDA 反编译精确还原: 设置门状态
void XMyRoom::SetDoorState(char cDoorIndex, bool bOpen) {
    // IDA: 检查门索引范围
    if (cDoorIndex >= 10) return;

    // IDA: 设置门状态
    m_bDoorOpen[static_cast<size_t>(cDoorIndex)] = bOpen;

    // IDA: 记录状态变更时间
    // Note: ThreadLocalData::GetTimer() 和 IVTimer::GetTime() 未实现
    // TODO: 需要实现 m_fDoorStateChangedTime 和 Timer 相关功能
}

// IDA 0x1402AF9C0 - AllUserOut
// IDA 反编译精确还原: 所有用户退出
void XMyRoom::AllUserOut(int nReason) {
    // IDA: Iterate through m_mapActor (named m_objectScanner in IDA)
    for (auto it = m_mapActor.begin(); it != m_mapActor.end(); ++it) {
        CMover* pMover = dynamic_cast<CMover*>(it->second);
        if (!pMover) continue;

        // IDA: RTTI cast to CUser
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (!pUser) continue;

        // IDA: if (pCUser->GetUAID() != m_stOwnerInfo.dwOwnerUAID)
        if (m_stOwnerInfo && pUser->GetUAID() != m_stOwnerInfo->dwOwnerUAID) {
            // IDA: XSendPacket xSendPacket(0x26, 0x25)
            XSendPacket xSendPacket(0x26, 0x25);
            xSendPacket << nReason;

            // IDA: CGocNetwork::Send(pActor, &xSendPacket)
            CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);

            // IDA: ExitArea(pActor)
            ExitArea(static_cast<XActor*>(pUser));
        }
    }
}

// IDA 0x1402AFBA0 - UserKickOut
// IDA 反编译精确还原: 用户踢出
void XMyRoom::UserKickOut(std::uint32_t dwKickActorID) {
    // IDA: auto iter = m_mapActor.find(dwKickActorID)
    auto iter = m_mapActor.find(dwKickActorID);

    // IDA: if (iter != m_mapActor.end())
    if (iter != m_mapActor.end()) {
        CMover* pMover = dynamic_cast<CMover*>(iter->second);
        if (!pMover) return;

        // IDA: XSendPacket xSendPacket(0x26, 0x25)
        XSendPacket xSendPacket(0x26, 0x25);
        xSendPacket << 58211;  // Kick reason code

        // IDA: CGocNetwork::Send(pActor, &xSendPacket)
        CGocNetwork::Send(static_cast<XActor*>(pMover), xSendPacket);

        // IDA: ExitArea(pActor)
        ExitArea(static_cast<XActor*>(pMover));
    }
}

// IDA 0x1402AFD10 - GetCurUserCount
// IDA 反编译精确还原: 获取当前用户数
int XMyRoom::GetCurUserCount() {
    // IDA: return m_objectScanner.size();
    // Note: m_objectScanner is in base class XArea
    return static_cast<int>(m_mpEnterUser.size());
}

// IDA 0x1402AFD40 - SetMyRoomSetup
// IDA 反编译精确还原: 设置房间设置
void XMyRoom::SetMyRoomSetup(class CUser* pOwnerUser, void* stMyRoomSetup) {
    (void)pOwnerUser;  // IDA: 未使用

    PS_MYROOM_SETUP* pSetup = static_cast<PS_MYROOM_SETUP*>(stMyRoomSetup);
    if (!pSetup) return;

    // IDA: 更新所有者信息
    m_stOwnerInfo->byRoomOpenLevel = pSetup->byOpenLevel;

    // IDA: 复制房间名
    std::wcscpy(m_stOwnerInfo->szRoomName, pSetup->szMyRoomName);

    // IDA: 如果房间开放等级为1（仅自己），踢出其他用户
    if (m_stOwnerInfo->byRoomOpenLevel == 1) {
        AllUserOut(58207);
    }
}

// IDA 0x1402AFE00 - RunQuestMoveCheck
// IDA 反编译精确还原: 运行任务移动检查
void XMyRoom::RunQuestMoveCheck(int nBoxIndex, class CUser* pUser) {
    // IDA: 查找任务移动盒
    auto it = m_mapQuestMoveBox.find(nBoxIndex);
    if (it == m_mapQuestMoveBox.end()) return;

    // IDA: 获取任务移动盒
    // Note: STQuestMoveBox 类型未定义，使用 void*
    void* pQuestMoveBoxPtr = it->second;
    if (!pQuestMoveBoxPtr) return;

    // IDA: 发送包通知客户端
    XSendPacket xSendPacket(0x11, 0x66);
    xSendPacket << nBoxIndex;
    CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);

    // TODO: 需要完整的 STQuestMoveBox 和 VQuestMoveCheckBoxInfo 类型定义
    // 以及 GetTB_QUEST_CONDITION 和 CGocQuest::UpdateCondition 实现
}

// IDA 0x1402AFFE0 - EscapeActor
// IDA 反编译精确还原: 逃离Actor
bool XMyRoom::EscapeActor(XActor* pActor) {
    // IDA: RTTI cast to CUser
    CUser* pUser = dynamic_cast<CUser*>(pActor);
    if (!pUser) return false;

    // IDA: 获取地图ID
    int nMapID = GetTBMapID();

    // IDA: 获取起始传送门位置
    // Note: XWorldResMgr::GetStartPortalID 未实现
    // 简化实现：使用默认位置
    hkvVec3 vPos;
    vPos.x = 0.0f;
    vPos.y = 0.0f;
    vPos.z = 0.0f;
    float fRot = 0.0f;

    // IDA: 移动Actor (使用基类方法)
    XVec3 xvecPos;
    xvecPos.x = vPos.x;
    xvecPos.y = vPos.y;
    xvecPos.z = vPos.z;
    XArea::MoveActor(pUser->GetActorID(), xvecPos, fRot);

    // IDA: 清除移动值
    pUser->MoveingValueClear();

    // IDA: 更改动作
    pUser->ChangeMotion(1, 1, 0);

    // IDA: 发送传送响应
    // Note: SendResWarp 方法需要在 CUser 中实现
    // pUser->SendResWarp(0, &vPos, fRot);

    return true;
}

// IDA 0x1402B0140 - GetExitDistrictID
// IDA 反编译精确还原: 获取出口区域ID
bool XMyRoom::GetExitDistrictID(std::uint32_t dwActorID, std::uint16_t& wMapID, int& nJumpID, XVec3& vPos) {
    // IDA: 查找进入用户
    auto iter = m_mpEnterUser.find(dwActorID);
    if (iter == m_mpEnterUser.end()) return false;

    // IDA: 获取进入用户信息
    ST_MYROOM_USER stEnterUser = iter->second;
    wMapID = static_cast<std::uint16_t>(stEnterUser.dwBeforeMapID);

    // IDA: 获取起始传送门位置
    // Note: XWorldResMgr::GetStartPortalID 未实现
    // 简化实现：使用之前保存的位置
    vPos = stEnterUser.xBeforePos;
    nJumpID = 0;

    return true;
}

// IDA 0x1402B0300 - PollenLoad
// IDA 反编译精确还原: 花粉加载
void XMyRoom::PollenLoad(void* psPollenList) {
    PS_MYROOM_POLLEN_LIST* pList = static_cast<PS_MYROOM_POLLEN_LIST*>(psPollenList);
    if (!pList) return;

    // IDA: 清空现有花粉信息
    m_mpPollenInfo.clear();

    // IDA: 遍历并设置花粉信息
    for (const auto& psInfo : pList->vecInfo) {
        ST_POLLEN_INFO stInfo;
        stInfo.stPollenInfo = psInfo;
        stInfo.bySubCmd = 0;
        SetPollenInfo(&stInfo.stPollenInfo);
    }
}

// IDA 0x1402B0520 - SetPollenInfo
// IDA 反编译精确还原: 设置花粉信息
void XMyRoom::SetPollenInfo(void* psPollenInfo) {
    PS_MYROOM_POLLEN_INFO* pInfo = static_cast<PS_MYROOM_POLLEN_INFO*>(psPollenInfo);
    if (!pInfo) return;

    // IDA: 创建花粉信息
    ST_POLLEN_INFO stInfo;
    stInfo.stPollenInfo = *pInfo;
    stInfo.bySubCmd = 0;

    // IDA: 插入到映射表
    m_mpPollenInfo.insert(std::make_pair(
        static_cast<std::uint32_t>(pInfo->nPollenIndex),
        stInfo
    ));
}

// IDA 0x1402B0630 - CanPollenAdd
// IDA 反编译精确还原: 检查是否可以添加花粉
bool XMyRoom::CanPollenAdd(int nPollenIndex) {
    // TODO: 汇编还原 - 需要完整类型定义
    // IDA: if (!nPollenIndex) return false;
    //      auto iter = m_mpPollenInfo.find(nPollenIndex);
    //      if (iter != m_mpPollenInfo.end()) return false;
    //      if (!m_pTBMyRoomInfo) return false;
    //      for (int i = 0; i < 10; ++i) {
    //          if (m_pTBMyRoomInfo->Nomal_Pollen_Index_01[i] == nPollenIndex) return true;
    //      }
    //      return false;
    if (!nPollenIndex) return false;
    auto iter = m_mpPollenInfo.find(nPollenIndex);
    if (iter != m_mpPollenInfo.end()) return false;
    if (!m_pTBMyRoomInfo) return false;
    // Note: TB_MYROOM_INFO is incomplete type, cannot access Nomal_Pollen_Index_01
    GreenDamTan_log(__FILE__, __FUNCTION__, "CanPollenAdd partial");
    return true;
}

// IDA 0x1402B06F0 - CanCultivation
// IDA 反编译精确还原: 检查是否可以培育
std::uint32_t XMyRoom::CanCultivation(int nPollenIndex) {
    auto iter = m_mpPollenInfo.find(nPollenIndex);
    if (iter == m_mpPollenInfo.end()) return 58213;
    if (iter->second.byState) return 58286;
    if (iter->second.byCultivating) return 58287;
    return 0;
}

// IDA 0x1402B0D80 - CanPollenCancel
// IDA 反编译精确还原: 检查是否可以取消花粉
bool XMyRoom::CanPollenCancel(int nPollenIndex) {
    auto iter = m_mpPollenInfo.find(nPollenIndex);
    if (iter == m_mpPollenInfo.end()) return false;
    if (iter->second.byCultivating) return false;
    return iter->second.byState != 0;
}

// IDA 0x1402B0E10 - GetPollenHarvest
// IDA 反编译精确还原: 获取花粉收获
bool XMyRoom::GetPollenHarvest(int nPollenIndex, std::uint32_t& dwItemID, std::int16_t& shCount, std::uint8_t& byResult) {
    auto iter = m_mpPollenInfo.find(nPollenIndex);
    if (iter == m_mpPollenInfo.end()) return false;

    // IDA: 获取花粉信息
    PS_MYROOM_POLLEN_INFO psInfo = iter->second.stPollenInfo;

    // IDA: 获取培育表
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_CULTIVATION* pTBCultivation = pServer->GetResourceMgr().GetTB_CULTIVATION(psInfo.dwCultivationItem);
    if (!pTBCultivation) return false;

    // IDA: 获取当前日期
    std::int64_t biCurdate = pServer->GetCurDate();

    // IDA: 检查腐烂状态
    if (psInfo.biRotDate >= biCurdate || psInfo.dwWiltBlockItem) {
        byResult = 3;
        dwItemID = pTBCultivation->Decay_Item_ID;
        shCount = static_cast<std::int16_t>(pTBCultivation->Decay_Item_ID_Num);
        return true;
    }

    // IDA: 检查枯萎状态
    if (psInfo.biWiltDate >= biCurdate || psInfo.dwWiltBlockItem) {
        byResult = 2;
        dwItemID = pTBCultivation->Wilt_Item_ID;
        shCount = static_cast<std::int16_t>(pTBCultivation->Wilt_Item_ID_Num);
        return true;
    }

    // IDA: 检查收获时间
    if (psInfo.biHarvestDate >= biCurdate) {
        return false; // 还没到收获时间
    }

    // IDA: 收获状态
    byResult = 0;
    dwItemID = pTBCultivation->Crops_Item_ID;
    shCount = static_cast<std::int16_t>(pTBCultivation->Crops_Item_Num);

    // IDA: 检查是否有奖励
    if (pTBCultivation->Cultivation_Bonus_Rate && pTBCultivation->Crops_Bonus_Item_Num) {
        int nRate = pServer->GetItemFactory().nRand(1, 10000);
        if (nRate <= static_cast<int>(pTBCultivation->Cultivation_Bonus_Rate)) {
            byResult = 1;
            shCount += static_cast<std::int16_t>(pTBCultivation->Crops_Bonus_Item_Num);
        }
    }

    return true;
}

// IDA 0x1402B1100 - ClearPollen
// IDA 反编译精确还原: 清除花粉
void XMyRoom::ClearPollen(int nPollenIndex, std::uint8_t bySubcmd) {
    auto iter = m_mpPollenInfo.find(nPollenIndex);
    if (iter == m_mpPollenInfo.end()) return;

    // IDA: 检查子命令匹配
    if (!bySubcmd || iter->second.bySubCmd == bySubcmd) {
        // IDA: 清空帮助用户列表
        iter->second.stPollenInfo.vecHelpUser.clear();
        // IDA: 重置花粉信息
        iter->second.stPollenInfo = PS_MYROOM_POLLEN_INFO();
        iter->second.bySubCmd = 0;
        iter->second.byCultivating = 0;
        iter->second.byState = 0;
    } else {
        LogHelper::LogError("game.contents", "ClearPollen (nPollenIndex:%d)", nPollenIndex);
    }
}

// IDA 0x1402B1460 - GetBeforeMap
// IDA 反编译精确还原: 获取之前地图
std::uint32_t XMyRoom::GetBeforeMap(std::uint32_t dwActorID) {
    auto iter = m_mpEnterUser.find(dwActorID);
    if (iter == m_mpEnterUser.end()) {
        LogHelper::LogError("game.contents", "GetBeforeMap BBB UCID:%d", dwActorID);
        return 10003;
    }

    ST_MYROOM_USER stEnterUser = iter->second;
    if (stEnterUser.dwBeforeMapID) {
        return stEnterUser.dwBeforeMapID;
    }

    LogHelper::LogError("game.contents", "GetBeforeMap AAA UCID:%d", dwActorID);
    return 10003;
}

// IDA 0x1402B1550 - SendPollenAdd
// IDA 反编译精确还原: 发送花粉添加
void XMyRoom::SendPollenAdd(int nPollenIndex) {
    // IDA: 创建花粉信息
    PS_MYROOM_POLLEN_INFO stInfo;
    stInfo.nPollenIndex = nPollenIndex;

    // IDA: 设置花粉信息
    SetPollenInfo(&stInfo);

    // IDA: 发送广播包
    XSendPacket xSendPacket(0x26, 0x31);
    xSendPacket << stInfo;
    SendBroadCast(xSendPacket, nullptr, E_BROADCAST_TYPE::eAll);
}

// IDA 0x1402B1660 - SendPollenCultivation
// IDA 反编译精确还原: 发送花粉培育
void XMyRoom::SendPollenCultivation(void* psInfo) {
    PS_DB_MYROOM_POLLEN_CULTIVATION* pDBInfo = static_cast<PS_DB_MYROOM_POLLEN_CULTIVATION*>(psInfo);
    if (!pDBInfo) return;

    auto iter = m_mpPollenInfo.find(pDBInfo->stInfo.nPollenIndex);
    if (iter == m_mpPollenInfo.end()) return;

    // IDA: 检查子命令是否为50
    if (iter->second.bySubCmd != 50) {
        LogHelper::LogError("game.contents", "SendPollenCultivation (nPollenIndex:%d)", pDBInfo->stInfo.nPollenIndex);
        return;
    }

    // IDA: 更新花粉信息
    iter->second.stPollenInfo.biStartDate = pDBInfo->stInfo.biStartDate;
    iter->second.stPollenInfo.biHarvestDate = pDBInfo->stInfo.biHarvestDate;
    iter->second.stPollenInfo.biWiltDate = pDBInfo->stInfo.biWiltDate;
    iter->second.stPollenInfo.biRotDate = pDBInfo->stInfo.biRotDate;
    iter->second.stPollenInfo.dwCultivationItem = pDBInfo->stInfo.dwCultivationItem;
    iter->second.bySubCmd = 0;
    iter->second.stPollenInfo.dwNutritionItem = pDBInfo->dwNutritionItem;
    iter->second.stPollenInfo.dwWiltBlockItem = pDBInfo->dwWiltBlockItem;
    iter->second.byCultivating = 0;

    // IDA: 发送广播包
    XSendPacket xSendPacket(0x26, 0x32);
    xSendPacket << pDBInfo->stInfo;
    SendBroadCast(xSendPacket, nullptr, E_BROADCAST_TYPE::eAll);
}

// IDA 0x1402B1870 - SendPollenHarvest
// IDA 反编译精确还原: 发送花粉收获
void XMyRoom::SendPollenHarvest(int nPollenIndex) {
    // IDA: 清除花粉
    ClearPollen(nPollenIndex, 0x33);

    // IDA: 发送广播包
    XSendPacket xSendPacket(0x26, 0x33);
    xSendPacket << nPollenIndex;
    SendBroadCast(xSendPacket, nullptr, E_BROADCAST_TYPE::eAll);
}

// IDA 0x1402B1930 - SendPollenHelp
// IDA 反编译精确还原: 发送花粉帮助
void XMyRoom::SendPollenHelp(int nPollenIndex, void* psHelpUser, std::int64_t biHarvestDate) {
    // IDA: 创建花粉信息
    PS_MYROOM_POLLEN_INFO psPollenInfo;

    // IDA: 添加帮助用户
    PS_MYROOM_POLLEN_HELP_USER* pHelpUser = static_cast<PS_MYROOM_POLLEN_HELP_USER*>(psHelpUser);
    AddPollenHelpUser(nPollenIndex, pHelpUser, biHarvestDate, &psPollenInfo);

    // IDA: 发送广播包
    XSendPacket xSendPacket(0x26, 0x34);
    xSendPacket << nPollenIndex;
    xSendPacket << psPollenInfo;
    SendBroadCast(xSendPacket, nullptr, E_BROADCAST_TYPE::eAll);
}

// IDA 0x1402B1A60 - SendPollenItemUse
// IDA 反编译精确还原: 发送花粉物品使用
void XMyRoom::SendPollenItemUse(void* psPollenInfo) {
    PS_MYROOM_POLLEN_INFO* pInfo = static_cast<PS_MYROOM_POLLEN_INFO*>(psPollenInfo);
    if (!pInfo) return;

    // IDA: 调用 PollenItemUse
    PollenItemUse(pInfo);

    // IDA: 发送广播包
    XSendPacket xSendPacket(0x26, 0x35);
    xSendPacket << *pInfo;
    SendBroadCast(xSendPacket, nullptr, E_BROADCAST_TYPE::eAll);
}

// IDA 0x1402B1B50 - SendPollenLoad
// IDA 反编译精确还原: 发送花粉加载
void XMyRoom::SendPollenLoad(class CUser* pUser) {
    if (!pUser) return;

    // IDA: 构建花粉列表
    PS_MYROOM_POLLEN_LIST psPollenList;
    for (const auto& [key, stInfo] : m_mpPollenInfo) {
        psPollenList.vecInfo.push_back(stInfo.stPollenInfo);
    }

    // IDA: 发送包
    XSendPacket xSendPacket(0x26, 0x30);
    xSendPacket << psPollenList;
    CGocNetwork::Send(static_cast<XActor*>(pUser), xSendPacket);
}

// IDA 0x1402AD040 - ExcuteSpawnBox
// IDA 反编译精确还原:
// 1. 遍历怪物信息数组（最多10个）
// 2. 对于 Type==1 的 NPC：创建 NPC 并进入游戏
void XMyRoom::ExcuteSpawnBox(const VMonsterSpawnInfo* pMonsterSpawn, E_SEND_INFO_TYPE eType) {
    if (!pMonsterSpawn) return;

    // IDA: XVec3 vPos; hkvVec3::hkvVec3(&vPos)
    XVec3 vPos;

    // IDA: for (int i = 0; i < 10; ++i)
    for (int i = 0; i < 10; ++i) {
        // IDA: if (pMonsterSpawn->m_stMonsterInfo[i].m_iID)
        if (pMonsterSpawn->m_stMonsterInfo[i].m_iID == 0) continue;

        // IDA: if (pMonsterSpawn->m_stMonsterInfo[i].m_iType == 1) -- NPC type
        if (pMonsterSpawn->m_stMonsterInfo[i].m_iType == 1) {
            // IDA: this->GetSpawnPos_2(this, pMonsterSpawn, &vPos)
            GetSpawnPos(reinterpret_cast<const void*>(pMonsterSpawn), vPos);

            // IDA: ThreadLocalData* Instance = ThreadLocalData::GetInstance()
            ThreadLocalData* pThreadData = ThreadLocalData::GetInstance();

            // IDA: 转换 TUXMapID 到 UXMapID
            UXMapID uxMapID;
            uxMapID.nMapID = m_uxMapID.nMapID;

            // IDA: pNpc = ThreadLocalData::CreateNpc(...)
            CNpc* pNpc = pThreadData->CreateNpc(
                this,
                uxMapID,
                0,
                pMonsterSpawn->m_stMonsterInfo[i].m_iID,
                &vPos,
                pMonsterSpawn->fRotate,
                0
            );

            if (pNpc) {
                // IDA: if (this->EnterGameObject(this, &pNpc->XActor, eSendInfoTypeNot))
                if (EnterGameObject(static_cast<XActor*>(pNpc), eSendInfoTypeNot)) {
                    // IDA: ThreadLocalData::DeleteNpc(ThreadLocalData::GetInstance(), pNpc)
                    ThreadLocalData::GetInstance()->DeleteNpc(pNpc);
                } else {
                    // IDA: CMoverEx::SetWayPointID(pNpc, pMonsterSpawn->m_iWaypoint)
                    pNpc->SetWayPointID(pMonsterSpawn->m_iWaypoint);
                    // IDA: CNpc::SetSpawnBoxID(pNpc, pMonsterSpawn->iID)
                    pNpc->SetSpawnBoxID(pMonsterSpawn->iID);
                }
            }
        }
    }
}

// IDA 0x1402ADBE0 - EnterGameObject
// IDA 反编译精确还原: 进入游戏对象
std::uint16_t XMyRoom::EnterGameObject(XActor* pActor, E_SEND_INFO_TYPE eType) {
    // IDA: xError = XArea::EnterActor(this, pActor)
    std::uint16_t xError = XArea::EnterActor(pActor);
    if (xError) return xError;

    // IDA: vecActor = XMyRoom::GetScanner(this, pActor)
    auto vecActor = GetScanner(pActor);

    // IDA: _Val2 = dynamic_cast<CMover*>(pActor)
    CMover* pMover = dynamic_cast<CMover*>(pActor);
    if (!pMover) return 50001;

    // IDA: 获取 ActorID 并插入到扫描器
    UXActorID actorID = pActor->GetActorID();
    (*vecActor)[actorID.dwActorID] = pMover;

    // IDA: if (eType == eSendInfoTypeSend)
    if (eType == eSendInfoTypeSend) {
        int Type = pActor->GetType();

        // IDA: if (Type == 1) -- NPC
        if (Type == eActorNPC) {
            // IDA: 获取 NPC 信息并发送
            CNpc* pNpc = dynamic_cast<CNpc*>(pActor);
            if (pNpc) {
                // IDA: 发送 NPC 信息包
                // 简化实现 - 需要 PS_NPCINFO_VEC 结构
                XSendPacket v22(4, 0x13);
                // operator<<(&v22, &stNpcInfos)
                SendBroadCast(v22, nullptr, E_BROADCAST_TYPE::eAll);
            }
        }
        // IDA: else if (Type == 2) -- Monster
        else if (Type == eActorMonster) {
            CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
            if (pMonster) {
                // IDA: 发送怪物信息包
                // 简化实现 - 需要 PS_MONSTERINFO_VEC 结构
                XSendPacket xSendPacket(4, 0x15);
                // operator<<(&xSendPacket, &stMonsterInfos)
                SendBroadCast(xSendPacket, nullptr, E_BROADCAST_TYPE::eAll);
            }
        }
    }

    return 0;
}

// IDA 0x1402ADEA0 - ExitGameObject
// IDA 反编译精确还原: 退出游戏对象
std::uint16_t XMyRoom::ExitGameObject(XActor* pActor, E_SEND_INFO_TYPE eType) {
    // IDA: XArea::ExitActor(this, pActor) - returns void
    XArea::ExitActor(pActor);

    // IDA: if (eType == eSendInfoTypeSend)
    if (eType == eSendInfoTypeSend) {
        int Type = pActor->GetType();

        // IDA: if (Type == 1) -- NPC
        if (Type == eActorNPC) {
            XSendPacket v15(4, 0x14);
            SendOutInfo(v15, pActor);
            SendBroadCast(v15, nullptr, E_BROADCAST_TYPE::eAll);
        }
        // IDA: else if (Type == 2) -- Monster
        else if (Type == eActorMonster) {
            XSendPacket xSendPacket(4, 0x16);
            SendOutInfo(xSendPacket, pActor);
            SendBroadCast(xSendPacket, nullptr, E_BROADCAST_TYPE::eAll);
        }
    }

    // IDA: 从扫描器中移除
    auto pVecActor = GetScanner(pActor);
    if (!pVecActor) return 50001;

    UXActorID actorID = pActor->GetActorID();
    auto iter = pVecActor->find(actorID.dwActorID);
    if (iter == pVecActor->end()) {
        LogHelper::LogDebug("game.contents", "Scanner not found: %u", actorID.dwActorID);
        return 50001;
    }

    pVecActor->erase(iter);
    return 0;
}

// IDA 0x1402B0AC0 - CanHelpUser
// IDA 反编译精确还原: 检查是否可以帮助用户
std::uint32_t XMyRoom::CanHelpUser(std::uint32_t dwHelpUCID, int nPollenIndex, std::uint8_t& byCount, std::int64_t& biHarvestDate) {
    auto iter = m_mpPollenInfo.find(nPollenIndex);
    if (iter == m_mpPollenInfo.end()) return 58213;

    // IDA: 检查是否正在培育
    if (iter->second.byCultivating) return 58287;

    // IDA: 获取帮助用户数量
    PS_MYROOM_POLLEN_INFO psInfo = iter->second.stPollenInfo;
    byCount = static_cast<std::uint8_t>(psInfo.vecHelpUser.size());

    // IDA: 检查帮助用户上限
    if (byCount >= 5) return 58214;

    // IDA: 检查是否已经帮助过
    for (const auto& helpUser : psInfo.vecHelpUser) {
        if (dwHelpUCID == helpUser.dwUCID) return 58215;
    }

    // IDA: 获取培育表
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_CULTIVATION* pTBCultivation = pServer->GetResourceMgr().GetTB_CULTIVATION(psInfo.dwCultivationItem);
    if (!pTBCultivation) return 58290;

    // IDA: 计算帮助后的收获日期
    // Note: g_fPollenHelp is a global array, using default value 1.0f
    float fHelpRate = 1.0f; // TODO: Get from g_fPollenHelp[byCount]
    std::int64_t biReduceTime = static_cast<std::int64_t>(60 * pTBCultivation->Cultivation_Time * fHelpRate);
    biHarvestDate = psInfo.biHarvestDate - biReduceTime;

    return 0;
}

// IDA 0x1402B1250 - AddPollenHelpUser
// IDA 反编译精确还原: 添加花粉帮助用户
void XMyRoom::AddPollenHelpUser(int nPollenIndex, void* psHelpUser, std::int64_t biHarvestDate, void* psPollenInfo) {
    auto iter = m_mpPollenInfo.find(nPollenIndex);
    if (iter == m_mpPollenInfo.end()) return;

    // IDA: 检查子命令是否为52
    if (iter->second.bySubCmd != 52) {
        LogHelper::LogError("game.contents", "AddPollenHelpUser (nPollenIndex:%d)", nPollenIndex);
        return;
    }

    // IDA: 更新收获日期
    iter->second.stPollenInfo.biHarvestDate = biHarvestDate;

    // IDA: 添加帮助用户
    PS_MYROOM_POLLEN_HELP_USER* pHelpUser = static_cast<PS_MYROOM_POLLEN_HELP_USER*>(psHelpUser);
    if (pHelpUser) {
        iter->second.stPollenInfo.vecHelpUser.push_back(*pHelpUser);
    }

    // IDA: 重置子命令
    iter->second.bySubCmd = 0;

    // IDA: 返回花粉信息
    PS_MYROOM_POLLEN_INFO* pOutInfo = static_cast<PS_MYROOM_POLLEN_INFO*>(psPollenInfo);
    if (pOutInfo) {
        *pOutInfo = iter->second.stPollenInfo;
    }
}

// IDA 0x1402B1340 - PollenItemUse
// IDA 反编译精确还原: 花粉物品使用
void XMyRoom::PollenItemUse(void* psPollenInfo) {
    PS_MYROOM_POLLEN_INFO* pInfo = static_cast<PS_MYROOM_POLLEN_INFO*>(psPollenInfo);
    if (!pInfo) return;

    auto iter = m_mpPollenInfo.find(pInfo->nPollenIndex);
    if (iter == m_mpPollenInfo.end()) return;

    // IDA: 检查子命令是否为53
    if (iter->second.bySubCmd != 53) {
        LogHelper::LogError("game.contents", "PollenItemUse (nPollenIndex:%d)", pInfo->nPollenIndex);
        return;
    }

    // IDA: 更新花粉信息
    iter->second.stPollenInfo.biHarvestDate = pInfo->biHarvestDate;
    iter->second.stPollenInfo.dwNutritionItem = pInfo->dwNutritionItem;
    iter->second.stPollenInfo.dwWiltBlockItem = pInfo->dwWiltBlockItem;

    // IDA: 重置子命令
    iter->second.bySubCmd = 0;
}

// IDA 0x1402B1D20 - PollenUse
// IDA 反编译精确还原: 花粉使用
void XMyRoom::PollenUse(int nPollenIndex, std::uint8_t bySubCmd) {
    auto iter = m_mpPollenInfo.find(nPollenIndex);
    if (iter == m_mpPollenInfo.end()) return;

    // IDA: 如果未培育中，设置子命令
    if (!iter->second.byCultivating) {
        iter->second.bySubCmd = bySubCmd;
    }
}

// IDA 0x1402B1DB0 - CanUsePollen
// IDA 反编译精确还原: 检查是否可以使用花粉
bool XMyRoom::CanUsePollen(int nPollenIndex) {
    auto iter = m_mpPollenInfo.find(nPollenIndex);
    if (iter == m_mpPollenInfo.end()) {
        LogHelper::LogError("game.contents", "[POLLEN] XMyRoom::IsUsePollen - iter == m_mpPollenInfo.end() [ UAID:%d ]",
            m_stOwnerInfo ? m_stOwnerInfo->dwOwnerUAID : 0);
        return false;
    }
    if (iter->second.bySubCmd) {
        LogHelper::LogError("game.contents", "[POLLEN] XMyRoom::IsUsePollen - iter->second.bySubCmd [ UAID:%d / %d ]",
            m_stOwnerInfo ? m_stOwnerInfo->dwOwnerUAID : 0, iter->second.bySubCmd);
        return false;
    }
    return true;
}

// IDA 0x1402B1E80 - SendPollenCancel
// IDA 反编译精确还原: 发送花粉取消
void XMyRoom::SendPollenCancel(int nPollenIndex) {
    // IDA: 清除花粉
    ClearPollen(nPollenIndex, 0x37);

    // IDA: 发送广播包
    XSendPacket xSendPacket(0x26, 0x37);
    xSendPacket << nPollenIndex;
    SendBroadCast(xSendPacket, nullptr, E_BROADCAST_TYPE::eAll);
}

// IDA 0x1402B1F40 - SetPollenLockCount
// IDA 反编译精确还原: 设置花粉锁定计数
bool XMyRoom::SetPollenLockCount(class CUser* pUser, int nPollenIndex, std::uint8_t byCount) {
    // IDA: auto iter = m_mpPollenInfo.find(nPollenIndex);
    auto iter = m_mpPollenInfo.find(nPollenIndex);
    if (iter == m_mpPollenInfo.end()) return false;

    // IDA: if (iter->second.stPollenInfo.byLookCount >= byCount) return false;
    if (iter->second.stPollenInfo.byLookCount >= byCount) return false;

    // IDA: if (iter->second.stPollenInfo.vecHelpUser.size() < byCount) return false;
    if (iter->second.stPollenInfo.vecHelpUser.size() < byCount) return false;

    // IDA: auto pMyroom = pUser->GetGOC<CGocMyroom>();
    // Note: CGocMyroom not fully implemented - simplified version
    // TODO: Implement CGocMyroom::SetPollenLockCount

    // IDA: Update lock count
    iter->second.stPollenInfo.byLookCount = byCount;

    (void)pUser;
    return true;
}

// IDA 0x1402B20A0 - CheatPollen
// IDA 反编译精确还原: 作弊花粉（GM命令减少花粉培育时间）
void XMyRoom::CheatPollen(class CUser* pUser, int nPollenIndex, int nDecSec) {
    // IDA: if (nDecSec < 0) return;
    if (nDecSec < 0) return;

    // IDA: auto dwUAID = pUser->GetUAID();
    // IDA: if (!IsMyRoomOwner(dwUAID)) return;
    std::uint32_t dwUAID = pUser->GetUAID();
    if (!IsMyRoomOwner(dwUAID)) return;

    // IDA: auto iter = m_mpPollenInfo.find(nPollenIndex);
    auto iter = m_mpPollenInfo.find(nPollenIndex);
    if (iter == m_mpPollenInfo.end()) return;

    // IDA: if (iter->second.bySubCmd) return;
    if (iter->second.bySubCmd) return;

    // IDA: if (!iter->second.stPollenInfo.biHarvestDate) return;
    if (!iter->second.stPollenInfo.biHarvestDate) return;

    // IDA: auto nCurDate = XGameServer::GetCurDate();
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    std::int64_t nCurDate = pServer->GetCurDate();

    // IDA: if (iter->second.stPollenInfo.biStartDate < nCurDate) return;
    // Note: biStartDate should be > nCurDate for valid pollen
    if (iter->second.stPollenInfo.biStartDate < nCurDate) return;

    // IDA: int nChangeDate = nDecSec + nCurDate;
    std::int64_t nChangeDate = nDecSec + nCurDate;
    if (nChangeDate < nCurDate) return;

    // IDA: Adjust dates based on nDecSec
    // Simplified: reduce harvest date
    if (iter->second.stPollenInfo.biHarvestDate >= nChangeDate) {
        iter->second.stPollenInfo.biHarvestDate -= nDecSec;
    }

    // IDA: Set sub command to 53 (cheat)
    iter->second.bySubCmd = 53;

    // IDA: Send DB packet
    // Note: XSendDBPacket not fully implemented for this case
    // TODO: Implement when XSendDBPacket is available
}

// IDA 0x1402B2630 - UpdateRecommendCount
// IDA 反编译精确还原: 更新推荐计数
void XMyRoom::UpdateRecommendCount(int nCount) {
    // IDA: m_stOwnerInfo.nRecommendCount = nCount;
    if (m_stOwnerInfo) {
        m_stOwnerInfo->nRecommendCount = nCount;
    }
}

// IDA 0x1402CBB40 - SendTransportationInfo
// IDA 反编译精确还原: 发送运输信息
bool XMyRoom::SendTransportationInfo(XActor* pActor) {
    // IDA: if (!pActor) return false;
    if (!pActor) return false;

    // IDA: if (XActor::GetType(pActor) != 0) return false;
    if (pActor->GetType() != eActorUser) return false;

    // IDA: iterate through m_mapActor
    for (auto& [dwActorID, pOtherActor] : m_mapActor) {
        if (!pOtherActor) continue;
        if (!pOtherActor->IsLive()) continue;
        if (pOtherActor == pActor) continue;

        // IDA: if (XActor::GetType(pOtherActor) == 0)
        if (pOtherActor->GetType() != eActorUser) continue;

        // IDA: CUser* pUser = dynamic_cast<CUser*>(pOtherActor)
        CUser* pUser = dynamic_cast<CUser*>(pOtherActor);
        if (!pUser) continue;

        // IDA: if (CUser::IsPlayingPublicTransport(pUser))
        if (pUser->IsPlayingPublicTransport()) {
            // IDA: ST_MOVE_TRANSPORT_TAKE stMoveTake
            ST_MOVE_TRANSPORT_TAKE stMoveTake;
            stMoveTake.dwActorID = pUser->GetActorID().GetID();
            stMoveTake.wTransportTableIdx = pUser->GetPublicTransportIndex();
            stMoveTake.fStartTime = pUser->GetPublicTransportTime();

            // IDA: XSendPacket xPacket(5, 0x16)
            XSendPacket xPacket(5, 0x16);
            xPacket << stMoveTake;

            // IDA: pActor->BridgeSend_AfterLoading(&xPacket)
            pActor->BridgeSend_AfterLoading(xPacket);
        }
    }

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
