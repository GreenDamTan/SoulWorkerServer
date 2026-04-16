#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "Soulworker/GameServer/XLoginServer/stdafx.h"

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

struct StEquipSlotRange {
    int m_nStartID = 0;
    int m_nEndID = 0;
};

enum e_EQUIP_SLOT_TYPE : int;

#ifdef _WIN32
using GreenDamTan_SYSTEMTIME = SYSTEMTIME;
#else
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
#endif

class XSeed {
public:
    ~XSeed();

    void Init(bool seedFromRandomDevice);
    void SetSeed(bool seedFromTime, bool unused = false);
    int GenTableForNumbers(bool seedFromTime, bool unused = false);
    double GetSeed();

private:
    int m_nDum = 123456789;
    int m_nDum2 = 123456789;
    int m_nIy = 0;
    int m_nIv[32] = {};
    std::unique_ptr<double[]> m_pArray;
    int m_nCurrIndex = 0;
    int m_nCurrMaxIndex = 0;
    bool m_bUseArray = false;
    int m_nMaxSeedCount = 1000000;
};
static_assert(sizeof(XSeed) == 0xA8, "XSeed size must match PDB");

/**
 * @brief `XItemFactory` 的最小证据化骨架。
 *
 * 当前已按 IDA 恢复建角链里实际使用到的物品创建分支，包括：
 * - 随机词条
 * - 服装布罗奇孔状态
 * - 普通装备固定/随机开孔数
 * - 强化尝试上限
 * - 称号组二段随机
 * - 攻防区间修正
 */
class XItemFactory {
public:
    void Init(std::uint8_t byGroupID, std::uint8_t byServerID);
    UXSerial GeneratSerial();
    int nRand(int nMin, int nMax);
    void GetItemTitle(unsigned int& nTitleID, unsigned int nTitleGroupID, XResourceMgr* xResourceMgr);
    void ReSetOption(STItem& stItem, TB_ITEM* pTBItem, XResourceMgr* xResourceMgr);
    void CreateItem(STItem& stItem,
                    int nItemID,
                    std::int16_t sCount,
                    bool bAddOption,
                    XResourceMgr* xResourceMgr,
                    bool bMakItemChangeRange);

private:
    CSimpleLock m_xLock;
    int m_nSeed = 0;
    std::uint8_t m_byGroupID = 0;
    std::uint8_t m_byServerID = 0;
    XSeed m_xSeed;
    GreenDamTan_SYSTEMTIME m_Time{};
    std::map<e_EQUIP_SLOT_TYPE, StEquipSlotRange> m_mapEquipSlotRange;
};
