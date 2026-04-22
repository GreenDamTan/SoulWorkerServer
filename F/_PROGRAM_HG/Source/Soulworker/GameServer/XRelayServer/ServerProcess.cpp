#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"

namespace {
constexpr std::uint32_t E_SERVER_SYNC_LOAD_USER = 1u;
constexpr std::uint32_t E_SERVER_SYNC_LOAD_MAZE_INFO = 2u;
}

void CServer::SetSyncLoad(std::uint32_t syncMask) {
    if ((m_dwSyncLoad & syncMask) == 0) {
        m_dwSyncLoad |= syncMask;
    }

    LogHelper::LogInfo("game.system",
                       "<CServer::SetSyncLoad> ID:%u, Server:%u, Req:%u",
                       static_cast<unsigned int>(GetServerID()),
                       static_cast<unsigned int>(m_dwSyncLoad),
                       static_cast<unsigned int>(syncMask));

    if (m_dwSyncLoad == (E_SERVER_SYNC_LOAD_USER | E_SERVER_SYNC_LOAD_MAZE_INFO)) {
        TXSingleton<XRelayServer>::Instance()->SendCachingLoad();
    }
}

bool CServerProcess::Parse(XPacket& xPacket) {
    switch (xPacket.GetSubCmd()) {
    case 1:
        return ReqCreateServer(xPacket);
    case 3:
        return ReqUpdateServerInfo(xPacket);
    case 51:
        return SyncUsersInfo(xPacket);
    }
    return true;  // IDA: default case returns 1
}

bool CServerProcess::ReqCreateServer(XPacket& xPacket) {
    CServer* server = GetClientPtr();
    if (!server) {
        return false;
    }

    SS_SERVER_INFO serverInfo{};
    xPacket >> serverInfo;
    server->SetServerInfo(serverInfo);
    TXSingleton<XRelayServer>::Instance()->AddServerInfo(server);
    return true;
}

bool CServerProcess::ReqUpdateServerInfo(XPacket& xPacket) {
    CServer* server = GetClientPtr();
    if (!server) {
        return false;
    }

    SS_UPDATE_SERVER_INFO updateInfo{};
    xPacket >> updateInfo;
    // IDA 0x1400CEA50: 仅反序列化，不处理updateInfo（原版即如此）
    return true;
}

bool CServerProcess::SyncUsersInfo(XPacket& xPacket) {
    CServer* server = GetClientPtr();
    if (!server) {
        return false;
    }

    PS_USERS_INFO usersInfo{};
    xPacket >> usersInfo;
    TXSingleton<XRelayServer>::Instance()->SetUsersInfo(server, &usersInfo);
    return true;
}
