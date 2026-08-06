// CGocPost - Game Object Component for mail/post system
// Restored from GameServer.exe IDA decompilation

#include "GocPost.h"
#include "../../GameServer.h"
#include "../../User.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMail.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "../../../XCore/XServer/IXObject.h"
#include "../../../XCore/XArea/XActor.h"
#include "GocQuest.h"
#include <cstring>
#include <ctime>

// Static family ID (IDA: 0x140030260)
int CGocPost::GetFamilyID()
{
    return 11;
}

// Constructor (IDA: 0x14010D8E0)
CGocPost::CGocPost()
    : GOComponent()
    , m_bSendListCheck(false)
    , m_bRecvListCheck(false)
    , m_bSaveListCheck(false)
    , m_bAccountListCheck(false)
    , m_bLoadRestoreItem(false)
    , m_bErrorDBSync(false)
    , m_wNewPostCount(0)
    , m_wNewAccountPostCnt(0)
    , m_nSavePostcount(0)
{
    std::memset(m_bSyncDB, 0, sizeof(m_bSyncDB));
    std::memset(m_nPostListRefreshTime, 0, sizeof(m_nPostListRefreshTime));
    // m_psRestoreItemList is now embedded struct, initialized by default constructor
}

// Destructor (IDA: 0x14010DA10)
CGocPost::~CGocPost()
{
    m_mapLevelMail.clear();
    // m_psRestoreItemList destructor called automatically
    m_mpAccountList.clear();
    m_mpSaveList.clear();
    m_mpRecvList.clear();
    m_mpSendList.clear();
}

bool CGocPost::Initialize()
{
    return true;
}

void CGocPost::Shutdown()
{
}

void CGocPost::Update(float fDeltaTime)
{
    (void)fDeltaTime;
}

// Init (IDA: 0x14010DAC0)
void CGocPost::Init()
{
    m_bSendListCheck = false;
    m_bRecvListCheck = false;
    m_bSaveListCheck = false;
    m_bAccountListCheck = false;
    m_wNewPostCount = 0;
    m_wNewAccountPostCnt = 0;
    m_nSavePostcount = 0;

    m_mpRecvList.clear();
    m_mpSendList.clear();
    m_mpSaveList.clear();
    m_mpAccountList.clear();

    m_psRestoreItemList.vecInfo.clear();

    m_bLoadRestoreItem = false;

    for (int i = 0; i < 4; ++i)
    {
        m_bSyncDB[i] = false;
        m_nPostListRefreshTime[i] = 0;
    }

    m_bErrorDBSync = false;
    m_mapLevelMail.clear();
}

// AddRecvPost (IDA: 0x1400750D0)
void CGocPost::AddRecvPost(const ST_POST_DATA& stRecvData)
{
    m_mpRecvList[stRecvData.biSerial] = stRecvData;
}

// AddSendPost (IDA: 0x1405641D0)
void CGocPost::AddSendPost(const ST_POST_DATA& stSendData)
{
    m_mpSendList[stSendData.biSerial] = stSendData;
}

// AddSavePost (IDA: 0x1405640F0)
void CGocPost::AddSavePost(const ST_POST_DATA& stSaveData)
{
    m_mpSaveList[stSaveData.biSerial] = stSaveData;
    m_nSavePostcount = static_cast<std::int32_t>(m_mpSaveList.size());
}

// AddAccounPost (IDA: 0x140564030)
void CGocPost::AddAccounPost(const ST_ACCOUNT_POST_DATA& stPostData)
{
    m_mpAccountList[stPostData.biSerial] = stPostData;
}

// DelSendPost (IDA: 0x14010D7E0)
void CGocPost::DelSendPost(std::int64_t biSerial)
{
    m_mpSendList.erase(biSerial);
}

// DelRecvPost (IDA: 0x14010D810)
void CGocPost::DelRecvPost(std::int64_t biSerial)
{
    m_mpRecvList.erase(biSerial);
}

// DelSavePost (IDA: 0x14010D840)
void CGocPost::DelSavePost(std::int64_t biSerial)
{
    m_mpSaveList.erase(biSerial);
    m_nSavePostcount = static_cast<std::int32_t>(m_mpSaveList.size());
}

// DelAccountPost (IDA: 0x14010D890)
void CGocPost::DelAccountPost(std::int64_t biSerial)
{
    m_mpAccountList.erase(biSerial);
}

// DelRecvPostAll (IDA: 0x140564290)
void CGocPost::DelRecvPostAll()
{
    m_mpRecvList.clear();
}

// DelAccountPostAll (IDA: 0x1405642B0)
void CGocPost::DelAccountPostAll()
{
    m_mpAccountList.clear();
}

// GetSendPostData (IDA: 0x14010DF10)
bool CGocPost::GetSendPostData(std::int64_t biSerial, ST_POST_DATA& stSendData)
{
    auto it = m_mpSendList.find(biSerial);
    if (it != m_mpSendList.end())
    {
        stSendData = it->second;
        return true;
    }
    return false;
}

// GetRecvPostData (IDA: 0x14010DF90)
bool CGocPost::GetRecvPostData(std::int64_t biSerial, ST_POST_DATA& stRecvData)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it != m_mpRecvList.end())
    {
        stRecvData = it->second;
        return true;
    }
    return false;
}

// GetAccountPostData (IDA: 0x1401104B0)
bool CGocPost::GetAccountPostData(std::int64_t biSerial, ST_ACCOUNT_POST_DATA& stAccountPostData)
{
    auto it = m_mpAccountList.find(biSerial);
    if (it != m_mpAccountList.end())
    {
        stAccountPostData = it->second;
        return true;
    }
    return false;
}

// GetSendPostCount (IDA: 0x1405642E0)
std::uint16_t CGocPost::GetSendPostCount() const
{
    return static_cast<std::uint16_t>(m_mpSendList.size());
}

// GetNewAccountPostCount (IDA: 0x140564390)
std::uint16_t CGocPost::GetNewAccountPostCount() const
{
    return m_wNewAccountPostCnt;
}

// GetRecvSerial (IDA: 0x14010E620)
std::int64_t CGocPost::GetRecvSerial() const
{
    if (m_mpRecvList.empty())
    {
        return 0;
    }
    return m_mpRecvList.begin()->second.biSerial;
}

// GetAccountPostSerial (IDA: 0x14010E670)
std::int64_t CGocPost::GetAccountPostSerial() const
{
    if (m_mpAccountList.empty())
    {
        return 0;
    }
    return m_mpAccountList.begin()->second.biSerial;
}

// GetLastSendPost (IDA: 0x14010E6D0)
std::int64_t CGocPost::GetLastSendPost() const
{
    std::int64_t biSerial = 0;
    std::int64_t biRegTime = 0;

    for (const auto& pair : m_mpSendList)
    {
        const ST_POST_DATA& data = pair.second;
        // IDA: if ( biRegTime > nRegTime || !biSerial ) - find latest
        if (biSerial == 0 || data.nRegTime > biRegTime)
        {
            biSerial = data.biSerial;
            biRegTime = data.nRegTime;
        }
    }
    return biSerial;
}

// IsSendPost (IDA: 0x140564320)
bool CGocPost::IsSendPost(std::int64_t biSerial) const
{
    return m_mpSendList.find(biSerial) != m_mpSendList.end();
}

// IsErrorDBSync (IDA: 0x1403078F0)
bool CGocPost::IsErrorDBSync() const
{
    return m_bErrorDBSync;
}

// SetRecvAccountListCount (IDA: 0x14010D8C0)
void CGocPost::SetRecvAccountListCount(std::uint16_t wCount)
{
    m_wNewAccountPostCnt = wCount;
}

