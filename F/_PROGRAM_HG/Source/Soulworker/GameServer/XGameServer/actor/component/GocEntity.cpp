// CGocEntity - Game Object Component for entity management
// Restored from GameServer.exe IDA decompilation
//
// Key functions decompiled from GameServer.exe (port 10004):
// - Constructor: 0x14005B440
// - Destructor: 0x14005B610
// - Init: 0x14005B6E0
// - OnUpdate: 0x14005B880
// - ClearInteraction: 0x14005B920
// - LoadTitle: 0x14005B940
// - AddTitle: 0x14005BDC0
// - DeleteTitle: 0x14005C9A0
// - InitTitle: 0x14005C480
// - UpdateTitle: 0x14005CC50
// - SendTitleList: 0x14005E420
// - ClearTitle: 0x14005DA10

#include "GocEntity.h"
#include <cstring>

// Forward declarations - avoid circular dependencies
class CMover;
class CGocAttribute;

// ST_HAVE_TITLE_INFO constructor implementation
ST_HAVE_TITLE_INFO::ST_HAVE_TITLE_INFO(TB_TITLE_INFO* pTB, bool bFav)
    : pTBTitle(pTB), bFavorite(bFav)
{
    for (int i = 0; i < 5; ++i) {
        wStatType[i] = 0;
        fStatValue[i] = 0.0f;
        byStatIndex[i] = 0;
    }
}

// Helper function to get owner mover (stub)
static CMover* GetOwnerMover()
{
    // TODO: Implement actual owner retrieval
    return nullptr;
}

// IDA: ??0CGocEntity@@QEAA@XZ (0x14005B440)
// Verified: Constructor initializes base class and all member variables
// Per IDA decompile at 0x14005B440:
// - Calls GOComponent::GOComponent(this)
// - Sets vftable to CGocEntity::`vftable'
// - Initializes m_bLoadTitle = 0
// - Constructs m_stInsideTitle, m_stOutsideTitle (PS_ROGUELIKE_CURRENT_INFO)
// - Constructs m_mapHaveTitle, m_setTitleOpen, m_mapInteractionBox
// - Constructs m_stSGAuthInfo, m_stGFAuthInfo, m_stWMAuthInfo, m_stRepresentativeInfo
// - Constructs m_stCheckAutoBlock array
// - Constructs m_mapProfilePhoto, m_vecRoguelikeMapList, m_vecRoguelikeResult
CGocEntity::CGocEntity()
    : GOComponent()
    , m_bLoadTitle(false)
    , m_shFavoritePrefixCount(0)
    , m_shFavoriteSuffixCount(0)
    , m_nVaccumCubeID(0)
    , m_bNetCafe(false)
    , m_bLoadNetCafe(false)
    , m_nNetCafeEventNo(0)
    , m_bSendSGCharInfo(false)
    , m_nFreeReviveCount(0)
    , m_nProfilePhotoTick(0)
    , m_nRoguelikeStep(0)
    , m_nRoguelikeTotalStep(0)
    , m_dwRoguelikeNextMap(0)
    , m_nCurRoguelikeMapIndex(0)
    , m_bRoguelikeState(false)
{
    // Per IDA: Member maps and structs are default-constructed
    std::memset(&m_stSGAuthInfo, 0, sizeof(m_stSGAuthInfo));
    std::memset(&m_stGFAuthInfo, 0, sizeof(m_stGFAuthInfo));
    std::memset(&m_stWMAuthInfo, 0, sizeof(m_stWMAuthInfo));
    std::memset(&m_stRepresentativeInfo, 0, sizeof(m_stRepresentativeInfo));
    std::memset(m_szNetCafeTestIP, 0, sizeof(m_szNetCafeTestIP));

    // Per IDA: Initialize m_stCheckAutoBlock array
    for (int i = 0; i < 1; ++i) {
        m_stCheckAutoBlock[i].bSend = false;
        m_stCheckAutoBlock[i].nCount = 0;
    }
}

// IDA: ??1CGocEntity@@UEAA@XZ (0x14005B610)
// Verified: Destructor cleans up in reverse order
// Per IDA decompile at 0x14005B610:
// - Sets vftable to CGocEntity::`vftable'
// - Destructs m_vecRoguelikeResult, m_vecRoguelikeMapList
// - Destructs m_mapProfilePhoto
// - Destructs m_stCheckAutoBlock array
// - Destructs m_mapInteractionBox, m_setTitleOpen, m_mapHaveTitle
// - Calls GOComponent::~GOComponent
CGocEntity::~CGocEntity()
{
    // Per IDA: Destruct in reverse order of construction
    m_vecRoguelikeResult.clear();
    m_vecRoguelikeMapList.clear();
    m_mapProfilePhoto.clear();

    for (int i = 0; i < 1; ++i) {
        m_stCheckAutoBlock[i].vecData.clear();
    }

    m_mapInteractionBox.clear();
    m_setTitleOpen.clear();
    m_mapHaveTitle.clear();
}

bool CGocEntity::Initialize()
{
    Init();
    return true;
}

void CGocEntity::Shutdown()
{
    ClearTitle();
    ClearInteraction();
}

void CGocEntity::Update(float fDeltaTime)
{
    (void)fDeltaTime;
    OnUpdate();
}

// IDA: ?Init@CGocEntity@@QEAAXXZ (0x14005B6E0)
// Verified: Initializes all member variables to default state
// Per IDA decompile at 0x14005B6E0:
// - Clears title info (m_stInsideTitle, m_stOutsideTitle)
// - Clears m_mapHaveTitle, m_setTitleOpen
// - Resets favorite counts, vaccum cube ID, netcafe state
// - Calls ClearInteraction()
// - Clears m_stSGAuthInfo, m_stCheckAutoBlock
// - Clears m_mapProfilePhoto, calls ClearRoguelikeData()
void CGocEntity::Init()
{
    // Per IDA: Clear title info
    m_stInsideTitle.dwPrefix = 0;
    m_stInsideTitle.dwSuffix = 0;
    m_stOutsideTitle.dwPrefix = 0;
    m_stOutsideTitle.dwSuffix = 0;
    m_bLoadTitle = false;

    // Per IDA: Clear title maps
    m_mapHaveTitle.clear();
    m_setTitleOpen.clear();

    // Per IDA: Reset favorite counts
    m_shFavoritePrefixCount = 0;
    m_shFavoriteSuffixCount = 0;

    // Per IDA: Reset vaccum cube ID
    m_nVaccumCubeID = 0;

    // Per IDA: Reset netcafe state
    m_bNetCafe = false;
    m_bLoadNetCafe = false;

    // Per IDA: Call ClearInteraction
    ClearInteraction();

    // Per IDA: Clear netcafe test IP and event number
    m_szNetCafeTestIP[0] = '\0';
    m_nNetCafeEventNo = 0;
    m_bSendSGCharInfo = false;

    // Per IDA: Clear SG auth info
    std::memset(&m_stSGAuthInfo, 0, sizeof(m_stSGAuthInfo));

    // Per IDA: Initialize m_stCheckAutoBlock array
    for (int i = 0; i < 1; ++i) {
        m_stCheckAutoBlock[i].bSend = false;
        m_stCheckAutoBlock[i].nCount = 0;
        m_stCheckAutoBlock[i].vecData.clear();
    }

    // Per IDA: Reset free revive count
    m_nFreeReviveCount = 0;

    // Per IDA: Clear profile photo map and tick
    m_mapProfilePhoto.clear();
    m_nProfilePhotoTick = 0;

    // Per IDA: Call ClearRoguelikeData
    ClearRoguelikeData();
}

