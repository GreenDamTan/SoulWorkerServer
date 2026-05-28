#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMisc.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCashShop.h"
#include <map>
#include <cstdint>

// Forward declarations
class CUser;
class CMover;
struct TB_NPC;
struct TB_CUSTOMER_GRADE;
struct TB_CUSTOMER_BENEFIT;

/**
 * @brief E_SHOP_PERIOD_TYPE - Shop period type enumeration
 */
enum E_SHOP_PERIOD_TYPE : std::uint8_t {
    E_SHOP_PERIOD_TYPE_DAY = 0,
    E_SHOP_PERIOD_TYPE_WEEK = 1,
    E_SHOP_PERIOD_TYPE_MONTH = 2,
};

/**
 * @brief ST_CREATE_ITEMS - Create items structure for grade rewards
 */
struct ST_CREATE_ITEMS {
    std::int32_t nItemID = 0;
    std::int16_t shCount = 0;
};

/**
 * @brief CGocNpcCredit - Game Object Component for NPC credit/reputation system
 *
 * IDA: handles NPC-related credits, reputation, and credit-based rewards.
 * This component manages:
 * - NPC credit points and grades per NPC group
 * - Shop item purchase tracking (daily/weekly/monthly limits)
 * - Shop account item tracking
 *
 * IDA layout (approximate):
 * - inherits GOComponent (16 bytes)
 * - m_mpNpcCredit: std::map<int, PS_NPC_CREDIT>
 * - m_mpShopItem: std::map<std::pair<int,int>, ST_SHOP_ITEM>
 * - m_mpShopAccountItem: std::map<std::pair<int,int>, ST_SHOP_ITEM>
 * - m_dw64WaitTick: std::uint64_t
 * - m_bEnable: bool
 */
class CGocNpcCredit : public GOComponent {
public:
    // IDA: ??0CGocNpcCredit@@QEAA@XZ (0x140105260)
    CGocNpcCredit();

    // IDA: ??1CGocNpcCredit@@UEAA@XZ (0x140105310)
    virtual ~CGocNpcCredit();

    // Static family ID for component registration
    // IDA: ?GetFamilyID@CGocNpcCredit@@SAHXZ (0x14009F750)
    static int GetFamilyID() { return 17; }

    // IDA: ?Init@CGocNpcCredit@@QEAAXXZ (0x140105380)
    void Init();

    // IDA: ?OnUpdate@CGocNpcCredit@@QEAAXXZ (0x1401053E0)
    void OnUpdate();

    // NPC Credit management
    // IDA: ?SetNpcCredit@CGocNpcCredit@@QEAAXUPS_NPC_CREDIT_LIST@@@Z (0x140105500)
    void SetNpcCredit(PS_NPC_CREDIT_LIST& stNpcCreditList);

    // IDA: ?UpdateNpcCredit@CGocNpcCredit@@QEAAXHH@Z (0x140105640)
    void UpdateNpcCredit(std::uint32_t nNpcID, std::int32_t nPoint);

    // IDA: ?UpdateNpcCredit@CGocNpcCredit@@QEAA_NHHAEAUPS_NPC_CREDIT@@AEAUST_CREATE_ITEMS@@AEAH@Z (0x140105B30)
    bool UpdateNpcCredit(std::int32_t nGroupID, std::int32_t nPoint,
                         PS_NPC_CREDIT* stUpdateInfo, ST_CREATE_ITEMS* stGradeItem,
                         std::int32_t* nGetPoint);

    // IDA: ?GetNpcCreditGrade@CGocNpcCredit@@QEAADH@Z (0x140106330)
    std::int8_t GetNpcCreditGrade(std::uint32_t nNpcID);

    // IDA: ?GetNpcCreditBenefit@CGocNpcCredit@@QEAAHHE@Z (0x1401063D0)
    std::int32_t GetNpcCreditBenefit(std::uint32_t nNpcID, std::uint8_t byBenefit);

    // IDA: ?CanNpcCreditBenefit@CGocNpcCredit@@QEAA_NHEK@Z (0x1401065C0)
    bool CanNpcCreditBenefit(std::uint32_t nNpcID, std::uint8_t byBenefit, std::uint32_t dwItemID);

