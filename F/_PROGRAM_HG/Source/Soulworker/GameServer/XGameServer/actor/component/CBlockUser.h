#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerFriend.h"
#include <cstdint>
#include <cwchar>

/**
 * @brief CBlockUser - Blocked user entry wrapper class
 *
 * Wraps block information (DB_BLOCK_INFO) for use in CGocFriend's block list.
 * IDA shows size 0x30 (48 bytes) for CBlockUser object.
 */
class CBlockUser {
public:
    // Default constructor
    CBlockUser();

    // Constructor from block info - IDA pattern
    explicit CBlockUser(const DB_BLOCK_INFO* stInfo);

    ~CBlockUser();

    // Get UCID (unique character ID) - primary key
    std::uint32_t GetUCID() const { return m_stInfo.dwUCID; }

    // Get blocked user name
    const wchar_t* GetName() const { return m_stInfo.strName; }

    // Get blocked user level
    std::uint8_t GetLevel() const { return m_stInfo.byLevel; }

    // Get full block info
    void GetInfo(DB_BLOCK_INFO* stInfo) const;

private:
    // Block info storage
    DB_BLOCK_INFO m_stInfo = {};
};
