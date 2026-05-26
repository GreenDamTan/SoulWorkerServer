#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <list>
#include <memory>
#include <mutex>
#include <new>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
#include <sql.h>
#include <sqlext.h>
#else
using SQLCHAR = unsigned char;
using SQLSMALLINT = std::int16_t;
using SQLUSMALLINT = std::uint16_t;
using SQLINTEGER = std::int32_t;
using SQLRETURN = std::int16_t;
using SQLLEN = std::int64_t;
using SQLULEN = std::uint64_t;
using SQLHANDLE = void*;
using SQLHENV = void*;
using SQLHDBC = void*;
using SQLHSTMT = void*;

inline constexpr SQLHANDLE SQL_NULL_HANDLE = nullptr;
inline constexpr SQLRETURN SQL_SUCCESS = 0;
inline constexpr SQLRETURN SQL_SUCCESS_WITH_INFO = 1;
inline constexpr SQLRETURN SQL_NO_DATA = 100;
inline constexpr SQLRETURN SQL_ERROR = -1;
inline constexpr SQLSMALLINT SQL_HANDLE_ENV = 1;
inline constexpr SQLSMALLINT SQL_HANDLE_DBC = 2;
inline constexpr SQLSMALLINT SQL_HANDLE_STMT = 3;
inline constexpr SQLSMALLINT SQL_COMMIT = 0;
inline constexpr SQLSMALLINT SQL_ROLLBACK = 1;
inline constexpr SQLINTEGER SQL_ATTR_ODBC_VERSION = 200;
inline constexpr SQLINTEGER SQL_OV_ODBC3 = 3;
inline constexpr SQLUSMALLINT SQL_DRIVER_NOPROMPT = 0;
inline constexpr SQLLEN SQL_NTS = -3;
#endif

#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"



#ifdef _WIN32
using GreenDamTan_Overlapped = OVERLAPPED;
#else
struct GreenDamTan_Overlapped {
    std::uintptr_t Internal = 0;
    std::uintptr_t InternalHigh = 0;
    union {
        struct {
            std::uint32_t Offset;
            std::uint32_t OffsetHigh;
        };
        void* Pointer = nullptr;
    };
    void* hEvent = nullptr;
};
#endif

inline void* GreenDamTan_GlobalAllocZero(std::size_t size) {
#ifdef _WIN32
    return ::GlobalAlloc(0x40u, size);
#else
    return std::calloc(1, size);
#endif
}

inline void GreenDamTan_GlobalFreeRaw(void* ptr) {
    if (!ptr) {
        return;
    }

#ifdef _WIN32
    ::GlobalFree(ptr);
#else
    std::free(ptr);
#endif
}

inline void GreenDamTan_DBGetLastError(SQLSMALLINT handleType,
                                       SQLHANDLE* handle,
                                       char* buffer,
                                       std::size_t bufferSize = 512) {
    if (!buffer || bufferSize == 0) {
        return;
    }

    buffer[0] = '\0';

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
    if (!handle || !*handle) {
        return;
    }

    std::array<SQLCHAR, 6> sqlState{};
    std::array<SQLCHAR, 512> message{};
    SQLINTEGER nativeError = 0;
    SQLSMALLINT textLength = 0;
    const SQLRETURN result = SQLGetDiagRecA(handleType,
                                            *handle,
                                            1,
                                            sqlState.data(),
                                            &nativeError,
                                            message.data(),
                                            static_cast<SQLSMALLINT>(message.size()),
                                            &textLength);
    if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO) {
        std::snprintf(buffer, bufferSize, "%s", reinterpret_cast<const char*>(message.data()));
    }
#else
    (void)handleType;
    (void)handle;
#endif
}

inline std::mutex& GreenDamTan_DBStubRowMutex() {
    static std::mutex s_mutex;
    return s_mutex;
}

inline std::unordered_map<SQLHSTMT, int>& GreenDamTan_DBStubRowCounts() {
    static std::unordered_map<SQLHSTMT, int> s_remainingRows;
    return s_remainingRows;
}

inline SQLRETURN GreenDamTan_DBStubFetch(SQLHSTMT statement) {
    std::lock_guard<std::mutex> lock(GreenDamTan_DBStubRowMutex());
    auto& rowsRef = GreenDamTan_DBStubRowCounts();
    auto it = rowsRef.find(statement);
    if (it == rowsRef.end()) {
        rowsRef.emplace(statement, 1);
        return SQL_SUCCESS;
    }

    int& remaining = it->second;
    if (remaining > 0) {
        --remaining;
        return SQL_NO_DATA;
    }
    return SQL_NO_DATA;
}

inline void GreenDamTan_DBStubReset(SQLHSTMT statement) {
    std::lock_guard<std::mutex> lock(GreenDamTan_DBStubRowMutex());
    GreenDamTan_DBStubRowCounts().erase(statement);
}

class XDBEnv;
class XDBConnect;

inline std::atomic<int>& GreenDamTan_XDBConnectIndex() {
    static std::atomic<int> s_index{0};
    return s_index;
}

class XDBError {
public:
    virtual ~XDBError() = default;

    void GetLastError(SQLSMALLINT handleType, SQLHANDLE* handle, char* buffer) {
        GreenDamTan_DBGetLastError(handleType, handle, buffer, sizeof(m_szErrorMsg));
    }

    SQLSMALLINT m_sHandleType = 0;
    SQLHANDLE* m_pHandle = nullptr;
    char m_szErrorMsg[512] = {};
};

class XDBEnv : public XDBError {
public:
    XDBEnv() {
        m_sHandleType = SQL_HANDLE_ENV;
        m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBEnvironment);
    }

    std::int16_t Init() {
#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hDBEnvironment) != SQL_SUCCESS) {
            m_hDBEnvironment = SQL_NULL_HANDLE;
            m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBEnvironment);
            return SQL_ERROR;
        }
        if (SQLSetEnvAttr(m_hDBEnvironment,
                          SQL_ATTR_ODBC_VERSION,
                          reinterpret_cast<void*>(static_cast<std::intptr_t>(SQL_OV_ODBC3)),
                          0) != SQL_SUCCESS) {
            SQLFreeHandle(SQL_HANDLE_ENV, m_hDBEnvironment);
            m_hDBEnvironment = SQL_NULL_HANDLE;
            m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBEnvironment);
            return SQL_ERROR;
        }
#else
        m_hDBEnvironment = reinterpret_cast<SQLHENV>(this);
#endif
        m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBEnvironment);
        return SQL_SUCCESS;
    }

    ~XDBEnv() override {
#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        if (m_hDBEnvironment) {
            SQLFreeHandle(SQL_HANDLE_ENV, m_hDBEnvironment);
        }
#endif
        m_hDBEnvironment = SQL_NULL_HANDLE;
        m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBEnvironment);
    }

    SQLHENV m_hDBEnvironment = SQL_NULL_HANDLE;
};

class XDBConnect : public XDBError {
public:
    XDBConnect() {
        m_nThisIndex = -1;
        m_sHandleType = SQL_HANDLE_DBC;
        m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBConnection);
    }

    std::int16_t Init(XDBEnv* pDBEnv) {
        m_pDBEnv = pDBEnv;
        m_bConnected = false;
        m_strDNS.clear();

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        if (!pDBEnv || !pDBEnv->m_hDBEnvironment ||
            SQLAllocHandle(SQL_HANDLE_DBC, pDBEnv->m_hDBEnvironment, &m_hDBConnection) != SQL_SUCCESS) {
            m_hDBConnection = SQL_NULL_HANDLE;
            m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBConnection);
            return SQL_ERROR;
        }
#else
        m_hDBConnection = reinterpret_cast<SQLHDBC>(this);
#endif
        m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBConnection);
        return pDBEnv ? SQL_SUCCESS : SQL_ERROR;
    }

    std::int64_t Connect(char* szDNS) {
        std::memset(m_szErrorMsg, 0, sizeof(m_szErrorMsg));
        m_strDNS = szDNS ? szDNS : "";

        if (GetDNS().empty()) {
            LogHelper::LogLegacy("Try Connect : %s", "");
            std::snprintf(m_szErrorMsg, sizeof(m_szErrorMsg), "%s", "Invalid DNS");
            LogHelper::LogLegacy("Connect Error:(%s)", m_szErrorMsg);
            DisConnect();
            return SQL_ERROR;
        }

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        std::array<SQLCHAR, 512> connStrOut{};
        SQLSMALLINT outLen = 0;
        const SQLRETURN result = SQLDriverConnectA(m_hDBConnection,
                                                   nullptr,
                                                   reinterpret_cast<SQLCHAR*>(const_cast<char*>(GetDNS().c_str())),
                                                   SQL_NTS,
                                                   connStrOut.data(),
                                                   static_cast<SQLSMALLINT>(connStrOut.size()),
                                                   &outLen,
                                                   SQL_DRIVER_NOPROMPT);
        if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO) {
            SQLGetInfoA(m_hDBConnection, 47, connStrOut.data(), 512, &outLen);
            std::snprintf(m_szErrorMsg, sizeof(m_szErrorMsg), "%s", reinterpret_cast<const char*>(connStrOut.data()));
            {
                m_bConnected = true;
            }
            m_nThisIndex = ++GreenDamTan_XDBConnectIndex();
            LogHelper::LogLegacy("Connect Success (%d)(%s)", m_nThisIndex, m_szErrorMsg);
            return SQL_SUCCESS;
        }
#else
        {
            m_bConnected = true;
        }
        std::snprintf(m_szErrorMsg, sizeof(m_szErrorMsg), "%s", "dbo");
        m_nThisIndex = ++GreenDamTan_XDBConnectIndex();
        LogHelper::LogLegacy("Connect Success (%d)(%s)", m_nThisIndex, m_szErrorMsg);
        return SQL_SUCCESS;
