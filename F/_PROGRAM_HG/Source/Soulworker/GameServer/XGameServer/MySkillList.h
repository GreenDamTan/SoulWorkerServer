#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <memory>
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"

// 前置声明
struct TB_SKILL;
struct TB_AKASHIC_RECORDS;
class XActor;
class CMover;
class CMoverEx;
class VGameProjectileObject;
class VGameTrapObject;
class VChainLightningObject;
class VActionResourceLump;
class SummonMonsterTrigger;
class RandomSummonTrigger;
class AttackJudgmentTrigger;

// tagSKILL_ACTION_DAMAGE - 技能伤害动作结构
#pragma pack(push, 1)
struct tagSKILL_ACTION_DAMAGE {
    std::uint8_t byReactionType;      // 反应类型
    std::uint8_t byHitPartsIndex;     // 命中部位索引
    char padding[18];                  // 其他字段 (总大小 20 字节)

    void Clear() {
        memset(this, 0, sizeof(tagSKILL_ACTION_DAMAGE));
    }
};
#pragma pack(pop)

// SChainHitInfo - 链式命中信息
struct SChainHitInfo {
    char data[32];  // 32 字节
};

// SSkillInfo - 技能信息结构
struct SSkillInfo {
    int nSkillID;
    char padding[28];  // 其他字段
};

// SRandomTrapEvent - 随机陷阱事件
struct SRandomTrapEvent;

// SRandomSummonEvent - 随机召唤事件
struct SRandomSummonEvent;

// SExplodeTrap - 爆炸陷阱
struct SExplodeTrap;

// VPList - 已在 VisionEngineTypes.h 中定义
// struct VPList { void* __vftable; };

// E_COOLTIME_TYPE 枚举 - 冷却时间类型
enum E_COOLTIME_TYPE {
    E_COOLTIME_SKILL = 0,       // 普通技能
    E_COOLTIME_AKASHIC = 1,     // Akashic技能
};

// ============================================================================
// CMySkillList - 技能列表管理类
// IDA 反编译来源: GameServer.exe
// ============================================================================
class CMySkillList {
public:
    CMySkillList();
    ~CMySkillList();

    // 初始化
    // IDA 0x1402B6460
    void Init(XActor* pActor);

    // === 技能使用 ===
    // UseSkill: IDA 0x1402B75E0
    // 使用技能，返回错误码 (0=成功)
    int UseSkill(TB_SKILL* pSkillTable, TB_SKILL* pChangedSkillTable = nullptr, float fSkillCost = -1.0f);

    // EndSkill: IDA 0x1402B79F0
    // 结束技能
    void EndSkill(TB_SKILL* pSkillTable);

    // IsCanUseSkill: IDA 0x1402B7B30
    // 检查是否可以使用技能
    int IsCanUseSkill(TB_SKILL* pSkillTable, TB_SKILL* pChangedSkillTable, float* fSkillCost, bool bExceptHaveCheck);

    // === 冷却时间管理 ===
    // SetSkillCooltime: IDA 0x1402C4AD0
    void SetSkillCooltime(TB_SKILL* pSkillTable);

    // GetCooltime: IDA 0x1402C4940
    // 获取剩余冷却时间 (秒)
    float GetCooltime(E_COOLTIME_TYPE eType, int nCooltimeGroup, std::uint16_t wGlobalCoolTime = 0, bool bCheckGlobalCool = true);

    // ReduceSkillCooltime: IDA 0x1402C5280
    // 减少所有技能的冷却时间 (fReduceRate 为百分比)
    void ReduceSkillCooltime(float fReduceRate);

    // ResetCoolTime: IDA 0x1402C4870
    // 重置指定类型的冷却时间
    void ResetCoolTime(E_COOLTIME_TYPE eType);

    // SetAkashicCooltime: IDA 0x1402C5060
    void SetAkashicCooltime(TB_AKASHIC_RECORDS* pAkashicTable);

    // === 技能消耗 ===
    // GetSkillCost: IDA 0x1402C54F0
    float GetSkillCost(TB_SKILL* pSkillTable);

    // === 技能组管理 ===
    // GetHaveSkillGroup: IDA 0x1402C53D0
    std::tr1::shared_ptr<class CSkill> GetHaveSkillGroup(int nSkillGroup);

    // === 攻击目标管理 ===
    // GetAttackTarget: IDA 0x14019B9B0
    std::uint32_t GetAttackTarget(int iIndex);

    // SetAttackTarget: IDA 0x1405FA3D0
    void SetAttackTarget(int iIndex, std::uint32_t dwVal);

