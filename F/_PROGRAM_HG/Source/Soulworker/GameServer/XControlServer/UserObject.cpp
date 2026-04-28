// UserObject.cpp
// CUserObject 用户对象类实现

#include "UserObject.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

// ============================================================================
// 构造函数
// ============================================================================

CUserObject::CUserObject(CServer* pServer, STCharInfo& stInfo, UXMapID uxMapID)
    : m_dwConnectTick(0)
    , m_pServer(pServer)
    , m_dwIP(0)
    , m_stCharInfo(stInfo)
    , m_uxMapID(uxMapID)
    , m_dwServerID(0)
    , m_byTradePasswordState(0)
    , m_biAuthSessionID(0)
    , m_byBlockType(0)
    , m_dw64GroupIDTick(0)
    , m_nWorldState(0)
    , m_nBeforeServerID(0)
    , m_dwWorldChangeWaitTick(0)
    , m_byAuthType(0)
{
    memset(&m_stPartyInfo, 0, sizeof(m_stPartyInfo));
    m_uxTransMapID.nMapID = 0;
}

// ============================================================================
// 析构函数
// ============================================================================

CUserObject::~CUserObject()
{
    // 清理逻辑
}

// ============================================================================
// GetName - 从 STCharInfo 获取名称
// ============================================================================

const std::wstring& CUserObject::GetName() const
{
    // 对齐 IDA: 名称存储在 m_stCharInfo.stBaseInfo.strName (wchar_t[21])
    // 返回缓存的名称
    static thread_local std::wstring s_cachedName;
    s_cachedName = FixedWideArrayToWString(m_stCharInfo.stBaseInfo.strName);
    return s_cachedName;
}

// ============================================================================
// Setter 方法实现
// ============================================================================

void CUserObject::SetWorldState(int nState, ULONGLONG dwWaitTick, UXMapID uxMapID)
{
    m_nWorldState = nState;
    m_dwWorldChangeWaitTick = dwWaitTick;
    m_uxTransMapID = uxMapID;
}

void CUserObject::SetPartyInfo(ST_PARTY_INFO& stPartyInfo)
{
    m_stPartyInfo = stPartyInfo;
}

void CUserObject::SetBeforeServerID(int nServerID)
{
    m_nBeforeServerID = nServerID;
}

void CUserObject::SetServer(CServer* pServer)
{
    m_pServer = pServer;
    // 对齐 IDA: 从 pServer 获取 ServerID
    if (pServer) {
        m_dwServerID = pServer->GetServerID();
    }
}

void CUserObject::SetMapIns(UXMapID uxMapID)
{
    m_uxMapID = uxMapID;
}

void CUserObject::SetAuthType(BYTE byAuthType)
{
    m_byAuthType = byAuthType;
}

void CUserObject::SetGroupIDTick(ULONGLONG dwTick)
{
    m_dw64GroupIDTick = dwTick;
}

void CUserObject::SetBlockType(BYTE byBlockType)
{
    m_byBlockType = byBlockType;
}

void CUserObject::SetAuthSessionID(__int64 biSessionID)
{
    m_biAuthSessionID = biSessionID;
}

void CUserObject::SetTradePWState(BYTE byState)
{
    m_byTradePasswordState = byState;
}

void CUserObject::SetIP(DWORD dwIP)
{
    m_dwIP = dwIP;
}

void CUserObject::SetName(const wchar_t* szName)
{
    // 对齐 IDA: 名称存储在 m_stCharInfo.stBaseInfo.strName (wchar_t[21])
    if (szName) {
        wcsncpy_s(m_stCharInfo.stBaseInfo.strName, szName, _TRUNCATE);
    }
}

void CUserObject::SetEnterWorldModeInfo(ST_WORLD_MODE stMode)
{
    // 对齐 IDA 0x140044C60 (CUserObject::SetEnterWorldModeInfo)
    // 查找是否已存在相同 nModeDateID 的记录
    for (auto& info : m_stEnterWorldModeInfo.vecInfo) {
        if (info.biEnterDate == stMode.nModeDateID) {
            // 更新现有记录
            info.nModeID = stMode.nModeID;
            return;
        }
    }
    // 不存在则添加新记录
    m_stEnterWorldModeInfo.vecInfo.push_back(stMode);
}

// ============================================================================
// Getter 方法实现
// ============================================================================

void CUserObject::GetEnterWorldModeInfo(ST_ENTER_WORLD_MODE_INFO& stInfo)
{
    stInfo = m_stEnterWorldModeInfo;
}

// ============================================================================
// 功能方法实现
// ============================================================================

void CUserObject::SendPacket(XSendPacket& packet)
{
    if (m_pServer)
    {
        m_pServer->SendEx(packet);
    }
}

bool CUserObject::CanChangeWorld()
{
    // 检查世界状态是否允许切换
    // m_nWorldState == 0 表示可以切换
    return m_nWorldState == 0;
}

void CUserObject::ResetWorldState(UXMapID uxMapID)
{
    m_nWorldState = 0;
    m_dwWorldChangeWaitTick = 0;
    m_uxMapID = uxMapID;
    m_uxTransMapID.nMapID = 0;
}

void CUserObject::LoadEnterWorldModeInfo(ST_ENTER_WORLD_MODE_INFO stInfo)
{
    m_stEnterWorldModeInfo = stInfo;
}
