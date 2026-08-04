/**
 * @file GocNetwork.cpp
 * @brief CGocNetwork - Network component implementation
 *
 * IDA Functions:
 * - ??0CGocNetwork@@QEAA@XZ (0x140103820) - Constructor
 * - ??1CGocNetwork@@UEAA@XZ (0x140103890) - Destructor
 * - ?Send@CGocNetwork@@SA_NPEAVXActor@@AEAVXSendPacket@@@Z (0x1401038C0)
 * - ?SendAfterLoading@CGocNetwork@@SA_NPEAVXActor@@AEAVXSendPacket@@@Z (0x140103910)
 * - ?Send@CGocNetwork@@SAXAEAV?$vector@PEAVCMover@@...@@@Z (0x140103960)
 * - ?Send@CGocNetwork@@SAXAEAV?$vector@PEAVCMover@@...@@PEAVXActor@@@Z (0x140103A40)
 * - ?SendAfterLoading@CGocNetwork@@SAXAEAV?$vector@PEAVCMover@@...@@PEAVXActor@@@Z (0x140103B30)
 * - ?BroadcastNearby@CGocNetwork@@SA_NPEAVXActor@@0AEAVXSendPacket@@@Z (0x140103C20)
 * - ?SendBroadCast@CGocNetwork@@SAXPEAVCMover@@AEAVXSendPacket@@W4E_BROADCAST_TYPE@IXArea@@@Z (0x140103CD0)
 * - ?SendBroadCastAfterLoading@CGocNetwork@@SAXPEAVCMover@@AEAVXSendPacket@@W4E_BROADCAST_TYPE@IXArea@@@Z (0x140103D60)
 * - ?SendErrorMessage@CGocNetwork@@SA_NPEAVCMover@@EEG@Z (0x140103DF0)
 */

#include "GocNetwork.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include <typeinfo>

// ============================================================================
// Constructor / Destructor
// ============================================================================

// IDA: ??0CGocNetwork@@QEAA@XZ (0x140103820)
// Decompiled:
//   GOComponent::GOComponent(this);
//   this->__vftable = (CGocNetwork_vtbl *)&CGocNetwork::`vftable';
//   return this;
CGocNetwork::CGocNetwork()
    : GOComponent()
{
    // VTable is set automatically by C++ virtual mechanism
}

// IDA: ??1CGocNetwork@@UEAA@XZ (0x140103890)
// Decompiled:
//   this->__vftable = (CGocNetwork_vtbl *)&CGocNetwork::`vftable';
//   GOComponent::~GOComponent(this);
CGocNetwork::~CGocNetwork()
{
    // VTable is set automatically by C++ virtual mechanism
    // Base class destructor called automatically
}

// ============================================================================
// GOComponent Interface
// ============================================================================

bool CGocNetwork::Initialize()
{
    return GOComponent::Initialize();
}

void CGocNetwork::Shutdown()
{
    GOComponent::Shutdown();
}

void CGocNetwork::Update(float fDeltaTime)
{
    GOComponent::Update(fDeltaTime);
}

// ============================================================================
// Static Send Functions - Single Actor
// ============================================================================

// IDA: ?Send@CGocNetwork@@SA_NPEAVXActor@@AEAVXSendPacket@@@Z (0x1401038C0)
// Decompiled:
//   if ( !pActor || !XActor::CanSync(pActor) )
//     return 0;
//   pActor->BridgeSend(pActor, packet);
//   return 1;
bool CGocNetwork::Send(XActor* pActor, XSendPacket& packet)
{
    if (!pActor || !pActor->CanSync()) {
        return false;
    }
    pActor->BridgeSend(packet);
    return true;
}

// IDA: ?SendAfterLoading@CGocNetwork@@SA_NPEAVXActor@@AEAVXSendPacket@@@Z (0x140103910)
// Decompiled:
//   if ( !pActor || !XActor::CanSync(pActor) )
//     return 0;
//   pActor->BridgeSend_AfterLoading(pActor, packet);
//   return 1;
bool CGocNetwork::SendAfterLoading(XActor* pActor, XSendPacket& packet)
{
    if (!pActor || !pActor->CanSync()) {
        return false;
    }
    pActor->BridgeSend_AfterLoading(packet);
    return true;
}

