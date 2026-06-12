#pragma once

#include "GOComponent.h"
#include <cstdint>
#include <map>
#include <vector>

// Forward declarations
class CMover;
class CUser;
class XGameServer;
struct TB_MAZE_INFO;
struct TB_MAZEREWARD_STANDARD;
struct TB_MAZEREWARD_RANK;
struct TB_MAZEREWARD_NORMAL;
struct TB_MAZEREWARD_REVISION;
struct TB_MAZEREWARD_TIME;
struct TB_MAZEREWARD_DIFFICULTY;
struct TB_MAZEREWARD_LEVEL;
struct TB_MAZEREWARD_PARTYVALUE;

// Forward struct declarations for maze-related types
struct ST_MAZE_CLEAR_INFO;
struct ST_MAZE_CASH_REWARD_INFO;
struct ST_DISTRICT_STATE;
struct ST_DISTRICT_STATE_LIST;
struct ST_MAZE_STATE_LIST;
struct PS_REWARD_PARTY_SCORE;
struct PS_MAZE_ENTER_LIMIT_COUNT_GROUP;
struct PS_UPDATE_MAZE_ENTER_LIMIT_COUNT;
struct ST_USER_RANKING_INFO;
struct ST_USER_LAST_RANKING_INFO;
struct ST_RANKING_KEY;
struct RANKING_LIST_DATA;
struct ST_RANDOM_BUFF;
struct PS_KILLED_USER_INFOS;
struct ST_CREATE_ITEM;
struct ST_PRIVATE_SHOP_LIST;
struct ST_LEAGUE_RECORD_LIST;

/**
 * @brief CGocRecode - Game Object Component for maze recording and rewards
 *
 * This component handles:
 * - Maze clear records and scoring
 * - Reward calculation (exp, money, items)
 * - Party score tracking
 * - Infinite tower progress
 * - Ranking data
 * - Over-indulgence system
 * - Enter maze limit counts
 *
 * IDA: Constructor at 0x140144B90, Destructor at 0x140144DD0
 */
class CGocRecode : public GOComponent {
public:
    // IDA: ??0CGocRecode@@QEAA@XZ (0x140144B90)
    CGocRecode();

    // IDA: ??1CGocRecode@@UEAA@XZ (0x140144DD0)
    virtual ~CGocRecode();

    // GOComponent interface
    // IDA: ?Init@CGocRecode@@QEAAXXZ (0x140144EE0)
    void Init();

    // IDA: ?OnUpdate@CGocRecode@@QEAAXXZ (0x1401455A0)
    void OnUpdate();

    // Helper to get owner user (similar to CGocFriend::GetOwnerUser)
    CUser* GetOwnerUser() const;

    // Static family ID
    // IDA: ?GetFamilyID@CGocRecode@@SAHXZ (0x1400487C0)
    static int GetFamilyID() { return 10; }

    // Maze initialization and rewards
    // IDA: ?InitMaze@CGocRecode@@QEAAXH@Z (0x140144F00)
    void InitMaze(int nMazeID);

    // IDA: ?MazeReward@CGocRecode@@QEAAHK_NH@Z (0x140145930)
    int MazeReward(unsigned int dwPlayTime, bool bApplyReward, int nPartyMemberCount);

    // Clear functions
    // IDA: ?Clear@CGocRecode@@QEAAXXZ (0x1401451B0)
    void Clear();

    // Recode operations
    // IDA: ?SetRecode@CGocRecode@@QEAAXHH@Z (0x140145810)
    void SetRecode(unsigned int nRecodeType, int nCount);

    // IDA: ?SetFullRecode@CGocRecode@@QEAAXPEAH@Z (0x140148110)
    void SetFullRecode(int* nRecode);

    // IDA: ?TestRandomScore@CGocRecode@@QEAAXXZ (0x1401480B0)
    void TestRandomScore();

