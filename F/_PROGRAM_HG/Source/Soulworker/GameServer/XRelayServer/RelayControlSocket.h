#pragma once

#include <cstdint>
#include <cstring>
#include <functional>
#include <map>

#include "Soulworker/Common/XNet/XCommon/PSCommon.h"
#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

class CUserProcess;
class CServer;

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

    // ServerProcess 子命令处理（main=242）
    bool RecvServerUpdate(XPacket& xPacket);
    virtual void RecvPacketFromRelay(XPacket& xPacket);
    virtual bool RecvChangeChannelRes(XPacket& xPacket);
    virtual bool RecvUpdateChannelAll(XPacket& xPacket);
    virtual bool RecvUpdateChannel(XPacket& xPacket);

    // UserProcess 子命令处理（main=243）
    virtual bool RecvUserKickout(XPacket& xPacket);
    virtual bool RecvUserWhisperRes(XPacket& xPacket);
    virtual bool RecvUserNotice(XPacket& xPacket);
    virtual bool RecvUserChangeServer(XPacket& xPacket);
    virtual bool RecvUserEnterServer(XPacket& xPacket);
    virtual bool RecvUserMegaPhone(XPacket& xPacket);
    virtual bool RecvUserTradePasswordState(XPacket& xPacket);
    virtual bool RecvExchangePriceHistory(XPacket& xPacket);
    virtual bool RecvExchangePost(XPacket& xPacket);
    virtual bool RecvCheckSessionID(XPacket& xPacket);
    virtual bool RecvGFBillingPostReload(XPacket& xPacket);

    void SendAddServer();
    void SendUpdateServerInfo(std::int16_t nState, int nUserCount);
    bool IsReady() const;

protected:
    SS_SERVER_INFO m_myInfo{};
    RelayInfo m_relayInfo{};
    int m_nSyncServerData = 0;
    std::map<std::uint32_t, CServer*> m_mapChannelInfo;  // 对齐 IDA 析构函数中的 ~map
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
