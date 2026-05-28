#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocSkill - Game Object Component for actor skills
 *
 * Handles actor skills, passive skills, gesture slots, etc.
 * Estimated ~60 functions based on PDB evidence.
 */
class CGocSkill : public GOComponent {
public:
    CGocSkill();
    virtual ~CGocSkill();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Passive skill functions
    int GetPassiveSkillCount() const;
    void SetPassiveSkillStat(int nSkillId, int nLevel);
    int GetPassiveSkillLevel(int nSkillId) const;

    // Active skill functions
    int GetSkillCount() const;
    bool HasSkill(int nSkillId) const;
    bool LearnSkill(int nSkillId);
    bool UpgradeSkill(int nSkillId);

    // Gesture slot functions
    int GetGestureSlot(int nSlotIndex) const;
    void SetGestureSlot(int nSlotIndex, int nSkillId);
    void ClearGestureSlot(int nSlotIndex);

    // Skill cooldowns
    bool IsSkillOnCooldown(int nSkillId) const;
    float GetSkillCooldownTime(int nSkillId) const;
    void SetSkillCooldown(int nSkillId, float fTime);

    // Skill points
    int GetSkillPoints() const;
    void SetSkillPoints(int nPoints);
    void AddSkillPoints(int nPoints);

    // Skill combo
    int GetCurrentComboCount() const;
    void ResetCombo();
    void IncrementCombo();

protected:
    // Member variables (estimated layout)
    int m_nPassiveSkillCount;
    int m_nSkillCount;
    int m_nSkillPoints;
    int m_nCurrentComboCount;
};
