// GameSockets.cpp
// Game Server Socket Implementations - Decompiled from IDA

#include "GameSockets.h"
#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/Common/XNet/XCommon/XSWCommand.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_MyRoomStructs.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerChat.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerWorldMode.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerMapMaze.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerLeague.h"
#include "Soulworker/Common/XNet/XCommon/PSServer/PSServerParty.h"
#include "Soulworker/Common/XNet/XCommon/PSOption.h"
#include "Soulworker/GameServer/XSCommon/Table/DBLoadTable.h"
#include "Soulworker/GameServer/XLoginServer/DayEventManager.h"
#include "Soulworker/GameServer/XLoginServer/RouletteEventManager.h"
#include "Soulworker/GameServer/XRelayServer/LeagueManager.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"
#include "User.h"
#include "GameServer.h"
#include "ThreadLocalData.h"
#include "Soulworker/GameServer/XGameServer/XPartyManager.h"
#include "Soulworker/GameServer/XGameServer/XForceManager.h"
#include "Soulworker/GameServer/XGameServer/CParty.h"
#include "Soulworker/GameServer/XGameServer/CForce.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XArea/XActor.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocParty.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocInventory.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocClassEvent.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocQuest.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocForce.h"
#include "Soulworker/GameServer/XGameServer/process/ForceProcess.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocNetwork.h"
#include "Soulworker/GameServer/XGameServer/actor/component/GocPost.h"
#include "Soulworker/GameServer/XGameServer/BattleZone.h"
#include <cstring>
#include <functional>

// ============================================================================
// CGameControlSocket Implementation
// ============================================================================

// 辅助函数：向所有线程广播 (1,9) 系统事件同步包
// CTimeEventMgr::ChangeEventValue / DeleteEventValue 的 SYSTEM_EVENT lambda 调用
void GreenDamTan_BroadcastSystemEvent(const PS_SYNC_SYSTEM_EVENT& stEvent) {
    XSendPacket xSendPacket(1u, 9u);
    xSendPacket << stEvent;
    ThreadLocalData::GetInstance()->SendBroadcast(xSendPacket);
}

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
    // Per IDA: switch on subcmd
    switch (xPacket->GetSubCmd()) {
        case 1: return RecvWorldModeStart(xPacket);
        case 2: return RecvWorldModeUpdate(xPacket);
        case 3: return RecvWorldModeClear(xPacket);
        case 4: return RecvWorldModeFinish(xPacket);
        case 5: return RecvWorldModeSync(xPacket);
        case 6: return RecvWorldModeCommand(xPacket);
        case 7: return RecvWorldModeComplete(xPacket);
        default: return true;
    }
}

// Per IDA 0x1401cb320: ForceProcess
bool CGameControlSocket::ForceProcess(XPacket* xPacket) {
    // Per IDA: subcmd 8 -> RecvForceEnterMaze
    if (xPacket->GetSubCmd() == 8) {
        return RecvForceEnterMaze(xPacket);
    }
    return true;
}

// ============================================================================
// RecvFindUser - IDA @ 0x1401CA880
// 已精确还原 - 解析 dwUCID + dwTargetUCID + dwServerID + byState；
// FindActorIDToUser 双查 pUser/pTarget。
// state==1 (看目标): 从目标 stPosInfo 取 nMapID (nMapID>>16>>16) 与位置；
//   pUser 无 Area 返回 0；IncrementJobCount ->
//   lambda0 (0x1401CAE90) DoJob: pUser IsLive 门 ->
//   SetInvisible(1,4,4,0,0,0,0) + send_eSUB_CMD_MOVE_INFO(pUser,2,1) +
//   EnterWorldToOther(nMapID, 0, stPosInfo, dwUCID)
//   -> lambda192 递减。
// state==2 (被看): 从请求者位置取 nMapID/stPosInfo；
//   pTarget 无 Area 返回 0；对 pTarget 计数 ->
//   lambda2 (0x1401CB080) DoJob: pTarget IsLive 门 ->
//   SetInvisible(1,4,4,0,0,0,0) + send_eSUB_CMD_MOVE_INFO(pTarget,2,1) +
//   EnterWorldToOther(nMapID, 0, stPosInfo, dwUCID) -> lambda192 递减。
// 其余 state 直接返回 1。
// ============================================================================
bool CGameControlSocket::RecvFindUser(XPacket* xPacket) {
    std::uint32_t dwUCID = 0;
    std::uint32_t dwTargetUCID = 0;
    std::uint32_t dwServerID = 0;
    std::uint8_t byState = 0;

    xPacket->XParse >> dwUCID;
    xPacket->XParse >> dwTargetUCID;
    xPacket->XParse >> dwServerID;
    xPacket->XParse >> byState;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwUCID);
    CUser* pTarget = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwTargetUCID);

    if (byState == 1) {
        // 状态1: 发送目标用户位置信息给请求用户
        if (pTarget) {
            int nMapID = static_cast<int>(
                pTarget->stMyCharInfoEx()->stPosInfo.uxMapID.nMapID >> 16 >> 16);
            STPosInfo stPosInfo = pTarget->stMyCharInfoEx()->stPosInfo;

            if (!pUser)
                return false;
            if (!pUser->GetArea())
                return false;

            pUser->IncrementJobCount();

            // Per IDA lambda0 (0x1401CAE90)
            CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
                [pUser, nMapID, stPosInfo, dwUCID]() {
                    if (!pUser || !pUser->IsLive())
                        return;
                    pUser->SetInvisible(1, 4, 4, 0, 0, 0, 0);
                    pUser->send_eSUB_CMD_MOVE_INFO(pUser, 2, 1);
                    STPosInfo stCopy = stPosInfo;
                    pUser->EnterWorldToOther(nMapID, 0, stCopy, dwUCID);
                });

            CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
                [pUser]() {
                    if (pUser)
                        pUser->DecrementJobCount();
                });
        }
    } else if (byState == 2) {
        // 状态2: 发送请求用户位置信息给目标用户
        if (pUser) {
            int nMapID = static_cast<int>(
                pUser->stMyCharInfoEx()->stPosInfo.uxMapID.nMapID >> 16 >> 16);
            STPosInfo stPosInfo = pUser->stMyCharInfoEx()->stPosInfo;

            if (!pTarget)
                return false;
            if (!pTarget->GetArea())
                return false;

            pTarget->IncrementJobCount();

            // Per IDA lambda2 (0x1401CB080)
            CLogicThreadManager::Instance().DoJob(pTarget->GetMapInsID().nMapID,
                [pUser, pTarget, nMapID, stPosInfo, dwUCID]() {
                    if (!pTarget || !pTarget->IsLive())
                        return;
                    pTarget->SetInvisible(1, 4, 4, 0, 0, 0, 0);
                    pTarget->send_eSUB_CMD_MOVE_INFO(pTarget, 2, 1);
                    STPosInfo stCopy = stPosInfo;
                    pUser->EnterWorldToOther(nMapID, 0, stCopy, dwUCID);
                });

            CLogicThreadManager::Instance().DoJob(pTarget->GetMapInsID().nMapID,
                [pTarget]() {
                    if (pTarget)
                        pTarget->DecrementJobCount();
                });
        }
    }

    return true;
}

// Per IDA 0x1401cb370: RecvCreateMazeReq
bool CGameControlSocket::RecvCreateMazeReq(XPacket* xPacket) {
    // Per IDA: 创建迷宫请求处理
    ST_CREATE_MAZE stCreatMaze = {};
    *xPacket >> stCreatMaze;

    // Per IDA: 创建lambda任务分发到逻辑线程
    std::function<void()> func = [stCreatMaze]() {
        // Per IDA: 在逻辑线程中处理创建迷宫请求
        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            // TODO: 调用实际的创建迷宫逻辑
            GreenDamTan_log(__FILE__, __FUNCTION__, 
                "RecvCreateMazeReq - processing maze creation, mapID=%lld", 
                stCreatMaze.uxMapID.nMapID);
        }
    };

    // Per IDA: 通过CLogicThreadManager分发任务
    CLogicThreadManager::Instance().DoJob(stCreatMaze.uxMapID.nMapID, func);

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
    std::function<void()> func = [dwUserID, uxMapID, stPartyInfo]() {
        // Per IDA: 在逻辑线程中处理用户进入服务器
        XGameServer* pServer = XGameServer::Instance();
        if (pServer) {
            CUser* pUser = pServer->FindActorIDToUser(dwUserID);
            if (pUser) {
                // TODO: 实现用户进入服务器的完整逻辑
                GreenDamTan_log(__FILE__, __FUNCTION__, 
                    "RecvUserEnterServer - userID=%u, mapID=%lld", dwUserID, uxMapID.nMapID);
            }
        }
    };

    // Per IDA: 通过CLogicThreadManager分发任务
    CLogicThreadManager::Instance().DoJob(uxMapID.nMapID, func);

    return true;
}

// Per IDA 0x1401cb9a0: RecvCreateMap
bool CGameControlSocket::RecvCreateMap(XPacket* xPacket) {
    // Per IDA: 创建地图
    PS_CREATE_MAP stCreateMap = {};
    *xPacket >> stCreateMap;

    // Per IDA: lambda7 检查地图是否已注册后分发到逻辑线程
    CLogicThreadManager::Instance().DoJob(stCreateMap.uxMapID.nMapID,
        [stCreateMap]() {
            if (ThreadLocalData::GetInstance()->FindArea(stCreateMap.uxMapID)) {
                if (stCreateMap.nResult > 0) {
                    LogHelper::LogError("game.contents",
                        "RecvCreateMap error - Failed Register map[ MapID:%d, Channel:%d ] ( %d )",
                        static_cast<int>(stCreateMap.uxMapID.nMapID >> 16) >> 16,
                        static_cast<int>(stCreateMap.uxMapID.nMapID) >> 24,
                        342);
                }
            }
        });

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

    // Per IDA: 检查用户Area是否存在
    if (!pReqUser->GetArea()) {
        return false;
    }

    // Per IDA: 增加任务计数
    pReqUser->IncrementJobCount();

    // Per IDA: 创建lambda任务分发到逻辑线程
    std::function<void()> func = [pReqUser, stEnterRes, stPosInfo, dwTargetID]() {
        // TODO: 实现进入其他地图的完整逻辑
        GreenDamTan_log(__FILE__, __FUNCTION__, 
            "RecvEnterMapToOther - userID=%u, targetID=%u", 
            stEnterRes.dwUserID, dwTargetID);
    };

    // Per IDA: 通过CLogicThreadManager分发任务
    CLogicThreadManager::Instance().DoJob(stEnterRes.dwUserID, func);

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
    if (!pReqUser->GetArea()) {
        return false;
    }

    // Per IDA: 增加任务计数
    pReqUser->IncrementJobCount();

    // Per IDA: 创建lambda任务分发到逻辑线程
    std::function<void()> func = [pReqUser, stEnterRes]() {
        // TODO: 实现进入地图的完整逻辑
        GreenDamTan_log(__FILE__, __FUNCTION__, 
            "RecvEnterMap - userID=%u", stEnterRes.dwUserID);
    };

    // Per IDA: 通过CLogicThreadManager分发任务
    CLogicThreadManager::Instance().DoJob(stEnterRes.dwUserID, func);

    return true;
}

// Per IDA 0x1401ccda0: RecvCheckPartyInMaze
bool CGameControlSocket::RecvCheckPartyInMaze(XPacket* xPacket) {
    // Per IDA: 解析迷宫 ID、进入请求与结果码
    UXMapID uxMazeID = {};
    PS_ENTER_MAP_REQ stEnterReq = {};
    int nResult = 0;

    xPacket->XParse >> uxMazeID.nMapID;
    *xPacket >> stEnterReq;
    xPacket->XParse >> nResult;

    // Per IDA: 按请求内 ActorID 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stEnterReq.dwActorID) : nullptr;

    if (!pUser) {
        LogHelper::LogError("game.contents",
            "RecvCheckPartyInMaze error - Cant Find User[ ActorID:%d ] ( %d )",
            stEnterReq.dwActorID, 580);
        return false;
    }

    // Per IDA: 用户不在存活态则直接拒绝（vtable+40 IsLive）
    if (!pUser->IsLive()) {
        return false;
    }

    // Per IDA: IncrementJobCount 后经 lambda12 分发到玩家所在线程
    pUser->IncrementJobCount();

    // Per IDA lambda12 (0x1401CD0F0): 在逻辑线程执行队伍/Force 迷宫进入判定
    std::function<void()> func = [pUser, uxMazeID, stEnterReq, nResult]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }
        // Per IDA: 结果码 <= 0 时按组类型走队伍/Force 迷宫进入
        if (nResult <= 0) {
            if (stEnterReq.stPartyInfo.byGroupType == 1 && stEnterReq.stPartyInfo.nID > 0) {
                // Per IDA: ThreadLocalData::GetInstance()->m_xPartyMgr.GetParty(actorID)
                XPartyManager* pPartyMgr = ThreadLocalData::GetInstance()->GetPartyMgr();
                if (pPartyMgr) {
                    std::shared_ptr<CParty> pParty = pPartyMgr->GetParty(pUser->GetActorID());
                    if (pParty) {
                        PS_ENTER_MAP_REQ stReq = stEnterReq;
                        pParty->EnterMaze(pUser, uxMazeID, &stReq);
                    }
                }
            } else if (stEnterReq.stPartyInfo.byGroupType == 2 && stEnterReq.stPartyInfo.nID > 0) {
                // Per IDA: m_xForceMgr.GetForce(actorID)
                XForceManager* pForceMgr = ThreadLocalData::GetInstance()->GetForceMgr();
                if (pForceMgr) {
                    std::shared_ptr<CForce> pForce = pForceMgr->GetForce(pUser->GetActorID());
                    if (pForce) {
                        PS_ENTER_MAP_REQ stReq = stEnterReq;
                        pForce->EnterMaze(pUser, uxMazeID, &stReq);
                    }
                }
            }
        } else {
            // Per IDA: 结果码为正即失败，向玩家回 (0x11,0x42) ST_CREATE_MAZE 错误包
            LogHelper::LogError("game.contents",
                "RecvCheckPartyInMaze error - Failed Enter Party Maze User[ ActorID:%d, Error:%d ] ( %d )",
                pUser->GetActorID().dwActorID, nResult, 591);
            ST_CREATE_MAZE stMaze = {};
            stMaze.nResult = nResult;
            stMaze.dwUserID = pUser->GetActorID().dwActorID;
            XSendPacket xSendPacket(0x11, 0x42);
            xSendPacket << stMaze;
            CGocNetwork::Send(pUser, xSendPacket);
        }
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
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
// 创建迷宫响应：解析 PS_ENTER_MAP_RES -> 按用户 ActorID 找人 -> 构造 lambda19
// 分发到玩家所在逻辑线程 -> lambda192 递减计数。找不到用户时静默返回 1。
bool CGameControlSocket::RecvCreateMazeRes(XPacket* xPacket) {
    PS_ENTER_MAP_RES stRecvEnterMapRes;
    *xPacket >> stRecvEnterMapRes;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stRecvEnterMapRes.dwUserID) : nullptr;

    if (pUser) {
        if (!pUser->GetArea()) {
            return true;
        }

        pUser->IncrementJobCount();

        // Per IDA lambda19 (0x1401CEA90): 在逻辑线程内完成迷宫进入判定与 DB/统计/日志
        std::function<void()> func = [pUser, stRecvEnterMapRes]() {
            if (!pUser || !pUser->IsLive()) {
                return;
            }
            PS_ENTER_MAP_RES stEnterMapRes = stRecvEnterMapRes;

            // Per IDA: 队伍迷宫 - byGroupType==1 且 nID 非零
            if (stEnterMapRes.stPartyInfo.byGroupType == 1 && stEnterMapRes.stPartyInfo.nID != 0) {
                XPartyManager* pPartyMgr = ThreadLocalData::GetInstance()->GetPartyMgr();
                if (pPartyMgr) {
                    std::shared_ptr<CParty> pParty = pPartyMgr->GetParty(pUser->GetActorID());
                    if (pParty) {
                        pParty->SendEnterMaze(pUser, &stEnterMapRes);
                    }
                }
            }
            // Per IDA: Force 迷宫 - byGroupType==2 且 nID 非零
            else if (stEnterMapRes.stPartyInfo.byGroupType == 2 && stEnterMapRes.stPartyInfo.nID != 0) {
                XForceManager* pForceMgr = ThreadLocalData::GetInstance()->GetForceMgr();
                if (pForceMgr) {
                    std::shared_ptr<CForce> pForce = pForceMgr->GetForce(pUser->GetActorID());
                    if (pForce) {
                        pForce->SendEnterMaze(&stEnterMapRes);
                    }
                }
            }
            // Per IDA: 结果码非零 - 失败回包 + 清状态 + 清除 warp 物品
            else if (stEnterMapRes.nResult) {
                pUser->ClearState(eStateChangeWorld);
                ST_CREATE_MAZE stMaze = {};
                stMaze.nResult = stEnterMapRes.nResult;
                stMaze.dwUserID = pUser->GetActorID().dwActorID;
                XSendPacket xSendPacket(0x11, 0x42);
                xSendPacket << stMaze;
                CGocNetwork::Send(pUser, xSendPacket);
                CGocInventory* pInventory = pUser->GetGOC<CGocInventory>();
                if (pInventory) {
                    pInventory->ClearUsedWarpItem(2);
                }
            }
            // Per IDA: 单人进迷宫 - 同步任务、传送门定位、DB 三包
            else {
                {
                    CGocQuest* pQuest = pUser->GetGOC<CGocQuest>();
                    if (pQuest) {
                        pQuest->DBSyncQuestCondition();
                    }
                }

                stEnterMapRes.byChangeType = 0;
                if (stEnterMapRes.uxParentInstanceID.nMapID > 0) {
                    stEnterMapRes.byChangeType = 5;
                }

                const int nMapID = static_cast<int>(
                    (stEnterMapRes.uxMapID.nMapID << 16) >> 48);
                if (XGameServer::Instance()->GetWorldResMgr().GetPortalPos(
                        nMapID, stEnterMapRes.nJumpID, &stEnterMapRes.stPosInfo)) {
                    if (stEnterMapRes.dwServerID != XGameServer::Instance()->GetOption().GetServerID()) {
                        stEnterMapRes.bChangeServer = true;
                    }

                    {
                        CGocInventory* pInventory = pUser->GetGOC<CGocInventory>();
                        if (pInventory) {
                            pInventory->DeleteUsedWarpItem();
                        }
                    }

                    LogHelper::LogDebug("game.contents",
                        "<RecvCreateMazeRes> User ( %d ) ( %I64d, %d %d ) bChange ( %d ) ",
                        stEnterMapRes.dwUserID,
                        stEnterMapRes.uxMapID.nMapID,
                        static_cast<int>((static_cast<std::uint64_t>(stEnterMapRes.uxMapID.nMapID) >> 16) >> 16),
                        stEnterMapRes.nJumpID, stEnterMapRes.bChangeServer ? 1 : 0);

                    {
                        int nPrevMapID = 0;
                        int nPrevRevivePoint = 0;
                        XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 3, 0x42);
                        xSendDBPacket << stEnterMapRes;
                        xSendDBPacket.XParse << nPrevMapID;
                        xSendDBPacket.XParse << nPrevRevivePoint;
                        XGameServer::Instance()->SendDBGame(xSendDBPacket);
                    }

                    {
                        ST_STATISTICS_MAP_SAVE stInfo = {};
                        stInfo.dwUCID = stEnterMapRes.dwUserID;
                        stInfo.dwMapID = static_cast<int>(
                            (static_cast<std::uint64_t>(stEnterMapRes.uxMapID.nMapID) >> 16) >> 16);
                        stInfo.dwServerID = XGameServer::Instance()->GetOption().GetServerID();
                        XSendDBPacket xSendDBStatistics(static_cast<XActor*>(pUser), 0xF0, 0x12);
                        xSendDBStatistics << stInfo;
                        XGameServer::Instance()->SendDBStatistics(xSendDBStatistics);
                    }

                    {
                        ST_LOG_GAME stLog = {};
                        stLog._nUAID = static_cast<int>(pUser->GetUAID());
                        stLog._nUCID = static_cast<int>(pUser->GetActorID().dwActorID);
                        stLog._sMainType = 5;
                        stLog._sSubType = 4;
                        stLog.nParam0 = static_cast<int>(
                            (static_cast<std::uint64_t>(stEnterMapRes.uxMapID.nMapID) >> 16) >> 16);
                        stLog.nParam3 = 1;
                        stLog.nParam5 = pUser->GetLevel();
                        stLog.nParam6 = stEnterMapRes.uxMapID.nMapID;
                        std::wcscpy(stLog.szComment, L"메이즈 입장");
                        XGameServer::Instance()->SendDBLog(stLog);
                    }
                } else {
                    pUser->ClearState(eStateChangeWorld);
                    LogHelper::LogError("game.contents",
                        "RecvCreateMazeRes error - No have World Data[ ActorID:%d, MapID:%d, JumpID:%d ] ( %d )",
                        stEnterMapRes.dwUserID,
                        static_cast<int>((static_cast<std::uint64_t>(stEnterMapRes.uxMapID.nMapID) >> 16) >> 16),
                        stEnterMapRes.nJumpID, 901);
                }
            }
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

        // Per IDA lambda192 (0x140427E60): 任务完成递减计数
        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
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
// Force 进入迷宫响应：解析 dwForceID + PS_ENTER_MAP_RES -> 按用户 ActorID 找人 ->
// 构造 lambda25 分发到玩家所在逻辑线程 -> lambda192 递减计数。
bool CGameControlSocket::RecvForceEnterMaze(XPacket* xPacket) {
    unsigned int dwForceID = 0;
    PS_ENTER_MAP_RES stRecvEnterMap;

    xPacket->XParse >> dwForceID;
    *xPacket >> stRecvEnterMap;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stRecvEnterMap.dwUserID) : nullptr;

    if (pUser) {
        if (!pUser->GetArea()) {
            return false;
        }

        pUser->IncrementJobCount();

        // Per IDA lambda25 (0x1401CFCC0): 与 RecvPartyEnterMaze 共享同一任务体，
        // 组内 ID 均透传给 XPartyManager::ResEnterMaze 处理
        std::function<void()> func = [pUser, dwForceID, stRecvEnterMap]() {
            if (!pUser || !pUser->IsLive()) {
                return;
            }
            CGocParty* pGocParty = pUser->GetGOC<CGocParty>();
            if (pGocParty && pGocParty->IsParty()) {
                PS_ENTER_MAP_RES stEnterMap = stRecvEnterMap;
                ThreadLocalData::GetInstance()->GetPartyMgr()->ResEnterMaze(pUser, dwForceID, stEnterMap);
            }
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

        // Per IDA lambda192 (0x140427E60): 任务完成递减计数
        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
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

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwRecvUCID) : nullptr;

    if (pUser && pUser->GetArea()) {
        // Per IDA: 增加任务计数
        pUser->IncrementJobCount();

        // Per IDA: lambda41 添加收件邮件并发送给用户
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
            [pUser, wPostCount, stPostData, stPostChar, byState]() {
                if (pUser && pUser->IsLive()) {
                    CGocPost* pPost = pUser->GetGOC<CGocPost>();
                    if (pPost) {
                        pPost->SetConditionValue(wPostCount);
                        if (byState) {
                            pPost->AddRecvPost(stPostData);
                            XSendPacket xSendPacket(0x20u, 9u);
                            xSendPacket.XParse << wPostCount;
                            xSendPacket << stPostData;
                            CGocNetwork::Send(pUser, xSendPacket);
                        } else {
                            ST_POST_DATA stRecvData = stPostData;
                            stRecvData.stCharInfo = stPostChar;
                            pPost->AddRecvPost(stRecvData);
                            XSendPacket xSendPacket(0x20u, 9u);
                            xSendPacket.XParse << wPostCount;
                            xSendPacket << stRecvData;
                            CGocNetwork::Send(pUser, xSendPacket);
                        }
                    }
                }
            });

        // Per IDA: 递减任务计数
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
            [pUser]() { if (pUser) pUser->DecrementJobCount(); });
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
    // Per IDA: 解析公告
    PS_CHAT_NOTICE stNotice = {};
    *xPacket >> stNotice;

    // Per IDA: lambda35 发送广播后分发到所有线程
    CLogicThreadManager::Instance().DoJobAllThread([stNotice]() {
        XSendPacket xSendPacket(7u, 4u);
        xSendPacket << stNotice;
        ThreadLocalData::GetInstance()->SendBroadcast(xSendPacket);
    });
    return true;
}

// Per IDA 0x1401d4a10: RecvServerDayEventBoosterList - moved to proper location

// Per IDA 0x1401d1680: RecvUserMegaPhone
bool CGameControlSocket::RecvUserMegaPhone(XPacket* xPacket) {
    // Per IDA: 解析喇叭消息与物品链接
    PS_CHAT_MEGAPHONE stMegaPhone = {};
    PS_CHAT_ITEM_LINK_FOR_SERVER psLinkItemInfo = {};

    *xPacket >> stMegaPhone;
    *xPacket >> psLinkItemInfo;

    // Per IDA: lambda36 组装 (7,6) 广播包后分发到所有线程
    CLogicThreadManager::Instance().DoJobAllThread([stMegaPhone, psLinkItemInfo]() {
        XSendPacket xSendPacket(7u, 6u);
        xSendPacket << stMegaPhone;
        xSendPacket.XParse << psLinkItemInfo.byItemLinkCount;
        for (std::uint8_t i = 0; i < psLinkItemInfo.byItemLinkCount; ++i) {
            PS_CHAT_ITEM_LINK psInfo = psLinkItemInfo.psItemLinkInfo[i];
            xSendPacket << psInfo;
        }
        ThreadLocalData::GetInstance()->SendBroadcast(xSendPacket);
    });
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
    if (!pUser->GetArea()) {
        return false;
    }

    // Per IDA: 增加任务计数
    pUser->IncrementJobCount();

    // Per IDA: lambda37 设置交易密码状态后分发到逻辑线程
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, byState]() {
            if (pUser && pUser->IsLive()) {
                CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
                if (pInven) {
                    pInven->SetTradePasswordState(byState);
                }
            }
        });

    // Per IDA: 递减任务计数
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() { if (pUser) pUser->DecrementJobCount(); });
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
    if (!pUser->GetArea()) {
        return false;
    }

    // Per IDA: 增加任务计数
    pUser->IncrementJobCount();

    // Per IDA: lambda39 失败时发送错误消息后分发到逻辑线程
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, byResult]() {
            if (pUser && pUser->IsLive() && byResult) {
                CGocNetwork::SendErrorMessage(pUser, 7u, 2u, 0xD994u);
            }
        });

    // Per IDA: 递减任务计数
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() { if (pUser) pUser->DecrementJobCount(); });
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
// 组队进入迷宫响应：解析 dwPartyID + PS_ENTER_MAP_RES -> 按用户 ActorID 找人 ->
// 构造 lambda25 分发到玩家所在逻辑线程 -> lambda192 递减计数。
bool CGameControlSocket::RecvPartyEnterMaze(XPacket* xPacket) {
    unsigned int dwPartyID = 0;
    PS_ENTER_MAP_RES stRecvEnterMap;

    xPacket->XParse >> dwPartyID;
    *xPacket >> stRecvEnterMap;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stRecvEnterMap.dwUserID) : nullptr;

    if (pUser) {
        if (!pUser->GetArea()) {
            return false;
        }

        pUser->IncrementJobCount();

        // Per IDA lambda25 (0x1401CFCC0): 在逻辑线程内按队伍处理迷宫进入响应
        std::function<void()> func = [pUser, dwPartyID, stRecvEnterMap]() {
            if (!pUser || !pUser->IsLive()) {
                return;
            }
            CGocParty* pGocParty = pUser->GetGOC<CGocParty>();
            if (pGocParty && pGocParty->IsParty()) {
                PS_ENTER_MAP_RES stEnterMap = stRecvEnterMap;
                ThreadLocalData::GetInstance()->GetPartyMgr()->ResEnterMaze(pUser, dwPartyID, stEnterMap);
            }
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

        // Per IDA lambda192 (0x140427E60): 任务完成递减计数
        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    }

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
    if (!pUser->GetArea()) {
        return false;
    }

    // Per IDA: 增加任务计数
    pUser->IncrementJobCount();

    // Per IDA: lambda21 记录日志并踢出用户后分发到逻辑线程
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, psKick]() {
            if (pUser && pUser->IsLive()) {
                PS_KICK_USER_INFO stKick = psKick;
                LogHelper::LogInfo("game.contents", "<RecvUserKickout> UAID : [%d], TYPE : [%d]",
                    stKick.dwUAID, stKick.byKickType);
                pUser->Kickout(&stKick, false);
            }
        });

    // Per IDA: 递减任务计数
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() { if (pUser) pUser->DecrementJobCount(); });
    return true;
}
// Per IDA 0x1401cf690: RecvUpdateChannelAll
bool CGameControlSocket::RecvUpdateChannelAll(XPacket* xPacket) {
    // Per IDA: 解析频道信息
    PS_CHANNEL_INFO stChannel = {};
    *xPacket >> stChannel;

    // Per IDA: lambda23 更新频道信息后分发到所有线程
    CLogicThreadManager::Instance().DoJobAllThread([stChannel]() {
        PS_CHANNEL_INFO stCopy = stChannel;
        ThreadLocalData::GetInstance()->UpdateChannelAll(stCopy);
    });
    return true;
}

