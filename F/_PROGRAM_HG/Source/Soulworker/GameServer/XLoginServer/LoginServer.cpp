#include "Soulworker/GameServer/XLoginServer/LoginServer.h"
#include "Soulworker/GameServer/XLoginServer/GameDBSocket.h"
#include "Soulworker/GameServer/XLoginServer/LoginControlSocket.h"

#include <algorithm>
#include <csignal>
#include <chrono>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cwchar>
#include <filesystem>
#include <random>

#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#undef SetPort
#else
#include <unistd.h>
#endif

namespace {
constexpr std::uint8_t kCharacterMainCmd = 3;
constexpr std::uint8_t kLogDBMainCmd = 0x42;
constexpr std::uint8_t kLogDBBasicSubCmd = 0;
constexpr std::uint8_t kLogDBStructSubCmd = 1;
constexpr std::uint8_t kRelayEnterServerMainCmd = 0xF2;
constexpr std::uint8_t kRelayEnterServerSubCmd = 0x31;
constexpr std::uint8_t kRelayMainCmd = 0xF3;
constexpr std::uint8_t kRelayChangeServerSubCmd = 0x12;
constexpr std::uint8_t kRelayCheckSessionSubCmd = 0x32;
constexpr std::uint64_t kRelayKickoutEchoWindowMs = 5000;
// 原版 Observe 的 10 秒重连 / 3 秒上报节拍当前落在类外静态状态里。
std::uint64_t dwObserveConnectTick = 0;
std::uint64_t dwUpdateObserveInfoTick = 0;

template <std::size_t N>
void CopyWideBuffer(wchar_t (&destination)[N], const wchar_t* source) {
    std::wmemset(destination, 0, N);
    if (!source) {
        return;
    }

#ifdef _WIN32
    wcsncpy_s(destination, N, source, N - 1);
#else
    std::wcsncpy(destination, source, N - 1);
    destination[N - 1] = L'\0';
#endif
}


std::string GreenDamTan_GetExecutablePath() {
#ifdef _WIN32
    char pathBuffer[MAX_PATH] = {};
    const DWORD length = GetModuleFileNameA(nullptr, pathBuffer, static_cast<DWORD>(std::size(pathBuffer)));
    if (length == 0 || length >= std::size(pathBuffer)) {
        return {};
    }
    return std::string(pathBuffer, pathBuffer + length);
#else
    return {};
#endif
}
std::uint64_t ReadAutoShutdownMs() {
    // TODO: 仅做测试用：便于还原工程在自动验证时退出，不代表原版登录服行为。
#ifdef _WIN32
    char* value = nullptr;
    std::size_t length = 0;
    if (_dupenv_s(&value, &length, "GREENDAMTAN_AUTOSTOP_MS") != 0 || !value || !*value) {
        if (value) {
            std::free(value);
        }
        return 0;
    }
#else
    const char* value = std::getenv("GREENDAMTAN_AUTOSTOP_MS");
    if (!value || !*value) {
        return 0;
    }
#endif

    char* end = nullptr;
    const unsigned long long parsed = std::strtoull(value, &end, 10);
    if (!end || *end != '\0') {
#ifdef _WIN32
        std::free(value);
#endif
        return 0;
    }
#ifdef _WIN32
    std::free(value);
#endif
    return static_cast<std::uint64_t>(parsed);
}


std::uint64_t GetTickCount64Compat() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

#ifdef _WIN32
BOOL WINAPI ConsoleCtrlThunk(DWORD opCode) {
    return XLoginServer::ConsolCtrlHandler(opCode) ? TRUE : FALSE;
}
#else
void ConsoleCtrlThunk(int signalCode) {
    XLoginServer::ConsolCtrlHandler(static_cast<unsigned int>(signalCode));
}
#endif
}

bool DecodeRelayCheckSessionRequest(const XPacket& packet,
                                    unsigned int& uaid,
                                    std::uint64_t& authSessionId) {
    const char* payload = packet.GetPayloadBuffer();
    const std::uint16_t payloadSize = packet.GetPayloadSize();
    if (!payload || payloadSize < sizeof(PACKET_ROOT) + sizeof(uaid) + sizeof(authSessionId)) {
        return false;
    }

    std::memcpy(&uaid, payload + sizeof(PACKET_ROOT), sizeof(uaid));
    std::memcpy(&authSessionId,
                payload + sizeof(PACKET_ROOT) + sizeof(uaid),
                sizeof(authSessionId));
    return true;
}

bool DecodeRelayEnterServerRequest(const XPacket& packet, PS_ENTER_MAP_REQ& enterMapRequest) {
    const char* payload = packet.GetPayloadBuffer();
    const std::uint16_t payloadSize = packet.GetPayloadSize();
    if (!payload || payloadSize < sizeof(PACKET_ROOT)) {
        return false;
    }

    XParse parser;
    parser.AssignRawBuffer(payload, payloadSize);
    parser.SetUsIndex(static_cast<short>(sizeof(PACKET_ROOT)));

    parser >> enterMapRequest.dwUAID;
    parser >> enterMapRequest.dwActorID;
    parser >> enterMapRequest.stPartyInfo.byGroupType;
    parser >> enterMapRequest.stPartyInfo.nID;
    parser >> enterMapRequest.wMapID;
    parser >> enterMapRequest.wChannel;
    parser >> enterMapRequest.nJumpID;
    parser >> enterMapRequest.nPortalID;
    parser >> enterMapRequest.uxMapID.nMapID;
    parser >> enterMapRequest.stEnterDistrictPos.sWorldID;
    parser >> enterMapRequest.stEnterDistrictPos.uxMapID.nMapID;
    parser >> enterMapRequest.stEnterDistrictPos.vPos.x;
    parser >> enterMapRequest.stEnterDistrictPos.vPos.y;
    parser >> enterMapRequest.stEnterDistrictPos.vPos.z;
    parser >> enterMapRequest.stEnterDistrictPos.fRot;
    parser >> enterMapRequest.byChangeType;
    parser >> enterMapRequest.vNextPos.x;
    parser >> enterMapRequest.vNextPos.y;
    parser >> enterMapRequest.vNextPos.z;
    return parser.GetLastError() == 0;
}

bool DecodeRelayChangeServerRequest(const XPacket& packet, PS_REQ_CHANGE_SERVER& changeServerRequest) {
    const char* payload = packet.GetPayloadBuffer();
    const std::uint16_t payloadSize = packet.GetPayloadSize();
    if (!payload || payloadSize < sizeof(PACKET_ROOT) + sizeof(changeServerRequest.dwActorID) +
                                        sizeof(changeServerRequest.dwUAID) +
                                        sizeof(changeServerRequest.byType)) {
        return false;
    }

    XParse parser;
    parser.AssignRawBuffer(payload, payloadSize);
    parser.SetUsIndex(static_cast<short>(sizeof(PACKET_ROOT)));
    parser >> changeServerRequest.dwActorID;
    parser >> changeServerRequest.dwUAID;
    parser >> changeServerRequest.byType;
    return parser.GetLastError() == 0;
}

PS_RES_CHANGE_SERVER BuildPendingChangeServerReply(const PS_REQ_CHANGE_SERVER& request,
                                                   const XOption& option) {
    PS_RES_CHANGE_SERVER reply{};
    reply.dwActorID = request.dwActorID;
    reply.dwUAID = request.dwUAID;
    reply.byType = request.byType;
    reply.bResult = false;

    const char* fallbackServerName = request.byType != 0 ? "LOGIN" : "AUTH";
    SERVER_SYSTEM_INFO serverInfo{};
    if (option.GreenDamTan_GetServerSystemInfo(fallbackServerName, &serverInfo)) {
#ifdef _WIN32
        strcpy_s(reply.szIP, std::size(reply.szIP), serverInfo.szPublicIP);
#else
        std::strncpy(reply.szIP, serverInfo.szPublicIP, sizeof(reply.szIP) - 1);
#endif
        reply.sPort = static_cast<std::int16_t>(serverInfo.nPort);
    }
    return reply;
}

XLoginServer::XLoginServer() {
    m_pIObjectMgr = &m_xUserObjectMgr;
    BindObjectMgr(&m_xUserObjectMgr);
    m_xClientPool.SetCreator([this]() -> XClient* {
        return m_xCreator.Create();
    });
}

XRelaySocket::~XRelaySocket() {
    backendStop_ = true;
    XIOCPClient::Shutdown(0xFFFFFFFFu);
    if (backendThread_.joinable() && backendThread_.get_id() != std::this_thread::get_id()) {
        backendThread_.join();
    }
}

void XRelaySocket::SendUserKickout(const PS_KICK_USER_INFO& kickInfo) {
    XSendPacket sendPacket(kRelayMainCmd, 0x07);
    sendPacket << kickInfo;
    XIOCPClient::Send(sendPacket);
}

void XRelaySocket::SendCreateMazeReq(const ST_CREATE_MAZE& createMaze) {
    XSendPacket sendPacket(eCMD_SERVER, eSUB_CMD_SERVER_CREATE_MAZE_REQ);
    sendPacket << createMaze;
    XIOCPClient::Send(sendPacket);
}

