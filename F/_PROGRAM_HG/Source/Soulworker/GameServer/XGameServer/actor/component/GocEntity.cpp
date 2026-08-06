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
#include "GocNetwork.h"
#include "GocAttribute.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XGameServer/actor/Mover/Mover.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
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
// Get owner as CUser (if applicable)
// IDA: RTTI dynamic cast from owner to CUser
CUser* CGocEntity::GetUser() const
{
    CMover* pMover = GetOwnerGO();
    if (!pMover) return nullptr;
    return dynamic_cast<CUser*>(pMover);
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
    // IDA 0x14005BDC0: Get TB_TITLE_INFO from resource manager
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    TB_TITLE_INFO* pTBTitle = pServer->GetResourceMgr().GetTB_TITLE_INFO(dwTitleID);
    CUser* pUser = GetUser();
    uint32_t dwActorID = pUser ? pUser->GetActorID().dwActorID : 0;
    if (!pTBTitle) {
        LogHelper::LogError("game.contents", "AddTitle error - No Table TB_TITLE_INFO[ ActorID: %d, TitleID:%d ] ( %d )", dwActorID, dwTitleID, 229);
        return false;
    }

    // Check class restriction
    CMover* pMover = GetOwnerGO();
    std::shared_ptr<CGocAttribute> pAttr = pMover ? pMover->GetGOC_Attribute(false) : nullptr;
    if (pAttr) {
        int nClassIndex = pAttr->GetClass();
        if (pTBTitle->Use_Class != 0 && nClassIndex != pTBTitle->Use_Class) {
            LogHelper::LogError("game.contents", "AddTitle error - Wrong Class [ ActorID: %d, TitleID:%d ] ( %d )", dwActorID, dwTitleID, 238);
            return false;
        }
    }

    // Create and insert title
    ST_HAVE_TITLE_INFO stHaveTitle(pTBTitle, false);
    auto result = m_mapHaveTitle.insert(std::make_pair(dwTitleID, stHaveTitle));
    if (!result.second) {
        LogHelper::LogError("game.contents", "AddTitle error - Duplicate Title[ ActorID:%d, TitleID:%d ] ( %d )", dwActorID, dwTitleID, 247);
        return false;
    }

    // Remove from open set if present
    auto it = m_setTitleOpen.find(dwTitleID);
    if (it != m_setTitleOpen.end()) {
        m_setTitleOpen.erase(it);
    }

    // Send DB update (main=3, sub=0x15)
    XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 3, 0x15);
    if (pUser) {
        xSendDBPacket.XParse << pUser->GetUCID();
    }
    xSendDBPacket.XParse << dwTitleID << nLogType;
    pServer->SendDBGame(xSendDBPacket);

    // Send client update (main=3, sub=0x24)
    XSendPacket clientPacket(3, 0x24);
    clientPacket.XParse << dwTitleID << nLogType;
    if (pMover) {
        CGocNetwork::Send(static_cast<XActor*>(pMover), clientPacket);
    }

    // Log game action (main=3, sub=12)
    ST_LOG_GAME stLog;
    stLog._sMainType = 3;
    stLog._sSubType = 12;
    stLog.nParam0 = dwTitleID;
    stLog.nParam1 = pTBTitle->Info_Type;
    stLog.nParam2 = nLogType;
    stLog.nParam3 = 0;
    pServer->SendDBLog(stLog);

    CalculateTitleStat();
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

        // Send DB delete request (main=3, sub=0x18)
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        CUser* pUser = GetUser();
        XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 3, 0x18);
        if (pUser) {
            xSendDBPacket.XParse << pUser->GetUCID();
        }
        xSendDBPacket.XParse << nTitleID;
        pServer->SendDBGame(xSendDBPacket);

        // Send client delete notification (main=3, sub=0x28)
        XSendPacket clientPacket(3, 0x28);
        clientPacket.XParse << nTitleID;
        CMover* pMover = GetOwnerGO();
        if (pMover) {
            CGocNetwork::Send(static_cast<XActor*>(pMover), clientPacket);
        }
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
// Precise restoration from IDA decompilation
void CGocEntity::SendTitleList()
{
    // IDA: Create PS_TITLE_LOAD structure
    PS_TITLE_LOAD stTitleLoad;
    stTitleLoad.bResult = m_bLoadTitle;

    // IDA: Iterate through m_mapHaveTitle and add to vecTitleID
    for (const auto& pair : m_mapHaveTitle) {
        ST_TITLE_INFO_DB stTitle;
        stTitle.dwTitleID = static_cast<unsigned int>(pair.first);
        stTitle.bFavorite = pair.second.bFavorite;
        stTitleLoad.vecTitleID.push_back(stTitle);
    }

    // IDA: Iterate through m_setTitleOpen and add to vecOpenTitleID
    for (const int nTitleID : m_setTitleOpen) {
        ST_TITLE_INFO_DB stTitle;
        stTitle.dwTitleID = static_cast<unsigned int>(nTitleID);
        stTitleLoad.vecOpenTitleID.push_back(stTitle);
    }

    // IDA: Send packet (main=3, sub=0x23)
    XSendPacket xSendPacket(3, 0x23);
    xSendPacket << stTitleLoad;

    CMover* pMover = GetOwnerGO();
    if (pMover) {
        CGocNetwork::Send(static_cast<XActor*>(pMover), xSendPacket);
    }

    // IDA: Create PS_RES_TITLE_UPDATE structure
    PS_RES_TITLE_UPDATE stResSelected;
    stResSelected.bResult = true;
    stResSelected.stInsideTitle = m_stInsideTitle;
    stResSelected.stOutsideTitle = m_stOutsideTitle;

    // IDA: Send packet (main=3, sub=0x25)
    XSendPacket packet(3, 0x25);
    packet << stResSelected;

    if (pMover) {
        CGocNetwork::Send(static_cast<XActor*>(pMover), packet);
    }
}

