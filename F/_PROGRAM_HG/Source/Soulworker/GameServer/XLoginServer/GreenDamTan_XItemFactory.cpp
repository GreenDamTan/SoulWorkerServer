#include "Soulworker/GameServer/XLoginServer/GreenDamTan_XItemFactory.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>

namespace {
#ifdef _WIN32
void GreenDamTan_GetSystemTime(GreenDamTan_SYSTEMTIME* outTime) {
    ::GetLocalTime(outTime);
}
#else
void GreenDamTan_GetSystemTime(GreenDamTan_SYSTEMTIME* outTime) {
    if (!outTime) {
        return;
    }
    const std::time_t rawTime = std::time(nullptr);
    std::tm localTime{};
    localtime_r(&rawTime, &localTime);
    outTime->wYear = static_cast<std::uint16_t>(localTime.tm_year + 1900);
    outTime->wMonth = static_cast<std::uint16_t>(localTime.tm_mon + 1);
    outTime->wDayOfWeek = static_cast<std::uint16_t>(localTime.tm_wday);
    outTime->wDay = static_cast<std::uint16_t>(localTime.tm_mday);
    outTime->wHour = static_cast<std::uint16_t>(localTime.tm_hour);
    outTime->wMinute = static_cast<std::uint16_t>(localTime.tm_min);
    outTime->wSecond = static_cast<std::uint16_t>(localTime.tm_sec);
    outTime->wMilliseconds = 0;
}
#endif

constexpr double RNMX_0 = 0.99999988;

int GreenDamTan_SchrageStep(int current, int multiplier, int modulus, int divisor) {
    const std::int64_t quotient = current / divisor;
    const std::int64_t next = static_cast<std::int64_t>(multiplier) * current -
                              static_cast<std::int64_t>(modulus) * quotient;
    if (next < 0) {
        return static_cast<int>(next + modulus);
    }
    return static_cast<int>(next);
}
}  // namespace

XSeed::~XSeed() = default;

void XSeed::Init(bool seedFromRandomDevice) {
    GenTableForNumbers(seedFromRandomDevice, false);
}

void XSeed::SetSeed(bool seedFromTime, bool unused) {
    (void)unused;
    m_nDum = 123456789;
    m_nDum2 = 123456789;
    m_nIy = 0;
    std::fill(std::begin(m_nIv), std::end(m_nIv), 0);
    m_pArray.reset();
    m_nMaxSeedCount = 1000000;
    m_pArray = std::make_unique<double[]>(static_cast<std::size_t>(m_nMaxSeedCount));
    std::fill_n(m_pArray.get(), static_cast<std::size_t>(m_nMaxSeedCount), 0.0);
    m_nCurrIndex = 0;
    m_nCurrMaxIndex = 0;
    m_bUseArray = false;

    unsigned int srandSeed = 0;
    if (seedFromTime) {
        srandSeed = static_cast<unsigned int>(std::time(nullptr));
    }
    std::srand(srandSeed);

    int value = 0;
    do {
        value = std::rand();
    } while (value == 0);
    if (value > 0) {
        value = -value;
    }
    m_nDum = value;
}

int XSeed::GenTableForNumbers(bool seedFromTime, bool unused) {
    SetSeed(seedFromTime, unused);
    if (m_nMaxSeedCount <= 0) {
        m_nCurrIndex = 0;
        m_bUseArray = true;
        m_nCurrMaxIndex = m_nMaxSeedCount;
        return 0;
    }

    for (int index = 0; index < m_nMaxSeedCount; ++index) {
        if (m_nDum <= 0) {
            int value = m_nDum <= 0 ? static_cast<int>(std::max<std::int64_t>(1, -static_cast<std::int64_t>(m_nDum)))
                            : m_nDum;
            m_nDum = value;
            m_nDum2 = value;
            for (int slot = 39; slot >= 0; --slot) {
                m_nDum = GreenDamTan_SchrageStep(m_nDum, 40014, 2147483563, 53668);
                if (slot < 32) {
                    m_nIv[slot] = m_nDum;
                }
            }
            m_nIy = m_nIv[0];
        }

        m_nDum = GreenDamTan_SchrageStep(m_nDum, 40014, 2147483563, 53668);
        m_nDum2 = GreenDamTan_SchrageStep(m_nDum2, 40692, 2147483399, 52774);

        const int tableIndex = static_cast<int>(static_cast<double>(m_nIy) / 67108862.0);
        m_nIy = m_nIv[tableIndex] - m_nDum2;
        m_nIv[tableIndex] = m_nDum;
        if (m_nIy < 1) {
            m_nIy += 2147483562;
        }

        double value = static_cast<double>(m_nIy) * 4.656613057391769e-10;
        if (value > RNMX_0) {
            value = RNMX_0;
        }
        m_pArray[static_cast<std::size_t>(index)] = value;
    }

    m_nCurrIndex = 0;
    m_bUseArray = true;
    m_nCurrMaxIndex = m_nMaxSeedCount;
    return 0;
}

