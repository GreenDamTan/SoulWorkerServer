#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerDB.h"
#include <cstdint>
#include <map>
#include <vector>

// Forward declarations
class XGameServer;
class CUser;
struct TB_BOOSTER;

/**
 * @brief E_BOOSTER_EFFECTTYPE - Booster effect type enumeration
 *
 * IDA verified from CGocBooster::GetTotalValue and GetTotalRate
 */
enum E_BOOSTER_EFFECTTYPE : std::uint8_t {
    eBooster_Effect_None = 0,
    eBooster_Effect_ExpRate = 1,
    eBooster_Effect_DropRate = 2,
    eBooster_Effect_Attack = 3,
    eBooster_Effect_AddFP = 4,      // FP (Fatigue Point) boost
    eBooster_Effect_Defense = 5,
    eBooster_Effect_HP = 6,
    eBooster_Effect_MP = 7,
    eBooster_Effect_SoulJamil = 8,
    eBooster_Effect_Special = 9,    // Special stat change (apply/clear)
    eBooster_Effect_Max
};

/**
 * @brief E_BOOSTER_TYPE - Booster type enumeration
 */
enum E_BOOSTER_TYPE : std::uint8_t {
    eBooster_Type_Party = 0,
    eBooster_Type_Echelon = 1,
    eBooster_Type_Item = 2,
    eBooster_Type_Event = 3,
    eBooster_Type_NetCafe = 4,
    eBooster_Type_Day_Event = 5,
    eBooster_Type_NetCafe_SG_EVENT = 6,
    eBooster_Type_Max = 7
};

/**
 * @brief ST_BOOSTER_OUTPUT - Booster output for client notification
 *
 * IDA verified from CGocBooster::_ConvertOutputData (0x14004A6D0)
 */
struct ST_BOOSTER_OUTPUT {
    std::uint16_t wBoosterID = 0;
    std::int32_t lRemainSec = 0;    // Remaining seconds (-1 for unlimited)
};

/**
 * @brief CGocBooster - Game Object Component for boost/buff system
 *
 * Handles active boosts, buffs, and temporary stat enhancements.
 * IDA verified class layout from constructor and member access patterns.
 */
class CGocBooster : public GOComponent {
public:
    // IDA: ??0CGocBooster@@QEAA@XZ (0x140049B10)
    CGocBooster();

    // IDA: ??1CGocBooster@@UEAA@XZ (0x140049BD0)
    virtual ~CGocBooster();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // IDA: ?GetFamilyID@CGocBooster@@SAHXZ (0x140039040)
    static int GetFamilyID() { return 0; }

    // IDA: ?SetChangeStat@CGocBooster@@QEAAX_N@Z (0x140049AF0)
    void SetChangeStat(bool bChange) { m_bChangeStat = bChange; }

    // IDA: ?Init@CGocBooster@@QEAAXXZ (0x140049C30)
    void Init();

    // IDA: ?OnUpdate@CGocBooster@@QEAAXM@Z (0x140049CA0)
    void OnUpdate(float fDeltaTime);

    // Booster entry/exit
    // IDA: ?EnterBooster@CGocBooster@@QEAAXE@Z (0x14004A080)
    void EnterBooster(std::uint8_t byConsumeArea);

    // IDA: ?ExitBooster@CGocBooster@@QEAAXXZ (0x140049FA0)
    void ExitBooster();

    // Booster info queries
    // IDA: ?FindBooster@CGocBooster@@QEAA_NG@Z (0x14004A540)
    bool FindBooster(std::uint16_t wIndex);

    // IDA: ?GetBooster@CGocBooster@@QEAA_NGAEAUST_BOOSTER_INFO@@@Z (0x14004A5A0)
    bool GetBooster(std::uint16_t wIndex, ST_BOOSTER_INFO& stBase);

    // IDA: ?GetBoosterList@CGocBooster@@QEAA_NAEAV?$vector@UST_BOOSTER_OUTPUT@@...@@Z (0x14004A140)
    bool GetBoosterList(std::vector<ST_BOOSTER_OUTPUT>& vecBooster);

