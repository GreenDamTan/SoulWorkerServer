#pragma once

#include "gocbase.h"
#include <cstdint>

/**
 * @brief CGocAkashicRecord - Game Object Component for Akashic Record system
 *
 * Handles Akashic Record unlocks, progression, and bonuses.
 */
class CGocAkashicRecord : public GOComponent {
public:
    CGocAkashicRecord();
    virtual ~CGocAkashicRecord();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Akashic info
    int GetRecordCount() const;
    int GetUnlockedRecordCount() const;
    bool IsRecordUnlocked(int nRecordId) const;
    int GetRecordLevel(int nRecordId) const;

    // Record operations
    bool UnlockRecord(int nRecordId);
    bool UpgradeRecord(int nRecordId);
    int GetUpgradeCost(int nRecordId) const;

    // Bonuses
    float GetStatBonus(int nStatType) const;
    int GetTotalBonusPoints() const;

protected:
    int m_nRecordCount;
    int m_nUnlockedRecordCount;
    int m_nTotalBonusPoints;
};
