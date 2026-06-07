// CGocPost - Game Object Component for mail/post system
// Restored from GameServer.exe IDA decompilation

#include "GocPost.h"
#include "../../GameServer.h"
#include "../../User.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMail.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
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
// Sends multiple DB requests for post lists
void CGocPost::SendDBPostList()
{
    // TODO: 需要实现与数据库通信的逻辑
    // 原始代码发送以下DB请求:
    // 1. Main=6, Sub=0x21 - Post level up event update (requires UAID)
    // 2. Main=6, Sub=1 - Send post list request (requires UCID + CurDate)
    // 3. Main=6, Sub=0 - Recv post list request (requires UCID)
    // 4. Main=6, Sub=0x14 - Save post list request (requires UAID)
    // 5. Main=6, Sub=0x13 - Account post list request (requires UCID)
    // 6. Main=6, Sub=0x24 - Another account post request (requires UCID)

    // 需要: XGameServer, XSendDBPacket, CUser, GetUAID, GetUCID
    // 需要访问 owner (CUser) 来获取 UAID 和 UCID
}

// SendPostSendList (IDA: 0x140115000)
// Sends the send post list to client in batches of 10
void CGocPost::SendPostSendList()
{
    // TODO: 需要实现发送已发送邮件列表的逻辑
    // IDA 反编译逻辑:
    // 1. 创建 ST_POST_LIST stSendList
    // 2. 遍历 m_mpSendList
    // 3. 每收集10个邮件发送一次 (Main=0x20, Sub=1)
    // 4. 最后发送剩余邮件并标记 bLoad=1 表示结束
    //
    // 需要: XSendPacket, CGocNetwork::Send, CUser/CMover 获取网络组件

    ST_POST_LIST stSendList;
    bool bLoad = false;

    for (auto it = m_mpSendList.begin(); it != m_mpSendList.end(); ++it)
    {
        stSendList.vecData.push_back(it->second);

        if (stSendList.vecData.size() >= 10)
        {
            // TODO: 发送数据包 (Main=0x20, Sub=1)
            // XSendPacket packet(0x20, 1);
            // packet << stSendList;
            // packet << bLoad;
            // CGocNetwork::Send(pActor, packet);
            stSendList.vecData.clear();
        }
    }

    // 发送最后一批
    bLoad = true;
    // TODO: XSendPacket packet(0x20, 1);
    // packet << stSendList;
    // packet << bLoad;
    // CGocNetwork::Send(pActor, packet);
}

// SendPostRecvList (IDA: 0x140115290)
// Sends the receive post list to client in batches of 10
void CGocPost::SendPostRecvList()
{
    // IDA 反编译逻辑:
    // 1. 创建 ST_POST_LIST stRecvList
    // 2. 遍历 m_mpRecvList
    // 3. 每收集10个邮件发送一次 (Main=0x20, Sub=2)
    // 4. 最后发送剩余邮件并标记 bLoad=1 表示结束

    ST_POST_LIST stRecvList;
    bool bLoad = false;

    for (auto it = m_mpRecvList.begin(); it != m_mpRecvList.end(); ++it)
    {
        stRecvList.vecData.push_back(it->second);

        if (stRecvList.vecData.size() >= 10)
        {
            // TODO: 发送数据包 (Main=0x20, Sub=2)
            // XSendPacket packet(0x20, 2);
            // packet << stRecvList;
            // packet << bLoad;
            // CGocNetwork::Send(pActor, packet);
            stRecvList.vecData.clear();
        }
    }

    // 发送最后一批
    bLoad = true;
    // TODO: XSendPacket packet(0x20, 2);
    // packet << stRecvList;
    // packet << bLoad;
    // CGocNetwork::Send(pActor, packet);
}

