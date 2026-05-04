#include "Soulworker/GameServer/XDBAgent/DBAgent.h"
#include "Soulworker/GameServer/XDBAgent/LogDB.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"

#include <cstdlib>
#include <functional>

namespace {
XDBStmt* GreenDamTan_GetLogStmt(XDBAgentDBManager& mgr) {
    if (mgr.GetMaxConnectCount() <= 0) {
        return nullptr;
    }
    const int index = std::rand() % mgr.GetMaxConnectCount();
    return mgr.GetDBStmt(static_cast<unsigned int>(index));
}
}

void CLogDB::WriteLog(ST_LOG_GAME& stLog) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nMaxConn = pAgent->m_xLogDBMgr.GetMaxConnectCount();
    if (nMaxConn <= 0) {
        return;
    }
    const unsigned int nIndex = static_cast<unsigned int>(std::rand() % nMaxConn);
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(nIndex);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: lambda 捕获 stLog 副本和 pDBStmt
    auto func = [this, stLog, pDBStmt]() {
        ST_LOG_GAME localLog = stLog;
        LogQuery(pDBStmt, localLog);
    };

    pAgent->m_xLogDBMgr.AddJob(static_cast<int>(nIndex), func);
}

void CLogDB::WriteChatLog(int nUAID, int nUCID, std::int16_t sType, int nParam0,
                          int nParam1, int nParam2, int nParam3, int nParam4,
                          std::int64_t nParam5, std::int64_t nParam6, const wchar_t* szMsg) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nMaxConn = pAgent->m_xLogDBMgr.GetMaxConnectCount();
    if (nMaxConn <= 0) {
        return;
    }
    const unsigned int nIndex = static_cast<unsigned int>(std::rand() % nMaxConn);
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(nIndex);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: 分配 wchar_t buffer 复制消息字符串
    wchar_t* pComment = new wchar_t[0x101];
    wcscpy_s(pComment, 0x101, szMsg);

    // 对齐 IDA: lambda 捕获所有参数副本和 pDBStmt
    auto func = [this, nUAID, nUCID, sType, nParam0, nParam1, nParam2, nParam3, nParam4, nParam5, nParam6, pComment, pDBStmt]() {
        ChatLogQuery(pDBStmt, nUAID, nUCID, sType, static_cast<std::int64_t>(nParam0),
                     nParam1, nParam2, nParam3, nParam4, nParam5, nParam6, pComment);
        delete[] pComment;
    };

    pAgent->m_xLogDBMgr.AddJob(static_cast<int>(nIndex), func);
}

