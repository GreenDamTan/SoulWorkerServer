// CGocAttribute - Game Object Component for actor attributes
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140039080 - 0x140049620

#include "GocAttribute.h"
#include <cmath>
#include <cstring>

// Forward declarations
class CCalculateStatus;
class XResourceMgr;
class CMover;
class CUser;
class XActor;

// Helper struct for equipped options (IDA verified from 0x140041E80)
struct SEquipedOption
{
    std::uint32_t dwIndex;      // Option index
    std::uint32_t dwOptionID;   // Option type ID
    float fOptionValue;         // Option value
};

// ============================================================================
// Constructor - IDA 0x140039080
// Verified: Initializes all member variables and arrays
// ============================================================================
CGocAttribute::CGocAttribute()
    : GOComponent()
{
    // Set vtable pointer (IDA verified)
    // this->__vftable = &CGocAttribute::`vftable'

    // Initialize vectors first (from IDA - constructor calls)
    // std::vector constructor for m_vecEquipedOption
    // std::map constructor for m_mapItemSkilllOption
    // std::map constructor for m_mapItemRateInfo
    // std::vector constructor for m_vecScaleStat_Cheat
    // std::vector constructor for m_vecAddStat_Cheat

    // Initialize cost stat indices (from IDA: 0x140039080)
    m_iCostStat[0] = 1;   // HP
    m_iCostStat[1] = 2;   // SG
    m_iCostStat[2] = 3;   // ST
    m_iCostStat[3] = 16;  // SV

    // Initialize arrays to zero (from IDA)
    std::memset(m_fScaleStat, 0, sizeof(m_fScaleStat));
    std::memset(m_fAddStat, 0, sizeof(m_fAddStat));
    std::memset(m_fFinalStat, 0, sizeof(m_fFinalStat));
    std::memset(m_fOriginStat, 0, sizeof(m_fOriginStat));
    std::memset(m_bCalcStat, 0, sizeof(m_bCalcStat));
    std::memset(m_fContinousCost, 0, sizeof(m_fContinousCost));
    std::memset(m_nSyncStat, 0, sizeof(m_nSyncStat));
    std::memset(m_fItemSpecaillEffect, 0, sizeof(m_fItemSpecaillEffect));
    std::memset(m_bItemSpecialEffectChanged, 0, sizeof(m_bItemSpecialEffectChanged));

    // Clear cheat vectors (IDA verified: after memsets)
    m_vecScaleStat_Cheat.clear();
    m_vecAddStat_Cheat.clear();

    // Initialize max stat arrays (IDA verified)
    std::memset(m_nMaxStat, 0, sizeof(m_nMaxStat));
    std::memset(m_nMaxStatEffect, 0, sizeof(m_nMaxStatEffect));

    // Initialize basic members (IDA verified order)
    m_nClass = 0;
    m_nLv = 1;
    m_nModeLv = 0;
    m_nTableIdx = 0;
    m_nStatusType = 0;
    m_fSTUpdateTime = 0.0f;
    m_fSGUpdateTime = 0.0f;
    m_fContinousCostSendTime = 0.0f;
    m_nExp = 0;
    m_bStartRegStat = false;
    m_bEnableSGRegStat = false;
    m_bEnableSTRegStat = true;
    m_bFPEffect = false;
    m_biFPInitDate = 0;
    m_dw64FPTick = 0;
    m_byEchelonLevel = 0;
    m_nEchelonExp = 0;
    m_byPrevEchelonLevel = 0;
    m_nPrevEchelonExp = 0;
    m_bStopSTRegStat = false;
    m_bStopSGRegStat = false;
    m_iEquipOptionIndex = 0;

    // Clear skill option effect (IDA verified: last in constructor)
    ClearSkillOptionEffect();
}

// ============================================================================
// Destructor - IDA 0x1400393F0
// Verified: Calls vector/map destructors in reverse declaration order, then base
// ============================================================================
CGocAttribute::~CGocAttribute()
{
    // IDA verified: Set vtable to destructor vtable first
    // this->__vftable = &CGocAttribute::`vftable'

    // IDA verified: Destructors called in reverse order of declaration
    // Order: m_vecAddStat_Cheat, m_vecScaleStat_Cheat, m_mapItemRateInfo,
    //        m_mapItemSkilllOption, m_vecEquipedOption, then base class
    m_vecAddStat_Cheat.~vector();
    m_vecScaleStat_Cheat.~vector();
    m_mapItemRateInfo.~map();
    m_mapItemSkilllOption.~map();
    m_vecEquipedOption.~vector();

    // Base class destructor called automatically
}

