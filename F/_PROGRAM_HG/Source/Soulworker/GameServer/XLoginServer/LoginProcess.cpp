#include "Soulworker/GameServer/XLoginServer/LoginProcess.h"

#ifndef GREENDAMTAN_Is_Check_Packet_Version
#define GREENDAMTAN_Is_Check_Packet_Version 0
#endif

namespace {
constexpr int kExpectedPacketVersion = 10000120;
constexpr int kSystemTypeSystemCheck = 99;

/**
 * @brief 按固定长度上限读取一个宽字符串字段。
 * @param packet 输入包。
 * @param maxLen 字符串上限，和反编译中的 `GetWString` 调用保持一致。
 * @param ok 返回读取是否成功。
 * @return 成功时返回宽字符串；失败时返回空串。
 */
std::wstring ReadBoundedWString(XPacket& packet, short maxLen, bool& ok) {
    std::vector<wchar_t> buffer(static_cast<std::size_t>(maxLen), L'\0');
    short outLen = 0;
    packet.XParse.GetWString(buffer.data(), maxLen, outLen);
    ok = (packet.XParse.GetType() == 0);
    if (!ok) {
        packet.XParse.ClearError();
        return {};
    }
    return std::wstring(buffer.data(), static_cast<std::size_t>(outLen));
}
}

/**
 * @brief 按子命令分派客户端请求。
 * @param xPacket 客户端发送的协议包。
 * @return 成功分发返回 true；当前还原中未知子命令直接忽略并返回 true。
 */
bool XLoginProcess::Parse(XPacket& xPacket) {
    CUser* currentUser = GetClientPtr();
    // TODO: 仅做测试用：补齐“选服后第二条连接是否真的把首包送进 XLoginProcess”观察点。
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginProcess.cpp::XLoginProcess::Parse sub=%u session=%d socket=%lld user=%p",
                        static_cast<unsigned int>(xPacket.GetSubCmd()),
                        currentUser ? currentUser->GetSessionID() : -1,
                        currentUser ? static_cast<long long>(currentUser->Socket) : -1LL,
                        static_cast<void*>(currentUser));

    switch (xPacket.GetSubCmd()) {
    case eSUB_CMD_LOGIN_REQ:
        return ReqUserLogin(xPacket);
    case eSUB_CMD_SERVER_LIST_REQ:
        SendServerList(xPacket);
        return true;
    case eSUB_CMD_SERVER_CONNECT_REQ:
        return ReqServerConnect(xPacket);
    case eSUB_CMD_ENTER_SERVER_REQ:
        return ReqEnterServer(xPacket);
    case 0x32:
        return ReqOptionUpdate(xPacket);
    case eSUB_CMD_ENTER_WAIT_NOTIFY:
        return ReqEnterWaitCheck(xPacket);
    case eSUB_CMD_ENTER_WAIT_CANCEL:
        return ReqEnterWaitCancel(xPacket);
    default:
        return true;
    }
}

/**
 * @brief 处理客户端登录请求。
 *
 * 关键步骤：
 * 1. 检查登录服是否关闭受理或处于系统检查模式。
 * 2. 依次读取账号、密码、MAC 三段宽字符串。
 * 3. 校验客户端版本是否与服务器硬编码版本一致。
 * 4. 把客户端状态推进到 `ENTER_SERVER_STATE_LOGIN_REQ`。
 * 5. 原样转发客户端包给 AccountDB，并在尾部追加 IP 与自动邮件时间。
 *
 * @param xPacket 客户端登录请求包。
 * @return 返回 true 表示请求已消费。
 */
