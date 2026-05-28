// CGocPost - Game Object Component for mail/post system
// Restored from GameServer.exe IDA decompilation
// Address range: 0x140030000 - 0x140030500

#include "GocPost.h"
#include <cstring>
#include <ctime>

// Static family ID (IDA: 0x140030260)
int CGocPost::GetFamilyID()
{
    return 11;
}

CGocPost::CGocPost()
    : GOComponent()
    , m_psRestoreItemList(nullptr)
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
}

CGocPost::~CGocPost()
{
    m_mpSendList.clear();
    m_mpRecvList.clear();
    m_mpSaveList.clear();
    m_mpAccountList.clear();
    m_mapLevelMail.clear();
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

void CGocPost::Init()
{
    m_mpSendList.clear();
    m_mpRecvList.clear();
    m_mpSaveList.clear();
    m_mpAccountList.clear();
    m_mapLevelMail.clear();

    m_bSendListCheck = false;
    m_bRecvListCheck = false;
    m_bSaveListCheck = false;
    m_bAccountListCheck = false;
    m_bLoadRestoreItem = false;
    m_bErrorDBSync = false;

    m_wNewPostCount = 0;
    m_wNewAccountPostCnt = 0;
    m_nSavePostcount = 0;

    std::memset(m_bSyncDB, 0, sizeof(m_bSyncDB));
    std::memset(m_nPostListRefreshTime, 0, sizeof(m_nPostListRefreshTime));
}

// Post list management
void CGocPost::AddRecvPost(const ST_POST_DATA& stRecvData)
{
    m_mpRecvList[stRecvData.biSerial] = stRecvData;
}

void CGocPost::AddSendPost(const ST_POST_DATA& stSendData)
{
    m_mpSendList[stSendData.biSerial] = stSendData;
}

void CGocPost::AddSavePost(const ST_POST_DATA& stSaveData)
{
    m_mpSaveList[stSaveData.biSerial] = stSaveData;
    m_nSavePostcount = static_cast<std::int32_t>(m_mpSaveList.size());
}

void CGocPost::AddAccounPost(const ST_ACCOUNT_POST_DATA& stPostData)
{
    m_mpAccountList[stPostData.biSerial] = stPostData;
}

void CGocPost::DelSendPost(std::int64_t biSerial)
{
    m_mpSendList.erase(biSerial);
}

void CGocPost::DelRecvPost(std::int64_t biSerial)
{
    m_mpRecvList.erase(biSerial);
}

void CGocPost::DelSavePost(std::int64_t biSerial)
{
    m_mpSaveList.erase(biSerial);
    m_nSavePostcount = static_cast<std::int32_t>(m_mpSaveList.size());
}

void CGocPost::DelAccountPost(std::int64_t biSerial)
{
    m_mpAccountList.erase(biSerial);
}

void CGocPost::DelRecvPostAll()
{
    m_mpRecvList.clear();
}

void CGocPost::DelAccountPostAll()
{
    m_mpAccountList.clear();
}

// Post data retrieval
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

// Post counts
std::uint16_t CGocPost::GetSendPostCount() const
{
    return static_cast<std::uint16_t>(m_mpSendList.size());
}

std::uint16_t CGocPost::GetNewAccountPostCount() const
{
    return m_wNewAccountPostCnt;
}

std::int64_t CGocPost::GetRecvSerial() const
{
    if (!m_mpRecvList.empty())
    {
        return m_mpRecvList.begin()->second.biSerial;
    }
    return 0;
}

std::int64_t CGocPost::GetAccountPostSerial() const
{
    if (!m_mpAccountList.empty())
    {
        return m_mpAccountList.begin()->second.biSerial;
    }
    return 0;
}

std::int64_t CGocPost::GetLastSendPost() const
{
    std::int64_t biSerial = 0;
    std::int64_t biRegTime = 0;

    for (const auto& pair : m_mpSendList)
    {
        const ST_POST_DATA& data = pair.second;
        if (biSerial == 0 || data.biSendDate > biRegTime)
        {
            biSerial = data.biSerial;
            biRegTime = data.biSendDate;
        }
    }
    return biSerial;
}

// Post checks
bool CGocPost::IsSendPost(std::int64_t biSerial) const
{
    return m_mpSendList.find(biSerial) != m_mpSendList.end();
}

bool CGocPost::IsErrorDBSync() const
{
    return m_bErrorDBSync;
}

// Post flags and state
void CGocPost::SetRecvAccountListCount(std::uint16_t wCount)
{
    m_wNewAccountPostCnt = wCount;
}

void CGocPost::SetPostListRefreshTime(std::uint8_t byType)
{
    if (byType < 4)
    {
        // Set to current time + 10 seconds
        m_nPostListRefreshTime[byType] = std::time(nullptr) + 10;
    }
}

bool CGocPost::CheckListRefreshTime(std::uint8_t byType)
{
    if (byType < 4)
    {
        return m_nPostListRefreshTime[byType] <= std::time(nullptr);
    }
    return false;
}

void CGocPost::SetDBSync(std::uint32_t nType, bool bSync)
{
    if (nType < 4)
    {
        m_bSyncDB[nType] = bSync;
    }
}

// Post operations
std::int64_t CGocPost::GetDelDate(std::int64_t biSerial)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it != m_mpRecvList.end())
    {
        return it->second.biDelDate;
    }
    return 0;
}

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

    // Check if can delete
    bDel = (data.byType == 0);

    return true;
}

bool CGocPost::CanReceipt(std::int64_t biSerial, std::uint8_t& byFlag)
{
    auto it = m_mpRecvList.find(biSerial);
    if (it == m_mpRecvList.end())
    {
        return false;
    }

    ST_POST_DATA& data = it->second;

    // Must be read first
    if ((data.byFlag & 1) != 1)
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

bool CGocPost::CanSendBack(std::int64_t biSerial, ST_POST_DATA& stRecvData)
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

    // Not a normal post
    if (data.byType != 0)
    {
        return false;
    }

    stRecvData = data;
    data.byFlag |= 2;
    stRecvData.byType = 2;  // Send back type
    stRecvData.byFlag = 0;

    return true;
}

// Database operations
void CGocPost::SendDBPostList()
{
    // TODO: Implement with XGameServer integration
}

// Restore item
bool CGocPost::GetLoadRestoreItem() const
{
    return m_bLoadRestoreItem;
}

void CGocPost::SetLoadRestoreItem(bool bLoad)
{
    m_bLoadRestoreItem = bLoad;
}

// Level up event mail
void CGocPost::SendLevelUpEvent(int nClass, int nLevel)
{
    (void)nClass;
    (void)nLevel;
}