// ============================================================================
// Static Send Functions - Vector of Movers
// ============================================================================

// IDA: ?Send@CGocNetwork@@SAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@AEAVXSendPacket@@@Z (0x140103960)
// Decompiled:
//   Iterate through gobList:
//     v6 = *(_QWORD *)iterator->szName;  // Get CMover pointer
//     if (v6)
//       v7 = (XActor *)(v6 + 872);  // CMover to XActor offset
//     else
//       v7 = nullptr;
//     pActor = v7;
//     if (v7 && XActor::CanSync(pActor))
//       pActor->BridgeSend(pActor, packet);
void CGocNetwork::Send(std::vector<CMover*>& gobList, XSendPacket& packet)
{
    if (gobList.empty()) {
        return;
    }

    for (CMover* pMover : gobList) {
        if (!pMover) {
            continue;
        }
        // Note: In IDA, CMover contains XActor at offset 872
        // In our reconstruction, we need to get the XActor from CMover
        // Since CMover has XActor as a member at a specific offset,
        // we use reinterpret_cast with the offset
        // TODO: 汇编还原 - Verify the actual CMover->XActor relationship
        XActor* pActor = reinterpret_cast<XActor*>(reinterpret_cast<char*>(pMover) + 872);
        if (pActor && pActor->CanSync()) {
            pActor->BridgeSend(packet);
        }
    }
}

// IDA: ?Send@CGocNetwork@@SAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@AEAVXSendPacket@@PEAVXActor@@@Z (0x140103A40)
// Decompiled:
//   Same as above but with pExceptActor check:
//     if (pExceptActor != v8 && XActor::CanSync(pActor))
//       pActor->BridgeSend(pActor, packet);
void CGocNetwork::Send(std::vector<CMover*>& gobList, XSendPacket& packet, XActor* pExceptActor)
{
    if (gobList.empty()) {
        return;
    }

    for (CMover* pMover : gobList) {
        if (!pMover) {
            continue;
        }
        XActor* pActor = reinterpret_cast<XActor*>(reinterpret_cast<char*>(pMover) + 872);
        if (pActor != pExceptActor && pActor && pActor->CanSync()) {
            pActor->BridgeSend(packet);
        }
    }
}

// IDA: ?SendAfterLoading@CGocNetwork@@SAXAEAV?$vector@PEAVCMover@@V?$allocator@PEAVCMover@@@std@@@std@@AEAVXSendPacket@@PEAVXActor@@@Z (0x140103B30)
// Decompiled:
//   Same as above but calls BridgeSend_AfterLoading
void CGocNetwork::SendAfterLoading(std::vector<CMover*>& gobList, XSendPacket& packet, XActor* pExceptActor)
{
    if (gobList.empty()) {
        return;
    }

    for (CMover* pMover : gobList) {
        if (!pMover) {
            continue;
        }
        XActor* pActor = reinterpret_cast<XActor*>(reinterpret_cast<char*>(pMover) + 872);
        if (pActor != pExceptActor && pActor && pActor->CanSync()) {
            pActor->BridgeSend_AfterLoading(packet);
        }
    }
}

// ============================================================================
// Broadcast Functions
// ============================================================================

