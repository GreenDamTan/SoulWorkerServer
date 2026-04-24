#include "Soulworker/GameServer/XRelayServer/MonitorProcess.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

// 对齐 IDA 0x140093B70: Parse 按 sub-cmd 分派
bool CMonitorProcess::Parse(XPacket& xPacket) {
    if (xPacket.GetSubCmd() == 1) {
        return ReqMonitorServerInfoAll(xPacket);
    }
    return true;
}

// 对齐 IDA 0x140093BC0: 请求所有服务器信息
bool CMonitorProcess::ReqMonitorServerInfoAll(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    TXSingleton<XRelayServer>::Instance()->SendServerInfoAll(pServer);
    return true;
}
