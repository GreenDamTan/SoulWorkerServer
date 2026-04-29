#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XControlServer/ControlServer.h"
#include "Soulworker/GameServer/XControlServer/MazeInfo.h"
#include "Soulworker/GameServer/XRelayServer/PartyManager.h"
#include "Soulworker/GameServer/XRelayServer/ForceManager.h"
#include "Soulworker/GameServer/XControlServer/WorldManager.h"

// 对齐 IDA 0x14003CAC0: CServerProcess::Parse - 包解析入口
bool CServerProcess::Parse(XPacket& xPacket) {
    // 对齐 IDA: switch on (unsigned __int8)GetSubCmd
    switch (xPacket.GetSubCmd()) {
    case 0x01:  // ReqCreateServer
        return ReqCreateServer(xPacket);
    case 0x03:  // ReqUpdateServerInfo
        return ReqUpdateServerInfo(xPacket);
    case 0x10:  // ReqChangeChannel
        return ReqChangeChannel(xPacket);
    case 0x21:  // ReqCreateMaze
        return ReqCreateMaze(xPacket);
    case 0x22:  // ResCreateMaze
        return ResCreateMaze(xPacket);
    case 0x23:  // SyncMaze
        return SyncMaze(xPacket);
    case 0x25:  // SyncRemoveMaze
        return SyncRemoveMaze(xPacket);
    case 0x26:  // SyncUpdateMaze
        return SyncUpdateMaze(xPacket);
    case 0x27:  // ReqGoBackMaze
        return ReqGoBackMaze(xPacket);
    case 0x30:  // ReqCreateMap
        return ReqCreateMap(xPacket);
    case 0x31:  // ReqEnterMap
        return ReqEnterMap(xPacket);
    case 0x32:  // ReqCheckPartyInMaze
        return ReqCheckPartyInMaze(xPacket);
    case 0x33:  // SyncUsersInfo
        return SyncUsersInfo(xPacket);
    case 0x36:  // SyncUserPartyInfo
        return SyncUserPartyInfo(xPacket);
    case 0x37:  // SyncLogicThreadCount
        return SyncLogicThreadCount(xPacket);
    case 0x38:  // ReqCheckEnterMaze
        return ReqCheckEnterMaze(xPacket);
    case 0x39:  // ReqDisconnectUserSync
        return ReqDisconnectUserSync(xPacket);
    case 0x40:  // SyncMaxMazeID
        return SyncMaxMazeID(xPacket);
    case 0x41:  // ResCreateMatchingMazeFromGame (PARTY)
        return ResCreateMatchingMazeFromGame(xPacket, E_PARTY_GROUP_TYPE_PARTY);
    case 0x42:  // ResCreateMatchingMazeFromGame (FORCE)
        return ResCreateMatchingMazeFromGame(xPacket, E_PARTY_GROUP_TYPE_FORCE);
    case 0x43:  // ReqCreateMatchingMazeFromCommunity
        return ReqCreateMatchingMazeFromCommunity(xPacket);
    case 0x49:  // ReqCreateMatchingModeMazeFromCommunity
        return ReqCreateMatchingModeMazeFromCommunity(xPacket);
    case 0x50:  // ReqMyRoomEnterReq
        return ReqMyRoomEnterReq(xPacket);
    case 0x51:  // ReqMyRoomEnterRes
        return ReqMyRoomEnterRes(xPacket);
    case 0x52:  // ReqMyRoomCreate
        return ReqMyRoomCreate(xPacket);
    case 0x53:  // ReqMyRoomDelete
        return ReqMyRoomDelete(xPacket);
    case 0x54:  // EnterOtherMap_cheat
        return EnterOtherMap_cheat(xPacket);
    case 0x55:  // PartyMazeSync
        return PartyMazeSync(xPacket);
    case 0x56:  // ReqPostSend
        return ReqPostSend(xPacket);
    case 0x58:  // ReqFindUser
        return ReqFindUser(xPacket);
    case 0x59:  // ResMyRoomDelete
        return ResMyRoomDelete(xPacket);
    case 0x63:  // ForceMazeSync
        return ForceMazeSync(xPacket);
    case 0x73:  // ResCreateModeMaze
        return ResCreateModeMaze(xPacket);
    case 0x75:  // ReqUpdateRouletteEvent
        return ReqUpdateRouletteEvent(xPacket);
    case 0x77:  // ReqReEnterMap
        return ReqReEnterMap(xPacket);
    default:
        return true;  // IDA: default case returns 1
    }
}

