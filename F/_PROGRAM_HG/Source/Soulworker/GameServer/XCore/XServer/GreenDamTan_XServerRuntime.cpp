#include "Soulworker/GameServer/XCore/XServer/XServer.h"
#include "Soulworker/GameServer/XCore/XServer/TXDBSocket.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <regex>
#include <thread>

#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_LogHelper.h"
#include "Soulworker/GameServer/XCore/XServer/GreenDamTan_ClientBase.h"

#ifdef _WIN32
#include <conio.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace {
constexpr std::size_t kDNSIndexAuth = 0;
constexpr std::size_t kDNSIndexGame = 1;
constexpr std::size_t kDNSIndexCommon = 2;
constexpr std::size_t kDNSIndexLog = 3;
constexpr std::size_t kDNSIndexGMTool = 4;
constexpr std::size_t kDNSIndexObserve = 5;
constexpr std::size_t kDNSIndexStatistics = 7;
constexpr std::size_t kDNSIndexSGNetCafe = 8;

std::uint64_t GetCurrentTickMs() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

template <typename T>
void DeleteOwnedObject(T*& pointer) {
    delete pointer;
    pointer = nullptr;
}

template <typename T>
void ReleaseRawBuffer(T*& pointer) {
    GreenDamTan_GlobalFreeRaw(pointer);
    pointer = nullptr;
}

class SimpleLockGuard {
public:
    explicit SimpleLockGuard(CSimpleLock& lock) : lock_(lock) {
        lock_.Lock();
    }

    ~SimpleLockGuard() {
        lock_.UnLock();
    }

    SimpleLockGuard(const SimpleLockGuard&) = delete;
    SimpleLockGuard& operator=(const SimpleLockGuard&) = delete;

private:
    CSimpleLock& lock_;
};

struct NetworkParamsSnapshot {
    std::string hostName;
    std::string scopeName;
    std::string nodeTypeText = "Unknown Node(0)";
    bool routingEnabled = false;
    bool winsProxyEnabled = false;
    bool valid = false;
};

std::string DescribeNodeType(unsigned int nodeType) {
    switch (nodeType) {
    case 1:
        return "Broadcast Node";
    case 2:
        return "Peer To Peer Node";
    case 4:
        return "Mixed Node";
    case 8:
        return "Hybrid Node";
    default: {
        char buffer[64] = {};
        std::snprintf(buffer, sizeof(buffer), "Unknown Node(%0x)", nodeType);
        return buffer;
    }
    }
}

void ForceCloseSocket(std::intptr_t socketHandle) {
    if (socketHandle < 0) {
        return;
    }

    linger lingerOption{};
    lingerOption.l_onoff = 1;
    lingerOption.l_linger = 0;
#ifdef _WIN32
    ::setsockopt(static_cast<SOCKET>(socketHandle),
                 SOL_SOCKET,
                 SO_LINGER,
                 reinterpret_cast<const char*>(&lingerOption),
                 static_cast<int>(sizeof(lingerOption)));
    ::closesocket(static_cast<SOCKET>(socketHandle));
#else
    ::setsockopt(static_cast<int>(socketHandle),
                 SOL_SOCKET,
                 SO_LINGER,
                 &lingerOption,
                 static_cast<socklen_t>(sizeof(lingerOption)));
    ::close(static_cast<int>(socketHandle));
#endif
}

void ForceShutdownSocket(std::intptr_t socketHandle) {
    if (socketHandle < 0) {
        return;
    }
#ifdef _WIN32
    ::shutdown(static_cast<SOCKET>(socketHandle), SD_BOTH);
#else
    ::shutdown(static_cast<int>(socketHandle), SHUT_RDWR);
#endif
}

bool SetSocketNonBlocking(std::intptr_t socketHandle) {
    if (socketHandle < 0) {
        return false;
    }
#ifdef _WIN32
    u_long enabled = 1;
    return ::ioctlsocket(static_cast<SOCKET>(socketHandle), FIONBIO, &enabled) == 0;
#else
    const int flags = ::fcntl(static_cast<int>(socketHandle), F_GETFL, 0);
    if (flags == -1) {
        return false;
    }
    return ::fcntl(static_cast<int>(socketHandle), F_SETFL, flags | O_NONBLOCK) == 0;
#endif
}

bool SetSocketCommonOptions(std::intptr_t socketHandle) {
    if (socketHandle < 0) {
        return false;
    }

    int nNodelay = 1;
    int nZero = 0;
    linger lingerOption{};
    lingerOption.l_onoff = 1;
    lingerOption.l_linger = 0;
#ifdef _WIN32
    const SOCKET sock = static_cast<SOCKET>(socketHandle);
    return ::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&nNodelay), sizeof(nNodelay)) != -1 &&
           ::setsockopt(sock, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&nZero), sizeof(nZero)) != -1 &&
           ::setsockopt(sock, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&nZero), sizeof(nZero)) != -1 &&
           ::setsockopt(sock, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&lingerOption), sizeof(lingerOption)) != -1;
#else
    const int sock = static_cast<int>(socketHandle);
    return ::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &nNodelay, sizeof(nNodelay)) != -1 &&
           ::setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &nZero, sizeof(nZero)) != -1 &&
           ::setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &nZero, sizeof(nZero)) != -1 &&
           ::setsockopt(sock, SOL_SOCKET, SO_LINGER, &lingerOption, sizeof(lingerOption)) != -1;
#endif
}

bool WaitSocketReadable(std::intptr_t socketHandle, int timeoutMs) {
    if (socketHandle < 0) {
        return false;
    }

    fd_set readSet;
    FD_ZERO(&readSet);
#ifdef _WIN32
    const SOCKET sock = static_cast<SOCKET>(socketHandle);
    FD_SET(sock, &readSet);
#else
    const int sock = static_cast<int>(socketHandle);
    FD_SET(sock, &readSet);
#endif

    timeval timeout{};
    timeout.tv_sec = timeoutMs / 1000;
    timeout.tv_usec = (timeoutMs % 1000) * 1000;
#ifdef _WIN32
    return ::select(0, &readSet, nullptr, nullptr, &timeout) > 0;
#else
    return ::select(sock + 1, &readSet, nullptr, nullptr, &timeout) > 0;
#endif
}

int ReadSocketBytes(std::intptr_t socketHandle, char* buffer, int length) {
    if (socketHandle < 0 || !buffer || length <= 0) {
        return -1;
    }
#ifdef _WIN32
    return ::recv(static_cast<SOCKET>(socketHandle), buffer, length, 0);
#else
    return ::recv(static_cast<int>(socketHandle), buffer, static_cast<std::size_t>(length), 0);
#endif
}

bool IsSocketWouldBlock() {
#ifdef _WIN32
    const int error = WSAGetLastError();
    return error == WSAEWOULDBLOCK || error == WSAEINPROGRESS;
#else
    return errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS;
#endif
}

bool SendSocketAll(std::intptr_t socketHandle, const char* buffer, int length) {
    if (socketHandle < 0 || !buffer || length <= 0) {
        return false;
    }

    int totalSent = 0;
    while (totalSent < length) {
#ifdef _WIN32
        const int sent = ::send(static_cast<SOCKET>(socketHandle), buffer + totalSent, length - totalSent, 0);
#else
        const int sent = static_cast<int>(::send(static_cast<int>(socketHandle), buffer + totalSent, static_cast<std::size_t>(length - totalSent), 0));
#endif
        if (sent > 0) {
            totalSent += sent;
            continue;
        }
        if (sent == 0) {
            return false;
        }
        if (!IsSocketWouldBlock()) {
            return false;
        }
        if (!WaitSocketReadable(socketHandle, 100)) {
            return false;
        }
    }
    return true;
}

} // namespace

NetworkParamsSnapshot QueryNetworkParams() {
    NetworkParamsSnapshot snapshot{};
#ifdef _WIN32
    ULONG bufferSize = sizeof(FIXED_INFO);
    std::vector<unsigned char> buffer(bufferSize);
    FIXED_INFO* fixedInfo = reinterpret_cast<FIXED_INFO*>(buffer.data());

    DWORD result = GetNetworkParams(fixedInfo, &bufferSize);
    if (result == ERROR_BUFFER_OVERFLOW) {
        buffer.resize(bufferSize);
        fixedInfo = reinterpret_cast<FIXED_INFO*>(buffer.data());
        result = GetNetworkParams(fixedInfo, &bufferSize);
    }

    if (result != ERROR_SUCCESS) {
        LogHelper::LogError("game.system", "GetNetworkParams ERROR error code[%u]", result);
        return snapshot;
    }

    snapshot.hostName = fixedInfo->HostName;
    snapshot.scopeName = fixedInfo->ScopeId;
    snapshot.nodeTypeText = DescribeNodeType(fixedInfo->NodeType);
    snapshot.routingEnabled = fixedInfo->EnableRouting != 0;
    snapshot.winsProxyEnabled = fixedInfo->EnableProxy != 0;
    snapshot.valid = true;
#else
    char hostName[256] = {};
    if (::gethostname(hostName, sizeof(hostName) - 1) == 0) {
        snapshot.hostName = hostName;
    }
    snapshot.valid = true;
#endif
    return snapshot;
}

bool XProcessComposite::InsertProcess(IXProcess* process) {
    if (!process) {
        return false;
    }

    if (!m_xMapComponet.m_AtlMap.Lookup(process->GetCmd())) {
        m_xMapComponet.m_AtlMap[process->GetCmd()] = process;
    }
    return true;
}

bool XProcessComposite::Init(XClient* client) {
    ATL::CAtlMap<std::uint8_t, IXProcess*>::CNode* node = m_xMapComponet.m_AtlMap.GetHeadPosition();
    while (node) {
        IXProcess* process = node->m_value;
        if (process && process->Init(client)) {
            node = m_xMapComponet.m_AtlMap.GetNext(node);
            continue;
        }

        const char* processName = process ? process->GetName().c_str() : "";
        LogHelper::LogError("game.system", "::Error Init false [%s]", processName);
        return false;
    }

    return true;
}

bool XProcessComposite::Parse(XPacket* packet) {
    if (!packet) {
        return false;
    }

    const std::uint8_t mainCmd = packet->GetMainCmd();
    const std::uint8_t subCmd = packet->GetSubCmd();
    ATL::CAtlMap<std::uint8_t, IXProcess*>::CNode* node = m_xMapComponet.m_AtlMap.LookupNode(mainCmd);
    if (!node || !node->m_value) {
        LogHelper::LogError("game.system",
                            "::Error Cannot Find Process [%02x][%02x]",
                            static_cast<unsigned int>(mainCmd),
                            static_cast<unsigned int>(subCmd));
        return false;
    }

    IXProcess* process = node->m_value;
    if (process->Parse(*packet)) {
        return true;
    }

    LogHelper::LogError("game.system",
                        "::Error Parse false [%s][%02x][%02x]",
                        process->GetName().c_str(),
                        static_cast<unsigned int>(mainCmd),
                        static_cast<unsigned int>(subCmd));
    return true;
}