#endif

        LogHelper::LogLegacy("Try Connect : %s", GetDNS().c_str());
        LogHelper::LogLegacy("Connect Error:(%s)", m_szErrorMsg);
        DisConnect();
        return SQL_ERROR;
    }

    std::int64_t Clear() {
        SQLRETURN disconnectResult = SQL_SUCCESS;
        m_bConnected = false;

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        if (m_hDBConnection) {
            disconnectResult = SQLDisconnect(m_hDBConnection);
            m_szErrorMsg[0] = '\0';
            if (disconnectResult != SQL_SUCCESS && disconnectResult != SQL_SUCCESS_WITH_INFO) {
                GetLastError(m_sHandleType, m_pHandle, m_szErrorMsg);
                return disconnectResult;
            }
        }

        if (!m_hDBConnection) {
            return SQL_ERROR;
        }

        const SQLRETURN freeResult = SQLFreeConnect(m_hDBConnection);
        m_szErrorMsg[0] = '\0';
        if (freeResult != SQL_SUCCESS && freeResult != SQL_SUCCESS_WITH_INFO) {
            GetLastError(m_sHandleType, m_pHandle, m_szErrorMsg);
        }
#else
        if (!m_hDBConnection) {
            return SQL_ERROR;
        }

        const SQLRETURN freeResult = SQL_SUCCESS;
#endif

        m_hDBConnection = SQL_NULL_HANDLE;
        m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBConnection);
        return freeResult;
    }

    std::int64_t DisConnect() {
        m_bConnected = false;
        if (!m_hDBConnection) {
            return SQL_SUCCESS;
        }

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        const SQLRETURN result = SQLDisconnect(m_hDBConnection);
        m_szErrorMsg[0] = '\0';
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            GetLastError(m_sHandleType, m_pHandle, m_szErrorMsg);
        }
#else
        const SQLRETURN result = SQL_SUCCESS;
#endif
        m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBConnection);
        return result;
    }

    ~XDBConnect() override {
        Clear();
        m_sHandleType = 0;
        m_pHandle = nullptr;
        m_pDBEnv = nullptr;
        m_strDNS.clear();
        m_bConnected = false;
    }

    bool IsConnected() const { return m_bConnected; }

    SQLHANDLE* GetHDBC() { return reinterpret_cast<SQLHANDLE*>(&m_hDBConnection); }

    std::int16_t SetEndTran(std::int16_t completionType) {
#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        if (!m_hDBConnection) {
            return SQL_ERROR;
        }
        return SQLTransact(SQL_NULL_HANDLE,
                           m_hDBConnection,
                           completionType == SQL_ROLLBACK ? SQL_ROLLBACK : SQL_COMMIT);
#else
        (void)completionType;
        return IsConnected() ? SQL_SUCCESS : SQL_ERROR;
#endif
    }

    std::string GetDNS() const { return m_strDNS; }

    int m_nThisIndex = 0;
    SQLHDBC m_hDBConnection = SQL_NULL_HANDLE;
    void* m_AutoCommit = nullptr;
    XDBEnv* m_pDBEnv = nullptr;
    std::string m_strDNS;
    bool m_bConnected = false;
};

class XDBStmt : public XDBError {
public:
    XDBStmt() {
        m_sHandleType = SQL_HANDLE_STMT;
        m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBStatement);
    }

    std::int64_t Init(XDBConnect* pConn, void* pTimeOutSec, void* pAsync) {
        (void)pTimeOutSec;
        (void)pAsync;

        if (m_hDBStatement || !pConn || !pConn->IsConnected()) {
            return SQL_ERROR;
        }

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        const SQLRETURN allocResult = SQLAllocStmt(pConn->m_hDBConnection, &m_hDBStatement);
        m_szErrorMsg[0] = '\0';
        if (allocResult != SQL_SUCCESS && allocResult != SQL_SUCCESS_WITH_INFO) {
            GetLastError(m_sHandleType, m_pHandle, m_szErrorMsg);
            m_hDBStatement = SQL_NULL_HANDLE;
            return SQL_ERROR;
        }

        const SQLRETURN timeoutResult = SQLSetStmtAttr(m_hDBStatement, 4, nullptr, SQL_NTS);
        m_szErrorMsg[0] = '\0';
        if (timeoutResult != SQL_SUCCESS && timeoutResult != SQL_SUCCESS_WITH_INFO) {
            GetLastError(m_sHandleType, m_pHandle, m_szErrorMsg);
            SQLFreeHandle(SQL_HANDLE_STMT, m_hDBStatement);
            m_hDBStatement = SQL_NULL_HANDLE;
            return SQL_ERROR;
        }

        const SQLRETURN asyncResult = SQLSetStmtAttr(m_hDBStatement, 0, nullptr, -6);
        m_szErrorMsg[0] = '\0';
        if (asyncResult != SQL_SUCCESS && asyncResult != SQL_SUCCESS_WITH_INFO) {
            GetLastError(m_sHandleType, m_pHandle, m_szErrorMsg);
            SQLFreeHandle(SQL_HANDLE_STMT, m_hDBStatement);
            m_hDBStatement = SQL_NULL_HANDLE;
            return SQL_ERROR;
        }
#else
        m_hDBStatement = reinterpret_cast<SQLHSTMT>(this);
        m_szErrorMsg[0] = '\0';
#endif

        m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBStatement);
        return SQL_SUCCESS;
    }

    std::int64_t SQLBindParameter(std::uint16_t ipar,
                                  std::int16_t fParamType,
                                  std::int16_t fCType,
                                  std::int16_t fSqlType,
                                  std::uint64_t cbColDef,
                                  std::int16_t ibScale,
                                  void* rgbValue,
                                  std::int64_t cbValueMax,
                                  std::int64_t* pcbValue) {
        if (!m_hDBStatement) {
            return SQL_ERROR;
        }

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        const SQLRETURN result = ::SQLBindParameter(m_hDBStatement,
                                                    ipar,
                                                    fParamType,
                                                    fCType,
                                                    fSqlType,
                                                    cbColDef,
                                                    ibScale,
                                                    rgbValue,
                                                    cbValueMax,
                                                    pcbValue);
        if (result == SQL_ERROR) {
            GetLastError(m_sHandleType, m_pHandle, m_szErrorMsg);
        } else {
            m_szErrorMsg[0] = '\0';
        }
        return result;
#else
        (void)ipar;
        (void)fParamType;
        (void)fCType;
        (void)fSqlType;
        (void)cbColDef;
        (void)ibScale;
        (void)rgbValue;

        if (pcbValue && *pcbValue == 0 && cbValueMax != 0) {
            *pcbValue = cbValueMax;
        }
        m_szErrorMsg[0] = '\0';
        return SQL_SUCCESS;
#endif
    }

    std::int64_t SQLGetData(std::uint16_t ColumnNumber,
                            std::int16_t TargetType,
                            void* TargetValue,
                            std::int64_t BufferLength,
                            std::int64_t* StrLen_or_Ind) {
        if (!m_hDBStatement) {
            return SQL_ERROR;
        }

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        const SQLRETURN result = ::SQLGetData(
            m_hDBStatement, ColumnNumber, TargetType, TargetValue, BufferLength, StrLen_or_Ind);
        if (result == SQL_ERROR) {
            GetLastError(m_sHandleType, m_pHandle, m_szErrorMsg);
        } else {
            m_szErrorMsg[0] = '\0';
        }
        return result;
#else
        (void)ColumnNumber;
        (void)TargetType;
        if (TargetValue && BufferLength > 0) {
            std::memset(TargetValue, 0, static_cast<std::size_t>(BufferLength));
        }
        if (StrLen_or_Ind) {
            *StrLen_or_Ind = 0;
        }
        m_szErrorMsg[0] = '\0';
        return SQL_SUCCESS;
#endif
    }

    std::int64_t Clear() {
#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        if (m_hDBStatement) {
            SQLFreeHandle(SQL_HANDLE_STMT, m_hDBStatement);
        }
#endif
        m_hDBStatement = SQL_NULL_HANDLE;
        m_pHandle = reinterpret_cast<SQLHANDLE*>(&m_hDBStatement);
        m_szErrorMsg[0] = '\0';
        return SQL_SUCCESS;
    }

    ~XDBStmt() override { Clear(); }

    SQLHSTMT m_hDBStatement = SQL_NULL_HANDLE;
};

class XDBBinder {
public:
    explicit XDBBinder(XDBStmt* pDBStmt)
        : m_pDBStmt(pDBStmt),
          m_sInParam(1),
          m_sOutParam(1) {}

    ~XDBBinder() = default;

    /// @brief 设置宽字符串参数
    /// @param buffer 宽字符串缓冲区
    /// @param bufferSize 缓冲区大小（字符数）
    /// @param pcbValue 指示符/长度指针
    /// @param direction 1=输入，2=输出，3=输入输出
    std::int64_t SetWString(wchar_t* buffer, std::uint16_t bufferSize, std::int64_t* pcbValue, int direction = 1) {
        if (!m_pDBStmt) {
            return SQL_ERROR;
        }

        const std::int16_t fParamType = (direction == 1) ? 1 : ((direction == 2) ? 2 : 3);  // SQL_PARAM_INPUT/OUTPUT
        const std::int16_t fCType = -8;   // SQL_C_WCHAR
        const std::int16_t fSqlType = -9; // SQL_WVARCHAR

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        const SQLRETURN result = ::SQLBindParameter(
            m_pDBStmt->m_hDBStatement,
            m_sInParam++,
            fParamType,
            fCType,
            fSqlType,
            bufferSize,
            0,
            buffer,
            bufferSize * sizeof(wchar_t),
            reinterpret_cast<SQLLEN*>(pcbValue));
        if (result == SQL_ERROR) {
            m_pDBStmt->GetLastError(m_pDBStmt->m_sHandleType, m_pDBStmt->m_pHandle, m_pDBStmt->m_szErrorMsg);
        }
        return result;
#else
        (void)buffer;
        (void)bufferSize;
        (void)direction;
        if (pcbValue && *pcbValue == 0) {
            *pcbValue = -3;  // SQL_NTS
        }
        m_sInParam++;
        return SQL_SUCCESS;
#endif
    }

