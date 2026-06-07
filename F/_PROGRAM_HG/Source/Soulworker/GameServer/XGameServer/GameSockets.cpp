// GameSockets.cpp
// Game Server Socket Implementations - Decompiled from IDA

#include "GameSockets.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerChat.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerWorldMode.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMapMaze.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerLeague.h"
#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XLoginServer/DayEventManager.h"
#include "Soulworker/GameServer/XLoginServer/RouletteEventManager.h"
#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"
#include "User.h"
#include "GameServer.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include <cstring>

// ============================================================================
// CGameControlSocket Implementation
// ============================================================================

// Per IDA 0x1401ca270: CGameControlSocket 构造函数
CGameControlSocket::CGameControlSocket()
    : XRelaySocket()
    , m_dwCachingLoad(0)
    , m_bSyncUserInfoReq(false)
    , m_bCachingLoadFinish(false)
{
    // Per IDA: 初始化成员变量
    std::memset(&m_myInfo, 0, sizeof(m_myInfo));
    std::memset(&m_relayInfo, 0, sizeof(m_relayInfo));
}

// Per IDA 0x1401ca300: CGameControlSocket 析构函数
CGameControlSocket::~CGameControlSocket() {
    // Per IDA: 清理资源
}

// Per IDA 0x1401ca350: SetMyInfo
void CGameControlSocket::SetMyInfo(XOption* pOption) {
    if (!pOption) {
        return;
    }

    // Per IDA: 设置服务器信息
    m_myInfo.dwID = pOption->GetServerID();
    m_myInfo.nGroup = pOption->GetGroupID();
    m_myInfo.nType = pOption->GetType();
    m_myInfo.nChannel = pOption->GetChannel();
    m_myInfo.nState = 1;
    m_myInfo.sPort = pOption->GetPort();
    m_myInfo.nCurUser = 0;
    m_myInfo.nMaxUser = pOption->GetMaxConnect();
    m_myInfo.sThreadCount = pOption->GetLogicThread();

    // Per IDA: 复制 IP 和名称
    std::strcpy(m_myInfo.szPrivateIP, pOption->GetIP());
    std::strcpy(m_myInfo.szPublicIP, pOption->GetPublicIP());
    std::strcpy(m_myInfo.szName, pOption->GetName());

    // Per IDA: 设置 relay 信息
    std::strcpy(m_relayInfo.szName, "CONTROL");
    std::uint16_t port = 0;
    pOption->GetServerPrivateIPAndPort(m_relayInfo.szName, m_relayInfo.szPrivateIP, port);
    m_relayInfo.sPort = static_cast<std::int16_t>(port);

    m_dwCachingLoad = 0;
    m_bSyncUserInfoReq = false;
    m_bCachingLoadFinish = false;
}

// Per IDA 0x1401ca500: ServerProcessEx
bool CGameControlSocket::ServerProcessEx(XPacket* xPacket) {
    switch (xPacket->GetSubCmd()) {
        case 0x08: return RecvServerShutdown(xPacket);
        case 0x09: return RecvTimeEvent(xPacket);
        case 0x14: return RecvValueEvent(xPacket);
        case 0x15: return RecvCashShopBanner(xPacket);
        case 0x21: return RecvCreateMazeReq(xPacket);
        case 0x22: return RecvCreateMazeRes(xPacket);
        case 0x25: return RecvRemoveMaze(xPacket);
        case 0x28: return RecvGoBackMazeRes(xPacket);
        case 0x30: return RecvCreateMap(xPacket);
        case 0x31: return RecvEnterMap(xPacket);
        case 0x32: return RecvCheckPartyInMaze(xPacket);
        case 0x33: return RecvUsersInfo(xPacket);
        case 0x38: return ResCheckEnterMaze(xPacket);
        case 0x41: return RecvPartyMatching(xPacket);
        case 0x42: return RecvForceMatching(xPacket);
        case 0x50: return RecvMyRoomEnterReq(xPacket);
        case 0x51: return RecvMyRoomEnter(xPacket);
        case 0x52: return RecvMyRoomCreate(xPacket);
        case 0x53: return RecvMyRoomDeleteReq(xPacket);
        case 0x54: return RecvEnterMapToOther(xPacket);
        case 0x56: return RecvPostSend(xPacket);
        case 0x58: return RecvFindUser(xPacket);
        case 0x59: return RecvMyRoomDeleteRes(xPacket);
        case 0x60: return RecvEnterUserInfo(xPacket);
        case 0x61: return RecvServerOptionUpdate(xPacket);
        case 0x66: return RecvServerDayEventBoosterList(xPacket);
        case 0x70: return RecvCachingComplete(xPacket);
        case 0x72: return RecvServerCreateModeMazeReq(xPacket);
        case 0x74: return RecvServerRouletteEvent(xPacket);
        default: return true;
    }
}

// Per IDA 0x1401cb1e0: PartyProcess
bool CGameControlSocket::PartyProcess(XPacket* xPacket) {
    // IDA: subcmd 8 -> RecvPartyEnterMaze
    if (xPacket->GetSubCmd() == 8) {
        return RecvPartyEnterMaze(xPacket);
    }
    return true;
}

// Per IDA 0x1401cb230: WorldModeProcess
bool CGameControlSocket::WorldModeProcess(XPacket* xPacket) {
    // TODO: IDA 精确还原
    return true;
}

// Per IDA 0x1401cb320: ForceProcess
bool CGameControlSocket::ForceProcess(XPacket* xPacket) {
    // TODO: IDA 精确还原
    return true;
}

// Per IDA 0x1401ca880: RecvFindUser
bool CGameControlSocket::RecvFindUser(XPacket* xPacket) {
    // Per IDA: 查找用户逻辑
    unsigned int dwUCID = 0;
    unsigned int dwTargetUCID = 0;
    unsigned int dwServerID = 0;
    unsigned char byState = 0;

    xPacket->XParse >> dwUCID;
    xPacket->XParse >> dwTargetUCID;
    xPacket->XParse >> dwServerID;
    xPacket->XParse >> byState;

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer->FindActorIDToUser(dwUCID);
    CUser* pTarget = pServer->FindActorIDToUser(dwTargetUCID);

    if (byState == 1) {
        // Per IDA: 状态1处理
        if (pTarget) {
            // TODO: 完整实现位置信息获取和任务分发
        }
    } else if (byState == 2) {
        // Per IDA: 状态2处理
        if (pUser) {
            // TODO: 完整实现位置信息获取和任务分发
        }
    }

    return true;
}

// Per IDA 0x1401cb370: RecvCreateMazeReq
bool CGameControlSocket::RecvCreateMazeReq(XPacket* xPacket) {
    // Per IDA: 创建迷宫请求处理
    // 需要ST_CREATE_MAZE结构和CLogicThreadManager
    // IDA显示：解析ST_CREATE_MAZE，创建lambda任务，分发到逻辑线程
    ST_CREATE_MAZE stCreatMaze = {};
    *xPacket >> stCreatMaze;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvCreateMazeReq - ST_CREATE_MAZE parsed, need CLogicThreadManager::DoJob");
    return true;
}

// Per IDA 0x1401cb830: RecvUserEnterServer
bool CGameControlSocket::RecvUserEnterServer(XPacket* xPacket) {
    // Per IDA: 用户进入服务器处理
    unsigned int dwUserID = 0;
    UXMapID uxMapID = {};
    ST_PARTY_INFO stPartyInfo = {};

    xPacket->XParse >> dwUserID;
    xPacket->XParse >> uxMapID.nMapID;
    *xPacket >> stPartyInfo;

    // Per IDA: 创建lambda任务分发到逻辑线程
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvUserEnterServer - parsed userID=%u, need CLogicThreadManager", dwUserID);
    return true;
}

