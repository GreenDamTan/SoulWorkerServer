#pragma once

#include <cstdint>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CServerModeMazeProcess : public TXProcess<CServer> {
public:
    explicit CServerModeMazeProcess(CServer* server = nullptr) {
        SetCmd(0xFD);
        SetName("CServerModeMazeProcess");
        Init(server);
    }

    bool Parse(XPacket& xPacket) override;
    bool ReqServerModeMazeEnter(XPacket& xPacket);
    bool ReqServerModeMazeExit(XPacket& xPacket);
    bool ReqServerModeMazeTime_Cheat(XPacket& xPacket);
    bool ReqServerModeMazeMatchingEvent(XPacket& xPacket);
};
