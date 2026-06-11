#pragma once

#include "GOComponent.h"
#include <cstdint>
#include <map>
#include <vector>
#include <ctime>
#include <memory>

// Forward declarations for quest-related structures
struct ST_QUEST_EPISODE;
struct ST_QUEST_REPEAT_INFO;
struct ST_QUEST_FIRST_DROP_ITEM;
struct SGroupID;

// Forward declarations
class CDropItemGroup;
class CDailyMissionInfo;
struct TB_QUEST_EPISODE;
struct TB_QUEST_CONDITION;

// Include structure definitions for map value types
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"

/**
 * @brief CQuestCondition - 任务条件类
 *
 * IDA 构造函数: 0x140125C20
 * 管理单个任务条件的追踪和状态
 */
class CQuestCondition {
public:
    // IDA: 0x140125C20 - 构造函数
    CQuestCondition(std::uint32_t dwQuestID, ST_QUEST_EPISODE* pQuest,
                    int nConditionIndex, TB_QUEST_CONDITION* pTBCondition);

    // IDA: 0x140125C90 - GetConditionID
    std::uint32_t GetConditionID() const { return m_dwQuestID; }

    // IDA: 0x140125CB0 - GetConditionType
    std::uint8_t GetConditionType() const;

    // IDA: 0x140125CE0 - GetNeedCompletionCondition
    int GetNeedCompletionCondition();

    // IDA: 0x140125D00 - AddConditionValue
    void AddConditionValue(std::int8_t nValue);

    // IDA: 0x140125D30 - GetConditionValue
    std::uint8_t GetConditionValue() const;

    // IDA: 0x140125D50 - SetConditionValue
    void SetConditionValue(std::uint8_t byValue);

    // IDA: 0x140125D70 - IsCompleteCondition
    bool IsCompleteCondition();

    // IDA: 0x14005AB80 - GetQuestID
    std::uint32_t GetQuestID() const { return m_dwQuestID; }

private:
    std::uint32_t m_dwQuestID = 0;              // 任务ID
    int m_nConditionIndex = 0;                   // 条件索引
    TB_QUEST_CONDITION* m_pTBCondition = nullptr; // 条件表指针
    ST_QUEST_EPISODE* m_pQuest = nullptr;        // 章节指针
    ST_QUEST_EPISODE_CONDITION* m_pCondition = nullptr; // 条件数据指针
};