XClient::~XClient() {
    TXMapUtil::DeletePtr<TXMap<std::uint8_t, IXProcess*>, IXProcess*>(m_xProcessComposite.m_xMapComponet);
    ReleaseRawBuffer(m_IoContextFrontBuffer);
    ReleaseRawBuffer(m_IoContextBackBuffer);
    DeleteOwnedObject(m_IoContextPool);
    while (!m_packetQueue.empty()) {
        m_packetQueue.pop();
    }
    m_socketContext.Destroy();
    xLock.Destroy();
}

bool XSocket::Init() {
    Socket = -1;
    std::memset(&scAddr, 0, sizeof(scAddr));
    eBlock = eBLOCK_OFF;
    std::memset(szBuffer.data(), 0, szBuffer.size());
    usSize = static_cast<std::uint16_t>(szBuffer.size());
    usOffset = 0;
    usInternal = 0;
    usInternalHigh = 0;
    m_nSendCount = 0;
    m_dwTick = GetCurrentTickMs();
    xLock.Init();
    return true;
}

bool XClient::Init() {
    m_pIOCPServer = nullptr;
    m_eNetState = eStateNone;
    m_nJobCount = 0;
    m_nLogBuffSize = 0;
    m_nTotalSendCount = 0;
    m_bEncrypt = false;
    m_bInit = false;
    SetSessionID(0);
    m_socketContext.Init();
    return XSocket::Init();
}

bool XClient::Init(XIOCPServer* pIOCPServer) {
    m_pIOCPServer = pIOCPServer;
    m_nLogBuffSize = 0;
    m_socketContext.m_overLab.Init(Socket, XOverLab::eOVERLAB_TYPE_NONE);
    m_socketContext.m_lock.Init();
    m_socketContext.mWSASendCnt = 0;
    m_bInit = true;
    return m_xProcessComposite.Init(this);
}

bool XClient::Register(std::uint8_t ucCmd, IXProcess* pProcess) {
    if (!pProcess) {
        return false;
    }

    if (ucCmd == pProcess->GetCmd()) {
        return m_xProcessComposite.InsertProcess(pProcess);
    }
    return true;
}

bool XClient::Parse(XPacket& xPacket) {
    const bool parseResult = m_xProcessComposite.Parse(&xPacket);
    if (!parseResult) {
        LogHelper::LogError("game.system",
                            "XClient::Parse fail main=%u sub=%u",
                            static_cast<unsigned int>(xPacket.GetMainCmd()),
                            static_cast<unsigned int>(xPacket.GetSubCmd()));
    }
    return parseResult;
}

bool XClient::SendEx(XSendPacket& xSendPacket) {
    if (!m_pIOCPServer) {
        return false;
    }

    ++m_nTotalSendCount;
    m_nLogBuffSize += xSendPacket.GetPayloadSize();
    return m_pIOCPServer->XSend(this, &xSendPacket);
}

// 对齐 IDA: XClient::SendErrorMessage - 构造错误包并走 SendEx 发出
// subCmd 高位置 0x80 表示错误响应，errorCode 序列化到包体
bool XClient::SendErrorMessage(std::uint8_t mainCmd, std::uint8_t subCmd, std::uint16_t errorCode) {
    XSendPacket sendPacket(mainCmd, static_cast<std::uint8_t>(subCmd | 0x80u));
    sendPacket.XParse << errorCode;
    return SendEx(sendPacket);
}


bool IsInlineClientOverLab(XSocket* pSocket, XOverLab* pOverLab) {
    XClient* client = dynamic_cast<XClient*>(pSocket);
    return client && pOverLab == &client->m_socketContext.m_overLab;
}

template <std::size_t N>
void CopyCString(char (&destination)[N], const std::string& source) {
    std::memset(destination, 0, sizeof(destination));
    if (source.empty()) {
        return;
    }

#ifdef _WIN32
    strncpy_s(destination, sizeof(destination), source.c_str(), _TRUNCATE);
#else
    std::strncpy(destination, source.c_str(), sizeof(destination) - 1);
#endif
}

template <std::size_t N>
void CopyCString(char (&destination)[N], const char* source) {
    CopyCString(destination, std::string(source ? source : ""));
}

std::string ReadTextFile(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        return {};
    }

    return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}

std::filesystem::path GetModuleDirectory() {
#ifdef _WIN32
    char modulePath[MAX_PATH] = {};
    GetModuleFileNameA(nullptr, modulePath, static_cast<DWORD>(std::size(modulePath)));
    return std::filesystem::path(modulePath).parent_path();
#elif defined(__linux__)
    std::error_code error;
    const std::filesystem::path exePath = std::filesystem::read_symlink("/proc/self/exe", error);
    if (!error && !exePath.empty()) {
        return exePath.parent_path();
    }
    return std::filesystem::current_path();
#else
    return std::filesystem::current_path();
#endif
}

std::filesystem::path FindConfigPath() {
    const std::filesystem::path moduleDir = GetModuleDirectory();
    const std::filesystem::path currentDir = std::filesystem::current_path();
    const std::array<std::filesystem::path, 4> candidates = {
        moduleDir / "config" / "Config.xml",
        moduleDir / "config" / "config.XML",
        currentDir / "config" / "Config.xml",
        currentDir / "config" / "config.XML",
    };

    for (const std::filesystem::path& candidate : candidates) {
        if (std::filesystem::exists(candidate)) {
            return candidate;
        }
    }

    return {};
}

std::string StripXmlComments(const std::string& text) {
    return std::regex_replace(text, std::regex(R"(<!--[\s\S]*?-->)"), "");
}

struct XmlBlock {
    std::string attributes;
    std::string body;
};

bool FindFirstBlock(const std::string& xml, const std::string& tagName, XmlBlock* block) {
    const std::regex pattern(
        "<" + tagName + R"(\b([^>]*)>([\s\S]*?)</)" + tagName + ">",
        std::regex::icase);
    std::smatch match;
    if (!std::regex_search(xml, match, pattern)) {
        return false;
    }

    if (block) {
        block->attributes = match[1].str();
        block->body = match[2].str();
    }
    return true;
}

std::vector<XmlBlock> FindAllBlocks(const std::string& xml, const std::string& tagName) {
    const std::regex pattern(
        "<" + tagName + R"(\b([^>]*)>([\s\S]*?)</)" + tagName + ">",
        std::regex::icase);
    std::vector<XmlBlock> blocks;
    for (std::sregex_iterator it(xml.begin(), xml.end(), pattern), end; it != end; ++it) {
        blocks.push_back({(*it)[1].str(), (*it)[2].str()});
    }
    return blocks;
}

std::vector<std::string> FindAllElementAttributes(const std::string& xml, const std::string& tagName) {
    const std::regex pattern("<" + tagName + R"(\b([^>]*)/?>)", std::regex::icase);
    std::vector<std::string> attributes;
    for (std::sregex_iterator it(xml.begin(), xml.end(), pattern), end; it != end; ++it) {
        attributes.push_back((*it)[1].str());
    }
    return attributes;
}

bool FindElementAttributes(const std::string& xml, const std::string& tagName, std::string* attributes) {
    const std::regex pattern("<" + tagName + R"(\b([^>]*)/?>)", std::regex::icase);
    std::smatch match;
    if (!std::regex_search(xml, match, pattern)) {
        return false;
    }

    if (attributes) {
        *attributes = match[1].str();
    }
    return true;
}

