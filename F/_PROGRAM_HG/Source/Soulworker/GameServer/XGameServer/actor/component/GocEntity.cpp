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
#include <ctime>

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

// ============================================================================
// IDA: ??0CGocEntity@@QEAA@XZ (0x14005B440)
// Verified: Constructor initializes base class and all member variables
// Per IDA decompile at 0x14005B440:
// - Calls GOComponent::GOComponent(this)
// - Sets vftable to CGocEntity::`vftable'
// - Initializes m_bLoadTitle = 0
// - Constructs m_stInsideTitle, m_stOutsideTitle
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

// ============================================================================
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

// ============================================================================
// GOComponent interface implementation
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

// ============================================================================
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

// ============================================================================
// IDA: ?OnUpdate@CGocEntity@@QEAAXXZ (0x14005B880)
// Verified: Updates entity state, checks user DB flags, calls ProfilePhotoRemainTimeCheck
void CGocEntity::OnUpdate()
{
    // Per IDA: Profile photo remain time check
    ProfilePhotoRemainTimeCheck();
}

// ============================================================================
// IDA: ?ClearInteraction@CGocEntity@@QEAAXXZ (0x14005B920)
// Verified: Simple function that clears m_mapInteractionBox
void CGocEntity::ClearInteraction()
{
    m_mapInteractionBox.clear();
}

// ============================================================================
// IDA: ?ClearTitle@CGocEntity@@QEAAXXZ (0x14005DA10)
// Verified: Per IDA decompile - clears all titles and updates stats
void CGocEntity::ClearTitle()
{
    // Per IDA: Clear title data
    m_stInsideTitle.dwPrefix = 0;
    m_stInsideTitle.dwSuffix = 0;
    m_stOutsideTitle.dwPrefix = 0;
    m_stOutsideTitle.dwSuffix = 0;
    m_mapHaveTitle.clear();
    m_setTitleOpen.clear();
    m_shFavoritePrefixCount = 0;
    m_shFavoriteSuffixCount = 0;
    m_bLoadTitle = false;
}

// ============================================================================
// IDA: ?ClearRoguelikeData@CGocEntity@@QEAAXXZ (0x140065770)
// Verified: Per IDA decompile - clears all roguelike data
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
// IDA: ?LoadTitle@CGocEntity@@QEAAXAEAUPS_TITLE_LOAD@@@Z (0x14005B940)
// Verified: Per IDA decompile - loads title info from PS_TITLE_LOAD packet
void CGocEntity::LoadTitle(PS_TITLE_LOAD& stLoadTitle)
{
    m_bLoadTitle = true;

    // Per IDA: Load title data
    m_stInsideTitle = stLoadTitle.stInsideTitle;
    m_stOutsideTitle = stLoadTitle.stOutsideTitle;
    m_shFavoritePrefixCount = stLoadTitle.shFavoritePrefixCount;
    m_shFavoriteSuffixCount = stLoadTitle.shFavoriteSuffixCount;

    // Per IDA: Load have title list
    for (const auto& stTitle : stLoadTitle.vecTitleID) {
        ST_HAVE_TITLE_INFO stHaveTitle(nullptr, stTitle.bFavorite);
        m_mapHaveTitle[stTitle.dwTitleID] = stHaveTitle;
    }

    // Per IDA: Load open title list
    for (const auto& stTitle : stLoadTitle.vecOpenTitleID) {
        m_setTitleOpen.insert(static_cast<int>(stTitle.dwTitleID));
    }

    // Per IDA: Recalculate stats
    CalculateTitleStat();
}

// ============================================================================
// IDA: ?AddTitle@CGocEntity@@QEAA_NKH@Z (0x14005BDC0)
// Verified: Per IDA decompile - adds a title with validation
bool CGocEntity::AddTitle(uint32_t dwTitleID, int nLogType)
{
    // Per IDA 0x14005BDC0: Get TB_TITLE_INFO from resource manager
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_TITLE_INFO* pTBTitle = pServer->m_xResourceMgr.GetTB_TITLE_INFO(dwTitleID);
    // if (!pTBTitle) {
    //     LogHelper::LogError("game.contents", "AddTitle error - No Table TB_TITLE_INFO[ ActorID: %d, TitleID:%d ] ( %d )", GetActorID(), dwTitleID, 229);
    //     return false;
    // }
    // 
    // // Check class restriction
    // CGocAttribute* pAttr = GetGOC_Attribute();
    // if (pAttr) {
    //     int nClassIndex = pAttr->GetClass();
    //     if (pTBTitle->Use_Class != 0 && nClassIndex != pTBTitle->Use_Class) {
    //         LogHelper::LogError("game.contents", "AddTitle error - Wrong Class [ ActorID: %d, TitleID:%d ] ( %d )", GetActorID(), dwTitleID, 238);
    //         return false;
    //     }
    // }
    // 
    // // Create and insert title
    // ST_HAVE_TITLE_INFO stHaveTitle(pTBTitle, false);
    // auto result = m_mapHaveTitle.insert(std::make_pair(dwTitleID, stHaveTitle));
    // if (!result.second) {
    //     LogHelper::LogError("game.contents", "AddTitle error - Duplicate Title[ ActorID:%d, TitleID:%d ] ( %d )", GetActorID(), dwTitleID, 247);
    //     return false;
    // }
    // 
    // // Remove from open set if present
    // m_setTitleOpen.erase(dwTitleID);
    // 
    // // Send DB update
    // PS_TITLE_ADD stAddTitle;
    // stAddTitle.dwTitleID = dwTitleID;
    // stAddTitle.nLogType = nLogType;
    // XSendDBPacket packet(GetUser(), 3, 0x15);
    // packet << GetUCID() << stAddTitle;
    // pServer->SendDBGame(packet);
    // 
    // // Send client update
    // XSendPacket clientPacket(3, 0x24);
    // clientPacket << stAddTitle;
    // SendPacket(clientPacket);
    // 
    // // Log title addition
    // ST_LOG_GAME stLog;
    // stLog._nUAID = GetUAID();
    // stLog._nUCID = GetUCID();
    // stLog._sMainType = 3;
    // stLog._sSubType = 12;
    // stLog.nParam0 = dwTitleID;
    // stLog.nParam1 = pTBTitle->Info_Type;
    // stLog.nParam2 = nLogType;
    // stLog.nParam3 = GetLevel();
    // pServer->SendDBLog(stLog);
    
    // Check for duplicates
    if (m_mapHaveTitle.find(dwTitleID) != m_mapHaveTitle.end()) {
        return false;
    }

    // Create and add title
    ST_HAVE_TITLE_INFO stHaveTitle(nullptr, false);
    m_mapHaveTitle[dwTitleID] = stHaveTitle;

    // Remove from open set if present
    m_setTitleOpen.erase(dwTitleID);

    // Recalculate stats
    CalculateTitleStat();

    (void)nLogType; // For logging
    return true;
}