// Per IDA 0x1401cf890: RecvUpdateChannel
bool CGameControlSocket::RecvUpdateChannel(XPacket* xPacket) {
    // Per IDA: 更新单个频道
    std::uint16_t wMapID = 0;
    ST_CHANNEL_INFO stChannel = {};

    xPacket->XParse >> wMapID;
    *xPacket >> stChannel;

    // Per IDA: lambda24 更新频道信息后分发到所有线程
    CLogicThreadManager::Instance().DoJobAllThread([wMapID, stChannel]() {
        ST_CHANNEL_INFO stCopy = stChannel;
        ThreadLocalData::GetInstance()->UpdateChannel(wMapID, stCopy);
    });
    return true;
}

// Per IDA 0x1401cd920: RecvChangeChannelRes
// 更换频道响应：解析 PS_ENTER_MAP_RES -> 按用户 ActorID 找人 -> 构造 lambda19
// 分发到玩家所在逻辑线程 -> lambda192 递减计数。所有路径均返回 0（false）。
bool CGameControlSocket::RecvChangeChannelRes(XPacket* xPacket) {
    PS_ENTER_MAP_RES stEnterRes;
    *xPacket >> stEnterRes;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pReqUser = pServer ? pServer->FindActorIDToUser(stEnterRes.dwUserID) : nullptr;

    if (!pReqUser) {
        LogHelper::LogError("game.contents",
            "RecvChangeChannelRes error - Cant find enter request user[ ActorID:%d ] ( %d )",
            stEnterRes.dwUserID, 672);
        return false;
    }

    if (!pReqUser->GetArea()) {
        return false;
    }

    pReqUser->IncrementJobCount();

    // Per IDA lambda19 (0x1401CEA90): 与 RecvCreateMazeRes 共享同一逻辑线程任务体
    std::function<void()> func = [pReqUser, stEnterRes]() {
        if (!pReqUser || !pReqUser->IsLive()) {
            return;
        }
        PS_ENTER_MAP_RES stEnterMapRes = stEnterRes;

        if (stEnterMapRes.stPartyInfo.byGroupType == 1 && stEnterMapRes.stPartyInfo.nID != 0) {
            XPartyManager* pPartyMgr = ThreadLocalData::GetInstance()->GetPartyMgr();
            if (pPartyMgr) {
                std::shared_ptr<CParty> pParty = pPartyMgr->GetParty(pReqUser->GetActorID());
                if (pParty) {
                    pParty->SendEnterMaze(pReqUser, &stEnterMapRes);
                }
            }
        } else if (stEnterMapRes.stPartyInfo.byGroupType == 2 && stEnterMapRes.stPartyInfo.nID != 0) {
            XForceManager* pForceMgr = ThreadLocalData::GetInstance()->GetForceMgr();
            if (pForceMgr) {
                std::shared_ptr<CForce> pForce = pForceMgr->GetForce(pReqUser->GetActorID());
                if (pForce) {
                    pForce->SendEnterMaze(&stEnterMapRes);
                }
            }
        } else if (stEnterMapRes.nResult) {
            pReqUser->ClearState(eStateChangeWorld);
            ST_CREATE_MAZE stMaze = {};
            stMaze.nResult = stEnterMapRes.nResult;
            stMaze.dwUserID = pReqUser->GetActorID().dwActorID;
            XSendPacket xSendPacket(0x11, 0x42);
            xSendPacket << stMaze;
            CGocNetwork::Send(pReqUser, xSendPacket);
            CGocInventory* pInventory = pReqUser->GetGOC<CGocInventory>();
            if (pInventory) {
                pInventory->ClearUsedWarpItem(2);
            }
        } else {
            {
                CGocQuest* pQuest = pReqUser->GetGOC<CGocQuest>();
                if (pQuest) {
                    pQuest->DBSyncQuestCondition();
                }
            }

            stEnterMapRes.byChangeType = 0;
            if (stEnterMapRes.uxParentInstanceID.nMapID > 0) {
                stEnterMapRes.byChangeType = 5;
            }

            const int nMapID = static_cast<int>(
                (stEnterMapRes.uxMapID.nMapID << 16) >> 48);
            if (XGameServer::Instance()->GetWorldResMgr().GetPortalPos(
                    nMapID, stEnterMapRes.nJumpID, &stEnterMapRes.stPosInfo)) {
                if (stEnterMapRes.dwServerID != XGameServer::Instance()->GetOption().GetServerID()) {
                    stEnterMapRes.bChangeServer = true;
                }

                {
                    CGocInventory* pInventory = pReqUser->GetGOC<CGocInventory>();
                    if (pInventory) {
                        pInventory->DeleteUsedWarpItem();
                    }
                }

                LogHelper::LogDebug("game.contents",
                    "<RecvCreateMazeRes> User ( %d ) ( %I64d, %d %d ) bChange ( %d ) ",
                    stEnterMapRes.dwUserID,
                    stEnterMapRes.uxMapID.nMapID,
                    static_cast<int>((static_cast<std::uint64_t>(stEnterMapRes.uxMapID.nMapID) >> 16) >> 16),
                    stEnterMapRes.nJumpID, stEnterMapRes.bChangeServer ? 1 : 0);

                {
                    int nPrevMapID = 0;
                    int nPrevRevivePoint = 0;
                    XSendDBPacket xSendDBPacket(static_cast<XActor*>(pReqUser), 3, 0x42);
                    xSendDBPacket << stEnterMapRes;
                    xSendDBPacket.XParse << nPrevMapID;
                    xSendDBPacket.XParse << nPrevRevivePoint;
                    XGameServer::Instance()->SendDBGame(xSendDBPacket);
                }

                {
                    ST_STATISTICS_MAP_SAVE stInfo = {};
                    stInfo.dwUCID = stEnterMapRes.dwUserID;
                    stInfo.dwMapID = static_cast<int>(
                        (static_cast<std::uint64_t>(stEnterMapRes.uxMapID.nMapID) >> 16) >> 16);
                    stInfo.dwServerID = XGameServer::Instance()->GetOption().GetServerID();
                    XSendDBPacket xSendDBStatistics(static_cast<XActor*>(pReqUser), 0xF0, 0x12);
                    xSendDBStatistics << stInfo;
                    XGameServer::Instance()->SendDBStatistics(xSendDBStatistics);
                }

                {
                    ST_LOG_GAME stLog = {};
                    stLog._nUAID = static_cast<int>(pReqUser->GetUAID());
                    stLog._nUCID = static_cast<int>(pReqUser->GetActorID().dwActorID);
                    stLog._sMainType = 5;
                    stLog._sSubType = 4;
                    stLog.nParam0 = static_cast<int>(
                        (static_cast<std::uint64_t>(stEnterMapRes.uxMapID.nMapID) >> 16) >> 16);
                    stLog.nParam3 = 1;
                    stLog.nParam5 = pReqUser->GetLevel();
                    stLog.nParam6 = stEnterMapRes.uxMapID.nMapID;
                    std::wcscpy(stLog.szComment, L"메이즈 입장");
                    XGameServer::Instance()->SendDBLog(stLog);
                }
            } else {
                pReqUser->ClearState(eStateChangeWorld);
                LogHelper::LogError("game.contents",
                    "RecvCreateMazeRes error - No have World Data[ ActorID:%d, MapID:%d, JumpID:%d ] ( %d )",
                    stEnterMapRes.dwUserID,
                    static_cast<int>((static_cast<std::uint64_t>(stEnterMapRes.uxMapID.nMapID) >> 16) >> 16),
                    stEnterMapRes.nJumpID, 901);
            }
        }
    };
    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pReqUser]() {
        pReqUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, funcDec);

    return false;  // Per IDA: 所有成功路径均返回 0
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
    pServer->GetTimeEventMgr().AddTimeEvent(stInfo);

    // Per IDA: 如果未标记为停用，则通过 lambda43 分发到所有线程广播
    if (!stInfo.byteUse) {
        CLogicThreadManager::Instance().DoJobAllThread([stInfo]() {
            ST_GM_TIME_EVENT_INFO stEvent = stInfo;
            ThreadLocalData::GetInstance()->SendTimeEvent(stEvent);
        });
    }
    return true;
}

// Per IDA 0x1401d2da0: RecvValueEvent
bool CGameControlSocket::RecvValueEvent(XPacket* xPacket) {
    // Per IDA: 解析数值事件列表
    PS_GM_VALUE_EVENT_LIST psList = {};
    *xPacket >> psList;

    // Per IDA: 添加数值事件
    XGameServer* pServer = XGameServer::Instance();
    pServer->GetTimeEventMgr().AddValueEvent(psList);
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

// ============================================================================
// XRelaySocket::SendCreateMazeReq - IDA @ 0x14077C390
// 已精确还原 - (0xF2, 0x21) 创建迷宫请求：ST_CREATE_MAZE 序列化后经
// XIOCPClient::Send 发往 ControlServer。
// CParty::CreateMazeReq (0x1403AAD60) / CForce::CreateMazeReq (0x1401BBC90)
// 的统一出口。
// ============================================================================
void XRelaySocket::SendCreateMazeReq(ST_CREATE_MAZE& stCreateMaze) {
    XSendPacket xSendPacket(eCMD_SERVER, eSUB_CMD_SERVER_CREATE_MAZE_REQ);
    xSendPacket << stCreateMaze;
    XIOCPClient::Send(xSendPacket);
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
    // Per IDA: 解析轮盘活动信息与是否广播标志
    PS_GM_ROULETTE_EVENT psInfo = {};
    bool bSend = false;

    *xPacket >> psInfo;
    xPacket->XParse >> bSend;

    // Per IDA: 设置轮盘事件
    XGameServer* pServer = XGameServer::Instance();
    pServer->GetTimeEventMgr().SetRouletteEvent(psInfo);

    // Per IDA: lambda64 捕获轮盘信息，经 DoJobAllThread 广播到所有线程
    CLogicThreadManager::Instance().DoJobAllThread([psInfo, bSend]() {
        LogHelper::LogError("game.contents", "Complete Roulette Event Load[ID:%d]", psInfo.nEventID);
        if (bSend) {
            PS_ROULETTE_EVENT psEventInfo = {};
            std::wcscpy(psEventInfo.szTitle, psInfo.szTitle);
            psEventInfo.nEventID = psInfo.nEventID;
            psEventInfo.byUseType = psInfo.byUseType;
            psEventInfo.nDayLimit = psInfo.nUseCount;
            psEventInfo.biStartDate = psInfo.biStartDate;
            psEventInfo.biEndDate = psInfo.biEndDate;
            psEventInfo.byCostType = psInfo.byCostType;
            psEventInfo.nCostID = psInfo.nCostID;
            psEventInfo.nCostCount = psInfo.nCostCount;
            for (const ST_GM_ROULETTE_EVENT_ITEM& stReward : psInfo.psRewardList.vecInfo) {
                ST_ROULETTE_EVENT_ITEM stItem;
                stItem.nRewardID = stReward.nRewradIndex;
                stItem.nItemID = stReward.nItemID;
                stItem.nCount = stReward.nCount;
                psEventInfo.psRewardItemList.vecInfo.push_back(stItem);
            }
            XSendPacket xSendPacket(0x2A, 0x27);
            xSendPacket << psEventInfo;
            ThreadLocalData::GetInstance()->SendBroadcast(xSendPacket);
        }
    });
    return true;
}

// ============================================================================
// CCommunitySocket Implementation
// ============================================================================

// IDA: ?SendCheck@CCommunitySocket@@QEAAXAEAVXSendPacket@@@Z @ 0x1401F37E0
void CCommunitySocket::SendCheck(XSendPacket* pPacket) {
    // Per IDA: 检查是否可以发送
    if (IsCanSend(this)) {
        XIOCPClient::Send(*pPacket);
    } else {
        LogHelper::LogError("game.contents",
            "Community Send error[ Caching:%d, Sync:%d ](%d)",
            m_dwCachingLoad, m_bSyncUserInfoReq, 58);
    }
}

// IDA: ?SendCmd@CCommunitySocket@@QEAAXAEAVXSendPacket@@PEAVCUser@@EE@Z @ 0x1401F3850
void CCommunitySocket::SendCmd(XSendPacket* pPacket, CUser* pUser, std::uint8_t byMainCmd, std::uint8_t bySubCmd) {
    // Per IDA: 检查是否可以发送
    if (IsCanSend(this)) {
        if (!XIOCPClient::Send(*pPacket) && pUser) {
            pUser->SendErrorMessage(byMainCmd, bySubCmd, 0xC35A);
        }
    } else {
        LogHelper::LogError("game.contents",
            "Community SendEx error[ Caching:%d, Sync:%d ](%d)",
            m_dwCachingLoad, m_bSyncUserInfoReq, 69);
        if (pUser) {
            pUser->SendErrorMessage(byMainCmd, bySubCmd, 0xC35A);
        }
    }
}

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

// ============================================================================
// CCommunitySocket Party Packet Handlers - Decompiled from IDA
// ============================================================================

// Per IDA 0x1401FDCD0: RecvPartyCreate
bool CCommunitySocket::RecvPartyCreate(XPacket* xPacket) {
    // Per IDA: 解析创建队伍请求
    PS_REQ_PARTY_CREATE stCreatParty = {};
    *xPacket >> stCreatParty;

    // Per IDA: 分发到所有逻辑线程
    std::function<void()> funcAllThreads = [stCreatParty]() {
        // Per IDA: 在所有线程中处理队伍创建通知
        GreenDamTan_log(__FILE__, __FUNCTION__,
            "RecvPartyCreate - partyID=%u, masterID=%u, memberID=%u",
            stCreatParty.dwPartyID, stCreatParty.masterInfo.dwMemberID, stCreatParty.memberInfo.dwMemberID);
    };

    CLogicThreadManager::Instance().DoJob(0, funcAllThreads);

    // Per IDA: 查找队长用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pMaster = pServer ? pServer->FindActorIDToUser(stCreatParty.masterInfo.dwMemberID) : nullptr;

    if (pMaster) {
        // Per IDA: 检查用户Area是否存在
        if (!pMaster->GetArea()) {
            return false;
        }

        pMaster->IncrementJobCount();

        // Per IDA: 分发到队长的逻辑线程
        std::function<void()> funcMaster = [pMaster, stCreatParty]() {
            GreenDamTan_log(__FILE__, __FUNCTION__,
                "RecvPartyCreate (master) - partyID=%u", stCreatParty.dwPartyID);
        };

        CLogicThreadManager::Instance().DoJob(stCreatParty.dwPartyID, funcMaster);
    }

    // Per IDA: 查找成员用户
    CUser* pMember = pServer ? pServer->FindActorIDToUser(stCreatParty.memberInfo.dwMemberID) : nullptr;

    if (pMember) {
        // Per IDA: 检查用户Area是否存在
        if (!pMember->GetArea()) {
            return false;
        }

        pMember->IncrementJobCount();

        // Per IDA: 分发到成员的逻辑线程
        std::function<void()> funcMember = [pMember, stCreatParty]() {
            GreenDamTan_log(__FILE__, __FUNCTION__,
                "RecvPartyCreate (member) - partyID=%u", stCreatParty.dwPartyID);
        };

        CLogicThreadManager::Instance().DoJob(stCreatParty.dwPartyID, funcMember);
    }

    return true;
}

// Per IDA 0x1401FD480: RecvPartyJoinMember
bool CCommunitySocket::RecvPartyJoinMember(XPacket* xPacket) {
    // Per IDA: 解析加入成员数据
    PS_PARTY_ADDMEMBER stRecvAddMember = {};
    PS_PARTY_INFO stRecvPartyInfo = {};

    *xPacket >> stRecvAddMember;
    *xPacket >> stRecvPartyInfo;

    // Per IDA: 查找加入的成员用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pMember = pServer ? pServer->FindActorIDToUser(stRecvAddMember.stMember.dwMemberID) : nullptr;

    if (pMember) {
        // Per IDA: 检查用户Area是否存在
        if (!pMember->GetArea()) {
            return false;
        }

        pMember->IncrementJobCount();

        // Per IDA: 分发到成员的逻辑线程
        std::function<void()> func = [pMember, stRecvAddMember, stRecvPartyInfo]() {
            GreenDamTan_log(__FILE__, __FUNCTION__,
                "RecvPartyJoinMember - memberID=%u, partyID=%u",
                stRecvAddMember.stMember.dwMemberID, stRecvAddMember.dwPartyID);
        };

        CLogicThreadManager::Instance().DoJob(stRecvAddMember.dwPartyID, func);
    }

    // Per IDA: 分发到所有逻辑线程
    std::function<void()> funcAllThreads = [stRecvAddMember]() {
        GreenDamTan_log(__FILE__, __FUNCTION__,
            "RecvPartyJoinMember (broadcast) - partyID=%u", stRecvAddMember.dwPartyID);
    };

    CLogicThreadManager::Instance().DoJob(0, funcAllThreads);

    return true;
}

// Per IDA 0x140200820: RecvPartyLeaveMember
bool CCommunitySocket::RecvPartyLeaveMember(XPacket* xPacket) {
    // Per IDA: 解析离开成员数据
    PS_PARTY_LEAVE stPartyLeave = {};
    unsigned int dwMasterID = 0;
    int nError = 0;

    *xPacket >> stPartyLeave;
    xPacket->XParse >> dwMasterID;
    xPacket->XParse >> nError;

    XGameServer* pServer = XGameServer::Instance();

    if (nError) {
        // Per IDA: 错误处理 - 通知队长
        CUser* pUser = pServer ? pServer->FindActorIDToUser(dwMasterID) : nullptr;

        if (pUser) {
            if (!pUser->GetArea()) {
                return false;
            }

            pUser->IncrementJobCount();

            std::function<void()> func = [pUser, nError]() {
                GreenDamTan_log(__FILE__, __FUNCTION__,
                    "RecvPartyLeaveMember (error) - error=%d", nError);
            };

            CLogicThreadManager::Instance().DoJob(stPartyLeave.dwPartyID, func);
        }
        return true;
    }

    // Per IDA: 分发到所有逻辑线程
    std::function<void()> funcAllThreads = [stPartyLeave, dwMasterID]() {
        GreenDamTan_log(__FILE__, __FUNCTION__,
            "RecvPartyLeaveMember (broadcast) - partyID=%u, leaveMember=%u",
            stPartyLeave.dwPartyID, stPartyLeave.dwLeaveMember);
    };

    CLogicThreadManager::Instance().DoJob(0, funcAllThreads);

    // Per IDA: 查找离开的成员用户
    CUser* pMember = pServer ? pServer->FindActorIDToUser(stPartyLeave.dwLeaveMember) : nullptr;

    if (pMember) {
        if (!pMember->GetArea()) {
            return false;
        }

        pMember->IncrementJobCount();

        std::function<void()> func = [pMember, stPartyLeave]() {
            GreenDamTan_log(__FILE__, __FUNCTION__,
                "RecvPartyLeaveMember (member) - partyID=%u", stPartyLeave.dwPartyID);
        };

        CLogicThreadManager::Instance().DoJob(stPartyLeave.dwPartyID, func);
    } else {
        // Per IDA: 如果成员不存在，通知队长
        CUser* pMaster = pServer ? pServer->FindActorIDToUser(dwMasterID) : nullptr;

        if (pMaster) {
            if (!pMaster->GetArea()) {
                return false;
            }

            pMaster->IncrementJobCount();

            std::function<void()> func = [pMaster, stPartyLeave]() {
                GreenDamTan_log(__FILE__, __FUNCTION__,
                    "RecvPartyLeaveMember (master) - partyID=%u", stPartyLeave.dwPartyID);
            };

            CLogicThreadManager::Instance().DoJob(stPartyLeave.dwPartyID, func);
        }
    }

    return true;
}

// Per IDA 0x140200440: RecvPartyChangeMaster
bool CCommunitySocket::RecvPartyChangeMaster(XPacket* xPacket) {
    // Per IDA: 解析更换队长数据
    PS_PARTY_CHANGE_MASTER stChangeMaster = {};
    *xPacket >> stChangeMaster;

    if (stChangeMaster.nErrorCode) {
        // Per IDA: 错误码 55061 特殊处理
        if (stChangeMaster.nErrorCode == 55061) {
            XGameServer* pServer = XGameServer::Instance();
            CUser* pUser = pServer ? pServer->FindActorIDToUser(stChangeMaster.dwReqActorID) : nullptr;

            if (pUser) {
                if (!pUser->GetArea()) {
                    return false;
                }

                pUser->IncrementJobCount();

                std::function<void()> func = [pUser, stChangeMaster]() {
                    GreenDamTan_log(__FILE__, __FUNCTION__,
                        "RecvPartyChangeMaster (error 55061) - partyID=%u", stChangeMaster.dwPartyID);
                };

                CLogicThreadManager::Instance().DoJob(stChangeMaster.dwPartyID, func);
            }
        }

        LogHelper::LogError("game.contents",
            "RecvPartyChangeMaster error[ Errorcode:%d ] ( %d)",
            stChangeMaster.nErrorCode, 1794);
        return false;
    }

    // Per IDA: 分发到所有逻辑线程
    std::function<void()> funcAllThreads = [stChangeMaster]() {
        GreenDamTan_log(__FILE__, __FUNCTION__,
            "RecvPartyChangeMaster - partyID=%u, newMasterID=%u",
            stChangeMaster.dwPartyID, stChangeMaster.dwNewMasterID);
    };

    CLogicThreadManager::Instance().DoJob(0, funcAllThreads);

    return true;
}

// Per IDA 0x1401FFA60: RecvPartyUpdateInfo
bool CCommunitySocket::RecvPartyUpdateInfo(XPacket* xPacket) {
    // Per IDA: 解析更新信息
    unsigned int dwPartyID = 0;
    UXMapID uxMazeID = {};

    xPacket->XParse >> dwPartyID;
    xPacket->XParse >> uxMazeID.nMapID;

    // Per IDA: 分发到所有逻辑线程
    std::function<void()> func = [dwPartyID, uxMazeID]() {
        GreenDamTan_log(__FILE__, __FUNCTION__,
            "RecvPartyUpdateInfo - partyID=%u, mazeID=%lld", dwPartyID, uxMazeID.nMapID);
    };

    CLogicThreadManager::Instance().DoJob(0, func);

    return true;
}

// Per IDA: RecvUpdatePartyMember
bool CCommunitySocket::RecvUpdatePartyMember(XPacket* xPacket) {
    // TODO: Implement party member update
    (void)xPacket;
    return true;
}

// Per IDA 0x1401FFBB0: RecvPartyEnterMaze
bool CCommunitySocket::RecvPartyEnterMaze(XPacket* xPacket) {
    // Per IDA: 解析进入迷宫数据
    unsigned int dwPartyID = 0;
    PS_ENTER_MAP_RES stRecvEnterMap = {};

    xPacket->XParse >> dwPartyID;
    *xPacket >> stRecvEnterMap;

    // Per IDA: 查找用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stRecvEnterMap.dwUserID) : nullptr;

    if (pUser) {
        if (!pUser->GetArea()) {
            return false;
        }

        pUser->IncrementJobCount();

        // Per IDA: 分发到用户的逻辑线程
        std::function<void()> func = [pUser, dwPartyID, stRecvEnterMap]() {
            GreenDamTan_log(__FILE__, __FUNCTION__,
                "RecvPartyEnterMaze - partyID=%u, userID=%u", dwPartyID, stRecvEnterMap.dwUserID);
        };

        CLogicThreadManager::Instance().DoJob(dwPartyID, func);
    }

    return true;
}

// Per IDA 0x140201370: RecvPartyDelete
bool CCommunitySocket::RecvPartyDelete(XPacket* xPacket) {
    // Per IDA: 解析删除队伍数据
    PS_PARTY_LEAVE stRecvPartyLeave = {};
    *xPacket >> stRecvPartyLeave;

    // Per IDA: 查找离开的成员用户
    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stRecvPartyLeave.dwLeaveMember) : nullptr;

    if (pUser) {
        if (!pUser->GetArea()) {
            return false;
        }

        pUser->IncrementJobCount();

        // Per IDA: 分发到用户的逻辑线程
        std::function<void()> func = [pUser, stRecvPartyLeave]() {
            GreenDamTan_log(__FILE__, __FUNCTION__,
                "RecvPartyDelete - partyID=%u, leaveMember=%u",
                stRecvPartyLeave.dwPartyID, stRecvPartyLeave.dwLeaveMember);
        };

        CLogicThreadManager::Instance().DoJob(stRecvPartyLeave.dwPartyID, func);
    }

    // Per IDA: 分发到所有逻辑线程
    std::function<void()> funcAllThreads = [stRecvPartyLeave]() {
        GreenDamTan_log(__FILE__, __FUNCTION__,
            "RecvPartyDelete (broadcast) - partyID=%u", stRecvPartyLeave.dwPartyID);
    };

    CLogicThreadManager::Instance().DoJob(0, funcAllThreads);

    return true;
}

// Per IDA 0x1401FD2A0: RecvPartyInfo
bool CCommunitySocket::RecvPartyInfo(XPacket* xPacket) {
    // Per IDA: 解析队伍信息
    unsigned int dwActorID = 0;
    PS_PARTY_INFO stPartyInfo = {};

    xPacket->XParse >> dwActorID;
    *xPacket >> stPartyInfo;

    // Per IDA: 分发到所有逻辑线程
    std::function<void()> func = [dwActorID, stPartyInfo]() {
        GreenDamTan_log(__FILE__, __FUNCTION__,
            "RecvPartyInfo - actorID=%u, partyID=%u, memberCount=%zu",
            dwActorID, stPartyInfo.dwPartyID, stPartyInfo.vecPartyMember.size());
    };

    CLogicThreadManager::Instance().DoJob(0, func);

    return true;
}

// Remaining stub implementations for CCommunitySocket packet handlers
bool CCommunitySocket::RecvPartyEnterServer(XPacket*) { return true; }

