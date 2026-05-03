#pragma once

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"

// Forward declarations for log structures not yet defined
struct ST_LOG_SG_CHAR;
struct ST_LOG_TEXT;
struct ST_LOG_MONEY;
struct ST_LOG_SYSTEM;
class XDBStmt;

// CLogDB - Log database handler class
// Size: 1 byte per IDA (placeholder with methods)
class CLogDB {
public:
    CLogDB() = default;
    ~CLogDB() = default;

    void WriteLog(ST_LOG_GAME& stLog);
    void WriteChatLog(int nUAID, int nUCID, float fUpdateTime, std::int64_t biAuthSessionID,
                      int nCharType, int nMapID, int nChannelID, int nCharLv, int nClass,
                      int nResult, const wchar_t* szMsg);
    void WriteStatLog(int, int, float, float, float, float, float, float, float, float, float);
    void WriteCashLog(int, int, std::int64_t, int, int, int, int, int, int, const char*);
    void WriteClientLog(int, int, std::uint8_t, const char*);
    void WriteConnectServerLog(int, int, std::uint8_t);
    void WriteClassEventLog(std::uint32_t, std::uint32_t, std::uint8_t, int);
    void WriteSGLog(ST_LOG_GAME& stLog);
    void WriteSGCharInfo(ST_LOG_SG_CHAR&);
    void WriteSGToken(ST_SG_AUTH_INFO&);
    void WriteTextLog(ST_LOG_TEXT&);
    void WriteMoneyLog(ST_LOG_MONEY&);

    int SystemLogQuery(XDBStmt* pDBStmt, ST_LOG_SYSTEM& stLog);
    int LogQuery(XDBStmt*, ST_LOG_GAME&);
    int ChatLogQuery(XDBStmt*, int, int, float, std::int64_t, int, int, int, int, int, int, wchar_t*);
    int CashLogQuery(XDBStmt*, int, int, std::int64_t, int, int, int, int, int, int, char*);
    int StatLogQuery(XDBStmt*, int, int, float&, float&, float&, float&, float&, float&, float&, float&, float&);
    int ClientLogQuery(XDBStmt*, int, int, std::uint8_t, char*);
    int ConnectServerLogQuery(XDBStmt*, int, int, std::uint8_t);
    int ClassEventLogQuery(XDBStmt*, std::uint32_t, std::uint32_t, std::uint8_t, int);
    int SGLogQuery(XDBStmt*, ST_LOG_GAME&);
    int SGTokenQuery(XDBStmt*, ST_SG_AUTH_INFO&);
    int TextLogQuery(XDBStmt*, ST_LOG_TEXT&);
    int MoneyLogQuery(XDBStmt*, ST_LOG_MONEY&);
};
