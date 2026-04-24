#include "Soulworker/GameServer/XLoginServer/LoginControlSocket.h"

namespace {
/**
 * @brief 兼容 `GetTickCount64` 语义的毫秒计数。
 * @return 当前稳态时钟毫秒数。
 */
std::uint64_t GetTickCount64Compat() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

std::int32_t ExtractMapIDFromUXMapID(const UXMapID& mapID) {
    return static_cast<std::int32_t>(mapID.parts.mapID);
}
}

// 对齐 IDA: SetMyInfo(PEAVXOption) = 非const指针
void CLoginControlSocket::SetMyInfo(XOption* option) {
    XRelaySocket::SetMyInfo(option);
}

bool CLoginControlSocket::ServerProcessEx(XPacket& xPacket) {
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginControlSocket.cpp::CLoginControlSocket::ServerProcessEx sub=%u",
                        static_cast<unsigned int>(xPacket.GetSubCmd()));
    switch (xPacket.GetSubCmd()) {
    case 0x08:
        return RecvServerShutDown(xPacket);
    case 0x22:
        return RecvCreateMazeRes(xPacket);
    case 0x31:
        return RecvEnterServer(xPacket);
    case 0x61:
        return RecvServerOptionUpdate(xPacket);
    case 0x71:
        return RecvMaxServerUserCount(xPacket);
    default:
        return true;
    }
}

/**
 * @brief 处理控制服返回的 SessionID 校验结果。
 *
 * 关键步骤：
 * 1. 读取 UAID 和校验结果值。
 * 2. 从在线表中找出当前用户并记录耗时日志。
 * 3. 成功时清理临时状态并向 GameDB 发送 `main=3, sub=1`。
 * 4. 失败时按错误类型发错误码并踢线。
 *
 * @param xPacket 控制服返回包。
 * @return 处理完成返回 true；找不到用户时也视为已消费。
 */
bool CLoginControlSocket::RecvCheckSessionID(XPacket& xPacket) {
    int uaid = 0;
    std::uint8_t byCheckResult = 0;
    xPacket.XParse >> uaid;
    xPacket.XParse >> byCheckResult;

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    CUser* user = loginServer->FindUIDToUser(uaid);
    if (!user) {
        LogHelper::LogError("game.contents", "<LOGIN> RecvCheckSessionID Not FindUIDToUser [UAID : %d]", uaid);
        return true;
    }

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginControlSocket.cpp::CLoginControlSocket::RecvCheckSessionID uaid=%d result=%u session=%d socket=%lld user=%p",
                        uaid,
                        static_cast<unsigned int>(byCheckResult),
                        user->GetSessionID(),
                        static_cast<long long>(user->Socket),
                        static_cast<void*>(user));
    TXSingleton<XLoginServer>::Instance()->GetControlSocket().GreenDamTan_ClearPendingCheckSession(
        static_cast<unsigned int>(uaid));

    const std::uint64_t now = GetTickCount64Compat();
    const std::uint64_t elapsedMs = now - user->GetSendCheckSessionID();
    LogHelper::LogError("game.system",
                        "CheckSessionID Tick [UAID : %d, Tick : %llu]",
                        uaid,
                        static_cast<unsigned long long>(elapsedMs));

    if (byCheckResult == 0) {
        // 校验成功后继续驱动进入 GameDB 的下一阶段初始化。
        user->SetEchelonLevel(0);
        user->SetEchelonExp(0);
        user->ClearLeagueInfo();

        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log LoginControlSocket.cpp::CLoginControlSocket::RecvCheckSessionID success-path uaid=%d session=%d enterState=%d elapsedMs=%llu",
                            user->GetUAID(),
                            user->GetSessionID(),
                            static_cast<int>(user->GetEnterServerState()),
                            static_cast<unsigned long long>(elapsedMs));

        XSendDBPacket sendPacket(user, 3, 1);
        sendPacket.XParse << user->GetUAID();
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log LoginControlSocket.cpp::CLoginControlSocket::RecvCheckSessionID->SendDBGame main=3 sub=1 uaid=%d session=%d",
                            user->GetUAID(),
                            user->GetSessionID());
        loginServer->SendDBGame(sendPacket);
        return true;
    }

    PS_KICK_USER_INFO kickInfo{};
    kickInfo.dwUAID = static_cast<unsigned int>(uaid);
    if (byCheckResult == 2) {
        user->SendErrorMessage(3, 0x12, 0xC35A);
        kickInfo.byKickType = 13;
    } else {
        kickInfo.byKickType = 1;
    }
    user->Kickout(kickInfo);
    return true;
}