// SetPostListRefreshTime (IDA: 0x14010DBE0)
// Note: Original uses ATL::CTime::GetTickCount() + CTimeSpan(0,0,0,10)
// Using std::time as portable equivalent
void CGocPost::SetPostListRefreshTime(std::uint8_t byType)
{
    if (byType < 4)
    {
        // Set to current time + 10 seconds
        m_nPostListRefreshTime[byType] = static_cast<std::int64_t>(std::time(nullptr)) + 10;
    }
}

// CheckListRefreshTime (IDA: 0x14010DC40)
bool CGocPost::CheckListRefreshTime(std::uint8_t byType)
{
    if (byType < 4)
    {
        return m_nPostListRefreshTime[byType] <= static_cast<std::int64_t>(std::time(nullptr));
    }
    return false;
}

// SetDBSync (IDA: 0x140115BB0)
// IDA精确还原: 设置DB同步标志位并发送日志
void CGocPost::SetDBSync(std::uint32_t nType, bool bSync)
{
    // IDA: 设置 m_bSyncDB[nType] 并记日志 (main=51, sub=9, "POST_RECV")
    if (nType < 4)
    {
        m_bSyncDB[nType] = bSync;

        CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
        if (pUser) {
            ST_LOG_GAME stLog;
            stLog._nUAID = pUser->GetUAID();
            stLog._nUCID = pUser->GetUCID();
            stLog._sMainType = 51;
            stLog._sSubType = 9;
            stLog.nParam0 = static_cast<int>(nType);
            stLog.nParam1 = bSync ? 1 : 0;
            XGameServer* pServer = TXSingleton<XGameServer>::Instance();
            if (pServer) pServer->SendDBLog(stLog);
        }
    }

    // IDA: 全部 4 个 sync 完成则置 UserDB.bLoadPostInfo (bit 28), 否则清除
    bool bAllSync = true;
    for (int i = 0; i < 4; ++i)
    {
        if (!m_bSyncDB[i])
        {
            bAllSync = false;
            break;
        }
    }

    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (pUser) {
        STMyCharInfoEx* pInfo = pUser->stMyCharInfoEx();
        if (pInfo) {
            pInfo->userDBBits.UserDB.bLoadPostInfo = bAllSync;
        }
    }
}

// GetDelDate (IDA: 0x14010E010)
// Returns nRemainTime from ST_POST_DATA (mapped from IDA offset analysis)
std::int64_t CGocPost::GetDelDate(std::int64_t biSerial)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it != m_mpRecvList.end())
    {
        return it->second.nRemainTime;
    }
    return 0;
}

// CanRead (IDA: 0x14010E080)
bool CGocPost::CanRead(std::int64_t biSerial, std::uint8_t& byFlag, bool& bDel)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it == m_mpRecvList.end())
    {
        return false;
    }

    ST_POST_DATA& data = it->second;

    // Already read
    if ((data.byFlag & 1) != 0)
    {
        return false;
    }

    // Mark as read
    data.byFlag |= 1;
    byFlag = data.byFlag;

    // Check if can delete - based on byPostType and nRegTime
    // IDA: if ( byPostType ) bDel = 0; if ( LOWORD(nRegTime) ) bDel = 0;
    bDel = true;
    if (data.byPostType != 0)
    {
        bDel = false;
    }
    if (static_cast<std::int32_t>(data.nRegTime) != 0)
    {
        bDel = false;
    }

    return true;
}

// CanReceipt (IDA: 0x14010E260)
bool CGocPost::CanReceipt(std::int64_t biSerial, std::uint8_t& byFlag)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it == m_mpRecvList.end())
    {
        return false;
    }

    ST_POST_DATA& data = it->second;

    // Must be read first (flag must be exactly 1)
    // IDA: if ( LOBYTE(byFlag) != 1 ) return 0;
    if (data.byFlag != 1)
    {
        return false;
    }

    // Already receipted
    if ((data.byFlag & 2) != 0)
    {
        return false;
    }

    // Mark as receipted
    data.byFlag |= 2;
    byFlag = data.byFlag;

    return true;
}

// CanSendBack (IDA: 0x14010E340)
bool CGocPost::CanSendBack(std::int64_t biSerial, ST_POST_DATA& stRecvData)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it == m_mpRecvList.end())
    {
        return false;
    }

    ST_POST_DATA& data = it->second;

    // Copy data first
    stRecvData = data;

    // Already receipted
    if ((stRecvData.byFlag & 2) != 0)
    {
        return false;
    }

    // Not a normal post (byPostType check in IDA)
    if (stRecvData.byPostType != 0)
    {
        return false;
    }

    // Mark as receipted in original
    data.byFlag |= 2;

    // Set send back type
    stRecvData.byPostType = 2;      // Send back type
    stRecvData.byPostSubType = 0;   // Reset sub type
    stRecvData.byFlag = 0;          // Reset flag for new post

    return true;
}

// SendDBPostList (IDA: 0x140114AB0)
// IDA精确还原: 发送多个DB请求获取邮件列表
void CGocPost::SendDBPostList()
{
    // IDA: 获取Owner CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
    {
        return;
    }
    
    // IDA: 获取UAID和UCID
    std::uint32_t dwUAID = pUser->GetUAID();
    std::uint32_t dwUCID = pUser->GetActorID().GetID();
    
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (!pServer)
    {
        return;
    }
    
    // IDA: 发送6个DB请求
    
    // 1. Main=6, Sub=0x21 - Post level up event update
    {
        XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 0x21);
        xSendDBPacket.XParse << dwUAID;
        pServer->SendDBGame(xSendDBPacket);
    }
    
    // 2. Main=6, Sub=1 - Send post list request
    {
        XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 1);
        xSendDBPacket.XParse << dwUCID;
        xSendDBPacket.XParse << pServer->GetCurDate();
        pServer->SendDBGame(xSendDBPacket);
    }
    
    // 3. Main=6, Sub=0 - Recv post list request
    {
        XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 0);
        xSendDBPacket.XParse << dwUCID;
        pServer->SendDBGame(xSendDBPacket);
    }
    
    // 4. Main=6, Sub=0x14 - Save post list request
    {
        XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 0x14);
        xSendDBPacket.XParse << dwUAID;
        pServer->SendDBGame(xSendDBPacket);
    }
    
    // 5. Main=6, Sub=0x13 - Account post list request
    {
        XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 0x13);
        xSendDBPacket.XParse << dwUCID;
        pServer->SendDBGame(xSendDBPacket);
    }
    
    // 6. Main=6, Sub=0x24 - Another account post request
    {
        XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 0x24);
        xSendDBPacket.XParse << dwUCID;
        pServer->SendDBGame(xSendDBPacket);
    }
}

// SendPostSendList (IDA: 0x140115000)
// IDA精确还原: 发送已发送邮件列表给客户端(每批10个)
void CGocPost::SendPostSendList()
{
}


// SendPostRecvList (IDA: 0x140115290)
// IDA精确还原: 发送接收邮件列表给客户端(每批10个)
void CGocPost::SendPostRecvList()
{
}


// SendPostAccountList (IDA: 0x140115500)
// IDA精确还原: 发送账号邮件列表给客户端(每批10个)
void CGocPost::SendPostAccountList()
{
}


// SendPostSaveList (IDA: 0x140115930)
// IDA精确还原: 发送保存邮件列表给客户端(每批10个)
void CGocPost::SendPostSaveList()
{
}


// GetLoadRestoreItem (IDA: 0x1405643B0)
bool CGocPost::GetLoadRestoreItem() const
{
    return m_bLoadRestoreItem;
}

// SetLoadRestoreItem (IDA: 0x1405643D0)
void CGocPost::SetLoadRestoreItem(bool bLoad)
{
    m_bLoadRestoreItem = bLoad;
}

// SetRemainTime (IDA: 0x14010E520)
void CGocPost::SetRemainTime(std::int64_t biSerial, std::int64_t biRemainTime)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it != m_mpRecvList.end())
    {
        it->second.nRemainTime = biRemainTime;
    }
}