bool XRelaySocket::Init(E_POOL_ID poolId, const char* ip, std::uint16_t port) {
    backendStop_ = true;
    XIOCPClient::Shutdown(0xFFFFFFFFu);
    if (backendThread_.joinable() && backendThread_.get_id() != std::this_thread::get_id()) {
        backendThread_.join();
    }

    m_myInfo = {};
    m_relayInfo = {};
    m_nSyncServerData = 0;

    backendStop_ = false;
    try {
        backendThread_ = std::thread(&XRelaySocket::OnStartThread, this);
    } catch (...) {
        backendStop_ = true;
        return false;
    }

    if (XIOCPClient::Init(poolId, ip, port)) {
        return true;
    }

    backendStop_ = true;
    if (backendThread_.joinable() && backendThread_.get_id() != std::this_thread::get_id()) {
        backendThread_.join();
    }
    return false;
}

void XRelaySocket::SetMyInfo(const XOption* option) {
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
    strcpy_s(m_myInfo.szPrivateIP, std::size(m_myInfo.szPrivateIP), option->GetIP());
    strcpy_s(m_myInfo.szPublicIP, std::size(m_myInfo.szPublicIP), option->GetPublicIP());
    strcpy_s(m_myInfo.szName, std::size(m_myInfo.szName), option->GetName());
#else
    std::strncpy(m_myInfo.szPrivateIP, option->GetIP(), sizeof(m_myInfo.szPrivateIP) - 1);
    std::strncpy(m_myInfo.szPublicIP, option->GetPublicIP(), sizeof(m_myInfo.szPublicIP) - 1);
    std::strncpy(m_myInfo.szName, option->GetName(), sizeof(m_myInfo.szName) - 1);
#endif

#ifdef _WIN32
    sprintf_s(m_relayInfo.szName, "CONTROL");
#else
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

bool XRelaySocket::Connect() {
    return XIOCPClient::Connect(m_relayInfo.szPrivateIP,
                                static_cast<std::uint16_t>(m_relayInfo.sPort));
}

void XRelaySocket::GreenDamTan_RecordPendingCheckSession(unsigned int uaid,
                                                        std::uint64_t authSessionID,
                                                        int sessionID) {
    if (uaid == 0 || authSessionID == 0 || sessionID <= 0) {
        return;
    }

    std::lock_guard<std::mutex> lock(pendingCheckSessionLock_);
    pendingCheckSessionReplies_[uaid] =
        GreenDamTan_PendingCheckSessionReply{uaid, authSessionID, GetTickCount64Compat(), sessionID};
}

void XRelaySocket::GreenDamTan_ClearPendingCheckSession(unsigned int uaid) {
    if (uaid == 0) {
        return;
    }

    std::lock_guard<std::mutex> lock(pendingCheckSessionLock_);
    pendingCheckSessionReplies_.erase(uaid);
}

void XRelaySocket::GreenDamTan_RecordPendingChangeServer(const PS_REQ_CHANGE_SERVER& request,
                                                         int sessionID) {
    if (request.dwUAID == 0) {
        return;
    }

    std::lock_guard<std::mutex> lock(pendingChangeServerLock_);
    pendingChangeServerReplies_[request.dwUAID] = {
        request,
        GetTickCount64Compat(),
        sessionID,
    };
}

void XRelaySocket::GreenDamTan_ClearPendingChangeServer(unsigned int uaid) {
    if (uaid == 0) {
        return;
    }

    std::lock_guard<std::mutex> lock(pendingChangeServerLock_);
    pendingChangeServerReplies_.erase(uaid);
}

void XRelaySocket::GreenDamTan_ProcessPendingCheckSessionReplies() {
    std::vector<GreenDamTan_PendingCheckSessionReply> pendingReplies;
    {
        std::lock_guard<std::mutex> lock(pendingCheckSessionLock_);
        if (pendingCheckSessionReplies_.empty()) {
            return;
        }
        pendingReplies.reserve(pendingCheckSessionReplies_.size());
        for (const auto& [uaid, reply] : pendingCheckSessionReplies_) {
            (void)uaid;
            pendingReplies.push_back(reply);
        }
    }

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    if (!loginServer) {
        return;
    }

    for (const GreenDamTan_PendingCheckSessionReply& pendingReply : pendingReplies) {
        CUser* user = loginServer->FindUIDToUser(static_cast<int>(pendingReply.uaid));
        if (!user) {
            GreenDamTan_ClearPendingCheckSession(pendingReply.uaid);
            continue;
        }

        if (user->GetSessionID() != pendingReply.sessionID) {
            continue;
        }

        if (user->GetUAID() != static_cast<int>(pendingReply.uaid) ||
            user->GetAuthSessionID() != pendingReply.authSessionID) {
            continue;
        }

        const std::uint64_t currentTick = GetTickCount64Compat();
        if (currentTick - pendingReply.queuedTick < 200) {
            continue;
        }

        XSendPacket replyPacket(kRelayMainCmd, kRelayCheckSessionSubCmd);
        replyPacket.XParse << static_cast<int>(pendingReply.uaid);
        replyPacket.XParse << static_cast<std::uint8_t>(0);

        LogHelper::LogDebug(
            "game.system",
            "GreenDamTan_log LoginServer.cpp::XRelaySocket::GreenDamTan_ProcessPendingCheckSessionReplies uaid=%u auth=%llu session=%d waitedMs=%llu",
            pendingReply.uaid,
            static_cast<unsigned long long>(pendingReply.authSessionID),
            pendingReply.sessionID,
            static_cast<unsigned long long>(currentTick - pendingReply.queuedTick));

        if (GreenDamTan_QueueRecvPacket(replyPacket)) {
            GreenDamTan_ClearPendingCheckSession(pendingReply.uaid);
        }
    }
}

void XRelaySocket::GreenDamTan_ProcessPendingChangeServerReplies() {
    std::vector<GreenDamTan_PendingChangeServerReply> pendingReplies;
    {
        std::lock_guard<std::mutex> lock(pendingChangeServerLock_);
        if (pendingChangeServerReplies_.empty()) {
            return;
        }
        pendingReplies.reserve(pendingChangeServerReplies_.size());
        for (const auto& [uaid, reply] : pendingChangeServerReplies_) {
            (void)uaid;
            pendingReplies.push_back(reply);
        }
    }

    XLoginServer* loginServer = TXSingleton<XLoginServer>::Instance();
    if (!loginServer) {
        return;
    }

    for (const GreenDamTan_PendingChangeServerReply& pendingReply : pendingReplies) {
        CUser* user = loginServer->FindUIDToUser(static_cast<int>(pendingReply.request.dwUAID));
        if (!user) {
            GreenDamTan_ClearPendingChangeServer(pendingReply.request.dwUAID);
            continue;
        }

        if (user->GetSessionID() != pendingReply.sessionID) {
            continue;
        }

        if (user->IsState(eStateGoBackLobby) || user->IsState(eStateGoBackAuth)) {
            GreenDamTan_ClearPendingChangeServer(pendingReply.request.dwUAID);
            continue;
        }

        const std::uint64_t currentTick = GetTickCount64Compat();
        if (currentTick - pendingReply.queuedTick < 200) {
            continue;
        }

        PS_RES_CHANGE_SERVER changeServerReply =
            BuildPendingChangeServerReply(pendingReply.request, loginServer->GetOption());
        XSendPacket replyPacket(kRelayMainCmd, kRelayChangeServerSubCmd);
        replyPacket << changeServerReply;

        LogHelper::LogDebug(
            "game.system",
            "GreenDamTan_log LoginServer.cpp::XRelaySocket::GreenDamTan_ProcessPendingChangeServerReplies uaid=%u actor=%u byType=%u session=%d target=%s:%d waitedMs=%llu",
            changeServerReply.dwUAID,
            changeServerReply.dwActorID,
            static_cast<unsigned int>(changeServerReply.byType),
            pendingReply.sessionID,
            changeServerReply.szIP,
            static_cast<int>(changeServerReply.sPort),
            static_cast<unsigned long long>(currentTick - pendingReply.queuedTick));

        if (GreenDamTan_QueueRecvPacket(replyPacket)) {
            GreenDamTan_ClearPendingChangeServer(pendingReply.request.dwUAID);
        }
    }
}

bool XRelaySocket::IsReady() const {
    return m_eState == eConnectStateConnected && m_relayInfo.nState == 2;
}

void XRelaySocket::OnStartThread() {
    using namespace std::chrono;

    while (!backendStop_ && m_eState != eConnectStateShutdown) {
        const steady_clock::time_point begin = steady_clock::now();
        if (m_bInit) {
            GreenDamTan_PumpSocketRecv();
            GreenDamTan_ProcessPendingCheckSessionReplies();
            GreenDamTan_ProcessPendingChangeServerReplies();
            if (m_eState == eConnectStateReconnect && !m_stReConnectInfo.szAddr.empty()) {
                const std::string reconnectAddr = m_stReConnectInfo.szAddr;
                const std::uint16_t reconnectPort = m_stReConnectInfo.usPort;
                XIOCPClient::Shutdown(0xFFFFFFFFu);
                XIOCPClient::Init(ePoolIDNone, "127.0.0.1", 5001);
                XIOCPClient::Connect(const_cast<char*>(reconnectAddr.c_str()), reconnectPort);
            }

            if (m_xSocket.eBlock != SocketState::eBLOCK_OFF) {
                if (m_eState == eConnectStateConnected) {
                    OnDisConnect();
                    GreenDamTan_DetachConnectThread();
                    XIOCPClient::Close();
                }
            } else {
                GreenDamTan_ParseIoContextBuffer();
            }
        }
        const auto elapsed = duration_cast<milliseconds>(steady_clock::now() - begin);
        if (elapsed < milliseconds(15)) {
            std::this_thread::sleep_for(milliseconds(15) - elapsed);
        }
    }
}

void XRelaySocket::OnFinishThread() {
}

std::uint64_t XRelaySocket::_BackendThreadStartingPoint(void* ptr) {
    XRelaySocket* self = static_cast<XRelaySocket*>(ptr);
    if (!self) {
        return 0;
    }

    self->OnStartThread();
    self->OnFinishThread();
    return 0;
}

void XRelaySocket::OnConnect() {
    m_nSyncServerData = 0;
    SendAddServer();
}

void XRelaySocket::OnDisConnect() {
    m_relayInfo.nState = 0;
    LogHelper::LogInfo("game.system", "Relay Client DisConnect");
    m_eState = eConnectStateDisconnected;
}

void XRelaySocket::OnNotConnect() {
    LogHelper::LogInfo("game.system", "NotConnect to Relay! ");
}

bool XRelaySocket::OnParse(XPacket& xPacket) {
    switch (xPacket.GetMainCmd()) {
    case eCMD_SERVER:
        return ServerProcess(xPacket);
    case kRelayMainCmd:
        return UserProcess(xPacket);
    case 0xF4:
        return PartyProcess(xPacket);
    case 0xF5:
        return FriendProcess(xPacket);
    case 0xF6:
        return LeagueProcess(xPacket);
    case 0xFA:
        return ForceProcess(xPacket);
    case 0xFB:
        return WorldModeProcess(xPacket);
    case 0xFD:
        return ModeMazeProcess(xPacket);
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
        return ServerProcessEx(xPacket);
    }
}

bool XRelaySocket::ServerProcessEx(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::UserProcess(XPacket& xPacket) {
    switch (xPacket.GetSubCmd()) {
    case 0x07:
        return RecvUserKickout(xPacket);
    case 0x10:
        return RecvUserWhisperRes(xPacket);
    case 0x11:
        return RecvUserNotice(xPacket);
    case kRelayChangeServerSubCmd:
        return RecvUserChangeServer(xPacket);
    case 0x14:
        return RecvUserEnterServer(xPacket);
    case 0x17:
        return RecvUserMegaPhone(xPacket);
    case 0x27:
        return RecvUserTradePasswordState(xPacket);
    case 0x28:
        return RecvExchangePriceHistory(xPacket);
    case 0x30:
        return RecvExchangePost(xPacket);
    case kRelayCheckSessionSubCmd:
        return RecvCheckSessionID(xPacket);
    case 0x37:
        return RecvGFBillingPostReload(xPacket);
    default:
        return false;
    }
}

bool XRelaySocket::PartyProcess(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::FriendProcess(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::LeagueProcess(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::ForceProcess(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::WorldModeProcess(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::ModeMazeProcess(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvServerUpdate(XPacket& xPacket) {
    SS_UPDATE_SERVER_INFO updateInfo{};
    xPacket >> updateInfo;
    m_relayInfo.nState = static_cast<int>(updateInfo.nState);
    return true;
}

void XRelaySocket::RecvPacketFromRelay(XPacket& xPacket) {
    static_cast<void>(xPacket);
}

bool XRelaySocket::RecvChangeChannelRes(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvUpdateChannelAll(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvUpdateChannel(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvUserKickout(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvUserWhisperRes(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvUserNotice(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvUserChangeServer(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvUserEnterServer(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvUserMegaPhone(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvUserTradePasswordState(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvExchangePriceHistory(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvExchangePost(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvCheckSessionID(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

bool XRelaySocket::RecvGFBillingPostReload(XPacket& xPacket) {
    static_cast<void>(xPacket);
    return true;
}

void XRelaySocket::SendAddServer() {
    m_myInfo.nState = 1;
    XSendPacket sendPacket(eCMD_SERVER, eSUB_CMD_SERVER_ADD);
    sendPacket << m_myInfo;
    XIOCPClient::Send(sendPacket);
}

void XRelaySocket::SendUpdateServerInfo(std::int16_t nState, int nUserCount) {
    if (static_cast<float>(nUserCount) >= static_cast<float>(m_myInfo.nMaxUser) * 0.95f) {
        nState = 3;
    }

    SS_UPDATE_SERVER_INFO stServerInfo{};
    stServerInfo.dwID = m_myInfo.dwID;
    stServerInfo.nState = static_cast<std::uint16_t>(nState);
    stServerInfo.nCurUser = static_cast<std::uint16_t>(nUserCount);

    m_myInfo.nCurUser = nUserCount;

    XSendPacket sendPacket(eCMD_SERVER, eSUB_CMD_SERVER_UPDATE);
    sendPacket << stServerInfo;
    XIOCPClient::Send(sendPacket);
}

bool CLogThreadManager::Start(const char* szName) {
    if (m_isStart) {
        return false;
    }

    const char* logName = szName ? szName : "LOGIN";
    std::filesystem::path configPath;
    try {
        configPath = std::filesystem::current_path();
    } catch (const std::filesystem::filesystem_error&) {
        configPath.clear();
    }

    configPath /= "Config";
    configPath /= std::string(logName) + "_LogSetting.properties";
    m_strConfigPath = configPath.generic_string();

    // TODO: 推测结果：当前仍未恢复原版 `CLogThreadProc` 文件线程主体；
    // 这里先把日志落盘目录与官方 `Log/<ServerName>` 结构对齐，确保重建版能产生可核对的日志文件。
    LogHelper::ConfigureFileLogging(logName);
    LogHelper::LogInfo("game.system", "GreenDamTan_log file logging ready server=%s", logName);

    // TODO: 推测结果：原版日志在 `CLogThreadProc::ThreadProc` 线程入口输出；
    // 当前日志线程主体尚未恢复，先在 Start 对齐首条启动日志。
    LogHelper::LogInfo("game.system", "Start Log Thread ( %d )", 0);
    m_isStart = true;
    return true;
}

void CLogThreadManager::End() {
    m_isStart = false;
    LogHelper::ShutdownFileLogging();
}

bool CXigncode::Init() {
    m_bInit = true;
    return true;
}

void CXigncode::ConnectUser(int sessionID, unsigned int nAddr) {
    (void)nAddr;
    LogHelper::LogDebug("game.system", "<XIGNCODE> Connect User ( %d ) ", sessionID);
}

void CXigncode::DisconnectUser(int sessionID) {
    LogHelper::LogDebug("game.system", "<XIGNCODE> Disconnect User ( %d ) ", sessionID);
}

void CXigncode::RecvXigncode(int sessionID, const PS_XIGNCODE_UPDATE& stXigncode) {
    (void)stXigncode;
    LogHelper::LogDebug("game.system", "<XIGNCODE> Recv User ( %d ) ", sessionID);
}

void CXigncode::Release() {
    m_bInit = false;
}

void XSeed::Init(bool seedFromRandomDevice) {
    if (seedFromRandomDevice) {
        m_dwSeed = std::random_device{}();
    } else {
        m_dwSeed = 0;
    }
}

XGameDBSocketMgr::~XGameDBSocketMgr() {
    Clear();
}

void XGameDBSocketMgr::Init() {
    Clear();
    InitAgentGroup(0, m_pGameDBAgent, m_nGameAgentCnt);
    InitAgentGroup(1, m_pAccountDBAgent, m_nAccountAgentCnt);
    InitAgentGroup(2, m_pLogDBAgent, m_nLogAgentCnt);
    InitAgentGroup(3, m_pStatisticsDBAgent, m_nStatisticsAgentCnt);
    InitAgentGroup(4, m_pSGLogDBAgent, m_nSGLogAgentCnt);
}

void XGameDBSocketMgr::AutoConnect() {
    AutoConnectGroup(m_pAccountDBAgent, m_nAccountAgentCnt);
    AutoConnectGroup(m_pGameDBAgent, m_nGameAgentCnt);
    AutoConnectGroup(m_pLogDBAgent, m_nLogAgentCnt);
    AutoConnectGroup(m_pStatisticsDBAgent, m_nStatisticsAgentCnt);
    AutoConnectGroup(m_pSGLogDBAgent, m_nSGLogAgentCnt);
}

void XGameDBSocketMgr::DisConnect() {
    DisconnectGroup(m_pAccountDBAgent, m_nAccountAgentCnt);
    DisconnectGroup(m_pGameDBAgent, m_nGameAgentCnt);
    DisconnectGroup(m_pLogDBAgent, m_nLogAgentCnt);
    DisconnectGroup(m_pStatisticsDBAgent, m_nStatisticsAgentCnt);
    DisconnectGroup(m_pSGLogDBAgent, m_nSGLogAgentCnt);
}

bool XGameDBSocketMgr::SendAccountDBAgent(int iIndex, const XSendDBPacket& xSendPacket) {
    return m_pAccountDBAgent && iIndex < GetAccountDBAgentCount() &&
           m_pAccountDBAgent[iIndex].m_bState && m_pAccountDBAgent[iIndex].Send(xSendPacket);
}

bool XGameDBSocketMgr::SendGameDBAgent(int iIndex, const XSendDBPacket& xSendPacket) {
    return m_pGameDBAgent && iIndex < GetGameDBAgentCount() &&
           m_pGameDBAgent[iIndex].m_bState && m_pGameDBAgent[iIndex].Send(xSendPacket);
}

bool XGameDBSocketMgr::SendLogDBAgent(int iIndex, const XSendDBPacket& xSendPacket) {
    return m_pLogDBAgent && iIndex < GetLogDBAgentCount() && m_pLogDBAgent[iIndex].m_bState &&
           m_pLogDBAgent[iIndex].Send(xSendPacket);
}

bool XGameDBSocketMgr::SendStatisticsDBAgent(int iIndex, const XSendDBPacket& xSendPacket) {
    return m_pStatisticsDBAgent && iIndex < GetStatisticsDBAgentCount() &&
           m_pStatisticsDBAgent[iIndex].m_bState && m_pStatisticsDBAgent[iIndex].Send(xSendPacket);
}

int XGameDBSocketMgr::GetStatisticsDBAgentCount() const {
    return m_nStatisticsAgentCnt;
}

int XGameDBSocketMgr::GetGameDBAgentCount() const {
    return m_nGameAgentCnt;
}

int XGameDBSocketMgr::GetAccountDBAgentCount() const {
    return m_nAccountAgentCnt;
}

int XGameDBSocketMgr::GetLogDBAgentCount() const {
    return m_nLogAgentCnt;
}

void XGameDBSocketMgr::InitAgentGroup(std::uint8_t byType,
                                      XGameDBSocket*& ppAgents,
                                      int& nAgentCount) {
    ppAgents = nullptr;
    nAgentCount = 0;

    XOption& option = TXSingleton<XLoginServer>::Instance()->GetOption();
    char szIP[513] = {};
    char szName[21] = {};
    std::int16_t shPort = 0;
    int nMaxIOPoolCount = 0;
    if (!option.GetDBAgentInfo(byType, szIP, &shPort, szName, &nMaxIOPoolCount, &nAgentCount) ||
        nAgentCount <= 0) {
        nAgentCount = 0;
        return;
    }

    ppAgents = new XGameDBSocket[static_cast<std::size_t>(nAgentCount)];
    for (int index = 0; index < nAgentCount; ++index) {
        if (ppAgents[index].Init(ePoolIDNone, szIP, shPort, szName, nMaxIOPoolCount)) {
            ppAgents[index].m_bState = true;
            ppAgents[index].m_byType = byType;
        }
    }
}

void XGameDBSocketMgr::AutoConnectGroup(XGameDBSocket* pAgents, int nAgentCount) {
    if (!pAgents || nAgentCount <= 0) {
        return;
    }

    for (int index = 0; index < nAgentCount; ++index) {
        if (pAgents[index].m_bState) {
            pAgents[index].AutoConnect();
        }
    }
}

void XGameDBSocketMgr::DisconnectGroup(XGameDBSocket* pAgents, int nAgentCount) {
    if (!pAgents || nAgentCount <= 0) {
        return;
    }

    for (int index = 0; index < nAgentCount; ++index) {
        if (pAgents[index].m_bState) {
            pAgents[index].SetSafetyShutdown(true);
            pAgents[index].DisConnect();
        }
    }
}

void XGameDBSocketMgr::Clear() {
    delete[] m_pGameDBAgent;
    delete[] m_pAccountDBAgent;
    delete[] m_pLogDBAgent;
    delete[] m_pStatisticsDBAgent;
    delete[] m_pSGLogDBAgent;

    m_pGameDBAgent = nullptr;
    m_nGameAgentCnt = 0;
    m_pAccountDBAgent = nullptr;
    m_nAccountAgentCnt = 0;
    m_pLogDBAgent = nullptr;
    m_nLogAgentCnt = 0;
    m_pStatisticsDBAgent = nullptr;
    m_nStatisticsAgentCnt = 0;
    m_pSGLogDBAgent = nullptr;
    m_nSGLogAgentCnt = 0;
}

void CObserveSocket::SetMyInfo(const XOption* option) {
    if (!option) {
        m_myInfo = {};
        m_observeInfo = {};
        return;
    }

    m_myInfo = {};
    m_observeInfo = {};
    m_myInfo.dwID = option->GetServerID();
    m_myInfo.nGroup = option->GetGroupID();
    m_myInfo.nType = option->GetType();
    m_myInfo.nChannel = option->GetChannel();
    m_myInfo.nState = 1;
    m_myInfo.nCurUser = 0;
    m_myInfo.sPort = static_cast<std::int16_t>(option->GetPort());
    m_myInfo.nMaxUser = option->GetMaxConnect();
    m_myInfo.sThreadCount = option->GetLogicThread();
#ifdef _WIN32
    strcpy_s(m_myInfo.szPrivateIP, std::size(m_myInfo.szPrivateIP), option->GetIP());
    strcpy_s(m_myInfo.szPublicIP, std::size(m_myInfo.szPublicIP), option->GetPublicIP());
    strcpy_s(m_myInfo.szName, std::size(m_myInfo.szName), option->GetName());
    sprintf_s(m_observeInfo.szName, "OBSERVEAGENT");
#else
    std::strncpy(m_myInfo.szPrivateIP, option->GetIP(), sizeof(m_myInfo.szPrivateIP) - 1);
    std::strncpy(m_myInfo.szPublicIP, option->GetPublicIP(), sizeof(m_myInfo.szPublicIP) - 1);
    std::strncpy(m_myInfo.szName, option->GetName(), sizeof(m_myInfo.szName) - 1);
    std::snprintf(m_observeInfo.szName, sizeof(m_observeInfo.szName), "%s", "OBSERVEAGENT");
#endif

    std::uint16_t observePort = 0;
    if (option->GetAgentPrivateIPAndPort(1, m_observeInfo.szPrivateIP, observePort)) {
        m_observeInfo.sPort = static_cast<std::int16_t>(observePort);
    } else {
        m_observeInfo.sPort = 0;
    }
}

bool CObserveSocket::StartUp(XOption* pOption) {
    SetMyInfo(pOption);
    if (!m_observeInfo.sPort) {
        LogHelper::LogInfo("game.system", "No Information for Observe Socket");
        return true;
    }

#ifdef _WIN32
    m_dwProcessID = GetCurrentProcessId();
#else
    m_dwProcessID = static_cast<std::uint32_t>(getpid());
#endif
    if (!Init(ePoolIDNone, "127.0.0.1", 5001)) {
        LogHelper::LogError("game.system", "Error Observe Socket Init fail ");
        return false;
    }

    if (!XIOCPClient::Connect(m_observeInfo.szPrivateIP, static_cast<std::uint16_t>(m_observeInfo.sPort))) {
        LogHelper::LogError("game.system", "Failed conect Observe Server!!");
        return false;
    }

    m_bActivate = true;
    return true;
}

void CObserveSocket::OnConnect() {
    m_nSyncServerData = 0;
    LogHelper::LogInfo("game.system", "Success to Connect ObserveAgent! ");
}

void CObserveSocket::OnDisConnect() {
    m_observeInfo.nState = 0;
    m_bActivate = false;
    XIOCPClient::OnDisConnect();
}

void CObserveSocket::OnNotConnect() {
    LogHelper::LogInfo("game.system", "NotConnect to Observe! ");
}

void CObserveSocket::CalculateThreadStatus(wchar_t* szLogicThread, int nMaxThreadCount) {
    if (!szLogicThread || nMaxThreadCount <= 0 || nMaxThreadCount >= 20) {
        return;
    }

    std::wstring strState;
    {
        CFAutoSlimReadLock autolock(&m_rwThreadTickLock);
        for (int index = 0; index < nMaxThreadCount; ++index) {
            const std::uint64_t lastTick = m_dwThreadFpsTick_Last[static_cast<std::size_t>(index)];
            const std::uint64_t nowTick = m_dwThreadFpsTick_Now[static_cast<std::size_t>(index)];
            if (!lastTick || !nowTick) {
                continue;
            }

            strState.push_back(lastTick == nowTick ? L'0' : L'1');
            if (index < nMaxThreadCount - 1) {
                strState.push_back(L'/');
            }
        }
        m_dwThreadFpsTick_Last = m_dwThreadFpsTick_Now;
    }

    constexpr std::size_t kLogicThreadTextCapacity = 33;
    std::wmemset(szLogicThread, 0, kLogicThreadTextCapacity);
    if (strState.empty()) {
        return;
    }

#ifdef _WIN32
    wcsncpy_s(szLogicThread, kLogicThreadTextCapacity, strState.c_str(), _TRUNCATE);
#else
    std::wcsncpy(szLogicThread, strState.c_str(), kLogicThreadTextCapacity - 1);
    szLogicThread[kLogicThreadTextCapacity - 1] = L'\0';
#endif
}

void CObserveSocket::SendReportServerStatus(int nServerType,
                                            int nUserCount,
                                            bool bControlConnect,
                                            bool bCommunityConnect,
                                            int nMaxThreadCount,
                                            char* szIP,
                                            int nPort,
                                            bool bNetCafe) {
    SS_REPORT_SERVER_STATUS stServerStatus{};
    stServerStatus.nServerType = nServerType;
    stServerStatus.dwServerID = m_myInfo.dwID;
    stServerStatus.dwProcessID = m_dwProcessID;
    stServerStatus.nUserCount = nUserCount;
    if (szIP) {
#ifdef _WIN32
        strcpy_s(stServerStatus.szIP, std::size(stServerStatus.szIP), szIP);
#else
        std::strncpy(stServerStatus.szIP, szIP, sizeof(stServerStatus.szIP) - 1);
#endif
    }
    stServerStatus.nPort = nPort;
    stServerStatus.bNetCafe = bNetCafe;
    stServerStatus.poolInfo = m_poolInfo;
    stServerStatus.connectInfo.bControlConnect = bControlConnect;
    stServerStatus.connectInfo.bCommunityConnect = bCommunityConnect;
    if (nServerType == 2) {
        CalculateThreadStatus(stServerStatus.connectInfo.szLogicThread, nMaxThreadCount);
    }

    // 原版这里会在构包后立刻走 `XIOCPClient::Send` 发往 ObserveAgent。
    XSendPacket sendPacket(eCMD_SERVER, eSUB_CMD_SERVER_REPORT_STATUS);
    sendPacket << stServerStatus;
    XIOCPClient::Send(sendPacket);
}

bool CObserveSocket::OnUpdate(std::uint64_t currentTick,
                              const char* ip,
                              int port,
                              int userCount,
                              bool controlConnect,
                              bool communityConnect,
                              int maxThreadCount,
                              bool netCafe) {
    if (!m_bActivate) {
        return false;
    }

    if (dwObserveConnectTick == 0) {
        dwObserveConnectTick = GetTickCount64Compat();
    }
    if (dwUpdateObserveInfoTick == 0) {
        dwUpdateObserveInfoTick = GetTickCount64Compat();
    }

    if (!XIOCPClient::IsConnection()) {
        if (dwObserveConnectTick < currentTick) {
            LogHelper::LogInfo("game.system", "Try to Connection Observe Agent!");
            XIOCPClient::Connect(m_observeInfo.szPrivateIP, static_cast<std::uint16_t>(m_observeInfo.sPort));
            dwObserveConnectTick = currentTick + 10000;
        }
        return false;
    }

    if (!XIOCPClient::IsConnection() || dwUpdateObserveInfoTick >= currentTick) {
        return false;
    }

    SendReportServerStatus(m_myInfo.nType,
                           userCount,
                           controlConnect,
                           communityConnect,
                           maxThreadCount,
                           const_cast<char*>(ip),
                           port,
                           netCafe);
    dwUpdateObserveInfoTick = currentTick + 3000;
    return true;
}

int XLoginServer::ConsolCtrlHandler(unsigned int dwOPCode) {
#ifndef _WIN32
    if (dwOPCode == static_cast<unsigned int>(SIGINT) ||
        dwOPCode == static_cast<unsigned int>(SIGTERM)) {
        return TXSingleton<XLoginServer>::Instance()->Shutdown(0xFFFFFFFFu) ? 1 : 0;
    }
#endif

    if (dwOPCode > 2 && (dwOPCode <= 4 || dwOPCode > 6)) {
        return 0;
    }

    return TXSingleton<XLoginServer>::Instance()->Shutdown(0xFFFFFFFFu) ? 1 : 0;
}

bool XLoginServer::OnAccect(XClient* pClient) {
    if (!XServer::OnAccect(pClient)) {
        return false;
    }

    if (GetOption().GetSecurityType() == SECURITY_ON) {
        const char* pszAddr = inet_ntoa(pClient->scAddr.sin_addr);
        const unsigned int nAddr = pszAddr ? inet_addr(pszAddr) : 0;
        const int sessionID = pClient->GetSessionID();
        m_xignCode.ConnectUser(sessionID, nAddr);
    }

    return true;
}

void XLoginServer::SetName() {
    std::snprintf(m_szName, sizeof(m_szName), "%s", "LOGIN");
}

bool XLoginServer::InitServer() {
    CLogThreadManager* logThreadManager = TXSingleton<CLogThreadManager>::Instance();
    logThreadManager->Start(GetName());

    if (GetOption().GetSecurityType() == SECURITY_ON) {
        const bool initXignCode = m_xignCode.Init();
        GetOption().SetInitXignCode(initXignCode);
    }

    GetOption().ShowServerInfo();
    m_xSeed.Init(true);

    const std::string currentWorkingDirectory = std::filesystem::current_path().string();
    const std::string executablePath = GreenDamTan_GetExecutablePath();
    LogHelper::LogInfo("game.system",
                       "GreenDamTan_log LoginServer.cpp::XLoginServer::InitServer exe=%s cwd=%s",
                       executablePath.empty() ? "<unknown>" : executablePath.c_str(),
                       currentWorkingDirectory.c_str());

    const char* commonDNS = GetOption().GetDNS(2);
    if (!resourceMgr_.Init(commonDNS, nullptr, 0)) {
        LogHelper::LogError("game.system", "Error ResourceMgr Init fail");
        return false;
    }

    if (!resourceMgr_.Load(GetOption().GetResLoadType(), GetOption().GetResFilePath(), 0, 0)) {
        LogHelper::LogError("game.system", "Error ResourceMgr Load fail");
        return false;
    }

    if (TB_PHOTO_ITEM* defaultPhotoItem = resourceMgr_.FindDefaultPhotoItemID(3, 1); defaultPhotoItem) {
        LogHelper::LogInfo("game.system",
                           "GreenDamTan_log LoginServer.cpp::XLoginServer::InitServer photo-self-check class=3 type=1 hit id=%u photoName=%u photoGroup=%u",
                           defaultPhotoItem->ID,
                           defaultPhotoItem->Photo_Name,
                           static_cast<unsigned int>(defaultPhotoItem->Photo_Group));
    } else {
        LogHelper::LogError("game.system",
                            "GreenDamTan_log LoginServer.cpp::XLoginServer::InitServer photo-self-check class=3 type=1 miss");
    }
    if (GetOption().GetContentsOption()->nOptionFlag == 2) {
        for (int i = E_SERVER_OPTION_ATTENDANCE; i < E_SERVER_OPTION_MAX; ++i) {
            resourceMgr_.SetServerContents(i, GetOption().GetContentsOption()->bContents[i]);
        }
    }

    m_xDBAgentMgr.Init();
    m_xDBAgentMgr.AutoConnect();
    LogHelper::LogInfo("game.system", "[INIT] DBAgent Init ");

    if (!controlSocket_.Init(ePoolIDRelayServer, "127.0.0.1", 5001)) {
        LogHelper::LogError("game.system", "Error Relay Socket Init fail");
        return false;
    }

    controlSocket_.SetMyInfo(&GetOption());
    if (!controlSocket_.Connect()) {
        LogHelper::LogError("game.system", "Failed conect Relay Server!!");
        return false;
    }

    LogHelper::LogInfo("game.contents", "[INIT] ControlSocket - Init ");
    if (!m_scObserveSocket.StartUp(&GetOption())) {
        return false;
    }

    m_xItemFactory.Init(static_cast<std::uint8_t>(GetOption().GetGroupID()),
                        static_cast<std::uint8_t>(GetOption().GetChannel()));

    LogHelper::LogInfo("game.system",
                       "<ITEM_FACTORY> Factory Init ( %d, %d )",
                       static_cast<int>(GetOption().GetGroupID()),
                       GetOption().GetType());
    return true;
}

bool XLoginServer::Clear(std::uint32_t /*maxWait*/) {
    if (m_bClose) {
        return false;
    }

    if (GetOption().GetSecurityType() == SECURITY_ON) {
        m_xignCode.Release();
    }

    TXSingleton<CLogThreadManager>::Instance()->End();
    controlSocket_.Shutdown(0xFFFFFFFFu);
    m_xDBAgentMgr.DisConnect();
    waitQueue_.clear();
    {
        std::unique_lock<std::shared_mutex> userLock(usersByUaidLock_);
        usersByUaid_.clear();
    }
    {
        std::unique_lock<std::shared_mutex> actorLock(usersByActorIdLock_);
        usersByActorId_.clear();
    }
    m_bClose = true;
    return true;
}

void XLoginServer::OnUpdate(std::uint64_t currentTick) {
    int onlineUserCount = 0;
    {
        std::shared_lock<std::shared_mutex> userLock(usersByUaidLock_);
        onlineUserCount = static_cast<int>(usersByUaid_.size());
    }

    if (m_dwSGUpdateTick == 0) {
        m_dwSGUpdateTick = currentTick + 60000;
    }
    if (m_dwWaitUserTick == 0) {
        m_dwWaitUserTick = currentTick + 1000;
    }
    if (m_dw64FPSTick == 0) {
        m_dw64FPSTick = currentTick;
    }

    if (currentTick - m_dw64FPSTick >= 1000) {
        XServer::OnUpdate(currentTick);
        m_dw64FPSTick = currentTick;
        m_dwFrame = 0;
        ++m_nCheckUserCount;
    } else {
        ++m_dwFrame;
    }

    const bool controlConnected = controlSocket_.XIOCPClient::IsConnection();
    if (controlConnected || m_bClose) {
        if (controlConnected && m_dwUpdateServerInfoTick < currentTick) {
            controlSocket_.SendUpdateServerInfo(2, onlineUserCount);
            m_dwUpdateServerInfoTick = currentTick + 10000;
        }
    } else if (m_dwConnectTick < currentTick) {
        controlSocket_.Connect();
        m_dwConnectTick = currentTick + 30000;
        LogHelper::LogInfo("game.system", "OnUpdate [CONTROL_SOCKET.Connect()]");
    }
    if (m_dwSGUpdateTick < currentTick) {
        m_dwSGUpdateTick = currentTick + 60000;
    }

    m_scObserveSocket.OnUpdate(currentTick,
                               GetOption().GetIP(),
                               GetOption().GetPort(),
                               onlineUserCount,
                               false,
                               false,
                               0,
                               false);

    if (m_dwWaitUserTick < currentTick) {
        ProcessWaitUser(currentTick);
        m_dwWaitUserTick = currentTick + 1000;
    }

    if (m_nAutoShutdownTick == 0) {
        const std::uint64_t autoShutdownMs = ReadAutoShutdownMs();
        if (autoShutdownMs != 0) {
            // TODO: 仅做测试用：便于自动验证时在后台更新线程里主动停机。
            m_nAutoShutdownTick = currentTick + autoShutdownMs;
        }
    }

    if (m_nAutoShutdownTick != 0 && currentTick >= m_nAutoShutdownTick) {
        LogHelper::LogInfo("game.system",
                           "Auto shutdown tick reached after %llu ms",
                           static_cast<unsigned long long>(ReadAutoShutdownMs()));
        Shutdown(0xFFFFFFFFu);
    }

    if (m_bClose) {
        m_bRunFlag = false;
    }
}

int XLoginServer::SetConsoleHandler(int add) {
#ifdef _WIN32
    return SetConsoleCtrlHandler(ConsoleCtrlThunk, add ? TRUE : FALSE) ? 1 : 0;
#else
    if (add) {
        std::signal(SIGINT, ConsoleCtrlThunk);
        std::signal(SIGTERM, ConsoleCtrlThunk);
    } else {
        std::signal(SIGINT, SIG_DFL);
        std::signal(SIGTERM, SIG_DFL);
    }
    return 1;
#endif
}

int XLoginServer::nRand(int minValue, int maxValue) const {
    if (minValue > maxValue) {
        std::swap(minValue, maxValue);
    }
    return minValue + ((maxValue - minValue) / 2);
}

/**
 * @brief 转发一条发往 AccountDB 的包。
 * @param packet 已构造好的 DB 包。
 * @return 发送成功返回 true，否则返回 false。
 */
bool XLoginServer::SendDBAccount(const XSendDBPacket& packet) {
    const bool isEnterServerPacket = packet.GetMainCmd() == eCMD_LOGIN && packet.GetSubCmd() == 0x11;
    if (GetOption().GetSystemType() == SYSTEM_TYPE_DEV) {
        const bool sendOk = SendDBGame(packet);
        if (isEnterServerPacket) {
            LogHelper::LogDebug("game.system",
                                "GreenDamTan_log SendDBAccount main=%u sub=%u order=%d route=dev-game sendOk=%d",
                                static_cast<unsigned int>(packet.GetMainCmd()),
                                static_cast<unsigned int>(packet.GetSubCmd()),
                                packet.GetOrderID(),
                                sendOk ? 1 : 0);
        }
        return sendOk;
    }

    const int agentCount = m_xDBAgentMgr.GetAccountDBAgentCount();
    const int nIndex = packet.GetOrderID() % agentCount;
    const bool sendOk = m_xDBAgentMgr.SendAccountDBAgent(nIndex, packet);
    if (isEnterServerPacket) {
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log SendDBAccount main=%u sub=%u order=%d agentCount=%d index=%d systemType=%d sendOk=%d",
                            static_cast<unsigned int>(packet.GetMainCmd()),
                            static_cast<unsigned int>(packet.GetSubCmd()),
                            packet.GetOrderID(),
                            agentCount,
                            nIndex,
                            static_cast<int>(GetOption().GetSystemType()),
                            sendOk ? 1 : 0);
    }
    if (sendOk) {
        return true;
    }

    LogHelper::LogError("game.system", "<Send AccountDB> Error Stat!");
    return false;
}

/**
 * @brief 转发一条发往 GameDB 的包。
 * @param packet 已构造好的 DB 包。
 * @return 发送成功返回 true，否则返回 false。
 */
bool XLoginServer::SendDBGame(const XSendDBPacket& packet) {
    const int agentCount = m_xDBAgentMgr.GetGameDBAgentCount();
    const int nIndex = packet.GetOrderID() % agentCount;
    if (m_xDBAgentMgr.SendGameDBAgent(nIndex, packet)) {
        return true;
    }

    LogHelper::LogError("game.system", "<Send GameDB> Error Stat!");
    return false;
}

/**
 * @brief 转发一条发往 LogDB 的包。
 * @param packet 已构造好的 LogDB 包。
 * @return 发送成功返回 true，否则返回 false。
 */
bool XLoginServer::SendDBLog(const XSendDBPacket& packet) {
    if (GetOption().GetSystemType() == SYSTEM_TYPE_DEV) {
        return SendDBGame(packet);
    }

    const int agentCount = m_xDBAgentMgr.GetLogDBAgentCount();
    const int nIndex = packet.GetOrderID() % agentCount;
    if (m_xDBAgentMgr.SendLogDBAgent(nIndex, packet)) {
        return true;
    }

    LogHelper::LogError("game.system", "<Send LogDB> Error Stat!");
    return false;
}

/**
 * @brief 转发一条发往 StatisticsDB 的包。
 * @param packet 已构造好的统计 DB 包。
 * @return 发送成功返回 true，否则返回 false。
 */
bool XLoginServer::SendDBStatistics(const XSendDBPacket& packet) {
    if (GetOption().GetSystemType() == SYSTEM_TYPE_DEV) {
        return SendDBGame(packet);
    }

    const int agentCount = m_xDBAgentMgr.GetStatisticsDBAgentCount();
    const int nIndex = packet.GetOrderID() % agentCount;
    if (m_xDBAgentMgr.SendStatisticsDBAgent(nIndex, packet)) {
        return true;
    }

    LogHelper::LogError("game.system", "<Send StatisticsDB> Error Stat!");
    return false;
}

/**
 * @brief 把用户登记进在线表。
 * @param user 玩家对象。
 * @return 若命中更新的旧会话则返回 false，否则返回 true。
 */
bool XLoginServer::EnterUser(CUser* user) {
    if (!user || user->GetUAID() == 0) {
        return false;
    }

    std::unique_lock<std::shared_mutex> autolock(usersByUaidLock_);
    const auto existingIt = usersByUaid_.find(user->GetUAID());
    if (existingIt != usersByUaid_.end()) {
        CUser* existingUser = existingIt->second;
        LogHelper::LogDebug("game.system",
                            "GreenDamTan_log LoginServer.cpp::XLoginServer::EnterUser session=%d uaid=%d existing=%p existingSession=%d existingAuth=%llu newAuth=%llu",
                            user->GetSessionID(),
                            user->GetUAID(),
                            static_cast<void*>(existingUser),
                            existingUser ? existingUser->GetSessionID() : -1,
                            existingUser ? static_cast<unsigned long long>(existingUser->GetAuthSessionID()) : 0ull,
                            static_cast<unsigned long long>(user->GetAuthSessionID()));
        if (existingUser && existingUser->GetAuthSessionID() != 0 && user->GetAuthSessionID() != 0) {
            if (existingUser->GetAuthSessionID() > user->GetAuthSessionID()) {
                LogHelper::LogDebug("game.system",
                                    "GreenDamTan_log LoginServer.cpp::XLoginServer::EnterUser reject-older session=%d uaid=%d existingAuth=%llu newAuth=%llu",
                                    user->GetSessionID(),
                                    user->GetUAID(),
                                    static_cast<unsigned long long>(existingUser->GetAuthSessionID()),
                                    static_cast<unsigned long long>(user->GetAuthSessionID()));
                return false;
            }

            PS_KICK_USER_INFO kickInfo{};
            kickInfo.dwUAID = static_cast<unsigned int>(existingUser->GetUAID());
            kickInfo.byKickType = 1;
            existingUser->Kickout(kickInfo);
            existingUser->SetDeleteUserInfo(false);
            usersByUaid_.erase(existingIt);
        }
    }

    user->SetDeleteUserInfo(true);
    usersByUaid_.emplace(user->GetUAID(), user);
    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginServer.cpp::XLoginServer::EnterUser inserted session=%d uaid=%d size=%zu",
                        user->GetSessionID(),
                        user->GetUAID(),
                        usersByUaid_.size());
    return true;
}

/**
 * @brief 根据 UAID 查找在线用户。
 * @param uaid 账号 UAID。
 * @return 找到则返回用户指针，否则返回空。
 */
CUser* XLoginServer::FindUIDToUser(int uaid) {
    std::shared_lock<std::shared_mutex> autolock(usersByUaidLock_);
    const auto it = usersByUaid_.find(uaid);
    return it == usersByUaid_.end() ? nullptr : it->second;
}

/**
 * @brief 记录角色 UCID 到当前连接对象的映射。
 * @param ucid 角色 ID。
 * @param user 当前连接对象。
 */
void XLoginServer::AddActor(unsigned int ucid, CUser* user) {
    if (!user || ucid == 0) {
        return;
    }

    std::unique_lock<std::shared_mutex> autolock(usersByActorIdLock_);
    usersByActorId_.emplace(ucid, user);
}

/**
 * @brief 根据角色 UCID 查找当前登录连接。
 * @param ucid 角色 ID。
 * @return 找到则返回用户指针，否则返回空。
 */
CUser* XLoginServer::FindActor(unsigned int ucid) {
    std::shared_lock<std::shared_mutex> autolock(usersByActorIdLock_);
    const auto it = usersByActorId_.find(ucid);
    return it == usersByActorId_.end() ? nullptr : it->second;
}

void XLoginServer::RemoveActor(unsigned int dwUCID) {
    std::unique_lock<std::shared_mutex> autolock(usersByActorIdLock_);
    const auto it = usersByActorId_.find(dwUCID);
    if (it != usersByActorId_.end()) {
        usersByActorId_.erase(it);
    }
}

void XLoginServer::ExitUser(CUser* pUser) {
    if (!pUser) {
        return;
    }

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log LoginServer.cpp::XLoginServer::ExitUser begin session=%d uaid=%d delete=%d ptr=%p",
                        pUser->GetSessionID(),
                        pUser->GetUAID(),
                        pUser->GetDeleteUserInfo() ? 1 : 0,
                        static_cast<void*>(pUser));

    {
        std::unique_lock<std::shared_mutex> autolock(usersByUaidLock_);
        const auto it = usersByUaid_.find(pUser->GetUAID());
        if (it != usersByUaid_.end() && it->second == pUser) {
            usersByUaid_.erase(it);
            LogHelper::LogDebug("game.system",
                                "GreenDamTan_log LoginServer.cpp::XLoginServer::ExitUser erased session=%d uaid=%d remaining=%zu",
                                pUser->GetSessionID(),
                                pUser->GetUAID(),
                                usersByUaid_.size());
        } else {
            LogHelper::LogDebug("game.system",
                                "GreenDamTan_log LoginServer.cpp::XLoginServer::ExitUser skip-erase session=%d uaid=%d found=%d samePtr=%d",
                                pUser->GetSessionID(),
                                pUser->GetUAID(),
                                it != usersByUaid_.end() ? 1 : 0,
                                (it != usersByUaid_.end() && it->second == pUser) ? 1 : 0);
        }
    }

    if (GetOption().GetSecurityType() == SECURITY_ON) {
        m_xignCode.DisconnectUser(pUser->GetSessionID());
    }
}

void XLoginServer::KickoutAll(std::uint8_t byType) {
    std::vector<CUser*> users;
    {
        std::shared_lock<std::shared_mutex> autolock(usersByUaidLock_);
        users.reserve(usersByUaid_.size());
        for (const auto& entry : usersByUaid_) {
            if (entry.second) {
                users.push_back(entry.second);
            }
        }
    }

    for (CUser* user : users) {
        PS_KICK_USER_INFO kickInfo{};
        kickInfo.dwUAID = static_cast<unsigned int>(user->GetUAID());
        kickInfo.byKickType = byType;
        user->Kickout(kickInfo);
    }
}

/**
 * @brief 根据基础参数组装 `ST_LOG_GAME` 并发往 LogDB。
 *
 * IDA 中该重载走 `main=0x42, sub=0`；
 * 当前骨架保留该协议方向，并继续把登录链常用字段同步到最小审计容器里。
 */
bool XLoginServer::WriteLogDB(int uaid,
                              int ucid,
                              std::uint16_t mainType,
                              std::uint16_t subType,
                              int param0,
                              int param1,
                              int param2,
                              int param3,
                              int param4,
                              std::int64_t param5,
                              std::int64_t param6,
                              const wchar_t* comment,
                              const wchar_t* comment2) {
    ST_LOG_GAME stLog{};
    stLog._nUAID = uaid;
    stLog._nUCID = ucid;
    stLog._sMainType = mainType;
    stLog._sSubType = subType;
    stLog.nParam0 = param0;
    stLog.nParam1 = param1;
    stLog.nParam2 = param2;
    stLog.nParam3 = param3;
    stLog.nParam4 = param4;
    stLog.nParam5 = param5;
    stLog.nParam6 = param6;
    CopyWideBuffer(stLog.szComment, comment);
    CopyWideBuffer(stLog.szComment2, comment2);

    loginAuditRecords_.push_back({uaid,
                                  static_cast<std::uint8_t>(subType),
                                  param1,
                                  static_cast<std::uint32_t>(param2),
                                  param6,
                                  comment ? std::wstring(comment) : std::wstring(),
                                  comment2 ? std::wstring(comment2) : std::wstring()});

    XSendDBPacket sendPacket(nullptr, kLogDBMainCmd, kLogDBBasicSubCmd);
    sendPacket << stLog;
    return SendDBLog(sendPacket);
}

/**
 * @brief 直接发送一份完整的 `ST_LOG_GAME` 到 LogDB。
 *
 * IDA 中该重载走 `main=0x42, sub=1`，
 * 主要用于建角流程里的细粒度物品日志。
 */
void XLoginServer::WriteLogDB(ST_LOG_GAME stLog) {
    XSendDBPacket sendPacket(nullptr, kLogDBMainCmd, kLogDBStructSubCmd);
    sendPacket << stLog;
    SendDBLog(sendPacket);
}

/**
 * @brief 把用户压入等待队列，并同步写回所有等待态字段。
 * @param user 玩家对象。
 * @param uaid 账号 UAID。
 * @param ticketToken 入服请求中附带的 SessionID / 票据。
 * @param groupId 上次服务器索引或当前登录服分组索引。
 */
void XLoginServer::PushWaitUser(CUser* user, int uaid, std::uint64_t ticketToken, std::uint16_t groupId) {
    if (!user) {
        return;
    }

    // 进入等待状态后，客户端状态机会先切到排队阶段。
    XClient::SetState(user, eStateEnterWait);
    user->SetUAID_Wait(static_cast<unsigned int>(uaid));
    user->SetTicket_Wait(m_nWaitTicket++);
    user->SetLastServerIndex_Wait(groupId);
    user->SetAuthSessionID_Wait(ticketToken);
    user->SetCancel_Wait(false);
    user->SetAddTime_Wait();
    waitQueue_.push_back(user);

    LogHelper::LogInfo("game.system", "PushWaitUser ( UAID : %d )", uaid);
}

/**
 * @brief 根据当前等待位置和服务器负载，判断是否需要给客户端发等待提示。
 * @param ticket 用户当前持有的等待票号。
 * @return true 表示应该发等待提示包；false 表示暂不需要。
 */
bool XLoginServer::CheckUserWaitCountSend(std::int64_t ticket) const {
    const std::int64_t relativeTicket = ticket - GetLastEnterWaitTicket();
    const bool shouldSend =
        (relativeTicket + GetSendGameDBUserCount() + m_nControlServerUserCount > m_nMaxServerUserCount) ||
        (relativeTicket > 50 - GetSendGameDBUserCount());

    LogHelper::LogInfo("game.system",
                       shouldSend ? "CheckUserWaitCountSend YES [T:%d] (%d/%d/%d) "
                                  : "CheckUserWaitCountSend NO [T:%d] (%d/%d/%d) ",
                       static_cast<int>(relativeTicket),
                       m_nControlServerUserCount,
                       GetSendGameDBUserCount(),
                       m_nMaxServerUserCount);
    return shouldSend;
}

void XLoginServer::UpdateMaxUserCount(int nMaxCount, int nServerUserCount) {
    if (nMaxCount < 6) {
        nMaxCount = 6;
    }
    if (nServerUserCount < 0) {
        nServerUserCount = 0;
    }

    m_nMaxServerUserCount = nMaxCount;
    m_nControlServerUserCount = nServerUserCount;
}

/**
 * @brief 从等待队列头部弹出一名用户。
 * @return 若队列非空则返回用户指针，否则返回空。
 */
CUser* XLoginServer::PopWaitUser() {
    if (waitQueue_.empty()) {
        return nullptr;
    }

    CUser* user = waitQueue_.front();
    waitQueue_.pop_front();
    return user;
}

/**
 * @brief 增减当前已送往 GameDB 的等待用户数。
 * @param delta 增量，可为负数。
 */
void XLoginServer::AddSendGameDBUserCount(int delta) {
    m_nSendGameDBUserCount += delta;
    if (m_nSendGameDBUserCount < 0) {
        m_nSendGameDBUserCount = 0;
    }
}

/**
 * @brief 生成当前世界日期结构。
 * @param outDate 输出的时间信息。
 */
void XLoginServer::GetCurDate(ST_WORLD_CUR_DATE& outDate) const {
    const auto now = std::chrono::system_clock::now();
    const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &nowTime);
#else
    localTime = *std::localtime(&nowTime);
#endif

    outDate.n64CurDate = static_cast<std::int64_t>(nowTime);
    outDate.shYear = static_cast<std::int16_t>(localTime.tm_year + 1900);
    outDate.shMonth = static_cast<std::int16_t>(localTime.tm_mon + 1);
    outDate.shDay = static_cast<std::int16_t>(localTime.tm_mday);
    outDate.shHour = static_cast<std::int16_t>(localTime.tm_hour);
    outDate.shMin = static_cast<std::int16_t>(localTime.tm_min);
    outDate.shSec = static_cast<std::int16_t>(localTime.tm_sec);
    outDate.shDST = static_cast<std::int16_t>(localTime.tm_isdst);
}

std::int64_t XLoginServer::GetCurDate() const {
    ST_WORLD_CUR_DATE outDate{};
    GetCurDate(outDate);
    return outDate.n64CurDate;
}

/**
 * @brief 处理等待队列，把一部分玩家推进到入服 DB 阶段。
 * @param biTick 当前调度时刻，单位毫秒。
 */
void XLoginServer::ProcessWaitUser(std::uint64_t biTick) {
    int dispatchBudget = nRand(30, 50);
    const int onlineAndPendingCount = m_nControlServerUserCount + GetSendGameDBUserCount();
    if (onlineAndPendingCount < m_nMaxServerUserCount) {
        dispatchBudget = std::min(dispatchBudget, m_nMaxServerUserCount - onlineAndPendingCount);
        dispatchBudget = std::min(dispatchBudget, 50 - GetSendGameDBUserCount());
    } else {
        dispatchBudget = 0;
    }

    LogHelper::LogDebug("game.system",
                        "ProcessWaitUser [T:%d] (%d/%d/%d) ",
                        dispatchBudget,
                        m_nControlServerUserCount,
                        GetSendGameDBUserCount(),
                        m_nMaxServerUserCount);

    while (dispatchBudget > 0) {
        CUser* user = PopWaitUser();
        if (!user) {
            break;
        }

        m_nLastEnterWaitTicket = user->GetTicket_Wait();
        XClient::ClearState(user, eStateEnterWait);
        if (user->IsCancel_Wait()) {
            continue;
        }

        --dispatchBudget;
        LogHelper::LogInfo("game.system", "PopWaitUser ( UAID : %d )", user->GetUAID_Wait());

        XClient::SetState(user, eStateEnterWaitDB);
        user->SetEnterServerState(ENTER_SERVER_STATE_SELECT_WORLD_REQ);

        // 和 `ReqEnterServer` 直通分支一致，等待放行后也会走 `sub=0x11` 的 AccountDB 请求。
        XSendDBPacket sendPacket(user, eCMD_LOGIN, 0x11);
        sendPacket.XParse << static_cast<int>(user->GetUAID_Wait());
        sendPacket.XParse << user->GetLastServerIndex_Wait();
        sendPacket.XParse << user->GetAuthSessionID_Wait();
        sendPacket.XParse << 0;
        SendDBAccount(sendPacket);
        AddSendGameDBUserCount(1);
    }

    if (m_nNextWaitNotifyTick == 0) {
        m_nNextWaitNotifyTick = biTick + 3000;
    }

    if (m_nNextWaitNotifyTick < biTick) {
        for (CUser* user : waitQueue_) {
            if (!user || user->IsCancel_Wait()) {
                continue;
            }
            if (!XClient::IsState(user, eStateEnterWait) || !user->IsSendWaitPacket()) {
                continue;
            }

            const int waitPosition = static_cast<int>(user->GetTicket_Wait() - m_nLastEnterWaitTicket);
            XSendPacket waitPacket(eCMD_LOGIN, eSUB_CMD_ENTER_WAIT_NOTIFY);
            waitPacket.XParse << waitPosition;
            user->BridgeSend(waitPacket);
        }

        m_nNextWaitNotifyTick = biTick + 3000;
    }
}

/**
 * @brief 更新控制服下发的内容开关集合。
 * @param contentsInfo 控制服发送的内容开关结构。
 */
void XLoginServer::SendServerOption_SecondPW(const PS_CONTENTS_INFO* contentsInfo) {
    if (!contentsInfo) {
        return;
    }

    m_stContentsInfo = *contentsInfo;
    resourceMgr_.SetServerContents(m_stContentsInfo);

    std::vector<CUser*> onlineUsers;
    {
        std::shared_lock<std::shared_mutex> userLock(usersByUaidLock_);
        onlineUsers.reserve(usersByUaid_.size());
        for (const auto& [uaid, user] : usersByUaid_) {
            (void)uaid;
            if (user) {
                onlineUsers.push_back(user);
            }
        }
    }

    for (CUser* user : onlineUsers) {
        XSendPacket sendPacket(1, 7);
        sendPacket << m_stContentsInfo;
        user->BridgeSend(sendPacket);
    }

    LogHelper::LogInfo("game.system",
                       "RecvServerOptionUpdate: secondPW=%d waitSystem=%d broadcast=%zu",
                       static_cast<int>(m_stContentsInfo.bContents[E_SERVER_OPTION_SECOND_PW]),
                       static_cast<int>(m_stContentsInfo.bContents[E_SERVER_OPTION_WAIT_SYSTEM]),
                       onlineUsers.size());
}

/**
 * @brief 向客户端下发服务器组列表以及该账号在各组的角色数。
 * @param pUser 目标客户端。
 * @param stInfos 数据库返回的服务器角色计数列表。
 */
void XLoginServer::SendServerGroupList(CUser* pUser, ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC& stInfos) {
    if (!pUser) {
        return;
    }

    PS_SERVERS_INFO_FOR_USER serverInfos{};
    ST_SERVER_INFO_FOR_USER serverInfo{};

    serverInfo.wID = GetOption().GetGroupID();
    serverInfo.nState = 1;
    serverInfo.byCharacterCount = 0;
    std::snprintf(serverInfo.szName, sizeof(serverInfo.szName), "%s", GetOption().GetPublicIP());
    std::snprintf(serverInfo.szPublicIP, sizeof(serverInfo.szPublicIP), "%s", GetOption().GetPublicIP());
    serverInfo.sPort = static_cast<std::int16_t>(GetOption().GetPort());

    for (const ST_USER_CHARACTER_COUNT_FOR_SERVER& info : stInfos.vecInfo) {
        if (info.nServerGroupID == static_cast<int>(GetOption().GetGroupID())) {
            serverInfo.byCharacterCount = static_cast<std::uint8_t>(info.wCount);
            break;
        }
    }

    LogHelper::LogDebug("game.system",
                        "SendServerGroupList uaid=%u group=%u lastServer=%d selectedCount=%u itemCount=%zu",
                        pUser->GetUAID(),
                        static_cast<unsigned int>(serverInfo.wID),
                        stInfos.nLastServerID,
                        static_cast<unsigned int>(serverInfo.byCharacterCount),
                        stInfos.vecInfo.size());

    // IDA 中 `SendServerGroupList` 没有消费 `ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC::nLastServerID`。
    // 结合 DBAgent 侧 `ST_USER_CHARACTER_COUNT_FOR_SERVER_VEC` 构造函数也没有初始化该字段，
    // 当前必须保留客户端下发值为 0，不能把未初始化栈垃圾的低字节转发给客户端。
    serverInfos.byLastServerID = 0;
    serverInfos.vecServerInfo.push_back(serverInfo);

    XSendPacket sendPacket(eCMD_LOGIN, 4);
    sendPacket << serverInfos;
    pUser->BridgeSend(sendPacket);

    WriteLogDB(pUser->GetUAID(), 0, 1, 15, 0, 0, 0, 0, 0, 0, 0, L"", L"");
}
