#include "Soulworker/GameServer/XGameServer/MoverEx.h"
#include "Soulworker/GameServer/XGameServer/MySkillList.h"
#include "Soulworker/GameServer/XGameServer/ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/Maze.h"
#include "Soulworker/GameServer/XGameServer/Item/CItem.h"
#undef PS_ITEM_PACKAGE_LIST

int CItem::GetID() const
{
    return m_stItem.nItemID;
}

void CMover::ApplySkillDamageFrame(int nSkillID, std::int16_t nTriggerIdx,
                                   std::uint8_t byAttackTargetCnt, hkvVec3* vPos,
                                   float fDirYaw, std::uint16_t wContinousHit,
                                   bool byDamageType, bool bPenetrate)
{
    (void)nSkillID;
    (void)nTriggerIdx;
    (void)byAttackTargetCnt;
    (void)vPos;
    (void)fDirYaw;
    (void)wContinousHit;
    (void)byDamageType;
    (void)bPenetrate;
}

float CMover::GetMaxSuperArmorGage()
{
    return m_fMaxSuperArmorGage;
}

float CMover::GetCurSuperArmorGage()
{
    return m_fCurSuperArmorGage;
}

float CMover::GetSGAbsorbRate()
{
    return m_fSkillAbsorbSGRate;
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
int CMover::FindBuffStatus(std::uint16_t nBuffIndex, std::uint32_t dwAttackerID)
{
    (void)nBuffIndex;
    (void)dwAttackerID;
    return -1;
}

void CMover::send_eSUB_CMD_ACTION_SKILL(CMover* pMover, std::uint32_t nSkillID,
                                        std::int16_t nTriggerIdx, hkvVec3* vPos,
                                        std::uint8_t byAttackTargetCnt,
                                        std::uint16_t wContinousHit, bool bPenetrate)
{
    (void)pMover;
    (void)nSkillID;
    (void)nTriggerIdx;
    (void)vPos;
    (void)byAttackTargetCnt;
    (void)wContinousHit;
    (void)bPenetrate;
}

void CMover::send_eSUB_CMD_CHAIN(CMover* pMover, std::uint32_t nSkillID,
                                 std::int16_t nTriggerIdx, hkvVec3* vPos, hkvVec3* vDir,
                                 std::uint32_t nSessionID, std::uint32_t dwTargetID)
{
    (void)pMover;
    (void)nSkillID;
    (void)nTriggerIdx;
    (void)vPos;
    (void)vDir;
    (void)nSessionID;
    (void)dwTargetID;
}

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