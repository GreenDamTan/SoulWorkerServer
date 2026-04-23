#include "Soulworker/GameServer/XRelayServer/ObserveSocket.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

// 对齐 IDA: 全局 tick 变量
static std::uint64_t dwObserveConnectTick = 0;
static std::uint64_t dwUpdateObserveInfoTick = 0;
static std::uint32_t g_ObserveInitFlags = 0;  // 一次初始化标志

// ============================================================================
// 构造/析构
// ============================================================================

CObserveSocket::CObserveSocket() {
    // 对齐 IDA 0x14013f780: 初始化成员
    m_bActivate = false;
    m_dwProcessID = 0;
}

// ============================================================================
// 启动
// ============================================================================

bool CObserveSocket::StartUp(XOption* pOption) {
    // 对齐 IDA 0x14013f110: StartUp 初始化观察套接字
    SetMyInfo(pOption);

    // 检查 ObserveAgent 端口是否配置
    if (m_observeInfo.sPort == 0) {
        LogHelper::LogDebug("game.relay", "No Information for Observe Socket");
        return true;  // 对齐 IDA: 端口为0时不连接，但返回true
    }

    // 设置进程 ID
#ifdef _WIN32
    m_dwProcessID = static_cast<std::uint32_t>(GetCurrentProcessId());
#else
    m_dwProcessID = static_cast<std::uint32_t>(getpid());
#endif

    // 对齐 IDA: 初始化 IOCP 客户端
    // IDA: Init_2(ePoolIDNone, &pOverLab.szBuffer[672], 5001u)
    // 简化实现：直接调用基类 Init
    if (!XIOCPClient::Init(ePoolIDNone, nullptr, 5001u)) {
        LogHelper::LogDebug("game.relay", "Error Observe Socket Init fail");
        return false;
    }

    // 连接 ObserveAgent
    if (!XIOCPClient::Connect(m_observeInfo.szPrivateIP, m_observeInfo.sPort)) {
        LogHelper::LogDebug("game.relay", "Failed conect Observe Server!!");
        return false;
    }

    m_bActivate = true;
    return true;
}

// ============================================================================
// 定时更新
// ============================================================================

bool CObserveSocket::OnUpdate(std::uint64_t dw64CurrentTick,
                               const char* szIP,
                               int nPort,
                               int nUserCount,
                               bool bControlConnect,
                               bool bCommunityConnect,
                               int nMaxThreadCount,
                               bool bNetCafe) {
    // 对齐 IDA 0x14013f9b0: 定期向 ObserveAgent 报告服务器状态
    if (!m_bActivate) {
        return false;
    }

    // 一次性初始化 tick
    if ((g_ObserveInitFlags & 1) == 0) {
        g_ObserveInitFlags |= 1u;
#ifdef _WIN32
        dwObserveConnectTick = GetTickCount64();
#else
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        dwObserveConnectTick = static_cast<std::uint64_t>(tv.tv_sec) * 1000 + tv.tv_usec / 1000;
#endif
    }
    if ((g_ObserveInitFlags & 2) == 0) {
        g_ObserveInitFlags |= 2u;
#ifdef _WIN32
        dwUpdateObserveInfoTick = GetTickCount64();
#else
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        dwUpdateObserveInfoTick = static_cast<std::uint64_t>(tv.tv_sec) * 1000 + tv.tv_usec / 1000;
#endif
    }

    // 未连接 → 尝试重连（10秒间隔）
    if (!XIOCPClient::IsConnection()) {
        if (dwObserveConnectTick < dw64CurrentTick) {
            LogHelper::LogDebug("game.relay", "Try to Connection Observe Agent!");
            XIOCPClient::Connect(m_observeInfo.szPrivateIP, m_observeInfo.sPort);
            dwObserveConnectTick = dw64CurrentTick + 10000;
        }
        return false;
    }

    // 已连接 → 每3秒发送状态报告
    if (XIOCPClient::IsConnection() && dwUpdateObserveInfoTick < dw64CurrentTick) {
        SendReportServerStatus(
            static_cast<std::int32_t>(m_myInfo.nType),
            nUserCount,
            bControlConnect,
            bCommunityConnect,
            nMaxThreadCount,
            szIP,
            nPort,
            bNetCafe);
        dwUpdateObserveInfoTick = dw64CurrentTick + 3000;
        return true;
    }

    return false;
}

// ============================================================================
// 发送服务器状态报告
// ============================================================================

