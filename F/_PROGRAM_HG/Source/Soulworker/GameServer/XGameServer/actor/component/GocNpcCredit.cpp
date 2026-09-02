// GocNpcCredit.cpp - NPC Credit Component Implementation
// Restored from IDA GameServer.exe

#include "GocNpcCredit.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XGameServer/User.h"
#include "Soulworker/GameServer/XGameServer/Mover.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocPost.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocAchieve.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include <algorithm>

// ============================================================================
// Constructor / Destructor
// ============================================================================

// IDA: ??0CGocNpcCredit@@QEAA@XZ (0x140105260)
CGocNpcCredit::CGocNpcCredit()
    : GOComponent()
    , m_dw64WaitTick(0)
    , m_bEnable(false)
{
    // Maps are default-constructed
}

// IDA: ??1CGocNpcCredit@@UEAA@XZ (0x140105310)
CGocNpcCredit::~CGocNpcCredit()
{
    // Maps are automatically destroyed
}

// ============================================================================
// Initialization
// ============================================================================

// IDA: ?Init@CGocNpcCredit@@QEAAXXZ (0x140105380)
void CGocNpcCredit::Init()
{
    m_mpNpcCredit.clear();
    m_mpShopItem.clear();
    m_mpShopAccountItem.clear();
    m_dw64WaitTick = 0;
    m_bEnable = false;
}

// IDA: ?OnUpdate@CGocNpcCredit@@QEAAXXZ (0x1401053E0)
void CGocNpcCredit::OnUpdate()
{
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    // Check if user has NPC credit feature enabled
    CUser* pUser = nullptr; // TODO: RTTI cast from pMover
    if (!pUser) {
        return;
    }

    // Check user flags for NPC credit (0x20 flag) and shop (0x40 flag)
    // TODO: Access STMyCharInfoEx to check flags
    // if ((pUser->GetUserFlags() & 0x20) == 0) return;
    // if ((pUser->GetUserFlags() & 0x40) == 0) return;

    // Check wait tick timing
    if (m_dw64WaitTick >= GetTickCount64()) {
        return;
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return;
    }

    std::uint64_t dwInitTick = pGameServer->GetInitTick();
    if (m_dw64WaitTick < dwInitTick) {
        if (m_dw64WaitTick != 0) {
            m_dw64WaitTick = dwInitTick;
            OnInitShopItem();
            OnInitNpcCredit();
        } else {
            m_dw64WaitTick = dwInitTick;
        }
    }
}

// ============================================================================
// NPC Credit Management
// ============================================================================

// IDA: ?SetNpcCredit@CGocNpcCredit@@QEAAXUPS_NPC_CREDIT_LIST@@@Z (0x140105500)
void CGocNpcCredit::SetNpcCredit(PS_NPC_CREDIT_LIST& stNpcCreditList)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return;
    }

    std::int64_t biCurDate = pGameServer->GetCurDate();
    std::int64_t biBeforeDate = pGameServer->GetBeforeInitDate();

    m_bEnable = true;

    for (auto& stNpcCredit : stNpcCreditList.vecInfo) {
        // Check if update date is before server init date
        if (stNpcCredit.nUpdateDate < biBeforeDate) {
            stNpcCredit.nUpdateDate = biCurDate;
            stNpcCredit.shCurPoint = 0;
        }

        // Insert into map
        m_mpNpcCredit[stNpcCredit.nGroupID] = stNpcCredit;
    }
}

// IDA: ?UpdateNpcCredit@CGocNpcCredit@@QEAAXHH@Z (0x140105640)
void CGocNpcCredit::UpdateNpcCredit(std::uint32_t nNpcID, std::int32_t nPoint)
{
    if (!m_bEnable) {
        return;
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return;
    }

    TB_NPC* pTBNPC = pGameServer->GetXResourceMgr().GetTB_NPC(nNpcID);
    if (!pTBNPC) {
        return;
    }

    PS_NPC_CREDIT stNpcCredit = {};
    ST_CREATE_ITEMS stGradeItem = {}; // TODO: This should be a vector
    std::int32_t nGetPoint = 0;

    stNpcCredit.nGroupID = pTBNPC->Npc_Group_ID;

    if (UpdateNpcCredit(pTBNPC->Npc_Group_ID, nPoint, &stNpcCredit, &stGradeItem, &nGetPoint)) {
        CMover* pMover = GetOwnerGO();
        if (!pMover) {
            return;
        }

        // Send DB update packet
        CUser* pUser = nullptr; // TODO: RTTI cast
        XSendDBPacket xSendDBPacket(pUser, 0x22, 0x11);
        xSendDBPacket.XParse << pUser->GetUCID(); // TODO: Get correct ID
        xSendDBPacket << stNpcCredit;
        xSendDBPacket.XParse << nPoint;
        xSendDBPacket.XParse << nGetPoint;
        pGameServer->SendDBGame(&xSendDBPacket);

        // Update achievement
        // TODO: GetGOC<CGocAchieve> and call UpdateAchieve1

        // Log
        // TODO: Send ST_LOG_GAME

        // Send post for grade rewards
        // TODO: Iterate stGradeItem and send SystemPostSend
    }
}

