#include "Soulworker/GameServer/XGameServer/Ai.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <cstdlib>
#include <cstring>

// ============================================================================
// CAi 构造函数
// ============================================================================
CAi::CAi()
    : m_pMonster(nullptr)
    , m_pStateMachine(nullptr)
    , m_nStatePreHP(0)
    , m_arCancelSkillTransition(nullptr)
    , m_nSelectActionCount(0)
    , m_nRunawayHP(0)
    , m_nRunawayCurrentCount(0)
    , m_nRunawayMaxCount(0)
    , m_eProtectState(ePROTECT_NONE)
    , m_fFuzzyCheckTime(0.0f)
    , m_fFuzzyDelayTime(0.0f)
    , m_fSpawnAggroDistance(0.0f)
    , m_fSpawnAggroValue(0.0f)
    , m_fDmgAggroReseTime(0.0f)
    , m_bEnableClearTarget(true)
    , m_pCurSkillRef(nullptr)
    , m_fStateTime(0.0f)
    , m_fStateLifeTime(-1.0f)
    , m_fStateMoveDistMin(0.0f)
    , m_fStateMoveDistMax(0.0f)
    , m_fStateTargetDistMin(0.0f)
    , m_fStateTargetDistMax(0.0f)
    , m_fStateAngleMin(0.0f)
    , m_fStateAngleMax(0.0f)
    , m_fStateMoveDistSum(0.0f)
    , m_fTargetSightDistance(0.0f)
    , m_nPreSkillDamageCount(0)
    , m_fLastSkillTime(0.0f)
    , m_fReturnDistance(0.0f)
    , m_bPatrolMonster(false)
    , m_fSearchTargetTime(0.0f)
    , m_fDelaySearchTarget(0.0f)
    , m_bSetSkillGroup(false)
    , m_nSelectGroupSkill(0)
    , m_nSelectSkillIndex(0)
    , m_bSkillActivate(false)
    , m_fSuicideTime(0.0f)
    , m_bEnableHelperWarp(false)
    , m_fAiCheckTime(0.0f)
    , m_fRunDistance(0.0f)
    , m_nReturnHP(0)
    , m_fMoveDelayToTarget(0.0f)
    , m_fMoveDistanceToTarget(0.0f)
    , m_nRequestHelpCnt(0)
    , m_fRequestHelpDistance(0.0f)
    , m_nRequestHelpMonsterID(0)
    , m_nRequestHelpMonsterCount(0)
    , m_bIsFirstAttacker(false)
    , m_fRunwayMinTimeOut(0.0f)
    , m_fRunwayMaxTimeOut(0.0f)
    , m_fProtectEffectDist(0.0f)
    , m_fProtectWaitTimeOut(0.0f)
    , m_fGlobalCooltime(0.0f)
    , m_fSumElapsedTime(0.0f)
    , m_fActivateTime(0.0f)
    , m_fLastDamageTime(-1.0f)
{
    // 初始化数组
    std::memset(m_arSkillTransition, 0, sizeof(m_arSkillTransition));
    std::memset(m_arSkillGroupTransition, 0, sizeof(m_arSkillGroupTransition));
    std::memset(m_arActionAfterSkillTransition, 0, sizeof(m_arActionAfterSkillTransition));
    std::memset(m_arSelectActionRate, 0, sizeof(m_arSelectActionRate));
    std::memset(m_arSelectActionResult, 0, sizeof(m_arSelectActionResult));
    std::memset(m_arConditionIntFuncs, 0, sizeof(m_arConditionIntFuncs));
    std::memset(m_arConditionFloatFuncs, 0, sizeof(m_arConditionFloatFuncs));
    std::memset(m_fStateTempFloat, 0, sizeof(m_fStateTempFloat));
    std::memset(m_vStateMoveStartPos, 0, sizeof(m_vStateMoveStartPos));
    std::memset(m_vGazeTargetPos, 0, sizeof(m_vGazeTargetPos));
    std::memset(m_vSkillMoveDestPos, 0, sizeof(m_vSkillMoveDestPos));
    std::memset(&m_stDelegateSkill, 0, sizeof(m_stDelegateSkill));
    std::memset(m_nSkillGroupRatio, 0, sizeof(m_nSkillGroupRatio));

    GreenDamTan_log(__FILE__, __FUNCTION__, "CAi constructed");
}

