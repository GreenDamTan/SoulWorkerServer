#include "Soulworker/GameServer/XGameServer/XItemFactory.h"

#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <initializer_list>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

struct EquipSlotRangeEntry {
    e_EQUIP_SLOT_TYPE type;
    int start;
    int end;
};

constexpr EquipSlotRangeEntry kEquipSlotRanges[] = {
    {E_EQUIP_SLOT_TYPE_SOUL_WEAPON, 0, 0},
    {E_EQUIP_SLOT_TYPE_SUB_WEAPON, 1, 1},
    {E_EQUIP_SLOT_TYPE_HAIR_ACC, 0, 0},
    {E_EQUIP_SLOT_TYPE_HAIR_CAP, 1, 1},
    {E_EQUIP_SLOT_TYPE_FACE_UPPER, 2, 2},
    {E_EQUIP_SLOT_TYPE_FACE_LOWER, 3, 3},
    {E_EQUIP_SLOT_TYPE_GLOVE, 4, 4},
    {E_EQUIP_SLOT_TYPE_UNDERWEAR, 5, 5},
    {E_EQUIP_SLOT_TYPE_COSTUME, 6, 6},
    {E_EQUIP_SLOT_TYPE_BACK, 7, 7},
    {E_EQUIP_SLOT_TYPE_SOCKS, 8, 8},
    {E_EQUIP_SLOT_TYPE_SHOES, 9, 9},
    {E_EQUIP_SLOT_TYPE_WEAPON_SKIN, 10, 10},
    {E_EQUIP_SLOT_TYPE_PANTS, 11, 11},
    {E_EQUIP_SLOT_TYPE_TAIL, 12, 12},
    {E_EQUIP_SLOT_TYPE_EFFECT, 13, 13},
    {E_EQUIP_SLOT_TYPE_PENDANT, 3, 3},
    {E_EQUIP_SLOT_TYPE_EARING, 2, 2},
    {E_EQUIP_SLOT_TYPE_RING, 4, 5},
    {E_EQUIP_SLOT_TYPE_HEAD_GEAR, 6, 6},
    {E_EQUIP_SLOT_TYPE_SHOULDER_GEAR, 7, 7},
    {E_EQUIP_SLOT_TYPE_BODY_GEAR, 8, 8},
    {E_EQUIP_SLOT_TYPE_LEG_GEAR, 9, 9},
    {E_EQUIP_SLOT_TYPE_MEMORY1, 10, 10},
    {E_EQUIP_SLOT_TYPE_MEMORY2, 11, 11},
    {E_EQUIP_SLOT_TYPE_MEMORY3, 12, 12},
    {E_EQUIP_SLOT_TYPE_MEMORY4, 13, 13},
    {E_EQUIP_SLOT_TYPE_MEMORY5, 14, 14},
    {E_EQUIP_SLOT_TYPE_MEMORY6, 15, 15},
    {E_EQUIP_SLOT_TYPE_MEMORY7, 16, 16},
    {E_EQUIP_SLOT_TYPE_MEMORY8, 17, 17},
    {E_EQUIP_SLOT_TYPE_MEMORY9, 18, 18},
    {E_EQUIP_SLOT_TYPE_MEMORY10, 19, 19},
};

}  // namespace

