#include "Soulworker/GameServer/XCore/XArea/XDistrict.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

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
{
    GreenDamTan_log(__FILE__, __FUNCTION__, "XDistrict constructed");
}

XDistrict::~XDistrict() {
    GreenDamTan_log(__FILE__, __FUNCTION__, "XDistrict destructed");
}

void XDistrict::AddWaitForRecvInfo(CUser* pUser) {
    // TODO: 从 IDA 还原
    // GreenDamTan_log(__FILE__, __FUNCTION__, "stub - pUser=%p", pUser);
}

void XDistrict::RemoveWaitForRecvInfo(CUser* pUser) {
    // TODO: 从 IDA 还原
    // GreenDamTan_log(__FILE__, __FUNCTION__, "stub - pUser=%p", pUser);
}
