#pragma once

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include <cstdint>
#include <string>

class CTextDBLog {
public:
    CTextDBLog();
    ~CTextDBLog();

    void Init(std::int64_t nMapInsID);
    void AddLog(int nType, int nParam0, int nParam1, char* szLog);
    void AddLog(int nType, std::int64_t nObjectID, int nUCID,
                int param0, int param1, int param2, int param3, int param4, int param5);
    void SendLogDB();
    void SendLogDB(bool bSend);

private:
    std::string m_strLog;
    UXMapID m_uxMapID;
};

#if defined(_WIN32) && defined(_ITERATOR_DEBUG_LEVEL) && _ITERATOR_DEBUG_LEVEL > 0
static_assert(sizeof(CTextDBLog) == 48, "CTextDBLog size must match GameServer PDB");
#endif