// IDA: ?OnUpdate@CGocEntity@@QEAAXXZ (0x14005B880)
// Verified: Updates entity state, checks user DB flags, calls ProfilePhotoRemainTimeCheck
// Per IDA decompile at 0x14005B880:
// - Gets owner CUser via internal pointer
// - Checks stMyCharInfoEx.UserDB bit flags
// - If flag set, gets CGocInventory and calls SetReadyLoadCash(1)
// - Calls ProfilePhotoRemainTimeCheck()
void CGocEntity::OnUpdate()
{
    // Per IDA: Get owner CUser
    // CMover* pOwner = GetOwnerGO();
    // if (!pOwner) return;
    //
    // CUser* pUser = static_cast<CUser*>(pOwner);
    //
    // Per IDA: Check UserDB flags (bit 3 of UserDB+4)
    // if ((pUser->stMyCharInfoEx->UserDB & 8) != 0) {
    //     // Get CGocInventory component
    //     std::shared_ptr<CGocInventory> pInvePtr;
    //     CMover::GetGOC<CGocInventory>(pOwner, &pInvePtr, 0);
    //     if (pInvePtr) {
    //         CGocInventory* pInventory = pInvePtr.get();
    //         CGocInventory::SetReadyLoadCash(pInventory, 1);
    //     }
    // }
    //
    // Per IDA: Call ProfilePhotoRemainTimeCheck
    // ProfilePhotoRemainTimeCheck();

    // TODO: 汇编还原 - Need CUser, CGocInventory, ProfilePhotoRemainTimeCheck implementation
}

// IDA: ?ClearInteraction@CGocEntity@@QEAAXXZ (0x14005B920)
// Verified: Simple function that clears m_mapInteractionBox
// Per IDA decompile at 0x14005B920:
// - Calls std::map::clear on m_mapInteractionBox
void CGocEntity::ClearInteraction()
{
    m_mapInteractionBox.clear();
}

// IDA: ?LoadTitle@CGocEntity@@QEAAXAEAUPS_TITLE_LOAD@@@Z (0x14005B940)
// Verified: Per IDA decompile - loads title info from PS_TITLE_LOAD packet
// - Sets m_bLoadTitle = true
// - Gets TB_COMMON for favorite limit check (index 0x9C41 = 40001)
// - Iterates vecTitleID, validates against TB_TITLE_INFO, handles favorite counts
// - Iterates vecOpenTitleID, validates and inserts into m_setTitleOpen
void CGocEntity::LoadTitle(PS_TITLE_LOAD& stLoadTitle)
{
    m_bLoadTitle = true;

    // TODO: 汇编还原 - Need XGameServer, XResourceMgr, TB_COMMON, TB_TITLE_INFO types
    // Per IDA:
    // - Get TB_COMMON for favorite limit (index 0x9C41 = 40001)
    // - For each title in vecTitleID:
    //   - Validate against TB_TITLE_INFO
    //   - Handle favorite count limits based on Title_Type (prefix/suffix)
    //   - Create ST_HAVE_TITLE_INFO and insert into m_mapHaveTitle
    // - For each title in vecOpenTitleID:
    //   - Validate and insert into m_setTitleOpen

    // Stub implementation - just store titles without validation
    for (const auto& stInfo : stLoadTitle.vecTitleID)
    {
        ST_HAVE_TITLE_INFO stHaveTitle(nullptr, stInfo.bFavorite);
        m_mapHaveTitle.insert(std::make_pair(stInfo.dwTitleID, stHaveTitle));
    }

    for (const auto& stInfo : stLoadTitle.vecOpenTitleID)
    {
        m_setTitleOpen.insert(static_cast<int>(stInfo.dwTitleID));
    }
}

// IDA: ?InitTitle@CGocEntity@@QEAAXAEAUST_TitleInfo@@0@Z (0x14005C480)
// Verified: Per IDA decompile - initializes equipped titles with stat updates
// - Validates outside title prefix/suffix against TB_TITLE_INFO
// - Copies stOutsideTitle to m_stOutsideTitle and CUser::stMyCharInfoEx.stTitleInfo
// - For inside title prefix/suffix, applies stat effects via UpdateTitleStat
// - Calls CalculateChangedStat and CalculateChangedEffect if changed
void CGocEntity::InitTitle(ST_TitleInfo& stOutsideTitle, ST_TitleInfo& stInsideTitle)
{
    // TODO: 汇编还原 - Need XGameServer, XResourceMgr, TB_TITLE_INFO, CGocAttribute types
    // Per IDA:
    // - Validate outside title prefix/suffix against TB_TITLE_INFO
    // - Set m_stOutsideTitle and update CUser::stMyCharInfoEx.stTitleInfo
    // - For inside title prefix/suffix, apply stat effects via UpdateTitleStat
    // - Call CalculateChangedStat/CalculateChangedEffect if changed

    // Stub implementation - just store title info
    m_stOutsideTitle = stOutsideTitle;
    m_stInsideTitle = stInsideTitle;
}

// IDA: ?UpdateTitle@CGocEntity@@QEAAXAEAUPS_REQ_TITLE_UPDATE@@@Z (0x14005CC50)
// Verified: Per IDA decompile - updates equipped titles with full validation
// - Validates all 4 title IDs (inside prefix/suffix, outside prefix/suffix)
// - Removes old stat effects and applies new ones for inside title changes
// - Sends DB packet (main=3, sub=0x16) if inside/outside changed
// - Broadcasts PS_SYNC_TITLE_UPDATE (main=3, sub=0x26) if outside changed
// - Logs to ST_LOG_GAME (main=3, sub=13)
void CGocEntity::UpdateTitle(PS_REQ_TITLE_UPDATE& stTitleInfo)
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Validate all 4 title IDs via IsValidTitle
    // - Handle inside prefix/suffix changes with stat effect updates
    // - Send DB packet (main=3, sub=0x16) if changed
    // - Broadcast PS_SYNC_TITLE_UPDATE (main=3, sub=0x26) if outside changed
    // - Log to ST_LOG_GAME (main=3, sub=13)

    // Stub implementation - just update title info
    // Note: ST_TITLE_INFO_SELECT uses dwTitleID for prefix and dwSuffix for suffix
    bool bChangeOutside = (m_stOutsideTitle.dwPrefix != stTitleInfo.stOutsideTitle.dwTitleID ||
                           m_stOutsideTitle.dwSuffix != stTitleInfo.stOutsideTitle.dwSuffix);
    bool bChangeInside = (m_stInsideTitle.dwPrefix != stTitleInfo.stInsideTitle.dwTitleID ||
                          m_stInsideTitle.dwSuffix != stTitleInfo.stInsideTitle.dwSuffix);

    m_stOutsideTitle.dwPrefix = stTitleInfo.stOutsideTitle.dwTitleID;
    m_stOutsideTitle.dwSuffix = stTitleInfo.stOutsideTitle.dwSuffix;
    m_stInsideTitle.dwPrefix = stTitleInfo.stInsideTitle.dwTitleID;
    m_stInsideTitle.dwSuffix = stTitleInfo.stInsideTitle.dwSuffix;

    // TODO: Send update notification
}

// IDA: Helper functions for title validation and update
bool CGocEntity::IsValidTitle(uint32_t dwTitleID, bool bIsSuffix)
{
    // TODO: 汇编还原 - Validate title exists in TB_TITLE_INFO and player owns it
    if (dwTitleID == 0) return true;  // 0 means no title equipped
    return m_mapHaveTitle.find(dwTitleID) != m_mapHaveTitle.end();
}

void CGocEntity::SendUpdateTitle(ST_TitleInfo& stInsideTitle, ST_TitleInfo& stOutsideTitle, bool bResult)
{
    // TODO: 汇编还原 - Send title update packet to client
    // Per IDA: XSendPacket (main=3, sub=0x25) with PS_RES_TITLE_UPDATE
}

// IDA: ?ClearTitle@CGocEntity@@QEAAXXZ (0x14005DA10)
// Verified: Per IDA decompile - clears all titles and updates stats
// - Gets CGocAttribute component
// - For equipped inside title prefix/suffix, applies negative stat effects
// - Calls CalculateChangedStat and CalculateChangedEffect if changed
// - Resets m_stInsideTitle and m_stOutsideTitle to 0
// - Updates CUser::stMyCharInfoEx.stTitleInfo
// - Clears m_mapHaveTitle
// - Sends DB packet (main=3, sub=0x17)
// - Broadcasts PS_SYNC_TITLE_UPDATE packet (main=3, sub=0x26)
void CGocEntity::ClearTitle()
{
    // TODO: 汇编还原 - Need CGocAttribute, TB_TITLE_INFO types
    // Per IDA:
    // - Get CGocAttribute component and remove stat effects from equipped titles
    // - Call CalculateChangedStat/CalculateChangedEffect if changed
    // - Reset m_stInsideTitle and m_stOutsideTitle to 0
    // - Update CUser::stMyCharInfoEx.stTitleInfo
    // - Clear m_mapHaveTitle
    // - Send DB packet (main=3, sub=0x17)
    // - Broadcast PS_SYNC_TITLE_UPDATE (main=3, sub=0x26)

    // Stub implementation - just clear data
    m_stInsideTitle.dwPrefix = 0;
    m_stInsideTitle.dwSuffix = 0;
    m_stOutsideTitle.dwPrefix = 0;
    m_stOutsideTitle.dwSuffix = 0;
    m_mapHaveTitle.clear();
}

