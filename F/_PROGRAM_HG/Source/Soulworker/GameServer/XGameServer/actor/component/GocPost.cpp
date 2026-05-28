// CGocPost - Game Object Component for mail/post system
// Restored from GameServer.exe IDA decompilation

#include "GocPost.h"
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
// TODO: 汇编还原 - Complex function with CUser/CMover integration and logging
void CGocPost::SetDBSync(std::uint32_t nType, bool bSync)
{
    if (nType < 4)
    {
        m_bSyncDB[nType] = bSync;
        // TODO: 需要实现日志记录和CUser状态更新
        // 原始代码涉及: ST_LOG_GAME, XGameServer::SendDBLog, CUser::stMyCharInfoEx
    }

    // Check if all syncs are complete
    bool bAllSync = true;
    for (int i = 0; i < 4; ++i)
    {
        if (!m_bSyncDB[i])
        {
            bAllSync = false;
            break;
        }
    }

    // TODO: 更新CUser的UserDB标志位
    // 原始代码: (*v16 |= 0x10u) 或 (*v13 &= ~0x10u)
    (void)bAllSync;
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
// TODO: 汇编还原 - Complex function with XGameServer integration
// This function sends multiple DB requests for post lists
void CGocPost::SendDBPostList()
{
    // TODO: 需要实现与数据库通信的逻辑
    // 原始代码发送以下DB请求:
    // 1. Main=6, Sub=0x21 - Post level up event update
    // 2. Main=6, Sub=1 - Send post list request
    // 3. Main=6, Sub=0 - Recv post list request
    // 4. Main=6, Sub=0x14 - Save post list request
    // 5. Main=6, Sub=0x13 - Account post list request
    // 6. Main=6, Sub=0x24 - Another account post request

    // 需要: XGameServer, XSendDBPacket, CUser, GetUAID
}

// SendPostSendList (IDA: 0x140115000)
// TODO: 汇编还原
void CGocPost::SendPostSendList()
{
    // TODO: 需要实现发送已发送邮件列表的逻辑
}

// SendPostRecvList (IDA: 0x140115290)
// TODO: 汇编还原
void CGocPost::SendPostRecvList()
{
    // TODO: 需要实现发送接收邮件列表的逻辑
}

// SendPostAccountList (IDA: 0x140115500)
// TODO: 汇编还原
void CGocPost::SendPostAccountList()
{
    // TODO: 需要实现发送账号邮件列表的逻辑
}

// SendPostSaveList (IDA: 0x140115930)
// TODO: 汇编还原
void CGocPost::SendPostSaveList()
{
    // TODO: 需要实现发送保存邮件列表的逻辑
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
// TODO: 汇编还原 - Complex function with XGameServer/Table integration
void CGocPost::SendLevelUpEvent(int nClass, int nLv)
{
    // TODO: 需要实现发送升级邮件的逻辑
    // 原始代码流程:
    // 1. 调用 XResourceMgr::GetLevelUpMailGroupID 获取对应等级的邮件组ID列表
    // 2. 遍历列表，获取 TB_LEVEL_MAIL 表数据
    // 3. 检查角色类型匹配 (LevelMail_Character_Type_ID)
    // 4. 根据 LevelMail_Condition_ID 处理不同条件:
    //    - case 0: 直接发送
    //    - case 1/2: 检查 UCID 条件
    //    - case 3: 检查创建日期范围
    // 5. 调用 SendLevelUpEventPost 发送邮件
    // 6. 记录日志 ST_LOG_GAME (MainType=7, SubType=15)

    // 需要: XGameServer, XResourceMgr, TB_LEVEL_MAIL, CUser, ST_LOG_GAME
    (void)nClass;
    (void)nLv;
}

// SendLevelUpEventPost (IDA: 0x14011D9D0)
// TODO: 汇编还原
bool CGocPost::SendLevelUpEventPost(TB_LEVEL_MAIL* pTBMail)
{
    // TODO: 需要实现发送升级邮件的具体逻辑
    // 原始代码流程:
    // 1. 检查 LevelMail_Post_Type_ID:
    //    - == 1: 发送系统邮件 (ST_SYSTEM_POST, Main=6, Sub=9)
    //    - != 1: 发送账号邮件 (ST_ACCOUNT_POST_DATA, Main=6, Sub=0x18)
    // 2. 设置邮件内容: Gold, ItemID, Count
    // 3. 检查物品堆叠上限 (Item_Stack_Max)
    // 4. 生成邮件序列号 (XItemFactory::GeneratSerial)
    // 5. 发送 DB 请求

    (void)pTBMail;
    return false;
}

// ResetLevelUpEvent (IDA: 0x14011E100)
// TODO: 汇编还原
void CGocPost::ResetLevelUpEvent()
{
    // TODO: 需要实现重置升级邮件事件的逻辑
    // 原始代码:
    // 1. 获取 CUser 的 UAID
    // 2. 发送 DB 包 (Main=6, Sub=0x23) 清除服务器端的升级邮件数据
    // 3. 清空 m_mapLevelMail

    m_mapLevelMail.clear();
}

// SendAutoMail (IDA: 0x14011C450)
// TODO: 汇编还原
bool CGocPost::SendAutoMail(std::uint16_t nAutoMailID)
{
    // TODO: 需要实现自动邮件发送逻辑
    // 原始代码流程:
    // 1. 获取 TB_SYSTEMMAIL_ADD 表数据
    // 2. 检查 AutoMail_Type_On_Off
    // 3. 根据 MailBox_Type:
    //    - == 0: 发送账号邮件 (ST_ACCOUNT_POST_DATA)
    //    - == 1: 发送系统邮件 (ST_SYSTEM_POST)
    // 4. 设置 Gold 和物品列表 (AutoMail_ADD_Item_01~05)
    // 5. 检查物品堆叠上限
    // 6. 发送 DB 请求并记录日志

    (void)nAutoMailID;
    return false;
}

// LoadRestoreItem (IDA: 0x14011E240)
// TODO: 汇编还原
void CGocPost::LoadRestoreItem(PS_ITEM_RESTORE_LIST& psRestoreItemList)
{
    // TODO: 需要实现加载恢复物品的逻辑
    // 原始代码流程:
    // 1. 遍历 psRestoreItemList 中的物品
    // 2. 调用 XItemFactory::CreateItem 创建物品
    // 3. 设置 byInvenType=100, shSlotPos=0
    // 4. 发送 DB 包 (Main=0x21, Sub=0x39) 存储物品
    // 5. 复制到 m_psRestoreItemList
    // 6. 记录日志 ST_LOG_GAME (MainType=4, SubType=103)

    (void)psRestoreItemList;
}

// SendRestorePost (IDA: 0x14011E680)
// TODO: 汇编还原
void CGocPost::SendRestorePost()
{
    // TODO: 需要实现发送恢复邮件的逻辑
    // 原始代码流程:
    // 1. 遍历 m_psRestoreItemList 中的物品
    // 2. 生成邮件序列号
    // 3. 构建 PS_GMT_POST_LIST
    // 4. 分批发送 (每批最多20个)
    // 5. 发送 DB 包 (Main=6, Sub=0x11)

}

// SendRestoreAttendancePost (IDA: 0x14011EC50)
// TODO: 汇编还原
void CGocPost::SendRestoreAttendancePost(PS_ITEM_RESTORE_LIST& psRestoreItemList)
{
    // TODO: 需要实现发送考勤恢复邮件的逻辑

    (void)psRestoreItemList;
}

// AccountPostSend (IDA: 0x14011EF60)
// TODO: 汇编还原
bool CGocPost::AccountPostSend(ST_CREATE_ITEMS& stCreateItems, std::uint8_t bySubType, std::uint16_t wType)
{
    // TODO: 需要实现账号邮件发送逻辑
    // 原始代码流程:
    // 1. 获取 CUser 和 UAID
    // 2. 构建ST_ACCOUNT_POST_DATA:
    //    - byMainType = 4
    //    - bySubType = XGameServer::GetSystemPostTableIndex(bySubType, wType)
    //    - dwUAID, biRegTime, biDelDate
    // 3. 复制物品列表
    // 4. 发送 DB 包 (Main=6, Sub=0x18)

    (void)stCreateItems;
    (void)bySubType;
    (void)wType;
    return false;
}

// SystemPostSend overloads (IDA: various addresses)

// SystemPostSend (IDA: 0x14010E790) - with event ID
bool CGocPost::SystemPostSend(std::uint8_t bySubType, std::uint16_t wType, std::int64_t biEventID, std::uint32_t dwRecvUCID)
{
    // TODO: 汇编还原
    // 原始代码:
    // 1. 构建ST_SYSTEM_POST: byPostType=1, byPostSubType
    // 2. 生成邮件序列号
    // 3. 发送 DB 包 (Main=6, Sub=9)

    (void)bySubType;
    (void)wType;
    (void)biEventID;
    (void)dwRecvUCID;
    return false;
}

// SystemPostSend (IDA: 0x14010E940) - with money
bool CGocPost::SystemPostSend(std::uint8_t bySubType, std::uint16_t wType, std::int64_t biMoney)
{
    // TODO: 汇编还原

    (void)bySubType;
    (void)wType;
    (void)biMoney;
    return false;
}

// SystemPostSend (IDA: 0x14010EB30) - with item ID
bool CGocPost::SystemPostSend(std::uint32_t nItemID, std::int16_t shCount, std::uint8_t bySubType, std::uint16_t wType, std::uint32_t nNpcID)
{
    // TODO: 汇编还原
    // 原始代码: 获取 TB_ITEM，然后调用带 TB_ITEM* 参数的重载

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
    // TODO: 汇编还原
    // 原始代码流程:
    // 1. 构建ST_SYSTEM_POST: byPostType=1, byPostSubType
    // 2. 复制标题 (如果有)
    // 3. 设置物品信息: ItemID, Attack, Defense, Count
    // 4. 检查堆叠上限
    // 5. 生成邮件序列号
    // 6. 发送 DB 包 (Main=6, Sub=9)

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
    // TODO: 汇编还原

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

    // Check if already receipted (flag & 2)
    if ((data.byFlag & 2) == 0)
    {
        // Not receipted, check if can delete
        bDel = (data.byPostType != 0);
        return true;
    }

    // Already receipted
    bDel = true;
    return true;
}

// CanSavePost (IDA: 0x140110210)
bool CGocPost::CanSavePost(std::int64_t biSerial, ST_POST_DATA& stSaveData)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it == m_mpRecvList.end())
    {
        return false;
    }

    ST_POST_DATA& data = it->second;

    // Already receipted
    if ((data.byFlag & 2) != 0)
    {
        return false;
    }

    stSaveData = data;

    // Mark as receipted and move to save list
    data.byFlag |= 2;
    stSaveData.byFlag = data.byFlag;

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

    // Check if already receipted
    if ((data.byFlag & 2) == 0)
    {
        bDel = false;
        return true;
    }

    bDel = true;
    return true;
}

// CheckAccountPost (IDA: 0x140110380)
bool CGocPost::CheckAccountPost(std::int64_t biSerial)
{
    return m_mpAccountList.find(biSerial) != m_mpAccountList.end();
}

// CanReadSavePost (IDA: 0x1401103F0)
bool CGocPost::CanReadSavePost(std::int64_t biSerial, std::uint8_t& byFlag)
{
    auto it = m_mpSaveList.find(biSerial);
    if (it == m_mpSaveList.end())
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

    return true;
}
