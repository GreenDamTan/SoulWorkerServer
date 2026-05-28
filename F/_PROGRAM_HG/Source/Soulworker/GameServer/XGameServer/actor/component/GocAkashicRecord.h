#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <cstdint>
#include <map>
#include <vector>

// Forward declarations
struct TB_AKASHIC_RECORDS;
class CMover;

/**
 * @brief ST_AKASHIC_RECORD - Akashic record entry
 */
struct ST_AKASHIC_RECORD {
    std::uint32_t dwAkashicID;      // Akashic ID
    int nPosition;                  // Position/index
    int nAkashicExp;                // Experience points
};

/**
 * @brief PS_QUICKSLOT_CARD - Quick slot card data
 */
struct PS_QUICKSLOT_CARD {
    std::uint32_t dwAkashicID;
    std::uint8_t bySlotIndex;
    std::uint8_t byActive;
};

/**
 * @brief CGocAkashicRecord - Game Object Component for Akashic Record system
 *
 * Handles Akashic Record unlocks, progression, and bonuses.
 * Restored from GameServer.exe IDA decompilation.
 * Address range: 0x140018B80 - 0x140021DC3
 */
class CGocAkashicRecord : public GOComponent {
public:
    // Constructor (0x140018B80)
    CGocAkashicRecord();

    // Destructor (0x140018C70)
    virtual ~CGocAkashicRecord();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Static method - Get family ID
    static int GetFamilyID() { return 12; }

    // Init (0x140018CF0) - Initialize and clear
    bool Init();

    // Clear (0x140018D10) - Clear all data
    void Clear();

    // SendDBAkashicRecordLoad (0x140018DB0) - Request load from DB
    void SendDBAkashicRecordLoad();

    // ResAkashicRecordLoad (0x140018EC0) - Handle DB load response
    void ResAkashicRecordLoad(ST_AKASHIC_LIST& stAkashicList);

    // SendAkasicRecordList (0x140019000) - Send list to client
    void SendAkasicRecordList();

    // AddAkashicRecord (0x140019170) - Add new akashic record
    bool AddAkashicRecord(std::uint32_t nItemID, int nAkashicExp);

    // Reset (0x14001D0D0) - Reset all akashic records
    void Reset();

    // Quick slot operations
    void SetQuickSlotCard(PS_QUICKSLOT_CARD* pCard);
    void GetQuickSlotCard(PS_QUICKSLOT_CARD* pCard);

    // Accessors
    bool IsLoaded() const { return m_bLoad; }
    int GetRecordCount() const { return static_cast<int>(m_mapAkashic.size()); }

    // Get owner mover
    CMover* GetOwnerMover() const;

protected:
    // Member variables (from IDA structure at 0x140018B80)
    std::map<int, ST_AKASHIC_RECORD> m_mapAkashic;              // Active akashic records
    std::map<int, ST_AKASHIC_RECORD> m_mapAkashicPassive;       // Passive akashic records
    std::vector<ST_AKASHIC_RECORD> m_vecSortedAkashicPassive;   // Sorted passive list
    std::map<std::uint16_t, std::uint8_t> m_setAkashicGetInfo;  // Get info set

    PS_QUICKSLOT_CARD m_psQuickSlotCard[5];                     // Quick slot cards (5 decks)
    std::uint8_t m_byDeckCount;                                 // Deck count
    std::uint8_t m_byActiveDeck;                                // Active deck index

    bool m_bLoad;                                               // Data loaded flag
    bool m_bUserLoad;                                           // User data loaded
    bool m_bDisassembleAkashic;                                 // Disassemble in progress
};
