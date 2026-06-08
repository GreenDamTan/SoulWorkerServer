#pragma once

// Shop system structures for personal shop, NPC shop, and cash shop

#include <cstdint>
#include <vector>
#include <map>
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

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

// Shop Item structure
struct ST_SHOP_ITEM
{
    int nShopIndex;
    unsigned int nItemID;
    short shCount;
    __int64 nUpdateDate;
    
    ST_SHOP_ITEM();
};

// Shop Item List structure
struct ST_SHOP_ITEM_LIST
{
    std::vector<ST_SHOP_ITEM> vecInfo;
};

// Cash Shop Buy Count structure
struct PS_CASH_BUY_COUNT_LIST
{
    // Structure for tracking cash shop purchase limits
    std::vector<ST_SHOP_ITEM> vecItems;
};

// Shop Fail Item structure
struct PS_SHOP_FAIL_ITEM
{
    std::vector<unsigned int> vecFailItemIDs;
};

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

// Roguelike Mode Shop My Info
struct PS_ROGUELIKE_SHOP_MY_INFO
{
    int nRoguelikeMoney;
    std::map<int, ST_ROGUELIKE_EVENT_INFO> mapMyBuyList;
    
    PS_ROGUELIKE_SHOP_MY_INFO();
    ~PS_ROGUELIKE_SHOP_MY_INFO();
};

// Roguelike Event Info
struct ST_ROGUELIKE_EVENT_INFO
{
    int nID;
    int nCount;
    int nType;
    
    ST_ROGUELIKE_EVENT_INFO();
};

// DB Shop Item structure
struct PS_DB_SHOP_ITEM
{
    unsigned int dwUAID;
    ST_SHOP_ITEM stShopItem;
    
    PS_DB_SHOP_ITEM();
};

// Cash Shop Tab Info
struct ST_CASH_SHOP_TAB_INFO
{
    unsigned int dwCategoryID;
    unsigned short wOrder;
};

// System Post structure for shop purchases
struct ST_SYSTEM_POST
{
    unsigned char byPostType;
    unsigned char byPostSubType;
    wchar_t strName[64];
    ST_CREATE_ITEM stSysItem[10];
    
    ST_SYSTEM_POST();
};

// Appearance List for cash shop
struct ST_APPEARANCE_LIST
{
    std::vector<ST_CASH_SHOP_TAB_INFO> vecAppearance;
};

// Item Create structure
struct ST_CREATE_ITEMS
{
    std::vector<ST_CASH_SHOP_TAB_INFO> vecItems;
};
