#pragma once

#include "GOComponent.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMail.h"
#include <cstdint>
#include <map>
#include <vector>
#include <string>

// Use ST_POST_DATA and ST_ACCOUNT_POST_DATA from PSServerMail.h

/**
 * @brief ST_LEVEL_UP_EVENT_DATA - Level up event data for post
 * IDA verified: struct at m_mapLevelMail
 */
struct ST_LEVEL_UP_EVENT_DATA {
    int nLv = 0;        // Level value
    int nUCID = 0;      // Character UCID
};

// Forward declarations
struct PS_ITEM_RESTORE_LIST;
struct TB_LEVEL_MAIL;
struct TB_ITEM;
struct ST_CREATE_ITEMS;
class CUser;
class XGameServer;

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
    bool CanAccountPostRead(std::int64_t biSerial, std::uint8_t& byFlag, std::int64_t& biDelDate);
    bool CanRecvDel(std::int64_t biSerial, bool& bDel);
    bool CanSavePost(std::int64_t biSerial, ST_POST_DATA& stSaveData);
    bool CanSaveDel(std::int64_t biSerial, bool& bDel);
    bool CheckAccountPost(std::int64_t biSerial);
    bool CanReadSavePost(std::int64_t biSerial, std::uint8_t& byFlag);

    // Account post operations
    bool CanAccountPostReceipt(std::int64_t biSerial, std::uint8_t& byFlag);
    void PostReceipt(std::int64_t biSerial);
    void SetPostAccountReceipt(std::int64_t biSerial);
    bool CanAccountPostDel(std::int64_t biSerial, bool& bDec, std::int64_t& biDelDate);

    // Post info
    void RecvPostInfo(ST_POST_DATA& stPostData, std::uint16_t wPostCount);

    // GMT operations
    bool CheckGMTSystemPostSendCondition(ST_GMT_POST_CONDITION& stCondition);
    bool DBReqGMTSendPostList(int nRefreshPostType);
    bool GMTSystemPostSend(PS_GMT_POST_LIST& ptSendList);

    // Post delete operations
    void GetDeletePostList(std::uint8_t byPostType, PS_POST_DELETE_ALL_SERVER& psDeleteList);
    void DeletePostAll(PS_POST_DELETE_ALL_SERVER& psPostDeleteList);
    void GetRecvPostList(std::uint8_t byPostType, PS_POST_DELETE_LIST& psResPostList);
    void CheckDeletePost(PS_POST_DELETE_ALL_SERVER& psPostDeleteInfo);

    // Database operations
    void SendDBPostList();
    void SendPostSendList();
    void SendPostRecvList();
    void SendPostAccountList();
    void SendPostSaveList();

    // Restore item
    bool GetLoadRestoreItem() const;
    void SetLoadRestoreItem(bool bLoad);
    void LoadRestoreItem(PS_ITEM_RESTORE_LIST& psRestoreItemList);
    void SendRestorePost();
    void SendRestoreAttendancePost(PS_ITEM_RESTORE_LIST& psRestoreItemList);

    // Remain time management
    void SetRemainTime(std::int64_t biSerial, std::int64_t biRemainTime);
    void SetPostAccountRemainTime(std::int64_t biSerial, std::int64_t biRemainTime);

    // Level up event mail
    void SetLevelUpEvent(int nGroupID, int nLv, int nEventUCID);
    void SendLevelUpEvent(int nClass, int nLv);
    bool SendLevelUpEventPost(TB_LEVEL_MAIL* pTBMail);
    void ResetLevelUpEvent();

    // Auto mail
    bool SendAutoMail(std::uint16_t nAutoMailID);

    // Coupon reward
    void SendCoupounReward(int nItem, std::int16_t nCount, std::uint8_t byType);

    // System post send (multiple overloads)
    bool SystemPostSend(std::uint8_t bySubType, std::uint16_t wType, std::int64_t biEventID, std::uint32_t dwRecvUCID);
    bool SystemPostSend(std::uint8_t bySubType, std::uint16_t wType, std::int64_t biMoney);
    bool SystemPostSend(std::uint32_t nItemID, std::int16_t shCount, std::uint8_t bySubType, std::uint16_t wType, std::uint32_t nNpcID);
    bool SystemPostSend(TB_ITEM* pTBItem, std::int16_t shCount, std::uint8_t bySubType, std::uint16_t wType, std::uint32_t dwEventID, const wchar_t* strTitle);
    bool SystemPostSend(ST_CREATE_ITEMS& stCreateItems, std::uint8_t bySubType, std::uint16_t wType, const wchar_t* strTitle);

    // Account post
    bool AccountPostSend(ST_CREATE_ITEMS& stCreateItems, std::uint8_t bySubType, std::uint16_t wType);

    // Post flag
    void SetPostFlag(std::int64_t biSerial, std::uint8_t byFlag);

    // Save post count
    void SetSavePostCount(int nCount);

    // Post receipt operations (IDA verified)
    void ReqPostReceipt(std::uint32_t dwNpcID, std::int64_t biRecvSerial);
    void ReqPostReceiptAll(std::int64_t biSerial, int& nResult);
    void ReqPostAccountReceiptAll(std::int64_t biSerial, int& nResult);
    void ReceiptPostReceiveList(PS_POST_RECEIPT_ALL_SERVER& psPostReceiptInfo, PS_RES_POST_RECEIPT& psResPostReceiptAllInfo);
    void ReceiptPostAccountList(PS_POST_RECEIPT_ALL_SERVER& psPostReceiptInfo, PS_RES_POST_RECEIPT& psResPostReceiptAllInfo);
    bool CanReceiptAll(std::int64_t biSerial, std::uint8_t& byFlag, std::uint8_t byPostType, bool& bDecrease);

protected:
    // Post lists (IDA verified member names)
    std::map<std::int64_t, ST_POST_DATA> m_mpSendList;      // Send list
    std::map<std::int64_t, ST_POST_DATA> m_mpRecvList;      // Receive list
    std::map<std::int64_t, ST_POST_DATA> m_mpSaveList;      // Save list
    std::map<std::int64_t, ST_ACCOUNT_POST_DATA> m_mpAccountList; // Account list

    // Restore item list (IDA: m_psRestoreItemList is embedded struct, not pointer)
    PS_ITEM_RESTORE_LIST m_psRestoreItemList;

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
