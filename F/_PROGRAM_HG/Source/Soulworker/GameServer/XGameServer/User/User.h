#pragma once

#include <string>
#include <cstdint>

// Windows type definitions
typedef unsigned char BYTE;
typedef unsigned long DWORD;

// Forward declarations
struct STPosInfo;

// Character info structures
struct STLeagueInfo
{
    int nLeagueID;
    // TODO: Add other league info fields as discovered
};

struct STCharInfo
{
    char szAccountID[64];  // Account ID string
    short shBonusFP;       // Bonus FP
    short shFP;            // FP
    BYTE byGMPower;        // GM power level
    __int64 nExp;          // Experience
    DWORD dwActiveBroachEffect;  // Active broach effect
    STLeagueInfo stLeagueInfo;   // League info
    // TODO: Add other char info fields as discovered
};

class CUser
{
public:
    // Getter functions
    char* GetAccountID();
    short GetBonusFP();
    short GetFP();
    __int64 GetLastLevelupDate();
    bool GetFirstEnter();
    BYTE GetBlockType();
    BYTE GetGMPower();
    bool IsMatching();
    __int64 GetExp();
    DWORD GetSocialUseID();
    DWORD GetActiveBroachEffect();
    __int64 GetCreateDate();
    DWORD GetFirstUCID();
    __int64 GetAccountCreateDate();
    short GetMaxComboCount();
    int GetLeagueID();
    __int64 GetLastAccountComeBackDate();
    __int64 GetLastComeBackDate();
    bool IsPVPPenalty();
    bool IsFullStat();

    // Setter functions
    void SetLastLevelupDate(__int64 biDate);
    void SetMatchingState(bool bState);
    void SetReserveRevive(int bReserve);
    void SetSocialUseID(DWORD dwID);
    void SetFullStat(bool bFirstEnter);
    void SetClientLoadComplete(bool bRet);

private:
    // Member variables
    STCharInfo m_stCharInfo;              // Character information
    __int64 m_biLastLevelUpDate;          // Last level up date
    bool m_bFirstEnter;                   // First enter flag
    BYTE m_byBlockType;                   // Block type
    bool m_bMatchingState;                // Matching state
    bool m_bReserveRevive;                // Reserve revive flag
    DWORD m_dwSocialUseID;                // Social use ID
    __int64 m_nCreateDate;                // Create date
    DWORD m_dwFirstUCID;                  // First UCID
    __int64 m_biAccountCreateDate;        // Account create date
    short m_nMaxContinousAttackHit;       // Max continuous attack hit
    __int64 m_biLastAccountComeBackDate;  // Last account come back date
    __int64 m_biLastComeBackDate;         // Last come back date
    bool m_bPVPPenalty;                   // PVP penalty flag
    bool m_bFirstWorldEnter;              // First world enter flag (used by IsFullStat/SetFullStat)
    bool m_bClientLoadComplete;           // Client load complete flag
    // TODO: Add other member variables as discovered
};
