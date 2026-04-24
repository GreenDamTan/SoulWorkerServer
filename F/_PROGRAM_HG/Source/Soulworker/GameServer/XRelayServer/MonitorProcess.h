#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

// 对齐 IDA: CMonitorProcess 用于处理监控子命令 (主命令 0xF7)
class CMonitorProcess : public TXProcess<CServer> {
public:
    // 对齐 IDA: 构造函数 QEAA@XZ 无参数
    CMonitorProcess() {
        SetCmd(0xF7);
        SetName("CMonitorProcess");
    }

    // 对齐 IDA: UEAA_NAEAVXPacket@@@Z = public virtual
    bool Parse(XPacket& xPacket) override;

    // 对齐 IDA: QEAA_NAEAVXPacket@@@Z = public 非const
    bool ReqMonitorServerInfoAll(XPacket& xPacket);
};
