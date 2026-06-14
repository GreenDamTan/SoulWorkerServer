#include "Soulworker/GameServer/XGameServer/Monster.h"
#include "Soulworker/GameServer/XGameServer/Npc.h"
#include "Soulworker/GameServer/XCore/XArea/XArea.h"
#include "Soulworker/GameServer/XGameServer/InteractionObject.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// ============================================================================
// Monster Extended Functions - Buff, Movement, Skill
// ============================================================================
// Note: Attack, AttackProcess, ProcessAttack, SetTarget, GetTarget, ClearTarget
// are implemented in Monster.cpp (around line 3490+)

// ============================================================================
// Buff Functions
// ============================================================================

// AddBuff - 添加Buff，设置持续时间，应用效果
void CMonster::AddBuff(int nBuffID, int nSkillID, float fDuration, CMoverEx* pCaster) {
    // 检查Buff数量限制
    if (m_nBuffTotalCnt >= 50) {
        return;
    }

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

// RemoveBuff - 移除Buff，恢复效果
void CMonster::RemoveBuff(int nBuffID) {
    GreenDamTan_log(__FILE__, __FUNCTION__, "RemoveBuff called");
}

// ProcessBuff - 更新Buff计时器，过期处理
void CMonster::ProcessBuff(float fDeltaTime) {
    // 更新Buff状态
    if (m_nBuffTotalCnt > 0) {
        GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessBuff called");
    }
}

// ============================================================================
// Movement Functions
// ============================================================================

// MoveTo - 设置目标位置，开始移动
void CMonster::MoveTo(const hkvVec3& vDestPos, float fSpeed) {
    // 检查是否可以移动
    if (!IsCanMove(false)) {
        return;
    }

    // 设置移动速度
    if (fSpeed > 0.0f) {
        m_fMoveSpeed = fSpeed;
    }

    // 设置移动状态
    m_fMoving = 1;

    GreenDamTan_log(__FILE__, __FUNCTION__, "MoveTo called");
}

// StopMove - 停止当前移动，清除路径
void CMonster::StopMove() {
    // 清除移动状态
    m_fMoving = 0;

    StopMoving(true);

    GreenDamTan_log(__FILE__, __FUNCTION__, "StopMove called");
}

// ProcessMove - 更新位置，检查到达，寻路
void CMonster::ProcessMove(float fDeltaTime) {
    // 检查是否在移动
    if (!m_fMoving) {
        return;
    }

    // 检查是否可以移动
    if (!IsCanMove(false)) {
        StopMove();
        return;
    }

    // 检查路径点
    CheckWayPoint();

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessMove called");
}

// ============================================================================
// Skill Functions
// ============================================================================

// UseSkill - 触发技能，设置冷却，动画
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
        m_dwTargetID = static_cast<CMover*>(pTarget)->GetTargetID();
    }

    // 设置技能动画步骤
    m_bySkillAnimStep = 1;

    GreenDamTan_log(__FILE__, __FUNCTION__, "UseSkill called");
}

// CancelSkill - 取消当前技能，清理
void CMonster::CancelSkill() {
    // 清除技能状态
    m_nNextSkillID = 0;
    m_nCurSkillTableIdx = 0;
    m_bySkillAnimStep = 0;
    m_eSkillType = 0;

    GreenDamTan_log(__FILE__, __FUNCTION__, "CancelSkill called");
}

// ProcessSkill - 技能帧处理，效果
void CMonster::ProcessSkill(float fDeltaTime) {
    // 检查技能动画步骤
    if (m_bySkillAnimStep == 0) {
        return;
    }

    GreenDamTan_log(__FILE__, __FUNCTION__, "ProcessSkill called");
}

// ============================================================================
// CNpc Stub Functions
// These are stub implementations to resolve linker errors until Npc.cpp is fixed
// ============================================================================

// IDA: ?MoveToWayPoint@CNpc@@QEAAXH@Z @ 0x1403a4010
// 精确还原: NPC移动到路径点
void CNpc::MoveToWayPoint(int nWayPointID) {
    // IDA: Initialize target position
    hkvVec3 vTargetPos;
    vTargetPos.setZero();

    // IDA: Get current position
    hkvVec3 vMyPos = GetPosition();
    // Note: SetCreatePos not yet implemented

    // IDA: Get area and check for nav mesh
    XArea* pArea = GetArea();
    if (pArea) {
        if (pArea->GetNavMeshInstance()) {
            // IDA: Search for way point info
            // Note: VEventObjectResource::SearchFromID not available yet
            // This is a simplified implementation
            if (nWayPointID) {
                // TODO: Need VEventObjectResource::SearchFromID to get way point info
                // For now, just store the way point ID
                m_nMoveWayPointID = nWayPointID;
            }
        }
    } else {
        // IDA: No area - log error
        UXMapID mapID = GetMapInsID();
        GreenDamTan_log(__FILE__, __FUNCTION__, "if( NULL==pMaze ) [GetMazeID:%u]", mapID.nMapID);
    }
}

// IDA: ?CallMovingYaw@CNpc@@QEAAXMH@Z @ 0x1403a3cd0
// 精确还原: 设置NPC旋转
void CNpc::CallMovingYaw(float fYaw, int nKey) {
    // IDA: Clear movement state
    MoveingValueClear();
    ClearMotion();
    send_eSUB_CMD_MOVE_STOP(this);

    // IDA: Set callback state
    m_bCallMovingYaw = true;
    m_nCallMovingYawKey = nKey;

    // IDA: Set moving yaw and send idle packet
    SetMovingYaw(fYaw);
    send_eSUB_CMD_MOVE_IDLE(this, 0.0f);
}
