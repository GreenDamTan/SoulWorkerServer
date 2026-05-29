// CGocEntity - Game Object Component for entity management
// Restored from GameServer.exe IDA decompilation

#include "GocEntity.h"
#include <cstring>

// Forward declarations - avoid circular dependencies
class CMover;
class CGocAttribute;
class XSendDBPacket;
class XSendPacket;

CGocEntity::CGocEntity()
    : GOComponent()
{
}

CGocEntity::~CGocEntity()
{
    m_mapHaveTitle.clear();
    m_setTitleOpen.clear();
    m_mapInteractionBox.clear();
    m_mapProfilePhoto.clear();
    m_vecRoguelikeMapList.clear();
    m_vecRoguelikeResult.clear();
}

bool CGocEntity::Initialize()
{
    return true;
}

void CGocEntity::Shutdown()
{
}

void CGocEntity::Update(float fDeltaTime)
{
    (void)fDeltaTime;
}

void CGocEntity::Init()
{
    m_stInsideTitle.dwPrefix = 0;
    m_stInsideTitle.dwSuffix = 0;
    m_stOutsideTitle.dwPrefix = 0;
    m_stOutsideTitle.dwSuffix = 0;
    m_bLoadTitle = false;

    m_mapHaveTitle.clear();
    m_setTitleOpen.clear();

    m_shFavoritePrefixCount = 0;
    m_shFavoriteSuffixCount = 0;

    m_nVaccumCubeID = 0;

    m_bNetCafe = false;
    m_bLoadNetCafe = false;

    ClearInteraction();

    m_szNetCafeTestIP[0] = '\0';
    m_nNetCafeEventNo = 0;
    m_bSendSGCharInfo = false;

    std::memset(&m_stSGAuthInfo, 0, sizeof(m_stSGAuthInfo));

    for (int i = 0; i < 1; ++i) {
        m_stCheckAutoBlock[i].bSend = false;
        m_stCheckAutoBlock[i].nCount = 0;
        m_stCheckAutoBlock[i].vecData.clear();
    }

    m_nFreeReviveCount = 0;

    m_mapProfilePhoto.clear();
    m_nProfilePhotoTick = 0;

    ClearRoguelikeData();
}

void CGocEntity::OnUpdate()
{
    // TODO: 汇编还原 - Complex function (IDA: 0x14005B880)
    // Checks user DB flags, gets CGocInventory, calls SetReadyLoadCash
    // Calls ProfilePhotoRemainTimeCheck at the end
}

void CGocEntity::ClearInteraction()
{
    m_mapInteractionBox.clear();
}

// IDA: 0x14005DA10 - Clear all titles and update stats via CGocAttribute
void CGocEntity::ClearTitle()
{
    // TODO: 汇编还原 - Complex function (IDA: 0x14005DA10)
    // Updates title stats via CGocAttribute for equipped titles
    // Clears m_mapHaveTitle, m_setTitleOpen, m_stInsideTitle, m_stOutsideTitle
    // Sends DB packet and broadcasts to nearby players
    m_mapHaveTitle.clear();
    m_setTitleOpen.clear();
    m_stInsideTitle.dwPrefix = 0;
    m_stInsideTitle.dwSuffix = 0;
    m_stOutsideTitle.dwPrefix = 0;
    m_stOutsideTitle.dwSuffix = 0;
}

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

// Title system
// IDA: 0x14005BDC0 - Complex function with table lookup, DB packet, logging
bool CGocEntity::AddTitle(uint32_t dwTitleID, int nLogType)
{
    // TODO: 汇编还原 - Complex function (IDA: 0x14005BDC0)
    // Checks TB_TITLE_INFO table for title existence
    // Validates class requirement
    // Creates ST_HAVE_TITLE_INFO and inserts into m_mapHaveTitle
    // Sends DB packet (main=3, sub=0x15)
    // Broadcasts packet (main=3, sub=0x24)
    // Logs to ST_LOG_GAME
    (void)nLogType;
    ST_HAVE_TITLE_INFO stInfo;
    auto result = m_mapHaveTitle.insert(std::make_pair(dwTitleID, stInfo));
    return result.second;
}

// IDA: 0x14005C9A0 - Delete title with validation, DB packet, broadcast
void CGocEntity::DeleteTitle(int nTitleID)
{
    // Validate title not currently equipped
    if (nTitleID == m_stInsideTitle.dwPrefix ||
        nTitleID == m_stInsideTitle.dwSuffix ||
        nTitleID == m_stOutsideTitle.dwPrefix ||
        nTitleID == m_stOutsideTitle.dwSuffix) {
        return;  // Cannot delete equipped title
    }

    // Find and erase title from map
    auto it = m_mapHaveTitle.find(nTitleID);
    if (it == m_mapHaveTitle.end()) {
        return;  // Title not found
    }
    m_mapHaveTitle.erase(it);

    // TODO: 汇编还原 - 需要XSendDBPacket, XSendPacket类型
    // Send DB packet (main=3, sub=0x18)
    // Broadcast packet (main=3, sub=0x28)
}

// IDA: 0x14005E420 - Send title list to client
void CGocEntity::SendTitleList()
{
    // TODO: 汇编还原 - 需要XSendPacket, PS_TITLE_LOAD, PS_RES_TITLE_UPDATE类型
    // Builds PS_TITLE_LOAD packet with m_bLoadTitle, m_mapHaveTitle, m_setTitleOpen
    // Sends packet (main=3, sub=0x23)
    // Builds PS_RES_TITLE_UPDATE with current selected titles
    // Sends packet (main=3, sub=0x25)
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

bool CGocEntity::IsRoguelikeState() const
{
    return m_bRoguelikeState;
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

// IDA: 0x140065830 - Complex function with resource lookup and random shuffle
void CGocEntity::InitRoguelikeMap()
{
    // TODO: 汇编还原 - Complex function (IDA: 0x140065830)
    // Gets roguelike map list from XResourceMgr::GetRoguelikeMap
    // Random shuffles the list
    // Takes first 4 maps and pushes to m_vecRoguelikeMapList
    // Initializes m_vecRoguelikeResult with map IDs
    m_vecRoguelikeMapList.clear();
    m_nRoguelikeStep = 0;
    m_nCurRoguelikeMapIndex = 0;
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

uint8_t CGocEntity::GetSGAuthType() const
{
    return m_stSGAuthInfo.byAuthType;
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
