#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocClassEvent - Game Object Component for class event system
 *
 * Handles class-specific events, bonuses, and special class activities.
 */
class CGocClassEvent : public GOComponent {
public:
    CGocClassEvent();
    virtual ~CGocClassEvent();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Event info
    int GetActiveEventId() const;
    bool IsEventActive() const;
    int GetEventProgress() const;
    float GetEventBonus() const;

    // Event operations
    bool JoinEvent(int nEventId);
    bool LeaveEvent();
    void UpdateEventProgress(int nProgress);
    bool CompleteEvent();

    // Event rewards
    bool ClaimEventReward();
    bool CanClaimEventReward() const;

protected:
    int m_nActiveEventId;
    int m_nEventProgress;
    float m_fEventBonus;
    bool m_bEventActive;
};
