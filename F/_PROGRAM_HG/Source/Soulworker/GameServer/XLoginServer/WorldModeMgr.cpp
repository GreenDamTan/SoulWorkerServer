// WorldModeMgr.cpp
// CWorldModeMgr 世界模式管理器实现
// 对齐 ControlServer.exe IDA (Port 10001)

#include "WorldModeMgr.h"
#include "Soulworker/GameServer/XControlServer/ControlServer.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"

#ifdef _WIN32
#include <intrin.h>
#endif

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_StdCompat.h"

// ============================================================================
// 构造函数/析构函数
// ============================================================================

CWorldModeMgr::CWorldModeMgr()
    : m_bLoadReq(false)
    , m_bModeOn(true)
    , m_bLoadDB(false)
    , m_bSendMode(false)
    , m_dwUpdateTime(0)
{
}

CWorldModeMgr::~CWorldModeMgr()
{
    m_mapWorldMode.clear();
}

// ============================================================================
// Update - 对齐 IDA 0x14004A050
// ============================================================================

void CWorldModeMgr::Update()
{
    XControlServer* pServer = XControlServer::Instance();
    if (!pServer) return;

    // 发送 DB 包请求加载
    if (!m_bLoadReq && m_bModeOn) {
        XOption& option = pServer->GetOption();
        int nGroupID = option.GetGroupID();
        XSendDBPacket dbPacket(0, 0x49, 6);
        dbPacket.XParse << nGroupID;
        m_bLoadReq = pServer->SendDBGame(dbPacket);
    }

    // 检查是否完成缓存加载
    if (m_bLoadDB && pServer->IsCompleteCachingLoad()) {
        // 检查 D6_MODE 选项
        // m_bModeOn = pServer->GetResourceMgr()->GetServerContents(E_SERVER_OPTION_D6_MODE);
    }
}

// ============================================================================
// SendModeSync - 对齐 IDA 0x14004A630
// ============================================================================

void CWorldModeMgr::SendModeSync()
{
    // 对齐 IDA: CWorldModeMgr::SendModeSync
    // 完整实现需要:
    // 1. 获取当前星期
    // 2. 从 XResourceMgr 获取世界模式列表
    // 3. 遍历模式，获取 TB_MODE_DISTRICT6 信息
    // 4. 查找 m_mapWorldMode，获取 CWorldMode 信息
    // 5. 构建响应包 (0xFB, 5)
    // 6. 广播到所有服务器

    if (!m_bLoadDB) return;

    // Stub: 实现需要完整的 CWorldMode 类支持
}

// ============================================================================
// LoadMode - 对齐 IDA 0x1400498A0
// ============================================================================

void CWorldModeMgr::LoadMode(ST_WORLD_MODE_INFO_VEC& stDBInfo)
{
    // 对齐 IDA: CWorldModeMgr::LoadMode
    // 完整实现需要:
    // 1. 设置 m_bLoadDB = true
    // 2. 获取写锁
    // 3. 遍历星期1-7，从 XResourceMgr 获取模式列表
    // 4. 查找匹配 World_ID 的模式
    // 5. 创建 CWorldMode 对象并初始化
    // 6. 插入到 m_mapWorldMode

    m_bLoadDB = true;

    // Stub: 实现需要完整的 CWorldMode 类和 XResourceMgr 支持
}

// ============================================================================
// GetModeList
// ============================================================================

void CWorldModeMgr::GetModeList(ST_WORLD_MODE_INFO_VEC& stModeInfoVec)
{
    // Stub: 返回空列表
    stModeInfoVec.vecInfo.clear();
}

// ============================================================================
// SetModeStartTime/SetModeFinishTime
// ============================================================================

void CWorldModeMgr::SetModeStartTime(int nModeID, std::vector<int>& vecParam, __int64 biTime)
{
    // Stub: 对齐 IDA 0x14004A9A0
}

void CWorldModeMgr::SetModeFinishTime(int nModeID, std::vector<int>& vecParam, __int64 biTime)
{
    // Stub: 对齐 IDA 0x14004AAB0
}

// ============================================================================
// ReqModeComplete
// ============================================================================

bool CWorldModeMgr::ReqModeComplete(PS_WORLD_MODE_COMPLETE& stComplete, bool bForce)
{
    // Stub: 对齐 IDA 0x14004ABC0
    return false;
}

// ============================================================================
// IsActiveMode
// ============================================================================

bool CWorldModeMgr::IsActiveMode(int nModeID, std::vector<int>& vecParam)
{
    // Stub: 对齐 IDA 0x14004ACC0
    return false;
}

// ============================================================================
// IsTodayModeList
// ============================================================================

bool CWorldModeMgr::IsTodayModeList(int nModeID, std::vector<int>& vecParam)
{
    // Stub: 对齐 IDA 0x14004AFC0
    return false;
}

// ============================================================================
// SendModeList
// ============================================================================

void CWorldModeMgr::SendModeList()
{
    // Stub: 对齐 IDA 0x14004B490
}

// ============================================================================
// UpdateMonsterCount
// ============================================================================

void CWorldModeMgr::UpdateMonsterCount(PS_WORLD_MODE_UPDATE stUpdate)
{
    // Stub: 对齐 IDA 0x14004B730
}

// ============================================================================
// IsEnterMode - 对齐 IDA 0x14004B8A0
// ============================================================================

int CWorldModeMgr::IsEnterMode(ST_ENTER_WORLD_MODE_INFO* stEnterInfo, int* nErrorCode)
{
    // 对齐 IDA: CWorldModeMgr::IsEnterMode
    // 检查是否可以进入指定模式
    if (!stEnterInfo) {
        if (nErrorCode) *nErrorCode = 1;
        return 0;
    }

    // Stub: 实现需要检查 m_mapWorldMode 和 TB_MODE_DISTRICT6
    if (nErrorCode) *nErrorCode = 0;
    return 1;  // 返回1表示可以进入
}

// ============================================================================
// GetActiveMode
// ============================================================================

void CWorldModeMgr::GetActiveMode(int& nModeID, int& nState)
{
    // Stub: 对齐 IDA 0x14004BB40
    nModeID = 0;
    nState = 0;
}

// ============================================================================
// InitMode - 对齐 IDA 0x14004B140
// ============================================================================

void CWorldModeMgr::InitMode(/*ATL::CTime*/)
{
    // Stub: 初始化世界模式
}