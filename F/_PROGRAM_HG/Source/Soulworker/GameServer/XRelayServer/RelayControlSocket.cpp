#include "Soulworker/GameServer/XRelayServer/RelayControlSocket.h"

#include "Soulworker/Common/XNet/XUtil/TXSingleton.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/IXObject.h"
#include "Soulworker/GameServer/XRelayServer/ForceManager.h"
#include "Soulworker/GameServer/XRelayServer/ForceMatching.h"
#include "Soulworker/GameServer/XRelayServer/FriendProcess.h"
#include "Soulworker/GameServer/XRelayServer/ForceProcess.h"
#include "Soulworker/GameServer/XRelayServer/PartyManager.h"
#include "Soulworker/GameServer/XRelayServer/PartyMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/RelayServer.h"
#include "Soulworker/GameServer/XRelayServer/ModeMazeMatchingMgr.h"
#include "Soulworker/GameServer/XRelayServer/PartyProcess.h"
#include "Soulworker/GameServer/XRelayServer/LeagueProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerModeMazeProcess.h"
#include "Soulworker/GameServer/XRelayServer/ServerProcess.h"
#include "Soulworker/GameServer/XRelayServer/UserProcess.h"
#include "Soulworker/GameServer/XRelayServer/WorldModeProcess.h"

#include <chrono>
#include <cstdio>
#include <memory>
#include <thread>
#include <utility>

namespace {
thread_local std::unique_ptr<CServer> g_relayThreadServer;

CServer* GetRelayProcessServer() {
    if (!g_relayThreadServer) {
        g_relayThreadServer = std::make_unique<CServer>();
    }
    return g_relayThreadServer.get();
}
}

// 对齐 IDA 0x1400FE050: SetMyInfo(PEAVXOption) = 非const指针
void XRelaySocket::SetMyInfo(XOption* option) {
    if (!option) {
        m_myInfo = {};
        m_relayInfo = {};
        return;
    }

    m_myInfo.dwID = option->GetServerID();
    m_myInfo.nGroup = option->GetGroupID();
    m_myInfo.nType = option->GetType();
    m_myInfo.nChannel = option->GetChannel();
    m_myInfo.nState = 1;
    m_myInfo.nCurUser = 0;
    m_myInfo.nMaxUser = option->GetMaxConnect();
    m_myInfo.sThreadCount = option->GetLogicThread();
    m_myInfo.sPort = static_cast<std::uint16_t>(option->GetPort());
    m_myInfo.nMazeCount = 0;
    m_myInfo.nNPCCount = 0;
#ifdef _WIN32
    strcpy_s(m_myInfo.szPrivateIP, sizeof(m_myInfo.szPrivateIP), option->GetIP());
    strcpy_s(m_myInfo.szPublicIP, sizeof(m_myInfo.szPublicIP), option->GetPublicIP());
    strcpy_s(m_myInfo.szName, sizeof(m_myInfo.szName), option->GetName());
    sprintf_s(m_relayInfo.szName, "CONTROL");
#else
    std::strncpy(m_myInfo.szPrivateIP, option->GetIP(), sizeof(m_myInfo.szPrivateIP) - 1);
    std::strncpy(m_myInfo.szPublicIP, option->GetPublicIP(), sizeof(m_myInfo.szPublicIP) - 1);
    std::strncpy(m_myInfo.szName, option->GetName(), sizeof(m_myInfo.szName) - 1);
    std::snprintf(m_relayInfo.szName, sizeof(m_relayInfo.szName), "%s", "CONTROL");
#endif

    m_relayInfo.nState = 0;
    std::uint16_t relayPort = 0;
    if (option->GetServerPrivateIPAndPort(m_relayInfo.szName, m_relayInfo.szPrivateIP, relayPort)) {
        m_relayInfo.sPort = static_cast<std::int16_t>(relayPort);
    } else {
        m_relayInfo.sPort = 0;
    }
}

// 对齐 IDA 0x1400FDF90: Init(enum, char*, unsigned short)
// 实际类型：注意基类 TXDBSocket 使用 const char*，IDA 显示非const，暂保持 const 兼容基类
bool XRelaySocket::Init(E_POOL_ID poolId, const char* ip, std::uint16_t port) {
    m_myInfo = {};
    m_relayInfo = {};
    m_nSyncServerData = 0;
    return XIOCPClient::Init(poolId, ip, port);
}

