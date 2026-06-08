#include "User.h"

// Getter functions
char* CUser::GetAccountID()
{
    return m_stCharInfo.szAccountID;
}

short CUser::GetBonusFP()
{
    return (unsigned short)m_stCharInfo.shBonusFP;
}

short CUser::GetFP()
{
    return (unsigned short)m_stCharInfo.shFP;
}

__int64 CUser::GetLastLevelupDate()
{
    return m_biLastLevelUpDate;
}

bool CUser::GetFirstEnter()
{
    return m_bFirstEnter;
}

BYTE CUser::GetBlockType()
{
    return m_byBlockType;
}

BYTE CUser::GetGMPower()
{
    return m_stCharInfo.byGMPower;
}

bool CUser::IsMatching()
{
    return m_bMatchingState;
}

__int64 CUser::GetExp()
{
    return m_stCharInfo.nExp;
}

DWORD CUser::GetSocialUseID()
{
    return m_dwSocialUseID;
}

DWORD CUser::GetActiveBroachEffect()
{
    return m_stCharInfo.dwActiveBroachEffect;
}

__int64 CUser::GetCreateDate()
{
    return m_nCreateDate;
}

DWORD CUser::GetFirstUCID()
{
    return m_dwFirstUCID;
}

__int64 CUser::GetAccountCreateDate()
{
    return m_biAccountCreateDate;
}

short CUser::GetMaxComboCount()
{
    return m_nMaxContinousAttackHit;
}

int CUser::GetLeagueID()
{
    return (unsigned int)m_stCharInfo.stLeagueInfo.nLeagueID;
}

__int64 CUser::GetLastAccountComeBackDate()
{
    return m_biLastAccountComeBackDate;
}

__int64 CUser::GetLastComeBackDate()
{
    return m_biLastComeBackDate;
}

bool CUser::IsPVPPenalty()
{
    return m_bPVPPenalty;
}

bool CUser::IsFullStat()
{
    return m_bFirstWorldEnter;
}

// Setter functions
void CUser::SetLastLevelupDate(__int64 biDate)
{
    m_biLastLevelUpDate = biDate;
}

void CUser::SetMatchingState(bool bState)
{
    m_bMatchingState = bState;
}

void CUser::SetReserveRevive(int bReserve)
{
    m_bReserveRevive = bReserve;
}

void CUser::SetSocialUseID(DWORD dwID)
{
    m_dwSocialUseID = dwID;
}

void CUser::SetFullStat(bool bFirstEnter)
{
    m_bFirstWorldEnter = bFirstEnter;
}

void CUser::SetClientLoadComplete(bool bRet)
{
    m_bClientLoadComplete = bRet;
}

// Block type setter
void CUser::SetBlockType(BYTE byType)
{
    m_byBlockType = byType;
}

// Kick functions
void CUser::Kickout(struct PS_KICK_USER_INFO* psKick, bool bDirect)
{
    // TODO: Implement full kickout logic
    // This function should:
    // 1. Check if user is in connected state
    // 2. If in game and not direct, set kickout time for delayed kick
    // 3. Otherwise, set state to kickout immediately
    // 4. Send kick packet to client
    // 5. Log the kick event
    // 6. Send DB notification
    // For now, just set the kick flag
    m_dwKickoutTime = 0; // Will be set properly when fully implemented
}

bool CUser::CheckKickoutNow()
{
    if (!m_dwKickoutTime)
        return false;
    
    // TODO: Set state to kickout
    // XClient::SetState(this, eStateKickOut);
    return true;
}

void CUser::SetKick_AlreadyLogin()
{
    m_bKick_AlreadyLogin = true;
}

bool CUser::IsKick_AlreadyLogin()
{
    return m_bKick_AlreadyLogin;
}
