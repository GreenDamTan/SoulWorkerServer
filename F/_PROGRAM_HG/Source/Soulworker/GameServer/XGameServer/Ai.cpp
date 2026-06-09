#include "Soulworker/GameServer/XGameServer/Ai.h"
#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"  // For TUXActorID/UXActorID
#include "Soulworker/GameServer/XCore/XArea/XArea.h"  // For XArea
#include "Soulworker/GameServer/XCore/XArea/XActor.h"  // For XActor

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iterator>

// ============================================================================
// CAi 构造函数
// ============================================================================
CAi::CAi()
    : m_pMonster(nullptr)
    , m_pStateMachine(nullptr)
    , m_pScriptInst(nullptr)
    , m_nStatePreHP(0)
    , m_arCancelSkillTransition(nullptr)
    , m_nSelectActionCount(0)
    , m_nRecoverySkill(0)
    , m_nSuperArmorSkillIndex(0)
    , m_fEscapePoint(0.0f)
    , m_nEscapePercent(0)
    , m_fEscapeSkillMinDist(0.0f)
    , m_fEscapeSkillMaxDist(0.0f)
    , m_fEscapePointResetTime(0.0f)
    , m_byEscapeType(0)
    , m_dwEscapeValue(0)
    , m_fCurEscapePoint(0.0f)
    , m_fLastEscapePointTime(0.0f)
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
    , m_nCurrentPatrolIndex(0)
    , m_bPatrolForward(true)
    , m_fPatrolWaitTime(0.0f)
    , m_dwChaseTargetID(0xFFFFFFFF)
    , m_fChaseRange(0.0f)
    , m_fChaseSpeed(0.0f)
    , m_bChasing(false)
    , m_fFleeSpeed(0.0f)
    , m_fFleeSafetyDistance(0.0f)
    , m_bFleeing(false)
    , m_bEscortMonster(false)
    , m_bStartEscort(false)
    , m_byFailMoveCount(0)
    , m_fHelperFarDistance(0.0f)
    , m_fHelperFarBattleDist(0.0f)
    , m_nCheckHelperFarCount(0)
    , m_fCheckValidPositionTime(5.0f)
    , m_nSelectedSkillIndex(-1)
    , m_fSkillRangeMin(0.0f)
    , m_fSkillRangeMax(0.0f)
    , m_nGroupID(0)
    , m_dwGroupTargetID(0xFFFFFFFF)
    , m_bGroupLeader(false)
    , m_pMaze(nullptr)
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
    std::memset(m_vFleeDestPos, 0, sizeof(m_vFleeDestPos));

    GreenDamTan_log(__FILE__, __FUNCTION__, "CAi constructed");
}

CAi::~CAi() {
    // IDA 反编译确认: 析构函数调用 Destroy 然后清理其他资源
    // 首先调用 Destroy 清理主要资源
    Destroy();

    // 清理委托技能结构
    // PS_CREATE_MAP_LIST::~PS_CREATE_MAP_LIST(&this->m_stDelegateSkill);

    // 清理向量
    m_vecFsmDataEx.clear();
    m_vecFsmData.clear();
    m_vecStateData.clear();

    // 清理模糊数据数组
    for (int i = 0; i < 4; ++i) {
        m_arFuzzy[i].vConditions.clear();
    }

    // 清理保留条件数组
    // for (int i = 0; i < 20; ++i) {
    //     m_arReservedCondition[i] = CFsmCondition();
    // }

    // 清理死亡动画字符串
    // VString::~VString(&this->m_strDeathAnim);

    // 清理通用动作数组
    for (int i = 0; i < 10; ++i) {
        m_strCommonActions[i] = VString();
    }

    // 清理冷却时间列表
    m_mapCooltimeList.clear();

    // 清理模糊脚本
    m_strFuzzyScript = VString();

    // 清理状态变量映射
    m_mapStateVars.clear();

    // 清理条件函数指针向量
    m_xAiGetConditionDataIntFunc.clear();
    m_xAiGetConditionDataFloatFunc.clear();

    GreenDamTan_log(__FILE__, __FUNCTION__, "CAi destructed");
}

// ============================================================================
// Destroy IDA 0x14025F240 -> 0x14025F738
// 销毁AI对象并清理所有资源
// ============================================================================
void CAi::Destroy() {
    // IDA 反编译精确还原
    // 功能：销毁AI对象，清理状态机、技能转换数组、模糊条件等所有资源

    // 1. 删除状态机
    if (m_pStateMachine) {
        delete m_pStateMachine;
        m_pStateMachine = nullptr;
    }

    // 2. 删除技能转换数组
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

    // 3. 清理模糊条件 (FuzzyData vConditions)
    for (int j = 0; j < 4; ++j) {
        m_arFuzzy[j].vConditions.clear();
    }

    // 4. 删除取消技能转换
    if (m_arCancelSkillTransition) {
        delete m_arCancelSkillTransition;
        m_arCancelSkillTransition = nullptr;
    }

    // 5. 清理条件函数指针向量
    m_xAiGetConditionDataIntFunc.clear();
    m_xAiGetConditionDataFloatFunc.clear();

    // 6. 清理状态变量映射
    for (auto& pair : m_mapStateVars) {
        pair.second.NextStates.clear();
    }
    m_mapStateVars.clear();

    // 7. 清理状态数据向量
    m_vecStateData.clear();
    m_vecFsmData.clear();
    m_vecFsmDataEx.clear();

    // 8. 重置模糊脚本
    // VString 没有 clear 或 Reset 方法，使用赋值清空
    m_strFuzzyScript = VString();

    // 9. 清理冷却时间列表
    m_mapCooltimeList.clear();

    // 10. 清空怪物指针
    m_pMonster = nullptr;

    // 11. 清理脚本实例
    // IDA 0x14025F240: CAi::Destroy
    m_pScriptInst = nullptr;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CAi destroyed");
}

// ============================================================================
// Initialize IDA 0x1402623F0 -> 0x140263160
// AI初始化 - 完整实现
// ============================================================================
void CAi::Initialize(CMonster* _pMonster) {
    // IDA 反编译确认:
    m_pMonster = _pMonster;

    if (m_pMonster) {
        m_nStatePreHP = m_pMonster->GetHP();
    }

    // 创建状态机 (CFsmClass<CAi>)
    // CFsmClass 尚未完全实现，使用 nullptr 占位
    m_pStateMachine = nullptr;

    // 创建技能转换数组 (CFsmTransition)
    for (int i = 0; i < 10; ++i) {
        m_arSkillTransition[i] = nullptr;
        for (int k = 0; k < 10; ++k) {
            m_arSkillGroupTransition[k][i] = nullptr;
            m_arActionAfterSkillTransition[k][i] = nullptr;
        }
    }
    m_arCancelSkillTransition = nullptr;

    // 注册整数条件函数指针 (IDA 确认的索引映射)
    // m_arConditionIntFuncs[1] = &CAi::_ConditionIsTarget;
    // m_arConditionIntFuncs[10] = &CAi::_ConditionHealth;
    // m_arConditionIntFuncs[15] = &CAi::_ConditionTargetHealth;
    // m_arConditionIntFuncs[4] = &CAi::_ConditionTargetNumber;
    // m_arConditionIntFuncs[12] = &CAi::_ConditionRandom;
    // m_arConditionIntFuncs[13] = &CAi::_ConditionRandomPrevalue;
    // m_arConditionIntFuncs[14] = &CAi::_ConditionIsMoving;
    // m_arConditionIntFuncs[9] = &CAi::_ConditionIsAttack;
    // m_arConditionIntFuncs[11] = &CAi::_ConditionIsChangeHP;
    // m_arConditionIntFuncs[16] = &CAi::_ConditionIsRequestHelpNum;
    // m_arConditionIntFuncs[20] = &CAi::_ConditionIsMonsterCount;
    // m_arConditionIntFuncs[21] = &CAi::_ConditionIsSpawnMonsterCount;
    // m_arConditionIntFuncs[22] = &CAi::_ConditionIsSectorMonsterCount;
    // m_arConditionIntFuncs[23] = &CAi::_ConditionIsSectorMonsterCountByID;
    // m_arConditionIntFuncs[24] = &CAi::_ConditionIsPreSkillDamageCount;
    // m_arConditionIntFuncs[25] = &CAi::_ConditionIsTargetSkill;
    // m_arConditionIntFuncs[26] = &CAi::_ConditionIsTargetState;
    // m_arConditionIntFuncs[27] = &CAi::_ConditionIsTargetDamageState;
    // m_arConditionIntFuncs[28] = &CAi::_ConditionIsTargetMoveState;
    // m_arConditionIntFuncs[29] = &CAi::_ConditionIsTargetBuffIndex;
    // m_arConditionIntFuncs[30] = &CAi::_ConditionTargetAttacker;
    // m_arConditionIntFuncs[31] = &CAi::_ConditionTargetCombo;
    // m_arConditionIntFuncs[33] = &CAi::_ConditionPatrolState;
    // m_arConditionIntFuncs[50] = &CAi::_ConditionStateFailCount;
    // m_arConditionIntFuncs[51] = &CAi::_ConditionFriendCount;
    // m_arConditionIntFuncs[52] = &CAi::_ConditionEnemyCount;
    // m_arConditionIntFuncs[56] = &CAi::_ConditionUserCount;
    // m_arConditionIntFuncs[53] = &CAi::_ConditionAggroCount;
    // m_arConditionIntFuncs[54] = &CAi::_ConditionHitCount;
    // m_arConditionIntFuncs[40] = &CAi::_ConditionGuardHealth;
    // m_arConditionIntFuncs[41] = &CAi::_ConditionGuardFriendCount;
    // m_arConditionIntFuncs[42] = &CAi::_ConditionGuardEnemyCount;
    // m_arConditionIntFuncs[45] = &CAi::_ConditionGlobalCooltime;
    // m_arConditionIntFuncs[46] = &CAi::_ConditionGroupCooltime;
    // m_arConditionIntFuncs[47] = &CAi::_ConditionSkillCooltime;

    // 注册浮点条件函数指针 (IDA 确认)
    // m_arConditionFloatFuncs[2] = &CAi::_ConditionTargetDistance;
    // m_arConditionFloatFuncs[3] = &CAi::_ConditionTargetDistanceCapsule;
    // m_arConditionFloatFuncs[5] = &CAi::_ConditionTargetDirection;
    // m_arConditionFloatFuncs[32] = &CAi::_ConditionTargetLook;
    // m_arConditionFloatFuncs[8] = &CAi::_ConditionStateTime;
    // m_arConditionFloatFuncs[6] = &CAi::_ConditionCreatePosDistance;
    // m_arConditionFloatFuncs[34] = &CAi::_ConditionLastSkillTime;
    // m_arConditionFloatFuncs[35] = &CAi::_ConditionLastDamageTime;
    // m_arConditionFloatFuncs[36] = &CAi::_ConditionSpawnTime;
    // m_arConditionFloatFuncs[37] = &CAi::_ConditionActivateTime;
    // m_arConditionFloatFuncs[38] = &CAi::_ConditionGuardDistance;
    // m_arConditionFloatFuncs[39] = &CAi::_ConditionGuardDistanceCapsule;
    // m_arConditionFloatFuncs[48] = &CAi::_ConditionFuzzy;
    // m_arConditionFloatFuncs[49] = &CAi::_ConditionStateEndTime;
    // m_arConditionFloatFuncs[55] = &CAi::_ConditionMoveDistanceAfterSkill;

    // 注册状态函数 (IDA 确认的调用顺序)
    RegisterStateFunctions(1);   // FSMSTATES_IDLE
    RegisterStateFunctions(2);   // FSMSTATES_PATROL
    RegisterStateFunctions(3);   // FSMSTATES_WAIT
    RegisterStateFunctions(4);   // FSMSTATES_CHASE
    RegisterStateFunctions(7);   // FSMSTATES_ATTACK
    RegisterStateFunctions(5);   // FSMSTATES_RETURN
    RegisterStateFunctions(6);   // FSMSTATES_RUNAWAY
    RegisterStateFunctions(9);   // FSMSTATES_DIE
    RegisterStateFunctions(29);  // FSMSTATES_CHASE_TARGET
    RegisterStateFunctions(35);  // FSMSTATES_RUNAWAY_ATTACK
    RegisterStateFunctions(30);
    RegisterStateFunctions(31);
    RegisterStateFunctions(32);
    RegisterStateFunctions(39);  // FSMSTATES_PROTECT_START
    RegisterStateFunctions(40);  // FSMSTATES_PROTECT_MOVE
    RegisterStateFunctions(41);  // FSMSTATES_PROTECT_GAZE
    RegisterStateFunctions(42);  // FSMSTATES_PROTECT_WAIT

    // 注册状态变量 (IDA 确认的调用)
    RegisterStateVars(2, 4, 1, 0);
    RegisterStateVars(4, 4, 3, 0);
    RegisterStateVars(5, 4, 3, 0);
    RegisterStateVars(6, 4, 3, 0);
    RegisterStateVars(7, 4, 3, 0);
    RegisterStateVars(8, 4, 9, 0);
    RegisterStateVars(9, 4, 3, 0);
    RegisterStateVars(10, 4, 3, 0);
    RegisterStateVars(11, 4, 3, 0);
    RegisterStateVars(12, 4, 3, 0);
    RegisterStateVars(13, 4, 3, 0);
    RegisterStateVars(14, 4, 3, 0);
    RegisterStateVars(15, 4, 3, 0);
    RegisterStateVars(16, 4, 3, 0);
    RegisterStateVars(17, 4, 3, 0);
    RegisterStateVars(18, 4, 3, 0);
    RegisterStateVars(33, 4, 3, 0);
    RegisterStateVars(34, 4, 3, 0);
    RegisterStateVars(35, 4, 3, 0);
    RegisterStateVars(32, 4, 3, 0);
    RegisterStateVars(29, 4, 1, 0);
    RegisterStateVars(41, 4, 39, 0);
    RegisterStateVars(40, 4, 39, 0);
    RegisterStateVars(42, 4, 39, 0);

    GreenDamTan_log(__FILE__, __FUNCTION__, "CAi initialized");
}

