// TXPool.h
// Object pool template class for SoulWorker server
// Reconstructed from IDA decompilation

#pragma once

#include <list>
#include <queue>
#include <cstdint>
#include "Soulworker/GameServer/XCore/XServer/IXObject.h"

// Forward declaration
class CMonster;

/**
 * @brief TXPool - Object pool template for managing IXObject-derived objects
 *
 * IDA Analysis:
 * - Constructor @ 0x140189BA0
 * - Destructor @ 0x14018A9D0
 * - Clear @ 0x14018A700
 *
 * Layout:
 * - vftable pointer: 8 bytes
 * - m_xLock: CSimpleLock (16 bytes)
 * - m_Queue: std::queue<IXObject*> (48 bytes typically)
 * - m_List: std::list<IXObject*> (24 bytes typically)
 * - m_bThreadSafe: 1 byte
 * - m_nCurMaxSize: 4 bytes
 */
template <typename TObject>
class TXPool {
public:
    /**
     * @brief IXCreator - Interface for object creation
     * IDA @ 0x14018B3B0
     */
    class IXCreator {
    public:
        IXCreator() {}
        virtual ~IXCreator() = default;
        virtual TObject* Create() = 0;
    };

    /**
     * @brief IXDeletor - Interface for object deletion
     * IDA @ 0x140189A30
     */
    class IXDeletor {
    public:
        IXDeletor() {}
        virtual ~IXDeletor() = default;
        virtual void Delete(TObject* pObj) = 0;
    };

    /**
     * @brief TXCreator - Template creator for specific object types
     * IDA @ 0x140365680 (TXPool<IXObject>::TXCreator<CMonster>)
     */
    template <typename TDerived>
    class TXCreator : public IXCreator {
    public:
        TXCreator() : IXCreator() {}

        TObject* Create() override {
            // IDA @ 0x140365600 - Creates object via operator new and constructor
            TDerived* obj = new TDerived();
            return static_cast<TObject*>(obj);
        }
    };

public:
    TXPool() {
        m_bThreadSafe = false;
        m_nCurMaxSize = 0;
        m_xLock.Init();
    }

    virtual ~TXPool() {
        // IDA @ 0x14018A9D0 - Clears queue and list, destroys lock
        m_Queue = std::queue<TObject*>();
        m_List.clear();
        m_xLock.Destroy();
    }

    /**
     * @brief Initialize pool with size and creator
     * @param nMaxSize Maximum pool size
     * @param pCreator Object creator interface
     * @param bThreadSafe Enable thread safety
     * @return true on success
     */
    bool Init(int nMaxSize, IXCreator* pCreator, bool bThreadSafe = false) {
        m_nCurMaxSize = nMaxSize;
        m_bThreadSafe = bThreadSafe;

        if (pCreator && nMaxSize > 0) {
            SafeArrayLock();
            for (int i = 0; i < nMaxSize; ++i) {
                TObject* pObj = pCreator->Create();
                if (pObj) {
                    m_Queue.push(pObj);
                }
            }
            SafeArrayUnLock();
        }
        return true;
    }

    /**
     * @brief Clear pool using optional deletor
     * IDA @ 0x14018A700
     */
    void Clear(IXDeletor* pDeletor = nullptr) {
        SafeArrayLock();

        for (auto it = m_List.begin(); it != m_List.end(); ++it) {
            TObject* pData = *it;
            if (pDeletor) {
                pDeletor->Delete(pData);
            } else if (pData) {
                delete pData;
            }
            if (!m_Queue.empty()) {
                m_Queue.pop();
            }
        }

        m_List.clear();
        SafeArrayUnLock();
    }

    /**
     * @brief Push object to pool queue
     * IDA @ 0x14018B4A0
     */
    void Push(TObject* pObj) {
        SafeArrayLock();
        m_Queue.push(pObj);
        SafeArrayUnLock();
    }

    /**
     * @brief Pop object from pool queue
     */
    TObject* Pop() {
        SafeArrayLock();
        if (m_Queue.empty()) {
            SafeArrayUnLock();
            return nullptr;
        }
        TObject* pObj = m_Queue.front();
        m_Queue.pop();
        SafeArrayUnLock();
        return pObj;
    }

    /**
     * @brief Add object to active list
     */
    void AddToList(TObject* pObj) {
        SafeArrayLock();
        m_List.push_back(pObj);
        SafeArrayUnLock();
    }

    /**
     * @brief Remove object from active list
     */
    void RemoveFromList(TObject* pObj) {
        SafeArrayLock();
        m_List.remove(pObj);
        SafeArrayUnLock();
    }

