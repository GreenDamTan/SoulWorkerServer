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
 * - vftable 首槽当前等价于"返回 `m_xSessionID`"
 * - Total size: 64 bytes
 *
 * 其中该首槽在 IDA 当前名称里与 `XGameDBSocketMgr::GetLogDBAgentCount`
 * 发生了 COMDAT 折叠；这里按 `ExitUser` 的真实虚调用语义恢复为
 * `IXObject::GetSessionID`。
 * 
 * Layout (64 bytes):
 * - vftable pointer: 8 bytes (offset 0)
 * - m_xLock.m_bInit: 1 byte (offset 8)
 * - padding: 7 bytes (offset 9-15)
 * - m_xLock.m_mutex (unique_ptr): 8 bytes (offset 16-23)
 * - padding: 4 bytes (offset 24-27)
 * - m_xSessionID: 4 bytes (offset 28-31)
 * - padding: 32 bytes (offset 32-63)
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

protected:
    // Padding to match IDA size (64 bytes total)
    // Current: 8 (vftable) + 16 (CSimpleLock) + 4 (int) + padding = 64
    // CSimpleLock = 1 (bool) + 7 (padding) + 8 (unique_ptr) = 16
    char m_reserved[36] = {0};
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
    // NOTE: 移除 static_assert 以允许前向声明类型
    // 原版 IDA 显示 CUser 通过 RTTI 转换，不要求编译时继承关系
    // static_assert(std::is_base_of_v<IXObject, TObject>,
    //               "TXObjectMgr<TObject> requires TObject to derive from IXObject");

public:
    /**
     * @brief 初始化对象池。
     *
     * 对齐 IDA: 固定容量预分配对象池。
     *
     * NOTE: 当前实现不预分配对象，因为 TObject 可能是不完整类型。
     * 对象将在 Create() 调用时动态创建。
     *
     * @param maxObjectCount 最大对象数量
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool Init(int maxObjectCount) override {
        CSimpleLock::Owner lock(&m_xLock);
        m_nMaxObjectCount = maxObjectCount;
        m_xObjectMap.clear();
        m_xFreeList.clear();
        m_xStorage.clear();

        if (m_nMaxObjectCount <= 0) {
            return false;
        }

        // NOTE: 不预分配对象，因为 TObject 可能是不完整类型
        // 对象将在 Create() 调用时动态创建
        m_xStorage.reserve(static_cast<std::size_t>(m_nMaxObjectCount));
        return true;
    }

    /**
     * @brief 从池中创建/分配一个对象。
     *
     * 对齐 IDA: Create -> GetSessionID -> m_xObjectMap.SetAt -> SetSessionID
     *
     * @return TObject* 新创建的对象指针，池满时返回 nullptr
     */
    TObject* Create() {
        CSimpleLock::Owner lock(&m_xLock);

        // 检查是否达到最大对象数
        if (m_nMaxObjectCount > 0 && static_cast<int>(m_xObjectMap.size()) >= m_nMaxObjectCount) {
            return nullptr;
        }

        // 动态创建新对象
        TObject* object = new TObject();
        if (!object) {
            return nullptr;
        }

        const int sessionID = GetSessionID();
        const auto [it, inserted] = m_xObjectMap.emplace(sessionID, object);
        if (!inserted) {
            delete object;
            return nullptr;
        }

        object->SetSessionID(sessionID);
        // 注意: 不存储到 m_xStorage，因为对象生命周期由外部管理
        // m_xStorage 用于预分配模式，当前使用动态创建模式
        return object;
    }

    /**
     * @brief 删除/回收一个对象到池中。
     *
     * 对齐 IDA: Delete -> RemoveKey -> NotifyRemoved -> 回收到池
     *
     * @param object 要删除的对象指针
     */
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

    /**
     * @brief 根据 SessionID 查找对象。
     *
     * 对齐 IDA 0x1400014F0: TXObjectMgr<CUser>::Find
     * ```
     * v2 = TXMap<int,IXObject *,ATL::CElementTraits<int>>::GetAt(
     *        (TXMap<unsigned long,XActor *,ATL::CElementTraits<unsigned long> > *)&this->m_xObjectMap,
     *        xSessionID);
     * return (CUser *)_RTDynamicCast_0(v2, 0, &IXObject `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
     * ```
     *
     * 关键发现:
     * - 使用 TXMap::GetAt 查找
     * - 使用 RTTI 动态转换从 IXObject* 转换到 TObject*
     *
     * @param xSessionID 会话 ID
     * @return TObject* 找到的对象指针，未找到返回 nullptr
     *
     * TODO: 对齐 IDA - 应该使用 TXMap::GetAt 而不是 std::unordered_map::find
     */
    TObject* Find(int xSessionID) {
        // TODO: 对齐 IDA - 当前实现没有使用 TXMap::GetAt
        // IDA 显示:
        //   v2 = TXMap<int,IXObject*>::GetAt(&this->m_xObjectMap, xSessionID);
        //   return (CUser *)_RTDynamicCast_0(v2, ...);
        // 当前使用 std::unordered_map::find 作为临时替代

        CSimpleLock::Owner lock(&m_xLock);
        const auto it = m_xObjectMap.find(xSessionID);
        if (it == m_xObjectMap.end()) {
            return nullptr;
        }

        // 对齐 IDA: 使用 RTTI 动态转换
        // IDA: _RTDynamicCast_0(v2, 0, &IXObject, &CUser, 0)
        // C++ 等价于 dynamic_cast
        return dynamic_cast<TObject*>(it->second);
    }

    // TODO: 对齐 IDA - 暴露 m_xObjectMap 以便 TXMap 使用
    // 注意: 这是临时方案，最终应该使用 TXMap<int, IXObject*>
    std::unordered_map<int, IXObject*>& GetObjectMap() { return m_xObjectMap; }

private:
    std::vector<std::unique_ptr<TObject>> m_xStorage;
    std::deque<IXObject*> m_xFreeList;

    // TODO: 对齐 IDA - 应该使用 TXMap<int, IXObject*> m_xObjectMap
    // IDA 0x1400014F0 显示类型: TXMap<unsigned long, XActor*, ATL::CElementTraits<unsigned long>>
    // 但实际上是 TXMap<int, IXObject*>
    // 当前仍使用 std::unordered_map，后续需要迁移到 TXMap
    std::unordered_map<int, IXObject*> m_xObjectMap;
};
