// CGocAttribute - Game Object Component for actor attributes
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140039080 - 0x140049620

#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerCore.h"  // For ST_UPDATE_SPECIAL_OPTION
#include <cstdint>
#include <vector>
#include <map>
#include <cstring>

// Forward declarations
struct STMyCharInfoEx;
struct TB_LEVELUP_POINT;
struct TB_SOUL_GUAGE;
struct TB_STATUS;
struct FIRST_STATUS_TABLE;
class CMover;
class CUser;
class XGameServer;
class XActor;
class CGocAttribute;  // Forward declaration for CCalculateStatus

// Stat constants (from IDA analysis)
constexpr int MAX_STAT_COUNT = 77;  // 0x4D
constexpr int MAX_SPECIAL_EFFECT = 55;  // 0x37

/**
 * @brief StatInfo - Single stat entry for GetFinalStats vector version
 * IDA: struct StatInfo { unsigned __int8 byIndex; float statValue; }
 */
struct StatInfo {
    std::uint8_t byIndex;
    float statValue;
};

/**
 * @brief CCalculateStatus - Status calculation helper class
 *
 * Contains static methods for calculating various stats from CGocAttribute.
 * Based on IDA decompilation of GameServer.exe (0x1402D6BF0 - 0x1402D8620)
 */
class CCalculateStatus {
public:
    // Initialization
    static void Init(CCalculateStatus* pStatus) {
        // IDA: Initialization stub - no-op in original
        (void)pStatus;
    }

    // Basic stat calculations (0x1402D6BF0 - 0x1402D6D80)
    static float CALCULATE_STAT_STR(CGocAttribute* pAttr);
    static float CALCULATE_STAT_DEX(CGocAttribute* pAttr);
    static float CALCULATE_STAT_INT(CGocAttribute* pAttr);
    static float CALCULATE_STAT_AGI(CGocAttribute* pAttr);
    static float CALCULATE_STAT_BAL(CGocAttribute* pAttr);
    static float CALCULATE_STAT_VIT(CGocAttribute* pAttr);
    static float CALCULATE_STAT_LUC(CGocAttribute* pAttr);