// Per IDA 0x1401cb9a0: RecvCreateMap
bool CGameControlSocket::RecvCreateMap(XPacket* xPacket) {
    // Per IDA: 创建地图
    PS_CREATE_MAP stCreateMap = {};
    *xPacket >> stCreateMap;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvCreateMap - need CLogicThreadManager::DoJob");
    return true;
}

// Per IDA 0x1401cbb00: RecvEnterMapToOther
bool CGameControlSocket::RecvEnterMapToOther(XPacket* xPacket) {
    // Per IDA: 进入其他地图
    PS_ENTER_MAP_RES stEnterRes = {};
    STPosInfo stPosInfo = {};
    unsigned int dwTargetID = 0;

    *xPacket >> stEnterRes;
    *xPacket >> stPosInfo;
    xPacket->XParse >> dwTargetID;

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pReqUser = pServer ? pServer->FindActorIDToUser(stEnterRes.dwUserID) : nullptr;

    if (!pReqUser) {
        LogHelper::LogError("game.contents",
            "RecvEnterMapToOther error - Cant find request enter user[ ActorID: %d ] ( %d )",
            stEnterRes.dwUserID, 365);
        return false;
    }

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvEnterMapToOther - need CLogicThreadManager::DoJob");
    return true;
}

// Per IDA 0x1401cc340: RecvEnterMap
bool CGameControlSocket::RecvEnterMap(XPacket* xPacket) {
    // Per IDA: 进入地图处理
    PS_ENTER_MAP_RES stEnterRes = {};
    *xPacket >> stEnterRes;

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pReqUser = pServer ? pServer->FindActorIDToUser(stEnterRes.dwUserID) : nullptr;

    if (!pReqUser) {
        LogHelper::LogError("game.contents",
            "RecvEnterMap error - Cant find enter request user[ ActorID:%d ] ( %d )",
            stEnterRes.dwUserID, 440);
        return false;
    }

    // Per IDA: 检查用户Area是否存在
    // if (!pReqUser->GetArea()) {
    //     return false;
    // }

    // Per IDA: 增加任务计数并分发到逻辑线程
    // pReqUser->IncrementJobCount();
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvEnterMap - need CLogicThreadManager::DoJob");
    return true;
}

// Per IDA 0x1401ccda0: RecvCheckPartyInMaze
bool CGameControlSocket::RecvCheckPartyInMaze(XPacket* xPacket) {
    // Per IDA: 检查迷宫中的队伍
    UXMapID uxMazeID = {};
    PS_ENTER_MAP_REQ stEnterReq = {};
    int nResult = 0;

    xPacket->XParse >> uxMazeID.nMapID;
    *xPacket >> stEnterReq;
    xPacket->XParse >> nResult;

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stEnterReq.dwActorID) : nullptr;

    if (!pUser) {
        LogHelper::LogError("game.contents",
            "RecvCheckPartyInMaze error - Cant Find User[ ActorID:%d ] ( %d )",
            stEnterReq.dwActorID, 580);
        return false;
    }

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvCheckPartyInMaze - need CLogicThreadManager::DoJob");
    return true;
}

// Per IDA 0x1401cd4b0: RecvUserChangeServer
bool CGameControlSocket::RecvUserChangeServer(XPacket* xPacket) {
    // Per IDA: 用户切换服务器处理
    PS_RES_CHANGE_SERVER stRecvChangeServer = {};
    *xPacket >> stRecvChangeServer;

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stRecvChangeServer.dwActorID) : nullptr;

    if (!pUser) {
        LogHelper::LogError("game.contents",
            "RecvUserChangeServer error - Cant Find User[ ActorID:%d ] ( %d )",
            stRecvChangeServer.dwActorID, 636);
        return false;
    }

    // Per IDA: 检查用户Area是否存在
    // if (!pUser->GetArea()) {
    //     return false;
    // }

    // Per IDA: 增加任务计数并分发到逻辑线程
    // pUser->IncrementJobCount();
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvUserChangeServer - need CLogicThreadManager::DoJob");
    return true;
}

