#include "Soulworker/GameServer/XCore/XArea/XArea.h"
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
    // TODO: 从 IDA 还原 - 基类实现
}

void XArea::EnterActor(XActor* pActor) {
    // TODO: 从 IDA 还原
    // GreenDamTan_log(__FILE__, __FUNCTION__, "stub - pActor=%p", pActor);
}

void XArea::ExitActor(XActor* pActor) {
    // TODO: 从 IDA 还原
    // GreenDamTan_log(__FILE__, __FUNCTION__, "stub - pActor=%p", pActor);
}

XActor* XArea::FindActor(std::uint32_t dwActorID) {
    // TODO: 从 IDA 还原
    auto it = m_mapActor.find(dwActorID);
    if (it != m_mapActor.end()) {
        return it->second;
    }
    return nullptr;
}