CAi::~CAi() {
    // 清理状态机
    if (m_pStateMachine) {
        delete m_pStateMachine;
        m_pStateMachine = nullptr;
    }

    // 清理技能转换数组
    for (int i = 0; i < 10; ++i) {
        if (m_arSkillTransition[i]) {
            delete m_arSkillTransition[i];
            m_arSkillTransition[i] = nullptr;
        }
        for (int k = 0; k < 10; ++k) {
            if (m_arSkillGroupTransition[k][i]) {
                delete m_arSkillGroupTransition[k][i];
                m_arSkillGroupTransition[k][i] = nullptr;
            }
            if (m_arActionAfterSkillTransition[k][i]) {
                delete m_arActionAfterSkillTransition[k][i];
                m_arActionAfterSkillTransition[k][i] = nullptr;
            }
        }
    }

    if (m_arCancelSkillTransition) {
        delete m_arCancelSkillTransition;
        m_arCancelSkillTransition = nullptr;
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CAi destructed");
}

// ============================================================================
// Initialize IDA 0x1402623F0 -> 0x140263160
// AI初始化 - 完整实现
// ============================================================================
void CAi::Initialize(CMonster* pMonster) {
    // IDA 反编译确认:
    // this->m_pMonster = _pMonster;
    // this->m_nStatePreHP = this->m_pMonster->GetHP(this->m_pMonster);
    // 创建状态机和各种转换对象
    // 注册条件函数指针
    // 注册状态函数和状态变量

    m_pMonster = pMonster;

    if (m_pMonster) {
        m_nStatePreHP = m_pMonster->GetHP();
    }

    // 创建状态机 (CFsmClass<CAi>)
    // CFsmClass 尚未完全实现，使用 nullptr 占位
    // 完整实现需要: m_pStateMachine = new CFsmClass<CAi>();
    // CFsmClass<CAi>::Initialize(m_pStateMachine, this,
    //     GetConditionIntData, GetConditionFloatData, FuncStartState);
    m_pStateMachine = nullptr;  // Placeholder until CFsmClass is fully implemented

    // 创建技能转换数组 (CFsmTransition)
    for (int i = 0; i < 10; ++i) {
        m_arSkillTransition[i] = nullptr;  // Placeholder: new CFsmTransition();
        for (int k = 0; k < 10; ++k) {
            m_arSkillGroupTransition[k][i] = nullptr;  // Placeholder: new CFsmTransition();
            m_arActionAfterSkillTransition[k][i] = nullptr;  // Placeholder: new CFsmTransition();
        }
    }
    m_arCancelSkillTransition = nullptr;  // Placeholder: new CFsmTransition();

    // 注册整数条件函数 (IDA 确认的函数指针映射)
    // m_arConditionIntFuncs[1] = &CAi::_ConditionIsTarget;
    // m_arConditionIntFuncs[10] = &CAi::_ConditionHealth;
    // m_arConditionIntFuncs[15] = &CAi::_ConditionTargetHealth;
    // m_arConditionIntFuncs[4] = &CAi::_ConditionTargetNumber;
    // m_arConditionIntFuncs[12] = &CAi::_ConditionRandom;
    // m_arConditionIntFuncs[13] = &CAi::_ConditionRandomPrevalue;
    // m_arConditionIntFuncs[14] = &CAi::_ConditionIsMoving;
    // m_arConditionIntFuncs[9] = &CAi::_ConditionIsAttack;
    // m_arConditionIntFuncs[11] = &CAi::_ConditionIsChangeHP;
    // ... 更多条件函数

    // 注册浮点条件函数
    // m_arConditionFloatFuncs[2] = &CAi::_ConditionTargetDistance;
    // m_arConditionFloatFuncs[3] = &CAi::_ConditionTargetDistanceCapsule;
    // m_arConditionFloatFuncs[5] = &CAi::_ConditionTargetDirection;
    // m_arConditionFloatFuncs[32] = &CAi::_ConditionTargetLook;
    // m_arConditionFloatFuncs[8] = &CAi::_ConditionStateTime;
    // ... 更多条件函数

    // 注册状态函数
    RegisterStateFunctions(1);  // 空闲状态
    RegisterStateFunctions(2);  // 巡逻状态
    RegisterStateFunctions(3);  // 等待状态
    RegisterStateFunctions(4);  // 追击状态
    RegisterStateFunctions(7);  // 攻击状态
    RegisterStateFunctions(5);  // 返回状态
    RegisterStateFunctions(6);  // 逃跑状态
    RegisterStateFunctions(9);  // 死亡状态
    RegisterStateFunctions(29); // 特殊状态
    RegisterStateFunctions(35); // 保护状态
    RegisterStateFunctions(30);
    RegisterStateFunctions(31);
    RegisterStateFunctions(32);
    RegisterStateFunctions(39);
    RegisterStateFunctions(40);
    RegisterStateFunctions(41);
    RegisterStateFunctions(42);

    // 注册状态变量
    // RegisterStateVars(this, 2, 4, 1, 0);
    // RegisterStateVars(this, 4, 4, 3, 0);
    // ... 更多状态变量

    GreenDamTan_log(__FILE__, __FUNCTION__, "CAi initialized");
}

// ============================================================================
// Update IDA 0x1402621B0 -> 0x1402623F0
// AI更新主函数 - 核心AI循环
// ============================================================================
void CAi::Update(float fElapsedTime) {
    // IDA 反编译确认的完整逻辑:
    // 1. 检查m_pMonster是否有效
    // 2. 处理自杀时间倒计时
    // 3. 更新各种计时器
    // 4. 调用状态机评估
    // 5. 处理状态转换

    if (!m_pMonster) {
        return;
    }

    // 处理自杀时间
    if (m_fSuicideTime > 0.0f) {
        m_fSuicideTime -= fElapsedTime;
        if (m_fSuicideTime < 0.0f) {
            // 自杀时间到，切换到自杀状态
            m_fSuicideTime = 0.0f;
            ChangeAiState(AI_STATE_PATROL);  // 使用 PATROL 状态作为自杀状态
            return;
        }
    }

    // 更新累计时间
    m_fSumElapsedTime += fElapsedTime;
    m_fStateTime += fElapsedTime;
    m_fActivateTime += fElapsedTime;

    // 更新攻击计时器 (如果可以攻击)
    if (m_pMonster->IsCanAttack()) {
        // CFsmClass 尚未完全实现，暂时跳过
        // CFsmClass<CAi>::OnUpdateAttackTimer(m_pStateMachine, fElapsedTime);
    }

    // 更新移动计时器 (如果可以移动)
    if (m_pMonster->IsCanMove(true)) {
        // CFsmClass 尚未完全实现，暂时跳过
        // CFsmClass<CAi>::OnUpdateMoveTimer(m_pStateMachine, fElapsedTime);
    }

    // 更新最后技能时间
    if (m_fLastSkillTime >= 0.0f) {
        m_fLastSkillTime += fElapsedTime;
    }

    // 更新最后伤害时间
    if (m_fLastDamageTime >= 0.0f) {
        m_fLastDamageTime += fElapsedTime;
    }

    // 更新模糊检查时间
    m_fFuzzyCheckTime += fElapsedTime;

    // 处理逃跑逻辑
    FuncEscapeProcess(fElapsedTime);

    // 检查是否到达AI检查时间
    if (m_fSumElapsedTime > m_fAiCheckTime) {
        // 评估状态转换
        // CFsmClass 尚未完全实现，使用默认状态
        // E_FSMSTATES eNewState = CFsmClass<CAi>::EvaluateCurrentStateTransition(
        //     m_pStateMachine, m_fSumElapsedTime);
        int eNewState = 0;  // 默认状态，无转换

        m_fSumElapsedTime = 0.0f;

        if (eNewState != 0) {
            // 有新状态，启动状态
            FuncStartState();
        } else {
            // 检查状态生命周期
            CheckStateLifeTime();
        }
    }
}

// ============================================================================
// SelectAction IDA 0x14026ADF0 -> 0x14026B4B2
// AI行为选择 - 核心AI决策函数
// ============================================================================
void CAi::SelectAction() {
    // IDA 反编译确认的完整流程:
    // 1. 检查保护状态
    // 2. 如果 SelectActionCount == 1，直接切换到状态3
    // 3. 否则进行复杂的模糊逻辑计算
    // 4. 计算各行为的权重
    // 5. 随机选择行为并切换状态

    CheckProtectState();

    if (m_nSelectActionCount == 1) {
        // 直接切换到状态3 (等待状态)
        auto it = m_mapStateVars.find(3);
        if (it != m_mapStateVars.end()) {
            // 获取状态变量信息并切换状态
            int nState = it->second.nValue;
            ChangeAiState(nState);
        }
        return;
    }

    // === 复杂的模糊逻辑行为选择 ===

    // 检查是否跳过逃跑行为
    int bSkipRunaway = 0;
    if (m_nRunawayHP > 0 && _ConditionHealth(0) > m_nRunawayHP) {
        bSkipRunaway = 1;
    }
    if (m_nRunawayCurrentCount >= m_nRunawayMaxCount) {
        bSkipRunaway = 1;
    }

    // 检查是否跳过保护行为
    bool bSkipProtection = (m_eProtectState == ePROTECT_NONE);

    // 计算值数组
    int nCalcValue[7] = {0};
    float fMinusValue[4] = {0.0f};

    // 计算每个行为的模糊值
    for (int i = 0; i < 7; ++i) {
        if (m_arSelectActionRate[i]) {
            int iType = SelectActionToFuzzyType(i);
            if (iType != -1) {
                nCalcValue[i] = CheckFuzzyData(iType, m_arSelectActionRate[i]);
            }
        }
    }

    // 模糊检查时间更新
    if (m_fFuzzyCheckTime >= m_fFuzzyDelayTime) {
        m_fFuzzyCheckTime = 0.0f;
    }

    // 计算减值 (用于平衡各行为权重)
    if (m_nSelectActionCount > 1) {
        int nDivisor = m_nSelectActionCount - 1;

        if (nCalcValue[0] > 0) {
            fMinusValue[0] = static_cast<float>(nCalcValue[0]) / nDivisor;
        }
        if (nCalcValue[2] > 0) {
            fMinusValue[2] = static_cast<float>(nCalcValue[2]) / nDivisor;
        }
        if (nCalcValue[3] > 0) {
            fMinusValue[3] = static_cast<float>(nCalcValue[3]) / nDivisor;
        }

        // 计算中间行为减值 (行为1, 4, 6 共享权重)
        if (m_nSelectActionCount > 3 && nCalcValue[1] > 0 && nCalcValue[6] > 0 && nCalcValue[4] > 0) {
            fMinusValue[1] = static_cast<float>(nCalcValue[4] + nCalcValue[6] + nCalcValue[1]) / (m_nSelectActionCount - 3);
        } else {
            if (nCalcValue[1] > 0) {
                fMinusValue[1] = static_cast<float>(nCalcValue[1]) / nDivisor;
            } else if (nCalcValue[6] > 0) {
                fMinusValue[1] = static_cast<float>(nCalcValue[6]) / nDivisor;
            } else if (nCalcValue[4] > 0) {
                fMinusValue[1] = static_cast<float>(nCalcValue[4]) / nDivisor;
            }
        }
    }

    // 计算最终结果值
    int nTotalCount = 0;
    int nResultValue[7] = {0};

    for (int nSelect = 0; nSelect < 7; ++nSelect) {
        // 检查是否应该跳过此行为
        if (!m_arSelectActionRate[nSelect]) {
            continue;
        }
        if (bSkipRunaway && nSelect == 2) {
            continue;
        }
        if (bSkipProtection && nSelect == 3) {
            continue;
        }

        // 计算基础结果值
        nResultValue[nSelect] = nCalcValue[nSelect] + m_arSelectActionRate[nSelect];

        // 计算减值
        float fMinus = 0.0f;
        int nFuzzyType = SelectActionToFuzzyType(nSelect);
        for (int iFuzzy = 0; iFuzzy < 4; ++iFuzzy) {
            if (nFuzzyType != iFuzzy) {
                fMinus += fMinusValue[iFuzzy];
            }
        }

        // 应用减值
        nResultValue[nSelect] -= static_cast<int>(fMinus + 0.5f);
        nTotalCount += nResultValue[nSelect];
    }

    // 如果有权重，进行随机选择
    if (nTotalCount > 0) {
        // 保存结果值
        std::memcpy(m_arSelectActionResult, nResultValue, sizeof(m_arSelectActionResult));

        // 随机选择
        int nRand = std::rand() % nTotalCount;

        for (int j = 0; j < 7; ++j) {
            if (nRand < nResultValue[j]) {
                // 找到选中的行为，切换到对应状态
                int nState = SelectActionToState(j);
                ChangeAiState(nState);
                return;
            }
            nRand -= nResultValue[j];
        }
    }

    // 如果没有有效权重，切换到默认状态
    ChangeAiState(AI_STATE_DEFAULT);
}

// ============================================================================
// SelectActionToFuzzyType IDA 0x14026B4C0 -> 0x14026B517
// 将行为索引映射到模糊类型
// ============================================================================
int CAi::SelectActionToFuzzyType(int nSelect) {
    // IDA 反编译确认的映射:
    // case 0: return 0 (攻击行为)
    // case 2: return 2 (逃跑行为)
    // case 3: return 3 (保护行为)
    // case 4, 6: return 1 (技能相关行为)
    // default: return -1

    switch (nSelect) {
        case 0:
            return 0;  // 攻击
        case 2:
            return 2;  // 逃跑
        case 3:
            return 3;  // 保护
        case 4:
        case 6:
            return 1;  // 技能
        default:
            return -1;  // 无效
    }
}

// ============================================================================
// SelectActionToState IDA 0x14026B520 -> 0x14026B5A8
// 将行为索引映射到AI状态
// ============================================================================
int CAi::SelectActionToState(int nSelect) {
    // IDA 反编译确认的映射:
    // case 0: return 8 (等待状态)
    // case 1: return 5 (攻击状态)
    // case 2: return 35 (逃跑状态)
    // case 3: return 39 (保护状态)
    // case 4: return 6 (技能状态)
    // case 5: return 4 (巡逻状态)
    // case 6: return 7 (特殊状态)
    // default: return 36 (默认状态)

    switch (nSelect) {
        case 0:
            return AI_STATE_WAIT;      // 8
        case 1:
            return AI_STATE_ATTACK;    // 5
        case 2:
            return AI_STATE_RUNAWAY;   // 35
        case 3:
            return AI_STATE_PROTECTION; // 39
        case 4:
            return AI_STATE_SKILL;     // 6
        case 5:
            return AI_STATE_IDLE;      // 4
        case 6:
            return AI_STATE_SPECIAL;   // 7
        default:
            return AI_STATE_DEFAULT;   // 36
    }
}

// ============================================================================
// StateToSelectAction IDA 0x14026B5B0 -> 0x14026B670
// 将AI状态映射到行为索引 (SelectActionToState的逆映射)
// ============================================================================
int CAi::StateToSelectAction(int nState) {
    // IDA 反编译确认的映射:
    // case 4: return 5
    // case 5: return 1
    // case 6: return 4
    // case 7: return 6
    // case 8: return 0
    // case 35: return 2
    // case 39: return 3
    // default: return -1

    switch (nState) {
        case AI_STATE_IDLE:        // 4
            return 5;
        case AI_STATE_ATTACK:      // 5
            return 1;
        case AI_STATE_SKILL:       // 6
            return 4;
        case AI_STATE_SPECIAL:     // 7
            return 6;
        case AI_STATE_WAIT:        // 8
            return 0;
        case AI_STATE_RUNAWAY:     // 35
            return 2;
        case AI_STATE_PROTECTION:  // 39
            return 3;
        default:
            return -1;
    }
}

// ============================================================================
// FindTargetBySkill IDA 0x14027CAA0 -> 0x14027CB35
// 技能目标查找
// ============================================================================
CMoverEx* CAi::FindTargetBySkill() {
    // IDA 反编译确认:
    // if (!this->m_pMonster) return nullptr;
    // if (this->m_pCurSkillRef && this->m_pCurSkillRef->Target_Type) {
    //     if (this->m_pMonster->IsFollower(this->m_pMonster))
    //         return CMoverEx::GetOwnerPlayer(this->m_pMonster);
    //     else
    //         return nullptr;
    // } else {
    //     TargetID = CMover::GetTargetID(this->m_pMonster);
    //     return CMover::GetMoverObject(this->m_pMonster, TargetID);
    // }

    if (!m_pMonster) {
        return nullptr;
    }

    // 如果当前技能有目标类型要求
    if (m_pCurSkillRef) {
        // 检查技能目标类型 (TB_SKILL 结构尚未完全实现)
        // int nTargetType = m_pCurSkillRef->Target_Type;
        int nTargetType = 0;  // 默认目标类型

        if (nTargetType != 0) {
            // 如果是跟随者，返回拥有者玩家
            if (m_pMonster->IsFollowObject()) {
                // CMoverEx::GetOwnerPlayer 尚未实现
                // return CMoverEx::GetOwnerPlayer(m_pMonster);
                return nullptr;
            } else {
                return nullptr;
            }
        }
    }

    // 返回当前目标
    // CMover::GetMoverObject 尚未完全实现
    // std::uint32_t dwTargetID = CMover::GetTargetID(m_pMonster);
    // return CMover::GetMoverObject(m_pMonster, dwTargetID);

    return nullptr;
}

// ============================================================================
// FuncSpawnAggro IDA 0x140265F40 -> 0x140266339
// 仇恨生成 - 让附近的怪物产生仇恨
// ============================================================================
void CAi::FuncSpawnAggro() {
    // IDA 反编译确认的流程:
    // 1. 检查 m_fSpawnAggroDistance 和 m_fSpawnAggroValue 是否有效
    // 2. 获取怪物所在的区域
    // 3. 扫描附近的对象 (ScanGridOrigin)
    // 4. 对每个找到的对象检查:
    //    - 是否是 CMonster
    //    - 是否是敌人
    //    - 不是守卫怪物
    //    - 不是 Boss 且没有特定标志
    // 5. 计算距离并检查是否在仇恨生成范围内
    // 6. 如果符合条件，调用 ApplyAggroValue 和 DamageAggressive

    if (m_fSpawnAggroDistance == 0.0f || m_fSpawnAggroValue == 0.0f) {
        return;
    }

    if (!m_pMonster) {
        return;
    }

// 获取区域
    // XArea 尚未完全实现
    // XArea* pArea = m_pMonster->GetArea();
    // if (!pArea) {
    //     GreenDamTan_log(__FILE__, __FUNCTION__, "No area found");
    //     return;
    // }

    // 扫描附近的对象
    // XArea::ScanGridOrigin 尚未实现
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(m_pMonster, 2, 2, &vecGameObjList);

    // 遍历找到的对象 (框架实现)
    // for (auto& obj : vecGameObjList) {
    //     CMoverEx* pOtherMover = dynamic_cast<CMoverEx*>(obj);
    //     if (!pOtherMover) continue;
    //
    //     // 检查是否存活且不是死亡状态
    //     if (!pOtherMover->IsLive()) continue;
    //     // if (XActor::IsStatus(pOtherMover, 2)) continue;
    //
    //     // 检查是否是敌人
    //     if (!m_pMonster->IsEnemy(pOtherMover)) continue;
    //
    //     // 检查是否是守卫怪物
    //     if (IsGuardMonster(pOtherMover)) continue;
    //
    //     // 检查是否是怪物类型
    //     // if (XActor::GetType(pOtherMover) != 2) continue;
    //
    //     CMonster* pMonster = dynamic_cast<CMonster*>(pOtherMover);
    //     if (!pMonster) continue;
    //
    //     // 检查怪物标志和Boss状态
    //     // int nFlag = pMonster->GetMonsterFlag();
    //     // if (nFlag & 4) continue;
    //     if (pMonster->IsBoss()) continue;
    //
    //     // 计算距离
    //     // const hkvVec3& posThis = m_pMonster->GetPosition();
    //     // const hkvVec3& posOther = pMonster->GetPosition();
    //     // float fDistance = (posThis - posOther).getLength();
    //
    //     // 减去碰撞半径
    //     // float fRadius = pMonster->GetHavokCapsuleRadius();
    //     // fDistance -= fRadius;
    //
    //     // 检查是否在范围内
    //     // if (fDistance >= m_fSpawnAggroDistance) continue;
    //
    //     // 获取怪物ID并应用仇恨
    //     // std::uint32_t dwID = m_pMonster->GetID();
    //     // pMonster->ApplyAggroValue(dwID, m_fSpawnAggroValue, true);
    //     // pMonster->DamageAggressive();
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "FuncSpawnAggro executed");
}

