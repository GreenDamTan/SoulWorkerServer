#include "Skill.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"

// ============================================================================
// CSkill 构造函数
// IDA 0x1401681B0: ??0CSkill@@QEAA@PEAUTB_SKILL@@@Z
// 反编译代码:
//   this->m_pTBSkill = _pTBSkill;
//   this->m_pTBDivergence = nullptr;
//   this->m_pTBSwapSkill = nullptr;
//   this->m_pTBSwapDivergence = nullptr;
//   this->m_nSkill_Index = 0;
//   this->m_nSkill_Group = 0;
//   if ( this->m_pTBSkill ) {
//     this->m_nSkill_Index = this->m_pTBSkill->Skill_Index;
//     this->m_nSkill_Group = this->m_pTBSkill->Skill_Group;
//   }
// ============================================================================
CSkill::CSkill(TB_SKILL* pTBSkill)
    : m_pTBSkill(pTBSkill)
    , m_pTBDivergence(nullptr)
    , m_pTBSwapSkill(nullptr)
    , m_pTBSwapDivergence(nullptr)
    , m_nSkill_Index(0)
    , m_nSkill_Group(0)
{
    // 如果技能表有效，初始化索引和组ID
    if (m_pTBSkill) {
        m_nSkill_Index = m_pTBSkill->Skill_Index;
        m_nSkill_Group = m_pTBSkill->Skill_Group;
    }
}

// ============================================================================
// GetLevel - 获取技能等级
// 从分歧表获取，如果没有分歧表则返回1
// ============================================================================
int CSkill::GetLevel() const {
    if (m_pTBDivergence) {
        // TODO: 从分歧表获取等级
        // return m_pTBDivergence->Skill_Level;
        return 1;
    }
    return 1;
}
