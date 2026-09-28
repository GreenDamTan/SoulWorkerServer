#pragma once

#include <cstddef>
#include "Soulworker/GameServer/XCore/VisionEngineTypes/hkvMat3.h"

class TiXmlElement;

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

enum eEventPointType {
    eEventPointType_Way = 0,
    eEventPointType_Escort = 1,
    eEventPointType_Count = 2,
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
    VEventObjectInfo();
    VEventObjectInfo(const VEventObjectInfo& rhs);

    // Virtual methods (PDB vtable 顺序: Clone@0, Load@8, InitPlane@16)
    // 状态: 部分还原 - Clone (0x1407640E0) 的分配/复制链已落地，底层 VBaseAlloc_rel 尚不可用。
    // TODO: 需人工审查 - VBaseObject::operator new 当前是标准分配替身，原版使用 Vision 分配器。
    virtual VEventObjectInfo* Clone();
    // 状态: STUB - Load (0x140763AC0) 尚未还原。
    // TODO: 需人工审查 - 依赖尚未落地的 TinyXML 与外部 XMLHelper：
    //   1. 依次读取对象 ID、唯一 ID、三个向量与旋转角；
    //   2. 依次尝试 29 层及 4 类枚举，生成层位掩码。
    virtual bool Load(TiXmlElement*) { return false; }
    // 状态: 部分还原 - InitPlane (0x140764340) 已恢复六平面写入顺序。
    // TODO: 需人工审查 - 依赖外部 ConvertEulerToMat3_Rad 的旋转矩阵结果；
    //   1. 核对 setFromEulerAngles(0, 0, fRotate) 与原始引擎的矩阵值；
    //   2. 核对旋转后六个平面的法线及有符号距离。
    virtual void InitPlane();

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

    bool Collision(const hkvVec3& vPos);
    bool IsIn(const hkvVec3& vTarget);

    // Members (from PDB)
    // 原始默认构造不写入标量字段，只构造三个向量和六个平面。
    int iID;                   // offset 8
    int iUniqueID;             // offset 12
    eEventObjectType eType;    // offset 16
    hkvVec3 PosTopLeft;        // offset 20
    hkvVec3 PosBottomRight;    // offset 32
    hkvVec3 Size;              // offset 44
    float fRotate;             // offset 56
    hkvPlane Plane[6];         // offset 60
    unsigned int iLayerBitmask;  // offset 156
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
    VEventBoxInfo();
    VEventBoxInfo(const VEventBoxInfo& rhs);
    // TODO: 需人工审查 - Clone (0x140764800) 的原版分配器 VBaseAlloc_rel 尚未落地。
    VEventBoxInfo* Clone() override;
    // TODO: 需人工审查 - Load (0x1407647D0) 转发已还原，基类 XML 加载仍为占位。
    bool Load(TiXmlElement* pElement) override;

    eEventBoxType eBoxType;    // offset 160
};
static_assert(sizeof(VEventBoxInfo) == 164, "VEventBoxInfo size must match PDB (164)");

struct VEventPointInfo : public VEventObjectInfo {
    VEventPointInfo();
    VEventPointInfo(const VEventPointInfo& rhs);
    // TODO: 需人工审查 - Clone (0x1407690E0) 的原版分配器 VBaseAlloc_rel 尚未落地。
    VEventPointInfo* Clone() override;
    // TODO: 需人工审查 - Load (0x1407647D0) 与 VEventBoxInfo::Load 折叠，基类 XML 加载仍为占位。
    bool Load(TiXmlElement* pElement) override;

    eEventPointType ePointType;
};
static_assert(sizeof(VEventPointInfo) == 164, "VEventPointInfo size must match PDB (164)");
static_assert(offsetof(VEventPointInfo, ePointType) == 160, "VEventPointInfo.ePointType offset mismatch");

struct VCheckEventSpawnBoxInfo : public VEventBoxInfo {
    VCheckEventSpawnBoxInfo();
    VCheckEventSpawnBoxInfo(const VCheckEventSpawnBoxInfo& rhs);
    // TODO: 需人工审查 - Clone (0x140769FC0) 的原版分配器 VBaseAlloc_rel 尚未落地。
    VCheckEventSpawnBoxInfo* Clone() override;
    // 状态: STUB - Load (0x140769C90) 的 XML 读取尚未还原。
    // TODO: 需人工审查 - 依赖基类 Load、TinyXML 与外部 XMLHelper：
    //   1. 读取 Equilibrium/Hidden 枚举与事件概率、延时、操作 ID、事件时间；
    //   2. 依次读取 m_iSpawn_Box_ID_0 至 m_iSpawn_Box_ID_4。
    bool Load(TiXmlElement* pElement) override;

