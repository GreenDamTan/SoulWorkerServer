#pragma once

#include <cstddef>
#include <cstdint>
#include <new>
#include <type_traits>

namespace ATL {
template <typename T>
struct CElementTraits {};

struct CAtlPlex {
    CAtlPlex* m_pNext = nullptr;

    static CAtlPlex* Create(CAtlPlex** head, unsigned int blockSize, std::size_t elementSize) {
        const std::size_t allocationSize =
            sizeof(CAtlPlex) + static_cast<std::size_t>(blockSize) * elementSize;
        auto* block = static_cast<CAtlPlex*>(::operator new(allocationSize));
        block->m_pNext = *head;
        *head = block;
        return block;
    }

    static void FreeDataChain(CAtlPlex* block) {
        while (block) {
            CAtlPlex* next = block->m_pNext;
            ::operator delete(block);
            block = next;
        }
    }

    void* GetData() {
        return reinterpret_cast<char*>(this) + sizeof(CAtlPlex);
    }
};

template <typename KeyType,
          typename ValueType,
          typename KeyTraits = CElementTraits<KeyType>,
          typename ValueTraits = CElementTraits<ValueType>>
class CAtlMap {
public:
    struct CPair {
        KeyType m_key{};
        ValueType m_value{};
    };

    struct CNode : CPair {
        CNode* m_pNext = nullptr;
        unsigned int m_nHash = 0;
    };

    CAtlMap() {
        RefreshThresholds(m_nBins);
    }

    ~CAtlMap() {
        RemoveAll();
    }

    bool InitHashTable(unsigned int nBins, bool bAllocNow) {
        if (m_ppBins) {
            delete[] m_ppBins;
            m_ppBins = nullptr;
        }

        if (bAllocNow) {
            m_ppBins = new (std::nothrow) CNode*[nBins]();
            if (!m_ppBins) {
                return false;
            }
        }

        m_nBins = nBins;
        RefreshThresholds(m_nBins);
        return true;
    }

    CNode* LookupNode(KeyType key) const {
        if (!m_ppBins || m_nBins == 0) {
            return nullptr;
        }

        const unsigned int hash = static_cast<unsigned int>(key);
        CNode* node = m_ppBins[hash % m_nBins];
        while (node) {
            if (node->m_nHash == hash && node->m_key == key) {
                return node;
            }
            node = node->m_pNext;
        }
        return nullptr;
    }

    ValueType* Lookup(KeyType key) const {
        CNode* node = LookupNode(key);
        return node ? const_cast<ValueType*>(&node->m_value) : nullptr;
    }

    ValueType& operator[](KeyType key) {
        if (!m_ppBins && !InitHashTable(m_nBins, true)) {
            throw std::bad_alloc();
        }

        if (CNode* node = LookupNode(key)) {
            return node->m_value;
        }

        const unsigned int hash = static_cast<unsigned int>(key);
        const unsigned int binIndex = hash % m_nBins;
        return NewNode(key, binIndex, hash)->m_value;
    }

    bool SetAt(KeyType key, ValueType value) {
        if (!m_ppBins && !InitHashTable(m_nBins, true)) {
            return false;
        }

        if (CNode* node = LookupNode(key)) {
            node->m_value = value;
            return true;
        }

        const unsigned int hash = static_cast<unsigned int>(key);
        NewNode(key, hash % m_nBins, hash)->m_value = value;
        return true;
    }

    bool RemoveKey(KeyType key) {
        if (!m_ppBins || m_nBins == 0) {
            return false;
        }

        const unsigned int hash = static_cast<unsigned int>(key);
        CNode** link = &m_ppBins[hash % m_nBins];
        while (*link) {
            CNode* node = *link;
            if (node->m_nHash == hash && node->m_key == key) {
                *link = node->m_pNext;
                node->~CNode();
                node->m_pNext = m_pFree;
                m_pFree = node;
                --m_nElements;
                return true;
            }
            link = &node->m_pNext;
        }
        return false;
    }

    void RemoveAtPos(CNode* node) {
        if (node) {
            RemoveKey(node->m_key);
        }
    }

    std::uint64_t GetCount() const {
        return m_nElements;
    }

    CNode* Begin() const {
        return GetHeadPosition();
    }

