#pragma once

#include <cstddef>
#include <cstdint>
#include <list>
#include <queue>

// This header is included from IXObject.h after CSimpleLock and IXObject exist.

// The target MSVC STL stores the pointer queue in 48 bytes; the active clang-cl
// STL stores the same std::queue in 40 bytes. Keep that ABI-only adjustment out
// of TXPool's declared member list.
template <typename TObject>
class GreenDamTan_TxQueue {
public:
    bool empty() const { return m_queue.empty(); }
    std::size_t size() const { return m_queue.size(); }
    TObject* front() { return m_queue.front(); }
    const TObject* front() const { return m_queue.front(); }
    void pop() { m_queue.pop(); }
    void push(TObject* pObject) { m_queue.push(pObject); }

private:
    std::queue<TObject*> m_queue;
    std::uint64_t m_layoutCompatibility = 0;
};

static_assert(sizeof(GreenDamTan_TxQueue<int>) == 48,
              "TXPool queue ABI mismatch");

template <typename TObject>
class TXPool {
public:
    class IXCreator {
    public:
        IXCreator() = default;
        virtual ~IXCreator() = default;
        virtual TObject* Create() = 0;
    };

    class IXDeletor {
    public:
        IXDeletor() = default;
        virtual ~IXDeletor() = default;
        virtual void Delete(TObject* pObj) = 0;
    };

    template <typename TDerived>
    class TXCreator : public IXCreator {
    public:
        TObject* Create() override {
            return static_cast<TObject*>(new TDerived());
        }
    };

    TXPool() {
        m_xLock.Init();
    }

    virtual ~TXPool() {
        Clear(nullptr);
        m_xLock.Clear();
    }

    bool Init(int nMaxSize, IXCreator* pCreator, bool bThreadSafe = false) {
        m_nCurMaxSize = nMaxSize;
        m_bThreadSafe = bThreadSafe;
        if (!pCreator || nMaxSize <= 0) {
            return true;
        }

        SafeArrayLock();
        for (int i = 0; i < nMaxSize; ++i) {
            TObject* pObject = pCreator->Create();
            if (pObject) {
                m_Queue.push(pObject);
            }
        }
        SafeArrayUnLock();
        return true;
    }

    TObject* Pop() {
        SafeArrayLock();
        if (m_Queue.empty()) {
            SafeArrayUnLock();
            return nullptr;
        }

        TObject* pObject = m_Queue.front();
        m_Queue.pop();
        SafeArrayUnLock();
        return pObject;
    }

    void Push(TObject* pObject) {
        SafeArrayLock();
        m_Queue.push(pObject);
        SafeArrayUnLock();
    }

    void Clear(IXDeletor* pDeletor = nullptr) {
        SafeArrayLock();
        while (!m_List.empty()) {
            TObject* pObject = m_List.front();
            m_List.pop_front();
            if (pDeletor) {
                pDeletor->Delete(pObject);
            } else {
                delete pObject;
            }
        }
        while (!m_Queue.empty()) {
            TObject* pObject = m_Queue.front();
            m_Queue.pop();
            if (pDeletor) {
                pDeletor->Delete(pObject);
            } else {
                delete pObject;
            }
        }
        SafeArrayUnLock();
    }

    void Add(TObject* pObject) {
        SafeArrayLock();
        m_List.push_back(pObject);
        SafeArrayUnLock();
    }

    void AddToList(TObject* pObject) {
        Add(pObject);
    }

    void RemoveFromList(TObject* pObject) {
        SafeArrayLock();
        m_List.remove(pObject);
        SafeArrayUnLock();
    }

    int GetFullSize() const {
        return static_cast<int>(m_Queue.size() + m_List.size());
    }

    int GetCurSize() const {
        return static_cast<int>(m_Queue.size());
    }

    int GetCurMaxSize() const {
        return m_nCurMaxSize;
    }

    bool IsEmpty() const {
        return m_Queue.empty();
    }

    std::size_t GetSize() const {
        return m_Queue.size();
    }

    std::list<TObject*>& GetList() {
        return m_List;
    }

    const std::list<TObject*>& GetList() const {
        return m_List;
    }

    void SetSafeArrayFlag(bool bThreadSafe) {
        m_bThreadSafe = bThreadSafe;
    }

