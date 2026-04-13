#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ctime>

#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

/**
 * @brief 当前世界时间结构。
 *
 * 登录服在 `ResEnterServer` 成功后会立刻向客户端下发该结构，
 * 用于同步服务器当前日期时间与夏令时信息。
 */
struct ST_WORLD_CUR_DATE {
    std::int64_t n64CurDate = 0;
    std::int16_t shYear = 2000;
    std::int16_t shMonth = 1;
    std::int16_t shDay = 1;
    std::int16_t shHour = 0;
    std::int16_t shMin = 0;
    std::int16_t shSec = 0;
    std::int16_t shDST = -1;
};

static_assert(sizeof(ST_WORLD_CUR_DATE) == 0x18, "ST_WORLD_CUR_DATE size must match PDB");
static_assert(offsetof(ST_WORLD_CUR_DATE, shYear) == 0x8, "ST_WORLD_CUR_DATE.shYear offset mismatch");
static_assert(offsetof(ST_WORLD_CUR_DATE, shMonth) == 0xA, "ST_WORLD_CUR_DATE.shMonth offset mismatch");
static_assert(offsetof(ST_WORLD_CUR_DATE, shDay) == 0xC, "ST_WORLD_CUR_DATE.shDay offset mismatch");
static_assert(offsetof(ST_WORLD_CUR_DATE, shHour) == 0xE, "ST_WORLD_CUR_DATE.shHour offset mismatch");
static_assert(offsetof(ST_WORLD_CUR_DATE, shMin) == 0x10, "ST_WORLD_CUR_DATE.shMin offset mismatch");
static_assert(offsetof(ST_WORLD_CUR_DATE, shSec) == 0x12, "ST_WORLD_CUR_DATE.shSec offset mismatch");
static_assert(offsetof(ST_WORLD_CUR_DATE, shDST) == 0x14, "ST_WORLD_CUR_DATE.shDST offset mismatch");

/**
 * @brief 把世界当前时间写入发送包。
 * @param packet 输出包。
 * @param value 世界时间结构。
 * @return 返回包本身，便于链式调用。
 */
inline XPacket& operator<<(XPacket& packet, const ST_WORLD_CUR_DATE& value) {
    packet.XParse << value.n64CurDate;
    packet.XParse << value.shYear;
    packet.XParse << value.shMonth;
    packet.XParse << value.shDay;
    packet.XParse << value.shHour;
    packet.XParse << value.shMin;
    packet.XParse << value.shSec;
    packet.XParse << value.shDST;
    return packet;
}
