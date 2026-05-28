// CGocAchieve - Game Object Component for Achievement System
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140029030 - 0x14002EEB6

#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <cstdint>
#include <map>
#include <memory>

// Forward declarations
struct TB_ACHIEVEMENT;
struct TB_MONSTER;
class CAchieveType;
class CAchieve;
class CMover;

/**
 * @brief CGocAchieve - Game Object Component for achievement system
 *
 * Handles achievement tracking, completion, and rewards.
 * Inherits from GOComponent and manages achievement types and progress.
 */
class CGocAchieve : public GOComponent {
public:
    // Constructor (0x140029030)
    CGocAchieve();

    // Destructor (0x1400290E0)
    virtual ~CGocAchieve();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Static method - Get family ID (0x1400487D0)
    static int GetFamilyID();

    // Init (0x140029130) - Initialize achievement system
    void Init();

    // ClearAchieve (0x140029150) - Clear all achievement data
    void ClearAchieve();

    // CheckAchieveReward (0x140029220) - Check if achievement reward can be claimed
    bool CheckAchieveReward(int nBit);

    // SetAchieveReward (0x1400293A0) - Set achievement reward claimed
    void SetAchieveReward(int nBit);

    // SendDBAchieveList (0x140029470) - Send achievement list to DB
    void SendDBAchieveList();

    // SendDBUpdateList (0x140029580) - Send update list to DB and user
    void SendDBUpdateList(ST_ACHIEVE_UPDATE_LIST& stSendUser, ST_ACHIEVE_UPDATE_LIST& stSendDB);

    // AchieveReward (0x140029B70) - Process achievement reward
    bool AchieveReward(int nIndex);

    // UpdateEnduranceAchieve (0x14002A8D0) - Update endurance achievement
    void UpdateEnduranceAchieve(std::uint8_t byEquipType);

    // UpdateQuestAchieve (0x14002AAD0) - Update quest achievement
    void UpdateQuestAchieve(std::uint8_t byQuestType);

    // UpdateMonsterAchieve (0x14002AC80) - Update monster kill achievement
    void UpdateMonsterAchieve(TB_MONSTER* pTBMonster);

    // UpdatemMazeClearAchieve (0x14002AF90) - Update maze clear achievement
    void UpdatemMazeClearAchieve(int nRank, int nClearTime, int nMazeID, std::uint8_t byMazeType);

    // OnUpdatePlayTime (0x14002B2E0) - Update play time achievement
    void OnUpdatePlayTime();

    // GMClearAchieve (0x14002B390) - GM command to clear achievements
    void GMClearAchieve();

    // GMAchieveComplete (0x14002B4B0) - GM command to complete achievement
    void GMAchieveComplete(int nGroupID, int nStep);

    // LoadAchieve (0x14002BDD0) - Load achievement data
    void LoadAchieve(ST_ACHIEVE_BIT& stAchieveBit, ST_ACHIEVE_LIST& stAchieveList,
                     ST_ACHIEVE_CATEGORY& stCatagory, bool bFirst);

    // CheckAchieveComplete (0x14002CA30) - Check if achievement is complete
    bool CheckAchieveComplete(int nBit);

    // InitAchieve (0x14002CAD0) - Initialize achievement from table
    void InitAchieve(TB_ACHIEVEMENT* pTBAchieve);

    // InitAchieve (0x14002CCE0) - Initialize achievement from info
    void InitAchieve(ST_ACHIEVE_INFO& stAchieveInfo);

    // UpdateAchieve1 (0x14002CEB0) - Update achievement count
    void UpdateAchieve1(std::uint16_t wType, int nCount, int nTargetID);

    // UpdateCollect (0x14002D590) - Update collect achievement (immediate send)
    void UpdateCollect(std::uint16_t wType, int nCount, int nTargetID);

    // UpdateCollect (0x14002DD30) - Update collect achievement (batch)
    void UpdateCollect(std::uint16_t wType, int nCount, ST_ACHIEVE_UPDATE_LIST& stSendUser,
                       ST_ACHIEVE_UPDATE_LIST& stSendDB, int nTargetID);

    // LevelUp (0x14002DEC0) - Handle level up achievements
    void LevelUp();

    // EndCollect (0x14002E000) - End collection and send updates
    void EndCollect();

    // GMAchieveCount (0x14002E510) - GM command to set achievement count
    void GMAchieveCount(int nGroupID, int nCount);

    // SendAchieveList (0x14002EA80) - Send achievement list to client
    void SendAchieveList();

    // GetAchievePtr (0x14002EC60) - Get achievement pointer
    std::shared_ptr<CAchieve> GetAchievePtr(std::uint16_t wAchieveType, int nTagetID);

    // UpdateItemUpgradeAchieve (0x14002ED30) - Update item upgrade achievement
    void UpdateItemUpgradeAchieve(std::uint8_t byResult);

    // UpdateHarvestAchieve (0x14002EDA0) - Update harvest achievement
    void UpdateHarvestAchieve(std::uint8_t byHarvestType);

    // GetFirstAchieveID (0x14002EE30) - Get first achievement ID in chain
    std::uint32_t GetFirstAchieveID(TB_ACHIEVEMENT* pTBAchieve, std::uint16_t& wNowCategoryCount);

    // Get owner mover
    CMover* GetOwnerMover() const;

    // Accessors for internal data (needed by other components)
    ST_ACHIEVE_BIT* GetAchieveBit() { return &m_stAchieveBit; }
    ST_ACHIEVE_CATEGORY* GetCategory() { return &m_stCategory; }

protected:
    // Member variables (from IDA structure)
    std::uint64_t m_dw64LastUpdate;                           // Last update timestamp
    ST_ACHIEVE_CATEGORY m_stCategory;                          // Category counts
    ST_ACHIEVE_BIT m_stAchieveBit;                             // Reward bits
    std::map<int, std::shared_ptr<CAchieveType>> m_mpAchieveTypeList;  // Achievement types
};
