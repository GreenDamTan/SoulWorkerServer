#pragma once

#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <type_traits>
#include <unordered_map>
#include <vector>

/**
 * @brief `CSimpleLock` 的跨平台最小还原。
 *
 * 原版基于 `CRITICAL_SECTION`；当前继续用 `std::recursive_mutex`
 * 维持 `Init/Lock/UnLock` 外形，避免把平台差异散落到业务代码。
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

    void Init() {
        if (m_bInit && m_mutex) {
            return;
        }
        m_mutex = std::make_unique<std::recursive_mutex>();
        m_bInit = true;
    }

    void Destroy() {
        if (!m_bInit) {
            return;
        }

        m_mutex.reset();
        m_bInit = false;
    }

    void Lock() {
        if (!m_bInit || !m_mutex) {
            Init();
        }

        m_mutex->lock();
    }

    void UnLock() {
        if (!m_bInit || !m_mutex) {
            return;
        }

        m_mutex->unlock();
    }

    bool m_bInit = false;

private:
    std::unique_ptr<std::recursive_mutex> m_mutex;
};

/**
 * @brief `IXObject` 的最小落地。
 *
 * IDA 已确认：
 * - 构造函数会初始化 `m_xLock`
 * - `SetSessionID` 直接写 `m_xSessionID`
 * - vftable 首槽当前等价于“返回 `m_xSessionID`”
 *
 * 其中该首槽在 IDA 当前名称里与 `XGameDBSocketMgr::GetLogDBAgentCount`
 * 发生了 COMDAT 折叠；这里按 `ExitUser` 的真实虚调用语义恢复为
 * `IXObject::GetSessionID`。
 */
class IXObject {
public:
    IXObject() {
        m_xLock.Init();
    }

    virtual ~IXObject() {
        m_xLock.Destroy();
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
    IXObjectMgr() {
        m_xLock.Init();
    }

    virtual ~IXObjectMgr() = default;

    virtual bool Init(int maxObjectCount) {
        m_nMaxObjectCount = maxObjectCount;
        return true;
    }

    int GetSessionID() {
        CSimpleLock::Owner xLock(&m_xLock);
        if (++m_xSessionIdx > 0x1FFFFFFFu) {
            m_xSessionIdx = 1000;
        }
        return static_cast<int>(m_xSessionIdx);
    }

protected:
    CSimpleLock m_xLock;
    std::uint32_t m_xSessionIdx = 0;
    int m_nMaxObjectCount = 0;
};

// 前置声明 - TXMap 在 GreenDamTan_ClientBase.h 中定义
template <typename KeyType, typename ValueType, typename KeyTraits>
struct TXMap;

// 前置声明 - ATL::CAtlMap 在 GreenDamTan_ClientBase.h 中定义
namespace ATL {
template <typename KeyType, typename ValueType, typename KeyTraits, typename ValueTraits>
class CAtlMap;
}

/**
 * @brief `TXObjectMgr<TObject>` 的最小跨平台还原。
 *
 * 根据 IDA 中当前已核实的对象管理器主链：
 * - `TXObjectMgr<...>::Init   @ 0x14002ab70`
 * - `TXObjectMgr<...>::Create @ 0x14001e280`
 * - `TXObjectMgr<...>::Delete @ 0x14002b5a0`
 * - `TXObjectMgr<...>::Find   @ 0x1400014f0`
 *
 * IDA 0x1400014F0 TXObjectMgr<CUser>::Find 反编译:
 * ```
 * CUser *__fastcall TXObjectMgr<CUser>::Find(TXObjectMgr<CUser> *this, unsigned int xSessionID)
 * {
 *   XActor *v2 = TXMap<int,IXObject *,ATL::CElementTraits<int>>::GetAt(
 *          (TXMap<unsigned long,XActor *,ATL::CElementTraits<unsigned long> > *)&this->m_xObjectMap,
 *          xSessionID);
 *   return (CUser *)_RTDynamicCast_0(v2, 0, &IXObject `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
 * }
 * ```
 *
 * 当前已明确恢复：
 * 1. 固定容量对象池初始化
 * 2. `Create -> GetSessionID -> m_xObjectMap.SetAt -> SetSessionID`
 * 3. `Delete -> RemoveKey -> NotifyRemoved -> 回收到池`
 * 4. `Find -> m_xObjectMap.GetAt` + RTTI 动态转换
 */
template <typename TObject>
class TXObjectMgr : public IXObjectMgr {
    static_assert(std::is_base_of_v<IXObject, TObject>,
                  "TXObjectMgr<TObject> requires TObject to derive from IXObject");

public:
    // 前向声明 GetAt 辅助函数 - 实际实现在 GreenDamTan_ClientBase.h 中
    // 使用 ATL::CAtlMap::Lookup 来实现 GetAt 功能

