#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_MyRoomStructs.h"
#include <map>
#include <set>
#include <vector>
#include <memory>

// Forward declarations
class CUser;
class XMyRoom;
class XGameServer;
struct TB_MYROOM_INFO;

/**
 * @brief CGocMyroom - Game Object Component for personal room/housing system
 *
 * Handles player room customization, furniture, pollen cultivation,
 * favorites, recommendations, and ranking.
 *
 * IDA address: 0x1400FAC40 (constructor)
 */
class CGocMyroom : public GOComponent {
public:
    // IDA: ??0CGocMyroom@@QEAA@XZ (0x1400FAC40)
    CGocMyroom();

    // IDA: ??1CGocMyroom@@UEAA@XZ (0x1400FADE0)
    virtual ~CGocMyroom();

    // GOComponent interface
    virtual bool Initialize() override { return true; }
    virtual void Shutdown() override {}
    virtual void Update(float fDeltaTime) override;

    // Static family ID
    // IDA: ?GetFamilyID@CGocMyroom@@SAHXZ (0x1401F3620)
    static int GetFamilyID() { return 23; }

    // Initialization
    // IDA: ?Init@CGocMyroom@@QEAAXXZ (0x1400FAE90)
    void Init();

    // IDA: ?Clear@CGocMyroom@@QEAAXXZ (0x1400FAEB0)
    void Clear();

    // Update
    // IDA: ?OnUpdate@CGocMyroom@@QEAAXM@Z (0x1400FAF50)
    void OnUpdate(float fDeltaTime);

    // IDA: ?UpdateData@CGocMyroom@@QEAAXXZ (0x1400FB0F0)
    void UpdateData();

    // MyRoom info
    // IDA: ?SetMyRoomInfo@CGocMyroom@@QEAAXUST_MYROOM_OWNER_INFO@@@Z (0x1400FC370)
    void SetMyRoomInfo(const ST_MYROOM_OWNER_INFO& stMyRoomInfo);

    // IDA: ?GetMyRoomInfo@CGocMyroom@@QEAAXAEAUST_MYROOM_OWNER_INFO@@@Z (0x1400FC410)
    void GetMyRoomInfo(ST_MYROOM_OWNER_INFO& stMyRoomInfo) const;

    // IDA: ?IsMyRoomCreate@CGocMyroom@@QEAA_NXZ (0x1400FC460)
    bool IsMyRoomCreate() const;

    // IDA: ?SetMyRoomSetup@CGocMyroom@@QEAAXUPS_MYROOM_SETUP@@@Z (0x1400FC490)
    void SetMyRoomSetup(const PS_MYROOM_SETUP& stMyRoomSetup);

    // Pollen operations
    // IDA: ?LoadPollenInfo@CGocMyroom@@QEAAXUPS_MYROOM_POLLEN_LIST@@@Z (0x1400FB530)
    void LoadPollenInfo(PS_MYROOM_POLLEN_LIST& psPollenList);

    // IDA: ?SendMyRoomLoad@CGocMyroom@@QEAAXXZ (0x1400FB780)
    void SendMyRoomLoad();

    // IDA: ?PollenAdd@CGocMyroom@@QEAAXH@Z (0x1400FB920)
    void PollenAdd(int nPollenIndex);

    // IDA: ?PollenCultivation@CGocMyroom@@QEAAXAEAUPS_DB_MYROOM_POLLEN_CULTIVATION@@@Z (0x1400FBA60)
    void PollenCultivation(PS_DB_MYROOM_POLLEN_CULTIVATION& psCultivation);

    // IDA: ?PollenClear@CGocMyroom@@QEAAXH@Z (0x1400FBBF0)
    void PollenClear(int nPollenIndex);

    // IDA: ?PollenItemUse@CGocMyroom@@QEAAXUPS_MYROOM_POLLEN_INFO@@@Z (0x1400FBD50)
    void PollenItemUse(const PS_MYROOM_POLLEN_INFO& psInfo);

    // IDA: ?PollenAddHelpUser@CGocMyroom@@QEAAXHUPS_MYROOM_POLLEN_HELP_USER@@_J@Z (0x1400FBED0)
    void PollenAddHelpUser(int nPollenIndex, const PS_MYROOM_POLLEN_HELP_USER& stHelpUser, std::int64_t biHelpDate);

    // IDA: ?SendDBMyRoomIndex@CGocMyroom@@QEAAXXZ (0x1400FC0C0)
    void SendDBMyRoomIndex();

    // IDA: ?SendPollenUpdate@CGocMyroom@@QEAAXUPS_MYROOM_POLLEN_INFO@@@Z (0x1400FC1D0)
    void SendPollenUpdate(const PS_MYROOM_POLLEN_INFO& psInfo);

    // IDA: ?SetPollenLockCount@CGocMyroom@@QEAA_NHE@Z (0x1400FC2B0)
    bool SetPollenLockCount(int nPollenIndex, int nLockCount);

    // Recommend operations
    // IDA: ?LoadMyroomRecommend@CGocMyroom@@QEAAXUPS_MYROOM_RECOMMEND_LIST@@@Z (0x1400FC520)
    void LoadMyroomRecommend(PS_MYROOM_RECOMMEND_LIST& psRecommendList);