bool CGocAttribute::Initialize()
{
    return true;
}

void CGocAttribute::Shutdown()
{
}

void CGocAttribute::Update(float fDeltaTime)
{
    (void)fDeltaTime;
}

// ============================================================================
// Helper functions - Owner access via internal structure
// ============================================================================

// Get owner actor from component's internal owner pointer
// IDA: std::list<CBattleZone*>::size returns VChunkLocker* which contains owner
XActor* CGocAttribute::GetOwnerActor() const
{
    // TODO: 需要实现正确的 owner 指针获取
    // IDA pattern in multiple functions:
    // v20 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    // if (v20) pActor = (XActor*)&v20[3].m_ChunkSizeTempMemOfs;
    // The owner is stored at offset +3 of the returned pointer
    return nullptr;
}

// Get owner as CMover (if applicable)
// IDA: Same pattern but cast to CMover
CMover* CGocAttribute::GetMover() const
{
    // TODO: RTTI cast from owner actor to CMover
    // IDA pattern: v6 = (CMover*)std::list<CBattleZone*>::size((VChunkLocker*)this);
    return nullptr;
}

// Get owner as CUser (if applicable)
// IDA: RTTI dynamic cast from owner to CUser
CUser* CGocAttribute::GetUser() const
{
    // TODO: 需要实现 RTTI cast
    // IDA pattern from GetAwaken (0x1400444E0):
    // v1 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    // pUser = (CUser*)_RTDynamicCast_0(v1, 0, &CMover`RTTI Type Descriptor', &CUser`RTTI Type Descriptor', 0);
    return nullptr;
}

// Find next available equipped option index
std::uint32_t CGocAttribute::FindEquipedOptionIndex() const
{
    return static_cast<std::uint32_t>(m_vecEquipedOption.size());
}

// ============================================================================
// SetOriginStat - IDA 0x140039B90
// ============================================================================
void CGocAttribute::SetOriginStat()
{
    // Save current add/scale stats
    float fAddStat[MAX_STAT_COUNT];
    float fScaleStat[MAX_STAT_COUNT];
    std::memcpy(fAddStat, m_fAddStat, sizeof(fAddStat));
    std::memcpy(fScaleStat, m_fScaleStat, sizeof(fScaleStat));

    // Clear add/scale and calculate all stats
    std::memset(m_fAddStat, 0, sizeof(m_fAddStat));
    std::memset(m_fScaleStat, 0, sizeof(m_fScaleStat));
    // TODO: CCalculateStatus::CalculateStatusAll(this)

    // Copy final stat to origin stat
    std::memcpy(m_fOriginStat, m_fFinalStat, sizeof(m_fOriginStat));

    // Restore add/scale stats
    std::memcpy(m_fAddStat, fAddStat, sizeof(m_fAddStat));
    std::memcpy(m_fScaleStat, fScaleStat, sizeof(fScaleStat));

    // TODO: Get TB_LEVELUP_POINT for SV max
    // int nLv = GetLevelForStat();
    // TB_LEVELUP_POINT* pTBLevel = XResourceMgr::GetTB_LEVELUP_POINT(nLv);
    // if (pTBLevel) m_fOriginStat[17] = (float)pTBLevel->SV_Max_Point;
}

// ============================================================================
// SetStartStat - IDA 0x140039DD0
// ============================================================================
void CGocAttribute::SetStartStat()
{
    // Set HP to max (stat 10 = max HP)
    SetStat(1, GetStat(10), false);

    // Set ST to max (stat 14 = max ST)
    SetStat(3, GetStat(14), false);

    // Set SV to 0
    SetStat(16, 0.0f, false);

    // Handle SG based on player status
    if (IsPlayer())
    {
        // TODO: Check world ID > 20000 for SG reset logic
        // For now, set SG to max
        SetStat(2, GetStat(12), false);
    }
    else
    {
        // Non-player: set SG to max
        SetStat(2, GetStat(12), false);
    }
}