// IDA: ?UpdateNpcCredit@CGocNpcCredit@@QEAA_NHHAEAUPS_NPC_CREDIT@@AEAUST_CREATE_ITEMS@@AEAH@Z (0x140105B30)
bool CGocNpcCredit::UpdateNpcCredit(std::int32_t nGroupID, std::int32_t nPoint,
                                     PS_NPC_CREDIT* stUpdateInfo, ST_CREATE_ITEMS* stGradeItem,
                                     std::int32_t* nGetPoint)
{
    if (nPoint < 1) {
        return false;
    }

    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return false;
    }

    // TODO: RTTI cast to CUser
    CUser* pUser = nullptr;
    if (!pUser) {
        return false;
    }

    // Check league skill bonus
    std::int32_t nValue = 0;
    // TODO: if (pUser->IsLeagueSkill(5)) nValue = pUser->GetLeagueSkillEffectValue(5);

    if (!m_bEnable) {
        return false;
    }

    std::int32_t nRemainPoint = nPoint;
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return false;
    }

    // Find existing credit entry
    auto it = m_mpNpcCredit.find(nGroupID);

    if (it == m_mpNpcCredit.end()) {
        // New entry - start from grade 1
        std::int32_t nIndex = 10 * nGroupID + 1;
        TB_CUSTOMER_GRADE* pTBCustomerGrade = pGameServer->GetXResourceMgr().GetTB_CUSTOMER_GRADE(nIndex);

        if (!pTBCustomerGrade) {
            return false;
        }

        std::int32_t nDayLimit = nValue + pTBCustomerGrade->Day_Credit_Limit;
        if (nDayLimit < nPoint) {
            nRemainPoint = nDayLimit;
        }

        stUpdateInfo->shCurPoint = static_cast<std::int16_t>(nRemainPoint);
        *nGetPoint = nRemainPoint;

        // Process credit grades
        while (pTBCustomerGrade) {
            if (pTBCustomerGrade->Need_Credit > nRemainPoint) {
                stUpdateInfo->nUpdateDate = pGameServer->GetCurDate();
                stUpdateInfo->byGrade = pTBCustomerGrade->Customer_Grade - 1;
                stUpdateInfo->nPoint = nRemainPoint;
                m_mpNpcCredit[nGroupID] = *stUpdateInfo;
                return true;
            }

            // Add grade reward item
            // TODO: Push reward item to stGradeItem
            // stGradeItem.nItemID = pTBCustomerGrade->Reward_Item;
            // stGradeItem.shCount = pTBCustomerGrade->Reward_Item_Count;

            // Get next grade
            std::int32_t nNextIndex = 10 * nGroupID + pTBCustomerGrade->Customer_Grade + 1;
            TB_CUSTOMER_GRADE* pTBNextGrade = pGameServer->GetXResourceMgr().GetTB_CUSTOMER_GRADE(nNextIndex);

            if (!pTBNextGrade) {
                // Max grade reached
                stUpdateInfo->nUpdateDate = pGameServer->GetCurDate();
                stUpdateInfo->byGrade = 5;
                stUpdateInfo->nPoint = 0;
                stUpdateInfo->shCurPoint = 0;
                m_mpNpcCredit[nGroupID] = *stUpdateInfo;
                return true;
            }

            nRemainPoint -= pTBCustomerGrade->Need_Credit;
            if (nRemainPoint < 0) {
                return false;
            }

            pTBCustomerGrade = pTBNextGrade;
        }

        return false;
    } else {
        // Existing entry
        *stUpdateInfo = it->second;

        if (stUpdateInfo->byGrade == 5) {
            return false; // Max grade
        }

        std::int32_t nIndex = 10 * nGroupID + stUpdateInfo->byGrade + 1;
        TB_CUSTOMER_GRADE* pTBCustomerGrade = pGameServer->GetXResourceMgr().GetTB_CUSTOMER_GRADE(nIndex);

        if (!pTBCustomerGrade) {
            return false;
        }

        std::int32_t nDayLimit = nValue + pTBCustomerGrade->Day_Credit_Limit;
        std::int64_t biBeforeDate = pGameServer->GetBeforeInitDate();

        // Check if we need to reset daily points
        if (stUpdateInfo->nUpdateDate >= biBeforeDate) {
            if (nDayLimit <= stUpdateInfo->shCurPoint) {
                return false; // Daily limit reached
            }
        } else {
            stUpdateInfo->shCurPoint = 0;
        }

        std::int16_t shCurPoint = stUpdateInfo->shCurPoint;
        stUpdateInfo->shCurPoint += static_cast<std::int16_t>(nRemainPoint);

        if (stUpdateInfo->shCurPoint > nDayLimit) {
            nRemainPoint = nDayLimit - shCurPoint;
            stUpdateInfo->shCurPoint = static_cast<std::int16_t>(nDayLimit);
        }

        *nGetPoint = nRemainPoint;

        // Process credit grades
        while (pTBCustomerGrade) {
            if (pTBCustomerGrade->Need_Credit > nRemainPoint + stUpdateInfo->nPoint) {
                stUpdateInfo->nUpdateDate = pGameServer->GetCurDate();
                stUpdateInfo->byGrade = pTBCustomerGrade->Customer_Grade - 1;
                stUpdateInfo->nPoint += nRemainPoint;
                it->second = *stUpdateInfo;
                return true;
            }

            // Add grade reward item
            // TODO: Push reward item

            // Get next grade
            std::int32_t nNextIndex = 10 * nGroupID + pTBCustomerGrade->Customer_Grade + 1;
            TB_CUSTOMER_GRADE* pTBNextGrade = pGameServer->GetXResourceMgr().GetTB_CUSTOMER_GRADE(nNextIndex);

            if (!pTBNextGrade) {
                // Max grade reached
                stUpdateInfo->nUpdateDate = pGameServer->GetCurDate();
                stUpdateInfo->byGrade = 5;
                stUpdateInfo->nPoint = 0;
                stUpdateInfo->shCurPoint = 0;
                it->second = *stUpdateInfo;
                return true;
            }

            nRemainPoint = nRemainPoint + stUpdateInfo->nPoint - pTBCustomerGrade->Need_Credit;
            stUpdateInfo->nPoint = 0;

            if (nRemainPoint < 0) {
                return false;
            }

            pTBCustomerGrade = pTBNextGrade;
        }

        return false;
    }
}

