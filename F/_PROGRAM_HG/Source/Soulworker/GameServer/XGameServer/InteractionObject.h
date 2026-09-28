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
#include "Soulworker/Common/XNet/XCommon/VEventObjectDefine.h"

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

#pragma pack(push, 4)

// PDB UDT 0x6A3F8: VEventBoxInfo base (164 bytes) and ten check-box IDs at +176.
struct VCheckMonsterSpawnInfo : public VEventBoxInfo {
    int m_iType;
    int m_iLoopCount;
    int m_iEntityID;
    int m_iCheckBox[10];
};
static_assert(sizeof(VCheckMonsterSpawnInfo) == 216,
              "VCheckMonsterSpawnInfo size must match PDB (216)");

// ============================================================================
// VEventObjectResource - Event object resource container
// PDB UDT 0x4A165, Size = 2320 (Vision 引擎 XCommon.lib, VEventObjectResourceManager.obj)
// 完整 2320B 布局属 Vision 子系统未还原范围，此处仅按 PDB 权威签名声明
// 本工程活跃链路所需的接口方法，不做布局断言。
// TODO: 需人工审查 - m_EventObjectList (VMap<int,void*>) 及其余 Vision 成员待子系统批次还原
// ============================================================================
class VEventObjectResource {
public:
    // IDA: ?SearchFromID@VEventObjectResource@@QEBAPEBUVEventObjectInfo@@H@Z (0x140760A80)
    // PDB publics RVA 0x75FA80; const 成员函数
    // IDA 逻辑: VMap<int,void*>::Lookup(&m_EventObjectList, nID, &pResult)
    //           命中返回 (const VEventObjectInfo*)pResult，否则 nullptr
    const VEventObjectInfo* SearchFromID(int nID) const {
        // TODO: 需人工审查 - 依赖 Vision VMap 子系统未还原，活跃层暂返回 nullptr，
        // 由 VEventObjectResource 子系统批次精确还原
        (void)nID;
        return nullptr;
    }
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
static_assert(sizeof(VInterActionBoxInfo) == 296, "VInterActionBoxInfo size must match PDB (296)");

// ============================================================================
// VCommonPositionBoxInfo - Common position box info (176 bytes)
// PDB UDT 0x77D8F, Size = 176, inherits VEventBoxInfo (164), fieldlist 0x77D8E
//   +164 m_eTarget, +168 m_iEntityID, +172 m_iGroup
// PDB 方法集: Clone/Load (VIRTUAL), 构造x2, operator=
// ============================================================================
struct VCommonPositionBoxInfo : public VEventBoxInfo {
    int m_eTarget = 0;      // +164
    int m_iEntityID = 0;    // +168
    int m_iGroup = 0;       // +172
};
static_assert(sizeof(VCommonPositionBoxInfo) == 176,
              "VCommonPositionBoxInfo size must match PDB (176)");
#pragma pack(pop)

// ============================================================================
// VSectorBoxInfo - PDB UDT 0x7426B, Size = 468, inherits VEventBoxInfo (164)
// 完整 468B 布局已在 cvdump types 0x7426A 核实 (m_iRelativeSectorID @ +444)，
// 但活跃 CSector 布局 (Sector.h) 尚未对齐 PDB 352B 布局，当前落地该完整
// 定义会与 Sector.h 既有的同名自造结构冲突。待 CSector 布局对齐批次统一还原。
// TODO: 需人工审查 - VSectorBoxInfo 完整布局待 CSector 对齐批次落地
// ============================================================================

// ============================================================================
// VPortalBoxInfo - Portal box info (524 bytes)
// PDB UDT 0x6BF4F, Size = 524, inherits VEventBoxInfo (164)
// PDB fieldlist 0x6BF4E (cvdump types): 30 fields + nested tag
// ============================================================================
struct VPortalBoxInfo : public VEventBoxInfo {
    int m_bShowGUI = 0;                    // +164 (PDB: T_INT4)
    int m_iGUI = 0;                        // +168
    int m_iJumpType = 0;                   // +172
    int m_iJumpMap = 0;                    // +176
    int m_iJump = 0;                       // +180
    int m_iEffectState = 0;                // +184
    char m_szDisableEffect[128] = {};      // +188 (PDB 0x4CE1, 128 字节)
    char m_szEnableEffect[128] = {};       // +316
    int m_iUIString = 0;                   // +444
    int m_iNextSectorID = 0;               // +448
    int m_bCallScript = 0;                 // +452
    std::uint32_t m_uiOpenEpisode = 0;     // +456
    std::uint32_t m_uiCompleteEpisode = 0; // +460
    hkvVec3 m_vUIStringOffset{};           // +464
    int m_iClearSectorID1 = 0;             // +476
    int m_iClearSectorID2 = 0;             // +480
    int m_iClearSectorID3 = 0;             // +484
    int m_iClearSectorID4 = 0;             // +488
    int m_iClearSectorID5 = 0;             // +492
    float m_fClearSectorChance1 = 0.0f;    // +496
    float m_fClearSectorChance2 = 0.0f;    // +500
    float m_fClearSectorChance3 = 0.0f;    // +504
    float m_fClearSectorChance4 = 0.0f;    // +508
    float m_fClearSectorChance5 = 0.0f;    // +512
    int m_iMaxUserCount = 0;                // +516
    int m_iMaxTimeCount = 0;                // +520
};
static_assert(sizeof(VPortalBoxInfo) == 524, "VPortalBoxInfo size must match PDB (524)");
static_assert(offsetof(VPortalBoxInfo, m_iNextSectorID) == 448,
              "VPortalBoxInfo.m_iNextSectorID offset mismatch");
static_assert(offsetof(VPortalBoxInfo, m_iMaxTimeCount) == 520,
              "VPortalBoxInfo.m_iMaxTimeCount offset mismatch");

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
