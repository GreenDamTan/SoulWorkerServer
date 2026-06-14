// CGocNpcAttribute - NPC Attribute Game Object Component
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140039D40 - 0x140105260

// First include DBLoadTable.h which defines the TB structs
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"

#include "GocNpcAttribute.h"
#include "Soulworker/GameServer/XSCommon/Table/TB_STATUS.h"
#include <cstring>

// Forward declarations for external classes
class CCalculateStatus;
class CMover;
class XSendPacket;
struct ST_UPDATE_STAT_LIST;
struct ST_UPDATE_STAT;

// External function declarations
// TODO: These need proper header includes when available

// Global NPC sync stat array - defines which stats should be synchronized for NPCs
// IDA Address: 0x140D1D430
// These are the stat IDs that get synchronized to clients when changed
const int g_NpcSyncStat[4] = { 1, 2, 3, 10 };  // HP, SG, ST, MaxHP

// ============================================================================
// Constructor - IDA 0x140103F00
// Verified: Calls base class constructor and sets vtable
// ============================================================================
CGocNpcAttribute::CGocNpcAttribute()
    : CGocAttribute()
{
    // Set vtable pointer for CGocNpcAttribute
    // IDA: this->__vftable = &CGocNpcAttribute::`vftable'

    // Initialize monster origin stat array
    std::memset(m_fMonsterOriginStat, 0, sizeof(m_fMonsterOriginStat));
}

// ============================================================================
// Destructor - IDA 0x140103F70
// Verified: Sets vtable then calls base destructor
// ============================================================================
CGocNpcAttribute::~CGocNpcAttribute()
{
    // IDA: Set vtable to destructor vtable first
    // this->__vftable = &CGocNpcAttribute::`vftable'

    // Base class destructor called automatically
}

// ============================================================================
// Initialize - IDA 0x140103FA0
// Verified: Initialize from TB_MONSTER table with class
// ============================================================================
void CGocNpcAttribute::Init(std::uint16_t nClass, const TB_MONSTER* pMonsterTable)
{
    // Clear all stat arrays
    std::memset(m_fScaleStat, 0, sizeof(m_fScaleStat));
    std::memset(m_fAddStat, 0, sizeof(m_fAddStat));
    std::memset(m_fFinalStat, 0, sizeof(m_fFinalStat));
    std::memset(m_nSyncStat, 0, sizeof(m_nSyncStat));
    std::memset(m_bCalcStat, 0, sizeof(m_bCalcStat));
    std::memset(m_fContinousCost, 0, sizeof(m_fContinousCost));
    std::memset(m_fMonsterOriginStat, 0, sizeof(m_fMonsterOriginStat));

    // Set basic info from monster table
    m_nClass = nClass;
    m_nLv = pMonsterTable->Monster_Lv;
    m_nTableIdx = pMonsterTable->Monster_Status_ID;
    m_nStatusType = pMonsterTable->Monster_Status_Type;

    // Clear update timers
    m_fSTUpdateTime = 0.0f;
    m_fSGUpdateTime = 0.0f;
    m_fContinousCostSendTime = 0.0f;

    // Set reg stat flags
    m_bStartRegStat = false;
    m_bEnableSGRegStat = false;
    m_bEnableSTRegStat = true;

    // Set status table from base class
    SetStatusTable();

    // Set monster-specific status table values
    SetMonsterStatusTable(pMonsterTable);

    // Update resistance stats from monster table
    // IDA verified: stat IDs 0x31-0x3C (49-60) are resistance values
    UpdateAddStat(0x31, static_cast<float>(pMonsterTable->Res_Burn_INT_Value), false);       // 49: Burn resistance
    UpdateAddStat(0x32, static_cast<float>(pMonsterTable->Res_Poision_INT_Value), false);    // 50: Poison resistance
    UpdateAddStat(0x33, static_cast<float>(pMonsterTable->Res_Shcok_INT_Value), false);      // 51: Shock resistance
    UpdateAddStat(0x34, static_cast<float>(pMonsterTable->Res_Bleed_INT_Value), false);      // 52: Bleed resistance
    UpdateAddStat(0x35, static_cast<float>(pMonsterTable->Res_Stun_INT_Value), false);       // 53: Stun resistance
    UpdateAddStat(0x36, static_cast<float>(pMonsterTable->Res_Paralysis_INT_Value), false);  // 54: Paralysis resistance
    UpdateAddStat(0x37, static_cast<float>(pMonsterTable->Res_Sleep_INT_Value), false);      // 55: Sleep resistance
    UpdateAddStat(0x38, static_cast<float>(pMonsterTable->Res_Freeze_INT_Value), false);     // 56: Freeze resistance
    UpdateAddStat(0x3B, static_cast<float>(pMonsterTable->Res_Silence_INT_Value), false);    // 59: Silence resistance
    UpdateAddStat(0x3C, static_cast<float>(pMonsterTable->Res_Weak_INT_Value), false);       // 60: Weak resistance
    UpdateAddStat(0x39, static_cast<float>(pMonsterTable->Res_Charm_INT_Value), false);      // 57: Charm resistance
    UpdateAddStat(0x3A, static_cast<float>(pMonsterTable->Res_Confusion_INT_Value), false);  // 58: Confusion resistance

    // Calculate all stats
    // TODO: Call CCalculateStatus::CalculateStatusAll
    // CCalculateStatus* pCalc = TXSingleton<CCalculateStatus>::Instance();
    // CCalculateStatus::CalculateStatusAll(pCalc, this);
}