// IDA: ?GetNpcCreditGrade@CGocNpcCredit@@QEAADH@Z (0x140106330)
std::int8_t CGocNpcCredit::GetNpcCreditGrade(std::uint32_t nNpcID)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return -1;
    }

    TB_NPC* pTBNPC = pGameServer->GetXResourceMgr().GetTB_NPC(nNpcID);
    if (!pTBNPC) {
        return -1;
    }

    auto it = m_mpNpcCredit.find(pTBNPC->Npc_Group_ID);
    if (it == m_mpNpcCredit.end()) {
        return 0;
    }

    return static_cast<std::int8_t>(it->second.byGrade);
}

// IDA: ?GetNpcCreditBenefit@CGocNpcCredit@@QEAAHHE@Z (0x1401063D0)
std::int32_t CGocNpcCredit::GetNpcCreditBenefit(std::uint32_t nNpcID, std::uint8_t byBenefit)
{
    if (!m_bEnable) {
        return 0;
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return 0;
    }

    TB_NPC* pTBNPC = pGameServer->GetXResourceMgr().GetTB_NPC(nNpcID);
    if (!pTBNPC) {
        return 0;
    }

    auto it = m_mpNpcCredit.find(pTBNPC->Npc_Group_ID);
    if (it == m_mpNpcCredit.end()) {
        return 0;
    }

    const PS_NPC_CREDIT& stNpcCredit = it->second;
    if (stNpcCredit.byGrade == 0) {
        return 0;
    }

    std::int32_t nIndex = stNpcCredit.byGrade + 10 * pTBNPC->Npc_Group_ID;
    TB_CUSTOMER_GRADE* pTBCustomerGrade = pGameServer->GetXResourceMgr().GetTB_CUSTOMER_GRADE(nIndex);
    if (!pTBCustomerGrade) {
        return 0;
    }

    // Check up to 6 benefit IDs
    for (std::int32_t i = 0; i < 6; ++i) {
        std::int32_t nBenefitID = (&pTBCustomerGrade->Benefit_ID_01)[i];
        if (nBenefitID == 0) {
            break;
        }

        TB_CUSTOMER_BENEFIT* pTBCreditBenefit = pGameServer->GetXResourceMgr().GetTB_CUSTOMER_BENEFIT(nBenefitID);
        if (pTBCreditBenefit && pTBCreditBenefit->Benefit_Type == byBenefit) {
            return pTBCreditBenefit->Benefit_Value_Rate;
        }
    }

    return 0;
}

