#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// ============================================================================
// 扩展函数 - Attack (攻击相关)
// ============================================================================

// ============================================================================
// Attack - 发起攻击，计算伤害，触发动画
// ============================================================================
void CMonster::Attack(CMoverEx* pTarget, int nSkillID, float fDamage) {
    // 检查是否可以攻击
    if (!IsCanAttack()) {
        return;
    }

    // 检查目标有效性
    if (!pTarget) {
        return;
    }

    // 设置攻击目标 (CMoverEx inherits from CMover which has GetTargetID)
    m_dwTargetID = pTarget->GetTargetID();

    // 设置技能ID
    m_nNextSkillID = nSkillID;

    // 计算伤害
    // TODO: 从技能表获取伤害参数
    // TB_SKILL* pSkill = GetSkillTable(nSkillID);
    // if (pSkill) {
    //     fDamage = CalculateDamage(pSkill, pTarget);
    // }

    // 触发攻击动画
    // ChangeMotion(15, 1, 0);  // 15 = 攻击动作

    // 设置攻击状态
    m_bySkillAnimStep = 1;

    // 更新仇恨值
    // ApplyAggroValue(dwTargetID, fDamage * 0.0002f, true);

    GreenDamTan_log(__FILE__, __FUNCTION__, "Attack called");
}

// ============================================================================
// AttackProcess - 处理攻击帧，击中检测，伤害应用
// ============================================================================
void CMonster::AttackProcess(float fDeltaTime) {
    // 检查技能动画步骤
    if (m_bySkillAnimStep == 0) {
        return;
    }

    // 获取当前技能表索引
    if (!m_nCurSkillTableIdx) {
        return;
    }

    // 处理攻击判定
    // TODO: 获取攻击判定触发器
    // TB_SKILL* pSkill = GetSkillTable(m_nCurSkillTableIdx);
    // if (pSkill) {
    //     // 遍历攻击判定触发器
    //     for (int i = 0; i < pSkill->TriggerCount; ++i) {
    //         AttackJudgmentTrigger* pTrigger = pSkill->Triggers[i];
    //         if (pTrigger) {
    //             // 检测击中目标
    //             CMoverEx* pTarget = GetTarget();
    //             if (pTarget) {
    //                 // 应用伤害
    //                 tagACTION_DAMAGE dmgInfo = {};
    //                 dmgInfo.dwID = GetID();
    //                 dmgInfo.nDamage = CalculateDamage(pSkill, pTarget);
    //                 pTarget->Damage(dmgInfo, m_nCurSkillTableIdx, nullptr);
    //             }
    //         }
    //     }
    // }

    // 更新攻击帧时间 (使用 GetSkillLoopTime 或局部变量)
    // m_fSkillLoopTime += fDeltaTime;

    GreenDamTan_log(__FILE__, __FUNCTION__, "AttackProcess called");
}

// ============================================================================
// ProcessAttack - 处理攻击结果，连击，冷却
// ============================================================================
void CMonster::ProcessAttack() {
    // 检查攻击是否完成
    if (m_bySkillAnimStep == 0) {
        return;
    }

    // 检查连击
    if (m_nHitCount > 0) {
        // 连击处理
        // TODO: 根据连击数增加伤害倍率
        // float fComboRate = 1.0f + m_nHitCount * 0.1f;
    }

    // 设置技能冷却
    if (m_nCurSkillTableIdx > 0) {
        // TODO: 从技能表获取冷却时间
        // TB_SKILL* pSkill = GetSkillTable(m_nCurSkillTableIdx);
        // if (pSkill) {
        //     float fCoolDown = pSkill->CoolDown * GetSkillCoolDownRate();
        //     SetSkillCoolDown(m_nCurSkillTableIdx, fCoolDown);
        // }
    }

    // 重置攻击状态
    m_bySkillAnimStep = 0;
    m_nNextSkillID = 0;

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessAttack called");
}

// ============================================================================
// 扩展函数 - Buff (增益/减益相关)
// ============================================================================

