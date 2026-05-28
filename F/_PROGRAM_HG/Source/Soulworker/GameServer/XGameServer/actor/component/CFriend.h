#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerFriend.h"
#include "Soulworker/GameServer/XRelayServer/UserObject.h"
#include <cstdint>
#include <string>

/**
 * @brief CFriend - Friend entry wrapper class
 *
 * Wraps friend information for use in CGocFriend's friend list.
 * IDA shows size 0xA0 (160 bytes) for CFriend object.
 * Uses ST_FRIEND_INFO as the primary data structure (from IDA decompilation).
 *
 * IDA verified functions:
 * - CFriend::CFriend(ST_FRIEND_INFO*) at 0x140085F30
 * - CFriend::GetUCID at 0x140085F90
 * - CFriend::GetType at 0x140085FA0
 * - CFriend::GetName at 0x140085FB0
 * - CFriend::GetInfo at 0x140086230
 * - CFriend::UpdateInfo(ST_FRIEND_INFO*) at 0x140086280
 * - CFriend::UpdateInfo(ST_FRIEND_COMMUNITY*) at 0x140086430
 */
class CFriend {
public:
    // Default constructor
    CFriend();

    // Constructor from ST_FRIEND_INFO - IDA pattern (primary constructor)
    // IDA: 0x140085F30 - ??0CFriend@@QEAA@AEAUST_FRIEND_INFO@@@Z
    explicit CFriend(const ST_FRIEND_INFO* stInfo);

    // Constructor from DB friend format (for DB loading)
    explicit CFriend(const DB_FRIEND_INFO* stInfo);

    // Constructor from DB friend format
    explicit CFriend(const PS_DB_FRIEND* stDbFriend);

    ~CFriend();

    // Get UCID (unique character ID) - primary key
    // IDA: 0x140085F90 - ?GetUCID@CFriend@@QEBA?BKXZ
    std::uint32_t GetUCID() const { return m_stFriendInfo.dwID; }

    // Get friend type (1=Friend, 2=Special Friend, 3=Unlimited)
    // IDA: 0x140085FA0 - ?GetType@CFriend@@QEBA?BEXZ
    std::uint8_t GetType() const { return m_stFriendInfo.byType; }

    // Get friend name - IDA returns std::wstring
    // IDA: 0x140085FB0 - ?GetName@CFriend@@QEBA?BV?$basic_string@_WU?$char_traits@_W@std@@V?$allocator@_W@2@@std@@XZ
    std::wstring GetName() const;

    // Get friend level
    std::uint8_t GetLevel() const { return m_stFriendInfo.byLevel; }

    // Get friend class
    std::uint8_t GetClass() const { return m_stFriendInfo.byClass; }

    // Get online state
    std::uint8_t GetState() const { return m_stFriendInfo.byState; }

    // Is friend logged in
    bool IsLogin() const { return m_stFriendInfo.bLogin; }

    // Get friend point
    std::int64_t GetFriendPoint() const { return m_stFriendInfo.nFriendPoint; }

    // Update friend info - IDA: CFriend::UpdateInfo
    // IDA: 0x140086280 - ?UpdateInfo@CFriend@@QEAAXAEAUST_FRIEND_INFO@@@Z
    void UpdateInfo(const ST_FRIEND_INFO* stInfo);
    void UpdateInfo(const DB_FRIEND_INFO* stInfo);

    // Update from community info
    // IDA: 0x140086430 - ?UpdateInfo@CFriend@@QEAAXAEAUST_FRIEND_COMMUNITY@@@Z
    void UpdateInfo(const ST_FRIEND_COMMUNITY* stCommunity);

    // Get full friend info - IDA: CFriend::GetInfo
    // IDA: 0x140086230 - ?GetInfo@CFriend@@QEAAXAEAUST_FRIEND_INFO@@@Z
    void GetInfo(ST_FRIEND_INFO* stInfo) const;
    void GetInfo(DB_FRIEND_INFO* stInfo) const;

    // Set community info (wrapper for UpdateInfo(ST_FRIEND_COMMUNITY*))
    void SetCommunityInfo(std::uint8_t byState, const wchar_t* szComment);

    // Add friend point
    void AddFriendPoint(std::int64_t nPoint);

private:
    // Friend info storage - uses ST_FRIEND_INFO as primary structure (IDA verified)
    // IDA uses m_stFriendInfo as the member name
    ST_FRIEND_INFO m_stFriendInfo = {};
};
