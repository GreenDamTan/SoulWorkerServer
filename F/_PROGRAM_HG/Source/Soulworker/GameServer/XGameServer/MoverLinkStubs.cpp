#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XGameServer/ActionResMgr.h"
#include "Soulworker/GameServer/XGameServer/MySkillList.h"
#include "Soulworker/GameServer/XGameServer/VChainLightningObject.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#undef PS_ITEM_PACKAGE_LIST

// Note: CItem::GetID is now implemented in Item/CItem.cpp

// Note: ApplySkillDamageFrame, GetSGAbsorbRate, FindBuffStatus,
// send_eSUB_CMD_ACTION_SKILL, and send_eSUB_CMD_CHAIN
// are now implemented in Mover.cpp

// IDA: ?GetMaxSuperArmorGage@CMover@@QEAAMXZ @ 0x1402A5050
float CMover::GetMaxSuperArmorGage() const
{
    return m_fMaxSuperArmorGage;
}

// IDA: ?GetCurSuperArmorGage@CMover@@QEAAMXZ @ 0x1402A5030
float CMover::GetCurSuperArmorGage() const
{
    return m_fCurSuperArmorGage;
}

// IDA: ?SetCurSuperArmorGage@CMover@@QEAAXM@Z @ 0x140353C60
void CMover::SetCurSuperArmorGage(float fCurSuperArmorGage)
{
    m_fCurSuperArmorGage = fCurSuperArmorGage;
}

// IDA: ?IsCanApplyBuff@CMoverEx@@QEAA_NGPEAUTB_BUFF@@@Z
// IDA 精确还原 - 检查是否可应用Buff（CMoverEx版本）
// 注意：此函数在 IDA 中未找到，根据逻辑推断实现
bool CMoverEx::IsCanApplyBuff(std::uint16_t nBuffIndex, TB_BUFF* pBuffTable) {
    // 如果没有传入 pBuffTable，从资源管理器获取
    if (!pBuffTable) {
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (!pServer) {
            return false;
        }
        pBuffTable = pServer->GetResourceMgr().GetTB_BUFF(nBuffIndex);
        if (!pBuffTable) {
            return false;
        }
    }

    // 调用基类虚函数检查区域限制
    return CMover::IsCanApplyBuff(nBuffIndex, nullptr);
}

// Note: UpdateSkillAnimInfo is now implemented in Mover.cpp

// Note: GetSkillAnimName is now implemented in Mover.cpp

int CMoverEx::GetRandomDamage(std::uint8_t bySkillAttribute, int iItemRateResult)
{
    (void)bySkillAttribute;
    return iItemRateResult;
}

bool CMoverEx::IsExceptionalDamage()
{
    return false;
}

// Note: CheckPassiveSkillByHit is now implemented in Mover.cpp

void CMoverEx::NotifyPhaseChanged(std::uint8_t byOldPhase)
{
    (void)byOldPhase;
}

// Note: SetBuffAbility is now implemented in Mover.cpp

void CMoverEx::ApplyBuffAbilityForAttacker(std::uint32_t dwAttackerID)
{
    (void)dwAttackerID;
}

void CMoverEx::ClearBuffAbilityForAttacker(std::uint32_t dwAttackerID)
{
    (void)dwAttackerID;
}

void CMoverEx::CheckOptionEffectInvoke(EFFECT_CONDITION_TYPE eConditionType, CMoverEx* pMover,
                                       float fParam, EFFECT_INVOKE_TYPE eInvokeType)
{
    (void)eConditionType;
    (void)pMover;
    (void)fParam;
    (void)eInvokeType;
}

float CMoverEx::GetTotalOptionEffectValue(EFFECT_STATUS_TYPE eStatusType)
{
    (void)eStatusType;
    return 0.0f;
}

float CMoverEx::GetMultipleAbsorbSG()
{
    return m_fMultipleAbsorbSG;
}

void CMoverEx::CheckBuffDamage(CMoverEx* pTargetMover, CMoverEx* pAttacker, int nIndex, int nDamage)
{
    (void)pTargetMover;
    (void)pAttacker;
    (void)nIndex;
    (void)nDamage;
}