bool CServerProcess::ReqCreateServer(XPacket& xPacket) {
    CServer* server = GetClientPtr();
    if (!server) {
        return false;
    }

    SS_SERVER_INFO serverInfo{};
    xPacket >> serverInfo;
    server->SetServerInfo(serverInfo);
    TXSingleton<XRelayServer>::Instance()->AddServerInfo(server);
    return true;
}

bool CServerProcess::ReqUpdateServerInfo(XPacket& xPacket) {
    CServer* server = GetClientPtr();
    if (!server) {
        return false;
    }

    SS_UPDATE_SERVER_INFO updateInfo{};
    xPacket >> updateInfo;
    // IDA 0x1400CEA50: 仅反序列化，不处理updateInfo（原版即如此）
    return true;
}

bool CServerProcess::SyncUsersInfo(XPacket& xPacket) {
    CServer* server = GetClientPtr();
    if (!server) {
        return false;
    }

    PS_USERS_INFO usersInfo{};
    xPacket >> usersInfo;
    TXSingleton<XRelayServer>::Instance()->SetUsersInfo(server, usersInfo);
    return true;
}

// 对齐 IDA 0x14003D130: ReqChangeChannel (sub 0x10)
bool CServerProcess::ReqChangeChannel(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    PS_ENTER_MAP_REQ stEnterMap{};
    xPacket >> stEnterMap;
    XControlServer::Instance()->GetWorldManager().ReqChangeChannel(pServer, &stEnterMap);
    return true;
}

// 对齐 IDA 0x14003D1F0: ReqCreateMaze (sub 0x21)
bool CServerProcess::ReqCreateMaze(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    ST_CREATE_MAZE stCreateMaze{};
    xPacket >> stCreateMaze;
    XControlServer::Instance()->ReqCreateMaze(pServer, &stCreateMaze);
    return true;
}

// 对齐 IDA 0x14003D360: ResCreateMaze (sub 0x22)
bool CServerProcess::ResCreateMaze(XPacket& xPacket) {
    ST_CREATE_MAZE ssCreateMaze{};
    xPacket >> ssCreateMaze;
    XControlServer::Instance()->ResCreateMaze(&ssCreateMaze);
    return true;
}

// 对齐 IDA 0x14003D4F0: SyncMaze (sub 0x23)
bool CServerProcess::SyncMaze(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    PS_MAZE_UPDATE_INFO_SYNC stMazeInfo{};
    xPacket >> stMazeInfo;
    pServer->SyncMaze(&stMazeInfo);
    XControlServer::Instance()->SyncEventMaze(&stMazeInfo);
    return true;
}

// 对齐 IDA 0x14003D410: SyncRemoveMaze (sub 0x25)
bool CServerProcess::SyncRemoveMaze(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    UXMapID uxMapID{};
    char bResult = 0;
    xPacket.XParse >> uxMapID.nMapID;
    xPacket.XParse >> bResult;
    pServer->RemoveMaze(uxMapID, bResult != 0);
    return true;
}

// 对齐 IDA 0x14003D480: SyncUpdateMaze (sub 0x26)
bool CServerProcess::SyncUpdateMaze(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    PS_MAZE_UPDATE_INFO stMazeInfo{};
    xPacket >> stMazeInfo;
    pServer->UpdateMaze(&stMazeInfo);
    return true;
}

// 对齐 IDA 0x14003D2A0: ReqGoBackMaze (sub 0x27)
bool CServerProcess::ReqGoBackMaze(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    ST_GO_BACK_MAZE stGoBackMaze{};
    xPacket >> stGoBackMaze;
    XControlServer::Instance()->ReqGoBackMaze(pServer, &stGoBackMaze);
    return true;
}

// 对齐 IDA 0x14003D5D0: ReqCreateMap (sub 0x30)
bool CServerProcess::ReqCreateMap(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    PS_CREATE_MAP_LIST stMapList{};
    int nMazeCount = 0;
    xPacket >> stMapList;
    xPacket.XParse >> nMazeCount;

    XControlServer::Instance()->GetWorldManager().AddMap(pServer, &stMapList);
    XControlServer::Instance()->GetWorldManager().SendChannelInfoAll();
    XControlServer::Instance()->AddMazeServerInfo(pServer, nMazeCount);
    return true;
}