// ============================================================================
// Initialize - IDA 0x1401042E0
// Verified: Initialize from TB_NPC table
// ============================================================================
void CGocNpcAttribute::Init(std::uint16_t nClass, const TB_NPC* pNPCTable)
{
    // Clear all stat arrays
    std::memset(m_fScaleStat, 0, sizeof(m_fScaleStat));
    std::memset(m_fAddStat, 0, sizeof(m_fAddStat));
    std::memset(m_fFinalStat, 0, sizeof(m_fFinalStat));
    std::memset(m_nSyncStat, 0, sizeof(m_nSyncStat));
    std::memset(m_bCalcStat, 0, sizeof(m_bCalcStat));
    std::memset(m_fContinousCost, 0, sizeof(m_fContinousCost));
    std::memset(m_fMonsterOriginStat, 0, sizeof(m_fMonsterOriginStat));

    // Set basic info from NPC table
    m_nClass = nClass;
    m_nLv = pNPCTable->NPC_Level;
    m_nTableIdx = pNPCTable->NPC_Status_ID;
    m_nStatusType = pNPCTable->NPC_Status_Type;

    // Clear update timers
    m_fSTUpdateTime = 0.0f;
    m_fSGUpdateTime = 0.0f;
    m_fContinousCostSendTime = 0.0f;

    // Set reg stat flags
    m_bStartRegStat = false;
    m_bEnableSGRegStat = false;
    m_bEnableSTRegStat = true;

    // Set status table from base class
    SetStatusTable();

    // Calculate all stats
    // TODO: Call CCalculateStatus::CalculateStatusAll
    // CCalculateStatus* pCalc = TXSingleton<CCalculateStatus>::Instance();
    // CCalculateStatus::CalculateStatusAll(pCalc, this);
}

// ============================================================================
// Initialize - IDA 0x140104460
// Verified: Initialize from TB_MONSTER table with class and level override
// ============================================================================
void CGocNpcAttribute::Init(int nClass, int nLevel, const TB_MONSTER* pMonsterTable)
{
    // Clear all stat arrays
    std::memset(m_fScaleStat, 0, sizeof(m_fScaleStat));
    std::memset(m_fAddStat, 0, sizeof(m_fAddStat));
    std::memset(m_fFinalStat, 0, sizeof(m_fFinalStat));
    std::memset(m_nSyncStat, 0, sizeof(m_nSyncStat));
    std::memset(m_bCalcStat, 0, sizeof(m_bCalcStat));
    std::memset(m_fContinousCost, 0, sizeof(m_fContinousCost));
    std::memset(m_fMonsterOriginStat, 0, sizeof(m_fMonsterOriginStat));

    // Set basic info from monster table with level override
    m_nClass = nClass;
    m_nLv = nLevel;  // Use provided level instead of table level
    m_nTableIdx = pMonsterTable->Monster_Status_ID;
    m_nStatusType = pMonsterTable->Monster_Status_Type;

    // Clear update timers
    m_fSTUpdateTime = 0.0f;
    m_fSGUpdateTime = 0.0f;
    m_fContinousCostSendTime = 0.0f;

    // Set reg stat flags
    m_bStartRegStat = false;
    m_bEnableSGRegStat = false;
    m_bEnableSTRegStat = true;

    // Set status table from base class
    SetStatusTable();

    // Set monster-specific status table values
    SetMonsterStatusTable(pMonsterTable);

    // Calculate all stats
    // TODO: Call CCalculateStatus::CalculateStatusAll
    // CCalculateStatus* pCalc = TXSingleton<CCalculateStatus>::Instance();
    // CCalculateStatus::CalculateStatusAll(pCalc, this);
}

