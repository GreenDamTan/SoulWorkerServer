// SocialItemObjectMgr.h
// SocialItemObjectMgr - Social item object manager
// IDA decompilation from GameServer.exe
//
// Functions:
// - SocialItemObjectMgr::Delete (0x140190010)

#pragma once

#include <cstdint>

// Forward declarations
class CSocialItemObject;
template<typename T> class TXObjectMgr;

// ============================================================================
// TXSocialItemObjectCreator - Template creator for CSocialItemObject
// ============================================================================
template<typename T>
class TXSocialItemObjectCreator {
public:
    // Constructor
    // IDA: ??0?$TXSocialItemObjectCreator@VCSocialItemObject@@@@QEAA@XZ @ 0x14018FF20
    TXSocialItemObjectCreator();

    // Create - Create a new social item object
    // IDA: ?Create@?$TXSocialItemObjectCreator@VCSocialItemObject@@@@UEAAPEAVCSocialItemObject@@XZ @ 0x140190130
    T* Create();

    // Create returning IXObject pointer
    // IDA: ?Create@?$TXSocialItemObjectCreator@VCSocialItemObject@@@@QEAAPEAVIXObject@@XZ @ 0x140190370
    class IXObject* CreateAsObject();

protected:
    void* __vftable;
};

// ============================================================================
// TXSocialItemObjectDeletor - Template deletor for CSocialItemObject
// ============================================================================
template<typename T>
class TXSocialItemObjectDeletor {
public:
    // Constructor
    // IDA: ??0?$TXSocialItemObjectDeletor@VCSocialItemObject@@@@QEAA@XZ @ 0x14018FFE0
    TXSocialItemObjectDeletor();
};

// ============================================================================
// SocialItemObjectMgr - Social item object manager
// Manages creation, deletion, and lookup of CSocialItemObject instances
// ============================================================================
class SocialItemObjectMgr : public TXObjectMgr<CSocialItemObject> {
public:
    // Delete - Delete a social item object
    // IDA: ?Delete@SocialItemObjectMgr@@QEAAXPEAVCSocialItemObject@@@Z @ 0x140190010
    void Delete(CSocialItemObject* pObject);

    // ClearAll - Clear all social item objects
    // IDA: ?ClearAll@SocialItemObjectMgr@@QEAAXXZ @ 0x14018FFB0
    void ClearAll();
};

// ============================================================================
// TXObjectMgr<CSocialItemObject> - Template specialization for CSocialItemObject
// ============================================================================
template<>
class TXObjectMgr<CSocialItemObject> {
public:
    // Constructor
    // IDA: ??0?$TXObjectMgr@VCSocialItemObject@@@@QEAA@XZ @ 0x140190060
    TXObjectMgr();

    // Destructor
    // IDA: ??1?$TXObjectMgr@VCSocialItemObject@@@@UEAA@XZ @ 0x1401900C0
    virtual ~TXObjectMgr();

    // Init - Initialize the object manager with max size
    // IDA: ?Init@?$TXObjectMgr@VCSocialItemObject@@@@UEAA_NH@Z @ 0x140190260
    bool Init(int nMaxSize);

    // Delete - Delete an object from the manager
    void Delete(void* pObject);

protected:
    void* __vftable;
    int m_nMaxSize;
    // TXMap<__int64, CSocialItemObject*> m_xObjectMap;
    // TXPool<IXObject> m_xPool;
};
