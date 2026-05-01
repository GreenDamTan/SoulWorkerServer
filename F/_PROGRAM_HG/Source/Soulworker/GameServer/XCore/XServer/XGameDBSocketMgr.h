// XGameDBSocketMgr.h
// XGameDBSocketMgr DB Agent 连接管理器
// 对齐 ControlServer.exe IDA

#pragma once

#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include <chrono>
#include <cstdint>
#include <thread>

// 前向声明
class CServer;

// CGameDBSocket - DB Agent Socket 类型
// 对齐 IDA 0x1400313F0: 继承自 TXDBSocket<CServer> 的 DB 响应处理类
// 完整实现包含 DBLoginParse, DBEventParse 等响应路由
class CGameDBSocket : public TXDBSocketT<CServer> {
public:
    bool m_bState = false;        // 连接状态
    std::uint8_t m_byType = 0;    // Agent 类型 (0=GAME, 1=AUTH, 2=LOG, 3=STATISTICS, 4=SGLOG, 5=CONTROL)
    bool m_bCloseProcess = false; // 关闭处理标志
    bool m_bSafetyShutdown = false; // 安全关闭标志

    // 对齐 IDA 0x1400313F0: 构造函数设置 m_byType = 5
    CGameDBSocket();

    // 对齐 IDA 0x140031440: 析构函数
    virtual ~CGameDBSocket();

    // 对齐 IDA 0x140031470: FindUser - 查找用户
    CServer* FindUser(int xSessionID) override;

    // 对齐 IDA 0x1400314A0: SetInfomation - 设置名称为 "DBAGENT"
    void SetInfomation();

    // 对齐 IDA 0x140031530: OnParse - 解析包，调用 DBParse
    bool OnParse(XPacket& xPacket) override;

    // 对齐 IDA 0x140031580: DBParse - 路由 MainCmd
    bool DBParse(CServer* pServer, XPacket& xPacket);

    // 对齐 IDA 0x140031620: DBLoginParse - subcmd 路由
    bool DBLoginParse(CServer* pServer, XPacket& xPacket);

    // 对齐 IDA 0x1400316B0: DBEventParse - subcmd 路由
    bool DBEventParse(CServer* pServer, XPacket& xPacket);

    // 对齐 IDA 0x140031720: ResAddServerGroupInfo
    bool ResAddServerGroupInfo(CServer* pServer, XPacket& xPacket);

    // 对齐 IDA 0x1400317A0: ResServerGroupMaxUserCount
    bool ResServerGroupMaxUserCount(CServer* pServer, XPacket& xPacket);

    // 对齐 IDA 0x140031830: ResCheckServerGroupID
    bool ResCheckServerGroupID(CServer* pServer, XPacket& xPacket);

    // 对齐 IDA 0x1400318C0: OnDisConnect - 断开连接处理
    void OnDisConnect() override;

    // 对齐 IDA 0x140031990: OnNotConnect - 连接失败处理
    void OnNotConnect() override;

    // 对齐 IDA 0x140031A30: ResWorldModeLoad
    bool ResWorldModeLoad(CServer* pServer, XPacket& xPacket);

    // 对齐 IDA 0x140031AA0: ResRouletteEventRewardLoad
    bool ResRouletteEventRewardLoad(XPacket& xPacket);

    // 对齐 IDA 0x140031B90: WriteLog
    void WriteLog(char* szLog, ...);

    // 对齐 IDA: AutoConnect 方法
    bool AutoConnect() {
        bool result = Connect();
        LogHelper::LogDebug("game.system", "CGameDBSocket::AutoConnect type=%d result=%d", m_byType, result);
        return result;
    }

protected:
    // 对齐 IDA: OnConnect 处理
    void OnConnect() override {
        LogHelper::LogInfo("game.system", "[DB_SOCKET] Connect ( %d ) !!", m_byType);
    }
};

