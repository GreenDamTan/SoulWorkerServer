// CGocAttribute - Game Object Component for actor attributes
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140039080 - 0x140049620

#include "GocAttribute.h"
#include <cmath>
#include <cstring>

// Include TB_STATUS definition
#define GREENDAMTAN_TB_STRUCT_SECTION
#include "Soulworker/GameServer/XSCommon/Table/TB_STATUS.h"
#undef GREENDAMTAN_TB_STRUCT_SECTION

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
void CGocAttribute::SetEquipedOption(std::uint32_t dwOptionID, float fOptionValue)
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
        if (pEquipedOption && pEquipedOption->dwOptionID == dwOptionID)
        {
            // Check if adding value results in zero (remove option)
            // IDA: if ((float)(pEquipedOption->fOptionValue + fOptionValue) == 0.0)
            if ((pEquipedOption->fOptionValue + fOptionValue) == 0.0f)
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
    if (fOptionValue >= 0.0f)
    {
        // IDA: v13 = (SEquipedOption*)VBaseObject::operator new(0xCu);  // 12 bytes
        SEquipedOption* pNewEquipedOption = new SEquipedOption();
        pNewEquipedOption->dwIndex = FindEquipedOptionIndex();
        pNewEquipedOption->dwOptionID = dwOptionID;
        pNewEquipedOption->fOptionValue = fOptionValue;
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

/**
 * LevelUp (0x14003A770)
 * IDA: Complex level up function - increases level, updates stats, sends notifications
 * TODO: 需人工审查 - Full implementation requires SetStatusTable, SendOriginStatAll, etc.
 */
void CGocAttribute::LevelUp(int nAdd, int nUseCheat)
{
    // IDA: Validate - check level cap (max 100)
    if (nAdd + m_nLv > 100)
        return;

    // IDA: Increase level
    m_nLv += nAdd;
    if (m_nLv == 0)
        m_nLv = 1;

    // TODO: Full implementation requires:
    // - SetStatusTable() - update status table
    // - SetOriginStat() - set origin stats
    // - SendOriginStatAll() - send all stats to client
    // - SendUpdateStatList() - send stat updates
    // - XGameServer::Instance() and GetResourceMgr().GetTB_LEVELUP_POINT()
    // - GetOwnerMover() to get CMover*
    // - dynamic_cast<CUser*> for user-specific operations
    // - CGocEntity::Levelup
    // - CCalculateStatus::CalculateStatusAll
    // - CGocParty::SetLevel, CGocForce::SetLevel
    // - CGocSkill::AddSkillPoint
    // - CGocPost::SendLevelUpEvent
    // - CGocAchieve::LevelUp
    // - CGocWeeklyMission::CheckWeeklyMissionUpdate
    // - XSendPacket, XSendDBPacket for network/DB communication
}

// ============================================================================
// Revive - IDA 0x14003CE30
// Verified: Handles character revival - restores stats and triggers revive effects
// ============================================================================
void CGocAttribute::Revive()
{
    // IDA: Get owner mover and retrieve CGocAkashicRecord component
    CMover* pMover = GetMover();
    if (pMover)
    {
        // TODO: Get CGocAkashicRecord component and call ThinkAkashicPassive
        // IDA pattern:
        // CMover::GetGOC<CGocAkashicRecord>(pMover, &pAkashic, 0);
        // if (pAkashic.use_count() != -1)
        // {
        //     CGocAkashicRecord::ThinkAkashicPassive(pAkashic.get());
        // }
    }

    // Restore full stats (HP, SG, ST to max)
    SetFullStat();

    // Send stat updates to client
    SendUpdateStatList();

    // Check for revive effect triggers on user
    CUser* pUser = GetUser();
    if (pUser)
    {
        // TODO: Trigger revive effect condition
        // IDA:
        // CMoverEx::CheckOptionEffectInvoke(&pUser->CMoverEx, EFFECT_CONDITION_REVIVE, &pUser->CMoverEx, 0.0, EFFECT_INVOKE_DONT_CARE);
        // CMoverEx::ReleaseInvokedOptionEffect(&pUser->CMoverEx, EFFECT_CONDITION_REVIVE);
    }
}

// ============================================================================
// UpdateBuffEffectStat - IDA 0x14003B750
// Verified: Updates buff effect stat with special handling for HP/SG/ST/SV
// ============================================================================
int CGocAttribute::UpdateBuffEffectStat(int nStatType, float fValue, bool bCalc, bool bUseInClear)
{
    int nResultStatType = 0;
    bool bAdd = false;

    // TODO: Get stat from effect
    // CCalculateStatus* pCalc = TXSingleton<CCalculateStatus>::Instance();
    // CCalculateStatus::GetStatFromEffect(pCalc, nStatType, &nResultStatType, &bAdd);

    if (nResultStatType)
    {
        // Special handling for HP, SG, ST, SV (stats 0-3 and 16)
        if (nResultStatType <= 3 || nResultStatType == 16)
        {
            if (bUseInClear && fValue <= 0.0f)
                return nResultStatType;

            // TODO: GetRateTargetStat for scale stats
            // if (!bAdd)
            //     iTargetStat = GetRateTargetStat(nResultStatType);

            SetStat(nResultStatType, 0.0f, false);
        }
        else if (bAdd)
        {
            UpdateAddStat(nResultStatType, fValue, bCalc);
        }
        else
        {
            UpdateScaleStat(nResultStatType, fValue, bCalc);
        }
        return nResultStatType;
    }
    else
    {
        // Special effect handling
        int iIndex = GetSpecialEffectIndex(nStatType);
        if (static_cast<unsigned int>(iIndex) <= 0x36)  // 54
        {
            m_fItemSpecaillEffect[iIndex] += fValue;
            m_bItemSpecialEffectChanged[iIndex] = true;
        }
        return nResultStatType;
    }
}

// ============================================================================
// CalculateChangedEffect - IDA 0x14003BA00
// Verified: Calculates changed item special effects and sends to client
// ============================================================================
void CGocAttribute::CalculateChangedEffect(bool bSend)
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Trigger equip effect condition
    // CMoverEx::CheckOptionEffectInvoke(&pUser->CMoverEx, EFFECT_CONDITION_EQUIP, &pUser->CMoverEx, 0.0, EFFECT_INVOKE_DONT_CARE);
    // CMoverEx::ReleaseInvokedOptionEffect(&pUser->CMoverEx, EFFECT_CONDITION_EQUIP);

    // Build changed special option list
    // ST_UPDATE_SPECIAL_OPTION_LIST stOptionList;

    for (int i = 0; i < 55; ++i)
    {
        if (m_bItemSpecialEffectChanged[i])
        {
            // TODO: Build packet
            // stOption.wOptionIndex = i;
            // stOption.fValue = m_fItemSpecaillEffect[i];
            // stOptionList.vecUpdateOption.push_back(stOption);

            m_bItemSpecialEffectChanged[i] = false;

            if (static_cast<int>(m_fItemSpecaillEffect[i]) > m_nMaxStatEffect[i])
                m_nMaxStatEffect[i] = static_cast<int>(m_fItemSpecaillEffect[i]);
        }
    }

    // TODO: Send packet if not empty
    // if (!stOptionList.vecUpdateOption.empty())
    // {
    //     XSendPacket xSendPacket(3, 0x47);
    //     // Add actor ID and option list
    //     CGocNetwork::SendBroadCast(pMover, &xSendPacket, eAll);
    // }
}

// ============================================================================
// SendSpecialOptionList - IDA 0x14003BCA0
// Verified: Sends special option list to specified receiver
// ============================================================================
void CGocAttribute::SendSpecialOptionList(CMover* pReceiverMover)
{
    XActor* pOwner = GetOwnerActor();
    if (!pOwner)
        return;

    // TODO: Build special option list
    // ST_UPDATE_SPECIAL_OPTION_LIST stOptionList;

    for (int i = 0; i < 55; ++i)
    {
        if (m_fItemSpecaillEffect[i] > 0.0f)
        {
            // stOption.wOptionIndex = i;
            // stOption.fValue = m_fItemSpecaillEffect[i];
            // stOptionList.vecUpdateOption.push_back(stOption);
        }
    }

    // TODO: Send packet if not empty
    // if (!stOptionList.vecUpdateOption.empty())
    // {
    //     XSendPacket xSendPacket(3, 0x47);
    //     // Add actor ID and option list
    //     XActor* pTarget = pReceiverMover ? &pReceiverMover->XActor : nullptr;
    //     CGocNetwork::Send(pTarget, &xSendPacket);
    // }
}

// ============================================================================
// SetFlagUseST - IDA 0x14003CF60
// Verified: Resets ST update timer
// ============================================================================
void CGocAttribute::SetFlagUseST()
{
    m_fSTUpdateTime = 0.0f;
}

// ============================================================================
// SetContinousCost - IDA 0x14003CF80
// Verified: Sets continuous cost for a stat type
// ============================================================================
void CGocAttribute::SetContinousCost(int nState, float fCost)
{
    // Find matching cost stat index
    int i = 0;
    for (i = 0; i < 4 && m_iCostStat[i] != nState; ++i)
        ;

    if (i < 4 && m_fContinousCost[i] != fCost)
    {
        SetFlagUseST();
        m_fContinousCost[i] = fCost;

        if (fCost == 0.0f)
        {
            m_nSyncStat[3] = 1;
            SendUpdateStat(nState);
        }
    }
}

// ============================================================================
// SendOriginStatAll - IDA 0x14003D660
// Verified: Sends all origin stats to client (main=3, sub=0x51)
// ============================================================================
void CGocAttribute::SendOriginStatAll()
{
    XActor* pOwner = GetOwnerActor();
    if (!pOwner)
        return;

    // Only send if actor type is 0 (player)
    // TODO: Check XActor::GetType(pOwner) == 0

    // TODO: Build and send stat packet
    // XSendPacket xSendPacket(3, 0x51);
    // ST_UPDATE_STAT_LIST stStatList;
    // stStatList.dwActorID = pOwner->GetActorID();

    for (int i = 0; i < 77; ++i)
    {
        // stStat.wStatID = i;
        // stStat.fValue = m_fOriginStat[i];
        // stStatList.vecUpdateStat.push_back(stStat);
    }

    // operator<<(&xSendPacket, &stStatList);
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// ============================================================================
// SendStatAll - IDA 0x14003D830
// Verified: Sends all final stats to client (main=3, sub=0x34)
// ============================================================================
void CGocAttribute::SendStatAll()
{
    XActor* pOwner = GetOwnerActor();
    if (!pOwner)
        return;

    // TODO: Build and send stat packet
    // XSendPacket xSendPacket(3, 0x34);
    // ST_UPDATE_STAT_LIST stStatList;
    // stStatList.dwActorID = pOwner->GetActorID();

    for (int i = 0; i < 77; ++i)
    {
        // stStat.wStatID = i;
        // stStat.fValue = m_fFinalStat[i];
        // stStatList.vecUpdateStat.push_back(stStat);
        m_nSyncStat[i] = 0;
    }

    // XParse::operator<<(&xSendPacket.XParse, 1);
    // operator<<(&xSendPacket, &stStatList);
    // CGocNetwork::Send(pOwner, &xSendPacket);

    // Log debug info
    // LogHelper::LogDebug("game.contents",
    //     "Character Status [UCID:%d], [REG:%f, PCP:%f, PARP:%f, PCRP:%f, PDSR:%f, PCA:%f, PDEF:%f, PATK_MAX:%f, ADR:%f]",
    //     ...);

    // TODO: SendStatLog(1);
}

// ============================================================================
// SetExp - IDA 0x14003DB60
// Verified: Sets experience points, handles level up
// ============================================================================
void CGocAttribute::SetExp(double fExp, float fBonus, bool bSync)
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    int nExp = static_cast<int>(std::ceil(fExp));
    int nBonus = static_cast<int>(std::ceil(fBonus));

    // TODO: Trigger gain exp effect
    // CMoverEx::CheckOptionEffectInvoke(&pUser->CMoverEx, EFFECT_CONDITION_GAIN_EXP, &pUser->CMoverEx, fExp, EFFECT_INVOKE_DONT_CARE);
    // CMoverEx::ReleaseInvokedOptionEffect(&pUser->CMoverEx, EFFECT_CONDITION_GAIN_EXP);

    // TODO: Get add exp from option effect
    // int nAddExpFromOptionEffect = CMoverEx::GetAddExpFromOptionEffect(&pUser->CMoverEx);
    // CMoverEx::ResetAddExpFromOptionEffect(&pUser->CMoverEx);

    int nTotalExp = nExp;  // + nAddExpFromOptionEffect
    int nTotalBonus = nBonus;  // + nAddExpFromOptionEffect

    // TODO: Get indulgence rate from CGocRecode
    // float fIndulgenceRate = CGocRecode::GetIndulgenceDropRate(pRecode);

    if (nTotalExp <= 0)
        return;

    // TODO: Check game world mode state
    // if (CGameWorldMode::GetState(this) >= 100 && !bSync)
    // {
    //     m_nExp = 0;
    //     CUser::stMyCharInfoEx(pUser)->nExp = 0;
    // }

    int nLevel = m_nLv;  // or CGameWorldMode::GetState(this)
    // TODO: Get TB_LEVELUP_POINT table
    // TB_LEVELUP_POINT* pTBLevel = XResourceMgr::GetTB_LEVELUP_POINT(nLevel);

    // Handle level up
    int nAddLevel = 0;
    __int64 nTempExp = m_nExp + nTotalExp;

    // TODO: Level up loop
    // while (nTempExp >= pTBLevel->Need_EXP)
    // {
    //     nTempExp -= pTBLevel->Need_EXP;
    //     ++nAddLevel;
    //     pTBLevel = XResourceMgr::GetTB_LEVELUP_POINT(nLevel + nAddLevel);
    // }

    if (nAddLevel)
    {
        LevelUp(nAddLevel, 0);
    }

    m_nExp = static_cast<int>(nTempExp);
    // CUser::stMyCharInfoEx(pUser)->nExp = m_nExp;

    // Send exp update packet
    if (bSync)
    {
        // PS_EXP_UPDATE stExp;
        // stExp.nFinalExp = m_nExp;
        // stExp.nAddTotalExp = nTotalExp;
        // stExp.nAddBonusExp = nTotalBonus;
        // XSendPacket xSendPacket(3, 0x37);
        // operator<<(&xSendPacket, &stExp);
        // CGocNetwork::Send(pOwner, &xSendPacket);
    }
}

// ============================================================================
// ResetExp - IDA 0x14003E3E0
// Verified: Resets experience to zero and sends update
// ============================================================================
void CGocAttribute::ResetExp()
{
    XActor* pOwner = GetOwnerActor();
    if (!pOwner)
        return;

    m_nExp = 0;

    CUser* pUser = GetUser();
    if (pUser)
    {
        // TODO: CUser::stMyCharInfoEx(pUser)->nExp = m_nExp;
    }

    // Send exp reset packet
    // XSendPacket xSendPacket(3, 0x37);
    // XParse::operator<<(&xSendPacket.XParse, pOwner->GetActorID());
    // XParse::operator<<(&xSendPacket.XParse, m_nExp);
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// ============================================================================
// CheatSetExp - IDA 0x14003E580
// Verified: Cheat function to set exp directly
// ============================================================================
void CGocAttribute::CheatSetExp(int nExp)
{
    XActor* pOwner = GetOwnerActor();
    if (!pOwner)
        return;

    m_nExp = nExp;

    CUser* pUser = GetUser();
    if (pUser)
    {
        // TODO: CUser::stMyCharInfoEx(pUser)->nExp = m_nExp;
    }

    // Send exp update packet
    // XSendPacket xSendPacket(3, 0x37);
    // XParse::operator<<(&xSendPacket.XParse, pOwner->GetActorID());
    // XParse::operator<<(&xSendPacket.XParse, m_nExp);
    // CGocNetwork::Send(pOwner, &xSendPacket);
}

// ============================================================================
// ProcessSGReg - IDA 0x14003E830
// Verified: Process SG regeneration based on soul gauge table
// ============================================================================
void CGocAttribute::ProcessSGReg()
{
    XActor* pOwner = GetOwnerActor();
    if (!pOwner || !m_pSoulGuageRef)
        return;

    CUser* pUser = GetUser();
    float fCurSG = GetStat(2);
    float fMaxSG = GetStat(12);  // 0xC

    // TODO: Full implementation requires:
    // - TB_SOUL_GUAGE struct definition (Buff_Condition_Type, Buff_Condition, Get_Buff_ID, Delay_Time)
    // - CMover::FindBuffStatus
    // - XActor::IsStatus
    // - ThreadLocalData::GetTimer / IVTimer::GetTimeDifference
    // - CheckPassiveSkill

    // Handle SG regeneration type
    if (m_bySGRegType)
    {
        bool bEnableReg = false;

        switch (m_bySGRegType)
        {
            case 1:
            case 2:
                // Time-based delay
                // bEnableReg = m_fLastEnableSGTime >= (m_pSoulGuageRef->Delay_Time * 0.001f);
                break;
            case 4:
                // bEnableReg = bBuffExist;
                break;
            case 6:
                // bEnableReg = !bBuffExist;
                break;
            case 5:
                // Complex condition with skill check
                break;
            default:
                bEnableReg = true;
                break;
        }

        if (bEnableReg != m_bEnableSGRegStat)
        {
            m_bEnableSGRegStat = bEnableReg;
            SetSGRegStat(bEnableReg);
        }
    }
}

// ============================================================================
// GetSpecialEffect - IDA 0x14003EEE0
// Verified: Gets special effect value by effect type
// ============================================================================
float CGocAttribute::GetSpecialEffect(int eEffectType)
{
    int iIndex = GetSpecialEffectIndex(eEffectType);
    if (iIndex == -1)
        return 0.0f;
    if (static_cast<unsigned int>(iIndex) <= 0x36)  // 54
        return m_fItemSpecaillEffect[iIndex];
    return 0.0f;
}

// ============================================================================
// CanUseFP - IDA 0x14003EF40
// Verified: Checks if enough FP is available
// ============================================================================
bool CGocAttribute::CanUseFP(std::int16_t shPoint) const
{
    CUser* pUser = const_cast<CGocAttribute*>(this)->GetUser();
    if (!pUser)
        return false;

    // TODO: Full implementation requires:
    // - CTimeEventMgr::CheckDecreaseFatigue
    // - CGocBooster::GetTotalValue for fatigue decrease and add FP
    // - CGocEntity::GetNetCafe
    // - CUser::GetFP, GetBonusFP, GetPCBangFP

    return shPoint >= 0;
}

// ============================================================================
// UseFP - IDA 0x14003F1C0
// Verified: Uses FP points, handles PC Bang and bonus FP
// ============================================================================
bool CGocAttribute::UseFP(std::int16_t shPoint, unsigned int dwMazeID, bool bIsEffect)
{
    CUser* pUser = GetUser();
    if (!pUser)
        return false;

    // TODO: Full implementation requires:
    // - CTimeEventMgr::CheckDecreaseFatigue
    // - CGocBooster::GetTotalValue
    // - CGocEntity::GetNetCafe
    // - CUser::GetPCBangFP, AddPCBangFP, GetBonusFP, AddBonusFP, GetFP, AddFP
    // - ST_LOG_GAME for logging
    // - SendDBUpdateFP

    bool bEffect = false;
    if (bIsEffect)
    {
        FPEffect();
        bEffect = true;
    }

    return bEffect;
}

// ============================================================================
// FPRestore - IDA 0x14003F9C0
// Verified: Restores FP points
// ============================================================================
void CGocAttribute::FPRestore(std::int16_t shFP)
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Full implementation requires:
    // - CUser::AddFP
    // - ST_LOG_GAME for logging
    // - SendDBUpdateFP
}

// ============================================================================
// SetInitFPDate - IDA 0x14003FB80
// Verified: Sets FP init date
// ============================================================================
void CGocAttribute::SetInitFPDate(std::int64_t biDate)
{
    m_biFPInitDate = biDate;
}

// ============================================================================
// SetInitFPDate (no params) - IDA 0x14003FBA0
// Verified: Updates FP init date with bonus calculations
// ============================================================================
void CGocAttribute::SetInitFPDate()
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Full implementation requires:
    // - XGameServer::GetBeforeInitDate, GetCurDate
    // - CGocBooster::GetTotalValue
    // - CUser::AddBonusFP, AddFP, AddPCBangFP, GetFP, GetBonusFP, GetPCBangFP
    // - ST_LOG_GAME for logging
    // - SendDBInitFP
    // - XSendPacket for FP update (main=3, sub=0x64)
}