    // ST/SG/HP calculations (0x1402D6DD0 - 0x1402D71A0)
    static float CALCULATE_STAT_HP_MAX(CGocAttribute* pAttr);
    static float CALCULATE_STAT_SG_MAX(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ST_MAX(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ST_REGEN(CGocAttribute* pAttr);
    static float CALCULATE_STAT_SV_MAX(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MSR(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ASR(CGocAttribute* pAttr);
    static float CALCULATE_STAT_SG_REG(CGocAttribute* pAttr);
    static float CALCULATE_STAT_SG_REGEN(CGocAttribute* pAttr);
    static float CALCULATE_STAT_HP_REGEN(CGocAttribute* pAttr);

    // Attack calculations (0x1402D73A0 - 0x1402D76EA)
    static float CALCULATE_STAT_PA(CGocAttribute* pAttr);
    static float CALCULATE_STAT_PA_RATE(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MA(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MA_RATE(CGocAttribute* pAttr);
    static float CALCULATE_STAT_PATK_MAX(CGocAttribute* pAttr);
    static float CALCULATE_STAT_PATK_MIN(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MATK_MAX(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MATK_MIN(CGocAttribute* pAttr);
    static float CALCULATE_STAT_PDEF(CGocAttribute* pAttr);

    // Defense calculations (0x1402D77D0 - 0x1402D7370)
    static float CALCULATE_STAT_PD(CGocAttribute* pAttr);
    static float CALCULATE_STAT_PD_RATE(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MD(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MD_RATE(CGocAttribute* pAttr);

    // Defense calculations (0x1402D77D0 - 0x1402D790D)
    static float CALCULATE_STAT_MDEF(CGocAttribute* pAttr);
    static float CALCULATE_STAT_PAR(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MAR(CGocAttribute* pAttr);

    // Dodge/Critical calculations (0x1402D7A70 - 0x1402D7EAC)
    static float CALCULATE_STAT_PARP(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MARP(CGocAttribute* pAttr);
    static float CALCULATE_STAT_PCP(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MCP(CGocAttribute* pAttr);
    static float CALCULATE_STAT_PCRP(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MCRP(CGocAttribute* pAttr);
    static float CALCULATE_STAT_PCA(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MCA(CGocAttribute* pAttr);
    static float CALCULATE_STAT_PDSR(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ADR(CGocAttribute* pAttr);
    static float CALCULATE_STAT_CAR(CGocAttribute* pAttr);
    static float CALCULATE_STAT_CAD(CGocAttribute* pAttr);

    // Resistance calculations (0x1402D7FD0 - 0x1402D81FD)
    static float CALCULATE_STAT_RES_BURN(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_POISON(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_SHOCK(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_BLEED(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_STUN(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_PARALYSIS(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_SLEEP(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_FREEZE(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_CHARM(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_CONFUSION(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_SILENCE(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_WEAKNESS(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_FIRE(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_ICE(CGocAttribute* pAttr);
    static float CALCULATE_STAT_RES_ELECTRIC(CGocAttribute* pAttr);

    // PDPR/MDPR calculations (0x1402D8390 - 0x1402D8422)
    static float CALCULATE_STAT_PDPR(CGocAttribute* pAttr);
    static float CALCULATE_STAT_MDPR(CGocAttribute* pAttr);

    // Attribute calculations (0x1402D84C0 - 0x1402D8620)
    static float CALCULATE_STAT_ATTRIBUTE_FIRE(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_ICE(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_POISON(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_ELECTRIC(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_BLEED(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_ABHOR(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_LIGHT(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_DARKNESS(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_COOL(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_HEAL(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_PAIN(CGocAttribute* pAttr);

    // Attribute resistance calculations
    static float CALCULATE_STAT_ATTRIBUTE_RES_FIRE(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_RES_ICE(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_RES_POISON(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_RES_ELECTRIC(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_RES_BLEED(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_RES_ABHOR(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_RES_LIGHT(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_RES_DARKNESS(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_RES_COOL(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_RES_HEAL(CGocAttribute* pAttr);
    static float CALCULATE_STAT_ATTRIBUTE_RES_PAIN(CGocAttribute* pAttr);

    // PVP calculations
    static float CALCULATE_STAT_PVP_ATK(CGocAttribute* pAttr);
    static float CALCULATE_STAT_PVP_DEF(CGocAttribute* pAttr);
};

// ST_UPDATE_SPECIAL_OPTION 定义在 PSServerCore.h 中
// 前向声明在此处使用
struct ST_UPDATE_SPECIAL_OPTION;

/**
 * @brief CGocAttribute - Game Object Component for actor attributes
 *
 * Handles actor attributes like HP, FP, stats, experience, level, etc.
 * Based on IDA decompilation of GameServer.exe
 */
class CGocAttribute : public GOComponent {
public:
    // Constructor (0x140039080)
    CGocAttribute();

    // Destructor (0x1400393F0)
    virtual ~CGocAttribute();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Static GetFamilyID
    static int GetFamilyID() { return 1; }  // Family ID for attribute component

    // Init (0x140039490) - Initialize from character info
    void Init(STMyCharInfoEx& stCharInfoEx, std::uint16_t nTableIdx, int nStatusType);

    // Reset (0x140039B40)
    void Reset();

    // Stat functions (0x140039B90)
    virtual void SetOriginStat();

    // SetStartStat (0x140039DD0)
    virtual void SetStartStat();

    // SetStatusTable (0x140039FA0)
    bool SetStatusTable();

    // OnUpdate (0x14003A0E0)
    virtual void OnUpdate(float fDeltaTime);

    // IsRanger (0x14003A710)
    bool IsRanger() const;

    // IsPlayer (0x14003A730)
    bool IsPlayer() const;

    // LevelUp (0x14003A770)
    void LevelUp(int nAdd, int nUseCheat);

    // UpdateScaleStat (0x14003B510)
    void UpdateScaleStat(int nStat, float fValue, bool bCalc);

    // UpdateAddStat (0x14003B5A0)
    void UpdateAddStat(int nStat, float fValue, bool bCalc);

    // UpdateEffectStat (0x14003B640)
    void UpdateEffectStat(int nClassType, int nStatType, float fValue, bool bCalc);

    // UpdateBuffEffectStat (0x14003B750)
    int UpdateBuffEffectStat(int nStatType, float fValue, bool bCalc, bool bUseInClear);

    // CalculateChangedStat (0x14003B920)
    void CalculateChangedStat(bool bSend);

    // CalculateChangedEffect (0x14003BA00)
    void CalculateChangedEffect(bool bSync);

    // SendSpecialOptionList (0x14003BCA0)
    void SendSpecialOptionList(CMover* pReceiverMover);

    // GetEquipIndex (0x1400420D0)
    int GetEquipIndex(std::uint32_t dwOptionID, float fOptionValue);

    // GetRateTargetStat (0x140042190)
    int GetRateTargetStat(int iStatType);

    // SetSkillOptionEffect (0x1400421E0)
    void SetSkillOptionEffect(bool bEquip, int nSkillGroupIndex, int nType, int nValue);

    // GetSkillOptionEffect (0x1400439F0)
    void GetSkillOptionEffect(int nSkillGroupIndex, int nType, float& fValue);

    // ClearSkillOptionEffectPart (0x140043AB0)
    void ClearSkillOptionEffectPart(int nSkillGroupIndex, int nType);

    // SetStat (0x14003C080)
    virtual void SetStat(int nStatID, float fValue, bool bSync);

    // SetFinalStat (0x14003C170)
    virtual void SetFinalStat(int nStatID, float fValue, bool bSync);

    // GetStat (0x14003CDF0)
    float GetStat(int nStat) const;

    // Revive (0x14003CE30)
    void Revive();

    // SetFlagUseST (0x14003CF60)
    void SetFlagUseST();

    // SetContinousCost (0x14003CF80)
    void SetContinousCost(int nState, float fCost);

    // SetHP (0x14003D050)
    void SetHP(float fValue);

    // SendUpdateStatList (0x14003D090)
    virtual void SendUpdateStatList();

    // SendUpdateStat (0x14003D3B0)
    virtual void SendUpdateStat(int nStat);

    // SendOriginStatAll (0x14003D660)
    void SendOriginStatAll();

    // SendStatAll (0x14003D830)
    void SendStatAll();

    // SendEmptySpecialOptionList (0x140043850)
    void SendEmptySpecialOptionList();

    // SetExp (0x14003DB60)
    void SetExp(double fExp, float fBonus, bool bSync);

    // ResetExp (0x14003E3E0)
    void ResetExp();

    // CheatSetExp (0x14003E580)
    void CheatSetExp(int nExp);

    // CanUseFP (0x14003EF40)
    bool CanUseFP(std::int16_t shPoint) const;

    // UseFP (0x14003F1C0)
    bool UseFP(std::int16_t shPoint, std::uint32_t dwMazeID, bool bIsEffect);

    // FPEffect (0x14003F930)
    void FPEffect();

    // DelFPEffect (0x14003F970)
    void DelFPEffect();

    // FPRestore (0x14003F9C0)
    void FPRestore(std::int16_t shFP);

    // GetSpecialEffect (0x14003EEE0)
    float GetSpecialEffect(int eEffectType);

    // ProcessSGReg (0x14003E830)
    void ProcessSGReg();

    // SetInitFPDate (0x14003FB80)
    void SetInitFPDate(std::int64_t biDate);

    // SetInitFPDate (0x14003FBA0)
    void SetInitFPDate();

    // CheatUpdateStat (0x1400402D0)
    void CheatUpdateStat(unsigned int nStat, float fValue, std::uint8_t byType);

    // CheatResetStat (0x1400403B0)
    void CheatResetStat();

    // OnUpdateInitDate (0x140040A80)
    void OnUpdateInitDate();

    // AddEchelonExp (0x140040F30)
    void AddEchelonExp(int nExp, int nBonus, bool bSync);

    // Echelon related
    void CheckEchelonInfo();
    void SetEchelonLevelBooster();
    void GM_EchelonLevelUp(std::uint8_t byLevel, int nExp);
    void SendUpdateEchelonExp(int nExp, int nBonus, bool bSync);
    void SendEchelonInfoSave();

    // ResetMoveSpeed (0x140041AE0)
    void ResetMoveSpeed(bool bSync);

    // CheatFPChange (0x1400405D0)
    void CheatFPChange(int nType, std::int16_t shFP);

    // SendDBInitFP (0x140040CF0)
    void SendDBInitFP();

    // CalculateOtherChangedEffect (0x140040530)
    void CalculateOtherChangedEffect(std::vector<struct ST_UPDATE_SPECIAL_OPTION>& stOptionList);

    // Roguelike functions (0x140042390, 0x140042DC0)
    void InitRoguelike();
    void ExitRoguelike();

    // CalculateCharacterStat (0x140043450)
    void CalculateCharacterStat();

    // SetItemRateInfo (0x140044540)
    void SetItemRateInfo(std::uint8_t bySlot, float fValueAtk, float fValueDef,
                         std::uint16_t wLevel, std::uint8_t byRank);

    // UnsetItemRateInfo (0x1400446F0)
    void UnsetItemRateInfo(std::uint8_t bySlot);

    // AddItemRateInfo (0x140044780)
    void AddItemRateInfo(std::uint8_t bySlot, float fAddValue);

    // GetItemRateInfo (0x140044860)
    const void* GetItemRateInfo(std::uint8_t bySlot) const;

    // SendMaxStatLog (0x1400448E0)
    void SendMaxStatLog();

    // GetCharStatInfo (0x14003E7A0)
    void GetCharStatInfo(std::vector<struct ST_UPDATE_STAT>& vecBaseStat,
                         std::vector<struct ST_UPDATE_STAT>& vecFinalStat);

    // GetMonsterOriginStat (0x140049AD0) - Returns 0.0 for monster
    virtual float GetMonsterOriginStat(std::uint16_t wStatID) const;

    // IsHaveMonsterOriginStat (0x140049AE0) - Returns false for monster
    virtual bool IsHaveMonsterOriginStat(std::uint16_t wStatID) const;

    // GetFPEffect (0x1400682D0)
    bool GetFPEffect() const;

    // GetFinalStats (0x14019B9D0)
    float* GetFinalStats();

    // GetFinalStats vector version (0x14003E730)
    void GetFinalStats(std::vector<StatInfo>& vecStats);

    // SetSTRegStat (0x1402C7EC0)
    void SetSTRegStat(bool bEnable);

    // GetOriginStat (0x1402F73D0)
    float GetOriginStat(int nStat) const;

    // GetMaxRat (0x1402F73F0)
    float GetMaxRat(int nStat) const;

    // GetStatusTable (0x1402F7410)
    struct TB_STATUS* GetStatusTable();

    // GetMaxInt (0x1402F7420)
    float GetMaxInt(int nStat) const;

    // GetHP (0x140378810)
    int GetHP() const;

    // SetGameModeState (0x140048B60)
    void SetGameModeState(int nState);

    // GetLevelForStat (0x140043820)
    virtual int GetLevelForStat() const;

    // SendInfo (0x140043510)
    void SendInfo();

    // SetAwaken (0x140043B40)
    void SetAwaken(std::uint8_t byAwaken, bool bSync);

    // GetAwaken (0x1400444E0)
    std::uint8_t GetAwaken() const;

    // ClearSkillOptionEffect (0x140043A80)
    void ClearSkillOptionEffect();

    // Helper functions (from IDA)
    void SetStartRegStat(bool bEnable);
    void SetSGRegStat(bool bEnable);
    void SetFullStat();
    void IsValidStat(int nStatID, float* pfValue);
    int GetSpecialEffectIndex(int nStatType);
    void SetEquipedOption(std::uint32_t dwOptionID, float fOptionValue);
    void SendDBUpdateFP();
    void SendStatLog(int nType);

    // Stat broadcast helpers (IDA 0x140041CC0)
    bool IsShouldSyncStatBroadcast(int nStat);

    // World enter stat setup (IDA 0x140041D10)
    void SetStartStatEnterWorld(int nWorldType);

    // Sync stat flag (IDA 0x1403A1B60)
    void SetSyncStatFlag(int iIndex, int eSyncStatType);

    // Simple accessors
    int GetLevel() const { return m_nLv; }
    int GetClass() const { return m_nClass; }
    std::int64_t GetExp() const { return m_nExp; }
    std::uint8_t GetEchelonLevel() const { return m_byEchelonLevel; }
    int GetEchelonExp() const { return m_nEchelonExp; }

    // Additional simple getters/setters (IDA verified)
    // SetNoSpendST (0x140406E10)
    void SetNoSpendST(bool bNoSpend);

    // SetStopSGRegStat (0x140406E30)
    void SetStopSGRegStat(bool bStop);

    // SetStopSTRegStat (0x140406E50)
    void SetStopSTRegStat(bool bStop);

    // ResetLastEnableSGTime (0x14070ACB0)
    void ResetLastEnableSGTime();

    // GetSGRegType (0x14070ACD0)
    std::uint8_t GetSGRegType() const;

    // Helper functions for owner access (added for restoration)
    XActor* GetOwnerActor() const;
    CMover* GetMover() const;
    CUser* GetUser() const;
    std::uint32_t FindEquipedOptionIndex() const;

protected:
    // Member variables (from IDA structure analysis)

    // Cost stat indices (initialized in constructor)
    int m_iCostStat[4];                                      // Cost stat indices [1,2,3,16]

    // Scale and Add stat arrays
    float m_fScaleStat[MAX_STAT_COUNT];                      // Scale stat multipliers
    float m_fAddStat[MAX_STAT_COUNT];                        // Add stat values
    float m_fFinalStat[MAX_STAT_COUNT];                      // Final calculated stats
    float m_fOriginStat[MAX_STAT_COUNT];                     // Original stats
    bool m_bCalcStat[MAX_STAT_COUNT];                        // Stat calculation flags

    // Continuous cost
    float m_fContinousCost[4];                               // Continuous cost values
    int m_nSyncStat[MAX_STAT_COUNT];                         // Sync stat flags

    // Item special effects
    float m_fItemSpecaillEffect[MAX_SPECIAL_EFFECT];         // Item special effect values
    bool m_bItemSpecialEffectChanged[MAX_SPECIAL_EFFECT];    // Effect change flags

    // Cheat vectors
    std::vector<void*> m_vecScaleStat_Cheat;                 // Scale stat cheat
    std::vector<void*> m_vecAddStat_Cheat;                   // Add stat cheat

    // Equipped options (SEquipedOption*)
    std::vector<void*> m_vecEquipedOption;                   // Equipped option list

    // Item skill options map
    std::map<int, void*> m_mapItemSkilllOption;              // Item skill options

    // Item rate info map
    std::map<int, void*> m_mapItemRateInfo;                  // Item rate info

    // Max stat tracking
    int m_nMaxStat[MAX_STAT_COUNT];                          // Max stat values
    int m_nMaxStatEffect[MAX_SPECIAL_EFFECT];                // Max special effect values

    // Status table data (from IDA)
    char m_tbFirstStatus[196];                               // FIRST_STATUS_TABLE
    char m_StatusTable[196];                                 // TB_STATUS copy

    // Basic character info
    int m_nClass;                                            // Character class
    int m_nLv;                                               // Level
    int m_nModeLv;                                           // Mode level
    int m_nTableIdx;                                         // Table index
    int m_nStatusType;                                       // Status type

    // Update timers
    float m_fSTUpdateTime;                                   // ST update time
    float m_fSGUpdateTime;                                   // SG update time
    float m_fContinousCostSendTime;                          // Continuous cost send time

    // Experience
    std::int64_t m_nExp;                                     // Experience points

    // Reg stat flags
    bool m_bStartRegStat;                                    // Start reg stat
    bool m_bEnableSGRegStat;                                 // Enable SG reg stat
    bool m_bEnableSTRegStat;                                 // Enable ST reg stat

    // FP related
    bool m_bFPEffect;                                        // FP effect active
    std::int64_t m_biFPInitDate;                             // FP init date
    std::uint64_t m_dw64FPTick;                              // FP tick

    // Echelon (rank) system
    std::uint8_t m_byEchelonLevel;                           // Echelon level
    int m_nEchelonExp;                                       // Echelon experience
    std::uint8_t m_byPrevEchelonLevel;                       // Previous echelon level
    int m_nPrevEchelonExp;                                   // Previous echelon exp

    // Stop flags
    bool m_bStopSTRegStat;                                   // Stop ST reg stat
    bool m_bStopSGRegStat;                                   // Stop SG reg stat

    // Equip option index
    int m_iEquipOptionIndex;                                 // Equip option index

    // Soul gauge reference
    const TB_SOUL_GUAGE* m_pSoulGuageRef;                    // Soul gauge table reference

    // Nation
    std::uint8_t m_byNation;                                 // Nation

    // SG related
    float m_fPrevSG;                                         // Previous SG
    bool m_bEnableSGReg;                                     // Enable SG reg
    std::uint8_t m_bySGRegType;                              // SG reg type
    float m_fLastEnableSGTime;                               // Last enable SG time

    // No spend ST
    bool m_bNoSpendST;                                       // No spend ST flag

    // Awaken
    std::uint8_t m_byAwaken;                                 // Awaken level

    // Game mode state
    int m_nGameModeState;                                    // Game mode state
};