    // AddSkillTarget: IDA 0x1402BE150
    void AddSkillTarget(std::uint8_t byIndex, std::uint32_t dwTargetID, std::uint8_t byReaction, std::uint8_t byHitPartsIndex);

    // ClearSkillTarget: IDA 0x1402BE1C0
    void ClearSkillTarget();

    // === 攻击伤害管理 ===
    // SetAttackDamage: IDA 0x1402C7DE0
    void SetAttackDamage(int iIndex, tagSKILL_ACTION_DAMAGE* stVal);

    // GetAttackDamage: IDA 0x1402C7E40
    tagSKILL_ACTION_DAMAGE GetAttackDamage(int iIndex);

    // ClearAttackDamage: IDA 0x1402C3270
    void ClearAttackDamage();

    // ClearAttackDamage: IDA 0x1402C3290
    void ClearAttackDamage(unsigned int iIndex);

    // === 基础伤害 ===
    // SetBaseDamage: IDA 0x1403A26A0
    void SetBaseDamage(int iIndex, int nVal);

    // GetBaseDamage: IDA 0x1403A2770
    int GetBaseDamage(int iIndex);

    // === 飞行速度 ===
    // SetAttackFlySpeed: IDA 0x1402C7350
    void SetAttackFlySpeed(int iIndex, float fFlySpeed);

    // GetAttackFlySpeed: IDA 0x140375140
    float GetAttackFlySpeed(int iIndex);

    // === 额外移动 ===
    // SetAttackExtraMove: IDA 0x1402C7380
    void SetAttackExtraMove(int iIndex, hkvVec3* vMove);

    // GetAttackExtraMove: IDA 0x140375160
    hkvVec3& GetAttackExtraMove(int iIndex);

    // === 投射物管理 ===
    // RemoveProjectile: IDA 0x1402C0710
    void RemoveProjectile(VGameProjectileObject* pProjectile);

    // === 陷阱管理 ===
    // AddTrap: IDA 0x14063FE30
    void AddTrap(VGameTrapObject* pTrap);

    // RemoveTrap: IDA 0x1402C0E80
    void RemoveTrap(VGameTrapObject* pTrap);

    // === 链式闪电管理 ===
    // RemoveChainLightning (Object): IDA 0x1402C0EF0
    void RemoveChainLightning(VChainLightningObject* pChainLightning);

    // RemoveChainLightning (MoverEx): IDA 0x1402C0F70
    void RemoveChainLightning(CMoverEx* pMoverEx);

    // CheckChainLightningTarget: IDA 0x1402C1040
    void CheckChainLightningTarget(CMoverEx* pMoverEx);

    // === 投射物索引 ===
    // CheckProjectileIndex: IDA 0x1402C5900
    bool CheckProjectileIndex(std::int16_t shTriggerIndex, int iSkillIndex, std::uint8_t bySkillAnimCount);

    // ProjectileIndexClear: IDA 0x140375300
    void ProjectileIndexClear();

    // === 召唤怪物管理 ===
    // SetExplodeSummon: IDA 0x1402C57E0
    void SetExplodeSummon(int iIndex, SummonMonsterTrigger* pTrigger);

    // GetExplodeSummon: IDA 0x1402C5890
    SummonMonsterTrigger* GetExplodeSummon(int iIndex);

    // ExplodeSummonClear: IDA 0x1403A2D30
    void ExplodeSummonClear();

    // ClearRandomSummon: IDA 0x1406CE540
    void ClearRandomSummon();

    // === 连续近战检查 ===
    // IsCheckContinuousMelee: IDA 0x1403645F0
    bool IsCheckContinuousMelee() const;

    // SetCheckContinuousMelee: IDA 0x1403A2150
    void SetCheckContinuousMelee(bool bCheck);

    // === 延迟投射物检查 ===
    // SetCheckDelayedProjectile: IDA 0x1403A2170
    void SetCheckDelayedProjectile(bool bCheck);

    // === 动作资源 ===
    // GetProjPathActionRes: IDA 0x1402C7710
    VActionResourceLump* GetProjPathActionRes();

    // === 测试模式 ===
    // SetTestMode: IDA 0x140407090
    void SetTestMode(int bTestMode);

    // === 思考函数 ===
    // ThinkFunction: IDA 0x1402B6500
    void ThinkFunction();

    // === 技能管理 (Phase 6 新增) ===
    // CancelSkill - 取消当前技能
    void CancelSkill();