    CNode* GetValueAt(CNode* node) const {
        return node;
    }

    KeyType GetKeyAt(CNode* node) const {
        return node ? node->m_key : KeyType{};
    }

    CNode* GetHeadPosition() const {
        if (!m_ppBins) {
            return nullptr;
        }

        for (unsigned int i = 0; i < m_nBins; ++i) {
            if (m_ppBins[i]) {
                return m_ppBins[i];
            }
        }
        return nullptr;
    }

    CNode* GetNext(CNode* node) const {
        if (!node) {
            return nullptr;
        }

        if (node->m_pNext) {
            return node->m_pNext;
        }

        if (!m_ppBins || m_nBins == 0) {
            return nullptr;
        }

        for (unsigned int binIndex = (node->m_nHash % m_nBins) + 1; binIndex < m_nBins; ++binIndex) {
            if (m_ppBins[binIndex]) {
                return m_ppBins[binIndex];
            }
        }
        return nullptr;
    }

    void RemoveAll() {
        if (m_ppBins) {
            delete[] m_ppBins;
            m_ppBins = nullptr;
        }

        m_nElements = 0;
        if (m_pBlocks) {
            CAtlPlex::FreeDataChain(m_pBlocks);
            m_pBlocks = nullptr;
        }
        m_pFree = nullptr;
        RefreshThresholds(m_nBins);
    }

    CNode** m_ppBins = nullptr;
    std::uint64_t m_nElements = 0;
    unsigned int m_nBins = 17;
    float m_fOptimalLoad = 0.75f;
    float m_fLoThreshold = 0.25f;
    float m_fHiThreshold = 2.25f;
    std::uint64_t m_nHiRehashThreshold = 38;
    std::uint64_t m_nLoRehashThreshold = 0;
    unsigned int m_nLockCount = 0;
    unsigned int m_nBlockSize = 10;
    CAtlPlex* m_pBlocks = nullptr;
    CNode* m_pFree = nullptr;

private:
    static unsigned int PickSize(std::uint64_t elementCount) {
        static constexpr unsigned int kPrimes[] = {
            17u,         29u,         47u,         97u,         191u,        257u,        521u,
            769u,        1543u,       3079u,       6151u,       12289u,      24593u,      49157u,
            98317u,      196613u,     393241u,     786433u,     1572869u,    3145739u,    6291469u,
            12582917u,   25165843u,   50331653u,   100663319u,  201326611u,  402653189u,  805306457u,
            1610612741u, 0xFFFFFFFFu};

        for (unsigned int prime : kPrimes) {
            if (prime == 0xFFFFFFFFu || elementCount <= prime) {
                return prime == 0xFFFFFFFFu ? static_cast<unsigned int>(elementCount) : prime;
            }
        }
        return static_cast<unsigned int>(elementCount);
    }

    void RefreshThresholds(unsigned int nBins) {
        m_nHiRehashThreshold = static_cast<std::uint64_t>(static_cast<float>(nBins) * m_fHiThreshold);

        const std::uint64_t loThreshold =
            static_cast<std::uint64_t>(static_cast<float>(nBins) * m_fLoThreshold);
        m_nLoRehashThreshold = loThreshold < 0x11 ? 0 : loThreshold;
    }

    void Rehash(unsigned int nBins) {
        if (nBins == 0) {
            nBins = 17;
        }

        CNode** oldBins = m_ppBins;
        const unsigned int oldBinCount = m_nBins;

        m_ppBins = new (std::nothrow) CNode*[nBins]();
        if (!m_ppBins) {
            m_ppBins = oldBins;
            return;
        }

        m_nBins = nBins;
        RefreshThresholds(m_nBins);

        if (oldBins) {
            for (unsigned int binIndex = 0; binIndex < oldBinCount; ++binIndex) {
                CNode* node = oldBins[binIndex];
                while (node) {
                    CNode* next = node->m_pNext;
                    const unsigned int newBinIndex = node->m_nHash % m_nBins;
                    node->m_pNext = m_ppBins[newBinIndex];
                    m_ppBins[newBinIndex] = node;
                    node = next;
                }
            }

            delete[] oldBins;
        }
    }

