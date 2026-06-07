// TrapMgr.h
// TrapMgr - Trap object manager
// IDA decompilation from GameServer.exe
//
// Functions:
// - TrapMgr::Init (0x1406e2970)

#pragma once

#include <cstdint>

// Forward declarations
class VGameTrapObject;
template<typename T> class TXPool;
template<typename K, typename V> class TXMap;

// ============================================================================
// TXTrapCreator - Template creator for VGameTrapObject
// ============================================================================
template<typename T>
class TXTrapCreator {
public:
    TXTrapCreator();
    virtual T* Create();
protected:
    void* __vftable;
};

// ============================================================================
// TXObjectMgr<VGameTrapObject> - Template specialization
// ============================================================================
template<>
class TXObjectMgr<VGameTrapObject> {
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
// TrapMgr - Trap object manager
// Manages creation, deletion, and lookup of VGameTrapObject instances
// ============================================================================
class TrapMgr : public TXObjectMgr<VGameTrapObject> {
public:
    TrapMgr();
    virtual ~TrapMgr();

    // Init - Initialize the trap manager
    // IDA: ?Init@TrapMgr@@UEAA_NH@Z @ 0x1406E2970
    bool Init(int nMaxSize);

    void ClearAll();
};