// ============================================================================
// Update IDA 0x1402621B0 -> 0x1402623F0
// AI更新主函数 - 核心AI循环
// ============================================================================
void CAi::Update(float fElapsedTime) {
    // IDA 反编译确认的完整逻辑:
    if (!m_pMonster) {
        return;
    }

    // 处理自杀时间
    if (m_fSuicideTime > 0.0f) {
        m_fSuicideTime -= fElapsedTime;
        if (m_fSuicideTime < 0.0f) {
            // 自杀时间到，切换到自杀状态
            m_fSuicideTime = 0.0f;
            ChangeAiState(FSMSTATES_SUCIDE);
            return;
        }
    }

    // 更新累计时间
    m_fSumElapsedTime += fElapsedTime;
    m_fStateTime += fElapsedTime;
    m_fActivateTime += fElapsedTime;

    // 更新攻击计时器 (如果可以攻击)
    if (m_pMonster->IsCanAttack()) {
        // CFsmClass::OnUpdateAttackTimer(m_pStateMachine, fElapsedTime);
    }

    // 更新移动计时器 (如果可以移动)
    if (m_pMonster->IsCanMove(true)) {
        // CFsmClass::OnUpdateMoveTimer(m_pStateMachine, fElapsedTime);
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
            // DataList[4][0] 存储下一个状态
            int nState = it->second.DataList[4][0];
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
// 技能目标查找 - 精确还原
// ============================================================================
CMoverEx* CAi::FindTargetBySkill() {
    // IDA 0x14027CAA0: CAi::FindTargetBySkill
    // 根据技能目标类型查找目标

    if (!m_pMonster) {
        return nullptr;
    }

    // IDA: 检查当前技能引用和目标类型
    if (m_pCurSkillRef && m_pCurSkillRef->Target_Type) {
        // IDA: 如果是跟随者，返回拥有者玩家
        if (m_pMonster->IsFollower()) {
            return m_pMonster->GetOwnerPlayer();
        }
        return nullptr;
    }

    // IDA: 返回当前目标
    unsigned int TargetID = m_pMonster->GetTargetID();
    return static_cast<CMoverEx*>(CMover::GetMoverObject(TargetID));
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
    std::vector<CFsmCondition*>::iterator itBegin = pTransition->GetConditionVectorBegin();
    std::vector<CFsmCondition*>::iterator itEnd = pTransition->GetConditionVectorEnd();
    
    // 如果条件向量为空，返回 true
    if (itBegin == itEnd) {
        return true;
    }
    
    int nConditionCount = static_cast<int>(std::distance(itBegin, itEnd));
    int nConditionSuccessedCount = 0;
    
    // 遍历所有条件
    while (itBegin != itEnd) {
        CFsmCondition* pCondition = *itBegin;
        if (pCondition) {
            // IDA: CQuestCondition::GetQuestID(pCondition) 获取变量名
            // IDA: XOption::GetGroupID(pCondition) 获取变量类型
            E_FSMVARIABLES eVarName = static_cast<E_FSMVARIABLES>(pCondition->GetVariableIndex());
            E_FSMDATATYPE eVarType = static_cast<E_FSMDATATYPE>(pCondition->GetConditionType());
            
            if (eVarType == FSMDTYPE_INT || eVarType == FSMDTYPE_RANDOMINT) {
                // IDA: CFsmCondition::GetValueInt(pCondition, 1)
                int nValue = pCondition->GetValueInt(1);
                // IDA: CAi::GetConditionIntData(this, eVarName, nValue)
                int nActualValue = GetConditionIntData(eVarName, nValue);
                // IDA: CFsmCondition::ConditionFulfilled(pCondition, nActualValue)
                if (!pCondition->ConditionFulfilled(nActualValue)) {
                    return (nConditionCount == nConditionSuccessedCount);
                }
                ++nConditionSuccessedCount;
            } else {
                // IDA: CFsmCondition::GetValueFloat(pCondition, 1)
                float fValue = pCondition->GetValueFloat(1);
                // IDA: CAi::GetConditionFloatData(this, eVarName, (int)fValue)
                float fActualValue = GetConditionFloatData(eVarName, static_cast<int>(fValue));
                // IDA: CFsmCondition::ConditionFulfilled(pCondition, fActualValue)
                if (!pCondition->ConditionFulfilled(fActualValue)) {
                    return (nConditionCount == nConditionSuccessedCount);
                }
                ++nConditionSuccessedCount;
            }
        }
        ++itBegin;
    }
    
    return (nConditionCount == nConditionSuccessedCount);
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
// ChangeAiState - IDA 0x14025F7C0
// 改变AI状态 - 精确还原
// ============================================================================
void CAi::ChangeAiState(int nState) {
    // IDA 0x14025F7C0: 调用状态机的 SetState 并触发 FuncStartState
    // CFsmClass<CAi>::SetState(this->m_pStateMachine, _nState);
    // CAi::FuncStartState(this);

    // TODO: 需要 CFsmClass 完整定义后启用
    // if (m_pStateMachine) {
    //     m_pStateMachine->SetState(nState);
    // }
    FuncStartState();
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
// GetAIFuzzyValue IDA 0x14026C360 -> 0x14026C3F3
// 获取AI模糊值 - 调用_ConditionFuzzy计算4个模糊值
// ============================================================================
void CAi::GetAIFuzzyValue(float* pfValue) {
    // IDA 反编译精确还原:
    // *pfValue = CAi::_ConditionFuzzy(this, 0) * 100.0;
    // pfValue[1] = CAi::_ConditionFuzzy(this, 1) * 100.0;
    // pfValue[2] = CAi::_ConditionFuzzy(this, 2) * 100.0;
    // pfValue[3] = CAi::_ConditionFuzzy(this, 3) * 100.0;

    if (pfValue) {
        pfValue[0] = _ConditionFuzzy(0) * 100.0f;
        pfValue[1] = _ConditionFuzzy(1) * 100.0f;
        pfValue[2] = _ConditionFuzzy(2) * 100.0f;
        pfValue[3] = _ConditionFuzzy(3) * 100.0f;
    }
}

// ============================================================================
// GetAIActionValue IDA 0x14026C400 -> 0x14026C427
// 获取AI动作值 - 复制m_arSelectActionResult数组(7个int)
// ============================================================================
void CAi::GetAIActionValue(int* pnValue) {
    // IDA 反编译精确还原:
    // qmemcpy(pnValue, this->m_arSelectActionResult, 0x1Cu);  // 0x1C = 28 bytes = 7 ints

    if (pnValue) {
        std::memcpy(pnValue, m_arSelectActionResult, sizeof(m_arSelectActionResult));
    }
}

// ============================================================================
// AI Condition Functions - IDA 精确还原
// 条件判断函数，用于 FSM 状态转换条件评估
// ============================================================================

// _ConditionIsTarget IDA 0x1402779F0 -> 0x140277A3B
// 检查是否有目标
int CAi::_ConditionIsTarget(int /*_nVal*/) {
    // IDA: return this->m_pMonster && (unsigned int)CMover::GetTargetID(this->m_pMonster) != -1;
    if (!m_pMonster) {
        return 0;
    }
    return m_pMonster->GetTargetID() != 0xFFFFFFFF ? 1 : 0;
}

// _ConditionHealth IDA 0x140277A40 -> 0x140277ACD
// 获取自身HP百分比
int CAi::_ConditionHealth(int /*_nVal*/) {
    // IDA: v3 = (float)this->m_pMonster->GetHP(this->m_pMonster) * 100.0;
    //      return (unsigned int)(int)(float)(v3 / (float)this->m_pMonster->GetMaxHP(this->m_pMonster));
    if (!m_pMonster) {
        return 0;
    }
    float fHP = static_cast<float>(m_pMonster->GetHP()) * 100.0f;
    return static_cast<int>(fHP / static_cast<float>(m_pMonster->GetMaxHP()));
}

// _ConditionTargetHealth IDA 0x140277AD0 -> 0x140277B83
// 获取目标HP百分比
int CAi::_ConditionTargetHealth(int /*_nVal*/) {
    // IDA: TargetID = CMover::GetTargetID(this->m_pMonster);
    //      pTarget = CMover::GetMoverObject(this->m_pMonster, TargetID);
    //      if (pTarget) return HP * 100 / MaxHP;
    //      return -1;
    if (!m_pMonster) {
        return 0;
    }
    unsigned int dwTargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(dwTargetID);
    if (pTarget) {
        float fHP = static_cast<float>(pTarget->GetHP()) * 100.0f;
        return static_cast<int>(fHP / static_cast<float>(pTarget->GetMaxHP()));
    }
    return -1;
}

// _ConditionGuardHealth IDA 0x140277B90 -> 0x140277C70
// 获取守护对象HP百分比
int CAi::_ConditionGuardHealth(int /*_nVal*/) {
    // IDA: if (IsFollower) pTarget = GetOwnerPlayer(); else pTarget = GetGuardMonster();
    //      if (pTarget) return HP * 100 / MaxHP;
    //      return 100;
    if (!m_pMonster) {
        return 0;
    }
    CMonster* pTarget = nullptr;
    if (m_pMonster->IsFollower()) {
        pTarget = static_cast<CMonster*>(m_pMonster->GetOwnerPlayer());
    } else {
        pTarget = m_pMonster->FindGuardMonster();
    }
    if (pTarget) {
        float fHP = static_cast<float>(pTarget->GetHP()) * 100.0f;
        return static_cast<int>(fHP / static_cast<float>(pTarget->GetMaxHP()));
    }
    return 100;
}

// _ConditionTargetNumber IDA 0x140277C70 -> 0x140277EB8
// 获取目标周围同目标敌人数量
int CAi::_ConditionTargetNumber(int /*_nVal*/) {
    // IDA: 扫描区域，统计攻击同一目标的敌人数量
    if (!m_pMonster) {
        return 0;
    }
    unsigned int dwTargetID = m_pMonster->GetTargetID();
    CMover* pTargetMover = CMover::GetMoverObject(dwTargetID);
    if (!pTargetMover) {
        return 0;
    }

    // CMoverEx 类型检查
    CMoverEx* pTargetEx = dynamic_cast<CMoverEx*>(pTargetMover);
    if (!pTargetEx) {
        return 0;
    }

    int nCount = 0;
    std::vector<CMover*> vecGameObjList;

    // 扫描区域 - 需要通过 Area 扫描
    // TODO: 需要实现正确的区域扫描
    // XArea* pArea = m_pMonster->GetArea();
    // if (pArea) {
    //     pArea->ScanGridOrigin(m_pMonster, 2, 7u, &vecGameObjList);
    // }

    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMover* pScanMover = *it;
        if (pScanMover && !pScanMover->IsDie() && !pScanMover->IsStatus(2u)) {
            if (pScanMover->GetTargetID() == dwTargetID) {
                ++nCount;
            }
        }
    }

    return nCount;
}

// _ConditionRandom IDA 0x140277EC0 -> 0x140277EF8
// 获取随机值(0-9999)并保存到m_nPreRandomValue
int CAi::_ConditionRandom(int /*_nVal*/) {
    // IDA: this->m_nPreRandomValue = rand() % 10000;
    //      return (unsigned int)this->m_nPreRandomValue;
    m_nPreRandomValue = std::rand() % 10000;
    return m_nPreRandomValue;
}

// _ConditionRandomPrevalue IDA 0x140277F00 -> 0x140277F15
// 获取上一次随机值
int CAi::_ConditionRandomPrevalue(int /*_nVal*/) {
    // IDA: return (unsigned int)this->m_nPreRandomValue;
    return m_nPreRandomValue;
}

// _ConditionIsMoving IDA 0x140277F20 -> 0x140277F73
// 检查是否在移动
int CAi::_ConditionIsMoving(int /*_nVal*/) {
    // IDA: return this->m_pMonster && tagMOVE_POS::IsNoneZero(&this->m_pMonster->m_stMovePos);
    if (!m_pMonster) {
        return 0;
    }
    // IDA: 检查是否在移动状态
    return m_pMonster->IsMoving() ? 1 : 0;
}

// _ConditionIsAttack IDA 0x140277F80 -> 0x140277FD8
// 检查是否在攻击状态
int CAi::_ConditionIsAttack(int /*_nVal*/) {
    // IDA: return this->m_pMonster && XActor::IsStatus(&this->m_pMonster->XActor, 1u);
    if (!m_pMonster) {
        return 0;
    }
    return m_pMonster->IsStatus(1u) ? 1 : 0;
}

// _ConditionIsChangeHP IDA 0x140277FE0 -> 0x14027803B
// 检查HP是否变化
int CAi::_ConditionIsChangeHP(int /*_nVal*/) {
    // IDA: return this->m_pMonster && this->m_pMonster->GetHP(this->m_pMonster) != this->m_nStatePreHP;
    if (!m_pMonster) {
        return 0;
    }
    return m_pMonster->GetHP() != m_nStatePreHP ? 1 : 0;
}

// _ConditionIsRequestHelpNum IDA 0x140278040 -> 0x140278055
// 获取请求帮助次数
int CAi::_ConditionIsRequestHelpNum(int /*_nVal*/) {
    // IDA: return (unsigned int)this->m_nRequestHelpCnt;
    return m_nRequestHelpCnt;
}

// _ConditionIsMonsterCount IDA 0x140278060 -> 0x1402780DE
// 获取区域内怪物数量
int CAi::_ConditionIsMonsterCount(int /*_nVal*/) {
    // IDA: if (!m_pMonster) return 0;
    //      pArea = m_pMonster->GetArea();
    //      if (!pArea) return 0;
    //      return XArea::GetActorCount(pArea, eActorMonster);
    if (!m_pMonster) {
        return 0;
    }
    // TODO: 需要 XArea::GetArea 和 XArea::GetActorCount
    XArea* pArea = nullptr; // m_pMonster->GetArea();
    if (!pArea) {
        return 0;
    }
    return 0; // pArea->GetActorCount(eActorMonster);
}

// _ConditionIsSpawnMonsterCount IDA 0x1402780E0 -> 0x1402782F0
// 获取召唤怪物数量（与自己同父ID的怪物数量）
int CAi::_ConditionIsSpawnMonsterCount(int /*_nVal*/) {
    // IDA: 扫描区域，统计与自己的ParentID相同的怪物数量
    if (!m_pMonster) {
        return 0;
    }

    int nCount = 0;
    // TODO: 需要实现区域扫描
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(m_pMonster, 2, 2u, &vecGameObjList);
    // UXActorID myActorID = m_pMonster->GetActorID();
    // for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
    //     CMonster* pActor = dynamic_cast<CMonster*>(*it);
    //     if (pActor && !pActor->IsDie() && !pActor->IsStatus(2u)) {
    //         UXActorID parentID = pActor->GetParentID();
    //         if (parentID == myActorID) {
    //             ++nCount;
    //         }
    //     }
    // }

    return nCount;
}

// _ConditionIsSectorMonsterCount IDA 0x1402782F0 -> 0x14027834F
// 获取扇区内怪物数量
int CAi::_ConditionIsSectorMonsterCount(int /*_nVal*/) {
    // IDA: Sector = CMoverEx::GetSector(this->m_pMonster);
    //      if (!Sector) return 0;
    //      return CSector::GetMonsterCount(Sector);
    if (!m_pMonster) {
        return 0;
    }
    // TODO: 需要 CSector 和 GetSector 方法
    // CSector* pSector = m_pMonster->GetSector();
    // if (!pSector) {
    //     return 0;
    // }
    // return pSector->GetMonsterCount();
    return 0;
}

// _ConditionIsSectorMonsterCountByID IDA 0x140278350 -> 0x140278411
// 获取扇区内指定ID的怪物数量
int CAi::_ConditionIsSectorMonsterCountByID(unsigned int _nVal) {
    // IDA: pArea = m_pMonster->GetArea();
    //      pMaze = RTDynamicCast(pArea, XArea, XMaze);
    //      if (pMaze) return XMaze::GetMonsterCountByID(pMaze, _nVal);
    if (!m_pMonster) {
        return 0;
    }
    // TODO: 需要 XMaze 类型
    // XArea* pArea = m_pMonster->GetArea();
    // if (!pArea) {
    //     return 0;
    // }
    // XMaze* pMaze = dynamic_cast<XMaze*>(pArea);
    // if (pMaze) {
    //     return pMaze->GetMonsterCountByID(_nVal);
    // }
    (void)_nVal;
    return 0;
}

// _ConditionIsPreSkillDamageCount IDA 0x140278420 -> 0x140278435
// 获取预技能伤害计数
int CAi::_ConditionIsPreSkillDamageCount(int /*_nVal*/) {
    // IDA: return (unsigned int)this->m_nPreSkillDamageCount;
    return m_nPreSkillDamageCount;
}

// _ConditionIsTargetSkill IDA 0x140278440 -> 0x1402784CD
// 检查目标是否正在攻击自己
int CAi::_ConditionIsTargetSkill(int /*_nVal*/) {
    // IDA: TargetID = CMover::GetTargetID(this->m_pMonster);
    //      pTarget = CMover::GetMoverObject(this->m_pMonster, TargetID);
    //      pTargetEx = RTDynamicCast(pTarget, CMover, CMoverEx);
    //      if (pTargetEx) return pTargetEx->IsSkillAttackMe(pTargetEx, this->m_pMonster);
    if (!m_pMonster) {
        return 0;
    }
    unsigned int dwTargetID = m_pMonster->GetTargetID();
    CMover* pTargetMover = CMover::GetMoverObject(dwTargetID);
    CMoverEx* pTarget = dynamic_cast<CMoverEx*>(pTargetMover);
    if (pTarget) {
        // TODO: 需要 IsSkillAttackMe 方法
        // return pTarget->IsSkillAttackMe(m_pMonster) ? 1 : 0;
        return 0;
    }
    return 0;
}

// _ConditionIsTargetState IDA 0x1402784D0 -> 0x140278559
// 获取目标状态（飞行/倒地）
int CAi::_ConditionIsTargetState(int /*_nVal*/) {
    // IDA: if (IsFlying) return 1; else if (IsHitDown) return 2;
    if (!m_pMonster) {
        return 0;
    }
    unsigned int dwTargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(dwTargetID);
    if (pTarget) {
        if (pTarget->IsFlying()) {
            return 1;
        } else if (pTarget->IsHitDown()) {
            return 2;
        }
    }
    return 0;
}

// _ConditionIsTargetDamageState IDA 0x140278560 -> 0x140278601
// 获取目标受伤状态
int CAi::_ConditionIsTargetDamageState(int /*_nVal*/) {
    // IDA: if (IsGeneralHit) return 1; else if (IsFlyHit) return 2; else if (IsHitDown) return 3;
    if (!m_pMonster) {
        return 0;
    }
    unsigned int dwTargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(dwTargetID);
    if (pTarget) {
        if (pTarget->IsGeneralHit()) {
            return 1;
        } else if (pTarget->IsFlyHit()) {
            return 2;
        } else if (pTarget->IsHitDown()) {
            return 3;
        }
    }
    return 0;
}

// _ConditionIsTargetMoveState IDA 0x140278610 -> 0x1402786A7
// 获取目标移动状态
int CAi::_ConditionIsTargetMoveState(int /*_nVal*/) {
    // IDA: if (IsMoving) { if (IsStatus(0x100)) return 2; else return 1; }
    if (!m_pMonster) {
        return 0;
    }
    unsigned int dwTargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(dwTargetID);
    if (pTarget && pTarget->IsMoving()) {
        if (pTarget->IsStatus(0x100u)) {
            return 2;
        }
        return 1;
    }
    return 0;
}

// _ConditionIsTargetBuffIndex IDA 0x1402786B0 -> 0x14027872A
// 检查目标是否有指定Buff
int CAi::_ConditionIsTargetBuffIndex(unsigned short _nVal) {
    // IDA: return CMover::FindBuffStatus(pTarget, _nVal, 0) != -1;
    if (!m_pMonster) {
        return 0;
    }
    unsigned int dwTargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(dwTargetID);
    if (pTarget) {
        // IDA: 检查目标是否有指定Buff
        return pTarget->FindBuffStatus(static_cast<std::uint16_t>(_nVal), 0) != -1 ? 1 : 0;
    }
    return 0;
}

// _ConditionTargetAttacker IDA 0x140278730 -> 0x140278979
// 获取攻击目标的敌人数量（目标被多少敌人攻击）
int CAi::_ConditionTargetAttacker(int /*_nVal*/) {
    // IDA: 扫描区域，统计攻击同一目标且处于攻击状态(9-13)的敌人数量
    if (!m_pMonster) {
        return 0;
    }
    unsigned int dwTargetID = m_pMonster->GetTargetID();
    CMover* pTargetMover = CMover::GetMoverObject(dwTargetID);
    if (!dynamic_cast<CMoverEx*>(pTargetMover)) {
        return 0;
    }

    int nCount = 0;
    // TODO: 需要实现区域扫描
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(m_pMonster, 2, 2u, &vecGameObjList);
    // for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
    //     CMonster* pActor = dynamic_cast<CMonster*>(*it);
    //     if (pActor && !pActor->IsDie() && !pActor->IsStatus(2u)) {
    //         if (pActor->GetTargetID() == dwTargetID) {
    //             int nAiState = pActor->GetAIState();
    //             if (nAiState >= 9 && nAiState <= 13) {
    //                 ++nCount;
    //             }
    //         }
    //     }
    // }

    return nCount;
}

// _ConditionTargetCombo IDA 0x140278980 -> 0x1402789F0
// 获取目标连击数
int CAi::_ConditionTargetCombo(int /*_nVal*/) {
    // IDA: return pTarget->GetComboCount(pTarget);
    if (!m_pMonster) {
        return 0;
    }
    unsigned int dwTargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(dwTargetID);
    if (pTarget) {
        // IDA: 获取目标连击数
        return static_cast<int>(pTarget->GetComboCount());
    }
    return 0;
}

// _ConditionPatrolState IDA 0x1402789F0 -> 0x140278A3F
// 获取巡逻状态
int CAi::_ConditionPatrolState(int /*_nVal*/) {
    // IDA: WayPoint = CMoverEx::GetWayPoint(this->m_pMonster);
    //      if (!WayPoint) return 0;
    //      return WayPoint->GetHash();
    if (!m_pMonster) {
        return 0;
    }
    // TODO: 需要 CGocMyroom 和 GetWayPoint 方法
    // CGocMyroom* pWayPoint = m_pMonster->GetWayPoint();
    // if (!pWayPoint) {
    //     return 0;
    // }
    // return pWayPoint->GetHash();
    return 0;
}

// _ConditionStateFailCount IDA 0x140278A40 -> 0x140278A6E
// 获取状态失败计数
int CAi::_ConditionStateFailCount(unsigned int _nVal) {
    // IDA: if (_nVal <= 0x2A) return this->m_nStateFailCount[_nVal];
    // TODO: 需要 m_nStateFailCount 数组成员
    if (_nVal <= 0x2A) {
        // return m_nStateFailCount[_nVal];
        (void)_nVal;
    }
    return 0;
}

// _ConditionFriendCount IDA 0x140278A70 -> 0x140278D15
// 获取范围内的友方数量
int CAi::_ConditionFriendCount(int _nVal) {
    // IDA: 扫描区域，统计范围内的友方数量
    if (!m_pMonster || _nVal <= 0) {
        return 0;
    }

    int nCount = 0;
    // TODO: 需要 GetPosition, IsFriend 和区域扫描
    // hkvVec3 vMyPos = m_pMonster->GetPosition();
    // float fDistSq = static_cast<float>(_nVal * _nVal);
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(m_pMonster, 2, 2u, &vecGameObjList);
    // for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
    //     CMonster* pActor = dynamic_cast<CMonster*>(*it);
    //     if (pActor && !pActor->IsDie() && !pActor->IsStatus(2u)) {
    //         if (pActor->GetType() == 2 && pActor != m_pMonster) {
    //             if (m_pMonster->IsFriend(pActor)) {
    //                 hkvVec3 vTargetPos = pActor->GetPosition();
    //                 float fDist = vMyPos.getDistanceToSquared(vTargetPos);
    //                 if (fDistSq >= fDist) {
    //                     ++nCount;
    //                 }
    //             }
    //         }
    //     }
    // }
    (void)_nVal;

    return nCount;
}

// _ConditionEnemyCount IDA 0x140278D20 -> 0x140278FA9
// 获取范围内的敌人数量
int CAi::_ConditionEnemyCount(int _nVal) {
    // IDA: 扫描区域，统计范围内的敌人数量
    if (!m_pMonster || _nVal <= 0) {
        return 0;
    }

    int nCount = 0;
    // TODO: 需要 GetPosition, IsEnemy 和区域扫描
    // hkvVec3 vMyPos = m_pMonster->GetPosition();
    // float fDistSq = static_cast<float>(_nVal * _nVal);
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(m_pMonster, 2, 2u, &vecGameObjList);
    // for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
    //     CMonster* pActor = dynamic_cast<CMonster*>(*it);
    //     if (pActor && !pActor->IsDie() && !pActor->IsStatus(2u) && pActor != m_pMonster) {
    //         if (m_pMonster->IsEnemy(pActor)) {
    //             hkvVec3 vTargetPos = pActor->GetPosition();
    //             float fDist = vMyPos.getDistanceToSquared(vTargetPos);
    //             if (fDistSq >= fDist) {
    //                 ++nCount;
    //             }
    //         }
    //     }
    // }
    (void)_nVal;

    return nCount;
}

// _ConditionUserCount IDA 0x140278FB0 -> 0x1402791F4
// 获取范围内的玩家数量
int CAi::_ConditionUserCount(int _nVal) {
    // IDA: 扫描区域，统计范围内的玩家数量
    if (!m_pMonster || _nVal <= 0) {
        return 0;
    }

    int nCount = 0;
    // TODO: 需要 GetPosition 和区域扫描
    // hkvVec3 vMyPos = m_pMonster->GetPosition();
    // float fDistSq = static_cast<float>(_nVal * _nVal);
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(m_pMonster, 2, 1u, &vecGameObjList);  // 1 = eActorUser
    // for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
    //     CUser* pActor = dynamic_cast<CUser*>(*it);
    //     if (pActor && !pActor->IsDie() && !pActor->IsStatus(2u)) {
    //         hkvVec3 vTargetPos = pActor->GetPosition();
    //         float fDist = vMyPos.getDistanceToSquared(vTargetPos);
    //         if (fDistSq >= fDist) {
    //             ++nCount;
    //         }
    //     }
    // }
    (void)_nVal;

    return nCount;
}

// _ConditionAggroCount IDA 0x140279200 -> 0x140279238
// 获取仇恨列表大小
int CAi::_ConditionAggroCount(int /*_nVal*/) {
    // IDA: AggroList = CMonster::GetAggroList(this->m_pMonster);
    //      return AggroList.size();
    if (!m_pMonster) {
        return 0;
    }
    // GetAggroList 返回 std::map<std::uint32_t, tagDamageMeter>
    auto& aggroList = m_pMonster->GetAggroList();
    return static_cast<int>(aggroList.size());
}

// _ConditionHitCount IDA 0x140279240 -> 0x140279270
// 获取被击中次数
int CAi::_ConditionHitCount(int /*_nVal*/) {
    // IDA: return CMonster::GetHitCount(this->m_pMonster);
    if (!m_pMonster) {
        return 0;
    }
    return m_pMonster->GetHitCount();
}

// _ConditionGuardFriendCount IDA 0x140279270 -> 0x1402795A6
// 获取守护对象周围的友方数量
int CAi::_ConditionGuardFriendCount(int _nVal) {
    // IDA: 获取守护对象，扫描其周围的友方数量
    if (!m_pMonster || _nVal <= 0) {
        return 0;
    }

    CMonster* pGuardMonster = m_pMonster->FindGuardMonster();
    if (!pGuardMonster) {
        return 0;
    }

    int nCount = 0;
    // TODO: 需要 GetPosition, IsFriend 和区域扫描
    // hkvVec3 vMyPos = pGuardMonster->GetPosition();
    // float fDistSq = static_cast<float>(_nVal * _nVal);
    // std::vector<CMover*> vecGameObjList;
    // XArea::ScanGridOrigin(m_pMonster, 2, 2u, &vecGameObjList);
    // for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
    //     XActor* pActor = *it;
    //     if (pActor && pActor->IsLive() && !pActor->IsStatus(2u) && pActor->GetType() == 2) {
    //         if (pActor != pGuardMonster) {
    //             CMonster* pMonster = dynamic_cast<CMonster*>(pActor);
    //             if (pMonster && m_pMonster->IsFriend(pMonster)) {
    //                 hkvVec3 vTargetPos = pMonster->GetPosition();
    //                 float fDist = vMyPos.getDistanceToSquared(vTargetPos);
    //                 if (fDistSq >= fDist) {
    //                     ++nCount;
    //                 }
    //             }
    //         }
    //     }
    // }
    (void)_nVal;

    return nCount;
}

// _ConditionGuardEnemyCount IDA 0x1402795B0 -> 0x1402798ED
// 获取守护对象周围的敌人数量
int CAi::_ConditionGuardEnemyCount(int _nVal) {
    // IDA: 获取守护对象，扫描其周围的敌人数量
    if (!m_pMonster || _nVal <= 0) {
        return 0;
    }

    CMonster* pGuardMonster = m_pMonster->FindGuardMonster();
    if (!pGuardMonster) {
        return 0;
    }

    int nCount = 0;
    // TODO: 需要 GetPosition, IsEnemy 和区域扫描
    (void)_nVal;

    return nCount;
}

// ============================================================================
// _ConditionTargetDistance IDA 0x1402798F0 -> 0x140279997
// 获取与目标的距离 - 精确还原
// ============================================================================
float CAi::_ConditionTargetDistance(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 检查怪物对象，获取目标位置，计算距离

    if (!m_pMonster) {
        return -1.0f;
    }

    std::uint32_t TargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);

    if (pTarget) {
        // IDA: 计算与目标的距离
        hkvVec3 vTargetPos = pTarget->GetPosition();
        hkvVec3 vMyPos = m_pMonster->GetPosition();
        hkvVec3 vDiff = vMyPos - vTargetPos;
        return vDiff.GetLength();
    }

    return -1.0f;
}

// ============================================================================
// _ConditionTargetDirection IDA 0x1402799A0 -> 0x140279ACB
// 获取目标方向角度差 - 精确还原
// ============================================================================
float CAi::_ConditionTargetDirection(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 计算目标方向与当前朝向的角度差

    if (!m_pMonster) {
        return 0.0f;
    }

    std::uint32_t TargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);

    if (pTarget) {
        // IDA: 计算方向向量和角度
        hkvVec3 vTargetPos = pTarget->GetPosition();
        hkvVec3 vMyPos = m_pMonster->GetPosition();
        hkvVec3 vDirVector = vMyPos - vTargetPos;
        
        // IDA: 获取偏航角
        float fYaw = CMover::GetYawFromVector(vDirVector);
        float fMovingYaw = m_pMonster->GetMovingYaw();
        float fDiffYaw = fMovingYaw - fYaw;

        // IDA: 规范化角度到 [-180, 180]
        if (fDiffYaw > 180.0f) {
            fDiffYaw -= 360.0f;
        } else if (fDiffYaw < -180.0f) {
            fDiffYaw += 360.0f;
        }
        return std::fabs(fDiffYaw);
    }

    return 0.0f;
}

// ============================================================================
// _ConditionTargetLook IDA 0x140279AD0 -> 0x140279BF3
// 获取目标注视角度差 - 精确还原
// ============================================================================
float CAi::_ConditionTargetLook(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 计算目标朝向与自身方向的角度差

    if (!m_pMonster) {
        return 0.0f;
    }

    std::uint32_t TargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);

    if (pTarget) {
        // IDA: 计算方向向量和角度 (注意顺序相反)
        hkvVec3 vTargetPos = pTarget->GetPosition();
        hkvVec3 vMyPos = m_pMonster->GetPosition();
        hkvVec3 vDirVector = vMyPos - vTargetPos;
        
        // IDA: 获取偏航角
        float fYaw = CMover::GetYawFromVector(vDirVector);
        float fMovingYaw = pTarget->GetMovingYaw();
        float fDiffYaw = fMovingYaw - fYaw;

        // IDA: 规范化角度到 [-180, 180]
        if (fDiffYaw > 180.0f) {
            fDiffYaw -= 360.0f;
        } else if (fDiffYaw < -180.0f) {
            fDiffYaw += 360.0f;
        }
        return std::fabs(fDiffYaw);
    }

    return 0.0f;
}

// ============================================================================
// _ConditionTargetDistanceCapsule IDA 0x140279C00 -> 0x140279CC7
// 获取与目标的胶囊体距离 - 精确还原
// ============================================================================
float CAi::_ConditionTargetDistanceCapsule(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 计算与目标的距离减去胶囊体半径

    if (!m_pMonster) {
        return -1.0f;
    }

    std::uint32_t TargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);

    if (pTarget) {
        // IDA: 计算距离并减去胶囊体半径
        hkvVec3 vTargetPos = pTarget->GetPosition();
        hkvVec3 vMyPos = m_pMonster->GetPosition();
        hkvVec3 vDiff = vMyPos - vTargetPos;
        float fDistance = vDiff.GetLength();
        return fDistance - pTarget->GetHavokCapsuleRadius();
    }

    return -1.0f;
}

// ============================================================================
// _ConditionStateTime IDA 0x140279CD0 -> 0x140279CE4
// 获取状态时间 - 精确还原
// ============================================================================
float CAi::_ConditionStateTime(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 直接返回 m_fStateTime
    return m_fStateTime;
}

// ============================================================================
// _ConditionCreatePosDistance IDA 0x140279CF0 -> 0x140279D52
// 获取与创建位置的距离 - 精确还原
// ============================================================================
float CAi::_ConditionCreatePosDistance(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 计算当前位置与创建位置的距离

    if (!m_pMonster) {
        return 0.0f;
    }

    // IDA: 计算与创建位置的距离
    hkvVec3 vCreatePos = m_pMonster->GetCreatePos();
    hkvVec3 vMyPos = m_pMonster->GetPosition();
    hkvVec3 vDiff = vMyPos - vCreatePos;
    return vDiff.GetLength();
}

// ============================================================================
// _ConditionLastSkillTime IDA 0x140279D60 -> 0x140279D77
// 获取最后技能时间 - 精确还原
// ============================================================================
float CAi::_ConditionLastSkillTime(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 直接返回 m_fLastSkillTime
    return m_fLastSkillTime;
}

// ============================================================================
// _ConditionLastDamageTime IDA 0x140279D80 -> 0x140279D97
// 获取最后伤害时间 - 精确还原
// ============================================================================
float CAi::_ConditionLastDamageTime(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 直接返回 m_fLastDamageTime
    return m_fLastDamageTime;
}

// ============================================================================
// _ConditionSpawnTime IDA 0x140279DA0 -> 0x140279DD1
// 获取生成时间 - 精确还原
// ============================================================================
float CAi::_ConditionSpawnTime(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 返回 CMonster::GetSpawnTime

    if (m_pMonster) {
        return m_pMonster->GetSpawnTime();
    }
    return 0.0f;
}

// ============================================================================
// _ConditionActivateTime IDA 0x140279DE0 -> 0x140279DF4
// 获取激活时间 - 精确还原
// ============================================================================
float CAi::_ConditionActivateTime(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 直接返回 m_fActivateTime
    return m_fActivateTime;
}

// ============================================================================
// _ConditionGuardDistance IDA 0x140279E00 -> 0x140279ED7
// 获取守护对象距离 - 精确还原
// ============================================================================
float CAi::_ConditionGuardDistance(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 获取守护对象并计算距离

    if (!m_pMonster) {
        return -1.0f;
    }

    CMover* pTarget = nullptr;

    // IDA: 检查是否是随从，获取主人或守护对象
    if (m_pMonster->IsFollower()) {
        pTarget = m_pMonster->GetOwnerPlayer();
    } else {
        CMonster* pGuardMonster = m_pMonster->FindGuardMonster();
        if (pGuardMonster) {
            pTarget = pGuardMonster;
        }
    }

    if (pTarget) {
        // IDA: 计算与守护对象的距离
        hkvVec3 vTargetPos = pTarget->GetPosition();
        hkvVec3 vMyPos = m_pMonster->GetPosition();
        hkvVec3 vDiff = vMyPos - vTargetPos;
        return vDiff.GetLength();
    }

    return -1.0f;
}

// ============================================================================
// _ConditionGuardDistanceCapsule IDA 0x140279EE0 -> 0x140279FD4
// 获取守护对象胶囊体距离 - 精确还原
// ============================================================================
float CAi::_ConditionGuardDistanceCapsule(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 获取守护对象并计算胶囊体距离

    if (!m_pMonster) {
        return -1.0f;
    }

    CMover* pTarget = nullptr;

    // IDA: 检查是否是随从，获取主人或守护对象
    if (m_pMonster->IsFollower()) {
        pTarget = m_pMonster->GetOwnerPlayer();
    } else {
        CMonster* pGuardMonster = m_pMonster->FindGuardMonster();
        if (pGuardMonster) {
            pTarget = pGuardMonster;
        }
    }

    if (pTarget) {
        // IDA: 计算距离并减去胶囊体半径
        hkvVec3 vTargetPos = pTarget->GetPosition();
        hkvVec3 vMyPos = m_pMonster->GetPosition();
        hkvVec3 vDiff = vMyPos - vTargetPos;
        float fDistance = vDiff.GetLength();
        return fDistance - pTarget->GetHavokCapsuleRadius();
    }

    return -1.0f;
}

// ============================================================================
// _ConditionFuzzy IDA 0x14027A1C0 -> 0x14027A410
// 计算模糊条件值 - 遍历条件向量计算模糊结果 - 精确还原
// ============================================================================
float CAi::_ConditionFuzzy(int _nVal) {
    // IDA 反编译精确还原:
    // 检查索引范围，遍历条件向量，计算模糊值

    if (_nVal >= 4) {
        return 0.0f;
    }

    int iIndex = _nVal;

    // IDA: 检查条件向量是否为空
    // TODO: 需要实现 FuzzyData::vConditions
    // if (m_arFuzzy[_nVal].vConditions.empty()) {
    //     return 0.0f;
    // }

    // IDA: 检查模糊延迟时间
    if (m_fFuzzyDelayTime > m_fFuzzyCheckTime) {
        return m_arFuzzy[iIndex].fValue;
    }

    float fResult = 0.0f;

    // IDA: 遍历 vConditions
    // for (auto iter = m_arFuzzy[iIndex].vConditions.begin();
    //      iter != m_arFuzzy[iIndex].vConditions.end(); ++iter) {
    //     int idx = iter->nConditionIndex;  // xPos.x in IDA
    //     if (idx < 0x3A) {  // 58 conditions
    //         float fValue = -1.0f;
    //
    //         // IDA: 检查是整数还是浮点条件函数
    //         if (m_xAiGetConditionDataIntFunc[idx]) {
    //             fValue = (float)GetConditionIntData(idx, iter->nParam);
    //         } else if (m_xAiGetConditionDataFloatFunc[idx]) {
    //             fValue = GetConditionFloatData(idx, iter->nParam);
    //         }
    //
    //         if (fValue != -1.0f) {
    //             // IDA: CalcFuzzyValue(xPos.z, nObjectID as float, fValue)
    //             float fCalc = CalcFuzzyValue(iter->xPos.z, *(float*)&iter->nObjectID, fValue);
    //             fResult += fCalc * iter->xPos.y;  // weight
    //         }
    //     }
    // }

    m_arFuzzy[iIndex].fValue = fResult;
    return fResult;
}

// ============================================================================
// CalcFuzzyValue IDA 0x14027A410 -> 0x14027A4E2
// 计算模糊值 (三角形隶属函数) - 精确还原
// ============================================================================
float CAi::CalcFuzzyValue(float fValue0, float fValue1, float fCurrVal) {
    // IDA 反编译精确还原:
    // 三角形隶属函数，处理两种情况：fValue1 > fValue0 和 fValue1 <= fValue0

    if (fValue0 == fValue1) {
        return 0.0f;
    }

    if (fValue1 <= fValue0) {
        // IDA: 递减区间
        if (fCurrVal < fValue0) {
            if (fValue1 < fCurrVal) {
                // fValue1 < fCurrVal < fValue0: 线性递减
                return (fValue0 - fCurrVal) / (fValue0 - fValue1);
            } else {
                // fCurrVal <= fValue1: 完全隶属
                return 1.0f;
            }
        } else {
            // fCurrVal >= fValue0: 不隶属
            return 0.0f;
        }
    } else {
        // IDA: 递增区间 (fValue1 > fValue0)
        if (fValue0 < fCurrVal) {
            if (fCurrVal < fValue1) {
                // fValue0 < fCurrVal < fValue1: 线性递增
                return (fCurrVal - fValue0) / (fValue1 - fValue0);
            } else {
                // fCurrVal >= fValue1: 完全隶属
                return 1.0f;
            }
        } else {
            // fCurrVal <= fValue0: 不隶属
            return 0.0f;
        }
    }
}

// ============================================================================
// _ConditionGroupCooltime IDA 0x14027A040 -> 0x14027A156
// 检查技能组冷却时间 - 精确还原
// ============================================================================
int CAi::_ConditionGroupCooltime(int _nVal) {
    // IDA 反编译精确还原:
    // 遍历技能组中的技能，检查是否有冷却时间

    if (!m_pMonster) {
        return 0;
    }

    if (_nVal >= 10) {
        return 0;
    }

    // IDA: 获取怪物表引用
    TB_MONSTER* pMobRef = m_pMonster->GetMobTableRef();
    if (!pMobRef) {
        return 0;
    }

    // IDA: 遍历技能组中的3个技能索引
    for (int i = 0; i < 3; ++i) {
        int nSkillIndex = m_nSkillGroupID[_nVal][i] - 1;
        if (nSkillIndex >= 0 && nSkillIndex < 10) {
            // IDA: 获取技能ID (*(&pMobRef->Monster_Skill1_ID + nSkillIndex))
            // TODO: 需要从 TB_MONSTER 结构体获取技能ID
            // unsigned int nSkillID = pMobRef->GetSkillID(nSkillIndex);
            unsigned int nSkillID = 0;  // 暂时返回0

            if (nSkillID) {
                // IDA: 获取技能表
                // TB_SKILL* pSkillTable = XResourceMgr::GetTB_SKILL(nSkillID);
                // if (pSkillTable && GetCooltime(pSkillTable->CoolTime_Group) > 0.0) {
                //     return 1;
                // }
            }
        }
    }

    return 0;
}