    // Booster operations
    // IDA: ?AddBooster@CGocBooster@@QEAAXG_N@Z (0x14004AA30)
    void AddBooster(std::uint16_t wIndex, bool bAccount);

    // IDA: ?RemoveBooster@CGocBooster@@QEAAXG@Z (0x14004AC20)
    void RemoveBooster(std::uint16_t wIndex);

    // IDA: ?ClearAllBoosters@CGocBooster@@QEAAXXZ (0x14004AF30)
    void ClearAllBoosters();

    // Booster time management
    // IDA: ?SetBoosterTime@CGocBooster@@QEAAXG_J@Z (0x14004A650)
    void SetBoosterTime(std::uint16_t wIndex, std::int64_t nTime);

    // IDA: ?UpdateBoosterTime@CGocBooster@@QEAAXG@Z (0x14004BA50)
    void UpdateBoosterTime(std::uint16_t wBoosterID);

    // Booster stat effects
    // IDA: ?ApplyBoosterStat@CGocBooster@@QEAAXPEAUTB_BOOSTER@@@Z (0x14004B040)
    void ApplyBoosterStat(TB_BOOSTER* pBoosterTable);

    // IDA: ?ClearBoosterStat@CGocBooster@@QEAAXPEAUTB_BOOSTER@@@Z (0x14004B0F0)
    void ClearBoosterStat(TB_BOOSTER* pBoosterTable);

    // Total effect calculations
    // IDA: ?GetTotalRate@CGocBooster@@QEAAMW4E_BOOSTER_EFFECTTYPE@@@Z (0x14004B220)
    float GetTotalRate(E_BOOSTER_EFFECTTYPE eType);

    // IDA: ?GetTotalValue@CGocBooster@@QEAAHW4E_BOOSTER_EFFECTTYPE@@@Z (0x14004B350)
    int GetTotalValue(E_BOOSTER_EFFECTTYPE eType);

    // IDA: ?GetEffectValue@CGocBooster@@QEAAMW4E_BOOSTER_EFFECTTYPE@@M@Z (0x14004B1A0)
    float GetEffectValue(E_BOOSTER_EFFECTTYPE eType, float fBaseValue);

    // IDA: ?IsExist@CGocBooster@@QEAA_NW4E_BOOSTER_EFFECTTYPE@@@Z (0x14004B5F0)
    bool IsExist(E_BOOSTER_EFFECTTYPE eType);

    // Booster change operations
    // IDA: ?ChangeBooster@CGocBooster@@QEAAXW4E_BOOSTER_TYPE@@G_J_N@Z (0x14004B6D0)
    void ChangeBooster(E_BOOSTER_TYPE eType, std::uint16_t wBoosterID, std::int64_t lTime, bool bAccount);

    // IDA: ?_ChangeBooster@CGocBooster@@AEAAXW4E_BOOSTER_TYPE@@G_J_N@Z (0x14004B720)
    void _ChangeBooster(E_BOOSTER_TYPE eType, std::uint16_t wBoosterID, std::int64_t lTime, bool bAccount);

    // Event boosters
    // IDA: ?CheckTimeEventBooster@CGocBooster@@QEAAXXZ (0x14004BBD0)
    void CheckTimeEventBooster();

    // IDA: ?CheckDayEventBooster@CGocBooster@@QEAAXG@Z (0x14004BE30)
    void CheckDayEventBooster(std::uint16_t wBoosterID);

    // Loading
    // IDA: ?LoadBoosterList@CGocBooster@@QEAAXUPS_BOOSTER_LIST_RES@@@Z (0x14004A800)
    void LoadBoosterList(PS_BOOSTER_LIST_RES& psRes);

    // IDA: ?LoadBooster@CGocBooster@@QEAAXAEAUST_BOOSTER_INFO@@@Z (0x14004A920)
    void LoadBooster(ST_BOOSTER_INFO& stInfo);

