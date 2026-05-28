#include "GocMyRoom.h"
#include "XCore/XServer/XSendPacket.h"
#include "XCore/XServer/XSendDBPacket.h"
#include <cstring>

// Forward declarations for external dependencies
class CUser;
class XMyRoom;
class XGameServer;

// Helper function declarations (to be implemented when dependencies are available)
extern XGameServer* GetGameServerInstance();
extern void SendPacketToGameDB(XSendDBPacket* pPacket);

//=============================================================================
// Constructor / Destructor
//=============================================================================

// IDA: ??0CGocMyroom@@QEAA@XZ (0x1400FAC40)
CGocMyroom::CGocMyroom()
    : GOComponent()
    , m_stMyRoomInfo{}
    , m_mpPollenInfo()
    , m_setRecommendList()
    , m_mapFavoriteList()
    , m_stBoardInfo{}
    , m_psCurrentRankList()
    , m_psPastRankList()
    , m_psMyCurrentRankInfo{}
    , m_psMyPastRankInfo{}
    , m_psFunitureList()
    , m_tInitDate(0)
    , m_bFavoriteListInfo(false)
    , m_bLoadMyroom(false)
{
    Clear();
}

// IDA: ??1CGocMyroom@@UEAA@XZ (0x1400FADE0)
CGocMyroom::~CGocMyroom()
{
    Clear();
}

//=============================================================================
// Initialization
//=============================================================================

// IDA: ?Init@CGocMyroom@@QEAAXXZ (0x1400FAE90)
void CGocMyroom::Init()
{
    Clear();
}

// IDA: ?Clear@CGocMyroom@@QEAAXXZ (0x1400FAEB0)
void CGocMyroom::Clear()
{
    // Clear MyRoom info
    std::memset(&m_stMyRoomInfo, 0, sizeof(m_stMyRoomInfo));

    // Set init date to epoch (0)
    m_tInitDate = 0;

    // Clear all containers
    m_mpPollenInfo.clear();
    m_setRecommendList.clear();
    m_mapFavoriteList.clear();
    m_psFunitureList.clear();

    // Reset flags
    m_bFavoriteListInfo = false;
    m_bLoadMyroom = false;
}

//=============================================================================
// Update
//=============================================================================

// IDA: ?OnUpdate@CGocMyroom@@QEAAXM@Z (0x1400FAF50)
void CGocMyroom::OnUpdate(float fDeltaTime)
{
    // TODO: 汇编还原 - OnUpdate logic
    // Check for daily update at 9 AM
    // This requires CUser, ATL::CTime, and other dependencies
    (void)fDeltaTime;
}

// IDA: ?UpdateData@CGocMyroom@@QEAAXXZ (0x1400FB0F0)
void CGocMyroom::UpdateData()
{
    // TODO: 汇编还原 - Send DB request to update myroom data
    // Requires CUser, XGameServer, XSendDBPacket dependencies
}

//=============================================================================
// MyRoom Info
//=============================================================================

// IDA: ?SetMyRoomInfo@CGocMyroom@@QEAAXUST_MYROOM_OWNER_INFO@@@Z (0x1400FC370)
void CGocMyroom::SetMyRoomInfo(const ST_MYROOM_OWNER_INFO& stMyRoomInfo)
{
    // Copy the structure
    m_stMyRoomInfo = stMyRoomInfo;

    // Lookup map ID from table
    // TODO: 需要 XResourceMgr::GetTB_MYROOM_INFO
    // std::uint32_t dwIndex = m_stMyRoomInfo.shMapIndex;
    // TB_MYROOM_INFO* pMyRoomInfo = XResourceMgr::GetTB_MYROOM_INFO(dwIndex);
    // if (pMyRoomInfo) {
    //     m_stMyRoomInfo.dwMapID = pMyRoomInfo->My_Room_Field_ID;
    // }
}

// IDA: ?GetMyRoomInfo@CGocMyroom@@QEAAXAEAUST_MYROOM_OWNER_INFO@@@Z (0x1400FC410)
void CGocMyroom::GetMyRoomInfo(ST_MYROOM_OWNER_INFO& stMyRoomInfo) const
{
    stMyRoomInfo = m_stMyRoomInfo;
}

// IDA: ?IsMyRoomCreate@CGocMyroom@@QEAA_NXZ (0x1400FC460)
bool CGocMyroom::IsMyRoomCreate() const
{
    return m_stMyRoomInfo.shGridNo != 0 && m_stMyRoomInfo.shMapIndex != 0;
}