// SendPostAccountList (IDA: 0x140115500)
// Sends the account post list to client in batches of 10
void CGocPost::SendPostAccountList()
{
    // IDA 反编译逻辑:
    // 1. 创建 PS_ACCOUNT_POST_LIST stAccountList
    // 2. 遍历 m_mpAccountList
    // 3. 每收集10个邮件发送一次 (Main=0x20, Sub=0x14)
    // 4. 最后发送剩余邮件并标记 bLoad=1 表示结束

    PS_ACCOUNT_POST_LIST stAccountList;
    bool bLoad = false;

    for (auto it = m_mpAccountList.begin(); it != m_mpAccountList.end(); ++it)
    {
        stAccountList.vecAccountPostList.push_back(it->second);

        if (stAccountList.vecAccountPostList.size() >= 10)
        {
            // TODO: 发送数据包 (Main=0x20, Sub=0x14)
            // XSendPacket packet(0x20, 0x14);
            // packet << stAccountList;
            // packet << bLoad;
            // CGocNetwork::Send(pActor, packet);
            stAccountList.vecAccountPostList.clear();
        }
    }

    // 发送最后一批
    bLoad = true;
    // TODO: XSendPacket packet(0x20, 0x14);
    // packet << stAccountList;
    // packet << bLoad;
    // CGocNetwork::Send(pActor, packet);
}

