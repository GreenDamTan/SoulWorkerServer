#pragma once

#include <array>
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

#include "Soulworker/GameServer/XCore/XServer/IXObject.h"
#include "Soulworker/GameServer/XCore/XServer/Option.h"

class XClient;
class XSocket;
class XSendPacket;

enum E_POOL_ID : std::int32_t {
    ePoolIDNone = 0,
    ePoolIDServer = 1,
    ePoolIDGameSc = 2,
    ePoolIDAccountSc = 3,
    ePoolIDRelayServer = 4,
};

/**
 * @brief `XOverLab` 的最小跨平台还原。
 *
 * 本轮只保留 `XRecv / AcceptThread` 已经证实会访问的字段：
 * - `Socket`
 * - `m_xLock`
 * - `szBuffer`
 * - `usSize`
 * - `usOffset`
 * - `eType`
 */
struct XOverLab {
    enum E_OVERLAB_TYPE : std::uint32_t {
        eOVERLAB_TYPE_NONE = 0,
        eOVERLAB_TYPE_READ = 1,
        eOVERLAB_TYPE_WRITE = 2,
        eOVERLAB_TYPE_CONNECT = 3,
    };

    void Init(std::intptr_t socketHandle, E_OVERLAB_TYPE overLabType) {
        Socket = socketHandle;
        m_xLock.Init();
        usSize = static_cast<std::uint16_t>(szBuffer.size());
        usOffset = 0;
        eType = overLabType;
    }

    void Destroy() {
        m_xLock.Destroy();
    }

    std::intptr_t Socket = -1;
    CSimpleLock m_xLock;
    std::array<char, 0xFFFF> szBuffer{};
    std::uint16_t usSize = 0xFFFFu;
    std::uint16_t usOffset = 0;
    std::uint32_t eType = eOVERLAB_TYPE_NONE;
};

class XIOPool {
public:
    XIOPool() {
        m_xLock.Init();
    }

    void Reset() {
        CSimpleLock::Owner lock(&m_xLock);
        m_freeList.clear();
        m_storage.clear();
        m_nIOCount = 0;
        m_ePoolID = ePoolIDNone;
        m_nReadCount = 0;
        m_nWriteCount = 0;
    }

    XOverLab* AllocIO(std::intptr_t socketHandle, XOverLab::E_OVERLAB_TYPE overLabType) {
        CSimpleLock::Owner lock(&m_xLock);
        XOverLab* overLab = nullptr;
        if (!m_freeList.empty()) {
            overLab = m_freeList.back();
            m_freeList.pop_back();
        } else {
            auto newOverLab = std::make_unique<XOverLab>();
            overLab = newOverLab.get();
            m_storage.push_back(std::move(newOverLab));
        }

        if (overLab) {
            overLab->Init(socketHandle, overLabType);
            if (overLabType == XOverLab::eOVERLAB_TYPE_READ) {
                ++m_nReadCount;
            } else if (overLabType == XOverLab::eOVERLAB_TYPE_WRITE) {
                ++m_nWriteCount;
            }
        }
        return overLab;
    }

    void FreeIO(XOverLab* overLab) {
        if (!overLab) {
            return;
        }

        CSimpleLock::Owner lock(&m_xLock);
        if (overLab->eType == XOverLab::eOVERLAB_TYPE_READ && m_nReadCount > 0) {
            --m_nReadCount;
        } else if (overLab->eType == XOverLab::eOVERLAB_TYPE_WRITE && m_nWriteCount > 0) {
            --m_nWriteCount;
        }
        overLab->Init(-1, XOverLab::eOVERLAB_TYPE_NONE);
        m_freeList.push_back(overLab);
    }

    bool Clear(std::uint32_t /*maxWait*/) {
        Reset();
        return true;
    }

    CSimpleLock m_xLock;
    int m_nIOCount = 0;
    E_POOL_ID m_ePoolID = ePoolIDNone;
    int m_nReadCount = 0;
    int m_nWriteCount = 0;

private:
    std::vector<std::unique_ptr<XOverLab>> m_storage;
    std::deque<XOverLab*> m_freeList;
};

/**
 * @brief `XIOCPSkeleton` 的跨平台最小还原。
 *
 * 本轮继续沿 `XTCPSkeleton::XSend / OnSend / OnRecv / BlockSocket` 下钻，
 * 把原版 `GetQueuedCompletionStatus / PostQueuedCompletionStatus` 的完成事件方向
 * 收口成一层跨平台 synthetic IOCP 队列。
 *
 * // TODO: 需人工审查：当前仍未直接恢复 WinSock/IOCP 原生句柄与 `_OVERLAPPED`
 * 系统调用，只保留原版完成事件类型与线程收口方向。
 */
class XIOCPSkeleton {
public:
    XIOCPSkeleton();
    virtual ~XIOCPSkeleton();

