// GreenDamTan_GocNpcCreditLink.cpp
// 中文说明：
// 1. 本文件为人工补充承载层：GocNpcCredit.cpp 原文件目前是历史遗留半成品
//    (GetXResourceMgr 误名、XSendDBPacket 构造半成品、IXObject 歧义转换)，
//    暂未达到编译标准，故按 GreenDamTan_ 规则以本文件承载
//    ReqWorldDistrictTransport (lambda22) 调用链所需的两个方法。
// 2. UpdateNpcCredit(uint32,int) 与 GetNpcCreditBenefit(uint32,uint8) 均按
//    IDA 反编译 (0x140105640 / 0x1401063D0) 精确还原，非推测逻辑。
// 3. 待 GocNpcCredit.cpp 主体修复后，按 PDB OBJ gocnpccredit.obj 归属
//    把这两段收敛回原文件，届时删除本文件。

#include "GocNpcCredit.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAchieve.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocPost.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

#include <algorithm>

// ============================================================================
// CGocNpcCredit::GetNpcCreditBenefit (0x1401063D0)
// 已精确还原 - 查询 NPC 组当前信用等级对应 Benefit_Type 的加成率。
// 逻辑: m_bEnable 门 -> GetTB_NPC -> m_mpNpcCredit 查组 -> byGrade 为 0 返回
// 0 -> GetTB_CUSTOMER_GRADE(10*GroupID + byGrade) -> 遍历 Benefit_ID_01..06
// 找 Benefit_Type == byBenefit 的 TB_CUSTOMER_BENEFIT 返回 Benefit_Value_Rate。
// ============================================================================
std::int32_t CGocNpcCredit::GetNpcCreditBenefit(std::uint32_t nNpcID,
                                                std::uint8_t byBenefit) {
    if (!m_bEnable)
        return 0;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_NPC* pTBNPC = pGameServer->GetResourceMgr().GetTB_NPC(nNpcID);
    if (!pTBNPC)
        return 0;

    auto iter = m_mpNpcCredit.find(pTBNPC->Npc_Group_ID);
    if (iter == m_mpNpcCredit.end())
        return 0;

    PS_NPC_CREDIT stNpcCredit = iter->second;
    if (!stNpcCredit.byGrade)
        return 0;

    int nIndex = stNpcCredit.byGrade + 10 * pTBNPC->Npc_Group_ID;
    TB_CUSTOMER_GRADE* pTBCustomerGrade =
        pGameServer->GetResourceMgr().GetTB_CUSTOMER_GRADE(nIndex);
    if (!pTBCustomerGrade)
        return 0;

    // Per IDA: Benefit_ID_01..06 连续数组遍历 (i<6 且非 0)
    for (int i = 0; i < 6; ++i) {
        std::int32_t nBenefitID = *(&pTBCustomerGrade->Benefit_ID_01 + i);
        if (!nBenefitID)
            break;
        TB_CUSTOMER_BENEFIT* pTBCreditBenefit =
            pGameServer->GetResourceMgr().GetTB_CUSTOMER_BENEFIT(nBenefitID);
        if (pTBCreditBenefit && pTBCreditBenefit->Benefit_Type == byBenefit)
            return pTBCreditBenefit->Benefit_Value_Rate;
    }
    return 0;
}

