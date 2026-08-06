// SocialItemObject.h
// CSocialItemObject - Social item object class
// IDA decompilation from GameServer.exe
//
// Functions:
// - CSocialItemObject::CreateObject (0x14018b580)
// - CSocialItemObject::GetTypeId (0x14018b5e0)
// - CSocialItemObject::InitComponant (0x14018bc10)
// - CSocialItemObject::BuildInfoPacket (0x14018bc70)
// - CSocialItemObject::SetInfoPacket (0x14018bde0)
// - CSocialItemObject::AddUser (0x14018bec0)
// - CSocialItemObject::IsExistUser (0x14018c2a0)

#pragma once

#include <cstdint>
#include <vector>
#include <map>

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

// Forward declarations
class VType;
class CGocAttribute;
class CTextDBLog;
class XArea;

struct PS_SOCIALITEM_PLAY;
struct PS_SOCIAL_ITEM_PLAY_START;
struct PS_SOCIALITEM_USER;
struct ST_SOCIALITEM_CARD;

// ============================================================================
// PS_SOCIALITEM_PLAY - Social item play packet
// ============================================================================
struct PS_SOCIALITEM_PLAY {
    int nType = 0;                      // 0=normal play, 1=turn change
    std::uint32_t dwCardID = 0;         // Selected card ID
    bool bFinish = false;               // Game finished flag
    
    struct {
        bool bTurn = false;             // Is owner's turn
        int nTotalPoint = 0;            // Owner's total points
    } psOwnerInfo;
    
    struct {
        int nTotalPoint = 0;            // Guest's total points
    } psGuestInfo;
};

// ============================================================================
// PS_SOCIAL_ITEM_PLAY_START - Social item play start packet
// ============================================================================
struct PS_SOCIAL_ITEM_PLAY_START {
    std::vector<std::uint32_t> vecMission;  // Card IDs for the game
};

// ============================================================================
// PS_SOCIALITEM_USER - Social item user info
// ============================================================================
struct PS_SOCIALITEM_USER {
    std::uint32_t dwUCID = 0;           // User character ID
    int nPoint = 0;                     // User's points
};

// ============================================================================
// ST_SOCIALITEM_CARD - Social item card info
// ============================================================================
struct ST_SOCIALITEM_CARD {
    std::uint32_t dwCardID = 0;         // Card ID
    std::uint8_t byType = 0;            // Card type
};

// Social object state enumeration
enum E_SOCIAL_OBJECT_STATE {
    E_SOCIAL_OBJECT_STATE_NONE = 0,
    E_SOCIAL_OBJECT_STATE_WAIT = 1,
    E_SOCIAL_OBJECT_STATE_READY = 2,
    E_SOCIAL_OBJECT_STATE_PLAY = 3,
    E_SOCIAL_OBJECT_STATE_START = 4,
    E_SOCIAL_OBJECT_STATE_FINISH = 5
};

// Social object type enumeration
enum E_SOCIAL_OBJECT_TYPE {
    E_SOCIAL_OBJECT_TYPE_NORMAL = 1,
    E_SOCIAL_OBJECT_TYPE_OWNER_FIRST = 2,
    E_SOCIAL_OBJECT_TYPE_PARTY = 3
};

// ============================================================================
// ST_SOCIAL_ITEM_INFO - Social item info structure
// ============================================================================
struct ST_SOCIAL_ITEM_INFO {
    std::uint32_t dwObjectID = 0;       // Object ID
    std::uint32_t dwOwnerID = 0;        // Owner ID
    std::uint32_t dwItemID = 0;         // Item ID
    std::uint16_t wSocialItemID = 0;    // Social item ID
    std::int64_t lRemainTime = 0;       // Remaining time
    std::vector<std::uint32_t> vecUsers; // User list

    // Used slot info (4 slots)
    struct ST_USED_SLOT {
        std::uint32_t dwUserID = 0;
        std::uint8_t byAniIndex = 0;    // Animation index
    } stUsedSlot[4];
};

