#include "Soulworker/GameServer/XGameServer/MySkillList.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"

// TODO: 需要包含正确的头文件
// #include "Soulworker/GameServer/XGameServer/Mover.h"
// #include "Soulworker/GameServer/XCore/VisionEngineTimer.h"

// 前置声明 - TODO: 需要正确的头文件
class CMover;
class VDefaultTimer;

// 效果类型常量 (来自 IDA)
const int EFFECT_SKILL_OPTION_COOLTIME = 0;      // TODO: 确认正确值
const int EFFECT_SPECIAL_COOLTIME_RAT = 0;       // TODO: 确认正确值
const int EFFECT_STATUS_COOLTIME = 0;            // TODO: 确认正确值

// ============================================================================
// CMySkillList 实现
// ============================================================================

CMySkillList::CMySkillList()
    : m_pActor(nullptr)
    , m_bTestMode(false)
{
    m_fGlobalCooltime[0] = 0.0f;
    m_fGlobalCooltime[1] = 0.0f;
}

CMySkillList::~CMySkillList() {
}

void CMySkillList::Init(XActor* pActor) {
    m_pActor = pActor;
}

// ============================================================================
// UseSkill - 使用技能
// IDA 0x1402B75E0
// ============================================================================
int CMySkillList::UseSkill(TB_SKILL* pSkillTable, TB_SKILL* pChangedSkillTable, float fSkillCost) {
    // 确定最终技能表
    TB_SKILL* pFinalSkillTable = pChangedSkillTable ? pChangedSkillTable : pSkillTable;
    if (!pFinalSkillTable) {
        return 0;
    }

    int nRet = 0;

    // 如果技能消耗为负，先检查是否可以使用
    if (fSkillCost < 0.0f) {
        // TODO: 调用 IsCanUseSkill 检查
        // nRet = IsCanUseSkill(pSkillTable, pChangedSkillTable, &fSkillCost, 0);
        GreenDamTan_log(__FILE__, __FUNCTION__, "IsCanUseSkill check - stub");
    }

    if (nRet != 0) {
        return nRet;
    }

    // 非测试模式下扣除消耗
    if (fSkillCost >= 0.0f && !m_bTestMode) {
        int iIndex = -1;
        std::uint8_t Skill_Cost_Attribute = pFinalSkillTable->Skill_Cost_Attribute;

        // 根据消耗属性类型确定索引
        switch (Skill_Cost_Attribute) {
            case 1:  // HP
                iIndex = 1;
                break;
            case 2:  // SG
                // TODO: 检查 CMover::IsNoSkillCostSG 和 GetIgnoreSkillCost
                // if (!CMover::IsNoSkillCostSG(m_pActor)) {
                //     if (CMover::GetIgnoreSkillCost(m_pActor) == 1)
                //         iIndex = -1;
                //     else
                //         iIndex = 2;
                // }
                iIndex = 2;  // SG
                break;
            case 3:  // Stamina
                iIndex = 3;
                // TODO: 应用减少速率
                // if (CMover::GetDecreaseStaminaRate(m_pActor) > 0.0f) {
                //     float fRate = CMover::GetDecreaseStaminaRate(m_pActor);
                //     fSkillCost = fSkillCost - (fSkillCost * fRate);
                // }
                break;
            case 4:  // 其他
                iIndex = 16;
                break;
        }

        if (iIndex != -1) {
            // TODO: 根据 Cost_Type 处理消耗
            // if (pFinalSkillTable->Cost_Type && pFinalSkillTable->Cost_Type != 2) {
            //     CMover::SetContinousCost(m_pActor, iIndex, fSkillCost);
            // } else {
            //     float fStat = CMover::GetStat(m_pActor, iIndex);
            //     CMover::SetStat(m_pActor, iIndex, fStat - fSkillCost);
            //     CMover::SendUpdateStat(m_pActor, iIndex);
            // }
            GreenDamTan_log(__FILE__, __FUNCTION__, "Apply skill cost - stub");
        }
    }

    // 设置冷却时间
    SetSkillCooltime(pFinalSkillTable);

    // TODO: 更新技能活跃计数
    // CMover::GetGOC<CGocSkill>(m_pActor, &pSkillPtr, 0);
    // CGocSkill::AddModeSkillActiveCount(pSkillPtr, pSkillTable->Skill_Group, -1, 0);

    return nRet;
}