// SendPostSaveList (IDA: 0x140115930)
// Sends the save post list to client in batches of 10
void CGocPost::SendPostSaveList()
{
    // IDA 反编译逻辑:
    // 1. 创建 ST_POST_LIST stSaveList
    // 2. 遍历 m_mpSaveList
    // 3. 每收集10个邮件发送一次 (Main=0x20, Sub=0x13)
    // 4. 最后发送剩余邮件并标记 bLoad=1 表示结束

    ST_POST_LIST stSaveList;
    bool bLoad = false;

    for (auto it = m_mpSaveList.begin(); it != m_mpSaveList.end(); ++it)
    {
        stSaveList.vecData.push_back(it->second);

        if (stSaveList.vecData.size() >= 10)
        {
            // TODO: 发送数据包 (Main=0x20, Sub=0x13)
            // XSendPacket packet(0x20, 0x13);
            // packet << stSaveList;
            // packet << bLoad;
            // CGocNetwork::Send(pActor, packet);
            stSaveList.vecData.clear();
        }
    }

    // 发送最后一批
    bLoad = true;
    // TODO: XSendPacket packet(0x20, 0x13);
    // packet << stSaveList;
    // packet << bLoad;
    // CGocNetwork::Send(pActor, packet);
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

    // TODO: Need XSendDBPacket, XItemFactory::GeneratSerial
    // ST_SYSTEM_POST stSystemPost;
    // stSystemPost.byPostType = 1;
    // XGameServer* pServer = XGameServer::Instance();
    // stSystemPost.byPostSubType = pServer->GetSystemPostTableIndex(bySubType, wType);
    // stSystemPost.dwEventID = biEventID;
    // UXSerial result;
    // std::int64_t biPostSerial = XItemFactory::GeneratSerial(&pServer->m_xItemFactory, &result);
    // XSendDBPacket xSendDBPacket(pObject, 6, 9);
    // xSendDBPacket.XParse << dwRecvUCID;
    // xSendDBPacket.XParse << biPostSerial;
    // xSendDBPacket << stSystemPost;
    // return pServer->SendDBGame(xSendDBPacket);

    (void)wType;
    (void)biEventID;
    (void)dwRecvUCID;
    return false;
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
void CGocPost::RecvPostInfo(ST_POST_DATA& stPostData, std::uint16_t wPostCount)
{
    // Add to receive list
    AddRecvPost(stPostData);

    // TODO: Send packet to client (requires XSendPacket and CGocNetwork)
    // Original code sends packet with Main=0x20, Sub=9
    // Also updates daily mission condition
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
    CUser* pUser = GetOwnerUser();
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
    // IDA: 获取 CUser
    CUser* pUser = GetOwnerUser();
    if (!pUser) {
        return false;
    }

    // IDA: 获取 UCID
    int nUCID = pUser->GetUCID();

    // IDA: 发送 DB 包 (Main=6, Sub=0x10)
    XSendDBPacket xSendDBPacket(pUser, 6, 0x10);
    xSendDBPacket.XParse << nUCID;
    xSendDBPacket.XParse << nRefreshPostType;

    XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    if (pServer) {
        return pServer->SendDBGame(&xSendDBPacket);
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
    CUser* pUser = GetOwnerUser();
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
        XSendDBPacket xSendDBPacket(pUser, 6, 0x18);
        xSendDBPacket << stAccountPostData;

        if (pServer) {
            pServer->SendDBGame(&xSendDBPacket);
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
// Request to receipt a single post item
// TODO: 汇编还原 - Complex function with item processing, requires external dependencies
void CGocPost::ReqPostReceipt(std::uint32_t dwNpcID, std::int64_t biRecvSerial)
{
    // IDA: Complex ~50KB function handling single post receipt
    // Key logic flow:
    // 1. Get owner CUser via RTDynamicCast
    // 2. Get UCID and CGocInventory component
    // 3. Get receive post data by biRecvSerial
    // 4. Validate post money (biMoney >= 0)
    // 5. For each item (max 5):
    //    - Get TB_ITEM from XResourceMgr
    //    - Get TB_ITEM_CLASSIFY
    //    - If GroupID == 19 (appearance): handle appearance item
    //    - Else: add to inventory via CGocInventory::AddItem2
    // 6. Check CanReceiptAll
    // 7. Check money overflow via CGocInventory::CheckOverMoney
    // 8. Update items via CGocInventory::UpdateItemEnd and AddItemEnd
    // 9. Add money via CGocInventory::AddMoney
    // 10. Send DB packet (Main=6, Sub=0x20) with PS_POST_RECEIPT_ALL_SERVER
    // 11. Log via ST_LOG_GAME and XGameServer::SendDBLog

    // TODO: Requires: CUser, CGocInventory, XResourceMgr, XGameServer, XSendDBPacket, XSendPacket
    (void)dwNpcID;
    (void)biRecvSerial;
}

// ReqPostReceiptAll (IDA: 0x140116AD0)
// Request to receipt all items from a post
// TODO: 汇编还原 - Complex function with item processing, requires external dependencies
void CGocPost::ReqPostReceiptAll(std::int64_t biSerial, int& nResult)
{
    // IDA: Complex ~30KB function handling bulk post receipt
    // Key logic flow:
    // 1. Get owner CUser via RTDynamicCast
    // 2. Get UCID and CGocInventory component
    // 3. Get receive post data by biSerial
    // 4. Validate post money (biMoney >= 0)
    // 5. Initialize storage info structs (psCreateItem, psUpdateItem, psUpdateSerial)
    // 6. For each item (max 5):
    //    - Get TB_ITEM from XResourceMgr
    //    - Check Item_Stack_Max
    //    - Get TB_ITEM_CLASSIFY
    //    - If GroupID == 19 (appearance):
    //      - Check TB_APPEARANCE
    //      - Check if already has appearance
    //      - Update appearance via CGocInventory::UpdateAppearance
    //      - Log appearance item
    //    - Else:
    //      - Get inventory by Item_Inven_Type
    //      - If has serial: find empty slot and add to psUpdateSerial
    //      - Else: use CGocInventory::AddItem2
    // 7. Check CanReceiptAll
    // 8. Check money overflow via CGocInventory::CheckOverMoney
    // 9. Update items via CGocInventory::UpdateItemEnd and AddItemEnd
    // 10. Add money via CGocInventory::AddMoney
    // 11. Build PS_POST_RECEIPT_ALL_SERVER
    // 12. Send DB packet (Main=6, Sub=0x20) with flag 25
    // 13. Log via ST_LOG_GAME and XGameServer::SendDBLog

    // TODO: Requires: CUser, CGocInventory, XResourceMgr, XGameServer, XSendDBPacket
    nResult = 0;
    (void)biSerial;
}

// ReqPostAccountReceiptAll (IDA: 0x140118CB0)
// Request to receipt all items from an account post
// TODO: 汇编还原 - Complex function with item processing, requires external dependencies
void CGocPost::ReqPostAccountReceiptAll(std::int64_t biSerial, int& nResult)
{
    // IDA: Complex ~20KB function handling account post receipt
    // Key logic flow:
    // 1. Get owner CUser via RTDynamicCast
    // 2. Get UCID and CGocInventory component
    // 3. Get account post data by biSerial
    // 4. Validate post money (biMoney >= 0)
    // 5. Initialize storage info structs
    // 6. For each item (max 5):
    //    - Get TB_ITEM from XResourceMgr
    //    - Get TB_ITEM_CLASSIFY
    //    - Get inventory by Item_Inven_Type
    //    - If has serial: find empty slot and add to psUpdateSerial
    //    - Else: use CGocInventory::AddItem2
    // 7. Check money overflow via CGocInventory::CheckOverMoney
    // 8. Check CanReceiptAll with byPostType=1
    // 9. Update items via CGocInventory::UpdateItemEnd and AddItemEnd
    // 10. Add money via CGocInventory::AddMoney
    // 11. Build PS_POST_RECEIPT_ALL_SERVER with byPostType=1
    // 12. Send DB packet (Main=6, Sub=0x20) with flag 24
    // 13. Log via ST_LOG_GAME and XGameServer::SendDBLog

    // TODO: Requires: CUser, CGocInventory, XResourceMgr, XGameServer, XSendDBPacket
    nResult = 0;
    (void)biSerial;
}

// ReceiptPostReceiveList (IDA: 0x14011A290)
// Process received post list with socket/broach/package data
// TODO: 汇编还原 - Complex function with item processing, requires external dependencies
void CGocPost::ReceiptPostReceiveList(PS_POST_RECEIPT_ALL_SERVER& psPostReceiptInfo, PS_RES_POST_RECEIPT& psResPostReceiptAllInfo)
{
    // IDA: Complex ~70KB function processing post receipt with item details
    // Key logic flow:
    // 1. Get owner CUser via RTDynamicCast
    // 2. Get CGocInventory component
    // 3. Get receive post data by biSerial
    // 4. Process psUpdateItem items:
    //    - Get slot item via CGocInventory::GetSlotItem
    //    - Check lock status (must be 100)
    //    - Set item count via CItem::SetCount
    //    - Log item via PS_LOG_ITEM
    // 5. Process psCreateItem items:
    //    - Add items via CGocInventory::AddItem
    //    - Unlock slots via CGocInventory::SetLock
    // 6. Add appearance items via CGocInventory::AddAppearance
    // 7. Log original post items
    // 8. Process psUpdateSerial items:
    //    - Check random option via CGocInventory::CheckRandomOption
    //    - Add items via CGocInventory::AddItem
    //    - Get item pointer via CGocInventory::GetItemPtr
    //    - Set socket list, broach list, package list on new item
    // 9. Set remain time via SetRemainTime
    // 10. Call PostReceipt
    // 11. Send update packets via CGocInventory::SendUpdateItem, SendCreateItem
    // 12. Get CGocAkashicRecord and add akashic info
    // 13. Build response PS_RES_POST_RECEIPT
    // 14. Send socket/broach/package packets to client

    // TODO: Requires: CUser, CGocInventory, CGocAkashicRecord, XSendPacket, CGocNetwork
    (void)psPostReceiptInfo;
    (void)psResPostReceiptAllInfo;
}

// ReceiptPostAccountList (IDA: 0x14011B720)
// Process account post list with socket/broach/package data
// TODO: 汇编还原 - Complex function with item processing, requires external dependencies
void CGocPost::ReceiptPostAccountList(PS_POST_RECEIPT_ALL_SERVER& psPostReceiptInfo, PS_RES_POST_RECEIPT& psResPostReceiptAllInfo)
{
    // IDA: Complex ~50KB function processing account post receipt with item details
    // Key logic flow:
    // 1. Get owner CUser via RTDynamicCast
    // 2. Get CGocInventory component
    // 3. Get account post data by biSerial
    // 4. Process psUpdateSerial items:
    //    - Check random option via CGocInventory::CheckRandomOption
    //    - Add items via CGocInventory::AddItem
    //    - Get item pointer via CGocInventory::GetItemPtr
    //    - Set socket list, broach list, package list on new item
    //    - Unlock slots via CGocInventory::SetLock
    // 5. Set remain time via SetPostAccountRemainTime
    // 6. Call SetPostAccountReceipt
    // 7. Update account post count via SetRecvAccountListCount
    // 8. Send update packets via CGocInventory::SendUpdateItem, SendCreateItem
    // 9. Build response PS_RES_POST_RECEIPT
    // 10. Send socket/broach/package packets to client

    // TODO: Requires: CUser, CGocInventory, XSendPacket, CGocNetwork
    (void)psPostReceiptInfo;
    (void)psResPostReceiptAllInfo;
}