// IDA: ?CanNpcCreditBenefit@CGocNpcCredit@@QEAA_NHEK@Z (0x1401065C0)
bool CGocNpcCredit::CanNpcCreditBenefit(std::uint32_t nNpcID, std::uint8_t byBenefit, std::uint32_t dwItemID)
{
    if (!m_bEnable) {
        return false;
    }

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return false;
    }

    TB_NPC* pTBNPC = pGameServer->GetXResourceMgr().GetTB_NPC(nNpcID);
    if (!pTBNPC) {
        return false;
    }

    auto it = m_mpNpcCredit.find(pTBNPC->Npc_Group_ID);
    if (it == m_mpNpcCredit.end()) {
        return false;
    }

    const PS_NPC_CREDIT& stNpcCredit = it->second;
    if (stNpcCredit.byGrade == 0) {
        return false;
    }

    std::int32_t nIndex = stNpcCredit.byGrade + 10 * pTBNPC->Npc_Group_ID;
    TB_CUSTOMER_GRADE* pTBCustomerGrade = pGameServer->GetXResourceMgr().GetTB_CUSTOMER_GRADE(nIndex);
    if (!pTBCustomerGrade) {
        return false;
    }

    // Check up to 6 benefit IDs
    for (std::int32_t i = 0; i < 6; ++i) {
        std::int32_t nBenefitID = (&pTBCustomerGrade->Benefit_ID_01)[i];
        if (nBenefitID == 0) {
            break;
        }

        TB_CUSTOMER_BENEFIT* pTBCreditBenefit = pGameServer->GetXResourceMgr().GetTB_CUSTOMER_BENEFIT(nBenefitID);
        if (pTBCreditBenefit && pTBCreditBenefit->Benefit_Type == byBenefit) {
            if (dwItemID == 0) {
                return true;
            }

            // Check if item matches any of the 3 benefit item IDs
            for (std::int32_t k = 0; k < 3; ++k) {
                if (dwItemID == (&pTBCreditBenefit->Benefit_Item_ID_01)[k]) {
                    return true;
                }
            }
        }
    }

    return false;
}

// ============================================================================
// Database Operations
// ============================================================================

// IDA: ?SendDBNpcCreditLoad@CGocNpcCredit@@QEAAXXZ (0x140106220)
void CGocNpcCredit::SendDBNpcCreditLoad()
{
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    // TODO: Get CUser from mover
    CUser* pUser = nullptr; // RTTI cast
    if (!pUser) {
        return;
    }

    XSendDBPacket xSendDBPacket(pUser, 0x22, 0x10);
    xSendDBPacket.XParse << pUser->GetUCID();

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer) {
        pGameServer->SendDBGame(&xSendDBPacket);
    }
}

// IDA: ?SendDBShopItemLoad@CGocNpcCredit@@QEAAXXZ (0x140106800)
void CGocNpcCredit::SendDBShopItemLoad()
{
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    // TODO: RTTI cast to CUser
    CUser* pUser = nullptr;
    if (!pUser) {
        return;
    }

    // Check user flags for shop feature
    // if ((pUser->GetUserFlags() & 0x40) == 0) return;

    XSendDBPacket xSendDBPacket(pUser, 0x22, 0x13);
    xSendDBPacket.XParse << pUser->GetUCID();
    xSendDBPacket.XParse << pUser->GetUAID();

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (pGameServer) {
        pGameServer->SendDBGame(&xSendDBPacket);
    }
}

// ============================================================================
// Shop Item Management
// ============================================================================

