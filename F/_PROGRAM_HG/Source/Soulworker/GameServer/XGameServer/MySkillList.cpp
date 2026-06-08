#include "Soulworker/GameServer/XGameServer/MySkillList.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XCore/VisionEngineTypes.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include <cstring>

// Forward declarations
class CMover;

// 效果类型常量 (来自 IDA)
const int EFFECT_SKILL_OPTION_COOLTIME = 0;      // TODO: 确认正确值
const int EFFECT_SPECIAL_COOLTIME_RAT = 0;       // TODO: 确认正确值
const int EFFECT_STATUS_COOLTIME = 0;            // TODO: 确认正确值

// 错误码常量 (来自 IsCanUseSkill)
const int SKILL_ERROR_SUCCESS = 0;
const int SKILL_ERROR_NOT_HAVE = 56001;
const int SKILL_ERROR_WRONG_CLASS = 56002;
const int SKILL_ERROR_LEVEL_LOW = 56003;
const int SKILL_ERROR_HP_LOW = 56004;
const int SKILL_ERROR_SG_LOW = 56005;
const int SKILL_ERROR_STAMINA_LOW = 56006;
const int SKILL_ERROR_OTHER_LOW = 56007;
const int SKILL_ERROR_INVALID = 56009;
const int SKILL_ERROR_COOLTIME = 56010;

// ============================================================================
// CMySkillList 构造函数
// IDA 0x1402B5BD0
// ============================================================================
CMySkillList::CMySkillList()
    : m_pActor(nullptr)
    , m_bTestMode(false)
    , m_nAttackTargetCount(0)
    , m_bCheckDelayedProj(true)
    , m_bCheckContinuousMelee(true)
    , m_pActionRes(nullptr)
    , m_nCurrentSkillID(0)
    , m_bUsingSkill(false)
{
    m_fGlobalCooltime[0] = 0.0f;
    m_fGlobalCooltime[1] = 0.0f;

    // 初始化数组
    memset(m_stAttackDamage, 0, sizeof(m_stAttackDamage));
    memset(m_dwAttackTarget, 0, sizeof(m_dwAttackTarget));
    memset(m_stChainHitInfo, 0, sizeof(m_stChainHitInfo));
    memset(&m_stChainSkillInfo, 0, sizeof(m_stChainSkillInfo));
    memset(m_nBaseDamage, 0, sizeof(m_nBaseDamage));
    memset(m_fAttackFlySpeed, 0, sizeof(m_fAttackFlySpeed));

    // 初始化 hkvVec3 数组
    for (int i = 0; i < 100; ++i) {
        m_vAttackExtraMove[i] = hkvVec3();
    }
}

// ============================================================================
// CMySkillList 析构函数
// IDA 0x1402B5E40
// ============================================================================
CMySkillList::~CMySkillList() {
    // 清理投射物列表
    for (auto it = m_vProjectiles.begin(); it != m_vProjectiles.end(); ) {
        VGameProjectileObject* pProjectile = *it;
        if (pProjectile) {
            RemoveProjectile(pProjectile);
        }
        it = m_vProjectiles.erase(it);
    }
    m_vProjectiles.clear();

    // 清理陷阱列表
    for (auto it = m_vTraps.begin(); it != m_vTraps.end(); ) {
        VGameTrapObject* pTrap = *it;
        if (pTrap) {
            RemoveTrap(pTrap);
        }
        it = m_vTraps.erase(it);
    }
    m_vTraps.clear();

    // 清理链式闪电列表
    for (auto it = m_vChainLightningObject.begin(); it != m_vChainLightningObject.end(); ) {
        VChainLightningObject* pChain = *it;
        if (pChain) {
            RemoveChainLightning(pChain);
        }
        it = m_vChainLightningObject.erase(it);
    }
    m_vChainLightningObject.clear();

    // 清理爆炸陷阱列表
    for (auto it = m_vecExplodeTrap.begin(); it != m_vecExplodeTrap.end(); ) {
        SExplodeTrap* pExplodeTrap = *it;
        if (pExplodeTrap) {
            delete pExplodeTrap;
        }
        it = m_vecExplodeTrap.erase(it);
    }
    m_vecExplodeTrap.clear();

    // 清理随机陷阱事件列表
    for (auto it = m_vecRandomTrapEvent.begin(); it != m_vecRandomTrapEvent.end(); ) {
        SRandomTrapEvent* pRandomTrap = *it;
        if (pRandomTrap) {
            delete pRandomTrap;
        }
        it = m_vecRandomTrapEvent.erase(it);
    }
    m_vecRandomTrapEvent.clear();

    // 清理随机召唤事件列表
    for (auto it = m_vecRandomSummonEvent.begin(); it != m_vecRandomSummonEvent.end(); ) {
        SRandomSummonEvent* pRandomSummon = *it;
        if (pRandomSummon) {
            delete pRandomSummon;
        }
        it = m_vecRandomSummonEvent.erase(it);
    }
    m_vecRandomSummonEvent.clear();

    // 清理映射
    m_mapExplodeSummon.clear();
    m_mapCooltimeList.clear();
    m_mapProjectileIndex.clear();
}

