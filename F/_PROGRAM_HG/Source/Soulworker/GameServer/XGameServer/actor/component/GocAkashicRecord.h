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

// Struct definitions are in PSServer headers:
// - ST_AKASHIC_RECORD, ST_AKASHIC_LIST: PSServerDB.h
// - PS_QUICKSLOT_CARD, PS_QUICKSLOT_CARD_VEC: PSServerCore.h
// - ST_STATISTICS_AKASHIC: PSServerDB.h
// - PS_DB_AKASHIC_USE: PSServerDB.h
// - PS_DB_AKASHIC_GETINFO: PSServerMail.h
// - PS_RES_AkashicRecord: PSServerDB.h

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
    bool SetQuickSlotCard(PS_QUICKSLOT_UPDATE_CARD_VEC& psUpdate);
    void GetQuickSlotCard(PS_QUICKSLOT_UPDATE_CARD_VEC& psInfo);
    void GetQuickSlotCard(std::uint32_t* pQuickSlotCard);
    void DisassembleQuickSlotCard(std::uint32_t dwAkashicID);

    // ChangeActiveDeck (0x140020910) - Change active deck
    int ChangeActiveDeck(PS_DECK_ACTIVE& stActive);

    // ChangeDeckName (0x140020ca0) - Change deck name
    int ChangeDeckName(PS_DECK_NAME_VEC& stChange);

    // IsOverlapCard (0x14001fc30) - Check card overlap
    bool IsOverlapCard(std::uint32_t* uniCard);

    // OpenCardDeck (0x1400212d0) - Open card deck
    int OpenCardDeck();

    // IsCombineAkashic (0x14001daf0) - Combine akashic check
    int IsCombineAkashic(PS_ITEM_SLOT_INFO& psMainInfo, PS_ITEM_SLOT_INFOS& psNeedInfos,
                         PS_RES_STORAGE_INFO& psCreateItemList, PS_RES_STORAGE_INFO& psUpdateItemList,
                         std::uint8_t& bySuccess, int& nCreateAkashicID);

    // IsComposeHiddenAkashic (0x14001ef10) - Compose hidden akashic
    int IsComposeHiddenAkashic(PS_ITEM_SLOT_INFO& psMainInfo, PS_ITEM_SLOT_INFO& psNeedInfo,
                               PS_RES_STORAGE_INFO& psCreateItemList, PS_RES_STORAGE_INFO& psUpdateItemList,
                               int& nCreateAkashicID);

    // Save operations (0x14001B4A0, 0x14001B6B0)
    void SaveQuickSlotAll();
    void SaveQuickSlot(std::uint8_t byPage);

    // Slot operations (0x14001BCD0, 0x14001BE10, 0x14001BF00)
    std::uint32_t GetAkashicIDFromSlot(std::uint32_t nSlot);
    bool OverlappedAkashic(std::uint32_t dwID);
    void RemoveExistBuff(std::uint32_t dwExistCard);

    // Deck info operations (0x14001AED0, 0x140021200, 0x140021970)
    void GetAkashicID(std::uint32_t& dwAkashicID);
    void GetDeckName(PS_DECK_NAME_VEC& stDeckNameVec);
    void SetDeckPageInfo(PS_QUICKSLOT_CARD& stCard, std::uint8_t byPage);

    // Quick slot info (0x14001C5C0)
    void GetQuickSlotInfo(PS_QUICKSLOT_CARD_VEC& stQuickSlotCardVec);

    // Akashic get info (0x14001D990)
    void LoadAkashicGetInfo(PS_AKASHIC_GETINFO_LIST& stAkashicGetInfoList);

    // Net cafe event operations (0x14001FDC0 - 0x140020420)
    void CheckEventNetCafeAkashicRecord();
    void CheckEventNetCafeQuickSlot();
    void EventNetCafeAkashicRecordAdd(bool bSend);
    void EventNetCafeAkashicRecordDelete(bool bSend);
    void EventNetCafeQuickSlotDelete(bool bSend);

    // Roguelike and disassemble operations (0x140020780 - 0x140021C90)
    void ResetRoguelikeMode();
    void DisassembleAkashicForCheat(int nDisCount);

    // Accessors
    bool IsLoaded() const { return m_bLoad; }
    int GetRecordCount() const { return static_cast<int>(m_mapAkashic.size()); }
    std::uint8_t GetDeckCount() const { return m_byDeckCount; }
    std::uint8_t GetActiveDeck() const { return m_byActiveDeck; }
    void SetUserLoad(bool bLoad) { m_bUserLoad = bLoad; }
    bool GetUseDisassemble() const { return m_bDisassembleAkashic; }
    void SetUseDisassemble(bool bDisassemble) { m_bDisassembleAkashic = bDisassemble; }

    // Get owner objects - inline implementations using m_pOwner from GOComponent
    CMover* GetOwnerMover() const { return static_cast<CMover*>(m_pOwner); }
    // Note: GetOwnerObject and GetOwnerActor require dynamic_cast since CMover inherits from XActor
    IXObject* GetOwnerObject() const;  // Defined in cpp
    XActor* GetOwnerActor() const;     // Defined in cpp

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