UXSerial XItemFactory::GeneratSerial() {
    GreenDamTan_SYSTEMTIME localTime{};
#ifdef _WIN32
    SYSTEMTIME systemTime{};
    GetLocalTime(&systemTime);
    localTime.wYear = systemTime.wYear;
    localTime.wMonth = systemTime.wMonth;
    localTime.wDayOfWeek = systemTime.wDayOfWeek;
    localTime.wDay = systemTime.wDay;
    localTime.wHour = systemTime.wHour;
    localTime.wMinute = systemTime.wMinute;
    localTime.wSecond = systemTime.wSecond;
    localTime.wMilliseconds = systemTime.wMilliseconds;
#else
    const std::time_t rawTime = std::time(nullptr);
    std::tm calendarTime{};
    localtime_r(&rawTime, &calendarTime);
    localTime.wYear = static_cast<std::uint16_t>(calendarTime.tm_year + 1900);
    localTime.wMonth = static_cast<std::uint16_t>(calendarTime.tm_mon + 1);
    localTime.wDayOfWeek = static_cast<std::uint16_t>(calendarTime.tm_wday);
    localTime.wDay = static_cast<std::uint16_t>(calendarTime.tm_mday);
    localTime.wHour = static_cast<std::uint16_t>(calendarTime.tm_hour);
    localTime.wMinute = static_cast<std::uint16_t>(calendarTime.tm_min);
    localTime.wSecond = static_cast<std::uint16_t>(calendarTime.tm_sec);
#endif

    UXSerial result{};
    result.Entity.GroupID = m_byGroupID & 0xF;
    result.Entity.ServerID = m_byServerID & 0xF;
    result.Entity.Years = localTime.wYear & 0xF;
    result.Entity.Month = localTime.wMonth & 0xF;
    result.Entity.Day = localTime.wDay & 0x1F;
    result.Entity.Hour = localTime.wHour & 0x1F;
    result.Entity.Min = localTime.wMinute & 0x3F;

    CSimpleLock::Owner lock(&m_xLock);
    if (m_Time.wMinute == localTime.wMinute)
        ++m_nSeed;
    else
        m_nSeed = 0;
    result.Entity.Seed = static_cast<std::uint32_t>(m_nSeed);
    m_Time = localTime;
    return result;
}

int XItemFactory::nRand(unsigned int nMin, int nMax) {
    if (nMin == static_cast<unsigned int>(nMax))
        return static_cast<int>(nMin);

    int minValue = static_cast<int>(nMin);
    int maxValue = nMax;
    if (static_cast<int>(nMin) > nMax) {
        minValue = nMax;
        maxValue = static_cast<int>(nMin);
    }

    int result = static_cast<int>(
        m_xSeed.GetSeed() * static_cast<double>(maxValue - minValue + 1) +
        static_cast<double>(minValue));
    if (result > maxValue)
        return maxValue;
    return result;
}

void XItemFactory::CreateItem(STItem& stItem, STItem stCreateItem) {
    if (stCreateItem.nItemID < 0)
        return;

    stItem = stCreateItem;
    stItem.xSerial = GeneratSerial().xSerial;
}

void XItemFactory::GetItemTitle(unsigned int& nTitleID,
                                unsigned int nTitleGroupID,
                                XResourceMgr* xResourceMgr) {
    nTitleID = 0;
    if (!xResourceMgr || nTitleGroupID == 0)
        return;

    TB_ITEM_TITLE* titleRow = xResourceMgr->GetTB_ITEM_TITLE(nTitleGroupID);
    if (!titleRow)
        return;

    const auto groupIds = titleRow->GetGroupIDs();
    const auto groupRates = titleRow->GetGroupRates();
    int randomValue = nRand(1, 10000);
    int groupIndex = 0;
    for (; groupIndex < 10; ++groupIndex) {
        randomValue -= groupRates[static_cast<std::size_t>(groupIndex)];
        if (randomValue <= 0)
            break;
    }
    if (groupIndex >= 10)
        return;

    TB_ITEM_TITLE_GROUP* titleGroupRow =
        xResourceMgr->GetTB_ITEM_TITLE_GROUP(
            groupIds[static_cast<std::size_t>(groupIndex)]);
    if (!titleGroupRow)
        return;

    const auto titleIds = titleGroupRow->GetTitleIDs();
    const auto titleRates = titleGroupRow->GetTitleRates();
    randomValue = nRand(1, 10000);
    int titleIndex = 0;
    for (; titleIndex < 10; ++titleIndex) {
        randomValue -= titleRates[static_cast<std::size_t>(titleIndex)];
        if (randomValue <= 0)
            break;
    }
    if (titleIndex >= 10)
        return;

    TB_ITEM_TITLE_VALUE* titleValueRow =
        xResourceMgr->GetTB_ITEM_TITLE_VALUE(
            titleIds[static_cast<std::size_t>(titleIndex)]);
    if (titleValueRow)
        nTitleID = titleValueRow->Item_Title_ID;
}