void CMoverEx::_GenerateEventObject(int eSelfTypeA, int iSelfIDA)
{
    (void)eSelfTypeA;
    (void)iSelfIDA;
}

void CMoverEx::SetGazeMoving(bool bGaze)
{
    m_bGazeMoving = bGaze ? 1 : 0;
}

// IDA: ?AddDefenseChangeInfo@CMoverEx@@QEAAHEEKM@Z (0x14037CF80)
// IDA 精确还原 - 添加防御类型变更信息
int CMoverEx::AddDefenseChangeInfo(std::uint8_t byType, std::uint8_t byDefenseType, std::uint32_t dwID, float fTime) {
    if (byType == 1) {
        SDefenseChangeInfo* pInfo = new SDefenseChangeInfo(dwID, byDefenseType, fTime);
        m_listDefenseChangeInfo.push_back(pInfo);
    } else if (byType == 2) {
        m_stDefenseChangeInfoByEffect.dwID = dwID;
        m_stDefenseChangeInfoByEffect.byDefenseType = byDefenseType;
        m_stDefenseChangeInfoByEffect.fChangeTime = fTime;
        if (byDefenseType == 3) {
            SetInvincibleActor(true);
        }
    } else {
        // byType == 0
        m_stDefenseChangeInfoByTrigger.dwID = dwID;
        m_stDefenseChangeInfoByTrigger.byDefenseType = byDefenseType;
        m_stDefenseChangeInfoByTrigger.fChangeTime = fTime;
    }

    m_DefanseChangeFlag |= (1 << byDefenseType);
    return ApplyDefenseChangeInfo();
}

// IDA: ?RemoveDefenseChangeInfo@CMoverEx@@QEAAHEK@Z (0x14037D100)
// IDA 精确还原 - 移除防御类型变更信息
int CMoverEx::RemoveDefenseChangeInfo(std::uint8_t byType, std::uint32_t dwID) {
    if (byType == 1) {
        for (auto it = m_listDefenseChangeInfo.begin(); it != m_listDefenseChangeInfo.end(); ++it) {
            SDefenseChangeInfo* pInfo = *it;
            if (pInfo->dwID == dwID) {
                delete pInfo;
                m_listDefenseChangeInfo.erase(it);
                break;
            }
        }
    } else if (byType == 2) {
        if (m_stDefenseChangeInfoByEffect.byDefenseType == 3) {
            SetInvincibleActor(false);
        }
        m_stDefenseChangeInfoByEffect.Clear();
    } else {
        // byType == 0
        m_stDefenseChangeInfoByTrigger.Clear();
        ClearImmunityStatus(2);
    }

    // 重新计算 m_DefanseChangeFlag
    m_DefanseChangeFlag = 0;

    if (m_stDefenseChangeInfoByTrigger.fChangeTime > 0.0f) {
        m_DefanseChangeFlag |= (1 << m_stDefenseChangeInfoByTrigger.byDefenseType);
    }

    for (const auto* pInfo : m_listDefenseChangeInfo) {
        m_DefanseChangeFlag |= (1 << pInfo->byDefenseType);
    }

    if (m_stDefenseChangeInfoByEffect.fChangeTime > 0.0f) {
        m_DefanseChangeFlag |= (1 << m_stDefenseChangeInfoByEffect.byDefenseType);
    }

    return ApplyDefenseChangeInfo();
}

// IDA: ?ApplyDefenseChangeInfo@CMoverEx@@QEAAHXZ (0x14037D5B0)
// IDA 精确还原 - 应用防御类型变更
bool CMoverEx::ApplyDefenseChangeInfo() {
    std::uint8_t byDefenseOld = m_byDefenseType;
    TypeOfDefense eDefense = TypeOfDefense::DEFENSE_NONE;

    if (m_pCurMotionEvent) {
        eDefense = m_pCurMotionEvent->eDefenseType;
    }

    if ((m_DefanseChangeFlag & 8) != 0 || eDefense == TypeOfDefense::INVINCIBLE) {
        SetSimpleDefenseType(3);
    } else if ((m_DefanseChangeFlag & 4) != 0 || eDefense == TypeOfDefense::SUPER_ARMOR) {
        SetSimpleDefenseType(2);
    } else {
        if ((m_DefanseChangeFlag & 1) == 0) {
            if (eDefense == TypeOfDefense::DEFENSE_NONE) {
                SetSimpleDefenseType(m_byDefaultDefenseType);
                return byDefenseOld != m_byDefenseType;
            }
            if (!XActor::IsStatus(2u) && !XActor::IsStatus(4u)) {
                SetSimpleDefenseType(static_cast<std::uint8_t>(eDefense));
                return byDefenseOld != m_byDefenseType;
            }
        }
        SetSimpleDefenseType(0);
    }

    return byDefenseOld != m_byDefenseType;
}