/**
 * @brief CGocQuest - Game Object Component for quest management
 *
 * Handles quest state, progress, completion, and quest-related interactions.
 * Family ID: 4 (IDA verified)
 *
 * IDA Constructor: 0x140125DD0
 * IDA Destructor: 0x140125F00
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
    bool Init();                           // IDA: 0x140125F90
    void Clear();                          // IDA: 0x140125FB0
    void OnUpdate();                       // IDA: 0x1401250A0

    // Episode (Quest) List Operations
    void SendEpisodeList();                // IDA: 0x140126240
    void SendCompleteEpisodeList();        // IDA: 0x140126400
    void SendReqQuestList();               // IDA: 0x140128EF0
    void SetEpisodeList(std::uint32_t dwEpisodeID, const ST_QUEST_EPISODE* pEpisode);  // Load episode from DB
    void SetConditionList(std::uint32_t dwConditionID, std::shared_ptr<CQuestCondition> pCondition);  // Load condition from DB
    void SetCompleteEpisodeList(const std::uint8_t* pData, std::size_t nSize);  // Load complete episode bits from DB

    // Quest State Queries
    bool FindEpisode(std::uint32_t dwEpisodeID) const;           // IDA: 0x1401264E0
    bool FindCondition(std::uint32_t dwConditionID) const;       // IDA: 0x140126560
    bool IsCompleteEpisode(std::uint32_t dwEpisodeID) const;     // IDA: 0x140126690
    bool IsCompleteCondition(int nConditionID) const;            // IDA: 0x140138970
    bool ValidCompleteEpisode(std::uint32_t dwEpisodeID) const;  // IDA: 0x140127170

    // Quest Operations
    bool AcceptQuest(std::uint32_t dwEpisodeID, bool bCheckMaxCount = true);           // IDA: 0x14012BBD0
    bool CompleteQuest(std::uint32_t dwEpisodeID, std::uint32_t dwRewardItemID);       // IDA: 0x14012F100
    bool GiveUp(std::uint32_t dwEpisodeID, bool bGiveUpCheck = true);                  // IDA: 0x1401324A0
    bool AcceptQuestByForce(std::uint32_t dwEpisodeID);                                // IDA: 0x14012E1F0
    bool CompleteQuestByForce(std::uint32_t dwEpisodeID);                              // IDA: 0x140130C50
    bool CompleteConditionByForce(std::uint32_t dwConditionID);                        // IDA: 0x140126890
    void FailQuest(std::uint32_t dwQuestID);                                           // IDA: 0x140139B60
    bool ResetQuest(std::uint32_t dwEpisodeID);                                        // IDA: 0x140139610
    void ResetQuestAll();                                                              // IDA: 0x140137650

    // Quest Item Operations
    bool CheckAcceptQuestByItem(std::uint32_t dwEpisodeID, int* nError);               // IDA: 0x14012CE50
    bool AcceptQuestByItem(std::uint32_t dwEpisodeID, void* psCreateItem, void* psUpdateItem); // IDA: 0x14012CEE0

    // SetQuestAddObject - Add quest objects (items)
    bool SetQuestAddObject(std::uint32_t dwEpisodeID, std::uint8_t* byType, std::uint32_t* dwID);  // IDA: 0x140138F20
    bool SetQuestAddObject(std::uint32_t dwEpisodeID, void* psCreateItem, void* psUpdateItem);     // IDA: 0x1401392C0

    // Get needed item count for condition
    int GetNeedConditionItemCount(std::uint32_t dwConditionID, std::uint32_t dwItemID);  // IDA: 0x140139780

    // Condition Operations
    void UpdateCondition(std::uint8_t byType, std::uint8_t byTarget,
                         std::uint32_t dwObjectID, int nCount, bool bPartyWith = false);  // IDA: 0x140133D80
    bool UpdateCondition(std::uint32_t dwConditionID, int nCount, bool bPartyWith = false); // IDA: 0x140135820
    void UpdateItemCondition();                                                            // IDA: 0x140133330
    void UpdateMazeGameMode(int eType, std::int16_t nMazeID, bool bPartyWith);            // IDA: 0x140133900
    bool CompleteCondition(std::uint32_t dwQuestID, std::shared_ptr<CQuestCondition> pCondition);  // Complete a condition
    bool CheckUpdateCondition(std::uint32_t dwConditionID, int nParam);  // Check if condition can be updated

    // Episode Management
    bool DeleteEpisode(std::uint32_t dwEpisodeID);             // IDA: 0x140127730
    bool DeleteFailedEpisode(std::uint32_t dwEpisodeID);       // IDA: 0x140127810
    bool SetEpisodeHelper(std::uint32_t dwEpisodeID, std::uint8_t byAddHelper);  // IDA: 0x140127A70
    bool SetHelper(std::uint32_t dwEpisodeID, std::uint8_t byHelper);            // IDA: 0x140132690
    void CheckEpisodeCount();                                  // IDA: 0x140128530

    // Episode Completion
    bool CompleteEpisode(std::uint32_t dwEpisodeID, void* stGetInfo);  // IDA: 0x1401281F0
    void CompleteEpisodeAdd(void* psAddList);                          // IDA: 0x140128C00

    // Database Sync
    void DBSyncQuestCondition();                               // IDA: 0x1401397C0
    const char* GetCompleteEpisode() const;                    // IDA: 0x140124DB0

    // Repeat Quest Operations
    bool CheckAcceptRepeatQuest(std::uint32_t dwEpisodeID, bool bCheck, int* pnError);  // IDA: 0x14013A090
    void AcceptRepeatQuest(std::uint32_t dwEpisodeID);                                  // IDA: 0x14013B6D0
    void CompleteRepeatQuest(std::uint32_t dwEpisodeID);                                // IDA: 0x14013B930
    void ResetRepeatQuest(std::uint32_t dwEpisodeID);                                   // IDA: 0x14013BBD0
    void DBAddRepeatQuest(std::uint32_t dwEpisodeID);                                   // IDA: 0x14013BE70

    // Interaction Object
    void EnableInteractionObject(std::uint32_t dwConditionID, int nParam);    // IDA: 0x140138A60
    void DisableInteractionObject(std::uint32_t dwConditionID, int nType, int nParam);  // IDA: 0x140138CE0

    // Sector Clear Quest
    void UpdateQuestConditionForSectorClear();    // IDA: 0x1401393D0
    void InitQuestConditionForSectorClear();      // IDA: 0x140138E40

    // Quest Update Helpers
    void ClearUpdateQuestCondition_GiveUp(std::uint32_t dwEpisodeID);  // Called when giving up quest
    void UpdateQuestRespawn(int nConditionID);                           // Update respawn on condition complete
    void DBUpdateEpisodeInfo(std::uint32_t dwEpisodeID, ST_QUEST_EPISODE* pEpisode);  // Sync episode to DB
    void ClearUpdateQuestCondition(int nConditionID);                    // Clear condition update state
    void CompleteQuestForNewChar(int nType, float fParam);               // Complete quest for new character
    void CheckSyncQuestCondition(std::shared_ptr<CQuestCondition> pCondition);  // Check and sync quest condition

    // Quest First Drop Item
    void AddQuestFirstDropItem(std::uint32_t dwQuestID, std::uint32_t dwItemID, int nCount);  // IDA: 0x14013C260
    void SetQuestFirstDropItem(std::uint32_t dwEpisodeID, int nItemID, int nCount);  // IDA: 0x14013B1B0
    bool CheckQuestFirstDropItem(std::uint32_t dwQuestID, std::uint32_t dwItemID = 0);  // IDA: 0x14013C370

    // Send Message Sector Clear
    bool IsSendMsgSectorClear() const;      // IDA: 0x140310530
    void SetSectorClearQuestState(int nSectorID, bool bFlag);  // IDA: 0x14013A200
    bool IsQuestCondtionForSectorClear();   // IDA: 0x14013A230

    // DB Sync Check
    bool CheckQuestDBSync();                // IDA: 0x14019D1B0

    // Complete Quest Request Flag
    bool GetCompleteQuestReq() const;       // IDA: 0x1405971E0
    void SetCompleteQuestQeq(bool bComplete);  // IDA: 0x140597200

    // Get Repeat Quest Info
    void GetRepeatQuestInfo(std::uint32_t dwEpisodeID, ST_QUEST_REPEAT_INFO* pInfo);  // IDA: 0x14013AFF0
    void SetRepeatQuestList(std::uint32_t dwEpisodeID, const ST_QUEST_REPEAT_INFO* pInfo);  // IDA: 0x14013AD20

protected:
    // Internal helpers
    void ClearSyncQuestCondition_All();     // IDA: 0x14013AB20

protected:
    // Episode map: EpisodeID -> ST_QUEST_EPISODE
    // IDA-verified: map stores ST_QUEST_EPISODE objects directly (not pointers)
    std::map<std::uint32_t, ST_QUEST_EPISODE> m_mapEpisode;

    // Condition map using std::shared_ptr
    // Key: ConditionID, QuestID, ConditionType
    std::map<std::uint32_t, std::shared_ptr<class CQuestCondition>> m_mapCondition;

    // Update condition map
    std::map<int, std::shared_ptr<class CDropItemGroup>> m_mapUpdateCondition;

    // Repeat quest map
    std::map<std::uint32_t, ST_QUEST_REPEAT_INFO> m_mapRepeatQuest;

    // Quest first drop item map
    std::map<std::uint32_t, ST_QUEST_FIRST_DROP_ITEM> m_mapQuestFirstDrop;

    // Last init date for daily reset
    std::time_t m_tLastInitDate = 0;

    // Completed episode bit array (256 bytes = 2048 bits)
    std::uint8_t m_szCompleteEpisode[256] = {};

    // Quest state clear array
    std::uint8_t m_bQuestStateClear[256] = {};

    // Helper count (max 7)
    int m_nHelperCount = 0;

    // Load state
    bool m_bLoad = false;

    // Sector clear message flag
    bool m_bSendMsgSectorClear = false;

    // Complete flag
    bool m_bComplete = false;
};
