#include "Soulworker/GameServer/XDBAgent/DBAgent.h"
#include "Soulworker/GameServer/XDBAgent/LogDB.h"

#include <cstdlib>

namespace {
XDBStmt* GreenDamTan_GetLogStmt(XDBAgentDBManager& mgr) {
    if (mgr.GetMaxConnectCount() <= 0) {
        return nullptr;
    }
    const int index = std::rand() % mgr.GetMaxConnectCount();
    return mgr.GetDBStmt(static_cast<unsigned int>(index));
}
}

// TODO: 汇编还原 - CLogDB methods need full implementation with log structures
void CLogDB::WriteLog(ST_LOG_GAME& stLog) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nIndex = std::rand() % pAgent->m_xLogDBMgr.GetMaxConnectCount();
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(static_cast<unsigned int>(nIndex));
    if (!pDBStmt) {
        return;
    }
    // TODO: 需人工审查 - LogQuery implementation
}

void CLogDB::WriteChatLog(int nUAID, int nUCID, float fUpdateTime, std::int64_t biAuthSessionID,
                          int nCharType, int nMapID, int nChannelID, int nCharLv, int nClass,
                          int nResult, const wchar_t* szMsg) {
    // TODO: 需人工审查 - WriteChatLog implementation
}

void CLogDB::WriteStatLog(int, int, float, float, float, float, float, float, float, float, float) {}
void CLogDB::WriteCashLog(int, int, std::int64_t, int, int, int, int, int, int, const char*) {}
void CLogDB::WriteClientLog(int, int, std::uint8_t, const char*) {}
void CLogDB::WriteConnectServerLog(int, int, std::uint8_t) {}
void CLogDB::WriteClassEventLog(std::uint32_t, std::uint32_t, std::uint8_t, int) {}
void CLogDB::WriteSGLog(ST_LOG_GAME& stLog) { WriteLog(stLog); }
void CLogDB::WriteSGCharInfo(ST_LOG_SG_CHAR&) {}
void CLogDB::WriteSGToken(ST_SG_AUTH_INFO&) {}
void CLogDB::WriteTextLog(ST_LOG_TEXT&) {}
void CLogDB::WriteMoneyLog(ST_LOG_MONEY&) {}

int CLogDB::SystemLogQuery(XDBStmt* pDBStmt, ST_LOG_SYSTEM& stLog) {
    // TODO: 需人工审查 - SystemLogQuery implementation
    return 0;
}

int CLogDB::LogQuery(XDBStmt*, ST_LOG_GAME&) { return 0; }
int CLogDB::ChatLogQuery(XDBStmt*, int, int, float, std::int64_t, int, int, int, int, int, int, wchar_t*) { return 0; }
int CLogDB::CashLogQuery(XDBStmt*, int, int, std::int64_t, int, int, int, int, int, int, char*) { return 0; }
int CLogDB::StatLogQuery(XDBStmt*, int, int, float&, float&, float&, float&, float&, float&, float&, float&, float&) { return 0; }
int CLogDB::ClientLogQuery(XDBStmt*, int, int, std::uint8_t, char*) { return 0; }
int CLogDB::ConnectServerLogQuery(XDBStmt*, int, int, std::uint8_t) { return 0; }
int CLogDB::ClassEventLogQuery(XDBStmt*, std::uint32_t, std::uint32_t, std::uint8_t, int) { return 0; }
int CLogDB::SGLogQuery(XDBStmt*, ST_LOG_GAME&) { return 0; }
int CLogDB::SGTokenQuery(XDBStmt*, ST_SG_AUTH_INFO&) { return 0; }
int CLogDB::TextLogQuery(XDBStmt*, ST_LOG_TEXT&) { return 0; }
int CLogDB::MoneyLogQuery(XDBStmt*, ST_LOG_MONEY&) { return 0; }