std::string GetAttributeValue(const std::string& attributes, const std::string& name) {
    const std::regex pattern(name + R"ATTR(\s*=\s*"([^"]*)")ATTR", std::regex::icase);
    std::smatch match;
    if (!std::regex_search(attributes, match, pattern)) {
        return {};
    }
    return match[1].str();
}

int GetServerTypeFromName(const std::string& typeName) {
    if (typeName == "LOGIN") {
        return 1;
    }
    if (typeName == "MAZE") {
        return 2;
    }
    if (typeName == "RELAY" || typeName == "COMMUNITY") {
        return 3;
    }
    if (typeName == "LOG") {
        return 4;
    }
    if (typeName == "DBAGENT") {
        return 5;
    }
    if (typeName == "CONTROL") {
        return 6;
    }
    if (typeName == "GMTOOLAGENT") {
        return 7;
    }
    if (typeName == "OBSERVEAGENT") {
        return 8;
    }
    return 0;
}

int GetIntAttribute(const std::string& attributes, const std::string& name, int defaultValue = 0) {
    const std::string value = GetAttributeValue(attributes, name);
    return value.empty() ? defaultValue : std::atoi(value.c_str());
}

bool GetBoolAttribute(const std::string& attributes, const std::string& name, bool defaultValue = false) {
    const std::string value = GetAttributeValue(attributes, name);
    if (value.empty()) {
        return defaultValue;
    }
    return std::atoi(value.c_str()) > 0 || value == "ON" || value == "on";
}

std::string ComposeConnectionString(const std::string& driver,
                                    const std::string& server,
                                    const std::string& database,
                                    const std::string& uid,
                                    const std::string& password,
                                    bool appendDefaultPort) {
    std::string serverValue = server;
    // TODO: 推测结果：原版大多直接补 `,1433`，但当前常见配置经常已自带端口。
    // 为了兼容现有服端配置，这里仅在未显式给端口时追加默认端口。
    if (appendDefaultPort && serverValue.find(',') == std::string::npos) {
        serverValue += ",1433";
    }

    char buffer[2048] = {};
    std::snprintf(buffer,
                  sizeof(buffer),
                  "Driver={%s};Server=%s;Database=%s;Uid=%s;Pwd=%s;",
                  driver.c_str(),
                  serverValue.c_str(),
                  database.c_str(),
                  uid.c_str(),
                  password.c_str());
    return buffer;
}

std::uint32_t MakeServerID(const SERVER_SYSTEM_INFO& info) {
    return static_cast<std::uint32_t>(info.nChannel & 0xFF) |
           (static_cast<std::uint32_t>(info.nGroup & 0xFF) << 8) |
           (static_cast<std::uint32_t>(info.nType & 0xFFFF) << 16);
}

void FillServerCommonInfo(SERVER_SYSTEM_INFO& info, const XmlBlock& block) {
    const std::string typeName = GetAttributeValue(block.attributes, "TYPE");
    CopyCString(info.szName, typeName);
    info.nType = GetServerTypeFromName(typeName);
    info.nGroup = GetIntAttribute(block.attributes, "GROUP");
    info.nChannel = GetIntAttribute(block.attributes, "CHANNEL");
    info.nMaxUser = GetIntAttribute(block.attributes, "MAX_USER", GetIntAttribute(block.attributes, "MAX_RELAY"));
    info.bUserAuth = GetBoolAttribute(block.attributes, "USE_AUTH", false);
    info.nSubType = GetIntAttribute(block.attributes, "SUB_TYPE");
    info.nState = 1;
    info.dwID = MakeServerID(info);

    std::string commonAttributes;
    if (FindElementAttributes(block.body, "COMMON", &commonAttributes)) {
        CopyCString(info.szIP, GetAttributeValue(commonAttributes, "PRIVATE_IP"));
        CopyCString(info.szPublicIP, GetAttributeValue(commonAttributes, "PUBLIC_IP"));
        info.nPort = GetIntAttribute(commonAttributes, "PORT");
    }

    std::string systemAttributes;
    if (FindElementAttributes(block.body, "SYSTEM", &systemAttributes)) {
        info.nIoThread = GetIntAttribute(systemAttributes, "IO_THREAD");
        info.nLogicThread = GetIntAttribute(systemAttributes, "LOGIC_THREAD");
        info.nMaxIOPool = GetIntAttribute(systemAttributes, "MAX_IO_POOL", 1536);
        if (info.nMaxIOPool == 0) {
            info.nMaxIOPool = 1536;
        }
        info.nLimitIOPool = GetIntAttribute(systemAttributes, "LIMIT_IO_POOL");
        const std::string path = GetAttributeValue(systemAttributes, "PATH");
        if (!path.empty()) {
            CopyCString(info.szPath, path);
        }
    } else {
        info.nMaxIOPool = 1536;
    }
}

void FillServerDBAgentInfo(SERVER_SYSTEM_INFO& info, const std::string& xmlBody) {
    XmlBlock dbAgentBlock;
    if (!FindFirstBlock(xmlBody, "DBAGENT", &dbAgentBlock)) {
        return;
    }

    for (const std::string& dbAttributes : FindAllElementAttributes(dbAgentBlock.body, "DB")) {
        const std::string type = GetAttributeValue(dbAttributes, "TYPE");
        std::size_t slot = info.stDBAgentInfo.size();
        if (type == "GAME") {
            slot = 0;
        } else if (type == "AUTH") {
            slot = 1;
        } else if (type == "LOG") {
            slot = 2;
        } else if (type == "STATISTICS") {
            slot = 3;
        } else if (type == "SG") {
            slot = 4;
        }

        if (slot >= info.stDBAgentInfo.size()) {
            continue;
        }

        ST_SERVER_INFO& dbInfo = info.stDBAgentInfo[slot];
        if (dbInfo.nType != 0) {
            continue;
        }

        CopyCString(dbInfo.szName, type);
        CopyCString(dbInfo.szIP, GetAttributeValue(dbAttributes, "IP"));
        dbInfo.nPort = GetIntAttribute(dbAttributes, "PORT");
        dbInfo.nType = static_cast<int>(slot) + 1;
        dbInfo.nMaxIOPool = GetIntAttribute(dbAttributes, "MAX_IO_POOL", 1536);
        dbInfo.nDBAgentCount = GetIntAttribute(dbAttributes, "DBAGENT_COUNT", 1);
        if (dbInfo.nDBAgentCount <= 0) {
            dbInfo.nDBAgentCount = 1;
        }
    }
}

void FillObjectPoolInfo(OBJECT_POOL_INFO& poolInfo, const std::string& xmlBody) {
    std::string objectAttributes;
    if (!FindElementAttributes(xmlBody, "OBJECT", &objectAttributes)) {
        return;
    }

    poolInfo.nPCMaxCount = GetIntAttribute(objectAttributes, "PC", 100);
    poolInfo.nMonsterMaxCount = GetIntAttribute(objectAttributes, "MONSTER", 1000);
    poolInfo.nNPCMaxCount = GetIntAttribute(objectAttributes, "NPC", 100);
    poolInfo.nDistrictMaxCount = GetIntAttribute(objectAttributes, "DISTRICT", 100);
    poolInfo.nMazeMaxCount = GetIntAttribute(objectAttributes, "MAZE", 100);
    poolInfo.nProjectileMaxCount = GetIntAttribute(objectAttributes, "PROJECTILE", 100);
    poolInfo.nTrapMaxCount = GetIntAttribute(objectAttributes, "TRAP", 100);
    poolInfo.nInteractionMaxCount = GetIntAttribute(objectAttributes, "INTERACTION", 100);
    poolInfo.nAkashicMaxCount = GetIntAttribute(objectAttributes, "AKASHIC", 100);
}
XClient* XClientPool::GetHead() {
    std::lock_guard<std::mutex> lock(m_xLock);
    if (m_xList.empty()) {
        m_itList = m_xList.end();
        return nullptr;
    }

    m_itList = m_xList.begin();
    return *m_itList;
}

XClient* XClientPool::GetNext() {
    std::lock_guard<std::mutex> lock(m_xLock);
    if (m_itList == m_xList.end()) {
        return nullptr;
    }

    ++m_itList;
    return m_itList == m_xList.end() ? nullptr : *m_itList;
}

XClientPool::~XClientPool() {
    GreenDamTan_Clear();
    for (XClient* client : m_xOwnedList) {
        delete client;
    }
    m_xOwnedList.clear();
}

XClient* XClientPool::AllocClient(std::intptr_t sc) {
    XClient* client = nullptr;
    ClientCreator creator;
    {
        std::lock_guard<std::mutex> lock(m_xLock);
        if (!m_xFreeList.empty()) {
            client = m_xFreeList.back();
            m_xFreeList.pop_back();
        } else {
            creator = m_creator;
            if (!creator) {
                // TODO: inferred - the original `TXPool<XClient>::Pop` uses a fixed-capacity pool.
                client = new XClient();
                m_xOwnedList.push_back(client);
            }
        }
    }

    if (!client && creator) {
        client = creator();
    }

    if (!client) {
        LogHelper::LogError("game.system", "empty XClient Pool");
        return nullptr;
    }

    {
        SimpleLockGuard clientLock(client->xLock);
        const int sessionID = client->GetSessionID();
        client->Init();
        client->SetSessionID(sessionID);
        client->Socket = sc;
        client->eBlock = XSocket::eBLOCK_OFF;
        client->m_dwTick = GetCurrentTickMs();
        client->m_socketContext.m_overLab.Init(sc, XOverLab::eOVERLAB_TYPE_READ);
    }

    GreenDamTan_LinkClient(client);
    return client;
}

void XClientPool::FreeClient(XClient* client) {
    if (!client) {
        LogHelper::LogError("game.system", "FreeClient pClient NULL");
        return;
    }

    ClientRecycler recycler;
    {
        std::lock_guard<std::mutex> lock(m_xLock);
        m_xList.remove(client);
        m_itList = m_xList.end();
        recycler = m_recycler;
    }

    {
        SimpleLockGuard clientLock(client->xLock);
        const int sessionID = client->GetSessionID();
        ForceCloseSocket(client->Socket);
        client->Init();
        if (recycler) {
            client->SetSessionID(sessionID);
            recycler(client);
        }
    }

    if (!recycler) {
        std::lock_guard<std::mutex> lock(m_xLock);
        if (std::find(m_xFreeList.begin(), m_xFreeList.end(), client) == m_xFreeList.end()) {
            m_xFreeList.push_back(client);
        }
    }
}

void XClientPool::GreenDamTan_LinkClient(XClient* client) {
    if (!client) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_xLock);
    if (std::find(m_xList.begin(), m_xList.end(), client) == m_xList.end()) {
        m_xList.push_back(client);
    }
}

void XClientPool::GreenDamTan_UnlinkClient(XClient* client) {
    std::lock_guard<std::mutex> lock(m_xLock);
    m_xList.remove(client);
    m_itList = m_xList.end();
}

void XClientPool::GreenDamTan_Clear() {
    std::lock_guard<std::mutex> lock(m_xLock);
    m_xList.clear();
    m_xFreeList.clear();
    m_itList = m_xList.end();
}

void XIOCPServer::StartLog() {
    m_bLog = true;
}

XIOCPSkeleton::XIOCPSkeleton() {
    m_hIOCP = reinterpret_cast<void*>(-1);
    m_nIOCPWorkThread = 0;
    m_phIOCPWorkThread = nullptr;
    m_xIOPool.m_ePoolID = ePoolIDNone;
#ifdef _WIN32
    WSADATA wsaData{};
    // 按 IDA `??0XIOCPSkeleton@@QEAA@XZ @ 0x14003dd36`，构造函数末尾会调用 `WSAStartup(0x202, ...)`。
    ::WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

XIOCPSkeleton::~XIOCPSkeleton() {
    this->XIOCPSkeleton::Shutdown(0);
    m_phIOCPWorkThread = nullptr;
    m_xIOPool.Reset();
#ifdef _WIN32
    ::WSACleanup();
#endif
}

bool XIOCPSkeleton::CreateIOCP(const char* szAddr,
                               std::uint16_t usPort,
                               E_POOL_ID ePoolID,
                               int nIOCount) {
    m_xIOPool.Reset();
    m_xIOPool.m_ePoolID = ePoolID;
    m_xIOPool.m_nIOCount = nIOCount;
    m_nIOCPWorkThread = 4;

    std::memset(&m_scAddr, 0, sizeof(m_scAddr));
    m_scAddr.sin_family = AF_INET;
    m_scAddr.sin_port = htons(usPort);

    const char* pszAddr = szAddr ? szAddr : "";
    bool bResolved = pszAddr[0] == '\0';
    if (!bResolved) {
#ifdef _WIN32
        const unsigned long nIP = ::inet_addr(pszAddr);
        if (nIP != INADDR_NONE) {
            m_scAddr.sin_addr.s_addr = nIP;
            bResolved = true;
        }
#else
        if (::inet_pton(AF_INET, pszAddr, &m_scAddr.sin_addr) == 1) {
            bResolved = true;
        }
#endif
    }

    if (!bResolved && pszAddr[0] != '\0') {
        hostent* hostEntry = ::gethostbyname(pszAddr);
        if (hostEntry && hostEntry->h_addr_list && hostEntry->h_addr_list[0]) {
            std::memcpy(&m_scAddr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
            bResolved = true;
        }
    }

    if (!bResolved) {
        m_scAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    {
        std::lock_guard<std::mutex> lock(GreenDamTan_IOCPEventLock);
        GreenDamTan_IOCPEvents.clear();
    }
    GreenDamTan_ClearCompletionKeys();

    std::uint64_t handleSeed = ++GreenDamTan_hIOCPSeed;
    if (handleSeed == static_cast<std::uint64_t>(-1)) {
        handleSeed = ++GreenDamTan_hIOCPSeed;
    }
    m_hIOCP = reinterpret_cast<void*>(static_cast<std::uintptr_t>(handleSeed));

    GreenDamTan_IOCPThreads.clear();
    GreenDamTan_IOCPThreads.reserve(static_cast<std::size_t>(m_nIOCPWorkThread));
    for (int index = 0; index < m_nIOCPWorkThread; ++index) {
        GreenDamTan_IOCPThreads.emplace_back([this]() {
            WorkerThread();
        });
    }

    return true;
}

bool XIOCPSkeleton::BlockSocket(XSocket* pSocket, XOverLab* pOverLab, std::uint8_t ucCloseNo) {
    if (pSocket) {
        LogHelper::LogLegacy(
            "<SOCKET> BlockSocket ( %d / %d )",
            static_cast<int>(ucCloseNo),
            static_cast<int>(pSocket->Socket));
        if (pSocket->eBlock == XSocket::eBLOCK_OFF) {
            ForceShutdownSocket(pSocket->Socket);
            pSocket->eBlock = XSocket::eBLOCK_ON;
        }
    } else {
        LogHelper::LogLegacy("<BLOCK_SOCKET> IS_NULL<XOverLab>(pOverLab)");
    }

    if (pOverLab) {
        if (!IsInlineClientOverLab(pSocket, pOverLab)) {
            m_xIOPool.FreeIO(pOverLab);
        } else {
            pOverLab->usOffset = 0;
        }
    } else {
        LogHelper::LogLegacy("XIOCPSkeleton::FreeXIO :: IS_NULL<XOverLab>(pOverLab) 1 ");
    }

    return pSocket != nullptr;
}

bool XIOCPSkeleton::Shutdown(std::uint32_t /*maxWait*/) {
    if (m_hIOCP != reinterpret_cast<void*>(-1)) {
        GreenDamTan_PostExitEvents();
    }

    GreenDamTan_IOCPEventCv.notify_all();
    for (std::thread& worker : GreenDamTan_IOCPThreads) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    GreenDamTan_IOCPThreads.clear();

    {
        std::lock_guard<std::mutex> lock(GreenDamTan_IOCPEventLock);
        GreenDamTan_IOCPEvents.clear();
    }

    GreenDamTan_ClearCompletionKeys();
    m_xIOPool.Clear(0xFFFFFFFFu);
    m_nIOCPWorkThread = 0;
    m_hIOCP = reinterpret_cast<void*>(-1);
    return true;
}

std::uint32_t XIOCPSkeleton::WorkerThread() {
    while (true) {
        GreenDamTan_IOCP_EVENT event{};
        {
            std::unique_lock<std::mutex> lock(GreenDamTan_IOCPEventLock);
            GreenDamTan_IOCPEventCv.wait(lock, [this]() {
                return !GreenDamTan_IOCPEvents.empty();
            });
            event = GreenDamTan_IOCPEvents.front();
            GreenDamTan_IOCPEvents.pop_front();
        }

        if (event.eventType == GreenDamTan_IOCP_EVENT_TYPE::eExit) {
            break;
        }

        if (event.eventType == GreenDamTan_IOCP_EVENT_TYPE::eClose) {
            LogHelper::LogLegacy("IOCP_CLOSE [%d]", static_cast<int>(reinterpret_cast<std::intptr_t>(m_hIOCP)));
            continue;
        }

        XSocket* pSocket = reinterpret_cast<XSocket*>(event.completionKey);
        if (event.bSuccess && event.dwNumberOfBytesTransferred != 0) {
            if (event.pOverLab && event.pOverLab->eType == XOverLab::eOVERLAB_TYPE_READ) {
                OnRecv(pSocket, event.pOverLab, event.dwNumberOfBytesTransferred);
            } else if (event.pOverLab && event.pOverLab->eType == XOverLab::eOVERLAB_TYPE_WRITE) {
                OnSend(pSocket, event.pOverLab);
            } else {
                OnEtcEvent(event.pOverLab);
            }
            continue;
        }

        if (pSocket) {
            BlockSocket(pSocket, event.pOverLab, 0);
        } else {
            if (event.pOverLab) {
                m_xIOPool.FreeIO(event.pOverLab);
            } else {
                LogHelper::LogLegacy("XIOCPSkeleton::FreeXIO :: IS_NULL<XOverLab>(pOverLab) 1 ");
            }
            LogHelper::LogLegacy("<BLOCK_SOCKET> IS_NULL<XOverLab>(pOverLab)");
        }
    }

    return 0;
}

void XIOCPSkeleton::GreenDamTan_PostIOCPEvent(void* completionKey,
                                              XOverLab* pOverLab,
                                              std::uint32_t dwNumberOfBytesTransferred,
                                              bool bSuccess) {
    {
        std::lock_guard<std::mutex> lock(GreenDamTan_IOCPEventLock);
        GreenDamTan_IOCPEvents.push_back({GreenDamTan_IOCP_EVENT_TYPE::eCompletion,
                                          completionKey,
                                          pOverLab,
                                          dwNumberOfBytesTransferred,
                                          bSuccess});
    }
    GreenDamTan_IOCPEventCv.notify_one();
}

void XIOCPSkeleton::GreenDamTan_PostCloseEvent() {
    {
        std::lock_guard<std::mutex> lock(GreenDamTan_IOCPEventLock);
        GreenDamTan_IOCPEvents.push_back(
            {GreenDamTan_IOCP_EVENT_TYPE::eClose, nullptr, nullptr, 0, true});
    }
    GreenDamTan_IOCPEventCv.notify_one();
}

void XIOCPSkeleton::GreenDamTan_PostExitEvents() {
    {
        std::lock_guard<std::mutex> lock(GreenDamTan_IOCPEventLock);
        for (int index = 0; index < m_nIOCPWorkThread; ++index) {
            GreenDamTan_IOCPEvents.push_back(
                {GreenDamTan_IOCP_EVENT_TYPE::eExit, nullptr, nullptr, 0, true});
        }
    }
    GreenDamTan_IOCPEventCv.notify_all();
}

void XIOCPSkeleton::GreenDamTan_RegisterCompletionKey(std::intptr_t socketHandle, void* completionKey) {
    if (socketHandle < 0) {
        return;
    }

    std::lock_guard<std::mutex> lock(GreenDamTan_CompletionKeyLock);
    GreenDamTan_CompletionKeys[socketHandle] = completionKey;
}

void XIOCPSkeleton::GreenDamTan_UnregisterCompletionKey(std::intptr_t socketHandle) {
    if (socketHandle < 0) {
        return;
    }

    std::lock_guard<std::mutex> lock(GreenDamTan_CompletionKeyLock);
    GreenDamTan_CompletionKeys.erase(socketHandle);
}

void* XIOCPSkeleton::GreenDamTan_FindCompletionKey(std::intptr_t socketHandle) {
    std::lock_guard<std::mutex> lock(GreenDamTan_CompletionKeyLock);
    const auto it = GreenDamTan_CompletionKeys.find(socketHandle);
    return it != GreenDamTan_CompletionKeys.end() ? it->second : nullptr;
}

void XIOCPSkeleton::GreenDamTan_ClearCompletionKeys() {
    std::lock_guard<std::mutex> lock(GreenDamTan_CompletionKeyLock);
    GreenDamTan_CompletionKeys.clear();
}

void XIOCPSkeleton::OnSend(XSocket* /*pSocket*/, XOverLab* pOverLab) {
    if (!pOverLab) {
        LogHelper::LogLegacy(" <SOCKET> XTCPSkeleton::OnSend :: IS_NULL<XOverLab>(pOverLab)");
        return;
    }
    m_xIOPool.FreeIO(pOverLab);
}

bool XIOCPSkeleton::OnRecv(XSocket* /*pSocket*/,
                           XOverLab* /*pOverLab*/,
                           std::uint32_t /*dwNumberOfBytesTransferred*/) {
    return false;
}

void XIOCPSkeleton::OnEtcEvent(XOverLab* pOverLab) {
    if (pOverLab) {
        m_xIOPool.FreeIO(pOverLab);
    } else {
        LogHelper::LogLegacy("XTCPSkeleton::OnEtcEvent :: IS_NULL<XOverLab>(pOverLab) ");
    }
}

bool XTCPSkeleton::XSend(XOverLab* pOverLab) {
    if (!pOverLab) {
        LogHelper::LogLegacy("XTCPSkeleton::XSend :: IS_NULL<XOverLab>(pOverLab) ");
        return false;
    }

    if (pOverLab->Socket < 0 || pOverLab->usOffset == 0) {
        LogHelper::LogLegacy("XTCPSkeleton::XSend Failed! %d ", -1);
        m_xIOPool.FreeIO(pOverLab);
        return false;
    }

    if (!SendSocketAll(pOverLab->Socket, pOverLab->szBuffer.data(), pOverLab->usOffset)) {
#ifdef _WIN32
        const int errorCode = WSAGetLastError();
#else
        const int errorCode = errno;
#endif
        LogHelper::LogLegacy("XTCPSkeleton::XSend Failed! %d ", errorCode);
        m_xIOPool.FreeIO(pOverLab);
        return false;
    }

    this->GreenDamTan_PostIOCPEvent(this->GreenDamTan_FindCompletionKey(pOverLab->Socket),
                                    pOverLab,
                                    pOverLab->usOffset,
                                    true);
    return true;
}

bool XTCPSkeleton::XRecv(XOverLab* pOverLab) {
    if (!pOverLab) {
        LogHelper::LogError("game.system", "XTCPSkeleton::XRecv :: IS_NULL<XOverLab>(pOverLab) 2");
        return false;
    }

    const std::uint16_t usOffset = pOverLab->usOffset;
    if (usOffset > pOverLab->usSize || pOverLab->Socket < 0) {
        LogHelper::LogError("game.system", "XTCPSkeleton::XRecv Failed %d", -1);
        return false;
    }

    const std::uint16_t recvLength = static_cast<std::uint16_t>(pOverLab->usSize - usOffset);
    return recvLength > 0;
}

void XTCPSkeleton::OnSend(XSocket* pSocket, XOverLab* pOverLab) {
    if (!pOverLab) {
        LogHelper::LogLegacy(" <SOCKET> XTCPSkeleton::OnSend :: IS_NULL<XOverLab>(pOverLab)");
        return;
    }

    if (pSocket && m_nLimitIOPool > 0 && pSocket->m_nSendCount > 0) {
        --pSocket->m_nSendCount;
    }
    m_xIOPool.FreeIO(pOverLab);
}

bool XTCPSkeleton::OnRecv(XSocket* pSocket,
                          XOverLab* pOverLab,
                          std::uint32_t dwNumberOfBytesTransferred) {
    if (!pSocket) {
        return false;
    }

    SimpleLockGuard lock(pSocket->xLock);
    char* pWriteBuffer = pSocket->szBuffer.data() + pSocket->usOffset + pSocket->usInternalHigh;
    std::uint32_t dwWriteable = pSocket->usSize - pSocket->usOffset - pSocket->usInternalHigh;

    if (dwNumberOfBytesTransferred > 0xFFFFu) {
        LogHelper::LogLegacy(
            " <SOCKET> XTCPSkeleton::OnRecv :: if( TCP_PACKET_MAX_SIZE < dwNumberOfBytesSent ) %d ",
            static_cast<int>(pSocket->Socket));
        return false;
    }

    pOverLab->usOffset =
        static_cast<std::uint16_t>(pOverLab->usOffset + dwNumberOfBytesTransferred);
    std::uint16_t usRemain = pOverLab->usOffset;
    std::uint16_t usConsumed = 0;

    if (usRemain >= sizeof(PACKET_HEADER)) {
        while (true) {
            char* pPacketBuffer = pOverLab->szBuffer.data() + usConsumed;
            std::uint16_t usPacketSize = 0;
            std::memcpy(&usPacketSize, pPacketBuffer + 2, sizeof(usPacketSize));
            if (usPacketSize < sizeof(PACKET_HEADER)) {
                return this->BlockSocket(pSocket, pOverLab, 2);
            }
            if (usPacketSize > usRemain) {
                break;
            }

            std::uint16_t usVer = 0;
            std::memcpy(&usVer, pPacketBuffer, sizeof(usVer));
            if (usVer != 2) {
                return this->BlockSocket(pSocket, pOverLab, 0x0F);
            }

            if (usPacketSize > dwWriteable) {
                LogHelper::LogLegacy(
                    "<SOCKET> Socket Buffer Overflow ( %d / %d )",
                    static_cast<int>(usPacketSize),
                    static_cast<int>(dwWriteable));
                return this->BlockSocket(pSocket, pOverLab, 3);
            }

            if (usPacketSize > sizeof(PACKET_HEADER)) {
                char* pPayload = pPacketBuffer + sizeof(PACKET_HEADER);
                const std::uint16_t usPayloadSize =
                    static_cast<std::uint16_t>(usPacketSize - sizeof(PACKET_HEADER));
                for (std::uint16_t index = 0; index < usPayloadSize; ++index) {
                    pPayload[index] ^= static_cast<char>(SY_KEY_TABLE[static_cast<std::uint8_t>(usVer)][index % 3]);
                }
            }

            std::memcpy(pWriteBuffer, pPacketBuffer, usPacketSize);
            pWriteBuffer += usPacketSize;
            dwWriteable -= usPacketSize;
            pSocket->usInternalHigh =
                static_cast<std::uint16_t>(pSocket->usInternalHigh + usPacketSize);
            pSocket->m_dwTick = GetCurrentTickMs();

            usRemain = static_cast<std::uint16_t>(usRemain - usPacketSize);
            usConsumed = static_cast<std::uint16_t>(usConsumed + usPacketSize);
            if (usRemain < sizeof(PACKET_HEADER)) {
                break;
            }
        }
    }

    if (usConsumed != 0) {
        if (IsInlineClientOverLab(pSocket, pOverLab)) {
            if (usRemain > 0) {
                std::memmove(pOverLab->szBuffer.data(),
                             pOverLab->szBuffer.data() + usConsumed,
                             usRemain);
            }
            pOverLab->usOffset = usRemain;
        } else {
            XOverLab* pNewOverLab =
                m_xIOPool.AllocIO(pSocket->Socket, XOverLab::eOVERLAB_TYPE_READ);
            if (!pNewOverLab) {
                LogHelper::LogLegacy("<SOCKET> BlockSocket ( %d / %d )",
                                     20,
                                     static_cast<int>(pSocket->Socket));
                if (pSocket->eBlock == XSocket::eBLOCK_OFF) {
                    ForceShutdownSocket(pSocket->Socket);
                    pSocket->eBlock = XSocket::eBLOCK_ON;
                }
                LogHelper::LogLegacy("XIOCPSkeleton::FreeXIO :: IS_NULL<XOverLab>(pOverLab) 1 ");
                return false;
            }
            if (usRemain > 0) {
                std::memcpy(pNewOverLab->szBuffer.data(),
                            pOverLab->szBuffer.data() + usConsumed,
                            usRemain);
                pNewOverLab->usOffset = usRemain;
            }
            m_xIOPool.FreeIO(pOverLab);
            pOverLab = pNewOverLab;
        }
    }

    if (!XRecv(pOverLab)) {
        LogHelper::LogLegacy("<SOCKET> BlockSocket ( %d / %d )", 4, static_cast<int>(pSocket->Socket));
        if (pSocket->eBlock == XSocket::eBLOCK_OFF) {
            ForceShutdownSocket(pSocket->Socket);
            pSocket->eBlock = XSocket::eBLOCK_ON;
        }
        LogHelper::LogLegacy("XIOCPSkeleton::FreeXIO :: IS_NULL<XOverLab>(pOverLab) 1 ");
        return false;
    }

    return true;
}

void XTCPSkeleton::OnEtcEvent(XOverLab* pOverLab) {
    if (pOverLab) {
        m_xIOPool.FreeIO(pOverLab);
    } else {
        LogHelper::LogLegacy("XTCPSkeleton::OnEtcEvent :: IS_NULL<XOverLab>(pOverLab) ");
    }
}

bool XIOCPServer::OnAccect(XClient* pClient) {
    return pClient != nullptr;
}

XIOCPServer::~XIOCPServer() {
    Shutdown(0xFFFFFFFFu);
}

bool XIOCPServer::SetSocket() {
#ifdef _WIN32
    const SOCKET listenSocket = ::WSASocketA(AF_INET, SOCK_STREAM, 0, nullptr, 0, 1u);
    if (listenSocket == INVALID_SOCKET) {
        return false;
    }
    m_scListen = static_cast<std::intptr_t>(listenSocket);
#else
    const int listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        return false;
    }
    m_scListen = static_cast<std::intptr_t>(listenSocket);
    const int reuseAddr = 1;
    // TODO: 推测结果：Linux 侧重复启动调试时需要复用地址，原版 WinSock 无此分支。
    ::setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));
#endif

#ifdef _WIN32
    if (::bind(listenSocket, reinterpret_cast<const sockaddr*>(&m_scAddr), sizeof(m_scAddr)) == -1) {
#else
    if (::bind(listenSocket, reinterpret_cast<const sockaddr*>(&m_scAddr), sizeof(m_scAddr)) == -1) {
#endif
        ForceCloseSocket(m_scListen);
        m_scListen = -1;
        return false;
    }

    if (::listen(listenSocket, 0x7FFFFFFF) == -1) {
        ForceCloseSocket(m_scListen);
        m_scListen = -1;
        return false;
    }

    if (!SetSocketCommonOptions(m_scListen)) {
        ForceCloseSocket(m_scListen);
        m_scListen = -1;
        return false;
    }

    return true;
}

bool XIOCPServer::Init(E_POOL_ID ePoolID, const char* ip, std::uint16_t port, int maxIOPool) {
    m_ePoolID = ePoolID;
    m_bNetworkInitialized = false;
    m_bStopBackendThread = false;
    m_eState = 0;
    m_scListen = -1;
    m_wListenPort = port;
    m_nMaxIOPool = maxIOPool;
    m_nLimitIOPool = maxIOPool;
    m_dw64FPSTick_0 = 0;
    m_dwFrame_0 = 0;
    m_nFPSCount = 0;

    std::memset(m_szListenIP, 0, sizeof(m_szListenIP));
    if (ip) {
#ifdef _WIN32
        strncpy_s(m_szListenIP, sizeof(m_szListenIP), ip, _TRUNCATE);
#else
        std::strncpy(m_szListenIP, ip, sizeof(m_szListenIP) - 1);
#endif
    }

    if (!XIOCPSkeleton::CreateIOCP(m_szListenIP, port, ePoolID, maxIOPool)) {
        return false;
    }

    if (!SetSocket()) {
        LogHelper::LogLegacy("XIOCPServer::Init :: Listen Socket Init Fail.!");
        XIOCPSkeleton::Shutdown(0xFFFFFFFFu);
        return false;
    }

    if (m_hIOCPAcceptThread.joinable()) {
        if (m_hIOCPAcceptThread.get_id() == std::this_thread::get_id()) {
            m_hIOCPAcceptThread.detach();
        } else {
            m_hIOCPAcceptThread.join();
        }
    }
    if (m_hIOCPBackEndThread.joinable()) {
        if (m_hIOCPBackEndThread.get_id() == std::this_thread::get_id()) {
            m_hIOCPBackEndThread.detach();
        } else {
            m_hIOCPBackEndThread.join();
        }
    }

    m_hIOCPAcceptThread = std::thread([this]() {
        AcceptThread();
    });
    m_hIOCPBackEndThread = std::thread([this]() {
        BackendThreadProc();
    });
    m_bNetworkInitialized = true;
    m_eState = 1;
    return true;
}

std::uint32_t XIOCPServer::AcceptThread() {
    while (m_eState.load() != 2) {
        sockaddr_in scAddr{};
#ifdef _WIN32
        int nAddrLen = sizeof(scAddr);
        const SOCKET acceptedSocket =
            ::accept(static_cast<SOCKET>(m_scListen), reinterpret_cast<sockaddr*>(&scAddr), &nAddrLen);
        if (acceptedSocket == INVALID_SOCKET) {
            if (m_eState.load() == 2) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }
        const std::intptr_t acceptedHandle = static_cast<std::intptr_t>(acceptedSocket);
#else
        socklen_t nAddrLen = static_cast<socklen_t>(sizeof(scAddr));
        const int acceptedSocket =
            ::accept(static_cast<int>(m_scListen), reinterpret_cast<sockaddr*>(&scAddr), &nAddrLen);
        if (acceptedSocket < 0) {
            if (m_eState.load() == 2) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }
        const std::intptr_t acceptedHandle = static_cast<std::intptr_t>(acceptedSocket);
#endif

        XServer* server = dynamic_cast<XServer*>(this);
        if (!server) {
            ForceCloseSocket(acceptedHandle);
            return 0;
        }

        XClient* pClient = server->GetClientPool().AllocClient(acceptedHandle);
        if (!pClient) {
            LogHelper::LogError("game.system", "XIOCPServer::AcceptThread ==> m_xClientPool Empty");
            ForceCloseSocket(acceptedHandle);
            return 0;
        }

        SetSocketNonBlocking(acceptedHandle);
        pClient->scAddr = scAddr;
        GreenDamTan_RegisterCompletionKey(acceptedHandle, pClient);

        const bool accepted = XAccept(pClient);
        if (accepted) {
            // `XClient::Init(this)` 会重置 `m_socketContext`，必须在 `XAccept` 之后重新把
            // 当前跨平台骨架里复用的 inline recv overlab 设回 READ，避免完成事件被误判成 etc。
            pClient->m_socketContext.m_overLab.Init(pClient->Socket, XOverLab::eOVERLAB_TYPE_READ);
            pClient->m_socketContext.m_overLab.usOffset = pClient->usOffset;
        }

        if (accepted && this->XTCPSkeleton::XRecv(&pClient->m_socketContext.m_overLab)) {
            const char* pszAddr = inet_ntoa(scAddr.sin_addr);
            LogHelper::LogDebug("game.system",
                                "<SOCKET> Create Socket ( %lld / %s / %p )",
                                static_cast<long long>(pClient->Socket),
                                pszAddr ? pszAddr : "",
                                static_cast<void*>(pClient));
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        LogHelper::LogError("game.system",
                            "<SOCKET> BlockSocket ( %d / %lld )",
                            6,
                            static_cast<long long>(pClient->Socket));
        GreenDamTan_UnregisterCompletionKey(acceptedHandle);
        if (pClient->eBlock == XSocket::eBLOCK_OFF) {
            ForceShutdownSocket(pClient->Socket);
            pClient->eBlock = XSocket::eBLOCK_ON;
        }
        LogHelper::LogError("game.system", "XIOCPSkeleton::FreeXIO :: IS_NULL<XOverLab>(pOverLab) 1 ");
        server->GetClientPool().FreeClient(pClient);
    }

    return 0;
}

bool XIOCPServer::BackSends() {
    return true;
}

std::uint32_t XIOCPServer::BackendThreadProc() {
    while (m_eState.load() != 2) {
        const std::uint64_t currentTick = GetCurrentTickMs();
        XClient* blockedClient = nullptr;

        XServer* server = dynamic_cast<XServer*>(this);
        XClient* head = server ? server->GetClientPool().GetHead() : nullptr;
        while (head) {
            if (head->eBlock == XSocket::eBLOCK_OFF) {
                XOverLab* pRecvOverLab = &head->m_socketContext.m_overLab;
                if (pRecvOverLab->Socket == head->Socket && pRecvOverLab->usOffset < pRecvOverLab->usSize &&
                    WaitSocketReadable(head->Socket, 0)) {
                    const int recvLength = static_cast<int>(pRecvOverLab->usSize - pRecvOverLab->usOffset);
                    const int recvResult = ReadSocketBytes(head->Socket,
                                                           pRecvOverLab->szBuffer.data() + pRecvOverLab->usOffset,
                                                           recvLength);
                    if (recvResult > 0) {
                        GreenDamTan_PostIOCPEvent(head,
                                                  pRecvOverLab,
                                                  static_cast<std::uint32_t>(recvResult),
                                                  true);
                    } else if (recvResult == 0 || !IsSocketWouldBlock()) {
                        GreenDamTan_PostIOCPEvent(head, pRecvOverLab, 0, false);
                    }
                }
            }

            {
                SimpleLockGuard clientLock(head->xLock);
                if ((head->m_eNetState & XClient::eStateKickOut) != 0) {
                    LogHelper::LogError("game.system",
                                        "<SOCKET> BlockSocket ( %d / %lld )",
                                        15,
                                        static_cast<long long>(head->Socket));
                    if (head->eBlock == XSocket::eBLOCK_OFF) {
                        ForceShutdownSocket(head->Socket);
                        head->eBlock = XSocket::eBLOCK_ON;
                    }
                    LogHelper::LogError("game.system",
                                        "XIOCPSkeleton::FreeXIO :: IS_NULL<XOverLab>(pOverLab) 1 ");
                }

                if (head->eBlock != XSocket::eBLOCK_OFF) {
                    blockedClient = head;
                } else if (head->usInternalHigh != 0) {
                    const char* pBuffer = head->szBuffer.data() + head->usOffset;
                    while (head->usInternal < head->usInternalHigh) {
                        const std::uint16_t packetOffset = head->usInternal;
                        std::uint16_t packetSize = 0;
                        std::memcpy(&packetSize, pBuffer + packetOffset + 2, sizeof(packetSize));
                        if (packetSize < sizeof(PACKET_HEADER) ||
                            packetOffset + packetSize > head->usInternalHigh) {
                            break;
                        }

                        XPacket xPacket;
                        if (!xPacket.GreenDamTan_AssignNetworkPacket(pBuffer + packetOffset, packetSize)) {
                            break;
                        }

                        head->Parse(xPacket);
                        head->usInternal = static_cast<std::uint16_t>(head->usInternal + packetSize);
                    }
                    head->usInternal = 0;
                    head->usInternalHigh = 0;
                }
            }

            head = server ? server->GetClientPool().GetNext() : nullptr;
            if (blockedClient) {
                CloseClient(blockedClient);
                blockedClient = nullptr;
            }
        }

        BackSends();
        OnUpdate(currentTick);
        PrintFPS(currentTick);
    }

    return 0;
}

bool XIOCPServer::XAccept(XClient* pClient) {
    if (!pClient) {
        return false;
    }

    SimpleLockGuard clientLock(pClient->xLock);
    pClient->usOffset = 0;
    if (!OnAccect(pClient)) {
        const char* pszAddr = inet_ntoa(pClient->scAddr.sin_addr);
        LogHelper::LogError("game.system", "]] Error pClient Init !! : [%s]", pszAddr ? pszAddr : "");
        LogHelper::LogError("game.system",
                            "<SOCKET> BlockSocket ( %d / %lld / %d )",
                            7,
                            static_cast<long long>(pClient->Socket),
                            -2);
        if (pClient->eBlock == XSocket::eBLOCK_OFF) {
            ForceShutdownSocket(pClient->Socket);
            pClient->eBlock = XSocket::eBLOCK_ON;
        }
        LogHelper::LogError("game.system", "XIOCPSkeleton::FreeXIO :: IS_NULL<XOverLab>(pOverLab) 1 ");
        return false;
    }

    return true;
}

bool XIOCPServer::XSend(XClient* pClient, XSendPacket* xSendPacket) {
    if (!pClient || !xSendPacket) {
        return false;
    }

    if (pClient->eBlock == XSocket::eBLOCK_ON) {
        return true;
    }

    if (!m_bNetworkInitialized) {
        return false;
    }

    if (m_nLimitIOPool > 0 && pClient->m_nSendCount > m_nLimitIOPool) {
        pClient->SetState(XClient::eStateKickOut);
        LogHelper::LogError("game.system",
                            "<SendPoolError> pClient->m_nSendCount > %d ",
                            m_nLimitIOPool);
        return false;
    }

    XOverLab* pOverLab = m_xIOPool.AllocIO(pClient->Socket, XOverLab::eOVERLAB_TYPE_WRITE);
    if (!pOverLab) {
        LogHelper::LogLegacy("]] XIOCPServer::XSend : m_xIOPool **Empty** ");
        LogHelper::LogError("game.contents",
                            "<POOL> Pool is Full ( %d , %d )",
                            m_xIOPool.m_nReadCount,
                            m_xIOPool.m_nWriteCount);
        return false;
    }

    std::uint16_t usOutSize = 0;
    xSendPacket->usSize = static_cast<std::uint16_t>(xSendPacket->XParse.m_usIndex + sizeof(PACKET_HEADER));
    if (!xSendPacket->Encrypt(pOverLab->szBuffer.data(), usOutSize)) {
        m_xIOPool.FreeIO(pOverLab);
        return false;
    }

    pOverLab->usOffset = usOutSize;
    ++pClient->m_nSendCount;
    pClient->m_dwTick = GetCurrentTickMs();
    return this->XTCPSkeleton::XSend(pOverLab);
}

void XIOCPServer::CloseClient(XClient* pClient) {

    if (!pClient) {
        return;
    }

    if (pClient->m_nJobCount > 0) {
        LogHelper::LogDebug("game.system",
                            "[CloseClient] Delaya For Job ( %lld / %d ) ",
                            static_cast<long long>(pClient->Socket),
                            pClient->m_nJobCount);
        return;
    }

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GreenDamTan_XServerRuntime.cpp::XIOCPServer::CloseClient session=%d socket=%lld state=0x%X finish=%d logout=%d ptr=%p",
                        pClient->GetSessionID(),
                        static_cast<long long>(pClient->Socket),
                        static_cast<unsigned int>(pClient->m_eNetState),
                        pClient->IsState(XClient::eStateFinish) ? 1 : 0,
                        pClient->IsState(XClient::eStateLogOut) ? 1 : 0,
                        static_cast<void*>(pClient));

    LogHelper::LogDebug("game.system",
                        "GreenDamTan_log GreenDamTan_XServerRuntime.cpp::XIOCPServer::CloseClient session=%d socket=%lld state=0x%X finish=%d logout=%d ptr=%p",
                        pClient->GetSessionID(),
                        static_cast<long long>(pClient->Socket),
                        static_cast<unsigned int>(pClient->m_eNetState),
                        pClient->IsState(XClient::eStateFinish) ? 1 : 0,
                        pClient->IsState(XClient::eStateLogOut) ? 1 : 0,
                        static_cast<void*>(pClient));

    if (pClient->IsState(XClient::eStateFinish)) {
        LogHelper::LogDebug("game.system",
                            "[CloseClient] FreeCleint ( %lld / %p ) ",
                            static_cast<long long>(pClient->Socket),
                            static_cast<void*>(pClient));
        pClient->Init();
        if (XServer* server = dynamic_cast<XServer*>(this)) {
            server->GetClientPool().FreeClient(pClient);
        }
        return;
    }

    if (!pClient->IsState(XClient::eStateLogOut) && OnLogOut(pClient)) {
        LogHelper::LogDebug("game.system",
                            "[CloseClient] OnLogout ( %lld ) ",
                            static_cast<long long>(pClient->Socket));
        pClient->SetState(XClient::eStateLogOut);
    }
}

bool XIOCPServer::OnLogOut(XClient* pClient) {
    return pClient && pClient->OnLogOut();
}

bool XIOCPServer::Shutdown(std::uint32_t /*maxWait*/) {
    m_bStopBackendThread = true;
    m_bNetworkInitialized = false;
    m_eState = 2;
    if (m_scListen >= 0) {
        ForceCloseSocket(m_scListen);
        m_scListen = -1;
    }

    if (m_hIOCPAcceptThread.joinable()) {
        if (m_hIOCPAcceptThread.get_id() == std::this_thread::get_id()) {
            m_hIOCPAcceptThread.detach();
        } else {
            m_hIOCPAcceptThread.join();
        }
    }

    if (m_hIOCPBackEndThread.joinable()) {
        if (m_hIOCPBackEndThread.get_id() == std::this_thread::get_id()) {
            m_hIOCPBackEndThread.detach();
        } else {
            m_hIOCPBackEndThread.join();
        }
    }

    return XIOCPSkeleton::Shutdown(0xFFFFFFFFu);
}

void XIOCPServer::OnUpdate(std::uint64_t /*currentTick*/) {
}

void XIOCPServer::PrintFPS(std::uint64_t currentTick) {
    if (m_dw64FPSTick_0 == 0) {
        m_dw64FPSTick_0 = GetCurrentTickMs();
    }

    if (currentTick - m_dw64FPSTick_0 >= 1000) {
        ++m_nFPSCount;
        m_dw64FPSTick_0 = currentTick;
        m_dwFrame_0 = 0;
    } else {
        ++m_dwFrame_0;
    }

    const std::uint64_t elapsed = GetCurrentTickMs() - currentTick;
    if (elapsed < 15) {
        std::this_thread::sleep_for(std::chrono::milliseconds(15 - elapsed));
    }
}

bool XServer::OnAccect(XClient* pClient) {
    if (!pClient) {
        LogHelper::LogError("game.system", "]] if( NULL==pClient ) ");
        return false;
    }

    const char* pszAddr = inet_ntoa(pClient->scAddr.sin_addr);
    LogHelper::LogDebug("game.system",
                        "<SOCKET> Accept Socket ( %lld / %s )",
                        static_cast<long long>(pClient->Socket),
                        pszAddr ? pszAddr : "");

    if (!pClient->Init(this)) {
        LogHelper::LogError("game.system", "]] if( false == pClient->Init(this) ) ");
        return false;
    }

    pClient->SetState(XClient::eStateConnect);
    return true;
}

bool XServer::LoadConfig() {
    return m_xOption.Load(m_szName);
}

bool XServer::Clear() {  // 对齐 IDA: 无参数
    m_bClose = true;
    return true;
}

bool XServer::Init() {
    SetName();
    if (!LoadConfig()) {
        LogHelper::LogError("game.system", "Error Config.XML load fail");
        return false;
    }

    m_nMaxUserCount = GetOption().GetMaxConnect();
    if (m_pIObjectMgr &&
        !m_pIObjectMgr->Init(static_cast<int>(static_cast<double>(m_nMaxUserCount) * 1.2))) {
        LogHelper::LogError("game.system", "Error m_xObjectMgr init fail");
        return false;
    }

    if (!InitServer()) {
        LogHelper::LogError("game.system", "Init Server fail");
        return false;
    }

    if (!XIOCPServer::Init(
            ePoolIDServer, GetOption().GetIP(), GetOption().GetPort(), GetOption().GetMaxIOPool())) {
        LogHelper::LogError("game.system", "Error Network Init fail");
        return false;
    }

    LogHelper::LogInfo("game.contents", "[ %s ] server Start!", m_szName);
    return true;
}

bool XServer::Run() {
    if (!SetConsoleHandler(1)) {
        return true;
    }

    if (Init()) {
        m_bRunFlag = true;
        while (m_bRunFlag) {
            const std::uint64_t tickBegin = GetCurrentTickMs();
            if (m_bClose) {
                m_bRunFlag = false;
            }

            const std::uint64_t elapsed = GetCurrentTickMs() - tickBegin;
            if (elapsed < 100) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(static_cast<int>(100 - elapsed)));
            }
        }
    }

    SetConsoleHandler(0);
    return true;
}