// ============================================================================
// RecvPartyInvite - 处理队伍邀请 (IDA 0x1401FE250)
// 解析 PS_REQ_PARTY_INVITE + dwUAID + byLevel[24] + dwPartyID
// nResult<=0: 发送 (0x12,1) 邀请给被邀请者（含匹配日期/交易/世界类型/安全区检查，失败发拒绝）
// nResult>0: 发送 (0x12,1) 邀请确认给请求者
// ============================================================================
bool CCommunitySocket::RecvPartyInvite(XPacket* xPacket)
{
    PS_REQ_PARTY_INVITE stInvite;
    std::uint32_t dwUAID = 0;
    std::uint8_t byLevel[24] = {};
    std::uint32_t dwPartyID = 0;

    *xPacket >> stInvite;
    xPacket->XParse >> dwUAID;
    for (int i = 0; i < 24; ++i) {
        xPacket->XParse >> byLevel[i];
    }
    xPacket->XParse >> dwPartyID;

    if (stInvite.nResult <= 0) {
        CUser* pInviteActorID = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stInvite.dwInviteActorID);
        if (!pInviteActorID)
            return false;
        if (!pInviteActorID->GetArea())
            return false;

        pInviteActorID->IncrementJobCount();
        CLogicThreadManager::Instance().DoJob(pInviteActorID->GetMapInsID().nMapID,
            [pInviteActorID, stInvite, dwUAID, byLevel, dwPartyID]() {
                if (!pInviteActorID || !pInviteActorID->IsLive())
                    return;
                auto pPartyPtr = pInviteActorID->GetGOC_Party(false);
                auto pInvenPtr = pInviteActorID->GetGOC_Inventory(false);
                if (pPartyPtr && pPartyPtr->IsMatchingDate() ||
                    pInvenPtr && pInvenPtr->GetTradeState() != E_TRADE_STATE_NONE) {
                    // 匹配中或交易中 - 发送拒绝 (0xF4, 0x13)
                    PS_PARTY_REJECT stReject;
                    stReject.dwRejectID = pInviteActorID->GetActorID().dwActorID;
                    stReject.dwReqActor = stInvite.dwReqActorID;
                    std::wstring strName = pInviteActorID->GetName();
                    std::wcsncpy(stReject.strRejectName, strName.c_str(), 20);
                    stReject.strRejectName[20] = L'\0';
                    stReject.dwErrorID = 53028;
                    XSendPacket xSendPacket(0xF4u, 0x13u);
                    xSendPacket << stReject;
                    TXSingleton<XGameServer>::Instance()->GetCommunitySocket().SendCheck(&xSendPacket);
                } else if (pInviteActorID->GetArea()) {
                    XArea* pArea = pInviteActorID->GetArea();
                    if (pArea->GetWorldType() != 2) {
                        // 不在战场 - 发送邀请 (0x12, 1)
                        PS_REQ_PARTY_INVITE stNewInvite = stInvite;
                        XSendPacket xSendPacket(0x12u, 1u);
                        xSendPacket << stNewInvite;
                        CGocNetwork::Send(pInviteActorID, xSendPacket);

                        ST_LOG_GAME stLog;
                        stLog._sMainType = 22;
                        stLog._sSubType = 6;
                        stLog._nUAID = static_cast<int>(dwUAID);
                        stLog._nUCID = stNewInvite.dwReqActorID;
                        stLog.nParam0 = byLevel[0];
                        stLog.nParam1 = pInviteActorID->GetActorID().dwActorID;
                        stLog.nParam2 = pInviteActorID->GetLevel();
                        stLog.nParam6 = dwPartyID;
                        TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);

                        ST_LOG_GAME stLogRecv;
                        stLogRecv._sMainType = 22;
                        stLogRecv._sSubType = 12;
                        stLogRecv._nUAID = static_cast<int>(pInviteActorID->GetUAID());
                        stLogRecv._nUCID = pInviteActorID->GetActorID().dwActorID;
                        stLogRecv.nParam0 = pInviteActorID->GetLevel();
                        stLogRecv.nParam1 = stNewInvite.dwReqActorID;
                        stLogRecv.nParam6 = dwPartyID;
                        TXSingleton<XGameServer>::Instance()->SendDBLog(stLogRecv);
                    } else {
                        // 战场中 - 先构建拒绝 (0xF4, 0x13) 53031
                        PS_PARTY_REJECT stReject;
                        stReject.dwRejectID = pInviteActorID->GetActorID().dwActorID;
                        stReject.dwReqActor = stInvite.dwReqActorID;
                        std::wstring strName = pInviteActorID->GetName();
                        std::wcsncpy(stReject.strRejectName, strName.c_str(), 20);
                        stReject.strRejectName[20] = L'\0';
                        stReject.dwErrorID = 53031;
                        CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
                        if (!pBattleZone) {
                            // 战场类型转换失败 - 发送拒绝 53031
                            XSendPacket xSendPacket(0xF4u, 0x13u);
                            xSendPacket << stReject;
                            TXSingleton<XGameServer>::Instance()->GetCommunitySocket().SendCheck(&xSendPacket);
                            return;
                        }
                        if (pBattleZone->IsInSafetyZone(pInviteActorID)) {
                            // 安全区内 - 发送邀请
                            PS_REQ_PARTY_INVITE stNewInvite = stInvite;
                            XSendPacket xSendPacket(0x12u, 1u);
                            xSendPacket << stNewInvite;
                            CGocNetwork::Send(pInviteActorID, xSendPacket);

                            ST_LOG_GAME stLog;
                            stLog._sMainType = 22;
                            stLog._sSubType = 6;
                            stLog._nUAID = static_cast<int>(dwUAID);
                            stLog._nUCID = stNewInvite.dwReqActorID;
                            stLog.nParam0 = byLevel[0];
                            stLog.nParam1 = pInviteActorID->GetActorID().dwActorID;
                            stLog.nParam2 = pInviteActorID->GetLevel();
                            stLog.nParam6 = dwPartyID;
                            TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);

                            ST_LOG_GAME stLogRecv;
                            stLogRecv._sMainType = 22;
                            stLogRecv._sSubType = 12;
                            stLogRecv._nUAID = static_cast<int>(pInviteActorID->GetUAID());
                            stLogRecv._nUCID = pInviteActorID->GetActorID().dwActorID;
                            stLogRecv.nParam0 = pInviteActorID->GetLevel();
                            stLogRecv.nParam1 = stNewInvite.dwReqActorID;
                            stLogRecv.nParam6 = dwPartyID;
                            TXSingleton<XGameServer>::Instance()->SendDBLog(stLogRecv);
                        } else {
                            // 非安全区 - 发送拒绝 53031
                            XSendPacket xSendPacket(0xF4u, 0x13u);
                            xSendPacket << stReject;
                            TXSingleton<XGameServer>::Instance()->GetCommunitySocket().SendCheck(&xSendPacket);
                        }
                    }
                } else {
                    // 无 Area - 发送拒绝 (0xF4, 0x13)
                    PS_PARTY_REJECT stReject;
                    stReject.dwRejectID = pInviteActorID->GetActorID().dwActorID;
                    stReject.dwReqActor = stInvite.dwReqActorID;
                    std::wstring strName = pInviteActorID->GetName();
                    std::wcsncpy(stReject.strRejectName, strName.c_str(), 20);
                    stReject.strRejectName[20] = L'\0';
                    stReject.dwErrorID = 53011;
                    XSendPacket xSendPacket(0xF4u, 0x13u);
                    xSendPacket << stReject;
                    TXSingleton<XGameServer>::Instance()->GetCommunitySocket().SendCheck(&xSendPacket);
                }
            });

        CLogicThreadManager::Instance().DoJob(pInviteActorID->GetMapInsID().nMapID,
            [pInviteActorID]() {
                if (pInviteActorID)
                    pInviteActorID->DecrementJobCount();
            });
    } else {
        CUser* pReqUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stInvite.dwReqActorID);
        if (!pReqUser)
            return false;
        if (!pReqUser->GetArea())
            return false;

        pReqUser->IncrementJobCount();
        CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID,
            [pReqUser, stInvite]() {
                if (!pReqUser || !pReqUser->IsLive())
                    return;
                PS_REQ_PARTY_INVITE stNewInvite = stInvite;
                XSendPacket xSendPacket(0x12u, 1u);
                xSendPacket << stNewInvite;
                CGocNetwork::Send(pReqUser, xSendPacket);
            });

        CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID,
            [pReqUser]() {
                if (pReqUser)
                    pReqUser->DecrementJobCount();
            });
    }

    return false;
}