    // Reward value setters
    // IDA: ?SetRewardValue@CGocRecode@@QEAAXMHMM@Z (0x1401458A0)
    void SetRewardValue(float fPlayPoint, unsigned int nRank, float fExpValue, float fMoneyValue);

    // IDA: ?SetPlayPointOrder@CGocRecode@@QEAAXHH@Z (0x140146F00)
    void SetPlayPointOrder(int nCount, int nOrder);

    // Calculation functions
    // IDA: ?CalculateFirst@CGocRecode@@QEAAXAEAMH@Z (0x140145D90)
    void CalculateFirst(float& fPlayPoint, int nPartyMemberCount);

    // IDA: ?CalculateSecond@CGocRecode@@QEAAXAEAM@Z (0x1401468D0)
    void CalculateSecond(float& fPlayPoint);

    // IDA: ?CalculateThird@CGocRecode@@QEAAXAEAM@Z (0x140146B10)
    void CalculateThird(float& fPlayPoint);

    // IDA: ?CalculateFourth@CGocRecode@@QEAAXAEAM@Z (0x140146C20)
    void CalculateFourth(float& fPlayPoint);

    // IDA: ?GetRank@CGocRecode@@QEAAXM@Z (0x140146D80)
    void GetRank(float fPlayPoint);

    // Apply rewards
    // IDA: ?ApplyReward@CGocRecode@@QEAAXAEAUPS_REWARD_PARTY_SCORE@@@Z (0x140146FB0)
    void ApplyReward(PS_REWARD_PARTY_SCORE& psPartyScore);

    // IDA: ?ApplyReward@CGocRecode@@QEAAXXZ (0x140147730)
    void ApplyReward();

    // IDA: ?SendReward@CGocRecode@@QEAAXXZ (0x140147CC0)
    void SendReward();

    // IDA: ?SetRewardItem@CGocRecode@@QEAAX_N@Z (0x14014ADD0)
    void SetRewardItem(bool bFlag);

    // IDA: ?GetRewardItem@CGocRecode@@QEAAXXZ (0x14014C040)
    void GetRewardItem();

    // IDA: ?GetRewardItemEx@CGocRecode@@QEAAXXZ (0x14014CCD0)
    void GetRewardItemEx();

    // Show cutscene
    // IDA: ?SetShowCutscene@CGocRecode@@QEAAX_N@Z (0x1400682F0)
    void SetShowCutscene(bool bShow) { m_bShowCutscene = bShow; }

    // Infinite tower functions
    // IDA: ?GetInfiniteTowerClearStage@CGocRecode@@QEAAFXZ (0x1403E1C10)
    short GetInfiniteTowerClearStage() const { return m_sInfiniteTowerClearStage; }

    // IDA: ?GetInfiniteTowerClearChapter@CGocRecode@@QEAAFXZ (0x1403E1C30)
    short GetInfiniteTowerClearChapter() const { return m_sInfiniteTowerClearChapter; }

    // IDA: ?GetInfiniteTowerLimitCount@CGocRecode@@QEAAHXZ (0x1400F9080)
    int GetInfiniteTowerLimitCount() const { return m_nInfiniteTowerLimitCount; }

    // IDA: ?GetInfiniteTowerLimitPCBangCount@CGocRecode@@QEAAHXZ (0x1401665E0)
    int GetInfiniteTowerLimitPCBangCount() const { return m_nInfiniteTowerLimitPCBangCount; }

    // IDA: ?SetInfiniteTowerInfo@CGocRecode@@QEAAXFF_JFH_N@Z (0x14014D9F0)
    void SetInfiniteTowerInfo(short sClearChapter, short sClearStage, __int64 nLimitTime, int nLimitCount, short sLimitPCBangCount, bool bEnter);

    // IDA: ?InitInfiniteTowerInfo@CGocRecode@@QEAAX_J@Z (0x14014DBC0)
    void InitInfiniteTowerInfo(__int64 nTime);