// ============================================================================
// ST_SOCIAL_ITEM_RES - Social item response structure
// ============================================================================
struct ST_SOCIAL_ITEM_RES {
    ST_SOCIAL_ITEM_INFO itemInfo;
    float fPosX = 0.0f;
    float fPosY = 0.0f;
    float fPosZ = 0.0f;
    float fRot = 0.0f;
};

// ============================================================================
// CSocialItemObject - Social item game object
// Objects like chairs, benches that multiple players can use
// ============================================================================
class CSocialItemObject {
public:
    // === Static Factory ===

    // CreateObject - Create a new social item object
    // IDA: ?CreateObject@CSocialItemObject@@SAPEAVVTypedObject@@XZ @ 0x14018B580
    static CSocialItemObject* CreateObject();

    // === Virtual Methods ===

    // GetTypeId - Get type ID for RTTI
    // IDA: ?GetTypeId@CSocialItemObject@@UEBAPEAUVType@@XZ @ 0x14018B5E0
    virtual VType* GetTypeId() const;

    // === Initialization ===

    // InitComponant - Initialize components (creates CGocAttribute)
    // IDA: ?InitComponant@CSocialItemObject@@QEAAXXZ @ 0x14018BC10
    void InitComponant();

    // === Info Methods ===

    // BuildInfoPacket - Build info packet for network transmission
    // IDA: ?BuildInfoPacket@CSocialItemObject@@QEAAXAEAUST_SOCIAL_ITEM_RES@@@Z @ 0x14018BC70
    void BuildInfoPacket(ST_SOCIAL_ITEM_RES* stInfo);

    // SetInfoPacket - Set info packet for network send
    // IDA: ?SetInfoPacket@CSocialItemObject@@UEAAXAEAVXSendPacket@@@Z @ 0x14018BDE0
    virtual void SetInfoPacket(XSendPacket& xSendPacket);

    // === User Management ===

    // AddUser - Add a user to the social item
    // IDA: ?AddUser@CSocialItemObject@@QEAA_NKAEAHH@Z @ 0x14018BEC0
    bool AddUser(std::uint32_t dwActorID, int* nSlot, int byAniIndex);

    // IsExistUser - Check if user exists in the social item
    // IDA: ?IsExistUser@CSocialItemObject@@QEAA_NK@Z @ 0x14018C2A0
    bool IsExistUser(std::uint32_t dwActorID);

    // DeleteUser - Delete a user from the social item
    // IDA: ?DeleteUser@CSocialItemObject@@QEAA_NK@Z @ 0x14018C320
    bool DeleteUser(std::uint32_t dwActorID);

    // SetSocialType - Set social type
    // IDA: ?SetSocialType@CSocialItemObject@@QEAAXE@Z @ 0x14018C850
    void SetSocialType(std::uint8_t bySocialType);

    // IsFunniture - Check if this is furniture (type 1 or 2)
    // IDA: ?IsFunniture@CSocialItemObject@@QEAA_NXZ @ 0x14018C870
    bool IsFunniture();

    // SetInfoLeavePacket - Set leave info packet
    // IDA: ?SetInfoLeavePacket@CSocialItemObject@@QEAAXAEAVXSendPacket@@@Z @ 0x14018BE90
    void SetInfoLeavePacket(XSendPacket& xSendPacket);

    // === Furniture Methods ===

    // SetFurnitureInfo - Set furniture max user count
    // IDA: ?SetFurnitureInfo@CSocialItemObject@@QEAAXH@Z @ 0x14018C830
    void SetFurnitureInfo(std::uint8_t nMaxUseNum);

    // === Play Methods ===

    // SendPlayInfo - Send play info to all users
    // IDA: ?SendPlayInfo@CSocialItemObject@@QEAAXUPS_SOCIALITEM_PLAY@@@Z @ 0x14018C8A0
    void SendPlayInfo(PS_SOCIALITEM_PLAY* psPlayInfo);

    // GetPlayNextTurn - Get next turn player UCID
    // IDA: ?GetPlayNextTurn@CSocialItemObject@@QEAAKXZ @ 0x14018CA10
    std::uint32_t GetPlayNextTurn();

