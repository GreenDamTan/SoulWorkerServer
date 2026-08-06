#pragma once

#include <algorithm>
#include <array>
#include <cstring>
#include <ctime>
#include <new>
#include <queue>
#include <shared_mutex>
#include <type_traits>

#include "Soulworker/Common/XNet/XIOCPBase/Packet.h"
#include "Soulworker/GameServer/XCore/XIOCPServer/TXProcess.h"
#include "Soulworker/GameServer/XCore/XServer/IXObject.h"
#include "GreenDamTan_TxMap.h"
#include "Soulworker/GameServer/XCore/XServer/XServer.h"

class XClient;
class cIoContextPool;
struct IoContextBuffer;

namespace ATL {
// 对齐 IDA: CTimeSpan 定义在 CTime 之前，因为 CTime::operator+ 引用 CTimeSpan
class CTimeSpan {
public:
    CTimeSpan() = default;
    // 对齐 IDA: CTimeSpan(long days, int hours, int mins, int secs) 原始构造函数
    CTimeSpan(long days, int hours, int mins, int secs)
        : m_timeSpan(static_cast<std::int64_t>(days) * 24 * 60 * 60 +
                     static_cast<std::int64_t>(hours) * 60 * 60 +
                     static_cast<std::int64_t>(mins) * 60 +
                     static_cast<std::int64_t>(secs)) {}
    // 对齐 IDA: CTimeSpan 从秒数隐式构造 (0x15180 = 86400秒 = 1天)
    CTimeSpan(std::int64_t secs) : m_timeSpan(secs) {}

    std::int64_t GetTotalSeconds() const {
        return m_timeSpan;
    }

    // 对齐 IDA: CTimeSpan 运算符
    CTimeSpan operator+(const CTimeSpan& other) const {
        return CTimeSpan(m_timeSpan + other.m_timeSpan);
    }

    CTimeSpan operator-(const CTimeSpan& other) const {
        return CTimeSpan(m_timeSpan - other.m_timeSpan);
    }

    bool operator<=(const CTimeSpan& other) const {
        return m_timeSpan <= other.m_timeSpan;
    }

    bool operator<(const CTimeSpan& other) const {
        return m_timeSpan < other.m_timeSpan;
    }

    bool operator>=(const CTimeSpan& other) const {
        return m_timeSpan >= other.m_timeSpan;
    }

    bool operator>(const CTimeSpan& other) const {
        return m_timeSpan > other.m_timeSpan;
    }

    // Additional methods for IDA compatibility
    int GetTotalHours() const {
        return static_cast<int>(m_timeSpan / 3600);
    }

    int GetHours() const {
        return static_cast<int>((m_timeSpan / 3600) % 24);
    }

    int GetMinutes() const {
        return static_cast<int>((m_timeSpan / 60) % 60);
    }

    int GetSeconds() const {
        return static_cast<int>(m_timeSpan % 60);
    }

    std::int64_t m_timeSpan = 0;
};

class CTime {
public:
    CTime() = default;
    explicit CTime(std::int64_t timeValue) : m_time(timeValue) {}

    // 对齐 IDA: CTime(year, month, day, hour, minute, second, nDST) 构造函数
    CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST = -1) {
        struct tm atm;
        atm.tm_sec = nSec;
        atm.tm_min = nMin;
        atm.tm_hour = nHour;
        atm.tm_mday = nDay;
        atm.tm_mon = nMonth - 1;
        atm.tm_year = nYear - 1900;
        atm.tm_isdst = nDST;
        m_time = static_cast<std::int64_t>(std::mktime(&atm));
    }

    static CTime GetTickCount() {
        return CTime(static_cast<std::int64_t>(std::time(nullptr)));
    }

    std::int64_t GetTime() const {
        return m_time;
    }

    // 对齐 IDA: GetHour - 获取小时 (0-23)
    int GetHour() const {
        std::time_t t = m_time;
        struct tm* tm_info = std::localtime(&t);
        return tm_info ? tm_info->tm_hour : 0;
    }

    // 对齐 IDA: GetDay - 获取日期 (1-31)
    int GetDay() const {
        std::time_t t = m_time;
        struct tm* tm_info = std::localtime(&t);
        return tm_info ? tm_info->tm_mday : 1;
    }

