#pragma once

#include <cstdint>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"
#include "Soulworker/GameServer/XCore/XServer/IXObject.h"

// 对齐 IDA: E_SERVER_SYNC_LOAD 枚举用于服务器同步加载状态
enum E_SERVER_SYNC_LOAD : std::uint32_t {
    E_SERVER_SYNC_LOAD_USER = 1u,        // 用户数据同步完成
    E_SERVER_SYNC_LOAD_MAZE_INFO = 2u,   // 迷宫信息同步完成
};

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

    void SetServerInfo(SS_SERVER_INFO& info) {  // 对齐 IDA: 非const引用 AEAU
        m_serverInfo = info;
        m_stSyncInfo.nSyncData = 0;
        if (info.nType != 1) {
            m_stSyncInfo.SyncData |= 1u;
        }
    }

    int GetServerType() {  // 对齐 IDA: QEAAHXZ 非const
        return m_serverInfo.nType;
    }

    std::uint32_t GetServerID() {  // 对齐 IDA: QEAAKXZ 非const
        return m_serverInfo.dwID;
    }

    void RecvUserInfo() {
        m_stSyncInfo.SyncData &= static_cast<std::uint8_t>(~1u);
    }

    // 对齐 IDA: SetSyncLoad(W4E_SERVER_SYNC_LOAD) = 枚举参数
    void SetSyncLoad(E_SERVER_SYNC_LOAD eServerSync);
    bool IsSyncLoad() {  // 对齐 IDA: QEAA_NXZ 非const
        return m_dwSyncLoad == (E_SERVER_SYNC_LOAD_USER | E_SERVER_SYNC_LOAD_MAZE_INFO);
    }
    // 对齐 IDA 0x1400BD5C0 UpdateServerState: 检查服务器信息是否已接收
    bool IsRecvServerInfo() {  // 对齐 IDA: QEAA_NXZ 非const
        return (m_stSyncInfo.SyncData & 1u) == 0u;
    }

    std::uint8_t GetBlockType() const {
        return 0;
    }

    // 对齐 IDA 0x1400C8A90: 获取服务器信息
    SS_SERVER_INFO& GetServerInfo() {
        return m_serverInfo;
    }
    const SS_SERVER_INFO& GetServerInfo() const {
        return m_serverInfo;
    }

    // 对齐 IDA 0x1400C8A70: 获取当前用户数
    int GetUserCount() {  // 对齐 IDA: QEAAHXZ 非const
        return m_serverInfo.nCurUser;
    }

private:
    SS_SERVER_INFO m_serverInfo{};
    ST_SYNC_INFO m_stSyncInfo{};
    std::uint32_t m_dwSyncLoad = 0;
};

class XRelayServer;

class CServerProcess : public TXProcess<CServer> {
public:
    // 对齐 IDA: 构造函数 QEAA@XZ 无参数，Init 不在构造中调用
    CServerProcess() {
        SetCmd(eCMD_SERVER);
        SetName("CServerProcess");
    }

    bool Parse(XPacket& xPacket) override;

protected:  // 对齐 IDA: 这些方法在 IDA 中是 protected (IEAA)
    bool ReqCreateServer(XPacket& xPacket);
    bool ReqUpdateServerInfo(XPacket& xPacket);
    bool SyncUsersInfo(XPacket& xPacket);
};
