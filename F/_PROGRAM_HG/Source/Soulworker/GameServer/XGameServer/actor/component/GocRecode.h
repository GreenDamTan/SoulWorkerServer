#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocRecode - Game Object Component for recode/upgrade system
 *
 * Handles item recoding, upgrades, and enhancement functionality.
 */
class CGocRecode : public GOComponent {
public:
    CGocRecode();
    virtual ~CGocRecode();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Recode info
    int GetRecodeLevel() const;
    int GetRecodeExp() const;
    int GetRecodeExpNeeded() const;

    // Recode operations
    bool CanRecode() const;
    bool PerformRecode();
    void AddRecodeExp(int nExp);

    // Material requirements
    int GetRequiredMaterial(int nMaterialId) const;
    bool HasRequiredMaterials() const;

    // Success rate
    float GetSuccessRate() const;
    bool IsRecodeInProgress() const;

protected:
    int m_nRecodeLevel;
    int m_nRecodeExp;
    bool m_bRecodeInProgress;
};
