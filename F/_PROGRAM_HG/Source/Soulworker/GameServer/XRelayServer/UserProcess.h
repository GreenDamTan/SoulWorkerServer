#pragma once

#include "Soulworker/Common/XNet/XCommon/PSServer.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

class CUserProcess : public TXProcess<CServer> {
public:
    explicit CUserProcess(CServer* server = nullptr) {
        SetCmd(0xF3);
        SetName("CUserProcess");
        Init(server);
    }

    bool Parse(XPacket& xPacket) override;
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