// ============================================================================
// RecvPartyAccept - 处理队伍邀请接受 (IDA 0x1401FF380)
// 解析 PS_RES_PARTY_ACCEPT -> 发送 (0x12,2) 给请求者
// ============================================================================
bool CCommunitySocket::RecvPartyAccept(XPacket* xPacket)
{
    PS_RES_PARTY_ACCEPT stPartyAccept;
    xPacket->XParse >> stPartyAccept.dwAcceptID;
    xPacket->XParse >> stPartyAccept.nResult;

    CUser* pReqUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stPartyAccept.dwAcceptID);
    if (!pReqUser)
        return false;
    if (!pReqUser->GetArea())
        return false;

    pReqUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID,
        [pReqUser, stPartyAccept]() {
            if (!pReqUser || !pReqUser->IsLive())
                return;
            PS_RES_PARTY_ACCEPT stNewPartyAccept = stPartyAccept;
            XSendPacket xSendPacket(0x12u, 2u);
            xSendPacket << stNewPartyAccept;
            CGocNetwork::Send(pReqUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID,
        [pReqUser]() {
            if (pReqUser)
                pReqUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// RecvPartyReject - 处理队伍邀请拒绝 (IDA 0x1401FF6C0)
// 解析 PS_PARTY_REJECT -> 发送 (0x12,8) 给请求者
// ============================================================================
bool CCommunitySocket::RecvPartyReject(XPacket* xPacket)
{
    PS_PARTY_REJECT stReject;
    *xPacket >> stReject;

    CUser* pReqUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stReject.dwReqActor);
    if (!pReqUser)
        return false;
    if (!pReqUser->GetArea())
        return false;

    pReqUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID,
        [pReqUser, stReject]() {
            if (!pReqUser || !pReqUser->IsLive())
                return;
            PS_PARTY_REJECT stNewReject = stReject;
            XSendPacket xSendPacket(0x12u, 8u);
            xSendPacket << stNewReject;
            CGocNetwork::Send(pReqUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID,
        [pReqUser]() {
            if (pReqUser)
                pReqUser->DecrementJobCount();
        });

    return true;
}
bool CCommunitySocket::RecvPartyMessage(XPacket*) { return true; }

// ============================================================================
// RecvPartyMatchingEnter - 处理队伍匹配进入 (IDA 0x1402033C0)
// 解析 dwActorID + byResult + dwEnterActorID + ST_MATCHING_INFO
// byResult 分派：100=错误53035, 0=错误53028, 其他=匹配确认+发送 (0x12,0x30)
// ============================================================================
bool CCommunitySocket::RecvPartyMatchingEnter(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    std::uint8_t byResult = 0;
    std::uint32_t dwEnterActorID = 0;
    ST_MATCHING_INFO stMatchingInfo;

    xPacket->XParse >> dwActorID;
    xPacket->XParse >> byResult;
    xPacket->XParse >> dwEnterActorID;
    *xPacket >> stMatchingInfo;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, byResult, dwEnterActorID, stMatchingInfo]() {
            if (!pUser || !pUser->IsLive())
                return;
            if (byResult) {
                if (byResult == 100) {
                    pUser->SendErrorMessage(0x12u, 0x30u, 53035);
                } else {
                    if (pUser->GetActorID().dwActorID == dwEnterActorID) {
                        auto pPartyPtr = pUser->GetGOC_Party(false);
                        if (pPartyPtr) {
                            pPartyPtr->SetMatchingDate(TXSingleton<XGameServer>::Instance()->GetCurDate());
                        }
                    }
                    ST_MATCHING_INFO st = stMatchingInfo;
                    XSendPacket xSendPacket(0x12u, 0x30u);
                    xSendPacket << st;
                    CGocNetwork::Send(pUser, xSendPacket);
                }
            } else {
                pUser->SendErrorMessage(0x12u, 0x30u, 53028);
            }
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// RecvPartyMatchingExit - 处理队伍匹配退出 (IDA 0x1402039B0)
// 解析 dwActorID + dwExitActorID + byReason
// 若 ActorID==dwExitActorID 或 byReason==0 则重置匹配时间；发送 (0x12,0x31)
// ============================================================================
bool CCommunitySocket::RecvPartyMatchingExit(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    std::uint32_t dwExitActorID = 0;
    std::uint8_t byReason = 0;

    xPacket->XParse >> dwActorID;
    xPacket->XParse >> dwExitActorID;
    xPacket->XParse >> byReason;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, dwExitActorID, byReason]() {
            if (!pUser || !pUser->IsLive())
                return;
            if (pUser->GetActorID().dwActorID == dwExitActorID || byReason == 0) {
                auto pPartyPtr = pUser->GetGOC_Party(false);
                if (pPartyPtr) {
                    pPartyPtr->SetMatchingDate(0);
                }
            }
            XSendPacket xSendPacket(0x12u, 0x31u);
            xSendPacket.XParse << dwExitActorID;
            xSendPacket.XParse << byReason;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyMatchingCheck - 处理队伍匹配检查 (IDA 0x140203E10)
// 解析 dwActorID -> 发送 (0x12,0x32) 空包给用户
// ============================================================================
bool CCommunitySocket::RecvPartyMatchingCheck(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    xPacket->XParse >> dwActorID;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (!pUser || !pUser->IsLive())
                return;
            XSendPacket xSendPacket(0x12u, 0x32u);
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyMatchingReset - 处理队伍匹配重置 (IDA 0x140204100)
// 解析 dwActorID -> 发送 (0x12,0x33) 空包给用户
// ============================================================================
bool CCommunitySocket::RecvPartyMatchingReset(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    xPacket->XParse >> dwActorID;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (!pUser || !pUser->IsLive())
                return;
            XSendPacket xSendPacket(0x12u, 0x33u);
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyMatchingWait - 处理队伍匹配等待 (IDA 0x1402043F0)
// 解析 dwActorID + dwLeaderID -> 发送 (0x12,0x34) + dwLeaderID 给用户
// ============================================================================
bool CCommunitySocket::RecvPartyMatchingWait(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    std::uint32_t dwLeaderID = 0;
    xPacket->XParse >> dwActorID;
    xPacket->XParse >> dwLeaderID;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, dwLeaderID]() {
            if (!pUser || !pUser->IsLive())
                return;
            XSendPacket xSendPacket(0x12u, 0x34u);
            xSendPacket.XParse << dwLeaderID;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitAdd - 处理队伍招募添加结果 (IDA 0x140205470)
// 解析 PS_SERVER_PARTY_RECRUIT_ADD_RES -> DoJob x2
// lambda112: nResult==1 发送 (0x12,0x38) 成功招募包，否则发错误
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitAdd(XPacket* xPacket)
{
    PS_SERVER_PARTY_RECRUIT_ADD_RES psRecruitRes;
    *xPacket >> psRecruitRes;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(psRecruitRes.dwUCID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, psRecruitRes]() {
            if (!pUser || !pUser->IsLive())
                return;
            if (psRecruitRes.nResult == 1) {
                ST_PARTY_RECRUIT st = psRecruitRes.stRecruitInfo.stRecruit;
                XSendPacket xSendPacket(0x12u, 0x38u);
                xSendPacket.XParse << static_cast<std::uint8_t>(psRecruitRes.nRemainSec);
                xSendPacket.XParse << psRecruitRes.dwRecruitID;
                xSendPacket << st;
                CGocNetwork::Send(pUser, xSendPacket);
            } else {
                pUser->SendErrorMessage(0x12u, 0x38u, static_cast<std::uint16_t>(psRecruitRes.nResult));
            }
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitDel - 处理队伍招募删除 (IDA 0x140205AC0)
// 解析 ST_PARTY_RECRUIT_DEL_LIST，遍历每个 ST_PARTY_RECRUIT_DEL
// 对每个 dwMasterID 查找用户 -> DoJob x2（lambda114 发送 (0x12,0x3D) + dwRecruitID）
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitDel(XPacket* xPacket)
{
    ST_PARTY_RECRUIT_DEL_LIST stDelRecruitList;
    *xPacket >> stDelRecruitList;

    for (const auto& stInfo : stDelRecruitList.vecInfo) {
        CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stInfo.dwMasterID);
        if (!pUser)
            continue;
        if (!pUser->GetArea())
            return false;

        pUser->IncrementJobCount();
        std::uint32_t dwRecruitID = stInfo.dwRecruitID;
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
            [pUser, dwRecruitID]() {
                if (!pUser || !pUser->IsLive())
                    return;
                XSendPacket xSendPacket(0x12u, 0x3Du);
                xSendPacket.XParse << dwRecruitID;
                CGocNetwork::Send(pUser, xSendPacket);
            });

        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
            [pUser]() {
                if (pUser)
                    pUser->DecrementJobCount();
            });
    }

    return true;
}
// ============================================================================
// RecvPartyRecruitApply - 处理队伍招募申请 (IDA 0x140205E90)
// 解析 dwActorID + byResult + dwRecruitID + byCount -> DoJob x2
// lambda116: byResult 非零发 (0x12,0x39)+0+byResult，否则发 (0x12,0x39)+dwRecruitID+byCount
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitApply(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    std::uint8_t byResult = 0;
    std::uint32_t dwRecruitID = 0;
    std::uint8_t byCount = 0;

    xPacket->XParse >> dwActorID;
    xPacket->XParse >> byResult;
    xPacket->XParse >> dwRecruitID;
    xPacket->XParse >> byCount;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, byResult, dwRecruitID, byCount]() {
            if (!pUser || !pUser->IsLive())
                return;
            if (byResult) {
                XSendPacket packet(0x12u, 0x39u);
                packet.XParse << static_cast<std::uint32_t>(0);
                packet.XParse << byResult;
                CGocNetwork::Send(pUser, packet);
            } else {
                XSendPacket xSendPacket(0x12u, 0x39u);
                xSendPacket.XParse << dwRecruitID;
                xSendPacket.XParse << byCount;
                CGocNetwork::Send(pUser, xSendPacket);
            }
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitApplyAccept - 处理招募申请接受结果 (IDA 0x140206360)
// 解析 dwActorID + nErrorCode -> DoJob x2（lambda118 发送 (0x12,0x3A) 错误消息）
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitApplyAccept(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    int nErrorCode = 0;
    xPacket->XParse >> dwActorID;
    xPacket->XParse >> nErrorCode;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, nErrorCode]() {
            if (pUser && pUser->IsLive()) {
                pUser->SendErrorMessage(0x12u, 0x3Au, static_cast<std::uint16_t>(nErrorCode));
            }
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitApplyReject - 处理招募申请拒绝结果 (IDA 0x140206600)
// 解析 dwActorID + dwRejectID -> DoJob x2
// lambda120: dwRejectID 非零发 (0x12,0x3B)+dwRejectID，否则错误 53027
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitApplyReject(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    std::uint32_t dwRejectID = 0;
    xPacket->XParse >> dwActorID;
    xPacket->XParse >> dwRejectID;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, dwRejectID]() {
            if (!pUser || !pUser->IsLive())
                return;
            if (dwRejectID) {
                XSendPacket xSendPacket(0x12u, 0x3Bu);
                xSendPacket.XParse << dwRejectID;
                CGocNetwork::Send(pUser, xSendPacket);
            } else {
                pUser->SendErrorMessage(0x12u, 0x3Bu, 53027);
            }
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitApplyUpdate - 处理招募申请更新 (IDA 0x140207F60)
// 解析 dwActorID + ST_PARTY_RECRUIT_UPDATE -> DoJob x2
// lambda133: 发送 (0x12,0x3C) + ST_PARTY_RECRUIT_UPDATE
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitApplyUpdate(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    ST_PARTY_RECRUIT_UPDATE stUpdate;
    xPacket->XParse >> dwActorID;
    *xPacket >> stUpdate;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stUpdate]() {
            if (!pUser || !pUser->IsLive())
                return;
            ST_PARTY_RECRUIT_UPDATE st = stUpdate;
            XSendPacket xSendPacket(0x12u, 0x3Cu);
            xSendPacket << st;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitList - 处理队伍招募列表 (IDA 0x140206970)
// 解析 dwActorID + ST_PARTY_RECRUIT_LIST -> DoJob x2
// lambda122: 发送 (0x12,0x36) + ST_PARTY_RECRUIT_LIST
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitList(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    ST_PARTY_RECRUIT_LIST stRecruitList;
    xPacket->XParse >> dwActorID;
    *xPacket >> stRecruitList;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stRecruitList]() {
            if (!pUser || !pUser->IsLive())
                return;
            XSendPacket xSendPacket(0x12u, 0x36u);
            xSendPacket << stRecruitList;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitMyApplyList - 处理我的招募申请列表 (IDA 0x140206E70)
// 解析 dwActorID + ST_PARTY_RECRUIT_LIST -> DoJob x2
// lambda124: 发送 (0x12,0x37) + ST_PARTY_RECRUIT_LIST
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitMyApplyList(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    ST_PARTY_RECRUIT_LIST stApplyList;
    xPacket->XParse >> dwActorID;
    *xPacket >> stApplyList;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stApplyList]() {
            if (!pUser || !pUser->IsLive())
                return;
            XSendPacket xSendPacket(0x12u, 0x37u);
            xSendPacket << stApplyList;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitApplyList - 处理队伍招募申请列表 (IDA 0x140207250)
// 解析 dwActorID + ST_APPLY_MEMBER_LIST -> DoJob x2
// lambda126: 发送 (0x12,0x3E) + ST_APPLY_MEMBER_LIST
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitApplyList(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    ST_APPLY_MEMBER_LIST stApplyList;
    xPacket->XParse >> dwActorID;
    *xPacket >> stApplyList;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stApplyList]() {
            if (!pUser || !pUser->IsLive())
                return;
            XSendPacket xSendPacket(0x12u, 0x3Eu);
            xSendPacket << stApplyList;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitInfo - 处理队伍招募信息 (IDA 0x140207770)
// 解析 dwActorID + ST_PARTY_RECRUIT -> DoJob x2
// lambda129: 发送 (0x12,0x3F) + ST_PARTY_RECRUIT
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitInfo(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    ST_PARTY_RECRUIT stRecruit;
    xPacket->XParse >> dwActorID;
    *xPacket >> stRecruit;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stRecruit]() {
            if (!pUser || !pUser->IsLive())
                return;
            XSendPacket xSendPacket(0x12u, 0x3Fu);
            xSendPacket << stRecruit;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitApplyDel - 处理队伍招募申请删除 (IDA 0x140207BD0)
// 解析 dwActorID + dwSendActorID + dwRecruit -> DoJob x2
// lambda131: 发送 (0x12,0x40) + dwSendActorID + dwRecruit
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitApplyDel(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    std::uint32_t dwSendActorID = 0;
    std::uint32_t dwRecruit = 0;
    xPacket->XParse >> dwActorID;
    xPacket->XParse >> dwSendActorID;
    xPacket->XParse >> dwRecruit;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, dwSendActorID, dwRecruit]() {
            if (!pUser || !pUser->IsLive())
                return;
            XSendPacket xSendPacket(0x12u, 0x40u);
            xSendPacket.XParse << dwSendActorID;
            xSendPacket.XParse << dwRecruit;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitApplyInfo - 处理队伍招募申请信息 (IDA 0x1402082E0)
// 解析 ST_PARTY_RECRUIT_APPLY_INFO -> DoJob x2
// lambda135: 发送 (0x12,0x41) + ST_PARTY_RECRUIT_APPLY_INFO
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitApplyInfo(XPacket* xPacket)
{
    ST_PARTY_RECRUIT_APPLY_INFO stInfo;
    *xPacket >> stInfo;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stInfo.dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stInfo]() {
            if (!pUser || !pUser->IsLive())
                return;
            XSendPacket xSendPacket(0x12u, 0x41u);
            xSendPacket << stInfo;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitApplyNotice - 处理队伍招募申请通知 (IDA 0x140208840)
// 解析 dwActorID + ST_APPLY_MEMBER -> DoJob x2
// lambda137: 发送 (0x12,0x42) + ST_APPLY_MEMBER
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitApplyNotice(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    ST_APPLY_MEMBER stApply;
    xPacket->XParse >> dwActorID;
    *xPacket >> stApply;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stApply]() {
            if (!pUser || !pUser->IsLive())
                return;
            XSendPacket xSendPacket(0x12u, 0x42u);
            xSendPacket << stApply;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyRecruitApplyAcceptCheck - 队伍招募申请接受校验 (IDA 0x14020F030)
// TODO: 阻塞 - lambda179 (0x14020F390) 为巨型校验函数，依赖
// CGocParty::IsParty/CGocForce 状态、CBattleZone::IsInSafetyZone、
// XArea::GetWorldType/GetTBMapID、XResourceMgr::GetTB_DISTRICT 校验链，
// 待对应依赖完整后单独还原。
// ============================================================================
bool CCommunitySocket::RecvPartyRecruitApplyAcceptCheck(XPacket*) { return true; }
// ============================================================================
// RecvPartyNameChange - 处理队伍名称变更 (IDA 0x14020B930)
// 解析 dwUCID + PS_CHANGE_NAME -> DoJob x2
// lambda157: 队伍存在则 ChangePartyMemberName + 发送 (8,0x53) + PS_CHANGE_NAME
// ============================================================================
bool CCommunitySocket::RecvPartyNameChange(XPacket* xPacket)
{
    std::uint32_t dwUCID = 0;
    PS_CHANGE_NAME stInfo;
    xPacket->XParse >> dwUCID;
    *xPacket >> stInfo;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwUCID);
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stInfo]() {
            if (!pUser || !pUser->IsLive())
                return;
            auto pPartyPtr = pUser->GetGOC<CGocParty>();
            if (pPartyPtr) {
                PS_CHANGE_NAME stChangeName = stInfo;
                pPartyPtr->ChangePartyMemberName(stChangeName);
                XSendPacket xSendPacket(8u, 0x53u);
                xSendPacket << stChangeName;
                CGocNetwork::Send(pUser, xSendPacket);
            }
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvPartyMatchingMaze - 队伍匹配进入迷宫 (IDA 0x140204730)
// TODO: 阻塞 - lambda109 (0x140204A90) 为巨型迷宫进入函数，依赖
// XPartyManager::GetParty/AddParty（ThreadLocalData m_xPartyMgr 未还原）、
// XWorldResMgr::GetPortalPos（stub）等，待依赖完整后单独还原。
// ============================================================================
bool CCommunitySocket::RecvPartyMatchingMaze(XPacket*) { return true; }
// ============================================================================
// RecvPartyMazeClear - 队伍迷宫清除 (IDA 0x1402076C0)
// 解析 dwPartyID -> lambda128 (0x140207740) DoJobAllThread:
// ThreadLocalData::GetInstance()->m_xPartyMgr.RecvPartyMazeClear(dwPartyID)。
// 活动层 m_xPartyMgr 为指针占位，空时跳过。返回 1。
// ============================================================================
bool CCommunitySocket::RecvPartyMazeClear(XPacket* xPacket)
{
    std::uint32_t dwPartyID = 0;
    xPacket->XParse >> dwPartyID;

    // Per IDA lambda128 (0x140207740)
    CLogicThreadManager::Instance().DoJobAllThread([dwPartyID]() {
        XPartyManager* pPartyMgr = ThreadLocalData::GetInstance()->GetPartyMgr();
        if (pPartyMgr) {
            pPartyMgr->RecvPartyMazeClear(dwPartyID);
        }
    });
    return true;
}

// ============================================================================
// Force implementations
// ============================================================================

// Per IDA 0x140211C50: RecvForceCreate
// 创建 Force 响应：解析 PS_REQ_FORCE_CREATE -> lambda190 广播到所有线程
// （XForceManager::CreateForce(stReq)）-> 队长与成员分别 lambda191 DoJob
// （存活时 XForceManager::CreateForce(pUser, dwForceID)）+ lambda192 递减。
bool CCommunitySocket::RecvForceCreate(XPacket* xPacket) {
    PS_REQ_FORCE_CREATE stCreatForce;
    *xPacket >> stCreatForce;

    // Per IDA lambda190 (0x140212160): 所有线程创建 Force 数据
    {
        std::function<void()> funcCreate = [stCreatForce]() {
            PS_REQ_FORCE_CREATE stReq = stCreatForce;
            ThreadLocalData::GetInstance()->GetForceMgr()->CreateForce(stReq);
        };
        CLogicThreadManager::Instance().DoJobAllThread(funcCreate);
    }

    const std::uint32_t dwForceID = stCreatForce.dwForceID;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pMaster = pServer ? pServer->FindActorIDToUser(stCreatForce.masterInfo.dwMemberID) : nullptr;

    if (pMaster) {
        if (!pMaster->GetArea()) {
            return false;
        }

        pMaster->IncrementJobCount();

        // Per IDA lambda191 (0x140212190): 队长线程注册 Force
        std::function<void()> funcMaster = [pMaster, dwForceID]() {
            if (pMaster && pMaster->IsLive()) {
                ThreadLocalData::GetInstance()->GetForceMgr()->CreateForce(pMaster, dwForceID);
            }
        };
        CLogicThreadManager::Instance().DoJob(pMaster->GetMapInsID().nMapID, funcMaster);

        // Per IDA lambda192 (0x140427E60): 递减计数
        std::function<void()> funcDecMaster = [pMaster]() {
            pMaster->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pMaster->GetMapInsID().nMapID, funcDecMaster);
    }

    CUser* pMember = pServer ? pServer->FindActorIDToUser(stCreatForce.memberInfo.dwMemberID) : nullptr;

    if (pMember) {
        if (!pMember->GetArea()) {
            return false;
        }

        pMember->IncrementJobCount();

        // Per IDA lambda191: 成员线程注册 Force（同一任务体）
        std::function<void()> funcMember = [pMember, dwForceID]() {
            if (pMember && pMember->IsLive()) {
                ThreadLocalData::GetInstance()->GetForceMgr()->CreateForce(pMember, dwForceID);
            }
        };
        CLogicThreadManager::Instance().DoJob(pMember->GetMapInsID().nMapID, funcMember);

        std::function<void()> funcDecMember = [pMember]() {
            pMember->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pMember->GetMapInsID().nMapID, funcDecMember);
    }

    return true;
}

// Per IDA 0x140212200: RecvForceJoinMember
// 成员加入 Force 响应：解析 PS_FORCE_ADDMEMBER + PS_FORCE_INFO ->
// lambda195 DoJob（XForceManager::AddMember + (0xF3,4) 上报地图信息）+
// lambda192 递减 -> lambda53 广播到所有线程。
bool CCommunitySocket::RecvForceJoinMember(XPacket* xPacket) {
    PS_FORCE_INFO stRecvForceInfo;
    PS_FORCE_ADDMEMBER stRecvAddMember;

    *xPacket >> stRecvAddMember;
    *xPacket >> stRecvForceInfo;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pMember = pServer ? pServer->FindActorIDToUser(stRecvAddMember.stMember.dwMemberID) : nullptr;

    if (pMember) {
        if (!pMember->GetArea()) {
            return false;
        }

        pMember->IncrementJobCount();

        // Per IDA lambda195 (0x140212560): 成员线程加入 Force 并上报地图信息
        std::function<void()> func = [pMember, stRecvAddMember, stRecvForceInfo]() {
            if (!pMember || !pMember->IsLive()) {
                return;
            }
            if (!pMember->GetArea()) {
                return;
            }

            PS_FORCE_INFO stForceInfo = stRecvForceInfo;
            PS_FORCE_ADDMEMBER stAddMember = stRecvAddMember;
            ThreadLocalData::GetInstance()->GetForceMgr()->AddMember(pMember, stAddMember, stForceInfo);

            // Per IDA: (0xF3,4) PS_UPDATE_USER_MAP_INFO 上报 ControlServer
            PS_UPDATE_USER_MAP_INFO stMapInfo = {};
            stMapInfo.dwUAID = pMember->GetUAID();
            stMapInfo.dwActorID = pMember->GetActorID().dwActorID;
            stMapInfo.uxMapID = pMember->GetMapInsID();
            stMapInfo.stPartyInfo.byGroupType = 2;
            stMapInfo.stPartyInfo.nID = stForceInfo.dwForceID;
            stMapInfo.biAuthSessionID = pMember->GetAuthSessionID();
            XSendPacket xSendPacket(0xF3, 4);
            xSendPacket << stMapInfo;
            XGameServer::Instance()->GetControlSocket().SendCheck(&xSendPacket);
        };
        CLogicThreadManager::Instance().DoJob(pMember->GetMapInsID().nMapID, func);

        // Per IDA lambda192 (0x140427E60): 递减计数
        std::function<void()> funcDec = [pMember]() {
            pMember->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pMember->GetMapInsID().nMapID, funcDec);
    }

    // Per IDA lambda53: 广播成员加入到所有线程
    {
        std::function<void()> funcAll = [stRecvAddMember]() {
            PS_FORCE_ADDMEMBER stAddMember = stRecvAddMember;
            ThreadLocalData::GetInstance()->GetForceMgr()->AddMember(stAddMember);
        };
        CLogicThreadManager::Instance().DoJobAllThread(funcAll);
    }

    return true;
}

// Per IDA 0x140212910: RecvForceLeaveMember
// 成员离开 Force 响应：解析 PS_FORCE_LEAVE + dwMasterID + nError。
// nError 非零：lambda198 任务体（SendErrorMessage(0x2E,5,nError)）到队长线程；
// 否则：lambda200 广播 XForceManager::LeaveForce -> 离开成员或队长线程
// lambda133 任务体（(0x12,0x3C) ST_PARTY_RECRUIT_UPDATE）+ lambda201/203
// (0xF3,4) 上报 -> lambda192 递减。
bool CCommunitySocket::RecvForceLeaveMember(XPacket* xPacket) {
    std::uint32_t dwMasterID = 0;
    int nError = 0;
    PS_FORCE_LEAVE stForceLeave;

    *xPacket >> stForceLeave;
    xPacket->XParse >> dwMasterID;
    xPacket->XParse >> nError;

    XGameServer* pServer = XGameServer::Instance();

    if (nError) {
        CUser* pUser = pServer ? pServer->FindActorIDToUser(dwMasterID) : nullptr;

        if (pUser) {
            if (!pUser->GetArea()) {
                return false;
            }

            pUser->IncrementJobCount();

            // Per IDA lambda198 (0x140213040): 队长线程发送离开错误
            std::function<void()> func = [pUser, nError]() {
                if (pUser && pUser->IsLive()) {
                    CGocNetwork::SendErrorMessage(pUser, 0x2E, 5, static_cast<std::uint16_t>(nError));
                }
            };
            CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

            std::function<void()> funcDec = [pUser]() {
                pUser->DecrementJobCount();
            };
            CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
        }

        return true;
    }

    // Per IDA lambda200 (0x1402130D0): 所有线程移除成员
    {
        std::function<void()> funcLeave = [stForceLeave, dwMasterID]() {
            PS_FORCE_LEAVE stLeave = stForceLeave;
            ThreadLocalData::GetInstance()->GetForceMgr()->LeaveForce(stLeave, dwMasterID);
        };
        CLogicThreadManager::Instance().DoJobAllThread(funcLeave);
    }

    CUser* pMember = pServer ? pServer->FindActorIDToUser(stForceLeave.dwLeaveMember) : nullptr;

    if (pMember) {
        if (!pMember->GetArea()) {
            return false;
        }

        pMember->IncrementJobCount();

        // Per IDA lambda133 (0x1402081A0): 离开成员线程通知 (0x12,0x3C)
        std::function<void()> funcMember = [pMember, stForceLeave]() {
            if (!pMember || !pMember->IsLive()) {
                return;
            }
            ST_PARTY_RECRUIT_UPDATE st;
            st.dwActorID = stForceLeave.dwLeaveMember;
            XSendPacket xSendPacket(0x12, 0x3C);
            xSendPacket << st;
            CGocNetwork::Send(pMember, xSendPacket);
        };
        CLogicThreadManager::Instance().DoJob(pMember->GetMapInsID().nMapID, funcMember);

        // Per IDA lambda201 (0x140213100): 离开成员线程上报 (0xF3,4)
        std::function<void()> funcMapMember = [pMember, stForceLeave]() {
            if (!pMember || !pMember->IsLive()) {
                return;
            }
            if (!pMember->GetArea()) {
                return;
            }
            PS_UPDATE_USER_MAP_INFO stMapInfo = {};
            stMapInfo.dwUAID = pMember->GetUAID();
            stMapInfo.dwActorID = pMember->GetActorID().dwActorID;
            stMapInfo.uxMapID = pMember->GetMapInsID();
            stMapInfo.stPartyInfo.byGroupType = 2;
            stMapInfo.stPartyInfo.nID = stForceLeave.dwForceID;
            stMapInfo.biAuthSessionID = pMember->GetAuthSessionID();
            stMapInfo.bLeaveParty = true;
            XSendPacket xSendPacket(0xF3, 4);
            xSendPacket << stMapInfo;
            XGameServer::Instance()->GetControlSocket().SendCheck(&xSendPacket);
        };
        CLogicThreadManager::Instance().DoJob(pMember->GetMapInsID().nMapID, funcMapMember);

        std::function<void()> funcDecMember = [pMember]() {
            pMember->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pMember->GetMapInsID().nMapID, funcDecMember);

        return true;
    }

    CUser* pMaster = pServer ? pServer->FindActorIDToUser(dwMasterID) : nullptr;

    if (pMaster) {
        if (!pMaster->GetArea()) {
            return false;
        }

        pMaster->IncrementJobCount();

        // Per IDA lambda133: 队长线程通知 (0x12,0x3C)
        std::function<void()> funcMaster = [pMaster, stForceLeave]() {
            if (!pMaster || !pMaster->IsLive()) {
                return;
            }
            ST_PARTY_RECRUIT_UPDATE st;
            st.dwActorID = stForceLeave.dwLeaveMember;
            XSendPacket xSendPacket(0x12, 0x3C);
            xSendPacket << st;
            CGocNetwork::Send(pMaster, xSendPacket);
        };
        CLogicThreadManager::Instance().DoJob(pMaster->GetMapInsID().nMapID, funcMaster);

        // Per IDA lambda203 (0x1402132E0): 队长线程上报 (0xF3,4)，离线成员补零
        std::function<void()> funcMapMaster = [pMaster, stForceLeave]() {
            if (!pMaster || !pMaster->IsLive()) {
                return;
            }
            PS_UPDATE_USER_MAP_INFO stMapInfo = {};
            stMapInfo.dwUAID = 0;
            stMapInfo.dwActorID = stForceLeave.dwLeaveMember;
            stMapInfo.uxMapID = UXMapID{};
            stMapInfo.stPartyInfo.byGroupType = 2;
            stMapInfo.stPartyInfo.nID = stForceLeave.dwForceID;
            stMapInfo.biAuthSessionID = 0;
            stMapInfo.bLeaveParty = true;
            XSendPacket xSendPacket(0xF3, 4);
            xSendPacket << stMapInfo;
            XGameServer::Instance()->GetControlSocket().SendCheck(&xSendPacket);
        };
        CLogicThreadManager::Instance().DoJob(pMaster->GetMapInsID().nMapID, funcMapMaster);

        std::function<void()> funcDecMaster = [pMaster]() {
            pMaster->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pMaster->GetMapInsID().nMapID, funcDecMaster);
    }

    return true;
}

// Per IDA 0x140213410: RecvForceChangeMaster
// 更换队长响应：解析 PS_FORCE_CHANGE_MASTER -> nErrorCode 非零记日志返回 0；
// 否则 lambda205 广播到所有线程（XForceManager::ChangeMaster(dwForceID, 新队长)）。
bool CCommunitySocket::RecvForceChangeMaster(XPacket* xPacket) {
    PS_FORCE_CHANGE_MASTER stChangeMaster;
    *xPacket >> stChangeMaster;

    if (stChangeMaster.nErrorCode) {
        LogHelper::LogError("game.contents",
            "RecvForceChangeMaster error[ Errorcode:%d ] ( %d)",
            stChangeMaster.nErrorCode, 4029);
        return false;
    }

    // Per IDA lambda205 (0x140213510): 所有线程更新队长
    std::function<void()> func = [stChangeMaster]() {
        ThreadLocalData::GetInstance()->GetForceMgr()->ChangeMaster(
            stChangeMaster.dwForceID, UXActorID(stChangeMaster.dwNewMasterID));
    };
    CLogicThreadManager::Instance().DoJobAllThread(func);

    return true;
}

// Per IDA 0x140213690: RecvForceDelete
// 解散 Force 响应：解析 PS_FORCE_DELETE -> lambda207 广播到所有线程
// （XForceManager::DeleteForce）。
bool CCommunitySocket::RecvForceDelete(XPacket* xPacket) {
    PS_FORCE_DELETE stForceDelete;
    *xPacket >> stForceDelete;

    // Per IDA lambda207 (0x140213950): 所有线程删除 Force 数据
    std::function<void()> func = [stForceDelete]() {
        PS_FORCE_LEAVE stLeave = {};
        stLeave.dwForceID = stForceDelete.dwForceID;
        stLeave.dwLeaveMember = stForceDelete.dwLeaveMember;
        ThreadLocalData::GetInstance()->GetForceMgr()->DeleteForce(stLeave);
    };
    CLogicThreadManager::Instance().DoJobAllThread(func);

    return true;
}

// Per IDA 0x140213D90: RecvForceEnterMaze
// Force 进迷宫响应：解析 dwForceID + PS_ENTER_MAP_RES -> 按用户找 lambda25
// 任务体（IsParty 门控后 XPartyManager::ResEnterMaze）+ lambda192 递减。
bool CCommunitySocket::RecvForceEnterMaze(XPacket* xPacket) {
    std::uint32_t dwForceID = 0;
    PS_ENTER_MAP_RES stRecvEnterMap;

    xPacket->XParse >> dwForceID;
    *xPacket >> stRecvEnterMap;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stRecvEnterMap.dwUserID) : nullptr;

    if (pUser) {
        if (!pUser->GetArea()) {
            return false;
        }

        pUser->IncrementJobCount();

        // Per IDA lambda25 (0x1401CFCC0): 共享任务体，组 ID 透传 ResEnterMaze
        std::function<void()> func = [pUser, dwForceID, stRecvEnterMap]() {
            if (!pUser || !pUser->IsLive()) {
                return;
            }
            CGocParty* pGocParty = pUser->GetGOC<CGocParty>();
            if (pGocParty && pGocParty->IsParty()) {
                PS_ENTER_MAP_RES stEnterMap = stRecvEnterMap;
                ThreadLocalData::GetInstance()->GetPartyMgr()->ResEnterMaze(pUser, dwForceID, stEnterMap);
            }
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    }

    return true;
}

// Per IDA 0x140214120: RecvForceUpdateInfo
// Force 更新信息：解析 dwForceID + uxMazeID -> lambda214 广播到所有线程
// （XForceManager::ResForceUpdateInfo）。
bool CCommunitySocket::RecvForceUpdateInfo(XPacket* xPacket) {
    std::uint32_t dwForceID = 0;
    UXMapID uxMazeID = {};

    xPacket->XParse >> dwForceID;
    xPacket->XParse >> uxMazeID.nMapID;

    // Per IDA lambda214 (0x140214210): 所有线程更新 Force 迷宫信息
    std::function<void()> func = [dwForceID, uxMazeID]() {
        ThreadLocalData::GetInstance()->GetForceMgr()->ResForceUpdateInfo(dwForceID, uxMazeID);
    };
    CLogicThreadManager::Instance().DoJobAllThread(func);

    return true;
}

// Per IDA 0x140214250: RecvForceEnterServer
// 进入服务器 Force 同步：解析 PS_RES_FORCE_ENTER_SERVER ->
// lambda215 广播（XForceManager::AddForce(stForceInfo)）-> 成员线程
// lambda216 DoJob（RegisterForce(dwForceID, pUser)）+ lambda192 递减。
bool CCommunitySocket::RecvForceEnterServer(XPacket* xPacket) {
    PS_RES_FORCE_ENTER_SERVER stRecvEnterServer;
    *xPacket >> stRecvEnterServer;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stRecvEnterServer.stEnterMember.dwMemberID) : nullptr;

    if (pUser) {
        // Per IDA lambda215 (0x1402145A0): 所有线程重建 Force 数据
        std::function<void()> funcAdd = [stRecvEnterServer]() {
            PS_FORCE_INFO stForceInfo = stRecvEnterServer.stForceInfo;
            ThreadLocalData::GetInstance()->GetForceMgr()->AddForce(stForceInfo);
        };
        CLogicThreadManager::Instance().DoJobAllThread(funcAdd);

        if (!pUser->GetArea()) {
            return false;
        }

        pUser->IncrementJobCount();

        // Per IDA lambda216 (0x140214650): 成员线程注册 Force
        const std::uint32_t dwForceID = stRecvEnterServer.stForceInfo.dwForceID;
        std::function<void()> func = [pUser, dwForceID]() {
            if (pUser && pUser->IsLive()) {
                ThreadLocalData::GetInstance()->GetForceMgr()->RegisterForce(dwForceID, pUser);
            }
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    }

    return true;
}

// Per IDA 0x1402147D0: RecvForceInvite
// Force 邀请响应：解析 PS_REQ_FORCE_INVITE + dwUAID + byLevel + dwForceID。
// nResult<=0：被邀人线程 lambda220（IsMatchingDate/GetTradeState/安全区检查
// 三重拒绝路径或 (0x2E,1) 邀请包 + ST_LOG_GAME(23,6)）；
// nResult>0：邀请人线程 lambda59（错误通知）。所有路径返回 0。
bool CCommunitySocket::RecvForceInvite(XPacket* xPacket) {
    PS_REQ_FORCE_INVITE stInvite;
    std::uint32_t dwUAID = 0;
    std::uint8_t byLevel = 0;
    std::uint32_t dwForceID = 0;

    *xPacket >> stInvite;
    xPacket->XParse >> dwUAID;
    xPacket->XParse >> byLevel;
    xPacket->XParse >> dwForceID;

    XGameServer* pServer = XGameServer::Instance();

    if (stInvite.nResult <= 0) {
        CUser* pInviteActorID = pServer ? pServer->FindActorIDToUser(stInvite.dwInviteActorID) : nullptr;

        if (pInviteActorID) {
            if (!pInviteActorID->GetArea()) {
                return false;
            }

            pInviteActorID->IncrementJobCount();

            // Per IDA lambda220 (0x140214EA0): 被邀人线程处理邀请
            std::function<void()> func = [pInviteActorID, stInvite, dwUAID, byLevel, dwForceID]() {
                if (!pInviteActorID || !pInviteActorID->IsLive()) {
                    return;
                }

                CGocForce* pGocForce = pInviteActorID->GetGOC<CGocForce>();
                CGocInventory* pInventory = pInviteActorID->GetGOC<CGocInventory>();

                // Per IDA: 匹配中或交易中 -> (0xFA,0xD) 拒绝 53131
                if ((pGocForce && pGocForce->IsMatchingDate())
                    || (pInventory && pInventory->GetTradeState())) {
                    PS_FORCE_REJECT stReject = {};
                    stReject.dwRejectID = pInviteActorID->GetActorID().dwActorID;
                    stReject.dwReqActor = stInvite.dwReqActorID;
                    const std::wstring strName = pInviteActorID->GetName();
                    std::size_t nIndex = 0;
                    for (wchar_t ch : strName) {
                        if (nIndex >= 20) {
                            break;
                        }
                        stReject.strRejectName[nIndex++] = ch;
                    }
                    stReject.dwErrorID = 53131;
                    XSendPacket xSendPacket(0xFA, 0xD);
                    xSendPacket << stReject;
                    XGameServer::Instance()->GetCommunitySocket().SendCheck(&xSendPacket);
                    return;
                }

                XArea* pArea = pInviteActorID->GetArea();
                if (pArea) {
                    // Per IDA: 世界类型 2（战场）需要安全区检查
                    if (pArea->GetWorldType() == 2) {
                        PS_FORCE_REJECT stReject = {};
                        stReject.dwRejectID = pInviteActorID->GetActorID().dwActorID;
                        stReject.dwReqActor = stInvite.dwReqActorID;
                        const std::wstring strName = pInviteActorID->GetName();
                        std::size_t nIndex = 0;
                        for (wchar_t ch : strName) {
                            if (nIndex >= 20) {
                                break;
                            }
                            stReject.strRejectName[nIndex++] = ch;
                        }
                        stReject.dwErrorID = 53119;

                        CBattleZone* pBattleZone = dynamic_cast<CBattleZone*>(pArea);
                        if (pBattleZone && pBattleZone->IsInSafetyZone(pInviteActorID)) {
                            // Per IDA: 安全区 -> 放行邀请
                            PS_REQ_FORCE_INVITE stNewInvite = stInvite;
                            XSendPacket xSendPacket(0x2E, 1);
                            xSendPacket << stNewInvite;
                            CGocNetwork::Send(pInviteActorID, xSendPacket);

                            ST_LOG_GAME stLog = {};
                            stLog._sMainType = 23;
                            stLog._sSubType = 6;
                            stLog._nUAID = static_cast<int>(dwUAID);
                            stLog._nUCID = static_cast<int>(stNewInvite.dwReqActorID);
                            stLog.nParam0 = static_cast<int>(byLevel);
                            stLog.nParam1 = static_cast<int>(pInviteActorID->GetActorID().dwActorID);
                            stLog.nParam2 = static_cast<int>(pInviteActorID->GetLevel());
                            stLog.nParam6 = static_cast<int>(dwForceID);
                            XGameServer::Instance()->SendDBLog(stLog);
                        } else {
                            XSendPacket xSendPacket(0xFA, 0xD);
                            xSendPacket << stReject;
                            XGameServer::Instance()->GetCommunitySocket().SendCheck(&xSendPacket);
                        }
                        return;
                    }

                    // Per IDA: 非战场直接放行邀请
                    PS_REQ_FORCE_INVITE stNewInvite = stInvite;
                    XSendPacket xSendPacket(0x2E, 1);
                    xSendPacket << stNewInvite;
                    CGocNetwork::Send(pInviteActorID, xSendPacket);

                    ST_LOG_GAME stLog = {};
                    stLog._sMainType = 23;
                    stLog._sSubType = 6;
                    stLog._nUAID = static_cast<int>(dwUAID);
                    stLog._nUCID = static_cast<int>(stNewInvite.dwReqActorID);
                    stLog.nParam0 = static_cast<int>(byLevel);
                    stLog.nParam1 = static_cast<int>(pInviteActorID->GetActorID().dwActorID);
                    stLog.nParam2 = static_cast<int>(pInviteActorID->GetLevel());
                    stLog.nParam6 = static_cast<int>(dwForceID);
                    XGameServer::Instance()->SendDBLog(stLog);
                } else {
                    // Per IDA: 无 Area -> (0xFA,0xD) 拒绝 53111
                    PS_FORCE_REJECT stReject = {};
                    stReject.dwRejectID = pInviteActorID->GetActorID().dwActorID;
                    stReject.dwReqActor = stInvite.dwReqActorID;
                    const std::wstring strName = pInviteActorID->GetName();
                    std::size_t nIndex = 0;
                    for (wchar_t ch : strName) {
                        if (nIndex >= 20) {
                            break;
                        }
                        stReject.strRejectName[nIndex++] = ch;
                    }
                    stReject.dwErrorID = 53111;
                    XSendPacket xSendPacket(0xFA, 0xD);
                    xSendPacket << stReject;
                    XGameServer::Instance()->GetCommunitySocket().SendCheck(&xSendPacket);
                }
            };
            CLogicThreadManager::Instance().DoJob(pInviteActorID->GetMapInsID().nMapID, func);

            std::function<void()> funcDec = [pInviteActorID]() {
                pInviteActorID->DecrementJobCount();
            };
            CLogicThreadManager::Instance().DoJob(pInviteActorID->GetMapInsID().nMapID, funcDec);
        }
    } else {
        CUser* pReqUser = pServer ? pServer->FindActorIDToUser(stInvite.dwReqActorID) : nullptr;

        if (pReqUser) {
            if (!pReqUser->GetArea()) {
                return false;
            }

            pReqUser->IncrementJobCount();

            // Per IDA lambda59 (0x1401FE760): 邀请人线程接收失败通知
            std::function<void()> func = [pReqUser, stInvite]() {
                if (!pReqUser || !pReqUser->IsLive()) {
                    return;
                }
                CGocNetwork::SendErrorMessage(pReqUser, 0x2E, 2,
                    static_cast<std::uint16_t>(stInvite.nResult));
            };
            CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, func);

            std::function<void()> funcDec = [pReqUser]() {
                pReqUser->DecrementJobCount();
            };
            CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, funcDec);
        }
    }

    return false;
}

// Per IDA 0x1401FCF70: ForceProcess
// Force 包主分发：subcmd 路由到各 Recv 处理器，未知返回 1。
bool CCommunitySocket::ForceProcess(XPacket* xPacket) {
    switch (xPacket->GetSubCmd()) {
        case 0x01: return RecvForceCreate(xPacket);
        case 0x02: return RecvForceJoinMember(xPacket);
        case 0x03: return RecvForceLeaveMember(xPacket);
        case 0x04: return RecvForceChangeMaster(xPacket);
        case 0x05: return RecvUpdateForceMember(xPacket);
        case 0x06: return RecvForceDelete(xPacket);
        case 0x08: return RecvForceEnterMaze(xPacket);
        case 0x09: return RecvForceUpdateInfo(xPacket);
        case 0x0A: return RecvForceEnterServer(xPacket);
        case 0x0B: return RecvForceInvite(xPacket);
        case 0x0C: return RecvForceAccept(xPacket);
        case 0x0D: return RecvForceReject(xPacket);
        case 0x10: return RecvForceMessage(xPacket);
        case 0x13: return RecvForceMatchingEnter(xPacket);
        case 0x14: return RecvForceMatchingExit(xPacket);
        case 0x15: return RecvForceMatchingCheck(xPacket);
        case 0x16: return RecvForceMatchingReset(xPacket);
        case 0x17: return RecvForceMatchingWait(xPacket);
        case 0x18: return RecvForceMatchingMaze(xPacket);
        case 0x19: return RecvForceMazeClear(xPacket);
        case 0x1A: return RecvForceInfo(xPacket);
        case 0x20: return RecvForceNameChange(xPacket);
        default: return true;
    }
}

// ============================================================================
// 以下 Force 处理器为待还原占位（func-index 状态 blocked）
// TODO: 需人工审查 - 各函数在下一批次从 IDA 反编译精确还原
// ============================================================================

// Per IDA 0x140213570: RecvUpdateForceMember
// Force 成员信息更新：解析 ST_UPDATE_FORCE_MEMBER -> lambda206 广播到
// 所有线程（XForceManager::UpdateMemberInfo）。
bool CCommunitySocket::RecvUpdateForceMember(XPacket* xPacket) {
    ST_UPDATE_FORCE_MEMBER stForceMember;
    *xPacket >> stForceMember;

    // Per IDA lambda206 (0x140213660): 所有线程更新成员信息
    std::function<void()> func = [stForceMember]() {
        ST_UPDATE_FORCE_MEMBER stUpdate = stForceMember;
        ThreadLocalData::GetInstance()->GetForceMgr()->UpdateMemberInfo(stUpdate);
    };
    CLogicThreadManager::Instance().DoJobAllThread(func);

    return true;
}

// Per IDA 0x1402157D0: RecvForceAccept
// Force 邀请接受响应：解析 PS_RES_FORCE_ACCEPT -> 按 dwAcceptID 找人 ->
// lambda222 DoJob（(0x2E,2) 接受结果包）+ lambda192 递减。返回 1。
bool CCommunitySocket::RecvForceAccept(XPacket* xPacket) {
    PS_RES_FORCE_ACCEPT stForceAccept;
    *xPacket >> stForceAccept;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pReqUser = pServer ? pServer->FindActorIDToUser(stForceAccept.dwAcceptID) : nullptr;

    if (pReqUser) {
        if (!pReqUser->GetArea()) {
            return false;
        }

        pReqUser->IncrementJobCount();

        // Per IDA lambda222 (0x1402159F0): 玩家线程发送接受结果
        std::function<void()> func = [pReqUser, stForceAccept]() {
            if (!pReqUser || !pReqUser->IsLive()) {
                return;
            }
            PS_RES_FORCE_ACCEPT stNewForceAccept = stForceAccept;
            XSendPacket xSendPacket(0x2E, 2);
            xSendPacket << stNewForceAccept;
            CGocNetwork::Send(pReqUser, xSendPacket);
        };
        CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, func);

        std::function<void()> funcDec = [pReqUser]() {
            pReqUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, funcDec);
    }

    return true;
}

// Per IDA 0x140215B00: RecvForceReject
// Force 邀请拒绝响应：解析 PS_FORCE_REJECT -> 按 dwReqActor 找人 ->
// lambda224 DoJob（(0x2E,8) 拒绝包）+ lambda192 递减。找不到人直接返回 0。
bool CCommunitySocket::RecvForceReject(XPacket* xPacket) {
    PS_FORCE_REJECT stReject;
    *xPacket >> stReject;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pReqUser = pServer ? pServer->FindActorIDToUser(stReject.dwReqActor) : nullptr;

    if (!pReqUser) {
        return false;
    }

    if (!pReqUser->GetArea()) {
        return false;
    }

    pReqUser->IncrementJobCount();

    // Per IDA lambda224 (0x140215D60): 玩家线程发送拒绝结果
    std::function<void()> func = [pReqUser, stReject]() {
        if (!pReqUser || !pReqUser->IsLive()) {
            return;
        }
        PS_FORCE_REJECT stNewReject = stReject;
        XSendPacket xSendPacket(0x2E, 8);
        xSendPacket << stNewReject;
        CGocNetwork::Send(pReqUser, xSendPacket);
    };
    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, func);

    std::function<void()> funcDec = [pReqUser]() {
        pReqUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// Per IDA 0x140215EA0: RecvForceMessage
// Force 聊天消息：解析 PS_CHAT_FORCE + PS_CHAT_ITEM_LINK_FOR_SERVER ->
// lambda226 广播到所有线程（GetForce(dwForceID) 存在则 (7,1) 聊天包
// 附带消息与物品链接转发给全 Force）。返回 1。
bool CCommunitySocket::RecvForceMessage(XPacket* xPacket) {
    PS_CHAT_FORCE stChatForce;
    PS_CHAT_ITEM_LINK_FOR_SERVER psChatItemLinkInfo;

    *xPacket >> stChatForce;
    *xPacket >> psChatItemLinkInfo;

    // Per IDA lambda226 (0x140215FF0): 所有线程向 Force 成员转发聊天
    std::function<void()> func = [stChatForce, psChatItemLinkInfo]() {
        std::shared_ptr<CForce> pForce =
            ThreadLocalData::GetInstance()->GetForceMgr()->GetForce(stChatForce.dwPartyID);
        if (!pForce) {
            return;
        }

        XSendPacket xSendPacket(7, 1);
        xSendPacket.XParse << stChatForce.dwActorID;
        xSendPacket.XParse << 3;
        xSendPacket.XParse << GreenDamTan_BoundedWideString(stChatForce.szMsg);
        xSendPacket.XParse << psChatItemLinkInfo.byItemLinkCount;
        for (int i = 0; i < psChatItemLinkInfo.byItemLinkCount; ++i) {
            PS_CHAT_ITEM_LINK psInfo = psChatItemLinkInfo.psItemLinkInfo[i];
            xSendPacket << psInfo;
        }
        pForce->Send(xSendPacket, 0);
    };
    CLogicThreadManager::Instance().DoJobAllThread(func);

    return true;
}

// Per IDA 0x140216240: RecvForceMatchingEnter
// Force 匹配进入响应：解析 dwActorID + byResult + dwEnterActorID +
// ST_FORCE_MATCHING_INFO -> lambda227 DoJob（byResult==0 错误 53131；
// ==100 错误 53161；其余时若 dwEnterActorID 是本人则记录匹配时间并清状态，
// 随后 (0x2E,0x30) 匹配信息包）+ lambda192 递减。
bool CCommunitySocket::RecvForceMatchingEnter(XPacket* xPacket) {
    std::uint32_t dwActorID = 0;
    std::uint8_t byResult = 0;
    std::uint32_t dwEnterActorID = 0;
    ST_FORCE_MATCHING_INFO stMatchingInfo;

    xPacket->XParse >> dwActorID;
    xPacket->XParse >> byResult;
    xPacket->XParse >> dwEnterActorID;
    *xPacket >> stMatchingInfo;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (!pUser) {
        return true;
    }

    if (!pUser->GetArea()) {
        return false;
    }

    pUser->IncrementJobCount();

    // Per IDA lambda227 (0x1402165A0): 玩家线程处理匹配结果
    std::function<void()> func = [pUser, byResult, dwEnterActorID, stMatchingInfo]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }
        if (byResult == 0) {
            pUser->SendErrorMessage(0x2E, 0x30, 53131);
            return;
        }
        if (byResult == 100) {
            pUser->SendErrorMessage(0x2E, 0x30, 53161);
            return;
        }

        // Per IDA: 进入者即本人则记录匹配时间并清除匹配状态
        if (pUser->GetActorID() == UXActorID(dwEnterActorID)) {
            CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
            if (pGocForce) {
                const std::int64_t biDate = XGameServer::Instance()->GetCurDate();
                pGocForce->SetMatchingDate(biDate);
                pGocForce->SetMatchingState(0);
            }
        }

        ST_FORCE_MATCHING_INFO st = stMatchingInfo;
        XSendPacket xSendPacket(0x2E, 0x30);
        xSendPacket << st;
        CGocNetwork::Send(pUser, xSendPacket);
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// Per IDA 0x140216850: RecvForceMatchingExit
// Force 匹配退出响应：解析 dwActorID + dwExitActorID + byReason ->
// lambda229 DoJob（退出者本人或 0 时清匹配时间/状态；(0x2E,0x31) 退出通知）
// + lambda192 递减。
bool CCommunitySocket::RecvForceMatchingExit(XPacket* xPacket) {
    std::uint32_t dwActorID = 0;
    std::uint32_t dwExitActorID = 0;
    std::uint8_t byReason = 0;

    xPacket->XParse >> dwActorID;
    xPacket->XParse >> dwExitActorID;
    xPacket->XParse >> byReason;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (!pUser) {
        return true;
    }

    if (!pUser->GetArea()) {
        return false;
    }

    pUser->IncrementJobCount();

    // Per IDA lambda229 (0x140216AB0): 玩家线程处理匹配退出
    std::function<void()> func = [pUser, dwExitActorID, byReason]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }

        // Per IDA: 退出者本人或全队退出（0）时清除匹配状态
        if (pUser->GetActorID() == UXActorID(dwExitActorID) || dwExitActorID == 0) {
            CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
            if (pGocForce) {
                pGocForce->SetMatchingDate(0);
                pGocForce->SetMatchingState(0);
            }
        }

        XSendPacket xSendPacket(0x2E, 0x31);
        xSendPacket.XParse << dwExitActorID;
        xSendPacket.XParse << byReason;
        CGocNetwork::Send(pUser, xSendPacket);
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// Per IDA 0x140216CC0: RecvForceMatchingCheck
// Force 匹配检查：解析 dwActorID + dwMazeID -> lambda231 DoJob
// （XForceProcess::CheckForceMatchingEnterUser 前置校验；失败按错误码分支
// (0x2E,5) SendErrorMessage 55035 带物品 ID / 其他错误码，(0xFA,0x15) 检查回包 +
// (0xFA,0x14) 取消包回 RelayServer；成功 (0x2E,0x32) 空包给客户端）+
// lambda192 递减。
bool CCommunitySocket::RecvForceMatchingCheck(XPacket* xPacket) {
    std::uint32_t dwActorID = 0;
    std::uint32_t dwMazeID = 0;

    xPacket->XParse >> dwActorID;
    xPacket->XParse >> dwMazeID;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (!pUser) {
        return true;
    }

    if (!pUser->GetArea()) {
        return false;
    }

    pUser->IncrementJobCount();

    // Per IDA lambda231 (0x140216F10): 玩家线程校验匹配进入条件
    std::function<void()> func = [pUser, dwMazeID]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }

        int nError = 0;
        int nNeedItemID = 0;

        // Per IDA: XClient::GetProcessPtr<XForceProcess>(0x2E) 后
        // CheckForceMatchingEnterUser(pUser, dwMazeID, &nError, &nNeedItemID)
        XForceProcess* pProcess = pUser->GetProcessPtr<XForceProcess>(0x2E);
        if (pProcess) {
            pProcess->CheckForceMatchingEnterUser(pUser, static_cast<std::uint16_t>(dwMazeID),
                                                   nError, &nNeedItemID);
        }

        if (nError) {
            if (nError == 55035) {
                pUser->SendErrorMessage(0x2E, 5, 55035, nNeedItemID);
            } else {
                pUser->SendErrorMessage(0x2E, 5, static_cast<std::uint16_t>(nError));
            }

            // Per IDA: (0xFA,0x15) 检查失败回包
            PS_SERVER_FORCE_MATCHING_CHECK psCheck = {};
            psCheck.dwUAID = pUser->GetUAID();
            psCheck.dwUCID = pUser->GetActorID().dwActorID;
            psCheck.byCheck = 0;
            psCheck.nError = nError;
            {
                XSendPacket xSendPacket(0xFA, 0x15);
                xSendPacket << psCheck;
                XGameServer::Instance()->GetCommunitySocket().SendCmd(
                    &xSendPacket, pUser, 0x2E, 0x32);
            }

            // Per IDA: (0xFA,0x14) 匹配取消回包（带 UCID/0/UAID/等级）
            {
                XSendPacket xSendPacket(0xFA, 0x14);
                xSendPacket.XParse << pUser->GetActorID().dwActorID;
                xSendPacket.XParse << 0;
                xSendPacket.XParse << pUser->GetUAID();
                xSendPacket.XParse << pUser->GetLevel();
                XGameServer::Instance()->GetCommunitySocket().SendCmd(
                    &xSendPacket, pUser, 0x2E, 0x31);
            }
        } else {
            // Per IDA: 成功 (0x2E,0x32) 空包
            XSendPacket packet(0x2E, 0x32);
            CGocNetwork::Send(pUser, packet);
        }
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// Per IDA 0x1402172A0: RecvForceMatchingReset
// Force 匹配重置响应：解析 dwActorID + nRemainTick + byReason + nResetCount ->
// lambda233 DoJob（(0x2E,0x33) 重置包；byReason==1 加 60 秒并报 53153；
// ==2 只加 60 秒）+ lambda192 递减。
bool CCommunitySocket::RecvForceMatchingReset(XPacket* xPacket) {
    std::uint32_t dwActorID = 0;
    int nRemainTick = 0;
    std::uint8_t byReason = 0;
    int nResetCount = 0;

    xPacket->XParse >> dwActorID;
    xPacket->XParse >> nRemainTick;
    xPacket->XParse >> byReason;
    xPacket->XParse >> nResetCount;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (!pUser) {
        return true;
    }

    if (!pUser->GetArea()) {
        return false;
    }

    pUser->IncrementJobCount();

    // Per IDA lambda233 (0x1402175A0): 玩家线程处理匹配重置
    std::function<void()> func = [pUser, nRemainTick, byReason, nResetCount]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }

        XSendPacket xSendPacket(0x2E, 0x33);
        xSendPacket.XParse << nRemainTick;
        xSendPacket.XParse << nResetCount;
        CGocNetwork::Send(pUser, xSendPacket);

        // Per IDA: byReason==1 加 60 秒并报 53153；==2 只加 60 秒
        if (byReason == 1) {
            CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
            if (pGocForce) {
                pGocForce->AddMatchingDate(60);
            }
            pUser->SendErrorMessage(0x2E, 0x33, 53153);
        } else if (byReason == 2) {
            CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
            if (pGocForce) {
                pGocForce->AddMatchingDate(60);
            }
        }
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// Per IDA 0x1402177D0: RecvForceMatchingWait
// Force 匹配等待：解析 dwActorID + dwLeaderID -> lambda235 DoJob
// （SetMatchingState(1) + (0x2E,0x34) 队长 ID 包）+ lambda192 递减。
bool CCommunitySocket::RecvForceMatchingWait(XPacket* xPacket) {
    std::uint32_t dwActorID = 0;
    std::uint32_t dwLeaderID = 0;

    xPacket->XParse >> dwActorID;
    xPacket->XParse >> dwLeaderID;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (!pUser) {
        return true;
    }

    if (!pUser->GetArea()) {
        return false;
    }

    pUser->IncrementJobCount();

    // Per IDA lambda235 (0x140217A10): 玩家线程进入匹配等待
    std::function<void()> func = [pUser, dwLeaderID]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }

        CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
        if (pGocForce) {
            pGocForce->SetMatchingState(1);
        }

        XSendPacket xSendPacket(0x2E, 0x34);
        xSendPacket.XParse << dwLeaderID;
        CGocNetwork::Send(pUser, xSendPacket);
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// Per IDA 0x140217B70: RecvForceMatchingMaze
// Force 匹配迷宫进入：解析 ST_CREATE_MAZE + PS_FORCE_INFO -> 按用户找人 ->
// lambda237 DoJob（byGroupType==2 时 GetForce/AddForce 分支：新 Force 则
// SetMazeID + SendForceInfo + lambda239 全线程广播；已有 Force 则绑定
// CGocForce::SetForce + SendForceInfo + 清匹配时间；随后 DBSyncQuestCondition、
// GetPortalPos 门控、(3,0x42)+(0xF0,0x12) DB 双包、ST_LOG_GAME(5,4)）+
// lambda192 递减。
bool CCommunitySocket::RecvForceMatchingMaze(XPacket* xPacket) {
    ST_CREATE_MAZE stCreateMaze;
    PS_FORCE_INFO stForceInfo;

    *xPacket >> stCreateMaze;
    *xPacket >> stForceInfo;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stCreateMaze.dwUserID) : nullptr;

    if (pUser) {
        if (!pUser->GetArea()) {
            return false;
        }

        pUser->IncrementJobCount();

        // Per IDA lambda237 (0x140217E60): 玩家线程执行匹配迷宫进入
        std::function<void()> func = [pUser, stCreateMaze, stForceInfo]() {
            if (!pUser || !pUser->IsLive()) {
                return;
            }

            PS_FORCE_INFO stSendForceInfo = stForceInfo;

            // Per IDA: 以 ST_CREATE_MAZE 的 ST_MAP_INFO 基础构造 PS_ENTER_MAP_RES
            PS_ENTER_MAP_RES stEnterMapRes = {};
            static_cast<ST_MAP_INFO&>(stEnterMapRes) = static_cast<const ST_MAP_INFO&>(stCreateMaze);
            stEnterMapRes.nResult = 0;
            stEnterMapRes.bChangeServer = false;
            if (stCreateMaze.dwServerID != XGameServer::Instance()->GetOption().GetServerID()) {
                stEnterMapRes.bChangeServer = true;
            }

            // Per IDA: byGroupType==2 -> Force 绑定/重建分支
            if (stCreateMaze.stPartyInfo.byGroupType == 2) {
                std::shared_ptr<CForce> pForce =
                    ThreadLocalData::GetInstance()->GetForceMgr()->GetForce(
                        static_cast<std::uint32_t>(stCreateMaze.stPartyInfo.nID));
                if (!pForce) {
                    // Per IDA: 本线程无实例 -> AddForce + SetMazeID + 广播
                    PS_FORCE_INFO stInfo = stSendForceInfo;
                    pForce = ThreadLocalData::GetInstance()->GetForceMgr()->AddForce(stInfo);
                    if (!pForce) {
                        return;
                    }
                    pForce->SetMazeID(stSendForceInfo.uxMazeID);
                    pForce->SendForceInfo(pUser, 1);

                    // Per IDA lambda239 (0x1402186B0): 全线程同步 Force 数据
                    std::function<void()> funcAdd = [stInfo]() {
                        PS_FORCE_INFO stCopy = stInfo;
                        ThreadLocalData::GetInstance()->GetForceMgr()->AddForce(stCopy);
                    };
                    CLogicThreadManager::Instance().DoJobAllThread(funcAdd);
                } else {
                    // Per IDA: 已有实例 -> 绑定成员 CGocForce 并清匹配时间
                    CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
                    if (pGocForce) {
                        pGocForce->SetForce(pForce);
                    }
                    pForce->SendForceInfo(pUser, 1);
                    if (pGocForce) {
                        pGocForce->SetMatchingDate(0);
                    }
                }
            }

            ST_LOG_GAME stLog = {};
            stLog._nUAID = static_cast<int>(pUser->GetUAID());
            stLog._nUCID = static_cast<int>(pUser->GetActorID().dwActorID);
            stLog._sMainType = 5;
            stLog._sSubType = 4;
            stLog.nParam0 = static_cast<int>(
                (static_cast<std::uint64_t>(stForceInfo.uxMazeID.nMapID) >> 16) >> 16);
            stLog.nParam1 = static_cast<int>(stForceInfo.dwMaster);
            stLog.nParam3 = static_cast<int>(stCreateMaze.vecEnterMember.size());
            stLog.nParam4 = stCreateMaze.stPartyInfo.nID;
            stLog.nParam5 = pUser->GetLevel();
            stLog.nParam6 = stCreateMaze.uxMapID.nMapID;
            std::wcscpy(stLog.szComment, L"메이즈 입장");
            XGameServer::Instance()->SendDBLog(stLog);

            {
                CGocQuest* pQuest = pUser->GetGOC<CGocQuest>();
                if (pQuest) {
                    pQuest->DBSyncQuestCondition();
                }
            }

            stEnterMapRes.byChangeType = 0;
            const int nMapID = static_cast<int>(
                (stEnterMapRes.uxMapID.nMapID << 16) >> 48);
            if (XGameServer::Instance()->GetWorldResMgr().GetPortalPos(
                    nMapID, stEnterMapRes.nJumpID, &stEnterMapRes.stPosInfo)) {
                {
                    int nPrevMapID = 0;
                    XSendDBPacket xSendDBPacket(static_cast<XActor*>(pUser), 3, 0x42);
                    xSendDBPacket << stEnterMapRes;
                    xSendDBPacket.XParse << nPrevMapID;
                    xSendDBPacket.XParse << 0;
                    XGameServer::Instance()->SendDBGame(xSendDBPacket);
                }

                {
                    ST_STATISTICS_MAP_SAVE stInfo = {};
                    stInfo.dwUCID = stEnterMapRes.dwUserID;
                    stInfo.dwMapID = static_cast<int>(
                        (static_cast<std::uint64_t>(stEnterMapRes.uxMapID.nMapID) >> 16) >> 16);
                    stInfo.dwServerID = XGameServer::Instance()->GetOption().GetServerID();
                    XSendDBPacket xSendDBStatistics(static_cast<XActor*>(pUser), 0xF0, 0x12);
                    xSendDBStatistics << stInfo;
                    XGameServer::Instance()->SendDBStatistics(xSendDBStatistics);
                }
            } else {
                pUser->ClearState(eStateChangeWorld);
                LogHelper::LogError("game.contents",
                    "RecvForceMatchingMaze error - No have World Data[ ActorID:%d, MapID:%d, JumpID:%d ] ( %d )",
                    stEnterMapRes.dwUserID,
                    static_cast<int>((static_cast<std::uint64_t>(stEnterMapRes.uxMapID.nMapID) >> 16) >> 16),
                    stEnterMapRes.nJumpID, 4695);
            }
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    }

    return true;
}

// Per IDA 0x140218700: RecvForceMazeClear
// Force 迷宫通关：解析 dwForceID -> lambda240 广播到所有线程
// （XForceManager::RecvForceMazeClear）。
bool CCommunitySocket::RecvForceMazeClear(XPacket* xPacket) {
    std::uint32_t dwForceID = 0;
    xPacket->XParse >> dwForceID;

    // Per IDA lambda240 (0x140218780): 所有线程处理迷宫通关
    std::function<void()> func = [dwForceID]() {
        ThreadLocalData::GetInstance()->GetForceMgr()->RecvForceMazeClear(dwForceID);
    };
    CLogicThreadManager::Instance().DoJobAllThread(func);

    return true;
}

// Per IDA 0x1402187B0: RecvForceInfo
// Force 信息同步：解析 dwActorID + PS_FORCE_INFO -> lambda241 广播到
// 所有线程（XForceManager::AddForce）。dwActorID 仅入捕获不参与逻辑。
bool CCommunitySocket::RecvForceInfo(XPacket* xPacket) {
    std::uint32_t dwActorID = 0;
    PS_FORCE_INFO stForceInfo;

    xPacket->XParse >> dwActorID;
    *xPacket >> stForceInfo;

    // Per IDA lambda241 (0x1402188D0): 所有线程重建 Force 数据
    std::function<void()> func = [stForceInfo]() {
        PS_FORCE_INFO stNewInfo = stForceInfo;
        ThreadLocalData::GetInstance()->GetForceMgr()->AddForce(stNewInfo);
    };
    CLogicThreadManager::Instance().DoJobAllThread(func);

    return true;
}

// Per IDA 0x1402189A0: RecvForceNameChange
// Force 改名同步：解析 dwUCID + PS_CHANGE_NAME -> 按用户找人 ->
// lambda242 DoJob（GetGOC<CGocForce> 存在时 ChangePartyMemberName +
// (8,0x53) 改名包）+ lambda192 递减。找不到人返回 0。
bool CCommunitySocket::RecvForceNameChange(XPacket* xPacket) {
    std::uint32_t dwUCID = 0;
    PS_CHANGE_NAME stInfo;

    xPacket->XParse >> dwUCID;
    *xPacket >> stInfo;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwUCID) : nullptr;

    if (!pUser) {
        return false;
    }

    if (!pUser->GetArea()) {
        return false;
    }

    pUser->IncrementJobCount();

    // Per IDA lambda242 (0x140218C20): 玩家线程同步改名
    std::function<void()> func = [pUser, stInfo]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }

        CGocForce* pGocForce = pUser->GetGOC<CGocForce>();
        if (!pGocForce) {
            return;
        }

        PS_CHANGE_NAME psChangeName = stInfo;
        pGocForce->ChangePartyMemberName(psChangeName);

        PS_CHANGE_NAME st = stInfo;
        XSendPacket xSendPacket(8, 0x53);
        xSendPacket << st;
        CGocNetwork::Send(pUser, xSendPacket);
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// Per IDA 0x140203130: RecvForceMatching
// Force 匹配成功：解析 ST_CREATE_MAZE + PS_FORCE_INFO + dwMatchingID ->
// lambda97/98 包装后按 stCreateMaze.uxMapID（目标地图线程）DoJob
// （ThreadLocalData::CreateMatchingMaze 建立匹配迷宫）。
bool CCommunitySocket::RecvForceMatching(XPacket* xPacket) {
    ST_CREATE_MAZE stCreateMaze;
    PS_FORCE_INFO stForceInfo;
    std::uint32_t dwMatchingID = 0;

    *xPacket >> stCreateMaze;
    *xPacket >> stForceInfo;
    xPacket->XParse >> dwMatchingID;

    // Per IDA lambda97/98 (0x1402032C0): 目标地图线程建立匹配迷宫
    std::function<void()> func = [stCreateMaze, stForceInfo, dwMatchingID]() {
        ST_CREATE_MAZE stMaze = stCreateMaze;
        PS_FORCE_INFO stInfo = stForceInfo;
        // Per IDA: ThreadLocalData::GetInstance()->CreateMatchingMaze(stMaze, stInfo, dwMatchingID)
        // TODO: 需人工审查 - CreateMatchingMaze (0x1406D2960) 依赖 m_xMazePool/XMaze::Create
        // 的完整 ThreadLocalData 0x1450 布局（按计划为独立批次，当前活动层为 ThreadLocalData_Stub）；
        // 依赖落地后接入。活动边界内 stub 实现已记录于 ThreadLocalData_Stub.cpp。
        ThreadLocalData::GetInstance()->CreateMatchingMaze(stMaze, stInfo, dwMatchingID);
    };
    CLogicThreadManager::Instance().DoJob(stCreateMaze.uxMapID.nMapID, func);

    return true;
}

// ============================================================================
// League implementations
// ============================================================================

// ============================================================================
// RecvCreateLeague (0x1401FC4C0) - 已精确还原
// 解析 ST_LEAGUE_INFO + ST_LEAGUE_MEMBER_EX + dwActorID + ST_LEAGUE_INFO_EX +
// ST_LEAGUE_INFO_FOR_GAME；FindActorIDToUser 失败仅 LogError 返回 1。
// lambda47 (0x1401FC960) 在玩家线程执行：
// SetLeagueInfo -> AddMoney(-100000, 0x31, nLeagueID, 0, 0) ->
// (0x22,1) ST_LEAGUE_INFO + ST_LEAGUE_MEMBER_EX 发送 ->
// (0x22,0x44) ST_LEAGUE_INFO_EX{dwUCID=自己 actorID&0x1FFFFFFF} 广播 ->
// ThreadLocalData::AddLeagueMember -> ST_LOG_GAME(15,1) SendDBLog ->
// CGocClassEvent 8 号事件通知。
// ============================================================================
bool CCommunitySocket::RecvCreateLeague(XPacket* xPacket) {
    ST_LEAGUE_INFO stLeagueInfo;
    ST_LEAGUE_MEMBER_EX stMaster;
    ST_LEAGUE_INFO_EX stInfoEx;
    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame;

    *xPacket >> stLeagueInfo;
    *xPacket >> stMaster;
    std::uint32_t dwActorID = 0;
    xPacket->XParse >> dwActorID;
    *xPacket >> stInfoEx;
    *xPacket >> stLeagueInfoForGame;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (!pUser) {
        LogHelper::LogError("game.contents",
            "[LEAUGE] CCommunitySocket::RecvCreateLeague - No pUser : %d [%02x][%02x]",
            dwActorID, xPacket->GetMainCmd(), xPacket->GetSubCmd());
        return true;
    }

    // Per IDA lambda47 (0x1401FC960): 玩家线程执行创建 League 逻辑
    std::function<void()> func = [pUser, stLeagueInfo, stMaster, stInfoEx, stLeagueInfoForGame]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea()) {
            return;
        }

        // Per IDA: 写入用户 League 信息
        ST_LEAGUE_INFO_EX stCopyInfoEx = stInfoEx;
        pUser->SetLeagueInfo(stCopyInfoEx, stLeagueInfoForGame);

        // Per IDA: 创建公会扣除 100000 金钱
        CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
        if (pInven) {
            pInven->AddMoney(-100000, 0x31, stLeagueInfo.nLeagueID, 0, false);
        }

        // Per IDA: (0x22,1) 发送 League 信息与队长数据
        XSendPacket xSendPacket(0x22, 1);
        xSendPacket << stLeagueInfo;
        xSendPacket << stMaster;
        CGocNetwork::Send(pUser, xSendPacket);

        // Per IDA: (0x22,0x44) 广播用户 League 摘要
        ST_LEAGUE_INFO_EX stInfoExForBroadcast = stInfoEx;
        stInfoExForBroadcast.dwUCID = pUser->GetActorID().dwActorID & 0x1FFFFFFF;
        XSendPacket xSendPacket2(0x22, 0x44);
        xSendPacket2 << stInfoExForBroadcast;
        CGocNetwork::BroadcastNearby(pUser, nullptr, xSendPacket2);

        // Per IDA: 注册到本线程 League 成员表
        ThreadLocalData::GetInstance()->AddLeagueMember(pUser);

        // Per IDA: 记录创建日志 (MainType=15, SubType=1)
        ST_LOG_GAME stLog;
        stLog._nUAID = static_cast<int>(pUser->GetUAID());
        stLog._nUCID = static_cast<int>(pUser->GetActorID().dwActorID);
        stLog._sMainType = 15;
        stLog._sSubType = 1;
        stLog.nParam0 = stLeagueInfo.nLeagueID;
        stLog.nParam2 = static_cast<int>(pUser->stMyCharInfoEx()->biMoney);
        stLog.nParam5 = pUser->GetLevel();
        wcscpy_s(stLog.szComment, stLeagueInfo.szLeagueName);
        XGameServer::Instance()->SendDBLog(stLog);

        // Per IDA: 通知 ClassEvent 组件 8 号事件
        CGocClassEvent* pClassEvent = pUser->GetGOC<CGocClassEvent>();
        if (pClassEvent) {
            // TODO: 需人工审查 - IDA 误标为 CItem::CanBroachEquip(8,0)，
            // 按调用语义应为 CGocClassEvent 的 8 号事件通知接口，具体方法名待
            // CGocClassEvent 还原批次核实。
        }
    };

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    return true;
}

// ============================================================================
// RecvLeagueInfo (0x1401FB930) - 已精确还原
// 8 段解析序: ST_LEAGUE_INFO + ST_LEAGUE_MEMBER_LIST +
// ST_LEAGUE_APPLICANT_LIST + ST_LEAGUE_BOARD_LIST + dwActorID(int) +
// byState(u8) + ST_LEAGUE_RECORD_LIST + ST_LEAGUE_INFO_FOR_GAME。
// 按 dwActorID 查用户:
// - 找不到: LogError "[LEAUGE] ... No pUser" (保留原始拼写) 后返回 1。
// - 找到且在区域中: lambda45 (0x1401FBF30) DoJob:
//   存活检查 -> 从 stLeagueInfo 提取 nLeagueID/LeagueCard/szLeagueName
//   构造 ST_LEAGUE_INFO_EX -> SetLeagueInfo -> (0x22,7) 依次发送
//   stInfo/member/applicant/board/record/byState；
//   随后 lambda192 DoJob 递减计数。
// - 找到但不在区域中: 返回 0。
// 返回 1。
// ============================================================================
bool CCommunitySocket::RecvLeagueInfo(XPacket* xPacket) {
    ST_LEAGUE_INFO stLeagueInfo;
    ST_LEAGUE_MEMBER_LIST stMemberList;
    ST_LEAGUE_APPLICANT_LIST stApplicant;
    ST_LEAGUE_BOARD_LIST stBoard;
    int dwActorID = 0;
    bool byState = false;
    ST_LEAGUE_RECORD_LIST stRecordList;
    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame;

    *xPacket >> stLeagueInfo;
    *xPacket >> stMemberList;
    *xPacket >> stApplicant;
    *xPacket >> stBoard;
    xPacket->XParse >> dwActorID;
    xPacket->XParse >> byState;
    *xPacket >> stRecordList;
    *xPacket >> stLeagueInfoForGame;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(
        static_cast<std::uint32_t>(dwActorID)) : nullptr;

    if (!pUser) {
        LogHelper::LogError("game.contents",
            "[LEAUGE] CCommunitySocket::RecvLeagueInfo - No pUser : %d [%02x][%02x]",
            dwActorID, xPacket->GetMainCmd(), xPacket->GetSubCmd());
        return true;
    }

    // Per IDA lambda45 (0x1401FBF30): League 信息下发链
    std::function<void()> func = [pUser, stLeagueInfo, stMemberList, stApplicant,
                                  stBoard, byState, stRecordList,
                                  stLeagueInfoForGame]() {
        if (!pUser || !pUser->IsLive() || !pUser->GetArea()) {
            return;
        }

        ST_LEAGUE_INFO_EX stInfoEx;
        stInfoEx.nLeagueID = stLeagueInfo.nLeagueID;
        stInfoEx.dwLeagueCard = stLeagueInfo.dwLeagueCard;
        wcscpy_s(stInfoEx.szLeagueName, stLeagueInfo.szLeagueName);

        ST_LEAGUE_INFO_FOR_GAME stLocalForGame = stLeagueInfoForGame;
        pUser->SetLeagueInfo(stInfoEx, stLocalForGame);

        XSendPacket xSendPacket(0x22, 7);
        xSendPacket << stLeagueInfo;
        xSendPacket << stMemberList;
        xSendPacket << stApplicant;
        xSendPacket << stBoard;
        xSendPacket << stRecordList;
        xSendPacket.XParse << byState;
        CGocNetwork::Send(pUser, xSendPacket);
    };

    if (!pUser->GetArea()) {
        return false;
    }

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    return true;
}

// ============================================================================
// RecvLeagueLogin (0x1401F51A0) - 已精确还原
// 11 段解析序: bLogin(u8) + ST_LEAGUE_MEMBER_UPDATE + ST_LEAGUE_INFO +
// ST_LEAGUE_MEMBER_LIST + ST_LEAGUE_APPLICANT_LIST + ST_LEAGUE_BOARD_LIST +
// byState(u8) + ST_LEAGUE_INFO_EX + ST_LEAGUE_RECORD_LIST +
// ST_LEAGUE_INFO_FOR_GAME + nSyncCount(int)。
// 按 stUpdate.dwActorID 查用户:
// - 找不到: 返回 0。
// - bLogin==true 且用户在区域中: lambda8 (0x1401F5920) DoJob:
//   存活检查 -> SetLeagueInfo(stInfoEx, stLeagueInfoForGame) ->
//   stInfoEx.dwUCID=actorID&0x1FFFFFFF -> UpdateLeagueSyncCount(nSyncCount) ->
//   (0x22,7) 依次发送 stInfo/member/applicant/board/record/byState ->
//   (0x22,0x44) stInfoEx 广播附近；随后 lambda192 DoJob 递减计数，
//   lambda152 DoJobAllThread 全线程 UpdateLeagueMember(stUpdate)。
// - bLogin==false: 仅 CUser::ClearLeagueInfo()。
// 返回 1。
// ============================================================================
bool CCommunitySocket::RecvLeagueLogin(XPacket* xPacket) {
    bool bLogin = false;
    ST_LEAGUE_MEMBER_UPDATE stUpdate;
    ST_LEAGUE_INFO stInfo;
    ST_LEAGUE_MEMBER_LIST stMemberList;
    ST_LEAGUE_APPLICANT_LIST stApplicantList;
    ST_LEAGUE_BOARD_LIST stBoardList;
    bool byState = false;
    ST_LEAGUE_INFO_EX stInfoEx;
    ST_LEAGUE_RECORD_LIST stRecordList;
    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame;
    int nSyncCount = 0;

    xPacket->XParse >> bLogin;
    *xPacket >> stUpdate;
    *xPacket >> stInfo;
    *xPacket >> stMemberList;
    *xPacket >> stApplicantList;
    *xPacket >> stBoardList;
    xPacket->XParse >> byState;
    *xPacket >> stInfoEx;
    *xPacket >> stRecordList;
    *xPacket >> stLeagueInfoForGame;
    xPacket->XParse >> nSyncCount;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(stUpdate.dwActorID) : nullptr;

    if (!pUser) {
        return false;
    }

    if (bLogin) {
        // Per IDA lambda8 (0x1401F5920): League 登录通知链
        std::function<void()> func = [pUser, stInfo, stMemberList, stApplicantList,
                                      stBoardList, byState, stInfoEx, stRecordList,
                                      stLeagueInfoForGame, nSyncCount]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea()) {
                return;
            }

            ST_LEAGUE_INFO_EX stLocalInfoEx = stInfoEx;
            ST_LEAGUE_INFO_FOR_GAME stLocalForGame = stLeagueInfoForGame;
            pUser->SetLeagueInfo(stLocalInfoEx, stLocalForGame);
            stLocalInfoEx.dwUCID = pUser->GetActorID().dwActorID & 0x1FFFFFFF;
            pUser->UpdateLeagueSyncCount(nSyncCount);

            XSendPacket xSendPacket(0x22, 7);
            xSendPacket << stInfo;
            xSendPacket << stMemberList;
            xSendPacket << stApplicantList;
            xSendPacket << stBoardList;
            xSendPacket << stRecordList;
            xSendPacket.XParse << byState;
            CGocNetwork::Send(pUser, xSendPacket);

            XSendPacket xSendPacket2(0x22, 0x44);
            xSendPacket2 << stLocalInfoEx;
            CGocNetwork::BroadcastNearby(pUser, nullptr, xSendPacket2);
        };

        if (!pUser->GetArea()) {
            return false;
        }

        pUser->IncrementJobCount();
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

        // Per IDA lambda152 (0x1401F4460): 全线程按 LeagueID 更新成员信息
        std::function<void()> funcAll = [stUpdate]() {
            ST_LEAGUE_MEMBER_UPDATE stCopy = stUpdate;
            ThreadLocalData::GetInstance()->UpdateLeagueMember(stCopy);
        };
        CLogicThreadManager::Instance().DoJobAllThread(funcAll);
    } else {
        pUser->ClearLeagueInfo();
    }

    return true;
}

// ============================================================================
// RecvLeagueMemberUpdate (0x1401F4320) - 已精确还原
// 解析 ST_LEAGUE_MEMBER_UPDATE；构造 lambda152 (0x1401F4460) 经
// DoJobAllThread 分发到所有逻辑线程，各线程
// ThreadLocalData::UpdateLeagueMember(stUpdate) ->
// CLeagueMember::UpdateLeagueMember 向未同步成员广播 (0x22,0x22)。
// ============================================================================
bool CCommunitySocket::RecvLeagueMemberUpdate(XPacket* xPacket) {
    ST_LEAGUE_MEMBER_UPDATE stUpdate;
    *xPacket >> stUpdate;

    // Per IDA lambda152 (0x1401F4460): 全线程按 LeagueID 更新成员信息
    std::function<void()> func = [stUpdate]() {
        ST_LEAGUE_MEMBER_UPDATE stCopy = stUpdate;
        ThreadLocalData::GetInstance()->UpdateLeagueMember(stCopy);
    };
    CLogicThreadManager::Instance().DoJobAllThread(func);
    return true;
}

// ============================================================================
// RecvLeagueNoticeChange (0x1401F44A0) - 已精确还原
// 解析 ST_LEAGUE_NOTICE + dwActorID(int)。
// 1. 若 dwActorID 用户在线: lambda1 (0x1401F47B0) 经 DoJob 发到其线程，
//    存活检查通过后 (0x22,0x36) + ST_LEAGUE_NOTICE 发给该用户；
//    随后 lambda192 (0x140427E60) DoJob 递减任务计数。
// 2. 无论用户是否在线，lambda3 (0x1401F4990) 经 DoJobAllThread 分发到
//    所有逻辑线程，各线程 ThreadLocalData::SendLeagueNoticeChangeToMember
//    (stNotice, dwActorID) -> CLeagueMember::SendLeagueNotice 向未同步且
//    非 dwActorID 本人的成员广播 (0x22,0x29)。
// ============================================================================
bool CCommunitySocket::RecvLeagueNoticeChange(XPacket* xPacket) {
    ST_LEAGUE_NOTICE stNotice;
    *xPacket >> stNotice;

    int dwActorID = 0;
    xPacket->XParse >> dwActorID;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(
        static_cast<std::uint32_t>(dwActorID)) : nullptr;

    if (pUser) {
        // Per IDA lambda1 (0x1401F47B0): 公告发起者本人通知
        std::function<void()> func = [pUser, stNotice]() {
            if (!pUser || !pUser->IsLive()) {
                return;
            }

            XSendPacket xSendPacket(0x22, 0x36);
            xSendPacket << stNotice;
            CGocNetwork::Send(pUser, xSendPacket);
        };

        pUser->IncrementJobCount();
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    }

    // Per IDA lambda3 (0x1401F4990): 全线程向 League 成员广播公告
    std::function<void()> funcAll = [stNotice, dwActorID]() {
        ST_LEAGUE_NOTICE stCopy = stNotice;
        ThreadLocalData::GetInstance()->SendLeagueNoticeChangeToMember(
            stCopy, static_cast<unsigned int>(dwActorID));
    };
    CLogicThreadManager::Instance().DoJobAllThread(funcAll);

    return true;
}

// ============================================================================
// RecvLeagueDelete (0x1401FAFB0) - 已精确还原
// 解析 dwActorID + nLeagueID + biPenalty + nErrorCode；
// nErrorCode <= 0 (成功): lambda43 (0x1401FB5B0) - SetLeagueInventorySend(0)
// -> (0x22,2){nErrorCode} 发送 -> SetLeagueDeletePenalty(biPenalty) ->
// ThreadLocalData::DeleteLeague(nLeagueID, dwActorID) -> ClearLeagueInfo ->
// GetLeagueInfo(EX) + dwUCID=actorID&0x1FFFFFFF -> (0x22,0x44) 广播 ->
// ST_LOG_GAME(15,2, L"해체") SendDBLog。
// nErrorCode > 0 (失败): lambda41 (0x1401FB410) - 仅 (0x22,2){nErrorCode} 发送。
// ============================================================================
bool CCommunitySocket::RecvLeagueDelete(XPacket* xPacket) {
    std::uint32_t dwActorID = 0;
    int nLeagueID = 0;
    std::int64_t biPenalty = 0;
    int nErrorCode = 0;

    xPacket->XParse >> dwActorID;
    xPacket->XParse >> nLeagueID;
    xPacket->XParse >> biPenalty;
    xPacket->XParse >> nErrorCode;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (!pUser) {
        return true;
    }
    if (!pUser->GetArea()) {
        return true;
    }

    if (nErrorCode <= 0) {
        // Per IDA lambda43 (0x1401FB5B0): 成功路径 - 解散 League
        std::function<void()> func = [pUser, dwActorID, nLeagueID, biPenalty, nErrorCode]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea()) {
                return;
            }

            pUser->SetLeagueInventorySend(false);

            XSendPacket xSendPacket(0x22, 2);
            xSendPacket.XParse << nErrorCode;
            CGocNetwork::Send(pUser, xSendPacket);

            pUser->SetLeagueDeletePenalty(biPenalty);
            ThreadLocalData::GetInstance()->DeleteLeague(
                static_cast<unsigned int>(nLeagueID), dwActorID);

            ST_LEAGUE_INFO_EX stInfoEx;
            pUser->ClearLeagueInfo();
            pUser->GetLeagueInfo(stInfoEx);
            stInfoEx.dwUCID = pUser->GetActorID().dwActorID & 0x1FFFFFFF;

            XSendPacket xSendPacket2(0x22, 0x44);
            xSendPacket2 << stInfoEx;
            CGocNetwork::BroadcastNearby(pUser, nullptr, xSendPacket2);

            ST_LOG_GAME stLog;
            stLog._nUAID = static_cast<int>(pUser->GetUAID());
            stLog._nUCID = static_cast<int>(pUser->GetActorID().dwActorID);
            stLog._sMainType = 15;
            stLog._sSubType = 2;
            stLog.nParam0 = nLeagueID;
            stLog.nParam5 = pUser->GetLevel();
            wcscpy_s(stLog.szComment, L"해체");
            XGameServer::Instance()->SendDBLog(stLog);
        };

        pUser->IncrementJobCount();
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    } else {
        // Per IDA lambda41 (0x1401FB410): 失败路径 - 仅发送错误码
        std::function<void()> func = [pUser, nErrorCode]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea()) {
                return;
            }

            XSendPacket xSendPacket(0x22, 2);
            xSendPacket.XParse << nErrorCode;
            CGocNetwork::Send(pUser, xSendPacket);
        };

        pUser->IncrementJobCount();
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    }
    return true;
}

// ============================================================================
// RecvLeagueDelegate - 处理公会会长转让结果 (IDA 0x14020CD00)
// 解析 dwReqUCID + dwDelegatedUCID + PS_RES_LEAGUE_DELEGATE + nSyncCount。
// 请求者在线(lambda167 0x14020D0F0 DoJob): IsLive+GetArea 门 ->
// (0x22,0x49) 发送 psDelegateRes 副本。
// nResult==0 时(lambda169 0x14020D2E0 DoJobAllThread): 若 pLocalData 非空则
// ThreadLocalData::SendLeagueDelegate(psDelegateRes 副本, dwDelegatedUCID,
// nSyncCount)。请求者不在线时跳过 lambda167，但 lambda169 仍执行。
// ============================================================================
bool CCommunitySocket::RecvLeagueDelegate(XPacket* xPacket) {
    std::uint32_t dwReqUCID = 0;
    std::uint32_t dwDelegatedUCID = 0;
    PS_RES_LEAGUE_DELEGATE psDelegateRes;
    int nSyncCount = 0;

    xPacket->XParse >> dwReqUCID;
    xPacket->XParse >> dwDelegatedUCID;
    *xPacket >> psDelegateRes;
    xPacket->XParse >> nSyncCount;

    CUser* pUser = XGameServer::Instance()->FindActorIDToUser(dwReqUCID);
    if (pUser && pUser->GetArea()) {
        // Per IDA lambda167 (0x14020D0F0)
        std::function<void()> func = [pUser, psDelegateRes]() {
            if (!pUser || !pUser->IsLive()) return;
            if (!pUser->GetArea()) return;
            PS_RES_LEAGUE_DELEGATE psRes = psDelegateRes;
            XSendPacket xSendPacket(0x22, 0x49);
            xSendPacket << psRes;
            CGocNetwork::Send(pUser, xSendPacket);
        };

        pUser->IncrementJobCount();
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);
        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    }

    if (psDelegateRes.nResult == 0) {
        // Per IDA lambda169 (0x14020D2E0) - DoJobAllThread
        std::function<void()> funcAll = [psDelegateRes, dwDelegatedUCID, nSyncCount]() {
            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                PS_RES_LEAGUE_DELEGATE psRes = psDelegateRes;
                pLocalData->SendLeagueDelegate(psRes, dwDelegatedUCID, nSyncCount);
            }
        };
        CLogicThreadManager::Instance().DoJobAllThread(funcAll);
    }
    return true;
}
bool CCommunitySocket::RecvLeagueWithDraw(XPacket*) { return true; }

// ============================================================================
// RecvLeagueMemberKick (0x1401F8B70) - 已精确还原
// 解析 nErrorCode + nLeagueID + dwReqActorID + dwTargetActorID +
// ST_LEAGUE_INFO_UPDATE + shLevel + nSyncCount。
// 请求者(lambda27 0x1401F91A0): UpdateLeagueSyncCount -> (0x22,0x28)
// {dwTargetActorID} -> (0x22,0x43) 更新 -> ST_LOG_GAME(15,10)，
// szComment=硬编码字符串，szComment2=League 名。
// 被踢者(lambda29 0x1401F94C0): SetLeagueInventorySend(0) ->
// UpdateLeagueSyncCount -> ClearLeagueInfo + GetLeagueInfo(EX) +
// dwUCID=actorID&0x1FFFFFFF -> (0x22,0x44) 广播 -> (0x22,0xF) 发送。
// 全线程(lambda31 0x1401F97A0): ThreadLocalData::SendLeagueKickout
// (nLeagueID, dwReqActorID, dwTargetActorID, stInfoUpdate, nSyncCount)。
// ============================================================================
bool CCommunitySocket::RecvLeagueMemberKick(XPacket* xPacket) {
    int nErrorCode = 0;
    int nLeagueID = 0;
    std::uint32_t dwReqActorID = 0;
    std::uint32_t dwTargetActorID = 0;
    ST_LEAGUE_INFO_UPDATE stInfoUpdate;
    std::int16_t shLevel = 0;
    int nSyncCount = 0;

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

    if (pReqUser && pReqUser->GetArea()) {
        // Per IDA lambda27 (0x1401F91A0): 请求者通知 + 踢出日志
        std::function<void()> funcReq = [pReqUser, dwTargetActorID, stInfoUpdate, shLevel, nSyncCount]() {
            if (!pReqUser || !pReqUser->IsLive() || !pReqUser->GetArea()) {
                return;
            }

            pReqUser->UpdateLeagueSyncCount(nSyncCount);

            XSendPacket xSendPacket(0x22, 0x28);
            xSendPacket.XParse << dwTargetActorID;
            CGocNetwork::Send(pReqUser, xSendPacket);

            XSendPacket xSendPacket2(0x22, 0x43);
            xSendPacket2 << stInfoUpdate;
            CGocNetwork::Send(pReqUser, xSendPacket2);

            ST_LOG_GAME stLog;
            stLog._nUAID = static_cast<int>(pReqUser->GetUAID());
            stLog._nUCID = static_cast<int>(pReqUser->GetActorID().dwActorID);
            stLog._sMainType = 15;
            stLog._sSubType = 10;
            stLog.nParam0 = stInfoUpdate.nLeagueID;
            stLog.nParam1 = static_cast<int>(dwTargetActorID);
            stLog.nParam5 = shLevel;
            wcscpy_s(stLog.szComment, L"추방");
            ST_LEAGUE_INFO_EX stLeagueInfo;
            pReqUser->GetLeagueInfo(stLeagueInfo);
            wcscpy_s(stLog.szComment2, stLeagueInfo.szLeagueName);
            XGameServer::Instance()->SendDBLog(stLog);
        };

        pReqUser->IncrementJobCount();
        CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, funcReq);
        std::function<void()> funcReqDec = [pReqUser]() {
            pReqUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, funcReqDec);
    }

    if (pKickUser && pKickUser->GetArea()) {
        // Per IDA lambda29 (0x1401F94C0): 被踢者清理
        std::function<void()> funcKick = [pKickUser, nSyncCount]() {
            if (!pKickUser || !pKickUser->IsLive() || !pKickUser->GetArea()) {
                return;
            }

            pKickUser->SetLeagueInventorySend(false);
            pKickUser->UpdateLeagueSyncCount(nSyncCount);

            ST_LEAGUE_INFO_EX stInfoEx;
            pKickUser->ClearLeagueInfo();
            pKickUser->GetLeagueInfo(stInfoEx);
            stInfoEx.dwUCID = pKickUser->GetActorID().dwActorID & 0x1FFFFFFF;

            XSendPacket xSendPacket2(0x22, 0x44);
            xSendPacket2 << stInfoEx;
            CGocNetwork::BroadcastNearby(pKickUser, nullptr, xSendPacket2);

            XSendPacket xSendPacket(0x22, 0xF);
            CGocNetwork::Send(pKickUser, xSendPacket);
        };

        pKickUser->IncrementJobCount();
        CLogicThreadManager::Instance().DoJob(pKickUser->GetMapInsID().nMapID, funcKick);
        std::function<void()> funcKickDec = [pKickUser]() {
            pKickUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pKickUser->GetMapInsID().nMapID, funcKickDec);
    }

    // Per IDA lambda31 (0x1401F97A0): 全线程广播踢出
    std::function<void()> funcAll = [nLeagueID, dwReqActorID, dwTargetActorID, stInfoUpdate, nSyncCount]() {
        ST_LEAGUE_INFO_UPDATE stUpdate = stInfoUpdate;
        ThreadLocalData::GetInstance()->SendLeagueKickout(
            static_cast<unsigned int>(nLeagueID),
            static_cast<int>(dwReqActorID),
            dwTargetActorID, stUpdate, nSyncCount);
    };
    CLogicThreadManager::Instance().DoJobAllThread(funcAll);

    return true;
}

// ============================================================================
// RecvLeagueInfoChange (0x1401F49E0) - 已精确还原
// 解析 ST_LEAGUE_INFO；lambda4 (0x1401F4AE0) 经 DoJobAllThread 分发到
// 所有逻辑线程，各线程 ThreadLocalData::LeagueInfoChange(stInfo) ->
// CLeagueMember::SendLeagueInfo 广播给本线程该 League 在线成员。
// ============================================================================
bool CCommunitySocket::RecvLeagueInfoChange(XPacket* xPacket) {
    ST_LEAGUE_INFO stInfo;
    *xPacket >> stInfo;

    // Per IDA lambda4 (0x1401F4AE0): 全线程广播 League 信息变更
    std::function<void()> func = [stInfo]() {
        ST_LEAGUE_INFO stCopy = stInfo;
        ThreadLocalData::GetInstance()->LeagueInfoChange(stCopy);
    };
    CLogicThreadManager::Instance().DoJobAllThread(func);
    return true;
}

// ============================================================================
// RecvLeagueInvite - 处理公会邀请 (IDA 0x1401FA5E0)
// 解析 ST_REQ_LEAGUE_INVITE；nResult<=0 时给请求者日志+发结果，nResult>0 时给邀请目标发结果
// lambda37: nResult==57042/57043 发错误消息 (0x22,0x13,0xDED2/0xDED3)，否则发 (0x22,0x37)+ST_REQ_LEAGUE_INVITE
// ============================================================================
bool CCommunitySocket::RecvLeagueInvite(XPacket* xPacket)
{
    ST_REQ_LEAGUE_INVITE stInvite;
    *xPacket >> stInvite;

    if (stInvite.nResult <= 0) {
        CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stInvite.dwActorID);
        CUser* pTargetUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stInvite.dwTargetActorID);
        std::uint8_t byTargetLevel = 0;
        if (pTargetUser)
            byTargetLevel = static_cast<std::uint8_t>(pTargetUser->GetLevel());
        if (pUser) {
            ST_LOG_GAME stLog;
            stLog._nUAID = static_cast<int>(pUser->GetUAID());
            stLog._nUCID = pUser->GetActorID().dwActorID;
            stLog._sMainType = 15;
            stLog._sSubType = 3;
            stLog.nParam0 = stInvite.nLeagueID;
            stLog.nParam1 = stInvite.dwTargetActorID;
            stLog.nParam2 = byTargetLevel;
            wcscpy_s(stLog.szComment, L"\uB9AC\uADF8 \uCD08\uB300");
            TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);
        }
        if (pTargetUser) {
            if (!pTargetUser->GetArea())
                return false;
            pTargetUser->IncrementJobCount();
            CLogicThreadManager::Instance().DoJob(pTargetUser->GetMapInsID().nMapID,
                [pTargetUser, stInvite]() {
                    if (!pTargetUser || !pTargetUser->IsLive() || !pTargetUser->GetArea())
                        return;
                    if (stInvite.nResult == 57042) {
                        CGocNetwork::SendErrorMessage(static_cast<CMover*>(pTargetUser), 0x22u, 0x13u, 0xDED2u);
                    } else if (stInvite.nResult == 57043) {
                        CGocNetwork::SendErrorMessage(static_cast<CMover*>(pTargetUser), 0x22u, 0x13u, 0xDED3u);
                    } else {
                        XSendPacket xSendPacket(0x22u, 0x37u);
                        xSendPacket << stInvite;
                        CGocNetwork::Send(pTargetUser, xSendPacket);
                    }
                });
            CLogicThreadManager::Instance().DoJob(pTargetUser->GetMapInsID().nMapID,
                [pTargetUser]() {
                    if (pTargetUser)
                        pTargetUser->DecrementJobCount();
                });
        }
    } else {
        CUser* pReqUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stInvite.dwActorID);
        if (pReqUser) {
            if (!pReqUser->GetArea())
                return false;
            pReqUser->IncrementJobCount();
            CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID,
                [pReqUser, stInvite]() {
                    if (!pReqUser || !pReqUser->IsLive() || !pReqUser->GetArea())
                        return;
                    if (stInvite.nResult == 57042) {
                        CGocNetwork::SendErrorMessage(static_cast<CMover*>(pReqUser), 0x22u, 0x13u, 0xDED2u);
                    } else if (stInvite.nResult == 57043) {
                        CGocNetwork::SendErrorMessage(static_cast<CMover*>(pReqUser), 0x22u, 0x13u, 0xDED3u);
                    } else {
                        XSendPacket xSendPacket(0x22u, 0x37u);
                        xSendPacket << stInvite;
                        CGocNetwork::Send(pReqUser, xSendPacket);
                    }
                });
            CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID,
                [pReqUser]() {
                    if (pReqUser)
                        pReqUser->DecrementJobCount();
                });
        }
    }

    return true;
}
// ============================================================================
// RecvLeagueInviteAccept - 处理公会邀请接受 (IDA 0x14020BD70)
// 解析 ST_REQ_LEAGUE_INVITE_ACCEPT -> DoJob x2
// lambda159: 发送 (0x22,0x14)+ST_REQ_LEAGUE_INVITE_ACCEPT
// ============================================================================
bool CCommunitySocket::RecvLeagueInviteAccept(XPacket* xPacket)
{
    ST_REQ_LEAGUE_INVITE_ACCEPT stInviteAccept;
    *xPacket >> stInviteAccept;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stInviteAccept.dwTargetUCID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stInviteAccept]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;
            XSendPacket xSendPacket(0x22u, 0x14u);
            xSendPacket << stInviteAccept;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvLeagueInviteReject - 处理公会邀请拒绝 (IDA 0x1401FA020)