// IDA: ?IsHaveImunityInvincibleBuff@CMover@@QEAAHG@Z (0x14036A640)
// IDA 精确还原 - 检查是否有免疫无敌Buff
bool CMover::IsHaveImunityInvincibleBuff(std::uint16_t nExceptBuffIndex) const {
    for (int i = 0; i < 50; ++i) {
        if (m_stBuffState[i].nBuffIndex != nExceptBuffIndex &&
            m_stBuffState[i].nBuffIndex != 0 &&
            m_stBuffState[i].byEffectType == 5 &&
            m_stBuffState[i].byStatType[0] == 1) {
            return true;
        }
    }
    return false;
}

// IDA: ?ClearImmunityStatus@CMover@@QEAAXK@Z (0x140353040)
// IDA 精确还原 - 清除免疫状态
void CMover::ClearImmunityStatus(std::uint32_t dwStatus) {
    m_dwImmunityStatus &= ~dwStatus;
}

// IDA: ?UpdateDefenseDisableBuff@CMover@@QEAAXXZ (0x14036B4D0)
// IDA 精确还原 - 更新防御禁用Buff标志
void CMover::UpdateDefenseDisableBuff() {
    std::uint8_t byOldVal = m_byDefenseTypeDisableFlag;
    m_byDefenseTypeDisableFlag = 0;

    if (m_nBuffTotalCnt > 0) {
        for (int i = 0; i < 50; ++i) {
            if (m_stBuffState[i].IsLife()) {
                if (m_stBuffState[i].byEffectType == 6) {
                    m_byDefenseTypeDisableFlag |= m_stBuffState[i].byStatType[0];
                    if (m_byDefenseTypeDisableFlag == 19) {
                        break;
                    }
                }
            }
        }
    }

    if (m_byDefenseTypeDisableFlag != byOldVal) {
        UpdateDefenseType();
    }
}

// IDA: ?send_eSUB_CMD_BUFF_DELETE@CMover@@QEAAXPEAV1@GK_NE@Z (0x140372D90)
// IDA 精确还原 - 发送Buff删除包
void CMover::send_eSUB_CMD_BUFF_DELETE(CMover* pMover, std::int16_t wBuffID, std::uint32_t dwOwnerID,
                                        bool bExcuteOutSkill, std::uint8_t bySendType) {
    XSendPacket xPacket(6, 0x15);
    UXActorID uxActorID = pMover->GetActorID();
    int nQuestID = CQuestCondition::GetQuestID(uxActorID);
    xPacket << nQuestID;
    xPacket << wBuffID;
    xPacket << bExcuteOutSkill;
    xPacket << dwOwnerID;

    if (!pMover->GetArea()) {
        bySendType = 1;
    }

    if (bySendType) {
        CGocNetwork::Send(this, xPacket);
    } else {
        CGocNetwork::SendBroadCast(this, xPacket, E_BROADCAST_TYPE::eNoneSelf);
    }

    DebugOut("send_eSUB_CMD_BUFF_DELETE>> %d", static_cast<std::uint16_t>(wBuffID));
}

// IDA: ?AnimPause@CMover@@QEAAXXZ (0x1403A2390)
// IDA 精确还原 - 暂停动画
void CMover::AnimPause() {
    m_bAnimPlay = 0;
}

// IDA: ?MoveingValueClear@CMover@@QEAAXXZ (0x1402A4BE0)
// IDA 精确还原 - 清除移动值
void CMover::MoveingValueClear() {
    m_fMoving = 0;
    m_stMovePos.Clear();
    m_stMoveGap.Clear();
    m_stMoveOffset.Clear();
}