// IDA: ?SetShopItem@CGocNpcCredit@@QEAAXUST_SHOP_ITEM_LIST@@@Z (0x140106A40)
void CGocNpcCredit::SetShopItem(ST_SHOP_ITEM_LIST& stShopItemList)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return;
    }

    std::int64_t biCurDate = pGameServer->GetCurDate();

    for (auto& stInfo : stShopItemList.vecInfo) {
        // Reset if expired
        if (stInfo.nUpdateDate <= biCurDate) {
            stInfo.shCount = 0;
            stInfo.nUpdateDate = 0;
        }

        // Insert with key (nShopIndex, nItemID)
        auto key = std::make_pair(static_cast<std::int32_t>(stInfo.nShopIndex),
                                   static_cast<std::int32_t>(stInfo.nItemID));
        m_mpShopItem[key] = stInfo;
    }
}

// IDA: ?SetShopAccountItem@CGocNpcCredit@@QEAAXUST_SHOP_ITEM_LIST@@@Z (0x140106C00)
void CGocNpcCredit::SetShopAccountItem(ST_SHOP_ITEM_LIST& stShopAccountItemList)
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return;
    }

    std::int64_t biCurDate = pGameServer->GetCurDate();

    for (auto& stInfo : stShopAccountItemList.vecInfo) {
        // Reset if expired
        if (stInfo.nUpdateDate <= biCurDate) {
            stInfo.shCount = 0;
            stInfo.nUpdateDate = 0;
        }

        // Insert with key (nShopIndex, nItemID)
        auto key = std::make_pair(static_cast<std::int32_t>(stInfo.nShopIndex),
                                   static_cast<std::int32_t>(stInfo.nItemID));
        m_mpShopAccountItem[key] = stInfo;
    }
}

// IDA: ?UpdateShopItem@CGocNpcCredit@@QEAA_NHHE_NH@Z (0x140106CC0)
bool CGocNpcCredit::UpdateShopItem(std::uint32_t nNpcID, std::uint32_t nShopIndex,
                                    std::uint8_t byGrade, bool bAddRate, std::int16_t nBuyCount)
{
    // Get owner user
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return false;
    }

    // TODO: RTTI cast to CUser
    CUser* pUser = nullptr;
    // if (pMover) pUser = pMover->GetUser();

    // Check user flags for shop feature (0x40)
    // if ((*((_BYTE *)&CUser::stMyCharInfoEx(pUser)->UserDB + 2) & 0x40) == 0) return false;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return false;
    }

    // Get NPC table entry
    TB_NPC* pTBNPC = pGameServer->GetXResourceMgr().GetTB_NPC(nNpcID);
    if (!pTBNPC) {
        return false;
    }

    // Get Shop table entry
    TB_SHOP* pTBShop = pGameServer->GetXResourceMgr().GetTB_SHOP(nShopIndex);
    if (!pTBShop) {
        return false;
    }

    // Check grade requirement
    if (byGrade < pTBShop->Customer_Grade) {
        return false;
    }

    // Get period type and end date
    E_SHOP_PERIOD_TYPE ePeriodType = static_cast<E_SHOP_PERIOD_TYPE>(pTBShop->Period_Type);
    std::int64_t biEndDate = 0;
    GetShopItemUpdateDate(ePeriodType, biEndDate);
    if (biEndDate == -1) {
        return false;
    }

    // Create key for shop item lookup
    auto key = std::make_pair(static_cast<std::int32_t>(pTBNPC->Npc_Group_ID),
                               static_cast<std::int32_t>(pTBShop->SellItem_ID));

    // Find existing shop item
    auto it = m_mpShopItem.find(key);

    if (it == m_mpShopItem.end()) {
        // Create new entry
        ST_SHOP_ITEM stShopItem = {};
        stShopItem.nShopIndex = pTBNPC->Npc_Group_ID;
        stShopItem.nItemID = pTBShop->SellItem_ID;
        stShopItem.shCount = nBuyCount;
        stShopItem.nUpdateDate = static_cast<std::int32_t>(biEndDate);

        m_mpShopItem[key] = stShopItem;

        // Send DB packet
        XSendDBPacket xSendDBPacket(pUser, 0x22, 0x14);
        if (pUser) {
            xSendDBPacket.XParse << pUser->GetUCID();
        }
        xSendDBPacket << stShopItem;
        pGameServer->SendDBGame(&xSendDBPacket);

        return true;
    }
    else {
        // Update existing entry
        std::int16_t sTempCount = nBuyCount + it->second.shCount;

        // Check day limit
        if (bAddRate) {
            if (2 * pTBShop->Day_Limit < sTempCount) {
                return false;
            }
        }
        else {
            if (pTBShop->Day_Limit < sTempCount) {
                return false;
            }
        }

        // Update end date if new one is later
        if (it->second.nUpdateDate < biEndDate) {
            it->second.nUpdateDate = static_cast<std::int32_t>(biEndDate);
        }

        // Update count
        it->second.shCount = sTempCount;

        // Send DB packet
        XSendDBPacket xSendDBPacket(pUser, 0x22, 0x14);
        if (pUser) {
            xSendDBPacket.XParse << pUser->GetUCID();
        }
        xSendDBPacket << it->second;
        pGameServer->SendDBGame(&xSendDBPacket);

        return true;
    }
}