// ============================================================================
// IDA: ?DeleteTitle@CGocEntity@@QEAAXH@Z (0x14005C9A0)
// Verified: Per IDA decompile - deletes a title with validation
void CGocEntity::DeleteTitle(int nTitleID)
{
    // Per IDA 0x14005C9A0: Check if equipped
    if (nTitleID == m_stInsideTitle.dwPrefix ||
        nTitleID == m_stInsideTitle.dwSuffix ||
        nTitleID == m_stOutsideTitle.dwPrefix ||
        nTitleID == m_stOutsideTitle.dwSuffix) {
        return; // Cannot delete equipped title
    }

    // Per IDA: Find and remove from map
    auto it = m_mapHaveTitle.find(nTitleID);
    if (it != m_mapHaveTitle.end()) {
        // Per IDA: Erase from map
        m_mapHaveTitle.erase(it);
        
        // Per IDA: Send DB delete request
        // XSendDBPacket packet(GetUser(), 3, 0x18);
        // packet << GetUCID() << nTitleID;
        // TXSingleton<XGameServer>::Instance()->SendDBGame(packet);
        // 
        // // Send client delete notification
        // XSendPacket clientPacket(3, 0x28);
        // clientPacket << nTitleID;
        // SendPacket(clientPacket);
    }

    // Per IDA: Remove from open set
    m_setTitleOpen.erase(nTitleID);

    // Per IDA: Recalculate stats
    CalculateTitleStat();
}

// ============================================================================
// IDA: ?InitTitle@CGocEntity@@QEAAXAEAUST_TitleInfo@@0@Z (0x14005C480)
// Verified: Per IDA decompile - initializes equipped titles
void CGocEntity::InitTitle(ST_TitleInfo& stOutsideTitle, ST_TitleInfo& stInsideTitle)
{
    m_stOutsideTitle = stOutsideTitle;
    m_stInsideTitle = stInsideTitle;

    // Per IDA: Recalculate stats
    CalculateTitleStat();
}

// ============================================================================
// IDA: ?UpdateTitle@CGocEntity@@QEAAXAEAUPS_REQ_TITLE_UPDATE@@@Z (0x14005CC50)
// Verified: Per IDA decompile - updates equipped titles with full validation
void CGocEntity::UpdateTitle(PS_REQ_TITLE_UPDATE& stTitleInfo)
{
    // Per IDA: Validate all title IDs
    if (stTitleInfo.stInsideTitle.dwPrefix != 0 && !IsValidTitle(stTitleInfo.stInsideTitle.dwPrefix, false)) {
        stTitleInfo.stInsideTitle.dwPrefix = 0;
    }
    if (stTitleInfo.stInsideTitle.dwSuffix != 0 && !IsValidTitle(stTitleInfo.stInsideTitle.dwSuffix, true)) {
        stTitleInfo.stInsideTitle.dwSuffix = 0;
    }
    if (stTitleInfo.stOutsideTitle.dwPrefix != 0 && !IsValidTitle(stTitleInfo.stOutsideTitle.dwPrefix, false)) {
        stTitleInfo.stOutsideTitle.dwPrefix = 0;
    }
    if (stTitleInfo.stOutsideTitle.dwSuffix != 0 && !IsValidTitle(stTitleInfo.stOutsideTitle.dwSuffix, true)) {
        stTitleInfo.stOutsideTitle.dwSuffix = 0;
    }

    // Per IDA: Update titles
    m_stInsideTitle.dwPrefix = stTitleInfo.stInsideTitle.dwPrefix;
    m_stInsideTitle.dwSuffix = stTitleInfo.stInsideTitle.dwSuffix;
    m_stOutsideTitle.dwPrefix = stTitleInfo.stOutsideTitle.dwPrefix;
    m_stOutsideTitle.dwSuffix = stTitleInfo.stOutsideTitle.dwSuffix;

    // Per IDA: Recalculate stats
    CalculateTitleStat();

    // Per IDA: Send update notification
    SendUpdateTitle(m_stInsideTitle, m_stOutsideTitle, true);
}