// IDA: ?BroadcastNearby@CGocNetwork@@SA_NPEAVXActor@@0AEAVXSendPacket@@@Z (0x140103C20)
// Decompiled:
//   if (!pActor->GetArea(pActor))
//     return 0;
//   std::vector<ST_DAILY_MISSION_INFO>::vector<ST_DAILY_MISSION_INFO>(&vecGameObjList);
//   XArea::ScanGridOrigin(pActor, 2, 1u, &vecGameObjList);
//   if (pExceptActor)
//     CGocNetwork::Send(&vecGameObjList, packet, pExceptActor);
//   else
//     CGocNetwork::Send(&vecGameObjList, packet);
//   v6 = 1;
//   std::vector<CFsmTransition *>::~vector<CFsmTransition *>(&vecGameObjList);
//   return v6;
bool CGocNetwork::BroadcastNearby(XActor* pActor, XActor* pExceptActor, XSendPacket& packet)
{
    if (!pActor) {
        return false;
    }

    XArea* pArea = pActor->GetArea();
    if (!pArea) {
        return false;
    }

    // Scan grid for nearby objects
    // IDA shows: XArea::ScanGridOrigin(pActor, 2, 1u, &vecGameObjList)
    // Parameter 2 = scan range/type, 1u = some flag
    std::vector<CMover*> vecGameObjList;
    pArea->ScanGridOrigin(pActor, 2, 1, vecGameObjList);

    // Send to all found objects
    if (pExceptActor) {
        Send(vecGameObjList, packet, pExceptActor);
    } else {
        Send(vecGameObjList, packet);
    }

    return true;
}

// IDA: ?SendBroadCast@CGocNetwork@@SAXPEAVCMover@@AEAVXSendPacket@@W4E_BROADCAST_TYPE@IXArea@@@Z (0x140103CD0)
// Decompiled:
//   pArea = pObject->GetArea(&pObject->XActor);
//   if (pArea) {
//     if (pObject)
//       pArea->SendBroadCast(pArea, packet, &pObject->XActor, (IXArea::E_BROADCAST_TYPE)eBroadCastType);
//     else
//       pArea->SendBroadCast(pArea, packet, nullptr, (IXArea::E_BROADCAST_TYPE)eBroadCastType);
//   }
void CGocNetwork::SendBroadCast(CMover* pObject, XSendPacket& packet, E_BROADCAST_TYPE eBroadCastType)
{
    if (!pObject) {
        return;
    }

    // Get XActor from CMover (offset 872 in IDA struct)
    // Note: In IDA, CMover contains XActor at offset 872
    XActor* pActor = reinterpret_cast<XActor*>(reinterpret_cast<char*>(pObject) + 872);
    XArea* pArea = pActor->GetArea();
    if (!pArea) {
        return;
    }

    pArea->SendBroadCast(packet, pActor, eBroadCastType);
}

// IDA: ?SendBroadCastAfterLoading@CGocNetwork@@SAXPEAVCMover@@AEAVXSendPacket@@W4E_BROADCAST_TYPE@IXArea@@@Z (0x140103D60)
// Decompiled:
//   Same as SendBroadCast but calls SendBroadCastAfterLoading
void CGocNetwork::SendBroadCastAfterLoading(CMover* pObject, XSendPacket& packet, E_BROADCAST_TYPE eBroadCastType)
{
    if (!pObject) {
        return;
    }

    XActor* pActor = reinterpret_cast<XActor*>(reinterpret_cast<char*>(pObject) + 872);
    XArea* pArea = pActor->GetArea();
    if (!pArea) {
        return;
    }

    pArea->SendBroadCastAfterLoading(packet, pActor, eBroadCastType);
}

// ============================================================================
// Error Message Functions
// ============================================================================

// IDA: ?SendErrorMessage@CGocNetwork@@SA_NPEAVCMover@@EEG@Z (0x140103DF0)
// Decompiled:
//   if (!pObject)
//     return 0;
//   v4 = (CUser *)_RTDynamicCast_0(pObject, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
//   CUser::SendErrorMessage(v4, ucMainCmd, ucSubCmd, xErrorCode);
//   return 1;
bool CGocNetwork::SendErrorMessage(CMover* pObject, std::uint8_t ucMainCmd, std::uint8_t ucSubCmd, std::uint16_t xErrorCode)
{
    if (!pObject) {
        return false;
    }

    // Dynamic cast from CMover to CUser (RTTI)
    // In the original code, this uses _RTDynamicCast to check if pObject is actually a CUser
    CUser* pUser = dynamic_cast<CUser*>(pObject);
    if (pUser) {
        pUser->SendErrorMessage(ucMainCmd, ucSubCmd, xErrorCode);
    }
    return true;
}