// 对齐 IDA 0x14003D6E0: ReqEnterMap (sub 0x31)
bool CServerProcess::ReqEnterMap(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    PS_ENTER_MAP_REQ stEnterMap{};
    xPacket >> stEnterMap;
    XControlServer::Instance()->GetWorldManager().ReqEnterMap(pServer, &stEnterMap);
    return true;
}

// 对齐 IDA 0x14003D8C0: ReqCheckPartyInMaze (sub 0x32)
bool CServerProcess::ReqCheckPartyInMaze(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    PS_ENTER_MAP_REQ stCreateMaze{};
    xPacket >> stCreateMaze;

    // 对齐 IDA: byGroupType 分发
    if (stCreateMaze.stPartyInfo.byGroupType == 1) {
        XControlServer::Instance()->CheckPartyInMaze(pServer, &stCreateMaze);
    }
    else if (stCreateMaze.stPartyInfo.byGroupType == 2) {
        XControlServer::Instance()->CheckForceInMaze(pServer, &stCreateMaze);
    }
    return true;
}

// 对齐 IDA 0x14003D9E0: SyncUserPartyInfo (sub 0x36)
bool CServerProcess::SyncUserPartyInfo(XPacket& xPacket) {
    ST_PARTY_INFO stPartyInfo{};
    UXMapID uxPartyMapID{};
    UXMapID uxMapID{};
    DWORD dwUCID = 0;

    xPacket >> stPartyInfo;
    xPacket.XParse >> uxPartyMapID.nMapID;
    xPacket.XParse >> uxMapID.nMapID;
    xPacket.XParse >> dwUCID;

    auto pUserInfo = XControlServer::Instance()->GetUser(dwUCID);
    if (!pUserInfo) {
        return false;
    }

    // 对齐 IDA: Party 处理 (byGroupType==1)
    if (stPartyInfo.byGroupType == 1 && stPartyInfo.nID > 0) {
        bool bExist = XControlServer::Instance()->GetPartyManager().IsParty(stPartyInfo.nID);
        XControlServer::Instance()->GetPartyManager().SetMember(stPartyInfo.nID, dwUCID, uxMapID);
        if (!bExist) {
            XControlServer::Instance()->GetPartyManager().SetMazeID(stPartyInfo.nID, uxPartyMapID);
        }
    }
    // 对齐 IDA: Force 处理 (byGroupType==2)
    else if (stPartyInfo.byGroupType == 2 && stPartyInfo.nID > 0) {
        bool IsParty = XControlServer::Instance()->GetForceManager().IsForce(stPartyInfo.nID);
        XControlServer::Instance()->GetForceManager().SetMember(stPartyInfo.nID, dwUCID, uxMapID);
        if (!IsParty) {
            XControlServer::Instance()->GetForceManager().SetMazeID(stPartyInfo.nID, uxPartyMapID);
        }
    }

    pUserInfo->SetPartyInfo(&stPartyInfo);
    return true;
}

// 对齐 IDA 0x14003DBF0: SyncLogicThreadCount (sub 0x37)
bool CServerProcess::SyncLogicThreadCount(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    DWORD nLogicThreadCount = 0;
    xPacket.XParse >> nLogicThreadCount;
    pServer->SetLogicThreadCount(nLogicThreadCount);
    return true;
}

// 对齐 IDA 0x14003DC40: ReqCheckEnterMaze (sub 0x38)
bool CServerProcess::ReqCheckEnterMaze(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    ST_SERVER_CHECK_ENTER_MAZE stCheckEnter{};
    xPacket >> stCheckEnter;
    XControlServer::Instance()->ReqCheckEnterMaze(pServer, &stCheckEnter);
    return true;
}

// 对齐 IDA 0x14003DCA0: ReqDisconnectUserSync (sub 0x39)
bool CServerProcess::ReqDisconnectUserSync(XPacket& xPacket) {
    ST_MAZE_WAIT_ENTER_USER_INFO stDisconnect{};
    unsigned __int64 nMapInstance = 0;
    xPacket >> stDisconnect;
    xPacket.XParse >> nMapInstance;
    XControlServer::Instance()->ReqDisconnectUserSync(nMapInstance, &stDisconnect);
    return true;
}

