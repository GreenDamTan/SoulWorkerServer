#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerFriend.h"
#include <cstdint>

/**
 * @brief CFriend - Friend entry wrapper class
 *
 * Wraps friend information (DB_FRIEND_INFO) for use in CGocFriend's friend list.
 * IDA shows size 0xA0 (160 bytes) for CFriend object.
 */
class CFriend {
public:
    // Default constructor
    CFriend();

    // Constructor from friend info - IDA pattern
    explicit CFriend(const DB_FRIEND_INFO* stInfo);

    // Constructor from DB friend format
    explicit CFriend(const PS_DB_FRIEND* stDbFriend);

    ~CFriend();

    // Get UCID (unique character ID) - primary key
    std::uint32_t GetUCID() const { return m_stInfo.dwID; }

    // Get friend type (1=Friend, 2=Special Friend, 3=Unlimited)
    std::uint8_t GetType() const { return m_stInfo.byType; }

    // Get friend name
    const wchar_t* GetName() const { return m_stInfo.strName; }

    // Get friend level
    std::uint8_t GetLevel() const { return m_stInfo.byLevel; }

    // Get friend class
    std::uint8_t GetClass() const { return m_stInfo.byClass; }

    // Get online state
    std::uint8_t GetState() const { return m_stInfo.byState; }

    // Is friend logged in
    bool IsLogin() const { return m_stInfo.bLogin; }

    // Get friend point
    std::int64_t GetFriendPoint() const { return m_stInfo.nFriendPoint; }

    // Update friend info - IDA: CFriend::UpdateInfo
    void UpdateInfo(const DB_FRIEND_INFO* stInfo);

    // Get full friend info - IDA: CFriend::GetInfo
    void GetInfo(DB_FRIEND_INFO* stInfo) const;

    // Set community info
    void SetCommunityInfo(std::uint8_t byState, const wchar_t* szComment);

    // Add friend point
    void AddFriendPoint(std::int64_t nPoint);

private:
    // Friend info storage
    DB_FRIEND_INFO m_stInfo = {};
};