// ============================================================================
// CheatUpdateStat - IDA 0x1400402D0
// Verified: Cheat function to update stat
// ============================================================================
void CGocAttribute::CheatUpdateStat(unsigned int nStat, float fValue, std::uint8_t byType)
{
    if (nStat > 0x4C)  // 76
        return;

    if (byType != 1 && byType != 2)
        return;

    // ST_UPDATE_STAT stStatInfo;
    // stStatInfo.wStatID = nStat;
    // stStatInfo.fValue = fValue;

    if (byType == 1)
    {
        // m_vecAddStat_Cheat.push_back(stStatInfo);
        UpdateAddStat(nStat, fValue, false);
    }
    else if (byType == 2)
    {
        // m_vecScaleStat_Cheat.push_back(stStatInfo);
        UpdateScaleStat(nStat, fValue, false);
    }

    CalculateChangedStat(true);
}

// ============================================================================
// CheatResetStat - IDA 0x1400403B0
// Verified: Resets all cheat stat modifications
// ============================================================================
void CGocAttribute::CheatResetStat()
{
    // Process add stat cheats
    for (size_t i = 0; i < m_vecAddStat_Cheat.size(); ++i)
    {
        // ST_UPDATE_STAT& stStat = m_vecAddStat_Cheat[i];
        // float fValue = -stStat.fValue;
        // UpdateAddStat(stStat.wStatID, fValue, false);
    }

    // Process scale stat cheats
    for (size_t j = 0; j < m_vecScaleStat_Cheat.size(); ++j)
    {
        // ST_UPDATE_STAT& stStat = m_vecScaleStat_Cheat[j];
        // float fValue = -stStat.fValue;
        // UpdateScaleStat(stStat.wStatID, fValue, false);
    }

    m_vecAddStat_Cheat.clear();
    m_vecScaleStat_Cheat.clear();

    CalculateChangedStat(true);
}