// 对齐 ControlServer.exe IDA struct (72 bytes):
// +0x00: vtable* (8 bytes) - 虚函数表指针
// +0x08: m_pAccountDBAgent* (8 bytes)
// +0x10: m_pGameDBAgent* (8 bytes)
// +0x18: m_pLogDBAgent* (8 bytes)
// +0x20: m_pStatisticsDBAgent* (8 bytes)
// +0x28: m_pSGLogDBAgent* (8 bytes)
// +0x30: m_nAccountAgentCnt (4 bytes)
// +0x34: m_nGameAgentCnt (4 bytes)
// +0x38: m_nLogAgentCnt (4 bytes)
// +0x3C: m_nStatisticsAgentCnt (4 bytes)
// +0x40: m_nSGLogAgentCnt (4 bytes)
class XGameDBSocketMgr {
public:
    XGameDBSocketMgr();
    ~XGameDBSocketMgr();

    // 对齐 IDA: XGameDBSocketMgr::Init
    void Init();

    // 对齐 IDA: XGameDBSocketMgr::AutoConnect
    void AutoConnect();

    // 对齐 IDA: XGameDBSocketMgr::DisConnect
    void DisConnect();

    // 对齐 IDA 0x140027590: GetAccountDBAgentCount
    int GetAccountDBAgentCount() const { return m_nAccountAgentCnt; }

    // 对齐 IDA 0x140027580: GetGameDBAgentCount
    int GetGameDBAgentCount() const { return m_nGameAgentCnt; }

    // 对齐 IDA 0x140027570: GetLogDBAgentCount
    int GetLogDBAgentCount() const { return m_nLogAgentCnt; }

    // 对齐 IDA 0x140034410: IsAccountDBConnectionAll - 检查所有 AccountDB 连接状态
    bool IsAccountDBConnectionAll() const {
        if (!m_pAccountDBAgent) return false;
        for (int i = 0; i < m_nAccountAgentCnt; ++i) {
            if (!m_pAccountDBAgent[i].IsConnection()) return false;
        }
        return true;
    }

    // 对齐 IDA 0x140034290: SendAccountDBAgent
    bool SendAccountDBAgent(int iIndex, XSendPacket& packet);

    // 对齐 IDA 0x140034310: SendGameDBAgent
    bool SendGameDBAgent(int iIndex, XSendPacket& packet);

    // 对齐 IDA 0x140034390: SendLogDBAgent
    bool SendLogDBAgent(int iIndex, XSendPacket& packet);

private:
    // 对齐 IDA: 初始化指定类型的 Agent 组
    void InitAgentGroup(std::uint8_t byType, CGameDBSocket*& pAgents, int& nAgentCount);

    // 对齐 IDA: 自动连接一组 Agent
    void AutoConnectGroup(CGameDBSocket* pAgents, int nAgentCount);

    // 对齐 IDA: 断开一组 Agent
    void DisconnectGroup(CGameDBSocket* pAgents, int nAgentCount);

private:
    // 对齐 IDA struct 布局 (72 bytes)
    // 注意: vtable 指针在偏移 0，由编译器自动生成
    CGameDBSocket* m_pAccountDBAgent = nullptr;     // +0x08 (8)
    CGameDBSocket* m_pGameDBAgent = nullptr;         // +0x10 (16)
    CGameDBSocket* m_pLogDBAgent = nullptr;          // +0x18 (24)
    CGameDBSocket* m_pStatisticsDBAgent = nullptr;   // +0x20 (32)
    CGameDBSocket* m_pSGLogDBAgent = nullptr;        // +0x28 (40)
    int m_nAccountAgentCnt = 0;                      // +0x30 (48)
    int m_nGameAgentCnt = 0;                         // +0x34 (52)
    int m_nLogAgentCnt = 0;                          // +0x38 (56)
    int m_nStatisticsAgentCnt = 0;                   // +0x3C (60)
    int m_nSGLogAgentCnt = 0;                        // +0x40 (64)
    // +0x44 (68): padding to 72 bytes
};

