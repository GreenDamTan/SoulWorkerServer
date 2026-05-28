// CGocAttribute - Game Object Component for actor attributes
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140039080 - 0x140049620

#include "GocAttribute.h"
#include <cmath>
#include <cstring>

// Forward declarations
class CCalculateStatus;
class XResourceMgr;

// ============================================================================
// Constructor - IDA 0x140039080
// ============================================================================
CGocAttribute::CGocAttribute()
    : GOComponent()
    , m_nClass(0)
    , m_nLv(1)
    , m_nModeLv(0)
    , m_nTableIdx(0)
    , m_nStatusType(0)
    , m_fSTUpdateTime(0.0f)
    , m_fSGUpdateTime(0.0f)
    , m_fContinousCostSendTime(0.0f)
    , m_nExp(0)
    , m_bStartRegStat(false)
    , m_bEnableSGRegStat(false)
    , m_bEnableSTRegStat(true)
    , m_bFPEffect(false)
    , m_biFPInitDate(0)
    , m_dw64FPTick(0)
    , m_byEchelonLevel(0)
    , m_nEchelonExp(0)
    , m_byPrevEchelonLevel(0)
    , m_nPrevEchelonExp(0)
    , m_bStopSTRegStat(false)
    , m_bStopSGRegStat(false)
    , m_bNoSpendST(false)
    , m_pSoulGuageRef(nullptr)
    , m_fPrevSG(0.0f)
    , m_bEnableSGReg(false)
    , m_bySGRegType(0)
    , m_fLastEnableSGTime(0.0f)
    , m_iEquipOptionIndex(0)
    , m_byAwaken(0)
    , m_nGameModeState(0)
    , m_byNation(0)
{
    // Initialize vectors first (from IDA)
    m_vecEquipedOption.clear();
    m_mapItemSkilllOption.clear();
    m_mapItemRateInfo.clear();
    m_vecScaleStat_Cheat.clear();
    m_vecAddStat_Cheat.clear();

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
    std::memset(m_nMaxStat, 0, sizeof(m_nMaxStat));
    std::memset(m_nMaxStatEffect, 0, sizeof(m_nMaxStatEffect));

    // Clear skill option effect
    ClearSkillOptionEffect();
}

// ============================================================================
// Destructor - IDA 0x1400393F0
// ============================================================================
CGocAttribute::~CGocAttribute()
{
    // Clear equipped options
    for (auto* pOption : m_vecEquipedOption)
    {
        if (pOption)
        {
            delete pOption;
        }
    }
    m_vecEquipedOption.clear();

    m_vecAddStat_Cheat.clear();
    m_vecScaleStat_Cheat.clear();
    m_mapItemRateInfo.clear();
    m_mapItemSkilllOption.clear();
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
// ============================================================================
bool CGocAttribute::IsRanger() const
{
    return m_nStatusType == 2;
}

// ============================================================================
// IsPlayer - IDA 0x14003A730
// ============================================================================
bool CGocAttribute::IsPlayer() const
{
    // TODO: Get owner actor and check if it's a player type
    return true;
}

// ============================================================================
// GetStat - IDA 0x14003CDF0
// ============================================================================
float CGocAttribute::GetStat(int nStat) const
{
    if (nStat < 0 || nStat > 0x4C)  // 76
        return 0.0f;
    return m_fFinalStat[nStat];
}

// ============================================================================
// SetStat - IDA 0x14003C080
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
        }
    }
    else
    {
        UpdateAddStat(nStatID, fValue, true);
    }
}

