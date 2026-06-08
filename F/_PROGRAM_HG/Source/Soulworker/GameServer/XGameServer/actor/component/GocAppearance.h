#pragma once

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/GameServer/XCore/XServer/XSendPacket.h"
#include "Soulworker/GameServer/XCore/XServer/XSendDBPacket.h"
#include <map>
#include <cstdint>

// Forward declarations
class CGocInventory;
class CUser;
class XGameServer;

// Appearance info structure (single appearance item)
struct ST_APPEARANCE_INFO
{
    unsigned short wAppearanceID = 0;      // Appearance item ID
    __int64 biEndDate = 0;                 // Expiration date (0 = permanent)
};

// Appearance list structure (multiple appearance items)
struct ST_APPEARANCE_LIST
{
    std::vector<ST_APPEARANCE_INFO> vecInfo;

    ST_APPEARANCE_LIST() = default;
    ~ST_APPEARANCE_LIST() = default;
};

// Appearance data stored in player info
struct UAppearanceData
{
    unsigned short wAppearanceID[4] = {0, 0, 0, 0};  // 4 appearance slots
};

// Extended appearance structure
struct UAppearanceEx
{
    union
    {
        UAppearanceData stAppearance;
        __int64 biAppearance = 0;  // 64-bit bitmask for appearance slots
    };
};

/**
 * CGocAppearance - Game Object Component for character appearance customization
 * 
 * This component manages character appearance customization including:
 * - Appearance item storage and retrieval
 * - Appearance equip/unequip operations
 * - Appearance expiration checking
 * - Database synchronization for appearance data
 */
class CGocAppearance : public GOComponent
{
public:
    CGocAppearance();
    virtual ~CGocAppearance();

    // GOComponent interface
    virtual void Init() override;
    virtual void OnUpdate() override;
    virtual void LogOut();

    // =====================================================
    // Database Operations
    // =====================================================
    
    /**
     * Send request to load appearance data from database
     * IDA: 0x1400BB160
     */
    void SendDBAppearanceLoad();

    /**
     * Load appearance list from database response
     * IDA: 0x1400BB270
     * @param stList Appearance list from DB
     */
    void LoadAppearanceList(ST_APPEARANCE_LIST& stList);

    // =====================================================
    // Appearance Management
    // =====================================================

    /**
     * Add appearance items from list
     * IDA: 0x1400BB640
     * @param stList List of appearance items to add
     */
    void AddAppearance(ST_APPEARANCE_LIST& stList);

    /**
     * Add single appearance item
     * IDA: 0x1400BB800
     * @param wAppearanceID Appearance ID
     * @param biEndDate Expiration date (0 = permanent)
     */
    void AddAppearance(unsigned short wAppearanceID, __int64 biEndDate);

    /**
     * Update appearance expiration time
     * IDA: 0x1400BB8C0
     * @param wAppearanceID Appearance ID
     * @param biAddSec Seconds to add
     * @param biEndDate Output: new end date
     */
    void UpdateAppearance(unsigned short wAppearanceID, __int64 biAddSec, __int64& biEndDate);

    /**
     * Check if player has specific appearance
     * IDA: 0x1400BB6E0
     * @param wAppearanceID Appearance ID to check
     * @return true if player has this appearance and it's not expired
     */
    bool IsHaveAppearance(unsigned short wAppearanceID);

    /**
     * Equip appearance to player
     * IDA: 0x1400BB9D0
     * @param reqAppearance Appearance data to equip
     */
    void EquipAppearance(UAppearanceEx& reqAppearance);

    /**
     * Send equipped appearance to nearby players
     * IDA: 0x1400BBC70
     */
    void SendEquipAppearance();

    /**
     * Update appearance on periodic check (expire old appearances)
     * IDA: 0x1400BBDD0
     */
    void OnAppearanceUpdate();

    // =====================================================
    // Appearance Item Usage
    // =====================================================

    /**
     * Check if appearance item can be used
     * IDA: 0x1400DC260
     * @param pItem Item to check
     * @return true if item can be used
     */
    bool CanUseItemAppearance(const std::tr1::shared_ptr<CItem>& pItem);

    /**
     * Use appearance item
     * IDA: 0x1400DC450
     * @param pItem Item to use
     * @return true on success
     */
    bool UseItemAppearance(const std::tr1::shared_ptr<CItem>& pItem);

private:
    // Map of appearance ID to expiration date
    std::map<unsigned short, __int64> m_mpAppearanceList;

    // Appearance slots currently equipped (stored in user's STMyCharInfoEx)
    // UAppearanceEx is stored in STMyCharInfoEx.stBaseInfo.uAppearanceEx
};