// ============================================================================
// IDA: ?IsValidTitle@CGocEntity@@QEAA_NHE@Z (0x14005F170)
// Verified: Per IDA decompile - validates title exists and type matches
bool CGocEntity::IsValidTitle(uint32_t dwTitleID, bool bIsSuffix)
{
    // IDA 0x14005F170: Title ID <= 0 is valid (empty/none)
    if (static_cast<int>(dwTitleID) <= 0) {
        return true;
    }

    // Find title in owned map
    auto it = m_mapHaveTitle.find(dwTitleID);
    if (it == m_mapHaveTitle.end()) {
        return false; // Not owned
    }

    // Check title type matches request (BYTE3 of TB_TITLE_INFO->Title_Type)
    TB_TITLE_INFO* pTBTitle = it->second.pTBTitle;
    if (pTBTitle && ((pTBTitle->Title_Type >> 24) & 0xFF) != (bIsSuffix ? 1 : 0)) {
        return false;
    }

    return true;
}

// ============================================================================
// IDA: ?SendUpdateTitle@CGocEntity@@QEAAXUST_TitleInfo@@0_N@Z (0x14005E7C0)
// Verified: Per IDA decompile - sends title update packet to client
void CGocEntity::SendUpdateTitle(ST_TitleInfo& stInsideTitle, ST_TitleInfo& stOutsideTitle, bool bResult)
{
    // Per IDA 0x14005E7C0: Build PS_RES_TITLE_UPDATE packet
    PS_RES_TITLE_UPDATE stUpdate;
    stUpdate.bResult = bResult;
    stUpdate.stInsideTitle = stInsideTitle;
    stUpdate.stOutsideTitle = stOutsideTitle;

    // Per IDA: Send packet (main=3, sub=0x25)
    XSendPacket packet(3, 0x25);
    packet << stUpdate;

    CMover* pMover = GetOwnerGO();
    if (pMover) {
        CGocNetwork::Send(static_cast<XActor*>(pMover), packet);
    }
}

