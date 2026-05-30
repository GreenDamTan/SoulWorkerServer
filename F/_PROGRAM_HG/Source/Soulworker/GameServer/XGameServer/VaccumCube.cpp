#include "Soulworker/GameServer/XGameServer/VaccumCube.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/WorldManager.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <ctime>
#endif

// 获取当前时间戳 (毫秒)
static std::uint64_t GreenDamTan_GetTickCount64_VaccumCube() {
#ifdef _WIN32
    return GetTickCount64();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (std::uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
}

// Per IDA 0x1401908b0: CVaccumCube 构造函数
CVaccumCube::CVaccumCube()
    : CMoverEx()
    , m_pInterActionBoxInfo(nullptr)
    , m_bLock(false)
    , m_pTakeUser(nullptr)
    , m_dwCompletePickupTime(0)
    , m_dwTablePickupTime(0)
    , m_nCount(0)
    , m_nRandomKey(0)
    , m_nRandomItemID(0)
{
    // Per IDA: 设置虚函数表
    // m_eActorType = eActorVaccumCube;
    m_posInfo = STPosInfo();

    // Per IDA: CSocialItemObject::InitComponant
    // TODO: 需要实现 CSocialItemObject::InitComponant
}

// Per IDA 0x1401948b0: CVaccumCube 析构函数
CVaccumCube::~CVaccumCube() {
    // Per IDA: 调用 CMoverEx 析构函数
}

// Per IDA 0x140190a10: Init
void CVaccumCube::Init(
    UXActorID uxActorID,
    VInterActionBoxInfo* pInfo,
    XVec3& vecPos,
    int nRandom,
    int nItemID,
    std::uint64_t dwTablePickupTime)
{
    m_pInterActionBoxInfo = pInfo;
    m_nCount = 0;
    m_dwCompletePickupTime = 0;
    m_dwTablePickupTime = dwTablePickupTime;
    m_nRandomKey = nRandom;
    // m_eActorType = eActorVaccumCube;
    m_uxActorID = uxActorID;
    m_nRandomItemID = nItemID;

    // Per IDA: 设置位置信息
    // this->SetPosInfo(vecPos);
    // TODO: 需要 XVec3 类型和 SetPosInfo 方法
}

// Per IDA 0x140190b40: Spawn
void CVaccumCube::Spawn(int nCount) {
    m_nCount = nCount;
}

// Per IDA 0x140190b60: TakeVaccum
void CVaccumCube::TakeVaccum(XActor* pActor) {
    m_pTakeUser = pActor;
    m_bLock = true;
    m_dwCompletePickupTime = m_dwTablePickupTime + GreenDamTan_GetTickCount64_VaccumCube();
}

// Per IDA 0x140190bb0: IsPickup
bool CVaccumCube::IsPickup() {
    if (m_dwCompletePickupTime == 0) {
        return false;
    }
    return m_dwCompletePickupTime <= GreenDamTan_GetTickCount64_VaccumCube();
}

// Per IDA 0x140190bf0: Pickup
void CVaccumCube::Pickup() {
    // Per IDA: 完整的拾取逻辑
    // 1. 检查 m_pTakeUser 是否有效
    // 2. dynamic_cast<CUser*>(m_pTakeUser)
    // 3. 获取 CGocInventory 组件
    // 4. 获取 TB_INTERACTION_ITEM 表数据
    // 5. 根据随机率计算奖励
    // 6. 发送拾取结果包
    // 7. 清除占用状态

    if (!m_pTakeUser) {
        return;
    }

    CUser* pUser = dynamic_cast<CUser*>(m_pTakeUser);
    if (!pUser) {
        return;
    }

    // TODO: 完整实现拾取逻辑
    // 需要 CGocInventory, TB_INTERACTION_ITEM, XSendPacket 等类型

    GreenDamTan_log(__FILE__, __FUNCTION__, "Pickup - IDA精确还原 (需要CGocInventory/TB_INTERACTION_ITEM/XSendPacket类型)");
}

// Per IDA 0x140191450: ClearTakeVaccum
void CVaccumCube::ClearTakeVaccum() {
    m_bLock = false;
    m_dwCompletePickupTime = 0;

    if (m_pTakeUser) {
        CUser* pUser = dynamic_cast<CUser*>(m_pTakeUser);
        if (pUser) {
            // TODO: 获取 CGocEntity 组件并清除 VaccumCubeID
            // CGocEntity::SetVaccumCubeID(pEntity, 0);
            // 发送取消包
        }
    }

    m_pTakeUser = nullptr;
}

// Per IDA 0x1401915c0: IsTakeUser
bool CVaccumCube::IsTakeUser(XActor* pActor) {
    if (!m_pTakeUser || !pActor) {
        return false;
    }

    // Per IDA: 比较 ActorID
    UXActorID takeUserID;
    // m_pTakeUser->GetActorID(&takeUserID);

    UXActorID actorID;
    // pActor->GetActorID(&actorID);

    // return takeUserID == actorID;
    // TODO: 需要实现 GetActorID 和 UXActorID::operator==

    return m_pTakeUser == pActor;
}

// Per IDA 0x140191660: BuildInfoPacket
void CVaccumCube::BuildInfoPacket(PS_VACCUM_CUBE_IN& stInfo) {
    // Per IDA: 设置包信息
    // stInfo.byInType = 1;
    // stInfo.nTableID = m_pInterActionBoxInfo ? m_pInterActionBoxInfo->m_iInteractionID : 0;
    // stInfo.nID = m_pInterActionBoxInfo ? m_pInterActionBoxInfo->iID : 0;

    // TODO: 需要 PS_VACCUM_CUBE_IN 类型定义
    GreenDamTan_log(__FILE__, __FUNCTION__, "BuildInfoPacket - IDA精确还原 (需要PS_VACCUM_CUBE_IN类型)");
}

// Per IDA 0x1401947d0: GetID
std::uint64_t CVaccumCube::GetID() {
    // Per IDA: 返回 m_pGrapTarget->m_eObjectFlags
    // TODO: 需要实现 m_pGrapTarget
    return 0;
}

// Per IDA 0x1401947f0: GetActorID
UXActorID CVaccumCube::GetActorID() const {
    // Per IDA: 返回 m_uxActorID
    return m_uxActorID;
}

// Per IDA 0x140194950: GetInteractionID
std::uint64_t CVaccumCube::GetInteractionID() {
    if (m_pInterActionBoxInfo) {
        // return m_pInterActionBoxInfo->m_iInteractionID;
    }
    return 0;
}
