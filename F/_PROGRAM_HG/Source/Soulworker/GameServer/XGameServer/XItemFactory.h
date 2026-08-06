// XItemFactory.h
// Item Factory

#pragma once

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>

#include "Soulworker/GameServer/XCore/XServer/IXObject.h"
#include "Soulworker/GameServer/XCore/XServer/XSeed.h"

// UXSerial - Serial number union
union UXSerial {
    struct Data {
        std::int64_t Seed : 32;
        std::int64_t Min : 6;
        std::int64_t Hour : 5;
        std::int64_t Day : 5;
        std::int64_t Month : 4;
        std::int64_t Years : 4;
        std::int64_t GroupID : 4;
        std::int64_t ServerID : 4;
    } Entity;
    std::int64_t xSerial = 0;
};

// Forward declarations
class XResourceMgr;
struct TB_ITEM;
struct GreenDamTan_SYSTEMTIME;

// Include STItem definition
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

// PDB eITEM_INVEN_TYPE: enum underlying type is int.
enum eITEM_INVEN_TYPE : int {
    E_ITEM_INVEN_TYPE_COMMON = 0,
    E_ITEM_INVEN_TYPE_COSTUME = 2,
    E_ITEM_INVEN_TYPE_CUBE = 9,
    E_ITEM_INVEN_TYPE_CASH = 13,
};

// PDB eITEM_USE_TYPE: enum underlying type is int.
enum eITEM_USE_TYPE : int {
    E_ITEM_USE_TYPE_SHAPE = 1,
    E_ITEM_USE_TYPE_ABILITY = 2,
    E_ITEM_USE_TYPE_HELPER_EQUIP = 3,
    E_ITEM_USE_TYPE_FURNITURE = 4,
    E_ITEM_USE_TYPE_AKASHIC_RECORD = 5,
    E_ITEM_USE_TYPE_TITLE = 6,
    E_ITEM_USE_TYPE_APPEARANCE = 111,
    E_ITEM_USE_TYPE_SELECT = 113,
    E_ITEM_USE_TYPE_COUNTBOX = 114,
    E_ITEM_USE_TYPE_RESEAL_PACKAGE = 121,
    E_ITEM_USE_TYPE_SKILL_DECK_OPEN = 122,
    E_ITEM_USE_TYPE_AKASHIC_DECK_OPEN = 123,
};

// PDB e_EQUIP_SLOT_TYPE: enum underlying type is int.
enum e_EQUIP_SLOT_TYPE : int {
    E_EQUIP_SLOT_TYPE_SOUL_WEAPON = 1,
    E_EQUIP_SLOT_TYPE_SUB_WEAPON = 2,
    E_EQUIP_SLOT_TYPE_HAIR_ACC = 11,
    E_EQUIP_SLOT_TYPE_HAIR_CAP = 12,
    E_EQUIP_SLOT_TYPE_FACE_UPPER = 13,
    E_EQUIP_SLOT_TYPE_FACE_LOWER = 14,
    E_EQUIP_SLOT_TYPE_GLOVE = 15,
    E_EQUIP_SLOT_TYPE_UNDERWEAR = 16,
    E_EQUIP_SLOT_TYPE_COSTUME = 17,
    E_EQUIP_SLOT_TYPE_BACK = 18,
    E_EQUIP_SLOT_TYPE_SOCKS = 19,
    E_EQUIP_SLOT_TYPE_SHOES = 20,
    E_EQUIP_SLOT_TYPE_WEAPON_SKIN = 21,
    E_EQUIP_SLOT_TYPE_PANTS = 22,
    E_EQUIP_SLOT_TYPE_TAIL = 23,
    E_EQUIP_SLOT_TYPE_EFFECT = 24,
    E_EQUIP_SLOT_TYPE_PENDANT = 111,
    E_EQUIP_SLOT_TYPE_EARING = 131,
    E_EQUIP_SLOT_TYPE_RING = 141,
    E_EQUIP_SLOT_TYPE_HEAD_GEAR = 151,
    E_EQUIP_SLOT_TYPE_SHOULDER_GEAR = 161,
    E_EQUIP_SLOT_TYPE_BODY_GEAR = 171,
    E_EQUIP_SLOT_TYPE_LEG_GEAR = 181,
    E_EQUIP_SLOT_TYPE_BROACH_ATK = 97,
    E_EQUIP_SLOT_TYPE_BROACH_DEF = 98,
    E_EQUIP_SLOT_TYPE_BROACH_FUN = 99,
    E_EQUIP_SLOT_TYPE_MEMORY1 = 101,
    E_EQUIP_SLOT_TYPE_MEMORY2 = 102,
    E_EQUIP_SLOT_TYPE_MEMORY3 = 103,
    E_EQUIP_SLOT_TYPE_MEMORY4 = 104,
    E_EQUIP_SLOT_TYPE_MEMORY5 = 105,
    E_EQUIP_SLOT_TYPE_MEMORY6 = 106,
    E_EQUIP_SLOT_TYPE_MEMORY7 = 107,
    E_EQUIP_SLOT_TYPE_MEMORY8 = 108,
    E_EQUIP_SLOT_TYPE_MEMORY9 = 109,
    E_EQUIP_SLOT_TYPE_MEMORY10 = 110,
    E_EQUIP_SLOT_TYPE_HEAR = 251,
    E_EQUIP_SLOT_TYPE_HEAR_COLOR = 252,
    E_EQUIP_SLOT_TYPE_SKIN_COLOR = 253,
    E_EQUIP_SLOT_TYPE_EYE_COLOR = 254,
    E_EQUIP_SLOT_TYPE_SKILL = 199,
    E_EQUIP_SLOT_TYPE_SOCIAL = 203,
    E_EQUIP_SLOT_OWN_AKASHIC_RECODE = 201,
    E_EQUIP_SLOT_SKILL_AKASHIC_RECODE = 202,
    E_EQUIP_SLOT_CONSUME_ITEM = 200,
    E_EQUIP_SLOT_HELPER_ITEM_1 = 240,
    E_EQUIP_SLOT_HELPER_ITEM_2 = 241,
    E_EQUIP_SLOT_HELPER_COSTUME = 242,
    E_EQUIP_SLOT_TYPE_MAX = 243,
};

