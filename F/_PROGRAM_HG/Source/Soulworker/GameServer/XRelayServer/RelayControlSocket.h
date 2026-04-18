#pragma once

#include <cstdint>
#include <cstring>
#include <functional>

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

class CUserProcess;

class XRelaySocket : public XIOCPClient {
public:
    struct RelayInfo {
        char szName[21] = {};
        char szPrivateIP[513] = {};
        std::int16_t sPort = 0;
        int nState = 0;
    };

    ~XRelaySocket() override = default;

    bool Init(E_POOL_ID poolId, const char* ip, std::uint16_t port) override;
    virtual void SetMyInfo(const XOption* option);
    bool Connect() override;

    void OnConnect() override;
    void OnDisConnect() override;
    void OnNotConnect() override;
    bool OnParse(XPacket& xPacket) override;

    bool ServerProcess(XPacket& xPacket);
    bool UserProcess(XPacket& xPacket);
    virtual bool ServerProcessEx(XPacket& xPacket);
    void OnStartThread();

    bool RecvServerUpdate(XPacket& xPacket);
    void RecvPacketFromRelay(XPacket& xPacket);
    bool RecvChangeChannelRes(XPacket& xPacket);
    bool RecvUpdateChannelAll(XPacket& xPacket);
    bool RecvUpdateChannel(XPacket& xPacket);
    void SendAddServer();
    void SendUpdateServerInfo(std::int16_t nState, int nUserCount);
    bool IsReady() const;

protected:
    SS_SERVER_INFO m_myInfo{};
    RelayInfo m_relayInfo{};
    int m_nSyncServerData = 0;
};

class CRelayControlSocket : public XRelaySocket {
public:
    void SetMyInfo(const XOption* option) override;
    bool ServerProcessEx(XPacket& xPacket) override;

    bool ResCreateMatchingMaze(XPacket& xPacket);
    bool SyncPartyMazeInfo(XPacket& xPacket);
    bool SyncForceMazeInfo(XPacket& xPacket);
    bool ResCreateMatchingModeMaze(XPacket& xPacket);

private:
    bool DispatchLogicJob(std::int64_t instanceID, const std::function<void()>& job);
};