    int m_eEvent_Type;
    float m_fEvent_Rate;
    float m_fEvent_Delay_Time;
    int m_iEvent_Operation_ID;
    float m_fEvent_Time;
    int m_iSpawn_Box_ID[5];
};
static_assert(sizeof(VCheckEventSpawnBoxInfo) == 204, "VCheckEventSpawnBoxInfo size must match PDB (204)");
static_assert(offsetof(VCheckEventSpawnBoxInfo, m_eEvent_Type) == 164, "VCheckEventSpawnBoxInfo.m_eEvent_Type offset mismatch");
static_assert(offsetof(VCheckEventSpawnBoxInfo, m_iSpawn_Box_ID) == 184, "VCheckEventSpawnBoxInfo.m_iSpawn_Box_ID offset mismatch");

// PDB UDT 0x72695, fieldlist 0x72694; original module XCommon.lib/VEventObjectDefine.obj.
struct VMonsterSpawnInfo : public VEventBoxInfo {
    struct VSpawnInfo {
        int m_iType;
        int m_iID;
        int m_iChance;
    };

    VMonsterSpawnInfo();
    VMonsterSpawnInfo(const VMonsterSpawnInfo& rhs);
    // TODO: 需人工审查 - Clone (0x1407660D0) 的原版分配器 VBaseAlloc_rel 尚未落地。
    VMonsterSpawnInfo* Clone() override;

    VSpawnInfo m_stMonsterInfo[10];
    int m_iCreationPositionType;
    int m_iMoveType;
    int m_iCreationCondition;
    float m_fWaitCreationDelayTime;
    float m_fWaitCreationSequenceTime;
    int m_iWaitCreationMaxWave;
    int m_iMaxEntityCount;
    int m_iWaypoint;
    int m_iAggroGroupID;
    int m_iAggroDistance;
    int m_iAggroMaxCount;
    char m_szObjectKey[128];
    int m_iSectorID;
    float m_fTakeTargetRatio;
    int m_iScriptType;
    int m_iCheckScirptHP[5];
    int m_iWaitCreationSequenceType;
    char m_ChangeSpawnAction[128];
    int m_ProtectionTarget;
    float m_RespawnTime;
    int m_iStep;
    int m_eRespawnType;
    int m_iRespawnCondition;
    char m_CreationEffectFile[128];
    int m_iGroupID;
    // 状态: STUB - Load (0x140764B00) 尚未还原。
    // TODO: 需人工审查 - 依赖 Vision XML 解析、基类 Load 与生成字段读取。
};
static_assert(sizeof(VMonsterSpawnInfo::VSpawnInfo) == 12, "VSpawnInfo size must match PDB (12)");
static_assert(offsetof(VMonsterSpawnInfo::VSpawnInfo, m_iType) == 0, "VSpawnInfo.m_iType offset mismatch");
static_assert(offsetof(VMonsterSpawnInfo::VSpawnInfo, m_iID) == 4, "VSpawnInfo.m_iID offset mismatch");
static_assert(sizeof(VMonsterSpawnInfo) == 772, "VMonsterSpawnInfo size must match PDB (772)");
static_assert(offsetof(VMonsterSpawnInfo, m_stMonsterInfo) == 164, "VMonsterSpawnInfo.m_stMonsterInfo offset mismatch");
static_assert(offsetof(VMonsterSpawnInfo, m_iCreationPositionType) == 284, "VMonsterSpawnInfo.m_iCreationPositionType offset mismatch");
static_assert(offsetof(VMonsterSpawnInfo, m_szObjectKey) == 328, "VMonsterSpawnInfo.m_szObjectKey offset mismatch");
static_assert(offsetof(VMonsterSpawnInfo, m_iSectorID) == 456, "VMonsterSpawnInfo.m_iSectorID offset mismatch");
static_assert(offsetof(VMonsterSpawnInfo, m_ChangeSpawnAction) == 492, "VMonsterSpawnInfo.m_ChangeSpawnAction offset mismatch");
static_assert(offsetof(VMonsterSpawnInfo, m_RespawnTime) == 624, "VMonsterSpawnInfo.m_RespawnTime offset mismatch");
static_assert(offsetof(VMonsterSpawnInfo, m_CreationEffectFile) == 640, "VMonsterSpawnInfo.m_CreationEffectFile offset mismatch");
static_assert(offsetof(VMonsterSpawnInfo, m_iGroupID) == 768, "VMonsterSpawnInfo.m_iGroupID offset mismatch");
#pragma pack(pop)
