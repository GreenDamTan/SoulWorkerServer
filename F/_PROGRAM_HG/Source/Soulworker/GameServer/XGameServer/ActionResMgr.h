#pragma once

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include <cstdint>
#include <map>
#include <set>

// 前置声明
class VActionResourceLump;
class VManagedResource;
struct VAnimationInfo;
struct tagHIT_COLLISION_DATA;
struct tagHIT_TRACE_BONE_NAME_DATA;
struct TB_CHARACTER_INFO;
struct TB_MONSTER;
struct TB_NPC;
struct TB_AKASHIC_RECORDS;

// XActionResMgr - 动作资源管理器
// IDA 构造函数: 0x140003660
// IDA 析构函数: 0x140003770
// IDA Clear: 0x1400099d0
// IDA LoadAll: 0x140008ef0
// 继承自 VActionResourceManager
class XActionResMgr : public VActionResourceManager {
public:
    // 构造/析构
    // IDA 0x140003660
    XActionResMgr();
    // IDA 0x140003770
    virtual ~XActionResMgr();

    // 核心方法
    // IDA 0x1400099d0 - 清理所有资源
    void Clear();

    // IDA 0x140008ef0 - 加载所有动作资源
    void LoadAll();

    // IDA 0x140003810 - 加载基础动画
    void LoadBaseAnimation(VActionResourceLump* pActionRes, bool bPlayer);

    // 动画注册
    void RegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, const VString& strAnimName, std::int16_t nType);

    // 动画加载辅助函数 (从 LoadAll 调用)
    void LoadCharacterAnimation(VActionResourceLump* pActionRes, TB_CHARACTER_INFO* pCharInfo);
    void LoadMonsterAnimation(VActionResourceLump* pActionRes, TB_MONSTER* pMobRef);
    void LoadNpcAnimation(VActionResourceLump* pActionRes, TB_NPC* pNpcRef);
    void LoadAkashicAnimation(VActionResourceLump* pActionRes, TB_AKASHIC_RECORDS* pTableRef);

    // XML 加载函数
    tagHIT_COLLISION_DATA* LoadHitCollisionFromXML(const char* szFilePath);
    tagHIT_TRACE_BONE_NAME_DATA* LoadTraceBoneNameFromXML(const char* szFilePath);

    // 资源加载 (继承自 VActionResourceManager)
    VManagedResource* Load(const char* szFilePath);
    void RemoveAllResourceLump();

    // 访问器
    VActionResourceLump* GetActionResource() const { return m_pActionResource; }
    VActionResourceLump* GetCommonSkillBoneRes() const { return m_pCommonSkillBoneRes; }

private:
    // === IDA 确认的成员变量 (从 Clear 和 LoadAll 反编译) ===

    // offset 8: m_mapHitCollisionInfo (std::map<VString, tagHIT_COLLISION_DATA*>, 48 bytes)
    // 存储 Hit Collision 数据，键为资源名称 (如 "SW_Erwin")
    std::map<VString, tagHIT_COLLISION_DATA*> m_mapHitCollisionInfo;

    // offset 56: m_mapTraceBoneName (std::map<VString, tagHIT_TRACE_BONE_NAME_DATA*>, 48 bytes)
    // 存储 Trace Bone Name 数据，键为资源名称
    std::map<VString, tagHIT_TRACE_BONE_NAME_DATA*> m_mapTraceBoneName;

    // offset 104: m_mapAnimInfoKey (std::map<int, std::map<VString, unsigned long>*>, 48 bytes)
    // 动画信息映射 (Key 版本)
    std::map<std::int32_t, std::map<VString, unsigned long>*> m_mapAnimInfoKey;

    // offset 152: m_mapAnimInfoString (std::map<int, std::map<unsigned long, VString>*>, 48 bytes)
    // 动画信息映射 (String 版本)
    std::map<std::int32_t, std::map<unsigned long, VString>*> m_mapAnimInfoString;

    // offset 200: m_mapSkillAttackTrigger (std::map<int, std::set<unsigned long>*>, 48 bytes)
    // 技能攻击触发器映射
    std::map<std::int32_t, std::set<unsigned long>*> m_mapSkillAttackTrigger;

    // offset 248: m_pActionResource (VActionResourceLump*, 8 bytes)
    // 当前动作资源块
    VActionResourceLump* m_pActionResource;

    // offset 256: m_pCommonSkillBoneRes (VActionResourceLump*, 8 bytes)
    // 通用技能骨骼资源 (从 LoadAll 最后加载)
    VActionResourceLump* m_pCommonSkillBoneRes;

    // Total size: 264 bytes (verified from IDA)
};