// ============================================================================
// SetSkillCooltime - 设置技能冷却
// IDA 0x1402C4AD0
// ============================================================================
void CMySkillList::SetSkillCooltime(TB_SKILL* pSkillTable) {
    // 基本检查
    if (!m_pActor || !pSkillTable) {
        return;
    }

    // 如果没有冷却时间，直接返回
    if (pSkillTable->CoolTime == 0 && pSkillTable->CoolTime_Global == 0) {
        return;
    }

    // 被动技能必须有 CoolTime
    if (pSkillTable->Skill_Type == 1 && pSkillTable->CoolTime == 0) {
        return;
    }

    // TODO: 获取当前时间
    // VDefaultTimer* Timer = ThreadLocalData::GetTimer();
    // float fCurrTime = IVTimer::GetTime(Timer);
    float fCurrTime = 0.0f;  // TODO: 使用正确的计时器

    // 获取基础冷却时间 (毫秒)
    float fTotalTime = static_cast<float>(pSkillTable->CoolTime);

    // TODO: 应用 Roguelike 技能冷却修正
    // CMover::GetGOC<CGocSkill>(m_pActor, &pSkill, 0);
    // fTotalTime = CGocSkill::GetRoguelikeSkillCoolTime(pSkill, fTotalTime, pSkillTable->Skill_Group);

    // TODO: 获取冷却速率
    // float fCoolDownRate = CMover::GetSkillCoolDownRate(m_pActor);
    float fCoolDownRate = 0.0f;

    // TODO: 应用属性效果修正
    // CMover::GetGOC<CGocAttribute>(m_pActor, &pAttr, 0);
    // float fSkillOptionCooltime = 0.0f;
    // CGocAttribute::GetSkillOptionEffect(pAttr, pSkillTable->Skill_Group, EFFECT_SKILL_OPTION_COOLTIME, &fSkillOptionCooltime);
    // float fItemCoolDownRate = CGocAttribute::GetSpecialEffect(pAttr, EFFECT_SPECIAL_COOLTIME_RAT);
    // fCoolDownRate = fCoolDownRate + fItemCoolDownRate + fSkillOptionCooltime;

    // TODO: 应用 Deck Bonus 修正
    // CMoverEx* pMoverEx = dynamic_cast<CMoverEx*>(m_pActor);
    // if (pMoverEx) {
    //     TB_DECK_BONUS* pCurDeckBonus = CMoverEx::GetCurDeckBouns(pMoverEx);
    //     if (pCurDeckBonus && pCurDeckBonus->Bonus_Type == 1) {
    //         fCoolDownRate = fCoolDownRate + (pCurDeckBonus->Bonus_Value * 100.0f);
    //     }
    // }

    // 限制冷却速率范围
    if (fCoolDownRate > 100.0f) {
        fCoolDownRate = 100.0f;
    }
    if (fCoolDownRate < 0.0f) {
        fCoolDownRate = 0.0f;
    }

    // 被动技能不应用冷却速率
    if (pSkillTable->Skill_Type == 1) {
        fCoolDownRate = 0.0f;
    }

    // 应用冷却速率减少
    if (fCoolDownRate > 0.0f) {
        float fReduceTime = fTotalTime * (fCoolDownRate * 0.01f);
        fTotalTime = fTotalTime - fReduceTime;
        if (fTotalTime < 0.0f) {
            fTotalTime = 0.0f;
        }
    }

    // 计算结束时间 (毫秒转秒)
    float fCooldownTime = fCurrTime + (fTotalTime * 0.001f);

    // 更新或添加冷却记录
    int nCooltimeGroup = pSkillTable->CoolTime_Group;
    auto iter = m_mapCooltimeList.find(nCooltimeGroup);

    if (iter != m_mapCooltimeList.end()) {
        // 已存在冷却记录，更新为较大值
        if (fCooldownTime > iter->second.fEndTime) {
            iter->second.fStartTime = fCurrTime;
            iter->second.fEndTime = fCooldownTime;
            iter->second.dwTotalTime = static_cast<int>(fTotalTime);
        }
    } else {
        // 添加新冷却记录
        tagCOOLTIME newData;
        newData.fStartTime = fCurrTime;
        newData.dwTotalTime = static_cast<int>(fTotalTime);
        newData.fEndTime = fCooldownTime;
        newData.byType = 0;
        m_mapCooltimeList[nCooltimeGroup] = newData;
    }

    // 设置全局冷却 (非被动技能)
    if (pSkillTable->Skill_Type != 1 && pSkillTable->CoolTime_Global != 0) {
        float fGlobalCooldownTime = fCurrTime + (static_cast<float>(pSkillTable->CoolTime_Global) * 0.001f);
        if (fGlobalCooldownTime > m_fGlobalCooltime[0]) {
            m_fGlobalCooltime[0] = fGlobalCooldownTime;
        }
    }

    // TODO: 检查并应用状态效果冷却减少
    // if (pMoverEx && CMoverEx::GetTotalOptionEffectValue(pMoverEx, EFFECT_STATUS_COOLTIME) > 0.0f) {
    //     float fReduceValue = CMoverEx::GetTotalOptionEffectValue(pMoverEx, EFFECT_STATUS_COOLTIME);
    //     ReduceSkillCooltime(fReduceValue);
    //     CMover::send_eSUB_CMD_SKILL_COOLTIME_REDUCE(m_pActor, m_pActor, fReduceValue);
    // }
}

