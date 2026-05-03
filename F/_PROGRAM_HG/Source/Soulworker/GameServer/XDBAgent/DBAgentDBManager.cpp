#include "Soulworker/GameServer/XDBAgent/DBAgent.h"

#include <vector>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#endif

XDBAgentDBManager::XDBAgentDBManager() {
    // Per IDA: m_nMaxConnectCount default = 8
    m_pDBEnv = nullptr;
    m_nMaxConnectCount = 8;
    m_pDBConnectCreator = nullptr;
    m_pDBThread = nullptr;
}

XDBAgentDBManager::~XDBAgentDBManager() {
    // TODO: 需人工审查 - destructor cleanup
    if (m_pDBThread) {
        for (int i = 0; i < m_nMaxConnectCount; ++i) {
            m_pDBThread[i].SetStart(false);
        }
        // Delete thread array
        operator delete(m_pDBThread);
        m_pDBThread = nullptr;
    }
    if (m_pDBConnectCreator) {
        delete m_pDBConnectCreator;
        m_pDBConnectCreator = nullptr;
    }
}

bool XDBAgentDBManager::Init(unsigned char* szDNS, std::uint8_t byType, int nMaxConnectCount) {
    m_nMaxConnectCount = nMaxConnectCount;

    // SetEnv returns non-zero on error per IDA
    if (SetEnv() != 0) {
        return false;
    }

    // Create XDBCreator
    m_pDBConnectCreator = new XDBCreator(szDNS, m_pDBEnv);

    // Initialize connection pool - single arg version
    m_DBConnectPool.Init(m_nMaxConnectCount);

    // Allocate thread array
    // Per IDA: new XDBThread[count] with vector constructor
    m_pDBThread = new XDBThread[m_nMaxConnectCount];

    bool bOK = true;
    XDBConnect* pDBConnect = nullptr;

    for (int i = 0; i < m_nMaxConnectCount; ++i) {
        pDBConnect = GetDBConnect();
        if (!pDBConnect || !m_pDBThread[i].CreateThread(pDBConnect, byType)) {
            bOK = false;
            break;
        }
    }

    if (!bOK) {
        for (int j = 0; j < m_nMaxConnectCount; ++j) {
            m_pDBThread[j].SetStart(false);
        }
    }

    return true;
}

XDBConnect* XDBAgentDBManager::GetDBConnect() {
    // Pop from pool if available
    XDBConnect* pConnect = m_DBConnectPool.Pop();
    if (pConnect) {
        return pConnect;
    }

    // Pool empty - try to add new connection
    if (!m_DBConnectPool.Add(m_pDBConnectCreator)) {
        // TODO: 需人工审查 - XPRINT macro not defined, using printf temporarily
        printf("]] Add Cannot Connect DB)\n");
        return nullptr;
    }

    // TODO: 需人工审查 - get pool size (GetFullSize not available in stub)
    printf("]] m_DBConnectPool.Add Max Size[%d]\n", m_nMaxConnectCount);
    return nullptr;
}

void XDBAgentDBManager::CollectDBConnect(XDBConnect* pDBConnect) {
    m_DBConnectPool.Push(pDBConnect);
}

XDBStmt* XDBAgentDBManager::GetDBStmt(unsigned int nIndex) {
    if (static_cast<int>(nIndex) < m_nMaxConnectCount && m_pDBThread) {
        return m_pDBThread[nIndex].GetDBStmt();
    }
    return nullptr;
}

void XDBAgentDBManager::AddJob(int nIndex, std::function<void()> func) {
    if (nIndex >= 0 && nIndex < m_nMaxConnectCount && m_pDBThread) {
        m_pDBThread[nIndex].AddJob(std::move(func));
    }
}

std::int64_t XDBAgentDBManager::SetEnv() {
    // TODO: 需人工审查 - SetEnv 完整实现需要环境变量检查
    // Per IDA: returns 0 on success
    return 0;
}

int XDBAgentDBManager::GetMaxConnectCount() {
    return m_nMaxConnectCount;
}

// XDBCreator::Create implementation
XDBConnect* XDBAgentDBManager::XDBCreator::Create() {
    // TODO: 需人工审查 - XDBConnect creation needs ODBC implementation
    XDBConnect* pConnect = new XDBConnect();
    // TODO: 汇编还原 - actual connect logic using m_szDNS and m_pDBEnv
    return pConnect;
}

// Static AddJob for routing SQL jobs
void XDBAgentDBManager::AddJob(XDBAgentDBManager* pMgr, XSQLProcess* pProcess, XPacket& xPacket) {
    if (!pMgr || !pProcess) {
        return;
    }

    // Per IDA: parse session ID from packet to determine thread index
    int xReturnSessionID = 0;
    xPacket.XParse >> xReturnSessionID;
    unsigned int nIndex = static_cast<unsigned int>(xReturnSessionID % pMgr->GetMaxConnectCount());

    if (nIndex >= static_cast<unsigned int>(pMgr->GetMaxConnectCount())) {
        return;
    }

    if (!pMgr->m_pDBThread) {
        return;
    }

    XDBStmt* pDBStmt = pMgr->m_pDBThread[nIndex].GetDBStmt();
    if (!pDBStmt) {
        return;
    }

    // Create packet header copy
    PACKET_HEADER* pHeader = new PACKET_HEADER();
    pHeader->usSize = xPacket.usSize;
    pHeader->usVer = xPacket.usVer;
    pHeader->usTos = xPacket.usTos;

    // Copy packet data
    char* pBuffer = new char[xPacket.usSize];
    std::memcpy(pBuffer, xPacket.m_pRoot, xPacket.usSize);

    // TODO: 需人工审查 - lambda capture and job scheduling
    // Per IDA: creates a lambda that calls the process with stmt
    auto job = [pProcess, pHeader, pBuffer, pDBStmt]() {
        // TODO: 汇编还原 - actual process execution
        delete pHeader;
        delete[] pBuffer;
    };

    pMgr->m_pDBThread[nIndex].AddJob(std::move(job));
}