void XItemFactory::GetCostumeBroachState(STItem& stItem,
                                         unsigned int nItemID,
                                         XResourceMgr* xResourceMgr) {
    if (!xResourceMgr)
        return;

    TB_ITEM* itemRow = xResourceMgr->GetTB_ITEM(nItemID);
    if (!itemRow)
        return;
    TB_ITEM_CLASSIFY* classifyRow =
        xResourceMgr->GetTB_ITEM_CLASSIFY(itemRow->Item_Classify_Index);
    if (!classifyRow || classifyRow->Item_Use_Type != 1)
        return;

    TB_COSTUME_SOCKET* socketRow =
        xResourceMgr->GetTB_COSTUME_SOCKET(itemRow->Item_Socket_ID);
    if (!socketRow || socketRow->Socket_Piece_EA > 5u)
        return;

    const auto fixSockets = socketRow->GetFixSocketCounts();
    const auto extendSockets = socketRow->GetExtendSocketCounts();
    char* broachState = stItem.szBroachState;
    for (int pieceIndex = 0; pieceIndex < 5; ++pieceIndex,
         broachState += 3) {
        const int socketCount = static_cast<int>(
            fixSockets[static_cast<std::size_t>(pieceIndex)] +
            extendSockets[static_cast<std::size_t>(pieceIndex)]);
        if (socketCount > 3)
            break;
        for (int slotIndex = 0; slotIndex < socketCount; ++slotIndex)
            broachState[slotIndex] =
                slotIndex < fixSockets[static_cast<std::size_t>(pieceIndex)]
                    ? '1'
                    : '2';
    }
}

void XItemFactory::ReSetOption(STItem& stItem,
                               TB_ITEM* pTBItem,
                               XResourceMgr* xResourceMgr) {
    if (!xResourceMgr || !pTBItem || pTBItem->Item_Ex_Option_ID == 0)
        return;

    TB_RANDOM_OPTION* randomOptionRow =
        xResourceMgr->GetTB_RANDOM_OPTION(pTBItem->Item_Ex_Option_ID);
    if (!randomOptionRow || randomOptionRow->Option_Max > 5)
        return;

    const int optionCount = nRand(randomOptionRow->Option_Min,
                                  randomOptionRow->Option_Max);
    if (optionCount <= 0)
        return;

    const int optionReferRange = randomOptionRow->Option_Refer_Range;
    if (optionCount > 5 || optionCount > optionReferRange)
        return;

    std::vector<int> optionIndices;
    optionIndices.reserve(static_cast<std::size_t>(optionReferRange));
    for (int optionIndex = 0; optionIndex < optionReferRange; ++optionIndex)
        optionIndices.push_back(optionIndex);
    for (std::size_t last = optionIndices.size(); last > 1; --last) {
        const int pick = nRand(0, static_cast<int>(last - 1));
        std::swap(optionIndices[last - 1],
                  optionIndices[static_cast<std::size_t>(pick)]);
    }

    const auto optionTypes = randomOptionRow->GetOptionTypes();
    const auto optionMins = randomOptionRow->GetOptionMins();
    const auto optionMaxs = randomOptionRow->GetOptionMaxs();
    for (int slotIndex = 0; slotIndex < optionCount; ++slotIndex) {
        const int optionIndex = optionIndices[static_cast<std::size_t>(slotIndex)];
        ST_EXTEND_OPTION& extendOption = stItem.stExtendOption[slotIndex];
        extendOption.byType = static_cast<std::int16_t>(
            optionTypes[static_cast<std::size_t>(optionIndex)]);
        extendOption.nOption = nRand(
            optionMins[static_cast<std::size_t>(optionIndex)],
            optionMaxs[static_cast<std::size_t>(optionIndex)]);
    }
}