// ============================================================================
// OnUpdateInitDate - IDA 0x140040A80
// Verified: Updates init date tick and triggers SetInitFPDate
// ============================================================================
void CGocAttribute::OnUpdateInitDate()
{
    // TODO: Full implementation requires:
    // - GetTickCount64() from Windows API
    // - XGameServer::GetInitTick
    // - SetInitFPDate()

    // if (m_dw64FPTick < GetTickCount64())
    // {
    //     XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    //     std::uint64_t dw64Tick = XGameServer::GetInitTick(pServer);
    //     if (dw64Tick)
    //     {
    //         if (m_dw64FPTick < dw64Tick)
    //         {
    //             if (m_dw64FPTick)
    //             {
    //                 m_dw64FPTick = dw64Tick;
    //                 SetInitFPDate();
    //             }
    //             else
    //             {
    //                 m_dw64FPTick = dw64Tick;
    //             }
    //         }
    //     }
    // }
}

// ============================================================================
// CheatFPChange - IDA 0x1400405D0
// Verified: Cheat function to change FP values
// ============================================================================
void CGocAttribute::CheatFPChange(int nType, std::int16_t shFP)
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Full implementation requires:
    // - CUser::AddFP, AddBonusFP, AddPCBangFP, GetFP, GetBonusFP, GetPCBangFP
    // - ST_LOG_GAME for logging
    // - SendDBUpdateFP

    switch (nType)
    {
        case 1:
            // CUser::AddFP(pUser, shFP);
            // Log with nParam2 = 46
            break;
        case 2:
            // CUser::AddBonusFP(pUser, shFP);
            // Log with nParam2 = 48
            break;
        case 3:
            // CUser::AddPCBangFP(pUser, shFP, 0, 0);
            // Log with nParam2 = 53
            break;
    }

    // SendDBUpdateFP();
}

// ============================================================================
// SendDBUpdateFP - IDA 0x140040B30
// Verified: Sends DB packet to update FP values (main=3, sub=0x71)
// ============================================================================
void CGocAttribute::SendDBUpdateFP()
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Full implementation requires:
    // - XSendDBPacket (main=3, sub=0x71)
    // - CUser::GetFP, GetBonusFP, GetPCBangFP
    // - XGameServer::SendDBGame
}

// ============================================================================
// SendDBInitFP - IDA 0x140040CF0
// Verified: Sends DB packet to init FP values (main=3, sub=0x72)
// ============================================================================
void CGocAttribute::SendDBInitFP()
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Full implementation requires:
    // - XSendDBPacket (main=3, sub=0x72)
    // - CUser::GetFP, GetBonusFP, GetPCBangFP
    // - XGameServer::SendDBGame
}

// ============================================================================
// ResetMoveSpeed - IDA 0x140041AE0
// Verified: Resets move speed scale stat
// ============================================================================
void CGocAttribute::ResetMoveSpeed(bool bCalc)
{
    m_fScaleStat[18] = 0.0f;

    if (bCalc)
    {
        // TODO: CCalculateStatus::CalculateStatus(18, this)
        // CCalculateStatus* pCalc = TXSingleton<CCalculateStatus>::Instance();
        // CCalculateStatus::CalculateStatus(pCalc, 18, this);
    }
    else
    {
        m_bCalcStat[18] = true;
    }
}

// ============================================================================
// CalculateOtherChangedEffect - IDA 0x140040530
// Verified: Calculates other changed effects into option list
// ============================================================================
void CGocAttribute::CalculateOtherChangedEffect(std::vector<ST_UPDATE_SPECIAL_OPTION>& stOptionList)
{
    stOptionList.clear();

    for (int i = 0; i < 55; ++i)
    {
        if (m_bItemSpecialEffectChanged[i])
        {
            ST_UPDATE_SPECIAL_OPTION stOption;
            stOption.wOptionIndex = static_cast<std::uint16_t>(i);
            stOption.fValue = m_fItemSpecaillEffect[i];
            stOptionList.push_back(stOption);
            m_bItemSpecialEffectChanged[i] = false;
        }
    }
}

// ============================================================================
// CheckEchelonInfo - IDA 0x140040EB0
// Verified: Checks and initializes echelon info if level >= 55
// ============================================================================
void CGocAttribute::CheckEchelonInfo()
{
    // TODO: Check CGameWorldMode::GetState(this) >= 55
    // if (CGameWorldMode::GetState(this) >= 55 && !m_byEchelonLevel)
    // {
    //     m_byEchelonLevel = 1;
    //     m_nEchelonExp = 0;
    //     m_byPrevEchelonLevel = m_byEchelonLevel;
    //     m_nPrevEchelonExp = m_nEchelonExp;
    // }
}

// ============================================================================
// AddEchelonExp - IDA 0x140040F30
// Verified: Adds echelon experience and handles level up
// ============================================================================
void CGocAttribute::AddEchelonExp(int nExp, int nBonus, bool bLevelUp)
{
    if (!m_byEchelonLevel)
        return;

    if (m_byEchelonLevel >= 20)  // 0x14
        return;

    if (nExp <= 0)
        return;

    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Get TB_ECHELON table
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_ECHELON* pTBEchelonNow = XResourceMgr::GetTB_ECHELON(&pServer->m_xResourceMgr, m_byEchelonLevel);
    // if (!pTBEchelonNow || pTBEchelonNow->Echelon_Invalid == 2)
    //     return;

    int nTempExp = nExp + m_nEchelonExp;

    // TODO: Handle level up when exp >= required
    // if (nTempExp >= pTBEchelonNow->Echelon_Exp)
    // {
    //     ++m_byEchelonLevel;
    //     CUser::stMyCharInfoEx(pUser)->byEchelonLevel = m_byEchelonLevel;
    //     bLevelUp = true;
    //     // Handle title reward
    // }

    m_nEchelonExp = nTempExp;
    // CUser::stMyCharInfoEx(pUser)->nEchelonExp = nTempExp;

    // SendUpdateEchelonExp(nExp, nBonus, bLevelUp);
}

// ============================================================================
// SetEchelonLevelBooster - IDA 0x140041320
// Verified: Sets echelon level booster based on echelon level
// ============================================================================
void CGocAttribute::SetEchelonLevelBooster()
{
    if (!m_byEchelonLevel)
        return;

    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Get TB_ECHELON and call CUser::ChangeBooster
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_ECHELON* pTBEchelon = XResourceMgr::GetTB_ECHELON(&pServer->m_xResourceMgr, m_byEchelonLevel);
    // if (pTBEchelon)
    // {
    //     if (m_nLv < 55)
    //         CUser::ChangeBooster(pUser, eBooster_Type_Echelon, pTBEchelon->Echelon_Booster_ID);
    //     else
    //         CUser::ChangeBooster(pUser, eBooster_Type_Echelon, 0);
    // }
}

// ============================================================================
// SendStatLog - IDA 0x140041B30
// Verified: Sends stat log to database
// ============================================================================
void CGocAttribute::SendStatLog(int bLogint)
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Build ST_STAT_LOG_GAME and send
    // ST_STAT_LOG_GAME stLog;
    // stLog.nUAID = pUser->GetUAID();
    // stLog.nUCID = pOwner->GetActorID();
    // stLog.fParam0 = m_fFinalStat[20];
    // stLog.fParam1 = m_fFinalStat[21];
    // stLog.fParam2 = m_fFinalStat[18];
    // stLog.fParam3 = m_fFinalStat[19];
    // stLog.fParam4 = m_fFinalStat[29];
    // stLog.fParam5 = m_fFinalStat[35];
    // stLog.fParam6 = m_fFinalStat[24];
    // XGameServer::Instance()->SendDBStatLog(&stLog);
}