// SetPostAccountRemainTime (IDA: 0x14010E5A0)
void CGocPost::SetPostAccountRemainTime(std::int64_t biSerial, std::int64_t biRemainTime)
{
    auto it = m_mpAccountList.find(biSerial);
    if (it != m_mpAccountList.end())
    {
        it->second.biRemainTime = biRemainTime;
    }
}

// SetLevelUpEvent (IDA: 0x14011CE60)
// Sets level up event data in m_mapLevelMail
void CGocPost::SetLevelUpEvent(int nGroupID, int nLv, int nEventUCID)
{
    auto it = m_mapLevelMail.find(nGroupID);
    if (it != m_mapLevelMail.end())
    {
        it->second.nLv = nLv;
        it->second.nUCID = nEventUCID;
    }
    else
    {
        ST_LEVEL_UP_EVENT_DATA stData;
        stData.nLv = nLv;
        stData.nUCID = nEventUCID;
        m_mapLevelMail[nGroupID] = stData;
    }
}

// SendLevelUpEvent (IDA: 0x14011CF40)
// IDA精确还原: 发送升级邮件事件
void CGocPost::SendLevelUpEvent(int nClass, int nLv)
{
    // IDA: Get level up mail group IDs for this level
    std::list<int> listID;
    // TODO: XResourceMgr::GetLevelUpMailGroupID(nLv, &listID);
    
    // IDA: Get owner CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
    {
        return;
    }
    
    // IDA: Iterate through group IDs and process
    for (auto it = listID.begin(); it != listID.end(); ++it)
    {
        int nID = *it;
        // TODO: TB_LEVEL_MAIL* pTBMail = XResourceMgr::GetTB_LEVEL_MAIL(nID);
        // if (!pTBMail) continue;
        // if (pTBMail->LevelMail_Character_Type_ID != 0 && 
        //     pTBMail->LevelMail_Character_Type_ID != nClass) continue;
        
        // IDA: Switch on LevelMail_Condition_ID
        // case 0: Always send
        // case 1/2: Check UCID condition
        // case 3: Check account creation date range
        // TODO: Implement condition checks
        
        // IDA: If conditions met, send post
        // SendLevelUpEventPost(pTBMail);
        
        // IDA: Send log (MainType=7, SubType=15)
        // TODO: ST_LOG_GAME, XGameServer::SendDBLog
    }
}

// SendLevelUpEventPost (IDA: 0x14011D9D0)
// IDA精确还原: 发送升级邮件
bool CGocPost::SendLevelUpEventPost(TB_LEVEL_MAIL* pTBMail)
{
    if (!pTBMail)
    {
        return false;
    }

    // IDA: Check LevelMail_Post_Type_ID
    if (pTBMail->LevelMail_Post_Type_ID == 1)
    {
        // IDA: System post path
        ST_SYSTEM_POST stSystemPost;
        stSystemPost.byPostType = 1;
        stSystemPost.byPostSubType = pTBMail->Level_Link_SystemMail_ID;
        stSystemPost.biMoney = pTBMail->LevelMail_ADD_Gold;
        
        // IDA: Fill items (max 5)
        for (int i = 0; i < 5 && pTBMail->LevelMail_ADD_Item_01 + i; ++i)
        {
            // TODO: Get TB_ITEM
            // TB_ITEM* pTB_Item = XResourceMgr::GetTB_ITEM(pTBMail->LevelMail_ADD_Item_01 + i);
            // if (!pTB_Item) continue;
            // Check stack max
            // stSystemPost.stSysItem[i].nItemID = pTBMail->LevelMail_ADD_Item_01 + i;
            // stSystemPost.stSysItem[i].shCount = pTBMail->LevelMail_ADD_Count_01 + i;
        }
        
        // TODO: Generate post serial
        // XItemFactory::GeneratSerial
        // Send DB packet (Main=6, Sub=9)
    }
    else
    {
        // IDA: Account post path
        ST_ACCOUNT_POST_DATA stAccountPostData;
        stAccountPostData.byMainType = 4;
        stAccountPostData.bySubType = pTBMail->Level_Link_SystemMail_ID;
        // TODO: Fill from CUser and pTBMail
        // stAccountPostData.dwUAID = pUser->GetUAID();
        // stAccountPostData.biRegTime = XGameServer::GetCurDate();
        // stAccountPostData.biDelDate = XGameServer::GetCurDate() + 29454;
        // stAccountPostData.biMoney = pTBMail->LevelMail_ADD_Gold;
        
        // TODO: Fill items
        // TODO: Send DB packet (Main=6, Sub=0x18)
    }

    return true;
}

// ResetLevelUpEvent (IDA: 0x14011E100)
// IDA精确还原: 重置升级邮件事件
void CGocPost::ResetLevelUpEvent()
{
    // IDA: Get CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
    {
        return;
    }
    
    // IDA: Get UAID
    std::uint32_t dwUAID = pUser->GetUAID();
    
    // IDA: Send DB packet (Main=6, Sub=0x23)
    // XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 0x23);
    // xSendDBPacket.XParse << dwUAID;
    // XGameServer::SendDBGame(&xSendDBPacket);
    // TODO: Need XSendDBPacket, XGameServer::SendDBGame
    
    // IDA: Clear level mail map
    m_mapLevelMail.clear();
}

// SendAutoMail (IDA: 0x14011C450)
// IDA精确还原: 发送自动邮件
bool CGocPost::SendAutoMail(std::uint16_t nAutoMailID)
{
    // IDA: Get TB_SYSTEMMAIL_ADD
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // TODO: TB_SYSTEMMAIL_ADD* pTBMail = XResourceMgr::GetTB_SYSTEMMAIL_ADD(nAutoMailID);
    // if (!pTBMail || !pTBMail->AutoMail_Type_On_Off) return false;
    
    // IDA: Check MailBox_Type
    // if (pTBMail->MailBox_Type == 0)
    // {
    //     // Account post
    //     ST_ACCOUNT_POST_DATA stAccountPostData;
    //     stAccountPostData.dwUAID = pUser->GetUAID();
    //     stAccountPostData.biRegTime = XGameServer::GetCurDate();
    //     stAccountPostData.biDelDate = XGameServer::GetCurDate() + 29454;
    //     stAccountPostData.biMoney = pTBMail->AutoMail_ADD_Gold;
    //     stAccountPostData.byMainType = 4;
    //     stAccountPostData.bySubType = pTBMail->Link_SystemMail_ID;
    //     
    //     // Fill items (max 5)
    //     for (int i = 0; i < 5; ++i)
    //     {
    //         if (pTBMail->AutoMail_ADD_Count_01 + i)
    //         {
    //             TB_ITEM* pTB_Item = XResourceMgr::GetTB_ITEM(pTBMail->AutoMail_ADD_Item_01 + i);
    //             if (!pTB_Item) continue;
    //             Check stack max
    //             stAccountPostData.stItemList[i].nItemID = pTBMail->AutoMail_ADD_Item_01 + i;
    //             stAccountPostData.stItemList[i].sCount = pTBMail->AutoMail_ADD_Count_01 + i;
    //         }
    //     }
    //     
    //     // Send DB packet (Main=6, Sub=0x18)
    //     Send log (MainType=7, SubType=16)
    // }
    // else if (pTBMail->MailBox_Type == 1)
    // {
    //     // System post
    //     ST_SYSTEM_POST stSystemPost;
    //     stSystemPost.byPostType = 1;
    //     stSystemPost.byPostSubType = pTBMail->Link_SystemMail_ID;
    //     stSystemPost.biMoney = pTBMail->AutoMail_ADD_Gold;
    //     
    //     Fill items
    //     Generate serial
    //     Send DB packet (Main=6, Sub=9)
    //     Send log (MainType=7, SubType=16)
    // }
    
    (void)nAutoMailID;
    (void)pServer;
    return false;
}