bool XServer::OnLogOut(XClient* pClient) {
    return pClient && pClient->OnLogOut();
}

bool XServer::Shutdown(std::uint32_t maxWait) {
    if (!Clear()) {  // 对齐 IDA: Clear() 无参数
        return false;
    }
    return XIOCPServer::Shutdown(0xFFFFFFFFu);
}

void XServer::OnUpdate(std::uint64_t currentTick) {
    std::vector<XClient*> blockedClients;

    for (XClient* client = m_xClientPool.GetHead(); client; client = m_xClientPool.GetNext()) {
        client->OnUpdate(currentTick);

        if (client->IsState(XClient::eStateKickOut) && client->eBlock == XSocket::eBLOCK_OFF) {
            // TODO: 推测结果: 原版 `BackendThreadProc` 会先 `shutdown` 再置 `eBlock`，
            // 当前仅保留最小断线触发骨架，避免让踢线状态长期停留在“已置位、未收尾”。
            ForceShutdownSocket(client->Socket);
            client->eBlock = XSocket::eBLOCK_ON;
        }

        if (client->eBlock != XSocket::eBLOCK_OFF) {
            blockedClients.push_back(client);
        }
    }

    for (XClient* client : blockedClients) {
        CloseClient(client);
    }
}

SYSTEM_TYPE XOption::GetSystemType(const char* szType) {
    if (!szType) {
        return SYSTEM_TYPE_NONE;
    }
    if (std::strcmp(szType, "DEV") == 0) {
        return SYSTEM_TYPE_DEV;
    }
    if (std::strcmp(szType, "REAL") == 0) {
        return SYSTEM_TYPE_REAL;
    }
    return std::strcmp(szType, "TEST") == 0 ? SYSTEM_TYPE_TEST : SYSTEM_TYPE_NONE;
}