// ============================================================================
// CGocNpcCredit::UpdateNpcCredit (0x140105640)
// 已精确还原 - 提升玩家对 NPC 组的信用点并落库。
// 逻辑: m_bEnable 门 -> GetTB_NPC -> 内部 5 参 UpdateNpcCredit 更新 ->
// 成功: (0x22,0x11) DB 包 (QuestID + stNpcCredit + nPoint + nGetPoint) ->
// CGocAchieve::UpdateAchieve1(0x39, nGetPoint, nGroupID) ->
// ST_LOG_GAME (3,14) SendDBLog -> 遍历 stGradeItem.vecInfo 逐项
// SystemPostSend(itemID, count, 1, NpcGroupID, nNpcID)。
// ============================================================================
void CGocNpcCredit::UpdateNpcCredit(std::uint32_t nNpcID, std::int32_t nPoint) {
    if (!m_bEnable)
        return;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    TB_NPC* pTBNPC = pGameServer->GetResourceMgr().GetTB_NPC(nNpcID);
    if (!pTBNPC)
        return;

    PS_NPC_CREDIT stNpcCredit{};
    ST_CREATE_ITEMS stGradeItem{};
    std::int32_t nGetPoint = 0;

    stNpcCredit.nGroupID = pTBNPC->Npc_Group_ID;

    if (UpdateNpcCredit(pTBNPC->Npc_Group_ID, nPoint, &stNpcCredit,
                        &stGradeItem, &nGetPoint)) {
        CMover* pMover = GetOwnerGO();
        CUser* pUser = dynamic_cast<CUser*>(pMover);
        if (pUser) {
            // Per IDA: pObject 为 owner 的 IXObject 基指针 (歧义转换需显式走 XActor)
            XSendDBPacket xSendDBPacket(
                static_cast<IXObject*>(static_cast<XActor*>(pUser)),
                0x22, 0x11);
            xSendDBPacket.XParse << CQuestCondition::GetQuestID(
                pUser->GetActorID());
            xSendDBPacket << stNpcCredit;
            xSendDBPacket.XParse << nPoint;
            xSendDBPacket.XParse << nGetPoint;
            pGameServer->SendDBGame(xSendDBPacket);

            CGocAchieve* pAchieve = pUser->GetGOC<CGocAchieve>();
            if (pAchieve)
                pAchieve->UpdateAchieve1(0x39, nGetPoint,
                                         stNpcCredit.nGroupID);

            ST_LOG_GAME stLog{};
            stLog._nUAID = static_cast<int>(pUser->GetUAID());
            stLog._nUCID = CQuestCondition::GetQuestID(pUser->GetActorID());
            stLog._sMainType = 3;
            stLog._sSubType = 14;
            stLog.nParam0 = static_cast<int>(nNpcID);
            stLog.nParam1 = stNpcCredit.byGrade;
            stLog.nParam2 = stNpcCredit.nPoint;
            stLog.nParam3 = stNpcCredit.shCurPoint;
            stLog.nParam4 = nPoint;
            stLog.nParam5 = pUser->GetLevel();
            pGameServer->SendDBLog(stLog);

            CGocPost* pPost = pUser->GetGOC<CGocPost>();
            for (const auto& stItem : stGradeItem.vecInfo) {
                if (pPost)
                    pPost->SystemPostSend(
                        static_cast<std::uint32_t>(stItem.nItemID),
                        stItem.shCount, 1,
                        static_cast<std::uint16_t>(pTBNPC->Npc_Group_ID),
                        nNpcID);
            }
        }
    }
}