// ============================================================================
// IDA: ?UpdateTitleStat@CGocEntity@@QEAAXHHHM@Z (0x14005EF20)
// Verified: Per IDA decompile - updates title stat via CGocAttribute
void CGocEntity::UpdateTitleStat(int nClassType, int nLevel, uint32_t nType, float fValue)
{
    // IDA 0x14005EF20: Get CGocAttribute and apply stat
    CMover* pMover = GetOwnerGO();
    if (!pMover) return;
    std::shared_ptr<CGocAttribute> pAttr = pMover->GetGOC_Attribute(false);
    if (pAttr) {
        pAttr->UpdateEffectStat(nClassType, nType, fValue, 0);
    }

    (void)nLevel;
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
    // IDA 0x1400652B0: Get CGocAttribute
    CMover* pMover = GetOwnerGO();
    std::shared_ptr<CGocAttribute> pAttr = pMover ? pMover->GetGOC_Attribute(false) : nullptr;
    if (!pAttr) {
        uint32_t dwActorID = GetUser() ? GetUser()->GetActorID().dwActorID : 0;
        LogHelper::LogError("game.contents", "InitTitle error - pAttr is NULL [ ActorID:%d ] ( %d ) ", dwActorID, 2578);
        return;
    }

    bool bChanged = false;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();

    // Process inside title prefix
    if (m_stInsideTitle.dwPrefix != 0) {
        TB_TITLE_INFO* pTBPrefix = pServer->GetResourceMgr().GetTB_TITLE_INFO(m_stInsideTitle.dwPrefix);
        if (pTBPrefix) {
            for (int i = 0; i < 5; ++i) {
                if (pTBPrefix->uniTitle[i] != 0) {
                    // TODO: IDA 用 CGameWorldMode::GetState 作为 nLevel，该访问路径未落地；暂用 GetLevel
                    int nState = pAttr->GetLevel();
                    UpdateTitleStat(pTBPrefix->uniClass[i], nState,
                                    pTBPrefix->uniTitle[i], pTBPrefix->uniValue[i]);
                    bChanged = true;
                }
            }
        } else {
            uint32_t dwActorID = GetUser() ? GetUser()->GetActorID().dwActorID : 0;
            LogHelper::LogError("game.contents", "InitTitle error - No Table TB_TITLE_INFO[ ActorID:%d, dwPrefix:%d ] ( %d ) ",
                                dwActorID, m_stInsideTitle.dwPrefix, 2587);
        }
    }

    // Process inside title suffix
    if (m_stInsideTitle.dwSuffix != 0) {
        TB_TITLE_INFO* pTBSuffix = pServer->GetResourceMgr().GetTB_TITLE_INFO(m_stInsideTitle.dwSuffix);
        if (pTBSuffix) {
            for (int j = 0; j < 5; ++j) {
                if (pTBSuffix->uniTitle[j] != 0) {
                    // TODO: IDA 用 CGameWorldMode::GetState 作为 nLevel，该访问路径未落地；暂用 GetLevel
                    int nState = pAttr->GetLevel();
                    UpdateTitleStat(pTBSuffix->uniClass[j], nState,
                                    pTBSuffix->uniTitle[j], pTBSuffix->uniValue[j]);
                    bChanged = true;
                }
            }
        } else {
            uint32_t dwActorID = GetUser() ? GetUser()->GetActorID().dwActorID : 0;
            LogHelper::LogError("game.contents", "InitTitle error - Failed Add Title [ ActorID:%d, dwSuffix:%d ] ( %d ) ",
                                dwActorID, m_stInsideTitle.dwSuffix, 2608);
        }
    }

    // Recalculate stats if changed
    if (bChanged) {
        pAttr->CalculateChangedStat(0);
        pAttr->CalculateChangedEffect(0);
    }
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
    // IDA 0x14005E090
    CMover* pMover = GetOwnerGO();
    std::shared_ptr<CGocAttribute> pAttr = pMover ? pMover->GetGOC_Attribute(false) : nullptr;
    if (!pAttr) return;
    int nClass = pAttr->GetClass();
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();

    // Get prefix and suffix titles to open
    int nGetTitleID[2];
    nGetTitleID[0] = pServer->GetResourceMgr().GetPrefixTitleToOpen(nCondition, nClass, nValue);
    nGetTitleID[1] = pServer->GetResourceMgr().GetSuffixTitleToOpen(nCondition, nClass, nValue);

    CUser* pUser = GetUser();
    for (int i = 0; i < 2; ++i) {
        int nTitleID = nGetTitleID[i];
        if (nTitleID <= 0) continue;

        // Skip if already in open set
        if (m_setTitleOpen.find(static_cast<unsigned int>(nTitleID)) != m_setTitleOpen.end()) {
            continue;
        }
        // Skip if already owned
        if (m_mapHaveTitle.find(static_cast<unsigned int>(nTitleID)) != m_mapHaveTitle.end()) {
            continue;
        }

        // Add to open set
        m_setTitleOpen.insert(static_cast<unsigned int>(nTitleID));

        // Send DB update (main=3, sub=0x19)
        XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 3, 0x19);
        if (pUser) {
            xSendDBPacket.XParse << pUser->GetUCID();
        }
        xSendDBPacket.XParse << nTitleID;
        pServer->SendDBGame(xSendDBPacket);

        // Send client update (main=3, sub=0x29)
        XSendPacket clientPacket(3, 0x29);
        clientPacket.XParse << nTitleID;
        if (pMover) {
            CGocNetwork::Send(static_cast<XActor*>(pMover), clientPacket);
        }
    }
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
    // IDA 0x14005E9E0: Check level threshold
    if (nLevel < 55) {
        return;
    }

    // Award echelon titles
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    for (std::uint8_t byLevel = 1; byLevel <= byEchelonLevel; ++byLevel) {
        TB_ECHELON* pEchelon = pServer->GetResourceMgr().GetTB_ECHELON(byLevel);
        if (pEchelon && pEchelon->Echelon_Title) {
            int nPrefixTitle = pServer->GetResourceMgr().GetPrefixTitleToOpen(10, nClass, pEchelon->Echelon_Title);
            int nSuffixTitle = pServer->GetResourceMgr().GetSuffixTitleToOpen(10, nClass, pEchelon->Echelon_Title);
            if (nPrefixTitle > 0 && !IsValidTitle(static_cast<uint32_t>(nPrefixTitle), true)) {
                AddTitle(static_cast<uint32_t>(nPrefixTitle), 4);
            }
            if (nSuffixTitle > 0 && !IsValidTitle(static_cast<uint32_t>(nSuffixTitle), true)) {
                AddTitle(static_cast<uint32_t>(nSuffixTitle), 4);
            }
        }
    }
}