NATION_TYPE XOption::GetNationType(const char* szType) {
    if (!szType) {
        return NATION_TYPE_NONE;
    }
    if (std::strcmp(szType, "KOR") == 0) {
        return NATION_TYPE_KOR;
    }
    if (std::strcmp(szType, "JPN") == 0) {
        return NATION_TYPE_JPN;
    }
    if (std::strcmp(szType, "CHN") == 0) {
        return NATION_TYPE_CHN;
    }
    return std::strcmp(szType, "TWN") == 0 ? NATION_TYPE_TWN : NATION_TYPE_KOR;
}

int XOption::GetServerType(const char* szType) {
    if (!szType) {
        return 0;
    }
    if (std::strcmp(szType, "MAZE") == 0) {
        return 2;
    }
    if (std::strcmp(szType, "CONTROL") == 0) {
        return 6;
    }
    if (std::strcmp(szType, "LOG") == 0) {
        return 4;
    }
    if (std::strcmp(szType, "DBAGENT") == 0) {
        return 5;
    }
    if (std::strcmp(szType, "RELAY") == 0 || std::strcmp(szType, "COMMUNITY") == 0) {
        return 3;
    }
    if (std::strcmp(szType, "GMTOOLAGENT") == 0) {
        return 7;
    }
    return std::strcmp(szType, "OBSERVEAGENT") == 0 ? 8 : 0;
}