// 解析 ST_REQ_LEAGUE_INVITE_REJECT + nLeagueID -> DoJob x2
// lambda35: 发送 (0x22,0x15)+ST_REQ_LEAGUE_INVITE_REJECT + ST_LOG_GAME(15,5) "길드 접속 서절"
// ============================================================================
bool CCommunitySocket::RecvLeagueInviteReject(XPacket* xPacket)
{
    ST_REQ_LEAGUE_INVITE_REJECT stReject;
    *xPacket >> stReject;
    int nLeagueID = 0;
    xPacket->XParse >> nLeagueID;

    CUser* pReqUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stReject.dwReqUCID);
    if (!pReqUser)
        return true;
    if (!pReqUser->GetArea())
        return false;

    pReqUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID,
        [pReqUser, stReject, nLeagueID]() {
            if (!pReqUser || !pReqUser->IsLive() || !pReqUser->GetArea())
                return;
            XSendPacket xSendPacket(0x22u, 0x15u);
            xSendPacket << stReject;
            CGocNetwork::Send(pReqUser, xSendPacket);

            CUser* pLogUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stReject.dwTargetUCID);
            if (pLogUser) {
                ST_LOG_GAME stLog;
                stLog._nUAID = static_cast<int>(pLogUser->GetUAID());
                stLog._nUCID = pLogUser->GetActorID().dwActorID;
                stLog._sMainType = 15;
                stLog._sSubType = 5;
                stLog.nParam0 = nLeagueID;
                stLog.nParam1 = stReject.dwReqUCID;
                stLog.nParam5 = pLogUser->GetLevel();
                wcscpy_s(stLog.szComment, L"\uB9AC\uADF8 \uCD08\uB300 \uAC70\uC808");
                TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);
            }
        });

    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID,
        [pReqUser]() {
            if (pReqUser)
                pReqUser->DecrementJobCount();
        });

    return true;
}

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