    bool Init(int maxObjectCount) override {
        CSimpleLock::Owner lock(&m_xLock);
        m_nMaxObjectCount = maxObjectCount;
        m_xObjectMap.clear();
        m_xFreeList.clear();
        m_xStorage.clear();

        if (m_nMaxObjectCount <= 0) {
            return false;
        }

        m_xStorage.reserve(static_cast<std::size_t>(m_nMaxObjectCount));
        m_xFreeList.resize(static_cast<std::size_t>(m_nMaxObjectCount), nullptr);
        for (int index = 0; index < m_nMaxObjectCount; ++index) {
            auto object = std::make_unique<TObject>();
            IXObject* rawObject = object.get();
            m_xStorage.push_back(std::move(object));
            m_xFreeList[static_cast<std::size_t>(index)] = rawObject;
        }
        return true;
    }

    TObject* Create() {
        CSimpleLock::Owner lock(&m_xLock);
        if (m_xFreeList.empty()) {
            return nullptr;
        }

        IXObject* object = m_xFreeList.back();
        m_xFreeList.pop_back();
        if (!object) {
            return nullptr;
        }

        const int sessionID = GetSessionID();
        const auto [it, inserted] = m_xObjectMap.emplace(sessionID, object);
        if (!inserted) {
            m_xFreeList.push_back(object);
            return nullptr;
        }

        object->SetSessionID(sessionID);
        return dynamic_cast<TObject*>(object);
    }

    void Delete(IXObject* object) {
        if (!object) {
            return;
        }

        CSimpleLock::Owner lock(&m_xLock);
        const auto it = m_xObjectMap.find(object->GetSessionID());
        if (it == m_xObjectMap.end()) {
            return;
        }

        m_xObjectMap.erase(it);
        object->NotifyRemoved();
        m_xFreeList.push_back(object);
    }

    // 对齐 IDA 0x1400014F0: TXObjectMgr<CUser>::Find
    // IDA 显示: TXMap::GetAt + _RTDynamicCast
    TObject* Find(int sessionID) {
        CSimpleLock::Owner lock(&m_xLock);
        // 对齐 IDA: 使用 GetAt 查找，然后使用 RTTI 动态转换
        const auto it = m_xObjectMap.find(sessionID);
        if (it == m_xObjectMap.end()) {
            return nullptr;
        }
        // 对齐 IDA: _RTDynamicCast 从 IXObject 转换到 TObject
        return dynamic_cast<TObject*>(it->second);
    }

    // 对齐 IDA: 暴露 m_xObjectMap 以便 TXMap 使用
    // 注意: 这是临时方案，最终应该使用 TXMap<int, IXObject*>
    std::unordered_map<int, IXObject*>& GetObjectMap() { return m_xObjectMap; }

private:
    std::vector<std::unique_ptr<TObject>> m_xStorage;
    std::deque<IXObject*> m_xFreeList;
    // TODO: 对齐 IDA 使用 TXMap<int, IXObject*>
    // 当前仍使用 std::unordered_map，后续需要迁移到 TXMap
    std::unordered_map<int, IXObject*> m_xObjectMap;
};