// IDA: ?UpdateShopItem@CGocNpcCredit@@QEAA_NHKEHW4E_SHOP_PERIOD_TYPE@@@Z (0x140107250)
bool CGocNpcCredit::UpdateShopItem(std::int32_t nNpcGroupID, std::uint32_t dwItemID,
                                    std::uint8_t byLimitCount, std::int16_t nBuyCount,
                                    E_SHOP_PERIOD_TYPE byPeriodType)
{
    // Get owner user
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return false;
    }

    // TODO: RTTI cast to CUser
    CUser* pUser = nullptr;
    // if (pMover) pUser = pMover->GetUser();

    // Check user flags for shop feature (0x40)
    // if ((*((_BYTE *)&CUser::stMyCharInfoEx(pUser)->UserDB + 2) & 0x40) == 0) return false;

    if (byLimitCount == 0) {
        return false;
    }

    std::int64_t biEndDate = 0;
    GetShopItemUpdateDate(byPeriodType, biEndDate);
    if (biEndDate == -1) {
        return false;
    }

    auto key = std::make_pair(nNpcGroupID, static_cast<std::int32_t>(dwItemID));
    auto it = m_mpShopItem.find(key);

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();

    if (it == m_mpShopItem.end()) {
        // Create new entry
        ST_SHOP_ITEM stShopItem = {};
        stShopItem.nShopIndex = nNpcGroupID;
        stShopItem.nItemID = dwItemID;
        stShopItem.shCount = nBuyCount;
        stShopItem.nUpdateDate = static_cast<std::int32_t>(biEndDate);

        m_mpShopItem[key] = stShopItem;

        // Send DB packet
        XSendDBPacket xSendDBPacket(pUser, 0x22, 0x14);
        if (pUser) {
            xSendDBPacket.XParse << pUser->GetUCID();
        }
        xSendDBPacket << stShopItem;
        if (pGameServer) {
            pGameServer->SendDBGame(&xSendDBPacket);
        }

        return true;
    }
    else {
        // Update existing entry
        std::int16_t sTempCount = nBuyCount + it->second.shCount;
        if (sTempCount > static_cast<std::int16_t>(byLimitCount)) {
            return false;
        }

        if (it->second.nUpdateDate < static_cast<std::int32_t>(biEndDate)) {
            it->second.nUpdateDate = static_cast<std::int32_t>(biEndDate);
        }

        it->second.shCount = sTempCount;

        // Send DB packet
        XSendDBPacket xSendDBPacket(pUser, 0x22, 0x14);
        if (pUser) {
            xSendDBPacket.XParse << pUser->GetUCID();
        }
        xSendDBPacket << it->second;
        if (pGameServer) {
            pGameServer->SendDBGame(&xSendDBPacket);
        }

        return true;
    }
}

