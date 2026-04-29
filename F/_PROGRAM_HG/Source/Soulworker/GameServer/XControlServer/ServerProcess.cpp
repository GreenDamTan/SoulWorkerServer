// ServerProcess.cpp
// ControlServer CServerProcess 包处理实现 (对齐 IDA)

#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XControlServer/ControlServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XControlServer/WorldManager.h"
#include "Soulworker/GameServer/XControlServer/UserObject.h"
#include <cstdio>

// 对齐 IDA 0x14003CAC0: Parse - 包解析入口
bool CServerProcess::Parse(XPacket& xPacket) {
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
    case 0x41:  // ResCreateMatchingMazeFromGame (Party)
        return ResCreateMatchingMazeFromGame(xPacket, E_PARTY_GROUP_TYPE_PARTY);
    case 0x42:  // ResCreateMatchingMazeFromGame (Force)
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

// 对齐 IDA 0x14003D0A0: ReqCreateServer (sub 0x01)
bool CServerProcess::ReqCreateServer(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    SS_SERVER_INFO serverInfo{};
    xPacket >> serverInfo;
    pServer->SetServerInfo(serverInfo);
    XControlServer::Instance()->AddServerInfo(pServer);
    return true;
}

// 对齐 IDA: ReqUpdateServerInfo (sub 0x03)
bool CServerProcess::ReqUpdateServerInfo(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    SS_UPDATE_SERVER_INFO updateInfo{};
    xPacket >> updateInfo;
    // 对齐 IDA: 仅反序列化，不处理 updateInfo
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
    XControlServer::Instance()->ReqCreateMaze(pServer, stCreateMaze);
    return true;
}

// 对齐 IDA 0x14003D360: ResCreateMaze (sub 0x22)
bool CServerProcess::ResCreateMaze(XPacket& xPacket) {
    ST_CREATE_MAZE ssCreateMaze{};
    xPacket >> ssCreateMaze;
    XControlServer::Instance()->ResCreateMaze(ssCreateMaze);
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
    XControlServer::Instance()->SyncEventMaze(stMazeInfo);
    return true;
}

// 对齐 IDA 0x14003D410: SyncRemoveMaze (sub 0x25)
bool CServerProcess::SyncRemoveMaze(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    UXMapID uxMapID{};
    bool bResult = false;
    xPacket.XParse >> uxMapID.nMapID;
    xPacket.XParse >> bResult;
    pServer->RemoveMaze(uxMapID, bResult);
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
    XControlServer::Instance()->ReqGoBackMaze(pServer, stGoBackMaze);
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

    XControlServer::Instance()->GetWorldManager().AddMap(pServer, stMapList);
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
    XControlServer::Instance()->GetWorldManager().ReqEnterMap(pServer, stEnterMap);
    return true;
}

// 对齐 IDA: SyncUsersInfo (sub 0x33)
bool CServerProcess::SyncUsersInfo(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    PS_USERS_INFO usersInfo{};
    xPacket >> usersInfo;
    XControlServer::Instance()->SetUsersInfo(pServer, usersInfo);
    return true;
}

// 对齐 IDA 0x14003DBF0: SyncLogicThreadCount (sub 0x37)
bool CServerProcess::SyncLogicThreadCount(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    std::uint32_t nLogicThreadCount = 0;
    xPacket.XParse >> nLogicThreadCount;
    pServer->SetLogicThreadCount(nLogicThreadCount);
    return true;
}

// 对齐 IDA 0x14003DCA0: ReqDisconnectUserSync (sub 0x39)
bool CServerProcess::ReqDisconnectUserSync(XPacket& xPacket) {
    ST_MAZE_WAIT_ENTER_USER_INFO stDisconnect{};
    std::uint64_t nMapInstance = 0;

    xPacket >> stDisconnect;
    xPacket.XParse >> nMapInstance;
    XControlServer::Instance()->ReqDisconnectUserSync(static_cast<__int64>(nMapInstance), stDisconnect);
    return true;
}

// 对齐 IDA 0x14003DD10: SyncMaxMazeID (sub 0x40)
bool CServerProcess::SyncMaxMazeID(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    std::uint32_t nMaxMazeID = 0;
    xPacket.XParse >> nMaxMazeID;
    pServer->SetSerial(static_cast<LONG>(nMaxMazeID));
    return true;
}

// 对齐 IDA 0x14003E9F0: PartyMazeSync (sub 0x3C)
bool CServerProcess::PartyMazeSync(XPacket& xPacket) {
    std::uint32_t dwPartyID = 0;
    UXMapID uxMapID{};

    xPacket.XParse >> dwPartyID;
    // 对齐 IDA: 不从包读取 UXMapID，而是设置为 0
    uxMapID.nMapID = 0;

    XControlServer::Instance()->GetPartyManager().SetMazeID(static_cast<int>(dwPartyID), uxMapID);
    return true;
}

// 对齐 IDA 0x14003EA60: ForceMazeSync (sub 0x3D)
bool CServerProcess::ForceMazeSync(XPacket& xPacket) {
    std::uint32_t dwForceID = 0;
    UXMapID uxMapID{};

    xPacket.XParse >> dwForceID;
    // 对齐 IDA: 不从包读取 UXMapID，而是设置为 0
    uxMapID.nMapID = 0;

    XControlServer::Instance()->GetForceManager().SetMazeID(static_cast<int>(dwForceID), uxMapID);
    return true;
}

// 对齐 IDA 0x14003ED40: ReqMyRoomCreate (sub 0x54)
bool CServerProcess::ReqMyRoomCreate(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    int nResult = 0;
    std::uint32_t dwOwnerUAID = 0;
    ST_MYROOM_USER stCreateUser{};
    std::uint32_t dwOwnerUCID = 0;

    xPacket.XParse >> nResult;
    xPacket.XParse >> dwOwnerUAID;
    xPacket >> stCreateUser;
    xPacket.XParse >> dwOwnerUCID;

    XControlServer::Instance()->ReqCreateMyRoom(nResult, dwOwnerUAID, stCreateUser, pServer, dwOwnerUCID);
    return true;
}

// 对齐 IDA 0x14003EE80: ReqMyRoomDelete (sub 0x55)
bool CServerProcess::ReqMyRoomDelete(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    std::uint32_t dwOwnerUAID = 0;
    std::uint64_t biUxMapID = 0;

    xPacket.XParse >> dwOwnerUAID;
    xPacket.XParse >> biUxMapID;

    UXMapID uxMapID{};
    uxMapID.nMapID = static_cast<std::int64_t>(biUxMapID);

    XControlServer::Instance()->DeleteMyRoomReq(dwOwnerUAID, uxMapID, pServer);
    return true;
}

// 对齐 IDA 0x14003E460: ReqMyRoomEnterReq (sub 0x52)
bool CServerProcess::ReqMyRoomEnterReq(XPacket& xPacket) {
    CServer* pServer = GetClientPtr();
    if (!pServer) {
        return false;
    }

    ST_MYROOM_USER stMyRoomUser{};
    ST_MYROOM_OWNER_INFO stOwnerInfo{};
    std::uint32_t dwOwnerUCID = 0;

    xPacket >> stMyRoomUser;
    xPacket >> stOwnerInfo;
    xPacket.XParse >> dwOwnerUCID;

    if (!XControlServer::Instance()->MyRoomEnterReq(stMyRoomUser, stOwnerInfo, pServer, dwOwnerUCID)) {
        // 对齐 IDA: 发送错误响应 (0xF2, 0x51)
        XSendPacket xSendPacket(0xF2, 0x51);
        xSendPacket.XParse << static_cast<int>(58209);  // 错误码
        PS_ENTER_MAP_RES stEnterMapRes{};
        ST_MYROOM_OWNER_INFO stEmptyOwner{};
        xSendPacket << stEnterMapRes;
        xSendPacket << stEmptyOwner;
        xSendPacket.XParse << dwOwnerUCID;
        pServer->SendEx(xSendPacket);
    }
    return true;
}

// 对齐 IDA 0x14003DFF0: ResCreateMatchingMazeFromGame (sub 0x3A)
bool CServerProcess::ResCreateMatchingMazeFromGame(XPacket& xPacket, int eType) {
    std::uint32_t dwMatchingID = 0;
    ST_CREATE_MAZE stCreateMaze{};
    PS_PARTY_INFO stPartyInfo{};

    xPacket.XParse >> dwMatchingID;
    xPacket >> stCreateMaze;
    xPacket >> stPartyInfo;

    XControlServer::Instance()->ResCreateMatchingMaze(dwMatchingID, stCreateMaze, stPartyInfo);
    return true;
}

// 对齐 IDA 0x14003D9E0: SyncUserPartyInfo (sub 0x34)
bool CServerProcess::SyncUserPartyInfo(XPacket& xPacket) {
    ST_PARTY_INFO stPartyInfo{};
    UXMapID uxPartyMapID{};
    UXMapID uxMapID{};
    std::uint32_t dwUCID = 0;

    xPacket >> stPartyInfo;
    xPacket.XParse >> uxPartyMapID.nMapID;
    xPacket.XParse >> uxMapID.nMapID;
    xPacket.XParse >> dwUCID;

    auto pServer = TXSingleton<XControlServer>::Instance();
    auto pUserInfo = pServer->GetUser(dwUCID);
    if (!pUserInfo) {
        return false;
    }

    if (stPartyInfo.byGroupType == 1 && stPartyInfo.nID > 0) {
        // Party type
        bool bExist = pServer->GetPartyManager().IsParty(stPartyInfo.nID);
        pServer->GetPartyManager().SetMember(stPartyInfo.nID, static_cast<int>(dwUCID), uxMapID);
        if (!bExist) {
            pServer->GetPartyManager().SetMazeID(stPartyInfo.nID, uxPartyMapID);
        }
    } else if (stPartyInfo.byGroupType == 2 && stPartyInfo.nID > 0) {
        // Force type
        bool bExist = pServer->GetForceManager().IsParty(stPartyInfo.nID);
        pServer->GetForceManager().SetMember(stPartyInfo.nID, static_cast<int>(dwUCID), uxMapID);
        if (!bExist) {
            pServer->GetForceManager().SetMazeID(stPartyInfo.nID, uxPartyMapID);
        }
    }

    pUserInfo->SetPartyInfo(stPartyInfo);
    return true;
}

// 对齐 IDA 0x14003E680: ReqMyRoomEnterRes (sub 0x53)
bool CServerProcess::ReqMyRoomEnterRes(XPacket& xPacket) {
    int nErrorCode = 0;
    ST_MYROOM_USER stEnterUser{};
    std::uint64_t biUxMapID = 0;
    ST_MYROOM_OWNER_INFO stOwnerInfo{};
    std::uint32_t dwOwnerUCID = 0;

    xPacket.XParse >> nErrorCode;
    xPacket >> stEnterUser;
    xPacket.XParse >> biUxMapID;
    xPacket >> stOwnerInfo;
    xPacket.XParse >> dwOwnerUCID;

    UXMapID uxMapID{};
    uxMapID.nMapID = static_cast<std::int64_t>(biUxMapID);

    auto pControlServer = TXSingleton<XControlServer>::Instance();
    CServer* pSendServer = pControlServer->GetServer(stEnterUser.nServerID);
    if (!pSendServer) {
        return true;
    }

    PS_ENTER_MAP_RES ssCreateMaze{};
    ssCreateMaze.dwUserID = stEnterUser.dwUCID;

    if (nErrorCode == 0) {
        auto pMyRoom = pControlServer->FindMyRoom(stOwnerInfo.dwOwnerUAID);
        if (!pMyRoom) {
            XSendPacket xSendPacket(0xF2, 0x51);
            xSendPacket.XParse << 100;  // 错误码
            xSendPacket << ssCreateMaze;
            xSendPacket << stOwnerInfo;
            xSendPacket.XParse << dwOwnerUCID;
            pSendServer->SendEx(xSendPacket);
            return true;
        }

        pMyRoom->EnterSucc();

        CServer* pMyRoomServer = GetClientPtr();
        SS_SERVER_INFO* pServerInfo = pMyRoomServer->GetServerInfo();
        strcpy_s(ssCreateMaze.szIP, sizeof(ssCreateMaze.szIP), pServerInfo->szPublicIP);
        ssCreateMaze.sPort = pServerInfo->sPort;
        ssCreateMaze.dwServerID = pMyRoomServer->GetServerID();
        ssCreateMaze.uxMapID = uxMapID;
        ssCreateMaze.nResult = 0;
        ssCreateMaze.byChangeType = 0;
        ssCreateMaze.bChangeServer = (pMyRoomServer != pSendServer);
    }

    XSendPacket xSendPacket(0xF2, 0x51);
    xSendPacket.XParse << nErrorCode;
    xSendPacket << ssCreateMaze;
    xSendPacket << stOwnerInfo;
    xSendPacket.XParse << dwOwnerUCID;
    pSendServer->SendEx(xSendPacket);
    return true;
}

// 对齐 IDA 0x14003EAD0: ReqPostSend (sub 0x56)
bool CServerProcess::ReqPostSend(XPacket& xPacket) {
    std::uint32_t dwRecvUCID = 0;
    std::uint32_t dwServerID = 0;
    std::uint16_t wPostCount = 0;
    ST_POST_DATA stPostData{};
    std::uint8_t byState = 0;
    ST_POST_CHAR stPostChar{};

    xPacket.XParse >> dwRecvUCID;
    xPacket.XParse >> dwServerID;
    xPacket.XParse >> wPostCount;
    xPacket >> stPostData;
    xPacket.XParse >> byState;

    if (byState == 0) {
        xPacket >> stPostChar;
    }

    auto pControlServer = TXSingleton<XControlServer>::Instance();
    if (!pControlServer->GetServer(dwServerID)) {
        return false;
    }

    auto pReceiver = pControlServer->GetUser(dwRecvUCID);
    if (!pReceiver) {
        return false;
    }

    XSendPacket xSendPacket(0xF2, 0x56);
    xSendPacket.XParse << dwRecvUCID;
    xSendPacket.XParse << wPostCount;
    xSendPacket << stPostData;
    xSendPacket.XParse << byState;
    xSendPacket << stPostChar;
    pReceiver->SendPacket(xSendPacket);
    return true;
}

// 对齐 IDA 0x14003DD80: ReqCreateMatchingMazeFromCommunity (sub 0x3E)
bool CServerProcess::ReqCreateMatchingMazeFromCommunity(XPacket& xPacket) {
    ST_CREATE_MAZE stCreateMaze{};
    PS_PARTY_INFO stCreateParty{};
    PS_FORCE_INFO stCreateForce{};
    std::uint32_t dwMatchingID = 0;

    xPacket >> stCreateMaze;
    xPacket >> stCreateParty;
    xPacket >> stCreateForce;
    xPacket.XParse >> dwMatchingID;

    auto pServer = TXSingleton<XControlServer>::Instance();

    if (stCreateMaze.stPartyInfo.byGroupType == 1) {
        pServer->CreateMatchingMaze(stCreateMaze, stCreateParty, dwMatchingID);
    } else if (stCreateMaze.stPartyInfo.byGroupType == 2) {
        pServer->CreateMatchingMaze(stCreateMaze, stCreateForce, dwMatchingID);
    }
    return true;
}

// 对齐 IDA 0x14003D750: EnterOtherMap_cheat (sub 0x32)
bool CServerProcess::EnterOtherMap_cheat(XPacket& xPacket) {
    PS_ENTER_MAP_REQ stEnterMap{};
    STPosInfo stPosInfo{};
    std::uint32_t dwTargetID = 0;

    xPacket >> stEnterMap;
    xPacket >> stPosInfo;
    xPacket.XParse >> dwTargetID;

    auto pControlServer = TXSingleton<XControlServer>::Instance();
    auto pUser = pControlServer->GetUser(dwTargetID);
    std::uint32_t dwServerID = 0;
    if (pUser) {
        dwServerID = pUser->GetServerID();
    }

    CServer* pServer = pControlServer->GetServer(dwServerID);
    if (!pServer) {
        return false;
    }

    pControlServer->GetWorldManager().ReqEnterMapToOther(pServer, stEnterMap, stPosInfo, dwTargetID);
    return true;
}

// 对齐 IDA 0x14003EFC0: ResCreateModeMaze (sub 0x2A)
bool CServerProcess::ResCreateModeMaze(XPacket& xPacket) {
    ST_CREATE_MODE_MAZE stCreateModeMaze{};
    xPacket >> stCreateModeMaze;

    auto pControlServer = TXSingleton<XControlServer>::Instance();
    pControlServer->ResCreateModeMaze(stCreateModeMaze);
    return true;
}

// 对齐 IDA 0x14003D8C0: ReqCheckPartyInMaze (sub 0x29)
bool CServerProcess::ReqCheckPartyInMaze(XPacket& xPacket) {
    PS_ENTER_MAP_REQ stEnterMap{};
    xPacket >> stEnterMap;

    CServer* pServer = GetClientPtr();
    auto pControlServer = TXSingleton<XControlServer>::Instance();

    if (stEnterMap.stPartyInfo.byGroupType == 1) {
        pControlServer->CheckPartyInMaze(pServer, stEnterMap);
    } else if (stEnterMap.stPartyInfo.byGroupType == 2) {
        pControlServer->CheckForceInMaze(pServer, stEnterMap);
    }
    return true;
}

// 对齐 IDA 0x14003EE20: ReqReEnterMap (sub 0x4B)
bool CServerProcess::ReqReEnterMap(XPacket& xPacket) {
    PS_ENTER_MAP_REQ stInfo{};
    xPacket >> stInfo;

    CServer* pServer = GetClientPtr();
    auto pControlServer = TXSingleton<XControlServer>::Instance();
    pControlServer->ReqReEnterMap(pServer, stInfo);
    return true;
}

// 对齐 IDA 0x14003F070: ReqUpdateRouletteEvent (sub 0x57)
bool CServerProcess::ReqUpdateRouletteEvent(XPacket& xPacket) {
    PS_ROULETTE_EVENT_UPDATE_SERVER psUpdateInfo{};
    xPacket >> psUpdateInfo;

    auto pControlServer = TXSingleton<XControlServer>::Instance();
    pControlServer->UpdateRouletteEventInfo(psUpdateInfo);
    return true;
}

// 对齐 IDA 0x14003EF20: ResMyRoomDelete (sub 0x5A)
bool CServerProcess::ResMyRoomDelete(XPacket& xPacket) {
    std::uint32_t dwOwnerUAID = 0;
    std::uint64_t biUxMapID = 0;

    xPacket.XParse >> dwOwnerUAID;
    xPacket.XParse >> biUxMapID;

    UXMapID uxMapID{};
    uxMapID.nMapID = static_cast<std::int64_t>(biUxMapID);

    CServer* pServer = GetClientPtr();
    auto pControlServer = TXSingleton<XControlServer>::Instance();
    pControlServer->DeleteMyRoomRes(dwOwnerUAID, uxMapID, pServer);
    return true;
}

// 对齐 IDA 0x14003D130: ReqChangeChannel (sub 0x35)
bool CServerProcess::ReqChangeChannel(XPacket& xPacket) {
    PS_ENTER_MAP_REQ stEnterMap{};
    xPacket >> stEnterMap;

    CServer* pServer = GetClientPtr();
    auto pControlServer = TXSingleton<XControlServer>::Instance();
    pControlServer->GetWorldManager().ReqChangeChannel(pServer, stEnterMap);
    return true;
}

// 对齐 IDA 0x14003DC40: ReqCheckEnterMaze (sub 0x38)
bool CServerProcess::ReqCheckEnterMaze(XPacket& xPacket) {
    ST_SERVER_CHECK_ENTER_MAZE stCheckEnter{};
    xPacket >> stCheckEnter;

    CServer* pServer = GetClientPtr();
    auto pControlServer = TXSingleton<XControlServer>::Instance();
    pControlServer->ReqCheckEnterMaze(pServer, stCheckEnter);
    return true;
}

// 对齐 IDA 0x14003DF20: ReqCreateMatchingModeMazeFromCommunity (sub 0x3F)
bool CServerProcess::ReqCreateMatchingModeMazeFromCommunity(XPacket& xPacket) {
    ST_CREATE_MODE_MAZE stCreateModeMaze{};
    xPacket >> stCreateModeMaze;

    auto pControlServer = TXSingleton<XControlServer>::Instance();
    pControlServer->CreateMatchingModeMaze(stCreateModeMaze);
    return true;
}

// 对齐 IDA 0x14003CEE0: ReqFindUser (sub 0x50)
bool CServerProcess::ReqFindUser(XPacket& xPacket) {
    wchar_t szOtherName[21] = {};
    short sLen = 0;
    std::uint32_t dwUCID = 0;
    std::uint8_t byState = 0;

    xPacket.XParse.GetWString(szOtherName, 21, sLen);
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> byState;

    auto pControlServer = TXSingleton<XControlServer>::Instance();
    auto pUser = pControlServer->GetUser(szOtherName);
    if (!pUser) {
        return false;
    }

    CServer* pServer = GetClientPtr();
    XSendPacket xSendPacket(0xF3, 0x58);
    xSendPacket.XParse << dwUCID;
    xSendPacket.XParse << pUser->GetCID();
    xSendPacket.XParse << pServer->GetServerID();
    xSendPacket.XParse << byState;
    pServer->SendEx(xSendPacket);
    return true;
}