    // IDA: ?SetInfiniteTowerLimitTime@CGocRecode@@QEAAX_N0@Z (0x14014DF10)
    void SetInfiniteTowerLimitTime(bool bReset, bool bSend);

    // IDA: ?SetInfiniteTowerLimitCount@CGocRecode@@QEAAXH_N@Z (0x14014E0C0)
    void SetInfiniteTowerLimitCount(int nCount, bool bSend);

    // IDA: ?RewardInfinteTower@CGocRecode@@QEAAXHHK@Z (0x14014E1E0)
    void RewardInfinteTower(int nMazeID, int nChapter, unsigned long dwPlayTime);

    // IDA: ?SendInfiniteTowerInfo@CGocRecode@@QEAAXXZ (0x14014EB30)
    void SendInfiniteTowerInfo();

    // Maze state functions
    // IDA: ?IsClearMazeOnce@CGocRecode@@QEAA_NH@Z (0x140149190)
    bool IsClearMazeOnce(int nMazeID);

    // IDA: ?IsClearMaze@CGocRecode@@QEAA_NH@Z (0x140149200)
    bool IsClearMaze(int nMazeID);

    // IDA: ?UpdateClearInfo@CGocRecode@@QEAAXK@Z (0x140149320)
    void UpdateClearInfo(unsigned int dwPlayTime);

    // IDA: ?UpdateClearInfo_cheat@CGocRecode@@QEAAXH@Z (0x140149850)
    void UpdateClearInfo_cheat(int nMazeID);

    // IDA: ?SendDBLogClearMaze@CGocRecode@@QEAAXHH@Z (0x14015AD70)
    void SendDBLogClearMaze(int nMazeID, int nClearCount);

    // BP/DS Point functions
    // IDA: ?AddBPCombo@CGocRecode@@QEAAXH@Z (0x140148D70)
    void AddBPCombo(int nBP);

    // IDA: ?AddDSPoint@CGocRecode@@QEAAXH@Z (0x140148FE0)
    void AddDSPoint(int nDSPoint);

    // IDA: ?DBUpdatePoint@CGocRecode@@QEAAXXZ (0x140149100)
    void DBUpdatePoint();

    // District state functions
    // IDA: ?ReqWorldState@CGocRecode@@QEAAXXZ (0x140148140)
    void ReqWorldState();

    // IDA: ?LoadDistrictState@CGocRecode@@QEAAXAEAUST_DISTRICT_STATE_LIST@@@Z (0x140148380)
    void LoadDistrictState(ST_DISTRICT_STATE_LIST& stStateList);

    // IDA: ?LoadMazeState@CGocRecode@@QEAAXAEAUST_MAZE_STATE_LIST@@@Z (0x1401484D0)
    void LoadMazeState(ST_MAZE_STATE_LIST& stStateList);

    // IDA: ?SendDistrictState@CGocRecode@@QEAAXXZ (0x140148840)
    void SendDistrictState();

    // IDA: ?SendMazeState@CGocRecode@@QEAAXXZ (0x1401489F0)
    void SendMazeState();

    // IDA: ?AddDistrictState@CGocRecode@@QEAAXG_N@Z (0x14014F6C0)
    void AddDistrictState(unsigned short wDistrictID, bool bSend);

    // Share point functions
    // IDA: ?RewardSharePoint@CGocRecode@@QEAAXHH@Z (0x140149E30)
    void RewardSharePoint(int nSharePoint, int nType);

    // IDA: ?ApplyRewardItemForSharePoint@CGocRecode@@QEAA_NHHAEAUST_CREATE_ITEM@@@Z (0x14014A910)
    bool ApplyRewardItemForSharePoint(int nSharePoint, int nType, ST_CREATE_ITEM& stItem);

    // IDA: ?FullSharePoint@CGocRecode@@QEAAXH@Z (0x14014FBB0)
    void FullSharePoint(int nSharePoint);

    // IDA: ?UpdateSharePoint@CGocRecode@@QEAAXXZ (0x14014FDE0)
    void UpdateSharePoint();

