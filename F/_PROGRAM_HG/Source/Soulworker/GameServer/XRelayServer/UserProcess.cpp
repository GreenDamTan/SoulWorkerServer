#include "Soulworker/GameServer/XRelayServer/UserProcess.h"

#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

bool CUserProcess::Parse(XPacket& xPacket) {
    switch (xPacket.GetSubCmd()) {
    case eSUB_CMD_SERVER_USER_LOGIN:
        return SyncLoginUser(xPacket);
    case eSUB_CMD_SERVER_USER_LOGOUT:
        return SyncLogoutUser(xPacket);
    case eSUB_CMD_SERVER_USER_UPDATE_MAP:
        return SyncUpdateUserMap(xPacket);
    case 0x08:
        return SyncUserLevelup(xPacket);
    case 0x09:
        return ReqUserChatWhisper(xPacket);
    case 0x11:
        return ReqUserChatNotice(xPacket);
    case 0x17:
        return ReqUserChatMegaPhone(xPacket);
    case 0x33:
        return ReqUserOption(xPacket);
    case 0x36:
        return SyncUserAwaken(xPacket);
    case 0x38:
        return SyncUserProfilePhoto(xPacket);
    default:
        return false;
    }
}

bool CUserProcess::SyncLoginUser(XPacket& xPacket) {
    CServer* server = GetClientPtr();
    STCharInfo charInfo{};
    ST_GAME_OPTION gameOption{};
    UXMapID mapID{};
    xPacket >> charInfo;
    xPacket >> gameOption;
    return TXSingleton<XRelayServer>::Instance()->AddUser(server, charInfo, mapID, gameOption);
}

bool CUserProcess::SyncLogoutUser(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    int accountState = 0;
    bool kickAlreadyLogin = false;
    xPacket.XParse >> actorID;
    xPacket.XParse >> accountState;
    xPacket.XParse >> kickAlreadyLogin;
    TXSingleton<XRelayServer>::Instance()->RemoveUser(actorID, accountState, kickAlreadyLogin);
    return true;
}

bool CUserProcess::SyncUpdateUserMap(XPacket& xPacket) {
    CServer* server = GetClientPtr();
    PS_UPDATE_USER_MAP_INFO updateInfo{};
    xPacket >> updateInfo;
    TXSingleton<XRelayServer>::Instance()->UpdateUserMap(server, updateInfo);
    return true;
}

bool CUserProcess::SyncUserLevelup(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    std::uint8_t level = 0;
    xPacket.XParse >> actorID;
    xPacket.XParse >> level;
    TXSingleton<XRelayServer>::Instance()->UpdateUserLevelUp(actorID, level);
    return true;
}

bool CUserProcess::ReqUserChatWhisper(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    PS_CHAT_WHISPER chatWhisper{};
    PS_CHAT_ITEM_LINK_FOR_SERVER itemLinkInfo{};
    xPacket.XParse >> actorID;
    xPacket >> chatWhisper;
    xPacket >> itemLinkInfo;
    TXSingleton<XRelayServer>::Instance()->SendChatWhisper(actorID, chatWhisper, itemLinkInfo);
    return true;
}

bool CUserProcess::ReqUserChatNotice(XPacket& xPacket) {
    PS_CHAT_NOTICE notice{};
    xPacket >> notice;
    TXSingleton<XRelayServer>::Instance()->SendChatNotice(notice);
    return true;
}

bool CUserProcess::ReqUserChatMegaPhone(XPacket& xPacket) {
    PS_CHAT_MEGAPHONE megaPhone{};
    PS_CHAT_ITEM_LINK_FOR_SERVER itemLinkInfo{};
    xPacket >> megaPhone;
    xPacket >> itemLinkInfo;
    TXSingleton<XRelayServer>::Instance()->SendChatMegaPhone(megaPhone, itemLinkInfo);
    return true;
}

bool CUserProcess::ReqUserOption(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    ST_OPTION_BIT optionBit{};
    xPacket.XParse >> actorID;
    xPacket >> optionBit;

    const std::shared_ptr<CUserObject> userInfo = TXSingleton<XRelayServer>::Instance()->GetUser(actorID);
    if (!userInfo) {
        LogHelper::LogError("game.contents",
                            "<Find Fail> XRelayServer::ReqUserOption [%u]",
                            static_cast<unsigned int>(actorID));
        return true;
    }

    userInfo->SetGameOption(&optionBit);
    return true;
}

bool CUserProcess::SyncUserAwaken(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    std::uint8_t awaken = 0;
    xPacket.XParse >> actorID;
    xPacket.XParse >> awaken;
    TXSingleton<XRelayServer>::Instance()->UpdateUserAwaken(actorID, awaken);
    return true;
}

bool CUserProcess::SyncUserProfilePhoto(XPacket& xPacket) {
    std::uint32_t actorID = 0;
    std::uint32_t profilePhotoID = 0;
    xPacket.XParse >> actorID;
    xPacket.XParse >> profilePhotoID;
    TXSingleton<XRelayServer>::Instance()->UpdateUserProfilePhoto(actorID, profilePhotoID);
    return true;
}