// IDA: ?AddTitle@CGocEntity@@QEAA_NKH@Z (0x14005BDC0)
// Verified: Per IDA decompile - adds a title with class validation
// - Gets TB_TITLE_INFO from XResourceMgr
// - Validates class requirement (Use_Class must match or be 0)
// - Creates ST_HAVE_TITLE_INFO and inserts into m_mapHaveTitle
// - Removes from m_setTitleOpen if present
// - Sends DB packet (main=3, sub=0x15)
// - Broadcasts packet (main=3, sub=0x24)
// - Logs to ST_LOG_GAME (main=3, sub=12)
bool CGocEntity::AddTitle(uint32_t dwTitleID, int nLogType)
{
    // TODO: 汇编还原 - Need XGameServer, XResourceMgr, TB_TITLE_INFO, CGocAttribute types
    // Per IDA:
    // - Get TB_TITLE_INFO from XResourceMgr
    // - Validate class requirement (Use_Class must match or be 0)
    // - Create ST_HAVE_TITLE_INFO and insert into m_mapHaveTitle
    // - Remove from m_setTitleOpen if present
    // - Send DB packet (main=3, sub=0x15)
    // - Broadcast packet (main=3, sub=0x24)
    // - Log to ST_LOG_GAME (main=3, sub=12)

    // Stub implementation - just add title without validation
    ST_HAVE_TITLE_INFO stHaveTitle(nullptr, false);

    auto result = m_mapHaveTitle.insert(std::make_pair(dwTitleID, stHaveTitle));
    if (!result.second)
    {
        return false;  // Duplicate title
    }

    // Remove from open set if present
    auto itOpen = m_setTitleOpen.find(dwTitleID);
    if (itOpen != m_setTitleOpen.end())
    {
        m_setTitleOpen.erase(itOpen);
    }

    // TODO: Send DB packet and broadcast

    return true;
}

// IDA: ?DeleteTitle@CGocEntity@@QEAAXH@Z (0x14005C9A0)
// Verified: Per IDA decompile - deletes a title with validation
// - Checks if title is currently equipped (inside/outside prefix/suffix)
// - Finds title in m_mapHaveTitle and erases it
// - Sends DB packet (main=3, sub=0x18)
// - Broadcasts packet (main=3, sub=0x28)
void CGocEntity::DeleteTitle(int nTitleID)
{
    // Validate title not currently equipped
    if (nTitleID == m_stInsideTitle.dwPrefix ||
        nTitleID == m_stInsideTitle.dwSuffix ||
        nTitleID == m_stOutsideTitle.dwPrefix ||
        nTitleID == m_stOutsideTitle.dwSuffix)
    {
        return;  // Cannot delete equipped title
    }

    // Find and erase title from map
    auto it = m_mapHaveTitle.find(nTitleID);
    if (it == m_mapHaveTitle.end())
    {
        return;  // Title not found
    }
    m_mapHaveTitle.erase(it);

    // Send DB packet (main=3, sub=0x18)
    // TODO: XSendDBPacket with QuestID and TitleID

    // Broadcast packet (main=3, sub=0x28)
    // TODO: XSendPacket with TitleID
}

// IDA: ?SendTitleList@CGocEntity@@QEAAXXZ (0x14005E420)
// Verified: Per IDA decompile - sends title list to client
// - Creates PS_TITLE_LOAD packet with m_bLoadTitle flag
// - Iterates m_mapHaveTitle and adds to vecTitleID (ST_TITLE_INFO_DB)
// - Iterates m_setTitleOpen and adds to vecOpenTitleID
// - Sends packet (main=3, sub=0x23)
// - Creates PS_RES_TITLE_UPDATE with current selected titles
// - Sends packet (main=3, sub=0x25)
void CGocEntity::SendTitleList()
{
    // TODO: 汇编还原 - Need XSendPacket, PS_RES_TITLE_UPDATE types
    // Per IDA:
    // - Build PS_TITLE_LOAD packet with m_bLoadTitle, m_mapHaveTitle, m_setTitleOpen
    // - Send packet (main=3, sub=0x23)
    // - Build PS_RES_TITLE_UPDATE with current selected titles
    // - Send packet (main=3, sub=0x25)

    // Build PS_TITLE_LOAD packet
    PS_TITLE_LOAD stTitleLoad;
    // Note: PS_TITLE_LOAD doesn't have bResult field - it's handled differently

    // Add owned titles
    for (const auto& pair : m_mapHaveTitle)
    {
        ST_TITLE_INFO_DB stTitle;
        stTitle.dwTitleID = pair.first;
        stTitle.bFavorite = pair.second.bFavorite;
        stTitleLoad.vecTitleID.push_back(stTitle);
    }

    // Add open titles
    for (const auto& nTitleID : m_setTitleOpen)
    {
        ST_TITLE_INFO_DB stTitle;
        stTitle.dwTitleID = static_cast<unsigned int>(nTitleID);
        stTitleLoad.vecOpenTitleID.push_back(stTitle);
    }

    // Send packet (main=3, sub=0x23)
    // TODO: XSendPacket with stTitleLoad

    // Build response packet for selected titles
    // Note: Need to define PS_RES_TITLE_UPDATE structure
    // PS_RES_TITLE_UPDATE stResSelected;
    // stResSelected.bResult = true;
    // stResSelected.stInsideTitle = m_stInsideTitle;
    // stResSelected.stOutsideTitle = m_stOutsideTitle;

    // Send packet (main=3, sub=0x25)
    // TODO: XSendPacket with stResSelected
}

// IDA: 0x14005EF20 - Update title stat via CGocAttribute
void CGocEntity::UpdateTitleStat(int nClassType, int nLevel, uint32_t nType, float fValue)
{
    // TODO: 汇编还原 - Complex function (IDA: 0x14005EF20)
    // Gets owner CMover and CGocAttribute component
    // Calls CGocAttribute::UpdateEffectStat(nClassType, nType, fValue, 0)
    (void)nClassType;
    (void)nLevel;
    (void)nType;
    (void)fValue;
}

// IDA: 0x14005EB40 - Empty function (no-op)
void CGocEntity::Levelup(int nBeforeLevel, int nAfterLevel)
{
    (void)nBeforeLevel;
    (void)nAfterLevel;
}

// Title getters
bool CGocEntity::GetNetCafe() const
{
    return m_bNetCafe;
}

bool CGocEntity::IsLoadNetCafe() const
{
    return m_bLoadNetCafe;
}

// Roguelike system
void CGocEntity::UpdateRoguelikeStep()
{
    ++m_nRoguelikeStep;
    ++m_nRoguelikeTotalStep;
}

// IDA: 0x1400656B0 - Get next roguelike map
int CGocEntity::GetRoguelikeNextMap(bool bFirst)
{
    if (!bFirst) {
        ++m_nCurRoguelikeMapIndex;
    }
    if (m_nCurRoguelikeMapIndex >= m_vecRoguelikeMapList.size()) {
        return 0;
    }
    int nMapID = static_cast<int>(m_vecRoguelikeMapList[m_nCurRoguelikeMapIndex]);
    if (nMapID > 0) {
        m_dwRoguelikeNextMap = nMapID;
    }
    m_nRoguelikeStep = 0;
    return nMapID;
}

int CGocEntity::GetRoguelikeStep() const
{
    return m_nRoguelikeStep;
}

