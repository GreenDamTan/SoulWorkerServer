// UserProcess.h - ControlServer
// 对齐 IDA ControlServer.exe: CUserProcess 用户包处理器

#pragma once

#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "CServer.h"

// 前向声明
class XControlServer;

// 对齐 IDA ControlServer.exe 0x140044FF0: CUserProcess
class CUserProcess : public TXProcess<CServer> {
public:
    // 对齐 IDA: 构造函数 QEAA@XZ 无参数
    CUserProcess() {
        SetCmd(0xF3);  // 对齐 IDA: main command 0xF3
        SetName("CUserProcess");
    }

    // 对齐 IDA 0x1400450E0: Parse - 包解析入口
    bool Parse(XPacket& xPacket) override;

protected:  // 对齐 IDA: 这些方法在 IDA 中是 protected (IEAA)
    // 对齐 IDA 0x140045310: SyncSelectCharacter (sub 0x01)
    bool SyncSelectCharacter(XPacket& xPacket);

    // 对齐 IDA 0x1400454B0: SyncLogoutUser (sub 0x03)
    bool SyncLogoutUser(XPacket& xPacket);

    // 对齐 IDA 0x140045570: SyncUpdateUserMap (sub 0x04)
    bool SyncUpdateUserMap(XPacket& xPacket);

    // 对齐 IDA 0x1400455D0: SyncUserKickout (sub 0x07)
    bool SyncUserKickout(XPacket& xPacket);

    // 对齐 IDA 0x140045640: ReqUserChatNotice (sub 0x11)
    bool ReqUserChatNotice(XPacket& xPacket);

    // 对齐 IDA 0x1400456B0: ReqUserChatMegaPhone (sub 0x17)
    bool ReqUserChatMegaPhone(XPacket& xPacket);

    // 对齐 IDA 0x140045A10: ReqUserChangeServer (sub 0x12)
    bool ReqUserChangeServer(XPacket& xPacket);

    // 对齐 IDA 0x140045A80: SyncUserMoneyLog (sub 0x13)
    bool SyncUserMoneyLog(XPacket& xPacket);

    // 对齐 IDA 0x140045AC0: ReqUserEnterPartyMaze (sub 0x16)
    bool ReqUserEnterPartyMaze(XPacket& xPacket);

    // 对齐 IDA 0x140045B70: ReqUserEnterForceMaze (sub 0x20)
    bool ReqUserEnterForceMaze(XPacket& xPacket);

    // 对齐 IDA 0x140045C30: ReqUserTradePasswordStateSync (sub 0x26)
    bool ReqUserTradePasswordStateSync(XPacket& xPacket);

    // 对齐 IDA 0x140045CB0: ReqUserTradePasswordState (sub 0x27)
    bool ReqUserTradePasswordState(XPacket& xPacket);

    // 对齐 IDA 0x140045D10: ReqNameChange (sub 0x31)
    bool ReqNameChange(XPacket& xPacket);

    // 对齐 IDA 0x140045D70: ReqCheckSessionID (sub 0x32)
    bool ReqCheckSessionID(XPacket& xPacket);

    // 对齐 IDA 0x140045E00: ReqGameServerEnterUser (sub 0x60)
    bool ReqGameServerEnterUser(XPacket& xPacket);

    // 对齐 IDA 0x140045F40: ReqUserUpdateAuthType (sub 0x35)
    bool ReqUserUpdateAuthType(XPacket& xPacket);
};