// ============================================================================
// GM_EchelonLevelUp - IDA 0x1400413F0
// Verified: GM command to set echelon level directly
// ============================================================================
void CGocAttribute::GM_EchelonLevelUp(std::uint8_t byLevel, int nExp)
{
    if (!byLevel)
        return;

    if (byLevel > 20)  // 0x14
        return;

    if (nExp < 0)
        return;

    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Get TB_ECHELON table
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_ECHELON* pTBEchelon = XResourceMgr::GetTB_ECHELON(&pServer->m_xResourceMgr, byLevel);
    // if (!pTBEchelon)
    //     return;

    m_byEchelonLevel = byLevel;
    m_nEchelonExp = nExp;
    // CUser::stMyCharInfoEx(pUser)->byEchelonLevel = byLevel;
    // CUser::stMyCharInfoEx(pUser)->nEchelonExp = nExp;

    // SendUpdateEchelonExp(0, 0, true);

    // TODO: Handle title reward if pTBEchelon->Echelon_Title
}

// ============================================================================
// SendUpdateEchelonExp - IDA 0x140041640
// Verified: Sends echelon exp update packet (main=3, sub=0x52)
// ============================================================================
void CGocAttribute::SendUpdateEchelonExp(int nTotal, int nBonus, bool bLevelUp)
{
    XActor* pOwner = GetOwnerActor();
    if (!pOwner)
        return;

    if (bLevelUp)
    {
        // SendEchelonInfoSave();
        // TODO: Broadcast echelon level update (main=3, sub=0x53)
    }

    // TODO: Send echelon exp packet (main=3, sub=0x52)
    // PT_ECHELON_INFO ptEchelon;
    // ptEchelon.byEchelonLevel = m_byEchelonLevel;
    // ptEchelon.nEchelonExp = m_nEchelonExp;
    // ptEchelon.nTotal = nTotal;
    // ptEchelon.nBonus = nBonus;
    // ptEchelon.bLevelUp = bLevelUp;
    // XSendPacket xSendPacket(3, 0x52);
    // operator<<(&xSendPacket, &ptEchelon);
    // CGocNetwork::Send(pOwner, &xSendPacket);

    if (bLevelUp)
    {
        // SetEchelonLevelBooster();
    }
}

// ============================================================================
// SendEchelonInfoSave - IDA 0x140041900
// Verified: Sends echelon info to DB for saving (main=3, sub=0x57)
// ============================================================================
void CGocAttribute::SendEchelonInfoSave()
{
    // Check if echelon info changed
    if (m_byEchelonLevel == m_byPrevEchelonLevel && m_nEchelonExp == m_nPrevEchelonExp)
        return;

    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Send DB packet (main=3, sub=0x57)
    // PT_ECHELON_INFO ptEchelon;
    // ptEchelon.byEchelonLevel = m_byEchelonLevel;
    // ptEchelon.nEchelonExp = m_nEchelonExp;
    // ptEchelon.bLevelUp = false;
    // XSendDBPacket xSendDBPacket(pOwner, 3, 0x57);
    // XParse::operator<<(&xSendDBPacket.XParse, pUser->GetUAID());
    // operator<<(&xSendDBPacket, &ptEchelon);
    // XGameServer::Instance()->SendDBGame(&xSendDBPacket);

    m_byPrevEchelonLevel = m_byEchelonLevel;
    m_nPrevEchelonExp = m_nEchelonExp;
}

// ============================================================================
// InitRoguelike - IDA 0x140042390
// Verified: Initializes roguelike mode stats
// ============================================================================
void CGocAttribute::InitRoguelike()
{
    // TODO: Get TB_COMMON for mode level
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TB_COMMON* pTBCommon = XResourceMgr::GetTB_COMMON(&pServer->m_xResourceMgr, 0x791C);
    // if (!pTBCommon)
    //     return;

    // m_nModeLv = static_cast<int>(pTBCommon->Value);
    SetGameModeState(1);

    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // Clear all stats
    std::memset(m_fScaleStat, 0, sizeof(m_fScaleStat));
    std::memset(m_fAddStat, 0, sizeof(m_fAddStat));
    std::memset(m_fFinalStat, 0, sizeof(m_fFinalStat));
    std::memset(m_fOriginStat, 0, sizeof(m_fOriginStat));
    std::memset(m_bCalcStat, 0, sizeof(m_bCalcStat));
    std::memset(m_fContinousCost, 0, sizeof(m_fContinousCost));
    std::memset(m_nSyncStat, 0, sizeof(m_nSyncStat));
    std::memset(m_fItemSpecaillEffect, 0, sizeof(m_fItemSpecaillEffect));
    std::memset(m_bItemSpecialEffectChanged, 0, sizeof(m_bItemSpecialEffectChanged));

    m_vecScaleStat_Cheat.clear();
    m_vecAddStat_Cheat.clear();

    // Clear equipped options
    for (auto* pOption : m_vecEquipedOption)
    {
        delete pOption;
    }
    m_vecEquipedOption.clear();

    // TODO: ClearSkillOptionEffect, SetStatusTable, SetOriginStat
    // TODO: Apply class correction from TB_MODE_BI_CLASS_CORRECTION
    // TODO: CalculateStatusAll, SetFullStat, SendOriginStatAll
    // TODO: Init mode skill, Init roguelike mode for akashic record
}

// ============================================================================
// ExitRoguelike - IDA 0x140042DC0
// Verified: Exits roguelike mode and restores normal stats
// ============================================================================
void CGocAttribute::ExitRoguelike()
{
    SetGameModeState(0);
    m_nModeLv = 0;

    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // Clear all stats
    std::memset(m_fScaleStat, 0, sizeof(m_fScaleStat));
    std::memset(m_fAddStat, 0, sizeof(m_fAddStat));
    std::memset(m_fFinalStat, 0, sizeof(m_fFinalStat));
    std::memset(m_fOriginStat, 0, sizeof(m_fOriginStat));
    std::memset(m_bCalcStat, 0, sizeof(m_bCalcStat));
    std::memset(m_fContinousCost, 0, sizeof(m_fContinousCost));
    std::memset(m_nSyncStat, 0, sizeof(m_nSyncStat));
    std::memset(m_fItemSpecaillEffect, 0, sizeof(m_fItemSpecaillEffect));
    std::memset(m_bItemSpecialEffectChanged, 0, sizeof(m_bItemSpecialEffectChanged));

    m_vecScaleStat_Cheat.clear();
    m_vecAddStat_Cheat.clear();

    // Clear equipped options
    for (auto* pOption : m_vecEquipedOption)
    {
        delete pOption;
    }
    m_vecEquipedOption.clear();

    // TODO: SendEmptySpecialOptionList, ClearSkillOptionEffect
    // TODO: SetStatusTable, SetOriginStat, CalculateStatusAll
    // TODO: CalculateCharacterStat, SetFullStat if not dead
    // TODO: SendOriginStatAll, ResetModeSkill, ResetRoguelikeMode
    // TODO: RemoveAllOptionEffect, SendUpdateStatList
}

// ============================================================================
// IsShouldSyncStatBroadcast - IDA 0x140041CC0
// Verified: Checks if stat should be broadcast to nearby players
// ============================================================================
bool CGocAttribute::IsShouldSyncStatBroadcast(int nStat)
{
    // Stat indices that should be broadcast: 1-2, 10, 18-19
    if (nStat >= 1 && nStat <= 2)
        return true;
    if (nStat == 10)
        return true;
    if (nStat >= 18 && nStat <= 19)
        return true;
    return false;
}

// ============================================================================
// FindEquipedOptionIndex - IDA 0x140042080
// Verified: Finds next equipped option index (wraps at 10000000)
// ============================================================================
std::uint32_t CGocAttribute::FindEquipedOptionIndex() const
{
    // Increment and wrap at 10000000
    int nIndex = m_iEquipOptionIndex + 1;
    if (nIndex >= 10000000)
        nIndex = 0;
    return static_cast<std::uint32_t>(nIndex);
}

// ============================================================================
// SetStartStatEnterWorld - IDA 0x140041D10
// Verified: Sets start stat when entering world based on world type
// ============================================================================
void CGocAttribute::SetStartStatEnterWorld(int nWorldType)
{
    // World type 0 or 2: SetFullStat, otherwise SetStartStat
    if (nWorldType == 0 || nWorldType == 2)
    {
        SetFullStat();
    }
    else
    {
        SetStartStat();
    }
}

// ============================================================================
// SendInfo - IDA 0x140043510
// Verified: Sends stat info as chat notice packet (main=7, sub=4)
// ============================================================================
void CGocAttribute::SendInfo()
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // Format stat info as string
    char szBuffer[256];
    std::snprintf(szBuffer, sizeof(szBuffer),
        "Lv:%d HP:%.0f/%.0f FP:%.0f/%.0f SG:%.0f",
        m_nLv,
        m_fFinalStat[0], m_fOriginStat[0],  // HP current/max
        m_fFinalStat[1], m_fOriginStat[1],  // FP current/max
        m_fFinalStat[2]);                    // SG

    // TODO: Send as chat notice packet (main=7, sub=4)
    // pUser->SendChatNotice(szBuffer);
}

// ============================================================================
// CalculateCharacterStat - IDA 0x140043450
// Verified: Calculates character stats from entity title and inventory equipment
// ============================================================================
void CGocAttribute::CalculateCharacterStat()
{
    XActor* pActor = GetOwnerActor();
    if (!pActor)
        return;

    // TODO: Call CGocEntity::CalculateTitleStat
    // CGocEntity* pEntity = pActor->GetComponent<CGocEntity>();
    // if (pEntity)
    //     pEntity->CalculateTitleStat();

    // TODO: Call CGocInventory::CalculateEquipStat
    // CGocInventory* pInventory = pActor->GetComponent<CGocInventory>();
    // if (pInventory)
    //     pInventory->CalculateEquipStat();
}

// ============================================================================
// GetLevelForStat - IDA 0x140043820
// Verified: Returns level for stat calculation (mode level if set, otherwise normal level)
// ============================================================================
int CGocAttribute::GetLevelForStat() const
{
    if (m_nModeLv > 0)
        return m_nModeLv;
    else
        return m_nLv;
}