void XOption::AddChannelInfo(const CHANNEL_INFO& stInfo) {
    m_mapChannelInfo[stInfo.nID].push_back(stInfo);
}

void XOption::AddChannelDistrict6Info(const CHANNEL_INFO& stInfo) {
    m_mapChannelDistrict6Info[stInfo.nID].push_back(stInfo);
}

bool XOption::Load(const char* szName) {
    // TODO: 推测结果：原版使用 TinyXML 逐层取节点；当前跨平台工程先用文本/正则解析
    // 对齐已在 IDA 中核实的节点顺序与属性名，后续仍需继续按原版 XML 访问细节收紧。
    *this = XOption{};
    SetName(szName);

    const std::filesystem::path configPath = FindConfigPath();
    if (configPath.empty()) {
        return false;
    }

    const std::string xml = StripXmlComments(ReadTextFile(configPath));
    if (xml.empty()) {
        return false;
    }

    XmlBlock configBlock;
    if (!FindFirstBlock(xml, "CONFIG", &configBlock)) {
        return false;
    }

    std::string infoAttributes;
    if (FindElementAttributes(configBlock.body, "INFO", &infoAttributes)) {
        m_eSystemType = GetSystemType(GetAttributeValue(infoAttributes, "TYPE").c_str());
        m_eNationType = GetNationType(GetAttributeValue(infoAttributes, "NATION").c_str());
        const std::string billingType = GetAttributeValue(infoAttributes, "BILLING");
        m_eBillingType = billingType == "TEST" ? BILLING_TYPE_TEST : BILLING_TYPE_REAL;
        m_bDev = m_eSystemType == SYSTEM_TYPE_DEV;
        m_bTestServer = m_eSystemType == SYSTEM_TYPE_TEST;
        m_bRealServer = m_eSystemType == SYSTEM_TYPE_REAL;
    }

    std::string mobileAttributes;
    if (FindElementAttributes(configBlock.body, "MOBILE", &mobileAttributes)) {
        CopyCString(m_stMobileInfo.szMobileIP, GetAttributeValue(mobileAttributes, "MOBILE_IP"));
        m_stMobileInfo.nMobilePort = GetIntAttribute(mobileAttributes, "MOBILE_PORT", 81);
    }

    std::string xigncodeAttributes;
    if (FindElementAttributes(configBlock.body, "XIGNCODE", &xigncodeAttributes)) {
        const std::string securityType = GetAttributeValue(xigncodeAttributes, "TYPE");
        if (!securityType.empty()) {
            m_eSecurityType = securityType == "OFF" ? SECURITY_OFF : SECURITY_ON;
        }
    }

    std::string resLoadAttributes;
    if (FindElementAttributes(configBlock.body, "RESLOAD", &resLoadAttributes)) {
        const std::string loadType = GetAttributeValue(resLoadAttributes, "TYPE");
        m_eResLoadType = loadType == "FILE" ? RES_LOAD_TYPE_FILE : RES_LOAD_TYPE_DB;
        CopyCString(m_szResFilePath, GetAttributeValue(resLoadAttributes, "FILE_PATH"));
    }

    XmlBlock optionBlock;
    if (FindFirstBlock(configBlock.body, "OPTION", &optionBlock)) {
        m_ContentsOption.nOptionFlag = GetIntAttribute(optionBlock.attributes, "USE_FLAG", 1);
        const std::array<std::pair<const char*, int>, 11> optionMappings = {{
            {"USE_ATTENDANCE", E_SERVER_OPTION_ATTENDANCE},
            {"USE_SECOND_PW", E_SERVER_OPTION_SECOND_PW},
            {"USE_PVP_DISTRICT", E_SERVER_OPTION_PVP_DISTRICT},
            {"USE_RANKING", E_SERVER_OPTION_RANKING},
            {"USE_CASHSHOP", E_SERVER_OPTION_CASHSHOP},
            {"USE_D6_MODE", E_SERVER_OPTION_D6_MODE},
            {"USE_OVER_INDULGENCE", E_SERVER_OPTION_OVER_INDULGENCE},
            {"USE_WEEKLY_MISSION", E_SERVER_OPTION_SOUL_WEEKLY_MISSION},
            {"USE_NETCAFE", E_SERVER_OPTION_NETCAFE},
            {"USE_ITEM_EXCHANGE", E_SERVER_OPTION_ITEM_EXCHANGE},
            {"USE_WAIT_SYSTEM", E_SERVER_OPTION_WAIT_SYSTEM},
        }};

        for (const auto& [tagName, optionIndex] : optionMappings) {
            std::string attributes;
            if (FindElementAttributes(optionBlock.body, tagName, &attributes)) {
                m_ContentsOption.bContents[optionIndex] = GetBoolAttribute(attributes, "ON");
            }
        }

        std::string operationMazeAttributes;
        if (FindElementAttributes(optionBlock.body, "USE_OPERATION_MAZE", &operationMazeAttributes)) {
            m_ContentsOption.bContents[E_SERVER_OPTION_OPERATION_MAZE] =
                GetBoolAttribute(operationMazeAttributes, "ON");
        }

        // `XOption::Load` 当前没有对应 `USE_BROACH_EVENT` / `USE_SOUL_EVENT` 读取字符串证据。
        m_ContentsOption.bContents[E_SERVER_OPTION_BROACH_EVENT] = false;
        m_ContentsOption.bContents[E_SERVER_OPTION_SOUL_EVENT] = false;
    }

    XmlBlock dbBlock;
    if (FindFirstBlock(configBlock.body, "DB", &dbBlock)) {
        m_nDBThreadCount = GetIntAttribute(dbBlock.attributes, "THREAD_COUNT");

        const struct {
            const char* tagName;
            std::size_t dnsIndex;
            bool appendDefaultPort;
            bool* flag;
        } dbMappings[] = {
            {"AUTH", kDNSIndexAuth, true, nullptr},
            {"GAME", kDNSIndexGame, true, nullptr},
            {"COMMON", kDNSIndexCommon, true, nullptr},
            {"LOG", kDNSIndexLog, true, &m_bWriteLog},
            {"GMTOOL", kDNSIndexGMTool, false, nullptr},
            {"OBSERVE", kDNSIndexObserve, true, nullptr},
            {"STATISTICS", kDNSIndexStatistics, true, &m_bWriteStatistics},
            {"SGNETCAFE", kDNSIndexSGNetCafe, true, &m_bWriteSGNetcafe},
        };

        for (const auto& mapping : dbMappings) {
            std::string attributes;
            if (!FindElementAttributes(dbBlock.body, mapping.tagName, &attributes)) {
                continue;
            }

            m_strDNS[mapping.dnsIndex] = ComposeConnectionString(GetAttributeValue(attributes, "DRIVER"),
                                                                 GetAttributeValue(attributes, "SERVER"),
                                                                 GetAttributeValue(attributes, "DB"),
                                                                 GetAttributeValue(attributes, "UID"),
                                                                 GetAttributeValue(attributes, "PWD"),
                                                                 mapping.appendDefaultPort);
            if (mapping.flag) {
                *mapping.flag = true;
            }
        }
    }

    for (const XmlBlock& serverBlock : FindAllBlocks(configBlock.body, "SERVER")) {
        SERVER_SYSTEM_INFO serverInfo{};
        FillServerCommonInfo(serverInfo, serverBlock);
        FillServerDBAgentInfo(serverInfo, serverBlock.body);

        if (serverInfo.nType == 3) {
            CopyCString(m_szRelayIP, serverInfo.szIP);
            m_sRelayPort = static_cast<std::uint16_t>(serverInfo.nPort);
        }

        m_mapSystemInfo[serverInfo.szName] = serverInfo;
        if (szName && std::strcmp(szName, serverInfo.szName) == 0) {
            m_serverInfo = serverInfo;
            FillObjectPoolInfo(m_stObjectPoolInfo, serverBlock.body);

            XmlBlock channelBlock;
            if (FindFirstBlock(serverBlock.body, "CHANNEL", &channelBlock)) {
                m_serverInfo.nChannelFlag = GetIntAttribute(channelBlock.attributes, "CHANNEL_FLAG", 1);
                for (const XmlBlock& channelInfoBlock : FindAllBlocks(channelBlock.body, "CHANNEL_INFO")) {
                    CHANNEL_INFO channelInfo{};
                    channelInfo.nID = GetIntAttribute(channelInfoBlock.attributes, "DISTRICT");
                    channelInfo.wFrom = static_cast<std::uint16_t>(
                        GetIntAttribute(channelInfoBlock.attributes, "FROM"));
                    channelInfo.wTo = static_cast<std::uint16_t>(
                        GetIntAttribute(channelInfoBlock.attributes, "TO"));
                    AddChannelInfo(channelInfo);
                }
                for (const XmlBlock& district6Block : FindAllBlocks(channelBlock.body, "DISTRICT6_INFO")) {
                    CHANNEL_INFO channelInfo{};
                    channelInfo.nID = GetIntAttribute(district6Block.attributes, "MAZE");
                    channelInfo.wFrom = static_cast<std::uint16_t>(
                        GetIntAttribute(district6Block.attributes, "FROM"));
                    channelInfo.wTo = static_cast<std::uint16_t>(
                        GetIntAttribute(district6Block.attributes, "TO"));
                    AddChannelDistrict6Info(channelInfo);
                }
            }
        }
    }

    for (const XmlBlock& agentBlock : FindAllBlocks(configBlock.body, "AGENT")) {
        AGENT_SYSTEM_INFO agentInfo{};
        FillServerCommonInfo(agentInfo.stServerInfo, agentBlock);
        agentInfo.nDBThreadCount = m_nDBThreadCount;

        const std::string typeName = agentInfo.stServerInfo.szName;
        int slot = 3;
        if (typeName == "GMTOOLAGENT") {
            slot = 0;
            agentInfo.strDNS = m_strDNS[kDNSIndexGMTool];
        } else if (typeName == "OBSERVEAGENT") {
            slot = 1;
            agentInfo.strDNS = m_strDNS[kDNSIndexObserve];
        } else if (typeName == "DUMPAGENT") {
            slot = 2;
            agentInfo.strDNS = m_strDNS[kDNSIndexObserve];
        }

        if (slot < static_cast<int>(m_agentInfo.size())) {
            m_agentInfo[static_cast<std::size_t>(slot)] = agentInfo;
        }

        if (szName && std::strcmp(szName, agentInfo.stServerInfo.szName) == 0) {
            m_serverInfo = agentInfo.stServerInfo;
        }
    }

    return true;
}