// IDA: ?SetMyRoomSetup@CGocMyroom@@QEAAXUPS_MYROOM_SETUP@@@Z (0x1400FC490)
void CGocMyroom::SetMyRoomSetup(const PS_MYROOM_SETUP& stMyRoomSetup)
{
    m_stMyRoomInfo.byRoomOpenLevel = stMyRoomSetup.byOpenLevel;

    // Copy room name (wide string)
    std::wcscpy(m_stMyRoomInfo.szRoomName, stMyRoomSetup.szMyRoomName);
}

//=============================================================================
// Pollen Operations
//=============================================================================

// IDA: ?LoadPollenInfo@CGocMyroom@@QEAAXUPS_MYROOM_POLLEN_LIST@@@Z (0x1400FB530)
void CGocMyroom::LoadPollenInfo(PS_MYROOM_POLLEN_LIST& psPollenList)
{
    m_mpPollenInfo.clear();

    for (const auto& psPollenInfo : psPollenList.vecInfo) {
        m_mpPollenInfo[psPollenInfo.nPollenIndex] = psPollenInfo;
    }
}

// IDA: ?SendMyRoomLoad@CGocMyroom@@QEAAXXZ (0x1400FB780)
void CGocMyroom::SendMyRoomLoad()
{
    // TODO: 汇编还原 - Build and send PS_MYROOM_POLLEN_LIST packet
    // Requires XSendPacket, CGocNetwork::Send
}

// IDA: ?PollenAdd@CGocMyroom@@QEAAXH@Z (0x1400FB920)
void CGocMyroom::PollenAdd(int nPollenIndex)
{
    // Check if already exists
    if (m_mpPollenInfo.find(nPollenIndex) != m_mpPollenInfo.end()) {
        return;
    }

    // Add new pollen info
    PS_MYROOM_POLLEN_INFO psInfo{};
    psInfo.nPollenIndex = nPollenIndex;
    m_mpPollenInfo[nPollenIndex] = psInfo;

    // Send update to client
    SendPollenUpdate(psInfo);
}

// IDA: ?PollenCultivation@CGocMyroom@@QEAAXAEAUPS_DB_MYROOM_POLLEN_CULTIVATION@@@Z (0x1400FBA60)
void CGocMyroom::PollenCultivation(PS_DB_MYROOM_POLLEN_CULTIVATION& psCultivation)
{
    // TODO: 汇编还原 - Pollen cultivation logic
    (void)psCultivation;
}

// IDA: ?PollenClear@CGocMyroom@@QEAAXH@Z (0x1400FBBF0)
void CGocMyroom::PollenClear(int nPollenIndex)
{
    m_mpPollenInfo.erase(nPollenIndex);
}

// IDA: ?PollenItemUse@CGocMyroom@@QEAAXUPS_MYROOM_POLLEN_INFO@@@Z (0x1400FBD50)
void CGocMyroom::PollenItemUse(const PS_MYROOM_POLLEN_INFO& psInfo)
{
    // TODO: 汇编还原 - Pollen item use logic
    (void)psInfo;
}

// IDA: ?PollenAddHelpUser@CGocMyroom@@QEAAXHUPS_MYROOM_POLLEN_HELP_USER@@_J@Z (0x1400FBED0)
void CGocMyroom::PollenAddHelpUser(int nPollenIndex, const PS_MYROOM_POLLEN_HELP_USER& stHelpUser, std::int64_t biHelpDate)
{
    // TODO: 汇编还原 - Add help user to pollen
    (void)nPollenIndex;
    (void)stHelpUser;
    (void)biHelpDate;
}

// IDA: ?SendDBMyRoomIndex@CGocMyroom@@QEAAXXZ (0x1400FC0C0)
void CGocMyroom::SendDBMyRoomIndex()
{
    // TODO: 汇编还原 - Send DB request for myroom index
}

// IDA: ?SendPollenUpdate@CGocMyroom@@QEAAXUPS_MYROOM_POLLEN_INFO@@@Z (0x1400FC1D0)
void CGocMyroom::SendPollenUpdate(const PS_MYROOM_POLLEN_INFO& psInfo)
{
    // TODO: 汇编还原 - Send pollen update packet to client
    (void)psInfo;
}

// IDA: ?SetPollenLockCount@CGocMyroom@@QEAA_NHE@Z (0x1400FC2B0)
bool CGocMyroom::SetPollenLockCount(int nPollenIndex, int nLockCount)
{
    // TODO: 汇编还原 - Set pollen lock count
    (void)nPollenIndex;
    (void)nLockCount;
    return false;
}

//=============================================================================
// Recommend Operations
//=============================================================================

