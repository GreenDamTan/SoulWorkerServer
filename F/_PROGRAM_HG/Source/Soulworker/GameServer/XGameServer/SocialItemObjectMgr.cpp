// SocialItemObjectMgr.cpp
// SocialItemObjectMgr implementation
// IDA decompilation from GameServer.exe

#include "SocialItemObjectMgr.h"
#include "SocialItemObject.h"

// ============================================================================
// TXSocialItemObjectCreator<CSocialItemObject> - Template creator implementation
// ============================================================================

// Constructor
// IDA @ 0x14018FF20
template<typename T>
TXSocialItemObjectCreator<T>::TXSocialItemObjectCreator()
{
    // IDA code:
    // TXSocialItemObjectCreator<CSocialItemObject> *__fastcall TXSocialItemObjectCreator<CSocialItemObject>::TXSocialItemObjectCreator<CSocialItemObject>(
    //   TXSocialItemObjectCreator<CSocialItemObject> *this)
    // {
    //   TXPool<IXObject>::IXCreator::IXCreator(this);
    //   this->__vftable = (TXSocialItemObjectCreator<CSocialItemObject>_vtbl *)&TXSocialItemObjectCreator<CSocialItemObject>::`vftable';
    //   return this;
    // }
    // Note: Base class constructor called
}

// Create - Create a new social item object
// IDA @ 0x140190130
template<typename T>
T* TXSocialItemObjectCreator<T>::Create()
{
    // IDA code:
    // CSocialItemObject *__fastcall TXSocialItemObjectCreator<CSocialItemObject>::Create(
    //   TXSocialItemObjectCreator<CSocialItemObject> *this)
    // {
    //   hkvVec3 vPos; // [rsp+30h] [rbp-18h] BYREF
    //   hkvVec3::hkvVec3((XVec3 *)&vPos);
    //   return (CSocialItemObject *)VisGame_cl::CreateEntity(
    //                                 (VisGame_cl *)Vision::Game.__vftable,
    //                                 "CSocialItemObject",
    //                                 &vPos,
    //                                 nullptr,
    //                                 nullptr);
    // }

    // TODO: Implement when Vision Engine available
    // return VisGame_cl::CreateEntity("CSocialItemObject", &vPos, nullptr, nullptr);
    return T::CreateObject();
}

// CreateAsObject - Create returning IXObject pointer
// IDA @ 0x140190370
template<typename T>
class IXObject* TXSocialItemObjectCreator<T>::CreateAsObject()
{
    // IDA code:
    // XActor *__fastcall TXSocialItemObjectCreator<CSocialItemObject>::Create(
    //   TXSocialItemObjectCreator<CSocialItemObject> *this)
    // {
    //   CSocialItemObject *v2; // [rsp+20h] [rbp-18h]
    //   v2 = TXSocialItemObjectCreator<CSocialItemObject>::Create(this);
    //   if ( v2 )
    //     return &v2->XActor;
    //   else
    //     return nullptr;
    // }

    T* pObject = Create();
    if (pObject)
    {
        // TODO: Return IXObject pointer when base class available
        // return static_cast<IXObject*>(pObject);
        return reinterpret_cast<class IXObject*>(pObject);
    }
    return nullptr;
}

// ============================================================================
// SocialItemObjectMgr implementation
// ============================================================================

// Constructor
// IDA @ 0x14018FE20
SocialItemObjectMgr::SocialItemObjectMgr()
    : TXObjectMgr<CSocialItemObject>()
{
    // IDA code:
    // SocialItemObjectMgr *__fastcall SocialItemObjectMgr::SocialItemObjectMgr(SocialItemObjectMgr *this)
    // {
    //   TXObjectMgr<CSocialItemObject>::TXObjectMgr<CSocialItemObject>(this);
    //   this->__vftable = (SocialItemObjectMgr_vtbl *)&SocialItemObjectMgr::`vftable';
    //   return this;
    // }
    // Note: Base class constructor called, vtable set
}

// Destructor
// IDA @ 0x14018FE90
SocialItemObjectMgr::~SocialItemObjectMgr()
{
    // IDA code:
    // void __fastcall SocialItemObjectMgr::~SocialItemObjectMgr(SocialItemObjectMgr *this)
    // {
    //   this->__vftable = (SocialItemObjectMgr_vtbl *)&SocialItemObjectMgr::`vftable';
    //   TXObjectMgr<CSocialItemObject>::~TXObjectMgr<CSocialItemObject>(this);
    // }
    // Note: vtable restored, base destructor called
}

