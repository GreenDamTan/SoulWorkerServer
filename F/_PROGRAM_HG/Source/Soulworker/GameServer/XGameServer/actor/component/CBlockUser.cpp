// CBlockUser - Blocked user entry wrapper class implementation
// IDA verified from GameServer.exe

#include "CBlockUser.h"
#include <cwchar>

CBlockUser::CBlockUser() {
    // Zero initialize
    m_stInfo = {};
}

// Constructor from block info - IDA pattern
CBlockUser::CBlockUser(const DB_BLOCK_INFO* stInfo) {
    if (stInfo) {
        m_stInfo = *stInfo;
    } else {
        m_stInfo = {};
    }
}

CBlockUser::~CBlockUser() {
    // Nothing to clean up
}

// GetInfo - IDA pattern
void CBlockUser::GetInfo(DB_BLOCK_INFO* stInfo) const {
    if (stInfo) {
        *stInfo = m_stInfo;
    }
}