/**
 * @brief 处理控制服下发的停服通知。
 *
 * 根据 IDA 中 `CLoginControlSocket::RecvServerShutDown(0x140016370)`，原版当前只做两件事：
 * 1. `SetServerAcceptClosed(true)`
 * 2. `KickoutAll(0x0B)`
 */
bool CLoginControlSocket::RecvServerShutDown(XPacket& xPacket) {
    static_cast<void>(xPacket);

    if (XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance()) {
        loginServer->SetServerAcceptClosed(true);
        loginServer->KickoutAll(0x0B);
    }
    return true;
}

/**
 * @brief 处理控制服返回的踢线通知。
 *
 * 根据 IDA 中 `CLoginControlSocket::RecvUserKickout(0x140016347)`，当前已恢复：
 * 1. 读取 `PS_KICK_USER_INFO`
 * 2. 按 `dwUAID` 回查当前在线连接
 * 3. 命中时直接调用 `CUser::Kickout`
 * 4. 无论是否命中都视为该控制包已消费
 */
bool CLoginControlSocket::RecvUserKickout(XPacket& xPacket) {
    PS_KICK_USER_INFO kickInfo{};
    xPacket >> kickInfo;

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    if (!loginServer) {
        return true;
    }

    if (CUser* user = loginServer->FindUIDToUser(static_cast<int>(kickInfo.dwUAID))) {
        user->Kickout(kickInfo);
    }
    return true;
}

/**
 * @brief 处理控制服返回的建图结果。
 *
 * 根据 IDA 中 `CLoginControlSocket::RecvCreateMazeRes(0x1400160c6)`，当前已恢复：
 * 1. 读取 `PS_ENTER_MAP_RES`
 * 2. 按 `dwUserID` 回查当前选中的角色对象
 * 3. 失败时记录 `<MAZE>` 日志并清理 `eStateChangeServer`
 * 4. 成功时把 `byChangeType` 强制收束到 `CHANGE_SERVER_TYPE_ENTER_MAP`
 * 5. 向 GameDB 发送 `main=3, sub=0x42`，并补 `0 / 0` 两个旧地图参数
 * 6. 同步发送 `ST_STATISTICS_MAP_SAVE` 到 StatisticsDB `main=0xF0, sub=0x12`
 */
bool CLoginControlSocket::RecvCreateMazeRes(XPacket& xPacket) {
    PS_ENTER_MAP_RES enterMapResult{};
    xPacket >> enterMapResult;

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginControlSocket.cpp::CLoginControlSocket::RecvCreateMazeRes ucid=%u result=%d changeType=%u map=%llu",
                        static_cast<unsigned int>(enterMapResult.dwUserID),
                        enterMapResult.nResult,
                        static_cast<unsigned int>(enterMapResult.byChangeType),
                        static_cast<unsigned long long>(enterMapResult.uxMapID.nMapID));

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    CUser* requestUser = loginServer ? loginServer->FindActor(enterMapResult.dwUserID) : nullptr;
    if (!requestUser) {
        LogHelper::LogError("game.relay", "<MAZE> Cant Find %d User", enterMapResult.dwUserID);
        return false;
    }

    if (enterMapResult.nResult != 0) {
        LogHelper::LogError("game.relay",
                            "<MAZE> Recv from failed server ( %d / %d )",
                            ExtractMapIDFromUXMapID(enterMapResult.uxMapID),
                            enterMapResult.nResult);
        requestUser->ClearState(eStateChangeServer);
        return false;
    }

    enterMapResult.byChangeType = CHANGE_SERVER_TYPE_ENTER_MAP;

    XSendDBPacket sendPacket(static_cast<IXObject*>(requestUser), 3, 0x42);
    sendPacket << enterMapResult;
    sendPacket.XParse << 0;
    sendPacket.XParse << 0;
    loginServer->SendDBGame(sendPacket);

    ST_STATISTICS_MAP_SAVE statisticsInfo{};
    statisticsInfo.dwUCID = enterMapResult.dwUserID;
    statisticsInfo.dwMapID = static_cast<std::uint32_t>(ExtractMapIDFromUXMapID(enterMapResult.uxMapID));
    statisticsInfo.dwServerID = static_cast<std::uint32_t>(loginServer->GetOption().GetChannel());

    XSendDBPacket statisticsPacket(static_cast<IXObject*>(requestUser), 0xF0, 0x12);
    statisticsPacket << statisticsInfo;
    loginServer->SendDBStatistics(statisticsPacket);
    return true;
}