// ============================================================================
// SetFinalStat - IDA 0x14003C170
// ============================================================================
void CGocAttribute::SetFinalStat(int nStatID, float fValue, bool bSync)
{
    if (nStatID < 0 || nStatID > 0x4C)
        return;

    float fValidValue = fValue;
    IsValidStat(nStatID, &fValidValue);

    if (m_fFinalStat[nStatID] == fValidValue)
        return;

    if (m_bNoSpendST && nStatID == 3)
        return;

    m_fFinalStat[nStatID] = fValidValue;
    m_nSyncStat[nStatID] = 1;

    switch (nStatID)
    {
        case 1:  // Current HP
            m_nSyncStat[nStatID] = 2;
            break;
        case 2:  // Current SG
            m_nSyncStat[nStatID] = 2;
            break;
        case 10:  // Max HP
            m_nSyncStat[nStatID] = 2;
            if (m_fFinalStat[1] > m_fFinalStat[10])
            {
                m_fFinalStat[1] = m_fFinalStat[10];
                m_nSyncStat[1] = 2;
            }
            break;
        case 12:  // Max SG
            m_nSyncStat[nStatID] = 2;
            if (m_fFinalStat[2] > m_fFinalStat[12])
            {
                m_fFinalStat[2] = m_fFinalStat[12];
                m_nSyncStat[2] = 2;
            }
            break;
        case 14:  // Max ST
            if (m_fFinalStat[3] > m_fFinalStat[14])
            {
                m_fFinalStat[3] = m_fFinalStat[14];
                m_nSyncStat[3] = 1;
            }
            break;
        case 16:  // SV
            if (m_fFinalStat[16] > m_fFinalStat[17])
                SetStat(16, m_fFinalStat[17], false);
            break;
        case 18:  // MSR
        case 19:  // ASR
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
// ============================================================================
void CGocAttribute::SetHP(float fValue)
{
    SetStat(1, fValue, true);
}

// ============================================================================
// UpdateScaleStat - IDA 0x14003B510
// ============================================================================
void CGocAttribute::UpdateScaleStat(int nStat, float fValue, bool bCalc)
{
    if (nStat >= 0 && nStat <= 0x4C)
    {
        m_fScaleStat[nStat] += fValue;
        if (bCalc)
        {
            // TODO: CCalculateStatus::CalculateStatus(nStat, this)
        }
        else
        {
            m_bCalcStat[nStat] = true;
        }
    }
}

// ============================================================================
// UpdateAddStat - IDA 0x14003B5A0
// ============================================================================
void CGocAttribute::UpdateAddStat(int nStat, float fValue, bool bCalc)
{
    if (nStat >= 0 && nStat <= 0x4C && fValue != 0.0f)
    {
        m_fAddStat[nStat] += fValue;
        if (bCalc)
        {
            // TODO: CCalculateStatus::CalculateStatus(nStat, this)
        }
        else
        {
            m_bCalcStat[nStat] = true;
        }
    }
}

// ============================================================================
// UpdateEffectStat - IDA 0x14003B640
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
            if (iIndex >= 0 && iIndex <= 0x36)
            {
                m_fItemSpecaillEffect[iIndex] += fValue;
                m_bItemSpecialEffectChanged[iIndex] = true;
            }
        }
    }
}

// ============================================================================
// CalculateChangedStat - IDA 0x14003B920
// ============================================================================
void CGocAttribute::CalculateChangedStat(bool bSend)
{
    for (int i = 0; i < MAX_STAT_COUNT; ++i)
    {
        if (m_bCalcStat[i])
        {
            // TODO: CCalculateStatus::CalculateStatus(i, this)
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
// ============================================================================
int CGocAttribute::GetSpecialEffectIndex(int iItemEffectType)
{
    if (iItemEffectType >= 100 && iItemEffectType < 155)
        return iItemEffectType - 100;
    return -1;  // 0xFFFFFFFF
}

// ============================================================================
// FPEffect - IDA 0x14003F930
// ============================================================================
void CGocAttribute::FPEffect()
{
    m_bFPEffect = true;
    UpdateEffectStat(0, 0x73, 10.0f, true);  // Effect type 115 = 0x73
}

// ============================================================================
// DelFPEffect - IDA 0x14003F970
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
// ============================================================================
int CGocAttribute::GetLevelForStat() const
{
    if (m_nModeLv <= 0)
        return m_nLv;
    return m_nModeLv;
}

// ============================================================================
// SetStartRegStat - IDA 0x1400495E0
// ============================================================================
void CGocAttribute::SetStartRegStat(bool bEnable)
{
    m_bStartRegStat = bEnable;
}

// ============================================================================
// SetSGRegStat - IDA 0x140049620
// ============================================================================
void CGocAttribute::SetSGRegStat(bool bEnable)
{
    m_bEnableSGRegStat = bEnable;
    m_fSGUpdateTime = 0.0f;
}

// ============================================================================
// SetGameModeState - IDA 0x140048B60
// ============================================================================
void CGocAttribute::SetGameModeState(int nState)
{
    m_nGameModeState = nState;
}

// ============================================================================
// GetAwaken - IDA 0x1400444E0
// ============================================================================
std::uint8_t CGocAttribute::GetAwaken() const
{
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

void CGocAttribute::SetEquipedOption(int nStatType, float fValue)
{
    (void)nStatType;
    (void)fValue;
    // TODO: Implement
}

void CGocAttribute::SendUpdateStatList()
{
    // TODO: IDA 0x14003D090
}

void CGocAttribute::SendUpdateStat(int nStat)
{
    (void)nStat;
    // TODO: IDA 0x14003D3B0
}