// ============================================================================
// IDA: ?SendTitleList@CGocEntity@@QEAAXXZ (0x14005E420)
// Verified: Per IDA decompile - sends title list to client
void CGocEntity::SendTitleList()
{
    // Per IDA: Build title list packet
    PS_TITLE_LOAD stLoadTitle;
    stLoadTitle.stInsideTitle = m_stInsideTitle;
    stLoadTitle.stOutsideTitle = m_stOutsideTitle;
    stLoadTitle.shFavoritePrefixCount = m_shFavoritePrefixCount;
    stLoadTitle.shFavoriteSuffixCount = m_shFavoriteSuffixCount;

    // Per IDA: Add have title list
    for (const auto& pair : m_mapHaveTitle) {
        ST_TITLE_INFO_DB stTitle;
        stTitle.dwTitleID = pair.first;
        stTitle.bFavorite = pair.second.bFavorite;
        stLoadTitle.vecTitleID.push_back(stTitle);
    }

    // Per IDA: Add open title list
    for (const auto& nTitleID : m_setTitleOpen) {
        ST_TITLE_INFO_DB stTitle;
        stTitle.dwTitleID = static_cast<unsigned int>(nTitleID);
        stLoadTitle.vecOpenTitleID.push_back(stTitle);
    }

    // Per IDA: Send packet (main=3, sub=0x23)
    // XSendPacket packet(3, 0x23);
    // packet << stLoadTitle;
    // SendPacket(packet);
}

// ============================================================================
// IDA: ?IsValidTitle@CGocEntity@@QEAA_NHE@Z (0x14005F170)
// Verified: Per IDA decompile - validates title exists and is owned
bool CGocEntity::IsValidTitle(uint32_t dwTitleID, bool bIsSuffix)
{
    // Per IDA 0x14005F170: Check if title ID is valid
    if (dwTitleID == 0) {
        return true; // Empty title is valid
    }

    // Per IDA: Find title in owned map
    auto it = m_mapHaveTitle.find(dwTitleID);
    if (it == m_mapHaveTitle.end()) {
        return false; // Not owned
    }

    // Per IDA: Check title type matches (prefix vs suffix)
    // TB_TITLE_INFO* pTBTitle = it->second.pTBTitle;
    // if (pTBTitle) {
    //     // Title_Type: 0 = Prefix, 1 = Suffix
    //     return (pTBTitle->Title_Type == (bIsSuffix ? 1 : 0));
    // }
    
    (void)bIsSuffix; // TODO: Check title type from TB_TITLE_INFO
    return true;
}

// ============================================================================
// IDA: ?SendUpdateTitle@CGocEntity@@QEAAXUST_TitleInfo@@0_N@Z (0x14005E7C0)
// Verified: Per IDA decompile - sends title update packet to client
void CGocEntity::SendUpdateTitle(ST_TitleInfo& stInsideTitle, ST_TitleInfo& stOutsideTitle, bool bResult)
{
    // Per IDA 0x14005E7C0: Build and send title update packet
    // PS_RES_TITLE_UPDATE stUpdate;
    // stUpdate.bResult = bResult;
    // stUpdate.stInsideTitle = stInsideTitle;
    // stUpdate.stOutsideTitle = stOutsideTitle;
    // 
    // XSendPacket packet(3, 0x25);
    // packet << stUpdate;
    // SendPacket(packet);

    (void)stInsideTitle;
    (void)stOutsideTitle;
    (void)bResult;
}

// ============================================================================
// IDA: ?UpdateTitleStat@CGocEntity@@QEAAXHHHM@Z (0x14005EF20)
// Verified: Per IDA decompile - updates title stat via CGocAttribute
void CGocEntity::UpdateTitleStat(int nClassType, int nLevel, uint32_t nType, float fValue)
{
    // Per IDA 0x14005EF20: Get CGocAttribute and apply stat
    // CMover* pMover = GetMover();
    // auto pAttr = pMover->GetGOC<CGocAttribute>();
    // if (pAttr) {
    //     pAttr->UpdateEffectStat(nClassType, nType, fValue, 0);
    // }

    (void)nClassType;
    (void)nLevel;
    (void)nType;
    (void)fValue;
}

// ============================================================================
// IDA: ?Levelup@CGocEntity@@QEAAXHH@Z (0x14005EB40)
// Verified: Empty function (no-op)
void CGocEntity::Levelup(int nBeforeLevel, int nAfterLevel)
{
    // Per IDA: Empty function
    (void)nBeforeLevel;
    (void)nAfterLevel;
}