    bool CreateIOCP(const char* szAddr, std::uint16_t usPort, E_POOL_ID ePoolID, int nIOCount);
    bool BlockSocket(XSocket* pSocket, XOverLab* pOverLab, std::uint8_t ucCloseNo);
    virtual bool Shutdown(std::uint32_t maxWait);
    std::uint32_t WorkerThread();

protected:
    enum class GreenDamTan_IOCP_EVENT_TYPE : std::uint8_t {
        eCompletion = 0,
        eClose = 1,
        eExit = 2,
    };

    struct GreenDamTan_IOCP_EVENT {
        GreenDamTan_IOCP_EVENT_TYPE eventType = GreenDamTan_IOCP_EVENT_TYPE::eCompletion;
        void* completionKey = nullptr;
        XOverLab* pOverLab = nullptr;
        std::uint32_t dwNumberOfBytesTransferred = 0;
        bool bSuccess = true;
    };

    void GreenDamTan_PostIOCPEvent(void* completionKey,
                                   XOverLab* pOverLab,
                                   std::uint32_t dwNumberOfBytesTransferred,
                                   bool bSuccess);
    void GreenDamTan_PostCloseEvent();
    void GreenDamTan_PostExitEvents();
    void GreenDamTan_RegisterCompletionKey(std::intptr_t socketHandle, void* completionKey);
    void GreenDamTan_UnregisterCompletionKey(std::intptr_t socketHandle);
    void* GreenDamTan_FindCompletionKey(std::intptr_t socketHandle);
    void GreenDamTan_ClearCompletionKeys();

    virtual void OnSend(XSocket* pSocket, XOverLab* pOverLab);
    virtual bool OnRecv(XSocket* pSocket, XOverLab* pOverLab, std::uint32_t dwNumberOfBytesTransferred);
    virtual void OnEtcEvent(XOverLab* pOverLab);

    void* m_hIOCP = reinterpret_cast<void*>(-1);
    XIOPool m_xIOPool{};
    int m_nIOCPWorkThread = 0;
    void** m_phIOCPWorkThread = nullptr;
    sockaddr_in m_scAddr{};
    std::atomic<std::uint64_t> GreenDamTan_hIOCPSeed{0};
    std::deque<GreenDamTan_IOCP_EVENT> GreenDamTan_IOCPEvents;
    std::mutex GreenDamTan_IOCPEventLock;
    std::condition_variable GreenDamTan_IOCPEventCv;
    std::vector<std::thread> GreenDamTan_IOCPThreads;
    std::unordered_map<std::intptr_t, void*> GreenDamTan_CompletionKeys;
    std::mutex GreenDamTan_CompletionKeyLock;
};

/**
 * @brief `XTCPSkeleton` 的跨平台最小还原。
 *
 * 本轮已把：
 * - `XSend`
 * - `XRecv`
 * - `OnSend`
 * - `OnRecv`
 * - `OnEtcEvent`
 *
 * 的主方向接回工程，并把 `XSend/OnSend` 收口到 `XIOCPSkeleton::WorkerThread`
 * 的 synthetic 完成事件队列。
 */
class XTCPSkeleton : public XIOCPSkeleton {
public:
    virtual ~XTCPSkeleton() = default;

    virtual bool XSend(XOverLab* pOverLab);
    virtual bool XRecv(XOverLab* pOverLab);
    void OnSend(XSocket* pSocket, XOverLab* pOverLab) override;
    bool OnRecv(XSocket* pSocket, XOverLab* pOverLab, std::uint32_t dwNumberOfBytesTransferred) override;
    void OnEtcEvent(XOverLab* pOverLab) override;

protected:
    int m_nLimitIOPool = 0;
};

/**
 * @brief `TXServer<CUser>` 的最小会话对象桥接层。
 *
 * 根据 IDA：
 * - `TXServer<CUser>::FindUser @ 0x140001480`
 * - `TXServer<CUser>::XCreator<CUser>::Create @ 0x14001b362`
 *
 * 当前只恢复登录服已经明确依赖的两条语义：
 * 1. `FindUser -> TXObjectMgr<CUser>::Find`
 * 2. `XCreator::Create -> TXObjectMgr<CUser>::Create`
 */
template <typename TUser>
class TXServer {
public:
    class XCreator {
    public:
        XClient* Create() {
            return m_pObjectMgr ? m_pObjectMgr->Create() : nullptr;
        }

        TXObjectMgr<TUser>* m_pObjectMgr = nullptr;
    };

    TUser* FindUser(int xSessionID) {
        return m_pObjectMgr ? m_pObjectMgr->Find(xSessionID) : nullptr;
    }

protected:
    void BindObjectMgr(TXObjectMgr<TUser>* pObjectMgr) {
        m_pObjectMgr = pObjectMgr;
        m_xCreator.m_pObjectMgr = pObjectMgr;
    }

    TXObjectMgr<TUser>* m_pObjectMgr = nullptr;
    XCreator m_xCreator{};
};

class XClientPool {
public:
    using ClientCreator = std::function<XClient*()>;
    using ClientRecycler = std::function<void(XClient*)>;

    ~XClientPool();