void XOption::ShowServerInfo() {
    const NetworkParamsSnapshot networkParams = QueryNetworkParams();
#ifdef _WIN32
    if (!networkParams.valid) {
        return;
    }
#endif

    LogHelper::LogInfo("game.system", "============[%s] Information=================", GetName());
    LogHelper::LogInfo(
        "game.system", "   Server Group ID\t: %d - %d", m_serverInfo.nGroup, m_serverInfo.nChannel);
    LogHelper::LogInfo("game.system", "   Server IP\t\t: %s ", m_serverInfo.szIP);
    LogHelper::LogInfo("game.system", "   Server Real IP\t: %s ", m_serverInfo.szPublicIP);
    LogHelper::LogInfo("game.system", "   Server Port\t\t: %d ", m_serverInfo.nPort);
    LogHelper::LogInfo("game.system", "   Max Connection\t: %d ", m_serverInfo.nMaxUser);
    LogHelper::LogInfo("game.system", "   Host Name\t\t: %s ", networkParams.hostName.c_str());
    LogHelper::LogInfo("game.system", "   DHCP Scope name\t: %s ", networkParams.scopeName.c_str());
    LogHelper::LogInfo("game.system", "   Node Type\t\t: %s ", networkParams.nodeTypeText.c_str());
    LogHelper::LogInfo("game.system",
                        "   IP Routing Enabled...: %s ",
                        networkParams.routingEnabled ? "YES" : "NO");
    LogHelper::LogInfo("game.system",
                        "   WINS Proxy Enabled...: %s ",
                        networkParams.winsProxyEnabled ? "YES" : "NO");
    LogHelper::LogInfo("game.system", "   SERVER_NATION : %d ", m_eNationType);
    LogHelper::LogInfo("game.system", "   SYSTEM_TYPE   : %d ", m_eSystemType);
    LogHelper::LogInfo("game.system", "   XIGNCODE      : %d ", m_eSecurityType);

    for (const ST_SERVER_INFO& info : m_serverInfo.stDBAgentInfo) {
        if (!info.nPort) {
            continue;
        }
        LogHelper::LogInfo("game.system", "   [%s]DBAgent IP\t: %s ", info.szName, info.szIP);
        LogHelper::LogInfo("game.system", "   [%s]DBAgent PORT\t: %d ", info.szName, info.nPort);
    }

    LogHelper::LogInfo(
        "game.system", "   INIT XIGNCODE : %s ", m_bInitXignCode ? "ON" : "OFF");
    if (m_eNationType == NATION_TYPE_JPN) {
        LogHelper::LogInfo(
            "game.system", "   INIT HAN_BILLING : %s ", m_bInitHanBilling ? "ON" : "OFF");
        LogHelper::LogInfo(
            "game.system", "   INIT HAN_NETCAFE : %s ", m_bInitHanNetCafe ? "ON" : "OFF");
    }
    LogHelper::LogInfo("game.system", "==============================================");
}