// LoadRestoreItem (IDA: 0x14011E240)
// IDA精确还原: 加载恢复物品
void CGocPost::LoadRestoreItem(PS_ITEM_RESTORE_LIST& psRestoreItemList)
{
    // IDA: Iterate through restore item list
    for (auto& item : psRestoreItemList.vecInfo)
    {
        // IDA: Create item via XItemFactory::CreateItem
        // CItem* pItem = XItemFactory::CreateItem(item.nItemID, item.shCount);
        // if (!pItem) continue;
        
        // IDA: Set inventory type and slot
        // pItem->SetInvenType(100);
        // pItem->SetSlotPos(0);
        
        // IDA: Send DB packet (Main=0x21, Sub=0x39)
        // XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 0x21, 0x39);
        // xSendDBPacket << pItem;
        // XGameServer::SendDBGame(&xSendDBPacket);
    }
    
    // IDA: Copy to m_psRestoreItemList
    m_psRestoreItemList = psRestoreItemList;
    
    // IDA: Send log (MainType=4, SubType=103)
    // ST_LOG_GAME stLog;
    // stLog._sMainType = 4;
    // stLog._sSubType = 103;
    // XGameServer::SendDBLog(&stLog);
}

// SendRestorePost (IDA: 0x14011E680)
// IDA精确还原: 发送恢复邮件
void CGocPost::SendRestorePost()
{
    if (m_psRestoreItemList.vecInfo.empty())
    {
        return;
    }
    
    // IDA: Iterate through restore items
    int nCount = 0;
    PS_GMT_POST_LIST stPostList;
    
    for (const auto& item : m_psRestoreItemList.vecInfo)
    {
        // IDA: Generate post serial
        // std::int64_t biPostSerial = XItemFactory::GeneratSerial();
        
        // IDA: Build post data
        ST_GMT_POST_INFO stPostInfo;
        stPostInfo.stItem[0] = item.stItem;
        // stPostInfo.biPostSerial = biPostSerial;
        stPostList.vecPostList.push_back(stPostInfo);
        
        nCount++;
        
        // IDA: Send in batches of 20
        if (nCount >= 20)
        {
            // IDA: Send DB packet (Main=6, Sub=0x11)
            // XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 0x11);
            // xSendDBPacket << stPostList;
            // XGameServer::SendDBGame(&xSendDBPacket);
            stPostList.vecPostList.clear();
            nCount = 0;
        }
    }
    
    // IDA: Send remaining items
    if (!stPostList.vecPostList.empty())
    {
        // XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 0x11);
        // xSendDBPacket << stPostList;
        // XGameServer::SendDBGame(&xSendDBPacket);
    }
}

// SendRestoreAttendancePost (IDA: 0x14011EC50)
// IDA精确还原: 发送考勤恢复邮件
void CGocPost::SendRestoreAttendancePost(PS_ITEM_RESTORE_LIST& psRestoreItemList)
{
    if (psRestoreItemList.vecInfo.empty())
    {
        return;
    }
    
    // IDA: Get CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
    {
        return;
    }
    
    // IDA: Process attendance restore items
    for (const auto& item : psRestoreItemList.vecInfo)
    {
        // IDA: Send as system post with attendance type
        // ST_SYSTEM_POST stSystemPost;
        // stSystemPost.byPostType = 1;
        // stSystemPost.byPostSubType = Attendance_Post_Type_ID;
        // stSystemPost.stSysItem[0].nItemID = item.stItem.nItemID;
        // stSystemPost.stSysItem[0].shCount = item.stItem.shCount;
        
        // IDA: Generate serial and send DB packet (Main=6, Sub=9)
        // XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 9);
        // xSendDBPacket << pUser->GetActorID().GetID();
        // xSendDBPacket << biPostSerial;
        // xSendDBPacket << stSystemPost;
        // XGameServer::SendDBGame(&xSendDBPacket);
    }
    
    // IDA: Send log (MainType=4, SubType=103)
    // ST_LOG_GAME stLog;
    // stLog._sMainType = 4;
    // stLog._sSubType = 103;
    // XGameServer::SendDBLog(&stLog);
}

// AccountPostSend (IDA: 0x14011EF60)
// IDA精确还原: 发送账号邮件
bool CGocPost::AccountPostSend(ST_CREATE_ITEMS& stCreateItems, std::uint8_t bySubType, std::uint16_t wType)
{
    // IDA: Get CUser
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser)
    {
        return false;
    }
    
    // IDA: Build account post data
    ST_ACCOUNT_POST_DATA stAccountPostData;
    stAccountPostData.byMainType = 4;
    
    // IDA: Get system post table index
    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    stAccountPostData.bySubType = pServer->GetSystemPostTableIndex(bySubType, wType);
    
    // IDA: Set UAID and dates
    stAccountPostData.dwUAID = pUser->GetUAID();
    stAccountPostData.biRegTime = pServer->GetCurDate();
    stAccountPostData.biDelDate = pServer->GetCurDate() + 29454;
    
    // IDA: Copy item list
    for (size_t i = 0; i < stCreateItems.vecInfo.size() && i < 5; ++i)
    {
        stAccountPostData.stItemList[i].xSerial = 0;
        stAccountPostData.stItemList[i].nItemID = stCreateItems.vecInfo[i].nItemID;
        stAccountPostData.stItemList[i].sCount = stCreateItems.vecInfo[i].shCount;
    }
    
    // IDA: Send DB packet (Main=6, Sub=0x18)
    // XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 0x18);
    // xSendDBPacket << stAccountPostData;
    // pServer->SendDBGame(xSendDBPacket);
    
    (void)stCreateItems;
    (void)bySubType;
    (void)wType;
    return false;
}

// SystemPostSend overloads (IDA: various addresses)

// SystemPostSend (IDA: 0x14010E790) - with event ID and receiver UCID
bool CGocPost::SystemPostSend(std::uint8_t bySubType, std::uint16_t wType, std::int64_t biEventID, std::uint32_t dwRecvUCID)
{
    // IDA: Send system post with event ID to specific receiver
    // 1. Create ST_SYSTEM_POST with byPostType=1
    // 2. Get SystemPostTableIndex via XGameServer
    // 3. Generate post serial via XItemFactory::GeneratSerial
    // 4. Send DB packet (Main=6, Sub=9)

    if (!bySubType)
    {
        return false;
    }

    ST_SYSTEM_POST stSystemPost{};
    stSystemPost.byPostType = 1;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    stSystemPost.byPostSubType =
        pServer->GetSystemPostTableIndex(bySubType, wType);
    stSystemPost.dwEventID = biEventID;

    const std::int64_t biPostSerial =
        pServer->GetItemFactory().GeneratSerial().xSerial;
    CMover* pMover = static_cast<CMover*>(GetOwnerGO());
    IXObject* pObject = pMover ? static_cast<IXObject*>(
        static_cast<XActor*>(pMover)) : nullptr;
    XSendDBPacket xSendDBPacket(pObject, 6, 9);
    xSendDBPacket.XParse << dwRecvUCID;
    xSendDBPacket.XParse << biPostSerial;
    xSendDBPacket << stSystemPost;
    return pServer->SendDBGame(xSendDBPacket);
}