    // IsUsePlaySocialItem - Check if social item can be used for play
    // IDA: ?IsUsePlaySocialItem@CSocialItemObject@@QEAA_NXZ @ 0x14018DAB0
    bool IsUsePlaySocialItem();

    // GetPlayGuestID - Get guest player UCID
    // IDA: ?GetPlayGuestID@CSocialItemObject@@QEAAKXZ @ 0x14018DAF0
    std::uint32_t GetPlayGuestID();

    // FinishPlaySocialItemObject - Finish play and send rewards
    // IDA: ?FinishPlaySocialItemObject@CSocialItemObject@@QEAAXK@Z @ 0x14018D3B0
    void FinishPlaySocialItemObject(std::uint32_t dwLeaveUCID);

    // StartPlaySocialItem - Start play social item game
    // IDA: ?StartPlaySocialItem@CSocialItemObject@@QEAA_NUPS_SOCIAL_ITEM_PLAY_START@@@Z @ 0x14018DF00
    bool StartPlaySocialItem(PS_SOCIAL_ITEM_PLAY_START* psStart);

    // SendStartInfo - Send start info to all users
    // IDA: ?SendStartInfo@CSocialItemObject@@QEAAXUPS_SOCIAL_ITEM_PLAY_START@@@Z @ 0x14018E1C0
    void SendStartInfo(PS_SOCIAL_ITEM_PLAY_START* psStartInfo);

    // IsPlayGame - Check and process play game
    // IDA: ?IsPlayGame@CSocialItemObject@@QEAAHKUPS_SOCIALITEM_PLAY@@@Z @ 0x14018CA50
    int IsPlayGame(std::uint32_t dwUCID, PS_SOCIALITEM_PLAY* psPlay);

    // AddPlayUserInfo - Add play user info
    // IDA: ?AddPlayUserInfo@CSocialItemObject@@QEAAXUPS_SOCIALITEM_USER@@@Z @ 0x14018DBA0
    void AddPlayUserInfo(PS_SOCIALITEM_USER* psInfo);

    // GetOtherInfo - Get other player's UCID
    // IDA: ?GetOtherInfo@CSocialItemObject@@QEAAKK@Z @ 0x14018DE70
    std::uint32_t GetOtherInfo(std::uint32_t dwUCID);

    // CheckRemainTime - Check and update remaining time
    // IDA: ?CheckRemainTime@CSocialItemObject@@QEAA_NM@Z @ 0x14018C7B0
    bool CheckRemainTime(float fElapsedTime);

    // GetOwnerID - Get owner ID
    // IDA: ?GetOwnerID@CSocialItemObject@@QEAAKXZ @ 0x14018FC40
    std::uint32_t GetOwnerID();

    // GetRadius - Get radius
    // IDA: ?GetRadius@CSocialItemObject@@QEAAMXZ @ 0x1402D36C0
    float GetRadius();

    // SetMaxCount - Set max user count
    // IDA: ?SetMaxCount@CSocialItemObject@@QEAAXE@Z @ 0x1402D3730
    void SetMaxCount(std::uint8_t byCount);

    // SetRadius - Set radius
    // IDA: ?SetRadius@CSocialItemObject@@QEAAXM@Z @ 0x1402D3750
    void SetRadius(float fRadius);

    // GetSocialItemID - Get social item ID
    // IDA: ?GetSocialItemID@CSocialItemObject@@QEAAGXZ @ 0x140601C40
    std::uint16_t GetSocialItemID();

    // SetRemainTime - Set remaining time
    // IDA: ?SetRemainTime@CSocialItemObject@@QEAAXK@Z @ 0x140601CB0
    void SetRemainTime(std::uint32_t dwDuration);

    // SetItemSerialID - Set item serial ID
    // IDA: ?SetItemSerialID@CSocialItemObject@@QEAAX_J@Z @ 0x140601CD0
    void SetItemSerialID(std::int64_t xSerial);