// IDA: ?LoadMyroomRecommend@CGocMyroom@@QEAAXUPS_MYROOM_RECOMMEND_LIST@@@Z (0x1400FC520)
void CGocMyroom::LoadMyroomRecommend(PS_MYROOM_RECOMMEND_LIST& psRecommendList)
{
    m_setRecommendList.clear();

    for (const auto& dwOwnerUAID : psRecommendList.vecUAID) {
        m_setRecommendList.insert(dwOwnerUAID);
    }

    // TODO: 汇编还原 - Set init date based on current time
    // ATL::CTime tCurr;
    // ATL::CTime::GetTickCount(&tCurr);
    // if (ATL::CTime::GetHour(&tCurr) < 9) {
    //     m_tInitDate = tCurr;
    // }
}

// IDA: ?FindRecommend@CGocMyroom@@QEAA_NK@Z (0x1400FC6F0)
bool CGocMyroom::FindRecommend(std::uint32_t dwOwnerUAID) const
{
    return m_setRecommendList.find(dwOwnerUAID) != m_setRecommendList.end();
}

// IDA: ?AddRecommend@CGocMyroom@@QEAAXK@Z (0x1400FCDB0)
void CGocMyroom::AddRecommend(std::uint32_t dwOwnerUAID)
{
    m_setRecommendList.insert(dwOwnerUAID);
}

// IDA: ?Recommend@CGocMyroom@@QEAAXKK@Z (0x1400FC8F0)
void CGocMyroom::Recommend(std::uint32_t dwUAID, std::uint32_t dwOwnerUAID)
{
    // TODO: 汇编还原 - Full recommend logic
    // 1. Check if UAID is the room owner
    // 2. Check if OwnerUAID matches
    // 3. Check if already recommended today
    // 4. Update daily mission
    // 5. Send DB request
    // 6. Send result packet
    (void)dwUAID;
    (void)dwOwnerUAID;
}

// IDA: ?SendRecommend@CGocMyroom@@QEAAXUPS_RES_MYROOM_RECOMMEND@@@Z (0x1400FCF70)
void CGocMyroom::SendRecommend(const PS_RES_MYROOM_RECOMMEND& psResRecommend)
{
    // TODO: 汇编还原 - Send recommend result packet
    (void)psResRecommend;
}

//=============================================================================
// Favorite Operations
//=============================================================================

// IDA: ?LoadMyroomFavorite@CGocMyroom@@QEAAXUPS_MYROOM_FAVORITE_LIST@@@Z (0x1400FC5D0)
void CGocMyroom::LoadMyroomFavorite(PS_MYROOM_FAVORITE_LIST& psFavoriteList)
{
    m_mapFavoriteList.clear();

    for (const auto& stFavoriteInfo : psFavoriteList.vecInfo) {
        m_mapFavoriteList[stFavoriteInfo.dwOwnerUAID] = stFavoriteInfo;
    }
}

// IDA: ?SetFavoriteList@CGocMyroom@@QEAAXUPS_MYROOM_FAVORITE_LIST@@@Z (0x1400FC750)
void CGocMyroom::SetFavoriteList(PS_MYROOM_FAVORITE_LIST& psFavoriteList)
{
    m_mapFavoriteList.clear();

    for (const auto& stFavoriteInfo : psFavoriteList.vecInfo) {
        AddFavorite(stFavoriteInfo);
    }
}

// IDA: ?SendFavoriteList@CGocMyroom@@QEAAXUPS_MYROOM_FAVORITE_LIST@@@Z (0x1400FC810)
void CGocMyroom::SendFavoriteList(PS_MYROOM_FAVORITE_LIST& psFavoriteList)
{
    // TODO: 汇编还原 - Send favorite list packet
    (void)psFavoriteList;
}

// IDA: ?AddFavorite@CGocMyroom@@QEAAXUST_MYROOM_FAVORITE_INFO@@@Z (0x1400FCDE0)
void CGocMyroom::AddFavorite(const ST_MYROOM_FAVORITE_INFO& stFavoriteInfo)
{
    m_mapFavoriteList[stFavoriteInfo.dwOwnerUAID] = stFavoriteInfo;
    SendDBLog(6, stFavoriteInfo.dwOwnerUAID, 0);
}

// IDA: ?DeleteFavorite@CGocMyroom@@QEAAXUST_MYROOM_FAVORITE_INFO@@@Z (0x1400FCE70)
void CGocMyroom::DeleteFavorite(const ST_MYROOM_FAVORITE_INFO& stFavoriteInfo)
{
    m_mapFavoriteList.erase(stFavoriteInfo.dwOwnerUAID);
    SendDBLog(6, stFavoriteInfo.dwOwnerUAID, 0);
}

