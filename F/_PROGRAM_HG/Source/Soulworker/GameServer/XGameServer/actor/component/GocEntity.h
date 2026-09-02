#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMail.h"
#include <cstdint>
#include <map>
#include <vector>
#include <set>
#include <string>

// Forward declarations
struct TB_TITLE_INFO;
struct TB_COMMON;
struct TB_PHOTO_ITEM;

// Forward declarations for types not in shared headers
struct ST_CHECK_AUTO_BLOCK_INFO {
    bool bSend = false;
    int nCount = 0;
    std::vector<float> vecData;
};

struct ST_HAVE_TITLE_INFO {
    TB_TITLE_INFO* pTBTitle = nullptr;
    bool bFavorite = false;
    std::uint16_t wStatType[5] = {};
    float fStatValue[5] = {};
    std::uint8_t byStatIndex[5] = {};

    ST_HAVE_TITLE_INFO() {
        pTBTitle = nullptr;
        bFavorite = false;
        for (int i = 0; i < 5; ++i) {
            wStatType[i] = 0;
            fStatValue[i] = 0.0f;
            byStatIndex[i] = 0;
        }
    }

    ST_HAVE_TITLE_INFO(TB_TITLE_INFO* pTB, bool bFav);
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

// PDB LF_FIELDLIST (UDT 0x3680): TB_PHOTO_ITEM* pTB_PHOTO_ITEM @0, ST_PROFILE_PHOTO_INFO stInfo @8, size 24.
struct ST_HAVE_PROFILE_PHOTO_INFO {
    TB_PHOTO_ITEM* pTB_PHOTO_ITEM = nullptr;
    ST_PROFILE_PHOTO_INFO stInfo{};

    ST_HAVE_PROFILE_PHOTO_INFO() = default;
    // IDA: ?0ST_HAVE_PROFILE_PHOTO_INFO@@QEAA@PEAUTB_PHOTO_ITEM@@AEBUST_PROFILE_PHOTO_INFO@@@Z
    ST_HAVE_PROFILE_PHOTO_INFO(TB_PHOTO_ITEM* pTB, const ST_PROFILE_PHOTO_INFO& st)
        : pTB_PHOTO_ITEM(pTB), stInfo(st) {}
};

static_assert(sizeof(ST_HAVE_PROFILE_PHOTO_INFO) == 24,
              "ST_HAVE_PROFILE_PHOTO_INFO size must match PDB");

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
    void LoadTitle(PS_TITLE_LOAD& stLoadTitle);
    bool AddTitle(uint32_t dwTitleID, int nLogType);
    void DeleteTitle(int nTitleID);
    void SendTitleList();
    void InitTitle(ST_TitleInfo& stOutsideTitle, ST_TitleInfo& stInsideTitle);
    void UpdateTitle(PS_REQ_TITLE_UPDATE& stTitleInfo);
    bool IsValidTitle(uint32_t dwTitleID, bool bIsSuffix);
    void SendUpdateTitle(ST_TitleInfo& stInsideTitle, ST_TitleInfo& stOutsideTitle, bool bResult);
    void Levelup(int nBeforeLevel, int nAfterLevel);
    void UpdateTitleStat(int nClassType, int nLevel, uint32_t nType, float fValue);

    // IDA: ?AddTitleByClass@CGocEntity@@QEAAKKH@Z (0x14005C3C0)
    std::uint32_t AddTitleByClass(std::uint32_t dwTitleGroupID, int nLogType);

    // IDA: ?UpdateOpenTitle@CGocEntity@@QEAAXHH@Z (0x14005E090)
    void UpdateOpenTitle(int nCondition, int nValue);

    // IDA: ?SendDBLoadTitle@CGocEntity@@QEAAXXZ (0x14005E8D0)
    void SendDBLoadTitle();

    // IDA: ?CheckAutoBlockCount@CGocEntity@@QEAAXW4AUTO_BLOCK_CHECK_TYPE@@HK@Z (0x14005D970)
    void CheckAutoBlockCount(int eType, int nAdd, uint32_t dwData);

    // IDA: ?CheckEchelonTitle@CGocEntity@@QEAAXHEH@Z (0x14005E9E0)
    void CheckEchelonTitle(int nLevel, uint8_t byEchelonLevel, int nClass);

