#include "VEventObjectDefine.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"

#include <cstring>
#include <new>

VEventObjectInfo::VEventObjectInfo(const VEventObjectInfo& rhs)
    : iID(rhs.iID),
      iUniqueID(rhs.iUniqueID),
      eType(rhs.eType),
      PosTopLeft(rhs.PosTopLeft),
      PosBottomRight(rhs.PosBottomRight),
      Size(rhs.Size),
      fRotate(rhs.fRotate),
      Plane{rhs.Plane[0], rhs.Plane[1], rhs.Plane[2],
            rhs.Plane[3], rhs.Plane[4], rhs.Plane[5]},
      iLayerBitmask(rhs.iLayerBitmask) {}

VEventBoxInfo::VEventBoxInfo(const VEventBoxInfo& rhs)
    : VEventObjectInfo(rhs), eBoxType(rhs.eBoxType) {}

VEventPointInfo::VEventPointInfo(const VEventPointInfo& rhs)
    : VEventObjectInfo(rhs), ePointType(rhs.ePointType) {}

VCheckEventSpawnBoxInfo::VCheckEventSpawnBoxInfo(const VCheckEventSpawnBoxInfo& rhs)
    : VEventBoxInfo(rhs),
      m_eEvent_Type(rhs.m_eEvent_Type),
      m_fEvent_Rate(rhs.m_fEvent_Rate),
      m_fEvent_Delay_Time(rhs.m_fEvent_Delay_Time),
      m_iEvent_Operation_ID(rhs.m_iEvent_Operation_ID),
      m_fEvent_Time(rhs.m_fEvent_Time) {
    std::memcpy(m_iSpawn_Box_ID, rhs.m_iSpawn_Box_ID, sizeof(m_iSpawn_Box_ID));
}

// TODO: 需人工审查 - 两个折叠的 Load 调用基类占位实现，XMLHelper 尚未落地。
bool VEventBoxInfo::Load(TiXmlElement* pElement) {
    return VEventObjectInfo::Load(pElement) != 0;
}

// TODO: 需人工审查 - 两个折叠的 Load 调用基类占位实现，XMLHelper 尚未落地。
bool VEventPointInfo::Load(TiXmlElement* pElement) {
    return VEventObjectInfo::Load(pElement) != 0;
}

// 状态: STUB - Load (0x140769C90) 缺少 XMLHelper 与基类加载实现。
// TODO: 需人工审查 - 待按 IDA 顺序读取事件类型、概率、延时、操作 ID、时间和五个生成盒 ID。
// 依赖: VEventBoxInfo::Load、TinyXML、XMLHelper::Exchange_Enum/Float/Int。
bool VCheckEventSpawnBoxInfo::Load(TiXmlElement* pElement) {
    (void)pElement;
    return false;
}

VEventObjectInfo* VEventObjectInfo::Clone() {
    // TODO: 需人工审查 - VBaseObject::operator new 的 VBaseAlloc_rel 尚由标准分配替代。
    void* memory = VBaseObject::operator new(sizeof(VEventObjectInfo));
    if (memory)
        return ::new (memory) VEventObjectInfo(*this);
    return nullptr;
}

VEventBoxInfo* VEventBoxInfo::Clone() {
    // TODO: 需人工审查 - VBaseObject::operator new 的 VBaseAlloc_rel 尚由标准分配替代。
    void* memory = VBaseObject::operator new(sizeof(VEventBoxInfo));
    if (memory)
        return ::new (memory) VEventBoxInfo(*this);
    return nullptr;
}

VEventPointInfo* VEventPointInfo::Clone() {
    // TODO: 需人工审查 - VBaseObject::operator new 的 VBaseAlloc_rel 尚由标准分配替代。
    void* memory = VBaseObject::operator new(sizeof(VEventPointInfo));
    if (memory)
        return ::new (memory) VEventPointInfo(*this);
    return nullptr;
}

VCheckEventSpawnBoxInfo* VCheckEventSpawnBoxInfo::Clone() {
    // TODO: 需人工审查 - VBaseObject::operator new 的 VBaseAlloc_rel 尚由标准分配替代。
    void* memory = VBaseObject::operator new(sizeof(VCheckEventSpawnBoxInfo));
    if (memory)
        return ::new (memory) VCheckEventSpawnBoxInfo(*this);
    return nullptr;
}

VMonsterSpawnInfo* VMonsterSpawnInfo::Clone() {
    // TODO: 需人工审查 - VBaseObject::operator new 的 VBaseAlloc_rel 尚由标准分配替代。
    void* memory = VBaseObject::operator new(sizeof(VMonsterSpawnInfo));
    if (memory)
        return ::new (memory) VMonsterSpawnInfo(*this);
    return nullptr;
}