// IDA: ?FindFavorite@CGocMyroom@@QEAA_NK@Z (0x1400FCF00)
bool CGocMyroom::FindFavorite(std::uint32_t dwOwnerUAID) const
{
    return m_mapFavoriteList.find(dwOwnerUAID) != m_mapFavoriteList.end();
}

// IDA: ?SendFavoriteInfo@CGocMyroom@@QEAAXXZ (0x1400FD040)
void CGocMyroom::SendFavoriteInfo()
{
    // TODO: 汇编还原 - Send favorite info packet
}

// IDA: ?CheckFavorite@CGocMyroom@@QEAAHUPS_REQ_MYROOM_FAVORITE@@K@Z (0x1400FDF30)
int CGocMyroom::CheckFavorite(const PS_REQ_MYROOM_FAVORITE& psReqFavorite, std::uint32_t dwOwnerUAID)
{
    // TODO: 汇编还原 - Check favorite status
    (void)psReqFavorite;
    (void)dwOwnerUAID;
    return 0;
}

//=============================================================================
// Board Operations
//=============================================================================

// IDA: ?LoadBoardList@CGocMyroom@@QEAAXUPS_REQ_MYROOM_BOARD_INFO@@@Z (0x1400FD160)
void CGocMyroom::LoadBoardList(const PS_REQ_MYROOM_BOARD_INFO& psReqBoardInfo)
{
    // TODO: 汇编还原 - Load board list from DB
    (void)psReqBoardInfo;
}

// IDA: ?SendBoardList@CGocMyroom@@QEAAXUPS_MYROOM_BOARD_LIST@@@Z (0x1400FD240)
void CGocMyroom::SendBoardList(PS_MYROOM_BOARD_LIST& psBoardList)
{
    // TODO: 汇编还原 - Send board list packet
    (void)psBoardList;
}

// IDA: ?GetCrop@CGocMyroom@@QEAAXPEAUPS_MYROOM_BOARD_POT@@@Z (0x1400FD360)
void CGocMyroom::GetCrop(PS_MYROOM_BOARD_POT* psCropInfo)
{
    if (!psCropInfo) return;

    int i = 0;
    for (const auto& pair : m_mpPollenInfo) {
        if (i >= 6) break;

        const auto& psPollenInfo = pair.second;
        // TODO: 汇编还原 - Check if pollen is harvestable
        // if (psPollenInfo.biHarvestDate > GetCurrentDate()) {
        //     psCropInfo[i].nItemID = psPollenInfo.dwCultivationItem;
        //     psCropInfo[i].biHarvestDate = psPollenInfo.biHarvestDate;
        // }
        (void)psPollenInfo;
        ++i;
    }
}

// IDA: ?GetFunitureCount@CGocMyroom@@QEAAXAEAH@Z (0x1400FD450)
void CGocMyroom::GetFunitureCount(int& nCount) const
{
    nCount = static_cast<int>(m_psFunitureList.size());
}

// IDA: ?WriteBoard@CGocMyroom@@QEAAXUPS_REQ_MYROOM_BOARD_WRITE@@@Z (0x1400FD480)
void CGocMyroom::WriteBoard(const PS_REQ_MYROOM_BOARD_WRITE& psReqBoardWrite)
{
    // TODO: 汇编还原 - Write board message
    (void)psReqBoardWrite;
}

// IDA: ?SendBoardWrite@CGocMyroom@@QEAAXUPS_RES_MYROOM_BOARD_WRITE@@@Z (0x1400FDAB0)
void CGocMyroom::SendBoardWrite(const PS_RES_MYROOM_BOARD_WRITE& psResBoardWrite)
{
    // TODO: 汇编还原 - Send board write result
    (void)psResBoardWrite;
}

// IDA: ?SetBoard@CGocMyroom@@QEAAXUST_MYROOM_BOARD_INFO@@@Z (0x1400FDB80)
void CGocMyroom::SetBoard(const ST_MYROOM_BOARD_INFO& stBoardInfo)
{
    m_stBoardInfo = stBoardInfo;
}

// IDA: ?SendWriteBoardInfo@CGocMyroom@@QEAAXXZ (0x1400FE040)
void CGocMyroom::SendWriteBoardInfo()
{
    // TODO: 汇编还原 - Send write board info packet
}

//=============================================================================
// Rank Operations
//=============================================================================