// TODO: 静态断言暂时禁用 - IDA struct 有 vtable (72 bytes)，当前实现无 vtable (64 bytes)
// static_assert(sizeof(XGameDBSocketMgr) == 72, "XGameDBSocketMgr size mismatch with IDA (72 bytes)");

// ============================================================================
// 内联实现 (对齐 ControlServer.exe IDA)
// ============================================================================

inline XGameDBSocketMgr::XGameDBSocketMgr() {
    m_pAccountDBAgent = nullptr;
    m_pGameDBAgent = nullptr;
    m_pLogDBAgent = nullptr;
    m_pStatisticsDBAgent = nullptr;
    m_pSGLogDBAgent = nullptr;
    m_nAccountAgentCnt = 0;
    m_nGameAgentCnt = 0;
    m_nLogAgentCnt = 0;
    m_nStatisticsAgentCnt = 0;
    m_nSGLogAgentCnt = 0;
}

inline XGameDBSocketMgr::~XGameDBSocketMgr() {
    DisConnect();
}

inline void XGameDBSocketMgr::Init() {
    DisConnect();
    // Agent 类型: 0=GAME, 1=ACCOUNT, 2=LOG, 3=STATISTICS, 4=SGLOG
    InitAgentGroup(0, m_pGameDBAgent, m_nGameAgentCnt);
    InitAgentGroup(1, m_pAccountDBAgent, m_nAccountAgentCnt);
    InitAgentGroup(2, m_pLogDBAgent, m_nLogAgentCnt);
    InitAgentGroup(3, m_pStatisticsDBAgent, m_nStatisticsAgentCnt);
    InitAgentGroup(4, m_pSGLogDBAgent, m_nSGLogAgentCnt);
}

inline void XGameDBSocketMgr::AutoConnect() {
    AutoConnectGroup(m_pAccountDBAgent, m_nAccountAgentCnt);
    AutoConnectGroup(m_pGameDBAgent, m_nGameAgentCnt);
    AutoConnectGroup(m_pLogDBAgent, m_nLogAgentCnt);
    AutoConnectGroup(m_pStatisticsDBAgent, m_nStatisticsAgentCnt);
    AutoConnectGroup(m_pSGLogDBAgent, m_nSGLogAgentCnt);
}

inline void XGameDBSocketMgr::DisConnect() {
    DisconnectGroup(m_pAccountDBAgent, m_nAccountAgentCnt);
    DisconnectGroup(m_pGameDBAgent, m_nGameAgentCnt);
    DisconnectGroup(m_pLogDBAgent, m_nLogAgentCnt);
    DisconnectGroup(m_pStatisticsDBAgent, m_nStatisticsAgentCnt);
    DisconnectGroup(m_pSGLogDBAgent, m_nSGLogAgentCnt);
}

inline bool XGameDBSocketMgr::SendAccountDBAgent(int iIndex, XSendPacket& packet) {
    return m_pAccountDBAgent && iIndex < m_nAccountAgentCnt &&
           m_pAccountDBAgent[iIndex].m_bState && m_pAccountDBAgent[iIndex].Send(packet);
}

inline bool XGameDBSocketMgr::SendGameDBAgent(int iIndex, XSendPacket& packet) {
    if (!m_pGameDBAgent) {
        LogHelper::LogError("game.system", "SendGameDBAgent: m_pGameDBAgent is null");
        return false;
    }
    if (iIndex >= m_nGameAgentCnt) {
        LogHelper::LogError("game.system", "SendGameDBAgent: iIndex=%d >= count=%d", iIndex, m_nGameAgentCnt);
        return false;
    }
    if (!m_pGameDBAgent[iIndex].m_bState) {
        LogHelper::LogError("game.system", "SendGameDBAgent: socket state=false");
        return false;
    }

    // 等待异步连接完成（最多 1000ms）
    if (!m_pGameDBAgent[iIndex].IsConnection()) {
        LogHelper::LogDebug("game.system", "SendGameDBAgent: waiting for connection...");
        constexpr int kMaxWaitMs = 1000;
        int waitedMs = 0;
        while (!m_pGameDBAgent[iIndex].IsConnection() && waitedMs < kMaxWaitMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            waitedMs += 10;
        }
        if (!m_pGameDBAgent[iIndex].IsConnection()) {
            LogHelper::LogError("game.system", "SendGameDBAgent: connection timeout after %dms", waitedMs);
            return false;
        }
        LogHelper::LogDebug("game.system", "SendGameDBAgent: connected after %dms", waitedMs);
    }

    return m_pGameDBAgent[iIndex].Send(packet);
}

