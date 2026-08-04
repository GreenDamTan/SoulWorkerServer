#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include <cstdint>
#include <ctime>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

// Forward declarations
class CDropItemGroup;
class CDailyMissionInfo;
struct TB_QUEST_CONDITION;
// PDB: GameServer.pdb type records 0xd07d and 0xd07f.
enum E_CONDITION_TARGET : int {
    eCONDITION_TARGET_NPC = 0,
    eCONDITION_TARGET_MONSTER = 1,
    eCONDITION_TARGET_PC = 2,
    eCONDITION_TARGET_ITEM = 3,
    eCONDITION_TARGET_OBJECT = 4,
    eCONDITION_TARGET_COLLISION = 5,
    eCONDITION_TARGET_TRIGGER = 6,
    eCONDITION_TARGET_MAZE = 7,
    eCONDITION_TARGET_MONSTER_GROUP = 8,
    eCONDITION_TARGET_SECTOR = 9,
    eCONDITION_TARGET_SKILL_DECK = 10,
    eCONDITION_TARGET_EVENT = 11,
};

enum E_CONDITION_TYPE : int {
    eCONDITION_TYPE_TALK = 0,
    eCONDITION_TYPE_MOVE = 1,
    eCONDITION_TYPE_TRIGGER = 2,
    eCONDITION_TYPE_HUNT = 3,
    eCONDITION_TYPE_COLLECT = 4,
    eCONDITION_TYPE_DELIVER = 5,
    eCONDITION_TYPE_PROTECT = 6,
    eCONDITION_TYPE_SURVIAL = 7,
    eCONDITION_TYPE_TRACE = 8,
    eCONDITION_TYPE_GUARD = 9,
    eCONDITION_TYPE_CLEAR = 10,
    eCONDITION_TYPE_BUY = 11,
    eCONDITION_TYPE_SELL = 12,
    eCONDITION_TYPE_MAKE = 13,
    eCONDITION_TYPE_UPGRADE = 14,
    eCONDITION_TYPE_SKILL = 15,
    eCONDITION_TYPE_DISASSEMBLE = 16,
    eCONDITION_TYPE_AKASHIC_MAKE = 17,
    eCONDITION_TYPE_CULTIVATION = 18,
    eCONDITION_TYPE_HARVEST = 19,
    eCONDITION_TYPE_HUNT_ALL = 20,
    eCONDITION_TYPE_END = 21,
};

static_assert(sizeof(E_CONDITION_TARGET) == 4,
              "E_CONDITION_TARGET must match the GameServer PDB");
static_assert(sizeof(E_CONDITION_TYPE) == 4,
              "E_CONDITION_TYPE must match the GameServer PDB");

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
    std::uint32_t GetConditionID() const;

    // IDA: 0x140125CB0 - GetConditionType
    std::uint8_t GetConditionType() const;

    // IDA: 0x140125CE0 - GetNeedCompletionCondition
    int GetNeedCompletionCondition();

    // IDA: 0x140125D00 - AddConditionValue
    void AddConditionValue(int nValue);

    // IDA: 0x140125D30 - GetConditionValue
    std::uint8_t GetConditionValue() const;

    // IDA: 0x140125D50 - SetConditionValue
    void SetConditionValue(std::uint8_t byValue);

    // IDA: 0x140125D70 - IsCompleteCondition
    bool IsCompleteCondition();

    int GetConditionIndex() const { return m_nConditionIndex; }
    void SetConditionTable(TB_QUEST_CONDITION* pTBCondition) { m_pTBCondition = pTBCondition; }
    TB_QUEST_CONDITION* GetConditionTable() { return m_pTBCondition; }
    ST_QUEST_EPISODE* GetQuestEpisode() { return m_pQuest; }
    ST_QUEST_CONDITION* GetCondition() { return m_pCondition; }

    // IDA: 0x14005AB80 - GetQuestID
    std::uint32_t GetQuestID() const { return m_dwQuestID; }

    // Static helper - converts actor ID to quest ID (helper function)
    // Used by CMoverEx::ProcessMove to check quest targeting
    static std::uint32_t GetQuestID(std::uint32_t dwActorID) {
        // Quest IDs are typically derived from actor IDs
        // This is a helper to convert actor ID to quest ID
        return dwActorID;  // Default: same as actor ID
    }

    // Overload for UXActorID
    static std::uint32_t GetQuestID(UXActorID uxActorID) {
        return GetQuestID(uxActorID.dwActorID);
    }

private:
    std::uint32_t m_dwQuestID = 0;              // 任务ID
    ST_QUEST_EPISODE* m_pQuest = nullptr;        // 章节指针
    ST_QUEST_CONDITION* m_pCondition = nullptr; // 条件数据指针
    int m_nConditionIndex = 0;                   // 条件索引
    TB_QUEST_CONDITION* m_pTBCondition = nullptr; // 条件表指针
};

