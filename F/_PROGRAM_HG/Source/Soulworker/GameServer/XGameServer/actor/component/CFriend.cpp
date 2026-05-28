// CFriend - Friend entry wrapper class implementation
// IDA verified from GameServer.exe

#include "CFriend.h"
#include <cwchar>

CFriend::CFriend() {
    // Zero initialize
    m_stInfo = {};
}

// Constructor from friend info - IDA pattern
CFriend::CFriend(const DB_FRIEND_INFO* stInfo) {
    if (stInfo) {
        m_stInfo = *stInfo;
    } else {
        m_stInfo = {};
    }
}

// Constructor from DB friend format
CFriend::CFriend(const PS_DB_FRIEND* stDbFriend) {
    if (stDbFriend) {
        // Convert from DB format to internal format
        m_stInfo = {};
        wcscpy_s(m_stInfo.strName, stDbFriend->strName);
        m_stInfo.dwID = stDbFriend->dwUCID;
        m_stInfo.byLevel = stDbFriend->byLevel;
        m_stInfo.byClass = stDbFriend->byClass;
        m_stInfo.byAwaken = stDbFriend->byAwaken;
        m_stInfo.dwProfilePhotoID = stDbFriend->dwProfilePhotoID;
        m_stInfo.byType = stDbFriend->byType;
        m_stInfo.byState = stDbFriend->byState;
        wcscpy_s(m_stInfo.strMemo, stDbFriend->strMemo);
        m_stInfo.nFriendPoint = stDbFriend->nFriendPoint;
        m_stInfo.tLogOut = stDbFriend->tLogOut;
        m_stInfo.tRemain = stDbFriend->tRemain;
    } else {
        m_stInfo = {};
    }
}

CFriend::~CFriend() {
    // Nothing to clean up
}

// UpdateFriend - IDA: 0x140089B50 calls CFriend::UpdateInfo
void CFriend::UpdateInfo(const DB_FRIEND_INFO* stInfo) {
    if (stInfo) {
        // Preserve certain fields while updating others
        std::uint32_t dwID = m_stInfo.dwID;  // Keep ID
        std::uint8_t byType = m_stInfo.byType;  // Keep type

        m_stInfo = *stInfo;

        // Restore preserved fields
        m_stInfo.dwID = dwID;
        m_stInfo.byType = byType;
    }
}

// GetInfo - IDA pattern
void CFriend::GetInfo(DB_FRIEND_INFO* stInfo) const {
    if (stInfo) {
        *stInfo = m_stInfo;
    }
}

// SetCommunityInfo - IDA: UpdateFriendCommunity
void CFriend::SetCommunityInfo(std::uint8_t byState, const wchar_t* szComment) {
    m_stInfo.byState = byState;
    if (szComment) {
        // Copy comment to memo field
        wcscpy_s(m_stInfo.strMemo, szComment);
    }
}

// AddFriendPoint - IDA: 0x140089EE0
void CFriend::AddFriendPoint(std::int64_t nPoint) {
    m_stInfo.nFriendPoint += nPoint;
}