// ============================================================================
// AddBuff - 添加Buff，设置持续时间，应用效果
// ============================================================================
void CMonster::AddBuff(int nBuffID, int nSkillID, float fDuration, CMoverEx* pCaster) {
    // 检查Buff数量限制
    if (m_nBuffTotalCnt >= 50) {
        return;
    }

    // 检查是否已有相同Buff
    // TODO: 遍历 m_stBuffState 检查重复
    // for (int i = 0; i < 50; ++i) {
    //     if (m_stBuffState[i].nBuffID == nBuffID && m_stBuffState[i].bActive) {
    //         // 刷新持续时间
    //         m_stBuffState[i].fRemainTime = fDuration;
    //         return;
    //     }
    // }

    // 找到空闲Buff槽位
    // int nSlot = -1;
    // for (int i = 0; i < 50; ++i) {
    //     if (!m_stBuffState[i].bActive) {
    //         nSlot = i;
    //         break;
    //     }
    // }

    // if (nSlot < 0) {
    //     return;
    // }

    // 设置Buff信息
    // m_stBuffState[nSlot].nBuffID = nBuffID;
    // m_stBuffState[nSlot].nSkillID = nSkillID;
    // m_stBuffState[nSlot].fRemainTime = fDuration;
    // m_stBuffState[nSlot].dwCasterID = pCaster ? pCaster->GetTargetID() : 0;
    // m_stBuffState[nSlot].bActive = true;

    // 应用Buff效果
    // TODO: 从Buff表获取效果并应用
    // TB_BUFF* pBuff = GetBuffTable(nBuffID);
    // if (pBuff) {
    //     ApplyBuffEffect(pBuff);
    // }

    // 增加Buff计数
    ++m_nBuffTotalCnt;
    if (nBuffID >= 0) {
        ++m_nBuffCnt;
    } else {
        ++m_nDebuffCnt;
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "AddBuff called");
}

// ============================================================================
// RemoveBuff - 移除Buff，恢复效果
// ============================================================================
void CMonster::RemoveBuff(int nBuffID) {
    // 查找Buff
    // TODO: 遍历 m_stBuffState 查找
    // for (int i = 0; i < 50; ++i) {
    //     if (m_stBuffState[i].nBuffID == nBuffID && m_stBuffState[i].bActive) {
    //         // 移除Buff效果
    //         TB_BUFF* pBuff = GetBuffTable(nBuffID);
    //         if (pBuff) {
    //             RemoveBuffEffect(pBuff);
    //         }
    //         
    //         // 标记为无效
    //         m_stBuffState[i].bActive = false;
    //         
    //         // 减少计数
    //         --m_nBuffTotalCnt;
    //         if (nBuffID >= 0) {
    //             --m_nBuffCnt;
    //         } else {
    //             --m_nDebuffCnt;
    //         }
    //         
    //         break;
    //     }
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "RemoveBuff called");
}

// ============================================================================
// ProcessBuff - 更新Buff计时器，过期处理
// ============================================================================
void CMonster::ProcessBuff(float fDeltaTime) {
    // 遍历所有Buff
    // TODO: 遍历 m_stBuffState
    // for (int i = 0; i < 50; ++i) {
    //     if (m_stBuffState[i].bActive) {
    //         // 减少剩余时间
    //         m_stBuffState[i].fRemainTime -= fDeltaTime;
    //         
    //         // 检查是否过期
    //         if (m_stBuffState[i].fRemainTime <= 0.0f) {
    //             // 移除Buff
    //             RemoveBuff(m_stBuffState[i].nBuffID);
    //         }
    //         
    //         // 处理持续效果 (DoT/HoT)
    //         TB_BUFF* pBuff = GetBuffTable(m_stBuffState[i].nBuffID);
    //         if (pBuff && pBuff->IsPeriodic) {
    //             m_stBuffState[i].fPeriodicTimer += fDeltaTime;
    //             if (m_stBuffState[i].fPeriodicTimer >= pBuff->PeriodicInterval) {
    //                 m_stBuffState[i].fPeriodicTimer = 0.0f;
    //                 ApplyPeriodicEffect(pBuff);
    //             }
    //         }
    //     }
    // }

    // 更新Buff状态
    if (m_nBuffTotalCnt > 0) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessBuff called");
    }
}

