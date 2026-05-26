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
// ============================================================================
void CAi::Initialize(CMonster* pMonster) {
    // IDA 反编译确认:
    // this->m_pMonster = _pMonster;
    // this->m_nStatePreHP = this->m_pMonster->GetHP(this->m_pMonster);
    // 创建状态机和各种转换对象
    // 注册条件函数和状态函数

    m_pMonster = pMonster;

    if (m_pMonster) {
        m_nStatePreHP = m_pMonster->GetHP();
    }

    // TODO: 创建状态机 (CFsmClass)
    // TODO: 创建技能转换数组 (CFsmTransition)
    // TODO: 注册条件函数指针
    // TODO: 注册状态函数和状态变量

    GreenDamTan_log(__FILE__, __FUNCTION__, "CAi initialized");
}

// ============================================================================
// Update - 更新AI
// ============================================================================
void CAi::Update(float fDeltaTime) {
    // TODO: 实现完整的AI更新逻辑
    // 包括状态机更新、仇恨更新、条件检查等

    (void)fDeltaTime;  // 暂时避免未使用警告
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
        // TODO: 检查 m_pCurSkillRef->Target_Type
        // 假设 Target_Type 字段存在
        // int nTargetType = m_pCurSkillRef->Target_Type;
        int nTargetType = 0;  // TODO: 获取实际值

        if (nTargetType != 0) {
            // 如果是跟随者，返回拥有者玩家
            if (m_pMonster->IsFollowObject()) {
                // TODO: return CMoverEx::GetOwnerPlayer(m_pMonster);
                return nullptr;  // 暂时返回 nullptr
            } else {
                return nullptr;
            }
        }
    }

    // 返回当前目标
    // TODO: std::uint32_t dwTargetID = CMover::GetTargetID(m_pMonster);
    // TODO: return CMover::GetMoverObject(m_pMonster, dwTargetID);

    return nullptr;  // 暂时返回 nullptr
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
    // TODO: XArea* pArea = m_pMonster->GetArea();
    // if (!pArea) {
    //     // 输出警告日志
    //     return;
    // }

    // 扫描附近的对象
    // TODO: std::vector<CMover*> vecGameObjList;
    // TODO: XArea::ScanGridOrigin(m_pMonster, 2, 2, &vecGameObjList);

    // 遍历找到的对象
    // for (auto& obj : vecGameObjList) {
    //     CMoverEx* pOtherMover = dynamic_cast<CMoverEx*>(obj);
    //     if (!pOtherMover) continue;
    //
    //     // 检查是否存活且不是死亡状态
    //     if (!pOtherMover->IsLive()) continue;
    //     // TODO: if (XActor::IsStatus(pOtherMover, 2)) continue;
    //
    //     // 检查是否是敌人
    //     if (!m_pMonster->IsEnemy(pOtherMover)) continue;
    //
    //     // 检查是否是守卫怪物
    //     if (IsGuardMonster(pOtherMover)) continue;
    //
    //     // 检查是否是怪物类型
    //     // TODO: if (XActor::GetType(pOtherMover) != 2) continue;
    //
    //     CMonster* pMonster = dynamic_cast<CMonster*>(pOtherMover);
    //     if (!pMonster) continue;
    //
    //     // 检查怪物标志和Boss状态
    //     // TODO: int nFlag = pMonster->GetMonsterFlag();
    //     // if (nFlag & 4) continue;  // 特定标志
    //     if (pMonster->IsBoss()) continue;
    //
    //     // 计算距离
    //     const hkvVec3& posThis = m_pMonster->GetPosition();
    //     const hkvVec3& posOther = pMonster->GetPosition();
    //     float fDistance = (posThis - posOther).getLength();
    //
    //     // 减去碰撞半径
    //     float fRadius = pMonster->GetHavokCapsuleRadius();
    //     fDistance -= fRadius;
    //
    //     // 检查是否在范围内
    //     if (fDistance >= m_fSpawnAggroDistance) continue;
    //
    //     // 获取怪物ID并应用仇恨
    //     std::uint32_t dwID = m_pMonster->GetID();
    //     pMonster->ApplyAggroValue(dwID, m_fSpawnAggroValue, true);
    //     pMonster->DamageAggressive();
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "FuncSpawnAggro executed");
}

// ============================================================================
// CheckSkillCondition IDA 0x140269930 -> 0x140269B22
// 技能条件检查
// ============================================================================
bool CAi::CheckSkillCondition(unsigned int nSkillIndex, int nSkillGroup) {
    // IDA 反编译确认的流程:
    // 1. 检查技能组条件
    // 2. 检查技能索引范围
    // 3. 获取技能转换的条件向量
    // 4. 遍历条件并检查是否满足
    // 5. 返回所有条件是否满足

    // 检查技能组条件
    if (!CheckSkillGroupCondition(nSkillIndex, nSkillGroup)) {
        return false;
    }

    // 检查技能索引范围
    if (nSkillIndex >= 10) {
        return false;
    }

    // 获取技能转换对象
    CFsmTransition* pTransition = m_arSkillTransition[nSkillIndex];
    if (!pTransition) {
        // 没有条件，直接返回true
        return true;
    }

    // TODO: 获取条件向量
    // CFsmTransition::GetConditionVectorBegin(pTransition, &itBegin);
    // CFsmTransition::GetConditionVectorEnd(pTransition, &itEnd);

    // TODO: 获取条件数量
    // int nConditionCount = CFsmTransition::GetConditionNumber(pTransition);
    int nConditionCount = 0;
    int nConditionSuccessedCount = 0;

    // TODO: 遍历条件并检查
    // while (itBegin != itEnd) {
    //     CFsmCondition* pCondition = *itBegin;
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
    //     ++itBegin;
    // }

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

    // TODO: 实现实际的守卫检查逻辑
    // 检查怪物的 GuardID 或特定标志

    return false;
}

// ============================================================================
// CheckProtectState
// 检查保护状态
// ============================================================================
void CAi::CheckProtectState() {
    // TODO: 实现保护状态检查
    // 检查是否需要进入保护状态
}

// ============================================================================
// CheckFuzzyData
// 检查模糊数据
// ============================================================================
int CAi::CheckFuzzyData(int iType, int nRate) {
    // TODO: 实现模糊逻辑检查
    // 根据模糊类型计算实际权重值

    (void)iType;
    return nRate;  // 暂时返回原始值
}

// ============================================================================
// ChangeAiState
// 改变AI状态
// ============================================================================
void CAi::ChangeAiState(int nState) {
    // TODO: 实现状态切换逻辑
    // 1. 保存当前状态
    // 2. 设置新状态
    // 3. 触发状态进入函数

    GreenDamTan_log(__FILE__, __FUNCTION__, "ChangeAiState called");
    (void)nState;
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