/**
 * @brief 处理控制服返回的“继续切服”结果。
 *
 * 根据 IDA 中 `CLoginControlSocket::RecvUserChangeServer(0x140016670)`，当前已恢复：
 * - `byType != 0` 时切到 `eStateGoBackLobby`
 * - `byType == 0` 时切到 `eStateGoBackAuth`
 * - 始终向客户端回 `main=3, sub=0x60`
 */
bool CLoginControlSocket::RecvUserChangeServer(XPacket& xPacket) {
    PS_RES_CHANGE_SERVER changeServer{};
    xPacket >> changeServer;

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    CUser* user = loginServer ? loginServer->FindUIDToUser(static_cast<int>(changeServer.dwUAID))
                              : nullptr;
    if (!user) {
        LogHelper::LogError("game.relay",
                            "<FAILED> Cant Find User %d ( RecvUserChangeServer ) ",
                            changeServer.dwUAID);
        return false;
    }

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginControlSocket.cpp::CLoginControlSocket::RecvUserChangeServer session=%d uaid=%u actor=%u byType=%u result=%d target=%s:%d user=%p beforeState=0x%X enterState=%d secondPW=%u tradePW=%u",
                        user->GetSessionID(),
                        changeServer.dwUAID,
                        changeServer.dwActorID,
                        static_cast<unsigned int>(changeServer.byType),
                        changeServer.bResult ? 1 : 0,
                        changeServer.szIP,
                        static_cast<int>(changeServer.sPort),
                        static_cast<void*>(user),
                        static_cast<unsigned int>(user->m_eNetState),
                        static_cast<int>(user->GetEnterServerState()),
                        static_cast<unsigned int>(user->GetSecondPWState()),
                        static_cast<unsigned int>(user->GetTradePWState()));

    if (changeServer.byType != 0) {
        user->SetState(eStateGoBackLobby);
    } else {
        user->SetState(eStateGoBackAuth);
    }

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginControlSocket.cpp::CLoginControlSocket::RecvUserChangeServer after-state session=%d uaid=%u byType=%u state=0x%X goBackLobby=%d goBackAuth=%d change=%d secondPW=%u tradePW=%u",
                        user->GetSessionID(),
                        changeServer.dwUAID,
                        static_cast<unsigned int>(changeServer.byType),
                        static_cast<unsigned int>(user->m_eNetState),
                        user->IsState(eStateGoBackLobby) ? 1 : 0,
                        user->IsState(eStateGoBackAuth) ? 1 : 0,
                        user->IsState(eStateChangeServer) ? 1 : 0,
                        static_cast<unsigned int>(user->GetSecondPWState()),
                        static_cast<unsigned int>(user->GetTradePWState()));

    if (loginServer) {
        loginServer->GetControlSocket().GreenDamTan_ClearPendingChangeServer(changeServer.dwUAID);
    }

    XSendPacket sendPacket(3, 0x60);
    sendPacket << changeServer;
    user->BridgeSend(sendPacket);
    return true;
}

/**
 * @brief 处理控制服返回的进服/进图结果。
 *
 * 根据 IDA 中 `CLoginControlSocket::RecvEnterServer(0x1400167be)`，当前已恢复：
 * 1. 失败时回客户端 `main=3, sub=0x14`，并把入服状态回退到 `SELECT_WORLD_RES`
 * 2. 成功且 `byChangeType == ENTER_DISTRICT` 时，继续走 GameDB `main=3, sub=0x42`
 * 3. 成功且 `byChangeType == LOGIN` 时，继续走 GameDB `main=3, sub=0x39`
 */