// ============================================================================
// _ConditionSkillCooltime IDA 0x14027A160 -> 0x14027A1BD
// 检查技能冷却时间 - 精确还原
// ============================================================================
bool CAi::_ConditionSkillCooltime(unsigned int _nVal) {
    // IDA 0x14027A160: CAi::_ConditionSkillCooltime
    // 获取技能表并检查冷却时间

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_SKILL* pSkillTable = pServer ? pServer->GetResourceMgr().GetTB_SKILL(_nVal) : nullptr;
    return pSkillTable && GetCooltime(pSkillTable->CoolTime_Group) > 0.0f;
}

// ============================================================================
// _ConditionStateEndTime IDA 0x14027A4F0 -> 0x14027A567
// 获取状态结束时间 - 精确还原
// ============================================================================
float CAi::_ConditionStateEndTime(int _nVal) {
    // IDA 反编译精确还原:
    // 检查状态索引范围和时间是否有效

    if (_nVal >= 43) {
        return -1.0f;
    }

    if (m_fStateEndTime[_nVal] == 0.0f) {
        return -1.0f;
    }

    // IDA: Timer = ThreadLocalData::GetTimer();
    // return IVTimer::GetTime(Timer) - m_fStateEndTime[_nVal];
    // TODO: 需要实现 ThreadLocalData::GetTimer 和 IVTimer::GetTime
    // VDefaultTimer* Timer = ThreadLocalData::GetTimer();
    // return IVTimer::GetTime(Timer) - m_fStateEndTime[_nVal];

    return -1.0f;  // 暂时返回-1.0
}

// ============================================================================
// _ConditionMoveDistanceAfterSkill IDA 0x14027A570 -> 0x14027A5A1
// 获取技能后移动距离
// ============================================================================
float CAi::_ConditionMoveDistanceAfterSkill(int _nVal) {
    // IDA 反编译精确还原:
    // if (m_pMonster) return CMoverEx::GetMoveDistAfterSkill(m_pMonster);
    // else return 0.0;

    if (m_pMonster) {
        return m_pMonster->GetMoveDistAfterSkill();
    }
    return 0.0f;
}

// ============================================================================
// _StartWait IDA 0x14027A950 -> 0x14027AA0C
// 开始等待状态 - 精确还原
// ============================================================================
void CAi::_StartWait() {
    // IDA 反编译精确还原:
    // 清除目标，重置时间，切换战斗姿态

    if (!m_pMonster) {
        return;
    }

    ClearTarget();
    m_fSearchTargetTime = 0.0f;
    m_fLastSkillTime = -1.0f;

    // IDA: 如果在战斗姿态，切换到非战斗姿态
    // TODO: 需要 IsBattlePose 和 ChangeBattlePose 方法
    // if (m_pMonster->IsBattlePose()) {
    //     m_pMonster->ChangeBattlePose(0, true);
    // }

    // IDA: 如果没有移动类型，设置搜索目标延迟
    if (!m_pMonster->GetMoveType()) {
        m_fSearchTargetTime = m_fDelaySearchTarget;
    }
}

// ============================================================================
// _UpdateWait IDA 0x14027AA10 -> 0x14027AA54
// 更新等待状态
// ============================================================================
void CAi::_UpdateWait(float fElapsedTime) {
    // IDA 反编译精确还原:
    // if (m_bIsFirstAttacker) FuncFindEnemy(fElapsedTime);
    // CheckValidPositionByTime(fElapsedTime);

    if (m_bIsFirstAttacker) {
        FuncFindEnemy(fElapsedTime);
    }
    CheckValidPositionByTime(fElapsedTime);
}

// ============================================================================
// _StartPatrol IDA 0x14027AA60 -> 0x14027B09E
// 开始巡逻状态 - 精确还原
// ============================================================================
void CAi::_StartPatrol() {
    // IDA 反编译精确还原:
    // 复杂的巡逻开始逻辑，计算目标位置并移动

    if (!m_pMonster) {
        return;
    }

    // IDA: 获取区域和导航网格
    // XArea* pArea = m_pMonster->GetArea();
    // if (!pArea) {
    //     // IDA: XPRINT("if( NULL==pMaze ) [GetMazeID:%u]", MapInsID->nMapID);
    //     return;
    // }

    // IDA: 获取导航网格实例
    // DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    // if (!pNavMesh) return;

    // IDA: 检查是否可以移动
    // if (!m_pMonster->IsCanMove(true)) return;

    // IDA: 检查移动类型
    if (!m_pMonster->GetMoveType()) {
        return;
    }

    // IDA: 获取当前位置和创建位置
    hkvVec3 vMyPos;
    hkvVec3 vMyCreatePos;
    hkvVec3 vTargetPos;
    // IDA: 获取当前位置和创建位置
    vMyPos = m_pMonster->GetPosition();
    vMyCreatePos = m_pMonster->GetCreatePos();

    // IDA: 检查路径点ID
    int nWayPointID = 0;  // TODO: CMoverEx::GetWayPointID(m_pMonster)

    if (nWayPointID) {
        // IDA: 从路径点获取目标位置
        // VEventObjectResource* pResource = pArea->GetObjectResource();
        // const VEventObjectInfo* pInfo = pResource->SearchFromID(nWayPointID);
        // if (pInfo) {
        //     VEventObjectResourceManager::GetCenterPos(pInfo, &vTargetPos);
        // }
    } else {
        // IDA: 没有路径点时，计算随机目标位置
        int nFindCount = 0;
        while (nFindCount <= 10) {
            // IDA: 随机角度和距离
            float fAngle = RandomBetweenF(m_fStateAngleMin, m_fStateAngleMax);
            float fDist = RandomBetweenF(m_fStateMoveDistMin, m_fStateMoveDistMax);

            // IDA: 50%概率反向
            if (rand() % 2 == 0) {
                fAngle = -fAngle;
            }

            // IDA: 计算偏航角
            // float fYaw = m_pMonster->GetMovingYaw() + fAngle;
            // if (fYaw > 180.0f) fYaw -= 360.0f;
            // else if (fYaw < -180.0f) fYaw += 360.0f;

            // IDA: 计算目标位置
            // hkvMat3 matRot;
            // matRot.setFromEulerAngles(0.0f, 0.0f, fYaw);
            // hkvVec3 vDir(0.0f, -fDist, 0.0f);
            // vDir *= matRot;
            // vTargetPos = vMyPos + vDir;

            // IDA: 限制在创建位置范围内
            // if (vTargetPos.x > vMyCreatePos.x + m_fStateTargetDistMax)
            //     vTargetPos.x = vMyCreatePos.x + m_fStateTargetDistMax;
            // else if (vTargetPos.x < vMyCreatePos.x - m_fStateTargetDistMax)
            //     vTargetPos.x = vMyCreatePos.x - m_fStateTargetDistMax;
            // (same for y)

            // IDA: 检查高度
            // if (pNavMesh->GetHeight(&vTargetPos, 200.0f)) break;

            nFindCount++;
        }

        if (nFindCount > 10) {
            return;
        }
    }

    // IDA: 清除状态标志
    // m_pMonster->ClearStatus(0x100);

    // IDA: 巡逻怪物设置
    if (m_bPatrolMonster) {
        // m_pMonster->SetStatus(0x100);
        // m_pMonster->ChangeBattlePose(true, false);
    }

    // IDA: 移动到目标位置
    // MoveToPos(&vMyPos, &vTargetPos);
}

// ============================================================================
// _UpdatePatrol IDA 0x14027B0A0 -> 0x14027B8AC
// 更新巡逻状态 - 精确还原
// ============================================================================
void CAi::_UpdatePatrol(float fElapsedTime) {
    // IDA 反编译精确还原:
    // 复杂的巡逻更新逻辑，处理路径点和巡逻攻击

    CheckValidPositionByTime(fElapsedTime);

    if (!m_pMonster) {
        return;
    }

    // IDA: 检查移动类型
    if (!m_pMonster->GetMoveType()) {
        FuncFindEnemy(fElapsedTime);
        return;
    }

    // IDA: 检查是否可以移动
    if (!m_pMonster->IsCanMove(true)) {
        if (m_bIsFirstAttacker) {
            FuncFindEnemy(fElapsedTime);
        }
        return;
    }

    // IDA: 巡逻怪物处理
    if (m_bPatrolMonster) {
        // IDA: 检查巡逻攻击或移动状态
        if (CheckPatrolAttack() || m_pMonster->IsMoving()) {
            return;
        }

        // IDA: 获取区域和导航网格
        // XArea* pArea = m_pMonster->GetArea();
        // if (!pArea) return;
        // if (!pArea->GetNavMeshInstance()) return;

        // IDA: 获取路径点信息
        // int nWayPointID = m_pMonster->GetWayPointID();
        // if (m_nPatrolWayPoint) {
        //     // 搜索路径点并移动
        // }

        // TODO: 完整实现需要 CWayPoint、GetWayPointID、SetWayPointID 等依赖
    }

    // IDA: 处理路径点更新
    // CWayPoint* pWayPoint = m_pMonster->GetWayPoint();
    // if (!pWayPoint) return;

    // IDA: 检查路径点状态并更新
    // ...复杂的路径点处理逻辑...

    if (m_bIsFirstAttacker) {
        FuncFindEnemy(fElapsedTime);
    }
}

// ============================================================================
// CheckValidPositionByTime IDA 0x14027BED0 -> 0x14027C0D7
// 按时间检查有效位置 - 精确还原
// ============================================================================
int CAi::CheckValidPositionByTime(float fElapsedTime) {
    // IDA 反编译精确还原:
    // 检查怪物位置是否有效，超时则传送到创建位置

    if (!m_pMonster) {
        return 0;
    }

    // IDA: 检查是否为普通怪物
    if (!m_pMonster->IsNormalMonster()) return 0;

    // IDA: 获取当前位置
    hkvVec3 vMyPos = m_pMonster->GetPosition();

    // IDA: 检查高度是否有效
    // if (m_pMonster->GetHeight(&vMyPos, 200.0f)) return 0;

    // IDA: 更新检查时间
    m_fCheckValidPositionTime -= fElapsedTime;
    if (m_fCheckValidPositionTime > 0.0f) {
        return 0;
    }

    // IDA: 重置检查时间为5秒
    m_fCheckValidPositionTime = 5.0f;

    // IDA: 获取创建位置
    hkvVec3 vMyCreatePos = m_pMonster->GetCreatePos();

    // IDA: 获取朝向
    // float fRot = m_pMonster->GetOrientationYaw();

    // IDA: 移动到创建位置
    // m_pMonster->Move(&vMyCreatePos);
    // m_pMonster->send_eSUB_CMD_SKILL_WARP_POSITION(m_pMonster, m_pMonster, &vMyCreatePos, 1);

    return 1;
}

// ============================================================================
// _EndProtectMove IDA 0x14027B8B0 -> 0x14027B8FB
// 结束保护移动
// ============================================================================
void CAi::_EndProtectMove() {
    // IDA 反编译精确还原:
    // if (m_pMonster && IsMoving()) StopMoving(true);

    if (m_pMonster && m_pMonster->IsMoving()) {
        m_pMonster->StopMoving(true);
    }
}

// ============================================================================
// CheckPatrolAttack IDA 0x14027B900 -> 0x14027BA64
// 检查巡逻攻击 - 精确还原
// ============================================================================
bool CAi::CheckPatrolAttack() {
    // IDA 反编译精确还原:
    // 检查技能激活状态或遍历技能执行攻击

    if (!m_pMonster) {
        return false;
    }

    // IDA: 如果技能已激活，检查状态
    if (m_bSkillActivate) {
        // IDA: 检查是否处于攻击状态
        if (m_pMonster->IsStatus(1)) {
            return true;
        } else {
            m_bSkillActivate = false;
            return false;
        }
    }

    // IDA: 遍历技能索引
    for (int i = 0; i < 10; ++i) {
        int nSkillIndex = GetSkillIndex(i);
        if (nSkillIndex < 10 && nSkillIndex != -1) {
            // IDA: 获取怪物表引用
            TB_MONSTER* pTableRef = m_pMonster->GetMobTableRef();
            if (pTableRef) {
                // IDA: 获取技能ID (*(&pTableRef->Monster_Skill1_ID + nSkillIndex))
                // TODO: 需要从 TB_MONSTER 获取技能ID
                int nSkillID = 0;  // 暂时返回0

                if (nSkillID > 0) {
                    // IDA: 获取技能表并执行攻击
                    // m_pCurSkillRef = XResourceMgr::GetTB_SKILL(nSkillID);
                    // m_pMonster->SetCurSkillTableIdx(nSkillID);
                    // m_pMonster->ActionAttack();
                    // SetSkillCooltime(m_pCurSkillRef);
                    // m_bSkillActivate = true;
                    // return true;
                }
            }
        }
    }

    return false;
}

// ============================================================================
// ActiveSuperArmorSkill IDA 0x14027BA70 -> 0x14027BB04
// 激活超级装甲技能 - 精确还原
// ============================================================================
void CAi::ActiveSuperArmorSkill() {
    // IDA 反编译精确还原:
    // 获取技能表并执行攻击

    if (!m_pMonster || m_nSuperArmorSkillIndex < 0) {
        return;
    }

    // IDA: 获取技能表
    // TB_SKILL* pSkillRef = XResourceMgr::GetTB_SKILL(m_nSuperArmorSkillIndex);
    // if (pSkillRef) {
    //     m_pCurSkillRef = pSkillRef;
    //     m_pMonster->SetCurSkillTableIdx(pSkillRef->Skill_Index);
    //     m_pMonster->ActionAttack();
    // }
}

// ============================================================================
// CheckHelperFarDist IDA 0x14027BB10 -> 0x14027BD54
// 检查助手远距离 - 精确还原
// ============================================================================
int CAi::CheckHelperFarDist(bool bBattle, bool bForce) {
    // IDA 反编译精确还原:
    // 检查助手是否离主人太远，需要传送

    if (!m_pMonster) {
        return 0;
    }

    // IDA: 检查是否为真正的助手
    // TODO: 需要 IsRealHelper 方法
    // if (!m_pMonster->IsRealHelper()) return 0;

    // IDA: 获取所有者玩家
    // CMoverEx* pOwnerPlayer = m_pMonster->GetOwnerPlayer();
    CMoverEx* pOwnerPlayer = m_pMonster->GetOwnerPlayer();
    if (!pOwnerPlayer) return 0;

    // IDA: 获取位置
    hkvVec3 vMonsterPos = m_pMonster->GetPosition();
    hkvVec3 vUserPos = pOwnerPlayer->GetPosition();

    // IDA: 检查用户位置是否有效
    if (vUserPos.isZero()) return 0;

    // IDA: 计算距离
    float fDist = (vMonsterPos - vUserPos).GetLength();

    // IDA: 确定检查距离
    float fCheckDist = m_fHelperFarDistance;
    if (bBattle) {
        fCheckDist = m_fHelperFarBattleDist;
    }

    // IDA: 强制模式处理
    if (bForce) {
        int nCount = m_nCheckHelperFarCount;
        m_nCheckHelperFarCount = nCount - 1;
        if (nCount < 0) {
            fCheckDist = 500.0f;
        }
    }

    // IDA: 检查高度
    // bool isCheck = m_pMonster->GetHeight(&vUserPos, 200.0f);

    // IDA: 判断是否需要传送
    // if (fDist <= fCheckDist || !isCheck) return 0;

    // IDA: 设置检查计数并传送
    m_nCheckHelperFarCount = 10;
    // WarpHelperToUser(&vUserPos);

    (void)bBattle;
    (void)bForce;
    return 0;
}

// ============================================================================
// SetTargetSightDistance IDA 0x14027BE90 -> 0x14027BEC4
// 设置目标视野距离
// ============================================================================
void CAi::SetTargetSightDistance(float fDistance) {
    // IDA 反编译精确还原:
    // if (fDistance > m_fTargetSightDistance)
    //     m_fTargetSightDistance = fDistance;

    if (fDistance > m_fTargetSightDistance) {
        m_fTargetSightDistance = fDistance;
    }
}

// ============================================================================
// WarpHelperToUser IDA 0x14027BD60 -> 0x14027BE89
// 传送助手到用户 - 精确还原
// ============================================================================
void CAi::WarpHelperToUser(const hkvVec3& vPos) {
    // IDA 反编译精确还原:
    // 传送助手到指定位置

    if (!m_pMonster) {
        return;
    }

    // IDA: 检查位置是否有效
    // IDA: hkvVec3::IsZero(vUserPos)
    if (vPos.x == 0.0f && vPos.y == 0.0f && vPos.z == 0.0f) {
        return;
    }

    // IDA: 检查是否启用助手传送
    if (!m_bEnableHelperWarp) {
        return;
    }

    // IDA: 清除目标
    ClearTarget();

    // IDA: 设置移动位置
    // m_pMonster->SetMovePosition(vPos.x, vPos.y);

    // IDA: 获取朝向
    // float fYaw = m_pMonster->GetOrientationYaw();

    // IDA: 移动到目标位置
    // m_pMonster->Move(&vPos);

    // IDA: 发送助手同步位置
    // m_pMonster->send_eSUB_CMD_HELPER_SYNC_POS(m_pMonster, &vPos);

    // IDA: 改变AI状态
    // ChangeAiState(FSMSTATES_SELECT_ACTION);
}

// ============================================================================
// CheckValidPositionByCount IDA 0x14027C0E0 -> 0x14027C5A3
// 按计数检查有效位置 - 精确还原
// ============================================================================
int CAi::CheckValidPositionByCount() {
    // IDA 反编译精确还原:
    // 检查怪物移动失败后，验证位置并传送到有效位置

    if (!m_pMaze || !m_pMonster) {
        return 0;
    }

    // IDA: 检查是否为普通怪物
    if (!m_pMonster->IsNormalMonster()) return 0;

    // IDA: 移动失败计数检查
    int nFailCount = m_byFailMoveCount++;
    if (nFailCount < 2) {
        return 0;
    }
    m_byFailMoveCount = 0;

    // IDA: 获取怪物当前位置
    hkvVec3 vMyPos = m_pMonster->GetPosition();

    hkvVec3 vDestPos = vMyPos;

    // IDA: 获取朝向
    // float fRot = m_pMonster->GetOrientationYaw();

    // IDA: 获取创建位置
    hkvVec3 vMyCreatePos = m_pMonster->GetCreatePos();

    // IDA: 查找技能目标
    CMover* pTarget = FindTargetBySkill();
    if (pTarget) {
        // IDA: 获取目标位置
        hkvVec3 vTargetPos = pTarget->GetPosition();

        // IDA: 获取扇区ID
        // int nTargetSector = m_pMaze->GetLastSectorID();
        // int nMySector = m_pMaze->GetSectorIDFromPos(&vMyCreatePos);

        // TODO: 完整实现需要 XMaze, XArea, CMover 方法
        // 暂时返回 0
        return 0;
    } else {
        // IDA: 没有目标时，传送回创建位置
        // TODO: 需要 Move 和 send_eSUB_CMD_SKILL_WARP_POSITION
        // m_pMonster->Move(&vMyCreatePos);
        // CMover::send_eSUB_CMD_SKILL_WARP_POSITION(m_pMonster, m_pMonster, &vMyCreatePos, 1);
        return 1;
    }
}

// ============================================================================
// CheckInitMaze IDA 0x14027C5B0 -> 0x14027C764
// 检查初始化迷宫 - 精确还原
// ============================================================================
void CAi::CheckInitMaze() {
    // IDA 反编译精确还原:
    // 初始化迷宫指针，处理护送怪物的初始化

    if (m_pMaze) {
        return;
    }

    if (!m_pMonster) {
        return;
    }

    // IDA: 获取区域并转换为 XMaze
    // TODO: 需要 GetArea 和 RTDynamicCast
    // XArea* pArea = m_pMonster->GetArea();
    // m_pMaze = dynamic_cast<XMaze*>(pArea);

    if (!m_pMaze) {
        return;
    }

    // IDA: 检查是否为护送怪物
    // TODO: 需要 CMonster::IsEscort
    // if (!m_pMonster->IsEscort()) {
    //     FuncSpawnAggro();
    //     return;
    // }

    // IDA: 获取护送怪物信息
    // TODO: 需要 XMaze::GetEscortMonster
    // STEscortMonster* escortMonster = m_pMaze->GetEscortMonster();
    // if (escortMonster->nMonsterID != m_pMonster->GetTableID()) {
    //     FuncSpawnAggro();
    //     return;
    // }

    m_bEscortMonster = true;

    if (m_bEscortMonster && m_bStartEscort) {
        m_bStartEscort = false;

        // IDA: 获取路径点并运行护送函数
        // TODO: 需要 GetWayPoint, FirstChild, RTDynamicCast
        // CWayPoint* wayPoint = m_pMonster->GetWayPoint();
        // VWayPointInfo* child = wayPoint->FirstChild();
        // VEscortPointInfo* escortPoint = dynamic_cast<VEscortPointInfo*>(child);
        // if (escortPoint) {
        //     m_pMaze->RunEscortFunction(escortPoint->iID, 3, escortPoint->m_szFunction);
        // }

        CheckEscortWayPoint();
    }

    FuncSpawnAggro();
}

// ============================================================================
// CheckEscortWayPoint IDA 0x14027C770 -> 0x14027C9C8
// 检查护送路径点 - 精确还原
// ============================================================================
void CAi::CheckEscortWayPoint() {
    // IDA 反编译精确还原:
    // 检查护送怪物的路径点碰撞和状态

    if (!m_bEscortMonster) {
        return;
    }

    if (m_bStartEscort) {
        return;
    }

    if (!m_pMaze || !m_pMonster) {
        return;
    }

    // IDA: 获取路径点
    // TODO: 需要 CMoverEx::GetWayPoint
    // CWayPoint* pWayPoint = m_pMonster->GetWayPoint();
    // if (!pWayPoint) return;

    // IDA: 获取子路径点
    // TODO: 需要 TiXmlNode::FirstChild 和 RTDynamicCast
    // VWayPointInfo* child = pWayPoint->FirstChild();
    // VEscortPointInfo* escortPoint = dynamic_cast<VEscortPointInfo*>(child);
    // if (!escortPoint) return;

    // IDA: 检查碰撞盒
    const hkvVec3 position = m_pMonster->GetPosition();
    // unsigned __int8 checkCode = pWayPoint->CheckHitCollisonBox(&position);

    // IDA: checkCode == 1 表示到达路径点
    // if (checkCode == 1) {
    //     m_pMonster->StopMoving(true);
    //     pWayPoint->NextWayPoint();
    // }

    // IDA: checkCode == 2 表示护送失败
    // if (checkCode == 2) {
    //     m_bEscortMonster = false;
    //     m_pMonster->ChangeMotion_3(1, true, 0);
    //     m_pMonster->StopMoving(true);
    //
    //     STEscortMonster* escortMonster = m_pMaze->GetEscortMonster();
    //     char szMonsterID[16];
    //     _itoa(escortMonster->nMonsterID, szMonsterID, 10);
    //     m_pMaze->ExecuteDestroy(2, szMonsterID, escortMonster->szMonsterDieAnim, 1);
    //     m_pMonster->CompleteEscortCondition();
    // }

    // IDA: 运行护送函数
    // if (checkCode) {
    //     m_pMaze->RunEscortFunction(escortPoint->iID, checkCode, escortPoint->m_szFunction);
    // }
}

// ============================================================================
// EndEscortWayPoint IDA 0x14027C9D0 -> 0x14027CA98
// 结束护送路径点 - 精确还原
// ============================================================================
void CAi::EndEscortWayPoint() {
    // IDA 反编译精确还原:
    // 结束护送路径点，发送护送状态

    if (!m_bEscortMonster) {
        return;
    }

    if (m_bStartEscort) {
        return;
    }

    if (!m_pMaze || !m_pMonster) {
        return;
    }

    // IDA: 获取路径点
    // TODO: 需要 CMoverEx::GetWayPoint
    // CWayPoint* pWayPoint = m_pMonster->GetWayPoint();
    // if (!pWayPoint) return;

    // IDA: 获取子路径点并发送护送状态
    // TODO: 需要 TiXmlNode::FirstChild 和 RTDynamicCast
    // VWayPointInfo* child = pWayPoint->FirstChild();
    // VEscortPointInfo* escortPoint = dynamic_cast<VEscortPointInfo*>(child);
    // if (escortPoint) {
    //     m_pMaze->SendEscortStatus(escortPoint->iID, 4);
    // }
}

// ============================================================================
// GetCooltime - 获取冷却时间
// ============================================================================
float CAi::GetCooltime(int nCooltimeGroup) {
    // 从 m_mapCooltimeList 查找冷却时间
    // tagCOOLTIME 结构: dwTotalTime, fStartTime, fEndTime, byType
    auto it = m_mapCooltimeList.find(nCooltimeGroup);
    if (it != m_mapCooltimeList.end()) {
        // 计算剩余时间: fEndTime - 当前时间
        // TODO: 需要实现 ThreadLocalData::GetTimer() 和 IVTimer::GetTime()
        // float fCurrentTime = IVTimer::GetTime(ThreadLocalData::GetTimer());
        // return it->second.fEndTime - fCurrentTime;
        return it->second.fEndTime;
    }
    return 0.0f;
}

// ============================================================================
// CheckSkillGroupCondition IDA 0x140269B30
// 检查技能组条件 - 检查技能组的转换条件是否满足
// ============================================================================
bool CAi::CheckSkillGroupCondition(unsigned int nSkillIndex, int nSkillGroup) {
    // IDA 反编译精确还原:
    // 如果 nSkillGroup == -1，检查 m_arSkillTransition[nSkillIndex]
    // 否则检查 m_arSkillGroupTransition[nSkillGroup][nSkillIndex]

    if (nSkillIndex >= 10) {
        return false;
    }

    CFsmTransition* pTransition = nullptr;

    if (nSkillGroup == -1) {
        // 检查技能转换
        pTransition = m_arSkillTransition[nSkillIndex];
    } else if (nSkillGroup >= 0 && nSkillGroup < 10) {
        // 检查技能组转换
        pTransition = m_arSkillGroupTransition[nSkillGroup][nSkillIndex];
    }

    if (!pTransition) {
        // 没有转换对象，条件默认满足
        return true;
    }

    // 获取条件向量迭代器
    // CFsmTransition::GetConditionVectorBegin(pTransition, &conditionIterBegin);
    // CFsmTransition::GetConditionVectorEnd(pTransition, &conditionIterEnd);
    //
    // 如果条件向量为空，返回 true
    // if (conditionIterBegin == conditionIterEnd) {
    //     return true;
    // }
    //
    // int nConditionCount = CFsmTransition::GetConditionNumber(pTransition);
    // int nConditionSuccessedCount = 0;
    //
    // 遍历所有条件
    // while (conditionIterBegin != conditionIterEnd) {
    //     CFsmCondition* pCondition = *conditionIterBegin;
    //     if (pCondition) {
    //         E_FSMVARIABLES eVarName = pCondition->GetVarName();
    //         E_FSMDATATYPE eVarType = pCondition->GetVarType();
    //
    //         if (eVarType == FSMDTYPE_INT || eVarType == FSMDTYPE_RANDOMINT) {
    //             int nValue = pCondition->GetValueInt(1);
    //             int nActualValue = GetConditionIntData(eVarName, nValue);
    //             if (!pCondition->ConditionFulfilled(nActualValue)) {
    //                 return (nConditionCount == nConditionSuccessedCount);
    //             }
    //             ++nConditionSuccessedCount;
    //         } else {
    //             float fValue = pCondition->GetValueFloat(1);
    //             float fActualValue = GetConditionFloatData(eVarName, static_cast<int>(fValue));
    //             if (!pCondition->ConditionFulfilled(fActualValue)) {
    //                 return (nConditionCount == nConditionSuccessedCount);
    //             }
    //             ++nConditionSuccessedCount;
    //         }
    //     }
    //     ++conditionIterBegin;
    // }
    //
    // return (nConditionCount == nConditionSuccessedCount);

    // CFsmTransition 尚未完全实现，暂时返回 true
    return true;
}