    void AllocateBlock() {
        CAtlPlex* block = CAtlPlex::Create(&m_pBlocks, m_nBlockSize, sizeof(CNode));
        auto* data = static_cast<char*>(block->GetData());
        for (int i = static_cast<int>(m_nBlockSize) - 1; i >= 0; --i) {
            auto* node = reinterpret_cast<CNode*>(data + static_cast<std::size_t>(i) * sizeof(CNode));
            node->m_pNext = m_pFree;
            m_pFree = node;
        }
    }

    CNode* NewNode(KeyType key, unsigned int binIndex, unsigned int hash) {
        if (!m_pFree) {
            AllocateBlock();
        }

        CNode* node = m_pFree;
        m_pFree = node->m_pNext;

        ::new (node) CNode();
        node->m_key = key;
        node->m_nHash = hash;
        node->m_pNext = m_ppBins[binIndex];
        m_ppBins[binIndex] = node;

        ++m_nElements;
        if (m_nElements > m_nHiRehashThreshold && !m_nLockCount) {
            Rehash(PickSize(m_nElements));
        }
        return node;
    }
};
} // namespace ATL

template <typename KeyType, typename ValueType, typename KeyTraits = ATL::CElementTraits<KeyType>>
struct TXMap {
    ATL::CAtlMap<KeyType,
                 ValueType,
                 ATL::CElementTraits<KeyType>,
                 ATL::CElementTraits<ValueType>>
        m_AtlMap;

    // 对齐 IDA: TXMap::GetAt - 查找键对应的值
    // IDA 0x1400014F0 显示 TXMap<int,IXObject*>::GetAt 返回 IXObject*，
    // 而非底层 CAtlMap::Lookup 返回的 IXObject**。
    using LookupResult = std::conditional_t<std::is_pointer_v<ValueType>,
                                            ValueType,
                                            ValueType*>;

    LookupResult GetAt(KeyType key) {
        if constexpr (std::is_pointer_v<ValueType>) {
            ValueType* value = m_AtlMap.Lookup(key);
            return value ? *value : nullptr;
        } else {
            return m_AtlMap.Lookup(key);
        }
    }

    LookupResult GetAt(KeyType key) const {
        if constexpr (std::is_pointer_v<ValueType>) {
            ValueType* value = m_AtlMap.Lookup(key);
            return value ? *value : nullptr;
        } else {
            return m_AtlMap.Lookup(key);
        }
    }

    bool SetAt(KeyType key, ValueType value) {
        return m_AtlMap.SetAt(key, value);
    }

    bool RemoveKey(KeyType key) {
        return m_AtlMap.RemoveKey(key);
    }

    void RemoveAtPos(typename ATL::CAtlMap<KeyType, ValueType>::CNode* node) {
        m_AtlMap.RemoveAtPos(node);
    }

    void RemoveAll() {
        m_AtlMap.RemoveAll();
    }

    int GetCount() const {
        return static_cast<int>(m_AtlMap.GetCount());
    }

    typename ATL::CAtlMap<KeyType, ValueType>::CNode* Begin() const {
        return m_AtlMap.GetHeadPosition();
    }

    void GetNext(typename ATL::CAtlMap<KeyType, ValueType>::CNode** node) const {
        if (node) {
            *node = m_AtlMap.GetNext(*node);
        }
    }

    ValueType* GetValueAt(typename ATL::CAtlMap<KeyType, ValueType>::CNode* node) const {
        return node ? &node->m_value : nullptr;
    }

    KeyType* GetKeyAt(typename ATL::CAtlMap<KeyType, ValueType>::CNode* node) const {
        return node ? &node->m_key : nullptr;
    }
};

namespace TXMapUtil {
template <typename MapType, typename PointerType>
void DeletePtr(MapType& map) {
    using AtlMapType = std::remove_reference_t<decltype(map.m_AtlMap)>;
    using NodeType = typename AtlMapType::CNode;

    NodeType* node = map.m_AtlMap.GetHeadPosition();
    while (node) {
        NodeType* next = map.m_AtlMap.GetNext(node);
        delete node->m_value;
        node->m_value = nullptr;
        node = next;
    }

    map.m_AtlMap.RemoveAll();
}
} // namespace TXMapUtil