    /// @brief 设置数据参数（整数类型）
    /// @param data 数据指针
    /// @param size 数据大小（字节）
    /// @param direction 1=输入，2=输出，3=输入输出
    std::int64_t SetData(void* data, std::uint16_t size, int direction = 1) {
        if (!m_pDBStmt) {
            return SQL_ERROR;
        }

        const std::int16_t fParamType = (direction == 1) ? 1 : ((direction == 2) ? 2 : 3);  // SQL_PARAM_INPUT/OUTPUT

        // 根据大小确定 SQL 类型
        std::int16_t fSqlType = 4;  // SQL_INTEGER
        std::int16_t fCType = 4;    // SQL_C_LONG

        if (size == 1) {
            fSqlType = -6;  // SQL_TINYINT
            fCType = -2;    // SQL_C_TINYINT
        } else if (size == 2) {
            fSqlType = 5;   // SQL_SMALLINT
            fCType = 5;     // SQL_C_SHORT
        } else if (size == 8) {
            fSqlType = -5;  // SQL_BIGINT
            fCType = -25;   // SQL_C_SBIGINT
        }

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        const SQLRETURN result = ::SQLBindParameter(
            m_pDBStmt->m_hDBStatement,
            m_sInParam++,
            fParamType,
            fCType,
            fSqlType,
            size,
            0,
            data,
            size,
            nullptr);
        if (result == SQL_ERROR) {
            m_pDBStmt->GetLastError(m_pDBStmt->m_sHandleType, m_pDBStmt->m_pHandle, m_pDBStmt->m_szErrorMsg);
        }
        return result;
#else
        (void)data;
        (void)direction;
        m_sInParam++;
        return SQL_SUCCESS;
#endif
    }

    std::int64_t Execute(unsigned char* szQuery) {
        if (!m_pDBStmt || !m_pDBStmt->m_hDBStatement || !szQuery || !*szQuery) {
            return SQL_ERROR;
        }

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        const SQLRETURN result = SQLExecDirect(m_pDBStmt->m_hDBStatement, szQuery, SQL_NTS);
        if (result == SQL_ERROR) {
            m_pDBStmt->GetLastError(m_pDBStmt->m_sHandleType, m_pDBStmt->m_pHandle, m_pDBStmt->m_szErrorMsg);
        } else {
            m_pDBStmt->m_szErrorMsg[0] = '\0';
        }
#else
        m_stubQuery = reinterpret_cast<const char*>(szQuery);
        GreenDamTan_DBStubReset(m_pDBStmt->m_hDBStatement);
        const SQLRETURN result = SQL_SUCCESS;
        m_pDBStmt->m_szErrorMsg[0] = '\0';
#endif

        m_sOutParam = 1;
        return result;
    }

    std::int64_t Fetch() {
        m_sOutParam = 1;
        if (!m_pDBStmt || !m_pDBStmt->m_hDBStatement) {
            return SQL_ERROR;
        }

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        return SQLFetch(m_pDBStmt->m_hDBStatement);
#else
        return GreenDamTan_DBStubFetch(m_pDBStmt->m_hDBStatement);
#endif
    }

    std::int64_t Close() {
        if (!m_pDBStmt || !m_pDBStmt->m_hDBStatement) {
            m_sInParam = 1;
            m_sOutParam = 1;
            return SQL_SUCCESS;
        }

        m_sInParam = 1;
        m_sOutParam = 1;

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        while (SQLMoreResults(m_pDBStmt->m_hDBStatement) == SQL_SUCCESS) {
        }
        SQLCloseCursor(m_pDBStmt->m_hDBStatement);
#else
        GreenDamTan_DBStubReset(m_pDBStmt->m_hDBStatement);
        m_stubQuery.clear();
#endif
        return SQL_SUCCESS;
    }

    /// @brief 获取整数数据
    /// @param pData 数据指针
    std::int64_t GetData(void* pData) {
        if (!m_pDBStmt || !m_pDBStmt->m_hDBStatement) {
            return SQL_ERROR;
        }

        // 默认获取 4 字节整数
        return m_pDBStmt->SQLGetData(m_sOutParam++, 4, pData, 4, nullptr);
    }

    /// @brief 获取字符串数据
    /// @param buffer 字符串缓冲区
    /// @param bufferSize 缓冲区大小
    std::int64_t GetString(char* buffer, std::uint16_t bufferSize) {
        if (!m_pDBStmt || !m_pDBStmt->m_hDBStatement || !buffer || bufferSize == 0) {
            return SQL_ERROR;
        }

        // SQL_C_CHAR = 1
        return m_pDBStmt->SQLGetData(m_sOutParam++, 1, buffer, bufferSize, nullptr);
    }

    /// @brief 获取宽字符串数据
    /// @param buffer 宽字符串缓冲区
    /// @param bufferSize 缓冲区大小（字符数）
    std::int64_t GetWString(wchar_t* buffer, std::uint16_t bufferSize) {
        if (!m_pDBStmt || !m_pDBStmt->m_hDBStatement || !buffer || bufferSize == 0) {
            return SQL_ERROR;
        }

        // SQL_C_WCHAR = -8
        return m_pDBStmt->SQLGetData(m_sOutParam++, -8, buffer, bufferSize * sizeof(wchar_t), nullptr);
    }

    /// @brief 获取宽字符串数据（带输出长度）
    /// @param buffer 宽字符串缓冲区
    /// @param bufferSize 缓冲区大小（字符数）
    /// @param outLen 输出实际长度（未使用，保持接口兼容）
    std::int64_t GetWString(wchar_t* buffer, std::uint16_t bufferSize, short& outLen) {
        outLen = 0;  // 暂不返回实际长度
        return GetWString(buffer, bufferSize);
    }

    /// @brief 获取二进制数据
    /// @param buffer 数据缓冲区
    /// @param bufferSize 缓冲区大小（字节数）
    std::int64_t GetBinary(void* buffer, std::uint16_t bufferSize) {
        if (!m_pDBStmt || !m_pDBStmt->m_hDBStatement || !buffer || bufferSize == 0) {
            return SQL_ERROR;
        }

        // SQL_C_BINARY = -2
        return m_pDBStmt->SQLGetData(m_sOutParam++, -2, buffer, bufferSize, nullptr);
    }

    /// @brief 设置字符串参数
    /// @param buffer 字符串缓冲区
    /// @param bufferSize 缓冲区大小
    /// @param pcbValue 指示符/长度指针
    /// @param direction 1=输入，2=输出，3=输入输出
    std::int64_t SetString(char* buffer, std::uint16_t bufferSize, std::int64_t* pcbValue, int direction = 1) {
        if (!m_pDBStmt) {
            return SQL_ERROR;
        }

        const std::int16_t fParamType = (direction == 1) ? 1 : ((direction == 2) ? 2 : 3);  // SQL_PARAM_INPUT/OUTPUT
        const std::int16_t fCType = 1;    // SQL_C_CHAR
        const std::int16_t fSqlType = 12; // SQL_VARCHAR

#if defined(GREENDAMTAN_HAS_NATIVE_ODBC)
        const SQLRETURN result = ::SQLBindParameter(
            m_pDBStmt->m_hDBStatement,
            m_sInParam++,
            fParamType,
            fCType,
            fSqlType,
            bufferSize,
            0,
            buffer,
            bufferSize,
            reinterpret_cast<SQLLEN*>(pcbValue));
        if (result == SQL_ERROR) {
            m_pDBStmt->GetLastError(m_pDBStmt->m_sHandleType, m_pDBStmt->m_pHandle, m_pDBStmt->m_szErrorMsg);
        }
        return result;
#else
        (void)buffer;
        (void)bufferSize;
        (void)direction;
        if (pcbValue && *pcbValue == 0) {
            *pcbValue = -3;  // SQL_NTS
        }
        m_sInParam++;
        return SQL_SUCCESS;
#endif
    }

    XDBStmt* m_pDBStmt = nullptr;
    std::uint16_t m_sInParam = 1;
    std::uint16_t m_sOutParam = 1;

private:
    std::string m_stubQuery;
};

template <typename TObject>
class GreenDamTan_TXPool {
public:
    struct IXCreator {
        virtual ~IXCreator() = default;
        virtual TObject* Create() = 0;
    };

    struct XCreator : IXCreator {};

    void Init(int nMaxCount) {
        m_xLock.Init();
        m_Queue = {};
        m_List.clear();
        m_nCurMaxSize = (std::max)(0, nMaxCount);
        m_bThreadSafe = true;
    }

    TObject* Pop() {
        CSimpleLock::Owner lock(&m_xLock);
        if (m_Queue.empty()) {
            return nullptr;
        }
        TObject* value = m_Queue.front();
        m_Queue.pop();
        return value;
    }

    void Push(TObject* value) {
        if (!value) {
            return;
        }
        CSimpleLock::Owner lock(&m_xLock);
        m_Queue.push(value);
    }

    bool Add(IXCreator* pCreator) {
        CSimpleLock::Owner lock(&m_xLock);
        if (!pCreator) {
            return false;
        }
        if (m_nCurMaxSize > 0 && static_cast<int>(m_List.size()) >= m_nCurMaxSize) {
            return true;
        }
        TObject* created = pCreator->Create();
        if (!created) {
            return false;
        }
        m_List.push_back(created);
        m_Queue.push(created);
        return false;
    }

