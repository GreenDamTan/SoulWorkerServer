#pragma once

// Forward declarations for DBAgent types
class XDBConnect;
class XDBEnv;
class XDBStmt;
class XPacket;
class XSQLProcess;

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/TXMultiPoolServer.h"
#include "Soulworker/GameServer/XCore/XServer/CObserveSocket.h"
#include "Soulworker/GameServer/XDBAgent/LogDB.h"

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

// Undo Windows macro pollution
#ifdef AddJob
#undef AddJob
#endif

#include "Soulworker/GameServer/XDBAgent/ServerCon.h"

class XDBThread {
public:
    XDBThread();
    virtual ~XDBThread();

    void SetStart(bool bStart);
    bool IsStart();
    void Clear();
    static unsigned int _BackendThreadStartingPoint(void* ptr);
    void _DBWorkingThread();
    bool CreateThread(XDBConnect* pDBConnect, std::uint8_t byType);
    void AddJob(std::function<void()> func);
    XDBStmt* GetDBStmt();

private:
    std::mutex m_queueMutex;
    std::condition_variable m_jobCv;
    std::queue<std::function<void()>> m_jobs;
    int m_nLastSessionID = 0;
    std::thread m_worker;
    unsigned int m_nThreadID = 0;
    std::atomic<bool> m_bStart{false};
    XDBConnect* m_pDBConnect = nullptr;
    XDBStmt* m_pDBStmt = nullptr;
    bool m_hasJobEvent = false;
    std::uint8_t m_byType = 0;
};

class XDBAgentDBManager {
public:
    class XDBCreator : public GreenDamTan_TXPool<XDBConnect>::XCreator {
    public:
        XDBCreator(unsigned char* szDNS, XDBEnv* pDBEnv) : m_szDNS(szDNS), m_pDBEnv(pDBEnv) {}
        XDBConnect* Create() override;

        unsigned char* m_szDNS = nullptr;
        XDBEnv* m_pDBEnv = nullptr;
    };

    XDBAgentDBManager();
    virtual ~XDBAgentDBManager();

    bool Init(unsigned char* szDNS, std::uint8_t byType, int nMaxConnectCount);
    XDBConnect* GetDBConnect();
    void CollectDBConnect(XDBConnect* pDBConnect);
    XDBStmt* GetDBStmt(unsigned int nIndex);
    void AddJob(int nIndex, std::function<void()> func);
    std::int64_t SetEnv();
    int GetMaxConnectCount();

    GreenDamTan_TXPool<XDBConnect> m_DBConnectPool;
    XDBEnv* m_pDBEnv = nullptr;
    int m_nMaxConnectCount = 0;
    XDBCreator* m_pDBConnectCreator = nullptr;
    XDBThread* m_pDBThread = nullptr;

    // Static helper for routing SQL jobs (matches IDA signature)
    // Declaration only - implementation in separate cpp to avoid circular deps
    static void AddJob(XDBAgentDBManager* pMgr, XSQLProcess* pProcess, XPacket& xPacket);
};

// XDBAgent - main DBAgent server class
// Size: 117056 (0x1C940) bytes per IDA
class XDBAgent : public TXMultiPoolServer<XServerCon>
{
public:
    XDBAgent();
    virtual ~XDBAgent();

    // XServer overrides
    void SetName() override;
    bool InitServer() override;
    bool Clear() override;
    int SetConsoleHandler(int add) override;
    void PrintFPS(std::uint64_t currentTick) override;
    void OnUpdate(std::uint64_t currentTick) override;

    static unsigned int __stdcall ConsolCtrlHandler(unsigned long dwCtrlType);

    // DB Manager members (per IDA struct layout)
    // gap at offset 46264 (0xB4B8) for padding
    char _gapB4B8[1] = {};  // padding between base class and members

    XDBAgentDBManager m_xAccountDBMgr;   // offset 46272 (0xB4C0)
    XDBAgentDBManager m_xGameDBMgr;      // offset 46448 (0xB570)
    XDBAgentDBManager m_xLogDBMgr;       // offset 46624 (0xB620)
    XDBAgentDBManager m_xStatisticsDBMgr; // offset 46800 (0xB6D0)
    XDBAgentDBManager m_xSGNetCafeDBMgr;  // offset 46976 (0xB780)
    XDBAgentDBManager m_xCommonDBMgr;    // offset 47152 (0xB830)
    CLogDB m_xLogDB;                      // offset 47328 (0xB8E0)
    CObserveSocket m_scObserveSocket;     // offset 47336 (0xB8E8)
};