// ============================================================================
// OnUpdate - IDA 0x14003A0E0
// ============================================================================
void CGocAttribute::OnUpdate(float fDeltaTime)
{
    // Process continuous cost
    bool bCostChanged = false;

    for (int i = 0; i < 4; ++i)
    {
        if (m_fContinousCost[i] > 0.0f)
        {
            float fDeltaVal = m_fContinousCost[i] * fDeltaTime;
            float fCalcVal = GetStat(m_iCostStat[i]) - fDeltaVal;
            if (fCalcVal <= 0.0f)
                fDeltaVal = fDeltaVal + fCalcVal;
            SetStat(m_iCostStat[i], GetStat(m_iCostStat[i]) - fDeltaVal, false);
            bCostChanged = true;
        }
    }

    if (bCostChanged)
    {
        m_fContinousCostSendTime += fDeltaTime;
        if (m_fContinousCostSendTime >= 1.0f)
        {
            m_fContinousCostSendTime = 0.0f;
            SendUpdateStatList();
        }
    }

    // ST regeneration
    if (m_bEnableSTRegStat && m_fContinousCost[2] == 0.0f && !m_bStopSTRegStat)
    {
        m_fSTUpdateTime += fDeltaTime;
        if (m_fSTUpdateTime >= 1.0f)
        {
            SetStat(3, GetStat(3) + GetStat(15), true);  // ST + ST regen rate
            m_fSTUpdateTime = 0.0f;
        }
    }

    // SG regeneration
    if (m_bEnableSGRegStat && m_fContinousCost[1] == 0.0f && !m_bStopSGRegStat)
    {
        m_fSGUpdateTime += fDeltaTime;
        if (m_fSGUpdateTime >= 1.0f)
        {
            SetStat(2, GetStat(2) + GetStat(13), true);  // SG + SG regen rate
            m_fSGUpdateTime = 0.0f;
        }
    }
}

// ============================================================================
// Reset - IDA 0x140039B40
// Verified: Resets SG-related state and removes FP effect
// ============================================================================
void CGocAttribute::Reset()
{
    m_fPrevSG = 0.0f;
    m_fLastEnableSGTime = 0.0f;
    SetStartRegStat(false);
    DelFPEffect();
}

// ============================================================================
// IsRanger - IDA 0x14003A710
// Verified: Simple check for ranger class type
// ============================================================================
bool CGocAttribute::IsRanger() const
{
    return m_nStatusType == 2;
}

// ============================================================================
// IsPlayer - IDA 0x14003A730
// Verified: Gets owner actor and calls XActor::IsPlayer
// ============================================================================
bool CGocAttribute::IsPlayer() const
{
    // IDA pattern:
    // if (!std::list<CBattleZone*>::size((VChunkLocker*)this))
    //     return 0;
    // v2 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    // return XActor::IsPlayer((XActor*)&v2[3].m_ChunkSizeTempMemOfs);

    XActor* pOwner = GetOwnerActor();
    if (!pOwner)
        return false;

    // TODO: Call XActor::IsPlayer
    // return pOwner->IsPlayer();
    return true;  // Placeholder
}

// ============================================================================
// GetStat - IDA 0x14003CDF0
// Verified: Returns stat value if index valid (0-76), otherwise 0.0f
// ============================================================================
float CGocAttribute::GetStat(int nStat) const
{
    if (static_cast<unsigned int>(nStat) <= 0x4C)  // 76
        return m_fFinalStat[nStat];
    return 0.0f;
}

// ============================================================================
// SetStat - IDA 0x14003C080
// Verified: Sets stat value with appropriate handling per stat type
// ============================================================================
void CGocAttribute::SetStat(int nStatID, float fValue, bool bSync)
{
    // HP, ST, SV use SetFinalStat directly
    if (nStatID == 1 || nStatID == 3 || nStatID == 16)
    {
        SetFinalStat(nStatID, fValue, bSync);
    }
    else if (nStatID == 2)  // SG
    {
        SetFinalStat(2, fValue, false);
        if (m_nSyncStat[2])
        {
            if (bSync)
                SendUpdateStat(2);
            // TODO: CCalculateStatus::CalculateStatus(2, this)
            // IDA: v5 = TXSingleton<CCalculateStatus>::Instance();
            //      CCalculateStatus::CalculateStatus(v5, 2, this);
        }
    }
    else
    {
        UpdateAddStat(nStatID, fValue, true);
    }
}

