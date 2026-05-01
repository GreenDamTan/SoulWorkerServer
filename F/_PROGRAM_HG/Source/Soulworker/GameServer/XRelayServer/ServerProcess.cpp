// ServerProcess.cpp - RelayServer
// 对齐 IDA RelayServer.exe: CServerProcess 只有 3 个 sub command

#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"

// ============================================================================
// CServer 方法实现 (对齐 IDA RelayServer.exe)
// ============================================================================

// 对齐 IDA 0x1400D1F60: SetServerInfo
void CServer::SetServerInfo(const SS_SERVER_INFO& info) {
    m_serverInfo = info;
}

// 对齐 IDA 0x1400D1FF0: SetSyncLoad
// IDA 流程: if (eServerSync & m_dwSyncLoad == 0) -> m_dwSyncLoad |= eServerSync
// LogHelper::LogInfo -> if (m_dwSyncLoad == 3) -> XRelayServer::SendCachingLoad
void CServer::SetSyncLoad(E_SERVER_SYNC_LOAD eServerSync) {
    if ((eServerSync & m_dwSyncLoad) == 0) {
        m_dwSyncLoad |= static_cast<std::uint32_t>(eServerSync);
    }

    LogHelper::LogInfo("game.system",
                       "<CServer::SetSyncLoad> ID:%d, Server:%d, Req:%d",
                       static_cast<int>(GetServerID()),
                       static_cast<int>(m_dwSyncLoad),
                       static_cast<int>(eServerSync));

    if (m_dwSyncLoad == 3) {
        TXSingleton<XRelayServer>::Instance()->SendCachingLoad();
    }
}

// ============================================================================
// CServerProcess 方法实现
// ============================================================================

// 对齐 IDA 0x1400CE940: CServerProcess::Parse
// switch on (unsigned __int8)GetSubCmd
// 只处理 case 1, 3, 51 (其他返回 1)
bool CServerProcess::Parse(XPacket& xPacket) {
    switch (static_cast<unsigned char>(xPacket.GetSubCmd())) {
    case 1:   // ReqCreateServer
        return ReqCreateServer(xPacket);
    case 3:   // ReqUpdateServerInfo
        return ReqUpdateServerInfo(xPacket);
    case 51:  // SyncUsersInfo (0x33)
        return SyncUsersInfo(xPacket);
    default:
        return true;  // IDA: default case returns 1
    }
}

// 对齐 IDA 0x1400CE9C0: ReqCreateServer
// 流程: GetClientPtr -> SS_SERVER_INFO::SS_SERVER_INFO -> operator>> -> SetServerInfo -> XRelayServer::AddServerInfo
bool CServerProcess::ReqCreateServer(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    SS_SERVER_INFO serverInfo{};
    xPacket >> serverInfo;
    pServer->SetServerInfo(serverInfo);
    TXSingleton<XRelayServer>::Instance()->AddServerInfo(pServer);
    return true;
}

// 对齐 IDA 0x1400CEA50: ReqUpdateServerInfo
// 流程: GetClientPtr -> operator>>
bool CServerProcess::ReqUpdateServerInfo(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    SS_UPDATE_SERVER_INFO stUpdateServerInfo{};
    xPacket >> stUpdateServerInfo;
    // TODO: 推测结果 - 需要确认是否有其他处理
    return true;
}

// 对齐 IDA 0x1400CEA90: SyncUsersInfo
// 流程: PS_USERS_INFO::PS_USERS_INFO -> operator>> -> GetClientPtr -> XRelayServer::SetUsersInfo
bool CServerProcess::SyncUsersInfo(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    PS_USERS_INFO stUsersInfo{};
    xPacket >> stUsersInfo;
    TXSingleton<XRelayServer>::Instance()->SetUsersInfo(pServer, stUsersInfo);
    return true;
}
