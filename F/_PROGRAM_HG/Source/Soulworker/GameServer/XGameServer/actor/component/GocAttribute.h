#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocAttribute - Game Object Component for actor attributes
 *
 * Handles actor attributes like HP, FP, stats, game mode state, etc.
 * Estimated ~50 functions based on PDB evidence.
 */
class CGocAttribute : public GOComponent {
public:
    CGocAttribute();
    virtual ~CGocAttribute();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // HP/FP functions
    int GetHP() const;
    int GetMaxHP() const;
    void SetHP(int nHP);
    int GetFP() const;
    int GetMaxFP() const;
    void SetFP(int nFP);
    void FPRestore();

    // Stats functions
    int GetLevel() const;
    void SetLevel(int nLevel);
    int GetExp() const;
    void SetExp(int nExp);

    // Game mode state
    int GetGameModeState() const;
    void SetGameModeState(int nState);

    // Attribute modifiers
    void ApplyModifier(int nModifierType, int nValue);
    void RemoveModifier(int nModifierType, int nValue);

    // Combat stats
    int GetAttackPower() const;
    int GetDefense() const;
    int GetCriticalRate() const;
    int GetCriticalDamage() const;

protected:
    // Member variables (estimated layout)
    int m_nHP;
    int m_nMaxHP;
    int m_nFP;
    int m_nMaxFP;
    int m_nLevel;
    int m_nExp;
    int m_nGameModeState;
    int m_nAttackPower;
    int m_nDefense;
    int m_nCriticalRate;
    int m_nCriticalDamage;
};