// IDA: ?StopMoving@CMover@@UEAAX_N@Z - virtual function
// Default implementation - overridden by CMonster, CUser, CNpc
void CMover::StopMoving(bool bSendPacket) {
    // Default: just clear moving values
    MoveingValueClear();
    (void)bSendPacket;
}

// IDA: ?InitJumpData@CMoverEx@@QEAAXM_N@Z (0x140395BA0)
// IDA 精确还原 - 初始化跳跃数据
void CMoverEx::InitJumpData(float fDesiredHeight, bool bGravityScale) {
    (void)bGravityScale;  // Not used in this function

    m_bJumpAnim = 0;

    float fVel = CalcFlyVelocity(fDesiredHeight);

    if (fDesiredHeight <= 0.0f) {
        m_fJumpTime = 0.5f;
        m_fPrevJumpHeight = CalcFlyHeight(m_fJumpHeight, 0.5f);
        m_fFlyDirValue = 0.0f;
    } else {
        m_fFlyVelocity = fVel;
        m_fJumpHeight = fDesiredHeight;
        m_fJumpTime = 0.0f;
        m_fPrevJumpHeight = 0.0f;
        m_fFlyDirValue = 1.0f;
    }
}

// Note: FindBuffStatus, send_eSUB_CMD_ACTION_SKILL, send_eSUB_CMD_CHAIN
// are now implemented in Mover.cpp

bool CMySkillList::IsInAttackArea(CMoverEx* pMover, tagATTACK_AREA* stAreaInfo,
                                  int nDownAttack, int bPassiveType)
{
    (void)pMover;
    (void)stAreaInfo;
    (void)nDownAttack;
    (void)bPassiveType;
    return false;
}

VChainLightningObject* CMySkillList::CreateChainLightningObject(hkvVec3 vPos, hkvVec3 vDir,
                                                                CMover* pMover,
                                                                AttackJudgmentTrigger* pActionEvent,
                                                                CMoverEx* pTarget)
{
    (void)vPos;
    (void)vDir;
    (void)pMover;
    (void)pActionEvent;
    (void)pTarget;
    return nullptr;
}

XArea* ThreadLocalData::FindArea(UXMapID uxMapID)
{
    (void)uxMapID;
    return nullptr;
}

// Note: STCasualRaidTime is now defined inline in Timer.h

// Note: SetCurSuperArmorGage is defined earlier in this file

// IDA: ?SetFlyState@CMover@@QEAAX_N@Z (0x14036D2F0)
void CMover::SetFlyState(bool bFly) {
    if (bFly) {
        XActor::SetStatus(0x400000u);
    } else {
        XActor::ClearStatus(0x400000u);
    }
}

// IDA: ?GetCheckAttachToAttacker@CMoverEx@@QEAA_NXZ (0x1403A27B0)
bool CMoverEx::GetCheckAttachToAttacker() const {
    return m_bCheckAttachToAttacker;
}

// IDA: ?SetMovingYaw@CMoverEx@@IEAAXM@Z (0x14037F3E0)
void CMoverEx::SetMovingYaw(float fYaw) {
    m_fMovingYaw = fYaw;
}

// IDA: ?SetDirectionYaw@CMoverEx@@UEAAXME@Z (0x14037F400)
void CMoverEx::SetDirectionYaw(float fYaw, std::uint8_t byType) {
    if (byType == 1) {
        SetMovingYaw(fYaw);
        SetOrientationYaw(fYaw);
    } else {
        bool bImmediate = (byType != 3);
        SetMovingYaw(fYaw);
        if (IsControlMonster()) {
            if (bImmediate) {
                SetOrientationYaw(fYaw);
            }
        }
    }
}

