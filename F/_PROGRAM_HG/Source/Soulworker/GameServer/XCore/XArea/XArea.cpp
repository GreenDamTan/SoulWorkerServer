#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

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