bool XLoginProcess::ReqUserLogin(XPacket& xPacket) {
    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    if (loginServer->IsServerAcceptClosed() || loginServer->GetOption().GetSystemType() == kSystemTypeSystemCheck) {
        SendErrorMessage(eSUB_CMD_LOGIN_REQ, 0xC3BB);
        return true;
    }

    bool ok = false;
    const std::wstring authId = ReadBoundedWString(xPacket, 21, ok);
    if (!ok) {
        SendErrorMessage(eSUB_CMD_LOGIN_REQ, 0xC3B6);
        return true;
    }

    const std::wstring password = ReadBoundedWString(xPacket, 21, ok);
    if (!ok) {
        SendErrorMessage(eSUB_CMD_LOGIN_REQ, 0xC3B7);
        return true;
    }

    const std::wstring macAddress = ReadBoundedWString(xPacket, 18, ok);
    if (!ok) {
        SendErrorMessage(eSUB_CMD_LOGIN_REQ, 0xC3B8);
        return true;
    }

    int packetVersion = 0;
    xPacket.XParse >> packetVersion;
#if GREENDAMTAN_Is_Check_Packet_Version
    if (packetVersion > kExpectedPacketVersion) {
        LogHelper::LogError("game.contents", "<ReqUserLogin> Packet Version ( %d, %d )", kExpectedPacketVersion, packetVersion);
        SendErrorMessage(eSUB_CMD_LOGIN_REQ, 0xC382);
        return true;
    }
    if (packetVersion < kExpectedPacketVersion) {
        LogHelper::LogError("game.contents", "<ReqUserLogin> Packet Version ( %d, %d )", kExpectedPacketVersion, packetVersion);
        SendErrorMessage(eSUB_CMD_LOGIN_REQ, 0xC383);
        return true;
    }
#else
    // TODO: 按用户要求默认关闭版本检查，仅保留字段读取以维持包解析偏移。
    (void)packetVersion;
#endif

    CUser* user = GetClientPtr();
    if (!user || user->GetEnterServerState() != ENTER_SERVER_STATE_NONE) {
        SendErrorMessage(eSUB_CMD_LOGIN_REQ, 0xC3BB);
        return true;
    }

    int autoMailTimeValue = 0;
    if (TB_SYSTEMMAIL_ADD* row = loginServer->GetResourceMgr().GetTB_SYSTEMMAIL_ADD(2)) {
        // 表 `TB_SYSTEMMAIL_ADD` 的 2 号记录控制登录后自动邮件的开启与延时。
        if (row->AutoMail_Type_On_Off != 0) {
            autoMailTimeValue = row->AutoMail_Time_Value;
        }
    }

    // 状态先置为 LOGIN_REQ，避免重复登录包并发进入 DB。
    user->SetEnterServerState(ENTER_SERVER_STATE_LOGIN_REQ);

    // 保留原始客户端包体，再在尾部追加来源 IP 与自动邮件延时配置。
    XSendDBPacket sendPacket(user, eCMD_LOGIN, 1, xPacket);
    sendPacket.XParse << static_cast<int>(user->GetIPv4());
    sendPacket.XParse << autoMailTimeValue;
    loginServer->SendDBAccount(sendPacket);

    LogHelper::LogInfo("game.contents",
                       "Queued login request for authId='%ls', passwordLen=%zu, mac='%ls'",
                       authId.c_str(),
                       password.size(),
                       macAddress.c_str());
    return true;
}

/**
 * @brief 请求服务器列表及补充信息。
 *
 * 从反编译看，该函数会针对同一个请求参数向 AccountDB 发两条查询：
 * 1. `main=2, sub=0x14`：服务器列表/入服前主查询。
 * 2. `main=2, sub=0x31`：与服务器列表同时需要的补充查询。
 *
 * @param xPacket 客户端发来的服务器列表请求。
 */
void XLoginProcess::SendServerList(XPacket& xPacket) {
    int requestValue = 0;
    xPacket.XParse >> requestValue;

    CUser* user = GetClientPtr();

    // 第一条 DB 请求：主服务器列表数据。
    XSendDBPacket serverListPacket(user, eCMD_LOGIN, 0x14);
    serverListPacket.XParse << requestValue;
    TXSingleton<XLoginServer>::Instance()->SendDBAccount(serverListPacket);

    // 第二条 DB 请求：补充数据，原程序里使用 sub=0x31。
    XSendDBPacket extraPacket(user, eCMD_LOGIN, 0x31);
    extraPacket.XParse << requestValue;
    TXSingleton<XLoginServer>::Instance()->SendDBAccount(extraPacket);
}

/**
 * @brief 处理客户端切服连接信息请求。
 *
 * 该函数读取客户端请求里的 16 位服务器组 ID，但真正下发给客户端的是：
 * - 当前登录服配置中的公网 IP
 * - 当前登录服配置中的监听端口
 *
 * @param xPacket 客户端切服请求包。
 * @return 成功处理返回 true。
 */