// Per IDA 0x1401d05f0: RecvServerShutdown
bool CGameControlSocket::RecvServerShutdown(XPacket* xPacket) {
    // Per IDA: 设置服务器接受关闭状态并踢出所有用户
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        pServer->SetServerAcceptClosed(true);
        pServer->KickoutUserAll(0x0B);  // 0x0B = kick reason
    }
    return true;
}
// Per IDA 0x1401ce810: RecvCreateMazeRes
bool CGameControlSocket::RecvCreateMazeRes(XPacket* xPacket) {
    // Per IDA: 创建迷宫响应
    PS_ENTER_MAP_RES stRecvEnterMapRes = {};
    *xPacket >> stRecvEnterMapRes;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stRecvEnterMapRes.dwUserID) : nullptr;

    if (pUser) {
        // Per IDA: 检查用户Area是否存在
        // if (!pUser->GetArea()) {
        //     return false;
        // }

        // Per IDA: 增加任务计数并分发到逻辑线程
        // pUser->IncrementJobCount();

        // Per IDA: 通过CLogicThreadManager分发任务
        // TODO: 完整实现需要CLogicThreadManager::DoJob
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvCreateMazeRes - userID=%u, need CLogicThreadManager::DoJob", stRecvEnterMapRes.dwUserID);
    }

    return true;
}
// Per IDA 0x1401cb6a0: RecvRemoveMaze
bool CGameControlSocket::RecvRemoveMaze(XPacket* xPacket) {
    // Per IDA: 移除迷宫
    // IDA: XParse::operator>>(&xPacket->XParse, (unsigned __int64 *)&uxMazeID);
    UXMapID uxMazeID = {};
    int nMazeType = 0;

    xPacket->XParse >> uxMazeID.nMapID;
    xPacket->XParse >> nMazeType;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvRemoveMaze - uxMazeID parsed, need CLogicThreadManager::DoJob");
    return true;
}
// Per IDA 0x1401ce150: RecvGoBackMazeRes
bool CGameControlSocket::RecvGoBackMazeRes(XPacket* xPacket) {
    // Per IDA: 返回迷宫响应
    ST_GO_BACK_MAZE stGoBackMaze = {};
    *xPacket >> stGoBackMaze;

    if (!stGoBackMaze.bResult) {
        return false;
    }

    UXMapID uxInsID = {};
    uxInsID.nMapID = stGoBackMaze.uxMapID.nMapID;

    UXMapID zeroID = {};
    zeroID.nMapID = 0;

    if (uxInsID.nMapID != zeroID.nMapID) {
        // Per IDA: 通过CLogicThreadManager分发任务
        // TODO: 完整实现需要CLogicThreadManager::DoJob
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvGoBackMazeRes - uxInsID=%lld, need CLogicThreadManager::DoJob", uxInsID.nMapID);
    }

    return true;
}
// Per IDA 0x1401cf9e0: RecvUsersInfo
bool CGameControlSocket::RecvUsersInfo(XPacket* xPacket) {
    // Per IDA: 同步用户信息
    int nCount = 0;
    xPacket->XParse >> nCount;

    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        pServer->SyncUsersInfo(6, 0);
    }

    return true;
}
// Per IDA 0x1401cfe20: RecvPartyMatching
bool CGameControlSocket::RecvPartyMatching(XPacket* xPacket) {
    // Per IDA: 组队匹配
    ST_CREATE_MAZE stCreateMaze = {};
    PS_PARTY_INFO stPartyInfo = {};
    unsigned int dwMatchingID = 0;

    *xPacket >> stCreateMaze;
    *xPacket >> stPartyInfo;
    xPacket->XParse >> dwMatchingID;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvPartyMatching - matchingID=%u, need CLogicThreadManager::DoJob", dwMatchingID);
    return true;
}
// Per IDA 0x1401d0040: RecvForceMatching
bool CGameControlSocket::RecvForceMatching(XPacket* xPacket) {
    // Per IDA: Force匹配
    ST_CREATE_MAZE stCreateMaze = {};
    PS_FORCE_INFO stForceInfo = {};
    unsigned int dwMatchingID = 0;

    *xPacket >> stCreateMaze;
    *xPacket >> stForceInfo;
    xPacket->XParse >> dwMatchingID;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvForceMatching - matchingID=%u, need CLogicThreadManager::DoJob", dwMatchingID);
    return true;
}
// Per IDA 0x1401d01d0: RecvForceEnterMaze
bool CGameControlSocket::RecvForceEnterMaze(XPacket* xPacket) {
    // Per IDA: Force进入迷宫
    unsigned int dwForceID = 0;
    PS_ENTER_MAP_RES stRecvEnterMap = {};

    xPacket->XParse >> dwForceID;
    *xPacket >> stRecvEnterMap;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stRecvEnterMap.dwUserID) : nullptr;

    if (pUser) {
        // Per IDA: 检查用户Area是否存在
        // if (!pUser->GetArea()) {
        //     return false;
        // }

        // Per IDA: 增加任务计数并分发到逻辑线程
        // pUser->IncrementJobCount();

        // Per IDA: 通过CLogicThreadManager分发任务
        // TODO: 完整实现需要CLogicThreadManager::DoJob
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvForceEnterMaze - forceID=%u, userID=%u, need CLogicThreadManager::DoJob", dwForceID, stRecvEnterMap.dwUserID);
    }

    return true;
}
// Per IDA 0x1401d0900: RecvMyRoomEnterReq
bool CGameControlSocket::RecvMyRoomEnterReq(XPacket* xPacket) {
    // Per IDA: 进入我的房间请求
    UXMapID uxMapID = {};
    ST_MYROOM_USER stEnterUser = {};
    unsigned int dwOwnerUCID = 0;

    xPacket->XParse >> uxMapID.nMapID;
    *xPacket >> stEnterUser;
    xPacket->XParse >> dwOwnerUCID;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvMyRoomEnterReq - ownerUCID=%u, need CLogicThreadManager::DoJob", dwOwnerUCID);
    return true;
}
// Per IDA 0x1401d0b00: RecvMyRoomEnter
bool CGameControlSocket::RecvMyRoomEnter(XPacket* xPacket) {
    // Per IDA: 进入我的房间
    int nErrorCode = 0;
    PS_ENTER_MAP_RES stEnterMapRes = {};
    ST_MYROOM_OWNER_INFO stOwnerInfo = {};
    unsigned int dwOwnerUCID = 0;

    xPacket->XParse >> nErrorCode;
    *xPacket >> stEnterMapRes;
    *xPacket >> stOwnerInfo;
    xPacket->XParse >> dwOwnerUCID;

    LogHelper::LogError("game.contents", "<Control RecvMyRoomEnter> Error:%d UserID:%d", nErrorCode, stEnterMapRes.dwUserID);

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stEnterMapRes.dwUserID) : nullptr;

    if (pUser) {
        // Per IDA: 检查用户Area是否存在
        // if (!pUser->GetArea()) {
        //     return false;
        // }

        // Per IDA: 增加任务计数并分发到逻辑线程
        // pUser->IncrementJobCount();

        // Per IDA: 通过CLogicThreadManager分发任务
        // TODO: 完整实现需要CLogicThreadManager::DoJob
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvMyRoomEnter - userID=%u, error=%d, need CLogicThreadManager::DoJob", stEnterMapRes.dwUserID, nErrorCode);
    }

    return true;
}
// Per IDA 0x1401d0630: RecvMyRoomCreate
bool CGameControlSocket::RecvMyRoomCreate(XPacket* xPacket) {
    // Per IDA: 创建我的房间
    ST_MYROOM_USER stEnterUser = {};
    ST_MYROOM_OWNER_INFO stOwnerInfo = {};
    UXMapID uxMapID = {};
    unsigned int dwOwnerUCID = 0;

    *xPacket >> stEnterUser;
    *xPacket >> stOwnerInfo;
    xPacket->XParse >> uxMapID.nMapID;
    xPacket->XParse >> dwOwnerUCID;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvMyRoomCreate - ownerUCID=%u, need CLogicThreadManager::DoJob", dwOwnerUCID);
    return true;
}
// Per IDA 0x1401d31d0: RecvMyRoomDeleteReq
bool CGameControlSocket::RecvMyRoomDeleteReq(XPacket* xPacket) {
    // Per IDA: 删除我的房间请求
    unsigned int dwOwnerUAID = 0;
    UXMapID biMapID = {};

    xPacket->XParse >> dwOwnerUAID;
    xPacket->XParse >> biMapID.nMapID;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvMyRoomDeleteReq - ownerUAID=%u, mapID=%lld, need CLogicThreadManager::DoJob", dwOwnerUAID, biMapID.nMapID);
    return true;
}
// Per IDA 0x1401d2350: RecvPostSend
bool CGameControlSocket::RecvPostSend(XPacket* xPacket) {
    // Per IDA: 邮件发送
    unsigned int dwRecvUCID = 0;
    std::uint16_t wPostCount = 0;
    ST_POST_DATA stPostData = {};
    std::uint8_t byState = 0;
    ST_POST_CHAR stPostChar = {};

    xPacket->XParse >> dwRecvUCID;
    xPacket->XParse >> wPostCount;
    *xPacket >> stPostData;
    xPacket->XParse >> byState;

    if (!byState) {
        *xPacket >> stPostChar;
    }

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwRecvUCID) : nullptr;

    if (pUser) {
        // Per IDA: 检查用户Area是否存在
        // if (!pUser->GetArea()) {
        //     return false;
        // }

        // Per IDA: 增加任务计数并分发到逻辑线程
        // pUser->IncrementJobCount();

        // Per IDA: 通过CLogicThreadManager分发任务
        // TODO: 完整实现需要CLogicThreadManager::DoJob
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvPostSend - recvUCID=%u, postCount=%u, need CLogicThreadManager::DoJob", dwRecvUCID, wPostCount);
        return true;
    }

    return false;
}
// Per IDA 0x1401d3300: RecvMyRoomDeleteRes
bool CGameControlSocket::RecvMyRoomDeleteRes(XPacket* xPacket) {
    // Per IDA: 删除我的房间响应
    UXMapID biMapID = {};

    xPacket->XParse >> biMapID.nMapID;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvMyRoomDeleteRes - mapID=%lld, need CLogicThreadManager::DoJob", biMapID.nMapID);
    return true;
}
// Per IDA 0x1401d33e0: RecvEnterUserInfo
bool CGameControlSocket::RecvEnterUserInfo(XPacket* xPacket) {
    // Per IDA: 进入用户信息
    unsigned int dwActorID = 0;
    std::uint8_t byBlockType = 0;

    xPacket->XParse >> dwActorID;
    xPacket->XParse >> byBlockType;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (pUser) {
        // Per IDA: 检查用户Area是否存在
        // if (!pUser->GetArea()) {
        //     return false;
        // }

        // Per IDA: 增加任务计数并分发到逻辑线程
        // pUser->IncrementJobCount();

        // Per IDA: 通过CLogicThreadManager分发任务
        // TODO: 完整实现需要CLogicThreadManager::DoJob
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvEnterUserInfo - actorID=%u, blockType=%u, need CLogicThreadManager::DoJob", dwActorID, byBlockType);
    }

    return true;
}
// Per IDA 0x1401d3670: RecvServerOptionUpdate
bool CGameControlSocket::RecvServerOptionUpdate(XPacket* xPacket) {
    // Per IDA: 服务器选项更新处理
    PS_CONTENTS_INFO stServerOptionInfo = {};
    *xPacket >> stServerOptionInfo;

    // Per IDA: 打印所有选项信息
    for (int i = 0; i < 14; ++i) {
        LogHelper::LogDebug("game.contents", "Server Option Info:%d ( %d )", stServerOptionInfo.bContents[i], 1657);
    }

    // Per IDA: 设置服务器内容选项
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        for (int eType = 0; eType < 14; ++eType) {
            pServer->GetResourceMgr().SetServerContents(eType, stServerOptionInfo.bContents[eType]);
        }
    }

    // Per IDA: 通过CLogicThreadManager分发到所有线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvServerOptionUpdate - need CLogicThreadManager::DoJobAllThread");
    return true;
}
// Per IDA 0x1401d14c0: RecvUserNotice
bool CGameControlSocket::RecvUserNotice(XPacket* xPacket) {
    // Per IDA: 用户公告通知处理
    PS_CHAT_NOTICE stNotice = {};
    *xPacket >> stNotice;

    // Per IDA: 通过CLogicThreadManager分发到所有线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread 和 lambda
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvUserNotice - type=%d, messageCode=%d, need CLogicThreadManager::DoJobAllThread",
                    stNotice.byType, stNotice.nMessageCode);
    return true;
}

