// GameDBSocketMgr.cpp
// XGameDBSocketMgr implementation
// Reconstructed from IDA decompilation: GameServer.exe
//
// Functions:
// - XGameDBSocketMgr::Init (0x1401ec400)

#include "GameDBSocketMgr.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include <new>
#include <cstring>

// Forward declarations
class XGameServer;
class XOption;
template<typename T> class TXSingleton;

// E_POOL_ID enumeration
enum E_POOL_ID
{
    ePoolIDNone = 0,
};

// ============================================================================
// XGameDBSocketMgr implementation
// ============================================================================

XGameDBSocketMgr::XGameDBSocketMgr()
    : m_pGameDBAgent(nullptr)
    , m_nGameAgentCnt(0)
    , m_pAccountDBAgent(nullptr)
    , m_nAccountAgentCnt(0)
    , m_pLogDBAgent(nullptr)
    , m_nLogAgentCnt(0)
    , m_pStatisticsDBAgent(nullptr)
    , m_nStatisticsAgentCnt(0)
    , m_pSGLogDBAgent(nullptr)
    , m_nSGLogAgentCnt(0)
{
}

XGameDBSocketMgr::~XGameDBSocketMgr()
{
    // Delete all socket arrays
    if (m_pGameDBAgent) {
        delete[] m_pGameDBAgent;
        m_pGameDBAgent = nullptr;
    }
    if (m_pAccountDBAgent) {
        delete[] m_pAccountDBAgent;
        m_pAccountDBAgent = nullptr;
    }
    if (m_pLogDBAgent) {
        delete[] m_pLogDBAgent;
        m_pLogDBAgent = nullptr;
    }
    if (m_pStatisticsDBAgent) {
        delete[] m_pStatisticsDBAgent;
        m_pStatisticsDBAgent = nullptr;
    }
    if (m_pSGLogDBAgent) {
        delete[] m_pSGLogDBAgent;
        m_pSGLogDBAgent = nullptr;
    }
}

// Init - Initialize all database socket connections
// IDA @ 0x1401EC400
void XGameDBSocketMgr::Init()
{
    // IDA code summary:
    // This function initializes 5 types of database socket arrays:
    // 1. GameDB (type 0)
    // 2. AccountDB (type 1)
    // 3. LogDB (type 2)
    // 4. StatisticsDB (type 3)
    // 5. SGLogDB (type 4)
    //
    // For each type:
    // - Get DB agent info from XOption (IP, port, name, max IO pool count, agent count)
    // - Allocate array of XGameDBSocket objects (size 0x10568 = 66920 bytes each)
    // - Initialize each socket with Init_2()
    // - Set m_bState = 1 and m_byType = type index

    char szIP[513];
    short shPort;
    char szName[21];
    int nMaxIOPoolCount;
    int nDBAgentCnt;

    // Initialize GameDB agents (type 0)
    memset(szIP, 0, sizeof(szIP));
    shPort = 0;
    memset(szName, 0, sizeof(szName));
    nMaxIOPoolCount = 0;

    // TODO: Get DB agent info from XOption
    // XGameServer* pServer = TXSingleton<XGameServer>::Instance();
    // XOption* pOption = pServer->GetOption();
    // if (pOption->GetDBAgentInfo(0, szIP, &shPort, szName, &nMaxIOPoolCount, &nDBAgentCnt))
    // {
    //     // Allocate socket array
    //     m_pGameDBAgent = new XGameDBSocket[nDBAgentCnt];
    //     m_nGameAgentCnt = nDBAgentCnt;
    //
    //     // Initialize each socket
    //     for (int i = 0; i < nDBAgentCnt; ++i)
    //     {
    //         if (m_pGameDBAgent[i].Init_2(ePoolIDNone, szIP, shPort, szName, nMaxIOPoolCount))
    //         {
    //             m_pGameDBAgent[i].m_bState = 1;
    //             m_pGameDBAgent[i].m_byType = eDBAgent_GameDB;
    //         }
    //     }
    // }

    // Similar initialization for AccountDB, LogDB, StatisticsDB, SGLogDB...
    // (type 1, 2, 3, 4 respectively)

    GreenDamTan_log(__FILE__, __FUNCTION__, "Init - IDA精确还原 (需要XOption/XGameDBSocket依赖)");
}