// ============================================================================
// SetMonsterStatusTable - IDA 0x140104C40
// Verified: Sets monster stats from TB_MONSTER INT values
// ============================================================================
void CGocNpcAttribute::SetMonsterStatusTable(const TB_MONSTER* pMonsterTable)
{
    // Set basic stats from monster table INT values
    // All calls use bCalc = true (1) to trigger immediate recalculation

    // Stat 10 (0x0A): Max HP
    UpdateAddStat(0x0A, static_cast<float>(pMonsterTable->HP_Max_INT_Value), true);

    // Stat 12 (0x0C): Max SG
    UpdateAddStat(0x0C, static_cast<float>(pMonsterTable->SG_Max_INT_Value), true);

    // Attack stats
    UpdateAddStat(0x15, static_cast<float>(pMonsterTable->PATK_Max_INT_Value), true);  // Stat 21: Physical attack
    UpdateAddStat(0x17, static_cast<float>(pMonsterTable->MATK_Max_INT_Value), true);  // Stat 23: Magic attack

    // Defense stats
    UpdateAddStat(0x18, static_cast<float>(pMonsterTable->PDEF_INT_Value), true);  // Stat 24: Physical defense
    UpdateAddStat(0x19, static_cast<float>(pMonsterTable->MDEF_INT_Value), true);  // Stat 25: Magic defense

    // Hit rate stats
    UpdateAddStat(0x1A, static_cast<float>(pMonsterTable->PAR_INT_Value), true);  // Stat 26: Physical hit rate
    UpdateAddStat(0x1B, static_cast<float>(pMonsterTable->MAR_INT_Value), true);  // Stat 27: Magic hit rate

    // Critical and other combat stats
    UpdateAddStat(0x1C, static_cast<float>(pMonsterTable->ADR_INT_Value), true);   // Stat 28: Critical rate
    UpdateAddStat(0x1E, static_cast<float>(pMonsterTable->MCP_INT_Value), true);   // Stat 30: Magic critical rate
    UpdateAddStat(0x1F, static_cast<float>(pMonsterTable->PCRP_INT_Value), true);  // Stat 31: Physical crit resist
    UpdateAddStat(0x20, static_cast<float>(pMonsterTable->MCRP_INT_Value), true);  // Stat 32: Magic crit resist
    UpdateAddStat(0x23, static_cast<float>(pMonsterTable->PCA_INT_Value), true);   // Stat 35: Physical crit amt
    UpdateAddStat(0x24, static_cast<float>(pMonsterTable->MCA_INT_Value), true);   // Stat 36: Magic crit amt
    UpdateAddStat(0x25, static_cast<float>(pMonsterTable->DAR_INT_Value), true);   // Stat 37: Dodge rate
    UpdateAddStat(0x26, static_cast<float>(pMonsterTable->PDSR_INT_Value), true);  // Stat 38: Physical down strike
    UpdateAddStat(0x27, static_cast<float>(pMonsterTable->MDSR_INT_Value), true);  // Stat 39: Magic down strike

    // Parry and penetration stats
    UpdateAddStat(0x2B, static_cast<float>(pMonsterTable->PARP_INT_Value), true);  // Stat 43: Physical parry pen
    UpdateAddStat(0x2C, static_cast<float>(pMonsterTable->MARP_INT_Value), true);  // Stat 44: Magic parry pen

    // Attack rate stats
    UpdateAddStat(0x2F, static_cast<float>(pMonsterTable->PDPR_RAT_Value), true);  // Stat 47: Physical dmg rate
    UpdateAddStat(0x30, static_cast<float>(pMonsterTable->MDPR_RAT_Value), true);  // Stat 48: Magic dmg rate

    // Attack power elements (0x3F-0x44 = stats 63-68)
    UpdateAddStat(0x3F, static_cast<float>(pMonsterTable->AtkPower_Light_INT_Value), true);
    UpdateAddStat(0x40, static_cast<float>(pMonsterTable->AtkPower_Dark_INT_Value), true);
    UpdateAddStat(0x41, static_cast<float>(pMonsterTable->AtkPower_Blue_INT_Value), true);
    UpdateAddStat(0x42, static_cast<float>(pMonsterTable->AtkPower_Red_INT_Value), true);
    UpdateAddStat(0x43, static_cast<float>(pMonsterTable->AtkPower_White_INT_Value), true);
    UpdateAddStat(0x44, static_cast<float>(pMonsterTable->AtkPower_Green_INT_Value), true);

    // Resistance elements (0x45-0x4A = stats 69-74)
    UpdateAddStat(0x45, static_cast<float>(pMonsterTable->Resist_Light_INT_Value), true);
    UpdateAddStat(0x46, static_cast<float>(pMonsterTable->Resist_Dark_INT_Value), true);
    UpdateAddStat(0x47, static_cast<float>(pMonsterTable->Resist_Blue_INT_Value), true);
    UpdateAddStat(0x48, static_cast<float>(pMonsterTable->Resist_Red_INT_Value), true);
    UpdateAddStat(0x49, static_cast<float>(pMonsterTable->Resist_White_INT_Value), true);
    UpdateAddStat(0x4A, static_cast<float>(pMonsterTable->Resist_Green_INT_Value), true);
}

