// AkashicObjectMgr.h
// XAkashicObjectMgr - Akashic object manager
// IDA decompilation from GameServer.exe
//
// Functions:
// - XAkashicObjectMgr::Create (0x14019bd90)
// - TXObjectMgr<CAkashicObject>::TXObjectMgr (0x14019c0a0)
// - TXObjectMgr<CAkashicObject>::~TXObjectMgr (0x14019c680)
// - TXObjectMgr<CAkashicObject>::Init (0x14019c570)
// - TXAkashicObjectCreator<CAkashicObject>::Create (0x14019c100)

#pragma once

#include <cstdint>

// Forward declarations
class CAkashicObject;
class VType;
// 注意: UXMapID 在 PSCommon.h 中为 union (PDB LF_UNION 0xE4B7), 前向声明须用 union
union UXMapID;
struct XVec3;
struct TB_AKASHIC_RECORDS;

// ============================================================================
// TXAkashicObjectCreator - Template creator for CAkashicObject
// ============================================================================
template<typename T>
class TXAkashicObjectCreator {
public:
    // Constructor
    // IDA: ??0?$TXAkashicObjectCreator@VCAkashicObject@@@@QEAA@XZ @ 0x14019bd00
    TXAkashicObjectCreator();

    // Create - Create a new Akashic object
    // IDA: ?Create@?$TXAkashicObjectCreator@VCAkashicObject@@@@UEAAPEAVCAkashicObject@@XZ @ 0x14019c100
    virtual T* Create();

    // Create returning IXObject pointer
    // IDA: ?Create@?$TXAkashicObjectCreator@VCAkashicObject@@@@QEAAPEAVIXObject@@XZ @ 0x14019c340
    class IXObject* CreateAsObject();

protected:
    void* __vftable;
};

// ============================================================================
// TXAkashicObjectDeletor - Template deletor for CAkashicObject
// ============================================================================
template<typename T>
class TXAkashicObjectDeletor {
public:
    // Constructor
    // IDA: ??0?$TXAkashicObjectDeletor@VCAkashicObject@@@@QEAA@XZ @ 0x14019bd60
    TXAkashicObjectDeletor();
};

// ============================================================================
// TXObjectMgr<CAkashicObject> - Template specialization for CAkashicObject
// ============================================================================
template<>
class TXObjectMgr<CAkashicObject> {
public:
    // Constructor
    // IDA: ??0?$TXObjectMgr@VCAkashicObject@@@@QEAA@XZ @ 0x14019c0a0
    TXObjectMgr();

    // Destructor
    // IDA: ??1?$TXObjectMgr@VCAkashicObject@@@@UEAA@XZ @ 0x14019c680
    virtual ~TXObjectMgr();

    // Init - Initialize the object manager with max size
    // IDA: ?Init@?$TXObjectMgr@VCAkashicObject@@@@UEAA_NH@Z @ 0x14019c570
    bool Init(int nMaxSize);

    // Delete - Delete an object from the manager
    // IDA: ?Delete@?$TXObjectMgr@VCInteractionObject@@@@UEAAXPEAVIXObject@@@Z @ 0x14019c610
    void Delete(void* pObject);

protected:
    void* __vftable;
    int m_nMaxSize;
    // TXMap<__int64, CAkashicObject*> m_xObjectMap;
    // TXPool<IXObject> m_xPool;
};

// ============================================================================
// XAkashicObjectMgr - Akashic object manager
// Manages creation, deletion, and lookup of CAkashicObject instances
// ============================================================================
class XAkashicObjectMgr : public TXObjectMgr<CAkashicObject> {
public:
    // Constructor
    XAkashicObjectMgr();

    // Destructor
    virtual ~XAkashicObjectMgr();

    // Init - Initialize the object manager with max size
    // IDA: ?Init@XAkashicObjectMgr@@UEAA_NH@Z @ 0x14019BAA0
    bool Init(int nMaxSize);

    // Create - Create a new Akashic object at position
    // IDA: ?Create@XAkashicObjectMgr@@QEAAPEAVCAkashicObject@@TUXMapID@@HUXVec3@@MK@Z @ 0x14019BD90
    CAkashicObject* Create(UXMapID uxMapInsID, unsigned int nAkashicID, XVec3* vPos, float fRot, unsigned int dwParentID);

    // ClearAll - Clear all Akashic objects
    void ClearAll();
};