// ============================================================================
// SetFinalStat - IDA 0x14003C170
// Verified: Complex stat setting with party/force sync and special handling
// ============================================================================
void CGocAttribute::SetFinalStat(int nStatID, float fValue, bool bSync)
{
    if (nStatID < 0 || nStatID > 0x4C)
        return;

    float fValidValue = fValue;
    IsValidStat(nStatID, &fValidValue);

    // Check if value changed and ST spending is allowed
    if (m_fFinalStat[nStatID] == fValidValue)
        return;

    if (m_bNoSpendST && nStatID == 3)
        return;

    // Get user for additional processing (IDA pattern)
    CUser* pUser = GetUser();

    // Special SV handling for maze game state (IDA verified)
    if (pUser && nStatID == 16)
    {
        // TODO: Check maze game state
        // XMaze* pMaze = RTDynamicCast to XMaze from area
        // if (pMaze && XMaze::GetMazeGameState(pMaze) && fValidValue > 0)
        //     return;  // Don't set SV in maze with game state active
    }

    // Store previous value for delta calculation
    float fPrevFinalStat = m_fFinalStat[nStatID];

    // Set the new value
    m_fFinalStat[nStatID] = fValidValue;
    m_nSyncStat[nStatID] = 1;

    switch (nStatID)
    {
        case 1:  // Current HP
            if (pUser)
            {
                float fOffsetStat = m_fFinalStat[1] - fPrevFinalStat;
                if (m_fFinalStat[1] > fPrevFinalStat)
                {
                    // TODO: CMoverEx::CheckOptionEffectInvoke for heal effect
                    // CMoverEx::CheckOptionEffectInvoke(&pUser->CMoverEx, EFFECT_CONDITION_HEAL, &pUser->CMoverEx, fOffsetStat, EFFECT_INVOKE_DONT_CARE);
                    // CMoverEx::ReleaseInvokedOptionEffect(&pUser->CMoverEx, EFFECT_CONDITION_HEAL);
                }
                // TODO: Update ability info
                // CUser::stMyCharInfoEx(pUser)->stAbility.nCurAbility[0] = (int)m_fFinalStat[1];

                // TODO: Update party/force HP
                // CGocParty::SetHP / CGocForce::SetHP
            }
            m_nSyncStat[nStatID] = 2;
            break;

        case 2:  // Current SG
            if (pUser)
            {
                // TODO: Update ability info
                // CUser::stMyCharInfoEx(pUser)->stAbility.nCurAbility[1] = (int)m_fFinalStat[2];
            }
            m_nSyncStat[nStatID] = 2;
            break;

        case 3:  // Current ST
            if (pUser)
            {
                // TODO: Update ability info
                // CUser::stMyCharInfoEx(pUser)->stAbility.nCurAbility[3] = (int)m_fFinalStat[3];
            }
            break;

        case 10:  // Max HP
            if (pUser)
            {
                // TODO: Update ability max
                // CUser::stMyCharInfoEx(pUser)->stAbility.nMaxAbility[0] = (int)fValidValue;
                // TODO: Update party/force MaxHP
            }
            m_nSyncStat[nStatID] = 2;
            if (m_fFinalStat[1] > m_fFinalStat[10])
            {
                m_fFinalStat[1] = m_fFinalStat[10];
                m_nSyncStat[1] = 2;
                // TODO: Update party/force HP
            }
            // TODO: Check area and auto-heal if in non-pvp area
            break;

        case 12:  // Max SG
            if (pUser)
            {
                // TODO: Update ability max
                // CUser::stMyCharInfoEx(pUser)->stAbility.nMaxAbility[1] = (int)fValidValue;
            }
            m_nSyncStat[nStatID] = 2;
            if (m_fFinalStat[2] > m_fFinalStat[12])
            {
                m_fFinalStat[2] = m_fFinalStat[12];
                m_nSyncStat[2] = 2;
                // TODO: Update ability cur
            }
            break;

        case 14:  // Max ST
            if (pUser)
            {
                // TODO: Update ability max
                // CUser::stMyCharInfoEx(pUser)->stAbility.nMaxAbility[3] = (int)fValidValue;
            }
            if (m_fFinalStat[3] > m_fFinalStat[14])
            {
                m_fFinalStat[3] = m_fFinalStat[14];
                m_nSyncStat[3] = 1;
                // TODO: Update ability cur
            }
            break;

        case 16:  // SV
            if (pUser)
            {
                float fOffsetSV = m_fFinalStat[16] - fPrevFinalStat;
                if (m_fFinalStat[16] > fPrevFinalStat)
                {
                    // TODO: CMoverEx::CheckOptionEffectInvoke for SV up
                    // CMoverEx::CheckOptionEffectInvoke(&pUser->CMoverEx, EFFECT_CONDITION_SV_GAGE_UP, &pUser->CMoverEx, fOffsetSV, EFFECT_INVOKE_DONT_CARE);
                    // CMoverEx::ReleaseInvokedOptionEffect(&pUser->CMoverEx, EFFECT_CONDITION_SV_GAGE_UP);
                }
                else if (m_fFinalStat[16] < fPrevFinalStat)
                {
                    // TODO: CMoverEx::CheckOptionEffectInvoke for SV down
                    // CMoverEx::CheckOptionEffectInvoke(&pUser->CMoverEx, EFFECT_CONDITION_SV_GAGE_DOWN, &pUser->CMoverEx, fOffsetSV, EFFECT_INVOKE_DONT_CARE);
                    // CMoverEx::ReleaseInvokedOptionEffect(&pUser->CMoverEx, EFFECT_CONDITION_SV_GAGE_DOWN);
                }
            }
            if (m_fFinalStat[16] > m_fFinalStat[17])
                SetStat(16, m_fFinalStat[17], false);
            break;

        case 18:  // MSR
            if (pUser)
            {
                // TODO: Update ability
                // CUser::stMyCharInfoEx(pUser)->stAbility.fMSR = fValidValue;
            }
            m_nSyncStat[nStatID] = 2;
            break;

        case 19:  // ASR
            if (pUser)
            {
                // TODO: Update ability
                // CUser::stMyCharInfoEx(pUser)->stAbility.fASR = fValidValue;
            }
            m_nSyncStat[nStatID] = 2;
            break;

        default:
            break;
    }

    if (bSync)
        SendUpdateStat(nStatID);
}