int CGocEntity::GetRoguelikeMapIndex() const
{
    return m_nCurRoguelikeMapIndex;
}

// Auth system
char* CGocEntity::GetWMPortalID()
{
    return m_stWMAuthInfo.szWMAccountID;
}

uint64_t CGocEntity::GetRepresentativeUCID() const
{
    return m_stRepresentativeInfo.dwUCID;
}

const ST_REPRESENTATIVE_INFO& CGocEntity::GetRepresentativeInfo() const
{
    return m_stRepresentativeInfo;
}

void CGocEntity::SetRepresentativeInfo(const ST_REPRESENTATIVE_INFO& stInfo)
{
    m_stRepresentativeInfo = stInfo;
}

void CGocEntity::SetWMAuthInfo(const ST_WM_AUTH_INFO& stInfo)
{
    m_stWMAuthInfo = stInfo;
}

void CGocEntity::SetSGAuthInfo(const ST_SG_AUTH_INFO& stInfo)
{
    m_stSGAuthInfo = stInfo;
}

// VaccumCube system
void CGocEntity::SetVaccumCubeID(int nID)
{
    m_nVaccumCubeID = nID;
}

int CGocEntity::GetVaccumCubeID() const
{
    return m_nVaccumCubeID;
}

// ============================================================================
// IDA: ?GetOutsideTitle@CGocEntity@@QEAA?AUST_TitleInfo@@XZ (0x14004EA00)
// Verified: Per IDA decompile - returns outside title info
// - Simply copies m_stOutsideTitle to result
ST_TitleInfo CGocEntity::GetOutsideTitle() const
{
    return m_stOutsideTitle;
}

// ============================================================================
// IDA: ?AddTitleByClass@CGocEntity@@QEAAKKH@Z (0x14005C3C0)
// Verified: Per IDA decompile - adds title based on class
// - Gets TB_TITLE_REWARD from XResourceMgr
// - Gets player class from CGocAttribute
// - Selects title ID from Title_Reward_ID array by class index
// - Calls AddTitle with selected title ID
std::uint32_t CGocEntity::AddTitleByClass(std::uint32_t dwTitleGroupID, int nLogType)
{
    // TODO: 汇编还原 - Need XGameServer, XResourceMgr, TB_TITLE_REWARD, TXSingleton types
    // Per IDA:
    // - Get XGameServer singleton
    // - Get TB_TITLE_REWARD from XResourceMgr by dwTitleGroupID
    // - Get player class from CGocAttribute component
    // - Select title ID from Title_Reward_ID array by class index
    // - Call AddTitle with selected title ID

    (void)dwTitleGroupID;
    (void)nLogType;
    return 0;
}

// ============================================================================
// IDA: ?UpdateOpenTitle@CGocEntity@@QEAAXHH@Z (0x14005E090)
// Verified: Per IDA decompile - updates open title based on condition
// - Gets prefix and suffix titles to open from XResourceMgr
// - For each title, checks if already in m_setTitleOpen or m_mapHaveTitle
// - If not owned, adds to m_setTitleOpen and sends DB/client notifications
void CGocEntity::UpdateOpenTitle(int nCondition, int nValue)
{
    // TODO: 汇编还原 - Need XGameServer, XResourceMgr, GetPrefixTitleToOpen, GetSuffixTitleToOpen
    // Per IDA:
    // - Get player class from CGocAttribute
    // - Get XGameServer singleton
    // - Get prefix and suffix titles to open from XResourceMgr
    // - For each title, check if already in m_setTitleOpen or m_mapHaveTitle
    // - If not owned, add to m_setTitleOpen and send DB/client notifications

    (void)nCondition;
    (void)nValue;
}

// ============================================================================
// IDA: ?SendDBLoadTitle@CGocEntity@@QEAAXXZ (0x14005E8D0)
// Verified: Per IDA decompile - sends DB request to load title
// - Creates XSendDBPacket with main=3, sub=0x14
// - Sends via XGameServer::SendDBGame
void CGocEntity::SendDBLoadTitle()
{
    // TODO: 汇编还原 - Need XSendDBPacket, XGameServer types
    // Per IDA:
    // - Create XSendDBPacket with main=3, sub=0x14
    // - Send via XGameServer::SendDBGame
}

// ============================================================================
// IDA: ?CheckAutoBlockCount@CGocEntity@@QEAAXW4AUTO_BLOCK_CHECK_TYPE@@HK@Z (0x14005D970)
// Verified: Per IDA decompile - checks auto block count
// - Gets owner CUser via RTDynamicCast
// - Updates auto block check info based on type
void CGocEntity::CheckAutoBlockCount(int eType, int nAdd, uint32_t dwData)
{
    // TODO: 汇编还原 - Need AUTO_BLOCK_CHECK_TYPE enum, CUser type
    // Per IDA:
    // - Get owner CUser via RTDynamicCast from CMover
    // - Update auto block check info based on type
    (void)eType;
    (void)nAdd;
    (void)dwData;
}

// ============================================================================
// IDA: ?CheckEchelonTitle@CGocEntity@@QEAAXHEH@Z (0x14005E9E0)
// Verified: Per IDA decompile - checks and awards echelon titles
// - For levels >= 55, iterates through echelon levels
// - Gets TB_ECHELON from XResourceMgr and awards prefix/suffix titles
void CGocEntity::CheckEchelonTitle(int nLevel, uint8_t byEchelonLevel, int nClass)
{
    // TODO: 汇编还原 - Need XGameServer, XResourceMgr, TB_ECHELON types
    // Per IDA:
    // - Check if level >= 55
    // - For each echelon level from 1 to byEchelonLevel:
    //   - Get TB_ECHELON from XResourceMgr
    //   - Get prefix/suffix titles to open
    //   - Add title if valid and not already owned
    (void)nLevel;
    (void)byEchelonLevel;
    (void)nClass;
}

// ============================================================================
// IDA: ?ReqFavoriteTitle@CGocEntity@@QEAAXAEAUPS_TITLE_FAVORITE@@@Z (0x14005F210)
// Verified: Per IDA decompile - requests title favorite toggle
// - Validates title exists in TB_TITLE_INFO and player owns it
// - Updates favorite counts based on title type (prefix/suffix)
// - Sends DB packet to update favorite status
void CGocEntity::ReqFavoriteTitle(PS_TITLE_FAVORITE& stTitleFavorite)
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Get owner actor ID
    // - Validate title exists in TB_TITLE_INFO
    // - Get TB_COMMON for favorite limit
    // - Check player owns title in m_mapHaveTitle
    // - Update favorite counts based on title type
    // - Send DB packet (main=3, sub=0x24) if changed
    (void)stTitleFavorite;
}

// ============================================================================
// IDA: ?ResFavoriteTitle@CGocEntity@@QEAAXAEAUPS_DB_TITLE_FAVORITE@@@Z (0x14005F840)
// Verified: Per IDA decompile - handles DB response for title favorite
// - On error, reverts favorite count changes
// - On success, updates m_mapHaveTitle and sends packet to client
void CGocEntity::ResFavoriteTitle(PS_DB_TITLE_FAVORITE& stTitleFavorite)
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Check for DB error, revert counts on failure
    // - Get title from m_mapHaveTitle
    // - Update favorite flag in ST_HAVE_TITLE_INFO
    // - Send packet to client (main=3, sub=0x2A)
    // - Log to ST_LOG_GAME
    (void)stTitleFavorite;
}

// ============================================================================
// IDA: ?UpdateCutscene@CGocEntity@@QEAAXAEAUPS_CUTSCENE_UPDATE@@@Z (0x14005FDD0)
// Verified: Per IDA decompile - updates cutscene state
// - Gets XMaze from owner's area
// - Calls CutSceneMgr to set/skip cutscene based on bOnPlay flag
// - Updates CGocRecode component
void CGocEntity::UpdateCutscene(PS_CUTSCENE_UPDATE& stCutscene)
{
    // TODO: 汇编还原 - Need XMaze, CCutsceneManager, CGocRecode types
    // Per IDA:
    // - Get XMaze from owner's area
    // - If bOnPlay: call CCutsceneManager::SetCutscene
    // - Else: call CCutsceneManager::SkipCutscene
    // - Get CGocRecode component and call SetShowCutscene(1)
    (void)stCutscene;
}