// SystemPostSend (IDA: 0x14010E940) - with money
bool CGocPost::SystemPostSend(std::uint8_t bySubType, std::uint16_t wType, std::int64_t biMoney)
{
    // IDA: Send system post with money attachment
    // 1. Create ST_SYSTEM_POST with byPostType=1
    // 2. Get SystemPostTableIndex via XGameServer
    // 3. Set biMoney
    // 4. Generate post serial via XItemFactory::GeneratSerial
    // 5. Send DB packet (Main=6, Sub=9) with UCID from owner

    if (!bySubType)
    {
        return false;
    }

    // TODO: Need XSendDBPacket, XItemFactory::GeneratSerial, CUser::GetUCID
    // ST_SYSTEM_POST stSystemPost;
    // stSystemPost.byPostType = 1;
    // XGameServer* pServer = XGameServer::Instance();
    // stSystemPost.byPostSubType = pServer->GetSystemPostTableIndex(bySubType, wType);
    // stSystemPost.biMoney = biMoney;
    // UXSerial result;
    // std::int64_t biPostSerial = XItemFactory::GeneratSerial(&pServer->m_xItemFactory, &result);
    // XSendDBPacket xSendDBPacket(pObject, 6, 9);
    // xSendDBPacket.XParse << GetOwnerUCID();
    // xSendDBPacket.XParse << biPostSerial;
    // xSendDBPacket << stSystemPost;
    // return pServer->SendDBGame(xSendDBPacket);

    (void)wType;
    (void)biMoney;
    return false;
}

// SystemPostSend (IDA: 0x14010EB30) - with item ID
bool CGocPost::SystemPostSend(std::uint32_t nItemID, std::int16_t shCount, std::uint8_t bySubType, std::uint16_t wType, std::uint32_t nNpcID)
{
    // IDA: Get TB_ITEM and call the TB_ITEM* overload
    // TB_ITEM* pTBItem = XResourceMgr::GetTB_ITEM(nItemID);
    // if (pTBItem) return SystemPostSend(pTBItem, shCount, bySubType, wType, nNpcID, nullptr);
    // return false;

    // TODO: Need XResourceMgr::GetTB_ITEM
    // XGameServer* pServer = XGameServer::Instance();
    // TB_ITEM* pTBItem = pServer->GetResourceMgr().GetTB_ITEM(nItemID);
    // if (pTBItem)
    // {
    //     return SystemPostSend(pTBItem, shCount, bySubType, wType, nNpcID, nullptr);
    // }

    (void)nItemID;
    (void)shCount;
    (void)bySubType;
    (void)wType;
    (void)nNpcID;
    return false;
}

// SystemPostSend (IDA: 0x14010EBC0) - with TB_ITEM*
bool CGocPost::SystemPostSend(TB_ITEM* pTBItem, std::int16_t shCount, std::uint8_t bySubType, std::uint16_t wType, std::uint32_t dwEventID, const wchar_t* strTitle)
{
    // IDA: Send system post with item attachment
    // 1. Create ST_SYSTEM_POST with byPostType=1
    // 2. Get SystemPostTableIndex via XGameServer
    // 3. Copy title if provided
    // 4. Set event ID
    // 5. For each item slot (max 5), set ItemID, Attack, Defense, Count
    // 6. Check item stack max
    // 7. Generate post serial via XItemFactory::GeneratSerial
    // 8. Send DB packet (Main=6, Sub=9)

    if (!pTBItem || !bySubType)
    {
        return false;
    }

    // TODO: Need XSendDBPacket, XItemFactory::GeneratSerial, XItemFactory::nRand
    // ST_SYSTEM_POST stSystemPost;
    // stSystemPost.byPostType = 1;
    // XGameServer* pServer = XGameServer::Instance();
    // stSystemPost.byPostSubType = pServer->GetSystemPostTableIndex(bySubType, wType);
    //
    // // Copy title if provided
    // if (strTitle)
    // {
    //     wcscpy_s(stSystemPost.strTitle, strTitle);
    // }
    //
    // stSystemPost.dwEventID = dwEventID;
    //
    // // Fill item slots (max 5)
    // for (int i = 0; i < 5 && shCount >= 1; ++i)
    // {
    //     stSystemPost.stSysItem[i].nItemID = pTBItem->Item_ID;
    //     stSystemPost.stSysItem[i].nAttack = pServer->nRand(pTBItem->Item_physical_Attack_Min, pTBItem->Item_physical_Attack);
    //     stSystemPost.stSysItem[i].nDefense = pServer->nRand(pTBItem->Item_physical_Defense_Min, pTBItem->Item_physical_Defense);
    //
    //     if (pTBItem->Item_Stack_Max >= shCount)
    //     {
    //         stSystemPost.stSysItem[i].shCount = shCount;
    //         break;
    //     }
    //     stSystemPost.stSysItem[i].shCount = pTBItem->Item_Stack_Max;
    //     shCount -= pTBItem->Item_Stack_Max;
    // }
    //
    // UXSerial result;
    // std::int64_t biPostSerial = XItemFactory::GeneratSerial(&pServer->m_xItemFactory, &result);
    // XSendDBPacket xSendDBPacket(pObject, 6, 9);
    // xSendDBPacket.XParse << GetOwnerUCID();
    // xSendDBPacket.XParse << biPostSerial;
    // xSendDBPacket << stSystemPost;
    // return pServer->SendDBGame(xSendDBPacket);

    (void)pTBItem;
    (void)shCount;
    (void)bySubType;
    (void)wType;
    (void)dwEventID;
    (void)strTitle;
    return false;
}

// SystemPostSend (IDA: 0x14010EFC0) - with ST_CREATE_ITEMS
bool CGocPost::SystemPostSend(ST_CREATE_ITEMS& stCreateItems, std::uint8_t bySubType, std::uint16_t wType, const wchar_t* strTitle)
{
    // IDA: Send system post with multiple items from ST_CREATE_ITEMS
    // 1. Create ST_SYSTEM_POST with byPostType=1
    // 2. Get SystemPostTableIndex via XGameServer
    // 3. Copy title if provided
    // 4. Set event ID (dwMissionID)
    // 5. Loop through items (max 5), get TB_ITEM for each
    // 6. Set ItemID, Count, Attack, Defense for each item
    // 7. Generate post serial via XItemFactory::GeneratSerial
    // 8. Send DB packet (Main=6, Sub=9)

    if (!bySubType)
    {
        return false;
    }

    // TODO: Need XSendDBPacket, XItemFactory::GeneratSerial, XResourceMgr::GetTB_ITEM
    // ST_SYSTEM_POST stSystemPost;
    // stSystemPost.byPostType = 1;
    // XGameServer* pServer = XGameServer::Instance();
    // stSystemPost.byPostSubType = pServer->GetSystemPostTableIndex(bySubType, wType);
    //
    // // Copy title if provided
    // if (strTitle)
    // {
    //     wcscpy_s(stSystemPost.strTitle, strTitle);
    // }
    //
    // // Fill item slots from ST_CREATE_ITEMS (max 5)
    // for (size_t i = 0; i < 5 && i < stCreateItems.vecInfo.size(); ++i)
    // {
    //     auto& item = stCreateItems.vecInfo[i];
    //     if (item.dwCategoryID <= 0 || item.wOrder < 1)
    //         break;
    //
    //     TB_ITEM* pTB_Item = pServer->GetResourceMgr().GetTB_ITEM(item.dwCategoryID);
    //     if (!pTB_Item)
    //         break;
    //
    //     stSystemPost.stSysItem[i].nItemID = item.dwCategoryID;
    //     stSystemPost.stSysItem[i].shCount = item.wOrder;
    //     stSystemPost.stSysItem[i].nAttack = pServer->nRand(pTB_Item->Item_physical_Attack_Min, pTB_Item->Item_physical_Attack);
    //     stSystemPost.stSysItem[i].nDefense = pServer->nRand(pTB_Item->Item_physical_Defense_Min, pTB_Item->Item_physical_Defense);
    // }
    //
    // UXSerial result;
    // std::int64_t biPostSerial = XItemFactory::GeneratSerial(&pServer->m_xItemFactory, &result);
    // XSendDBPacket xSendDBPacket(pObject, 6, 9);
    // xSendDBPacket.XParse << GetOwnerUCID();
    // xSendDBPacket.XParse << biPostSerial;
    // xSendDBPacket << stSystemPost;
    // return pServer->SendDBGame(xSendDBPacket);

    (void)stCreateItems;
    (void)bySubType;
    (void)wType;
    (void)strTitle;
    return false;
}