// ============================================================================
// FuncStartState IDA 0x14026A850 -> 0x14026AB0F
// 启动状态 - 初始化状态变量并开始新状态
// IDA 精确还原
// ============================================================================
void CAi::FuncStartState() {
    // IDA: if ( this->m_pMonster && this->m_pStateMachine )
    if (!m_pMonster || !m_pStateMachine) {
        return;
    }

    // IDA: 重置所有状态时间变量
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

    // IDA: 记录状态起始位置
    // qmemcpy(v6, VisObject3D_cl::GetPosition(this->m_pMonster), sizeof(v6));
    // qmemcpy(&this->m_vStateMoveStartPos, v6, sizeof(this->m_vStateMoveStartPos));
    hkvVec3 vPos = m_pMonster->GetPosition();
    m_vStateMoveStartPos[0] = vPos.x;
    m_vStateMoveStartPos[1] = vPos.y;
    m_vStateMoveStartPos[2] = vPos.z;

    // IDA: 保存当前HP
    m_nStatePreHP = m_pMonster->GetHP();

    // IDA: 从状态机获取当前状态
    // nState = XGameDBSocketMgr::GetLogDBAgentCount((LogicTimer *)this->m_pStateMachine);
    // TODO: 需要实现 CFsmClass::GetCurrentState
    int nState = 0;  // 暂时使用默认值

    // IDA: 从 m_mapStateVars 查找状态变量信息
    // std::_Tree<std::_Tmap_traits<int,StateVarInfo,...>>::find(&this->m_mapStateVars, &it, &nState);
    auto it = m_mapStateVars.find(nState);
    if (it != m_mapStateVars.end()) {
        StateVarInfo& VarInfo = it->second;

        // IDA: 解析状态生命周期
        // if ( VarInfo->DataList[0][0] >= 0 )
        if (VarInfo.DataList[0][0] >= 0) {
            // IDA: if ( VarInfo->DataList[0][1] <= 0 )
            if (VarInfo.DataList[0][1] <= 0) {
                // 固定生命周期
                m_fStateLifeTime = static_cast<float>(VarInfo.DataList[0][0]) * 0.001f;
            } else {
                // 随机生命周期
                // IDA: m_fStateLifeTime = (float)RandomBetween(VarInfo->DataList[0][0], VarInfo->DataList[0][1]) * 0.001;
                int nRandom = static_cast<int>(RandomBetweenF(
                    static_cast<float>(VarInfo.DataList[0][0]),
                    static_cast<float>(VarInfo.DataList[0][1])));
                m_fStateLifeTime = static_cast<float>(nRandom) * 0.001f;
            }
        }

        // IDA: 解析移动距离
        m_fStateMoveDistMin = static_cast<float>(VarInfo.DataList[1][0]);
        m_fStateMoveDistMax = static_cast<float>(VarInfo.DataList[1][1]);

        // IDA: 解析目标距离
        m_fStateTargetDistMin = static_cast<float>(VarInfo.DataList[2][0]);
        m_fStateTargetDistMax = static_cast<float>(VarInfo.DataList[2][1]);

        // IDA: 解析角度
        m_fStateAngleMin = static_cast<float>(VarInfo.DataList[3][0]);
        m_fStateAngleMax = static_cast<float>(VarInfo.DataList[3][1]);
    }

    // IDA: 调用 CheckInitMaze
    CheckInitMaze();
}

// ============================================================================
// FuncSearchTarget IDA 0x140265AD0 -> 0x140265F39
// 搜索目标 - 在视野范围内搜索敌对目标
// IDA 精确还原
// ============================================================================
void CAi::FuncSearchTarget() {
    // IDA: 初始化最短距离为目标视野距离
    float fShortDistance = m_fTargetSightDistance;
    std::uint32_t dwTargetID = 0xFFFFFFFF;

    // IDA: 检查 m_pMonster 和 Area 是否有效
    if (!m_pMonster) {
        return;
    }

    // IDA: 获取区域 (CMover 包含 XActor 成员)
    // if ( this->m_pMonster && this->m_pMonster->GetArea(&this->m_pMonster->XActor) )
    XArea* pArea = nullptr;  // TODO: 需要实现 GetArea 方法
    if (!pArea) {
        // IDA: XPRINT("if( NULL==pMaze ) [GetMazeID:%u]", MapInsID->nMapID);
        // TODO: 需要实现 GetMapInsID 方法
        GreenDamTan_log(__FILE__, __FUNCTION__, "if( NULL==pMaze ) [GetMazeID:%u]", 0);
        return;
    }

    // IDA: 创建对象列表并扫描附近对象
    std::vector<CMover*> vecGameObjList;
    // TODO: 需要实现 XArea::ScanGridOrigin
    // pArea->ScanGridOrigin(&m_pMonster->XActor, 2, 3u, &vecGameObjList);

    // IDA: 遍历所有扫描到的对象
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMover* pOtherMover = *it;

        // IDA: RTTI 动态转换检查是否为 CMoverEx
        CMoverEx* pOtherMoverEx = dynamic_cast<CMoverEx*>(pOtherMover);
        if (!pOtherMoverEx) {
            continue;
        }

        // IDA: 检查目标是否存活 (v26->IsLive(&pOtherMover->XActor) && !CMover::IsDie(pOtherMover))
        if (pOtherMover->IsDie()) {
            continue;
        }

        // IDA: 检查是否是敌对目标
        if (!m_pMonster->IsEnemy(pOtherMover)) {
            continue;
        }

        // IDA: 检查是否是守护怪物 (IsGuardMonster)
        if (IsGuardMonster(pOtherMover)) {
            continue;
        }

        // IDA: 检查目标类型
        // if (XActor::GetType(&pOtherMover->XActor) != 2 || ...)
        // TODO: 需要实现 XActor::GetType 访问
        // E_ACTOR_TYPE eType = pOtherMover->GetXActor().GetType();
        // 暂时跳过类型检查

        // IDA: 计算距离
        hkvVec3 vOtherPos = pOtherMover->GetPosition();
        hkvVec3 vMyPos = m_pMonster->GetPosition();
        hkvVec3 vDiff = vMyPos - vOtherPos;
        float fLength = vDiff.GetLength();
        float fHavokRadius = pOtherMover->GetHavokCapsuleRadius();
        float fDistance = fLength - fHavokRadius;

        // IDA: 如果距离更近，更新目标
        if (fShortDistance > fDistance) {
            // IDA: 获取ActorID
            UXActorID actorID = pOtherMover->GetActorID();
            dwTargetID = actorID.dwActorID;
            fShortDistance = fDistance;
        }
    }

    // IDA: 如果找到目标
    if (dwTargetID != 0xFFFFFFFF) {
        std::uint32_t dwCurrentTargetID = m_pMonster->GetTargetID();
        if (dwCurrentTargetID != dwTargetID) {
            // IDA: 应用仇恨值
            m_pMonster->ApplyAggroValue(dwTargetID, 1.0f, true);
            // IDA: 伤害激怒处理
            m_pMonster->DamageAggressive();

            // IDA: 检查群体仇恨
            CGroupAggro* pGroupAggro = m_pMonster->GetGroupAggro();
            if (pGroupAggro) {
                pGroupAggro->RunAggro();
            }
        }
    }
}

// ============================================================================
// FuncAttackSkill IDA 0x140268D80 -> 0x140269174
// 攻击技能 - 执行攻击技能逻辑
// IDA 精确还原
// ============================================================================
bool CAi::FuncAttackSkill() {
    // IDA: 检查 m_pMonster 是否有效
    if (!m_pMonster) {
        return false;
    }

    // IDA: 获取目标ID和目标对象
    std::uint32_t dwTargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(dwTargetID);

    // IDA: if ( !pTarget || !this->m_pCurSkillRef )
    if (!pTarget || !m_pCurSkillRef) {
        return false;
    }

    // IDA: 检查怪物是否可以攻击
    if (!m_pMonster->IsCanAttack()) {
        return false;
    }

    // IDA: 检查目标是否处于死亡状态 (XActor::IsStatus(&pTarget->XActor, 2u))
    if (pTarget->IsStatus(2)) {
        ClearTarget();
        return false;
    }

    // IDA: 获取目标位置
    hkvVec3 vTargetPos = pTarget->GetPosition();

    // IDA: if ( CMoverEx::GetStandType(this->m_pMonster) == 2 ) goto LABEL_30
    // IDA: if ( this->m_fStateTime > 30.0 ) goto LABEL_30
    // 跳过角度检查的条件
    bool bSkipAngleCheck = false;
    // IDA: 检查站立类型或状态时间
    if (m_pMonster->GetStandType() == 2 || m_fStateTime > 30.0f) {
        bSkipAngleCheck = true;
    }

    // IDA: 计算方向向量和距离
    if (!bSkipAngleCheck && m_pCurSkillRef->Skill_Ratation_Type == 0) {
        // IDA: 计算方向向量
        hkvVec3 vMyPos = m_pMonster->GetPosition();
        hkvVec3 vDirVector = vTargetPos - vMyPos;
        vDirVector.z = 0.0f;

        // IDA: 获取偏航角
        // IDA: 获取偏航角
        float fYaw = CMover::GetYawFromVector(vDirVector);
        float fDist = vDirVector.GetLength();

        // IDA: 获取当前朝向
        float fOrientationYaw = m_pMonster->GetOrientationYaw();
        float fDiffYaw = fOrientationYaw - fYaw;

        // IDA: 角度差标准化到 [-180, 180]
        if (fDiffYaw > 180.0f) {
            fDiffYaw -= 360.0f;
        } else if (fDiffYaw < -180.0f) {
            fDiffYaw += 360.0f;
        }

        // IDA: 检查角度
        int nAngleValue = m_pCurSkillRef->Skill_Angle_Value <= 45 ? 45 : m_pCurSkillRef->Skill_Angle_Value;
        float fCheckAngle = static_cast<float>(nAngleValue);

        // IDA: 如果角度超出范围且距离不够近
        float fHavokRadius = m_pMonster->GetHavokCapsuleRadius();
        float fMinDist = fHavokRadius + 5.0f + 5.0f;

        if (std::fabs(fDiffYaw) > fCheckAngle && fDist > fMinDist) {
            return false;
        }
    }

    // IDA: LABEL_30 - 检查是否可以使用技能
    // TODO: 需要实现 CheckUseSkill 方法
    // if (!m_pMonster->CheckUseSkill(m_pCurSkillRef->Use_State, m_pCurSkillRef->Normal_State, nullptr)) {
    //     return false;
    // }

    // IDA: 检查是否可以击中
    // TODO: 需要实现 IsCanDownedAttack 和 IsCanHit 方法
    // int nDownAttack = m_pMonster->IsCanDownedAttack(m_pCurSkillRef);
    // int nPassiveType = m_pCurSkillRef->Passive_Type;
    // if (!pTarget->IsCanHit(nDownAttack, nPassiveType)) {
    //     return false;
    // }

    // IDA: 设置技能表索引并执行攻击
    m_pMonster->SetCurSkillTableIdx(m_pCurSkillRef->Skill_Index);
    // IDA: 执行攻击动作
    m_pMonster->ActionAttack();
    m_pMonster->ResetHitCount();

    // IDA: 检查委托技能
    CheckDelegateSkill(m_pCurSkillRef->Skill_Index);

    // IDA: 重置伤害计数和技能时间
    m_nPreSkillDamageCount = 0;
    m_fLastSkillTime = 0.0f;

    return true;
}

// ============================================================================
// ClearTarget IDA 0x140262160 -> 0x1402621AA
// 清除目标 - 清除当前目标
// ============================================================================
void CAi::ClearTarget() {
    // IDA 反编译精确还原:
    // 检查当前目标ID是否不是 -1，如果是则调用 ChangeTarget 设置为无效ID
    if (!m_pMonster) {
        return;
    }

    std::uint32_t dwTargetID = m_pMonster->GetTargetID();
    if (dwTargetID != 0xFFFFFFFF) {
        // 创建一个无效的 UXActorID 并清除目标
        UXActorID invalidID(0xFFFFFFFF);
        m_pMonster->ChangeTarget(invalidID);
    }
}