// Per IDA 0x1401d4a10: RecvServerDayEventBoosterList - moved to proper location

// Per IDA 0x1401d1680: RecvUserMegaPhone
bool CGameControlSocket::RecvUserMegaPhone(XPacket* xPacket) {
    // Per IDA: 喇叭消息处理
    PS_CHAT_MEGAPHONE stMegaPhone = {};
    PS_CHAT_ITEM_LINK_FOR_SERVER psLinkItemInfo = {};

    *xPacket >> stMegaPhone;
    *xPacket >> psLinkItemInfo;

    // Per IDA: 通过CLogicThreadManager分发到所有线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread 和 lambda
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvUserMegaPhone - ucid=%u, need CLogicThreadManager::DoJobAllThread",
                    stMegaPhone.dwUCID);
    return true;
}

// Per IDA 0x1401d1ab0: RecvUserTradePasswordState
bool CGameControlSocket::RecvUserTradePasswordState(XPacket* xPacket) {
    // Per IDA: 交易密码状态处理
    unsigned int dwUCID = 0;
    std::uint8_t byState = 0;

    xPacket->XParse >> dwUCID;
    xPacket->XParse >> byState;

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwUCID) : nullptr;

    if (!pUser) {
        return false;
    }

    // Per IDA: 检查用户Area是否存在
    // if (!pUser->GetArea()) {
    //     return false;
    // }

    // Per IDA: 增加任务计数
    // pUser->IncrementJobCount();

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvUserTradePasswordState - ucid=%u, state=%u, need CLogicThreadManager::DoJob",
                    dwUCID, byState);
    return true;
}

// Per IDA 0x1401d1da0: RecvUserWhisperRes
bool CGameControlSocket::RecvUserWhisperRes(XPacket* xPacket) {
    // Per IDA: 私聊响应处理
    unsigned int dwUID = 0;
    PS_CHAT_WHISPER stChatWhisper = {};
    std::uint8_t byResult = 0;

    xPacket->XParse >> dwUID;
    *xPacket >> stChatWhisper;
    xPacket->XParse >> byResult;

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwUID) : nullptr;

    if (!pUser) {
        return false;
    }

    // Per IDA: 检查用户Area是否存在
    // if (!pUser->GetArea()) {
    //     return false;
    // }

    // Per IDA: 增加任务计数
    // pUser->IncrementJobCount();

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvUserWhisperRes - uid=%u, result=%u, need CLogicThreadManager::DoJob",
                    dwUID, byResult);
    return true;
}
// Per IDA 0x1401d37e0: RecvCachingComplete
bool CGameControlSocket::RecvCachingComplete(XPacket* xPacket) {
    // Per IDA: 缓存加载完成通知
    xPacket->XParse >> m_dwCachingLoad;

    if (m_dwCachingLoad == 7 && !m_bSyncUserInfoReq) {
        m_bSyncUserInfoReq = true;
        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            pServer->SyncUsersInfo(6, 0);
        }
    }

    LogHelper::LogInfo("game.system", "<Control RecvCachingComplete> CachingState : [%d]", m_dwCachingLoad);

    if (m_dwCachingLoad == 15) {
        m_bCachingLoadFinish = true;
    }

    return true;
}
// RecvServerCreateModeMazeReq moved to proper location below
// RecvServerRouletteEvent moved to proper location below
// Per IDA 0x1401cfa20: RecvPartyEnterMaze
bool CGameControlSocket::RecvPartyEnterMaze(XPacket* xPacket) {
    // Per IDA: 组队进入迷宫
    unsigned int dwPartyID = 0;
    PS_ENTER_MAP_RES stRecvEnterMap = {};

    xPacket->XParse >> dwPartyID;
    *xPacket >> stRecvEnterMap;

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stRecvEnterMap.dwUserID) : nullptr;

    if (!pUser) {
        return true;
    }

    // Per IDA: 检查用户Area是否存在
    // if (!pUser->GetArea()) {
    //     return false;
    // }

    // Per IDA: 增加任务计数并分发到逻辑线程
    // pUser->IncrementJobCount();
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvPartyEnterMaze - partyID=%u, need CLogicThreadManager::DoJob", dwPartyID);
    return true;
}
// Per IDA 0x1401cf2e0: RecvUserKickout
bool CGameControlSocket::RecvUserKickout(XPacket* xPacket) {
    // Per IDA: 踢出用户
    PS_KICK_USER_INFO psKick = {};
    *xPacket >> psKick;

    // Per IDA: 查找用户 (使用 FindUIDToUser)
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindUIDToUser(psKick.dwUAID) : nullptr;

    if (!pUser) {
        return true;
    }

    // Per IDA: 检查用户Area是否存在
    // if (!pUser->GetArea()) {
    //     return false;
    // }

    // Per IDA: 增加任务计数并分发到逻辑线程
    // pUser->IncrementJobCount();
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvUserKickout - need CLogicThreadManager::DoJob");
    return true;
}
// Per IDA 0x1401cf690: RecvUpdateChannelAll
bool CGameControlSocket::RecvUpdateChannelAll(XPacket* xPacket) {
    // Per IDA: 更新所有频道信息
    PS_CHANNEL_INFO stChannel = {};
    *xPacket >> stChannel;

    // Per IDA: 通过CLogicThreadManager分发到所有线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvUpdateChannelAll - need CLogicThreadManager::DoJobAllThread");
    return true;
}

// Per IDA 0x1401cf890: RecvUpdateChannel
bool CGameControlSocket::RecvUpdateChannel(XPacket* xPacket) {
    // Per IDA: 更新单个频道
    std::uint16_t wMapID = 0;
    ST_CHANNEL_INFO stChannel = {};

    xPacket->XParse >> wMapID;
    *xPacket >> stChannel;

    // Per IDA: 通过CLogicThreadManager分发到所有线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvUpdateChannel - mapID=%u, need CLogicThreadManager::DoJobAllThread", wMapID);
    return true;
}

