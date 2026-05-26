#pragma once

#include <cstdint>
#include <map>
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"

// 前置声明
struct TB_SKILL;
struct TB_AKASHIC_RECORDS;
class XActor;
class CMover;
class CMoverEx;

// E_COOLTIME_TYPE 枚举 - 冷却时间类型
enum E_COOLTIME_TYPE {
    E_COOLTIME_NORMAL = 0,      // 普通技能
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
    void Init(XActor* pActor);

    // === 技能使用 ===
    // UseSkill: IDA 0x1402B75E0
    // 使用技能，返回错误码 (0=成功)
    int UseSkill(TB_SKILL* pSkillTable, TB_SKILL* pChangedSkillTable = nullptr, float fSkillCost = -1.0f);

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

    // === 测试模式 ===
    void SetTestMode(int bTestMode);

    // === 思考函数 ===
    // ThinkFunction: IDA 0x1402B6500
    void ThinkFunction();

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

    // 其他成员待补充...
};