// ============================================================================
// Init - 初始化
// IDA 0x1402B6460
// ============================================================================
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
        nRet = IsCanUseSkill(pSkillTable, pChangedSkillTable, &fSkillCost, false);
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
                iIndex = 2;  // SG
                break;
            case 3:  // Stamina
                iIndex = 3;
                break;
            case 4:  // 其他
                iIndex = 16;
                break;
        }

        if (iIndex != -1) {
            // TODO: 根据 Cost_Type 处理消耗
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
// EndSkill - 结束技能
// IDA 0x1402B79F0
// ============================================================================
void CMySkillList::EndSkill(TB_SKILL* pSkillTable) {
    if (!pSkillTable) {
        return;
    }

    int iIndex = -1;
    std::uint8_t Skill_Cost_Attribute = pSkillTable->Skill_Cost_Attribute;

    switch (Skill_Cost_Attribute) {
        case 1:  // HP
            iIndex = 1;
            break;
        case 2:  // SG
            iIndex = 2;
            break;
        case 3:  // Stamina
            iIndex = 3;
            break;
        case 4:  // 其他
            iIndex = 16;
            break;
    }

    float fSkillCost = GetSkillCost(pSkillTable);

    if (fSkillCost > 0.0f && pSkillTable->Cost_Type == 1 && !m_bTestMode && iIndex != -1) {
        // TODO: CMover::SetContinousCost(m_pActor, iIndex, 0.0f);
        GreenDamTan_log(__FILE__, __FUNCTION__, "SetContinousCost - stub");
    }

    if (iIndex == 3 && fSkillCost == 0.0f) {
        // TODO: 处理 Stamina 相关逻辑
        GreenDamTan_log(__FILE__, __FUNCTION__, "Stamina handling - stub");
    }
}

// ============================================================================
// IsCanUseSkill - 检查是否可以使用技能
// IDA 0x1402B7B30
// ============================================================================
int CMySkillList::IsCanUseSkill(TB_SKILL* pSkillTable, TB_SKILL* pChangedSkillTable, float* fSkillCost, bool bExceptHaveCheck) {
    if (!m_pActor) {
        return SKILL_ERROR_INVALID;
    }

    if (!pSkillTable) {
        return SKILL_ERROR_INVALID;
    }

    // TODO: 获取 GOC 属性组件检查
    // CMover::GetGOC<CGocAttribute>(m_pActor, &pAttr, 0);

    // 检查是否拥有技能
    if (!bExceptHaveCheck) {
        // TODO: 检查 CGocSkill::IsHaveBaseSkill 和 IsHaveSkillQuickSlot
        GreenDamTan_log(__FILE__, __FUNCTION__, "Check have skill - stub");
    }

    // 检查等级要求
    // TODO: int nLv = CGocAttribute::GetLv(pAttr);
    // if (pSkillTable->Req_Min_LV > nLv) {
    //     return SKILL_ERROR_LEVEL_LOW;
    // }

    // 检查职业要求
    if (pSkillTable->Use_Class) {
        // TODO: std::uint8_t byClass = CMover::GetClass(m_pActor);
        // if (pSkillTable->Use_Class != byClass) {
        //     return SKILL_ERROR_WRONG_CLASS;
        // }
        GreenDamTan_log(__FILE__, __FUNCTION__, "Check class requirement - stub");
    }

    // 检查觉醒等级
    // TODO: if (CGocAttribute::GetAwaken(pAttr) < pSkillTable->Req_Min_AwakeningGrade) {
    //     return SKILL_ERROR_INVALID;
    // }

    // 检查是否可以攻击
    // TODO: if (!CMover::IsCanAttack(m_pActor)) {
    //     return SKILL_ERROR_INVALID;
    // }

    // 测试模式直接返回成功
    if (m_bTestMode) {
        return SKILL_ERROR_SUCCESS;
    }

    // 确定最终技能表
    TB_SKILL* pFinalSkillTable = pChangedSkillTable ? pChangedSkillTable : pSkillTable;

    // 获取技能消耗
    *fSkillCost = GetSkillCost(pFinalSkillTable);

    // 检查资源是否足够
    if (*fSkillCost > 0.0f) {
        std::uint8_t Skill_Cost_Attribute = pFinalSkillTable->Skill_Cost_Attribute;

        switch (Skill_Cost_Attribute) {
            case 1:  // HP
                // TODO: if (*fSkillCost >= CMover::GetStat(m_pActor, 1)) {
                //     return SKILL_ERROR_HP_LOW;
                // }
                break;
            case 2:  // SG
                // TODO: if (!CMover::IsNoSkillCostSG(m_pActor) &&
                //          *fSkillCost > CMover::GetStat(m_pActor, 2)) {
                //     return SKILL_ERROR_SG_LOW;
                // }
                break;
            case 3:  // Stamina
                // TODO: float fDecreaseRate = CMover::GetDecreaseStaminaRate(m_pActor);
                // *fSkillCost = *fSkillCost - (*fSkillCost * fDecreaseRate);
                // if (*fSkillCost > CMover::GetStat(m_pActor, 3)) {
                //     return SKILL_ERROR_STAMINA_LOW;
                // }
                break;
            case 4:  // 其他
                // TODO: if (*fSkillCost > CMover::GetStat(m_pActor, 16)) {
                //     return SKILL_ERROR_OTHER_LOW;
                // }
                break;
        }
    }

    // 检查冷却时间
    float fCooltime = GetCooltime(E_COOLTIME_SKILL, pFinalSkillTable->CoolTime_Group,
                                   pFinalSkillTable->CoolTime_Global, true);
    if (fCooltime > 0.0f) {
        return SKILL_ERROR_COOLTIME;
    }

    return SKILL_ERROR_SUCCESS;
}

// ============================================================================
// SetSkillCooltime - 设置技能冷却
// IDA 0x1402C4AD0 - 精确还原
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

    // IDA: 获取基础冷却时间 (毫秒)
    float fTotalTime = static_cast<float>(pSkillTable->CoolTime);

    // IDA: 获取 Roguelike 技能冷却时间修正
    // CMover::GetGOC<CGocSkill>(m_pActor, &pSkill, 0);
    // if (pSkill) {
    //     fTotalTime = CGocSkill::GetRoguelikeSkillCoolTime(pSkill, fTotalTime, pSkillTable->Skill_Group);
    // }
    // TODO: 需要实现 CGocSkill::GetRoguelikeSkillCoolTime

    // IDA: 获取冷却速率
    float fCoolDownRate = 0.0f;
    // fCoolDownRate = CMover::GetSkillCoolDownRate(m_pActor);
    // TODO: 需要实现 CMover::GetSkillCoolDownRate

    // IDA: 获取属性效果冷却时间修正
    float fSkillOptionCooltime = 0.0f;
    float fItemCoolDownRate = 0.0f;
    // CMover::GetGOC<CGocAttribute>(m_pActor, &pAttr, 0);
    // if (pAttr) {
    //     CGocAttribute::GetSkillOptionEffect(pAttr, pSkillTable->Skill_Group, EFFECT_SKILL_OPTION_COOLTIME, &fSkillOptionCooltime);
    //     fItemCoolDownRate = CGocAttribute::GetSpecialEffect(pAttr, EFFECT_SPECIAL_COOLTIME_RAT);
    //     fCoolDownRate = fCoolDownRate + (fItemCoolDownRate + fSkillOptionCooltime);
    // }
    // TODO: 需要实现 CGocAttribute 方法

    // IDA: 获取 Deck Bonus 冷却修正
    // CMoverEx* pMoverEx = dynamic_cast<CMoverEx*>(m_pActor);
    // if (pMoverEx) {
    //     TB_DECK_BONUS* pCurDeckBonus = CMoverEx::GetCurDeckBouns(pMoverEx);
    //     if (pCurDeckBonus && pCurDeckBonus->Bonus_Type == 1) {
    //         fCoolDownRate = fCoolDownRate + (pCurDeckBonus->Bonus_Value * 100.0f);
    //     }
    // }
    // TODO: 需要实现 CMoverEx::GetCurDeckBouns

    // IDA: 限制冷却速率范围 [0, 100]
    if (fCoolDownRate > 100.0f) {
        fCoolDownRate = 100.0f;
    }
    if (fCoolDownRate < 0.0f) {
        fCoolDownRate = 0.0f;
    }

    // IDA: 被动技能不应用冷却速率
    if (pSkillTable->Skill_Type == 1) {
        fCoolDownRate = 0.0f;
    }

    // IDA: 应用冷却速率减少
    if (fCoolDownRate > 0.0f) {
        float fReduceTime = fTotalTime * (fCoolDownRate * 0.01f);
        fTotalTime = fTotalTime - fReduceTime;
        if (fTotalTime < 0.0f) {
            fTotalTime = 0.0f;
        }
    }

    // IDA: 获取当前时间
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fCurrTime = pTimer ? pTimer->GetTime() : 0.0f;

    // IDA: 计算结束时间 (毫秒转秒)
    float fCooldownTime = fCurrTime + (fTotalTime * 0.001f);

    // IDA: 更新或添加冷却记录
    int nCooltimeGroup = pSkillTable->CoolTime_Group;
    auto iter = m_mapCooltimeList.find(nCooltimeGroup);

    if (iter != m_mapCooltimeList.end()) {
        // IDA: 已存在冷却记录，更新为较大值
        if (fCooldownTime > iter->second.fEndTime) {
            iter->second.fStartTime = fCurrTime;
            iter->second.fEndTime = fCooldownTime;
            iter->second.dwTotalTime = static_cast<int>(fTotalTime);
        }
    } else {
        // IDA: 添加新冷却记录
        tagCOOLTIME newData;
        newData.fStartTime = fCurrTime;
        newData.dwTotalTime = static_cast<int>(fTotalTime);
        newData.fEndTime = fCooldownTime;
        newData.byType = 0;
        m_mapCooltimeList[nCooltimeGroup] = newData;
    }

    // IDA: 设置全局冷却 (非被动技能)
    if (pSkillTable->Skill_Type != 1 && pSkillTable->CoolTime_Global != 0) {
        float fGlobalCooldownTime = fCurrTime + (static_cast<float>(pSkillTable->CoolTime_Global) * 0.001f);
        if (fGlobalCooldownTime > m_fGlobalCooltime[0]) {
            m_fGlobalCooltime[0] = fGlobalCooldownTime;
        }
    }

    // IDA: 检查并应用状态效果冷却时间减少
    // if (pMoverEx) {
    //     float fReduceValue = CMoverEx::GetTotalOptionEffectValue(pMoverEx, EFFECT_STATUS_COOLTIME);
    //     if (fReduceValue > 0.0f) {
    //         ReduceSkillCooltime(fReduceValue);
    //         CMover::send_eSUB_CMD_SKILL_COOLTIME_REDUCE(m_pActor, m_pActor, fReduceValue);
    //     }
    // }
    // TODO: 需要实现相关方法
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

    // IDA 0x1406D1A80 + 0x140276890: Get current time
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fCurrTime = pTimer ? pTimer->GetTime() : 0.0f;

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
// GetCooltime - 获取指定技能组的剩余冷却时间
// Phase 6 新增
// ============================================================================
float CMySkillList::GetCooltime(int nSkillGroup) {
    return GetCooltime(E_COOLTIME_SKILL, nSkillGroup, 0, false);
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
// IDA 0x1402C5060 - 精确还原
// ============================================================================
void CMySkillList::SetAkashicCooltime(TB_AKASHIC_RECORDS* pAkashicTable) {
    // IDA: 检查参数和测试模式
    if (!pAkashicTable || m_bTestMode) {
        return;
    }

    // IDA: 获取当前时间
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fCurrTime = pTimer ? pTimer->GetTime() : 0.0f;

    // IDA: 计算结束时间 (毫秒转秒)
    float fCooldownTime = fCurrTime + (static_cast<float>(pAkashicTable->CoolTime) * 0.001f);

    // IDA: 查找冷却记录
    int nCooltimeGroup = pAkashicTable->CoolTime_Group;
    auto iter = m_mapCooltimeList.find(nCooltimeGroup);

    if (iter != m_mapCooltimeList.end()) {
        // IDA: 已存在冷却记录，更新为较大值
        if (fCooldownTime > iter->second.fEndTime) {
            iter->second.fStartTime = fCurrTime;
            iter->second.fEndTime = fCooldownTime;
            iter->second.dwTotalTime = pAkashicTable->CoolTime;
        }
    } else {
        // IDA: 添加新冷却记录
        tagCOOLTIME newData;
        newData.dwTotalTime = pAkashicTable->CoolTime;
        newData.fStartTime = fCurrTime;
        newData.fEndTime = fCooldownTime;
        newData.byType = 1;  // Akashic 类型
        m_mapCooltimeList[nCooltimeGroup] = newData;
    }

    // IDA: 设置 Akashic 全局冷却
    if (pAkashicTable->CoolTime_Global != 0) {
        float fGlobalCooldownTime = fCurrTime + (static_cast<float>(pAkashicTable->CoolTime_Global) * 0.001f);
        if (fGlobalCooldownTime > m_fGlobalCooltime[1]) {
            m_fGlobalCooltime[1] = fGlobalCooldownTime;
        }
    }
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
// GetHaveSkillGroup - 获取拥有的技能组
// IDA 0x1402C53D0
// ============================================================================
std::tr1::shared_ptr<CSkill> CMySkillList::GetHaveSkillGroup(int nSkillGroup) {
    // TODO: 实现
    // CMover::GetGOC<CGocSkill>(m_pActor, &pSkillPtr, 0);
    // if (pSkillPtr) {
    //     return CGocSkill::GetHaveSkillGroup(pSkillPtr, nSkillGroup);
    // }
    GreenDamTan_log(__FILE__, __FUNCTION__, "GetHaveSkillGroup - stub");
    return std::tr1::shared_ptr<CSkill>();
}

// ============================================================================
// GetAttackTarget - 获取攻击目标
// IDA 0x14019B9B0
// ============================================================================
std::uint32_t CMySkillList::GetAttackTarget(int iIndex) {
    return m_dwAttackTarget[iIndex];
}

// ============================================================================
// SetAttackTarget - 设置攻击目标
// IDA 0x1405FA3D0
// ============================================================================
void CMySkillList::SetAttackTarget(int iIndex, std::uint32_t dwVal) {
    m_dwAttackTarget[iIndex] = dwVal;
}

// ============================================================================
// AddSkillTarget - 添加技能目标
// IDA 0x1402BE150
// ============================================================================
void CMySkillList::AddSkillTarget(std::uint8_t byIndex, std::uint32_t dwTargetID,
                                   std::uint8_t byReaction, std::uint8_t byHitPartsIndex) {
    if (byIndex < 100) {
        m_dwAttackTarget[byIndex] = dwTargetID;
        m_stAttackDamage[byIndex].byReactionType = byReaction;
        m_stAttackDamage[byIndex].byHitPartsIndex = byHitPartsIndex;
    }
}

// ============================================================================
// ClearSkillTarget - 清除技能目标
// IDA 0x1402BE1C0
// ============================================================================
void CMySkillList::ClearSkillTarget() {
    memset(m_stAttackDamage, 0, sizeof(m_stAttackDamage));
    memset(m_dwAttackTarget, 0, sizeof(m_dwAttackTarget));
    memset(m_stChainHitInfo, 0, sizeof(m_stChainHitInfo));
    memset(&m_stChainSkillInfo, 0, sizeof(m_stChainSkillInfo));
}

// ============================================================================
// SetAttackDamage - 设置攻击伤害
// IDA 0x1402C7DE0
// ============================================================================
void CMySkillList::SetAttackDamage(int iIndex, tagSKILL_ACTION_DAMAGE* stVal) {
    if (iIndex >= 0 && iIndex < 100 && stVal) {
        m_stAttackDamage[iIndex] = *stVal;
    }
}

// ============================================================================
// GetAttackDamage - 获取攻击伤害
// IDA 0x1402C7E40 - 精确还原：直接拷贝，无边界检查
// ============================================================================
tagSKILL_ACTION_DAMAGE CMySkillList::GetAttackDamage(int iIndex) {
    tagSKILL_ACTION_DAMAGE result;
    // IDA精确还原：使用memcpy拷贝20字节的m_stAttackDamage[iIndex]
    memcpy(&result, &m_stAttackDamage[iIndex], sizeof(tagSKILL_ACTION_DAMAGE));
    return result;
}

// ============================================================================
// ClearAttackDamage - 清除攻击伤害
// IDA 0x1402C3270
// ============================================================================
void CMySkillList::ClearAttackDamage() {
    memset(m_stAttackDamage, 0, sizeof(m_stAttackDamage));
}

// ============================================================================
// ClearAttackDamage (带索引) - 清除指定攻击伤害
// IDA 0x1402C3290
// ============================================================================
void CMySkillList::ClearAttackDamage(unsigned int iIndex) {
    if (iIndex < 100) {
        m_stAttackDamage[iIndex].Clear();
    }
}

// ============================================================================
// SetBaseDamage - 设置基础伤害
// IDA 0x1403A26A0
// ============================================================================
void CMySkillList::SetBaseDamage(int iIndex, int nVal) {
    if (iIndex >= 0 && iIndex < 100) {
        m_nBaseDamage[iIndex] = nVal;
    }
}

// ============================================================================
// GetBaseDamage - 获取基础伤害
// IDA 0x1403A2770
// ============================================================================
int CMySkillList::GetBaseDamage(int iIndex) {
    if (iIndex >= 0 && iIndex < 100) {
        return m_nBaseDamage[iIndex];
    }
    return 0;
}

// ============================================================================
// SetAttackFlySpeed - 设置攻击飞行速度
// IDA 0x1402C7350
// ============================================================================
void CMySkillList::SetAttackFlySpeed(int iIndex, float fFlySpeed) {
    if (iIndex >= 0 && iIndex < 100) {
        m_fAttackFlySpeed[iIndex] = fFlySpeed;
    }
}

// ============================================================================
// GetAttackFlySpeed - 获取攻击飞行速度
// IDA 0x140375140
// ============================================================================
float CMySkillList::GetAttackFlySpeed(int iIndex) {
    if (iIndex >= 0 && iIndex < 100) {
        return m_fAttackFlySpeed[iIndex];
    }
    return 0.0f;
}

// ============================================================================
// SetAttackExtraMove - 设置攻击额外移动
// IDA 0x1402C7380
// ============================================================================
void CMySkillList::SetAttackExtraMove(int iIndex, hkvVec3* vMove) {
    if (iIndex >= 0 && iIndex < 100 && vMove) {
        m_vAttackExtraMove[iIndex] = *vMove;
    }
}

// ============================================================================
// GetAttackExtraMove - 获取攻击额外移动
// IDA 0x140375160
// ============================================================================
hkvVec3& CMySkillList::GetAttackExtraMove(int iIndex) {
    static hkvVec3 dummy;
    if (iIndex >= 0 && iIndex < 100) {
        return m_vAttackExtraMove[iIndex];
    }
    return dummy;
}

// ============================================================================
// RemoveProjectile - 移除投射物
// IDA 0x1402C0710
// ============================================================================
void CMySkillList::RemoveProjectile(VGameProjectileObject* pProjectile) {
    // TODO: CMover::DebugOut(m_pActor, "RemoveProjectile>> %x", pProjectile);

    // TODO: ThreadLocalData::DeleteProjectile
    GreenDamTan_log(__FILE__, __FUNCTION__, "RemoveProjectile - stub");
}

// ============================================================================
// AddTrap - 添加陷阱
// IDA 0x14063FE30
// ============================================================================
void CMySkillList::AddTrap(VGameTrapObject* pTrap) {
    if (pTrap) {
        m_vTraps.push_back(pTrap);
    }
}

// ============================================================================
// RemoveTrap - 移除陷阱
// IDA 0x1402C0E80
// ============================================================================
void CMySkillList::RemoveTrap(VGameTrapObject* pTrap) {
    // TODO: CMover::DebugOut(m_pActor, "RemoveTrap>> %x", pTrap);

    // TODO: ThreadLocalData::DeleteTrap
    GreenDamTan_log(__FILE__, __FUNCTION__, "RemoveTrap - stub");
}

// ============================================================================
// RemoveChainLightning (Object) - 移除链式闪电对象
// IDA 0x1402C0EF0
// ============================================================================
void CMySkillList::RemoveChainLightning(VChainLightningObject* pChainLightning) {
    // TODO: CMover::DebugOut(m_pActor, "RemoveChainLightning>> %x", pChainLightning);
    // TODO: VChainLightningObject::ReleaseAllChainEffect(pChainLightning);
    // TODO: ThreadLocalData::DeleteChainLightning
    GreenDamTan_log(__FILE__, __FUNCTION__, "RemoveChainLightning(Object) - stub");
}

// ============================================================================
// RemoveChainLightning (MoverEx) - 移除 MoverEx 的链式闪电
// IDA 0x1402C0F70
// ============================================================================
void CMySkillList::RemoveChainLightning(CMoverEx* pMoverEx) {
    // TODO: CMover::DebugOut(m_pActor, "RemoveChainLightning>> %x", pMoverEx);

    for (auto it = m_vChainLightningObject.begin(); it != m_vChainLightningObject.end(); ++it) {
        VChainLightningObject* pChainLightning = *it;
        if (pChainLightning) {
            // TODO: 检查是否激活并设置结束
            // if (VChainLightningObject::IsActivate(pChainLightning)) {
            //     if (VChainBase_cl::GetOwner(pChainLightning) == pMoverEx) {
            //         pChainLightning->SetFinish();
            //     }
            // }
        }
    }
    GreenDamTan_log(__FILE__, __FUNCTION__, "RemoveChainLightning(MoverEx) - stub");
}

// ============================================================================
// CheckChainLightningTarget - 检查链式闪电目标
// IDA 0x1402C1040
// ============================================================================
void CMySkillList::CheckChainLightningTarget(CMoverEx* pMoverEx) {
    for (auto it = m_vChainLightningObject.begin(); it != m_vChainLightningObject.end(); ++it) {
        VChainLightningObject* pChainLightning = *it;
        if (pChainLightning) {
            // TODO: if (VChainLightningObject::IsActivate(pChainLightning)) {
            //     pChainLightning->AddDeletedTarget(pMoverEx);
            // }
        }
    }
    GreenDamTan_log(__FILE__, __FUNCTION__, "CheckChainLightningTarget - stub");
}

// ============================================================================
// CheckProjectileIndex - 检查投射物索引
// IDA 0x1402C5900
// ============================================================================
bool CMySkillList::CheckProjectileIndex(std::int16_t shTriggerIndex, int iSkillIndex, std::uint8_t bySkillAnimCount) {
    auto iter = m_mapProjectileIndex.find(shTriggerIndex);

    if (iter != m_mapProjectileIndex.end()) {
        // 找到记录
        if (iter->second.first == iSkillIndex && bySkillAnimCount == 4) {
            // 增加计数并检查
            iter->second.second++;
            return iter->second.second < 3;
        }
        return false;
    } else {
        // 添加新记录
        m_mapProjectileIndex[shTriggerIndex] = std::make_pair(iSkillIndex, static_cast<std::int16_t>(1));
        return true;
    }
}

// ============================================================================
// ProjectileIndexClear - 清除投射物索引
// IDA 0x140375300
// ============================================================================
void CMySkillList::ProjectileIndexClear() {
    m_mapProjectileIndex.clear();
}

// ============================================================================
// SetExplodeSummon - 设置爆炸召唤
// IDA 0x1402C57E0
// ============================================================================
void CMySkillList::SetExplodeSummon(int iIndex, SummonMonsterTrigger* pTrigger) {
    // 先删除已存在的记录
    auto iter = m_mapExplodeSummon.find(iIndex);
    if (iter != m_mapExplodeSummon.end()) {
        m_mapExplodeSummon.erase(iter);
    }

    // 添加新记录
    m_mapExplodeSummon[iIndex] = pTrigger;
}

// ============================================================================
// GetExplodeSummon - 获取爆炸召唤
// IDA 0x1402C5890
// ============================================================================
SummonMonsterTrigger* CMySkillList::GetExplodeSummon(int iIndex) {
    auto iter = m_mapExplodeSummon.find(iIndex);
    if (iter != m_mapExplodeSummon.end()) {
        return iter->second;
    }
    return nullptr;
}

// ============================================================================
// ExplodeSummonClear - 清除爆炸召唤
// IDA 0x1403A2D30
// ============================================================================
void CMySkillList::ExplodeSummonClear() {
    m_mapExplodeSummon.clear();
}

// ============================================================================
// ClearRandomSummon - 清除随机召唤
// IDA 0x1406CE540
// ============================================================================
void CMySkillList::ClearRandomSummon() {
    m_vecRandomSummonEvent.clear();
}

// ============================================================================
// IsCheckContinuousMelee - 是否检查连续近战
// IDA 0x1403645F0
// ============================================================================
bool CMySkillList::IsCheckContinuousMelee() const {
    return m_bCheckContinuousMelee;
}

// ============================================================================
// SetCheckContinuousMelee - 设置是否检查连续近战
// IDA 0x1403A2150
// ============================================================================
void CMySkillList::SetCheckContinuousMelee(bool bCheck) {
    m_bCheckContinuousMelee = bCheck;
}

// ============================================================================
// SetCheckDelayedProjectile - 设置是否检查延迟投射物
// IDA 0x1403A2170
// ============================================================================
void CMySkillList::SetCheckDelayedProjectile(bool bCheck) {
    m_bCheckDelayedProj = bCheck;
}

// ============================================================================
// GetProjPathActionRes - 获取投射物路径动作资源
// IDA 0x1402C7710
// ============================================================================
VActionResourceLump* CMySkillList::GetProjPathActionRes() {
    return m_pActionRes;
}

// ============================================================================
// SetTestMode - 设置测试模式
// IDA 0x140407090
// ============================================================================
void CMySkillList::SetTestMode(int bTestMode) {
    m_bTestMode = (bTestMode != 0);
}

// ============================================================================
// ThinkFunction - 思考函数
// IDA 0x1402B6500
// ============================================================================
void CMySkillList::ThinkFunction() {
    if (!m_pActor) {
        return;
    }

    // 处理投射物列表
    for (auto it = m_vProjectiles.begin(); it != m_vProjectiles.end(); ) {
        VGameProjectileObject* pProjectile = *it;
        if (!pProjectile) {
            ++it;
            continue;
        }

        // TODO: 获取区域和实例ID
        // TODO: 调用 Tick
        // TODO: 检查是否正在移动

        // TODO: if (!CMover::IsGazeMoving(pProjectile)) {
        //     RemoveProjectile(pProjectile);
        //     it = m_vProjectiles.erase(it);
        // } else {
        //     ++it;
        // }
        ++it;
    }

    // 处理陷阱列表
    for (auto it = m_vTraps.begin(); it != m_vTraps.end(); ) {
        VGameTrapObject* pTrap = *it;
        if (!pTrap) {
            ++it;
            continue;
        }

        // TODO: 检查区域和日志
        // TODO: 调用 ThinkFunction
        // TODO: 检查是否激活

        // TODO: if (!VGameTrapObject::IsActivate(pTrap)) {
        //     RemoveTrap(pTrap);
        //     it = m_vTraps.erase(it);
        // } else {
        //     ++it;
        // }
        ++it;
    }

    // 处理链式闪电列表
    for (auto it = m_vChainLightningObject.begin(); it != m_vChainLightningObject.end(); ) {
        VChainLightningObject* pChainLightning = *it;
        if (!pChainLightning) {
            ++it;
            continue;
        }

        // TODO: 调用 Tick
        // TODO: 检查是否激活

        // TODO: if (!VChainLightningObject::IsActivate(pChainLightning)) {
        //     RemoveChainLightning(pChainLightning);
        //     it = m_vChainLightningObject.erase(it);
        // } else {
        //     ++it;
        // }
        ++it;
    }

    // 处理随机陷阱事件
    for (auto it = m_vecRandomTrapEvent.begin(); it != m_vecRandomTrapEvent.end(); ) {
        SRandomTrapEvent* pRandomTrap = *it;
        if (!pRandomTrap) {
            ++it;
            continue;
        }

        // TODO: 处理随机陷阱逻辑
        // - 更新时间
        // - 检查延迟
        // - 创建随机陷阱或投掷物
        // - 减少计数

        ++it;
    }

    // 处理随机召唤事件
    for (auto it = m_vecRandomSummonEvent.begin(); it != m_vecRandomSummonEvent.end(); ) {
        SRandomSummonEvent* pRandomSummon = *it;
        if (!pRandomSummon) {
            ++it;
            continue;
        }

        // TODO: 处理随机召唤逻辑
        // - 更新时间
        // - 检查延迟
        // - 创建随机召唤
        // - 减少计数

        ++it;
    }

    // 处理爆炸陷阱列表
    for (auto it = m_vecExplodeTrap.begin(); it != m_vecExplodeTrap.end(); ) {
        SExplodeTrap* pExplodeTrap = *it;
        if (!pExplodeTrap) {
            ++it;
            continue;
        }

        // TODO: 处理爆炸陷阱逻辑
        // - 获取 ActorID
        // - 调用 RetiveExplodeTrap
        // - 删除并移除

        ++it;
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ThinkFunction - partial implementation");
}

// ============================================================================
// CancelSkill - 取消当前技能
// Phase 6 新增
// ============================================================================
void CMySkillList::CancelSkill() {
    if (!m_pActor) {
        return;
    }

    // 清除攻击目标
    ClearSkillTarget();

    // 清除攻击伤害
    ClearAttackDamage();

    // 清除投射物索引
    ProjectileIndexClear();

    // 重置技能使用状态
    m_bUsingSkill = false;
    m_nCurrentSkillID = 0;

    // TODO: 通知 GOC 技能组件取消当前技能
    // CMover::GetGOC<CGocSkill>(m_pActor, &pSkillPtr, 0);
    // if (pSkillPtr) {
    //     CGocSkill::CancelCurrentSkill(pSkillPtr);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CancelSkill - stub");
}

// ============================================================================
// ProcessSkillCoolTime - 处理技能冷却时间更新
// Phase 6 新增
// ============================================================================
void CMySkillList::ProcessSkillCoolTime(float fElapsedTime) {
    // 遍历所有冷却记录，减少冷却时间
    for (auto iter = m_mapCooltimeList.begin(); iter != m_mapCooltimeList.end(); ) {
        tagCOOLTIME& cooltime = iter->second;

        // 减少结束时间
        cooltime.fEndTime -= fElapsedTime;

        // 如果冷却已结束，移除记录
        if (cooltime.fEndTime <= 0.0f) {
            iter = m_mapCooltimeList.erase(iter);
        } else {
            ++iter;
        }
    }

    // 更新全局冷却时间
    for (int i = 0; i < 2; ++i) {
        m_fGlobalCooltime[i] -= fElapsedTime;
        if (m_fGlobalCooltime[i] < 0.0f) {
            m_fGlobalCooltime[i] = 0.0f;
        }
    }
}

// ============================================================================
// SetCooltime - 设置技能冷却时间
// Phase 6 新增
// ============================================================================
void CMySkillList::SetCooltime(int nSkillGroup, float fCooltimeSec) {
    if (!m_pActor) {
        return;
    }

    // IDA 0x1406D1A80 + 0x140276890: Get current time
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fCurrTime = pTimer ? pTimer->GetTime() : 0.0f;

    // 查找或创建冷却记录
    auto iter = m_mapCooltimeList.find(nSkillGroup);

    if (iter != m_mapCooltimeList.end()) {
        // 更新现有记录
        iter->second.fStartTime = fCurrTime;
        iter->second.fEndTime = fCurrTime + fCooltimeSec;
        iter->second.dwTotalTime = static_cast<int>(fCooltimeSec * 1000.0f);
    } else {
        // 创建新记录
        tagCOOLTIME newData;
        newData.fStartTime = fCurrTime;
        newData.fEndTime = fCurrTime + fCooltimeSec;
        newData.dwTotalTime = static_cast<int>(fCooltimeSec * 1000.0f);
        newData.byType = 0;
        m_mapCooltimeList[nSkillGroup] = newData;
    }
}

// ============================================================================
// ResetCooltime - 重置指定技能的冷却时间
// Phase 6 新增
// ============================================================================
void CMySkillList::ResetCooltime(int nSkillGroup) {
    auto iter = m_mapCooltimeList.find(nSkillGroup);
    if (iter != m_mapCooltimeList.end()) {
        m_mapCooltimeList.erase(iter);
    }
}

// ============================================================================
// CheckSkillCondition - 检查技能使用条件
// Phase 6 新增
// ============================================================================
bool CMySkillList::CheckSkillCondition(TB_SKILL* pSkillTable) {
    if (!pSkillTable) {
        return false;
    }

    if (!m_pActor) {
        return false;
    }

    float fSkillCost = 0.0f;
    int nResult = IsCanUseSkill(pSkillTable, nullptr, &fSkillCost, false);

    return (nResult == SKILL_ERROR_SUCCESS);
}

// ============================================================================
// GetSkillLevel - 获取技能等级
// Phase 6 新增
// ============================================================================
int CMySkillList::GetSkillLevel(int nSkillGroup) {
    if (!m_pActor) {
        return 0;
    }

    // TODO: 获取技能组件并查询等级
    // CMover::GetGOC<CGocSkill>(m_pActor, &pSkillPtr, 0);
    // if (pSkillPtr) {
    //     std::tr1::shared_ptr<CSkill> pSkill = CGocSkill::GetHaveSkillGroup(pSkillPtr, nSkillGroup);
    //     if (pSkill) {
    //         return CSkill::GetLevel(pSkill.get());
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "GetSkillLevel - stub");
    return 0;
}

// ============================================================================
// GetSkillLevelByIndex - 通过索引获取技能等级
// Phase 6 新增
// ============================================================================
int CMySkillList::GetSkillLevelByIndex(int nIndex) {
    if (!m_pActor) {
        return 0;
    }

    if (nIndex < 0) {
        return 0;
    }

    // 遍历冷却列表，找到指定索引的技能
    int nCount = 0;
    for (const auto& pair : m_mapCooltimeList) {
        if (nCount == nIndex) {
            // 找到对应索引，获取技能等级
            return GetSkillLevel(pair.first);
        }
        ++nCount;
    }

    // 索引超出范围
    return 0;
}

// ============================================================================
// IsSkillLearned - 检查技能是否已学习
// Phase 6 新增
// ============================================================================
bool CMySkillList::IsSkillLearned(int nSkillGroup) {
    if (!m_pActor) {
        return false;
    }

    // TODO: 获取技能组件并检查是否拥有该技能
    // CMover::GetGOC<CGocSkill>(m_pActor, &pSkillPtr, 0);
    // if (pSkillPtr) {
    //     return CGocSkill::IsHaveBaseSkill(pSkillPtr, nSkillGroup);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "IsSkillLearned - stub");
    return false;
}

// ============================================================================
// LearnSkill - 学习新技能
// Phase 6 新增
// ============================================================================
bool CMySkillList::LearnSkill(int nSkillGroup, int nLevel) {
    if (!m_pActor) {
        return false;
    }

    // TODO: 获取技能组件并学习技能
    // CMover::GetGOC<CGocSkill>(m_pActor, &pSkillPtr, 0);
    // if (pSkillPtr) {
    //     return CGocSkill::LearnSkill(pSkillPtr, nSkillGroup, nLevel);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "LearnSkill - stub");
    return false;
}

// ============================================================================
// ResetSkill - 重置技能到默认状态
// Phase 6 新增
// ============================================================================
void CMySkillList::ResetSkill(int nSkillGroup) {
    if (!m_pActor) {
        return;
    }

    // 移除冷却时间
    ResetCooltime(nSkillGroup);

    // 如果这是当前正在使用的技能，取消使用状态
    if (m_nCurrentSkillID == nSkillGroup) {
        m_bUsingSkill = false;
        m_nCurrentSkillID = 0;
    }

    // TODO: 重置技能组件中的技能状态
    // CMover::GetGOC<CGocSkill>(m_pActor, &pSkillPtr, 0);
    // if (pSkillPtr) {
    //     CGocSkill::ResetSkill(pSkillPtr, nSkillGroup);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ResetSkill - stub");
}

// ============================================================================
// GetSkillList - 获取技能列表
// Phase 6 新增
// ============================================================================
const std::map<int, tagCOOLTIME>& CMySkillList::GetSkillList() const {
    return m_mapCooltimeList;
}

// ============================================================================
// IsUsingSkill - 检查是否正在使用技能
// Phase 6 新增
// ============================================================================
bool CMySkillList::IsUsingSkill() const {
    return m_bUsingSkill;
}

// ============================================================================
// GetCurrentSkill - 获取当前技能ID
// Phase 6 新增
// ============================================================================
int CMySkillList::GetCurrentSkill() const {
    return m_nCurrentSkillID;
}

// ============================================================================
// GetSkillState - 获取技能状态
// Phase 6 新增
// 返回值:
//   0 = 未学习
//   1 = 可用
//   2 = 冷却中
//   3 = 正在使用
// ============================================================================
int CMySkillList::GetSkillState(int nSkillGroup) const {
    // 检查是否正在使用
    if (m_bUsingSkill && m_nCurrentSkillID == nSkillGroup) {
        return 3;  // 正在使用
    }

    // 检查是否在冷却中
    auto iter = m_mapCooltimeList.find(nSkillGroup);
    if (iter != m_mapCooltimeList.end()) {
        // 获取当前时间并检查
        VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
        float fCurrTime = pTimer->GetTimeDifference();
        if (iter->second.fEndTime > fCurrTime) {
            return 2;  // 冷却中
        }
    }

    // 检查是否已学习
    // TODO: 检查技能组件
    // if (IsSkillLearned(nSkillGroup)) {
    //     return 1;  // 可用
    // }

    return 0;  // 未学习
}

// ============================================================================
// IsCooltime - 检查是否在冷却中
// Phase 6 新增
// ============================================================================
bool CMySkillList::IsCooltime(int nSkillGroup) const {
    auto iter = m_mapCooltimeList.find(nSkillGroup);
    if (iter == m_mapCooltimeList.end()) {
        return false;
    }

    // 获取当前时间并检查
    VDefaultTimer* pTimer = ThreadLocalData::GetTimer();
    float fCurrTime = pTimer->GetTimeDifference();
    return (iter->second.fEndTime > fCurrTime);
}

// ============================================================================
// ResetAllCooltime - 重置所有冷却时间
// Phase 6 新增
// ============================================================================
void CMySkillList::ResetAllCooltime() {
    m_mapCooltimeList.clear();
    m_fGlobalCooltime[0] = 0.0f;
    m_fGlobalCooltime[1] = 0.0f;
}

// ============================================================================
// Chain Attack System Functions
// ============================================================================

// ProcessChain: IDA 0x1402B9730
// Process chain attack targets and store hit information
void CMySkillList::ProcessChain(
    CMoverEx* pMover,
    hkvVec3* vPos,
    hkvVec3* vDir,
    SSkillInfo* sSkillInfo,
    CMoverEx* pProjTarget,
    std::uint8_t byDamageType,
    int iTargetOrder,
    bool bPenetrate,
    std::uint8_t byHitPartsIndex,
    bool bTrapExplode,
    bool bHitWall)
{
    if (!pMover || !pProjTarget || !sSkillInfo) {
        return;
    }
    
    if (pMover != m_pActor) {
        return;
    }
    
    // Get skill table
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) return;
    
    TB_SKILL* pSkillTable = pServer->GetResourceMgr().GetTB_SKILL(sSkillInfo->nSkillID);
    if (!pSkillTable) return;
    
    AttackJudgmentTrigger* pActionEvent = sSkillInfo->pTrigger;
    if (!pActionEvent) return;
    
    // Check if target is enemy for chain
    if (!pMover->IsEnemyForChain(pProjTarget)) {
        return;
    }
    
    // Check if target can be hit
    int nPassiveType = pSkillTable->Passive_Type;
    if (!pProjTarget->IsCanHit(pActionEvent->sReactionInfo.iTargetStatus, nPassiveType)) {
        return;
    }
    
    // Get target ID
    std::uint32_t nID = pProjTarget->GetQuestID();
    
    // Check if already in target list
    for (int idx = 0; idx < 100; ++idx) {
        if (m_dwAttackTarget[idx] == nID) {
            return;
        }
    }
    
    // Find empty slot
    int idxa = 0;
    for (idxa = 0; idxa < 100 && m_dwAttackTarget[idxa]; ++idxa) {
        // Continue searching
    }
    
    if (idxa < 100) {
        // Add target to list
        AddSkillTarget(idxa, nID, pActionEvent->sReactionInfo.iReactionType, byHitPartsIndex);
        
        // Store chain hit info
        memcpy(&m_stChainHitInfo[idxa], vPos, sizeof(hkvVec3));
        memcpy(&m_stChainHitInfo[idxa].vHitDir, vDir, sizeof(hkvVec3));
        m_stChainHitInfo[idxa].iTargetOrder = iTargetOrder;
        m_stChainHitInfo[idxa].bHitWall = bHitWall;
    }
    
    // Store chain skill info
    memcpy(&m_stChainSkillInfo, sSkillInfo, sizeof(SSkillInfo));
}

// SendChainResult: IDA 0x1402B9A50
// Send chain attack results to targets
void CMySkillList::SendChainResult(CMoverEx* pMover) {
    if (!pMover) return;
    
    // Get skill table
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) return;
    
    TB_SKILL* pSkillTable = pServer->GetResourceMgr().GetTB_SKILL(m_stChainSkillInfo.nSkillID);
    if (!pSkillTable) return;
    
    AttackJudgmentTrigger* pActionEvent = m_stChainSkillInfo.pTrigger;
    if (!pActionEvent) return;
    
    // Count attack targets
    std::uint8_t bAttackTargetCnt = 0;
    for (int i = 0; i < 100; ++i) {
        if (m_dwAttackTarget[i]) {
            ++bAttackTargetCnt;
        }
    }
    
    if (pMover != m_pActor) {
        return;
    }
    
    // Process each target
    std::uint16_t wContinousHit = 0;
    for (int j = 0; j < bAttackTargetCnt; ++j) {
        CMoverEx* pTarget = reinterpret_cast<CMoverEx*>(CMover::GetMoverObject(m_dwAttackTarget[j]));
        if (!pTarget) {
            m_stAttackDamage[j].byDamageFlag = 0;
            m_stAttackDamage[j].nDamage = 0;
            continue;
        }
        
        // Calculate damage if rate > 0
        if (pActionEvent->sReactionInfo.fDamageRate > 0.0f || pActionEvent->sReactionInfo.iReactionType) {
            // Apply chain damage multiplier
            int fChainDamageRate = 100;  // Base rate
            if (m_stChainHitInfo[j].iTargetOrder > 0) {
                fChainDamageRate = static_cast<int>(pActionEvent->sConnectionInfo.fDamageMutiple);
            }
            
            // Calculate damage
            // TODO: Call CalcTargetDamage when fully implemented
            pMover->CalcTargetDamage(pTarget, j, pMover->GetAllowAbsorbSG(), 
                                    pSkillTable, pActionEvent, fChainDamageRate);
            
            // Check for combo triggers
            CMoverEx* pOwner = pMover->GetOwnerPlayer();
            if (pOwner == pMover || (pOwner && pMover->IsPlayer())) {
                // Check monster combo or akashic combo
                if (pMover->IsMonsterCombo() || pMover->IsComboAkashic()) {
                    wContinousHit = pOwner->CheckContinousAttack(bAttackTargetCnt);
                }
            } else if (pMover->IsPlayer()) {
                wContinousHit = pMover->CheckContinousAttack(1);
            }
        }
    }
    
    // Apply skill damage frame
    pMover->ApplySkillDamageFrame(m_stChainSkillInfo.nSkillID, pActionEvent->EventID,
                                   bAttackTargetCnt, &m_stChainHitInfo[0].vHitPos,
                                   0.0f, wContinousHit, false, false);
    
    // Send skill action packet
    pMover->send_eSUB_CMD_ACTION_SKILL(pMover, m_stChainSkillInfo.nSkillID,
                                        pActionEvent->EventID, &m_stChainHitInfo[0].vHitPos,
                                        bAttackTargetCnt, wContinousHit, 0);
}

// CalcChainSkillTarget: IDA 0x1402BF7B0
// Calculate chain skill targets within area
void CMySkillList::CalcChainSkillTarget(
    CMover* pMover,
    tagATTACK_AREA* stAreaInfo,
    AttackJudgmentTrigger* pActionEvent,
    SSkillInfo* SkillInfo,
    std::uint8_t* bAttackTargetCnt,
    TB_SKILL* pSkillRef)
{
    if (!pMover || !pActionEvent || !pSkillRef || !SkillInfo) {
        return;
    }
    
    // Get position
    hkvVec3 vPos = pMover->GetPosition();
    
    // Find closest target
    CMoverEx* pClosestTarget = nullptr;
    float fClosestDistance = 1000000.0f;
    
    // Scan for nearby actors
    std::vector<CMover*> vecGameObjList;
    pMover->GetArea()->ScanGridOrigin(pMover, 2, 3, &vecGameObjList);
    
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMoverEx* pOtherActor = reinterpret_cast<CMoverEx*>(*it);
        if (!pOtherActor) continue;
        
        // Check if target is valid
        if (!pOtherActor->IsLive()) continue;
        if (pOtherActor->IsStatus(2)) continue;
        
        // Check reaction target
        if (!pMover->CheckReactionTarget(pActionEvent->sReactionInfo.iTargetType, pOtherActor, true)) {
            continue;
        }
        
        // Check actor type
        E_ACTOR_TYPE eType = pOtherActor->GetType();
        if (eType != eActorPlayer && eType != eActorMonster) {
            continue;
        }
        
        // Check if in attack area
        if (!IsInAttackArea(pOtherActor, stAreaInfo, pActionEvent->sReactionInfo.iTargetStatus, pSkillRef->Passive_Type)) {
            continue;
        }
        
        // Check target count limit
        if (*bAttackTargetCnt >= pSkillRef->Target_Damage_Count) {
            continue;
        }
        
        // Calculate distance
        hkvVec3 vTargetPos = pOtherActor->GetPosition();
        hkvVec3 vDiff = vTargetPos - vPos;
        float fDist = vDiff.GetLength();
        
        if (fClosestDistance > fDist) {
            pClosestTarget = pOtherActor;
            fClosestDistance = fDist;
        }
    }
    
    // Create chain lightning object
    hkvVec3 vDir(0.0f, -1.0f, 0.0f);
    
    XGameServer* pServer = XGameServer::Instance();
    TB_SKILL* pSkillTable = pServer->GetResourceMgr().GetTB_SKILL(SkillInfo->nSkillID);
    if (pSkillTable) {
        vDir *= pSkillTable->Skill_Range_Max;
        
        VChainLightningObject* pChainObj = CreateChainLightningObject(vPos, vDir, pMover, pActionEvent, pClosestTarget);
        if (pChainObj) {
            pChainObj->SetSkillInfo(SkillInfo);
            m_vChainLightningObject.push_back(pChainObj);
            
            // Get target ID
            std::uint32_t dwTargetID = -1;
            if (pClosestTarget) {
                dwTargetID = pClosestTarget->GetQuestID();
            }
            
            // Send chain packet
            int nSessionID = pChainObj->GetSessionID();
            pMover->send_eSUB_CMD_CHAIN(pMover, SkillInfo->nSkillID, pActionEvent->EventID,
                                        &vPos, &vDir, nSessionID, dwTargetID);
        }
    }
}
