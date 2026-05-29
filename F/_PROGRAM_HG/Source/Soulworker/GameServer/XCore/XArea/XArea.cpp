#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"

XArea::XArea()
    : m_mapActor()
    , m_rwLock()
    , m_uxMapID()
    , m_nMaxUserCount(0)
    , m_listSpawnNPC()
    , m_mapWorldMode()
    , m_vecWorldModeList_dummy()
{
    GreenDamTan_log(__FILE__, __FUNCTION__, "XArea constructed");
}

XArea::~XArea() {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XArea destructed");
}

void XArea::OnUpdate(float fDelta) {
    // Base area update - no special logic needed
}

void XArea::EnterActor(XActor* pActor) {
    if (!pActor) return;

    CFAutoSlimWriteLock lock(m_rwLock);
    m_mapActor[pActor->GetOriginID()] = pActor;
}

void XArea::ExitActor(XActor* pActor) {
    if (!pActor) return;

    CFAutoSlimWriteLock lock(m_rwLock);
    m_mapActor.erase(pActor->GetOriginID());
}

XActor* XArea::FindActor(std::uint32_t dwActorID) {
    CFAutoSlimReadLock lock(m_rwLock);
    auto it = m_mapActor.find(dwActorID);
    if (it != m_mapActor.end()) {
        return it->second;
    }
    return nullptr;
}

// GetTBMapID is inline in XArea.h (IDA 0x1400492D0)

// ============================================================================
// Broadcast Functions (Stub implementations)
// ============================================================================

void XArea::SendBroadCast(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) {
    // TODO: 汇编还原 - XArea::SendBroadCast
    // IDA: Need to reverse engineer the actual broadcast implementation
    // This is a stub that sends to all actors in the area
    CFAutoSlimReadLock lock(m_rwLock);
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (pActor && pActor != pExceptActor && pActor->CanSync()) {
            pActor->BridgeSend(packet);
        }
    }
}

void XArea::SendBroadCastAfterLoading(XSendPacket& packet, XActor* pExceptActor, E_BROADCAST_TYPE eBroadCastType) {
    // TODO: 汇编还原 - XArea::SendBroadCastAfterLoading
    // IDA: Need to reverse engineer the actual broadcast implementation
    // This is a stub that sends to all actors in the area
    CFAutoSlimReadLock lock(m_rwLock);
    for (auto& pair : m_mapActor) {
        XActor* pActor = pair.second;
        if (pActor && pActor != pExceptActor && pActor->CanSync()) {
            pActor->BridgeSend_AfterLoading(packet);
        }
    }
}

void XArea::ScanGridOrigin(XActor* pActor, int nRange, unsigned int uFlag, std::vector<CMover*>& vecOut) {
    // TODO: 汇编还原 - XArea::ScanGridOrigin
    // IDA: Need to reverse engineer the actual grid scan implementation
    // This is a stub that returns all actors in the area
    (void)nRange;
    (void)uFlag;

    if (!pActor) return;

    CFAutoSlimReadLock lock(m_rwLock);
    for (auto& pair : m_mapActor) {
        XActor* pFoundActor = pair.second;
        if (pFoundActor && pFoundActor != pActor) {
            // Cast XActor to CMover using dynamic_cast
            // Note: In IDA, CMover contains XActor at offset 872
            // In our reconstruction, CMover is not directly derived from XActor
            // We use dynamic_cast for safe downcasting
            CMover* pMover = dynamic_cast<CMover*>(pFoundActor);
            if (pMover) {
                vecOut.push_back(pMover);
            }
        }
    }
}