bool XRelaySocket::Connect() {
    return XIOCPClient::Connect(m_relayInfo.szPrivateIP,
                                static_cast<std::uint16_t>(m_relayInfo.sPort));
}

void XRelaySocket::OnConnect() {
    m_nSyncServerData = 0;
    SendAddServer();
}

void XRelaySocket::OnDisConnect() {
    m_relayInfo.nState = 0;
    LogHelper::LogInfo("game.relay", "Relay Client DisConnect");
    m_eState = eConnectStateDisconnected;
}

void XRelaySocket::OnNotConnect() {
    LogHelper::LogInfo("game.relay", "NotConnect to Relay! ");
}

bool XRelaySocket::OnParse(XPacket& xPacket) {
    switch (xPacket.GetMainCmd()) {
    case eCMD_SERVER:
        return ServerProcess(xPacket);
    case 0xF3: {
        // 对齐 IDA: OnParse 通过虚函数分派，这里临时创建 process 保持功能
        CUserProcess userProcess;
        userProcess.Init(GetRelayProcessServer());
        return userProcess.Parse(xPacket);
    }
    case 0xF4: {
        CPartyProcess partyProcess;
        partyProcess.Init(GetRelayProcessServer());
        return partyProcess.Parse(xPacket);
    }
    case 0xF5: {
        CFriendProcess friendProcess;
        friendProcess.Init(GetRelayProcessServer());
        return friendProcess.Parse(xPacket);
    }
    case 0xF6: {
        CLeagueProcess leagueProcess;
        leagueProcess.Init(GetRelayProcessServer());
        return leagueProcess.Parse(xPacket);
    }
    case 0xFA: {
        CForceProcess forceProcess;
        forceProcess.Init(GetRelayProcessServer());
        return forceProcess.Parse(xPacket);
    }
    case 0xFB: {
        CServerWorldModeProcess worldModeProcess;
        worldModeProcess.Init(GetRelayProcessServer());
        return worldModeProcess.Parse(xPacket);
    }
    case 0xFD: {
        CServerModeMazeProcess modeMazeProcess;
        modeMazeProcess.Init(GetRelayProcessServer());
        return modeMazeProcess.Parse(xPacket);
    }
    default:
        return false;
    }
}

bool XRelaySocket::ServerProcess(XPacket& xPacket) {
    switch (xPacket.GetSubCmd()) {
    case eSUB_CMD_SERVER_UPDATE:
        return RecvServerUpdate(xPacket);
    case eSUB_CMD_SERVER_FROM_RELAY:
        RecvPacketFromRelay(xPacket);
        return true;
    case eSUB_CMD_SERVER_CHANGE_CHANNEL:
        RecvChangeChannelRes(xPacket);
        return true;
    case eSUB_CMD_SERVER_CHANNEL_UPDATE_ALL:
        return RecvUpdateChannelAll(xPacket);
    case eSUB_CMD_SERVER_CHANNEL_UPDATE:
        return RecvUpdateChannel(xPacket);
    default:
        // 对齐 IDA: default 分支直接调用 ServerProcessEx，不创建 CServerProcess
        return ServerProcessEx(xPacket);
    }
}