// IDA: ?SetDie@CMoverEx@@UEAAXFH@Z (0x140397520)
// IDA 精确还原 - 设置死亡状态
// Note: IDA decorated name shows UEAA (virtual) but second param is int not bool
void CMoverEx::SetDie(std::int16_t nMotion, int bSuicide) {
    // IDA: if (!IsSystemActor() && !IsStatus(4) && !IsStatus(2))
    if (!IsSystemActor()
        && !XActor::IsStatus(4u)
        && !XActor::IsStatus(2u)) {
        // IDA: MoveingValueClear(this);
        CMover::MoveingValueClear();
        // IDA: AllBuffClear(this, 1u);
        CMover::AllBuffClear(1u);
        // IDA: XActor::SetStatus(&this->XActor, 4u);
        XActor::SetStatus(4u);

        if (nMotion == -1) {
            // IDA: if (IsHitDown()) { RealDie(13); }
            if (IsHitDown()) {
                RealDie(13);
            }
            // IDA: else if (!IsKnockDown() || m_nHitStatus == 5) { RealDie(12); }
            else if (!IsKnockDown() || m_nHitStatus == 5) {
                RealDie(12);
            }
        } else {
            // IDA: RealDie(nMotion);
            RealDie(nMotion);
        }
    }
    (void)bSuicide;  // Note: bSuicide parameter not used in this function per IDA
}

// IDA: ?ChangeToAkashicData@CMoverEx@@QEAAXPEBDH@Z (0x140395820)
// IDA exact restoration - modifies animation for Akashic skills
void CMoverEx::ChangeToAkashicData(const char* szSeqName, int nGroupID) {
    // IDA: Create key string for lookup
    VString strKey;
    strKey.Format("%d:%s", nGroupID, szSeqName);

    // IDA: Get action data list from Akashic resource manager
    XGameServer* pServer = XGameServer::Instance();
    if (!pServer) {
        return;
    }

    VArray<ActionTrigger*>* pActionData = pServer->GetAkashicManager().GetActionDataList(1, strKey);
    if (pActionData) {
        // IDA: Create Akashic action info
        CMover::CreateAkashicActionInfo(szSeqName);

        if (m_pAkashicActionInfo) {
            float fTime = GetAkashicTriggerTime();
            int iDataCount = pActionData->GetCurID();

            m_pAkashicActionInfo->arTriggers.SetSize(iDataCount, -1, 1);

            for (int i = 0; i < iDataCount; ++i) {
                ActionTrigger* pNewTrigger = *pActionData->ElementAt(i);
                if (pNewTrigger) {
                    // IDA: Set start time for type 18 triggers
                    if (pNewTrigger->TypeOfTrigger == 18) {
                        pNewTrigger->StartTime = fTime;
                    }
                }
                m_pAkashicActionInfo->arTriggers.SetAt(i, &pNewTrigger);
            }
        }
    } else {
        DebugOut("ChangeToAkashicData>> No action data found. Key:%s", VString::AsChar(&strKey));
    }
}

// IDA: ?ChangeActionTrigger@CMover@@QEAAXPEBD@Z (0x14036C9B0)
// IDA exact restoration - changes the current action trigger
void CMover::ChangeActionTrigger(const char* szAnimName) {
    // IDA: Check and delete action buffer if RESETAFTER behavior
    if (m_pCurMotionEvent && m_pCurMotionEvent->eActionBufferBehavior == RESETAFTER) {
        DeleteActionBuffer();
    }

    // IDA: Clear projectile index if skill manager exists
    if (m_pSkillMgr) {
        m_pSkillMgr->ProjectileIndexClear();
    }

    m_bSkipAnimOffset = 0;

    // IDA: Get new action description (const_cast needed - IDA shows this cast)
    m_pCurMotionEvent = const_cast<VAnimationInfo*>(CMover::GetActionDesc(szAnimName));

    if (m_pCurMotionEvent) {
        m_nAnimGroup = m_pCurMotionEvent->iAnimGroup;

        // IDA: Call change motion callback
        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            pServer->GetActionManager().ChangeMotionCallback(this, m_pCurMotionEvent);
        }
    } else {
        DebugOut("ChangeActionTrigger>> Not found anim info. (%s)", szAnimName);
    }
}