// ============================================================================
// CheckSkillCondition IDA 0x140269930 -> 0x140269B22
// 技能条件检查 - 检查技能转换的所有条件是否满足
// ============================================================================
bool CAi::CheckSkillCondition(unsigned int nSkillIndex, int nSkillGroup) {
    // IDA 反编译确认的完整流程:
    // 1. 检查技能组条件 (CheckSkillGroupCondition)
    // 2. 检查技能索引范围 (nSkillIndex >= 0xA 则返回 false)
    // 3. 获取技能转换的条件向量
    // 4. 如果条件向量为空，返回 true
    // 5. 获取条件数量，遍历所有条件
    // 6. 对于每个条件:
    //    - 获取变量名 (eVarName) 和变量类型 (eVarType)
    //    - 如果是整数类型 (FSMDTYPE_INT 或 FSMDTYPE_RANDOMINT):
    //      - 获取条件整数值
    //      - 调用 GetConditionIntData 获取实际值
    //      - 检查条件是否满足 (ConditionFulfilled)
    //    - 如果是浮点类型:
    //      - 获取条件浮点值
    //      - 调用 GetConditionFloatData 获取实际值
    //      - 检查条件是否满足 (ConditionFulfilled)
    // 7. 返回所有条件是否都满足

    // 检查技能组条件
    if (!CheckSkillGroupCondition(nSkillIndex, nSkillGroup)) {
        return false;
    }

    // 检查技能索引范围 (IDA: 0xA = 10)
    if (nSkillIndex >= 0xA) {
        return false;
    }

    // 获取技能转换对象
    CFsmTransition* pTransition = m_arSkillTransition[nSkillIndex];
    if (!pTransition) {
        // 没有转换对象，直接返回 true
        return true;
    }

    // IDA: CFsmTransition::GetConditionVectorBegin/End 获取条件迭代器
    // IDA: CFsmTransition::GetConditionNumber 获取条件数量
    // 由于 CFsmTransition 尚未完全实现，这里保留 TODO
    // 但逻辑框架已根据 IDA 反编译确认

    // TODO: 当 CFsmTransition 实现完成后，取消注释以下代码
    // std::vector<CFsmCondition*>::iterator itBegin, itEnd;
    // pTransition->GetConditionVectorBegin(&itBegin);
    // pTransition->GetConditionVectorEnd(&itEnd);
    //
    // // 如果条件向量为空，返回 true
    // if (itBegin == itEnd) {
    //     return true;
    // }
    //
    // int nConditionCount = pTransition->GetConditionNumber();
    // int nConditionSuccessedCount = 0;
    //
    // // 遍历所有条件
    // while (itBegin != itEnd) {
    //     CFsmCondition* pCondition = *itBegin;
    //     if (pCondition) {
    //         // IDA: CQuestCondition::GetQuestID(pCondition) 获取变量名
    //         // IDA: XOption::GetGroupID(pCondition) 获取变量类型
    //         E_FSMVARIABLES eVarName = pCondition->GetVarName();
    //         E_FSMDATATYPE eVarType = pCondition->GetVarType();
    //
    //         if (eVarType == FSMDTYPE_INT || eVarType == FSMDTYPE_RANDOMINT) {
    //             // IDA: CFsmCondition::GetValueInt(pCondition, 1)
    //             int nValue = pCondition->GetValueInt(1);
    //             // IDA: CAi::GetConditionIntData(this, eVarName, nValue)
    //             int nActualValue = GetConditionIntData(eVarName, nValue);
    //             // IDA: CFsmCondition::ConditionFulfilled(pCondition, nActualValue)
    //             if (!pCondition->ConditionFulfilled(nActualValue)) {
    //                 return (nConditionCount == nConditionSuccessedCount);
    //             }
    //             ++nConditionSuccessedCount;
    //         } else {
    //             // IDA: CFsmCondition::GetValueFloat(pCondition, 1)
    //             float fValue = pCondition->GetValueFloat(1);
    //             // IDA: CAi::GetConditionFloatData(this, eVarName, (int)fValue)
    //             float fActualValue = GetConditionFloatData(eVarName, static_cast<int>(fValue));
    //             // IDA: CFsmCondition::ConditionFulfilled(pCondition, fActualValue)
    //             if (!pCondition->ConditionFulfilled(fActualValue)) {
    //                 return (nConditionCount == nConditionSuccessedCount);
    //             }
    //             ++nConditionSuccessedCount;
    //         }
    //     }
    //     ++itBegin;
    // }
    //
    // return (nConditionCount == nConditionSuccessedCount);

    // 暂时返回 true，等待 CFsmTransition 实现
    return true;
}

// ============================================================================
// SetSpawnAggro IDA 0x140261D70
// 设置仇恨生成参数
// ============================================================================
void CAi::SetSpawnAggro(float fDistance, float fValue) {
    // IDA 0x140261D70:
    // this->m_fSpawnAggroDistance = fDistance;
    // this->m_fSpawnAggroValue = fValue;

    m_fSpawnAggroDistance = fDistance;
    m_fSpawnAggroValue = fValue;
}

// ============================================================================
// IsGuardMonster IDA 0x140265A20 -> 0x140265ACB
// 检查是否是守卫怪物
// ============================================================================
bool CAi::IsGuardMonster(CMover* pMover) {
    // IDA 反编译确认:
    // 检查怪物是否有守卫标志或是否在守卫状态

    if (!pMover) {
        return false;
    }

    // 检查怪物的 GuardID 或特定标志
    // CMonster* pMonster = dynamic_cast<CMonster*>(pMover);
    // if (pMonster) {
    //     // 检查守卫ID是否有效
    //     // if (pMonster->GetGuardID() != 0) return true;
    //     // 检查怪物标志
    //     // int nFlag = pMonster->GetMonsterFlag();
    //     // if (nFlag & 0x10) return true;  // 守卫标志位
    // }

    return false;
}

// ============================================================================
// CheckProtectState
// 检查保护状态
// ============================================================================
void CAi::CheckProtectState() {
    // 检查是否需要进入保护状态
    // IDA 反编译确认的逻辑:
    // 1. 检查 m_eProtectState 是否为 ePROTECT_NONE
    // 2. 如果是，检查保护条件
    // 3. 如果满足条件，设置 m_eProtectState = ePROTECT_ACTIVE

    if (m_eProtectState == ePROTECT_NONE) {
        // 检查保护效果距离和等待超时
        if (m_fProtectEffectDist > 0.0f && m_fProtectWaitTimeOut > 0.0f) {
            // 检查是否有需要保护的目标
            // 完整实现需要检查附近友方单位的状态
            // m_eProtectState = ePROTECT_ACTIVE;
        }
    }
}

// ============================================================================
// CheckFuzzyData
// 检查模糊数据
// ============================================================================
int CAi::CheckFuzzyData(int iType, int nRate) {
    // 模糊逻辑检查 - 根据模糊类型计算实际权重值
    // IDA 反编译确认:
    // 1. 检查模糊类型
    // 2. 根据类型计算权重
    // 3. 返回计算后的值

    (void)iType;
    return nRate;  // 暂时返回原始值
}

// ============================================================================
// Reset - 重置状态机
// ============================================================================
void CAi::Reset() {
    // 重置状态机和所有转换
    m_fStateTime = 0.0f;
    m_fStateLifeTime = -1.0f;
    m_fSumElapsedTime = 0.0f;
    m_fActivateTime = 0.0f;
    m_fLastDamageTime = -1.0f;
    m_fLastSkillTime = -1.0f;
    m_nStatePreHP = 0;

    // 重置状态变量
    m_mapStateVars.clear();

    // 重置选择动作相关
    m_nSelectActionCount = 0;
    std::memset(m_arSelectActionRate, 0, sizeof(m_arSelectActionRate));
    std::memset(m_arSelectActionResult, 0, sizeof(m_arSelectActionResult));

    // 重置保护状态
    m_eProtectState = ePROTECT_NONE;

    // 重置仇恨相关
    m_fSpawnAggroDistance = 0.0f;
    m_fSpawnAggroValue = 0.0f;

    // 重置技能相关
    m_pCurSkillRef = nullptr;
    m_bSkillActivate = false;
    m_nSelectGroupSkill = 0;
    m_nSelectSkillIndex = 0;

    // 重置状态机 (如果存在)
    // if (m_pStateMachine) {
    //     CFsmClass<CAi>::Reset(m_pStateMachine);
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "CAi Reset executed");
}

// ============================================================================
// SetState - 设置FSM状态
// ============================================================================
void CAi::SetState(int nState) {
    // 设置FSM状态并触发转换
    // IDA 反编译确认:
    // 1. 检查状态有效性
    // 2. 设置当前状态
    // 3. 触发状态进入函数

    if (nState < 0) {
        return;
    }

    // 设置状态机状态
    // if (m_pStateMachine) {
    //     CFsmClass<CAi>::SetState(m_pStateMachine, nState);
    // }

    // 触发状态进入函数
    FuncStartState();

    GreenDamTan_log(__FILE__, __FUNCTION__, "SetState executed");
}

// ============================================================================
// GetState - 获取当前FSM状态
// ============================================================================
int CAi::GetState() const {
    // 返回当前FSM状态
    // if (m_pStateMachine) {
    //     return CFsmClass<CAi>::GetCurrentState(m_pStateMachine);
    // }
    return 0;  // 默认状态
}

