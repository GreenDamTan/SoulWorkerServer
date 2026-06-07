// InteractionObject.cpp
// CInteractionObject implementation
// IDA decompilation from GameServer.exe
//
// Functions:
// - CInteractionObject::CreateObject (0x140188140)
// - CInteractionObject::GetTypeId (0x1401881a0)
// - CInteractionObject::CInteractionObject (0x1401881b0)
// - CInteractionObject::~CInteractionObject (0x1401882f0)
// - CInteractionObject::BuildInfoPacket (0x1401885d0)
// - CInteractionObject::SetInfoPacket (0x140188680)
// - CInteractionObject::SendObjectInfo (0x140188720)

#include "InteractionObject.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <new>

// Forward declarations
class XGameServer;
class XResourceMgr;
class VType;

// Template for singleton access
template<typename T>
class TXSingleton {
public:
    static T* Instance();
};

// Static type info for RTTI
VType CInteractionObject::classCInteractionObject;

// ============================================================================
// CInteractionObject::CInteractionObject - Constructor
// IDA @ 0x1401881B0
// ============================================================================
CInteractionObject::CInteractionObject()
    : CMoverEx()
{
    // IDA code:
    // CInteractionObject *__fastcall CInteractionObject::CInteractionObject(CInteractionObject *this)
    // {
    //   CMoverEx::CMoverEx(this);
    //   this->...vftable = (CInteractionObject_vtbl *)&CInteractionObject::`vftable'{for `VisTypedEngineObject_cl'};
    //   this->...vftable = (VUserDataObj_vtbl *)&CInteractionObject::`vftable'{for `VUserDataObj'};
    //   this->...vftable = (VisModuleSystemParent_cl_vtbl *)&CInteractionObject::`vftable'{for `VisModuleSystemParent_cl'};
    //   this->...vftable = (VisModuleSystemChild_cl_vtbl *)&CInteractionObject::`vftable'{for `VisModuleSystemChild_cl'};
    //   this->...vftable = (VVisibilityData_vtbl *)&CInteractionObject::`vftable'{for `VVisibilityData'};
    //   this->...vftable = (VisElementManager_cl<VisBaseEntity_cl *>_vtbl *)&CInteractionObject::`vftable'{for `VisElementManager_cl<VisBaseEntity_cl *>'};
    //   this->...vftable = (XActor_vtbl *)&CInteractionObject::`vftable';
    //   STPosInfo::STPosInfo(&this->m_posInfo);
    //   this->m_pPosInfo = &this->m_posInfo;
    //   CSocialItemObject::InitComponant((CVaccumCube *)this);
    //   this->m_pInteractionInfo = nullptr;
    //   this->m_pTBInteraction = nullptr;
    //   return this;
    // }

    // Note: Base class CMoverEx constructor called
    // Virtual tables set for Vision Engine interfaces
    // Position info initialized
    // InitComponant called for components
    m_pInteractionInfo = nullptr;
    m_pTBInteraction = nullptr;
}

// ============================================================================
// CInteractionObject::~CInteractionObject - Destructor
// IDA @ 0x1401882F0
// ============================================================================
CInteractionObject::~CInteractionObject()
{
    // IDA code:
    // void __fastcall CInteractionObject::~CInteractionObject(CInteractionObject *this)
    // {
    //   this->...vftable = (CInteractionObject_vtbl *)&CInteractionObject::`vftable'{for `VisTypedEngineObject_cl'};
    //   this->...vftable = (VUserDataObj_vtbl *)&CInteractionObject::`vftable'{for `VUserDataObj'};
    //   this->...vftable = (VisModuleSystemParent_cl_vtbl *)&CInteractionObject::`vftable'{for `VisModuleSystemParent_cl'};
    //   this->...vftable = (VisModuleSystemChild_cl_vtbl *)&CInteractionObject::`vftable'{for `VisModuleSystemChild_cl'};
    //   this->...vftable = (VVisibilityData_vtbl *)&CInteractionObject::`vftable'{for `VVisibilityData'};
    //   this->...vftable = (VisElementManager_cl<VisBaseEntity_cl *>_vtbl *)&CInteractionObject::`vftable'{for `VisElementManager_cl<VisBaseEntity_cl *>'};
    //   this->...vftable = (XActor_vtbl *)&CInteractionObject::`vftable';
    //   CMoverEx::~CMoverEx(this);
    // }

    // Note: Virtual tables restored to CInteractionObject vtables
    // Base class CMoverEx destructor called
}