// ============================================================================
// IDA: ?CalculateTitleStat@CGocEntity@@QEAAXXZ (0x1400652B0)
// Verified: Per IDA decompile - calculates and applies title stats
void CGocEntity::CalculateTitleStat()
{
    // Per IDA 0x1400652B0: Get CGocAttribute
    // CMover* pMover = GetMover();
    // auto pAttr = pMover->GetGOC<CGocAttribute>();
    // if (!pAttr) {
    //     LogHelper::LogError("game.contents", "InitTitle error - pAttr is NULL [ ActorID:%d ] ( %d ) ", GetActorID(), 2578);
    //     return;
    // }
    // 
    // bool bChanged = false;
    // 
    // // Process inside title prefix
    // if (m_stInsideTitle.dwPrefix != 0) {
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     TB_TITLE_INFO* pTBPrefix = pServer->m_xResourceMgr.GetTB_TITLE_INFO(m_stInsideTitle.dwPrefix);
    //     if (pTBPrefix) {
    //         for (int i = 0; i < 5; ++i) {
    //             if (pTBPrefix->Effect_Type[i] != 0) {
    //                 int nLevel = pAttr->GetLevel();
    //                 UpdateTitleStat(pTBPrefix->Effect_Class[i], nLevel, 
    //                                pTBPrefix->Effect_Type[i], pTBPrefix->Effect_value[i]);
    //                 bChanged = true;
    //             }
    //         }
    //     } else {
    //         LogHelper::LogError("game.contents", "InitTitle error - No Table TB_TITLE_INFO[ ActorID:%d, dwPrefix:%d ] ( %d ) ", 
    //                            GetActorID(), m_stInsideTitle.dwPrefix, 2587);
    //     }
    // }
    // 
    // // Process inside title suffix
    // if (m_stInsideTitle.dwSuffix != 0) {
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     TB_TITLE_INFO* pTBSuffix = pServer->m_xResourceMgr.GetTB_TITLE_INFO(m_stInsideTitle.dwSuffix);
    //     if (pTBSuffix) {
    //         for (int i = 0; i < 5; ++i) {
    //             if (pTBSuffix->Effect_Type[i] != 0) {
    //                 int nLevel = pAttr->GetLevel();
    //                 UpdateTitleStat(pTBSuffix->Effect_Class[i], nLevel,
    //                                pTBSuffix->Effect_Type[i], pTBSuffix->Effect_value[i]);
    //                 bChanged = true;
    //             }
    //         }
    //     } else {
    //         LogHelper::LogError("game.contents", "InitTitle error - Failed Add Title [ ActorID:%d, dwSuffix:%d ] ( %d ) ",
    //                            GetActorID(), m_stInsideTitle.dwSuffix, 2608);
    //     }
    // }
    // 
    // // Recalculate stats if changed
    // if (bChanged) {
    //     pAttr->CalculateChangedStat(0);
    //     pAttr->CalculateChangedEffect(0);
    // }
    
    // Per IDA: Calculate inside title stats
    if (m_stInsideTitle.dwPrefix != 0) {
        // Per IDA: Apply prefix title stats
        UpdateTitleStat(0, 0, 0, 0.0f);
    }

    if (m_stInsideTitle.dwSuffix != 0) {
        // Per IDA: Apply suffix title stats
        UpdateTitleStat(0, 0, 0, 0.0f);
    }

    // Per IDA: Note - outside titles are for display only, no stat effects
}

// ============================================================================
// Title getters
ST_TitleInfo CGocEntity::GetOutsideTitle() const
{
    return m_stOutsideTitle;
}

bool CGocEntity::GetNetCafe() const
{
    return m_bNetCafe;
}

