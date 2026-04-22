#include "Soulworker/GameServer/XRelayServer/FriendProcess.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

bool CFriendProcess::Parse(XPacket& xPacket) {
    // 对齐 IDA 0x140040370: 完整 sub switch
    switch (static_cast<unsigned char>(xPacket.GetSubCmd())) {
    case 0x01:
        return ReqFriendListLoad(xPacket);
    case 0x02:
        return ReqBlockListLoad(xPacket);
    case 0x03:
        return ReqFriendInvite(xPacket);
    case 0x04:
        return ReqFriendAccept(xPacket);
    case 0x05:
        return ReqFriendDelete(xPacket);
    case 0x07:
        return ReqBlockListAdd(xPacket);
    case 0x08:
        return ReqBlockListDelete(xPacket);
    case 0x11:
        return ReqFriendRecommand(xPacket);
    case 0x15:
        return ReqFriendRecruitList(xPacket);
    case 0x16:
        return ReqFriendRecruitAdd(xPacket);
    case 0x17:
        return ReqFriendRecruitDelete(xPacket);
    case 0x18:
        return ReqFriendRecruitInfo(xPacket);
    case 0x21:
        return ReqUpdateFriendCommunity(xPacket);
    case 0x22:
        return ReqFriendFind(xPacket);
    case 0x25:
        return ReqCheckDailyMissionFirend(xPacket);
    case 0x26:
        return ResCheckDailyMissionFirend(xPacket);
    case 0x27:
        return ReqHelperSupportInfo(xPacket);
    case 0x28:
        return ReqHelperSupportRegister(xPacket);
    case 0x29:
        return ReqHelperSupportReward(xPacket);
    case 0x30:
        return ReqHelperSupportList(xPacket);
    case 0x31:
        return ReqHelperSupportEquip(xPacket);
    default:
        return true;
    }
}

bool CFriendProcess::ReqFriendListLoad(XPacket& xPacket) {
    // 对齐 IDA 0x1400405F0
    std::uint32_t actorID = 0;
    xPacket.XParse >> actorID;
    TXSingleton<XRelayServer>::Instance()->SendFriendList(actorID);
    return true;
}

bool CFriendProcess::ReqBlockListLoad(XPacket& xPacket) {
    // 对齐 IDA 0x140040640
    std::uint32_t actorID = 0;
    xPacket.XParse >> actorID;
    TXSingleton<XRelayServer>::Instance()->SendBlockList(actorID);
    return true;
}

bool CFriendProcess::ReqFriendInvite(XPacket& xPacket) {
    // 对齐 IDA 0x140040760: PS_RES_FRIEND_INVITE>> + PrepareFriendInvite
    // TODO: 定义 PS_RES_FRIEND_INVITE 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->PrepareFriendInvite(nullptr);
    return true;
}

bool CFriendProcess::ReqFriendAccept(XPacket& xPacket) {
    // 对齐 IDA 0x1400407D0: PS_REQ_FRIEND_ACCEPT>> + PrepareFriendAccept
    // TODO: 定义 PS_REQ_FRIEND_ACCEPT 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->PrepareFriendAccept(nullptr);
    return true;
}

bool CFriendProcess::ReqFriendDelete(XPacket& xPacket) {
    // 对齐 IDA 0x140040690: PS_REQ_FRIEND_DELETE>> + PrepareDeleteFriend
    // TODO: 定义 PS_REQ_FRIEND_DELETE 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->PrepareDeleteFriend(nullptr);
    return true;
}

bool CFriendProcess::ReqBlockListAdd(XPacket& xPacket) {
    // 对齐 IDA 0x140040830: PS_REQ_FRIEND_BLOCK_ADD>> + PrepareBlockListAdd
    // TODO: 定义 PS_REQ_FRIEND_BLOCK_ADD 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->PrepareBlockListAdd(nullptr);
    return true;
}

bool CFriendProcess::ReqBlockListDelete(XPacket& xPacket) {
    // 对齐 IDA 0x140040890: PS_REQ_FRIEND_BLOCK_DELETE>> + PrepareBlockListDel
    // TODO: 定义 PS_REQ_FRIEND_BLOCK_DELETE 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->PrepareBlockListDel(nullptr);
    return true;
}

bool CFriendProcess::ReqFriendRecommand(XPacket& xPacket) {
    // 对齐 IDA 0x1400408F0: PS_RES_FRIEND_RECOMMAND>> + RecommandFriend
    // TODO: 定义 PS_RES_FRIEND_RECOMMAND 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->RecommandFriend(nullptr);
    return true;
}

bool CFriendProcess::ReqFriendRecruitList(XPacket& xPacket) {
    // 对齐 IDA 0x140040970: PS_RECRUIT_LIST>> + GetClientPtr + DoJob(2, lambda)
    // TODO: 定义 PS_RECRUIT_LIST 后完善反序列化
    CServer* server = GetClientPtr();
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->SendRecruitList(server, nullptr);
    return true;
}