VMonsterSpawnInfo::VMonsterSpawnInfo(const VMonsterSpawnInfo& rhs)
    : VEventBoxInfo(rhs) {
    std::memcpy(m_stMonsterInfo, rhs.m_stMonsterInfo, sizeof(m_stMonsterInfo));
    m_iCreationPositionType = rhs.m_iCreationPositionType;
    m_iMoveType = rhs.m_iMoveType;
    m_iCreationCondition = rhs.m_iCreationCondition;
    m_fWaitCreationDelayTime = rhs.m_fWaitCreationDelayTime;
    m_fWaitCreationSequenceTime = rhs.m_fWaitCreationSequenceTime;
    m_iWaitCreationMaxWave = rhs.m_iWaitCreationMaxWave;
    m_iMaxEntityCount = rhs.m_iMaxEntityCount;
    m_iWaypoint = rhs.m_iWaypoint;
    m_iAggroGroupID = rhs.m_iAggroGroupID;
    m_iAggroDistance = rhs.m_iAggroDistance;
    m_iAggroMaxCount = rhs.m_iAggroMaxCount;
    std::memcpy(m_szObjectKey, rhs.m_szObjectKey, sizeof(m_szObjectKey));
    m_iSectorID = rhs.m_iSectorID;
    m_fTakeTargetRatio = rhs.m_fTakeTargetRatio;
    m_iScriptType = rhs.m_iScriptType;
    std::memcpy(m_iCheckScirptHP, rhs.m_iCheckScirptHP, sizeof(m_iCheckScirptHP));
    m_iWaitCreationSequenceType = rhs.m_iWaitCreationSequenceType;
    std::memcpy(m_ChangeSpawnAction, rhs.m_ChangeSpawnAction, sizeof(m_ChangeSpawnAction));
    m_ProtectionTarget = rhs.m_ProtectionTarget;
    m_RespawnTime = rhs.m_RespawnTime;
    m_iStep = rhs.m_iStep;
    m_eRespawnType = rhs.m_eRespawnType;
    m_iRespawnCondition = rhs.m_iRespawnCondition;
    std::memcpy(m_CreationEffectFile, rhs.m_CreationEffectFile, sizeof(m_CreationEffectFile));
    m_iGroupID = rhs.m_iGroupID;
}

void VEventObjectInfo::InitPlane() {
    // TODO: 需人工审查 - 旋转矩阵的外部 ConvertEulerToMat3_Rad 尚无当前工作根内的实现可对照。
    hkvMat3 rotMat;
    rotMat.setFromEulerAngles(0.0f, 0.0f, fRotate);
    const hkvVec3 vHalfSize = Size * 0.5f;
    const hkvVec3 vCenter = GetCenter();

    const hkvVec3 vDirX = rotMat.getAxis(0);
    const hkvVec3 vNegDirX = hkvVec3() - vDirX;
    Plane[0].setFromPointAndNormal(vCenter + vNegDirX * vHalfSize.x, vNegDirX);
    Plane[1].setFromPointAndNormal(vCenter + vDirX * vHalfSize.x, vDirX);

    const hkvVec3 vDirY = rotMat.getAxis(1);
    const hkvVec3 vNegDirY = hkvVec3() - vDirY;
    Plane[2].setFromPointAndNormal(vCenter + vNegDirY * vHalfSize.y, vNegDirY);
    Plane[3].setFromPointAndNormal(vCenter + vDirY * vHalfSize.y, vDirY);

    const hkvVec3 vDirZ = rotMat.getAxis(2);
    Plane[4].setFromPointAndNormal(vCenter + vDirZ * Size.z, vDirZ);
    Plane[5].setFromPointAndNormal(vCenter, hkvVec3() - vDirZ);
}

bool VEventObjectInfo::Collision(const hkvVec3& vPos) {
    hkvVec3 topLeft;
    hkvVec3 topRight;

    topLeft.x = PosTopLeft.x - Size.x / 2.0f;
    topLeft.y = PosTopLeft.y - Size.y / 2.0f;
    topLeft.z = PosTopLeft.z;
    topRight.x = PosTopLeft.x + Size.x / 2.0f;
    topRight.y = PosTopLeft.y + Size.y / 2.0f;
    topRight.z = PosTopLeft.z + Size.z;

    if (topLeft.x > vPos.x || vPos.x > topRight.x)
        return false;
    if (topLeft.y > vPos.y || vPos.y > topRight.y)
        return false;
    return topLeft.z <= vPos.z && vPos.z <= topRight.z;
}

bool VEventObjectInfo::IsIn(const hkvVec3& vTarget) {
    for (int iIndex = 0; iIndex < 4; ++iIndex) {
        if (Plane[iIndex].GetSide(vTarget))
            return false;
    }
    return true;
}
