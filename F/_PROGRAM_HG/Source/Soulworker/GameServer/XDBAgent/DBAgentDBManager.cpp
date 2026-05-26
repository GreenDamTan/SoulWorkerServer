#include "Soulworker/GameServer/XDBAgent/DBAgent.h"
#include "Soulworker/GameServer/XDBAgent/SQLProcess.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

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

    // Per IDA 0x1400068A0: SetEnv returns non-zero on error
    if (SetEnv() != 0) {
        return false;
    }

    // Create XDBCreator
    m_pDBConnectCreator = new XDBCreator(szDNS, m_pDBEnv);

    // Initialize connection pool - Per IDA: TXPool::Init(count, creator, 1)
    // Our simplified version only takes count parameter
    m_DBConnectPool.Init(m_nMaxConnectCount);

    // Allocate thread array - Per IDA: new XDBThread[count] with vector constructor
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
    // Per IDA 0x140006BE0: Check pool size first
    if (m_DBConnectPool.GetCurSize() > 0) {
        return m_DBConnectPool.Pop();
    }

    // Pool empty - try to add new connection
    if (!m_DBConnectPool.Add(m_pDBConnectCreator)) {
        // Per IDA: XPRINT for error
        printf("]] Add Cannot Connect DB)\n");
        return nullptr;
    }

    // Per IDA: XPRINT for max size info
    int fullSize = m_DBConnectPool.GetFullSize();
    printf("]] m_DBConnectPool.Add Max Size[%d]\n", fullSize);
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
    // Per IDA 0x140007130: creates XDBEnv and calls Init
    // Returns SQL return code (0 = success)
    XDBEnv* pDBEnv = new XDBEnv();
    m_pDBEnv = pDBEnv;
    std::int16_t sqlReturn = pDBEnv->Init();
    return sqlReturn;
}

int XDBAgentDBManager::GetMaxConnectCount() {
    return m_nMaxConnectCount;
}

// XDBCreator::Create implementation
// Per IDA 0x140008320: Creates XDBConnect, calls Init and Connect
XDBConnect* XDBAgentDBManager::XDBCreator::Create() {
    XDBConnect* pConnect = new XDBConnect();

    // Per IDA: XDBConnect::Init returns SQL return code
    if (pConnect->Init(m_pDBEnv) != 0) {
        delete pConnect;
        return nullptr;
    }

    // Per IDA: XDBConnect::Connect returns SQL return code
    if (pConnect->Connect(reinterpret_cast<char*>(m_szDNS)) != 0) {
        delete pConnect;
        return nullptr;
    }

    return pConnect;
}

// Static AddJob for routing SQL jobs
void XDBAgentDBManager::AddJob(XDBAgentDBManager* pMgr, XSQLProcess* pProcess, XPacket& xPacket) {
    if (!pMgr || !pProcess) {
        return;
    }

    // Per IDA 0x140006D20: parse session ID from packet to determine thread index
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

    // Create packet header copy - Per IDA: operator new(5) for 5-byte header
    PACKET_HEADER* pHeader = new PACKET_HEADER();
    pHeader->usSize = xPacket.usSize;
    pHeader->usVer = xPacket.usVer;
    pHeader->usTos = xPacket.usTos;

    // Copy packet data
    char* pBuffer = new char[xPacket.usSize];
    std::memcpy(pBuffer, xPacket.m_pRoot, xPacket.usSize);

    // Per IDA: lambda captures pProcess, pHeader, pBuffer, pDBStmt
    // Simplified implementation: directly call DBParse with the original packet
    // Note: Full packet reconstruction would require GreenDamTan_AssignNetworkPacket
    auto job = [pProcess, pHeader, pBuffer, pDBStmt, xReturnSessionID]() {
        // Reconstruct XPacket from copied buffer using the proper API
        XPacket reconstructedPacket;
        reconstructedPacket.usSize = pHeader->usSize;
        reconstructedPacket.usVer = pHeader->usVer;
        reconstructedPacket.usTos = pHeader->usTos;

        // Use GreenDamTan_AssignNetworkPacket to properly set up the packet
        // The buffer contains the full packet starting from PACKET_ROOT
        reconstructedPacket.GreenDamTan_AssignNetworkPacket(pBuffer, pHeader->usSize);

        // Skip the session ID that was already read
        int dummySessionID = 0;
        reconstructedPacket.XParse >> dummySessionID;

        // Execute SQL process
        pProcess->DBParse(pDBStmt, reconstructedPacket, xReturnSessionID);

        // Cleanup
        delete pHeader;
        delete[] pBuffer;
    };

    pMgr->m_pDBThread[nIndex].AddJob(std::move(job));
}