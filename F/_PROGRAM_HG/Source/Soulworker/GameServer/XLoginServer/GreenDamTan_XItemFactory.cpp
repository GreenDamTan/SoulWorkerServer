#include "Soulworker/GameServer/XLoginServer/GreenDamTan_XItemFactory.h"

#include <cmath>

namespace {
std::tm GetLocalTimeCompat(std::time_t rawTime) {
    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &rawTime);
#else
    localtime_r(&rawTime, &localTime);
#endif
    return localTime;
}
}  // namespace

void XItemFactory::Init(std::uint8_t byGroupID, std::uint8_t byServerID) {
    m_byGroupID = byGroupID;
    m_byServerID = byServerID;
    m_Time = GetLocalTimeCompat(std::time(nullptr));
    m_nSeed = 0;
    // TODO: 推测结果：原始实现调用 `CSimpleLock::Init` 与 `XSeed::GenTableForNumbers`。
    // 当前骨架用 `std::mutex` 与 `std::mt19937` 维持跨平台可编译/可执行行为。
    m_xSeed.seed(std::random_device{}());
}

UXSerial XItemFactory::GeneratSerial() {
    const std::time_t now = std::time(nullptr);
    const std::tm localTime = GetLocalTimeCompat(now);

    UXSerial result{};
    result.xSerial = 0;
    result.Entity.GroupID = m_byGroupID & 0xF;
    result.Entity.ServerID = m_byServerID & 0xF;
    result.Entity.Years = (localTime.tm_year + 1900) & 0xF;
    result.Entity.Month = (localTime.tm_mon + 1) & 0xF;
    result.Entity.Day = localTime.tm_mday & 0x1F;
    result.Entity.Hour = localTime.tm_hour & 0x1F;
    result.Entity.Min = localTime.tm_min & 0x3F;

    std::lock_guard<std::mutex> lock(m_xLock);
    if (m_Time.tm_year == localTime.tm_year &&
        m_Time.tm_mon == localTime.tm_mon &&
        m_Time.tm_mday == localTime.tm_mday &&
        m_Time.tm_hour == localTime.tm_hour &&
        m_Time.tm_min == localTime.tm_min) {
        ++m_nSeed;
    } else {
        m_nSeed = 0;
    }

    m_Time = localTime;
    result.Entity.Seed = static_cast<std::uint32_t>(m_nSeed);
    return result;
}

int XItemFactory::nRand(unsigned int nMin, int nMax) {
    int minValue = static_cast<int>(nMin);
    int maxValue = nMax;
    if (minValue == maxValue) {
        return minValue;
    }
    if (minValue > maxValue) {
        std::swap(minValue, maxValue);
    }

    std::uniform_int_distribution<int> distribution(minValue, maxValue);
    return distribution(m_xSeed);
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

    const std::uint8_t optionCount =
        static_cast<std::uint8_t>(nRand(randomOptionRow->Option_Min, randomOptionRow->Option_Max));
    if (optionCount == 0) {
        return;
    }

    constexpr int kRandomOptionTypeCount = 25;
    const int optionReferRange = static_cast<int>(randomOptionRow->Option_Refer_Range);
    if (optionReferRange <= 0 || optionReferRange > kRandomOptionTypeCount) {
        // TODO: 需人工审查：当前仅恢复到 25 组随机词条，越界配置暂不继续推断。
        return;
    }
    if (optionCount > static_cast<std::uint8_t>(optionReferRange)) {
        return;
    }

    std::vector<int> optionIndices;
    optionIndices.reserve(static_cast<std::size_t>(optionReferRange));
    for (int optionIndex = 0; optionIndex < optionReferRange; ++optionIndex) {
        optionIndices.push_back(optionIndex);
    }
    std::shuffle(optionIndices.begin(), optionIndices.end(), m_xSeed);

    const auto optionTypes = randomOptionRow->GetOptionTypes();
    const auto optionMins = randomOptionRow->GetOptionMins();
    const auto optionMaxs = randomOptionRow->GetOptionMaxs();

    for (int slotIndex = 0; slotIndex < optionCount; ++slotIndex) {
        const int optionIndex = optionIndices[static_cast<std::size_t>(slotIndex)];
        ST_EXTEND_OPTION& extendOption = stItem.stExtendOption[slotIndex];
        extendOption.byType = static_cast<std::int16_t>(optionTypes[optionIndex]);
        extendOption.nOption =
            nRand(static_cast<unsigned int>(optionMins[optionIndex]), optionMaxs[optionIndex]);
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

    TB_ITEM_CLASSIFY* classifyRow =
        xResourceMgr->GetTB_ITEM_CLASSIFY(itemRow->Item_Classify_Index);
    if (!classifyRow) {
        return;
    }

    if (itemRow->Item_Bind_Type == 2) {
        stItem.bBindType = 1;
    }

    stItem.byEndurance = itemRow->Item_Endurance_Max;
    stItem.byUpgradeLimit = 0;

    if (itemRow->Item_Socket_ID != 0) {
        if (classifyRow->Item_Use_Type == 1) {
            TB_COSTUME_SOCKET* costumeSocketRow =
                xResourceMgr->GetTB_COSTUME_SOCKET(itemRow->Item_Socket_ID);
            if (costumeSocketRow) {
                if (costumeSocketRow->Socket_Piece_EA > 5) {
                    return;
                }

                const auto fixSockets = costumeSocketRow->GetFixSocketCounts();
                const auto extendSockets = costumeSocketRow->GetExtendSocketCounts();
                std::fill(std::begin(stItem.szBroachState), std::end(stItem.szBroachState), '\0');
                for (int pieceIndex = 0; pieceIndex < 5; ++pieceIndex) {
                    const int totalSockets =
                        static_cast<int>(fixSockets[pieceIndex] + extendSockets[pieceIndex]);
                    if (totalSockets > 3) {
                        break;
                    }

                    for (int slotIndex = 0; slotIndex < totalSockets; ++slotIndex) {
                        stItem.szBroachState[pieceIndex * 3 + slotIndex] =
                            (slotIndex < fixSockets[pieceIndex]) ? '1' : '2';
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
            if (reinforceRow->Basic_Try <= 0xFF) {
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

    stItem.nAttack = nRand(itemRow->Item_physical_Attack_Min,
                           static_cast<int>(itemRow->Item_physical_Attack));
    stItem.nDefense = nRand(itemRow->Item_physical_Defense_Min,
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
            const int correctedAttackMin =
                attackMin + static_cast<int>(std::ceil(attackRange * 0.41f));
            stItem.nAttack = nRand(static_cast<unsigned int>(correctedAttackMin), attackMax);
        }
        return;
    }

    if (defenseRange <= 0.0f) {
        return;
    }

    const int correctedDefenseMin =
        defenseMin + static_cast<int>(std::ceil(defenseRange * 0.41f));
    stItem.nDefense = nRand(static_cast<unsigned int>(correctedDefenseMin), defenseMax);
}
