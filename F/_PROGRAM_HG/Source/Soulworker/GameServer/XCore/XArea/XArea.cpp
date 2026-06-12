#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"

// IDA 0x1408F0CA0 - XArea constructor
XArea::XArea()
    : m_mapActor()
    , m_rwLock()
    , m_uxMapID()
    , m_nMaxUserCount(0)
    , m_listSpawnNPC()
    , m_mapWorldMode()
    , m_vecWorldModeList_dummy()
{
    // IDA 精确还原代码:
    // this->__vftable = (XArea_vtbl *)&XArea::`vftable';
    // TXMap<unsigned long,XActor *>::TXMap(&this->m_mapActor);
    // InitializeSRWLock(&this->m_rwLock.m_SharedLock);
    // this->m_uxMapID.nMapID = 0;
    // this->m_nMaxUserCount = 0;
    // std::_List_val<XDistrict *>::_List_val(&this->m_listSpawnNPC);
    // std::map<unsigned long,TB_SOULSTONE_LEVELUP>::map(&this->m_mapWorldMode);
    // this->m_vecWorldModeList.vecInfo._Myfirst = nullptr;
    // this->m_vecWorldModeList.vecInfo._Mylast = nullptr;
    // this->m_vecWorldModeList.vecInfo._Myend = nullptr;
    GreenDamTan_log(__FILE__, __FUNCTION__, "XArea constructed");
}

// IDA 0x1408F0B90 - XArea destructor
XArea::~XArea() {
    // IDA 精确还原代码:
    // this->__vftable = (XArea_vtbl *)&XArea::`vftable';
    // if (m_vecWorldModeList.vecInfo._Myfirst) operator delete(m_vecWorldModeList.vecInfo._Myfirst);
    // m_vecWorldModeList.vecInfo._Myfirst = nullptr;
    // m_vecWorldModeList.vecInfo._Mylast = nullptr;
    // m_vecWorldModeList.vecInfo._Myend = nullptr;
    // std::_Tree<...>::erase(&this->m_mapWorldMode, ...);
    // operator delete(this->m_mapWorldMode._Myhead);
    // ... 清理 m_listSpawnNPC ...
    // ATL::CAtlMap<...>::~CAtlMap(&this->m_mapActor.m_AtlMap);
    // this->__vftable = (XArea_vtbl *)&IXArea::`vftable';

    Clear();
    m_mapActor.clear();
    m_listSpawnNPC.clear();
    m_mapWorldMode.clear();
    m_vecWorldModeList_dummy.clear();
    GreenDamTan_log(__FILE__, __FUNCTION__, "XArea destructed");
}

// IDA 0x1408F0C70 - Clear
void XArea::Clear() {
    // IDA 精确还原代码:
    // ATL::CAtlMap<...>::RemoveAll(&this->m_mapActor.m_AtlMap);
    // std::_Tree<...>::clear(&this->m_mapWorldMode);
    // std::vector<ST_WORLD_MODE_INFO>::clear(&this->m_vecWorldModeList.vecInfo);
    m_mapActor.clear();
    m_mapWorldMode.clear();
    m_vecWorldModeList_dummy.clear();
}

// IDA 0x1408F0660 - EnterActor
// IDA: ?EnterActor@XArea@@UEAAGPEAVXActor@@@Z - returns unsigned short (0 = success, 50001 = already exists)
std::uint16_t XArea::EnterActor(XActor* pActor) {
    // IDA 精确还原代码:
    // UXActorID uxActorID;
    // pActor->GetActorID(pActor, &uxActorID);
    // auto iter = m_mapActor.find(uxActorID.dwActorID);
    // if (iter != m_mapActor.end()) return 50001;  // 已存在
    // m_mapActor[uxActorID.dwActorID] = pActor;
    // return 0;

    if (!pActor) return 0;

    CFAutoSlimWriteLock lock(m_rwLock);
    UXActorID uxActorID = pActor->GetActorID();
    auto it = m_mapActor.find(uxActorID.dwActorID);
    if (it != m_mapActor.end()) {
        return 50001; // 已存在，返回 50001 错误码
    }
    m_mapActor[uxActorID.dwActorID] = pActor;
    return 0;
}

// IDA 0x1408F0620 - ExitActor
// IDA 反编译精确还原: 退出Actor
void XArea::ExitActor(XActor* pActor) {
    // IDA 精确还原代码:
    // UXActorID uxActorID;
    // pActor->GetActorID(pActor, &uxActorID);
    // if (ATL::CAtlMap<...>::RemoveKey(&this->m_mapActor.m_AtlMap, uxActorID.dwActorID)) {
    //     return 0;
    // } else {
    //     return 50001;  // 不存在
    // }

    if (!pActor) return;

    CFAutoSlimWriteLock lock(m_rwLock);
    UXActorID uxActorID = pActor->GetActorID();
    m_mapActor.erase(uxActorID.dwActorID);
}

