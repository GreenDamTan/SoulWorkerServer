#pragma once

#include <cstdint>
#include <vector>
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

/**
 * @brief ST_DROP_ITEM_INFO - 掉落物品信息结构体
 *
 * IDA struct size: 32 bytes
 * Per IDA: 0x140411E90 (CDropItemGroup constructor uses this)
 */
struct ST_DROP_ITEM_INFO {
    XVec3 xPos;              // offset 0, size 12 - 物品掉落位置
    int nObjectID;           // offset 12, size 4 - 物品对象ID
    int nCount;              // offset 16, size 4 - 物品数量
    int nBonus;              // offset 20, size 4 - 奖励值
    std::int64_t xSerial;    // offset 24, size 8 - 序列号
};

/**
 * @brief PS_DROP_INFOS - 掉落信息包结构体
 *
 * IDA struct size: 48 bytes
 * Per IDA: 0x140411E90 (CDropItemGroup constructor parameter)
 * Per IDA: 0x14040E4C0 (SetDropInfos uses this)
 */
struct PS_DROP_INFOS {
    int nGroup;                              // offset 0, size 4 - 组ID
    bool bBossDrop;                          // offset 4, size 1 - 是否BOSS掉落
    std::vector<ST_DROP_ITEM_INFO> vecInfo; // offset 8, size 32 - 掉落物品列表
    int nJpID;                               // offset 40, size 4 - JP ID
    std::uint8_t byJpMulti;                  // offset 44, size 1 - JP倍数

    // IDA: PS_DROP_INFOS::PS_DROP_INFOS (constructor)
    PS_DROP_INFOS()
        : nGroup(0)
        , bBossDrop(false)
        , vecInfo()
        , nJpID(0)
        , byJpMulti(0)
    {
    }

    // IDA: PS_DB_ITEM_MAKE_LIMIT_INIT::~PS_DB_ITEM_MAKE_LIMIT_INIT (destructor)
    ~PS_DROP_INFOS() = default;
};