    // IDA: ?ReqFavoriteTitle@CGocEntity@@QEAAXAEAUPS_TITLE_FAVORITE@@@Z (0x14005F210)
    void ReqFavoriteTitle(PS_TITLE_FAVORITE& stTitleFavorite);

    // IDA: ?ResFavoriteTitle@CGocEntity@@QEAAXAEAUPS_DB_TITLE_FAVORITE@@@Z (0x14005F840)
    void ResFavoriteTitle(PS_DB_TITLE_FAVORITE& stTitleFavorite);

    // IDA: ?UpdateCutscene@CGocEntity@@QEAAXAEAUPS_CUTSCENE_UPDATE@@@Z (0x14005FDD0)
    void UpdateCutscene(PS_CUTSCENE_UPDATE& stCutscene);

    // IDA: ?LoginNetCafe@CGocEntity@@QEAAX_N@Z (0x14005FFA0)
    void LoginNetCafe(bool bFirstEnter);

    // IDA: ?SetNetCafe@CGocEntity@@QEAAX_N00@Z (0x140060030)
    void SetNetCafe(bool bNetCafe, bool bDBSync, bool bLoad);

    // IDA: ?SendNetCafeState@CGocEntity@@QEAAXXZ (0x140060650)
    void SendNetCafeState();

    // IDA: ?SendSGAuthInfo@CGocEntity@@QEAAX_N@Z (0x140060740)
    void SendSGAuthInfo(bool bSyncDB);

    // IDA: ?EventNetCafeItemBuy@CGocEntity@@QEAAXAEAUPS_EVENT_NETCAFE_ITEM_BUY@@@Z (0x140060A10)
    void EventNetCafeItemBuy(PS_EVENT_NETCAFE_ITEM_BUY& stBuy);

    // IDA: ?EventNetCafeItemDelete@CGocEntity@@QEAAXXZ (0x1400619D0)
    void EventNetCafeItemDelete();

    // IDA: ?SetFreeReviveCount@CGocEntity@@QEAAXH_N@Z (0x140062070)
    void SetFreeReviveCount(int nCount, bool bSync);

    // IDA: ?ReviveFree@CGocEntity@@QEAA_NXZ (0x1400621F0)
    bool ReviveFree();

    // IDA: ?SendDBProfilePhoto@CGocEntity@@QEAAXXZ (0x1400622D0)
    void SendDBProfilePhoto();

    // IDA: ?LoadProfilePhoto@CGocEntity@@QEAAXAEAUPS_PROFILE_PHOTO_LOAD@@@Z (0x1400623E0)
    void LoadProfilePhoto(PS_PROFILE_PHOTO_LOAD& stLoad);

    // IDA: ?CheckEquipProfilePhoto@CGocEntity@@QEAAXXZ (0x1400624E0)
    void CheckEquipProfilePhoto();

    // IDA: ?CheckAddProfilePhoto@CGocEntity@@QEAA_NKAEAUST_PROFILE_PHOTO_INFO@@@Z (0x140062820)
    bool CheckAddProfilePhoto(uint32_t dwItemID, ST_PROFILE_PHOTO_INFO& stPhoto);

    // IDA: ?AddProfilePhoto@CGocEntity@@QEAAHAEAUST_PROFILE_PHOTO_INFO@@@Z (0x140062E50)
    int AddProfilePhoto(ST_PROFILE_PHOTO_INFO& stPhoto);

    // IDA: ?SendProfilePhoto@CGocEntity@@QEAAXXZ (0x140063170)
    void SendProfilePhoto();

    // IDA: ?ProfilePhotoRemainTimeCheck@CGocEntity@@QEAAXXZ (0x140063370)
    void ProfilePhotoRemainTimeCheck();

    // IDA: ?ReqChangeProfilePhoto@CGocEntity@@QEAAHK@Z (0x1400634C0)
    int ReqChangeProfilePhoto(uint32_t dwPhotoID);

    // IDA: ?ResChangeProfilePhoto@CGocEntity@@QEAAXAEAUPS_DB_PROFILE_PHOTO_CHANGE@@@Z (0x1400638C0)
    void ResChangeProfilePhoto(PS_DB_PROFILE_PHOTO_CHANGE& psPhoto);

    // IDA: ?ProfilePhotoFavorite@CGocEntity@@QEAAXAEAUPS_PROFILE_PHOTO_FAVORITE@@@Z (0x1400643D0)
    void ProfilePhotoFavorite(PS_PROFILE_PHOTO_FAVORITE& psFavorite);