    // IDA: ?UpdateSharePointByForce@CGocRecode@@QEAAXHH@Z (0x140150570)
    void UpdateSharePointByForce(int nSharePoint, int nType);

    // IDA: ?ResetSharePoint@CGocRecode@@QEAAXXZ (0x150CA0)
    void ResetSharePoint();

    // Killed user functions
    // IDA: ?SetKilledUserInfo@CGocRecode@@QEAAXAEAUPS_KILLED_USER_INFOS@@@Z (0x14014ECC0)
    void SetKilledUserInfo(PS_KILLED_USER_INFOS& stInfo);

    // IDA: ?AddKilledUser@CGocRecode@@QEAAXPEAVCUser@@@Z (0x14014ED80)
    void AddKilledUser(CUser* pUser);

    // IDA: ?ClearKilledUser@CGocRecode@@QEAAXXZ (0x14014F520)
    void ClearKilledUser();

    // Enter maze limit count functions
    // IDA: ?LoadEnterMazeLimitCount@CGocRecode@@QEAAXAEAUPS_MAZE_ENTER_LIMIT_COUNT_LIST@@@Z (0x140150EC0)
    void LoadEnterMazeLimitCount();

    // IDA: ?UpdateEnterMazeLimitCount@CGocRecode@@QEAAXGEH_N@Z (0x140151160)
    void UpdateEnterMazeLimitCount(unsigned short wMazeID, unsigned char byType, int nCount, bool bSend);

    // IDA: ?ClearEnterMazeLimitCount@CGocRecode@@QEAAX_J@Z (0x140151800)
    void ClearEnterMazeLimitCount(__int64 nTime);

    // IDA: ?GetEnterMazeLimitCount@CGocRecode@@QEAAGG@Z (0x140151A10)
    unsigned short GetEnterMazeLimitCount();

    // IDA: ?GetEnterMazeLimitPCBangCount@CGocRecode@@QEAAGG@Z (0x140151BB0)
    unsigned short GetEnterMazeLimitPCBangCount();

    // IDA: ?SendEnterMazeLimitCount@CGocRecode@@QEAAXXZ (0x140153CB0)
    void SendEnterMazeLimitCount();

    // Group limit count functions
    // IDA: ?LoadEnterGroupLimitCount@CGocRecode@@QEAAXAEAUPS_MAZE_ENTER_LIMIT_COUNT_GROUP_LIST@@@Z (0x140151D50)
    void LoadEnterGroupLimitCount();

    // IDA: ?UpdateEnterGroupLimitCount@CGocRecode@@QEAAXGE_N0@Z (0x1401520A0)
    void UpdateEnterGroupLimitCount(unsigned short wGroupID, unsigned char byType, bool bAccount, bool bSend);

    // IDA: ?ClearEnterGroupLimitCount@CGocRecode@@QEAAX_JE@Z (0x140152C60)
    void ClearEnterGroupLimitCount(__int64 nTime, unsigned char byType);

    // IDA: ?ResetEnterMazeLimiteCount@CGocRecode@@QEAAXH@Z (0x140153020)
    void ResetEnterMazeLimiteCount(int nType);

    // IDA: ?DecreaseEnterCasualMazeLimiteCount@CGocRecode@@QEAAXXZ (0x140153920)
    void DecreaseEnterCasualMazeLimiteCount();

    // IDA: ?SetEnterGroupLimitCountTime_Account@CGocRecode@@QEAAX_J@Z (0x140166050)
    void SetEnterGroupLimitCountTime_Account(__int64 nTime);

    // IDA: ?SetEnterGroupLimitCountTime_Character@CGocRecode@@QEAAX_J@Z (0x140166070)
    void SetEnterGroupLimitCountTime_Character(__int64 nTime);

    // Ranking functions
    // IDA: ?RankingDataUpdate@CGocRecode@@QEAAXKKH@Z (0x140154150)
    void RankingDataUpdate(unsigned long dwData1, unsigned long dwData2, int nData3);