// 对齐 IDA 0x14003DD10: SyncMaxMazeID (sub 0x40)
bool CServerProcess::SyncMaxMazeID(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    DWORD nMaxMazeID = 0;
    xPacket.XParse >> nMaxMazeID;
    pServer->SetSerial(nMaxMazeID);
    return true;
}

// 对齐 IDA 0x14003DFF0: ResCreateMatchingMazeFromGame (sub 0x41/0x42)
bool CServerProcess::ResCreateMatchingMazeFromGame(XPacket& xPacket, int eType) {
    if (eType == E_PARTY_GROUP_TYPE_PARTY) {
        ST_CREATE_MAZE stCreateMaze{};
        PS_PARTY_INFO stPartyInfo{};
        DWORD dwMatchingID = 0;
        xPacket.XParse >> dwMatchingID;
        xPacket >> stCreateMaze;
        xPacket >> stPartyInfo;
        XControlServer::Instance()->ResCreateMatchingMaze(dwMatchingID, &stCreateMaze, &stPartyInfo);
    }
    else if (eType == E_PARTY_GROUP_TYPE_FORCE) {
        ST_CREATE_MAZE stCreateMaze{};
        PS_FORCE_INFO stForceInfo{};
        DWORD dwMatchingID = 0;
        xPacket.XParse >> dwMatchingID;
        xPacket >> stCreateMaze;
        xPacket >> stForceInfo;
        XControlServer::Instance()->ResCreateMatchingMaze(dwMatchingID, &stCreateMaze, &stForceInfo);
    }
    return true;
}

// 对齐 IDA 0x14003DD60: ReqCreateMatchingMazeFromCommunity (sub 0x43)
bool CServerProcess::ReqCreateMatchingMazeFromCommunity(XPacket& xPacket) {
    ST_CREATE_MAZE stCreateMaze{};
    PS_PARTY_INFO stCreateParty{};
    PS_FORCE_INFO stCreateForce{};
    DWORD dwMatchingID = 0;

    xPacket >> stCreateMaze;
    xPacket >> stCreateParty;
    xPacket >> stCreateForce;
    xPacket.XParse >> dwMatchingID;

    if (stCreateMaze.stPartyInfo.byGroupType == 1) {
        XControlServer::Instance()->CreateMatchingMaze(&stCreateMaze, &stCreateParty, dwMatchingID);
    }
    else if (stCreateMaze.stPartyInfo.byGroupType == 2) {
        XControlServer::Instance()->CreateMatchingMaze(&stCreateMaze, &stCreateForce, dwMatchingID);
    }
    return true;
}

// 对齐 IDA 0x14003DF20: ReqCreateMatchingModeMazeFromCommunity (sub 0x49)
bool CServerProcess::ReqCreateMatchingModeMazeFromCommunity(XPacket& xPacket) {
    ST_CREATE_MODE_MAZE stCreateModeMaze{};
    xPacket >> stCreateModeMaze;

    // 对齐 IDA: 调用 XControlServer::CreateMatchingModeMaze
    return XControlServer::Instance()->CreateMatchingModeMaze(stCreateModeMaze);
}

// 对齐 IDA 0x14003E460: ReqMyRoomEnterReq (sub 0x50)
bool CServerProcess::ReqMyRoomEnterReq(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    ST_MYROOM_USER stMyRoomUser{};
    ST_MYROOM_OWNER_INFO stOwnerInfo{};
    DWORD dwOwnerUCID = 0;

    xPacket >> stMyRoomUser;
    xPacket >> stOwnerInfo;
    xPacket.XParse >> dwOwnerUCID;

    if (!XControlServer::Instance()->MyRoomEnterReq(&stMyRoomUser, &stOwnerInfo, pServer, dwOwnerUCID)) {
        // 对齐 IDA: MyRoomEnterReq 失败时发送响应包
        PS_ENTER_MAP_RES stEnterMapRes{};
        ST_MYROOM_OWNER_INFO st{};
        XSendPacket xSendPacket(0xF2, 0x51);
        xSendPacket.XParse << static_cast<int>(58209);
        xSendPacket << stEnterMapRes;
        xSendPacket << st;
        xSendPacket.XParse << static_cast<int>(dwOwnerUCID);
        pServer->SendEx(xSendPacket);
    }
    return true;
}

