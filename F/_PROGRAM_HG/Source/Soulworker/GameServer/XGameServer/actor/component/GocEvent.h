#pragma once

#include "GOComponent.h"
#include <cstdint>

/**
 * @brief CGocEvent - Game Object Component for event handling
 *
 * Handles game events, triggers, and event-based interactions.
 */
class CGocEvent : public GOComponent {
public:
    CGocEvent();
    virtual ~CGocEvent();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Event handling
    void OnEvent(int nEventId, void* pEventData);
    bool HasEvent(int nEventId) const;

    // Event registration
    void RegisterEvent(int nEventId);
    void UnregisterEvent(int nEventId);
    void ClearEvents();

    // Event triggers
    void TriggerEvent(int nEventId);
    void TriggerDelayedEvent(int nEventId, float fDelay);

    // Event queries
    int GetActiveEventCount() const;
    int GetEventState(int nEventId) const;

protected:
    int m_nActiveEventCount;
};
