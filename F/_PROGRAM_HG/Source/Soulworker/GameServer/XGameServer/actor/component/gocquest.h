#pragma once

#include "gocbase.h"
#include <cstdint>

/**
 * @brief CGocQuest - Game Object Component for quest management
 *
 * Handles quest state, progress, completion, and quest-related interactions.
 */
class CGocQuest : public GOComponent {
public:
    CGocQuest();
    virtual ~CGocQuest();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Quest state
    int GetQuestState(int nQuestId) const;
    void SetQuestState(int nQuestId, int nState);

    // Quest progress
    int GetQuestProgress(int nQuestId) const;
    void SetQuestProgress(int nQuestId, int nProgress);
    void AddQuestProgress(int nQuestId, int nAmount);

    // Quest operations
    bool StartQuest(int nQuestId);
    bool CompleteQuest(int nQuestId);
    bool AbandonQuest(int nQuestId);
    bool HasQuest(int nQuestId) const;
    bool IsQuestComplete(int nQuestId) const;

    // Quest queries
    int GetActiveQuestCount() const;
    int GetCompletedQuestCount() const;

    // Quest objectives
    int GetObjectiveCount(int nQuestId) const;
    int GetObjectiveProgress(int nQuestId, int nObjectiveIndex) const;

protected:
    int m_nActiveQuestCount;
    int m_nCompletedQuestCount;
};