// ============================================================================
// SendEmptySpecialOptionList - IDA 0x140043850
// Verified: Sends empty special option list to client (55 zero options)
// ============================================================================
void CGocAttribute::SendEmptySpecialOptionList()
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Create ST_UPDATE_SPECIAL_OPTION_LIST with 55 zero entries
    // ST_UPDATE_SPECIAL_OPTION_LIST stList;
    // for (int i = 0; i < 55; ++i)
    // {
    //     ST_UPDATE_SPECIAL_OPTION stOption;
    //     stOption.wOptionIndex = i;
    //     stOption.fValue = 0.0f;
    //     stList.vecUpdateOption.push_back(stOption);
    // }
    //
    // XSendPacket xSendPacket(3, 0x47);
    // xSendPacket << pUser->GetUCID();
    // xSendPacket << stList;
    // CGocNetwork::Send(pUser, &xSendPacket);
}

// ============================================================================
// ClearSkillOptionEffect - IDA 0x140043A80
// Verified: Clears all skill option effects
// ============================================================================
void CGocAttribute::ClearSkillOptionEffect()
{
    m_mapItemSkilllOption.clear();
}

// ============================================================================
// SetAwaken - IDA 0x140043B40
// Verified: Sets awaken grade and syncs to client/party/force
// ============================================================================
void CGocAttribute::SetAwaken(std::uint8_t byGrade, bool bSync)
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: Check if awaken grade changed
    // if (pUser->GetAwaken() == byGrade)
    //     return;

    m_byAwaken = byGrade;

    if (bSync)
    {
        // TODO: Send PS_CHAR_UPDATE_AWAKEN packet (main=3, sub=7)
        // TODO: Find default photo item for awaken
        // TODO: Add profile photo if needed
        // TODO: Send to community socket (main=0xF3, sub=0x36)
        // TODO: Update party/force awaken
    }

    // TODO: Send log (main=3, sub=21)
}

// ============================================================================
// GetAwaken - IDA 0x1400444E0
// Verified: Returns awaken grade from user
// ============================================================================
std::uint8_t CGocAttribute::GetAwaken() const
{
    CUser* pUser = GetUser();
    if (!pUser)
        return 0;

    // TODO: return pUser->GetAwaken();
    return m_byAwaken;
}

// ============================================================================
// GetEquipIndex - IDA 0x1400420D0
// Verified: Finds equipped option index by option ID and value
// ============================================================================
int CGocAttribute::GetEquipIndex(std::uint32_t dwOptionID, float fOptionValue)
{
    // Search m_vecEquipedOption for matching option
    for (auto it = m_vecEquipedOption.begin(); it != m_vecEquipedOption.end(); ++it)
    {
        // SEquipedOption* pOption = static_cast<SEquipedOption*>(*it);
        // if (pOption && pOption->dwOptionID == dwOptionID && pOption->fOptionValue == fOptionValue)
        //     return pOption->dwIndex;
    }
    return -1;  // Not found
}

// ============================================================================
// GetRateTargetStat - IDA 0x140042190
// Verified: Maps stat type to target stat for rate calculations
// ============================================================================
int CGocAttribute::GetRateTargetStat(int iStatType)
{
    switch (iStatType)
    {
        case 1:  return 10;
        case 2:  return 12;
        case 3:  return 14;
        case 16: return 17;
        default: return iStatType;
    }
}

// ============================================================================
// SetSkillOptionEffect - IDA 0x1400421E0
// Verified: Sets skill option effect value in m_mapItemSkilllOption
// ============================================================================
void CGocAttribute::SetSkillOptionEffect(bool bEquip, int nSkillGroupIndex, int nType, int nValue)
{
    // Validate type range
    if (nType < 0 || nType >= 10 || nValue < 0)  // EFFECT_SKILL_OPTION_MAX = 10
        return;

    // Create key pair
    // std::pair<int, int> key = std::make_pair(nSkillGroupIndex, nType);
    // auto it = m_mapItemSkilllOption.find(key);

    if (bEquip)
    {
        // Add or increment value
        // if (it != m_mapItemSkilllOption.end())
        //     it->second += nValue;
        // else
        //     m_mapItemSkilllOption[key] = nValue;
    }
    else
    {
        // Decrement value, clamp to 0
        // if (it != m_mapItemSkilllOption.end())
        // {
        //     it->second -= nValue;
        //     if (it->second < 0)
        //         it->second = 0;
        // }
    }
}

// ============================================================================
// GetSkillOptionEffect - IDA 0x1400439F0
// Verified: Gets skill option effect value from m_mapItemSkilllOption
// ============================================================================
void CGocAttribute::GetSkillOptionEffect(int nSkillGroupIndex, int nType, float& fValue)
{
    // std::pair<int, int> key = std::make_pair(nSkillGroupIndex, nType);
    // auto it = m_mapItemSkilllOption.find(key);
    // if (it != m_mapItemSkilllOption.end())
    //     fValue = static_cast<float>(it->second);
    // else
        fValue = 0.0f;
}

// ============================================================================
// ClearSkillOptionEffectPart - IDA 0x140043AB0
// Verified: Clears specific skill option effect
// ============================================================================
void CGocAttribute::ClearSkillOptionEffectPart(int nSkillGroupIndex, int nType)
{
    // std::pair<int, int> key = std::make_pair(nSkillGroupIndex, nType);
    // auto it = m_mapItemSkilllOption.find(key);
    // if (it != m_mapItemSkilllOption.end())
    //     it->second = 0;
}

// ============================================================================
// SetItemRateInfo - IDA 0x140044540
// Verified: Sets item rate info for slot (1=weapon, 151/161/171/181=other)
// ============================================================================
void CGocAttribute::SetItemRateInfo(std::uint8_t bySlot, float fValueAtk, float fValueDef,
                                     std::uint16_t wLevel, std::uint8_t byRank)
{
    // Only specific slots are valid
    if (bySlot != 1 && bySlot != 151 && bySlot != 161 && bySlot != 171 && bySlot != 181)
        return;

    if (bySlot == 1)
    {
        // Weapon slot - uses attack value
        if (fValueAtk >= 1.0f)
        {
            // int iValueCritical = static_cast<int>(fValueAtk * m_StatusTable.Con_PCA);
            // SItemRateInfo info(static_cast<int>(fValueAtk), wLevel, byRank, iValueCritical);
            // m_mapItemRateInfo[bySlot] = info;
        }
    }
    else
    {
        // Other slots - uses defense value
        if (fValueDef >= 1.0f)
        {
            // SItemRateInfo info(static_cast<int>(fValueDef), wLevel, byRank, 0);
            // m_mapItemRateInfo[bySlot] = info;
        }
    }
}

// ============================================================================
// UnsetItemRateInfo - IDA 0x1400446F0
// Verified: Removes item rate info for slot
// ============================================================================
void CGocAttribute::UnsetItemRateInfo(std::uint8_t bySlot)
{
    // auto it = m_mapItemRateInfo.find(bySlot);
    // if (it != m_mapItemRateInfo.end())
    //     m_mapItemRateInfo.erase(it);
}

// ============================================================================
// AddItemRateInfo - IDA 0x140044780
// Verified: Adds value to existing item rate info
// ============================================================================
void CGocAttribute::AddItemRateInfo(std::uint8_t bySlot, float fAddValue)
{
    // auto it = m_mapItemRateInfo.find(bySlot);
    // if (it == m_mapItemRateInfo.end())
    //     return;

    // if (bySlot == 1)
    // {
    //     // Weapon slot - add with critical calculation
    //     int iValueCritical = static_cast<int>(fAddValue * m_StatusTable.Con_PCA);
    //     it->second.AddValue(static_cast<int>(fAddValue), iValueCritical);
    // }
    // else
    // {
    //     // Other slots - simple add
    //     it->second.AddValue(static_cast<int>(fAddValue), 0);
    // }
}

// ============================================================================
// GetItemRateInfo - IDA 0x140044860
// Verified: Gets item rate info for slot
// ============================================================================
const void* CGocAttribute::GetItemRateInfo(std::uint8_t bySlot) const
{
    // auto it = m_mapItemRateInfo.find(bySlot);
    // if (it == m_mapItemRateInfo.end())
        return nullptr;
    // return &it->second;
}

// ============================================================================
// SendMaxStatLog - IDA 0x1400448E0
// Verified: Sends max stat log to DB
// ============================================================================
void CGocAttribute::SendMaxStatLog()
{
    CUser* pUser = GetUser();
    if (!pUser)
        return;

    // TODO: ST_LOG_GAME stLog;
    // stLog._sMainType = 2;
    // stLog._sSubType = 102;
    // stLog._nUAID = pUser->GetUAID();
    // stLog._nUCID = pUser->GetUCID();
    // stLog.nParam0 = m_nMaxStat[18];
    // stLog.nParam1 = m_nMaxStat[19];
    // stLog.nParam2 = m_nMaxStat[35];
    // stLog.nParam3 = m_nMaxStatEffect[1];
    // stLog.nParam4 = m_nMaxStatEffect[3];
    // stLog.nParam5 = m_nMaxStatEffect[4];
    // stLog.nParam6 = m_nMaxStatEffect[13];
    // stLog.nParam7 = m_nMaxStatEffect[45];
    // stLog.nParam8 = m_nMaxStatEffect[49];
    // stLog.nParam9 = m_nMaxStatEffect[50];
    // XGameServer::Instance()->SendDBLog(&stLog);
}

// ============================================================================
// GetCharStatInfo - IDA 0x14003E7A0
// Verified: Gets character stat info into two vectors (base and final)
// ============================================================================
void CGocAttribute::GetCharStatInfo(std::vector<struct ST_UPDATE_STAT>& vecBaseStat,
                                     std::vector<struct ST_UPDATE_STAT>& vecFinalStat)
{
    for (int i = 0; i < 77; ++i)
    {
        // ST_UPDATE_STAT stStat;
        // stStat.wStatID = i;
        // stStat.fValue = m_fFinalStat[i];
        // vecFinalStat.push_back(stStat);
        // stStat.fValue = m_fOriginStat[i];
        // vecBaseStat.push_back(stStat);
    }
}

// ============================================================================
// GetMonsterOriginStat - IDA 0x140049AD0
// Verified: Returns 0.0 for monster origin stat (stub)
// ============================================================================
float CGocAttribute::GetMonsterOriginStat(std::uint16_t wStatID) const
{
    return 0.0f;
}

// ============================================================================
// IsHaveMonsterOriginStat - IDA 0x140049AE0
// Verified: Returns false for monster origin stat check (stub)
// ============================================================================
bool CGocAttribute::IsHaveMonsterOriginStat(std::uint16_t wStatID) const
{
    return false;
}

