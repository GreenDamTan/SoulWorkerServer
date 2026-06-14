// InteractionObject.h
// CInteractionObject - Interactive object class
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

#pragma once

#include <cstdint>
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"

// Forward declarations
class XActor;
class XSendPacket;
class VType;
struct ST_BATCH_INTERACTION;
struct ST_BATCH_INTERACTION_LIST;
struct STInteractionBox;
struct TB_INTERACTION_OBJECT;
class XResourceMgr;
class XGameServer;

// ============================================================================
// eEventObjectType - Event object type enumeration
// ============================================================================
enum eEventObjectType {
    EVENT_OBJECT_TYPE_UNKNOWN = 0,
    EVENT_OBJECT_TYPE_INTERACTION = 1,
    EVENT_OBJECT_TYPE_TRIGGER = 2,
};

// ============================================================================
// eEventBoxType - Event box type enumeration
// ============================================================================
enum eEventBoxType {
    EVENT_BOX_TYPE_UNKNOWN = 0,
    EVENT_BOX_TYPE_INTERACTION = 1,
    EVENT_BOX_TYPE_VACCUM = 2,
};

// ============================================================================
// VEventObjectInfo - Base event object info (160 bytes)
// IDA: size 160
// ============================================================================
struct VEventObjectInfo {
    virtual ~VEventObjectInfo() = default;

    // Virtual methods
    virtual int GetID() const { return iID; }
    virtual int GetUniqueID() const { return iUniqueID; }
    virtual eEventObjectType GetType() const { return eType; }

    // Static methods
    // IDA: ?GetEventUniqueID@VEventObjectInfo@@SAHHH@Z (0x1401ADD50)
    static int GetEventUniqueID(int nID, int nLevelA) {
        return 100000 * (nLevelA + 1) + nID;
    }

    // Members (from IDA)
    void* __vftable;           // offset 0
    int iID;                   // offset 8
    int iUniqueID;             // offset 12
    eEventObjectType eType;    // offset 16
    hkvVec3 PosTopLeft;        // offset 20
    hkvVec3 PosBottomRight;    // offset 32
    hkvVec3 Size;              // offset 44
    float fRotate;             // offset 56
    hkvPlane Plane[6];         // offset 60
    unsigned int iLayerBitmask; // offset 156
};

// ============================================================================
// VEventBoxInfo - Event box info (164 bytes)
// IDA: size 164, inherits VEventObjectInfo
// ============================================================================
struct VEventBoxInfo : public VEventObjectInfo {
    eEventBoxType eBoxType;    // offset 160
};

// ============================================================================
// VInterActionBoxInfo - Interaction box info (296 bytes)
// IDA: size 296, inherits VEventBoxInfo
// ============================================================================
struct VInterActionBoxInfo : public VEventBoxInfo {
    int m_iInteractionID;      // offset 164
    char m_szObjectKey[128];   // offset 168

    // Helper methods
    int GetInteractionID() const { return m_iInteractionID; }
    const char* GetObjectKey() const { return m_szObjectKey; }
};

// ============================================================================
// CInteractionObject - Interactive game object
// Objects that players can interact with (buttons, switches, etc.)
// ============================================================================
class CInteractionObject {
public:
    // === Static Factory ===

    // CreateObject - Create a new interaction object
    // IDA: ?CreateObject@CInteractionObject@@SAPEAVVTypedObject@@XZ @ 0x140188140
    static CInteractionObject* CreateObject();

    // === Constructor / Destructor ===

    // Constructor
    // IDA: ??0CInteractionObject@@QEAA@XZ @ 0x1401881B0
    CInteractionObject();

    // Destructor
    // IDA: ??1CInteractionObject@@UEAA@XZ @ 0x1401882F0
    virtual ~CInteractionObject();

    // === Virtual Methods ===

    // GetTypeId - Get type ID for RTTI
    // IDA: ?GetTypeId@CInteractionObject@@UEBAPEAUVType@@XZ @ 0x1401881A0
    virtual VType* GetTypeId() const;

    // === Info Methods ===

    // BuildInfoPacket - Build info packet for network transmission
    // IDA: ?BuildInfoPacket@CInteractionObject@@QEAA_NAEAUST_BATCH_INTERACTION@@@Z @ 0x1401885D0
    bool BuildInfoPacket(ST_BATCH_INTERACTION* stInfo);

    // SetInfoPacket - Set info packet for network send
    // IDA: ?SetInfoPacket@CInteractionObject@@UEAAXAEAVXSendPacket@@@Z @ 0x140188680
    virtual void SetInfoPacket(XSendPacket& xSendPacket);

    // SendObjectInfo - Send object info to nearby players
    // IDA: ?SendObjectInfo@CInteractionObject@@QEAAXXZ @ 0x140188720
    void SendObjectInfo();

    // === Accessors ===
    STInteractionBox* GetInteractionInfo() const { return m_pInteractionInfo; }
    void SetInteractionInfo(STInteractionBox* pInfo) { m_pInteractionInfo = pInfo; }
    TB_INTERACTION_OBJECT* GetTBInteraction() const { return m_pTBInteraction; }
    void SetTBInteraction(TB_INTERACTION_OBJECT* pTB) { m_pTBInteraction = pTB; }

protected:
    // Member variables
    // Note: CInteractionObject inherits from XActor, size 0xEC18 bytes
    STInteractionBox* m_pInteractionInfo;      // Interaction info pointer
    TB_INTERACTION_OBJECT* m_pTBInteraction;   // Interaction table pointer

    // Additional padding to reach 0xEC18 bytes
    // The actual structure is much larger due to inheritance

private:
    // Static type info for RTTI
    static VType classCInteractionObject;
};

// ============================================================================
// ST_BATCH_INTERACTION - Batch interaction packet structure
// ============================================================================
struct ST_BATCH_INTERACTION {
    bool bShow;          // Show flag
    bool bEnable;        // Enable flag
    int nBoxIndex;       // Box index
    int nCallCount;      // Call count
};

// STInteractionBox 定义在 BattleZone.h 中，此处使用前向声明
struct STInteractionBox;