inline bool XGameDBSocketMgr::SendLogDBAgent(int iIndex, XSendPacket& packet) {
    return m_pLogDBAgent && iIndex < m_nLogAgentCnt &&
           m_pLogDBAgent[iIndex].m_bState && m_pLogDBAgent[iIndex].Send(packet);
}

inline void XGameDBSocketMgr::InitAgentGroup(std::uint8_t byType, CGameDBSocket*& pAgents, int& nAgentCount) {
    pAgents = nullptr;
    nAgentCount = 0;

    // 对齐 IDA: 从配置文件读取 DBAgent 信息
    // Agent 类型映射: 0=GAME, 1=AUTH(ACCOUNT), 2=LOG, 3=STATISTICS, 4=SGLOG
    char szIP[513] = {};
    char szName[21] = {};
    std::int16_t shPort = 0;
    int nMaxIOPoolCount = 0;

    // TODO: 完整实现需要从 XControlServer::Instance()->GetOption() 获取配置
    // 当前硬编码使用 config.XML 中的 DBAgent 配置
    // ControlServer 配置: <DB TYPE="AUTH" IP="127.0.0.1" PORT="13001"/>

    // 简化配置 - 使用默认 DBAgent 地址
    if (byType <= 2) {  // GAME, AUTH, LOG 都连接到同一个 DBAgent
        strcpy_s(szIP, sizeof(szIP), "127.0.0.1");
        shPort = 13001;
        nAgentCount = 1;
        nMaxIOPoolCount = 10000;

        pAgents = new CGameDBSocket[static_cast<std::size_t>(nAgentCount)];
        for (int index = 0; index < nAgentCount; ++index) {
            // 使用 ePoolIDNone 因为 DBAgent 连接不需要在 IOCP 服务端池中
            // TXDBSocketT::Init 需要 5 个参数: ePoolID, ip, port, name, maxConnectCount
            pAgents[index].Init(ePoolIDNone, szIP, static_cast<std::int16_t>(shPort), "DBAGENT", 1);
            pAgents[index].m_bState = true;  // 标记为可连接状态
            pAgents[index].m_byType = byType;
        }

        LogHelper::LogInfo("game.system",
            "<DBAgent> InitAgentGroup type=%d ip=%s port=%d count=%d",
            byType, szIP, shPort, nAgentCount);
    }
}

inline void XGameDBSocketMgr::AutoConnectGroup(CGameDBSocket* pAgents, int nAgentCount) {
    if (!pAgents || nAgentCount <= 0) {
        return;
    }

    for (int index = 0; index < nAgentCount; ++index) {
        if (pAgents[index].m_bState) {
            pAgents[index].AutoConnect();
        }
    }
}

inline void XGameDBSocketMgr::DisconnectGroup(CGameDBSocket* pAgents, int nAgentCount) {
    if (pAgents) {
        delete[] pAgents;
        pAgents = nullptr;
    }
    nAgentCount = 0;
}

// ============================================================================
// CGameDBSocket 内联实现 (对齐 ControlServer.exe IDA)
// ============================================================================

// 对齐 IDA 0x1400313F0: 构造函数
inline CGameDBSocket::CGameDBSocket() : TXDBSocketT<CServer>() {
    m_byType = 5;  // 对齐 IDA: 默认类型为 5 (CONTROL)
}