double XSeed::GetSeed() {
    int currentIndex = m_nCurrIndex;
    if (currentIndex >= m_nMaxSeedCount) {
        currentIndex = 0;
    }
    m_nCurrIndex = currentIndex;
    const double result = m_pArray ? m_pArray[static_cast<std::size_t>(currentIndex)] : 0.0;
    int nextIndex = currentIndex + 1;
    if (nextIndex >= m_nCurrMaxIndex) {
        nextIndex = 0;
    }
    m_nCurrIndex = nextIndex;
    return result;
}

void XItemFactory::Init(std::uint8_t byGroupID, std::uint8_t byServerID) {
    m_byGroupID = byGroupID;
    m_byServerID = byServerID;
    GreenDamTan_GetSystemTime(&m_Time);
    m_nSeed = 0;
    m_xLock.Init();
    m_xSeed.GenTableForNumbers(true, false);
}

UXSerial XItemFactory::GeneratSerial() {
    GreenDamTan_SYSTEMTIME localTime{};
    GreenDamTan_GetSystemTime(&localTime);

    UXSerial result{};
    result.xSerial = 0;
    result.Entity.GroupID = m_byGroupID & 0xF;
    result.Entity.ServerID = m_byServerID & 0xF;
    result.Entity.Years = localTime.wYear & 0xF;
    result.Entity.Month = localTime.wMonth & 0xF;
    result.Entity.Day = localTime.wDay & 0x1F;
    result.Entity.Hour = localTime.wHour & 0x1F;
    result.Entity.Min = localTime.wMinute & 0x3F;

    CSimpleLock::Owner lock(&m_xLock);
    if (m_Time.wYear == localTime.wYear &&
        m_Time.wMonth == localTime.wMonth &&
        m_Time.wDay == localTime.wDay &&
        m_Time.wHour == localTime.wHour &&
        m_Time.wMinute == localTime.wMinute) {
        ++m_nSeed;
    } else {
        m_nSeed = 0;
    }

    m_Time = localTime;
    result.Entity.Seed = static_cast<std::uint32_t>(m_nSeed);
    return result;
}

int XItemFactory::nRand(int nMin, int nMax) {
    int minValue = nMin;
    int maxValue = nMax;
    if (nMin == nMax) {
        return nMin;
    }
    if (nMin > nMax) {
        minValue = nMax;
        maxValue = nMin;
    }

    int result = static_cast<int>(m_xSeed.GetSeed() * static_cast<double>(maxValue - minValue + 1) +
                                  static_cast<double>(minValue));
    if (result > maxValue) {
        return maxValue;
    }
    return result;
}

void XItemFactory::GetItemTitle(unsigned int& nTitleID,
                                unsigned int nTitleGroupID,
                                XResourceMgr* xResourceMgr) {
    nTitleID = 0;
    if (!xResourceMgr || nTitleGroupID == 0) {
        return;
    }

    TB_ITEM_TITLE* titleRow = xResourceMgr->GetTB_ITEM_TITLE(nTitleGroupID);
    if (!titleRow) {
        return;
    }

    const auto groupIds = titleRow->GetGroupIDs();
    const auto groupRates = titleRow->GetGroupRates();
    int randomValue = nRand(1, 10000);
    int groupIndex = 0;
    for (; groupIndex < 10; ++groupIndex) {
        randomValue -= groupRates[groupIndex];
        if (randomValue <= 0) {
            break;
        }
    }
    if (groupIndex >= 10) {
        return;
    }

    TB_ITEM_TITLE_GROUP* titleGroupRow = xResourceMgr->GetTB_ITEM_TITLE_GROUP(groupIds[groupIndex]);
    if (!titleGroupRow) {
        return;
    }

    const auto titleIds = titleGroupRow->GetTitleIDs();
    const auto titleRates = titleGroupRow->GetTitleRates();
    randomValue = nRand(1, 10000);
    int titleIndex = 0;
    for (; titleIndex < 10; ++titleIndex) {
        randomValue -= titleRates[titleIndex];
        if (randomValue <= 0) {
            break;
        }
    }
    if (titleIndex >= 10) {
        return;
    }

    TB_ITEM_TITLE_VALUE* titleValueRow = xResourceMgr->GetTB_ITEM_TITLE_VALUE(titleIds[titleIndex]);
    if (!titleValueRow) {
        return;
    }
    nTitleID = titleValueRow->Item_Title_ID;
}