// ============================================================================
// SetHP - IDA 0x14003D050
// Verified: Sets HP stat with sync enabled
// ============================================================================
void CGocAttribute::SetHP(float fValue)
{
    SetStat(1, fValue, true);
}

// ============================================================================
// UpdateScaleStat - IDA 0x14003B510
// Verified: Updates scale stat and optionally recalculates
// ============================================================================
void CGocAttribute::UpdateScaleStat(int nStat, float fValue, bool bCalc)
{
    if (static_cast<unsigned int>(nStat) <= 0x4C)
    {
        m_fScaleStat[nStat] += fValue;
        if (bCalc)
        {
            // TODO: CCalculateStatus::CalculateStatus(nStat, this)
            // IDA: v4 = TXSingleton<CCalculateStatus>::Instance();
            //      CCalculateStatus::CalculateStatus(v4, nStat, this);
        }
        else
        {
            m_bCalcStat[nStat] = true;
        }
    }
}

// ============================================================================
// UpdateAddStat - IDA 0x14003B5A0
// Verified: Updates add stat if value non-zero and optionally recalculates
// ============================================================================
void CGocAttribute::UpdateAddStat(int nStat, float fValue, bool bCalc)
{
    if (static_cast<unsigned int>(nStat) <= 0x4C && fValue != 0.0f)
    {
        m_fAddStat[nStat] += fValue;
        if (bCalc)
        {
            // TODO: CCalculateStatus::CalculateStatus(nStat, this)
            // IDA: v4 = TXSingleton<CCalculateStatus>::Instance();
            //      CCalculateStatus::CalculateStatus(v4, nStat, this);
        }
        else
        {
            m_bCalcStat[nStat] = true;
        }
    }
}

// ============================================================================
// UpdateEffectStat - IDA 0x14003B640
// Verified: Updates effect stat based on class type
// ============================================================================
void CGocAttribute::UpdateEffectStat(int nClassType, int nStatType, float fValue, bool bCalc)
{
    if (nClassType)
    {
        SetEquipedOption(nStatType, fValue);
    }
    else
    {
        int nResultStatType = 0;
        bool bAdd = false;
        // TODO: CCalculateStatus::GetStatFromEffect(nStatType, &nResultStatType, &bAdd)
        // IDA: v5 = TXSingleton<CCalculateStatus>::Instance();
        //      CCalculateStatus::GetStatFromEffect(v5, nStatType, &nResultStatType, &bAdd);

        if (nResultStatType)
        {
            if (bAdd)
                UpdateAddStat(nResultStatType, fValue, bCalc);
            else
                UpdateScaleStat(nResultStatType, fValue, bCalc);
        }
        else
        {
            int iIndex = GetSpecialEffectIndex(nStatType);
            if (static_cast<unsigned int>(iIndex) <= 0x36)  // 54
            {
                m_fItemSpecaillEffect[iIndex] += fValue;
                m_bItemSpecialEffectChanged[iIndex] = true;
            }
        }
    }
}

