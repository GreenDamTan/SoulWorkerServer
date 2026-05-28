// CGocNpcAttribute - NPC Attribute Game Object Component
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140039D40 - 0x140105260

#pragma once

#include "GocAttribute.h"
#include <cstdint>

// Forward declarations
struct TB_MONSTER;
struct TB_NPC;

// Global NPC sync stat array - defines which stats should be synchronized
// Address: 0x140D1D430 in GameServer.exe
// Values: { 1, 2, 3, 10 } = HP, SG, ST, MaxHP
extern const int g_NpcSyncStat[4];

/**
 * @brief CGocNpcAttribute - NPC-specific attribute component
 *
 * Extends CGocAttribute with NPC/monster specific functionality.
 * Handles initialization from TB_MONSTER and TB_NPC tables,
 * and provides NPC-specific stat management.
 */
class CGocNpcAttribute : public CGocAttribute {
public:
    // Constructor - IDA 0x140103F00
    CGocNpcAttribute();

    // Destructor - IDA 0x140103F70
    virtual ~CGocNpcAttribute();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Initialization from table data
    // IDA 0x140103FA0 - Initialize from monster table with class
    void Init(std::uint16_t nClass, const TB_MONSTER* pMonsterTable);

    // IDA 0x1401042E0 - Initialize from NPC table
    void Init(std::uint16_t nClass, const TB_NPC* pNPCTable);

    // IDA 0x140104460 - Initialize from monster table with class and level
    void Init(int nClass, int nLevel, const TB_MONSTER* pMonsterTable);

    // Override virtual functions from base class
    // IDA 0x140039D40 - Set HP/SG/ST to max values
    void SetFullStat() override;

    // IDA 0x1401045E0 - Set origin stat for NPC
    void SetOriginStat() override;

    // IDA 0x140104710 - Add stat value (multiply current by fValue)
    void AddStat(int nStateID, float fValue) override;

    // IDA 0x140104770 - Add stat value directly (calls UpdateAddStat)
    virtual void PlusStat(int nStateID, float fValue);

    // IDA 0x1401047C0 - Set stat with sync flag
    void SetStat(int nStatID, float fValue, bool bSync) override;

    // IDA 0x140104800 - Set final stat with validation and sync
    void SetFinalStat(int nStatID, float fValue, bool bSync) override;

    // IDA 0x1401048A0 - Send updated stat list to clients
    void SendUpdateStatList() override;

    // IDA 0x140104A70 - Send single stat update
    virtual void SendUpdateStat(int nStatID);

    // IDA 0x140104C40 - Set monster status table values
    void SetMonsterStatusTable(const TB_MONSTER* pMonsterTable);

    // Monster origin stat functions
    // IDA 0x140105120 - Copy final stats to monster origin stats
    void SetMonterOriginStat();

    // IDA 0x140105150 - Get monster origin stat value
    virtual float GetMonsterOriginStat(std::uint16_t wStatID) const;

    // IDA 0x140105180 - Check if monster has origin stat
    virtual bool IsHaveMonsterOriginStat(std::uint16_t wStatID) const;

    // IDA 0x1401051E0 - Show info (debug logging)
    void ShowInfo();

protected:
    // Monster-specific origin stats (additional to base class)
    float m_fMonsterOriginStat[MAX_STAT_COUNT];
};