    // SetCooltime - 设置技能冷却时间
    void SetCooltime(int nSkillGroup, float fCooltimeSec);

    // CheckSkillCondition - 检查技能使用条件
    bool CheckSkillCondition(TB_SKILL* pSkillTable);

    // GetSkillLevel - 获取技能等级
    int GetSkillLevel(int nSkillGroup);

    // IsSkillLearned - 检查技能是否已学习
    bool IsSkillLearned(int nSkillGroup);

    // LearnSkill - 学习新技能
    bool LearnSkill(int nSkillGroup, int nLevel = 1);

    // ResetSkill - 重置技能到默认状态
    void ResetSkill(int nSkillGroup);

    // GetSkillList - 获取技能列表
    const std::map<int, tagCOOLTIME>& GetSkillList() const;

    // IsUsingSkill - 检查是否正在使用技能
    bool IsUsingSkill() const;

    // GetCurrentSkill - 获取当前技能ID
    int GetCurrentSkill() const;

    // GetSkillState - 获取技能状态
    int GetSkillState(int nSkillGroup) const;

    // IsCooltime - 检查是否在冷却中
    bool IsCooltime(int nSkillGroup) const;

    // ResetAllCooltime - 重置所有冷却时间
    void ResetAllCooltime();

protected:
    // === 成员变量 (来自 IDA 反编译) ===

    // m_pActor - 关联的 Actor 对象
    XActor* m_pActor;

    // m_mapCooltimeList - 冷却时间列表
    // key: CoolTime_Group, value: tagCOOLTIME
    std::map<int, tagCOOLTIME> m_mapCooltimeList;

    // m_fGlobalCooltime - 全局冷却时间数组
    // [0] = 普通技能全局冷却
    // [1] = Akashic技能全局冷却
    float m_fGlobalCooltime[2];

    // m_bTestMode - 测试模式 (不消耗、无冷却)
    bool m_bTestMode;

    // m_vProjectiles - 投射物列表
    std::vector<VGameProjectileObject*> m_vProjectiles;

    // m_vTraps - 陷阱列表
    std::vector<VGameTrapObject*> m_vTraps;

    // m_vChainLightningObject - 链式闪电对象列表
    std::vector<VChainLightningObject*> m_vChainLightningObject;

    // m_vecRandomTrapEvent - 随机陷阱事件列表
    std::vector<SRandomTrapEvent*> m_vecRandomTrapEvent;

    // m_vecRandomSummonEvent - 随机召唤事件列表
    std::vector<SRandomSummonEvent*> m_vecRandomSummonEvent;

    // m_vecExplodeTrap - 爆炸陷阱列表
    std::vector<SExplodeTrap*> m_vecExplodeTrap;

    // m_mapExplodeSummon - 爆炸召唤映射
    std::map<int, SummonMonsterTrigger*> m_mapExplodeSummon;

    // m_mapProjectileIndex - 投射物索引映射
    std::map<std::int16_t, std::pair<int, std::int16_t>> m_mapProjectileIndex;

    // m_stAttackDamage - 攻击伤害数组 (100 个元素)
    tagSKILL_ACTION_DAMAGE m_stAttackDamage[100];

    // m_stChainHitInfo - 链式命中信息数组 (100 个元素)
    SChainHitInfo m_stChainHitInfo[100];

    // m_stChainSkillInfo - 链式技能信息
    SSkillInfo m_stChainSkillInfo;

    // m_vAttackExtraMove - 攻击额外移动数组 (100 个元素)
    hkvVec3 m_vAttackExtraMove[100];

    // m_dwAttackTarget - 攻击目标ID数组 (100 个元素)
    std::uint32_t m_dwAttackTarget[100];

    // m_nAttackTargetCount - 攻击目标计数
    int m_nAttackTargetCount;

    // m_nBaseDamage - 基础伤害数组
    int m_nBaseDamage[100];  // 假设大小

    // m_fAttackFlySpeed - 攻击飞行速度数组
    float m_fAttackFlySpeed[100];  // 假设大小

    // m_bCheckDelayedProj - 是否检查延迟投射物
    bool m_bCheckDelayedProj;

    // m_bCheckContinuousMelee - 是否检查连续近战
    bool m_bCheckContinuousMelee;

    // m_pActionRes - 动作资源指针
    VActionResourceLump* m_pActionRes;

    // m_nCurrentSkillID - 当前正在使用的技能ID
    int m_nCurrentSkillID;

    // m_bUsingSkill - 是否正在使用技能
    bool m_bUsingSkill;
};