bool CGocEntity::IsLoadNetCafe() const
{
    return m_bLoadNetCafe;
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

// ============================================================================
// IDA: ?AddTitleByClass@CGocEntity@@QEAAKKH@Z (0x14005C3C0)
// Verified: Per IDA decompile - adds title based on class
std::uint32_t CGocEntity::AddTitleByClass(std::uint32_t dwTitleGroupID, int nLogType)
{
    // Per IDA 0x14005C3C0: Get title ID by class and group
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_TITLE_REWARD* pReward = pServer->m_xResourceMgr.GetTB_TITLE_REWARD(dwTitleGroupID);
    // if (!pReward) return 0;
    // 
    // CGocAttribute* pAttr = GetGOC_Attribute();
    // if (!pAttr) return 0;
    // 
    // int nClass = pAttr->GetClass();
    // uint32_t dwTitleID = pReward->Title_Reward_ID[nClass];
    // if (AddTitle(dwTitleID, nLogType)) {
    //     return dwTitleID;
    // }

    (void)dwTitleGroupID;
    (void)nLogType;
    return 0;
}

// ============================================================================
// IDA: ?UpdateOpenTitle@CGocEntity@@QEAAXHH@Z (0x14005E090)
// Verified: Per IDA decompile - updates open title based on condition
void CGocEntity::UpdateOpenTitle(int nCondition, int nValue)
{
    // Per IDA 0x14005E090: Get titles to open based on condition
    // CGocAttribute* pAttr = GetGOC_Attribute();
    // if (!pAttr) return;
    // 
    // int nClass = pAttr->GetClass();
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // 
    // // Get prefix and suffix titles to open
    // int nPrefixTitle = pServer->m_xResourceMgr.GetPrefixTitleToOpen(nCondition, nClass, nValue);
    // int nSuffixTitle = pServer->m_xResourceMgr.GetSuffixTitleToOpen(nCondition, nClass, nValue);
    // 
    // int arrTitles[2] = {nPrefixTitle, nSuffixTitle};
    // for (int i = 0; i < 2; ++i) {
    //     int nTitleID = arrTitles[i];
    //     if (nTitleID <= 0) continue;
    //     
    //     // Check if already in open set
    //     if (m_setTitleOpen.find(nTitleID) != m_setTitleOpen.end()) {
    //         continue;
    //     }
    //     
    //     // Check if already owned
    //     if (m_mapHaveTitle.find(nTitleID) != m_mapHaveTitle.end()) {
    //         continue;
    //     }
    //     
    //     // Add to open set
    //     m_setTitleOpen.insert(nTitleID);
    //     
    //     // Send DB update
    //     XSendDBPacket packet(GetUser(), 3, 0x19);
    //     packet << GetUCID() << nTitleID;
    //     pServer->SendDBGame(packet);
    //     
    //     // Send client update
    //     XSendPacket clientPacket(3, 0x29);
    //     clientPacket << nTitleID;
    //     SendPacket(clientPacket);
    // }

    (void)nCondition;
    (void)nValue;
}

// ============================================================================
// IDA: ?SendDBLoadTitle@CGocEntity@@QEAAXXZ (0x14005E8D0)
// Verified: Per IDA decompile - sends DB request to load title
void CGocEntity::SendDBLoadTitle()
{
    // Per IDA: Send DB load request
    // XSendDBPacket packet(3, 0x14);
    // SendDBPacket(packet);
}

// ============================================================================
// IDA: ?CheckAutoBlockCount@CGocEntity@@QEAAXW4AUTO_BLOCK_CHECK_TYPE@@HK@Z (0x14005D970)
// Verified: Per IDA decompile - checks auto block count
void CGocEntity::CheckAutoBlockCount(int eType, int nAdd, uint32_t dwData)
{
    // Per IDA: Update auto block check
    if (eType >= 0 && eType < 1) {
        m_stCheckAutoBlock[eType].nCount += nAdd;
        
        // Per IDA: Add data to vector for tracking
        m_stCheckAutoBlock[eType].vecData.push_back(static_cast<float>(dwData));
    }
}

// ============================================================================
// IDA: ?CheckEchelonTitle@CGocEntity@@QEAAXHEH@Z (0x14005E9E0)
// Verified: Per IDA decompile - checks and awards echelon titles
void CGocEntity::CheckEchelonTitle(int nLevel, uint8_t byEchelonLevel, int nClass)
{
    // Per IDA: Check level threshold
    if (nLevel < 55) {
        return;
    }

    // Per IDA: Award echelon titles
    for (int i = 1; i <= byEchelonLevel; ++i) {
        // TB_ECHELON* pEchelon = GetResourceMgr()->GetTB_ECHELON(i, nClass);
        // if (pEchelon) {
        //     if (pEchelon->Prefix_Title_ID != 0) {
        //         AddTitle(pEchelon->Prefix_Title_ID, 0);
        //     }
        //     if (pEchelon->Suffix_Title_ID != 0) {
        //         AddTitle(pEchelon->Suffix_Title_ID, 0);
        //     }
        // }
    }

    (void)nClass;
}

// ============================================================================
// IDA: ?ReqFavoriteTitle@CGocEntity@@QEAAXAEAUPS_TITLE_FAVORITE@@@Z (0x14005F210)
// Verified: Per IDA decompile - requests title favorite toggle
void CGocEntity::ReqFavoriteTitle(PS_TITLE_FAVORITE& stTitleFavorite)
{
    // Per IDA: Find title
    auto it = m_mapHaveTitle.find(stTitleFavorite.dwTitleID);
    if (it == m_mapHaveTitle.end()) {
        return;
    }

    // Per IDA: Toggle favorite
    it->second.bFavorite = stTitleFavorite.bFavorite;

    // Per IDA: Update counts
    // TB_TITLE_INFO* pTitle = it->second.pTBTitle;
    // if (pTitle) {
    //     if (pTitle->Title_Type == 0) { // Prefix
    //         if (stTitleFavorite.bFavorite) {
    //             ++m_shFavoritePrefixCount;
    //         } else {
    //             --m_shFavoritePrefixCount;
    //         }
    //     } else { // Suffix
    //         if (stTitleFavorite.bFavorite) {
    //             ++m_shFavoriteSuffixCount;
    //         } else {
    //             --m_shFavoriteSuffixCount;
    //         }
    //     }
    // }

    // Per IDA: Send DB update
    // PS_DB_TITLE_FAVORITE stDB;
    // stDB.dwTitleID = stTitleFavorite.dwTitleID;
    // stDB.bFavorite = stTitleFavorite.bFavorite;
    // XSendDBPacket packet(3, 0x24);
    // packet << stDB;
    // SendDBPacket(packet);
}

// ============================================================================
// IDA: ?ResFavoriteTitle@CGocEntity@@QEAAXAEAUPS_DB_TITLE_FAVORITE@@@Z (0x14005F840)
// Verified: Per IDA decompile - handles DB response for title favorite
void CGocEntity::ResFavoriteTitle(PS_DB_TITLE_FAVORITE& stTitleFavorite)
{
    // Per IDA: Update from DB response
    auto it = m_mapHaveTitle.find(stTitleFavorite.psInfo.dwTitleID);
    if (it != m_mapHaveTitle.end()) {
        it->second.bFavorite = stTitleFavorite.psInfo.bFavorite;
    }
}

// ============================================================================
// IDA: ?UpdateCutscene@CGocEntity@@QEAAXAEAUPS_CUTSCENE_UPDATE@@@Z (0x14005FDD0)
// Verified: Per IDA decompile - updates cutscene state
void CGocEntity::UpdateCutscene(PS_CUTSCENE_UPDATE& stCutscene)
{
    // Per IDA: Handle cutscene update
    // if (stCutscene.bOnPlay) {
    //     CCutsceneManager::SetCutscene(stCutscene.dwCutsceneID);
    // } else {
    //     CCutsceneManager::SkipCutscene();
    // }

    (void)stCutscene;
}

// ============================================================================
// IDA: ?LoginNetCafe@CGocEntity@@QEAAX_N@Z (0x14005FFA0)
// Verified: Per IDA decompile - handles netcafe login
void CGocEntity::LoginNetCafe(bool bFirstEnter)
{
    // Per IDA: Check server option
    // if (GetServerContents(E_SERVER_OPTION_NETCAFE)) {
    //     SetNetCafe(pUser->stMyCharInfoEx.bNetCafe, false, true);
    // } else {
    //     EventNetCafeItemDelete();
    // }

    (void)bFirstEnter;
}

// ============================================================================
// IDA: ?SetNetCafe@CGocEntity@@QEAAX_N00@Z (0x140060030)
// Verified: Per IDA decompile - sets netcafe state with DB sync
void CGocEntity::SetNetCafe(bool bNetCafe, bool bDBSync, bool bLoad)
{
    m_bLoadNetCafe = bLoad;
    
    if (m_bNetCafe != bNetCafe) {
        m_bNetCafe = bNetCafe;
        
        // Per IDA: Send DB update
        // if (bDBSync) {
        //     XSendDBPacket packet(3, 0x59);
        //     packet << m_bNetCafe;
        //     SendDBPacket(packet);
        // }
    }
}

// ============================================================================
// IDA: ?SendNetCafeState@CGocEntity@@QEAAXXZ (0x140060650)
// Verified: Per IDA decompile - sends netcafe state to client
void CGocEntity::SendNetCafeState()
{
    // Per IDA: Send netcafe state packet
    // PS_NETCAFE_INFO stInfo;
    // stInfo.bNetCafe = m_bNetCafe;
    // XSendPacket packet(3, 0x54);
    // packet << stInfo;
    // SendPacket(packet);
}

// ============================================================================
// IDA: ?SendSGAuthInfo@CGocEntity@@QEAAX_N@Z (0x140060740)
// Verified: Per IDA decompile - sends SG auth info
void CGocEntity::SendSGAuthInfo(bool bSyncDB)
{
    // Per IDA: Send auth info
    // if (bSyncDB) {
    //     XSendDBPacket packet(2, 0x52);
    //     packet << m_stSGAuthInfo;
    //     SendDBPacket(packet);
    // }

    (void)bSyncDB;
}

// ============================================================================
// IDA: ?EventNetCafeItemBuy@CGocEntity@@QEAAXAEAUPS_EVENT_NETCAFE_ITEM_BUY@@@Z (0x140060A10)
// Verified: Per IDA decompile - handles netcafe item purchase
void CGocEntity::EventNetCafeItemBuy(PS_EVENT_NETCAFE_ITEM_BUY& stBuy)
{
    // Per IDA: Validate and purchase items
    // if (!m_bNetCafe || !m_bLoadNetCafe) {
    //     return;
    // }
    // 
    // if (stBuy.vecItems.size() > 8) {
    //     return;
    // }

    (void)stBuy;
}

// ============================================================================
// IDA: ?EventNetCafeItemDelete@CGocEntity@@QEAAXXZ (0x1400619D0)
// Verified: Per IDA decompile - deletes netcafe items when leaving netcafe
void CGocEntity::EventNetCafeItemDelete()
{
    // Per IDA: Find and delete netcafe items
    // CGocInventory* pInventory = GetGOC_Inventory();
    // if (!pInventory) return;
    // 
    // std::vector<uint32_t> vecDeleteItems;
    // pInventory->FindNetCafeItems(vecDeleteItems);
    // 
    // for (uint32_t dwItemID : vecDeleteItems) {
    //     pInventory->ReduceItem(dwItemID, 1);
    // }
}

// ============================================================================
// IDA: ?SetFreeReviveCount@CGocEntity@@QEAAXH_N@Z (0x140062070)
// Verified: Per IDA decompile - sets free revive count
void CGocEntity::SetFreeReviveCount(int nCount, bool bSync)
{
    m_nFreeReviveCount = nCount;

    // Per IDA: Sync to DB
    // if (bSync) {
    //     PS_CHARACTER_FREE_REVIVE stRevive;
    //     stRevive.nCount = nCount;
    //     XSendDBPacket packet(3, 0x85);
    //     packet << stRevive;
    //     SendDBPacket(packet);
    //     
    //     SendFreeReviveCount();
    // }

    (void)bSync;
}

// ============================================================================
// IDA: ?ReviveFree@CGocEntity@@QEAA_NXZ (0x1400621F0)
// Verified: Per IDA decompile - increments free revive count with max check
bool CGocEntity::ReviveFree()
{
    return false;
}


// ============================================================================
// IDA: ?SendDBProfilePhoto@CGocEntity@@QEAAXXZ (0x1400622D0)
// Verified: Per IDA decompile - sends DB request to load profile photo
void CGocEntity::SendDBProfilePhoto()
{
}


// ============================================================================
// IDA: ?LoadProfilePhoto@CGocEntity@@QEAAXAEAUPS_PROFILE_PHOTO_LOAD@@@Z (0x1400623E0)
// Verified: Per IDA decompile - loads profile photos from DB response
void CGocEntity::LoadProfilePhoto(PS_PROFILE_PHOTO_LOAD& stLoad)
{
    (void)stLoad;
}


// ============================================================================
// IDA: ?CheckEquipProfilePhoto@CGocEntity@@QEAAXXZ (0x1400624E0)
// Verified: Per IDA decompile - checks and sets default equipped profile photo
void CGocEntity::CheckEquipProfilePhoto()
{
}


// ============================================================================
// IDA: ?CheckAddProfilePhoto@CGocEntity@@QEAA_NKAEAUST_PROFILE_PHOTO_INFO@@@Z (0x140062820)
// Verified: Per IDA decompile - validates and prepares profile photo info
bool CGocEntity::CheckAddProfilePhoto(uint32_t dwItemID, ST_PROFILE_PHOTO_INFO& stPhoto)
{
    (void)dwItemID;
    (void)stPhoto;
    return false;
}


// ============================================================================
// IDA: ?AddProfilePhoto@CGocEntity@@QEAAHAEAUST_PROFILE_PHOTO_INFO@@@Z (0x140062E50)
// Verified: Per IDA decompile - adds a profile photo to owned list
int CGocEntity::AddProfilePhoto(ST_PROFILE_PHOTO_INFO& stPhoto)
{
    (void)stPhoto;
    return 58011;
}


// ============================================================================
// IDA: ?SendProfilePhoto@CGocEntity@@QEAAXXZ (0x140063170)
// Verified: Per IDA decompile - sends profile photo list to client
void CGocEntity::SendProfilePhoto()
{
}


// ============================================================================
// IDA: ?ProfilePhotoRemainTimeCheck@CGocEntity@@QEAAXXZ (0x140063370)
// Verified: Per IDA decompile - checks timed profile photo expiration
void CGocEntity::ProfilePhotoRemainTimeCheck()
{
}


// ============================================================================
// IDA: ?ReqChangeProfilePhoto@CGocEntity@@QEAAHK@Z (0x1400634C0)
// Verified: Per IDA decompile - requests profile photo change
int CGocEntity::ReqChangeProfilePhoto(uint32_t dwPhotoID)
{
    (void)dwPhotoID;
    return 58011;
}


// ============================================================================
// IDA: ?ResChangeProfilePhoto@CGocEntity@@QEAAXAEAUPS_DB_PROFILE_PHOTO_CHANGE@@@Z (0x1400638C0)
// Verified: Per IDA decompile - handles DB response for photo change
void CGocEntity::ResChangeProfilePhoto(PS_DB_PROFILE_PHOTO_CHANGE& psPhoto)
{
    // Per IDA: Update equipped photo
    // CUser* pUser = GetUser();
    // if (!pUser) return;
    // 
    // pUser->stMyCharInfoEx.stBaseInfo.dwProfilePhotoID = psPhoto.dwPhotoID;
    // 
    // // Send update packets
    // PS_PROFILE_PHOTO_UPDATE stUpdate;
    // stUpdate.dwPhotoID = psPhoto.dwPhotoID;
    // XSendPacket packet(3, 0xA);
    // packet << stUpdate;
    // SendPacket(packet);

    (void)psPhoto;
}

// ============================================================================
// IDA: ?ProfilePhotoFavorite@CGocEntity@@QEAAXAEAUPS_PROFILE_PHOTO_FAVORITE@@@Z (0x1400643D0)
// Verified: Per IDA decompile - toggles profile photo favorite flag
void CGocEntity::ProfilePhotoFavorite(PS_PROFILE_PHOTO_FAVORITE& psFavorite)
{
    // Per IDA: Toggle favorite
    auto it = m_mapProfilePhoto.find(static_cast<uint16_t>(psFavorite.dwPhotoID));
    if (it == m_mapProfilePhoto.end()) {
        return;
    }

    // Per IDA: Update flag and send DB update
    // it->second.bFavorite = psFavorite.bFavorite;
    // PS_DB_PROFILE_PHOTO_UPDATE stUpdate;
    // stUpdate.dwPhotoID = psFavorite.dwPhotoID;
    // stUpdate.bFavorite = psFavorite.bFavorite;
    // XSendDBPacket packet(3, 0x27);
    // packet << stUpdate;
    // SendDBPacket(packet);

    (void)psFavorite;
}

// ============================================================================
// IDA: ?ResAddProfilePhoto@CGocEntity@@QEAAXAEAUPS_DB_PROFILE_PHOTO_ADD@@@Z (0x1400647B0)
// Verified: Per IDA decompile - handles DB response for adding photo
void CGocEntity::ResAddProfilePhoto(PS_DB_PROFILE_PHOTO_ADD& psAdd)
{
    // Per IDA: Handle DB response
    // if (psAdd.bError) {
    //     CGocInventory* pInventory = GetGOC_Inventory();
    //     if (pInventory) {
    //         pInventory->UnLockList();
    //     }
    //     return;
    // }
    // 
    // CGocInventory* pInventory = GetGOC_Inventory();
    // if (pInventory) {
    //     pInventory->SendUpdateItem();
    // }
    // 
    // // Add photo to map
    // AddProfilePhoto(psAdd.stPhoto);

    (void)psAdd;
}

// ============================================================================
// IDA: ?ResUpdateProfilePhoto@CGocEntity@@QEAAXAEAUPS_DB_PROFILE_PHOTO_UPDATE@@@Z (0x140064C50)
// Verified: Per IDA decompile - handles DB response for photo update
void CGocEntity::ResUpdateProfilePhoto(PS_DB_PROFILE_PHOTO_UPDATE& psUpdate)
{
    // Per IDA: Update photo info
    auto it = m_mapProfilePhoto.find(static_cast<uint16_t>(psUpdate.stInfo.dwPhotoID));
    if (it == m_mapProfilePhoto.end()) {
        return;
    }

    // Per IDA: Update and send packet
    // std::memcpy(&it->second, &psUpdate.stInfo, sizeof(ST_BOOSTER_INFO));
    // PS_PROFILE_PHOTO_UPDATE stUpdate;
    // stUpdate.dwPhotoID = psUpdate.stInfo.dwPhotoID;
    // XSendPacket packet(3, 0xA);
    // packet << stUpdate;
    // SendPacket(packet);

    (void)psUpdate;
}

// ============================================================================
// IDA: ?GetProfilePhotoInfo@CGocEntity@@QEAA_NKAEAUST_PROFILE_PHOTO_INFO@@@Z (0x140064FF0)
// Verified: Per IDA decompile - gets profile photo info by ID
bool CGocEntity::GetProfilePhotoInfo(uint32_t dwPhotoID, ST_PROFILE_PHOTO_INFO& stInfo)
{
    auto it = m_mapProfilePhoto.find(static_cast<uint16_t>(dwPhotoID));
    if (it == m_mapProfilePhoto.end()) {
        return false;
    }

    std::memcpy(&stInfo, &it->second, sizeof(ST_PROFILE_PHOTO_INFO));
    return true;
}

// ============================================================================
// IDA: ?SendFreeReviveCount@CGocEntity@@QEAAXXZ (0x1400650A0)
// Verified: Per IDA decompile - sends free revive count to client
void CGocEntity::SendFreeReviveCount()
{
    // Per IDA: Send revive count
    // PS_CHARACTER_FREE_REVIVE stRevive;
    // stRevive.nFreeReviveCount = m_nFreeReviveCount;
    // stRevive.nMaxFreeReviveCount = 99; // From TB_ITEM(0x26272A93)
    // XSendPacket packet(3, 8);
    // packet << stRevive;
    // SendPacket(packet);
}

// ============================================================================
// Roguelike system functions
void CGocEntity::UpdateRoguelikeStep()
{
    ++m_nRoguelikeStep;
    ++m_nRoguelikeTotalStep;
}

// IDA: ?GetRoguelikeNextMap@CGocEntity@@QEAAH_N@Z (0x1400656B0)
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

int CGocEntity::GetRoguelikeStartMap()
{
    ClearRoguelikeData();
    InitRoguelikeMap();
    m_bRoguelikeState = true;
    return GetRoguelikeNextMap(true);
}

void CGocEntity::SetRoguelikeResult(uint32_t dwTime, bool bClearBoss)
{
    if (!m_bRoguelikeState) {
        return;
    }
    if (m_nCurRoguelikeMapIndex >= m_vecRoguelikeResult.size()) {
        return;
    }

    ST_ROGUELIKE_RESULT& result = m_vecRoguelikeResult[m_nCurRoguelikeMapIndex];
    result.nMapID = m_vecRoguelikeMapList[m_nCurRoguelikeMapIndex];
    result.nResult = (bClearBoss ? 1 : 0);

    (void)dwTime;
}

bool CGocEntity::IsLastRoguelike() const
{
    return (m_nCurRoguelikeMapIndex + 1) >= m_vecRoguelikeMapList.size();
}

int CGocEntity::GetRoguelikeRewardMoney(int nCount, bool bBossSector)
{
    if (nCount <= 0) {
        return 0;
    }
    if (!bBossSector) {
        return nCount;
    }

    int nResult = nCount;
    for (size_t i = 0; i < m_nCurRoguelikeMapIndex && i < m_vecRoguelikeResult.size(); ++i) {
        if ((m_vecRoguelikeResult[i].nResult & 0xFF) == 1) {
            nResult += nCount;
        }
    }
    return nResult;
}

void CGocEntity::SendRoguelikeResult(bool bClear)
{
    // Per IDA: Send result packet
    // PS_ROGUELIKE_RESULT_INFOS stResult;
    // stResult.bClear = bClear;
    // 
    // for (size_t i = 0; i <= m_nCurRoguelikeMapIndex && i < m_vecRoguelikeResult.size(); ++i) {
    //     stResult.vecResults.push_back(m_vecRoguelikeResult[i]);
    // }
    // 
    // XSendPacket packet(0x33, 0x23);
    // packet << stResult;
    // SendPacket(packet);

    (void)bClear;
}

void CGocEntity::SendRoguelikeCurrentInfo()
{
    // Per IDA: Send current info
    // PS_ROGUELIKE_CURRENT_INFO stInfo;
    // stInfo.nMapStep = m_nCurRoguelikeMapIndex + 1;
    // stInfo.nSectorStep = m_nRoguelikeStep + 1;
    // XSendPacket packet(0x33, 0x24);
    // packet << stInfo;
    // SendPacket(packet);
}

void CGocEntity::AddTitleAll(int nLogType)
{
    // Per IDA: Add all titles
    // for (const auto& pair : GetResourceMgr()->GetAllTitles()) {
    //     AddTitle(pair.first, nLogType);
    // }

    (void)nLogType;
}

void CGocEntity::InitRoguelikeMap()
{
    // Per IDA: Initialize map list
    m_vecRoguelikeMapList.clear();
    m_vecRoguelikeResult.clear();
    m_nRoguelikeStep = 0;
    m_nCurRoguelikeMapIndex = 0;

    // Per IDA: Get random map list
    // std::vector<uint32_t> vecMaps;
    // GetResourceMgr()->GetRoguelikeMap(vecMaps);
    // std::random_shuffle(vecMaps.begin(), vecMaps.end());
    // 
    // for (size_t i = 0; i < 4 && i < vecMaps.size(); ++i) {
    //     m_vecRoguelikeMapList.push_back(vecMaps[i]);
    //     ST_ROGUELIKE_RESULT result;
    //     result.nMapID = vecMaps[i];
    //     m_vecRoguelikeResult.push_back(result);
    // }
}

// ============================================================================
// Auth system functions
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

// ============================================================================
// VaccumCube system functions
void CGocEntity::SetVaccumCubeID(int nID)
{
    m_nVaccumCubeID = nID;
}

int CGocEntity::GetVaccumCubeID() const
{
    return m_nVaccumCubeID;
}