// ============================================================================
// IDA: ?LoginNetCafe@CGocEntity@@QEAAX_N@Z (0x14005FFA0)
// Verified: Per IDA decompile - handles netcafe login
// - Checks server option E_SERVER_OPTION_NETCAFE
// - If enabled, sets netcafe state from user's stMyCharInfoEx
// - Otherwise calls EventNetCafeItemDelete
void CGocEntity::LoginNetCafe(bool bFirstEnter)
{
    // TODO: 汇编还原 - Need XGameServer, XResourceMgr, CUser types
    // Per IDA:
    // - Get XGameServer singleton
    // - Check GetServerContents(E_SERVER_OPTION_NETCAFE)
    // - If enabled: get CUser and call SetNetCafe with stMyCharInfoEx.bNetCafe
    // - Else: call EventNetCafeItemDelete
    (void)bFirstEnter;
}

// ============================================================================
// IDA: ?SetNetCafe@CGocEntity@@QEAAX_N00@Z (0x140060030)
// Verified: Per IDA decompile - sets netcafe state with DB sync
// - Updates m_bLoadNetCafe and m_bNetCafe flags
// - Syncs with CUser::stMyCharInfoEx
// - Sends DB packet if bDBSync is true
// - Updates CGocBooster for netcafe bonus
// - Sends FP update packet to client
// - Updates CGocEvent and CGocAkashicRecord components
void CGocEntity::SetNetCafe(bool bNetCafe, bool bDBSync, bool bLoad)
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Set m_bLoadNetCafe = bLoad
    // - Get CUser via RTDynamicCast and update stMyCharInfoEx flags
    // - If bNetCafe changed:
    //   - Update m_bNetCafe
    //   - Send DB packet (main=3, sub=0x59) if bDBSync
    //   - Update CGocBooster (eBooster_Type_NetCafe)
    //   - Send FP update packet (main=3, sub=0x64)
    // - Call CGocEvent::SetStartNetCafeMission
    // - If !m_bNetCafe && bDBSync: call EventNetCafeItemDelete
    // - Check CGocAkashicRecord events

    m_bLoadNetCafe = bLoad;
    m_bNetCafe = bNetCafe;
    (void)bDBSync;
}

// ============================================================================
// IDA: ?SendNetCafeState@CGocEntity@@QEAAXXZ (0x140060650)
// Verified: Per IDA decompile - sends netcafe state to client
// - Creates PS_NETCAFE_INFO packet
// - Sends packet (main=3, sub=0x54)
void CGocEntity::SendNetCafeState()
{
    // TODO: 汇编还原 - Need PS_NETCAFE_INFO, XSendPacket types
    // Per IDA:
    // - Create PS_NETCAFE_INFO with m_bNetCafe
    // - Send packet (main=3, sub=0x54)
}

// ============================================================================
// IDA: ?SendSGAuthInfo@CGocEntity@@QEAAX_N@Z (0x140060740)
// Verified: Per IDA decompile - sends SG auth info
// - If bSyncDB: sends DB packet (main=2, sub=0x52) to AccountDB
// - Sends log packet (main=0x42, sub=0x20)
// - If auth type is 2 or 3: sends token update packet (main=1, sub=0x11)
void CGocEntity::SendSGAuthInfo(bool bSyncDB)
{
    // TODO: 汇编还原 - Need XSendDBPacket, XSendPacket, PS_SG_TOKEN_UPDATE types
    // Per IDA:
    // - If bSyncDB: send DB packet (main=2, sub=0x52) with m_stSGAuthInfo
    // - Send log packet (main=0x42, sub=0x20) with m_stSGAuthInfo
    // - If auth type is 2 or 3:
    //   - Create PS_SG_TOKEN_UPDATE with converted tokens
    //   - Send packet (main=1, sub=0x11)
    (void)bSyncDB;
}

// ============================================================================
// IDA: ?EventNetCafeItemBuy@CGocEntity@@QEAAXAEAUPS_EVENT_NETCAFE_ITEM_BUY@@@Z (0x140060A10)
// Verified: Per IDA decompile - handles netcafe item purchase
// - Validates server option, load state, and netcafe state
// - Checks item count limit (max 8 items)
// - Validates each item against TB_ITEM, TB_ITEM_CLASSIFY, PCCostume
// - Creates items via CGocInventory::CreateItem2
// - Sends DB packet to log purchase
void CGocEntity::EventNetCafeItemBuy(PS_EVENT_NETCAFE_ITEM_BUY& stBuy)
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Check server option NETCAFE, m_bLoadNetCafe, m_bNetCafe
    // - Limit purchase to 8 items max
    // - For each item:
    //   - Validate PCCostume, TB_ITEM, TB_ITEM_CLASSIFY
    //   - Check item type (Item_Use_Type==1, Item_Cash==3, Item_Inven_Type==2)
    //   - Check class restriction
    //   - Check if item already exists
    // - Create items via CGocInventory::CreateItem2
    // - Send DB packet (main=0x21, sub=0x49)
    (void)stBuy;
}

// ============================================================================
// IDA: ?EventNetCafeItemDelete@CGocEntity@@QEAAXXZ (0x1400619D0)
// Verified: Per IDA decompile - deletes netcafe items when leaving netcafe
// - Gets CGocInventory component
// - Iterates through inventory and cash inventory
// - Finds items with Item_Cash==3 (netcafe items)
// - Reduces and deletes them
// - Sends DB and client notifications
void CGocEntity::EventNetCafeItemDelete()
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Get CGocInventory component
    // - Get inventory info for cash (type 4) and costume (type 3)
    // - Find all items with Item_Cash==3
    // - Call CGocInventory::ReduceItem3 for each
    // - Call CGocInventory::UpdateItemEnd
    // - Send DB packet (main=0x21, sub=0x22)
    // - Send client packet (main=0x2A, sub=0x2B) with deleted item IDs
}

// ============================================================================
// IDA: ?SetFreeReviveCount@CGocEntity@@QEAAXH_N@Z (0x140062070)
// Verified: Per IDA decompile - sets free revive count
// - Updates m_nFreeReviveCount
// - If bSync: sends DB packet (main=3, sub=0x85) and calls SendFreeReviveCount
void CGocEntity::SetFreeReviveCount(int nCount, bool bSync)
{
    // TODO: 汇编还原 - Need XSendDBPacket, PS_CHARACTER_FREE_REVIVE types
    // Per IDA:
    // - Set m_nFreeReviveCount = nCount
    // - If bSync:
    //   - Create PS_CHARACTER_FREE_REVIVE with UCID and count
    //   - Send DB packet (main=3, sub=0x85)
    //   - Call SendFreeReviveCount

    m_nFreeReviveCount = nCount;
    (void)bSync;
}

// ============================================================================
// IDA: ?ReviveFree@CGocEntity@@QEAA_NXZ (0x1400621F0)
// Verified: Per IDA decompile - increments free revive count
// - Gets TB_ITEM for item ID 0x26272A93
// - Checks if count is below max stack
// - Increments and syncs via SetFreeReviveCount
bool CGocEntity::ReviveFree()
{
    // TODO: 汇编还原 - Need XGameServer, XResourceMgr, TB_ITEM types
    // Per IDA:
    // - Get TB_ITEM for item ID 0x26272A93
    // - If count >= Item_Stack_Max: return false
    // - Increment count and call SetFreeReviveCount with sync=true
    // - Return true

    // Increment free revive count if below max
    // Max is from TB_ITEM(0x26272A93)->Item_Stack_Max
    ++m_nFreeReviveCount;
    return true;
}

// ============================================================================
// IDA: ?SendDBProfilePhoto@CGocEntity@@QEAAXXZ (0x1400622D0)
// Verified: Per IDA decompile - sends DB request to load profile photo
// - Sends DB packet (main=3, sub=0x25) with UCID
void CGocEntity::SendDBProfilePhoto()
{
    // TODO: 汇编还原 - Need XSendDBPacket, XGameServer types
    // Per IDA:
    // - Create XSendDBPacket (main=3, sub=0x25)
    // - Send UCID
    // - Send via XGameServer::SendDBGame
}