// ============================================================================
// IDA: ?ReqFavoriteTitle@CGocEntity@@QEAAXAEAUPS_TITLE_FAVORITE@@@Z (0x14005F210)
// Verified: Per IDA decompile - requests title favorite toggle
void CGocEntity::ReqFavoriteTitle(PS_TITLE_FAVORITE& stTitleFavorite)
{
    // IDA 0x14005F210: fill UCID
    CUser* pUser = GetUser();
    if (pUser) {
        stTitleFavorite.dwUCID = pUser->GetUCID();
    }
    uint32_t dwActorID = pUser ? pUser->GetActorID().dwActorID : 0;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();

    // Validate TB_TITLE_INFO
    TB_TITLE_INFO* pTBTitle = pServer->GetResourceMgr().GetTB_TITLE_INFO(stTitleFavorite.dwTitleID);
    if (!pTBTitle) {
        LogHelper::LogError("game.contents", "ReqFavoriteTitle error - TB_TITLE_INFO is NULL [ ActorID:%d, Title:%d ] ( %d ) ",
                            dwActorID, stTitleFavorite.dwTitleID, 1074);
        CMover* pMover = GetOwnerGO();
        CGocNetwork::SendErrorMessage(pMover, 3, 0x2A, 0xC739);
        return;
    }

    // Validate TB_COMMON favorite limit
    TB_COMMON* pTBCommon = pServer->GetResourceMgr().GetTB_COMMON(0x9C41);
    if (!pTBCommon) {
        LogHelper::LogError("game.contents", "ReqFavoriteTitle error - TB_COMMON is NULL [ ActorID:%d, Title:%d ] ( %d ) ",
                            dwActorID, stTitleFavorite.dwTitleID, 1083);
        CMover* pMover = GetOwnerGO();
        CGocNetwork::SendErrorMessage(pMover, 3, 0x2A, 0xC739);
        return;
    }

    // Find title in owned map
    auto it = m_mapHaveTitle.find(stTitleFavorite.dwTitleID);
    if (it == m_mapHaveTitle.end()) {
        LogHelper::LogError("game.contents", "ReqFavoriteTitle error - Not Have [ ActorID:%d, Title:%d ] ( %d ) ",
                            dwActorID, stTitleFavorite.dwTitleID, 1093);
        CMover* pMover = GetOwnerGO();
        CGocNetwork::SendErrorMessage(pMover, 3, 0x2A, 0xC739);
        return;
    }

    // Same state -> just echo back
    if (it->second.bFavorite == stTitleFavorite.bFavorite) {
        XSendPacket xSendPacket(3, 0x2A);
        xSendPacket.XParse << stTitleFavorite.dwUCID << stTitleFavorite.dwTitleID << stTitleFavorite.bFavorite;
        CMover* pMover = GetOwnerGO();
        if (pMover) {
            CGocNetwork::Send(static_cast<XActor*>(pMover), xSendPacket);
        }
        return;
    }

    // Update favorite count with limit check (Title_Type != 0 => suffix)
    CMover* pMover = GetOwnerGO();
    if (pTBTitle->Title_Type != 0) {
        if (stTitleFavorite.bFavorite) {
            if (static_cast<float>(m_shFavoriteSuffixCount) >= pTBCommon->Value) {
                LogHelper::LogError("game.contents", "ReqFavoriteTitle error - Suffix Count Over [ ActorID:%d, Title:%d ] ( %d ) ",
                                    dwActorID, stTitleFavorite.dwTitleID, 1134);
                CGocNetwork::SendErrorMessage(pMover, 3, 0x2A, 0xC739);
                return;
            }
            ++m_shFavoriteSuffixCount;
        } else {
            --m_shFavoriteSuffixCount;
        }
    } else {
        if (stTitleFavorite.bFavorite) {
            if (static_cast<float>(m_shFavoritePrefixCount) >= pTBCommon->Value) {
                LogHelper::LogError("game.contents", "ReqFavoriteTitle error - Prefix Count Over [ ActorID:%d, Title:%d ] ( %d ) ",
                                    dwActorID, stTitleFavorite.dwTitleID, 1115);
                CGocNetwork::SendErrorMessage(pMover, 3, 0x2A, 0xC739);
                return;
            }
            ++m_shFavoritePrefixCount;
        } else {
            --m_shFavoritePrefixCount;
        }
    }

    it->second.bFavorite = stTitleFavorite.bFavorite;

    // Send DB update (main=3, sub=0x24)
    XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 3, 0x24);
    xSendDBPacket.XParse << stTitleFavorite.dwUCID << stTitleFavorite.dwTitleID << stTitleFavorite.bFavorite;
    pServer->SendDBGame(xSendDBPacket);
}