    void Seed(TObject* value) {
        if (!value) {
            return;
        }
        CSimpleLock::Owner lock(&m_xLock);
        m_List.push_back(value);
        m_Queue.push(value);
    }

    int GetCurSize() {
        CSimpleLock::Owner lock(&m_xLock);
        return static_cast<int>(m_Queue.size());
    }

    int GetFullSize() {
        CSimpleLock::Owner lock(&m_xLock);
        return static_cast<int>(m_List.size());
    }

    virtual ~GreenDamTan_TXPool() {
        for (TObject* value : m_List) {
            delete value;
        }
        m_List.clear();
        m_Queue = {};
    }

    CSimpleLock m_xLock;
    std::queue<TObject*> m_Queue;
    std::list<TObject*> m_List;
    bool m_bThreadSafe = false;
    int m_nCurMaxSize = 0;

};

class XDBManager {
public:
    class XDBCreator : public GreenDamTan_TXPool<XDBConnect>::XCreator {
    public:
        XDBCreator(unsigned char* szDNS, XDBEnv* pDBEnv)
            : m_szDNS(szDNS),
              m_pDBEnv(pDBEnv) {}

        XDBConnect* Create() override {
            auto connection = std::make_unique<XDBConnect>();
            if (connection->Init(m_pDBEnv) != SQL_SUCCESS ||
                connection->Connect(reinterpret_cast<char*>(m_szDNS)) != SQL_SUCCESS) {
                return nullptr;
            }
            return connection.release();
        }

        unsigned char* m_szDNS = nullptr;
        XDBEnv* m_pDBEnv = nullptr;
    };

    class XDBDeletor : public GreenDamTan_TXPool<XDBConnect>::IXCreator {
    public:
        XDBConnect* Create() override { return nullptr; }
        virtual void Delete(XDBConnect* pDBConnect) {
            delete pDBConnect;
        }
    };

    virtual ~XDBManager() {
        delete m_pDBConnectCreator;
        m_pDBConnectCreator = nullptr;
    }

    bool Init(unsigned char* szDNS, int nMaxConnectCount) {
        delete m_pDBConnectCreator;
        m_pDBConnectCreator = nullptr;

        m_pDBEnv = new XDBEnv();
        if (!m_pDBEnv || m_pDBEnv->Init() != SQL_SUCCESS) {
            return false;
        }

        m_pDBConnectCreator = new XDBCreator(szDNS, m_pDBEnv);
        m_nMaxConnectCount = nMaxConnectCount;
        m_DBConnectPool.Init(m_nMaxConnectCount);
        XDBConnect* initialConnection = m_pDBConnectCreator ? m_pDBConnectCreator->Create() : nullptr;
        if (!initialConnection) {
            return false;
        }
        m_DBConnectPool.Seed(initialConnection);
        return true;
    }

    XDBConnect* GetDBConnect() {
        if (m_DBConnectPool.m_Queue.size() != 0) {
            return m_DBConnectPool.Pop();
        }

        if (m_DBConnectPool.Add(m_pDBConnectCreator)) {
            LogHelper::LogLegacy("]] m_DBConnectPool.Add Max Size[%d]",
                                 static_cast<int>(m_DBConnectPool.m_List.size()));
        } else {
            LogHelper::LogLegacy("]] Add Cannot Connect DB)");
        }
        return nullptr;
    }

    void CollectDBConnect(XDBConnect* pDBConnect) {
        m_DBConnectPool.Push(pDBConnect);
    }

    GreenDamTan_TXPool<XDBConnect> m_DBConnectPool;
    XDBEnv* m_pDBEnv = nullptr;
    int m_nMaxConnectCount = 0;
    XDBCreator* m_pDBConnectCreator = nullptr;
};

struct PerIoContext {
    void Reset() {
        std::memset(&wsaOverlapped, 0, sizeof(wsaOverlapped));
        socket = -1;
        std::memset(&addr, 0, sizeof(addr));
        offset = 0;
        requestType = 0;
        requestResult = 0;
        Internal = 0;
        InternalHigh = 0;
        iParam = 0;
        lParam = 0;
        prev = nullptr;
        next = nullptr;
    }

    GreenDamTan_Overlapped wsaOverlapped{};
    std::intptr_t socket = -1;
    sockaddr_in addr{};
    char* buffer = nullptr;
    std::uint32_t offset = 0;
    std::uint32_t length = 0;
    std::uint32_t requestType = 0;
    std::uint32_t requestResult = 0;
    std::uint32_t Internal = 0;
    std::uint32_t InternalHigh = 0;
    std::uint64_t iParam = 0;
    std::uint64_t lParam = 0;
    PerIoContext* prev = nullptr;
    PerIoContext* next = nullptr;
};


static_assert(sizeof(GreenDamTan_Overlapped) == 0x20, "GreenDamTan_Overlapped layout mismatch");
static_assert(sizeof(PerIoContext) == 0x78, "PerIoContext layout mismatch");
static_assert(offsetof(PerIoContext, buffer) == 0x38, "PerIoContext::buffer offset mismatch");
static_assert(offsetof(PerIoContext, next) == 0x70, "PerIoContext::next offset mismatch");

struct IoContextBuffer {
    static constexpr std::size_t kMaxPacketCount = 0x20000;

    void Init() {
        offset = -1;
        usLeftSize = 0;
    }

    void Reset() {
        Init();
    }

    bool Push(PerIoContext* ioContext) {
        if (!ioContext) {
            return false;
        }

        const int nextOffset = offset + 1;
        if (nextOffset < 0 || nextOffset >= static_cast<int>(kMaxPacketCount)) {
            return false;
        }

        offset = nextOffset;
        buffer[static_cast<std::size_t>(offset)] = ioContext;
        usLeftSize = 0xFFFFu;
        return true;
    }

    std::array<PerIoContext*, kMaxPacketCount> buffer{};
    int offset = -1;
    std::uint16_t usLeftSize = 0;
};

static_assert(sizeof(IoContextBuffer) == 0x100008, "IoContextBuffer layout mismatch");
static_assert(offsetof(IoContextBuffer, offset) == 0x100000, "IoContextBuffer::offset mismatch");
static_assert(offsetof(IoContextBuffer, usLeftSize) == 0x100004, "IoContextBuffer::usLeftSize mismatch");

class cIoContextPool {
public:
    cIoContextPool() {
        mrwLock.Init();
    }

    virtual ~cIoContextPool() {
        Shutdown();
    }

    void Reset() {
        mBufferLength = 0xFFFFu;
        Shutdown();
    }

    void Shutdown() {
        CSimpleLock::Owner lock(&mrwLock);
        ShutdownList(mPagedPoolUsage, mQuotaPagedPoolUsage);
        ShutdownList(mNonPagedPoolUsage, mQuotaNonPagedPoolUsage);
    }

    PerIoContext* GetPool() {
        CSimpleLock::Owner lock(&mrwLock);
        PerIoContext* ioContext = mNonPagedPoolUsage;
        if (ioContext) {
            DetachNode(ioContext, mNonPagedPoolUsage, mQuotaNonPagedPoolUsage);
        } else {
            ioContext = AllocIoContext();
        }

        if (!ioContext) {
            return nullptr;
        }

        AttachFront(ioContext, mPagedPoolUsage, mQuotaPagedPoolUsage);
        return ioContext;
    }

    void ReleaseIoContext(PerIoContext* ioContext, std::uint32_t requestType) {
        (void)requestType;
        ReleaseIoContext(ioContext, false, nullptr);
    }

    void ReleaseIoContext(PerIoContext* ioContext, bool deleteIoContext, unsigned int* errorPtr) {
        (void)errorPtr;
        if (!ioContext) {
            return;
        }

        CSimpleLock::Owner lock(&mrwLock);
        const std::uint64_t usedBytes =
            static_cast<std::uint64_t>(ioContext->InternalHigh) + ioContext->offset;
        if (usedBytes > ioContext->length) {
            throw "Error - void cIoContextPool::ReleaseIoContext( ) is overflow.";
        }

        std::memset(&ioContext->wsaOverlapped, 0, sizeof(ioContext->wsaOverlapped));
        ioContext->socket = -1;
        std::memset(&ioContext->addr, 0, sizeof(ioContext->addr));
        if (ioContext->buffer && usedBytes != 0) {
            std::memset(ioContext->buffer, 0, static_cast<std::size_t>(usedBytes));
        }
        ioContext->offset = 0;
        ioContext->Internal = 0;
        ioContext->InternalHigh = 0;
        ioContext->requestType = 0;
        ioContext->requestResult = 0;
        ioContext->iParam = 0;
        ioContext->lParam = 0;

        ReleasePool(ioContext, deleteIoContext);
    }

    CSimpleLock mrwLock;
    std::uint32_t mBufferLength = 0xFFFFu;
    PerIoContext* mPagedPoolUsage = nullptr;
    PerIoContext* mNonPagedPoolUsage = nullptr;
    std::uint32_t mQuotaPagedPoolUsage = 0;
    std::uint32_t mQuotaNonPagedPoolUsage = 0;
    std::uint32_t mWorkingSetSize = 0;

private:
    static void AttachFront(PerIoContext* ioContext, PerIoContext*& head, std::uint32_t& count) {
        if (!ioContext) {
            return;
        }

        ioContext->prev = nullptr;
        ioContext->next = head;
        if (head) {
            head->prev = ioContext;
        }
        head = ioContext;
        ++count;
    }

    static void DetachNode(PerIoContext* ioContext, PerIoContext*& head, std::uint32_t& count) {
        if (!ioContext) {
            return;
        }

        if (ioContext->prev) {
            ioContext->prev->next = ioContext->next;
        } else {
            head = ioContext->next;
        }

        if (ioContext->next) {
            ioContext->next->prev = ioContext->prev;
        }

        ioContext->prev = nullptr;
        ioContext->next = nullptr;
        if (count > 0) {
            --count;
        }
    }

