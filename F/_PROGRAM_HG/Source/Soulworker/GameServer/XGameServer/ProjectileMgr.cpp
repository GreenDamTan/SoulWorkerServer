// ProjectileMgr.cpp
// ProjectileMgr implementation
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - ProjectileMgr::Init (0x140638d10)

#include "ProjectileMgr.h"
#include "VGameProjectileObject.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <new>

// ============================================================================
// TXProjectileCreator<VGameProjectileObject> implementation
// ============================================================================

template<typename T>
TXProjectileCreator<T>::TXProjectileCreator()
{
}

template<typename T>
T* TXProjectileCreator<T>::Create()
{
    return T::CreateObject();
}

// ============================================================================
// TXObjectMgr<VGameProjectileObject> implementation
// ============================================================================

TXObjectMgr<VGameProjectileObject>::TXObjectMgr()
    : m_nMaxSize(0)
{
}

TXObjectMgr<VGameProjectileObject>::~TXObjectMgr()
{
}

bool TXObjectMgr<VGameProjectileObject>::Init(int nMaxSize)
{
    m_nMaxSize = nMaxSize;
    return true;
}

void TXObjectMgr<VGameProjectileObject>::Delete(void* pObject)
{
}

// ============================================================================
// ProjectileMgr implementation
// ============================================================================

ProjectileMgr::ProjectileMgr()
    : TXObjectMgr<VGameProjectileObject>()
{
}

ProjectileMgr::~ProjectileMgr()
{
}

// Init - Initialize the projectile manager
// IDA @ 0x140638D10
bool ProjectileMgr::Init(int nMaxSize)
{
    // IDA code:
    // char __fastcall ProjectileMgr::Init(ProjectileMgr *this, int nMaxSize)
    // {
    //   TXProjectileCreator<VGameProjectileObject> xCreator; // [rsp+20h] [rbp-18h] BYREF
    //   TXProjectileCreator<VGameProjectileObject>::TXProjectileCreator<VGameProjectileObject>(&xCreator);
    //   if ( !TXPool<XDistrict>::Init((TXPool<XMaze> *)&this->m_xPool, nMaxSize, (TXPool<XMaze>::IXCreator *)&xCreator, 0) )
    //     return 0;
    //   this->m_nMaxSize = nMaxSize;
    //   return 1;
    // }

    TXProjectileCreator<VGameProjectileObject> xCreator;
    // if (!m_xPool.Init(nMaxSize, &xCreator, false))
    //     return false;
    m_nMaxSize = nMaxSize;
    return true;
}

void ProjectileMgr::ClearAll()
{
}

// Explicit template instantiation
template class TXProjectileCreator<VGameProjectileObject>;
