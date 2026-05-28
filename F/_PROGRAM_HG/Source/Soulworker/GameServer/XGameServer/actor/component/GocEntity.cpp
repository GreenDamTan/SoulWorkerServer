// CGocEntity - Game Object Component for entity management
// Restored from GameServer.exe IDA decompilation

#include "GocEntity.h"
#include <cstring>

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
    // TODO: Implement update logic
}

void CGocEntity::ClearInteraction()
{
    m_mapInteractionBox.clear();
}

void CGocEntity::ClearTitle()
{
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
bool CGocEntity::AddTitle(uint32_t dwTitleID, int nLogType)
{
    (void)nLogType;
    ST_HAVE_TITLE_INFO stInfo;
    auto result = m_mapHaveTitle.insert(std::make_pair(dwTitleID, stInfo));
    return result.second;
}

void CGocEntity::DeleteTitle(int nTitleID)
{
    m_mapHaveTitle.erase(nTitleID);
}

void CGocEntity::SendTitleList()
{
    // TODO: Implement packet sending
}

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

int CGocEntity::GetRoguelikeNextMap(bool bNext)
{
    (void)bNext;
    return m_dwRoguelikeNextMap;
}

void CGocEntity::InitRoguelikeMap()
{
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