    void ShutdownList(PerIoContext*& head, std::uint32_t& count) {
        while (head) {
            PerIoContext* ioContext = head;
            head = ioContext->next;
            ioContext->prev = nullptr;
            ioContext->next = nullptr;
            if (ioContext->buffer) {
                GreenDamTan_GlobalFreeRaw(ioContext->buffer);
                ioContext->buffer = nullptr;
            }
            GreenDamTan_GlobalFreeRaw(ioContext);
            if (count > 0) {
                --count;
            }
            if (mWorkingSetSize > 0) {
                --mWorkingSetSize;
            }
        }
    }

    PerIoContext* AllocIoContext() {
        auto* ioContext =
            static_cast<PerIoContext*>(GreenDamTan_GlobalAllocZero(sizeof(PerIoContext)));
        if (!ioContext) {
            throw "Error - PerIoContext* cIoContextPool::AllocIoContext( ) is null.";
        }

        ioContext->buffer = static_cast<char*>(GreenDamTan_GlobalAllocZero(mBufferLength));
        if (!ioContext->buffer) {
            GreenDamTan_GlobalFreeRaw(ioContext);
            throw "Error - PerIoContext* cIoContextPool::AllocIoContext( ) is null.";
        }

        ioContext->length = mBufferLength;
        ioContext->Reset();
        ++mWorkingSetSize;
        return ioContext;
    }

    void ReleasePool(PerIoContext* ioContext, bool deleteIoContext) {
        (void)deleteIoContext;
        DetachNode(ioContext, mPagedPoolUsage, mQuotaPagedPoolUsage);
        AttachFront(ioContext, mNonPagedPoolUsage, mQuotaNonPagedPoolUsage);
    }
};

/**
 * @brief `TXDBSocketT<TUser>` 依赖的 `XIOCPClient` 最小跨平台还原。
 *
 * 本轮继续沿 `SetSocket / OnRecv / Parsing` 往前推进，当前已恢复：
 * - `Init / Connect / ConnectThread / SetSocket / Send / OnRecv / Parsing / DisConnect / Shutdown`
 * - `OnConnect / OnDisConnect / OnNotConnect / OnParse`
 * - `m_xIOPool / m_IoContextPool / m_IoContextFrontBuffer / m_IoContextBackBuffer`
 * - `XIOCPSkeleton::CreateIOCP / WorkerThread` 驱动的 synthetic IOCP 收口
 *
 * // TODO: 需人工审查：当前仍未直接恢复原版 `setsockopt / CreateIoCompletionPort /
 * PostQueuedCompletionStatus / closesocket` 的 WinSock 系统调用，只保留完成事件类型、
 * completion key 与收包/发包/断开方向的跨平台承接骨架。
 */
class XIOCPClient : public XTCPSkeleton {
public:
    enum E_CONNECT_STATE : int {
        eConnectStateNone = 0,
        eConnectStateConnected = 1,
        eConnectStateDisconnected = 2,
        eConnectStateReconnect = 3,
        eConnectStateShutdown = 4,
    };

    struct PacketAudit {
        std::uint8_t mainCmd = 0;
        std::uint8_t subCmd = 0;
        std::size_t fieldCount = 0;
    };

    struct ReConnectInfo {
        std::string szAddr;
        std::uint16_t usPort = 0;
    };

    struct SocketState {
        enum E_BLOCK_TYPE : std::uint8_t {
            eBLOCK_OFF = 0,
            eBLOCK_ON = 1,
        };

        void Init() {
            Socket = -1;
            eBlock = eBLOCK_OFF;
            xLock.Init();
        }

        std::intptr_t Socket = -1;
        E_BLOCK_TYPE eBlock = eBLOCK_OFF;
        CSimpleLock xLock;
    };

    struct PendingRecvCompletion {
        XOverLab* pOverLab = nullptr;
        std::uint32_t dwBytesTransferred = 0;
        std::vector<std::uint8_t> buffer;
    };

    virtual ~XIOCPClient() = default;

    virtual bool Init(E_POOL_ID ePoolID, const char* szAddr, std::uint16_t wPort) {
        this->XIOCPSkeleton::Shutdown(0xFFFFFFFFu);
        GreenDamTan_JoinConnectThread();
        GreenDamTan_ReleaseOwnedIoState();

        m_ePoolID = ePoolID;
        m_stReConnectInfo.szAddr = szAddr ? szAddr : "";
        m_stReConnectInfo.usPort = wPort;
        m_cConnectRetry = 0;
        m_eState = eConnectStateNone;
        m_nErrorCode = 0;
        if (!this->XIOCPSkeleton::CreateIOCP(
                m_stReConnectInfo.szAddr.c_str(), wPort, ePoolID, 1536)) {
            m_bInit = false;
            return false;
        }

        m_xSocket.Init();
        m_IoContextPool = new (std::nothrow) cIoContextPool();
        m_IoContextFrontBuffer = GreenDamTan_AllocIoContextBuffer();
        m_IoContextBackBuffer = GreenDamTan_AllocIoContextBuffer();
        if (!m_IoContextPool || !m_IoContextFrontBuffer || !m_IoContextBackBuffer) {
            GreenDamTan_ReleaseOwnedIoState();
            m_bInit = false;
            return false;
        }

        m_IoContextFrontBuffer->Init();
        m_IoContextBackBuffer->Init();
        m_bInit = true;
        m_pRecvOverLab = nullptr;
        {
            std::lock_guard<std::mutex> recvLock(m_xPendingRecvLock);
            m_xPendingRecvCompletions.clear();
        }
        std::lock_guard<std::mutex> lock(m_xSendAuditLock);
        m_vecSendAudit.clear();
        return true;
    }

    virtual bool Connect(char* szAddr, std::uint16_t usPort) {
        if (!szAddr || GreenDamTan_HasConnectThreadHandle()) {
            return false;
        }

        if (!GreenDamTan_SetRemoteAddress(szAddr, usPort)) {
            return false;
        }

        m_stReConnectInfo.szAddr = szAddr;
        m_stReConnectInfo.usPort = usPort;
        m_cConnectRetry = 0;
        return XIOCPClient::Connect();
    }

    virtual bool Connect() {
        if (!m_bInit || GreenDamTan_HasConnectThreadHandle() ||
            m_eState == eConnectStateShutdown || m_cConnectRetry >= 3) {
            return false;
        }

        if (m_xSocket.Socket != -1) {
#ifdef _WIN32
            ::closesocket(static_cast<SOCKET>(m_xSocket.Socket));
#else
            ::close(static_cast<int>(m_xSocket.Socket));
#endif
            m_xSocket.Init();
        }

#ifdef _WIN32
        const SOCKET socketHandle = ::WSASocketA(AF_INET, SOCK_STREAM, 0, nullptr, 0, 1u);
        if (socketHandle == INVALID_SOCKET) {
            return false;
        }
        m_xSocket.Socket = static_cast<std::intptr_t>(socketHandle);
#else
        const int socketHandle = ::socket(AF_INET, SOCK_STREAM, 0);
        if (socketHandle < 0) {
            return false;
        }
        m_xSocket.Socket = static_cast<std::intptr_t>(socketHandle);
#endif
        m_eState = eConnectStateNone;
        m_bConnectThreadActive = true;
        try {
            m_hIOCPConnectThread = std::thread([this]() {
                _fnConnectThread(this);
            });
        } catch (...) {
#ifdef _WIN32
            ::closesocket(static_cast<SOCKET>(m_xSocket.Socket));
#else
            ::close(static_cast<int>(m_xSocket.Socket));
#endif
            m_xSocket.Init();
            m_bConnectThreadActive = false;
            return false;
        }
        ++m_cConnectRetry;
        return true;
    }

    bool IsConnection() const { return m_eState == eConnectStateConnected; }

    static std::uint32_t _fnConnectThread(void* ptr) {
        auto* self = static_cast<XIOCPClient*>(ptr);
        return self ? self->ConnectThread() : 0;
    }

    virtual std::uint32_t ConnectThread() {
        if (m_eState == eConnectStateShutdown) {
            m_bConnectThreadActive = false;
            return 1;
        }

        const int nConnectResult = ::connect(m_xSocket.Socket,
                                             reinterpret_cast<const sockaddr*>(&m_scAddr),
                                             sizeof(m_scAddr));
        if (nConnectResult == -1) {
            LogHelper::LogLegacy(
                "XIOCPClient::ConnectThread :: connect Error [addr : %s][port : %u] ",
                m_stReConnectInfo.szAddr.c_str(),
                static_cast<unsigned int>(m_stReConnectInfo.usPort));
        }

        XOverLab* pOverLab =
            this->m_xIOPool.AllocIO(m_xSocket.Socket, XOverLab::eOVERLAB_TYPE_CONNECT);
        if (!pOverLab) {
            m_nErrorCode |= 1u;
            m_bConnectThreadActive = false;
            return 1;
        }

        std::memcpy(pOverLab->szBuffer.data(), &nConnectResult, sizeof(nConnectResult));
        pOverLab->usOffset = 1;
        this->GreenDamTan_PostIOCPEvent(nullptr, pOverLab, 1, true);
        m_bConnectThreadActive = false;
        return 0;
    }

    virtual bool SetSocket() {
        if (m_xSocket.Socket == -1) {
            return false;
        }

        if (!GreenDamTan_SetSocketOptions(m_xSocket.Socket)) {
            return false;
        }

        this->GreenDamTan_RegisterCompletionKey(m_xSocket.Socket, &m_xSocket);
        m_pRecvOverLab = this->m_xIOPool.AllocIO(m_xSocket.Socket, XOverLab::eOVERLAB_TYPE_READ);
        return m_pRecvOverLab != nullptr && this->XTCPSkeleton::XRecv(m_pRecvOverLab);
    }