bool XRelaySocket::ServerProcessEx(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

void XRelaySocket::OnStartThread() {
    using namespace std::chrono;

    while (m_eState != eConnectStateShutdown) {
        const steady_clock::time_point begin = steady_clock::now();
        if (m_bInit) {
            GreenDamTan_PumpSocketRecv();
            XIOCPClient::Parsing();
        }
        const auto elapsed = duration_cast<milliseconds>(steady_clock::now() - begin);
        if (elapsed < milliseconds(15)) {
            std::this_thread::sleep_for(milliseconds(15) - elapsed);
        }
    }
}

bool XRelaySocket::RecvServerUpdate(XPacket& xPacket) {
    SS_UPDATE_SERVER_INFO updateInfo{};
    xPacket >> updateInfo;
    m_relayInfo.nState = static_cast<int>(updateInfo.nState);
    return true;
}

// 对齐 IDA: 来自其他 Relay 的包转发
void XRelaySocket::RecvPacketFromRelay(XPacket& xPacket) {
    // 对齐 IDA: 转发到所有 GameServer
    XSendPacket sendPacket(xPacket.GetMainCmd(), xPacket.GetSubCmd());
    // 复制包体数据（PACKET_ROOT 之后的部分）
    const std::uint16_t srcPayloadSize = xPacket.GetPayloadSize();
    if (srcPayloadSize > sizeof(PACKET_ROOT)) {
        const std::uint16_t bodySize = static_cast<std::uint16_t>(srcPayloadSize - sizeof(PACKET_ROOT));
        const char* srcBody = xPacket.GetPayloadBuffer() + sizeof(PACKET_ROOT);
        char* dstBody = sendPacket.GetPayloadBuffer() + sizeof(PACKET_ROOT);
        std::memcpy(dstBody, srcBody, bodySize);
        sendPacket.XParse.SetUsIndex(static_cast<short>(srcPayloadSize));
    }
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(sendPacket);
}

// 对齐 IDA: 切换频道响应
bool XRelaySocket::RecvChangeChannelRes(XPacket& xPacket) {
    // 对齐 IDA 0x1400FE380: 切换频道结果处理
    PS_RES_CHANGE_SERVER changeRes{};
    xPacket >> changeRes;

    // 查找目标用户并转发响应
    const std::shared_ptr<CUserObject> userInfo =
        TXSingleton<XRelayServer>::Instance()->GetUserByUAID(changeRes.dwUAID);
    if (!userInfo) {
        LogHelper::LogDebug("game.relay",
                            "<RecvChangeChannelRes> User not found: %u",
                            static_cast<unsigned int>(changeRes.dwUAID));
        return true;
    }

    XSendPacket sendPacket(0xF3u, 0x12u);
    sendPacket << changeRes;
    userInfo->SendPacket(sendPacket);
    return true;
}

// 对齐 IDA: 更新所有频道信息
bool XRelaySocket::RecvUpdateChannelAll(XPacket& xPacket) {
    // 对齐 IDA: 转发频道信息到所有 GameServer
    XSendPacket sendPacket(xPacket.GetMainCmd(), xPacket.GetSubCmd());
    // 复制包体数据
    const std::uint16_t srcPayloadSize = xPacket.GetPayloadSize();
    if (srcPayloadSize > sizeof(PACKET_ROOT)) {
        const std::uint16_t bodySize = static_cast<std::uint16_t>(srcPayloadSize - sizeof(PACKET_ROOT));
        const char* srcBody = xPacket.GetPayloadBuffer() + sizeof(PACKET_ROOT);
        char* dstBody = sendPacket.GetPayloadBuffer() + sizeof(PACKET_ROOT);
        std::memcpy(dstBody, srcBody, bodySize);
        sendPacket.XParse.SetUsIndex(static_cast<short>(srcPayloadSize));
    }
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(sendPacket);
    return true;
}

// 对齐 IDA: 更新单个频道信息
bool XRelaySocket::RecvUpdateChannel(XPacket& xPacket) {
    // 对齐 IDA: 转发频道信息到所有 GameServer
    XSendPacket sendPacket(xPacket.GetMainCmd(), xPacket.GetSubCmd());
    // 复制包体数据
    const std::uint16_t srcPayloadSize = xPacket.GetPayloadSize();
    if (srcPayloadSize > sizeof(PACKET_ROOT)) {
        const std::uint16_t bodySize = static_cast<std::uint16_t>(srcPayloadSize - sizeof(PACKET_ROOT));
        const char* srcBody = xPacket.GetPayloadBuffer() + sizeof(PACKET_ROOT);
        char* dstBody = sendPacket.GetPayloadBuffer() + sizeof(PACKET_ROOT);
        std::memcpy(dstBody, srcBody, bodySize);
        sendPacket.XParse.SetUsIndex(static_cast<short>(srcPayloadSize));
    }
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(sendPacket);
    return true;
}

// 对齐 IDA 0x1400FE430 sub=7: 踢出用户
bool XRelaySocket::RecvUserKickout(XPacket& xPacket) {
    // 对齐 IDA: ControlServer 发送 PS_KICK_USER_INFO (含 dwUAID)
    PS_KICK_USER_INFO psKick{};
    xPacket >> psKick;

    // 通过 UAID 查找用户获取 UCID
    const std::shared_ptr<CUserObject> userInfo =
        TXSingleton<XRelayServer>::Instance()->GetUserByUAID(psKick.dwUAID);
    if (!userInfo) {
        LogHelper::LogDebug("game.relay",
                            "<RecvUserKickout> User not found: %u",
                            static_cast<unsigned int>(psKick.dwUAID));
        return true;
    }

    // 调用 KickOutUser 转发到 GameServer
    TXSingleton<XRelayServer>::Instance()->KickOutUser(userInfo->GetCID(), psKick.byKickType);
    return true;
}

// 对齐 IDA 0x1400FE430 sub=16: 私聊响应
bool XRelaySocket::RecvUserWhisperRes(XPacket& xPacket) {
    // 对齐 IDA: 转发私聊响应到目标用户
    std::uint32_t matchingID = 0;
    PS_CHAT_WHISPER chatWhisper{};
    std::uint8_t byResult = 0;
    PS_CHAT_ITEM_LINK_FOR_SERVER itemLinkInfo{};

    xPacket.XParse >> matchingID;
    xPacket >> chatWhisper;
    xPacket.XParse >> byResult;
    xPacket >> itemLinkInfo;

    // 查找目标用户（接收者）并转发
    const std::shared_ptr<CUserObject> targetUser =
        TXSingleton<XRelayServer>::Instance()->GetUser(chatWhisper.strReciver);
    if (!targetUser) {
        LogHelper::LogDebug("game.relay",
                            "<RecvUserWhisperRes> Target not found: %ls",
                            chatWhisper.strReciver);
        return true;
    }

    XSendPacket sendPacket(0xF3u, 0x10u);
    sendPacket.XParse << matchingID;
    sendPacket << chatWhisper;
    sendPacket.XParse << byResult;
    sendPacket << itemLinkInfo;
    targetUser->SendPacket(sendPacket);
    return true;
}

// 对齐 IDA 0x1400FE430 sub=17: 用户通知广播
bool XRelaySocket::RecvUserNotice(XPacket& xPacket) {
    // 对齐 IDA: ControlServer 发送 PS_CHAT_NOTICE 广播
    PS_CHAT_NOTICE chatNotice{};
    xPacket >> chatNotice;

    // 转发到所有 GameServer
    TXSingleton<XRelayServer>::Instance()->SendChatNotice(chatNotice);
    return true;
}

// 对齐 IDA 0x1400FE430 sub=18: 用户切换服务器
bool XRelaySocket::RecvUserChangeServer(XPacket& xPacket) {
    // 对齐 IDA: 服务器切换响应
    PS_RES_CHANGE_SERVER changeRes{};
    xPacket >> changeRes;

    // 查找目标用户并转发
    const std::shared_ptr<CUserObject> userInfo =
        TXSingleton<XRelayServer>::Instance()->GetUserByUAID(changeRes.dwUAID);
    if (!userInfo) {
        LogHelper::LogDebug("game.relay",
                            "<RecvUserChangeServer> User not found: %u",
                            static_cast<unsigned int>(changeRes.dwUAID));
        return true;
    }

    XSendPacket sendPacket(0xF3u, 0x12u);
    sendPacket << changeRes;
    userInfo->SendPacket(sendPacket);
    return true;
}

// 对齐 IDA 0x1400FE430 sub=20: 用户进入服务器
bool XRelaySocket::RecvUserEnterServer(XPacket& xPacket) {
    // 对齐 IDA: 用户进入服务器通知
    ST_ENTER_SERVER enterServer{};
    xPacket >> enterServer;

    // 查找目标用户并转发
    const std::shared_ptr<CUserObject> userInfo =
        TXSingleton<XRelayServer>::Instance()->GetUserByUAID(enterServer.dwUAID);
    if (!userInfo) {
        LogHelper::LogDebug("game.relay",
                            "<RecvUserEnterServer> User not found: %u",
                            static_cast<unsigned int>(enterServer.dwUAID));
        return true;
    }

    XSendPacket sendPacket(0xF3u, 0x14u);
    sendPacket << enterServer;
    userInfo->SendPacket(sendPacket);
    return true;
}

// 对齐 IDA 0x1400FE430 sub=23: 大喇叭广播
bool XRelaySocket::RecvUserMegaPhone(XPacket& xPacket) {
    // 对齐 IDA: ControlServer 发送 PS_CHAT_MEGAPHONE 广播
    PS_CHAT_MEGAPHONE megaPhone{};
    PS_CHAT_ITEM_LINK_FOR_SERVER itemLinkInfo{};

    xPacket >> megaPhone;
    xPacket >> itemLinkInfo;

    // 转发到所有 GameServer
    TXSingleton<XRelayServer>::Instance()->SendChatMegaPhone(megaPhone, itemLinkInfo);
    return true;
}

// 对齐 IDA 0x1400FE430 sub=39: 交易密码状态
bool XRelaySocket::RecvUserTradePasswordState(XPacket& xPacket) {
    // 对齐 IDA: 交易密码状态更新
    std::uint32_t dwUCID = 0;
    std::uint8_t byState = 0;
    xPacket.XParse >> dwUCID;
    xPacket.XParse >> byState;

    // 查找目标用户并转发
    const std::shared_ptr<CUserObject> userInfo =
        TXSingleton<XRelayServer>::Instance()->GetUser(dwUCID);
    if (!userInfo) {
        LogHelper::LogDebug("game.relay",
                            "<RecvUserTradePasswordState> User not found: %u",
                            static_cast<unsigned int>(dwUCID));
        return true;
    }

    XSendPacket sendPacket(0xF3u, 0x27u);
    sendPacket.XParse << dwUCID;
    sendPacket.XParse << byState;
    userInfo->SendPacket(sendPacket);
    return true;
}

// 对齐 IDA 0x1400FE430 sub=40: 交易所价格历史
bool XRelaySocket::RecvExchangePriceHistory(XPacket& xPacket) {
    // 对齐 IDA: 交易所价格历史响应
    PS_EXCHANGE_PRICE_HISTORY_RES priceHistory{};
    xPacket >> priceHistory;

    // 查找目标用户并转发
    const std::shared_ptr<CUserObject> userInfo =
        TXSingleton<XRelayServer>::Instance()->GetUser(priceHistory.dwUCID);
    if (!userInfo) {
        LogHelper::LogDebug("game.relay",
                            "<RecvExchangePriceHistory> User not found: %u",
                            static_cast<unsigned int>(priceHistory.dwUCID));
        return true;
    }

    XSendPacket sendPacket(0xF3u, 0x28u);
    sendPacket << priceHistory;
    userInfo->SendPacket(sendPacket);
    return true;
}

// 对齐 IDA 0x1400FE430 sub=48: 交易所邮件
bool XRelaySocket::RecvExchangePost(XPacket& xPacket) {
    // 对齐 IDA: 交易所邮件通知
    std::uint32_t dwUCID = 0;
    xPacket.XParse >> dwUCID;

    // 查找目标用户并转发通知
    const std::shared_ptr<CUserObject> userInfo =
        TXSingleton<XRelayServer>::Instance()->GetUser(dwUCID);
    if (!userInfo) {
        LogHelper::LogDebug("game.relay",
                            "<RecvExchangePost> User not found: %u",
                            static_cast<unsigned int>(dwUCID));
        return true;
    }

    XSendPacket sendPacket(0xF3u, 0x30u);
    sendPacket.XParse << dwUCID;
    userInfo->SendPacket(sendPacket);
    return true;
}

// 对齐 IDA 0x1400FE430 sub=50: 检查会话ID
bool XRelaySocket::RecvCheckSessionID(XPacket& xPacket) {
    // 对齐 IDA: 会话ID检查结果
    std::uint32_t dwUAID = 0;
    std::int64_t biSessionID = 0;
    bool bResult = false;

    xPacket.XParse >> dwUAID;
    xPacket.XParse >> biSessionID;
    xPacket.XParse >> bResult;

    // 查找目标用户并转发
    const std::shared_ptr<CUserObject> userInfo =
        TXSingleton<XRelayServer>::Instance()->GetUserByUAID(dwUAID);
    if (!userInfo) {
        LogHelper::LogDebug("game.relay",
                            "<RecvCheckSessionID> User not found: %u",
                            static_cast<unsigned int>(dwUAID));
        return true;
    }

    XSendPacket sendPacket(0xF3u, 0x32u);
    sendPacket.XParse << dwUAID;
    sendPacket.XParse << biSessionID;
    sendPacket.XParse << bResult;
    userInfo->SendPacket(sendPacket);
    return true;
}

// 对齐 IDA 0x1400FE430 sub=55: GF计费邮件重载
bool XRelaySocket::RecvGFBillingPostReload(XPacket& xPacket) {
    // 对齐 IDA: GF计费邮件重载通知
    // 广播到所有 GameServer
    XSendPacket sendPacket(0xF3u, 0x37u);
    TXSingleton<XRelayServer>::Instance()->SendPacketAll(sendPacket);

    LogHelper::LogDebug("game.relay",
                        "<RecvGFBillingPostReload> Broadcast reload notification");
    static_cast<void>(xPacket);
    return true;
}

void XRelaySocket::SendAddServer() {
    m_myInfo.nState = 1;
    XSendPacket sendPacket(eCMD_SERVER, eSUB_CMD_SERVER_ADD);
    sendPacket << m_myInfo;
    XIOCPClient::Send(sendPacket);
}

// 对齐 IDA 0x1400FE6E0: SendUpdateServerInfo(H, H) = (int, int)
void XRelaySocket::SendUpdateServerInfo(int nState, int nUserCount) {
    if (static_cast<float>(nUserCount) >= static_cast<float>(m_myInfo.nMaxUser) * 0.95f) {
        nState = 3;
    }

    SS_UPDATE_SERVER_INFO updateInfo{};
    updateInfo.dwID = m_myInfo.dwID;
    updateInfo.nState = static_cast<std::uint16_t>(nState);
    m_myInfo.nCurUser = nUserCount;
    updateInfo.nCurUser = static_cast<std::uint16_t>(nUserCount);

    XSendPacket sendPacket(eCMD_SERVER, eSUB_CMD_SERVER_UPDATE);
    sendPacket << updateInfo;
    XIOCPClient::Send(sendPacket);
}

bool XRelaySocket::IsReady() const {
    return XIOCPClient::IsConnection();
}

// 对齐 IDA: SetMyInfo(PEAVXOption) = 非const指针
void CRelayControlSocket::SetMyInfo(XOption* option) {
    XRelaySocket::SetMyInfo(option);
}

bool CRelayControlSocket::ServerProcessEx(XPacket& xPacket) {
    LogHelper::LogDebug("game.relay",
                        "GreenDamTan_log RelayControlSocket.cpp::CRelayControlSocket::ServerProcessEx sub=%u",
                        static_cast<unsigned int>(xPacket.GetSubCmd()));
    switch (xPacket.GetSubCmd()) {
    case 'D':
        return ResCreateMatchingMaze(xPacket);
    case 'E':
        return SyncPartyMazeInfo(xPacket);
    case 'F':
        return SyncForceMazeInfo(xPacket);
    case 'J':
        return ResCreateMatchingModeMaze(xPacket);
    default:
        return false;
    }
}

bool CRelayControlSocket::DispatchLogicJob(std::int64_t instanceID, const std::function<void()>& job) {
    if (!job) {
        return false;
    }
    return CLogicThreadManager::Instance().DoJob(instanceID, job);
}

bool CRelayControlSocket::ResCreateMatchingMaze(XPacket& xPacket) {
    std::uint32_t matchingID = 0;
    ST_CREATE_MAZE createMaze{};
    PS_PARTY_INFO partyInfo{};
    PS_FORCE_INFO forceInfo{};

    xPacket.XParse >> matchingID;
    xPacket >> createMaze;
    xPacket >> partyInfo;
    xPacket >> forceInfo;

    // 对齐 IDA 0x14003CFC0: DoJob(0, ...) 线程索引为 0，而非 matchingID
    return CLogicThreadManager::Instance().DoJob(0,
                            [matchingID, createMaze, partyInfo, forceInfo]() mutable {
                                XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();

                                if (createMaze.stPartyInfo.byGroupType == 1 &&
                                    createMaze.stPartyInfo.nID > 0) {
                                    // Party path
                                    relayServer.GetPartyManager().SetMaze(
                                        static_cast<std::uint32_t>(createMaze.stPartyInfo.nID),
                                        createMaze.uxMapID,
                                        createMaze.uxParentInstanceID);

                                    relayServer.GetPartyMatchingMgr().SendCreateMatchingMaze(
                                        matchingID,
                                        createMaze,
                                        partyInfo);

                                    // DB update 0x04/0x08
                                    IXObject* pObject = nullptr;
                                    XSendDBPacket xSendDBPacket(pObject, 4u, 8u);
                                    xSendDBPacket.XParse << createMaze.stPartyInfo.nID;
                                    xSendDBPacket.XParse << createMaze.uxMapID.nMapID;
                                    xSendDBPacket.XParse << 0;
                                    relayServer.SendDBGame(xSendDBPacket);
                                } else if (createMaze.stPartyInfo.byGroupType == 2 &&
                                           createMaze.stPartyInfo.nID > 0) {
                                    // Force path
                                    relayServer.GetForceManager().SetMaze(
                                        static_cast<std::uint32_t>(createMaze.stPartyInfo.nID),
                                        createMaze.uxMapID,
                                        createMaze.uxParentInstanceID);

                                    relayServer.GetForceMatchingMgr().SendCreateMatchingMaze(
                                        matchingID,
                                        createMaze,
                                        forceInfo);

                                    // DB update 0x04/0x08
                                    IXObject* pObject = nullptr;
                                    XSendDBPacket xSendDBPacket(pObject, 4u, 8u);
                                    xSendDBPacket.XParse << createMaze.stPartyInfo.nID;
                                    xSendDBPacket.XParse << createMaze.uxMapID.nMapID;
                                    xSendDBPacket.XParse << 0;
                                    relayServer.SendDBGame(xSendDBPacket);
                                }

                                LogHelper::LogDebug(
                                    "game.relay",
                                    "GreenDamTan_log RelayControlSocket.cpp::CRelayControlSocket::ResCreateMatchingMaze matchingID=%u createType=%d result=%d partyType=%u forceID=%u",
                                    static_cast<unsigned int>(matchingID),
                                    createMaze.nCreateType,
                                    createMaze.nResult,
                                    static_cast<unsigned int>(partyInfo.byPartyType),
                                    static_cast<unsigned int>(forceInfo.dwForceID));
                            });
}

bool CRelayControlSocket::SyncPartyMazeInfo(XPacket& xPacket) {
    std::uint32_t partyID = 0;
    UXMapID mapID{};
    UXMapID beforeMapID{};

    xPacket.XParse >> partyID;
    xPacket.XParse >> mapID.nMapID;
    xPacket.XParse >> beforeMapID.nMapID;

    // 对齐 IDA 0x14003D6B0: DoJob(0, ...) 线程索引为 0，而非 partyID
    // 对齐 IDA 0x14003D810: lambda1_ 调用 CPartyManager::SetMaze
    return CLogicThreadManager::Instance().DoJob(0,
                            [partyID, mapID, beforeMapID]() {
                                XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
                                relayServer.GetPartyManager().SetMaze(partyID, mapID, beforeMapID);
                            });
}

bool CRelayControlSocket::SyncForceMazeInfo(XPacket& xPacket) {
    std::uint32_t forceID = 0;
    UXMapID mapID{};
    UXMapID beforeMapID{};

    xPacket.XParse >> forceID;
    xPacket.XParse >> mapID.nMapID;
    xPacket.XParse >> beforeMapID.nMapID;

    // 对齐 IDA 0x14003D850: DoJob(0, ...) 线程索引为 0，而非 forceID
    // 对齐 IDA 0x14003D970: lambda2_ 调用 CForceManager::SetMaze
    return CLogicThreadManager::Instance().DoJob(0,
                            [forceID, mapID, beforeMapID]() {
                                XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
                                relayServer.GetForceManager().SetMaze(forceID, mapID, beforeMapID);
                            });
}

bool CRelayControlSocket::ResCreateMatchingModeMaze(XPacket& xPacket) {
    ST_CREATE_MODE_MAZE createModeMaze{};
    xPacket >> createModeMaze;

    // 对齐 IDA 0x14003D9B0: DoJob(0, ...) 线程索引为 0
    // 对齐 IDA 0x14003DB10: lambda3_ 调用 TXSingleton<XRelayServer>::Instance()->m_ModeMazeMatchingMgr
    return DispatchLogicJob(0, [createModeMaze]() mutable {
        XRelayServer& relayServer = *TXSingleton<XRelayServer>::Instance();
        relayServer.GetModeMazeMatchingMgr().SendCreateMatchingModeMaze(createModeMaze);
        LogHelper::LogDebug(
            "game.relay",
            "GreenDamTan_log RelayControlSocket.cpp::CRelayControlSocket::ResCreateMatchingModeMaze matchingID=%u eventRoomID=%u result=%d memberCount=%zu",
            static_cast<unsigned int>(createModeMaze.dwMatchingID),
            static_cast<unsigned int>(createModeMaze.dwEventRoomID),
            createModeMaze.nResult,
            createModeMaze.vecEnterMember.size());
    });
}