static_assert(sizeof(CQuestCondition) == 0x28,
              "CQuestCondition must match the GameServer PDB");

struct quest_indices {
    enum INDEX {
        IDX_UNIQUE_CONDITION_ID,
        IDX_NON_UNIQUE_QUEST_ID,
        IDX_NON_UNIQUE_CONDITION_TYPE,
        IDX_END,
    };
};

// Preserves the PDB-backed Boost.MultiIndex lookup and ordering semantics.
class GreenDamTan_QuestConditionContainer {
public:
    using value_type = std::shared_ptr<CQuestCondition>;
    using ConditionTypeIndex = std::multimap<std::uint8_t, value_type>;
    using ConditionTypeRange = std::pair<ConditionTypeIndex::const_iterator,
                                         ConditionTypeIndex::const_iterator>;

    bool insert(const value_type& condition) {
        if (!condition) {
            return false;
        }

        const auto [it, inserted] =
            m_byConditionID.emplace(condition->GetConditionID(), condition);
        if (!inserted) {
            return false;
        }

        m_byQuestID.emplace(condition->GetQuestID(), condition);
        m_byConditionType.emplace(condition->GetConditionType(), condition);
        return true;
    }

    const value_type* FindByConditionID(std::uint32_t dwConditionID) const {
        const auto it = m_byConditionID.find(dwConditionID);
        return it != m_byConditionID.end() ? &it->second : nullptr;
    }

    ConditionTypeRange GetByConditionType(std::uint8_t byConditionType) const {
        return m_byConditionType.equal_range(byConditionType);
    }

    void EraseByQuestID(std::uint32_t dwQuestID) {
        const auto range = m_byQuestID.equal_range(dwQuestID);
        for (auto it = range.first; it != range.second; ++it) {
            const value_type& condition = it->second;
            m_byConditionID.erase(condition->GetConditionID());

            const auto typeRange =
                m_byConditionType.equal_range(condition->GetConditionType());
            for (auto typeIt = typeRange.first; typeIt != typeRange.second;
                 ++typeIt) {
                if (typeIt->second == condition) {
                    m_byConditionType.erase(typeIt);
                    break;
                }
            }
        }
        m_byQuestID.erase(range.first, range.second);
    }

    void clear() {
        m_byConditionID.clear();
        m_byQuestID.clear();
        m_byConditionType.clear();
    }

private:
    std::unordered_map<std::uint32_t, value_type> m_byConditionID;
    std::multimap<std::uint32_t, value_type> m_byQuestID;
    ConditionTypeIndex m_byConditionType;
};