// 对齐 IDA 0x14003E680: ReqMyRoomEnterRes (sub 0x51)
bool CServerProcess::ReqMyRoomEnterRes(XPacket& xPacket) {
    CServer* pMyRoomServer = GetClientPtr();

    int nErrorCode = 0;
    ST_MYROOM_USER stEnterUser{};
    UXMapID uxMapID{};
    ST_MYROOM_OWNER_INFO stOwnerInfo{};
    DWORD dwOwnerUCID = 0;

    xPacket.XParse >> nErrorCode;
    xPacket >> stEnterUser;
    xPacket.XParse >> uxMapID.nMapID;
    xPacket >> stOwnerInfo;
    xPacket.XParse >> dwOwnerUCID;

    CServer* pServer = XControlServer::Instance()->GetServer(stEnterUser.nServerID);
    if (!pServer) {
        return true;
    }

    PS_ENTER_MAP_RES ssCreateMaze{};
    ssCreateMaze.dwUserID = stEnterUser.dwUCID;

    if (nErrorCode == 0) {
        // 对齐 IDA: 查找 MyRoom 并调用 EnterSucc
        auto pMyRoom = XControlServer::Instance()->FindMyRoom(stOwnerInfo.dwOwnerUAID);
        if (!pMyRoom) {
            XSendPacket xSendPacket(0xF2, 0x51);
            xSendPacket.XParse << static_cast<int>(100);
            xSendPacket << ssCreateMaze;
            xSendPacket << stOwnerInfo;
            xSendPacket.XParse << static_cast<int>(dwOwnerUCID);
            pServer->SendEx(xSendPacket);
            return true;
        }

        pMyRoom->EnterSucc();

        // 对齐 IDA: 填充响应信息
        SS_SERVER_INFO* pServerInfo = pMyRoomServer->GetServerInfo();
        strcpy_s(ssCreateMaze.szIP, pServerInfo->szPublicIP);
        ssCreateMaze.sPort = pServerInfo->sPort;
        ssCreateMaze.dwServerID = pMyRoomServer->GetServerID();
        ssCreateMaze.uxMapID = uxMapID;
        ssCreateMaze.nResult = 0;
        ssCreateMaze.byChangeType = 0;
        ssCreateMaze.bChangeServer = (pMyRoomServer != pServer);
    }

    // 对齐 IDA: 发送响应包
    XSendPacket xSendPacket(0xF2, 0x51);
    xSendPacket.XParse << nErrorCode;
    xSendPacket << ssCreateMaze;
    xSendPacket << stOwnerInfo;
    xSendPacket.XParse << static_cast<int>(dwOwnerUCID);
    pServer->SendEx(xSendPacket);
    return true;
}

// 对齐 IDA 0x14003ED40: ReqMyRoomCreate (sub 0x52)
bool CServerProcess::ReqMyRoomCreate(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    int nResult = 0;
    DWORD dwOwnerUAID = 0;
    ST_MYROOM_USER stCreateUser{};
    DWORD dwOwnerUCID = 0;

    xPacket.XParse >> nResult;
    xPacket.XParse >> dwOwnerUAID;
    xPacket >> stCreateUser;
    xPacket.XParse >> dwOwnerUCID;

    XControlServer::Instance()->ReqCreateMyRoom(nResult, dwOwnerUAID, &stCreateUser, pServer, dwOwnerUCID);
    return true;
}

// 对齐 IDA 0x14003EE80: ReqMyRoomDelete (sub 0x53)
bool CServerProcess::ReqMyRoomDelete(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    DWORD dwOwnerUAID = 0;
    UXMapID uxMapID{};

    xPacket.XParse >> dwOwnerUAID;
    xPacket.XParse >> uxMapID.nMapID;

    XControlServer::Instance()->DeleteMyRoomReq(dwOwnerUAID, uxMapID, pServer);
    return true;
}