struct StEquipSlotRange {
    int m_nStartID = 0;
    int m_nEndID = 0;
};

static_assert(sizeof(StEquipSlotRange) == 8,
              "StEquipSlotRange size must match GameServer PDB");

struct GreenDamTan_SYSTEMTIME {
    std::uint16_t wYear = 0;
    std::uint16_t wMonth = 0;
    std::uint16_t wDayOfWeek = 0;
    std::uint16_t wDay = 0;
    std::uint16_t wHour = 0;
    std::uint16_t wMinute = 0;
    std::uint16_t wSecond = 0;
    std::uint16_t wMilliseconds = 0;
};

static_assert(sizeof(GreenDamTan_SYSTEMTIME) == 16,
              "GreenDamTan_SYSTEMTIME size must match Win32 SYSTEMTIME");

// XItemFactory - Item Factory
class XItemFactory {
public:
    XItemFactory();
    ~XItemFactory();

    void Init(std::uint8_t byGroupID,
              std::uint8_t byServerID);

    UXSerial GeneratSerial();
    int nRand(unsigned int nMin, int nMax);

    // CreateItem - IDA: 0x1408B1160
    void CreateItem(STItem& stItem, STItem stCreateItem);

    void GetItemTitle(unsigned int& nTitleID,
                      unsigned int nTitleGroupID,
                      XResourceMgr* xResourceMgr);
    void GetCostumeBroachState(STItem& stItem,
                               unsigned int nItemID,
                               XResourceMgr* xResourceMgr);
    void ReSetOption(STItem& stItem,
                     TB_ITEM* pTBItem,
                     XResourceMgr* xResourceMgr);

    void CreateItem(STItem& stItem, unsigned int nItemID,
                    std::int16_t sCount, bool bAddOption,
                    XResourceMgr* xResourceMgr,
                    bool bMakItemChangeRange);

    bool IsCanEquip(e_EQUIP_SLOT_TYPE nItemSlot, int nDestSlot);
    void FindEquipSlot(e_EQUIP_SLOT_TYPE nItemSlot, std::vector<int>& vecSlot);

private:
    CSimpleLock m_xLock;
    int m_nSeed = 0;
    std::uint8_t m_byGroupID = 0;
    std::uint8_t m_byServerID = 0;
    std::uint8_t m_padding[2] = {};
    XSeed m_xSeed;
    GreenDamTan_SYSTEMTIME m_Time{};
    std::map<e_EQUIP_SLOT_TYPE, StEquipSlotRange> m_mapEquipSlotRange;
#if defined(_MSC_VER) && !defined(__clang__)
    static_assert(sizeof(std::map<e_EQUIP_SLOT_TYPE, StEquipSlotRange>) == 32,
                  "XItemFactory map size must match GameServer PDB");
#else
    // GameServer PDB uses the 32-byte MSVC _Tree layout; this clang-cl
    // STL instance is 24 bytes while preserving the map's public semantics.
    std::uint8_t m_mapAbiTail[8] = {};
#endif
};

static_assert(sizeof(XItemFactory) == 272,
              "XItemFactory size must match GameServer PDB");
