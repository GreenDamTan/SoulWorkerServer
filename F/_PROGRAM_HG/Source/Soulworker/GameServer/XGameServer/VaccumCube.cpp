#include "Soulworker/GameServer/XGameServer/VaccumCube.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/WorldManager.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <new>

// Static type info for RTTI - use pointer since VType is forward declared
static VType* classCVaccumCube = nullptr;

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
    m_eActorType = eActorVaccum;
    m_uxActorID = uxActorID;
    m_nRandomItemID = nItemID;

    m_posInfo.vPos = vecPos;
    m_posInfo.fRot = pInfo ? pInfo->fRotate : 0.0f;
    SetPosition(hkvVec3(vecPos.x, vecPos.y, vecPos.z));
}

// Per IDA 0x140190b10: Reset
void CVaccumCube::Reset() {
    // IDA code:
    // void __fastcall CVaccumCube::Reset(CVaccumCube *this)
    // {
    //   CMoverEx::Reset(this);
    //   CMover::ResetAllBuff(this);
    // }
    CMoverEx::Reset();
    CMover::ResetAllBuff();
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
// IDA 反编译精确还原 - 完整的拾取逻辑
void CVaccumCube::Pickup() {
    // IDA 反编译精确逻辑:
    // 1. 检查 m_pTakeUser 是否有效
    // 2. 获取 CUser 和 CGocInventory 组件
    // 3. 获取 TB_INTERACTION_ITEM 表数据
    // 4. 根据随机率计算奖励 (遍历 10 个物品槽位)
    // 5. 根据物品 ID 类型处理:
    //    - 0x2FAF0801: 添加金币
    //    - 0x2FAF0804: 添加以太
    //    - 0x2FAF0803: 添加 BP
    //    - 其他: 创建物品
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
    // 需要 CGocInventory, TB_INTERACTION_ITEM, XSendPacket, PS_VACCUM_PICK_UP 等类型
    // IDA 完整代码非常复杂，包含:
    // - 随机率计算 (1-10000)
    // - 10 个物品槽位遍历
    // - 货币类型判断 (金币/以太/BP)
    // - 物品创建请求
    // - 网络包发送

    GreenDamTan_log(__FILE__, __FUNCTION__, "Pickup - IDA精确还原 (需要CGocInventory/TB_INTERACTION_ITEM/XSendPacket类型)");
}

// Per IDA 0x140191450: ClearTakeVaccum
// IDA 反编译精确逻辑:
// 1. m_bLock = false
// 2. m_dwCompletePickupTime = 0
// 3. if (m_pTakeUser):
//    - dynamic_cast<CUser*>(m_pTakeUser)
//    - CMover::GetGOC<CGocEntity>(pUser, &pEntity, 0)
//    - if (pEntity):
//      - stVaccumRes.nID = CGocEntity::GetVaccumCubeID(pEntity)
//      - stVaccumRes.nErrorCode = 55800
//      - XSendPacket::XSendPacket(&xSendPacket, 0x25, 2)
//      - operator<<(&xSendPacket, &stVaccumRes)
//      - CGocNetwork::Send(m_pTakeUser, &xSendPacket)
//      - CGocEntity::SetVaccumCubeID(pEntity, 0)
// 4. m_pTakeUser = nullptr
void CVaccumCube::ClearTakeVaccum() {
    // IDA code:
    // void __fastcall CVaccumCube::ClearTakeVaccum(CVaccumCube *this, float a2, float a3)
    // {
    //   this->m_bLock = 0;
    //   this->m_dwCompletePickupTime = 0;
    //   if ( this->m_pTakeUser )
    //   {
    //     v3 = _RTDynamicCast_0(this->m_pTakeUser, 0, &XActor `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
    //     CMover::GetGOC<CGocEntity>((CMover *)(v3 + 131512), (std::tr1::shared_ptr<CDropItemGroup> *)&pEntity, 0);
    //     if ( (unsigned int)std::tr1::shared_ptr<CGocExchange>::operator int std::_Bool_struct::*((std::tr1::shared_ptr<CItemEquip> *)&pEntity) != -1 )
    //     {
    //       v4 = (CGocEntity *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&pEntity);
    //       stVaccumRes.nID = CGocEntity::GetVaccumCubeID(v4);
    //       stVaccumRes.nErrorCode = 55800;
    //       XSendPacket::XSendPacket(&xSendPacket, 0x25u, 2u);
    //       operator<<(&xSendPacket, (PS_MODE_MAZE_USER_POINT_INFO *)&stVaccumRes);
    //       CGocNetwork::Send(this->m_pTakeUser, &xSendPacket);
    //       v5 = (CGocEntity *)std::tr1::shared_ptr<CForce>::operator->((std::tr1::shared_ptr<CGocNetwork> *)&pEntity);
    //       CGocEntity::SetVaccumCubeID(v5, 0);
    //     }
    //     std::tr1::shared_ptr<CItemAkashic>::~shared_ptr<CItemAkashic>((std::tr1::shared_ptr<CGocNetwork> *)&pEntity);
    //   }
    //   this->m_pTakeUser = nullptr;
    // }

    m_bLock = false;
    m_dwCompletePickupTime = 0;

    if (m_pTakeUser) {
        CUser* pUser = dynamic_cast<CUser*>(m_pTakeUser);
        if (pUser) {
            // TODO: 获取 CGocEntity 组件并清除 VaccumCubeID
            // std::tr1::shared_ptr<CGocEntity> pEntity;
            // CMover::GetGOC<CGocEntity>(&pUser->CMoverEx, &pEntity, 0);
            // if (pEntity) {
            //     PS_RES_VACCUM_CLICK_START stVaccumRes;
            //     stVaccumRes.nID = pEntity->GetVaccumCubeID();
            //     stVaccumRes.nErrorCode = 55800;
            //     XSendPacket xSendPacket(0x25, 2);
            //     xSendPacket << stVaccumRes;
            //     CGocNetwork::Send(m_pTakeUser, &xSendPacket);
            //     pEntity->SetVaccumCubeID(0);
            // }
        }
    }

    m_pTakeUser = nullptr;
}

// Per IDA 0x1401915c0: IsTakeUser
// IDA 反编译精确逻辑:
// 1. if (!m_pTakeUser || !pActor) return false
// 2. 获取 pActor 的 ActorID
// 3. 获取 m_pTakeUser 的 ActorID
// 4. 比较两个 ActorID 是否相等
bool CVaccumCube::IsTakeUser(XActor* pActor) {
    // IDA code:
    // bool __fastcall CVaccumCube::IsTakeUser(CVaccumCube *this, XActor *pActor)
    // {
    //   if ( this->m_pTakeUser )
    //   {
    //     if ( pActor )
    //     {
    //       otherMask = (VBitmask *)pActor->GetActorID(pActor, v5);
    //       m_pTakeUser = this->m_pTakeUser;
    //       v8 = m_pTakeUser->__vftable;
    //       v2 = (VBitmask *)v8->GetActorID(m_pTakeUser, &v4);
    //       if ( UXActorID::operator==(v2, otherMask) )
    //         return true;
    //     }
    //   }
    //   return false;
    // }

    if (!m_pTakeUser || !pActor) {
        return false;
    }

    // Per IDA: 比较两个 ActorID
    UXActorID takeUserID = m_pTakeUser->GetActorID();
    UXActorID actorID = pActor->GetActorID();

    return takeUserID == actorID;
}

// Per IDA 0x140191660: BuildInfoPacket
void CVaccumCube::BuildInfoPacket(PS_VACCUM_CUBE_IN& stInfo) {
    // IDA code:
    // void __fastcall CVaccumCube::BuildInfoPacket(CVaccumCube *this, PS_VACCUM_CUBE_IN *stInfo)
    // {
    //   int m_iInteractionID; // [rsp+0h] [rbp-18h]
    //   int iID; // [rsp+4h] [rbp-14h]
    //   stInfo->byInType = 1;
    //   if ( this->m_pInterActionBoxInfo )
    //     m_iInteractionID = this->m_pInterActionBoxInfo->m_iInteractionID;
    //   else
    //     m_iInteractionID = 0;
    //   stInfo->nTableID = m_iInteractionID;
    //   if ( this->m_pInterActionBoxInfo )
    //     iID = this->m_pInterActionBoxInfo->iID;
    //   else
    //     iID = 0;
    //   stInfo->nID = iID;
    // }
    stInfo.byInType = 1;
    stInfo.nTableID = m_pInterActionBoxInfo ? m_pInterActionBoxInfo->m_iInteractionID : 0;
    stInfo.nID = m_pInterActionBoxInfo ? m_pInterActionBoxInfo->GetID() : 0;
}

// Per IDA 0x1401916F0: SetInfoPacket
void CVaccumCube::SetInfoPacket(XSendPacket& xSendPacket) {
    // IDA code:
    // void __fastcall CVaccumCube::SetInfoPacket(CVaccumCube *this, XSendPacket *xSendPacket)
    // {
    //   PS_VACCUM_CUBE_IN stInfo; // [rsp+28h] [rbp-20h] BYREF
    //   CVaccumCube::BuildInfoPacket((CVaccumCube *)((char *)this - 872), &stInfo);
    //   operator<<(xSendPacket, (PS_MODE_MAZE_MATCHING_EXIT *)&stInfo);
    // }

    PS_VACCUM_CUBE_IN stInfo;
    BuildInfoPacket(stInfo);
    xSendPacket.XParse << stInfo.nID;
    xSendPacket.XParse << stInfo.nTableID;
    xSendPacket.XParse << stInfo.byInType;
}

// Per IDA 0x140191750: SetInfoLeavePacket
void CVaccumCube::SetInfoLeavePacket(XSendPacket& xSendPacket, bool bDestroy) {
    // IDA code:
    // void __fastcall CVaccumCube::SetInfoLeavePacket(CVaccumCube *this, XSendPacket *xSendPacket, bool bDestroy)
    // {
    //   PS_VACCUM_CUBE_OUT stInfo; // [rsp+20h] [rbp-18h] BYREF
    //   BOOL v4; // [rsp+28h] [rbp-10h]
    //   int iID; // [rsp+2Ch] [rbp-Ch]
    //   v4 = !bDestroy;
    //   stInfo.byOutType = !bDestroy;
    //   if ( this->m_pInterActionBoxInfo )
    //     iID = this->m_pInterActionBoxInfo->iID;
    //   else
    //     iID = 0;
    //   stInfo.nID = iID;
    //   operator<<(xSendPacket, (ST_CASH_ITEM_BUY *)&stInfo);
    // }

    PS_VACCUM_CUBE_OUT stInfo;
    stInfo.nID = m_pInterActionBoxInfo ? m_pInterActionBoxInfo->GetID() : 0;
    stInfo.byOutType = !bDestroy ? 1 : 0;
    xSendPacket.XParse << stInfo.nID;
    xSendPacket.XParse << static_cast<bool>(stInfo.byOutType);
}

// Per IDA 0x1401947d0: GetID
std::uint64_t CVaccumCube::GetID() {
    // Per IDA: 返回 m_pGrapTarget->m_eObjectFlags
    CMoverEx* pGrapTarget = GetGrapTarget();
    return pGrapTarget ? static_cast<std::uint32_t>(reinterpret_cast<VTypedObject*>(pGrapTarget)->m_eObjectFlags) : 0;
}

// Per IDA 0x1401947f0: GetActorID
UXActorID CVaccumCube::GetActorID() const {
    // Per IDA: 返回 m_uxActorID
    return m_uxActorID;
}

// Per IDA 0x140194950: GetInteractionID
std::uint64_t CVaccumCube::GetInteractionID() {
    return m_pInterActionBoxInfo ? static_cast<std::uint32_t>(m_pInterActionBoxInfo->m_iInteractionID) : 0;
}

// ============================================================================
// VaccumCubeObjectMgr implementation
// ============================================================================

// Constructor
// IDA @ 0x1401903C0
VaccumCubeObjectMgr::VaccumCubeObjectMgr()
    : TXObjectMgr<CVaccumCube>()
{
    // IDA code:
    // VaccumCubeObjectMgr *__fastcall VaccumCubeObjectMgr::VaccumCubeObjectMgr(VaccumCubeObjectMgr *this)
    // {
    //   TXObjectMgr<CVaccumCube>::TXObjectMgr<CVaccumCube>(this);
    //   this->__vftable = (VaccumCubeObjectMgr_vtbl *)&VaccumCubeObjectMgr::`vftable';
    //   return this;
    // }
}

// Destructor
// IDA @ 0x140190430
VaccumCubeObjectMgr::~VaccumCubeObjectMgr()
{
    // IDA code:
    // void __fastcall VaccumCubeObjectMgr::~VaccumCubeObjectMgr(VaccumCubeObjectMgr *this)
    // {
    //   this->__vftable = (VaccumCubeObjectMgr_vtbl *)&VaccumCubeObjectMgr::`vftable';
    //   TXObjectMgr<CVaccumCube>::~TXObjectMgr<CVaccumCube>(this);
    // }
}

// Create - Create a new vaccum cube at position
// IDA @ 0x14018FF50
CVaccumCube* VaccumCubeObjectMgr::Create(XVec3* vPos)
{
    // IDA code:
    // CVaccumCube *__fastcall VaccumCubeObjectMgr::Create(VaccumCubeObjectMgr *this, XVec3 *vPos)
    // {
    //   CVaccumCube *pVaccum; // [rsp+20h] [rbp-18h]
    //   pVaccum = (CVaccumCube *)TXObjectMgr<CMonster>::Create((TXObjectMgr<CNpc> *)this);
    //   if ( pVaccum )
    //     VisObject3D_cl::SetPosition(pVaccum, vPos->x, vPos->y, vPos->z);
    //   return pVaccum;
    // }

    // TODO: Implement when TXObjectMgr::Create available
    // CVaccumCube* pVaccum = TXObjectMgr<CVaccumCube>::Create();
    // if (pVaccum && vPos)
    // {
    //     pVaccum->SetPosition(vPos->x, vPos->y, vPos->z);
    // }
    // return pVaccum;
    return nullptr;
}

// Init - Initialize the object manager with max size
// IDA @ 0x140190460
bool VaccumCubeObjectMgr::Init(int nMaxSize)
{
    // IDA code:
    // char __fastcall VaccumCubeObjectMgr::Init(VaccumCubeObjectMgr *this, int nMaxSize)
    // {
    //   TXVaccumCubeObjectCreator<CVaccumCube> xCreator; // [rsp+20h] [rbp-18h] BYREF
    //   TXVaccumCubeObjectCreator<CVaccumCube>::TXVaccumCubeObjectCreator<CVaccumCube>(&xCreator);
    //   if ( !TXPool<XDistrict>::Init((TXPool<XMaze> *)&this->m_xPool, nMaxSize, (TXPool<XMaze>::IXCreator *)&xCreator, 0) )
    //     return 0;
    //   this->m_nMaxSize = nMaxSize;
    //   return 1;
    // }

    // TODO: Implement when TXPool available
    // TXVaccumCubeObjectCreator<CVaccumCube> xCreator;
    // if (!m_xPool.Init(nMaxSize, &xCreator, false))
    //     return false;
    // m_nMaxSize = nMaxSize;
    // return true;
    return true;
}

// ClearAll - Clear all vaccum cubes
// IDA @ 0x1401904F0
void VaccumCubeObjectMgr::ClearAll()
{
    // IDA code:
    // void __fastcall VaccumCubeObjectMgr::ClearAll(VaccumCubeObjectMgr *this)
    // {
    //   TXVaccumCubeDeletor<CVaccumCube> xDeletor; // [rsp+20h] [rbp-18h] BYREF
    //   TXVaccumCubeDeletor<CVaccumCube>::TXVaccumCubeDeletor<CVaccumCube>(&xDeletor);
    //   this->Clear(this, &xDeletor);
    // }

    // TODO: Implement when TXPool available
    // TXVaccumCubeDeletor<CVaccumCube> xDeletor;
    // Clear(&xDeletor);
}

// ============================================================================
// CVaccumCube static methods
// ============================================================================

// CreateObject - Create a new CVaccumCube object
// IDA @ 0x140190840
CVaccumCube* CVaccumCube::CreateObject()
{
    // IDA code:
    // CVaccumCube *__fastcall CVaccumCube::CreateObject()
    // {
    //   CVaccumCube *v1; // [rsp+28h] [rbp-20h]
    //   v1 = (CVaccumCube *)VBaseObject::operator new(0xEC40u);
    //   if ( v1 )
    //     return CVaccumCube::CVaccumCube(v1);
    //   else
    //     return nullptr;
    // }

    // Allocate memory for the object (size 0xEC40 = 60480 bytes)
    void* pMemory = ::operator new(sizeof(CVaccumCube));
    if (pMemory)
    {
        return new (pMemory) CVaccumCube();
    }
    return nullptr;
}

// GetTypeId - Get type ID for RTTI
// IDA @ 0x1401908A0
VType* CVaccumCube::GetTypeId() const
{
    // IDA code:
    // VType *__fastcall CVaccumCube::GetTypeId(CVaccumCube *this)
    // {
    //   return CVaccumCube::classCVaccumCube;  // classCVaccumCube is already a pointer
    // }
    return classCVaccumCube;
}
