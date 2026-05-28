#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocHelper - Game Object Component for helper/assistant system
 *
 * Handles helper NPCs, auto-battle assistants, and helper-related functionality.
 */
class CGocHelper : public GOComponent {
public:
    CGocHelper();
    virtual ~CGocHelper();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Helper info
    bool HasActiveHelper() const;
    int GetHelperId() const;
    int GetHelperLevel() const;

    // Helper operations
    bool SummonHelper(int nHelperId);
    bool DismissHelper();
    bool SetHelperMode(int nMode);

    // Helper skills
    bool UseHelperSkill(int nSkillId);
    int GetHelperSkillCount() const;

protected:
    int m_nHelperId;
    int m_nHelperLevel;
    int m_nHelperMode;
    bool m_bHelperActive;
};