// IDA 0x1408EF530 - FindActor
// IDA 反编译精确还原: 查找Actor
XActor* XArea::FindActor(std::uint32_t dwActorID) {
    // IDA 精确还原代码:
    // if (m_mapActor.m_AtlMap.m_ppBins) {
    //     for (auto node = m_ppBins[dwActorID % m_nBins]; node; node = node->m_pNext) {
    //         if (node->m_nHash == dwActorID && node->m_key == dwActorID)
    //             return node->m_value;
    //     }
    // }
    // return nullptr;

    CFAutoSlimReadLock lock(m_rwLock);
    auto it = m_mapActor.find(dwActorID);
    if (it != m_mapActor.end()) {
        return it->second;
    }
    return nullptr;
}

// IDA 0x1408EF570 - GetActorCount
// IDA 反编译精确还原: 获取Actor数量
int XArea::GetActorCount(int eType) {
    // IDA 精确还原代码:
    // int count = 0;
    // for (auto& [key, pActor] : m_mapActor) {
    //     if (pActor && pActor->IsLive() && eType == pActor->m_eActorType)
    //         ++count;
    // }
    // return count;

    // TODO: 需要实现 E_ACTOR_TYPE 枚举和 XActor::m_eActorType
    (void)eType;
    CFAutoSlimReadLock lock(m_rwLock);
    return static_cast<int>(m_mapActor.size());
}

// IDA 0x1408EF100 - IsPvPZone
// IDA 反编译精确还原: 检查是否是PvP区域
bool XArea::IsPvPZone() {
    // IDA 精确还原代码:
    // XResourceMgr* pMgr = GetResourceMgr();
    // return pMgr
    //     && (GetWorldType() == 2 && XResourceMgr::GetServerContents(pMgr, E_SERVER_OPTION_PVP_DISTRICT)
    //     || ((m_uxMapID.nMapID << 16) >> 48) == 20005);

    // TODO: 需要实现 GetResourceMgr 和 GetServerContents
    return false;
}

// IDA 0x1406E04B0 - SetMaxUserCount
void XArea::SetMaxUserCount(int nMaxUserCount) {
    m_nMaxUserCount = nMaxUserCount;
}

// GetTBMapID is inline in XArea.h (IDA 0x1400492D0)

// ============================================================================
// Broadcast Functions
// ============================================================================

// IDA 0x1408EF660 - SendBroadCast
// IDA 反编译精确还原: 发送广播
void XArea::SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) {
    CFAutoSlimReadLock lock(m_rwLock);
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (!pActor || !pActor->IsLive()) continue;
        // IDA: Only send to players (m_eActorType == 0)
        if (pActor->GetActorType() != eActorUser) continue;
        if (eBroadCastType == E_BROADCAST_TYPE::eNoneSelf && pActor == pExceptActor) continue;
        pActor->BridgeSend(packet);
    }
}

// IDA 0x1408EF800 - SendBroadCastAfterLoading
// IDA 反编译精确还原: 加载后发送广播
void XArea::SendBroadCastAfterLoading(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) {
    CFAutoSlimReadLock lock(m_rwLock);
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (!pActor || !pActor->IsLive()) continue;
        // IDA: Only send to players (m_eActorType == 0)
        if (pActor->GetActorType() != eActorUser) continue;
        if (eBroadCastType == E_BROADCAST_TYPE::eNoneSelf && pActor == pExceptActor) continue;
        pActor->BridgeSend_AfterLoading(packet);
    }
}

// IDA 0x1408EF9A0 - SendInInfo
// IDA 反编译精确还原: 发送进入信息
void XArea::SendInInfo(XSendPacket& packet, XActor* pActor) {
    // IDA: pActor->SetInfoPacket() would be called here
    // Currently using simplified version

    CFAutoSlimReadLock lock(m_rwLock);
    for (auto& pair : m_mapActor) {
        XActor* pOtherActor = pair.second;
        if (pOtherActor && pOtherActor != pActor) {
            // IDA: Only send to players (m_eActorType == 0) who are live
            if (pOtherActor->GetActorType() == eActorUser && pOtherActor->IsLive()) {
                pOtherActor->BridgeSend(packet);
            }
        }
    }
}

