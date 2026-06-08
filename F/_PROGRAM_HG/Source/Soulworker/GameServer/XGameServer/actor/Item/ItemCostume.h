#pragma once

#include "Soulworker/GameServer/XGameServer/XGameServer/Item/CItem.h"
#include <cstdint>

// Forward declarations
class CUser;
class XGameServer;

// Broach structure for costume enhancement
struct ST_ITEM_BROACH
{
    unsigned int dwBroachID = 0;        // Broach item ID
    __int64 biSerial = 0;               // Broach serial number
    unsigned char bySlot = 0;           // Slot index (0-4)

    ST_ITEM_BROACH() = default;
    ~ST_ITEM_BROACH() = default;
};

// Costume set data from TB_ITEM_COSTUMESET
struct TB_ITEM_COSTUMESET
{
    unsigned int dwCostumeSetID = 0;
    unsigned int dwBuffID[5] = {0, 0, 0, 0, 0};  // Buff IDs for each slot
    // ... other fields
};

/**
 * CItemCostume - Costume item class
 * 
 * Costume items are special equipment that provide:
 * - Visual appearance changes
 * - Set bonuses when wearing multiple pieces
 * - Broach socket system for additional enhancements
 * - Gesture and equalizer effects
 * 
 * IDA Base Address: 0x140287F20 (constructor)
 */
class CItemCostume : public CItem
{
public:
    CItemCostume();
    virtual ~CItemCostume();

    // =====================================================
    // Costume-specific functionality
    // =====================================================

    /**
     * Check if broach can be activated in slot
     * IDA: 0x140288080
     * @param bySlot Slot index (0-4)
     * @return true if broach can be activated
     */
    bool CanBroachActive(unsigned char bySlot);

    /**
     * Set costume effect (visual/buff activation)
     * IDA: 0x1402883A0
     * @param pUser User wearing the costume
     */
    void SetEffect(CUser* pUser);

    /**
     * Unset costume effect (visual/buff deactivation)
     * IDA: 0x140288600
     * @param pUser User removing the costume
     */
    void UnsetEffect(CUser* pUser);

    // =====================================================
    // Broach Management
    // =====================================================

    /**
     * Get broach in specific slot
     * @param bySlot Slot index (0-4)
     * @return Broach data, or nullptr if empty
     */
    const ST_ITEM_BROACH* GetBroach(unsigned char bySlot) const;

    /**
     * Set broach in slot
     * @param bySlot Slot index (0-4)
     * @param dwBroachID Broach item ID
     * @param biSerial Broach serial number
     * @return true on success
     */
    bool SetBroach(unsigned char bySlot, unsigned int dwBroachID, __int64 biSerial);

    /**
     * Remove broach from slot
     * @param bySlot Slot index (0-4)
     * @return true on success
     */
    bool RemoveBroach(unsigned char bySlot);

    // =====================================================
    // Costume Set System
    // =====================================================

    /**
     * Check if costume is part of a set
     * @return true if costume belongs to a set
     */
    bool IsCostumeSet() const;

    /**
     * Get costume set ID
     * @return Costume set ID, or 0 if not part of set
     */
    unsigned int GetCostumeSetID() const;

    /**
     * Get buff ID for specific slot
     * @param bySlot Slot index (0-4)
     * @return Buff ID for slot
     */
    unsigned int GetSetBuffID(unsigned char bySlot) const;

    // =====================================================
    // Costume Gesture/Equalizer
    // =====================================================

    /**
     * Get costume gesture info
     * IDA: 0x1400E1AE0 (in CGocInventory)
     * @param dwItemID Costume item ID
     * @param dwGestureID Output: gesture ID
     * @return true if costume has gesture
     */
    static bool GetCostumeGestureInfo(unsigned int dwItemID, unsigned int& dwGestureID);

    /**
     * Get costume equalizer info
     * IDA: 0x1400E2600 (in CGocInventory)
     * @param dwItemID Costume item ID
     * @param dwEqualizerID Output: equalizer ID
     * @return true if costume has equalizer
     */
    static bool GetCostumeEqualizerInfo(unsigned int dwItemID, unsigned int& dwEqualizerID);

private:
    // Broach sockets (5 slots)
    ST_ITEM_BROACH m_stItemBroach[5];

    // Costume set buff IDs (from TB_ITEM_COSTUMESET)
    unsigned int m_dwSetBuffID[5] = {0, 0, 0, 0, 0};

    // Costume set ID
    unsigned int m_dwCostumeSetID = 0;
};
