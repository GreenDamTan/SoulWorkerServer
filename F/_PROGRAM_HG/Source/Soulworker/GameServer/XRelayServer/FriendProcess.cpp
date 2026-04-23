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
    PS_RES_FRIEND_INVITE stInvite{};
    xPacket >> stInvite;
    TXSingleton<XRelayServer>::Instance()->PrepareFriendInvite(stInvite);
    return true;
}

bool CFriendProcess::ReqFriendAccept(XPacket& xPacket) {
    // 对齐 IDA 0x1400407D0: PS_REQ_FRIEND_ACCEPT>> + PrepareFriendAccept
    PS_REQ_FRIEND_ACCEPT stAccept{};
    xPacket >> stAccept;
    TXSingleton<XRelayServer>::Instance()->PrepareFriendAccept(stAccept);
    return true;
}

bool CFriendProcess::ReqFriendDelete(XPacket& xPacket) {
    // 对齐 IDA 0x140040690: PS_REQ_FRIEND_DELETE>> + PrepareDeleteFriend
    PS_REQ_FRIEND_DELETE stDelete{};
    xPacket >> stDelete;
    TXSingleton<XRelayServer>::Instance()->PrepareDeleteFriend(stDelete);
    return true;
}

bool CFriendProcess::ReqBlockListAdd(XPacket& xPacket) {
    // 对齐 IDA 0x140040830: PS_REQ_FRIEND_BLOCK_ADD>> + PrepareBlockListAdd
    PS_REQ_FRIEND_BLOCK_ADD stBlock{};
    xPacket >> stBlock;
    TXSingleton<XRelayServer>::Instance()->PrepareBlockListAdd(stBlock);
    return true;
}

bool CFriendProcess::ReqBlockListDelete(XPacket& xPacket) {
    // 对齐 IDA 0x140040890: PS_REQ_FRIEND_BLOCK_DELETE>> + PrepareBlockListDel
    PS_REQ_FRIEND_BLOCK_DELETE stBlock{};
    xPacket >> stBlock;
    TXSingleton<XRelayServer>::Instance()->PrepareBlockListDel(stBlock);
    return true;
}

bool CFriendProcess::ReqFriendRecommand(XPacket& xPacket) {
    // 对齐 IDA 0x1400408F0: PS_RES_FRIEND_RECOMMAND>> + RecommandFriend
    PS_RES_FRIEND_RECOMMAND stRecommand{};
    xPacket >> stRecommand;
    TXSingleton<XRelayServer>::Instance()->RecommandFriend(stRecommand);
    return true;
}

bool CFriendProcess::ReqFriendRecruitList(XPacket& xPacket) {
    // 对齐 IDA 0x140040970: PS_REQ_RECRUIT_LIST>> + GetClientPtr + DoJob(2, lambda)
    CServer* server = GetClientPtr();
    PS_REQ_RECRUIT_LIST stList{};
    xPacket >> stList;
    TXSingleton<XRelayServer>::Instance()->SendRecruitList(server, stList);
    return true;
}

bool CFriendProcess::ReqFriendRecruitAdd(XPacket& xPacket) {
    // 对齐 IDA 0x140040AD0: ST_RECRUIT_INFO>> + DoJob(2, lambda)
    ST_RECRUIT_INFO stAdd{};
    xPacket >> stAdd;
    TXSingleton<XRelayServer>::Instance()->SendRecruitAdd(stAdd);
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
    // 对齐 IDA 0x140040C30: XParse>>dwUCID + DoJob(2, lambda) → PrepareRecruitInfo
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> dwUCID;
    return CLogicThreadManager::Instance().DoJob(2, [dwUCID]() {
        TXSingleton<XRelayServer>::Instance()->PrepareRecruitInfo(dwUCID);
    });
}

bool CFriendProcess::ReqUpdateFriendCommunity(XPacket& xPacket) {
    // 对齐 IDA 0x1400406D0: XParse>>dwActorID + ST_CHAR_COMMUNITY>> + UpdateFriendCommunity
    std::uint32_t dwActorID = 0;
    ST_CHAR_COMMUNITY stCommunity{};
    xPacket.XParse >> dwActorID;
    xPacket >> stCommunity;
    TXSingleton<XRelayServer>::Instance()->UpdateFriendCommunity(dwActorID, stCommunity);
    return true;
}

bool CFriendProcess::ReqFriendFind(XPacket& xPacket) {
    // 对齐 IDA 0x140040CF0: PS_REQ_FRIEND_FIND>> + ReqFriendFind
    PS_REQ_FRIEND_FIND stFind{};
    xPacket >> stFind;
    TXSingleton<XRelayServer>::Instance()->ReqFriendFind(stFind);
    return true;
}

bool CFriendProcess::ReqCheckDailyMissionFirend(XPacket& xPacket) {
    // 对齐 IDA 0x140040D50: PS_DAILY_MISSION_FRIEND_REQ>> + DailyMissionFriendReq
    PS_DAILY_MISSION_FRIEND_REQ psMission{};
    xPacket >> psMission;
    TXSingleton<XRelayServer>::Instance()->DailyMissionFriendReq(psMission);
    return true;
}

bool CFriendProcess::ResCheckDailyMissionFirend(XPacket& xPacket) {
    // 对齐 IDA 0x140040DD0: PS_DAILY_MISSION_FRIEND_RES>> + DailyMissionFriendRes
    PS_DAILY_MISSION_FRIEND_RES psMission{};
    xPacket >> psMission;
    TXSingleton<XRelayServer>::Instance()->DailyMissionFriendRes(psMission);
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
    PS_SERVER_HELPER_SUPPORT_REGISTER psSupport{};
    xPacket >> psSupport;
    TXSingleton<XRelayServer>::Instance()->HelperSupportRegister(psSupport);
    return true;
}

bool CFriendProcess::ReqHelperSupportReward(XPacket& xPacket) {
    // 对齐 IDA 0x140040EF0: PS_SERVER_HELPER_SUPPORT_REWARD>> + HelperSupportReward
    PS_SERVER_HELPER_SUPPORT_REWARD psReward{};
    xPacket >> psReward;
    TXSingleton<XRelayServer>::Instance()->HelperSupportReward(psReward);
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
    PS_HELPER_SUPPORT_EQUIP_REQ psEquip{};
    xPacket >> psEquip;
    TXSingleton<XRelayServer>::Instance()->HelperSupportEquip(psEquip);
    return true;
}