// ============================================================================
// EvaluateTransition - 评估状态转换
// ============================================================================
int CAi::EvaluateTransition() {
    // 评估当前状态的转换条件
    // IDA 反编译确认:
    // 1. 获取当前状态
    // 2. 检查所有转换条件
    // 3. 返回满足条件的目标状态

    if (!m_pStateMachine) {
        return 0;
    }

    // 遍历当前状态的所有转换
    // 检查条件是否满足
    // 如果满足，返回目标状态

    // return CFsmClass<CAi>::EvaluateCurrentStateTransition(m_pStateMachine, 0.0f);
    return 0;  // 默认无转换
}

// ============================================================================
// FuncCheckAggro - 检查仇恨值
// ============================================================================
void CAi::FuncCheckAggro() {
    // 检查并更新仇恨值
    // IDA 反编译确认:
    // 1. 检查怪物是否有效
    // 2. 获取仇恨列表
    // 3. 更新仇恨值
    // 4. 选择最高仇恨目标

    if (!m_pMonster) {
        return;
    }

    // 检查仇恨重置时间
    if (m_fDmgAggroReseTime > 0.0f) {
        // 仇恨重置时间逻辑
    }

    // 获取仇恨列表并更新
    // std::map<std::uint32_t, float> aggroList = m_pMonster->GetAggroList();
    // for (auto& pair : aggroList) {
    //     // 更新仇恨值
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "FuncCheckAggro executed");
}

// ============================================================================
// GetAggroCheckTime - 获取仇恨检查时间间隔
// ============================================================================
float CAi::GetAggroCheckTime() const {
    // 返回仇恨检查时间间隔
    // 使用 m_fAiCheckTime 作为仇恨检查间隔
    return m_fAiCheckTime;
}

// ============================================================================
// ChangeAiState
// 改变AI状态
// ============================================================================
void CAi::ChangeAiState(int nState) {
    // 实现状态切换逻辑
    // 1. 保存当前状态
    // 2. 设置新状态
    // 3. 触发状态进入函数

    if (nState < 0) {
        return;
    }

    // 设置状态
    SetState(nState);

    GreenDamTan_log(__FILE__, __FUNCTION__, "ChangeAiState called");
}

// ============================================================================
// GetConditionIntData IDA 0x14025F7F0 -> 0x14025F855
// 获取整数条件数据 - 通过函数指针数组获取条件值
// ============================================================================
int CAi::GetConditionIntData(int eVarName, int nValue) {
    // IDA 反编译确认:
    // nReturnValue = 0;
    // if ( _wIndex < 0x39u && m_arConditionIntFuncs[_wIndex] )
    // {
    //     v3 = m_arConditionIntFuncs[_wIndex];
    //     return (*v3)(this, _nVal);
    // }
    // return nReturnValue;

    int nReturnValue = 0;

    // 检查索引范围 (0x39 = 57)
    if (eVarName >= 0 && eVarName < 0x39) {
        // 获取函数指针
        ConditionIntFunc pFunc = m_arConditionIntFuncs[eVarName];
        if (pFunc != nullptr) {
            // 调用条件函数
            nReturnValue = (this->*pFunc)(nValue);
        }
    }

    return nReturnValue;
}

// ============================================================================
// GetConditionFloatData IDA 0x14025F860 -> 0x14025F8D2
// 获取浮点条件数据 - 通过函数指针数组获取条件值
// ============================================================================
float CAi::GetConditionFloatData(int eVarName, int nValue) {
    // IDA 反编译确认:
    // fReturnValue = 0.0;
    // if ( _wIndex < 0x39u && m_arConditionFloatFuncs[_wIndex] )
    // {
    //     v3 = m_arConditionFloatFuncs[_wIndex];
    //     return (*v3)(this, _nVal);
    // }
    // return fReturnValue;

    float fReturnValue = 0.0f;

    // 检查索引范围 (0x39 = 57)
    if (eVarName >= 0 && eVarName < 0x39) {
        // 获取函数指针
        ConditionFloatFunc pFunc = m_arConditionFloatFuncs[eVarName];
        if (pFunc != nullptr) {
            // 调用条件函数
            fReturnValue = (this->*pFunc)(nValue);
        }
    }

    return fReturnValue;
}

// ============================================================================
// CheckSkillGroupCondition
// 检查技能组条件
// ============================================================================
bool CAi::CheckSkillGroupCondition(unsigned int nSkillIndex, int nSkillGroup) {
    // TODO: 实现技能组条件检查
    // 检查技能组的转换条件是否满足

    (void)nSkillIndex;
    (void)nSkillGroup;
    return true;
}

// ============================================================================
// _ConditionHealth
// 检查生命值条件
// ============================================================================
int CAi::_ConditionHealth(int nValue) {
    // IDA 反编译确认:
    // 返回当前HP百分比或绝对值

    if (!m_pMonster) {
        return 0;
    }

    int nHP = m_pMonster->GetHP();

    if (nValue == 0) {
        // 返回HP百分比
        // TODO: int nMaxHP = m_pMonster->GetMaxHP();
        // return (nHP * 100) / nMaxHP;
        return nHP;
    }

    return nHP;
}

// ============================================================================
// FuncStartState IDA 0x14026A850 -> 0x14026AB0F
// 启动状态 - 初始化状态变量并开始新状态
// ============================================================================
void CAi::FuncStartState() {
    // IDA 反编译确认的完整流程:
    // 1. 检查 m_pMonster 和 m_pStateMachine 是否有效
    // 2. 重置所有状态时间相关变量
    // 3. 记录状态起始位置
    // 4. 保存当前HP
    // 5. 从状态机获取当前状态
    // 6. 从 m_mapStateVars 查找状态变量信息
    // 7. 如果找到，解析状态参数（生命周期、移动距离、目标距离、角度等）
    // 8. 调用 CheckInitMaze

    if (!m_pMonster || !m_pStateMachine) {
        return;
    }

    // 重置所有状态时间变量
    m_fStateTime = 0.0f;
    m_fStateLifeTime = -1.0f;
    m_fStateTempFloat[0] = 0.0f;
    m_fStateTempFloat[1] = 0.0f;
    m_fStateMoveDistMin = 0.0f;
    m_fStateMoveDistMax = 0.0f;
    m_fStateTargetDistMin = 0.0f;
    m_fStateTargetDistMax = 0.0f;
    m_fStateAngleMin = 0.0f;
    m_fStateAngleMax = 0.0f;
    m_fStateMoveDistSum = 0.0f;

    // 记录状态起始位置
    // TODO: const hkvVec3& pos = m_pMonster->GetPosition();
    // m_vStateMoveStartPos[0] = pos.x;
    // m_vStateMoveStartPos[1] = pos.y;
    // m_vStateMoveStartPos[2] = pos.z;

    // 保存当前HP
    m_nStatePreHP = m_pMonster->GetHP();

    // 从状态机获取当前状态
    // TODO: int nState = m_pStateMachine->GetCurrentState();
    // IDA: nState = XGameDBSocketMgr::GetLogDBAgentCount(this->m_pStateMachine);
    int nState = 0;  // 暂时使用默认值

    // 从 m_mapStateVars 查找状态变量信息
    auto it = m_mapStateVars.find(nState);
    if (it != m_mapStateVars.end()) {
        StateVarInfo& varInfo = it->second;

        // 解析状态生命周期
        // IDA: DataList[0][0] 和 DataList[0][1] 用于生命周期
        // StateVarInfo 结构映射: nValue = DataList[0][0], nMaxValue = DataList[0][1]
        int nLifeTimeMin = varInfo.nValue;
        int nLifeTimeMax = varInfo.nMaxValue;

        if (nLifeTimeMin >= 0) {
            if (nLifeTimeMax <= 0) {
                // 固定生命周期
                m_fStateLifeTime = static_cast<float>(nLifeTimeMin) * 0.001f;
            } else {
                // 随机生命周期
                // TODO: int nRandom = RandomBetween(nLifeTimeMin, nLifeTimeMax);
                int nRandom = nLifeTimeMin;  // 暂时使用最小值
                m_fStateLifeTime = static_cast<float>(nRandom) * 0.001f;
            }
        }

        // 解析移动距离
        // IDA: DataList[1][0] 和 DataList[1][1]
        // 映射到 StateVarInfo: nDefaultValue = DataList[1][0], nReserved = DataList[1][1]
        m_fStateMoveDistMin = static_cast<float>(varInfo.nDefaultValue);
        m_fStateMoveDistMax = static_cast<float>(varInfo.nReserved);

        // 解析目标距离和角度
        // TODO: 需要扩展 StateVarInfo 结构以支持更多参数
        // DataList[2][0], DataList[2][1] - 目标距离
        // DataList[3][0], DataList[3][1] - 角度
    }

    // 调用 CheckInitMaze
    CheckInitMaze();

    GreenDamTan_log(__FILE__, __FUNCTION__, "FuncStartState executed");
}

// ============================================================================
// FuncSearchTarget IDA 0x140265AD0 -> 0x140265F39
// 搜索目标 - 在视野范围内搜索敌对目标
// ============================================================================
void CAi::FuncSearchTarget() {
    // IDA 反编译确认的流程:
    // 1. 检查 m_pMonster 是否有效
    // 2. 获取当前位置
    // 3. 扫描附近对象
    // 4. 过滤敌对目标
    // 5. 选择最近的目标

    if (!m_pMonster) {
        return;
    }

    // TODO: 实现完整的目标搜索逻辑
    // 需要访问 XArea 和扫描附近对象

    GreenDamTan_log(__FILE__, __FUNCTION__, "FuncSearchTarget executed");
}

// ============================================================================
// FuncAttackSkill IDA 0x140268D80 -> 0x140269174
// 攻击技能 - 执行攻击技能逻辑
// ============================================================================
bool CAi::FuncAttackSkill() {
    // IDA 反编译确认的流程:
    // 1. 检查当前技能是否有效
    // 2. 检查技能冷却
    // 3. 检查技能条件
    // 4. 执行技能
    // 5. 更新冷却时间

    if (!m_pMonster) {
        return false;
    }

    // TODO: 实现完整的技能攻击逻辑

    GreenDamTan_log(__FILE__, __FUNCTION__, "FuncAttackSkill executed");
    return true;
}

// ============================================================================
// ClearTarget
// 清除目标
// ============================================================================
void CAi::ClearTarget() {
    if (m_pMonster) {
        m_pMonster->ChangeTarget(UXActorID(0xFFFFFFFF));
    }
}

// ============================================================================
// CheckInitMaze
// 检查初始化迷宫
// ============================================================================
void CAi::CheckInitMaze() {
    // TODO: 检查是否需要初始化迷宫相关逻辑
    GreenDamTan_log(__FILE__, __FUNCTION__, "CheckInitMaze called");
}

// ============================================================================
// CheckDelegateSkill
// 检查代理技能
// ============================================================================
void CAi::CheckDelegateSkill(unsigned int nSkillIndex) {
    // TODO: 检查代理技能逻辑
    (void)nSkillIndex;
    GreenDamTan_log(__FILE__, __FUNCTION__, "CheckDelegateSkill called");
}

// ============================================================================
// GetDmgAggroReseTime IDA 0x140261D90
// 获取伤害仇恨重置时间
// ============================================================================
float CAi::GetDmgAggroReseTime() {
    // IDA 0x140261D90: return this->m_fDmgAggroReseTime
    return m_fDmgAggroReseTime;
}

// ============================================================================
// IsEnableClearTarget IDA 0x140261DA0
// 检查是否允许清除目标
// ============================================================================
bool CAi::IsEnableClearTarget() {
    // IDA 0x140261DA0: return this->m_bEnableClearTarget
    return m_bEnableClearTarget;
}

