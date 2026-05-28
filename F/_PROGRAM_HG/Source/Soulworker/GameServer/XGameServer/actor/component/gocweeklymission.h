#pragma once

#include "gocbase.h"
#include <cstdint>

/**
 * @brief CGocWeeklyMission - Game Object Component for weekly mission system
 *
 * Handles weekly mission tracking, completion, and rewards.
 */
class CGocWeeklyMission : public GOComponent {
public:
    CGocWeeklyMission();
    virtual ~CGocWeeklyMission();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Mission info
    int GetWeeklyMissionCount() const;
    int GetCompletedMissionCount() const;
    bool IsMissionComplete(int nMissionId) const;
    int GetMissionProgress(int nMissionId) const;

    // Mission operations
    bool AcceptMission(int nMissionId);
    bool AbandonMission(int nMissionId);
    void UpdateMissionProgress(int nMissionId, int nProgress);
    bool CompleteMission(int nMissionId);

    // Weekly reset
    int GetCurrentWeek() const;
    void OnWeeklyReset();

protected:
    int m_nWeeklyMissionCount;
    int m_nCompletedMissionCount;
    int m_nCurrentWeek;
};
