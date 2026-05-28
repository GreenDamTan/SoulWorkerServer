#pragma once

#include "gocbase.h"
#include <cstdint>

/**
 * @brief CGocForce - Game Object Component for force/faction system
 *
 * Handles faction alignment, force battles, and force-related functionality.
 */
class CGocForce : public GOComponent {
public:
    CGocForce();
    virtual ~CGocForce();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Force info
    int GetForceId() const;
    int GetForceRank() const;
    int GetForcePoints() const;
    bool IsInForce() const;

    // Force operations
    bool JoinForce(int nForceId);
    bool LeaveForce();
    bool ChangeForce(int nNewForceId);

    // Force contribution
    void AddContributionPoints(int nPoints);
    int GetContributionPoints() const;
    int GetDailyContribution() const;

protected:
    int m_nForceId;
    int m_nForceRank;
    int m_nForcePoints;
    int m_nContributionPoints;
};