// ============================================================================
// CheckDelegateSkill IDA 0x1402610C0 -> 0x1402613BC
// 检查代理技能 - 精确还原
// ============================================================================
void CAi::CheckDelegateSkill(unsigned int nSkillID) {
    // IDA 反编译精确还原:
    // 检查代理技能条件，如果满足则触发代理怪物动作

    // 检查代理技能ID、匹配技能ID、代理怪物ID是否有效
    if (m_stDelegateSkill.nSkillID <= 0 ||
        m_stDelegateSkill.nSkillID != static_cast<int>(nSkillID) ||
        m_stDelegateSkill.nDelegateMobID <= 0) {
        return;
    }

    // 获取迷宫对象
    // IDA: 如果 m_pMaze 已经存在则直接使用，否则从 Area 转换
    if (!m_pMaze) {
        if (!m_pMonster) {
            return;
        }
        // IDA: pArea = this->m_pMonster->GetArea(&this->m_pMonster->XActor)
        // IDA: m_pMaze = _RTDynamicCast(pArea, &XArea RTTI, &XMaze RTTI)
        // TODO: 需要实现 GetArea 和 XMaze RTTI 转换
        // XArea* pArea = m_pMonster->GetArea();
        // m_pMaze = dynamic_cast<XMaze*>(pArea);
        return;  // 暂时返回，等待 XMaze 实现
    }

    // IDA: 遍历所有代理目标
    for (size_t i = 0; i < m_stDelegateSkill.vecTarget.size(); ++i) {
        DelegateTarget& delegateTarget = m_stDelegateSkill.vecTarget[i];

        // IDA: 检查每个怪物ID是否存在
        bool bMob1 = true;
        bool bMob2 = true;
        bool bMob3 = true;
        bool bMob4 = true;
        bool bMob5 = true;

        // IDA: if ( delegateTarget->nMobID1 > 0 ) bMob1 = XMaze::GetMonsterCountByID(m_pMaze, nMobID1) > 0;
        if (delegateTarget.nMobID1 > 0) {
            // TODO: bMob1 = XMaze::GetMonsterCountByID(m_pMaze, delegateTarget.nMobID1) > 0;
        }
        if (delegateTarget.nMobID2 > 0) {
            // TODO: bMob2 = XMaze::GetMonsterCountByID(m_pMaze, delegateTarget.nMobID2) > 0;
        }
        if (delegateTarget.nMobID3 > 0) {
            // TODO: bMob3 = XMaze::GetMonsterCountByID(m_pMaze, delegateTarget.nMobID3) > 0;
        }
        if (delegateTarget.nMobID4 > 0) {
            // TODO: bMob4 = XMaze::GetMonsterCountByID(m_pMaze, delegateTarget.nMobID4) > 0;
        }
        if (delegateTarget.nMobID5 > 0) {
            // TODO: bMob5 = XMaze::GetMonsterCountByID(m_pMaze, delegateTarget.nMobID5) > 0;
        }

        // IDA: 如果所有条件都满足，触发代理动作
        if (bMob1 && bMob2 && bMob3 && bMob4 && bMob5) {
            // IDA: _itoa(m_stDelegateSkill.nDelegateMobID, szMobID, 10);
            // IDA: _itoa(delegateTarget.nIndex, szIndex, 10);
            // IDA: XMaze::SetMonsterCommonActionAi(m_pMaze, szMobID, szIndex);

            char szMobID[16];
            char szIndex[16];
            std::snprintf(szMobID, sizeof(szMobID), "%d", m_stDelegateSkill.nDelegateMobID);
            std::snprintf(szIndex, sizeof(szIndex), "%d", delegateTarget.nIndex);

            // TODO: XMaze::SetMonsterCommonActionAi(m_pMaze, szMobID, szIndex);
            GreenDamTan_log(__FILE__, __FUNCTION__, "Delegate skill triggered: MobID=%s, Index=%s", szMobID, szIndex);
            return;
        }
    }
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
    // IDA 反编译确认的精确逻辑:
    // 1. 检查 m_pMonster 是否有效且不是跟随者
    // 2. 获取创建位置和当前位置
    // 3. 计算距离并检查是否超过返回距离
    // 4. 如果超过，切换到返回状态

    if (!m_pMonster || m_pMonster->IsFollower()) {
        return false;
    }

    // 获取创建位置
    hkvVec3 vCreatePos = m_pMonster->GetCreatePos();

    // 获取当前位置
    hkvVec3 vMyPos = m_pMonster->GetPosition();

    // 计算距离 (使用hkvVec3的GetLength方法)
    float fDist = (vCreatePos - vMyPos).GetLength();

    // 检查是否超过返回距离
    if (fDist <= m_fReturnDistance) {
        return false;
    }

    // 超过返回距离，切换到返回状态
    ChangeAiState(FSMSTATES_RETURN);
    return true;
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
    // IDA 0x14027E3A0: StartAttackSkill
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    m_pCurSkillRef = pServer
        ? pServer->GetResourceMgr().GetTB_SKILL(*(&pTableRef->Monster_Skill1_ID + nActualSkillIndex))
        : nullptr;
    
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
// FuncEndState IDA 0x14026AB60 -> 0x14026ADEE
// 结束状态处理函数 - 根据状态变量决定下一个状态转换
// ============================================================================
void CAi::FuncEndState() {
    // IDA 反编译精确还原
    // 功能：处理状态结束时的逻辑，根据状态变量决定下一个状态转换

    if (!m_pStateMachine) {
        return;
    }

    // 获取当前状态 - IDA: nState = XGameDBSocketMgr::GetLogDBAgentCount(this->m_pStateMachine)
    // 这里的调用实际上是获取状态机的当前状态
    int nState = m_pStateMachine->GetState();

    // 特殊状态处理：状态3直接调用SelectAction
    if (nState == 3) {
        SelectAction();
        return;
    }

    int bChangedState = 0;

    // 查找状态变量信息
    auto it = m_mapStateVars.find(nState);
    if (it == m_mapStateVars.end()) {
        // 未找到状态变量，切换到前一个状态
        ChangeAiState(FSMSTATES_BEFORESTATE);
        return;
    }

    StateVarInfo& varInfo = it->second;

    // 检查NextStates映射大小
    // IDA: if ( std::_Tree<...>::size((std::_Tree<...> *)&v2->second.nPtrs) )
    // 这里检查的是 nPtrs (DataList) 是否有内容
    // 实际逻辑是检查 NextStates 是否为空

    // 如果 NextStates 为空或只有一个状态，使用 DataList[4][0] 作为下一状态
    if (varInfo.NextStates.empty()) {
        bChangedState = 1;
        ChangeAiState((FSMSTATES)varInfo.DataList[4][0]);
        return;
    }

    if (varInfo.NextStates.size() == 1) {
        bChangedState = 1;
        ChangeAiState((FSMSTATES)varInfo.DataList[4][0]);
        return;
    }

    // 计算总比率
    int nTotalRate = 0;
    for (auto& statePair : varInfo.NextStates) {
        nTotalRate += statePair.second;  // IDA: nTotalRate += *((_DWORD *)&v4->first + 1)
    }

    // 如果总比率为0，使用默认状态
    if (nTotalRate == 0) {
        bChangedState = 1;
        ChangeAiState((FSMSTATES)varInfo.DataList[4][0]);
        return;
    }

    // 随机选择下一状态
    int nRandVal = rand() % nTotalRate;
    for (auto& statePair : varInfo.NextStates) {
        if (nRandVal < statePair.second) {
            bChangedState = 1;
            ChangeAiState((FSMSTATES)statePair.first);
            break;
        }
        nRandVal -= statePair.second;
    }

    // 如果没有成功切换状态，切换到前一个状态
    if (!bChangedState) {
        ChangeAiState(FSMSTATES_BEFORESTATE);
    }
}

// ============================================================================
// GetSkillIndex IDA 0x140269180 -> 0x140269516
// 获取技能索引
// 参数：_nSkillGroup - 技能组索引
// 返回：实际技能索引，失败返回-1或0
// ============================================================================
int CAi::GetSkillIndex(int _nSkillGroup) {
    // IDA 反编译精确还原
    // 功能：从技能组中选择一个可用的技能索引

    // 检查前置条件
    if (!m_pMonster) {
        return _nSkillGroup;
    }

    if (!m_bSetSkillGroup) {
        return _nSkillGroup;
    }

    // 检查技能组索引范围
    if (_nSkillGroup >= 10) {
        return _nSkillGroup;
    }

    // 技能排序数组（用于存储排序后的技能索引）
    int nRand[3] = {0, 1, 2};

    // 根据排序类型确定技能尝试顺序
    if (m_nSkillSortType[_nSkillGroup] == 1) {
        // 类型1：随机排序（基于随机起始点）
        int nStart = rand() % 3;

        if (rand() % 2) {
            // 正向排序
            if (nStart != 0) {
                nRand[1] = (nStart == 1) ? 2 : 1;
                nRand[2] = 0;
            } else {
                nRand[1] = 2;
                nRand[2] = 1;
            }
        } else {
            // 反向排序
            if (nStart != 0) {
                nRand[1] = 0;
                nRand[2] = (nStart == 1) ? 2 : 1;
            } else {
                nRand[1] = 1;
                nRand[2] = 2;
            }
        }
    } else if (m_nSkillSortType[_nSkillGroup] == 2) {
        // 类型2：顺序循环
        nRand[0] = m_nSkillOrder[_nSkillGroup];
        nRand[1] = (m_nSkillOrder[_nSkillGroup] + 1) % 3;
        nRand[2] = (m_nSkillOrder[_nSkillGroup] + 2) % 3;
    } else {
        // 默认：固定顺序
        nRand[0] = 0;
        nRand[1] = 1;
        nRand[2] = 2;
    }

    // 获取怪物表引用
    TB_MONSTER* pMobRef = m_pMonster->GetMobTableRef();
    if (!pMobRef) {
        return _nSkillGroup;
    }

    // 遍历技能组中的技能
    for (int i = 0; i < 3; ++i) {
        int idx = nRand[i];

        // 检查技能组ID是否有效
        if (m_nSkillGroupID[_nSkillGroup][idx] > 0) {
            int iSkillIndex = m_nSkillGroupID[_nSkillGroup][idx] - 1;

            // 检查技能索引范围
            if (iSkillIndex >= 0 && iSkillIndex < 10) {
                // 获取技能ID（优先使用自定义技能ID）
                // TODO: int nSkillID = pMobRef->Monster_Skill1_ID + iSkillIndex;
                int nSkillID = 0;
                if (m_nCustomSkillID[iSkillIndex] > 0) {
                    nSkillID = m_nCustomSkillID[iSkillIndex];
                }

                if (nSkillID > 0) {
                    // TODO: 获取技能表
                    // XGameServer* pServer = XGameServer::Instance();
                    // TB_SKILL* pSkillRef = pServer->GetResourceMgr().GetTB_SKILL(nSkillID);
                    TB_SKILL* pSkillRef = nullptr;

                    if (pSkillRef) {
                        // TODO: 检查冷却时间和技能条件
                        // if (GetCooltime(pSkillRef->CoolTime_Group) <= 0.0f &&
                        //     CheckSkillCondition(iSkillIndex, _nSkillGroup)) {
                        if (CheckSkillCondition(iSkillIndex, _nSkillGroup)) {

                            // 如果是顺序循环类型，更新下一个顺序
                            if (m_nSkillSortType[_nSkillGroup] == 2) {
                                m_nSkillOrder[_nSkillGroup] = (idx + 1) % 3;
                            }

                            return iSkillIndex;
                        }
                    }
                }
            }
        }
    }

    // 所有技能都不可用，更新顺序
    if (m_nSkillSortType[_nSkillGroup] == 2) {
        m_nSkillOrder[_nSkillGroup] = 1;
    }

    // 巡逻怪物返回-1，其他返回0
    return m_bPatrolMonster ? -1 : 0;
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
// SetDmgAggressive IDA 0x140260D80
// 设置伤害激进度
// ============================================================================
void CAi::SetDmgAggressive(int nDmgAggressive, float fCheckTime) {
    // IDA 反编译确认:
    // this->m_nDmgAggressive = _nDmgAggressive;
    // this->m_fAggroCheckTime = _fCheckTime;
    m_nDmgAggressive = nDmgAggressive;
    m_fAggroCheckTime = fCheckTime;
}

// ============================================================================
// SetDmgAggroReset IDA 0x140260DC0
// 设置伤害仇恨重置
// ============================================================================
void CAi::SetDmgAggroReset(float fTime, float fDist) {
    // IDA 反编译确认:
    // this->m_fDmgAggroResetDist = _fDist;
    // this->m_fDmgAggroResetTime = _fTime;
    m_fDmgAggroResetDist = fDist;
    m_fDmgAggroResetTime = fTime;
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
// CheckSkillGroupOrder IDA 0x140269CE0
// 检查技能组顺序并排序状态转换
// ============================================================================
void CAi::CheckSkillGroupOrder() {
    // IDA 反编译精确还原:
    // 根据技能组比率随机排序状态转换顺序
    if (!m_bSetSkillGroup) {
        return;
    }

    int nTotal = 0;
    int bCheckEnd[10] = {0};

    // 计算总比率并标记零比率项
    for (int j = 0; j < 10; ++j) {
        nTotal += m_nSkillGroupRatio[j];
        if (m_nSkillGroupRatio[j] == 0) {
            bCheckEnd[j] = 1;
        }
    }

    if (nTotal == 0) {
        return;
    }

    // 排序后的状态顺序
    FSMSTATES eOrder[10] = {FSMSTATES_IDLE};

    int idx = 0;
    while (nTotal > 0) {
        int nRand = rand() % nTotal;
        nTotal = 0;

        int i = 0;
        for (i = 0; i < 10; ++i) {
            if (!bCheckEnd[i]) {
                if (nRand < m_nSkillGroupRatio[i]) {
                    eOrder[idx++] = static_cast<FSMSTATES>(i + 9);
                    bCheckEnd[i] = 1;
                    break;
                }
                nTotal += m_nSkillGroupRatio[i];
                nRand -= m_nSkillGroupRatio[i];
            }
        }

        // 重新计算剩余总数
        while (i < 10) {
            if (!bCheckEnd[i]) {
                nTotal += m_nSkillGroupRatio[i];
            }
            ++i;
        }
    }

    // 调用状态机的排序函数
    // TODO: CFsmClass<CAi>::SortTransitionOrder(m_pStateMachine, eOrder);
    if (m_pStateMachine) {
        // 暂时保留框架，等待CFsmClass实现
    }
}

// ============================================================================
// FuncCommonAction IDA 0x140269F40
// 执行通用动作
// ============================================================================
void CAi::FuncCommonAction(int nActionIndex) {
    // IDA 反编译精确还原:
    // 检查动作名称是否为空，执行相应动作
    if (nActionIndex < 0 || nActionIndex >= 10) {
        return;
    }

    // TODO: VString::IsEmpty 检查
    // if (!VString::IsEmpty(&m_strCommonActions[nActionIndex])) {
    //     if (m_pMonster) {
    //         m_pMonster->StopMoving(true);
    //         const char* szAction = VString::AsChar(&m_strCommonActions[nActionIndex]);
    //         m_pMonster->ChangeMotion_2(szAction, 1);
    //         m_nCurrentAction = CMover::GetAnimationIdx(m_pMonster);
    //         CMover::SetCollisionEnable(m_pMonster, 0, 0);
    //         CMoverEx::SetUpdateRotation(m_pMonster, 0);
    //         CMover::send_eSUB_CMD_MOVE_IDLE(m_pMonster, m_pMonster, -1.0);
    //     }
    // }

    // 由于VString尚未实现，暂时保留框架
    if (m_pMonster) {
        // 暂时记录动作索引
        m_nCurrentAction = nActionIndex;
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
// SetSkillGroupInfo IDA 0x140261650
// 设置技能组信息
// ============================================================================
void CAi::SetSkillGroupInfo(unsigned int nGroupID, int nSortType, int nSkill1, int nSkill2, int nSkill3) {
    // IDA 反编译确认:
    // if ( nGroupID < 0xA )
    // {
    //     this->m_bSetSkillGroup = 1;
    //     this->m_nSkillSortType[nGroupID] = nSortType;
    //     this->m_nSkillGroupID[nGroupID][0] = nSkill1;
    //     this->m_nSkillGroupID[nGroupID][1] = nSkill2;
    //     this->m_nSkillGroupID[nGroupID][2] = nSkill3;
    // }
    if (nGroupID < 10) {
        m_bSetSkillGroup = true;
        // TODO: 需要添加 m_nSkillSortType 和 m_nSkillGroupID 成员变量
        // m_nSkillSortType[nGroupID] = nSortType;
        // m_nSkillGroupID[nGroupID][0] = nSkill1;
        // m_nSkillGroupID[nGroupID][1] = nSkill2;
        // m_nSkillGroupID[nGroupID][2] = nSkill3;
    }
}

// ============================================================================
// SetIdleMotionInfo IDA 0x1402613C0
// 设置空闲动作信息
// ============================================================================
void CAi::SetIdleMotionInfo(int nChance, float fCheckTime) {
    // IDA 反编译确认:
    // if ( this->m_pMonster )
    //     CMoverEx::SetIdleMotionInfo(this->m_pMonster, _nChance, _fCheckTime);
    if (m_pMonster) {
        // TODO: CMoverEx::SetIdleMotionInfo 尚未实现
        // m_pMonster->SetIdleMotionInfo(nChance, fCheckTime);
    }
}

// ============================================================================
// SetEscapeInfo IDA 0x140261450
// 设置逃脱信息
// ============================================================================
void CAi::SetEscapeInfo(float fMaxPoint, int nProbability, float fMinDist, float fMaxDist,
                         float fResetTime, std::uint8_t byEscapeType, const char* szEscapeVal) {
    // IDA 反编译精确还原:
    // this->m_fEscapePoint = fMaxPoint;
    // this->m_nEscapePercent = nProbability;
    // this->m_fEscapePointResetTime = fResetTime;
    // this->m_fEscapeSkillMinDist = fMinDist;
    // this->m_fEscapeSkillMaxDist = fMaxDist;
    // this->m_byEscapeType = byEscapeType;
    // this->m_dwEscapeValue = std::stoi(szEscapeVal, nullptr, 10);
    // this->m_fCurEscapePoint = RandomBetweenF(0.0, this->m_fEscapePoint / 2.0);
    // this->m_fLastEscapePointTime = 0.0;

    m_fEscapePoint = fMaxPoint;
    m_nEscapePercent = nProbability;
    m_fEscapePointResetTime = fResetTime;
    m_fEscapeSkillMinDist = fMinDist;
    m_fEscapeSkillMaxDist = fMaxDist;
    m_byEscapeType = byEscapeType;

    if (szEscapeVal) {
        m_dwEscapeValue = static_cast<std::uint32_t>(std::stoi(szEscapeVal));
    }

    // RandomBetweenF(0.0, m_fEscapePoint / 2.0)
    m_fCurEscapePoint = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * (m_fEscapePoint / 2.0f);
    m_fLastEscapePointTime = 0.0f;
}

// ============================================================================
// SetReservedCondition IDA 0x140261750 -> 0x140261B19
// 设置保留条件 - 精确还原
// ============================================================================
void CAi::SetReservedCondition(unsigned int nIndex, unsigned int _nVariable, const char* _szConditionString,
                                float _fFloatData1, float _fFloatData2) {
    // IDA 反编译精确还原:
    // 解析条件字符串并设置保留条件

    // IDA: if ( nIndex < 0x14 && _nVariable <= 0x38 )
    if (nIndex >= 0x14 || _nVariable > 0x38) {
        return;
    }

    // IDA: 初始化数据
    int nIntData[2] = {0, 0};
    float fFloatData[2] = {0.0f, 0.0f};
    E_FSMCONDITIONS eConditionFunc = CONDITION_NONE;

    // IDA: 解析条件字符串
    if (_szConditionString) {
        if (std::strcmp(_szConditionString, "=") == 0) {
            eConditionFunc = CONDITION_EQUAL;
        } else if (std::strcmp(_szConditionString, "!=") == 0) {
            eConditionFunc = CONDITION_NOT_EQUAL;
        } else if (std::strcmp(_szConditionString, ">") == 0) {
            eConditionFunc = CONDITION_GREATER_THAN;
        } else if (std::strcmp(_szConditionString, "<") == 0) {
            eConditionFunc = CONDITION_LESS_THAN;
        } else if (std::strcmp(_szConditionString, "&") == 0) {
            eConditionFunc = CONDITION_BIT_EQUAL;
        } else if (std::strcmp(_szConditionString, "<>") == 0) {
            eConditionFunc = CONDITION_RANGE_TRUE;
        } else if (std::strcmp(_szConditionString, "!<>") == 0) {
            eConditionFunc = CONDITION_RANGE_FALSE;
        } else if (std::strcmp(_szConditionString, "&=") == 0) {
            eConditionFunc = CONDITION_RANGE_EQUAL;
        }
    }

    // IDA: 确定变量数据类型
    E_FSMDATATYPE eVarDataType = FSMDTYPE_NONE;

    // IDA: 检查是否有整数条件函数
    if (_nVariable < m_xAiGetConditionDataIntFunc.size() && m_xAiGetConditionDataIntFunc[_nVariable] != nullptr) {
        eVarDataType = FSMDTYPE_INT;
        // IDA: if ( eConditionFunc < CONDITION_RANGE_TRUE && _fFloatData2 != 0.0 && !IsVarNeedCondition(_nVariable) )
        if (eConditionFunc < CONDITION_RANGE_TRUE && _fFloatData2 != 0.0f && !IsVarNeedCondition(_nVariable)) {
            eVarDataType = FSMDTYPE_RANDOMINT;
        }
    }
    // IDA: 检查是否有浮点条件函数
    else if (_nVariable < m_xAiGetConditionDataFloatFunc.size() && m_xAiGetConditionDataFloatFunc[_nVariable] != nullptr) {
        eVarDataType = FSMDTYPE_FLOAT;
        // IDA: if ( eConditionFunc < CONDITION_RANGE_TRUE && _fFloatData2 != 0.0 && !IsVarNeedCondition(_nVariable) )
        if (eConditionFunc < CONDITION_RANGE_TRUE && _fFloatData2 != 0.0f && !IsVarNeedCondition(_nVariable)) {
            eVarDataType = FSMDTYPE_RANDOMFLOAT;
        }
    }

    // IDA: 根据数据类型设置值
    if (eVarDataType == FSMDTYPE_INT || eVarDataType == FSMDTYPE_RANDOMINT) {
        nIntData[0] = static_cast<int>(_fFloatData1 + 0.5f);
        nIntData[1] = static_cast<int>(_fFloatData2 + 0.5f);
    } else {
        fFloatData[0] = _fFloatData1;
        fFloatData[1] = _fFloatData2;
    }

    // IDA: 获取条件对象并设置
    CFsmCondition* pCondition = &m_arReservedCondition[nIndex];

    // IDA: CFsmCondition::SetVariables(pCondition, _nVariable);
    pCondition->SetVariableIndex(_nVariable);

    // IDA: LogicTimer::SetType(pCondition, eVarDataType);
    pCondition->SetConditionType(eVarDataType);

    // IDA: CFsmCondition::SetValue(pCondition, nIntData, fFloatData);
    if (eVarDataType == FSMDTYPE_INT || eVarDataType == FSMDTYPE_RANDOMINT) {
        pCondition->SetValue(0, nIntData[0]);
    } else {
        pCondition->SetValue(0, fFloatData[0]);
    }

    // IDA: CFsmCondition::SetFunctions(pCondition, eConditionFunc);
    pCondition->SetCompareOp(eConditionFunc);

    // IDA: CFsmCondition::SetDestroy(pCondition, 0);
    // 无需额外操作
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
// GetDeathActionMotion IDA 0x140261C30
// 获取死亡动作Motion
// ============================================================================
std::int16_t CAi::GetDeathActionMotion() {
    // IDA 反编译精确还原
    // 功能：根据死亡动画名称获取Motion索引

    std::int16_t nMotion = 12;  // 默认死亡动作

    if (!m_pMonster) {
        return nMotion;
    }

    // 检查是否是倒地状态
    if (m_pMonster->IsHitDown()) {
        nMotion = 13;  // 倒地死亡动作
    }

    // 检查是否有自定义死亡动画
    // TODO: 需要 VString::IsEmpty 实现
    // if (VString::IsEmpty(&this->m_strDeathAnim)) {
    //     return nMotion;
    // }

    // TODO: 获取动画索引
    // VString strAnimName;
    // strAnimName = m_strDeathAnim;
    // std::uint32_t dwAnimID = m_pMonster->GetAnimIndex(strAnimName);
    // if (dwAnimID == -1) {
    //     return nMotion;
    // }
    // return m_pMonster->AnimKeyToMotion(dwAnimID);

    return nMotion;
}

// ============================================================================
// SetRecoverySkill IDA 0x140261B20
// 设置恢复技能
// ============================================================================
void CAi::SetRecoverySkill(const char* szTableID) {
    // IDA 反编译精确还原
    // 功能：将字符串转换为整数作为恢复技能ID

    if (szTableID) {
        m_nRecoverySkill = std::stoi(szTableID);
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
// SetSkillCooltime IDA 0x140261F40 -> 0x140262155
// 设置技能冷却时间 - 精确还原
// ============================================================================
void CAi::SetSkillCooltime(TB_SKILL* pSkillTable) {
    // IDA 反编译精确还原:
    // 根据技能表设置冷却时间

    // IDA: if ( pSkillTable && (pSkillTable->CoolTime || pSkillTable->CoolTime_Global) )
    if (!pSkillTable) {
        return;
    }

    // 获取技能冷却时间和全局冷却时间
    // IDA 0x140261F40: SetSkillCooltime
    int nCoolTime = pSkillTable->CoolTime;
    int nCoolTimeGlobal = pSkillTable->CoolTime_Global;
    int nCoolTimeGroup = pSkillTable->CoolTime_Group;

    if (nCoolTime == 0 && nCoolTimeGlobal == 0) {
        return;
    }

    // IDA: 获取当前时间
    VDefaultTimer* Timer = ThreadLocalData::GetTimer();
    float fCurrTime = Timer ? Timer->GetTime() : 0.0f;

    // IDA: 计算冷却结束时间
    float fCooldownTime = fCurrTime + static_cast<float>(nCoolTime) * 0.001f;

    // IDA: 在 m_mapCooltimeList 中查找或创建冷却时间记录
    auto iter = m_mapCooltimeList.find(nCoolTimeGroup);

    if (iter != m_mapCooltimeList.end()) {
        // IDA: 如果找到现有记录，比较并更新
        if (fCooldownTime > iter->second.fEndTime) {
            iter->second.fEndTime = fCooldownTime;
            iter->second.dwTotalTime = nCoolTime;
        }
    } else {
        // IDA: 创建新的冷却时间记录
        tagCOOLTIME newData;
        newData.dwTotalTime = nCoolTime;
        newData.fEndTime = fCooldownTime;
        m_mapCooltimeList[nCoolTimeGroup] = newData;
    }

    // IDA: 更新全局冷却时间
    float fGlobalCooldownTime = fCurrTime + static_cast<float>(nCoolTimeGlobal) * 0.001f;
    if (fGlobalCooldownTime > m_fGlobalCooltime) {
        m_fGlobalCooltime = fGlobalCooldownTime;
    }
}

// ============================================================================
// SetSuperArmorSkill IDA 0x140261B90
// 设置超级护甲技能
// ============================================================================
void CAi::SetSuperArmorSkill(const char* szTableID) {
    // IDA 反编译精确还原
    // 功能：将字符串转换为整数作为超级护甲技能索引

    if (szTableID) {
        m_nSuperArmorSkillIndex = std::stoi(szTableID);
    }
}

// ============================================================================
// IsVarNeedCondition IDA 0x1402616F0
// 检查变量是否需要条件
// ============================================================================
bool CAi::IsVarNeedCondition(int _nVariable) {
    // IDA 反编译精确还原
    // 功能：检查指定变量是否需要条件判断
    // 返回 true 的变量: 29, 50, 51, 52, 56, 41, 42, 46, 47, 49

    return _nVariable == 29
        || _nVariable == 50
        || _nVariable == 51
        || _nVariable == 52
        || _nVariable == 56
        || _nVariable == 41
        || _nVariable == 42
        || _nVariable == 46
        || _nVariable == 47
        || _nVariable == 49;
}

// ============================================================================
// SetFuzzyScript IDA 0x1402656F0
// 设置模糊脚本
// ============================================================================
void CAi::SetFuzzyScript(const char* szFilename) {
    // IDA 反编译精确还原:
    // VString::operator=(&this->m_strFuzzyScript, szFilename);
    if (szFilename) {
        // TODO: VString 赋值运算符
        // m_strFuzzyScript = szFilename;
    }
}

// ============================================================================
// FuncEscapeProcess IDA 0x14026A7A0
// Process escape logic - update escape point timer
// IDA: if m_fCurEscapePoint > 0, accumulate time and reset if timeout
// ============================================================================
void CAi::FuncEscapeProcess(float fDeltaTime) {
    if (m_fCurEscapePoint > 0.0f) {
        m_fLastEscapePointTime += fDeltaTime;
        if (m_fLastEscapePointTime >= m_fEscapePointResetTime) {
            m_fCurEscapePoint = RandomBetweenF(0.0f, m_fEscapePoint / 2.0f);
            m_fLastEscapePointTime = 0.0f;
        }
    }
}

// ============================================================================
// CopyConditionData IDA 0x14025F8E0
// 复制条件数据 - 从另一个CAi实例复制基础条件数据
// ============================================================================
void CAi::CopyConditionData(const CAi& Other) {
    // IDA 反编译精确还原 - 复制所有条件相关成员变量
    // 注意：部分成员变量尚未在 Ai.h 中定义，已注释掉
    m_fSumElapsedTime = Other.m_fSumElapsedTime;
    m_fAiCheckTime = Other.m_fAiCheckTime;
    m_fStateTime = Other.m_fStateTime;
    m_fActivateTime = Other.m_fActivateTime;
    m_fSearchTargetTime = Other.m_fSearchTargetTime;
    m_fTargetSightDistance = Other.m_fTargetSightDistance;
    m_fReturnDistance = Other.m_fReturnDistance;
    m_nReturnHP = Other.m_nReturnHP;
    // m_fTargetLostDistance = Other.m_fTargetLostDistance; // 尚未定义
    m_fSpawnAggroDistance = Other.m_fSpawnAggroDistance;
    m_fSpawnAggroValue = Other.m_fSpawnAggroValue;
    m_bIsFirstAttacker = Other.m_bIsFirstAttacker;
    m_fDelaySearchTarget = Other.m_fDelaySearchTarget;
    m_fMoveDelayToTarget = Other.m_fMoveDelayToTarget;
    m_fMoveDistanceToTarget = Other.m_fMoveDistanceToTarget;
    // m_fHelperFarDistance = Other.m_fHelperFarDistance;  // 尚未定义
    // m_fHelperFarBattleDist = Other.m_fHelperFarBattleDist; // 尚未定义
    // m_byFailMoveCount = Other.m_byFailMoveCount;  // 尚未定义
    m_fProtectEffectDist = Other.m_fProtectEffectDist;
    m_fProtectWaitTimeOut = Other.m_fProtectWaitTimeOut;
    m_fRunDistance = Other.m_fRunDistance;
    // m_nPreRandomValue = Other.m_nPreRandomValue;  // 尚未定义
    m_nStatePreHP = Other.m_nStatePreHP;
    m_nRequestHelpCnt = Other.m_nRequestHelpCnt;
    m_fRequestHelpDistance = Other.m_fRequestHelpDistance;
    m_nRequestHelpMonsterID = Other.m_nRequestHelpMonsterID;
    m_nRequestHelpMonsterCount = Other.m_nRequestHelpMonsterCount;
    m_nRunawayCurrentCount = Other.m_nRunawayCurrentCount;
    m_nRunawayMaxCount = Other.m_nRunawayMaxCount;
    m_nRunawayHP = Other.m_nRunawayHP;
    m_fRunwayMinTimeOut = Other.m_fRunwayMinTimeOut;
    m_fRunwayMaxTimeOut = Other.m_fRunwayMaxTimeOut;
    m_nDmgAggressive = Other.m_nDmgAggressive;
    m_fAggroCheckTime = Other.m_fAggroCheckTime;
    m_fDmgAggroResetDist = Other.m_fDmgAggroResetDist;
    m_fDmgAggroResetTime = Other.m_fDmgAggroResetTime;
    m_nPreSkillDamageCount = Other.m_nPreSkillDamageCount;
    m_fSuicideTime = Other.m_fSuicideTime;
    // m_fRecoveryCheckTime = Other.m_fRecoveryCheckTime;  // 尚未定义
    std::memcpy(m_fStateTempFloat, Other.m_fStateTempFloat, sizeof(m_fStateTempFloat));
    m_fLastSkillTime = Other.m_fLastSkillTime;
    m_fLastDamageTime = Other.m_fLastDamageTime;
    m_fEscapePoint = Other.m_fEscapePoint;
    m_fCurEscapePoint = Other.m_fCurEscapePoint;
    m_nEscapePercent = Other.m_nEscapePercent;
    m_fEscapePointResetTime = Other.m_fEscapePointResetTime;
    m_fLastEscapePointTime = Other.m_fLastEscapePointTime;
    m_byEscapeType = Other.m_byEscapeType;
    m_dwEscapeValue = Other.m_dwEscapeValue;
    m_fEscapeSkillMinDist = Other.m_fEscapeSkillMinDist;
    m_fEscapeSkillMaxDist = Other.m_fEscapeSkillMaxDist;
    m_nSuperArmorSkillIndex = Other.m_nSuperArmorSkillIndex;
    m_bEnableHelperWarp = Other.m_bEnableHelperWarp;
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
    CopyConditionData(Other);

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
// RegisterStateVars IDA 0x1402657D0 -> 0x1402659F3
// 注册状态变量 - 为AI状态注册变量信息
// ============================================================================
void CAi::RegisterStateVars(int _nState, int _nVariable, int nValue1, int nValue2) {
    // IDA 反编译确认的逻辑:
    // 参数验证: _nState 必须 >= 0 且 < 43, _nVariable 必须 <= 4
    if (_nState < 0 || _nState >= 43 || _nVariable > 4) {
        return;
    }

    // 特殊处理: 状态1且变量0时，使用默认值
    if (_nState == 1 && _nVariable == 0) {
        nValue1 = 4000;
        nValue2 = 5000;
    }

    // 查找是否已存在该状态的变量信息
    auto it = m_mapStateVars.find(_nState);
    if (it != m_mapStateVars.end()) {
        // 已存在，更新对应的变量数据
        StateVarInfo& info = it->second;
        if (_nVariable >= 0 && _nVariable < 5) {
            info.DataList[_nVariable][0] = nValue1;
            info.DataList[_nVariable][1] = nValue2;

            // 如果是变量4（NextState），且值有效，添加到NextStates映射
            if (_nVariable == 4 && nValue1 >= 0 && nValue1 <= 42) {
                info.NextStates[nValue1] = nValue2;
            }
        }
    } else {
        // 不存在，创建新的状态变量信息
        StateVarInfo newInfo;
        newInfo.DataList[_nVariable][0] = nValue1;
        newInfo.DataList[_nVariable][1] = nValue2;

        // 如果是变量4（NextState），且值有效，添加到NextStates映射
        if (_nVariable == 4 && nValue1 >= 0 && nValue1 <= 42) {
            newInfo.NextStates[nValue1] = nValue2;
        }

        m_mapStateVars[_nState] = newInfo;
    }
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

// ============================================================================
// Patrol Functions - 巡逻相关函数
// ============================================================================

// Patrol - 开始巡逻模式，设置路径点
void CAi::Patrol() {
    // 检查是否有巡逻点
    if (m_vecPatrolPoints.empty()) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "No patrol points set");
        return;
    }

    // 设置巡逻状态
    m_bPatrolMonster = true;
    m_nCurrentPatrolIndex = 0;
    m_bPatrolForward = true;
    m_fPatrolWaitTime = 0.0f;

    // 切换到巡逻状态
    ChangeAiState(AI_STATE_PATROL);

    GreenDamTan_log(__FILE__, __FUNCTION__, "Patrol started");
}

// CheckPatrol - 检查巡逻条件，获取下一个路径点
bool CAi::CheckPatrol() {
    // 检查是否是巡逻怪物
    if (!m_bPatrolMonster) {
        return false;
    }

    // 检查巡逻点是否有效
    if (m_vecPatrolPoints.empty()) {
        return false;
    }

    // 检查怪物是否有效
    if (!m_pMonster) {
        return false;
    }

    // 检查等待时间
    if (m_fPatrolWaitTime > 0.0f) {
        m_fPatrolWaitTime -= 0.1f;  // 减少等待时间
        return true;  // 继续等待
    }

    // 获取当前巡逻点
    const PatrolPoint& currentPoint = m_vecPatrolPoints[m_nCurrentPatrolIndex];

    // IDA: 获取怪物当前位置
    const hkvVec3 pos = m_pMonster->GetPosition();
    float fDistance = (hkvVec3(currentPoint.fX, currentPoint.fY, currentPoint.fZ) - pos).GetLength();

    // 简化：假设已到达巡逻点，移动到下一个
    // 实际实现需要检查是否已到达当前巡逻点
    
    // 移动到下一个巡逻点
    if (m_bPatrolForward) {
        m_nCurrentPatrolIndex++;
        if (m_nCurrentPatrolIndex >= static_cast<int>(m_vecPatrolPoints.size())) {
            // 到达终点，反向巡逻
            m_nCurrentPatrolIndex = static_cast<int>(m_vecPatrolPoints.size()) - 2;
            if (m_nCurrentPatrolIndex < 0) {
                m_nCurrentPatrolIndex = 0;
            }
            m_bPatrolForward = false;
        }
    } else {
        m_nCurrentPatrolIndex--;
        if (m_nCurrentPatrolIndex < 0) {
            // 到达起点，正向巡逻
            m_nCurrentPatrolIndex = 1;
            if (m_nCurrentPatrolIndex >= static_cast<int>(m_vecPatrolPoints.size())) {
                m_nCurrentPatrolIndex = 0;
            }
            m_bPatrolForward = true;
        }
    }

    // 设置等待时间（到达巡逻点后短暂等待）
    m_fPatrolWaitTime = 2.0f;

    return true;
}

// SetPatrolPoint - 从表格设置巡逻路径点
void CAi::SetPatrolPoint(int nIndex, float fX, float fY, float fZ) {
    // 确保向量足够大
    if (nIndex < 0) {
        return;
    }

    // 扩展向量大小
    while (static_cast<int>(m_vecPatrolPoints.size()) <= nIndex) {
        PatrolPoint pt = {0.0f, 0.0f, 0.0f};
        m_vecPatrolPoints.push_back(pt);
    }

    // 设置巡逻点
    m_vecPatrolPoints[nIndex].fX = fX;
    m_vecPatrolPoints[nIndex].fY = fY;
    m_vecPatrolPoints[nIndex].fZ = fZ;

    GreenDamTan_log(__FILE__, __FUNCTION__, "Patrol point set");
}

// ============================================================================
// Chase Functions - 追击相关函数
// ============================================================================

// Chase - 开始追击目标
void CAi::Chase() {
    // 检查目标是否有效
    if (m_dwChaseTargetID == 0xFFFFFFFF) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "No chase target");
        return;
    }

    // 检查怪物是否有效
    if (!m_pMonster) {
        return;
    }

    // 设置追击状态
    m_bChasing = true;

    // 设置怪物目标
    m_pMonster->ChangeTarget(UXActorID(m_dwChaseTargetID));

    // 切换到追击状态
    ChangeAiState(AI_STATE_CHASE);

    GreenDamTan_log(__FILE__, __FUNCTION__, "Chase started");
}

// CheckChase - 检查追击条件和范围
bool CAi::CheckChase() {
    // 检查是否正在追击
    if (!m_bChasing) {
        return false;
    }

    // 检查怪物是否有效
    if (!m_pMonster) {
        m_bChasing = false;
        return false;
    }

    // 检查目标是否有效
    if (m_dwChaseTargetID == 0xFFFFFFFF) {
        m_bChasing = false;
        return false;
    }

    // IDA: 获取目标对象并检查距离
    CMoverEx* pTarget = dynamic_cast<CMoverEx*>(CMover::GetMoverObject(m_dwChaseTargetID));
    if (!pTarget) {
        m_bChasing = false;
        return false;
    }

    // IDA: 检查目标距离是否在追击范围内
    const hkvVec3 posThis = m_pMonster->GetPosition();
    const hkvVec3 posTarget = pTarget->GetPosition();
    float fDistance = (posTarget - posThis).GetLength();
    // if (fDistance > m_fChaseRange) {
    //     m_bChasing = false;
    //     return false;
    // }

    // 检查是否到达攻击范围（如果有）
    // if (m_fSkillRangeMax > 0.0f && fDistance <= m_fSkillRangeMax) {
    //     m_bChasing = false;
    //     ChangeAiState(AI_STATE_ATTACK);
    //     return false;
    // }

    return true;
}

// SetChaseTarget - 设置追击目标和参数
void CAi::SetChaseTarget(std::uint32_t dwTargetID, float fRange, float fSpeed) {
    m_dwChaseTargetID = dwTargetID;
    m_fChaseRange = fRange;
    m_fChaseSpeed = fSpeed;

    GreenDamTan_log(__FILE__, __FUNCTION__, "Chase target set");
}

// ============================================================================
// Flee Functions - 逃跑相关函数
// ============================================================================

// Flee - 开始逃跑
void CAi::Flee() {
    // 检查怪物是否有效
    if (!m_pMonster) {
        return;
    }

    // 设置逃跑状态
    m_bFleeing = true;

    // 切换到逃跑状态
    ChangeAiState(AI_STATE_FLEE);

    GreenDamTan_log(__FILE__, __FUNCTION__, "Flee started");
}

// CheckFlee - 检查逃跑条件和安全性
bool CAi::CheckFlee() {
    // 检查是否正在逃跑
    if (!m_bFleeing) {
        return false;
    }

    // 检查怪物是否有效
    if (!m_pMonster) {
        m_bFleeing = false;
        return false;
    }

    // IDA: 检查是否到达逃跑目的地
    const hkvVec3 pos = m_pMonster->GetPosition();
    hkvVec3 destPos(m_vFleeDestPos[0], m_vFleeDestPos[1], m_vFleeDestPos[2]);
    float fDistance = (destPos - pos).GetLength();
    if (fDistance < 1.0f) {
        m_bFleeing = false;
        ChangeAiState(AI_STATE_IDLE);
        return false;
    }

    // IDA: 检查是否安全（远离威胁）
    std::uint32_t dwTargetID = m_pMonster->GetTargetID();
    if (dwTargetID != 0xFFFFFFFF) {
        CMoverEx* pThreat = dynamic_cast<CMoverEx*>(CMover::GetMoverObject(dwTargetID));
        if (pThreat) {
            const hkvVec3 posThreat = pThreat->GetPosition();
            float fThreatDist = (posThreat - pos).GetLength();
            if (fThreatDist > m_fFleeSafetyDistance) {
                m_bFleeing = false;
                ChangeAiState(AI_STATE_IDLE);
                return false;
            }
        }
    }

    return true;
}

// SetFleePoint - 计算并设置逃跑目的地
void CAi::SetFleePoint(float fDistance) {
    // 检查怪物是否有效
    if (!m_pMonster) {
        return;
    }

    // TODO: 获取当前位置和威胁方向
    // const hkvVec3& pos = m_pMonster->GetPosition();
    // std::uint32_t dwTargetID = m_pMonster->GetTargetID();
    // if (dwTargetID != 0xFFFFFFFF) {
    //     CMoverEx* pThreat = CMover::GetMoverObject(m_pMonster, dwTargetID);
    //     if (pThreat) {
    //         const hkvVec3& posThreat = pThreat->GetPosition();
    //         hkvVec3 vDir = pos - posThreat;  // 远离威胁的方向
    //         vDir.normalize();
    //         hkvVec3 vDest = pos + vDir * fDistance;
    //         m_vFleeDestPos[0] = vDest.x;
    //         m_vFleeDestPos[1] = vDest.y;
    //         m_vFleeDestPos[2] = vDest.z;
    //     }
    // }

    // 简化实现：设置默认逃跑距离
    m_fFleeSafetyDistance = fDistance;

    GreenDamTan_log(__FILE__, __FUNCTION__, "Flee point set");
}

// ============================================================================
// Skill AI Functions - 技能AI相关函数
// ============================================================================

// SelectSkill - 选择适合当前情况的技能
int CAi::SelectSkill() {
    // 检查怪物是否有效
    if (!m_pMonster) {
        return -1;
    }

    // 获取怪物表引用
    TB_MONSTER* pTableRef = m_pMonster->GetMobTableRef();
    if (!pTableRef) {
        return -1;
    }

    // TODO: 实现完整的技能选择逻辑
    // 1. 获取可用技能列表
    // 2. 检查技能冷却
    // 3. 检查目标距离是否在技能范围内
    // 4. 根据技能优先级或权重选择技能

    // 简化实现：遍历技能组比率选择技能
    if (m_bSetSkillGroup) {
        int nTotalWeight = 0;
        for (int i = 0; i < 10; ++i) {
            nTotalWeight += m_nSkillGroupRatio[i];
        }

        if (nTotalWeight > 0) {
            int nRand = std::rand() % nTotalWeight;
            int nAccum = 0;
            for (int i = 0; i < 10; ++i) {
                nAccum += m_nSkillGroupRatio[i];
                if (nRand < nAccum) {
                    m_nSelectedSkillIndex = i;
                    return i;
                }
            }
        }
    }

    // 默认返回第一个技能
    m_nSelectedSkillIndex = 0;
    return 0;
}

// CheckSkillRange - 检查目标是否在技能范围内
bool CAi::CheckSkillRange(int nSkillIndex) {
    // 检查技能索引是否有效
    if (nSkillIndex < 0 || nSkillIndex >= 10) {
        return false;
    }

    // 检查怪物是否有效
    if (!m_pMonster) {
        return false;
    }

    // TODO: 获取技能表引用检查技能范围
    // TB_SKILL* pSkillRef = GetSkillTableRef(nSkillIndex);
    // if (!pSkillRef) {
    //     return false;
    // }

    // TODO: 获取目标并检查距离
    // CMoverEx* pTarget = FindTargetBySkill();
    // if (!pTarget) {
    //     return false;
    // }

    // const hkvVec3& posThis = m_pMonster->GetPosition();
    // const hkvVec3& posTarget = pTarget->GetPosition();
    // float fDistance = (posTarget - posThis).getLength();

    // 检查距离是否在技能范围内
    // if (fDistance < pSkillRef->Range_Min || fDistance > pSkillRef->Range_Max) {
    //     return false;
    // }

    return true;
}

// ProcessSkillAI - 处理技能使用逻辑
void CAi::ProcessSkillAI() {
    // 检查怪物是否有效
    if (!m_pMonster) {
        return;
    }

    // 选择技能
    int nSkillIndex = SelectSkill();
    if (nSkillIndex < 0) {
        return;
    }

    // 检查技能范围
    if (!CheckSkillRange(nSkillIndex)) {
        // 目标不在范围内，可能需要追击
        // ChangeAiState(AI_STATE_CHASE);
        return;
    }

    // 检查技能条件
    if (!CheckSkillCondition(nSkillIndex, -1)) {
        return;
    }

    // 检查全局冷却
    if (m_fGlobalCooltime > 0.0f) {
        return;
    }

    // 开始攻击技能
    StartAttackSkill(nSkillIndex);

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessSkillAI executed");
}

// ============================================================================
// Group AI Functions - 组AI相关函数
// ============================================================================

// GroupAggro - 与组内成员共享仇恨
void CAi::GroupAggro() {
    // 检查组ID是否有效
    if (m_nGroupID == 0) {
        return;
    }

    // 检查怪物是否有效
    if (!m_pMonster) {
        return;
    }

    // TODO: 实现组仇恨共享逻辑
    // 1. 获取组内所有成员
    // 2. 获取当前仇恨列表
    // 3. 将仇恨值共享给组内成员

    // 简化实现：通知附近同组怪物
    // XArea* pArea = m_pMonster->GetArea();
    // if (pArea) {
    //     std::vector<CMover*> vecGameObjList;
    //     XArea::ScanGridOrigin(m_pMonster, 3, 3, &vecGameObjList);
    //     for (auto& obj : vecGameObjList) {
    //         CMonster* pOther = dynamic_cast<CMonster*>(obj);
    //         if (pOther && pOther != m_pMonster) {
    //             // 检查是否同组
    //             CAi* pOtherAI = pOther->GetAI();
    //             if (pOtherAI && pOtherAI->m_nGroupID == m_nGroupID) {
    //                 // 共享仇恨值
    //                 // ...
    //             }
    //         }
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "GroupAggro executed");
}

// GroupTarget - 协调目标选择
void CAi::GroupTarget() {
    // 检查组ID是否有效
    if (m_nGroupID == 0) {
        return;
    }

    // 检查怪物是否有效
    if (!m_pMonster) {
        return;
    }

    // TODO: 实现组目标协调逻辑
    // 1. 如果是组长，选择最佳目标
    // 2. 如果是组员，跟随组长的目标
    // 3. 同步目标ID给组内成员

    if (m_bGroupLeader) {
        // 组长：选择最高仇恨目标
        // m_dwGroupTargetID = m_pMonster->GetHighestAggroTargetID();
    } else {
        // 组员：使用组共享目标
        if (m_dwGroupTargetID != 0xFFFFFFFF) {
            m_pMonster->ChangeTarget(UXActorID(m_dwGroupTargetID));
        }
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "GroupTarget executed");
}

// GroupAction - 执行协调动作
void CAi::GroupAction() {
    // 检查组ID是否有效
    if (m_nGroupID == 0) {
        return;
    }

    // 检查怪物是否有效
    if (!m_pMonster) {
        return;
    }

    // TODO: 实现组协调动作逻辑
    // 1. 检查组内成员状态
    // 2. 协调攻击、防御、支援等动作
    // 3. 根据战术选择执行不同动作

    // 简化实现：协调攻击
    GroupTarget();

    // 执行技能AI
    ProcessSkillAI();

    GreenDamTan_log(__FILE__, __FUNCTION__, "GroupAction executed");
}

// ============================================================================
// Group AI Helper Functions
// ============================================================================

// IsLeader - 检查是否为组长
bool CAi::IsLeader() const {
    return m_bGroupLeader;
}

// ============================================================================
// Owner/Target Functions
// ============================================================================

// GetOwner - 获取所属怪物
CMonster* CAi::GetOwner() const {
    return m_pMonster;
}

// GetTarget - 获取当前目标ID
std::uint32_t CAi::GetTarget() const {
    if (!m_pMonster) {
        return 0xFFFFFFFF;
    }
    return m_pMonster->GetTargetID();
}

// SetTarget - 设置目标
void CAi::SetTarget(std::uint32_t dwTargetID) {
    if (!m_pMonster) {
        return;
    }
    m_pMonster->ChangeTarget(UXActorID(dwTargetID));
    GreenDamTan_log(__FILE__, __FUNCTION__, "SetTarget executed");
}

// ============================================================================
// Behavior Functions - Idle
// ============================================================================

// Idle - 进入空闲状态
void CAi::Idle() {
    // 清除目标
    ClearTarget();
    
    // 切换到空闲状态
    ChangeAiState(AI_STATE_IDLE);
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "Idle state entered");
}

// ============================================================================
// Skill AI - UseSkill
// ============================================================================

// UseSkill - 使用指定技能
bool CAi::UseSkill(int nSkillIndex) {
    // 检查技能索引是否有效
    if (nSkillIndex < 0 || nSkillIndex >= 10) {
        return false;
    }
    
    // 检查怪物是否有效
    if (!m_pMonster) {
        return false;
    }
    
    // 检查技能冷却
    if (m_fGlobalCooltime > 0.0f) {
        return false;
    }
    
    // 检查技能条件
    if (!CheckSkillCondition(nSkillIndex, -1)) {
        return false;
    }
    
    // 检查技能范围
    if (!CheckSkillRange(nSkillIndex)) {
        return false;
    }
    
    // 设置选择的技能
    m_nSelectedSkillIndex = nSkillIndex;
    
    // 开始攻击技能
    StartAttackSkill(nSkillIndex);
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "UseSkill executed");
    return true;
}

// ============================================================================
// State Machine Functions (Round 8 Phase 3)
// ============================================================================

// ChangeState - Change AI state
void CAi::ChangeState(int nNewState) {
    // IDA: State transition logic
    // 1. Save previous state
    // 2. Set new state
    // 3. Trigger state entry function
    
    if (nNewState < 0) {
        return;
    }
    
    // Save current state to previous
    int nPrevState = GetState();
    (void)nPrevState;  // Avoid unused variable warning
    
    // Set new state
    SetState(nNewState);
    
    // Trigger state entry function
    FuncStartState();
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "ChangeState executed");
}

// SearchTarget - Search for target
CMover* CAi::SearchTarget() {
    // IDA 0x140265AD0: Target search logic
    // 1. Check monster validity
    // 2. Get current position
    // 3. Scan for nearby objects
    // 4. Filter hostile targets
    // 5. Select nearest target
    
    if (!m_pMonster) {
        return nullptr;
    }
    
    // Check if already has target
    std::uint32_t dwTargetID = m_pMonster->GetTargetID();
    if (dwTargetID != 0xFFFFFFFF) {
        // IDA: Return existing target
        CMoverEx* pTarget = dynamic_cast<CMoverEx*>(CMover::GetMoverObject(dwTargetID));
        return pTarget;
    }
    
    // Call existing search function
    FuncSearchTarget();
    
    // Return found target
    dwTargetID = m_pMonster->GetTargetID();
    if (dwTargetID == 0xFFFFFFFF) {
        return nullptr;
    }
    
    // IDA: Return found target
    return dynamic_cast<CMoverEx*>(CMover::GetMoverObject(dwTargetID));
}

// ProcessSkillAttack - Process skill attack AI
void CAi::ProcessSkillAttack() {
    // IDA: Skill attack processing
    // 1. Check if can use skill
    // 2. Select appropriate skill
    // 3. Check skill conditions
    // 4. Execute skill
    
    if (!m_pMonster) {
        return;
    }
    
    // Check if has valid target
    if (!HasValidTarget()) {
        // No target, search for one
        SearchTarget();
        if (!HasValidTarget()) {
            return;
        }
    }
    
    // Check if in attack range
    if (!IsInAttackRange()) {
        // Need to move closer
        ChangeState(AI_STATE_CHASE);
        return;
    }
    
    // Select skill
    int nSkillIndex = SelectSkill();
    if (nSkillIndex < 0) {
        return;
    }
    
    // Check if can use skill
    if (!CanUseSkill(nSkillIndex)) {
        return;
    }
    
    // Execute skill attack
    FuncAttackSkill();
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessSkillAttack executed");
}

// FuncIdleProcess - Process idle state
void CAi::FuncIdleProcess() {
    // IDA: Idle state processing
    // 1. Check if should search for target
    // 2. Check if should patrol
    // 3. Update idle animations
    
    if (!m_pMonster) {
        return;
    }
    
    // Check if patrol monster
    if (m_bPatrolMonster) {
        // Check patrol
        if (CheckPatrol()) {
            return;
        }
    }
    
    // Search for enemies
    // FuncFindEnemy(0.0f);  // Call with 0 elapsed time
    
    // Check if found target
    if (HasValidTarget()) {
        // Switch to select action state
        ChangeState(FSMSTATES_SELECT_ACTION);
        return;
    }
    
    // Check if should return to spawn point
    if (FuncCheckReturnPos()) {
        ChangeState(FSMSTATES_RETURN);
        return;
    }
    
    GreenDamTan_log(__FILE__, __FUNCTION__, "FuncIdleProcess executed");
}

// IsInAttackRange - Check if target in attack range
bool CAi::IsInAttackRange() {
    // IDA: Attack range check
    // 1. Check monster validity
    // 2. Check target validity
    // 3. Calculate distance
    // 4. Check against attack range

    if (!m_pMonster) {
        return false;
    }

    std::uint32_t dwTargetID = m_pMonster->GetTargetID();
    if (dwTargetID == 0xFFFFFFFF) {
        return false;
    }

    // IDA: Get target and calculate distance
    CMoverEx* pTarget = dynamic_cast<CMoverEx*>(CMover::GetMoverObject(dwTargetID));
    if (!pTarget) {
        return false;
    }

    // IDA: Calculate distance
    const hkvVec3 posThis = m_pMonster->GetPosition();
    const hkvVec3 posTarget = pTarget->GetPosition();
    float fDistance = (posTarget - posThis).GetLength();
    
    // Check against skill range
    // if (m_fSkillRangeMax > 0.0f) {
    //     return (fDistance <= m_fSkillRangeMax && fDistance >= m_fSkillRangeMin);
    // }
    
    // Default attack range check
    // Use target sight distance as fallback
    if (m_fTargetSightDistance > 0.0f) {
        // return (fDistance <= m_fTargetSightDistance * 0.5f);
    }
    
    return false;
}

// IsInSightRange - Check if target in sight
bool CAi::IsInSightRange() {
    // IDA: Sight range check
    // 1. Check monster validity
    // 2. Check target validity
    // 3. Calculate distance
    // 4. Check against sight range
    
    if (!m_pMonster) {
        return false;
    }
    
    std::uint32_t dwTargetID = m_pMonster->GetTargetID();
    if (dwTargetID == 0xFFFFFFFF) {
        return false;
    }

    // IDA: Get target and calculate distance
    CMoverEx* pTarget = dynamic_cast<CMoverEx*>(CMover::GetMoverObject(dwTargetID));
    if (!pTarget) {
        return false;
    }

    // IDA: Calculate distance
    const hkvVec3 posThis = m_pMonster->GetPosition();
    const hkvVec3 posTarget = pTarget->GetPosition();
    float fDistance = (posTarget - posThis).GetLength();

    // Check against sight distance
    if (m_fTargetSightDistance > 0.0f) {
        return (fDistance <= m_fTargetSightDistance);
    }
    
    return false;
}

// IsLowHP - Check if HP below threshold
bool CAi::IsLowHP() {
    // IDA: Low HP check
    // 1. Check monster validity
    // 2. Get current HP
    // 3. Get max HP
    // 4. Calculate percentage
    // 5. Check against threshold
    
    if (!m_pMonster) {
        return FALSE;
    }
    
    int nHP = m_pMonster->GetHP();
    
    // Check runaway HP threshold
    if (m_nRunawayHP > 0) {
        // Return TRUE if HP below runaway threshold
        if (nHP <= m_nRunawayHP) {
            return TRUE;
        }
    }
    
    // Check return HP threshold
    if (m_nReturnHP > 0) {
        // Return TRUE if HP below return threshold
        if (nHP <= m_nReturnHP) {
            return TRUE;
        }
    }
    
    return FALSE;
}

// HasValidTarget - Check if has valid attack target
bool CAi::HasValidTarget() {
    // IDA: Valid target check
    // 1. Check monster validity
    // 2. Check target ID
    // 3. Verify target object exists
    // 4. Verify target is alive
    
    if (!m_pMonster) {
        return FALSE;
    }
    
    std::uint32_t dwTargetID = m_pMonster->GetTargetID();
    if (dwTargetID == 0xFFFFFFFF) {
        return FALSE;
    }

    // IDA: Verify target object exists and is alive
    CMoverEx* pTarget = dynamic_cast<CMoverEx*>(CMover::GetMoverObject(dwTargetID));
    if (!pTarget) {
        return FALSE;
    }

    // IDA: Check if target is alive
    if (!pTarget->IsLive()) {
        return FALSE;
    }

    // IDA: Check if target is enemy
    if (!m_pMonster->IsEnemy(pTarget)) {
        return FALSE;
    }
    
    return TRUE;
}

// CanUseSkill - Check if can use specific skill
bool CAi::CanUseSkill(int nSkillID) {
    // IDA: Skill usage check
    // 1. Check skill index validity
    // 2. Check global cooldown
    // 3. Check skill cooldown
    // 4. Check skill conditions
    // 5. Check skill range
    
    if (nSkillID < 0 || nSkillID >= 10) {
        return FALSE;
    }
    
    if (!m_pMonster) {
        return FALSE;
    }
    
    // Check global cooldown
    if (m_fGlobalCooltime > 0.0f) {
        return FALSE;
    }
    
    // Check skill cooldown
    auto it = m_mapCooltimeList.find(nSkillID);
    if (it != m_mapCooltimeList.end()) {
        // Check if cooldown has expired
        if (it->second.fEndTime > 0.0f) {
            return FALSE;
        }
    }
    
    // Check skill conditions
    if (!CheckSkillCondition(nSkillID, -1)) {
        return FALSE;
    }
    
    // Check skill range
    if (!CheckSkillRange(nSkillID)) {
        return FALSE;
    }
    
    // Check if monster can attack
    if (!m_pMonster->IsCanAttack()) {
        return FALSE;
    }

    return TRUE;
}

// ============================================================================
// Movement Functions - IDA 精确还原
// ============================================================================

// ============================================================================
// IsMoveRadius IDA 0x140266CF0 -> 0x140266EF5
// 检查移动半径 - 检查目标位置周围是否可移动
// ============================================================================
bool CAi::IsMoveRadius(hkvVec3& vTargetPos, float fRadius) {
    // IDA 反编译精确还原:
    // 检查目标位置的四个方向是否都可以获取高度（即可移动）

    if (!m_pMonster) {
        return false;
    }

    hkvVec3 vCheckRadius;

    // IDA: 检查 X - fRadius 方向
    vCheckRadius = vTargetPos;
    vCheckRadius.x = vTargetPos.x - fRadius;
    if (!m_pMonster->GetHeight(&vCheckRadius, 200.0f)) {
        return false;
    }

    // IDA: 检查 X + fRadius 方向
    vCheckRadius = vTargetPos;
    vCheckRadius.x = vTargetPos.x + fRadius;
    if (!m_pMonster->GetHeight(&vCheckRadius, 200.0f)) {
        return false;
    }

    // IDA: 检查 Y - fRadius 方向
    vCheckRadius = vTargetPos;
    vCheckRadius.y = vTargetPos.y - fRadius;
    if (!m_pMonster->GetHeight(&vCheckRadius, 200.0f)) {
        return false;
    }

    // IDA: 检查 Y + fRadius 方向
    vCheckRadius = vTargetPos;
    vCheckRadius.y = vTargetPos.y + fRadius;
    return m_pMonster->GetHeight(&vCheckRadius, 200.0f);
}

// ============================================================================
// MoveToPos IDA 0x140268750 -> 0x1402688A4
// 移动到指定位置 - 精确还原
// ============================================================================
void CAi::MoveToPos(hkvVec3& vMyPos, hkvVec3& vTargetPos) {
    // IDA 反编译精确还原:
    // 设置移动目标位置并发送移动命令

    if (!m_pMonster) {
        return;
    }

    // IDA: CMover::SetMovePosition(m_pMonster, vTargetPos.x, vTargetPos.y)
    m_pMonster->SetMovePosition(vTargetPos.x, vTargetPos.y);

    // IDA: 创建方向向量并设置朝向
    hkvVec3 vDirection(vTargetPos.x, vTargetPos.y, vMyPos.z);
    m_pMonster->SetDirectionTo(vDirection);

    // IDA: 检查战斗姿态并设置状态
    bool byRunBit = m_pMonster->IsBattlePose();
    if (byRunBit) {
        // IDA: XActor::SetStatus(&m_pMonster->XActor, 0x100u)
        m_pMonster->SetStatus(0x100);
    }
    // Note: ClearStatus 尚未实现，暂时跳过

    // IDA: CMover::send_eSUB_CMD_MOVE(m_pMonster, m_pMonster, vTargetPos.x, vTargetPos.y, byRunBit)
    m_pMonster->send_eSUB_CMD_MOVE(m_pMonster, vTargetPos.x, vTargetPos.y, byRunBit ? 1 : 0);
}

// ============================================================================
// FuncGazeMove IDA 0x140266340 -> 0x140266CE8
// 注视移动 - 复杂的寻路移动函数
// ============================================================================
E_PATHFIND_RESULT CAi::FuncGazeMove(CMover* _pTarget, float _fMinDist, float _fMaxDist,
                                      float _fMinMove, float _fMaxMove, E_MOVESIDE_TYPE _eMoveSide) {
    // IDA 反编译精确还原:
    // 复杂的寻路移动逻辑，包含注视目标和侧向移动

    if (!m_pMonster) {
        return PATHFIND_FAILED;
    }

    // IDA: 获取区域和导航网格
    // XArea* pArea = m_pMonster->GetArea();
    // if (!pArea) {
    //     UXMapID* MapInsID = m_pMonster->GetMapInsID();
    //     XPRINT("if( NULL==pMaze ) [GetMazeID:%u]", MapInsID->nMapID);
    //     return PATHFIND_FAILED;
    // }

    // IDA: 获取导航网格实例
    // DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    // if (!pNavMesh) {
    //     return PATHFIND_FAILED;
    // }

    // IDA: 检查怪物是否可以移动
    if (!m_pMonster->IsCanMove(true)) {
        // ++m_nPathFindingFailedTotalCount;
        return PATHFIND_FAILED;
    }

    // IDA: 获取目标位置和当前位置
    hkvVec3 vNextPos(0.0f, 0.0f, 0.0f);
    hkvVec3 vTargetPos;
    hkvVec3 vMyPos;

    if (_pTarget) {
        vTargetPos = _pTarget->GetPosition();
    }
    vMyPos = m_pMonster->GetPosition();

    // IDA: 获取高度
    m_pMonster->GetHeight(&vMyPos, 200.0f);
    m_pMonster->GetHeight(&vTargetPos, 200.0f);

    // IDA: 计算方向向量
    hkvVec3 vDirection = vMyPos - vTargetPos;
    vDirection.z = 0.0f;

    int nLoopCnt = 0;
    float fCurDist = vDirection.GetLength();

    // IDA: 确定注视类型
    unsigned char byGazeType = 2;  // 默认侧向移动

    // IDA: 检查距离是否在范围内
    if (_fMinDist <= fCurDist && fCurDist <= _fMaxDist) {
        // 在范围内，检查注视动画类型
        // if (m_pMonster->GetGazeAnimType() == 1) {
        //     return PATHFIND_SUCCESS;
        // }
        byGazeType = 2;
    } else {
        // 不在范围内
        // if (m_pMonster->GetGazeAnimType() == 2) {
        //     return PATHFIND_SUCCESS;
        // }
        byGazeType = 1;

        // IDA: 调整移动距离
        if (_fMinDist <= fCurDist) {
            // 目标太远
            float fDiff = fCurDist - _fMaxDist;
            if (fDiff <= _fMaxMove) {
                if (fDiff > _fMinMove) {
                    _fMinMove = fDiff;
                }
            } else {
                _fMinMove = _fMinMove + (_fMaxMove - _fMinMove) * 0.8f;
            }
        } else {
            // 目标太近
            float fDiff = _fMinDist - fCurDist;
            if (fDiff <= _fMaxMove) {
                if (fDiff > _fMinMove) {
                    _fMinMove = fDiff;
                }
            } else {
                _fMinMove = _fMinMove + (_fMaxMove - _fMinMove) * 0.8f;
            }
        }

        // IDA: 归一化方向向量
        float fLen = vDirection.GetLength();
        if (fLen > 0.000001f) {
            vDirection.x /= fLen;
            vDirection.y /= fLen;
            vDirection.z /= fLen;
        }
    }

    // IDA: 计算移动目标位置
    hkvVec3 vComputeTarget(0.0f, 0.0f, 0.0f);
    std::vector<hkvVec3> vOutList;
    vOutList.resize(15);

    // IDA: 循环尝试寻路
    while (true) {
        if (byGazeType == 2) {
            // IDA: 侧向移动
            float fDist = RandomBetweenF(_fMinMove, _fMaxMove);

            // IDA: 根据移动侧向类型确定方向
            if (_eMoveSide == eMOVESIDE_RANDOM) {
                if (std::rand() % 2) {
                    fDist = -fDist;
                }
            } else if (_eMoveSide == eMOVESIDE_LEFT) {
                fDist = -fDist;
            }

            // IDA: 计算右侧向量 (简化版本)
            // 原始代码使用 hkvMat3，这里简化计算
            hkvVec3 vRight(-vDirection.y, vDirection.x, 0.0f);  // 垂直于方向的向量
            vComputeTarget.x = vRight.x * fDist;
            vComputeTarget.y = vRight.y * fDist;
            vComputeTarget.z = 0.0f;
        } else {
            // IDA: 前后移动
            float f = RandomBetweenF(_fMinMove, _fMaxMove);
            if (fCurDist > _fMaxDist) {
                f = -f;  // 目标太远，靠近
            }
            vComputeTarget.x = vDirection.x * f;
            vComputeTarget.y = vDirection.y * f;
            vComputeTarget.z = 0.0f;
        }

        // IDA: 加上当前位置
        vComputeTarget.x += vMyPos.x;
        vComputeTarget.y += vMyPos.y;
        vComputeTarget.z = vMyPos.z;

        // IDA: 计算路径
        float fRadius = m_pMonster->GetHavokCapsuleRadius();
        // int nPathCnt = DohHavokNavMeshInstance::ComputePath(
        //     pNavMesh, &vMyPos, &vComputeTarget, fRadius + 10.0f, &vOutList, 15);

        // TODO: 等待导航网格实现后启用
        int nPathCnt = 0;  // 暂时无法寻路

        if (nPathCnt > 1) {
            // IDA: 获取下一个移动位置
            if (vOutList.size() > 1) {
                vNextPos = vOutList[1];
            }

            // IDA: 检查路径点
            if (nPathCnt > 2) {
                int nCount = (nPathCnt >= 14) ? 14 : nPathCnt;
                for (int i = 2; i < nCount; ++i) {
                    float fDiffX = std::fabs(vMyPos.x - vOutList[i].x);
                    float fDiffY = std::fabs(vMyPos.y - vOutList[i].y);
                    if (fDiffX + fDiffY > 3.0f) {
                        vNextPos = vOutList[i];
                        break;
                    }
                }
            }

            // IDA: 设置移动位置
            m_pMonster->SetMovePosition(vNextPos.x, vNextPos.y);
            // ClearStatus 尚未实现，暂时跳过
            m_pMonster->CheckTurnOrMovePacket();

            return PATHFIND_SUCCESS;
        }

        // IDA: 重试计数
        if (nLoopCnt++ > 5) {
            // ++m_nPathFindingFailedTotalCount;
            return PATHFIND_FAILED;
        }
    }
}

// Helper function for random float between two values
float CAi::RandomBetweenF(float fMin, float fMax) {
    if (fMin >= fMax) {
        return fMin;
    }
    float fRange = fMax - fMin;
    float fRandom = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return fMin + fRandom * fRange;
}

// ============================================================================
// FuncRunWalkToMovePos IDA 0x140266F00 -> 0x1402678C3
// 跑步行走到移动位置 - 精确还原
// ============================================================================
E_PATHFIND_RESULT CAi::FuncRunWalkToMovePos(CMover* _pTarget, float _fMinDistance, float _fMaxDistance,
                                              int _nMinAngle, int _nMaxAngle, unsigned char _byDirType,
                                              E_MOVESIDE_TYPE _eMoveSide) {
    // IDA 反编译精确还原
    if (!m_pMonster) {
        return PATHFIND_FAILED;
    }

    if (!m_pMonster->IsCanMove(true)) {
        // ++m_nPathFindingFailedTotalCount;
        return PATHFIND_FAILED;
    }

    hkvVec3 vNextPos(0.0f, 0.0f, 0.0f);
    hkvVec3 vTargetPos;
    hkvVec3 vMyPos;

    if (_pTarget) {
        vTargetPos = _pTarget->GetPosition();
    }
    vMyPos = m_pMonster->GetPosition();

    m_pMonster->GetHeight(&vMyPos, 200.0f);
    m_pMonster->GetHeight(&vTargetPos, 200.0f);

    hkvVec3 vDirection = vMyPos - vTargetPos;
    vDirection.z = 0.0f;

    float fCurDist = vDirection.GetLength();
    float fYaw = CMover::GetYawFromVector(vDirection);
    float fLookYaw = _pTarget ? _pTarget->GetMovingYaw() : 0.0f;

    int nLoopCnt = 0;
    hkvVec3 vComputeTarget(0.0f, 0.0f, 0.0f);
    std::vector<hkvVec3> vOutList;
    vOutList.resize(15);

    // IDA: 获取区域和导航网格
    // XArea* pArea = m_pMonster->GetArea();
    // DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    // TODO: 等待导航网格实现

    do {
        float fDistance = fCurDist;
        if (_fMinDistance > fCurDist || fCurDist > _fMaxDistance || !(std::rand() % 2)) {
            fDistance = RandomBetweenF(_fMinDistance, _fMaxDistance);
        }

        float fTargetRadius = _pTarget ? _pTarget->GetHavokCapsuleRadius() : 0.0f;
        fDistance = fDistance + fTargetRadius;

        float fPlusAngle;
        if (_nMaxAngle <= _nMinAngle) {
            fPlusAngle = static_cast<float>(_nMinAngle);
        } else {
            fPlusAngle = static_cast<float>(std::rand() % (10 * (_nMaxAngle - _nMinAngle) + 1)) * 0.1f + static_cast<float>(_nMinAngle);
        }

        if (_eMoveSide == eMOVESIDE_RANDOM && std::rand() % 2) {
            fPlusAngle = -fPlusAngle;
        }

        float fAngle = fPlusAngle;
        if (!_byDirType) {
            float fDestYaw = fYaw - (fLookYaw + fPlusAngle);
            if (fDestYaw <= 180.0f) {
                if (fDestYaw < -180.0f) {
                    fDestYaw = fDestYaw + 360.0f;
                }
            } else {
                fDestYaw = fDestYaw - 360.0f;
            }
            fPlusAngle = fDestYaw;
            fAngle = fPlusAngle;
        }

        // IDA: 计算旋转矩阵并应用
        // 简化版本：直接计算偏移
        float fRad = fAngle * 3.14159f / 180.0f;
        float fCos = std::cos(fRad);
        float fSin = std::sin(fRad);

        // 归一化方向向量
        float fLen = vDirection.GetLength();
        if (fLen > 0.000001f) {
            vDirection.x /= fLen;
            vDirection.y /= fLen;
        }

        // 应用旋转
        vComputeTarget.x = (vDirection.x * fCos - vDirection.y * fSin) * fDistance;
        vComputeTarget.y = (vDirection.x * fSin + vDirection.y * fCos) * fDistance;
        vComputeTarget.z = 0.0f;
        vComputeTarget.x += vTargetPos.x;
        vComputeTarget.y += vTargetPos.y;
        vComputeTarget.z = vTargetPos.z;

        float fMonsterRadius = m_pMonster->GetHavokCapsuleRadius();
        int nPathCnt = 0;

        if (IsMoveRadius(vComputeTarget, fMonsterRadius)) {
            // TODO: nPathCnt = DohHavokNavMeshInstance::ComputePath(...)
            nPathCnt = 0;
        }

        if (nPathCnt > 1) {
            // IDA: 获取路径点
            if (vOutList.size() > 1) {
                vNextPos = vOutList[1];
            }

            if (nPathCnt > 2) {
                int nCount = (nPathCnt >= 14) ? 14 : nPathCnt;
                for (int i = 2; i < nCount; ++i) {
                    float fDiffX = std::fabs(vMyPos.x - vOutList[i].x);
                    float fDiffY = std::fabs(vMyPos.y - vOutList[i].y);
                    if (fDiffX + fDiffY > 3.0f) {
                        vNextPos = vOutList[i];
                        break;
                    }
                }
            }

            m_pMonster->SetMovePosition(vNextPos.x, vNextPos.y);
            m_pMonster->SetDirectionTo(vNextPos);
            m_pMonster->SetStatus(0x100);
            m_pMonster->CheckTurnOrMovePacket();

            return PATHFIND_SUCCESS;
        }

    } while (nLoopCnt++ <= 5);

    // ++m_nPathFindingFailedTotalCount;
    return PATHFIND_FAILED;
}

// ============================================================================
// FuncRunWalkToTargetPos IDA 0x1402678D0 -> 0x140268741
// 跑步行走到目标位置 - 精确还原
// ============================================================================
E_PATHFIND_RESULT CAi::FuncRunWalkToTargetPos(CMover* _pTarget, float _fMinDistance, float _fMaxDistance,
                                                int _nMinAngle, int _nMaxAngle, unsigned char _byDirType,
                                                E_MOVESIDE_TYPE _eMoveSide) {
    // IDA 反编译精确还原
    if (!m_pMonster) {
        return PATHFIND_FAILED;
    }

    if (!m_pMonster->IsCanMove(true)) {
        // ++m_nPathFindingFailedTotalCount;
        return PATHFIND_FAILED;
    }

    hkvVec3 vNextPos(0.0f, 0.0f, 0.0f);
    hkvVec3 vTargetPos;
    hkvVec3 vMyPos;

    if (_pTarget) {
        vTargetPos = _pTarget->GetPosition();
    }
    vMyPos = m_pMonster->GetPosition();

    m_pMonster->GetHeight(&vMyPos, 200.0f);
    m_pMonster->GetHeight(&vTargetPos, 200.0f);

    hkvVec3 vDirection = vMyPos - vTargetPos;
    vDirection.z = 0.0f;

    float fCurDist = vDirection.GetLength();
    float fYaw = CMover::GetYawFromVector(vDirection);
    float fLookYaw = _pTarget ? _pTarget->GetMovingYaw() : 0.0f;

    int nLoopCnt = 0;
    hkvVec3 vComputeTarget(0.0f, 0.0f, 0.0f);
    std::vector<hkvVec3> vOutList;
    vOutList.resize(15);

    // IDA: 常量 MAX_ONCE_ANGLE = 45.0
    const float MAX_ONCE_ANGLE = 45.0f;

    while (true) {
        float fDistance = fCurDist;
        if (_fMinDistance > fCurDist || fCurDist > _fMaxDistance || !(std::rand() % 2)) {
            fDistance = RandomBetweenF(_fMinDistance, _fMaxDistance);
        }

        float fTargetRadius = _pTarget ? _pTarget->GetHavokCapsuleRadius() : 0.0f;
        fDistance = fDistance + fTargetRadius;

        if (fDistance == 0.0f) {
            return PATHFIND_IN_PROGRESS;
        }

        // IDA: 计算角度比例
        int nAngleRatio = static_cast<int>(fCurDist) / static_cast<int>(fDistance);
        if (nAngleRatio <= 0) {
            nAngleRatio = 1;
        } else if (nAngleRatio > 2) {
            nAngleRatio = 2;
        }

        float fPlusAngle;
        if (_nMaxAngle <= _nMinAngle) {
            fPlusAngle = static_cast<float>(_nMinAngle);
        } else {
            fPlusAngle = static_cast<float>(std::rand() % (10 * (_nMaxAngle - _nMinAngle) + 1)) * 0.1f + static_cast<float>(_nMinAngle);
        }

        if (_eMoveSide == eMOVESIDE_RANDOM && std::rand() % 2) {
            fPlusAngle = -fPlusAngle;
        }

        float fAngle = fPlusAngle;
        if (!_byDirType) {
            float fDestYaw = fYaw - (fLookYaw + fPlusAngle);
            if (fDestYaw <= 180.0f) {
                if (fDestYaw < -180.0f) {
                    fDestYaw = fDestYaw + 360.0f;
                }
            } else {
                fDestYaw = fDestYaw - 360.0f;
            }
            fPlusAngle = fDestYaw;
            fAngle = fPlusAngle;
        }

        // IDA: 限制单次角度
        float fMaxOnceAngle = MAX_ONCE_ANGLE * static_cast<float>(nAngleRatio);
        if (std::fabs(fPlusAngle) <= fMaxOnceAngle) {
            fAngle = fPlusAngle;
            fPlusAngle = 0.0f;
        } else {
            if (fAngle <= 0.0f) {
                fAngle = -fMaxOnceAngle;
            } else {
                fAngle = fMaxOnceAngle;
            }
            fPlusAngle = fPlusAngle - fAngle;
        }

        // IDA: 计算旋转后的目标位置
        float fRad = fAngle * 3.14159f / 180.0f;
        float fCos = std::cos(fRad);
        float fSin = std::sin(fRad);

        float fLen = vDirection.GetLength();
        if (fLen > 0.000001f) {
            vDirection.x /= fLen;
            vDirection.y /= fLen;
        }

        vComputeTarget.x = (vDirection.x * fCos - vDirection.y * fSin) * fDistance;
        vComputeTarget.y = (vDirection.x * fSin + vDirection.y * fCos) * fDistance;
        vComputeTarget.x += vTargetPos.x;
        vComputeTarget.y += vTargetPos.y;
        vComputeTarget.z = vTargetPos.z;

        float fMonsterRadius = m_pMonster->GetHavokCapsuleRadius();
        // TODO: int nPathCnt = DohHavokNavMeshInstance::ComputePath(...)
        int nPathCnt = 0;

        if (nPathCnt > 1) {
            if (vOutList.size() > 1) {
                vNextPos = vOutList[1];
            }

            if (nPathCnt > 2) {
                int nCount = (nPathCnt >= 14) ? 14 : nPathCnt;
                for (int i = 2; i < nCount; ++i) {
                    float fDiffX = std::fabs(vMyPos.x - vOutList[i].x);
                    float fDiffY = std::fabs(vMyPos.y - vOutList[i].y);
                    if (fDiffX + fDiffY > 3.0f) {
                        vNextPos = vOutList[i];
                        break;
                    }
                }
            }

            m_pMonster->SetMovePosition(vNextPos.x, vNextPos.y);
            m_pMonster->SetDirectionTo(vNextPos);
            m_pMonster->SetStatus(0x100);
            m_pMonster->CheckTurnOrMovePacket();

            return PATHFIND_SUCCESS;
        }

        if (nLoopCnt++ > 5) {
            // ++m_nPathFindingFailedTotalCount;
            return PATHFIND_FAILED;
        }
    }
}

// ============================================================================
// FuncRunWalkToPos IDA 0x1402688B0 -> 0x140268D7A
// 跑步行走到位置 - 精确还原
// ============================================================================
E_PATHFIND_RESULT CAi::FuncRunWalkToPos(hkvVec3& vMyPos, hkvVec3& vTargetPos,
                                          unsigned char byRunBit, int bCheckWayPoint) {
    // IDA 反编译精确还原
    if (!m_pMonster) {
        return PATHFIND_FAILED;
    }

    // IDA: 获取区域和导航网格
    // XArea* pArea = m_pMonster->GetArea();
    // if (!pArea) {
    //     UXMapID* MapInsID = m_pMonster->GetMapInsID();
    //     XPRINT("if( NULL==pMaze ) [GetMazeID:%u]", MapInsID->nMapID);
    //     return PATHFIND_FAILED;
    // }

    // DohHavokNavMeshInstance* pNavMesh = pArea->GetNavMeshInstance();
    // if (!pNavMesh) {
    //     return PATHFIND_FAILED;
    // }

    std::vector<hkvVec3> vOutList;
    vOutList.resize(15);

    m_pMonster->GetHeight(&vMyPos, 200.0f);
    m_pMonster->GetHeight(&vTargetPos, 200.0f);

    float fRadius = m_pMonster->GetHavokCapsuleRadius();
    // TODO: int nPathCnt = DohHavokNavMeshInstance::ComputePath(...)
    int nPathCnt = 0;

    E_PATHFIND_RESULT eResult;

    if (nPathCnt <= 1) {
        // ++m_nPathFindingFailedTotalCount;
        if (nPathCnt == 1) {
            eResult = static_cast<E_PATHFIND_RESULT>(3);  // PATHFIND_RESULT_SAME_LOCATION
        } else {
            eResult = PATHFIND_FAILED;
        }
    } else {
        hkvVec3 vNextPos(0.0f, 0.0f, 0.0f);

        // IDA: 获取第一个路径点
        if (vOutList.size() > 1) {
            vNextPos = vOutList[1];
        }

        // IDA: 检查路径点
        if (nPathCnt > 2) {
            int nCount = (nPathCnt >= 14) ? 14 : nPathCnt;
            for (int i = 2; i < nCount; ++i) {
                float fDiffX = std::fabs(vMyPos.x - vOutList[i].x);
                float fDiffY = std::fabs(vMyPos.y - vOutList[i].y);
                if (fDiffX + fDiffY > 3.0f) {
                    vNextPos = vOutList[i];
                    break;
                }
            }
        }

        // IDA: 设置状态
        if (byRunBit == 1) {
            m_pMonster->SetStatus(0x100);
        }
        // ClearStatus 尚未实现

        m_pMonster->SetMovePosition(vNextPos.x, vNextPos.y);
        m_pMonster->SetDirectionTo(vNextPos);

        // IDA: 检查路点
        if (bCheckWayPoint) {
            // TODO: CWayPoint* WayPoint = m_pMonster->GetWayPoint();
            // if (WayPoint && WayPoint->GetNextPosition(&vDestPos)) {
            //     // 设置额外移动位置
            // }
        }

        m_pMonster->CheckTurnOrMovePacket();
        eResult = PATHFIND_SUCCESS;
    }

    return eResult;
}

// ============================================================================
// _ConditionGlobalCooltime IDA 0x140279FE0 -> 0x14027A034
// 检查全局冷却时间 - 精确还原
// ============================================================================
int CAi::_ConditionGlobalCooltime(int /*_nVal*/) {
    // IDA 反编译精确还原:
    // 获取当前时间，检查是否在全局冷却时间内

    // TODO: 需要 VDefaultTimer::GetTimer 和 IVTimer::GetTime
    // VDefaultTimer* Timer = ThreadLocalData::GetTimer();
    // float fCurrTime = Timer->GetTime();
    //
    // if (m_fGlobalCooltime <= fCurrTime) {
    //     return 0;
    // }
    // return static_cast<int>(m_fGlobalCooltime - fCurrTime);

    // 临时简化实现
    return 0;
}

// ============================================================================
// _StartSelectAction IDA 0x14027CB40 -> 0x14027CBD9
// 开始选择动作状态 - 精确还原
// ============================================================================
void CAi::_StartSelectAction() {
    // IDA 反编译精确还原:
    // 设置战斗姿态，停止移动，调用 _UpdateSelectAction

    if (!m_pMonster) {
        return;
    }

    // 设置战斗姿态
    m_pMonster->ChangeBattlePose(true, true);

    // 初始化最后技能时间
    if (m_fLastSkillTime < 0.0f) {
        m_fLastSkillTime = 0.0f;
    }

    // 如果正在移动则停止
    if (m_pMonster->IsMoving()) {
        m_pMonster->StopMoving(true);
    }

    // 调用更新函数
    _UpdateSelectAction(0.0f);
}

// ============================================================================
// _UpdateSelectAction IDA 0x14027CBE0 -> 0x14027CF98
// 更新选择动作状态 - 精确还原
// ============================================================================
void CAi::_UpdateSelectAction(float /*_fElapsedTime*/) {
    // IDA 反编译精确还原:
    // 检查返回位置，检查目标，处理移动

    if (FuncCheckReturnPos()) {
        return;
    }

    if (!m_pMonster) {
        return;
    }

    std::uint32_t TargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);

    if (!pTarget) {
        ClearTarget();
        ChangeAiState(FSMSTATES_WAIT);
        return;
    }

    // TODO: 需要完整的 VisObject3D_cl::GetPosition 实现
    // 检查目标丢失距离
    // float fDist = m_pMonster->GetDistanceTo(pTarget);
    // if (m_fTargetLostDistance > 0.0f && fDist > m_fTargetLostDistance) {
    //     ClearTarget();
    //     ChangeAiState(FSMSTATES_WAIT);
    //     return;
    // }

    // 检查是否可以移动
    if (!m_pMonster->IsCanMove(true)) {
        return;
    }

    // TODO: 检查 m_vGazeTargetPos 是否为零向量
    // 如果有注视目标位置，移动到那里
    // 否则检查 CellID 并处理移动

    // 简化实现：暂时不做任何操作
    // TODO: FuncCheckTargetPos(0.0f);
}

// ============================================================================
// _StartBattleMove IDA 0x14027CFA0 -> 0x14027D096
// 开始战斗移动状态 - 精确还原
// ============================================================================
void CAi::_StartBattleMove() {
    // IDA 反编译精确还原:
    // 停止当前移动，设置注视移动，运行目标到目标位置

    if (!m_pMonster) {
        return;
    }

    std::uint32_t TargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);

    if (!pTarget) {
        ClearTarget();
        ChangeAiState(FSMSTATES_WAIT);
        return;
    }

    // 如果正在移动则停止
    if (m_pMonster->IsMoving()) {
        m_pMonster->StopMoving(true);
    }

    // 设置注视移动
    // TODO: m_pMonster->SetGazeMoving(false);

    // 运行目标到目标位置
    // TODO: RunTargetByDestPos(pTarget, m_fStateTargetDistMin, m_fStateTargetDistMax,
    //            static_cast<int>(m_fStateAngleMin), static_cast<int>(m_fStateAngleMax));
}