    virtual bool Send(const XPacket& xSendPacket) {
        if (!IsConnection()) {
            return false;
        }

        XOverLab* pOverLab =
            this->m_xIOPool.AllocIO(m_xSocket.Socket, XOverLab::eOVERLAB_TYPE_WRITE);
        if (!pOverLab) {
            m_nErrorCode |= 2u;
            LogHelper::LogLegacy("XIOCPClient::Send ==> empty m_xIOPool ");
            return false;
        }

        std::uint16_t usOutSize = 0;
        if (!xSendPacket.GreenDamTan_Encrypt(pOverLab->szBuffer.data(), usOutSize)) {
            this->m_xIOPool.FreeIO(pOverLab);
            return false;
        }
        pOverLab->usOffset = usOutSize;

        std::lock_guard<std::mutex> lock(m_xSendAuditLock);
        m_vecSendAudit.push_back(
            {xSendPacket.GetMainCmd(), xSendPacket.GetSubCmd(), xSendPacket.XParse.Size()});
        return this->XTCPSkeleton::XSend(pOverLab);
    }

    bool OnRecv(XSocket* pSocket,
                XOverLab* pOverLab,
                std::uint32_t dwNumberOfBytesSent) override {
        (void)pSocket;
        if (!pOverLab) {
            return false;
        }
        if (dwNumberOfBytesSent > 0xFFFFu) {
            m_nErrorCode |= 4u;
            return false;
        }

        PendingRecvCompletion completion;
        const bool hasSyntheticCompletion = PopPendingRecvCompletion(pOverLab, &completion);
        if (hasSyntheticCompletion) {
            if (completion.buffer.size() != dwNumberOfBytesSent) {
                dwNumberOfBytesSent = static_cast<std::uint32_t>(completion.buffer.size());
            }
            if (static_cast<std::size_t>(pOverLab->usOffset) + completion.buffer.size() >
                pOverLab->szBuffer.size()) {
                m_nErrorCode |= 8u;
                return false;
            }

            if (!completion.buffer.empty()) {
                std::memcpy(
                    pOverLab->szBuffer.data() + pOverLab->usOffset,
                    completion.buffer.data(),
                    completion.buffer.size());
            }
        }
        pOverLab->usOffset = static_cast<std::uint16_t>(pOverLab->usOffset + dwNumberOfBytesSent);

        std::uint16_t usRemain = pOverLab->usOffset;
        std::uint16_t usConsumed = 0;
        if (usRemain >= sizeof(PACKET_HEADER)) {
            while (true) {
                char* pPacketBuffer = pOverLab->szBuffer.data() + usConsumed;
                std::uint16_t usPacketSize = 0;
                std::memcpy(&usPacketSize, pPacketBuffer + 2, sizeof(usPacketSize));
                if (usPacketSize < sizeof(PACKET_HEADER)) {
                    m_nErrorCode |= 0x40u;
                    m_xSocket.eBlock = SocketState::eBLOCK_ON;
                    return false;
                }
                if (usPacketSize > usRemain) {
                    break;
                }

                PerIoContext* pIoContext = nullptr;
                {
                    std::lock_guard<std::mutex> lock(m_xIoContextBufferLock);
                    if (m_IoContextBackBuffer && m_IoContextBackBuffer->offset >= 0 &&
                        m_IoContextBackBuffer->usLeftSize > usPacketSize) {
                        pIoContext =
                            m_IoContextBackBuffer->buffer[static_cast<std::size_t>(m_IoContextBackBuffer->offset)];
                    } else {
                        pIoContext = m_IoContextPool ? m_IoContextPool->GetPool() : nullptr;
                        if (!pIoContext) {
                            m_nErrorCode |= 0x10u;
                            return false;
                        }
                        pIoContext->socket = m_xSocket.Socket;
                        pIoContext->requestType = 0;
                        if (!m_IoContextBackBuffer || !m_IoContextBackBuffer->Push(pIoContext)) {
                            if (m_IoContextPool) {
                                m_IoContextPool->ReleaseIoContext(pIoContext, 2);
                            }
                            m_nErrorCode |= 8u;
                            return false;
                        }
                    }
                }

                if (!pIoContext || !pIoContext->buffer ||
                    pIoContext->InternalHigh + usPacketSize > pIoContext->length) {
                    m_nErrorCode |= 8u;
                    return false;
                }

                std::memcpy(
                    pIoContext->buffer + pIoContext->InternalHigh, pPacketBuffer, usPacketSize);
                const std::uint8_t byVersion = static_cast<std::uint8_t>(pPacketBuffer[0]);
                if (byVersion < SY_KEY_TABLE.size()) {
                    for (std::uint16_t i = 0; i < usPacketSize - sizeof(PACKET_HEADER); ++i) {
                        pIoContext->buffer[pIoContext->InternalHigh + sizeof(PACKET_HEADER) + i] ^=
                            static_cast<char>(SY_KEY_TABLE[byVersion][i % 3]);
                    }
                }

                pIoContext->InternalHigh =
                    static_cast<std::uint32_t>(pIoContext->InternalHigh + usPacketSize);
                usRemain = static_cast<std::uint16_t>(usRemain - usPacketSize);
                usConsumed = static_cast<std::uint16_t>(usConsumed + usPacketSize);
                {
                    std::lock_guard<std::mutex> lock(m_xIoContextBufferLock);
                    if (m_IoContextBackBuffer) {
                        if (m_IoContextBackBuffer->usLeftSize >= usPacketSize) {
                            m_IoContextBackBuffer->usLeftSize =
                                static_cast<std::uint16_t>(m_IoContextBackBuffer->usLeftSize - usPacketSize);
                        } else {
                            m_IoContextBackBuffer->usLeftSize = 0;
                        }
                    }
                }

                if (usRemain < sizeof(PACKET_HEADER)) {
                    break;
                }
            }
        }

        if (usConsumed != 0) {
            XOverLab* pNewOverLab =
                this->m_xIOPool.AllocIO(m_xSocket.Socket, XOverLab::eOVERLAB_TYPE_READ);
            if (!pNewOverLab) {
                m_nErrorCode |= 1u;
                m_xSocket.eBlock = SocketState::eBLOCK_ON;
                return false;
            }
            if (usRemain > 0) {
                std::memcpy(pNewOverLab->szBuffer.data(), pOverLab->szBuffer.data() + usConsumed, usRemain);
                pNewOverLab->usOffset = usRemain;
            }
            this->m_xIOPool.FreeIO(pOverLab);
            pOverLab = pNewOverLab;
        }

        m_pRecvOverLab = pOverLab;
        return this->XTCPSkeleton::XRecv(pOverLab);
    }

    virtual void Parsing() {
        if (!m_bInit) {
            return;
        }

        if (m_eState == eConnectStateReconnect && !m_stReConnectInfo.szAddr.empty()) {
            const std::string reconnectAddr = m_stReConnectInfo.szAddr;
            const std::uint16_t reconnectPort = m_stReConnectInfo.usPort;
            XIOCPClient::Shutdown(0xFFFFFFFFu);
            XIOCPClient::Init(ePoolIDNone, "127.0.0.1", 5001);
            XIOCPClient::Connect(const_cast<char*>(reconnectAddr.c_str()), reconnectPort);
        }

        if (m_xSocket.eBlock != SocketState::eBLOCK_OFF) {
            if (m_eState == eConnectStateConnected) {
                OnDisConnect();
                GreenDamTan_DetachConnectThread();
                Close();
            }
            return;
        }

        GreenDamTan_ParseIoContextBuffer();
    }

    virtual bool DisConnect() {
        if (!IsConnection()) {
            return false;
        }

        OnDisConnect();
        GreenDamTan_DetachConnectThread();
        Close();
        return true;
    }

    bool Shutdown(std::uint32_t dwMaxWait) override {
        m_eState = eConnectStateShutdown;
        m_bInit = false;
        GreenDamTan_JoinConnectThread();
        const std::intptr_t socketHandle = m_xSocket.Socket;
        this->GreenDamTan_UnregisterCompletionKey(socketHandle);
        if (socketHandle != -1) {
#ifdef _WIN32
            ::closesocket(static_cast<SOCKET>(socketHandle));
#else
            ::close(static_cast<int>(socketHandle));
#endif
        }
        m_xSocket.Init();
        GreenDamTan_ReleaseOwnedIoState();
        (void)dwMaxWait;
        return this->XIOCPSkeleton::Shutdown(0xFFFFFFFFu);
    }

    /**
     * @brief 供跨平台骨架注入一条“已收到的 DB 包”。
     *
     * 原版 `OnRecv` 从原始字节流里切包、解密并写入 `IoContextBuffer`；
     * 本轮已把 `PACKET_HEADER + SY_KEY_TABLE` 的真实切包/解密方向接回，
     * 这里继续保留一个 `GreenDamTan_` 前缀注入入口，用于跨平台骨架投递
     * “一条已经完成加密前序列化的包”。
     */
    bool GreenDamTan_QueueRecvPacket(const XPacket& xPacket) {
        if (!m_bInit || m_xSocket.Socket == -1) {
            return false;
        }

        if (!m_pRecvOverLab) {
            m_pRecvOverLab =
                this->m_xIOPool.AllocIO(m_xSocket.Socket, XOverLab::eOVERLAB_TYPE_READ);
            if (!m_pRecvOverLab || !this->XTCPSkeleton::XRecv(m_pRecvOverLab)) {
                return false;
            }
        }

        PendingRecvCompletion completion;
        completion.pOverLab = m_pRecvOverLab;
        std::array<char, 0x10005> encryptedBuffer{};
        std::uint16_t usPacketSize = 0;
        if (!xPacket.GreenDamTan_Encrypt(encryptedBuffer.data(), usPacketSize)) {
            return false;
        }
        completion.dwBytesTransferred = usPacketSize;
        completion.buffer.assign(
            reinterpret_cast<const std::uint8_t*>(encryptedBuffer.data()),
            reinterpret_cast<const std::uint8_t*>(encryptedBuffer.data() + usPacketSize));
        {
            std::lock_guard<std::mutex> lock(m_xPendingRecvLock);
            m_xPendingRecvCompletions.push_back(std::move(completion));
        }
        this->GreenDamTan_PostIOCPEvent(
            this->GreenDamTan_FindCompletionKey(m_pRecvOverLab->Socket),
            m_pRecvOverLab,
            usPacketSize,
            true);
        m_pRecvOverLab = nullptr;
        return true;
    }

protected:
    virtual void OnConnect() {
        LogHelper::LogLegacy("Client Connect");
    }

