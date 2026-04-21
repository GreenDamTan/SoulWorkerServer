#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CServerWorldModeProcess : public TXProcess<CServer> {
public:
    explicit CServerWorldModeProcess(CServer* server = nullptr) {
        SetCmd(0xFB);
        SetName("CServerWorldModeProcess");
        Init(server);
    }

    bool Parse(XPacket& xPacket) override;
};