// ============================================================================
// CGocNpcCredit::UpdateNpcCredit 5 参版 (0x140105B30)
// 已精确还原 - 信用点累计与升级判定核心。
// 逻辑: nPoint<1 返回假 -> owner RTTI 转 CUser -> IsLeagueSkill(5) 时取
// GetLeagueSkillEffectValue(5) 为加成 -> m_bEnable 门 ->
// m_mpNpcCredit 无条目: GetTB_CUSTOMER_GRADE(10*GroupID+1)，日上限截断后
// 按 Need_Credit 循环升级、Reward_Item 逐级入 stGradeItem，无下一级时
// 封顶 byGrade=5 写入并返回真；
// 有条目: byGrade==5 返回假 -> 下一级表 -> 日上限/跨日重置逻辑 ->
// 升级循环同上（点数累计从 stUpdateInfo->nPoint 续算）。
// ============================================================================
bool CGocNpcCredit::UpdateNpcCredit(std::int32_t nGroupID, std::int32_t nPoint,
                                    PS_NPC_CREDIT* stUpdateInfo,
                                    ST_CREATE_ITEMS* stGradeItem,
                                    std::int32_t* nGetPoint) {
    if (nPoint < 1)
        return false;

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
        return false;

    int nValue = 0;
    if (pUser->IsLeagueSkill(5))
        nValue = pUser->GetLeagueSkillEffectValue(5);

    if (!m_bEnable)
        return false;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    int nRemainPoint = nPoint;
    auto iter = m_mpNpcCredit.find(nGroupID);

    if (iter == m_mpNpcCredit.end()) {
        // Per IDA: 新条目 - 从 1 级表开始
        int nIndex = 10 * nGroupID + 1;
        TB_CUSTOMER_GRADE* pTBGrade =
            pGameServer->GetResourceMgr().GetTB_CUSTOMER_GRADE(nIndex);
        if (!pTBGrade)
            return false;

        int nDayLimit = nValue + pTBGrade->Day_Credit_Limit;
        if (nDayLimit < nPoint)
            nRemainPoint = nDayLimit;
        stUpdateInfo->shCurPoint = static_cast<std::int16_t>(nRemainPoint);
        *nGetPoint = nRemainPoint;

        while (pTBGrade) {
            if (pTBGrade->Need_Credit > nRemainPoint) {
                stUpdateInfo->nUpdateDate = pGameServer->GetCurDate();
                stUpdateInfo->byGrade =
                    static_cast<std::uint8_t>(pTBGrade->Customer_Grade - 1);
                stUpdateInfo->nPoint = nRemainPoint;
                m_mpNpcCredit[nGroupID] = *stUpdateInfo;
                return true;
            }
            ST_CREATE_ITEM stItem{};
            stItem.nItemID = pTBGrade->Reward_Item;
            stItem.shCount = pTBGrade->Reward_Item_Count;
            stGradeItem->vecInfo.push_back(stItem);

            int nNextIndex = 10 * nGroupID + pTBGrade->Customer_Grade + 1;
            TB_CUSTOMER_GRADE* pTBNextGrade =
                pGameServer->GetResourceMgr().GetTB_CUSTOMER_GRADE(nNextIndex);
            if (!pTBNextGrade) {
                // Per IDA: 无更高等级 - 封顶 5 级清零写入
                stUpdateInfo->nUpdateDate = pGameServer->GetCurDate();
                stUpdateInfo->byGrade = 5;
                stUpdateInfo->nPoint = 0;
                stUpdateInfo->shCurPoint = 0;
                m_mpNpcCredit[nGroupID] = *stUpdateInfo;
                return true;
            }
            nRemainPoint -= pTBGrade->Need_Credit;
            if (nRemainPoint < 0)
                return false;
            pTBGrade = pTBNextGrade;
        }
        return false;
    }

    // Per IDA: 已有条目
    *stUpdateInfo = iter->second;
    if (stUpdateInfo->byGrade == 5)
        return false;

    int nIndex = 10 * nGroupID + stUpdateInfo->byGrade + 1;
    TB_CUSTOMER_GRADE* pTBCustomerGrade =
        pGameServer->GetResourceMgr().GetTB_CUSTOMER_GRADE(nIndex);
    if (!pTBCustomerGrade)
        return false;

    int nDayLimit = nValue + pTBCustomerGrade->Day_Credit_Limit;
    if (stUpdateInfo->nUpdateDate >= pGameServer->GetBeforeInitDate()) {
        if (nDayLimit <= stUpdateInfo->shCurPoint)
            return false;
    } else {
        stUpdateInfo->shCurPoint = 0;
    }

    std::int16_t shCurPoint = stUpdateInfo->shCurPoint;
    stUpdateInfo->shCurPoint = static_cast<std::int16_t>(
        stUpdateInfo->shCurPoint + nRemainPoint);
    if (stUpdateInfo->shCurPoint > nDayLimit) {
        nRemainPoint = nDayLimit - shCurPoint;
        stUpdateInfo->shCurPoint = static_cast<std::int16_t>(nDayLimit);
    }
    *nGetPoint = nRemainPoint;

    while (pTBCustomerGrade) {
        if (pTBCustomerGrade->Need_Credit > nRemainPoint + stUpdateInfo->nPoint) {
            stUpdateInfo->nUpdateDate = pGameServer->GetCurDate();
            stUpdateInfo->byGrade =
                static_cast<std::uint8_t>(pTBCustomerGrade->Customer_Grade - 1);
            stUpdateInfo->nPoint += nRemainPoint;
            iter->second = *stUpdateInfo;
            return true;
        }
        ST_CREATE_ITEM stItem{};
        stItem.nItemID = pTBCustomerGrade->Reward_Item;
        stItem.shCount = pTBCustomerGrade->Reward_Item_Count;
        stGradeItem->vecInfo.push_back(stItem);

        int nNextIndex = 10 * nGroupID + pTBCustomerGrade->Customer_Grade + 1;
        TB_CUSTOMER_GRADE* pTBNextGrade =
            pGameServer->GetResourceMgr().GetTB_CUSTOMER_GRADE(nNextIndex);
        if (!pTBNextGrade) {
            stUpdateInfo->nUpdateDate = pGameServer->GetCurDate();
            stUpdateInfo->byGrade = 5;
            stUpdateInfo->nPoint = 0;
            stUpdateInfo->shCurPoint = 0;
            iter->second = *stUpdateInfo;
            return true;
        }
        nRemainPoint = nRemainPoint + stUpdateInfo->nPoint
            - pTBCustomerGrade->Need_Credit;
        stUpdateInfo->nPoint = 0;
        if (nRemainPoint < 0)
            return false;
        pTBCustomerGrade = pTBNextGrade;
    }
    return false;
}