    virtual void OnDisConnect() {
        LogHelper::LogLegacy("Client DisConnect");
        m_eState = eConnectStateDisconnected;
    }

    virtual void OnNotConnect() {
        LogHelper::LogLegacy("Client NotConnect");
    }

    virtual bool OnParse(XPacket& xPacket) {
        (void)xPacket;
        return true;
    }

    void OnEtcEvent(XOverLab* pOverLab) override {
        if (!pOverLab) {
            LogHelper::LogLegacy("XIOCPClient::OnEtcEvent :: IS_NULL<XOverLab>(pOverLab)");
            return;
        }

        if (pOverLab->eType == XOverLab::eOVERLAB_TYPE_CONNECT &&
            m_eState != eConnectStateShutdown) {
            GreenDamTan_JoinConnectThread();

            int nConnectResult = 0;
            std::memcpy(&nConnectResult, pOverLab->szBuffer.data(), sizeof(nConnectResult));
            if (nConnectResult == -1) {
                OnNotConnect();
            } else if (SetSocket()) {
                m_eState = eConnectStateConnected;
                OnConnect();
            } else {
                LogHelper::LogLegacy(
                    "XIOCPClient::OnEtcEvent :: if( false == SetSocket() )");
#ifdef _WIN32
                ::closesocket(static_cast<SOCKET>(m_xSocket.Socket));
#else
                ::close(static_cast<int>(m_xSocket.Socket));
#endif
                m_xSocket.Init();
            }
        }

        this->m_xIOPool.FreeIO(pOverLab);
    }

    virtual void Close() {
        const std::intptr_t socketHandle = m_xSocket.Socket;
        this->GreenDamTan_PostCloseEvent();
        this->GreenDamTan_UnregisterCompletionKey(socketHandle);
        if (socketHandle != -1) {
            linger lingerOption{};
            lingerOption.l_onoff = 1;
            lingerOption.l_linger = 0;
#ifdef _WIN32
            const SOCKET socketValue = static_cast<SOCKET>(socketHandle);
            ::setsockopt(socketValue,
                         SOL_SOCKET,
                         SO_LINGER,
                         reinterpret_cast<const char*>(&lingerOption),
                         sizeof(lingerOption));
            ::closesocket(socketValue);
            ::closesocket(socketValue);
#else
            const int socketValue = static_cast<int>(socketHandle);
            ::setsockopt(socketValue, SOL_SOCKET, SO_LINGER, &lingerOption, sizeof(lingerOption));
            ::close(socketValue);
            ::close(socketValue);
#endif
        }
        m_xSocket.Init();
    }

    void OnSend(XSocket* pSocket, XOverLab* pOverLab) override {
        (void)pSocket;
        if (!pOverLab) {
            LogHelper::LogLegacy("XTCPSkeleton::OnSend :: IS_NULL<XOverLab>(pOverLab)");
            return;
        }
        this->m_xIOPool.FreeIO(pOverLab);
    }

    void GreenDamTan_JoinConnectThread() {
        if (m_hIOCPConnectThread.joinable() &&
            m_hIOCPConnectThread.get_id() != std::this_thread::get_id()) {
            m_hIOCPConnectThread.join();
        }
        m_bConnectThreadActive = false;
    }

    void GreenDamTan_DetachConnectThread() {
        if (m_hIOCPConnectThread.joinable() &&
            m_hIOCPConnectThread.get_id() != std::this_thread::get_id()) {
            m_hIOCPConnectThread.detach();
        }
        m_bConnectThreadActive = false;
    }

    bool GreenDamTan_HasConnectThreadHandle() const {
        return m_bConnectThreadActive || m_hIOCPConnectThread.joinable();
    }