void XItemFactory::CreateItem(STItem& stItem,
                              unsigned int nItemID,
                              std::int16_t sCount,
                              bool bAddOption,
                              XResourceMgr* xResourceMgr,
                              bool bMakItemChangeRange) {
    if (nItemID == static_cast<unsigned int>(-1))
        return;

    stItem.sCount = sCount;
    stItem.xSerial = GeneratSerial().xSerial;
    stItem.nItemID = static_cast<int>(nItemID);
    stItem.bBindType = 0;

    if (!xResourceMgr)
        return;

    TB_ITEM* itemRow = xResourceMgr->GetTB_ITEM(nItemID);
    if (!itemRow)
        return;

    TB_ITEM_CLASSIFY* classifyRow =
        xResourceMgr->GetTB_ITEM_CLASSIFY(itemRow->Item_Classify_Index);
    if (!classifyRow)
        return;

    if (itemRow->Item_Bind_Type == 2)
        stItem.bBindType = 1;
    stItem.byEndurance = itemRow->Item_Endurance_Max;

    if (itemRow->Item_Socket_ID != 0) {
        if (classifyRow->Item_Use_Type == 1) {
            TB_COSTUME_SOCKET* costumeSocketRow =
                xResourceMgr->GetTB_COSTUME_SOCKET(itemRow->Item_Socket_ID);
            if (costumeSocketRow) {
                if (costumeSocketRow->Socket_Piece_EA > 5u)
                    return;

                const auto fixSockets = costumeSocketRow->GetFixSocketCounts();
                const auto extendSockets =
                    costumeSocketRow->GetExtendSocketCounts();
                char* broachState = stItem.szBroachState;
                for (int pieceIndex = 0; pieceIndex < 5;
                     ++pieceIndex, broachState += 3) {
                    const int socketCount = static_cast<int>(
                        fixSockets[static_cast<std::size_t>(pieceIndex)] +
                        extendSockets[static_cast<std::size_t>(pieceIndex)]);
                    if (socketCount > 3)
                        break;
                    for (int slotIndex = 0; slotIndex < socketCount; ++slotIndex) {
                        broachState[slotIndex] =
                            slotIndex <
                                    fixSockets[static_cast<std::size_t>(pieceIndex)]
                                ? '1'
                                : '2';
                    }
                }
            }
        } else if (classifyRow->Item_Use_Type == 2) {
            TB_SOCKET* socketRow =
                xResourceMgr->GetTB_SOCKET(itemRow->Item_Socket_ID);
            if (!socketRow)
                return;

            if (socketRow->Random_Socket + socketRow->Fix_Socket >
                    socketRow->Max_Socket ||
                socketRow->Max_Socket > 4 ||
                socketRow->Random_Socket > 3) {
                return;
            }

            stItem.bySocketActiveCount = socketRow->Fix_Socket;
            std::uint8_t chanceIndex = 0;
            const auto randomChances = socketRow->GetRandomChances();
            for (int attempt = 0; attempt < socketRow->Random_Socket;
                 ++attempt) {
                if (stItem.bySocketActiveCount >= socketRow->Max_Socket)
                    break;
                if (nRand(1, 10000) <=
                    randomChances[static_cast<std::size_t>(chanceIndex)]) {
                    ++stItem.bySocketActiveCount;
                    ++chanceIndex;
                }
            }
        }
    }

    if (itemRow->Item_Reinforce_ID != 0) {
        TB_REINFORCE* reinforceRow =
            xResourceMgr->GetTB_REINFORCE(itemRow->Item_Reinforce_ID);
        if (reinforceRow) {
            if (reinforceRow->Basic_Try <= 0xFFu)
                stItem.byUpgradeLimit =
                    static_cast<std::uint8_t>(reinforceRow->Basic_Try);
            else
                stItem.byUpgradeLimit = 0xFF;
        }
    }

    if (!bAddOption) {
        stItem.nAttack = static_cast<int>(itemRow->Item_physical_Attack_Min);
        stItem.nDefense =
            static_cast<int>(itemRow->Item_physical_Defense_Min);
        return;
    }

    ReSetOption(stItem, itemRow, xResourceMgr);
    if (itemRow->Item_Title_ID != 0) {
        int titleID = 0;
        unsigned int titleValue = 0;
        GetItemTitle(titleValue, itemRow->Item_Title_ID, xResourceMgr);
        titleID = static_cast<int>(titleValue);
        stItem.nTitleID = titleID;
    }

    stItem.nAttack = nRand(static_cast<int>(itemRow->Item_physical_Attack_Min),
                           static_cast<int>(itemRow->Item_physical_Attack));
    stItem.nDefense =
        nRand(static_cast<int>(itemRow->Item_physical_Defense_Min),
              static_cast<int>(itemRow->Item_physical_Defense));

    if (!bMakItemChangeRange)
        return;

    const int attackMax = static_cast<int>(itemRow->Item_physical_Attack);
    const int attackMin =
        static_cast<int>(itemRow->Item_physical_Attack_Min);
    const int defenseMax = static_cast<int>(itemRow->Item_physical_Defense);
    const int defenseMin =
        static_cast<int>(itemRow->Item_physical_Defense_Min);
    const float attackRange = static_cast<float>(attackMax - attackMin);
    const float defenseRange = static_cast<float>(defenseMax - defenseMin);

    if (classifyRow->Item_Slot_Type == 1) {
        if (attackRange > 0.0f) {
            const int corrected = static_cast<int>(
                std::ceil(attackRange * 0.41f));
            stItem.nAttack = nRand(attackMin + corrected, attackMax);
        }
        return;
    }

    if (defenseRange <= 0.0f)
        return;

    const int corrected = static_cast<int>(std::ceil(defenseRange * 0.41f));
    stItem.nDefense = nRand(defenseMin + corrected, defenseMax);
}