// ============================================================================
// CalculateChangedStat - IDA 0x14003B920
// Verified: Iterates all stats, recalculates changed ones, tracks max values
// ============================================================================
void CGocAttribute::CalculateChangedStat(bool bSend)
{
    for (int i = 0; i < MAX_STAT_COUNT; ++i)
    {
        if (m_bCalcStat[i])
        {
            // TODO: CCalculateStatus::CalculateStatus(i, this)
            // IDA: v2 = TXSingleton<CCalculateStatus>::Instance();
            //      CCalculateStatus::CalculateStatus(v2, i, this);
            m_bCalcStat[i] = false;

            int nStatValue = static_cast<int>(m_fFinalStat[i]);
            if (nStatValue > m_nMaxStat[i])
                m_nMaxStat[i] = nStatValue;
        }
    }

    if (bSend)
        SendUpdateStatList();
}

// ============================================================================
// GetSpecialEffectIndex - IDA 0x14003EEB0
// Verified: Maps item effect type 100-154 to index 0-54, returns -1 otherwise
// ============================================================================
int CGocAttribute::GetSpecialEffectIndex(int iItemEffectType)
{
    if (iItemEffectType >= 100 && iItemEffectType < 155)
        return iItemEffectType - 100;
    return -1;  // 0xFFFFFFFF
}

// ============================================================================
// FPEffect - IDA 0x14003F930
// Verified: Sets FP effect flag and applies +10 attack bonus
// ============================================================================
void CGocAttribute::FPEffect()
{
    m_bFPEffect = true;
    UpdateEffectStat(0, 0x73, 10.0f, true);  // Effect type 115 = 0x73
}

// ============================================================================
// DelFPEffect - IDA 0x14003F970
// Verified: Removes FP effect if active, applies -10 attack penalty
// ============================================================================
void CGocAttribute::DelFPEffect()
{
    if (m_bFPEffect)
    {
        m_bFPEffect = false;
        UpdateEffectStat(0, 0x73, -10.0f, true);
    }
}

// ============================================================================
// GetLevelForStat - IDA 0x140043820
// Verified: Returns mode level if > 0, otherwise base level
// ============================================================================
int CGocAttribute::GetLevelForStat() const
{
    if (m_nModeLv <= 0)
        return m_nLv;
    return m_nModeLv;
}

// ============================================================================
// SetStartRegStat - IDA 0x1400495E0
// Verified: Simple setter for start reg stat flag
// ============================================================================
void CGocAttribute::SetStartRegStat(bool bStart)
{
    m_bStartRegStat = bStart;
}

// ============================================================================
// SetSGRegStat - IDA 0x140049620
// Verified: Sets SG reg stat flag and resets SG update timer
// ============================================================================
void CGocAttribute::SetSGRegStat(bool bEnable)
{
    m_bEnableSGRegStat = bEnable;
    m_fSGUpdateTime = 0.0f;
}

// ============================================================================
// SetGameModeState - IDA 0x140048B60
// Verified: Simple setter for game mode state
// ============================================================================
void CGocAttribute::SetGameModeState(int nState)
{
    m_nGameModeState = nState;
}

// ============================================================================
// GetAwaken - IDA 0x1400444E0
// Verified: Gets awaken value from CUser's character info via RTTI cast
// ============================================================================
std::uint8_t CGocAttribute::GetAwaken() const
{
    // IDA decompiled:
    // v1 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    // pUser = (CUser*)_RTDynamicCast_0(v1, 0, &CMover`RTTI Type Descriptor', &CUser`RTTI Type Descriptor', 0);
    // if (pUser)
    //     return CUser::stMyCharInfoEx(pUser)->stBaseInfo.byAwaken;
    // else
    //     return 0;

    CUser* pUser = GetUser();
    if (pUser)
    {
        // TODO: Get awaken from CUser's character info
        // return CUser::stMyCharInfoEx(pUser)->stBaseInfo.byAwaken;
    }
    return m_byAwaken;
}

