#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include <cstdint>
#include <map>
#include <vector>
#include <set>

// Forward declarations
struct TB_AKASHIC_RECORDS;
class CMover;
class CUser;
class CMySkillList;

/**
 * @brief ST_AKASHIC_RECORD - Akashic record entry (12 bytes)
 * Restored from IDA: PS_MODE_MAZE_RANKING_FOR_MATCHING structure
 */
struct ST_AKASHIC_RECORD {
    std::uint32_t dwAkashicID = 0;      // Akashic ID
    int nPosition = 0;                  // Position/index (Array_Index from table)
    int nAkashicExp = 0;                // Experience points
};

/**
 * @brief ST_AKASHIC_LIST - Vector of akashic records
 */
struct ST_AKASHIC_LIST {
    std::vector<ST_AKASHIC_RECORD> vecInfo;
};

/**
 * @brief PS_QUICKSLOT_CARD - Quick slot card data for one deck (48 bytes per IDA)
 * Contains 5 card slots per deck, with page/deck index and deck name
 * IDA: sizeof(PS_QUICKSLOT_CARD) = 0x30 = 48 bytes
 *
 * Layout from IDA:
 * - byPage: offset 0, 1 byte
 * - padding: offset 1, 1 byte
 * - szDeckName: offset 2, 26 bytes (wchar_t[13])
 * - uniCard (union): offset 28, 20 bytes (5 x uint32_t)
 */
struct PS_QUICKSLOT_CARD {
    std::uint8_t byPage = 0;            // Deck/page index (offset 0x00)
    std::uint8_t _pad1 = 0;             // Padding (offset 0x01)
    wchar_t szDeckName[13] = {};        // Deck name (offset 0x02, 26 bytes)
    union {
        std::uint32_t nCard[5];         // Card slots as array (offset 0x1C = 28)
        struct {
            std::uint32_t nCard_1;      // Card slot 1 (offset 0x1C)
            std::uint32_t nCard_2;      // Card slot 2 (offset 0x20)
            std::uint32_t nCard_3;      // Card slot 3 (offset 0x24)
            std::uint32_t nCard_4;      // Card slot 4 (offset 0x28)
            std::uint32_t nCard_5;      // Card slot 5 (offset 0x2C)
        };
    };
};

static_assert(sizeof(PS_QUICKSLOT_CARD) == 48, "PS_QUICKSLOT_CARD size must match IDA (0x30 = 48 bytes)");

/**
 * @brief PS_QUICKSLOT_CARD_VEC - Quick slot card vector with active deck
 * IDA: sizeof(PS_QUICKSLOT_CARD_VEC) = 40 bytes
 *
 * Layout from IDA:
 * - byActivePage: offset 0, 1 byte
 * - padding: offset 1-7, 7 bytes
 * - vecInfo: offset 8, 32 bytes (std::vector<PS_QUICKSLOT_CARD>)
 */
struct PS_QUICKSLOT_CARD_VEC {
    std::uint8_t byActivePage = 0;              // Active deck index (offset 0x00)
    std::uint8_t _pad[7] = {};                  // Padding (offset 0x01-0x07)
    std::vector<PS_QUICKSLOT_CARD> vecInfo;     // Deck list (offset 0x08)
};

static_assert(sizeof(PS_QUICKSLOT_CARD_VEC) >= 40, "PS_QUICKSLOT_CARD_VEC size check");

/**
 * @brief ST_STATISTICS_AKASHIC - Statistics packet for akashic
 */
struct ST_STATISTICS_AKASHIC {
    std::uint8_t byFlag = 0;
    std::uint32_t dwUCID = 0;
    std::uint32_t dwAkashicID = 0;
};

/**
 * @brief PS_DB_AKASHIC_USE - DB packet for akashic use
 */
struct PS_DB_AKASHIC_USE {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwAkashicID = 0;
    std::uint8_t byState = 0;
    int nAkashicExp = 0;
};

/**
 * @brief PS_DB_AKASHIC_GETINFO - DB packet for akashic get info
 */