// Init - Initialize the object manager with max size
// IDA @ 0x14018FEC0
bool SocialItemObjectMgr::Init(int nMaxSize)
{
    // IDA code:
    // char __fastcall SocialItemObjectMgr::Init(SocialItemObjectMgr *this, int nMaxSize)
    // {
    //   TXSocialItemObjectCreator<CSocialItemObject> xCreator; // [rsp+20h] [rbp-18h] BYREF
    //   TXSocialItemObjectCreator<CSocialItemObject>::TXSocialItemObjectCreator<CSocialItemObject>(&xCreator);
    //   if ( !TXPool<XDistrict>::Init((TXPool<XMaze> *)&this->m_xPool, nMaxSize, (TXPool<XMaze>::IXCreator *)&xCreator, 0) )
    //     return 0;
    //   this->m_nMaxSize = nMaxSize;
    //   return 1;
    // }

    // TODO: Implement when TXPool available
    // TXSocialItemObjectCreator<CSocialItemObject> xCreator;
    // if (!m_xPool.Init(nMaxSize, &xCreator, false))
    //     return false;
    m_nMaxSize = nMaxSize;
    return true;
}

// ============================================================================
// TXObjectMgr<CSocialItemObject> - Template specialization implementation
// ============================================================================

// Constructor
// IDA @ 0x140190060
TXObjectMgr<CSocialItemObject>::TXObjectMgr()
    : m_nMaxSize(0)
{
    // IDA code:
    // TXObjectMgr<CSocialItemObject> *__fastcall TXObjectMgr<CSocialItemObject>::TXObjectMgr<CSocialItemObject>(
    //   TXObjectMgr<CSocialItemObject> *this)
    // {
    //   IXObjectMgr::IXObjectMgr(this);
    //   this->__vftable = (TXObjectMgr<CSocialItemObject>_vtbl *)&TXObjectMgr<CSocialItemObject>::`vftable';
    //   TXMap<__int64,GameModeBase *,ATL::CElementTraits<__int64>>::TXMap<__int64,GameModeBase *,ATL::CElementTraits<__int64>>((TXMap<__int64,GameModeBase *,ATL::CElementTraits<__int64> > *)&this->m_xObjectMap);
    //   TXPool<IXObject>::TXPool<IXObject>(&this->m_xPool);
    //   return this;
    // }
    // Note: Base class and member initialization
}

// Destructor
// IDA @ 0x1401900C0
TXObjectMgr<CSocialItemObject>::~TXObjectMgr()
{
    // IDA code:
    // void __fastcall TXObjectMgr<CSocialItemObject>::~TXObjectMgr<CSocialItemObject>(TXObjectMgr<CSocialItemObject> *this)
    // {
    //   this->__vftable = (TXObjectMgr<CSocialItemObject>_vtbl *)&TXObjectMgr<CSocialItemObject>::`vftable';
    //   if ( this->m_nMaxSize )
    //     TXObjectMgr<DefenceMode>::Clear((TXObjectMgr<VGameTrapObject> *)this, nullptr);
    //   TXPool<IXObject>::~TXPool<IXObject>(&this->m_xPool);
    //   TXMap<__int64,GameModeBase *,ATL::CElementTraits<__int64>>::~TXMap<__int64,GameModeBase *,ATL::CElementTraits<__int64>>((TXMap<__int64,GameModeBase *,ATL::CElementTraits<__int64> > *)&this->m_xObjectMap);
    //   IXObjectMgr::~IXObjectMgr(this);
    // }
    if (m_nMaxSize)
    {
        // Clear(nullptr);
    }
    // TODO: Cleanup m_xPool and m_xObjectMap
}

// Init - Initialize the object manager with max size
// IDA @ 0x140190260
bool TXObjectMgr<CSocialItemObject>::Init(int nMaxSize)
{
    // IDA code:
    // char __fastcall TXObjectMgr<CSocialItemObject>::Init(TXObjectMgr<CSocialItemObject> *this, int nMaxSize)
    // {
    //   TXPool<IXObject>::TXCreator<CSocialItemObject> xCreator; // [rsp+20h] [rbp-18h] BYREF
    //   TXPool<IXObject>::TXCreator<CSocialItemObject>::TXCreator<CSocialItemObject>(&xCreator);
    //   if ( !TXPool<XDistrict>::Init((TXPool<XMaze> *)&this->m_xPool, nMaxSize, (TXPool<XMaze>::IXCreator *)&xCreator, 1) )
    //     return 0;
    //   this->m_nMaxSize = nMaxSize;
    //   return 1;
    // }

    // TODO: Implement when TXPool available
    // TXPool<IXObject>::TXCreator<CSocialItemObject> xCreator;
    // if (!m_xPool.Init(nMaxSize, &xCreator, true))
    //     return false;
    m_nMaxSize = nMaxSize;
    return true;
}

void TXObjectMgr<CSocialItemObject>::Delete(void* pObject)
{
    // TODO: Implement object deletion from pool
}

// Explicit template instantiation
template class TXSocialItemObjectCreator<CSocialItemObject>;