// 对齐 IDA 0x140031440: 析构函数
inline CGameDBSocket::~CGameDBSocket() {
    // 基类析构函数会自动调用
}

// 对齐 IDA 0x140031470: FindUser - 从 XControlServer 查找用户
inline CServer* CGameDBSocket::FindUser(int xSessionID) {
    // 对齐 IDA: 调用 XControlServer::FindUser
    extern class XControlServer;
    // TODO: 需要 XControlServer 单例
    return nullptr;
}

// 对齐 IDA 0x1400314A0: SetInfomation - 设置名称为 "DBAGENT"
inline void CGameDBSocket::SetInfomation() {
    sprintf_s(m_szName, "DBAGENT");
    // TODO: 从 XControlServer 获取 IP 和 Port
}

// 对齐 IDA 0x140031530: OnParse - 解析包，调用 DBParse
inline bool CGameDBSocket::OnParse(XPacket& xPacket) {
    int xSessionID = 0;
    xPacket.XParse >> xSessionID;
    return DBParse(nullptr, xPacket);
}

// 对齐 IDA 0x140031580: DBParse - 路由 MainCmd
inline bool CGameDBSocket::DBParse(CServer* pServer, XPacket& xPacket) {
    std::uint8_t byMainCmd = xPacket.GetMainCmd();
    std::uint8_t bySubCmd = xPacket.GetSubCmd();

    // XPRINT 是调试宏，暂时用日志替代
    LogHelper::LogDebug("game.system", "DBParse Main:%02X Sub:%02X", byMainCmd, bySubCmd);

    switch (byMainCmd) {
    case 2:   // MainCmd=2: DBLoginParse
        return DBLoginParse(pServer, xPacket);
    case 73:  // MainCmd=73 (0x49): DBEventParse
        return DBEventParse(pServer, xPacket);
    default:
        return true;
    }
}

// 对齐 IDA 0x140031620: DBLoginParse - subcmd 路由
inline bool CGameDBSocket::DBLoginParse(CServer* pServer, XPacket& xPacket) {
    std::uint8_t bySubCmd = xPacket.GetSubCmd();

    switch (bySubCmd) {
    case 0x21:  // '!' - ResAddServerGroupInfo
        return ResAddServerGroupInfo(pServer, xPacket);
    case 0x22:  // '"' - ResServerGroupMaxUserCount
        return ResServerGroupMaxUserCount(pServer, xPacket);
    case 0x50:  // 'P' - ResCheckServerGroupID
        return ResCheckServerGroupID(pServer, xPacket);
    default:
        return true;
    }
}

// 对齐 IDA 0x1400316B0: DBEventParse - subcmd 路由
inline bool CGameDBSocket::DBEventParse(CServer* pServer, XPacket& xPacket) {
    std::uint8_t bySubCmd = xPacket.GetSubCmd();

    switch (bySubCmd) {
    case 6:
        return ResWorldModeLoad(pServer, xPacket);
    case 45:
        return ResRouletteEventRewardLoad(xPacket);
    default:
        return true;
    }
}

// 对齐 IDA 0x140031720: ResAddServerGroupInfo
inline bool CGameDBSocket::ResAddServerGroupInfo(CServer* pServer, XPacket& xPacket) {
    int nErrorCode = 0;
    xPacket.XParse >> nErrorCode;

    if (nErrorCode <= 0) {
        // 对齐 IDA: 调用 XControlServer::SetRegisterAuth(1)
        LogHelper::LogInfo("game.relay", "<SERVER_GROUP> Register Server Group!");
    } else {
        LogHelper::LogError("game.relay", "<SERVER_GROUP> Failed Register Server Group ( errorcode : %d ) ", nErrorCode);
    }

    return true;
}