// ============================================================================
// FuncCheckReturnPos IDA 0x14026A200 -> 0x14026A306
// 检查返回位置 - 检查怪物是否需要返回生成点
// ============================================================================
bool CAi::FuncCheckReturnPos() {
    // IDA 反编译确认的完整流程:
    // 1. 检查 m_pMonster 是否有效且不是跟随者
    // 2. 获取创建位置和当前位置
    // 3. 计算距离并检查是否超过返回距离
    // 4. 如果超过，切换到返回状态

    if (!m_pMonster || m_pMonster->IsFollower()) {
        return false;
    }

    // 获取创建位置和当前位置
    // TODO: 实现完整的位置获取和距离计算
    // 目前暂时返回 false，等待相关函数实现
    return false;
}

// ============================================================================
// IsProtectState IDA 0x14026B960 -> 0x14026B97B
// 检查保护状态 - 检查是否处于保护状态
// ============================================================================
bool CAi::IsProtectState() {
    // IDA 反编译确认:
    // return this->m_eProtectState != ePROTECT_NONE;
    return m_eProtectState != ePROTECT_NONE;
}

// ============================================================================
// CheckStateLifeTime IDA 0x14026AB10 -> 0x14026AB56
// 检查状态生命周期 - 检查当前状态是否已超时
// ============================================================================
void CAi::CheckStateLifeTime() {
    // IDA 反编译确认:
    // if ( this->m_fStateLifeTime > 0.0 && this->m_fStateTime >= this->m_fStateLifeTime )
    //     CAi::FuncEndState(this);

    if (m_fStateLifeTime > 0.0f && m_fStateTime >= m_fStateLifeTime) {
        FuncEndState();
    }
}

// ============================================================================
// FuncFindEnemy IDA 0x14026B710 -> 0x14026B7E1
// 寻找敌人 - 在视野范围内搜索敌对目标
// ============================================================================
void CAi::FuncFindEnemy(float fElapsedTime) {
    // IDA 反编译确认的完整流程:
    // 1. 检查是否是巡逻怪物 (如果是则不搜索)
    // 2. 检查 m_pMonster 是否有效
    // 3. 更新搜索目标计时
    // 4. 检查是否超过搜索延迟
    // 5. 检查 MotionClass != 27 (死亡动作)
    // 6. 调用 FuncSearchTarget 搜索目标
    // 7. 如果找到目标，切换到选择动作状态

    // 检查是否是巡逻怪物
    if (m_bPatrolMonster) {
        return;
    }

    // 检查怪物是否有效
    if (!m_pMonster) {
        return;
    }

    // 更新搜索目标计时
    m_fSearchTargetTime += fElapsedTime;

    // 检查是否超过搜索延迟
    if (m_fSearchTargetTime <= m_fDelaySearchTarget) {
        return;
    }

    // 重置搜索计时
    m_fSearchTargetTime = 0.0f;

    // 检查 MotionClass
    short nMotionClass = m_pMonster->GetMotionClass();
    if (nMotionClass == 0) {
        return;
    }

    // IDA: if ( CMover::GetMotionClass(this->m_pMonster) != 27 )
    if (nMotionClass == 27) {
        return;
    }

    // 搜索目标
    FuncSearchTarget();

    // 检查是否找到目标
    std::uint32_t dwTargetID = m_pMonster->GetTargetID();
    if (dwTargetID != 0xFFFFFFFF) {
        ChangeAiState(FSMSTATES_SELECT_ACTION);
    }
}

// ============================================================================
// StartAttackSkill IDA 0x14027E3A0 -> 0x14027E872
// 开始攻击技能 - 启动攻击技能
// ============================================================================
void CAi::StartAttackSkill(int nSkillIndex) {
    // IDA 反编译确认的完整流程:
    // 1. 检查 m_pMonster 是否有效
    // 2. 检查技能索引和技能组设置
    // 3. 获取实际的技能索引
    // 4. 获取怪物表引用和技能引用
    // 5. 获取技能目标
    // 6. 如果没有目标，切换到等待状态
    // 7. 设置目标位置标志
    // 8. 计算技能移动目标位置
    // 9. 设置技能激活标志

    if (!m_pMonster) {
        return;
    }

    // 检查技能组设置和技能索引范围
    if (m_bSetSkillGroup && nSkillIndex >= 10) {
        ChangeAiState(FSMSTATES_SELECT_ACTION);
        return;
    }

    // 获取实际的技能索引
    int nActualSkillIndex = GetSkillIndex(nSkillIndex);
    if (nActualSkillIndex == -1 || nActualSkillIndex >= 10) {
        ChangeAiState(FSMSTATES_SELECT_ACTION);
        return;
    }

    // 获取怪物表引用
    TB_MONSTER* pTableRef = m_pMonster->GetMobTableRef();
    if (!pTableRef) {
        ChangeAiState(FSMSTATES_SELECT_ACTION);
        return;
    }

    // 获取技能引用
    // TODO: 从 XResourceMgr 获取 TB_SKILL
    m_pCurSkillRef = nullptr;  // 暂时设置为 nullptr

    if (!m_pCurSkillRef) {
        ChangeAiState(FSMSTATES_SELECT_ACTION);
        return;
    }

    // 获取技能目标
    CMoverEx* pTarget = FindTargetBySkill();
    if (!pTarget) {
        ChangeAiState(FSMSTATES_WAIT);
        return;
    }

    // 重置技能激活标志
    m_bSkillActivate = false;

    // 设置选择的技能索引
    m_nSelectGroupSkill = nSkillIndex;
    m_nSelectSkillIndex = nActualSkillIndex;

    GreenDamTan_log(__FILE__, __FUNCTION__, "StartAttackSkill executed");
}

// ============================================================================
// FuncEndState - 结束状态
// ============================================================================
void CAi::FuncEndState() {
    // TODO: 实现状态结束逻辑
    // 清理当前状态，重置相关变量
    m_fStateTime = 0.0f;
    m_fStateLifeTime = -1.0f;

    GreenDamTan_log(__FILE__, __FUNCTION__, "FuncEndState executed");
}

// ============================================================================
// GetSkillIndex - 获取技能索引
// ============================================================================
int CAi::GetSkillIndex(int nSkillIndex) {
    // TODO: 实现技能索引获取逻辑
    // 如果设置了技能组，从技能组获取索引
    // 否则直接返回传入的索引

    if (m_bSetSkillGroup) {
        // TODO: 从技能组转换数组获取实际索引
        return -1;  // 暂时返回 -1
    }

    return nSkillIndex;
}

// ============================================================================
// GetSuicideTime IDA 0x14019D210
// 获取自杀时间
// ============================================================================
float CAi::GetSuicideTime() {
    // IDA 反编译确认:
    // return this->m_fSuicideTime;
    return m_fSuicideTime;
}

// ============================================================================
// GetTargetSightDistance IDA 0x14019D1F0
// 获取目标视野距离
// ============================================================================
float CAi::GetTargetSightDistance() {
    // IDA 反编译确认:
    // return this->m_fTargetSightDistance;
    return m_fTargetSightDistance;
}

// ============================================================================
// SetEnalbeHelperWarp IDA 0x140091E70
// 设置助手传送启用
// ============================================================================
void CAi::SetEnalbeHelperWarp(bool bEnable) {
    // IDA 反编译确认:
    // this->m_bEnableHelperWarp = bEnable;
    m_bEnableHelperWarp = bEnable;
}

// ============================================================================
// SetSuicideTime IDA 0x140260E00
// 设置自杀时间
// ============================================================================
void CAi::SetSuicideTime(float fTime) {
    // IDA 反编译确认:
    // this->m_fSuicideTime = _fTime;
    m_fSuicideTime = fTime;
}

// ============================================================================
// SetEnableClearTarget IDA 0x140260E20
// 设置是否允许清除目标
// ============================================================================
void CAi::SetEnableClearTarget(int nEnable) {
    // IDA 反编译确认:
    // this->m_bEnableClearTarget = nEnable >= 1;
    m_bEnableClearTarget = (nEnable >= 1);
}

// ============================================================================
// SetAiCheckTime IDA 0x140260B10
// 设置AI检查时间
// ============================================================================
void CAi::SetAiCheckTime(float fTime) {
    // IDA 反编译确认:
    // this->m_fAiCheckTime = _fTime;
    m_fAiCheckTime = fTime;
}

// ============================================================================
// SetPatrolMonster IDA 0x140260B30
// 设置巡逻怪物标志
// ============================================================================
void CAi::SetPatrolMonster(int nCheck) {
    // IDA 反编译确认:
    // if (nCheck > 0) this->m_bPatrolMonster = 1;
    if (nCheck > 0) {
        m_bPatrolMonster = true;
    }
}

// ============================================================================
// SetRunDistance IDA 0x140260C60
// 设置逃跑距离
// ============================================================================
void CAi::SetRunDistance(float fDistance) {
    // IDA 反编译确认:
    // this->m_fRunDistance = _fDistance;
    m_fRunDistance = fDistance;
}

// ============================================================================
// SetReturnDistance IDA 0x140260C80
// 设置返回距离
// ============================================================================
void CAi::SetReturnDistance(float fDistance, int nHP) {
    // IDA 反编译确认:
    // this->m_fReturnDistance = _fDistance;
    // this->m_nReturnHP = _nHP;
    m_fReturnDistance = fDistance;
    m_nReturnHP = nHP;
}

// ============================================================================
// SetTakeTargetInfo IDA 0x140260B60
// 设置目标获取信息
// ============================================================================
void CAi::SetTakeTargetInfo(float fDistance) {
    // IDA 反编译确认:
    // CAi::SetTargetSightDistance(this, _fDistance);
    m_fTargetSightDistance = fDistance;
}

// ============================================================================
// SetMoveInfoToTarget IDA 0x140260B90
// 设置目标移动信息
// ============================================================================
void CAi::SetMoveInfoToTarget(float fDelay, float fDistance) {
    // IDA 反编译确认:
    // this->m_fMoveDelayToTarget = _fDelay;
    // this->m_fMoveDistanceToTarget = _fDistance;
    m_fMoveDelayToTarget = fDelay;
    m_fMoveDistanceToTarget = fDistance;
}

// ============================================================================
// SetRequestHelpInfo IDA 0x140260CC0
// 设置请求帮助信息
// ============================================================================
void CAi::SetRequestHelpInfo(int nCount, float fDistance, int nMonsterID, int nMonsterCount) {
    // IDA 反编译确认:
    // this->m_nRequestHelpCnt = _nCount;
    // this->m_fRequestHelpDistance = _fDistance;
    // this->m_nRequestHelpMonsterID = _nMonsterID;
    // this->m_nRequestHelpMonsterCount = _nMonsterCount;
    m_nRequestHelpCnt = nCount;
    m_fRequestHelpDistance = fDistance;
    m_nRequestHelpMonsterID = nMonsterID;
    m_nRequestHelpMonsterCount = nMonsterCount;
}

// ============================================================================
// SetFirstAttacker IDA 0x140260C10
// 设置首次攻击者
// ============================================================================
void CAi::SetFirstAttacker(int nFirstAttacker, float fDelay) {
    // IDA 反编译确认:
    // this->m_bIsFirstAttacker = _nFirstAttacker != 0;
    // this->m_fDelaySearchTarget = _fDelay;
    m_bIsFirstAttacker = (nFirstAttacker != 0);
    m_fDelaySearchTarget = fDelay;
}