    bool GreenDamTan_SetRemoteAddress(const char* szAddr, std::uint16_t usPort) {
        if (!szAddr) {
            return false;
        }

        std::memset(&m_scAddr, 0, sizeof(m_scAddr));
        m_scAddr.sin_family = AF_INET;
        m_scAddr.sin_port = htons(usPort);
        const auto nIP = ::inet_addr(szAddr);
        if (nIP != INADDR_NONE) {
            m_scAddr.sin_addr.s_addr = nIP;
            return true;
        }

        hostent* hostEntry = ::gethostbyname(szAddr);
        if (!hostEntry || !hostEntry->h_addr_list || !hostEntry->h_addr_list[0]) {
            return false;
        }

        std::memcpy(&m_scAddr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
        return true;
    }

    static bool GreenDamTan_SetSocketOptions(std::intptr_t socketHandle) {
        if (socketHandle == -1) {
            return false;
        }

        int nNodelay = 1;
        int nZero = 0;
        linger lingerOption{};
        lingerOption.l_onoff = 1;
        lingerOption.l_linger = 0;

#ifdef _WIN32
        const SOCKET socketValue = static_cast<SOCKET>(socketHandle);
        return ::setsockopt(socketValue,
                            IPPROTO_TCP,
                            TCP_NODELAY,
                            reinterpret_cast<const char*>(&nNodelay),
                            sizeof(nNodelay)) != -1 &&
               ::setsockopt(socketValue,
                            SOL_SOCKET,
                            SO_SNDBUF,
                            reinterpret_cast<const char*>(&nZero),
                            sizeof(nZero)) != -1 &&
               ::setsockopt(socketValue,
                            SOL_SOCKET,
                            SO_RCVBUF,
                            reinterpret_cast<const char*>(&nZero),
                            sizeof(nZero)) != -1 &&
               ::setsockopt(socketValue,
                            SOL_SOCKET,
                            SO_LINGER,
                            reinterpret_cast<const char*>(&lingerOption),
                            sizeof(lingerOption)) != -1;
#else
        const int socketValue = static_cast<int>(socketHandle);
        return ::setsockopt(socketValue, IPPROTO_TCP, TCP_NODELAY, &nNodelay, sizeof(nNodelay)) != -1 &&
               ::setsockopt(socketValue, SOL_SOCKET, SO_SNDBUF, &nZero, sizeof(nZero)) != -1 &&
               ::setsockopt(socketValue, SOL_SOCKET, SO_RCVBUF, &nZero, sizeof(nZero)) != -1 &&
               ::setsockopt(socketValue, SOL_SOCKET, SO_LINGER, &lingerOption, sizeof(lingerOption)) != -1;
#endif
    }

    void GreenDamTan_PumpSocketRecv() {
        if (!IsConnection() || m_xSocket.Socket == -1 || m_xSocket.eBlock != SocketState::eBLOCK_OFF ||
            !m_pRecvOverLab) {
            return;
        }

        fd_set readSet;
        FD_ZERO(&readSet);
#ifdef _WIN32
        const SOCKET socketValue = static_cast<SOCKET>(m_xSocket.Socket);
        FD_SET(socketValue, &readSet);
#else
        const int socketValue = static_cast<int>(m_xSocket.Socket);
        FD_SET(socketValue, &readSet);
#endif

        timeval timeout{};
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
#ifdef _WIN32
        const int selectResult = ::select(0, &readSet, nullptr, nullptr, &timeout);
#else
        const int selectResult = ::select(socketValue + 1, &readSet, nullptr, nullptr, &timeout);
#endif
        if (selectResult <= 0) {
            return;
        }

        XOverLab* pRecvOverLab = m_pRecvOverLab;
        const int recvLength = static_cast<int>(pRecvOverLab->usSize - pRecvOverLab->usOffset);
        if (recvLength <= 0) {
            return;
        }

#ifdef _WIN32
        const int recvResult =
            ::recv(socketValue, pRecvOverLab->szBuffer.data() + pRecvOverLab->usOffset, recvLength, 0);
        const int lastError = recvResult == -1 ? WSAGetLastError() : 0;
#else
        const int recvResult =
            ::recv(socketValue,
                   pRecvOverLab->szBuffer.data() + pRecvOverLab->usOffset,
                   static_cast<std::size_t>(recvLength),
                   0);
        const int lastError = recvResult == -1 ? errno : 0;
#endif

        if (recvResult > 0) {
            this->GreenDamTan_PostIOCPEvent(this->GreenDamTan_FindCompletionKey(pRecvOverLab->Socket),
                                            pRecvOverLab,
                                            static_cast<std::uint32_t>(recvResult),
                                            true);
            m_pRecvOverLab = nullptr;
            return;
        }

#ifdef _WIN32
        if (lastError == WSAEWOULDBLOCK || lastError == WSAEINPROGRESS) {
#else
        if (lastError == EAGAIN || lastError == EWOULDBLOCK || lastError == EINPROGRESS) {
#endif
            return;
        }

        m_xSocket.eBlock = SocketState::eBLOCK_ON;
    }

    void GreenDamTan_ParseIoContextBuffer() {
        {
            std::lock_guard<std::mutex> lock(m_xIoContextBufferLock);
            if (m_IoContextBackBuffer && m_IoContextBackBuffer->offset >= 0) {
                std::swap(m_IoContextFrontBuffer, m_IoContextBackBuffer);
                if (m_IoContextBackBuffer) {
                    m_IoContextBackBuffer->Reset();
                }
            }
        }

        if (!m_IoContextFrontBuffer || m_IoContextFrontBuffer->offset < 0) {
            return;
        }

        for (int index = 0; index <= m_IoContextFrontBuffer->offset; ++index) {
            PerIoContext* ioContext =
                m_IoContextFrontBuffer->buffer[static_cast<std::size_t>(index)];
            if (!ioContext) {
                continue;
            }

            while (ioContext->Internal < ioContext->InternalHigh) {
                char* pPacketBuffer = ioContext->buffer + ioContext->Internal;
                std::uint16_t usPacketSize = 0;
                std::memcpy(&usPacketSize, pPacketBuffer + 2, sizeof(usPacketSize));
                if (usPacketSize < sizeof(PACKET_HEADER) ||
                    ioContext->Internal + usPacketSize > ioContext->InternalHigh) {
                    break;
                }

                XPacket xPacket;
                if (xPacket.GreenDamTan_AssignNetworkPacket(pPacketBuffer, usPacketSize)) {
                    OnParse(xPacket);
                }
                ioContext->Internal =
                    static_cast<std::uint32_t>(ioContext->Internal + usPacketSize);
            }
            ioContext->Internal = 0;
            ioContext->InternalHigh = 0;
            if (m_IoContextPool) {
                m_IoContextPool->ReleaseIoContext(ioContext, 2);
            }
            m_IoContextFrontBuffer->buffer[static_cast<std::size_t>(index)] = nullptr;
        }
        m_IoContextFrontBuffer->Reset();
    }

    bool PopPendingRecvCompletion(XOverLab* pOverLabFilter, PendingRecvCompletion* outCompletion) {
        if (!outCompletion) {
            return false;
        }

        std::lock_guard<std::mutex> lock(m_xPendingRecvLock);
        if (m_xPendingRecvCompletions.empty()) {
            return false;
        }

        if (pOverLabFilter == nullptr) {
            *outCompletion = std::move(m_xPendingRecvCompletions.front());
            m_xPendingRecvCompletions.pop_front();
            return true;
        }

        for (auto it = m_xPendingRecvCompletions.begin(); it != m_xPendingRecvCompletions.end();
             ++it) {
            if (it->pOverLab == pOverLabFilter) {
                *outCompletion = std::move(*it);
                m_xPendingRecvCompletions.erase(it);
                return true;
            }
        }
        return false;
    }

    static IoContextBuffer* GreenDamTan_AllocIoContextBuffer() {
        auto* ioContextBuffer =
            static_cast<IoContextBuffer*>(GreenDamTan_GlobalAllocZero(sizeof(IoContextBuffer)));
        if (ioContextBuffer) {
            ioContextBuffer->Init();
        }
        return ioContextBuffer;
    }

    void GreenDamTan_ReleaseOwnedIoState() {
        GreenDamTan_GlobalFreeRaw(m_IoContextFrontBuffer);
        m_IoContextFrontBuffer = nullptr;
        GreenDamTan_GlobalFreeRaw(m_IoContextBackBuffer);
        m_IoContextBackBuffer = nullptr;
        delete m_IoContextPool;
        m_IoContextPool = nullptr;
        m_pRecvOverLab = nullptr;
        std::lock_guard<std::mutex> recvLock(m_xPendingRecvLock);
        m_xPendingRecvCompletions.clear();
    }

    void ReleaseIoContextBuffer(IoContextBuffer* ioContextBuffer) {
        if (!ioContextBuffer || !m_IoContextPool || ioContextBuffer->offset < 0) {
            if (ioContextBuffer) {
                ioContextBuffer->Reset();
            }
            return;
        }

        for (int index = 0; index <= ioContextBuffer->offset; ++index) {
            PerIoContext*& ioContext = ioContextBuffer->buffer[static_cast<std::size_t>(index)];
            if (!ioContext) {
                continue;
            }
            m_IoContextPool->ReleaseIoContext(ioContext, 2);
            ioContext = nullptr;
        }
        ioContextBuffer->Reset();
    }

public:
    ReConnectInfo m_stReConnectInfo{};
    E_POOL_ID m_ePoolID = ePoolIDNone;
    std::atomic<int> m_eState{eConnectStateNone};
    std::atomic<bool> m_bInit{false};
    std::atomic<std::uint8_t> m_cConnectRetry{0};
    unsigned int m_nErrorCode = 0;
    SocketState m_xSocket{};
    cIoContextPool* m_IoContextPool = nullptr;
    IoContextBuffer* m_IoContextFrontBuffer = nullptr;
    IoContextBuffer* m_IoContextBackBuffer = nullptr;
    XOverLab* m_pRecvOverLab = nullptr;

protected:
    inline static std::atomic<std::intptr_t> s_nSyntheticSocketSeed{1000};

    std::mutex m_xSendAuditLock;
    std::vector<PacketAudit> m_vecSendAudit;
    std::mutex m_xPendingRecvLock;
    std::deque<PendingRecvCompletion> m_xPendingRecvCompletions;
    std::mutex m_xIoContextBufferLock;
    std::thread m_hIOCPConnectThread;
    std::atomic<bool> m_bConnectThreadActive{false};
};

template <typename TUser>
class TXDBSocketT : public XIOCPClient {
public:
    virtual ~TXDBSocketT() {
        Shutdown(0xFFFFFFFFu);
    }

    bool Init(E_POOL_ID ePoolID,
              const char* ip,
              std::int16_t port,
              const char* name,
              int maxConnectCount) {
        Shutdown(0xFFFFFFFFu);

        m_ePoolID = ePoolID;
        m_sPort = port;
        m_nMaxConnectCount = maxConnectCount;
        m_bState = true;
        m_bSafetyShutdown = false;
        m_bCloseProcess = false;
        CopyString(m_szIP, ip);
        CopyString(m_szName, name);
        if (!XIOCPClient::Init(ePoolID, m_szIP, static_cast<std::uint16_t>(m_sPort))) {
            return false;
        }

        m_hIOCPBackendThread = std::thread(&TXDBSocketT::OnStartThread, this);
        return true;
    }

    bool AutoConnect() {
        return XIOCPClient::Connect(m_szIP, static_cast<std::uint16_t>(m_sPort));
    }

    void SetSafetyShutdown(bool value) { m_bSafetyShutdown = value; }
    bool IsConnection() const { return XIOCPClient::IsConnection(); }

    bool Shutdown(std::uint32_t dwMaxWait) override {
        m_eState = eConnectStateShutdown;
        if (m_hIOCPBackendThread.joinable() &&
            m_hIOCPBackendThread.get_id() != std::this_thread::get_id()) {
            LogHelper::LogLegacy("TXDBSocket<T>::Shutdown :: WaitForSingleObjectEx ");
            m_hIOCPBackendThread.join();
        }
        return XIOCPClient::Shutdown(dwMaxWait);
    }

    void OnStartThread() {
        using namespace std::chrono;

        while (m_eState != eConnectStateShutdown) {
            const steady_clock::time_point begin = steady_clock::now();
            GreenDamTan_PumpSocketRecv();
            XIOCPClient::Parsing();
            const auto elapsed = duration_cast<milliseconds>(steady_clock::now() - begin);
            if (elapsed < milliseconds(15)) {
                std::this_thread::sleep_for(milliseconds(15) - elapsed);
            }
        }
    }

    static std::uint64_t _BackendThreadStartingPoint(void* ptr) {
        auto* self = static_cast<TXDBSocketT*>(ptr);
        if (!self) {
            return 0;
        }
        self->OnStartThread();
        return 0;
    }

    bool OnParse(XPacket& xPacket) override {
        int xSessionID = 0;
        xPacket.XParse >> xSessionID;

        TUser* pUser = FindUser(xSessionID);  // 对齐 IDA: int 参数
        if (pUser && !pUser->GetBlockType()) {
            return DBParse(pUser, xPacket);
        }

        if (!pUser) {
            const std::uint8_t byMainCmd = xPacket.GetMainCmd();
            if (byMainCmd == 37 || byMainCmd == 1 || byMainCmd == 40) {
                return DBParse(nullptr, xPacket);
            }
        }

        return false;
    }

protected:
    // 对齐 IDA: 参数类型 H = int
    virtual TUser* FindUser(int xSessionID) {
        (void)xSessionID;
        return nullptr;
    }

    virtual bool DBParse(TUser* pUser, XPacket& xPacket) {
        (void)pUser;
        (void)xPacket;
        return true;
    }

protected:
    void OnConnect() override {
        LogHelper::LogLegacy("Connect [%s][%s]", m_szName, m_szIP);
    }

    void OnDisConnect() override {
        LogHelper::LogLegacy("Client DisConnect[%s][%s]", m_szName, m_szIP);
    }

    void OnNotConnect() override {
        LogHelper::LogLegacy("Client NotConnect[%s]", m_szName);
    }

    template <std::size_t N>
    static void CopyString(char (&destination)[N], const char* source) {
        std::memset(destination, 0, N);
        if (!source) {
            return;
        }
#ifdef _WIN32
        strncpy_s(destination, N, source, _TRUNCATE);
#else
        std::strncpy(destination, source, N - 1);
        destination[N - 1] = '\0';
#endif
    }

public:
    std::thread m_hIOCPBackendThread;
    char m_szIP[513] = {};
    std::int16_t m_sPort = 0;
    char m_szName[21] = {};
    int m_nMaxConnectCount = 0;
    bool m_bState = false;
    std::uint8_t m_byType = 0;
    bool m_bSafetyShutdown = false;
    bool m_bCloseProcess = false;
};