OBJECT_POOL_INFO& XOption::GetPoolInfo() {
    return m_stObjectPoolInfo;
}

const OBJECT_POOL_INFO& XOption::GetPoolInfo() const {
    return m_stObjectPoolInfo;
}

bool XOption::GetDBAgentInfo(std::uint16_t type,
                             char* ip,
                             std::int16_t* port,
                             char* name,
                             int* maxIOPoolCount,
                             int* dbAgentCount) const {
    if (type >= m_serverInfo.stDBAgentInfo.size()) {
        return false;
    }

    const ST_SERVER_INFO& info = m_serverInfo.stDBAgentInfo[type];
    if (!info.nType) {
        return false;
    }

    if (ip) {
#ifdef _WIN32
        strcpy_s(ip, 0x201u, info.szIP);
#else
        std::strncpy(ip, info.szIP, 0x200);
        ip[0x200] = '\0';
#endif
    }
    if (port) {
        *port = static_cast<std::int16_t>(info.nPort);
    }
    if (name) {
#ifdef _WIN32
        strcpy_s(name, 21u, info.szName);
#else
        std::strncpy(name, info.szName, 20);
        name[20] = '\0';
#endif
    }
    if (maxIOPoolCount) {
        *maxIOPoolCount = info.nMaxIOPool;
    }
    if (dbAgentCount) {
        *dbAgentCount = info.nDBAgentCount > 0 ? info.nDBAgentCount : 1;
    }
    return true;
}

bool XOption::GetServerPrivateIPAndPort(char* szServerName, char* szIP, std::uint16_t& sPort) const {
    if (!szServerName) {
        return false;
    }

    const auto it = m_mapSystemInfo.find(szServerName);
    if (it == m_mapSystemInfo.end()) {
        return false;
    }

    if (szIP) {
#ifdef _WIN32
        strcpy_s(szIP, 0x201u, it->second.szIP);
#else
        std::strncpy(szIP, it->second.szIP, 0x200);
        szIP[0x200] = '\0';
#endif
    }
    sPort = static_cast<std::uint16_t>(it->second.nPort);
    return true;
}

bool XOption::GetAgentPrivateIPAndPort(int nType, char* szIP, std::uint16_t& sPort) const {
    if (nType < 0 || nType >= static_cast<int>(m_agentInfo.size())) {
        return false;
    }

    const AGENT_SYSTEM_INFO& info = m_agentInfo[static_cast<std::size_t>(nType)];
    if (szIP) {
#ifdef _WIN32
        strcpy_s(szIP, 0x201u, info.stServerInfo.szIP);
#else
        std::strncpy(szIP, info.stServerInfo.szIP, 0x200);
        szIP[0x200] = '\0';
#endif
    }
    sPort = static_cast<std::uint16_t>(info.stServerInfo.nPort);
    return true;
}

bool XOption::GreenDamTan_GetServerSystemInfo(const char* szServerName,
                                              SERVER_SYSTEM_INFO* outInfo) const {
    if (!szServerName || !outInfo) {
        return false;
    }

    const auto it = m_mapSystemInfo.find(szServerName);
    if (it == m_mapSystemInfo.end()) {
        return false;
    }

    *outInfo = it->second;
    return true;
}
