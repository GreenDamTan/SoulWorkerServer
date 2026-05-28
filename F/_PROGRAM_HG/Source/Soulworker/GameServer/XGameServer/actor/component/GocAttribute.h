// CGocAttribute - Game Object Component for actor attributes
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140039080 - 0x140049620

#pragma once

#include "GOComponent.h"
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
class CCalculateStatus;
class CMover;
class CUser;
class XGameServer;
class XActor;

// Stat constants (from IDA analysis)
constexpr int MAX_STAT_COUNT = 77;  // 0x4D
constexpr int MAX_SPECIAL_EFFECT = 55;  // 0x37

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

    // CalculateChangedStat (0x14003B920)
    void CalculateChangedStat(bool bSend);

    // CalculateChangedEffect (0x14003BA00)
    void CalculateChangedEffect(bool bSync);

    // SetStat (0x14003C080)
    virtual void SetStat(int nStatID, float fValue, bool bSync);

    // SetFinalStat (0x14003C170)
    virtual void SetFinalStat(int nStatID, float fValue, bool bSync);

    // GetStat (0x14003CDF0)
    float GetStat(int nStat) const;

    // Revive (0x14003CE30)
    void Revive();

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
    void FPRestore(float fRatio);

    // ProcessSGReg (0x14003E830)
    void ProcessSGReg();

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

    // Roguelike functions (0x140042390, 0x140042DC0)
    void InitRoguelike();
    void ExitRoguelike();

    // CalculateCharacterStat (0x140043450)
    void CalculateCharacterStat();

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
    void SetEquipedOption(int nStatType, float fValue);
    void SendDBUpdateFP();
    void SendStatLog(int nType);

    // Simple accessors
    int GetLevel() const { return m_nLv; }
    int GetClass() const { return m_nClass; }
    std::int64_t GetExp() const { return m_nExp; }
    std::uint8_t GetEchelonLevel() const { return m_byEchelonLevel; }
    int GetEchelonExp() const { return m_nEchelonExp; }

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