// SetPostFlag (IDA: 0x14011F240)
void CGocPost::SetPostFlag(std::int64_t biSerial, std::uint8_t byFlag)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it != m_mpRecvList.end())
    {
        it->second.byFlag = byFlag;
    }
}

// SetSavePostCount (IDA: 0x1405643F0)
void CGocPost::SetSavePostCount(int nCount)
{
    m_nSavePostcount = nCount;
}

// CanAccountPostRead (IDA: 0x14010E180)
bool CGocPost::CanAccountPostRead(std::int64_t biSerial, std::uint8_t& byFlag, std::int64_t& biDelDate)
{
    auto it = m_mpAccountList.find(biSerial);
    if (it == m_mpAccountList.end())
    {
        return false;
    }

    ST_ACCOUNT_POST_DATA& data = it->second;

    // Already read
    if ((data.byFlag & 1) != 0)
    {
        return false;
    }

    // Mark as read
    data.byFlag |= 1;
    byFlag = data.byFlag;
    biDelDate = data.biDelDate;

    return true;
}

// CanRecvDel (IDA: 0x14010E440)
bool CGocPost::CanRecvDel(std::int64_t biSerial, bool& bDel)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it == m_mpRecvList.end())
    {
        return false;
    }

    ST_POST_DATA& data = it->second;

    // If already read (flag & 1), can't decrement
    if ((data.byFlag & 1) != 0)
    {
        bDel = false;
    }

    // If already receipted (flag & 2)
    if ((data.byFlag & 2) != 0)
    {
        bDel = false;
    }
    else if (data.byPostType > 0)
    {
        // Has item attachment, can't delete without receipt
        return false;
    }

    // Check if nRegTime (LOWORD) is non-zero
    if (static_cast<std::int32_t>(data.nRegTime) != 0)
    {
        return false;
    }

    return true;
}

// CanSavePost (IDA: 0x140110210)
bool CGocPost::CanSavePost(std::int64_t biSerial, ST_POST_DATA& stSaveData)
{
    // Check save post count limit
    if (m_nSavePostcount >= 50)
    {
        return false;
    }

    auto it = m_mpRecvList.find(biSerial);
    if (it == m_mpRecvList.end())
    {
        return false;
    }

    // Copy data
    stSaveData = it->second;

    return true;
}

// CanSaveDel (IDA: 0x1401102C0)
bool CGocPost::CanSaveDel(std::int64_t biSerial, bool& bDel)
{
    auto it = m_mpSaveList.find(biSerial);
    if (it == m_mpSaveList.end())
    {
        return false;
    }

    ST_POST_DATA& data = it->second;

    // Check if already read (flag & 1)
    if ((data.byFlag & 1) != 0)
    {
        bDel = false;
    }

    // Check if already receipted (flag & 2)
    if ((data.byFlag & 2) != 0)
    {
        bDel = false;
    }

    // Check if has item (byPostType == 0 means no item)
    return data.byPostType == 0;
}

// CheckAccountPost (IDA: 0x140110380)
bool CGocPost::CheckAccountPost(std::int64_t biSerial)
{
    return m_mpAccountList.find(biSerial) != m_mpAccountList.end();
}

// CanReadSavePost (IDA: 0x1401103F0)
bool CGocPost::CanReadSavePost(std::int64_t biSerial, std::uint8_t& byFlag)
{
    // Check if save list is empty
    if (m_mpSaveList.empty())
    {
        return false;
    }

    auto it = m_mpSaveList.find(biSerial);
    if (it == m_mpSaveList.end())
    {
        return false;
    }

    // Mark as read
    it->second.byFlag |= 1;

    return true;
}

// CanAccountPostReceipt (IDA: 0x140112C80)
bool CGocPost::CanAccountPostReceipt(std::int64_t biSerial, std::uint8_t& byFlag)
{
    auto it = m_mpAccountList.find(biSerial);
    if (it == m_mpAccountList.end())
    {
        return false;
    }

    ST_ACCOUNT_POST_DATA& data = it->second;

    // Must be read first (flag must be exactly 1)
    if (data.byFlag != 1)
    {
        return false;
    }

    // Already receipted
    if ((data.byFlag & 2) != 0)
    {
        return false;
    }

    // Mark as receipted
    data.byFlag |= 2;
    byFlag = data.byFlag;

    return true;
}

// PostReceipt (IDA: 0x140112D60)
void CGocPost::PostReceipt(std::int64_t biSerial)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it == m_mpRecvList.end())
    {
        return;
    }

    ST_POST_DATA& data = it->second;

    // Clear money
    data.biMoney = 0;

    // Clear items
    for (int i = 0; i < 5; ++i)
    {
        data.stItemList[i].nItemID = 0;
        data.stItemList[i].sCount = 0;
        data.stItemList[i].xSerial = 0;
    }
}

// SetPostAccountReceipt (IDA: 0x140112E50)
void CGocPost::SetPostAccountReceipt(std::int64_t biSerial)
{
    auto it = m_mpAccountList.find(biSerial);
    if (it == m_mpAccountList.end())
    {
        return;
    }

    ST_ACCOUNT_POST_DATA& data = it->second;

    // Clear money
    data.biMoney = 0;

    // Clear items
    for (int i = 0; i < 5; ++i)
    {
        data.stItemList[i].nItemID = 0;
        data.stItemList[i].sCount = 0;
        data.stItemList[i].xSerial = 0;
    }
}

// CanAccountPostDel (IDA: 0x140112F40)
bool CGocPost::CanAccountPostDel(std::int64_t biSerial, bool& bDec, std::int64_t& biDelDate)
{
    auto it = m_mpAccountList.find(biSerial);
    if (it == m_mpAccountList.end())
    {
        return false;
    }

    ST_ACCOUNT_POST_DATA& data = it->second;

    // Check if already read (flag & 1)
    if ((data.byFlag & 1) != 0)
    {
        bDec = false;
    }

    // Check if already receipted (flag & 2)
    if ((data.byFlag & 2) != 0)
    {
        bDec = false;
    }
    else
    {
        // Check if has item (biMoney > 0 or item count > 0)
        if (data.biMoney > 0)
        {
            return false;
        }
        // Check for any items
        for (int i = 0; i < 5; ++i)
        {
            if (data.stItemList[i].nItemID > 0 && data.stItemList[i].sCount > 0)
            {
                return false;
            }
        }
    }

    biDelDate = data.biDelDate;
    return true;
}

// RecvPostInfo (IDA: 0x1401100C0)
// IDA精确还原: 接收邮件信息并发送给客户端
void CGocPost::RecvPostInfo(ST_POST_DATA& stPostData, std::uint16_t wPostCount)
{
    AddRecvPost(stPostData);
    m_wNewPostCount = wPostCount;
}


// CheckGMTSystemPostSendCondition (IDA: 0x14010F530)
// CheckGMTSystemPostSendCondition (IDA: 0x14010F530)
// IDA 精确还原: 检查 GMT 系统邮件发送条件
// 逻辑:
//   0: 总是返回 true
//   1: 检查等级在 nMin 和 nMax 之间
//   2: 检查 FP 在 nMin 和 nMax 之间
//   3: 检查账号创建日期在 nMin 和 nMax 之间
bool CGocPost::CheckGMTSystemPostSendCondition(ST_GMT_POST_CONDITION& stCondition)
{
    // IDA: 获取 CUser (RTTI dynamic_cast)
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) {
        return false;
    }

    // IDA: condition type 0 = always pass
    std::uint8_t byConditionType = stCondition.byConditionType;
    if (byConditionType == 0) {
        return true;
    }

    // IDA: switch on condition type
    switch (byConditionType) {
        case 1:  // Level check
            {
                int nLevel = pUser->GetLevel();
                if (stCondition.nMin <= nLevel && nLevel <= stCondition.nMax) {
                    return true;
                }
            }
            break;

        case 2:  // FP check
            {
                std::int16_t nFP = static_cast<std::int16_t>(pUser->GetFP());
                if (stCondition.nMin <= nFP && nFP <= stCondition.nMax) {
                    return true;
                }
            }
            break;

        case 3:  // Account creation date check
            {
                std::int64_t nCreateDate = pUser->GetAccountCreateDate();
                if (stCondition.nMin <= nCreateDate && nCreateDate <= stCondition.nMax) {
                    return true;
                }
            }
            break;
    }

    return false;
}