// ============================================================================
// SetFullStat - IDA 0x140039D40
// Verified: Sets HP, SG, ST to their max values
// ============================================================================
void CGocNpcAttribute::SetFullStat()
{
    // Get max HP (stat 10) and set current HP (stat 1)
    float fMaxHP = GetStat(10);
    SetStat(1, fMaxHP, false);

    // Get max SG (stat 14 from IDA? Actually stat 12 is max SG)
    // IDA shows GetStat(0xE) = stat 14 = Max ST being used
    // But decompilation shows it sets stat 3 (ST), not stat 2 (SG)
    // This appears to be: SetStat(1, GetStat(10)), SetStat(3, GetStat(14)), SetStat(2, GetStat(12))
    float fMaxSG = GetStat(12);
    SetStat(2, fMaxSG, false);

    // Get max ST (stat 14) and set current ST (stat 3)
    float fMaxST = GetStat(14);
    SetStat(3, fMaxST, false);
}

// ============================================================================
// SetOriginStat - IDA 0x1401045E0
// Verified: Saves current stats as origin, recalculates, then restores add/scale
// ============================================================================
void CGocNpcAttribute::SetOriginStat()
{
    // Save current add/scale stats
    float fAddStatBackup[MAX_STAT_COUNT];
    float fScaleStatBackup[MAX_STAT_COUNT];
    std::memcpy(fAddStatBackup, m_fAddStat, sizeof(fAddStatBackup));
    std::memcpy(fScaleStatBackup, m_fScaleStat, sizeof(fScaleStatBackup));

    // Clear add/scale and calculate base stats
    std::memset(m_fAddStat, 0, sizeof(m_fAddStat));
    std::memset(m_fScaleStat, 0, sizeof(m_fScaleStat));

    // TODO: Calculate all stats from scratch
    // CCalculateStatus* pCalc = TXSingleton<CCalculateStatus>::Instance();
    // CCalculateStatus::CalculateStatusAll(pCalc, this);

    // Copy final stats to origin stats
    std::memcpy(m_fOriginStat, m_fFinalStat, sizeof(m_fOriginStat));

    // Restore add/scale stats
    std::memcpy(m_fAddStat, fAddStatBackup, sizeof(m_fAddStat));
    std::memcpy(m_fScaleStat, fScaleStatBackup, sizeof(m_fScaleStat));
}

// ============================================================================
// AddStat - IDA 0x140104710
// Verified: Adds a percentage of current stat value
// ============================================================================
void CGocNpcAttribute::AddStat(int nStateID, float fValue)
{
    // Get current stat value
    float fCurrentValue = GetStat(nStateID);

    // Calculate add amount: current * multiplier
    float fAddAmount = static_cast<float>(static_cast<int>(fCurrentValue)) * fValue;

    // Set the new value
    SetStat(nStateID, fCurrentValue + fAddAmount, false);
}

// ============================================================================
// PlusStat - IDA 0x140104770
// Verified: Directly adds value to stat via UpdateAddStat
// ============================================================================
void CGocNpcAttribute::PlusStat(int nStateID, float fValue)
{
    // Simply call base class UpdateAddStat without recalculation
    UpdateAddStat(nStateID, fValue, false);
}

// ============================================================================
// SetStat - IDA 0x1401047C0
// Verified: Calls SetFinalStat with sync flag
// ============================================================================
void CGocNpcAttribute::SetStat(int nStatID, float fValue, bool bSync)
{
    // Simply delegate to SetFinalStat
    SetFinalStat(nStatID, fValue, bSync);
}

