#pragma once

// Shop system structures for personal shop, NPC shop, and cash shop

#include <cstdint>
#include <vector>
#include <map>
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCashShop.h"  // For shared structures
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"  // For PS_ROGUELIKE_SHOP_MY_INFO
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerItem.h"  // For ST_CREATE_ITEMS
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMail.h"  // For ST_SYSTEM_POST

// Forward declarations
class CItem;
class TB_ITEM;
class TB_SHOP;
class TB_NPC;

// Personal Shop Item structure
struct STPrivateShopItem
{
    std::shared_ptr<CItem> pItem;
    __int64 biMoney;

    STPrivateShopItem();
    STPrivateShopItem(const STPrivateShopItem& other);
    ~STPrivateShopItem();
};

// Personal Shop Info structure
struct ST_PRIVATE_SHOP_INFO
{
    __int64 biMoney;
    STItem stItem;

    ST_PRIVATE_SHOP_INFO();
};

// Personal Shop List structure
struct ST_PRIVATE_SHOP_LIST
{
    std::vector<ST_PRIVATE_SHOP_INFO> vecInfo;
};

// ST_SHOP_ITEM, ST_SHOP_ITEM_LIST, PS_CASH_BUY_COUNT_LIST, PS_SHOP_FAIL_ITEM, PS_DB_SHOP_ITEM
// are now defined in PSServerCashShop.h

// Shop Period Type enumeration
enum E_SHOP_PERIOD_TYPE : unsigned char
{
    E_SHOP_PERIOD_NONE = 0,
    E_SHOP_PERIOD_DAY = 1,
    E_SHOP_PERIOD_WEEK = 2,
    E_SHOP_PERIOD_MONTH = 3,
    E_SHOP_PERIOD_LIMIT = 4
};

// Cash Shop Buy Limit Type enumeration
// Note: This enum is also defined in ShopStructures.h for use by game server code
enum E_CASH_SHOP_BUY : unsigned char
{
    E_CASH_SHOP_BUY_NONE = 0,
    E_CASH_SHOP_BUY_LIMIT = 1,
    E_CASH_SHOP_BUY_LIMIT_DAY = 2,
    E_CASH_SHOP_BUY_LIMIT_WEEK = 3,
    E_CASH_SHOP_BUY_LIMIT_MONTH = 4,
    E_CASH_SHOP_BUY_LIMIT_ACCOUNT = 5,
    E_CASH_SHOP_BUY_LIMIT_ACCOUNT_DAY = 6,
    E_CASH_SHOP_BUY_LIMIT_ACCOUNT_WEEK = 7,
    E_CASH_SHOP_BUY_LIMIT_ACCOUNT_MONTH = 8
};

// PS_ROGUELIKE_SHOP_MY_INFO is defined in PSServerDB.h
// ST_SYSTEM_POST is defined in PSServerMail.h
// ST_APPEARANCE_LIST is defined in PSCommon.h
// ST_CREATE_ITEMS is defined in PSServerItem.h

// Roguelike Event Info
struct ST_ROGUELIKE_EVENT_INFO
{
    int nID;
    int nCount;
    int nType;

    ST_ROGUELIKE_EVENT_INFO();
};

// Cash Shop Tab Info
struct ST_CASH_SHOP_TAB_INFO
{
    unsigned int dwCategoryID;
    unsigned short wOrder;
};