    // IDA: ?CanRecvRankingReward@CGocRecode@@QEAAHGAEAUST_USER_LAST_RANKING_INFO@@@Z (0x1401553D0)
    bool CanRecvRankingReward(unsigned short wType, void* pInfo);

    // IDA: ?SetRankingMyInfo@CGocRecode@@QEAAXG_NAEAUST_USER_RANKING_INFO@@_K@Z (0x1401554B0)
    void SetRankingMyInfo(unsigned short wType, bool bFlag, void* pInfo, unsigned long long dwData);

    // IDA: ?ReqRankingList@CGocRecode@@QEAA_NAEAUPS_RANKING_LIST_REQ@@@Z (0x140155BC0)
    bool ReqRankingList(void* pReq);

    // IDA: ?ResRankingMyInfo@CGocRecode@@QEAAXAEAUPS_DB_MY_RANKING_INFO_RES@@@Z (0x140156540)
    void ResRankingMyInfo(void* pRes);

    // IDA: ?ReqRankingReward@CGocRecode@@QEAAXAEAUPS_RANKING_REWARD_REQ@@@Z (0x1401569D0)
    void ReqRankingReward(void* pReq);

    // IDA: ?ResRankingReward@CGocRecode@@QEAAXAEAUPS_DB_RANKING_REWARD@@@Z (0x140158410)
    void ResRankingReward(void* pRes);

    // IDA: ?Ranking_Cheat@CGocRecode@@QEAA_NHHH@Z (0x14015BB20)
    bool Ranking_Cheat(int nType, int nRank, int nScore);

    // Over-indulgence functions
    // IDA: ?SetOverIndulgence@CGocRecode@@QEAAX_J000@Z (0x140158A90)
    void SetOverIndulgence(__int64 nConnectTerm, __int64 nDisconnectTerm, __int64 nAlertTick, __int64 nPrevTick);

    // IDA: ?SaveOverIndulgence@CGocRecode@@QEAAXXZ (0x1401590B0)
    void SaveOverIndulgence();

    // IDA: ?UpdateOverIndulgence@CGocRecode@@QEAAXXZ (0x140159400)
    void UpdateOverIndulgence();

    // IDA: ?CheckOverIndulgenceState@CGocRecode@@QEAAHXZ (0x140159840)
    int CheckOverIndulgenceState();

    // IDA: ?GetIndulgenceDropRate@CGocRecode@@QEAAMXZ (0x140159890)
    float GetIndulgenceDropRate();

    // IDA: ?SetIndulgenceByForce@CGocRecode@@QEAAXHH@Z (0x1401598F0)
    void SetIndulgenceByForce(int nState, int nType);

    // IDA: ?SendDBLoadIndulgence@CGocRecode@@QEAAX_N@Z (0x14015A210)
    void SendDBLoadIndulgence(bool bSend);

    // IDA: ?ShowIndulgenceInfo@CGocRecode@@QEAAXXZ (0x14015A370)
    void ShowIndulgenceInfo();

    // Special reward functions
    // IDA: ?GetSpecialExp@CGocRecode@@QEAAHM@Z (0x14014FA40)
    int GetSpecialExp(int nExp);

    // IDA: ?GetSpecialGold@CGocRecode@@QEAAHH@Z (0x14014FB00)
    int GetSpecialGold(int nGold);

    // IDA: ?SendLeagueWealth@CGocRecode@@QEAAXFH@Z (0x1401588F0)
    void SendLeagueWealth(float fValue, int nMoney);

    // Tutorial
    // IDA: ?SetClearTurtorial@CGocRecode@@QEAAX_N0@Z (0x14015A4C0)
    void SetClearTurtorial(bool bClear, bool bSend);