// ============================================================================
// SetFinalStat - IDA 0x140104800
// Verified: Sets final stat with validation and sync flag
// ============================================================================
void CGocNpcAttribute::SetFinalStat(int nStatID, float fValue, bool bSync)
{
    // Validate the stat value
    float fValidValue = fValue;
    IsValidStat(nStatID, &fValidValue);

    // Only update if value changed
    if (m_fFinalStat[nStatID] != fValidValue)
    {
        // Set the new value
        m_fFinalStat[nStatID] = fValidValue;

        // Mark for sync (value 2 = broadcast)
        m_nSyncStat[nStatID] = 2;

        // Send update if sync requested
        if (bSync)
        {
            SendUpdateStatList();
        }
    }
}

// ============================================================================
// SendUpdateStatList - IDA 0x1401048A0
// Verified: Sends updated stats to nearby clients
// ============================================================================
void CGocNpcAttribute::SendUpdateStatList()
{
    // TODO: Implement proper packet sending
    // This function:
    // 1. Creates XSendPacket with main=0x17, sub=0x12
    // 2. Builds ST_UPDATE_STAT_LIST structure
    // 3. Gets actor ID from owner (via CQuestCondition::GetQuestID)
    // 4. Iterates g_NpcSyncStat array (4 entries: 1, 2, 3, 10)
    // 5. For each stat in g_NpcSyncStat where m_nSyncStat[statID] > 0:
    //    - Add ST_UPDATE_STAT to list
    //    - Clear sync flag
    // 6. If list not empty, send broadcast packet via CGocNetwork::SendBroadCast

    // Placeholder implementation
    for (int i = 0; i < 4; ++i)
    {
        int nStatID = g_NpcSyncStat[i];
        if (nStatID >= 0 && nStatID < MAX_STAT_COUNT)
        {
            if (m_nSyncStat[nStatID] > 0)
            {
                // TODO: Build packet and send
                // Reset sync flag after sending
                m_nSyncStat[nStatID] = 0;
            }
        }
    }
}

// ============================================================================
// SendUpdateStat - IDA 0x140104A70
// Verified: Sends single stat update to clients
// ============================================================================
void CGocNpcAttribute::SendUpdateStat(int nStatID)
{
    // TODO: Implement proper packet sending
    // This function is similar to SendUpdateStatList but focuses on a specific stat
    // It creates XSendPacket(0x17, 0x12), builds stat list and broadcasts

    (void)nStatID;  // Suppress unused parameter warning
}

// ============================================================================
// SetMonterOriginStat - IDA 0x140105120
// Verified: Copies final stats to monster origin stats
// ============================================================================
void CGocNpcAttribute::SetMonterOriginStat()
{
    std::memcpy(m_fMonsterOriginStat, m_fFinalStat, sizeof(m_fMonsterOriginStat));
}

// ============================================================================
// GetMonsterOriginStat - IDA 0x140105150
// Verified: Returns monster origin stat value or 0 if out of range
// ============================================================================
float CGocNpcAttribute::GetMonsterOriginStat(std::uint16_t wStatID) const
{
    if (wStatID >= MAX_STAT_COUNT)  // 0x4D = 77
        return 0.0f;

    return m_fMonsterOriginStat[wStatID];
}

// ============================================================================
// IsHaveMonsterOriginStat - IDA 0x140105180
// Verified: Checks if monster has a non-zero origin stat
// ============================================================================
bool CGocNpcAttribute::IsHaveMonsterOriginStat(std::uint16_t wStatID) const
{
    // Returns true only if stat ID is valid AND value is greater than 0
    return wStatID < MAX_STAT_COUNT && m_fMonsterOriginStat[wStatID] > 0.0f;
}

// ============================================================================
// ShowInfo - IDA 0x1401051E0
// Verified: Logs all non-zero final stats for debugging
// ============================================================================
void CGocNpcAttribute::ShowInfo()
{
    // Iterate through all stats and log non-zero values
    for (int i = 0; i < MAX_STAT_COUNT; ++i)
    {
        if (m_fFinalStat[i] != 0.0f)
        {
            // TODO: Implement proper logging
            // LogHelper::LogInfo("game.monster", "[MONSTER_STAT] %d %0.2f ", i, m_fFinalStat[i]);
        }
    }
}

// ============================================================================
// GOComponent interface implementations
// ============================================================================
bool CGocNpcAttribute::Initialize()
{
    return CGocAttribute::Initialize();
}

void CGocNpcAttribute::Shutdown()
{
    CGocAttribute::Shutdown();
}

void CGocNpcAttribute::Update(float fDeltaTime)
{
    CGocAttribute::Update(fDeltaTime);
}
