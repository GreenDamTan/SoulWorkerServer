#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CUserProcess : public TXProcess<CServer> {
public:
    // 对齐 IDA: 构造函数 QEAA@XZ 无参数
    CUserProcess() {
        SetCmd(0xF3);
        SetName("CUserProcess");
    }

    bool Parse(XPacket& xPacket) override;

protected:  // 对齐 IDA: 这些方法在 IDA 中是 protected (IEAA)
    bool SyncLoginUser(XPacket& xPacket);
    bool SyncLogoutUser(XPacket& xPacket);
    bool SyncUpdateUserMap(XPacket& xPacket);
    bool SyncUserLevelup(XPacket& xPacket);
    bool ReqUserChatWhisper(XPacket& xPacket);
    bool ReqUserChatNotice(XPacket& xPacket);
    bool ReqUserChatMegaPhone(XPacket& xPacket);
    bool ReqExchangePriceList(XPacket& xPacket);
    bool ReqExchangePriceUpdate(XPacket& xPacket);
    bool ReqNameChange(XPacket& xPacket);
    bool ReqUserOption(XPacket& xPacket);
    bool ReqMyRoomPollenSync(XPacket& xPacket);
    bool SyncUserAwaken(XPacket& xPacket);
    bool SyncUserProfilePhoto(XPacket& xPacket);
};