    // 对齐 IDA: GetMonth - 获取月份 (1-12)
    int GetMonth() const {
        std::time_t t = m_time;
        struct tm* tm_info = std::localtime(&t);
        return tm_info ? tm_info->tm_mon + 1 : 1;
    }

    // 对齐 IDA: GetYear - 获取年份
    int GetYear() const {
        std::time_t t = m_time;
        struct tm* tm_info = std::localtime(&t);
        return tm_info ? tm_info->tm_year + 1900 : 1970;
    }

    // 对齐 IDA: GetDayOfWeek - 获取星期几 (1=周日, 7=周六)
    int GetDayOfWeek() const {
        std::time_t t = m_time;
        struct tm* tm_info = std::localtime(&t);
        return tm_info ? tm_info->tm_wday + 1 : 1;
    }

    // 对齐 IDA: GetMinute - 获取分钟 (0-59)
    int GetMinute() const {
        std::time_t t = m_time;
        struct tm* tm_info = std::localtime(&t);
        return tm_info ? tm_info->tm_min : 0;
    }

    // 对齐 IDA: GetSecond - 获取秒 (0-59)
    int GetSecond() const {
        std::time_t t = m_time;
        struct tm* tm_info = std::localtime(&t);
        return tm_info ? tm_info->tm_sec : 0;
    }

    // 对齐 IDA: CTime::operator- 支持 CTime 减法返回 CTimeSpan
    CTimeSpan operator-(const CTime& other) const {
        return CTimeSpan(m_time - other.m_time);
    }

    // 对齐 IDA: CTime::operator+ 支持 CTimeSpan 加法
    CTime operator+(const CTimeSpan& span) const {
        return CTime(m_time + span.m_timeSpan);
    }

    // 对齐 IDA: CTime::operator- 支持 CTimeSpan 减法
    CTime operator-(const CTimeSpan& span) const {
        return CTime(m_time - span.m_timeSpan);
    }

    // 对齐 IDA: CTime::operator< 支持比较
    bool operator<(const CTime& other) const {
        return m_time < other.m_time;
    }

    // 对齐 IDA: CTime::operator<= 支持比较
    bool operator<=(const CTime& other) const {
        return m_time <= other.m_time;
    }

    // 对齐 IDA: CTime::operator>= 支持比较
    bool operator>=(const CTime& other) const {
        return m_time >= other.m_time;
    }

    // 对齐 IDA: CTime::operator> 支持比较
    bool operator>(const CTime& other) const {
        return m_time > other.m_time;
    }

    // 对齐 IDA: CTime::operator== 支持比较
    bool operator==(const CTime& other) const {
        return m_time == other.m_time;
    }

    std::int64_t m_time = 0;
};
} // namespace ATL

// 使用CFSRWLock.h中的定义
#include "CFSRWLock.h"
#if 0
// 仅当未包含CFSRWLock.h时才定义这些结构
#ifndef CFSRWLOCK_H_ALREADY_DEFINED
struct CFSRWLock {
    CFSRWLock() : m_SharedLock(new std::shared_mutex()) {}

    ~CFSRWLock() {
        delete static_cast<std::shared_mutex*>(m_SharedLock);
        m_SharedLock = nullptr;
    }

    CFSRWLock(const CFSRWLock&) = delete;
    CFSRWLock& operator=(const CFSRWLock&) = delete;

    void lock() {
        static_cast<std::shared_mutex*>(m_SharedLock)->lock();
    }

    void unlock() {
        static_cast<std::shared_mutex*>(m_SharedLock)->unlock();
    }

    void lock_shared() {
        static_cast<std::shared_mutex*>(m_SharedLock)->lock_shared();
    }

    void unlock_shared() {
        static_cast<std::shared_mutex*>(m_SharedLock)->unlock_shared();
    }

    void* m_SharedLock = nullptr;
};

struct CFAutoSlimReadLock {
    explicit CFAutoSlimReadLock(CFSRWLock* lock) : m_pLock(lock) {
        if (m_pLock) {
            m_pLock->lock_shared();
        }
    }

    ~CFAutoSlimReadLock() {
        if (m_pLock) {
            m_pLock->unlock_shared();
        }
    }