    // IDA: ?FindRecommend@CGocMyroom@@QEAA_NK@Z (0x1400FC6F0)
    bool FindRecommend(std::uint32_t dwOwnerUAID) const;

    // IDA: ?AddRecommend@CGocMyroom@@QEAAXK@Z (0x1400FCDB0)
    void AddRecommend(std::uint32_t dwOwnerUAID);

    // IDA: ?Recommend@CGocMyroom@@QEAAXKK@Z (0x1400FC8F0)
    void Recommend(std::uint32_t dwUAID, std::uint32_t dwOwnerUAID);

    // IDA: ?SendRecommend@CGocMyroom@@QEAAXUPS_RES_MYROOM_RECOMMEND@@@Z (0x1400FCF70)
    void SendRecommend(const PS_RES_MYROOM_RECOMMEND& psResRecommend);

    // Favorite operations
    // IDA: ?LoadMyroomFavorite@CGocMyroom@@QEAAXUPS_MYROOM_FAVORITE_LIST@@@Z (0x1400FC5D0)
    void LoadMyroomFavorite(PS_MYROOM_FAVORITE_LIST& psFavoriteList);

    // IDA: ?SetFavoriteList@CGocMyroom@@QEAAXUPS_MYROOM_FAVORITE_LIST@@@Z (0x1400FC750)
    void SetFavoriteList(PS_MYROOM_FAVORITE_LIST& psFavoriteList);

    // IDA: ?SendFavoriteList@CGocMyroom@@QEAAXUPS_MYROOM_FAVORITE_LIST@@@Z (0x1400FC810)
    void SendFavoriteList(PS_MYROOM_FAVORITE_LIST& psFavoriteList);

    // IDA: ?AddFavorite@CGocMyroom@@QEAAXUST_MYROOM_FAVORITE_INFO@@@Z (0x1400FCDE0)
    void AddFavorite(const ST_MYROOM_FAVORITE_INFO& stFavoriteInfo);

    // IDA: ?DeleteFavorite@CGocMyroom@@QEAAXUST_MYROOM_FAVORITE_INFO@@@Z (0x1400FCE70)
    void DeleteFavorite(const ST_MYROOM_FAVORITE_INFO& stFavoriteInfo);

    // IDA: ?FindFavorite@CGocMyroom@@QEAA_NK@Z (0x1400FCF00)
    bool FindFavorite(std::uint32_t dwOwnerUAID) const;

    // IDA: ?SendFavoriteInfo@CGocMyroom@@QEAAXXZ (0x1400FD040)
    void SendFavoriteInfo();

    // IDA: ?CheckFavorite@CGocMyroom@@QEAAHUPS_REQ_MYROOM_FAVORITE@@K@Z (0x1400FDF30)
    int CheckFavorite(const PS_REQ_MYROOM_FAVORITE& psReqFavorite, std::uint32_t dwOwnerUAID);

    // Board operations
    // IDA: ?LoadBoardList@CGocMyroom@@QEAAXUPS_REQ_MYROOM_BOARD_INFO@@@Z (0x1400FD160)
    void LoadBoardList(const PS_REQ_MYROOM_BOARD_INFO& psReqBoardInfo);

    // IDA: ?SendBoardList@CGocMyroom@@QEAAXUPS_MYROOM_BOARD_LIST@@@Z (0x1400FD240)
    void SendBoardList(PS_MYROOM_BOARD_LIST& psBoardList);

    // IDA: ?GetCrop@CGocMyroom@@QEAAXPEAUPS_MYROOM_BOARD_POT@@@Z (0x1400FD360)
    void GetCrop(PS_MYROOM_BOARD_POT* psCropInfo);

    // IDA: ?GetFunitureCount@CGocMyroom@@QEAAXAEAH@Z (0x1400FD450)
    void GetFunitureCount(int& nCount) const;

    // IDA: ?WriteBoard@CGocMyroom@@QEAAXUPS_REQ_MYROOM_BOARD_WRITE@@@Z (0x1400FD480)
    void WriteBoard(const PS_REQ_MYROOM_BOARD_WRITE& psReqBoardWrite);

    // IDA: ?SendBoardWrite@CGocMyroom@@QEAAXUPS_RES_MYROOM_BOARD_WRITE@@@Z (0x1400FDAB0)
    void SendBoardWrite(const PS_RES_MYROOM_BOARD_WRITE& psResBoardWrite);

    // IDA: ?SetBoard@CGocMyroom@@QEAAXUST_MYROOM_BOARD_INFO@@@Z (0x1400FDB80)
    void SetBoard(const ST_MYROOM_BOARD_INFO& stBoardInfo);

    // IDA: ?SendWriteBoardInfo@CGocMyroom@@QEAAXXZ (0x1400FE040)
    void SendWriteBoardInfo();

