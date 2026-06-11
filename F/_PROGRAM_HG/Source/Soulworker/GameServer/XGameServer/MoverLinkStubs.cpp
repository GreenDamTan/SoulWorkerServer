#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XGameServer/ActionResMgr.h"
#include "Soulworker/GameServer/XGameServer/MySkillList.h"
#include "Soulworker/GameServer/XGameServer/VChainLightningObject.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#undef PS_ITEM_PACKAGE_LIST

int CItem::GetID() const
{
    return m_stItem.nItemID;
}

// Note: ApplySkillDamageFrame, GetSGAbsorbRate, FindBuffStatus,
// send_eSUB_CMD_ACTION_SKILL, and send_eSUB_CMD_CHAIN
// are now implemented in Mover.cpp

float CMover::GetMaxSuperArmorGage()
{
    return m_fMaxSuperArmorGage;
}

float CMover::GetCurSuperArmorGage()
{
    return m_fCurSuperArmorGage;
}

void CMoverEx::UpdateSkillAnimInfo(TB_SKILL* pSkillTableRef)
{
    m_pCurSkillTableRef = pSkillTableRef;
    m_bySkillAnimStep = 0;
    m_bySkillAnimCount = 1;
}

const char* CMoverEx::GetSkillAnimName(TB_SKILL* pSkillTableRef, std::uint8_t byStep)
{
    (void)pSkillTableRef;
    (void)byStep;
    return "";
}

int CMoverEx::GetRandomDamage(std::uint8_t bySkillAttribute, int iItemRateResult)
{
    (void)bySkillAttribute;
    return iItemRateResult;
}

bool CMoverEx::IsExceptionalDamage()
{
    return false;
}

void CMoverEx::CheckPassiveSkillByHit(CMoverEx* pMover, TB_SKILL* pSkillTable, std::uint8_t byResult)
{
    (void)pMover;
    (void)pSkillTable;
    (void)byResult;
}

void CMoverEx::NotifyPhaseChanged(std::uint8_t byOldPhase)
{
    (void)byOldPhase;
}

void CMoverEx::SetBuffAbility(int nIndex, float fValue)
{
    (void)nIndex;
    (void)fValue;
}

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

STCasualRaidTime::STCasualRaidTime()
    : nIntValue(0)
    , fFloatValue(0.0f)
    , fWaitSendTime(0.0f)
{
}

void XMaze::RunSectorAI(int nSector, bool bIsPotal)
{
    (void)nSector;
    (void)bIsPotal;
}