    // SetItemID - Set item ID
    // IDA: ?SetItemID@CSocialItemObject@@QEAAXK@Z @ 0x140601CF0
    void SetItemID(std::uint32_t dwItemID);

    // GetSocialPlayState - Get social play state
    // IDA: ?GetSocialPlayState@CSocialItemObject@@QEAA?AW4E_SOCIAL_OBJECT_STATE@@XZ @ 0x1402D3A60
    E_SOCIAL_OBJECT_STATE GetSocialPlayState();

    // GetItemInfo - Get item info reference
    // IDA: ?GetItemInfo@CSocialItemObject@@QEAAAEAUST_SOCIAL_ITEM_INFO@@XZ @ 0x1402D3A80
    ST_SOCIAL_ITEM_INFO& GetItemInfo();

    // GetItemSerialID - Get item serial ID
    // IDA: ?GetItemSerialID@CSocialItemObject@@QEAA_JXZ @ 0x14048CF90
    std::int64_t GetItemSerialID();

    // Init - Initialize social item with area, position, owner
    // IDA: ?Init@CSocialItemObject@@QEAAXPEAVXArea@@AEAUXVec3@@MKG@Z @ 0x14018B890
    void Init(class XArea* pArea, struct XVec3* vecPos, float fRot, std::uint32_t dwOwnerID, std::uint16_t wItemID);

    // EndProcess - End process and cleanup users
    // IDA: ?EndProcess@CSocialItemObject@@QEAAXXZ @ 0x14018C6D0
    void EndProcess();

    // GetActorID - Get actor ID
    // IDA: ?GetActorID@CSocialItemObject@@UEAA?ATUXActorID@@XZ @ 0x14018BC40
    std::uint32_t GetActorID();

    // === Accessors ===
    const ST_SOCIAL_ITEM_INFO& GetItemInfo() const { return m_itemInfo; }
    void SetItemInfo(const ST_SOCIAL_ITEM_INFO& info) { m_itemInfo = info; }

    std::uint8_t GetSocialType() const { return m_bySocialType; }

    std::uint8_t GetMaxUserCount() const { return m_byMaxUserCount; }
    void SetMaxUserCount(std::uint8_t byCount) { m_byMaxUserCount = byCount; }

protected:
    // Constructor (called by CreateObject)
    CSocialItemObject();

    // Member variables
    // Note: CSocialItemObject inherits from XActor, size 0xED08 bytes
    ST_SOCIAL_ITEM_INFO m_itemInfo;    // Item info
    float m_vPosition[3] = {0.0f};      // Position (x, y, z)
    float m_fRot = 0.0f;                // Rotation
    float m_fRadius = 0.0f;             // Radius
    std::int64_t m_xItemSerial = 0;     // Item serial ID
    std::uint8_t m_bySocialType = 1;    // Social type (1=normal, 2=owner first, 3=party)
    std::uint8_t m_byMaxUserCount = 1;  // Max user count
    std::uint8_t m_byPlayState = 0;     // Play state
    std::uint8_t m_byPlayCount = 0;     // Play count
    std::uint8_t m_byMaxPlayCount = 0;  // Max play count
    std::uint8_t m_byReverseCount = 0;  // Reverse count
    std::uint8_t m_byTurnCnt = 0;       // Turn count
    std::uint8_t m_byBonus = 0;         // Bonus count
    std::uint32_t m_dwTurnUCID = 0;     // Current turn UCID
    std::uint32_t m_dwCheckCardID = 0;  // Check card ID
    bool m_bSendLogDB = false;          // Send log to DB flag

    // Card info map for card matching game (cardID -> useCount)
    std::map<std::uint32_t, std::uint8_t> m_mpCardInfo;
    
    // Player info vector for multiplayer games
    std::vector<PS_SOCIALITEM_USER> m_vecPlayerInfo;
    
    // Text DB log for game logging
    CTextDBLog* m_textDBLog = nullptr;

private:
    // Static type info for RTTI
    static VType* classCSocialItemObject;
};
