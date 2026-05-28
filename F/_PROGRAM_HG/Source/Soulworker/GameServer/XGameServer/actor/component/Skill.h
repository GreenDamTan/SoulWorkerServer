#pragma once

#include <cstdint>

// Forward declarations
struct TB_SKILL;
struct TB_SKILL_DIVERGENCE;
struct TB_SWAP_SKILL;
struct TB_SWAP_SKILL_DIVERGENCE;

/**
 * @brief CSkill - 技能类
 *
 * 表示单个技能实例，包含技能表引用和相关数据。
 * IDA 反编译来源: GameServer.exe
 * 构造函数地址: 0x1401681B0
 */
class CSkill {
public:
    // ============================================================================
    // 构造函数
    // IDA 0x1401681B0: ??0CSkill@@QEAA@PEAUTB_SKILL@@@Z
    // ============================================================================
    CSkill(TB_SKILL* pTBSkill = nullptr);

    // 析构函数
    ~CSkill() = default;

    // ============================================================================
    // GetID - 获取技能ID
    // IDA 0x140085A40: ?GetID@CSkill@@QEBA?BHXZ
    // 返回 m_nSkill_Index
    // ============================================================================
    int GetID() const { return m_nSkill_Index; }

    // ============================================================================
    // GetGroup - 获取技能组ID
    // IDA 0x140168240: ?GetGroup@CSkill@@QEBA?BHXZ
    // 返回 m_nSkill_Group
    // ============================================================================
    int GetGroup() const { return m_nSkill_Group; }

    // ============================================================================
    // GetTable - 获取技能表指针
    // ============================================================================
    TB_SKILL* GetTable() const { return m_pTBSkill; }

    // ============================================================================
    // GetDivergence - 获取分歧表指针
    // ============================================================================
    TB_SKILL_DIVERGENCE* GetDivergence() const { return m_pTBDivergence; }

    // ============================================================================
    // GetSwapSkill - 获取交换技能表指针
    // ============================================================================
    TB_SWAP_SKILL* GetSwapSkill() const { return m_pTBSwapSkill; }

    // ============================================================================
    // GetSwapDivergence - 获取交换分歧表指针
    // ============================================================================
    TB_SWAP_SKILL_DIVERGENCE* GetSwapDivergence() const { return m_pTBSwapDivergence; }

    // ============================================================================
    // SetDivergence - 设置分歧表指针
    // ============================================================================
    void SetDivergence(TB_SKILL_DIVERGENCE* pDivergence) { m_pTBDivergence = pDivergence; }

    // ============================================================================
    // SetSwapSkill - 设置交换技能表指针
    // ============================================================================
    void SetSwapSkill(TB_SWAP_SKILL* pSwapSkill) { m_pTBSwapSkill = pSwapSkill; }

    // ============================================================================
    // SetSwapDivergence - 设置交换分歧表指针
    // ============================================================================
    void SetSwapDivergence(TB_SWAP_SKILL_DIVERGENCE* pSwapDivergence) { m_pTBSwapDivergence = pSwapDivergence; }

    // ============================================================================
    // GetDivergenceID - 获取分歧ID (从分歧表获取Div_GroupID)
    // IDA 显示分歧ID存储在偏移 +3 (Div_GroupID)
    // ============================================================================
    int GetDivergenceID() const;

    // ============================================================================
    // GetDivergenceID2 - 获取第二个分歧ID (从分歧表获取)
    // IDA 显示第二个分歧ID存储在偏移 +7
    // ============================================================================
    int GetDivergenceID2() const;

    // ============================================================================
    // GetLevel - 获取技能等级 (从分歧表获取)
    // ============================================================================
    int GetLevel() const;

    // ============================================================================
    // 成员变量 (来自 IDA 反编译)
    // ============================================================================
protected:
    // m_pTBSkill - 技能表指针
    TB_SKILL* m_pTBSkill;

    // m_pTBDivergence - 技能分歧表指针
    TB_SKILL_DIVERGENCE* m_pTBDivergence;

    // m_pTBSwapSkill - 交换技能表指针
    TB_SWAP_SKILL* m_pTBSwapSkill;

    // m_pTBSwapDivergence - 交换分歧表指针
    TB_SWAP_SKILL_DIVERGENCE* m_pTBSwapDivergence;

    // m_nSkill_Index - 技能索引ID
    int m_nSkill_Index;

    // m_nSkill_Group - 技能组ID
    int m_nSkill_Group;
};