    void SafeArrayLock() {
        if (m_bThreadSafe) {
            m_xLock.Lock();
        }
    }

    void SafeArrayUnLock() {
        if (m_bThreadSafe) {
            m_xLock.UnLock();
        }
    }

protected:
    CSimpleLock m_xLock;
    GreenDamTan_TxQueue<TObject> m_Queue;
    std::list<TObject*> m_List;
    bool m_bThreadSafe = false;
    int m_nCurMaxSize = 0;
};

template <>
class TXPool<IXObject> {
public:
    class IXCreator {
    public:
        IXCreator() = default;
        virtual ~IXCreator() = default;
        virtual IXObject* Create() = 0;
    };

    class IXDeletor {
    public:
        IXDeletor() = default;
        virtual ~IXDeletor() = default;
        virtual void Delete(IXObject* pObj) = 0;
    };

    template <typename TDerived>
    class TXCreator : public IXCreator {
    public:
        IXObject* Create() override {
            return static_cast<IXObject*>(new TDerived());
        }
    };

    TXPool() {
        m_xLock.Init();
    }

    virtual ~TXPool() {
        Clear(nullptr);
        m_xLock.Clear();
    }

    bool Init(int nMaxSize, IXCreator* pCreator, bool bThreadSafe = false) {
        m_nCurMaxSize = nMaxSize;
        m_bThreadSafe = bThreadSafe;
        if (!pCreator || nMaxSize <= 0) {
            return true;
        }

        SafeArrayLock();
        for (int i = 0; i < nMaxSize; ++i) {
            IXObject* pObject = pCreator->Create();
            if (pObject) {
                m_Queue.push(pObject);
            }
        }
        SafeArrayUnLock();
        return true;
    }

    IXObject* Pop() {
        SafeArrayLock();
        if (m_Queue.empty()) {
            SafeArrayUnLock();
            return nullptr;
        }

        IXObject* pObject = m_Queue.front();
        m_Queue.pop();
        SafeArrayUnLock();
        return pObject;
    }

    void Push(IXObject* pObject) {
        SafeArrayLock();
        m_Queue.push(pObject);
        SafeArrayUnLock();
    }

    void Clear(IXDeletor* pDeletor = nullptr) {
        SafeArrayLock();
        while (!m_List.empty()) {
            IXObject* pObject = m_List.front();
            m_List.pop_front();
            if (pDeletor) {
                pDeletor->Delete(pObject);
            } else {
                delete pObject;
            }
        }
        while (!m_Queue.empty()) {
            IXObject* pObject = m_Queue.front();
            m_Queue.pop();
            if (pDeletor) {
                pDeletor->Delete(pObject);
            } else {
                delete pObject;
            }
        }
        SafeArrayUnLock();
    }

    void Add(IXObject* pObject) {
        SafeArrayLock();
        m_List.push_back(pObject);
        SafeArrayUnLock();
    }

    void AddToList(IXObject* pObject) {
        Add(pObject);
    }

    void RemoveFromList(IXObject* pObject) {
        SafeArrayLock();
        m_List.remove(pObject);
        SafeArrayUnLock();
    }

    int GetFullSize() const {
        return static_cast<int>(m_Queue.size() + m_List.size());
    }

    int GetCurSize() const {
        return static_cast<int>(m_Queue.size());
    }

    int GetCurMaxSize() const {
        return m_nCurMaxSize;
    }

    bool IsEmpty() const {
        return m_Queue.empty();
    }

    std::size_t GetSize() const {
        return m_Queue.size();
    }

    std::list<IXObject*>& GetList() {
        return m_List;
    }

    const std::list<IXObject*>& GetList() const {
        return m_List;
    }

    void SetSafeArrayFlag(bool bThreadSafe) {
        m_bThreadSafe = bThreadSafe;
    }

    void SafeArrayLock() {
        if (m_bThreadSafe) {
            m_xLock.Lock();
        }
    }

    void SafeArrayUnLock() {
        if (m_bThreadSafe) {
            m_xLock.UnLock();
        }
    }

protected:
    CSimpleLock m_xLock;
    GreenDamTan_TxQueue<IXObject> m_Queue;
    std::list<IXObject*> m_List;
    bool m_bThreadSafe = false;
    int m_nCurMaxSize = 0;
};

static_assert(sizeof(TXPool<int>) == 136, "TXPool layout mismatch");