XItemFactory::XItemFactory() {
    m_nSeed = 0;
    m_byGroupID = 1;
    m_byServerID = 1;

#ifdef _WIN32
    SYSTEMTIME systemTime{};
    GetLocalTime(&systemTime);
    m_Time.wYear = systemTime.wYear;
    m_Time.wMonth = systemTime.wMonth;
    m_Time.wDayOfWeek = systemTime.wDayOfWeek;
    m_Time.wDay = systemTime.wDay;
    m_Time.wHour = systemTime.wHour;
    m_Time.wMinute = systemTime.wMinute;
    m_Time.wSecond = systemTime.wSecond;
    m_Time.wMilliseconds = systemTime.wMilliseconds;
#else
    m_Time = GreenDamTan_SYSTEMTIME{};
#endif

    m_xLock.Init();
    m_xSeed.GenTableForNumbers(true, false);

    for (const auto& entry : kEquipSlotRanges) {
        m_mapEquipSlotRange.emplace(
            entry.type,
            StEquipSlotRange{entry.start, entry.end});
    }
}

XItemFactory::~XItemFactory() {
    m_mapEquipSlotRange.clear();
    m_xLock.Clear();
}

void XItemFactory::Init(std::uint8_t byGroupID,
                        std::uint8_t byServerID) {
    m_byGroupID = byGroupID;
    m_byServerID = byServerID;

#ifdef _WIN32
    SYSTEMTIME systemTime{};
    GetLocalTime(&systemTime);
    m_Time.wYear = systemTime.wYear;
    m_Time.wMonth = systemTime.wMonth;
    m_Time.wDayOfWeek = systemTime.wDayOfWeek;
    m_Time.wDay = systemTime.wDay;
    m_Time.wHour = systemTime.wHour;
    m_Time.wMinute = systemTime.wMinute;
    m_Time.wSecond = systemTime.wSecond;
    m_Time.wMilliseconds = systemTime.wMilliseconds;
#else
    m_Time = GreenDamTan_SYSTEMTIME{};
#endif

    m_nSeed = 0;
    m_xLock.Init();
    m_xSeed.GenTableForNumbers(true, false);
}

bool XItemFactory::IsCanEquip(e_EQUIP_SLOT_TYPE nItemSlot,
                              int nDestSlot) {
    const auto iter = m_mapEquipSlotRange.lower_bound(nItemSlot);
    if (iter == m_mapEquipSlotRange.end() ||
        iter->first < nItemSlot) {
        return false;
    }

    return nDestSlot >= iter->second.m_nStartID &&
           nDestSlot <= iter->second.m_nEndID;
}

void XItemFactory::FindEquipSlot(e_EQUIP_SLOT_TYPE nItemSlot,
                                 std::vector<int>& vecSlot) {
    vecSlot.clear();

    const auto iter = m_mapEquipSlotRange.lower_bound(nItemSlot);
    if (iter == m_mapEquipSlotRange.end() ||
        iter->first < nItemSlot) {
        return;
    }

    for (int slot = iter->second.m_nStartID;
         slot <= iter->second.m_nEndID;
         ++slot) {
        vecSlot.push_back(slot);
    }
}