// ============================================================================
// CInteractionObject::CreateObject - Create new interaction object
// IDA @ 0x140188140
// ============================================================================
CInteractionObject* CInteractionObject::CreateObject()
{
    // IDA code:
    // CInteractionObject *__fastcall CInteractionObject::CreateObject()
    // {
    //   CInteractionObject *v1; // [rsp+28h] [rbp-20h]
    //
    //   v1 = (CInteractionObject *)VBaseObject::operator new(0xEC18u);
    //   if ( v1 )
    //     return CInteractionObject::CInteractionObject(v1);
    //   else
    //     return nullptr;
    // }

    // Allocate memory for the object (size 0xEC18 = 60440 bytes)
    void* pMemory = ::operator new(sizeof(CInteractionObject));
    if (pMemory)
    {
        return new (pMemory) CInteractionObject();
    }
    return nullptr;
}

// ============================================================================
// CInteractionObject::GetTypeId - Get type ID for RTTI
// IDA @ 0x1401881A0
// ============================================================================
VType* CInteractionObject::GetTypeId() const
{
    // IDA code:
    // VType *__fastcall CInteractionObject::GetTypeId(CInteractionObject *this)
    // {
    //   return &CInteractionObject::classCInteractionObject;
    // }
    return &classCInteractionObject;
}

// ============================================================================
// CInteractionObject::BuildInfoPacket - Build info packet
// IDA @ 0x1401885D0
// ============================================================================
bool CInteractionObject::BuildInfoPacket(ST_BATCH_INTERACTION* stInfo)
{
    // IDA code:
    // char __fastcall CInteractionObject::BuildInfoPacket(CInteractionObject *this, ST_BATCH_INTERACTION *stInfo)
    // {
    //   XGameServer *v2; // rax
    //   STInteractionBox *pInteraction; // [rsp+20h] [rbp-28h]
    //   VInterActionBoxInfo *pInteractionBox; // [rsp+30h] [rbp-18h]
    //
    //   pInteraction = this->m_pInteractionInfo;
    //   if ( !pInteraction )
    //     return 0;
    //   pInteractionBox = pInteraction->pInteractionBox;
    //   v2 = TXSingleton<XGameServer>::Instance();
    //   if ( !XResourceMgr::GetTB_INTERACTION_OBJECT(&v2->m_xResourceMgr, pInteractionBox->m_iInteractionID) )
    //     return 0;
    //   stInfo->bShow = pInteraction->bShow;
    //   stInfo->bEnable = pInteraction->bEnable;
    //   stInfo->nBoxIndex = pInteraction->nBoxIndex;
    //   stInfo->nCallCount = pInteraction->nCallCount;
    //   return 1;
    // }

    if (!m_pInteractionInfo)
        return false;

    // TODO: Verify interaction table exists
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // if (!XResourceMgr::GetTB_INTERACTION_OBJECT(&pServer->m_xResourceMgr, m_pInteractionInfo->pInteractionBox->m_iInteractionID))
    //     return false;

    stInfo->bShow = m_pInteractionInfo->bShow;
    stInfo->bEnable = m_pInteractionInfo->bEnable;
    stInfo->nBoxIndex = m_pInteractionInfo->nBoxIndex;
    stInfo->nCallCount = m_pInteractionInfo->nCallCount;

    return true;
}