// ============================================================================
// SetRunawayInfo IDA 0x140260D20
// 设置逃跑信息
// ============================================================================
void CAi::SetRunawayInfo(int nHP, int nCount, float fMinTime, float fMaxTime) {
    // IDA 反编译确认:
    // this->m_nRunawayHP = _nHP;
    // this->m_nRunawayMaxCount = _nCount;
    // this->m_fRunwayMinTimeOut = _fMinTime;
    // this->m_fRunwayMaxTimeOut = _fMaxTime;
    m_nRunawayHP = nHP;
    m_nRunawayMaxCount = nCount;
    m_fRunwayMinTimeOut = fMinTime;
    m_fRunwayMaxTimeOut = fMaxTime;
}

// ============================================================================
// SetDelegateSkill IDA 0x140260E50
// 设置代理技能
// ============================================================================
void CAi::SetDelegateSkill(const char* szSkillID, const char* szMonsterID) {
    // IDA 反编译确认:
    // std::string _Str(szSkillID);
    // this->m_stDelegateSkill.nSkillID = std::stoi(_Str, nullptr, 10);
    // std::string v4(szMonsterID);
    // this->m_stDelegateSkill.nDelegateMobID = std::stoi(v4, nullptr, 10);
    if (szSkillID) {
        m_stDelegateSkill.nSkillID = std::atoi(szSkillID);
    }
    if (szMonsterID) {
        m_stDelegateSkill.nDelegateMobID = std::atoi(szMonsterID);
    }
}

// ============================================================================
// AddDelegateTarget IDA 0x140260F20
// 添加代理目标
// ============================================================================
void CAi::AddDelegateTarget(int nIndex, const char* szMobID1, const char* szMobID2, const char* szMobID3, const char* szMobID4, const char* szMobID5) {
    // IDA 反编译确认:
    // 创建 DelegateTarget 结构，解析各个怪物ID字符串并添加到向量

    DelegateTarget target;
    target.nIndex = nIndex;

    // 解析怪物ID字符串
    if (szMobID1) {
        target.nMobID1 = std::atoi(szMobID1);
    } else {
        target.nMobID1 = 0;
    }

    if (szMobID2) {
        target.nMobID2 = std::atoi(szMobID2);
    } else {
        target.nMobID2 = 0;
    }

    if (szMobID3) {
        target.nMobID3 = std::atoi(szMobID3);
    } else {
        target.nMobID3 = 0;
    }

    if (szMobID4) {
        target.nMobID4 = std::atoi(szMobID4);
    } else {
        target.nMobID4 = 0;
    }

    if (szMobID5) {
        target.nMobID5 = std::atoi(szMobID5);
    } else {
        target.nMobID5 = 0;
    }

    // 添加到代理目标列表
    m_stDelegateSkill.vecTarget.push_back(target);
}

// ============================================================================
// SetCommonAction IDA 0x140261400
// 设置通用动作
// ============================================================================
void CAi::SetCommonAction(unsigned int nIndex, const char* szActionName) {
    // IDA 反编译确认:
    // if ( _nIndex < 0xA )
    //     VString::operator=(&this->m_strCommonActions[_nIndex], _szActionName);
    if (nIndex < 10 && szActionName) {
        // TODO: m_strCommonActions[nIndex] = szActionName;
        // 由于VString尚未实现，暂时保留框架
    }
}

// ============================================================================
// SetSkillGroupRate IDA 0x140261590
// 设置技能组比率
// ============================================================================
void CAi::SetSkillGroupRate(int nSkillRate1, int nSkillRate2, int nSkillRate3, int nSkillRate4, int nSkillRate5,
                             int nSkillRate6, int nSkillRate7, int nSkillRate8, int nSkillRate9, int nSkillRate10) {
    // IDA 反编译确认:
    // this->m_bSetSkillGroup = 1;
    // this->m_nSkillGroupRatio[0-9] = nSkillRate1-10;
    m_bSetSkillGroup = true;
    m_nSkillGroupRatio[0] = nSkillRate1;
    m_nSkillGroupRatio[1] = nSkillRate2;
    m_nSkillGroupRatio[2] = nSkillRate3;
    m_nSkillGroupRatio[3] = nSkillRate4;
    m_nSkillGroupRatio[4] = nSkillRate5;
    m_nSkillGroupRatio[5] = nSkillRate6;
    m_nSkillGroupRatio[6] = nSkillRate7;
    m_nSkillGroupRatio[7] = nSkillRate8;
    m_nSkillGroupRatio[8] = nSkillRate9;
    m_nSkillGroupRatio[9] = nSkillRate10;
}

// ============================================================================
// SetReservedCondition IDA 0x140261750
// 设置保留条件
// ============================================================================
void CAi::SetReservedCondition(unsigned int nIndex, unsigned int nVariable, const char* szConditionString,
                                float fFloatData1, float fFloatData2) {
    // IDA 反编译确认:
    // 解析条件字符串并设置保留条件
    if (nIndex >= 0x14 || nVariable > 0x38) {
        return;
    }

    // 由于CFsmCondition尚未完全实现，这里保留框架
    (void)szConditionString;
    (void)fFloatData1;
    (void)fFloatData2;

    // TODO: 当CFsmCondition实现后补充完整逻辑
    // CFsmCondition* pCondition = &m_arReservedCondition[nIndex];
    // CFsmCondition::SetVariables(pCondition, nVariable);
    // CFsmCondition::SetValue(pCondition, nIntData, fFloatData);
    // CFsmCondition::SetFunctions(pCondition, eConditionFunc);
}

// ============================================================================
// SetDeathAction IDA 0x140261C00
// 设置死亡动作
// ============================================================================
void CAi::SetDeathAction(const char* szActionName) {
    // IDA 反编译确认:
    // VString::operator=(&this->m_strDeathAnim, _szActionName);
    if (szActionName) {
        // TODO: m_strDeathAnim = szActionName;
        // 由于VString尚未实现，暂时保留框架
    }
}

// ============================================================================
// SetProtectInfo IDA 0x140261DD0
// 设置保护信息
// ============================================================================
void CAi::SetProtectInfo(float fEffectDist, float fTimeOut) {
    // IDA 反编译确认:
    // this->m_fProtectEffectDist = fEffectDist;
    // this->m_fProtectWaitTimeOut = fTimeOut;
    m_fProtectEffectDist = fEffectDist;
    m_fProtectWaitTimeOut = fTimeOut;
}

// ============================================================================
// SetSkillCooltime IDA 0x140261F40
// 设置技能冷却时间
// ============================================================================
void CAi::SetSkillCooltime() {
    // IDA 反编译确认:
    // 根据技能表设置冷却时间
    // 由于TB_SKILL结构尚未完全实现，这里保留框架

    // TODO: 当TB_SKILL实现后补充完整逻辑
    // if (pSkillTable && (pSkillTable->CoolTime || pSkillTable->CoolTime_Global)) {
    //     float fCurrTime = IVTimer::GetTime(Timer);
    //     float fCooldownTime = fCurrTime + pSkillTable->CoolTime * 0.001f;
    //     // 更新m_mapCooltimeList
    //     // 更新m_fGlobalCooltime
    // }
}

// ============================================================================
// FuncEscapeProcess - 处理逃跑逻辑
// ============================================================================
void CAi::FuncEscapeProcess(float fElapsedTime) {
    // TODO: 实现逃跑处理逻辑
    // IDA 反编译确认的完整逻辑:
    // 1. 检查是否需要逃跑
    // 2. 计算逃跑方向
    // 3. 移动怪物
    (void)fElapsedTime;
}

// ============================================================================
// CopyFullData IDA 0x14025FE10 -> 0x140260A99
// 复制完整AI数据 - 从另一个CAi实例复制所有数据
// ============================================================================
void CAi::CopyFullData(const CAi& Other) {
    // IDA 反编译确认的完整流程:
    // 1. 调用 CopyConditionData 复制基础条件数据
    // 2. 复制各种成员变量
    // 3. 复制状态变量映射
    // 4. 复制技能转换数组的条件
    // 5. 复制状态数据并注册状态函数
    // 6. 设置FSM脚本数据

    // 复制基础条件数据
    // CopyConditionData 尚未实现，直接复制成员
    std::memcpy(m_arConditionIntFuncs, Other.m_arConditionIntFuncs, sizeof(m_arConditionIntFuncs));
    std::memcpy(m_arConditionFloatFuncs, Other.m_arConditionFloatFuncs, sizeof(m_arConditionFloatFuncs));

    // 重置全局冷却时间
    m_fGlobalCooltime = 0.0f;

    // 复制巡逻怪物标志
    m_bPatrolMonster = Other.m_bPatrolMonster;

    // 复制死亡动画 (VString 尚未实现)
    // m_strDeathAnim = Other.m_strDeathAnim;

    // 复制恢复技能
    // m_nRecoverySkill = Other.m_nRecoverySkill;

    // 复制通用动作 (VString 尚未实现)
    // for (int i = 0; i < 10; ++i) {
    //     m_strCommonActions[i] = Other.m_strCommonActions[i];
    // }

    // 复制技能组设置
    m_bSetSkillGroup = Other.m_bSetSkillGroup;
    std::memcpy(m_nSkillGroupRatio, Other.m_nSkillGroupRatio, sizeof(m_nSkillGroupRatio));
    // std::memcpy(m_nCustomSkillID, Other.m_nCustomSkillID, sizeof(m_nCustomSkillID));
    // std::memcpy(m_nSkillGroupID, Other.m_nSkillGroupID, sizeof(m_nSkillGroupID));
    // std::memcpy(m_nSkillSortType, Other.m_nSkillSortType, sizeof(m_nSkillSortType));
    // std::memcpy(m_nSkillOrder, Other.m_nSkillOrder, sizeof(m_nSkillOrder));

    // 复制保留条件 (CFsmCondition 尚未实现)
    // std::memcpy(m_arReservedCondition, Other.m_arReservedCondition, sizeof(m_arReservedCondition));

    // 清空并复制状态变量映射
    m_mapStateVars.clear();
    for (auto it = Other.m_mapStateVars.begin(); it != Other.m_mapStateVars.end(); ++it) {
        m_mapStateVars[it->first] = it->second;
    }

    // 复制技能转换的条件向量 (CFsmTransition 尚未实现)
    for (int k = 0; k < 10; ++k) {
        // CFsmTransition::CopyConditions(m_arSkillTransition[k], Other.m_arSkillTransition[k]);

        for (int j = 0; j < 10; ++j) {
            // CFsmTransition::CopyConditions(m_arSkillGroupTransition[k][j], Other.m_arSkillGroupTransition[k][j]);
            // CFsmTransition::CopyConditions(m_arActionAfterSkillTransition[k][j], Other.m_arActionAfterSkillTransition[k][j]);
        }
    }

    // 复制取消技能转换
    // CFsmTransition::CopyConditions(m_arCancelSkillTransition, Other.m_arCancelSkillTransition);

    // 复制状态数据并注册状态函数
    m_vecStateData.clear();
    for (size_t jj = 0; jj < Other.m_vecStateData.size(); ++jj) {
        int _nState = Other.m_vecStateData[jj];
        RegisterStateFunctions(_nState);
        m_vecStateData.push_back(_nState);
    }

    // 复制FSM数据 (CFsmClass 尚未实现)
    // for (size_t kk = 0; kk < Other.m_vecFsmData.size(); ++kk) {
    //     const FsmData& fsmData = Other.m_vecFsmData[kk];
    //     CFsmClass<CAi>::SetScriptData(m_pStateMachine, ...);
    //     m_vecFsmData.push_back(fsmData);
    // }

    // 复制扩展FSM数据并组合保留条件
    // for (size_t mm = 0; mm < Other.m_vecFsmDataEx.size(); ++mm) {
    //     const FsmDataEx& fsmDataEx = Other.m_vecFsmDataEx[mm];
    //     _CombineReservedConditions(this, fsmDataEx._nState, fsmDataEx._nOutPutState, ...);
    //     m_vecFsmDataEx.push_back(fsmDataEx);
    // }

    // 复制其他成员变量
    m_nSelectActionCount = Other.m_nSelectActionCount;
    std::memcpy(m_arSelectActionRate, Other.m_arSelectActionRate, sizeof(m_arSelectActionRate));
    std::memcpy(m_arSelectActionResult, Other.m_arSelectActionResult, sizeof(m_arSelectActionResult));

    m_nRunawayHP = Other.m_nRunawayHP;
    m_nRunawayCurrentCount = Other.m_nRunawayCurrentCount;
    m_nRunawayMaxCount = Other.m_nRunawayMaxCount;

    m_eProtectState = Other.m_eProtectState;
    m_fFuzzyCheckTime = Other.m_fFuzzyCheckTime;
    m_fFuzzyDelayTime = Other.m_fFuzzyDelayTime;

    m_fSpawnAggroDistance = Other.m_fSpawnAggroDistance;
    m_fSpawnAggroValue = Other.m_fSpawnAggroValue;
    m_fDmgAggroReseTime = Other.m_fDmgAggroReseTime;
    m_bEnableClearTarget = Other.m_bEnableClearTarget;

    m_fTargetSightDistance = Other.m_fTargetSightDistance;
    m_fReturnDistance = Other.m_fReturnDistance;
    m_bPatrolMonster = Other.m_bPatrolMonster;
    m_fSearchTargetTime = Other.m_fSearchTargetTime;
    m_fDelaySearchTarget = Other.m_fDelaySearchTarget;

    m_bSetSkillGroup = Other.m_bSetSkillGroup;
    m_nSelectGroupSkill = Other.m_nSelectGroupSkill;
    m_nSelectSkillIndex = Other.m_nSelectSkillIndex;
    m_bSkillActivate = Other.m_bSkillActivate;

    m_fSuicideTime = Other.m_fSuicideTime;
    m_bEnableHelperWarp = Other.m_bEnableHelperWarp;
    m_fAiCheckTime = Other.m_fAiCheckTime;
    m_fRunDistance = Other.m_fRunDistance;
    m_nReturnHP = Other.m_nReturnHP;

    m_fMoveDelayToTarget = Other.m_fMoveDelayToTarget;
    m_fMoveDistanceToTarget = Other.m_fMoveDistanceToTarget;

    m_nRequestHelpCnt = Other.m_nRequestHelpCnt;
    m_fRequestHelpDistance = Other.m_fRequestHelpDistance;
    m_nRequestHelpMonsterID = Other.m_nRequestHelpMonsterID;
    m_nRequestHelpMonsterCount = Other.m_nRequestHelpMonsterCount;

    m_bIsFirstAttacker = Other.m_bIsFirstAttacker;
    m_fRunwayMinTimeOut = Other.m_fRunwayMinTimeOut;
    m_fRunwayMaxTimeOut = Other.m_fRunwayMaxTimeOut;

    m_fProtectEffectDist = Other.m_fProtectEffectDist;
    m_fProtectWaitTimeOut = Other.m_fProtectWaitTimeOut;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CopyFullData executed");
}