struct PS_DB_AKASHIC_GETINFO {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwAkashicGroupID = 0;
};

/**
 * @brief PS_RES_AkashicRecord - Response packet for akashic activation
 */
struct PS_RES_AkashicRecord {
    std::uint32_t dwAkashicID = 0;
    std::uint32_t uxUseActorID = 0;     // Actor ID of user
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

    // UpdateAkashicPassiveList (0x14001B880) - Update passive list
    void UpdateAkashicPassiveList();

    // GetPassiveAkashicByGrade (0x14001BAF0) - Get passive by cooltime group
    TB_AKASHIC_RECORDS* GetPassiveAkashicByGrade(std::uint32_t dwCoolTimeGroup, int iIndex);

    // ThinkAkashicPassive (0x14001C640) - Process passive effects
    void ThinkAkashicPassive();

    // CheckPassiveSkill (0x14001CA80) - Check and activate passive skill
    bool CheckPassiveSkill(std::uint8_t byCondition);

    // SendAkasicRecordRes (0x14001C8C0) - Send activation response
    void SendAkasicRecordRes(CUser* pUser, TB_AKASHIC_RECORDS* pTBAkashic);

    // RegisterAllAkashicRecord (0x14001CD60) - Register all akashics
    void RegisterAllAkashicRecord();

    // LoadQuickSlotCard (0x14001BFC0) - Load quick slot cards
    bool LoadQuickSlotCard(PS_QUICKSLOT_CARD_VEC& psCardDeck);

    // ReqDisassembleAkashic (0x140019550) - Request disassemble
    bool ReqDisassembleAkashic(std::vector<std::uint32_t>& psList);

    // ResDisassembleAkashic (0x14001A920) - Handle disassemble response
    bool ResDisassembleAkashic(std::vector<std::uint32_t>& psList, std::uint8_t byState);

    // SendDBLog (0x14001D390) - Send log to DB
    void SendDBLog(std::int16_t shSubType, int nParam0, int nParam1, int nParam4, int nParam3);

    // AddAkashicGetInfo (0x14001D500) - Add akashic get info
    void AddAkashicGetInfo(std::uint32_t dwAkashicID);

    // Quick slot operations
    void SetQuickSlotCard(PS_QUICKSLOT_CARD* pCard);
    void GetQuickSlotCard(PS_QUICKSLOT_CARD* pCard);
    void DisassembleQuickSlotCard(std::uint32_t dwAkashicID);

    // Accessors
    bool IsLoaded() const { return m_bLoad; }
    int GetRecordCount() const { return static_cast<int>(m_mapAkashic.size()); }
    std::uint8_t GetDeckCount() const { return m_byDeckCount; }
    std::uint8_t GetActiveDeck() const { return m_byActiveDeck; }
    void SetUserLoad(bool bLoad) { m_bUserLoad = bLoad; }

    // Get owner objects
    CMover* GetOwnerMover() const;
    IXObject* GetOwnerObject() const;
    XActor* GetOwnerActor() const;

protected:
    // Member variables (from IDA structure at 0x140018B80)
    std::map<int, ST_AKASHIC_RECORD> m_mapAkashic;                      // Active akashic records
    std::map<std::uint32_t, TB_AKASHIC_RECORDS*> m_mapAkashicPassive;   // Passive akashic records (ID -> table ptr)
    std::vector<TB_AKASHIC_RECORDS*> m_vecSortedAkashicPassive;         // Sorted passive list (table ptrs)
    std::set<std::uint32_t> m_setAkashicGetInfo;                        // Get info set (Akashic_Group IDs)

    PS_QUICKSLOT_CARD m_psQuickSlotCard[5];                             // Quick slot cards (5 decks)
    std::uint8_t m_byDeckCount = 0;                                     // Deck count
    std::uint8_t m_byActiveDeck = 0;                                    // Active deck index

    bool m_bLoad = false;                                               // Data loaded flag
    bool m_bUserLoad = false;                                           // User data loaded
    bool m_bDisassembleAkashic = false;                                 // Disassemble in progress
};