// ============================================================================
// ClearSkillOptionEffect - IDA 0x140043A80
// ============================================================================
void CGocAttribute::ClearSkillOptionEffect()
{
    // TODO: m_fSkillOptionAttack/m_fSkillOptionDefense 需要在头文件中声明
    m_mapItemSkilllOption.clear();
}

// ============================================================================
// Helper functions
// ============================================================================

void CGocAttribute::SetFullStat()
{
    SetStat(1, GetStat(10), false);  // HP to max
    SetStat(2, GetStat(12), false);  // SG to max
    SetStat(3, GetStat(14), false);  // ST to max
}

void CGocAttribute::IsValidStat(int nStatID, float* pfValue)
{
    switch (nStatID)
    {
        case 1:  // Current HP
            if (*pfValue > m_fFinalStat[10])
                *pfValue = m_fFinalStat[10];
            else if (*pfValue < 0.0f)
                *pfValue = 0.0f;
            break;
        case 2:  // Current SG
            if (*pfValue > m_fFinalStat[12])
                *pfValue = m_fFinalStat[12];
            else if (*pfValue < 0.0f)
                *pfValue = 0.0f;
            break;
        case 3:  // Current ST
            if (*pfValue > m_fFinalStat[14])
                *pfValue = m_fFinalStat[14];
            else if (*pfValue < 0.0f)
                *pfValue = 0.0f;
            break;
        case 16:  // SV
            if (*pfValue > m_fFinalStat[17])
                *pfValue = m_fFinalStat[17];
            else if (*pfValue < 0.0f)
                *pfValue = 0.0f;
            break;
        case 18:  // MSR
        case 19:  // ASR
            {
                float fMin = m_fOriginStat[nStatID] * 0.1f;
                if (*pfValue < fMin)
                    *pfValue = fMin;
            }
            break;
        default:
            break;
    }
}

// ============================================================================
// SetEquipedOption - IDA 0x140041E80
// Verified: Manages equipped option list, adds or removes options based on value
// ============================================================================
void CGocAttribute::SetEquipedOption(int nStatType, float fValue)
{
    // IDA: Get user via RTTI cast
    // v3 = std::list<CBattleZone*>::size((VChunkLocker*)this);
    // pUser = (CUser*)_RTDynamicCast_0(v3, 0, &CMover`RTTI Type Descriptor', &CUser`RTTI Type Descriptor', 0);

    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // Search for existing option with same ID (IDA verified loop)
    for (auto itor = m_vecEquipedOption.begin(); itor != m_vecEquipedOption.end(); ++itor)
    {
        SEquipedOption* pEquipedOption = static_cast<SEquipedOption*>(*itor);
        if (pEquipedOption && pEquipedOption->dwOptionID == static_cast<std::uint32_t>(nStatType))
        {
            // Check if adding value results in zero (remove option)
            // IDA: if ((float)(pEquipedOption->fOptionValue + fOptionValue) == 0.0)
            if ((pEquipedOption->fOptionValue + fValue) == 0.0f)
            {
                // IDA verified: RemoveOptionEffect from CMoverEx
                // CMoverEx::RemoveOptionEffect(&pUser->CMoverEx, pEquipedOption->dwIndex);

                // Delete the option and remove from vector
                delete pEquipedOption;
                m_vecEquipedOption.erase(itor);
                return;
            }
        }
    }

    // Add new option if value is non-negative (IDA verified condition)
    if (fValue >= 0.0f)
    {
        // IDA: v13 = (SEquipedOption*)VBaseObject::operator new(0xCu);  // 12 bytes
        SEquipedOption* pNewEquipedOption = new SEquipedOption();
        pNewEquipedOption->dwIndex = FindEquipedOptionIndex();
        pNewEquipedOption->dwOptionID = static_cast<std::uint32_t>(nStatType);
        pNewEquipedOption->fOptionValue = fValue;
        m_vecEquipedOption.push_back(pNewEquipedOption);
    }
}

