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
// IDA decompiled: Check for daily update at 9 AM and call UpdateData if needed
void CGocMyroom::OnUpdate(float fDeltaTime)
{
    // Get owner CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    // Check if user has myroom loaded (bit 6 of UserDB+3)
    if ((pUser->GetCharInfo()->UserDB[3] & 0x40) != 0) return;
    
    // Get current time
    std::time_t now = std::time(nullptr);
    std::tm* tm_now = std::localtime(&now);
    
    // Check if hour >= 9
    if (tm_now->tm_hour >= 9) {
        // Create today's 9 AM time
        std::tm tm_9am = *tm_now;
        tm_9am.tm_hour = 9;
        tm_9am.tm_min = 0;
        tm_9am.tm_sec = 0;
        std::time_t t_9am = std::mktime(&tm_9am);
        std::time_t t_now = std::mktime(tm_now);
        
        // Check if m_tInitDate < t_9am and m_tInitDate > 0
        if (m_tInitDate < t_9am && m_tInitDate > 0) {
            UpdateData();
            m_tInitDate = t_now;
        }
    }
    (void)fDeltaTime;
}

// IDA: ?UpdateData@CGocMyroom@@QEAAXXZ (0x1400FB0F0)
// IDA decompiled: Send DB request to update myroom data
void CGocMyroom::UpdateData()
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return;
    
    // Send DB packet (Main=0x25, Sub=0x31)
    XSendDBPacket xSendDBPacket(pUser, 0x25, 0x31);
    xSendDBPacket.XParse << pUser->GetUAID();
    pServer->SendDBGame(xSendDBPacket);
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
// IDA decompiled: Build and send PS_MYROOM_POLLEN_LIST packet to client
void CGocMyroom::SendMyRoomLoad()
{
    // Build pollen list from m_mpPollenInfo
    PS_MYROOM_POLLEN_LIST psPollenList;
    for (const auto& pair : m_mpPollenInfo) {
        psPollenList.vecInfo.push_back(pair.second);
    }
    
    // Send packet (Main=0x26, Sub=0x12)
    XSendPacket xSendPacket(0x26, 0x12);
    xSendPacket << m_stMyRoomInfo;
    xSendPacket << psPollenList;
    
    XActor* pActor = GetOwnerActor();
    if (pActor) {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
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
// IDA decompiled: Update pollen cultivation dates and item info
void CGocMyroom::PollenCultivation(PS_DB_MYROOM_POLLEN_CULTIVATION& psCultivation)
{
    auto it = m_mpPollenInfo.find(psCultivation.stInfo.nPollenIndex);
    if (it == m_mpPollenInfo.end()) return;
    
    PS_MYROOM_POLLEN_INFO& psInfo = it->second;
    psInfo.biHarvestDate = psCultivation.stInfo.biHarvestDate;
    psInfo.biStartDate = psCultivation.stInfo.biStartDate;
    psInfo.biWiltDate = psCultivation.stInfo.biWiltDate;
    psInfo.biRotDate = psCultivation.stInfo.biRotDate;
    psInfo.dwCultivationItem = psCultivation.stInfo.dwCultivationItem;
    
    if (psCultivation.dwNutritionItem) {
        psInfo.dwNutritionItem = psCultivation.dwNutritionItem;
    }
    if (psCultivation.dwWiltBlockItem) {
        psInfo.dwWiltBlockItem = psCultivation.dwWiltBlockItem;
    }
    
    SendPollenUpdate(psInfo);
}

// IDA: ?PollenClear@CGocMyroom@@QEAAXH@Z (0x1400FBBF0)
void CGocMyroom::PollenClear(int nPollenIndex)
{
    m_mpPollenInfo.erase(nPollenIndex);
}

// IDA: ?PollenItemUse@CGocMyroom@@QEAAXUPS_MYROOM_POLLEN_INFO@@@Z (0x1400FBD50)
// IDA decompiled: Update pollen item usage info
void CGocMyroom::PollenItemUse(const PS_MYROOM_POLLEN_INFO& psInfo)
{
    auto it = m_mpPollenInfo.find(psInfo.nPollenIndex);
    if (it == m_mpPollenInfo.end()) return;
    
    PS_MYROOM_POLLEN_INFO& psExisting = it->second;
    psExisting.biHarvestDate = psInfo.biHarvestDate;
    psExisting.dwNutritionItem = psInfo.dwNutritionItem;
    psExisting.dwWiltBlockItem = psInfo.dwWiltBlockItem;
    
    if (psInfo.biWiltDate) {
        psExisting.biWiltDate = psInfo.biWiltDate;
    }
    if (psInfo.biRotDate) {
        psExisting.biRotDate = psInfo.biRotDate;
    }
    
    SendPollenUpdate(psExisting);
}

// IDA: ?PollenAddHelpUser@CGocMyroom@@QEAAXHUPS_MYROOM_POLLEN_HELP_USER@@_J@Z (0x1400FBED0)
// IDA decompiled: Add help user to pollen for cultivation assistance
void CGocMyroom::PollenAddHelpUser(int nPollenIndex, const PS_MYROOM_POLLEN_HELP_USER& stHelpUser, std::int64_t biHelpDate)
{
    auto it = m_mpPollenInfo.find(nPollenIndex);
    if (it == m_mpPollenInfo.end()) return;
    
    PS_MYROOM_POLLEN_INFO& psInfo = it->second;
    
    // Check max help users (5)
    if (psInfo.vecHelpUsers.size() >= 5) return;
    
    // Check if user already helping
    for (const auto& helper : psInfo.vecHelpUsers) {
        if (helper.dwUCID == stHelpUser.dwUCID) return;
    }
    
    // Add help user
    psInfo.vecHelpUsers.push_back(stHelpUser);
    psInfo.biHarvestDate = biHelpDate;
    
    SendPollenUpdate(psInfo);
}

// IDA: ?SendDBMyRoomIndex@CGocMyroom@@QEAAXXZ (0x1400FC0C0)
// IDA decompiled: Send DB request for myroom index
void CGocMyroom::SendDBMyRoomIndex()
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return;
    
    // Send DB packet (Main=0x25, Sub=6)
    XSendDBPacket xSendDBPacket(pUser, 0x25, 6);
    xSendDBPacket.XParse << pUser->GetUCID();
    pServer->SendDBGame(xSendDBPacket);
}

// IDA: ?SendPollenUpdate@CGocMyroom@@QEAAXUPS_MYROOM_POLLEN_INFO@@@Z (0x1400FC1D0)
// IDA decompiled: Send pollen update packet to client
void CGocMyroom::SendPollenUpdate(const PS_MYROOM_POLLEN_INFO& psInfo)
{
    // Send packet (Main=0x26, Sub=0x36)
    XSendPacket xSendPacket(0x26, 0x36);
    xSendPacket << psInfo;
    
    XActor* pActor = GetOwnerActor();
    if (pActor) {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// IDA: ?SetPollenLockCount@CGocMyroom@@QEAA_NHE@Z (0x1400FC2B0)
// IDA decompiled: Set pollen lock count (number of helpers allowed)
bool CGocMyroom::SetPollenLockCount(int nPollenIndex, int nLockCount)
{
    auto it = m_mpPollenInfo.find(nPollenIndex);
    if (it == m_mpPollenInfo.end()) return false;
    
    PS_MYROOM_POLLEN_INFO& psInfo = it->second;
    
    // Check if current lock count >= new count
    if (psInfo.byLockCount >= nLockCount) return false;
    
    // Check if help user count < new count
    if (psInfo.vecHelpUsers.size() < static_cast<size_t>(nLockCount)) return false;
    
    psInfo.byLockCount = static_cast<std::uint8_t>(nLockCount);
    return true;
}

//=============================================================================
// Recommend Operations
//=============================================================================

// IDA: ?LoadMyroomRecommend@CGocMyroom@@QEAAXUPS_MYROOM_RECOMMEND_LIST@@@Z (0x1400FC520)
// IDA decompiled: Load recommendation list and set init date
void CGocMyroom::LoadMyroomRecommend(PS_MYROOM_RECOMMEND_LIST& psRecommendList)
{
    m_setRecommendList.clear();
    
    for (const auto& dwOwnerUAID : psRecommendList.vecUAID) {
        AddRecommend(dwOwnerUAID);
    }
    
    // Set init date if before 9 AM
    std::time_t now = std::time(nullptr);
    std::tm* tm_now = std::localtime(&now);
    if (tm_now->tm_hour < 9) {
        m_tInitDate = now;
    }
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
// IDA decompiled: Process room recommendation request
void CGocMyroom::Recommend(std::uint32_t dwUAID, std::uint32_t dwOwnerUAID)
{
    // Get XMyRoom instance
    XMyRoom* pMyRoom = GetMyRoomInstance();
    if (!pMyRoom) {
        PS_RES_MYROOM_RECOMMEND psRes;
        psRes.nResult = 58277;
        SendRecommend(psRes);
        return;
    }
    
    // Check if trying to recommend own room
    if (pMyRoom->IsMyRoomOwner(dwUAID)) {
        PS_RES_MYROOM_RECOMMEND psRes;
        psRes.nResult = 58234;
        SendRecommend(psRes);
        return;
    }
    
    // Check if room owner matches
    if (pMyRoom->GetOwnerUAID() != dwOwnerUAID) {
        PS_RES_MYROOM_RECOMMEND psRes;
        psRes.nResult = 58236;
        SendRecommend(psRes);
        return;
    }
    
    std::uint32_t ownerUAID = pMyRoom->GetOwnerUAID();
    
    // Check if already recommended today
    if (FindRecommend(ownerUAID)) {
        PS_RES_MYROOM_RECOMMEND psRes;
        psRes.nResult = 58235;
        SendRecommend(psRes);
        return;
    }
    
    // Update daily mission
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (pUser) {
        auto pMission = pUser->GetGOC<CGocDailyMission>();
        if (pMission) {
            pMission->UpdateMyRoomType(eDAILY_MISSION_CONDITION_MYROOM_RECOMMEND);
        }
    }
    
    // Add recommendation
    AddRecommend(ownerUAID);
    
    // Send DB update
    PS_DB_MYROOM_RECOMMEND psDBRecommend;
    psDBRecommend.dwUAID = dwUAID;
    psDBRecommend.dwOwnerUAID = ownerUAID;
    
    XSendDBPacket xSendDBPacket(pUser, 0x25, 0x24);
    xSendDBPacket << psDBRecommend;
    XGameServer::Instance()->SendDBGame(xSendDBPacket);
    
    // Update recommend count and send response
    ST_MYROOM_OWNER_INFO stOwnerInfo;
    pMyRoom->GetOwnerInfo(&stOwnerInfo);
    
    PS_RES_MYROOM_RECOMMEND psRes;
    psRes.nRecommendCount = ++stOwnerInfo.nRecommendCount;
    pMyRoom->UpdateRecommendCount(stOwnerInfo.nRecommendCount);
    SendRecommend(psRes);
    
    SendDBLog(7, ownerUAID, 0);
}

// IDA: ?SendRecommend@CGocMyroom@@QEAAXUPS_RES_MYROOM_RECOMMEND@@@Z (0x1400FCF70)
// IDA decompiled: Send recommend result packet to client
void CGocMyroom::SendRecommend(const PS_RES_MYROOM_RECOMMEND& psResRecommend)
{
    // Send packet (Main=0x26, Sub=0x40)
    XSendPacket xSendPacket(0x26, 0x40);
    xSendPacket << psResRecommend;
    
    XActor* pActor = GetOwnerActor();
    if (pActor) {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
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
// IDA decompiled: Send favorite list packet to client
void CGocMyroom::SendFavoriteList(PS_MYROOM_FAVORITE_LIST& psFavoriteList)
{
    // Send packet (Main=0x26, Sub=0x47)
    XSendPacket xSendPacket(0x26, 0x47);
    xSendPacket << psFavoriteList;
    
    XActor* pActor = GetOwnerActor();
    if (pActor) {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
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
// IDA decompiled: Request favorite info from DB
void CGocMyroom::SendFavoriteInfo()
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return;
    
    // Send DB packet (Main=0x25, Sub=0x33)
    XSendDBPacket xSendDBPacket(pUser, 0x25, 0x33);
    xSendDBPacket.XParse << pUser->GetUAID();
    pServer->SendDBGame(xSendDBPacket);
}

// IDA: ?CheckFavorite@CGocMyroom@@QEAAHUPS_REQ_MYROOM_FAVORITE@@K@Z (0x1400FDF30)
// IDA decompiled: Check favorite request validity
int CGocMyroom::CheckFavorite(const PS_REQ_MYROOM_FAVORITE& psReqFavorite, std::uint32_t dwUAID)
{
    std::uint32_t dwOwnerUAID = psReqFavorite.dwOwnerUAID;
    
    if (psReqFavorite.bFavorite) {
        // Adding favorite - check limits
        if (m_mapFavoriteList.size() >= 50) return 58240;  // Max 50 favorites
        if (FindFavorite(dwOwnerUAID)) return 58239;  // Already favorited
        
        XMyRoom* pMyRoom = GetMyRoomInstance();
        if (!pMyRoom) return 58277;
        
        if (pMyRoom->GetOwnerUAID() == dwUAID) return 58237;  // Can't favorite own room
        if (pMyRoom->GetOwnerUAID() != dwOwnerUAID) return 58241;  // Invalid owner
    } else {
        // Removing favorite
        if (!FindFavorite(dwOwnerUAID)) return 58238;  // Not in favorite list
    }
    
    return 0;  // Success
}

//=============================================================================
// Board Operations
//=============================================================================

// IDA: ?LoadBoardList@CGocMyroom@@QEAAXUPS_REQ_MYROOM_BOARD_INFO@@@Z (0x1400FD160)
// IDA decompiled: Request board list from DB
void CGocMyroom::LoadBoardList(const PS_REQ_MYROOM_BOARD_INFO& psReqBoardInfo)
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return;
    
    // Send DB packet (Main=0x25, Sub=0x26)
    XSendDBPacket xSendDBPacket(pUser, 0x25, 0x26);
    xSendDBPacket << psReqBoardInfo;
    pServer->SendDBGame(xSendDBPacket);
}

// IDA: ?SendBoardList@CGocMyroom@@QEAAXUPS_MYROOM_BOARD_LIST@@@Z (0x1400FD240)
// IDA decompiled: Send board list packet to client
void CGocMyroom::SendBoardList(PS_MYROOM_BOARD_LIST& psBoardList)
{
    // Send packet (Main=0x26, Sub=0x48)
    XSendPacket xSendPacket(0x26, 0x48);
    xSendPacket << psBoardList;
    
    XActor* pActor = GetOwnerActor();
    if (pActor) {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// IDA: ?GetCrop@CGocMyroom@@QEAAXPEAUPS_MYROOM_BOARD_POT@@@Z (0x1400FD360)
// IDA decompiled: Get crop info from pollen (harvestable items)
void CGocMyroom::GetCrop(PS_MYROOM_BOARD_POT* psCropInfo)
{
    if (!psCropInfo) return;
    
    int i = 0;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return;
    
    std::int64_t biCurDate = pServer->GetCurDate();
    
    for (auto iter = m_mpPollenInfo.begin(); iter != m_mpPollenInfo.end() && i <= 6; ++iter) {
        const PS_MYROOM_POLLEN_INFO& psPollenInfo = iter->second;
        
        // Check if pollen is harvestable (harvest date > current date)
        if (psPollenInfo.biHarvestDate > biCurDate) {
            psCropInfo[i].nItemID = psPollenInfo.dwCultivationItem;
            psCropInfo[i].biHarvestDate = psPollenInfo.biHarvestDate;
            ++i;
        }
    }
}

// IDA: ?GetFunitureCount@CGocMyroom@@QEAAXAEAH@Z (0x1400FD450)
void CGocMyroom::GetFunitureCount(int& nCount) const
{
    nCount = static_cast<int>(m_psFunitureList.size());
}

// IDA: ?WriteBoard@CGocMyroom@@QEAAXUPS_REQ_MYROOM_BOARD_WRITE@@@Z (0x1400FD480)
// IDA decompiled: Write board message to DB
void CGocMyroom::WriteBoard(const PS_REQ_MYROOM_BOARD_WRITE& psReqBoardWrite)
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    // Validate UCID
    if (psReqBoardWrite.dwUCID != pUser->GetUCID()) {
        LogHelper::LogError("game.contents", "WriteBoard error - Invalid User[ UCID:%d ]( %d )", 
            psReqBoardWrite.dwUCID, 511);
        return;
    }
    
    PS_RES_MYROOM_BOARD_WRITE psResWriteInfo;
    
    // Check if myroom is created
    if (!IsMyRoomCreate()) {
        psResWriteInfo.nResult = 58245;
        SendBoardWrite(psResWriteInfo);
        return;
    }
    
    // Validate content length (must be 2-32 characters)
    size_t contentLen = wcslen(psReqBoardWrite.szContents);
    if (contentLen < 2 || contentLen > 32) {
        psResWriteInfo.nResult = 58246;
        SendBoardWrite(psResWriteInfo);
        return;
    }
    
    // Check if board already exists and not expired
    if (m_stBoardInfo.dwOwnerUAID && m_stBoardInfo.biRegDate > XGameServer::Instance()->GetCurDate()) {
        psResWriteInfo.nResult = 58244;
        psResWriteInfo.nRemainDate = static_cast<int>(m_stBoardInfo.biRegDate - XGameServer::Instance()->GetCurDate());
        SendBoardWrite(psResWriteInfo);
        return;
    }
    
    // Create board info
    ST_MYROOM_BOARD_INFO stBoardInfo;
    stBoardInfo.dwOwnerUAID = pUser->GetUAID();
    stBoardInfo.dwUCID = psReqBoardWrite.dwUCID;
    stBoardInfo.nRecommendCount = m_stMyRoomInfo.nRecommendCount;
    stBoardInfo.nFavoriteCount = m_stMyRoomInfo.nFavoriteCount;
    stBoardInfo.biRegDate = XGameServer::Instance()->GetCurDate() + 7;  // 7 days expiry
    wcscpy_s(stBoardInfo.szContents, psReqBoardWrite.szContents);
    wcscpy_s(stBoardInfo.szName, m_stMyRoomInfo.szRoomName);
    
    for (int i = 0; i < 12; ++i) {
        stBoardInfo.nFunitureID[i] = psReqBoardWrite.nFunitureID[i];
    }
    
    GetCrop(stBoardInfo.stPotInfo);
    GetFunitureCount(stBoardInfo.nFunitureCount);
    
    // Send to DB
    XSendDBPacket xSendDBPacket(pUser, 0x25, 0x27);
    xSendDBPacket << stBoardInfo;
    XGameServer::Instance()->SendDBGame(xSendDBPacket);
    
    // Update local board info
    SetBoard(stBoardInfo);
    
    // Send response
    psResWriteInfo = m_stBoardInfo;
    SendBoardWrite(psResWriteInfo);
    
    SendDBLog(5, 0, 0);
}

// IDA: ?SendBoardWrite@CGocMyroom@@QEAAXUPS_RES_MYROOM_BOARD_WRITE@@@Z (0x1400FDAB0)
// IDA decompiled: Send board write result packet to client
void CGocMyroom::SendBoardWrite(const PS_RES_MYROOM_BOARD_WRITE& psResBoardWrite)
{
    // Send packet (Main=0x26, Sub=0x42)
    XSendPacket xSendPacket(0x26, 0x42);
    xSendPacket << psResBoardWrite;
    
    XActor* pActor = GetOwnerActor();
    if (pActor) {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// IDA: ?SetBoard@CGocMyroom@@QEAAXUST_MYROOM_BOARD_INFO@@@Z (0x1400FDB80)
void CGocMyroom::SetBoard(const ST_MYROOM_BOARD_INFO& stBoardInfo)
{
    m_stBoardInfo = stBoardInfo;
}

// IDA: ?SendWriteBoardInfo@CGocMyroom@@QEAAXXZ (0x1400FE040)
// IDA decompiled: Send write board info packet to client
void CGocMyroom::SendWriteBoardInfo()
{
    // Send packet (Main=0x26, Sub=0x43)
    XSendPacket xSendPacket(0x26, 0x43);
    xSendPacket << m_psFunitureList;
    
    XActor* pActor = GetOwnerActor();
    if (pActor) {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
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
// IDA decompiled: Send myroom rank info packet to client
void CGocMyroom::SendMyroomRankInfo()
{
    // Send packet (Main=0x26, Sub=0x46)
    XSendPacket xSendPacket(0x26, 0x46);
    xSendPacket << m_psCurrentRankList;
    xSendPacket << m_psPastRankList;
    xSendPacket << m_psMyCurrentRankInfo;
    xSendPacket << m_psMyPastRankInfo;
    
    XActor* pActor = GetOwnerActor();
    if (pActor) {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// IDA: ?GetRankRewardID@CGocMyroom@@QEAAHXZ (0x1400FE290)
// IDA decompiled: Get rank reward ID based on past rank
int CGocMyroom::GetRankRewardID()
{
    int nRewardID = 0;
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return 0;
    
    auto& rankRewardMap = pServer->GetResourceMgr().GetTB_ROOMRANK_REWARD_Map();
    
    for (auto iter = rankRewardMap.begin(); iter != rankRewardMap.end(); ++iter) {
        const TB_ROOMRANK_REWARD* pReward = &iter->second;
        
        // Check if user's rank falls within reward range
        if (m_psMyPastRankInfo.byPastRank >= pReward->Rank_Range_Min &&
            m_psMyPastRankInfo.byPastRank <= pReward->Rank_Range_Max) {
            return pReward->Ranking_Reward_ID;
        }
        
        // Store default reward (range 0-0) if no specific range matches
        if (pReward->Rank_Range_Min == 0 && pReward->Rank_Range_Max == 0) {
            nRewardID = pReward->Ranking_Reward_ID;
        }
    }
    
    return nRewardID;
}

// IDA: ?CheckRank@CGocMyroom@@QEAAHK@Z (0x1400FE380)
// IDA decompiled: Check and process rank reward
int CGocMyroom::CheckRank(std::uint32_t dwUAID)
{
    // Get inventory component
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return 0;
    
    auto pInventory = pUser->GetGOC<CGocInventory>();
    if (!pInventory) return 0;
    
    // Check if reward already claimed
    if (m_psMyPastRankInfo.bReward) {
        return 58243;  // Already received
    }
    
    // Check if user has valid rank
    if (m_psMyPastRankInfo.byPastRank <= 0) {
        LogHelper::LogError("game.contents", "CheckRank error - Not participation myroom ranking[UAID:%d, Rank:%d]",
            dwUAID, m_psMyPastRankInfo.byPastRank);
        return 58248;  // No rank
    }
    
    // Get reward ID
    int nRankRewardID = GetRankRewardID();
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return 0;
    
    const TB_ROOMRANK_REWARD* pTB_RankReward = pServer->GetResourceMgr().GetTB_ROOMRANK_REWARD(nRankRewardID);
    if (!pTB_RankReward) {
        LogHelper::LogError("game.contents", "CheckRank error - TB_ROOMRANK_REWARD[UAID:%d, Rank:%d, ID:%d]",
            dwUAID, m_psMyPastRankInfo.byPastRank, nRankRewardID);
        return 58242;  // Reward table not found
    }
    
    // Build reward items
    ST_CREATE_ITEMS stCreateItems;
    for (int i = 0; i < 2; ++i) {
        std::uint32_t nItemID = (i == 0) ? pTB_RankReward->Ranking_Reward_Item_01 : pTB_RankReward->Ranking_Reward_Item_02;
        std::int16_t sCount = (i == 0) ? pTB_RankReward->Item_Amount_01 : pTB_RankReward->Item_Amount_02;
        
        const TB_ITEM* pTB_Item = pServer->GetResourceMgr().GetTB_ITEM(nItemID);
        if (!pTB_Item) break;
        
        // Check stack limit
        if (pTB_Item->Item_Stack_Max < sCount) {
            return 52013;  // Stack limit exceeded
        }
        
        ST_CREATE_ITEM stCreateItemInfo;
        stCreateItemInfo.nItemID = nItemID;
        stCreateItemInfo.shCount = sCount;
        stCreateItems.push_back(stCreateItemInfo);
    }
    
    // Validate item count
    if (stCreateItems.size() > 5 || stCreateItems.empty()) {
        return 52013;  // Invalid item count
    }
    
    // Send DB request
    PS_DB_MYROOM_RANK_REWARD psDBRewardInfo;
    psDBRewardInfo.dwUAID = dwUAID;
    psDBRewardInfo.byReward = 1;
    psDBRewardInfo.stRewardItems = stCreateItems;
    
    XSendDBPacket xSendDBPacket(pUser, 0x25, 0x30);
    xSendDBPacket << psDBRewardInfo;
    pServer->SendDBGame(xSendDBPacket);
    
    return 0;  // Success
}

// IDA: ?RankReward@CGocMyroom@@QEAAXUPS_DB_MYROOM_RANK_REWARD@@@Z (0x1400FE7F0)
// IDA decompiled: Process rank reward from DB response
void CGocMyroom::RankReward(PS_DB_MYROOM_RANK_REWARD& psRankReward)
{
    int nResult = 0;
    
    if (psRankReward.nResult) {
        // Error occurred
        nResult = 58243;
    } else {
        // Send reward items via post
        SendRankRewardPost(psRankReward.dwUAID, psRankReward.stRewardItems);
        m_psMyPastRankInfo.bReward = 1;
        SendDBLog(8, 0, m_psMyPastRankInfo.byPastRank);
    }
    
    // Send result packet (Main=0x26, Sub=0x48)
    XSendPacket xSendPacket(0x26, 0x48);
    xSendPacket << nResult;
    
    XActor* pActor = GetOwnerActor();
    if (pActor) {
        CGocNetwork::Send(pActor, &xSendPacket);
    }
}

// IDA: ?SendRankRewardPost@CGocMyroom@@QEAAXKUST_CREATE_ITEMS@@@Z (0x1400FE980)
// IDA decompiled: Send rank reward items via post system
void CGocMyroom::SendRankRewardPost(std::uint32_t dwUAID, const ST_CREATE_ITEMS& stCreateItems)
{
    // Build post data
    ST_ACCOUNT_POST_DATA stAccountPostData;
    stAccountPostData.byMainType = 4;  // System post
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer) return;
    
    stAccountPostData.bySubType = pServer->GetSystemPostTableIndex(5, 4);
    stAccountPostData.dwUAID = dwUAID;
    stAccountPostData.biRegTime = pServer->GetCurDate();
    stAccountPostData.biDelDate = pServer->GetCurDate() + 29454;  // ~81 days expiry
    
    // Add reward items
    for (size_t i = 0; i < stCreateItems.size(); ++i) {
        stAccountPostData.stItemList[i].xSerial = 0;
        stAccountPostData.stItemList[i].nItemID = stCreateItems[i].nItemID;
        stAccountPostData.stItemList[i].sCount = stCreateItems[i].shCount;
    }
    
    // Validate post type
    if (!stAccountPostData.bySubType) {
        LogHelper::LogError("game.contents", "SendRankRewardPost error - Check TB_SystemMail( %d )", 790);
    }
    
    // Send to DB
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    XSendDBPacket xSendDBPacket(pUser, 6, 0x18);
    xSendDBPacket << stAccountPostData;
    pServer->SendDBGame(xSendDBPacket);
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
// IDA decompiled: Send DB log for myroom activities
void CGocMyroom::SendDBLog(int nLogType, std::uint32_t dwUAID, int nParam)
{
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) return;
    
    ST_LOG_GAME stLog;
    stLog._sMainType = 19;  // MyRoom log type
    stLog._sSubType = static_cast<std::int16_t>(nLogType);
    stLog._nUAID = pUser->GetUAID();
    stLog._nUCID = pUser->GetUCID();
    stLog.nParam0 = m_stMyRoomInfo.dwMapID;
    stLog.nParam1 = dwUAID;
    stLog.nParam2 = nParam;
    stLog.nParam5 = pUser->GetLevel();
    
    XGameServer::Instance()->SendDBLog(&stLog);
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