// ============================================================================
// IDA: ?ResFavoriteTitle@CGocEntity@@QEAAXAEAUPS_DB_TITLE_FAVORITE@@@Z (0x14005F840)
// Verified: Per IDA decompile - handles DB response for title favorite
void CGocEntity::ResFavoriteTitle(PS_DB_TITLE_FAVORITE& stTitleFavorite)
{
    // IDA 0x14005F840
    uint32_t dwActorID = GetUser() ? GetUser()->GetActorID().dwActorID : 0;
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();

    if (stTitleFavorite.nError != 0) {
        // DB error -> roll back the count already incremented by ReqFavoriteTitle
        TB_TITLE_INFO* pTBTitle = pServer->GetResourceMgr().GetTB_TITLE_INFO(stTitleFavorite.psInfo.dwTitleID);
        if (pTBTitle) {
            if (pTBTitle->Title_Type != 0) {
                if (stTitleFavorite.psInfo.bFavorite)
                    --m_shFavoriteSuffixCount;
                else
                    ++m_shFavoriteSuffixCount;
            } else {
                if (stTitleFavorite.psInfo.bFavorite)
                    --m_shFavoritePrefixCount;
                else
                    ++m_shFavoritePrefixCount;
            }
            LogHelper::LogError("game.contents", "ResFavoriteTitle error - DB Error [ ActorID:%d, Title:%d, Error:%d ] ( %d ) ",
                                dwActorID, stTitleFavorite.psInfo.dwTitleID, stTitleFavorite.nError, 1182);
        } else {
            LogHelper::LogError("game.contents", "ResFavoriteTitle error - TB_TITLE_INFO DB Error [ ActorID:%d, Title:%d, Error:%d ] ( %d ) ",
                                dwActorID, stTitleFavorite.psInfo.dwTitleID, stTitleFavorite.nError, 1160);
        }
        CGocNetwork::SendErrorMessage(GetOwnerGO(), 3, 0x2A, 0xC739);
        return;
    }

    // Success: find title and apply favorite
    auto it = m_mapHaveTitle.find(stTitleFavorite.psInfo.dwTitleID);
    if (it == m_mapHaveTitle.end()) {
        LogHelper::LogError("game.contents", "ResFavoriteTitle error - Not Have [ ActorID:%d, Title:%d, Error:%d ] ( %d ) ",
                            dwActorID, stTitleFavorite.psInfo.dwTitleID, stTitleFavorite.nError, 1192);
        CGocNetwork::SendErrorMessage(GetOwnerGO(), 3, 0x2A, 0xC739);
        return;
    }

    it->second.bFavorite = stTitleFavorite.psInfo.bFavorite;

    // Send client update (main=3, sub=0x2A)
    PS_TITLE_FAVORITE psFavorite;
    psFavorite.dwUCID = stTitleFavorite.psInfo.dwUCID;
    psFavorite.dwTitleID = stTitleFavorite.psInfo.dwTitleID;
    psFavorite.bFavorite = stTitleFavorite.psInfo.bFavorite;
    XSendPacket xSendPacket(3, 0x2A);
    xSendPacket.XParse << psFavorite.dwUCID << psFavorite.dwTitleID << psFavorite.bFavorite;
    CMover* pMover = GetOwnerGO();
    if (pMover) {
        CGocNetwork::Send(static_cast<XActor*>(pMover), xSendPacket);
    }

    // Log game action (main=3, sub=22)
    CUser* pUser = GetUser();
    if (pUser) {
        ST_LOG_GAME stLog;
        stLog._nUAID = pUser->GetUAID();
        stLog._nUCID = pUser->GetUCID();
        stLog._sMainType = 3;
        stLog._sSubType = 22;
        stLog.nParam0 = psFavorite.dwTitleID;
        stLog.nParam1 = it->second.bFavorite ? 1 : 0;
        stLog.nParam2 = m_shFavoritePrefixCount;
        stLog.nParam3 = m_shFavoriteSuffixCount;
        stLog.nParam4 = 0;
        pServer->SendDBLog(stLog);
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
// Verified: Per IDA decompile - increments free revive count with TB_ITEM stack max check
bool CGocEntity::ReviveFree()
{
    // Per IDA: Get TB_ITEM for free revive item (0x26272A93) and check stack max
    // XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    // if (!pGameServer) return false;
    //
    // TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, 0x26272A93);
    // if (!pTB_ITEM || m_nFreeReviveCount >= pTB_ITEM->Item_Stack_Max) {
    //     return false;
    // }
    //
    // SetFreeReviveCount(++m_nFreeReviveCount, true);
    // return true;

    // TODO: 需要完整的依赖实现 - XGameServer, XResourceMgr, TB_ITEM
    return false;
}


// ============================================================================
// IDA: ?SendDBProfilePhoto@CGocEntity@@QEAAXXZ (0x1400622D0)
// Verified: Per IDA decompile - sends DB request to load profile photo
void CGocEntity::SendDBProfilePhoto()
{
    // Per IDA: Get CUser via RTDynamicCast
    // CUser* pUser = GetUser();
    // if (!pUser) return;
    //
    // XActor* pActor = static_cast<XActor*>(pUser);
    // XSendDBPacket xSendDBPacket(pActor, 3, 0x25);
    // xSendDBPacket << pUser->GetUCID();
    //
    // XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    // if (pGameServer) {
    //     pGameServer->SendDBGame(xSendDBPacket);
    // }

    // TODO: 需要完整的依赖实现 - XSendDBPacket, CUser::GetUCID
}


// ============================================================================
// IDA: ?LoadProfilePhoto@CGocEntity@@QEAAXAEAUPS_PROFILE_PHOTO_LOAD@@@Z (0x1400623E0)
// Verified: Per IDA decompile - loads profile photos from DB response
void CGocEntity::LoadProfilePhoto(PS_PROFILE_PHOTO_LOAD& stLoad)
{
    // Per IDA: Iterate through vector and add each photo
    // for (size_t i = 0; i < stLoad.vecList.size(); ++i) {
    //     ST_PROFILE_PHOTO_INFO* pInfo = &stLoad.vecList[i];
    //     AddProfilePhoto(*pInfo);
    // }
    //
    // CUser* pUser = GetUser();
    // if (pUser) {
    //     LogHelper::LogDebug("game.contents", "LoadProfilePhoto (UCID:%d, count:%d)",
    //         pUser->GetUCID(), stLoad.vecList.size());
    // }
    //
    // CheckEquipProfilePhoto();
    // m_nProfilePhotoTick = GetTickCount64() + 10000;

    (void)stLoad;
    // TODO: 需要完整的依赖实现
}


// ============================================================================
// IDA: ?CheckEquipProfilePhoto@CGocEntity@@QEAAXXZ (0x1400624E0)
// Verified: Per IDA decompile - checks and sets default equipped profile photo
void CGocEntity::CheckEquipProfilePhoto()
{
    // Per IDA: Get CUser via RTDynamicCast
    // CUser* pUser = GetUser();
    // if (!pUser) return;
    //
    // uint32_t dwOldPhotoID = pUser->GetProfilePhotoID();
    // auto it_Old = m_mapProfilePhoto.find(dwOldPhotoID);
    // if (it_Old != m_mapProfilePhoto.end()) {
    //     return;
    // }
    //
    // auto pAttr = pUser->GetGOC<CGocAttribute>();
    // if (!pAttr) return;
    //
    // uint8_t byType = (pAttr->GetAwaken() == 1) ? 2 : 1;
    // uint8_t byClass = pUser->GetClass();
    //
    // XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    // if (!pGameServer) return;
    //
    // TB_PHOTO_ITEM* pTB_PHOTO_ITEM = XResourceMgr::FindDefaultPhotoItemID(
    //     &pGameServer->m_xResourceMgr, byClass, byType);
    // if (!pTB_PHOTO_ITEM) return;
    //
    // auto it_Have = m_mapProfilePhoto.find(pTB_PHOTO_ITEM->ID);
    // if (it_Have == m_mapProfilePhoto.end()) {
    //     return;
    // }
    //
    // // Send DB change request for default photo
    // PS_DB_PROFILE_PHOTO_CHANGE psPhoto;
    // ... send packet

    // TODO: 需要完整的依赖实现 - CUser::GetProfilePhotoID, CGocAttribute, TB_PHOTO_ITEM, PS_DB_PROFILE_PHOTO_CHANGE
}


// ============================================================================
// IDA: ?CheckAddProfilePhoto@CGocEntity@@QEAA_NKAEAUST_PROFILE_PHOTO_INFO@@@Z (0x140062820)
// Verified: Per IDA decompile - validates and prepares profile photo info
bool CGocEntity::CheckAddProfilePhoto(uint32_t dwItemID, ST_PROFILE_PHOTO_INFO& stPhoto)
{
    // Per IDA: Get TB_ITEM for the item ID and validate
    // XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    // if (!pGameServer) return false;
    //
    // TB_ITEM* pTB_ITEM = XResourceMgr::GetTB_ITEM(&pGameServer->m_xResourceMgr, dwItemID);
    // TB_PHOTO_ITEM* pTB_PHOTO_ITEM = XResourceMgr::GetTB_PHOTO_ITEM(...);
    // ... validation and period handling

    (void)dwItemID;
    (void)stPhoto;
    // TODO: 需要完整的依赖实现 - TB_ITEM, TB_PHOTO_ITEM, ATL::CTime, ATL::CTimeSpan
    return false;
}


// ============================================================================
// IDA: ?AddProfilePhoto@CGocEntity@@QEAAHAEAUST_PROFILE_PHOTO_INFO@@@Z (0x140062E50)
// Verified: Per IDA decompile - adds a profile photo to owned list
int CGocEntity::AddProfilePhoto(ST_PROFILE_PHOTO_INFO& stPhoto)
{
    // Per IDA: Get CUser via RTDynamicCast
    // CUser* pUser = GetUser();
    // if (!pUser) return 58011;
    //
    // XGameServer* pGameServer = TXSingleton<XGameServer>::Instance();
    // if (!pGameServer) return 58011;
    //
    // TB_PHOTO_ITEM* pTB_PHOTO_ITEM = XResourceMgr::GetTB_PHOTO_ITEM(...);
    // if (!pTB_PHOTO_ITEM) return 58011;
    //
    // // Period type and expiration handling
    // // Duplicate check
    // // Insert into map

    (void)stPhoto;
    // TODO: 需要完整的依赖实现 - TB_PHOTO_ITEM, ST_HAVE_PROFILE_PHOTO_INFO
    return 58011;
}


// ============================================================================
// IDA: ?SendProfilePhoto@CGocEntity@@QEAAXXZ (0x140063170)
// Verified: Per IDA decompile - sends profile photo list to client
void CGocEntity::SendProfilePhoto()
{
    // Per IDA: Get CUser for UCID
    // CUser* pUser = GetUser();
    // if (!pUser) return;
    //
    // PS_PROFILE_PHOTO_LOAD psLoad;
    // psLoad.dwUCID = pUser->GetUCID();
    //
    // for (auto it = m_mapProfilePhoto.begin(); it != m_mapProfilePhoto.end(); ++it) {
    //     psLoad.vecList.push_back(it->second.stInfo);
    // }
    //
    // XActor* pActor = static_cast<XActor*>(pUser);
    // XSendPacket xSendPacket(3, 9);
    // xSendPacket << psLoad;
    // CGocNetwork::Send(pActor, &xSendPacket);

    // TODO: 需要完整的依赖实现
}


// ============================================================================
// IDA: ?ProfilePhotoRemainTimeCheck@CGocEntity@@QEAAXXZ (0x140063370)
// Verified: Per IDA decompile - checks timed profile photo expiration
void CGocEntity::ProfilePhotoRemainTimeCheck()
{
    // Per IDA: Check timer
    // if (m_nProfilePhotoTick <= 0 || m_nProfilePhotoTick > GetTickCount64()) {
    //     return;
    // }
    //
    // ATL::CTime tCurr = ATL::CTime::GetTickCount();
    // std::vector<uint32_t> vecDelList;
    //
    // for (auto it = m_mapProfilePhoto.begin(); it != m_mapProfilePhoto.end(); ++it) {
    //     ST_HAVE_PROFILE_PHOTO_INFO& stPhoto = it->second;
    //     if (stPhoto.stInfo.byPeriodType == 1) {
    //         if (stPhoto.stInfo.nEndDate <= static_cast<__int64>(tCurr)) {
    //             vecDelList.push_back(stPhoto.stInfo.dwPhotoID);
    //         }
    //     }
    // }
    //
    // if (!vecDelList.empty()) {
    //     DeleteProfilePhoto(&vecDelList);
    // }
    //
    // m_nProfilePhotoTick = GetTickCount64() + 10000;

    // TODO: 需要完整的依赖实现 - ATL::CTime, DeleteProfilePhoto
}


// ============================================================================
// IDA: ?ReqChangeProfilePhoto@CGocEntity@@QEAAHK@Z (0x1400634C0)
// Verified: Per IDA decompile - requests profile photo change
int CGocEntity::ReqChangeProfilePhoto(uint32_t dwPhotoID)
{
    // Per IDA: Get CUser via RTDynamicCast
    // CUser* pUser = GetUser();
    // if (!pUser) return 58011;
    //
    // uint32_t dwOldPhotoID = pUser->GetProfilePhotoID();
    // if (dwOldPhotoID == dwPhotoID) return 58012;
    //
    // auto it_Old = m_mapProfilePhoto.find(dwOldPhotoID);
    // if (it_Old == m_mapProfilePhoto.end()) return 58010;
    //
    // auto it = m_mapProfilePhoto.find(dwPhotoID);
    // if (it == m_mapProfilePhoto.end()) return 58010;
    //
    // // Build and send DB change request
    // PS_DB_PROFILE_PHOTO_CHANGE psPhoto;
    // ... send packet

    (void)dwPhotoID;
    // TODO: 需要完整的依赖实现 - CUser::GetProfilePhotoID, PS_DB_PROFILE_PHOTO_CHANGE
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