// ============================================================================
// 扩展函数 - Movement (移动相关)
// ============================================================================

// ============================================================================
// MoveTo - 设置目标位置，开始移动
// ============================================================================
void CMonster::MoveTo(const hkvVec3& vDestPos, float fSpeed) {
    // 检查是否可以移动
    if (!IsCanMove(false)) {
        return;
    }

    // 设置目标位置
    // TODO: 设置到 m_stMovePos
    // m_stMovePos.vDestPos = vDestPos;
    // m_stMovePos.bActive = true;

    // 设置移动速度
    if (fSpeed > 0.0f) {
        m_fMoveSpeed = fSpeed;
    }

    // 设置移动状态
    m_bMoving = true;

    // 设置朝向目标
    SetDirectionTo(vDestPos);

    // 发送移动包
    // TODO: send_eSUB_CMD_MOVE(this, vDestPos.x, vDestPos.y, 0);

    GreenDamTan_log(__FILE__, __FUNCTION__, "MoveTo called");
}

// ============================================================================
// StopMove - 停止当前移动，清除路径
// ============================================================================
void CMonster::StopMove() {
    // 清除移动状态
    m_bMoving = false;

    // 清除移动值
    // TODO: CMover::MoveingValueClear();

    // 清除目标位置
    // m_stMovePos.bActive = false;

    // 发送停止移动包
    StopMoving(true);

    GreenDamTan_log(__FILE__, __FUNCTION__, "StopMove called");
}

// ============================================================================
// ProcessMove - 更新位置，检查到达，寻路
// ============================================================================
void CMonster::ProcessMove(float fDeltaTime) {
    // 检查是否在移动
    if (!m_bMoving) {
        return;
    }

    // 检查是否可以移动
    if (!IsCanMove(false)) {
        StopMove();
        return;
    }

    // 获取当前位置和目标位置
    hkvVec3 vCurPos = GetPosition();
    // TODO: hkvVec3 vDestPos = m_stMovePos.vDestPos;

    // 计算移动方向和距离
    // hkvVec3 vDir = vDestPos - vCurPos;
    // float fDist = vDir.getLength();
    // 
    // if (fDist < 0.1f) {
    //     // 到达目标
    //     StopMove();
    //     return;
    // }
    // 
    // // 归一化方向
    // vDir /= fDist;
    // 
    // // 计算移动距离
    // float fMoveDist = m_fMoveSpeed * fDeltaTime;
    // if (fMoveDist > fDist) {
    //     fMoveDist = fDist;
    // }
    // 
    // // 更新位置
    // hkvVec3 vNewPos = vCurPos + vDir * fMoveDist;
    // SetPositionXVec3(vNewPos);

    // 检查路径点
    CheckWayPoint();

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessMove called");
}

// ============================================================================
// 扩展函数 - Skill (技能相关)
// ============================================================================

// ============================================================================
// UseSkill - 触发技能，设置冷却，动画
// ============================================================================
void CMonster::UseSkill(int nSkillID, CMoverEx* pTarget) {
    // 检查是否可以攻击
    if (!IsCanAttack()) {
        return;
    }

    // 设置技能ID
    m_nNextSkillID = nSkillID;
    m_nCurSkillTableIdx = nSkillID;

    // 设置目标
    if (pTarget) {
        m_dwTargetID = pTarget->GetTargetID();
    }

    // 获取技能表
    // TODO: TB_SKILL* pSkill = GetSkillTable(nSkillID);
    // if (pSkill) {
    //     // 设置技能动画
    //     ChangeMotion(pSkill->MotionClass, 1, 0);
    //     
    //     // 设置冷却
    //     float fCoolDown = pSkill->CoolDown * GetSkillCoolDownRate();
    //     SetSkillCoolDown(nSkillID, fCoolDown);
    //     
    //     // 设置技能类型
    //     m_eSkillType = pSkill->Skill_Type;
    // }

    // 设置技能动画步骤
    m_bySkillAnimStep = 1;

    GreenDamTan_log(__FILE__, __FUNCTION__, "UseSkill called");
}

