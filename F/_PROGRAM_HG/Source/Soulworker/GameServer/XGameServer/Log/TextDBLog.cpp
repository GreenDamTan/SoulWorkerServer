#include "GreenDamTan_TextDBLog.h"
#include "Soulworker/GameServer/XGameServer/GameServer.h"

#include <algorithm>
#include <cstdio>
#include <cstring>

CTextDBLog::CTextDBLog() : m_strLog(), m_uxMapID(0) {}

CTextDBLog::~CTextDBLog() = default;

void CTextDBLog::Init(std::int64_t nMapInsID) {
    m_strLog.clear();
    m_uxMapID.nMapID = nMapInsID;
}

void CTextDBLog::AddLog(int nType, int nParam0, int nParam1, char* szLog) {
    char buf[128];
    // TODO: 需人工审查 - 原版 sprintf 未限制 szLog 长度；这里限制写入以避免栈溢出。
    std::snprintf(buf, sizeof(buf), "%d\t%d\t%d\t%s\n", nType, nParam0, nParam1, szLog);
    m_strLog.append(buf);
    if (m_strLog.size() > 0x1D4C) {
        SendLogDB();
    }
}

void CTextDBLog::AddLog(int nType, std::int64_t nObjectID, int nUCID,
                        int param0, int param1, int param2, int param3, int param4, int param5) {
    char buf[128];
    std::snprintf(buf, sizeof(buf), "%d\t%lld\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                  nType, static_cast<long long>(nObjectID), nUCID,
                  param0, param1, param2, param3, param4, param5);
    m_strLog.append(buf);
    if (m_strLog.size() > 0x1D4C) {
        SendLogDB();
    }
}

void CTextDBLog::SendLogDB() {
    const std::size_t nLen = m_strLog.size();
    if (nLen) {
        ST_LOG_TEXT stLog;
        stLog.shMainType = 1;
        stLog.nInstanceID = m_uxMapID.nMapID;
        stLog.nMapID = static_cast<int>(static_cast<std::uint64_t>(m_uxMapID.nMapID) >> 16) >> 16;
        // TODO: 需人工审查 - 原版 strcpy 无界；超出协议字段时仅复制可容纳的数据。
        const std::size_t nCopy = std::min(nLen, sizeof(stLog.szMsg) - 1);
        std::memcpy(stLog.szMsg, m_strLog.data(), nCopy);
        stLog.szMsg[std::min(nLen - 1, sizeof(stLog.szMsg) - 1)] = '\0';
        TXSingleton<XGameServer>::Instance()->SendDBTextLog(stLog);
        m_strLog.clear();
    }
}

void CTextDBLog::SendLogDB(bool bSend) {
    const std::size_t nLen = m_strLog.size();
    if (nLen) {
        if (bSend) {
            ST_LOG_TEXT stLog;
            stLog.shMainType = 1;
            stLog.nInstanceID = m_uxMapID.nMapID;
            stLog.nMapID = static_cast<int>(static_cast<std::uint64_t>(m_uxMapID.nMapID) >> 16) >> 16;
            // TODO: 需人工审查 - 原版 strcpy 无界；超出协议字段时仅复制可容纳的数据。
            const std::size_t nCopy = std::min(nLen, sizeof(stLog.szMsg) - 1);
            std::memcpy(stLog.szMsg, m_strLog.data(), nCopy);
            stLog.szMsg[std::min(nLen - 1, sizeof(stLog.szMsg) - 1)] = '\0';
            TXSingleton<XGameServer>::Instance()->SendDBTextLog(stLog);
        }
        m_strLog.clear();
    }
}
