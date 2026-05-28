#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocAchieve - Game Object Component for achievement system
 *
 * Handles achievement tracking, completion, and rewards.
 */
class CGocAchieve : public GOComponent {
public:
    CGocAchieve();
    virtual ~CGocAchieve();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Achievement info
    int GetAchievementCount() const;
    int GetCompletedAchievementCount() const;
    bool IsAchievementComplete(int nAchievementId) const;
    int GetAchievementProgress(int nAchievementId) const;

    // Achievement operations
    void UpdateAchievementProgress(int nAchievementId, int nProgress);
    bool CompleteAchievement(int nAchievementId);

    // Rewards
    bool ClaimAchievementReward(int nAchievementId);
    bool CanClaimReward(int nAchievementId) const;
    int GetUnclaimedRewardCount() const;

    // Achievement points
    int GetTotalAchievementPoints() const;

protected:
    int m_nAchievementCount;
    int m_nCompletedAchievementCount;
    int m_nTotalAchievementPoints;
};