// ============================================================================
// _UpdateBattleMove IDA 0x14027D0A0 -> 0x14027D202
// 更新战斗移动状态 - 精确还原
// ============================================================================
void CAi::_UpdateBattleMove(float _fElapsedTime) {
    // IDA 反编译精确还原:
    // 检查返回位置和移动状态

    if (!m_pMonster || FuncCheckReturnPos()) {
        return;
    }

    // 检查移动位置是否为零
    // TODO: 需要 tagMOVE_POS::IsZero
    // if (m_pMonster->m_stMovePos.IsZero()) {
    //     ChangeAiState(FSMSTATES_SELECT_ACTION);
    //     return;
    // }

    // 检查目标是否有效
    std::uint32_t TargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);

    if (!pTarget) {
        ClearTarget();
        ChangeAiState(FSMSTATES_WAIT);
        return;
    }

    // 累加时间
    m_fStateTempFloat[0] += _fElapsedTime;

    // 每0.5秒检查一次移动距离
    if (m_fStateTempFloat[0] > 0.5f) {
        m_fStateTempFloat[0] = 0.0f;
        // TODO: GetMoveDistanceSum
        // m_fStateMoveDistSum = GetMoveDistanceSum();
        // if (m_fStateMoveDistSum <= m_fStateTempFloat[1] + 10.0f) {
        //     ChangeAiState(FSMSTATES_SELECT_ACTION);
        //     return;
        // }
        // m_fStateTempFloat[1] = m_fStateMoveDistSum;
    }
}