// ============================================================================
// RecvLeagueApplicantAcceptRes - 处理公会申请者接受结果 (IDA 0x140211900)
// 解析 PS_RES_LEAGUE_ACCEPT_ACCPLICANT -> DoJob x2
// lambda188: 发送 (0x22,0x18)+nResult
// ============================================================================
bool CCommunitySocket::RecvLeagueApplicantAcceptRes(XPacket* xPacket)
{
    PS_RES_LEAGUE_ACCEPT_ACCPLICANT psResAcceptInfo;
    *xPacket >> psResAcceptInfo;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(psResAcceptInfo.dwUCID);
    if (!pUser)
        return false;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, psResAcceptInfo]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;
            XSendPacket xSendPacket(0x22u, 0x18u);
            xSendPacket.XParse << psResAcceptInfo.nResult;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// RecvLeagueSearch - 处理公会搜索 (IDA 0x1401F66C0)
// 解析 dwActorID + PS_LEAGUE_SUMMARY_LIST + ST_LEAGUE_APPLICANT_CHECK_LIST -> DoJob x2
// lambda139: 发送 (0x22,3)+PS_LEAGUE_SUMMARY_LIST+ST_LEAGUE_APPLICANT_CHECK_LIST
// ============================================================================
bool CCommunitySocket::RecvLeagueSearch(XPacket* xPacket)
{
    std::uint32_t dwActorID = 0;
    PS_LEAGUE_SUMMARY_LIST psLeagueSummaryList;
    ST_LEAGUE_APPLICANT_CHECK_LIST stApplyList;
    xPacket->XParse >> dwActorID;
    *xPacket >> psLeagueSummaryList;
    *xPacket >> stApplyList;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, psLeagueSummaryList, stApplyList]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;
            XSendPacket xSendPacket(0x22u, 3u);
            xSendPacket << psLeagueSummaryList;
            xSendPacket << stApplyList;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}