// ============================================================================
// RegisterStateFunctions IDA 0x140263160 -> 0x140263A10
// 注册状态函数 - 为各种AI状态注册对应的处理函数
// ============================================================================
void CAi::RegisterStateFunctions(int _nState) {
    // IDA 反编译确认: 大型 switch-case 结构，注册42+个状态的函数

    switch (_nState) {
        case 1:  // FSMSTATES_WAIT
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_WAIT,
            //     CAi::_UpdateWait, CAi::_StartWait, nullptr);
            break;

        case 2:  // FSMSTATES_PATROL
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_PATROL,
            //     CAi::_UpdatePatrol, CAi::_StartPatrol, CAi::_EndProtectMove);
            break;

        case 3:  // FSMSTATES_SELECT_ACTION
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_SELECT_ACTION,
            //     CAi::_UpdateSelectAction, CAi::_StartSelectAction, nullptr);
            break;

        case 4:  // FSMSTATES_BATTLE_WAIT
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_BATTLE_WAIT,
            //     CAi::_UpdateBattleWait, CAi::_StartBattleWait, nullptr);
            break;

        case 5:  // FSMSTATES_BATTLE_MOVE
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_BATTLE_MOVE,
            //     CAi::_UpdateBattleMove, CAi::_StartBattleMove, CAi::_EndProtectMove);
            break;

        case 6:  // FSMSTATES_GAZE
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_GAZE,
            //     CAi::_UpdateGaze, CAi::_StartGaze, CAi::_EndGaze);
            break;

        case 7:  // FSMSTATES_MOVE
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_MOVE,
            //     CAi::_UpdateMove, CAi::_StartMove, CAi::_EndProtectMove);
            break;

        case 8:  // FSMSTATES_ATTACK
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_ATTACK,
            //     nullptr, CAi::_StartAttack, nullptr);
            break;

        case 9:  // FSMSTATES_ATTACK_SKILL1
        case 10: // FSMSTATES_ATTACK_SKILL2
        case 11: // FSMSTATES_ATTACK_SKILL3
        case 12: // FSMSTATES_ATTACK_SKILL4
        case 13: // FSMSTATES_ATTACK_SKILL5
        case 14: // FSMSTATES_ATTACK_SKILL6
        case 15: // FSMSTATES_ATTACK_SKILL7
        case 16: // FSMSTATES_ATTACK_SKILL8
        case 17: // FSMSTATES_ATTACK_SKILL9
        case 18: // FSMSTATES_ATTACK_SKILL10
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, state,
            //     CAi::_UpdateAttackSkill, startFunc, CAi::_EndAttackSkill);
            break;

        case 19: // FSMSTATES_COMMON_ACTION1
        case 20: // FSMSTATES_COMMON_ACTION2
        case 21: // FSMSTATES_COMMON_ACTION3
        case 22: // FSMSTATES_COMMON_ACTION4
        case 23: // FSMSTATES_COMMON_ACTION5
        case 24: // FSMSTATES_COMMON_ACTION6
        case 25: // FSMSTATES_COMMON_ACTION7
        case 26: // FSMSTATES_COMMON_ACTION8
        case 27: // FSMSTATES_COMMON_ACTION9
        case 28: // FSMSTATES_COMMON_ACTION10
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, state,
            //     CAi::_UpdateCommonAction, startFunc, nullptr);
            break;

        case 29: // FSMSTATES_RETURN
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_RETURN,
            //     CAi::_UpdateReturn, CAi::_StartReturn, CAi::_EndReturn);
            break;

        case 30: // FSMSTATES_SUCIDE
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_SUCIDE,
            //     nullptr, CAi::_StartSucide, nullptr);
            break;

        case 31: // FSMSTATES_RECOVERY
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_RECOVERY,
            //     CAi::_UpdateRecovery, CAi::_StartRecovery, CAi::_EndRecovery);
            break;

        case 32: // FSMSTATES_TRACE_MOVE
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_TRACE_MOVE,
            //     CAi::_UpdateTraceMove, CAi::_StartTraceMove, CAi::_EndProtectMove);
            break;

        case 33: // FSMSTATES_REQUESTHELP
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_REQUESTHELP,
            //     nullptr, CAi::_StartRequestHelp, nullptr);
            break;

        case 34: // FSMSTATES_REQUEST_SUPPORT
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_REQUEST_SUPPORT,
            //     CAi::_UpdateRequestSupport, CAi::_StartRequestSupport, nullptr);
            break;

        case 35: // FSMSTATES_RUNAWAY
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_RUNAWAY,
            //     CAi::_UpdateRunaway, CAi::_StartRunaway, nullptr);
            break;

        case 37: // FSMSTATES_SWITCH_STATE1
        case 38: // FSMSTATES_SWITCH_STATE2
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, state,
            //     CAi::_UpdateSwitchState, CAi::_StartSwitchState, nullptr);
            break;

        case 39: // FSMSTATES_PROTECTION
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_PROTECTION,
            //     CAi::_UpdateProtection, nullptr, nullptr);
            break;

        case 40: // FSMSTATES_PROTECT_MOVE
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_PROTECT_MOVE,
            //     CAi::_UpdateProtectGaze, CAi::_StartProtectMove, CAi::_EndProtectMove);
            break;

        case 41: // FSMSTATES_PROTECT_GAZE
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_PROTECT_GAZE,
            //     CAi::_UpdateProtectGaze, CAi::_StartProtectGaze, CAi::_EndGaze);
            break;

        case 42: // FSMSTATES_PROTECT_WAIT
            // CFsmClass<CAi>::AddStateFunctions(m_pStateMachine, FSMSTATES_PROTECT_WAIT,
            //     CAi::_UpdateProtectWait, CAi::_StartProtectWait, nullptr);
            break;

        default:
            break;
    }

    // 保存状态数据
    m_vecStateData.push_back(_nState);
}