// ============================================================================
// IDA: ?LoadProfilePhoto@CGocEntity@@QEAAXAEAUPS_PROFILE_PHOTO_LOAD@@@Z (0x1400623E0)
// Verified: Per IDA decompile - loads profile photos from DB response
// - Iterates through vecList and calls AddProfilePhoto for each
// - Calls CheckEquipProfilePhoto
// - Sets m_nProfilePhotoTick for timeout check
void CGocEntity::LoadProfilePhoto(PS_PROFILE_PHOTO_LOAD& stLoad)
{
    // TODO: 汇编还原 - Need PS_PROFILE_PHOTO_LOAD, ST_PROFILE_PHOTO_INFO types
    // Per IDA:
    // - For each entry in vecList: call AddProfilePhoto
    // - Call CheckEquipProfilePhoto
    // - Set m_nProfilePhotoTick = GetTickCount64() + 10000

    (void)stLoad;
    m_nProfilePhotoTick = static_cast<int>(std::time(nullptr)) + 10000;
}

// ============================================================================
// IDA: ?CheckEquipProfilePhoto@CGocEntity@@QEAAXXZ (0x1400624E0)
// Verified: Per IDA decompile - checks and sets default equipped profile photo
// - Gets CUser and current equipped photo ID
// - If photo not in owned list, finds default photo based on class/awaken
// - Sends DB packet to update equipped photo
void CGocEntity::CheckEquipProfilePhoto()
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Get CUser via RTDynamicCast
    // - Get current equipped photo ID from stMyCharInfoEx.stBaseInfo.dwProfilePhotoID
    // - Check if photo exists in m_mapProfilePhoto
    // - If not, get default photo based on class and awaken state
    // - Send DB packet (main=3, sub=0x28) to update
}

// ============================================================================
// IDA: ?CheckAddProfilePhoto@CGocEntity@@QEAA_NKAEAUST_PROFILE_PHOTO_INFO@@@Z (0x140062820)
// Verified: Per IDA decompile - validates and prepares profile photo info for adding
// - Validates TB_ITEM and TB_PHOTO_ITEM exist
// - Checks class restriction
// - Handles period type (permanent vs timed)
// - Returns false if validation fails
bool CGocEntity::CheckAddProfilePhoto(uint32_t dwItemID, ST_PROFILE_PHOTO_INFO& stPhoto)
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Get TB_ITEM and TB_PHOTO_ITEM
    // - Check class restriction
    // - Handle Item_Use_Period_Type:
    //   - 0: permanent photo
    //   - 2: timed photo with duration
    // - Calculate end date for timed photos
    // - Return true if valid

    (void)dwItemID;
    (void)stPhoto;
    return false;
}

// ============================================================================
// IDA: ?AddProfilePhoto@CGocEntity@@QEAAHAEAUST_PROFILE_PHOTO_INFO@@@Z (0x140062E50)
// Verified: Per IDA decompile - adds a profile photo to owned list
// - Validates TB_PHOTO_ITEM exists
// - Checks for duplicates
// - Creates ST_HAVE_PROFILE_PHOTO_INFO and inserts into m_mapProfilePhoto
// - Returns error code (0=success, 58010=duplication, 58011=error)
int CGocEntity::AddProfilePhoto(ST_PROFILE_PHOTO_INFO& stPhoto)
{
    // TODO: 汇编还原 - Need TB_PHOTO_ITEM, ST_HAVE_PROFILE_PHOTO_INFO types
    // Per IDA:
    // - Get CUser via RTDynamicCast
    // - Get TB_PHOTO_ITEM by stPhoto.dwPhotoID
    // - Check end date for timed photos
    // - Check for duplicates in m_mapProfilePhoto
    // - Create ST_HAVE_PROFILE_PHOTO_INFO and insert
    // - Return 0 on success, error code otherwise

    (void)stPhoto;
    return 0;
}

// ============================================================================
// IDA: ?SendProfilePhoto@CGocEntity@@QEAAXXZ (0x140063170)
// Verified: Per IDA decompile - sends profile photo list to client
// - Builds PS_PROFILE_PHOTO_LOAD with all owned photos
// - Sends packet (main=3, sub=9)
void CGocEntity::SendProfilePhoto()
{
    // TODO: 汇编还原 - Need PS_PROFILE_PHOTO_LOAD, XSendPacket types
    // Per IDA:
    // - Create PS_PROFILE_PHOTO_LOAD
    // - Iterate m_mapProfilePhoto and add to vecList
    // - Send packet (main=3, sub=9)
}

// ============================================================================
// IDA: ?ProfilePhotoRemainTimeCheck@CGocEntity@@QEAAXXZ (0x140063370)
// Verified: Per IDA decompile - checks timed profile photo expiration
// - Checks if m_nProfilePhotoTick has elapsed
// - Iterates timed photos and finds expired ones
// - Calls DeleteProfilePhoto for expired photos
// - Resets tick for next check
void CGocEntity::ProfilePhotoRemainTimeCheck()
{
    // TODO: 汇编还原 - Need ATL::CTime types
    // Per IDA:
    // - Check if m_nProfilePhotoTick > 0 and elapsed
    // - Get current time
    // - Iterate m_mapProfilePhoto for timed photos (byPeriodType==1)
    // - Check if nEndDate <= current time
    // - Collect expired photo IDs
    // - Call DeleteProfilePhoto with expired list
    // - Reset m_nProfilePhotoTick = GetTickCount64() + 10000
}

// ============================================================================
// IDA: ?ReqChangeProfilePhoto@CGocEntity@@QEAAHK@Z (0x1400634C0)
// Verified: Per IDA decompile - requests profile photo change
// - Validates new photo is owned and different from current
// - Sends DB packet (main=3, sub=0x28) to update
// - Returns error code (0=success, 58010=not owned, 58011=error, 58012=same)
int CGocEntity::ReqChangeProfilePhoto(uint32_t dwPhotoID)
{
    // TODO: 汇编还原 - Need CUser, XSendDBPacket, PS_DB_PROFILE_PHOTO_CHANGE types
    // Per IDA:
    // - Get CUser via RTDynamicCast
    // - Get current equipped photo ID
    // - Check if same as requested
    // - Validate both old and new photos exist in m_mapProfilePhoto
    // - Send DB packet (main=3, sub=0x28)

    (void)dwPhotoID;
    return 0;
}

// ============================================================================
// IDA: ?ResChangeProfilePhoto@CGocEntity@@QEAAXAEAUPS_DB_PROFILE_PHOTO_CHANGE@@@Z (0x1400638C0)
// Verified: Per IDA decompile - handles DB response for photo change
// - Updates stMyCharInfoEx.stBaseInfo.dwProfilePhotoID
// - Updates photo states in m_mapProfilePhoto
// - Sends update packets to client
// - Updates CGocParty and CGocForce with new photo
// - Logs to ST_LOG_GAME
void CGocEntity::ResChangeProfilePhoto(PS_DB_PROFILE_PHOTO_CHANGE& psPhoto)
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Get CUser via RTDynamicCast
    // - If error: log and return
    // - Update equipped photo ID in stMyCharInfoEx
    // - Update old photo state (byState=0)
    // - Update new photo state (byState=1)
    // - Send PS_PROFILE_PHOTO_UPDATE packets (main=3, sub=0xA)
    // - Send photo change packet (main=3, sub=0xC)
    // - Update community socket
    // - Update CGocParty::SetProfilePhoto
    // - Update CGocForce::SetProfilePhoto
    // - Log to ST_LOG_GAME

    (void)psPhoto;
}