// IDA: ?LoadMyroomRankInfo@CGocMyroom@@QEAAXUPS_MYROOM_RANK_LIST@@0UPS_MYROOM_RANK_INFO@@1@Z (0x1400FDBE0)
void CGocMyroom::LoadMyroomRankInfo(PS_MYROOM_RANK_LIST& psCurrentRankList, PS_MYROOM_RANK_LIST& psPastRankList,
                                     PS_MYROOM_RANK_INFO& psMyCurrentRankInfo, PS_MYROOM_RANK_INFO& psMyPastRankInfo)
{
    m_psCurrentRankList = psCurrentRankList;
    m_psPastRankList = psPastRankList;
    m_psMyCurrentRankInfo = psMyCurrentRankInfo;
    m_psMyPastRankInfo = psMyPastRankInfo;
}

// IDA: ?SendMyroomRankInfo@CGocMyroom@@QEAAXXZ (0x1400FDE00)
void CGocMyroom::SendMyroomRankInfo()
{
    // TODO: 汇编还原 - Send rank info packet
}

// IDA: ?GetRankRewardID@CGocMyroom@@QEAAHXZ (0x1400FE290)
int CGocMyroom::GetRankRewardID()
{
    // TODO: 汇编还原 - Get rank reward ID from table
    return 0;
}

// IDA: ?CheckRank@CGocMyroom@@QEAAHK@Z (0x1400FE380)
int CGocMyroom::CheckRank(std::uint32_t dwUAID)
{
    // TODO: 汇编还原 - Check rank status
    (void)dwUAID;
    return 0;
}

// IDA: ?RankReward@CGocMyroom@@QEAAXUPS_DB_MYROOM_RANK_REWARD@@@Z (0x1400FE7F0)
void CGocMyroom::RankReward(PS_DB_MYROOM_RANK_REWARD& psRankReward)
{
    // TODO: 汇编还原 - Process rank reward
    (void)psRankReward;
}

// IDA: ?SendRankRewardPost@CGocMyroom@@QEAAXKUST_CREATE_ITEMS@@@Z (0x1400FE980)
void CGocMyroom::SendRankRewardPost(std::uint32_t dwUAID, const ST_CREATE_ITEMS& stCreateItems)
{
    // TODO: 汇编还原 - Send rank reward via post
    (void)dwUAID;
    (void)stCreateItems;
}

//=============================================================================
// Furniture Operations
//=============================================================================

// IDA: ?LoadMyroomFunitureList@CGocMyroom@@QEAAXUPS_MYROOM_FUNITURE_LIST@@@Z (0x1400FE120)
void CGocMyroom::LoadMyroomFunitureList(PS_MYROOM_FUNITURE_LIST& psFunitureList)
{
    m_psFunitureList = psFunitureList.vecInfo;
}

// IDA: ?AddFuniture@CGocMyroom@@QEAAXH@Z (0x1400FE160)
void CGocMyroom::AddFuniture(int nFunitureID)
{
    m_psFunitureList.push_back(nFunitureID);
}

// IDA: ?RemoveFuniture@CGocMyroom@@QEAAXH@Z (0x1400FE1F0)
void CGocMyroom::RemoveFuniture(int nFunitureID)
{
    for (auto it = m_psFunitureList.begin(); it != m_psFunitureList.end(); ++it) {
        if (*it == nFunitureID) {
            m_psFunitureList.erase(it);
            return;
        }
    }
}

//=============================================================================
// DB Log
//=============================================================================

// IDA: ?SendDBLog@CGocMyroom@@QEAAXFKH@Z (0x1400FEBD0)
void CGocMyroom::SendDBLog(int nLogType, std::uint32_t dwUAID, int nParam)
{
    // TODO: 汇编还原 - Send DB log
    (void)nLogType;
    (void)dwUAID;
    (void)nParam;
}

// IDA: ?GetMyroomBoardInfo@CGocMyroom@@QEAAXAEAUST_MYROOM_OWNER_INFO@@@Z (0x1400FED50)
void CGocMyroom::GetMyroomBoardInfo(ST_MYROOM_OWNER_INFO& stOwnerInfo) const
{
    stOwnerInfo = m_stMyRoomInfo;
}

// IDA: ?SetCommunityInfo@CGocMyroom@@QEAAXUPS_MYROOM_COMMUNITY_INFO@@@Z (0x1400FEDB0)
void CGocMyroom::SetCommunityInfo(const PS_MYROOM_COMMUNITY_INFO& psCommunityInfo)
{
    m_stMyRoomInfo.nRecommendCount = psCommunityInfo.nRecommendCount;
    m_stMyRoomInfo.nFavoriteCount = psCommunityInfo.nFavoriteCount;
}
