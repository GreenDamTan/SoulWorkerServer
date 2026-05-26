#pragma once

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include <cstdint>
#include <map>

// 前置声明
class VActionResourceLump;
struct VAnimationInfo;
struct tagHIT_TRACE_BONE_NAME_DATA;
struct VCommonPositionBoxInfo;

// XActionResMgr - 动作资源管理器
// IDA 构造函数: 0x140003660
// IDA 析构函数: 0x140003770
// 继承自 VActionResourceManager
class XActionResMgr : public VActionResourceManager {
public:
    // 构造/析构
    XActionResMgr();
    virtual ~XActionResMgr();

    // 核心方法
    // IDA 0x140003810
    void LoadBaseAnimation(VActionResourceLump* pActionRes, bool bPlayer);

    // 动画注册
    void RegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, const VString& strAnimName, std::int16_t nType);

    // 访问器
    VActionResourceLump* GetActionResource() const { return m_pActionResource; }

private:
    // === IDA 确认的成员变量 ===
    // offset 8: m_mapHitCollisionInfo (std::map<VString, tagHIT_TRACE_BONE_NAME_DATA*>, 48 bytes)
    std::map<VString, tagHIT_TRACE_BONE_NAME_DATA*> m_mapHitCollisionInfo;

    // offset 56: m_mapTraceBoneName (std::map<VString, tagHIT_TRACE_BONE_NAME_DATA*>, 48 bytes)
    std::map<VString, tagHIT_TRACE_BONE_NAME_DATA*> m_mapTraceBoneName;

    // offset 104: m_mapAnimInfoKey (std::map<int, VCommonPositionBoxInfo*>, 48 bytes)
    std::map<std::int32_t, VCommonPositionBoxInfo*> m_mapAnimInfoKey;

    // offset 152: m_mapAnimInfoString (std::map<int, VCommonPositionBoxInfo*>, 48 bytes)
    std::map<std::int32_t, VCommonPositionBoxInfo*> m_mapAnimInfoString;

    // offset 200: m_mapSkillAttackTrigger (std::map<int, VCommonPositionBoxInfo*>, 48 bytes)
    std::map<std::int32_t, VCommonPositionBoxInfo*> m_mapSkillAttackTrigger;

    // offset 248: m_pActionResource (VActionResourceLump*, 8 bytes)
    VActionResourceLump* m_pActionResource;

    // Total size: 256 bytes (verified from IDA)
};
