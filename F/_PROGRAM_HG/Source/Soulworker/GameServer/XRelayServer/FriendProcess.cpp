#include "Soulworker/GameServer/XRelayServer/FriendProcess.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

bool CFriendProcess::Parse(XPacket& xPacket) {
    switch (static_cast<unsigned char>(xPacket.GetSubCmd())) {
    case 0x01:
        return ReqFriendListLoad(xPacket);
    case 0x02:
        return ReqBlockListLoad(xPacket);
    case 0x18:
        return ReqFriendRecruitDelete(xPacket);
    default:
        return true;
    }
}

bool CFriendProcess::ReqFriendListLoad(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    xPacket.XParse >> actorID;
    TXSingleton<XRelayServer>::Instance()->SendFriendList(actorID);
    return true;
}

bool CFriendProcess::ReqBlockListLoad(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    xPacket.XParse >> actorID;
    TXSingleton<XRelayServer>::Instance()->SendBlockList(actorID);
    return true;
}

bool CFriendProcess::ReqFriendRecruitDelete(XPacket& xPacket) {
    PS_RECRUIT_DELETE deleteInfo{};
    xPacket >> deleteInfo;
    return CLogicThreadManager::Instance().DoJob(2, [deleteInfo]() {
        TXSingleton<XRelayServer>::Instance()->PrepareDeleteRecruit(deleteInfo);
    });
}