    // Rank operations
    // IDA: ?LoadMyroomRankInfo@CGocMyroom@@QEAAXUPS_MYROOM_RANK_LIST@@0UPS_MYROOM_RANK_INFO@@1@Z (0x1400FDBE0)
    void LoadMyroomRankInfo(PS_MYROOM_RANK_LIST& psCurrentRankList, PS_MYROOM_RANK_LIST& psPastRankList,
                            PS_MYROOM_RANK_INFO& psMyCurrentRankInfo, PS_MYROOM_RANK_INFO& psMyPastRankInfo);

    // IDA: ?SendMyroomRankInfo@CGocMyroom@@QEAAXXZ (0x1400FDE00)
    void SendMyroomRankInfo();

    // IDA: ?GetRankRewardID@CGocMyroom@@QEAAHXZ (0x1400FE290)
    int GetRankRewardID();

    // IDA: ?CheckRank@CGocMyroom@@QEAAHK@Z (0x1400FE380)
    int CheckRank(std::uint32_t dwUAID);

    // IDA: ?RankReward@CGocMyroom@@QEAAXUPS_DB_MYROOM_RANK_REWARD@@@Z (0x1400FE7F0)
    void RankReward(PS_DB_MYROOM_RANK_REWARD& psRankReward);

    // IDA: ?SendRankRewardPost@CGocMyroom@@QEAAXKUST_CREATE_ITEMS@@@Z (0x1400FE980)
    void SendRankRewardPost(std::uint32_t dwUAID, const ST_CREATE_ITEMS& stCreateItems);

    // Furniture operations
    // IDA: ?LoadMyroomFunitureList@CGocMyroom@@QEAAXUPS_MYROOM_FUNITURE_LIST@@@Z (0x1400FE120)
    void LoadMyroomFunitureList(PS_MYROOM_FUNITURE_LIST& psFunitureList);

    // IDA: ?AddFuniture@CGocMyroom@@QEAAXH@Z (0x1400FE160)
    void AddFuniture(int nFunitureID);

    // IDA: ?RemoveFuniture@CGocMyroom@@QEAAXH@Z (0x1400FE1F0)
    void RemoveFuniture(int nFunitureID);

    // DB Log
    // IDA: ?SendDBLog@CGocMyroom@@QEAAXFKH@Z (0x1400FEBD0)
    void SendDBLog(int nLogType, std::uint32_t dwUAID, int nParam);

    // IDA: ?GetMyroomBoardInfo@CGocMyroom@@QEAAXAEAUST_MYROOM_OWNER_INFO@@@Z (0x1400FED50)
    void GetMyroomBoardInfo(ST_MYROOM_OWNER_INFO& stOwnerInfo) const;

    // IDA: ?SetCommunityInfo@CGocMyroom@@QEAAXUPS_MYROOM_COMMUNITY_INFO@@@Z (0x1400FEDB0)
    void SetCommunityInfo(const PS_MYROOM_COMMUNITY_INFO& psCommunityInfo);

    // Getters/Setters
    // IDA: ?GetLoadMyroom@CGocMyroom@@QEAA_NXZ (0x1402AC5B0)
    bool GetLoadMyroom() const { return m_bLoadMyroom; }

    // IDA: ?SetLoadMyroom@CGocMyroom@@QEAAX_N@Z (0x14052A700)
    void SetLoadMyroom(bool bLoad) { m_bLoadMyroom = bLoad; }

    // IDA: ?IsReqFavoriteList@CGocMyroom@@QEAA_NXZ (0x14052A6C0)
    bool IsReqFavoriteList() const { return m_bFavoriteListInfo; }

    // IDA: ?SetReqFavoriteList@CGocMyroom@@QEAAX_N@Z (0x14052A6E0)
    void SetReqFavoriteList(bool bReq) { m_bFavoriteListInfo = bReq; }

protected:
    // Member variables (ordered by IDA constructor initialization sequence)

    // MyRoom owner info
    ST_MYROOM_OWNER_INFO m_stMyRoomInfo;

    // Pollen info map: key = nPollenIndex
    std::map<int, PS_MYROOM_POLLEN_INFO> m_mpPollenInfo;

    // Recommend list (set of OwnerUAID that have been recommended today)
    std::set<std::uint32_t> m_setRecommendList;

    // Favorite list map: key = OwnerUAID
    std::map<std::uint32_t, ST_MYROOM_FAVORITE_INFO> m_mapFavoriteList;

    // Board info
    ST_MYROOM_BOARD_INFO m_stBoardInfo;

    // Rank lists
    PS_MYROOM_RANK_LIST m_psCurrentRankList;
    PS_MYROOM_RANK_LIST m_psPastRankList;

    // My rank info
    PS_MYROOM_RANK_INFO m_psMyCurrentRankInfo;
    PS_MYROOM_RANK_INFO m_psMyPastRankInfo;

    // Furniture list (vector of furniture IDs)
    std::vector<std::int32_t> m_psFunitureList;

    // Init date for daily update check (ATL::CTime)
    std::int64_t m_tInitDate = 0;

    // Flags
    bool m_bFavoriteListInfo = false;  // Whether favorite list has been requested
    bool m_bLoadMyroom = false;        // Whether myroom data has been loaded
};