    XClient* GetHead();
    XClient* GetNext();
    XClient* AllocClient(std::intptr_t sc);
    void FreeClient(XClient* client);
    void SetCreator(ClientCreator creator) { m_creator = std::move(creator); }
    void SetRecycler(ClientRecycler recycler) { m_recycler = std::move(recycler); }

    // TODO: inferred - the original `AllocClient/FreeClient -> TXPool<XClient>`
    // chain is still not restored, so the current code only keeps link/unlink helpers.
    void GreenDamTan_LinkClient(XClient* client);
    void GreenDamTan_UnlinkClient(XClient* client);
    void GreenDamTan_Clear();

private:
    std::list<XClient*> m_xList;
    std::vector<XClient*> m_xFreeList;
    std::vector<XClient*> m_xOwnedList;
    std::list<XClient*>::iterator m_itList = m_xList.end();
    std::mutex m_xLock;
    ClientCreator m_creator;
    ClientRecycler m_recycler;
};

/**
 * @brief `XIOCPServer` 的跨平台最小还原。
 *
 * 当前仍未恢复原版 IOCP / epoll 线程池；
 * 这里只保留：
 * - 日志开关
 * - 监听地址与端口
 * - 后台更新线程骨架
 *
 * // TODO: 需人工审查：若后续继续补网络层，仍需回到 IDA 继续还原
 * `AcceptThread / BackEndThread / IOCP` 的真实成员布局。
 */
class XIOCPServer : public XTCPSkeleton {
public:
    virtual ~XIOCPServer();

    void StartLog();
    bool SetSocket();
    bool Init(E_POOL_ID ePoolID, const char* ip, std::uint16_t port, int maxIOPool);
    std::uint32_t AcceptThread();
    virtual bool BackSends();
    std::uint32_t BackendThreadProc();
    virtual void PrintFPS(std::uint64_t currentTick);
    bool XAccept(XClient* pClient);
    virtual bool XSend(XClient* pClient, XSendPacket* xSendPacket);
    void CloseClient(XClient* pClient);
    virtual bool OnAccect(XClient* pClient);
    virtual bool OnLogOut(XClient* pClient);
    virtual bool Shutdown(std::uint32_t maxWait);
    virtual void OnUpdate(std::uint64_t currentTick);

    bool IsLogEnabled() const { return m_bLog; }
    bool IsNetworkInitialized() const { return m_bNetworkInitialized; }
    const char* GetListenIP() const { return m_szListenIP; }
    std::uint16_t GetListenPort() const { return m_wListenPort; }
    E_POOL_ID GetPoolID() const { return m_ePoolID; }

protected:
    bool m_bLog = false;
    std::atomic<bool> m_bNetworkInitialized{false};
    char m_szListenIP[64] = {};
    std::uint16_t m_wListenPort = 0;
    int m_nMaxIOPool = 0;
    E_POOL_ID m_ePoolID = ePoolIDNone;
    std::intptr_t m_scListen = -1;
    std::atomic<int> m_eState{0};
    std::atomic<bool> m_bStopBackendThread{false};
    std::thread m_hIOCPAcceptThread;
    std::thread m_hIOCPBackEndThread;
    std::uint64_t m_dw64FPSTick_0 = 0;
    std::uint32_t m_dwFrame_0 = 0;
    std::uint32_t m_nFPSCount = 0;
};

/**
 * @brief 登录服公共服务基类的最小还原。
 *
 * 本轮继续把启动链与客户端池遍历骨架往原版方向纠偏：
 * - `Init`
 * - `Run`
 * - `LoadConfig`
 * - `Shutdown`
 * - `OnUpdate`
 */
class XServer : public XIOCPServer {
public:
    virtual ~XServer() = default;

    XOption& GetOption() { return m_xOption; }
    const XOption& GetOption() const { return m_xOption; }
    unsigned int GetServerID() const { return m_xOption.GetServerID(); }
    void SetServerID(unsigned int serverId) { m_xOption.SetServerID(serverId); }
    const char* GetName() const { return m_szName; }
    XClientPool& GetClientPool() { return m_xClientPool; }
    const XClientPool& GetClientPool() const { return m_xClientPool; }

    bool Init();
    bool Run();
    bool OnLogOut(XClient* pClient) override;
    virtual bool Shutdown(std::uint32_t maxWait) override;
    virtual void OnUpdate(std::uint64_t currentTick) override;

protected:
    bool OnAccect(XClient* pClient) override;
    virtual void SetName() {}
    virtual bool LoadConfig();
    virtual bool InitServer() { return true; }
    virtual bool Clear(std::uint32_t maxWait);
    virtual int SetConsoleHandler(int add) { return add; }

    XOption m_xOption;
    IXObjectMgr m_xObjectMgr;
    IXObjectMgr* m_pIObjectMgr = &m_xObjectMgr;
    int m_nMaxUserCount = 0;
    char m_szName[21] = {};
    bool m_bRunFlag = false;
    bool m_bClose = false;
    XClientPool m_xClientPool;
};