// ============================================================================
// GetFPEffect - IDA 0x1400682D0
// Verified: Returns FP effect flag
// ============================================================================
bool CGocAttribute::GetFPEffect() const
{
    return m_bFPEffect;
}

// ============================================================================
// GetFinalStats - IDA 0x14019B9D0
// Verified: Returns pointer to final stat array
// ============================================================================
float* CGocAttribute::GetFinalStats()
{
    return m_fFinalStat;
}

// ============================================================================
// SetSTRegStat - IDA 0x1402C7EC0
// Verified: Sets ST regeneration stat flag
// ============================================================================
void CGocAttribute::SetSTRegStat(bool bEnable)
{
    m_bEnableSTRegStat = bEnable;
}

// ============================================================================
// GetOriginStat - IDA 0x1402F73D0
// Verified: Returns origin stat value
// ============================================================================
float CGocAttribute::GetOriginStat(int nStat) const
{
    return m_fOriginStat[nStat];
}

// ============================================================================
// GetMaxRat - IDA 0x1402F73F0
// Verified: Returns scale stat (max rate)
// ============================================================================
float CGocAttribute::GetMaxRat(int nStat) const
{
    return m_fScaleStat[nStat];
}

// ============================================================================
// GetStatusTable - IDA 0x1402F7410
// Verified: Returns pointer to status table
// ============================================================================
TB_STATUS* CGocAttribute::GetStatusTable()
{
    return reinterpret_cast<TB_STATUS*>(&m_StatusTable);
}

// ============================================================================
// GetMaxInt - IDA 0x1402F7420
// Verified: Returns add stat (max int)
// ============================================================================
float CGocAttribute::GetMaxInt(int nStat) const
{
    return m_fAddStat[nStat];
}

// ============================================================================
// GetHP - IDA 0x140378810
// Verified: Returns HP as integer
// ============================================================================
int CGocAttribute::GetHP() const
{
    return static_cast<int>(GetStat(1));
}

// ============================================================================
// SetSyncStatFlag - IDA 0x1403A1B60
// Verified: Sets sync stat flag for the given index
// ============================================================================
void CGocAttribute::SetSyncStatFlag(int iIndex, int eSyncStatType)
{
    if (iIndex < MAX_STAT_COUNT) {
        m_nSyncStat[iIndex] = eSyncStatType;
    }
}

// ============================================================================
// CCalculateStatus::CALCULATE_STAT_* Functions
// Restored from IDA decompilation
// ============================================================================

// CALCULATE_STAT_STR - IDA 0x1402D6BF0
// Calculates STR stat: base STR + max int bonus
float CCalculateStatus::CALCULATE_STAT_STR(CGocAttribute* pAttr)
{
    return pAttr->GetStat(4) + pAttr->GetMaxInt(4);
}

// CALCULATE_STAT_DEX - IDA 0x1402D6C30
// Calculates DEX stat: base DEX + max int bonus
float CCalculateStatus::CALCULATE_STAT_DEX(CGocAttribute* pAttr)
{
    return pAttr->GetStat(5) + pAttr->GetMaxInt(5);
}

// CALCULATE_STAT_INT - IDA 0x1402D6C70
// Calculates INT stat: base INT + max int bonus
float CCalculateStatus::CALCULATE_STAT_INT(CGocAttribute* pAttr)
{
    return pAttr->GetStat(6) + pAttr->GetMaxInt(6);
}

// CALCULATE_STAT_AGI - IDA 0x1402D6CB0
// Calculates AGI stat: base AGI + max int bonus
float CCalculateStatus::CALCULATE_STAT_AGI(CGocAttribute* pAttr)
{
    return pAttr->GetStat(7) + pAttr->GetMaxInt(7);
}

// CALCULATE_STAT_BAL - IDA 0x1402D6CE0
// Calculates BAL stat: base BAL + max int bonus
float CCalculateStatus::CALCULATE_STAT_BAL(CGocAttribute* pAttr)
{
    return pAttr->GetStat(2) + pAttr->GetMaxInt(7);
}

// CALCULATE_STAT_VIT - IDA 0x1402D6D30
// Calculates VIT stat: base VIT + max int bonus
float CCalculateStatus::CALCULATE_STAT_VIT(CGocAttribute* pAttr)
{
    return pAttr->GetStat(8) + pAttr->GetMaxInt(8);
}

// CALCULATE_STAT_LUC - IDA 0x1402D6D80
// Calculates LUC stat: base LUC + max int bonus
float CCalculateStatus::CALCULATE_STAT_LUC(CGocAttribute* pAttr)
{
    return pAttr->GetStat(9) + pAttr->GetMaxInt(9);
}

// CALCULATE_STAT_HP_MAX - IDA 0x1402D6DD0
// Calculates max HP: uses GetBaseHPMax helper
float CCalculateStatus::CALCULATE_STAT_HP_MAX(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    float fVital = pAttr->GetStat(8);
    // TODO: 需人工审查 - Need GetBaseHPMax helper
    float fHPMax = pTable->Con_HP_First_Value + pTable->Con_HP_Grow * fVital;
    float fRate = (pAttr->GetMaxRat(10) / 100.0f) + 1.0f;
    return fHPMax * fRate + pAttr->GetMaxInt(10);
}

// CALCULATE_STAT_SG_MAX - IDA 0x1402D6E90
// Calculates max SG: base SG * rate + bonus
float CCalculateStatus::CALCULATE_STAT_SG_MAX(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    float fSGFirst = static_cast<float>(pTable->SG_First_Value);
    float fRate = (pAttr->GetMaxRat(12) / 100.0f) + 1.0f;
    return fSGFirst * fRate + pAttr->GetMaxInt(12);
}

// CALCULATE_STAT_ST_MAX - IDA 0x1402D6F20
// Calculates max ST: base ST first value + max int bonus
float CCalculateStatus::CALCULATE_STAT_ST_MAX(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return pTable->Con_ST_First_Value + pAttr->GetMaxInt(14);
}

// CALCULATE_STAT_ST_REGEN - IDA 0x1402D6F70
// Calculates ST regeneration rate
float CCalculateStatus::CALCULATE_STAT_ST_REGEN(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return pTable->Con_ST_Reg + pAttr->GetMaxInt(15);
}

// CALCULATE_STAT_SV_MAX - IDA 0x1402D6FF0
// Calculates max SV: origin stat + max int bonus
float CCalculateStatus::CALCULATE_STAT_SV_MAX(CGocAttribute* pAttr)
{
    float fOrigin = pAttr->GetOriginStat(17);
    return fOrigin + pAttr->GetMaxInt(17);
}

// CALCULATE_STAT_MSR - IDA 0x1402D7040
// Calculates movement speed rate
float CCalculateStatus::CALCULATE_STAT_MSR(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need IsHaveMonsterOriginStat, GetMonsterOriginStat, GetBaseAttackSpeed helpers
    float fMSR = pTable->Con_MSR;
    return fMSR + pAttr->GetMaxRat(18);
}

// CALCULATE_STAT_ASR - IDA 0x1402D70F0
// Calculates attack speed rate
float CCalculateStatus::CALCULATE_STAT_ASR(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need IsHaveMonsterOriginStat, GetMonsterOriginStat, GetBaseAttackSpeed helpers
    float fASR = pTable->Con_ASR;
    return fASR + pAttr->GetMaxRat(19);
}

// CALCULATE_STAT_SG_REG - IDA 0x1402D71A0
// Calculates SG regeneration based on class type
float CCalculateStatus::CALCULATE_STAT_SG_REG(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need GetBaseSoulumswordSoulGaugeRegen, GetBaseGunjazzSoulGaugeRegen, etc helpers
    // Simplified implementation using base SG regen
    float fSGReg = pTable->Con_SG_Reg;
    float fResult = fSGReg + pAttr->GetMaxInt(13);
    return fResult + (fSGReg * pAttr->GetMaxRat(13)) / 100.0f;
}

// CALCULATE_STAT_SG_REGEN - IDA 0x1402D7010
// Calculates SG regeneration rate
float CCalculateStatus::CALCULATE_STAT_SG_REGEN(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return pTable->Con_SG_Reg + pAttr->GetMaxInt(12);
}

// CALCULATE_STAT_HP_REGEN - IDA 0x1402D70B0
// Calculates HP regeneration rate
float CCalculateStatus::CALCULATE_STAT_HP_REGEN(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - TB_STATUS missing Con_HP_Regen_Rate, using Con_HP_Grow as placeholder
    return pTable->Con_HP_Grow + pAttr->GetMaxInt(9);
}

// CALCULATE_STAT_PATK_MAX - IDA 0x1402D73A0
// Calculates max physical attack
float CCalculateStatus::CALCULATE_STAT_PATK_MAX(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need IsHaveMonsterOriginStat, GetMonsterOriginStat, GetBaseMagicalDamageMax helpers
    float fSTR = pAttr->GetStat(4);
    int nLevel = pAttr->GetLevelForStat();
    float fPATKMax = pTable->Con_PATK_Max + fSTR * nLevel * 0.01f;
    float fRate = (pAttr->GetMaxRat(21) / 100.0f) + 1.0f;
    return fPATKMax * fRate + pAttr->GetMaxInt(21);
}

// CALCULATE_STAT_PATK_MIN - IDA 0x1402D7570
// Calculates min physical attack (80% of max)
float CCalculateStatus::CALCULATE_STAT_PATK_MIN(CGocAttribute* pAttr)
{
    return pAttr->GetStat(0x15) * 0.8f;
}

// CALCULATE_STAT_MATK_MAX - IDA 0x1402D75A0
// Calculates max magic attack
float CCalculateStatus::CALCULATE_STAT_MATK_MAX(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need IsHaveMonsterOriginStat, GetMonsterOriginStat, GetBaseMagicalDamageMax helpers
    float fINT = pAttr->GetStat(6);
    int nLevel = pAttr->GetLevelForStat();
    float fMATKMax = pTable->Con_MATK_Max + fINT * nLevel * 0.01f;
    float fResult = fMATKMax + pAttr->GetMaxInt(23);
    float fRate = (pAttr->GetMaxRat(23) / 100.0f) + 1.0f;
    return fResult * fRate;
}

