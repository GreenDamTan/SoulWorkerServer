#pragma once

#include <cstdint>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XCore/XServer/IXObject.h"

struct ST_SYNC_INFO {
    union {
        std::uint32_t nSyncData = 0;
        struct {
            std::uint8_t SyncData;
            std::uint8_t Reserved0;
            std::uint8_t Reserved1;
            std::uint8_t Reserved2;
        };
    };
};

class CServer : public XClient {
public:
    CServer() = default;

    void SetServerInfo(const SS_SERVER_INFO& info) {
        m_serverInfo = info;
        m_stSyncInfo.nSyncData = 0;
        if (info.nType != 1) {
            m_stSyncInfo.SyncData |= 1u;
        }
    }

    int GetServerType() const {
        return m_serverInfo.nType;
    }

    std::uint32_t GetServerID() const {
        return m_serverInfo.dwID;
    }

    void RecvUserInfo() {
        m_stSyncInfo.SyncData &= static_cast<std::uint8_t>(~1u);
    }

    void SetSyncLoad(std::uint32_t syncMask);
    bool IsSyncLoad() const {
        return m_dwSyncLoad == 3u;
    }
    // 对齐 IDA 0x1400BD5C0 UpdateServerState: 检查服务器信息是否已接收
    bool IsRecvServerInfo() const {
        return (m_stSyncInfo.SyncData & 1u) == 0u;
    }

    std::uint8_t GetBlockType() const {
        return 0;
    }

private:
    SS_SERVER_INFO m_serverInfo{};
    ST_SYNC_INFO m_stSyncInfo{};
    std::uint32_t m_dwSyncLoad = 0;
};

class XRelayServer;

class CServerProcess : public TXProcess<CServer> {
public:
    explicit CServerProcess(CServer* server = nullptr) {
        SetCmd(eCMD_SERVER);
        SetName("CServerProcess");
        Init(server);
    }

    bool Parse(XPacket& xPacket) override;
    bool ReqCreateServer(XPacket& xPacket);
    bool ReqUpdateServerInfo(XPacket& xPacket);
    bool SyncUsersInfo(XPacket& xPacket);
};
