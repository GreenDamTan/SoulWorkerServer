#pragma once

#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "GreenDamTan_TxMap.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <windows.h>
#endif

/**
 * @brief `CSimpleLock` 的跨平台最小还原。
 *
 * PDB layout: `m_CriticalSection` at offset 0, `m_bInit` at offset 40,
 * total size 48 bytes. Windows therefore uses the native 40-byte
 * `CRITICAL_SECTION`; the non-Windows path retains the same object size.
 */
class CSimpleLock {
public:
    class Owner {
    public:
        explicit Owner(CSimpleLock* lock) : m_xLock(lock) {
            if (m_xLock) {
                m_xLock->Lock();
            }
        }

        ~Owner() {
            if (m_xLock) {
                m_xLock->UnLock();
            }
        }

        Owner(const Owner&) = delete;
        Owner& operator=(const Owner&) = delete;

    private:
        CSimpleLock* m_xLock = nullptr;
    };

    CSimpleLock() = default;
    ~CSimpleLock() {
        Clear();
    }

    CSimpleLock(const CSimpleLock&) = delete;
    CSimpleLock& operator=(const CSimpleLock&) = delete;

    void Init() {
        if (m_bInit) {
            return;
        }

#ifdef _WIN32
        InitializeCriticalSection(&m_CriticalSection);
#else
        m_mutex = std::make_unique<std::recursive_mutex>();
#endif
        m_bInit = true;
    }

    void Clear() {
        if (!m_bInit) {
            return;
        }

#ifdef _WIN32
        DeleteCriticalSection(&m_CriticalSection);
#else
        m_mutex.reset();
#endif
        m_bInit = false;
    }

    void Lock() {
        if (!m_bInit) {
            Init();
        }

#ifdef _WIN32
        EnterCriticalSection(&m_CriticalSection);
#else
        m_mutex->lock();
#endif
    }

    void UnLock() {
        if (!m_bInit) {
            return;
        }

#ifdef _WIN32
        LeaveCriticalSection(&m_CriticalSection);
#else
        m_mutex->unlock();
#endif
    }

protected:
#ifdef _WIN32
    CRITICAL_SECTION m_CriticalSection{};
#else
    std::unique_ptr<std::recursive_mutex> m_mutex;
    std::uint8_t m_padding[31] = {};
#endif
    bool m_bInit = false;
};

#ifdef _WIN32
static_assert(sizeof(CRITICAL_SECTION) == 40,
              "CRITICAL_SECTION size must match GameServer PDB");
#endif
static_assert(sizeof(CSimpleLock) == 48,
              "CSimpleLock size must match GameServer PDB");

/**
 * @brief `IXObject` 的最小落地。
 *
 * IDA 已确认：
 * - 构造函数会初始化 `m_xLock`
 * - `SetSessionID` 直接写 `m_xSessionID`
 * - vftable 首槽当前等价于"返回 `m_xSessionID`"
 * - Total size: 64 bytes
 *
 * 其中该首槽在 IDA 当前名称里与 `XGameDBSocketMgr::GetLogDBAgentCount`
 * 发生了 COMDAT 折叠；这里按 `ExitUser` 的真实虚调用语义恢复为
 * `IXObject::GetSessionID`。
 * 
 * Layout (64 bytes):
 * - vftable pointer: 8 bytes (offset 0)
 * - m_xLock: 48 bytes (offset 8)
 * - m_xSessionID: 4 bytes (offset 56)
 * - tail padding: 4 bytes (offset 60-63)
 */
class IXObject {
public:
    IXObject() {
        m_xLock.Init();
    }

    virtual ~IXObject() {
        m_xLock.Clear();
    }

    virtual int GetSessionID() {
        return m_xSessionID;
    }

    virtual void SetSessionID(int xSessionID) {
        m_xSessionID = xSessionID;
    }

    virtual void NotifyRemoved() {
        m_xSessionID = 0;
    }

    CSimpleLock m_xLock;
    int m_xSessionID = 0;
};

static_assert(sizeof(IXObject) == 64, "IXObject size mismatch - expected 64 bytes from IDA");

/**
 * @brief 登录服启动链里出现的对象管理器最小还原。
 *
 * 本轮依据 `IXObjectMgr::GetSessionID(0x140029100)` 收紧：
 * 1. 该类持有独立会话号计数器
 * 2. 取号时加锁
 * 3. 超过 `0x1FFFFFFF` 后回卷到 `1000`
 */
class IXObjectMgr {
public:
    IXObjectMgr()
        : m_nMaxSize(0)
        , m_xLock()
        , m_xSessionIdx(0) {
        m_xLock.Init();
    }