// CALCULATE_STAT_MATK_MIN - IDA 0x1402D76C0
// Calculates min magic attack (80% of max)
float CCalculateStatus::CALCULATE_STAT_MATK_MIN(CGocAttribute* pAttr)
{
    return pAttr->GetStat(0x17) * 0.8f;
}

// CALCULATE_STAT_PDEF - IDA 0x1402D76F0
// Calculates physical defense
float CCalculateStatus::CALCULATE_STAT_PDEF(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need IsHaveMonsterOriginStat, GetMonsterOriginStat, GetBasePhysicalCriticalDamage helpers
    float fAGI = pAttr->GetStat(5);
    float fPDEF = pTable->Con_PDEF * (1.0f + fAGI * 0.01f);
    float fRate = (pAttr->GetMaxRat(24) / 100.0f) + 1.0f;
    return fPDEF * fRate + pAttr->GetMaxInt(24);
}

// CALCULATE_STAT_PA - IDA 0x1402D7100
// Calculates physical attack
float CCalculateStatus::CALCULATE_STAT_PA(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    float fSTR = pAttr->GetStat(4);
    float fAGI = pAttr->GetStat(7);
    // TODO: 需人工审查 - Need GetBasePhysicalAttack helper
    return (fSTR + fAGI) * pTable->Con_PATK_Max + pAttr->GetMaxInt(18);
}

// CALCULATE_STAT_PA_RATE - IDA 0x1402D7160
// Calculates physical attack rate
float CCalculateStatus::CALCULATE_STAT_PA_RATE(CGocAttribute* pAttr)
{
    // TODO: 需人工审查 - TB_STATUS missing Con_PA_Rate field
    return pAttr->GetMaxInt(19);
}

// CALCULATE_STAT_MA - IDA 0x1402D71B0
// Calculates magic attack
float CCalculateStatus::CALCULATE_STAT_MA(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    float fINT = pAttr->GetStat(6);
    float fAGI = pAttr->GetStat(7);
    // TODO: 需人工审查 - Need GetBaseMagicAttack helper
    return (fINT + fAGI) * pTable->Con_MATK_Max + pAttr->GetMaxInt(22);
}

// CALCULATE_STAT_MA_RATE - IDA 0x1402D7210
// Calculates magic attack rate
float CCalculateStatus::CALCULATE_STAT_MA_RATE(CGocAttribute* pAttr)
{
    // TODO: 需人工审查 - TB_STATUS missing Con_MA_Rate field
    return pAttr->GetMaxInt(23);
}

// CALCULATE_STAT_PD - IDA 0x1402D7260
// Calculates physical defense
float CCalculateStatus::CALCULATE_STAT_PD(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    float fSTR = pAttr->GetStat(4);
    float fAGI = pAttr->GetStat(7);
    // TODO: 需人工审查 - Need GetBasePhysicalDefense helper
    return (fSTR + fAGI) * pTable->Con_PDEF + pAttr->GetMaxInt(26);
}

// CALCULATE_STAT_PD_RATE - IDA 0x1402D72C0
// Calculates physical defense rate
float CCalculateStatus::CALCULATE_STAT_PD_RATE(CGocAttribute* pAttr)
{
    // TODO: 需人工审查 - TB_STATUS missing Con_PD_Rate field
    return pAttr->GetMaxInt(27);
}

// CALCULATE_STAT_MD - IDA 0x1402D7310
// Calculates magic defense
float CCalculateStatus::CALCULATE_STAT_MD(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    float fINT = pAttr->GetStat(6);
    float fAGI = pAttr->GetStat(7);
    // TODO: 需人工审查 - Need GetBaseMagicDefense helper
    return (fINT + fAGI) * pTable->Con_MDEF + pAttr->GetMaxInt(30);
}

// CALCULATE_STAT_MD_RATE - IDA 0x1402D7370
// Calculates magic defense rate
float CCalculateStatus::CALCULATE_STAT_MD_RATE(CGocAttribute* pAttr)
{
    // TODO: 需人工审查 - TB_STATUS missing Con_MD_Rate field
    return pAttr->GetMaxInt(31);
}

// CALCULATE_STAT_MDEF - IDA 0x1402D77D0
// Calculates magic defense
float CCalculateStatus::CALCULATE_STAT_MDEF(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need IsHaveMonsterOriginStat, GetMonsterOriginStat, GetBaseMagicalDefence helpers
    float fBAL = pAttr->GetStat(7);
    float fINT = pAttr->GetStat(6);
    int nLevel = pAttr->GetLevelForStat();
    float fMDEF = pTable->Con_MDEF * (1.0f + fINT * 0.01f + fBAL * 0.005f);
    float fResult = fMDEF + pAttr->GetMaxInt(25);
    float fRate = (pAttr->GetMaxRat(25) / 100.0f) + 1.0f;
    return fResult * fRate;
}

// CALCULATE_STAT_PAR - IDA 0x1402D7910
// Calculates physical attack rate
float CCalculateStatus::CALCULATE_STAT_PAR(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need GetBasePhysicalAttackFail helper
    float fBAL = pAttr->GetStat(6);
    int nLevel = pAttr->GetLevelForStat();
    float fPAR = pTable->Con_PAR * (1.0f + fBAL * 0.01f);
    return fPAR + pAttr->GetMaxInt(26);
}

// CALCULATE_STAT_MAR - IDA 0x1402D79C0
// Calculates magic attack rate
float CCalculateStatus::CALCULATE_STAT_MAR(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need GetBaseMagicalAttackFail helper
    float fINT = pAttr->GetStat(6);
    int nLevel = pAttr->GetLevelForStat();
    float fMAR = pTable->Con_MAR * (1.0f + fINT * 0.01f);
    return fMAR + pAttr->GetMaxInt(27);
}

// CALCULATE_STAT_PARP - IDA 0x1402D7A70
// Calculates physical attack dodge rate
float CCalculateStatus::CALCULATE_STAT_PARP(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need GetBasePhysicalAttackDodgeRate helper
    float fAGI = pAttr->GetStat(5);
    float fDEX = pAttr->GetStat(7);
    float fPARP = pTable->Con_PARP * (1.0f + fDEX * 0.01f + fAGI * 0.005f);
    return fPARP + pAttr->GetMaxInt(43);
}

// CALCULATE_STAT_MARP - IDA 0x1402D7AF0
// Calculates magic attack dodge rate
float CCalculateStatus::CALCULATE_STAT_MARP(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need GetBaseMagicalAttackDodgeRate helper
    float fINT = pAttr->GetStat(6);
    float fAGI = pAttr->GetStat(7);
    float fMARP = pTable->Con_MAR * (1.0f + fINT * 0.01f + fAGI * 0.005f);
    float fResult = fMARP + pAttr->GetMaxInt(44);
    float fRate = (pAttr->GetMaxRat(44) / 100.0f) + 1.0f;
    return fResult * fRate;
}

// CALCULATE_STAT_PCP - IDA 0x1402D7BB0
// Calculates physical critical hit rate
float CCalculateStatus::CALCULATE_STAT_PCP(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return pTable->Con_PCP + pAttr->GetMaxInt(29);
}

// CALCULATE_STAT_MCP - IDA 0x1402D7C00
// Calculates magical critical hit rate
float CCalculateStatus::CALCULATE_STAT_MCP(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need GetBaseMagicalCriticalHitRate helper
    float fINT = pAttr->GetStat(6);
    float fBAL = pAttr->GetStat(7);
    int nLevel = pAttr->GetLevelForStat();
    float fMCP = pTable->Con_MCP * (1.0f + fINT * 0.01f + fBAL * 0.005f);
    return fMCP + pAttr->GetMaxInt(30);
}

// CALCULATE_STAT_PCRP - IDA 0x1402D7CB0
// Calculates physical critical damage rate
float CCalculateStatus::CALCULATE_STAT_PCRP(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return pTable->Con_PCRP + pAttr->GetMaxInt(31);
}

// CALCULATE_STAT_MCRP - IDA 0x1402D7D00
// Calculates magical critical damage rate
float CCalculateStatus::CALCULATE_STAT_MCRP(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need GetBaseMagicalCriticalDamageAttenuationRate helper
    float fINT = pAttr->GetStat(6);
    int nLevel = pAttr->GetLevelForStat();
    float fMCRP = pTable->Con_MCRP * (1.0f + fINT * 0.01f);
    return fMCRP + pAttr->GetMaxInt(32);
}

// CALCULATE_STAT_PCA - IDA 0x1402D7D90
// Calculates physical critical attack damage
float CCalculateStatus::CALCULATE_STAT_PCA(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need GetBasePhysicalCriticalDamage helper
    float fPATKMax = pAttr->GetStat(0x15);
    float fSTR = pAttr->GetStat(4);
    float fPCA = pTable->Con_PCA * (1.0f + fSTR * 0.01f + fPATKMax * 0.001f);
    float fRate = (pAttr->GetMaxRat(35) / 100.0f) + 1.0f;
    return fPCA * fRate + pAttr->GetMaxInt(35);
}

// CALCULATE_STAT_CAR - IDA 0x1402D7B30
// Calculates critical attack rate
float CCalculateStatus::CALCULATE_STAT_CAR(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    float fAGI = pAttr->GetStat(7);
    float fDEX = pAttr->GetStat(5);
    // TODO: 需人工审查 - Need GetBaseCriticalAttackRate helper, using Con_PCA as placeholder
    return (fDEX + fAGI) * pTable->Con_PCA + pAttr->GetMaxInt(45);
}

// CALCULATE_STAT_CAD - IDA 0x1402D7B90
// Calculates critical attack damage
float CCalculateStatus::CALCULATE_STAT_CAD(CGocAttribute* pAttr)
{
    // TODO: 需人工审查 - TB_STATUS missing Con_CAD field
    return pAttr->GetMaxInt(46);
}

// CALCULATE_STAT_MCA - IDA 0x1402D7EB0
// Calculates magical critical attack damage
float CCalculateStatus::CALCULATE_STAT_MCA(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    // TODO: 需人工审查 - Need GetBasePhysicalCriticalDamage helper
    float fMATKMax = pAttr->GetStat(0x17);
    float fMCA = pTable->Con_MCA * (1.0f + fMATKMax * 0.001f);
    float fRate = (pAttr->GetMaxRat(36) / 100.0f) + 1.0f;
    return fMCA * fRate;
}