void XItemFactory::ReSetOption(STItem& stItem, TB_ITEM* pTBItem, XResourceMgr* xResourceMgr) {
    if (!xResourceMgr || !pTBItem || pTBItem->Item_Ex_Option_ID == 0) {
        return;
    }

    TB_RANDOM_OPTION* randomOptionRow = xResourceMgr->GetTB_RANDOM_OPTION(pTBItem->Item_Ex_Option_ID);
    if (!randomOptionRow || randomOptionRow->Option_Max > 5) {
        return;
    }

    const int optionCount = nRand(randomOptionRow->Option_Min, randomOptionRow->Option_Max);
    if (optionCount <= 0) {
        return;
    }

    const int optionReferRange = static_cast<int>(randomOptionRow->Option_Refer_Range);
    if (optionCount > 5 || optionCount > optionReferRange) {
        return;
    }

    std::vector<int> optionIndices;
    for (int optionIndex = 0; optionIndex < optionReferRange; ++optionIndex) {
        optionIndices.push_back(optionIndex);
    }
    if (!optionIndices.empty()) {
        for (std::size_t last = optionIndices.size() - 1; last > 0; --last) {
            const int pick = nRand(0, static_cast<int>(last));
            std::swap(optionIndices[last], optionIndices[static_cast<std::size_t>(pick)]);
        }
    }

    const auto optionTypes = randomOptionRow->GetOptionTypes();
    const auto optionMins = randomOptionRow->GetOptionMins();
    const auto optionMaxs = randomOptionRow->GetOptionMaxs();
    for (int slotIndex = 0; slotIndex < optionCount; ++slotIndex) {
        const int optionIndex = optionIndices[static_cast<std::size_t>(slotIndex)];
        ST_EXTEND_OPTION& extendOption = stItem.stExtendOption[slotIndex];
        extendOption.byType = static_cast<std::int16_t>(optionTypes[optionIndex]);
        extendOption.nOption = nRand(optionMins[optionIndex], optionMaxs[optionIndex]);
    }
}