// IDA: ?UpdateShopAccountItem@CGocNpcCredit@@QEAA_NHKEHW4E_SHOP_PERIOD_TYPE@@@Z (0x140107740)
bool CGocNpcCredit::UpdateShopAccountItem(std::int32_t nNpcGroupID, std::uint32_t dwItemID,
                                          std::uint8_t byLimitCount, std::int16_t nBuyCount,
                                          E_SHOP_PERIOD_TYPE byPeriodType)
{
    // Get owner user via RTTI cast
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return false;
    }

    // TODO: RTTI cast to CUser - IDA uses _RTDynamicCast_0
    CUser* pUser = nullptr;
    // if (pMover) pUser = dynamic_cast<CUser*>(pMover);

    if (!pUser) {
        return false;
    }

    // Check user flags for shop feature (0x40)
    // if ((*((_BYTE *)&CUser::stMyCharInfoEx(pUser)->UserDB + 2) & 0x40) == 0) return false;

    if (byLimitCount == 0) {
        return false;
    }

    std::int64_t biEndDate = 0;
    GetShopItemUpdateDate(byPeriodType, biEndDate);
    if (biEndDate == -1) {
        return false;
    }

    auto key = std::make_pair(nNpcGroupID, static_cast<std::int32_t>(dwItemID));
    auto it = m_mpShopAccountItem.find(key);

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();

    if (it == m_mpShopAccountItem.end()) {
        // Create new entry
        ST_SHOP_ITEM stShopItem = {};
        stShopItem.nShopIndex = nNpcGroupID;
        stShopItem.nItemID = dwItemID;
        stShopItem.shCount = nBuyCount;
        stShopItem.nUpdateDate = static_cast<std::int32_t>(biEndDate);

        m_mpShopAccountItem[key] = stShopItem;

        // Send DB packet with PS_DB_SHOP_ITEM (main=0x22, sub=0x28)
        PS_DB_SHOP_ITEM stDBShopItem = {};
        stDBShopItem.dwUAID = pUser->GetUAID();
        stDBShopItem.stShopItem = stShopItem;

        XSendDBPacket xSendDBPacket(pUser, 0x22, 0x28);
        xSendDBPacket << stDBShopItem;
        if (pGameServer) {
            pGameServer->SendDBGame(&xSendDBPacket);
        }

        return true;
    }
    else {
        // Update existing entry
        std::int16_t sTempCount = nBuyCount + it->second.shCount;
        if (sTempCount > static_cast<std::int16_t>(byLimitCount)) {
            return false;
        }

        if (it->second.nUpdateDate < static_cast<std::int32_t>(biEndDate)) {
            it->second.nUpdateDate = static_cast<std::int32_t>(biEndDate);
        }

        it->second.shCount = sTempCount;

        // Send DB packet with PS_DB_SHOP_ITEM (main=0x22, sub=0x28)
        PS_DB_SHOP_ITEM stDBShopItem = {};
        stDBShopItem.dwUAID = pUser->GetUAID();
        stDBShopItem.stShopItem = it->second;

        XSendDBPacket xSendDBPacket(pUser, 0x22, 0x28);
        xSendDBPacket << stDBShopItem;
        if (pGameServer) {
            pGameServer->SendDBGame(&xSendDBPacket);
        }

        return true;
    }
}

// ============================================================================
// Initialization Callbacks
// ============================================================================

// IDA: ?OnInitShopItem@CGocNpcCredit@@QEAAXXZ (0x140107C00)
void CGocNpcCredit::OnInitShopItem()
{
    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        return;
    }

    std::int64_t biCurDate = pGameServer->GetCurDate();

    ST_SHOP_ITEM_LIST stInitShopList;
    ST_SHOP_ITEM_LIST stInitAccountShopList;

    // Iterate shop items and reset expired ones
    for (auto it = m_mpShopItem.begin(); it != m_mpShopItem.end(); ++it) {
        ST_SHOP_ITEM& stInfo = it->second;
        if (stInfo.nUpdateDate <= biCurDate) {
            stInfo.nUpdateDate = 0;
            stInfo.shCount = 0;
            stInitShopList.vecInfo.push_back(stInfo);
        }
    }

    // Iterate account shop items and reset expired ones
    for (auto it = m_mpShopAccountItem.begin(); it != m_mpShopAccountItem.end(); ++it) {
        ST_SHOP_ITEM& stInfo = it->second;
        if (stInfo.nUpdateDate <= biCurDate) {
            stInfo.nUpdateDate = 0;
            stInfo.shCount = 0;
            stInitAccountShopList.vecInfo.push_back(stInfo);
        }
    }

    // Send to client if there are updates
    if (!stInitShopList.vecInfo.empty() || !stInitAccountShopList.vecInfo.empty()) {
        CMover* pMover = GetOwnerGO();
        if (pMover) {
            XSendPacket xSendPacket(9, 0x12);
            xSendPacket << stInitShopList;
            xSendPacket << stInitAccountShopList;
            // TODO: CGocNetwork::Send(pMover->GetGOC<CGocNetwork>(), &xSendPacket);
        }
    }
}

// IDA: ?OnInitNpcCredit@CGocNpcCredit@@QEAAXXZ (0x140107F40)
void CGocNpcCredit::OnInitNpcCredit()
{
    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    XSendPacket xSendPacket(3, 0x67);
    // TODO: CGocNetwork::Send(pMover->GetGOC<CGocNetwork>(), &xSendPacket);
}

// ============================================================================
// Send to Client
// ============================================================================