bool XLoginProcess::ReqServerConnect(XPacket& xPacket) {
    std::uint16_t requestedGroupID = 0;
    xPacket.XParse >> requestedGroupID;

    CUser* user = GetClientPtr();
    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    const std::int16_t targetPort = loginServer->GetOption().GetPort();
    const char* targetIP = loginServer->GetOption().GetPublicIP();

    XSendPacket sendPacket(eCMD_LOGIN, eSUB_CMD_SERVER_CONNECT_RES);
    sendPacket.XParse << std::string(targetIP ? targetIP : "");
    sendPacket.XParse << targetPort;

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginProcess.cpp::XLoginProcess::ReqServerConnect session=%d requestedGroup=%u target=%s:%d user=%p",
                        user ? user->GetSessionID() : -1,
                        static_cast<unsigned int>(requestedGroupID),
                        targetIP ? targetIP : "",
                        static_cast<int>(targetPort),
                        static_cast<void*>(user));

    if (user) {
        user->BridgeSend(sendPacket);
        XClient::SetState(user, eStateChangeServer);
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log LoginProcess.cpp::XLoginProcess::ReqServerConnect post-send session=%d uaid=%d clientStateChangeServer=%d enterState=%d",
                            user->GetSessionID(),
                            user->GetUAID(),
                            XClient::IsState(user, eStateChangeServer) ? 1 : 0,
                            static_cast<int>(user->GetEnterServerState()));
    }
    return true;
}

/**
 * @brief 处理进入服务器请求。
 *
 * 关键步骤：
 * 1. 读取 UAID、上次服务器索引、SessionID/票据、是否绕过排队。
 * 2. 校验受理开关和玩家当前入服状态。
 * 3. 若不需要排队，则直接转发 `sub=0x11` 到 AccountDB。
 * 4. 若需要排队，则压入等待队列并视情况返回排队名次。
 *
 * @param xPacket 入服请求包。
 * @return 返回 true 表示请求已消费。
 */
bool XLoginProcess::ReqEnterServer(XPacket& xPacket) {
    int uaid = -1;
    std::uint16_t lastServerIndex = 0;
    std::uint64_t ticketToken = 0;
    bool bypassWait = false;

    xPacket.XParse >> uaid;
    if (uaid <= 0) {
        SendErrorMessage(eSUB_CMD_ENTER_SERVER_REQ, 0xC3B8);
        LogHelper::LogError("game.contents", "if( 0<= nUAID )");
        return true;
    }

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    if (loginServer->IsServerAcceptClosed()) {
        SendErrorMessage(eSUB_CMD_ENTER_SERVER_REQ, 0xC3BB);
        return true;
    }

    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    const int currentUserUAID = user->GetUAID();
    const ENTER_SERVER_STATE state = user->GetEnterServerState();
    if (state != ENTER_SERVER_STATE_LOGIN_RES && state != ENTER_SERVER_STATE_NONE) {
        SendErrorMessage(eSUB_CMD_ENTER_SERVER_REQ, 0xC3BB);
        return true;
    }

    xPacket.XParse >> lastServerIndex;
    xPacket.XParse >> ticketToken;
    xPacket.XParse >> bypassWait;

    // 反编译显示这里会把参数里的服务器索引覆盖成当前登录服分组 ID。
    lastServerIndex = loginServer->GetOption().GetGroupID();

    const bool waitSystemEnabled = loginServer->GetResourceMgr().GetServerContents(E_SERVER_OPTION_WAIT_SYSTEM);
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginProcess.cpp::XLoginProcess::ReqEnterServer session=%d packetUAID=%d userUAID=%d lastServer=%u ticket=%llu bypass=%d waitSystem=%d state=%d",
                        user ? user->GetSessionID() : -1,
                        uaid,
                        currentUserUAID,
                        static_cast<unsigned int>(lastServerIndex),
                        static_cast<unsigned long long>(ticketToken),
                        bypassWait ? 1 : 0,
                        waitSystemEnabled ? 1 : 0,
                        static_cast<int>(state));
    if (bypassWait || !waitSystemEnabled) {
        // 直通路径：不进等待队列，立即向 AccountDB 请求进入世界。
        user->SetEnterServerState(ENTER_SERVER_STATE_SELECT_WORLD_REQ);
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log LoginProcess.cpp::XLoginProcess::ReqEnterServer state-transition session=%d packetUAID=%d userUAID=%d enterState=%d",
                            user->GetSessionID(),
                            uaid,
                            currentUserUAID,
                            static_cast<int>(user->GetEnterServerState()));

        XSendDBPacket sendPacket(user, eCMD_LOGIN, 0x11);
        sendPacket.XParse << uaid;
        sendPacket.XParse << lastServerIndex;
        sendPacket.XParse << ticketToken;
        sendPacket.XParse << bypassWait;
        const bool sendOk = loginServer->SendDBAccount(sendPacket);
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log LoginProcess.cpp::XLoginProcess::ReqEnterServer->SendDBAccount session=%d packetUAID=%d userUAID=%d lastServer=%u ticket=%llu bypass=%d sendOk=%d enterState=%d",
                            user ? user->GetSessionID() : -1,
                            uaid,
                            currentUserUAID,
                            static_cast<unsigned int>(lastServerIndex),
                            static_cast<unsigned long long>(ticketToken),
                            bypassWait ? 1 : 0,
                            sendOk ? 1 : 0,
                            static_cast<int>(user->GetEnterServerState()));
        return true;
    }

    // 排队路径：写入等待字段，再根据当前服务器负载决定是否立即回包提示排位。
    loginServer->PushWaitUser(user, uaid, ticketToken, lastServerIndex);
    const std::int64_t userTicket = user->GetTicket_Wait();
    if (loginServer->CheckUserWaitCountSend(userTicket)) {
        user->SetSendWaitPacket(true);
        const int waitPosition = static_cast<int>(userTicket - loginServer->GetLastEnterWaitTicket());

        XSendPacket waitPacket(eCMD_LOGIN, eSUB_CMD_ENTER_WAIT_NOTIFY);
        waitPacket.XParse << waitPosition;
        user->BridgeSend(waitPacket);

        LogHelper::LogInfo("game.system", "PushWaitUser Send Packet ( UAID : %d )", uaid);
    } else {
        user->SetSendWaitPacket(false);
    }

    return true;
}