// 对齐 IDA 0x1400317A0: ResServerGroupMaxUserCount
inline bool CGameDBSocket::ResServerGroupMaxUserCount(CServer* pServer, XPacket& xPacket) {
    std::uint16_t shStatus = 0;
    int nUserCount = 0;
    int nMaxUserCount = 0;

    xPacket.XParse >> shStatus;
    xPacket.XParse >> nUserCount;
    xPacket.XParse >> nMaxUserCount;

    // 对齐 IDA: 调用 XControlServer::UpdateMaxServerUserCount
    // XControlServer::Instance()->UpdateMaxServerUserCount(nMaxUserCount);

    return true;
}

// 对齐 IDA 0x140031830: ResCheckServerGroupID
inline bool CGameDBSocket::ResCheckServerGroupID(CServer* pServer, XPacket& xPacket) {
    std::uint32_t dwUAID = 0;
    int nServerGroupID = 0;
    int nLastServerGroupID = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> nServerGroupID;
    xPacket.XParse >> nLastServerGroupID;

    // 对齐 IDA: 调用 XControlServer::ResCheckServerGroupID
    // XControlServer::Instance()->ResCheckServerGroupID(dwUAID, nServerGroupID, nLastServerGroupID);

    return true;
}

// 对齐 IDA 0x1400318C0: OnDisConnect - 断开连接处理
inline void CGameDBSocket::OnDisConnect() {
    LogHelper::LogError("game.system", "[DB_SOCKET] OnDisconnect ( %d ) !!", m_byType);

    // 调用基类断开处理
    XIOCPClient::OnDisConnect();

    // 对齐 IDA: 如果类型不是 4，则关闭
    if (m_byType != 4) {
        XIOCPClient::Shutdown(0xFFFFFFFFu);
    }

    // 对齐 IDA: 如果类型是 0 且不是安全关闭，则关闭整个服务
    if (m_byType == 0 && !m_bSafetyShutdown) {
        LogHelper::LogError("game.system", "[DB_SOCKET] OnDisConnect !!");
        Sleep(1000);
// TODO: SET_SERVICE_STATE 是服务管理宏，暂时注释
        // SET_SERVICE_STATE(1u, 3u);
        // XControlServer::Instance()->Shutdown(0xFFFFFFFF);
    }
}

// 对齐 IDA 0x140031990: OnNotConnect - 连接失败处理
inline void CGameDBSocket::OnNotConnect() {
    LogHelper::LogError("game.system", "[DB_SOCKET] OnNotConnect ( %d ) !!", m_byType);

    // 对齐 IDA: 如果没有关闭处理且类型不是 4
    if (!m_bCloseProcess && m_byType != 4) {
        m_bCloseProcess = true;
        Sleep(1000);
// TODO: SET_SERVICE_STATE 是服务管理宏，暂时注释
        // SET_SERVICE_STATE(1u, 3u);
        // XControlServer::Instance()->Shutdown(0xFFFFFFFF);
    }
}

// 对齐 IDA 0x140031A30: ResWorldModeLoad
inline bool CGameDBSocket::ResWorldModeLoad(CServer* pServer, XPacket& xPacket) {
    // TODO: 需要 ST_WORLD_MODE_INFO_VEC 结构体
    // xPacket >> stWorldModeInfoVec;
    // XControlServer::Instance()->GetWorldModeManager().LoadMode(stWorldModeInfoVec);

    return true;
}

// 对齐 IDA 0x140031AA0: ResRouletteEventRewardLoad
inline bool CGameDBSocket::ResRouletteEventRewardLoad(XPacket& xPacket) {
    // TODO: 需要 PS_DB_ROULETTE_REWARD_INFO 结构体
    // xPacket >> stRouletteRewardInfo;
    // XControlServer::Instance()->GetRouletteEventManager().SetRouletteRewardInfo(stRouletteRewardInfo);

    return true;
}

// 对齐 IDA 0x140031B90: WriteLog
inline void CGameDBSocket::WriteLog(char* szLog, ...) {
    char szFormat[512];
    va_list va;
    va_start(va, szLog);
    vsprintf_s(szFormat, sizeof(szFormat), szLog, va);
    va_end(va);
    LogHelper::LogError("game.system", szFormat);
}