// ============================================================================
// SendUpdateStatList - IDA 0x14003D090
// Verified: Builds two stat lists - one for self, one for broadcast
// ============================================================================
void CGocAttribute::SendUpdateStatList()
{
    // Get owner actor (IDA: std::list<CBattleZone*>::size check)
    XActor* pOwner = GetOwnerActor();
    if (!pOwner)
        return;

    // IDA verified: Build two stat lists
    // ST_UPDATE_STAT_LIST stStatList;        // Sent to self only (m_nSyncStat[i] == 1)
    // ST_UPDATE_STAT_LIST stBroadCastStatList; // Broadcast to nearby (m_nSyncStat[i] == 2)

    // TODO: Define ST_UPDATE_STAT_LIST structure
    // struct ST_UPDATE_STAT {
    //     std::uint16_t wStatID;
    //     float fValue;
    // };
    // struct ST_UPDATE_STAT_LIST {
    //     std::uint32_t dwActorID;
    //     std::vector<ST_UPDATE_STAT> vecUpdateStat;
    // };

    bool bHasSelfStats = false;
    bool bHasBroadcastStats = false;

    for (int i = 0; i < 77; ++i)
    {
        if (m_nSyncStat[i] == 1)
        {
            // Add to self-only stat list
            // stStat.wStatID = i;
            // stStat.fValue = m_fFinalStat[i];
            // stStatList.vecUpdateStat.push_back(stStat);
            bHasSelfStats = true;
        }
        else if (m_nSyncStat[i] == 2)
        {
            // Add to broadcast stat list
            // stStat.wStatID = i;
            // stStat.fValue = m_fFinalStat[i];
            // stBroadCastStatList.vecUpdateStat.push_back(stStat);
            bHasBroadcastStats = true;
        }

        // Reset sync flag after processing
        m_nSyncStat[i] = 0;
    }

    // Send self-only stats
    if (bHasSelfStats)
    {
        // TODO: Implement packet sending
        // XSendPacket xSendPacket(3, 0x34);  // main=3, sub=0x34
        // xSendPacket << 0;  // Some flag
        // xSendPacket << stStatList;
        // CGocNetwork::Send(pOwner, &xSendPacket);
    }

    // Send broadcast stats
    if (bHasBroadcastStats)
    {
        // TODO: Implement packet sending
        // XSendPacket packet(3, 0x34);
        // packet << 0;
        // packet << stBroadCastStatList;
        // CMover* pMover = GetMover();
        // CGocNetwork::SendBroadCast(pMover, &packet, eAll);
    }
}

// ============================================================================
// SendUpdateStat - IDA 0x14003D3B0
// Verified: Sends single stat update, handles stat 21 special case
// ============================================================================
void CGocAttribute::SendUpdateStat(int nStat)
{
    if (nStat < 0 || nStat > 0x4C)
        return;

    XActor* pOwner = GetOwnerActor();
    if (!pOwner)
        return;

    // IDA verified: Build packet with main=3, sub=0x34
    // XSendPacket xSendPacket(3, 0x34);
    // ST_UPDATE_STAT_LIST stStatList;
    // stStatList.dwActorID = pOwner->GetActorID();

    // Add the stat to the list
    // ST_UPDATE_STAT st;
    // st.wStatID = nStat;
    // st.fValue = m_fFinalStat[nStat];
    // stStatList.vecUpdateStat.push_back(st);

    // IDA verified: stat 21 triggers additional stats 35 and 20
    if (nStat == 21)
    {
        // Add stat 35
        // ST_UPDATE_STAT v10;
        // v10.wStatID = 35;
        // v10.fValue = m_fFinalStat[35];
        // stStatList.vecUpdateStat.push_back(v10);

        // Add stat 20
        // ST_UPDATE_STAT v11;
        // v11.wStatID = 20;
        // v11.fValue = m_fFinalStat[20];
        // stStatList.vecUpdateStat.push_back(v11);

        m_nSyncStat[20] = 0;
        m_nSyncStat[35] = 0;
    }

    // IDA verified: Determine send type based on sync stat flag
    if (m_nSyncStat[nStat] == 1)
    {
        // Send to self only
        // xSendPacket << 0;
        // xSendPacket << stStatList;
        // CGocNetwork::Send(pOwner, &xSendPacket);
    }
    else if (m_nSyncStat[nStat] == 2)
    {
        // Broadcast to nearby
        // xSendPacket << 0;
        // xSendPacket << stStatList;
        // CMover* pMover = GetMover();
        // CGocNetwork::SendBroadCast(pMover, &xSendPacket, eAll);
    }

    m_nSyncStat[nStat] = 0;
}
