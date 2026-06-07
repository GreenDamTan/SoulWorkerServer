// ProjectileMgr.h
// ProjectileMgr - Projectile object manager
// IDA decompilation from GameServer.exe
//
// Functions:
// - ProjectileMgr::Init (0x140638d10)

#pragma once

#include <cstdint>

// Forward declarations
class VGameProjectileObject;
template<typename T> class TXPool;
template<typename K, typename V> class TXMap;

// ============================================================================
// TXProjectileCreator - Template creator for VGameProjectileObject
// ============================================================================
template<typename T>
class TXProjectileCreator {
public:
    TXProjectileCreator();
    virtual T* Create();
protected:
    void* __vftable;
};

// ============================================================================
// TXObjectMgr<VGameProjectileObject> - Template specialization
// ============================================================================
template<>
class TXObjectMgr<VGameProjectileObject> {
public:
    TXObjectMgr();
    virtual ~TXObjectMgr();
    bool Init(int nMaxSize);
    void Delete(void* pObject);
protected:
    void* __vftable;
    int m_nMaxSize;
};

// ============================================================================
// ProjectileMgr - Projectile object manager
// Manages creation, deletion, and lookup of VGameProjectileObject instances
// ============================================================================
class ProjectileMgr : public TXObjectMgr<VGameProjectileObject> {
public:
    ProjectileMgr();
    virtual ~ProjectileMgr();

    // Init - Initialize the projectile manager
    // IDA: ?Init@ProjectileMgr@@UEAA_NH@Z @ 0x140638D10
    bool Init(int nMaxSize);

    void ClearAll();
};
