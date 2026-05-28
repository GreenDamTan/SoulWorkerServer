#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocSoulMetry - Game Object Component for SoulMetry system
 *
 * Handles SoulMetry mechanics, energy, and related functionality.
 */
class CGocSoulMetry : public GOComponent {
public:
    CGocSoulMetry();
    virtual ~CGocSoulMetry();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // SoulMetry energy
    int GetSoulEnergy() const;
    int GetMaxSoulEnergy() const;
    float GetSoulEnergyRatio() const;
    void SetSoulEnergy(int nEnergy);

    // SoulMetry operations
    bool UseSoulEnergy(int nAmount);
    void RechargeSoulEnergy(int nAmount);
    void DrainSoulEnergy(int nAmount);

    // SoulMetry skills
    bool CanUseSoulSkill(int nSkillId) const;
    bool ActivateSoulSkill(int nSkillId);
    void DeactivateSoulSkill(int nSkillId);

protected:
    int m_nSoulEnergy;
    int m_nMaxSoulEnergy;
    int m_nActiveSkillId;
};
