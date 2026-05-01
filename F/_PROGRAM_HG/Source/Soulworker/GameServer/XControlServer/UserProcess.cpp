// UserProcess.cpp
// CUserProcess 用户包处理类实现 (对齐 IDA ControlServer.exe)

#include "UserProcess.h"
#include "ControlServer.h"
#include "UserObject.h"
#include "CServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"

// 对齐 IDA 0x1400450E0: Parse - 包解析入口
bool CUserProcess::Parse(XPacket& xPacket) {
    switch (xPacket.GetSubCmd()) {
        case 0x01: return SyncSelectCharacter(xPacket);
        case 0x03: return SyncLogoutUser(xPacket);
        case 0x04: return SyncUpdateUserMap(xPacket);
        case 0x07: return SyncUserKickout(xPacket);
        case 0x11: return ReqUserChatNotice(xPacket);
        case 0x12: return ReqUserChangeServer(xPacket);
        case 0x13: return SyncUserMoneyLog(xPacket);
        case 0x16: return ReqUserEnterPartyMaze(xPacket);
        case 0x17: return ReqUserChatMegaPhone(xPacket);
        case 0x20: return ReqUserEnterForceMaze(xPacket);
        case 0x26: return ReqUserTradePasswordStateSync(xPacket);
        case 0x27: return ReqUserTradePasswordState(xPacket);
        case 0x31: return ReqNameChange(xPacket);
        case 0x32: return ReqCheckSessionID(xPacket);
        case 0x35: return ReqUserUpdateAuthType(xPacket);
        case 0x60: return ReqGameServerEnterUser(xPacket);
        default:
            GreenDamTan_log(__FILE__, __FUNCTION__, "unknown sub command");
            return false;
    }
}

// 对齐 IDA 0x140045310: SyncSelectCharacter (sub 0x01)
bool CUserProcess::SyncSelectCharacter(XPacket& xPacket) {
    STCharInfo stCharInfo{};
    std::uint32_t dwIP = 0;
    std::uint8_t byTradePasswordState = 0;
    std::uint64_t biAuthSessionID = 0;
    std::uint8_t byBlockType = 0;

    xPacket >> stCharInfo;
    xPacket.XParse >> dwIP;
    xPacket.XParse >> byTradePasswordState;
    xPacket.XParse >> biAuthSessionID;
    xPacket.XParse >> byBlockType;

    LogHelper::LogDebug("game.relay", "<SELECT CHAR> %d Add User ( %d ) ",
        stCharInfo.uxActorID.dwActorID, dwIP);

    UXMapID uxMapID{};
    CServer* pServer = GetClientPtr();
    return TXSingleton<XControlServer>::Instance()->AddUser(pServer, stCharInfo, uxMapID, dwIP,
        byTradePasswordState, biAuthSessionID, byBlockType);
}

// 对齐 IDA 0x1400454B0: SyncLogoutUser (sub 0x03)
bool CUserProcess::SyncLogoutUser(XPacket& xPacket) {
    std::uint32_t dwActorID = 0;
    int nAccountState = 0;
    std::uint8_t byKick_AlreadyLogin = 0;

    xPacket.XParse >> dwActorID;
    xPacket.XParse >> nAccountState;
    xPacket.XParse >> byKick_AlreadyLogin;

    bool bKick_AlreadyLogin = (byKick_AlreadyLogin == 1);
    TXSingleton<XControlServer>::Instance()->RemoveUser(dwActorID, nAccountState, bKick_AlreadyLogin);
    return true;
}

// 对齐 IDA 0x140045570: SyncUpdateUserMap (sub 0x04)
bool CUserProcess::SyncUpdateUserMap(XPacket& xPacket) {
    PS_UPDATE_USER_MAP_INFO stUpdateMap{};
    xPacket >> stUpdateMap;

    CServer* pServer = GetClientPtr();
    TXSingleton<XControlServer>::Instance()->UpdateUserMap(pServer, stUpdateMap);
    return true;
}

// 对齐 IDA 0x1400455D0: SyncUserKickout (sub 0x07)
bool CUserProcess::SyncUserKickout(XPacket& xPacket) {
    PS_KICK_USER_INFO psKick{};
    xPacket >> psKick;

    TXSingleton<XControlServer>::Instance()->KickoutUser_UseLock(psKick, true);
    return true;
}

// 对齐 IDA 0x140045640: ReqUserChatNotice (sub 0x11)
bool CUserProcess::ReqUserChatNotice(XPacket& xPacket) {
    PS_CHAT_NOTICE stNotice{};
    xPacket >> stNotice;

    TXSingleton<XControlServer>::Instance()->SendChatNotice(stNotice);
    return true;
}

// 对齐 IDA 0x1400456B0: ReqUserChatMegaPhone (sub 0x17)
bool CUserProcess::ReqUserChatMegaPhone(XPacket& xPacket) {
    PS_CHAT_MEGAPHONE stMegaPhone{};
    PS_CHAT_ITEM_LINK_FOR_SERVER stLinkItem{};

    xPacket >> stMegaPhone;
    xPacket >> stLinkItem;

    TXSingleton<XControlServer>::Instance()->SendChatMegaPhone(stMegaPhone, stLinkItem);
    return true;
}