    // Time event boosters
    // IDA: ?AddTimeEventBooster@CGocBooster@@QEAAXG_J@Z (0x14004AB20)
    void AddTimeEventBooster(std::uint16_t wBoosterID, std::int64_t lRemainTime);

    // Send operations
    // IDA: ?SendBoosterList@CGocBooster@@QEAAXXZ (0x14004A210)
    void SendBoosterList();

    // IDA: ?SendAddBooster@CGocBooster@@QEAAXAEAUST_BOOSTER_OUTPUT@@@Z (0x14004A340)
    void SendAddBooster(ST_BOOSTER_OUTPUT& stBooster);

    // IDA: ?SendRemoveBooster@CGocBooster@@QEAAXG@Z (0x14004A460)
    void SendRemoveBooster(std::uint16_t wBoosterID);

    // IDA: ?CheckSendBuffAbility@CGocBooster@@QEAAXXZ (0x14004A0F0)
    void CheckSendBuffAbility();

    // Database operations
    // IDA: ?SendDBBoosterList@CGocBooster@@QEAAXXZ (0x14004BE90)
    void SendDBBoosterList();

    // IDA: ?DeleteBoosterDB@CGocBooster@@QEAAXG_N@Z (0x14004C010)
    void DeleteBoosterDB(std::uint16_t wBoosterID, bool bAccount);

    // IDA: ?SaveBoosterDB@CGocBooster@@QEAAXG_J_N@Z (0x14004C230)
    void SaveBoosterDB(std::uint16_t wBoosterID, std::int64_t lRemainTime, bool bAccount);

    // Group ID management
    // IDA: ?GetBoosterIDByGID@CGocBooster@@QEAAGG@Z (0x14004C460)
    std::uint16_t GetBoosterIDByGID(std::uint16_t wGroupID);

    // IDA: ?AddGroupID@CGocBooster@@QEAA_NGG@Z (0x14004C4D0)
    bool AddGroupID(std::uint16_t wGroupID, std::uint16_t wBoosterID);

    // IDA: ?DeleteGroupID@CGocBooster@@QEAAXG@Z (0x14004C560)
    void DeleteGroupID(std::uint16_t wGroupID);

    // IDA: ?GetBoosterOutput@CGocBooster@@QEAA_NGAEAUST_BOOSTER_OUTPUT@@@Z (0x14004A770)
    bool GetBoosterOutput(std::uint16_t wIndex, ST_BOOSTER_OUTPUT& stOutput);

protected:
    // IDA: ?_ConvertOutputData@CGocBooster@@AEAAXAEAUST_BOOSTER_INFO@@AEAUST_BOOSTER_OUTPUT@@@Z (0x14004A6D0)
    void _ConvertOutputData(ST_BOOSTER_INFO* input, ST_BOOSTER_OUTPUT* output);

    // IDA: ?_GetTotalValue@CGocBooster@@AEAAXW4E_BOOSTER_EFFECTTYPE@@AEAM1@Z (0x14004B480)
    void _GetTotalValue(E_BOOSTER_EFFECTTYPE eType, float& fRate, float& fValue);

protected:
    // IDA verified member layout from constructor (0x140049B10) and member access patterns
    // IDA: std::map<unsigned short, ST_BOOSTER_INFO> m_mapBooster
    std::map<std::uint16_t, ST_BOOSTER_INFO> m_mapBooster;  // Active boosters map
    // IDA: std::map<unsigned short, unsigned short> m_mapGroupID (not unsigned char!)
    std::map<std::uint16_t, std::uint16_t> m_mapGroupID;    // Group ID to booster ID mapping
    std::uint8_t m_byConsumeArea = 0;                        // Consume area type (1=maze, 2=field)
    bool m_bChangeStat = false;                              // Stat change pending flag
    bool m_bLoadDB = false;                                  // DB load complete flag
    std::uint16_t m_wBoosterID[10] = {};                     // Booster ID array (for event boosters)
};
