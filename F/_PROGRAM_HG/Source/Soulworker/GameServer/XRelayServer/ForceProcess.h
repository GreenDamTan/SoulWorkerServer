#pragma once

#include <cstdint>
#include <functional>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ForceManager.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CForceProcess : public TXProcess<CServer> {
public:
    explicit CForceProcess(CServer* server = nullptr, CForceManager* forceManager = nullptr) :
        m_pForceManager(forceManager) {
        SetCmd(0xFA);
        SetName("CForceProcess");
        Init(server);
    }

    bool Parse(XPacket& xPacket) override;
    bool ReqForceCreate(XPacket& xPacket);
    bool ReqForceLeaveMember(XPacket& xPacket);
    bool ReqForceChangeMaster(XPacket& xPacket);
    bool ReqForceUpdateMember(XPacket& xPacket);
    bool ReqForceEnterServer(XPacket& xPacket);
    bool ReqForceInvite(XPacket& xPacket);
    bool ReqForceAccept(XPacket& xPacket);
    bool ReqForceCancel(XPacket& xPacket);
    bool SyncForceMessage(XPacket& xPacket);
    bool ReqForceMatchingEnter(XPacket& xPacket);
    bool ReqForceMatchingExit(XPacket& xPacket);
    bool ReqForceMatchingCheck(XPacket& xPacket);
    bool ReqForceMazeClear(XPacket& xPacket);
    bool ReqForceInfo(XPacket& xPacket);
    bool ReqForceChangeMazeOpenCheck(XPacket& xPacket);

private:
    bool DispatchForceJob(const std::function<void()>& job);

    CForceManager* m_pForceManager = nullptr;
};