    // Database operations
    // IDA: ?SendDBNpcCreditLoad@CGocNpcCredit@@QEAAXXZ (0x140106220)
    void SendDBNpcCreditLoad();

    // IDA: ?SendDBShopItemLoad@CGocNpcCredit@@QEAAXXZ (0x140106800)
    void SendDBShopItemLoad();

    // Shop Item management
    // IDA: ?SetShopItem@CGocNpcCredit@@QEAAXUST_SHOP_ITEM_LIST@@@Z (0x140106A40)
    void SetShopItem(ST_SHOP_ITEM_LIST& stShopItemList);

    // IDA: ?SetShopAccountItem@CGocNpcCredit@@QEAAXUST_SHOP_ITEM_LIST@@@Z (0x140106C00)
    void SetShopAccountItem(ST_SHOP_ITEM_LIST& stShopAccountItemList);

    // IDA: ?UpdateShopItem@CGocNpcCredit@@QEAA_NHHE_NH@Z (0x140106DC0)
    bool UpdateShopItem(std::uint32_t nNpcID, std::uint32_t nShopIndex,
                        std::uint8_t byGrade, bool bAddRate, std::int16_t nBuyCount);

    // IDA: ?UpdateShopItem@CGocNpcCredit@@QEAA_NHKEHW4E_SHOP_PERIOD_TYPE@@@Z (0x140107250)
    bool UpdateShopItem(std::int32_t nNpcGroupID, std::uint32_t dwItemID,
                        std::uint8_t byLimitCount, std::int16_t nBuyCount,
                        E_SHOP_PERIOD_TYPE byPeriodType);

    // IDA: ?UpdateShopAccountItem@CGocNpcCredit@@QEAA_NHKEHW4E_SHOP_PERIOD_TYPE@@@Z (0x140107740)
    bool UpdateShopAccountItem(std::int32_t nNpcGroupID, std::uint32_t dwItemID,
                               std::uint8_t byLimitCount, std::int16_t nBuyCount,
                               E_SHOP_PERIOD_TYPE byPeriodType);

    // Initialization callbacks
    // IDA: ?OnInitShopItem@CGocNpcCredit@@QEAAXXZ (0x140107C00)
    void OnInitShopItem();

    // IDA: ?OnInitNpcCredit@CGocNpcCredit@@QEAAXXZ (0x140107F40)
    void OnInitNpcCredit();

    // Send to client
    // IDA: ?SendNpcCredit@CGocNpcCredit@@QEAAXXZ (0x140108770)
    void SendNpcCredit();

    // IDA: ?SendShopItem@CGocNpcCredit@@QEAAXXZ (0x140108910)
    void SendShopItem();

    // Cheat/GM command
    // IDA: ?NpcCreditCheat@CGocNpcCredit@@QEAAXHHHH@Z (0x140108000)
    void NpcCreditCheat(std::int32_t nType, std::int32_t nGroupID,
                        std::int32_t nGrade, std::uint32_t nNpcID);

protected:
    // IDA: ?GetShopItemUpdateDate@CGocNpcCredit@@QEAAXW4E_SHOP_PERIOD_TYPE@@AEA_J@Z (0x140108B90)
    void GetShopItemUpdateDate(E_SHOP_PERIOD_TYPE byShopPeriodType, std::int64_t& biDate);

private:
    // NPC credit data per group ID
    // IDA: m_mpNpcCredit - std::map<int, PS_NPC_CREDIT>
    std::map<std::int32_t, PS_NPC_CREDIT> m_mpNpcCredit;

    // Shop item purchase tracking - key is (nGroupID, nItemID)
    // IDA: m_mpShopItem - std::map<std::pair<int,int>, ST_SHOP_ITEM>
    std::map<std::pair<std::int32_t, std::int32_t>, ST_SHOP_ITEM> m_mpShopItem;

    // Account-level shop item purchase tracking
    // IDA: m_mpShopAccountItem - std::map<std::pair<int,int>, ST_SHOP_ITEM>
    std::map<std::pair<std::int32_t, std::int32_t>, ST_SHOP_ITEM> m_mpShopAccountItem;

    // Wait tick for initialization timing
    // IDA: m_dw64WaitTick
    std::uint64_t m_dw64WaitTick = 0;

    // Enable flag
    // IDA: m_bEnable
    bool m_bEnable = false;
};