void CLogDB::WriteStatLog(int nUAID, int nUCID, float fParam0, float fParam1, float fParam2,
                          float fParam3, float fParam4, float fParam5, float fParam6, float fParam7, float fParam8) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nMaxConn = pAgent->m_xLogDBMgr.GetMaxConnectCount();
    if (nMaxConn <= 0) {
        return;
    }
    const unsigned int nIndex = static_cast<unsigned int>(std::rand() % nMaxConn);
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(nIndex);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: lambda 捕获所有参数副本和 pDBStmt
    // 注意: IDA 中 StatLogQuery 只使用 7 个 float (Param0-Param6)
    auto func = [this, nUAID, nUCID, fParam0, fParam1, fParam2, fParam3, fParam4, fParam5, fParam6, pDBStmt]() {
        StatLogQuery(pDBStmt, nUAID, nUCID, fParam0, fParam1, fParam2, fParam3, fParam4, fParam5, fParam6);
    };

    pAgent->m_xLogDBMgr.AddJob(static_cast<int>(nIndex), func);
}
void CLogDB::WriteCashLog(int nUAID, int nUCID, std::int64_t nOrderNo,
                         int nParam1, int nParam2, int nParam3, int nParam4,
                         int nParam5, int nParam6, const char* szComment) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nMaxConn = pAgent->m_xLogDBMgr.GetMaxConnectCount();
    if (nMaxConn <= 0) {
        return;
    }
    const unsigned int nIndex = static_cast<unsigned int>(std::rand() % nMaxConn);
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(nIndex);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: 分配 char buffer 复制评论
    char* pComment = new char[0x15];
    strcpy_s(pComment, 0x15, szComment);

    // 对齐 IDA: lambda 捕获所有参数副本和 pDBStmt
    auto func = [this, nUAID, nUCID, nOrderNo, nParam1, nParam2, nParam3, nParam4, nParam5, nParam6, pComment, pDBStmt]() {
        CashLogQuery(pDBStmt, nUAID, nUCID, nOrderNo, nParam1, nParam2, nParam3, nParam4, nParam5, nParam6, pComment);
        delete[] pComment;
    };

    pAgent->m_xLogDBMgr.AddJob(static_cast<int>(nIndex), func);
}
void CLogDB::WriteClientLog(int nUAID, int nUCID, std::uint8_t byType, const char* szComment) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nMaxConn = pAgent->m_xLogDBMgr.GetMaxConnectCount();
    if (nMaxConn <= 0) {
        return;
    }
    const unsigned int nIndex = static_cast<unsigned int>(std::rand() % nMaxConn);
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(nIndex);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: 分配 char buffer 复制评论，使用 strncpy_s
    char* pComment = new char[0x80];
    strncpy_s(pComment, 0x80, szComment, 0x7F);

    // 对齐 IDA: lambda 捕获所有参数副本和 pDBStmt
    auto func = [this, nUAID, nUCID, byType, pComment, pDBStmt]() {
        ClientLogQuery(pDBStmt, nUAID, nUCID, byType, pComment);
        delete[] pComment;
    };

    pAgent->m_xLogDBMgr.AddJob(static_cast<int>(nIndex), func);
}
void CLogDB::WriteConnectServerLog(int nUCID, int nServerID, std::uint8_t byType) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nMaxConn = pAgent->m_xLogDBMgr.GetMaxConnectCount();
    if (nMaxConn <= 0) {
        return;
    }
    const unsigned int nIndex = static_cast<unsigned int>(std::rand() % nMaxConn);
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(nIndex);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: lambda 捕获所有参数副本和 pDBStmt
    auto func = [this, nUCID, nServerID, byType, pDBStmt]() {
        ConnectServerLogQuery(pDBStmt, nUCID, nServerID, byType);
    };

    pAgent->m_xLogDBMgr.AddJob(static_cast<int>(nIndex), func);
}
void CLogDB::WriteClassEventLog(std::uint32_t dwUAID, std::uint32_t dwUCID,
                                 std::uint8_t byType, int nReward) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nMaxConn = pAgent->m_xLogDBMgr.GetMaxConnectCount();
    if (nMaxConn <= 0) {
        return;
    }
    const unsigned int nIndex = static_cast<unsigned int>(std::rand() % nMaxConn);
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(nIndex);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: lambda 捕获所有参数副本和 pDBStmt
    auto func = [this, dwUAID, dwUCID, byType, nReward, pDBStmt]() {
        ClassEventLogQuery(pDBStmt, dwUAID, dwUCID, byType, nReward);
    };

    pAgent->m_xLogDBMgr.AddJob(static_cast<int>(nIndex), func);
}
void CLogDB::WriteSGLog(ST_LOG_GAME& stLog) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nMaxConn = pAgent->m_xLogDBMgr.GetMaxConnectCount();
    if (nMaxConn <= 0) {
        return;
    }
    const unsigned int nIndex = static_cast<unsigned int>(std::rand() % nMaxConn);
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(nIndex);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: lambda 捕获 stLog 副本和 pDBStmt
    auto func = [this, stLog, pDBStmt]() {
        ST_LOG_GAME localLog = stLog;
        SGLogQuery(pDBStmt, localLog);
    };

    pAgent->m_xLogDBMgr.AddJob(static_cast<int>(nIndex), func);
}
void CLogDB::WriteSGCharInfo(ST_LOG_SG_CHAR& stLog) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    // 对齐 IDA: WriteSGCharInfo 固定使用索引 0
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(0);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: lambda 内部调用 SGLogQuery（复用 ST_LOG_GAME 的查询逻辑）
    // 因为 ST_LOG_SG_CHAR 只有 szMsg 字段，直接复用 SGLogQuery
    auto func = [this, stLog, pDBStmt]() {
        // 将 ST_LOG_SG_CHAR 强转为 ST_LOG_GAME（因为 SGLogQuery 参数是 ST_LOG_GAME）
        // 实际上这里只是传递 szMsg 数据
        SGLogQuery(pDBStmt, reinterpret_cast<ST_LOG_GAME&>(const_cast<ST_LOG_SG_CHAR&>(stLog)));
    };

    pAgent->m_xLogDBMgr.AddJob(0, func);
}
void CLogDB::WriteSGToken(ST_SG_AUTH_INFO& stLog) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nMaxConn = pAgent->m_xLogDBMgr.GetMaxConnectCount();
    if (nMaxConn <= 0) {
        return;
    }
    const unsigned int nIndex = static_cast<unsigned int>(std::rand() % nMaxConn);
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(nIndex);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: lambda 捕获 stLog 副本和 pDBStmt
    auto func = [this, stLog, pDBStmt]() {
        ST_SG_AUTH_INFO localLog = stLog;
        SGTokenQuery(pDBStmt, localLog);
    };

    pAgent->m_xLogDBMgr.AddJob(static_cast<int>(nIndex), func);
}
void CLogDB::WriteTextLog(ST_LOG_TEXT& stLog) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nMaxConn = pAgent->m_xLogDBMgr.GetMaxConnectCount();
    if (nMaxConn <= 0) {
        return;
    }
    const unsigned int nIndex = static_cast<unsigned int>(std::rand() % nMaxConn);
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(nIndex);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: lambda 捕获 stLog 副本和 pDBStmt
    auto func = [this, stLog, pDBStmt]() {
        ST_LOG_TEXT localLog = stLog;
        TextLogQuery(pDBStmt, localLog);
    };

    pAgent->m_xLogDBMgr.AddJob(static_cast<int>(nIndex), func);
}
void CLogDB::WriteMoneyLog(ST_LOG_MONEY& stLog) {
    auto* pAgent = TXSingleton<XDBAgent>::Instance();
    const int nMaxConn = pAgent->m_xLogDBMgr.GetMaxConnectCount();
    if (nMaxConn <= 0) {
        return;
    }
    const unsigned int nIndex = static_cast<unsigned int>(std::rand() % nMaxConn);
    XDBStmt* pDBStmt = pAgent->m_xLogDBMgr.GetDBStmt(nIndex);
    if (!pDBStmt) {
        return;
    }

    // 对齐 IDA: lambda 捕获 stLog 副本和 pDBStmt
    auto func = [this, stLog, pDBStmt]() {
        ST_LOG_MONEY localLog = stLog;
        MoneyLogQuery(pDBStmt, localLog);
    };

    pAgent->m_xLogDBMgr.AddJob(static_cast<int>(nIndex), func);
}