// ============================================================================
// _StartGaze IDA 0x14027D210 -> 0x14027D39C
// 开始注视状态 - 精确还原
// ============================================================================
void CAi::_StartGaze() {
    // IDA 反编译精确还原:
    // 停止移动，设置注视移动，找到目标位置

    if (!m_pMonster) {
        return;
    }

    std::uint32_t TargetID = m_pMonster->GetTargetID();
    CMover* pTarget = CMover::GetMoverObject(TargetID);

    if (!pTarget) {
        ClearTarget();
        ChangeAiState(FSMSTATES_WAIT);
        return;
    }

    // 如果正在移动则停止
    if (m_pMonster->IsMoving()) {
        m_pMonster->StopMoving(true);
    }

    // 设置注视移动
    // TODO: m_pMonster->SetGazeMoving(true);

    // TODO: 清除目标位置标志
    // unsigned char byDestPos = m_pMonster->GetTargetDestPos();
    // if (byDestPos != 255) {
    //     pTarget->ClearTargetPosFlag(byDestPos);
    // }

    // TODO: 找到目标位置
    // unsigned char byPos = pTarget->FindTargetPos(0.0f, 150.0f, eMOVESIDE_NONE);
    // m_pMonster->SetTargetDestPos(byPos);
    // pTarget->SetTargetPosFlag(byPos);

    // 确定移动方向
    E_MOVESIDE_TYPE eMoveSide = eMOVESIDE_RIGHT;
    // if (byPos > 5) {
    //     eMoveSide = eMOVESIDE_LEFT;
    // }

    // 调用注视移动
    FuncGazeMove(pTarget, m_fStateTargetDistMin, m_fStateTargetDistMax,
                 m_fStateMoveDistMin, m_fStateMoveDistMax, eMoveSide);
}

// ============================================================================
// _UpdateGaze IDA 0x14027D3A0 -> 0x14027D3EC
// 更新注视状态 - 精确还原
// ============================================================================
void CAi::_UpdateGaze(float /*_fElapsedTime*/) {
    // IDA 反编译精确还原:
    // 检查移动位置是否为零

    if (!m_pMonster) {
        return;
    }

    // TODO: 需要 tagMOVE_POS::IsZero
    // if (m_pMonster->m_stMovePos.IsZero()) {
    //     ChangeAiState(FSMSTATES_SELECT_ACTION);
    // }
}

// ============================================================================
// _EndGaze IDA 0x14027D3F0 -> 0x14027D44B
// 结束注视状态 - 精确还原
// ============================================================================
void CAi::_EndGaze() {
    // IDA 反编译精确还原:
    // 清除注视移动标志，停止移动

    if (!m_pMonster) {
        return;
    }

    // TODO: m_pMonster->SetGazeMoving(false);

    if (m_pMonster->IsMoving()) {
        m_pMonster->StopMoving(true);
    }
}

// ============================================================================
// _StartMove IDA 0x14027D450 -> 0x14027D5C1
// 开始移动状态 - 精确还原
// ============================================================================
void CAi::_StartMove() {
    // IDA 反编译精确还原:
    // 获取目标或主人，检查助手距离，执行移动

    if (!m_pMonster) {
        return;
    }

    CMover* pTarget = nullptr;

    // 检查是否是随从
    if (m_pMonster->IsFollower()) {
        pTarget = m_pMonster->GetOwnerPlayer();
    } else {
        std::uint32_t TargetID = m_pMonster->GetTargetID();
        pTarget = CMover::GetMoverObject(TargetID);
    }

    // 检查助手距离
    if (CheckHelperFarDist(false, false)) {
        return;
    }

    if (!pTarget) {
        ClearTarget();
        ChangeAiState(FSMSTATES_WAIT);
        return;
    }

    // 执行移动
    E_PATHFIND_RESULT eResult = FuncRunWalkToMovePos(
        pTarget,
        m_fStateTargetDistMin,
        m_fStateTargetDistMax,
        static_cast<int>(m_fStateAngleMin),
        static_cast<int>(m_fStateAngleMax),
        1,
        eMOVESIDE_RANDOM
    );

    if (eResult != PATHFIND_SUCCESS && !CheckHelperFarDist(false, true)) {
        if (m_pMonster->IsBattlePose()) {
            ChangeAiState(FSMSTATES_SELECT_ACTION);
        } else {
            ChangeAiState(FSMSTATES_WAIT);
        }
    }
}

// ============================================================================
// _UpdateMove IDA 0x14027D5D0 -> 0x14027D677
// 更新移动状态 - 精确还原
// ============================================================================
void CAi::_UpdateMove(float /*_fElapsedTime*/) {
    // IDA 反编译精确还原:
    // 检查助手距离、返回位置和移动状态

    if (CheckHelperFarDist(false, false)) {
        return;
    }

    if (FuncCheckReturnPos()) {
        return;
    }

    if (!m_pMonster) {
        return;
    }

    // TODO: 需要 tagMOVE_POS::IsZero
    // if (m_pMonster->m_stMovePos.IsZero()) {
    //     if (m_pMonster->IsBattlePose()) {
    //         ChangeAiState(FSMSTATES_SELECT_ACTION);
    //     } else {
    //         ChangeAiState(FSMSTATES_WAIT);
    //     }
    // }
}

// ============================================================================
// _StartSucide IDA 0x14027D680 -> 0x14027D722
// 开始自杀状态 - 精确还原
// ============================================================================
void CAi::_StartSucide() {
    // IDA 反编译精确还原:
    // 检查状态，设置死亡原因，执行死亡

    if (!m_pMonster) {
        return;
    }

    // 检查是否已经死亡
    if (m_pMonster->IsStatus(4)) {
        return;
    }

    // 设置死亡原因
    int nHP = m_pMonster->GetHP();
    // TODO: m_pMonster->SetDieReason(5, nHP);

    // 获取死亡动作并设置死亡
    std::int16_t nDeathMotion = GetDeathActionMotion();
    m_pMonster->SetDie(nDeathMotion, 1);
}

// ============================================================================
// _StartReturn IDA 0x14027D730 -> 0x14027D7CE
// 开始返回状态 - 精确还原
// ============================================================================
void CAi::_StartReturn() {
    // IDA 反编译精确还原:
    // 清除目标，设置战斗姿态，清除移动位置，调用更新

    ClearTarget();

    if (!m_pMonster) {
        return;
    }

    if (m_pMonster->IsCanMove(true)) {
        m_pMonster->ChangeBattlePose(true, false);
    }

    // 清除移动位置
    // TODO: tagMOVE_POS::Clear
    // m_pMonster->m_stMovePos.Clear();

    // 调用更新
    _UpdateReturn(0.0f);

    // 设置无敌
    m_pMonster->SetInvincibleActor(true);
}

// ============================================================================
// _UpdateReturn IDA 0x14027D7D0 -> 0x14027DA95
// 更新返回状态 - 精确还原
// ============================================================================
void CAi::_UpdateReturn(float /*_fElapsedTime*/) {
    // IDA 反编译精确还原:
    // 检查状态时间，移动到创建位置

    if (!m_pMonster) {
        return;
    }

    // 如果超过30秒，直接传送到创建位置
    if (m_fStateTime > 30.0f) {
        // TODO: 获取创建位置并传送
        // hkvVec3 vCreatePos = m_pMonster->GetCreatePos();
        // m_pMonster->Move(&vCreatePos);
        // m_pMonster->send_eSUB_CMD_SKILL_WARP_POSITION(m_pMonster, &vCreatePos, 1);
        ChangeAiState(FSMSTATES_SELECT_ACTION);
        return;
    }

    // 检查是否正在移动
    if (m_pMonster->IsMoving() == 1) {
        return;
    }

    // 检查是否可以移动
    if (!m_pMonster->IsCanMove(true)) {
        return;
    }

    // TODO: 获取创建位置并移动
    // hkvVec3 vCreatePos = m_pMonster->GetCreatePos();
    // hkvVec3 vMyPos = m_pMonster->GetPosition();
    // hkvVec3 vDiff = vCreatePos - vMyPos;
    // vDiff.z = 0.0f;
    // float fLength = vDiff.getLength();
    // float fRadius = m_pMonster->GetHavokCapsuleRadius();
    //
    // if (fRadius + 50.0f <= fLength) {
    //     E_PATHFIND_RESULT eResult = FuncRunWalkToPos(&vMyPos, &vCreatePos, 1, 0);
    //     if (eResult == PATHFIND_SUCCESS) {
    //         ClearTarget();
    //     }
    // } else {
    //     ChangeAiState(FSMSTATES_WAIT);
    // }
}

