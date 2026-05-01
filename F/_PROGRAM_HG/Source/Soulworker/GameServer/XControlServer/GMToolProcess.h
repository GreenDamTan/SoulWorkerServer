// GMToolProcess.h - ControlServer
// 对齐 IDA ControlServer.exe: CGMToolProcess GM工具包处理器

#pragma once

#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "CServer.h"

// 前向声明
class XControlServer;

// 对齐 IDA ControlServer.exe: CGMToolProcess
class CGMToolProcess : public TXProcess<CServer> {
public:
    CGMToolProcess() {
        SetCmd(0xF8);  // 对齐 IDA: main command 0xF8
        SetName("CGMToolProcess");
    }
    virtual ~CGMToolProcess() = default;

    // 对齐 IDA 0x140034640: Parse - 包解析入口
    bool Parse(XPacket& xPacket) override;

protected:
    // 对齐 IDA 0x140034730: ReqGMUserKick (sub 0x01)
    bool ReqGMUserKick(XPacket& xPacket);

    // 对齐 IDA 0x1400347C0: ReqGMNotice (sub 0x02)
    bool ReqGMNotice(XPacket& xPacket);

    // 对齐 IDA 0x140034850: ReqGMShutDwon (sub 0x03)
    bool ReqGMShutDwon(XPacket& xPacket);

    // 对齐 IDA 0x1400348B0: ReqGMTimeEvent (sub 0x04)
    bool ReqGMTimeEvent(XPacket& xPacket);

    // 对齐 IDA 0x140034910: ReqGMValueEvent (sub 0x05)
    bool ReqGMValueEvent(XPacket& xPacket);

    // 对齐 IDA 0x1400349A0: ReqGMServerOption (sub 0x06)
    bool ReqGMServerOption(XPacket& xPacket);

    // 对齐 IDA 0x140034A10: ReqGMCashShopBanner (sub 0x07)
    bool ReqGMCashShopBanner(XPacket& xPacket);
};