int CLogDB::SystemLogQuery(XDBStmt* pDBStmt, ST_LOG_SYSTEM& stLog) {
    if (!pDBStmt) {
        return -1;
    }

    XDBBinder xDBBinder(pDBStmt);
    unsigned char szQuery[] = "{call SP_SYSTEM_LOG_INSERT(?,?,?,?, ?,?,?,?,?,?,?,?,?,? ) }";
    std::int64_t cbTID = -3;
    SQLRETURN sqlReturn = 0;

    xDBBinder.SetData(&stLog.nUAID, 1);
    xDBBinder.SetData(&stLog.nUCID, 1);
    xDBBinder.SetData(&stLog.sType, 1);
    xDBBinder.SetData(&stLog.nParam0, 1);
    xDBBinder.SetData(&stLog.nParam1, 1);
    xDBBinder.SetData(&stLog.nParam2, 1);
    xDBBinder.SetData(&stLog.nParam3, 1);
    xDBBinder.SetData(&stLog.nParam4, 1);
    xDBBinder.SetData(&stLog.nParam5, 1);
    xDBBinder.SetData(&stLog.nParam6, 1);
    xDBBinder.SetData(&stLog.nParam7, 1);
    xDBBinder.SetData(&stLog.nParam8, 1);
    xDBBinder.SetData(&stLog.nParam9, 1);
    xDBBinder.SetWString(stLog.szParam10, static_cast<std::uint16_t>(sizeof(stLog.szParam10) / sizeof(wchar_t)), &cbTID, 1);

    sqlReturn = xDBBinder.Execute(szQuery);
    if (sqlReturn != 0 && sqlReturn != 100) {
        xDBBinder.Close();
        return -1;
    }

    int nErrorCode = 0;
    if (sqlReturn == 0 && xDBBinder.Fetch() == 0) {
        xDBBinder.GetData(&nErrorCode);
    }
    xDBBinder.Close();
    return nErrorCode;
}