// IDA 0x1408F0D50 - SendOutInfo
// IDA 反编译精确还原: 发送退出信息
void XArea::SendOutInfo(XSendPacket& xSendPacket, XActor* pActor) {
    // IDA 精确还原代码:
    // PS_OBJECT_REMOVE stObjectInfo;
    // memset(&stObjectInfo, 0, sizeof(stObjectInfo));
    // UXActorID uxActorID;
    // pActor->GetActorID(&uxActorID);
    // stObjectInfo.vecObjectID.push_back(uxActorID);
    // xSendPacket << stObjectInfo;
    // for (auto& [key, pOtherActor] : m_mapActor) {
    //     if (pOtherActor->m_eActorType == 0 && pOtherActor->IsLive() && pOtherActor != pActor) {
    //         pOtherActor->BridgeSend(xSendPacket);
    //     }
    // }

    if (!pActor) return;

    PS_OBJECT_REMOVE stObjectInfo;
    UXActorID uxActorID = pActor->GetActorID();
    stObjectInfo.vecObjectID.push_back(uxActorID);
    xSendPacket << stObjectInfo;

    CFAutoSlimReadLock lock(m_rwLock);
    for (auto& pair : m_mapActor) {
        XActor* pOtherActor = pair.second;
        if (pOtherActor && pOtherActor != pActor) {
            // IDA: Only send to players (m_eActorType == 0) who are live
            if (pOtherActor->GetActorType() == eActorUser && pOtherActor->IsLive()) {
                pOtherActor->BridgeSend(xSendPacket);
            }
        }
    }
}

// IDA 0x1408F06F0 - SendOtherInfos
// IDA 反编译精确还原: 发送其他Actor信息
void XArea::SendOtherInfos(XSendPacket& xSendPacket, XActor* pActor, int eActorType) {
    // IDA: Collect actors of specified type and send their info
    std::vector<XActor*> listActor;

    CFAutoSlimReadLock lock(m_rwLock);
    for (auto& pair : m_mapActor) {
        XActor* pOtherActor = pair.second;
        if (pOtherActor && pOtherActor->IsLive() && pOtherActor != pActor
            && static_cast<int>(pOtherActor->GetActorType()) == eActorType) {
            listActor.push_back(pOtherActor);
        }
    }

    // IDA: Write count as 2-byte value
    std::uint16_t shCount = static_cast<std::uint16_t>(listActor.size());
    xSendPacket << shCount;

    // IDA: Send each actor's info
    for (auto pOtherActor : listActor) {
        // IDA: pOtherActor->SendOutInfo(xSendPacket) would be called
        // Currently using simplified version
        (void)pOtherActor;
    }

    // IDA: pActor->BridgeSend(xSendPacket)
    if (pActor) {
        pActor->BridgeSend(xSendPacket);
    }
}

void XArea::OnUpdate(float fDelta) {
    // Base area update - no special logic needed
    (void)fDelta;
}

// IDA: Virtual ScanGridOrigin - base class stub (does nothing)
// Derived classes (XDistrict, XMaze) override this with actual implementations
void XArea::ScanGridOrigin(float dx, float dy, unsigned char byNation, int sectorRange, unsigned int dwOptions, std::vector<CMover*>& vecOut) {
    // Base class implementation - does nothing
    // Derived classes override this to perform actual scanning
    (void)dx;
    (void)dy;
    (void)byNation;
    (void)sectorRange;
    (void)dwOptions;
    (void)vecOut;
}

// IDA 0x1408EF080 - Static helper that dispatches to virtual ScanGridOrigin
// Mangled: ?ScanGridOrigin@XArea@@SA_NPEAVXActor@@HKAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@@Z
// Returns bool, takes XActor*, int, unsigned int, vector
bool XArea::ScanGridOrigin(XActor* pActor, int nRange, unsigned int uFlag, std::vector<CMover*>& vecOut) {
    // IDA: if ( !pActor || !pActor->GetArea(pActor) )
    if (!pActor || !pActor->GetArea()) {
        return false;
    }
    
    // IDA: v8 = pActor->GetArea(pActor);
    XArea* pArea = pActor->GetArea();
    
    // IDA: Extract position from actor
    STPosInfo* pPosInfo = pActor->GetPosInfo();
    if (!pPosInfo) {
        return false;
    }
    
    XVec3 vPos = pPosInfo->vPos;
    unsigned char byNation = pActor->GetNation();
    
    // IDA: Call virtual ScanGridOrigin on the area
    // Virtual function signature: (float dx, float dy, unsigned char byNation, int sectorRange, unsigned int dwOptions, std::vector<CMover*>& vecOut)
    pArea->ScanGridOrigin(vPos.x, vPos.y, byNation, nRange, uFlag, vecOut);
    
    return true;
}