    /**
     * @brief Check if pool is empty
     */
    bool IsEmpty() const {
        return m_Queue.empty();
    }

    /**
     * @brief Get current pool size
     */
    size_t GetSize() const {
        return m_Queue.size();
    }

    /**
     * @brief Get active list
     */
    std::list<TObject*>& GetList() { return m_List; }
    const std::list<TObject*>& GetList() const { return m_List; }

protected:
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
    std::queue<TObject*> m_Queue;
    std::list<TObject*> m_List;
    bool m_bThreadSafe;
    int m_nCurMaxSize;
};

// Specialization for IXObject base type
template<>
class TXPool<IXObject> {
public:
    class IXCreator {
    public:
        IXCreator() {}
        virtual ~IXCreator() = default;
        virtual IXObject* Create() = 0;
    };

    class IXDeletor {
    public:
        IXDeletor() {}
        virtual ~IXDeletor() = default;
        virtual void Delete(IXObject* pObj) = 0;
    };

    template <typename TDerived>
    class TXCreator : public IXCreator {
    public:
        TXCreator() : IXCreator() {}

        IXObject* Create() override {
            TDerived* obj = new TDerived();
            return static_cast<IXObject*>(obj);
        }
    };

public:
    TXPool() {
        m_bThreadSafe = false;
        m_nCurMaxSize = 0;
        m_xLock.Init();
    }

    virtual ~TXPool() {
        m_Queue = std::queue<IXObject*>();
        m_List.clear();
        m_xLock.Destroy();
    }

    bool Init(int nMaxSize, IXCreator* pCreator, bool bThreadSafe = false) {
        m_nCurMaxSize = nMaxSize;
        m_bThreadSafe = bThreadSafe;

        if (pCreator && nMaxSize > 0) {
            SafeArrayLock();
            for (int i = 0; i < nMaxSize; ++i) {
                IXObject* pObj = pCreator->Create();
                if (pObj) {
                    m_Queue.push(pObj);
                }
            }
            SafeArrayUnLock();
        }
        return true;
    }

    void Clear(IXDeletor* pDeletor = nullptr) {
        SafeArrayLock();

        for (auto it = m_List.begin(); it != m_List.end(); ++it) {
            IXObject* pData = *it;
            if (pDeletor) {
                pDeletor->Delete(pData);
            } else if (pData) {
                delete pData;
            }
            if (!m_Queue.empty()) {
                m_Queue.pop();
            }
        }

        m_List.clear();
        SafeArrayUnLock();
    }

    void Push(IXObject* pObj) {
        SafeArrayLock();
        m_Queue.push(pObj);
        SafeArrayUnLock();
    }

    IXObject* Pop() {
        SafeArrayLock();
        if (m_Queue.empty()) {
            SafeArrayUnLock();
            return nullptr;
        }
        IXObject* pObj = m_Queue.front();
        m_Queue.pop();
        SafeArrayUnLock();
        return pObj;
    }

    void AddToList(IXObject* pObj) {
        SafeArrayLock();
        m_List.push_back(pObj);
        SafeArrayUnLock();
    }

    void RemoveFromList(IXObject* pObj) {
        SafeArrayLock();
        m_List.remove(pObj);
        SafeArrayUnLock();
    }

    bool IsEmpty() const {
        return m_Queue.empty();
    }

    size_t GetSize() const {
        return m_Queue.size();
    }

    std::list<IXObject*>& GetList() { return m_List; }
    const std::list<IXObject*>& GetList() const { return m_List; }

protected:
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
    std::queue<IXObject*> m_Queue;
    std::list<IXObject*> m_List;
    bool m_bThreadSafe;
    int m_nCurMaxSize;
};

// ============================================================================
// TXMonsterCreator - Monster creator for object pool
// IDA @ 0x1403650E0
// ============================================================================

class TXMonsterCreator : public TXPool<IXObject>::TXCreator<CMonster> {
public:
    TXMonsterCreator();
    virtual ~TXMonsterCreator() = default;

    // IDA @ 0x140365530 - Creates CMonster via VisGame_cl::CreateEntity
    // Note: Returns IXObject* to match base class signature (covariant return not possible with incomplete type)
    IXObject* Create() override;

    // Convenience method for direct CMonster creation
    CMonster* CreateMonster();
};

// ============================================================================
// TXMonsterDeletor - Monster deletor for object pool
// IDA @ 0x140365140
// ============================================================================

class TXMonsterDeletor : public TXPool<IXObject>::IXDeletor {
public:
    TXMonsterDeletor();
    virtual ~TXMonsterDeletor() = default;

    // Delete monster from pool
    void Delete(IXObject* pObj) override;
};
