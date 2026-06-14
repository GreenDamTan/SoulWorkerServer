// XGameDBSocketMgr.cpp
// XGameDBSocketMgr implementation for shared XCore layer
// Reconstructed from IDA decompilation
//
// NOTE: This file provides basic constructor/destructor only.
// Server-specific implementations that access CGameDBSocket members
// are provided in each server's GameDBSocket.cpp file.

#include "Soulworker/GameServer/XCore/XServer/XGameDBSocketMgr.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"

// ============================================================================
// XGameDBSocketMgr implementation - basic lifecycle only
// Full implementations are in each server's GameDBSocket.cpp
// ============================================================================

XGameDBSocketMgr::XGameDBSocketMgr()
    : m_pGameDBAgent(nullptr)
    , m_nGameAgentCnt(0)
    , m_pAccountDBAgent(nullptr)
    , m_nAccountAgentCnt(0)
    , m_pLogDBAgent(nullptr)
    , m_nLogAgentCnt(0)
    , m_pStatisticsDBAgent(nullptr)
    , m_nStatisticsAgentCnt(0)
    , m_pSGLogDBAgent(nullptr)
    , m_nSGLogAgentCnt(0)
{
}

XGameDBSocketMgr::~XGameDBSocketMgr()
{
    Clear();
}

void XGameDBSocketMgr::Clear()
{
    // Note: Cannot delete CGameDBSocket arrays here because type is incomplete
    // Server-specific implementations should override this
    m_pGameDBAgent = nullptr;
    m_nGameAgentCnt = 0;
    m_pAccountDBAgent = nullptr;
    m_nAccountAgentCnt = 0;
    m_pLogDBAgent = nullptr;
    m_nLogAgentCnt = 0;
    m_pStatisticsDBAgent = nullptr;
    m_nStatisticsAgentCnt = 0;
    m_pSGLogDBAgent = nullptr;
    m_nSGLogAgentCnt = 0;
}

void XGameDBSocketMgr::Init()
{
    Clear();
    // Server-specific implementations should override
    GreenDamTan_log(__FILE__, __FUNCTION__, "XGameDBSocketMgr::Init - base implementation");
}

void XGameDBSocketMgr::AutoConnect()
{
    // Server-specific implementations should override
    GreenDamTan_log(__FILE__, __FUNCTION__, "XGameDBSocketMgr::AutoConnect - base implementation");
}

void XGameDBSocketMgr::DisConnect()
{
    // Server-specific implementations should override
    GreenDamTan_log(__FILE__, __FUNCTION__, "XGameDBSocketMgr::DisConnect - base implementation");
}

int XGameDBSocketMgr::GetAccountDBAgentCount()
{
    return m_nAccountAgentCnt;
}

int XGameDBSocketMgr::GetGameDBAgentCount()
{
    return m_nGameAgentCnt;
}

bool XGameDBSocketMgr::IsAccountDBConnectionAll() const
{
    return m_pAccountDBAgent != nullptr && m_nAccountAgentCnt > 0;
}

bool XGameDBSocketMgr::SendAccountDBAgent(int iIndex, XSendPacket& packet)
{
    // Server-specific implementations should override
    return false;
}

bool XGameDBSocketMgr::SendGameDBAgent(int iIndex, XSendPacket& packet)
{
    // Server-specific implementations should override
    return false;
}

bool XGameDBSocketMgr::SendLogDBAgent(int iIndex, XSendPacket& packet)
{
    // Server-specific implementations should override
    return false;
}

void XGameDBSocketMgr::InitAgentGroup(std::uint8_t byType, CGameDBSocket*& ppAgents, int& nAgentCount)
{
    // Server-specific implementations should override
    ppAgents = nullptr;
    nAgentCount = 0;
}

void XGameDBSocketMgr::AutoConnectGroup(CGameDBSocket* pAgents, int nAgentCount)
{
    // Server-specific implementations should override
}

void XGameDBSocketMgr::DisconnectGroup(CGameDBSocket* pAgents, int nAgentCount)
{
    // Server-specific implementations should override
}
