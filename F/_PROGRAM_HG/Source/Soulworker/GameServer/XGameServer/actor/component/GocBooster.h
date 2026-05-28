#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocBooster - Game Object Component for boost/buff system
 *
 * Handles active boosts, buffs, and temporary stat enhancements.
 */
class CGocBooster : public GOComponent {
public:
    CGocBooster();
    virtual ~CGocBooster();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Booster info
    int GetActiveBoosterCount() const;
    bool HasBooster(int nBoosterId) const;
    float GetBoosterRemainingTime(int nBoosterId) const;

    // Booster operations
    bool ActivateBooster(int nBoosterId, float fDuration);
    bool DeactivateBooster(int nBoosterId);
    void DeactivateAllBoosters();

    // Experience boosters
    float GetExpBoostMultiplier() const;
    float GetDropRateBoostMultiplier() const;

protected:
    int m_nActiveBoosterCount;
    float m_fExpBoostMultiplier;
    float m_fDropRateBoostMultiplier;
};