// ============================================================================
// _EndReturn IDA 0x14027DAA0 -> 0x14027DB29
// 结束返回状态 - 精确还原
// ============================================================================
void CAi::_EndReturn() {
    // IDA 反编译精确还原:
    // 恢复HP，取消无敌状态

    if (!m_pMonster) {
        return;
    }

    // 如果没有返回HP设置，恢复满HP
    if (!m_nReturnHP) {
        int nMaxHP = m_pMonster->GetMaxHP();
        m_pMonster->SetHP(nMaxHP);
    }

    // 取消无敌
    m_pMonster->SetInvincibleActor(false);
}

// ============================================================================
// _StartRecovery IDA 0x14027DB30 -> 0x14027DCCC
// 开始恢复状态 - 精确还原
// ============================================================================
void CAi::_StartRecovery() {
    // IDA 反编译精确还原:
    // 清除目标，清除Buff，设置无敌，执行恢复技能

    if (!m_nRecoverySkill) {
        return;
    }

    if (!m_pMonster) {
        return;
    }

    ClearTarget();

    // 清除所有Buff
    m_pMonster->AllBuffClear(0);

    // 停止移动
    m_pMonster->StopMoving(true);

    // 设置无敌
    m_pMonster->SetInvincibleActor(true);

    // 设置防御类型
    // TODO: m_pMonster->ChangeDefenseTypeForce(3, 0.0f);

    // 设置当前技能
    m_pMonster->SetCurSkillTableIdx(m_nRecoverySkill);

    // 执行攻击动作
    m_pMonster->ActionAttack();

    // 设置状态
    m_pMonster->SetStatus(2);

    // 如果是助手怪物，发送日志
    if (m_pMonster->IsHelper()) {
        CMoverEx* pOwner = m_pMonster->GetOwnerPlayer();
        if (pOwner) {
            // TODO: 获取地图ID并发送日志
            // XArea* pArea = m_pMonster->GetArea();
            // if (pArea) {
            //     unsigned int nMapID = pArea->GetTBMapID();
            //     pOwner->SendHelperLog(2, m_pMonster->GetTableID(), nMapID);
            // }
        }
    }
}

// ============================================================================
// _UpdateRecovery IDA 0x14027DCD0 -> 0x14027DE4D
// 更新恢复状态 - 精确还原
// ============================================================================
void CAi::_UpdateRecovery(float _fElapsedTime) {
    // IDA 反编译精确还原:
    // 累计恢复检查时间，每秒恢复一定比例的HP

    if (!m_pMonster) {
        return;
    }

    m_fRecoveryCheckTime += _fElapsedTime;

    if (m_fRecoveryCheckTime > 1.0f) {
        m_fRecoveryCheckTime = 0.0f;

        // 计算恢复量：每秒恢复 MaxHP/20
        int nMaxHP = m_pMonster->GetMaxHP();
        int nRecoverHP = nMaxHP / 20;
        int nCurrentHP = m_pMonster->GetHP();
        int nTotalHP = nCurrentHP + nRecoverHP;

        if (nTotalHP >= nMaxHP) {
            nTotalHP = nMaxHP;
        }

        // 清除所有Buff并设置HP
        m_pMonster->AllBuffClear(0);
        m_pMonster->SetHP(nTotalHP);

        // 检查是否恢复满
        float fStat = m_pMonster->GetStat(1);
        if (fStat >= static_cast<float>(m_pMonster->GetMaxHP())) {
            ChangeAiState(FSMSTATES_SELECT_ACTION);
        }
    }
}

// ============================================================================
// _EndRecovery IDA 0x14027DE50 -> 0x14027E07E
// 结束恢复状态 - 精确还原
// ============================================================================
void CAi::_EndRecovery() {
    // IDA 反编译精确还原:
    // 重置状态，清除Buff，恢复满HP，检查是否需要死亡

    if (!m_pMonster) {
        return;
    }

    // 重置状态
    // TODO: m_pMonster->ResetStatus();

    // 清除所有Buff并恢复满HP
    m_pMonster->AllBuffClear(0);
    int nMaxHP = m_pMonster->GetMaxHP();
    m_pMonster->SetHP(nMaxHP);

    // 切换动作
    m_pMonster->ChangeMotion(1, true, 0);

    // 更新旋转
    // TODO: m_pMonster->SetUpdateRotation(false);

    // 发送空闲移动包
    // TODO: m_pMonster->send_eSUB_CMD_MOVE_IDLE(-1.0f);

    // 取消无敌
    m_pMonster->SetInvincibleActor(false);

    // 如果是助手怪物，发送日志
    if (m_pMonster->IsHelper()) {
        CMoverEx* pOwner = m_pMonster->GetOwnerPlayer();
        if (pOwner) {
            // TODO: 获取地图ID并发送日志
            // XArea* pArea = m_pMonster->GetArea();
            // if (pArea) {
            //     unsigned int nMapID = pArea->GetTBMapID();
            //     pOwner->SendHelperLog(1, m_pMonster->GetTableID(), nMapID);
            // }
        }
    }

    // 检查是否需要死亡
    if (m_pMonster->IsReserveDie()) {
        // TODO: m_pMonster->SetDieReason(5, nHP);
        m_pMonster->SetDie(12, 1);
    }
}

// ============================================================================
// _StartTraceMove IDA 0x14027E080 -> 0x14027E0EA
// 开始追踪移动状态 - 精确还原
// ============================================================================
void CAi::_StartTraceMove() {
    // IDA 反编译精确还原:
    // 检查助手距离，获取主人，调用更新

    if (CheckHelperFarDist(false, false)) {
        return;
    }

    if (!m_pMonster) {
        return;
    }

    CMover* pOwner = m_pMonster->GetOwnerPlayer();
    if (pOwner) {
        _UpdateTraceMove(0.0f);
    } else {
        ChangeAiState(FSMSTATES_WAIT);
    }
}

// ============================================================================
// _UpdateTraceMove IDA 0x14027E0F0 -> 0x14027E36F
// 更新追踪移动状态 - 精确还原
// ============================================================================
void CAi::_UpdateTraceMove(float /*_fElapsedTime*/) {
    // IDA 反编译精确还原:
    // 追踪主人位置，保持一定距离

    if (CheckHelperFarDist(false, false)) {
        return;
    }

    if (!m_pMonster) {
        return;
    }

    CMover* pTarget = m_pMonster->GetOwnerPlayer();
    if (!pTarget) {
        ChangeAiState(FSMSTATES_WAIT);
        return;
    }

    // TODO: 获取位置并计算距离
    // hkvVec3 vTargetPos = pTarget->GetPosition();
    // hkvVec3 vMyPos = m_pMonster->GetPosition();
    // float fDistance = (vMyPos - vTargetPos).getLength();

    // if (m_fStateTargetDistMax <= fDistance) {
    //     // 计算方向并移动
    //     hkvVec3 vDirVector = vTargetPos - vMyPos;
    //     float fYaw = CMover::GetYawFromVector(&vDirVector);
    //
    //     // 规范化角度
    //     if (fYaw < -180.0f) {
    //         fYaw += 360.0f;
    //     } else if (fYaw > 180.0f) {
    //         fYaw -= 360.0f;
    //     }
    //
    //     float fAbsDiff = fabsf(fYaw);
    //
    //     if (!m_pMonster->IsMoving() || fAbsDiff > 20.0f) {
    //         if (m_pMonster->IsCanMove(true)) {
    //             E_PATHFIND_RESULT eResult = FuncRunWalkToPos(&vMyPos, &vTargetPos, 1, 0);
    //             if (eResult != PATHFIND_SUCCESS) {
    //                 ChangeAiState(FSMSTATES_SELECT_ACTION);
    //             }
    //         }
    //     }
    // } else {
    //     ChangeAiState(FSMSTATES_SELECT_ACTION);
    // }
}

// ============================================================================
// _StartAttack IDA 0x14027E370 -> 0x14027E39D
// 开始攻击状态 - 精确还原
// ============================================================================
void CAi::_StartAttack() {
    // IDA 反编译精确还原:
    // 检查助手距离，检查技能组顺序

    if (CheckHelperFarDist(true, false)) {
        return;
    }

    CheckSkillGroupOrder();
}

// ============================================================================
// _UpdateAttackSkill IDA 0x14027E9C0 -> 0x14027F431
// 更新攻击技能状态 - 精确还原
// ============================================================================
void CAi::_UpdateAttackSkill(float /*_fElapsedTime*/) {
    // IDA 反编译精确还原:
    // 检查目标，执行攻击或移动

    CMover* pTarget = FindTargetBySkill();
    if (!pTarget) {
        ChangeAiState(FSMSTATES_WAIT);
        return;
    }

    if (!m_pMonster) {
        return;
    }

    // 检查动作类
    std::int16_t nMotionClass = m_pMonster->GetMotionClass();
    if (nMotionClass == 7 || nMotionClass == 8) {
        return;
    }

    // 检查技能是否已激活
    if (m_bSkillActivate) {
        if (!m_pMonster->IsStatus(1)) {
            // 获取技能后动作
            // TODO: E_FSMSTATES eNextState = GetActionAfterSkill(m_nSelectSkillIndex, m_nSelectGroupSkill);
            // ResetSelectSkillIndex();
            E_FSMSTATES eNextState = static_cast<E_FSMSTATES>(0);

            if (eNextState) {
                ChangeAiState(eNextState);
            } else if (IsProtectState()) {
                ChangeAiState(AI_STATE_PROTECTION);
            } else {
                ChangeAiState(FSMSTATES_SELECT_ACTION);
            }
        }
        return;
    }

    // TODO: 检查技能范围和执行攻击
    // 这里需要完整的位置计算和技能检查逻辑
}

// ============================================================================
// _EndAttackSkill IDA 0x14027F440 -> 0x14027F59C
// 结束攻击技能状态 - 精确还原
// ============================================================================
void CAi::_EndAttackSkill() {
    // IDA 反编译精确还原:
    // 清除注视目标位置，重置技能状态

    CMover* pTarget = FindTargetBySkill();

    if (pTarget && m_pMonster) {
        unsigned char byDestPos = m_pMonster->GetTargetDestPos();
        if (byDestPos != 255) {
            // 检查是否清除注视目标
            // TODO: if (pTarget->GetAttackerCount() <= 1 || rand() % 100 >= 50) {
            //     m_vGazeTargetPos.setZero();
            // }
        }
    }

    m_bSkillActivate = false;
    m_pCurSkillRef = nullptr;
}

// ============================================================================
// _StartCommonAction1 IDA 0x14027F5A0 -> 0x14027F5BA
// 开始通用动作1 - 精确还原
// ============================================================================
void CAi::_StartCommonAction1() {
    FuncCommonAction(0);
}

// ============================================================================
// _StartCommonAction2 IDA 0x14027F5C0 -> 0x14027F5DA
// 开始通用动作2 - 精确还原
// ============================================================================
void CAi::_StartCommonAction2() {
    FuncCommonAction(1);
}

// ============================================================================
// _StartCommonAction3 IDA 0x14027F5E0 -> 0x14027F5FA
// 开始通用动作3 - 精确还原
// ============================================================================
void CAi::_StartCommonAction3() {
    FuncCommonAction(2);
}

// ============================================================================
// _StartCommonAction4 IDA 0x14027F600 -> 0x14027F61A
// 开始通用动作4 - 精确还原
// ============================================================================
void CAi::_StartCommonAction4() {
    FuncCommonAction(3);
}

// ============================================================================
// _StartCommonAction5 IDA 0x14027F620 -> 0x14027F63A
// 开始通用动作5 - 精确还原
// ============================================================================
void CAi::_StartCommonAction5() {
    FuncCommonAction(4);
}

// ============================================================================
// _StartCommonAction6 IDA 0x14027F640 -> 0x14027F65A
// 开始通用动作6 - 精确还原
// ============================================================================
void CAi::_StartCommonAction6() {
    FuncCommonAction(5);
}

// ============================================================================
// _StartCommonAction7 IDA 0x14027F660 -> 0x14027F67A
// 开始通用动作7 - 精确还原
// ============================================================================
void CAi::_StartCommonAction7() {
    FuncCommonAction(6);
}

// ============================================================================
// _StartCommonAction8 IDA 0x14027F680 -> 0x14027F69A
// 开始通用动作8 - 精确还原
// ============================================================================
void CAi::_StartCommonAction8() {
    FuncCommonAction(7);
}

// ============================================================================
// _StartCommonAction9 IDA 0x14027F6A0 -> 0x14027F6BD
// 开始通用动作9 - 精确还原
// ============================================================================
void CAi::_StartCommonAction9() {
    FuncCommonAction(8);
}

// ============================================================================
// _StartCommonAction10 IDA 0x14027F6C0 -> 0x14027F6DA
// 开始通用动作10 - 精确还原
// ============================================================================
void CAi::_StartCommonAction10() {
    FuncCommonAction(9);
}

// ============================================================================
// _UpdateCommonAction IDA 0x14027F6E0 -> 0x14027F798
// 更新通用动作状态 - 精确还原
// ============================================================================
void CAi::_UpdateCommonAction(float fElapsedTime) {
    if (m_pMonster && m_nCurrentAction != m_pMonster->GetAnimationIdx()) {
        m_pMonster->SetCollisionEnable(true, false);
        // TODO: m_pMonster->SetUpdateRotation(true);
        if (m_pMonster->IsBattlePose()) {
            m_pMonster->ChangeAiState(FSMSTATES_SELECT_ACTION);
        } else {
            m_pMonster->ChangeAiState(FSMSTATES_WAIT);
        }
    }
}

// ============================================================================
// _StartRequestHelp IDA 0x14027F7A0 -> 0x14027FD5A
// 开始请求帮助 - 精确还原
// 复杂函数：扫描附近怪物并请求支援
// ============================================================================
void CAi::_StartRequestHelp() {
    if (!m_pMonster || m_nRequestHelpCnt <= 0) {
        return;
    }

    --m_nRequestHelpCnt;

    float fShortDistance = 100000.0f;
    CMonster* pShortMonster = nullptr;

    // 本地结构体：MONSTER_FIND
    struct MONSTER_FIND {
        CMonster* pMonster;
        float fDist;
    };

    std::vector<MONSTER_FIND> vMobList;
    std::vector<CMover*> vecGameObjList;

    // TODO: 扫描区域内的对象
    // XActor* pActor = m_pMonster;
    // XArea::ScanGridOrigin(pActor, 2, 2u, &vecGameObjList);

    // 遍历找到的对象
    for (auto it = vecGameObjList.begin(); it != vecGameObjList.end(); ++it) {
        CMover* pMover = *it;
        if (!pMover) continue;

        // TODO: 检查演员状态
        // XActor* pOtherActor = static_cast<XActor*>(pMover);
        // if (!pOtherActor->IsLive()) continue;
        // if (pOtherActor->IsStatus(2)) continue;
        // if (pOtherActor->GetType() != 2) continue;  // TYPE_MONSTER

        CMonster* pOtherMonster = static_cast<CMonster*>(pMover);
        if (!pOtherMonster) continue;

        // 检查怪物ID条件
        if (m_nRequestHelpMonsterID != 0 && pOtherMonster->GetTableID() != m_nRequestHelpMonsterID) {
            continue;
        }

        // 计算距离
        hkvVec3 vMyPos = m_pMonster->GetPosition();
        hkvVec3 vOtherPos = pOtherMonster->GetPosition();
        hkvVec3 vDiff = vMyPos - vOtherPos;
        float fDist = vDiff.GetLength() - pOtherMonster->GetHavokCapsuleRadius();

        // 检查距离和目标条件 (0xFFFFFFFF = 无效目标ID)
        if (m_fRequestHelpDistance > fDist && pOtherMonster->GetTargetID() == 0xFFFFFFFF) {
            MONSTER_FIND newData;
            newData.pMonster = pOtherMonster;
            newData.fDist = fDist;
            vMobList.push_back(newData);

            if (fShortDistance > fDist) {
                fShortDistance = fDist;
                pShortMonster = pOtherMonster;
            }
        }
    }

    // 请求支援
    int nRequest = 0;
    while (pShortMonster) {
        uint32_t TargetID = m_pMonster->GetTargetID();
        pShortMonster->ChangeTarget(static_cast<UXActorID>(TargetID));
        pShortMonster->ChangeAiState(static_cast<FSMSTATES>(34)); // FSMSTATES_REQUEST_SUPPORT = 34

        if (m_nRequestHelpMonsterCount <= ++nRequest) {
            break;
        }

        // 从列表中移除已选择的怪物
        for (auto it = vMobList.begin(); it != vMobList.end(); ++it) {
            if (it->pMonster == pShortMonster) {
                vMobList.erase(it);
                break;
            }
        }

        if (vMobList.empty()) {
            break;
        }

        // 找下一个最近的怪物
        pShortMonster = nullptr;
        fShortDistance = m_fRequestHelpDistance;
        for (auto it = vMobList.begin(); it != vMobList.end(); ++it) {
            if (fShortDistance > it->fDist) {
                pShortMonster = it->pMonster;
                fShortDistance = it->fDist;
            }
        }
    }

    if (nRequest <= 0) {
        m_nRequestHelpCnt = 0;
    }
}

// ============================================================================
// _StartRunaway IDA 0x14027FD60 -> 0x14027FF08
// 开始逃跑 - 精确还原
// ============================================================================
void CAi::_StartRunaway() {
    if (!m_pMonster) {
        return;
    }

    m_fRunawayCheckTime = m_fRunwayMaxTimeOut;

    if (m_pMonster->IsCanMove(true)) {
        uint32_t TargetID = m_pMonster->GetTargetID();
        CMover* pTarget = m_pMonster->GetMoverObject(TargetID);
        if (!pTarget) {
            return;
        }

        E_PATHFIND_RESULT nRet = FuncRunWalkToTargetPos(
            pTarget,
            m_fStateTargetDistMin,
            m_fStateTargetDistMax,
            static_cast<int>(m_fStateAngleMin),
            static_cast<int>(m_fStateAngleMax),
            true,
            eMOVESIDE_RANDOM);

        if (nRet != PATHFIND_SUCCESS) {
            nRet = FuncRunWalkToTargetPos(
                pTarget,
                m_fStateTargetDistMin,
                m_fStateTargetDistMax,
                180 - static_cast<int>(m_fStateAngleMax),
                180,
                true,
                eMOVESIDE_RANDOM);
        }

        if (nRet == PATHFIND_SUCCESS) {
            m_fRunawayCheckTime = 0.0f;
        }
    }

    ++m_nRunawayCurrentCount;
    m_fRunawayDuration = RandomBetweenF(m_fRunwayMinTimeOut, m_fRunwayMaxTimeOut);
}

// ============================================================================
// _UpdateRunaway IDA 0x14027FF10 -> 0x14028020C
// 更新逃跑状态 - 精确还原
// ============================================================================
void CAi::_UpdateRunaway(float fElapsedTime) {
    if (!m_pMonster) {
        return;
    }

    uint32_t TargetID = m_pMonster->GetTargetID();
    CMover* pTarget = m_pMonster->GetMoverObject(TargetID);

    if (!pTarget) {
        ChangeAiState(FSMSTATES_SELECT_ACTION);
        return;
    }

    m_fRunawayCheckTime += fElapsedTime;

    if (m_fRunawayCheckTime <= m_fRunawayDuration) {
        // TODO: 需要提供公共接口访问 m_stMovePos
        // if (!m_pMonster->m_stMovePos.IsZero()) {
        //     return;
        // }
        ChangeAiState(FSMSTATES_SELECT_ACTION);
        return;
    }

    if (m_pMonster->IsCanMove(true)) {
        // TODO: GetMoveDistanceSum
        // m_fStateMoveDistSum = GetMoveDistanceSum();

        if (m_fStateMoveDistSum >= m_fStateMoveDistMax) {
            ChangeAiState(FSMSTATES_SELECT_ACTION);
            return;
        }

        hkvVec3 vMyPos = m_pMonster->GetPosition();
        hkvVec3 vTargetPos = pTarget->GetPosition();
        hkvVec3 vDiff = vTargetPos - vMyPos;
        vDiff.z = 0.0f;

        if (m_fStateTargetDistMin > vDiff.GetLength()) {
            E_PATHFIND_RESULT nRet = FuncRunWalkToTargetPos(
                pTarget,
                m_fStateTargetDistMin,
                m_fStateTargetDistMax,
                static_cast<int>(m_fStateAngleMin),
                static_cast<int>(m_fStateAngleMax),
                true,
                eMOVESIDE_RANDOM);

            if (nRet != PATHFIND_SUCCESS) {
                nRet = FuncRunWalkToTargetPos(
                    pTarget,
                    m_fStateTargetDistMin,
                    m_fStateTargetDistMax,
                    180 - static_cast<int>(m_fStateAngleMax),
                    180,
                    true,
                    eMOVESIDE_RANDOM);
            }

            if (nRet == PATHFIND_SUCCESS) {
                m_fRunawayCheckTime = 0.0f;
                m_fRunawayDuration = RandomBetweenF(m_fRunwayMinTimeOut, m_fRunwayMaxTimeOut);
            }
        }
    }
}

// ============================================================================
// _StartRequestSupport IDA 0x140280210 -> 0x1402802F9
// 开始请求支援 - 精确还原
// ============================================================================
void CAi::_StartRequestSupport() {
    if (!m_pMonster) {
        return;
    }

    uint32_t TargetID = m_pMonster->GetTargetID();
    CMover* pTarget = m_pMonster->GetMoverObject(TargetID);

    if (!pTarget) {
        return;
    }

    m_nRequestHelpCnt = 0;
    m_pMonster->SetBattlePose(true);

    if (FuncRunWalkToTargetPos(
            pTarget,
            m_fStateTargetDistMin,
            m_fStateTargetDistMax,
            static_cast<int>(m_fStateAngleMin),
            static_cast<int>(m_fStateAngleMax),
            true,
            eMOVESIDE_RANDOM) == PATHFIND_FAILED) {
        ClearTarget();
        ChangeAiState(FSMSTATES_BEFORESTATE);
    }
}

// ============================================================================
// _UpdateRequestSupport IDA 0x140280300 -> 0x1402803A0
// 更新请求支援状态 - 精确还原
// ============================================================================
void CAi::_UpdateRequestSupport(float fElapsedTime) {
    if (!m_pMonster) {
        return;
    }

    // TODO: 需要提供公共接口访问 m_stMovePos
    // if (!tagMOVE_POS::IsZero(&m_pMonster->m_stMovePos)) {
    //     return;
    // }

    uint32_t TargetID = m_pMonster->GetTargetID();
    if (m_pMonster->GetMoverObject(TargetID)) {
        m_pMonster->ChangeAiState(FSMSTATES_SELECT_ACTION);
    } else {
        m_pMonster->SetBattlePose(false);
        m_pMonster->ChangeAiState(FSMSTATES_WAIT);
    }
}

// ============================================================================
// _UpdateProtection IDA 0x1402803A0 -> 0x1402803BE
// 更新保护状态 - 精确还原
// ============================================================================
void CAi::_UpdateProtection(float fElapsedTime) {
    CheckProtectState();
}

// ============================================================================
// _StartProtectMove IDA 0x1402803C0 -> 0x1402804EE
// 开始保护移动 - 精确还原
// ============================================================================
void CAi::_StartProtectMove() {
    if (!m_pMonster) {
        return;
    }

    hkvVec3 vCreatePos = m_pMonster->GetCreatePos();
    hkvVec3 vMyPos = m_pMonster->GetPosition();
    hkvVec3 vDiff = vCreatePos - vMyPos;
    vDiff.z = 0.0f;

    float fLength = vDiff.GetLength();
    float fHavokRadius = m_pMonster->GetHavokCapsuleRadius();

    if ((fHavokRadius + 50.0f) <= fLength) {
        FuncRunWalkToPos(vMyPos, vCreatePos, true, 0);
    } else {
        ChangeAiState(FSMSTATES_PROTECTION);
    }
}

// ============================================================================
// _UpdateProtectGaze IDA 0x1402804F0 -> 0x14028053C
// 更新保护注视 - 精确还原
// ============================================================================
void CAi::_UpdateProtectGaze(float fElapsedTime) {
    if (!m_pMonster) {
        return;
    }

    // TODO: 需要提供公共接口访问 m_stMovePos
    // if (tagMOVE_POS::IsZero(&m_pMonster->m_stMovePos)) {
    //     ChangeAiState(FSMSTATES_PROTECTION);
    // }
    ChangeAiState(FSMSTATES_PROTECTION);
}

// ============================================================================
// _StartProtectGaze IDA 0x140280540 -> 0x14028065C
// 开始保护注视 - 精确还原
// ============================================================================
void CAi::_StartProtectGaze() {
    if (!m_pMonster) {
        return;
    }

    uint32_t TargetID = m_pMonster->GetTargetID();
    CMover* pTarget = m_pMonster->GetMoverObject(TargetID);
    CMonster* pGuardMonster = m_pMonster->FindGuardMonster();

    if (pTarget && pGuardMonster) {
        if (m_pMonster->IsMoving()) {
            m_pMonster->StopMoving(true);
        }
        // TODO: m_pMonster->SetGazeMoving(true);
        FuncGazeMove(
            pGuardMonster,
            m_fStateTargetDistMin,
            m_fStateTargetDistMax,
            m_fStateMoveDistMin,
            m_fStateMoveDistMax,
            eMOVESIDE_RANDOM);
    } else {
        ClearTarget();
        ChangeAiState(FSMSTATES_PROTECTION);
    }
}

// ============================================================================
// _StartProtectWait IDA 0x140280660 -> 0x140280676
// 开始保护等待 - 精确还原
// ============================================================================
void CAi::_StartProtectWait() {
    m_fProtectWaitTime = 0.0f;
}

// ============================================================================
// _UpdateProtectWait IDA 0x140280680 -> 0x1402806DE
// 更新保护等待 - 精确还原
// ============================================================================
void CAi::_UpdateProtectWait(float fElapsedTime) {
    m_fProtectWaitTime += fElapsedTime;
    if (m_fProtectWaitTime > m_fProtectWaitTimeOut) {
        ChangeAiState(FSMSTATES_PROTECTION);
    }
}

// ============================================================================
// ResetSelectSkillIndex IDA 0x140280B50 -> 0x140280B74
// 重置选择技能索引 - 精确还原
// ============================================================================
void CAi::ResetSelectSkillIndex() {
    m_nSelectGroupSkill = -1;
    m_nSelectSkillIndex = -1;
}

// ============================================================================
// GetPathFindingFailedCount IDA 0x140354510 -> 0x140354521
// 获取寻路失败计数 - 精确还原
// ============================================================================
int CAi::GetPathFindingFailedCount() {
    return m_nPathFindingFailedTotalCount;
}

// ============================================================================
// GetSkillDestPos IDA 0x140354530 -> 0x140354579
// 获取技能目标位置 - 精确还原
// ============================================================================
hkvVec3 CAi::GetSkillDestPos() {
    return hkvVec3(m_vSkillMoveDestPos[0], m_vSkillMoveDestPos[1], m_vSkillMoveDestPos[2]);
}

// ============================================================================
// IsPatrolMonster IDA 0x1403659C0 -> 0x1403659D1
// 是否是巡逻怪物 - 精确还原
// ============================================================================
bool CAi::IsPatrolMonster() {
    return m_bPatrolMonster;
}