// 对齐 IDA 0x14003D750: EnterOtherMap_cheat (sub 0x54)
bool CServerProcess::EnterOtherMap_cheat(XPacket& xPacket) {
    PS_ENTER_MAP_REQ stEnterMap{};
    STPosInfo stPosInfo{};
    DWORD dwTargetID = 0;

    xPacket >> stEnterMap;
    xPacket >> stPosInfo;
    xPacket.XParse >> dwTargetID;

    // 对齐 IDA: 获取目标用户
    DWORD dwServerID = 0;
    auto pUser = XControlServer::Instance()->GetUser(dwTargetID);
    if (pUser) {
        dwServerID = pUser->GetServerID();
    }

    // 对齐 IDA: 获取目标服务器并转发
    CServer* pServer = XControlServer::Instance()->GetServer(dwServerID);
    if (pServer) {
        XControlServer::Instance()->GetWorldManager().ReqEnterMapToOther(pServer, &stEnterMap, &stPosInfo, dwTargetID);
        return true;
    }
    return false;
}

// 对齐 IDA 0x14003E9F0: PartyMazeSync (sub 0x55)
bool CServerProcess::PartyMazeSync(XPacket& xPacket) {
    DWORD dwPartyID = 0;
    UXMapID uxMapID{};

    xPacket.XParse >> dwPartyID;
    uxMapID.nMapID = 0;  // 对齐 IDA: UXMapID::UXMapID(v5, 0)

    XControlServer::Instance()->GetPartyManager().SetMazeID(dwPartyID, uxMapID);
    return true;
}

// 对齐 IDA 0x14003EAD0: ReqPostSend (sub 0x56)
bool CServerProcess::ReqPostSend(XPacket& xPacket) {
    DWORD dwRecvUCID = 0;
    DWORD dwServerID = 0;
    unsigned __int16 wPostCount = 0;
    ST_POST_DATA stPostData{};
    char byState = 0;
    ST_POST_CHAR stPostChar{};

    xPacket.XParse >> dwRecvUCID;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> wPostCount;
    xPacket >> stPostData;
    xPacket.XParse >> byState;
    if (!byState) {
        xPacket >> stPostChar;
    }

    // 对齐 IDA: 检查服务器是否存在
    CServer* pServer = XControlServer::Instance()->GetServer(dwServerID);
    if (!pServer) {
        return false;
    }

    // 对齐 IDA: 获取接收用户
    auto pReceiver = XControlServer::Instance()->GetUser(dwRecvUCID);
    if (!pReceiver) {
        return false;
    }

    // 对齐 IDA: 发送 0xF2/0x56 邮件包给接收用户
    XSendPacket xSendPacket(0xF2, 0x56);
    xSendPacket.XParse << dwRecvUCID;
    xSendPacket.XParse << wPostCount;
    xSendPacket << stPostData;
    xSendPacket.XParse << byState;
    xSendPacket << stPostChar;
    pReceiver->SendPacket(xSendPacket);
    return true;
}

// 对齐 IDA 0x14003CEE0: ReqFindUser (sub 0x58)
bool CServerProcess::ReqFindUser(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    wchar_t szOtherName[24] = {0};
    __int16 sLen = 0;
    DWORD dwUCID = 0;
    char byState = 0;

    xPacket.XParse.GetWString(szOtherName, 21, &sLen);
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> byState;

    // 对齐 IDA: 根据名字查找用户
    auto pUser = XControlServer::Instance()->GetUser(szOtherName);
    if (!pUser) {
        return false;
    }

    // 对齐 IDA: 发送 0xF3/0x58 查找结果包
    XSendPacket xSendPacket(0xF3, 0x58);
    xSendPacket.XParse << dwUCID;
    xSendPacket.XParse << pUser->GetCID();
    xSendPacket.XParse << pServer->GetServerID();
    xSendPacket.XParse << byState;
    pServer->SendEx(xSendPacket);
    return true;
}

// 对齐 IDA 0x14003EF20: ResMyRoomDelete (sub 0x59)
bool CServerProcess::ResMyRoomDelete(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    DWORD dwOwnerUAID = 0;
    UXMapID uxMapID{};

    xPacket.XParse >> dwOwnerUAID;
    xPacket.XParse >> uxMapID.nMapID;

    XControlServer::Instance()->DeleteMyRoomRes(dwOwnerUAID, uxMapID, pServer);
    return true;
}

// 对齐 IDA 0x14003EA60: ForceMazeSync (sub 0x63)
bool CServerProcess::ForceMazeSync(XPacket& xPacket) {
    DWORD dwForceID = 0;
    UXMapID uxMapID{};

    xPacket.XParse >> dwForceID;
    uxMapID.nMapID = 0;  // 对齐 IDA: UXMapID::UXMapID(v5, 0)

    XControlServer::Instance()->GetForceManager().SetMazeID(dwForceID, uxMapID);
    return true;
}

