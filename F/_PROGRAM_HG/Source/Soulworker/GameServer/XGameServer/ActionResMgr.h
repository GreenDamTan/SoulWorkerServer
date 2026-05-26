#pragma once

#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"  // 需要完整类型 CMover
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

// ActionTrigger - 动作触发器结构
// IDA: 从 RetrieveEvent 函数推断
// 用于存储动画中的事件触发器 (如攻击判定、特效触发等)
struct ActionTrigger {
    std::int16_t TypeOfTrigger;  // 触发器类型 (动作代码)
    // TODO: 其他字段需要从 IDA 还原

    ActionTrigger() : TypeOfTrigger(0) {}
};

// AttackJudgmentTrigger - 攻击判定触发器
class AttackJudgmentTrigger;

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
    // IDA 0x14000c250 - 注册动画信息
    bool RegisterAnimInfo(std::int16_t nMotionClass, std::int16_t nSubClass, const VString& strAnimName, bool bBattlePose);

    // 技能攻击触发器注册
    // IDA 未知地址 - 需进一步反编译
    void RegisterSkillAttackTrigger(VActionResourceLump* pActionRes, std::int32_t nCharacterID);

    // 动画加载辅助函数 (从 LoadAll 调用)
    // IDA 0x140004e60
    void LoadCharacterAnimation(VActionResourceLump* pActionRes, TB_CHARACTER_INFO* pCharInfo);
    // IDA 0x140007020
    void LoadMonsterAnimation(VActionResourceLump* pActionRes, TB_MONSTER* pMobRef);
    // IDA 0x140008aa0
    void LoadNpcAnimation(VActionResourceLump* pActionRes, TB_NPC* pNpcRef);
    // IDA 0x140008c70
    void LoadAkashicAnimation(VActionResourceLump* pActionRes, TB_AKASHIC_RECORDS* pTableRef);
    // IDA 0x140008cc0
    void LoadExtraAnimation();

    // XML 加载函数
    // IDA 0x14000bbf0
    tagHIT_COLLISION_DATA* LoadHitCollisionFromXML(const char* szFilePath);
    // IDA 0x14000bf70
    tagHIT_TRACE_BONE_NAME_DATA* LoadTraceBoneNameFromXML(const char* szFilePath);

    // 动作描述获取
    // IDA 0x14000a0c0 - 获取动画信息
    const VAnimationInfo* GetActionDesc(VActionResourceLump* pActionResource, const char* pszAniName);

    // 事件检索
    // IDA 0x14000a180 - 静态函数，检索动作触发器
    static ActionTrigger* RetrieveEvent(std::int16_t actionCode, int iActionIdx, const VAnimationInfo* pActionInfo);

    // 动画回调
    // IDA 0x14000a230 - 更改动作回调
    void ChangeMotionCallback(CMover* pMover, const VAnimationInfo* pInfo);

    // 动作目标到实体
    // IDA 0x14000a280 - 将动作数据应用到实体
    void ActionDestToEntity(CMover* pMover, const VAnimationInfo* pInfo);

    // 设置碰撞数据到 Actor
    // IDA 0x14000b9b0
    void SetHitCollisionDataToActor(const char* szCodeName, CMover* pMover);

    // 设置骨骼追踪数据到 Actor
    // IDA 0x14000bab0
    void SetTraceBoneNameDataToActor(const char* szCodeName, CMover* pMover);

    // 动画索引获取 (两个重载版本)
    // IDA 0x14000c170 - 通过表ID和动画名查询索引
    std::int32_t GetAnimIndex(std::int32_t dwTableID, const VString& strAnimName);
    // IDA 0x140368a30 - 通过动作类型计算索引 (静态函数)
    static std::int32_t GetAnimIndex(std::int16_t nMotionClass, std::int16_t nSubClass, bool bBattlePose);

    // 设置动画信息到 Actor
    // IDA 0x14000d360
    bool SetAnimInfoToActor(std::uint32_t dwTableID, CMover* pMover);

    // 检查触发器ID是否正确
    // IDA 0x14000d470
    bool IsCorrectTriggerID(std::uint32_t iSkillID, std::uint32_t iEventID);

    // 资源加载 (继承自 VActionResourceManager)
    VManagedResource* Load(const char* szFilePath);
    void RemoveAllResourceLump();

    // 访问器
    VActionResourceLump* GetActionResource() const { return m_pActionResource; }
    VActionResourceLump* GetCommonSkillBoneRes() const { return m_pCommonSkillBoneRes; }

private:
    // === IDA 确认的成员变量 (从 Clear 和 LoadAll 反编译) ===

    // offset 0x8 (继承自 VActionResourceManager 后的起始): m_dwTableID
    // 当前处理的表 ID (角色/怪物/NPC/Akashic 的 ID)
    // 从 LoadCharacterAnimation/LoadMonsterAnimation/LoadNpcAnimation/LoadAkashicAnimation 设置
    // LoadExtraAnimation 结束时重置为 -1
    std::int32_t m_dwTableID;

    // offset 0x10: m_mapHitCollisionInfo (std::map<VString, tagHIT_COLLISION_DATA*>, 48 bytes)
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
