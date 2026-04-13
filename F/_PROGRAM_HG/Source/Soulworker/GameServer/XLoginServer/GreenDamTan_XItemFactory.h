#pragma once

#include <cstdint>
#include <mutex>
#include <random>

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
    int nRand(unsigned int nMin, int nMax);
    void GetItemTitle(unsigned int& nTitleID, unsigned int nTitleGroupID, XResourceMgr* xResourceMgr);
    void ReSetOption(STItem& stItem, TB_ITEM* pTBItem, XResourceMgr* xResourceMgr);
    void CreateItem(STItem& stItem,
                    int nItemID,
                    std::int16_t sCount,
                    bool bAddOption,
                    XResourceMgr* xResourceMgr,
                    bool bMakItemChangeRange);

private:
    // TODO: 推测结果：原始类型分别为 `CSimpleLock` / `XSeed`，当前以跨平台 STL 近似承接。
    std::mutex m_xLock;
    int m_nSeed = 0;
    std::uint8_t m_byGroupID = 0;
    std::uint8_t m_byServerID = 0;
    std::tm m_Time{};
    std::mt19937 m_xSeed{std::random_device{}()};
};