// Per IDA 0x1401cd920: RecvChangeChannelRes
bool CGameControlSocket::RecvChangeChannelRes(XPacket* xPacket) {
    // Per IDA: 更换频道响应
    PS_ENTER_MAP_RES stEnterRes = {};
    *xPacket >> stEnterRes;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pReqUser = pServer ? pServer->FindActorIDToUser(stEnterRes.dwUserID) : nullptr;

    if (!pReqUser) {
        LogHelper::LogError("game.contents",
            "RecvChangeChannelRes error - Cant find enter request user[ ActorID:%d ] ( %d )",
            stEnterRes.dwUserID, 672);
        return false;
    }

    // Per IDA: 检查用户Area是否存在
    // if (!pReqUser->GetArea()) {
    //     return false;
    // }

    // Per IDA: 增加任务计数并分发到逻辑线程
    // pReqUser->IncrementJobCount();

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvChangeChannelRes - userID=%u, need CLogicThreadManager::DoJob", stEnterRes.dwUserID);

    return false;  // Per IDA: 返回 false 表示继续处理
}

// Per IDA 0x1401d2320: OnConnect
void CGameControlSocket::OnConnect() {
    // Per IDA: 重置同步标志并调用父类
    m_bSyncUserInfoReq = false;
    XRelaySocket::OnConnect();
}

// Per IDA 0x1401d22f0: OnDisConnect
void CGameControlSocket::OnDisConnect() {
    // Per IDA: 设置所有用户信息同步状态
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        pServer->SetAllUserInfoSync(2, false);
    }
    XRelaySocket::OnDisConnect();
}

// Per IDA 0x1401d2180: OnStartThread
void CGameControlSocket::OnStartThread() {
    // Per IDA: 线程启动循环，初始化 Havok 内存系统
    // while (m_eState != 4) { ... }
    // TODO: 完整实现需要 hkMemorySystem 和 hkBaseSystem
}

// Per IDA 0x1401d2280: OnFinishThread
void CGameControlSocket::OnFinishThread() {
    // Per IDA: 线程结束，清理 Havok 内存系统
    // hkBaseSystem::quitThread(&result);
    // TODO: 完整实现需要 hkBaseSystem 和 hkMemorySystem
}

// Per IDA 0x1401d2c50: RecvTimeEvent
bool CGameControlSocket::RecvTimeEvent(XPacket* xPacket) {
    // Per IDA: 时间事件处理
    ST_GM_TIME_EVENT_INFO stInfo = {};
    *xPacket >> stInfo;

    // Per IDA: 添加时间事件
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        // CTimeEventMgr::AddTimeEvent(&pServer->m_TimeEventMgr, &stInfo);
    }

    // Per IDA: 如果不使用，分发到所有线程
    if (!stInfo.byteUse) {
        // TODO: 完整实现需要 CLogicThreadManager::DoJobAllThread
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvTimeEvent - need CLogicThreadManager::DoJobAllThread");
    }
    return true;
}

// Per IDA 0x1401d2da0: RecvValueEvent
bool CGameControlSocket::RecvValueEvent(XPacket* xPacket) {
    // Per IDA: 数值事件处理
    PS_GM_VALUE_EVENT_LIST psList = {};
    *xPacket >> psList;

    // Per IDA: 添加数值事件
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        // CTimeEventMgr::AddValueEvent(&pServer->m_TimeEventMgr, &psList);
    }
    return true;
}

// Per IDA 0x1401d2e30: RecvCashShopBanner
bool CGameControlSocket::RecvCashShopBanner(XPacket* xPacket) {
    // Per IDA: 商城横幅处理
    ST_BANNER_LIST stList_New = {};
    *xPacket >> stList_New;

    // Per IDA: 获取旧的横幅列表并比较
    // ST_BANNER_LIST stList_Old;
    // XResourceMgr::GetBannerInfo(&stList_Old);
    // 比较逻辑...

    // Per IDA: 如果有变化，更新并分发
    // TODO: 完整实现需要 XResourceMgr::SetBannerInfo 和 CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvCashShopBanner - need XResourceMgr and CLogicThreadManager");
    return true;
}

// Per IDA 0x1401d3a10: RecvWorldModeStart
bool CGameControlSocket::RecvWorldModeStart(XPacket* xPacket) {
    // Per IDA: WorldMode开始处理
    ST_WORLD_MODE_INFO sInfo = {};
    *xPacket >> sInfo;

    // Per IDA: 通过CLogicThreadManager分发到所有线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvWorldModeStart - modeID=%d, need CLogicThreadManager::DoJobAllThread", sInfo.nModeID);

    // Per IDA: 检查PVP区域选项
    XGameServer* pServer = XGameServer::Instance();
    if (pServer && pServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_PVP_DISTRICT)) {
        // TODO: SetRealPVPOption 需要实现
        // pServer->GetResourceMgr().SetRealPVPOption(1);
    }

    return true;
}

// Per IDA 0x1401d3c00: RecvWorldModeUpdate
bool CGameControlSocket::RecvWorldModeUpdate(XPacket* xPacket) {
    // Per IDA: WorldMode更新处理
    PS_WORLD_MODE_UPDATE stUpdate = {};
    *xPacket >> stUpdate;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvWorldModeUpdate - modeID=%d, monsterClear=%d, need CLogicThreadManager::DoJob",
                    stUpdate.stInfo.nModeID, stUpdate.nMonsterClearCount);
    return true;
}

// Per IDA 0x1401d3d00: RecvWorldModeFinish
bool CGameControlSocket::RecvWorldModeFinish(XPacket* xPacket) {
    // Per IDA: WorldMode结束处理
    PS_WORLD_MODE_FINISH sInfo = {};
    xPacket->XParse >> sInfo.nModeID;
    xPacket->XParse >> sInfo.uxMapID.nMapID;
    xPacket->XParse >> sInfo.nFinishTime;
    xPacket->XParse >> sInfo.nModeDateID;
    xPacket->XParse >> sInfo.nMonsterClearCount;
    short outLen = 0;
    xPacket->XParse.GetWString(sInfo.strKiller, 21, outLen);
    int nSuccess = 0;
    xPacket->XParse >> nSuccess;
    sInfo.bSuccess = (nSuccess != 0);

    // Per IDA: 通过CLogicThreadManager分发到所有线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvWorldModeFinish - modeID=%d, success=%d, need CLogicThreadManager::DoJobAllThread",
                    sInfo.nModeID, sInfo.bSuccess ? 1 : 0);

    // Per IDA: 检查模式表和PVP选项
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        TB_MODE_DISTRICT6* pTBMode = pServer->GetResourceMgr().GetTB_MODE_DISTRICT6(sInfo.nModeID);
        if (pTBMode && !pTBMode->After_Mode_ID) {
            if (!pServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_PVP_DISTRICT)) {
                // TODO: GetRealPVPOption/SetRealPVPOption 需要实现
                // if (pServer->GetResourceMgr().GetRealPVPOption()) {
                //     pServer->GetResourceMgr().SetRealPVPOption(0);
                // }
            }
        }
    }

    return true;
}

