#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocDailyMission - Game Object Component for daily mission system
 *
 * Handles daily mission tracking, completion, and rewards.
 */
class CGocDailyMission : public GOComponent {
public:
    CGocDailyMission();
    virtual ~CGocDailyMission();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Mission info
    int GetDailyMissionCount() const;
    int GetCompletedMissionCount() const;
    bool IsMissionComplete(int nMissionId) const;
    int GetMissionProgress(int nMissionId) const;

    // Mission operations
    bool AcceptMission(int nMissionId);
    bool AbandonMission(int nMissionId);
    void UpdateMissionProgress(int nMissionId, int nProgress);
    bool CompleteMission(int nMissionId);

    // Rewards
    bool ClaimMissionReward(int nMissionId);
    bool CanClaimReward(int nMissionId) const;

protected:
    int m_nDailyMissionCount;
    int m_nCompletedMissionCount;
};