/**
 * @brief 同步客户端选项位到 AccountDB。
 *
 * 协议上会读取一个 `ST_OPTION_BIT`，随后把当前 UAID 与该结构一起转发到
 * `main=2, sub=0x32` 的 DB 请求中。
 *
 * @param xPacket 客户端选项同步包。
 * @return 有效客户端返回 true，否则返回 false。
 */
bool XLoginProcess::ReqOptionUpdate(XPacket& xPacket) {
    ST_OPTION_BIT optionBits{};
    xPacket >> optionBits;

    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    XSendDBPacket sendPacket(user, eCMD_LOGIN, 0x32);
    sendPacket.XParse << user->GetUAID();
    sendPacket << optionBits;
    TXSingleton<XLoginServer>::Instance()->SendDBAccount(sendPacket);
    return true;
}

/**
 * @brief 主动查询当前等待名次。
 *
 * 该函数不访问数据库，只根据：
 * - 当前玩家的等待票号
 * - 登录服记录的最后放行票号
 *
 * 计算相对名次并回给客户端。
 *
 * @param xPacket 客户端查询包，当前实现中未使用包体。
 * @return 有效客户端返回 true，否则返回 false。
 */
bool XLoginProcess::ReqEnterWaitCheck(XPacket& xPacket) {
    (void)xPacket;

    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    const int waitPosition = static_cast<int>(user->GetTicket_Wait() - TXSingleton<XLoginServer>::Instance()->GetLastEnterWaitTicket());
    XSendPacket sendPacket(eCMD_LOGIN, eSUB_CMD_ENTER_WAIT_NOTIFY);
    sendPacket.XParse << waitPosition;
    user->BridgeSend(sendPacket);
    return true;
}

/**
 * @brief 取消当前等待队列请求。
 *
 * 原始函数会先读取一个 `ST_OPTION_BIT`，但当前分支并不使用该值；
 * 真正的动作是记录日志并设置 `cancel_wait` 标志，供等待队列处理逻辑后续跳过该玩家。
 *
 * @param xPacket 客户端取消排队包。
 * @return 有效客户端返回 true，否则返回 false。
 */
bool XLoginProcess::ReqEnterWaitCancel(XPacket& xPacket) {
    ST_OPTION_BIT optionBits{};
    xPacket >> optionBits;

    CUser* user = GetClientPtr();
    if (!user) {
        return false;
    }

    LogHelper::LogInfo("game.contents", "ReqEnterWaitCancel ( %d ).", user->GetUAID());
    user->SetCancel_Wait(true);
    return true;
}