// Per IDA 0x1401d3fc0: RecvWorldModeSync
bool CGameControlSocket::RecvWorldModeSync(XPacket* xPacket) {
    // Per IDA: WorldMode同步处理
    ST_WORLD_MODE_INFO_VEC stInfoVec = {};
    *xPacket >> stInfoVec;

    // Per IDA: 通过CLogicThreadManager分发到所有线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvWorldModeSync - count=%zu, need CLogicThreadManager::DoJobAllThread",
                    stInfoVec.vecInfo.size());

    // Per IDA: 检查是否有PVP模式
    for (const auto& info : stInfoVec.vecInfo) {
        if (info.nState == 1) {
            XGameServer* pServer = XGameServer::Instance();
            if (pServer && pServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_PVP_DISTRICT)) {
                // TODO: SetRealPVPOption 需要实现
                // pServer->GetResourceMgr().SetRealPVPOption(1);
            }
            break;
        }
    }

    return true;
}

// Per IDA 0x1401d42c0: RecvWorldModeCommand
bool CGameControlSocket::RecvWorldModeCommand(XPacket* xPacket) {
    // Per IDA: WorldMode命令处理
    unsigned int dwUCID = 0;
    ST_WORLD_MODE_INFO_VEC stModeList = {};

    xPacket->XParse >> dwUCID;
    *xPacket >> stModeList;

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwUCID) : nullptr;

    if (!pUser) {
        return false;
    }

    // Per IDA: 检查用户Area是否存在
    // if (!pUser->GetArea()) {
    //     return false;
    // }

    // Per IDA: 增加任务计数
    // pUser->IncrementJobCount();

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvWorldModeCommand - ucid=%u, count=%zu, need CLogicThreadManager::DoJob",
                    dwUCID, stModeList.vecInfo.size());
    return true;
}

// Per IDA 0x1401d4920: RecvWorldModeClear
bool CGameControlSocket::RecvWorldModeClear(XPacket* xPacket) {
    // Per IDA: WorldMode清除处理
    ST_WORLD_MODE_INFO stInfo = {};
    *xPacket >> stInfo;

    // Per IDA: 通过CLogicThreadManager分发到所有线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvWorldModeClear - modeID=%d, need CLogicThreadManager::DoJobAllThread", stInfo.nModeID);
    return true;
}

// Per IDA 0x1401d5a40: RecvWorldModeComplete
bool CGameControlSocket::RecvWorldModeComplete(XPacket* xPacket) {
    // Per IDA: WorldMode完成处理
    PS_WORLD_MODE_COMPLETE stComplete = {};
    unsigned int dwMonsterID = 0;

    *xPacket >> stComplete;
    xPacket->XParse >> dwMonsterID;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvWorldModeComplete - modeID=%d, monsterID=%u, need CLogicThreadManager::DoJob",
                    stComplete.nModeID, dwMonsterID);
    return true;
}

// Per IDA 0x1401d4f20: ResCheckEnterMaze
bool CGameControlSocket::ResCheckEnterMaze(XPacket* xPacket) {
    // Per IDA: 检查进入迷宫响应
    ST_SERVER_CHECK_ENTER_MAZE stInfo = {};
    *xPacket >> stInfo;

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stInfo.dwUCID) : nullptr;

    if (!pUser) {
        return true;
    }

    // Per IDA: 检查用户Area是否存在
    // if (!pUser->GetArea()) {
    //     return false;
    // }

    // Per IDA: 增加任务计数
    // pUser->IncrementJobCount();

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "ResCheckEnterMaze - ucid=%u, need CLogicThreadManager::DoJob", stInfo.dwUCID);
    return true;
}

// Per IDA 0x1401d3890: SendCheck
void CGameControlSocket::SendCheck(XSendPacket* pPacket) {
    // Per IDA: 检查是否可以发送
    if (IsCanSend(this)) {
        XIOCPClient::Send(*pPacket);
    } else {
        LogHelper::LogError("game.contents",
            "Control Send error[ Caching:%d, Sync:%d ](%d)",
            m_dwCachingLoad, m_bSyncUserInfoReq, 1694);
    }
}

// Per IDA 0x1401d3900: SendCmd
bool CGameControlSocket::SendCmd(XSendPacket* pPacket, CUser* pUser, std::uint8_t byMainCmd, std::uint8_t bySubCmd) {
    // Per IDA: 检查是否可以发送
    if (IsCanSend(this)) {
        if (XIOCPClient::Send(*pPacket)) {
            return true;
        } else {
            if (pUser) {
                pUser->SendErrorMessage(byMainCmd, bySubCmd, 0xC35A);
            }
            return false;
        }
    } else {
        LogHelper::LogError("game.contents",
            "ControlSendEx error[ Caching:%d, Sync:%d ](%d)",
            m_dwCachingLoad, m_bSyncUserInfoReq, 1705);
        if (pUser) {
            pUser->SendErrorMessage(byMainCmd, bySubCmd, 0xC35A);
        }
        return false;
    }
}

// Per IDA 0x1401d39d0: IsCanSend
bool CGameControlSocket::IsCanSend() {
    // Per IDA: 检查缓存加载完成且Relay连接就绪
    return m_dwCachingLoad == 15 && IsConnection();
}

// Per IDA 0x1402f6cf0: IsCachingLoadFinish
// Note: This is defined inline in the header, but keeping for reference
// bool CGameControlSocket::IsCachingLoadFinish() {
//     return m_bCachingLoadFinish;
// }

// Per IDA 0x1401d4a10: RecvServerDayEventBoosterList
bool CGameControlSocket::RecvServerDayEventBoosterList(XPacket* xPacket) {
    // Per IDA: 每日活动增益列表处理
    PS_DAY_EVENT_LIST psList = {};
    *xPacket >> psList;

    // Per IDA: 设置每日活动事件
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        // CDayEventMgr::SetDayEvent(&pServer->m_DayEventMgr, &psList);
    }

    // Per IDA: 通过CLogicThreadManager分发到所有线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvServerDayEventBoosterList - need CLogicThreadManager::DoJobAllThread");
    return true;
}

// Per IDA 0x1401d4bc0: RecvServerCreateModeMazeReq
bool CGameControlSocket::RecvServerCreateModeMazeReq(XPacket* xPacket) {
    // Per IDA: 创建模式迷宫请求
    ST_CREATE_MODE_MAZE stCreatModeMaze = {};
    *xPacket >> stCreatModeMaze;

    // Per IDA: 通过CLogicThreadManager分发任务
    // TODO: 完整实现需要CLogicThreadManager::DoJob
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvServerCreateModeMazeReq - need CLogicThreadManager::DoJob");
    return true;
}

// Per IDA 0x1401d5310: RecvServerRouletteEvent
bool CGameControlSocket::RecvServerRouletteEvent(XPacket* xPacket) {
    // Per IDA: 轮盘事件处理
    PS_GM_ROULETTE_EVENT psInfo = {};
    bool bSend = false;

    *xPacket >> psInfo;
    xPacket->XParse >> bSend;

    // Per IDA: 设置轮盘事件
    XGameServer* pServer = XGameServer::Instance();
    if (pServer) {
        // CTimeEventMgr::SetRouletteEvent(&pServer->m_TimeEventMgr, &psInfo);
    }

    // Per IDA: 通过CLogicThreadManager分发到所有线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvServerRouletteEvent - need CLogicThreadManager::DoJobAllThread");
    return true;
}

// ============================================================================
// CCommunitySocket Implementation
// ============================================================================