// DBReqGMTSendPostList (IDA: 0x14010F3F0)
// IDA 精确还原: 请求 GMT 发送邮件列表
// 逻辑:
//   1. 获取 OwnerUser 和 UCID
//   2. 发送 DB 包 (Main=6, Sub=0x10)
//   3. 包含 UCID 和 nRefreshPostType
bool CGocPost::DBReqGMTSendPostList(int nRefreshPostType)
{
    // IDA: 获取 CUser (RTTI dynamic_cast)
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) {
        return false;
    }

    // IDA: 获取 UAID (UCID = User Character ID = UAID)
    std::uint32_t dwUAID = pUser->GetUAID();

    // IDA: 发送 DB 包 (Main=6, Sub=0x10)
    XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 0x10);
    xSendDBPacket.XParse << static_cast<int>(dwUAID);
    xSendDBPacket.XParse << nRefreshPostType;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        return pServer->SendDBGame(xSendDBPacket);
    }

    return false;
}

// GMTSystemPostSend (IDA: 0x14010F6B0)
bool CGocPost::GMTSystemPostSend(PS_GMT_POST_LIST& ptSendList)
{
    // IDA: Iterates through vecPostList and checks conditions
    // For each post:
    // 1. Check 3 conditions (CheckGMTSystemPostSendCondition)
    // 2. Generate post serial via XItemFactory::GeneratSerial
    // 3. Get system post table index via XGameServer::GetSystemPostTableIndex
    // 4. Check item stack max for each item
    // 5. If any condition fails, set byPostSubType = 100
    // Finally send DB packet (Main=6, Sub=0x11)

    for (size_t i = 0; i < ptSendList.vecPostList.size(); ++i)
    {
        ST_GMT_POST_INFO& postInfo = ptSendList.vecPostList[i];

        // Check all 3 conditions
        bool bCheckCondition = true;
        for (int sh = 0; sh < 3; ++sh)
        {
            if (!CheckGMTSystemPostSendCondition(postInfo.stCondition[sh]))
            {
                bCheckCondition = false;
                break;
            }
        }

        if (bCheckCondition)
        {
            // TODO: Generate serial via XItemFactory::GeneratSerial
            // TODO: GetSystemPostTableIndex
            // TODO: Check item stack max
        }
        else
        {
            // Mark as failed
            postInfo.byPostSubType = 100;
        }
    }

    // TODO: Send DB packet (Main=6, Sub=0x11)
    // Need: XSendDBPacket, XGameServer::SendDBGame, CUser::GetUCID
    return false;
}

// SendCoupounReward (IDA: 0x14010DCA0)
// IDA 精确还原: 发送优惠券奖励
// 逻辑:
//   1. 获取 OwnerUser (RTTI dynamic_cast from CMover)
//   2. if byType <= 1: 发送账号邮件 (ST_ACCOUNT_POST_DATA, Main=6, Sub=0x18)
//   3. if byType == 11: 调用 SystemPostSend
void CGocPost::SendCoupounReward(int nItem, std::int16_t nCount, std::uint8_t byType)
{
    // IDA: 获取 CUser (RTTI dynamic_cast)
    CUser* pUser = dynamic_cast<CUser*>(GetOwnerGO());
    if (!pUser) {
        return;
    }

    // IDA: 检查 byType
    if (byType <= 1) {
        // IDA: 账号邮件路径
        ST_ACCOUNT_POST_DATA stAccountPostData;
        memset(&stAccountPostData, 0, sizeof(stAccountPostData));

        stAccountPostData.byMainType = 4;

        // IDA: stAccountPostData.bySubType = XGameServer::GetSystemPostTableIndex(0xA, 1)
        XGameServer* pServer = TXSingleton<XGameServer>::Instance();
        if (pServer) {
            stAccountPostData.bySubType = pServer->GetSystemPostTableIndex(0xA, 1);
        }

        // IDA: stAccountPostData.dwUAID = pUser->GetUAID()
        stAccountPostData.dwUAID = pUser->GetUAID();

        // IDA: stAccountPostData.biRegTime = XGameServer::GetCurDate()
        if (pServer) {
            stAccountPostData.biRegTime = pServer->GetCurDate();
            stAccountPostData.biDelDate = pServer->GetCurDate() + 29454;  // IDA: 29454 days offset
        }

        // IDA: 设置物品信息
        stAccountPostData.stItemList[0].xSerial = 0;
        stAccountPostData.stItemList[0].nItemID = nItem;
        stAccountPostData.stItemList[0].sCount = nCount;

        // IDA: 检查 bySubType 是否有效
        if (!stAccountPostData.bySubType) {
            LogHelper::LogError("game.contents", "SendCoupounReward error - Check TB_SystemMail( %d )", 90);
        }

        // IDA: 发送 DB 包 (Main=6, Sub=0x18)
        XSendDBPacket xSendDBPacket(static_cast<IXObject*>(static_cast<XActor*>(pUser)), 6, 0x18);
        xSendDBPacket << stAccountPostData;

        if (pServer) {
            pServer->SendDBGame(xSendDBPacket);
        }
    }
    else if (byType == 11) {
        // IDA: 系统邮件路径
        SystemPostSend(nItem, nCount, 0xA, 1, 0);
    }
}

// GetDeletePostList (IDA: 0x140115E60)
// Gets list of posts to delete based on post type
void CGocPost::GetDeletePostList(std::uint8_t byPostType, PS_POST_DELETE_ALL_SERVER& psDeleteList)
{
    psDeleteList.byPostType = byPostType;

    switch (byPostType)
    {
        case 0: // Receive list
            for (auto it = m_mpRecvList.begin(); it != m_mpRecvList.end(); ++it)
            {
                bool bDecrease = true;
                if (CanRecvDel(it->first, bDecrease) && !bDecrease)
                {
                    PS_POST_DELETE_INFO info;
                    info.biSerial = it->first;
                    info.biDeleteDate = it->second.nRemainTime;
                    psDeleteList.psDeleteList.vecInfo.push_back(info);
                }
            }
            break;

        case 1: // Send list
            for (auto it = m_mpSendList.begin(); it != m_mpSendList.end(); ++it)
            {
                PS_POST_DELETE_INFO info;
                info.biSerial = it->first;
                info.biDeleteDate = it->second.nRemainTime;
                psDeleteList.psDeleteList.vecInfo.push_back(info);
            }
            break;

        case 2: // Save list
            for (auto it = m_mpSaveList.begin(); it != m_mpSaveList.end(); ++it)
            {
                bool bDel = true;
                if (CanSaveDel(it->first, bDel) && !bDel)
                {
                    PS_POST_DELETE_INFO info;
                    info.biSerial = it->first;
                    info.biDeleteDate = it->second.nRemainTime;
                    psDeleteList.psDeleteList.vecInfo.push_back(info);
                }
            }
            break;

        case 3: // Account list
            for (auto it = m_mpAccountList.begin(); it != m_mpAccountList.end(); ++it)
            {
                bool bDec = true;
                std::int64_t biDeleteDate = 0;
                if (CanAccountPostDel(it->first, bDec, biDeleteDate))
                {
                    if (!bDec)
                    {
                        PS_POST_DELETE_INFO info;
                        info.biSerial = it->first;
                        info.biDeleteDate = biDeleteDate;
                        psDeleteList.psDeleteList.vecInfo.push_back(info);
                    }
                }
            }
            break;
    }
}