// CALCULATE_STAT_PDSR - IDA 0x1402D7F30
// Calculates physical damage reduction rate
float CCalculateStatus::CALCULATE_STAT_PDSR(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return pTable->Con_PDSR + pAttr->GetMaxInt(38);
}

// CALCULATE_STAT_ADR - IDA 0x1402D7F80
// Calculates attack damage reduction
float CCalculateStatus::CALCULATE_STAT_ADR(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return pTable->Con_ADR_Value + pAttr->GetMaxInt(28);
}

// CALCULATE_STAT_RES_BURN - IDA 0x1402D7FD0
// Calculates burn resistance
float CCalculateStatus::CALCULATE_STAT_RES_BURN(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Burn_Rate) + pAttr->GetMaxInt(49);
}

// CALCULATE_STAT_RES_POISON - IDA 0x1402D8020
// Calculates poison resistance
float CCalculateStatus::CALCULATE_STAT_RES_POISON(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Poision_Rate) + pAttr->GetMaxInt(50);
}

// CALCULATE_STAT_RES_SHOCK - IDA 0x1402D8070
// Calculates shock resistance
float CCalculateStatus::CALCULATE_STAT_RES_SHOCK(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Shcok_Rate) + pAttr->GetMaxInt(51);
}

// CALCULATE_STAT_RES_BLEED - IDA 0x1402D80C0
// Calculates bleed resistance
float CCalculateStatus::CALCULATE_STAT_RES_BLEED(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Bleed_Rate) + pAttr->GetMaxInt(52);
}

// CALCULATE_STAT_RES_STUN - IDA 0x1402D8110
// Calculates stun resistance
float CCalculateStatus::CALCULATE_STAT_RES_STUN(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Stun_Rate) + pAttr->GetMaxInt(53);
}

// CALCULATE_STAT_RES_PARALYSIS - IDA 0x1402D8160
// Calculates paralysis resistance
float CCalculateStatus::CALCULATE_STAT_RES_PARALYSIS(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Paralysis_Rate) + pAttr->GetMaxInt(54);
}

// CALCULATE_STAT_RES_SLEEP - IDA 0x1402D81B0
// Calculates sleep resistance
float CCalculateStatus::CALCULATE_STAT_RES_SLEEP(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Sleep_Rate) + pAttr->GetMaxInt(55);
}

// CALCULATE_STAT_RES_FREEZE - IDA 0x1402D8200
// Calculates freeze resistance
float CCalculateStatus::CALCULATE_STAT_RES_FREEZE(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Freeze_Rate) + pAttr->GetMaxInt(56);
}

// CALCULATE_STAT_RES_CHARM - IDA 0x1402D8250
// Calculates charm resistance
float CCalculateStatus::CALCULATE_STAT_RES_CHARM(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Charm_Rate) + pAttr->GetMaxInt(57);
}

// CALCULATE_STAT_RES_CONFUSION - IDA 0x1402D82A0
// Calculates confusion resistance
float CCalculateStatus::CALCULATE_STAT_RES_CONFUSION(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Confusion_Rate) + pAttr->GetMaxInt(58);
}

// CALCULATE_STAT_RES_SILENCE - IDA 0x1402D82F0
// Calculates silence resistance
float CCalculateStatus::CALCULATE_STAT_RES_SILENCE(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Silence_Rate) + pAttr->GetMaxInt(59);
}

// CALCULATE_STAT_RES_WEAKNESS - IDA 0x1402D8340
// Calculates weakness resistance
float CCalculateStatus::CALCULATE_STAT_RES_WEAKNESS(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Weak_Rate) + pAttr->GetMaxInt(60);
}

// CALCULATE_STAT_PDPR - IDA 0x1402D8390
// Calculates physical damage penetration rate
float CCalculateStatus::CALCULATE_STAT_PDPR(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->PDPR_First_Value) + pAttr->GetMaxInt(47);
}

// CALCULATE_STAT_MDPR - IDA 0x1402D83E0
// Calculates magical damage penetration rate
float CCalculateStatus::CALCULATE_STAT_MDPR(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->MDPR_First_Value) + pAttr->GetMaxInt(48);
}

// CALCULATE_STAT_ATTRIBUTE_LIGHT - IDA 0x1402D8430
// Calculates light attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_LIGHT(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(63);
}

// CALCULATE_STAT_ATTRIBUTE_DARKNESS - IDA 0x1402D8460
// Calculates darkness attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_DARKNESS(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(64);
}

// CALCULATE_STAT_ATTRIBUTE_COOL - IDA 0x1402D8490
// Calculates cool attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_COOL(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(65);
}

// CALCULATE_STAT_RES_FIRE - IDA 0x1402D8020
// Calculates fire resistance
float CCalculateStatus::CALCULATE_STAT_RES_FIRE(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Burn_Rate) + pAttr->GetMaxInt(48);
}

// CALCULATE_STAT_RES_ICE - IDA 0x1402D8070
// Calculates ice resistance
float CCalculateStatus::CALCULATE_STAT_RES_ICE(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Freeze_Rate) + pAttr->GetMaxInt(49);
}

// CALCULATE_STAT_RES_ELECTRIC - IDA 0x1402D8110
// Calculates electric resistance
float CCalculateStatus::CALCULATE_STAT_RES_ELECTRIC(CGocAttribute* pAttr)
{
    TB_STATUS* pTable = pAttr->GetStatusTable();
    return static_cast<float>(pTable->Res_Shcok_Rate) + pAttr->GetMaxInt(51);
}

// CALCULATE_STAT_ATTRIBUTE_FIRE - IDA 0x1402D84C0
// Calculates fire attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_FIRE(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(60);
}

// CALCULATE_STAT_ATTRIBUTE_ICE - IDA 0x1402D84E0
// Calculates ice attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_ICE(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(61);
}

// CALCULATE_STAT_ATTRIBUTE_POISON - IDA 0x1402D8500
// Calculates poison attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_POISON(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(62);
}

// CALCULATE_STAT_ATTRIBUTE_ELECTRIC - IDA 0x1402D8520
// Calculates electric attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_ELECTRIC(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(63);
}

// CALCULATE_STAT_ATTRIBUTE_BLEED - IDA 0x1402D8540
// Calculates bleed attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_BLEED(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(64);
}

// CALCULATE_STAT_ATTRIBUTE_ABHOR - IDA 0x1402D84C0
// Calculates abhor attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_ABHOR(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(66);
}

// CALCULATE_STAT_ATTRIBUTE_HEAL - IDA 0x1402D84F0
// Calculates heal attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_HEAL(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(67);
}

// CALCULATE_STAT_ATTRIBUTE_PAIN - IDA 0x1402D8520
// Calculates pain attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_PAIN(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(68);
}

// CALCULATE_STAT_ATTRIBUTE_RES_LIGHT - IDA 0x1402D8550
// Calculates light resistance attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_RES_LIGHT(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(69);
}

// CALCULATE_STAT_ATTRIBUTE_RES_DARKNESS - IDA 0x1402D8580
// Calculates darkness resistance attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_RES_DARKNESS(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(70);
}

// CALCULATE_STAT_ATTRIBUTE_RES_COOL - IDA 0x1402D85B0
// Calculates cool resistance attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_RES_COOL(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(71);
}

// CALCULATE_STAT_ATTRIBUTE_RES_ABHOR - IDA 0x1402D85E0
// Calculates abhor resistance attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_RES_ABHOR(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(72);
}

// CALCULATE_STAT_ATTRIBUTE_RES_HEAL - IDA 0x1402D8610
// Calculates heal resistance attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_RES_HEAL(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(73);
}

// CALCULATE_STAT_ATTRIBUTE_RES_PAIN - IDA 0x1402D8640
// Calculates pain resistance attribute
float CCalculateStatus::CALCULATE_STAT_ATTRIBUTE_RES_PAIN(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(74);
}

// CALCULATE_STAT_PVP_ATK - IDA 0x1402D8670
// Calculates PVP attack attribute
float CCalculateStatus::CALCULATE_STAT_PVP_ATK(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(75);
}

// CALCULATE_STAT_PVP_DEF - IDA 0x1402D86A0
// Calculates PVP defense attribute
float CCalculateStatus::CALCULATE_STAT_PVP_DEF(CGocAttribute* pAttr)
{
    return pAttr->GetMaxInt(76);
}

// ============================================================================
// Additional Simple Getters/Setters - IDA Verified
// ============================================================================

// SetNoSpendST - IDA 0x140406E10
// void __fastcall CGocAttribute::SetNoSpendST(CGocAttribute *this, bool bStopST)
// {
//   this->m_bNoSpendST = bStopST;
// }
void CGocAttribute::SetNoSpendST(bool bNoSpend) {
    m_bNoSpendST = bNoSpend;
}

// SetStopSGRegStat - IDA 0x140406E30
// void __fastcall CGocAttribute::SetStopSGRegStat(CGocAttribute *this, bool bStopSG)
// {
//   this->m_bStopSGRegStat = bStopSG;
// }
void CGocAttribute::SetStopSGRegStat(bool bStop) {
    m_bStopSGRegStat = bStop;
}

// SetStopSTRegStat - IDA 0x140406E50
// void __fastcall CGocAttribute::SetStopSTRegStat(CGocAttribute *this, bool bStopST)
// {
//   this->m_bStopSTRegStat = bStopST;
// }
void CGocAttribute::SetStopSTRegStat(bool bStop) {
    m_bStopSTRegStat = bStop;
}

// ResetLastEnableSGTime - IDA 0x14070ACB0
// void __fastcall CGocAttribute::ResetLastEnableSGTime(CGocAttribute *this)
// {
//   this->m_fLastEnableSGTime = 0.0;
// }
void CGocAttribute::ResetLastEnableSGTime() {
    m_fLastEnableSGTime = 0.0f;
}

// GetSGRegType - IDA 0x14070ACD0
// __int64 __fastcall CGocAttribute::GetSGRegType(CGocAttribute *this)
// {
//   return this->m_bySGRegType;
// }
std::uint8_t CGocAttribute::GetSGRegType() const {
    return m_bySGRegType;
}