// ============================================================================
// IDA: ?ProfilePhotoFavorite@CGocEntity@@QEAAXAEAUPS_PROFILE_PHOTO_FAVORITE@@@Z (0x1400643D0)
// Verified: Per IDA decompile - toggles profile photo favorite flag
// - Validates photo exists in TB_PHOTO_ITEM
// - Checks ownership in m_mapProfilePhoto
// - Sends DB packet (main=3, sub=0x27) to update favorite
void CGocEntity::ProfilePhotoFavorite(PS_PROFILE_PHOTO_FAVORITE& psFavorite)
{
    // TODO: 汇编还原 - Need XSendDBPacket, PS_DB_PROFILE_PHOTO_UPDATE types
    // Per IDA:
    // - Validate TB_PHOTO_ITEM exists
    // - Find photo in m_mapProfilePhoto
    // - Check if flag is already same
    // - Send DB packet (main=3, sub=0x27) with new favorite state

    (void)psFavorite;
}

// ============================================================================
// IDA: ?ResAddProfilePhoto@CGocEntity@@QEAAXAEAUPS_DB_PROFILE_PHOTO_ADD@@@Z (0x1400647B0)
// Verified: Per IDA decompile - handles DB response for adding photo
// - On error: unlocks inventory slots
// - On success: sends item update, adds photo to m_mapProfilePhoto
// - Sends PS_PROFILE_PHOTO_UPDATE packet to client
// - Logs to ST_LOG_GAME
void CGocEntity::ResAddProfilePhoto(PS_DB_PROFILE_PHOTO_ADD& psAdd)
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Get CUser and CGocInventory
    // - If error: call CGocInventory::UnLockList
    // - Else: call CGocInventory::SendUpdateItem
    // - Find or add photo in m_mapProfilePhoto
    // - Log to ST_LOG_GAME
    // - Send PS_PROFILE_PHOTO_UPDATE packet (main=3, sub=0xA)

    (void)psAdd;
}

// ============================================================================
// IDA: ?ResUpdateProfilePhoto@CGocEntity@@QEAAXAEAUPS_DB_PROFILE_PHOTO_UPDATE@@@Z (0x140064C50)
// Verified: Per IDA decompile - handles DB response for photo update
// - Updates photo info in m_mapProfilePhoto
// - Sends PS_PROFILE_PHOTO_UPDATE packet to client
// - Logs to ST_LOG_GAME if favorite changed
void CGocEntity::ResUpdateProfilePhoto(PS_DB_PROFILE_PHOTO_UPDATE& psUpdate)
{
    // TODO: 汇编还原 - Need CUser, XSendPacket, PS_PROFILE_PHOTO_UPDATE types
    // Per IDA:
    // - Get CUser via RTDynamicCast
    // - If error: log and return
    // - Find photo in m_mapProfilePhoto
    // - If favorite changed: log to ST_LOG_GAME
    // - Update photo info
    // - Send PS_PROFILE_PHOTO_UPDATE packet (main=3, sub=0xA)

    (void)psUpdate;
}

// ============================================================================
// IDA: ?GetProfilePhotoInfo@CGocEntity@@QEAA_NKAEAUST_PROFILE_PHOTO_INFO@@@Z (0x140064FF0)
// Verified: Per IDA decompile - gets profile photo info by ID
// - Searches m_mapProfilePhoto for dwPhotoID
// - Returns false if not found
// - Copies ST_PROFILE_PHOTO_INFO to output param on success
bool CGocEntity::GetProfilePhotoInfo(uint32_t dwPhotoID, ST_PROFILE_PHOTO_INFO& stInfo)
{
    auto it = m_mapProfilePhoto.find(static_cast<uint16_t>(dwPhotoID));
    if (it == m_mapProfilePhoto.end()) {
        return false;
    }
    // ST_HAVE_PROFILE_PHOTO_INFO contains ST_PROFILE_PHOTO_INFO at a specific offset
    // Per IDA: qmemcpy copies 16 bytes from second field of the map entry
    const auto& stHave = it->second;
    std::memcpy(&stInfo, &stHave, sizeof(ST_PROFILE_PHOTO_INFO));
    return true;
}

// ============================================================================
// IDA: ?SendFreeReviveCount@CGocEntity@@QEAAXXZ (0x1400650A0)
// Verified: Per IDA decompile - sends free revive count to client
// - Creates PS_CHARACTER_FREE_REVIVE with UCID and counts
// - Gets max count from TB_ITEM(0x26272A93)->Item_Stack_Max
// - Sends packet (main=3, sub=8) to client
void CGocEntity::SendFreeReviveCount()
{
    // TODO: 汇编还原 - Need CUser, XSendPacket, XGameServer, XResourceMgr, TB_ITEM types
    // Per IDA:
    // - Get owner actor via CMover
    // - Check actor exists and has network component
    // - Create PS_CHARACTER_FREE_REVIVE
    // - Set dwUCID from actor
    // - Set nFreeReviveCount = m_nFreeReviveCount
    // - Get TB_ITEM for ID 0x26272A93, set nMaxFreeReviveCount = Item_Stack_Max
    // - Create XSendPacket (main=3, sub=8)
    // - Send via CGocNetwork::Send
}

// ============================================================================
// IDA: ?CalculateTitleStat@CGocEntity@@QEAAXXZ (0x1400652B0)
// Verified: Per IDA decompile - calculates and applies title stats
// - Gets CGocAttribute component
// - For prefix title: applies up to 5 effects from TB_TITLE_INFO
// - For suffix title: applies up to 5 effects from TB_TITLE_INFO
// - Calls CGocAttribute::CalculateChangedStat and CalculateChangedEffect
void CGocEntity::CalculateTitleStat()
{
    // TODO: 汇编还原 - Need CMover, CGocAttribute, XGameServer, XResourceMgr, TB_TITLE_INFO types
    // Per IDA:
    // - Get CMover owner and CGocAttribute component
    // - If no attribute: log error and return
    // - If m_stInsideTitle.dwPrefix set:
    //   - Get TB_TITLE_INFO from XResourceMgr
    //   - For each of 5 effects: apply UpdateTitleStat if Effect_Type != 0
    // - If m_stInsideTitle.dwSuffix set:
    //   - Get TB_TITLE_INFO from XResourceMgr
    //   - For each of 5 effects: apply UpdateTitleStat if Effect_Type != 0
    // - If any changes: call CGocAttribute::CalculateChangedStat(0) and CalculateChangedEffect(0, a3)
}

// ============================================================================
// IDA: ?ClearRoguelikeData@CGocEntity@@QEAAXXZ (0x140065770)
// Verified: Per IDA decompile - clears all roguelike data
// - Clears m_vecRoguelikeMapList
// - Resets step counters, next map, and map index
// - Clears m_vecRoguelikeResult
// - Sets m_bRoguelikeState to false
void CGocEntity::ClearRoguelikeData()
{
    m_vecRoguelikeMapList.clear();
    m_nRoguelikeStep = 0;
    m_nRoguelikeTotalStep = 0;
    m_dwRoguelikeNextMap = 0;
    m_nCurRoguelikeMapIndex = 0;
    m_vecRoguelikeResult.clear();
    m_bRoguelikeState = false;
}

// ============================================================================
// IDA: ?GetRoguelikeStartMap@CGocEntity@@QEAAHXZ (0x1400657F0)
// Verified: Per IDA decompile - starts a new roguelike run
// - Calls ClearRoguelikeData
// - Calls InitRoguelikeMap
// - Sets m_bRoguelikeState to true
// - Returns GetRoguelikeNextMap(true)
int CGocEntity::GetRoguelikeStartMap()
{
    ClearRoguelikeData();
    InitRoguelikeMap();
    m_bRoguelikeState = true;
    return GetRoguelikeNextMap(true);
}

// ============================================================================
// IDA: ?SetRoguelikeResult@CGocEntity@@QEAAXK_N@Z (0x1400659D0)
// Verified: Per IDA decompile - sets roguelike result for current map
// - Checks m_bRoguelikeState and index validity
// - Updates m_vecRoguelikeResult[currentIndex] with time, boss clear flag, step count
// - Logs to ST_LOG_GAME
void CGocEntity::SetRoguelikeResult(uint32_t dwTime, bool bClearBoss)
{
    // TODO: 汇编还原 - Need CUser, ST_LOG_GAME, XGameServer, XArea types
    // Per IDA:
    // - If m_bRoguelikeState and index valid:
    //   - Set result.nCount = dwTime
    //   - Set result.nID low byte = bClearBoss
    //   - Set result.nType = m_nRoguelikeStep
    //   - Get CUser via RTDynamicCast
    //   - If CUser exists:
    //     - Get instance ID
    //     - Create ST_LOG_GAME
    //     - Set UAID, UCID, MainType=28, SubType=26
    //     - Set nParam0 = map ID, nParam1 = time, nParam2 = boss clear, nParam3 = step
    //     - Set comment to "ROGUE_MAP_RESULT"
    //     - Send via XGameServer::SendDBLog

    (void)dwTime;
    (void)bClearBoss;
}