int CLogDB::LogQuery(XDBStmt* pDBStmt, ST_LOG_GAME& stLog) {
    if (!pDBStmt) {
        return -1;
    }

    XDBBinder xDBBinder(pDBStmt);
    unsigned char szQuery[] = "{call SP_LOG_GAME(?,?,?,?, ?,?,?,?,?,?,?, ?,?, ?,?,?, ?,?, ?,? ) }";
    std::int64_t cbTID = -3;
    SQLRETURN sqlReturn = 0;

    xDBBinder.SetData(&stLog._nUAID, 1);
    xDBBinder.SetData(&stLog._nUCID, 1);
    xDBBinder.SetData(&stLog._sMainType, 1);
    xDBBinder.SetData(&stLog._sSubType, 1);
    xDBBinder.SetData(&stLog.nParam0, 1);
    xDBBinder.SetData(&stLog.nParam1, 1);
    xDBBinder.SetData(&stLog.nParam2, 1);
    xDBBinder.SetData(&stLog.nParam3, 1);
    xDBBinder.SetData(&stLog.nParam4, 1);
    xDBBinder.SetData(&stLog.nParam5, 1);
    xDBBinder.SetData(&stLog.nParam6, 1);
    xDBBinder.SetWString(stLog.szComment, static_cast<std::uint16_t>(sizeof(stLog.szComment) / sizeof(wchar_t)), &cbTID, 1);
    xDBBinder.SetWString(stLog.szComment2, static_cast<std::uint16_t>(sizeof(stLog.szComment2) / sizeof(wchar_t)), &cbTID, 1);
    xDBBinder.SetData(&stLog.nParam7, 1);
    xDBBinder.SetData(&stLog.nParam8, 1);
    xDBBinder.SetData(&stLog.nParam9, 1);
    xDBBinder.SetWString(stLog.szParam10, static_cast<std::uint16_t>(sizeof(stLog.szParam10) / sizeof(wchar_t)), &cbTID, 1);
    xDBBinder.SetData(&stLog.nWorld_Idx, 1);
    xDBBinder.SetData(&stLog.nParam11, 1);
    xDBBinder.SetData(&stLog.nParam12, 1);

    sqlReturn = xDBBinder.Execute(szQuery);
    if (sqlReturn != 0 && sqlReturn != 100) {
        xDBBinder.Close();
        return -1;
    }

    int nErrorCode = 0;
    if (sqlReturn == 0 && xDBBinder.Fetch() == 0) {
        xDBBinder.GetData(&nErrorCode);
    }
    xDBBinder.Close();
    return nErrorCode;
}
int CLogDB::ChatLogQuery(XDBStmt* pDBStmt, int nUAID, int nUCID, std::int16_t sType,
                         std::int64_t nParam0, int nParam1, int nParam2, int nParam3,
                         int nParam4, std::int64_t nParam5, std::int64_t nParam6, wchar_t* szComment) {
    if (!pDBStmt) {
        return -1;
    }

    XDBBinder xDBBinder(pDBStmt);
    unsigned char szQuery[] = "{call SP_CHAT_LOG_GAME(?,?,?,?,?,?,?,?,?,?,?) }";
    std::int64_t cbTID = -3;
    SQLRETURN sqlReturn = 0;

    xDBBinder.SetData(&nUAID, 1);
    xDBBinder.SetData(&nUCID, 1);
    xDBBinder.SetData(&sType, 1);
    xDBBinder.SetData(&nParam0, 1);
    xDBBinder.SetData(&nParam1, 1);
    xDBBinder.SetData(&nParam2, 1);
    xDBBinder.SetData(&nParam3, 1);
    xDBBinder.SetData(&nParam4, 1);
    xDBBinder.SetData(&nParam5, 1);
    xDBBinder.SetData(&nParam6, 1);
    xDBBinder.SetWString(szComment, 0x101, &cbTID, 1);

    sqlReturn = xDBBinder.Execute(szQuery);
    if (sqlReturn != 0 && sqlReturn != 100) {
        xDBBinder.Close();
        return -1;
    }

    int nErrorCode = 0;
    if (sqlReturn == 0 && xDBBinder.Fetch() == 0) {
        xDBBinder.GetData(&nErrorCode);
    }
    xDBBinder.Close();
    return nErrorCode;
}
int CLogDB::CashLogQuery(XDBStmt* pDBStmt, int nUAID, int nUCID, std::int64_t nOrderNo,
                         int nParam1, int nParam2, int nParam3, int nParam4,
                         int nParam5, int nParam6, char* szComment) {
    if (!pDBStmt) {
        return -1;
    }

    XDBBinder xDBBinder(pDBStmt);
    unsigned char szQuery[] = "{call SP_CASH_ITEM_BUY_LOG(?,?,?,?,?,?,?,?,?,?) }";
    std::int64_t cbTID = -3;
    SQLRETURN sqlReturn = 0;

    // 对齐 IDA: nOrderNo 是第一个参数
    xDBBinder.SetData(&nOrderNo, 1);
    xDBBinder.SetData(&nUAID, 1);
    xDBBinder.SetData(&nUCID, 1);
    xDBBinder.SetData(&nParam1, 1);
    xDBBinder.SetData(&nParam2, 1);
    xDBBinder.SetData(&nParam3, 1);
    xDBBinder.SetString(szComment, 0x15, &cbTID, 1);
    xDBBinder.SetData(&nParam4, 1);
    xDBBinder.SetData(&nParam5, 1);
    xDBBinder.SetData(&nParam6, 1);

    sqlReturn = xDBBinder.Execute(szQuery);
    if (sqlReturn != 0 && sqlReturn != 100) {
        xDBBinder.Close();
        return -1;
    }

    int nErrorCode = 0;
    if (sqlReturn == 0 && xDBBinder.Fetch() == 0) {
        xDBBinder.GetData(&nErrorCode);
    }
    xDBBinder.Close();
    return nErrorCode;
}
int CLogDB::StatLogQuery(XDBStmt* pDBStmt, int nUAID, int nUCID,
                         float fParam0, float fParam1, float fParam2,
                         float fParam3, float fParam4, float fParam5, float fParam6) {
    if (!pDBStmt) {
        return -1;
    }

    XDBBinder xDBBinder(pDBStmt);
    unsigned char szQuery[] = "{call SP_STAT_LOG_GAME(?,?,?,?,?,?,?,?,?) }";
    SQLRETURN sqlReturn = 0;

    xDBBinder.SetData(&nUAID, 1);
    xDBBinder.SetData(&nUCID, 1);
    xDBBinder.SetData(&fParam0, 1);
    xDBBinder.SetData(&fParam1, 1);
    xDBBinder.SetData(&fParam2, 1);
    xDBBinder.SetData(&fParam3, 1);
    xDBBinder.SetData(&fParam4, 1);
    xDBBinder.SetData(&fParam5, 1);
    xDBBinder.SetData(&fParam6, 1);

    sqlReturn = xDBBinder.Execute(szQuery);
    if (sqlReturn != 0 && sqlReturn != 100) {
        xDBBinder.Close();
        return -1;
    }

    int nErrorCode = 0;
    if (sqlReturn == 0 && xDBBinder.Fetch() == 0) {
        xDBBinder.GetData(&nErrorCode);
    }
    xDBBinder.Close();
    return nErrorCode;
}
int CLogDB::ClientLogQuery(XDBStmt* pDBStmt, int nUAID, int nUCID,
                           std::uint8_t byType, char* szComment) {
    if (!pDBStmt) {
        return -1;
    }

    XDBBinder xDBBinder(pDBStmt);
    unsigned char szQuery[] = "{call SP_LOG_CLIENT_ERROR_INSERT( ?,?,?,? )}";
    std::int64_t cbTID = -3;
    SQLRETURN sqlReturn = 0;

    xDBBinder.SetData(&nUAID, 1);
    xDBBinder.SetData(&nUCID, 1);
    xDBBinder.SetData(&byType, 1);
    xDBBinder.SetString(szComment, 0x80, &cbTID, 1);

    sqlReturn = xDBBinder.Execute(szQuery);
    if (sqlReturn != 0 && sqlReturn != 100) {
        xDBBinder.Close();
        return -1;
    }

    int nErrorCode = 0;
    if (sqlReturn == 0 && xDBBinder.Fetch() == 0) {
        xDBBinder.GetData(&nErrorCode);
    }
    xDBBinder.Close();
    return nErrorCode;
}
int CLogDB::ConnectServerLogQuery(XDBStmt* pDBStmt, int nUCID, int nServerID, std::uint8_t byType) {
    if (!pDBStmt) {
        return -1;
    }

    XDBBinder xDBBinder(pDBStmt);
    SQLRETURN sqlReturn = 0;
    int nErrorCode = 0;

    // 对齐 IDA: Type == 0 调用 SP_CHARACTER_CONNECT_INFO_SAVE，Type == 1 调用 SP_CHARACTER_CONNECT_INFO_RESET
    if (byType == 0) {
        unsigned char szQuery[] = "{call SP_CHARACTER_CONNECT_INFO_SAVE( ?,? )}";
        xDBBinder.SetData(&nUCID, 1);
        xDBBinder.SetData(&nServerID, 1);
        sqlReturn = xDBBinder.Execute(szQuery);
    } else if (byType == 1) {
        unsigned char szQuery[] = "{call SP_CHARACTER_CONNECT_INFO_RESET( ? )}";
        xDBBinder.SetData(&nServerID, 1);
        sqlReturn = xDBBinder.Execute(szQuery);
    } else {
        return 0;
    }

    if (sqlReturn != 0 && sqlReturn != 100) {
        xDBBinder.Close();
        return -1;
    }

    xDBBinder.Close();
    return nErrorCode;
}
int CLogDB::ClassEventLogQuery(XDBStmt* pDBStmt, std::uint32_t dwUAID, std::uint32_t dwUCID,
                               std::uint8_t byType, int nReward) {
    if (!pDBStmt) {
        return -1;
    }

    XDBBinder xDBBinder(pDBStmt);
    unsigned char szQuery[] = "{call SP_EVENT_NEW_CHARACTER_UPDATE( ?,?,?,? )}";
    SQLRETURN sqlReturn = 0;

    xDBBinder.SetData(&dwUAID, 1);
    xDBBinder.SetData(&dwUCID, 1);
    xDBBinder.SetData(&byType, 1);
    xDBBinder.SetData(&nReward, 1);

    sqlReturn = xDBBinder.Execute(szQuery);
    if (sqlReturn != 0 && sqlReturn != 100) {
        xDBBinder.Close();
        return -1;
    }

    xDBBinder.Close();
    return 0;
}
int CLogDB::SGLogQuery(XDBStmt*, ST_LOG_GAME&) { return 0; }
int CLogDB::SGTokenQuery(XDBStmt* pDBStmt, ST_SG_AUTH_INFO& stLog) {
    if (!pDBStmt) {
        return -1;
    }

    XDBBinder xDBBinder(pDBStmt);
    unsigned char szQuery[] = "{call SP_ACCOUNT_TOKEN_LOG_INSERT( ?,?,? )}";
    std::int64_t cbTID = -3;
    SQLRETURN sqlReturn = 0;

    xDBBinder.SetData(&stLog.nUAID, 1);
    xDBBinder.SetString(stLog.szToken, 0x401, &cbTID, 1);
    xDBBinder.SetString(stLog.szRefreshToken, 0x401, &cbTID, 1);

    sqlReturn = xDBBinder.Execute(szQuery);
    if (sqlReturn != 0 && sqlReturn != 100) {
        xDBBinder.Close();
        return -1;
    }

    xDBBinder.Close();
    return 0;
}
int CLogDB::TextLogQuery(XDBStmt* pDBStmt, ST_LOG_TEXT& stLog) {
    if (!pDBStmt) {
        return -1;
    }

    XDBBinder xDBBinder(pDBStmt);
    unsigned char szQuery[] = "{call SP_LOG_TEXT_INSERT(?,?,?,?,?,?,?) }";
    std::int64_t cbTID = -3;
    SQLRETURN sqlReturn = 0;

    xDBBinder.SetData(&stLog.dwUAID, 1);
    xDBBinder.SetData(&stLog.dwUCID, 1);
    xDBBinder.SetData(&stLog.shMainType, 1);
    xDBBinder.SetData(&stLog.shSubType, 1);
    xDBBinder.SetData(&stLog.nMapID, 1);
    xDBBinder.SetData(&stLog.nInstanceID, 1);
    xDBBinder.SetString(stLog.szMsg, static_cast<std::uint16_t>(sizeof(stLog.szMsg)), &cbTID, 1);

    sqlReturn = xDBBinder.Execute(szQuery);
    if (sqlReturn != 0 && sqlReturn != 100) {
        xDBBinder.Close();
        return -1;
    }

    int nErrorCode = 0;
    if (sqlReturn == 0 && xDBBinder.Fetch() == 0) {
        xDBBinder.GetData(&nErrorCode);
    }
    xDBBinder.Close();
    return nErrorCode;
}
int CLogDB::MoneyLogQuery(XDBStmt* pDBStmt, ST_LOG_MONEY& stLog) {
    if (!pDBStmt) {
        return -1;
    }

    XDBBinder xDBBinder(pDBStmt);
    unsigned char szQuery[] = "{ call SP_CHARACTER_MONEY_LOG_INSERT( ?, ?, ? ) }";
    SQLRETURN sqlReturn = 0;

    xDBBinder.SetData(&stLog.dwUCID, 1);
    xDBBinder.SetData(&stLog.biIncMoney, 1);
    xDBBinder.SetData(&stLog.biDescMoney, 1);

    sqlReturn = xDBBinder.Execute(szQuery);
    if (sqlReturn != 0 && sqlReturn != 100) {
        xDBBinder.Close();
        return -1;
    }

    int nErrorCode = 0;
    if (sqlReturn == 0 && xDBBinder.Fetch() == 0) {
        xDBBinder.GetData(&nErrorCode);
    }
    xDBBinder.Close();
    return nErrorCode;
}
