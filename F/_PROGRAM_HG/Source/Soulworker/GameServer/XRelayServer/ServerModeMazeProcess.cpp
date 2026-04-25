#include "Soulworker/GameServer/XRelayServer/ServerModeMazeProcess.h"

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XRelayServer/Thread/LogicThreadProcessor.h"

bool CServerModeMazeProcess::Parse(XPacket& xPacket) {
    switch (xPacket.GetSubCmd()) {
    case 1:
        return ReqServerModeMazeEnter(xPacket);
    case 3:
        return ReqServerModeMazeExit(xPacket);
    case 17:
        return ReqServerModeMazeTime_Cheat(xPacket);
    case 18:
        return ReqServerModeMazeMatchingEvent(xPacket);
    default:
        return true;
    }
}

bool CServerModeMazeProcess::ReqServerModeMazeEnter(XPacket& xPacket) {
    PS_SERVER_MODE_MAZE_MATCHING_ENTER_REQ enterReq{};
    xPacket >> enterReq;

    CServer* server = GetClientPtr();
    CLogicThreadManager::Instance().DoJob(0, [enterReq, server]() mutable {
        if (!CModeMazeMatchingMgr::Instance().EnterMatching(enterReq, server)) {
            LogHelper::LogError("game.relay",
                                "ModeMaze EnterMatching failed actor=%u uaid=%u modeMazeID=%u rank=%u server=%p",
                                static_cast<unsigned int>(enterReq.stMemberInfo.dwActorID),
                                static_cast<unsigned int>(enterReq.stMemberInfo.dwUAID),
                                static_cast<unsigned int>(enterReq.wModeMazeID),
                                static_cast<unsigned int>(enterReq.wRank),
                                static_cast<void*>(server));
        }
    });
    return true;  // 对齐 IDA: 始终返回 1
}

bool CServerModeMazeProcess::ReqServerModeMazeExit(XPacket& xPacket) {
    PS_MODE_MAZE_MATCHING_EXIT exitInfo{};
    xPacket >> exitInfo;

    CLogicThreadManager::Instance().DoJob(0, [exitInfo]() mutable {
        CModeMazeMatchingMgr::Instance().ExitMatching(exitInfo);
    });
    return true;  // 对齐 IDA: 始终返回 1
}

bool CServerModeMazeProcess::ReqServerModeMazeTime_Cheat(XPacket& xPacket) {
    int id = 0;
    int start = 0;
    int end = 0;
    xPacket.XParse >> id;
    xPacket.XParse >> start;
    xPacket.XParse >> end;

    CServer* server = GetClientPtr();
    CLogicThreadManager::Instance().DoJob(0, [id, start, end, server]() {
        LogHelper::LogDebug("game.relay",
                            "GreenDamTan_log ServerModeMazeProcess.cpp::CServerModeMazeProcess::ReqServerModeMazeTime_Cheat id=%d start=%d end=%d server=%p",
                            id,
                            start,
                            end,
                            static_cast<void*>(server));
    });
    return true;  // 对齐 IDA: 始终返回 1
}

bool CServerModeMazeProcess::ReqServerModeMazeMatchingEvent(XPacket& xPacket) {
    PS_SERVER_MODE_MAZE_MATCHING_EVENT eventInfo{};
    xPacket >> eventInfo;

    CLogicThreadManager::Instance().DoJob(0, [eventInfo]() mutable {
        CModeMazeMatchingMgr::Instance().ModeMazeMatchingEvent(eventInfo);
    });
    return true;  // 对齐 IDA: 始终返回 1
}