// ============================================================================
// CInteractionObject::SetInfoPacket - Set info packet for network send
// IDA @ 0x140188680
// ============================================================================
void CInteractionObject::SetInfoPacket(XSendPacket& xSendPacket)
{
    // IDA code:
    // void __fastcall CInteractionObject::SetInfoPacket(CInteractionObject *this, XSendPacket *xSendPacket)
    // {
    //   ST_BATCH_INTERACTION stInfo; // [rsp+28h] [rbp-50h] BYREF
    //   ST_BATCH_INTERACTION_LIST lstInterAction; // [rsp+38h] [rbp-40h] BYREF
    //   __int64 v4; // [rsp+58h] [rbp-20h]
    //
    //   v4 = -2;
    //   PS_POST_DELETE_LIST::PS_POST_DELETE_LIST((ST_PRIVATE_SHOP_LIST *)&lstInterAction);
    //   if ( CInteractionObject::BuildInfoPacket((CInteractionObject *)((char *)this - 872), &stInfo) )
    //     std::vector<ST_DISTRICT_STATE>::push_back(
    //       (std::vector<ST_PARTY_RECRUIT_DEL> *)&lstInterAction,
    //       (const ST_PARTY_RECRUIT_DEL *)&stInfo);
    //   operator<<(xSendPacket, &lstInterAction);
    //   ST_DISTRICT_STATE_LIST::~ST_DISTRICT_STATE_LIST((PS_POST_LEVEL_UP_EVENT_INFO_VEC *)&lstInterAction);
    // }

    ST_BATCH_INTERACTION stInfo;
    ST_BATCH_INTERACTION_LIST lstInterAction;

    if (BuildInfoPacket(&stInfo))
    {
        lstInterAction.vecInfo.push_back(stInfo);
    }

    // TODO: Implement packet serialization
    // xSendPacket << lstInterAction;
}

// ============================================================================
// CInteractionObject::SendObjectInfo - Send object info to nearby players
// IDA @ 0x140188720
// ============================================================================
void CInteractionObject::SendObjectInfo()
{
    // IDA code:
    // void __fastcall CInteractionObject::SendObjectInfo(CInteractionObject *this, float a2, float a3)
    // {
    //   unsigned __int8 v3; // r9
    //   int v4; // [rsp+20h] [rbp-10068h]
    //   __int128 stInfo; // [rsp+28h] [rbp-10060h] OVERLAPPED BYREF
    //   XSendPacket xSendPacket; // [rsp+40h] [rbp-10048h] BYREF
    //   __int64 v7; // [rsp+10060h] [rbp-28h]
    //   XActor *pActor; // [rsp+10068h] [rbp-20h]
    //
    //   v7 = -2;
    //   LOWORD(stInfo) = *(_WORD *)&this->m_pInteractionInfo->bShow;
    //   DWORD1(stInfo) = this->m_pInteractionInfo->nBoxIndex;
    //   DWORD2(stInfo) = this->m_pInteractionInfo->nCallCount;
    //   XSendPacket::XSendPacket(&xSendPacket, 0x11u, 0x57u);
    //   operator<<(&xSendPacket, (ST_BATCH_INTERACTION *)&stInfo);
    //   if ( this )
    //     pActor = &this->XActor;
    //   else
    //     pActor = nullptr;
    //   CGocNetwork::BroadcastNearby(pActor, nullptr, &xSendPacket);
    // }

    if (!m_pInteractionInfo)
        return;

    // TODO: Implement actual packet sending
    // ST_BATCH_INTERACTION stInfo;
    // stInfo.bShow = m_pInteractionInfo->bShow;
    // stInfo.bEnable = m_pInteractionInfo->bEnable;
    // stInfo.nBoxIndex = m_pInteractionInfo->nBoxIndex;
    // stInfo.nCallCount = m_pInteractionInfo->nCallCount;
    //
    // XSendPacket xSendPacket(0x11, 0x57);  // Main=0x11, Sub=0x57
    // xSendPacket << stInfo;
    // CGocNetwork::BroadcastNearby(this, nullptr, &xSendPacket);
}

// ============================================================================
// CInteractionObject constructor
// ============================================================================
CInteractionObject::CInteractionObject()
    : m_pInteractionInfo(nullptr)
{
    // Initialize base class members (XActor, etc.)
    // Total size is 0xEC18 bytes due to inheritance chain
}
