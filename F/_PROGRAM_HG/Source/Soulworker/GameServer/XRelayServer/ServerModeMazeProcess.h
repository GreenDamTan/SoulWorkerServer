#pragma once

#include <cstdint>

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CServerModeMazeProcess : public TXProcess<CServer> {
public:
    // 对齐 IDA: 构造函数 QEAA@XZ 无参数
    CServerModeMazeProcess() {
        SetCmd(0xFD);
        SetName("CServerModeMazeProcess");
    }

    bool Parse(XPacket& xPacket) override;

protected:  // 对齐 IDA: 这些方法在 IDA 中是 protected (IEAA)
    bool ReqServerModeMazeEnter(XPacket& xPacket);
    bool ReqServerModeMazeExit(XPacket& xPacket);
    bool ReqServerModeMazeTime_Cheat(XPacket& xPacket);
    bool ReqServerModeMazeMatchingEvent(XPacket& xPacket);
};
