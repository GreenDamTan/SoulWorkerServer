// ChainLightningMgr.h
// ChainLightningMgr - Chain lightning effect manager
// IDA decompilation from GameServer.exe
//
// Functions:
// - ChainLightningMgr::Init (0x1401ae050)
// - ChainLightningMgr::Create (0x1401ae150)
// - TXObjectMgr<VChainLightningObject>::Init (0x1401ae510)

#pragma once

#include <cstdint>

// Forward declarations
class VChainLightningObject;
class VType;
struct XVec3;
template<typename T> class TXPool;
template<typename K, typename V> class TXMap;

// ============================================================================
// TXChainLightningCreator - Template creator for VChainLightningObject
// ============================================================================
template<typename T>
class TXChainLightningCreator {
public:
    // Constructor
    TXChainLightningCreator();

    // Create - Create a new chain lightning object
    virtual T* Create();

protected:
    void* __vftable;
};

// ============================================================================
// TXObjectMgr<VChainLightningObject> - Template specialization
// ============================================================================
template<>
class TXObjectMgr<VChainLightningObject> {
public:
    // Constructor
    TXObjectMgr();

    // Destructor
    virtual ~TXObjectMgr();

    // Init - Initialize the object manager with max size
    // IDA: ?Init@?$TXObjectMgr@VVChainLightningObject@@@@UEAA_NH@Z @ 0x1401ae510
    bool Init(int nMaxSize);

    // Create - Create a new object from the pool
    VChainLightningObject* Create();

    // Delete - Delete an object from the manager
    void Delete(void* pObject);

protected:
    void* __vftable;
    int m_nMaxSize;
    // TXMap<unsigned long, VChainLightningObject*> m_xObjectMap;
    // TXPool<VChainLightningObject> m_xPool;
};

// ============================================================================
// ChainLightningMgr - Chain lightning object manager
// Manages creation, deletion, and lookup of VChainLightningObject instances
// ============================================================================
class ChainLightningMgr : public TXObjectMgr<VChainLightningObject> {
public:
    // Constructor
    ChainLightningMgr();

    // Destructor
    virtual ~ChainLightningMgr();

    // Init - Initialize the chain lightning manager
    // IDA: ?Init@ChainLightningMgr@@UEAA_NH@Z @ 0x1401AE050
    bool Init(int nMaxSize);

    // Create - Create a chain lightning object at position
    // IDA: ?Create@ChainLightningMgr@@QEAAPEAVVChainLightningObject@@UXVec3@@@Z @ 0x1401AE150
    VChainLightningObject* Create(XVec3* vPos);

    // ClearAll - Clear all chain lightning objects
    void ClearAll();
};
