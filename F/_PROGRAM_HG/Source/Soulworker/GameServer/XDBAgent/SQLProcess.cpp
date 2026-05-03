#include "Soulworker/GameServer/XDBAgent/SQLProcess.h"
#include "Soulworker/GameServer/XDBAgent/DBAgent.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"

XSQLProcess::XSQLProcess() = default;

XSQLProcess::~XSQLProcess() = default;

bool XSQLProcess::Init(XClient* pClient) {
    return true;
}

bool XSQLProcess::Clear() {
    return true;
}

bool XSQLProcess::Parse(XPacket& xPacket) {
    const char MainCmd = xPacket.GetMainCmd();
    auto* pAgent = TXSingleton<XDBAgent>::Instance();

    switch (MainCmd) {
    case 2:
        XDBAgentDBManager::AddJob(&pAgent->m_xAccountDBMgr, this, xPacket);
        break;

    case 66: // 0x42
        if (!pAgent->GetOption().IsWriteLog()) {
            return true;
        }
        XDBAgentDBManager::AddJob(&pAgent->m_xLogDBMgr, this, xPacket);
        break;

    case -16: // 0xF0
        if (!pAgent->GetOption().IsWriteStatistics()) {
            return true;
        }
        XDBAgentDBManager::AddJob(&pAgent->m_xStatisticsDBMgr, this, xPacket);
        break;

    case -14: // 0xF2
        if (!pAgent->GetOption().IsWriteSGNetCafe()) {
            return true;
        }
        XDBAgentDBManager::AddJob(&pAgent->m_xSGNetCafeDBMgr, this, xPacket);
        break;

    case -13: // 0xF3
        XDBAgentDBManager::AddJob(&pAgent->m_xCommonDBMgr, this, xPacket);
        break;

    default:
        XDBAgentDBManager::AddJob(&pAgent->m_xGameDBMgr, this, xPacket);
        break;
    }

    return true;
}