bool CLoginControlSocket::RecvEnterServer(XPacket& xPacket) {
    PS_ENTER_MAP_RES enterMapResult{};
    xPacket >> enterMapResult;

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginControlSocket.cpp::CLoginControlSocket::RecvEnterServer ucid=%u result=%d changeType=%u map=%llu",
                        static_cast<unsigned int>(enterMapResult.dwUserID),
                        enterMapResult.nResult,
                        static_cast<unsigned int>(enterMapResult.byChangeType),
                        static_cast<unsigned long long>(enterMapResult.uxMapID.nMapID));

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    CUser* user = loginServer ? loginServer->FindActor(enterMapResult.dwUserID) : nullptr;
    if (!user) {
        LogHelper::LogInfo("game.relay",
                           "<ENTER_SERVER> Failed RecvUserEnterServer : No User ( UCID : %d / RESULT : %d )",
                           enterMapResult.dwUserID,
                           enterMapResult.nResult);
        return false;
    }

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginControlSocket.cpp::CLoginControlSocket::RecvEnterServer actor-hit ucid=%u session=%d uaid=%d selectUCID=%u pendingSelectUCID=%u lastSelectUCID=%u secondPW=%u tradePW=%u state=0x%X",
                        static_cast<unsigned int>(enterMapResult.dwUserID),
                        user->GetSessionID(),
                        user->GetUAID(),
                        user->GetSelectUCID(),
                        user->GetPendingSelectUCID(),
                        user->GetLastSelectUCID(),
                        static_cast<unsigned int>(user->GetSecondPWState()),
                        static_cast<unsigned int>(user->GetTradePWState()),
                        static_cast<unsigned int>(user->m_eNetState));

    if (enterMapResult.nResult > 0) {
        LogHelper::LogError("game.relay",
                            "<ENTER_SERVER> RecvUserEnterServer : Error ( UCID : %d / RESULT : %d )",
                            enterMapResult.dwUserID,
                            enterMapResult.nResult);
        user->SetEnterServerState(ENTER_SERVER_STATE_SELECT_WORLD_RES);
        user->ClearState(eStateChangeServer);

        XSendPacket sendPacket(3, 0x14);
        sendPacket << enterMapResult;
        user->BridgeSend(sendPacket);
        return false;
    }

    user->SetEnterServerState(ENTER_SERVER_STATE_SELECT_GAME_RES);
    if (enterMapResult.byChangeType == CHANGE_SERVER_TYPE_ENTER_DISTRICT) {
        XSendDBPacket sendPacket(static_cast<IXObject*>(user), 3, 0x42);
        sendPacket << enterMapResult;
        sendPacket.XParse << 0;
        sendPacket.XParse << 0;
        loginServer->SendDBGame(sendPacket);

        ST_STATISTICS_MAP_SAVE statisticsInfo{};
        statisticsInfo.dwUCID = enterMapResult.dwUserID;
        statisticsInfo.dwMapID = static_cast<std::uint32_t>(ExtractMapIDFromUXMapID(enterMapResult.uxMapID));
        statisticsInfo.dwServerID = static_cast<std::uint32_t>(loginServer->GetOption().GetChannel());

        XSendDBPacket statisticsPacket(static_cast<IXObject*>(user), 0xF0, 0x12);
        statisticsPacket << statisticsInfo;
        loginServer->SendDBStatistics(statisticsPacket);
        return true;
    }

    if (enterMapResult.byChangeType == CHANGE_SERVER_TYPE_LOGIN) {
        XSendDBPacket sendPacket(static_cast<IXObject*>(user), 3, 0x39);
        sendPacket << enterMapResult;
        loginServer->SendDBGame(sendPacket);
    }
    return false;
}

/**
 * @brief 处理控制服下发的内容开关更新。
 *
 * 原始逻辑只是把 `PS_CONTENTS_INFO` 解包后转交给 `XLoginServer`，
 * 当前工程保持同样的依赖方向，方便后续把内容开关真正接入登录流程。
 *
 * @param xPacket 控制服发送的内容开关同步包。
 * @return 始终返回 true，表示该控制消息已消费。
 */
bool CLoginControlSocket::RecvServerOptionUpdate(XPacket& xPacket) {
    PS_CONTENTS_INFO contentsInfo{};
    xPacket >> contentsInfo;
    TXSingleton<XLoginServer>::Instance()->SendServerOption_SecondPW(&contentsInfo);
    return true;
}

/**
 * @brief 处理控制服下发的最大在线人数 / 当前在线人数同步。
 *
 * 根据 `CLoginControlSocket::RecvMaxServerUserCount(0x140016b30)`，原版逻辑仅：
 * 1. 顺序读取 `nMaxServerUserCount` 与 `nServerUserCount`
 * 2. 调 `XLoginServer::UpdateMaxUserCount`
 * 3. 返回 true
 */
bool CLoginControlSocket::RecvMaxServerUserCount(XPacket& xPacket) {
    int nMaxServerUserCount = 0;
    int nServerUserCount = 0;
    xPacket.XParse >> nMaxServerUserCount;
    xPacket.XParse >> nServerUserCount;

    TXSingleton<XLoginServer>::Instance()->UpdateMaxUserCount(nMaxServerUserCount, nServerUserCount);
    return true;
}