    // IDA: ?ResAddProfilePhoto@CGocEntity@@QEAAXAEAUPS_DB_PROFILE_PHOTO_ADD@@@Z (0x1400647B0)
    void ResAddProfilePhoto(PS_DB_PROFILE_PHOTO_ADD& psAdd);

    // IDA: ?ResUpdateProfilePhoto@CGocEntity@@QEAAXAEAUPS_DB_PROFILE_PHOTO_UPDATE@@@Z (0x140064C50)
    void ResUpdateProfilePhoto(PS_DB_PROFILE_PHOTO_UPDATE& psUpdate);

    // IDA: ?GetProfilePhotoInfo@CGocEntity@@QEAA_NKAEAUST_PROFILE_PHOTO_INFO@@@Z (0x140064FF0)
    bool GetProfilePhotoInfo(uint32_t dwPhotoID, ST_PROFILE_PHOTO_INFO& stInfo);

    // IDA: ?DeleteProfilePhoto@CGocEntity@@QEAAXAEAV?$vector@KV?$allocator@K@std@@@std@@@Z (0x140063FB0)
    void DeleteProfilePhoto(std::vector<uint32_t>& vecDelList);

    // IDA: ?SendFreeReviveCount@CGocEntity@@QEAAXXZ (0x1400650A0)
    void SendFreeReviveCount();

    // IDA: ?CalculateTitleStat@CGocEntity@@QEAAXXZ (0x1400652B0)
    void CalculateTitleStat();

    // Title getters
    ST_TitleInfo GetOutsideTitle() const;
    bool GetNetCafe() const;
    bool IsLoadNetCafe() const;
    bool IsRoguelikeState() const;

    // IDA: ?GetProfilePhotoID@CGocEntity@@QEAAKXZ (PDB S_GPROC32 [0001:00061270], Cb=56)
    // 说明: IDA 数据库中该地址被并入 EventNetCafeItemBuy 大函数体，未能单独反编译；
    // PDB 证实为独立 56 字节无参 getter。调用方（XForceProcess 匹配链）读取的正是
    // 角色 stMyCharInfoEx()->stBaseInfo.dwProfilePhotoID，与 ResChangeProfilePhoto
    // 落地路径一致 (GocEntity.cpp 先例)。
    // TODO: 需人工审查：若后续 IDA 边界拆分成功，按反编译结果复核此实现。
    std::uint32_t GetProfilePhotoID() const;

    // Roguelike system
    void UpdateRoguelikeStep();
    int GetRoguelikeNextMap(bool bNext);
    void InitRoguelikeMap();
    int GetRoguelikeStep() const;
    int GetRoguelikeMapIndex() const;
    int GetRoguelikeStartMap();
    void SetRoguelikeResult(uint32_t dwTime, bool bClearBoss);
    bool IsLastRoguelike() const;
    int GetRoguelikeRewardMoney(int nCount, bool bBossSector);
    void SendRoguelikeResult(bool bClear);
    void SendRoguelikeCurrentInfo();
    void AddTitleAll(int nLogType);
    bool IsRoguelikeFirstMap() const;
    int GetRoguelikeTotalStep() const;
    int GetRoguelikeMapToGoNext() const;
    bool IsRoguelikeMapToGoNext() const;
    void ResetRoguelikeNextMap();

    // Auth system
    char* GetWMPortalID();
    uint64_t GetRepresentativeUCID() const;
    const ST_REPRESENTATIVE_INFO& GetRepresentativeInfo() const;
    void SetRepresentativeInfo(const ST_REPRESENTATIVE_INFO& stInfo);
    void SetWMAuthInfo(const ST_WM_AUTH_INFO& stInfo);
    void SetSGAuthInfo(const ST_SG_AUTH_INFO& stInfo);
    uint8_t GetSGAuthType() const;

    // Get owner as CUser (if applicable)
    class CUser* GetUser() const;

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
    // IDA: std::_Tree<unsigned long, ST_HAVE_PROFILE_PHOTO_INFO> -> map<uint32_t, ST_HAVE_PROFILE_PHOTO_INFO>
    std::map<uint32_t, ST_HAVE_PROFILE_PHOTO_INFO> m_mapProfilePhoto;
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