// IDA: ?DeleteActionBuffer@CMoverEx@@UEAAXXZ (0x1403989A0)
// IDA exact restoration - deletes all action buffers except special cases
void CMoverEx::DeleteActionBuffer() {
    for (std::uint8_t byCode = 0; byCode < 0x35u; ++byCode) {
        if (byCode == 4) {
            ExcuteActionTrigger(byCode);
        } else {
            std::uint8_t byIndex = 0;
            tagACTION_BUFFER* pActionBuffer = m_xActionBuffer.FindCodeData(byCode, &byIndex);

            // Code 26 (SummonMonsterTrigger): keep while the summon is still pending
            if (byCode == 26 && pActionBuffer) {
                SummonMonsterTrigger* pTrigger = static_cast<SummonMonsterTrigger*>(pActionBuffer->pActionTrigger);
                if (pTrigger && !pTrigger->IsLocalClient && pTrigger->SummonType && pTrigger->AlphaValue == 0.0f) {
                    continue;
                }
            }

            // Code 23 (Trigger byte flags at offset 296/297): keep per IDA condition
            if (byCode == 23 && pActionBuffer && pActionBuffer->pActionTrigger) {
                std::uint8_t* pActionTrigger = reinterpret_cast<std::uint8_t*>(pActionBuffer->pActionTrigger);
                if (pActionTrigger[296] == 0 && pActionTrigger[297] != 0) {
                    continue;
                }
            }

            // Code 18 - KeepLookTarget
            if (byCode == 18 && m_bKeepLookTarget) {
                m_bKeepLookTarget = false;
                m_fDefTurnSpeed = m_fBackupTurnSpeed;
            }

            m_xActionBuffer.DeleteCodeData(byCode);
        }
    }
}

// IDA: ?ExcuteActionTrigger@CMoverEx@@UEAAXE@Z (0x140398B60)
// IDA exact restoration - executes all action triggers with given code
void CMoverEx::ExcuteActionTrigger(std::uint8_t byCode) {
    while (true) {
        std::uint8_t byIndex = 0;
        tagACTION_BUFFER* pActionBuffer = m_xActionBuffer.FindCodeData(byCode, &byIndex);
        if (!pActionBuffer) {
            break;
        }

        // IDA: ResetPosition then process
        pActionBuffer->ResetPosition();
        // ActionBufferProcess(pActionBuffer);
        // TODO [DEPENDENCY]: ?ActionBufferProcess@CMoverEx@@UEAAHPEAUtagACTION_BUFFER@@@Z @ 0x140391300
        // is the blocked 30+ branch action dispatcher; lands when its trigger/script/vtable deps are restored.
        m_xActionBuffer.Delete(byIndex);
    }
}

// IDA: ?CreateAkashicActionInfo@CMover@@QEAAXPEBD@Z (0x14036C800)
// IDA exact restoration - creates Akashic action info
void CMover::CreateAkashicActionInfo(const char* szAnimName) {
    if (!m_pActionResource) {
        return;
    }

    if (m_pAkashicActionInfo) {
        // IDA: Clear existing triggers
        m_pAkashicActionInfo->arTriggers.RemoveAll();

        // IDA: Compare names
        if (strcmp(m_pAkashicActionInfo->szName, szAnimName) != 0) {
            // IDA: Get new action desc and copy
            XGameServer* pServer = XGameServer::Instance();
            if (pServer) {
                const VAnimationInfo* pAnimInfo = pServer->GetActionManager().GetActionDesc(m_pActionResource, szAnimName);
                if (pAnimInfo) {
                    m_pAkashicActionInfo->arOffsetDeltaFrames.RemoveAll();
                    m_pAkashicActionInfo->arTranslationFrames.RemoveAll();
                    // IDA: CopyData
                    m_pAkashicActionInfo->CopyData(const_cast<VAnimationInfo&>(*pAnimInfo));
                }
            }
        }
    } else {
        // IDA: Create new Akashic action info
        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            const VAnimationInfo* pAnimInfo = pServer->GetActionManager().GetActionDesc(m_pActionResource, szAnimName);
            if (pAnimInfo) {
                m_pAkashicActionInfo = new VAnimationInfo();
                // IDA: CopyData
                m_pAkashicActionInfo->CopyData(const_cast<VAnimationInfo&>(*pAnimInfo));
            }
        }
    }
}

// RunSectorAI is implemented in Maze.cpp