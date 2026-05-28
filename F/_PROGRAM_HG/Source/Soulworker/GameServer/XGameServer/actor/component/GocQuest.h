#pragma once

#include "GOComponent.h"
#include <cstdint>
#include <map>
#include <vector>
#include <ctime>

/**
 * @brief CGocQuest - Game Object Component for quest management
 *
 * Handles quest state, progress, completion, and quest-related interactions.
 * Family ID: 4 (IDA verified)
 */
class CGocQuest : public GOComponent {
public:
    CGocQuest();
    virtual ~CGocQuest();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Static Family ID
    static int GetFamilyID() { return 4; }

    // Initialization
    void Init();
    void Clear();
    void OnUpdate();

    // Episode (Quest) List Operations
    void SendEpisodeList();
    void SendCompleteEpisodeList();
    void SendReqQuestList();

    // Quest State Queries
    bool FindEpisode(std::uint32_t dwEpisodeID) const;
    bool FindCondition(std::uint32_t dwConditionID) const;
    bool IsCompleteEpisode(std::uint32_t dwEpisodeID) const;
    bool IsCompleteCondition(int nConditionID) const;

    // Quest Operations
    bool AcceptQuest(std::uint32_t dwEpisodeID, bool bCheckMaxCount = true);
    bool CompleteQuest(std::uint32_t dwEpisodeID, std::uint32_t dwRewardItemID);
    bool GiveUp(std::uint32_t dwEpisodeID, bool bGiveUpCheck = true);
    bool AcceptQuestByForce(std::uint32_t dwEpisodeID);
    bool CompleteQuestByForce(std::uint32_t dwEpisodeID);
    bool CompleteConditionByForce(std::uint32_t dwConditionID);
    void FailQuest(std::uint32_t dwQuestID);
    bool ResetQuest(std::uint32_t dwEpisodeID);
    void ResetQuestAll();

    // Condition Operations
    void UpdateCondition(std::uint8_t byType, std::uint8_t byTarget,
                         std::uint32_t dwObjectID, int nCount, bool bPartyWith = false);
    bool UpdateCondition(std::uint32_t dwConditionID, int nCount, bool bPartyWith = false);
    void UpdateItemCondition();

    // Episode Management
    bool DeleteEpisode(std::uint32_t dwEpisodeID);
    bool DeleteFailedEpisode(std::uint32_t dwEpisodeID);
    bool SetEpisodeHelper(std::uint32_t dwEpisodeID, std::uint8_t byAddHelper);
    bool SetHelper(std::uint32_t dwEpisodeID, std::uint8_t byHelper);
    void CheckEpisodeCount();

    // Database Sync
    void DBSyncQuestCondition();

protected:
    // Episode map: EpisodeID -> state
    std::map<std::uint32_t, std::uint8_t> m_mapEpisode;

    // Repeat quest map
    std::map<std::uint32_t, std::uint32_t> m_mapRepeatQuest;

    // Last init date for daily reset (using std::time_t instead of ATL::CTime)
    std::time_t m_tLastInitDate = 0;

    // Completed episode bit array (256 bytes = 2048 bits)
    std::uint8_t m_szCompleteEpisode[256] = {};

    // Helper count (max 7)
    int m_nHelperCount = 0;

    // Load state
    bool m_bLoad = false;

    // Sector clear message flag
    bool m_bSendMsgSectorClear = false;

    // Complete flag
    bool m_bComplete = false;
};