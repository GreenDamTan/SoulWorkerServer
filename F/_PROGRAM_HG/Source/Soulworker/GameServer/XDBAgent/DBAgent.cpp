#include "Soulworker/GameServer/XDBAgent/DBAgent.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"

#include <cstdio>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

// CLogThreadManager forward declaration
class CLogThreadManager;

// TODO: 推测结果 - CLogThreadManager 需要 TXSingleton 包装
namespace {
    // Placeholder for CLogThreadManager::Start
    void GreenDamTan_LogThreadManagerStart(const char* szName) {
        // TODO: 需人工审查 - CLogThreadManager 实现需从其他模块导入
    }
    void GreenDamTan_LogThreadManagerEnd() {
        // TODO: 需人工审查 - CLogThreadManager::End 实现
    }
}

// Static variables for PrintFPS
static std::uint64_t m_dw64FPSTick = 0;
static std::uint32_t m_dwFrame = 0;
static int nCount = 0;

XDBAgent::XDBAgent()
    : TXMultiPoolServer<XServerCon>()
{
    // Per IDA: all XDBAgentDBManager members are default constructed
    // m_xLogDB and m_scObserveSocket are also default constructed
}

XDBAgent::~XDBAgent() {
    // TODO: 需人工审查 - destructor cleanup
}

void XDBAgent::SetName() {
    // Per IDA: sprintf_s<21>(m_szName, "DBAGENT")
    std::snprintf(m_szName, sizeof(m_szName), "DBAGENT");
}

bool XDBAgent::InitServer() {
    const char* szName = GetName();

    // Start log thread manager
    GreenDamTan_LogThreadManagerStart(szName);

    // Get thread count from config
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    std::int16_t nThreadCount = pAgent->GetOption().GetLogicThread();
    if (nThreadCount == 0) {
        nThreadCount = 8;
    }

    // Initialize AccountDB (DNS type 0)
    unsigned char* dnsAccount = reinterpret_cast<unsigned char*>(const_cast<char*>(GetOption().GetDNS(0)));
    if (!m_xAccountDBMgr.Init(dnsAccount, 0, nThreadCount)) {
        LogHelper::LogError("game.system", "[SYSTEM] Account DB init False ");
        return false;
    }
    LogHelper::LogInfo("game.system", "[INIT] m_xAccountDBMgr - Init");

    // Initialize GameDB (DNS type 1)
    unsigned char* dnsGame = reinterpret_cast<unsigned char*>(const_cast<char*>(GetOption().GetDNS(1)));
    if (!m_xGameDBMgr.Init(dnsGame, 1, nThreadCount)) {
        LogHelper::LogError("game.system", "[SYSTEM] Game DB init False ");
        return false;
    }
    LogHelper::LogInfo("game.system", "[INIT] m_xGameDBMgr - Init");

    // Initialize LogDB (DNS type 3) - only if IsWriteLog
    if (GetOption().IsWriteLog()) {
        unsigned char* dnsLog = reinterpret_cast<unsigned char*>(const_cast<char*>(GetOption().GetDNS(3)));
        if (!m_xLogDBMgr.Init(dnsLog, 3, nThreadCount)) {
            LogHelper::LogError("game.system", "[SYSTEM] Log DB init False ");
            return false;
        }
        LogHelper::LogInfo("game.system", "[INIT] m_xLogDBMgr - Init");
    }

    // Initialize StatisticsDB (DNS type 7) - only if IsWriteStatistics
    if (GetOption().IsWriteStatistics()) {
        unsigned char* dnsStats = reinterpret_cast<unsigned char*>(const_cast<char*>(GetOption().GetDNS(7)));
        if (!m_xStatisticsDBMgr.Init(dnsStats, 7, nThreadCount)) {
            LogHelper::LogError("game.system", "[SYSTEM] Statistics DB init False ");
            return false;
        }
        LogHelper::LogInfo("game.system", "[INIT] m_xStatisticsDBMgr - Init");
    }

    // Initialize CommonDB (DNS type 2) - fixed 2 threads
    unsigned char* dnsCommon = reinterpret_cast<unsigned char*>(const_cast<char*>(GetOption().GetDNS(2)));
    if (!m_xCommonDBMgr.Init(dnsCommon, 2, 2)) {
        LogHelper::LogError("game.system", "[SYSTEM] Common DB init False ");
        return false;
    }

    // Initialize SGNetCafeDB (DNS type 8) - only if IsWriteSGNetCafe
    if (GetOption().IsWriteSGNetCafe()) {
        unsigned char* dnsSGNetCafe = reinterpret_cast<unsigned char*>(const_cast<char*>(GetOption().GetDNS(8)));
        if (!m_xSGNetCafeDBMgr.Init(dnsSGNetCafe, 8, 2)) {
            LogHelper::LogError("game.system", "[SYSTEM] SG Netcafe DB init False ");
            return false;
        }
        LogHelper::LogInfo("game.system", "[INIT] m_xSGNetCafeDBMgr - Init");
    }

    GetOption().ShowServerInfo();

    // TODO: 需人工审查 - CObserveSocket::StartUp 需要实现
    // if (!m_scObserveSocket.StartUp(&m_xOption)) {
    //     return false;
    // }

    LogHelper::LogInfo("game.system", "[INIT] Complete Server Init");
    return true;
}

bool XDBAgent::Clear() {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();

    if (pAgent->GetOption().IsWriteLog()) {
        // TODO: 需人工审查 - CLogDB destructor call in IDA
    }

    GreenDamTan_LogThreadManagerEnd();
    return true;
}

void XDBAgent::OnUpdate(std::uint64_t dw64CurrentTick) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    int nPort = static_cast<int>(pAgent->GetOption().GetPort());
    const char* szIP = pAgent->GetOption().GetIP();

    // TODO: 需人工审查 - CObserveSocket::OnUpdate needs implementation
    // m_scObserveSocket.OnUpdate(dw64CurrentTick, szIP, nPort, 0, 0, 0, 0, 0);
}

void XDBAgent::PrintFPS(std::uint64_t dw64CurrentTick) {
    // TODO: 需人工审查 - PrintFPS 完整实现需要 XTime::GetTickCount 和 XPRINT
    if (m_dw64FPSTick == 0) {
        m_dw64FPSTick = dw64CurrentTick; // Simplified: use passed tick
    }

    if (dw64CurrentTick - m_dw64FPSTick >= 1000) {
        float fps = static_cast<float>(m_dwFrame) / static_cast<float>(dw64CurrentTick - m_dw64FPSTick);

        // TODO: 需人工审查 - DB connection alive check and XPRINT output

        m_dw64FPSTick = dw64CurrentTick;
        m_dwFrame = 0;
    } else {
        ++m_dwFrame;
    }

    // Sleep adjustment
    std::uint64_t elapsed = dw64CurrentTick; // Simplified
    if (elapsed < 15) {
        std::this_thread::sleep_for(std::chrono::milliseconds(15 - elapsed));
    }
}

int XDBAgent::SetConsoleHandler(int add) {
    // TODO: 需人工审查 - Console handler setup
    return add;
}

unsigned int __stdcall XDBAgent::ConsolCtrlHandler(unsigned long dwCtrlType) {
    // Per IDA: handle CTRL_C, CTRL_BREAK, CTRL_CLOSE, CTRL_LOGOFF, CTRL_SHUTDOWN
    if (dwCtrlType <= 2 || (dwCtrlType > 4 && dwCtrlType <= 6)) {
        auto* pAgent = TXSingleton<XDBAgent>::Instance();
        pAgent->Shutdown(0xFFFFFFFF);
        return 1;
    }
    return 0;
}