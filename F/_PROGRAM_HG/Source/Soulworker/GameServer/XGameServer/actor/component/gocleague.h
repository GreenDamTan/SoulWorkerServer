#pragma once

#include "gocbase.h"
#include <cstdint>

/**
 * @brief CGocLeague - Game Object Component for league/guild system
 *
 * Handles league creation, membership, and league-related functionality.
 */
class CGocLeague : public GOComponent {
public:
    CGocLeague();
    virtual ~CGocLeague();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // League info
    int GetLeagueId() const;
    bool IsInLeague() const;
    int GetLeagueRank() const;

    // League management
    bool CreateLeague(const char* pszName);
    bool JoinLeague(int nLeagueId);
    bool LeaveLeague();
    bool DisbandLeague();

    // Member operations
    int GetMemberCount() const;
    int GetMaxMembers() const;
    bool IsLeagueLeader() const;
    bool KickMember(int nPlayerId);

protected:
    int m_nLeagueId;
    int m_nLeagueRank;
    int m_nMemberCount;
};