// 对齐 IDA 0x14003EFC0: ResCreateModeMaze (sub 0x73)
bool CServerProcess::ResCreateModeMaze(XPacket& xPacket) {
    ST_CREATE_MODE_MAZE stCreateModeMaze{};
    xPacket >> stCreateModeMaze;

    // 对齐 IDA: 调用 XControlServer::ResCreateModeMaze
    XControlServer::Instance()->ResCreateModeMaze(stCreateModeMaze);
    return true;
}

// 对齐 IDA 0x14003F070: ReqUpdateRouletteEvent (sub 0x75)
bool CServerProcess::ReqUpdateRouletteEvent(XPacket& xPacket) {
    PS_ROULETTE_EVENT_UPDATE_SERVER psUpdateInfo{};
    xPacket >> psUpdateInfo;

    // 对齐 IDA: 调用 XControlServer::UpdateRouletteEventInfo
    XControlServer::Instance()->UpdateRouletteEventInfo(psUpdateInfo);
    return true;
}

// 对齐 IDA 0x14003EE20: ReqReEnterMap (sub 0x77)
bool CServerProcess::ReqReEnterMap(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    PS_ENTER_MAP_REQ stInfo{};
    xPacket >> stInfo;

    // 对齐 IDA: 调用 XControlServer::ReqReEnterMap
    XControlServer::Instance()->ReqReEnterMap(pServer, stInfo);
    return true;
}

// ============================================================================
// CServer 方法实现
// ============================================================================

// 对齐 IDA 0x140041930: RemoveMaze - 移除迷宫
void CServer::RemoveMaze(UXMapID uxMapID, bool bResult) {
    auto it = m_mapMazeInfo.find(uxMapID);
    if (it == m_mapMazeInfo.end()) {
        return;
    }

    auto pMazeInfo = it->second;
    if (!pMazeInfo) {
        m_mapMazeInfo.erase(it);
        return;
    }

    if (bResult) {
        // 对齐 IDA: ResetParentMaze, ResetChildMaze
        pMazeInfo->ResetParentMaze(this);
        pMazeInfo->ResetChildMaze();

        // 对齐 IDA: 处理 PartyID
        int nPartyID = pMazeInfo->GetPartyID();
        if (nPartyID > 0) {
            UXMapID emptyMapID{};
            emptyMapID.nMapID = 0;
            auto pControlServer = XControlServer::Instance();
            if (pControlServer && pControlServer->GetPartyManager().SetMazeID(nPartyID, emptyMapID, uxMapID) == 1) {
                // 对齐 IDA: 发送 0xF2, 0x45 到 Community
                XSendPacket xSendPacket(0xF2, 0x45);
                xSendPacket.XParse << nPartyID;
                xSendPacket.XParse << 0;  // dwResult = 0
                xSendPacket.XParse << uxMapID.nMapID;
                pControlServer->SendCommunity(xSendPacket);
            }
        }

        // 对齐 IDA: 处理 ForceID
        int nForceID = pMazeInfo->GetForceID();
        if (nForceID > 0) {
            UXMapID emptyMapID{};
            emptyMapID.nMapID = 0;
            auto pControlServer = XControlServer::Instance();
            if (pControlServer && pControlServer->GetForceManager().SetMazeID(nForceID, emptyMapID, uxMapID) == 1) {
                // 对齐 IDA: 发送 0xF2, 0x46 到 Community
                XSendPacket xSendPacket(0xF2, 0x46);
                xSendPacket.XParse << nForceID;
                xSendPacket.XParse << 0;  // dwResult = 0
                xSendPacket.XParse << uxMapID.nMapID;
                pControlServer->SendCommunity(xSendPacket);
            }
        }
        // 对齐 IDA: RemoveMaze 本身不从映射删除，由调用方(OnUpdate)负责删除
    }
    else {
        // 对齐 IDA: bResult=false 时，设置迷宫状态为 1
        pMazeInfo->SetMazeState(1, 0);
    }
}