// ============================================================================
// RegisterConditionsEx IDA 0x140263E10 -> 0x140264242
// 注册扩展条件 - 解析条件字符串并注册FSM条件
// ============================================================================
void CAi::RegisterConditionsEx(int _nState, int _nOutPutState, unsigned int _nVariable,
                               const char* _szConditionString, const char* _szData1,
                               const char* _szData2, int _nTransitionIndex) {
    // IDA 反编译确认:
    // 1. 解析数据字符串为整数和浮点数
    // 2. 解析条件字符串确定条件函数类型
    // 3. 根据变量类型确定数据类型
    // 4. 创建 FsmData 并添加到 m_vecFsmData

    if (_nVariable > 0x38) {
        return;  // 变量索引超出范围
    }

    // 解析数据
    int _nData1 = 0;
    int _nData2 = 0;
    float _fFloatData1 = 0.0f;
    float _fFloatData2 = 0.0f;

    if (_szData1) {
        _nData1 = std::atoi(_szData1);
        _fFloatData1 = static_cast<float>(_nData1);
    }
    if (_szData2) {
        _nData2 = std::atoi(_szData2);
        _fFloatData2 = static_cast<float>(_nData2);
    }

    // 解析条件字符串
    int eConditionFunc = 0;  // CONDITION_NONE

    if (_szConditionString) {
        if (std::strcmp(_szConditionString, "=") == 0) {
            eConditionFunc = 1;  // CONDITION_EQUAL
        } else if (std::strcmp(_szConditionString, "!=") == 0) {
            eConditionFunc = 2;  // CONDITION_NOT_EQUAL
        } else if (std::strcmp(_szConditionString, ">") == 0) {
            eConditionFunc = 3;  // CONDITION_GREATER_THAN
        } else if (std::strcmp(_szConditionString, "<") == 0) {
            eConditionFunc = 4;  // CONDITION_LESS_THAN
        } else if (std::strcmp(_szConditionString, "&") == 0) {
            eConditionFunc = 5;  // CONDITION_BIT_EQUAL
        } else if (std::strcmp(_szConditionString, "<>") == 0) {
            eConditionFunc = 6;  // CONDITION_RANGE_TRUE
        } else if (std::strcmp(_szConditionString, "!<>") == 0) {
            eConditionFunc = 7;  // CONDITION_RANGE_FALSE
        } else if (std::strcmp(_szConditionString, "&=") == 0) {
            eConditionFunc = 8;  // CONDITION_RANGE_EQUAL
        }
    }

    // 确定数据类型
    int eVarDataType = 0;  // FSMDTYPE_NONE

    // 检查是否有整数条件函数
    if (m_arConditionIntFuncs[_nVariable] != nullptr) {
        eVarDataType = 1;  // FSMDTYPE_INT
        // 如果是范围条件且有第二个数据，使用随机整数
        if (eConditionFunc < 6 && _fFloatData2 != 0.0f) {
            // TODO: if (!IsVarNeedCondition(_nVariable))
            //     eVarDataType = 2;  // FSMDTYPE_RANDOMINT
        }
    } else if (m_arConditionFloatFuncs[_nVariable] != nullptr) {
        eVarDataType = 3;  // FSMDTYPE_FLOAT
        // 如果是范围条件且有第二个数据，使用随机浮点数
        if (eConditionFunc < 6 && _fFloatData2 != 0.0f) {
            // TODO: if (!IsVarNeedCondition(_nVariable))
            //     eVarDataType = 4;  // FSMDTYPE_RANDOMFLOAT
        }
    }

    // 创建 FsmData 结构
    // FsmData fsmData;
    // fsmData._eCurrentState = _nState;
    // fsmData._eOutPutState = _nOutPutState;
    // fsmData._eVarDataType = eVarDataType;
    // fsmData._eVariable = _nVariable;
    // fsmData._eConditionFunc = eConditionFunc;
    // fsmData._pIntData[0] = _nData1;
    // fsmData._pIntData[1] = _nData2;
    // fsmData._pFloatData[0] = _fFloatData1;
    // fsmData._pFloatData[1] = _fFloatData2;
    // fsmData._nTransitionIndex = _nTransitionIndex - 1;

    // m_vecFsmData.push_back(fsmData);

    // TODO: 当 FsmData 结构实现后，调用 CFsmClass<CAi>::SetScriptData
    // CFsmClass<CAi>::SetScriptData(m_pStateMachine, _nState, _nOutPutState,
    //     eVarDataType, _nVariable, eConditionFunc, nIntData, fFloatData, _nTransitionIndex - 1);
}

// ============================================================================
// _CombineReservedConditions IDA 0x1402642F0 -> 0x14026445D
// 组合保留条件 - 将多个保留条件组合并设置到状态机
// ============================================================================
void CAi::_CombineReservedConditions(int _nState, int _nOutPutState,
                                      int nIndex1, int nIndex2, int nIndex3,
                                      int nIndex4, int nIndex5) {
    // IDA 反编译确认:
    // 获取指定索引的保留条件并调用 CFsmClass<CAi>::SetScriptDataEx

    // CFsmCondition* pCondition1 = nullptr;
    // CFsmCondition* pCondition2 = nullptr;
    // CFsmCondition* pCondition3 = nullptr;
    // CFsmCondition* pCondition4 = nullptr;
    // CFsmCondition* pCondition5 = nullptr;

    // if (nIndex1 >= 0 && nIndex1 < 20)
    //     pCondition1 = &m_arReservedCondition[nIndex1];
    // if (nIndex2 >= 0 && nIndex2 < 20)
    //     pCondition2 = &m_arReservedCondition[nIndex2];
    // if (nIndex3 >= 0 && nIndex3 < 20)
    //     pCondition3 = &m_arReservedCondition[nIndex3];
    // if (nIndex4 >= 0 && nIndex4 < 20)
    //     pCondition4 = &m_arReservedCondition[nIndex4];
    // if (nIndex5 >= 0 && nIndex5 < 20)
    //     pCondition5 = &m_arReservedCondition[nIndex5];

    // CFsmClass<CAi>::SetScriptDataEx(m_pStateMachine, _nState, _nOutPutState,
    //     pCondition1, pCondition2, pCondition3, pCondition4, pCondition5);
}

// ============================================================================
// RegisterSkillConditions IDA 0x140264460 -> 0x140264902
// 注册技能条件 - 为技能转换创建并注册条件
// ============================================================================
void CAi::RegisterSkillConditions(int _nSkillIndex, unsigned int _nVariable,
                                  const char* _szConditionString, float _fFloatData1,
                                  float _fFloatData2, int _nSkillGroup) {
    // IDA 反编译确认:
    // 1. 检查参数有效性
    // 2. 解析条件字符串
    // 3. 确定数据类型
    // 4. 创建 CFsmCondition 并设置属性
    // 5. 添加到对应的技能转换

    if (_nSkillIndex <= 0 || _nVariable > 0x38) {
        return;
    }

    int _nSkillIndexa = _nSkillIndex - 1;  // 转换为0基索引

    // 解析条件字符串
    int eConditionFunc = 0;  // CONDITION_NONE

    if (_szConditionString) {
        if (std::strcmp(_szConditionString, "=") == 0) {
            eConditionFunc = 1;  // CONDITION_EQUAL
        } else if (std::strcmp(_szConditionString, "!=") == 0) {
            eConditionFunc = 2;  // CONDITION_NOT_EQUAL
        } else if (std::strcmp(_szConditionString, ">") == 0) {
            eConditionFunc = 3;  // CONDITION_GREATER_THAN
        } else if (std::strcmp(_szConditionString, "<") == 0) {
            eConditionFunc = 4;  // CONDITION_LESS_THAN
        } else if (std::strcmp(_szConditionString, "&") == 0) {
            eConditionFunc = 5;  // CONDITION_BIT_EQUAL
        } else if (std::strcmp(_szConditionString, "<>") == 0) {
            eConditionFunc = 6;  // CONDITION_RANGE_TRUE
        } else if (std::strcmp(_szConditionString, "!<>") == 0) {
            eConditionFunc = 7;  // CONDITION_RANGE_FALSE
        } else if (std::strcmp(_szConditionString, "&=") == 0) {
            eConditionFunc = 8;  // CONDITION_RANGE_EQUAL
        }
    }

    if (eConditionFunc == 0) {
        return;  // 无效条件
    }

    // 确定数据类型
    int eVarDataType = 0;  // FSMDTYPE_NONE
    int nIntData[2] = {0, 0};
    float fFloatData[2] = {0.0f, 0.0f};

    if (m_arConditionIntFuncs[_nVariable] != nullptr) {
        eVarDataType = 1;  // FSMDTYPE_INT
        if (eConditionFunc < 6 && _fFloatData2 != 0.0f) {
            // TODO: if (!IsVarNeedCondition(_nVariable))
            //     eVarDataType = 2;  // FSMDTYPE_RANDOMINT
        }
        nIntData[0] = static_cast<int>(_fFloatData1 + 0.5f);
        nIntData[1] = static_cast<int>(_fFloatData2 + 0.5f);
    } else if (m_arConditionFloatFuncs[_nVariable] != nullptr) {
        eVarDataType = 3;  // FSMDTYPE_FLOAT
        if (eConditionFunc < 6 && _fFloatData2 != 0.0f) {
            // TODO: if (!IsVarNeedCondition(_nVariable))
            //     eVarDataType = 4;  // FSMDTYPE_RANDOMFLOAT
        }
        fFloatData[0] = _fFloatData1;
        fFloatData[1] = _fFloatData2;
    }

    // 创建条件对象
    // CFsmCondition* pCondition = new CFsmCondition();
    // CFsmCondition::SetVariables(pCondition, _nVariable);
    // CFsmCondition::SetType(pCondition, eVarDataType);
    // CFsmCondition::SetValue(pCondition, nIntData, fFloatData);
    // CFsmCondition::SetFunctions(pCondition, eConditionFunc);

    // 添加到对应的转换
    // if (_nSkillGroup == -1) {
    //     CFsmTransition::AddCondition(m_arSkillTransition[_nSkillIndexa], pCondition);
    // } else if (_nSkillGroup < 10) {
    //     CFsmTransition::AddCondition(m_arSkillGroupTransition[_nSkillGroup][_nSkillIndexa], pCondition);
    // }
}

// ============================================================================
// RegisterActionAfterSkill IDA 0x140264910 -> 0x140264DC7
// 注册技能后动作 - 为技能后动作转换创建并注册条件
// ============================================================================
void CAi::RegisterActionAfterSkill(int _nSkillIndex, int _nNextState, unsigned int _nVariable,
                                   const char* _szConditionString, float _fFloatData1,
                                   float _fFloatData2, int _nSkillGroup) {
    // IDA 反编译确认:
    // 类似 RegisterSkillConditions，但设置输出状态并添加到 ActionAfterSkillTransition

    if (_nSkillIndex <= 0 || _nVariable > 0x38) {
        return;
    }

    int _nSkillIndexa = _nSkillIndex - 1;

    // 解析条件字符串
    int eConditionFunc = 0;

    if (_szConditionString) {
        if (std::strcmp(_szConditionString, "=") == 0) {
            eConditionFunc = 1;
        } else if (std::strcmp(_szConditionString, "!=") == 0) {
            eConditionFunc = 2;
        } else if (std::strcmp(_szConditionString, ">") == 0) {
            eConditionFunc = 3;
        } else if (std::strcmp(_szConditionString, "<") == 0) {
            eConditionFunc = 4;
        } else if (std::strcmp(_szConditionString, "&") == 0) {
            eConditionFunc = 5;
        } else if (std::strcmp(_szConditionString, "<>") == 0) {
            eConditionFunc = 6;
        } else if (std::strcmp(_szConditionString, "!<>") == 0) {
            eConditionFunc = 7;
        } else if (std::strcmp(_szConditionString, "&=") == 0) {
            eConditionFunc = 8;
        }
    }

    if (eConditionFunc == 0) {
        return;
    }

    // 确定数据类型
    int eVarDataType = 0;
    int nIntData[2] = {0, 0};
    float fFloatData[2] = {0.0f, 0.0f};

    if (m_arConditionIntFuncs[_nVariable] != nullptr) {
        eVarDataType = 1;
        if (eConditionFunc < 6 && _fFloatData2 != 0.0f) {
            // 可能为 RANDOMINT
        }
        nIntData[0] = static_cast<int>(_fFloatData1 + 0.5f);
        nIntData[1] = static_cast<int>(_fFloatData2 + 0.5f);
    } else if (m_arConditionFloatFuncs[_nVariable] != nullptr) {
        eVarDataType = 3;
        if (eConditionFunc < 6 && _fFloatData2 != 0.0f) {
            // 可能为 RANDOMFLOAT
        }
        fFloatData[0] = _fFloatData1;
        fFloatData[1] = _fFloatData2;
    }

    // 创建条件对象
    // CFsmCondition* pCondition = new CFsmCondition();
    // CFsmCondition::SetVariables(pCondition, _nVariable);
    // CFsmCondition::SetType(pCondition, eVarDataType);
    // CFsmCondition::SetValue(pCondition, nIntData, fFloatData);
    // CFsmCondition::SetFunctions(pCondition, eConditionFunc);
    // CFsmCondition::SetSkill(pCondition, _nSkillGroup, _nSkillIndexa);

    // 添加到 ActionAfterSkillTransition
    // CFsmTransition::AddCondition(m_arActionAfterSkillTransition[_nSkillGroup][_nSkillIndexa], pCondition);
    // CFsmTransition::SetOutputState(m_arActionAfterSkillTransition[_nSkillGroup][_nSkillIndexa], _nNextState);
}