bool CFriendProcess::ReqFriendRecruitAdd(XPacket& xPacket) {
    // 对齐 IDA 0x140040AD0: PS_RECRUIT_ADD>> + DoJob(2, lambda)
    // TODO: 定义 PS_RECRUIT_ADD 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->SendRecruitAdd(nullptr);
    return true;
}

bool CFriendProcess::ReqFriendRecruitDelete(XPacket& xPacket) {
    // 对齐 IDA 0x1400404B0 (Parse switch entry) → 0x1400B8930 PrepareDeleteRecruit
    PS_RECRUIT_DELETE deleteInfo{};
    xPacket >> deleteInfo;
    return CLogicThreadManager::Instance().DoJob(2, [deleteInfo]() {
        TXSingleton<XRelayServer>::Instance()->PrepareDeleteRecruit(deleteInfo);
    });
}

bool CFriendProcess::ReqFriendRecruitInfo(XPacket& xPacket) {
    // 对齐 IDA 0x140040C30: XParse>>dwUCID + DoJob(2, lambda)
    // TODO: 完善 DoJob lambda
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> dwUCID;
    TXSingleton<XRelayServer>::Instance()->SendRecruitInfo(dwUCID);
    return true;
}

bool CFriendProcess::ReqUpdateFriendCommunity(XPacket& xPacket) {
    // 对齐 IDA 0x1400406D0: XParse>>dwActorID + ST_CHAR_COMMUNITY>> + UpdateFriendCommunity
    // TODO: 定义 ST_CHAR_COMMUNITY 后完善反序列化
    std::uint32_t dwActorID = 0;
    xPacket.XParse >> dwActorID;
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->UpdateFriendCommunity(dwActorID, nullptr);
    return true;
}

bool CFriendProcess::ReqFriendFind(XPacket& xPacket) {
    // 对齐 IDA 0x140040CF0: PS_REQ_FRIEND_FIND>> + ReqFriendFind
    // TODO: 定义 PS_REQ_FRIEND_FIND 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->ReqFriendFind(nullptr);
    return true;
}

bool CFriendProcess::ReqCheckDailyMissionFirend(XPacket& xPacket) {
    // 对齐 IDA 0x140040D50: PS_DAILY_MISSION_FRIEND_REQ>> + DailyMissionFriendReq
    // TODO: 定义 PS_DAILY_MISSION_FRIEND_REQ 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->DailyMissionFriendReq(nullptr);
    return true;
}

bool CFriendProcess::ResCheckDailyMissionFirend(XPacket& xPacket) {
    // 对齐 IDA 0x140040DD0: PS_DAILY_MISSION_FRIEND_RES>> + DailyMissionFriendRes
    // TODO: 定义 PS_DAILY_MISSION_FRIEND_RES 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->DailyMissionFriendRes(nullptr);
    return true;
}

bool CFriendProcess::ReqHelperSupportInfo(XPacket& xPacket) {
    // 对齐 IDA 0x140040E60: XParse>>dwUCID + HelperSupportInfo
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> dwUCID;
    TXSingleton<XRelayServer>::Instance()->HelperSupportInfo(dwUCID);
    return true;
}

bool CFriendProcess::ReqHelperSupportRegister(XPacket& xPacket) {
    // 对齐 IDA 0x140040EB0: PS_SERVER_HELPER_SUPPORT_REGISTER>> + HelperSupportRegister
    // TODO: 定义 PS_SERVER_HELPER_SUPPORT_REGISTER 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->HelperSupportRegister(nullptr);
    return true;
}

bool CFriendProcess::ReqHelperSupportReward(XPacket& xPacket) {
    // 对齐 IDA 0x140040EF0: PS_SERVER_HELPER_SUPPORT_REWARD>> + HelperSupportReward
    // TODO: 定义 PS_SERVER_HELPER_SUPPORT_REWARD 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->HelperSupportReward(nullptr);
    return true;
}

bool CFriendProcess::ReqHelperSupportList(XPacket& xPacket) {
    // 对齐 IDA 0x140040F70: XParse>>dwUCID + HelperSupportList
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> dwUCID;
    TXSingleton<XRelayServer>::Instance()->HelperSupportList(dwUCID);
    return true;
}

bool CFriendProcess::ReqHelperSupportEquip(XPacket& xPacket) {
    // 对齐 IDA 0x140040FC0: PS_HELPER_SUPPORT_EQUIP_REQ>> + HelperSupportEquip
    // TODO: 定义 PS_HELPER_SUPPORT_EQUIP_REQ 后完善反序列化
    static_cast<void>(xPacket);
    TXSingleton<XRelayServer>::Instance()->HelperSupportEquip(nullptr);
    return true;
}
