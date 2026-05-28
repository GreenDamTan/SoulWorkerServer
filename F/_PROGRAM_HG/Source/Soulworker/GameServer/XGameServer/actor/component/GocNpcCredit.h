#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocNpcCredit - Game Object Component for NPC credit/reputation system
 *
 * Handles NPC-related credits, reputation, and credit-based rewards.
 */
class CGocNpcCredit : public GOComponent {
public:
    CGocNpcCredit();
    virtual ~CGocNpcCredit();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Credit info
    int GetCreditBalance() const;
    int GetTotalCreditsEarned() const;
    int GetCreditTier() const;

    // Credit operations
    void AddCredits(int nAmount);
    bool SpendCredits(int nAmount);
    bool CanAfford(int nAmount) const;

    // Credit rewards
    bool ClaimCreditReward(int nRewardId);
    bool CanClaimCreditReward(int nRewardId) const;

    // Credit tiers
    int GetNextTierThreshold() const;
    float GetTierProgress() const;

protected:
    int m_nCreditBalance;
    int m_nTotalCreditsEarned;
    int m_nCreditTier;
};