// 对齐 IDA 0x140045A10: ReqUserChangeServer (sub 0x12)
bool CUserProcess::ReqUserChangeServer(XPacket& xPacket) {
    PS_REQ_CHANGE_SERVER stChangeServer{};
    xPacket >> stChangeServer;

    CServer* pReqServer = GetClientPtr();
    TXSingleton<XControlServer>::Instance()->ChangeServer(pReqServer, stChangeServer);
    return true;
}

// 对齐 IDA 0x140045A80: SyncUserMoneyLog (sub 0x13)
bool CUserProcess::SyncUserMoneyLog(XPacket& xPacket) {
    std::uint64_t biMoney = 0;
    xPacket.XParse >> biMoney;

    TXSingleton<XControlServer>::Instance()->SetMoneySupply(static_cast<__int64>(biMoney));
    return true;
}

// 对齐 IDA 0x140045AC0: ReqUserEnterPartyMaze (sub 0x16)
bool CUserProcess::ReqUserEnterPartyMaze(XPacket& xPacket) {
    std::uint32_t dwPartyID = 0;
    UXMapID uxMapID{};
    PS_ENTER_MAP_REQ stEnterMap{};

    xPacket.XParse >> dwPartyID;
    xPacket.XParse >> uxMapID.nMapID;
    xPacket >> stEnterMap;

    CServer* pServer = GetClientPtr();
    TXSingleton<XControlServer>::Instance()->EnterMemberInMaze(pServer, dwPartyID, uxMapID, stEnterMap);
    return true;
}

// 对齐 IDA 0x140045B70: ReqUserEnterForceMaze (sub 0x20)
bool CUserProcess::ReqUserEnterForceMaze(XPacket& xPacket) {
    std::uint32_t dwForceID = 0;
    UXMapID uxMapID{};
    PS_ENTER_MAP_REQ stEnterMap{};

    xPacket.XParse >> dwForceID;
    xPacket.XParse >> uxMapID.nMapID;
    xPacket >> stEnterMap;

    CServer* pServer = GetClientPtr();
    TXSingleton<XControlServer>::Instance()->EnterMemberInMazeForce(pServer, dwForceID, uxMapID, stEnterMap);
    return true;
}

// 对齐 IDA 0x140045C30: ReqUserTradePasswordStateSync (sub 0x26)
bool CUserProcess::ReqUserTradePasswordStateSync(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    std::uint8_t byTradePWState = 0;

    xPacket.XParse >> dwUCID;
    xPacket.XParse >> byTradePWState;

    CServer* pServer = GetClientPtr();
    TXSingleton<XControlServer>::Instance()->SendUserTradePasswordStateSync(pServer, dwUCID, static_cast<char>(byTradePWState));
    return true;
}

// 对齐 IDA 0x140045CB0: ReqUserTradePasswordState (sub 0x27)
bool CUserProcess::ReqUserTradePasswordState(XPacket& xPacket) {
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> dwUCID;

    CServer* pServer = GetClientPtr();
    TXSingleton<XControlServer>::Instance()->SendUserTradePasswordState(pServer, dwUCID);
    return true;
}

// 对齐 IDA 0x140045D10: ReqNameChange (sub 0x31)
bool CUserProcess::ReqNameChange(XPacket& xPacket) {
    PS_CHANGE_NAME stChangeName{};
    xPacket >> stChangeName;

    TXSingleton<XControlServer>::Instance()->CharacterNameChange(stChangeName.dwActorID, stChangeName.szChangeName);
    return true;
}

// 对齐 IDA 0x140045D70: ReqCheckSessionID (sub 0x32)
bool CUserProcess::ReqCheckSessionID(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) return false;

    std::uint32_t dwUAID = 0;
    std::uint64_t biAuthSessionID = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> biAuthSessionID;

    TXSingleton<XControlServer>::Instance()->CheckSessionID(pServer, dwUAID, biAuthSessionID);
    return true;
}

// 对齐 IDA 0x140045E00: ReqGameServerEnterUser (sub 0x60)
bool CUserProcess::ReqGameServerEnterUser(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) return false;

    std::uint32_t dwUAID = 0;
    std::uint32_t dwUCID = 0;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> dwUCID;

    auto pUser = TXSingleton<XControlServer>::Instance()->GetUser(dwUCID);
    if (pUser) {
        XSendPacket xSendPacket(0xF2, 0x60);
        xSendPacket.XParse << dwUCID;
        xSendPacket.XParse << pUser->GetBlockType();
        pServer->SendEx(xSendPacket);
    }
    return true;
}

// 对齐 IDA 0x140045F40: ReqUserUpdateAuthType (sub 0x35)
bool CUserProcess::ReqUserUpdateAuthType(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) return false;

    PS_USER_UPDATE_AUTH_TYPE stUpdateInfo{};
    xPacket >> stUpdateInfo;

    return TXSingleton<XControlServer>::Instance()->UpdateAuthType(stUpdateInfo.dwUAID, stUpdateInfo.byAuthType);
}