    // IDA: ?GetDailyBaseMazeID@CGocRecode@@QEAA_NGG_NAEAGAEAEAEAH@Z (0x14015A700)
    bool GetDailyBaseMazeID(unsigned short wMazeID, unsigned short wGroupID, bool bFlag,
                            unsigned short& wOut1, unsigned char& byOut2, unsigned char& byOut3, int& nOut);

    // Tool reward functions
    // IDA: ?GetToolClearRewardInfo@CGocRecode@@QEAAXAEAUPS_RES_TOOL_REWARD_INFO@@@Z (0x14015ACE0)
    void GetToolClearRewardInfo(void* pInfo);

    // IDA: ?ClearToolInfo@CGocRecode@@QEAAXXZ (0x14015AD40)
    void ClearToolInfo();

    // IDA: ?UseInfiniteTowerInitItem@CGocRecode@@QEAAXXZ (0x14014DD80)
    void UseInfiniteTowerInitItem();

    // IDA: ?MonsterKillScoreReward@CGocRecode@@QEAAXHE_K@Z (0x14015B6D0)
    void MonsterKillScoreReward(int nType, unsigned char byData, unsigned long long dwData);

    // Helper functions
    int GetClearTime() const { return m_nMazeClearTime; }
    int GetMazeLevel() const { return m_nMazeLevel; }

    int GetRecode(int nIndex) const {
        if (nIndex >= 0 && nIndex < 10)
            return m_nMazeRecode[nIndex];
        return 0;
    }

    __int64 GetEnterGroupLimitCountTime_Character() const { return m_nInitEnterGroupLimitCountTime_Character; }
    __int64 GetEnterGroupLimitCountTime_Account() const { return m_nInitEnterGroupLimitCountTime_Account; }

    // IDA: ?IsClearTurtorial@CGocRecode@@QEAA_NXZ (0x14059E3B0)
    bool IsClearTurtorial() const { return m_bClearTurtorial; }

    // IDA: ?IsLoadDB_All@CGocRecode@@QEAA_NXZ (0x1403E1830)
    bool IsLoadDB_All() const { return m_bLoadWorldSharePoint && m_bLoadMazeEnterLimitCount; }

    // IDA: ?SetReviveState@CGocRecode@@QEAAX_N@Z (0x14070AAE0)
    void SetReviveState(bool bUse) { m_bUseRevive = bUse; }

    // IDA: ?SetToolInfo@CGocRecode@@QEAAXHH@Z (0x14060DAE0)
    void SetToolInfo(int nMazeID, int nRank) { m_nMazeID = nMazeID; m_nRank = nRank; }

    // IDA: ?GetInfiniteTowerLimitTime@CGocRecode@@QEAA_JXZ (0x140406DF0)
    __int64 GetInfiniteTowerLimitTime() const { return m_nInfiniteTowerLimitTime; }

protected:
    // Maze clear info map
    std::map<int, ST_MAZE_CLEAR_INFO> m_mapMazeClearInfo;

    // District state map
    std::map<int, ST_DISTRICT_STATE> m_mapDistrictState;

    // Maze episode state
    std::map<int, ST_RANDOM_BUFF> m_mapMazeEpisodeState;

    // Maze group state
    std::map<int, ST_RANDOM_BUFF> m_mapMazeGroupState;

    // Killed user map (UCID -> kill count)
    std::map<int, int> m_mapKilledUser;

    // Ranking maps - using raw storage to avoid incomplete types
    // TODO: Define ST_USER_RANKING_INFO and ST_USER_LAST_RANKING_INFO structures
    // std::map<unsigned short, ST_USER_RANKING_INFO> m_mapMyRanking;
    // std::map<unsigned short, ST_USER_LAST_RANKING_INFO> m_mapMyLastRanking;
    std::map<unsigned short, std::vector<std::uint8_t>> m_mapMyRanking;
    std::map<unsigned short, std::vector<std::uint8_t>> m_mapMyLastRanking;

    // Enter maze limit count map
    std::map<int, PS_UPDATE_MAZE_ENTER_LIMIT_COUNT> m_mapEnterMazeLimitCount;