bool CCommunitySocket::PartyProcess(XPacket* xPacket) {
    switch (xPacket->GetSubCmd()) {
        case 0x01: return RecvPartyCreate(xPacket);
        case 0x02: return RecvPartyJoinMember(xPacket);
        case 0x03: return RecvPartyLeaveMember(xPacket);
        case 0x04: return RecvPartyChangeMaster(xPacket);
        case 0x05: return RecvUpdatePartyMember(xPacket);
        case 0x06: return RecvPartyDelete(xPacket);
        case 0x08: return RecvPartyEnterMaze(xPacket);
        case 0x09: return RecvPartyUpdateInfo(xPacket);
        case 0x10: return RecvPartyEnterServer(xPacket);
        case 0x11: return RecvPartyInvite(xPacket);
        case 0x12: return RecvPartyAccept(xPacket);
        case 0x13: return RecvPartyReject(xPacket);
        case 0x14: return RecvPartyMessage(xPacket);
        case 0x20: return RecvPartyMatchingEnter(xPacket);
        case 0x21: return RecvPartyMatchingExit(xPacket);
        case 0x22: return RecvPartyMatchingCheck(xPacket);
        case 0x23: return RecvPartyMatchingReset(xPacket);
        case 0x24: return RecvPartyMatchingWait(xPacket);
        case 0x25: return RecvPartyRecruitAdd(xPacket);
        case 0x26: return RecvPartyRecruitDel(xPacket);
        case 0x27: return RecvPartyRecruitApply(xPacket);
        case 0x28: return RecvPartyRecruitApplyAccept(xPacket);
        case 0x29: return RecvPartyRecruitApplyReject(xPacket);
        case 0x2A: return RecvPartyRecruitApplyUpdate(xPacket);
        case 0x2B: return RecvPartyRecruitList(xPacket);
        case 0x2C: return RecvPartyRecruitMyApplyList(xPacket);
        case 0x2D: return RecvPartyRecruitApplyList(xPacket);
        case 0x2E: return RecvPartyRecruitInfo(xPacket);
        case 0x2F: return RecvPartyRecruitApplyDel(xPacket);
        case 0x30: return RecvPartyRecruitApplyInfo(xPacket);
        case 0x31: return RecvPartyRecruitApplyNotice(xPacket);
        case 0x32: return RecvPartyRecruitApplyAcceptCheck(xPacket);
        case 0x40: return RecvPartyInfo(xPacket);
        case 0x41: return RecvPartyNameChange(xPacket);
        case 0x42: return RecvPartyMatchingMaze(xPacket);
        case 0x43: return RecvPartyMazeClear(xPacket);
        default: return true;
    }
}

bool CCommunitySocket::LeagueProcess(XPacket* xPacket) {
    switch (xPacket->GetSubCmd()) {
        case 0x01: return RecvCreateLeague(xPacket);
        case 0x02: return RecvLeagueDelete(xPacket);
        case 0x03: return RecvLeagueLogin(xPacket);
        case 0x06: return RecvLeagueInfo(xPacket);
        case 0x07: return RecvLeagueDelegate(xPacket);
        case 0x08: return RecvLeagueWithDraw(xPacket);
        case 0x09: return RecvLeagueMemberKick(xPacket);
        case 0x0A: return RecvLeagueInfoChange(xPacket);
        case 0x0C: return RecvLeagueInvite(xPacket);
        case 0x0D: return RecvLeagueInviteAccept(xPacket);
        case 0x10: return RecvLeagueInviteReject(xPacket);
        case 0x14: return RecvLeagueBoard(xPacket);
        case 0x16: return RecvLeagueApplicantAcceptRes(xPacket);
        case 0x18: return RecvLeagueSearch(xPacket);
        case 0x19: return RecvLeagueApplicantRes(xPacket);
        case 0x20: return RecvLeagueApplicantAdd(xPacket);
        case 0x21: return RecvLeagueApplicantReject(xPacket);
        case 0x22: return RecvLeagueApplicantDelete(xPacket);
        case 0x23: return RecvLeagueNoticeChange(xPacket);
        case 0x24: return RecvLeagueList(xPacket);
        case 0x25: return RecvLeagueNameChange(xPacket);
        case 0x26: return RecvLeagueCardChange(xPacket);
        case 0x27: return RecvLeaguePositionNameChange(xPacket);
        case 0x28: return RecvLeagueAuthChange(xPacket);
        case 0x36: return RecvLeagueMessage(xPacket);
        case 0x37: return RecvLeagueMemberPositionChange(xPacket);
        case 0x38: return RecvLeagueApplicantUpdate(xPacket);
        case 0x39: return RecvLeagueMemberLogOut(xPacket);
        case 0x40: return RecvLeagueApplicantJoinUser(xPacket);
        case 0x41: return RecvLeagueInviteJoinUser(xPacket);
        case 0x42: return RecvLeagueMemberUpdate(xPacket);
        case 0x45: return RecvLeagueOpenOrNot(xPacket);
        case 0x46: return RecvLeagueRecruitNotice(xPacket);
        case 0x47: return RecvLeagueRecordUpdate(xPacket);
        case 0x48: return RecvLeagueCardChangeRes(xPacket);
        case 0x51: return RecvLeagueLevelup(xPacket);
        case 0x53: return RecvLeagueSkillLearn(xPacket);
        case 0x55: return RecvLeagueWealth(xPacket);
        case 0x58: return RecvLeagueSyncInfo(xPacket);
        case 0x59: return RecvLeagueSyncLoad(xPacket);
        case 0x60: return RecvLeagueInventoryInfo(xPacket);
        case 0x61: return RecvLeagueInventoryMove(xPacket);
        default: return true;
    }
}

