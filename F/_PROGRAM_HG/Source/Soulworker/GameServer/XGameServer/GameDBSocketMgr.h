// GameDBSocketMgr.h
// XGameDBSocketMgr - Game database socket manager
// IDA decompilation from GameServer.exe
//
// Functions:
// - XGameDBSocketMgr::Init (0x1401ec400)

#pragma once

#include <cstdint>

// Forward declarations
class XGameDBSocket;
class XOption;

// E_DBAGENT_TYPE - Database agent type enumeration
enum E_DBAGENT_TYPE : int
{
    eDBAgent_GameDB = 0,       // Game database
    eDBAgent_AccountDB = 1,    // Account database
    eDBAgent_LogDB = 2,        // Log database
    eDBAgent_StatisticsDB = 3, // Statistics database
    eDBAgent_SGLogDB = 4,      // SG log database
};

// ============================================================================
// XGameDBSocketMgr - Game database socket manager
// Manages connections to various database agents (GameDB, AccountDB, LogDB, etc.)
// ============================================================================
class XGameDBSocketMgr {
public:
    // Constructor
    XGameDBSocketMgr();

    // Destructor
    ~XGameDBSocketMgr();

    // Init - Initialize all database socket connections
    // IDA: ?Init@XGameDBSocketMgr@@QEAAXXZ @ 0x1401EC400
    void Init();

    // GetGameDBAgent - Get GameDB socket by index
    XGameDBSocket* GetGameDBAgent(int nIndex = 0) {
        if (nIndex >= 0 && nIndex < m_nGameAgentCnt)
            return &m_pGameDBAgent[nIndex];
        return nullptr;
    }

    // GetAccountDBAgent - Get AccountDB socket by index
    XGameDBSocket* GetAccountDBAgent(int nIndex = 0) {
        if (nIndex >= 0 && nIndex < m_nAccountAgentCnt)
            return &m_pAccountDBAgent[nIndex];
        return nullptr;
    }

    // GetLogDBAgent - Get LogDB socket by index
    XGameDBSocket* GetLogDBAgent(int nIndex = 0) {
        if (nIndex >= 0 && nIndex < m_nLogAgentCnt)
            return &m_pLogDBAgent[nIndex];
        return nullptr;
    }

    // GetStatisticsDBAgent - Get StatisticsDB socket by index
    XGameDBSocket* GetStatisticsDBAgent(int nIndex = 0) {
        if (nIndex >= 0 && nIndex < m_nStatisticsAgentCnt)
            return &m_pStatisticsDBAgent[nIndex];
        return nullptr;
    }

    // GetSGLogDBAgent - Get SGLogDB socket by index
    XGameDBSocket* GetSGLogDBAgent(int nIndex = 0) {
        if (nIndex >= 0 && nIndex < m_nSGLogAgentCnt)
            return &m_pSGLogDBAgent[nIndex];
        return nullptr;
    }

    // Get counts
    int GetGameAgentCount() const { return m_nGameAgentCnt; }
    int GetAccountAgentCount() const { return m_nAccountAgentCnt; }
    int GetLogAgentCount() const { return m_nLogAgentCnt; }
    int GetStatisticsAgentCount() const { return m_nStatisticsAgentCnt; }
    int GetSGLogAgentCount() const { return m_nSGLogAgentCnt; }

protected:
    // === Member Variables ===
    XGameDBSocket* m_pGameDBAgent;        // GameDB socket array
    int m_nGameAgentCnt;                  // GameDB agent count

    XGameDBSocket* m_pAccountDBAgent;     // AccountDB socket array
    int m_nAccountAgentCnt;               // AccountDB agent count

    XGameDBSocket* m_pLogDBAgent;         // LogDB socket array
    int m_nLogAgentCnt;                   // LogDB agent count

    XGameDBSocket* m_pStatisticsDBAgent;  // StatisticsDB socket array
    int m_nStatisticsAgentCnt;            // StatisticsDB agent count

    XGameDBSocket* m_pSGLogDBAgent;       // SGLogDB socket array
    int m_nSGLogAgentCnt;                 // SGLogDB agent count
};