    CFAutoSlimReadLock(const CFAutoSlimReadLock&) = delete;
    CFAutoSlimReadLock& operator=(const CFAutoSlimReadLock&) = delete;

    CFSRWLock* m_pLock = nullptr;
};

struct CFAutoSlimWriteLock {
    explicit CFAutoSlimWriteLock(CFSRWLock* lock) : m_pLock(lock) {
        if (m_pLock) {
            m_pLock->lock();
        }
    }

    ~CFAutoSlimWriteLock() {
        if (m_pLock) {
            m_pLock->unlock();
        }
    }

    CFAutoSlimWriteLock(const CFAutoSlimWriteLock&) = delete;
    CFAutoSlimWriteLock& operator=(const CFAutoSlimWriteLock&) = delete;

    CFSRWLock* m_pLock = nullptr;
};
#endif // 0
#endif // CFSRWLOCK_H_ALREADY_DEFINED



template <typename KeyType, typename ValueType>
class TXComposite {
public:
    virtual ~TXComposite() = default;

    template <typename T>
    T* GetComponentPtr(KeyType key) {
        ValueType* component = m_xMapComponet.GetAt(key);
        return dynamic_cast<T*>(component);
    }

    TXMap<KeyType, ValueType*> m_xMapComponet;
};

static_assert(sizeof(ATL::CAtlMap<std::uint8_t, IXProcess*>) == 0x48, "CAtlMap layout mismatch");
static_assert(sizeof(ATL::CTime) == 0x8, "CTime layout mismatch");
static_assert(sizeof(ATL::CTimeSpan) == 0x8, "CTimeSpan layout mismatch");
static_assert(sizeof(CFSRWLock) == 0x8, "CFSRWLock layout mismatch");
static_assert(sizeof(CFAutoSlimReadLock) == 0x8, "CFAutoSlimReadLock layout mismatch");
static_assert(sizeof(CFAutoSlimWriteLock) == 0x8, "CFAutoSlimWriteLock layout mismatch");
static_assert(sizeof(TXMap<std::uint8_t, IXProcess*>) == 0x48, "TXMap layout mismatch");
static_assert(sizeof(TXComposite<std::uint8_t, IXProcess>) == 0x50, "TXComposite layout mismatch");

/**
 * @brief `PerSocketContext` 的跨平台最小还原。
 *
 * 这是共享网络层的基础结构，不应继续滞留在 `XLoginServer/User.h`。
 */
struct PerSocketContext {
    void Init() {
        m_overLab.Init(-1, XOverLab::eOVERLAB_TYPE_READ);
        m_lock.Init();
        mWSASendCnt = 0;
    }

    void Destroy() {
        m_lock.Clear();
        m_overLab.Destroy();
    }

    XOverLab m_overLab;
    CSimpleLock m_lock;
    int mWSASendCnt = 0;
};

class XProcessComposite : public TXComposite<std::uint8_t, IXProcess> {
public:
    XProcessComposite() = default;
    ~XProcessComposite() override = default;

    bool Init(XClient* client);
    bool Parse(XPacket* packet);

private:
    friend class XClient;
    bool InsertProcess(IXProcess* process);
};

/**
 * @brief `XSocket` 的跨平台最小还原。
 *
 * 当前只恢复 `XSocket::Init` 与 `XClient/XIOCPServer` 已证实访问的字段。
 */
class XSocket : public IXObject {
public:
    enum E_BLOCK_TYPE : std::uint8_t {
        eBLOCK_OFF = 0,
        eBLOCK_ON = 1,
    };

    virtual ~XSocket() {
        xLock.Clear();
    }
    virtual bool Init();

    std::intptr_t Socket = -1;
    sockaddr_in scAddr{};
    E_BLOCK_TYPE eBlock = eBLOCK_OFF;
    std::array<char, 0xFFFF> szBuffer{};
    std::uint16_t usSize = 0xFFFFu;
    std::uint16_t usOffset = 0;
    std::uint16_t usInternal = 0;
    std::uint16_t usInternalHigh = 0;
    cIoContextPool* m_IoContextPool = nullptr;
    IoContextBuffer* m_IoContextFrontBuffer = nullptr;
    IoContextBuffer* m_IoContextBackBuffer = nullptr;
    int m_nSendCount = 0;
    std::uint64_t m_dwTick = 0;
    CSimpleLock xLock;
};