void XItemFactory::CreateItem(STItem& stItem,
                              int nItemID,
                              std::int16_t sCount,
                              bool bAddOption,
                              XResourceMgr* xResourceMgr,
                              bool bMakItemChangeRange) {
    if (nItemID == -1) {
        return;
    }

    stItem = STItem{};
    stItem.xSerial = GeneratSerial().xSerial;
    stItem.sCount = sCount;
    stItem.nItemID = nItemID;
    stItem.bBindType = 0;

    if (!xResourceMgr) {
        return;
    }

    TB_ITEM* itemRow = xResourceMgr->GetTB_ITEM(static_cast<unsigned int>(nItemID));
    if (!itemRow) {
        return;
    }

    TB_ITEM_CLASSIFY* classifyRow = xResourceMgr->GetTB_ITEM_CLASSIFY(itemRow->Item_Classify_Index);
    if (!classifyRow) {
        return;
    }

    if (itemRow->Item_Bind_Type == 2) {
        stItem.bBindType = 1;
    }
    stItem.byEndurance = itemRow->Item_Endurance_Max;

    if (itemRow->Item_Socket_ID != 0) {
        if (classifyRow->Item_Use_Type == 1) {
            TB_COSTUME_SOCKET* costumeSocketRow =
                xResourceMgr->GetTB_COSTUME_SOCKET(itemRow->Item_Socket_ID);
            if (costumeSocketRow) {
                if (costumeSocketRow->Socket_Piece_EA > 5u) {
                    return;
                }
                std::fill(std::begin(stItem.szBroachState), std::end(stItem.szBroachState), '\0');
                const auto fixSockets = costumeSocketRow->GetFixSocketCounts();
                const auto extendSockets = costumeSocketRow->GetExtendSocketCounts();
                char* broachState = stItem.szBroachState;
                for (int pieceIndex = 0; pieceIndex < 5; ++pieceIndex, broachState += 3) {
                    const int totalSockets = static_cast<int>(fixSockets[pieceIndex] + extendSockets[pieceIndex]);
                    if (totalSockets > 3) {
                        break;
                    }
                    for (int slotIndex = 0; slotIndex < totalSockets; ++slotIndex) {
                        broachState[slotIndex] = (slotIndex < fixSockets[pieceIndex]) ? '1' : '2';
                    }
                }
            }
        } else if (classifyRow->Item_Use_Type == 2) {
            TB_SOCKET* socketRow = xResourceMgr->GetTB_SOCKET(itemRow->Item_Socket_ID);
            if (!socketRow) {
                return;
            }
            if (socketRow->Random_Socket + socketRow->Fix_Socket > socketRow->Max_Socket ||
                socketRow->Max_Socket > 4 || socketRow->Random_Socket > 3) {
                return;
            }
            stItem.bySocketActiveCount = socketRow->Fix_Socket;
            std::uint8_t chanceIndex = 0;
            const auto randomChances = socketRow->GetRandomChances();
            for (int attempt = 0; attempt < socketRow->Random_Socket; ++attempt) {
                if (stItem.bySocketActiveCount >= socketRow->Max_Socket) {
                    break;
                }
                if (nRand(1, 10000) <= randomChances[chanceIndex]) {
                    ++stItem.bySocketActiveCount;
                    ++chanceIndex;
                }
            }
        }
    }

    if (itemRow->Item_Reinforce_ID != 0) {
        TB_REINFORCE* reinforceRow = xResourceMgr->GetTB_REINFORCE(itemRow->Item_Reinforce_ID);
        if (reinforceRow) {
            if (reinforceRow->Basic_Try <= 0xFFu) {
                stItem.byUpgradeLimit = static_cast<std::uint8_t>(reinforceRow->Basic_Try);
            } else {
                stItem.byUpgradeLimit = 0xFF;
            }
        }
    }

    if (!bAddOption) {
        stItem.nAttack = static_cast<int>(itemRow->Item_physical_Attack_Min);
        stItem.nDefense = static_cast<int>(itemRow->Item_physical_Defense_Min);
        return;
    }

    ReSetOption(stItem, itemRow, xResourceMgr);
    if (itemRow->Item_Title_ID != 0) {
        unsigned int titleId = 0;
        GetItemTitle(titleId, itemRow->Item_Title_ID, xResourceMgr);
        stItem.nTitleID = static_cast<int>(titleId);
    }

    stItem.nAttack = nRand(static_cast<int>(itemRow->Item_physical_Attack_Min),
                           static_cast<int>(itemRow->Item_physical_Attack));
    stItem.nDefense = nRand(static_cast<int>(itemRow->Item_physical_Defense_Min),
                            static_cast<int>(itemRow->Item_physical_Defense));

    if (!bMakItemChangeRange) {
        return;
    }

    const int attackMax = static_cast<int>(itemRow->Item_physical_Attack);
    const int attackMin = static_cast<int>(itemRow->Item_physical_Attack_Min);
    const int defenseMax = static_cast<int>(itemRow->Item_physical_Defense);
    const int defenseMin = static_cast<int>(itemRow->Item_physical_Defense_Min);
    const float attackRange = static_cast<float>(attackMax - attackMin);
    const float defenseRange = static_cast<float>(defenseMax - defenseMin);

    if (classifyRow->Item_Slot_Type == 1) {
        if (attackRange > 0.0f) {
            const float corrected = std::ceil(attackRange * 0.41f);
            stItem.nAttack = nRand(attackMin + static_cast<int>(corrected), attackMax);
        }
        return;
    }

    if (defenseRange <= 0.0f) {
        return;
    }

    const float corrected = std::ceil(defenseRange * 0.41f);
    stItem.nDefense = nRand(defenseMin + static_cast<int>(corrected), defenseMax);
}