using QuestConditionContainer = GreenDamTan_QuestConditionContainer;

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
    void SetEpisodeList(std::map<std::uint32_t, ST_QUEST_EPISODE>& mapEpisode);  // IDA: 0x14012A000
    void SetCompleteEpisodeList(const std::uint8_t* pData, std::size_t nSize);  // Load complete episode bits from DB

    // Quest State Queries
    bool FindEpisode(std::uint32_t dwEpisodeID);                 // IDA: 0x1401264E0
    bool FindCondition(std::uint32_t dwConditionID);             // IDA: 0x140126560
    bool IsCompleteEpisode(std::uint32_t dwEpisodeID);           // IDA: 0x140126690
    bool IsCompleteCondition(int nConditionID);                  // IDA: 0x140138970
    bool ValidCompleteEpisode(std::uint32_t dwEpisodeID) const;  // IDA: 0x140127170

    // Quest Operations
    bool AcceptQuest(std::uint32_t dwEpisodeID, bool bCheckMaxCount = true);           // IDA: 0x14012BBD0
    bool CompleteQuest(std::uint32_t dwEpisodeID, std::uint32_t dwRewardItemID);       // IDA: 0x14012F100
    bool GiveUp(std::uint32_t dwEpisodeID, bool bGiveUpCheck = true);                  // IDA: 0x1401324A0
    bool AcceptQuestByForce(std::uint32_t dwEpisodeID);                                // IDA: 0x14012E1F0
    bool CompleteQuestByForce(std::uint32_t dwEpisodeID);                              // IDA: 0x140130C50
    bool CompleteConditionByForce(std::uint32_t dwConditionID);                        // IDA: 0x140127890
    void FailQuest(std::uint32_t dwQuestID);                                           // IDA: 0x140139B60
    bool ResetQuest(std::uint32_t dwEpisodeID);                                        // IDA: 0x140139610
    void ResetQuestAll();                                                              // IDA: 0x140137650

    // Quest Item Operations
    bool CheckAcceptQuestByItem(std::uint32_t dwEpisodeID, int* nError);               // IDA: 0x14012CE50
    bool AcceptQuestByItem(std::uint32_t dwEpisodeID,
                           PS_RES_STORAGE_INFO& psCreateItem,
                           PS_RES_STORAGE_INFO& psUpdateItem); // IDA: 0x14012CEE0

    // SetQuestAddObject - Add quest objects (items)
    bool SetQuestAddObject(std::uint32_t dwEpisodeID, std::uint8_t* byType, std::uint32_t* dwID);  // IDA: 0x140138F20
    bool SetQuestAddObject(std::uint32_t dwEpisodeID,
                           PS_RES_STORAGE_INFO& psCreateItem,
                           PS_RES_STORAGE_INFO& psUpdateItem); // IDA: 0x1401392C0

    // Get needed item count for condition
    int GetNeedConditionItemCount(std::uint32_t dwConditionID, std::uint32_t dwItemID);  // IDA: 0x140139780

    // Condition Operations
    void UpdateCondition(E_CONDITION_TYPE eType, E_CONDITION_TARGET eTarget,
                         std::uint32_t dwObjectID, int nCount, bool bPartyWith = false);  // IDA: 0x140133D80
    bool UpdateCondition(std::uint32_t dwConditionID, int nCount, bool bPartyWith = false); // IDA: 0x140135820
    void UpdateItemCondition();                                                            // IDA: 0x140133330
    void UpdateMazeGameMode(E_CONDITION_TYPE eType, std::int16_t nMazeID, bool bPartyWith);            // IDA: 0x140133900
    bool CompleteCondition(std::uint32_t dwQuestID, std::shared_ptr<CQuestCondition> pCondition);  // Complete a condition
    bool CheckUpdateCondition(std::uint32_t dwConditionID, std::int64_t nParam);  // Check if condition can be updated

    // Episode Management
    bool DeleteEpisode(std::uint32_t dwEpisodeID);             // IDA: 0x140127730
    bool DeleteFailedEpisode(std::uint32_t dwEpisodeID);       // IDA: 0x140127810
    bool SetEpisodeHelper(std::uint32_t dwEpisodeID, std::uint8_t byAddHelper);  // IDA: 0x140127A70
    bool SetHelper(std::uint32_t dwEpisodeID, std::uint8_t byHelper);            // IDA: 0x140132690
    void CheckEpisodeCount();                                  // IDA: 0x140129530

    // Episode Completion
    bool CompleteEpisode(std::uint32_t dwEpisodeID, void* stGetInfo);  // IDA: 0x1401281F0
    void CompleteEpisodeAdd(void* psAddList);                          // IDA: 0x140128C00

    // Database Sync
    void DBSyncQuestCondition();                               // IDA: 0x14013A7C0
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
    void UpdateQuestRespawn(int nConditionID);                           // Update respawn on condition complete
    void DBUpdateEpisodeInfo(std::uint32_t dwEpisodeID, ST_QUEST_EPISODE& stEpisode);  // Sync episode to DB
    void CompleteQuestForNewChar(bool bSend, float fParam);               // Complete quest for new character

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

private:
    void ClearSyncQuestCondition_All();     // IDA: 0x14013AB20
    void ClearUpdateQuestCondition(std::uint32_t dwConditionID);  // IDA: 0x14013A8E0
    void ClearUpdateQuestCondition_GiveUp(std::uint32_t dwEpisodeID);  // IDA: 0x14013A960
    void CheckSyncQuestCondition(std::shared_ptr<CQuestCondition> pCondition);  // IDA: 0x14013AA20

protected:
    // Pending condition updates keyed by ConditionID.
    std::map<std::uint32_t, std::shared_ptr<CQuestCondition>> m_mapUpdateCondition;

    // Episode map: EpisodeID -> ST_QUEST_EPISODE
    // IDA-verified: map stores ST_QUEST_EPISODE objects directly (not pointers)
    std::map<std::uint32_t, ST_QUEST_EPISODE> m_mapEpisode;

    QuestConditionContainer m_mapCondition;

    // Repeat quest map
    std::map<std::uint32_t, ST_QUEST_REPEAT_INFO> m_mapRepeatQuest;

    // Last init date for daily reset
    std::time_t m_tLastInitDate = 0;

    // Quest first drop item map
    std::map<std::uint32_t, ST_QUEST_FIRST_DROP_ITEM> m_mapQuestFirstDrop;

    // Helper count (max 7)
    int m_nHelperCount = 0;

    // Completed episode bits include the PDB-backed sentinel byte.
    char m_szCompleteEpisode[257] = {};

    // Sector-clear state has three PDB-backed slots.
    bool m_bQuestStateClear[3] = {};

    // Load state
    bool m_bLoad = false;

    // Sector clear message flag
    bool m_bSendMsgSectorClear = false;

    // Complete flag
    bool m_bComplete = false;
};