/**
 * @brief `XClient` 基类的最小还原。
 *
 * 该类属于共享网络/会话层，不应继续绑定在 `XLoginServer/User.h`。
 */
class XClient : public XSocket {
public:
    enum E_NET_STATE : std::uint16_t {
        eStateNone = 0,
        eStateConnect = 1,
        eStateLogin = 2,
        eStateInGame = 4,
        eStateLogOut = 8,
        eStateFinish = 16,
        eStateKickOut = 32,
        eStateChangeServer = 64,
        eStateChangeWorld = 128,
        eStateGoBackAuth = 256,
        eStateGoBackLobby = 512,
        eStateEnterWait = 1024,
        eStateEnterWaitDB = 2048,
        eStateEnterGameServer = 4096,
        eStateNoProcess = 224,
    };

    ~XClient() override;
    bool Init() override;
    virtual bool Init(XIOCPServer* pIOCPServer);
    virtual bool Parse(XPacket& xPacket);
    virtual bool OnLogOut() { return true; }
    virtual void OnUpdate(std::uint64_t currentTick) { (void)currentTick; }

    bool SendEx(XSendPacket& xSendPacket);
    bool SendErrorMessage(std::uint8_t mainCmd, std::uint8_t subCmd, std::uint16_t errorCode);
    void SetState(E_NET_STATE state) { m_eNetState = static_cast<E_NET_STATE>(m_eNetState | state); }
    void ClearState(E_NET_STATE state) { m_eNetState = static_cast<E_NET_STATE>(m_eNetState & ~state); }
    bool IsState(E_NET_STATE state) const { return state == static_cast<E_NET_STATE>(m_eNetState & state); }
    bool IsBit_OR(E_NET_STATE state);
    void IncrementJobCount() { ++m_nJobCount; }
    void DecrementJobCount() { --m_nJobCount; }

    // IDA: ?GetLogSendBuffer@XClient@@QEAAHXZ (0x1402A5130)
    // Returns the log send buffer size
    int GetLogSendBuffer() const { return m_nLogBuffSize; }

    template <typename T>
    T* GetProcessPtr(std::uint8_t ucCmd) {
        return m_xProcessComposite.GetComponentPtr<T>(ucCmd);
    }

protected:
    bool Register(std::uint8_t ucCmd, IXProcess* pProcess);

public:
    int m_nLogBuffSize = 0;
    int m_nTotalSendCount = 0;
    PerSocketContext m_socketContext;
    bool m_bInit = false;
    XIOCPServer* m_pIOCPServer = nullptr;
    XProcessComposite m_xProcessComposite;
    E_NET_STATE m_eNetState = eStateNone;
    bool m_bEncrypt = false;
    int m_nJobCount = 0;
    std::queue<XPacket*> m_packetQueue;
};

using CLIENT_STATE = XClient::E_NET_STATE;

inline constexpr CLIENT_STATE eStateConnect = XClient::eStateConnect;
inline constexpr CLIENT_STATE eStateLogin = XClient::eStateLogin;
inline constexpr CLIENT_STATE eStateInGame = XClient::eStateInGame;
inline constexpr CLIENT_STATE eStateLogOut = XClient::eStateLogOut;
inline constexpr CLIENT_STATE eStateFinish = XClient::eStateFinish;
inline constexpr CLIENT_STATE eStateKickOut = XClient::eStateKickOut;
inline constexpr CLIENT_STATE eStateChangeServer = XClient::eStateChangeServer;
inline constexpr CLIENT_STATE eStateChangeWorld = XClient::eStateChangeWorld;
inline constexpr CLIENT_STATE eStateGoBackAuth = XClient::eStateGoBackAuth;
inline constexpr CLIENT_STATE eStateGoBackLobby = XClient::eStateGoBackLobby;
inline constexpr CLIENT_STATE eStateEnterWait = XClient::eStateEnterWait;
inline constexpr CLIENT_STATE eStateEnterWaitDB = XClient::eStateEnterWaitDB;
inline constexpr CLIENT_STATE eStateEnterGameServer = XClient::eStateEnterGameServer;

static_assert(sizeof(XProcessComposite) == 0x50, "XProcessComposite layout mismatch");