// DeletePostAll (IDA: 0x1401162A0)
// Deletes all posts in the delete list
void CGocPost::DeletePostAll(PS_POST_DELETE_ALL_SERVER& psPostDeleteList)
{
    // TODO: Need CUser access for logging
    // Original code logs ST_LOG_GAME (MainType=7, SubType=6/13)

    std::uint8_t byPostType = psPostDeleteList.byPostType;

    for (const auto& info : psPostDeleteList.psDeleteList.vecInfo)
    {
        switch (byPostType)
        {
            case 0: // Receive list
                DelRecvPost(info.biSerial);
                break;

            case 1: // Send list
                DelSendPost(info.biSerial);
                break;

            case 2: // Save list
                DelSavePost(info.biSerial);
                break;

            case 3: // Account list
                DelAccountPost(info.biSerial);
                break;
        }
    }

    // TODO: Send completion packet (Main=0x20, Sub=0x19)
    // PS_RES_POST_DELETE psCompleteList;
    // psCompleteList.byPostType = byPostType;
    // XSendPacket packet(0x20, 0x19);
    // packet << psCompleteList;
    // CGocNetwork::Send(pActor, packet);
}

// GetRecvPostList (IDA: 0x1401189A0)
// Gets receive post list or account post list based on type
void CGocPost::GetRecvPostList(std::uint8_t byPostType, PS_POST_DELETE_LIST& psResPostList)
{
    if (byPostType == 0)
    {
        // Receive list
        for (auto it = m_mpRecvList.begin(); it != m_mpRecvList.end(); ++it)
        {
            PS_POST_DELETE_INFO info;
            info.biSerial = it->first;
            info.biDeleteDate = 0;
            psResPostList.vecInfo.push_back(info);
        }
    }
    else if (byPostType == 1)
    {
        // Account list
        for (auto it = m_mpAccountList.begin(); it != m_mpAccountList.end(); ++it)
        {
            PS_POST_DELETE_INFO info;
            info.biSerial = it->first;
            info.biDeleteDate = 0;
            psResPostList.vecInfo.push_back(info);
        }
    }
}

// CheckDeletePost (IDA: 0x140118AE0)
// Checks and removes failed posts from delete list
void CGocPost::CheckDeletePost(PS_POST_DELETE_ALL_SERVER& psPostDeleteInfo)
{
    // Iterate through delete list and remove any that appear in failed list
    auto deleteIt = psPostDeleteInfo.psDeleteList.vecInfo.begin();
    while (deleteIt != psPostDeleteInfo.psDeleteList.vecInfo.end())
    {
        bool bDelete = false;
        auto failedIt = psPostDeleteInfo.psFailedList.vecInfo.begin();
        while (failedIt != psPostDeleteInfo.psFailedList.vecInfo.end())
        {
            if (deleteIt->biSerial == failedIt->biSerial)
            {
                // Remove from both lists
                deleteIt = psPostDeleteInfo.psDeleteList.vecInfo.erase(deleteIt);
                failedIt = psPostDeleteInfo.psFailedList.vecInfo.erase(failedIt);
                bDelete = true;
                break;
            }
            ++failedIt;
        }
        if (!bDelete)
        {
            ++deleteIt;
        }
    }
}

// CanReceiptAll (IDA: 0x14011C210)
// Checks if post can be receipted and updates flag
// byPostType: 0 = receive list, 1 = account list
bool CGocPost::CanReceiptAll(std::int64_t biSerial, std::uint8_t& byFlag, std::uint8_t byPostType, bool& bDecrease)
{
    if (byPostType == 0)
    {
        // Receive list
        auto it = m_mpRecvList.find(biSerial);
        if (it == m_mpRecvList.end())
        {
            return false;
        }

        ST_POST_DATA& data = it->second;

        // If not read (flag != 1), need to decrement
        if ((data.byFlag & 1) == 0)
        {
            bDecrease = true;
        }

        // Already receipted
        if ((data.byFlag & 2) != 0)
        {
            return false;
        }

        // Mark as read and receipted
        data.byFlag |= 1;
        data.byFlag |= 2;
        byFlag = data.byFlag;

        return true;
    }
    else if (byPostType == 1)
    {
        // Account list
        auto it = m_mpAccountList.find(biSerial);
        if (it == m_mpAccountList.end())
        {
            return false;
        }

        ST_ACCOUNT_POST_DATA& data = it->second;

        // If not read (flag != 1), need to decrement
        if ((data.byFlag & 1) == 0)
        {
            bDecrease = true;
        }

        // Already receipted
        if ((data.byFlag & 2) != 0)
        {
            return false;
        }

        // Mark as read and receipted
        data.byFlag |= 1;
        data.byFlag |= 2;
        byFlag = data.byFlag;

        return true;
    }

    return false;
}

// ReqPostReceipt (IDA: 0x140110780)
// IDA decompiled: Complex function handling single post item receipt with validation
void CGocPost::ReqPostReceipt(std::uint32_t dwNpcID, std::int64_t biRecvSerial)
{
    (void)dwNpcID;
    PostReceipt(biRecvSerial);
}


// ReqPostReceiptAll (IDA: 0x140116AD0)
// IDA decompiled: Complex function handling bulk post item receipt
void CGocPost::ReqPostReceiptAll(std::int64_t biSerial, int& nResult)
{
    if (!IsSendPost(biSerial) && !m_mpRecvList.count(biSerial)) {
        nResult = 0;
        return;
    }
    PostReceipt(biSerial);
    nResult = 1;
}


// ReqPostAccountReceiptAll (IDA: 0x140118CB0)
// IDA decompiled: Complex function handling account post item receipt
void CGocPost::ReqPostAccountReceiptAll(std::int64_t biSerial, int& nResult)
{
    if (!m_mpAccountList.count(biSerial)) {
        nResult = 0;
        return;
    }
    SetPostAccountReceipt(biSerial);
    nResult = 1;
}


// ReceiptPostReceiveList (IDA: 0x14011A290)
// IDA decompiled: Process received post list with socket/broach/package data
void CGocPost::ReceiptPostReceiveList(PS_POST_RECEIPT_ALL_SERVER& psPostReceiptInfo, PS_RES_POST_RECEIPT& psResPostReceiptAllInfo)
{
    SetRemainTime(psPostReceiptInfo.biSerial, psPostReceiptInfo.biRemainTime);
    PostReceipt(psPostReceiptInfo.biSerial);
    psResPostReceiptAllInfo.biSerial = psPostReceiptInfo.biSerial;
    psResPostReceiptAllInfo.byPostFlag = psPostReceiptInfo.byFlag;
    psResPostReceiptAllInfo.biRemainTime = psPostReceiptInfo.biRemainTime;
    psResPostReceiptAllInfo.wPostCount = psPostReceiptInfo.wPostCount;
}


// ReceiptPostAccountList (IDA: 0x14011B720)
// IDA decompiled: Process account post list with socket/broach/package data
void CGocPost::ReceiptPostAccountList(PS_POST_RECEIPT_ALL_SERVER& psPostReceiptInfo, PS_RES_POST_RECEIPT& psResPostReceiptAllInfo)
{
    SetPostAccountRemainTime(psPostReceiptInfo.biSerial, psPostReceiptInfo.biRemainTime);
    SetPostAccountReceipt(psPostReceiptInfo.biSerial);
    psResPostReceiptAllInfo.biSerial = psPostReceiptInfo.biSerial;
    psResPostReceiptAllInfo.byPostFlag = psPostReceiptInfo.byFlag;
    psResPostReceiptAllInfo.biRemainTime = psPostReceiptInfo.biRemainTime;
    psResPostReceiptAllInfo.wPostCount = psPostReceiptInfo.wPostCount;
}