// ============================================================================
// IDA: ?IsLastRoguelike@CGocEntity@@QEAA_NXZ (0x140066300)
// Verified: Per IDA decompile - checks if at last roguelike map
// - Returns true if currentIndex + 1 >= map list size
bool CGocEntity::IsLastRoguelike() const
{
    return (m_nCurRoguelikeMapIndex + 1) >= m_vecRoguelikeMapList.size();
}

// ============================================================================
// IDA: ?GetRoguelikeRewardMoney@CGocEntity@@QEAAHH_N@Z (0x140066350)
// Verified: Per IDA decompile - calculates roguelike reward money
// - If nCount <= 0: returns 0
// - If not boss sector: returns nCount
// - For boss sector: adds nCount for each previous boss sector cleared
int CGocEntity::GetRoguelikeRewardMoney(int nCount, bool bBossSector)
{
    if (nCount <= 0) {
        return 0;
    }
    if (!bBossSector) {
        return nCount;
    }

    int nResult = nCount;
    int i = 0;
    for (const auto& result : m_vecRoguelikeResult) {
        if (i >= m_nCurRoguelikeMapIndex) {
            break;
        }
        // Check if this was a boss sector (nID low byte == 1)
        if ((result.nResult & 0xFF) == 1) {
            nResult += nCount;
        }
        ++i;
    }
    return nResult;
}

// ============================================================================
// IDA: ?InitRoguelikeMap@CGocEntity@@QEAAXXZ (0x140065830)
// Verified: Per IDA decompile - initializes roguelike map list
// - Gets roguelike map list from XResourceMgr::GetRoguelikeMap
// - Random shuffles the list
// - Takes first 4 maps and pushes to m_vecRoguelikeMapList
// - Initializes m_vecRoguelikeResult with map IDs
void CGocEntity::InitRoguelikeMap()
{
    // TODO: 汇编还原 - Need XGameServer, XResourceMgr, std::random_shuffle
    // Per IDA:
    // - Create temporary vector<unsigned long> vecMapList
    // - Call XResourceMgr::GetRoguelikeMap(&vecMapList)
    // - Call std::random_shuffle on vecMapList
    // - Take first 4 maps, push to m_vecRoguelikeMapList
    // - For each map in m_vecRoguelikeMapList:
    //   - Create ST_ROGUELIKE_RESULT with nMapID
    //   - Push to m_vecRoguelikeResult
    m_vecRoguelikeMapList.clear();
    m_nRoguelikeStep = 0;
    m_nCurRoguelikeMapIndex = 0;
}

// ============================================================================
// IDA: ?SendRoguelikeResult@CGocEntity@@QEAAX_N@Z (0x140065D20)
// Verified: Per IDA decompile - sends roguelike result to client
// - Collects results up to current map index
// - Gets reward from XResourceMgr::GetRoguelikeReward
// - Tries to add reward item to inventory, or sends via mail if full
// - Sends packet (main=0x33, sub=0x23) with results
// - Logs to ST_LOG_GAME
void CGocEntity::SendRoguelikeResult(bool bClear)
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Get CGocSkill component
    // - Create PS_ROGUELIKE_RESULT_INFOS
    // - Collect results from m_vecRoguelikeResult up to m_nCurRoguelikeMapIndex
    // - Set bClear flag
    // - Get current money from CGocSkill::GetModeShopMoney
    // - Get reward from XResourceMgr::GetRoguelikeReward
    // - If reward item exists:
    //   - Get TB_ITEM
    //   - Try CGocInventory::CreateItemReq
    //   - If fails: send via CGocPost::SystemPostSend
    // - Send packet (main=0x33, sub=0x23)
    // - Log to ST_LOG_GAME (main=28, sub=27)

    (void)bClear;
}

// ============================================================================
// IDA: ?SendRoguelikeCurrentInfo@CGocEntity@@QEAAXXZ (0x140066430)
// Verified: Per IDA decompile - sends current roguelike info to client
// - Creates PS_ROGUELIKE_CURRENT_INFO with map step and sector step
// - Sends packet (main=0x33, sub=0x24)
void CGocEntity::SendRoguelikeCurrentInfo()
{
    // TODO: 汇编还原 - Need XSendPacket, PS_ROGUELIKE_CURRENT_INFO types
    // Per IDA:
    // - Create PS_ROGUELIKE_CURRENT_INFO
    // - Set nMapStep = m_nCurRoguelikeMapIndex + 1
    // - Set nSectorStep = m_nRoguelikeStep + 1
    // - Create XSendPacket (main=0x33, sub=0x24)
    // - Send via CGocNetwork::Send
}

// ============================================================================
// IDA: ?AddTitleAll@CGocEntity@@QEAAXH@Z (0x140066530)
// Verified: Per IDA decompile - adds all titles to player
// - Iterates through all TB_TITLE_INFO entries
// - Checks class requirement
// - Adds titles not already owned
// - Sends DB packet with list of added titles
void CGocEntity::AddTitleAll(int nLogType)
{
    // TODO: 汇编还原 - Complex function needing many types
    // Per IDA:
    // - Get CGocAttribute component
    // - Create PS_TITLE_ADD_LIST
    // - Iterate through XGameServer's TB_TITLE_INFO map
    // - For each title:
    //   - Check Use_Class matches player class
    //   - If not owned: add to m_mapHaveTitle
    //   - Remove from m_setTitleOpen if present
    //   - Add to PS_TITLE_ADD_LIST
    // - Send DB packet (main=3, sub=0x91)

    (void)nLogType;
}

// ============================================================================
// IDA: ?IsRoguelikeState@CGocEntity@@QEAA_NXZ (0x1402C8130)
// Verified: Per IDA decompile - returns roguelike state flag
bool CGocEntity::IsRoguelikeState() const
{
    return m_bRoguelikeState;
}

// ============================================================================
// IDA: ?GetSGAuthType@CGocEntity@@QEAAEXZ (0x1402D3B40)
// Verified: Per IDA decompile - returns SG auth type
uint8_t CGocEntity::GetSGAuthType() const
{
    return m_stSGAuthInfo.byAuthType;
}

// ============================================================================
// IDA: ?IsRoguelikeFirstMap@CGocEntity@@QEAA_NXZ (0x140310390)
// Verified: Per IDA decompile - checks if at first roguelike map
bool CGocEntity::IsRoguelikeFirstMap() const
{
    return m_nCurRoguelikeMapIndex == 0;
}

// ============================================================================
// IDA: ?GetRoguelikeTotalStep@CGocEntity@@QEAAHXZ (0x140310400)
// Verified: Per IDA decompile - returns total roguelike steps
int CGocEntity::GetRoguelikeTotalStep() const
{
    return m_nRoguelikeTotalStep;
}

// ============================================================================
// IDA: ?GetRoguelikeMapToGoNext@CGocEntity@@QEAAHXZ (0x140310420)
// Verified: Per IDA decompile - returns next roguelike map ID
int CGocEntity::GetRoguelikeMapToGoNext() const
{
    return m_dwRoguelikeNextMap;
}

// ============================================================================
// IDA: ?IsRoguelikeMapToGoNext@CGocEntity@@QEAA_NXZ (0x140310440)
// Verified: Per IDA decompile - checks if there's a next map to go
bool CGocEntity::IsRoguelikeMapToGoNext() const
{
    return m_dwRoguelikeNextMap != 0;
}

// ============================================================================
// IDA: ?ResetRoguelikeNextMap@CGocEntity@@QEAAXXZ (0x140310470)
// Verified: Per IDA decompile - resets the next map ID to 0
void CGocEntity::ResetRoguelikeNextMap()
{
    m_dwRoguelikeNextMap = 0;
}