void CObserveSocket::SendReportServerStatus(std::int32_t nServerType,
                                             std::int32_t nUserCount,
                                             bool bControlConnect,
                                             bool bCommunityConnect,
                                             std::int32_t nMaxThreadCount,
                                             const char* szIP,
                                             std::int32_t nPort,
                                             bool bNetCafe) {
    // 对齐 IDA 0x14013f840: 构造服务器状态报告并发送给 ObserveAgent
    SS_REPORT_SERVER_STATUS stServerStatus{};
    memset(&stServerStatus.connectInfo, 0, sizeof(stServerStatus.connectInfo));
    stServerStatus.szIP[0] = '\0';
    stServerStatus.nPort = 0;
    stServerStatus.bNetCafe = false;

    // 填充基本信息
    stServerStatus.nServerType = nServerType;
    stServerStatus.dwServerID = m_myInfo.dwID;
    stServerStatus.dwProcessID = m_dwProcessID;
    stServerStatus.nUserCount = nUserCount;

    // 复制 IP 地址
    if (szIP) {
        strcpy_s(stServerStatus.szIP, szIP);
    }

    stServerStatus.nPort = nPort;
    stServerStatus.bNetCafe = bNetCafe;

    // 设置连接状态
    stServerStatus.connectInfo.bControlConnect = bControlConnect;
    stServerStatus.connectInfo.bCommunityConnect = bCommunityConnect;

    // 对齐 IDA: 当服务器类型为2（Relay）时，计算逻辑线程状态
    if (nServerType == 2) {
        CalculateThreadStatus(stServerStatus.connectInfo.szLogicThread, nMaxThreadCount);
    }

    // 构建发送包（主命令0xF2，子命令0x13）
    XSendPacket xSendPacket(0xF2, 0x13);
    xSendPacket << stServerStatus;
    XIOCPClient::Send(xSendPacket);
}

// ============================================================================
// 计算线程状态
// ============================================================================

void CObserveSocket::CalculateThreadStatus(wchar_t* szLogicThread, std::int32_t nMaxThreadCount) {
    // 对齐 IDA 0x14013f4e0: 根据线程 FPS 计算 "0/1/0/1..." 状态字符串
    // '0' = 线程未响应（tick 相同），'1' = 线程活跃（tick 变化）
    if (!szLogicThread || !nMaxThreadCount || nMaxThreadCount >= 20) {
        return;
    }

    std::wstring strState;
    {
        // 加读锁
        CFAutoSlimReadLock autolock(&m_rwThreadTickLock);

        for (int i = 0; i < nMaxThreadCount; ++i) {
            // 比较 Last 和 Now tick 来判断线程是否活跃
            if (m_dwThreadFpsTick_Last[i] != 0 || m_dwThreadFpsTick_Now[i] != 0) {
                if (m_dwThreadFpsTick_Last[i] == m_dwThreadFpsTick_Now[i]) {
                    // tick 未变化，线程可能卡住
                    strState += L'0';
                } else {
                    // tick 变化，线程活跃
                    strState += L'1';
                }
            }

            if (i < nMaxThreadCount - 1) {
                strState += L'/';
            }
        }

        // 将 Now 复制到 Last（为下一次检查准备）
        for (int i = 0; i < 20; ++i) {
            m_dwThreadFpsTick_Last[i] = m_dwThreadFpsTick_Now[i];
        }
    } // 解锁

    if (!strState.empty()) {
        wcscpy_s(szLogicThread, 33, strState.c_str());
    }
}

// ============================================================================
// 连接回调
// ============================================================================

void CObserveSocket::OnConnect() {
    // 对齐 IDA 0x14013f0b0: 连接成功时发送服务器注册信息
    XRelaySocket::SendAddServer();
}

void CObserveSocket::OnDisConnect() {
    // 对齐 IDA 0x14013f0d0: 断开连接（空实现）
}

void CObserveSocket::OnNotConnect() {
    // 对齐 IDA 0x14013f0f0: 连接失败（空实现）
}

// ============================================================================
// 设置服务器信息
// ============================================================================

void CObserveSocket::SetMyInfo(const XOption* pOption) {
    // 对齐 IDA 0x14013f1c0: 设置服务器信息
    XRelaySocket::SetMyInfo(pOption);

    // 对齐 IDA: 获取 ObserveAgent 的 IP 和端口
    if (pOption) {
        std::uint16_t observePort = 0;
        pOption->GetAgentPrivateIPAndPort(1, m_observeInfo.szPrivateIP, observePort);
        m_observeInfo.sPort = static_cast<std::int16_t>(observePort);
    }
}
