#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CServerWorldModeProcess : public TXProcess<CServer> {
public:
    // 注意：此类不存在于 IDA，为重构辅助类
    CServerWorldModeProcess() {
        SetCmd(0xFB);
        SetName("CServerWorldModeProcess");
    }

    bool Parse(XPacket& xPacket) override;
};