// ============================================================================
// GetCooltime - 获取技能剩余冷却时间
// IDA 0x1402C4940
// ============================================================================
float CMySkillList::GetCooltime(E_COOLTIME_TYPE eType, int nCooltimeGroup, std::uint16_t wGlobalCoolTime, bool bCheckGlobalCool) {
    if (!m_pActor) {
        return 0.0f;
    }

    if (m_bTestMode) {
        return 0.0f;
    }

    // TODO: 获取当前时间
    // VDefaultTimer* Timer = ThreadLocalData::GetTimer();
    // float fCurrTime = IVTimer::GetTime(Timer);
    float fCurrTime = 0.0f;  // TODO: 使用正确的计时器

    // 查找冷却记录
    auto iter = m_mapCooltimeList.find(nCooltimeGroup);

    if (iter != m_mapCooltimeList.end()) {
        // 找到冷却记录
        if (iter->second.fEndTime <= fCurrTime) {
            // 冷却已结束
            // 检查 Akashic 全局冷却
            if (eType == E_COOLTIME_AKASHIC && wGlobalCoolTime != 0 && m_fGlobalCooltime[1] > fCurrTime) {
                return m_fGlobalCooltime[1] - fCurrTime;
            }
            // 移除已结束的冷却记录
            m_mapCooltimeList.erase(iter);
            return 0.0f;
        } else {
            // 返回剩余时间
            return iter->second.fEndTime - fCurrTime;
        }
    }

    // 没有找到冷却记录，检查全局冷却
    if (bCheckGlobalCool && wGlobalCoolTime != 0) {
        int nIndex = static_cast<int>(eType);
        if (nIndex >= 0 && nIndex < 2 && m_fGlobalCooltime[nIndex] > fCurrTime) {
            return m_fGlobalCooltime[nIndex] - fCurrTime;
        }
    }

    return 0.0f;
}

// ============================================================================
// ReduceSkillCooltime - 减少技能冷却时间
// IDA 0x1402C5280
// ============================================================================
void CMySkillList::ReduceSkillCooltime(float fReduceRate) {
    // 遍历所有冷却记录
    for (auto& pair : m_mapCooltimeList) {
        tagCOOLTIME& cooltime = pair.second;

        // 跳过 Akashic 类型
        if (cooltime.byType != 0) {
            continue;
        }

        // 计算减少后的总冷却时间
        float fTotalCoolTime = static_cast<float>(cooltime.dwTotalTime);
        fTotalCoolTime = fTotalCoolTime - (fTotalCoolTime * fReduceRate * 0.01f);
        if (fTotalCoolTime < 0.0f) {
            fTotalCoolTime = 0.0f;
        }

        // 更新结束时间
        cooltime.fEndTime = cooltime.fStartTime + (fTotalCoolTime * 0.001f);
        cooltime.dwTotalTime = static_cast<int>(fTotalCoolTime);
    }
}

// ============================================================================
// ResetCoolTime - 重置冷却时间
// IDA 0x1402C4870
// ============================================================================
void CMySkillList::ResetCoolTime(E_COOLTIME_TYPE eType) {
    // 遍历并移除指定类型的冷却记录
    for (auto iter = m_mapCooltimeList.begin(); iter != m_mapCooltimeList.end(); ) {
        if (iter->second.byType == static_cast<std::uint8_t>(eType)) {
            iter = m_mapCooltimeList.erase(iter);
        } else {
            ++iter;
        }
    }

    // 重置全局冷却
    int nIndex = static_cast<int>(eType);
    if (nIndex >= 0 && nIndex < 2) {
        m_fGlobalCooltime[nIndex] = 0.0f;
    }
}

// ============================================================================
// SetAkashicCooltime - 设置 Akashic 冷却
// IDA 0x1402C5060
// ============================================================================
void CMySkillList::SetAkashicCooltime(TB_AKASHIC_RECORDS* pAkashicTable) {
    if (!m_pActor || !pAkashicTable) {
        return;
    }

    // TODO: 实现 Akashic 冷却设置
    // 类似 SetSkillCooltime，但使用 Akashic 表数据
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetAkashicCooltime - stub");
}

// ============================================================================
// GetSkillCost - 获取技能消耗
// IDA 0x1402C54F0
// ============================================================================
float CMySkillList::GetSkillCost(TB_SKILL* pSkillTable) {
    if (!pSkillTable) {
        return 0.0f;
    }

    // TODO: 计算技能消耗
    // 基础消耗 * 各种修正
    return static_cast<float>(pSkillTable->Skill_Cost);
}

// ============================================================================
// SetTestMode - 设置测试模式
// ============================================================================
void CMySkillList::SetTestMode(int bTestMode) {
    m_bTestMode = (bTestMode != 0);
}

// ============================================================================
// ThinkFunction - 思考函数
// IDA 0x1402B6500
// ============================================================================
void CMySkillList::ThinkFunction() {
    // TODO: 实现思考函数
    // 处理技能更新、冷却检查等
    GreenDamTan_log(__FILE__, __FUNCTION__, "ThinkFunction - stub");
}