    virtual ~IXObjectMgr() {
        m_xLock.Clear();
    }

    virtual bool Init(int maxObjectCount) = 0;

    int GetSessionID() {
        CSimpleLock::Owner xLock(&m_xLock);
        if (++m_xSessionIdx > 0x1FFFFFFFu) {
            m_xSessionIdx = 1000;
        }
        return static_cast<int>(m_xSessionIdx);
    }

protected:
    int m_nMaxSize;
    CSimpleLock m_xLock;
    std::uint32_t m_xSessionIdx;

};

static_assert(sizeof(IXObjectMgr) == 72,
              "IXObjectMgr size mismatch");

#include "GreenDamTan_TxPool.h"

/**
 * @brief `TXObjectMgr<TObject>` 的跨平台还原。
 *
 * 根据 IDA 中当前已核实的对象管理器主链：
 * - `TXObjectMgr<...>::Init   @ 0x14002ab70`
 * - `TXObjectMgr<...>::Create @ 0x14001e280`
 * - `TXObjectMgr<...>::Delete @ 0x14002b5a0`
 * - `TXObjectMgr<...>::Find   @ 0x1400014f0`
 *
 * IDA 0x1400014F0 TXObjectMgr<CUser>::Find 反编译:
 * ```cpp
 * CUser *__fastcall TXObjectMgr<CUser>::Find(TXObjectMgr<CUser> *this, unsigned int xSessionID)
 * {
 *   XActor *v2 = TXMap<int,IXObject *,ATL::CElementTraits<int>>::GetAt(
 *          (TXMap<unsigned long,XActor *,ATL::CElementTraits<unsigned long> > *)&this->m_xObjectMap,
 *          xSessionID);
 *   return (CUser *)_RTDynamicCast_0(v2, 0, &IXObject `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
 * }
 * ```
 *
 * 关键发现:
 * 1. 使用 TXMap<int, IXObject*> 作为对象映射类型
 * 2. Find 方法使用 RTTI 动态转换 (_RTDynamicCast)
 * 3. 固定容量对象池初始化
 * 4. `Create -> GetSessionID -> m_xObjectMap.SetAt -> SetSessionID`
 * 5. `Delete -> RemoveKey -> NotifyRemoved -> 回收到池`
 *
 * TODO: 当前使用 std::unordered_map 替代 TXMap，需要迁移到 TXMap
 * TODO: TXMap 在 GreenDamTan_ClientBase.h 中定义，需要处理头文件依赖
 */
template <typename TObject>
class TXObjectMgr : public IXObjectMgr {
public:
    virtual ~TXObjectMgr() {
        if (m_nMaxSize) {
            Clear(nullptr);
        }
    }

    TObject* Create() {
        IXObject* object = m_xPool.Pop();
        if (!object) {
            return nullptr;
        }

        const int sessionID = GetSessionID();
        if (!m_xObjectMap.SetAt(sessionID, object)) {
            m_xPool.Push(object);
            return nullptr;
        }

        object->SetSessionID(sessionID);
        m_xPool.Add(object);
        return dynamic_cast<TObject*>(object);
    }

    TObject* Find(int xSessionID) {
        return dynamic_cast<TObject*>(m_xObjectMap.GetAt(xSessionID));
    }

    bool Init(int maxObjectCount) override {
        m_nMaxSize = maxObjectCount;
        m_xObjectMap.RemoveAll();
        m_xPool.SetSafeArrayFlag(true);
        return true;
    }

    virtual void Delete(IXObject* object) {
        if (!object) {
            return;
        }

        if (m_xObjectMap.RemoveKey(object->GetSessionID())) {
            object->NotifyRemoved();
            m_xPool.RemoveFromList(object);
            m_xPool.Push(object);
        }
    }

    virtual void Clear(TXPool<IXObject>::IXDeletor* pDeletor = nullptr) {
        m_xObjectMap.RemoveAll();
        m_xPool.Clear(pDeletor);
    }

    int GetQueueSize() const {
        return m_xPool.GetCurSize();
    }

    int GetMaxQueueSize() const {
        return m_xPool.GetFullSize();
    }

    int GetCurMaxQueue() const {
        return m_xPool.GetCurMaxSize();
    }

    TXMap<int, IXObject*>& GetObjectMap() {
        return m_xObjectMap;
    }

public:
    TXMap<int, IXObject*> m_xObjectMap;
    TXPool<IXObject> m_xPool;
};

static_assert(sizeof(TXObjectMgr<int>) == 280,
              "TXObjectMgr layout mismatch");
