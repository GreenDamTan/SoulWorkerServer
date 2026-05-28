#pragma once

#include "gocbase.h"
#include <cstdint>

/**
 * @brief CGocFriend - Game Object Component for friend/social system
 *
 * Handles friend list, social interactions, and friend-related functionality.
 */
class CGocFriend : public GOComponent {
public:
    CGocFriend();
    virtual ~CGocFriend();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Friend list operations
    int GetFriendCount() const;
    int GetMaxFriends() const;
    bool IsFriendListFull() const;

    // Friend management
    bool AddFriend(int nPlayerId);
    bool RemoveFriend(int nPlayerId);
    bool IsFriend(int nPlayerId) const;

    // Friend requests
    int GetPendingRequestCount() const;
    bool HasPendingRequest(int nPlayerId) const;
    bool AcceptFriendRequest(int nPlayerId);
    bool RejectFriendRequest(int nPlayerId);
    bool SendFriendRequest(int nPlayerId);

    // Friend status
    bool IsFriendOnline(int nPlayerId) const;
    int GetOnlineFriendCount() const;

    // Block list
    bool BlockPlayer(int nPlayerId);
    bool UnblockPlayer(int nPlayerId);
    bool IsBlocked(int nPlayerId) const;
    int GetBlockedCount() const;

protected:
    int m_nFriendCount;
    int m_nMaxFriends;
    int m_nBlockedCount;
};