// Stub implementations for CCommunitySocket packet handlers
bool CCommunitySocket::RecvPartyCreate(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyJoinMember(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyLeaveMember(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyChangeMaster(XPacket*) { return true; }
bool CCommunitySocket::RecvUpdatePartyMember(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyDelete(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyEnterMaze(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyUpdateInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyEnterServer(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyInvite(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyAccept(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyReject(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMessage(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingEnter(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingExit(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingCheck(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingReset(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingWait(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitAdd(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitDel(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApply(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyAccept(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyReject(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyUpdate(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitList(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitMyApplyList(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyList(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyDel(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyNotice(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyRecruitApplyAcceptCheck(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyNameChange(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMatchingMaze(XPacket*) { return true; }
bool CCommunitySocket::RecvPartyMazeClear(XPacket*) { return true; }

// ============================================================================
// League implementations
// ============================================================================

// IDA: 0x1401FC4C0 - RecvCreateLeague
// TODO: 需要从IDA反编译确认正确的结构类型
bool CCommunitySocket::RecvCreateLeague(XPacket* xPacket) {
    // TODO: 正确的结构类型需要从IDA反编译确认
    // ST_LEAGUE_INFO 不存在，暂时使用占位符
    std::uint32_t dwActorID = 0;

    // 跳过未知的数据包内容
    // *xPacket >> stLeagueInfo;
    // *xPacket >> stMaster;
    // xPacket->Parse() >> dwActorID;
    // *xPacket >> stInfoEx;
    // *xPacket >> stLeagueInfoForGame;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (!pUser) {
        LogHelper::LogError("game.contents",
            "[LEAUGE] CCommunitySocket::RecvCreateLeague - No pUser : %d [%02x][%02x]",
            dwActorID, xPacket->GetMainCmd(), xPacket->GetSubCmd());
        return true;
    }

    // TODO: 需要实现正确的逻辑
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvCreateLeague - TODO: need correct struct types from IDA");
    return true;
}

// IDA: 0x1401FB930 - RecvLeagueInfo
// TODO: 需要从IDA反编译确认正确的结构类型
bool CCommunitySocket::RecvLeagueInfo(XPacket* xPacket) {
    // TODO: 正确的结构类型需要从IDA反编译确认
    std::uint32_t dwActorID = 0;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (!pUser) {
        LogHelper::LogError("game.contents",
            "[LEAUGE] CCommunitySocket::RecvLeagueInfo - No pUser : %d [%02x][%02x]",
            dwActorID, xPacket->GetMainCmd(), xPacket->GetSubCmd());
        return true;
    }

    // TODO: 需要实现正确的逻辑
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvLeagueInfo - TODO: need correct struct types from IDA");
    return true;
}

// IDA: 0x1401F51A0 - RecvLeagueLogin
// TODO: 需要从IDA反编译确认正确的结构类型
bool CCommunitySocket::RecvLeagueLogin(XPacket* xPacket) {
    // TODO: 正确的结构类型需要从IDA反编译确认
    std::uint32_t dwActorID = 0;
    // TODO: 需要正确的数据包解析
    // *xPacket >> stApplicantList;
    // *xPacket >> stBoardList;
    // xPacket->Parse().read((char*)byState, sizeof(byState));
    // *xPacket >> stInfoEx;
    // *xPacket >> stRecordList;
    // *xPacket >> stLeagueInfoForGame;
    // xPacket->Parse() >> nSyncCount;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (!pUser) {
        return false;
    }

    // TODO: 需要实现正确的逻辑
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvLeagueLogin - TODO: need correct struct types from IDA");
    return true;
}

// IDA: 0x1401F4320 - RecvLeagueMemberUpdate
bool CCommunitySocket::RecvLeagueMemberUpdate(XPacket* xPacket) {
    // TODO: ST_LEAGUE_MEMBER_UPDATE stUpdate;
    // *xPacket >> stUpdate;

    // Per IDA: 分发到所有逻辑线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvLeagueMemberUpdate - need CLogicThreadManager::DoJobAllThread");
    return true;
}

// IDA: 0x1401F44A0 - RecvLeagueNoticeChange
bool CCommunitySocket::RecvLeagueNoticeChange(XPacket* xPacket) {
    // TODO: ST_LEAGUE_NOTICE stNotice;
    std::uint32_t dwActorID = 0;

    // *xPacket >> stNotice;
    // xPacket->Parse() >> dwActorID;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (pUser) {
        // Per IDA: 增加任务计数并分发到逻辑线程
        pUser->IncrementJobCount();
        // TODO: 完整实现需要CLogicThreadManager::DoJob
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvLeagueNoticeChange - need CLogicThreadManager::DoJob");
    }

    return true;
}

// IDA: 0x1401FAFB0 - RecvLeagueDelete
bool CCommunitySocket::RecvLeagueDelete(XPacket* xPacket) {
    std::uint32_t dwActorID = 0;
    int nLeagueID = 0;
    std::int64_t biPenalty = 0;
    int nErrorCode = 0;

    // IDA: XParse::operator>>(&xPacket->XParse, &dwActorID);
    xPacket->XParse >> dwActorID;
    xPacket->XParse >> nLeagueID;
    xPacket->XParse >> biPenalty;
    xPacket->XParse >> nErrorCode;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (pUser) {
        // Per IDA: 增加任务计数并分发到逻辑线程
        pUser->IncrementJobCount();
        // TODO: 完整实现需要CLogicThreadManager::DoJob
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvLeagueDelete - need CLogicThreadManager::DoJob");
    }
    return true;
}

bool CCommunitySocket::RecvLeagueDelegate(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueWithDraw(XPacket*) { return true; }

// IDA: 0x1401F8B70 - RecvLeagueMemberKick
bool CCommunitySocket::RecvLeagueMemberKick(XPacket* xPacket) {
    int nErrorCode = 0;
    int nLeagueID = 0;
    std::uint32_t dwReqActorID = 0;
    std::uint32_t dwTargetActorID = 0;
    ST_LEAGUE_INFO_UPDATE stInfoUpdate;
    std::int16_t shLevel = 0;
    int nSyncCount = 0;

    // IDA: XParse::operator>>
    xPacket->XParse >> nErrorCode;
    xPacket->XParse >> nLeagueID;
    xPacket->XParse >> dwReqActorID;
    xPacket->XParse >> dwTargetActorID;
    *xPacket >> stInfoUpdate;
    xPacket->XParse >> shLevel;
    xPacket->XParse >> nSyncCount;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pReqUser = pServer ? pServer->FindActorIDToUser(dwReqActorID) : nullptr;
    CUser* pKickUser = pServer ? pServer->FindActorIDToUser(dwTargetActorID) : nullptr;

    if (pReqUser) {
        // Per IDA: 检查GetArea并增加任务计数
        pReqUser->IncrementJobCount();
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvLeagueMemberKick - need CLogicThreadManager::DoJob");
    }

    if (pKickUser) {
        pKickUser->IncrementJobCount();
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvLeagueMemberKick (kick user) - need CLogicThreadManager::DoJob");
    }

    return true;
}

// IDA: 0x1401F49E0 - RecvLeagueInfoChange
bool CCommunitySocket::RecvLeagueInfoChange(XPacket* xPacket) {
    // TODO: ST_LEAGUE_INFO stInfo;
    // *xPacket >> stInfo;

    // Per IDA: 分发到所有逻辑线程
    // TODO: 完整实现需要CLogicThreadManager::DoJobAllThread
    GreenDamTan_log(__FILE__, __FUNCTION__, "RecvLeagueInfoChange - need CLogicThreadManager::DoJobAllThread");
    return true;
}

bool CCommunitySocket::RecvLeagueInvite(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInviteAccept(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInviteReject(XPacket*) { return true; }

// IDA: 0x1401F6050 - RecvLeagueBoard
bool CCommunitySocket::RecvLeagueBoard(XPacket* xPacket) {
    ST_LEAGUE_BOARD stBoard;
    std::uint32_t dwActorID = 0;
    int nLeagueID = 0;
    std::int64_t biRemainDate = 0;

    *xPacket >> stBoard;
    xPacket->XParse >> dwActorID;
    xPacket->XParse >> nLeagueID;
    xPacket->XParse >> biRemainDate;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (pUser) {
        pUser->IncrementJobCount();
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvLeagueBoard - need CLogicThreadManager::DoJob");
    }

    return true;
}

bool CCommunitySocket::RecvLeagueApplicantAcceptRes(XPacket*) { return true; }

// IDA: 0x1401F66C0 - RecvLeagueSearch
bool CCommunitySocket::RecvLeagueSearch(XPacket* xPacket) {
    PS_LEAGUE_SUMMARY_LIST psLeagueSummaryList;
    ST_LEAGUE_APPLICANT_CHECK_LIST stApplyList;
    std::uint32_t dwActorID = 0;

    xPacket->XParse >> dwActorID;
    *xPacket >> psLeagueSummaryList;
    *xPacket >> stApplyList;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (pUser) {
        pUser->IncrementJobCount();
        // TODO: 完整实现需要CLogicThreadManager::DoJob
        GreenDamTan_log(__FILE__, __FUNCTION__, "RecvLeagueSearch - need CLogicThreadManager::DoJob");
    }

    return true;
}

bool CCommunitySocket::RecvLeagueApplicantRes(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantAdd(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantReject(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantDelete(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueList(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueNameChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueCardChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeaguePositionNameChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueAuthChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueMessage(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueMemberPositionChange(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantUpdate(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueMemberLogOut(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueApplicantJoinUser(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInviteJoinUser(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueOpenOrNot(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueRecruitNotice(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueRecordUpdate(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueCardChangeRes(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueLevelup(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueSkillLearn(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueWealth(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueSyncInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueSyncLoad(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInventoryInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueInventoryMove(XPacket*) { return true; }