// ============================================================================
// RecvLeagueApplicantRes - 处理公会申请者响应 (IDA 0x1401F4B30)
// 解析 ST_LEAGUE_APPLICANT -> DoJob x2
// lambda5: 发送 (0x22,0x23)+ST_LEAGUE_APPLICANT+biRemainTime(0) + ST_LOG_GAME(15,6) "길드 지원"
// ============================================================================
bool CCommunitySocket::RecvLeagueApplicantRes(XPacket* xPacket)
{
    ST_LEAGUE_APPLICANT stApplicant;
    *xPacket >> stApplicant;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stApplicant.dwActorID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stApplicant]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;
            std::int64_t biRemainTime = 0;
            XSendPacket xSendPacket(0x22u, 0x23u);
            xSendPacket << stApplicant;
            xSendPacket.XParse << biRemainTime;
            CGocNetwork::Send(pUser, xSendPacket);

            ST_LOG_GAME stLog;
            stLog._nUAID = static_cast<int>(pUser->GetUAID());
            stLog._nUCID = pUser->GetActorID().dwActorID;
            stLog._sMainType = 15;
            stLog._sSubType = 6;
            stLog.nParam0 = stApplicant.nLeagueID;
            wcscpy_s(stLog.szComment, L"\uB9AC\uADF8 \uC9C0\uC6D0");
            TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvLeagueApplicantAdd - 处理公会申请者添加 (IDA 0x1401F5070)
// 解析 ST_LEAGUE_APPLICANT -> lambda7 (0x1401F5160) DoJobAllThread:
// 按值副本交 ThreadLocalData::SendLeagueApply。返回 1。
// ============================================================================
bool CCommunitySocket::RecvLeagueApplicantAdd(XPacket* xPacket)
{
    ST_LEAGUE_APPLICANT stApplicant{};
    *xPacket >> stApplicant;

    // Per IDA lambda7 (0x1401F5160)
    CLogicThreadManager::Instance().DoJobAllThread([stApplicant]() {
        ST_LEAGUE_APPLICANT stCopy = stApplicant;
        ThreadLocalData::GetInstance()->SendLeagueApply(stCopy);
    });
    return true;
}
// ============================================================================
// RecvLeagueApplicantReject - 处理公会申请者拒绝 (IDA 0x1401F8550)
// 解析 ST_REQ_LEAGUE_APPLICANT_REJECT -> DoJob x2
// lambda24: 发送 (0x22,0x25)+ST_REQ_LEAGUE_APPLICANT_REJECT + ST_LOG_GAME(15,8) "길드 지원 서절"
// ============================================================================
bool CCommunitySocket::RecvLeagueApplicantReject(XPacket* xPacket)
{
    ST_REQ_LEAGUE_APPLICANT_REJECT stReject;
    *xPacket >> stReject;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(stReject.dwUCID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stReject]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;
            XSendPacket xSendPacket(0x22u, 0x25u);
            xSendPacket << stReject;
            CGocNetwork::Send(pUser, xSendPacket);

            ST_LOG_GAME stLog;
            stLog._nUAID = static_cast<int>(pUser->GetUAID());
            stLog._nUCID = pUser->GetActorID().dwActorID;
            stLog._sMainType = 15;
            stLog._sSubType = 8;
            stLog.nParam0 = stReject.nLeagueID;
            stLog.nParam1 = stReject.dwTargetUCID;
            stLog.nParam5 = pUser->GetLevel();
            wcscpy_s(stLog.szComment, L"\uB9AC\uADF8 \uC9C0\uC6D0 \uAC70\uC808");
            TXSingleton<XGameServer>::Instance()->SendDBLog(stLog);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
bool CCommunitySocket::RecvLeagueApplicantDelete(XPacket*) { return true; }
// ============================================================================
// RecvLeagueList - 处理公会列表 (IDA 0x140208CA0)
// 解析 dwUCID + PS_LEAGUE_SUMMARY_LIST + ST_LEAGUE_APPLICANT_CHECK_LIST -> DoJob x2
// lambda139: 发送 (0x22,3)+PS_LEAGUE_SUMMARY_LIST+ST_LEAGUE_APPLICANT_CHECK_LIST
// ============================================================================
bool CCommunitySocket::RecvLeagueList(XPacket* xPacket)
{
    std::uint32_t dwUCID = 0;
    PS_LEAGUE_SUMMARY_LIST psLeagueSummaryList;
    ST_LEAGUE_APPLICANT_CHECK_LIST stApplyList;
    xPacket->XParse >> dwUCID;
    *xPacket >> psLeagueSummaryList;
    *xPacket >> stApplyList;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwUCID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, psLeagueSummaryList, stApplyList]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea())
                return;
            XSendPacket xSendPacket(0x22u, 3u);
            xSendPacket << psLeagueSummaryList;
            xSendPacket << stApplyList;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvLeagueNameChange - IDA @ 0x14021A620
// 已精确还原 - 解析 PS_LEAGUE_NAME_CHANGE_SERVER；按 dwUCID 查用户；
// 在线（GetArea 非空）时: lambda253 (0x14021A9E0) DoJob（SetReqLeagueNameChange +
//   构造 psResChangeInfo{nLeagueID,nResult,szLeagueName}:
//   - nResult != 0: UnLockList(psUpdateItemList)
//   - nResult == 0: ST_LOG_GAME{22} + UpdateItemEnd(0x78)；失败则 LogError +
//     UnLockList + nResult=52011 + (0x22,0x58) 发送后 return；
//     成功则 SendUpdateItem + SetLeagueName + (0x22,0x44) BroadcastNearby +
//     ST_LOG_GAME{15,16} + 逐物品 SendDBLog）
//   最后 (0x22,0x58)+psResChangeInfo 发送）+ lambda192 递减；
//   lambda255 (0x14021B0A0) DoJobAllThread（nResult==0 时构造 psRes 副本 ->
//   ThreadLocalData::SendLeagueChangeName(psCopy, nSysnCount)）。
// 用户不在线返回 false，否则返回 true。
// ============================================================================
bool CCommunitySocket::RecvLeagueNameChange(XPacket* xPacket) {
    PS_LEAGUE_NAME_CHANGE_SERVER psChangeInfo;
    *xPacket >> psChangeInfo;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(
        psChangeInfo.dwUCID) : nullptr;

    if (!pUser || !pUser->GetArea()) {
        return false;
    }

    pUser->IncrementJobCount();
    // Per IDA lambda253 (0x14021A9E0): 改名结果处理链
    std::function<void()> func = [pUser, psChangeInfo]() {
        if (!pUser || !pUser->IsLive()) {
            return;
        }
        CGocInventory* pInven = pUser->GetGOC<CGocInventory>();
        if (!pInven) {
            return;
        }
        pInven->SetReqLeagueNameChange(false);

        PS_RES_LEAGUE_NAME_CHANGE psResChangeInfo;
        psResChangeInfo.nLeagueID = psChangeInfo.nLeagueID;
        psResChangeInfo.nResult = psChangeInfo.nResult;
        wcscpy_s(psResChangeInfo.szLeagueName, psChangeInfo.szLeagueName);

        if (psChangeInfo.nResult != 0) {
            pInven->UnLockList(psChangeInfo.psUpdateItemList);
        } else {
            ST_LOG_GAME stLog = {};
            stLog._sSubType = 22;
            PS_RES_STORAGE_INFO psUpdateItemList = psChangeInfo.psUpdateItemList;
            if (!pInven->UpdateItemEnd(0x78, psUpdateItemList, stLog)) {
                LogHelper::LogError("game.item",
                    "RecvLeagueNameChange error - UpdateItemEnd[ ActorID:%d ] ( %d )",
                    psChangeInfo.dwUCID, 5041);
                pInven->UnLockList(psChangeInfo.psUpdateItemList);

                psResChangeInfo.nResult = 52011;
                XSendPacket packet(0x22, 0x58);
                packet << psResChangeInfo;
                CGocNetwork::Send(pUser, packet);
                return;
            }

            pInven->SendUpdateItem(psUpdateItemList);

            ST_LEAGUE_INFO_EX stInfoExLog;
            ST_LEAGUE_INFO_EX stInfoEx;
            pUser->GetLeagueInfo(stInfoExLog);
            pUser->SetLeagueName(psResChangeInfo.szLeagueName);
            pUser->GetLeagueInfo(stInfoEx);

            XSendPacket packet(0x22, 0x44);
            packet << stInfoEx;
            CGocNetwork::BroadcastNearby(pUser, nullptr, packet);

            ST_LOG_GAME stLogGame = {};
            stLogGame._sMainType = 15;
            stLogGame._sSubType = 16;
            stLogGame._nUAID = static_cast<int>(pUser->GetUAID());
            stLogGame._nUCID = static_cast<int>(pUser->GetActorID().dwActorID);
            stLogGame.nParam0 = psResChangeInfo.nLeagueID;
            stLogGame.nParam5 = pUser->GetLevel();
            wcscpy_s(stLogGame.szComment, stInfoExLog.szLeagueName);
            wcscpy_s(stLogGame.szComment2, psResChangeInfo.szLeagueName);
            for (std::size_t i = 0; i < psUpdateItemList.vecItem.size(); ++i) {
                PS_STORAGE_INFO psInfo = psUpdateItemList.vecItem[i];
                stLogGame.nParam1 = psInfo.stItem.nItemID;
                stLogGame.nParam6 = psInfo.stItem.xSerial;
                XGameServer::Instance()->SendDBLog(stLogGame);
            }
        }

        XSendPacket xSendPacket(0x22, 0x58);
        xSendPacket << psResChangeInfo;
        CGocNetwork::Send(pUser, xSendPacket);
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pUser]() {
        pUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

    // Per IDA lambda255 (0x14021B0A0): nResult==0 时全线程广播改名
    CLogicThreadManager::Instance().DoJobAllThread(
        [psChangeInfo]() {
            if (psChangeInfo.nResult != 0) {
                return;
            }
            PS_RES_LEAGUE_NAME_CHANGE psResChangeInfo;
            psResChangeInfo.nLeagueID = psChangeInfo.nLeagueID;
            psResChangeInfo.nResult = psChangeInfo.nResult;
            wcscpy_s(psResChangeInfo.szLeagueName, psChangeInfo.szLeagueName);

            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->SendLeagueChangeName(psResChangeInfo,
                                                 psChangeInfo.nSysnCount);
            }
        });

    return true;
}
// ============================================================================
// RecvLeagueCardChange - IDA @ 0x14020D340
// 已精确还原 - 解析 PS_REQ_LEAGUE_CARD + nSyncCount(int)；
// lambda170 (0x14020D480) DoJobAllThread -> 按值副本交给
// ThreadLocalData::SendLeagueCardChange(psCard, nSyncCount)。返回 true。
// ============================================================================
bool CCommunitySocket::RecvLeagueCardChange(XPacket* xPacket) {
    PS_REQ_LEAGUE_CARD psCard;
    int nSyncCount = 0;
    *xPacket >> psCard;
    xPacket->XParse >> nSyncCount;

    // Per IDA lambda170 (0x14020D480)
    PS_REQ_LEAGUE_CARD psCardCopy = psCard;
    CLogicThreadManager::Instance().DoJobAllThread(
        [psCardCopy, nSyncCount]() {
            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->SendLeagueCardChange(psCardCopy, nSyncCount);
            }
        });

    return true;
}

