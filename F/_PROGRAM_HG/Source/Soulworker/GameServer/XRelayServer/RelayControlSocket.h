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

    // 对齐 IDA 0x1400FDF90: Init(W4E_POOL_ID, PEAD, G) = (enum, char*, unsigned short)
    // 注意：IDA 显示 PEAD = char*，但基类使用 const char*，保持与基类一致
    bool Init(E_POOL_ID poolId, const char* ip, std::uint16_t port) override;
    // 对齐 IDA 0x1400FE050: SetMyInfo(PEAVXOption) = 非const指针
    // 对齐 IDA: UEAA = public virtual
    virtual void SetMyInfo(XOption* option);
    // 对齐 IDA 0x1400FE180: QEAA = public
    bool Connect() override;

    // 对齐 IDA 0x1400FE7E0: UEAA = public virtual
    virtual bool RecvServerUpdate(XPacket& xPacket);
    // 对齐 IDA 0x1400FE810: UEAA = public virtual
    virtual void OnStartThread();

    // 对齐 IDA 0x1400FE620: QEAA = public
    void SendAddServer();
    // 对齐 IDA 0x1400FE6E0: SendUpdateServerInfo(H, H) = (int, int), QEAA = public
    void SendUpdateServerInfo(int nState, int nUserCount);
    bool IsReady() const;

protected:
    // 对齐 IDA: MEAA = protected (这些是从基类override的保护方法)
    void OnConnect() override;
    void OnDisConnect() override;
    void OnNotConnect() override;
    bool OnParse(XPacket& xPacket) override;
    // 对齐 IDA: MEAA = protected
    bool ServerProcess(XPacket& xPacket);
    bool UserProcess(XPacket& xPacket);
    // 对齐 IDA: UEAA = public virtual (在派生类中可能override)
    virtual bool ServerProcessEx(XPacket& xPacket);

    // 虚拟方法 (未在IDA中验证，保持原设计)
    virtual void RecvPacketFromRelay(XPacket& xPacket);
    virtual bool RecvChangeChannelRes(XPacket& xPacket);
    virtual bool RecvUpdateChannelAll(XPacket& xPacket);
    virtual bool RecvUpdateChannel(XPacket& xPacket);
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

    SS_SERVER_INFO m_myInfo{};
    RelayInfo m_relayInfo{};
    int m_nSyncServerData = 0;
    std::map<std::uint32_t, CServer*> m_mapChannelInfo;  // 对齐 IDA 析构函数中的 ~map
};

class CRelayControlSocket : public XRelaySocket {
public:
    // 对齐 IDA: SetMyInfo(PEAVXOption) = 非const指针
    void SetMyInfo(XOption* option) override;
    bool ServerProcessEx(XPacket& xPacket) override;

    bool ResCreateMatchingMaze(XPacket& xPacket);
    bool SyncPartyMazeInfo(XPacket& xPacket);
    bool SyncForceMazeInfo(XPacket& xPacket);
    bool ResCreateMatchingModeMaze(XPacket& xPacket);

private:
    bool DispatchLogicJob(std::int64_t instanceID, const std::function<void()>& job);
};
