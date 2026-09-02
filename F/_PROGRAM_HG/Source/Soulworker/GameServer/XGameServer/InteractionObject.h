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
// PDB UDT 0x1899B (cvdump types): 3 members, T_INT4
//   eEventObjectType_None = 0
//   eEventObjectType_Box = 1
//   eEventObjectType_Point = 2
// ============================================================================
enum eEventObjectType {
    eEventObjectType_None = 0,
    eEventObjectType_Box = 1,
    eEventObjectType_Point = 2,
};

// ============================================================================
// eEventBoxType - Event box type enumeration
// PDB UDT 0x1898B (cvdump types): 22 members, T_INT4
// ============================================================================
enum eEventBoxType {
    eEventBoxType_Start = 0,
    eEventBoxType_MonsterSpawn = 1,
    eEventBoxType_CheckMonsterSpawn = 2,
    eEventBoxType_OpenMaze = 3,
    eEventBoxType_CheckSceneDirecting = 4,
    eEventBoxType_Portal = 5,
    eEventBoxType_CommonPosition = 6,
    eEventBoxType_Sector = 7,
    eEventBoxType_CheckSector = 8,
    eEventBoxType_ServerGate = 9,
    eEventBoxType_MazeEscape = 10,
    eEventBoxType_LuaFunction = 11,
    eEventBoxType_InteractionObjectBox = 12,
    eEventBoxType_QuestMoveCheck = 13,
    eEventBoxType_CutScene = 14,
    eEventBoxType_CheckEventSpawn = 15,
    eEventBoxType_PortalExit = 16,
    eEventBoxType_PersonalShopArea = 17,
    eEventBoxType_SafeArea = 18,
    eEventBoxType_SectorStart = 19,
    eEventBoxType_SocialItemExclude = 20,
    eEventBoxType_Max = 21,
};

// ============================================================================
// VEventObjectInfo - Base event object info (160 bytes)
// PDB UDT 0x74D06, Size = 160, fieldlist 0x74D05 (cvdump types)
//   vfptr +0x00 (虚函数提供), iID +8, iUniqueID +12, eType +16,
//   PosTopLeft +20, PosBottomRight +32, Size +44, fRotate +56,
//   Plane(hkvPlane[6]) +60, iLayerBitmask +156
// PDB 方法集: GetCenter/IsIn/Collision (VANILLA 非虚),
//   Clone/Load/InitPlane (INTRODUCING VIRTUAL, vfptr 0/8/16),
//   GetEventUniqueID (STATIC), 构造x2, operator=
// 注意: PDB 无虚析构记录，禁止添加 virtual 析构以免改变 vtable 布局。
// 注意: PDB 三个 size (160/164/524/296) 均不按 8 圆整，证明原始工程对这些
//       Vision 类使用 pack(4)；用 pragma pack(4) 复现原始 MSVC 布局，
//       避免 clang-cl MSVC ABI 对 polymorphic 基类子对象圆整到 8 的偏移漂移。
// ============================================================================
#pragma pack(push, 4)
struct VEventObjectInfo {
    // Virtual methods (PDB vtable 顺序: Clone@0, Load@8, InitPlane@16)
    // TODO: 需人工审查 - Clone/Load/InitPlane 方法体依赖 Vision 资源子系统未还原
    virtual VEventObjectInfo* Clone() const { return nullptr; }
    virtual bool Load() { return false; }
    virtual void InitPlane() {}

    // Static methods
    // IDA: ?GetEventUniqueID@VEventObjectInfo@@SAHHH@Z (0x1401ADD50)
    static int GetEventUniqueID(int nID, int nLevelA) {
        return 100000 * (nLevelA + 1) + nID;
    }

    // IDA: ?GetCenter@VEventObjectInfo@@QEBA?AVhkvVec3@@XZ (0x140763A30)
    // PDB fieldlist 0x74D05 list[10] GetCenter (VANILLA 非虚), 返回 hkvVec3 by value
    // 精确还原: x/y 取 PosTopLeft/PosBottomRight 中点, z 取 PosTopLeft.z
    hkvVec3 GetCenter() const {
        hkvVec3 vResult;
        vResult.x = (PosTopLeft.x + PosBottomRight.x) * 0.5f;
        vResult.y = (PosTopLeft.y + PosBottomRight.y) * 0.5f;
        vResult.z = PosTopLeft.z;
        return vResult;
    }

    // Members (from PDB)
    int iID = 0;               // offset 8
    int iUniqueID = 0;         // offset 12
    eEventObjectType eType = eEventObjectType_None;  // offset 16
    hkvVec3 PosTopLeft{};      // offset 20
    hkvVec3 PosBottomRight{};  // offset 32
    hkvVec3 Size{};            // offset 44
    float fRotate = 0.0f;      // offset 56
    hkvPlane Plane[6] = {};    // offset 60
    unsigned int iLayerBitmask = 0;  // offset 156
};
static_assert(sizeof(VEventObjectInfo) == 160, "VEventObjectInfo size must match PDB (160)");
static_assert(offsetof(VEventObjectInfo, iID) == 8, "VEventObjectInfo.iID offset mismatch");
static_assert(offsetof(VEventObjectInfo, eType) == 16, "VEventObjectInfo.eType offset mismatch");
static_assert(offsetof(VEventObjectInfo, iLayerBitmask) == 156,
              "VEventObjectInfo.iLayerBitmask offset mismatch");

// ============================================================================
// VEventBoxInfo - Event box info (164 bytes)
// IDA: size 164, inherits VEventObjectInfo
// PDB UDT 0x74486: eBoxType (eEventBoxType) @ +160
// ============================================================================
struct VEventBoxInfo : public VEventObjectInfo {
    eEventBoxType eBoxType;    // offset 160
};

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