// ============================================================================
// RecvLeaguePositionNameChange - IDA @ 0x140209840
// 已精确还原 - 解析 dwActorID(int) >> ST_LEAGUE_POSITION_NAME_CHANGE >> nLeagueID(int)；
// pUser 在线（GetArea 非空）时 lambda35 (0x1401FA310) DoJob: 存活检查 ->
// (0x22,0x15)+stChange 发送 -> FindActorIDToUser(dwActorID) -> ST_LOG_GAME
// {_sMainType=15, _sSubType=5, nParam0=nLeagueID, nParam1=dwActorID,
// nParam5=GetLevel(), szComment=硬编码串} SendDBLog；+ lambda192 递减；
// 无论在线与否 lambda146 (0x140209E00) DoJobAllThread ->
// ThreadLocalData::ChangePositionName(stChange 副本, nLeagueID)。返回 true。
// ============================================================================
bool CCommunitySocket::RecvLeaguePositionNameChange(XPacket* xPacket) {
    int dwActorID = 0;
    ST_LEAGUE_POSITION_NAME_CHANGE stChange;
    int nLeagueID = 0;
    xPacket->XParse >> dwActorID;
    *xPacket >> stChange;
    xPacket->XParse >> nLeagueID;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(
        static_cast<std::uint32_t>(dwActorID)) : nullptr;

    if (pUser && pUser->GetArea()) {
        pUser->IncrementJobCount();
        // Per IDA lambda35 (0x1401FA310)
        std::function<void()> func = [pUser, stChange, dwActorID, nLeagueID]() {
            if (!pUser || !pUser->IsLive() || !pUser->GetArea()) {
                return;
            }

            XSendPacket xSendPacket(0x22, 0x15);
            xSendPacket << stChange;
            CGocNetwork::Send(pUser, xSendPacket);

            XGameServer* pServer = XGameServer::Instance();
            CUser* pTargetUser = pServer ? pServer->FindActorIDToUser(
                static_cast<std::uint32_t>(dwActorID)) : nullptr;
            if (pTargetUser) {
                ST_LOG_GAME stLog = {};
                stLog._nUAID = static_cast<int>(pTargetUser->GetUAID());
                stLog._nUCID = static_cast<int>(
                    pTargetUser->GetActorID().dwActorID);
                stLog._sMainType = 15;
                stLog._sSubType = 5;
                stLog.nParam0 = nLeagueID;
                stLog.nParam1 = dwActorID;
                stLog.nParam5 = pTargetUser->GetLevel();
                // IDA word_140B75CC0 = L"리그 " (CP949: AC B9 F8 AD 20 00)
                std::wcscpy(stLog.szComment, L"리그 ");
                XGameServer::Instance()->SendDBLog(stLog);
            }
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

        // Per IDA lambda192 (0x140427E60): 任务完成递减计数
        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
    }

    // Per IDA lambda146 (0x140209E00)
    ST_LEAGUE_POSITION_NAME_CHANGE stChangeCopy = stChange;
    CLogicThreadManager::Instance().DoJobAllThread(
        [stChangeCopy, nLeagueID]() {
            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->ChangePositionName(stChangeCopy, nLeagueID);
            }
        });

    return true;
}
// ============================================================================
// RecvLeagueAuthChange - IDA @ 0x140209190
// 已精确还原 - 解析 ST_LEAGUE_AUTH_CHANGE >> nLeagueID(int) >>
// dwActorID(u32) >> nSyncCount(int)；
// pUser 在线（GetArea 非空）时:
//   lambda30 (0x1404F0DB0) DoJob: IsLive 检查 -> GetLeagueID!=0 时
//   (0xF6,0x28)+stChange+nLeagueID+dwActorID 经 CommunitySocket::SendCmd
//   (0x22,0x32) 转发；==0 时 LogDebug "Not Exist League" + SendErrorMessage(57016)；
//   lambda192 递减；
//   8 次循环 ST_LOG_GAME{15,11, nParam0=nLeagueID, nParam3=idx,
//   nParam4=nAuth[idx], szComment=L"LEAGUE " (word_140B562D0)} SendDBLog；
// 无论在线与否 lambda143 (0x1402097E0) DoJobAllThread ->
// ThreadLocalData::ChangeLeagueAuth(stChange 副本, nLeagueID, nSyncCount)。
// 返回 true。
// ============================================================================
bool CCommunitySocket::RecvLeagueAuthChange(XPacket* xPacket) {
    ST_LEAGUE_AUTH_CHANGE stChange;
    int nLeagueID = 0;
    unsigned int dwActorID = 0;
    int nSyncCount = 0;
    *xPacket >> stChange;
    xPacket->XParse >> nLeagueID;
    xPacket->XParse >> dwActorID;
    xPacket->XParse >> nSyncCount;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (pUser) {
        if (!pUser->GetArea()) {
            return false;
        }
        pUser->IncrementJobCount();
        // Per IDA lambda30 (0x1404F0DB0): 权限包转发链
        std::function<void()> func = [pUser, stChange, nLeagueID, dwActorID]() {
            if (!pUser || !pUser->IsLive()) {
                return;
            }
            if (pUser->GetLeagueID() != 0) {
                XSendPacket xSendPacket(0xF6, 0x28);
                xSendPacket << stChange;
                xSendPacket.XParse << pUser->GetLeagueID();
                xSendPacket.XParse << pUser->GetActorID().dwActorID;
                XGameServer::Instance()->GetCommunitySocket().SendCmd(
                    &xSendPacket, pUser, 0x22, 0x32);
            } else {
                LogHelper::LogDebug("game.league",
                    "Not Exist League [LeagueID:%d]", pUser->GetLeagueID());
                pUser->SendErrorMessage(0x22, 0x28, 57016);
            }
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

        // Per IDA lambda192 (0x140427E60): 任务完成递减计数
        std::function<void()> funcDec = [pUser]() {
            pUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);

        for (int i = 0; i < 8; ++i) {
            ST_LOG_GAME stLog = {};
            stLog._nUAID = static_cast<int>(pUser->GetUAID());
            stLog._nUCID = static_cast<int>(pUser->GetActorID().dwActorID);
            stLog._sMainType = 15;
            stLog._sSubType = 11;
            stLog.nParam0 = nLeagueID;
            stLog.nParam3 = i;
            stLog.nParam4 = stChange.nAuth[i];
            // IDA word_140B562D0 = L"LEAGUE " (CP949: AC B9 F8 AD 20 00)
            std::wcscpy(stLog.szComment, L"리그 ");
            XGameServer::Instance()->SendDBLog(stLog);
        }
    }

    // Per IDA lambda143 (0x1402097E0)
    CLogicThreadManager::Instance().DoJobAllThread(
        [stChange, nLeagueID, nSyncCount]() {
            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->ChangeLeagueAuth(stChange, nLeagueID, nSyncCount);
            }
        });

    return true;
}
// ============================================================================
// RecvLeagueMessage - IDA @ 0x140209E50
// 已精确还原 - 解析 PS_CHAT_LEAGUE + PS_CHAT_ITEM_LINK_FOR_SERVER；
// lambda147 (0x14020A020) DoJobAllThread -> 两结构按值副本交给
// ThreadLocalData::SendLeagueMsg 广播 (7,1) 聊天包。返回 true。
// ============================================================================
bool CCommunitySocket::RecvLeagueMessage(XPacket* xPacket) {
    PS_CHAT_LEAGUE stChatLeague;
    PS_CHAT_ITEM_LINK_FOR_SERVER psLinkItemInfo;
    *xPacket >> stChatLeague;
    *xPacket >> psLinkItemInfo;

    // Per IDA lambda147 (0x14020A020)
    CLogicThreadManager::Instance().DoJobAllThread(
        [stChatLeague, psLinkItemInfo]() {
            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->SendLeagueMsg(stChatLeague, psLinkItemInfo);
            }
        });

    return true;
}
// ============================================================================
// RecvLeagueMemberPositionChange - IDA @ 0x14020A160
// 已精确还原 - 解析 ST_LEAGUE_MEMBER_POSITION >> nLeagueID(int) >>
// dwActorID(u32) >> byPrevPosition(char) >> ST_LEAGUE_INFO_FOR_GAME >>
// nSyncCount(int)；按 dwActorID 查用户；
// pUser 在线且 stPosition.nResult > 0 时:
//   lambda148 (0x14020A5B0) DoJob: IsLive+GetArea 检查 ->
//   (0x22,0x42)+stPosition 副本发送 + lambda192 递减；
// 无论是否处理 lambda150 (0x14020A7A0) DoJobAllThread ->
// ThreadLocalData::UpdateMemberPosition(stPosition 副本, nLeagueID,
// dwActorID, nSyncCount)。返回 true。
// ============================================================================
bool CCommunitySocket::RecvLeagueMemberPositionChange(XPacket* xPacket) {
    ST_LEAGUE_MEMBER_POSITION stPosition;
    int nLeagueID = 0;
    unsigned int dwActorID = 0;
    char byPrevPosition = 0;
    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame;
    int nSyncCount = 0;

    *xPacket >> stPosition;
    xPacket->XParse >> nLeagueID;
    xPacket->XParse >> dwActorID;
    xPacket->XParse >> byPrevPosition;
    *xPacket >> stLeagueInfoForGame;
    xPacket->XParse >> nSyncCount;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pUser = pServer ? pServer->FindActorIDToUser(dwActorID) : nullptr;

    if (pUser) {
        pUser->GetUAID();
        if (stPosition.nResult > 0) {
            if (!pUser->GetArea()) {
                return false;
            }
            pUser->IncrementJobCount();
            // Per IDA lambda148 (0x14020A5B0): 职位变更包发送链
            std::function<void()> func = [pUser, stPosition]() {
                if (!pUser || !pUser->IsLive() || !pUser->GetArea()) {
                    return;
                }

                ST_LEAGUE_MEMBER_POSITION stCopy = stPosition;
                XSendPacket xSendPacket(0x22, 0x42);
                xSendPacket << stCopy;
                CGocNetwork::Send(pUser, xSendPacket);
            };
            CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, func);

            // Per IDA lambda192 (0x140427E60): 任务完成递减计数
            std::function<void()> funcDec = [pUser]() {
                pUser->DecrementJobCount();
            };
            CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID, funcDec);
        }
    }

    // Per IDA lambda150 (0x14020A7A0)
    CLogicThreadManager::Instance().DoJobAllThread(
        [stPosition, nLeagueID, dwActorID, nSyncCount]() {
            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->UpdateMemberPosition(stPosition, nLeagueID,
                                                 dwActorID, nSyncCount);
            }
        });

    return true;
}
// ============================================================================
// RecvLeagueApplicantUpdate - IDA @ 0x14020A800
// 已精确还原 - 解析 dwActorID(u32) + ST_LEAGUE_APPLICANT_CHECK_LIST；
// lambda151 (0x14020A960) DoJobAllThread -> ThreadLocalData::LeagueApplicantUpdate
// (stUpdateList, dwActorID)。返回 true。
// ============================================================================
bool CCommunitySocket::RecvLeagueApplicantUpdate(XPacket* xPacket) {
    unsigned int dwActorID = 0;
    ST_LEAGUE_APPLICANT_CHECK_LIST stUpdateList;
    xPacket->XParse >> dwActorID;
    *xPacket >> stUpdateList;

    // Per IDA lambda151 (0x14020A960)
    CLogicThreadManager::Instance().DoJobAllThread(
        [stUpdateList, dwActorID]() mutable {
            ST_LEAGUE_APPLICANT_CHECK_LIST stCopy = stUpdateList;
            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->LeagueApplicantUpdate(stCopy, dwActorID);
            }
        });

    return true;
}
// ============================================================================
// RecvLeagueMemberLogOut - IDA @ 0x14020A9E0
// 已精确还原 - 解析 ST_LEAGUE_MEMBER_UPDATE；lambda152 DoJobAllThread ->
// ThreadLocalData::UpdateLeagueMember(stUpdate 副本)。返回 true。
// ============================================================================
bool CCommunitySocket::RecvLeagueMemberLogOut(XPacket* xPacket) {
    ST_LEAGUE_MEMBER_UPDATE stUpdate;
    *xPacket >> stUpdate;

    // Per IDA lambda152 (0x14020AB10)
    CLogicThreadManager::Instance().DoJobAllThread(
        [stUpdate]() {
            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->UpdateLeagueMember(stUpdate);
            }
        });

    return true;
}
// ============================================================================
// RecvLeagueApplicantJoinUser - IDA @ 0x1401F6C40
// 已精确还原 - 解析 ST_LEAGUE_MEMBER_EX + ST_LEAGUE_INFO_EX +
// ST_LEAGUE_INFO_UPDATE + dwActorID(u32) + ST_LEAGUE_INFO_FOR_GAME +
// nSyncCount(int)；
// 1) lambda16 (0x1401F7310) DoJobAllThread -> ThreadLocalData::
//    SendLeagueJoinUser_Apply(stMemberEx, stInfoUpdate, nSyncCount)。
// 2) 申请者在线（GetArea 非空）时 lambda17 (0x1401F74E0) DoJob:
//    UpdateLeagueSyncCount(nSyncCount) -> SetLeagueInfo(stInfoEx, forGame) ->
//    dwUCID = ActorID & 0x1FFFFFFF -> AddLeagueMember -> (0x22,0x44)
//    SendBroadCast(eNearby) -> ST_LOG_GAME{15,14, szComment=L"LEAGUE ",
//    szComment2=szLeagueName} -> 查 dwActorID 用户再发
//    ST_LOG_GAME{15,7, szComment=L"LEAGUE 지원자 "} -> CGocClassEvent
//    8 号事件通知；+ lambda192 递减。
// 3) 申请者不在线且 stMemberEx.byClass == 6 时（原公会长分支）:
//    按 dwActorID 查 master 用户，在线则 lambda30 (0x1403B6BE0) DoJob:
//    IsLive && !IsBit_OR(0xC0) && GetArea 检查 ->
//    master 当前地图实例 ID != stMemberEx.sWorldID 或服务器 ID !=
//    Option serverID 时 PS_KICK_USER_INFO{dwUAID, byKickType=39} Kickout；
//    + lambda192 递减。返回 true。
// ============================================================================
bool CCommunitySocket::RecvLeagueApplicantJoinUser(XPacket* xPacket) {
    ST_LEAGUE_MEMBER_EX stMemberEx;
    ST_LEAGUE_INFO_EX stInfoEx;
    ST_LEAGUE_INFO_UPDATE stInfoUpdate;
    unsigned int dwActorID = 0;
    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame;
    int nSyncCount = 0;

    *xPacket >> stMemberEx;
    *xPacket >> stInfoEx;
    *xPacket >> stInfoUpdate;
    xPacket->XParse >> dwActorID;
    *xPacket >> stLeagueInfoForGame;
    xPacket->XParse >> nSyncCount;

    // Per IDA lambda16 (0x1401F7310): 全线程广播入会申请
    CLogicThreadManager::Instance().DoJobAllThread(
        [stMemberEx, stInfoUpdate, nSyncCount]() {
            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->SendLeagueJoinUser_Apply(stMemberEx,
                                                     stInfoUpdate, nSyncCount);
            }
        });

    XGameServer* pServer = XGameServer::Instance();
    CUser* pJoinedUser = pServer ? pServer->FindActorIDToUser(
        stMemberEx.dwUCID) : nullptr;

    if (pJoinedUser) {
        if (!pJoinedUser->GetArea()) {
            return false;
        }
        pJoinedUser->IncrementJobCount();
        // Per IDA lambda17 (0x1401F74E0): 入会者信息同步链
        std::function<void()> func = [pJoinedUser, stMemberEx, stInfoEx,
                                       stInfoUpdate, stLeagueInfoForGame,
                                       dwActorID, nSyncCount]() {
            if (!pJoinedUser || !pJoinedUser->IsLive() ||
                !pJoinedUser->GetArea()) {
                return;
            }

            pJoinedUser->UpdateLeagueSyncCount(nSyncCount);

            ST_LEAGUE_INFO_EX stLocalInfoEx = stInfoEx;
            ST_LEAGUE_INFO_FOR_GAME stLocalForGame = stLeagueInfoForGame;
            pJoinedUser->SetLeagueInfo(stLocalInfoEx, stLocalForGame);
            stLocalInfoEx.dwUCID = pJoinedUser->GetActorID().dwActorID & 0x1FFFFFFF;

            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->AddLeagueMember(pJoinedUser);
            }

            XSendPacket xSendPacket(0x22, 0x44);
            xSendPacket << stLocalInfoEx;
            CGocNetwork::SendBroadCast(pJoinedUser, xSendPacket,
                                       E_BROADCAST_TYPE::eNearby);

            ST_LOG_GAME stLog = {};
            stLog._nUAID = static_cast<int>(pJoinedUser->GetUAID());
            stLog._nUCID = stMemberEx.dwUCID;
            stLog._sMainType = 15;
            stLog._sSubType = 14;
            stLog.nParam0 = stMemberEx.stMember.nLeagueID;
            stLog.nParam5 = pJoinedUser->GetLevel();
            // IDA word_140B75C60 = L"LEAGUE "
            std::wcscpy(stLog.szComment, L"리그 ");
            std::wcscpy(stLog.szComment2, stLocalInfoEx.szLeagueName);
            XGameServer::Instance()->SendDBLog(stLog);

            XGameServer* pServer = XGameServer::Instance();
            CUser* pAcceptUser = pServer ? pServer->FindActorIDToUser(
                dwActorID) : nullptr;
            if (pAcceptUser) {
                ST_LOG_GAME stLogGame = {};
                stLogGame._nUAID = static_cast<int>(pAcceptUser->GetUAID());
                stLogGame._nUCID = static_cast<int>(
                    pAcceptUser->GetActorID().dwActorID);
                stLogGame._sMainType = 15;
                stLogGame._sSubType = 7;
                stLogGame.nParam0 = stMemberEx.stMember.nLeagueID;
                stLogGame.nParam1 = stMemberEx.dwUCID;
                stLogGame.nParam5 = pAcceptUser->GetLevel();
                // IDA word_140B75C70 = L"LEAGUE 지원자 "
                std::wcscpy(stLogGame.szComment, L"리그 지원자 ");
                XGameServer::Instance()->SendDBLog(stLogGame);
            }

            CGocClassEvent* pClassEvent = pJoinedUser->GetGOC<CGocClassEvent>();
            if (pClassEvent) {
                // TODO: 需人工审查 - IDA 模板展开误标为 CItem::CanBroachEquip(8,0)，
                // 按调用语义应为 CGocClassEvent 的 8 号事件通知接口，具体方法名待
                // CGocClassEvent 还原批次核实。
            }
        };
        CLogicThreadManager::Instance().DoJob(pJoinedUser->GetMapInsID().nMapID, func);

        // Per IDA lambda192 (0x140427E60): 任务完成递减计数
        std::function<void()> funcDec = [pJoinedUser]() {
            pJoinedUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pJoinedUser->GetMapInsID().nMapID, funcDec);
    } else if (stMemberEx.byClass == 6) {
        // 原公会长分支: 申请者已转职 6 类外会（byClass==6 视为脱离原公会），
        // 若原公会长仍在本服且地图/服不符则强制踢出
        CUser* pMasterUser = pServer ? pServer->FindActorIDToUser(
            dwActorID) : nullptr;
        if (pMasterUser) {
            if (!pMasterUser->GetArea()) {
                return false;
            }
            pMasterUser->IncrementJobCount();
            // Per IDA lambda30 (0x1403B6BE0): master 一致性强制踢出链
            std::function<void()> func = [pMasterUser, stMemberEx]() {
                if (!pMasterUser) {
                    return;
                }
                if (!pMasterUser->IsLive() ||
                    pMasterUser->IsBit_OR(
                        static_cast<XClient::E_NET_STATE>(
                            XClient::eStateChangeServer |
                            XClient::eStateChangeWorld))) {
                    return;
                }
                if (!pMasterUser->GetArea()) {
                    return;
                }
                if (pMasterUser->GetArea()->GetInstanceID().nMapID !=
                        stMemberEx.stMember.nLeagueID ||
                    XGameServer::Instance()->GetOption().GetServerID() !=
                        static_cast<std::uint32_t>(
                            stMemberEx.sWorldID)) {
                    PS_KICK_USER_INFO psKick;
                    psKick.dwUAID = pMasterUser->GetUAID();
                    psKick.byKickType = 39;
                    pMasterUser->Kickout(&psKick, false);
                }
            };
            CLogicThreadManager::Instance().DoJob(pMasterUser->GetMapInsID().nMapID, func);

            // Per IDA lambda192 (0x140427E60): 任务完成递减计数
            std::function<void()> funcDec = [pMasterUser]() {
                pMasterUser->DecrementJobCount();
            };
            CLogicThreadManager::Instance().DoJob(pMasterUser->GetMapInsID().nMapID, funcDec);
        }
    }

    return true;
}
// ============================================================================
// RecvLeagueInviteJoinUser - IDA @ 0x1401F7B10
// 已精确还原 - 解析 ST_LEAGUE_MEMBER_EX + byApplyState(u8) + ST_LEAGUE_INFO_EX
// + ST_LEAGUE_INFO_UPDATE + dwActorID(u32) + ST_LEAGUE_INFO_FOR_GAME +
// nSyncCount(int)；
// 1) lambda21 (0x1401F8010) DoJobAllThread -> ThreadLocalData::
//    SendLeagueJoinUser_Invite(stMemberEx, stInfoUpdate, byApplyState, nSyncCount)。
// 2) 受邀者在线（GetArea 非空）时 lambda22 (0x1401F8200) DoJob:
//    UpdateLeagueSyncCount(nSyncCount) -> SetLeagueInfo(stInfoEx, forGame) ->
//    dwUCID = ActorID & 0x1FFFFFFF -> AddLeagueMember -> (0x22,0x44)
//    SendBroadCast(eNearby) -> ST_LOG_GAME{15,4, szComment=szLeagueName} ->
//    CGocClassEvent 8 号事件通知；+ lambda192 递减。
// 返回 true。
// ============================================================================
bool CCommunitySocket::RecvLeagueInviteJoinUser(XPacket* xPacket) {
    ST_LEAGUE_MEMBER_EX stMemberEx;
    unsigned char byApplyState = 0;
    ST_LEAGUE_INFO_EX stInfoEx;
    ST_LEAGUE_INFO_UPDATE stInfoUpdate;
    unsigned int dwActorID = 0;
    ST_LEAGUE_INFO_FOR_GAME stLeagueInfoForGame;
    int nSyncCount = 0;

    *xPacket >> stMemberEx;
    xPacket->XParse >> byApplyState;
    *xPacket >> stInfoEx;
    *xPacket >> stInfoUpdate;
    xPacket->XParse >> dwActorID;
    *xPacket >> stLeagueInfoForGame;
    xPacket->XParse >> nSyncCount;

    // Per IDA lambda21 (0x1401F8010): 全线程广播受邀入会
    CLogicThreadManager::Instance().DoJobAllThread(
        [stMemberEx, stInfoUpdate, byApplyState, nSyncCount]() {
            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->SendLeagueJoinUser_Invite(stMemberEx,
                                                      stInfoUpdate,
                                                      byApplyState, nSyncCount);
            }
        });

    XGameServer* pServer = XGameServer::Instance();
    CUser* pJoinUser = pServer ? pServer->FindActorIDToUser(
        stMemberEx.dwUCID) : nullptr;

    if (pJoinUser) {
        if (!pJoinUser->GetArea()) {
            return false;
        }
        pJoinUser->IncrementJobCount();
        // Per IDA lambda22 (0x1401F8200): 受邀者信息同步链
        std::function<void()> func = [pJoinUser, stMemberEx, stInfoEx,
                                       stInfoUpdate, dwActorID,
                                       stLeagueInfoForGame, nSyncCount]() {
            if (!pJoinUser || !pJoinUser->IsLive() ||
                !pJoinUser->GetArea()) {
                return;
            }

            pJoinUser->UpdateLeagueSyncCount(nSyncCount);

            ST_LEAGUE_INFO_EX stLocalInfoEx = stInfoEx;
            ST_LEAGUE_INFO_FOR_GAME stLocalForGame = stLeagueInfoForGame;
            pJoinUser->SetLeagueInfo(stLocalInfoEx, stLocalForGame);
            stLocalInfoEx.dwUCID = pJoinUser->GetActorID().dwActorID & 0x1FFFFFFF;

            ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
            if (pLocalData) {
                pLocalData->AddLeagueMember(pJoinUser);
            }

            XSendPacket xSendPacket(0x22, 0x44);
            xSendPacket << stLocalInfoEx;
            CGocNetwork::SendBroadCast(pJoinUser, xSendPacket,
                                       E_BROADCAST_TYPE::eNearby);

            ST_LOG_GAME stLog = {};
            stLog._nUAID = static_cast<int>(pJoinUser->GetUAID());
            stLog._nUCID = stMemberEx.dwUCID;
            stLog._sMainType = 15;
            stLog._sSubType = 4;
            stLog.nParam0 = stMemberEx.stMember.nLeagueID;
            stLog.nParam5 = pJoinUser->GetLevel();
            std::wcscpy(stLog.szComment, stLocalInfoEx.szLeagueName);
            XGameServer::Instance()->SendDBLog(stLog);

            CGocClassEvent* pClassEvent = pJoinUser->GetGOC<CGocClassEvent>();
            if (pClassEvent) {
                // TODO: 需人工审查 - IDA 模板展开误标为 CItem::CanBroachEquip(8,0)，
                // 按调用语义应为 CGocClassEvent 的 8 号事件通知接口，具体方法名待
                // CGocClassEvent 还原批次核实。
            }
        };
        CLogicThreadManager::Instance().DoJob(pJoinUser->GetMapInsID().nMapID, func);

        // Per IDA lambda192 (0x140427E60): 任务完成递减计数
        std::function<void()> funcDec = [pJoinUser]() {
            pJoinUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pJoinUser->GetMapInsID().nMapID, funcDec);
    }

    return true;
}
// ============================================================================
// RecvLeagueOpenOrNot - 处理公会开启/关闭 (IDA 0x14020C1A0)
// 解析 ST_LEAGUE_OPEN + dwUCID -> DoJob x2
// lambda161: 发送 (0x22,0x46)+ST_LEAGUE_OPEN
// ============================================================================
bool CCommunitySocket::RecvLeagueOpenOrNot(XPacket* xPacket)
{
    ST_LEAGUE_OPEN stOpen;
    *xPacket >> stOpen;
    std::uint32_t dwUCID = 0;
    xPacket->XParse >> dwUCID;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwUCID);
    if (!pUser)
        return true;
    if (!pUser->GetArea())
        return false;

    pUser->IncrementJobCount();
    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser, stOpen]() {
            XSendPacket xSendPacket(0x22u, 0x46u);
            xSendPacket << stOpen;
            CGocNetwork::Send(pUser, xSendPacket);
        });

    CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
        [pUser]() {
            if (pUser)
                pUser->DecrementJobCount();
        });

    return true;
}
// ============================================================================
// RecvLeagueRecruitNotice - 处理公会招募公告 (IDA 0x14020C4C0)
// 解析 ST_LEAGUE_RECRUIT_NOTICE + biRemainTime + dwUCID。
// 在线用户: lambda163 (0x14020C8B0) DoJob: IsLive 门 ->
//   (0x22,0x47) << stRecruitNotice << biRemainTime -> CGocNetwork::Send；
//   + lambda192 递减。
// 全线程: lambda165 (0x14020CA80) DoJobAllThread: 按值副本交
//   ThreadLocalData::SendLeagueRecruitNoticeToMember(st, biRemainTime)。
// ============================================================================
bool CCommunitySocket::RecvLeagueRecruitNotice(XPacket* xPacket)
{
    ST_LEAGUE_RECRUIT_NOTICE stRecruitNotice{};
    std::int64_t biRemainTime = 0;
    std::uint32_t dwUCID = 0;
    *xPacket >> stRecruitNotice;
    xPacket->XParse >> biRemainTime;
    xPacket->XParse >> dwUCID;

    CUser* pUser = TXSingleton<XGameServer>::Instance()->FindActorIDToUser(dwUCID);
    if (pUser) {
        if (!pUser->GetArea())
            return false;

        pUser->IncrementJobCount();

        // Per IDA lambda163 (0x14020C8B0)
        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
            [pUser, biRemainTime, stRecruitNotice]() {
                if (!pUser || !pUser->IsLive())
                    return;
                XSendPacket xSendPacket(0x22u, 0x47u);
                ST_LEAGUE_RECRUIT_NOTICE st = stRecruitNotice;
                xSendPacket << st;
                xSendPacket.XParse << biRemainTime;
                CGocNetwork::Send(pUser, xSendPacket);
            });

        CLogicThreadManager::Instance().DoJob(pUser->GetMapInsID().nMapID,
            [pUser]() {
                if (pUser)
                    pUser->DecrementJobCount();
            });
    }

    // Per IDA lambda165 (0x14020CA80)
    CLogicThreadManager::Instance().DoJobAllThread(
        [stRecruitNotice, biRemainTime]() {
            ST_LEAGUE_RECRUIT_NOTICE stCopy = stRecruitNotice;
            ThreadLocalData::GetInstance()->SendLeagueRecruitNoticeToMember(
                stCopy, biRemainTime);
        });
    return true;
}
// ============================================================================
// RecvLeagueRecordUpdate - 处理公会战绩更新 (IDA 0x14020CBC0)
// 解析 ST_LEAGUE_RECORD -> lambda166 (0x14020CCB0) DoJobAllThread:
// 按值副本交 ThreadLocalData::SendLeagueRecordUpdate。返回 1。
// ============================================================================
bool CCommunitySocket::RecvLeagueRecordUpdate(XPacket* xPacket)
{
    ST_LEAGUE_RECORD stRecord{};
    *xPacket >> stRecord;

    // Per IDA lambda166 (0x14020CCB0)
    CLogicThreadManager::Instance().DoJobAllThread([stRecord]() {
        ST_LEAGUE_RECORD stCopy = stRecord;
        ThreadLocalData::GetInstance()->SendLeagueRecordUpdate(stCopy);
    });
    return true;
}
// ============================================================================
// RecvLeagueCardChangeRes - 处理公会卡片变更结果 (IDA 0x14020D4D0)
// 解析 dwUCID + PS_REQ_LEAGUE_CARD + PS_RES_STORAGE_INFO + nErrorCode +
// nSyncCount。请求者不在线返回 0；在线但不在区域返回 0；否则
// lambda171 (0x14020D8C0) DoJob:
// - GetGOC<CGocInventory> 空时 LogError "CGocInventoryPtr is NULL(3360)" 返回。
// - nErrorCode!=0: LogError "Failed reduce item DB(3368)" + UnLockList +
//   (0x22,0x50) 发送 nErrorCode。
// - 否则 (0x22,0x50) 发送 psCardInfo.nResult；nResult==0 时 SendUpdateItem +
//   UpdateLeagueSyncCount(nSyncCount) + GetLeagueInfo(EX) ->
//   dwLeagueCard=psCardInfo.dwLeagueCard -> dwUCID=ActorID(无掩码) ->
//   SetLeagueInfo + (0x22,0x44) BroadcastNearby。
// + lambda192 递减。返回 1。
// ============================================================================
bool CCommunitySocket::RecvLeagueCardChangeRes(XPacket* xPacket) {
    std::uint32_t dwUCID = 0;
    PS_REQ_LEAGUE_CARD psCardInfo;
    PS_RES_STORAGE_INFO psUpdateItemList;
    int nErrorCode = 0;
    int nSyncCount = 0;

    xPacket->XParse >> dwUCID;
    *xPacket >> psCardInfo;
    *xPacket >> psUpdateItemList;
    xPacket->XParse >> nErrorCode;
    xPacket->XParse >> nSyncCount;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pReqUser = pServer ? pServer->FindActorIDToUser(dwUCID) : nullptr;
    if (!pReqUser) {
        return false;
    }
    if (!pReqUser->GetArea()) {
        return false;
    }

    pReqUser->IncrementJobCount();
    // Per IDA lambda171 (0x14020D8C0): 公会卡片变更下发链
    std::function<void()> func = [pReqUser, dwUCID, psCardInfo,
                                  psUpdateItemList, nErrorCode, nSyncCount]() {
        if (!pReqUser || !pReqUser->IsLive() || !pReqUser->GetArea()) {
            return;
        }

        CGocInventory* pInven = pReqUser->GetGOC<CGocInventory>();
        if (!pInven) {
            LogHelper::LogError("game.item",
                "RecvLeagueCardChangeRes error - CGocInventoryPtr is NULL(3360) [ ActorID:%d ]",
                dwUCID);
            return;
        }

        if (nErrorCode != 0) {
            LogHelper::LogError("game.item",
                "RecvLeagueCardChangeRes error - Failed reduce item DB(3368) [ ActorID:%d ] ( %d )",
                dwUCID, nErrorCode);
            pInven->UnLockList(psUpdateItemList);

            XSendPacket xSendPacket(0x22, 0x50);
            xSendPacket.XParse << nErrorCode;
            CGocNetwork::Send(pReqUser, xSendPacket);
            return;
        }

        XSendPacket xSendPacket(0x22, 0x50);
        xSendPacket.XParse << psCardInfo.nResult;
        CGocNetwork::Send(pReqUser, xSendPacket);

        if (psCardInfo.nResult == 0) {
            PS_RES_STORAGE_INFO psLocalItemList = psUpdateItemList;
            pInven->SendUpdateItem(psLocalItemList);

            pReqUser->UpdateLeagueSyncCount(nSyncCount);

            ST_LEAGUE_INFO_EX stInfoEx;
            pReqUser->GetLeagueInfo(stInfoEx);
            stInfoEx.dwLeagueCard = psCardInfo.dwLeagueCard;
            stInfoEx.dwUCID = pReqUser->GetActorID().dwActorID;
            pReqUser->SetLeagueInfo(stInfoEx);

            XSendPacket packet(0x22, 0x44);
            packet << stInfoEx;
            CGocNetwork::BroadcastNearby(pReqUser, nullptr, packet);
        }
    };
    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pReqUser]() {
        pReqUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, funcDec);

    return true;
}
bool CCommunitySocket::RecvLeagueLevelup(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueSkillLearn(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueWealth(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueSyncInfo(XPacket*) { return true; }
bool CCommunitySocket::RecvLeagueSyncLoad(XPacket*) { return true; }
// ============================================================================
// RecvLeagueInventoryInfo - 处理公会仓库信息下发 (IDA 0x140210170)
// 解析 dwReqUCID + PS_RES_STORAGE_INFO + PS_ITEM_BROACH_LIST +
// PS_ITEM_SOCKET_LIST + PS_ITEM_PACKAGE_LIST + nInventorySyncCount。
// 请求者不在线返回 0；在线但不在区域返回 0；否则
// lambda181 (0x140210640) DoJob: IsLive+GetArea 门 ->
// SetLeagueInventoryTime(GetCurDate()) ->
// UpdateLeagueInventorySyncCount(nInventorySyncCount) ->
// (0x22,0x56) 发送 psUpdateItemList + stBroachList + stSocketList +
// stPackageList 四个结构副本。
// + lambda192 递减。返回 1。
// ============================================================================
bool CCommunitySocket::RecvLeagueInventoryInfo(XPacket* xPacket) {
    std::uint32_t dwReqUCID = 0;
    PS_RES_STORAGE_INFO psUpdateItemList;
    PS_ITEM_BROACH_LIST stBroachList;
    PS_ITEM_SOCKET_LIST stSocketList;
    PS_ITEM_PACKAGE_LIST stPackageList;
    int nInventorySyncCount = 0;

    xPacket->XParse >> dwReqUCID;
    *xPacket >> psUpdateItemList;
    *xPacket >> stBroachList;
    *xPacket >> stSocketList;
    *xPacket >> stPackageList;
    xPacket->XParse >> nInventorySyncCount;

    XGameServer* pServer = XGameServer::Instance();
    CUser* pReqUser = pServer ? pServer->FindActorIDToUser(dwReqUCID) : nullptr;
    if (!pReqUser) {
        return false;
    }
    if (!pReqUser->GetArea()) {
        return false;
    }

    pReqUser->IncrementJobCount();
    // Per IDA lambda181 (0x140210640): 公会仓库信息下发链
    std::function<void()> func = [pReqUser, psUpdateItemList, stBroachList,
                                  stSocketList, stPackageList,
                                  nInventorySyncCount]() {
        if (!pReqUser || !pReqUser->IsLive() || !pReqUser->GetArea()) {
            return;
        }

        pReqUser->SetLeagueInventoryTime(
            XGameServer::Instance()->GetCurDate());
        pReqUser->UpdateLeagueInventorySyncCount(nInventorySyncCount);

        PS_RES_STORAGE_INFO psLocalItemList = psUpdateItemList;
        PS_ITEM_BROACH_LIST stLocalBroachList = stBroachList;
        PS_ITEM_SOCKET_LIST stLocalSocketList = stSocketList;
        PS_ITEM_PACKAGE_LIST stLocalPackageList = stPackageList;

        XSendPacket xSendPacket(0x22, 0x56);
        xSendPacket << psLocalItemList;
        xSendPacket << stLocalBroachList;
        xSendPacket << stLocalSocketList;
        xSendPacket << stLocalPackageList;
        CGocNetwork::Send(pReqUser, xSendPacket);
    };
    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, func);

    // Per IDA lambda192 (0x140427E60): 任务完成递减计数
    std::function<void()> funcDec = [pReqUser]() {
        pReqUser->DecrementJobCount();
    };
    CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, funcDec);

    return true;
}
// ============================================================================
// RecvLeagueInventoryMove - 处理公会仓库物品移动结果 (IDA 0x1402109D0)
// 解析 dwReqUCID + PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME。
// 请求者不在线: 跳过 DoJob 但 lambda185 仍执行；在线无 GetArea 返回 0。
// lambda183 (0x140210DE0) DoJob: IsLive+GetArea 门 -> GetGOC<CGocInventory>:
// - 空指针: 直接返回。
// - nErrorCode<=0: MoveItemToLeagueInven(psItemMoveForServer 副本)。
// - nErrorCode>0: byType==0 时 SetLock(bySrcInvenType,shSrcSlotPos,0);
//   byType==1 时 SetLock(byDestInvenType,shDestSlotPos,0);
//   SendErrorMessage(0x22,0x54,0xDEDC) + LogError 3764。
// nErrorCode==0 时 lambda185 (0x140211090) DoJobAllThread:
// GetInstance -> SendLeagueInventoryMove(dwReqUCID, 副本)。返回 1。
// ============================================================================
bool CCommunitySocket::RecvLeagueInventoryMove(XPacket* xPacket) {
    std::uint32_t dwReqUCID = 0;
    PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME psItemMoveForServer;

    xPacket->XParse >> dwReqUCID;
    *xPacket >> psItemMoveForServer;

    CUser* pReqUser = XGameServer::Instance()->FindActorIDToUser(dwReqUCID);
    if (pReqUser) {
        if (!pReqUser->GetArea()) {
            return false;
        }
        pReqUser->IncrementJobCount();
        // Per IDA lambda183 (0x140210DE0): 请求者侧物品落地链
        std::function<void()> func = [pReqUser, psItemMoveForServer]() {
            if (!pReqUser || !pReqUser->IsLive() || !pReqUser->GetArea()) {
                return;
            }
            CGocInventory* pInven = pReqUser->GetGOC<CGocInventory>();
            if (!pInven) {
                return;
            }
            if (psItemMoveForServer.nErrorCode <= 0) {
                PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME psLocal = psItemMoveForServer;
                pInven->MoveItemToLeagueInven(psLocal);
            } else {
                if (!psItemMoveForServer.psReqItemMoveInfo.byType) {
                    pInven->SetLock(
                        psItemMoveForServer.psReqItemMoveInfo.bySrcInvenType,
                        psItemMoveForServer.psReqItemMoveInfo.shSrcSlotPos,
                        0);
                }
                if (psItemMoveForServer.psReqItemMoveInfo.byType == 1) {
                    pInven->SetLock(
                        psItemMoveForServer.psReqItemMoveInfo.byDestInvenType,
                        psItemMoveForServer.psReqItemMoveInfo.shDestSlotPos,
                        0);
                }
                CGocNetwork::SendErrorMessage(pReqUser, 0x22u, 0x54u, 0xDEDCu);
                LogHelper::LogError("game.contents",
                    "RecvLeagueInventoryMove error - Failed move item in league inventory[ Move type:%d, Error:%d ]( %d )",
                    psItemMoveForServer.psReqItemMoveInfo.byType,
                    psItemMoveForServer.nErrorCode, 3764);
            }
        };
        CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, func);

        // Per IDA lambda192 (0x140427E60): 任务完成递减计数
        std::function<void()> funcDec = [pReqUser]() {
            pReqUser->DecrementJobCount();
        };
        CLogicThreadManager::Instance().DoJob(pReqUser->GetMapInsID().nMapID, funcDec);
    }

    if (psItemMoveForServer.nErrorCode == 0) {
        // Per IDA lambda185 (0x140211090): 全线程广播公会仓库移动
        CLogicThreadManager::Instance().DoJobAllThread(
            [dwReqUCID, psItemMoveForServer]() {
                ThreadLocalData* pLocalData = ThreadLocalData::GetInstance();
                if (pLocalData) {
                    PS_ITEM_MOVE_LEAGUE_INVEN_FOR_GAME psLocal = psItemMoveForServer;
                    pLocalData->SendLeagueInventoryMove(dwReqUCID, psLocal);
                }
            });
    }
    return true;
}