    // Enter group limit count maps
    std::map<unsigned short, PS_MAZE_ENTER_LIMIT_COUNT_GROUP> m_mapEnterGroupLimitCount_Account;
    std::map<unsigned short, PS_MAZE_ENTER_LIMIT_COUNT_GROUP> m_mapEnterGroupLimitCount_Character;

    // Base reward item (using raw storage to avoid incomplete type)
    // IDA shows this is 8 bytes: ST_CREATE_ITEM
    std::uint8_t m_stBaseRewardItem[8] = {};

    // Cash reward items (pointers - allocated externally)
    ST_MAZE_CASH_REWARD_INFO* m_stCashRewardItem = nullptr;
    ST_MAZE_CASH_REWARD_INFO* m_stCashRewardItemEx = nullptr;
    std::uint8_t m_stCashRewardItemListEX[56] = {}; // 7 * 8 bytes

    // Hidden event reward
    std::uint8_t m_stHiddenEventRewardItem[8] = {};
    unsigned char m_byHiddenEventSendPost = 0;

    // Tool reward info - placeholder storage
    std::uint8_t m_stToolRewardInfo[256] = {};

    // Time tracking
    __int64 m_nInitKilledUserTime = 0;
    __int64 m_nInitEnterMazeLimitCountTime = 0;
    __int64 m_nInitEnterGroupLimitCountTime_Account = 0;
    __int64 m_nInitEnterGroupLimitCountTime_Character = 0;

    // Flags
    bool m_bLoadWorldSharePoint = false;
    bool m_bLoadMazeEnterLimitCount = false;
    bool m_bShowCutscene = false;
    bool m_bEnterInfinite = false;
    bool m_bClearTurtorial = false;
    bool m_bUseRevive = false;

    // Play point and ranking
    int m_nPlayPointOrder = 1;
    float m_fPlayPoint = 0.0f;
    int m_nRank = 0;
    float m_fExpValue = 1.0f;
    float m_fMoneyValue = 1.0f;
    float m_fPartyExpValue = 0.0f;
    float m_fPartyMoneyValue = 0.0f;

    // Clear point
    int m_nClearPoint = 0;

    // Final rewards
    float m_fFinalRewardExp = 0.0f;
    int m_nFinalRewardMoney = 0;

    // Play time
    unsigned int m_dwPlayTime = 0;

    // Unity bonuses
    int m_nComboBPByUnity = 0;
    int m_nDSPointByUnity = 0;

    // Maze info
    int m_nMazeID = 0;
    int m_nMazeClearTime = 0;
    int m_nMazeEpisodeNo = 0;
    int m_nMazeDifficulty = 0;
    int m_nMazeLevel = 0;
    int m_nMazeExp = 0;
    int m_nMazeMoney = 0;
    int m_nMazeGroup = 0;

    // Maze recode (score tracking) - 10 elements
    int m_nMazeRecode[10] = {};

    // Party member count
    int m_nPartyMemberCount = 0;

    // Infinite tower
    short m_sInfiniteTowerClearChapter = 0;
    short m_sInfiniteTowerClearStage = 0;
    int m_nInfiniteTowerLimitTime = 0;
    int m_nInfiniteTowerLimitCount = 0;
    int m_nInfiniteTowerLimitPCBangCount = 0;

    // Reward infinite tower - placeholder storage
    std::uint8_t m_stRewardInfiniteTower[128] = {};

    // Wait district state
    unsigned short m_wWaitDistrictStateID = 0;

    // Over-indulgence
    int m_nIndulgenceConnectTermTick = 0;
    int m_nIndulgenceDisconnectTermTick = 0;
    int m_nIndulgenceAlertTick = 0;
    int m_nIndulgencePrevTick = 0;
    int m_nIndulgenceState = 0;

    // Ranking list tick
    unsigned long long m_dw64RankingListTick[4] = {};
};