// IDA: ?SendNpcCredit@CGocNpcCredit@@QEAAXXZ (0x140108770)
void CGocNpcCredit::SendNpcCredit()
{
    PS_NPC_CREDIT_LIST stNpcCreditList;

    for (const auto& pair : m_mpNpcCredit) {
        stNpcCreditList.vecInfo.push_back(pair.second);
    }

    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    XSendPacket xSendPacket(3, 0x65);
    xSendPacket << stNpcCreditList;
    // TODO: CGocNetwork::Send(pMover->GetGOC<CGocNetwork>(), &xSendPacket);
}

// IDA: ?SendShopItem@CGocNpcCredit@@QEAAXXZ (0x140108910)
void CGocNpcCredit::SendShopItem()
{
    ST_SHOP_ITEM_LIST stShopItemList;
    ST_SHOP_ITEM_LIST stShopAccountItemList;

    for (const auto& pair : m_mpShopItem) {
        stShopItemList.vecInfo.push_back(pair.second);
    }

    for (const auto& pair : m_mpShopAccountItem) {
        stShopAccountItemList.vecInfo.push_back(pair.second);
    }

    CMover* pMover = GetOwnerGO();
    if (!pMover) {
        return;
    }

    XSendPacket xSendPacket(9, 0x10);
    xSendPacket << stShopItemList;
    xSendPacket << stShopAccountItemList;
    // TODO: CGocNetwork::Send(pMover->GetGOC<CGocNetwork>(), &xSendPacket);
}

// ============================================================================
// Cheat/GM Command
// ============================================================================

// IDA: ?NpcCreditCheat@CGocNpcCredit@@QEAAXHHHH@Z (0x140108000)
void CGocNpcCredit::NpcCreditCheat(std::int32_t nType, std::int32_t nGroupID,
                                    std::int32_t nGrade, std::uint32_t nNpcID)
{
    if (!m_bEnable) {
        return;
    }

    auto it = m_mpNpcCredit.find(nGroupID);
    if (it == m_mpNpcCredit.end()) {
        return;
    }

    PS_NPC_CREDIT& stNpcCredit = it->second;

    // nType:
    // 0 = Reset grade to 0
    // 1 = Set grade directly
    // 2 = Reset grade and points
    // 3 = Add points (with grade up check)

    switch (nType) {
        case 0:
            // Reset
            stNpcCredit.byGrade = 0;
            stNpcCredit.shCurPoint = 0;
            stNpcCredit.nPoint = 0;
            break;

        case 1:
            // Set grade directly
            if (nGrade < 0) nGrade = 0;
            if (nGrade > 5) nGrade = 5;
            stNpcCredit.byGrade = static_cast<std::uint8_t>(nGrade);
            stNpcCredit.shCurPoint = 0;
            stNpcCredit.nPoint = 0;
            break;

        case 2:
            // Reset grade and points
            stNpcCredit.byGrade = static_cast<std::uint8_t>(nGrade);
            stNpcCredit.shCurPoint = 0;
            stNpcCredit.nPoint = 0;
            break;

        case 3:
            // Add points with grade up check
            // TODO: Complex logic for grade progression
            break;

        default:
            break;
    }

    // Send update to client
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        XSendPacket xSendPacket(3, 0x66);
        xSendPacket << stNpcCredit;
        // TODO: CGocNetwork::Send(pMover->GetGOC<CGocNetwork>(), &xSendPacket);
    }
}

// ============================================================================
// Helper Functions
// ============================================================================

// IDA: ?GetShopItemUpdateDate@CGocNpcCredit@@QEAAXW4E_SHOP_PERIOD_TYPE@@AEA_J@Z (0x140108B90)
void CGocNpcCredit::GetShopItemUpdateDate(E_SHOP_PERIOD_TYPE byShopPeriodType, std::int64_t& biDate)
{
    biDate = 0;

    XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    if (!pGameServer) {
        biDate = -1;
        return;
    }

    switch (byShopPeriodType) {
        case E_SHOP_PERIOD_TYPE_DAY:
            biDate = pGameServer->GetUpdateDate(9);
            break;

        case E_SHOP_PERIOD_TYPE_WEEK:
            {
                // Calculate next Tuesday 9:00 AM
                // TODO: Implement using ATL::CTime
                // Get current time, find next Tuesday
            }
            break;

        case E_SHOP_PERIOD_TYPE_MONTH:
            {
                // Calculate next month 1st 9:00 AM
                // TODO: Implement using ATL::CTime
            }
            break;

        default:
            biDate = -1;
            break;
    }
}
