#pragma once

#include "GOComponent.h"
#include <cstdint>
#include <map>
#include <vector>
#include <string>

/**
 * @brief ST_POST_DATA - Post/Mail data structure
 * Used for send/receive/save post lists
 */
struct ST_POST_DATA {
    std::int64_t biSerial = 0;           // Serial ID
    std::int64_t biSendDate = 0;         // Send date
    std::int64_t biDelDate = 0;          // Delete date
    std::uint32_t dwSendUAID = 0;        // Sender UAID
    std::uint32_t dwSendUCID = 0;        // Sender UCID
    std::uint32_t dwRecvUAID = 0;        // Receiver UAID
    std::uint32_t dwRecvUCID = 0;        // Receiver UCID
    char szSendName[64] = {};            // Sender name
    char szRecvName[64] = {};            // Receiver name
    char szTitle[256] = {};              // Mail title
    char szBody[1024] = {};              // Mail body
    std::uint8_t byFlag = 0;             // Flag (read/unread)
    std::uint8_t byType = 0;             // Type
    std::uint8_t byDel = 0;              // Delete flag
    std::int32_t nItemID = 0;            // Attached item ID
    std::int32_t nItemCount = 0;         // Attached item count
    std::int32_t nMoney = 0;             // Attached money
    std::int64_t biItemSerial = 0;       // Item serial
};

/**
 * @brief ST_ACCOUNT_POST_DATA - Account post data structure
 */
struct ST_ACCOUNT_POST_DATA {
    std::int64_t biSerial = 0;           // Serial ID
    std::int64_t biSendDate = 0;         // Send date
    std::uint32_t dwSendUAID = 0;        // Sender UAID
    char szTitle[256] = {};              // Title
    char szBody[1024] = {};              // Body
    std::uint8_t byFlag = 0;             // Flag
    std::uint8_t byType = 0;             // Type
};

/**
 * @brief ST_LEVEL_UP_EVENT_DATA - Level up event data for post
 */
struct ST_LEVEL_UP_EVENT_DATA {
    int nRewardIndex = 0;
    std::uint8_t byRewardType = 0;
    std::uint8_t byRewardState = 0;
};

// Forward declarations
struct PS_ITEM_RESTORE_LIST;

/**
 * @brief CGocPost - Game Object Component for mail/post system
 *
 * Handles in-game mail, messages, and post-related functionality.
 *
 * IDA verified: Component Family ID = 11
 */
class CGocPost : public GOComponent {
public:
    CGocPost();
    virtual ~CGocPost();

    // GOComponent interface
    bool Initialize() override;
    void Shutdown() override;
    void Update(float fDeltaTime) override;

    // Static family ID (IDA: 0x140030260 returns 11)
    static int GetFamilyID();

    // Initialization
    void Init();

    // Post list management
    void AddRecvPost(const ST_POST_DATA& stRecvData);
    void AddSendPost(const ST_POST_DATA& stSendData);
    void AddSavePost(const ST_POST_DATA& stSaveData);
    void AddAccounPost(const ST_ACCOUNT_POST_DATA& stPostData);

    void DelSendPost(std::int64_t biSerial);
    void DelRecvPost(std::int64_t biSerial);
    void DelSavePost(std::int64_t biSerial);
    void DelAccountPost(std::int64_t biSerial);
    void DelRecvPostAll();
    void DelAccountPostAll();

    // Post data retrieval
    bool GetSendPostData(std::int64_t biSerial, ST_POST_DATA& stSendData);
    bool GetRecvPostData(std::int64_t biSerial, ST_POST_DATA& stRecvData);
    bool GetAccountPostData(std::int64_t biSerial, ST_ACCOUNT_POST_DATA& stAccountPostData);

    // Post counts
    std::uint16_t GetSendPostCount() const;
    std::uint16_t GetNewAccountPostCount() const;
    std::int64_t GetRecvSerial() const;
    std::int64_t GetAccountPostSerial() const;
    std::int64_t GetLastSendPost() const;

    // Post checks
    bool IsSendPost(std::int64_t biSerial) const;
    bool IsErrorDBSync() const;

    // Post flags and state
    void SetRecvAccountListCount(std::uint16_t wCount);
    void SetPostListRefreshTime(std::uint8_t byType);
    bool CheckListRefreshTime(std::uint8_t byType);
    void SetDBSync(std::uint32_t nType, bool bSync);

    // Post operations
    std::int64_t GetDelDate(std::int64_t biSerial);
    bool CanRead(std::int64_t biSerial, std::uint8_t& byFlag, bool& bDel);
    bool CanReceipt(std::int64_t biSerial, std::uint8_t& byFlag);
    bool CanSendBack(std::int64_t biSerial, ST_POST_DATA& stRecvData);

    // Database operations
    void SendDBPostList();

    // Restore item
    bool GetLoadRestoreItem() const;
    void SetLoadRestoreItem(bool bLoad);

    // Level up event mail
    void SendLevelUpEvent(int nClass, int nLevel);

protected:
    // Post lists (IDA verified member names)
    std::map<std::int64_t, ST_POST_DATA> m_mpSendList;      // Send list
    std::map<std::int64_t, ST_POST_DATA> m_mpRecvList;      // Receive list
    std::map<std::int64_t, ST_POST_DATA> m_mpSaveList;      // Save list
    std::map<std::int64_t, ST_ACCOUNT_POST_DATA> m_mpAccountList; // Account list

    // Restore item list
    PS_ITEM_RESTORE_LIST* m_psRestoreItemList = nullptr;

    // Level up mail map
    std::map<int, ST_LEVEL_UP_EVENT_DATA> m_mapLevelMail;

    // Status flags (IDA verified)
    bool m_bSendListCheck = false;
    bool m_bRecvListCheck = false;
    bool m_bSaveListCheck = false;
    bool m_bAccountListCheck = false;
    bool m_bLoadRestoreItem = false;
    bool m_bErrorDBSync = false;

    // Post counts (IDA verified)
    std::uint16_t m_wNewPostCount = 0;
    std::uint16_t m_wNewAccountPostCnt = 0;
    std::int32_t m_nSavePostcount = 0;

    // DB sync flags (IDA verified)
    bool m_bSyncDB[4] = {false, false, false, false};

    // Refresh time (IDA verified - static array)
    std::int64_t m_nPostListRefreshTime[4] = {0, 0, 0, 0};
};
