#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include <cstdint>
#include <map>
#include <vector>
#include <set>
#include <string>

// Forward declarations for types not in shared headers
struct ST_CHECK_AUTO_BLOCK_INFO {
    bool bSend = false;
    int nCount = 0;
    std::vector<float> vecData;
};

struct ST_HAVE_TITLE_INFO {
    bool bFavorite = false;
    std::uint16_t wStatType[5] = {};
    float fStatValue[5] = {};
    std::uint8_t byStatIndex[5] = {};

    ST_HAVE_TITLE_INFO() {
        bFavorite = false;
        for (int i = 0; i < 5; ++i) {
            wStatType[i] = 0;
            fStatValue[i] = 0.0f;
            byStatIndex[i] = 0;
        }
    }
};

struct ST_USER_INTERACTION_INFO {
    bool bFlag = false;
    bool bSend = false;
    int nParam = 0;
};

struct ST_ROGUELIKE_RESULT {
    int nMapID = 0;
    int nResult = 0;
};

/**
 * @brief CGocEntity - Game Object Component for entity management
 */
class CGocEntity : public GOComponent {
public:
    CGocEntity();
    virtual ~CGocEntity();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Static family ID
    static int GetFamilyID() { return 4; }

    // Initialization and lifecycle
    void Init();
    void OnUpdate();
    void ClearInteraction();
    void ClearTitle();
    void ClearRoguelikeData();

    // Title system
    bool AddTitle(uint32_t dwTitleID, int nLogType);
    void DeleteTitle(int nTitleID);
    void SendTitleList();
    void Levelup(int nBeforeLevel, int nAfterLevel);

    // Title getters
    ST_TitleInfo GetOutsideTitle() const;
    bool GetNetCafe() const;
    bool IsLoadNetCafe() const;
    bool IsRoguelikeState() const;

    // Roguelike system
    void UpdateRoguelikeStep();
    int GetRoguelikeNextMap(bool bNext);
    void InitRoguelikeMap();
    int GetRoguelikeStep() const;
    int GetRoguelikeMapIndex() const;

    // Auth system
    char* GetWMPortalID();
    uint64_t GetRepresentativeUCID() const;
    const ST_REPRESENTATIVE_INFO& GetRepresentativeInfo() const;
    void SetRepresentativeInfo(const ST_REPRESENTATIVE_INFO& stInfo);
    void SetWMAuthInfo(const ST_WM_AUTH_INFO& stInfo);
    void SetSGAuthInfo(const ST_SG_AUTH_INFO& stInfo);
    uint8_t GetSGAuthType() const;

    // VaccumCube system
    void SetVaccumCubeID(int nID);
    int GetVaccumCubeID() const;

protected:
    // Title system members
    bool m_bLoadTitle = false;
    ST_TitleInfo m_stInsideTitle;
    ST_TitleInfo m_stOutsideTitle;
    std::map<int, ST_HAVE_TITLE_INFO> m_mapHaveTitle;
    std::set<int> m_setTitleOpen;
    short m_shFavoritePrefixCount = 0;
    short m_shFavoriteSuffixCount = 0;

    // Interaction system
    std::map<int, ST_USER_INTERACTION_INFO> m_mapInteractionBox;

    // Auth system members
    ST_SG_AUTH_INFO m_stSGAuthInfo;
    ST_GF_AUTH_INFO m_stGFAuthInfo;
    ST_WM_AUTH_INFO m_stWMAuthInfo;
    ST_REPRESENTATIVE_INFO m_stRepresentativeInfo;

    // Auto block check
    ST_CHECK_AUTO_BLOCK_INFO m_stCheckAutoBlock[1];

    // Profile photo system
    std::map<uint16_t, ST_BOOSTER_INFO> m_mapProfilePhoto;
    int m_nProfilePhotoTick = 0;

    // NetCafe system
    bool m_bNetCafe = false;
    bool m_bLoadNetCafe = false;
    char m_szNetCafeTestIP[256] = {};
    int m_nNetCafeEventNo = 0;
    bool m_bSendSGCharInfo = false;
    int m_nFreeReviveCount = 0;

    // Roguelike system members
    std::vector<uint32_t> m_vecRoguelikeMapList;
    std::vector<ST_ROGUELIKE_RESULT> m_vecRoguelikeResult;
    int m_nRoguelikeStep = 0;
    int m_nRoguelikeTotalStep = 0;
    uint32_t m_dwRoguelikeNextMap = 0;
    int m_nCurRoguelikeMapIndex = 0;
    bool m_bRoguelikeState = false;

    // VaccumCube system
    int m_nVaccumCubeID = 0;
};