// ============================================================================
// CancelSkill - 取消当前技能，清理
// ============================================================================
void CMonster::CancelSkill() {
    // 清除技能状态
    m_nNextSkillID = 0;
    m_nCurSkillTableIdx = 0;
    m_bySkillAnimStep = 0;
    m_eSkillType = 0;

    // 清除动作缓冲中的技能动作
    // TODO: CActionBuffer::DeleteCodeData(&m_xActionBuffer, 1);

    // 重置动画
    // ChangeMotion(1, 1, 0);  // 1 = 待机动作

    GreenDamTan_log(__FILE__, __FUNCTION__, "CancelSkill called");
}

// ============================================================================
// ProcessSkill - 技能帧处理，效果
// ============================================================================
void CMonster::ProcessSkill(float fDeltaTime) {
    // 检查技能动画步骤
    if (m_bySkillAnimStep == 0) {
        return;
    }

    // 根据技能动画步骤处理
    switch (m_bySkillAnimStep) {
    case 1:  // 准备阶段
        // TODO: 检查准备时间
        // if (GetSkillLoopTime() >= fPrepTime) {
        //     m_bySkillAnimStep = 2;
        // }
        break;

    case 2:  // 施法阶段
        // 处理攻击判定
        AttackProcess(fDeltaTime);
        // TODO: 检查施法时间
        // if (GetSkillLoopTime() >= fCastTime) {
        //     m_bySkillAnimStep = 3;
        // }
        break;

    case 3:  // 结束阶段
        // TODO: 检查结束时间
        // if (GetSkillLoopTime() >= fEndTime) {
        //     m_bySkillAnimStep = 0;
        //     ProcessAttack();
        // }
        break;

    default:
        break;
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessSkill called");
}

// ============================================================================
// 扩展函数 - Target (目标相关)
// ============================================================================

// ============================================================================
// SetTarget - 设置目标ID，通知AI
// ============================================================================
void CMonster::SetTarget(std::uint32_t dwTargetID) {
    // 检查目标是否有效
    if (dwTargetID == 0xFFFFFFFF) {
        ClearTarget();
        return;
    }

    // 设置目标ID
    m_dwTargetID = dwTargetID;

    // 通知AI
    if (m_pAi) {
        // TODO: CAi::SetTarget(m_pAi, dwTargetID);
    }

    // 更新仇恨
    // ApplyAggroValue(dwTargetID, 0.0f, false);

    GreenDamTan_log(__FILE__, __FUNCTION__, "SetTarget called");
}

// ============================================================================
// GetTarget - 返回当前目标
// ============================================================================
CMoverEx* CMonster::GetTarget() {
    // 检查目标ID
    if (m_dwTargetID == 0 || m_dwTargetID == 0xFFFFFFFF) {
        return nullptr;
    }

    // 获取目标对象
    CMover* pMover = CMover::GetMoverObject(m_dwTargetID);
    if (!pMover) {
        return nullptr;
    }

    // 转换为 CMoverEx
    return static_cast<CMoverEx*>(pMover);
}

// ============================================================================
// ClearTarget - 清除目标，重置AI
// ============================================================================
void CMonster::ClearTarget() {
    // 清除目标ID
    m_dwTargetID = 0xFFFFFFFF;

    // 重置AI目标
    if (m_pAi) {
        // TODO: CAi::ClearTarget(m_pAi);
    }

    // 清除仇恨列表中的目标仇恨
    // auto it = m_arDamageMeter.find(m_dwTargetID);
    // if (it != m_arDamageMeter.end()) {
    //     it->second.fAggro = 0.0f;
    // }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ClearTarget called");
}