// 对齐 IDA 0x1400421E0: UpdateMaze - 更新迷宫信息（不存在则创建）
void CServer::UpdateMaze(PS_MAZE_UPDATE_INFO* pMazeInfo) {
    if (!pMazeInfo) return;

    // 对齐 IDA: 查找迷宫
    auto it = m_mapMazeInfo.find(pMazeInfo->uxMapID);
    if (it != m_mapMazeInfo.end() && it->second) {
        // 对齐 IDA: 存在则更新
        it->second->UpdateMazeInfo(pMazeInfo);
    }
    else {
        // 对齐 IDA: 不存在则创建新 CMazeInfo
        auto pControlServer = XControlServer::Instance();
        if (pControlServer) {
            // 对齐 IDA: 使用 ClassFactory 创建 CMazeInfo
            auto pMazeInfoShared = pControlServer->GetMazeFactory().Create();
            if (pMazeInfoShared) {
                pMazeInfoShared->UpdateMazeInfo(pMazeInfo);
                m_mapMazeInfo[pMazeInfo->uxMapID] = pMazeInfoShared;
            }
        }
    }
}

// 对齐 IDA 0x140042340: SyncMaze - 同步迷宫信息（含 bLast 计数逻辑）
void CServer::SyncMaze(PS_MAZE_UPDATE_INFO_SYNC* pMazeInfo) {
    if (!pMazeInfo) return;

    // 对齐 IDA: bLast 处理同步计数
    if (pMazeInfo->bLast) {
        // 递增同步计数
        m_nLogicThreadSyncCount++;
        // 对齐 IDA: 检查是否所有逻辑线程都同步完成
        if (static_cast<int>(m_nLogicThreadSyncCount) == static_cast<int>(m_dwLogicThreadCount)) {
            SetSyncLoad(E_SERVER_SYNC_LOAD_MAZE_INFO);
        }
        // 对齐 IDA: 日志
        LogHelper::LogInfo("game.relay", "<SYNC> SyncMaze Finish [ID:%d] [%d / %d]",
            m_stServerInfo.dwID, m_nLogicThreadSyncCount, m_dwLogicThreadCount);
        return;
    }

    // 对齐 IDA: 查找迷宫
    auto it = m_mapMazeInfo.find(pMazeInfo->psMazeInfo.uxMapID);
    if (it != m_mapMazeInfo.end() && it->second) {
        // 对齐 IDA: 存在则同步
        it->second->SyncMazeInfo(pMazeInfo);
    }
    else {
        // 对齐 IDA: 不存在则创建新 CMazeInfo
        auto pControlServer = XControlServer::Instance();
        if (pControlServer) {
            // 对齐 IDA: 使用 ClassFactory 创建 CMazeInfo
            auto pMazeInfoShared = pControlServer->GetMazeFactory().Create();
            if (pMazeInfoShared) {
                pMazeInfoShared->SyncMazeInfo(pMazeInfo);
                m_mapMazeInfo[pMazeInfo->psMazeInfo.uxMapID] = pMazeInfoShared;
            }
        }
    }
}

// 对齐 IDA 0x140042550: OnUpdate - 服务器更新循环（发送 SyncRemoveMaze 包）
void CServer::OnUpdate(ULONGLONG dwTick) {
    (void)dwTick;  // IDA 中参数未使用
    // 对齐 IDA: 遍历迷宫，清理已销毁的迷宫
    for (auto it = m_mapMazeInfo.begin(); it != m_mapMazeInfo.end(); ) {
        if (it->second && it->second->IsDestroy()) {
            // 对齐 IDA: 创建 SyncRemoveMaze 包 (0xF2, 0x25)
            XSendPacket xSendPacket(0xF2, 0x25);

            // 对齐 IDA: 获取迷宫信息用于填充包
            ST_MAP_INFO stMapInfo = it->second->GetMazeInfo();
            xSendPacket.XParse << stMapInfo.uxMapID.nMapID;
            xSendPacket.XParse << it->second->GetMazeType();

            // 对齐 IDA: 发送包
            SendEx(xSendPacket);

            // 对齐 IDA: 调用 RemoveMaze(true)
            RemoveMaze(stMapInfo.uxMapID, true);

            // 对齐 IDA: 从映射中删除
            it = m_mapMazeInfo.erase(it);
        }
        else {
            ++it;
        }
    }
}

// 对齐 IDA 0x140041180: RegisterProcess - 注册包处理类
bool CServer::RegisterProcess() {
    // 对齐 IDA: 注册各种 Process 类
    return true;
}
